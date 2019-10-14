#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mockable.h"
#include "testable.h"
#include "button.h"

#include "esp_log.h"

#define BUTTON_GPIO CONFIG_BUTTON_GPIO


static const char TAG[] = "main";


static void button_event_cb(void)
{
  printf("\n");
  testable_calls_mockable();
  printf("\n");
}

void app_main(void)
{
  ESP_LOGI(TAG, "Hello from %s()!", __FUNCTION__);
  
  button_register_cb(&button_event_cb);
  button_init(BUTTON_GPIO);
}
