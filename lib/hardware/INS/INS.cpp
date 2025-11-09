#include "INS.hpp"

void INS::begin() {
    // I2C (IMU and sun sensor)
    const int SDA_PIN = 26;
    const int SCL_PIN = 27;
    // Serial2 UART (GNSS)
    const int RX_PIN = 16;
    const int TX_PIN = 17;

    microcontroller.begin();
    sunSensor.begin(SDA_PIN, SCL_PIN);
    imu.begin(SDA_PIN, SCL_PIN);
    gnss.begin(RX_PIN, TX_PIN);
}

void INS::update() {
    sunSensor.update();
    imu.update();
    gnss.update();
}

std::string INS::allSensorsToString() {
    return "\nSun Sensor:\n" + sunSensor.sensorDataToString() + "\nIMU:\n" + imu.allSensorsToString() + "\nGNSS:\n" + gnss.getLastNMEAmessage();
}