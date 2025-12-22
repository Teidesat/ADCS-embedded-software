#pragma once

#include <Adafruit_VEML7700.h>

class AdafruitVEML7700 {
    private:
        Adafruit_VEML7700 adafruitVEML7700;
        float ALS; // raw ambient light sensor value
        float white; //total light captured by the sensor (both visible and infrared)
        float lux; // amount of light received by the sensor
        
    public:
        void begin(const int iSDAPin, const int iSCLPin, const float iLowThreshold = 10000.0f, const float iHighThreshold = 20000.0f, const bool iInterruptEnable = true);
        void update();
        
        float getALS() const;
        float getWhite() const;
        float getLux() const;
};