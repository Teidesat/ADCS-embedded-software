#include "INStypes.hpp"

namespace ADCS {

Microcontroller::Microcontroller(const float deltaTime, const float temperature):
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

IMU::IMU(const math::Vector3d accelerometers, const math::Vector3d gyroscopes, const math::Vector3d magnetometers, const float temperature):
    accelerometers(accelerometers),
    gyroscopes(gyroscopes),
    magnetometers(magnetometers),
    temperature(temperature) {}

math::Vector3d IMU::getAccelerometers() const {return accelerometers;}
math::Vector3d IMU::getGyroscopes() const {return gyroscopes;}
math::Vector3d IMU::getMagnetometers() const {return magnetometers;}
float IMU::getTemperature() const {return temperature;}

GNSS::GNSS(std::string& message) { //use std::string_view?
    parseMessage(message);
}

void GNSS::parseMessage(const std::string& message) {
    // split mesage by the commas?
    // set only neccesary variables for now of altitude, latitude, longitude and date?
    //float altitudeDegrees = 0.0;
    //float altitudeMinutes = 0.0;
    //float latitudeDegrees = 0.0;
    //float latitudeMinutes = 0.0;
    //float longitudeDegrees = 0.0;
    //float longitudeMinutes = 0.0;
}

}