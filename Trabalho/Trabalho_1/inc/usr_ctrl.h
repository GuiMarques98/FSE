#ifndef USR_CTRL__HPP
#define USR_CTRL__HPP

#include <string.h>
#include "sys_disp.h"


void start();

void init_menu();

void menu();
void print_str(char *text, int type);
int print_options(int);
void set_potenciometer_ctrl();
void set_temperature_ctrl();

void set_keyboard_ctrl();

void interrupt_signal(int);

void close_menu();

#endif