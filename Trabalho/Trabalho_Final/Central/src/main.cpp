#include "../inc/server_conf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// void sig_stop(int sig) {
//     extern int fd_socket;
//     close(fd_socket);
//     exit(0);
// }

int main(int argc, char** argv) {
    // Init ncurses

    signal(SIGINT,interrupt);
    signal(SIGTERM,interrupt);
    start_all();
    // system("omxplayer doc/Alarm-Fast-A1-www.fesliyanstudios.com.mp3");
    return 0;
}