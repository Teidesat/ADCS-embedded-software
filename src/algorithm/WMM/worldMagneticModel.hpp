#pragma once

#include "constants/staticMagneticModel.hpp"
#include "../../lib/math/vector3d/vector3d.hpp"

#include <array>

namespace WMM {

class GeodeticCoordinates {
    public:
        float longitude = 0.0; //lambda?
        float latitude = 0.0; // phi?
        float HeightAboveEllipsoid = 0.0; //height above the ellipsoid (HaE)?
        float HeightAboveGeoid = 0.0; //height above the EGM96 geoid model?

        GeodeticCoordinates(const float newLatitude = 0.0, const float newLongitude = 0.0, const float newHeightAboveEllipsoid = 0.0, const float newHeightAboveGeoid = 0.0);
        // int UseGeoid = 0; // remove, use in corresponding function
};

class SphericalCoordinates {
    public:    
        float longitude = 0.0; //lambda?
        float latitude = 0.0; //phig?
        float radius = 0.0; //r?
};
// convert into template with numberOfterms as T?
class TimedMagneticModel {
    public:
        std::array<float, StaticMagneticModel::numberOfTerms> mainFieldCoefficientsG = {}; /* Gauss coefficients of main geomagnetic model (nT) */
        std::array<float, StaticMagneticModel::numberOfTerms> mainFieldCoefficientsH = {}; /* Gauss coefficients of main geomagnetic model (nT) */
        std::array<float, StaticMagneticModel::numberOfTerms> secularVarCoefficientsG = {}; /* Gauss coefficients of secular geomagnetic model (nT/yr) */
        std::array<float, StaticMagneticModel::numberOfTerms> secularVarCoefficientsH = {}; /* Gauss coefficients of secular geomagnetic model (nT/yr) */
        // int secularVariation = 0; // Whether or not the magnetic secular variation vector will be needed by program // to be removed
        int year = 0;
        int month = 0;
        int day = 0;
};

// convert into template with numberOfterms as T?
class SphericalHarmonicVariables {
    public:
        std::array<float, StaticMagneticModel::numberOfTerms + 1> relativeRadiusPower; // is it nmax or numberofterms, and +1 should be added?
        std::array<float, StaticMagneticModel::numberOfTerms + 1> cosMlambda;
        std::array<float, StaticMagneticModel::numberOfTerms + 1> sinMlambda;
};

// convert into template with numberOfterms as T?
class LegendreFunction {
    public:
        std::array<float, StaticMagneticModel::numberOfTerms + 1> Pcup;
        std::array<float, StaticMagneticModel::numberOfTerms + 1> derivedPcup;
};

class MagneticResults {
    public:
        float Bx = 0.0; // North
        float By = 0.0; // East
        float Bz = 0.0; // Down
};

class GeomagneticElements {
    public:
        float Decl; // Angle between the magnetic field vector and true north, positive east
        float Incl; // Angle between the magnetic field vector and the horizontal plane, positive down
        float F; // Magnetic Field Strength
        float H; // Horizontal Magnetic Field Strength
        float X; // Northern component of the magnetic field vector
        float Y; // Eastern component of the magnetic field vector
        float Z; // Downward component of the magnetic field vector
};


    
SphericalCoordinates convertGeodeticCoordinatesToSphericalcoordinates(const GeodeticCoordinates& geodeticCoordinates);
float dateToYears(const int year, const int month, const int day, const float epoch);
TimedMagneticModel timelyModifyCoefficients(const int year, const int month, const int day);
        
SphericalHarmonicVariables computeSphericalHarmonicVariables(const SphericalCoordinates& CoordSpherical);
        
LegendreFunction computePcupLow(const float sineOfLatitude);
LegendreFunction computePcupHigh(const float sineOfLatitude);
LegendreFunction computedAssociatedLegendreFunction(const SphericalCoordinates& sphericalCoordinates);

float summationSpecial(const TimedMagneticModel& timedMagneticModel, const SphericalHarmonicVariables& spericalHarmonicVariables, const SphericalCoordinates& sphericalCoordinates);
MagneticResults summation(const TimedMagneticModel& timedMagneticModel, const LegendreFunction& legendreFunction, const SphericalHarmonicVariables& spericalHarmonicVariables, const SphericalCoordinates& sphericalCoordinates);
        
MagneticResults rotateMagneticVector(const SphericalCoordinates& sphericalCoordinates, const GeodeticCoordinates& geodeticCoordinates, const MagneticResults& magneticResultsInput);
GeomagneticElements calculateGeoMagneticElements(const MagneticResults& magneticResultsGeo);

GeomagneticElements calculateGeomagneticError(const double H);

GeomagneticElements calculateGeomagneticResults(const TimedMagneticModel& timedMagneticModel, const SphericalCoordinates& CoordSpherical, const GeodeticCoordinates& CoordGeodetic);

GeomagneticElements calculatePointMagneticField(const float latitude, const float longitude,const float altitude,const int year,const int month,const int day);

}