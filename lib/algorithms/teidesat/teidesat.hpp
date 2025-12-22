#pragma once

#include "../../utils/math/rotation.hpp"

class Teidesat {
    private:
        Quaternion attitudeQuaternion;

        float calculateGyroscopesTrust(float luminosity) const;
        // functions return normalized quaternions
        Quaternion calculateAcclelerometersAttitude(const Vector3d& linearAcclerations) const;
        Quaternion calculateGyroscopesAttitude(const Vector3d& angularVelocities, const float deltaTime) const;
        Quaternion blendQuaternionsWeighted(const Quaternion& relativeQuaternion, const Quaternion& absoluteQuaterion, const float luminosity);

    public: 
        void update(const Vector3d& accelerations, const Vector3d& angularVelocities, const float luminosity, const float deltaTime);
        Quaternion getAttitude() const;
};