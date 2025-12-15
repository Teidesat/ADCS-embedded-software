#pragma once

#include <string>

#include "../../utils/rotationMath.hpp"

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>

class AdafruitLSM9DS1 {
    private:
        Adafruit_LSM9DS1 LSM9DS1;

        std::string axisSensorToString(const SensorData& sensorData);
    
    public:
        float temperature;
        SensorData accelerometers; // meters/seconds^2
        SensorData gyroscopes; // radians/second
        SensorData magnetometers; // gauss

        void begin(const int& SDAPin, const int& SCLPin, int AccelerometersRange = 0, int GyroscopesScale = 0, int MagnetometersGain = 0);
        void update();
        
        std::string accelerometersToString();
        std::string gyroscopesToString();
        std::string magnetometersToString();
        std::string temperatureToString();
        std::string allSensorsToString();
};