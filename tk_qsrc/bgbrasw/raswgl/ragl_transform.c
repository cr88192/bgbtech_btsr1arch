#include <bgbrasw.h>

void RASWGL_CheckExpandTransform0Vertices(
	RASWGL_Context *ctx, int nxyz)
{
	int i;

	if(!ctx->trans0_xyzbuf)
	{
		i=256;
		while(nxyz>=i)i=i+(i>>1);
		ctx->trans0_xyzbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans0_stbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans0_nvbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans0_clrbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans0_mvec=i;
	}

	if(nxyz>=ctx->trans0_mvec)
	{
		i=ctx->trans0_mvec;
		while(nxyz>=i)i=i+(i>>1);
		ctx->trans0_xyzbuf=
			bgbrasw_realloc(ctx->trans0_xyzbuf, i*4*sizeof(float));
		ctx->trans0_stbuf=
			bgbrasw_realloc(ctx->trans0_stbuf, i*4*sizeof(float));
		ctx->trans0_nvbuf=
			bgbrasw_realloc(ctx->trans0_nvbuf, i*4*sizeof(float));
		ctx->trans0_clrbuf=
			bgbrasw_realloc(ctx->trans0_clrbuf, i*4*sizeof(float));
		ctx->trans0_mvec=i;
	}
}

void RASWGL_CheckExpandTransform1Vertices(
	RASWGL_Context *ctx, int nxyz)
{
	int i;

	if(!ctx->trans1_xyzbuf)
	{
		i=256;
		while(nxyz>=i)i=i+(i>>1);
		ctx->trans1_xyzbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans1_stbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans1_nvbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans1_clrbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->trans1_mvec=i;
	}

	if(nxyz>=ctx->trans1_mvec)
	{
		i=ctx->trans1_mvec;
		while(nxyz>=i)i=i+(i>>1);
		ctx->trans1_xyzbuf=
			bgbrasw_realloc(ctx->trans1_xyzbuf, i*4*sizeof(float));
		ctx->trans1_stbuf=
			bgbrasw_realloc(ctx->trans1_stbuf, i*4*sizeof(float));
		ctx->trans1_nvbuf=
			bgbrasw_realloc(ctx->trans1_nvbuf, i*4*sizeof(float));
		ctx->trans1_clrbuf=
			bgbrasw_realloc(ctx->trans1_clrbuf, i*4*sizeof(float));
		ctx->trans1_mvec=i;
	}
}

void RASWGL_CheckExpandTransformNVertices(
	RASWGL_Context *ctx, int nxyz)
{
	int i;

	if(!ctx->transN_xyzbuf)
	{
		i=256;
		while(nxyz>=i)i=i+(i>>1);
		ctx->transN_xyzbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->transN_stbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->transN_nvbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->transN_clrbuf=
			bgbrasw_malloc(i*4*sizeof(float));
		ctx->transN_mvec=i;
	}

	if(nxyz>=ctx->transN_mvec)
	{
		i=ctx->transN_mvec;
		while(nxyz>=i)i=i+(i>>1);
		ctx->transN_xyzbuf=
			bgbrasw_realloc(ctx->transN_xyzbuf, i*4*sizeof(float));
		ctx->transN_stbuf=
			bgbrasw_realloc(ctx->transN_stbuf, i*4*sizeof(float));
		ctx->transN_nvbuf=
			bgbrasw_realloc(ctx->transN_nvbuf, i*4*sizeof(float));
		ctx->transN_clrbuf=
			bgbrasw_realloc(ctx->transN_clrbuf, i*4*sizeof(float));
		ctx->transN_mvec=i;
	}
}

void RASWGL_TransformVerticesModelviewXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv1[4], tv2[4];
	int i, j;
	
//	if(ixyz==oxyz)
	if(1)
	{
		for(i=0; i<nxyz; i++)
		{
			Vec3F_Copy(ixyz+i*ixyzsz, tv1);
			tv1[3]=1;
//			Mat4F_TransformVector4T(tv1,
//				ctx->modelview_matrix, tv2);

//			Mat4F_TransformPointT(tv1,
//				ctx->modelview_matrix, tv2);
			Mat4F_TransformVector4(tv1,
				ctx->modelview_matrix, tv2);
//			Mat4F_TransformPoint34G(tv1,
//				ctx->modelview_matrix, tv2);

//			tv2[3]=1;

			Vec4F_Copy(tv2, oxyz+i*oxyzsz);

//			Mat4F_TransformVector4T(ixyz+i*ixyzsz,
//				ctx->modelview_matrix, tv);
//			Mat4F_TransformPointT(ixyz+i*ixyzsz,
//				ctx->modelview_matrix, tv);
//			Vec4F_Copy(tv, oxyz+i*oxyzsz);
		}
	}else
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformPointT(ixyz+i*ixyzsz,
				ctx->modelview_matrix, oxyz+i*oxyzsz);
		}
	}
}

void RASWGL_TransformVerticesProjectionXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv1[4], tv2[4];
	int i, j;
	
//	if(ixyz==oxyz)
	if(1)
	{
		for(i=0; i<nxyz; i++)
		{
			Vec4F_Copy(ixyz+i*ixyzsz, tv1);

			Mat4F_TransformVector4(tv1,
				ctx->projection_matrix, tv2);
//			Mat4F_TransformVector4T(tv1,
//				ctx->projection_matrix, tv2);
//			Mat4F_TransformPoint34G(tv1,
//				ctx->projection_matrix, tv2);
			
			if(tv2[3]>=1)
//			if(tv2[3]>0.1)
//			if(tv2[3]>0)
//			if(tv2[3]!=0)
			{
				tv2[0]=tv2[0]/tv2[3];
				tv2[1]=tv2[1]/tv2[3];
				tv2[2]=tv2[2]/tv2[3];
			}else
//				if(tv2[3]<-(0.1))
//				if(tv2[3]<0)
				if(tv2[3]<=(-1))
			{
				tv2[0]=tv2[0]/(-tv2[3]);
				tv2[1]=tv2[1]/(-tv2[3]);
				tv2[2]=tv2[2]/(-tv2[3]);
			}
			
			Vec4F_Copy(tv2, oxyz+i*oxyzsz);

//			Mat4F_TransformVector4T(ixyz+i*ixyzsz,
//				ctx->projection_matrix, tv);
//			Mat4F_TransformPointT(ixyz+i*ixyzsz,
//				ctx->projection_matrix, tv);
//			Vec4F_Copy(tv, oxyz+i*oxyzsz);
		}
	}else
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformVector4T(ixyz+i*ixyzsz,
				ctx->projection_matrix, oxyz+i*oxyzsz);
//			Mat4F_TransformPointT(ixyz+i*ixyzsz,
//				ctx->projection_matrix, oxyz+i*oxyzsz);
		}
	}
}

void RASWGL_TransformNormalsModelviewXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv[4];
	int i, j;
	
	if(ixyz==oxyz)
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformVectorT(ixyz+i*ixyzsz,
				ctx->modelview_matrix, tv);
			Vec3F_Copy(tv, oxyz+i*oxyzsz);
		}
	}else
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformVectorT(ixyz+i*ixyzsz,
				ctx->modelview_matrix, oxyz+i*oxyzsz);
		}
	}
}

