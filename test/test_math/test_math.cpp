#include "../../lib/math/eulerAngles/eulerAngles.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

#include <unity.h>

#include "../../lib/math/constants.hpp"

// Quaternion tests

void test_quaternionToEulerAngles_levelFlight() {
    const math::Quaternion quaternion(1.0, 0.0, 0.0, 0.0);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();
 
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90RightRoll() { // is this to the right?
    const math::Quaternion quaternion(0.707107, 0.707107, 0.0, 0.0);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 1.570796945692514, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 90.0, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90LeftRoll() { // is this to the left?
    const math::Quaternion quaternion(0.707107, 0.707107, 0.0, 0.0);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();
    //const EulerAngles eulerAnglesDegrees = eulerAngles.radiansToDegrees();

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 1.570796, eulerAngles.roll);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.pitch);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.yaw);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 90.0, eulerAnglesDegrees.roll);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAnglesDegrees.pitch);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAnglesDegrees.yaw);
}

void test_quaternionToEulerAngles_levelFlight_inverted() {
    const math::Quaternion quaternion(0.0, 1.0, 0.0, 0.0);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 3.141593, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 180.0, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90DegreesPitchUp() {
    math::Quaternion quaternion(0.707107, 0.0, 0.707107, 0.0);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, eulerAngles.roll); // result 3.141593 // gimbal lock
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 1.570796, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, eulerAngles.yaw);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 90.0, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90DegreesPitchDown() {
    //math::Quaternion quaternion(0.707107f, 0.0f, 0.707107f, 0.0f);
    //math::EulerAngles eulerAngles = quaternion.toEulerAngles();
    //math::EulerAngles eulerAnglesDegrees = EulerAngles::toDegrees(eulerAngles);

    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, eulerAngles.roll); // result 3.141593 gimbal lock
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 1.570796f, eulerAngles.pitch);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, eulerAngles.yaw);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, eulerAnglesDegrees.roll); // result 180.f
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 90.0f, eulerAnglesDegrees.pitch);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0f, eulerAnglesDegrees.yaw);
}

void test_quaternionToEulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw() {
    math::Quaternion quaternion(0.976442, 0.166392, 0.135930, 0.019945);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.349066, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.261799, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.087266, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 20.0, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 15.0, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 5.0, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_compare_quaternions_levelFlight_inverted() {
    math::Quaternion quaternion1(0.0, 1.0, 0.0, 0.0);
    math::EulerAngles eulerAngles1 = quaternion1.toEulerAngles();
    math::Quaternion quaternion2(0.0, -1.0, 0.0, 0.0);
    math::EulerAngles eulerAngles2 = quaternion2.toEulerAngles();

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, eulerAngles1.roll, eulerAngles2.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, eulerAngles1.pitch, eulerAngles2.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, eulerAngles1.yaw, eulerAngles2.yaw);
    TEST_ASSERT_TRUE(quaternion1 == quaternion2);
}

// EulerAngles tests

void test_eulerAnglesToQuaternion_levelFlight() {
    const math::EulerAngles eulerAngles(0.0, 0.0, 0.0);
    const math::Quaternion quaternion = eulerAngles.toQuaternion();
    
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 1.0, quaternion.w);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, quaternion.x);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, quaternion.y);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, 0.0, quaternion.z);
}

void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();

    // Quaternion tests

    // test quaternion to euler angles
    RUN_TEST(test_quaternionToEulerAngles_levelFlight);
    RUN_TEST(test_quaternionToEulerAngles_90RightRoll);
    RUN_TEST(test_quaternionToEulerAngles_90LeftRoll);
    RUN_TEST(test_quaternionToEulerAngles_levelFlight_inverted);
    RUN_TEST(test_quaternionToEulerAngles_90DegreesPitchUp);
    RUN_TEST(test_quaternionToEulerAngles_90DegreesPitchDown);
    RUN_TEST(test_quaternionToEulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw);

    // test quaternion comparison
    //RUN_TEST(test_compare_quaternions_levelFlight_inverted);

    // EulerAngles tests

    RUN_TEST(test_eulerAnglesToQuaternion_levelFlight);

    UNITY_END();
}

void loop() {}

int main() {
    setup();
    return 0;
}