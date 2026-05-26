#include "quaternion.hpp"

#include <cmath>
#include <limits>

Quaternion::Quaternion(const float w, const float x, const float y, const float z): 
    w(w),
    x(x),
    y(y),
    z(z)
{}

Quaternion Quaternion::conjugate() const {return Quaternion(w, -x, -y, -z);}

Quaternion Quaternion::normalize() const {
    float magnitude = std::sqrt(w * w + x * x + y * y + z * z);
    float normalizedW = w; 
    float normalizedX = x; 
    float normalizedY = y; 
    float normalizedZ = z;

    if (magnitude > 0.0) {
        normalizedW /= magnitude;
        normalizedX /= magnitude; 
        normalizedY /= magnitude; 
        normalizedZ /= magnitude;
    }

    return Quaternion(normalizedW, normalizedX, normalizedY, normalizedZ);
}

float Quaternion::dotProduct(const Quaternion& quaternion) const {
    return w * quaternion.w + x * quaternion.x + y * quaternion.y + z * quaternion.z;
}

bool Quaternion::equal(const Quaternion& quaternion) const {
    const float dotProductValue = dotProduct(quaternion);
    if(1.0f - std::abs(dotProductValue) <= std::numeric_limits<float>::epsilon()) {return true;}

    return false;
}

bool Quaternion::operator==(const Quaternion& quaternion) const {return equal(quaternion);}
bool Quaternion::operator!=(const Quaternion& quaternion) const {return !equal(quaternion);}

Quaternion Quaternion::slerp(const Quaternion& quaternion, const float time) const {
    float dotProductValue = dotProduct(quaternion);
    Quaternion shortestPathQuaternion = quaternion;

    if (dotProductValue < 0.0) {
        dotProductValue = -dotProductValue;
        shortestPathQuaternion = Quaternion(-quaternion.w, -quaternion.x, -quaternion.y, -quaternion.z);
    }

    const float SLERP_THRESHOLD = 0.9995;
    float newW, newX, newY, newZ;

    if (dotProductValue > SLERP_THRESHOLD) {
        const float inverseTime = 1.0 - time;

        newW = (inverseTime * w) + (time * shortestPathQuaternion.w);
        newX = (inverseTime * x) + (time * shortestPathQuaternion.z);
        newY = (inverseTime * y) + (time * shortestPathQuaternion.y);
        newZ = (inverseTime * z) + (time * shortestPathQuaternion.z);
    }
    else {
        const float totalAngle = std::acos(dotProductValue);
        const float interpolatedAngle = totalAngle * time;
        
        const float sineTotalAngle = std::sin(totalAngle);
        const float sineInterpolatedAngle = std::sin(interpolatedAngle);

        const float startWeight = std::cos(interpolatedAngle) - dotProductValue * sineInterpolatedAngle / sineTotalAngle;
        const float endWeight = sineInterpolatedAngle / sineTotalAngle;

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
    const float newW = w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z;
    const float newX = w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y;
    const float newY = w * quaternion.y - x * quaternion.z + y * quaternion.w + z * quaternion.x;
    const float newZ = w * quaternion.z + x * quaternion.y - y * quaternion.x + z * quaternion.w;
    
    return Quaternion(newW, newX, newY, newZ);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const {return multiply(quaternion);}

void Quaternion::operator*=(const Quaternion& quaternion) {
    Quaternion multipliedQuaternion = multiply(quaternion);
    w = multipliedQuaternion.w;
    x = multipliedQuaternion.x;
    y = multipliedQuaternion.y;
    z = multipliedQuaternion.z;
}

Quaternion Quaternion::multiply(const float scalar) const {return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);}
Quaternion Quaternion::operator*(const float scalar) const {return multiply(scalar);}

void Quaternion::operator*=(const float scalar) {
    w *= scalar;
    x *= scalar; 
    y *= scalar; 
    z *= scalar; 
}

EulerAngles Quaternion::toEulerAngles() const {
    float roll = 0.0; // ROLL (X-axis): Standard display is -180 to 180
    float pitch = 0.0; // Check for Gimbal Lock (Pitch at +/- 90 degrees)
    float yaw = 0.0; // YAW (Z-axis): Calculate initially as -180 to 180

    const float pitchSine = 2.0 * (w * y - z * x);

    if (std::fabs(pitchSine) >= 0.999999) {
        pitch = std::copysign(M_PI / 2.0, pitchSine);
        roll = std::atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y));
    } 
    else {
        pitch = std::asin(pitchSine);

        const float rollNum = 2.0 * (w * x + y * z);
        const float rollDen = 1.0 - 2.0 * (x * x + y * y);
        roll = std::atan2(rollNum, rollDen);

        const float yawNum = 2.0 * (w * z + x * y);
        const float yawDen = 1.0 - 2.0 * (y * y + z * z);
        yaw = std::atan2(yawNum, yawDen);
    }

    if (yaw < 0) {yaw += 2.0 * M_PI;} // Wrap Yaw for display (0 to 360 degrees)

    return EulerAngles(roll, pitch, yaw);
}