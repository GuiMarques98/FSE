#include "../inc/gpio.h"

void init_gpio() {
    if (!bcm2835_init())
        exit(1);

    // Lamp
    bcm2835_gpio_fsel(LAMP_KITCHEN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(LAMP_ROOM, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_BEDROOM_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_BEDROOM_2, BCM2835_GPIO_FSEL_OUTP);
    
    // Air condition
    bcm2835_gpio_fsel(AIR_CONDITIONING_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(AIR_CONDITIONING_2, BCM2835_GPIO_FSEL_INPT);

    // Opening sensor
    // bcm2835_gpio_fsel(OPENING_WINDOW_KITCHEN, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OPENING_DOOR_KITCHEN, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OPENING_WINDOW_ROOM, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OPENING_DOOR_ROOM, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OPENING_WINDOW_BEDROOM_1, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OPENING_WINDOW_BEDROOM_2, BCM2835_GPIO_FSEL_OUTP);

    // // Presence sensor
    // bcm2835_gpio_fsel(PRESENCE_ROOM, BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(PRESENCE_KITCHEN, BCM2835_GPIO_FSEL_OUTP);

    // // Presence
    bcm2835_gpio_fsel(PRESENCE_ROOM, BCM2835_GPIO_FSEL_INPT);
    // bcm2835_gpio_set_pud(PRESENCE_ROOM, BCM2835_GPIO_PUD_UP);
    // bcm2835_gpio_len(PRESENCE_ROOM);

    // bcm2835_gpio_fsel(PRESENCE_KITCHEN, BCM2835_GPIO_FSEL_INPT);
    // bcm2835_gpio_set_pud(PRESENCE_KITCHEN, BCM2835_GPIO_PUD_UP);
    // bcm2835_gpio_len(PRESENCE_KITCHEN);
}

void close_gpio() {
    bcm2835_close();
}


