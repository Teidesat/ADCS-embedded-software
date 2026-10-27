#pragma once

namespace math {

class Constants {
    public:
        static constexpr double doubleComparisonThreshold = 1.0e-12;  // is this the bestvalue to compare doubles or the threshold is too small?
        static constexpr double pi = 3.14159265358979323846;
};

}