#include "WahbaRotM.hpp"



float normV(float V[3]) {return sqrtv(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);}


float detMatr(float (*A)[3]) {
    return A[0][0] * A[1][1] * A[2][2] + A[0][1] * A[1][2] * A[2][0]
            + A[0][2] * A[1][0] * A[2][1]
            - (A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][0] * A[2][2]
            + A[0][0] * A[1][2] * A[2][1]);
}



void mulMatrVec(float *v,float (*A)[3],float *x){
	//int i;
	v[0]=A[0][0]*x[0]+A[0][1]*x[1]+A[0][2]*x[2];
	v[1]=A[1][0]*x[0]+A[1][1]*x[1]+A[1][2]*x[2];
	v[2]=A[2][0]*x[0]+A[2][1]*x[1]+A[2][2]*x[2];
}



void mulMatrTrVec(float *v, float (*A)[3], float *x) {

    v[0] = A[0][0] * x[0] + A[1][0] * x[1] + A[2][0] * x[2];
    v[1] = A[0][1] * x[0] + A[1][1] * x[1] + A[2][1] * x[2];
    v[2] = A[0][2] * x[0] + A[1][2] * x[1] + A[2][2] * x[2];
}



void mulMatrMatrTr(float (*A)[3], float (*B)[3], float (*C)[3]) {
    //int i;
    for (int i = 0; i < 3; i++) {
        A[0][i] = B[0][0] * C[i][0] + B[0][1] * C[i][1] + B[0][2] * C[i][2];
        A[1][i] = B[1][0] * C[i][0] + B[1][1] * C[i][1] + B[1][2] * C[i][2];
        A[2][i] = B[2][0] * C[i][0] + B[2][1] * C[i][1] + B[2][2] * C[i][2];
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void dRpropagat(float (*Rbe)[3], float (*R)[3], float *wb, float dt) {
    float norm, th, c, s, u;
    float w[3];
    float dR[3][3];

    mulMatrTrVec(w, R, wb); // w body to earth;

    norm = normV(w);
    if (norm < 1e-6) {
        for (int i = 0; i < 3; i++) {
            dR[0][i] = 0;
            dR[1][i] = 0;
            dR[2][i] = 0;
        }
        dR[0][0] = 1;
        dR[1][1] = 1;
        dR[2][2] = 1;
    } else {
        w[0] = w[0] / norm;
        w[1] = w[1] / norm;
        w[2] = w[2] / norm;

        th = norm * dt;
        c = cosv(th);
        s = sinv(th);
        u = 1 - c;

        dR[0][0] = w[0] * w[0] * u + c;
        dR[0][1] = w[0] * w[1] * u - w[2] * s;
        dR[0][2] = w[0] * w[2] * u + w[1] * s;
        dR[1][0] = w[1] * w[0] * u + w[2] * s;
        dR[1][1] = w[1] * w[1] * u + c;
        dR[1][2] = w[1] * w[2] * u - w[0] * s;
        dR[2][0] = w[2] * w[0] * u - w[1] * s;
        dR[2][1] = w[2] * w[1] * u + w[0] * s;
        dR[2][2] = w[2] * w[2] * u + c;

        mulMatrMatrTr(Rbe, R, dR);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void rotmtx2euler(const float *rotmtx, float *euler) {

    const float tol = 1E-6;
    const float test = sqrt(rotmtx[0] * rotmtx[0] + rotmtx[3] * rotmtx[3]);

	if (test >= +tol) {
		euler[0] = atan2(rotmtx[3], rotmtx[0]);
		euler[1] = atan2(-rotmtx[6], test);
		euler[2] = atan2f(rotmtx[7], rotmtx[8]);
	} else {
		euler[0] = 0;
		euler[1] = atan2f(-rotmtx[6], test);
		euler[2] = atan2f(-rotmtx[5], rotmtx[4]);
	}
}



void rotmtx2quat(const float *rotmtx, Quat4 *q) {
    q->z = 0.0f;
    float sqtrp1;
    float sqtrp1x2;
    float d[3];
    float sqdip1;

    const float tr = rotmtx[0] + rotmtx[4] + rotmtx[8];
    if (tr > 1e-6f) {
        sqtrp1 = sqrtv(tr + 1.0f);
        sqtrp1x2 = 2.0 * sqtrp1;

        q->w = 0.5f * sqtrp1;
        q->x = (rotmtx[7] - rotmtx[5]) / sqtrp1x2;
        q->y = (rotmtx[2] - rotmtx[6]) / sqtrp1x2;
        q->z = (rotmtx[3] - rotmtx[1]) / sqtrp1x2;
    } else {
        d[0] = rotmtx[0];
        d[1] = rotmtx[4];
        d[2] = rotmtx[8];

        if ((d[1] > d[0]) && (d[1] > d[2])) {
            sqdip1 = sqrtv(d[1] - d[0] - d[2] + 1.0f);
            q->y = 0.5f * sqdip1;

            if (fabsf(sqdip1) > 1e-6f)
                sqdip1 = 0.5f / sqdip1;

            q->w = (rotmtx[2] - rotmtx[6]) * sqdip1;
            q->x = (rotmtx[3] + rotmtx[1]) * sqdip1;
            q->z = (rotmtx[7] + rotmtx[5]) * sqdip1;
        } else if (d[2] > d[0]) {
            //max value at R(3,3)
            sqdip1 = sqrtv(d[2] - d[0] - d[1] + 1.0f);

            q->z = 0.5f * sqdip1;

            if (fabsf(sqdip1) > 1e-6f)
                sqdip1 = 0.5f / sqdip1;

            q->w = (rotmtx[3] - rotmtx[1]) * sqdip1;
            q->x = (rotmtx[2] + rotmtx[6]) * sqdip1;
            q->y = (rotmtx[7] + rotmtx[5]) * sqdip1;
        } else {
            // max value at R(1,1)
            sqdip1 = sqrtv(d[0] - d[1] - d[2] + 1.0f);

            q->x = 0.5f * sqdip1;

            if (fabsf(sqdip1) > 1e-6f)
                sqdip1 = 0.5f / sqdip1;

            q->w = (rotmtx[7] - rotmtx[5]) * sqdip1;
            q->y = (rotmtx[3] + rotmtx[1]) * sqdip1;
            q->z = (rotmtx[2] + rotmtx[6]) * sqdip1;
        }
    }

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void initWahbaStruct(WahbaRotMStruct *WStruct, float dt) {

	WStruct->w_m[0] = 0.707;
	WStruct->w_m[1] = 0;
	WStruct->w_m[2] = 0.707;

	WStruct->w_a[0] = 0;
	WStruct->w_a[1] = 0;
	WStruct->w_a[2] = -1.0;

	WStruct->w_g[0] = -0.707;
	WStruct->w_g[1] = 0.0;
	WStruct->w_g[2] = 0.707;

	WStruct->dt=dt;

	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			WStruct->RotM[i][j]=0;
	for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				WStruct->RotM_prev[i][j]=0;
	for(int i=0;i<3;i++)
		WStruct->RotM[i][i]=1;
	for(int i=0;i<3;i++)
			WStruct->RotM_prev[i][i]=1;

	for(int i=0;i<3;i++){
		WStruct->g_va[i]=0;
		WStruct->g_vg[i]=0;
		WStruct->g_vm[i]=0;
	}
}



void WahbaRotM(float acc[3],float gyr[3],float mag[3],WahbaRotMStruct *WStruct) {
	static float a,g,ga,gm,m, A[3][3],dt;
	float (*RotM)[3];
	a=0.05;
	m=0.05;
	g=0.95;
	ga=1-a;
	gm=1-m;

	static float /*AccErr,*/*w_a,*w_g,*w_m,*g_va,*g_vm;//,*g_vg;
	w_a=WStruct->w_a;
	w_g=WStruct->w_g;
	w_m=WStruct->w_m;
	RotM=WStruct->RotM;
	g_va=WStruct->g_va;
	//g_vg=WStruct->g_vg;
	g_vm=WStruct->g_vm;

	dt=WStruct->dt;

	static float Rbe[3][3];

	float accn[3],magn[3],An,Mn;
	An=normV(acc);
	Mn=normV(mag);
	for(int i=0;i<3;i++){
		accn[i]=acc[i]/An;
		magn[i]=mag[i]/Mn;
	}



	for(int i=0;i<3;i++){
		A[i][0]=a*accn[0]*w_a[i] + m*magn[0]*w_m[i] + ga*g_va[0]*w_a[i] + gm*g_vm[0]*w_m[i];
		A[i][1]=a*accn[1]*w_a[i] + m*magn[1]*w_m[i] + ga*g_va[1]*w_a[i] + gm*g_vm[1]*w_m[i];
		A[i][2]=a*accn[2]*w_a[i] + m*magn[2]*w_m[i] + ga*g_va[2]*w_a[i] + gm*g_vm[2]*w_m[i];
	}
//	for(int i=0;i<3;i++){
//		A[i][0]=a*accn[0]*w_a[i] + m*magn[0]*w_m[i];
//		A[i][1]=a*accn[1]*w_a[i] + m*magn[1]*w_m[i];
//		A[i][2]=a*accn[2]*w_a[i] + m*magn[2]*w_m[i];
//	}
	//	float A1[3][3]={{1.21,3.214,0.1},{0,0,0},{2.12,0.120,0.43}};
	//float A1[3][3]={{1.001,2,3},{0,0,0},{2,4,6}}; //check svd


	WahbaJacobiSVDRotM(A,RotM);
	dRpropagat(Rbe,RotM,gyr,dt);

//	mulMatrTrVec(w_m,Rbe,magn);
//	w_m[0]=sqrtv(w_m[0]*w_m[0]+w_m[1]*w_m[1]);
//	w_m[1]=0;


	mulMatrVec(g_va,Rbe,w_a);
	mulMatrVec(g_vm,Rbe,w_m);

	//mulMatr(Rr,RtM,Rbe);
	//Conversion to euler takes 7-8us...
	rotmtx2euler((const float *)RotM,(float *)WStruct->Euler);
	rotmtx2quat((const float *)RotM,(Quat4 *)WStruct->q);

	// Calculate W from dR
	static float tmp;
	static float tr;

	mulMatrMatrTr(WStruct->dR,RotM,WStruct->RotM_prev);


	tr =  ((WStruct->dR[0][0] + WStruct->dR[1][1] + WStruct->dR[2][2]) - 1.0)/2.0 ;
	tr = SATUR(tr,1);
	tmp = acosv(tr);
	WStruct->dth = tmp;
	if (fabsv(tmp) < TOL){
		tmp = 0.5/dt;  // x/sin(x)|x->0
	}
	else{
		tmp = 0.5/dt*tmp/sinv(tmp);
	}
	WStruct->W[0] = tmp*(WStruct->dR[2][1] - WStruct->dR[1][2]);
	WStruct->W[1] = tmp*(WStruct->dR[0][2] - WStruct->dR[2][0]);
	WStruct->W[2] = tmp*(WStruct->dR[1][0] - WStruct->dR[0][1]);


	for(int i = 0;i < 3; i++){
		WStruct->RotM_prev[0][i] = RotM[0][i];
		WStruct->RotM_prev[1][i] = RotM[1][i];
		WStruct->RotM_prev[2][i] = RotM[2][i];
	}
}