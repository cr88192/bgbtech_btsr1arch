/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// byte *wrl_zmap;
// byte *wrl_nmap;

BCCX_Node *BTM_LookupMenuNode(char *name, char *subname);

int btm_tgrand(BTM_World *wrl)
{
	u16 v;
	u64 seed1;
	
	seed1=(wrl->tg_curseed*0x0000FFD302A2F2D1ULL)+4093;
	wrl->tg_curseed=seed1;
	v=seed1>>48;
	return(v);
}

int BTM_SetupWorldSeed(BTM_World *wrl, u64 seed)
{
	int i, j, k;

	wrl->tg_baseseed=seed;
	wrl->tg_curseed=seed;

	for(i=0; i<4096; i++)
		btm_tgrand(wrl);

	for(j=0; j<16; j++)
	{
		wrl->tg_nmap[j]=malloc(256);
		for(i=0; i<256; i++)
		{
			wrl->tg_nmap[j][i]=btm_tgrand(wrl);
//			wrl->tg_nmap[j][i]=rand();
		}
	}
	return(0);
}

int BTM_SetupLocalSeedXY(BTM_World *wrl, int cx, int cy)
{
	u64 seed1;
	
	seed1=wrl->tg_curseed;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+cx;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+cy;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+cx;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+cy;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+0;
	seed1=(seed1*0x0000FFD302A2F2D1ULL)+0;
	wrl->tg_curseed=seed1;
	return(0);
}

float BTM_NoisePt8F(BTM_World *wrl,
	int ix, int iy, int mskx, int msky, int mn)
{
	byte *nmap;
	int ix1, iy1, p;
	float f;

	ix&=mskx;
	iy&=msky;
	nmap=wrl->tg_nmap[mn];

	ix1=(2*ix-iy);
	iy1=(2*iy-ix);
	p=iy1*15+ix1;
	f=nmap[p&255]*(1.0/128)-1.0;
	return(f);
}

float BTM_NoisePtXyz8F(BTM_World *wrl,
	int ix, int iy, int iz, int mskx, int msky, int mskz, int mn)
{
	byte *nmap;
	int ix1, iy1, iz1, p;
	float f;

	ix&=mskx;
	iy&=msky;
	iz&=mskz;
	nmap=wrl->tg_nmap[mn];

	ix1=(2*ix-iy);
	iy1=(2*iy-ix);
	iz1=(2*iz-(2*iy1-ix1));
	p=iz1*7+iy1*3+ix1;
	p=(p*31)>>5;
	f=nmap[p&255]*(1.0/128)-1.0;
	return(f);
}

float BTM_Noise8F(BTM_World *wrl,
	float x, float y, int mskx, int msky, int mn)
{
	float f0, f1, f2, f3;
	float f4, f5, f6;
	float fx, fy;
	int ix, iy;
	int p0, p1, p2, p3;

	ix=x;		iy=y;
	fx=x-ix;	fy=y-iy;

	f0=BTM_NoisePt8F(wrl, ix+0, iy+0, mskx, msky, mn);
	f1=BTM_NoisePt8F(wrl, ix+1, iy+0, mskx, msky, mn);
	f2=BTM_NoisePt8F(wrl, ix+0, iy+1, mskx, msky, mn);
	f3=BTM_NoisePt8F(wrl, ix+1, iy+1, mskx, msky, mn);
	f4=(1.0-fx)*f0+(fx*f1);
	f5=(1.0-fx)*f2+(fx*f3);
	f6=(1.0-fy)*f4+(fy*f5);
	return(f6);
}

