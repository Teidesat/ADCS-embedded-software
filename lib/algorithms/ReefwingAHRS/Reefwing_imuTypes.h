/******************************************************************
  @file       Reefwing_imuTypes.h
  @brief      Common structs, enums and classes for the Reefwing IMU 
              Libraries
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     2.0.3
  Date:        02/08/23

  1.0.0     Original Release.               19/04/23
  1.0.1     Minor documentation changes.    24/04/23
  2.0.0     Modified Quaternion class.      27/05/23
  2.0.1     Added I2C addresses             09/06/23
  2.0.2     Added xiao Sense support        17/06/23
  2.0.3     Vector Data Type added          02/08/23

  There are two conventions for quaternions, Hamilton and JPL. 
  The difference between the two conventions is the relation 
  between the three imaginary bases. In the Hamilton convention, 
  ijk = −1, while JPL defines ijk = 1. As consequences, the 
  multiplication of quaternions and the transformation between 
  quaternions and other rotation parameterizations differ with 
  the quaternion convention used.

  We use the Hamilton Convention.

  Credit - Uses a modified version of the Madgwick Quaternion Class.
           (http://www.x-io.co.uk/quaternions/)

******************************************************************/

#ifndef Reefwing_imuTypes_h
#define Reefwing_imuTypes_h

/******************************************************************
 *
 * Structs - 
 * 
 ******************************************************************/

struct EulerAngles {
  float roll;     /* rotation around x axis in degrees */
  float pitch;    /* rotation around y axis in degrees */
  float yaw;      /* rotation around z axis in degrees */
  float heading;  /* rotation relative to magnetic north */
  float rollRadians, pitchRadians, yawRadians;
  int  timeStamp;
};

struct InertialMessage {
  float ax, ay, az;
  float gx, gy, gz;
  int timeStamp;
};

struct RawData {
  short int rx, ry, rz;
  int  timeStamp;
};

struct ScaledData {
  float sx, sy, sz;
  int  timeStamp;
};

struct TempData {
  float celsius;
  int  timeStamp;
};

struct IMUSensors {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  int gTimeStamp, aTimeStamp, mTimeStamp;
};

struct VectorData {
  float x, y, z;
};

/******************************************************************
 *
 * Quaternion Class Definition - 
 * 
 ******************************************************************/

class Quaternion {
  public:
      Quaternion();
      Quaternion(float w, float x, float y, float z);
      Quaternion(float yaw, float pitch, float roll);

      Quaternion getConjugate(void);
      EulerAngles getEulerAngles();
      EulerAngles toEulerAngles(float declination = 0.0);

      void reset();

      float q0, q1, q2, q3;      //  Euler Parameters
      int  timeStamp;

    private:
      float radiansToDegrees(float radians);

      float eInt[3] = {0.0f, 0.0f, 0.0f};       //  Vector to hold integral error for Mahony filter
      float att[4] = {1.0f, 0.0f, 0.0f, 0.0f};  //  Attitude quaternion for complementary filter
};

#endif