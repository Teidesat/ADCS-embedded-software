#pragma once

#include "../microcontroller/EspressifESP32WROOM32.hpp"
#include "../GNSS/UbloxNEO6M.hpp"
#include "../sunSensor/AdafruitVEML7700.hpp"
#include "../IMU/AdafruitLSM9DS1.hpp"

class  Microcontroller {
    private:
        float deltaTime;
        float temperature;

    public:
        Microcontroller(float iDeltatime, float iTemperature);
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
        SunSensor(float iALS, float iLux, float iWhite);
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
        IMU(Vector3d iAccelerometers, Vector3d iGyroscopes, Vector3d iMagnetometers, float iTemperature);
        Vector3d getAccelerometers() const;
        Vector3d getGyroscopes() const;
        Vector3d getMagnetometers() const;
        float getTemperature() const;
};

class INS {
    private:
        EspressifESP32WROOM32 microcontroller;
        UbloxNEO6M gnss;
        AdafruitVEML7700 sunSensor;
        AdafruitLSM9DS1 imu;

    public:
        void begin(const int iSDApin, const int iSCLpin, const int iRXpin, const int iTXpin);
        void update();

        Microcontroller getMicrocontrollerData() const;
        GNSS getGNSS() const;
        IMU getIMU() const;
        SunSensor getSunSensor() const;
};