#include "INS.hpp"

namespace ADCS {

void INS::begin(const int SDApin, 
                const int SCLpin, 
                const int RXpin, 
                const int TXpin, 
                const AdafruitLSM9DS1::AcclerometersRange accelerometersRange,
                const AdafruitLSM9DS1::GyroscopesRange gyroscopesRange,
                const AdafruitLSM9DS1::MagnetometersRange magnetometersRange,
                const float lowThreshold,
                const float highThreshold,
                const float interruptEnable,
                const bool pauseExecution,
                const int microcontrollerClockFrequency,
                const int I2CclockFrequency) {
    microcontroller.begin(true, microcontrollerClockFrequency);
    imu.begin(SDApin, 
              SCLpin, 
              AdafruitLSM9DS1::AcclerometersRange::G2, 
              AdafruitLSM9DS1::GyroscopesRange::DPS2000, 
              AdafruitLSM9DS1::MagnetometersRange::GAUSS4,
              I2CclockFrequency);
    gnss.begin(RXpin, TXpin);
    sunSensor.begin(SDApin, SCLpin, lowThreshold, highThreshold, interruptEnable, I2CclockFrequency);
}

void INS::update() {
    microcontroller.update();
    sunSensor.update();
    imu.update();
    //gnss.update();
}

Microcontroller INS::getMicrocontroller() const {return Microcontroller(microcontroller.getDeltaTime(), microcontroller.getTemperature());}
GNSS INS::getGNSS() const {return GNSS();}
IMU INS::getIMU() const {return IMU(imu.getAccelerometers(), imu.getGyroscopes(), imu.getMagnetometers(), imu.getTemperature());}
SunSensor INS::getSunSensor() const {return SunSensor(sunSensor.getALS(), sunSensor.getLux(), sunSensor.getWhite());}

}