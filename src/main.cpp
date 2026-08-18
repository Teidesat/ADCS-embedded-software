#include "hardware/INS/INS.hpp"
#include "algorithm/attitudeDetermination/attitudeDetermination.hpp"
#include "../lib/noiseFilters/noiseFilters.hpp"
#include "toJson/toJson.hpp"

#include <string>

ADCS::INS ins;
ADCS::AttitudeAlgorithm attitudeAlgorithm;

void setup() {
    // UART Pins (GNSS)
    const int RXpin = 16;
    const int TXpin = 17;
    
    // microcontroller setup
    const bool pauseExecution = true;
    const int microcontrollerClockFrequency = 240;

    // I2C Pins (IMU and sun sensor)
    const int SDApin = 26;
    const int SCLpin = 27;
    const int I2CclockFrequency = 400000;

    // IMU sensors ranges
    const AdafruitLSM9DS1::AcclerometersRange accelerometersRange = AdafruitLSM9DS1::AcclerometersRange::G2;
    const AdafruitLSM9DS1::GyroscopesRange gyroscopesRange = AdafruitLSM9DS1::GyroscopesRange::DPS245;
    const AdafruitLSM9DS1::MagnetometersRange magnetometersRange = AdafruitLSM9DS1::MagnetometersRange::GAUSS4;

    // light sensor range
    const float lowThreshold = 10000.0;
    const float highThreshold = 20000.0;
    const float interruptEnable = true;

    ins.begin(SDApin, 
              SCLpin, 
              RXpin, 
              TXpin, 
              accelerometersRange, 
              gyroscopesRange, 
              magnetometersRange, 
              lowThreshold,
              highThreshold,
              interruptEnable,
              pauseExecution,
              microcontrollerClockFrequency, 
              I2CclockFrequency);

    bool setupReady = true; // condition here checking that all necesary data is set
    if(!setupReady) {ESP.restart();}
}

void loop() {
    // update sensors
    ins.update();

    // process sensors data
    const float highGyroThreshold = 0.08f;
    const float lowGyroThreshold = -highGyroThreshold;
    const float clampedGyroscopeX = deadband(ins.getIMU().getGyroscopes().x, lowGyroThreshold, highGyroThreshold);
    const float clampedGyroscopeY = deadband(ins.getIMU().getGyroscopes().y, lowGyroThreshold, highGyroThreshold);
    const float clampedGyroscopeZ = deadband(ins.getIMU().getGyroscopes().z, lowGyroThreshold, highGyroThreshold);
    const Vector3d clampedGyroscopes(clampedGyroscopeX, clampedGyroscopeY, clampedGyroscopeZ);
    
    // calculate new position
    // add to all functions checks for all the parameters are in the correct thresholds, if not, restart the system?
    attitudeAlgorithm.update(ins.getIMU().getAccelerometers(), 
                             clampedGyroscopes, 
                             ins.getIMU().getMagnetometers(), 
                             ins.getSunSensor().getLux(), 
                             ins.getMicrocontroller().getDeltaTime());

    // output
    Quaternion attitudeQuaternion = attitudeAlgorithm.getAttitude();
    EulerAngles attitudeEulerAngles = EulerAngles::toDegrees(attitudeQuaternion.toEulerAngles());

    std::string luxTeleplot = ">lux: " + std::to_string(ins.getSunSensor().getLux());
    std::string gyroXteleplot = ">gyroX: " + std::to_string(ins.getIMU().getGyroscopes().x);
    std::string gyroYteleplot = ">gyroY: " + std::to_string(ins.getIMU().getGyroscopes().y);
    std::string gyroZteleplot = ">gyroZ: " + std::to_string(ins.getIMU().getGyroscopes().z);
    std::string gyroXclampedTeleplot = ">gyroXclamped: " + std::to_string(clampedGyroscopeX);
    std::string gyroYclampedTeleplot = ">gyroYclamped: " + std::to_string(clampedGyroscopeY);
    std::string gyroZclampedTeleplot = ">gyroZclamped: " + std::to_string(clampedGyroscopeZ);
    std::string rollTeleplot = ">roll: " + std::to_string(attitudeEulerAngles.getRoll());
    std::string pitchTeleplot = ">pitch: " + std::to_string(attitudeEulerAngles.getPitch());
    std::string yawTeleplot = ">yaw: " + std::to_string(attitudeEulerAngles.getYaw());

    //Serial.println(luxTeleplot.c_str());
    //Serial.println(gyroXteleplot.c_str());
    //Serial.println(gyroYteleplot.c_str());
    //Serial.println(gyroZteleplot.c_str());
    //Serial.println(gyroXclampedTeleplot.c_str());
    //Serial.println(gyroYclampedTeleplot.c_str());
    //Serial.println(gyroZclampedTeleplot.c_str());
    //Serial.println(rollTeleplot.c_str());
    //Serial.println(pitchTeleplot.c_str());
    //Serial.println(yawTeleplot.c_str());

    Serial.println(quaternionToJson(attitudeQuaternion).c_str());
    //Serial.println(eulerAnglesToJson(attitudeEulerAngles).c_str());
}