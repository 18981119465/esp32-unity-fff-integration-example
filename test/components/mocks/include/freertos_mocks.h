#ifndef FREERTOS_MOCKS_H
#define FREERTOS_MOCKS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "fff.h"

DECLARE_FAKE_VALUE_FUNC(
  BaseType_t,
  xTaskCreatePinnedToCore,
  TaskFunction_t,
  const char *const,
  const uint32_t,
  void *const,
  UBaseType_t,
  TaskHandle_t *const,
  const BaseType_t
);

DECLARE_FAKE_VALUE_FUNC(
  QueueHandle_t,
  xQueueGenericCreate,
  const UBaseType_t,
  const UBaseType_t,
  const uint8_t
);

DECLARE_FAKE_VALUE_FUNC(
  BaseType_t,
  xQueueGenericReceive,
  QueueHandle_t,
  void * const,
  TickType_t,
  const BaseType_t
);

DECLARE_FAKE_VALUE_FUNC(
  BaseType_t,
  xQueueGenericSendFromISR,
  QueueHandle_t,
  const void * const,
  BaseType_t * const,
  const BaseType_t
);

#endif /* FREERTOS_MOCKS_H */
