/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

s64 BJX2_Interp_GetVirtualUsec(BJX2_Context *ctx);
int SMus_CheckApplyRegisters();


u32 jx2i_smus_ctrl[128];		//SMus Control Registers
u32 jx2i_smus_accum[64];	//SMus Accumulators
s64 jx2i_smus_usec;			//microsecond timer

// int jx2i_smus_lastval[16];	//SMus Last Value

#define SMUS_RGBUF_SZ	4096

u32		jx2i_smus_rgbuf_val[SMUS_RGBUF_SZ];	//register ring buffer value
int		jx2i_smus_rgbuf_reg[SMUS_RGBUF_SZ];	//register ring buffer ID
s64		jx2i_smus_rgbuf_usc[SMUS_RGBUF_SZ];	//register ring buffer usec

short jx2i_smus_rgbuf_spos;
short jx2i_smus_rgbuf_epos;

u16		jx2i_smus_sintab[256];

u32		*jx2i_smus_aupcmbuf;

int SMus_ScaleWaveAttn(int val, int attn)
{
	int v0, v1;

	if(attn==63)
		return(0);

	v0=val;
	
//	v0=(val*(9+(~(attn&7))))>>4;
	v0=(val*(16-(attn&7)))>>4;
	
//	if(attn&4)v0-=(val>>1);
//	if(attn&2)v0-=(val>>2);
//	if(attn&1)v0-=(val>>3);
	v1=v0>>((attn>>3)&7);
	return(v1);
}

void SMus_Step64k()
{
	int i;
	
	for(i=0; i<32; i++)
		jx2i_smus_accum[i]+=jx2i_smus_ctrl[i];
	
	jx2i_smus_usec+=16;
	SMus_CheckApplyRegisters();
}

void SMus_Step32k()
{
	int i;
	
//	for(i=0; i<32; i++)
//		jx2i_smus_accum[i]+=jx2i_smus_ctrl[i]<<1;

	for(i=0; i<16; i++)
	{
		if(jx2i_smus_ctrl[i*2+0]&0x00100000)
		{
			jx2i_smus_accum[i*2+0]+=(jx2i_smus_ctrl[i*2+0]&0x000FFFFF)<<1;
			if(jx2i_smus_ctrl[i*2+1]&0x00100000)
				jx2i_smus_accum[i*2+1]-=jx2i_smus_accum[i*2+0]>>20;
			else
				jx2i_smus_accum[i*2+1]+=jx2i_smus_accum[i*2+0]>>20;
			jx2i_smus_accum[i*2+0]&=0x000FFFFF;
		}else
		{
			jx2i_smus_accum[i*2+0]+=jx2i_smus_ctrl[i*2+0]<<1;
			jx2i_smus_accum[i*2+1]+=jx2i_smus_ctrl[i*2+1]<<1;
		}
	}

	jx2i_smus_usec+=32;
	SMus_CheckApplyRegisters();
}

void SMus_Step16k()
{
	int i;
	
//	for(i=0; i<32; i++)
//		jx2i_smus_accum[i]+=jx2i_smus_ctrl[i]<<2;

	for(i=0; i<16; i++)
	{
		if(jx2i_smus_ctrl[i*2]&0x00100000)
		{
			jx2i_smus_accum[i*2+0]+=(jx2i_smus_ctrl[i*2+0]&0x000FFFFF)<<2;
			if(jx2i_smus_ctrl[i*2+1]&0x00100000)
				jx2i_smus_accum[i*2+1]-=jx2i_smus_accum[i*2+0]>>20;
			else
				jx2i_smus_accum[i*2+1]+=jx2i_smus_accum[i*2+0]>>20;
			jx2i_smus_accum[i*2+0]&=0x000FFFFF;
		}else
		{
			jx2i_smus_accum[i*2+0]+=jx2i_smus_ctrl[i*2+0]<<2;
			jx2i_smus_accum[i*2+1]+=jx2i_smus_ctrl[i*2+1]<<2;
		}
	}

	jx2i_smus_usec+=64;
//	jx2i_smus_usec+=23;
	SMus_CheckApplyRegisters();
}

int SMus_GetVoiceAttn(int vn)
{
	int an;
	an=(jx2i_smus_ctrl[vn]>>22)&63;
	return(an);
}

