double btic1g_ssqrt(double f)
{
	return((f>=0)?sqrt(f):(-sqrt(-f)));
}

double btic1g_ssqr(double f)
{
	return((f>=0)?(f*f):(-f*f));
}

double btic1g_slog(double f)
{
	return((f>=0)?log(f+1):(-log(-f+1)));
}

double btic1g_sexp(double f)
{
	return((f>=0)?exp(f)-1:(-exp(-f)+1));
}

void BTIC1H_InitQuant()
{
	static int init=0;
	int i, j, k;

	if(init)
		return;
	init=1;
}

#if 0
void BTIC1H_EncSwapCommandMTF(BTIC1H_Context *ctx, int idx)
{
	int i, j, k;
	switch(idx)
	{
	case 0:
		break;
	case 1:
		i=ctx->cmdwin[0];	j=ctx->cmdwin[1];
		ctx->cmdwin[0]=j;	ctx->cmdwin[1]=i;
		ctx->cmdidx[j]=0;	ctx->cmdidx[i]=1;
		break;
	case 2:
		i=ctx->cmdwin[1];	j=ctx->cmdwin[2];
		ctx->cmdwin[1]=j;	ctx->cmdwin[2]=i;
		ctx->cmdidx[j]=1;	ctx->cmdidx[i]=2;
		break;
	case 3:
		i=ctx->cmdwin[1];	j=ctx->cmdwin[2];
		k=ctx->cmdwin[3];	ctx->cmdwin[1]=k;
		ctx->cmdwin[2]=i;	ctx->cmdwin[3]=j;
		ctx->cmdidx[k]=1;	ctx->cmdidx[i]=2;
		ctx->cmdidx[j]=3;
		break;
	default:
		i=ctx->cmdwin[idx];
		for(j=idx; j>1; j--)
		{
			k=ctx->cmdwin[j-1];
			ctx->cmdwin[j]=k;
			ctx->cmdidx[k]=j;
		}
		ctx->cmdwin[1]=i;
		ctx->cmdidx[i]=1;
		break;
	}
}

void BTIC1H_EncRotateMTF(BTIC1H_Context *ctx)
{
	int i, j, k;

	j=ctx->cmdwin[15];
	ctx->cmdidx[j]=-1;

	for(i=15; i>1; i--)
	{
		j=ctx->cmdwin[i-1];
		ctx->cmdwin[i]=j;
		ctx->cmdidx[j]=i;
	}
}
#endif

#if 0
int BTIC1H_EmitCommandCode(BTIC1H_Context *ctx, int cmd)
{
	int bit0;
	int i, j, k;
	
	ctx->cnt_cmds++;
	
	j=ctx->cmdidx[cmd];
	i=(byte)(j-ctx->cmdwpos);
	
	if((j>=0) && (i<16))
	{
		bit0=ctx->bs_bits;
		BTIC1H_Rice_WriteAdSRice(ctx, i, &(ctx->rk_cmdidx));
		ctx->bits_cmdidx+=ctx->bs_bits-bit0;

		BTIC1H_EncSwapCommandMTF(ctx, i);
#if 0
		if(i)
		{
			j=(byte)(--ctx->cmdwpos);
			k=ctx->cmdwin[j&15];
			if(ctx->cmdidx[k]==j)
				ctx->cmdidx[k]=-1;

			ctx->cmdwin[j&15]=cmd;
			ctx->cmdidx[cmd]=j;
		}
#endif
		return(0);
	}
	
	bit0=ctx->bs_bits;

	BTIC1H_Rice_WriteAdSRice(ctx, -1, &(ctx->rk_cmdidx));
	BTIC1H_Rice_WriteAdRice(ctx, cmd, &(ctx->rk_cmdabs));
	
	ctx->bits_cmdabs+=ctx->bs_bits-bit0;

	BTIC1H_EncRotateMTF(ctx);
	ctx->cmdwin[1]=cmd;
	ctx->cmdidx[cmd]=1;

//	j=(byte)(--ctx->cmdwpos);
//	k=ctx->cmdwin[j&15];
//	if(ctx->cmdidx[k]==j)
//		ctx->cmdidx[k]=-1;

//	ctx->cmdwin[j&15]=cmd;
//	ctx->cmdidx[cmd]=j;

	return(0);
}
#endif

