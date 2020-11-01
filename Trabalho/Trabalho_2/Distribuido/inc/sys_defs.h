#ifndef SYS_DEFS__HPP
#define SYS_DEFS__HPP
#include <pthread.h>


#define I2C_TERMPERATURE   0x76
#define I2C_LED            0x27

#ifndef __LED_CONTROLL__
#define __LED_CONTROLL__
int LED_CONTROL = -1;
#endif

#ifndef __PTHREAD_CONTROLL__
#define __PTHREAD_CONTROLL__
pthread_t control_thread;
#endif



#ifndef __MUTEX_CONTROLL__
#define __MUTEX_CONTROLL__
pthread_mutex_t mutex_temperature_choice;
pthread_mutexattr_t mutex_temperature_choice_attr;
#endif



#ifndef _TEMPERATURE_POTENCIOMETER_CONTROLL__
#define _TEMPERATURE_POTENCIOMETER_CONTROLL__
float  extern_temperature_global = -1;
double intern_temperature_global = -1;
float potenciometer_global = -1;
float informed_temperature_global = -1;
char read_type_global = 'n';
#endif


#ifndef __UART_0_CONTROLL__
#define __UART_0_CONTROLL__
int uart0_filestream = -1;
#endif

#ifndef __BME_CONTROLL__
#define __BME_CONTROLL__
struct bme280_dev dev;
struct identifier id;
#endif

#ifndef __GPIO_CONTROLL__
#define __GPIO_CONTROLL__
int fan_control = 1;
int resistor_control = 1;
#endif


#endif