float BTM_NoiseXyz8F(BTM_World *wrl,
	float x, float y, float z, int mskx, int msky, int mskz, int mn)
{
	float f0, f1, f2, f3;
	float f4, f5, f6, f7, f8;
	float fx, fy, fz;
	int ix, iy, iz;
	int p0, p1, p2, p3;

	ix=x;		iy=y;		iz=z;
	fx=x-ix;	fy=y-iy;	fz=z-iz;

	f0=BTM_NoisePtXyz8F(wrl, ix+0, iy+0, iz+0, mskx, msky, mskz, mn);
	f1=BTM_NoisePtXyz8F(wrl, ix+1, iy+0, iz+0, mskx, msky, mskz, mn);
	f2=BTM_NoisePtXyz8F(wrl, ix+0, iy+1, iz+0, mskx, msky, mskz, mn);
	f3=BTM_NoisePtXyz8F(wrl, ix+1, iy+1, iz+0, mskx, msky, mskz, mn);
	f4=(1.0-fx)*f0+(fx*f1);
	f5=(1.0-fx)*f2+(fx*f3);
	f6=(1.0-fy)*f4+(fy*f5);

	f0=BTM_NoisePtXyz8F(wrl, ix+0, iy+0, iz+1, mskx, msky, mskz, mn);
	f1=BTM_NoisePtXyz8F(wrl, ix+1, iy+0, iz+1, mskx, msky, mskz, mn);
	f2=BTM_NoisePtXyz8F(wrl, ix+0, iy+1, iz+1, mskx, msky, mskz, mn);
	f3=BTM_NoisePtXyz8F(wrl, ix+1, iy+1, iz+1, mskx, msky, mskz, mn);
	f4=(1.0-fx)*f0+(fx*f1);
	f5=(1.0-fx)*f2+(fx*f3);
	f7=(1.0-fy)*f4+(fy*f5);

	f8=(1.0-fz)*f6+(fz*f7);
	return(f8);
}

int btm_abs(int x)
{
	return((x<0)?(-x):x);
}

int BTM_GenTree(BTM_World *wrl, int cx, int cy, int cz)
{
	int i, j, k, d, h;

	h=3+(btm_tgrand(wrl)&3);

	for(i=0; i<5; i++)
		for(j=0; j<5; j++)
			for(k=0; k<5; k++)
	{
		d=btm_abs(i-2)+btm_abs(j-2)+btm_abs(k-2);
		if(d>4)
			continue;
		BTM_SetWorldBlockNlXYZ(wrl,
			cx+(i-2), cy+(j-2), cz+(h-2)+k,
			BTM_BLKTY_LEAVES);
	}

	for(k=0; k<4; k++)
	{
		BTM_SetWorldBlockNlXYZ(wrl, cx, cy, cz+k,
			BTM_BLKTY_LOG);
	}
	
	return(0);
}

#define BTM_TGEN_BASEHEIGHT		64

float btm_abspow(float base, float exp)
{
	if(base<0)
		return(-pow(-base, exp));
	return(pow(base, exp));
}

int BTM_GenerateBaseHeightXY(BTM_World *wrl, int cx, int cy)
{
	int i, j, k, l;
	float f0, f1;
	
	i=cx;
	j=cy;

//	l=	BTM_Noise8F(i/32.0, j/32.0, 7, 7)*4 +
//		BTM_Noise8F(i/8.0, j/8.0, 31, 31)*2 +
//		BTM_Noise8F(i/2.0, j/2.0, 127, 127)*1 +
//		26;

	f0=
		BTM_Noise8F(wrl, i/128.0, j/128.0,   511,   511, 0)*12 +
		BTM_Noise8F(wrl, i/ 32.0, j/ 32.0,  2047,  2047, 0)* 6 +
		BTM_Noise8F(wrl, i/  8.0, j/  8.0,  8191,  8191, 0)* 3 +
		BTM_Noise8F(wrl, i/  2.0, j/  2.0, 32767, 32767, 0)* 1 ;

	f1=
		BTM_Noise8F(wrl, i/128.0, j/128.0,   511,   511, 1)* 0.75 +
		BTM_Noise8F(wrl, i/ 32.0, j/ 32.0,  2047,  2047, 1)* 0.25 +
		1.25;

//	f0=btm_abspow(f0, 1.5);
//	f0=btm_abspow(f0, 1.25);
	f0=btm_abspow(f0, f1);

	if(f0>16)
	{
		f0=sqrt(f0-16)+16;
	}

	if(f0<(-16))
	{
		f0=(-16)-sqrt((-16)-f0);
	}


	l=	f0 + (BTM_TGEN_BASEHEIGHT+4);
	
	if(l<24)
		l=24;
	if(l>112)
		l=112;
	
	return(l);
}

