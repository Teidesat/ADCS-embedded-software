#include <unity.h>

#include "../lib/utils/rotation.hpp"

int add(int a, int b) {return a + b;}

void test_addition(void) {
  TEST_ASSERT_EQUAL_INT(4, add(2, 2));
  TEST_ASSERT_NOT_EQUAL(5, add(2, 2));
}

void test_negative(void) {
  TEST_ASSERT_EQUAL_INT(-2, add(-1, -1));
}

void setup() {
  delay(2000); // wait for serial monitor
  UNITY_BEGIN();
  RUN_TEST(test_addition);
  RUN_TEST(test_negative);
  UNITY_END();
}

void loop() {}