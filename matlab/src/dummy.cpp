#include "dummy.hpp"

Dummy::Dummy(float newMinValue = 0, float newMaxValue = 0) {
    minValue = newMinValue;
    maxValue = newMaxValue;
}

float Dummy::clampValue(float valueToClamp) {
    if(valueToClamp < minValue) {return minValue;}
    else if(valueToClamp > maxValue) {return maxValue;}
    else {return valueToClamp;}
}