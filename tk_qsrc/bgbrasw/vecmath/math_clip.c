#include <bgbrasw.h>

VECMATH_API int Hull_LinePlaneIntersect(float *start, float *end,
	float *norm, float *point)
{
	float dir[3], x;

	//calc direction
	dir[0]=end[0]-start[0];
	dir[1]=end[1]-start[1];
	dir[2]=end[2]-start[2];

	x=V3F_DOT(dir, norm);
	if(x==0)return(-1);

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;
	dir[1]*=x;
	dir[2]*=x;

	//calc intersection
	x=V3F_NDOT(start, norm);

	x=(x<0)?-x:x;
	point[0]=start[0]+dir[0]*x;
	point[1]=start[1]+dir[1]*x;
	point[2]=start[2]+dir[2]*x;

	return(0);
}

VECMATH_API int Hull_LinePlaneIntersectST(float *sv, float *sst,
	float *ev, float *est,
	float *norm, float *ov, float *ost)
{
	float dir[3], stdir[2], x;

	//calc direction
	dir[0]=ev[0]-sv[0];
	dir[1]=ev[1]-sv[1];
	dir[2]=ev[2]-sv[2];

	stdir[0]=est[0]-sst[0];
	stdir[1]=est[1]-sst[1];


	x=V3F_DOT(dir, norm);
	if(x==0)
	{
		printf("Hull_LinePlaneIntersectST: Parallel Clip\n");
		V3F_COPY(sv, ov);
		V2F_COPY(sst, ost);
		return(-1);
	}

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;
	dir[1]*=x;
	dir[2]*=x;

	stdir[0]*=x;
	stdir[1]*=x;

	//calc intersection
	x=V3F_NDOT(sv, norm);

	x=(x<0)?-x:x;
	ov[0]=sv[0]+dir[0]*x;
	ov[1]=sv[1]+dir[1]*x;
	ov[2]=sv[2]+dir[2]*x;

	ost[0]=sst[0]+stdir[0]*x;
	ost[1]=sst[1]+stdir[1]*x;

	return(0);
}


VECMATH_API int Hull_LinePlaneIntersectSTNVRGBA(
	float *norm,
	float *sv, float *sst, float *snv, float *sclr,
	float *ev, float *est, float *env, float *eclr,
	float *ov, float *ost, float *onv, float *oclr,
	int flags)
{
	float dir[4], stdir[2], nvdir[4], cldir[4], x;

	//calc direction
	dir[0]=ev[0]-sv[0];
	dir[1]=ev[1]-sv[1];
	dir[2]=ev[2]-sv[2];
	if(flags&1)
		{ dir[3]=ev[3]-sv[3]; }

	stdir[0]=est[0]-sst[0];
	stdir[1]=est[1]-sst[1];

	nvdir[0]=env[0]-snv[0];
	nvdir[1]=env[1]-snv[1];
	nvdir[2]=env[2]-snv[2];
	if(flags&1)
		{ nvdir[3]=env[3]-snv[3]; }

	cldir[0]=eclr[0]-sclr[0];
	cldir[1]=eclr[1]-sclr[1];
	cldir[2]=eclr[2]-sclr[2];
	cldir[3]=eclr[3]-sclr[3];


	x=V3F_DOT(dir, norm);
	if(x==0)
	{
		printf("Hull_LinePlaneIntersectST: Parallel Clip\n");

		if(flags&1)
		{
			V4F_COPY(sv, ov);
			V2F_COPY(sst, ost);
			V4F_COPY(snv, onv);
			V4F_COPY(sclr, oclr);
		}else
		{
			V3F_COPY(sv, ov);
			V2F_COPY(sst, ost);
			V4F_COPY(snv, onv);
			V4F_COPY(sclr, oclr);
		}
		return(-1);
	}

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;		dir[1]*=x;		dir[2]*=x;		dir[3]*=x;
	stdir[0]*=x;	stdir[1]*=x;
	nvdir[0]*=x;	nvdir[1]*=x;	nvdir[2]*=x;	cldir[3]*=x;
	cldir[0]*=x;	cldir[1]*=x;	cldir[2]*=x;	cldir[3]*=x;

	//calc intersection
	x=V3F_NDOT(sv, norm);

	x=(x<0)?-x:x;
	ov[0]=sv[0]+dir[0]*x;
	ov[1]=sv[1]+dir[1]*x;
	ov[2]=sv[2]+dir[2]*x;
	if(flags&1)
		{ ov[3]=sv[3]+dir[3]*x; }

	ost[0]=sst[0]+stdir[0]*x;
	ost[1]=sst[1]+stdir[1]*x;

	onv[0]=snv[0]+nvdir[0]*x;
	onv[1]=snv[1]+nvdir[1]*x;
	onv[2]=snv[2]+nvdir[2]*x;
	if(flags&1)
		{ onv[3]=snv[3]+nvdir[3]*x; }

	oclr[0]=sclr[0]+cldir[0]*x;
	oclr[1]=sclr[1]+cldir[1]*x;
	oclr[2]=sclr[2]+cldir[2]*x;
	oclr[3]=sclr[3]+cldir[3]*x;

	return(0);
}

