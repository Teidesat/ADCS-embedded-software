#pragma once

#include "../../lib/math/vector3d/vector3d.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

Quaternion QUEST(const Vector3d& accelerometerBody, 
                 const Vector3d& gravityReference, 
                 const Vector3d& magnetometerBody, 
                 const Vector3d& magneticNorthReference, 
                 const float accelerometerWeight, 
                 const float magnetometerWeight);