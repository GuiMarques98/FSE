#include "../inc/server_conf.h"


// Screen ncurses variables
WINDOW* wenv;
WINDOW* wdevices;
int max_x, max_y;
float temp = 0.0, hum = 0.0;
int flag = 1;
bool devices_turn[6] = {false};

// Connection variables
int fd_socket;


// PThread variables
pthread_t control_thread;
pthread_mutex_t lock;
pthread_mutexattr_t lock_attr;

// namespace std
using namespace std;

void start_all() {
    init_menu();
    menu_cont();
    close_menu();
}

void init_menu() {
    // Socket initing
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

    // Pthread init mutex
    pthread_mutexattr_init(&lock_attr);
    pthread_mutexattr_settype(&lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&lock_attr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&lock, &lock_attr);

    // Creating threads for server listener commands
    pthread_create(&control_thread, NULL, get_env, NULL);


    // Initing ncurses
    initscr();
    noecho();
    cbreak();

    // Get max X and Y coordinates
    getmaxyx(stdscr, max_y, max_x);
    curs_set(0);

    // Create a new window
    wenv = newwin(4, max_x-1, 0, 0);
    wdevices = newwin(10, max_x-1, 4, 0);
    box(wdevices, 0, 0);
    box(wenv, 0, 0);
    refresh();
    wrefresh(wdevices);
    wrefresh(wenv);

    nodelay(wdevices, true);
    // We can use a arrow
    keypad(wdevices, true);
    keypad(wenv, false);


}

void menu_cont() {
    string devices_str[7] = {"Lampada da Cozinha", "Lampada da Sala", "Lampada da Quarto 1", "Lampada da Quarto 2", "Ar Condicionado do Quarto 1", "Ar Condicionado do Quarto 2", "Sair"};
    string temp_h[2] = {"DESLIGADO", "LIGADO      "};
    int choice, highlight = 0;
    while(flag) {

        for(int i=0; i < 7; ++i) {
            if(i == highlight)
                wattron(wdevices, A_REVERSE);
            mvwprintw(wdevices, i+1, 1, devices_str[i].c_str());
            wattroff(wdevices, A_REVERSE);
            int j = (devices_turn[i])?1:0;
            if(i == 7)
                continue;
            mvwprintw(wdevices, i+1, max_x/2, temp_h[j].c_str());
        }

        mvwprintw(wenv, 1, 1, "Temperatura: %.2f", temp);
        mvwprintw(wenv, 2, 1, "Humidade: %.2f", hum);
        wattroff(wenv, A_REVERSE);
        wrefresh(wenv);


        choice = wgetch(wdevices);

        switch(choice) {
            case KEY_UP:
                highlight--;
                if(highlight < 0)
                    highlight=0;
                break;

            case KEY_DOWN:
                highlight++;
                if(highlight > 6)
                    highlight=6;
                break;
            
            case 10:
                apply_choice(highlight);
                break;
        }

        // Sleep
        struct timespec ts;
        ts.tv_sec = 200 / 1000;
        ts.tv_nsec = (200 % 1000) * 1000000;
        nanosleep(&ts, NULL);

    }
}

void* get_env(void* args) {

    while(1) {
        int fd_connect = accept(fd_socket, NULL, NULL);
        if(fd_connect == -1) {
            fprintf(stderr, "Error receiving package\n");
            continue;
        } else {
            if(get_json(&fd_connect) != 0) {
                fprintf(stderr, "Error reading json!\n");
            }
            get_json(&fd_connect);
            shutdown(fd_connect, SHUT_RDWR);
            close(fd_connect);
        }
    }
}

int get_json(int* fd_connect) {
    char buffer[MAX_JSON_STRING] = "";

    int size = read(*fd_connect, buffer, MAX_JSON_STRING);
    printf("Json:\n%s\n", buffer);
    if(size <= 0) {
        fprintf(stderr, "Error reading package\n");
        return -1;
    }
    printf("\nJson recebido:\n%s\n", buffer);
    cJSON *json = cJSON_Parse(buffer);
    if(json == NULL)
        return -1;

    cJSON* item = cJSON_GetObjectItemCaseSensitive(json, "alarm");
    if(item != NULL) {
        play_alarm();
        return 0;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, "temp");
    if(item == NULL)
        return -1;
    temp = cJSON_GetNumberValue(item);
    printf("Temp = %.2f", temp);

    item = cJSON_GetObjectItemCaseSensitive(json, "hum");
    if(item == NULL)
        return -1;
    hum = cJSON_GetNumberValue(item);
    printf("Temp = %.2f", hum);

    return 0;
}

void play_alarm() {}

void close_menu() {

    pthread_mutexattr_destroy(&lock_attr);
    pthread_mutex_destroy(&lock);
    pthread_cancel(control_thread);
    close(fd_socket);
    endwin();
}

void send_command(int sig) {
    // char message[] = "{\"lamp1\":0,\"lamp2\":0,\"lamp3\":0,\"lamp4\":0,\"air1\":0,\"air2\":0}\0";
    // char message[] = "{\"lamp%d\":%d,\"lamp%d\":%d,\"lamp%d\":%d,\"lamp%d\":%d,\"air%d\":%d,\"air%d\":%d}\0";

    stringstream stringStream;
    stringStream << "{";

    for(int i=0; i< 6; ++i) {
        if(i < 4)
            stringStream << "\"lamp" << i << "\":" << devices_turn[i] << ",";
        else
            stringStream << "\"air" << (i-4) << "\":" << devices_turn[i];
        if(i == 4)
            stringStream << ",";
    }
    stringStream << "}";

    cout << stringStream.str() << endl;

    string message = stringStream.str();
    // char message[] = temp.c_str();

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        fprintf(stderr, "Failed to open socket.\n");
        exit(1);
    }

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(DIST_PORT);
    addr.sin_addr.s_addr = inet_addr(DIST_IP);

    int err = connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    if(err == 0) {
        err = send(fd, message.c_str(), message.length(), 0);
        close(fd);
    }

}

void interrupt(int sig) {
    close_menu();
}

void apply_choice(int highlight) {
    switch(highlight) {
        case 0:
            devices_turn[0] = !devices_turn[0];
            break;
        case 1:
            devices_turn[1] = !devices_turn[1];
            break;
        case 2:
            devices_turn[2] = !devices_turn[2];
            break;
        case 3:
            devices_turn[3] = !devices_turn[3];
            break;
        case 4:
            devices_turn[4] = !devices_turn[4];
            break;
        case 5:
            devices_turn[5] = !devices_turn[5];
            break;
        case 6:
            flag = 0;
            break;
    }
}
