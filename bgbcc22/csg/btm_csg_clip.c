int Hull_LinePlaneIntersect(float *start, float *end,
	float *norm, float *point)
{
	float dir[3], x;

	//calc direction
	dir[0]=end[0]-start[0];
	dir[1]=end[1]-start[1];
	dir[2]=end[2]-start[2];

	x=TKRA_Vec3F_DotProduct(dir, norm);
	if(x==0)return(-1);

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;
	dir[1]*=x;
	dir[2]*=x;

	//calc intersection
	x=TKRA_Vec3F_NDotProduct(start, norm);

	x=(x<0)?-x:x;
	point[0]=start[0]+dir[0]*x;
	point[1]=start[1]+dir[1]*x;
	point[2]=start[2]+dir[2]*x;

	return(0);
}

int Hull_LinePlaneIntersectST(float *sv, float *sst,
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


	x=TKRA_Vec3F_DotProduct(dir, norm);
	if(x==0)
	{
		printf("Hull_LinePlaneIntersectST: Parallel Clip\n");
		TKRA_Vec3F_Copy(sv, ov);
		TKRA_Vec2F_Copy(sst, ost);
		return(-1);
	}

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;
	dir[1]*=x;
	dir[2]*=x;

	stdir[0]*=x;
	stdir[1]*=x;

	//calc intersection
	x=TKRA_Vec3F_NDotProduct(sv, norm);

	x=(x<0)?-x:x;
	ov[0]=sv[0]+dir[0]*x;
	ov[1]=sv[1]+dir[1]*x;
	ov[2]=sv[2]+dir[2]*x;

	ost[0]=sst[0]+stdir[0]*x;
	ost[1]=sst[1]+stdir[1]*x;

	return(0);
}


