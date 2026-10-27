#include "EspressifESP32WROOM32.hpp"

#include <HardwareSerial.h>

void EspressifESP32WROOM32::begin(const int clockFrequency, const bool pauseExecution) {
    if(clockFrequency > 0 && clockFrequency <= 240) {setCpuFrequencyMhz(clockFrequency);}
	
    Serial.begin(baudRate); // remove code from production or put in main?
	if(pauseExecution) {
		while(!Serial) {delay(1);} // will pause until serial console opens
	}
}

void EspressifESP32WROOM32::update() {
    const unsigned long int currentTimeMicros = micros();
    const unsigned long int deltaMicros = currentTimeMicros - previousTimeMicros;
    const float microsecondsToSecondsConversionRate = 1000000.0;
    
    deltatime = (float)deltaMicros / microsecondsToSecondsConversionRate;
    previousTimeMicros = currentTimeMicros;
}

int EspressifESP32WROOM32::getClockFrequency() const {return getCpuFrequencyMhz();}
float EspressifESP32WROOM32::getDeltaTime() const {return deltatime;}
float EspressifESP32WROOM32::getTemperature() const {return temperatureRead();}