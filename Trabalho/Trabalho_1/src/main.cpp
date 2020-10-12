#include <iostream>
#include <bcm2835.h>
// #include <wiringPiI2C.h>
// #include <wiringPi.h>


#include "../inc/sys_disp.hpp"
// #include "../inc/i2c.hpp"
// #include "../inc/gpio.hpp"


using namespace std;
int main(int argc, char const *argv[])
{
    float temp = get_extern_temperature();
    cout << "Extern temperature " << temp << endl;

    // float pot = get_potentiometer();
    // cout << "Potenciometer " << pot << endl;

    // float temp = get_intern_temperature();
    // cout << "Intern temperature " << temp << endl;
    if (wiringPiSetup () == -1) exit (1);
    int fad = wiringPiI2CSetup(0x76);
    cout << wiringPiI2CRead(fad) << endl;

    return 0;
}
