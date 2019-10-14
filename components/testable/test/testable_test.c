#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"
#include "fff.h"
#include "mockable_mocks.h"
#include "testable.h"

static const char TAG[] = "[testable]";
DEFINE_FFF_GLOBALS;


TEST_CASE(
  "testable_calls_mockable() "
  "should return true if mockable_function() returns zero",
  TAG)
{
  RESET_FAKE(mockable_function);
  mockable_function_fake.return_val = 0;
  TEST_ASSERT_TRUE(testable_calls_mockable());
}

TEST_CASE(
  "testable_calls_mockable() "
  "should return false if mockable_function() returns non-zero",
  TAG)
{
  RESET_FAKE(mockable_function);
  mockable_function_fake.return_val = -1;
  TEST_ASSERT_FALSE(testable_calls_mockable());
}
