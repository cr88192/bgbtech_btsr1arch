#include <bgbrasw.h>

/* vector functions */
VECMATH_API void VecNF_Copy(float *a, float *b, int n)
{
	int i;
	for(i=0;i<n;i++)b[i]=a[i];
}

VECMATH_API void VecNF2D_Copy(float *a, double *b, int n)
{
	int i;
	for(i=0;i<n;i++)b[i]=a[i];
}

VECMATH_API void VecNF_ResizeVector(float *a, int an, float *b, int bn)
{
	int i;
	for(i=0;i<bn;i++)b[i]=((i>=an)?0:a[i]);
}

VECMATH_API float VecNF_DotProduct(float *a, float *b, int n)
{
	int i;
	float t;
	t=0;
	for(i=0; i<n; i++)t+=a[i]*b[i];
	return(t);
}

static float _VecNF_Len2(float *a, int n)
{
	int i;
	float t;

	for(i=0, t=0; i<n; i++)t+=a[i]*a[i];
	return(t);
}

VECMATH_API float VecNF_Length(float *a, int n)
{
	return(sqrt(_VecNF_Len2(a, n)));
}

VECMATH_API float VecNF_Normalize(float *a, float *b, int n)
{
	int i;
	float t;
	t=sqrt(_VecNF_Len2(a, n));
	if(t==0)t=1;
	
	for(i=0; i<n; i++)b[i]=a[i]/t;
	return(t);
}

VECMATH_API float VecNF_Distance(float *a, float *b, int n)
{
	int i;
	float t;

	for(i=0, t=0; i<n; i++)t+=(a[i]-b[i])*(a[i]-b[i]);
	return(sqrt(t));	
}

VECMATH_API void VecNF_Add(float *a, float *b, float *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]+b[i];
}

VECMATH_API void VecNF_Subtract(float *a, float *b, float *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]-b[i];
}

VECMATH_API void VecNF_Scale(float *a, float b, float *c, int n)
{
	int i;
	for(i=0; i<n; i++)c[i]=a[i]*b;
}

VECMATH_API void VecNF_ScaleAdd(float *a, float b, float *c, float *d, int n)
{
	int i;
	for(i=0; i<n; i++)d[i]=a[i]*b+c[i];
}

VECMATH_API void VecNF_AddScale(float *a, float *b, float c, float *d, int n)
{
	int i;
	for(i=0; i<n; i++)d[i]=a[i]+b[i]*c;
}

VECMATH_API void VecNF_ScaleAddScale(float *a, float b, float *c, float d, float *e, int n)
{
	int i;
	for(i=0; i<n; i++)e[i]=a[i]*b+c[i]*d;
}

VECMATH_API void VecNF_Zero(float *a, int n)
{
	int i;
	for(i=0; i<n; i++)a[i]=0;
}

VECMATH_API void VecNF_Const(float *a, float b, int n)
{
	int i;
	for(i=0; i<n; i++)a[i]=b;
}

VECMATH_API void VecNF_ProjectPointOnPlane(float *d, float *v, float *z, int n)
{
	int i;
	float f;

	f=VecNF_DotProduct(v, z, n);
	for(i=0; i<n; i++)
		d[i]=v[i]-(f*z[i]);
}

VECMATH_API void VecNF_ProjectPointOnPlane2(float *d, float *v, float *z, int n)
{
	int i;
	float f;

	f=VecNF_DotProduct(v, z, n)-z[n];
	for(i=0; i<n; i++)
		d[i]=v[i]-(f*z[i]);
}

VECMATH_API void VecNF_ProjectPointBelowPlane(float *d, float *v, float *z, int n)
{
	int i;
	float f;

	f=VecNF_DotProduct(v, z, n)-z[n];
	if(f>0)for(i=0; i<n; i++)
		d[i]=v[i]-(f*z[i]);
}

VECMATH_API void VecNF_ProjectPointAbovePlane(float *d, float *v, float *z, int n)
{
	int i;
	float f;

	f=VecNF_DotProduct(v, z, n)-z[n];
	if(f<0)for(i=0; i<n; i++)
		d[i]=v[i]-(f*z[i]);
}

VECMATH_API void VecNF_LinePlaneIntersect(
	float *start, float *end, float *norm,
	float *point, int n)
{
	float dir[16], x;

	if(VecNF_DotProduct(norm, norm, n)==0)
	{
		VecNF_Copy(start, point, n);
		return;
	}

	//calc direction
	VecNF_Subtract(end, start, dir, n);

	x=VecNF_DotProduct(dir, norm, n);
	x=(x<0)?-x:x;
	VecNF_Scale(dir, 1/x, dir, n);

	//calc intersection
	x=VecNF_DotProduct(start, norm, n)-norm[n];
	x=(x<0)?-x:x;
	VecNF_AddScale(start, dir, x, point, n);
}

VECMATH_API int VecNF_CheckPlaneBox(
	float *norm, float *mins, float *maxs, int n)
{
	float d0, d1;
	int i;

	d0=0;
	d1=0;
	for(i=0; i<n; i++)
	{
		if(norm[i]>0)
		{
			d0+=maxs[i]*norm[i];
			d1+=mins[i]*norm[i];
		}else
		{
			d0+=mins[i]*norm[i];
			d1+=maxs[i]*norm[i];
		}
	}

	i=0;
	if(d0>=norm[n])i|=1;	//>=dist
	if(d1<norm[n])i|=2;	//<dist

	return(i);
}
