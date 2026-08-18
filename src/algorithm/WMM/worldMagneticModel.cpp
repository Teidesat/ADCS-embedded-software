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

SphericalCoordinates WorldMagneticModel::convertGeodeticCoordinatesToSphericalcoordinates(const GeodeticCoordinates& geodeticCoordinates) {
    float latitudeCosine, latitudeSine, rc, xp, zp;
    SphericalCoordinates sphericalCoordinates;

    latitudeCosine = cos(EulerAngles::degreesToRadians(geodeticCoordinates.latitude));
    latitudeSine = sin(EulerAngles::degreesToRadians(geodeticCoordinates.latitude));

    rc = Ellipsoid::semiMajorAxis / sqrt(1.0 - Ellipsoid::firstEccentricitySquared * latitudeSine * latitudeSine);
    xp = (rc + geodeticCoordinates.HeightAboveEllipsoid) * latitudeCosine;
    zp = (rc * (1.0 - Ellipsoid::firstEccentricitySquared) + geodeticCoordinates.HeightAboveEllipsoid) * latitudeSine;

    sphericalCoordinates.radius = sqrt(xp * xp + zp * zp);
    sphericalCoordinates.latitude = EulerAngles::radiansToDegrees(asin(zp / sphericalCoordinates.radius));
    sphericalCoordinates.longitude = geodeticCoordinates.longitude; 

    return sphericalCoordinates;
}

