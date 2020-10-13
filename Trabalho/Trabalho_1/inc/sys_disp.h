#ifndef SYS_DISP__HPP
#define SYS_DISP__HPP

#ifdef delayMicroseconds
#undef delayMicroseconds
#endif

#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "bme280.h"
#include "bme280_defs.h"
#include "i2c.h"
#include "gpio.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
//#endif

/******************************************************************************/
/*!                         System header files                               */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

/******************************************************************************/
/*!                         Own header files                                  */
#include "bme280.h"

void init_devices();

double get_intern_temperature();

float get_extern_temperature();
float get_potentiometer();

int set_fan(int);
int set_resistor(int);

void set_lcd_temperatures();

void interrpt_signal(int);

#endif