#include "trigonometry.hpp"



Attitude calculateAttitude(float ax, float ay, float az, float mx, float my, float mz) {
  // calculate pitch, roll, yaw in radians using only the accelerometers
  const float pitchRadians = atan2(ax, az);
  const float rollRadians = atan2(ay, az);
  const float yawRadians = atan2(ay, ax);

  // convert radians to degrees
  Attitude calculatedAttitude;
  calculatedAttitude.pitch = pitchRadians * RADIANS_TO_DEGRESS_CONVERSION_RATIO;
  calculatedAttitude.roll = rollRadians * RADIANS_TO_DEGRESS_CONVERSION_RATIO;
  calculatedAttitude.yaw = 90.0 - atan(mx / my) * RADIANS_TO_DEGRESS_CONVERSION_RATIO;

  return calculatedAttitude;
}