#if 1
int BTIC1H_EmitCommandCode(BTIC1H_Context *ctx, int cmd)
{
	int bit0;
	int i, j, k;
	
	ctx->cnt_cmds++;

	if((cmd<0) || (cmd>=64))
		*(int *)-1=-1;

	j=ctx->cmdidx[cmd];
	i=(byte)(j-ctx->cmdwpos);
	if(j>0)
	{
		k=ctx->cmdwin[j&15];
		if(k!=cmd)
			j=-1;
//			*(int *)-1=-1;
	}
	
#if 1
	if((j>=0) && (i>=0) && (i<16))
	{
		if(i<0)
			return(-1);
	
		bit0=ctx->bs_bits;
//		BTIC1H_Rice_WriteBit(ctx, 0);
//		BTIC1H_Rice_WriteAdRice(ctx, i, &(ctx->rk_cmdidx));
//		BTIC1H_Rice_WriteAdSRice(ctx, i, &(ctx->rk_cmdidx));
		BTIC1H_Rice_WriteAdRice(ctx, i+1, &(ctx->rk_cmdidx));
		ctx->bits_cmdidx+=ctx->bs_bits-bit0;

#if 1
		if(i>0)
		{
			j=(byte)(ctx->cmdwpos);
			k=ctx->cmdwin[(j+i-1)&15];
			ctx->cmdwin[(j+i-1)&15]=cmd;
			ctx->cmdwin[(j+i+0)&15]=k;
			ctx->cmdidx[cmd]=(byte)(j+i-1);
			ctx->cmdidx[k]=(byte)(j+i);
		}
#endif

#if 0
		if(i==0)
		{
		}else if(i==1)
		{
			j=(byte)(ctx->cmdwpos);
			k=ctx->cmdwin[j&15];
			ctx->cmdwin[j&15]=cmd;
			ctx->cmdwin[(j+1)&15]=k;
			ctx->cmdidx[cmd]=j;
			ctx->cmdidx[k]=(byte)(j+1);
		}else
		{
			j=(byte)(--ctx->cmdwpos);
			k=ctx->cmdwin[j&15];

			if((k<0) || (k>=64))
				*(int *)-1=-1;
			if((ctx->cmdidx[k]&15)==(j&15))
				ctx->cmdidx[k]=-1;

			ctx->cmdwin[j&15]=cmd;
			ctx->cmdidx[cmd]=j;
		}
#endif
		return(0);
	}
#endif
	
	bit0=ctx->bs_bits;

//	BTIC1H_Rice_WriteAdSRice(ctx, -1, &(ctx->rk_cmdidx));
	BTIC1H_Rice_WriteAdRice(ctx, 0, &(ctx->rk_cmdidx));
	BTIC1H_Rice_WriteAdRice(ctx, cmd, &(ctx->rk_cmdabs));
//	BTIC1H_Rice_Write8Bits(ctx, cmd);
	
	ctx->bits_cmdabs+=ctx->bs_bits-bit0;

	j=(byte)(--ctx->cmdwpos);
	k=ctx->cmdwin[j&15];

	if((k<0) || (k>=64))
		*(int *)-1=-1;
	if((ctx->cmdidx[k]&15)==(j&15))
		ctx->cmdidx[k]=-1;

	ctx->cmdwin[j&15]=cmd;
	ctx->cmdidx[cmd]=j;

	return(0);
}
#endif

int BTIC1H_EmitDeltaYUV(BTIC1H_Context *ctx,
	int cy, int cu, int cv)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->cy;
	du=cu-ctx->cu;
	dv=cv-ctx->cv;

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;

	ctx->cy+=qdy*ctx->qfy;
	ctx->cu+=qdu*ctx->qfuv;
	ctx->cv+=qdv*ctx->qfuv;

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_duv));
	BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_duv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;
	
//	if((ctx->cy<0) || (ctx->cy>255) ||
//		(ctx->cu<0) || (ctx->cu>255) ||
//		(ctx->cv<0) || (ctx->cv>255))
//	{
//		*(int *)-1=-1;
//	}
	
	return(0);
}

