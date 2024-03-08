#ifndef WAHBAROTM_H
#define WAHBAROTM_H

#include "hw_conf.hpp"

#define fabsv fabs
#define sqrtv sqrt
#define fabsv fabs
#define acosv acos
#define cosv cos
#define sinv sin
#define SIGN(x) (((x) > 0) - ((x) < 0))
#define SATUR(x,Lim) (x>Lim)?(Lim):((x<-Lim)?(-Lim):x)

#define TOL 1e-6

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Quat4;

typedef struct {
	float w_a[3];
	float w_g[3];
	float w_m[3];
	float g_vg[3];
	float g_va[3];
	float g_vm[3];
	float q[4];
	float RotM[3][3];
	float RotM_prev[3][3];
	float dR[3][3];
	float dth;
	float W[3];
	float Euler[3];
	float AccErr;
	float dt;
} WahbaRotMStruct;



float normV(float V[3]);

void apply_jacobi_R(float (*A)[3], int p, int q, float c, float s, float k);
void apply_jacobi_L(float (*A)[3], int p, int q, float c, float s, float k);
void jacobi2Sided(float (*A)[3], float (*U)[3], float (*V)[3]);
void WahbaJacobiSVDRotM(float (*A)[3], float (*RotM)[3]);

float detMatr(float (*A)[3]);
void mulMatrVec(float *v,float (*A)[3],float *x);
void mulMatrTrVec(float *v, float (*A)[3], float *x);
void mulMatrMatrTr(float (*A)[3], float (*B)[3], float (*C)[3]);

void dRpropagat(float (*R)[3], float (*dR)[3], float w[3], float dt);

void rotmtx2euler(const float *rotmtx, float *euler);
void rotmtx2quat(const float *rotmtx, Quat4 *q);

void initWahbaStruct(WahbaRotMStruct *WStruct, float dt);
void WahbaRotM(float acc[3],float gyr[3],float mag[3], WahbaRotMStruct *WStruct);

#endif