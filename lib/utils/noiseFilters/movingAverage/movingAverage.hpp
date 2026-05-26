#pragma once

#include <vector>

class MovingAverage {
    private:
        std::vector<float> inputBuffer;
        int bufferMaxSize;
        float meanValue;
        float standardDeviation;

        void addValue(const float value);
        void calculateMeanValue();
        void calculateStandardDeviation();
        
    public:
        MovingAverage(const int bufferSize = 1);
        void setBufferSize(const int bufferSize);
        int getBufferSize();
        float getStandardDeviation();
        float update(const float value);
};