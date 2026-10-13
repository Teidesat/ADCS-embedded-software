#include "../../src/algorithm/WMM/worldMagneticModel.hpp"
#include "../../src/algorithm/WMM/worldMagneticModel.cpp"

#include <unity.h>
#include <limits> // use numeric_limits<float>::epsilon() to get the precision of float type?

const float EPSILON = std::numeric_limits<float>::epsilon(); // correct to use this presicion?

void test_worldMagneticModel_decimalDate() {
    const int day1 = 1;
    const int month1 = 1;
    const int year1 = 2025;
    const float result1 = 2025.0;
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, result1, WMM::dateToYears(day1, month1, year1, WMM::StaticMagneticModel::epoch));

    const int day2 = 1;
    const int month2 = 1;
    const int year2 = 2025;
    const float result2 = 2025.0;
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, result2, WMM::dateToYears(day2, month2, year2, WMM::StaticMagneticModel::epoch));
}

/*
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

    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 6521.6, geomagneticElements.X); // for the moment I dont know if it is passing but is wrong or passing and correct value
}
*/

void setUp() {}
void tearDown() {}

void setup() {
    UNITY_BEGIN();

    // TEST WORLD MAGNETIC MODEL
    RUN_TEST(test_worldMagneticModel_decimalDate);

    UNITY_END();
}

#ifdef ARDUINO
void loop() {}
#endif

int main() {
    setup();
    return 0;
}