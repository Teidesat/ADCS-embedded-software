#include "WMM.hpp"

#include <vector>
#include <fstream>

std::string readCoefficientsFile(const std::string& coefficientsFilename) {
    
    std::ifstream coefficientsFile(coefficientsFilename, std::ios::in | std::ios::binary | std::ios::ate);
    std::string coefficientsFileContent = "";
    
    if (coefficientsFile.is_open()) {
        const std::streamsize coefficientsFileSize = coefficientsFile.tellg();
        coefficientsFile.seekg(0, std::ios::beg);

        // Allocate memory and read the whole file in one go
        coefficientsFileContent.resize(coefficientsFileSize);
        coefficientsFile.read(&coefficientsFileContent[0], coefficientsFileSize);
        
        coefficientsFile.close(); 
    }

    return coefficientsFileContent;
}

MagneticModel setup() {
    const std::string coefficientsFilename = "../data/WMM.COF";
    std::string coefficientsFileContent = readCoefficientsFile(coefficientsFilename);

    MagneticModel magneticModel;
    if (!coefficientsFileContent.empty()) {
        magneticModel.setup(coefficientsFileContent);
        
        MagneticModel TimedMagneticModel;
    }

    return magneticModel;
}