#ifndef CLIENT_TASK__H
#define CLIENT_TASK__H

#define GPIO_LED 2
#define WIFI_DISCONNECT BIT0
#define HTTP_REQUEST BIT1

#define LED_HIGH 1
#define LED_LOW 0

#define MAX_CHAR 300

void start_program_client();

void gpioTask(void* params);
void networkTask(void* params);

#endif