void RASWGL_TransformNormalsProjectionXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv[4];
	int i, j;
	
	if(ixyz==oxyz)
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformVectorT(ixyz+i*ixyzsz,
				ctx->projection_matrix, tv);
			Vec3F_Copy(tv, oxyz+i*oxyzsz);
		}
	}else
	{
		for(i=0; i<nxyz; i++)
		{
			Mat4F_TransformVectorT(ixyz+i*ixyzsz,
				ctx->projection_matrix, oxyz+i*oxyzsz);
		}
	}
}


void RASWGL_TransformVerticesModelviewProjectionXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv1[4], tv2[4];
	int i, j;
	
	for(i=0; i<nxyz; i++)
	{
		Vec4F_Copy(ixyz+i*ixyzsz, tv1);
		tv1[3]=1;

		Mat4F_TransformVector4(tv1,
			ctx->modelview_projection_matrix, tv2);
//		Mat4F_TransformVector4T(tv1,
//			ctx->projection_matrix, tv2);
//		Mat4F_TransformPoint34G(tv1,
//			ctx->projection_matrix, tv2);

#if 0
		if(tv2[3]>=1)
//		if(tv2[3]>0.1)
//		if(tv2[3]>0)
//		if(tv2[3]!=0)
		{
			tv2[0]=tv2[0]/tv2[3];
			tv2[1]=tv2[1]/tv2[3];
			tv2[2]=tv2[2]/tv2[3];
		}else
//			if(tv2[3]<-(0.1))
//			if(tv2[3]<0)
			if(tv2[3]<=(-1))
		{
			tv2[0]=tv2[0]/(-tv2[3]);
			tv2[1]=tv2[1]/(-tv2[3]);
			tv2[2]=tv2[2]/(-tv2[3]);
		}
#endif

		Vec4F_Copy(tv2, oxyz+i*oxyzsz);

//		Mat4F_TransformVector4T(ixyz+i*ixyzsz,
//			ctx->projection_matrix, tv);
//		Mat4F_TransformPointT(ixyz+i*ixyzsz,
//			ctx->projection_matrix, tv);
//		Vec4F_Copy(tv, oxyz+i*oxyzsz);
	}
}

void RASWGL_TransformNormalsModelviewProjectionXYZ(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv1[4], tv2[4];
	int i, j;
	
	for(i=0; i<nxyz; i++)
	{
		Vec4F_Copy(ixyz+i*ixyzsz, tv1);
		tv1[3]=1;

		Mat4F_TransformVectorT(tv1,
			ctx->modelview_projection_matrix, tv2);
		Vec3F_Copy(tv2, oxyz+i*oxyzsz);
	}
}

void RASWGL_TransformVerticesDivide_XYZ_W(
	RASWGL_Context *ctx, int nxyz,
	int ixyzsz, float *ixyz,
	int oxyzsz, float *oxyz)
{
	float tv1[4], tv2[4];
	int i, j;
	
	for(i=0; i<nxyz; i++)
	{
		Vec4F_Copy(ixyz+i*ixyzsz, tv1);
//		tv1[3]=1;

		if(tv1[3]>=1)
//		if(tv2[3]>0.1)
//		if(tv2[3]>0.001)
//		if(tv2[3]>0)
//		if(tv2[3]!=0)
		{
			tv2[0]=tv1[0]/tv1[3];
			tv2[1]=tv1[1]/tv1[3];
			tv2[2]=tv1[2]/tv1[3];
			tv2[3]=tv1[3];
		}else
#if 1
//			if(tv2[3]<-(0.1))
//			if(tv2[3]<0)
			if(tv1[3]<=(-1))
//			if(tv2[3]<-(0.001))
		{
			tv2[0]=tv1[0]/(-tv1[3]);
			tv2[1]=tv1[1]/(-tv1[3]);
			tv2[2]=tv1[2]/(-tv1[3]);
			tv2[3]=tv1[3];
		}else
#endif
		{
			tv2[0]=tv1[0];
			tv2[1]=tv1[1];
			tv2[2]=tv1[2];
			tv2[3]=tv1[3];
		}
			
		Vec4F_Copy(tv2, oxyz+i*oxyzsz);
	}
}

float raswgl_perspprojxyzw(float *xyz0, float *dxyz)
{
	float tv0[4], tv1[4];
	float f, g;
	
	f=xyz0[3];
	if(f>=1)
	{
		dxyz[0]=512*(xyz0[0]/f);
		dxyz[1]=384*(xyz0[1]/f);
	}else
	{
		dxyz[0]=512*xyz0[0];
		dxyz[1]=384*xyz0[1];
	}
}

float raswgl_perspdist2xyzw(float *xyz0, float *xyz1)
{
	float tv0[4], tv1[4];
	float f, g;
	
	f=xyz0[3];
	if(f>=1)
	{
		tv0[0]=512*(xyz0[0]/f);
		tv0[1]=384*(xyz0[1]/f);
//		tv0[2]=(xyz0[2]/f);
	}else
	{
//		tv0[0]=1024*(xyz0[0]/0.000001);
//		tv0[1]=1024*(xyz0[1]/0.000001);
//		tv0[2]=1024*(xyz0[2]/0.000001);

		tv0[0]=512*xyz0[0];
		tv0[1]=384*xyz0[1];
//		tv0[2]=xyz0[2];
	}

	f=xyz1[3];
	if(f>=1)
	{
		tv1[0]=512*(xyz1[0]/f);
		tv1[1]=384*(xyz1[1]/f);
//		tv1[2]=(xyz1[2]/f);
	}else
	{
		tv1[0]=512*xyz1[0];
		tv1[1]=384*xyz1[1];
//		tv1[2]=xyz1[2];

//		tv1[0]=1024*(xyz1[0]/0.000001);
//		tv1[1]=1024*(xyz1[1]/0.000001);
//		tv1[2]=1024*(xyz1[2]/0.000001);
	}
	
//	g=(tv0[2]-tv1[2]);
//	f=V2F_DIST2(tv0, tv1)*(g*g);
	f=V2F_DIST2(tv0, tv1);
	return(f);
}

float raswgl_avgdist2xyzw(
	float *xyz0, float *xyz1, float *oxyz,
	float *st0, float *st1, float *ost,
	float *nv0, float *nv1, float *onv,
	float *rgb0, float *rgb1, float *orgb)
{
	float f, g, h;

	oxyz[0]=(xyz0[0]+xyz1[0])*0.5;
	oxyz[1]=(xyz0[1]+xyz1[1])*0.5;
	oxyz[2]=(xyz0[2]+xyz1[2])*0.5;
	oxyz[3]=(xyz0[3]+xyz1[3])*0.5;

	ost[0]=(st0[0]+st1[0])*0.5;
	ost[1]=(st0[1]+st1[1])*0.5;

#if 0
	if((xyz0[3]>=1) && (xyz1[3]>=1))
	{
		f=xyz0[2]/xyz0[3]; g=xyz1[2]/xyz1[3];
		h=(f+g)*0.5;
		ost[0]=((st0[0]/f)+(st1[0]/g))*0.5*h;
		ost[1]=((st0[1]/f)+(st1[1]/g))*0.5*h;
	}else
	{
		ost[0]=(st0[0]+st1[0])*0.5;
		ost[1]=(st0[1]+st1[1])*0.5;
	}
#endif

	onv[0]=(nv0[0]+nv1[0])*0.5;
	onv[1]=(nv0[1]+nv1[1])*0.5;
	onv[2]=(nv0[2]+nv1[2])*0.5;
	onv[3]=(nv0[3]+nv1[3])*0.5;

	orgb[0]=(rgb0[0]+rgb1[0])*0.5;
	orgb[1]=(rgb0[1]+rgb1[1])*0.5;
	orgb[2]=(rgb0[2]+rgb1[2])*0.5;
	orgb[3]=(rgb0[3]+rgb1[3])*0.5;
}

