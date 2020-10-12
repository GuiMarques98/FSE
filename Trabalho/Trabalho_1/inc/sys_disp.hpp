#ifndef SYS_DISP__HPP
#define SYS_DISP__HPP

#undef delayMicroseconds

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "bme280.h"
#include "bme280_defs.h"

#define I2C_TERMPERATURE   0x76
#define I2C_LED            0x27

float get_intern_temperature();

float get_extern_temperature();
float get_potentiometer();

int set_fan(int);
int set_resistor(int);

void interrpt_signal(int);

#endif