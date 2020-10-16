#include <bcm2835.h>
#include <signal.h>
// #include <wiringPiI2C.h>
// #include <wiringPi.h>


#include "../inc/usr_ctrl.h"



// using namespace std;
int main(int argc, char const *argv[])
{
    // Init signal handlers
    signal(SIGALRM,alarm_handler);
    signal(SIGINT,interrpt_signal);
    signal(SIGTERM,interrpt_signal);

    // Set signal to 2 seconds to write in csv file
    alarm(2);

    // Start menu and all devices
    start();

    return 0;

}
