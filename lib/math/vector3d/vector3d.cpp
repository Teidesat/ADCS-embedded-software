#include "vector3d.hpp"

namespace math {

Vector3d::Vector3d(const double xValue, const double yValue, const double zValue):
    x(xValue),
    y(yValue),
    z(zValue)
{}

Vector3d Vector3d::multiply(const double scalar) const {return Vector3d(x * scalar, y * scalar, z * scalar);}
Vector3d Vector3d::operator*(const double scalar) const {return multiply(scalar);}

}