#ifndef FREERTOS_MOCKS_H
#define FREERTOS_MOCKS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
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

#endif /* FREERTOS_MOCKS_H */
