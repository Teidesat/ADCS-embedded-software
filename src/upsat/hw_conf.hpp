#ifndef HW_CONF_H
#define HW_CONF_H

#include <cmath>

typedef struct {
	float GyrRaw[3];
	float MagRaw[3];
	float AccRaw[3];

	float TempRaw;
	float Gyr[3];
	float Acc[3];
	float Mag[3];
	float CalibGyr[3];
	float CalibAcc[3];
	float CalibMag[3];
} sensorDat;

sensorDat scaleSens(float acc[3],float gyr[3],float mag[3]);

void calibSens(sensorDat &sens);

#endif