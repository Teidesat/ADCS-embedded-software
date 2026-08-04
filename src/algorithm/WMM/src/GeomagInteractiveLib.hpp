#pragma once

#include "GeomagnetismLibrary.hpp"

int MAG_ValidateDMSstring(char *input, int min, int max, char *Error);
int MAG_GetAltitude(char* Query_String, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic* coords, int bounds[2], int AltitudeSetting);