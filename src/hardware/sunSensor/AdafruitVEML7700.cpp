#include "AdafruitVEML7700.hpp"

#include <Wire.h>

void AdafruitVEML7700::begin(const int sdaPin, 
                             const int sclPin, 
                             const float lowThreshold, 
                             const float highThreshold, 
                             const bool interruptEnable, 
                             const int frequencyClock) {
    Wire.begin(sdaPin, sclPin);
    if(frequencyClock > 0) {Wire.setClock(frequencyClock);}

    if (!adafruitVEML7700.begin()) {
        Serial.println("Unable to initialize the VEML7700");
        while(true) {delay(1);}
    }
    
    adafruitVEML7700.setLowThreshold(lowThreshold);
    adafruitVEML7700.setHighThreshold(highThreshold);
    adafruitVEML7700.interruptEnable(interruptEnable);
}

void AdafruitVEML7700::update() {
    ALS = adafruitVEML7700.readALS();
    white = adafruitVEML7700.readWhite();
    lux = adafruitVEML7700.readLux();
}

float AdafruitVEML7700::getALS() const {return ALS;}
float AdafruitVEML7700::getWhite() const {return white;}
float AdafruitVEML7700::getLux() const {return lux;}