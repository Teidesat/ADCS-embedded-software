#pragma once

class EspressifESP32WROOM32 {
    private:
        float deltatime;
        unsigned long int previousTimeMicros = 0;
        static const int baudRate = 9600;
        const float clockFrequency = 160;
        int timestamp;

    public:
        void begin(const int clockFrequency, const bool pauseExecution = false); // remove bool parameter?
        void update();
        
        int getClockFrequency() const;
        float getDeltaTime() const; // miliseconds
        float getTemperature() const; // celsius
};