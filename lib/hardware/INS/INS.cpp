#include "hardwareSetup.hpp"

void INS::setup() {
    microcontroller.setup();
    imu.setup();
}

void INS::update() {
    imu.update();
}