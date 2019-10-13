#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "mockable.h"
#include "testable.h"

#include "esp_log.h"


static const char TAG[] = "main";


void app_main(void)
{
  ESP_LOGI(TAG, "%s() - First, call some real functions...", __FUNCTION__);
  mockable_function();
  testable_calls_mockable();
  
  ESP_LOGI(TAG, "%s() - Then, go to test menu...", __FUNCTION__);
  unity_run_menu();
}