int BTM_GenerateBaseXY(BTM_World *wrl, int cx, int cy)
{
	float f0, f1;
	u32 blk;
	int i, j, k, l, lc, ld;
	
	i=cx;
	j=cy;

	l=BTM_GenerateBaseHeightXY(wrl, cx, cy);

	for(k=0; k<128; k++)
		{ BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_AIR2); }

//	for(k=l; k<24; k++)
//	for(k=l; k<48; k++)
	for(k=l; k<BTM_TGEN_BASEHEIGHT; k++)
		{ BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_WATER); }

	for(k=l-4; k<l; k++)
	{
//		if(l>(48+1))
		if(l>(BTM_TGEN_BASEHEIGHT+1))
			BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_DIRT);
		else
			BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_SAND);
	}

	for(k=0; k<l-3; k++)
		{ BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_STONE); }

	if(l>(BTM_TGEN_BASEHEIGHT+1))
		{ BTM_SetWorldBlockNlXYZ(wrl, i, j, l-1, BTM_BLKTY_GRASS); }
	else
		{ BTM_SetWorldBlockNlXYZ(wrl, i, j, l-1, BTM_BLKTY_SAND); }

	for(k=0; k<l; k++)
	{
	
		if(k<(l-3))
		{
			lc=	BTM_NoiseXyz8F(wrl,
					i/8.0, j/8.0, k/8.0, 8191, 8191, 15, 1)*16 +
				BTM_NoiseXyz8F(wrl,
					i/2.0, j/2.0, k/2.0, 32767, 32767, 63, 1)* 4 ;
			ld=	BTM_NoiseXyz8F(wrl,
					i/8.0, j/8.0, k/8.0, 8191, 8191, 15, 2)*16 +
				BTM_NoiseXyz8F(wrl,
					i/2.0, j/2.0, k/2.0, 32767, 32767, 63, 2)* 4 ;

			if(lc>=10)
			{
				if(ld>=10)
				{
					BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_STONE3);
				}else if(ld>=4)
				{
					BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_STONE2);
				}else if((ld<-10))
				{
					BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_DIRT);
				}
			}
		}

		f0=	
			BTM_Noise8F(wrl, i/32.0, j/32.0, 2047, 2047, 2)*10 +
			BTM_NoiseXyz8F(wrl, i/8.0, j/8.0, k/8.0, 8191, 8191, 255, 0)*7 +
			BTM_NoiseXyz8F(wrl, i/2.0, j/2.0, k/2.0, 32767, 32767, 255, 0)*3 ;

//		if(lc>=14)
//		if(lc>=8)
//		if(lc>=(10-((l-64)>>3)))
//		if(lc>=(9-((l-64)>>4)))
		if(f0>=(11-((l-64)>>4)))
		{
			BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_AIR1);
		}
	}

	BTM_SetWorldBlockNlXYZ(wrl, i, j, 0, BTM_BLKTY_HARDSTONE);
	
	if(btm_tgrand(wrl)&1)
	{
		BTM_SetWorldBlockNlXYZ(wrl, i, j, 1, BTM_BLKTY_HARDSTONE);
	}


#if 0
//		if(!(rand()&63))
//	if(!(rand()&255) && (l>25))
//	if(!(rand()&255) && (l>(48+1)))
//	{
//		BTM_GenTree(wrl, i, j, l);
//	}

	for(k=127; k>0; k--)
	{
		blk=BTM_GetWorldBlockXYZ(wrl, i, j, k);
		if(blk!=BTM_BLKTY_AIR2)
			break;
		
//		BTM_SetWorldBlockNlXYZ(wrl, i, j, k, BTM_BLKTY_AIR2);
	}
#endif

	return(0);
}

