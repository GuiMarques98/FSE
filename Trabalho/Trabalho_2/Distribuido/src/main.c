#include <bcm2835.h>
#include <signal.h>
#include <stdio.h>

// #include "../inc/cJSON.h"

#include "../inc/server_control.h"

// using namespace std;
int main(int argc, char const *argv[])
{
    signal(SIGINT,close_signal);
    signal(SIGTERM,close_signal);
   // start_server();
    init_devices();
    server_device(NULL);
    return 0;

}