int (*SMus_GetVoiceWaveRsVal[32])(int vn);
int (*SMus_GetVoiceWaveRsVal_Mod[32])(int vn);

int SMus_GetVoiceWaveRsVal_Rst(int vn)
{
//	jx2i_smus_accum[vn]=0x40000;
	return(0);
}

int SMus_GetVoiceWaveRsVal_Mute(int vn)
{
	return(0);
}

/** Get raw sample value for a voice waveform */
int SMus_GetVoiceWaveRsVal_Dfl(int vnfl)
{
	int fn, acc, v, vn, vm, am;
	int an, usesin;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
//	vn=vnfl&15;
	vn=vnfl&31;
	
	if(!jx2i_smus_ctrl[vn])
	{
//		jx2i_smus_accum[vn]=0x40000;
		
//		v=jx2i_smus_lastval[vn];
//		v=(v*31)/32;
//		jx2i_smus_lastval[vn]=v;

		return(0);
//		return(v);
	}
	
	acc=jx2i_smus_accum[vn];

//	if(((jx2i_smus_ctrl[vn&7]>>21)&1) && !(vnfl&16))
	if(((jx2i_smus_ctrl[vn&0x1E]>>21)&1) && !(vnfl&32))
	{
//		if(vn&8)
		if(vn&1)
		{
			/* Mute 8-15 if Linked. */
			return(0);
		}
		
//		am=SMus_GetVoiceAttn(vn|8);
//		vm=SMus_GetVoiceWaveRsVal_Mod[vn|8](vn|8|16);

		am=SMus_GetVoiceAttn(vn|1);
		vm=SMus_GetVoiceWaveRsVal_Mod[vn|1](vn|1|32);

//		vm*=4;
		vm*=2;
//		vm=SMus_ScaleWaveAttn(vm, am);
		acc+=vm*16;
//		acc+=jx2i_smus_accum[8|vn];
	}

	an=(jx2i_smus_ctrl[vn]>>22)&63;

	ph0=(acc>>4)&0xFFFF;	//19..4
	ph1=(acc>>3)&0xFFFF;	//18..3
	ph2=ph1^0xFFFF;
	ph3=(acc>>2)&0xFFFF;
	fn=(jx2i_smus_ctrl[vn]>>28)&7;
	
//	fn=0;
//	fn=7;
	
	x19=(((int)(acc<<12))>>31)&0xFFFF;	//mask for acc[19]
	x18=(((int)(acc<<13))>>31)&0xFFFF;	//mask for acc[18]
	usesin=0;
	
	switch(fn)
	{
	case 0:
		if(!usesin)
			v=ph1^x19;
		else
			v=jx2i_smus_sintab[ph0>>8];
		break;
	case 1:
		if(!usesin)
			v=ph1^x19;
		else
			v=jx2i_smus_sintab[ph0>>8];
		if(v<0x8000)v=0x8000;
		break;
	case 2:
		if(!usesin)
			v=ph1^x19;
		else
			v=jx2i_smus_sintab[ph0>>8];
		if(v<0x8000)v^=0xFFFF;
		break;
	case 3:
		if(!usesin)
			v=ph1;
		else
			v=jx2i_smus_sintab[ph0>>8]^x19^x18;
		if(v<0x8000)v=0x8000;
		break;

	case 4:
		v=ph1^x19;
//		v=ph3^x18;
//		if(x19)v=0x8000;
		break;

	case 5:			//Sawtooth
		v=ph1;
		break;

	case 6:			//Square
		v=x19;
		break;

	case 7:			//Noise
//		v=((ph3*4093)>>12)&0xFFFF;
//		v=((ph0*4093)>>12)&0xFFFF;
//		v=(ph0^(ph0<<3)^(ph0<<7)^(ph0<<11))&0xFFFF;
		v=(ph0^(ph0<<3)^(ph0<<7))&0xFFFF;
//		v=(ph0^(ph0<<3))&0xFFFF;
		break;

	default:			//Sawtooth
		v=ph1;
		break;
	}
	
	v=(short)(v^0x8000);

	v=SMus_ScaleWaveAttn(v, an);

//	jx2i_smus_lastval[vn]=v;

	return(v);
}