int Hull_LinePlaneIntersectSTNVRGBA(
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


	x=TKRA_Vec3F_DotProduct(dir, norm);
	if(x==0)
	{
		printf("Hull_LinePlaneIntersectST: Parallel Clip\n");

		if(flags&1)
		{
			TKRA_Vec4F_Copy(sv, ov);
			TKRA_Vec2F_Copy(sst, ost);
			TKRA_Vec4F_Copy(snv, onv);
			TKRA_Vec4F_Copy(sclr, oclr);
		}else
		{
			TKRA_Vec3F_Copy(sv, ov);
			TKRA_Vec2F_Copy(sst, ost);
			TKRA_Vec4F_Copy(snv, onv);
			TKRA_Vec4F_Copy(sclr, oclr);
		}
		return(-1);
	}

	x=1.0/((x<0)?-x:x);
	dir[0]*=x;		dir[1]*=x;		dir[2]*=x;		dir[3]*=x;
	stdir[0]*=x;	stdir[1]*=x;
	nvdir[0]*=x;	nvdir[1]*=x;	nvdir[2]*=x;	cldir[3]*=x;
	cldir[0]*=x;	cldir[1]*=x;	cldir[2]*=x;	cldir[3]*=x;

	//calc intersection
	x=TKRA_Vec3F_NDotProduct(sv, norm);

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

void Hull_AdjacentNormals(
	float *norm, float *udir, float *vdir)
{
	float f;

	TKRA_Vec3F_Zero(udir);
	TKRA_Vec3F_Zero(vdir);

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

	f=TKRA_Vec3F_DotProduct(udir, norm);
	TKRA_Vec3F_AddScale(udir, norm, -f, udir);
	f=TKRA_Vec3F_DotProduct(vdir, norm);
	TKRA_Vec3F_AddScale(vdir, norm, -f, vdir);

	TKRA_Vec3F_Normalize(udir, udir);
	TKRA_Vec3F_Normalize(vdir, vdir);
}

void Hull_AdjacentNormals2(
	float *norm, float *udir, float *vdir)
{
	float f;

	TKRA_Vec3F_Zero(udir);
	TKRA_Vec3F_Zero(vdir);

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

	f=TKRA_Vec3F_DotProduct(udir, norm);
	TKRA_Vec3F_AddScale(udir, norm, -f, udir);
	f=TKRA_Vec3F_DotProduct(vdir, norm);
	TKRA_Vec3F_AddScale(vdir, norm, -f, vdir);

	TKRA_Vec3F_Normalize(udir, udir);
	TKRA_Vec3F_Normalize(vdir, vdir);
}

void Hull_AdjacentNormals21(
	float *norm, float *udir, float *vdir)
{
	float f;

	TKRA_Vec3F_Zero(udir);
	TKRA_Vec3F_Zero(vdir);

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

//	f=TKRA_Vec3F_DotProduct(udir, norm);
//	TKRA_Vec3F_AddScale(udir, norm, -f, udir);
//	f=TKRA_Vec3F_DotProduct(vdir, norm);
//	TKRA_Vec3F_AddScale(vdir, norm, -f, vdir);

	TKRA_Vec3F_Normalize(udir, udir);
	TKRA_Vec3F_Normalize(vdir, vdir);
}

void Hull_AdjacentNormals22(
	float *norm, float *udir, float *vdir)
{
	float f, ax, ay, az;

	TKRA_Vec3F_Zero(udir);
	TKRA_Vec3F_Zero(vdir);

	//UxV=N

	ax=fabs(norm[0]); ay=fabs(norm[1]); az=fabs(norm[2]);

	if(ax>=ay && ax>=az) { udir[1]=1; vdir[2]=1; }
		else if(ay>=ax && ay>=az) { udir[0]=1; vdir[2]=1; }
		else { udir[0]=1; vdir[1]=1; }

	TKRA_Vec3F_Normalize(udir, udir);
	TKRA_Vec3F_Normalize(vdir, vdir);
}

void Hull_AdjacentNormals23(
	float *norm, float *udir, float *vdir)
{
	float tn[3];
	float f, ax, ay, az;

	TKRA_Vec3F_Zero(udir);
	TKRA_Vec3F_Zero(vdir);

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
//	if(TKRA_Vec3F_DotProduct(tn, norm)<0)
//		{ V3F_SCALE(udir, -1, udir); }

//	if(((ax*norm[0])+(ay*norm[1])+(az*norm[2]))<0)
//		{ V3F_SCALE(udir, -1, udir); }

	f=TKRA_Vec3F_DotProduct(udir, norm);
	TKRA_Vec3F_AddScale(udir, norm, -f, udir);
	f=TKRA_Vec3F_DotProduct(vdir, norm);
	TKRA_Vec3F_AddScale(vdir, norm, -f, vdir);

	TKRA_Vec3F_Normalize(udir, udir);
	TKRA_Vec3F_Normalize(vdir, vdir);
}

void Hull_MakePlaneFace(float *norm, float *pts)
{
	float v0[3], v1[3], v2[3];

	Hull_AdjacentNormals(norm, v0, v1);

	TKRA_Vec3F_ScaleAddScale(v0, -99999, v1, -99999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(0*3));

	TKRA_Vec3F_ScaleAddScale(v0,  99999, v1, -99999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(1*3));

	TKRA_Vec3F_ScaleAddScale(v0,  99999, v1,  99999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(2*3));

	TKRA_Vec3F_ScaleAddScale(v0, -99999, v1,  99999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(3*3));
}

int Hull_ClipFace(float *norm,
	float *ipts, float *opts, int num)
{
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)if(TKRA_Vec3F_NDotProduct(ipts+(i*3), norm)>0)break;

	if(i==num)	//nothing to clip
	{
		for(i=0; i<num*3; i++)opts[i]=ipts[i];
		return(num);	//nothing to clip
	}

	//first inside
	j=i;
	while(1)
	{
		if(TKRA_Vec3F_NDotProduct(ipts+(j*3), norm)<=0)break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		TKRA_Vec3F_Copy(ipts+(j*3), opts+(l*3));
		l++;

		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)>0)break;
		j=k;
	}

	//exit point
	if(TKRA_Vec3F_NDotProduct(ipts+(j*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)<=0)break;
		j=k;
	}

	//entry point
	if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	return(l);
}

int Hull_ClipFaceST(float *norm,
	float *ipts, float *ist, float *opts, float *ost, int num)
{
	float f, g;
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)if(TKRA_Vec3F_NDotProduct(ipts+(i*3), norm)>0)break;

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
		if(TKRA_Vec3F_NDotProduct(ipts+(j*3), norm)<=0)break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		TKRA_Vec3F_Copy(ipts+(j*3), opts+(l*3));
		TKRA_Vec2F_Copy(ist+(j*2), ost+(l*2));
		l++;

		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)>0)break;
		j=k;
	}

	//exit point
	f=TKRA_Vec3F_NDotProduct(ipts+(j*3), norm);
	if(f<0)
	{
		Hull_LinePlaneIntersectST(
			ipts+(j*3), ist+(j*2),
			ipts+(k*3), ist+(k*2),
			norm, opts+(l*3), ost+(l*2));
		l++;
	}

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)<=0)break;
		j=k;
	}

	//entry point
	f=TKRA_Vec3F_NDotProduct(ipts+(k*3), norm);
	if(f<0)
	{
		Hull_LinePlaneIntersectST(
			ipts+(j*3), ist+(j*2),
			ipts+(k*3), ist+(k*2),
			norm, opts+(l*3), ost+(l*2));
		l++;
	}
	return(l);
}

