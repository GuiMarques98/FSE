#include "publicIp.h"

#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "cJSON.h"

#include "wifi.h"
#include "sysdefs.h"

extern xSemaphoreHandle conexaoWifiSemaphore, httpIpStackSemaphore, httpEndRequestSemaphore;

extern EventGroupHandle_t s_esp_event_group;
#ifndef MAX_RESPONSE_LEN
#define MAX_RESPONSE_LEN 1000
#endif
static char response[MAX_RESPONSE_LEN];

esp_err_t _http_event_handle_public_ip(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_ON_HEADER");
        printf("%.*s", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            printf("%.*s", evt->data_len, (char *)evt->data);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        esp_http_client_read(evt->client, response, MAX_RESPONSE_LEN);
        xSemaphoreGive(httpEndRequestSemaphore);
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_PUBLIC_IP, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

short get_public_ip(char *public_ip)
{
    cJSON *request_json = cJSON_Parse(response);

    if (request_json == NULL)
    {
        ESP_LOGE(TAG_PUBLIC_IP, "JSON FAILED REQUEST %s", response);
        return -1;
    }

    cJSON *json_ip = cJSON_GetObjectItemCaseSensitive(request_json, "ip");
    if (json_ip == NULL)
    {
        ESP_LOGE(TAG_PUBLIC_IP, "JSON FAILED FIND IP %s", response);
        cJSON_Delete(request_json);
        return -1;
    }

    // Copy char value of lon and lat
    strcpy(public_ip, json_ip->valuestring);

    cJSON_Delete(request_json);
    ESP_LOGI(TAG_PUBLIC_IP, "JSON SUCCESS %s", response);
    return 0;
}

void http_request_public_ip()
{

    EventBits_t bits = xEventGroupWaitBits(s_esp_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    // Creating url
    char url[] = "https://api.ipify.org/?format=json";

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle_public_ip,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_PUBLIC_IP, "Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    xSemaphoreTake(httpEndRequestSemaphore, portMAX_DELAY);
    extern char ip_address[20];
    get_public_ip(ip_address);
    esp_http_client_cleanup(client);

    // Wait until end of the http request

    xEventGroupSetBits(s_esp_event_group, IP_STACK_COMPLETED_BIT);
}
