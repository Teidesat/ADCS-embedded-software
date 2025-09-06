#include "UbloxNEO6M.hpp"

UbloxNEO6M::UbloxNEO6M(): 
    Serial2(2)
{}

std::string UbloxNEO6M::getLastNMEAmessage() {return lastNMEAmessage;}

void UbloxNEO6M::begin(const int& RXPin, const int& TXPin) {
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RXPin, TXPin);
}

void UbloxNEO6M::update() {
    lastNMEAmessage = "";
    while (Serial2.available() > 0) {
        char character = Serial2.read();
        lastNMEAmessage += character;

        if (character == '\n') {
            // A complete message is received, now you can use it
            Serial.println(lastNMEAmessage.c_str());
            Serial.println("---------------------------------");
            NMEAparser.encode(Serial2.read());
            // Reset the string for the next message
            lastNMEAmessage = "";
        }
    }
    
}