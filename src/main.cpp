// teidesat

#include "../lib/hardware/INS/INS.hpp"
#include "../lib/algorithms/teidesat/teidesat.hpp"

INS ins;
Teidesat teidesatAlgorithm;

void setup() {
    // I2C Pins (IMU and sun sensor)
    const int SDA_PIN = 26;
    const int SCL_PIN = 27;
    // UART Pins (GNSS)
    const int RX_PIN = 16;
    const int TX_PIN = 17;

    ins.begin(SDA_PIN, SCL_PIN, RX_PIN, TX_PIN); // set all sensors up
}

void loop() {
    ins.update();
    teidesatAlgorithm.update(ins.getIMUdata().accelerometers, ins.getIMUdata().gyroscopes, ins.getSunVector3d().lux, ins.getMicrocontrollerData().deltaTime);
    
    Serial.println(teidesatAlgorithm.attitudeToString().c_str());
}