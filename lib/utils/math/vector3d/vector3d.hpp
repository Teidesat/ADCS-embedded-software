#pragma once

class Vector3d {
    public:
        float x;
        float y;
        float z;

        Vector3d(const float x = 0.0, const float y = 0.0, const float z = 0.0);
        Vector3d multiply(const float scalar) const;
        Vector3d operator*(const float scalar) const;
};