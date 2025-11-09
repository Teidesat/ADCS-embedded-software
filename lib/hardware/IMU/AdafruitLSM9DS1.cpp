#include "AdafruitLSM9DS1.hpp"

#include "../../utils/stringFormatting.hpp"

void AdafruitLSM9DS1::begin(const int& SDAPin, const int& SCLPin, int AccelerometersRange, int GyroscopesScale, int MagnetometersGain) {
    // Set pins for the I2C connection
    Wire.begin(SDAPin, SCLPin);

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

std::string AdafruitLSM9DS1::axisSensorToString(const SensorData& sensorData) {
    //std::string xStr = floatToFormattedString(sensorData.x);
    //std::string yStr = floatToFormattedString(sensorData.y);
    //std::string zStr = floatToFormattedString(sensorData.z);
    std::string xStr = std::to_string(sensorData.x);
    std::string yStr = std::to_string(sensorData.y);
    std::string zStr = std::to_string(sensorData.z);

    return xStr + STRING_SEPARATOR + yStr + STRING_SEPARATOR + zStr;
}

std::string AdafruitLSM9DS1::accelerometersToString() {return axisSensorToString(accelerometers);}
std::string AdafruitLSM9DS1::gyroscopesToString() {return axisSensorToString(gyroscopes);}
std::string AdafruitLSM9DS1::magnetometersToString() {return axisSensorToString(magnetometers);}
std::string AdafruitLSM9DS1::temperatureToString() {return std::to_string(temperature);}

std::string AdafruitLSM9DS1::allSensorsToString() {
    return accelerometersToString() + STRING_SEPARATOR + gyroscopesToString() + STRING_SEPARATOR + magnetometersToString() + STRING_SEPARATOR + temperatureToString();
}

