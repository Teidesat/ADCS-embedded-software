#pragma once

#include <string>

#include <HardwareSerial.h>
#include <TinyGPS++.h>

class UbloxNEO6M {
    private:
        // UART protocol pins
        const int BAUD_RATE = 9600;
        HardwareSerial Serial2;
        std::string lastNMEAmessage;

    public:
        UbloxNEO6M();
        std::string getLastNMEAmessage();
        TinyGPSPlus NMEAparser;
        void begin(const int& RXPin, const int& TXPin);
        void update();
        void shortInfo();
};