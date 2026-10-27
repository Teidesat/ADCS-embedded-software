#pragma once

#include "../../../../lib/math/vector3d/vector3d.hpp"

#include <string>

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

    public:
        GNSS(std::string& message);
        
        void parseMessage(const std::string& message);
    
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
        math::Vector3d accelerometers;
        math::Vector3d gyroscopes;
        math::Vector3d magnetometers;
        float temperature;

    public:
        IMU(const math::Vector3d accelerometers, const math::Vector3d gyroscopes, const math::Vector3d magnetometers, const float temperature);
        math::Vector3d getAccelerometers() const;
        math::Vector3d getGyroscopes() const;
        math::Vector3d getMagnetometers() const;
        float getTemperature() const;
};

}