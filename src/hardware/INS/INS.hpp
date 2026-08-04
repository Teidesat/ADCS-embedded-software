#pragma once

#include "INStypes/INStypes.hpp"
#include "../microcontroller/EspressifESP32WROOM32.hpp"
#include "../GNSS/UbloxNEO6M.hpp"
#include "../sunSensor/AdafruitVEML7700.hpp"
#include "../IMU/AdafruitLSM9DS1.hpp"

namespace ADCS {

class INS {
    private:
        EspressifESP32WROOM32 microcontroller;
        UbloxNEO6M gnss;
        AdafruitVEML7700 sunSensor;
        AdafruitLSM9DS1 imu;

    public:
        void begin(const int SDApin, 
                   const int SCLpin, 
                   const int RXpin, 
                   const int TXpin, 
                   const AdafruitLSM9DS1::AcclerometersRange accelerometersRange = AdafruitLSM9DS1::AcclerometersRange::G2,
                   const AdafruitLSM9DS1::GyroscopesRange gyroscopesRange = AdafruitLSM9DS1::GyroscopesRange::DPS245,
                   const AdafruitLSM9DS1::MagnetometersRange magnetometersRange = AdafruitLSM9DS1::MagnetometersRange::GAUSS4,
                   const float lowThreshold = 10000.0f,
                   const float highThreshold = 20000.0f,
                   const float interruptEnable = true,
                   const bool pauseExecution = true, 
                   const int microcontrollerClockFrequency = 0,
                   const int I2CclockFrequency = 0);
        void update();

        Microcontroller getMicrocontroller() const;
        GNSS getGNSS() const;
        IMU getIMU() const;
        SunSensor getSunSensor() const;
};

}