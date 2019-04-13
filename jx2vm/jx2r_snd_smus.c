s64 BJX2_Interp_GetVirtualUsec(BJX2_Context *ctx);


u32 jx2i_smus_ctrl[32];		//SMus Control Registers
u32 jx2i_smus_accum[32];	//SMus Accumulators
s64 jx2i_smus_usec;			//microsecond timer

// int jx2i_smus_lastval[16];	//SMus Last Value

#define SMUS_RGBUF_SZ	4096

u32		jx2i_smus_rgbuf_val[SMUS_RGBUF_SZ];	//register ring buffer value
int		jx2i_smus_rgbuf_reg[SMUS_RGBUF_SZ];	//register ring buffer ID
s64		jx2i_smus_rgbuf_usc[SMUS_RGBUF_SZ];	//register ring buffer usec

short jx2i_smus_rgbuf_spos;
short jx2i_smus_rgbuf_epos;

u16		jx2i_smus_sintab[256];

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
	jx2i_smus_accum[ 0]+=jx2i_smus_ctrl[ 0];
	jx2i_smus_accum[ 1]+=jx2i_smus_ctrl[ 1];
	jx2i_smus_accum[ 2]+=jx2i_smus_ctrl[ 2];
	jx2i_smus_accum[ 3]+=jx2i_smus_ctrl[ 3];
	jx2i_smus_accum[ 4]+=jx2i_smus_ctrl[ 4];
	jx2i_smus_accum[ 5]+=jx2i_smus_ctrl[ 5];

	jx2i_smus_accum[ 8]+=jx2i_smus_ctrl[ 8];
	jx2i_smus_accum[ 9]+=jx2i_smus_ctrl[ 9];
	jx2i_smus_accum[10]+=jx2i_smus_ctrl[10];
	jx2i_smus_accum[11]+=jx2i_smus_ctrl[11];
	jx2i_smus_accum[12]+=jx2i_smus_ctrl[12];
	jx2i_smus_accum[13]+=jx2i_smus_ctrl[13];

	jx2i_smus_accum[ 7]+=jx2i_smus_ctrl[ 7];
	jx2i_smus_accum[15]+=jx2i_smus_ctrl[15];
	
	jx2i_smus_usec+=16;
	SMus_CheckApplyRegisters();
}

void SMus_Step32k()
{
	jx2i_smus_accum[ 0]+=jx2i_smus_ctrl[ 0]<<1;
	jx2i_smus_accum[ 1]+=jx2i_smus_ctrl[ 1]<<1;
	jx2i_smus_accum[ 2]+=jx2i_smus_ctrl[ 2]<<1;
	jx2i_smus_accum[ 3]+=jx2i_smus_ctrl[ 3]<<1;
	jx2i_smus_accum[ 4]+=jx2i_smus_ctrl[ 4]<<1;
	jx2i_smus_accum[ 5]+=jx2i_smus_ctrl[ 5]<<1;
	jx2i_smus_accum[ 6]+=jx2i_smus_ctrl[ 6]<<1;
	jx2i_smus_accum[ 7]+=jx2i_smus_ctrl[ 7]<<1;

	jx2i_smus_accum[ 8]+=jx2i_smus_ctrl[ 8]<<1;
	jx2i_smus_accum[ 9]+=jx2i_smus_ctrl[ 9]<<1;
	jx2i_smus_accum[10]+=jx2i_smus_ctrl[10]<<1;
	jx2i_smus_accum[11]+=jx2i_smus_ctrl[11]<<1;
	jx2i_smus_accum[12]+=jx2i_smus_ctrl[12]<<1;
	jx2i_smus_accum[13]+=jx2i_smus_ctrl[13]<<1;
	jx2i_smus_accum[14]+=jx2i_smus_ctrl[14]<<1;
	jx2i_smus_accum[15]+=jx2i_smus_ctrl[15]<<1;

	jx2i_smus_usec+=32;
	SMus_CheckApplyRegisters();
}

