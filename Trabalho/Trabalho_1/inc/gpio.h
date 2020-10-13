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


#define RESISTOR RPI_V2_GPIO_P1_16
#define FAN RPI_V2_GPIO_P1_18
#define PWM_CHANNEL 0
#define RANGE 1024

// int PWM_RESISTOR_CONTROL;
// int PWM_FAN_CONTROL;

void config_gpio();
void config_pwm(int, int);
void set_pwm(int);



#endif
