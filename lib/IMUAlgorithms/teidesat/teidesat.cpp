#include "teidesat.hpp"

#include "../../utils/stringFormatting.hpp"

#include  <ArduinoEigenDense.h>

void updateAttitude(Eigen::Quaternion<float>& q_current, const Eigen::Vector3f& omega, float dt) {
    Eigen::Quaternion<float> q_omega(0.0f, omega.x(), omega.y(), omega.z());
    Eigen::Quaternion<float> q_dot = q_current * q_omega;
    
    q_dot.coeffs() *= (0.5f * dt); 
    q_current.coeffs() += q_dot.coeffs(); 
    q_current.normalize();
}


std::string Teidesat::attitudeToString() {
    std::string pitchStr = floatToFormattedString(attitude.pitch);
    std::string rollStr = floatToFormattedString(attitude.roll);
    std::string yawhStr = floatToFormattedString(attitude.yaw);

    return pitchStr + STRING_SEPARATOR + rollStr + STRING_SEPARATOR + yawhStr;
}