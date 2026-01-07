#pragma once

class EspressifESP32WROOM32 {
    private:
        // deltatime variables
        float deltatime;
        //unsigned long int previousTimeMiliseconds = 0;
        unsigned long int previousTimeMicros = 0;
        // I2C protocol connection
        static const int BAUD_RATE = 9600;

    public:
        void begin(const bool pauseExecution = true, const int clockFrequency = 160);
        void update();
        
        int getClockFrequency() const;
        float getDeltaTime() const; // miliseconds
        float getTemperature() const; // celsius
};