#include <bcm2835.h>
#include <signal.h>
#include <stdio.h>

// #include "../inc/cJSON.h"

#include "../inc/server_control.h"

// using namespace std;
int main(int argc, char const *argv[])
{
    int fd;
    signal(SIGINT,close_signal);
    signal(SIGTERM,close_signal);
    init_server(&fd);

    server_device(NULL);
    // turn_off_all_lamps();
    // turn_on_all_lamps();
    // turn_off_lamp(17);
    close_server();
    return 0;

}
