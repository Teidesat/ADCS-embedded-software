#include "eulerAngles.hpp"

#include "../quaternion/quaternion.hpp"

#include <cmath>



EulerAngles::EulerAngles(const float newRoll, const float newPitch, const float newYaw): 
    roll(newRoll), 
    pitch(newPitch), 
    yaw(newYaw) 
{}

float EulerAngles::getRoll() const {return roll;}  
float EulerAngles::getPitch() const {return pitch;}
float EulerAngles::getYaw() const {return yaw;}

float EulerAngles::radiansToDegrees(float radians) {
    constexpr float radiansToDegreesConversionRate = 180.0 / M_PI;
    return radians * radiansToDegreesConversionRate;}

EulerAngles EulerAngles::radiansToDegrees(EulerAngles attitude) {
    const float newRoll = radiansToDegrees(attitude.getRoll());
    const float newPitch = radiansToDegrees(attitude.getPitch());
    const float newYaw = radiansToDegrees(attitude.getYaw());

    return EulerAngles(newRoll, newPitch, newYaw);
}

float EulerAngles::degreesToRadians(float degrees) {
    constexpr float degreesToRadiansConversionRate = M_PI / 180.0;
    return degrees * degreesToRadiansConversionRate;
}

EulerAngles EulerAngles::degreesToRadians(EulerAngles attitude) {
    const float newRoll = degreesToRadians(attitude.getRoll());
    const float newPitch = degreesToRadians(attitude.getPitch());
    const float newYaw = degreesToRadians(attitude.getYaw());

    return EulerAngles(newRoll, newPitch, newYaw);
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