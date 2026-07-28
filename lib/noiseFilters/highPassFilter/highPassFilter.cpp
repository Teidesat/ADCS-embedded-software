#include "highPassFilter.hpp"

HighPassFilter::HighPassFilter(const float trust, const float value):
    previousValue(value)
{
    setTrustInNewData(trust);
}

void HighPassFilter::setTrustInNewData(const float trust) {
    trustInNewData = trust;
    trustInOldData = 1.0 - trustInNewData;
}

float HighPassFilter::getTrustInNewData() {return trustInNewData;}
float HighPassFilter::getTrustInOldNewData() {return trustInOldData;}

float HighPassFilter::getValue() {return previousValue;}

void HighPassFilter::update(const float value) {previousValue = trustInNewData * value + trustInOldData * previousValue;}