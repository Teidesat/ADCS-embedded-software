#pragma once

#include <vector>

class Coefficients {
    public:
        float mainFieldCoeffG = 0.0;
        float mainFieldCoeffH = 0.0;
        float secularVarCoeffG = 0.0;
        float secularVarCoeffH = 0.0;
        Coefficients(const float mainFieldCoeffG, const float mainFieldCoeffH, const float secularVarCoeffG, const float secularVarCoeffH);
};

class MagneticModel {
    public:
        //double edit//ionDate = 0.0;
        int nMax = 12; /* Maximum degree of spherical harmonic model */
        int nMaxSecVar = 0; /* Maximum degree of spherical harmonic secular model */
        int secularVariation = 0; // Whether or not the magnetic secular variation vector will be needed by program
        float epoch = 0.0;
        float coefficientFileEndDate = 0.0;
        float minYear = 0.0;
        std::string modelName = "";
        std::string publishDate = "";
        std::vector<float> mainFieldCoefficientsG = {0.0}; /* C - Gauss coefficients of main geomagnetic model (nT) Index is (n * (n + 1) / 2 + m) */
        std::vector<float> mainFieldCoefficientsH  = {0.0}; /* C - Gauss coefficients of main geomagnetic model (nT) */
        std::vector<float> secularVarCoefficientsG  = {0.0}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
        std::vector<float> secularVarCoefficientsH  = {0.0}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */

        void setup(const std::string& coefifcientsFileContent);
        Coefficients getCoefficients(const int n, const int m) const;

    private:
        double dateToYears(const std::string& date) const;
};