int RASWGL_TransformSubdivideInsertTriangle(
	RASWGL_Context *ctx, int rc,
	float *xyz0,	float *st0,		float *nv0,		float *rgb0,
	float *xyz1,	float *st1,		float *nv1,		float *rgb1,
	float *xyz2,	float *st2,		float *nv2,		float *rgb2,
	float *oxyz,	float *ost,		float *onv,		float *orgb,
	float *exyz,	float *est,		float *env,		float *ergb)
{
	float rxyz[4][4], rst[4][2], rnv[4][4], rclr[4][4];
	float pxyz[4][4], pst[4][2], pnv[4][4], pclr[4][4];
	float d0, d1, d2, d3;
	int otris;
	
	if(oxyz>=exyz)
		return(0);

	if((xyz0[3]<0) && (xyz1[3]<0) && (xyz2[3]<0))
		return(0);

	raswgl_perspprojxyzw(xyz0, pxyz[0]);
	raswgl_perspprojxyzw(xyz1, pxyz[1]);
	raswgl_perspprojxyzw(xyz2, pxyz[2]);

	d0=V2F_DIST2(pxyz[0], pxyz[1]);
	d1=V2F_DIST2(pxyz[1], pxyz[2]);
	d2=V2F_DIST2(pxyz[2], pxyz[0]);
	d3=d0+d1+d2;

#if 1
	if((pxyz[0][0]<(-512)) && (pxyz[1][0]<(-512)) && (pxyz[2][0]<(-512)))
		return(0);
	if((pxyz[0][0]>512) && (pxyz[1][0]>512) && (pxyz[2][0]>512))
		return(0);
	if((pxyz[0][1]<(-384)) && (pxyz[1][1]<(-384)) && (pxyz[2][1]<(-384)))
		return(0);
	if((pxyz[0][1]>384) && (pxyz[1][1]>384) && (pxyz[2][1]>384))
		return(0);
#endif

//	d0=raswgl_perspdist2xyzw(xyz0, xyz1);
//	d1=raswgl_perspdist2xyzw(xyz1, xyz2);
//	d2=raswgl_perspdist2xyzw(xyz2, xyz0);
//	d3=d0+d1+d2;

//	if((rc>3) || ((d0+d1+d2)<(3*256)))
//	if((rc>3) || ((d0+d1+d2)<(3*4096)))
//	if((rc>2) || ((d0+d1+d2)<(3*65536)))
//	if((rc>5) || ((d0+d1+d2)<(3*16384)))
//	if((rc>3) || ((d0+d1+d2)<(3*16384)))
//	if((rc>2) || (d3<(3*16384)))
//	if((rc>3) || (d3<(3*24576)))
	if((rc>3) || (d3<(3*16384)))
//	if((rc>1) || ((d0+d1+d2)<(3*16384)))
//	if(1)
//	if((rc>0) || ((d0+d1+d2)<(3*16384)))
	{
		//discard tiny triangles
//		if((d3<(3*2)) || (d0<1) || (d1<1) || (d2<1))
		if(d3<(3*2))
			return(0);
	
		Vec4F_Copy3Gather(xyz0, xyz1, xyz2, oxyz);
		Vec4F_Copy3Gather(nv0, nv1, nv2, onv);
		Vec4F_Copy3Gather(rgb0, rgb1, rgb2, orgb);

		Vec2F_Copy3Gather(st0, st1, st2, ost);

//		Vec4F_Copy(xyz0, oxyz+0*4);		Vec2F_Copy(st0, ost+0*2);
//		Vec4F_Copy(nv0, onv+0*4);		Vec4F_Copy(rgb0, orgb+0*4);
//		Vec4F_Copy(xyz1, oxyz+1*4);		Vec2F_Copy(st1, ost+1*2);
//		Vec4F_Copy(nv1, onv+1*4);		Vec4F_Copy(rgb1, orgb+1*4);
//		Vec4F_Copy(xyz2, oxyz+2*4);		Vec2F_Copy(st2, ost+2*2);
//		Vec4F_Copy(nv2, onv+2*4);		Vec4F_Copy(rgb2, orgb+2*4);
		return(1);
	}
	
	Vec4F_Copy(xyz0, rxyz[0]);	Vec2F_Copy( st0,  rst[0]);
	Vec4F_Copy( nv0,  rnv[0]);	Vec4F_Copy(rgb0, rclr[0]);
	Vec4F_Copy(xyz1, rxyz[1]);	Vec2F_Copy( st1,  rst[1]);
	Vec4F_Copy( nv1,  rnv[1]);	Vec4F_Copy(rgb1, rclr[1]);
	Vec4F_Copy(xyz2, rxyz[2]);	Vec2F_Copy( st2,  rst[2]);
	Vec4F_Copy( nv2,  rnv[2]);	Vec4F_Copy(rgb2, rclr[2]);

	raswgl_avgdist2xyzw(
		rxyz[0], rxyz[1], pxyz[0],		 rst[0],  rst[1],  pst[0],
		 rnv[0],  rnv[1],  pnv[0],		rclr[0], rclr[1], pclr[0]);
	raswgl_avgdist2xyzw(
		rxyz[1], rxyz[2], pxyz[1],		 rst[1],  rst[2],  pst[1],
		 rnv[1],  rnv[2],  pnv[1],		rclr[1], rclr[2], pclr[1]);
	raswgl_avgdist2xyzw(
		rxyz[2], rxyz[0], pxyz[2],		 rst[2],  rst[0],  pst[2],
		 rnv[2],  rnv[0],  pnv[2],		rclr[2], rclr[0], pclr[2]);

	otris=0;
	otris+=RASWGL_TransformSubdivideInsertTriangle(ctx, rc+1,
		rxyz[0], rst[0], rnv[0], rclr[0],
		pxyz[0], pst[0], pnv[0], pclr[0],
		pxyz[2], pst[2], pnv[2], pclr[2],
		oxyz+otris*3*4, ost+otris*3*2,
		onv+otris*3*4, orgb+otris*3*4,
		exyz, est, env, ergb);
	otris+=RASWGL_TransformSubdivideInsertTriangle(ctx, rc+1,
		pxyz[0], pst[0], pnv[0], pclr[0],
		rxyz[1], rst[1], rnv[1], rclr[1],
		pxyz[1], pst[1], pnv[1], pclr[1],
		oxyz+otris*3*4, ost+otris*3*2,
		onv+otris*3*4, orgb+otris*3*4,
		exyz, est, env, ergb);
	otris+=RASWGL_TransformSubdivideInsertTriangle(ctx, rc+1,
		pxyz[2], pst[2], pnv[2], pclr[2],
		pxyz[1], pst[1], pnv[1], pclr[1],
		rxyz[2], rst[2], rnv[2], rclr[2],
		oxyz+otris*3*4, ost+otris*3*2,
		onv+otris*3*4, orgb+otris*3*4,
		exyz, est, env, ergb);
	otris+=RASWGL_TransformSubdivideInsertTriangle(ctx, rc+1,
		pxyz[0], pst[0], pnv[0], pclr[0],
		pxyz[1], pst[1], pnv[1], pclr[1],
		pxyz[2], pst[2], pnv[2], pclr[2],
		oxyz+otris*3*4, ost+otris*3*2,
		onv+otris*3*4, orgb+otris*3*4,
		exyz, est, env, ergb);
	return(otris);
}

