#pragma once

#include "../../utils/math/vector3d/vector3d.hpp"

#include <Adafruit_LSM9DS1.h>

class AdafruitLSM9DS1 {
    private:
        Adafruit_LSM9DS1 LSM9DS1;
        Vector3d accelerometers; // meters/seconds^2
        Vector3d gyroscopes; // radians/second
        Vector3d magnetometers; // gauss
        float temperature; // degree centigrade
    
    public:
        void begin(const int iSDAPin, const int iSCLPin, int iAccelerometersRange = 0, int iGyroscopesScale = 0, int iMagnetometersGain = 0);
        void update();

        Vector3d getAccelerometers() const;
        Vector3d getGyroscopes() const;
        Vector3d getMagnetometers() const;
        float getTemperature() const;
};