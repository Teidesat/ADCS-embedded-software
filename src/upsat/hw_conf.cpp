#include "hw_conf.hpp"

sensorDat scaleSens(float acc[3],float gyr[3],float mag[3]) {

	sensorDat sens;
	sens.AccRaw[0] = acc[0];
	sens.AccRaw[1] = acc[1];
	sens.AccRaw[2] = acc[2];
	sens.GyrRaw[0] = gyr[0];
	sens.GyrRaw[1] = gyr[1];
	sens.GyrRaw[2] = gyr[2];
	sens.MagRaw[0] = mag[0];
	sens.MagRaw[1] = mag[1];
	sens.MagRaw[2] = mag[2];

	float vec[3];
	// int i;
	const float AccSc[9]={ 	 1.0,  0.0,   0.0, \
			0.0,  1.0,   0.0,\
			0.0,  0.0,   1.0 };
	const float AccOf[3]={0.095839126788067, 0.050385565686325, 0.006112150167768};

	const float MagSc[9]={ 	1.19,  0.0,   0.0, \
							0.0,  1.37170,   0.0,\
							0.0,  0.0,   1.3760 };
	const float MagOf[3]={0.2235, 0.1018, 0.1352};



	for(int i=0;i<3;i++){
		vec[i]=((float)sens.AccRaw[i])*0.122/1e3;
		//sens.Gyr[i]=((float)sens.GyrRaw[i])*4.0*(17.5/1e3)*(M_PI);//8.75/1e3;
		sens.Gyr[i] = sens.GyrRaw[i];
	}

	//sens.Acc[0] = AccSc[0]*vec[0]+AccSc[1]*vec[1]+AccSc[2]*vec[2]+AccOf[0];
	//sens.Acc[1] = AccSc[3]*vec[0]+AccSc[4]*vec[1]+AccSc[5]*vec[2]+AccOf[1];
	//sens.Acc[2] = AccSc[6]*vec[0]+AccSc[7]*vec[1]+AccSc[8]*vec[2]+AccOf[2];
	
	sens.Acc[0] = sens.AccRaw[0];
	sens.Acc[1] = sens.AccRaw[1]; 
	sens.Acc[2] = sens.AccRaw[2]; 

	//sens.Mag[0] = ( sens.MagRaw[0]/6500.0 + MagOf[0]);
	//sens.Mag[1] = ( sens.MagRaw[1]/6500.0 + MagOf[1]);
	//sens.Mag[2] = (-sens.MagRaw[2]/6500.0 + MagOf[2]);
	
	sens.Mag[0] = sens.MagRaw[0];
	sens.Mag[1] = sens.MagRaw[1];
	sens.Mag[2] = sens.MagRaw[2];

	//sens.Mag[0] = MagSc[0]*sens.Mag[0] + MagSc[1]*sens.Mag[1] + MagSc[2]*sens.Mag[2];
	//sens.Mag[1] = MagSc[3]*sens.Mag[0] + MagSc[4]*sens.Mag[1] + MagSc[5]*sens.Mag[2];
	//sens.Mag[2] = MagSc[6]*sens.Mag[0] + MagSc[7]*sens.Mag[1] + MagSc[8]*sens.Mag[2];

	return sens;
}