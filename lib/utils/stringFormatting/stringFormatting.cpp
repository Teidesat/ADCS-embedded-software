#include "stringFormatting.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>
/*
std::string floatToFormattedString(const float number) {
    char sign = (number < 0) ? '-' : ' ';
    number = std::abs(number);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << number;
    std::string tmpStr = std::string(number, 2);

    int decimalPos = tmpStr.indexOf('.');
    std::string wholePart = tmpStr.substring(0, decimalPos);
    std::string decimalPart = tmpStr.substring(decimalPos + 1, 2);

    std::string formattedHolePart;
    if (wholePart.length() > 3) {formattedHolePart = "###";} 
    else {formattedHolePart = wholePart;}

    std::string raw_output = "";
    if (sign == '-') {raw_output += sign;}
    raw_output += formattedHolePart;
    raw_output += '.';
    raw_output += decimalPart;

    int spacesToAdd = 7 - raw_output.length();
    if (spacesToAdd < 0) spacesToAdd = 0;

    return std::string(spacesToAdd, ' ') + raw_output.c_str();
}
*/