int BTM_GenerateBaseRegion(BTM_World *wrl, BTM_Region *rgn)
{
	u64 rpos, rcix;
	u32 blk;
	int cx, cy, x, y, z, cix;
	int l;
	
	blk=BTM_GetRegionBlockCix(wrl, rgn, 0);

//	if(rgn->vox[0])
//	if(blk)
	if(blk&255)
		return(0);
	
	rgn->dirty|=4;
	
	rpos=BTM_ConvRixToBlkPos(rgn->rgnix);
	cx=(rpos>> 0)&65535;
	cy=(rpos>>16)&65535;
	
	BTM_SetupLocalSeedXY(wrl, cx, cy);
	
	for(y=0; y<128; y++)
		for(x=0; x<128; x++)
	{
		BTM_GenerateBaseXY(wrl, cx+x, cy+y);
	}

	for(y=0; y<128; y++)
		for(x=0; x<128; x++)
	{
//		BTM_GenerateBaseXY(wrl, cx+x, cy+y);

		if(!(btm_tgrand(wrl)&255) &&
			(x>2) && (x<126) &&
			(y>2) && (y<126))
		{
			l=BTM_GenerateBaseHeightXY(wrl, cx+x, cy+y);
			if(l>(BTM_TGEN_BASEHEIGHT+1))
				BTM_GenTree(wrl, cx+x, cy+y, l);
		}
	}

	for(y=0; y<128; y++)
		for(x=0; x<128; x++)
	{
		for(z=127; z>8; z--)
		{
			blk=BTM_GetWorldBlockXYZ(wrl, cx+x, cy+y, z);
			if(blk!=BTM_BLKTY_AIR2)
				break;
//			blk|=0x0F<<18;
			blk|=0x0F<<20;
			BTM_SetWorldBlockNlXYZ(wrl, cx+x, cy+y, z, blk);
		}

		for(z=1; z<16; z++)
		{
			blk=BTM_GetWorldBlockXYZ(wrl, cx+x, cy+y, z);
			if(blk!=BTM_BLKTY_AIR1)
				continue;
			blk=BTM_BLKTY_LAVA;
			BTM_SetWorldBlockNlXYZ(wrl, cx+x, cy+y, z, blk);
		}
	}

	for(z=0; z<128; z++)
		for(y=0; y<128; y++)
			for(x=0; x<128; x++)
	{
		cix=(z<<14)|(y<<7)|x;
		rcix=(((u64)rgn->rgnix)<<21)|cix;
		BTM_UpdateWorldBlockOccCix(wrl, rcix);
	}

	for(z=12; z<112; z++)
		for(y=8; y<120; y++)
			for(x=8; x<120; x++)
	{
		BTM_UpdateBlockLightForRCix(wrl, rcix);
	}

	return(0);
}

int BTM_InstFill(BTM_World *wrl,
	int mcx, int mcy, int mcz,
	int ncx, int ncy, int ncz,
	u32 blk, u32 rpblk)
{
	u32 tblk;
	int x, y, z;

	for(z=mcz; z<=ncz; z++)
		for(y=mcy; y<=ncy; y++)
			for(x=mcx; x<=ncx; x++)
	{
		if(rpblk)
		{
			tblk=BTM_GetWorldBlockXYZ(wrl, x, y, z);
			if((tblk&255)==(rpblk&255))
				BTM_SetWorldBlockNlXYZ(wrl, x, y, z, blk);
		}else
		{
			BTM_SetWorldBlockNlXYZ(wrl, x, y, z, blk);
		}
	}

	return(0);
}

int BTM_InstRelight(BTM_World *wrl,
	int mcx, int mcy, int mcz,
	int ncx, int ncy, int ncz)
{
	u64 rcix;
	int x, y, z;

	for(z=mcz; z<=ncz; z++)
		for(y=mcy; y<=ncy; y++)
			for(x=mcx; x<=ncx; x++)
	{
		rcix=BTM_BlockCoordsToRcix(x, y, z);
		BTM_UpdateWorldBlockOccCix2(wrl, rcix);
	}

	for(z=mcz; z<=ncz; z++)
		for(y=mcy; y<=ncy; y++)
			for(x=mcx; x<=ncx; x++)
	{
//		BTM_SetWorldBlockNlXYZ(wrl, +x, y, z, blk);
		rcix=BTM_BlockCoordsToRcix(x, y, z);
//		BTM_UpdateWorldBlockOccCix2(wrl, rcix);
		BTM_UpdateBlockLightForRCix(wrl, rcix);
	}

	return(0);
}