int Hull_ClipFaceSTNVRGBA(float *norm,
	int xyzsz, int stsz, int nvsz, int clrsz,
	float *ipts, float *ist, float *inv, float *iclr,
	float *opts, float *ost, float *onv, float *oclr,
	int num, int flags)
{
	float f, g;
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)
		if(TKRA_Vec3F_NDotProduct(ipts+(i*xyzsz), norm)>0)
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
			TKRA_Vec4F_Copy(ipts+i*xyzsz, opts+i*xyzsz);
			TKRA_Vec2F_Copy(ist+i*stsz, ost+i*stsz);
			TKRA_Vec4F_Copy(inv+i*nvsz, onv+i*nvsz);
			TKRA_Vec4F_Copy(iclr+i*stsz, oclr+i*clrsz);
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
		if(TKRA_Vec3F_NDotProduct(ipts+(j*xyzsz), norm)<=0)
			break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
//		TKRA_Vec3F_Copy(ipts+(j*3), opts+(l*3));
//		TKRA_Vec2F_Copy(ist+(j*2), ost+(l*2));
		if(flags&1)
		{
			TKRA_Vec4F_Copy(ipts+(j*xyzsz), opts+(l*xyzsz));
			TKRA_Vec2F_Copy(ist+(j*stsz), ost+(l*stsz));
			TKRA_Vec4F_Copy(inv+(j*nvsz), onv+(l*nvsz));
			TKRA_Vec4F_Copy(iclr+(j*clrsz), oclr+(l*clrsz));
		}else
		{
			TKRA_Vec3F_Copy(ipts+(j*xyzsz), opts+(l*xyzsz));
			TKRA_Vec2F_Copy(ist+(j*stsz), ost+(l*stsz));
			TKRA_Vec3F_Copy(inv+(j*nvsz), onv+(l*nvsz));
			TKRA_Vec4F_Copy(iclr+(j*clrsz), oclr+(l*clrsz));
		}
		l++;

		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*xyzsz), norm)>0)break;
		j=k;
	}

	//exit point
	f=TKRA_Vec3F_NDotProduct(ipts+(j*xyzsz), norm);
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
		if(TKRA_Vec3F_NDotProduct(ipts+(k*xyzsz), norm)<=0)break;
//		if(TKRA_Vec3F_NDotProduct(ipts+(k*xyzsz), norm)<0)break;
		j=k;
	}

	//entry point
	f=TKRA_Vec3F_NDotProduct(ipts+(k*xyzsz), norm);
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

void Hull_BoxPlaneExtents(
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

void Hull_SpherePlaneExtents(
	float *org, float rad, float *norm,
	float *rm, float *rn)
{
	float f;
	
	f=TKRA_Vec3F_NDotProduct(org, norm);
	*rm=f-rad; *rn=f+rad;
}

void Hull_PolyPlaneExtents(
	float *pts, int npts, float *norm,
	float *rm, float *rn)
{
	float f, m, n;
	int i;
	
	m= 999999999;
	n=-999999999;
	for(i=0; i<npts; i++)
	{
		f=TKRA_Vec3F_NDotProduct(pts+i*3, norm);
		if(f<m)		m=f;
		if(f>n)		n=f;
	}
	*rm=m; *rn=n;
}

void Hull_CalcPolyBBox(
	float *pts, int npts,
	float *rmins, float *rmaxs)
{
	float f, m, n;
	int i;
	
	TKRA_Vec3F_Set(rmins,  999999999.0,  999999999.0,  999999999.0);
	TKRA_Vec3F_Set(rmaxs, -999999999.0, -999999999.0, -999999999.0);
	for(i=0; i<npts; i++)
	{
		TKRA_Vec3F_Min(rmins, pts+i*3, rmins);
		TKRA_Vec3F_Max(rmaxs, pts+i*3, rmaxs);
	}
}

void HullF_MakePlaneFace(float *norm, float *pts)
{
	float v0[3], v1[3], v2[3];

	Hull_AdjacentNormals2(norm, v0, v1);

	TKRA_Vec3F_ScaleAddScale(v0, -999999, v1, -999999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(0*3));
	TKRA_Vec3F_ScaleAddScale(v0,  999999, v1, -999999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(1*3));
	TKRA_Vec3F_ScaleAddScale(v0,  999999, v1,  999999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(2*3));
	TKRA_Vec3F_ScaleAddScale(v0, -999999, v1,  999999, v2);
	TKRA_Vec3F_AddScale(v2, norm, norm[3], pts+(3*3));
}

int HullF_ClipFace(float *norm,
	float *ipts, float *opts, int num)
{
	int i, j, k, l;

	//first outside
	for(i=0; i<num; i++)if(TKRA_Vec3F_NDotProduct(ipts+(i*3), norm)>0)break;

	if(i==num)	//nothing to clip
	{
		for(i=0; i<num*3; i++)opts[i]=ipts[i];
		return(num);	//nothing to clip
	}

	//first inside
	j=i;
	while(1)
	{
		if(TKRA_Vec3F_NDotProduct(ipts+(j*3), norm)<=0)break;
		j=(j+1)%num;
		if(j==i)return(0);	//everything clipped
	}

	//copy inside
	i=j;
	l=0;
	while(1)
	{
		TKRA_Vec3F_Copy(ipts+(j*3), opts+(l*3));
		l++;

		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)>0)break;
		j=k;
	}

	//exit point
	if(TKRA_Vec3F_NDotProduct(ipts+(j*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	j=k;
	while(1)
	{
		k=(j+1)%num;
		if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)<=0)break;
		j=k;
	}

	//entry point
	if(TKRA_Vec3F_NDotProduct(ipts+(k*3), norm)<0)
	{
		Hull_LinePlaneIntersect(
			ipts+(j*3), ipts+(k*3), norm, opts+(l*3));
		l++;
	}

	return(l);
}

