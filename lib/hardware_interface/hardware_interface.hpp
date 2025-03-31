#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H


#include <Adafruit_LSM9DS1.h>

// hardware

// Arduino data transfer rate
const unsigned short int DATA_RATE = 9600;

// IMU object
extern Adafruit_LSM9DS1 lsm;



// structures for the retrival of the sensors readings

class SensorData {
    public:
        float x, y, z;
};

class IMUdata {
    public:
        SensorData accelerometers;
        SensorData gyroscopes;
        SensorData magnetometers;
        float temperature;
};

/**
 * Attitude from IMU axis
 * x (pitch)  
 * y (roll)
 * z (yaw / heading)
 */
class Attitude {
    public:
        float pitch, roll, yaw;
};


// funtions interfacing the hardware

void setupESP12(const bool pauseExecution = true);

void setupLSM9DS1();

void setupHardware(const bool pauseExecution = true);

IMUdata readSensors();






// standard attitude format

Attitude fillAttitudeValues(float pitch, float roll, float yaw);

Attitude convertAttitudeRadiansToDegrees(const Attitude& attitudeReadingInRadians);


// Serial plot
const char  STRING_SEPARATOR = ',';

String formatSensorValues(const SensorData& sensorReading);

String formatIMUvalues(const IMUdata& imuReading);

String formatPitchandRoll(const Attitude& attitudeReading);

String formatAttitudeValues(const Attitude& attitudeReading);


#endif