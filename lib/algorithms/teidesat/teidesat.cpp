#include "teidesat.hpp"

float Teidesat::calculateGyroscopesTrust(float luminosity) const {return (95/15) * luminosity;}

Quaternion Teidesat::calculateAcclelerometersAttitude(const Vector3d& linearAcclerations) const {
    float pitchDenominator = std::sqrt(linearAcclerations.getY() * linearAcclerations.getY() + linearAcclerations.getZ() * linearAcclerations.getZ());
    const float pitch = std::atan2(linearAcclerations.getX(), pitchDenominator);
    const float roll = std::atan2(linearAcclerations.getY(), linearAcclerations.getZ());
    EulerAngles accelerometersdAttitude(pitch, roll, 0.0f);
    
    return accelerometersdAttitude.toQuaternion().normalize();
}

Quaternion Teidesat::calculateGyroscopesAttitude(const Vector3d& angularVelocities, const float deltaTime) const {
    Quaternion angularVelocitiesQuaternion(angularVelocities.getX(), angularVelocities.getY(), angularVelocities.getZ(), 0.0f);
    Quaternion rateOfChangeQuaternion = attitudeQuaternion * angularVelocitiesQuaternion;
    rateOfChangeQuaternion = rateOfChangeQuaternion * (0.5f * deltaTime); // the division by 2 is neccesary

    return rateOfChangeQuaternion.normalize();
}

Quaternion Teidesat::blendQuaternionsWeighted(const Quaternion& relativeQuaternion, const Quaternion& absoluteQuaterion, const float luminosity) {
    Quaternion errorQuaternion = absoluteQuaterion * relativeQuaternion.conjugate();
    if (errorQuaternion.getW() < 0) {errorQuaternion *= -1.0f;}
    Vector3d errorVector(errorQuaternion.getX(), errorQuaternion.getY(), errorQuaternion.getZ());

    float trust = calculateGyroscopesTrust(luminosity);
    Vector3d correctedErrorVector = errorVector * trust;

    Quaternion correctionQuaternion(correctedErrorVector.getX(),correctedErrorVector.getY(),correctedErrorVector.getZ(), 1.0f);
    Quaternion newAttitudeQuaternion = correctionQuaternion * relativeQuaternion;
    newAttitudeQuaternion.normalize();
    
    return newAttitudeQuaternion;
}

void Teidesat::update(const Vector3d& accelerations, const Vector3d& angularVelocities, float luminosity, float deltaTime) {
    Quaternion gyroscopesAttitude = calculateGyroscopesAttitude(angularVelocities, deltaTime);
    Quaternion accelerationsAttitude = calculateAcclelerometersAttitude(accelerations);

    // combine both attitudes according to trust in light conditions
    attitudeQuaternion = blendQuaternionsWeighted(accelerationsAttitude, gyroscopesAttitude, luminosity);
}

Quaternion Teidesat::getAttitude() const {return attitudeQuaternion;}