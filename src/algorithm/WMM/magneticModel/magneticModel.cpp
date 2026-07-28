
#include "magneticModel.hpp"


MagneticModel::MagneticModel() {}

MagneticModel::MagneticModel(const int nMax, 
                             const float epoch, 
                             const std::array<float, WMM::Coefficients::numberOfCoefficients>& mainFieldCoefficientsG, 
                             const std::array<float, WMM::Coefficients::numberOfCoefficients>& mainFieldCoefficientsH, 
                             const std::array<float, WMM::Coefficients::numberOfCoefficients>& secularVarCoefficientsG, 
                             const std::array<float, WMM::Coefficients::numberOfCoefficients>& secularVarCoefficientsH) {
    nMax = nMax;
    nMaxSecVar = nMax;
    epoch = epoch;

    mainFieldCoefficientsG = mainFieldCoefficientsG;
    mainFieldCoefficientsH = mainFieldCoefficientsH;
    secularVarCoefficientsG = secularVarCoefficientsG;
    secularVarCoefficientsH = secularVarCoefficientsH;
    coefficientsEndDate = epoch + 5.0;
}

int MagneticModel::calculateIndex(const int column, const int row) {
    const int index = (column * (column + 1) / 2) + row;
    return index;
}