#pragma once

#include "../hardware/INS/INStypes/INStypes.hpp"
#include "../../lib/math/vector3d/vector3d.hpp"
#include "../../lib/math/eulerAngles/eulerAngles.hpp"
#include "../../lib/math/quaternion/quaternion.hpp"

#include <string>

namespace ADCS {

std::string microcontrollerToJson(const Microcontroller& microcontroller);
std::string GNSStoJson(const GNSS& gnss);
std::string sunSensorToJson(const SunSensor& sunSensor);
std::string vector3dToJson(const Vector3d& vector);
std::string IMUtoJson(const IMU& imu);

std::string eulerAnglesToJson(const EulerAngles& eulerAngles);
std::string quaternionToJson(const Quaternion& quaternion);

std::string combineJsons(const std::string& json1, const std::string& json2);

}