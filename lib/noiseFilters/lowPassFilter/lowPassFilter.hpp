#pragma once

class LowPassFilter {
    private:
        float previousValue;
        float trustInNewData;
        float trustInOldData;

    public:
        LowPassFilter(const float trust, const float value = 0.0);
        void setTrustInNewData(const float trust);
        float getTrustInNewData();
        float getTrustInOldNewData();
        float getValue();
        void update(const float value);
};