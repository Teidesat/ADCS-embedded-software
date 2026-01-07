#include "attitudeDetermination.hpp"

#include <cmath>
#include <limits>

Quaternion AttitudeAlgorithm::calculateAccelerationsAttitude(const Vector3d& linearAcclerations, const Vector3d& magneticFlux) const {
    const float roll  = std::atan2(linearAcclerations.getY(), linearAcclerations.getZ());
    
    const float pitchDenominator = std::sqrt(linearAcclerations.getY() * linearAcclerations.getY() + linearAcclerations.getZ() * linearAcclerations.getZ());
    const float pitch = std::atan2(-linearAcclerations.getX(), pitchDenominator);

    const float pitchCosine = std::cos(pitch);
    const float pitchSine = std::sin(pitch);
    const float rollCosine = std::cos(roll);
    const float rollSine = std::sin(roll);
    const float magneticHorizontalX = magneticFlux.getX() * pitchCosine + magneticFlux.getY() * pitchSine * rollSine + magneticFlux.getZ() * pitchSine * rollCosine;
    const float magneticHorizontalY = magneticFlux.getY() * rollCosine - magneticFlux.getZ() * rollSine;
    const float yaw = std::atan2(-magneticHorizontalY, magneticHorizontalX);

    const EulerAngles newAttitude(roll, pitch, yaw);

    return newAttitude.toQuaternion().normalize();
}

Quaternion AttitudeAlgorithm::calculateAngularVelocitiesAttitude(const Vector3d& angularVelocities, const float deltaTime) const {
    const float magnitude = std::sqrt(angularVelocities.getX() * angularVelocities.getX() + 
                                      angularVelocities.getY() * angularVelocities.getY() + 
                                      angularVelocities.getZ() * angularVelocities.getZ());

    if (magnitude <= std::numeric_limits<float>::epsilon()) {return attitudeQuaternion;}

    const float halfAngle = 0.5f;
    const float halftheta = magnitude * deltaTime * halfAngle;
    const float sineHalfTheta = std::sin(halftheta);
    const float cosineHalfTheta = std::cos(halftheta);

    const Quaternion deltaQuaternion(
        cosineHalfTheta,
        (angularVelocities.getX() / magnitude) * sineHalfTheta,
        (angularVelocities.getY() / magnitude) * sineHalfTheta,
        (angularVelocities.getZ() / magnitude) * sineHalfTheta
    );

    const Quaternion newAttitude = attitudeQuaternion * deltaQuaternion;

    return newAttitude.normalize();
}

float AttitudeAlgorithm::calculateTrustInAccelerometers(const float luminosity, const float maxLuminosity) const {
    float trustInAccelerometers = ((100.0f / maxLuminosity) * luminosity) / 100;
    if(trustInAccelerometers > 1.0f) {trustInAccelerometers = 1.0f;}

    return trustInAccelerometers;
}

Quaternion AttitudeAlgorithm::blendQuaternionsWeighted(const Quaternion& gyroAttitude, const Quaternion& accelerometersAttitude, const float luminosity) {
    const float maxLuminosity = 100.0f;
    const float trustInAccelerometers = calculateTrustInAccelerometers(luminosity, maxLuminosity);
    const float trustInGyroscopes = 1 - trustInAccelerometers;
    return gyroAttitude.slerp(accelerometersAttitude, trustInAccelerometers); // should use this trust in gyroscopes instead of trust in accelerometers
}

void AttitudeAlgorithm::update(const Vector3d& linearAcclerations, 
                               const Vector3d& angularVelocities, 
                               const Vector3d& magneticFlux, 
                               const float luminosity, 
                               const float deltaTime) {
    const Quaternion angularVelocitiesAttitude = calculateAngularVelocitiesAttitude(angularVelocities, deltaTime);
    const Quaternion accelerationsAttitude = calculateAccelerationsAttitude(linearAcclerations, magneticFlux);

    attitudeQuaternion = blendQuaternionsWeighted(accelerationsAttitude, angularVelocitiesAttitude, luminosity);
}

Quaternion AttitudeAlgorithm::getAttitude() const {return attitudeQuaternion;}