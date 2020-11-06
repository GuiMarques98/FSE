#include "../inc/sys_disp.h"

#include "../inc/sys_defs.h"

struct bme280_dev dev;
struct identifier id;

bme_env_t env_global = {-100, -100, 5};
int alarm_house = 0;
const int hysteresis = 2;

void init_devices() {
    // Init GPIO
    init_gpio();

    // Intern Temperature init
    const char i2c_path[] = "/dev/i2c-1";

    id.fd = open(i2c_path, O_RDWR);
    if(id.fd == -1) {
        fprintf( stderr, "Error: I2C not working\n");
        return;
    }

    id.dev_addr = BME280_I2C_ADDR_PRIM;
    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    /* Make sure to select BME280_I2C_ADDR_PRIM or BME280_I2C_ADDR_SEC as needed */
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;
    dev.intf_ptr = &id;

    int rslt = BME280_OK;
    rslt = bme280_init(&dev);

    if (rslt != BME280_OK) {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }
}

void interrpt_signal(int signal) {
    close(id.fd);
    close_gpio();
    exit(0);
}

bme_env_t get_temperature_house() {
    // bme_env_t temp = stream_sensor_data_normal_mode(&dev);
    bme_env_t temp = get_temp_sensor_data_forced_mode(&dev);
    temp.err = is_bme_wrong(temp);

    if(env_global.err > 3)
        temp.err = env_global.err;
    switch(temp.err) {
        case 0:
            env_global = temp;
            break;
        case 1:
            env_global.hum = temp.hum;
            env_global.err = temp.err;
            break;
        case 2:
            env_global.temp = temp.temp;
            env_global.err = temp.err;
            break;
        // default:
            // Everything is wrong
    }

    return env_global;
}


/*
If BME read is okay return 0
else return 1 if only temperature is wrong
return 2 if only humidity is wrong
return 3 if both are wrong
*/
int is_bme_wrong(bme_env_t env) {
    int err_pres = 2, err = 0;
    // printf("This is ENV the temperature %.2f and this humidity %.2f\n", env.temp, env.hum);
    if(env_global.err > 3) {
        env_global.err--;
        return 0;
    }
    if(env_global.temp == -100.0 || env_global.hum == -100.0)
        return 0;
    if((env_global.temp - err_pres) > env.temp || (env_global.temp - err_pres) > env.temp)
        err++;

    if((env_global.hum - err_pres) > env.hum || (env_global.hum - err_pres) > env.hum) {
        if(err)
            return 3;
        return 2;
    }

    return err;
}

void turn_lamp(uint8_t lamp, int data) {
    bcm2835_gpio_write(lamp, data);
}

void turn_on_lamp(uint8_t lamp) {
    bcm2835_gpio_write(lamp, 1);
}

void turn_off_lamp(uint8_t lamp) {
    bcm2835_gpio_write(lamp, 0);
}

void turn_on_all_lamps() {
    bcm2835_gpio_write(LAMP_KITCHEN, 1);
    bcm2835_gpio_write(LAMP_ROOM, 1);
    bcm2835_gpio_write(LAMP_BEDROOM_1, 1);
    bcm2835_gpio_write(LAMP_BEDROOM_2, 1);
}

void turn_off_all_lamps() {
    bcm2835_gpio_write(LAMP_KITCHEN, 0);
    bcm2835_gpio_write(LAMP_ROOM, 0);
    bcm2835_gpio_write(LAMP_BEDROOM_1, 0);
    bcm2835_gpio_write(LAMP_BEDROOM_2, 0);
}


void turn_air(uint8_t air, int data) {
    bcm2835_gpio_write(air, data);
}

void turn_on_air(uint8_t air) {
    bcm2835_gpio_write(air, 1);
}

void turn_off_air(uint8_t air) {
    bcm2835_gpio_write(air, 0);
}

void turn_om_all_airs() {
    bcm2835_gpio_write(AIR_CONDITIONING_1, 1);
    bcm2835_gpio_write(AIR_CONDITIONING_2, 1);
}

void turn_off_all_airs() {
    bcm2835_gpio_write(AIR_CONDITIONING_1, 0);
    bcm2835_gpio_write(AIR_CONDITIONING_2, 0);
}

int detect_presence(uint8_t disp) {
    return bcm2835_gpio_lev(disp);
}

int detect_any_presence() {
    return bcm2835_gpio_lev(PRESENCE_ROOM) ||
    bcm2835_gpio_lev(PRESENCE_KITCHEN) ||
    bcm2835_gpio_lev(OPENING_DOOR_KITCHEN) ||
    bcm2835_gpio_lev(OPENING_WINDOW_KITCHEN) ||
    bcm2835_gpio_lev(OPENING_DOOR_ROOM) ||
    bcm2835_gpio_lev(OPENING_WINDOW_ROOM) ||
    bcm2835_gpio_lev(OPENING_WINDOW_BEDROOM_1) ||
    bcm2835_gpio_lev(OPENING_WINDOW_BEDROOM_2);
}
