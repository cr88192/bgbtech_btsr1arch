/*
 MIT No Attribution

 Copyright (c) 2018-2024 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person 
 obtaining a copy of this software and associated documentation 
 files (the "Software"), to deal in the Software without 
 restriction, including without limitation the rights to use, 
 copy, modify, merge, publish, distribute, sublicense, and/or sell 
 copies of the Software, and to permit persons to whom the 
 Software is furnished to do so.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 OR OTHER DEALINGS IN THE SOFTWARE.
*/

u32 jx2i_sndaupcm_blkbuf_l[4096];
u32 jx2i_sndaupcm_blkbuf_r[4096];
u32 jx2i_sndaupcm_ctlr[16];

int jx2i_sndaupcm_pcm_rov;
int jx2i_sndaupcm_acc_us;

int jx2i_sndaupcm_t_pcm_rov;
int jx2i_sndaupcm_t_acc_us;

int sblk0_dec(int v)
{
	int e, v0, v1;
	
	if(v&0x70)
	{
		e=((v>>4)&7)+3;
		v0=(v&15)|16;
		v1=v0<<e;
	}else
	{
		e=4;
		v0=v&15;
		v1=v0<<e;
	}
	if(v&0x80)
		v1=-v1;
	return(v1);
}

byte	jx2i_sndaupcm_is16b;
byte	jx2i_sndaupcm_iscmp;
byte	jx2i_sndaupcm_isste;
u32		jx2i_sndaupcm_cr0;
int		jx2i_sndaupcm_sampus;

int SndAuPcm_UpdateForRegisters()
{
	u32 cr0;
	int sampus;

	cr0=jx2i_sndaupcm_ctlr[0];
	jx2i_sndaupcm_cr0=cr0;
	jx2i_sndaupcm_iscmp=cr0&1;
	jx2i_sndaupcm_is16b=cr0&2;
	jx2i_sndaupcm_isste=cr0&4;
	
	switch((cr0>>4)&15)
	{
		case 0: sampus=125; break;		//8kHz
		case 1: sampus=91; break;		//11kHz
		case 2: sampus=64; break;		//16kHz
		case 3: sampus=45; break;		//22kHz
		case 4: sampus=32; break;		//32kHz
		case 5: sampus=23; break;		//44kHz
		case 6: sampus=15; break;		//64kHz
		case 7: sampus=11; break;		//88kHz

		case  9: sampus=21; break;		//48kHz
		case 11: sampus=10; break;		//96kHz

		default: sampus=10; break;		//96kHz
	}
	
	jx2i_sndaupcm_sampus=sampus;
	
	return(0);
}

int SndAuPcm_GetStepSamples(short *pcm, int nsamp, int step_us)
{
	int c0, c1;
	int cr0, is16b, iscmp, isste;
	int sampus, accus, rov;
	int i;
	
	SndAuPcm_UpdateForRegisters();
	
	cr0=jx2i_sndaupcm_cr0;
	iscmp=jx2i_sndaupcm_iscmp;
	is16b=jx2i_sndaupcm_is16b;
	isste=jx2i_sndaupcm_isste;
	sampus=jx2i_sndaupcm_sampus;
	
	if((cr0&0xC)==0)
	{
		for(i=0; i<nsamp; i++)
		{
			pcm[i*2+0]=0;
			pcm[i*2+1]=0;
		}
		return(0);
	}
	
	accus=jx2i_sndaupcm_t_acc_us;
	rov=jx2i_sndaupcm_t_pcm_rov;
//	accus=0;
	
	for(i=0; i<nsamp; i++)
	{
		if(is16b)
//		if(1)
		{
			c0=jx2i_sndaupcm_blkbuf_l[(rov>>1)&2047];
			c1=jx2i_sndaupcm_blkbuf_r[(rov>>1)&2047];
			if(rov&1)	{ c0>>=16; c1>>=16; }
			if(iscmp)		{ c0=(s16)c0; c1=(s16)c1; }
			else		{ c0=(c0&65535)-32768; c1=(c1&65535)-32768; }
		}else
		{
			c0=jx2i_sndaupcm_blkbuf_l[(rov>>2)&2047];
			c1=jx2i_sndaupcm_blkbuf_r[(rov>>2)&2047];
			if(rov&2)	{ c0>>=16; c1>>=16; }
			if(rov&1)	{ c0>>=8; c1>>=8; }
			if(iscmp)
			{
				c0=sblk0_dec(c0&255);
				c1=sblk0_dec(c1&255);
			}else
			{
				c0=((c0&255)<<8)-32768;
				c1=((c1&255)<<8)-32768;
			}
		}
		
		if(!isste)
			c1=c0;

		pcm[i*2+0]=c0;
		pcm[i*2+1]=c1;
	
		accus+=step_us;
		while(accus>=sampus)
			{ accus-=sampus; rov++; }
	}

	jx2i_sndaupcm_t_acc_us=accus;
	jx2i_sndaupcm_t_pcm_rov=rov;

//	SndAuPcm_SyncTimeUsec(ctx);

	return(0);
}

