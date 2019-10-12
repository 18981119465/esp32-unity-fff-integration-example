#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mockable.h"
#include "testable.h"

#include "esp_log.h"


static const char TAG[] = "testable";


bool testable_calls_mockable(void)
{
  ESP_LOGI(TAG, "%s() - Lets see if mockable_function() is real...", __FUNCTION__);
  
  if (0 == mockable_function()) {
    ESP_LOGI(TAG, "%s() - It seems real.", __FUNCTION__);
    return true;
  }
  else {
    ESP_LOGW(TAG, "%s() - It seems it has been mocked!", __FUNCTION__);
    return false;
  }
}
