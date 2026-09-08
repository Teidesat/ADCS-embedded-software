#include "worldMagneticModel.hpp"

#include "../../lib/math/eulerAngles/eulerAngles.hpp"
#include "constants/ellipsoid.hpp"

#include <math.h>

namespace WMM {

GeodeticCoordinates::GeodeticCoordinates(const float newLatitude, const float newLongitude, const float newHeightAboveEllipsoid, const float newHeightAboveGeoid ): 
    latitude(newLatitude),
    longitude(newLongitude),
    HeightAboveEllipsoid(newHeightAboveEllipsoid),
    HeightAboveGeoid(newHeightAboveGeoid)
{}

SphericalCoordinates convertGeodeticCoordinatesToSphericalcoordinates(const GeodeticCoordinates& geodeticCoordinates) {
    const float latitudeCosine = cos(EulerAngles::degreesToRadians(geodeticCoordinates.latitude));
    const float latitudeSine = sin(EulerAngles::degreesToRadians(geodeticCoordinates.latitude));
    const float rc = Ellipsoid::semiMajorAxis / sqrt(1.0 - Ellipsoid::firstEccentricitySquared * latitudeSine * latitudeSine);
    const float xp = (rc + geodeticCoordinates.HeightAboveEllipsoid) * latitudeCosine;
    const float zp = (rc * (1.0 - Ellipsoid::firstEccentricitySquared) + geodeticCoordinates.HeightAboveEllipsoid) * latitudeSine;

    SphericalCoordinates sphericalCoordinates;
    sphericalCoordinates.radius = sqrt(xp * xp + zp * zp);
    sphericalCoordinates.latitude = EulerAngles::radiansToDegrees(asin(zp / sphericalCoordinates.radius));
    sphericalCoordinates.longitude = geodeticCoordinates.longitude; 

    return sphericalCoordinates;
}

float dateToYears(const int year, const int month, const int day, const float epoch) {  //check that month day and year are valid?
    float extraDay = 0;
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {extraDay = 1;}

    std::array<int, 13> daysInAMonth = {0, 31 ,28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const float daysInAYear = 365.0;
    const float daysInAYearLeapYearAdjust = daysInAYear + extraDay;
    daysInAMonth[2] += extraDay;

    int totalDays = 0;
    for(int i = 0; i < month; i++) {totalDays += daysInAMonth[i];}
    totalDays += day;
    float decimalDate = year + (totalDays - 1) / daysInAYearLeapYearAdjust;

    if(decimalDate == -1) {decimalDate = epoch;} 
    return decimalDate;
}

TimedMagneticModel timelyModifyCoefficients(const int year, const int month, const int day) {
    TimedMagneticModel timedMagneticModel;
    timedMagneticModel.year = year;
    timedMagneticModel.month = month;
    timedMagneticModel.day = day;

    constexpr int b = (StaticMagneticModel::nMax * (StaticMagneticModel::nMax + 1) / 2 + StaticMagneticModel::nMax);
    const float timedModeldecimalDate = dateToYears(timedMagneticModel.year, timedMagneticModel.month, timedMagneticModel.day, StaticMagneticModel::epoch);
    
    for(int i = 1; i <= StaticMagneticModel::nMax; i++) {
        for(int j = 0; j <= i; j++) {
            const int index = StaticMagneticModel::calculateIndex(i, j);
            if(index <= b) {
                timedMagneticModel.mainFieldCoefficientsH[index] = StaticMagneticModel::mainFieldCoefficientsH[index] + (timedModeldecimalDate - StaticMagneticModel::epoch) * StaticMagneticModel::secularVarCoefficientsH[index];
                timedMagneticModel.mainFieldCoefficientsG[index] = StaticMagneticModel::mainFieldCoefficientsG[index] + (timedModeldecimalDate - StaticMagneticModel::epoch) * StaticMagneticModel::secularVarCoefficientsG[index];
                timedMagneticModel.secularVarCoefficientsH[index] = StaticMagneticModel::secularVarCoefficientsH[index];
                timedMagneticModel.secularVarCoefficientsG[index] = StaticMagneticModel::secularVarCoefficientsG[index];
            } 
            else {
                timedMagneticModel.mainFieldCoefficientsH[index] = StaticMagneticModel::mainFieldCoefficientsH[index];
                timedMagneticModel.mainFieldCoefficientsG[index] = StaticMagneticModel::mainFieldCoefficientsG[index];
            }
        }
    }
    return timedMagneticModel;
}

SphericalHarmonicVariables computeSphericalHarmonicVariables(const SphericalCoordinates& CoordSpherical) {
    SphericalHarmonicVariables sphericalHarmonicVariables;
    const float radiusRatio = Ellipsoid::meanRadius / CoordSpherical.radius;
    sphericalHarmonicVariables.relativeRadiusPower[0] = (Ellipsoid::meanRadius / CoordSpherical.radius) * (Ellipsoid::meanRadius / CoordSpherical.radius);
    for(int i = 1; i <= StaticMagneticModel::numberOfTerms; i++) {
        sphericalHarmonicVariables.relativeRadiusPower[i] = sphericalHarmonicVariables.relativeRadiusPower[i - 1] * (Ellipsoid::meanRadius / CoordSpherical.radius);
    }

    const float cosLongitude = cos(EulerAngles::degreesToRadians(CoordSpherical.longitude));
    const float sinLongitude = sin(EulerAngles::degreesToRadians(CoordSpherical.longitude));

    sphericalHarmonicVariables.cosMlambda[0] = 1.0;
    sphericalHarmonicVariables.sinMlambda[0] = 0.0;

    sphericalHarmonicVariables.cosMlambda[1] = cosLongitude;
    sphericalHarmonicVariables.sinMlambda[1] = sinLongitude;
       
    for(int i = 2; i <= StaticMagneticModel::numberOfTerms; i++) {
        const int iMinusOne = i - 1;
        sphericalHarmonicVariables.cosMlambda[i] = sphericalHarmonicVariables.cosMlambda[iMinusOne] * cosLongitude - sphericalHarmonicVariables.sinMlambda[iMinusOne] * sinLongitude;
        sphericalHarmonicVariables.sinMlambda[i] = sphericalHarmonicVariables.cosMlambda[iMinusOne] * sinLongitude + sphericalHarmonicVariables.sinMlambda[iMinusOne] * cosLongitude;
    }

    return sphericalHarmonicVariables;
}

LegendreFunction computePcupLow(const float sineOfLatitude) {
    // Compute the Gauss-normalized associated Legendre functions
    const float cosineOfLatitude = sqrt((1.0 - sineOfLatitude) * (1.0 + sineOfLatitude));
    LegendreFunction legendreFunction;
    legendreFunction.Pcup[0] = 1.0;
    legendreFunction.derivedPcup[0] = 0.0;

    for(int i = 1; i <= StaticMagneticModel::numberOfTerms; i++) {
        for(int j = 0; j <= i; j++) { // for all arrays used, never go out of bounds for the index?
            const int pyramidIndex = i * (i + 1) / 2 + j; // 
            if(i == j) {
                const int pyramidIndex = (i - 1) * i / 2 + j - 1; // pyramid index is overwritten here, is it ok, check all indexes?
                legendreFunction.Pcup [pyramidIndex] = cosineOfLatitude * legendreFunction.Pcup[pyramidIndex];
                legendreFunction.derivedPcup[pyramidIndex] = cosineOfLatitude * legendreFunction.derivedPcup[pyramidIndex] + sineOfLatitude * legendreFunction.Pcup[pyramidIndex];
            } 
            else if(i == 1 && j == 0) {
                const int pyramidIndex = (i - 1) * i / 2 + j;
                legendreFunction.Pcup[pyramidIndex] = sineOfLatitude * legendreFunction.Pcup[pyramidIndex];
                legendreFunction.derivedPcup[pyramidIndex] = sineOfLatitude * legendreFunction.derivedPcup[pyramidIndex] - cosineOfLatitude * legendreFunction.Pcup[pyramidIndex];
            } 
            else if(i > 1 && i != j) {
                const int pyramidIndex1 = (i - 2) * (i - 1) / 2 + j;
                const int pyramidIndex2 = (i - 1) * i / 2 + j;
                if(j > i - 2){
                    legendreFunction.Pcup[pyramidIndex1] = sineOfLatitude * legendreFunction.Pcup[pyramidIndex2];
                    legendreFunction.derivedPcup[pyramidIndex1] = sineOfLatitude * legendreFunction.derivedPcup[pyramidIndex2] - cosineOfLatitude * legendreFunction.Pcup[pyramidIndex2];
                } 
                else {
                    const float k = (((i - 1) * (i - 1)) - (j * j)) / ((2 * i - 1) * (2 * i - 3));
                    legendreFunction.Pcup[pyramidIndex1] = sineOfLatitude * legendreFunction.Pcup[pyramidIndex2] - k * legendreFunction.Pcup[pyramidIndex1];
                    legendreFunction.derivedPcup[pyramidIndex1] = sineOfLatitude * legendreFunction.derivedPcup[pyramidIndex2] - cosineOfLatitude * legendreFunction.Pcup[pyramidIndex2] - k * legendreFunction.derivedPcup[pyramidIndex1];
                }
            }
        }
    }

    // Compute the ration between the the Schmidt quasi-normalized associated Legendre functions and the Gauss-normalized version. 
    std::array<float, StaticMagneticModel::numberOfTerms + 1> schmidtQuasiNorm;
    schmidtQuasiNorm[0] = 1.0;
    
    for(int i = 1; i <= StaticMagneticModel::numberOfTerms; i++) {
        const int pyramidIndex1 = (i * (i + 1)) / 2;
        const int pyramidIndex2 = (i - 1) * i / 2;
        schmidtQuasiNorm[pyramidIndex1] = schmidtQuasiNorm[pyramidIndex2] * (2 * i - 1) / i; // for j = 0

        for(int j = 1; j <= i; j++) {
            const int pyramidIndex3 = pyramidIndex1 + j;
            const int pyramidIndex4 = pyramidIndex1 + j - 1;
            schmidtQuasiNorm[pyramidIndex3] = schmidtQuasiNorm[pyramidIndex4] * sqrt(((i - j + 1) * (j == 1 ? 2 : 1)) / (i + j));
        }
    }

    // Converts the Gauss-normalized associated Legendre functions to the Schmidt quasi-normalized version using pre-computedrelation stored in the variable schmidtQuasiNorm
    // The sign is changed since we use derive with respect to latitude insted of co-latitude
    for(int i = 1; i <= StaticMagneticModel::numberOfTerms; i++) {
        for(int j = 0; j <= i; j++) {
            const int pyramidIndex = (i * (i + 1) / 2 + j);
            legendreFunction.Pcup[pyramidIndex] = legendreFunction.Pcup[pyramidIndex] * schmidtQuasiNorm[pyramidIndex];
            legendreFunction.derivedPcup[pyramidIndex] = (-legendreFunction.derivedPcup[pyramidIndex]) * schmidtQuasiNorm[pyramidIndex]; 
        }
    }

    return legendreFunction;
}

/*	This function evaluates all of the Schmidt-semi normalized associated Legendre
        functions up to degree nMax. The functions are initially scaled by
        10^280 sin^m in order to minimize the effects of underflow at large m
        near the poles
        Note that this function performs the same operation as MAG_PcupLow.
        However this function also can be used for high degree (large nMax) models.

  Change from the previous version
  The prevous version computes the derivatives as
  dP(n,m)(x)/dx, where x = sin(latitude) (or cos(colatitude) ).
  However, the WMM Geomagnetic routines requires dP(n,m)(x)/dlatitude.
  Hence the derivatives are multiplied by sin(latitude).
  Removed the options for CS phase and normalizations.

  The derivatives can't be computed for latitude = |90| degrees.
 */
LegendreFunction computePcupHigh(const float sineOfLatitude) {
    const float cosineOfLatitude = sqrt((1.0 - sineOfLatitude) * (1.0 + sineOfLatitude));

    // return empty object if any of these conditions are met?
    if(cosineOfLatitude == 0) {printf("Error in PcupHigh: \n");}
    if(fabs(sineOfLatitude) == 1.0) {printf("Error in PcupHigh: derivative cannot be calculated at poles\n");}

    std::array<float, StaticMagneticModel::numberOfTerms + 1> f1;
    std::array<float, StaticMagneticModel::numberOfTerms + 1> f2;
    std::array<float, StaticMagneticModel::numberOfTerms + 1> squareRoots;

    for(int i = 0; i <= 2 * StaticMagneticModel::nMax + 1; i++) {squareRoots[i] = sqrt(i);}

    int k = 2;
    // const index = 2 * i - 1?


    for(int i = 2; i <= StaticMagneticModel::nMax; i++) {
        k = k + 1;
        f1[k] = (2 * i - 1) / i;
        f2[k] = (i - 1) / i;
        for(int j = 1; j <= i - 2; j++) {
            k = k + 1;
            f1[k] = (2 * i - 1) / squareRoots[i + j] / squareRoots[i - j];
            f2[k] = squareRoots[i - j - 1] * squareRoots[i + j - 1] / squareRoots[i + j] / squareRoots[i - j];
        }
        k = k + 2;
    }

    LegendreFunction legendreFunction;
    
    float pm1 = sineOfLatitude;
    float pm2 = 1.0;
    k = 1;
    legendreFunction.Pcup[0] = 1.0;
    legendreFunction.derivedPcup[0] = 0.0;
    legendreFunction.Pcup[1] = pm1;
    legendreFunction.derivedPcup[1] = cosineOfLatitude;
    

    for(int i = 2; i <= StaticMagneticModel::nMax; i++) {
        k = k + i;
        const float plm = f1[k] * sineOfLatitude * pm1 - f2[k] * pm2;
        legendreFunction.Pcup[k] = plm;
        legendreFunction.derivedPcup[k] = i * (pm1 - sineOfLatitude * plm) / cosineOfLatitude;
        pm2 = pm1;
        pm1 = plm;
    }

    const float scalef = 1.0e-280; // use constant threshold?
    float pmm = squareRoots[2] * scalef;
    float rescalem = 1.0 / scalef;
    int kstart = 0;

    for(int j = 1; j < StaticMagneticModel::nMax; j++) { // for(int j = 1; j <= StaticMagneticModel::nMax - 1; j++) condition before was good?
        rescalem = rescalem * cosineOfLatitude;

        // Calculate Pcup(m,m)
        kstart = kstart + j + 1;

        // const int index = 2 + j + 1;?
        pmm = pmm * squareRoots[2 * j + 1] / squareRoots[2 * j];
        legendreFunction.Pcup[kstart] = pmm * rescalem / squareRoots[2 * j + 1];
        legendreFunction.derivedPcup[kstart] = -(j * sineOfLatitude * legendreFunction.Pcup[kstart] / cosineOfLatitude);
        pm2 = pmm / squareRoots[2 * j + 1];
        // Calculate Pcup(m+1,m)
        k = kstart + j + 1;
        pm1 = sineOfLatitude * squareRoots[2 * j + 1] * pm2;
        legendreFunction.Pcup[k] = pm1*rescalem;
        legendreFunction.derivedPcup[k] = ((pm2 * rescalem) * squareRoots[2 * j + 1] - sineOfLatitude *  (j + 1) * legendreFunction.Pcup[k]) / cosineOfLatitude;

        // Calculate Pcup(n,m)
        for(int i = j + 2; i <= StaticMagneticModel::nMax; i++) {
            k = k + i;
            const float plm = sineOfLatitude * f1[k] * pm1 - f2[k] * pm2;
            legendreFunction.Pcup[k] = plm*rescalem;
            legendreFunction.derivedPcup[k] = (squareRoots[i + j] * squareRoots[i - j] * (pm1 * rescalem) -  i * sineOfLatitude * legendreFunction.Pcup[k]) / cosineOfLatitude;
            pm2 = pm1;
            pm1 = plm;
        }
    }

    // Calculate Pcup(nMax,nMax)
    rescalem = rescalem * sineOfLatitude;
    kstart = kstart + StaticMagneticModel::nMax + 1; // (kstart = kstart + j + 1;) coode before was good?
    pmm = pmm / squareRoots[2 * StaticMagneticModel::nMax];
    legendreFunction.Pcup[kstart] = pmm * rescalem;
    legendreFunction.derivedPcup[kstart] = -(StaticMagneticModel::nMax) * sineOfLatitude * legendreFunction.Pcup[kstart] / cosineOfLatitude;

    return legendreFunction;
}

/* This function evaluates all of the Schmidt-semi normalized associated Legendre. Note: In geomagnetism, the derivatives of 
    ALF are usually found with respect to the colatitudes. Here the derivatives are found with respect to the latitude. 
    The difference is a sign reversal for the derivative of the Associated Legendre Functions.*/
LegendreFunction computedAssociatedLegendreFunction(const SphericalCoordinates& sphericalCoordinates) {
    const float threshold = 1.0e-10; // used in other places already, store it in file for constants threshold?
    LegendreFunction legendre;
    float sineOfLatitude = sin(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));
    if(1 - fabs(sineOfLatitude) < threshold) {legendre = computePcupLow(sineOfLatitude);}   // If nMax is less tha 16 or at the poles and Overflow may occur if nMax > 20?
    else {legendre = computePcupHigh(sineOfLatitude);}

    return legendre;
}

float summationSpecial(const TimedMagneticModel& timedMagneticModel, const SphericalHarmonicVariables& spericalHarmonicVariables, const SphericalCoordinates& sphericalCoordinates) {
    std::array<float, StaticMagneticModel::numberOfTerms + 1> PcupS = {};
    PcupS[0] = 1;
    float schmidtQuasiNorm1 = 1.0;


    float magneticResultBy = 0.0;
    const float sin_phi = sin(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));

