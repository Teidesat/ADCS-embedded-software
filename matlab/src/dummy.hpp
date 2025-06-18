#ifndef DUMMY_H
#define DUMMY_H

class Dummy {
    private:
        float minValue, maxValue;

    public:
    Dummy(float newMinValue = 0, float newMaxValue = 0) {
        minValue = newMinValue;
        maxValue = newMaxValue;
    }
    float clampValue(float valueToClamp) {
        if(valueToClamp < minValue) {return minValue;}
        else if(valueToClamp > maxValue) {return maxValue;}
        else {return valueToClamp;}
    }
};

#endif