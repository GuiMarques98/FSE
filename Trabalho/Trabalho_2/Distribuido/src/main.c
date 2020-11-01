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
    init_gpio();
    int lampada;
    printf("Como está a presença de cada pino a seguir\n");

    printf("O de Presença da sala está %d\n", bcm2835_gpio_eds(PRESENCE_ROOM));
    // printf("O de Presença da cozinha está %d\n", bcm2835_gpio_eds(PRESENCE_KITCHEN));
    // printf("O de Abertura da parta da cozinha está %d\n", bcm2835_gpio_eds(OPENING_DOOR_KITCHEN));
    // printf("O de Abertura da janela da cozinha está %d\n", bcm2835_gpio_eds(OPENING_WINDOW_KITCHEN));
    // printf("O de Abertura da parta da sala está %d\n", bcm2835_gpio_eds(OPENING_DOOR_ROOM));
    // printf("O de Abertura da janela da sala está %d\n", bcm2835_gpio_eds(OPENING_WINDOW_ROOM));
    // printf("O de Abertura da janela do quarto 1 está %d\n", bcm2835_gpio_eds(OPENING_WINDOW_BEDROOM_1));
    // printf("O de Abertura da janela do quarto 2 está %d\n", bcm2835_gpio_eds(OPENING_WINDOW_BEDROOM_2));

    // Set signal to 2 seconds to write in csv file
    // alarm(2);
    close_gpio();
    // interrpt_signal(0);
    return 0;

}
