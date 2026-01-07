#pragma once

#include "../microcontroller/EspressifESP32WROOM32.hpp"
#include "../GNSS/UbloxNEO6M.hpp"
#include "../sunSensor/AdafruitVEML7700.hpp"
#include "../IMU/AdafruitLSM9DS1.hpp"



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

class INS {
    private:
        EspressifESP32WROOM32 microcontroller;
        UbloxNEO6M gnss;
        AdafruitVEML7700 sunSensor;
        AdafruitLSM9DS1 imu;

    public:
        void begin(const int SDApin, 
                   const int SCLpin, 
                   const int RXpin, 
                   const int TXpin, 
                   const AdafruitLSM9DS1::AcclerometersRange accelerometersRange = AdafruitLSM9DS1::AcclerometersRange::G2,
                   const AdafruitLSM9DS1::GyroscopesRange gyroscopesRange = AdafruitLSM9DS1::GyroscopesRange::DPS245,
                   const AdafruitLSM9DS1::MagnetometersRange magnetometersRange = AdafruitLSM9DS1::MagnetometersRange::GAUSS4,
                   const float lowThreshold = 10000.0f,
                   const float highThreshold = 20000.0f,
                   const float interruptEnable = true,
                   const bool pauseExecution = true, 
                   const int microcontrollerClockFrequency = 0,
                   const int I2CclockFrequency = 0);
        void update();

        Microcontroller getMicrocontroller() const;
        GNSS getGNSS() const;
        IMU getIMU() const;
        SunSensor getSunSensor() const;
};