#pragma once

namespace math {

class Vector3d {
    public:
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        Vector3d();
        Vector3d(const double xValue, const double yValue, const double zValue);
        Vector3d multiply(const double scalar) const;
        Vector3d operator*(const double scalar) const;
};

}