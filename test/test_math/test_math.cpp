#include "../../lib/math/eulerAngles/eulerAngles.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

#include <unity.h>

#include "../../lib/math/constants.hpp"

// EulerAngles tests

// convert radians to degrees

void test_convertRadiansToDegrees_0degrees() {
    const double radians = 0.0;
    const double expectedDegrees = 0.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_90degrees() {
    const double radians = 1.5707963267948966;
    const double expectedDegrees = 90.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_180degrees() {
    const double radians = math::Constants::pi;
    const double expectedDegrees = 180.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_270degrees() {
    const double radians = 4.7123889803846897;
    const double expectedDegrees = 270.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_360degrees() {
    const double radians = math::Constants::pi * 2;
    const double expectedDegrees = 360.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_361degrees() {
    const double radians = 6.3006385996995298;
    const double expectedDegrees = 361.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_negative90degrees() {
    const double radians = -1.5707963267948966;
    const double expectedDegrees = -90.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}

void test_convertRadiansToDegrees_negative361degrees() {
    const double radians = -6.3006385996995298;
    const double expectedDegrees = -361.0;
    const double degrees = math::EulerAngles::radiansToDegrees(radians);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedDegrees, degrees);
}


// convert degrees to radians


// convert euler angles to quaternions

void test_eulerAnglesToQuaternion_levelFlight() {
    const double roll = 0.0;
    const double pitch = 0.0;
    const double yaw = 0.0;
    const math::EulerAngles eulerAngles(roll, pitch, yaw);
    const math::Quaternion quaternion = eulerAngles.toQuaternion();

    const double expectedW = 1.0;
    const double expectedX = 0.0;
    const double expectedY = 0.0;
    const double expectedZ = 0.0;
    
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedW, quaternion.w);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedX, quaternion.x);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedY, quaternion.y);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedZ, quaternion.z);
}




// Quaternion tests

// convert quaternions to euler angles

void test_quaternionToEulerAngles_levelFlight() {
    const double w = 1.0;
    const double x = 0.0;
    const double y = 0.0;
    const double z = 0.0;
    const math::Quaternion quaternion(w, x, y, z);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = 0.0;
    const double expectedPitchRadians = 0.0;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = 0.0;
    const double expectedPitchDegrees = 0.0;
    const double expectedYawDegrees = 0.0;
 
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90RightRoll() { // is this to the right?
    const double w = 0.7071067811865476;
    const double x = 0.7071067811865476;
    const double y = 0.0;
    const double z = 0.0;
    const math::Quaternion quaternion(w, x, y, z);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = 1.5707963267948966;
    const double expectedPitchRadians = 0.0;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = 90.0;
    const double expectedPitchDegrees = 0.0;
    const double expectedYawDegrees = 0.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90LeftRoll() { // is this to the left?
    const double w = -0.7071067811865475;
    const double x = 0.7071067811865476;
    const double y = 0.0;
    const double z = 0.0;
    const math::Quaternion quaternion(w, x, y, z);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = -1.5707963267948966;
    const double expectedPitchRadians = 0.0;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = -90.0;
    const double expectedPitchDegrees = 0.0;
    const double expectedYawDegrees = 0.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_invertedFlight() {
    const double w = 0.0;
    const double x = 1.0;
    const double y = 0.0;
    const double z = 0.0;
    const math::Quaternion quaternion(w, x, y, z);
    const math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = math::Constants::pi;
    const double expectedPitchRadians = 0.0;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = 180.0;
    const double expectedPitchDegrees = 0.0;
    const double expectedYawDegrees = 0.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90DegreesPitchUp() {
    const double w = 0.7071067811865475;
    const double x = 0.0;
    const double y = 0.7071067811865476;
    const double z = 0.0;
    math::Quaternion quaternion(w, x, y, z);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = 0.0;
    const double expectedPitchRadians = 1.5707963267948966;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = 0.0;
    const double expectedPitchDegrees = 90.0;
    const double expectedYawDegrees = 0.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90DegreesPitchDown() {
    const double w = -0.7071067811865475;
    const double x = 0.0;
    const double y = 0.7071067811865476;
    const double z = 0.0;
    math::Quaternion quaternion(w, x, y, z);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = 0.0;
    const double expectedPitchRadians = -1.5707963267948966;
    const double expectedYawRadians = 0.0;
    const double expectedRollDegrees = 0.0;
    const double expectedPitchDegrees = -90.0;
    const double expectedYawDegrees = 0.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_90DegreesPitchUp_45Yaw() {
    const double w = 0.6532814824381884;
    const double x = -0.2705980500730985;
    const double y = 0.6532814824381883;
    const double z = 0.2705980500730985;
    math::Quaternion quaternion(w, x, y, z);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    const double expectedRollRadians = 0.0;
    const double expectedPitchRadians = 1.5707963267948966;
    const double expectedYawRadians = 0.7853981633974483;
    const double expectedRollDegrees = 0.0;
    const double expectedPitchDegrees = 90.0;
    const double expectedYawDegrees = 45.0;

    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void test_quaternionToEulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw() {
    const double w = 0.9764419474228081;
    const double x = 0.1663917568952356;
    const double y = 0.1359304882729864;
    const double z = 0.0199451474885093;
    math::Quaternion quaternion(w, x, y, z);
    math::EulerAngles eulerAngles = quaternion.toEulerAngles();

    //const double expectedRollRadians = 0.3490658503988659;
    //const double expectedPitchRadians = 0.2617993877991494;
    //const double expectedYawRadians = 0.0872664625997165;
    const double expectedRollDegrees = 20;
    const double expectedPitchDegrees = 15;
    const double expectedYawDegrees = 5;

    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollRadians, eulerAngles.roll);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchRadians, eulerAngles.pitch);
    //TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawRadians, eulerAngles.yaw);
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedRollDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.roll));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedPitchDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.pitch));
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, expectedYawDegrees, math::EulerAngles::radiansToDegrees(eulerAngles.yaw));
}

void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();

    // EulerAngles tests

    // convert radians to degrees
    RUN_TEST(test_convertRadiansToDegrees_0degrees);
    RUN_TEST(test_convertRadiansToDegrees_90degrees);
    RUN_TEST(test_convertRadiansToDegrees_180degrees);
    RUN_TEST(test_convertRadiansToDegrees_270degrees);
    RUN_TEST(test_convertRadiansToDegrees_360degrees);
    RUN_TEST(test_convertRadiansToDegrees_361degrees);
    RUN_TEST(test_convertRadiansToDegrees_negative90degrees);
    RUN_TEST(test_convertRadiansToDegrees_negative361degrees);
    // convert degrees to radians
    // convert euler angles to quaternions
    RUN_TEST(test_eulerAnglesToQuaternion_levelFlight);



    // Quaternion tests

    // convert quaternions to euler angles
    RUN_TEST(test_quaternionToEulerAngles_levelFlight);
    RUN_TEST(test_quaternionToEulerAngles_90RightRoll);
    RUN_TEST(test_quaternionToEulerAngles_90LeftRoll);
    RUN_TEST(test_quaternionToEulerAngles_invertedFlight);
    RUN_TEST(test_quaternionToEulerAngles_90DegreesPitchUp);
    RUN_TEST(test_quaternionToEulerAngles_90DegreesPitchDown);
    RUN_TEST(test_quaternionToEulerAngles_90DegreesPitchUp_45Yaw);
    RUN_TEST(test_quaternionToEulerAngles_20degreesRoll_15DegreesPitchUp_5degreesYaw);

    UNITY_END();
}

void loop() {}

int main() {
    setup();
    return 0;
}