VECMATH_API void Hull_AdjacentNormals(
	float *norm, float *udir, float *vdir)
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

VECMATH_API void Hull_AdjacentNormals2(
	float *norm, float *udir, float *vdir)
{
	float f;

	V3F_ZERO(udir);
	V3F_ZERO(vdir);

	//UxV=N

	if(fabs(norm[0])>fabs(norm[1]))
	{
		if(fabs(norm[0])>fabs(norm[2]))
		{
			udir[1]=(norm[0]>0)?1:-1; 
			vdir[2]=1;
		}else
		{
			udir[0]=(norm[2]>0)?1:-1; 
			vdir[1]=1;
		}
	}else
	{
		if(fabs(norm[1])>fabs(norm[2]))
		{
			udir[0]=(norm[1]>0)?-1:1;
			vdir[2]=1; 
		}else
		{
			udir[0]=(norm[2]>0)?1:-1; 
			vdir[1]=1;
		}
	}

	f=V3F_DOT(udir, norm);
	V3F_ADDSCALE(udir, norm, -f, udir);
	f=V3F_DOT(vdir, norm);
	V3F_ADDSCALE(vdir, norm, -f, vdir);

	V3F_NORMALIZE(udir, udir);
	V3F_NORMALIZE(vdir, vdir);
}

VECMATH_API void Hull_AdjacentNormals21(
	float *norm, float *udir, float *vdir)
{
	float f;

	V3F_ZERO(udir);
	V3F_ZERO(vdir);

	//UxV=N

	if(fabs(norm[0])>fabs(norm[1]))
	{
		if(fabs(norm[0])>fabs(norm[2]))
		{
			udir[1]=1; 
			vdir[2]=1;
		}else
		{
			udir[0]=1; 
			vdir[1]=1;
		}
	}else
	{
		if(fabs(norm[1])>fabs(norm[2]))
		{
			udir[0]=1;
			vdir[2]=1; 
		}else
		{
			udir[0]=1; 
			vdir[1]=1;
		}
	}

//	f=V3F_DOT(udir, norm);
//	V3F_ADDSCALE(udir, norm, -f, udir);
//	f=V3F_DOT(vdir, norm);
//	V3F_ADDSCALE(vdir, norm, -f, vdir);

	V3F_NORMALIZE(udir, udir);
	V3F_NORMALIZE(vdir, vdir);
}

VECMATH_API void Hull_AdjacentNormals22(
	float *norm, float *udir, float *vdir)
{
	float f, ax, ay, az;

	V3F_ZERO(udir);
	V3F_ZERO(vdir);

	//UxV=N

	ax=fabs(norm[0]); ay=fabs(norm[1]); az=fabs(norm[2]);

	if(ax>=ay && ax>=az) { udir[1]=1; vdir[2]=1; }
		else if(ay>=ax && ay>=az) { udir[0]=1; vdir[2]=1; }
		else { udir[0]=1; vdir[1]=1; }

	V3F_NORMALIZE(udir, udir);
	V3F_NORMALIZE(vdir, vdir);
}

