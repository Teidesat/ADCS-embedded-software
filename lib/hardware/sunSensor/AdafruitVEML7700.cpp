#include "AdafruitVEML7700.hpp"

#include "../../utils/stringFormatting.hpp"

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

void AdafruitVEML7700::update() {
    ALS = adafruitVEML7700.readALS();
    white = adafruitVEML7700.readWhite();
    lux = adafruitVEML7700.readLux();
}

std::string AdafruitVEML7700::sensorDataToString() {
    //std::string ALSStr = floatToFormattedString(ALS);
    //std::string whiteStr = floatToFormattedString(white);
    std::string ALSStr = std::to_string(ALS);
    std::string whiteStr = std::to_string(white);
    std::string luxStr = std::to_string(lux);

    return ALSStr + STRING_SEPARATOR + whiteStr + STRING_SEPARATOR + luxStr;
}