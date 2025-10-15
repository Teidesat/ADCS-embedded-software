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
        //lastNMEAmessage.push_back(Serial2.read());

        //if(lastNMEAmessage.back() == '\n') {
        //    Serial.println(lastNMEAmessage.c_str());
        //    lastNMEAmessage = "";
        //}
        char a = Serial2.read();
        Serial.write(a);
    }
}

void UbloxNEO6M::shortInfo() {
    const byte getVersionCommand[] = { 
      0xB5, 0x62, 
      0x0A, 0x04, 
      0x00, 0x00, 
      0x0E, 0x34
    };

    int getVersionCommandSize = sizeof(getVersionCommand);
    for(size_t i = 0; i < getVersionCommandSize; i++) {
        Serial2.write(getVersionCommand[i]);
    }

    delay(150); 
    
    long startTime = millis();
    while ((millis() - startTime) < 500) { 
        if(Serial2.available()) {Serial.write(Serial2.read());}
    }
}