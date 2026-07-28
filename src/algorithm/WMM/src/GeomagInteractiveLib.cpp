#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

void MAG_clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Simply read and discard each character
    }
}
int MAG_ValidateDMSstring(char *input, int min, int max, char *Error)

/* Validates a latitude DMS string, and returns 1 for a success and returns 0 for a failure.
It copies an error message to the Error string in the event of a failure.

INPUT : input (DMS string)
OUTPUT : Error : Error string
CALLS : none
 */
{
    int degree, minute, second, j = 0, n, max_minute = 60, max_second = 60;
    int i;
    degree = -1000;
    minute = -1;
    second = -1;
    n = (int) strlen(input);
    int Error_size = 255;

    for(i = 0; i <= n - 1; i++) /*tests for legal characters*/
    {
        if((input[i] < '0' || input[i] > '9') && (input[i] != ',' && input[i] != ' ' && input[i] != '-' && input[i] != '\0' && input[i] != '\n'))
        {
          //  The Error is passed as char pointer from MAG_GetDeg(), so I can't use "sizeof()" to estimate its size. The size of Error is 255 and defined in MAG_GetDeg(). MAG_GetDeg() is the only function which will call MAG_ValidateDMSstring().
          MAG_strlcpy_equivalent(Error, "\nError: Input contains an illegal character, legal characters for Degree, Minute, Second format are:\n '0-9' ',' '-' '[space]' '[Enter]'\n", Error_size);
            return false;
        }
        if(input[i] == ',')
            j++;
    }
    if(j == 2)
        j = sscanf(input, "%d, %d, %d", &degree, &minute, &second); /*tests for legal formatting and range*/
    else
        j = sscanf(input, "%d %d %d", &degree, &minute, &second);
    if(j == 1)
    {
        minute = 0;
        second = 0;
        j = 3;
    }
    if(j != 3)
    {
        MAG_strlcpy_equivalent(Error, "\nError: Not enough numbers used for Degrees, Minutes, Seconds format\n or they were incorrectly formatted\n The legal format is DD,MM,SS or DD MM SS\n", Error_size);
        return false;
    }
    if(degree > max || degree < min)
    {
        sprintf(Error, "\nError: Degree input is outside legal range\n The legal range is from %d to %d\n", min, max);
        return false;
    }
    if(degree == max || degree == min)
        max_minute = 0;
    if(minute > max_minute || minute < 0)
    {
        MAG_strlcpy_equivalent(Error, "\nError: Minute input is outside legal range\n The legal minute range is from 0 to 60\n", Error_size);
        return false;
    }
    if(minute == max_minute)
        max_second = 0;
    if(second > max_second || second < 0)
    {
        MAG_strlcpy_equivalent(Error, "\nError: Second input is outside legal range\n The legal second range is from 0 to 60\n", Error_size);
        return false;
    }
    return true;
} /*MAG_ValidateDMSstring*/

