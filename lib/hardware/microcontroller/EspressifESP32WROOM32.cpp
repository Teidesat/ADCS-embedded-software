#include "EspressifESP32WROOM32.hpp"

void EspressifESP32WROOM32::begin(const bool pauseExecution) {
	// Serial connection with PC
	Serial.begin(BAUD_RATE);
	if(pauseExecution) {
		while(!Serial) {delay(1);} // will pause until serial console opens
	}
}