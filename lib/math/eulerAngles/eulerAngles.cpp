#include "eulerAngles.hpp"

#include "../quaternion/quaternion.hpp"

#include <cmath>

namespace math {

EulerAngles::EulerAngles(const double rollValue, const double pitchValue, const double yawValue): 
    roll(rollValue), 
    pitch(pitchValue), 
    yaw(yawValue) 
{}

double EulerAngles::radiansToDegrees(const double radians) {
    constexpr double radiansToDegreesConversionRate = 180.0 / pi;
    return radians * radiansToDegreesConversionRate;
}

double EulerAngles::degreesToRadians(const double degrees) {
    constexpr double degreesToRadiansConversionRate = pi / 180.0;
    return degrees * degreesToRadiansConversionRate;
}

Quaternion EulerAngles::toQuaternion() const {
    constexpr double halfAngle = 0.5;
    const double rollHalfAngle = roll * halfAngle;
    const double pitchHalfAngle = pitch * halfAngle;
    const double yawHalfAngle = yaw * halfAngle;

    const double rollCosine = std::cos(rollHalfAngle);
    const double rollSine = std::sin(rollHalfAngle);
    const double pitchCosine = std::cos(pitchHalfAngle);
    const double pitchSine = std::sin(pitchHalfAngle);
    const double yawCosine = std::cos(yawHalfAngle);
    const double yawSine = std::sin(yawHalfAngle);

    // rotations applied in the order Yaw * Pitch * Roll
    const double w = rollCosine * pitchCosine * yawCosine + rollSine * pitchSine * yawSine;
    const double x = rollSine * pitchCosine * yawCosine - rollCosine * pitchSine * yawSine;
    const double y = rollCosine * pitchSine * yawCosine + rollSine * pitchCosine * yawSine;
    const double z = rollCosine * pitchCosine * yawSine - rollSine * pitchSine * yawCosine;

    return Quaternion(w, x, y, z);
}

}