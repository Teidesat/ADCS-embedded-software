
#include <Adafruit_LSM9DS1.h>

// hardware

// Arduino data transfer rate
const unsigned short int DATA_RATE = 9600;

// IMU object
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();



// structures for the retrival of the sensors readings

typedef struct SensorData {
    float x, y, z;
};

typedef struct IMUdata {
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

typedef struct Attitude {
    float pitch, roll, yaw;
};


// funtions interfacing the hardware

void setupESP12(const bool pauseExecution = true);

void setupLSM9DS1();

void setupHardware(const bool pauseExecution = true);

IMUdata readSensors();










// Serial port output

const float RADIANS_TO_DEGRESS_CONVERSION_RATIO = 180.0 / M_PI;

const char  STRING_SEPARATOR = ',';

String formatSensorValues(SensorData& sensorReading);

String formatIMUvalues(IMUdata& imuReading);

String formatAttitudeValues(Attitude& attitudeReading);