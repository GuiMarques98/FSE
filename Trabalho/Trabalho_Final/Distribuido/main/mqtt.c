#include "mqtt.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/semphr.h"

// #include "MQTT" "MQTT"

#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "cJSON.h"

char comodo_name[MAX_LENGTH];
esp_mqtt_client_handle_t client;
nvs_handle partition_handler;

extern char mac_address[50];

extern xSemaphoreHandle conexaoWifiSemaphore, mqttRequestInitialization, mqttInicializationFinished;

static esp_err_t mqtt_topico_handler(char *topico, int topico_len, char *data, int data_len)
{
    // data[data_len+1] = 0;
    // topico[topico_len] = 0;
    // ESP_LOGI("MQTT", "JSON %d %s", data_len, data);
    char topico_if[100], topico_if2[100];
    sprintf(topico_if, "fse2020/160029503/dispositivos/%s", mac_address);
    sprintf(topico_if2, "%.*s", topico_len, topico);
    cJSON *json = cJSON_Parse(data);

    if (json == NULL)
    {
        ESP_LOGE("MQTT", "JSON INVALIDO %s", data);
        xSemaphoreGive(mqttRequestInitialization);
        return ESP_ERR_NOT_FOUND;
    }
    ESP_LOGI("MQTT", "Topico %.*s\r", topico_len, topico);
    ESP_LOGI("MQTT", "Topico 2 %s", topico_if);
    if (!strcmp(topico_if, topico_if2))
    {
        cJSON *json_topico = cJSON_GetObjectItemCaseSensitive(json, "comodo");
        ESP_LOGI("MQTT", "Json recebido e comodo é %s", json_topico->valuestring);
        sprintf(comodo_name, "fse2020/160029503/%s", json_topico->valuestring);
        ESP_LOGI("MQTT", "Topido do comodo é %s", comodo_name);
        
        nvs_set_str(partition_handler, "name", json_topico->valuestring);
        xSemaphoreGive(mqttRequestInitialization);
    }
    return ESP_OK;
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI("MQTT", "MQTT_EVENT_CONNECTED");
        char topico[100];
        sprintf(topico, "fse2020/160029503/dispositivos/%s", mac_address);
        esp_mqtt_client_subscribe(client, topico, 0);
        xSemaphoreGive(mqttRequestInitialization);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI("MQTT", "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI("MQTT", "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI("MQTT", "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI("MQTT", "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI("MQTT", "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI("MQTT", "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        return mqtt_topico_handler(event->topic, event->topic_len, event->data, event->data_len);
        // break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI("MQTT", "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI("MQTT", "Other event id:%d", event->event_id);
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD("MQTT", "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

int get_comodo()
{
    ESP_LOGI("MQTT", "Pegando o comodo da pessoa");

    esp_err_t ret = nvs_flash_init_partition(PARTITION_NAME);
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase_partition(PARTITION_NAME));
        ret = nvs_flash_init_partition(PARTITION_NAME);
    }
    ESP_ERROR_CHECK(ret);

 
    esp_err_t res_nvs = nvs_open_from_partition(PARTITION_NAME, "comodo", NVS_READWRITE, &partition_handler);
    char name_request[20];
    size_t length_req = 20;
    esp_err_t res = nvs_get_str(partition_handler, "name", name_request, &length_req);
    ESP_LOGI("MQTT", "Chegou no if (%x)", res);

    if (res != ESP_OK)
    {
        cJSON *mac_obj = cJSON_CreateObject();
        cJSON *mac_string = cJSON_CreateString(mac_address);
        cJSON_AddItemToObject(mac_obj, "id", mac_string);

        char *mac_request = cJSON_PrintUnformatted(mac_obj);
        mqtt_sent_message("fse2020/160029503/dispositivos", mac_request);
        cJSON_Delete(mac_obj);

        xSemaphoreTake(mqttRequestInitialization, portMAX_DELAY);
    }
    else if (res == ESP_OK)
    {
        sprintf(comodo_name, "fse2020/160029503/%s", name_request);
    }
    return 0;
}

void mqtt_start()
{
    esp_mqtt_client_config_t mqtt_config = {
        .uri = MQTT_BROKER_URL,
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    xSemaphoreTake(mqttRequestInitialization, portMAX_DELAY);
    get_comodo();
    xSemaphoreGive(mqttInicializationFinished);
}

void mqtt_sent_message(char *topico, char *mensage)
{
    int message_id = esp_mqtt_client_publish(client, topico, mensage, 0, 1, 0);
    ESP_LOGI("MQTT", "Mensagem enviada, ID: %d", message_id);
}
