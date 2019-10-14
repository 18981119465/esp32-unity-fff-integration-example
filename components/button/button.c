#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "button.h"

#include "esp_log.h"


static const char TAG[] = "button";

static struct {
  TaskHandle_t task_h;
  QueueHandle_t queue_h;
} priv_state = {
  .task_h = NULL,
  .queue_h = NULL
};


static void button_task(void *pvParameters)
{
  while (1) {
    vTaskSuspend(NULL);
  }
}

static void IRAM_ATTR button_isr(void *arg)
{
  gpio_num_t gpio_num = (gpio_num_t) arg;
  xQueueSendFromISR(priv_state.queue_h, (void *) gpio_num, NULL);
}

void button_init(gpio_num_t butt_gpio)
{
  gpio_config_t cfg = {
    .pin_bit_mask = 1U << butt_gpio,
    .mode = GPIO_MODE_INPUT,
    .intr_type = GPIO_INTR_NEGEDGE
  };
  
  priv_state.queue_h = xQueueCreate(4, sizeof(gpio_num_t));
  
  gpio_config(&cfg);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(butt_gpio, &button_isr, (void *) butt_gpio);
  
  xTaskCreatePinnedToCore(
    &button_task,
    "button_task",
    1024,
    NULL,
    2,
    &priv_state.task_h,
    tskNO_AFFINITY
  );
}
