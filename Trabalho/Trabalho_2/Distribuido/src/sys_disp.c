#include "../inc/sys_disp.h"

#include "../inc/sys_defs.h"


void alarm_handler(int signal) {
    alarm(2);
    write_csv("doc/log.csv");
    // pause();
}

void write_csv(char *f) {
    // open csv file
    FILE * csv_f = fopen(f, "a");
    
    // Get current data
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(csv_f, "\"%d-%02d-%02d %02d:%02d:%02d\",\"%.2f\",\"%.2f\"\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, extern_temperature_global, intern_temperature_global);

    // write in csv file
    fclose(csv_f);
}

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
    bme_env_t temp = get_temp_sensor_data_forced_mode(&dev);
    return temp;
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
