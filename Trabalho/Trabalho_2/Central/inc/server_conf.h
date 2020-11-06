#ifndef SERVER_CONF__H
#define SERVER_CONF__H

#include <signal.h>
#include <stdio.h>

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#include <string>
#include <iostream>
#include <sstream>

#include "cJSON.h"


#define MAX_JSON_STRING 5000
#define DIST_PORT 10117
#define CENTRAL_PORT 10017
#define DIST_IP "192.168.0.52"
#define CENTRAL_IP "192.168.0.53"
#define MAX_DEVICE 6
#define QUEUE_LEN 5


void start_all();

void init_menu();

void menu_cont();
void* get_env(void*);

int get_json(int*);
void play_alarm();
int control_device(int*, int*);
void apply_choice(int);


void close_menu();

void send_command(int);
void interrupt(int);

#endif