    for(int i = 1; i <= StaticMagneticModel::nMax; i++) {

    /*Compute the ration between the Gauss-normalized associated Legendre
    functions and the Schmidt quasi-normalized version. This is equivalent to
    sqrt((m==0?1:2)*(n-m)!/(n+m!))*(2n-1)!!/(n-m)!  */
        const float schmidtQuasiNorm2 = schmidtQuasiNorm1 * (2 * i - 1) / i;
        const float schmidtQuasiNorm3 = schmidtQuasiNorm2 * sqrt((i * 2) / (i + 1));
        schmidtQuasiNorm1 = schmidtQuasiNorm2;
        if(i == 1) {PcupS[i] = PcupS[i - 1];} // // PcupS[0] = 1, but then PcupS[1] = PcupS[1 - 1]?
        else{
            const float k = (((i - 1) * (i - 1)) - 1) / ((2 * i - 1) * (2 * i - 3));
            PcupS[i] = sin_phi * PcupS[i - 1] - k * PcupS[i - 2];
        }

        /*		  1 nMax  (n+2)    n     m            m           m
                By =    SUM (a/r) (m)  SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                           n=1             m=0   n            n           n  */
        /* Equation 11 in the WMM Technical report. Derivative with respect to longitude, divided by radius. */
        const int index = (i * (i + 1) / 2 + 1);
        magneticResultBy += spericalHarmonicVariables.relativeRadiusPower[i] *
                (timedMagneticModel.mainFieldCoefficientsG[index] * spericalHarmonicVariables.sinMlambda[1] -
                timedMagneticModel.mainFieldCoefficientsH[index] * spericalHarmonicVariables.cosMlambda[1])
                * PcupS[i] * schmidtQuasiNorm3;
    }

