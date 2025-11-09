#pragma once

#include <string>

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>

// structures for the retrival of the sensors readings
class SensorData {
    public:
        float x, y, z;
};

class AdafruitLSM9DS1 {
    private:
        Adafruit_LSM9DS1 LSM9DS1;

        std::string axisSensorToString(const SensorData& sensorData);
    
    public:
        float temperature;
        SensorData accelerometers;
        SensorData gyroscopes;
        SensorData magnetometers;


        void begin(const int& SDAPin, const int& SCLPin, int AccelerometersRange = 0, int GyroscopesScale = 0, int MagnetometersGain = 0);
        void update();
        float getTemperature();
        
        std::string accelerometersToString();
        std::string gyroscopesToString();
        std::string magnetometersToString();
        std::string temperatureToString();
        std::string allSensorsToString();
};