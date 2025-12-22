#include "INS.hpp"

Microcontroller::Microcontroller(float iDeltatime, float iTemperature): 
    deltaTime(iDeltatime), 
    temperature(iTemperature) 
{}
float Microcontroller::getDeltaTime() const {return deltaTime;}
float Microcontroller::getTemperature() const {return temperature;}

SunSensor::SunSensor(float iALS, float iLux, float iWhite): 
    ALS(iALS), 
    lux(iLux), 
    white(iWhite) 
{}
float SunSensor::getALS() const {return ALS;}
float SunSensor::getLux() const {return lux;}
float SunSensor::getWhite() const {return white;}

IMU::IMU(Vector3d iAccelerometers, Vector3d iGyroscopes, Vector3d iMagnetometers, float iTemperature) :
    accelerometers(iAccelerometers),
    gyroscopes(iGyroscopes),
    magnetometers(iMagnetometers),
    temperature(iTemperature)
{}
Vector3d IMU::getAccelerometers() const {return accelerometers;}
Vector3d IMU::getGyroscopes() const {return gyroscopes;}
Vector3d IMU::getMagnetometers() const {return magnetometers;}
float IMU::getTemperature() const {return temperature;}

void INS::begin(const int iSDApin, const int iSCLpin, const int iRXpin, const int iTXpin) {
    microcontroller.begin();
    sunSensor.begin(iSDApin, iSCLpin);
    imu.begin(iSDApin, iSCLpin);
    gnss.begin(iRXpin, iTXpin);
}

void INS::update() {
    microcontroller.update();
    sunSensor.update();
    imu.update();
    //gnss.update();
}

Microcontroller INS::getMicrocontrollerData() const {return Microcontroller(microcontroller.getDeltaTime(), microcontroller.getTemperature());}
GNSS INS::getGNSS() const {return GNSS();}
IMU INS::getIMU() const {return IMU(imu.getAccelerometers(), imu.getGyroscopes(), imu.getMagnetometers(), imu.getTemperature());}
SunSensor INS::getSunSensor() const {return SunSensor(sunSensor.getALS(), sunSensor.getLux(), sunSensor.getWhite());}