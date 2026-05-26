#pragma once

#include "../../lib/hardware/INS/INS.hpp"
#include "../../lib/algorithm/attitudeDetermination/attitudeDetermination.hpp"

#include <string>

std::string microcontrollerToJson(const Microcontroller& microcontroller);
std::string GNSStoJson(const GNSS& gnss);
std::string sunSensorToJson(const SunSensor& sunSensor);
std::string vector3dToJson(const Vector3d& vector);
std::string IMUtoJson(const IMU& imu);

std::string eulerAnglesToJson(const EulerAngles& eulerAngles);
std::string quaternionToJson(const Quaternion& quaternion);
std::string AttitudeToJson(const AttitudeAlgorithm& attitudeAlgorithm);

std::string combineJsons(const std::string& json1, const std::string& json2);