int RASWGL_TransformSubdivideInsertQuad(
	RASWGL_Context *ctx, int rc,
	float *xyz0,	float *st0,		float *nv0,		float *rgb0,
	float *xyz1,	float *st1,		float *nv1,		float *rgb1,
	float *xyz2,	float *st2,		float *nv2,		float *rgb2,
	float *xyz3,	float *st3,		float *nv3,		float *rgb3,
	float *oxyz,	float *ost,		float *onv,		float *orgb,
	float *exyz,	float *est,		float *env,		float *ergb)
{
	float rxyz[8][4], rst[8][2], rnv[8][4], rclr[8][4];
	float pxyz[8][4], pst[8][2], pnv[8][4], pclr[8][4];
	float d0, d1, d2, d3;
	int oq;

	if(oxyz>=exyz)
		return(0);

	if((xyz0[3]<0) && (xyz1[3]<0) &&
		(xyz2[3]<0) && (xyz3[3]<0))
			return(0);

	d0=raswgl_perspdist2xyzw(xyz0, xyz1);
	d1=raswgl_perspdist2xyzw(xyz1, xyz2);
	d2=raswgl_perspdist2xyzw(xyz2, xyz3);
	d3=raswgl_perspdist2xyzw(xyz3, xyz0);

//	if((rc>3) || ((d0+d1+d2+d3)<(4*256)))
//	if((rc>3) || ((d0+d1+d2+d3)<(4*4096)))
//	if((rc>5) || ((d0+d1+d2+d3)<(4*16384)))
	if((rc>3) || ((d0+d1+d2+d3)<(4*16384)))
//	if((rc>2) || ((d0+d1+d2+d3)<(4*65536)))
//	if((rc>1) || ((d0+d1+d2+d3)<(4*16384)))
//	if(1)
//	if((rc>0) || ((d0+d1+d2+d3)<(4*16384)))
	{
		Vec4F_Copy(xyz0, oxyz+0*4);		Vec2F_Copy( st0,  ost+0*2);
		Vec4F_Copy( nv0,  onv+0*4);		Vec4F_Copy(rgb0, orgb+0*4);
		Vec4F_Copy(xyz1, oxyz+1*4);		Vec2F_Copy( st1,  ost+1*2);
		Vec4F_Copy( nv1,  onv+1*4);		Vec4F_Copy(rgb1, orgb+1*4);
		Vec4F_Copy(xyz2, oxyz+2*4);		Vec2F_Copy( st2,  ost+2*2);
		Vec4F_Copy( nv2,  onv+2*4);		Vec4F_Copy(rgb2, orgb+2*4);
		Vec4F_Copy(xyz3, oxyz+3*4);		Vec2F_Copy( st3,  ost+3*2);
		Vec4F_Copy( nv3,  onv+3*4);		Vec4F_Copy(rgb3, orgb+3*4);
		return(1);
	}
	
	Vec4F_Copy(xyz0, rxyz[0]);	Vec2F_Copy( st0,  rst[0]);
	Vec4F_Copy( nv0,  rnv[0]);	Vec4F_Copy(rgb0, rclr[0]);
	Vec4F_Copy(xyz1, rxyz[1]);	Vec2F_Copy( st1,  rst[1]);
	Vec4F_Copy( nv1,  rnv[1]);	Vec4F_Copy(rgb1, rclr[1]);
	Vec4F_Copy(xyz2, rxyz[2]);	Vec2F_Copy( st2,  rst[2]);
	Vec4F_Copy( nv2,  rnv[2]);	Vec4F_Copy(rgb2, rclr[2]);
	Vec4F_Copy(xyz3, rxyz[3]);	Vec2F_Copy( st3,  rst[3]);
	Vec4F_Copy( nv3,  rnv[3]);	Vec4F_Copy(rgb3, rclr[3]);

	raswgl_avgdist2xyzw(
		rxyz[0], rxyz[1], pxyz[0],		 rst[0],  rst[1],  pst[0],
		 rnv[0],  rnv[1],  pnv[0],		rclr[0], rclr[1], pclr[0]);
	raswgl_avgdist2xyzw(
		rxyz[1], rxyz[2], pxyz[1],		 rst[1],  rst[2],  pst[1],
		 rnv[1],  rnv[2],  pnv[1],		rclr[1], rclr[2], pclr[1]);
	raswgl_avgdist2xyzw(
		rxyz[2], rxyz[3], pxyz[2],		 rst[2],  rst[3],  pst[2],
		 rnv[2],  rnv[3],  pnv[2],		rclr[2], rclr[3], pclr[2]);
	raswgl_avgdist2xyzw(
		rxyz[3], rxyz[0], pxyz[3],		 rst[3],  rst[0],  pst[3],
		 rnv[3],  rnv[0],  pnv[3],		rclr[3], rclr[0], pclr[3]);

	raswgl_avgdist2xyzw(
		rxyz[0], rxyz[2], pxyz[6],		 rst[0],  rst[2],  pst[6],
		 rnv[0],  rnv[2],  pnv[6],		rclr[0], rclr[2], pclr[6]);
	raswgl_avgdist2xyzw(
		rxyz[1], rxyz[3], pxyz[7],		 rst[1],  rst[3],  pst[7],
		 rnv[1],  rnv[3],  pnv[7],		rclr[1], rclr[3], pclr[7]);

//	raswgl_avgdist2xyzw(
//		pxyz[3], pxyz[1], pxyz[6],		 pst[3],  pst[1],  pst[6],
//		 pnv[3],  pnv[1],  pnv[6],		pclr[3], pclr[1], pclr[6]);
//	raswgl_avgdist2xyzw(
//		pxyz[2], pxyz[0], pxyz[7],		 pst[2],  pst[0],  pst[7],
//		 pnv[2],  pnv[0],  pnv[7],		pclr[2], pclr[0], pclr[7]);
	raswgl_avgdist2xyzw(
		pxyz[6], pxyz[7], pxyz[4],		 pst[6],  pst[7],  pst[4],
		 pnv[6],  pnv[7],  pnv[4],		pclr[6], pclr[7], pclr[4]);

	oq=0;
	oq+=RASWGL_TransformSubdivideInsertQuad(ctx, rc+1,
		rxyz[0], rst[0], rnv[0], rclr[0],
		pxyz[0], pst[0], pnv[0], pclr[0],
		pxyz[4], pst[4], pnv[4], pclr[4],
		pxyz[3], pst[3], pnv[3], pclr[3],
		oxyz+oq*4*4,  ost+oq*4*2,
		 onv+oq*4*4, orgb+oq*4*4,
		exyz, est, env, ergb);
	oq+=RASWGL_TransformSubdivideInsertQuad(ctx, rc+1,
		pxyz[0], pst[0], pnv[0], pclr[0],
		rxyz[1], rst[1], rnv[1], rclr[1],
		pxyz[1], pst[1], pnv[1], pclr[1],
		pxyz[4], pst[4], pnv[4], pclr[4],
		oxyz+oq*4*4,  ost+oq*4*2,
		 onv+oq*4*4, orgb+oq*4*4,
		exyz, est, env, ergb);
	oq+=RASWGL_TransformSubdivideInsertQuad(ctx, rc+1,
		pxyz[4], pst[4], pnv[4], pclr[4],
		pxyz[1], pst[1], pnv[1], pclr[1],
		rxyz[2], rst[2], rnv[2], rclr[2],
		pxyz[2], pst[2], pnv[2], pclr[2],
		oxyz+oq*4*4,  ost+oq*4*2,
		 onv+oq*4*4, orgb+oq*4*4,
		exyz, est, env, ergb);
	oq+=RASWGL_TransformSubdivideInsertQuad(ctx, rc+1,
		pxyz[3], pst[3], pnv[3], pclr[3],
		pxyz[4], pst[4], pnv[4], pclr[4],
		pxyz[2], pst[2], pnv[2], pclr[2],
		rxyz[3], rst[3], rnv[3], rclr[3],
		oxyz+oq*4*4,  ost+oq*4*2,
		 onv+oq*4*4, orgb+oq*4*4,
		exyz, est, env, ergb);
	return(oq);
}

