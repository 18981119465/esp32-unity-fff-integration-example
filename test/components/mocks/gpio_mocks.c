#include "gpio_mocks.h"

DEFINE_FAKE_VALUE_FUNC(esp_err_t, gpio_config, const gpio_config_t *);
DEFINE_FAKE_VALUE_FUNC(esp_err_t, gpio_install_isr_service, int);
DEFINE_FAKE_VALUE_FUNC(esp_err_t, gpio_isr_handler_add, gpio_num_t, gpio_isr_t, void *);
