#include "../inc/server_control.h"
// #include "../inc/sys_defs.h"

pthread_t control_thread[2];
pthread_mutex_t lock;
pthread_mutexattr_t lock_attr;

int fd_socket;


void start_server() {
    init_server();
}

void init_server() {
    // Initing all devices
    init_devices();

    struct sockaddr_in addr;
    fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_socket == -1) {
        fprintf(stderr, "Failed to open socket.\n");
        exit(1);
    }
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    int err = bind(fd_socket, (struct sockaddr *)&addr,sizeof(struct sockaddr_in));
    if(err == -1) {
        fprintf(stderr, "Error bind port %d\n", SERVER_PORT);
        exit(1);
    }

    err = listen(fd_socket, QUEUE_LEN);
    if(err == -1) {
        fprintf(stderr, "Error initing listen to\n");
        exit(1);
    }

    // Initing server thread
    // extern pthread_t control_thread[2];
    server_arg_t args;
    args.fd = &fd_socket;
    args.addr = &addr;

    // Mutex init
    pthread_mutexattr_init(&lock_attr);
    pthread_mutexattr_settype(&lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&lock_attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&lock, &lock_attr);

    // Creating threads for server listener commands
    pthread_create(&control_thread[0], NULL, server_listener, (void *)&args);


    // Creating thread for server listener to look up devices
    pthread_create(&control_thread[1], NULL, server_device, (void *)&args);

    pthread_join(control_thread[0], NULL);
    pthread_join(control_thread[1], NULL);

}

void close_server() {
    // extern pthread_t control_thread[2];
    interrpt_signal(0);
    pthread_mutexattr_destroy(&lock_attr);
    pthread_mutex_destroy(&lock);
    pthread_cancel(control_thread[0]);
    pthread_cancel(control_thread[1]);
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
        int fd_connect = accept(fd_socket, NULL, NULL);
        if(fd_connect == -1) {
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
    printf("\nJson recebido:\n%s\n", buffer);
    cJSON *json = cJSON_Parse(buffer);
    // Getting all items

    // Getting lamp Json
    cJSON* item = cJSON_GetObjectItemCaseSensitive(json, "lamp1");
    // printf("Item=%p\n", item);
    lamp[0] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;
    item = cJSON_GetObjectItemCaseSensitive(json, "lamp2");

    lamp[1] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;

    item = cJSON_GetObjectItemCaseSensitive(json, "lamp3");
    lamp[2] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;

    item = cJSON_GetObjectItemCaseSensitive(json, "lamp4");
    lamp[3] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;

    // Getting air Json
    item = cJSON_GetObjectItemCaseSensitive(json, "air1");
    air[0] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;

    item = cJSON_GetObjectItemCaseSensitive(json, "air2");
    air[1] = cJSON_GetNumberValue(item);
    if(item == NULL)
        return -1;

    // make json stuff
    cJSON_Delete(json);
    for(int i=0;i<4;++i)
        printf("Lamp[%d]=%f, ", i, lamp[i]);

    printf("\n");

    printf("Air[%d]=%f, Air[%d]=%f\n", 0, air[0], 1, air[1]);
    

    return 0;
}

void* server_device(void* args) {
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    double time_b = begin .tv_sec;
    time_b += begin.tv_nsec / 1000000000.0;

    while(1) {
        // if(detect_any_presence()) {
        //     printf("Detect presence!\n");
        //     while(send_alarm());
        // }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_e = end .tv_sec;
        time_e += end.tv_nsec / 1000000000.0;

        if((time_e - time_b) >= 1.0) {
            clock_gettime(CLOCK_MONOTONIC, &begin);
            time_b = begin .tv_sec;
            time_b += begin.tv_nsec / 1000000000.0;

            bme_env_t env = get_temperature_house();
            if(env.err) {
                printf("Error in read code %d\n", env.err);
                continue;
            }
            
            send_env(env);

            // sprintf
            printf("This is the temperature %.2f and this humidity %.2f\n", env.temp, env.hum);
        }

        // Sleep
        struct timespec ts;
        ts.tv_sec = 200 / 1000;
        ts.tv_nsec = (200 % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
}


int send_env(bme_env_t env) {
    struct sockaddr_in addr;
    int fd_alarm = socket(AF_INET, SOCK_STREAM, 0);
    if(fd_alarm < 0) {
        fprintf(stderr, "Error open socket\n");
        return -1;

    }


    addr.sin_port = htons(CENTRAL_PORT);
    addr.sin_addr.s_addr = inet_addr("192.168.0.53");
    addr.sin_family = AF_INET;

    int err = connect(fd_alarm, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    if(err < 0) {
        fprintf(stderr, "Error sending env package\n");
        return -1;
    }

    char buffer[MAX_JSON_STRING]; 
    
    sprintf(buffer, "{\"temp\":%.2f,\"hum\":%.2f}", env.temp, env.hum);
    err = send(fd_alarm, buffer, MAX_JSON_STRING, 0);

    close(fd_alarm);
    if(err <= 0) {
        fprintf(stderr, "Error sending alarm package\n");
        return -1;
    }

    return 0;

    return 0;

}

int send_alarm() {
    struct sockaddr_in addr;
    int fd_alarm = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_port = htons(CENTRAL_PORT);
    addr.sin_addr.s_addr = inet_addr(CENTRAL_IP);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    int err = connect(fd_alarm, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    if(err < 0) {
        fprintf(stderr, "Error connecting\n");
        return -1;
    }
    char buffer[100] = "{\"alarm\":1}"; 
    err = send(fd_alarm, buffer, 100, 0);
    close(fd_alarm);
    if(err <= 0) {
        fprintf(stderr, "Error sending alarm package\n");
        return -1;
    }

    return 0;
}