void MAG_GetDeg(char* Query_String, double* latitude, double bounds[2]) {
	/*Gets a degree value from the user using the standard input*/
	char buffer[64], Error_Message[255];
	int done, i, j;
	
	printf("%s", Query_String);
    while (NULL == fgets(buffer, sizeof(buffer), stdin)){
        printf("%s", Query_String);
        if (buffer[sizeof(buffer) - 1] != '\n'){
            MAG_clear_input_buffer(); // Remove the left characters from stdin if the buffer is not able to read the whole stdin
        }    
    }
     
    for(i = 0, done = 0, j = 0; i < (int) sizeof(buffer) && !done; i++)
    {
        if(buffer[i] == '.')
        {
            j = sscanf(buffer, "%lf", latitude);
            if(j == 1)
                done = 1;
            else
                done = -1;
        }
        if(buffer[i] == ',')
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message))
            {
                MAG_DMSstringToDegree(buffer, latitude);
                done = 1;
            } else
                done = -1;
        }
        if(buffer[i] == ' ')/* This detects if there is a ' ' somewhere in the string,
		if there is the program tries to interpret the input as Degrees Minutes Seconds.*/
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message))
            {
                MAG_DMSstringToDegree(buffer, latitude);
                done = 1;
            } else
                done = -1;
        }
        if(buffer[i] == '\0' || done == -1)
        {
            if(MAG_ValidateDMSstring(buffer, bounds[0], bounds[1], Error_Message) && done != -1)
            {
                sscanf(buffer, "%lf", latitude);
                done = 1;
            } else
            {
                printf("%s", Error_Message);
                MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
                printf("\nError encountered, please re-enter as '(-)DDD,MM,SS' or in Decimal Degrees DD.ddd:\n");
                while(NULL == fgets(buffer, sizeof(buffer), stdin)) {
                    printf("\nError encountered, please re-enter as '(-)DDD,MM,SS' or in Decimal Degrees DD.ddd:\n");
                    if (buffer[sizeof(buffer) - 1] != '\n'){
                        MAG_clear_input_buffer(); // Remove the left characters from stdin if the buffer is not able to read the whole stdin
                    }
                }
                 
                i = -1;
                done = 0;
            }
        }
    }
}

int MAG_GetAltitude(char* Query_String, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic* coords, int bounds[2], int AltitudeSetting){
	int done, j, UpBoundOn;
	char tmp;
	char buffer[64];
	double value;
	done = 0;
    if(bounds[1] != NO_ALT_MAX){
        UpBoundOn = true;    
    } else {
        UpBoundOn = false;
    }
    printf("%s", Query_String);
	
    while(!done)
    {
        MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
        while(NULL == fgets(buffer, sizeof(buffer), stdin)) {
            printf("%s", Query_String);
        }
        j = 0;
        if((AltitudeSetting != MSLON) && (buffer[0] == 'e' || buffer[0] == 'E' || AltitudeSetting == WGS84ON))  {
            /* User entered height above WGS-84 ellipsoid, copy it to CoordGeodetic->HeightAboveEllipsoid */
			if(buffer[0]=='e' || buffer[0]=='E') {
				j = sscanf(buffer, "%c%lf", &tmp, &coords->HeightAboveEllipsoid);
			} else {
				j = sscanf(buffer, "%lf", &coords->HeightAboveEllipsoid);
			}
            if(j == 2)
                j = 1;
            Geoid->UseGeoid = 0;
            coords->HeightAboveGeoid = coords->HeightAboveEllipsoid;
			value = coords->HeightAboveEllipsoid;
        } 
        else {
            /* User entered height above MSL, convert it to the height above WGS-84 ellipsoid */
            Geoid->UseGeoid = 1;
            j = sscanf(buffer, "%lf", &coords->HeightAboveGeoid);
            MAG_ConvertGeoidToEllipsoidHeight(coords, Geoid);
			value = coords->HeightAboveGeoid;
        }
        if(j == 1)
            done = 1;
        else
            printf("\nIllegal Format, please re-enter as '(-)HHH.hhh:'\n");
        if((value < bounds[0] || (value > bounds[1] && UpBoundOn)) && done == 1) {
			if(UpBoundOn) {
				done = 0;
				printf("\nWarning: The value you have entered of %f km for the elevation is outside of the required range.\n", value);
				printf(" An elevation between %d km and %d km is needed. \n", bounds[0], bounds[1]);
				if (AltitudeSetting == WGS84ON){
				    printf("Please enter height above WGS-84 Ellipsoid (in kilometers):\n");
				} else if (AltitudeSetting==MSLON){
				    printf("Please enter height above mean sea level (in kilometers):\n");
				} else {
				    printf("Please enter height in kilometers (prepend E for height above WGS-84 Ellipsoid):");
				}
			} else {
				switch(MAG_Warnings(3, value, NULL)) {
					case 0:
						return USER_GAVE_UP;
					case 1:
						done = 0;
						printf("Please enter height above sea level (in kilometers):\n");
						break;
					case 2:
						break;
				}
            }
        }
    }
    return 0;
}


