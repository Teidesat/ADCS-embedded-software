#pragma once

#include "../../utils/math/vector3d/vector3d.hpp"

#include <Adafruit_LSM9DS1.h>



class AdafruitLSM9DS1 {
    private:
        Adafruit_LSM9DS1 LSM9DS1;
        Vector3d accelerometers; // meters/seconds^2
        Vector3d gyroscopes; // radians/second
        Vector3d magnetometers; // micro teslas
        float temperature; // degrees celsius

    public:
        enum class AcclerometersRange {G2, G4, G8, G16};
        enum class GyroscopesRange {DPS245, DPS500, DPS2000};
        enum class MagnetometersRange {GAUSS4, GAUSS8, GAUSS12, GAUSS16};

        void begin(const int SDApin, 
                   const int SCLpin, 
                   const AcclerometersRange accelerometersRange = AcclerometersRange::G2, 
                   const GyroscopesRange gyroscopesRange = GyroscopesRange::DPS245, 
                   const MagnetometersRange magnetometersRange = MagnetometersRange::GAUSS4,
                   const int frequencyClock = 0);
        void update();

        Vector3d getAccelerometers() const;
        Vector3d getGyroscopes() const;
        Vector3d getMagnetometers() const;
        float getTemperature() const;
};