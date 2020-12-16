#ifndef SERVER_CONF__H
#define SERVER_CONF__H

#include <stdio.h>
#include <signal.h>

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include <string>
#include <iostream>
#include <sstream>
#include <stdarg.h>

#include "cJSON.h"


#define MAX_JSON_STRING 5000
#define DIST_PORT 10117
#define CENTRAL_PORT 10017
#define DIST_IP "192.168.0.52"
#define CENTRAL_IP "192.168.0.53"
#define MAX_DEVICE 6
#define QUEUE_LEN 10


void start_all();

void init_menu();

void menu_cont();
void* get_env(void*);

void print_err_handle();
int get_json(int*);
void play_alarm();
int control_device(int*, int*);
void apply_choice(int);
void* alarm_play_thread(void*);

void close_menu();

void send_command();
void interrupt(int);

void init_maintain_data();
void maintain_data_csv();

#endif