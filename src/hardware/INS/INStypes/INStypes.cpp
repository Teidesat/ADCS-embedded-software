#pragma once

#include "INStypes.hpp"

namespace ADCS {

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

}