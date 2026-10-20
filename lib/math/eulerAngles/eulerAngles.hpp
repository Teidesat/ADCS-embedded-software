#pragma once

namespace math {

class Quaternion;

class EulerAngles { // only store angles in radians, because of the trigonometric functions
    public:
        double roll = 0.0;
        double pitch = 0.0;
        double yaw = 0.0;

        EulerAngles(const double rollValue, const double pitchValue, const double yawValue);
        
        static double radiansToDegrees(const double radians);
        static double degreesToRadians(const double degrees);
        Quaternion toQuaternion() const;
};

}