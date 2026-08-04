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
        int timestamp; // milliseconds since the program started

        const int MAX_NMEA_LENGTH = 82; // standard maximum NMEA message length
        const int NMEA_MESSAGES_HISTORY_MAX_SIZE = 5;
        static const int BAUD_RATE = 9600;
        
    public:
        UbloxNEO6M();
        void begin(const int RXpin, const int TXpin);
        void update();
        void printNMEAmessages();
        std::string getLastNMEAmessage() const;
};