#pragma once

#include <vector>

class MovingAverage {
    private:
        std::vector<float> inputBuffer;
        int bufferMaxSize;
        float meanValue;
        float standardDeviation;

        void pushValue(const float value);
        void calculateMeanValue();
        void calculateStandardDeviation();
        
    public:
        void setBufferSize(const int bufferSize);
        int getBufferSize();
        float getStandardDeviation();
        float update(const float value);
};


float clamp(const float value, const float threshold);