#include "clamp.hpp"

float clamp(float value, const float lowerThreshold, const float upperThreshold) {
    if(value > upperThreshold) {value = upperThreshold;}
    else if (value < lowerThreshold) {value = lowerThreshold;}
    return value;
}

float deadband(float value, const float lowerThreshold, const float upperThreshold) {
    const float midThreshold = (upperThreshold + lowerThreshold) / 2.0;
    if(value < upperThreshold || value < lowerThreshold) {value = midThreshold;}
    return value;
}