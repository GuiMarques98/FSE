#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>

#define debug(x) cout << #x << " = " << (x) << endl

using namespace std;
int main(int argc, char const *argv[])
{
    // Setup
    int uart0_filestream = -1;
    string a = "/dev/serial0";
    int mat = 0x95030000;

    uart0_filestream = open(a.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
    {
        cerr << "Error: Serial port not working" << endl;
        return -1;
    }

    // Config UART
    struct termios options;
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    if (uart0_filestream == -1)
    {
        cerr << "Error: Serial port not working" << endl;
        return -1;
    }

    unsigned char uart_buffer[256] = {0};
    long int buffer = 0x0000A1 | mat;
    debug(buffer);
    int count = write(uart0_filestream, &buffer, sizeof(long int));
    if (count <= 0)
    {
        cerr << "Error: Dont write in uart comunication" << endl;
    }
    debug(count);

    count = 0;
    usleep(100);
    float buffer_float = 0;
    count = read(uart0_filestream, (void *)&buffer_float, 4);
    debug(count);
    debug(buffer_float);
    if (count < 0)
    {
        cerr << "Error: Dont read in uart comunication" << endl;
    }
    else if (count == 0)
    {
        cerr << "Device not find" << endl;
    }
    else
    {
        cout << "The uart answer is " << uart_buffer << endl;
    }

    close(uart0_filestream);
    return 0;   
}
