#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef HAVE_STDINT_H
#include <stdint.h>

typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef int8_t		sbyte;
typedef int16_t	s16;
typedef int32_t	s32;
typedef int64_t	s64;

#else

typedef unsigned char			byte;
typedef unsigned short		u16;
typedef unsigned int			u32;
typedef unsigned long long	u64;

typedef signed char			sbyte;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;

#endif

#include "tkmod_info.h"

#include "tfau_comm.c"
#include "tfau_wave.c"

byte *TkMod_LoadFile(char *name, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(name, "rb");
	if(!fd)
	{
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		if(i>0)
		{
			sz=i;
		}else
		{
			free(buf);
			return(NULL);
		}
	}
	
//	*(u64 *)(buf+sz+0)=0;
//	*(u64 *)(buf+sz+8)=0;
	memset(buf+sz, 0, 16);
	
	*rsz=sz;
	return(buf);
}

int TkMod_StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)
		return(-1);
	
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int TkMod_GetU16BE(byte *buf)
{
	return((buf[0]<<8)|buf[1]);
}

int TkMod_LoadFromBufferCtx(TkMod_Info *ctx, byte *data, int dsz)
{
	byte *cs;
	int maxpat, n_pat, pat;
	int cmda, cmdb, cmdc, cmds, cmdx, cmdy;
	int songlen;

	int ls, le;
	int i, j, k, l;
	memcpy(ctx->modname, data, 20);
	memcpy(&(ctx->samphead), data+20, 31*30);
	memcpy(&(ctx->hed2), data+20+(31*30), 134);
	
//	printf("%04s\n", ctx->hed2.sig);

	maxpat=0;
	for(i=0; i<ctx->hed2.num_positions; i++)
	{
		j=ctx->hed2.pattern_tab[i];
		if(j>maxpat)
			maxpat=j;
	}
	
	n_pat=maxpat+1;
	printf("n_pat=%d\n", n_pat);

	cs=data+0x438+4;

	ctx->patterns=malloc(n_pat*1024);
	memcpy(ctx->patterns, cs, n_pat*1024);
	cs+=n_pat*1024;
//	cs+=maxpat*1024;
//	cs+=(maxpat+2)*1024;

	k=0;
	for(i=0; i<31; i++)
	{
//		l=	(ctx->samphead[i].samplen[0]<<8) |
//			(ctx->samphead[i].samplen[1]<<0) ;
		l=TkMod_GetU16BE(ctx->samphead[i].samplen);

//		if(l&1)l++;
		
//		l--;
		ctx->sampofs[i]=k;
		k+=l*2;
//		ctx->sampdat[i]=malloc(l*2);
	}
	
	ctx->sampbuf=malloc(k+(64*4));
	memset(ctx->sampbuf, 0, k+(64*4));
//	memcpy(ctx->sampbuf, cs, k);

	for(i=0; i<31; i++)
	{
		l=TkMod_GetU16BE(ctx->samphead[i].samplen);
//		ctx->sampdat[i]=ctx->sampbuf+ctx->sampofs[i];
		ctx->sampdat[i]=ctx->sampbuf+ctx->sampofs[i]+(i*4);
		memcpy(ctx->sampdat[i], cs+ctx->sampofs[i], l*2);

		ls=TkMod_GetU16BE(ctx->samphead[i].repeat_start)*2;
		le=TkMod_GetU16BE(ctx->samphead[i].repeat_len)*2;
		j=TkMod_GetU16BE(ctx->sampdat[i]);
		printf("Sample %d Tag %04X Len=%d Ls=%d Ld=%d\n", i+1, j, l, ls, le);
	}

	ctx->tick_bpm=125;
	ctx->tick_div=6;
	ctx->mixrate=16000;

	ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
	ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
	ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
	
	printf("divsamp=%d\n", ctx->tick_divsamp);

#if 0
	for(i=0; i<ctx->hed2.num_positions; i++)
	{
		pat=ctx->hed2.pattern_tab[i];
		printf("Pattern %d\n", pat);
	
		cs=ctx->patterns + pat*1024;
		for(j=0; j<64; j++)
		{
			for(k=0; k<4; k++)
			{
				cmda=(cs[0]<<8)|cs[1];
				cmdb=(cs[2]<<8)|cs[3];
				cs+=4;
				
				cmds=((cmda>>12)<<4)|(cmdb>>12);
				cmdx=cmda&0x0FFF;
				cmdy=cmdb&0x00FF;
				cmdc=(cmdb>>8)&15;
				
//				printf("%04X-%04X ", cmda, cmdb);
				printf("%02X-%01X-%03X-%02X ", cmds, cmdc, cmdx, cmdy);
			}
			printf("\n");
		}
	}
#endif

	songlen=(ctx->hed2.num_positions*64*60)/ctx->tick_divmin;
	printf("length: %ds (%02d:%02d)\n", songlen,
		(songlen/60), (songlen%60));

	ctx->songlen=songlen;

//	ctx->mixbuf=malloc(16384);
//	ctx->mixpos=ctx->mixbuf;

	return(0);
}

