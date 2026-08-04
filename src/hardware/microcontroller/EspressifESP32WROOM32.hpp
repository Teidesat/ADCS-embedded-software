#pragma once

class EspressifESP32WROOM32 {
    private:
        float deltatime;
        unsigned long int previousTimeMicros = 0;
        static const int BAUD_RATE = 9600;
        int timestamp;

    public:
        void begin(const bool pauseExecution = true, const int clockFrequency = 160);
        void update();
        
        int getClockFrequency() const;
        float getDeltaTime() const; // miliseconds
        float getTemperature() const; // celsius
};