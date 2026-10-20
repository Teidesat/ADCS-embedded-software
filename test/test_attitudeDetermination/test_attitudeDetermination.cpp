#include "../../src/algorithm/attitudeDetermination/attitudeDetermination.hpp"
#include "../../src/algorithm/attitudeDetermination/attitudeDetermination.cpp"

#include <unity.h>

#include "../../lib/math/constants.hpp"

void test_suite_attitudeDetermination() {
    const math::Vector3d accelerometers(0.0f, 9.81f, 0.0f);
    const math::Vector3d gyroscopes(0.0f, 0.0f, 0.0f);
    const math::Vector3d magnetometers(0.0f, 0.0f, 0.0f);

    const float lux = 0;
    const float deltatime = 0.0;

    ADCS::AttitudeAlgorithm attitudeAlgorithm;
    attitudeAlgorithm.update(accelerometers, gyroscopes, magnetometers, lux, deltatime);

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, 0.0f);
}

void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();

    // test attitude from accelerations and magnetic flux
    RUN_TEST(test_suite_attitudeDetermination);

    // test attitude from gyroscopes
    // test blend relative and absolute attitude

    UNITY_END();
}

void loop() {}

int main() {
    setup();
    return 0;
}