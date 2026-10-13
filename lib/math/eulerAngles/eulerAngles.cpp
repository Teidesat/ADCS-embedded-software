#include "eulerAngles.hpp"

#include "../quaternion/quaternion.hpp"

#include <cmath>

EulerAngles::EulerAngles(const float pitchInput, const float rollInput, const float yawInput): 
    pitch(pitchInput), 
    roll(rollInput), 
    yaw(yawInput) 
{}

float EulerAngles::getRoll() const {return roll;}  
float EulerAngles::getPitch() const {return pitch;}
float EulerAngles::getYaw() const {return yaw;}

float EulerAngles::radiansToDegrees(const float radians) {
    constexpr float radiansToDegreesConversionRate = 180.0 / pi;
    return radians * radiansToDegreesConversionRate;
}

EulerAngles EulerAngles::radiansToDegrees() const {
    const float rollInDegrees = radiansToDegrees(roll);
    const float pitchInDegrees = radiansToDegrees(pitch);
    const float yawIndegrees = radiansToDegrees(yaw);

    return EulerAngles(rollInDegrees, pitchInDegrees, yawIndegrees);
}

float EulerAngles::degreesToRadians(const float degrees) {
    constexpr float degreesToRadiansConversionRate = pi / 180.0;
    return degrees * degreesToRadiansConversionRate;
}

EulerAngles EulerAngles::degreesToRadians() const {
    const float rollInRadians = degreesToRadians(roll);
    const float pitchInRadians = degreesToRadians(pitch);
    const float yawInRadians = degreesToRadians(yaw);

    return EulerAngles(rollInRadians, pitchInRadians, yawInRadians);
}

Quaternion EulerAngles::toQuaternion() const {
    const float halfAngle = 0.5;
    const float rollHalfAngle = roll * halfAngle;
    const float pitchHalfAngle = pitch * halfAngle;
    const float yawHalfAngle = yaw * halfAngle;

    const float rollCosine = std::cos(rollHalfAngle);
    const float rollSine = std::sin(rollHalfAngle);
    const float pitchCosine = std::cos(pitchHalfAngle);
    const float pitchSine = std::sin(pitchHalfAngle);
    const float yawCosine = std::cos(yawHalfAngle);
    const float yawSine = std::sin(yawHalfAngle);

    // rotations applied in the order Yaw * Pitch * Roll
    const float w = rollCosine * pitchCosine * yawCosine + rollSine * pitchSine * yawSine;
    const float x = rollSine * pitchCosine * yawCosine - rollCosine * pitchSine * yawSine;
    const float y = rollCosine * pitchSine * yawCosine + rollSine * pitchCosine * yawSine;
    const float z = rollCosine * pitchCosine * yawSine - rollSine * pitchSine * yawCosine;

    return Quaternion(w, x, y, z);
}