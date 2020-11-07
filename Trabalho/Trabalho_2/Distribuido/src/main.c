#include <bcm2835.h>
#include <signal.h>
#include <stdio.h>

// #include "../inc/cJSON.h"

#include "../inc/server_control.h"

// void interrupt_sig(int sig) {
//     interrpt_signal(0);
// }

// using namespace std;
int main(int argc, char const *argv[])
{
    signal(SIGINT,interrpt_signal);
    signal(SIGTERM,interrpt_signal);
    start_server();
    // init_devices();
    // server_device(NULL);
    return 0;

}
