u32 X3VM_ConvFp16ToFp32(u16 va)
{
	u32 frb, vb;
	int sga, exa, fra;
	int exb;
	
	sga=va>>15;
	exa=(va>>10)&31;
	fra=va&1023;
	exb=exa+(127-15);
	if(exa== 0)		exb=  0;
	if(exa==31)		exb=255;
	frb=((u32)fra)<<(23-10);
	vb=(((u32)sga)<<31) | (((u32)exb)<<23) | frb;
	return(vb);
}

u16 X3VM_ConvFp32ToFp16(u32 va)
{
	int sga, exa, fra;
	int exb, frb, vb;

	sga=va>>31;
	exa=(va>>23)&255;
	fra=va&((1<<23)-1);
	exb=exa+(15-127);
	if(!exa)	exb=0;
	if(exb<0)
		return(0);
	if(exb>=31)
		return(0x7C00|(sga<<15));
	frb=fra>>(23-10);
	
	vb=(sga<<15)|(exb<<10)|frb;
	return(vb);
}

u64 X3VM_ConvFp16ToFp64(u16 va)
{
	u64 frb, vb;
	int sga, exa, fra;
	int exb;
	
	sga=va>>15;
	exa=(va>>10)&31;
	fra=va&1023;
	exb=exa+(1023-15);
	if(exa== 0)		exb=   0;
	if(exa==31)		exb=2047;
	frb=((u64)fra)<<(52-10);
	vb=(((u64)sga)<<63) | (((u64)exb)<<52) | frb;
	return(vb);
}

u64 X3VM_ConvFp32ToFp64(u32 va)
{
	u64 frb, vb;
	int sga, exa, fra;
	int exb;
	
	sga=va>>31;
	exa=(va>>23)&255;
	fra=va&((1<<23)-1);
	exb=exa+(1023-127);
	if(exa==  0)	exb=   0;
	if(exa==255)	exb=2047;
	frb=((u64)fra)<<(52-23);
	vb=(((u64)sga)<<63) | (((u64)exb)<<52) | frb;
	return(vb);
}

u32 X3VM_ConvFp64ToFp32(u64 va)
{
	u64 fra;
	int sga, exa;
	int exb, frb, vb;

	sga=va>>63;
	exa=(va>>52)&2047;
	fra=va&((1ULL<<52)-1);
	exb=exa+(127-1023);
	if(!exa)	exb=0;
	if(exb<0)
		return(0);
	if(exb>=255)
		return(0x7F800000U|(sga<<31));
	frb=fra>>(52-23);
	
	vb=(sga<<31)|(exb<<23)|frb;
	return(vb);
}

u16 X3VM_ConvFp64ToFp16(u64 va)
{
	u32 va1;
	u16 vb;
	va1=X3VM_ConvFp64ToFp32(va);
	vb=X3VM_ConvFp32ToFp16(va1);
	return(vb);
}

u64 X3VM_Conv2xFp16To2xFp32(u32 va)
{
	u64 vn;
	vn=         X3VM_ConvFp16ToFp32(va>>16);
	vn=(vn<<32)|X3VM_ConvFp16ToFp32(va>> 0);
	return(vn);
}


void X3VM_Unpack4xFp16(float *fv, u64 va)
{
	((u32 *)fv)[0]=X3VM_ConvFp16ToFp32(va>> 0);
	((u32 *)fv)[1]=X3VM_ConvFp16ToFp32(va>>16);
	((u32 *)fv)[2]=X3VM_ConvFp16ToFp32(va>>32);
	((u32 *)fv)[3]=X3VM_ConvFp16ToFp32(va>>48);
}

u64 X3VM_Pack4xFp16(float *fv)
{
	u64 vc;
	vc=         X3VM_ConvFp32ToFp16(((u32 *)fv)[3]);
	vc=(vc<<16)|X3VM_ConvFp32ToFp16(((u32 *)fv)[2]);
	vc=(vc<<16)|X3VM_ConvFp32ToFp16(((u32 *)fv)[1]);
	vc=(vc<<16)|X3VM_ConvFp32ToFp16(((u32 *)fv)[0]);
	return(vc);
}

