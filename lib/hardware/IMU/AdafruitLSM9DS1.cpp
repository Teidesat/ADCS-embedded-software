#include "AdafruitLSM9DS1.hpp"

#include <Wire.h>

void AdafruitLSM9DS1::begin(const int SDApin, 
                            const int SCLpin, 
                            const AcclerometersRange accelerometersRange, 
                            const GyroscopesRange gyroscopesRange,
                            const MagnetometersRange magnetometersRange,
                            const int frequencyClock) {
    Wire.begin(SDApin, SCLpin);
    if(frequencyClock > 0) {Wire.setClock(frequencyClock);}
    
    if(!LSM9DS1.begin()) {
        Serial.println("Unable to initialize the LSM9DS1");
        while(true) {delay(1);}
    }

    switch(accelerometersRange) {
        case AcclerometersRange::G4:
            LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_4G);
            break;
        case AcclerometersRange::G8:
            LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_8G);
            break;
        case AcclerometersRange::G16:
            LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_16G);
            break;
        case AcclerometersRange::G2:
        default:
            LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_2G);
            break;
    }

    switch(gyroscopesRange) {
        case GyroscopesRange::DPS500:
            LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_500DPS);
            break;
        case GyroscopesRange::DPS2000:
            LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_2000DPS);
            break;
        case GyroscopesRange::DPS245:
        default:
            LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_245DPS);
            break;
    }

    switch(magnetometersRange) {
        case MagnetometersRange::GAUSS8:
            LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_8GAUSS);
            break;
        case MagnetometersRange::GAUSS12:
            LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_12GAUSS);
            break;
        case MagnetometersRange::GAUSS16:
            LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_16GAUSS);
            break;
        case MagnetometersRange::GAUSS4:
        default:
            LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_4GAUSS);
            break;
    }
}

void AdafruitLSM9DS1::update() {
    sensors_event_t accelerometers_event_t, magnetometers_event_t, gyroscopes_event_t, temperature_event_t;
    LSM9DS1.getEvent(&accelerometers_event_t, &magnetometers_event_t, &gyroscopes_event_t, &temperature_event_t);

    accelerometers = Vector3d(accelerometers_event_t.acceleration.x, accelerometers_event_t.acceleration.y, accelerometers_event_t.acceleration.z);
    gyroscopes = Vector3d(gyroscopes_event_t.gyro.x, gyroscopes_event_t.gyro.y, gyroscopes_event_t.gyro.z);
    magnetometers = Vector3d(magnetometers_event_t.magnetic.x, magnetometers_event_t.magnetic.y, magnetometers_event_t.magnetic.z);
    temperature = temperature_event_t.temperature;
}

Vector3d AdafruitLSM9DS1::getAccelerometers() const {return accelerometers;}
Vector3d AdafruitLSM9DS1::getGyroscopes() const {return gyroscopes;}
Vector3d AdafruitLSM9DS1::getMagnetometers() const {return magnetometers;}
float AdafruitLSM9DS1::getTemperature() const {return temperature;}