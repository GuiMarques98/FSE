#include "../inc/server_control.h"
// #include "../inc/sys_defs.h"

pthread_t control_thread[2];

void start_server(int* fd) {
    init_server(fd);
}

void init_server(int* fd) {
    // Initing all devices
    init_devices();

    struct sockaddr_in addr;
    *fd = socket(AF_INET, SOCK_STREAM, 0);
    if(*fd == -1) {
        fprintf(stderr, "Failed to open socket.\n");
        exit(1);
    }
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    int err = bind(*fd, (struct sockaddr *)&addr,sizeof(struct sockaddr_in));
    if(err == -1) {
        fprintf(stderr, "Error bind port %d\n", SERVER_PORT);
        exit(1);
    }

    err = listen(*fd, QUEUE_LEN);
    if(err == -1) {
        fprintf(stderr, "Error initing listen to\n");
        exit(1);
    }

    // Initing server thread
    // extern pthread_t control_thread[2];
    server_arg_t args;
    args.fd = fd;
    args.addr = &addr;
    // Creating threads for server listener commands
    // pthread_create(&control_thread[0], NULL, server_listener, (void *)&args);


    // Creating thread for server listener to look up devices
    // pthread_create(&control_thread[1], NULL, server_device, NULL);

}

void close_server() {
    // extern pthread_t control_thread[2];
    interrpt_signal(0);
    // pthread_cancel(control_thread[0]);
    // pthread_cancel(control_thread[1]);
}

void close_signal(int signal) {
    close_server();
}

void alarm_signal(int signal) {
    
}

void close_all(int signal) {
    close_server();
}

void* server_listener(void* args) {
    server_arg_t arg = *((server_arg_t*) args);
    while(1) {
        int fd_connect = accept(*(arg.fd), (struct sockaddr*)arg.addr, (socklen_t *)sizeof(struct sockaddr));
        if(fd_connect < 0) {
            fprintf(stderr, "Error receiving package\n");
            continue;
        } else {
            double lamp[4] = {0}, air[2] = {0};
            int err = get_json(&fd_connect, lamp, air);
            if(err == 0) {
                turn_lamp(LAMP_KITCHEN, lamp[0]);
                turn_lamp(LAMP_ROOM, lamp[1]);
                turn_lamp(LAMP_BEDROOM_1, lamp[2]);
                turn_lamp(LAMP_BEDROOM_2, lamp[3]);

                turn_air(AIR_CONDITIONING_1, air[0]);
                turn_air(AIR_CONDITIONING_2, air[1]);
            }
            shutdown(fd_connect, SHUT_RDWR);
            close(fd_connect);
        }

    }
}

int get_json(int* fd_connect, double* lamp, double* air) {
    char buffer[MAX_JSON_STRING];

    int size = read(*fd_connect, buffer, MAX_JSON_STRING);
    if(size <= 0) {
        fprintf(stderr, "Error reading package\n");
        return -1;
    }
    printf("%s", buffer);
    cJSON *json = cJSON_Parse(buffer);
    // Getting all items

    // Getting lamp Json
    cJSON* item = cJSON_GetObjectItemCaseSensitive(json, "lamp1");
    lamp[0] = cJSON_GetNumberValue(item);

    item = cJSON_GetObjectItemCaseSensitive(json, "lamp2");
    lamp[1] = cJSON_GetNumberValue(item);

    item = cJSON_GetObjectItemCaseSensitive(json, "lamp3");
    lamp[2] = cJSON_GetNumberValue(item);

    item = cJSON_GetObjectItemCaseSensitive(json, "lamp4");
    lamp[3] = cJSON_GetNumberValue(item);

    // Getting air Json
    item = cJSON_GetObjectItemCaseSensitive(json, "air1");
    air[0] = cJSON_GetNumberValue(item);

    item = cJSON_GetObjectItemCaseSensitive(json, "air2");
    air[1] = cJSON_GetNumberValue(item);

    // make json stuff
    cJSON_Delete(json);

    return 0;
}

void* server_device(void* args) {
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    double time_b = begin .tv_sec;
    time_b += begin.tv_nsec / 1000000000.0;

    while(1) {
        if(detect_any_presence()) {
            printf("Detect presence!\n");
            // alarm
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_e = end .tv_sec;
        time_e += end.tv_nsec / 1000000000.0;

        if((time_e - time_b) >= 1.0) {
            clock_gettime(CLOCK_MONOTONIC, &begin);
            time_b = begin .tv_sec;
            time_b += begin.tv_nsec / 1000000000.0;

            bme_env_t env = get_temperature_house();
            if(env.err)
                printf("Error in read code %d\n", env.err);
            printf("This is the temperature %.2f and this humidity %.2f\n", env.temp, env.hum);
        }

        // Sleep
        struct timespec ts;
        ts.tv_sec = 200 / 1000;
        ts.tv_nsec = (200 % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
}

int send_alarm() {
    struct sockaddr_in addr;
    int fd_alarm = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_port = htons(CENTRAL_PORT);
    addr.sin_addr.s_addr = inet_addr(CENTRAL_IP);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

}