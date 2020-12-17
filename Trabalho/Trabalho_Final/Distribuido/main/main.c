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

#include "wifi.h"

xSemaphoreHandle conexaoWifiSemaphore, mqttRequestInitialization, mqttInicializationFinished;

void app_main(void)
{
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    mqttRequestInitialization = xSemaphoreCreateBinary();
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        res = nvs_flash_init();
    }
    ESP_ERROR_CHECK(res);

    wifi_start();
    xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY);

}
