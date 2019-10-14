#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "mockable.h"


static const char TAG[] = "[mockable]";


TEST_CASE(
  "mockable_function(): "
  "Should return 0",
  TAG)
{
  TEST_ASSERT_EQUAL_INT(0, mockable_function());
}