int RASWGL_TransformSubdivideCheckCull(RASWGL_Context *ctx,
	float *xyz0, float *xyz1, float *xyz2)
{
	float tv0[3], tv1[3];
	float f;

	if(ctx->caps_enabled&BGBRASW_ENABLE_CULL_FACE)
	{
		V3F_SUB(xyz1, xyz0, tv0);
		V3F_SUB(xyz2, xyz0, tv1);
		f=V3F_DOT(tv0, tv1);
		if(f<0)return(1);
	}
	return(0);
} 
	
int RASWGL_TransformSubdivideTriangles(
	RASWGL_Context *ctx, int ntris, int mtris,
	float *ixyz,	float *ist,
	float *inv,		float *irgb,
	float *oxyz,	float *ost,
	float *onv,		float *orgb)
{
	int ontris;
	int i, j, k;
	
	ontris=0;
	for(i=0; i<ntris; i++)
	{
//		if(RASWGL_TransformSubdivideCheckCull(ctx,
//			ixyz+(i*3+0)*4, ixyz+(i*3+1)*4, ixyz+(i*3+2)*4))
//				continue;
		j=RASWGL_TransformSubdivideInsertTriangle(ctx, 0,
			ixyz+(i*3+0)*4, ist+(i*3+0)*2, inv+(i*3+0)*4, irgb+(i*3+0)*4, 
			ixyz+(i*3+1)*4, ist+(i*3+1)*2, inv+(i*3+1)*4, irgb+(i*3+1)*4, 
			ixyz+(i*3+2)*4, ist+(i*3+2)*2, inv+(i*3+2)*4, irgb+(i*3+2)*4,
			oxyz+ontris*3*4,	 ost+ontris*3*2,
			 onv+ontris*3*4,	orgb+ontris*3*4,
			oxyz+mtris*3*4,		 ost+mtris*3*2,
			 onv+mtris*3*4,		orgb+mtris*3*4);
		ontris+=j;
	}
	return(ontris);
}

int RASWGL_TransformSubdivideQuads(
	RASWGL_Context *ctx, int nquad, int mquad,
	float *ixyz,	float *ist,
	float *inv,		float *irgb,
	float *oxyz,	float *ost,
	float *onv,		float *orgb)
{
	int onquad;
	int i, j, k;
	
	onquad=0;
	for(i=0; i<nquad; i++)
	{
		j=RASWGL_TransformSubdivideInsertQuad(ctx, 0,
			ixyz+(i*4+0)*4, ist+(i*4+0)*2, inv+(i*4+0)*4, irgb+(i*4+0)*4, 
			ixyz+(i*4+1)*4, ist+(i*4+1)*2, inv+(i*4+1)*4, irgb+(i*4+1)*4, 
			ixyz+(i*4+2)*4, ist+(i*4+2)*2, inv+(i*4+2)*4, irgb+(i*4+2)*4,
			ixyz+(i*4+3)*4, ist+(i*4+3)*2, inv+(i*4+3)*4, irgb+(i*4+3)*4,
			oxyz+onquad*4*4,	 ost+onquad*4*2,
			 onv+onquad*4*4,	orgb+onquad*4*4,
			oxyz+mquad*4*4,		 ost+mquad*4*2,
			 onv+mquad*4*4,		orgb+mquad*4*4);
		onquad+=j;
	}
	return(onquad);
}

