// hardware without abstractions

#include "../lib/hardware/INS/INS.hpp"

INS ins;

void setup() {
  // I2C (IMU and sun sensor)
  //const int SDA_PIN = 26;
  //const int SCL_PIN = 27;
  // Serial2 UART (GNSS)
  const int RX_PIN = 16;
  const int TX_PIN = 17;

  ins.microcontroller.begin();
  //ins.imu.begin(SDA_PIN, SCL_PIN);
  ins.gnss.begin(RX_PIN, TX_PIN);

  //ins.gnss.shortInfo();
}

void loop() {
  //ins.imu.update();
  ins.gnss.update();
}