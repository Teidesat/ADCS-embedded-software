#include "attitudeDetermination.hpp"

#include "../../lib/math/eulerAngles/eulerAngles.hpp"
#include "../../../lib/math/constants.hpp"

#include <cmath>

namespace ADCS {

math::Quaternion AttitudeAlgorithm::calculateAccelerationsAttitude(const math::Vector3d& linearAcclerations, const math::Vector3d& magneticFlux) const {
    const float roll  = std::atan2(linearAcclerations.y, linearAcclerations.z);
    
    const float pitchDenominator = std::sqrt(linearAcclerations.y * linearAcclerations.y + linearAcclerations.z * linearAcclerations.z);
    const float pitch = std::atan2(-linearAcclerations.x, pitchDenominator);

    const float pitchCosine = std::cos(pitch);
    const float pitchSine = std::sin(pitch);
    const float rollCosine = std::cos(roll);
    const float rollSine = std::sin(roll);
    const float magneticHorizontalX = magneticFlux.x * pitchCosine + magneticFlux.y * pitchSine * rollSine + magneticFlux.z * pitchSine * rollCosine;
    const float magneticHorizontalY = magneticFlux.y * rollCosine - magneticFlux.z * rollSine;
    const float yaw = std::atan2(-magneticHorizontalY, magneticHorizontalX);

    const math::EulerAngles newAttitude(roll, pitch, yaw);

    return newAttitude.toQuaternion().normalize();
}

math::Quaternion AttitudeAlgorithm::calculateAngularVelocitiesAttitude(const math::Vector3d& angularVelocities, const float deltaTime) const {
    const float magnitude = std::sqrt(angularVelocities.x * angularVelocities.x + 
                                      angularVelocities.y * angularVelocities.y + 
                                      angularVelocities.z * angularVelocities.z);

    if (magnitude <= math::Constants::doubleComparisonThreshold) {return attitudeQuaternion;}

    const float halfAngle = 0.5;
    const float halftheta = magnitude * deltaTime * halfAngle;
    const float sineHalfTheta = std::sin(halftheta);
    const float cosineHalfTheta = std::cos(halftheta);

    const math::Quaternion deltaQuaternion(
        cosineHalfTheta,
        (angularVelocities.x / magnitude) * sineHalfTheta,
        (angularVelocities.y / magnitude) * sineHalfTheta,
        (angularVelocities.z / magnitude) * sineHalfTheta
    );

    const math::Quaternion newAttitude = attitudeQuaternion * deltaQuaternion;

    return newAttitude.normalize();
}

float AttitudeAlgorithm::calculateTrustInAccelerometers(const float luminosity, const float maxLuminosity) const {
    float trustInAccelerometers = ((100.0 / maxLuminosity) * luminosity) / 100; // magic numbers?
    if(trustInAccelerometers > 1.0) {trustInAccelerometers = 1.0;}

    return trustInAccelerometers;
}

math::Quaternion AttitudeAlgorithm::blendQuaternionsWeighted(const math::Quaternion& gyroAttitude, const math::Quaternion& accelerometersAttitude, const float luminosity) {
    const float maxLuminosity = 100.0f;
    const float trustInAccelerometers = calculateTrustInAccelerometers(luminosity, maxLuminosity);
    return gyroAttitude.slerp(accelerometersAttitude, trustInAccelerometers);
}

void AttitudeAlgorithm::update(const math::Vector3d& linearAcclerations, 
                               const math::Vector3d& angularVelocities, 
                               const math::Vector3d& magneticFlux, 
                               const float luminosity, 
                               const float deltaTime) {
    const math::Quaternion angularVelocitiesAttitude = calculateAngularVelocitiesAttitude(angularVelocities, deltaTime);
    const math::Quaternion accelerationsAttitude = calculateAccelerationsAttitude(linearAcclerations, magneticFlux);

    attitudeQuaternion = blendQuaternionsWeighted(accelerationsAttitude, angularVelocitiesAttitude, luminosity);
}

math::Quaternion AttitudeAlgorithm::getAttitude() const {return attitudeQuaternion;}

}