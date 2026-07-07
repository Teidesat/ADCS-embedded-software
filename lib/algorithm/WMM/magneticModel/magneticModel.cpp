
#include "MagneticModel.hpp"

#include <sstream>
#include <array>

Coefficients::Coefficients(const float mainFieldCoeffG, const float mainFieldCoeffH, const float secularVarCoeffG, const float secularVarCoeffH): 
    mainFieldCoeffG(mainFieldCoeffG), 
    mainFieldCoeffH(mainFieldCoeffH),
    secularVarCoeffG(secularVarCoeffG), 
    secularVarCoeffH(secularVarCoeffH) 
{}

void MagneticModel::setup(const std::string& coefifcientsFileContent) {
    if(!coefifcientsFileContent.empty()) {
        std::stringstream coefifcientsFileStringStream(coefifcientsFileContent);
        std::string coefifcientsFileCurrentLine;

        if(std::getline(coefifcientsFileStringStream, coefifcientsFileCurrentLine)) {
            std::stringstream header_ss(coefifcientsFileCurrentLine);
            header_ss >> epoch >> modelName >> publishDate;
        }
        
        minYear = dateToYears(publishDate);
        if(minYear == -1) {minYear = epoch;} 

        mainFieldCoefficientsG.push_back(0.0);
        mainFieldCoefficientsH.push_back(0.0);
        secularVarCoefficientsG.push_back(0.0);
        secularVarCoefficientsH.push_back(0.0);
        
        int indexColumn1 = 0;
        int indexColumn2 = 0;
        float mainFieldCoefficientG = 0.0f;
        float mainFieldCoefficientH = 0.0f;
        float secularVarCoefficientG = 0.0f;
        float secularVarCoefficientH = 0.0f;

        while(std::getline(coefifcientsFileStringStream, coefifcientsFileCurrentLine)) {
            if(coefifcientsFileCurrentLine.empty()) {continue;}

            std::stringstream lineData(coefifcientsFileCurrentLine);
            if(lineData >> indexColumn1 >> indexColumn2 >> mainFieldCoefficientG >> mainFieldCoefficientH >> secularVarCoefficientG >> secularVarCoefficientH) {
                mainFieldCoefficientsG.push_back(mainFieldCoefficientG);
                mainFieldCoefficientsH.push_back(mainFieldCoefficientH);
                secularVarCoefficientsG.push_back(secularVarCoefficientG);
                secularVarCoefficientsH.push_back(secularVarCoefficientH);
                nMax = indexColumn1;
                nMaxSecVar = indexColumn1;
            }
        }

        coefficientFileEndDate = epoch + 5.0;
    }
}

Coefficients MagneticModel::getCoefficients(const int n, const int m) const {
    const int index = (n * (n + 1) / 2) + m;
    return Coefficients(mainFieldCoefficientsG[index], mainFieldCoefficientsH[index], secularVarCoefficientsG[index], secularVarCoefficientsH[index]);
}

double MagneticModel::dateToYears(const std::string& date) const {
    double yearWihDecimal = 0.0;
    int day = 0; 
    int month = 0;
    int year = 0;
    char slash1 = ' ';
    char slash2 = ' ';
    std::stringstream ss(date);

    if(ss >> month >> slash1 >> day >> slash2 >> year && slash1 == '/' && slash2 == '/') {
        double extraDay = 0;
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {extraDay = 1;}

        std::array<int, 13> daysInAMonth = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        const double daysInAYear = 365.0;
        const double daysInAYearLeapYearAdjust = daysInAYear + extraDay;
        daysInAMonth[2] += extraDay;

        int totalDays = 0;
        for(int i = 0; i < month; i++) {totalDays += daysInAMonth[i];}
        totalDays += day;
        yearWihDecimal = (double) year + (double)(totalDays - 1) / daysInAYearLeapYearAdjust;
    }

    return yearWihDecimal;
}
