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

#include "client.h"

xSemaphoreHandle conexaoWifiSemaphore, mqttRequestInitialization, mqttInicializationFinished;

void app_main(void)
{
    start_program();
}
