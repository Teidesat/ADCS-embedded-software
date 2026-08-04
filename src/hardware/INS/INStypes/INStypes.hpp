#pragma once

#include "../../../../lib/math/vector3d/vector3d.hpp"

namespace ADCS {

class  Microcontroller {
    private:
        int clockFrequency;
        float deltaTime;
        float temperature;

    public:
        Microcontroller(const float deltaTime, const float temperature);
        float getDeltaTime() const;
        float getTemperature() const;
};

class GNSS {
    private:
        float latitude;
        float longitude;
        float altitude;
        float speed;
        float course;
        float satellites;
};

class SunSensor {
    private:
        float ALS;
        float lux;
        float white;

    public:
        SunSensor(const float ALS, const float lux, const float white);
        float getALS() const;
        float getLux() const;
        float getWhite() const;
};

class IMU {
    private:
        Vector3d accelerometers;
        Vector3d gyroscopes;
        Vector3d magnetometers;
        float temperature;

    public:
        IMU(const Vector3d accelerometers, const Vector3d gyroscopes, const Vector3d magnetometers, const float temperature);
        Vector3d getAccelerometers() const;
        Vector3d getGyroscopes() const;
        Vector3d getMagnetometers() const;
        float getTemperature() const;
};

}