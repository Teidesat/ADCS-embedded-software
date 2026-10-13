#pragma once

class Quaternion;

class EulerAngles {
    private:
        float pitch = 0.0;
        float roll = 0.0;
        float yaw = 0.0;

    public:
        static constexpr double pi = 3.14159265358979323846;  // good value for pi?

        EulerAngles(const float pitchInput, const float rollInput, const float yawInput);
        
        float getPitch() const;
        float getRoll() const;  
        float getYaw() const;
        
        static float radiansToDegrees(const float radians);
        EulerAngles radiansToDegrees() const;
        static float degreesToRadians(const float degrees);
        EulerAngles degreesToRadians() const;

        Quaternion toQuaternion() const;
};