void SMus_Step16k()
{
	jx2i_smus_accum[ 0]+=jx2i_smus_ctrl[ 0]<<2;
	jx2i_smus_accum[ 1]+=jx2i_smus_ctrl[ 1]<<2;
	jx2i_smus_accum[ 2]+=jx2i_smus_ctrl[ 2]<<2;
	jx2i_smus_accum[ 3]+=jx2i_smus_ctrl[ 3]<<2;
	jx2i_smus_accum[ 4]+=jx2i_smus_ctrl[ 4]<<2;

	jx2i_smus_accum[ 5]+=jx2i_smus_ctrl[ 5]<<2;
	jx2i_smus_accum[ 6]+=jx2i_smus_ctrl[ 6]<<2;
	jx2i_smus_accum[ 7]+=jx2i_smus_ctrl[ 7]<<2;

	jx2i_smus_accum[ 8]+=jx2i_smus_ctrl[ 8]<<2;
	jx2i_smus_accum[ 9]+=jx2i_smus_ctrl[ 9]<<2;
	jx2i_smus_accum[10]+=jx2i_smus_ctrl[10]<<2;
	jx2i_smus_accum[11]+=jx2i_smus_ctrl[11]<<2;
	jx2i_smus_accum[12]+=jx2i_smus_ctrl[12]<<2;

	jx2i_smus_accum[13]+=jx2i_smus_ctrl[13]<<2;
	jx2i_smus_accum[14]+=jx2i_smus_ctrl[14]<<2;
	jx2i_smus_accum[15]+=jx2i_smus_ctrl[15]<<2;

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

int (*SMus_GetVoiceWaveRsVal[16])(int vn);
int (*SMus_GetVoiceWaveRsVal_Mod[16])(int vn);

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
	int an;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
	vn=vnfl&15;
	
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

	if(((jx2i_smus_ctrl[vn&7]>>21)&1) && !(vnfl&16))
	{
		if(vn&8)
		{
			/* Mute 8-15 if Linked. */
			return(0);
		}
		
		am=SMus_GetVoiceAttn(vn|8);
		vm=SMus_GetVoiceWaveRsVal_Mod[vn|8](vn|8|16);
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
	
	switch(fn)
	{
	case 0:
//		v=ph1^x19;
		v=jx2i_smus_sintab[ph0>>8];
		break;
	case 1:
//		v=ph1^x19;
		v=jx2i_smus_sintab[ph0>>8];
		if(v<0x8000)v=0x8000;
		break;
	case 2:
//		v=ph1^x19;
		v=jx2i_smus_sintab[ph0>>8];
		if(v<0x8000)v^=0xFFFF;
		break;
	case 3:
//		v=ph1;
//		v=jx2i_smus_sintab[ph0>>8]^x19;
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
int SMus_GetVoiceWaveRsVal_Car0(int vnfl)
{
	int fn, acc, v, vn, vm, am;
	int an;
	int ph0, ph1, ph2, ph3;
	int x19, x18;
	
	vn=vnfl&15;
		
	acc=jx2i_smus_accum[vn];

	if((jx2i_smus_ctrl[vn&7]>>21)&1)
	{
		am=SMus_GetVoiceAttn(vn|8);
		vm=SMus_GetVoiceWaveRsVal_Mod[vn|8](vn|8|16);
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
	
	vn=vnfl&15;
		
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

int SMus_GetStepSample(int step)
{
	int v;
	int i, j, k;

	if(step==16)
		{ SMus_Step16k(); }
	if(step==32)
		{ SMus_Step32k(); }

	v=0;
//	for(i=0; i<16; i++)
//	{
//		v+=SMus_GetVoiceWaveRsVal[i](i);
//	}

	v+=SMus_GetVoiceWaveRsVal[ 0]( 0);
	v+=SMus_GetVoiceWaveRsVal[ 1]( 1);
	v+=SMus_GetVoiceWaveRsVal[ 2]( 2);
	v+=SMus_GetVoiceWaveRsVal[ 3]( 3);

	v+=SMus_GetVoiceWaveRsVal[ 4]( 4);


#if 1
	v+=SMus_GetVoiceWaveRsVal[ 8]( 8);
	v+=SMus_GetVoiceWaveRsVal[ 9]( 9);
	v+=SMus_GetVoiceWaveRsVal[10](10);
	v+=SMus_GetVoiceWaveRsVal[11](11);
	v+=SMus_GetVoiceWaveRsVal[12](12);
#endif

#if 1
	v+=SMus_GetVoiceWaveRsVal[ 5]( 5);
	v+=SMus_GetVoiceWaveRsVal[ 6]( 6);
	v+=SMus_GetVoiceWaveRsVal[ 7]( 7);

	v+=SMus_GetVoiceWaveRsVal[13](13);
	v+=SMus_GetVoiceWaveRsVal[14](14);
	v+=SMus_GetVoiceWaveRsVal[15](15);
#endif

//	v=sin(jx2i_smus_usec*(220/1000000.0)*(2*M_PI))*4096;

//	printf("%lld %d\n", jx2i_smus_usec, v);

	return(v);
}

int smus_clamp16s(int v)
{
//	if(((s16)v)!=v)
//	{
//		if(v<0)
//			return(-32768);
//		return(32767);
//	}
	if(v<-32768)	v=-32768;
	if(v> 32767)	v= 32767;
	return(v);
}

int smus_clamp14s(int v)
{
//	if(((s16)v)!=v)
//	{
//		if(v<0)
//			return(-32768);
//		return(32767);
//	}
	if(v<-8192)		v=-8192;
	if(v> 8192)		v= 8192;
	return(v);
}

int SMus_Init()
{
	static int init=0;
	int i, j, k;
	
	if(init)
		return(0);
	init=1;
	
	for(i=0; i<16; i++)
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

	return(1);
}

#if 0
// int SMus_GetStep1ms(short *pcm)
{
	int i, j, k;
	
	SMus_Init();
	
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
	int i, j, k;
	
	SMus_Init();
	
	for(i=0; i<nsamp; i++)
	{
//		printf("%d: ", i);
		j=SMus_GetStepSample(step);
		j=smus_clamp16s(j);
//		j=smus_clamp14s(j);
		pcm[i*2+0]=j;
		pcm[i*2+1]=j;
	}

	return(0);
}

u32 SMus_GetRegister(BJX2_Context *ctx, int reg)
{
	u32 rv;
	int vn;

	rv=0;
	if((reg>=0x00) && (reg<0x40))
	{
		vn=(reg>>2)&15;

		if((reg&3)==0)
			rv=jx2i_smus_ctrl[vn];
		if((reg&3)==1)
			rv=jx2i_smus_ctrl[16+vn];
		if((reg&3)==3)
			rv=jx2i_smus_accum[vn]&0x000FFFFF;
	}

	return(rv);
}

int SMus_SetRegisterI(BJX2_Context *ctx, int reg, u32 val)
{
	int vn;

	if((reg>=0x00) && (reg<0x40))
	{
		vn=(reg>>2)&15;
		if((reg&3)==0)
		{
			jx2i_smus_ctrl[vn]=val;
			
			if(!val)
			{
				SMus_GetVoiceWaveRsVal[vn]=SMus_GetVoiceWaveRsVal_Rst;
				SMus_GetVoiceWaveRsVal_Mod[vn]=SMus_GetVoiceWaveRsVal_Mute;
			}else
			{
#if 0
				switch((val>>28)&15)
				{
				case 0:
					SMus_GetVoiceWaveRsVal[vn]=
						SMus_GetVoiceWaveRsVal_Mod0;
					if((val&0x00200000) && !(vn&8))
					{
						SMus_GetVoiceWaveRsVal[vn]=
							SMus_GetVoiceWaveRsVal_Car0;
					}
					SMus_GetVoiceWaveRsVal_Mod[vn]=
						SMus_GetVoiceWaveRsVal_Mod0;
					break;

				default:
					SMus_GetVoiceWaveRsVal[vn]=
						SMus_GetVoiceWaveRsVal_Dfl;
					SMus_GetVoiceWaveRsVal_Mod[vn]=
						SMus_GetVoiceWaveRsVal_Dfl;
					break;
				}
#else
				SMus_GetVoiceWaveRsVal[vn]=
					SMus_GetVoiceWaveRsVal_Dfl;
				SMus_GetVoiceWaveRsVal_Mod[vn]=
					SMus_GetVoiceWaveRsVal_Dfl;
#endif
				
				if(jx2i_smus_ctrl[vn&7]&0x00200000)
				{
					SMus_GetVoiceWaveRsVal[8|(vn&7)]=
						SMus_GetVoiceWaveRsVal_Mute;
				}
			}
		}

		if((reg&3)==1)
			jx2i_smus_ctrl[16+vn]=val;
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

	SMus_Init();

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
