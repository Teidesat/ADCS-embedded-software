#pragma once

#include  <ArduinoEigenDense.h>

const float radiansToDegreesConversionRate = 180.0f / M_PI;

struct EulerAngles {float pitch, roll, yaw;};

EulerAngles eulerAnglesRadiansToDegrees(const EulerAngles& eulerAngles);

EulerAngles quaternionToEulerAngles(const Eigen::Quaternionf& quaternion); // attitude is returned in radians
Eigen::Quaternionf eulerAnglesToQuaternion(const EulerAngles& eulerAngles); //attitude is assumed to be in radians