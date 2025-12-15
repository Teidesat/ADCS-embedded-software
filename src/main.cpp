// teidesat

#include "../lib/hardware/INS/INS.hpp"
#include "../lib/IMUalgorithms/teidesat/teidesat.hpp"

INS ins;
Teidesat teidesatAlgorithm;

void setup() {
    ins.begin(); // set all sensors up
}

void loop() {
    ins.update();
    teidesatAlgorithm.update(ins.imu.accelerometers, ins.imu.gyroscopes, ins.sunSensor.lux, ins.microcontroller.getDeltaTime());
    
    Serial.println(teidesatAlgorithm.attitudeToString().c_str());
}