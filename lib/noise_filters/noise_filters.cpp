#include "noise_filters.hpp"

void NoiseFilters::push_input(float newInput) {
    if(inputBuffer.size() >= bufferMaxSize) {inputBuffer.pop_back();}
    inputBuffer.insert(inputBuffer.begin(), newInput);
}

void NoiseFilters::calculateMeanValue() {
    meanValue = inputBuffer[0];
    for(std::size_t i = 1; i < inputBuffer.size(); i++) {meanValue += inputBuffer[i];}
    int actualBufferSize = inputBuffer.size();
    if(actualBufferSize > bufferMaxSize) {actualBufferSize = bufferMaxSize;}
    meanValue /= inputBuffer.size();
}

void NoiseFilters::calculateStandardDeviation() {
    standardDeviation = std::pow(inputBuffer[0] - meanValue, 2);
    for(int i = 1; i < inputBuffer.size(); i++) {        
        standardDeviation += std::pow(inputBuffer[i] - meanValue, 2);
    }
    standardDeviation =  standardDeviation / (inputBuffer.size() - 1);
    standardDeviation = std::sqrt(standardDeviation);
}

void NoiseFilters::setBufferSize(int bufferSize) {bufferMaxSize = bufferSize;}

int NoiseFilters::getBufferSize() {return inputBuffer.size();}

float NoiseFilters::getStandardDeviation() {return standardDeviation;}

float NoiseFilters::updateMovingAverage(float newInput) {
    push_input(newInput);
    calculateMeanValue();
    calculateStandardDeviation();
    return meanValue;
}