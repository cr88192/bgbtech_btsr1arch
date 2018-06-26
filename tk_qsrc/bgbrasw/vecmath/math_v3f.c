#include <bgbrasw.h>

VECMATH_API void Vec3F_Set(float *a, float x, float y, float z)
	{ a[0]=x; a[1]=y; a[2]=z; }
VECMATH_API void Vec3F_Zero(float *a)
	{ a[0]=0; a[1]=0; a[2]=0; }

VECMATH_API void Vec3F_Copy(float *a, float *b)
	{ b[0]=a[0]; b[1]=a[1]; b[2]=a[2]; }
VECMATH_API void Vec3F_Add(float *a, float *b, float *c)
	{ c[0]=a[0]+b[0]; c[1]=a[1]+b[1]; c[2]=a[2]+b[2]; }
VECMATH_API void Vec3F_Subtract(float *a, float *b, float *c)
	{ c[0]=a[0]-b[0]; c[1]=a[1]-b[1]; c[2]=a[2]-b[2]; }
VECMATH_API void Vec3F_Scale(float *a, float b, float *c)
	{ c[0]=a[0]*b; c[1]=a[1]*b; c[2]=a[2]*b; }
VECMATH_API float Vec3F_DotProduct(float *a, float *b)
	{ return((a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2])); }
VECMATH_API float Vec3F_Length(float *a)
	{ return(sqrt(Vec3F_DotProduct(a, a))); }

VECMATH_API void Vec3F_ScaleAdd(float *a, float b, float *c, float *d)
	{ d[0]=a[0]*b+c[0]; d[1]=a[1]*b+c[1]; d[2]=a[2]*b+c[2]; }
VECMATH_API void Vec3F_AddScale(float *a, float *b, float c, float *d)
	{ d[0]=a[0]+b[0]*c; d[1]=a[1]+b[1]*c; d[2]=a[2]+b[2]*c; }
VECMATH_API void Vec3F_ScaleAddScale(float *a, float b, float *c, float d, float *e)
	{ e[0]=a[0]*b+c[0]*d; e[1]=a[1]*b+c[1]*d; e[2]=a[2]*b+c[2]*d; }

VECMATH_API float Vec3F_Normalize(float *a, float *b)
{
	float f, g;

	f=sqrt(Vec3F_DotProduct(a, a));
	g=(f==0)?1:f;
	b[0]=a[0]/g;
	b[1]=a[1]/g;
	b[2]=a[2]/g;

	return(f);
}

VECMATH_API float Vec3F_Distance(float *a, float *b)
{
	float dx, dy, dz;
	dx=a[0]-b[0]; dy=a[1]-b[1]; dz=a[2]-b[2];
	return(sqrt(dx*dx+dy*dy+dz*dz));	
}


VECMATH_API void Vec3F_CrossProduct(float *a, float *b, float *c)
{
	c[0]=(a[1]*b[2])-(a[2]*b[1]);
	c[1]=(a[2]*b[0])-(a[0]*b[2]);
	c[2]=(a[0]*b[1])-(a[1]*b[0]);
}

VECMATH_API float Vec3F_NearestRayDistance(float *a, float *adir, float *b, float *bdir)
{
	float c[3], cdir[3], f, g;

	//vector perpindicular to both lines
	Vec3F_CrossProduct(adir, bdir, cdir);

	Vec3F_Subtract(b, a, c);
	f=fabs(Vec3F_DotProduct(c, cdir));
	g=VecNF_Length(cdir, 3);

	return(f/g);
}

