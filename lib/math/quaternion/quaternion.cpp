#include "quaternion.hpp"

#include <cmath>
#include "../constants.hpp"

namespace math {

Quaternion::Quaternion(const double wValue, const double xValue, const double yValue, const double zValue): 
    w(wValue),
    x(xValue),
    y(yValue),
    z(zValue)
{}

Quaternion Quaternion::conjugate() const {return Quaternion(w, -x, -y, -z);}

Quaternion Quaternion::normalize() const {
    double magnitude = std::sqrt(w * w + x * x + y * y + z * z);
    double normalizedW = w; 
    double normalizedX = x; 
    double normalizedY = y; 
    double normalizedZ = z;

    if (magnitude > 0.0) {
        normalizedW /= magnitude;
        normalizedX /= magnitude; 
        normalizedY /= magnitude; 
        normalizedZ /= magnitude;
    }

    return Quaternion(normalizedW, normalizedX, normalizedY, normalizedZ);
}

double Quaternion::dotProduct(const Quaternion& quaternion) const {
    return w * quaternion.w + x * quaternion.x + y * quaternion.y + z * quaternion.z;
}

bool Quaternion::equal(const Quaternion& quaternion) const {
    const double dotProductValue = dotProduct(quaternion);
    bool isEqual = false;
    if(1.0f - std::abs(dotProductValue) <= Constants::doubleComparisonThreshold) {isEqual = true;}

    return isEqual;
}

bool Quaternion::operator==(const Quaternion& quaternion) const {return equal(quaternion);}
bool Quaternion::operator!=(const Quaternion& quaternion) const {return !equal(quaternion);}

Quaternion Quaternion::slerp(const Quaternion& quaternion, const double time) const {
    double dotProductValue = dotProduct(quaternion);
    Quaternion shortestPathQuaternion = quaternion;

    if (dotProductValue < 0.0) {
        dotProductValue = -dotProductValue;
        shortestPathQuaternion = Quaternion(-quaternion.w, -quaternion.x, -quaternion.y, -quaternion.z);
    }

    const double SLERP_THRESHOLD = 0.9995; // good value for comparison?
    double newW, newX, newY, newZ;

    if (dotProductValue > SLERP_THRESHOLD) {
        const double inverseTime = 1.0 - time;

        newW = (inverseTime * w) + (time * shortestPathQuaternion.w);
        newX = (inverseTime * x) + (time * shortestPathQuaternion.x);
        newY = (inverseTime * y) + (time * shortestPathQuaternion.y);
        newZ = (inverseTime * z) + (time * shortestPathQuaternion.z);
    }
    else {
        const double totalAngle = std::acos(dotProductValue);
        const double interpolatedAngle = totalAngle * time;
        
        const double sineTotalAngle = std::sin(totalAngle);
        const double sineInterpolatedAngle = std::sin(interpolatedAngle);

        const double startWeight = std::cos(interpolatedAngle) - dotProductValue * sineInterpolatedAngle / sineTotalAngle;
        const double endWeight = sineInterpolatedAngle / sineTotalAngle;

        newW = (startWeight * w) + (endWeight * shortestPathQuaternion.w);
        newX = (startWeight * x) + (endWeight * shortestPathQuaternion.x);
        newY = (startWeight * y) + (endWeight * shortestPathQuaternion.y);
        newZ = (startWeight * z) + (endWeight * shortestPathQuaternion.z);
    }

    return Quaternion(newW, newX, newY, newZ).normalize();
}

Quaternion Quaternion::sum(const Quaternion& quaternion) const {
    return Quaternion(w + quaternion.w, x + quaternion.x, y + quaternion.y, z + quaternion.z);
}

Quaternion Quaternion::operator+(const Quaternion& quaternion) const {return sum(quaternion);}

Quaternion Quaternion::multiply(const Quaternion& quaternion) const {
    const double newW = w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z;
    const double newX = w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y;
    const double newY = w * quaternion.y - x * quaternion.z + y * quaternion.w + z * quaternion.x;
    const double newZ = w * quaternion.z + x * quaternion.y - y * quaternion.x + z * quaternion.w;
    
    return Quaternion(newW, newX, newY, newZ);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const {return multiply(quaternion);}

Quaternion Quaternion::multiply(const double scalar) const {return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);}

Quaternion Quaternion::operator*(const double scalar) const {return multiply(scalar);}

EulerAngles Quaternion::toEulerAngles() const { // keep function in the cubesat?
    double roll = 0.0; // ROLL (X-axis): Standard display is -180 to 180
    double pitch = 0.0; // Check for Gimbal Lock (Pitch at +/- 90 degrees)
    double yaw = 0.0; // YAW (Z-axis): Calculate initially as -180 to 180

    const double pitchSine = 2.0 * (w * y - z * x);
    constexpr double gimbalLockThreshold = 1.0 - Constants::doubleComparisonThreshold;

    if (std::fabs(pitchSine) >= gimbalLockThreshold) {
        pitch = std::copysign(Constants::pi / 2.0, pitchSine);
        roll = std::atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y));
    } 
    else {
        pitch = std::asin(pitchSine);

        const double rollNumerator = 2.0 * (w * x + y * z);
        const double rollDenominator = 1.0 - 2.0 * (x * x + y * y);
        roll = std::atan2(rollNumerator, rollDenominator);

        const double yawNumerator = 2.0 * (w * z + x * y);
        const double yawDenumerator = 1.0 - 2.0 * (y * y + z * z);
        yaw = std::atan2(yawNumerator, yawDenumerator);
    }

    if (yaw < 0) {yaw += 2.0 * Constants::pi;} // Wrap Yaw for display (0 to 360 degrees)

    return EulerAngles(roll, pitch, yaw);
}

}