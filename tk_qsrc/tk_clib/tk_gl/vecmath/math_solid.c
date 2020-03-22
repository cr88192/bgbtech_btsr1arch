#include <tkgl/bgbrasw.h>

void SolidAABB_BoxMove(float *bmins, float *bmaxs,
	float *start, float *end, float *mins, float *maxs)
{
	float mins2[3], maxs2[3];
	int i;

	V3F_ADD(bmins, start, mins);
	V3F_ADD(bmaxs, start, maxs);
	V3F_ADD(bmins, end, mins2);
	V3F_ADD(bmaxs, end, maxs2);

	for(i=0; i<3; i++)
	{
		if(mins2[i]<mins[i])mins[i]=mins2[i];
		if(maxs2[i]>maxs[i])maxs[i]=maxs2[i];
	}
}

int SolidAABB_BoxCollideP(float *ma, float *na, float *mb, float *nb)
{
	if(na[0]<=mb[0])return(0);
	if(na[1]<=mb[1])return(0);
	if(na[2]<=mb[2])return(0);
	if(nb[0]<=ma[0])return(0);
	if(nb[1]<=ma[1])return(0);
	if(nb[2]<=ma[2])return(0);

	return(1);
}

int SolidAABB_BoxCollideAxis(float *ma, float *na, float *mb, float *nb)
{
	int i;

	i=0;
	if((na[0]>mb[0]) && !(ma[0]>=nb[0]))i|=1;
	if((na[1]>mb[1]) && !(ma[1]>=nb[1]))i|=2;
	if((na[2]>mb[2]) && !(ma[2]>=nb[2]))i|=4;

	if((nb[0]>ma[0]) && !(mb[0]>=na[0]))i|=8;
	if((nb[1]>ma[1]) && !(mb[1]>=na[1]))i|=16;
	if((nb[2]>ma[2]) && !(mb[2]>=na[2]))i|=32;

	return(i);
}

int SolidAABB_PointInBoxP(float *p, float *min, float *max)
{
	if(p[0]<=min[0])return(0);
	if(p[0]>=max[0])return(0);
	if(p[1]<=min[1])return(0);
	if(p[1]>=max[1])return(0);
	if(p[2]<=min[2])return(0);
	if(p[2]>=max[2])return(0);

	return(1);
}

void SolidAABB_BoxIntersection(float *ma, float *na,
	float *mb, float *nb, float *mc, float *nc)
{
	mc[0]=(ma[0]>mb[0])?ma[0]:mb[0];
	nc[0]=(na[0]<nb[0])?na[0]:nb[0];
	mc[1]=(ma[1]>mb[1])?ma[1]:mb[1];
	nc[1]=(na[1]<nb[1])?na[1]:nb[1];
	mc[2]=(ma[2]>mb[2])?ma[2]:mb[2];
	nc[2]=(na[2]<nb[2])?na[2]:nb[2];
}

int SolidAABB_BoxLineCollideP(float *min, float *max, float *sp, float *ep)
{
	float n[4];

	//eliminate if non-overlap
	if((sp[0]<min[0]) && (ep[0]<min[0]))return(0);
	if((sp[1]<min[1]) && (ep[1]<min[1]))return(0);
	if((sp[2]<min[2]) && (ep[2]<min[2]))return(0);
	if((sp[0]>max[0]) && (ep[0]>max[0]))return(0);
	if((sp[1]>max[1]) && (ep[1]>max[1]))return(0);
	if((sp[2]>max[2]) && (ep[2]>max[2]))return(0);

	n[0]=1; n[1]=0; n[2]=0; n[3]=min[0];
	if(sp[0]<min[0])PlaneF_LinePlaneIntersect(sp, ep, n, sp);
	if(ep[0]<min[0])PlaneF_LinePlaneIntersect(sp, ep, n, ep);

	n[0]=1; n[1]=0; n[2]=0; n[3]=max[0];
	if(sp[0]>max[0])PlaneF_LinePlaneIntersect(sp, ep, n, sp);
	if(ep[0]>max[0])PlaneF_LinePlaneIntersect(sp, ep, n, ep);

	if((sp[1]<min[1]) && (ep[1]<min[1]))return(0);
	if((sp[2]<min[2]) && (ep[2]<min[2]))return(0);
	if((sp[1]>max[1]) && (ep[1]>max[1]))return(0);
	if((sp[2]>max[2]) && (ep[2]>max[2]))return(0);

	n[0]=0; n[1]=1; n[2]=0; n[3]=min[1];
	if(sp[1]<min[1])PlaneF_LinePlaneIntersect(sp, ep, n, sp);
	if(ep[1]<min[1])PlaneF_LinePlaneIntersect(sp, ep, n, ep);

	n[0]=0; n[1]=1; n[2]=0; n[3]=max[1];
	if(sp[1]>max[1])PlaneF_LinePlaneIntersect(sp, ep, n, sp);
	if(ep[1]>max[1])PlaneF_LinePlaneIntersect(sp, ep, n, ep);

	if((sp[2]<min[2]) && (ep[2]<min[2]))return(0);
	if((sp[2]>max[2]) && (ep[2]>max[2]))return(0);

	return(1);
}



