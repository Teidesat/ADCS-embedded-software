#pragma once

#include "../eulerAngles/eulerAngles.hpp"

class Quaternion {
    private:    
        float x;
        float y;
        float z;
        float w; // real part

    public:
        Quaternion(const float iX = 0.0f, const float iY = 0.0f, const float iZ = 0.0f, const float iW = 1.0f);
        float getX() const;
        float getY() const;
        float getZ() const;
        float getW() const;
        Quaternion conjugate() const;
        Quaternion normalize() const;
        Quaternion multiply(const Quaternion& quaternion) const;
        Quaternion operator*(const Quaternion& quaternion) const;
        void operator*=(const Quaternion& quaternion);
        Quaternion multiply(const float scalar) const;
        Quaternion operator*(const float scalar) const;
        void operator*=(const float scalar);

        // implementing different methods for conversion
        EulerAngles toEulerAnglesTrigonometric() const; // it is suppose to be the best for space applications
        EulerAngles toEulerAnglesMatrix() const;
        EulerAngles toEulerAngles() const;
};