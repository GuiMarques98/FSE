#ifndef I2C__HPP
#define I2C__HPP

#ifdef delayMicroseconds
#undef delayMicroseconds
#endif

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <bcm2835.h>


#include "bme280.h"


// Define some device parameters
#define I2C_LCD_ADDR           0x27 // I2C device address
#define I2C_TEMPERATURE_ADDR   0x76 // I2C device address

// Define some device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0b00000100 // Enable bit

// int SETUP_FD;  // seen by all subroutines 

void lcd_init(int fd);
void lcd_byte(int bits, int mode, int fd);
void lcd_toggle_enable(int bits, int fd);

// added by Lewis
void typeInt(int i, int fd);
void typeFloat(float myFloat, int fd);
void lcdLoc(int line, int fd); //move cursor
void ClrLcd(int fd); // clr LCD return home
void typeln(const char *s, int fd);
void typeChar(char val, int fd);

/******************************************************************************/
/*!                               Structures                                  */

/* Structure that contains identifier details used in example */
struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};

/****************************************************************************/
/*!                         Functions                                       */

void user_delay_us(uint32_t period, void *intf_ptr);

void print_sensor_data(struct bme280_data *comp_data);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

double get_temperature(struct bme280_data *comp_data);

double get_temp_sensor_data_forced_mode(struct bme280_dev *dev);

#endif