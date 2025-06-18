#include "ESP12.hpp"

ESP12::ESP12():
  serialPort(RX_PIN, TX_PIN)
{}

void ESP12::setup(const bool pauseExecution) {
  Serial.begin(DATA_RATE);
  if(pauseExecution) {
    while(!Serial) {delay(1);} // will pause Zero, Leonardo, etc until serial console opens
  }
}