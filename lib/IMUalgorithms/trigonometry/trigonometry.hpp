#include "math.h"

struct Orientation {
  float pitch, roll, yaw;
};

Orientation calculateAttitude(float ax, float ay, float az, float mx, float my, float mz);