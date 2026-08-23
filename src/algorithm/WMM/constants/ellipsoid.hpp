#pragma once

#include <math.h>

namespace WMM {

class Ellipsoid {
    public:
        static const double semiMajorAxis = 6378.137; // a
        static const double semiMinorAxis = 6356.7523142; // b
        static constexpr double flattening = 1 / 298.257223563; // fla
        static constexpr double firstEccentricity = std::sqrt(1 - (semiMinorAxis * semiMinorAxis) / (semiMajorAxis * semiMajorAxis)); // eps
        static constexpr double firstEccentricitySquared = firstEccentricity * firstEccentricity; //epssq
        static const double meanRadius = 6371.2; // re
};

}