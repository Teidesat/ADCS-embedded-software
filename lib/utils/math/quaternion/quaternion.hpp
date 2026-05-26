#pragma once

#include "../eulerAngles/eulerAngles.hpp"

class EulerAngles;

class Quaternion {
    public:
        float w; // real element
        float x;
        float y;
        float z;

        Quaternion(const float w = 1.0, const float x = 0.0, const float y = 0.0, const float z = 0.0);
        
        Quaternion conjugate() const;
        Quaternion normalize() const;
        float dotProduct(const Quaternion& quaternion) const;

        // For comparing and interpolating, both current and input quaternion should be in a normalized form
        bool equal(const Quaternion& quaternion) const;
        bool operator==(const Quaternion& quaternion) const;
        bool operator!=(const Quaternion& quaternion) const;
        Quaternion slerp(const Quaternion& quaternion, const float time) const;

        Quaternion sum(const Quaternion& quaternion) const;
        Quaternion operator+(const Quaternion& quaternion) const;
        Quaternion multiply(const Quaternion& quaternion) const;
        Quaternion operator*(const Quaternion& quaternion) const;
        void operator*=(const Quaternion& quaternion);
        Quaternion multiply(const float scalar) const;
        Quaternion operator*(const float scalar) const;
        void operator*=(const float scalar);

        EulerAngles toEulerAngles() const; // using sequence ZYX
};