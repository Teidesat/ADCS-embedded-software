#ifndef HARDWARE_SETUP
#define HARDWARE_SETUP

#include "../Microcontroller/ESP12.hpp"
#include "../IMU/LSM9DS1.hpp"
#include "../GNSS/NEO6M.hpp"

void setupHardware();

class INS {
    public:
        ESP12 microcontroller;
        LSM9DS1 imu;
        TinyGPSPlus gnss;

        void setup();
        void update();
};

#endif