    return magneticResultBy;
}

MagneticResults summation(const TimedMagneticModel& timedMagneticModel, const LegendreFunction& legendreFunction, 
                                              const SphericalHarmonicVariables& spericalHarmonicVariables, 
                                              const SphericalCoordinates& sphericalCoordinates) {
    /* Computes Geomagnetic Field Elements X, Y and Z in Spherical coordinate system using
    spherical harmonic sumation.


    The vector Magnetic field is given by -grad V, where V is Geomagnetic scalar potential
    The gradient in spherical coordinates is given by:

                     dV ^     1 dV ^        1     dV ^
    grad V = -- r  +  - -- t  +  -------- -- p
                     dr       r dt       r sin(t) dp

     */

    MagneticResults magneticResults; // use vector3d?
    int m, n, index;
    double cos_phi;
    magneticResults.Bz = 0.0;
    magneticResults.By = 0.0;
    magneticResults.Bx = 0.0;
    for(n = 1; n <= StaticMagneticModel::nMax; n++) { // use index i and j?
        for(m = 0; m <= n; m++) {
            index = (n * (n + 1) / 2 + m);

            /*		    nMax  	(n+2) 	  n     m            m           m
                    Bz =   -SUM (a/r)   (n+1) SUM  [g cos(m p) + h sin(m p)] P (sin(phi))
                                    n=1      	      m=0   n            n           n  */
            /* Equation 12 in the WMM Technical report.  Derivative with respect to radius.*/
            magneticResults.Bz -= spericalHarmonicVariables.relativeRadiusPower[n] *
                    (timedMagneticModel.mainFieldCoefficientsG[index] * spericalHarmonicVariables.cosMlambda[m] +   // using timeMagneticModel?
                    timedMagneticModel.mainFieldCoefficientsH[index] * spericalHarmonicVariables.sinMlambda[m])
                    * (double) (n + 1) * legendreFunction.Pcup[index];

            /*		  1 nMax  (n+2)    n     m            m           m
                    By =    SUM (a/r) (m)  SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1             m=0   n            n           n  */
            /* Equation 11 in the WMM Technical report. Derivative with respect to longitude, divided by radius. */
            magneticResults.By += spericalHarmonicVariables.relativeRadiusPower[n] *
                    (timedMagneticModel.mainFieldCoefficientsG[index] * spericalHarmonicVariables.sinMlambda[m] -
                    timedMagneticModel.mainFieldCoefficientsH[index] * spericalHarmonicVariables.cosMlambda[m])
                    * (double) (m) * legendreFunction.Pcup[index];
            /*		   nMax  (n+2) n     m            m           m
                    Bx = - SUM (a/r)   SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1         m=0   n            n           n  */
            /* Equation 10  in the WMM Technical report. Derivative with respect to latitude, divided by radius. */

            magneticResults.Bx -= spericalHarmonicVariables.relativeRadiusPower[n] *
                    (timedMagneticModel.mainFieldCoefficientsG[index] * spericalHarmonicVariables.cosMlambda[m] +
                    timedMagneticModel.mainFieldCoefficientsH[index] * spericalHarmonicVariables.sinMlambda[m]) // no cosMlambda and sinLambda correctly used?
                    * legendreFunction.derivedPcup[index];

        }
    }

    cos_phi = cos(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));
    const float threshold = 1.0e-10; // use constant threshold?

    /* Special calculation for component - By - at Geographic poles.
     * If the user wants to avoid using this function,  please make sure that
     * the latitude is not exactly +/-90. An option is to make use the function
     * MAG_CheckGeographicPoles.
     */
    if(fabs(cos_phi) > threshold) {magneticResults.By = magneticResults.By / cos_phi;} 
    else{magneticResults.By = summationSpecial(timedMagneticModel, spericalHarmonicVariables, sphericalCoordinates);}
    
    return magneticResults;
}