int BTIC1H_EmitDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;

	dd=cd-ctx->cd;
//	qdd=dd/ctx->qfd;
	qdd=(dd*ctx->fx_qfd+2048)>>12;
	ctx->cd+=qdd*ctx->qfd;
	
	BTIC1H_EmitDeltaYUV(ctx, cy, cu, cv);	
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdd, &(ctx->rk_ddy));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

//	if((ctx->cd<0) || (ctx->cd>255))
//	{
//		*(int *)-1=-1;
//	}

	return(0);
}


int BTIC1H_EmitDeltaQfYUVD(BTIC1H_Context *ctx,
	int cy, int cuv, int cd)
{
	int dy, duv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->qfy;
	duv=cuv-ctx->qfuv;
	dd=cd-ctx->qfd;

	ctx->qfy=cy;
	ctx->qfuv=cuv;
	ctx->qfd=cd;

	ctx->fx_qfy =4096/cy;
	ctx->fx_qfuv=4096/cuv;
	ctx->fx_qfd =4096/cd;

	BTIC1H_Rice_WriteAdSRice(ctx, dy, &(ctx->rk_qfy));
	BTIC1H_Rice_WriteAdSRice(ctx, duv, &(ctx->rk_qfuv));
	BTIC1H_Rice_WriteAdSRice(ctx, dd, &(ctx->rk_qfdy));
	return(0);
}


int BTIC1H_CheckZeroDeltaYUV(BTIC1H_Context *ctx,
	int cy, int cu, int cv)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->cy;
	du=cu-ctx->cu;
	dv=cv-ctx->cv;

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;

	if(qdy || qdu || qdv)
		return(0);

#if 0
	if(qdy<(-1)||(qdy>1))
		return(0);
	if(qdu<(-1)||(qdu>1))
		return(0);
	if(qdv<(-1)||(qdv>1))
		return(0);
#endif
	return(1);
}

int BTIC1H_CheckZeroDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->cy;
	du=cu-ctx->cu;
	dv=cv-ctx->cv;
	dd=cd-ctx->cd;

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;
	qdd=(dd*ctx->fx_qfd +2048)>>12;

	if(qdy || qdu || qdv || qdd)
		return(0);

#if 0
	if(qdy<(-1)||(qdy>1))
		return(0);
	if(qdu<(-1)||(qdu>1))
		return(0);
	if(qdv<(-1)||(qdv>1))
		return(0);
	if(qdd<(-1)||(qdd>1))
		return(0);
#endif
	return(1);
}

