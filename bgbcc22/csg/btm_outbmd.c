#define BTM_FCC_BMD1	BTMGL_FOURCC('B', 'M', 'D', '1')

int BTM_MeshCheckExpandBuffer(byte **robuf, int sz)
{
	byte *obuf, *obufe, *oct;
	int obsz, oofs, osz1;

	obuf =robuf[0];
	obufe=robuf[1];
	oct  =robuf[2];
	obsz=obufe-obuf;
	
	if(!obuf)
	{
		obsz=4096;
		obuf=btm_malloc(obsz);
		robuf[0]=obuf;
		robuf[1]=obuf+obsz;
		robuf[2]=obuf;
	}
	
	if((oct+sz+1)>=obufe)
	{
		osz1=obsz+(obsz>>1);
		while((oct+sz+1)>=(obuf+osz1))
			osz1=osz1+(osz1>>1);
		oofs=oct-obuf;
		obuf=btm_realloc(obuf, osz1);
		obsz=osz1;
		obufe=obuf+obsz;
		oct=obuf+oofs;

		robuf[0]=obuf;
		robuf[1]=obufe;
		robuf[2]=oct;
	}
	return(0);
}

int BTM_MeshBufferGetOfs(byte **robuf)
{
	return(robuf[2]-robuf[0]);
}

int BTM_MeshBufferAlign(byte **robuf, int al)
{
	int i;
	i=robuf[2]-robuf[0];
	if(!(i&(al-1)))
		return(0);
	BTM_MeshCheckExpandBuffer(robuf, al);
	robuf[2]+=(al-(i&(al-1)));
	return(0);
}

int BTM_MeshBufferAdvance(byte **robuf, int sz)
{
	int i;
	BTM_MeshCheckExpandBuffer(robuf, sz);
	robuf[2]+=sz;
	return(0);
}

int BTM_MeshBufferSetOfsU32(byte **robuf, int ofs, u32 val)
{
	byte *ct;
	
	ct=robuf[0]+ofs;
	ct[0]=(val>> 0)&255;
	ct[1]=(val>> 8)&255;
	ct[2]=(val>>16)&255;
	ct[3]=(val>>24)&255;
	return(0);
}

int BTM_MeshBufferSetOfsU16(byte **robuf, int ofs, u16 val)
{
	byte *ct;
	
	ct=robuf[0]+ofs;
	ct[0]=(val>> 0)&255;
	ct[1]=(val>> 8)&255;
	return(0);
}

int BTM_MeshBufferSetOfsU8(byte **robuf, int ofs, u16 val)
{
	byte *ct;
	
	ct=robuf[0]+ofs;
	ct[0]=(val>> 0)&255;
	return(0);
}

int BTM_MeshBufferSetOfsU64(byte **robuf, int ofs, u64 val)
{
	byte *ct;
	
	ct=robuf[0]+ofs;
	ct[0]=(val>> 0)&255;
	ct[1]=(val>> 8)&255;
	ct[2]=(val>>16)&255;
	ct[3]=(val>>24)&255;
	ct[4]=(val>>32)&255;
	ct[5]=(val>>40)&255;
	ct[6]=(val>>48)&255;
	ct[7]=(val>>56)&255;
	return(0);
}

int BTM_MeshBufGetIndexString(byte **robuf, char *str, int *stra, int *rnstr)
{
	char *s0;
	int nstr;
	int i, j, k, l;

	nstr=*rnstr;
	for(i=0; i<nstr; i++)
	{
		s0=(char *)(robuf[0]+stra[i]);
		if(!strcmp(s0, str))
			return(i);
	}
	
	l=strlen(str)+1;
	BTM_MeshCheckExpandBuffer(robuf, l);
	i=nstr++;
	stra[i]=robuf[2]-robuf[0];
	memcpy(robuf[2], str, l);
	robuf[2]+=l;
	*rnstr=nstr;
	return(i);
}

