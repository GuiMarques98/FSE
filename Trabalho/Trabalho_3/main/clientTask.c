#include "clientTask.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi.h"
#include "openWether.h"
#include "ipStack.h"
#include "sysdefs.h"
#include "publicIp.h"

#define TASK_TAG_GPIO "GPIO TASK"
#define TASK_TAG_NETWORK "NETWORK TASK"

extern xSemaphoreHandle conexaoWifiSemaphore, httpIpStackSemaphore, httpEndRequestSemaphore;
extern EventGroupHandle_t s_esp_event_group;

void start_program_client()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initing led
    gpio_pad_select_gpio(GPIO_LED);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED, LED_HIGH);

    // Creating event handlers (Mutex, semaphore, event groups etc)
    s_esp_event_group = xEventGroupCreate();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    httpIpStackSemaphore = xSemaphoreCreateBinary();
    httpEndRequestSemaphore = xSemaphoreCreateBinary();

    wifi_start();

    xTaskCreate(&gpioTask, "Controla GPIO", 1024, "LED", 2, NULL);
    xTaskCreate(&networkTask, "Requisicao", 8192, "Network", 2, NULL);
}

void gpioTask(void *params)
{
    const int ms_time = 400;
    while (true)
    {
        // Wait to blink
        EventBits_t bits = xEventGroupWaitBits(s_esp_event_group, WIFI_COMPLETED_BIT | IP_STACK_COMPLETED_BIT | OPEN_WETHER_COMPLETED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

        if (!(bits & OPEN_WETHER_COMPLETED_BIT || bits & IP_STACK_COMPLETED_BIT) && bits | WIFI_COMPLETED_BIT)
        {
            continue;
        }
        // Blink
        gpio_set_level(GPIO_LED, LED_LOW);
        vTaskDelay(ms_time / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_LED, LED_HIGH);
        vTaskDelay(ms_time / portTICK_PERIOD_MS);

        if (bits & IP_STACK_COMPLETED_BIT)
        {
            xEventGroupClearBits(s_esp_event_group, IP_STACK_COMPLETED_BIT);
        }

        if (bits & OPEN_WETHER_COMPLETED_BIT)
        {
            xEventGroupClearBits(s_esp_event_group, OPEN_WETHER_COMPLETED_BIT);
        }
    }
}

void networkTask(void *params)
{
    const int minute_delay = 5;
    // const int connection_delay = minute_delay * 60 * 1000;
    const int connection_delay = 10 * 1000;
    char lat[MAX_CHAR], lon[MAX_CHAR];
    extern char ip_address[20];
    while (true)
    {
        http_request_public_ip();
        ESP_LOGI(TASK_TAG_NETWORK, "Public IP request finished IP: %s", ip_address);
        http_request_ip_stack(lat, MAX_CHAR, lon, MAX_CHAR);
        ESP_LOGI(TASK_TAG_NETWORK, "IP STACK finished Latitude: %s Longitude: %s", lat, lon);
        http_request_wether(lat, MAX_CHAR, lon, MAX_CHAR);
        vTaskDelay(connection_delay / portTICK_PERIOD_MS);
    }
}
