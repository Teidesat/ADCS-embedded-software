#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include "GeomagnetismLibrary.hpp"




void MAG_Error(int control) {
    switch(control) {
        case 1:
            printf("\nError allocating in MAG_LegendreFunctionMemory.\n");
            break;
        case 2:
            printf("\nError allocating in MAG_AllocateModelMemory.\n");
            break;
        case 3:
            printf("\nError allocating in MAG_InitializeGeoid\n");
            break;
        case 4:
            printf("\nError in setting default values.\n");
            break;
        case 5:
            printf("\nError initializing Geoid.\n");
            break;
        case 7:
            printf("\nError opening WMMSV.COF\n.");
            break;
        case 8:
            printf("\nError reading Magnetic Model.\n");
            break;
        case 9:
            printf("\nError printing Command Prompt introduction.\n");
            break;
        case 10:
            printf("\nError converting from geodetic co-ordinates to spherical co-ordinates.\n");
            break;
        case 11:
            printf("\nError in time modifying the Magnetic model\n");
            break;
        case 12:
            printf("\nError in Geomagnetic\n");
            break;
        case 13:
            printf("\nError printing user data\n");\
			break;
        case 16:
            printf("\nError in opening EGM9615.BIN file\n");
            break;
        case 17:
            printf("\nError: Latitude OR Longitude out of range in MAG_GetGeoidHeight\n");
            break;
        case 21:
            printf("\nError: UnitDepth too large\n");
            break;
        case 22:
            printf("\nYour system needs Big endian version of EGM9615.BIN.  \n");
            printf("Please download this file from http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml.  \n");
            printf("Replace the existing EGM9615.BIN file with the downloaded one\n");
            break;
    }
}

int MAG_Warnings(int control, double value, MAGtype_MagneticModel *MagneticModel)

