#include "EspressifESP32WROOM32.hpp"

#include <HardwareSerial.h>

void EspressifESP32WROOM32::begin(const bool pauseExecution, const int clockFrequency) {
    if(clockFrequency > 0 && clockFrequency <= 240) {setCpuFrequencyMhz(clockFrequency);}
	
    Serial.begin(BAUD_RATE);
	if(pauseExecution) {
		while(!Serial) {delay(1);} // will pause until serial console opens
	}
}

void EspressifESP32WROOM32::update() {
    const unsigned long int currentTimeMicros = micros();
    const unsigned long int deltaMicros = currentTimeMicros - previousTimeMicros;
    const float MICROSECONDS_TO_SECONDS = 1000000.0f;
    
    deltatime = (float)deltaMicros / MICROSECONDS_TO_SECONDS;
    previousTimeMicros = currentTimeMicros;
}

int EspressifESP32WROOM32::getClockFrequency() const {return getCpuFrequencyMhz();}
float EspressifESP32WROOM32::getDeltaTime() const {return deltatime;}
float EspressifESP32WROOM32::getTemperature() const {return temperatureRead();}