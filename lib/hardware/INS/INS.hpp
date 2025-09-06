#pragma once

#include "../microcontroller/EspressifESP32WROOM32.hpp"
#include "../IMU/AdafruitLSM9DS1.hpp"
#include "../GNSS/UbloxNEO6M.hpp"

class INS {
    public:
        EspressifESP32WROOM32 microcontroller;
        AdafruitLSM9DS1 imu;
        UbloxNEO6M gnss;

        void begin();
        void update();
};