/** Get raw sample value for a voice waveform */
int SMus_GetVoiceWaveRsVal_Pcm(int vnfl)
{
	u32 ctrl_a, ctrl_d, ctrl_c, ctrl_f;
	int fn, acc, v, v0, v1, vn, vm, am;
	int an, lofs, llen, lbeg, lend;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
	vn=vnfl&31;
	
	ctrl_a=jx2i_smus_ctrl[vn+0];
	ctrl_d=jx2i_smus_ctrl[vn+1];
	ctrl_c=jx2i_smus_ctrl[64+vn+0];
	ctrl_f=jx2i_smus_ctrl[64+vn+1];

	an=(ctrl_a>>22)&63;
	
	if(!ctrl_a)
	{
		return(0);
	}
	
	if(!jx2i_smus_aupcmbuf)
	{
		return(0);
	}
	
	lofs=(ctrl_c>> 0)&0xFFFF;
	llen=(ctrl_c>>16)&0xFFFF;
	lbeg=(ctrl_f>> 0)&0xFFFF;
	lend=(ctrl_f>>16)&0xFFFF;
	
	if(ctrl_a&0x00200000)
	{
		if(jx2i_smus_accum[vn+1]<lbeg)
		{
			if(ctrl_d&0x00100000)
			{
				jx2i_smus_accum[vn+1]=lbeg;
				jx2i_smus_ctrl[vn+1]=ctrl_d&(~0x00100000);
			}
		}

		if(jx2i_smus_accum[vn+1]>lend)
		{
			if(ctrl_d&0x00200000)
			{
				jx2i_smus_accum[vn+1]=lend;
				jx2i_smus_ctrl[vn+1]=ctrl_d|0x00100000;
			}else
			{
				jx2i_smus_accum[vn+1]=lbeg;
			}
		}
	}else
	{
		if(ctrl_d&0x00100000)
		{
			if(jx2i_smus_accum[vn+1]>llen)
				{ jx2i_smus_accum[vn+1]=0; }
		}else
		{
			if(jx2i_smus_accum[vn+1]>llen)
				{ jx2i_smus_accum[vn+1]=llen; }
		}
	}
	
	ph0=(lofs<<4)+(jx2i_smus_accum[vn+1]&0xFFFF);
	ph1=(jx2i_smus_accum[vn+0]>>16)&15;

//	ph1=jx2i_smus_aupcmbuf[ph0>>2];
//	v=sblk0_dec(ph1>>((ph0&3)*8));

	v0=((byte *)jx2i_smus_aupcmbuf)[ph0+0];
	v1=((byte *)jx2i_smus_aupcmbuf)[ph0+1];
	if(ctrl_d&0x00100000)
		v1=((byte *)jx2i_smus_aupcmbuf)[ph0-1];
	v0=sblk0_dec(v0);
	v1=sblk0_dec(v1);
	v=(((16-ph1)*v0)+(ph1*v1))>>4;

	v=SMus_ScaleWaveAttn(v, an);
	return(v);
}

#if 0
/** Get raw sample value for a voice waveform */
int SMus_GetVoiceWaveRsVal_Car0(int vnfl)
{
	int fn, acc, v, vn, vm, am;
	int an;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
//	vn=vnfl&15;
	vn=vnfl&31;
		
	acc=jx2i_smus_accum[vn];

//	if((jx2i_smus_ctrl[vn&7]>>21)&1)
	if((jx2i_smus_ctrl[vn&0x1E]>>21)&1)
	{
//		am=SMus_GetVoiceAttn(vn|8);
//		vm=SMus_GetVoiceWaveRsVal_Mod[vn|8](vn|8|16);
		am=SMus_GetVoiceAttn(vn|1);
		vm=SMus_GetVoiceWaveRsVal_Mod[vn|1](vn|1|32);
		vm*=4;
		acc+=vm*16;
	}

	an=(jx2i_smus_ctrl[vn]>>22)&63;

	ph0=(acc>>4)&0xFFFF;	//19..4
	ph1=(acc>>3)&0xFFFF;	//18..3
//	fn=(jx2i_smus_ctrl[vn]>>28)&7;
	
	x19=(((int)(acc<<12))>>31)&0xFFFF;	//mask for acc[19]
	
//	v=ph1^x19;
	v=jx2i_smus_sintab[ph0>>8]-32768;
	v=SMus_ScaleWaveAttn(v, an);
//	jx2i_smus_lastval[vn]=v;

	return(v);
}

