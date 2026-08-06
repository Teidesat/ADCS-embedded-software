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

Vector3d WorldMagneticModel::calculateExpectedMagneticField(const float latitude, 
                                                            const float longitude, 
                                                            const float altitude, 
                                                            const int publishDateYear, 
                                                            const int publishDateMonth, 
                                                            const int publishDateDay) {
    const GeodeticCoordinates geodeticCoordinates(latitude, longitude, altitude);
    const SphericalCoordinates sphericalCoordinates = convertGeodeticCordinatesToSphericalcoordinates(geodeticCoordinates);

    // Calculate the magnetic field using the WMM model
    TimedMagneticModel timedMagneticModel;
    timedMagneticModel.currentDateYear = publishDateYear;
    timedMagneticModel.currentDateMonth = publishDateMonth;
    timedMagneticModel.currentDateDay = publishDateDay;
    timedMagneticModel.currentDecimalDate = WMM::MagneticModel::dateToYears(publishDateYear, publishDateMonth, publishDateDay, WMM::MagneticModel::epoch);
    //call MAG_TimelyModifyMagneticModel
}

SphericalCoordinates WorldMagneticModel::convertGeodeticCordinatesToSphericalcoordinates(const GeodeticCoordinates& geodeticCoordinates) {
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

}