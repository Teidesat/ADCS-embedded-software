#pragma once

#include "../../lib/math/vector3d/vector3d.hpp"

namespace WMM {

class WorldMagneticModel {
    private:
        Vector3d calculateExpectedMagneticField(const double latitude, const double longitude, const double altitude, const double date) const;
};

}