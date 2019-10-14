#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"


/** Button event callback type. */
typedef void button_cb_t(void);


/**
 * @brief Setups given GPIO as input with falling edge interrupts.
 * 
 * @param butt_gpio GPIO number where button is connected.
 */
void button_init(gpio_num_t butt_gpio);

/**
 * @brief Register a callback to be called every time the button is pressed.
 * 
 * @param Pointer to callback function.
 */
void button_register_cb(button_cb_t *cb);

#endif /* BUTTON_H */
