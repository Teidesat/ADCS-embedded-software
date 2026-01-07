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
        static const int MAX_NMEA_LENGTH = 82; // standard maximum NMEA message length
        const int NMEA_MESSAGES_HISTORY_MAX_SIZE = 5;
        
    public:
        UbloxNEO6M();
        void begin(const int RXpin, const int TXpin);
        void update();
        void printNMEAmessages();
        std::string getLastNMEAmessage() const;
};