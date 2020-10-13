#include "../inc/sys_disp.h"

#include "../inc/sys_defs.h"



void init_devices() {
    // Setup WiringPI
    if (wiringPiSetup () == -1) {
        fprintf( stderr, "Wiring PI Setup Fail\n");
        return;
    }
    //Init LCD
    LED_CONTROL = wiringPiI2CSetup(I2C_LCD_ADDR);
    lcd_init(LED_CONTROL);

    // Set up UART file
    uart0_filestream = -1;
    const char uart_path[] = "/dev/serial0";

    uart0_filestream = open(uart_path, O_RDWR);
    if (uart0_filestream == -1)
    {
        fprintf( stderr, "Error: Serial port not working\n");
        return;
    }

    // Config UART
    struct termios options;
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    // Flush file and after set the configuration
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    
    if (uart0_filestream == -1)
    {
        fprintf( stderr, "Error: Serial port not working\n");
        return;
    }

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
    close(uart0_filestream);
    close(id.fd);
    exit(0);
}

float get_extern_temperature() {

    // Creating the message to send 0xA1 and registry
    unsigned char buffer[] = {0xa1,9,5,0,3};

    // Send
    int count_w = write(uart0_filestream, buffer, 5);
    if(count_w <= 0)
    {
        fprintf( stderr, "Error: Dont write in uart communication\n");
        return -1;
    }

    usleep(100);
    float buffer_temp = 0.0;

    // Read response from Arduino
    int count_r = read(uart0_filestream, &buffer_temp, 4);

    if(count_r < 0) {
        fprintf( stderr, "Error: Dont read in uart communication\n");
        return -1;
    } else if(count_r == 0) {
        fprintf( stderr, "Device not find\n");
        return -1;
    }

    return buffer_temp;
}

float get_potentiometer() {

    // Creating the message to send 0xA2 and registry
    unsigned char buffer[] = {0xa2,9,5,0,3};

    // Send
    int count_w = write(uart0_filestream, buffer, 5);
    if(count_w <= 0)
    {
        fprintf( stderr, "Error: Dont write in uart communication\n");
        return -1;
    }

    float buffer_pot = 0.0;

    // Read response from Arduino
    int count_r = read(uart0_filestream, &buffer_pot, 4);

    if(count_r < 0) {
        fprintf( stderr, "Error: Dont read in uart communication\n");
        return -1;
    } else if(count_r == 0) {
        fprintf( stderr, "Device not find\n");
        return -1;
    }

    return buffer_pot;
}

double get_intern_temperature() {
    return get_temp_sensor_data_forced_mode(&dev);
}

void set_lcd_temperatures() {
    float ex_temp = get_extern_temperature();
    float in_temp = get_intern_temperature();
    float re_temp = (ex_temp+in_temp)/2;
    // printf("Extern temperature %f\n", ex_temp);
    // printf("Intern temperature %f\n", in_temp);


    char line1[255];
    sprintf(line1, "TI:%.2f T2:%.2f", in_temp, ex_temp);

    char line2[255];
    sprintf(line2, "TR: %.2f ", re_temp);

    printf(line1);
    printf(line2);

    ClrLcd(LED_CONTROL);
    lcdLoc(LINE1, LED_CONTROL);
    typeln(line1, LED_CONTROL);
    lcdLoc(LINE2, LED_CONTROL);
    typeln(line2, LED_CONTROL);

}