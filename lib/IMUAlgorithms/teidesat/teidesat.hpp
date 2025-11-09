#pragma once

#include "../../utils/rotationMath.hpp"

#include <string>

class Teidesat {
    public:
        EulerAngles attitude;
        void calculateOrientation(float accelerationX, float accelerationY, float accelerationZ,
                                  float angularVelocityX, float angularVelocityY, float angularVelocityZ,
                                  float magneticFieldX, float magneticFieldY, float magneticFieldZ,
                                  int SunSensorData = 0, int GPSPosition = 0, int temperature = 0);
        Teidesat(float accelerationXBias = 0, float accelerationYBias = 0, float accelerationZBias = 0,
                 float angularVelocityXBias = 0, float angularVelocityYBias = 0, float angularVelocityZBias = 0,
                 float magneticFieldXBias = 0, float magneticFieldYBias = 0, float magneticFieldZBias = 0,
                 int SunSensorDataBias = 0, int GPSPositionBias = 0, int temperatureBias = 0);
    private:
        std::string attitudeToString();
        std::string positionToString();
};