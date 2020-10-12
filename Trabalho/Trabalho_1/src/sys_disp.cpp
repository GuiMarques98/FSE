#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>

#include "../inc/sys_disp.hpp"


float get_extern_temperature() {
    // Set up UART file
    int uart0_filestream = -1;
    std::string a = "/dev/serial0";

    uart0_filestream = open(a.c_str(), O_RDWR | O_NOCTTY);
    if (uart0_filestream == -1)
    {
        std::cerr << "Error: Serial port not working" << std::endl;
        return -1;
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
        std::cerr << "Error: Serial port not working" << std::endl;
        return -1;
    }

    // Creating the message to send 0xA1 and registry
    unsigned char buffer[] = {0xa1,9,5,0,3};

    // Send
    int count = write(uart0_filestream, buffer, 5);
    if(count <= 0)
    {
        std::cerr << "Error: Dont write in uart communication" << std::endl;
        return -1;
    }
    // Reset count
    count = 0;

    float buffer_temp = 0.0;

    // Read response from Arduino
    count = read(uart0_filestream, &buffer_temp, 4);

    if(count < 0) {
        std::cerr << "Error: Dont read in uart communication" << std::endl;
        return -1;
    } else if(count == 0) {
        std::cerr << "Device not find" << std::endl;
        std::cerr << "Return " << buffer_temp << std::endl;
        return -1;
    }
    return buffer_temp;
}

float get_potentiometer() {
    // Set up UART file
    int uart0_filestream = -1;
    std::string a = "/dev/serial0";

    uart0_filestream = open(a.c_str(), O_RDWR | O_NOCTTY);
    if (uart0_filestream == -1)
    {
        std::cerr << "Error: Serial port not working" << std::endl;
        return -1;
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
        std::cerr << "Error: Serial port not working" << std::endl;
        return -1;
    }

    // Creating the message to send 0xA2 and registry
    unsigned char buffer[] = {0xa2,9,5,0,3};

    // Send
    int count = write(uart0_filestream, buffer, 5);
    if(count <= 0)
    {
        std::cerr << "Error: Dont write in uart communication" << std::endl;
        return -1;
    }
    // Reset count
    count = 0;

    float buffer_pot = 0.0;

    // Read response from Arduino
    count = read(uart0_filestream, &buffer_pot, 4);

    if(count < 0) {
        std::cerr << "Error: Dont read in uart communication" << std::endl;
        return -1;
    } else if(count == 0) {
        std::cerr << "Device not find" << std::endl;
        std::cerr << "Return " << buffer_pot << std::endl;
        return -1;
    }
    return buffer_pot;
}


float get_intern_temperature() {
    
}
