#pragma once

#include "magneticModel/magneticModel.hpp"

#include <string>

std::string readCoefficientsFile(const std::string& coefficientsFilename);
MagneticModel setupWMM();