#include <bgbrasw.h>

#include <stdlib.h>

static float *vecnf_vecpool=NULL;
static double *vecnd_vecpool=NULL;
static int vecnf_vecpos;
static int vecnd_vecpos;

VECMATH_API float *VecNF_AllocVec(int sz)
{
	float *p;

	if(!vecnf_vecpool)
	{
		vecnf_vecpool=malloc((1<<16)*sizeof(float));
		vecnf_vecpos=0;
	}

	if((vecnf_vecpos+sz)>=(1<<16))
		vecnf_vecpos=0;

	p=vecnf_vecpool+vecnf_vecpos;
	vecnf_vecpos+=sz;

	return(p);
}

VECMATH_API double *VecND_AllocVec(int sz)
{
	double *p;

	if(!vecnd_vecpool)
	{
		vecnd_vecpool=malloc((1<<16)*sizeof(double));
		vecnd_vecpos=0;
	}

	if((vecnd_vecpos+sz)>=(1<<16))
		vecnd_vecpos=0;

	p=vecnd_vecpool+vecnd_vecpos;
	vecnd_vecpos+=sz;

	return(p);
}

/* matrix functions */

VECMATH_API void MatNF_MatMult(
	float *a, int ah, int aw, float *b, int bh, int bw, float *c)
{
	int i, j, k;

	if(aw!=bh)return;

	for(i=0; i<(ah*bw); i++)c[i]=0;
	for(i=0; i<ah; i++)
		for(j=0; j<bw; j++)
		for(k=0; k<aw; k++)
		c[j*bw+k]+=a[j*aw+i]*b[i*bw+k];
}

VECMATH_API void MatNF_MatMultT(
	float *a, int ah, int aw, float *b, int bh, int bw, float *c)
{
	int i, j, k;

	if(aw!=bh)return;

	for(i=0; i<(ah*bw); i++)c[i]=0;
	for(i=0; i<ah; i++)
		for(j=0; j<bw; j++)
		for(k=0; k<aw; k++)
		c[k*bh+j]+=a[i*ah+j]*b[k*bh+i];
}

VECMATH_API void MatNF_MatMultVector(float *a, float *b, float *c, int n)
{
	int i, j;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
		c[i*n+j]=a[i*n+j]*b[j];
}

VECMATH_API void MatNF_MatMultScaler(float *a, float b, float *c, int n)
{
	int i;
	for(i=0; i<(n*n); i++)c[i]=a[i]*b;
}

VECMATH_API void MatNF_ResizeMatrix(float *a, int ah, int aw, float *b, int bh, int bw)
{
	int i, j;
	for(i=0; i<bh; i++)for(j=0; j<bw; j++)
		if(a && i<ah && j<aw)b[i*bw+j]=a[i*aw+j];
			else b[i*bw+j]=((bh==bw) && (i==j))?1:0;
}

VECMATH_API void MatNF_FillIdentity(float *a, int n)
{
	int i;
	for(i=0; i<(n*n); i++)a[i]=0;
	for(i=0; i<n; i++)a[(i*n)+i]=1;
}

VECMATH_API void MatNF_Copy(float *a, float *b, int h, int w)
{
	int i;
	for(i=0;i<(h*w);i++)b[i]=a[i];
}

