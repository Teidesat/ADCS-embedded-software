#ifndef ESP12_H
#define ESP12_H  

#include <Arduino.h>
#include <SoftwareSerial.h>

class ESP12 {
    private:
        const int DATA_RATE = 9600;
        const int RX_PIN = 2;
        const int TX_PIN = 1;
        EspSoftwareSerial::UART serialPort;

    public:
        ESP12();
        void setup(const bool pauseExecution = true);
};

#endif