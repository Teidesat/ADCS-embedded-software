#include "quaternion.hpp"

#include <cmath>
#include <limits>

Quaternion::Quaternion(const float w, const float x, const float y, const float z): 
    w(w),
    x(x),
    y(y),
    z(z)
{}

float Quaternion::getW() const {return w;}
float Quaternion::getX() const {return x;}
float Quaternion::getY() const {return y;}
float Quaternion::getZ() const {return z;}

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
    return w * quaternion.getW() + x * quaternion.getX() + y * quaternion.getY() + z * quaternion.getZ();
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

    if (dotProductValue < 0.0f) {
        dotProductValue = -dotProductValue;
        shortestPathQuaternion = Quaternion(-quaternion.getW(), -quaternion.getX(), -quaternion.getY(), -quaternion.getZ());
    }

    const float SLERP_THRESHOLD = 0.9995f;
    float newW, newX, newY, newZ;

    if (dotProductValue > SLERP_THRESHOLD) {
        const float inverseTime = 1.0f - time;

        newW = (inverseTime * w) + (time * shortestPathQuaternion.getW());
        newX = (inverseTime * x) + (time * shortestPathQuaternion.getZ());
        newY = (inverseTime * y) + (time * shortestPathQuaternion.getY());
        newZ = (inverseTime * z) + (time * shortestPathQuaternion.getZ());
    }
    else {
        const float totalAngle = std::acos(dotProductValue);
        const float interpolatedAngle = totalAngle * time;
        
        const float sineTotalAngle = std::sin(totalAngle);
        const float sineInterpolatedAngle = std::sin(interpolatedAngle);

        const float startWeight = std::cos(interpolatedAngle) - dotProductValue * sineInterpolatedAngle / sineTotalAngle;
        const float endWeight = sineInterpolatedAngle / sineTotalAngle;

        newW = (startWeight * w) + (endWeight * shortestPathQuaternion.getW());
        newX = (startWeight * x) + (endWeight * shortestPathQuaternion.getX());
        newY = (startWeight * y) + (endWeight * shortestPathQuaternion.getY());
        newZ = (startWeight * z) + (endWeight * shortestPathQuaternion.getZ());
    }

    return Quaternion(newW, newX, newY, newZ).normalize();
}

Quaternion Quaternion::sum(const Quaternion& quaternion) const {
    return Quaternion(w + quaternion.getW(), x + quaternion.getX(), y + quaternion.getY(), z + quaternion.getZ());
}

Quaternion Quaternion::operator+(const Quaternion& quaternion) const {return sum(quaternion);}

Quaternion Quaternion::multiply(const Quaternion& quaternion) const {
    const float newW = w * quaternion.getW() - x * quaternion.getX() - y * quaternion.getY() - z * quaternion.getZ();
    const float newX = w * quaternion.getX() + x * quaternion.getW() + y * quaternion.getZ() - z * quaternion.getY();
    const float newY = w * quaternion.getY() - x * quaternion.getZ() + y * quaternion.getW() + z * quaternion.getX();
    const float newZ = w * quaternion.getZ() + x * quaternion.getY() - y * quaternion.getX() + z * quaternion.getW();
    
    return Quaternion(newW, newX, newY, newZ);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const {return multiply(quaternion);}

void Quaternion::operator*=(const Quaternion& quaternion) {
    Quaternion multipliedQuaternion = multiply(quaternion);
    w = multipliedQuaternion.getW();
    x = multipliedQuaternion.getX();
    y = multipliedQuaternion.getY();
    z = multipliedQuaternion.getZ();
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
    float roll = 0.0f; // ROLL (X-axis): Standard display is -180 to 180
    float pitch = 0.0f; // Check for Gimbal Lock (Pitch at +/- 90 degrees)
    float yaw = 0.0f; // YAW (Z-axis): Calculate initially as -180 to 180

    const float pitchSine = 2.0f * (w * y - z * x);

    if (std::fabs(pitchSine) >= 0.999999f) {
        pitch = std::copysign(M_PI / 2.0f, pitchSine);
        roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
    } 
    else {
        pitch = std::asin(pitchSine);

        const float rollNum = 2.0f * (w * x + y * z);
        const float rollDen = 1.0f - 2.0f * (x * x + y * y);
        roll = std::atan2(rollNum, rollDen);

        const float yawNum = 2.0f * (w * z + x * y);
        const float yawDen = 1.0f - 2.0f * (y * y + z * z);
        yaw = std::atan2(yawNum, yawDen);
    }

    if (yaw < 0) {yaw += 2.0f * M_PI;} // Wrap Yaw for display (0 to 360 degrees)

    return EulerAngles(roll, pitch, yaw);
}