#if 0
// float Vec3F_NearestLineDistance(float *As, float *Ae, float *Bs, float *Be)
{
	float adir[3], bdir[3];
	float ae[4], be[4];
	float c[3], cdir[3], f, g, h;

	Vec3F_Subtract(Ae, As, adir);
	Vec3F_Subtract(Be, Bs, bdir);
	Vec3F_Normalize(adir, adir);
	Vec3F_Normalize(bdir, bdir);

	ae[0]=Vec3F_DotProduct(As, adir);
	ae[1]=Vec3F_DotProduct(Ae, adir);
	ae[2]=Vec3F_DotProduct(Bs, adir);
	ae[3]=Vec3F_DotProduct(Be, adir);

	be[0]=Vec3F_DotProduct(Bs, bdir);
	be[1]=Vec3F_DotProduct(Be, bdir);
	be[2]=Vec3F_DotProduct(As, bdir);
	be[3]=Vec3F_DotProduct(Ae, bdir);

	if(ae[2]>ae[3])
	{
		f=ae[2];
		ae[2]=ae[3];
		ae[3]=f;
	}
	if(be[2]>be[3])
	{
		f=be[2];
		be[2]=be[3];
		be[3]=f;
	}

	if((ae[3]<ae[0]) || (ae[2]>ae[1]))
	{
		f=fabs(ae[3]-ae[1]);
		g=fabs(ae[2]-ae[0]);
		return((f<g)?f:g);
	}
	if((be[3]<be[0]) || (be[2]>be[1]))
	{
		f=fabs(be[3]-be[1]);
		g=fabs(be[2]-be[0]);
		return((f<g)?f:g);
	}

	//vector perpindicular to both lines
	Vec3F_CrossProduct(adir, bdir, cdir);
	Vec3F_Normalize(cdir, cdir);

	Vec3F_Subtract(b, a, c);
	f=fabs(Vec3F_DotProduct(c, cdir));
	return(f);
}
#endif

VECMATH_API float Vec3F_NearestRayPoints(float *a, float *adir, float *b, float *bdir,
	float *pA, float *pB)
{
	float cdir[3], f;
	float dir2[3], g;
	float na[4], nb[4];
	float pa[3], pb[3];

	//vector perpindicular to both lines
	Vec3F_CrossProduct(adir, bdir, cdir);
	f=Vec3F_Normalize(cdir, cdir);

	//lines are near parallel
	if(fabs(f)<0.001)
	{
		if(pA)VecNF_Copy(a, pA, 3);
		if(pB)VecNF_Copy(b, pB, 3);
		return(VecNF_Distance(a, b, 3));
	}

	//plane for each line
	Vec3F_CrossProduct(cdir, adir, na);
	na[3]=Vec3F_DotProduct(a, na);
	Vec3F_CrossProduct(cdir, bdir, nb);
	nb[3]=Vec3F_DotProduct(b, nb);

	//line A plane B intersect
	g=fabs(Vec3F_DotProduct(adir, nb));
	VecNF_Scale(adir, 1/g, dir2, 3);
	g=fabs(Vec3F_DotProduct(a, nb)-nb[3]);
	VecNF_AddScale(a, dir2, g, pa, 3);

	//line B plane A intersect
	g=fabs(Vec3F_DotProduct(bdir, na));
	VecNF_Scale(bdir, 1/g, dir2, 3);
	g=fabs(Vec3F_DotProduct(b, na)-na[3]);
	VecNF_AddScale(b, dir2, g, pb, 3);


	if(pA)VecNF_Copy(pa, pA, 3);
	if(pB)VecNF_Copy(pb, pB, 3);
	f=VecNF_Distance(pa, pb, 3);
	return(f);
}


