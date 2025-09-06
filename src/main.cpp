// main-adafruit-fork.txt

#include "../lib/hardware/INS/INS.hpp"
#include "../lib/IMUalgorithms/adafruitFork/MadgwickAHRS.hpp"

INS ins;
Madgwick attitude;

void setup() {
  ins.begin();
  const int UPDATE_RATE = 10;
  attitude.begin(UPDATE_RATE);
}

void loop() {  
  ins.update();
  Serial.println(ins.gnss.getLastNMEAmessage().c_str());

  //std::string a = "number of satellites: " + ins.gnss.NMEAparser.satellites.value(); 
  //Serial.println(a.c_str());
  //attitude.updateIMU(ins.imu.gyroscopes.x, ins.imu.gyroscopes.y, ins.imu.gyroscopes.z, ins.imu.accelerometers.x, ins.imu.gyroscopes.y, ins.imu.accelerometers.z);
  //ins.imu.attitude.pitch = attitude.getPitch();
  //ins.imu.attitude.roll = attitude.getRoll();
  //ins.imu.attitude.yaw = attitude.getYaw();

  //Serial.println(ins.imu.attitudeToString());
  //delay(100.0f);
}