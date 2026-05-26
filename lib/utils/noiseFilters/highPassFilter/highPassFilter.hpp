#pragma once

class HighPassFilter {
    private:
        float previousValue;
        float trustInNewData;
        float trustInOldData;

    public:
        HighPassFilter(const float trust, const float value = 0.0);
        void setTrustInNewData(const float trust);
        float getTrustInNewData();
        float getTrustInOldNewData();
        float getValue();
        void update(const float value);
};