VECMATH_API void Vec16F_RotateMatrix(
	float *amat, float *angles, float *bmat)
{
	float tmat[16];
	float sx, sy, sz, cx, cy, cz;
	MatNF_FillIdentity(tmat, 4);

	sx=sin(angles[0]*(M_PI/180.0));
	sy=sin(angles[1]*(M_PI/180.0));
	sz=sin(angles[2]*(M_PI/180.0));
	cx=cos(angles[0]*(M_PI/180.0));
	cy=cos(angles[1]*(M_PI/180.0));
	cz=cos(angles[2]*(M_PI/180.0));

//	tmp[1]=point[1]*cx-point[2]*sx;
//	tmp[2]=point[1]*sx+point[2]*cx;
//
//	tmp[2]=point[2]*cy-point[0]*sy;
//	tmp[0]=point[2]*sy+point[0]*cy;
//
//	tmp[0]=point[0]*cz-point[1]*sz;
//	tmp[1]=point[0]*sz+point[1]*cz;
//
//  tmp[0]=((point[1]*sx+point[2]*cx)*sy+point[0]*cy)*cz-(point[1]*cx-point[2]*sx)*sz;
//  tmp[1]=((point[1]*sx+point[2]*cx)*sy+point[0]*cy)*sz+(point[1]*cx-point[2]*sx)*cz;
//  tmp[2]=(point[1]*sx+point[2]*cx)*cy-point[0]*sy;
//  tmp[2]=(point[0]*sz+point[1]*cz)*sx+(point[2]*cy-(point[0]*cz-point[1]*sz)*sy)*cx;
//
//
//	tmp[1]=point[1]*cx-point[2]*sx;
//	tmp[2]=point[1]*sx+point[2]*cx;
//
//	tmp[2]=(point[1]*sx+point[2]*cx)*cy-point[0]*sy;
//	tmp[0]=(point[1]*sx+point[2]*cx)*sy+point[0]*cy;
//
//	tmp[0]=((point[1]*sx+point[2]*cx)*sy+point[0]*cy)*cz-(point[1]*cx-point[2]*sx)*sz;
//	tmp[1]=((point[1]*sx+point[2]*cx)*sy+point[0]*cy)*sz+(point[1]*cx-point[2]*sx)*cz;
//	tmp[2]=(point[1]*sx+point[2]*cx)*cy-point[0]*sy;

	tmat[0]=cy*cz;
	tmat[1]=cy*sz;
	tmat[2]=-sy;

//	VecNF_Normalize(tmat, tmat, 3);

	tmat[4]=sx*sy*cz-cx*sz;
	tmat[5]=sx*sy*sz+cx*cz;
	tmat[6]=sx*cy;

//	VecNF_Normalize(&tmat[4], &tmat[4], 3);

	tmat[8]=cx*sy*cz+sx*sz;
	tmat[9]=cx*sy*sz-sx*cz;
	tmat[10]=cx*cy;

//	VecNF_Normalize(&tmat[8], &tmat[8], 3);

	MatNF_MatMult(amat, 4, 4, tmat, 4, 4, bmat);
}

VECMATH_API void MatNF_GetLowerMatrix(float *a, float *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[i*n+j]=(i<j)?0:a[i*n+j];
}

VECMATH_API void MatNF_GetUpperMatrix(float *a, float *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[i*n+j]=(i>j)?0:a[i*n+j];
}

VECMATH_API void Vec16F_TranslateMatrix(
	float *amat, float *pos, float *bmat)
{
	float tmat[16];
	MatNF_FillIdentity(tmat, 4);
	tmat[3]=pos[0];
	tmat[7]=pos[1];
	tmat[11]=pos[2];
	MatNF_MatMult(amat, 4, 4, tmat, 4, 4, bmat);
}

VECMATH_API void Vec16F_ScaleMatrix(
	float *amat, float *pos, float *bmat)
{
	float tmat[16];
	MatNF_FillIdentity(tmat, 4);
	tmat[0]=pos[0];
	tmat[5]=pos[1];
	tmat[10]=pos[2];
	MatNF_MatMult(amat, 4, 4, tmat, 4, 4, bmat);
}

/* swap ordering, matrices to/from gl */
VECMATH_API void MatNF_SwapOrder(float *a, float *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[(i*n)+j]=a[(j*n)+i];
}