VECMATH_API float Vec3F_NearestLinePoints(float *As, float *Ae, float *Bs, float *Be,
	float *pA, float *pB)
{
	float adir[4], bdir[4];
	float dir2[3];
	float na[4], nb[4], nc[4];
	float pa[3], pb[3];
	float f, g;

	Vec3F_Subtract(Ae, As, adir);
	Vec3F_Subtract(Be, Bs, bdir);
	Vec3F_Normalize(adir, adir);
	Vec3F_Normalize(bdir, bdir);
	adir[3]=Vec3F_DotProduct(As, adir);
	bdir[3]=Vec3F_DotProduct(Bs, bdir);

	//plane perpindicular to both lines
	Vec3F_CrossProduct(adir, bdir, nc);
	f=Vec3F_Normalize(nc, nc);

	//lines are near parallel
	if(fabs(f)<0.001)
	{
		if(pA)Vec3F_Copy(As, pA);
		if(pB)Vec3F_Copy(Bs, pB);
		return(VecNF_Distance(As, Bs, 3));
	}

	//figure the plane dist
	nc[3]=Vec3F_DotProduct(As, nc);

	//plane for each line
	Vec3F_CrossProduct(nc, adir, na);
	Vec3F_Normalize(na, na);
	na[3]=Vec3F_DotProduct(As, na);

	Vec3F_CrossProduct(nc, bdir, nb);
	Vec3F_Normalize(nb, nb);
	nb[3]=Vec3F_DotProduct(Bs, nb);


	//check if lines fall one only 1 side of the plane
	//in this case, the nearest point is one of the ends

	f=Vec3F_DotProduct(As, nb)-nb[3];
	g=Vec3F_DotProduct(Ae, nb)-nb[3];
	if((f*g)>0)
	{
		f=fabs(f);
		g=fabs(g);

		if(f<g)
		{
			if(pA)Vec3F_Copy(As, pA);
			if(pB)
			{
				g=Vec3F_DotProduct(As, bdir)-bdir[3];
				Vec3F_AddScale(Bs, bdir, g, pB);
			}
			return(f);
		}else
		{
			if(pA)Vec3F_Copy(Ae, pA);
			if(pB)
			{
				f=Vec3F_DotProduct(Ae, bdir)-bdir[3];
				Vec3F_AddScale(Bs, bdir, f, pB);
			}
			return(g);
		}
	}

	f=Vec3F_DotProduct(Bs, na)-na[3];
	g=Vec3F_DotProduct(Be, na)-na[3];
	if((f*g)>0)
	{
		f=fabs(f);
		g=fabs(g);

		if(f<g)
		{
			if(pA)
			{
				g=Vec3F_DotProduct(Bs, adir)-adir[3];
				Vec3F_AddScale(As, adir, g, pA);
			}
			if(pB)Vec3F_Copy(Bs, pB);
			return(f);
		}else
		{
			if(pA)
			{
				f=Vec3F_DotProduct(Be, adir)-adir[3];
				Vec3F_AddScale(As, adir, f, pA);
			}
			if(pB)Vec3F_Copy(Be, pB);
			return(g);
		}
	}

	//line A plane B intersect
	g=fabs(Vec3F_DotProduct(adir, nb));
	VecNF_Scale(adir, 1/g, dir2, 3);
	g=fabs(Vec3F_DotProduct(As, nb)-nb[3]);
	VecNF_AddScale(As, dir2, g, pa, 3);

	//line B plane A intersect
	g=fabs(Vec3F_DotProduct(bdir, na));
	VecNF_Scale(bdir, 1/g, dir2, 3);
	g=fabs(Vec3F_DotProduct(Bs, na)-na[3]);
	VecNF_AddScale(Bs, dir2, g, pb, 3);

	if(pA)VecNF_Copy(pa, pA, 3);
	if(pB)VecNF_Copy(pb, pB, 3);
	f=VecNF_Distance(pa, pb, 3);
	return(f);
}

VECMATH_API float Vec3F_NearestLineDistance(float *As, float *Ae, float *Bs, float *Be)
{
	float u[3], v[3], w[3], dP[3];
	float a, b, c, d, e, f;
	float sc, sN, sD;
	float tc, tN, tD;

	Vec3F_Subtract(Ae, As, u);
	Vec3F_Subtract(Be, Bs, v);
	Vec3F_Subtract(As, Bs, w);

	a=Vec3F_DotProduct(u, u);
	b=Vec3F_DotProduct(u, v);
	c=Vec3F_DotProduct(v, v);
	d=Vec3F_DotProduct(u, w);
	e=Vec3F_DotProduct(v, w);
	f=a*c-b*b;

	sD=f;
	tD=f;

	if(f<0.00000001)
	{
		sN=0;
		sD=1;
		tN=e;
		tD=c;
	}else
	{
		sN=(b*e-c*d);
		tN=(a*e-b*d);
		if(sN<0)
		{
			sN=0;
			tN=e;
			tD=c;
	        }else if(sN > sD)
		{
			sN=sD;
			tN=e+b;
			tD=c;
		}
	}

	if(tN<0)
	{
		tN=0;
		if(d>=0)
		{
			sN=0;
		}else if(d<=a)
		{
			sN=sD;
		}else
		{
			sN=-d;
			sD=a;
		}
	}else if(tN>tD)
	{
		tN=tD;
		if((b-d)<0)
		{
			sN=0;
		}else if((b-d)>a)
		{
			sN=sD;
		}else
		{
			sN=b-d;
			sD=a;
		}
	}

	sc=((fabs(sN)<0.00000001)?0.0:(sN/sD));
	tc=((fabs(tN)<0.00000001)?0.0:(tN/tD));

	VC3(dP, VA3(w, VS3(VM3(u, sc), VM3(v, tc))));
	return VL3(dP);
}


