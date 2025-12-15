#include "EspressifESP32WROOM32.hpp"

void EspressifESP32WROOM32::begin(const bool pauseExecution) {
	// Serial connection with PC
	Serial.begin(BAUD_RATE);
	if(pauseExecution) {
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

float EspressifESP32WROOM32::getDeltaTime() {return deltatime;}