#pragma once

#include <Wire.h>
#include <Adafruit_VEML7700.h>

class AdafruitVEML7700 {
    public:
        Adafruit_VEML7700 adafruitVEML7700;
        
        void begin(const int& SDAPin, const int& SCLPin);
};