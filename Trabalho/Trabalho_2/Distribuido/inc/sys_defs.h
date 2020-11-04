#ifndef SYS_DEFS__HPP
#define SYS_DEFS__HPP
#include <pthread.h>


#define I2C_TERMPERATURE   0x76
#define I2C_LED            0x27


// #ifndef __PTHREAD_CONTROLL__
// #define __PTHREAD_CONTROLL__
// pthread_t control_thread[2];
// #endif



// #ifndef __MUTEX_CONTROLL__
// #define __MUTEX_CONTROLL__
// pthread_mutex_t mutex_temperature_choice;
// pthread_mutexattr_t mutex_temperature_choice_attr;
// #endif



// #ifndef _TEMPERATURE_POTENCIOMETER_CONTROLL__
// #define _TEMPERATURE_POTENCIOMETER_CONTROLL__
// float  temperature = -1;
// float  humidity = -1;
// int alarm_house = 0;
// #endif


// #ifndef __BME_CONTROLL__
// #define __BME_CONTROLL__
// struct bme280_dev dev;
// struct identifier id;
// #endif



#endif