MagneticResults rotateMagneticVector(const SphericalCoordinates& sphericalCoordinates, const GeodeticCoordinates& geodeticCoordinates, const MagneticResults& magneticResultsInput) { // magneticResultsInput, look for a better name?
    const float Psi = (M_PI / 180) * (sphericalCoordinates.latitude - geodeticCoordinates.latitude); // Difference between the spherical and Geodetic latitudes, is it the conversion below?
    // const float phi EulerAngles::degreesToRadians(sphericalCoordinates.phig - geodeticCoordinates.phi); 

    // Rotate spherical field components to the Geodetic system
    MagneticResults magneticResultsGeo;
    magneticResultsGeo.Bz = magneticResultsInput.Bx * sin(Psi) + magneticResultsInput.Bz * cos(Psi);
    magneticResultsGeo.Bx = magneticResultsInput.Bx * cos(Psi) - magneticResultsInput.Bz * sin(Psi);
    magneticResultsGeo.By = magneticResultsInput.By;
    
    return magneticResultsGeo;
}

GeomagneticElements calculateGeoMagneticElements(const MagneticResults& magneticResultsGeo) {
    GeomagneticElements geomagneticElements;
    
    geomagneticElements.X = magneticResultsGeo.Bx;
    geomagneticElements.Y = magneticResultsGeo.By;
    geomagneticElements.Z = magneticResultsGeo.Bz;

    geomagneticElements.H = sqrt(magneticResultsGeo.Bx * magneticResultsGeo.Bx + magneticResultsGeo.By * magneticResultsGeo.By);
    geomagneticElements.F = sqrt(geomagneticElements.H * geomagneticElements.H + magneticResultsGeo.Bz * magneticResultsGeo.Bz);
    geomagneticElements.Decl = EulerAngles::radiansToDegrees(atan2(geomagneticElements.Y, geomagneticElements.X)); // all calls from the original WMM code of radians to degrees are ok, and from degreees to radians?
    geomagneticElements.Incl = EulerAngles::radiansToDegrees(atan2(geomagneticElements.Z, geomagneticElements.H));
    
    return geomagneticElements;
}