/** Get raw sample value for a voice waveform */
int SMus_GetVoiceWaveRsVal_Mod0(int vnfl)
{
	int fn, acc, v, vn, vm, am;
	int an;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
//	vn=vnfl&15;
	vn=vnfl&31;
		
	acc=jx2i_smus_accum[vn];

	an=(jx2i_smus_ctrl[vn]>>22)&63;

	ph0=(acc>>4)&0xFFFF;	//19..4
	ph1=(acc>>3)&0xFFFF;	//18..3
//	fn=(jx2i_smus_ctrl[vn]>>28)&7;
	
	x19=(((int)(acc<<12))>>31)&0xFFFF;	//mask for acc[19]
	
//	v=ph1^x19;
	v=jx2i_smus_sintab[ph0>>8]-32768;
//	v=sin(ph0*(1.0/65536.0)*(2.0*M_PI))*32767;
//	v=0;
	
	v=SMus_ScaleWaveAttn(v, an);
//	v=v*(1.0/(64-an));
	
//	jx2i_smus_lastval[vn]=v;

	return(v);
}
#endif

int SMus_GetStepSample(int step)
{
	int v;
	int i, j, k;

	if(step==16)
		{ SMus_Step16k(); }
	if(step==32)
		{ SMus_Step32k(); }

	v=0;

	for(i=0; i<32; i++)
	{
		v+=SMus_GetVoiceWaveRsVal[i](i);
	}

//	v=sin(jx2i_smus_usec*(220/1000000.0)*(2*M_PI))*4096;

//	printf("%lld %d\n", jx2i_smus_usec, v);

	return(v);
}

int smus_clamp16s(int v)
{
	if(v<-32768)	v=-32768;
	if(v> 32767)	v= 32767;
	return(v);
}

int smus_clamp14s(int v)
{
	if(v<-8192)		v=-8192;
	if(v> 8192)		v= 8192;
	return(v);
}

int SMus_Init(BJX2_Context *ctx)
{
	static int init=0;
	BJX2_MemSpan *sp;
	int i, j, k;
	
	if(init)
		return(0);
	init=1;
	
	for(i=0; i<32; i++)
	{
		SMus_GetVoiceWaveRsVal[i]=
			SMus_GetVoiceWaveRsVal_Rst;
		SMus_GetVoiceWaveRsVal_Mod[i]=
			SMus_GetVoiceWaveRsVal_Rst;
	}
	
	for(i=0; i<256; i++)
	{
		jx2i_smus_sintab[i]=sin((i/256.0)*2*M_PI)*32767+32768;
	}

	jx2i_smus_aupcmbuf = NULL;

	sp=BJX2_MemSpanForName(ctx, "DRAM");
	if(sp)
	{
		jx2i_smus_aupcmbuf=(u32 *)(((byte *)sp->data)+
			((0x20800000-sp->modbase)&sp->modmask));
//		jx2i_gfxcon_lconbuf=jx2i_gfxcon_conbuf+262144;
	}

	return(1);
}

#if 0
// int SMus_GetStep1ms(short *pcm)
{
	int i, j, k;
	
//	SMus_Init();
	
	for(i=0; i<16; i++)
	{
		j=SMus_GetStepSample();
		j=smus_clamp16s(j);
//		j=smus_clamp14s(j);
		pcm[i*2+0]=j;
		pcm[i*2+1]=j;
	}
	
	return(0);
}
#endif

