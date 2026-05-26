#include "movingAverage.hpp"

#include <math.h>

MovingAverage::MovingAverage(const int bufferSize):
    meanValue(0.0),
    standardDeviation(0.0)
{
    setBufferSize(bufferSize);
}

void MovingAverage::addValue(const float value) {
    if(inputBuffer.size() >= bufferMaxSize) {inputBuffer.pop_back();}
    inputBuffer.insert(inputBuffer.begin(), value);
}

void MovingAverage::calculateMeanValue() {
    meanValue = inputBuffer[0];
    for(std::size_t i = 1; i < inputBuffer.size(); i++) {meanValue += inputBuffer[i];}
    int actualBufferSize = inputBuffer.size();
    if(actualBufferSize > bufferMaxSize) {actualBufferSize = bufferMaxSize;}
    meanValue /= inputBuffer.size();
}

void MovingAverage::calculateStandardDeviation() {
    standardDeviation = std::pow(inputBuffer[0] - meanValue, 2);
    for(int i = 1; i < inputBuffer.size(); i++) {standardDeviation += std::pow(inputBuffer[i] - meanValue, 2);}
    standardDeviation =  standardDeviation / (inputBuffer.size() - 1);
    standardDeviation = std::sqrt(standardDeviation);
}

void MovingAverage::setBufferSize(const int bufferSize) {bufferMaxSize = bufferSize;}

int MovingAverage::getBufferSize() {return inputBuffer.size();}

float MovingAverage::getStandardDeviation() {return standardDeviation;}

float MovingAverage::update(const float value) {
    addValue(value);
    calculateMeanValue();
    calculateStandardDeviation();

    return meanValue;
}