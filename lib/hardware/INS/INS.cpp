#include "INS.hpp"

void INS::begin() {
    // I2C
    const int SDA_PIN = 26;
    const int SCL_PIN = 27;
    // Serial2 UART
    const int RX_PIN = 16;
    const int TX_PIN = 17;

    microcontroller.begin();
    imu.begin(SDA_PIN, SCL_PIN);
    gnss.begin(RX_PIN, TX_PIN);
}

void INS::update() {
    imu.update();
    gnss.update();
}