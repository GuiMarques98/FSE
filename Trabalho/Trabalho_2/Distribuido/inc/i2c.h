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

/******************************************************************************/
/*!                               Structures                                  */
typedef struct bme_env {
    float temp;
    float hum;
    int err;
}bme_env_t;

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

bme_env_t get_temperature(struct bme280_data *comp_data);

bme_env_t get_temp_sensor_data_forced_mode(struct bme280_dev *dev);

bme_env_t stream_sensor_data_normal_mode(struct bme280_dev *dev);

#endif