u64 X3VM_PADDW(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((va>>48)+(vb>>48))&0xFFFF);
	vc=(vc<<16)|(((va>>32)+(vb>>32))&0xFFFF);
	vc=(vc<<16)|(((va>>16)+(vb>>16))&0xFFFF);
	vc=(vc<<16)|(((va>> 0)+(vb>> 0))&0xFFFF);
	return(vc);
}

u64 X3VM_PSUBW(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((va>>48)-(vb>>48))&0xFFFF);
	vc=(vc<<16)|(((va>>32)-(vb>>32))&0xFFFF);
	vc=(vc<<16)|(((va>>16)-(vb>>16))&0xFFFF);
	vc=(vc<<16)|(((va>> 0)-(vb>> 0))&0xFFFF);
	return(vc);
}

u64 X3VM_PMULSLW(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((s16)(va>>48)*(s16)(vb>>48))&0xFFFF);
	vc=(vc<<16)|(((s16)(va>>32)*(s16)(vb>>32))&0xFFFF);
	vc=(vc<<16)|(((s16)(va>>16)*(s16)(vb>>16))&0xFFFF);
	vc=(vc<<16)|(((s16)(va>> 0)*(s16)(vb>> 0))&0xFFFF);
	return(vc);
}

u64 X3VM_PMULULW(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((u16)(va>>48)*(u16)(vb>>48))&0xFFFF);
	vc=(vc<<16)|(((u16)(va>>32)*(u16)(vb>>32))&0xFFFF);
	vc=(vc<<16)|(((u16)(va>>16)*(u16)(vb>>16))&0xFFFF);
	vc=(vc<<16)|(((u16)(va>> 0)*(u16)(vb>> 0))&0xFFFF);
	return(vc);
}

u64 X3VM_PMULSHW(u64 va, u64 vb)
{
	u64 vc;
	vc=         ((((s16)(va>>48)*(s16)(vb>>48))>>16)&0xFFFF);
	vc=(vc<<16)|((((s16)(va>>32)*(s16)(vb>>32))>>16)&0xFFFF);
	vc=(vc<<16)|((((s16)(va>>16)*(s16)(vb>>16))>>16)&0xFFFF);
	vc=(vc<<16)|((((s16)(va>> 0)*(s16)(vb>> 0))>>16)&0xFFFF);
	return(vc);
}

u64 X3VM_PMULUHW(u64 va, u64 vb)
{
	u64 vc;
	vc=         ((((u16)(va>>48)*(u16)(vb>>48))>>16)&0xFFFF);
	vc=(vc<<16)|((((u16)(va>>32)*(u16)(vb>>32))>>16)&0xFFFF);
	vc=(vc<<16)|((((u16)(va>>16)*(u16)(vb>>16))>>16)&0xFFFF);
	vc=(vc<<16)|((((u16)(va>> 0)*(u16)(vb>> 0))>>16)&0xFFFF);
	return(vc);
}





u64 X3VM_PADDL(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((va>>32)+(vb>>32))&0xFFFFFFFFU);
	vc=(vc<<32)|(((va>> 0)+(vb>> 0))&0xFFFFFFFFU);
	return(vc);
}

u64 X3VM_PSUBL(u64 va, u64 vb)
{
	u64 vc;
	vc=         (((va>>32)-(vb>>32))&0xFFFFFFFFU);
	vc=(vc<<32)|(((va>> 0)-(vb>> 0))&0xFFFFFFFFU);
	return(vc);
}