int TkMod_UpdateSongCol(TkMod_Info *ctx, int col, int cmda, int cmdb)
{
	int cmdc, cmds, cmdx, cmdy, hz, step, ix;
	int i, j, k;

	cmds=((cmda>>12)<<4)|(cmdb>>12);
	cmdx=cmda&0x0FFF;
	cmdy=cmdb&0x00FF;
	cmdc=(cmdb>>8)&15;

//	printf("%02X-%01X-%03X-%02X ", cmds, cmdc, cmdx, cmdy);

	if(cmds!=0)
	{
		if(cmdx!=0)
		{
//			hz=7093789/(cmdx*2);
//			step=(hz<<12)/16000;
//			step=(hz<<12)/ctx->mixrate;
//			ctx->steprate[col]=step;
			ctx->step_fq[col]=cmdx;
			ctx->step_fq_slide[col]=0;
			ctx->step_fq_stgt[col]=0;
		}

		ix=cmds-1;

		j=TkMod_GetU16BE(ctx->samphead[ix].samplen)*2;
		j=(j*15)/16;
		
		if((ctx->sampidx[col]!=ix) || !ctx->samp_isloop[col])
		{
			ctx->sampidx[col]=ix;
//			ctx->sampvol[col]=ctx->samphead[ix].volume;
			ctx->stepfrac[col]=0;
		}

		ctx->sampvol[col]=ctx->samphead[ix].volume;

		j=TkMod_GetU16BE(ctx->samphead[ix].repeat_start)*2;
		k=TkMod_GetU16BE(ctx->samphead[ix].repeat_len)*2;

		if(k<=2)
		{
			j=TkMod_GetU16BE(ctx->samphead[ix].samplen)*2;
			k=1;
		}

		ctx->samp_lbeg[col]=j;
		ctx->samp_lend[col]=j+k;
		ctx->samp_isloop[col]=(k!=1);
	}

	switch(cmdc)
	{
	case 0x0:
		break;
	case 0x1:
		ctx->step_fq_slide[col]=-cmdy;
		break;
	case 0x2:
		ctx->step_fq_slide[col]=cmdy;
		break;
	case 0x3:
		if(cmdx>=ctx->step_fq[col])
			ctx->step_fq_slide[col]=cmdy;
		else
			ctx->step_fq_slide[col]=-cmdy;
		ctx->step_fq_stgt[col]=cmdx;
		break;
	case 0x9:
		ctx->stepfrac[col]=cmdy<<8;
		break;
	case 0xA:
		j=(cmdy>>4)&15;
		k=(cmdy>>0)&15;
		if(j!=0)
		{
			ctx->sampvol[col]+=(j-1);
			if(ctx->sampvol[col]>=64)
				ctx->sampvol[col]=63;
		}
		if(k!=0)
		{
			ctx->sampvol[col]-=(k-1);
			if(ctx->sampvol[col]<1)
				ctx->sampvol[col]=1;
		}
		break;

	case 0xB:
		ctx->row+=cmdy;
		break;

	case 0xC:
		ctx->sampvol[col]=cmdy;
		break;

	case 0xE:
//		printf("CMD E: %02X\n", cmdy);

		if((cmdy&0xF0)==0x10)
			ctx->step_fq[col]-=cmdy&0x0F;
		if((cmdy&0xF0)==0x20)
			ctx->step_fq[col]+=cmdy&0x0F;
		break;

	case 0xF:
//		ctx->sampvol[col]=cmdy;
		if(!cmdy)
			cmdy=1;
		if(cmdy<=32)
			ctx->tick_div=cmdy;
		else
			ctx->tick_bpm=cmdy;
		ctx->tick_divmin=(24*ctx->tick_bpm)/ctx->tick_div;
		ctx->tick_divsamp=(ctx->mixrate*60)/ctx->tick_divmin;
		ctx->tick_samp=ctx->tick_divsamp/ctx->tick_div;
		break;

	default:
		printf("Unhandled Effect %X\n", cmdc);
		break;
	}

	cmdx=ctx->step_fq[col];
	if(cmdx)
	{
		hz=7093789/(cmdx*2);
		step=(hz<<12)/ctx->mixrate;
		ctx->steprate[col]=step;
	//	ctx->step_fq[col]=cmdx;
	}
}

