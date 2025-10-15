#pragma once

#include "../microcontroller/EspressifESP32WROOM32.hpp"
#include "../sunSensor/AdafruitVEML7700.hpp"
#include "../IMU/AdafruitLSM9DS1.hpp"
#include "../GNSS/UbloxNEO6M.hpp"

class INS {
    public:
        EspressifESP32WROOM32 microcontroller;
        AdafruitVEML7700 sunSensor;
        AdafruitLSM9DS1 imu;
        UbloxNEO6M gnss;

        void begin();
        void update();
};