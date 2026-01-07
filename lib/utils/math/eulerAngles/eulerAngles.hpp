#pragma once

#include <string>

class Quaternion;

class EulerAngles {
    private:
        float roll; // corresponds to x axis of rotation
        float pitch; // corresponds to y axis of rotation
        float yaw; // corresponds to z axis of rotation

    public:
        EulerAngles(const float iPitch = 0.0f, const float iRoll = 0.0f, const float iYaw = 0.0f);
        
        float getRoll() const;  
        float getPitch() const;
        float getYaw() const;
        
        static float toDegrees(float component);
        static EulerAngles toDegrees(EulerAngles attitude);
        static float toRadians(float component);
        static EulerAngles toRadians(EulerAngles attitude);

        Quaternion toQuaternion() const;

        std::string shortInfo() const;
};