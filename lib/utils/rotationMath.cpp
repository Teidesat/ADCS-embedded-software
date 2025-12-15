#include "rotationMath.hpp"

EulerAngles eulerAnglesRadiansToDegrees(const EulerAngles& eulerAngles) {
    EulerAngles eulerAnglesInDegrees;
    eulerAnglesInDegrees.pitch = eulerAngles.pitch * RADIANS_TO_DEGREES;
    eulerAnglesInDegrees.roll = eulerAngles.roll * RADIANS_TO_DEGREES;
    eulerAnglesInDegrees.yaw = eulerAngles.yaw * RADIANS_TO_DEGREES;
    
    return eulerAnglesInDegrees;
}

EulerAngles quaternionToEulerAngles(const Eigen::Quaternionf& quaternion) {
    // (2, 1, 0) specifies the rotation sequence Z-Y-X (Yaw-Pitch-Roll), which is standard in aerospace and IMU systems
    Eigen::Vector3f eulerAnglesZYX = quaternion.toRotationMatrix().eulerAngles(2, 1, 0); 
    EulerAngles angles;
    angles.yaw = eulerAnglesZYX(0);
    angles.pitch = eulerAnglesZYX(1);
    angles.roll  = eulerAnglesZYX(2);

    return eulerAnglesRadiansToDegrees(angles);
}

Eigen::Quaternionf eulerAnglesToQuaternion(const EulerAngles& eulerAngles) {
    Eigen::AngleAxisf rollAngle(eulerAngles.roll, Eigen::Vector3f::UnitX());
    Eigen::AngleAxisf pitchAngle(eulerAngles.pitch, Eigen::Vector3f::UnitY());
    Eigen::AngleAxisf yawAngle(eulerAngles.yaw, Eigen::Vector3f::UnitZ());

    // Combine the rotations in Z-Y-X order (Yaw * Pitch * Roll). The multiplication order matters, The order below implements Z-Y-X.
    Eigen::Quaternionf quaternion = yawAngle * pitchAngle * rollAngle;
    quaternion.normalize(); 

    return quaternion;
}