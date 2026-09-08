#pragma once

class Quaternion;

class EulerAngles {
    private:
        float roll = 0; // corresponds to x axis of rotation
        float pitch = 0; // corresponds to y axis of rotation
        float yaw = 0; // corresponds to z axis of rotation

    public:
        EulerAngles(const float newPitch = 0.0, const float newRoll = 0.0, const float newYaw = 0.0);
        
        float getRoll() const;  
        float getPitch() const;
        float getYaw() const;
        
        static float radiansToDegrees(const float radians);
        EulerAngles radiansToDegrees() const;
        static float degreesToRadians(const float degrees);
        EulerAngles degreesToRadians() const;

        Quaternion toQuaternion() const;
};