int SolidOBB_CheckObbPlane(float *mins, float *maxs,
	float *xform, float *norm, float *io)
{
	float w[3];

	float xwa, ywa, zwa;
	float m, n;
	int i;

	w[0]=V3F_DOT(xform+(0*4), norm);
	w[1]=V3F_DOT(xform+(1*4), norm);
	w[2]=V3F_DOT(xform+(2*4), norm);

	xwa=xform[3*4+0]*norm[0];
	ywa=xform[3*4+1]*norm[1];
	zwa=xform[3*4+2]*norm[2];

	m=0;
	n=0;
	for(i=0; i<3; i++)
	{
		if(w[i]>0)
		{
			m+=mins[i]*w[i];
			n+=maxs[i]*w[i];
		}else
		{
			m+=maxs[i]*w[i];
			n+=mins[i]*w[i];
		}
	}
	m+=xwa+ywa+zwa-norm[3];
	n+=xwa+ywa+zwa-norm[3];

	i=0;
	if(m<0)i|=2;
	if(n>=0)i|=1;

	if(!i)
	{
//		printf("SolidOBB_CheckObbPlane: fail\n");

//		printf("[(%f %f %f) (%f %f %f)]\n",
//			mins[0], mins[1], mins[2],
//			maxs[0], maxs[1], maxs[2]);

//		printf("(%f %f %f %f)\n",
//			norm[0], norm[1], norm[2], norm[3]);

//		printf("%f %f\n", m, n);
	}

	if(io && (i==3))
		SolidOBB_ObbPlaneIntersect(
			mins, maxs, xform, norm, io);

	return(i);
}

void SolidOBB_GenPoints(float *mins, float *maxs, float *pos, float *pts)
{
	float v[3];
	int i;

	for(i=0; i<8; i++)
	{
		v[0]=(i&1)?maxs[0]:mins[0];
		v[1]=(i&2)?maxs[1]:mins[1];
		v[2]=(i&4)?maxs[2]:mins[2];
		PlaneF_TransformPoint(v, pos, pts+i*3);
	}
}

int SolidOBB_ObbPlaneIntersect(float *mins, float *maxs,
	float *xform, float *norm, float *io)
{
	float pts[8*3];
	float bo[3];
	float d;
	int i, j;

	SolidOBB_GenPoints(mins, maxs, xform, pts);

	V3F_ZERO(bo);
	j=0;

	for(i=0; i<8; i++)
	{
		d=V3F_NDOT(pts+i*3, norm);
		if(d<0)
		{
			V3F_ADD(bo, pts+i*3, bo);
			j++;
		}
	}

	if(!j)j++;
	V3F_SCALE(bo, 1.0/j, io);

	return(0);
}


int SolidOBB_ObbPlaneExtents(float *mins, float *maxs,
	float *xform, float *norm, float *ext)
{
	float w[3];

	float xwa, ywa, zwa;
	float m, n;
	int i;

	w[0]=V3F_DOT(xform+(0*4), norm);
	w[1]=V3F_DOT(xform+(1*4), norm);
	w[2]=V3F_DOT(xform+(2*4), norm);

	xwa=xform[3*4+0]*norm[0];
	ywa=xform[3*4+1]*norm[1];
	zwa=xform[3*4+2]*norm[2];

	m=0;
	n=0;

	for(i=0; i<3; i++)
	{
		if(w[i]>0)
		{
			m+=mins[i]*w[i];
			n+=maxs[i]*w[i];
		}else
		{
			m+=maxs[i]*w[i];
			n+=mins[i]*w[i];
		}
	}
	m+=xwa+ywa+zwa;
	n+=xwa+ywa+zwa;

	ext[0]=m;
	ext[1]=n;

	return(0);
}

