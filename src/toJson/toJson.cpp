#include "toJson.hpp"

namespace ADCS {

std::string microcontrollerToJson(const Microcontroller& microcontroller) {
    std::string json = "{";
    json += "\"deltaTime\":" + std::to_string(microcontroller.getDeltaTime()) + ",";
    json += "\"temperature\":" + std::to_string(microcontroller.getTemperature());
    json += "}";

    return json;
}

std::string GNSStoJson(const GNSS& gnss) {
    std::string json = "";

    return json;
}

std::string sunSensorToJson(const SunSensor& sunSensor) {
    std::string json = "{";
    json += "\"ALS\":" + std::to_string(sunSensor.getALS()) + ",";
    json += "\"lux\":" + std::to_string(sunSensor.getLux()) + ",";
    json += "\"white\":" + std::to_string(sunSensor.getWhite());
    json += "}";

    return json;
}

std::string vector3dToJson(const math::Vector3d& vector) {
    std::string json = "{";
    json += "\"x\":" + std::to_string(vector.x) + ",";
    json += "\"y\":" + std::to_string(vector.y) + ",";
    json += "\"z\":" + std::to_string(vector.z);
    json += "}";

    return json;
}

std::string IMUtoJson(const IMU& imu) {
    std::string json = "{";
    json += "\"accelerometers\":" + vector3dToJson(imu.getAccelerometers()) + ",";
    json += "\"gyroscopes\":" + vector3dToJson(imu.getGyroscopes()) + ",";
    json += "\"magnetometers\":" + vector3dToJson(imu.getMagnetometers()) + ",";
    json += "\"temperature\":" + std::to_string(imu.getTemperature());  
    json += "}";

    return json;
}

std::string eulerAnglesToJson(const math::EulerAngles& eulerAngles) {
    std::string json = "{";
    json += "\"roll\":" + std::to_string(eulerAngles.roll) + ",";
    json += "\"pitch\":" + std::to_string(eulerAngles.pitch) + ",";
    json += "\"yaw\":" + std::to_string(eulerAngles.yaw);
    json += "}";

    return json;
}
std::string quaternionToJson(const math::Quaternion& quaternion) {
    std::string json = "{";
    json += "\"w\":" + std::to_string(quaternion.w) + ",";
    json += "\"x\":" + std::to_string(quaternion.x) + ",";
    json += "\"y\":" + std::to_string(quaternion.y) + ",";
    json += "\"z\":" + std::to_string(quaternion.z);
    json += "}";

    return json;
}

std::string combineJsons(const std::string& json1, const std::string& json2) {
    std::string json = "{";
    json += json1.substr(1, json1.size() - 2) + ",";
    json += json2.substr(1, json2.size() - 2);
    json += "}";

    return json;
}

}