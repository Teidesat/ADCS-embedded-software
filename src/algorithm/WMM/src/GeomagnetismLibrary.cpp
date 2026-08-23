#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "GeomagnetismLibrary.hpp"

int MAG_Warnings(int control, double value, MAGtype_MagneticModel *MagneticModel)

/*Return value 0 means end program, Return value 1 means get new data, Return value 2 means continue.
  This prints a warning to the screen determined by the control integer. It also takes the value of the parameter causing the warning as a double.  This is unnecessary for some warnings.
  It requires the MagneticModel to determine the current epoch.

 */
{
    std::array<char, 20> ans = "";
    
    switch(control) {
        case 1:/* Horizontal Field strength low */
            do {
                printf("\nCaution: location is approaching the blackout zone around the magnetic pole as\n");
                printf("      defined by the WMM military specification \n");
                printf("      (https://www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-89500B.pdf). Compass\n");
                printf("      accuracy may be degraded in this region.\n");
                printf("Press enter to continue...\n");
            } while(NULL == fgets(ans, sizeof(ans), stdin)); 
            break;
        case 2:/* Horizontal Field strength very low */
            do {
                printf("\nWarning: location is in the blackout zone around the magnetic pole as defined\n");
                printf("      by the WMM military specification \n");
                printf("      (https://www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-89500B.pdf). Compass\n");
                printf("      accuracy is highly degraded in this region.\n");
            } while(NULL == fgets(ans, sizeof(ans), stdin));
            break;
        case 3:/* Elevation outside the recommended range */
            printf("\nWarning: The value you have entered of %.1f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
            while(1)
            {
                printf("\nPlease press 'C' to continue, 'G' to get new data or 'X' to exit...\n");
                while( NULL == fgets(ans, sizeof(ans), stdin)) {
                    printf("\nInvalid input\n");
                }
                switch(ans[0]) {
                    case 'X':
                    case 'x':
                        return 0;
                    case 'G':
                    case 'g':
                        return 1;
                    case 'C':
                    case 'c':
                        return 2;
                    default:
                        printf("\nInvalid input %c\n", ans[0]);
                        break;
                }
            }
            break;

        case 4:/*Date outside the recommended range*/
            printf("\nWARNING - TIME EXTENDS BEYOND INTENDED USAGE RANGE\n CONTACT NCEI FOR PRODUCT UPDATES:\n");
            printf("	National Centers for Environmental Information\n");
            printf("	NOAA E/NE42\n");
            printf("	325 Broadway\n");
            printf("\n	Boulder, CO 80305 USA");
            printf("	Attn: Manoj Nair or Arnaud Chulliat\n");
            printf("	Phone:	(303) 497-4642 or -6522\n");
            printf("	Email:	geomag.models@noaa.gov\n");
            printf("	Web: http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml\n");
            printf("\n VALID RANGE  = %d - %d\n", (int) MagneticModel->min_year, (int) MagneticModel->CoefficientFileEndDate);
            printf(" TIME   = %f\n", value);
            while(1)
            {
                printf("\nPlease press 'C' to continue, 'N' to enter new data or 'X' to exit...\n");
                while (NULL ==fgets(ans, sizeof(ans), stdin)){
                    printf("\nInvalid input\n");
                }
                switch(ans[0]) {
                    case 'X':
                    case 'x':
                        return 0;
                    case 'N':
                    case 'n':
                        return 1;
                    case 'C':
                    case 'c':
                        return 2;
                    default:
                        printf("\nInvalid input %c\n", ans[0]);
                        break;
                }
            }
            break;
		case 5:/*Elevation outside the allowable range*/
		    printf("\nError: The value you have entered of %f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
            while(1)
            {
                printf("\nPlease press 'C' to continue, 'G' to get new data or 'X' to exit...\n");
                while (NULL ==fgets(ans, sizeof(ans), stdin)){
                    printf("\nInvalid input\n");
                }
                switch(ans[0]) {
                    case 'X':
                    case 'x':
                        return 0;
                    case 'G':
                    case 'g':
                        return 1;
                    case 'C':
                    case 'c':
                        return 2;
                    default:
                        printf("\nInvalid input %c\n", ans[0]);
                        break;
                }
            }
            break;
    }
    return 2;
}

void MAG_DegreeToDMSstring(double DegreesOfArc, int UnitDepth, char *DMSstring) {
    int DMS[3], i;
    double temp = DegreesOfArc;
    int tmp_size = 36;
    int tmp2_size = 32;
    std::array<char, tmp2_size>tempstring = "";
    std::array<char, tmp2_size> tempstring2 = "";
    int DMSstring_size = 100;

    std::array<char, DMSstring_size> DMSstring = "";

    for(i = 0; i < UnitDepth; i++)
    {
        DMS[i] = (int) temp;
        switch(i) {
            case 0:
                tempstring2 = "Deg";
                break;
            case 1:
                tempstring2 = "Min";
                break;
            case 2:
                tempstring2 = "Sec";
                break;
        }
        temp = (temp - DMS[i])*60;
        if(i == UnitDepth - 1 && temp >= 30)
            DMS[i]++;
        else if(i == UnitDepth - 1 && temp <= -30)
            DMS[i]--;
        snprintf(tempstring, tmp_size, "%4d%4s", DMS[i], tempstring2);
        strcat(DMSstring, tempstring);   
    }
}

void MAG_PrintUserDataWithUncertainty(MAGtype_GeoMagneticElements GeomagElements,
                                    MAGtype_GeoMagneticElements Errors,
                                    MAGtype_CoordGeodetic SpaceInput,
                                    MAGtype_Date TimeInput,
                                    MAGtype_MagneticModel *MagneticModel,
                                    MAGtype_Geoid *Geoid) {
    int dms_size = 150;
    char *DeclString = malloc(sizeof(char)*dms_size);
    char *InclString = malloc(sizeof(char)*dms_size);
    memset(DeclString, '\0', dms_size);
    memset(InclString, '\0', dms_size);
    MAG_DegreeToDMSstring(GeomagElements.Incl, 2, InclString);
    if(GeomagElements.H < 6000 && GeomagElements.H > 2000) {MAG_Warnings(1, GeomagElements.H, MagneticModel);}
    if(GeomagElements.H < 2000) {MAG_Warnings(2, GeomagElements.H, MagneticModel);}
    MAG_DegreeToDMSstring(GeomagElements.Decl, 2, DeclString);
    printf("\n Results For \n\n");
    if(SpaceInput.phi < 0) {printf("Latitude	%.2fS\n", -SpaceInput.phi);}
    else {printf("Latitude	%.2fN\n", SpaceInput.phi);}
    if(SpaceInput.lambda < 0) {printf("Longitude	%.2fW\n", -SpaceInput.lambda);}
    else {printf("Longitude	%.2fE\n", SpaceInput.lambda);}
    if(Geoid->UseGeoid == 1) {printf("Altitude:	%.2f Kilometers above mean sea level\n", SpaceInput.HeightAboveGeoid);}
    else {printf("Altitude:	%.2f Kilometers above the WGS-84 ellipsoid\n", SpaceInput.HeightAboveEllipsoid);}

    printf("Date:		%.1f\n", TimeInput.DecimalYear);
    printf("\n	Main Field\n");
    printf("F	=	%-9.1f +/-%5.1f nT\n", GeomagElements.F, Errors.F);
    printf("H	=	%-9.1f +/-%5.1f nT\n", GeomagElements.H, Errors.H);
    printf("X	=	%-9.1f +/-%5.1f nT\n", GeomagElements.X, Errors.X);
    printf("Y	=	%-9.1f +/-%5.1f nT\n", GeomagElements.Y, Errors.Y);
    printf("Z	=	%-9.1f +/-%5.1f nT\n", GeomagElements.Z, Errors.Z);
    if(GeomagElements.Decl < 0) {printf("Decl	=%20s  (WEST)+/-%4f\n", DeclString, 60 * Errors.Decl);}
    else {printf("Decl	=%20s  (EAST)+/-%4f\n", DeclString, 60 * Errors.Decl);}
    if(GeomagElements.Incl < 0) {printf("Incl	=%20s  (UP)+/-%4f\n", InclString, 60 * Errors.Incl);}
    else {printf("Incl	=%20s  (DOWN)+/-%4f\n", InclString, 60 * Errors.Incl);}
}