s64 BTM_InstGetVarInt(BTM_World *wrl, char *name)
{
	BCCX_AttrVal *av;
	int ix;
	int i, j, k;

	ix=BCCX_StringToStridx(name);
	i=wrl->tgen_varstk_pos-1;
	while(i>=0)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
			if((j>>12)==BCCX_IVTY_INT)
				return(av->i);
			if((j>>12)==BCCX_IVTY_REAL)
				return(av->f);
			if((j>>12)==BCCX_IVTY_STRING)
				return(bccx_atoll(av->s));
			return(0);
		}
		i--;
	}

	i=wrl->tgen_vargbl_cnt-1;
	while(i>=0)
	{
		j=wrl->tgen_vargbl_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
			if((j>>12)==BCCX_IVTY_INT)
				return(av->i);
			if((j>>12)==BCCX_IVTY_REAL)
				return(av->f);
			if((j>>12)==BCCX_IVTY_STRING)
				return(bccx_atoll(av->s));
			return(0);
		}
		i--;
	}
	
	return(0);
}

double BTM_InstGetVarFloat(BTM_World *wrl, char *name)
{
	BCCX_AttrVal *av;
	int ix;
	int i, j, k;

	ix=BCCX_StringToStridx(name);

	i=wrl->tgen_varstk_pos-1;
	while(i>=0)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
			if((j>>12)==BCCX_IVTY_INT)
				return(av->i);
			if((j>>12)==BCCX_IVTY_REAL)
				return(av->f);
			if((j>>12)==BCCX_IVTY_STRING)
				return(bccx_atof(av->s));
			return(0);
		}
		i--;
	}

	i=wrl->tgen_vargbl_cnt-1;
	while(i>=0)
	{
		j=wrl->tgen_vargbl_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
			if((j>>12)==BCCX_IVTY_INT)
				return(av->i);
			if((j>>12)==BCCX_IVTY_REAL)
				return(av->f);
			if((j>>12)==BCCX_IVTY_STRING)
				return(bccx_atof(av->s));
			return(0);
		}
		i--;
	}
	
	return(0);
}

char *BTM_InstGetVarStr(BTM_World *wrl, char *name)
{
	char tb[64];
	BCCX_AttrVal *av;
	int ix;
	int i, j, k;

	ix=BCCX_StringToStridx(name);

	i=wrl->tgen_varstk_pos-1;
	while(i>=0)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
			if((j>>12)==BCCX_IVTY_INT)
			{
				sprintf(tb, "%lld", av->i);
				return(bccx_rstrdup(tb));
			}
			if((j>>12)==BCCX_IVTY_REAL)
			{
				sprintf(tb, "%f", av->f);
				return(bccx_rstrdup(tb));
			}
			if((j>>12)==BCCX_IVTY_STRING)
				return(av->s);
			return(NULL);
		}
		i--;
	}

	i=wrl->tgen_vargbl_cnt-1;
	while(i>=0)
	{
		j=wrl->tgen_vargbl_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
			if((j>>12)==BCCX_IVTY_INT)
			{
				sprintf(tb, "%lld", av->i);
				return(bccx_rstrdup(tb));
			}
			if((j>>12)==BCCX_IVTY_REAL)
			{
				sprintf(tb, "%f", av->f);
				return(bccx_rstrdup(tb));
			}
			if((j>>12)==BCCX_IVTY_STRING)
				return(av->s);
			return(NULL);
		}
		i--;
	}
	
	return(NULL);
}

s64 BTM_InstGetNodeAttrInt(BTM_World *wrl, BCCX_Node *node, char *name)
{
	char *str;
	s64 li;
	
	li=BCCX_GetInt(node, name);
	if(li!=0)
		return(li);

	str=BCCX_Get(node, name);
	if(str && (*str=='$'))
		return(BTM_InstGetVarInt(wrl, str+1));
	
	return(0);
}

double BTM_InstGetNodeAttrFloat(BTM_World *wrl, BCCX_Node *node, char *name)
{
	char *str;
	double lf;
	
	lf=BCCX_GetFloat(node, name);
	if(lf!=0)
		return(lf);

	str=BCCX_Get(node, name);
	if(str && (*str=='$'))
		return(BTM_InstGetVarFloat(wrl, str+1));
	
	return(0);
}

char *BTM_InstGetNodeAttrStr(BTM_World *wrl, BCCX_Node *node, char *name)
{
	char *str;
	
	str=BCCX_Get(node, name);
	if(str && (*str=='$'))
		return(BTM_InstGetVarStr(wrl, str+1));
	return(str);
}