GeomagneticElements calculateGeomagneticResults(const TimedMagneticModel& timedMagneticModel,
                                                                    const SphericalCoordinates& sphericalCoordinates, 
                                                                    const GeodeticCoordinates& geodeticCoordinates) {
    SphericalHarmonicVariables spericalHarmonicVariables = computeSphericalHarmonicVariables(sphericalCoordinates);
    LegendreFunction legendreFunction = computedAssociatedLegendreFunction(sphericalCoordinates);
    
    //do if(legendre.attribute != 0 && spericalHarmonicVariables.attribute != 0 && ... to check that everything is alright)?
    const MagneticResults magneticResultsSph = summation(timedMagneticModel, legendreFunction, spericalHarmonicVariables, sphericalCoordinates); // Accumulate the spherical harmonic coefficients
    const MagneticResults magneticResultsGeo = rotateMagneticVector(sphericalCoordinates, geodeticCoordinates, magneticResultsSph); // Map the computed Magnetic fields to Geodeitic coordinates
    GeomagneticElements geomagneticElements = calculateGeoMagneticElements(magneticResultsGeo);

    return geomagneticElements;
}

GeomagneticElements calculateGeomagneticError(const double H) {
    const float WMM_UNCERTAINTY_F =  138;
    const float WMM_UNCERTAINTY_H =  133;
    const float WMM_UNCERTAINTY_X =  137;
    const float WMM_UNCERTAINTY_Y =  89;
    const float WMM_UNCERTAINTY_Z =  141;
    const float WMM_UNCERTAINTY_I =  0.20;
    const float WMM_UNCERTAINTY_D_COEF = 5417;

    GeomagneticElements geomagneticElementsUncertainty;
    geomagneticElementsUncertainty.F = WMM_UNCERTAINTY_F;
    geomagneticElementsUncertainty.H = WMM_UNCERTAINTY_H;
    geomagneticElementsUncertainty.X = WMM_UNCERTAINTY_X;
    geomagneticElementsUncertainty.Z = WMM_UNCERTAINTY_Z;
    geomagneticElementsUncertainty.Incl = WMM_UNCERTAINTY_I;
    geomagneticElementsUncertainty.Y = WMM_UNCERTAINTY_Y;
    const float declVariable = (WMM_UNCERTAINTY_D_COEF / H);
    const float declConstantUncerntainty = 0.26;
    geomagneticElementsUncertainty.Decl = sqrt(declConstantUncerntainty * declConstantUncerntainty + declVariable * declVariable);
    if(geomagneticElementsUncertainty.Decl > 180) {geomagneticElementsUncertainty.Decl = 180;}

    return geomagneticElementsUncertainty;
}


