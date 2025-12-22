#include "AdafruitVEML7700.hpp"

#include <Wire.h>

void AdafruitVEML7700::begin(const int iSDAPin, const int iSCLPin, const float iLowThreshold, const float iHighThreshold, const bool iInterruptEnable) {
    Wire.begin(iSDAPin, iSCLPin);
    if (!adafruitVEML7700.begin()) {
        Serial.println("Unable to initialize the VEML7700");
        while (1); {delay(1);} // will pause until serial console opens
    }
    
    adafruitVEML7700.setLowThreshold(iLowThreshold);
    adafruitVEML7700.setHighThreshold(iHighThreshold);
    adafruitVEML7700.interruptEnable(iInterruptEnable);
}

void AdafruitVEML7700::update() {
    ALS = adafruitVEML7700.readALS();
    white = adafruitVEML7700.readWhite();
    lux = adafruitVEML7700.readLux();
}

float AdafruitVEML7700::getALS() const {return ALS;}
float AdafruitVEML7700::getWhite() const {return white;}
float AdafruitVEML7700::getLux() const {return lux;}