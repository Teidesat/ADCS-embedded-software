#pragma once

#include "../constants/coefficients.hpp"

#include <array>
#include <string>

class MagneticModel {
    public:
        int nMax = 0; /* Maximum degree of spherical harmonic model */
        int nMaxSecVar = 0; /* Maximum degree of spherical harmonic secular model */
        int secularVariation = 0; // Whether or not the magnetic secular variation vector will be needed by program
        float epoch = 0.0;
        float coefficientsEndDate = 0.0;
        std::array<float, WMM::Coefficients::numberOfCoefficients> mainFieldCoefficientsG = {}; /* C - Gauss coefficients of main geomagnetic model (nT) Index is (n * (n + 1) / 2 + m) */
        std::array<float, WMM::Coefficients::numberOfCoefficients> mainFieldCoefficientsH = {}; /* C - Gauss coefficients of main geomagnetic model (nT) */
        std::array<float, WMM::Coefficients::numberOfCoefficients> secularVarCoefficientsG = {}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
        std::array<float, WMM::Coefficients::numberOfCoefficients> secularVarCoefficientsH = {}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */

        MagneticModel();
        MagneticModel(const int nMax, 
                      const float epoch, 
                      const std::array<float, WMM::Coefficients::numberOfCoefficients>& mainFieldCoefficientsG, 
                      const std::array<float, WMM::Coefficients::numberOfCoefficients>& mainFieldCoefficientsH, 
                      const std::array<float, WMM::Coefficients::numberOfCoefficients>& secularVarCoefficientsG, 
                      const std::array<float, WMM::Coefficients::numberOfCoefficients>& secularVarCoefficientsH);
        
        static int calculateIndex(const int column, const int row);
};