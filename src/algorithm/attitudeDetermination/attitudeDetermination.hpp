#pragma once

#include "../../lib/math/vector3d/vector3d.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

namespace ADCS {

class AttitudeAlgorithm {
    private:
        math::Quaternion attitudeQuaternion;

        math::Quaternion calculateAccelerationsAttitude(const math::Vector3d& linearAcclerations, const math::Vector3d& magneticFlux) const;
        math::Quaternion calculateAngularVelocitiesAttitude(const math::Vector3d& angularVelocities, const float deltaTime) const;
        math::Quaternion blendQuaternionsWeighted(const math::Quaternion& relativeQuaternion, const math::Quaternion& absoluteQuaterion, const float luminosity);
        float calculateTrustInAccelerometers(const float luminosity, const float maxLuminosity) const;

    public: 
        void update(const math::Vector3d& linearAcclerations, 
                    const math::Vector3d& angularVelocities, 
                    const math::Vector3d& magneticFlux, 
                    const float luminosity, 
                    const float deltaTime);
        math::Quaternion getAttitude() const;
};

}