int BTIC1H_EncodeBlocksCtx(BTIC1H_Context *ctx,
	byte *blks, byte *lblks, int nblks, int stride,
	int *rnblks)
{
	byte *cs2, *csl2;
	byte *cs, *cse, *csl, *cs1, *cs1e;
	int bit0;
	int cy, cu, cv, cd, cm;
	int cy1, cu1, cv1, cd1, cm1;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
//	BTIC1H_InitQuant();
	
	cs=blks; cse=cs+nblks*stride; csl=lblks;
	
//	ctx->cy=0;	ctx->cu=0;
//	ctx->cv=0;	ctx->cd=0;

//	ctx->qfy=1;	ctx->qfuv=1; ctx->qfd=1;
//	ctx->qfy=4;	ctx->qfuv=4; ctx->qfd=4;

	BTIC1H_EmitCommandCode(ctx, 0x04);
	BTIC1H_EmitDeltaQfYUVD(ctx, 2, 4, 2);

	while(cs<cse)
	{
		cy=cs[0]; cu=cs[1]; cv=cs[2]; cd=cs[3];
		
		if(cd)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[3];
					if(!cd1)
						break;
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
//				l=1;

//				BTIC1H_EmitCommandCode(ctx, 0x15);

#if 1
				BTIC1H_EmitCommandCode(ctx, 0x15);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
//					BTIC1H_Rice_Write8Bits(ctx, cs[4]);
//					BTIC1H_Rice_Write8Bits(ctx, cs[5]);
//					BTIC1H_Rice_Write8Bits(ctx, cs[6]);
//					BTIC1H_Rice_Write8Bits(ctx, cs[7]);
					BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x4+=32;
					cs+=stride;
				}
				continue;
#endif
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x05);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
//			BTIC1H_Rice_Write8Bits(ctx, cs[4]);
//			BTIC1H_Rice_Write8Bits(ctx, cs[5]);
//			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
//			BTIC1H_Rice_Write8Bits(ctx, cs[7]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4+=32;
			cs+=stride;
			continue;
		}
		
		cm=cs[4];
		cd=cs[5];
		
		if(cm==0)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUV(ctx, cy, cu, cv))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=0))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2];
					if(!BTIC1H_CheckZeroDeltaYUV(ctx, cy1, cu1, cv1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x10);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				cs+=l*stride;
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x00);
			BTIC1H_EmitDeltaYUV(ctx, cy, cu, cv);
			cs+=stride;
			continue;
		}

		if(cm==1)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=1))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x11);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write8Bits(ctx, cs[6]);
					ctx->bits_pix2x2+=8;
					cs+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x01);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
			ctx->bits_pix2x2+=8;
			cs+=stride;
			continue;
		}

		if((cm==8) || (cm==9))
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=cm))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x12+(cm-8));
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write4Bits(ctx, cs[6]);
					ctx->bits_pix2x1+=4;
					cs+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x02+(cm-8));
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write4Bits(ctx, cs[6]);
			ctx->bits_pix2x1+=4;
			cs+=stride;
			continue;
		}

		if((cm==10) || (cm==11))
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=cm))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x16+(cm-10));
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
//					BTIC1H_Rice_Write8Bits(ctx, cs[6]);
//					BTIC1H_Rice_Write8Bits(ctx, cs[7]);
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x2+=16;
					cs+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x06+(cm-10));
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
//			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
//			BTIC1H_Rice_Write8Bits(ctx, cs[7]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x2+=16;
			cs+=stride;
			continue;
		}
	}

	BTIC1H_EmitCommandCode(ctx, 0x20);
	
	*rnblks=(cs-blks)/stride;
	return(0);
}

void BTIC1H_SetupContextInitial(BTIC1H_Context *ctx)
{
	ctx->cy=0;	ctx->cu=0;
	ctx->cv=0;	ctx->cd=0;
	
	ctx->cdy=0;		ctx->cdu=0;		ctx->cdv=0;

	ctx->qfy=1;		ctx->qfuv=1;	ctx->qfd=1;
	ctx->qfdy=1;	ctx->qfduv=1;

	ctx->qfay=1;	ctx->qfauv=1;	ctx->qfad=1;
	ctx->qfady=1;	ctx->qfaduv=1;

	ctx->rk_cmdidx=2;
	ctx->rk_cmdabs=4;
	ctx->rk_cmdcnt=2;

	ctx->rk_dy=2;
	ctx->rk_duv=2;
	ctx->rk_ddy=2;
	ctx->rk_dduv=2;

	ctx->rk_ay=5;
	ctx->rk_auv=5;
	ctx->rk_ady=5;
	ctx->rk_aduv=5;

	ctx->rk_qfy=3;
	ctx->rk_qfuv=3;
	ctx->rk_qfdy=3;
	ctx->rk_qfduv=3;
}

byte *BTIC1H_EncodeBlocksBuffer(byte *obuf,
	byte *blks, byte *lblks, int nblks, int stride,
	int obmsz, int *rnblks)
{
	BTIC1H_Context tctx;
	BTIC1H_Context *ctx;
	int i;
	
	ctx=&tctx;
	memset(ctx, 0, sizeof(BTIC1H_Context));
	
	BTIC1H_SetupContextInitial(ctx);
	
	for(i=0; i<64; i++)
		ctx->cmdidx[i]=-1;
	
	BTIC1H_Rice_SetupWrite(ctx, obuf, obmsz);
	
	BTIC1H_EncodeBlocksCtx(ctx, blks, lblks, nblks, stride, rnblks);
	
	BTIC1H_Rice_FlushBits(ctx);
	return(ctx->bs_ct);
}
