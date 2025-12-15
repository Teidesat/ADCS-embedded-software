#include "UbloxNEO6M.hpp"

UbloxNEO6M::UbloxNEO6M(): 
    Serial2(2),
    NMMEAmessagesHistoryMaxSize(2)
{}

void UbloxNEO6M::begin(const int& RXPin, const int& TXPin) {
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RXPin, TXPin);
}

void UbloxNEO6M::update() {
    static bool receivingSentence = false;
    std::string outputString = "";
    while (Serial2.available()) {
        char incomingChar = Serial2.read();

        if (incomingChar == '$') {
            lastNMEAmessage.clear();
            receivingSentence = true;
            lastNMEAmessage.push_back(incomingChar);
            continue;
        }
        if (receivingSentence) {
            if (lastNMEAmessage.length() < MAX_NMEA_LENGTH) {lastNMEAmessage.push_back(incomingChar);} 
            else {
                receivingSentence = false;
                lastNMEAmessage.clear();
                continue;
            }
            
            if (incomingChar == '\n' || incomingChar == '\r') {
                outputString = lastNMEAmessage;
                receivingSentence = false;
                lastNMEAmessage.clear();
            }
        }
    }
}

void UbloxNEO6M::printNMEAmessages() {
    while (Serial2.available() > 0) {
        char receivedCharacter = Serial2.read();
        Serial.write(receivedCharacter);
    }
}

std::string UbloxNEO6M::getLastNMEAmessage() {return lastNMEAmessage;}