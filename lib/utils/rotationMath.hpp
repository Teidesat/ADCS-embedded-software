#pragma once

#include  <ArduinoEigenDense.h>

const float RADIANS_TO_DEGREES = 180.0f / M_PI;

struct SensorData {float x, y, z;};
struct EulerAngles {float pitch, roll, yaw;};

EulerAngles eulerAnglesRadiansToDegrees(const EulerAngles& eulerAngles);
EulerAngles quaternionToEulerAngles(const Eigen::Quaternionf& quaternion); // attitude is returned in radians
Eigen::Quaternionf eulerAnglesToQuaternion(const EulerAngles& eulerAngles); //attitude is assumed to be in radians