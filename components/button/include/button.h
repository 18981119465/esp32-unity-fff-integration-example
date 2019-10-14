#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"


/**
 * @brief Setups given GPIO as input with falling edge interrupts.
 * 
 * @param butt_gpio GPIO number where button is connected.
 */
void button_init(gpio_num_t butt_gpio);

#endif /* BUTTON_H */