VECMATH_API void MatNF_Inverse(float *a, float *b, int n)
{
	static float a2buf[8*8*2], a3buf[16];
	float *a2p[8], *a3p;
	int i, j;

	VecNF_Zero(a2buf, n*n*2);
	VecNF_Zero(b, n*n);
	for(i=0; i<n; i++)a2p[i]=&a2buf[i*n*2];
	for(i=0; i<n; i++)VecNF_Copy(&a[i*n], a2p[i], n);
	for(i=0; i<n; i++)a2p[i][i+n]=1.0;

	for(i=0; i<n; i++)
	{
//		MatNF_Dump(a2buf, n, 2*n);
//		printf("\n");
		if(a2p[i][i]==0) /* search for a new pivot in a lower row */
		{
			for(j=n-1; j>i; j--)if(a2p[j][i]!=0)
			{
				a3p=a2p[i];
				a2p[i]=a2p[j];
				a2p[j]=a3p;
			}
			if(a2p[i][i]==0)return; /* no pivot */
		}
		VecNF_Scale(a2p[i], 1/a2p[i][i], a2p[i], 2*n);
		for(j=0; j<n; j++)if(i!=j)
		{
			VecNF_Scale(a2p[i], -a2p[j][i], a3buf, 2*n);
			VecNF_Add(a3buf, a2p[j], a2p[j], 2*n);
		}
	}

//	MatNF_Dump(a2buf, n, 2*n);
//	printf("\n");

	for(i=0; i<n; i++)VecNF_Copy(&a2p[i][n], &b[i*n], n);
}

/* functions to try for camera handling */

VECMATH_API void Vec3F_ToSphericalCoords(float *a, float *b)
{
	float a2[3];

	VecNF_Normalize(a, a2, 3);

	b[0]=VecNF_Length(a, 3);
	b[1]=atan2(a2[1], a2[0]);
	b[2]=acos(a2[2]);
}

VECMATH_API void Vec3F_FromSphericalCoords(float *a, float *b)
{
	b[0]=a[0]*sin(a[2])*cos(a[1]);
	b[1]=a[0]*sin(a[2])*sin(a[1]);
	b[2]=a[0]*cos(a[2]);
}

VECMATH_API void Vec16F_InvertAngles(float *amat, float *bmat)
{
	float v[3];

	MatNF_Copy(amat, bmat, 4, 4);

	Vec3F_ToSphericalCoords(&amat[0], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3F_FromSphericalCoords(v, &bmat[0]);

	Vec3F_ToSphericalCoords(&amat[4], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3F_FromSphericalCoords(v, &bmat[4]);

	Vec3F_ToSphericalCoords(&amat[8], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3F_FromSphericalCoords(v, &bmat[8]);

	bmat[3]=-amat[3];
	bmat[7]=-amat[7];
	bmat[11]=-amat[11];
}

VECMATH_API void Vec3F_RotatePoint(float *p, float *angles)
{
	float		angle;
	float		sx, sy, sz, cx, cy, cz;
	float		tmp[3], point[3];

	angle = angles[0] * (M_PI/180);
	sx = sin(angle);
	cx = cos(angle);
	angle = angles[1] * (M_PI/180);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[2] * (M_PI/180);
	sz = sin(angle);
	cz = cos(angle);

	point[0]=p[0];
	point[1]=p[1];
	point[2]=p[2];

	tmp[1]=point[1]*cx-point[2]*sx;
	tmp[2]=point[1]*sx+point[2]*cx;
	point[1]=tmp[1];
	point[2]=tmp[2];

	tmp[2]=point[2]*cy-point[0]*sy;
	tmp[0]=point[2]*sy+point[0]*cy;
	point[2]=tmp[2];
	point[0]=tmp[0];

	tmp[0]=point[0]*cz-point[1]*sz;
	tmp[1]=point[0]*sz+point[1]*cz;
	point[0]=tmp[0];
	point[1]=tmp[1];

	p[0]=point[0];
	p[1]=point[1];
	p[2]=point[2];
}

VECMATH_API void Vec3F_CalcAngleVectors(
	float *angles, float *forward, float *right, float *up)
{
	forward[0]=0;
	forward[1]=0;
	forward[2]=-1;
	Vec3F_RotatePoint(forward, angles);

	right[0]=1;
	right[1]=0;
	right[2]=0;
	Vec3F_RotatePoint(right, angles);

	up[0]=0;
	up[1]=1;
	up[2]=0;
	Vec3F_RotatePoint(up, angles);
}


VECMATH_API void Vec3F_NormalToParentSpace(float *a, float *sp, float *b)
{
	b[0]=(a[0]*sp[0])+(a[1]*sp[1])+(a[2]*sp[2]);
	b[1]=(a[0]*sp[4])+(a[1]*sp[5])+(a[2]*sp[6]);
	b[2]=(a[0]*sp[8])+(a[1]*sp[9])+(a[2]*sp[10]);

//	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8]);
//	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9]);
//	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10]);
}

VECMATH_API void Vec3F_NormalFromParentSpace(float *a, float *sp, float *b)
{
	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8]);
	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9]);
	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10]);

