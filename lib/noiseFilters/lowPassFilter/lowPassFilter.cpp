#include "lowPassFilter.hpp"

LowPassFilter::LowPassFilter(const float trust, const float value):
    previousValue(value)
{
    setTrustInNewData(trust);
}

void LowPassFilter::setTrustInNewData(const float trust) {
    trustInNewData = trust;
    trustInOldData = 1.0 - trustInNewData;
}

float LowPassFilter::getTrustInNewData() {return trustInNewData;}
float LowPassFilter::getTrustInOldNewData() {return trustInOldData;}

float LowPassFilter::getValue() {return previousValue;}

void LowPassFilter::update(const float value) {previousValue = trustInNewData * value + trustInOldData * previousValue;}