int SMus_GetStepSamples(short *pcm, int nsamp, int step)
{
	static int lm, lb, la0, la1;
	int i, j, k;
	
//	SMus_Init();
	
	for(i=0; i<nsamp; i++)
	{
//		printf("%d: ", i);
		j=SMus_GetStepSample(step);
		j=smus_clamp16s(j);
//		j=smus_clamp14s(j);
		pcm[i*2+0]=j;
		pcm[i*2+1]=j;
	}

#if 0
	j=pcm[0*2+0];
	j=(j+lb)>>1;
	pcm[0*2+0]=j;
	pcm[0*2+1]=j;
	lb=pcm[(nsamp-1)*2+0];
#endif

#if 1
	for(i=0; i<4; i++)
	{
		k=lb+(i+1)*lm;
//		k=lb;
		k=smus_clamp16s(k);

		j=pcm[i*2+0];
//		j=((j*(i+1))+(k*(15-i)))>>4;
		j=((j*(i+1))+(k*(3-i)))>>2;
		pcm[i*2+0]=j;
		pcm[i*2+1]=j;
	}


	lb=0; la0=0; la1=0;
	for(i=0; i<16; i++)
		{ lb+=pcm[(nsamp-16+i)*2+0]; }
	for(i=0; i<8; i++)
		{ la0+=pcm[((nsamp-16)+i+0)*2+0]; la1+=pcm[((nsamp-16)+i+8)*2+0]; }
	lb>>=4;
	la0>>=3;
	la1>>=3;
	lm=(la1-la0)>>3;

	lb=pcm[(nsamp-1)*2+0];
#endif

	return(0);
}

u32 SMus_GetRegister(BJX2_Context *ctx, int reg)
{
	u32 rv;
	int vn;

	rv=0;
//	if((reg>=0x00) && (reg<0x40))
	if((reg>=0x00) && (reg<0x80))
	{
//		vn=(reg>>2)&15;
		vn=(reg>>2)&31;

		if((reg&3)==0)
			rv=jx2i_smus_ctrl[vn];
		if((reg&3)==1)
//			rv=jx2i_smus_ctrl[16+vn];
			rv=jx2i_smus_ctrl[32+vn];
		if((reg&3)==2)
			rv=jx2i_smus_ctrl[64+vn];

		if((reg&3)==3)
			rv=jx2i_smus_accum[vn]&0x000FFFFF;
	}

	return(rv);
}

int SMus_SetRegisterI(BJX2_Context *ctx, int reg, u32 val)
{
	int vn;

//	if((reg>=0x00) && (reg<0x40))
	if((reg>=0x00) && (reg<0x80))
	{
//		vn=(reg>>2)&15;
		vn=(reg>>2)&31;
		if((reg&3)==0)
		{
			jx2i_smus_ctrl[vn]=val;
			
//			if(!val)
//			{
//				SMus_GetVoiceWaveRsVal[vn]=SMus_GetVoiceWaveRsVal_Rst;
//				SMus_GetVoiceWaveRsVal_Mod[vn]=SMus_GetVoiceWaveRsVal_Mute;
//			}else
			if(1)
			{
				SMus_GetVoiceWaveRsVal[vn]=
					SMus_GetVoiceWaveRsVal_Dfl;
				SMus_GetVoiceWaveRsVal_Mod[vn]=
					SMus_GetVoiceWaveRsVal_Dfl;
				
//				if(jx2i_smus_ctrl[vn&7]&0x00200000)
				if(jx2i_smus_ctrl[vn&0x1E]&0x00200000)
				{
//					SMus_GetVoiceWaveRsVal[15|(vn&7)]=
					SMus_GetVoiceWaveRsVal[(vn&0x1E)|1]=
						SMus_GetVoiceWaveRsVal_Mute;
				}

				if(jx2i_smus_ctrl[vn&0x1E]&0x00100000)
				{
					SMus_GetVoiceWaveRsVal[(vn&0x1E)|0]=
						SMus_GetVoiceWaveRsVal_Pcm;
					SMus_GetVoiceWaveRsVal[(vn&0x1E)|1]=
						SMus_GetVoiceWaveRsVal_Mute;
				}
			}

			if(jx2i_smus_ctrl[vn&0x1E]==0)
			{
				SMus_GetVoiceWaveRsVal[vn]=SMus_GetVoiceWaveRsVal_Rst;
				SMus_GetVoiceWaveRsVal_Mod[vn]=SMus_GetVoiceWaveRsVal_Mute;
			}
		}

		if((reg&3)==1)
			jx2i_smus_ctrl[32+vn]=val;
		if((reg&3)==2)
			jx2i_smus_ctrl[64+vn]=val;
		if((reg&3)==3)
			jx2i_smus_accum[vn]=val;
		return(1);
	}
	return(0);
}