void RASWGL_TransformDrawTriangles(
	RASWGL_Context *ctx,
	int base, int nxyz,
	int xyzsize, float *xyz,
	int stsize, float *st,
	int normsize, float *norm,
	int rgbsize, float *rgb)
{
	float clip[8*4];
	BGBRASW_Primitive *plst;
	int nclip;
	int nxyz2, mxyz2;
	int ntris, ntris2, mtris2;
	int i, j, k;

	RASWGL_CheckExpandTransform0Vertices(ctx, nxyz*16);
	RASWGL_CheckExpandTransform1Vertices(ctx, nxyz*16);

#if 1

#if 0
	RASWGL_TransformVerticesModelviewXYZ(
		ctx, nxyz,
		xyzsize, xyz+base*xyzsize,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformVerticesProjectionXYZ(
		ctx, nxyz,
		4, ctx->trans0_xyzbuf,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformNormalsModelviewXYZ(
		ctx, nxyz,
		normsize, norm+base*normsize,
		4, ctx->trans0_nvbuf);

	RASWGL_TransformNormalsProjectionXYZ(
		ctx, nxyz,
		4, ctx->trans0_nvbuf,
		4, ctx->trans0_nvbuf);
#endif

#if 1
	RASWGL_TransformVerticesModelviewProjectionXYZ(
		ctx, nxyz,
		xyzsize, xyz+base*xyzsize,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformNormalsModelviewProjectionXYZ(
		ctx, nxyz,
		normsize, norm+base*normsize,
		4, ctx->trans0_nvbuf);

#if 1
	ntris=nxyz/3;
	mtris2=ctx->trans0_mvec/3;
	ntris2=RASWGL_TransformSubdivideTriangles(ctx, ntris, mtris2,
		ctx->trans0_xyzbuf, st, ctx->trans0_nvbuf, rgb,
		ctx->trans1_xyzbuf,		ctx->trans0_stbuf,
		ctx->trans1_nvbuf,		ctx->trans0_clrbuf);
	
	if(ntris2>=mtris2)
	{
		i=(mtris2+(mtris2>>1))*3;
		RASWGL_CheckExpandTransform0Vertices(ctx, i);
		RASWGL_CheckExpandTransform1Vertices(ctx, i);
	}
	
	ntris=ntris2;
	nxyz=3*ntris2;

	RASWGL_CheckExpandTransform0Vertices(ctx, nxyz*3);
	RASWGL_CheckExpandTransform1Vertices(ctx, nxyz*3);
#endif
	
	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans1_xyzbuf, 4, ctx->trans0_xyzbuf);

	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans1_nvbuf, 4, ctx->trans0_nvbuf);
#endif

	clip[0*4+0]=-1;	clip[0*4+1]=0;	clip[0*4+2]=0;	clip[0*4+3]=1;
	clip[1*4+0]= 1;	clip[1*4+1]=0;	clip[1*4+2]=0;	clip[1*4+3]=1;
	clip[2*4+0]=0;	clip[2*4+1]=-1;	clip[2*4+2]=0;	clip[2*4+3]=1;
	clip[3*4+0]=0;	clip[3*4+1]= 1;	clip[3*4+2]=0;	clip[3*4+3]=1;
	clip[4*4+0]=0;	clip[4*4+1]=0;	clip[4*4+2]=-1;	clip[4*4+3]=1;
//	clip[4*4+0]=0;	clip[4*4+1]=0;	clip[4*4+2]=-1;	clip[4*4+3]=0;
	clip[5*4+0]=0;	clip[5*4+1]=0;	clip[5*4+2]= 1;	clip[5*4+3]=1;

	nclip=6;
//	if(!(ctx->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST))
//		{ nclip=4; }

#if 1
	nxyz2=RASWGL_TransformClipTriangles(
		ctx, nxyz, nclip,
		4, clip,
		4, 2, 4, 4,
		ctx->trans0_xyzbuf,		ctx->trans0_stbuf,
		ctx->trans0_nvbuf,		ctx->trans0_clrbuf,
		ctx->trans1_xyzbuf,		ctx->trans1_stbuf,
		ctx->trans1_nvbuf,		ctx->trans1_clrbuf);
#endif

#if 0	
	nxyz2=RASWGL_TransformClipTriangles(
		ctx, nxyz, 6,
		4, clip,
		4, stsize, 4, rgbsize,
		ctx->trans0_xyzbuf, st, ctx->trans0_nvbuf, rgb,
		ctx->trans1_xyzbuf,		ctx->trans1_stbuf,
		ctx->trans1_nvbuf,		ctx->trans1_clrbuf);
#endif
	
#if 0	
	plst=RASWGL_BuildPrimitiveListArrays(
		ctx,
		GL_TRIANGLES, 0, nxyz2,
		4, ctx->trans1_xyzbuf,
		stsize, ctx->trans1_stbuf,
		4, ctx->trans1_nvbuf,
		rgbsize, ctx->trans1_clrbuf);
#endif

#if 1
	RASWGL_BuildInsertPrimitiveArrays(
		ctx,
		GL_TRIANGLES, 0, nxyz2,
		4, ctx->trans1_xyzbuf,
		2, ctx->trans1_stbuf,
		4, ctx->trans1_nvbuf,
		4, ctx->trans1_clrbuf);
#endif

#endif

//	BGBRASW_SetupTestBlend(ctx->ractx, ctx->tabs);
//	BGBRASW_DrawPrimitiveList(ctx->ractx, plst);
//	BGBRASW_FreePrimitiveList(ctx->ractx, plst);
}

void RASWGL_TransformDrawQuads(
	RASWGL_Context *ctx,
	int base, int nxyz,
	int xyzsize, float *xyz,
	int stsize, float *st,
	int normsize, float *norm,
	int rgbsize, float *rgb)
{
	float clip[8*4], tv1[4], tv2[4], tv3[4];
	BGBRASW_Primitive *plst;
	float f;
	int nquad2, mquad2, nquad;
	int nxyz2;
	int i, j, k;

	RASWGL_CheckExpandTransform0Vertices(ctx, nxyz*4);
	RASWGL_CheckExpandTransform1Vertices(ctx, nxyz*4);

#if 1

#if 0
	RASWGL_TransformVerticesModelviewXYZ(
		ctx, nxyz,
		xyzsize, xyz+base*xyzsize,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformVerticesProjectionXYZ(
		ctx, nxyz,
		4, ctx->trans0_xyzbuf,
		4, ctx->trans0_xyzbuf);
	
	RASWGL_TransformNormalsModelviewXYZ(
		ctx, nxyz,
		normsize, norm+base*normsize,
		4, ctx->trans0_nvbuf);

	RASWGL_TransformNormalsProjectionXYZ(
		ctx, nxyz,
		4, ctx->trans0_nvbuf,
		4, ctx->trans0_nvbuf);
#endif

#if 0
	RASWGL_TransformVerticesModelviewProjectionXYZ(
		ctx, nxyz,
		xyzsize, xyz+base*xyzsize,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformNormalsModelviewProjectionXYZ(
		ctx, nxyz,
		normsize, norm+base*normsize,
		4, ctx->trans0_nvbuf);

	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans0_xyzbuf, 4, ctx->trans0_xyzbuf);

	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans0_nvbuf, 4, ctx->trans0_nvbuf);
#endif

#if 1
	RASWGL_TransformVerticesModelviewProjectionXYZ(
		ctx, nxyz,
		xyzsize, xyz+base*xyzsize,
		4, ctx->trans0_xyzbuf);

	RASWGL_TransformNormalsModelviewProjectionXYZ(
		ctx, nxyz,
		normsize, norm+base*normsize,
		4, ctx->trans0_nvbuf);

#if 1
	nquad=nxyz/4;
	mquad2=ctx->trans0_mvec/4;
	nquad2=RASWGL_TransformSubdivideQuads(ctx, nquad, mquad2,
		ctx->trans0_xyzbuf, st, ctx->trans0_nvbuf, rgb,
		ctx->trans1_xyzbuf,		ctx->trans0_stbuf,
		ctx->trans1_nvbuf,		ctx->trans0_clrbuf);
	
	if(nquad2>=mquad2)
	{
		i=(mquad2+(mquad2>>1))*4;
		RASWGL_CheckExpandTransform0Vertices(ctx, i);
		RASWGL_CheckExpandTransform1Vertices(ctx, i);
	}
	
	nquad=nquad2;
	nxyz=4*nquad2;
#endif
	
	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans1_xyzbuf, 4, ctx->trans0_xyzbuf);

	RASWGL_TransformVerticesDivide_XYZ_W(
		ctx, nxyz, 4, ctx->trans1_nvbuf, 4, ctx->trans0_nvbuf);
#endif

	clip[0*4+0]=-1;	clip[0*4+1]=0;	clip[0*4+2]=0;	clip[0*4+3]=1;
	clip[1*4+0]= 1;	clip[1*4+1]=0;	clip[1*4+2]=0;	clip[1*4+3]=1;
	clip[2*4+0]=0;	clip[2*4+1]=-1;	clip[2*4+2]=0;	clip[2*4+3]=1;
	clip[3*4+0]=0;	clip[3*4+1]= 1;	clip[3*4+2]=0;	clip[3*4+3]=1;
	clip[4*4+0]=0;	clip[4*4+1]=0;	clip[4*4+2]=-1;	clip[4*4+3]=1;
//	clip[4*4+0]=0;	clip[4*4+1]=0;	clip[4*4+2]=-1;	clip[4*4+3]=0;
	clip[5*4+0]=0;	clip[5*4+1]=0;	clip[5*4+2]= 1;	clip[5*4+3]=1;

#if 1
	nxyz2=RASWGL_TransformClipQuads(
		ctx, nxyz, 6,
		4, clip,
		4, 2, 4, 4,
		ctx->trans0_xyzbuf,		ctx->trans0_stbuf,
		ctx->trans0_nvbuf,		ctx->trans0_clrbuf,
		ctx->trans1_xyzbuf,		ctx->trans1_stbuf,
		ctx->trans1_nvbuf,		ctx->trans1_clrbuf);
#endif

#if 0
	nxyz2=RASWGL_TransformClipQuads(
		ctx, nxyz, 6,
		4, clip,
		4, stsize, normsize, rgbsize,
		ctx->trans0_xyzbuf, st, norm, rgb,
		ctx->trans1_xyzbuf,		ctx->trans1_stbuf,
		ctx->trans1_nvbuf,		ctx->trans1_clrbuf);
#endif

#if 0
	plst=RASWGL_BuildPrimitiveListArrays(
		ctx,
		GL_QUADS, 0, nxyz2,
		4, ctx->trans1_xyzbuf,
		stsize, ctx->trans1_stbuf,
		normsize, ctx->trans1_nvbuf,
		rgbsize, ctx->trans1_clrbuf);
#endif

#if 1
	RASWGL_BuildInsertPrimitiveArrays(
		ctx,
		GL_QUADS, 0, nxyz2,
		4, ctx->trans1_xyzbuf,
		stsize, ctx->trans1_stbuf,
		normsize, ctx->trans1_nvbuf,
		rgbsize, ctx->trans1_clrbuf);
#endif

#endif
	
//	BGBRASW_SetupTestBlend(ctx->ractx, ctx->tabs);
//	BGBRASW_DrawPrimitiveList(ctx->ractx, plst);
//	BGBRASW_FreePrimitiveList(ctx->ractx, plst);
}