int HullF_BoxHull(float *norm, int num,
	float *mins, float *maxs)
{
	static float pts0[16*3];
	static float pts1[16*3];

	float tv0[3], tv1[3], tv2[3];
	float sx, sy, f;
	int i, j, k, l, t;

	mins[0]=999999; mins[1]=999999; mins[2]=999999;
	maxs[0]=-999999; maxs[1]=-999999; maxs[2]=-999999;

	for(i=0; i<num; i++)
	{
		HullF_MakePlaneFace(norm+i*4, pts0);
		l=4;

		for(j=0; j<num; j++)
		{
			if(i==j)continue;

			l=HullF_ClipFace(norm+j*4, pts0, pts1, l);
			for(k=0; k<(l*3); k++)pts0[k]=pts1[k];
		}
		if(l<=0)return(-1);

		for(j=0; j<l; j++)
		{
			TKRA_Vec3F_Min(mins, pts0+j*3, mins);
			TKRA_Vec3F_Max(maxs, pts0+j*3, maxs);
		}
	}

	return(0);
}

int HullF_TransformPoint(float *iv, float *ov, float *trans)
{
	float tv[3];

	tv[0]=
		(iv[0]*trans[0])+
		(iv[1]*trans[4])+
		(iv[2]*trans[8])+
		trans[12];
	tv[1]=
		(iv[0]*trans[1])+
		(iv[1]*trans[5])+
		(iv[2]*trans[9])+
		trans[13];
	tv[2]=
		(iv[0]*trans[2])+
		(iv[1]*trans[6])+
		(iv[2]*trans[10])+
		trans[14];
	ov[0]=tv[0];
	ov[1]=tv[1];
	ov[2]=tv[2];
	return(0);
}

int HullF_TransformNormal(float *iv, float *ov, float *trans)
{
	float tv[3];

	tv[0]=
		(iv[0]*trans[0])+
		(iv[1]*trans[4])+
		(iv[2]*trans[8]);
	tv[1]=
		(iv[0]*trans[1])+
		(iv[1]*trans[5])+
		(iv[2]*trans[9]);
	tv[2]=
		(iv[0]*trans[2])+
		(iv[1]*trans[6])+
		(iv[2]*trans[10]);
	ov[0]=tv[0];
	ov[1]=tv[1];
	ov[2]=tv[2];
	return(0);
}


int HullF_TransformPlane(float *inorm, float *onorm, float *trans)
{
	float txy0[3], txy1[3], tnv0[4], tnv1[4];
	
	TKRA_Vec3F_Scale(inorm, inorm[3], txy0);
	HullF_TransformPoint(txy0, txy1, trans);

	HullF_TransformNormal(inorm, tnv0, trans);
	TKRA_Vec3F_Normalize(tnv0, tnv1);
	tnv1[3]=TKRA_Vec3F_DotProduct(txy1, tnv1);
	TKRA_Vec4F_Copy(tnv1, onorm);
	return(0);
}

int HullF_Mat4F_IdentityP(float *mat)
{
	static byte imat[16]={
		1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	int i;

	for(i=0; i<16; i++)
		if(mat[i]!=imat[i])
			return(0);
	return(1);
}