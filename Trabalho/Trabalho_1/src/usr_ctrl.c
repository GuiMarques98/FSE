#include "../inc/usr_ctrl.h"
// #include "../inc/sys_defs.h"

void start() {
    init_menu();
    menu();
}

void init_menu() {
    init_devices();
}

void close_menu() {
    interrpt_signal(0);
}



void menu() {
    int choice = 0;
    while(choice != 3) {
        system("clear");
        printf("Por favor insira a forma que deseja ajustar a temperatura:\n");
        printf("1 - Potenciometro\n2 - Teclado\n3 - Sair\n");
        scanf(" %d", &choice);
        fflush(stdin);
        switch(choice) {
            case 1:
                set_potenciometer_ctrl();
                break;
            case 2:
                set_temperature_ctrl();
        }
    }
    close_menu();

}

void set_potenciometer_ctrl() {
    extern char read_type_global;
    read_type_global = 'p';
    system("clear");
    printf("O programa irá ajustar a temperatura pelo potenciometro por favor digite algo e ENTER para sair");
    fflush(stdin);
    getchar();
    while(getchar()!='\n'); 
}

void set_temperature_ctrl() {
    extern char read_type_global;
    read_type_global = 't';
    system("clear");
    printf("Digite a temperatura a ser ajustada\n");
    fflush(stdin);
    extern float informed_temperature_global;
    extern pthread_mutex_t mutex_temperature_choice;
    pthread_mutex_lock(&mutex_temperature_choice);
    scanf("%f", &informed_temperature_global);
    pthread_mutex_unlock(&mutex_temperature_choice);

    system("clear");
    printf("O programa irá ajustar a temperatura de %f graus Celsius por favor digite algo e ENTER para sair", informed_temperature_global);
    fflush(stdin);
    getchar();
    while(getchar()!='\n'); 

}

