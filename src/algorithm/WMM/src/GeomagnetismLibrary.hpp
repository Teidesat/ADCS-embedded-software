#pragma once

#define ATanH(x)	    (0.5 * log((1 + x) / (1 - x)))

#define MAG_PS_MIN_LAT_DEGREE  -55 /* Minimum Latitude for  Polar Stereographic projection in degrees   */
#define MAG_PS_MAX_LAT_DEGREE  55  /* Maximum Latitude for Polar Stereographic projection in degrees     */
#define MAG_UTM_MIN_LAT_DEGREE -80.5  /* Minimum Latitude for UTM projection in degrees   */
#define MAG_UTM_MAX_LAT_DEGREE  84.5  /* Maximum Latitude for UTM projection in degrees     */


typedef struct {
    double epoch; /*Base time of Geomagnetic model epoch (yrs)*/
    double min_year;
    double *Main_Field_Coeff_G; /* C - Gauss coefficients of main geomagnetic model (nT) Index is (n * (n + 1) / 2 + m) */
    double *Main_Field_Coeff_H; /* C - Gauss coefficients of main geomagnetic model (nT) */
    double *Secular_Var_Coeff_G; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
    double *Secular_Var_Coeff_H; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
    int nMax; /* Maximum degree of spherical harmonic model */
    int SecularVariationUsed; /* Whether or not the magnetic secular variation vector will be needed by program*/
    double CoefficientFileEndDate; 
    
} MAGtype_MagneticModel;


typedef struct {
    double lambda; /* longitude */
    double phi; /* geodetic latitude */
    double HeightAboveEllipsoid; /* height above the ellipsoid (HaE) */
    double HeightAboveGeoid; /* (height above the EGM96 geoid model ) */
    int UseGeoid;
} MAGtype_CoordGeodetic;

typedef struct {
    double lambda; /* longitude*/
    double phig; /* geocentric latitude*/
    double r; /* distance from the center of the ellipsoid*/
} MAGtype_CoordSpherical;

typedef struct {
    double Bx; /* North */
    double By; /* East */
    double Bz; /* Down */
} MAGtype_MagneticResults;

typedef struct {
    double Decl; /* 1. Angle between the magnetic field vector and true north, positive east*/
    double Incl; /*2. Angle between the magnetic field vector and the horizontal plane, positive down*/
    double F; /*3. Magnetic Field Strength*/
    double H; /*4. Horizontal Magnetic Field Strength*/
    double X; /*5. Northern component of the magnetic field vector*/
    double Y; /*6. Eastern component of the magnetic field vector*/
    double Z; /*7. Downward component of the magnetic field vector*/
    double GV; /*8. The Grid Variation*/
    double Decldot; /*9. Yearly Rate of change in declination*/
    double Incldot; /*10. Yearly Rate of change in inclination*/
    double Fdot; /*11. Yearly rate of change in Magnetic field strength*/
    double Hdot; /*12. Yearly rate of change in horizontal field strength*/
    double Xdot; /*13. Yearly rate of change in the northern component*/
    double Ydot; /*14. Yearly rate of change in the eastern component*/
    double Zdot; /*15. Yearly rate of change in the downward component*/
    double GVdot; /*16. Yearly rate of change in grid variation*/
} MAGtype_GeoMagneticElements;

typedef struct {
    int NumbGeoidCols; /* 360 degrees of longitude at 15 minute spacing */
    int NumbGeoidRows; /* 180 degrees of latitude  at 15 minute spacing */
    int ScaleFactor; /* 4 grid cells per degree at 15 minute spacing  */
    float *GeoidHeightBuffer;
    int NumbGeoidElevs;
    int UseGeoid; /*Is the Geoid being used?*/
} MAGtype_Geoid;


typedef struct {
    double Easting; /* (X) in meters*/
    double Northing; /* (Y) in meters */
    int Zone; /*UTM Zone*/
    char HemiSphere;
    double CentralMeridian;
    double ConvergenceOfMeridians;
    double PointScale;
} MAGtype_UTMParameters;



int MAG_Warnings(int control, double value, MAGtype_MagneticModel *MagneticModel);

/*Conversions, Transformations, and other Calculations*/
int MAG_CalculateGeoMagneticElements(MAGtype_MagneticResults *MagneticResultsGeo, MAGtype_GeoMagneticElements *GeoMagneticElements);

void MAG_CalculateGradientElements(MAGtype_MagneticResults GradResults, MAGtype_GeoMagneticElements MagneticElements, MAGtype_GeoMagneticElements *GradElements);

int MAG_CalculateGridVariation(MAGtype_CoordGeodetic location, MAGtype_GeoMagneticElements *elements);

void MAG_DegreeToDMSstring(double DegreesOfArc, int UnitDepth, char *DMSstring);

int MAG_GetTransverseMercator(MAGtype_CoordGeodetic CoordGeodetic, MAGtype_UTMParameters *UTMParameters);

int MAG_GetUTMParameters(double Latitude,
        double Longitude,
        int *Zone,
        char *Hemisphere,
        double *CentralMeridian);

int MAG_RotateMagneticVector(MAGtype_CoordSpherical,
        MAGtype_CoordGeodetic CoordGeodetic,
        MAGtype_MagneticResults MagneticResultsSph,
        MAGtype_MagneticResults *MagneticResultsGeo);

void MAG_TMfwd4(double Eps, double Epssq, double K0R4, double K0R4oa,
        double Acoeff[], double Lam0, double K0, double falseE,
        double falseN, int XYonly, double Lambda, double Phi,
        double *X, double *Y, double *pscale, double *CoM);  

/*Geoid*/
int MAG_ConvertGeoidToEllipsoidHeight(MAGtype_CoordGeodetic *CoordGeodetic, MAGtype_Geoid *Geoid);
/*
 * The function Convert_Geoid_To_Ellipsoid_Height converts the specified WGS84
 * geoid height at the specified geodetic coordinates to the equivalent
 * ellipsoid height, using the EGM96 gravity model.
 *
 *    Latitude            : Geodetic latitude in radians           (input)
 *    Longitude           : Geodetic longitude in radians          (input)
 *    Geoid_Height        : Geoid height, in meters                (input)
 *    Ellipsoid_Height    : Ellipsoid height, in meters.           (output)
 *
 */

int MAG_GetGeoidHeight(double Latitude, double Longitude, double *DeltaHeight, MAGtype_Geoid *Geoid);
/*
 * The private function Get_Geoid_Height returns the height of the
 * WGS84 geiod above or below the WGS84 ellipsoid,
 * at the specified geodetic coordinates,
 * using a grid of height adjustments from the EGM96 gravity model.
 *
 *    Latitude            : Geodetic latitude in radians           (input)
 *    Longitude           : Geodetic longitude in radians          (input)
 *    DeltaHeight         : Height Adjustment, in meters.          (output)
 *
 */

void MAG_EquivalentLatLon(double lat, double lon, double *repairedLat, double  *repairedLon);

void MAG_PrintUserDataWithUncertainty(MAGtype_GeoMagneticElements GeomagElements,
        MAGtype_GeoMagneticElements Errors,
        MAGtype_CoordGeodetic SpaceInput,
        MAGtype_Date TimeInput,
        MAGtype_MagneticModel *MagneticModel,
        MAGtype_Geoid *Geoid);
size_t MAG_strlcpy_equivalent(char *dst, char *src, size_t dstlen);