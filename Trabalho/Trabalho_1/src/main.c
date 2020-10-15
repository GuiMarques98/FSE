#include <bcm2835.h>
#include <signal.h>
// #include <wiringPiI2C.h>
// #include <wiringPi.h>


#include "../inc/usr_ctrl.h"



// using namespace std;
int main(int argc, char const *argv[])
{

    start();
    // init_devices();
    // set_fan(TURN_ON);
    // set_resistor(TURN_ON);
    // int inp = 0;
    // int inp2 = 0;
    // while(1) {
    //     //set_fan(TURN);
    //     printf("digite 1 pra ventuinha e 2 pra o resistor\n");
    //     scanf("%d", &inp);

    //     printf("Digite se quer ligar ou desligar\n");
    //     scanf("%d", &inp2);
    //     if(inp == 1) {
    //         break;
    //         set_fan(inp2);
    //     } else {
    //         set_resistor(inp2);
    //     }
    //     // float temp = get_extern_temperature();
    //     // printf("Extern temperature %f\n", temp);
    //     // cout << "Extern temperature " << temp << endl;
    //     //set_lcd_temperatures();
    //     //sleep(1);
    // }
    // set_fan(TURN_OFF);
    // set_resistor(TURN_OFF);

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