int BTM_MeshBufferSetVertex128B(byte **robuf, int ofs,
	float *xyz, float *st, float *nv, u32 rgb, int bone)
{
	int clr;
	
	clr=BTM_GetBlockLightForRgb(rgb);

	BTM_MeshBufferSetOfsU16(robuf, ofs+0x0,
		BTM_ConvFloatToFP16A(xyz[0]));
	BTM_MeshBufferSetOfsU16(robuf, ofs+0x2,
		BTM_ConvFloatToFP16A(xyz[1]));
	BTM_MeshBufferSetOfsU16(robuf, ofs+0x4,
		BTM_ConvFloatToFP16A(xyz[2]));
//	BTM_MeshBufferSetOfsU16(robuf, ofs+0x6, 0xFD0F);
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x6, clr);
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x7, 0xFD);
	BTM_MeshBufferSetOfsU16(robuf, ofs+0x8, st[0]*4096);
	BTM_MeshBufferSetOfsU16(robuf, ofs+0xA, st[1]*4096);

	BTM_MeshBufferSetOfsU8(robuf, ofs+0xC,
		BTM_ConvFloatToFP8A(nv[0]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0xD,
		BTM_ConvFloatToFP8A(nv[1]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0xE,
		BTM_ConvFloatToFP8A(nv[2]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0xF,
		bone);
	return(0);
}

int BTM_MeshBufferSetVertex64A(byte **robuf, int ofs,
	float *xyz, float *st, float *nv, u32 rgb, int bone)
{
	float vs, vt;
	u32 pv;
	int sta;

	sta=0;
	vs=st[0];
	vt=st[1];

	if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
	{
		sta=1; vs*=0.25; vt*=0.25;
		if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
			{ sta=2; vs*=0.25; vt*=0.25; }
		if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
			{ sta=3; vs*=0.25; vt*=0.25; }
	}

	pv=BTM_MeshEncodePackSe32(xyz);
	BTM_MeshBufferSetOfsU32(robuf, ofs+0x0, pv);

	BTM_MeshBufferSetOfsU32(robuf, ofs+0x4,
		((((int)((vs+0.5)*1024))&2047)<< 0) |
		((((int)((vt+0.5)*1024))&2047)<<11) |
		(sta<<22) | (bone<<24));

//	BTM_MeshBufferSetOfsU32(robuf, ofs+0x4,
//		((((int)((st[0]+0.5)*2048))&4095)<< 0) |
//		((((int)((st[1]+0.5)*2048))&4095)<<12) |
//		(bone<<24));
	return(0);
}

int BTM_MeshBufferSetVertex64B(byte **robuf, int ofs,
	float *xyz, float *st, float *nv, u32 rgb, int bone)
{
	u32 pv;

	if(!bone)
	{
		pv=0;
	}

	pv=BTM_MeshEncodePackSe32(xyz);
	BTM_MeshBufferSetOfsU32(robuf, ofs+0x0, pv);

	BTM_MeshBufferSetOfsU8(robuf, ofs+0x4,
		BTM_ConvFloatToFP8A(nv[0]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x5,
		BTM_ConvFloatToFP8A(nv[1]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x6,
		BTM_ConvFloatToFP8A(nv[2]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x7,
		bone);

	return(0);
}

int BTM_MeshBufferSetVertex96A(byte **robuf, int ofs,
	float *xyz, float *st, float *nv, u32 rgb, int bone)
{
	float vs, vt;
	int tg1, tg2, ivs, ivt;
	int sta;
	u32 pv;

	sta=0;
	vs=st[0];
	vt=st[1];

	tg1=0xF8;
	tg2=BTM_GetBlockLightForRgb(rgb);

//	if((st[0]<(-0.5)) || (st[1]<(-0.5)) || (st[0]>1.5) || (st[0]>1.5))
	if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
	{
		tg1=0xF9;
		sta=1; vs*=0.25; vt*=0.25;
		if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
			{ sta=2; vs*=0.25; vt*=0.25; }
		if((vs<(-0.5)) || (vt<(-0.5)) || (vs>1.5) || (vt>1.5))
			{ sta=3; vs*=0.25; vt*=0.25; }
	}

	if(bone)
	{
		tg1=0xFA; tg2=bone;
		if(sta)
			tg1=0xFB;
	}

	pv=BTM_MeshEncodePackSe32(xyz);
	BTM_MeshBufferSetOfsU32(robuf, ofs+0x0, pv);

	if(sta)
//	if(1)
	{
		ivs=((int)((vs+0.5)*1024));
		ivt=((int)((vt+0.5)*1024));
		if(ivs<0)		ivs=0;
		if(ivt<0)		ivt=0;
		if(ivs>2047)	ivs=2047;
		if(ivt>2047)	ivt=2047;
		BTM_MeshBufferSetOfsU32(robuf, ofs+0x4,
			((ivs&2047)<< 0) |
			((ivt&2047)<<11) |
			(sta<<22) | (tg1<<24));
	}else
	{
		BTM_MeshBufferSetOfsU32(robuf, ofs+0x4,
			((((int)((vs+0.5)*2048))&4095)<< 0) |
			((((int)((vt+0.5)*2048))&4095)<<12) |
			(tg1<<24));
	}

	BTM_MeshBufferSetOfsU8(robuf, ofs+0x8,
		BTM_ConvFloatToFP8A(nv[0]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0x9,
		BTM_ConvFloatToFP8A(nv[1]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0xA,
		BTM_ConvFloatToFP8A(nv[2]));
	BTM_MeshBufferSetOfsU8(robuf, ofs+0xB,
		tg2);

	return(0);
}

int BTM_MeshBufferSetTriangle64(byte **robuf, int ofs,
	int *vidx)
{
	u64 v0;
	
	v0= 2 |
		(((u64)vidx[0])<< 2) |
		(((u64)vidx[1])<<22) |
		(((u64)vidx[2])<<42) ;
	BTM_MeshBufferSetOfsU64(robuf, ofs, v0);
	return(0);
}

int BTM_MeshBufferSetTriangle32(byte **robuf, int ofs,
	int *vidx)
{
	u64 v0;
	
	v0= 0 |
		(((u64)vidx[0])<< 2) |
		(((u64)vidx[1])<<12) |
		(((u64)vidx[2])<<22) ;
	BTM_MeshBufferSetOfsU32(robuf, ofs, v0);
	return(0);
}

int BTM_ExportMeshListBmdBuf(BTM_SolidMesh *mesh,
	byte **robuf, int *robsz)
{
	int stra[512];
	byte *obufa[4];
	BTM_SolidMesh *mcur;
	BTM_SolidSkel *skel;
	BTM_BtModelAnim *acur;
	byte *obuf, *oct, *tcabuf, *tcact;
	char *s0, *s1;
	u32 pv, pv1, pv2;
	int obsz, obmsz, nstrs, nmesh;
	int meshofs, meshsz, lodofs;
	int skelofs, skelsz, animofs, animsz;
	int vtxofs, trisofs, vtxsz, trissz, mix, mfl, szvtx;
	int nanim;
	int i, j, k, l;

//	BGBCC_LoadConvResource_SetupPal();

	obuf=*robuf;
	obmsz=*robsz;
	
	if(!obuf)
	{
		obmsz=65536;
		obuf=btm_malloc(obmsz);
		memset(obuf, 0, obmsz);
	}
	
	oct=obuf+0x40;
	
	obufa[0]=obuf;
	obufa[1]=obuf+obmsz;
	obufa[2]=oct;
	
	nstrs=0;	nmesh=0;
	mcur=mesh;
	while(mcur)
	{
//		s0=mcur->name;
//		BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
		
		s0=mcur->usetex[0];
		if(s0)
			BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
		
		nmesh++;
		mcur=mcur->next;
	}

	skel=mesh->skel;
	if(skel)
	{
		for(i=0; i<skel->n_bones; i++)
		{
			s0=skel->bone_name[i];
			if(s0)
				BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
		}
		
		nanim=0;
		acur=skel->anim;
		while(acur)
		{
			s0=acur->name;
			if(s0)
				BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
			nanim++;
			acur=acur->next;
		}
	}

	
//	BTM_MeshBufferAlign(obufa, 8);
	BTM_MeshBufferAlign(obufa, 4);

	lodofs=BTM_MeshBufferGetOfs(obufa);
	BTM_MeshBufferAdvance(obufa, 4);

	BTM_MeshBufferAlign(obufa, 16);
	
	meshofs=BTM_MeshBufferGetOfs(obufa);
	meshsz=nmesh*32;
	BTM_MeshBufferAdvance(obufa, meshsz);
	
	BTM_MeshBufferSetOfsU32(obufa, 0x00, BTM_FCC_BMD1);
	BTM_MeshBufferSetOfsU32(obufa, 0x04, 0);
	BTM_MeshBufferSetOfsU32(obufa, 0x08, 0);
	BTM_MeshBufferSetOfsU32(obufa, 0x0C, 0);
	
	BTM_MeshBufferSetOfsU32(obufa, 0x10, meshofs);
	BTM_MeshBufferSetOfsU32(obufa, 0x14, meshsz);
	BTM_MeshBufferSetOfsU32(obufa, 0x18, lodofs);
	BTM_MeshBufferSetOfsU32(obufa, 0x1C, 4);

	BTM_MeshBufferSetOfsU32(obufa, 0x20, 0);
	BTM_MeshBufferSetOfsU32(obufa, 0x24, 0);
	BTM_MeshBufferSetOfsU32(obufa, 0x28, 0);
	BTM_MeshBufferSetOfsU32(obufa, 0x2C, 0);

	BTM_MeshBufferSetOfsU8(obufa, lodofs+0, 0);
	BTM_MeshBufferSetOfsU8(obufa, lodofs+1, nmesh);
	BTM_MeshBufferSetOfsU8(obufa, lodofs+2, 0x01);

	mcur=mesh; mix=0;
	while(mcur)
	{
		szvtx=12;
//		szvtx=16;

		if(mcur->usetex[0][0]=='#')
			szvtx=8;

		mfl=0x0000000A;
		vtxsz=mcur->n_vtx*16;
		trissz=mcur->n_tris*8;
		if(mcur->n_vtx<1024)
		{
			trissz=mcur->n_tris*4;
			mfl&=~0xC;
		}
		if(szvtx==8)
		{
			mfl&=~0x3;
			vtxsz=mcur->n_vtx*8;
		}
		if(szvtx==12)
		{
			mfl&=~0x3;
			mfl|=1;
			vtxsz=mcur->n_vtx*12;
		}

		BTM_MeshBufferAlign(obufa, 16);
		vtxofs=BTM_MeshBufferGetOfs(obufa);
		BTM_MeshBufferAdvance(obufa, vtxsz+8);

		BTM_MeshBufferAlign(obufa, 16);
		trisofs=BTM_MeshBufferGetOfs(obufa);
		BTM_MeshBufferAdvance(obufa, trissz+8);

		BTM_MeshBufferAlign(obufa, 16);

		s0=mcur->usetex[0];
		i=BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);

		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x00, vtxofs);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x04, trisofs);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x08, stra[i]);
		BTM_MeshBufferSetOfsU16(obufa, meshofs+mix*32+0x0C, mcur->n_vtx);
		BTM_MeshBufferSetOfsU16(obufa, meshofs+mix*32+0x0E, mcur->n_tris);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x10, vtxsz);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x14, trissz);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x18, mfl);
		BTM_MeshBufferSetOfsU32(obufa, meshofs+mix*32+0x1C, 0);

		for(i=0; i<mcur->n_vtx; i++)
		{
			if(szvtx==16)
			{
				BTM_MeshBufferSetVertex128B(obufa, vtxofs+i*16,
					mcur->v_xyz+i*3,
					mcur->v_st+i*2,
					mcur->v_nv+i*3,
					mcur->v_cl[i], mcur->v_bn[i]);
			}else
				if(szvtx==12)
			{
				BTM_MeshBufferSetVertex96A(obufa, vtxofs+i*12,
					mcur->v_xyz+i*3,
					mcur->v_st+i*2,
					mcur->v_nv+i*3,
					mcur->v_cl[i], mcur->v_bn[i]);
			}else
				if(szvtx==8)
			{
				if(mcur->usetex[0][0]=='#')
				{
					BTM_MeshBufferSetVertex64B(obufa, vtxofs+i*8,
						mcur->v_xyz+i*3,
						mcur->v_st+i*2,
						mcur->v_nv+i*3,
						mcur->v_cl[i], mcur->v_bn[i]);
				}else
				{
					BTM_MeshBufferSetVertex64A(obufa, vtxofs+i*8,
						mcur->v_xyz+i*3,
						mcur->v_st+i*2,
						mcur->v_nv+i*3,
						mcur->v_cl[i], mcur->v_bn[i]);
				}
			}
		}

		for(i=0; i<mcur->n_tris; i++)
		{
			if(mcur->n_vtx<1024)
			{
				BTM_MeshBufferSetTriangle32(obufa, trisofs+i*4,
					mcur->t_vidx+i*3);
			}else
			{
				BTM_MeshBufferSetTriangle64(obufa, trisofs+i*8,
					mcur->t_vidx+i*3);
			}
		}

		mix++;
		mcur=mcur->next;
	}

	if(!skel)
	{
		*robuf=obufa[0];
		*robsz=obufa[2]-obufa[0];
		return(0);
	}

	skelsz=skel->n_bones*16;
	animsz=nanim*16;

	BTM_MeshBufferAlign(obufa, 16);
	skelofs=BTM_MeshBufferGetOfs(obufa);
	BTM_MeshBufferAdvance(obufa, skelsz);

	BTM_MeshBufferAlign(obufa, 16);
	animofs=BTM_MeshBufferGetOfs(obufa);
	BTM_MeshBufferAdvance(obufa, animsz);

	BTM_MeshBufferSetOfsU32(obufa, 0x20, skelofs);
	BTM_MeshBufferSetOfsU32(obufa, 0x24, skelsz);
	BTM_MeshBufferSetOfsU32(obufa, 0x28, animofs);
	BTM_MeshBufferSetOfsU32(obufa, 0x2C, animsz);

	for(i=0; i<skel->n_bones; i++)
	{
		s0=skel->bone_name[i]; j=0;
		if(s0)
			j=BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
		BTM_MeshBufferSetOfsU32(obufa, skelofs+i*16+0x08, stra[j]);
		BTM_MeshBufferSetOfsU8(obufa, skelofs+i*16+0x0C,
			skel->bone_parent[i]);

		pv=BTM_MeshEncodePackSe32(skel->bone_relorg+(i*3));
		BTM_MeshBufferSetOfsU32(obufa, skelofs+i*16+0x00, pv);
	}

	tcabuf=btm_malloc(65536);
	tcact=tcabuf;

	acur=skel->anim; i=0;
	while(acur)
	{
		s0=acur->name; j=0;
		if(s0)
			j=BTM_MeshBufGetIndexString(obufa, s0, stra, &nstrs);
		BTM_MeshBufferSetOfsU32(obufa, animofs+i*16+0x00, stra[j]);
		BTM_MeshBufferSetOfsU8(obufa, animofs+i*16+0x04, acur->n_frames);
		BTM_MeshBufferSetOfsU8(obufa, animofs+i*16+0x05, 10*4);

		tcact=tcabuf;
		for(j=0; j<acur->n_bones; j++)
		{
			l=0;
			for(k=0; k<acur->n_frames; k++)
			{
				pv =acur->frm_rot[k*acur->n_bones+j];
				pv1=acur->frm_org[k*acur->n_bones+j];
				pv2=acur->frm_scl[k*acur->n_bones+j];

				mfl=l;
				if(pv !=0xFFFFFFFFU)	mfl|=0x80;
				if(pv1!=0xFFFFFFFFU)	mfl|=0x40;
				if(pv2!=0xFFFFFFFFU)	mfl|=0x20;

				if(mfl&0xE0)
				{
					*tcact++=mfl;
					if(mfl&0x80)
					{
						*tcact++=pv;		*tcact++=pv>>8;
						*tcact++=pv>>16;	*tcact++=pv>>24;
					}
					if(mfl&0x40)
					{
						*tcact++=pv1;		*tcact++=pv1>>8;
						*tcact++=pv1>>16;	*tcact++=pv1>>24;
					}
					if(mfl&0x20)
					{
						*tcact++=pv2;		*tcact++=pv2>>8;
						*tcact++=pv2>>16;	*tcact++=pv2>>24;
					}
					l=0;
					continue;
				}
				
				if(l>=0x20)
				{
					*tcact++=(l-1);
					l=0;
					continue;
				}
				l++;
			}
			*tcact++=0;
		}

		vtxsz=tcact-tcabuf;

		BTM_MeshBufferAlign(obufa, 16);
		vtxofs=BTM_MeshBufferGetOfs(obufa);
		BTM_MeshBufferAdvance(obufa, vtxsz);

		memcpy(obufa[0]+vtxofs, tcabuf, vtxsz);

		BTM_MeshBufferSetOfsU32(obufa, animofs+i*16+0x08, vtxofs);
		BTM_MeshBufferSetOfsU32(obufa, animofs+i*16+0x0C, vtxsz);

		acur=acur->next; i++;
	}

	*robuf=obufa[0];
	*robsz=obufa[2]-obufa[0];
	return(0);
}

int BTM_ExportMeshListBmd(char *fname, BTM_SolidMesh *mesh)
{
	byte *obuf;
	int obsz;
	
	obuf=NULL; obsz=0;
	BTM_ExportMeshListBmdBuf(mesh, &obuf, &obsz);
	BTM_StoreFile(fname, obuf, obsz);
	return(0);
}

