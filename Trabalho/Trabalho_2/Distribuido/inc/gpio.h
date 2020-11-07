#ifndef GPIO__HPP
#define GPIO__HPP

#ifdef __KERNEL__
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <bcm2835.h>


#ifdef delayMicroseconds
#undef delayMicroseconds
#endif

#define LAMP_KITCHEN                17
#define LAMP_ROOM                   18
#define LAMP_BEDROOM_1              27
#define LAMP_BEDROOM_2              22
#define AIR_CONDITIONING_1          23
#define AIR_CONDITIONING_2          24
#define PRESENCE_ROOM               25
#define PRESENCE_KITCHEN            26
#define OPENING_DOOR_KITCHEN        05
#define OPENING_WINDOW_KITCHEN      06
#define OPENING_DOOR_ROOM           12
#define OPENING_WINDOW_ROOM         16
#define OPENING_WINDOW_BEDROOM_1    20
#define OPENING_WINDOW_BEDROOM_2    21

// int PWM_RESISTOR_CONTROL;
// int PWM_FAN_CONTROL;

void init_gpio();
void close_gpio();



#endif
