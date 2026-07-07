#pragma once

#include "GeomagnetismLibrary.hpp"

int MAG_ValidateDMSstring(char *input, int min, int max, char *Error);
void MAG_GetDeg(char* Query_String, double* latitude, double bounds[2]);
int MAG_GetAltitude(char* Query_String, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic* coords, int bounds[2], int AltitudeSetting);

int MAG_GetUserInput(MAGtype_MagneticModel *MagneticModel, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic *CoordGeodetic, MAGtype_Date *MagneticDate);

void MAG_clear_input_buffer();
