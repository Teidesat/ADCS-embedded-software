#include "../lib/hardware/INS/INS.hpp"
#include "../lib/algorithm/attitudeDetermination/attitudeDetermination.hpp"
#include "../lib/algorithm/noiseFilters/noiseFilters.hpp"

#include <string>

INS ins;
AttitudeAlgorithm attitudeAlgorithm;

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
    const float lowThreshold = 10000.0f;
    const float highThreshold = 20000.0f;
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
}

void loop() {
    // update sensors
    ins.update();

    // process sensors data
    //Vector3d pitchGyroscopes(0.0f, .getY(), 0.0f);

    // calculate new attitude
    attitudeAlgorithm.update(ins.getIMU().getAccelerometers(), 
                             ins.getIMU().getGyroscopes(), 
                             ins.getIMU().getMagnetometers(), 
                             ins.getSunSensor().getLux(), 
                             ins.getMicrocontroller().getDeltaTime());

    EulerAngles attitude = EulerAngles::toDegrees(attitudeAlgorithm.getAttitude().toEulerAngles());

    //Serial.println(attitude.shortInfo().c_str());

    // Sensors readings
    //std::string deltatimeStr = ">deltatime:" + std::to_string(ins.getMicrocontroller().getDeltaTime());
    //std::string microcontrollerTemperatureStr = ">microcontrollerTemperature:" + std::to_string(ins.getMicrocontroller().getTemperature());
    std::string luxStr = ">lux:" + std::to_string(ins.getSunSensor().getLux());
    //std::string accelerometerXstr = ">accelerometerX:" + std::to_string(ins.getIMU().getAccelerometers().getX());
    //std::string accelerometerYstr = ">accelerometerY:" + std::to_string(ins.getIMU().getAccelerometers().getY());
    //std::string accelerometerZstr = ">accelerometerZ:" + std::to_string(ins.getIMU().getAccelerometers().getZ());
    //std::string gyroscopeXstr = ">gryroscopeX:" + std::to_string(ins.getIMU().getGyroscopes().getX());
    //std::string gyroscopeYstr = ">gryroscopeY:" + std::to_string(ins.getIMU().getGyroscopes().getY());
    //std::string gyroscopeZstr = ">gryroscopeZ:" + std::to_string(ins.getIMU().getGyroscopes().getZ());
    //std::string magnetometerXstr = ">magnetometerX:" + std::to_string(ins.getIMU().getMagnetometers().getX());
    //std::string magnetometerYstr = ">magnetometerY:" + std::to_string(ins.getIMU().getMagnetometers().getY());
    //std::string magnetometerZstr = ">magnetometerZ:" + std::to_string(ins.getIMU().getMagnetometers().getZ());
    //std::string imuTemperatureStr = ">imuTemperature:" + std::to_string(ins.getIMU().getTemperature());

    //Serial.println(deltatimeStr.c_str());
    //Serial.println(microcontrollerTemperatureStr.c_str());
    Serial.println(luxStr.c_str());
    //Serial.println(accelerometerXstr.c_str());
    //Serial.println(accelerometerYstr.c_str());
    //Serial.println(accelerometerZstr.c_str());
    //Serial.println(gyroscopeXstr.c_str());
    //Serial.println(gyroscopeYstr.c_str());
    //Serial.println(gyroscopeZstr.c_str());
    //Serial.println(magnetometerXstr.c_str());
    //Serial.println(magnetometerYstr.c_str());
    //Serial.println(magnetometerZstr.c_str());
    //Serial.println(imuTemperatureStr.c_str());

    // Attitude
    std::string rollStr = ">Roll:" + std::to_string(attitude.getRoll());
    std::string pitchStr = ">Pitch:" + std::to_string(attitude.getPitch());
    std::string yawStr = ">Yaw:" + std::to_string(attitude.getYaw()); 

    Serial.println(rollStr.c_str());
    Serial.println(pitchStr.c_str());
    Serial.println(yawStr.c_str());
}