#include <bcm2835.h>
#include <signal.h>
// #include <wiringPiI2C.h>
// #include <wiringPi.h>


#include "../inc/sys_disp.h"


// using namespace std;
int main(int argc, char const *argv[])
{
    init_devices();
    signal(SIGINT, interrpt_signal); 
    while(1) {
        float temp = get_extern_temperature();
        printf("Extern temperature %f\n", temp);
        // cout << "Extern temperature " << temp << endl;
        set_lcd_temperatures();
        sleep(1);
    }

    // float pot = get_potentiometer();
    // printf("Extern temperature %f\n", pot);

    // float ex_temp = get_extern_temperature();
    // printf("Extern temperature %f\n", ex_temp);

    // double in_temp = get_intern_temperature();
    // printf("Extern temperature %lf\n", in_temp);
    // interrpt_signal(1);
    // while(1) {
    //     set_lcd_temperatures();
    //     delay(1000);
    // }

    return 0;

}
