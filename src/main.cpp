// main-adafruit-fork.txt

#include "../lib/hardware/INS/INS.hpp"
#include "../lib/IMUalgorithms/adafruitFork/MadgwickAHRS.hpp"

INS ins;
Madgwick attitude;

void setup() {
  const int RATE = 9;
  ins.setup();
  attitude.begin(RATE);
}

void loop() {  
  ins.update();
  
  attitude.updateIMU(ins.imu.gyroscopes.x, ins.imu.gyroscopes.y, ins.imu.gyroscopes.z, ins.imu.accelerometers.x, ins.imu.gyroscopes.y, ins.imu.accelerometers.z);
  ins.imu.attitude.pitch = attitude.getPitch();
  ins.imu.attitude.roll = attitude.getRoll();
  ins.imu.attitude.yaw = attitude.getYaw();

  Serial.println(ins.imu.attitudeToString());
}