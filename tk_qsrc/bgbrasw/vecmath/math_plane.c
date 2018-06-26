#include <bgbrasw.h>

VECMATH_API int PlaneF_LinePlaneIntersect(
	float *start, float *end, float *norm,
	float *point)
{
	float dir[3], x;

	if(VecNF_DotProduct(norm, norm, 3)==0)
	{
		VecNF_Copy(start, point, 3);
		return(-1);
	}

	//calc direction
	VecNF_Subtract(end, start, dir, 3);

	x=VecNF_DotProduct(dir, norm, 3);
	x=(x<0)?-x:x;
	VecNF_Scale(dir, 1/x, dir, 3);

	//calc intersection
	x=VecNF_DotProduct(start, norm, 3)-norm[3];
	x=(x<0)?-x:x;
	VecNF_AddScale(start, dir, x, point, 3);

	return(0);
}

VECMATH_API int PlaneF_CheckLinePlane(float *start, float *end, float *norm)
{
	float x;
	int i;

	if(VecNF_DotProduct(norm, norm, 3)==0)
		return(0);

	i=0;
	x=VecNF_DotProduct(start, norm, 3)-norm[3];
	if(x>=0)i|=1;
		else i|=2;

	x=VecNF_DotProduct(end, norm, 3)-norm[3];
	if(x>=0)i|=1;
		else i|=2;

	return(i);
}

VECMATH_API int PlaneF_ClipLinePlane(float *start, float *end, float *norm)
{
	int i;

	i=PlaneF_CheckLinePlane(start, end, norm);
	if(i!=3)return(i);

	PlaneF_LinePlaneIntersect(start, end, norm, end);
	return(i);
}

VECMATH_API void PlaneF_NearestPointLinePlane(float *start, float *end,
	float *norm, float *pt)
{
	float d0, d1;
	int i;

	i=PlaneF_CheckLinePlane(start, end, norm);
	if(i==3)
	{
		PlaneF_LinePlaneIntersect(start, end, norm, pt);
		return;
	}

	d0=V3F_NDOT(start, norm);
	d1=V3F_NDOT(end, norm);

	if(fabs(d0-d1)<0.001)
	{
		V3F_SCALEADDSCALE(start, 0.5, end, 0.5, pt);
	}else if(fabs(d0)<fabs(d1))
	{
		V3F_COPY(start, pt);
	}else
	{
		V3F_COPY(end, pt);
	}
}

VECMATH_API int PlaneF_CheckPlaneBox(float *norm, float *mins, float *maxs)
{
	float d0, d1;
	int i;

	d0=0;
	d1=0;
	for(i=0; i<3; i++)
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
	if(d0>=norm[3])i|=1;	//>=dist
	if(d1<norm[3])i|=2;	//<dist

	return(i);
}

VECMATH_API void PlaneF_BoxAxisExtents(float *mins, float *maxs,
	float *norm, float *min, float *max)
{
	float d0, d1;
	int i;

	d0=0;
	d1=0;
	for(i=0; i<3; i++)
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

	*min=d1;
	*max=d0;
}

VECMATH_API void PlaneF_BoxPlaneExtents(float *mins, float *maxs,
	float *norm, float *min, float *max)
{
	PlaneF_BoxAxisExtents(mins, maxs, norm, min, max);
	*min-=norm[3];
	*max-=norm[3];
}

VECMATH_API void PlaneF_TransformVector(float *dir, float *xform, float *v)
{
	v[0]=(dir[0]*xform[0])+(dir[1]*xform[4])+(dir[2]*xform[8]);
	v[1]=(dir[0]*xform[1])+(dir[1]*xform[5])+(dir[2]*xform[9]);
	v[2]=(dir[0]*xform[2])+(dir[1]*xform[6])+(dir[2]*xform[10]);
}

VECMATH_API void PlaneF_TransformVectorT(float *dir, float *xform, float *v)
{
	v[0]=(dir[0]*xform[0])+(dir[1]*xform[1])+(dir[2]*xform[2]);
	v[1]=(dir[0]*xform[4])+(dir[1]*xform[5])+(dir[2]*xform[6]);
	v[2]=(dir[0]*xform[8])+(dir[1]*xform[9])+(dir[2]*xform[10]);
}

VECMATH_API void PlaneF_TransformNormal(float *dir, float *xform, float *norm)
{
	float f;

	PlaneF_TransformVector(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]+f;
}

VECMATH_API void PlaneF_TransformNormalT(float *dir, float *xform, float *norm)
{
	float f;

	PlaneF_TransformVectorT(dir, xform, norm);
	f=(dir[0]*xform[12])+(dir[1]*xform[13])+(dir[2]*xform[14]);
	norm[3]=dir[3]-f;
}

