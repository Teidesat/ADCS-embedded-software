#pragma once

#include "GeomagnetismLibrary.hpp"

void point_calc(MAGtype_Ellipsoid Ellip, MAGtype_CoordGeodetic CoordGeodetic, MAGtype_CoordSpherical* CoordSpherical,
                MAGtype_Date UserDate, MAGtype_MagneticModel* MagneticModel, MAGtype_MagneticModel* TimedMagneticModel,
                MAGtype_GeoMagneticElements* GeoMagneticElements, MAGtype_GeoMagneticElements* Errors);

MAGtype_MagneticModel* allocate_coefsArr_memory(int nMax, MAGtype_MagneticModel* MagneticModel);