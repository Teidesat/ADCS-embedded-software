#pragma once

#include <HardwareSerial.h>

class EspressifESP32WROOM32 {
    private:
        // I2C protocol pins
        const int BAUD_RATE = 9600;

    public:
        void begin(const bool pauseExecution = true);
};