VECMATH_API void PlaneF_TransformNormalN(float *dir, float *xform, float *norm)
{
	float f;

	PlaneF_TransformVector(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]-f;
}

VECMATH_API void PlaneF_TransformNormalNT(float *dir, float *xform, float *norm)
{
	float f;

	PlaneF_TransformVectorT(dir, xform, norm);
	f=(norm[0]*xform[12])+(norm[1]*xform[13])+(norm[2]*xform[14]);
	norm[3]=dir[3]-f;
}

VECMATH_API void PlaneF_TransformPoint(float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[4])+
		(point[2]*xform[8])+xform[12];
	org[1]=(point[0]*xform[1])+(point[1]*xform[5])+
		(point[2]*xform[9])+xform[13];
	org[2]=(point[0]*xform[2])+(point[1]*xform[6])+
		(point[2]*xform[10])+xform[14];
}

VECMATH_API void PlaneF_TransformPointT(float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[1])+
		(point[2]*xform[2])+xform[12];
	org[1]=(point[0]*xform[4])+(point[1]*xform[5])+
		(point[2]*xform[6])+xform[13];
	org[2]=(point[0]*xform[8])+(point[1]*xform[9])+
		(point[2]*xform[10])+xform[14];
}

VECMATH_API void PlaneF_TransformPointN(float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[4])+
		(point[2]*xform[8])-xform[12];
	org[1]=(point[0]*xform[1])+(point[1]*xform[5])+
		(point[2]*xform[9])-xform[13];
	org[2]=(point[0]*xform[2])+(point[1]*xform[6])+
		(point[2]*xform[10])-xform[14];
}

VECMATH_API void PlaneF_TransformPointNT(float *point, float *xform, float *org)
{
	org[0]=(point[0]*xform[0])+(point[1]*xform[1])+
		(point[2]*xform[2])-xform[12];
	org[1]=(point[0]*xform[4])+(point[1]*xform[5])+
		(point[2]*xform[6])-xform[13];
	org[2]=(point[0]*xform[8])+(point[1]*xform[9])+
		(point[2]*xform[10])-xform[14];
}

VECMATH_API void PlaneF_TransformPointLocal(float *point, float *xform, float *org)
{
	float pt[3];

	V3F_SUB(pt, xform+3*4, pt);
	org[0]=(pt[0]*xform[0])+(pt[1]*xform[1])+(pt[2]*xform[2]);
	org[1]=(pt[0]*xform[4])+(pt[1]*xform[5])+(pt[2]*xform[6]);
	org[2]=(pt[0]*xform[8])+(pt[1]*xform[9])+(pt[2]*xform[10]);
}

VECMATH_API void PlaneF_AdjacentNormals(float *n, float *x, float *y)
{
	float f, g;

	V3F_ZERO(x);
	V3F_ZERO(y);

	if(fabs(n[0])>fabs(n[1]))
	{
		if(fabs(n[0])>fabs(n[2]))
		{
			x[1]=1;
			y[2]=1;
		}else
		{
			x[0]=1;
			y[1]=1;
		}
	}else
	{
		if(fabs(n[1])>fabs(n[2]))
		{
			x[0]=1;
			y[2]=1;
		}else
		{
			x[0]=1;
			y[1]=1;
		}
	}

	f=V3F_DOT(x, n);
	g=V3F_DOT(y, n);
	V3F_ADDSCALE(x, n, -f, x);
	V3F_ADDSCALE(y, n, -g, y);

	V3F_NORMALIZE(x, x);
	V3F_NORMALIZE(y, y);
}

VECMATH_API void PlaneF_AdjacentNormals2(float *norm, float *udir, float *vdir)
{
	float f;

	V3F_ZERO(udir);
	V3F_ZERO(vdir);

	//UxV=N

	if(fabs(norm[0])>fabs(norm[1]))
	{
		if(fabs(norm[0])>fabs(norm[2]))
		{
			udir[(norm[0]>0)?1:2]=1; 
			vdir[(norm[0]>0)?2:1]=1;
		}else
		{
			udir[(norm[2]>0)?0:1]=1; 
			vdir[(norm[2]>0)?1:0]=1;
		}
	}else
	{
		if(fabs(norm[1])>fabs(norm[2]))
		{
			udir[(norm[1]>0)?2:0]=1; 
			vdir[(norm[1]>0)?0:2]=1;
		}else
		{
			udir[(norm[2]>0)?0:1]=1; 
			vdir[(norm[2]>0)?1:0]=1;
		}
	}

	f=V3F_DOT(udir, norm);
	V3F_ADDSCALE(udir, norm, -f, udir);
	f=V3F_DOT(vdir, norm);
	V3F_ADDSCALE(vdir, norm, -f, vdir);

	V3F_NORMALIZE(udir, udir);
	V3F_NORMALIZE(vdir, vdir);
}
