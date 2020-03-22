#include <tkgl/bgbrasw.h>

int RASWGL_TransformClipTriangles(
	RASWGL_Context *ctx, int nxyz, int nclip,
	int clipsz, float *clip,
	int xyzsz,		int stsz,
	int nvsz,		int rgbsz,
	float *ixyz,	float *ist,
	float *inv,		float *irgb,
	float *oxyz,	float *ost,
	float *onv,		float *orgb)
{
	float txyz1[64*4], tnv1[64*4], tst1[64*4], trgb1[64*4];
	float txyz2[64*4], tnv2[64*4], tst2[64*4], trgb2[64*4];
	int i, j, k, l, l1, n, on;
	
	n=nxyz/3; on=0;
	for(i=0; i<n; i++)
	{
		for(j=0; j<3; j++)
		{
			Vec4F_Copy(ixyz+(i*3+j)*xyzsz, txyz1+j*xyzsz);
			Vec2F_Copy(ist+(i*3+j)*stsz, tst1+j*stsz);
			Vec4F_Copy(inv+(i*3+j)*nvsz, tnv1+j*nvsz);
			Vec4F_Copy(irgb+(i*3+j)*rgbsz, trgb1+j*rgbsz);
		}
		l=3;

#if 1
		for(j=0; j<nclip; j++)
		{
			l1=Hull_ClipFaceSTNVRGBA(
				clip+j*clipsz,
				xyzsz, stsz, nvsz, rgbsz,
				txyz1, tst1, tnv1, trgb1,
				txyz2, tst2, tnv2, trgb2, l, 3);
			if(l1==-2)
				continue;
			if(l1<3)
				{ l=l1; break; }
			l=l1;
			for(k=0; k<l; k++)
			{
				Vec4F_Copy(txyz2+k*xyzsz, txyz1+k*xyzsz);
				Vec2F_Copy(tst2+k*stsz, tst1+k*stsz);
				Vec4F_Copy(tnv2+k*nvsz, tnv1+k*nvsz);
				Vec4F_Copy(trgb2+k*rgbsz, trgb1+k*rgbsz);
			}
		}
#endif
		
		//clipped away
		if(l<3)
		{
			if(!l)continue;
			continue;
		}
		for(j=1; j<(l-1); j++)
		{
			Vec4F_Copy(txyz1, oxyz+(on*3+0)*xyzsz);
			Vec2F_Copy( tst1,  ost+(on*3+0)* stsz);
			Vec4F_Copy( tnv1,  onv+(on*3+0)* nvsz);
			Vec4F_Copy(trgb1, orgb+(on*3+0)*rgbsz);
			
			Vec4F_Copy(txyz1+(j+0)*xyzsz, oxyz+(on*3+1)*xyzsz);
			Vec2F_Copy( tst1+(j+0)*stsz,   ost+(on*3+1)* stsz);
			Vec4F_Copy( tnv1+(j+0)*nvsz,   onv+(on*3+1)* nvsz);
			Vec4F_Copy(trgb1+(j+0)*rgbsz, orgb+(on*3+1)*rgbsz);

			Vec4F_Copy(txyz1+(j+1)*xyzsz, oxyz+(on*3+2)*xyzsz);
			Vec2F_Copy( tst1+(j+1)*stsz,   ost+(on*3+2)* stsz);
			Vec4F_Copy( tnv1+(j+1)*nvsz,   onv+(on*3+2)* nvsz);
			Vec4F_Copy(trgb1+(j+1)*rgbsz, orgb+(on*3+2)*rgbsz);
			
			on++;
		}
	}
	
	return(on*3);
	
//	for(i=0; i<nxyz; i++)
//	{
//		Mat4F_TransformVectorT(xyz+i*ixyzsz,
//			ctx->projection_matrix, oxyz+i*oxyzsz);
//	}
}

