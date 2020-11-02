#ifndef SERVER_CONTROL__HPP
#define SERVER_CONTROL__HPP

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h> 

#include "cJSON.h"
#include "sys_disp.h"

#define MAX_JSON_STRING 5000
#define SERVER_PORT 10117
#define QUEUE_LEN 5

typedef struct server_arg {
    int *fd;
    struct sockaddr_in *addr;
}server_arg_t;

void start_server();

void init_server();

void alarm_signal(int);
void send_data();
void send_alarm();
void get_data();

void* server_listener(void*);
void* server_device(void*);

void close_server();

#endif