#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "testable.h"


static const char TAG[] = "[testable]";


TEST_CASE("testable_calls_mockable() "
          "should return true if mockable_function() returns zero",
          TAG)
{
  TEST_ASSERT_TRUE(testable_calls_mockable());
}

TEST_CASE("testable_calls_mockable() "
          "should return false if mockable_function() returns non-zero",
          TAG)
{
  TEST_ASSERT_FALSE(testable_calls_mockable());
}
