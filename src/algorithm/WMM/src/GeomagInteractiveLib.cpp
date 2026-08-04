#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>


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