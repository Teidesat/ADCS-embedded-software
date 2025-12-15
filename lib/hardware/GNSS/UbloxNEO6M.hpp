#pragma once

#include <string>
#include <vector>

#include <HardwareSerial.h>
#include <TinyGPS++.h>

class UbloxNEO6M {
    private:
        // UART protocol pins
        const int BAUD_RATE = 9600;
        const int MAX_NMEA_LENGTH = 120;
        const int NMMEAmessagesHistoryMaxSize;
        std::vector<std::string> NMMEAmessagesHistory;
        std::string lastNMEAmessage;

        HardwareSerial Serial2;
        TinyGPSPlus NMEAparser;

    public:
        UbloxNEO6M();
        void begin(const int& RXPin, const int& TXPin);
        void update();
        void printNMEAmessages();
        std::string getLastNMEAmessage();
};