// acubesat

#include "../lib/hardware/INS/INS.hpp"
//#include "../lib/IMUalgorithms/acubesat/acubesat.hpp"

INS ins;

void setup() {
  ins.begin();
  //ins.gnss.shortInfo();
}

void loop() {
  ins.update();
  //Serial.println(ins.gnss.getLastNMEAmessage().c_str());
}