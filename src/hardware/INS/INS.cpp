#include "INS.hpp"

Microcontroller::Microcontroller(const float deltaTime, const float temperature): 
    clockFrequency(clockFrequency),
    deltaTime(deltaTime),
    temperature(temperature)
{}
float Microcontroller::getDeltaTime() const {return deltaTime;}
float Microcontroller::getTemperature() const {return temperature;}

SunSensor::SunSensor(float ALS, float lux, float white): 
    ALS(ALS), 
    lux(lux), 
    white(white) 
{}
float SunSensor::getALS() const {return ALS;}
float SunSensor::getLux() const {return lux;}
float SunSensor::getWhite() const {return white;}

IMU::IMU(const Vector3d accelerometers, const Vector3d gyroscopes, const Vector3d magnetometers, const float temperature):
    accelerometers(accelerometers),
    gyroscopes(gyroscopes),
    magnetometers(magnetometers),
    temperature(temperature) {}

Vector3d IMU::getAccelerometers() const {return accelerometers;}
Vector3d IMU::getGyroscopes() const {return gyroscopes;}
Vector3d IMU::getMagnetometers() const {return magnetometers;}
float IMU::getTemperature() const {return temperature;}

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