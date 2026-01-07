#pragma once

#include "../../utils/math/rotation.hpp"

class AttitudeAlgorithm {
    private:
        Quaternion attitudeQuaternion;

        Quaternion calculateAccelerationsAttitude(const Vector3d& linearAcclerations, const Vector3d& magneticFlux) const;
        Quaternion calculateAngularVelocitiesAttitude(const Vector3d& angularVelocities, const float deltaTime) const;
        Quaternion blendQuaternionsWeighted(const Quaternion& relativeQuaternion, const Quaternion& absoluteQuaterion, const float luminosity);
        float calculateTrustInAccelerometers(const float luminosity, const float maxLuminosity) const;

    public: 
        void update(const Vector3d& linearAcclerations, 
                    const Vector3d& angularVelocities, 
                    const Vector3d& magneticFlux, 
                    const float luminosity, 
                    const float deltaTime);
        Quaternion getAttitude() const;
};