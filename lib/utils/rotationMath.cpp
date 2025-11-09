#include "rotationMath.hpp"

EulerAngles eulerAnglesRadiansToDegrees(const EulerAngles& eulerAngles) {
    EulerAngles eulerAnglesInDegrees;
    eulerAnglesInDegrees.pitch = eulerAngles.pitch * radiansToDegreesConversionRate;
    eulerAnglesInDegrees.roll = eulerAngles.roll * radiansToDegreesConversionRate;
    eulerAnglesInDegrees.yaw = eulerAngles.yaw * radiansToDegreesConversionRate;
    
    return eulerAnglesInDegrees;
}

EulerAngles quaternionToEulerAngles(const Eigen::Quaternionf& quaternion) {
    // (2, 1, 0) specifies the rotation sequence Z-Y-X (Yaw-Pitch-Roll), which is standard in aerospace and IMU systems
    Eigen::Vector3f eulerAnglesZYX = quaternion.toRotationMatrix().eulerAngles(2, 1, 0); 
    EulerAngles angles;

    angles.yaw = eulerAnglesZYX(0);
    angles.pitch = eulerAnglesZYX(1);
    angles.roll  = eulerAnglesZYX(2);

    return angles;
}

Eigen::Quaternionf eulerAnglesToQuaternion(const EulerAngles& eulerAngles) {
    // 1. Create rotation objects for each axis
    Eigen::AngleAxisf rollAngle(eulerAngles.roll, Eigen::Vector3f::UnitX());     // Rotation about X (Roll)
    Eigen::AngleAxisf pitchAngle(eulerAngles.pitch, Eigen::Vector3f::UnitY());   // Rotation about Y (Pitch)
    Eigen::AngleAxisf yawAngle(eulerAngles.yaw, Eigen::Vector3f::UnitZ());       // Rotation about Z (Yaw)

    // 2. Combine the rotations in Z-Y-X order (Yaw * Pitch * Roll). The multiplication order matters, The order below implements Z-Y-X.
    Eigen::Quaternionf quaternion = yawAngle * pitchAngle * rollAngle;

    // 3. Normalize is automatic upon creation, but can be called explicitly
    quaternion.normalize(); 

    return quaternion;
}