int MAG_GetUserInput(MAGtype_MagneticModel *MagneticModel, MAGtype_Geoid *Geoid, MAGtype_CoordGeodetic *CoordGeodetic, MAGtype_Date *MagneticDate)

/*
This prompts the user for coordinates, and accepts many entry formats.
It takes the MagneticModel and Geoid as input and outputs the Geographic coordinates and Date as objects.
Returns 0 when the user wants to exit and 1 if the user enters valid input data.
INPUT :  MagneticModel  : Data structure with the following elements used here
                        double epoch;       Base time of Geomagnetic model epoch (yrs)
                : Geoid Pointer to data structure MAGtype_Geoid (used for converting HeightAboveGeoid to HeightABoveEllipsoid

OUTPUT: CoordGeodetic : Pointer to data structure. Following elements are updated
                        double lambda; (longitude)
                        double phi; ( geodetic latitude)
                        double HeightAboveEllipsoid; (height above the ellipsoid (HaE) )
                        double HeightAboveGeoid;(height above the Geoid )

                MagneticDate : Pointer to data structure MAGtype_Date with the following elements updated
                        int	Year; (If user directly enters decimal year this field is not populated)
                        int	Month;(If user directly enters decimal year this field is not populated)
                        int	Day; (If user directly enters decimal year this field is not populated)
                        double DecimalYear;      decimal years

CALLS: 	MAG_DMSstringToDegree(buffer, &CoordGeodetic->lambda); (The program uses this to convert the string into a decimal longitude.)
                MAG_ValidateDMSstringlong(buffer, Error_Message)
                MAG_ValidateDMSstringlat(buffer, Error_Message)
                MAG_Warnings
                MAG_ConvertGeoidToEllipsoidHeight
                MAG_DateToYear

 */
{
    char Error_Message[255];
    char buffer[40];
    buffer[sizeof(buffer) - 1] = '\0';
    int i, j, a, b, c, done = 0;
	double lat_bound[2] = {LAT_BOUND_MIN, LAT_BOUND_MAX};
	double lon_bound[2] = {LON_BOUND_MIN, LON_BOUND_MAX};
    int alt_bound[2] = {ALT_BOUND_MIN, NO_ALT_MAX}; 
	int Qstring_size = 1028;
    char* Qstring = malloc(sizeof(char) * Qstring_size);
    memset(Qstring, '\0', Qstring_size); 
    MAG_strlcpy_equivalent(Qstring, "\nPlease enter latitude\nNorth latitude positive, For example:\n30, 30, 30 (D,M,S) or 30.508 (Decimal Degrees) (both are north)\n", Qstring_size);
	MAG_GetDeg(Qstring, &CoordGeodetic->phi, lat_bound);
    MAG_strlcpy_equivalent(buffer, "", sizeof(buffer)); /*Clear the input*/
    memset(Qstring, '\0', Qstring_size); // Clear QString to avoid show the string copied from previous time
    MAG_strlcpy_equivalent(Qstring,"\nPlease enter longitude\nEast longitude positive, West negative.  For example:\n-100.5 or -100, 30, 0 for 100.5 degrees west\n", Qstring_size);
	MAG_GetDeg(Qstring, &CoordGeodetic->lambda, lon_bound);
	
    memset(Qstring, '\0', Qstring_size);    
	MAG_strlcpy_equivalent(Qstring,"\nPlease enter height above mean sea level (in kilometers):\n[For height above WGS-84 ellipsoid prefix E, for example (E20.1)]\n", Qstring_size);
    if(MAG_GetAltitude(Qstring, Geoid, CoordGeodetic, alt_bound, false)==USER_GAVE_UP) {return false;}
    MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
    printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
    while (NULL == fgets(buffer, sizeof(buffer), stdin)) {
        printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
        
    }
     
    for(i = 0, done = 0; i < sizeof(buffer) && !done; i++)
    {
        if(buffer[i] == '.')
        {
            j = sscanf(buffer, "%lf", &MagneticDate->DecimalYear);
            if(j == 1)
                done = 1;
            else
                buffer[i] = '\0';
        }
        if(buffer[i] == '/')
        {
            sscanf(buffer, "%d/%d/%d", &MagneticDate->Month, &MagneticDate->Day, &MagneticDate->Year);
            if(!MAG_DateToYear(MagneticDate, Error_Message))
            {
                printf("%s", Error_Message);
                printf("\nPlease re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
                while (NULL == fgets(buffer, sizeof(buffer), stdin)) {
                    printf("\nPlease re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
                    if (buffer[sizeof(buffer) - 1] != '\n'){
                        MAG_clear_input_buffer(); // Remove the left characters from stdin if the buffer is not able to read the whole stdin
                    }
                }
                 
                i = 0;
            } else
                done = 1;
        }
        if((i < sizeof(buffer) - 1 && buffer[i] == ' ' && buffer[i + 1] != '/') || buffer[i] == '\0')
        {
            if(3 == sscanf(buffer, "%d %d %d", &a, &b, &c))
            {
                MagneticDate->Month = a;
                MagneticDate->Day = b;
                MagneticDate->Year = c;
                MagneticDate->DecimalYear = 99999;
            } else if(1 == sscanf(buffer, "%d %d %d", &a, &b, &c))
            {
                MagneticDate->DecimalYear = a;
                done = 1;
            }
            if(!(MagneticDate->DecimalYear == a))
            {
                if(!MAG_DateToYear(MagneticDate, Error_Message))
                {
                    printf("%s", Error_Message);
                    MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
                    printf("\nError encountered, please re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
                    while( NULL== fgets(buffer, sizeof(buffer), stdin)){
                        printf("\nError encountered, please re-enter Date in MM/DD/YYYY or MM DD YYYY format, or as a decimal year\n");
                        if (buffer[strlen(buffer) - 1] != '\n') {
                            MAG_clear_input_buffer(); 
                        }
                    }
                    
                    i = -1;
                } else
                    done = 1;
            }
        }
        if(buffer[i] == '\0' && i != -1 && done != 1)
        {
            MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
            printf("\nError encountered, please re-enter as MM/DD/YYYY, MM DD YYYY, or as YYYY.yyy:\n");
            while (NULL ==fgets(buffer, sizeof(buffer), stdin)) {
                printf("\nError encountered, please re-enter as MM/DD/YYYY, MM DD YYYY, or as YYYY.yyy:\n"); 
                if (buffer[sizeof(buffer) - 1] != '\n'){
                    MAG_clear_input_buffer(); // Remove the left characters from stdin if the buffer is not able to read the whole stdin
                }
            }
             
            i = -1;
        }
        if(done)
        {
            if(MagneticDate->DecimalYear > MagneticModel->CoefficientFileEndDate || MagneticDate->DecimalYear < MagneticModel->min_year)
            {
                switch(MAG_Warnings(4, MagneticDate->DecimalYear, MagneticModel)) {
                    case 0:
                        return 0;
                    case 1:
                        done = 0;
                        i = -1;
                        MAG_strlcpy_equivalent(buffer, "", sizeof(buffer));
                        printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
                        while(NULL == fgets(buffer, sizeof(buffer), stdin)){
                            printf("\nPlease enter the decimal year or calendar date\n (YYYY.yyy, MM DD YYYY or MM/DD/YYYY):\n");
                            if (buffer[sizeof(buffer) - 1] != '\n'){
                                MAG_clear_input_buffer(); // Remove the left characters from stdin if the buffer is not able to read the whole stdin
                            }
                        }
                         
                        break;
                    case 2:
                        break;
                }
            }
        }
    }
    free(Qstring);
    return 1;
} /*MAG_GetUserInput*/