void RASWGL_TransformDrawPolygon(
	RASWGL_Context *ctx,
	int base, int nxyz,
	int xyzsz, float *xyz,
	int stsz, float *st,
	int nvsz, float *nv,
	int rgbsz, float *rgb)
{
	int nxyz2;
	int i, j, n, nt;

//	return;

	if(nxyz==3)
	{
		RASWGL_TransformDrawTriangles(
			ctx, base, nxyz,
			xyzsz, xyz,		stsz, st,
			nvsz, nv,		rgbsz, rgb);
		return;
	}

#if 1
	if(nxyz==4)
	{
		RASWGL_TransformDrawQuads(
			ctx, base, nxyz,
			xyzsz, xyz,		stsz, st,
			nvsz, nv,		rgbsz, rgb);
		return;
	}
#endif

//	return;

//	RASWGL_CheckExpandTransform0Vertices(ctx, nxyz*2);
//	RASWGL_CheckExpandTransform1Vertices(ctx, nxyz*2);

	RASWGL_CheckExpandTransformNVertices(ctx, nxyz*3);
	
	nt=0;
	for(j=0; j<(nxyz-2); j++)
	{
		Vec3F_Copy(xyz+(base*xyzsz), ctx->transN_xyzbuf+(nt*3+0)*xyzsz);
		Vec2F_Copy( st+(base* stsz),  ctx->transN_stbuf+(nt*3+0)* stsz);
		Vec3F_Copy( nv+(base* nvsz),  ctx->transN_nvbuf+(nt*3+0)* nvsz);
		Vec4F_Copy(rgb+(base*rgbsz), ctx->transN_clrbuf+(nt*3+0)*rgbsz);
			
		Vec3F_Copy(xyz+(base+j+1)*xyzsz, ctx->transN_xyzbuf+(nt*3+1)*xyzsz);
		Vec2F_Copy( st+(base+j+1)* stsz,  ctx->transN_stbuf+(nt*3+1)* stsz);
		Vec3F_Copy( nv+(base+j+1)* nvsz,  ctx->transN_nvbuf+(nt*3+1)* nvsz);
		Vec4F_Copy(rgb+(base+j+1)*rgbsz, ctx->transN_clrbuf+(nt*3+1)*rgbsz);

		Vec3F_Copy(xyz+(base+j+2)*xyzsz, ctx->transN_xyzbuf+(nt*3+2)*xyzsz);
		Vec2F_Copy( st+(base+j+2)* stsz,  ctx->transN_stbuf+(nt*3+2)* stsz);
		Vec3F_Copy( nv+(base+j+2)* nvsz,  ctx->transN_nvbuf+(nt*3+2)* nvsz);
		Vec4F_Copy(rgb+(base+j+2)*rgbsz, ctx->transN_clrbuf+(nt*3+2)*rgbsz);

		nt++;
	}

	RASWGL_TransformDrawTriangles(
		ctx, 0, nt*3,
		xyzsz, ctx->transN_xyzbuf,
		 stsz, ctx->transN_stbuf,
		 nvsz, ctx->transN_nvbuf,
		rgbsz, ctx->transN_clrbuf);
}

void RASWGL_TransformDrawTriangleStrip(
	RASWGL_Context *ctx,
	int base, int nxyz,
	int xyzsz, float *xyz,
	int stsz, float *st,
	int nvsz, float *nv,
	int rgbsz, float *rgb)
{
	int nxyz2;
	int i0, i1, i2;
	int i, j, n, nt;

//	RASWGL_CheckExpandTransform0Vertices(ctx, nxyz*2);
//	RASWGL_CheckExpandTransform1Vertices(ctx, nxyz*2);

	RASWGL_CheckExpandTransformNVertices(ctx, nxyz*3);
	
	if(nxyz&1)
		{ i0=0; i1=1; i2=2; }
	else
		{ i0=1; i1=0; i2=2; }
	
	nt=0;
	for(j=0; j<(nxyz-2); j++)
	{
		Vec3F_Copy(xyz+(base+j+i0)*xyzsz, ctx->transN_xyzbuf+(nt*3+0)*xyzsz);
		Vec2F_Copy( st+(base+j+i0)* stsz,  ctx->transN_stbuf+(nt*3+0)* stsz);
		Vec3F_Copy( nv+(base+j+i0)* nvsz,  ctx->transN_nvbuf+(nt*3+0)* nvsz);
		Vec4F_Copy(rgb+(base+j+i0)*rgbsz, ctx->transN_clrbuf+(nt*3+0)*rgbsz);
			
		Vec3F_Copy(xyz+(base+j+i1)*xyzsz, ctx->transN_xyzbuf+(nt*3+1)*xyzsz);
		Vec2F_Copy( st+(base+j+i1)* stsz,  ctx->transN_stbuf+(nt*3+1)* stsz);
		Vec3F_Copy( nv+(base+j+i1)* nvsz,  ctx->transN_nvbuf+(nt*3+1)* nvsz);
		Vec4F_Copy(rgb+(base+j+i1)*rgbsz, ctx->transN_clrbuf+(nt*3+1)*rgbsz);

		Vec3F_Copy(xyz+(base+j+i2)*xyzsz, ctx->transN_xyzbuf+(nt*3+2)*xyzsz);
		Vec2F_Copy( st+(base+j+i2)* stsz,  ctx->transN_stbuf+(nt*3+2)* stsz);
		Vec3F_Copy( nv+(base+j+i2)* nvsz,  ctx->transN_nvbuf+(nt*3+2)* nvsz);
		Vec4F_Copy(rgb+(base+j+i2)*rgbsz, ctx->transN_clrbuf+(nt*3+2)*rgbsz);

		nt++;
	}

	RASWGL_TransformDrawTriangles(
		ctx, 0, nt*3,
		xyzsz, ctx->transN_xyzbuf,
		 stsz, ctx->transN_stbuf,
		 nvsz, ctx->transN_nvbuf,
		rgbsz, ctx->transN_clrbuf);
}

