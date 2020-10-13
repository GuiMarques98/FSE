#ifndef SYS_DEFS__HPP
#define SYS_DEFS__HPP


#define I2C_TERMPERATURE   0x76
#define I2C_LED            0x27

#ifndef __LED_CONTROLL__
#define __LED_CONTROLL__
int LED_CONTROL = -1;
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


#endif