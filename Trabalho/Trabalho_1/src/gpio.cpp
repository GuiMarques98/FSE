#include "../inc/gpio.hpp"

void config_gpio() {
    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_gpio_fsel(FAN, BCM2835_GPIO_FSEL_ALT5);
}

void config_pwm(int fun_start, int resistor_start) {
    // if(fun_start > RANGE-1) 
    //     PWM_RESISTOR_CONTROL = RANGE-1;
    // else if (fun_start <= 0)
    //     PWM_RESISTOR_CONTROL = 1;
    // else
    //     PWM_RESISTOR_CONTROL = fun_start;

    // if(resistor_start > RANGE-1) 
    //     PWM_FAN_CONTROL = RANGE-1;
    // else if (resistor_start <= 0)
    //     PWM_FAN_CONTROL = 1;
    // else
    //     PWM_FAN_CONTROL = resistor_start;
    
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);

}

void set_pwm(int data) {
    bcm2835_pwm_set_data(PWM_CHANNEL, data);
    bcm2835_delay(1);
}
