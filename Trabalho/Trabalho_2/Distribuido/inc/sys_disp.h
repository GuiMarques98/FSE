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
bme_env_t get_temperature_house();

void alarm_handler(int);
void interrpt_signal(int);

void write_csv(char *f);

void turn_on_lamp(uint8_t);
void turn_off_lamp(uint8_t);
void turn_on_all_lamps();
void turn_off_all_lamps();

#endif