VECMATH_API int Vec3F_BoxTouchP(float *ma, float *na, float *mb, float *nb)
{
	if(na[0]<mb[0])return(0);
	if(na[1]<mb[1])return(0);
	if(na[2]<mb[2])return(0);
	if(nb[0]<ma[0])return(0);
	if(nb[1]<ma[1])return(0);
	if(nb[2]<ma[2])return(0);

	return(1);
}

VECMATH_API int Vec3F_BoxCollideP(float *ma, float *na, float *mb, float *nb)
{
	if(na[0]<=mb[0])return(0);
	if(na[1]<=mb[1])return(0);
	if(na[2]<=mb[2])return(0);
	if(nb[0]<=ma[0])return(0);
	if(nb[1]<=ma[1])return(0);
	if(nb[2]<=ma[2])return(0);

	return(1);
}

VECMATH_API void Vec3F_LinePlaneIntersect(float *start, float *end, float *norm,
	float *point)
{
	float dir[3], x;

	if(Vec3F_DotProduct(norm, norm)==0)
	{
		Vec3F_Copy(start, point);
		return;
	}

	//calc direction
	Vec3F_Subtract(end, start, dir);

	x=Vec3F_DotProduct(dir, norm);
	x=(x<0)?-x:x;
	Vec3F_Scale(dir, 1/x, dir);

	//calc intersection
	x=Vec3F_DotProduct(start, norm)-norm[3];
	x=(x<0)?-x:x;
	Vec3F_AddScale(start, dir, x, point);
}


VECMATH_API int Vec3F_LineBoxCollideP(float *m, float *n, float *s, float *e)
{
	float d[4];

	if((s[0]<=m[0]) && (e[0]<=m[0]))return(0);
	V4F_SET(d, 1, 0, 0, m[0]);
	if(s[0]<m[0])Vec3F_LinePlaneIntersect(s, e, d, s);
	if(e[0]<m[0])Vec3F_LinePlaneIntersect(s, e, d, e);

	if((s[0]>=n[0]) && (e[0]>=n[0]))return(0);
	V4F_SET(d, 1, 0, 0, n[0]);
	if(s[0]>n[0])Vec3F_LinePlaneIntersect(s, e, d, s);
	if(e[0]>n[0])Vec3F_LinePlaneIntersect(s, e, d, e);


	if((s[1]<m[1]) && (e[1]<m[1]))return(0);
	V4F_SET(d, 0, 1, 0, m[1]);
	if(s[1]<m[1])Vec3F_LinePlaneIntersect(s, e, d, s);
	if(e[1]<m[1])Vec3F_LinePlaneIntersect(s, e, d, e);

	if((s[1]>n[1]) && (e[1]>n[1]))return(0);
	V4F_SET(d, 0, 1, 0, n[1]);
	if(s[1]>n[1])Vec3F_LinePlaneIntersect(s, e, d, s);
	if(e[1]>n[1])Vec3F_LinePlaneIntersect(s, e, d, e);

	if((s[2]<m[2]) && (e[2]<m[2]))return(0);
	if((s[2]>n[2]) && (e[2]>n[2]))return(0);

	return(1);
}



VECMATH_API void Vec3F_QuatToEulerYXZ(float *quat, float *ang)
{
	float q[4];
	double asinarg;

	QuatF_Normalize(quat, q);

	asinarg = 2*(q[1]*q[2] + q[3]*q[0]);
	if (asinarg<-1)asinarg=-1;
	if (asinarg>1)asinarg=1;

	ang[0] =  asin( asinarg ) * (180/M_PI);

	ang[1] =  atan2(-2*(q[0]*q[2] - q[3]*q[1]),
		q[3]*q[3] - q[0]*q[0] - q[1]*q[1] + q[2]*q[2] ) *
		(180/M_PI);

	ang[2] =  atan2(-2*(q[0]*q[1] - q[3]*q[2]),
		q[3]*q[3] - q[0]*q[0] + q[1]*q[1] - q[2]*q[2] ) *
		(180/M_PI);
}