int SMus_SyncTimeUsec(BJX2_Context *ctx)
{
	s64 usc, dt;
	usc=BJX2_Interp_GetVirtualUsec(ctx);
	dt=usc-jx2i_smus_usec;
	if(dt<0)dt=0;
	if(dt>100000)
		jx2i_smus_usec=usc;
	return(0);
}

int SMus_SetRegister(BJX2_Context *ctx, int reg, u32 val)
{
	s64 usc;
	int i, j;

	i=jx2i_smus_rgbuf_epos;
	j=(i+1)&(SMUS_RGBUF_SZ-1);

	usc=BJX2_Interp_GetVirtualUsec(ctx);

	if(j!=jx2i_smus_rgbuf_spos)
	{
		jx2i_smus_rgbuf_epos=j;
		jx2i_smus_rgbuf_val[i]=val;
		jx2i_smus_rgbuf_reg[i]=reg;
		jx2i_smus_rgbuf_usc[i]=usc;
	}
	return(0);
}

int SMus_CheckApplyRegisters()
{
	u32 val;
	int reg;
	int i, j;

//	SMus_Init();

	/* If no register write queue is empty, do nothing. */
	if(jx2i_smus_rgbuf_spos==jx2i_smus_rgbuf_epos)
		return(0);

	i=jx2i_smus_rgbuf_spos;
	j=(i+1)&(SMUS_RGBUF_SZ-1);
	
	while((i!=jx2i_smus_rgbuf_epos) &&
		(jx2i_smus_usec>=jx2i_smus_rgbuf_usc[i]))
	{
		reg=jx2i_smus_rgbuf_reg[i];
		val=jx2i_smus_rgbuf_val[i];
		SMus_SetRegisterI(NULL, reg, val);
		i=j;
		j=(i+1)&(SMUS_RGBUF_SZ-1);
	}

	jx2i_smus_rgbuf_spos=i;

	return(0);
}




int BJX2_MemSndSmusCb_GetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSmusCb_GetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

s32 BJX2_MemSndSmusCb_GetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr)
{
	s64 rvq;
	int ra, rv, vn;

	ra=addr-sp->addr_base;
	
	rv=0;
	rv=SMus_GetRegister(ctx, ra>>2);

#if 0
//	if((ra>>2)>=1024)
	if(1)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			rv=SMus_GetRegister(ctx, (ra-0x1800)>>2);
			return(rv);
		}
	}
#endif

	return(rv);
}

int BJX2_MemSndSmusCb_SetByte(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSmusCb_SetWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, int val)
{
	int ra;
	ra=addr-sp->addr_base;
	return(0);
}

int BJX2_MemSndSmusCb_SetDWord(BJX2_Context *ctx,
	BJX2_MemSpan *sp, bjx2_addr addr, s32 val)
{
	int ra;
	int i;

	ra=addr-sp->addr_base;

	SMus_SetRegister(ctx, ra>>2, val);

#if 0
//	if((ra>>2)>=1024)
	if(1)
	{
		if((ra>=0x1800) && (ra<0x1A00))
		{
			SMus_SetRegister(ctx, (ra-0x1800)>>2, val);
			return(0);
		}
	}
#endif

	return(0);
}

int BJX2_MemDefineSmus(BJX2_Context *ctx,
	char *name, bjx2_addr base, bjx2_addr lim)
{
	BJX2_MemSpan *sp;
	
	sp=BJX2_MemAllocSpan(ctx);
	
	sp->name=name;
	sp->addr_base=base;
	sp->addr_lim=lim;
	sp->addr_sz=lim-base;
//	sp->data=malloc((lim-base)+8);
	
	sp->GetByte=BJX2_MemSndSmusCb_GetByte;
	sp->GetWord=BJX2_MemSndSmusCb_GetWord;
	sp->GetDWord=BJX2_MemSndSmusCb_GetDWord;
	
	sp->SetByte=BJX2_MemSndSmusCb_SetByte;
	sp->SetWord=BJX2_MemSndSmusCb_SetWord;
	sp->SetDWord=BJX2_MemSndSmusCb_SetDWord;
	
	BJX2_MemAddSpan(ctx, sp);
	return(0);
}

