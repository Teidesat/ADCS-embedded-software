#include "../../src/algorithm/WMM/worldMagneticModel.hpp"
#include "../../src/algorithm/WMM/worldMagneticModel.cpp"

#include <unity.h>

#include "../../lib/math/constants.hpp"

// test decimal date

void test_worldMagneticModel_decimalDate2025() {
    const int day = 1;
    const int month = 1;
    const int year = 2025;
    const float result = 2025.0;
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, result, WMM::dateToYears(day, month, year, WMM::StaticMagneticModel::epoch));
}

void test_worldMagneticModel_decimalDate2026() {
    const int day = 1;
    const int month = 1;
    const int year = 2026;
    const float result = 2026.0;
    TEST_ASSERT_DOUBLE_WITHIN(math::Constants::doubleComparisonThreshold, result, WMM::dateToYears(day, month, year, WMM::StaticMagneticModel::epoch));
}

// test world magnetic model

void test_worldMagneticModel_calculatePointMagneticField() {

    // Field 1: Date
    // Field 2: Height above WGS84 ellipsoid (km)
    // Field 3: Geodetic Latitude (deg)
    // Field 4: Geodetic Longitude (deg)
    // Field 5: X (nT)
    // Field 6: Y (nT)
    // Field 7: Z (nT)
    // Field 8: H (nT)lear
    // Field 9: F (nT)
    // Field 10: Inclination (deg)
    // Field 11: Declination (deg)
    // 2025.0    0.0   80.0    0.0     6521.6      145.9    54791.5     6523.2    55178.5   83.21    1.28 

    const float altitude = 0;
    const float latitude = 80.0;
    const float longitude = 0.0;
    const int day = 1;
    const int moth = 1;
    const int year = 2025;

    WMM::GeomagneticElements geomagneticElements = WMM::calculatePointMagneticField(latitude, longitude, altitude, year, moth, day);

    TEST_ASSERT_FLOAT_WITHIN(math::Constants::doubleComparisonThreshold, 6521.6, geomagneticElements.X);
}


void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();

    // test decimal date
    RUN_TEST(test_worldMagneticModel_decimalDate2025);
    RUN_TEST(test_worldMagneticModel_decimalDate2026);

    // test world magnetic model
    RUN_TEST(test_worldMagneticModel_calculatePointMagneticField);

    UNITY_END();
}

void loop() {}

int main() {
    setup();
    return 0;
}