void RASWGL_TransformDrawQuadStrip(
	RASWGL_Context *ctx,
	int base, int nxyz,
	int xyzsz, float *xyz,
	int stsz, float *st,
	int nvsz, float *nv,
	int rgbsz, float *rgb)
{
	int nxyz2;
//	int i0, i1, i2, i3;
	int j0, j1, j2, j3;
	int i, j, n, nt;

	RASWGL_CheckExpandTransformNVertices(ctx, nxyz*3);
	
	nxyz2=nxyz/2;
	nt=0;
	for(j=1; j<nxyz2; j++)
	{
		j0=(base+2*j-1);
		j1=(base+2*j+0);
		j2=(base+2*j+2);
		j3=(base+2*j+1);

		Vec3F_Copy(xyz+j0*xyzsz, ctx->transN_xyzbuf+(nt*4+0)*xyzsz);
		Vec2F_Copy( st+j0* stsz,  ctx->transN_stbuf+(nt*4+0)* stsz);
		Vec3F_Copy( nv+j0* nvsz,  ctx->transN_nvbuf+(nt*4+0)* nvsz);
		Vec4F_Copy(rgb+j0*rgbsz, ctx->transN_clrbuf+(nt*4+0)*rgbsz);
			
		Vec3F_Copy(xyz+j1*xyzsz, ctx->transN_xyzbuf+(nt*4+1)*xyzsz);
		Vec2F_Copy( st+j1* stsz,  ctx->transN_stbuf+(nt*4+1)* stsz);
		Vec3F_Copy( nv+j1* nvsz,  ctx->transN_nvbuf+(nt*4+1)* nvsz);
		Vec4F_Copy(rgb+j1*rgbsz, ctx->transN_clrbuf+(nt*4+1)*rgbsz);

		Vec3F_Copy(xyz+j2*xyzsz, ctx->transN_xyzbuf+(nt*4+2)*xyzsz);
		Vec2F_Copy( st+j2* stsz,  ctx->transN_stbuf+(nt*4+2)* stsz);
		Vec3F_Copy( nv+j2* nvsz,  ctx->transN_nvbuf+(nt*4+2)* nvsz);
		Vec4F_Copy(rgb+j2*rgbsz, ctx->transN_clrbuf+(nt*4+2)*rgbsz);

		Vec3F_Copy(xyz+j3*xyzsz, ctx->transN_xyzbuf+(nt*4+3)*xyzsz);
		Vec2F_Copy( st+j3* stsz,  ctx->transN_stbuf+(nt*4+3)* stsz);
		Vec3F_Copy( nv+j3* nvsz,  ctx->transN_nvbuf+(nt*4+3)* nvsz);
		Vec4F_Copy(rgb+j3*rgbsz, ctx->transN_clrbuf+(nt*4+3)*rgbsz);

		nt++;
	}

	RASWGL_TransformDrawQuads(
		ctx, 0, nt*4,
		xyzsz, ctx->transN_xyzbuf,
		 stsz, ctx->transN_stbuf,
		 nvsz, ctx->transN_nvbuf,
		rgbsz, ctx->transN_clrbuf);
}

void RASWGL_SetupTabs(
	RASWGL_Context *ctx)
{
	BGBRASW_TestBlendData *tabs, *tabs2;
	int i, j;
	
	Mat4F_MatMultT(
		ctx->projection_matrix,
		ctx->modelview_matrix,
		ctx->modelview_projection_matrix);

	Mat4F_Inverse(ctx->modelview_projection_matrix,
		ctx->inv_modelview_projection_matrix);
	
	tabs=ctx->tabs;
	tabs->caps_enabled=ctx->caps_enabled;
	tabs->blend_src=ctx->blend_src;
	tabs->blend_dst=ctx->blend_dst;
	tabs->depth_func=ctx->depth_func;
	tabs->alpha_func=ctx->alpha_func;
	tabs->tex=ctx->texture;
	tabs->ref_clr=ctx->ref_clr;
	tabs->ref_z=ctx->ref_z;
	tabs->ref_sten=ctx->ref_sten;
	tabs->mask_clr=ctx->mask_clr;
	tabs->mask_z=ctx->mask_z;
	tabs->mask_sten=ctx->mask_sten;

	tabs->stencil_func=ctx->stencil_func;
	tabs->stencil_op_sfail=ctx->stencil_op_sfail;
	tabs->stencil_op_dpfail=ctx->stencil_op_dpfail;
	tabs->stencil_op_dppass=ctx->stencil_op_dppass;

	BGBRASW_SetupTestBlend(ctx->ractx, ctx->tabs);

	ctx->ractx->tabs=tabs;

	if(ctx->ractx_n_thread)
	{
		for(i=0; i<ctx->ractx_n_thread; i++)
		{
			BGBRASW_LockContext(ctx->ractx_thread[i]);
			tabs2=BGBRASW_AllocTestBlendData(ctx->ractx_thread[i]);
			BGBRASW_CopyTestBlend(ctx->ractx, tabs, tabs2);
			tabs2->refcount=1;
			ctx->ractx_thread[i]->tabs=tabs2;
			BGBRASW_UnlockContext(ctx->ractx_thread[i]);
		}
	}
}

void RASWGL_FinishTabs(
	RASWGL_Context *ctx)
{
	BGBRASW_TestBlendData *tabs, *tabs2;
	int i, j;
	
	if(ctx->ractx_n_thread)
	{
		for(i=0; i<ctx->ractx_n_thread; i++)
		{
			BGBRASW_LockContext(ctx->ractx_thread[i]);
			tabs=ctx->ractx_thread[i]->tabs;
			tabs->refcount--;
			if(!tabs->refcount)
			{
				BGBRASW_FreeTestBlendData(ctx->ractx_thread[i], tabs);
				ctx->ractx_thread[i]->tabs=NULL;
			}
			BGBRASW_UnlockContext(ctx->ractx_thread[i]);
		}
	}
}

void RASWGL_TransformDrawArrays(
	RASWGL_Context *ctx,
	int prim, int base, int nxyz,
	int xyzsize, float *xyz,
	int stsize, float *st,
	int normsize, float *norm,
	int rgbsize, float *rgb)
{
	RASWGL_SetupTabs(ctx);

	switch(prim)
	{
	case GL_TRIANGLES:
		RASWGL_TransformDrawTriangles(
			ctx, base, nxyz,
			xyzsize, xyz,
			stsize, st,
			normsize, norm,
			rgbsize, rgb);
		break;
	case GL_QUADS:
		RASWGL_TransformDrawQuads(
			ctx, base, nxyz,
			xyzsize, xyz,
			stsize, st,
			normsize, norm,
			rgbsize, rgb);
		break;
	case GL_POLYGON:
	case GL_TRIANGLE_FAN:
		RASWGL_TransformDrawPolygon(
			ctx, base, nxyz,
			xyzsize, xyz,
			stsize, st,
			normsize, norm,
			rgbsize, rgb);
		break;

#if 1
	case GL_TRIANGLE_STRIP:
		RASWGL_TransformDrawTriangleStrip(
			ctx, base, nxyz,
			xyzsize, xyz,
			stsize, st,
			normsize, norm,
			rgbsize, rgb);
		break;
	case GL_QUAD_STRIP:
		RASWGL_TransformDrawQuadStrip(
			ctx, base, nxyz,
			xyzsize, xyz,
			stsize, st,
			normsize, norm,
			rgbsize, rgb);
		break;
#endif
	default:
		break;
	}
	RASWGL_FinishTabs(ctx);
}

#if 0
void RASWGL_DrawBaseArrays(
	RASWGL_Context *ctx,
	int prim, int base, int nxyz,
	int xyzsize, int xyztype, int xyzstep, void *xyz, 
	int stsize, int sttype, int ststep, void *st, 
	int normsize, int normtype, int normstep, void *norm,
	int rgbsize, int rgbtype, int rgbstep, void *rgb)
{
}
#endif
