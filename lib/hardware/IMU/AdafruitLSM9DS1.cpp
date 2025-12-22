#include "AdafruitLSM9DS1.hpp"

#include <Wire.h>

void AdafruitLSM9DS1::begin(const int iSDAPin, const int iSCLPin, int iAccelerometersRange, int iGyroscopesScale, int iMagnetometersGain) {
    // Set pins for the I2C connection
    Wire.begin(iSDAPin, iSCLPin);

    // Try to initialise and warn if we couldn't detect the chip
    if(!LSM9DS1.begin()) {
        Serial.println("Unable to initialize the LSM9DS1");
        while (1);
    }

    // set sensors range / sensitivity
    LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_2G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_4G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_8G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_16G);
  
    LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_245DPS);
    //LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_500DPS);
    //LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_2000DPS);

    LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_4GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_8GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_12GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_16GAUSS);
}

void AdafruitLSM9DS1::update() {
    sensors_event_t accelerometers_event_t, magnetometers_event_t, gyroscopes_event_t, temperature_event_t;
    LSM9DS1.getEvent(&accelerometers_event_t, &magnetometers_event_t, &gyroscopes_event_t, &temperature_event_t);

    temperature = temperature_event_t.temperature;
  
    accelerometers.x = accelerometers_event_t.acceleration.x;
    accelerometers.y = accelerometers_event_t.acceleration.y;
    accelerometers.z = accelerometers_event_t.acceleration.z;

    gyroscopes.x = gyroscopes_event_t.gyro.x;
    gyroscopes.y = gyroscopes_event_t.gyro.y;
    gyroscopes.z = gyroscopes_event_t.gyro.z;

    magnetometers.x = magnetometers_event_t.magnetic.x;
    magnetometers.y = magnetometers_event_t.magnetic.y;
    magnetometers.z = magnetometers_event_t.magnetic.z;
}

Vector3d AdafruitLSM9DS1::getAccelerometers() const {return accelerometers;}
Vector3d AdafruitLSM9DS1::getGyroscopes() const {return gyroscopes;}
Vector3d AdafruitLSM9DS1::getMagnetometers() const {return magnetometers;}
float AdafruitLSM9DS1::getTemperature() const {return temperature;}