void SolidOBB_PlaneExtents(float *mins, float *maxs,
	float *xform, float *norm, float *min, float *max)
{
	float w[3];
	float m, n, d;
	int i;

	w[0]=V3F_DOT(xform+(0*4), norm);
	w[1]=V3F_DOT(xform+(1*4), norm);
	w[2]=V3F_DOT(xform+(2*4), norm);
	d=V3F_DOT(xform+3*4, norm);

	m=0;
	n=0;
	for(i=0; i<3; i++)
	{
		if(w[i]>0)
		{
			m+=mins[i]*w[i];
			n+=maxs[i]*w[i];
		}else
		{
			m+=maxs[i]*w[i];
			n+=mins[i]*w[i];
		}
	}
	*min=m+d;
	*max=n+d;
}

int SolidOBB_ObbPlaneMoveExtents(float *mins, float *maxs,
	float *sxform, float *exform, float *norm, float *ext)
{
	float ext2[4];

	SolidOBB_ObbPlaneExtents(mins, maxs, sxform, norm, ext2+0);
	SolidOBB_ObbPlaneExtents(mins, maxs, exform, norm, ext2+2);

	ext[0]=(ext2[0]<ext2[2])?ext2[0]:ext2[2];
	ext[1]=(ext2[1]>ext2[3])?ext2[1]:ext2[3];

	return(0);
}

int SolidOBB_CheckObbMovePlane(float *mins, float *maxs,
	float *sxform, float *exform, float *norm, float *io)
{
	float ext[2];
	int i;

	SolidOBB_ObbPlaneMoveExtents(mins, maxs, sxform, exform,
		norm, ext);

	i=0;
	if((ext[0]-norm[3])<0)i|=2;
	if((ext[1]-norm[3])>=0)i|=1;

	if(io && (i==3))
		SolidOBB_ObbPlaneIntersect(
			mins, maxs, exform, norm, io);

	return(i);
}

int SolidOBB_CheckCollide(
	float *amins, float *amaxs, float *apos,
	float *bmins, float *bmaxs, float *bpos)
{
	int i, j;
	float ext[8], dir[3];
	float omins[3], omaxs[3];

	for(i=0; i<3; i++)
	{
		SolidOBB_ObbPlaneExtents(amins, amaxs, apos,
			apos+i*4, ext+0);
		SolidOBB_ObbPlaneExtents(bmins, bmaxs, bpos,
			apos+i*4, ext+2);
		if((ext[0]>=ext[3]) || (ext[2]>=ext[1]))
			return(0);
	}

	for(i=0; i<3; i++)
	{
		SolidOBB_ObbPlaneExtents(amins, amaxs, apos,
			bpos+i*4, ext+0);
		SolidOBB_ObbPlaneExtents(bmins, bmaxs, bpos,
			bpos+i*4, ext+2);
		if((ext[0]>=ext[3]) || (ext[2]>=ext[1]))
			return(0);

		omins[i]=(ext[0]>ext[2])?ext[0]:ext[2];	//greater min
		omaxs[i]=(ext[1]<ext[3])?ext[1]:ext[3];	//lesser max
	}

	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
	{
		V3F_CROSS(apos+i*4, bpos+j*4, dir);

		SolidOBB_ObbPlaneExtents(amins, amaxs, apos,
			bpos+i*4, ext+0);
		SolidOBB_ObbPlaneExtents(bmins, bmaxs, bpos,
			bpos+i*4, ext+2);
		if((ext[0]>=ext[3]) || (ext[2]>=ext[1]))
			return(0);
	}

	return(1);
}

int SolidOBB_CheckObbMove(
	float *amins, float *amaxs, float *aspos, float *aepos,
	float *bmins, float *bmaxs, float *bspos, float *bepos,
	float *org)
{
	int i;
	float ext[8];
	float omins[3], omaxs[3], iorg[3];

	for(i=0; i<3; i++)
	{
		SolidOBB_ObbPlaneExtents(amins, amaxs, aepos,
			aepos+i*4, ext+0);
		SolidOBB_ObbPlaneExtents(bmins, bmaxs, bepos,
			aepos+i*4, ext+2);
		if((ext[0]>ext[3]) || (ext[2]>ext[1]))
			return(0);
	}

	for(i=0; i<3; i++)
	{
		SolidOBB_ObbPlaneExtents(amins, amaxs, aepos,
			bepos+i*4, ext+0);
		SolidOBB_ObbPlaneExtents(bmins, bmaxs, bepos,
			bepos+i*4, ext+2);
		if((ext[0]>ext[3]) || (ext[2]>ext[1]))
			return(0);

		omins[i]=(ext[0]>ext[2])?ext[0]:ext[2];	//greater min
		omaxs[i]=(ext[1]<ext[3])?ext[1]:ext[3];	//lesser max
	}


	for(i=0; i<3; i++)
		iorg[i]=(omins[i]+omaxs[i])*0.5;
	for(i=0; i<3; i++)
	{
		org[i]=iorg[0]*bepos[0*4+i]+
			iorg[1]*bepos[1*4+i]+
			iorg[2]*bepos[2*4+i]+
			bepos[3*4+i];
	}

	return(1);
}

