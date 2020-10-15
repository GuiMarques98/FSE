#include "../inc/gpio.h"

void init_gpio() {
    if (!bcm2835_init())
        exit(1);
    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(FAN, BCM2835_GPIO_FSEL_OUTP);
    //bcm2835_gpio_set_pud(RESISTOR, BCM2835_GPIO_PUD_UP);
    //bcm2835_gpio_set_pud(FAN, BCM2835_GPIO_PUD_UP);
}

void close_gpio() {
    bcm2835_close();
}


