#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"
#include "fff.h"
#include "gpio_mocks.h"
#include "freertos_mocks.h"
#include "button.h"


static const char TAG[] = "[button]";
DEFINE_FFF_GLOBALS;


static void reset_fakes(void)
{
  RESET_FAKE(gpio_config);
  RESET_FAKE(gpio_install_isr_service);
  RESET_FAKE(gpio_isr_handler_add);
  RESET_FAKE(xTaskCreatePinnedToCore);
  RESET_FAKE(xQueueGenericCreate);
  RESET_FAKE(xQueueGenericReceive);
  RESET_FAKE(xQueueGenericSendFromISR);
}

static void before_each(void)
{
  reset_fakes();
}

static void after_each(void)
{
  FFF_RESET_HISTORY();
}

static unsigned int get_first_call_idx(fff_function_t func)
{
  unsigned int call_idx;
  
  for (call_idx = 0; FFF_CALL_HISTORY_LEN > call_idx; call_idx++) {
    if (fff.call_history[call_idx] == func) {
      return call_idx;
    }
  }
  
  return call_idx;
}


TEST_CASE(
  "button_init(): "
  "Should config given GPIO as input",
  TAG)
{
  const gpio_num_t test_gpio = 4;
  gpio_config_t *actual_cfg;
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, gpio_config_fake.call_count);
  TEST_ASSERT_NOT_NULL(gpio_config_fake.arg0_val);
  actual_cfg = (gpio_config_t *) gpio_config_fake.arg0_val;
  TEST_ASSERT_BIT_HIGH(test_gpio, actual_cfg->pin_bit_mask);
  TEST_ASSERT_EQUAL_INT(GPIO_MODE_INPUT, actual_cfg->mode);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should enable falling edge interrupts on given GPIO",
  TAG)
{
  const gpio_num_t test_gpio = 4;
  gpio_config_t *actual_cfg;
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, gpio_config_fake.call_count);
  TEST_ASSERT_NOT_NULL(gpio_config_fake.arg0_val);
  actual_cfg = (gpio_config_t *) gpio_config_fake.arg0_val;
  TEST_ASSERT_EQUAL_INT(GPIO_INTR_NEGEDGE, actual_cfg->intr_type);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should install GPIO ISR service",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, gpio_install_isr_service_fake.call_count);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should add ISR handler with given GPIO as argument",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, gpio_isr_handler_add_fake.call_count);
  TEST_ASSERT_EQUAL_INT(test_gpio, gpio_isr_handler_add_fake.arg0_val);
  TEST_ASSERT_NOT_NULL(gpio_isr_handler_add_fake.arg1_val);
  TEST_ASSERT_EQUAL_INT(test_gpio, gpio_isr_handler_add_fake.arg2_val);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should create queue for at least one gpio_num_t elements",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, xQueueGenericCreate_fake.call_count);
  TEST_ASSERT_GREATER_THAN_UINT(0, xQueueGenericCreate_fake.arg0_val);
  TEST_ASSERT_EQUAL_UINT(sizeof(test_gpio), xQueueGenericCreate_fake.arg1_val);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should create queue before adding ISR handler",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  unsigned int xQueueGenericCreate_call_idx = 0;
  unsigned int gpio_isr_handler_add_call_idx = 0;
  
  before_each();
  
  button_init(test_gpio);
  
  xQueueGenericCreate_call_idx = get_first_call_idx((fff_function_t) xQueueGenericCreate);
  gpio_isr_handler_add_call_idx = get_first_call_idx((fff_function_t) gpio_isr_handler_add);
  
  TEST_ASSERT_LESS_THAN_UINT(gpio_isr_handler_add_call_idx, xQueueGenericCreate_call_idx);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should create queue before creating task",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  unsigned int xQueueGenericCreate_call_idx = 0;
  unsigned int xTaskCreatePinnedToCore_call_idx = 0;
  
  before_each();
  
  button_init(test_gpio);
  
  xQueueGenericCreate_call_idx = get_first_call_idx((fff_function_t) xQueueGenericCreate);
  xTaskCreatePinnedToCore_call_idx = get_first_call_idx((fff_function_t) xTaskCreatePinnedToCore);
  
  TEST_ASSERT_LESS_THAN_UINT(xTaskCreatePinnedToCore_call_idx, xQueueGenericCreate_call_idx);
  
  after_each();
}

TEST_CASE(
  "button_init(): "
  "Should create task with name 'button_task'",
  TAG)
{
  const gpio_num_t test_gpio = 4;
  const char task_name[] = "button_task";
  
  before_each();
  
  button_init(test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, xTaskCreatePinnedToCore_fake.call_count);
  TEST_ASSERT_NOT_NULL(xTaskCreatePinnedToCore_fake.arg0_val);
  TEST_ASSERT_NOT_NULL(xTaskCreatePinnedToCore_fake.arg1_val);
  TEST_ASSERT_EQUAL_STRING(task_name, xTaskCreatePinnedToCore_fake.arg1_val);
  
  after_each();
}

TEST_CASE(
  "button_isr(): "
  "Should send given argument to queue",
  TAG
)
{
  const gpio_num_t test_gpio = 4;
  gpio_isr_t button_isr = NULL;
  
  before_each();
  
  button_init(test_gpio);
  button_isr = gpio_isr_handler_add_fake.arg1_val;
  TEST_ASSERT_NOT_NULL(button_isr);
  button_isr((void *) test_gpio);
  
  TEST_ASSERT_EQUAL_INT(1, xQueueGenericSendFromISR_fake.call_count);
  TEST_ASSERT_EQUAL_INT(test_gpio, xQueueGenericSendFromISR_fake.arg1_val);
  
  after_each();
}
