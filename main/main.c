#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"


static const char TAG[] = "main";


void app_main(void)
{
  ESP_LOGI(TAG, "Hello from %s()!", __FUNCTION__);
}
