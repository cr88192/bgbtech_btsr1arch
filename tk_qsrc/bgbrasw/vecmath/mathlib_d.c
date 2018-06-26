#include <bgbrasw.h>

/* matrix functions */

VECMATH_API void MatND_MatMult(
	double *a, int ah, int aw, double *b, int bh, int bw, double *c)
{
	int i, j, k;

	if(aw!=bh)return;

	for(i=0; i<(ah*bw); i++)c[i]=0;
	for(i=0; i<ah; i++)
		for(j=0; j<bw; j++)
		for(k=0; k<aw; k++)
		c[j*bw+k]+=a[j*aw+i]*b[i*bw+k];
}

VECMATH_API void MatND_MatMultVector(
	double *a, double *b, double *c, int n)
{
	int i, j;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
		c[i*n+j]=a[i*n+j]*b[j];
}

VECMATH_API void MatND_MatMultScaler(
	double *a, double b, double *c, int n)
{
	int i;
	for(i=0; i<(n*n); i++)c[i]=a[i]*b;
}

VECMATH_API void MatND_ResizeMatrix(
	double *a, int ah, int aw, double *b, int bh, int bw)
{
	int i, j;
	for(i=0; i<bh; i++)for(j=0; j<bw; j++)
		if(a && i<ah && j<aw)b[i*bw+j]=a[i*aw+j];
			else b[i*bw+j]=((bh==bw) && (i==j))?1:0;
}

VECMATH_API void MatND_FillIdentity(double *a, int n)
{
	int i;
	for(i=0; i<(n*n); i++)a[i]=0;
	for(i=0; i<n; i++)a[(i*n)+i]=1;
}

VECMATH_API void MatND_Copy(double *a, double *b, int h, int w)
{
	int i;
	for(i=0;i<(h*w);i++)b[i]=a[i];
}

VECMATH_API void Vec16D_RotateMatrix(
	double *amat, double *angles, double *bmat)
{
	double tmat[16];
	double sx, sy, sz, cx, cy, cz;
	MatND_FillIdentity(tmat, 4);

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

//	VecND_Normalize(tmat, tmat, 3);

	tmat[4]=sx*sy*cz-cx*sz;
	tmat[5]=sx*sy*sz+cx*cz;
	tmat[6]=sx*cy;

//	VecND_Normalize(&tmat[4], &tmat[4], 3);

	tmat[8]=cx*sy*cz+sx*sz;
	tmat[9]=cx*sy*sz-sx*cz;
	tmat[10]=cx*cy;

//	VecND_Normalize(&tmat[8], &tmat[8], 3);

	MatND_MatMult(amat, 4, 4, tmat, 4, 4, bmat);
}

VECMATH_API void MatND_GetLowerMatrix(double *a, double *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[i*n+j]=(i<j)?0:a[i*n+j];
}

VECMATH_API void MatND_GetUpperMatrix(double *a, double *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[i*n+j]=(i>j)?0:a[i*n+j];
}

VECMATH_API void Vec16D_TranslateMatrix(
	double *amat, double *pos, double *bmat)
{
	double tmat[16];
	MatND_FillIdentity(tmat, 4);
	tmat[3]=pos[0];
	tmat[7]=pos[1];
	tmat[11]=pos[2];
	MatND_MatMult(amat, 4, 4, tmat, 4, 4, bmat);
}

/* swap ordering, matrices to/from gl */
VECMATH_API void MatND_SwapOrder(double *a, double *b, int n)
{
	int i, j;
	for(i=0; i<n; i++)for(j=0; j<n; j++)b[(i*n)+j]=a[(j*n)+i];
}

VECMATH_API void MatND_Inverse(double *a, double *b, int n)
{
	static double a2buf[8*8*2], a3buf[16];
	double *a2p[8], *a3p;
	int i, j;

	VecND_Zero(a2buf, n*n*2);
	VecND_Zero(b, n*n);
	for(i=0; i<n; i++)a2p[i]=&a2buf[i*n*2];
	for(i=0; i<n; i++)VecND_Copy(&a[i*n], a2p[i], n);
	for(i=0; i<n; i++)a2p[i][i+n]=1.0;

	for(i=0; i<n; i++)
	{
//		MatND_Dump(a2buf, n, 2*n);
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
		VecND_Scale(a2p[i], 1/a2p[i][i], a2p[i], 2*n);
		for(j=0; j<n; j++)if(i!=j)
		{
			VecND_Scale(a2p[i], -a2p[j][i], a3buf, 2*n);
			VecND_Add(a3buf, a2p[j], a2p[j], 2*n);
		}
	}

//	MatND_Dump(a2buf, n, 2*n);
//	printf("\n");

	for(i=0; i<n; i++)VecND_Copy(&a2p[i][n], &b[i*n], n);
}

/* vector functions */
VECMATH_API void VecND_Copy(double *a, double *b, int n)
{
	int i;
	for(i=0;i<n;i++)b[i]=a[i];
}

VECMATH_API void VecND2F_Copy(double *a, float *b, int n)
{
	int i;
	for(i=0;i<n;i++)b[i]=a[i];
}

VECMATH_API void VecND_ResizeVector(double *a, int an, double *b, int bn)
{
	int i;
	for(i=0;i<bn;i++)b[i]=((i>=an)?0:a[i]);
}

VECMATH_API double VecND_DotProduct(double *a, double *b, int n)
{
	int i;
	double t;
	t=0;
	for(i=0; i<n; i++)t+=a[i]*b[i];
	return(t);
}