s64 jx2i_sndaupcm_usec;

int SndAuPcm_SyncTimeUsec(BJX2_Context *ctx)
{
//	static int dbias=1024;
//	static int dbias=256;
	static int dbias=128;
	int trov;
	s64 usc, dt, ds;
	int sampus;

	SndAuPcm_UpdateForRegisters();

	usc=BJX2_Interp_GetVirtualUsec(ctx);
	dt=usc-jx2i_sndaupcm_usec;
	if(dt<0)dt=0;
//	if(dt>100000)
	jx2i_sndaupcm_usec=usc;
	if(dt>1000000)
		dt=0;

	sampus=jx2i_sndaupcm_sampus;
	jx2i_sndaupcm_acc_us+=dt;
	while(jx2i_sndaupcm_acc_us>=sampus)
	{
		jx2i_sndaupcm_acc_us-=sampus;
		jx2i_sndaupcm_pcm_rov++;
	}
	
	trov=(jx2i_sndaupcm_t_pcm_rov+dbias);
//	ds=jx2i_sndaupcm_pcm_rov-jx2i_sndaupcm_t_pcm_rov;
	ds=jx2i_sndaupcm_pcm_rov-trov;
	ds=ds^(ds>>31);
//	if(ds>2000)
	if(ds>3000)
//	if(ds>1000)
	{
//		printf("SndAuPcm_SyncTimeUsec: Sync %d\n", jx2i_sndaupcm_pcm_rov-trov);
//		jx2i_sndaupcm_t_acc_us=jx2i_sndaupcm_acc_us;
//		jx2i_sndaupcm_t_pcm_rov=jx2i_sndaupcm_pcm_rov;
		jx2i_sndaupcm_t_pcm_rov=jx2i_sndaupcm_pcm_rov-dbias;
	}

//	jx2i_sndaupcm_t_acc_us=jx2i_sndaupcm_acc_us;
//	jx2i_sndaupcm_t_pcm_rov=jx2i_sndaupcm_pcm_rov;

	return(0);
}


int BJX2_MemSndAuPcmCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndAuPcmCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemSndAuPcmCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	s64 rvq;
	int ra, rv, vn;
	int i, j, k;

	ra=addr-sp->addr_base;
	
	rv=0;

#if 0
	if((ra>>2)>=1024)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			rv=SMus_GetRegister(ctx, (ra-0x1800)>>2);
			return(rv);
		}
	}
#endif

//	if(jx2i_sndblk_blkbuf)
//	if(1)
//	{
//		rv=jx2i_sndblk_blkbuf[(ra>>2)&1023];
//	}

	if((ra>=0xF000) && (ra<0xFFFF))
	{
		SndAuPcm_SyncTimeUsec(ctx);

		i=(ra>>2)&255;
		switch(i)
		{
		case 0x0:	case 0x1:
		case 0x2:	case 0x3:
		case 0x4:	case 0x5:
		case 0x6:	case 0x7:
			rv=jx2i_sndaupcm_ctlr[i];
			break;
		case 0x8:
			rv=jx2i_sndaupcm_pcm_rov&8191;
//			rv=jx2i_sndaupcm_t_pcm_rov&8191;
//			rv=0;
			break;
		default:
			break;
		}
	}

	if((ra>>14)==0)
	{
		rv=jx2i_sndaupcm_blkbuf_l[(ra>>2)&4095];
	}

	if((ra>>14)==1)
	{
		rv=jx2i_sndaupcm_blkbuf_r[(ra>>2)&4095];
	}


	return(rv);
}

int BJX2_MemSndAuPcmCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndAuPcmCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndAuPcmCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra;
	int i;

	ra=addr-sp->addr_base;

#if 0
	if((ra>>2)>=1024)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			SMus_SetRegister(ctx, (ra-0x1800)>>2, val);
			return(0);
		}
	}
#endif

	if((ra>=0xF000) && (ra<0xFFFF))
	{
		i=(ra>>2)&255;
		switch(i)
		{
		case 0x0:	case 0x1:
		case 0x2:	case 0x3:
		case 0x4:	case 0x5:
		case 0x6:	case 0x7:
			jx2i_sndaupcm_ctlr[i]=val;
			break;
		default:
			break;
		}
	}

	if((ra>>14)==0)
	{
		jx2i_sndaupcm_blkbuf_l[(ra>>2)&4095]=val;
	}

	if((ra>>14)==1)
	{
		jx2i_sndaupcm_blkbuf_r[(ra>>2)&4095]=val;
	}

	return(0);
}

int BJX2_MemDefineSndAuPcm(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemSndAuPcmCb_GetByte;
	sp->GetWord=BJX2_MemSndAuPcmCb_GetWord;
	sp->GetDWord=BJX2_MemSndAuPcmCb_GetDWord;
	
	sp->SetByte=BJX2_MemSndAuPcmCb_SetByte;
	sp->SetWord=BJX2_MemSndAuPcmCb_SetWord;
	sp->SetDWord=BJX2_MemSndAuPcmCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

