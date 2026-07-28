class SphericalCoordinates {
    public:    
        double longitude = 0.0; //lambda
        double geocentricLatitude = 0.0;//phig
        double radius = 0.0; //r  /* distance from the center of the ellipsoid*/
};

class GeodeticCoordinates {
    public:
        double longitude = 0.0; // lambda
        double geodeticLatitude = 0.0; /* geodetic latitude */
        double HeightAboveEllipsoid = 0.0; /* height above the ellipsoid (HaE) */
        double HeightAboveGeoid = 0.0; /* (height above the EGM96 geoid model ) */
        int UseGeoid = 0;
};

class Date {
    public:
        int year = 0;
        int month = 0;
        int day = 0;
        double decimalYear = 0.0;
};

class GeomagneticElements{
    public:
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
};