/*
// print results?
int MAG_Warnings(int control, double value, MAGtype_MagneticModel *MagneticModel) {
    switch(control) {
        case 1:// Horizontal Field strength low 
            printf("\nCaution: location is approaching the blackout zone around the magnetic pole. Compass accuracy may be degraded in this region");
            break;
        case 2:// Horizontal Field strength very low 
            printf("\nWarning: location is in the blackout zone around the magnetic pole. Compass accuracy is highly degraded in this region.");
            break;
        case 3:// Elevation outside the recommended range 
            printf("\nWarning: The value you have entered of %.1f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
            break;
        case 4:// Date outside the recommended range
            printf("\nWARNING - TIME EXTENDS BEYOND INTENDED USAGE RANGE\n CONTACT NCEI FOR PRODUCT UPDATES:\n");
            printf("\n VALID RANGE  = %d - %d\n", (int) MagneticModel->min_year, (int) MagneticModel->CoefficientFileEndDate);
            printf(" TIME   = %f\n", value);
            break;
		case 5:// Elevation outside the allowable range
		    printf("\nError: The value you have entered of %f km for the elevation is outside of the recommended range.\n Elevations above -10.0 km are recommended for accurate results. \n", value);
            break;
    }
    return 2;
}

void MAG_PrintUserDataWithUncertainty(MAGtype_GeoMagneticElements GeomagElements,
                                    MAGtype_GeoMagneticElements Errors,
                                    MAGtype_CoordGeodetic SpaceInput,
                                    MAGtype_Date TimeInput,
                                    MAGtype_MagneticModel *timedMagneticModel,
                                    MAGtype_Geoid *Geoid) {

    if(GeomagElements.H < 6000 && GeomagElements.H > 2000) {MAG_Warnings(1, GeomagElements.H, timedMagneticModel);}
    if(GeomagElements.H < 2000) {MAG_Warnings(2, GeomagElements.H, timedMagneticModel);}

    // if(magneticField from magneometers too high) {"warning("you are in a solar storm, magnetic model might not be accurate")"})

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
*/