/*Return value 0 means end program, Return value 1 means get new data, Return value 2 means continue.
  This prints a warning to the screen determined by the control integer. It also takes the value of the parameter causing the warning as a double.  This is unnecessary for some warnings.
  It requires the MagneticModel to determine the current epoch.

 */
{
    char ans[20];
    MAG_strlcpy_equivalent(ans, "", sizeof(ans));
    
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



/******************************************************************************
 *************Conversions, Transformations, and other Calculations**************
 * This grouping consists of functions that perform unit conversions, coordinate
 * transformations and other simple or straightforward calculations that are 
 * usually easily replicable with a typical scientific calculator. 
 ******************************************************************************/




int MAG_CalculateGridVariation(MAGtype_CoordGeodetic location, MAGtype_GeoMagneticElements *elements)

/*Computes the grid variation for |latitudes| > MAG_MAX_LAT_DEGREE

Grivation (or grid variation) is the angle between grid north and
magnetic north. This routine calculates Grivation for the Polar Stereographic
projection for polar locations (Latitude => |55| deg). Otherwise, it computes the grid
variation in UTM projection system. However, the UTM projection codes may be used to compute
the grid variation at any latitudes.

 */
{
    MAGtype_UTMParameters UTMParameters;
    if(location.phi >= MAG_PS_MAX_LAT_DEGREE)
    {
        elements->GV = elements->Decl - location.lambda;
        return 1;
    } else if(location.phi <= MAG_PS_MIN_LAT_DEGREE)
    {
        elements->GV = elements->Decl + location.lambda;
        return 1;
    } else
    {
        MAG_GetTransverseMercator(location, &UTMParameters);
        elements->GV = elements->Decl - UTMParameters.ConvergenceOfMeridians;
    }
    return 0;
}

void MAG_CalculateGradientElements(MAGtype_MagneticResults GradResults, MAGtype_GeoMagneticElements MagneticElements, MAGtype_GeoMagneticElements *GradElements)
{
    GradElements->X = GradResults.Bx;
    GradElements->Y = GradResults.By;
    GradElements->Z = GradResults.Bz;
    
    GradElements->H = (GradElements->X * MagneticElements.X + GradElements->Y * MagneticElements.Y) / MagneticElements.H;
    GradElements->F = (GradElements->X * MagneticElements.X + GradElements->Y * MagneticElements.Y + GradElements->Z * MagneticElements.Z) / MagneticElements.F;
    GradElements->Decl = 180.0 / M_PI * (MagneticElements.X * GradElements->Y - MagneticElements.Y * GradElements->X) / (MagneticElements.H * MagneticElements.H);
    GradElements->Incl = 180.0 / M_PI * (MagneticElements.H * GradElements->Z - MagneticElements.Z * GradElements->H) / (MagneticElements.F * MagneticElements.F);
    GradElements->GV = GradElements->Decl;
}



void MAG_DegreeToDMSstring(double DegreesOfArc, int UnitDepth, char *DMSstring)
{
    int DMS[3], i;
    double temp = DegreesOfArc;
    int tmp_size = 36;
    int tmp2_size = 32;
    char *tempstring = malloc(sizeof(char)*tmp_size);
    char *tempstring2 = malloc(sizeof(char)*tmp2_size);
    int DMSstring_size = 100;

    memset(tempstring, '\0', tmp_size);
    memset(tempstring2, '\0', tmp2_size);

    // The DMSstring is passed as pointer and its size if defined outside of the function. The functions used to pass DMSstring to MAG_DegreeToDMSstring() define the size of DMSstring are all 100.
    //MAG_strlcpy_equivalent(DMSstring, "", DMSstring_size);
    if(UnitDepth > 3)
        MAG_Error(21);
    for(i = 0; i < UnitDepth; i++)
    {
        DMS[i] = (int) temp;
        switch(i) {
            case 0:
                MAG_strlcpy_equivalent(tempstring2, "Deg", tmp2_size);
                break;
            case 1:
                MAG_strlcpy_equivalent(tempstring2, "Min", tmp2_size);
                break;
            case 2:
                MAG_strlcpy_equivalent(tempstring2, "Sec", tmp2_size);
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
    
    free(tempstring);
    free(tempstring2);


} /*MAG_DegreeToDMSstring*/


int MAG_GetTransverseMercator(MAGtype_CoordGeodetic CoordGeodetic, MAGtype_UTMParameters *UTMParameters)
/* Gets the UTM Parameters for a given Latitude and Longitude.*/
{

    double Eps, Epssq;
    double Acoeff[8];
    double Lam0, K0, falseE, falseN;
    double K0R4, K0R4oa;
    double Lambda, Phi;
    int XYonly;
    double X, Y, pscale, CoM;
    int Zone;
    char Hemisphere;



    /*   Get the map projection  parameters */

    Lambda = DEG2RAD(CoordGeodetic.lambda);
    Phi = DEG2RAD(CoordGeodetic.phi);

    MAG_GetUTMParameters(Phi, Lambda, &Zone, &Hemisphere, &Lam0);
    K0 = 0.9996;



    if(Hemisphere == 'n' || Hemisphere == 'N')
    {
        falseN = 0;
    }
    if(Hemisphere == 's' || Hemisphere == 'S')
    {
        falseN = 10000000;
    }

    falseE = 500000;


    /* WGS84 ellipsoid */

    Eps = 0.081819190842621494335;
    Epssq = 0.0066943799901413169961;
    K0R4 = 6367449.1458234153093*K0;
    K0R4oa = K0R4/6378137;


    Acoeff[0] = 8.37731820624469723600E-04;
    Acoeff[1] = 7.60852777357248641400E-07;
    Acoeff[2] = 1.19764550324249124400E-09;
    Acoeff[3] = 2.42917068039708917100E-12;
    Acoeff[4] = 5.71181837042801392800E-15;
    Acoeff[5] = 1.47999793137966169400E-17;
    Acoeff[6] = 4.10762410937071532000E-20;
    Acoeff[7] = 1.21078503892257704200E-22;

    /* WGS84 ellipsoid */


    /*   Execution of the forward T.M. algorithm  */

    XYonly = 0;

    MAG_TMfwd4(Eps, Epssq, K0R4, K0R4oa, Acoeff,
            Lam0, K0, falseE, falseN,
            XYonly,
            Lambda, Phi,
            &X, &Y, &pscale, &CoM);

    /*   Report results  */

    UTMParameters->Easting = X; /* UTM Easting (X) in meters*/
    UTMParameters->Northing = Y; /* UTM Northing (Y) in meters */
    UTMParameters->Zone = Zone; /*UTM Zone*/
    UTMParameters->HemiSphere = Hemisphere;
    UTMParameters->CentralMeridian = RAD2DEG(Lam0); /* Central Meridian of the UTM Zone */
    UTMParameters->ConvergenceOfMeridians = RAD2DEG(CoM); /* Convergence of meridians of the UTM Zone and location */
    UTMParameters->PointScale = pscale;

    return 0;
}

int MAG_GetUTMParameters(double Latitude,
        double Longitude,
        int *Zone,
        char *Hemisphere,
        double *CentralMeridian)
{
    /*
     * The function MAG_GetUTMParameters converts geodetic (latitude and
     * longitude) coordinates to UTM projection parameters (zone, hemisphere and central meridian)
     * If any errors occur, the error code(s) are returned
     * by the function, otherwise TRUE is returned.
     *
     *    Latitude          : Latitude in radians                 (input)
     *    Longitude         : Longitude in radians                (input)
     *    Zone              : UTM zone                            (output)
     *    Hemisphere        : North or South hemisphere           (output)
     *    CentralMeridian	: Central Meridian of the UTM Zone in radians	   (output)
     */

    long Lat_Degrees;
    long Long_Degrees;
    long temp_zone;
    int Error_Code = 0;

    if((Latitude < DEG2RAD(MAG_UTM_MIN_LAT_DEGREE)) || (Latitude > DEG2RAD(MAG_UTM_MAX_LAT_DEGREE)))
    { /* Latitude out of range */
        MAG_Error(23);
        Error_Code = 1;
    }
    if((Longitude < -M_PI) || (Longitude > (2 * M_PI)))
    { /* Longitude out of range */
        MAG_Error(24);
        Error_Code = 1;
    }
    if(!Error_Code)
    { /* no errors */
        if(Longitude < 0)
            Longitude += (2 * M_PI) + 1.0e-10;
        Lat_Degrees = (long) (Latitude * 180.0 / M_PI);
        Long_Degrees = (long) (Longitude * 180.0 / M_PI);

        if(Longitude < M_PI)
            temp_zone = (long) (31 + ((Longitude * 180.0 / M_PI) / 6.0));
        else
            temp_zone = (long) (((Longitude * 180.0 / M_PI) / 6.0) - 29);
        if(temp_zone > 60)
            temp_zone = 1;
        /* UTM special cases */
        if((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > -1)
                && (Long_Degrees < 3))
            temp_zone = 31;
        if((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > 2)
                && (Long_Degrees < 12))
            temp_zone = 32;
        if((Lat_Degrees > 71) && (Long_Degrees > -1) && (Long_Degrees < 9))
            temp_zone = 31;
        if((Lat_Degrees > 71) && (Long_Degrees > 8) && (Long_Degrees < 21))
            temp_zone = 33;
        if((Lat_Degrees > 71) && (Long_Degrees > 20) && (Long_Degrees < 33))
            temp_zone = 35;
        if((Lat_Degrees > 71) && (Long_Degrees > 32) && (Long_Degrees < 42))
            temp_zone = 37;

        if(!Error_Code)
        {
            if(temp_zone >= 31)
                *CentralMeridian = (6 * temp_zone - 183) * M_PI / 180.0;
            else
                *CentralMeridian = (6 * temp_zone + 177) * M_PI / 180.0;
            *Zone = temp_zone;
            if(Latitude < 0) *Hemisphere = 'S';
            else *Hemisphere = 'N';
        }
    } /* END OF if (!Error_Code) */
    return (Error_Code);
}




void MAG_TMfwd4(double Eps, double Epssq, double K0R4, double K0R4oa,
        double Acoeff[], double Lam0, double K0, double falseE,
        double falseN, int XYonly, double Lambda, double Phi,
        double *X, double *Y, double *pscale, double *CoM)
{

    /*  Transverse Mercator forward equations including point-scale and CoM
            =--------- =------- =--=--= ---------

       Algorithm developed by: C. Rollins   August 7, 2006
       C software written by:  K. Robins


            Constants fixed by choice of ellipsoid and choice of projection parameters
            ---------------

              Eps          Eccentricity (epsilon) of the ellipsoid
              Epssq        Eccentricity squared
            ( R4           Meridional isoperimetric radius   )
            ( K0           Central scale factor              )
              K0R4         K0 times R4
              K0R4oa       K0 times Ratio of R4 over semi-major axis
              Acoeff       Trig series coefficients, omega as a function of chi
              Lam0         Longitude of the central meridian in radians
              K0           Central scale factor, for example, 0.9996 for UTM
              falseE       False easting, for example, 500000 for UTM
              falseN       False northing

       Processing option
       ---------- ------

              XYonly       If one (1), then only X and Y will be properly
                                       computed.  Values returned for point-scale
                                       and CoM will merely be the trivial values for
                                       points on the central meridian

       Input items that identify the point to be converted
       ----- -----

              Lambda       Longitude (from Greenwich) in radians
              Phi          Latitude in radians

       Output items
       ------ -----

              X            X coordinate (Easting) in meters
              Y            Y coordinate (Northing) in meters
              pscale       point-scale (dimensionless)
          CoM          Convergence-of-meridians in radians
     */

    double Lam, CLam, SLam, CPhi, SPhi;
    double P, part1, part2, denom, CChi, SChi;
    double U, V;
    double T, Tsq, denom2;
    double c2u, s2u, c4u, s4u, c6u, s6u, c8u, s8u;
    double c2v, s2v, c4v, s4v, c6v, s6v, c8v, s8v;
    double Xstar, Ystar;
    double sig1, sig2, comroo;

    /*
       Ellipsoid to sphere
       --------- -- ------

       Convert longitude (Greenwhich) to longitude from the central meridian
       It is unnecessary to find the (-Pi, Pi] equivalent of the result.
       Compute its cosine and sine.
     */

    Lam = Lambda - Lam0;
    CLam = cos(Lam);
    SLam = sin(Lam);

    /*   Latitude  */

    CPhi = cos(Phi);
    SPhi = sin(Phi);

    /*   Convert geodetic latitude, Phi, to conformal latitude, Chi
         Only the cosine and sine of Chi are actually needed.        */

    P = exp(Eps * ATanH(Eps * SPhi));
    part1 = (1 + SPhi) / P;
    part2 = (1 - SPhi) * P;
    denom = 1 / (part1 + part2);
    CChi = 2 * CPhi * denom;
    SChi = (part1 - part2) * denom;

    /*
       Sphere to first plane
       ------ -- ----- -----

       Apply spherical theory of transverse Mercator to get (u,v) coordinates
       Note the order of the arguments in Fortran's version of ArcTan, i.e.
                 atan2(y, x) = ATan(y/x)
       The two argument form of ArcTan is needed here.
     */

    T = CChi * SLam;
    U = ATanH(T);
    V = atan2(SChi, CChi * CLam);

    /*
       Trigonometric multiple angles
       ------------- -------- ------

       Compute Cosh of even multiples of U
       Compute Sinh of even multiples of U
       Compute Cos  of even multiples of V
       Compute Sin  of even multiples of V
     */

    Tsq = T * T;
    denom2 = 1 / (1 - Tsq);
    c2u = (1 + Tsq) * denom2;
    s2u = 2 * T * denom2;
    c2v = (-1 + CChi * CChi * (1 + CLam * CLam)) * denom2;
    s2v = 2 * CLam * CChi * SChi * denom2;

    c4u = 1 + 2 * s2u * s2u;
    s4u = 2 * c2u * s2u;
    c4v = 1 - 2 * s2v * s2v;
    s4v = 2 * c2v * s2v;

    c6u = c4u * c2u + s4u * s2u;
    s6u = s4u * c2u + c4u * s2u;
    c6v = c4v * c2v - s4v * s2v;
    s6v = s4v * c2v + c4v * s2v;

    c8u = 1 + 2 * s4u * s4u;
    s8u = 2 * c4u * s4u;
    c8v = 1 - 2 * s4v * s4v;
    s8v = 2 * c4v * s4v;


    /*   First plane to second plane
         ----- ----- -- ------ -----

         Accumulate terms for X and Y
     */

    Xstar = Acoeff[3] * s8u * c8v;
    Xstar = Xstar + Acoeff[2] * s6u * c6v;
    Xstar = Xstar + Acoeff[1] * s4u * c4v;
    Xstar = Xstar + Acoeff[0] * s2u * c2v;
    Xstar = Xstar + U;

    Ystar = Acoeff[3] * c8u * s8v;
    Ystar = Ystar + Acoeff[2] * c6u * s6v;
    Ystar = Ystar + Acoeff[1] * c4u * s4v;
    Ystar = Ystar + Acoeff[0] * c2u * s2v;
    Ystar = Ystar + V;

    /*   Apply isoperimetric radius, scale adjustment, and offsets  */

    *X = K0R4 * Xstar + falseE;
    *Y = K0R4 * Ystar + falseN;


    /*  Point-scale and CoM
        ----- ----- --- ---  */

    if(XYonly == 1)
    {
        *pscale = K0;
        *CoM = 0;
    } else
    {
        sig1 = 8 * Acoeff[3] * c8u * c8v;
        sig1 = sig1 + 6 * Acoeff[2] * c6u * c6v;
        sig1 = sig1 + 4 * Acoeff[1] * c4u * c4v;
        sig1 = sig1 + 2 * Acoeff[0] * c2u * c2v;
        sig1 = sig1 + 1;

        sig2 = 8 * Acoeff[3] * s8u * s8v;
        sig2 = sig2 + 6 * Acoeff[2] * s6u * s6v;
        sig2 = sig2 + 4 * Acoeff[1] * s4u * s4v;
        sig2 = sig2 + 2 * Acoeff[0] * s2u * s2v;

        /*    Combined square roots  */
        comroo = sqrt((1 - Epssq * SPhi * SPhi) * denom2 *
                (sig1 * sig1 + sig2 * sig2));

        *pscale = K0R4oa * 2 * denom * comroo;
        *CoM = atan2(SChi * SLam, CLam) + atan2(sig2, sig1);
    }
} /*MAG_TMfwd4*/


/******************************************************************************
 *************************************Geoid************************************
 * This grouping consists of functions that make calculations to adjust 
 * ellipsoid height to height above the geoid (Height above MSL). 
 ******************************************************************************
 ******************************************************************************/

int MAG_ConvertGeoidToEllipsoidHeight(MAGtype_CoordGeodetic *CoordGeodetic, MAGtype_Geoid *Geoid)

/*
 * The function Convert_Geoid_To_Ellipsoid_Height converts the specified WGS84
 * Geoid height at the specified geodetic coordinates to the equivalent
 * ellipsoid height, using the EGM96 gravity model.
 *
 *   CoordGeodetic->phi        : Geodetic latitude in degress           (input)
 *    CoordGeodetic->lambda     : Geodetic longitude in degrees          (input)
 *    CoordGeodetic->HeightAboveEllipsoid	     : Ellipsoid height, in kilometers         (output)
 *    CoordGeodetic->HeightAboveGeoid: Geoid height, in kilometers           (input)
 *

 */
{
    double DeltaHeight;
    int Error_Code;
    double lat, lon;

    if(Geoid->UseGeoid == 1){ 
        /* Geoid correction required */
        /* To ensure that latitude is less than 90 call MAG_EquivalentLatLon() */
        MAG_EquivalentLatLon(CoordGeodetic->phi, CoordGeodetic->lambda, &lat, &lon);
        Error_Code = MAG_GetGeoidHeight(lat, lon, &DeltaHeight, Geoid);
        CoordGeodetic->HeightAboveEllipsoid = CoordGeodetic->HeightAboveGeoid + DeltaHeight / 1000; /*  Input and output should be kilometers,
			However MAG_GetGeoidHeight returns Geoid height in meters - Hence division by 1000 */
    } 
    else {
        /* Geoid correction not required, copy the MSL height to Ellipsoid height */
        CoordGeodetic->HeightAboveEllipsoid = CoordGeodetic->HeightAboveGeoid;
        Error_Code = 1;
    }
    return ( Error_Code);
} /* MAG_ConvertGeoidToEllipsoidHeight*/

int MAG_GetGeoidHeight(double Latitude,
        double Longitude,
        double *DeltaHeight,
        MAGtype_Geoid *Geoid)
/*
 * The  function MAG_GetGeoidHeight returns the height of the
 * EGM96 geiod above or below the WGS84 ellipsoid,
 * at the specified geodetic coordinates,
 * using a grid of height adjustments from the EGM96 gravity model.
 *
 *    Latitude            : Geodetic latitude in radians           (input)
 *    Longitude           : Geodetic longitude in radians          (input)
 *    DeltaHeight         : Height Adjustment, in meters.          (output)
 *    Geoid				  : MAGtype_Geoid with Geoid grid		   (input)
        CALLS : none
 */
{
    long Index;
    double DeltaX, DeltaY;
    double ElevationSE, ElevationSW, ElevationNE, ElevationNW;
    double OffsetX, OffsetY;
    double PostX, PostY;
    double UpperY, LowerY;
    int Error_Code = 0;

    if((Latitude < -90) || (Latitude > 90))
    { /* Latitude out of range */
        Error_Code |= 1;
    }
    if((Longitude < -180) || (Longitude > 360))
    { /* Longitude out of range */
        Error_Code |= 1;
    }

    if(!Error_Code)
    { /* no errors */
        /*  Compute X and Y Offsets into Geoid Height Array:                          */

        if(Longitude < 0.0) {OffsetX = (Longitude + 360.0) * Geoid->ScaleFactor;} 
        else {OffsetX = Longitude * Geoid->ScaleFactor;}
        OffsetY = (90.0 - Latitude) * Geoid->ScaleFactor;

        /*  Find Four Nearest Geoid Height Cells for specified Latitude, Longitude;   */
        /*  Assumes that (0,0) of Geoid Height Array is at Northwest corner:          */

        PostX = floor(OffsetX);
        if((PostX + 1) == Geoid->NumbGeoidCols) {PostX--;}
        PostY = floor(OffsetY);
        if((PostY + 1) == Geoid->NumbGeoidRows) {PostY--;}

        Index = (long) (PostY * Geoid->NumbGeoidCols + PostX);
        ElevationNW = (double) Geoid->GeoidHeightBuffer[ Index ];
        ElevationNE = (double) Geoid->GeoidHeightBuffer[ Index + 1 ];

        Index = (long) ((PostY + 1) * Geoid->NumbGeoidCols + PostX);
        ElevationSW = (double) Geoid->GeoidHeightBuffer[ Index ];
        ElevationSE = (double) Geoid->GeoidHeightBuffer[ Index + 1 ];

        /*  Perform Bi-Linear Interpolation to compute Height above Ellipsoid:        */

        DeltaX = OffsetX - PostX;
        DeltaY = OffsetY - PostY;

        UpperY = ElevationNW + DeltaX * (ElevationNE - ElevationNW);
        LowerY = ElevationSW + DeltaX * (ElevationSE - ElevationSW);

        *DeltaHeight = UpperY + DeltaY * (LowerY - UpperY);
    } 
    else {
        MAG_Error(17);
        return (0);
    }
    return 1;
}

void MAG_EquivalentLatLon(double lat, double lon, double *repairedLat, double  *repairedLon) 
/*This function takes a latitude and longitude that are ordinarily out of range 
 and gives in range values that are equivalent on the Earth's surface.  This is
 required to get correct values for the geoid function.*/
{
    double colat;
    colat = 90 - lat;
    *repairedLon = lon;
    if (colat < 0) {colat = -colat;}
    while(colat > 360) {colat-=360;}
    if (colat > 180) {
        colat-=180;
        *repairedLon = *repairedLon+180;
    }
    *repairedLat = 90 - colat;
    if(*repairedLon > 360) {*repairedLon-=360;}
    if(*repairedLon < -180) {*repairedLon+=360;}
}




void MAG_PrintUserDataWithUncertainty(MAGtype_GeoMagneticElements GeomagElements,
        MAGtype_GeoMagneticElements Errors,
        MAGtype_CoordGeodetic SpaceInput,
        MAGtype_Date TimeInput,
        MAGtype_MagneticModel *MagneticModel,
        MAGtype_Geoid *Geoid)
{
    int dms_size = 150;
    char *DeclString = malloc(sizeof(char)*dms_size);
    char *InclString = malloc(sizeof(char)*dms_size);
    char *GVString = malloc(sizeof(char)*dms_size); 
    memset(DeclString, '\0', dms_size);
    memset(InclString, '\0', dms_size);
    memset(GVString, '\0', dms_size);
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

    if(MagneticModel->SecularVariationUsed == 1) {
        printf("Date:		%.1f\n", TimeInput.DecimalYear);
        printf("\n		Main Field\t\t\tSecular Change\n");
        printf("F	=	%9.1f +/- %5.1f nT\t\t Fdot = %5.1f\tnT/yr\n", GeomagElements.F, Errors.F, GeomagElements.Fdot);
        printf("H	=	%9.1f +/- %5.1f nT\t\t Hdot = %5.1f\tnT/yr\n", GeomagElements.H, Errors.H, GeomagElements.Hdot);
        printf("X	=	%9.1f +/- %5.1f nT\t\t Xdot = %5.1f\tnT/yr\n", GeomagElements.X, Errors.X, GeomagElements.Xdot);
        printf("Y	=	%9.1f +/- %5.1f nT\t\t Ydot = %5.1f\tnT/yr\n", GeomagElements.Y, Errors.Y, GeomagElements.Ydot);
        printf("Z	=	%9.1f +/- %5.1f nT\t\t Zdot = %5.1f\tnT/yr\n", GeomagElements.Z, Errors.Z, GeomagElements.Zdot);
        if(GeomagElements.Decl < 0) {printf("Decl	=%20s  (WEST) +/-%3.0f Min Ddot = %.1f\tMin/yr\n", DeclString, 60 * Errors.Decl, 60 * GeomagElements.Decldot);}
        else {printf("Decl	=%20s  (EAST) +/-%3.0f Min Ddot = %.1f\tMin/yr\n", DeclString, 60 * Errors.Decl, 60 * GeomagElements.Decldot);}
        if(GeomagElements.Incl < 0) {printf("Incl	=%20s  (UP)   +/-%3.0f Min Idot = %.1f\tMin/yr\n", InclString, 60 * Errors.Incl, 60 * GeomagElements.Incldot);}
        else {printf("Incl	=%20s  (DOWN) +/-%3.0f Min Idot = %.1f\tMin/yr\n", InclString, 60 * Errors.Incl, 60 * GeomagElements.Incldot);}
    } 
    else {
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

    MAG_DegreeToDMSstring(GeomagElements.GV, 2, GVString);
    
    /* Print Grid Variation */
    if(SpaceInput.phi < -55) {printf("\n\n Grid variation (SOUTH) =%20s\n", GVString);}
    else if (SpaceInput.phi > 55) {printf("\n\n Grid variation (NORTH) =%20s \n", GVString);}
}/*MAG_PrintUserDataWithUncertainty*/

size_t MAG_strlcpy_equivalent(char *dst, char *src, size_t dstlen){
/*The strlcpy is not the standard C library on Linux*/
    size_t srclen;
    if(src != NULL) {srclen = strlen(src);}
    else {return 0;}
    if (dstlen > 0) {
        int copy_size = (srclen >= dstlen) ? dstlen - 1:srclen;
        memcpy(dst, src, copy_size);
        dst[dstlen-1] = '\0';
    }
    
    return srclen;
}