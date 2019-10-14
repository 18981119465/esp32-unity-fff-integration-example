#include "freertos_mocks.h"

DEFINE_FAKE_VALUE_FUNC(
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