u64 X3VM_PADDF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=tv0[0]+tv1[0];
	tv2[1]=tv0[1]+tv1[1];
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PSUBF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=tv0[0]-tv1[0];
	tv2[1]=tv0[1]-tv1[1];
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PMULF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=tv0[0]*tv1[0];
	tv2[1]=tv0[1]*tv1[1];
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PDIVF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=0;	tv2[1]=0;
	if((tv1[0]!=0) && (tv1[0]==tv1[0]))		tv2[0]=1.0/tv1[0];
	if((tv1[1]!=0) && (tv1[1]==tv1[1]))		tv2[1]=1.0/tv1[1];
	tv2[0]=tv0[0]*tv2[0];
	tv2[1]=tv0[1]*tv2[1];
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PADDH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=tv0[0]+tv1[0];
	tv2[1]=tv0[1]+tv1[1];
	tv2[2]=tv0[2]+tv1[2];
	tv2[3]=tv0[3]+tv1[3];
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_PSUBH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=tv0[0]-tv1[0];
	tv2[1]=tv0[1]-tv1[1];
	tv2[2]=tv0[2]-tv1[2];
	tv2[3]=tv0[3]-tv1[3];
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_PMULH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=tv0[0]*tv1[0];
	tv2[1]=tv0[1]*tv1[1];
	tv2[2]=tv0[2]*tv1[2];
	tv2[3]=tv0[3]*tv1[3];
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_PDIVH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=0;	tv2[1]=0;
	tv2[2]=0;	tv2[3]=0;
	if((tv1[0]!=0) && (tv1[0]==tv1[0]))		tv2[0]=1.0/tv1[0];
	if((tv1[1]!=0) && (tv1[1]==tv1[1]))		tv2[1]=1.0/tv1[1];
	if((tv1[2]!=0) && (tv1[2]==tv1[2]))		tv2[2]=1.0/tv1[2];
	if((tv1[3]!=0) && (tv1[3]==tv1[3]))		tv2[3]=1.0/tv1[3];
	tv2[0]=tv0[0]*tv2[0];
	tv2[1]=tv0[1]*tv2[1];
	tv2[2]=tv0[2]*tv2[2];
	tv2[3]=tv0[3]*tv2[3];
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_PMINF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=fmin(tv0[0], tv1[0]);
	tv2[1]=fmin(tv0[1], tv1[1]);
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PMINH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=fmin(tv0[0], tv1[0]);
	tv2[1]=fmin(tv0[1], tv1[1]);
	tv2[2]=fmin(tv0[2], tv1[2]);
	tv2[3]=fmin(tv0[3], tv1[3]);
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_PMAXF(u64 va, u64 vb)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	memcpy(tv1, &vb, 8);
	tv2[0]=fmax(tv0[0], tv1[0]);
	tv2[1]=fmax(tv0[1], tv1[1]);
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PMAXH(u64 va, u64 vb)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	X3VM_Unpack4xFp16(tv1, vb);
	tv2[0]=fmax(tv0[0], tv1[0]);
	tv2[1]=fmax(tv0[1], tv1[1]);
	tv2[2]=fmax(tv0[2], tv1[2]);
	tv2[3]=fmax(tv0[3], tv1[3]);
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_FADD(u64 va, u64 vb)
{
	double fa, fb, fc;
	u64 vc;
	memcpy(&fa, &va, 8);
	memcpy(&fb, &vb, 8);
	fc=fa+fb;
	memcpy(&vc, &fc, 8);
	return(vc);
}

u64 X3VM_FSUB(u64 va, u64 vb)
{
	double fa, fb, fc;
	u64 vc;
	memcpy(&fa, &va, 8);
	memcpy(&fb, &vb, 8);
	fc=fa-fb;
	memcpy(&vc, &fc, 8);
	return(vc);
}

u64 X3VM_FMUL(u64 va, u64 vb)
{
	double fa, fb, fc;
	u64 vc;
	memcpy(&fa, &va, 8);
	memcpy(&fb, &vb, 8);
	fc=fa*fb;
	memcpy(&vc, &fc, 8);
	return(vc);
}

