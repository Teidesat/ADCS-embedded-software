#include <vector>
#include <math.h>

class NoiseFilters {
    private:
        std::vector<float> inputBuffer;
        int bufferMaxSize;
        float meanValue;
        float standardDeviation;
        void push_input(float newInput);
        void calculateMeanValue();
        void calculateStandardDeviation();
        
    public:
        void setBufferSize(int bufferSize);
        int getBufferSize();
        float getStandardDeviation();
        float updateMovingAverage(float newInput);
};