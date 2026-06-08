/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "../src/EGM9615.hpp"
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

int main()
{
    MAGtype_MagneticModel * MagneticModels[1], *TimedMagneticModel;
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_Geoid Geoid;
    char ans[20], b;
    int print_boz_warning_strong = FALSE;
    int print_boz_warning_weak = FALSE;
    char filename[] = "WMM.COF";
    char program_name[] = "wmm_point";
    int NumTerms, Flag = 1, nMax = 0;
    int epochs = 1;
    /* Memory allocation */

    if(!MAG_robustReadMagModels(filename, &MagneticModels, epochs)) {
        printf("\n %s not found.  Press enter to exit... \n ", filename);
        fgets(ans, 20, stdin);
        return 1;
    }

    NumTerms = ((nMax + 1) * (nMax + 2) / 2); // this is 1 * 2 / 2 = 1. Inside MAG_AllocateModelMemory the value is calculated again using 

    TimedMagneticModel = MAG_AllocateModelMemory(NumTerms); /* For storing the time modified WMM Model parameters */


    if(MagneticModels[0] == NULL || TimedMagneticModel == NULL){MAG_Error(2);}

    TimedMagneticModel = allocate_coefsArr_memory(nMax, MagneticModels[0]); // inside the function numTerms is calculated again instead of using the one in line 64, depending on a condition

    MAG_SetDefaults(&Ellip, &Geoid); /* Set default values and constants */
    /* Check for Geographic Poles */



    /* Set EGM96 Geoid parameters */
    Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
    Geoid.Geoid_Initialized = 1;
    /* Set EGM96 Geoid parameters END */
    while(Flag == 1)
    {
        if(MAG_GetUserInput(MagneticModels[0], &Geoid, &CoordGeodetic, &UserDate) == 1) /*Get User Input */
        {

            MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
            point_calc(Ellip, CoordGeodetic, &CoordSpherical,
                            UserDate, MagneticModels[0], TimedMagneticModel,
                            &GeoMagneticElements, &Errors);
           if(GeoMagneticElements.H <= 2000.0) {
                print_boz_warning_strong = TRUE;
            } else if (GeoMagneticElements.H <= 6000.0) {
                print_boz_warning_weak = TRUE;
            }
            if (CoordGeodetic.HeightAboveEllipsoid < -1 || CoordGeodetic.HeightAboveEllipsoid > 1900){
                printf("%s\n", WMM_MileSpec_WARN);
            }
            else{
                printf("%s\n",WMM_MileSpec_INFO);
            } 
            MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Print the results */
        }
       if(print_boz_warning_strong){ 
            printf("\n %s\n", BOZ_WARN_TEXT_STRONG);
        } else if (print_boz_warning_weak) {
            printf("\n %s\n", BOZ_WARN_TEXT_WEAK);
        }

        do {
            printf("\n\n Do you need more point data ? (y or n) \n ");
        } while (NULL == fgets(ans, 20, stdin));
        switch(ans[0]) {
            case 'Y':
            case 'y':
                Flag = 1;
                break;
            case 'N':
            case 'n':
                Flag = 0;
                break;
            default:
                Flag = 0;
                break;
        }

    }


    MAG_FreeMagneticModelMemory(TimedMagneticModel);
    MAG_FreeMagneticModelMemory(MagneticModels[0]);

    return 0;
}