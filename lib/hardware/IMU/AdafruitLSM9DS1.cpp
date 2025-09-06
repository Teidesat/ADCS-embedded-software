#include "AdafruitLSM9DS1.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>

void AdafruitLSM9DS1::begin(const int& SDAPin, const int& SCLPin) {
    // Set pins for the I2C connection
    Wire.begin(SDAPin, SCLPin);

    // Try to initialise and warn if we couldn't detect the chip
    if(!LSM9DS1.begin()) {
        Serial.println("Unable to initialize the LSM9DS1");
        while (1);
    }

    // set sensors range / sensitivity
    LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_2G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_4G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_8G);
    //LSM9DS1.setupAccel(LSM9DS1.LSM9DS1_ACCELRANGE_16G);
  
    LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_245DPS);
    //LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_500DPS);
    //LSM9DS1.setupGyro(LSM9DS1.LSM9DS1_GYROSCALE_2000DPS);

    LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_4GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_8GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_12GAUSS);
    //LSM9DS1.setupMag(LSM9DS1.LSM9DS1_MAGGAIN_16GAUSS);
}

void AdafruitLSM9DS1::update() {
    sensors_event_t accelerometers_event_t, magnetometers_event_t, gyroscopes_event_t, temperature_event_t;
    LSM9DS1.getEvent(&accelerometers_event_t, &magnetometers_event_t, &gyroscopes_event_t, &temperature_event_t);

    temperature = temperature_event_t.temperature;
  
    accelerometers.x = accelerometers_event_t.acceleration.x;
    accelerometers.y = accelerometers_event_t.acceleration.y;
    accelerometers.z = accelerometers_event_t.acceleration.z;

    gyroscopes.x = gyroscopes_event_t.gyro.x;
    gyroscopes.y = gyroscopes_event_t.gyro.y;
    gyroscopes.z = gyroscopes_event_t.gyro.z;

    magnetometers.x = magnetometers_event_t.magnetic.x;
    magnetometers.y = magnetometers_event_t.magnetic.y;
    magnetometers.z = magnetometers_event_t.magnetic.z;
}

float AdafruitLSM9DS1::getTemperature() {return temperature;}

String AdafruitLSM9DS1::floatToFormattedString(float number) {
    char sign = (number < 0) ? '-' : ' ';
    number = std::abs(number);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << number;
    String tmpStr = String(number, 2);

    int decimalPos = tmpStr.indexOf('.');
    String wholePart = tmpStr.substring(0, decimalPos);
    String decimalPart = tmpStr.substring(decimalPos + 1, 2);

    String formattedHolePart;
    if (wholePart.length() > 3) {formattedHolePart = "###";} 
    else {formattedHolePart = wholePart;}

    String raw_output = "";
    if (sign == '-') {raw_output += sign;}
    raw_output += formattedHolePart;
    raw_output += '.';
    raw_output += decimalPart;

    int spacesToAdd = 7 - raw_output.length();
    if (spacesToAdd < 0) spacesToAdd = 0;

    return String(spacesToAdd, ' ') + raw_output;
}

String AdafruitLSM9DS1::singleSensorToString(const SensorData& sensorData) {
    String xStr = floatToFormattedString(sensorData.x);
    String yStr = floatToFormattedString(sensorData.y);
    String zStr = floatToFormattedString(sensorData.z);

    return xStr + STRING_SEPARATOR + yStr + STRING_SEPARATOR + zStr;
}

String AdafruitLSM9DS1::accelerometersToString() {return singleSensorToString(accelerometers);}
String AdafruitLSM9DS1::gyroscopesToString() {return singleSensorToString(gyroscopes);}
String AdafruitLSM9DS1::magnetometersToString() {return singleSensorToString(magnetometers);}
String AdafruitLSM9DS1::temperatureToString() {return String(temperature);}

String AdafruitLSM9DS1::allSensorsToString() {
    return accelerometersToString() + STRING_SEPARATOR + gyroscopesToString() + STRING_SEPARATOR + magnetometersToString() + STRING_SEPARATOR + temperatureToString();
}

String AdafruitLSM9DS1::attitudeToString() {
    String pitchStr = floatToFormattedString(attitude.pitch);
    String rollStr = floatToFormattedString(attitude.roll);
    String yawhStr = floatToFormattedString(attitude.yaw);

    return pitchStr + STRING_SEPARATOR + rollStr + STRING_SEPARATOR + yawhStr;
}