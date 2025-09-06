#pragma once

//#include <string>

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>

// structures for the retrival of the sensors readings
class SensorData {
    public:
        float x, y, z;
};

class Attitude {
    public:
        float pitch, roll, yaw;
};

class AdafruitLSM9DS1 {
    private:
        const char STRING_SEPARATOR = ',';
        Adafruit_LSM9DS1 LSM9DS1;

        String floatToFormattedString(float number);
        String singleSensorToString(const SensorData& sensorData);
    
    public:
        float temperature;
        SensorData accelerometers;
        SensorData gyroscopes;
        SensorData magnetometers;
        Attitude attitude;

        void begin(const int& SDAPin, const int& SCLPin);
        void update();
        float getTemperature();
        
        String accelerometersToString();
        String gyroscopesToString();
        String magnetometersToString();
        String temperatureToString();
        String allSensorsToString();
        String attitudeToString();
};