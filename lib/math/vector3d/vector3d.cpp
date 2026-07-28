#include "vector3d.hpp"

Vector3d::Vector3d(const float x, const float y, const float z):
    x(x),
    y(y),
    z(z)
{}

Vector3d Vector3d::multiply(const float scalar) const {return Vector3d(x * scalar, y * scalar, z * scalar);}
Vector3d Vector3d::operator*(const float scalar) const {return multiply(scalar);}