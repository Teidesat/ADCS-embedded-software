/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <string>


#include "../src/magcalc.hpp"
#include "../src/GeomagnetismLibrary.hpp"
#include "../src/GeomagInteractiveLib.hpp"
/*---------------------------------------------------------------------------*/

/* 
WMM Point Calculation Program.

The Geomagnetism Library is used to make a command prompt program. The program prompts
the user to enter a location, performs the computations and prints the results to the
standard output.
 */


// BOZ: blackoutzone
const char* BOZ_WARN_TEXT_STRONG = "Warning: some calculated locations are "
                                   "in the blackout zone "
                                   "around the magnetic pole\nas defined by "
                                   "the WMM military specification \n(https://"
                                   "www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-"
                                   "89500B.pdf).\nCompass accuracy is highly "
                                   "degraded in this region.\n";
const char* BOZ_WARN_TEXT_WEAK = "Caution: some calculated locations approach "
                                "the blackout zone around the magnetic\npole "
                                "as defined by the WMM military specification "
                                "\n(https://www.ngdc.noaa.gov/geomag/WMM/data/"
                                "MIL-PRF-89500B.pdf).\n Compass accuracy may "
                                "be degraded in this region.\n";

const char* WMM_MileSpec_INFO = "Warning: The height validity of the geomagnetic components is dependent on the geomagnetic activity level. For more information see \n(https://www.ncei.noaa.gov/products/world-magnetic-model/accuracy-limitations-error-model)\n";
const char* WMM_MileSpec_WARN = "Warning: WMM will not meet MilSpec at this altitude. For more information see \n(https://www.ncei.noaa.gov/products/world-magnetic-model/accuracy-limitations-error-model)\n";

int main() {
    MAGtype_MagneticModel * MagneticModels, *TimedMagneticModel;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_Geoid Geoid;
    int print_boz_warning_strong = 0;
    int print_boz_warning_weak = 0;



    
    point_calc(Ellip, CoordGeodetic, &CoordSpherical, UserDate, MagneticModels, TimedMagneticModel, &GeoMagneticElements, &Errors);
    
    if(GeoMagneticElements.H <= 2000.0) {print_boz_warning_strong = 1;} 
    else if(GeoMagneticElements.H <= 6000.0) {print_boz_warning_weak = 1;}
    
    if(CoordGeodetic.HeightAboveEllipsoid < -1 || CoordGeodetic.HeightAboveEllipsoid > 1900) {printf("%s\n", WMM_MileSpec_WARN);}
    else {printf("%s\n",WMM_MileSpec_INFO);} 
        
    MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Print the results */

    if(print_boz_warning_strong) {printf("\n %s\n", BOZ_WARN_TEXT_STRONG);} 
    else if (print_boz_warning_weak) {printf("\n %s\n", BOZ_WARN_TEXT_WEAK);}


    return 0;
}