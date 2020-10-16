#include "../inc/sys_disp.h"

#include "../inc/sys_defs.h"

void* controller(void* args) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    while(1){
        set_lcd_temperatures();
        switch(read_type_global) {
            case 'p':
                if(floor(extern_temperature_global) > floor(potenciometer_global)) {
                    cold();
                } else if(floor(extern_temperature_global) ==  floor(potenciometer_global)) {
                    turn_off_fan_resistor();
                } else {
                    warm();
                }
                break;
            case 't':
                if(floor(extern_temperature_global) > floor(informed_temperature_global)) {
                    cold();
                } else if(floor(extern_temperature_global) ==  floor(informed_temperature_global)) {
                    turn_off_fan_resistor();
                } else {
                    warm();
                }
                break;
        }
        
        struct timespec ts;
        ts.tv_sec = 500 / 1000;
        ts.tv_nsec = (500 % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
}

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
    // Init threads
    pthread_mutexattr_init(&mutex_temperature_choice_attr);
    pthread_mutexattr_settype(&mutex_temperature_choice_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&mutex_temperature_choice_attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&mutex_temperature_choice, &mutex_temperature_choice_attr);

    if(pthread_create(&control_thread, NULL, controller, NULL) != 0){
        perror("Não foi possível criar a thread");
        exit(1);
    }
    
    // Init GPIO
    init_gpio();

    // Setup WiringPI
    if (wiringPiSetup () == -1) {
        fprintf( stderr, "Wiring PI Setup Fail\n");
        exit(1);
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
    pthread_cancel(control_thread);
    set_fan(TURN_OFF);
    set_resistor(TURN_OFF);
    lcd_init(LED_CONTROL);
    close(uart0_filestream);
    close(id.fd);
    close_gpio();
    exit(0);
}

double get_intern_temperature() {

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

    extern_temperature_global = buffer_temp;

    return extern_temperature_global;
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
    potenciometer_global = buffer_pot;
    return potenciometer_global;
}

float get_extern_temperature() {
    intern_temperature_global = get_temp_sensor_data_forced_mode(&dev);
    return intern_temperature_global;
}

void set_lcd_temperatures() {
    float ex_temp = get_extern_temperature();
    float in_temp = get_intern_temperature();
    float re_temp;
    switch(read_type_global) {
        case 'p':
            re_temp = get_potentiometer();
            break;
        case 't':
            re_temp = informed_temperature_global;
            break;
        default:
            re_temp = 0;
    }
    // printf("Extern temperature %f\n", ex_temp);
    // printf("Intern temperature %f\n", in_temp);


    char line1[255];
    sprintf(line1, "TI:%.2f TE:%.2f", in_temp, ex_temp);

    char line2[255];
    sprintf(line2, "TR: %.2f ", re_temp);

    // printf(line1);
    // printf(line2);

    ClrLcd(LED_CONTROL);
    lcdLoc(LINE1, LED_CONTROL);
    typeln(line1, LED_CONTROL);
    lcdLoc(LINE2, LED_CONTROL);
    typeln(line2, LED_CONTROL);

}

int set_fan(int data) {
    fan_control = data;
    bcm2835_gpio_write(FAN, data);
    //if(data != fan_control)
    //    fan_control = bcm2835_gpio_lev(FAN);
    return fan_control;
}

int set_resistor(int data) {
    resistor_control = data;
    bcm2835_gpio_write(RESISTOR, data);
    //if(data != resistor_control)
    //    resistor_control = bcm2835_gpio_lev(RESISTOR);
    return resistor_control;
}

void warm() {
    set_fan(TURN_OFF);
    set_resistor(TURN_ON);
}

void cold() {
    set_fan(TURN_ON);
    set_resistor(TURN_OFF);
}

void turn_off_fan_resistor() {
    set_fan(TURN_OFF);
    set_resistor(TURN_OFF);
}