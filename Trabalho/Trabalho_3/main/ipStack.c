#include "ipStack.h"

#include <stdio.h>
#include <stdlib.h>
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


static char response[MAX_RESPONSE_LEN];

esp_err_t _http_event_handle_ip_stack(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_ON_HEADER");
        printf("%.*s", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        printf("%.*s", evt->data_len, (char *)evt->data);
        // if (!esp_http_client_is_chunked_response(evt->client))
        // {
        // }

        break;
    case HTTP_EVENT_ON_FINISH:
        esp_http_client_read(evt->client, response, MAX_RESPONSE_LEN);
        xSemaphoreGive(httpEndRequestSemaphore);
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_IP_STACK, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

short get_lat_lon(char *lat, int n_lat, char *lon, int n_lon)
{
    ESP_LOGI(TAG_IP_STACK, "ENTER JSON");
    cJSON *request_json = cJSON_Parse(response);
    if (request_json == NULL) {
        ESP_LOGE(TAG_IP_STACK, "JSON CANT PARSE FROM RESPONSE %s", response);
        return -1;
    }

    cJSON *latitude = cJSON_GetObjectItemCaseSensitive(request_json, "latitude");
    if (latitude == NULL) {
        cJSON_Delete(request_json);
        ESP_LOGE(TAG_IP_STACK, "JSON CANT PARSE LATITUDE FROM RESPONSE %s", response);
        return -1;
    }

    cJSON *longitude = cJSON_GetObjectItemCaseSensitive(request_json, "longitude");
    if (longitude == NULL) {
        cJSON_Delete(request_json);
        ESP_LOGE(TAG_IP_STACK, "JSON CANT PARSE LONGITUDE FROM RESPONSE %s", response);
        return -1;
    }

    // Copy char value of lon and lat
    snprintf(lat, n_lat, "%lf", (latitude->valuedouble));
    snprintf(lon, n_lon, "%lf", (longitude->valuedouble));

    cJSON_Delete(request_json);
    ESP_LOGI(TAG_IP_STACK, "Latitude %s Longitude %s", lat, lon);
    // strncpy(lat, latitude->valuestring, n_lat);
    // strncpy(lon, longitude->valuestring, n_lon);


    return 0;
}

void http_request_ip_stack(char* lat, int nlat, char* lon, int nlon)
{

    EventBits_t bits = xEventGroupWaitBits(s_esp_event_group,
                                        WIFI_CONNECTED_BIT,
                                        pdFALSE,
                                        pdFALSE,
                                        portMAX_DELAY);

    // Creating url
    char url[300];
    extern char ip_address[20];

    /* https://ipstack.com/quickstart */
    sprintf(url, "http://api.ipstack.com/%s?access_key=%s&output=json&fields=latitude,longitude", ip_address, API_IP_STACK);

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle_ip_stack,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_IP_STACK, "Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);

    // Wait until end of the http request
    xSemaphoreTake(httpEndRequestSemaphore, portMAX_DELAY);
    get_lat_lon(lat, nlat, lon, nlon);
    xEventGroupSetBits(s_esp_event_group, IP_STACK_COMPLETED_BIT);

    
}
