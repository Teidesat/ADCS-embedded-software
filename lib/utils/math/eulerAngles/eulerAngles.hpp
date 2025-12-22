#pragma once

#include "../quaternion/quaternion.hpp"

class EulerAngles {
    private:
        // by default pitch, roll and yaw should be in radians
        float pitch; 
        float roll;
        float yaw;

    public:
        EulerAngles(const float iPitch = 0.0f, const float iRoll = 0.0f, const float iYaw = 0.0f);
        float getPitch() const;
        float getRoll() const;  
        float getYaw() const;
        // implementing different methods for conversion
        Quaternion toQuaternionTrigonometric() const;
        Quaternion toQuaternionHamilton() const; // it is suppose to be the best for space applications
        Quaternion toQuaternionMatrix() const;
        Quaternion toQuaternion() const;

        EulerAngles radiansToDegrees() const;
};