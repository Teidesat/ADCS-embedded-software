#include "eulerAngles.hpp"

#include "../quaternion/quaternion.hpp"

#include <cmath>



EulerAngles::EulerAngles(const float iRoll, const float iPitch, const float iYaw): 
    roll(iRoll), 
    pitch(iPitch), 
    yaw(iYaw) 
{}

float EulerAngles::getRoll() const {return roll;}  
float EulerAngles::getPitch() const {return pitch;}
float EulerAngles::getYaw() const {return yaw;}

float EulerAngles::toDegrees(float component) {
    constexpr float RADIANS_TO_DEGREES = 180.0f / M_PI;
    return component * RADIANS_TO_DEGREES;
}

EulerAngles EulerAngles::toDegrees(EulerAngles attitude) {
    const float roll = toDegrees(attitude.getRoll());
    const float pitch = toDegrees(attitude.getPitch());
    const float yaw = toDegrees(attitude.getYaw());

    return EulerAngles(roll, pitch, yaw);
}

float EulerAngles::toRadians(float component) {
    constexpr float DEGREES_TO_RADIANS = M_PI / 180.0f;
    return component * DEGREES_TO_RADIANS;
}

EulerAngles EulerAngles::toRadians(EulerAngles attitude) {
    const float roll = toRadians(attitude.getRoll());
    const float pitch = toRadians(attitude.getPitch());
    const float yaw = toRadians(attitude.getYaw());

    return EulerAngles(roll, pitch, yaw);
}

Quaternion EulerAngles::toQuaternion() const {
    const float rollHalfAngle = roll * 0.5;
    const float pitchHalfAngle = pitch * 0.5;
    const float yawHalfAngle = yaw * 0.5;

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

std::string EulerAngles::shortInfo() const {
    std::string attitude = "   Roll: " + std::to_string(roll) + "   Pitch: " + std::to_string(pitch) + "   Yaw: " + std::to_string(yaw);
    return attitude;
}