float WorldMagneticModel::dateToYears(const int year, const int month, const int day, const float epoch) {  //check that month day and year are valid?
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

TimedMagneticModel WorldMagneticModel::timelyModifyCoefficients(const int year, const int month, const int day) {
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


SphericalHarmonicVariables WorldMagneticModel::computeSphericalHarmonicVariables(const SphericalCoordinates& CoordSpherical) {
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
    if (StaticMagneticModel::numberOfTerms + 1 >= 2){
        sphericalHarmonicVariables.cosMlambda[1] = cosLongitude;
        sphericalHarmonicVariables.sinMlambda[1] = sinLongitude;
    }
   
    for(int i = 2; i <= StaticMagneticModel::numberOfTerms; i++) {
        const int iMinusOne = i - 1;
        sphericalHarmonicVariables.cosMlambda[i] = sphericalHarmonicVariables.cosMlambda[iMinusOne] * cosLongitude - sphericalHarmonicVariables.sinMlambda[iMinusOne] * sinLongitude;
        sphericalHarmonicVariables.sinMlambda[i] = sphericalHarmonicVariables.cosMlambda[iMinusOne] * sinLongitude + sphericalHarmonicVariables.sinMlambda[iMinusOne] * cosLongitude;
    }

    return sphericalHarmonicVariables;
}

LegendreFunction WorldMagneticModel::computePcupLow(const float sineOfLatitude) {
    // Compute the Gauss-normalized associated Legendre functions
    const float cosineOfLatitude = sqrt((1.0 - sineOfLatitude) * (1.0 + sineOfLatitude));
    LegendreFunction legendreFunction;
    legendreFunction.Pcup[0] = 1.0;
    legendreFunction.derivedPcup[0] = 0.0;

    for(int i = 1; i <= StaticMagneticModel::numberOfTerms; i++) {
        for(int j = 0; j <= i; j++) {
            const int pyramidIndex = i * (i + 1) / 2 + j;
            if(i == j) {
                const int pyramidIndex = (i - 1) * i / 2 + j - 1;
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
LegendreFunction WorldMagneticModel::computePcupHigh(const float sineOfLatitude) {
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
LegendreFunction WorldMagneticModel::computedAssociatedLegendreFunction(const SphericalCoordinates& sphericalCoordinates) {
    const float threshold = 1.0e-10; // used in other places already, store it in file for constants threshold?
    LegendreFunction legendre;
    float sineOfLatitude = sin(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));
    if(1 - fabs(sineOfLatitude) < threshold) {legendre = computePcupLow(sineOfLatitude);}   // If nMax is less tha 16 or at the poles and Overflow may occur if nMax > 20?
    else {legendre = computePcupHigh(sineOfLatitude);}

    return legendre;
}

float summationSpecial(const SphericalHarmonicVariables& spericalHarmonicVariablesphVariables, const SphericalCoordinates& sphericalCoordinates) {
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
        magneticResultBy += spericalHarmonicVariablesphVariables.relativeRadiusPower[i] *
                (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariablesphVariables.sinMlambda[1] -
                StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariablesphVariables.cosMlambda[1])
                * PcupS[i] * schmidtQuasiNorm3;
    }

    return magneticResultBy;
}

MagneticResults WorldMagneticModel::summation(const LegendreFunction& legendreFunction, 
                                              const SphericalHarmonicVariables& spericalHarmonicVariablesphVariables, 
                                              const SphericalCoordinates& sphericalCoordinates) {
    /* Computes Geomagnetic Field Elements X, Y and Z in Spherical coordinate system using
    spherical harmonic summation.


    The vector Magnetic field is given by -grad V, where V is Geomagnetic scalar potential
    The gradient in spherical coordinates is given by:

                     dV ^     1 dV ^        1     dV ^
    grad V = -- r  +  - -- t  +  -------- -- p
                     dr       r dt       r sin(t) dp

     */

    MagneticResults magneticResults;
    int m, n, index;
    double cos_phi;
    magneticResults.Bz = 0.0;
    magneticResults.By = 0.0;
    magneticResults.Bx = 0.0;
    for(n = 1; n <= StaticMagneticModel::nMax; n++) {
        for(m = 0; m <= n; m++) {
            index = (n * (n + 1) / 2 + m);

            /*		    nMax  	(n+2) 	  n     m            m           m
                    Bz =   -SUM (a/r)   (n+1) SUM  [g cos(m p) + h sin(m p)] P (sin(phi))
                                    n=1      	      m=0   n            n           n  */
            /* Equation 12 in the WMM Technical report.  Derivative with respect to radius.*/
            magneticResults.Bz -= spericalHarmonicVariablesphVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariablesphVariables.cosMlambda[m] +
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariablesphVariables.sinMlambda[m])
                    * (double) (n + 1) * legendreFunction.Pcup[index];

            /*		  1 nMax  (n+2)    n     m            m           m
                    By =    SUM (a/r) (m)  SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1             m=0   n            n           n  */
            /* Equation 11 in the WMM Technical report. Derivative with respect to longitude, divided by radius. */
            magneticResults.By += spericalHarmonicVariablesphVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariablesphVariables.sinMlambda[m] -
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariablesphVariables.cosMlambda[m])
                    * (double) (m) * legendreFunction.Pcup[index];
            /*		   nMax  (n+2) n     m            m           m
                    Bx = - SUM (a/r)   SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1         m=0   n            n           n  */
            /* Equation 10  in the WMM Technical report. Derivative with respect to latitude, divided by radius. */

            magneticResults.Bx -= spericalHarmonicVariablesphVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariablesphVariables.cosMlambda[m] +
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariablesphVariables.sinMlambda[m])
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
    else{magneticResults.By = summationSpecial(spericalHarmonicVariablesphVariables, sphericalCoordinates);}
    
    return magneticResults;
}

// Special calculation for the secular variation summation at the poles.
float secVarSummationSpecial(const SphericalHarmonicVariables& spericalHarmonicVariablesphVariables, const SphericalCoordinates& sphericalCoordinates) {

    std::array<float, StaticMagneticModel::nMax + 1> PcupS = {};
    PcupS[0] = 1;
    
    float By = 0.0;
    const float schmidtQuasiNorm1 = 1.0;
    const float sin_phi = sin(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));

    for(int i = 2; i <= StaticMagneticModel::nMax; i++) {
        const int index = (i * (i + 1) / 2 + 1);
        const float schmidtQuasiNorm2 = schmidtQuasiNorm1 * (2 * i - 1) /  i;
        const float schmidtQuasiNorm3 = schmidtQuasiNorm2 * sqrt((i * 2) /  (i + 1));
        const float schmidtQuasiNorm1 = schmidtQuasiNorm2;
        if(i == 1) {PcupS[i] = PcupS[i - 1];}    // PcupS[0] = 1, but then PcupS[1] = PcupS[1 - 1]?
        else {
            const float k = (((i - 1) * (i - 1)) - 1) / ((2 * i - 1) * (2 * i - 3));
            PcupS[i] = sin_phi * PcupS[i - 1] - k * PcupS[i - 2];
        }

        /*		  1 nMax  (n+2)    n     m            m           m
                By =    SUM (a/r) (m)  SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                           n=1             m=0   n            n           n  */
        /* Derivative with respect to longitude, divided by radius. */

        By += spericalHarmonicVariablesphVariables.relativeRadiusPower[n] *
                (StaticMagneticModel::secularVarCoefficientsG[index] * spericalHarmonicVariablesphVariables.sin_mlambda[1] -
                StaticMagneticModel::secularVarCoefficientsH[index] * spericalHarmonicVariablesphVariables.sinMlambda[1])
                * PcupS[n] * schmidtQuasiNorm3;
    }

    return By;
}

MagneticResults SecVarSummation(const LegendreFunction& legendreFunction,
                                const SphericalHarmonicVariables& spericalHarmonicVariables,
                                const SphericalCoordinates& sphericalCoordinates) {
    /*This Function sums the secular variation coefficients to get the secular variation of the Magnetic vector.*/
    
    MagneticResults magneticResults;
    //magneticResults.SecularVariationUsed = 1;  // what to do with this?
    magneticResults.Bz = 0.0;
    magneticResults.By = 0.0;
    magneticResults.Bx = 0.0;

    int m, n, index;
    double cos_phi;

    for(n = 1; n <= StaticMagneticModel::nMax; n++) {
        for(m = 0; m <= n; m++) {
            index = (n * (n + 1) / 2 + m);

            /*		    nMax  	(n+2) 	  n     m            m           m
                    Bz =   -SUM (a/r)   (n+1) SUM  [g cos(m p) + h sin(m p)] P (sin(phi))
                                    n=1      	      m=0   n            n           n  */
            /*  Derivative with respect to radius.*/
            magneticResults.Bz -= spericalHarmonicVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariables.cosMlambda[m] +
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariables.sinMlambda[m])
                    * (double) (n + 1) * legendreFunction.Pcup[index];

            /*		  1 nMax  (n+2)    n     m            m           m
                    By =    SUM (a/r) (m)  SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1             m=0   n            n           n  */
            /* Derivative with respect to longitude, divided by radius. */
            magneticResults.By += spericalHarmonicVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariables.sinMlambda[m] -
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariables.cosMlambda[m])
                    * (double) (m) * legendreFunction.Pcup[index];
            /*		   nMax  (n+2) n     m            m           m
                    Bx = - SUM (a/r)   SUM  [g cos(m p) + h sin(m p)] dP (sin(phi))
                               n=1         m=0   n            n           n  */
            /* Derivative with respect to latitude, divided by radius. */

            magneticResults.Bx -= spericalHarmonicVariables.relativeRadiusPower[n] *
                    (StaticMagneticModel::mainFieldCoefficientsG[index] * spericalHarmonicVariables.cosMlambda[m] +
                    StaticMagneticModel::mainFieldCoefficientsH[index] * spericalHarmonicVariables.sinMlambda[m])
                    * legendreFunction.derivedPcup[index];
        }
    }
    cos_phi = cos(EulerAngles::degreesToRadians(sphericalCoordinates.latitude));
    // threshold constant here as well?
    if(fabs(cos_phi) > 1.0e-10) {magneticResults.By = magneticResults.By / cos_phi;} // Special calculation for component By at Geographic poles 
    else {magneticResults.By = secVarSummationSpecial(spericalHarmonicVariables, sphericalCoordinates);}
    return magneticResults;
}

MagneticResults WorldMagneticModel::rotateMagneticVector(const SphericalCoordinates& sphericalCoordinates, const GeodeticCoordinates& geodeticCoordinates, const MagneticResults& magneticResultsInput) { // magneticResultsInput, look for a better name?
    const float Psi = (M_PI / 180) * (sphericalCoordinates.latitude - geodeticCoordinates.latitude); // Difference between the spherical and Geodetic latitudes, is it the conversion below?
    // const float phi EulerAngles::degreesToRadians(sphericalCoordinates.phig - geodeticCoordinates.phi); 

    // Rotate spherical field components to the Geodetic system
    MagneticResults magneticResultsGeo;
    magneticResultsGeo.Bz = magneticResultsInput.Bx * sin(Psi) + magneticResultsInput.Bz * cos(Psi);
    magneticResultsGeo.Bx = magneticResultsInput.Bx * cos(Psi) - magneticResultsInput.Bz * sin(Psi);
    magneticResultsGeo.By = magneticResultsInput.By;
    
    return magneticResultsGeo;
}

GeomagneticElements WorldMagneticModel::calculateGeoMagneticElements(const MagneticResults& magneticResultsGeo, const MagneticResults& magneticResultsGeoVar) {
    GeomagneticElements geomagneticElements;
    
    geomagneticElements.X = magneticResultsGeo.Bx;
    geomagneticElements.Y = magneticResultsGeo.By;
    geomagneticElements.Z = magneticResultsGeo.Bz;

    geomagneticElements.H = sqrt(magneticResultsGeo.Bx * magneticResultsGeo.Bx + magneticResultsGeo.By * magneticResultsGeo.By);
    geomagneticElements.F = sqrt(geomagneticElements.H * geomagneticElements.H + magneticResultsGeo.Bz * magneticResultsGeo.Bz);
    geomagneticElements.Decl = EulerAngles::radiansToDegrees(atan2(geomagneticElements.Y, geomagneticElements.X));
    geomagneticElements.Incl = EulerAngles::radiansToDegrees(atan2(geomagneticElements.Z, geomagneticElements.H));

    // secular variation elements
    geomagneticElements.Xdot = magneticResultsGeoVar.Bx;
    geomagneticElements.Ydot = magneticResultsGeoVar.By;
    geomagneticElements.Zdot = magneticResultsGeoVar.Bz;
    geomagneticElements.Hdot = (geomagneticElements.X * geomagneticElements.Xdot + geomagneticElements.Y * geomagneticElements.Ydot) / geomagneticElements.H;
    geomagneticElements.Fdot = (geomagneticElements.X * geomagneticElements.Xdot + geomagneticElements.Y * geomagneticElements.Ydot + geomagneticElements.Z * geomagneticElements.Zdot) / geomagneticElements.F;
    geomagneticElements.Decldot = 180.0 / M_PI * (geomagneticElements.X * geomagneticElements.Ydot - geomagneticElements.Y * geomagneticElements.Xdot) / (geomagneticElements.H * geomagneticElements.H); // use conversion function?
    geomagneticElements.Incldot = 180.0 / M_PI * (geomagneticElements.H * geomagneticElements.Zdot - geomagneticElements.Z * geomagneticElements.Hdot) / (geomagneticElements.F * geomagneticElements.F); // use conversion function?
    geomagneticElements.GVdot = geomagneticElements.Decldot;


    return geomagneticElements;
}

GeomagneticElements WorldMagneticModel::calculateGeomagneticResults(const SphericalCoordinates& sphericalCoordinates, 
                                                                    const GeodeticCoordinates& geodeticCoordinates) {
    SphericalHarmonicVariables spericalHarmonicVariables = computeSphericalHarmonicVariables(sphericalCoordinates);
    LegendreFunction legendreFunction = computedAssociatedLegendreFunction(sphericalCoordinates);
    
    //do if(legendre.attribute != 0 && spericalHarmonicVariables.attribute != 0 && ... to check that everything is alright)
    const MagneticResults magneticResultsSph = summation(legendreFunction, spericalHarmonicVariables, sphericalCoordinates); // Accumulate the spherical harmonic coefficients
    const MagneticResults magneticResultsSphVar = SecVarSummation(legendreFunction, spericalHarmonicVariables, sphericalCoordinates); //Sum the Secular Variation Coefficients
    const MagneticResults magneticResultsGeo = rotateMagneticVector(sphericalCoordinates, geodeticCoordinates, magneticResultsSph); // Map the computed Magnetic fields to Geodeitic coordinates
    const MagneticResults magneticResultsGeoVar = rotateMagneticVector(sphericalCoordinates, geodeticCoordinates, magneticResultsSphVar); // Map the secular variation field components to Geodetic coordinates
    GeomagneticElements geomagneticElements = calculateGeoMagneticElements(magneticResultsGeo, magneticResultsGeoVar); // Calculate the Geomagnetic elements, Equation 19 , WMM Technical report

    return geomagneticElements;
}

GeomagneticElements WorldMagneticModel::calculateGeomagneticError(const double H) {
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

// translating point_calc
Vector3d WorldMagneticModel::calculatePointMagneticField(const float latitude, const float longitude, const float altitude, const int year, const int month, const int day) {
    const GeodeticCoordinates geodeticCoordinates(latitude, longitude, altitude);
    const SphericalCoordinates sphericalCoordinates = convertGeodeticCoordinatesToSphericalcoordinates(geodeticCoordinates);
    const TimedMagneticModel timedMagneticModel = timelyModifyCoefficients(year, month, day);
    GeomagneticElements geomagneticElements = calculateGeomagneticResults(sphericalCoordinates, geodeticCoordinates);

    //MAG_CalculateGridVariation(CoordGeodetic, GeoMagneticElements);

    GeomagneticElements geomagneticElementsErrors = calculateGeomagneticError(geomagneticElements.H);



    return Vector3d(0, 0, 0);
}

}