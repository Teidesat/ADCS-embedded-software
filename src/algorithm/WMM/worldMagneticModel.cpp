#include "worldMagneticModel.hpp"

#include "magneticModel/magneticModel.hpp"

#include <vector>

namespace WMM {

Vector3d WorldMagneticModel::calculateExpectedMagneticField(const double latitude, const double longitude, const double altitude, const double date) const {
    const MagneticModel magneticModel;
    MagneticModel timedMagneticModel;
}

}