Vector3d calculatePointMagneticField(const float latitude, const float longitude, const float altitude, const int year, const int month, const int day) {
    const GeodeticCoordinates geodeticCoordinates(latitude, longitude, altitude);
    const SphericalCoordinates sphericalCoordinates = convertGeodeticCoordinatesToSphericalcoordinates(geodeticCoordinates);
    const TimedMagneticModel timedMagneticModel = timelyModifyCoefficients(year, month, day); // timed magnetic model not needed?
    GeomagneticElements geomagneticElements = calculateGeomagneticResults(timedMagneticModel, sphericalCoordinates, geodeticCoordinates);

    GeomagneticElements geomagneticElementsErrors = calculateGeomagneticError(geomagneticElements.H);

    // add warnings to the object?
    //MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Print the results */
    // BOZ: blackoutzone https://www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-89500B.pdf
    //const char* BOZ_WARN_TEXT_STRONG = "Warning: some calculated locations are in the blackout zone around the magnetic pole. Compass accuracy is highly degraded in this region";
    //const char* BOZ_WARN_TEXT_WEAK = "Caution: some calculated locations approach the blackout zone around the magnetic pole. Compass accuracy may be degraded in this region";
    //const char* WMM_MileSpec_WARN = "Warning: WMM will not meet MilSpec at this altitude https://www.ncei.noaa.gov/products/world-magnetic-model/accuracy-limitations-error-model";
    //const char* WMM_MileSpec_INFO = "Warning: The height validity of the geomagnetic components is dependent on the geomagnetic activity level https://www.ncei.noaa.gov/products/world-magnetic-model/accuracy-limitations-error-model";
    //if(CoordGeodetic.HeightAboveEllipsoid < -1 || CoordGeodetic.HeightAboveEllipsoid > 1900) {printf("%s\n", WMM_MileSpec_WARN);}
    //else {printf(WMM_MileSpec_INFO);} 
    //if(GeoMagneticElements.H <= 2000.0) {printf(BOZ_WARN_TEXT_STRONG);} 
    //else if(GeoMagneticElements.H <= 6000.0) {printf(BOZ_WARN_TEXT_WEAK);}

    return geomagneticElements;
}

}