int BTM_InstBindVarIntI(BTM_World *wrl, char *name, s64 val, int flag)
{
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=wrl->tgen_varstk_pos-1;
	k=wrl->tgen_varstk_mark;
	if(flag&1)
		k=0;
	while(i>=k)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_INT<<12);
			wrl->tgen_varstk_val[i]=val;
			return(0);
		}
		i--;
	}

	if(flag&1)
	{
		i=wrl->tgen_vargbl_cnt-1;
		while(i>=0)
		{
			j=wrl->tgen_vargbl_name[i];
			if((j&4095)==ix)
			{
				wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_INT<<12);
				wrl->tgen_vargbl_val[i]=val;
				return(0);
			}
			i--;
		}
	}

	if(flag&2)
	{
		i=wrl->tgen_vargbl_cnt++;
		wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_INT<<12);
		wrl->tgen_vargbl_val[i]=val;
	}else
	{
		i=wrl->tgen_varstk_pos++;
		wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_INT<<12);
		wrl->tgen_varstk_val[i]=val;
	}
	return(0);
}

int BTM_InstBindVarRealI(BTM_World *wrl, char *name, double val, int flag)
{
	BCCX_AttrVal *av;
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=wrl->tgen_varstk_pos-1;
	k=wrl->tgen_varstk_mark;
	if(flag&1)
		k=0;
	while(i>=k)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
			wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_REAL<<12);
			av->f=val;
			return(0);
		}
		i--;
	}

	if(flag&1)
	{
		i=wrl->tgen_vargbl_cnt-1;
		while(i>=0)
		{
			j=wrl->tgen_vargbl_name[i];
			if((j&4095)==ix)
			{
				av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
				wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_REAL<<12);
				av->f=val;
				return(0);
			}
			i--;
		}
	}

	if(flag&2)
	{
		i=wrl->tgen_vargbl_cnt++;
		av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
		wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_REAL<<12);
		av->f=val;
	}else
	{
		i=wrl->tgen_varstk_pos++;
		av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
		wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_REAL<<12);
		av->f=val;
	}
	return(0);
}

int BTM_InstBindVarStrI(BTM_World *wrl, char *name, char *val, int flag)
{
	BCCX_AttrVal *av;
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=wrl->tgen_varstk_pos-1;
	k=wrl->tgen_varstk_mark;
	if(flag&1)
		k=0;
	while(i>=k)
	{
		j=wrl->tgen_varstk_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
			wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_STRING<<12);
			av->s=bccx_strdup(val);
			return(0);
		}
		i--;
	}

	if(flag&1)
	{
		i=wrl->tgen_vargbl_cnt-1;
		while(i>=0)
		{
			j=wrl->tgen_vargbl_name[i];
			if((j&4095)==ix)
			{
				av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
				wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_STRING<<12);
				av->s=bccx_strdup(val);
				return(0);
			}
			i--;
		}
	}

	if(flag&2)
	{
		i=wrl->tgen_vargbl_cnt++;
		av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
		wrl->tgen_vargbl_name[i]=ix|(BCCX_IVTY_REAL<<12);
		av->s=bccx_strdup(val);
	}else
	{
		i=wrl->tgen_varstk_pos++;
		av=(BCCX_AttrVal *)(wrl->tgen_varstk_val+i);
		wrl->tgen_varstk_name[i]=ix|(BCCX_IVTY_STRING<<12);
		av->s=bccx_strdup(val);
	}
	return(0);
}

int BTM_InstBindVarInt(BTM_World *wrl, char *name, s64 val)
{
	return(BTM_InstBindVarIntI(wrl, name, val, 0));
}

int BTM_InstBindVarReal(BTM_World *wrl, char *name, double val)
{
	return(BTM_InstBindVarRealI(wrl, name, val, 0));
}

int BTM_InstBindVarStr(BTM_World *wrl, char *name, char *val)
{
	return(BTM_InstBindVarStrI(wrl, name, val, 0));
}

int BTM_InstSetVarInt(BTM_World *wrl, char *name, s64 val)
{
	return(BTM_InstBindVarIntI(wrl, name, val, 1));
}

