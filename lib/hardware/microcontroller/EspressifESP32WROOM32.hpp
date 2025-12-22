#pragma once

class EspressifESP32WROOM32 {
    private:
        // deltatime variables
        float deltatime;
        unsigned long previousTimeMiliseconds = 0;
        // I2C protocol connection
        static const int BAUD_RATE = 9600;

    public:
        void begin(const bool iPauseExecution = true);
        void update();
        float getDeltaTime() const; // miliseconds
        float getTemperature() const; // °C
};