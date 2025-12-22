#include "eulerAngles.hpp"

#include <math.h>

EulerAngles::EulerAngles(const float iPitch, const float iRoll, const float iYaw): 
    pitch(iPitch), 
    roll(iRoll), 
    yaw(iYaw) 
{}

float EulerAngles::getPitch() const {return pitch;}
float EulerAngles::getRoll() const {return roll;}  
float EulerAngles::getYaw() const {return yaw;}

Quaternion EulerAngles::toQuaternionTrigonometric() const {
    const float cosineOfPitch = std::cos(pitch * 0.5f); 
    const float sineOfPitch = std::sin(pitch * 0.5f);
    const float cosineOfRoll = std::cos(roll * 0.5f); 
    const float sineOfRoll = std::sin(roll * 0.5f);
    const float cosineOfYaw = std::cos(yaw * 0.5f); 
    const float sineOfYaw = std::sin(yaw * 0.5f);

    // Formula for YXZ Sequence
    const float newX = cosineOfPitch * sineOfRoll * cosineOfYaw - sineOfPitch * cosineOfRoll * sineOfYaw;
    const float newY = sineOfPitch * cosineOfRoll * cosineOfYaw + cosineOfPitch * sineOfRoll * sineOfYaw;
    const float newZ = cosineOfPitch * cosineOfRoll * sineOfYaw - sineOfPitch * sineOfRoll * cosineOfYaw;
    const float newW = cosineOfPitch * cosineOfRoll * cosineOfYaw + sineOfPitch * sineOfRoll * sineOfYaw;

    return Quaternion(newX, newY, newZ, newW);
}

Quaternion EulerAngles::toQuaternionHamilton() const {
    // Create axis-specific quaternions
    Quaternion quaternionPitch(std::sin(pitch), 0, 0, std::cos(pitch)); // Pitch (X-axis)
    Quaternion quaternionRoll(0, std::sin(roll), 0, std::cos(roll)); // Roll (Y-axis)
    Quaternion quaternionYaw(0, 0, std::sin(yaw), std::cos(yaw)); // Yaw (Z-axis)

    // order of the multiplication matters
    Quaternion quaternion = quaternionPitch * (quaternionRoll * quaternionYaw);

    return quaternion;
}

Quaternion EulerAngles::toQuaternionMatrix() const {
    const float cosinePitch = std::cos(pitch); 
    const float sinePitch = std::sin(pitch);
    const float cosineRoll = std::cos(roll); 
    const float sineRoll = std::sin(roll);
    const float cosineYaw = std::cos(yaw); 
    const float sineYaw = std::sin(yaw);

    // Build the Rotation Matrix (M) for YXZ sequence
    const float matrix00 = cosineYaw*cosineRoll + sineYaw*sinePitch*sineRoll;
    const float matrix01 = sineYaw*cosinePitch;
    const float matrix02 = cosineYaw*(-sineRoll) + sineYaw*sinePitch*cosineRoll;
    const float matrix10 = (-sineYaw)*cosineRoll + cosineYaw*sinePitch*sineRoll;
    const float matrix11 = cosineYaw*cosinePitch;
    const float matrix12 = sineYaw*sineRoll + cosineYaw*sinePitch*cosineRoll;
    const float matrix20 = cosinePitch*sineRoll;
    const float matrix21 = -sinePitch;
    const float matrix22 = cosinePitch*cosineRoll;
    const float matrix02 = cosineYaw*(-sineRoll) + sineYaw*sinePitch*cosineRoll;
    const float matrix10 = (-sineYaw)*cosineRoll + cosineYaw*sinePitch*sineRoll;
    const float matrix11 = cosineYaw*cosinePitch;
    const float matrix12 = sineYaw*sineRoll + cosineYaw*sinePitch*cosineRoll;
    const float matrix20 = cosinePitch*sineRoll;
    const float matrix21 = -sinePitch;
    const float matrix22 = cosinePitch*cosineRoll;

    // Extract Quaternion from Matrix
    Quaternion q;
    float newX = std::sqrt(std::fmax(0, 1 + matrix00 - matrix11 - matrix22)) / 2.0f;
    float newY = std::sqrt(std::fmax(0, 1 - matrix00 + matrix11 - matrix22)) / 2.0f;
    float newZ = std::sqrt(std::fmax(0, 1 - matrix00 - matrix11 + matrix22)) / 2.0f;
    const float newW = std::sqrt(std::fmax(0, 1 + matrix00 + matrix11 + matrix22)) / 2.0f;
    
    // Restore signs based on matrix differences
    newX = (matrix21 - matrix12) < 0 ? -newX : newX;
    newY = (matrix02 - matrix20) < 0 ? -newY : newY;
    newZ = (matrix10 - matrix01) < 0 ? -newZ : newZ;

    return Quaternion(newX, newY, newZ, newW);
}

Quaternion EulerAngles::toQuaternion() const {return toQuaternionHamilton();}

EulerAngles EulerAngles::radiansToDegrees() const {
    constexpr float RADIANS_TO_DEGREES = 180.0f / M_PI;
    const float pitchInDegrees = pitch * RADIANS_TO_DEGREES;
    const float rollInDegrees = roll * RADIANS_TO_DEGREES;
    const float yawInDegrees = yaw * RADIANS_TO_DEGREES;
    
    return EulerAngles(pitchInDegrees, rollInDegrees, yawInDegrees);
}