int BTM_InstSetVarReal(BTM_World *wrl, char *name, double val)
{
	return(BTM_InstBindVarRealI(wrl, name, val, 1));
}

int BTM_InstSetVarStr(BTM_World *wrl, char *name, char *val)
{
	return(BTM_InstBindVarStrI(wrl, name, val, 1));
}

int BTM_InstSetgVarInt(BTM_World *wrl, char *name, s64 val)
{
	return(BTM_InstBindVarIntI(wrl, name, val, 3));
}

int BTM_InstSetgVarReal(BTM_World *wrl, char *name, double val)
{
	return(BTM_InstBindVarRealI(wrl, name, val, 3));
}

int BTM_InstSetgVarStr(BTM_World *wrl, char *name, char *val)
{
	return(BTM_InstBindVarStrI(wrl, name, val, 3));
}


char *btm_curtopname;

int BTM_InstanceStructureNodeAt(BTM_World *wrl,
	int bcx, int bcy, int bcz, BCCX_Node *node)
{
	BCCX_Node *c;
	char *bty, *rpbty, *s0, *s1, *s2;
	char *otop;
	u32 blk, rpblk;
	int ovspos, ovsmark;
	int na, ci;
	int mx, my, mz, nx, ny, nz;

	if(!node)
		return(0);

	if(BCCX_TagIsP(node, "structure") ||
		BCCX_TagIsP(node, "list") ||
		BCCX_TagIsP(node, "object"))
	{
		na=BCCX_GetNodeChildCount(node);
		for(ci=0; ci<na; ci++)
		{
			c=BCCX_GetNodeIndex(node, ci);
			BTM_InstanceStructureNodeAt(wrl, bcx, bcy, bcz, c);
		}
		return(0);
	}

	if(BCCX_TagIsP(node, "random"))
	{
		na=BCCX_GetNodeChildCount(node);
		if(na<=0)
		{
			s0=BCCX_Get(node, "name");
			mx=BTM_InstGetNodeAttrInt(wrl, node, "min");
			nx=BTM_InstGetNodeAttrInt(wrl, node, "max");
			if((mx==0) && (nx==0))
				{ mx=0; nx=65535; }
			nz=(nx-mx)+1;
			if(s0)
			{
				if((nz>0) && (nz<=65536))
				{
					ci=mx+(btm_tgrand(wrl)%nz);
				}else if(nz>0)
				{
					ci=btm_tgrand(wrl);
					ci=(ci<<16)^btm_tgrand(wrl);
					ci=mx+(ci%nz);
				}
				BTM_InstBindVarInt(wrl, s0, ci);
			}
			
			return(0);
		}

		ci=btm_tgrand(wrl)%na;
		c=BCCX_GetNodeIndex(node, ci);
		BTM_InstanceStructureNodeAt(wrl, bcx, bcy, bcz, c);
		return(0);
	}

	if(BCCX_TagIsP(node, "fill"))
	{
		mx=BTM_InstGetNodeAttrInt(wrl, node, "min_x");
		my=BTM_InstGetNodeAttrInt(wrl, node, "min_y");
		mz=BTM_InstGetNodeAttrInt(wrl, node, "min_z");
		nx=BTM_InstGetNodeAttrInt(wrl, node, "max_x");
		ny=BTM_InstGetNodeAttrInt(wrl, node, "max_y");
		nz=BTM_InstGetNodeAttrInt(wrl, node, "max_z");

		if((mx==nx) && (mx==0))
			{ mx=BTM_InstGetNodeAttrInt(wrl, node, "rel_x"); nx=mx; }
		if((my==ny) && (my==0))
			{ my=BTM_InstGetNodeAttrInt(wrl, node, "rel_y"); ny=my; }
		if((mz==nz) && (mz==0))
			{ mz=BTM_InstGetNodeAttrInt(wrl, node, "rel_z"); nz=mz; }

		bty=BCCX_Get(node, "block");
		blk=BTM_BlockForName(wrl, bty);
		if(!blk)
			return(0);
		
		rpblk=0;
		
		rpbty=BCCX_Get(node, "replace");
		if(rpbty)
			rpblk=BTM_BlockForName(wrl, rpbty);

		BTM_InstFill(wrl,
			bcx+mx, bcy+my, bcz+mz,
			bcx+nx, bcy+ny, bcz+nz,
			blk, rpblk);
		return(0);
	}

	if(BCCX_TagIsP(node, "relight"))
	{
		mx=BTM_InstGetNodeAttrInt(wrl, node, "min_x");
		my=BTM_InstGetNodeAttrInt(wrl, node, "min_y");
		mz=BTM_InstGetNodeAttrInt(wrl, node, "min_z");
		nx=BTM_InstGetNodeAttrInt(wrl, node, "max_x");
		ny=BTM_InstGetNodeAttrInt(wrl, node, "max_y");
		nz=BTM_InstGetNodeAttrInt(wrl, node, "max_z");

		if((mx==nx) && (mx==0))
			{ mx=BTM_InstGetNodeAttrInt(wrl, node, "rel_x"); nx=mx; }
		if((my==ny) && (my==0))
			{ my=BTM_InstGetNodeAttrInt(wrl, node, "rel_y"); ny=my; }
		if((mz==nz) && (mz==0))
			{ mz=BTM_InstGetNodeAttrInt(wrl, node, "rel_z"); nz=mz; }

		BTM_InstRelight(wrl,
			bcx+mx, bcy+my, bcz+mz,
			bcx+nx, bcy+ny, bcz+nz);

		return(0);
	}

	if(BCCX_TagIsP(node, "instance"))
	{
		mx=BTM_InstGetNodeAttrInt(wrl, node, "rel_x");
		my=BTM_InstGetNodeAttrInt(wrl, node, "rel_y");
		mz=BTM_InstGetNodeAttrInt(wrl, node, "rel_z");

		s1=BCCX_Get(node, "name");
		
		if(s1)
		{
			if(!strcmp(s1, "tree"))
			{
				BTM_GenTree(wrl, bcx+mx, bcy+my, bcz+mz);

				BTM_InstRelight(wrl,
					bcx+mx-5, bcy+my-5, bcz+mz,
					bcx+mx+5, bcy+my+5, bcz+mz+10);

				return(0);
			}
		}
		
		if(!s1)
			s1=btm_curtopname;
		
		if(!s1)
			return(0);
		
		s2=BCCX_Get(node, "subname");
		
		otop=btm_curtopname;
		btm_curtopname=s1;

		ovspos=wrl->tgen_varstk_pos;
		ovsmark=wrl->tgen_varstk_mark;
		wrl->tgen_varstk_mark=ovspos;
		
		c=BTM_LookupMenuNode(s1, s2);
		BTM_InstanceStructureNodeAt(wrl, bcx+mx, bcy+my, bcz+mz, c);

		wrl->tgen_varstk_pos=ovspos;
		wrl->tgen_varstk_mark=ovsmark;
		
		btm_curtopname=otop;
		return(0);
	}

	return(0);
}

