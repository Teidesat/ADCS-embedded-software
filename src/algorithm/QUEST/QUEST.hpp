#pragma once

#include "../../lib/math/vector3d/vector3d.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

math::Quaternion QUEST(const math::Vector3d& accelerometerBody, 
                       const math::Vector3d& gravityReference, 
                       const math::Vector3d& magnetometerBody, 
                       const math::Vector3d& magneticNorthReference, 
                       const double accelerometerWeight, 
                       const double magnetometerWeight);