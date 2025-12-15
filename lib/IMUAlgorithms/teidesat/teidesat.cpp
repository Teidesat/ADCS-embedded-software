#include "teidesat.hpp"

#include "../../utils/stringFormatting.hpp"

#include  <ArduinoEigenDense.h>

float Teidesat::calculateGyroscopesTrust(const float& luminosity) {return (95/15) * luminosity;}

Eigen::Quaternionf Teidesat::calculateAcclelerometersAttitude(const SensorData& linearAcclerations) {
    // output is in  euler angles
    EulerAngles calculatedAttitude;
    calculatedAttitude.roll = std::atan2(linearAcclerations.y, linearAcclerations.z);
    float denominator = std::sqrt(linearAcclerations.y * linearAcclerations.y + linearAcclerations.z * linearAcclerations.z);
    calculatedAttitude.pitch = std::atan2(linearAcclerations.x, denominator);

    return eulerAnglesToQuaternion(calculatedAttitude);
}

Eigen::Quaternionf Teidesat::calculateGyroscopesAttitude(const SensorData& angularVelocities, const float& deltaTime) {
    // convention followed: q = w + xi + yj + zk, where z is real number, x is pith, y is roll, z is yaw

    Eigen::Quaternionf angularVelocitiesQuaternion(0.0f, angularVelocities.y, angularVelocities.x, angularVelocities.z);
    Eigen::Quaternionf rateOfChangeQuaternion = attitudeQuaternion * angularVelocitiesQuaternion;

    rateOfChangeQuaternion.coeffs() *= (0.5f * deltaTime); 
    attitudeQuaternion.coeffs() += rateOfChangeQuaternion.coeffs(); 
    attitudeQuaternion.normalize();
}


Eigen::Quaternionf Teidesat::blendQuaternionsWeighted(const Eigen::Quaternionf& Q_Primary, const Eigen::Quaternionf& Q_Absolute, const float& luminosity) {
    float trust = calculateGyroscopesTrust(luminosity);

    Eigen::Quaternionf Q_Error = Q_Absolute * Q_Primary.conjugate();

    if (Q_Error.w() < 0) {Q_Error.coeffs() *= -1.0f;}

    Eigen::Vector3f error_vec(Q_Error.x(), Q_Error.y(), Q_Error.z());
    Eigen::Vector3f corrected_error_vec = trust * error_vec;
    Eigen::Quaternionf Q_Correction(1.0f,corrected_error_vec.x(),corrected_error_vec.y(),corrected_error_vec.z());
    Eigen::Quaternionf Q_New = Q_Correction * Q_Primary;

    Q_New.normalize();
    
    return Q_New;
}

void Teidesat::update(const SensorData& accelerations, const SensorData& angularVelocities, const float& luminosity, const float& deltaTime) {
    Eigen::Quaternionf gyroscopesAttitude = calculateGyroscopesAttitude(angularVelocities, deltaTime);
    Eigen::Quaternionf accelerationsAttitude = calculateAcclelerometersAttitude(accelerations);

    // combine both attitudes according to trust in light conditions
    attitudeQuaternion = blendQuaternionsWeighted(accelerationsAttitude, gyroscopesAttitude, luminosity);
}

EulerAngles Teidesat::getAttitude() {return quaternionToEulerAngles(attitudeQuaternion);}

std::string Teidesat::attitudeToString() {
    EulerAngles attitude = quaternionToEulerAngles(attitudeQuaternion);
    std::string pitchStr = std::to_string(attitude.pitch);
    std::string rollStr = std::to_string(attitude.roll);
    std::string yawhStr = std::to_string(attitude.yaw);

    return pitchStr + STRING_SEPARATOR + rollStr;
}