int BTM_InstanceStructureAt(BTM_World *wrl,
	int bcx, int bcy, int bcz, char *name, char *subname)
{
	BCCX_Node *c;
	char *otop;
	u64 oseed;
	int ovspos, ovsmark;

	otop=btm_curtopname;
	oseed=wrl->tg_curseed;
	btm_curtopname=name;

	BTM_SetupLocalSeedXY(wrl, bcx, bcy);

	if(name)
	{
		if(!strcmp(name, "tree"))
		{
			BTM_GenTree(wrl, bcx, bcy, bcz);

			BTM_InstRelight(wrl,
				bcx-5, bcy-5, bcz,
				bcx+5, bcy+5, bcz+10);

			btm_curtopname=otop;
			wrl->tg_curseed=oseed;
			return(0);
		}
	}

	ovspos=wrl->tgen_varstk_pos;
	ovsmark=wrl->tgen_varstk_mark;
	wrl->tgen_varstk_mark=ovspos;

	c=BTM_LookupMenuNode(name, subname);
	BTM_InstanceStructureNodeAt(wrl, bcx, bcy, bcz, c);

	wrl->tgen_varstk_pos=ovspos;
	wrl->tgen_varstk_mark=ovsmark;
	
	btm_curtopname=otop;
	wrl->tg_curseed=oseed;
	return(0);
}
