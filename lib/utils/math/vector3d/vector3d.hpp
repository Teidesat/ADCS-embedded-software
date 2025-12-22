#pragma once

class Vector3d {
    private:
        float x;
        float y;
        float z;

    public:
        Vector3d(const float iX = 0.0f, const float iY = 0.0f, const float iZ = 0.0f);
        float getX() const;
        float getY() const;
        float getZ() const;
        Vector3d multiply(const float scalar) const;
        Vector3d operator*(const float scalar) const;
};