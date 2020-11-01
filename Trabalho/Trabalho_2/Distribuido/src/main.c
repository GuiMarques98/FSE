#include <bcm2835.h>
#include <signal.h>

#include "../inc/sys_disp.h"




// using namespace std;
int main(int argc, char const *argv[])
{
    // Init signal handlers
    // signal(SIGALRM,alarm_handler);
    signal(SIGINT,interrpt_signal);
    signal(SIGTERM,interrpt_signal);
    // init_gpio();
    init_devices();
    bme_env_t bme_medidor = get_temperature_house();
    printf("A temperatura é %f\nA umidade é %f\n", bme_medidor.temp, bme_medidor.hum);

    // Set signal to 2 seconds to write in csv file
    // alarm(2);
    // close_gpio();
    interrpt_signal(0);
    return 0;

}
