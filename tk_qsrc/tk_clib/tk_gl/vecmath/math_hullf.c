#include <tkgl/bgbrasw.h>

void HullF_MakePlaneFace(float *norm, float *pts)
{
	float v0[3], v1[3], v2[3];

	PlaneF_AdjacentNormals2(norm, v0, v1);

	V3F_SCALEADDSCALE(v0, -999999, v1, -999999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(0*3));
	V3F_SCALEADDSCALE(v0,  999999, v1, -999999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(1*3));
	V3F_SCALEADDSCALE(v0,  999999, v1,  999999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(2*3));
	V3F_SCALEADDSCALE(v0, -999999, v1,  999999, v2);
	V3F_ADDSCALE(v2, norm, norm[3], pts+(3*3));
}

int HullF_ClipFace(float *norm,
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
		Vec3F_LinePlaneIntersect(
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
		Vec3F_LinePlaneIntersect(
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

//	V3F_INITBOX(mins, maxs);
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
			V3F_MIN(mins, pts0+j*3, mins);
			V3F_MAX(maxs, pts0+j*3, maxs);
		}
	}

	return(0);
}


#if 0
// Solid_Hull *Hull_BuildHull(
	float *norm, int num)
{
	static float pts0[16*3];
	static float pts1[16*3];

	Solid_Hull *tmp;
	Hull_Face *fst, *lst, *ftmp;

	float tv0[3], tv1[3], tv2[3];
	float mins[3], maxs[3];
	float sx, sy, f;
	int i, j, k, l, t;

	V3F_INITBOX(mins, maxs);

	fst=NULL;
	lst=NULL;
	for(i=0; i<num; i++)
	{
		ftmp=malloc(sizeof(Hull_Face));
		V4F_COPY(norm+i*4, ftmp->norm);
		Hull_MakePlaneFace(norm+i*4, pts0);
		l=4;

		for(j=0; j<num; j++)
		{
			if(i==j)continue;

			l=Hull_ClipFace(norm+j*4, pts0, pts1, l);
			for(k=0; k<(l*3); k++)pts0[k]=pts1[k];
		}

		ftmp->nvecs=l;
		ftmp->vecs=malloc(3*l*sizeof(float));
		ftmp->evecs=malloc(4*l*sizeof(float));

		for(j=0; j<l; j++)
		{
			V3F_COPY(pts0+j*3, ftmp->vecs+j*3);
			k=(j+1)%l;

			V3F_SUB(pts0+k*3, pts0+j*3, tv0);
			V3F_CROSS(tv0, norm+i*4, tv1);
//			V3F_CROSS(norm+i*4, tv0, tv1);

			V3F_NORMALIZE(tv1, ftmp->evecs+j*4);
			ftmp->evecs[j*4+3]=V3F_DOT(pts0+j*3, ftmp->evecs+j*4);

			V3F_MIN(mins, pts0+j*3, mins);
			V3F_MAX(maxs, pts0+j*3, maxs);
		}

		if(fst)
		{
			lst->next=ftmp;
			lst=ftmp;
		}else
		{
			fst=ftmp;
			lst=ftmp;
		}
	}

	tmp=malloc(sizeof(Solid_Hull));
	memset(tmp, 0, sizeof(Solid_Hull));
	tmp->face=fst;

	tmp->rot[0]=1;
	tmp->rot[4]=1;
	tmp->rot[8]=1;

	V3F_COPY(mins, tmp->mins);
	V3F_COPY(maxs, tmp->maxs);

	tv0[0]=(maxs[0]-mins[0])/64;
	tv0[1]=(maxs[1]-mins[1])/64;
	tv0[2]=(maxs[2]-mins[2])/64;


	f=0; t=0;
	for(i=0; i<64; i++)
		for(j=0; j<64; j++)
			for(k=0; k<64; k++)
	{
		tv1[0]=mins[0]+tv0[0]*i;
		tv1[1]=mins[1]+tv0[1]*j;
		tv1[2]=mins[2]+tv0[2]*k;

		for(l=0; l<num; l++)
			if(V3F_NDOT(tv1, norm+l*4)>0)
				break;
		if(l<num)continue;

		f+=fabs(tv1[0]*tv1[1]*tv1[2]);
		t++;
	}

	tmp->mass=1;
	tmp->moment=f/t;

	printf("moment %f %f/%d\n", tmp->moment, f, t);


	return(tmp);
}

#endif

