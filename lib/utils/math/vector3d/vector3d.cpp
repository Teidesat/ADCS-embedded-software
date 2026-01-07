#include "vector3d.hpp"

Vector3d::Vector3d(const float iX, const float iY, const float iZ):
    x(iX),
    y(iY),
    z(iZ)
{}

float Vector3d::getX() const {return x;}
float Vector3d::getY() const {return y;}
float Vector3d::getZ() const {return z;}

Vector3d Vector3d::multiply(const float scalar) const {return Vector3d(x * scalar, y * scalar, z * scalar);}
Vector3d Vector3d::operator*(const float scalar) const {return multiply(scalar);}