#ifndef GPIO_MOCKS_H
#define GPIO_MOCKS_H

#include "driver/gpio.h"
#include "fff.h"

DECLARE_FAKE_VALUE_FUNC(esp_err_t, gpio_config, const gpio_config_t *);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, gpio_install_isr_service, int);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, gpio_isr_handler_add, gpio_num_t, gpio_isr_t, void *);

#endif /* GPIO_MOCKS_H */
