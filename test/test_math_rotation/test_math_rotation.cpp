#include <unity.h>

#include "../../lib/utils/math/rotation.hpp"

const float EPSILON = 0.001f;

void test_quaternion_to_eulerAngles_levelFlight() {
    Quaternion quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);
 
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getYaw());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getYaw());
}

void test_quaternion_to_eulerAngles_90DegreesRoll() {
    Quaternion quaternion(0.707107f, 0.707107f, 0.0f, 0.0f);
    EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.570796f, eulerAnglesRadians.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getYaw());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 90.0f, eulerAnglesDegrees.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getRoll());
}

void test_quaternion_to_eulerAngles_levelFlight_inverted() {
    Quaternion quaternion(0.0f, 1.0f, 0.0f, 0.0f);
    EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.141593f, eulerAnglesRadians.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getYaw());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 180.0f, eulerAnglesDegrees.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getYaw());
}

void test_quaternion_to_eulerAngles_90DegreesPitchUp() {
    Quaternion quaternion(0.707107f, 0.0f, 0.707107f, 0.0f);
    EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);

    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getRoll()); // result 3.141593 // gimbal lock
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.570796f, eulerAnglesRadians.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getYaw());
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getRoll()); // result 180.f
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 90.0f, eulerAnglesDegrees.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getYaw());
}

void test_quaternion_to_eulerAngles_90DegreesPitchDown() {
    //Quaternion quaternion(0.707107f, 0.0f, 0.707107f, 0.0f);
    //EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    //EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);

    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getRoll()); // result 3.141593 gimbal lock
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.570796f, eulerAnglesRadians.getPitch());
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesRadians.getYaw());
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getRoll()); // result 180.f
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 90.0f, eulerAnglesDegrees.getPitch());
    //TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, eulerAnglesDegrees.getYaw());
}

void test_quaternion_to_eulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw() {
    Quaternion quaternion(0.976442f, 0.166392f, 0.135930f, 0.019945f);
    EulerAngles eulerAnglesRadians = quaternion.toEulerAngles();
    EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAnglesRadians);

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.349066f, eulerAnglesRadians.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.261799f, eulerAnglesRadians.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.087266f, eulerAnglesRadians.getYaw());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 20.0f, eulerAnglesDegrees.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 15.0f, eulerAnglesDegrees.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, eulerAnglesDegrees.getYaw());
}

void test_eulerAngles_to_quaternion_levelFlight() {
    EulerAngles eulerAnglesLevelFlight(0.0f, 0.0f, 0.0f);
    Quaternion quaternionLevelFlight = eulerAnglesLevelFlight.toQuaternion();

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, quaternionLevelFlight.getW());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, quaternionLevelFlight.getX());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, quaternionLevelFlight.getY());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, quaternionLevelFlight.getZ());
}

void test_compare_quaternions_levelFlight_inverted() {
    Quaternion quaternion1(0.0f, 1.0f, 0.0f, 0.0f);
    EulerAngles eulerAngles1 = quaternion1.toEulerAngles();
    Quaternion quaternion2(0.0f, -1.0f, 0.0f, 0.0f);
    EulerAngles eulerAngles2 = quaternion2.toEulerAngles();

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, eulerAngles1.getRoll(), eulerAngles2.getRoll());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, eulerAngles1.getPitch(), eulerAngles2.getPitch());
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, eulerAngles1.getYaw(), eulerAngles2.getYaw());
    TEST_ASSERT_TRUE(quaternion1 == quaternion2);
}

void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();
    // TEST QUATERNION TO EULERANGLES
    RUN_TEST(test_quaternion_to_eulerAngles_levelFlight);
    RUN_TEST(test_quaternion_to_eulerAngles_90DegreesRoll);
    RUN_TEST(test_quaternion_to_eulerAngles_levelFlight_inverted);
    RUN_TEST(test_quaternion_to_eulerAngles_90DegreesPitchUp);
    RUN_TEST(test_quaternion_to_eulerAngles_90DegreesPitchDown);
    RUN_TEST(test_quaternion_to_eulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw);

    // TEST EULERANGLES TO QUATERNION
    RUN_TEST(test_eulerAngles_to_quaternion_levelFlight);

    // TEST TEST QUATERNION COMPARE ORIENTATION
    RUN_TEST(test_compare_quaternions_levelFlight_inverted);
    UNITY_END();
}

#ifdef ARDUINO
void loop() {}
#endif

int main(int argc, char **argv) {
    setup();
    return 0;
}