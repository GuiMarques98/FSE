#include "../inc/server_conf.h"


int main(int argc, char** argv) {
    // Init ncurses

    // signal(SIGINT,interrupt);
    // signal(SIGTERM,interrupt);
    // start_all();
    extern int fd_socket;

    struct sockaddr_in addr;
    fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_socket == -1) {
        fprintf(stderr, "Failed to open socket.\n");
        exit(1);
    }
    addr.sin_port = htons(CENTRAL_PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    int err = bind(fd_socket, (struct sockaddr *)&addr,sizeof(struct sockaddr_in));
    if(err == -1) {
        fprintf(stderr, "Error bind port %d\n", CENTRAL_PORT);
        exit(1);
    }

    err = listen(fd_socket, QUEUE_LEN);
    if(err == -1) {
        fprintf(stderr, "Error initing listen to\n");
        exit(1);
    }


    get_env(NULL);

    return 0;
}