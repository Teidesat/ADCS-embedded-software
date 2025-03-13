// main-adafruit-fork-test.txt

#include "../lib/algorithms/adafruitFork/MadgwickAHRS.hpp"

#include <string>
#include <fstream>

#include <unity.h>


void test_two_values_are_equal() {
  TEST_ASSERT_EQUAL(1, 1);
}

void draw_to_file() {
  std::ofstream outputfile;
  outputfile.open ("attitude.csv");

  std::string message = "roll,";
  outputfile << message + "\n";

  outputfile.close();
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_two_values_are_equal);
  RUN_TEST(draw_to_file);
  UNITY_END();

  return 0;
}