#include "client.h"

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "mqtt.h"
#include "dht11.h"
#include "wifi.h"

extern xSemaphoreHandle conexaoWifiSemaphore, mqttRequestInitialization, mqttInicializationFinished;

char comodo_name[MAX_LENGTH];

void start_program()
{
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    mqttRequestInitialization = xSemaphoreCreateBinary();
    mqttInicializationFinished = xSemaphoreCreateBinary();

    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        res = nvs_flash_init();
    }
    ESP_ERROR_CHECK(res);

    wifi_start();
    xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY);

    mqtt_start();

    xSemaphoreTake(mqttInicializationFinished, portMAX_DELAY);
    DHT11_init(DHT11_GPIO);

    xTaskCreate(&sendData, "Envia os dados", 8192, "SendData", 2, NULL);
}

void sendData(void *params)
{
    while (1)
    {
        struct dht11_reading dht11_struct = DHT11_read();
        char request_hum[MAX_SRT_LEN], request_tmp[MAX_SRT_LEN];

        sprintf(request_hum, "{\"humidade\":%d,\"status\":%d}", dht11_struct.humidity, dht11_struct.status);
        sprintf(request_tmp, "{\"temperatura\":%d,\"status\":%d}", dht11_struct.temperature, dht11_struct.status);

        ESP_LOGI("SEND DATA", "Temperatura enviada %d, Humidade enviada %d, Status %d", dht11_struct.temperature, dht11_struct.humidity, dht11_struct.status);

        char top_hum[500], top_tmp[500];
        sprintf(top_tmp, "%s/temperatura", comodo_name);
        sprintf(top_hum, "%s/humidade", comodo_name);

        mqtt_sent_message(top_tmp, request_tmp);
        mqtt_sent_message(top_hum, request_hum);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// void getData(void *params) {
//     while(1) {

//     }
// }