//	b[0]=(a[0]*sp[0])+(a[1]*sp[1])+(a[2]*sp[2]);
//	b[1]=(a[0]*sp[4])+(a[1]*sp[5])+(a[2]*sp[6]);
//	b[2]=(a[0]*sp[8])+(a[1]*sp[9])+(a[2]*sp[10]);
}

VECMATH_API void Vec3F_NormalToChildSpace(float *a, float *sp, float *b)
{
	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8]);
	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9]);
	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10]);
}

VECMATH_API void Vec3F_NormalFromChildSpace(float *a, float *sp, float *b)
{
	b[0]=(a[0]*sp[0])+(a[1]*sp[1])+(a[2]*sp[2]);
	b[1]=(a[0]*sp[4])+(a[1]*sp[5])+(a[2]*sp[6]);
	b[2]=(a[0]*sp[8])+(a[1]*sp[9])+(a[2]*sp[10]);
}

VECMATH_API void Vec3F_PointToParentSpace(float *a, float *sp, float *b)
{
//	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8])+sp[3];
//	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9])+sp[7];
//	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10])+sp[11];

	b[0]=(a[0]*sp[0])+(a[1]*sp[1])+(a[2]*sp[2])+sp[3];
	b[1]=(a[0]*sp[4])+(a[1]*sp[5])+(a[2]*sp[6])+sp[7];
	b[2]=(a[0]*sp[8])+(a[1]*sp[9])+(a[2]*sp[10])+sp[11];
}

VECMATH_API void Vec3F_PointFromParentSpace(float *a, float *sp, float *b)
{
	float av[3];

//	av[0]=a[0]-sp[3];
//	av[1]=a[1]-sp[7];
//	av[2]=a[2]-sp[11];

	av[0]=a[0]-sp[12];
	av[1]=a[1]-sp[13];
	av[2]=a[2]-sp[14];

	b[0]=(av[0]*sp[0])+(av[1]*sp[4])+(av[2]*sp[8]);
	b[1]=(av[0]*sp[1])+(av[1]*sp[5])+(av[2]*sp[9]);
	b[2]=(av[0]*sp[2])+(av[1]*sp[6])+(av[2]*sp[10]);

//	b[0]=(av[0]*sp[0])+(av[1]*sp[1])+(av[2]*sp[2]);
//	b[1]=(av[0]*sp[4])+(av[1]*sp[5])+(av[2]*sp[6]);
//	b[2]=(av[0]*sp[8])+(av[1]*sp[9])+(av[2]*sp[10]);
}

VECMATH_API void Vec3F_PointToChildSpace(float *a, float *sp, float *b)
{
	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8])+sp[3];
	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9])+sp[7];
	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10])+sp[11];

}

VECMATH_API void Vec3F_PointFromChildSpace(float *a, float *sp, float *b)
{
	float av[3];
//	av[0]=a[0]-sp[3];
//	av[1]=a[1]-sp[7];
//	av[2]=a[2]-sp[11];

	av[0]=a[0]-sp[12];
	av[1]=a[1]-sp[13];
	av[2]=a[2]-sp[14];

	b[0]=(av[0]*sp[0])+(av[1]*sp[1])+(av[2]*sp[2]);
	b[1]=(av[0]*sp[4])+(av[1]*sp[5])+(av[2]*sp[6]);
	b[2]=(av[0]*sp[8])+(av[1]*sp[9])+(av[2]*sp[10]);
}

/* misc */
VECMATH_API float Scaler_Random() /* random number, 0 to 1 */
{
	float tmp;
	tmp=(1.0*rand())/RAND_MAX;
	return(tmp);
}

VECMATH_API float Scaler_SignRandom() /* -1 to 1 */
{
	float tmp;
	tmp=((2.0*rand())/RAND_MAX)-1.0;
	return(tmp);
}

