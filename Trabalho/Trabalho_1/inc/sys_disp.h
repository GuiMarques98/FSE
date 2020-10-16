#ifndef SYS_DISP__HPP
#define SYS_DISP__HPP

#ifdef delayMicroseconds
#undef delayMicroseconds
#endif

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>
#include "bme280.h"
#include "bme280_defs.h"
#include "i2c.h"
#include "gpio.h"

void init_devices();
double get_intern_temperature();

float get_extern_temperature();
float get_potentiometer();

int set_fan(int);
int set_resistor(int);

void set_lcd_temperatures();

void* controller(void*);
void alarm_handler(int);
void interrpt_signal(int);

void write_csv(char *f);
void warm();
void cold();
void turn_off_fan_resistor();

#endif