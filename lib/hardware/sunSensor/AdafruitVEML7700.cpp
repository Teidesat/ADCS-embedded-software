#include "AdafruitVEML7700.hpp"

void AdafruitVEML7700::begin(const int& SDAPin, const int& SCLPin) {
    // Set pins for the I2C connection
    Wire.begin(SDAPin, SCLPin);

    if (!adafruitVEML7700.begin()) {
        Serial.println("Unable to initialize the VEML7700");
        while (1);
    }
    
    adafruitVEML7700.setLowThreshold(10000);
    adafruitVEML7700.setHighThreshold(20000);
    adafruitVEML7700.interruptEnable(true);
}
