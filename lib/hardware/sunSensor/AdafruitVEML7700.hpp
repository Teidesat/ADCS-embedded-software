#pragma once

#include <Wire.h>
#include <Adafruit_VEML7700.h>

class AdafruitVEML7700 {
    private:
        Adafruit_VEML7700 adafruitVEML7700;
        
    public:
        float ALS; // raw ambient light sensor value
        float white; //total light captured by the sensor (both visible and infrared)
        float lux; // amount of light received by the sensor
        
        void begin(const int& SDAPin, const int& SCLPin);
        void update();
        std::string sensorDataToString();
};