int SolidOBB_ObbImpactNormal(
	float *amins, float *amaxs, float *aspos, float *aepos,
	float *bmins, float *bmaxs, float *bspos, float *bepos,
	float *vel, float *norm)
{
	float rv[3], rn[3];
	float ext[2];
	int i;

	rv[0]=V3F_DOT(vel, bepos+0*4);
	rv[1]=V3F_DOT(vel, bepos+1*4);
	rv[2]=V3F_DOT(vel, bepos+2*4);

	if(fabs(rv[0])>fabs(rv[1]))
	{
		if(fabs(rv[0])>fabs(rv[2]))
		{
			rn[0]=(rv[0]>0)?1:-1;
			rn[1]=0;
			rn[2]=0;
		}else
		{
			rn[0]=0;
			rn[1]=0;
			rn[2]=(rv[2]>0)?1:-1;
		}
	}else
	{
		if(fabs(rv[1])>fabs(rv[2]))
		{
			rn[0]=0;
			rn[1]=(rv[1]>0)?1:-1;
			rn[2]=0;
		}else
		{
			rn[0]=0;
			rn[1]=0;
			rn[2]=(rv[2]>0)?1:-1;
		}
	}

	for(i=0; i<3; i++)
	{
		norm[i]=rn[0]*bepos[0*4+i]+
			rn[1]*bepos[1*4+i]+
			rn[2]*bepos[2*4+i];
	}

	SolidOBB_ObbPlaneExtents(bmins, bmaxs, bepos,
		norm, ext);
	norm[3]=ext[0];	//near extent

	return(0);
}

void SolidOBB_Box(float *mins, float *maxs, float *pos,
	float *bmins, float *bmaxs)
{
	float norm[3];
	float ext[2];
	int i;

	for(i=0; i<3; i++)
	{
		V3F_ZERO(norm);
		norm[i]=1;

		SolidOBB_ObbPlaneExtents(mins, maxs, pos,
			norm, ext);
		bmins[i]=ext[0];
		bmaxs[i]=ext[1];
	}
}

void SolidOBB_BoxMove(
	float *mins, float *maxs, float *spos, float *epos,
	float *bmins, float *bmaxs)
{
	float norm[3];
	float ext[2];
	int i;

	for(i=0; i<3; i++)
	{
		V3F_ZERO(norm);
		norm[i]=1;

		SolidOBB_ObbPlaneExtents(mins, maxs, spos,
			norm, ext);
		bmins[i]=ext[0];
		bmaxs[i]=ext[1];

		SolidOBB_ObbPlaneExtents(mins, maxs, epos,
			norm, ext);
		if(ext[0]<bmins[i])bmins[i]=ext[0];
		if(ext[1]>bmaxs[i])bmaxs[i]=ext[1];
	}
}

int SolidOBB_ObbPointCollide(
	float *mins, float *maxs, float *pos, float *org)
{
	float d;
	int i;

	for(i=0; i<3; i++)
	{
		d=V3F_DOT(org, pos+(i*4))-pos[3*4+i];
		if(d<mins[i])return(0);
		if(d>maxs[i])return(0);
	}
	return(1);
}

int SolidOBB_BoxLineCollideP(float *min, float *max, float *pos,
	float *sp, float *ep)
{
	float vsp[4], vep[4];
	int i;

	for(i=0; i<3; i++)
	{
		vsp[i]=V3F_DOT(sp, pos+(i*4))-pos[3*4+i];
		vep[i]=V3F_DOT(ep, pos+(i*4))-pos[3*4+i];
	}

	i=SolidAABB_BoxLineCollideP(min, max, vsp, vep);
	return(i);
}