VECMATH_API void Hull_AdjacentNormals23(
	float *norm, float *udir, float *vdir)
{
	float tn[3];
	float f, ax, ay, az;

	V3F_ZERO(udir);
	V3F_ZERO(vdir);

	//UxV=N

	ax=fabs(norm[0]); ay=fabs(norm[1]); az=fabs(norm[2]);

	if(ax>=ay && ax>=az)
	{
		udir[1]=1; vdir[2]=1;
		if(norm[0]<0)
			udir[1]=-1;
	}else if(ay>=ax && ay>=az)
	{
		udir[0]=1; vdir[2]=1;
		if(norm[1]<0)
			udir[0]=-1;
	}else
	{
		udir[1]=1; vdir[0]=1;
	}

//	V3F_CROSS(udir, vdir, tn);
//	if(V3F_DOT(tn, norm)<0)
//		{ V3F_SCALE(udir, -1, udir); }

//	if(((ax*norm[0])+(ay*norm[1])+(az*norm[2]))<0)
//		{ V3F_SCALE(udir, -1, udir); }

	f=V3F_DOT(udir, norm);
	V3F_ADDSCALE(udir, norm, -f, udir);
	f=V3F_DOT(vdir, norm);
	V3F_ADDSCALE(vdir, norm, -f, vdir);

	V3F_NORMALIZE(udir, udir);
	V3F_NORMALIZE(vdir, vdir);
}

VECMATH_API void Hull_MakePlaneFace(float *norm, float *pts)
{
	float v0[3], v1[3], v2[3];

	Hull_AdjacentNormals(norm, v0, v1);

	V3F_SCALEADDSCALE(v0, -99999, v1, -99999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(0*3));

	V3F_SCALEADDSCALE(v0,  99999, v1, -99999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(1*3));

	V3F_SCALEADDSCALE(v0,  99999, v1,  99999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(2*3));

	V3F_SCALEADDSCALE(v0, -99999, v1,  99999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(3*3));
}

VECMATH_API int Hull_ClipFace(float *norm,
	float *ipts, float *opts, int num)
{
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)if(V3F_NDOT(ipts+(i*3), norm)>0)break;

	if(i==num)	//nothing to clip
	{
		for(i=0; i<num*3; i++)opts[i]=ipts[i];
		return(num);	//nothing to clip
	}

	//first inside
	j=i;
	while(1)
	{
		if(V3F_NDOT(ipts+(j*3), norm)<=0)break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		V3F_COPY(ipts+(j*3), opts+(l*3));
		l++;

		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*3), norm)>0)break;
		j=k;
	}

	//exit point
	if(V3F_NDOT(ipts+(j*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*3), norm)<=0)break;
		j=k;
	}

	//entry point
	if(V3D_NDOT(ipts+(k*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	return(l);
}

VECMATH_API int Hull_ClipFaceST(float *norm,
	float *ipts, float *ist, float *opts, float *ost, int num)
{
	float f, g;
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)if(V3F_NDOT(ipts+(i*3), norm)>0)break;

	if(i==num)	//nothing to clip
	{
		for(i=0; i<num*3; i++)opts[i]=ipts[i];
		for(i=0; i<num*2; i++)ost[i]=ist[i];
		return(num);	//nothing to clip
	}

	//first inside
	j=i;
	while(1)
	{
		if(V3F_NDOT(ipts+(j*3), norm)<=0)break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		V3F_COPY(ipts+(j*3), opts+(l*3));
		V2F_COPY(ist+(j*2), ost+(l*2));
		l++;

		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*3), norm)>0)break;
		j=k;
	}

	//exit point
	f=V3F_NDOT(ipts+(j*3), norm);
//	g=V3F_NDOT(ipts+(k*3), norm);
//	if((f<0) && (g>0))
	if(f<0)
	{
		Hull_LinePlaneIntersectST(
			ipts+(j*3), ist+(j*2),
			ipts+(k*3), ist+(k*2),
			norm, opts+(l*3), ost+(l*2));
		l++;
	}
#if 0
	else if(g==0)
	{
		V3F_COPY(ipts+(k*3), opts+(l*3));
		V2F_COPY(ist+(k*2), ost+(l*2));
		l++;
		
//		k=(k+1)%num;
	}
#endif

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*3), norm)<=0)break;
		j=k;
	}

	//entry point
	f=V3D_NDOT(ipts+(k*3), norm);
	if(f<0)
	{
		Hull_LinePlaneIntersectST(
			ipts+(j*3), ist+(j*2),
			ipts+(k*3), ist+(k*2),
			norm, opts+(l*3), ost+(l*2));
		l++;
	}
#if 0
	else if(f==0)
	{
		V3F_COPY(ipts+(k*3), opts+(l*3));
		V2F_COPY(ist+(k*2), ost+(l*2));
		l++;
	}
#endif
	return(l);
}

