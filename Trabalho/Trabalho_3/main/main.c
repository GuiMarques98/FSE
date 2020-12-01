#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

#include "clientTask.h"

xSemaphoreHandle conexaoWifiSemaphore, httpIpStackSemaphore, httpEndRequestSemaphore;
EventGroupHandle_t s_esp_event_group;

void app_main(void) {
    start_program_client();
}
