#pragma once

#include "constants/magneticModel.hpp"
#include "../../lib/math/vector3d/vector3d.hpp"

#include <array>

namespace WMM {

class TimedMagneticModel {
    public:
        std::array<float, MagneticModel::numberOfCoefficients> mainFieldCoefficientsG = {}; /* C - Gauss coefficients of main geomagnetic model (nT) Index is (n * (n + 1) / 2 + m) */
        std::array<float, MagneticModel::numberOfCoefficients> mainFieldCoefficientsH = {}; /* C - Gauss coefficients of main geomagnetic model (nT) */
        std::array<float, MagneticModel::numberOfCoefficients> secularVarCoefficientsG = {}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
        std::array<float, MagneticModel::numberOfCoefficients> secularVarCoefficientsH = {}; /* CD - Gauss coefficients of secular geomagnetic model (nT/yr) */
        // int secularVariation = 0; // Whether or not the magnetic secular variation vector will be needed by program // to be removed
        float currentDecimalDate = 0;
        int currentDateYear = 0;
        int currentDateMonth = 0;
        int currentDateDay = 0;
};

class SphericalCoordinates {
    public:    
        float longitude = 0.0; //lambda
        float geodetic = 0.0;//phig 
        float radius = 0.0; //r  //distance from the center of the ellipsoid
};

    double lambda; /* longitude */
    double phi; /* geodetic latitude */
    double HeightAboveEllipsoid; /* height above the ellipsoid (HaE) */       // both values ar given by the GPS, geodetic and ellipsoid altitudes
    double HeightAboveGeoid; /* (height above the EGM96 geoid model ) */
    int UseGeoid;

class GeodeticCoordinates {
    public:
        float longitude = 0.0; //lambda
        float latitude = 0.0; //geodetic latitude // phi
        float HeightAboveEllipsoid = 0.0; //height above the ellipsoid (HaE)
        float HeightAboveGeoid = 0.0; //height above the EGM96 geoid model

        GeodeticCoordinates(const float newLatitude = 0.0, const float newLongitude = 0.0, const float newHeightAboveEllipsoid = 0.0, const float newHeightAboveGeoid = 0.0);
        // int UseGeoid = 0; // remove, use in corresponding function
};

class GeomagneticElements{
    public:
        float Decl; /* 1. Angle between the magnetic field vector and true north, positive east*/
        float Incl; /*2. Angle between the magnetic field vector and the horizontal plane, positive down*/
        float F; /*3. Magnetic Field Strength*/
        float H; /*4. Horizontal Magnetic Field Strength*/
        float X; /*5. Northern component of the magnetic field vector*/
        float Y; /*6. Eastern component of the magnetic field vector*/
        float Z; /*7. Downward component of the magnetic field vector*/
        float GV; /*8. The Grid Variation*/
        float Decldot; /*9. Yearly Rate of change in declination*/
        float Incldot; /*10. Yearly Rate of change in inclination*/
        float Fdot; /*11. Yearly rate of change in Magnetic field strength*/
        float Hdot; /*12. Yearly rate of change in horizontal field strength*/
        float Xdot; /*13. Yearly rate of change in the northern component*/
        float Ydot; /*14. Yearly rate of change in the eastern component*/
        float Zdot; /*15. Yearly rate of change in the downward component*/
        float GVdot; /*16. Yearly rate of change in grid variation*/
};


class WorldMagneticModel {
    public:
        static Vector3d calculateExpectedMagneticField(const float latitude,
                                                       const float longitude,
                                                       const float altitude,
                                                       const int publishDateYear,
                                                       const int publishDateMonth,
                                                       const int publishDateDay); 
    private:
        static SphericalCoordinates convertGeodeticCordinatesToSphericalcoordinates(const GeodeticCoordinates& geodeticCoordinates);

};

}