int TkMod_StepSongRow(TkMod_Info *ctx)
{
	byte *cs;
	int pat, row, cmda, cmdb;
	int i, j, k;
	
	row=ctx->row;
	ctx->row=row+1;
	i=row>>6;
	pat=ctx->hed2.pattern_tab[i];
//	printf("Pattern %d\n", pat);

	cs=ctx->patterns + pat*1024;
	cs+=(row&63)*16;

	for(k=0; k<4; k++)
	{
		cmda=(cs[0]<<8)|cs[1];
		cmdb=(cs[2]<<8)|cs[3];
		cs+=4;
		TkMod_UpdateSongCol(ctx, k, cmda, cmdb);
//		printf("%02X-%01X-%03X-%02X ", cmds, cmdc, cmdx, cmdy);
	}
//	printf("\n");
}

int TkMod_StepMixCtx(TkMod_Info *ctx, int cnt)
{
	sbyte *sd0, *sd1, *sd2, *sd3;
	s16 *ct;
	int sf0, sf1, sf2, sf3;
	int sr0, sr1, sr2, sr3;
	int sv0, sv1, sv2, sv3;
	int slb0, slb1, slb2, slb3;
	int sle0, sle1, sle2, sle3;
	int sld0, sld1, sld2, sld3;
	int i0, i1, i2, i3;
	int cmdx, hz, step;
	int i, j, k;

	ct=ctx->mixpos;

	i0=ctx->sampidx[0];		i1=ctx->sampidx[1];
	i2=ctx->sampidx[2];		i3=ctx->sampidx[3];
	sd0=ctx->sampdat[i0];	sd1=ctx->sampdat[i1];
	sd2=ctx->sampdat[i2];	sd3=ctx->sampdat[i3];
	sf0=ctx->stepfrac[0];	sf1=ctx->stepfrac[1];
	sf2=ctx->stepfrac[2];	sf3=ctx->stepfrac[3];
	sr0=ctx->steprate[0];	sr1=ctx->steprate[1];
	sr2=ctx->steprate[2];	sr3=ctx->steprate[3];
	sv0=ctx->sampvol[0];	sv1=ctx->sampvol[1];
	sv2=ctx->sampvol[2];	sv3=ctx->sampvol[3];
	
	slb0=ctx->samp_lbeg[0]<<12;		sle0=ctx->samp_lend[0]<<12;
	slb1=ctx->samp_lbeg[1]<<12;		sle1=ctx->samp_lend[1]<<12;
	slb2=ctx->samp_lbeg[2]<<12;		sle2=ctx->samp_lend[2]<<12;
	slb3=ctx->samp_lbeg[3]<<12;		sle3=ctx->samp_lend[3]<<12;

	sld0=sle0-slb0;	sld1=sle1-slb1;
	sld2=sle2-slb2;	sld3=sle3-slb3;
	
	for(i=0; i<cnt; i++)
	{
		i0=sf0>>12;		i1=sf1>>12;
		i2=sf2>>12;		i3=sf3>>12;
		i0=sd0[i0];		i1=sd1[i1];
		i2=sd2[i2];		i3=sd3[i3];
		k=(sv0*i0)+(sv1*i1)+(sv2*i2)+(sv3*i3);
		
		if(k<-32767)
			k=-32767;
		if(k>32767)
			k=32767;
		
		ct[i]=k;
		sf0+=sr0;	sf1+=sr1;
		sf2+=sr2;	sf3+=sr3;
		
		while(sf0>sle0)		sf0-=sld0;
		while(sf1>sle1)		sf1-=sld1;
		while(sf2>sle2)		sf2-=sld2;
		while(sf3>sle3)		sf3-=sld3;
	}

	ctx->stepfrac[0]=sf0;	ctx->stepfrac[1]=sf1;
	ctx->stepfrac[2]=sf2;	ctx->stepfrac[3]=sf3;
	ctx->mixpos=ct+cnt;
	
	for(i=0; i<4; i++)
	{
		if(ctx->step_fq_slide[i])
		{
			cmdx=ctx->step_fq[i];
			cmdx+=ctx->step_fq_slide[i];
			if(ctx->step_fq_stgt[i])
			{
				if(ctx->step_fq_slide[i]<0)
				{
					if(cmdx<ctx->step_fq_stgt[i])
					{
						cmdx=ctx->step_fq_stgt[i];
						ctx->step_fq_slide[i]=0;
					}
				}else
				{
					if(cmdx>ctx->step_fq_stgt[i])
					{
						cmdx=ctx->step_fq_stgt[i];
						ctx->step_fq_slide[i]=0;
					}
				}
			}else
			{
				if(cmdx<113)cmdx=113;
				if(cmdx>856)cmdx=856;
			}
			hz=7093789/(cmdx*2);
			step=(hz<<12)/ctx->mixrate;
			ctx->steprate[i]=step;
			ctx->step_fq[i]=cmdx;
		}
	}
}

