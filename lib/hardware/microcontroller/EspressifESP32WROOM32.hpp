#pragma once

#include <HardwareSerial.h>

class EspressifESP32WROOM32 {
    private:
        // I2C protocol connection
        const int BAUD_RATE = 9600;
        // deltatime variables
        unsigned long previousTimeMiliseconds = 0;
        float deltatime;

    public:
        void begin(const bool pauseExecution = true);
        void update();
        float getDeltaTime();
};