VECMATH_API int Hull_ClipFaceSTNVRGBA(float *norm,
	int xyzsz, int stsz, int nvsz, int clrsz,
	float *ipts, float *ist, float *inv, float *iclr,
	float *opts, float *ost, float *onv, float *oclr,
	int num, int flags)
{
	float f, g;
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)
		if(V3F_NDOT(ipts+(i*xyzsz), norm)>0)
			break;

#if 1
	if((flags&2) && (i==num))	//nothing to clip
		{ return(-2); }
#endif

#if 1
	if(i==num)	//nothing to clip
	{
		for(i=0; i<num; i++)
		{
			Vec4F_Copy(ipts+i*xyzsz, opts+i*xyzsz);
			Vec2F_Copy(ist+i*stsz, ost+i*stsz);
			Vec4F_Copy(inv+i*nvsz, onv+i*nvsz);
			Vec4F_Copy(iclr+i*stsz, oclr+i*clrsz);
//			opts[i*xyzsz+0]=ipts[i*xyzsz+0];
//			opts[i*xyzsz+1]=ipts[i*xyzsz+1];
//			opts[i*xyzsz+2]=ipts[i*xyzsz+2];
		}
		return(num);	//nothing to clip
	}
#endif

	//first inside
	j=i;
	while(1)
	{
		if(V3F_NDOT(ipts+(j*xyzsz), norm)<=0)
			break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
//		V3F_COPY(ipts+(j*3), opts+(l*3));
//		V2F_COPY(ist+(j*2), ost+(l*2));
		if(flags&1)
		{
			Vec4F_Copy(ipts+(j*xyzsz), opts+(l*xyzsz));
			Vec2F_Copy(ist+(j*stsz), ost+(l*stsz));
			Vec4F_Copy(inv+(j*nvsz), onv+(l*nvsz));
			Vec4F_Copy(iclr+(j*clrsz), oclr+(l*clrsz));
		}else
		{
			Vec3F_Copy(ipts+(j*xyzsz), opts+(l*xyzsz));
			Vec2F_Copy(ist+(j*stsz), ost+(l*stsz));
			Vec3F_Copy(inv+(j*nvsz), onv+(l*nvsz));
			Vec4F_Copy(iclr+(j*clrsz), oclr+(l*clrsz));
		}
		l++;

		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*xyzsz), norm)>0)break;
		j=k;
	}

	//exit point
	f=V3F_NDOT(ipts+(j*xyzsz), norm);
	if(f<0)
	{
		Hull_LinePlaneIntersectSTNVRGBA(
			norm,
			ipts+(j*xyzsz), ist+(j*stsz), inv+(j*nvsz), iclr+(j*clrsz),
			ipts+(k*xyzsz), ist+(k*stsz), inv+(k*nvsz), iclr+(k*clrsz),
			opts+(l*xyzsz), ost+(l*stsz), onv+(l*nvsz), oclr+(l*clrsz),
			flags);
		l++;
	}

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(V3F_NDOT(ipts+(k*xyzsz), norm)<=0)break;
//		if(V3F_NDOT(ipts+(k*xyzsz), norm)<0)break;
		j=k;
	}

	//entry point
	f=V3D_NDOT(ipts+(k*xyzsz), norm);
	if(f<0)
	{
		Hull_LinePlaneIntersectSTNVRGBA(
			norm,
			ipts+(j*xyzsz), ist+(j*stsz), inv+(j*nvsz), iclr+(j*clrsz),
			ipts+(k*xyzsz), ist+(k*stsz), inv+(k*nvsz), iclr+(k*clrsz),
			opts+(l*xyzsz), ost+(l*stsz), onv+(l*nvsz), oclr+(l*clrsz),
			flags);
		l++;
	}
	
	if(l<3)
	{
		return(l);
	}
	
	return(l);
}

VECMATH_API void Hull_BoxPlaneExtents(
	float *mins, float *maxs, float *norm,
	float *rm, float *rn)
{
	float d0, d1;
	int i;

	d0=0; d1=0;
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
	*rm=d1-norm[3]; *rn=d0-norm[3];
}

VECMATH_API void Hull_SpherePlaneExtents(
	float *org, float rad, float *norm,
	float *rm, float *rn)
{
	float f;
	
	f=V3F_NDOT(org, norm);
	*rm=f-rad; *rn=f+rad;
}
