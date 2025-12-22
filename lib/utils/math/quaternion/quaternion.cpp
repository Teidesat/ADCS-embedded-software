#include "quaternion.hpp"

#include <math.h>

Quaternion::Quaternion(const float iX, const float iY, const float iZ, const float iW): 
    x(iX), 
    y(iY),  
    z(iZ), 
    w(iW) 
{}

float Quaternion::getX() const {return x;}
float Quaternion::getY() const {return y;}
float Quaternion::getZ() const {return z;}
float Quaternion::getW() const {return w;}

Quaternion Quaternion::conjugate() const {return Quaternion(-x, -y, -z, w);}

Quaternion Quaternion::normalize() const {
    float magnitude = std::sqrt(x*x + y*y + z*z + w*w);
    float normalizedW = w; 
    float normalizedX = x; 
    float normalizedY = y; 
    float normalizedZ = z;

    if (magnitude > 0.0) {
        normalizedX /= magnitude; 
        normalizedY /= magnitude; 
        normalizedZ /= magnitude;
        normalizedW /= magnitude; 
    }

    return Quaternion(normalizedX, normalizedY, normalizedZ, normalizedW);
}

Quaternion Quaternion::multiply(const Quaternion& quaternion) const {
    const float newX = w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y;
    const float newY = w * quaternion.y - x * quaternion.z + y * quaternion.w + z * quaternion.x;
    const float newZ = w * quaternion.z + x * quaternion.y - y * quaternion.x + z * quaternion.w;
    const float newW = w * quaternion.w - x * quaternion.x - y * quaternion.y - z * quaternion.z;
    
    return Quaternion(newX, newY, newZ, newW);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const {return multiply(quaternion);}

void Quaternion::operator*=(const Quaternion& quaternion) {
    Quaternion multipliedQuaternion = multiply(quaternion);
    x = multipliedQuaternion.getX();
    y = multipliedQuaternion.getY();
    z = multipliedQuaternion.getZ();
    w = multipliedQuaternion.getW();
}

Quaternion Quaternion::multiply(const float scalar) const {return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);}
Quaternion Quaternion::operator*(const float scalar) const {return multiply(scalar);}
void Quaternion::operator*=(const float scalar) {x *= scalar; y *= scalar; z *= scalar; w *= scalar;}

EulerAngles Quaternion::toEulerAnglesTrigonometric() const {
    // We calculate the sine of the pitch angle first.
    float pitch = 0.0f;
    float sinePitch = 2.0f * (w * x - y * z);
    
    // Safety check: asinf(x) only works if x is between -1 and 1.
    // Floating point noise can sometimes push 1.0 to 1.000001.
    if (std::fabs(sinePitch) >= 1.0f) {
        // Gimbal Lock case: set to pi/2 (90 degrees)
        constexpr float DEGREES_TO_RADIANS = M_PI / 180.0f;
        pitch = std::copysign(90.0f * DEGREES_TO_RADIANS, sinePitch); 
    } 
    else {pitch = std::asin(sinePitch);}

    // 2. Roll (Y-axis)
    // atan2f is used here because it handles the 360-degree range correctly.
    const float rollNumerator = 2.0f * (w * y + x * z);
    const float rollDenominator = 1.0f - 2.0f * (x * x + y * y);
    const float roll = std::atan2(rollNumerator, rollDenominator);

    // 3. Yaw (Z-axis)
    const float yawNumerator = 2.0f * (w * z + x * y);
    const float yawDenominator = 1.0f - 2.0f * (x * x + z * z);
    const float yaw = std::atan2(yawNumerator, yawDenominator);

    return EulerAngles(pitch, roll, yaw);
}

EulerAngles Quaternion::toEulerAnglesMatrix() const {
    // Create Rotation Matrix entries from Quaternion
    const float matrix10 = 2.0f * (x * y + w * z);
    const float matrix11 = w * w - x * x + y * y - z * z;
    const float matrix12 = 2.0f * (y * z - w * x);
    const float matrix02 = 2.0f * (x * z + w * y);
    const float matrix22 = w * w - x * x - y * y + z * z;

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    // Extract Pitch (check for Gimbal Lock)
    if (std::fabs(matrix12) > 0.9999f) {
        pitch = std::copysign(90.0f, -matrix12);
        roll = 0; // Lock roll to zero
        yaw = std::atan2(-matrix10, matrix11) * 57.29578f;
    } 
    else {
        pitch = std::asin(-matrix12) * 57.29578f;
        roll  = std::atan2(matrix02, matrix22) * 57.29578f;
        yaw   = std::atan2(matrix10, matrix11) * 57.29578f;
    }

    return EulerAngles(pitch, roll, yaw);
}

EulerAngles Quaternion::toEulerAngles() const {return toEulerAnglesTrigonometric();}