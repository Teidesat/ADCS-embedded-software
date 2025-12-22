//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#include <cmath>

//---------------------------------------------------------------------------------------------------
// Function declarations

class XioMadgwick {
    private:
        float invSqrt(float x);
        
    public:
        float pitch;
        float roll;
        float yaw;
        void quaternionToEuler();
        float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	// quaternion of sensor frame relative to auxiliary frame
        void madgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
        void madgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
};

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
