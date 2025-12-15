#pragma once

#include "../../utils/rotationMath.hpp"

#include  <ArduinoEigenDense.h>

#include <string>

class Teidesat {
    private:
        Eigen::Quaternionf attitudeQuaternion;

        float calculateGyroscopesTrust(const float& luminosity);
        Eigen::Quaternionf calculateAcclelerometersAttitude(const SensorData& linearAcclerations);
        Eigen::Quaternionf calculateGyroscopesAttitude(const SensorData& angularVelocities, const float& deltaTime);
        Eigen::Quaternionf blendQuaternionsWeighted(const Eigen::Quaternionf& Q_Primary, const Eigen::Quaternionf& Q_Absolute, const float& luminosity);

    public: 
        void update(const SensorData& accelerations, const SensorData& angularVelocities, const float& luminosity, const float& deltaTime);
        EulerAngles getAttitude();
        std::string attitudeToString();
};