VECMATH_API void Vec3F_QuatToEulerZXY(float *quat, float *ang)
{
	float q[4];
	double asinarg;

	QuatF_Normalize(quat, q);

	asinarg = 2*(q[1]*q[2] + q[3]*q[0]);
	if (asinarg<-1)asinarg=-1;
	if (asinarg>1)asinarg=1;

	ang[0] =  asin( asinarg ) * (180/M_PI);

	ang[1] =  atan2( 2*(q[0]*q[2] + q[3]*q[1]),
		q[3]*q[3] - q[0]*q[0] - q[1]*q[1] + q[2]*q[2] ) *
		(180/M_PI);

	ang[2] =  atan2( 2*(q[0]*q[1] + q[3]*q[2]),
		q[3]*q[3] - q[0]*q[0] + q[1]*q[1] - q[2]*q[2] ) *
		(180/M_PI);
}

VECMATH_API void Vec3F_QuatToEulerXYZ(float *quat, float *ang)
{
	float q[4];
	double asinarg;

	QuatF_Normalize(quat, q);

	ang[0] = atan2( 2*(q[1]*q[2] + q[3]*q[0]),
		q[3]*q[3] - q[0]*q[0] - q[1]*q[1] + q[2]*q[2] ) *
		(180/M_PI);

	asinarg = -2*(q[0]*q[2] - q[3]*q[1]);
	if (asinarg<-1)asinarg=-1;
	if (asinarg>1)asinarg=1;

	ang[1] = asin( asinarg ) * (180/M_PI);

	ang[2] = atan2( 2*(q[0]*q[1] + q[3]*q[2]),
		q[3]*q[3] + q[0]*q[0] - q[1]*q[1] - q[2]*q[2] ) *
		(180/M_PI);
}

VECMATH_API void Vec3F_QuatFromEulerZXY(float *quat, float *ang)
{
	const double cx=cos(0.5*ang[0]*(M_PI/180));
	const double cy=cos(0.5*ang[1]*(M_PI/180));
	const double cz=cos(0.5*ang[2]*(M_PI/180));
	const double sx=sin(0.5*ang[0]*(M_PI/180));
	const double sy=sin(0.5*ang[1]*(M_PI/180));
	const double sz=sin(0.5*ang[2]*(M_PI/180));
	quat[3] = cx*cy*cz + sx*sy*sz;
	quat[0] = sx*cy*cz + cx*sy*sz;
	quat[1] = cx*sy*cz - sx*cy*sz;
	quat[2] = cx*cy*sz - sx*sy*cz;

	QuatF_Normalize(quat, quat);
}


VECMATH_API void Vec3F_Mat3ToEuler(float *mat, float *ang)
{
//	ang[0]=asin(mat[1*3+2])*(180/M_PI);
//	ang[1]=0;
//	ang[2]=atan2(mat[1*3+0], mat[1*3+1])*(180/M_PI);

	ang[0]=-asin(mat[1*3+2])*(180/M_PI);
	ang[1]=-asin(mat[0*3+2])*(180/M_PI);
	ang[2]=atan2(mat[1*3+0], mat[1*3+1])*(180/M_PI);

	if(mat[2*3+2]<0)
	{
//		ang[0]=180-ang[0];
		ang[1]=180-ang[1];
		ang[2]=360-ang[2];
	}

//	if(ang[0]<-90)ang[0]=-90;
//	if(ang[0]>90)ang[0]=90;
//	if(ang[2]<0)ang[2]+=360;
}


VECMATH_API void Vec3F_QuatToEulerZXZ(float *quat, float *ang)
{
	float norm[4];
	float f;

	QuatF_ToAxis(quat, norm, norm+3);

	f=Vec3F_Normalize(norm, norm);
	if(f<0.001) { norm[0]=0; norm[1]=0; norm[2]=1; }

	ang[0]=acos(norm[2])*(180/M_PI);
	ang[1]=atan2(norm[1], norm[0])*(180/M_PI);
	ang[2]=norm[3]*(180/M_PI);
}

VECMATH_API void Vec3F_AxisToRot3(float *axis, float angle, float *rot)
{
	float norm[4];
	float f;

	f=Vec3F_Normalize(axis, norm);
	if(f<0.001) { norm[0]=0; norm[1]=0; norm[2]=1; }

	rot[0]=acos(norm[2])*(180/M_PI);
	rot[1]=atan2(norm[1], norm[0])*(180/M_PI);
	rot[2]=angle;
}

VECMATH_API void Vec3F_RotatePointAxis(
	float *src, float *axis, float angle, float *dst)
{
	float tm[9];
	Mat3F_AxisTo3Matrix(axis, angle, tm);
	Mat3F_TransformVector(src, tm, dst);
}