int RASWGL_TransformClipQuads(
	RASWGL_Context *ctx, int nxyz, int nclip,
	int clipsz, float *clip,
	int xyzsz,		int stsz,
	int nvsz,		int rgbsz,
	float *ixyz,	float *ist,
	float *inv,		float *irgb,
	float *oxyz,	float *ost,
	float *onv,		float *orgb)
{
	float txyz1[64*4], tnv1[64*4], tst1[64*4], trgb1[64*4];
	float txyz2[64*4], tnv2[64*4], tst2[64*4], trgb2[64*4];
	int i, j, k, l, l1, n, on;
	
	n=nxyz/4; on=0;
	for(i=0; i<n; i++)
	{
		for(j=0; j<4; j++)
		{
			Vec4F_Copy(ixyz+(i*4+j)*xyzsz, txyz1+j*xyzsz);
			Vec2F_Copy(ist+(i*4+j)*stsz, tst1+j*stsz);
			Vec4F_Copy(inv+(i*4+j)*nvsz, tnv1+j*nvsz);
			Vec4F_Copy(irgb+(i*4+j)*rgbsz, trgb1+j*rgbsz);
		}
		l=4;

#if 1
		for(j=0; j<nclip; j++)
		{
			l1=Hull_ClipFaceSTNVRGBA(
				clip+j*clipsz,
				xyzsz, stsz, nvsz, rgbsz,
				txyz1, tst1, tnv1, trgb1,
				txyz2, tst2, tnv2, trgb2, l, 3);
			if(l1==-2)
				continue;
			if(l1<3)
				{ l=l1; break; }
			l=l1;
			for(k=0; k<l; k++)
			{
				Vec4F_Copy(txyz2+k*xyzsz, txyz1+k*xyzsz);
				Vec2F_Copy(tst2+k*stsz, tst1+k*stsz);
				Vec4F_Copy(tnv2+k*nvsz, tnv1+k*nvsz);
				Vec4F_Copy(trgb2+k*rgbsz, trgb1+k*rgbsz);
			}
		}
#endif
		
		//clipped away
		if(l<3)
			continue;
		
		if(l==3)
		{
			Vec4F_Copy(txyz1+0*xyzsz, oxyz+(on*4+0)*xyzsz);
			Vec2F_Copy( tst1+0*stsz,   ost+(on*4+0)* stsz);
			Vec4F_Copy( tnv1+0*nvsz,   onv+(on*4+0)* nvsz);
			Vec4F_Copy(trgb1+0*rgbsz, orgb+(on*4+0)*rgbsz);

			Vec4F_Copy(txyz1+1*xyzsz, oxyz+(on*4+1)*xyzsz);
			Vec2F_Copy( tst1+1*stsz,   ost+(on*4+1)* stsz);
			Vec4F_Copy( tnv1+1*nvsz,   onv+(on*4+1)* nvsz);
			Vec4F_Copy(trgb1+1*rgbsz, orgb+(on*4+1)*rgbsz);

			Vec4F_Copy(txyz1+2*xyzsz, oxyz+(on*4+2)*xyzsz);
			Vec2F_Copy( tst1+2*stsz,   ost+(on*4+2)* stsz);
			Vec4F_Copy( tnv1+2*nvsz,   onv+(on*4+2)* nvsz);
			Vec4F_Copy(trgb1+2*rgbsz, orgb+(on*4+2)*rgbsz);
			
			Vec4F_Copy(txyz1+2*xyzsz, oxyz+(on*4+3)*xyzsz);
			Vec2F_Copy( tst1+2*stsz,   ost+(on*4+3)* stsz);
			Vec4F_Copy( tnv1+2*nvsz,   onv+(on*4+3)* nvsz);
			Vec4F_Copy(trgb1+2*rgbsz, orgb+(on*4+3)*rgbsz);
			on++;
			continue;
		}

		if(l==4)
		{
			Vec4F_Copy(txyz1+0*xyzsz, oxyz+(on*4+0)*xyzsz);
			Vec2F_Copy( tst1+0*stsz,   ost+(on*4+0)* stsz);
			Vec4F_Copy( tnv1+0*nvsz,   onv+(on*4+0)* nvsz);
			Vec4F_Copy(trgb1+0*rgbsz, orgb+(on*4+0)*rgbsz);

			Vec4F_Copy(txyz1+1*xyzsz, oxyz+(on*4+1)*xyzsz);
			Vec2F_Copy( tst1+1*stsz,   ost+(on*4+1)* stsz);
			Vec4F_Copy( tnv1+1*nvsz,   onv+(on*4+1)* nvsz);
			Vec4F_Copy(trgb1+1*rgbsz, orgb+(on*4+1)*rgbsz);

			Vec4F_Copy(txyz1+2*xyzsz, oxyz+(on*4+2)*xyzsz);
			Vec2F_Copy( tst1+2*stsz,   ost+(on*4+2)* stsz);
			Vec4F_Copy( tnv1+2*nvsz,   onv+(on*4+2)* nvsz);
			Vec4F_Copy(trgb1+2*rgbsz, orgb+(on*4+2)*rgbsz);

			Vec4F_Copy(txyz1+3*xyzsz, oxyz+(on*4+3)*xyzsz);
			Vec2F_Copy( tst1+3*stsz,   ost+(on*4+3)* stsz);
			Vec4F_Copy( tnv1+3*nvsz,   onv+(on*4+3)* nvsz);
			Vec4F_Copy(trgb1+3*rgbsz, orgb+(on*4+3)*rgbsz);
			on++;
			continue;
		}
		
		for(j=1; j<(l-1); j++)
		{
			Vec4F_Copy(txyz1, oxyz+(on*4+0)*xyzsz);
			Vec2F_Copy( tst1,  ost+(on*4+0)* stsz);
			Vec4F_Copy( tnv1,  onv+(on*4+0)* nvsz);
			Vec4F_Copy(trgb1, orgb+(on*4+0)*rgbsz);
			
			Vec4F_Copy(txyz1+(j+0)*xyzsz, oxyz+(on*4+1)*xyzsz);
			Vec2F_Copy( tst1+(j+0)*stsz,   ost+(on*4+1)* stsz);
			Vec4F_Copy( tnv1+(j+0)*nvsz,   onv+(on*4+1)* nvsz);
			Vec4F_Copy(trgb1+(j+0)*rgbsz, orgb+(on*4+1)*rgbsz);

			Vec4F_Copy(txyz1+(j+1)*xyzsz, oxyz+(on*4+2)*xyzsz);
			Vec2F_Copy( tst1+(j+1)*stsz,   ost+(on*4+2)* stsz);
			Vec4F_Copy( tnv1+(j+1)*nvsz,   onv+(on*4+2)* nvsz);
			Vec4F_Copy(trgb1+(j+1)*rgbsz, orgb+(on*4+2)*rgbsz);

			Vec4F_Copy(txyz1+(j+1)*xyzsz, oxyz+(on*4+3)*xyzsz);
			Vec2F_Copy( tst1+(j+1)*stsz,   ost+(on*4+3)* stsz);
			Vec4F_Copy( tnv1+(j+1)*nvsz,   onv+(on*4+3)* nvsz);
			Vec4F_Copy(trgb1+(j+1)*rgbsz, orgb+(on*4+3)*rgbsz);
			
			on++;
		}
	}
	
	return(on*4);
	
//	for(i=0; i<nxyz; i++)
//	{
//		Mat4F_TransformVectorT(xyz+i*ixyzsz,
//			ctx->projection_matrix, oxyz+i*oxyzsz);
//	}
}
