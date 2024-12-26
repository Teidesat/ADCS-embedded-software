//main-trigonometry.txt

#include "hardware_interface/hardware_interface.hpp"
#include "algorithms/trigonometry/trigonometry.hpp"







void setup() {
  setupHardware();
}

void loop() {
  IMUdata imuRawValues = readSensors();
  String rawDataStr = formatIMUvalues(imuRawValues);
  
  Attitude currentAttitude = calculateAttitude(imuRawValues.accelerometers.x, imuRawValues.accelerometers.y, imuRawValues.accelerometers.z, imuRawValues.magnetometers.x, imuRawValues.magnetometers.y);
  Sting attitudeStr = formatAttitudeValues(currentAttitude);

  // print combined accelerometers and gyroscopes data and heading with magnetometer
  Serial.println(attitudeStr);
}