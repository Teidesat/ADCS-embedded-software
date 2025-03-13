#include "trigonometry.hpp"



Orientation calculateAttitude(float ax, float ay, float az, float mx, float my, float mz) {
  // calculate pitch, roll, yaw in radians using only the accelerometers
  const float pitchRadians = atan2(ax, az);
  const float rollRadians = atan2(ay, az);
  const float yawRadians = atan2(ay, ax);

  // convert radians to degrees
  Orientation calculatedAttitude;
  calculatedAttitude.pitch = pitchRadians;
  calculatedAttitude.roll = rollRadians;
  calculatedAttitude.yaw = 90.0 - atan(mx / my);

  return calculatedAttitude;
}
