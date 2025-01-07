#include "hardware_interface.hpp"

#include <math.h>

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

void setupESP12(const bool pauseExecution) {
  Serial.begin(DATA_RATE);
  if(pauseExecution) {
    while(!Serial) {delay(1);} // will pause Zero, Leonardo, etc until serial console opens
  }
}

// set up sensors sensitivity
void setupLSM9DS1() {

  // Try to initialise and warn if we couldn't detect the chip
  if(!lsm.begin()) {
    Serial.println("Unable to initialize the LSM9DS1");
    while (1);
  }

  // set sensors range / sensitivity

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
}

void setupHardware(const bool pauseExecution) {
  setupESP12(&pauseExecution);
  setupLSM9DS1();
}

IMUdata readSensors() {
  sensors_event_t accelerometers, magnetometers, gyroscopes, temperature;
  lsm.getEvent(&accelerometers, &magnetometers, &gyroscopes, &temperature);

  IMUdata IMUreadings;
  
  IMUreadings.accelerometers.x = accelerometers.acceleration.x;
  IMUreadings.accelerometers.y = accelerometers.acceleration.y;
  IMUreadings.accelerometers.z = accelerometers.acceleration.z;

  IMUreadings.gyroscopes.x = gyroscopes.gyro.x;
  IMUreadings.gyroscopes.y = gyroscopes.gyro.y;
  IMUreadings.gyroscopes.z = gyroscopes.gyro.z;

  IMUreadings.magnetometers.x = magnetometers.magnetic.x;
  IMUreadings.magnetometers.y = magnetometers.magnetic.y;
  IMUreadings.magnetometers.z = magnetometers.magnetic.z;

  IMUreadings.temperature = temperature.temperature;

  return IMUreadings;
}






Attitude fillAttitudeValues(float pitch, float roll, float yaw) {
  Attitude attitudeReading;

  attitudeReading.pitch = pitch;
  attitudeReading.roll = roll;
  attitudeReading.yaw = yaw;

  return attitudeReading;
}

Attitude convertAttitudeRadiansToDegrees(const Attitude& attitudeReadingInRadians) {
  const float RADIANS_TO_DEGRESS_CONVERSION_RATIO = 180.0 / M_PI;
  
  Attitude attitudeReadingInDegrees;
  attitudeReadingInDegrees.pitch = attitudeReadingInRadians.pitch * RADIANS_TO_DEGRESS_CONVERSION_RATIO;
  attitudeReadingInDegrees.roll = attitudeReadingInRadians.roll * RADIANS_TO_DEGRESS_CONVERSION_RATIO;
  attitudeReadingInDegrees.yaw = attitudeReadingInRadians.yaw * RADIANS_TO_DEGRESS_CONVERSION_RATIO;

  return attitudeReadingInDegrees;
}






// Serial port output

String formatSensorValues(const SensorData& sensorReading) {
  return String(sensorReading.x) + STRING_SEPARATOR + String(sensorReading.y) + STRING_SEPARATOR + String(sensorReading.z);
}

String formatIMUvalues(const IMUdata& imuReading) {
  String accelerometersFormattedValues = formatSensorValues(imuReading.accelerometers);
  String gyroscopesFormattedValues = formatSensorValues(imuReading.gyroscopes);
  String magnetometersFormattedValues = formatSensorValues(imuReading.magnetometers);

  return accelerometersFormattedValues + STRING_SEPARATOR + gyroscopesFormattedValues + STRING_SEPARATOR + magnetometersFormattedValues;
}

String formatPitchandRoll(const Attitude& attitudeReading) {
  return String(attitudeReading.pitch) + STRING_SEPARATOR + String(attitudeReading.roll);
}

String formatAttitudeValues(const Attitude& attitudeReading) {
  return String(attitudeReading.pitch) + STRING_SEPARATOR + String(attitudeReading.roll) + STRING_SEPARATOR + String(attitudeReading.yaw);
}