static double _VecND_Len2(double *a, int n)
{
	int i;
	double t;

	for(i=0, t=0; i<n; i++)t+=a[i]*a[i];
	return(t);
}

VECMATH_API double VecND_Length(double *a, int n)
{
	return(sqrt(_VecND_Len2(a, n)));
}

VECMATH_API double VecND_Normalize(double *a, double *b, int n)
{
	int i;
	double t;
	t=sqrt(_VecND_Len2(a, n));
	if(t==0)t=1;
	
	for(i=0; i<n; i++)b[i]=a[i]/t;
	return(t);
}

VECMATH_API double VecND_Distance(double *a, double *b, int n)
{
	int i;
	double t;

	for(i=0, t=0; i<n; i++)t+=(a[i]-b[i])*(a[i]-b[i]);
	return(sqrt(t));	
}

VECMATH_API void VecND_Add(double *a, double *b, double *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]+b[i];
}

VECMATH_API void VecND_Subtract(double *a, double *b, double *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]-b[i];
}

VECMATH_API void VecND_Scale(double *a, double b, double *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]*b;
}

VECMATH_API void VecND_ScaleAdd(double *a, double b, double *c, double *d, int n)
{
	int i;
	for(i=0; i<n; i++)d[i]=a[i]*b+c[i];
}

VECMATH_API void VecND_AddScale(double *a, double *b, double c, double *d, int n)
{
	int i;
	for(i=0; i<n; i++)d[i]=a[i]+b[i]*c;
}

VECMATH_API void VecND_ScaleAddScale(double *a, double b, double *c, double d, double *e, int n)
{
	int i;
	for(i=0; i<n; i++)e[i]=a[i]*b+c[i]*d;
}

VECMATH_API void VecND_Zero(double *a, int n)
{
	int i;
	for(i=0; i<n; i++)a[i]=0;
}

VECMATH_API void VecND_Const(double *a, double b, int n)
{
	int i;
	for(i=0; i<n; i++)a[i]=b;
}

VECMATH_API void VecND_ProjectPointOnPlane(double *d, double *v, double *z, int n)
{
	int i;
	double f;

	f=VecND_DotProduct(v, z, n);
	for(i=0; i<n; i++)
		d[i]=v[i]-(f*z[i]);
}

VECMATH_API void Vec3D_CrossProduct (double *v1, double *v2, double *cross)
{
	cross[0] = (v1[1]*v2[2]) - (v1[2]*v2[1]);
	cross[1] = (v1[2]*v2[0]) - (v1[0]*v2[2]);
	cross[2] = (v1[0]*v2[1]) - (v1[1]*v2[0]);
}

/* functions to try for camera handling */

VECMATH_API void Vec3D_ToSphericalCoords(double *a, double *b)
{
	double a2[3];

	VecND_Normalize(a, a2, 3);

	b[0]=VecND_Length(a, 3);
	b[1]=atan2(a2[1], a2[0]);
	b[2]=acos(a2[2]);
}

VECMATH_API void Vec3D_FromSphericalCoords(double *a, double *b)
{
	b[0]=a[0]*sin(a[2])*cos(a[1]);
	b[1]=a[0]*sin(a[2])*sin(a[1]);
	b[2]=a[0]*cos(a[2]);
}

VECMATH_API void Vec16D_InvertAngles(double *amat, double *bmat)
{
	double v[3];

	MatND_Copy(amat, bmat, 4, 4);

	Vec3D_ToSphericalCoords(&amat[0], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3D_FromSphericalCoords(v, &bmat[0]);

	Vec3D_ToSphericalCoords(&amat[4], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3D_FromSphericalCoords(v, &bmat[4]);

	Vec3D_ToSphericalCoords(&amat[8], v);
	v[0]=-v[0];
//	v[1]=-v[1];
//	v[2]=-v[2];
	Vec3D_FromSphericalCoords(v, &bmat[8]);

	bmat[3]=-amat[3];
	bmat[7]=-amat[7];
	bmat[11]=-amat[11];
}

VECMATH_API void Vec3D_RotatePoint(double *p, double *angles)
{
	double		angle;
	double		sx, sy, sz, cx, cy, cz;
	double		tmp[3], point[3];

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

VECMATH_API void Vec3D_CalcAngleVectors(double *angles, double *forward, double *right, double *up)
{
	forward[0]=0;
	forward[1]=0;
	forward[2]=-1;
	Vec3D_RotatePoint(forward, angles);

	right[0]=1;
	right[1]=0;
	right[2]=0;
	Vec3D_RotatePoint(right, angles);

	up[0]=0;
	up[1]=1;
	up[2]=0;
	Vec3D_RotatePoint(up, angles);
}

VECMATH_API void Vec3D_PointToParentSpace(double *a, double *sp, double *b)
{
//	b[0]=(a[0]*sp[0])+(a[1]*sp[4])+(a[2]*sp[8])+sp[3];
//	b[1]=(a[0]*sp[1])+(a[1]*sp[5])+(a[2]*sp[9])+sp[7];
//	b[2]=(a[0]*sp[2])+(a[1]*sp[6])+(a[2]*sp[10])+sp[11];

	b[0]=(a[0]*sp[0])+(a[1]*sp[1])+(a[2]*sp[2])+sp[3];
	b[1]=(a[0]*sp[4])+(a[1]*sp[5])+(a[2]*sp[6])+sp[7];
	b[2]=(a[0]*sp[8])+(a[1]*sp[9])+(a[2]*sp[10])+sp[11];
}
