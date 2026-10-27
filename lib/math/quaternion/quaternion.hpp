#pragma once

#include "../eulerAngles/eulerAngles.hpp"

namespace math {

class EulerAngles;

class Quaternion {
    public:
        double w = 0.0; // real part
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        Quaternion();
        Quaternion(const double wValue, const double xValue, const double yValue, const double zValue);
        
        Quaternion conjugate() const;
        Quaternion normalize() const;
        double dotProduct(const Quaternion& quaternion) const;
        Quaternion slerp(const Quaternion& quaternion, const double time) const;

        Quaternion sum(const Quaternion& quaternion) const;
        Quaternion operator+(const Quaternion& quaternion) const;
        Quaternion multiply(const Quaternion& quaternion) const;
        Quaternion operator*(const Quaternion& quaternion) const;
        Quaternion multiply(const double scalar) const;
        Quaternion operator*(const double scalar) const;

        EulerAngles toEulerAngles() const; // using sequence ZYX
};

}