int main(int argc, char *argv[])
{
	TkMod_Info tctx;
	TkMod_Info *ctx;
	byte *ibuf;
	int isz, len;
	int i, j, k;

	ibuf=TkMod_LoadFile("musix-shine.mod", &isz);
//	ibuf=TkMod_LoadFile("musix-kids.mod", &isz);
//	ibuf=TkMod_LoadFile("BGB_Noise1.mod", &isz);
	
	ctx=&tctx;
	memset(ctx, 0, sizeof(TkMod_Info));
	TkMod_LoadFromBufferCtx(ctx, ibuf, isz);

	len=(ctx->songlen+1)*16000;
	ctx->mixbuf=malloc(len*2);
	ctx->mixpos=ctx->mixbuf;
	ctx->mixbufe=ctx->mixbuf+len;

	while((ctx->mixpos+ctx->tick_divsamp)<ctx->mixbufe)
	{
		TkMod_StepSongRow(ctx);
//		TkMod_StepMixCtx(ctx, ctx->tick_divsamp);
		for(i=0; i<ctx->tick_div; i++)
			TkMod_StepMixCtx(ctx, ctx->tick_samp);
	}

	BGBMID_StoreWAV("out0.wav", (byte *)(ctx->mixbuf), 1, 16000, 16, len);

}