u64 X3VM_FDIV(u64 va, u64 vb)
{
	double fa, fb, fc;
	u64 vc;
	
	if(vb==0)
	{
		vc=2047LL<<52;
		return(vc);
	}
	
	memcpy(&fa, &va, 8);
	memcpy(&fb, &vb, 8);
	fc=fa/fb;
	memcpy(&vc, &fc, 8);
	return(vc);
}

u64 X3VM_PACKUU(u64 va, u64 vb)
{
	u64 vc;
	vc=(va&0xFFFFFFFF00000000ULL)|(vb>>32);
	return(vc);
}

u64 X3VM_PACKLL(u64 va, u64 vb)
{
	u64 vc;
	vc=(va<<32)|(vb&0x00000000FFFFFFFFULL);
	return(vc);
}

u64 X3VM_PACKUL(u64 va, u64 vb)
{
	u64 vc;
	vc=(va&0xFFFFFFFF00000000ULL)|(vb&0x00000000FFFFFFFFULL);
	return(vc);
}

u64 X3VM_PACKLU(u64 va, u64 vb)
{
	u64 vc;
	vc=(va<<32)|(vb>>32);
	return(vc);
}

int X3VM_FCMP(u64 va, u64 vb)
{
	if(va==vb)
	{
		if(((va>>52)&2047)==2047)
		{
			if((va>>48)&15)
				return(-2);
		}
		return(0);
	}

	if(va>>63)
	{
		if(vb>>63)
		{
			if(va>vb)
				return(-1);
			return(1);
		}else
		{
			return(-1);
		}
	}else
	{
		if(vb>>63)
		{
			return(1);
		}else
		{
			if(va>vb)
				return(1);
			return(-1);
		}
	}
}


int X3VM_FCMPF(u32 va, u32 vb)
{
	if(va==vb)
	{
		if(((va>>23)&255)==255)
		{
			if((va>>19)&15)
				return(-2);
		}
		return(0);
	}

	if(va>>31)
	{
		if(vb>>31)
		{
			if(va>vb)
				return(-1);
			return(1);
		}else
		{
			return(-1);
		}
	}else
	{
		if(vb>>31)
		{
			return(1);
		}else
		{
			if(va>vb)
				return(1);
			return(-1);
		}
	}
}

int X3VM_FCMPH(u16 va, u16 vb)
{
	if(va==vb)
	{
		if(((va>>10)&31)==31)
		{
			if((va>>6)&15)
				return(-2);
		}
		return(0);
	}

	if(va>>15)
	{
		if(vb>>15)
		{
			if(va>vb)
				return(-1);
			return(1);
		}else
		{
			return(-1);
		}
	}else
	{
		if(vb>>15)
		{
			return(1);
		}else
		{
			if(va>vb)
				return(1);
			return(-1);
		}
	}
}

double x3vm_ssqrt(double x)
{
	if(x<0)
		return(-sqrt(-x));
	return(sqrt(x));
}

u64 X3VM_PSQRTF(u64 va)
{
	float tv0[2], tv1[2], tv2[2];
	u64 vc;
	memcpy(tv0, &va, 8);
	tv2[0]=x3vm_ssqrt(tv0[0]);
	tv2[1]=x3vm_ssqrt(tv0[1]);
	memcpy(&vc, tv2, 8);
	return(vc);
}

u64 X3VM_PSQRTH(u64 va)
{
	float tv0[4], tv1[4], tv2[4];
	u64 vc;
	X3VM_Unpack4xFp16(tv0, va);
	tv2[0]=x3vm_ssqrt(tv0[0]);
	tv2[1]=x3vm_ssqrt(tv0[1]);
	tv2[2]=x3vm_ssqrt(tv0[2]);
	tv2[3]=x3vm_ssqrt(tv0[3]);
	vc=X3VM_Pack4xFp16(tv2);
	return(vc);
}

u64 X3VM_FSQRT(u64 va)
{
	double fx, fy;
	u64 vc;
	memcpy(&fx, &va, 8);
	fy=x3vm_ssqrt(fx);
	memcpy(&vc, &fy, 8);
	return(vc);
}
