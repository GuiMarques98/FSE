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
#define CENTRAL_PORT 10017
#define SERVER_IP "192.168.0.52"
#define CENTRAL_IP "192.168.0.53"
#define QUEUE_LEN 5

typedef struct server_arg {
    int *fd;
    struct sockaddr_in *addr;
}server_arg_t;

void start_server();

void init_server();

void alarm_signal(int);
void send_data();
void get_data();

void* server_listener(void*);
void* server_device(void*);

int get_json(int*, double*, double*);
int send_alarm();
void close_signal(int signal);
void close_server();

#endif