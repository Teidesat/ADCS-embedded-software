#pragma once

#include <string>
#include <vector>

#include <HardwareSerial.h>
#include <TinyGPS++.h>

class UbloxNEO6M {
    private:
        // UART protocol
        HardwareSerial Serial2;
        // NMEA messages
        TinyGPSPlus NMEAparser;
        std::vector<std::string> NMMEAmessagesHistory;
        std::string lastNMEAmessage;

        static const int BAUD_RATE = 9600;
        static const int MAX_NMEA_LENGTH = 82; // standard maximum NMEA sentence length
        static const int NMEA_MESSAGES_HISTORY_MAX_SIZE;
        
    public:
        UbloxNEO6M();
        void begin(const int iRXPin, const int iTXPin);
        void update();
        void printNMEAmessages();
        std::string getLastNMEAmessage() const;
};