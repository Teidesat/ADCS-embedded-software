#include "EspressifESP32WROOM32.hpp"

#include <HardwareSerial.h>

void EspressifESP32WROOM32::begin(const bool iPauseExecution) {
	Serial.begin(BAUD_RATE);
	if(iPauseExecution) {
		while(!Serial) {delay(1);} // will pause until serial console opens
	}
}

void EspressifESP32WROOM32::update() {
	unsigned long currentTimeMiliseconds = millis();
	unsigned long deltaTimeMiliseconds = currentTimeMiliseconds - previousTimeMiliseconds;
	float deltaTimeSeconds = (float)deltaTimeMiliseconds / 1000.0f;
	previousTimeMiliseconds = currentTimeMiliseconds;

	deltatime = deltaTimeSeconds;
}

float EspressifESP32WROOM32::getDeltaTime() const {return deltatime;}

float EspressifESP32WROOM32::getTemperature() const {return temperatureRead();}