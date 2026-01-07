#include <unity.h>

#include "../../lib/algorithm/attitudeDetermination/attitudeDetermination.hpp"

const float EPSILON = 0.001f;

void test_suite_attitudeDetermination() {
    const Vector3d accelerometers(0.0f, 9.81f, 0.0f);
    const Vector3d gyroscopes(0.0f, 0.0f, 0.0f);
    const Vector3d magnetometers(0.0f, 0.0f, 0.0f);

    const float lux = 0;
    const float deltatime = 0.0;

    AttitudeAlgorithm attitudeAlgorithm;
    attitudeAlgorithm.update(accelerometers, gyroscopes, magnetometers, lux, deltatime);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, 0.0f);
}


void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();
    // TEST ATTITUDE FROM ACCELERATIONS AND MAGNETIC FLUX
    RUN_TEST(test_suite_attitudeDetermination);

    // TEST ATTITUDE FROM GYROSCOPES

    // TEST BLEND RELATIVE AND ABSOLUTE ATTITUDE
    UNITY_END();
}

#ifdef ARDUINO
void loop() {}
#endif

int main(int argc, char **argv) {
    setup();
    return 0;
}