#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mockable.h"

#include "esp_log.h"


static const char TAG[] = "mockable";


int mockable_function(void)
{
  ESP_LOGI(TAG, "I'm the real %s()!", __FUNCTION__);
  return 0;
}
