/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

BTIC1H_Context *btic1h_context_free=NULL;


BTIC1H_Context *BTIC1H_AllocContext(void)
{
	BTIC1H_Context *tmp;

	tmp=btic1h_context_free;
	if(tmp)
	{
		btic1h_context_free=tmp->wqnext;
		memset(tmp, 0, sizeof(BTIC1H_Context));
		return(tmp);
	}
	
	tmp=malloc(sizeof(BTIC1H_Context));
	memset(tmp, 0, sizeof(BTIC1H_Context));
	return(tmp);
}

int BTIC1H_FreeContextF(BTIC1H_Context *ctx)
{
	ctx->wqnext=btic1h_context_free;
	btic1h_context_free=ctx;
	return(0);
}

int BTIC1H_DestroyContext(BTIC1H_Context *ctx)
{
	if(ctx->blks)free(ctx->blks);
	if(ctx->lblks)free(ctx->lblks);
	BTIC1H_FreeContextF(ctx);
//	free(ctx);
	return(0);
}

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

int BTIC1H_EmitCommandCode(BTIC1H_Context *ctx, int cmd)
	{ return(ctx->EmitCommandCode(ctx, cmd)); }

int BTIC1H_EmitCommandCodeBase(BTIC1H_Context *ctx, int cmd)
{
	int bit0;
	int i, j, k;
	
	if((cmd<0) || (cmd>=64))
		{ BT1H_TRAPCRASH }
                          
	ctx->cnt_cmds++;
	ctx->stat_cmds[cmd]++;

	j=ctx->cmdidx[cmd];
	i=(byte)(j-ctx->cmdwpos);
	if(j>0)
	{
		k=ctx->cmdwin[j&15];
		if(k!=cmd)
			j=-1;
	}
	
	if((j>=0) && (i>=0) && (i<16))
	{
		if(i<0)
			return(-1);
	
		bit0=ctx->bs_bits;
		BTIC1H_Rice_WriteAdRice(ctx, i+1, &(ctx->rk_cmdidx));
		ctx->bits_cmdidx+=ctx->bs_bits-bit0;

		if(i>0)
		{
			j=(byte)(ctx->cmdwpos);
			k=ctx->cmdwin[(j+i-1)&15];
			ctx->cmdwin[(j+i-1)&15]=cmd;
			ctx->cmdwin[(j+i+0)&15]=k;
			ctx->cmdidx[cmd&255]=(byte)(j+i-1);
			ctx->cmdidx[k&255]=(byte)(j+i);
		}

		return(0);
	}
	
	bit0=ctx->bs_bits;

	BTIC1H_Rice_WriteAdRice(ctx, 0, &(ctx->rk_cmdidx));
	BTIC1H_Rice_WriteAdRice(ctx, cmd, &(ctx->rk_cmdabs));
	
	ctx->bits_cmdabs+=ctx->bs_bits-bit0;

	j=(byte)(--ctx->cmdwpos);
	k=ctx->cmdwin[j&15];

	if((ctx->cmdidx[k]&15)==(j&15))
		ctx->cmdidx[k]=-1;

	ctx->cmdwin[j&15]=cmd;
	ctx->cmdidx[cmd&255]=j;

	return(0);
}

int BTIC1H_EmitCommandCodeSMTF(BTIC1H_Context *ctx, int cmd)
{
	int bit0;
	int i0, i1;
	int i, j, k;

	ctx->cnt_cmds++;
	ctx->stat_cmds[cmd]++;

	j=ctx->cmdidx[cmd];
	i=(byte)(j-ctx->cmdwpos);

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdRice(ctx, i, &(ctx->rk_cmdidx));
//	BTIC1H_Rice_WriteAdRiceDc(ctx, i, &(ctx->rk_cmdidx));
	ctx->bits_cmdidx+=ctx->bs_bits-bit0;

	if(i==0)
	{
	}else if(i<32)
	{
		i0=(ctx->cmdwpos+i-1)&255;
		i1=(ctx->cmdwpos+i+0)&255;
			
		k=ctx->cmdwin[i0];
		ctx->cmdwin[i0]=cmd;
		ctx->cmdwin[i1]=k;
		ctx->cmdidx[cmd&255]=i0;
		ctx->cmdidx[k&255]=i1;
	}else
	{
		i0=(ctx->cmdwpos+i)&255;
		i1=(ctx->cmdwpos-1)&255;
		ctx->cmdwpos--;

		j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
		ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
		ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;
	}
	return(0);
}

int BTIC1H_EmitParmVarVal(BTIC1H_Context *ctx, int var, int val)
{
	int bit0;

	ctx->cnt_parms++;

	BTIC1H_EmitCommandCode(ctx, 0x23);
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, var, &(ctx->rk_parmvar));
	BTIC1H_Rice_WriteAdSRice(ctx, val, &(ctx->rk_parmval));
	ctx->bits_parm+=ctx->bs_bits-bit0;
	return(0);
}

int BTIC1H_EmitEnableDeltaMask(BTIC1H_Context *ctx)
{
	int bit0;

	if(!ctx->updmask)
	{
		ctx->cnt_parms++;

		BTIC1H_EmitCommandCode(ctx, 0x26);
		bit0=ctx->bs_bits;
		BTIC1H_Rice_WriteAdSRice(ctx, -1, &(ctx->rk_parmvar));
		ctx->bits_parm+=ctx->bs_bits-bit0;
		ctx->updmask=255;
		ctx->nextupdmask=255;
		ctx->tgtupdmask=255;
	}
	return(0);
}

int BTIC1H_EmitDisableDeltaMask(BTIC1H_Context *ctx)
{
	int bit0;

	if(ctx->updmask)
	{
		ctx->cnt_parms++;

		BTIC1H_EmitCommandCode(ctx, 0x27);
		bit0=ctx->bs_bits;
		BTIC1H_Rice_WriteAdSRice(ctx, -1, &(ctx->rk_parmvar));
		ctx->bits_parm+=ctx->bs_bits-bit0;
		ctx->updmask=0;
		ctx->nextupdmask=0;
	}
	return(0);
}

int BTIC1H_EmitMaskCode(BTIC1H_Context *ctx, int val)
{
	int i, j, k;

	if(val>=0)
	{
		i=ctx->maskidx[val];
		if(i>0)
		{
			j=ctx->maskwin[i-1];
			k=ctx->maskwin[i  ];
			ctx->maskwin[i-1]=k;
			ctx->maskwin[i  ]=j;
			ctx->maskidx[k]=i-1;
			ctx->maskidx[j]=i;
		}
		BTIC1H_Rice_WriteAdRice(ctx, i+1, &(ctx->rk_maskidx));
		return(0);
	}else
	{
		BTIC1H_Rice_WriteAdRice(ctx, 0, &(ctx->rk_maskidx));
		return(0);
	}
}

#ifdef BT1H_USECLRPRED
int BTIC1H_EmitCheckUpdColorPred(BTIC1H_Context *ctx)
{
	if(ctx->reqcolorpred!=ctx->colorpred)
	{
		if(!ctx->colorpred)
		{
			ctx->lcy=ctx->cy;		ctx->lcu=ctx->cu;
			ctx->lcv=ctx->cv;		ctx->lcd=ctx->cd;
			ctx->lcdy=ctx->cdy;		ctx->lcdu=ctx->cdu;
			ctx->lcdv=ctx->cdv;
		}
	
		BTIC1H_EmitParmVarVal(ctx,
			-2, -ctx->reqcolorpred);
		ctx->colorpred=ctx->reqcolorpred;
	}
	return(0);
}
#endif

int BTIC1H_EmitCheckEnableDeltaMask(BTIC1H_Context *ctx)
{
	int i, j, k;

#ifdef BT1H_USECLRPRED
	BTIC1H_EmitCheckUpdColorPred(ctx);
#endif

	i=ctx->cnt_dpts;
	if(i<64)
		return(0);
	
//	return(0);
	
//	j=(3*ctx->cnt_dpts)>>2;
//	k=(1*ctx->cnt_dpts)>>2;

	j=(2*ctx->cnt_dpts)>>2;
	k=(1*ctx->cnt_dpts)>>2;

//	j=(7*ctx->cnt_dpts)>>3;
//	k=(3*ctx->cnt_dpts)>>3;
	if(ctx->cnt_dzpts>j)
		{ BTIC1H_EmitEnableDeltaMask(ctx); }
	if(ctx->cnt_dzpts<k)
		{ BTIC1H_EmitDisableDeltaMask(ctx); }

	if(ctx->updmask)
	{
		ctx->tgtupdmask=255;
		for(i=0; i<6; i++)
		{
			if((ctx->cnt_dcpts[i]>8) &&
//				(ctx->cnt_dczpts[i]>((3*ctx->cnt_dcpts[i])>>2)))
//				(ctx->cnt_dczpts[i]>((ctx->cnt_dcpts[i])>>1)))
				(ctx->cnt_dczpts[i]>((ctx->cnt_dcpts[i])>>2)))
					ctx->tgtupdmask&=~(1<<i);
		}
	}

	return(0);
}

int BTIC1H_EmitDeltaMaskI(BTIC1H_Context *ctx, int um, int bn)
{
	int bit0, bm;
	int i, j, k;
	
	bm=(1<<bn)-1;

	if((ctx->updmask==(ctx->updmask|um)) &&
		((ctx->updmask&bm)==((ctx->tgtupdmask|um)&bm)))
	{
		bit0=ctx->bs_bits;
		BTIC1H_EmitMaskCode(ctx, -1);
		ctx->bits_dumask+=ctx->bs_bits-bit0;
		return(ctx->updmask|um);
	}

	if((ctx->tgtupdmask&bm)==((ctx->tgtupdmask|um)&bm))
	{
		j=ctx->tgtupdmask;
		ctx->updmask=j;

		bit0=ctx->bs_bits;
		BTIC1H_EmitMaskCode(ctx, ((j<<1)|1)&255);
		ctx->bits_dumask+=ctx->bs_bits-bit0;
		return(ctx->tgtupdmask);
	}

	j=ctx->tgtupdmask|um;
	bit0=ctx->bs_bits;
	BTIC1H_EmitMaskCode(ctx, (j<<1)&255);
	ctx->bits_dumask+=ctx->bs_bits-bit0;
	return(ctx->tgtupdmask|um);
}

int BTIC1H_EmitDeltaMask3(BTIC1H_Context *ctx, int um)
	{ return(BTIC1H_EmitDeltaMaskI(ctx, um, 3)); }
int BTIC1H_EmitDeltaMask4(BTIC1H_Context *ctx, int um)
	{ return(BTIC1H_EmitDeltaMaskI(ctx, um, 4)); }
int BTIC1H_EmitDeltaMask6(BTIC1H_Context *ctx, int um)
	{ return(BTIC1H_EmitDeltaMaskI(ctx, um, 6)); }

void BTIC1H_EmitUpdPredDeltaYUV(BTIC1H_Context *ctx,
	int cy, int cu, int cv)
{
#ifdef BT1H_USECLRPRED
	int pw0, pw1, pw2, pwb;
	int py, pu, pv, pi;
	int dy, du, dv;

	dy=cy-ctx->cy;	du=cu-ctx->cu;	dv=cv-ctx->cv;
	pw0=2*btic1h_sgnfold(dy)+btic1h_sgnfold(du)+btic1h_sgnfold(dv);

	py=(3*ctx->cy-ctx->lcy)>>1;
	pu=(3*ctx->cu-ctx->lcu)>>1;
	pv=(3*ctx->cv-ctx->lcv)>>1;
	dy=cy-py;	du=cu-pu;	dv=cv-pv;
	pw1=2*btic1h_sgnfold(dy)+btic1h_sgnfold(du)+btic1h_sgnfold(dv);

	py=2*ctx->cy-ctx->lcy;
	pu=2*ctx->cu-ctx->lcu;
	pv=2*ctx->cv-ctx->lcv;
	dy=cy-py;	du=cu-pu;	dv=cv-pv;
	pw2=2*btic1h_sgnfold(dy)+btic1h_sgnfold(du)+btic1h_sgnfold(dv);

	pi=0; pwb=pw0;
	if(pw1<pwb) { pi=1; pwb=pw1; }
	if(pw2<pwb) { pi=2; pwb=pw2; }
	ctx->cnt_pred[pi]++;
	
	if(ctx->cnt_pred[pi]>(ctx->cnt_pred[ctx->colorpred]+16))
		{ ctx->reqcolorpred=pi; }

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
		{
			py=(3*ctx->cy-ctx->lcy)>>1;
			pu=(3*ctx->cu-ctx->lcu)>>1;
			pv=(3*ctx->cv-ctx->lcv)>>1;
		}else if(ctx->colorpred==2)
		{
			py=2*ctx->cy-ctx->lcy;
			pu=2*ctx->cu-ctx->lcu;
			pv=2*ctx->cv-ctx->lcv;
		}

#if 0
		if((py|pu|pv)>>8)
		{
			py=clamp255(py);
			pu=clamp255(pu);
			pv=clamp255(pv);
		}
#endif

		ctx->lcy=ctx->cy;
		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;
		
		ctx->cy=py;
		ctx->cu=pu;
		ctx->cv=pv;
	}else
	{
		ctx->lcy=ctx->cy;
		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;
//		py=ctx->cy;
//		pu=ctx->cu;
//		pv=ctx->cv;
	}
#endif
}

void BTIC1H_EmitUpdPredDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
#ifdef BT1H_USECLRPRED
	int pw0, pw1, pw2, pwb;
	int py, pu, pv, pd, pi;
	int dy, du, dv, dd;

	dy=cy-ctx->cy;	du=cu-ctx->cu;	dv=cv-ctx->cv;	dd=cd-ctx->cd;
	pw0=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(dd);

	py=(3*ctx->cy-ctx->lcy)>>1;	pu=(3*ctx->cu-ctx->lcu)>>1;
	pv=(3*ctx->cv-ctx->lcv)>>1;	pd=(3*ctx->cd-ctx->lcd)>>1;
	dy=cy-py;	du=cu-pu;	dv=cv-pv;	dd=cd-pd;
	pw1=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(dd);

	py=2*ctx->cy-ctx->lcy;	pu=2*ctx->cu-ctx->lcu;
	pv=2*ctx->cv-ctx->lcv;	pd=2*ctx->cd-ctx->lcd;
	dy=cy-py;	du=cu-pu;	dv=cv-pv;	dd=cd-pd;
	pw2=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(dd);

	pi=0; pwb=pw0;
	if(pw1<pwb) { pi=1; pwb=pw1; }
	if(pw2<pwb) { pi=2; pwb=pw2; }
	ctx->cnt_pred[pi]++;
	
	if(ctx->cnt_pred[pi]>(ctx->cnt_pred[ctx->colorpred]+16))
		{ ctx->reqcolorpred=pi; }

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
		{
			py=(3*ctx->cy-ctx->lcy)>>1;		pu=(3*ctx->cu-ctx->lcu)>>1;
			pv=(3*ctx->cv-ctx->lcv)>>1;		pd=(3*ctx->cd-ctx->lcd)>>1;
		}else if(ctx->colorpred==2)
		{
			py=2*ctx->cy-ctx->lcy;		pu=2*ctx->cu-ctx->lcu;
			pv=2*ctx->cv-ctx->lcv;		pd=2*ctx->cd-ctx->lcd;
		}

		if((py|pu|pv|pd)>>8)
		{
			dd=0;
		}

#if 0
		if((py|pu|pv|pd)>>8)
		{
			py=clamp255(py);
			pu=clamp255(pu);
			pv=clamp255(pv);
			pd=clamp255(pd);
		}
#endif

		ctx->lcy=ctx->cy;		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;		ctx->lcd=ctx->cd;
		ctx->cy=py;		ctx->cu=pu;
		ctx->cv=pv;		ctx->cd=pd;
	}else
	{
		ctx->lcy=ctx->cy;		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;		ctx->lcd=ctx->cd;
	}
#endif
}

void BTIC1H_EmitUpdPredDeltaYUVDyuv(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cdy, int cdu, int cdv)
{
#ifdef BT1H_USECLRPRED
	int pw0, pw1, pw2, pwb;
	int py, pu, pv, pdy, pdu, pdv, pi;
	int dy, du, dv, ddy, ddu, ddv;

	dy=cy-ctx->cy;		du=cu-ctx->cu;		dv=cv-ctx->cv;
	ddy=cdy-ctx->cdy;	ddu=cdu-ctx->cdu;	ddv=cdv-ctx->cdv;
	pw0=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(ddy)+
		btic1h_sgnfold(ddu)+btic1h_sgnfold(ddv);

	py=(3*ctx->cy-ctx->lcy)>>1;		pu=(3*ctx->cu-ctx->lcu)>>1;
	pv=(3*ctx->cv-ctx->lcv)>>1;		pdy=(3*ctx->cdy-ctx->lcdy)>>1;
	pdu=(3*ctx->cdu-ctx->lcdu)>>1;	pdv=(3*ctx->cdv-ctx->lcdv)>>1;
	dy=cy-py;		du=cu-pu;		dv=cv-pv;
	ddy=cdy-pdy;	ddu=cdu-pdu;	ddv=cdv-pdv;
	pw1=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(ddy)+
		btic1h_sgnfold(ddu)+btic1h_sgnfold(ddv);

	py=2*ctx->cy-ctx->lcy;		pu=2*ctx->cu-ctx->lcu;
	pv=2*ctx->cv-ctx->lcv;		pdy=2*ctx->cdy-ctx->lcdy;
	pdu=2*ctx->cdu-ctx->lcdu;	pdv=2*ctx->cdv-ctx->lcdv;
	dy=cy-py;		du=cu-pu;		dv=cv-pv;
	ddy=cdy-pdy;	ddu=cdu-pdu;	ddv=cdv-pdv;
	pw2=btic1h_sgnfold(dy)+btic1h_sgnfold(du)+
		btic1h_sgnfold(dv)+btic1h_sgnfold(ddy)+
		btic1h_sgnfold(ddu)+btic1h_sgnfold(ddv);

	pi=0; pwb=pw0;
	if(pw1<pwb) { pi=1; pwb=pw1; }
	if(pw2<pwb) { pi=2; pwb=pw2; }
	ctx->cnt_pred[pi]++;
	
	if(ctx->cnt_pred[pi]>(ctx->cnt_pred[ctx->colorpred]+16))
		{ ctx->reqcolorpred=pi; }

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
		{
			py=(3*ctx->cy-ctx->lcy)>>1;		pu=(3*ctx->cu-ctx->lcu)>>1;
			pv=(3*ctx->cv-ctx->lcv)>>1;
			pdy=(3*ctx->cdy-ctx->lcdy)>>1;	pdu=(3*ctx->cdu-ctx->lcdu)>>1;
			pdv=(3*ctx->cdv-ctx->lcdv)>>1;
		}else if(ctx->colorpred==2)
		{
			py=2*ctx->cy-ctx->lcy;		pu=2*ctx->cu-ctx->lcu;
			pv=2*ctx->cv-ctx->lcv;
			pdy=2*ctx->cdy-ctx->lcdy;	pdu=2*ctx->cdu-ctx->lcdu;
			pdv=2*ctx->cdv-ctx->lcdv;
		}

#if 0
		if((py|pu|pv)>>8)
		{
			py=clamp255(py);
			pu=clamp255(pu);
			pv=clamp255(pv);
		}

		if((pdy|pdu|pdv)>>8)
		{
			pdy=clamp255sg(pdy);
			pdu=clamp255sg(pdu);
			pdv=clamp255sg(pdv);
		}
#endif

		ctx->lcy=ctx->cy;		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;		ctx->lcdy=ctx->cdy;
		ctx->lcdu=ctx->cdu;		ctx->lcdv=ctx->cdv;
		ctx->cy=py;		ctx->cu=pu;
		ctx->cv=pv;		ctx->cdy=pdy;
		ctx->cdu=pdu;	ctx->cdv=pdv;
	}else
	{
		ctx->lcy=ctx->cy;		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;		ctx->lcdy=ctx->cdy;
		ctx->lcdu=ctx->cdu;		ctx->lcdv=ctx->cdv;
	}
#endif
}

void BTIC1H_EmitUpdPredDeltaY(BTIC1H_Context *ctx,
	int cy)
{
	int lcy, lcu, lcv;
	int tcy, tcu, tcv;

	lcu=ctx->lcu;	lcv=ctx->lcv;
	tcu=ctx->cu;	tcv=ctx->cv;
	BTIC1H_EmitUpdPredDeltaYUV(ctx, cy, tcu, tcv);
	ctx->lcu=lcu;	ctx->lcv=lcv;
	ctx->cu=tcu;	ctx->cv=tcv;
}

void BTIC1H_EmitUpdPredDeltaYD(BTIC1H_Context *ctx,
	int cy, int cd)
{
	int lcy, lcu, lcv;
	int tcy, tcu, tcv;

	lcu=ctx->lcu;	lcv=ctx->lcv;
	tcu=ctx->cu;	tcv=ctx->cv;
	BTIC1H_EmitUpdPredDeltaYUVD(ctx, cy, tcu, tcv, cd);
	ctx->lcu=lcu;	ctx->lcv=lcv;
	ctx->cu=tcu;	ctx->cv=tcv;
}

void BTIC1H_EmitUpdPredDeltaUV(BTIC1H_Context *ctx,
	int cu, int cv)
{
	int lcy, tcy;

	lcy=ctx->lcy;	tcy=ctx->cy;
	BTIC1H_EmitUpdPredDeltaYUV(ctx, tcy, cu, cv);
	ctx->lcy=lcy;	ctx->cy=tcy;
}


int BTIC1H_EmitDeltaYUV(BTIC1H_Context *ctx,
	int cy, int cu, int cv)
{
	int bit0, um;
	int dy, du, dv, dd;
	int py, pu, pv;
	int qdy, qdu, qdv, qdd;
	
	BTIC1H_EmitUpdPredDeltaYUV(ctx, cy, cu, cv);
	
	dy=cy-ctx->cy;
	du=cu-ctx->cu;
	dv=cv-ctx->cv;

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;

	py=ctx->cy+(qdy*ctx->qfy);
	pu=ctx->cu+(qdu*ctx->qfuv);
	pv=ctx->cv+(qdv*ctx->qfuv);

//	if((py|pu|pv)>>8)
	while((py|pu|pv)>>8)
	{
		qdy=qdy-(py>>8);
		qdu=qdu-(pu>>8);
		qdv=qdv-(pv>>8);

		py=ctx->cy+(qdy*ctx->qfy);
		pu=ctx->cu+(qdu*ctx->qfuv);
		pv=ctx->cv+(qdv*ctx->qfuv);
	}

	ctx->cy=py;
	ctx->cu=pu;
	ctx->cv=pv;

//	ctx->cy+=qdy*ctx->qfy;
//	ctx->cu+=qdu*ctx->qfuv;
//	ctx->cv+=qdv*ctx->qfuv;

#if 1
	ctx->cnt_dpts+=3;
	if(!qdy)ctx->cnt_dzpts++;
	if(!qdu)ctx->cnt_dzpts++;
	if(!qdv)ctx->cnt_dzpts++;
//	BTIC1H_EmitCheckEnableDeltaMask(ctx);
#endif

#if 1
	if(ctx->updmask)
	{
		ctx->cnt_dcpts[0]++;
		ctx->cnt_dcpts[1]++;
		ctx->cnt_dcpts[2]++;

		if(!qdy)ctx->cnt_dczpts[0]++;
		if(!qdu)ctx->cnt_dczpts[1]++;
		if(!qdv)ctx->cnt_dczpts[2]++;
	}
#endif

	if(ctx->updmask)
	{
		um=0;
		if(qdy)um|=1;
		if(qdu)um|=2;
		if(qdv)um|=4;
		um=BTIC1H_EmitDeltaMask3(ctx, um);
	}else { um=255; }

	bit0=ctx->bs_bits;
	if(um&1)
		BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&2)
		BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_duv));
	if(um&4)
		BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_duv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

#ifdef BT1H_DEBUG_TRAPRANGE
	if((ctx->cy<0) || (ctx->cy>255) ||
	   (ctx->cu<0) || (ctx->cu>255) ||
	   (ctx->cv<0) || (ctx->cv>255) ||
	   (ctx->cd<0) || (ctx->cd>255))
	{
		BT1H_TRAPCRASH
	}
#endif

	return(0);
}

#if 0
int BTIC1H_EmitDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0;
	int dd, qdd, pd;

	dd=cd-ctx->cd;
//	qdd=dd/ctx->qfd;
	qdd=(dd*ctx->fx_qfd+2048)>>12;
//	ctx->cd+=qdd*ctx->qfd;
	pd=ctx->cd+(qdd*ctx->qfd);
	if(pd>>8)
	{
		qdd=qdd-(pd>>8);
		pd=ctx->cd+(qdd*ctx->qfd);
	}
	ctx->cd=pd;
	
	BTIC1H_EmitDeltaYUV(ctx, cy, cu, cv);	
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdd, &(ctx->rk_ddy));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

	return(0);
}
#endif

#if 1
int BTIC1H_EmitDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0, um;
	int by, bu, bv, bd;
	int dy, du, dv, dd;
	int py, pu, pv, pd;
	int qdy, qdu, qdv, qdd;

	BTIC1H_EmitUpdPredDeltaYUVD(ctx, cy, cu, cv, cd);

	by=ctx->cy;		bu=ctx->cu;
	bv=ctx->cv;		bd=ctx->cd;
	dy=cy-by;		du=cu-bu;
	dv=cv-bv;		dd=cd-bd;
	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;
	qdd=(dd*ctx->fx_qfd +2048)>>12;
	py=by+(qdy*ctx->qfy);	pu=bu+(qdu*ctx->qfuv);
	pv=bv+(qdv*ctx->qfuv);	pd=bd+(qdd*ctx->qfd);

//	if((py|pu|pv|pd)>>8)
	while((py|pu|pv|pd)>>8)
	{
		qdy=qdy-(py>>8);		qdu=qdu-(pu>>8);
		qdv=qdv-(pv>>8);		qdd=qdd-(pd>>8);
		py=by+(qdy*ctx->qfy);	pu=bu+(qdu*ctx->qfuv);
		pv=bv+(qdv*ctx->qfuv);	pd=bd+(qdd*ctx->qfd);
	}

	ctx->cy=py;		ctx->cu=pu;
	ctx->cv=pv;		ctx->cd=pd;

#if 1
	ctx->cnt_dpts+=4;
	if(!qdy)ctx->cnt_dzpts++;
	if(!qdu)ctx->cnt_dzpts++;
	if(!qdv)ctx->cnt_dzpts++;
	if(!qdd)ctx->cnt_dzpts++;
//	BTIC1H_EmitCheckEnableDeltaMask(ctx);
#endif

#if 1
	if(ctx->updmask)
	{
		ctx->cnt_dcpts[0]++;
		ctx->cnt_dcpts[1]++;
		ctx->cnt_dcpts[2]++;
		ctx->cnt_dcpts[3]++;

		if(!qdy)ctx->cnt_dczpts[0]++;
		if(!qdu)ctx->cnt_dczpts[1]++;
		if(!qdv)ctx->cnt_dczpts[2]++;
		if(!qdd)ctx->cnt_dczpts[3]++;
	}
#endif

	if(ctx->updmask)
	{
		um=0;
		if(qdy)um|=1;
		if(qdu)um|=2;
		if(qdv)um|=4;
		if(qdd)um|=8;
		um=BTIC1H_EmitDeltaMask4(ctx, um);
	}else { um=255; }

	bit0=ctx->bs_bits;
	if(um&1)
		BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&2)
		BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_duv));
	if(um&4)
		BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_duv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&8)
		BTIC1H_Rice_WriteAdSRice(ctx, qdd, &(ctx->rk_ddy));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

#ifdef BT1H_DEBUG_TRAPRANGE
	if(	((ctx->cy<0) || (ctx->cy>255)) ||
		((ctx->cu<0) || (ctx->cu>255)) ||
		((ctx->cv<0) || (ctx->cv>255)) ||
		((ctx->cd<0) || (ctx->cd>255)))
	{
		BT1H_TRAPCRASH
	}
#endif

	return(0);
}
#endif

#if 1
int BTIC1H_EmitDeltaYUVDyuv(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int cdy, int cdu, int cdv)
{
	int bit0, um;
	int by, bu, bv, bdy, bdu, bdv;
	int dy, du, dv, ddy, ddu, ddv;
	int py, pu, pv, pdy, pdu, pdv;
	int qdy, qdu, qdv, qddy, qddu, qddv;

	BTIC1H_EmitUpdPredDeltaYUVDyuv(ctx, cy, cu, cv, cdy, cdu, cdv);

	by=ctx->cy;		bu=ctx->cu;
	bv=ctx->cv;		bdy=ctx->cdy;
	bdu=ctx->cdu;	bdv=ctx->cdv;
	dy=cy-by;		du=cu-bu;
	dv=cv-bv;		ddy=cdy-bdy;
	ddu=cdu-bdu;	ddv=cdv-bdv;
	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;
	qddy=(ddy*ctx->fx_qfdy +2048)>>12;
	qddu=(ddu*ctx->fx_qfduv+2048)>>12;
	qddv=(ddv*ctx->fx_qfduv+2048)>>12;
	py=by+(qdy*ctx->qfy);	pu=bu+(qdu*ctx->qfuv);
	pv=bv+(qdv*ctx->qfuv);
	pdy=bdy+(qddy*ctx->qfdy);
	pdu=bdu+(qddu*ctx->qfduv);
	pdv=bdv+(qddv*ctx->qfduv);

//	if((py|pu|pv|pdy|pdu|pdv)>>8)
//	if((py|pu|pv)>>8)
	while((py|pu|pv)>>8)
	{
		qdy=qdy-(py>>8);			qdu=qdu-(pu>>8);
		qdv=qdv-(pv>>8);
		py=by+(qdy*ctx->qfy);		pu=bu+(qdu*ctx->qfuv);
		pv=bv+(qdv*ctx->qfuv);
	}

//	if(((pdy<-256)||(pdy>256))||
//		((pdu<-256)||(pdu>256))||
//		((pdv<-256)||(pdv>256)))
//	if(((pdy^(pdy>>8))|(pdu^(pdu>>8))|(pdv^(pdv>>8)))>>8)
//	while(((pdy^(pdy>>8))|(pdu^(pdu>>8))|(pdv^(pdv>>8)))>>8)
	if(((pdy<-255)||(pdy>255))||
		((pdu<-255)||(pdu>255))||
		((pdv<-255)||(pdv>255)))
	{
		qddy=qddy-(pdy>>8);
		qddu=qddu-(pdu>>8);			qddv=qddv-(pdv>>8);
		pdy=bdy+(qddy*ctx->qfdy);
		pdu=bdu+(qddu*ctx->qfduv);	pdv=bdv+(qddv*ctx->qfduv);
	}

	ctx->cy=py;		ctx->cu=pu;
	ctx->cv=pv;		ctx->cdy=pdy;
	ctx->cdu=pdu;	ctx->cdv=pdv;

	ctx->cnt_dpts+=6;
	if(!qdy)ctx->cnt_dzpts++;
	if(!qdu)ctx->cnt_dzpts++;
	if(!qdv)ctx->cnt_dzpts++;
	if(!qddy)ctx->cnt_dzpts++;
	if(!qddu)ctx->cnt_dzpts++;
	if(!qddv)ctx->cnt_dzpts++;

#if 1
	if(ctx->updmask)
	{
		ctx->cnt_dcpts[0]++;
		ctx->cnt_dcpts[1]++;
		ctx->cnt_dcpts[2]++;
		ctx->cnt_dcpts[3]++;
		ctx->cnt_dcpts[4]++;
		ctx->cnt_dcpts[5]++;

		if(!qdy)ctx->cnt_dczpts[0]++;
		if(!qdu)ctx->cnt_dczpts[1]++;
		if(!qdv)ctx->cnt_dczpts[2]++;
		if(!qddy)ctx->cnt_dczpts[3]++;
		if(!qddu)ctx->cnt_dczpts[4]++;
		if(!qddv)ctx->cnt_dczpts[5]++;
	}
#endif

	if(ctx->updmask)
	{
		um=0;
		if(qdy)um|=1;
		if(qdu)um|=2;
		if(qdv)um|=4;
		if(qddy)um|=8;
		if(qddu)um|=16;
		if(qddv)um|=32;
		um=BTIC1H_EmitDeltaMask6(ctx, um);
	}else { um=255; }

	bit0=ctx->bs_bits;
	if(um&1)
		BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&2)
		BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_duv));
	if(um&4)
		BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_duv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&8)
		BTIC1H_Rice_WriteAdSRice(ctx, qddy, &(ctx->rk_ddy));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&16)
		BTIC1H_Rice_WriteAdSRice(ctx, qddu, &(ctx->rk_dduv));
	if(um&32)
		BTIC1H_Rice_WriteAdSRice(ctx, qddv, &(ctx->rk_dduv));
	ctx->bits_dduv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

#ifdef BT1H_DEBUG_TRAPRANGE
	if(	((ctx->cy<0) || (ctx->cy>255)) ||
		((ctx->cu<0) || (ctx->cu>255)) ||
		((ctx->cv<0) || (ctx->cv>255)))
	{
		BT1H_TRAPCRASH
	}

	if(	((ctx->cdy<-256)||(ctx->cdy>255))||
		((ctx->cdu<-256)||(ctx->cdu>255))||
		((ctx->cdv<-256)||(ctx->cdv>255)))
	{
		BT1H_TRAPCRASH
	}
#endif

	return(0);
}
#endif

int BTIC1H_EmitDeltaY(BTIC1H_Context *ctx, int cy)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;

	BTIC1H_EmitUpdPredDeltaY(ctx, cy);

	dy=cy-ctx->cy;
	qdy=(dy*ctx->fx_qfy +2048)>>12;
	ctx->cy+=qdy*ctx->qfy;

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;
	return(0);
}

int BTIC1H_EmitDeltaUV(BTIC1H_Context *ctx, int cu, int cv)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;

	BTIC1H_EmitUpdPredDeltaUV(ctx, cu, cv);

	du=cu-ctx->cu;
	dv=cv-ctx->cv;

	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;

	ctx->cu+=qdu*ctx->qfuv;
	ctx->cv+=qdv*ctx->qfuv;

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_duv));
	BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_duv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;
	
	return(0);
}

int BTIC1H_EmitDeltaYD(BTIC1H_Context *ctx, int cy, int cd)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;

	BTIC1H_EmitUpdPredDeltaYD(ctx, cy, cd);

	dy=cy-ctx->cy;
	dd=cd-ctx->cd;
	qdy=(dy*ctx->fx_qfy+2048)>>12;
	qdd=(dd*ctx->fx_qfd+2048)>>12;
	ctx->cy+=qdy*ctx->qfy;
	ctx->cd+=qdd*ctx->qfd;

	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_dy));
	BTIC1H_Rice_WriteAdSRice(ctx, qdd, &(ctx->rk_ddy));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	return(0);
}

int BTIC1H_EmitDeltaA(BTIC1H_Context *ctx, int cy)
	{ return(BTIC1H_EmitDeltaY(ctx, cy)); }

int BTIC1H_EmitDeltaAD(BTIC1H_Context *ctx, int cy, int cd)
	{ return(BTIC1H_EmitDeltaYD(ctx, cy, cd)); }


int BTIC1H_EmitDeltaQfYUVD(BTIC1H_Context *ctx,
	int cy, int cuv, int cd)
{
	int bit0;
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

	ctx->cnt_parms++;
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, dy, &(ctx->rk_qfy));
	BTIC1H_Rice_WriteAdSRice(ctx, duv, &(ctx->rk_qfuv));
	BTIC1H_Rice_WriteAdSRice(ctx, dd, &(ctx->rk_qfdy));
	ctx->bits_parm+=ctx->bs_bits-bit0;
	return(0);
}

int BTIC1H_EmitDeltaQfYUVDyuv(BTIC1H_Context *ctx,
	int cy, int cuv, int cdy, int cduv)
{
	int bit0;
	int dy, duv, ddy, dduv;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->qfy;
	duv=cuv-ctx->qfuv;
	ddy=cdy-ctx->qfdy;
	dduv=cduv-ctx->qfduv;

	ctx->qfy=cy;
	ctx->qfuv=cuv;
	ctx->qfdy=cdy;
	ctx->qfduv=cduv;

	ctx->fx_qfy  =4096/cy;
	ctx->fx_qfuv =4096/cuv;
	ctx->fx_qfdy =4096/cdy;
	ctx->fx_qfduv=4096/cduv;

	ctx->cnt_parms++;
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, dy, &(ctx->rk_qfy));
	BTIC1H_Rice_WriteAdSRice(ctx, duv, &(ctx->rk_qfuv));
	BTIC1H_Rice_WriteAdSRice(ctx, ddy, &(ctx->rk_qfdy));
	BTIC1H_Rice_WriteAdSRice(ctx, dduv, &(ctx->rk_qfduv));
	ctx->bits_parm+=ctx->bs_bits-bit0;
	return(0);
}

int BTIC1H_EmitDeltaQfAD(BTIC1H_Context *ctx,
	int cy, int cd)
{
	int bit0;
	int dy, duv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-ctx->qfy;
	dd=cd-ctx->qfd;

	ctx->qfy=cy;
	ctx->qfd=cd;

	ctx->fx_qfy=4096/cy;
	ctx->fx_qfd=4096/cd;

	ctx->cnt_parms++;
	bit0=ctx->bs_bits;
	BTIC1H_Rice_WriteAdSRice(ctx, dy, &(ctx->rk_qfy));
	BTIC1H_Rice_WriteAdSRice(ctx, dd, &(ctx->rk_qfdy));
	ctx->bits_parm+=ctx->bs_bits-bit0;
	return(0);
}


#if 1
int BTIC1H_EmitAbsYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0, um;
	int by, bu, bv, bd;
	int dy, du, dv, dd;
	int py, pu, pv, pd;
	int qdy, qdu, qdv, qdd;

	dy=cy-ctx->absyuvbias;
	du=cu-ctx->absyuvbias;
	dv=cv-ctx->absyuvbias;
	dd=cd;
	qdy=(dy*ctx->fx_qfay +2048)>>12;
	qdu=(du*ctx->fx_qfauv+2048)>>12;
	qdv=(dv*ctx->fx_qfauv+2048)>>12;
	qdd=(dd*ctx->fx_qfad +2048)>>12;
	py=ctx->absyuvbias+qdy*ctx->qfay;
	pu=ctx->absyuvbias+qdu*ctx->qfauv;
	pv=ctx->absyuvbias+qdv*ctx->qfauv;
	pd=qdd*ctx->qfad;

	if((py|pu|pv|pd)>>8)
	{
		qdy=qdy-(py>>8);		qdu=qdu-(pu>>8);
		qdv=qdv-(pv>>8);		qdd=qdd-(pd>>8);
		py=ctx->absyuvbias+qdy*ctx->qfay;
		pu=ctx->absyuvbias+qdu*ctx->qfauv;
		pv=ctx->absyuvbias+qdv*ctx->qfauv;
		pd=qdd*ctx->qfad;
	}

	ctx->cy=py;		ctx->cu=pu;
	ctx->cv=pv;		ctx->cd=pd;
	ctx->lcy=py;	ctx->lcu=pu;
	ctx->lcv=pv;	ctx->lcd=pd;

#if 0
	ctx->cnt_dpts+=4;
	if(!qdy)ctx->cnt_dzpts++;
	if(!qdu)ctx->cnt_dzpts++;
	if(!qdv)ctx->cnt_dzpts++;
	if(!qdd)ctx->cnt_dzpts++;
#endif

#if 0
	if(ctx->absupdmask)
	{
		ctx->cnt_dcpts[0]++;
		ctx->cnt_dcpts[1]++;
		ctx->cnt_dcpts[2]++;
		ctx->cnt_dcpts[3]++;

		if(!qdy)ctx->cnt_dczpts[0]++;
		if(!qdu)ctx->cnt_dczpts[1]++;
		if(!qdv)ctx->cnt_dczpts[2]++;
		if(!qdd)ctx->cnt_dczpts[3]++;
	}
#endif

	if(ctx->absupdmask)
	{
		um=0;
		if(qdy)um|=1;
		if(qdu)um|=2;
		if(qdv)um|=4;
		if(qdd)um|=8;
		um=BTIC1H_EmitDeltaMask4(ctx, um);
	}else { um=255; }

	bit0=ctx->bs_bits;
	if(um&1)
		BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_ay));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&2)
		BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_auv));
	if(um&4)
		BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_auv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&8)
		BTIC1H_Rice_WriteAdSRice(ctx, qdd, &(ctx->rk_ady));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

#ifdef BT1H_DEBUG_TRAPRANGE
	if(	((ctx->cy<0) || (ctx->cy>255)) ||
		((ctx->cu<0) || (ctx->cu>255)) ||
		((ctx->cv<0) || (ctx->cv>255)) ||
		((ctx->cd<0) || (ctx->cd>255)))
	{
		BT1H_TRAPCRASH
	}
#endif

	return(0);
}
#endif

#if 1
int BTIC1H_EmitAbsYUVDyuv(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int cdy, int cdu, int cdv)
{
	int bit0, um;
	int by, bu, bv, bdy, bdu, bdv;
	int dy, du, dv, ddy, ddu, ddv;
	int py, pu, pv, pdy, pdu, pdv;
	int qdy, qdu, qdv, qddy, qddu, qddv;

	dy=cy-ctx->absyuvbias;
	du=cu-ctx->absyuvbias;
	dv=cv-ctx->absyuvbias;
	ddy=cdy;
	ddu=cdu;
	ddv=cdv;
	qdy=(dy*ctx->fx_qfay +2048)>>12;
	qdu=(du*ctx->fx_qfauv+2048)>>12;
	qdv=(dv*ctx->fx_qfauv+2048)>>12;
	qddy=(ddy*ctx->fx_qfady +2048)>>12;
	qddu=(ddu*ctx->fx_qfaduv+2048)>>12;
	qddv=(ddv*ctx->fx_qfaduv+2048)>>12;
	py=ctx->absyuvbias+qdy*ctx->qfay;
	pu=ctx->absyuvbias+qdu*ctx->qfauv;
	pv=ctx->absyuvbias+qdv*ctx->qfauv;
	pdy=qddy*ctx->qfady;
	pdu=qddu*ctx->qfaduv;
	pdv=qddv*ctx->qfaduv;

	if((py|pu|pv)>>8)
	{
		qdy=qdy-(py>>8);			qdu=qdu-(pu>>8);
		qdv=qdv-(pv>>8);
		py=ctx->absyuvbias+qdy*ctx->qfay;
		pu=ctx->absyuvbias+qdu*ctx->qfauv;
		pv=ctx->absyuvbias+qdv*ctx->qfauv;
	}

	if(((pdy^(pdy>>8))|(pdu^(pdu>>8))|(pdv^(pdv>>8)))>>8)
	{
		qddy=qddy-(pdy>>8);
		qddu=qddu-(pdu>>8);			qddv=qddv-(pdv>>8);
		pdy=qddy*ctx->qfady;
		pdu=qddu*ctx->qfaduv;
		pdv=qddv*ctx->qfaduv;
	}

	ctx->cy=py;		ctx->cu=pu;
	ctx->cv=pv;		ctx->cdy=pdy;
	ctx->cdu=pdu;	ctx->cdv=pdv;

	ctx->lcy=py;	ctx->lcu=pu;
	ctx->lcv=pv;	ctx->lcdy=pdy;
	ctx->lcdu=pdu;	ctx->lcdv=pdv;

#if 0
	ctx->cnt_dpts+=6;
	if(!qdy)ctx->cnt_dzpts++;
	if(!qdu)ctx->cnt_dzpts++;
	if(!qdv)ctx->cnt_dzpts++;
	if(!qddy)ctx->cnt_dzpts++;
	if(!qddu)ctx->cnt_dzpts++;
	if(!qddv)ctx->cnt_dzpts++;
#endif

#if 0
	if(ctx->absupdmask)
	{
		ctx->cnt_dcpts[0]++;
		ctx->cnt_dcpts[1]++;
		ctx->cnt_dcpts[2]++;
		ctx->cnt_dcpts[3]++;
		ctx->cnt_dcpts[4]++;
		ctx->cnt_dcpts[5]++;

		if(!qdy)ctx->cnt_dczpts[0]++;
		if(!qdu)ctx->cnt_dczpts[1]++;
		if(!qdv)ctx->cnt_dczpts[2]++;
		if(!qddy)ctx->cnt_dczpts[3]++;
		if(!qddu)ctx->cnt_dczpts[4]++;
		if(!qddv)ctx->cnt_dczpts[5]++;
	}
#endif

	if(ctx->absupdmask)
	{
		um=0;
		if(qdy)um|=1;
		if(qdu)um|=2;
		if(qdv)um|=4;
		if(qddy)um|=8;
		if(qddu)um|=16;
		if(qddv)um|=32;
		um=BTIC1H_EmitDeltaMask6(ctx, um);
	}else { um=255; }

	bit0=ctx->bs_bits;
	if(um&1)
		BTIC1H_Rice_WriteAdSRice(ctx, qdy, &(ctx->rk_ay));
	ctx->bits_dy+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&2)
		BTIC1H_Rice_WriteAdSRice(ctx, qdu, &(ctx->rk_auv));
	if(um&4)
		BTIC1H_Rice_WriteAdSRice(ctx, qdv, &(ctx->rk_auv));
	ctx->bits_duv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&8)
		BTIC1H_Rice_WriteAdSRice(ctx, qddy, &(ctx->rk_ady));
	ctx->bits_ddy+=ctx->bs_bits-bit0;

	bit0=ctx->bs_bits;
	if(um&16)
		BTIC1H_Rice_WriteAdSRice(ctx, qddu, &(ctx->rk_aduv));
	if(um&32)
		BTIC1H_Rice_WriteAdSRice(ctx, qddv, &(ctx->rk_aduv));
	ctx->bits_dduv+=ctx->bs_bits-bit0;
	ctx->bits_dyuv+=ctx->bs_bits-bit0;

#ifdef BT1H_DEBUG_TRAPRANGE
	if(	((ctx->cy<0) || (ctx->cy>255)) ||
		((ctx->cu<0) || (ctx->cu>255)) ||
		((ctx->cv<0) || (ctx->cv>255)))
	{
		BT1H_TRAPCRASH
	}

	if(	((ctx->cdy<-256)||(ctx->cdy>255))||
		((ctx->cdu<-256)||(ctx->cdu>255))||
		((ctx->cdv<-256)||(ctx->cdv>255)))
	{
		BT1H_TRAPCRASH
	}
#endif

	return(0);
}
#endif


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
	return(1);
}

int BTIC1H_CheckZeroDeltaYUVD(BTIC1H_Context *ctx,
	int cy, int cu, int cv, int cd)
{
	int bit0;
	int dy, du, dv, dd;
	int sdy, sdu, sdv, sdd;
	int qdy, qdu, qdv, qdd;
	int qdy1, qdu1, qdv1, qdd1;
	
	dy=cy-ctx->cy;
	du=cu-ctx->cu;
	dv=cv-ctx->cv;
	dd=cd-ctx->cd;

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;
	qdd=(dd*ctx->fx_qfd +2048)>>12;

#if 0
	sdy=dy*ctx->fx_qfy;		sdu=du*ctx->fx_qfuv;
	sdv=dv*ctx->fx_qfuv;	sdd=dd*ctx->fx_qfd;
	qdy=(sdy+2048)>>12;		qdu=(sdu+2048)>>12;
	qdv=(sdv+2048)>>12;		qdd=(sdd+2048)>>12;
	qdy1=sdy>>12;			qdu1=sdu>>12;
	qdv1=sdv>>12;			qdd1=sdd>>12;
	if(!qdy1)qdy=qdy1;
	if(!qdu1)qdu=qdu1;
	if(!qdv1)qdv=qdv1;
	if(!qdd1)qdd=qdd1;
#endif

	if(qdy || qdu || qdv || qdd)
		return(0);
	return(1);
}

int BTIC1H_CheckRefDeltaYUV(BTIC1H_Context *ctx,
	int *rcy, int *rcu, int *rcv,
	int cy, int cu, int cv)
{
	int bit0;
	int dy, du, dv, dd;
	int qdy, qdu, qdv, qdd;
	
	dy=cy-(*rcy);
	du=cu-(*rcu);
	dv=cv-(*rcv);

	qdy=(dy*ctx->fx_qfy +2048)>>12;
	qdu=(du*ctx->fx_qfuv+2048)>>12;
	qdv=(dv*ctx->fx_qfuv+2048)>>12;

	if(qdy || qdu || qdv)
	{
		*rcy+=qdy*ctx->qfy;
		*rcu+=qdu*ctx->qfuv;
		*rcv+=qdv*ctx->qfuv;
		return(1);
	}
	return(0);
}

static byte btic1h_errfrac2_tab[4][4]={
	{0,1,4,8},	{1,0,1,4},
	{4,1,0,1},	{8,4,1,0}};

int btic1h_errfrac2(int a, int b)
{
//	static byte tab[4][4]={
//		{0,1,4,8},	{1,0,1,4},
//		{4,1,0,1},	{8,4,1,0}};
//	return(tab[a][b]);
	return(btic1h_errfrac2_tab[a][b]);
}

static byte btic1h_errfrac4_tab[16][16];
static byte btic1h_errfrac12to8_tab[4096];
static int btic1h_errfrac4_init=0;

int btic1h_errfrac4(int a, int b)
{
	int i, j;
	return(btic1h_errfrac4_tab[a][b]);
}

int btic1h_errfrac8(int a, int b)
{
	return(
		btic1h_errfrac4(a>>4, b>>4)+
		btic1h_errfrac4(a&15, b&15));
}

#if 0
int BTIC1H_CalcBlockErrPix(byte *blka, byte *blkb)
{
	byte pxa[4], pxb[4];
	int u0, u1, ue;
	int i0, i1, i2, i3;
	int a, b, c, d;
	int i, j, k, l;
	
	a=blka[0];	b=blka[1];
	c=a-b; d=a+b;
	if(c<0)c=0;
	if(d>255)d=255;
	pxa[0]=c;				pxa[1]=(11*c+5*d)>>4;
	pxa[2]=(5*c+11*d)>>4;	pxa[3]=d;

	a=blkb[0];	b=blkb[1];
	c=a-b; d=a+b;
	if(c<0)c=0;
	if(d>255)d=255;
	pxb[0]=c;				pxb[1]=(11*c+5*d)>>4;
	pxb[2]=(5*c+11*d)>>4;	pxb[3]=d;

	u0=blka[2];
	u1=blkb[2];
	ue=u0-u1; ue=ue*ue;

//	d=0;
	d=ue*16;
	for(i=0; i<4; i++)
	{
		i0=blka[4+i];
		i1=blkb[4+i];
		for(j=0; j<4; j++)
		{
			i2=(i0>>(j*2))&3;
			i3=(i1>>(j*2))&3;
			a=pxa[i2];
			b=pxb[i3];
			c=a-b;
			d=d+c*c;
		}
	}

	return(sqrt(d/16));
}
#endif

int BTIC1H_CheckSkip(BTIC1H_Context *ctx,
	byte *blk, byte *lblk, int blksz,
	int qf, int dyem, int dyen,
	int duvem, int duven,
	int *rerr)
{
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int j0, j1, j2, j3, j4, j5, j6, j7;
	int cy, cu, cv, cd, cm;
	int cy1, cu1, cv1, cd1, cm1;
	int ca, cb, ca1, cb1;
	int e0, e1, e2, e3, edy, euv;
	int mcy, ncy, dcy;
	int i, j, k, l;

	if(!memcmp(blk, lblk, blksz))
	{
		if(rerr)*rerr=0;
		return(2);
	}

	if(!btic1h_errfrac4_init)
	{
		for(i=0; i<16; i++)
			for(j=0; j<16; j++)
		{
			btic1h_errfrac4_tab[i][j]=
				btic1h_errfrac2(i>>2, j>>2)+
				btic1h_errfrac2(i&3, j&3);
		}
		
		for(i=0; i<4096; i++)
		{
			j=	((i>>4)&0xC0)|((i>>3)&0x30)|
				((i>>2)&0x0C)|((i>>1)&0x03);
			btic1h_errfrac12to8_tab[i]=j;
		}
		
		btic1h_errfrac4_init=1;
	}

//	return(0);

	cy=blk[0]; cu=blk[1]; cv=blk[2]; cd=blk[3];
	cy1=lblk[0]; cu1=lblk[1]; cv1=lblk[2]; cd1=lblk[3];

	cm=0; cm1=0;

	if(cd==0)
	{
		cm=blk[4];
//		if((cm==1) || (cm==2))
		if(cm!=0)
			{ cd=blk[5]; }
			
		if((cm==16) || (cm==17))
			return(0);
	}

	if(cd1==0)
	{
		cm1=lblk[4];
//		if((cm1==1) || (cm1==2))
		if(cm1!=0)
			{ cd1=lblk[5]; }

		if((cm1==16) || (cm1==17))
			return(0);
	}

#if 0
	if((btic1h_subfold(cy1, cy)>dye) ||
		(btic1h_subfold(cd1, cd)>dye) ||
		(btic1h_subfold(cu1, cu)>duve) ||
		(btic1h_subfold(cv1, cv)>duve))
			return(0);

	ca=cy+(cd>>1);		cb=cy-(cd>>1);
	ca1=cy1+(cd1>>1);	cb1=cy1-(cd1>>1);

//	ca=cy+cd;		cb=cy-cd;
//	ca1=cy1+cd1;	cb1=cy1-cd1;

	if((btic1h_subfold(ca1, cb)<dye) &&
		(btic1h_subfold(cb1, ca)<dye))
			return(1);
#endif

#if 1
	e0=btic1h_subfold(cy1, cy);
	e1=btic1h_subfold(cd1, cd);
	e2=btic1h_subfold(cu1, cu);
	e3=btic1h_subfold(cv1, cv);
	
	edy=e0;
	euv=e2+e3;
	
//	if((e0>dyen) || (e1>dyen) || (e2>duven) || (e3>duven))
	if((e0>dyen) || (e1>dyen) || (euv>duven))
	{
//		e3=e2+e3;
		if(rerr)*rerr=e0+(e1>>1)+euv;
		return(0);
	}
#endif

#if 1
//	ca=cy+cd;		cb=cy-cd;
//	ca1=cy1+cd1;	cb1=cy1-cd1;

	ca=cy+(cd>>1);		cb=cy-(cd>>1);
	ca1=cy1+(cd1>>1);	cb1=cy1-(cd1>>1);

//	e2=btic1g_subfold(cu1, cu);
//	e3=btic1g_subfold(cv1, cv);
	e0=btic1h_subfold(ca1, cb);
	e1=btic1h_subfold(cb1, ca);

//	if((e0<dyem) && (e1<dyem) && (e2<duvem) && (e3<duvem))
	if((e0<dyem) && (e1<dyem) && (euv<duvem))
	{
//		e3=e2+e3;
//		e3=(e2>e3)?e2:e3;
		e2=(e0>e1)?e0:e1;
//		e2=(e0<e1)?e0:e1;
		if(rerr)*rerr=e2+euv;
		return(2);
	}
#endif

#if 1
	mcy=ca; ncy=ca;
	if(cb<mcy)mcy=cb;
	if(cb>ncy)ncy=cb;
	if(ca1<mcy)mcy=ca1;
	if(ca1>ncy)ncy=ca1;
	if(cb1<mcy)mcy=cb1;
	if(cb1>ncy)ncy=cb1;
	
	dcy=ncy-mcy;
#endif

#if 1
	if(cm!=cm1)
	{
//		e0=btic1h_subfold(cy1, cy);
//		e1=(cd+cd1)>>1;
//		e3=16*9;
//		e2=e0+((e1*e3)>>7);
//		e2=e0+e1;
		e2=dcy;
		if(rerr)*rerr=e2+euv;
//		return(e2<dyem);
		return(0);
	}
#endif

#if 1
	switch(cm)
	{
	case 0:
		e3=	btic1h_errfrac8(blk[4], lblk[4])+
			btic1h_errfrac8(blk[5], lblk[5])+
			btic1h_errfrac8(blk[6], lblk[6])+
			btic1h_errfrac8(blk[7], lblk[7]);
		break;
	case 1:
		e3=btic1h_errfrac8(blk[6], lblk[6])*4;
		break;
//	case 2:
//		e3=16*9;
//		break;
	case 8:		case 9:
		e3=btic1h_errfrac4(blk[6]&15, lblk[6]&15)*8;
		break;
	case 10:	case 11:
	case 12:	case 13:
		e3=(btic1h_errfrac8(blk[6], lblk[6])+
			btic1h_errfrac8(blk[7], lblk[7]))*2;
		break;

	case 14:	case 16:
	case 17:	case 24:
	case 27:
		e3=	btic1h_errfrac8(blk[12], lblk[12])+
			btic1h_errfrac8(blk[13], lblk[13])+
			btic1h_errfrac8(blk[14], lblk[14])+
			btic1h_errfrac8(blk[15], lblk[15]);
		break;

	case 15:
	case 21:	case 22:
	case 25:	case 26:
		i=(blk[10]<<16)|(blk[11]<<8)|blk[12];
		j=(blk[13]<<16)|(blk[14]<<8)|blk[15];
		i0=btic1h_errfrac12to8_tab[(i>>12)&4095];
		i1=btic1h_errfrac12to8_tab[(i    )&4095];
		i2=btic1h_errfrac12to8_tab[(j>>12)&4095];
		i3=btic1h_errfrac12to8_tab[(j    )&4095];
		i=(lblk[10]<<16)|(lblk[11]<<8)|lblk[12];
		j=(lblk[13]<<16)|(lblk[14]<<8)|lblk[15];
		j0=btic1h_errfrac12to8_tab[(i>>12)&4095];
		j1=btic1h_errfrac12to8_tab[(i    )&4095];
		j2=btic1h_errfrac12to8_tab[(j>>12)&4095];
		j3=btic1h_errfrac12to8_tab[(j    )&4095];

		e3=	btic1h_errfrac8(i0, j0)+
			btic1h_errfrac8(i1, j1)+
			btic1h_errfrac8(i2, j2)+
			btic1h_errfrac8(i3, j3);
		break;

	case 18:	case 19:
	case 23:
		e3=btic1h_errfrac8(blk[6], lblk[6])*4;
		break;

	case 20:
		e3=	btic1h_errfrac8(blk[12], lblk[12])+
			btic1h_errfrac8(blk[13], lblk[13])+
			btic1h_errfrac8(blk[14], lblk[14])+
			btic1h_errfrac8(blk[15], lblk[15]);
		break;

	default:
		e3=16*9;
		break;
	}

//	e0=btic1h_subfold(cy1, cy);
//	e1=(cd>cd1)?cd:cd1;
//	e2=e0+((e1*e3)>>7);
//	e1=cd+cd1;
//	e2=(e0-cd-cd1)+((e1*e3)>>7);

//	e1=(cd+cd1)>>1;
//	e2=e0+((e1*e3)>>7);
	e2=edy+((dcy*e3)>>7);

	if(rerr)*rerr=e2+euv;
	return(e2<dyem);
#endif

//	return(0);
}

int BTIC1H_EncodeCopyBlocks(
	byte *dblk, byte *sblk, int nblks, int stride)
{
	byte *ct, *cs, *cse;
	if(stride==32)
	{
		ct=dblk; cs=sblk; cse=sblk+nblks*32;
		while(cs<cse)
		{
			((u32 *)ct)[0]=((u32 *)cs)[0];
			((u32 *)ct)[1]=((u32 *)cs)[1];
			((u32 *)ct)[2]=((u32 *)cs)[2];
			((u32 *)ct)[3]=((u32 *)cs)[3];
			((u32 *)ct)[4]=((u32 *)cs)[4];
			((u32 *)ct)[5]=((u32 *)cs)[5];
#ifdef BT1H_ENABLE_AX
			if((cs[3]==0) && (cs[4]==7))
			{	((u32 *)ct)[6]=((u32 *)cs)[6];
				((u32 *)ct)[7]=((u32 *)cs)[7];	}
#else
			((u32 *)ct)[6]=((u32 *)cs)[6];
			((u32 *)ct)[7]=((u32 *)cs)[7];
#endif
			ct+=32; cs+=32;
		}
	}else
	{
		memcpy(dblk, sblk, nblks*stride);
	}
	return(0);
}

int BTIC1H_EncodeBlocksCtx(BTIC1H_Context *ctx,
	byte *blks, byte *lblks, int nblks, int stride,
	int *rnblks, int qf)
{
	static const int hilbert4x4[16]=
		{ 0, 1, 5, 4, 8, 12, 13, 9, 10, 14, 15, 11, 7, 6, 2, 3 };
	static const int hilbert2x2[4]=
		{ 0, 2, 3, 1 };
	static const int voffs[6*4][2]={
		{-1, 0},{ 0,-1},{1, 0},{ 0, 1},
		{-1, 1},{ 1, 1},{1,-1},{-1,-1},
		{-2, 0},{ 0,-2},{2, 0},{ 0, 2},
		{-2, 1},{ 1,-2},{2, 1},{ 1, 2},
		{-2,-1},{-1,-2},{2,-1},{-1, 2},
		{-2, 2},{ 2, 2},{2,-2},{-2,-2}
		};

	BTIC1H_PTune *pt;
	byte *cs2, *csl2;
	byte *cs, *cse, *csl, *csle, *csle2, *lblks2, *cs1, *cs1e;
	double qrf;
	int bit0;
	int setyuv;
	int cy, cu, cv, cd, cm;
//	int dye, duve, qr;
	int dyem, duvem, qr;
	int dyen, duven;
	int cy1, cu1, cv1, cd1, cm1;
	int cy2, cu2, cv2;
	int i0, i1, i2, i3;
	int vx, vy, vo;
	int i, j, k, l;
	
//	BTIC1H_InitQuant();

	qr=100-(qf&127);
	if(qr<0)qr=0;

//	dye=qr*0.5;
//	duve=qr*0.75;
//	dye=qr*0.3;
//	duve=qr*0.4;

//	dyem=qr*0.2;
//	duvem=qr*0.3;
//	dyen=qr*0.4;
//	duven=qr*0.6;

//	dyem=qr*0.3;
//	duvem=qr*0.5;
//	dyen=qr*0.6;
//	duven=qr*0.9;

//	dyem=qr*0.5;
//	duvem=qr*0.7;
//	dyen=qr*0.8;
//	duven=qr*1.1;
//	dyen=qr*1.2;
//	duven=qr*1.5;

//	ctx->reqcolorpred=2;

	if((qf&BTIC1H_QFL_DBGPTUNE) &&
		(btic1h_dbg_ptune->parmfl&BTIC1H_PTFL_BLKSKIP))
	{
		pt=btic1h_dbg_ptune;

		qrf=qr/50.0;
		dyem =qr*(pt->dyem +qrf*pt->dyeme );
		duvem=qr*(pt->duvem+qrf*pt->duveme);
		dyen =qr*(pt->dyen +qrf*pt->dyene );
		duven=qr*(pt->duven+qrf*pt->duvene);
	}else
	{
		qrf=qr/50.0;
#if 0
		dyem=qr*(0.3+qrf*0.25);
		duvem=qr*(0.5+qrf*0.25);
		dyen=qr*(0.6+qrf*0.5);
		duven=qr*(0.8+qrf*0.5);
#endif

#if 1
		dyem=qr*(0.75+qrf*0.55);
		duvem=qr*(0.77+qrf*0.52);
		dyen=qr*(0.98+qrf*0.93);
		duven=qr*(1.30+qrf*0.93);
#endif

#if 0
		dyem=qr*(0.88+qrf*0.73);
		duvem=qr*(0.92+qrf*0.76);
		dyen=qr*(1.18+qrf*1.01);
		duven=qr*(1.22+qrf*1.04);
#endif

#if 0
		dyem=qr*(0.90+qrf*0.90);
		duvem=qr*(1.00+qrf*0.80);
		dyen=qr*(1.10+qrf*1.10);
		duven=qr*(1.40+qrf*1.10);
#endif

#if 0
		dyem=qr*(1.39+qrf*1.27);
		duvem=qr*(1.33+qrf*1.35);
		dyen=qr*(1.53+qrf*1.36);
		duven=qr*(1.72+qrf*1.59);
#endif
	}

	if((qf&BTIC1H_QFL_DBGPTUNE) &&
		(btic1h_dbg_ptune->parmfl&BTIC1H_PTFL_LQUANT))
	{
		pt=btic1h_dbg_ptune;
		if(lblks)
		{
			cy=qr*pt->qyp;
			cu=qr*pt->quvp;
			cd=qr*pt->qdyp;
		}else
		{
			cy=qr*pt->qyi;
			cu=qr*pt->quvi;
			cd=qr*pt->qdyi;
		}
	}else
	{
		if(lblks)
		{
			cy=qr*0.50;
			cu=qr*0.60;
			cd=qr*0.75;

//			cy=qr*0.70;
//			cu=qr*0.75;
//			cd=qr*0.90;

//			cy=qr*0.75;
//			cu=qr*0.79;
//			cd=qr*0.93;

//			cy=qr/5;
//			cu=qr/4;
//			cd=qr/3;
		}else
		{
			cy=qr*0.40;
			cu=qr*0.45;
			cd=qr*0.50;

//			cy=qr/6.5;
//			cu=qr/6;
//			cd=qr/5;
		}
	}

//	cy=(100-qf)/4;
//	cu=(100-qf)/3;
//	cy=(100-qf)/8;
//	cu=(100-qf)/6;

//	cy=(100-qf)/6.5;
//	cu=(100-qf)/6;
//	cd=(100-qf)/5;

	if(cy<1)cy=1;
	if(cu<1)cu=1;
	if(cd<1)cd=1;
	
	setyuv=0;
	

	cs=blks; cse=cs+nblks*stride;
	csl=lblks; csle=lblks+nblks*stride;

	if(lblks && ctx->lblks && (lblks>=ctx->lblks) &&
		(lblks<(ctx->lblks+(ctx->nblks*stride))))
	{
		lblks2=ctx->lblks;
		csle2=ctx->lblks+(ctx->nblks*stride);
	}else
	{
		lblks2=lblks;
		csle2=csle;
	}

	BTIC1H_EmitCommandCode(ctx, 0x04);
	BTIC1H_EmitDeltaQfYUVD(ctx, cy, cu, cd);

	BTIC1H_EmitCommandCode(ctx, 0x08);
	BTIC1H_EmitDeltaQfYUVDyuv(ctx, cy, cu, cd, cu);

	while(cs<cse)
	{
		if(lblks)
		{
			for(i=0; i<32; i++)
			{
				cs2=cs+i*stride;
				csl2=csl+i*stride;
				if(cs2>=cse)break;
//				if(!BTIC1H_CheckSkip(ctx, cs2, csl2, stride, qf, dye, duve))
//					break;
				if(BTIC1H_CheckSkip(ctx, cs2, csl2, stride,
						qf, dyem, dyen, duvem, duven, &k)<=0)
					break;
			}
			
			if(i>0)
			{
//				*ct++=0x80+(i-1);
				BTIC1H_EmitCommandCode(ctx, 0x21);
				BTIC1H_Rice_WriteAdRice(ctx, i, &(ctx->rk_cmdcnt));

				BTIC1H_EncodeCopyBlocks(cs, csl, i, stride);
//				memcpy(cs, csl, i*stride);

				cs+=i*stride;
				csl+=i*stride;
//				t1=GPIO_GetTimeUS();
				continue;
			}

#ifdef BT1H_USETSKIP
			i=0;
//			for(j=0; j<4; j++)
			for(j=0; j<BT1H_TSKIP_MAX; j++)
			{
				vx=voffs[j][0];
				vy=voffs[j][1];

				vo=(vy*ctx->xbsz+vx)*stride;
				for(i=0; i<32; i++)
				{
					cs2=cs+i*stride;
					csl2=csl+i*stride+vo;
					if(cs2>=cse)break;
					if((csl2<lblks2) || (csl2>=csle2))
						break;
					if(BTIC1H_CheckSkip(ctx, cs2, csl2, stride,
							qf, dyem, dyen, duvem, duven, &k)<=0)
						break;
				}
				if(i>0)break;
			}

			if(i>0)
			{
				BTIC1H_EmitCommandCode(ctx, 0x22);
				BTIC1H_Rice_WriteAdRice(ctx, i, &(ctx->rk_cmdcnt));
				BTIC1H_Rice_WriteAdSRice(ctx, vx, &(ctx->rk_parmxy));
				BTIC1H_Rice_WriteAdSRice(ctx, vy, &(ctx->rk_parmxy));

				csl2=csl+vo;

				BTIC1H_EncodeCopyBlocks(cs, csl2, i, stride);
//				memcpy(cs, csl2, i*stride);

				cs+=i*stride;
				csl+=i*stride;
//				t1=GPIO_GetTimeUS();
				continue;
			}
#endif
		}

		BTIC1H_EmitCheckEnableDeltaMask(ctx);

		cy=cs[0]; cu=cs[1]; cv=cs[2]; cd=cs[3];
		
		if(cd)
		{
			if(!setyuv)
			{
				BTIC1H_EmitCommandCode(ctx, 0x40);
				BTIC1H_EmitAbsYUVD(ctx, cy, cu, cv, cd);
//				BTIC1H_EmitAbsYUVD(ctx, 0, 128, 128, 0);
//				BTIC1H_EmitAbsYUVD(ctx, 128, 128, 128, 0);
				setyuv=1;
			}
			
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
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

				BTIC1H_EmitCommandCode(ctx, 0x15);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x4+=32;
					cs+=stride;
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x05);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4+=32;
			cs+=stride;
			csl+=stride;
			continue;
		}
		
		cm=cs[4];
		cd=cs[5];

		if(!setyuv)
		{
			BTIC1H_EmitCommandCode(ctx, 0x40);
			BTIC1H_EmitAbsYUVD(ctx, cy, cu, cv, cd);
//			BTIC1H_EmitAbsYUVD(ctx, 0, 128, 128, 0);
//			BTIC1H_EmitAbsYUVD(ctx, 128, 128, 128, 0);
			setyuv=1;
		}
		
		if(cm==0)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUV(ctx, cy, cu, cv))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
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
				csl+=l*stride;
				continue;
			}
#endif

#if 1
			cs1=cs+stride;
			cs1e=cs+256*stride;
			if(cse<cs1e)cs1e=cse;
			cy2=ctx->cy; cu2=ctx->cu; cv2=ctx->cv; k=0;
			while(cs1<cs1e)
			{
				if(cs1[3] || (cs1[4]!=0))
					break;
				cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2];
				if(!BTIC1H_CheckRefDeltaYUV(ctx,
						&cy2, &cu2, &cv2, cy1, cu1, cv1))
					{ if((++k)>=3)break; }
				else
					{ k=0; }
				cs1+=stride;
			}
			
			cs1-=k*stride;
			l=(cs1-cs)/stride;
			if(l>=3)
			{
				BTIC1H_EmitCommandCode(ctx, 0x1B);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_EmitDeltaYUV(ctx,
						cs[0], cs[1], cs[2]);
					cs+=stride;
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x00);
			BTIC1H_EmitDeltaYUV(ctx, cy, cu, cv);
			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==1)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
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
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x01);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
			ctx->bits_pix2x2+=8;
			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==2)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=2))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x1A);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x4x1+=16;
					cs+=stride;
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x2A);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4x1+=16;
			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==5)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[3] || (cs1[4]!=5))
						break;
					cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
					if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x19);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write4Bits(ctx, cs[6]);
					ctx->bits_pix4x4x1+=4;
					cs+=stride;
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x2B);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write4Bits(ctx, cs[6]);
			ctx->bits_pix4x4x1+=4;
			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==6)
		{
			BTIC1H_EmitCommandCode(ctx, 0x2C);
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[10]<<8)|cs[11]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[12]<<8)|cs[13]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[14]<<8)|cs[15]);
			ctx->bits_pix4x4x3+=48;
			cs+=stride;
			csl+=stride;
			continue;
		}

#ifdef BT1H_USEHV
		if((cm==8) || (cm==9))
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
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
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x02+(cm-8));
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write4Bits(ctx, cs[6]);
			ctx->bits_pix2x1+=4;
			cs+=stride;
			csl+=stride;
			continue;
		}
#endif

#ifdef BT1H_USEHV
		if((cm==10) || (cm==11))
		{
#if 1
			if(BTIC1H_CheckZeroDeltaYUVD(ctx, cy, cu, cv, cd))
			{
				cs1=cs+stride;
				cs1e=cs+256*stride;
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
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x2+=16;
					cs+=stride;
					csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x06+(cm-10));
			BTIC1H_EmitDeltaYUVD(ctx, cy, cu, cv, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x2+=16;
			cs+=stride;
			csl+=stride;
			continue;
		}
#endif

		if(cm==7)
		{
			BTIC1H_EmitCommandCode(ctx, 0x2E);
			for(i=0; i<16; i++)
			{
				BTIC1H_EmitDeltaY(ctx, cs[16+hilbert4x4[i]]);
			}
			for(i=0; i<4; i++)
			{
				BTIC1H_EmitDeltaUV(ctx,
					cs[8+hilbert2x2[i]], cs[12+hilbert2x2[i]]);
			}

			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==17)
		{
			BTIC1H_EmitCommandCode(ctx, 0x1D);

			BTIC1H_EmitDeltaYUVD(ctx, cs[ 0], cs[ 1], cs[ 2], cs[ 5]);
			BTIC1H_EmitDeltaYUVD(ctx, cs[16], cs[17], cs[18], cs[19]);
			BTIC1H_EmitDeltaYUVD(ctx, cs[20], cs[21], cs[22], cs[23]);
			BTIC1H_EmitDeltaYUVD(ctx, cs[ 8], cs[ 9], cs[10], cs[11]);

			BTIC1H_Rice_Write16Bits(ctx, (cs[12]<<8)|cs[13]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[14]<<8)|cs[15]);
			ctx->bits_pix4x4+=32;

			cs+=stride;
			csl+=stride;
			continue;
		}

#if 1
		if(cm==20)
		{
			BTIC1H_EmitCommandCode(ctx, 0x30);

			BTIC1H_EmitDeltaYUVDyuv(ctx,
				cs[ 0], cs[ 1], cs[ 2],
				(cs[5]-128)<<1, (cs[8]-128)<<1,
				(cs[9]-128)<<1);

			BTIC1H_Rice_Write16Bits(ctx, (cs[12]<<8)|cs[13]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[14]<<8)|cs[15]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[10]<<8)|cs[11]);
			ctx->bits_pix4x4+=32;
			ctx->bits_pix2x2+=16;

			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==21)
		{
			BTIC1H_EmitCommandCode(ctx, 0x31);

			BTIC1H_EmitDeltaYUVDyuv(ctx,
				cs[ 0], cs[ 1], cs[ 2],
				(cs[5]-128)<<1, (cs[8]-128)<<1,
				(cs[9]-128)<<1);

			BTIC1H_Rice_Write16Bits(ctx, (cs[10]<<8)|cs[11]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[12]<<8)|cs[13]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[14]<<8)|cs[15]);
			ctx->bits_pix4x4x3+=48;
			BTIC1H_Rice_Write16Bits(ctx, (cs[16]<<8)|cs[17]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[18]<<8)|cs[19]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[20]<<8)|cs[21]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[22]<<8)|cs[23]);
			ctx->bits_pix4x4+=64;

			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==22)
		{
			BTIC1H_EmitCommandCode(ctx, 0x32);

			BTIC1H_EmitDeltaYUVDyuv(ctx,
				cs[ 0], cs[ 1], cs[ 2],
				(cs[5]-128)<<1, (cs[8]-128)<<1,
				(cs[9]-128)<<1);

			BTIC1H_Rice_Write16Bits(ctx, (cs[10]<<8)|cs[11]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[12]<<8)|cs[13]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[14]<<8)|cs[15]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4x3+=48;
			ctx->bits_pix2x2+=16;

			cs+=stride;
			csl+=stride;
			continue;
		}

		if(cm==23)
		{
			BTIC1H_EmitCommandCode(ctx, 0x33);
			BTIC1H_EmitDeltaYUVDyuv(ctx,
				cs[0], cs[1], cs[2],
				(cs[5]-128)<<1, (cs[8]-128)<<1,
				(cs[9]-128)<<1);
			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[10]<<8)|cs[11]);
			ctx->bits_pix2x2+=24;
			cs+=stride;
			csl+=stride;
			continue;
		}
#endif

#ifdef BT1H_USEGRAD
		if(cm==18)
		{
			cs1=cs+stride;
			cs1e=cs+256*stride;
			if(cse<cs1e)cs1e=cse;
			while(cs1<cs1e)
			{
				if(cs1[3] || (cs1[4]!=18))
					break;
//				cy1=cs1[0]; cu1=cs1[1]; cv1=cs1[2]; cd1=cs1[5];
//				if(!BTIC1H_CheckZeroDeltaYUVD(ctx, cy1, cu1, cv1, cd1))
//					break;
				cs1+=stride;
			}
				
			l=(cs1-cs)/stride;
			BTIC1H_EmitCommandCode(ctx, 0x28);
			BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
			for(i=0; i<l; i++)
			{
				BTIC1H_EmitDeltaYUVD(ctx, cs[0], cs[1], cs[2], cs[5]);
				BTIC1H_Rice_Write8Bits(ctx, cs[6]);
				ctx->bits_pix2x2+=8;
				cs+=stride;
				csl+=stride;
			}
			continue;
		}
#endif

		printf("Invalid Block Type %d\n\n", cm);
		break;
	}

	BTIC1H_EmitCommandCode(ctx, 0x20);
	
	*rnblks=(cs-blks)/stride;
	return(0);
}

#ifdef BT1H_ENABLE_AX
int BTIC1H_CheckZeroDeltaA(BTIC1H_Context *ctx, int cy)
{
	int dy, dd, qdy, qdd;
	
	dy=cy-ctx->cy;
	qdy=(dy*ctx->fx_qfy+2048)>>12;

	if(qdy)
		return(0);
	return(1);
}

int BTIC1H_CheckZeroDeltaAD(BTIC1H_Context *ctx,
	int cy, int cd)
{
	int dy, dd, qdy, qdd;
	
	dy=cy-ctx->cy;		dd=cd-ctx->cd;
	qdy=(dy*ctx->fx_qfy+2048)>>12;
	qdd=(dd*ctx->fx_qfd+2048)>>12;

	if(qdy || qdd)
		return(0);
	return(1);
}

int BTIC1H_CheckAlphaSkip(BTIC1H_Context *ctx,
	byte *blk, byte *lblk, int blksz,
	int qf, int dyem, int dyen,
	int *rerr)
{
	int cy, cu, cv, cd, cm;
	int cy1, cu1, cv1, cd1, cm1;
	int ca, cb, ca1, cb1;
	int e0, e1, e2, e3, edy, euv;
	int mcy, ncy, dcy;
	int i, j, k, l;

	if(!memcmp(blk, lblk, 8))
	{
		if(rerr)*rerr=0;
		return(2);
	}

	cy=blk[0]; cd=blk[1];
	cy1=lblk[0]; cd1=lblk[1];

	cm=0; cm1=0;

	if(cd==0)
	{
		cm=blk[3];
		if((cm!=0) && (cm!=7))
			{ cd=blk[2]; }
	}

	if(cd1==0)
	{
		cm1=lblk[3];
		if((cm1!=0) && (cm1!=7))
			{ cd1=lblk[2]; }
	}

#if 0
	if((btic1h_subfold(cy1, cy)>dye) ||
		(btic1h_subfold(cd1, cd)>dye) ||
		(btic1h_subfold(cu1, cu)>duve) ||
		(btic1h_subfold(cv1, cv)>duve))
			return(0);

	ca=cy+(cd>>1);		cb=cy-(cd>>1);
	ca1=cy1+(cd1>>1);	cb1=cy1-(cd1>>1);

//	ca=cy+cd;		cb=cy-cd;
//	ca1=cy1+cd1;	cb1=cy1-cd1;

	if((btic1h_subfold(ca1, cb)<dye) &&
		(btic1h_subfold(cb1, ca)<dye))
			return(1);
#endif

#if 1
	e0=btic1h_subfold(cy1, cy);
	e1=btic1h_subfold(cd1, cd);
	edy=e0;
	
	if((e0>dyen) || (e1>dyen))
	{
		if(rerr)*rerr=e0+(e1>>1);
		return(0);
	}
#endif

#if 1
	ca=cy+(cd>>1);		cb=cy-(cd>>1);
	ca1=cy1+(cd1>>1);	cb1=cy1-(cd1>>1);

	e0=btic1h_subfold(ca1, cb);
	e1=btic1h_subfold(cb1, ca);

	if((e0<dyem) && (e1<dyem))
	{
		e2=(e0>e1)?e0:e1;
		if(rerr)*rerr=e2;
		return(2);
	}
#endif

	e2=(e0>e1)?e0:e1;
	if(rerr)*rerr=e2;

	return(0);
}

int BTIC1H_EncodeCopyAlphaBlocks(
	byte *dblk, byte *sblk, int nblks, int stride)
{
	byte *ct, *cs, *cse;
	if(stride==32)
	{
		ct=dblk; cs=sblk; cse=sblk+nblks*32;
		while(cs<cse)
		{
			((u32 *)ct)[0]=((u32 *)cs)[0];
			((u32 *)ct)[1]=((u32 *)cs)[1];
			ct+=32; cs+=32;
		}
	}else
	{
//		memcpy(dblk, sblk, nblks*stride);
	}
	return(0);
}

int BTIC1H_EncodeAlphaBlocksCtx(BTIC1H_Context *ctx,
	byte *blks, byte *lblks, int nblks, int stride,
	int *rnblks, int qf)
{
	byte *cs2, *csl2;
	byte *cs, *cse, *csl, *csle, *csle2, *lblks2, *cs1, *cs1e;
	int bit0;
	int cy, cu, cv, cd, cm;
	int dyem, dyen, qr;
	int cy1, cu1, cv1, cd1, cm1;
	int cy2, cu2, cv2;
	int i0, i1, i2, i3;
	int vx, vy, vo;
	int i, j, k, l;
	
//	BTIC1H_InitQuant();

	qr=100-(qf&127);
	if(qr<0)qr=0;

	dyem=qr*0.2;
	dyen=qr*0.4;

	cs=blks; cse=cs+nblks*stride;
	csl=lblks; csle=lblks+nblks*stride;

	if(lblks && ctx->lblks && (lblks>=ctx->lblks) &&
		(lblks<(ctx->lblks+(ctx->nblks*stride))))
	{
		lblks2=ctx->lblks;
		csle2=ctx->lblks+(ctx->nblks*stride);
	}else
	{
		lblks2=lblks;
		csle2=csle;
	}

	cy=qr/6.5;
	cd=qr/5;

	if(cy<1)cy=1;
	if(cd<1)cd=1;
	
	BTIC1H_EmitCommandCode(ctx, 0x04);
	BTIC1H_EmitDeltaQfAD(ctx, cy, cd);

	while(cs<cse)
	{
		if(lblks)
		{
			for(i=0; i<32; i++)
			{
				cs2=cs+i*stride;
				csl2=csl+i*stride;
				if(cs2>=cse)break;
				if(BTIC1H_CheckAlphaSkip(ctx, cs2, csl2, stride,
						qf, dyem, dyen, &k)<=0)
					break;
			}
			
			if(i>0)
			{
				BTIC1H_EmitCommandCode(ctx, 0x21);
				BTIC1H_Rice_WriteAdRice(ctx, i, &(ctx->rk_cmdcnt));
				
				BTIC1H_EncodeCopyAlphaBlocks(cs, csl, i, stride);
				cs+=i*stride;		csl+=i*stride;
				continue;
			}
		}

		if(cs[1]!=0)
		{
			cy=cs[0];
			cd=cs[1];

			BTIC1H_EmitCommandCode(ctx, 0x03);
			BTIC1H_EmitDeltaAD(ctx, cy, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[2]<<8)|cs[3]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4x3+=48;
			cs+=stride;		csl+=stride;
			continue;
		}

		cy=cs[0];
		cd=cs[2];
		cm=cs[3];

		if(cm==7)
		{
#if 1
			if(cy==0)
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[1] || (cs1[3]!=cm))
						break;
					if(cs1[2]!=0)
						break;
					cs1+=stride;
				}
			
				l=(cs1-cs)/stride;
				if(l>1)
				{
					BTIC1H_EmitCommandCode(ctx, 0x12);
					BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
					cs+=l*stride; csl+=l*stride;
					continue;
				}

				BTIC1H_EmitCommandCode(ctx, 0x11);
				cs+=stride;		csl+=stride;
				continue;
			}
#endif
		
#if 1
			if(BTIC1H_CheckZeroDeltaA(ctx, cy))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[1] || (cs1[3]!=cm))
						break;
					cy1=cs1[0]; cd1=cs1[2];
					if(!BTIC1H_CheckZeroDeltaA(ctx, cy1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x0C);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
					{ cs+=stride; csl+=stride; }
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x00);
			BTIC1H_EmitDeltaA(ctx, cy);
			cs+=stride;		csl+=stride;
			continue;
		}

		if(cm==1)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaAD(ctx, cy, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[1] || (cs1[3]!=cm))
						break;
					cy1=cs1[0]; cd1=cs1[2];
					if(!BTIC1H_CheckZeroDeltaAD(ctx, cy1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x0D);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write8Bits(ctx, cs[6]);
					ctx->bits_pix2x2+=8;
					cs+=stride;		csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x01);
			BTIC1H_EmitDeltaAD(ctx, cy, cd);
			BTIC1H_Rice_Write8Bits(ctx, cs[6]);
			ctx->bits_pix2x2+=8;
			cs+=stride;		csl+=stride;
			continue;
		}

		if(cm==6)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaAD(ctx, cy, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[1] || (cs1[3]!=cm))
						break;
					cy1=cs1[0]; cd1=cs1[2];
					if(!BTIC1H_CheckZeroDeltaAD(ctx, cy1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x0E);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x4+=32;
					cs+=stride;		csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x02);
			BTIC1H_EmitDeltaAD(ctx, cy, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[4]<<8)|cs[5]);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4+=32;
			cs+=stride;		csl+=stride;
			continue;
		}

		if(cm==2)
		{
#if 1
			if(BTIC1H_CheckZeroDeltaAD(ctx, cy, cd))
			{
				cs1=cs+stride;
				cs1e=cs+32*stride;
				if(cse<cs1e)cs1e=cse;
				while(cs1<cs1e)
				{
					if(cs1[1] || (cs1[3]!=cm))
						break;
					cy1=cs1[0]; cd1=cs1[2];
					if(!BTIC1H_CheckZeroDeltaAD(ctx, cy1, cd1))
						break;
					cs1+=stride;
				}
				
				l=(cs1-cs)/stride;
				BTIC1H_EmitCommandCode(ctx, 0x0F);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				for(i=0; i<l; i++)
				{
					BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
					ctx->bits_pix4x4x1+=16;
					cs+=stride;		csl+=stride;
				}
				continue;
			}
#endif

			BTIC1H_EmitCommandCode(ctx, 0x05);
			BTIC1H_EmitDeltaAD(ctx, cy, cd);
			BTIC1H_Rice_Write16Bits(ctx, (cs[6]<<8)|cs[7]);
			ctx->bits_pix4x4x1+=32;
			cs+=stride;		csl+=stride;
			continue;
		}

		if(cm==0)
		{
			cs1=cs+stride;
			cs1e=cs+32*stride;
			if(cse<cs1e)cs1e=cse;
			while(cs1<cs1e)
			{
				if(cs1[1] || (cs1[3]!=cm))
					break;
				cs1+=stride;
			}
			
			l=(cs1-cs)/stride;
			if(l>1)
			{
				BTIC1H_EmitCommandCode(ctx, 0x0B);
				BTIC1H_Rice_WriteAdRice(ctx, l, &(ctx->rk_cmdcnt));
				cs+=l*stride; csl+=l*stride;
				continue;
			}

			BTIC1H_EmitCommandCode(ctx, 0x0A);
			cs+=stride;		csl+=stride;
			continue;
		}

		printf("Invalid Alpha Block Type %d\n\n", cm);
		break;
	}

	BTIC1H_EmitCommandCode(ctx, 0x20);
	
	*rnblks=(cs-blks)/stride;
	return(0);
}
#endif

void BTIC1H_SetupContextInitial(BTIC1H_Context *ctx)
{
	int i, j, k;

	ctx->mark1=0x12345678;
	ctx->mark2=0x12488421;
	ctx->mark3=0x87654321;

	for(i=0; i<16; i++)
		{ ctx->cmdwin[i]=-1; }

	for(i=0; i<64; i++)
		{ ctx->cmdidx[i]=-1; }

	ctx->cmdwpos=0;

	ctx->cy=0;	ctx->cu=0;
	ctx->cv=0;	ctx->cd=0;
	
	ctx->absyuvbias=128;
	ctx->dyuv=0;
	ctx->updmask=0;
	ctx->nextupdmask=0;
	ctx->absupdmask=0;
	ctx->nextabsupdmask=0;
	ctx->tgtupdmask=0;
	ctx->colorpred=0;
	
	ctx->cdy=0;		ctx->cdu=0;		ctx->cdv=0;

	ctx->lcy=0;	ctx->lcu=0;
	ctx->lcv=0;	ctx->lcd=0;
	ctx->lcdy=0; ctx->lcdu=0; ctx->lcdv=0;

	ctx->qfy=1;		ctx->qfuv=1;	ctx->qfd=1;
	ctx->qfdy=1;	ctx->qfduv=1;

	ctx->qfay=1;	ctx->qfauv=1;	ctx->qfad=1;
	ctx->qfady=1;	ctx->qfaduv=1;

	ctx->fx_qfy  =4096;		ctx->fx_qfuv  =4096;
	ctx->fx_qfdy =4096;		ctx->fx_qfduv =4096;
	ctx->fx_qfay =4096;		ctx->fx_qfauv =4096;
	ctx->fx_qfady=4096;		ctx->fx_qfaduv=4096;
	ctx->fx_qfd  =4096;		ctx->fx_qfad  =4096;

	ctx->rk_cmdidx=2;	ctx->rk_cmdabs=4;
	ctx->rk_cmdcnt=2;

	ctx->rk_parmxy=2;	ctx->rk_parmvar=2;
	ctx->rk_parmval=2;	ctx->rk_parmix=2;
	ctx->rk_maskidx=2;

	ctx->rk_dy=2;		ctx->rk_duv=2;
	ctx->rk_ddy=2;		ctx->rk_dduv=2;

	ctx->rk_ay=5;		ctx->rk_auv=5;
	ctx->rk_ady=5;		ctx->rk_aduv=5;

	ctx->rk_qfy=3;		ctx->rk_qfuv=3;
	ctx->rk_qfdy=3;		ctx->rk_qfduv=3;

#if 0
	ctx->cnt_cmds=0;		ctx->bits_cmdidx=0;
	ctx->bits_cmdabs=0;

	ctx->cnt_parms=0;		ctx->bits_parm=0;

	ctx->bits_dumask=0;
	ctx->bits_dyuv=0;		ctx->bits_dy=0;
	ctx->bits_duv=0;		ctx->bits_ddy=0;
	ctx->bits_dduv=0;

	ctx->bits_pix4x4=0;		ctx->bits_pix4x4x1=0;
	ctx->bits_pix4x4x3=0;	ctx->bits_pix4x2=0;
	ctx->bits_pix2x2=0;		ctx->bits_pix2x1=0;
#endif

	ctx->cnt_dpts=0;
	ctx->cnt_dzpts=0;
	
	for(i=0; i<8; i++)
	{
		ctx->cnt_dcpts[i]=0;
		ctx->cnt_dczpts[i]=0;
	}

	for(i=0; i<256; i++)
	{
//		j=~i;
		j=255-i;
		ctx->maskwin[i]=j;
		ctx->maskidx[j]=i;
	}

	for(i=0; i<256; i++)
		{ ctx->cmdwin[i]=i; }
	for(i=0; i<256; i++)
		{ ctx->cmdidx[i]=i; }
}

void BTIC1H_SetupDecodeContextInitial(BTIC1H_Context *ctx)
{
	int i, j, k;

	BTIC1H_SetupContextInitial(ctx);
}

int BTIC1H_EncodeWorkSliceCtx(BTIC1H_Context *ctx)
{
	byte *ct, *cte;
	int sys, yb, nblks, str;
	int i, j, k;
	
	ct=ctx->bs_cts;
	cte=ctx->bs_cte;
	nblks=ctx->slbs;

	switch(ctx->clrs)
	{
	case BTIC1H_PXF_RGBA:
	case BTIC1H_PXF_BGRA:
	case BTIC1H_PXF_RGBX:
	case BTIC1H_PXF_BGRX:
		str=4; break;
	case BTIC1H_PXF_RGB:
	case BTIC1H_PXF_BGR:
		str=3; break;
	case BTIC1H_PXF_UYVY:
	case BTIC1H_PXF_YUYV:
		str=2; break;
	default:
		str=4; break;
	}

	if(ctx->flip)
	{
		yb=ctx->slix*ctx->slscl;
		k=yb*ctx->xs*str;
		sys=ctx->slscl;
		if((yb+sys)>ctx->ys)
		{
			sys=ctx->ys-yb;
			
			i=((sys+3)/4);
			nblks=i*ctx->xbsz;
		}
	}else
	{
		yb=(ctx->slix+1)*ctx->slscl;
		yb=ctx->ys-yb;
		k=yb*ctx->xs*str;
		sys=-ctx->slscl;
//		if((yb+sys)<0)
//			sys=yb;
//			sys=-yb;

		if(yb<0)
		{
			sys-=yb;
			yb=0; k=0;
//			k=yb*ctx->xs*4;

			i=(((-sys)+3)/4);
			nblks=i*ctx->xbsz;
		}
	}

	j=ctx->slix*ctx->slbs*32;

	BTIC1H_EncodeImageClrs(
		ctx->blks+j, 32, ctx->ibuf+k,
		ctx->xs, sys, ctx->qfl, ctx->clrs);

	BTIC1H_SetupContextInitial(ctx);
	for(j=0; j<256; j++)
		{ ctx->cmdwin[j]=j; ctx->cmdidx[j]=j; }

	ctx->EmitCommandCode=BTIC1H_EmitCommandCodeSMTF;

	BTIC1H_Rice_SetupWrite(ctx, ct, cte-ct);
	
	j=ctx->slix*ctx->slbs*32;
	BTIC1H_EncodeBlocksCtx(ctx,
		ctx->blks+j, (ctx->qfl&BTIC1H_QFL_PFRAME)?(ctx->lblks+j):NULL,
		nblks, 32, &k, ctx->qfl);
	BTIC1H_Rice_FlushBits(ctx);
	
	ctx->bs_cts=ct;
	j=ctx->bs_ct-ct;
	return(0);
}

BTIC1H_API int BTIC1H_EncodeCtx(BTIC1H_Context *ctx,
	byte *src, byte *dst, int dsz,
	int xs, int ys, int qfl, int clrs)
{
	BTIC1H_Context *encjob[128];
	BTIC1H_Context *etctx;
	byte *ct, *cte, *ct1;
	int sz, sz1, csim;
	int xs1, ys1, qfl1;
	int slys, slbs, sldbs, sltid;
	int i, j, k, l, n;

	if(!ctx->blks)
	{
		ctx->xbsz=(ctx->xs+3)>>2;
		ctx->ybsz=(ctx->ys+3)>>2;
		ctx->nblks=ctx->xbsz*ctx->ybsz;
		
		n=(ctx->xbsz+1)*(ctx->ybsz+1);
		ctx->blks=malloc(n*32);
		ctx->lblks=malloc(n*32);
		
#ifdef BT1H_ENABLE_MX
//		if(qfl&BTIC1H_QFL_USEMIP)
//			{ ctx->mtibuf=malloc(ctx->xs*ctx->ys*4); }
#endif
	}else if(qfl&(BTIC1H_QFL_USEMIP|BTIC1H_QFL_ISMIPLVL))
	{
		ctx->xs=xs;
		ctx->ys=ys;
		ctx->xbsz=(ctx->xs+3)>>2;
		ctx->ybsz=(ctx->ys+3)>>2;
		ctx->nblks=ctx->xbsz*ctx->ybsz;
	}

	ctx->clrs=clrs;
	ctx->qfl=qfl;
	ctx->clryuv=0;
	ctx->clrdty=0;

//	BTIC1H_EncodeImageClrs(ctx->blks, 32, src,
//		ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), qfl, clrs);

//	BTIC1H_SetupContextInitial(ctx);

#if 0
	ct=dst+4; cte=dst+dsz;
	ctx->EmitCommandCode=BTIC1H_EmitCommandCodeBase;
	for(i=0; i<64; i++)
		ctx->cmdidx[i]=-1;	
	BTIC1H_Rice_SetupWrite(ctx, dst+4, dsz-4);
#endif

#if 0
	ct=dst+6; cte=dst+dsz;
	*ct++=0x00;
	
	ctx->EmitCommandCode=BTIC1H_EmitCommandCodeSMTF;
	for(i=0; i<256; i++)
		{ ctx->cmdwin[i]=i; ctx->cmdidx[i]=i; }
	BTIC1H_Rice_SetupWrite(ctx, ct, cte-ct);
#endif

#if 0
	ct=dst+6; cte=dst+dsz;
	*ct++=((12-8)<<4)|1;
	
	ctx->EmitCommandCode=BTIC1H_EmitCommandCodeSMTF;
	for(i=0; i<256; i++)
		{ ctx->cmdwin[i]=i; ctx->cmdidx[i]=i; }
	BTIC1H_Rice_SetupWriteRangeBits(ctx, ct, cte-ct, 12);
#endif

	if(qfl&BTIC1H_QFL_USEGDBDR)
	{
		ctx->clryuv=2;
	}

#if 1
//	if((qfl&127)<75)
//	if(1)
	if((qfl&BTIC1H_QFL_USERC) ||
		((qfl&BTIC1H_QFL_USERC66) && ((qfl&127)<66)))
	{
		csim=1;
		if(csim==ctx->lcsim)
//		if(0)
			{ ct=dst+4; cte=dst+dsz; }
		else
			{ ct=dst+6; cte=dst+dsz; }

		*ct++=((12-8)<<4)|1;
		ctx->slscl=0;

		if((ctx->clryuv!=0) || (ctx->clrdty!=0))
		{
			ct[-1]|=0x80;
			*ct++=0xA2;
			*ct++=(ctx->clryuv&7)|
				((ctx->clrdty&7)<<3);
		}

		ctx->EmitCommandCode=BTIC1H_EmitCommandCodeSMTF;
		for(i=0; i<256; i++)
			{ ctx->cmdwin[i]=i; ctx->cmdidx[i]=i; }
		BTIC1H_Rice_SetupWriteRangeBits(ctx, ct, cte-ct, 12);
	}else
	{
		csim=1;
		if(csim==ctx->lcsim)
//		if(0)
			{ ct=dst+4; cte=dst+dsz; }
		else
			{ ct=dst+6; cte=dst+dsz; }

		if(qfl&BTIC1H_QFL_USESLICE)
		{
			i=ctx->ys/32;
//			i=ctx->ys/16;
//			i=ctx->ys/64;
			i=(i+1)&(~3);
			if(!i)i=4;
			ctx->slscl=i;

			*ct++=0x80;
//#ifdef BT1H_USEGDBDR
//			if(qfl&BTIC1H_QFL_USEGDBDR)
//			{
//				*ct++=0xA2;
//				*ct++=0x02;
//			}
//#endif

			if((ctx->clryuv!=0) || (ctx->clrdty!=0))
			{
				*ct++=0xA2;
				*ct++=(ctx->clryuv&7)|
					((ctx->clrdty&7)<<3);
			}

			*ct++=0x21;
			*ct++=i/4;
		}else
		{
			if((ctx->clryuv!=0) || (ctx->clrdty!=0))
			{
				*ct++=0x80;
				*ct++=0x22;
				*ct++=(ctx->clryuv&7)|
					((ctx->clrdty&7)<<3);
			}else
			{
				*ct++=0x00;
			}
			ctx->slscl=0;
		}
	
		ctx->EmitCommandCode=BTIC1H_EmitCommandCodeSMTF;
		for(i=0; i<256; i++)
			{ ctx->cmdwin[i]=i; ctx->cmdidx[i]=i; }
		BTIC1H_Rice_SetupWrite(ctx, ct, cte-ct);
	}
#endif

	if(ctx->slscl)
//	if(0)
	{
		slys=(ctx->ys+(ctx->slscl-1))/ctx->slscl;
		slbs=((ctx->xs+3)>>2)*((ctx->slscl+3)>>2);
		sldbs=(dsz-1024)/slys;

		j=0; k=0;
		for(i=0; i<4; i++)
		{
			if(ctx->cnt_pred[i]>k)
				{ j=i; k=ctx->cnt_pred[i]; }
		}
		ctx->reqcolorpred=j;

		if(btic1h_workqueue_defaultworkers>0)
		{
			sltid=BTIC1H_Work_AllocTidNb();

			for(i=0; i<slys; i++)
			{
				etctx=BTIC1H_AllocContext();
				encjob[i]=etctx;

				etctx->xs=ctx->xs;
				etctx->ys=ctx->ys;
				etctx->clrs=clrs;
				etctx->qfl=qfl;
				etctx->flip=ctx->flip;

				etctx->xbsz=ctx->xbsz;
				etctx->ybsz=ctx->ybsz;
				etctx->nblks=ctx->nblks;

				etctx->ibuf=src;
				etctx->blks=ctx->blks;
				etctx->lblks=ctx->lblks;
				
				etctx->bs_cts=(dst+1024)+i*sldbs;
				etctx->bs_cte=(dst+1024)+(i+1)*sldbs;
				etctx->slscl=ctx->slscl;
				etctx->slys=slys;
				etctx->slbs=slbs;
				etctx->slix=i;
				etctx->reqcolorpred=ctx->reqcolorpred;
				
				etctx->DoWork=BTIC1H_EncodeWorkSliceCtx;
				
//				BTIC1H_Work_QueueJob(encjob[i], sltid);
			}

#if 1
			btic1h_thLockQueue();
//			sltid=BTIC1H_Work_AllocTidNb();
			for(i=0; i<slys; i++)
				{ BTIC1H_Work_QueueJobNb(encjob[i], sltid); }
//			for(i=slys-1; i>=0; i--)
//				{ BTIC1H_Work_QueueJobNb(encjob[i], sltid); }
			btic1h_thUnlockQueue();
#endif
			BTIC1H_Work_CheckSpawnWorkers(
				btic1h_workqueue_defaultworkers);
			
			BTIC1H_Work_WaitTaskComplete(sltid);
//			while(BTIC1H_Work_GetTidCountNb(sltid)>0)
//				{ thSleep(1); }
			BTIC1H_Work_FreeTidNb(sltid);

			for(i=0; i<slys; i++)
			{
				etctx=encjob[i];

				k=etctx->bs_ct-etctx->bs_cts;

#if 1
				if(k<126)
				{
					if(k<=0)
					{
						printf("BTIC1H_EncodeCtx: "
							"Bad Slice Length %d\n", k);
						*ct++=0;
						continue;
					}
					
					j=k+1;
					ct[0]=j;
					memcpy(ct+1, etctx->bs_cts, k);
					ct+=j;
				}else if(k<16379)
				{
					j=k+2;
					ct[0]=0x80+((j>>8)&63);
					ct[1]=j;
					memcpy(ct+2, etctx->bs_cts, k);
					ct+=j;
				}else if(k<2097147)
				{
					j=k+3;
					ct[0]=0xC0+((j>>16)&31);
					ct[1]=j>> 8;
					ct[2]=j    ;
					memcpy(ct+3, etctx->bs_cts, k);
					ct+=j;
				}else
				{
					printf("BTIC1H_EncodeCtx: "
						"Debug Slice Length %d\n", k);

					j=k+4;
					ct[0]=0xE0+((j>>24)&15);
					ct[1]=j>>16;
					ct[2]=j>> 8;
					ct[3]=j    ;
					memcpy(ct+4, etctx->bs_cts, k);
					ct+=j;
				}
#endif

#if 0
				if(k>2097147)
				{
					j=k+4;
					ct[0]=0xE0+((j>>24)&15);
					ct[1]=j>>16;
					ct[2]=j>> 8;
					ct[3]=j    ;
					memcpy(ct+4, etctx->bs_cts, k);
					ct+=j;
				}else if(k>16379)
				{
					j=k+3;
					ct[0]=0xC0+((j>>16)&31);
					ct[1]=j>> 8;
					ct[2]=j    ;
					memcpy(ct+3, etctx->bs_cts, k);
					ct+=j;
				}else if(k>122)
				{
					j=k+2;
					ct[0]=0x80+((j>>8)&63);
					ct[1]=j;
					memcpy(ct+2, etctx->bs_cts, k);
					ct+=j;
				}else
				{
					j=k+1;
					ct[0]=j;
					memcpy(ct+1, etctx->bs_cts, k);
					ct+=j;
				}
#endif
				BTIC1H_AccumEncodeStats(ctx, etctx);
				BTIC1H_FreeContextF(etctx);
			}
			ctx->bits_total+=ctx->bs_bits;
		}else
		{
			BTIC1H_EncodeImageClrs(ctx->blks, 32, src,
				ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), qfl, clrs);

			for(i=0; i<slys; i++)
			{
				ct1=ct+3;
				BTIC1H_SetupContextInitial(ctx);
				for(j=0; j<256; j++)
					{ ctx->cmdwin[j]=j; ctx->cmdidx[j]=j; }

				BTIC1H_Rice_SetupWrite(ctx, ct1, cte-ct1);
				
				n=slbs;
				if((i*slbs+n)>ctx->nblks)
					n=ctx->nblks-i*slbs;
				
				j=i*slbs*32;
				BTIC1H_EncodeBlocksCtx(ctx,
					ctx->blks+j,
					(qfl&BTIC1H_QFL_PFRAME)?(ctx->lblks+j):NULL,
					n, 32, &k, qfl);
				ctx->bits_total+=ctx->bs_bits;
				BTIC1H_Rice_FlushBits(ctx);

				j=ctx->bs_ct-ct;
				ct[0]=0xC0+((j>>16)&31);
				ct[1]=j>> 8;
				ct[2]=j    ;
				ct+=j;
			}
		}

		ctx->bits_total+=ctx->bs_bits;
		sz=ct-dst;
	}else
	{
		BTIC1H_EncodeImageClrs(ctx->blks, 32, src,
			ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), qfl, clrs);

		BTIC1H_SetupContextInitial(ctx);
		for(j=0; j<256; j++)
			{ ctx->cmdwin[j]=j; ctx->cmdidx[j]=j; }

		BTIC1H_EncodeBlocksCtx(ctx,
			ctx->blks, (qfl&BTIC1H_QFL_PFRAME)?ctx->lblks:NULL,
			ctx->nblks, 32, &i, qfl);
		ctx->bits_total+=ctx->bs_bits;
		BTIC1H_Rice_FlushBits(ctx);
		sz=ctx->bs_ct-dst;
	}

	ctx->cnt_statframes++;
	ctx->cnt_totqfl+=qfl&127;

	if(csim==ctx->lcsim)
//	if(0)
	{
		dst[0]=0xE1;
		dst[1]=sz>>16;
		dst[2]=sz>>8;
		dst[3]=sz;
	}else
	{
		dst[0]=0xE3;
		dst[1]=sz>>16;
		dst[2]=sz>>8;
		dst[3]=sz;
		dst[4]='I';
		dst[5]='0'+csim;

//		if(qfl&BTIC1H_QFL_IFRAME)
//			{ ctx->lcsim=csim; }
//		else
//			{ ctx->lcsim=-1; }
	}

#if 0
	dst[0]=0xE1;
	dst[1]=sz>>16;
	dst[2]=sz>>8;
	dst[3]=sz;
#endif

#if 0
	dst[0]=0xE3;
	dst[1]=sz>>16;
	dst[2]=sz>>8;
	dst[3]=sz;
	dst[4]='I';
	dst[5]='1';
#endif
	
#ifdef BT1H_ENABLE_AX
	if((clrs==BTIC1H_PXF_RGBA) || (clrs==BTIC1H_PXF_BGRA))
	{
		ct=dst+sz;

		BTIC1H_Rice_SetupWrite(ctx, ct+6, cte-ct-6);
	
		BTIC1H_EncodeAlphaBlocksCtx(ctx,
			ctx->blks+24, (qfl&BTIC1H_QFL_PFRAME)?(ctx->lblks+24):NULL,
			ctx->nblks, 32, &i, qfl);
		ctx->bits_total+=ctx->bs_bits;
	
		BTIC1H_Rice_FlushBits(ctx);
		sz1=ctx->bs_ct-ct;
		dst[0]=0xE3;
		dst[1]=sz1>>16;
		dst[2]=sz1>>8;
		dst[3]=sz1;
		dst[4]='A';
		dst[5]='X';

		sz=ctx->bs_ct-dst;
	}
#endif

#ifdef BT1H_ENABLE_MX
	if(qfl&BTIC1H_QFL_USEMIP &&
		((clrs==BTIC1H_PXF_RGBA) ||
		 (clrs==BTIC1H_PXF_BGRA) ||
		 (clrs==BTIC1H_PXF_RGBX) ||
		 (clrs==BTIC1H_PXF_BGRX)))
	{
		if(!ctx->mtibuf)
			{ ctx->mtibuf=malloc(ctx->xs*ctx->ys*4); }

		ct=dst+sz;
		qfl1=qfl&(~BTIC1H_QFL_USEMIP);
		qfl1|=BTIC1H_QFL_ISMIPLVL;
		
		memcpy(ctx->mtibuf, src, xs*ys*4);
		BTIC1H_Tex_HalfSample(ctx->mtibuf, xs, ys, clrs);
		xs1=(xs+1)>>1; ys1=(ys+1)>>1; l=1;
		while((xs1>1)||(ys1>1))
		{
			ct=dst+sz;

			j=BTIC1H_EncodeCtx(ctx,
				ctx->mtibuf, ct+8, dsz-sz,
				xs1, ys1, qfl1, clrs);
			if(j<0)break;
//			if((sz+j+8)>dsz)break;

			j+=8;
			ct[0]=0xE4;		ct[1]=j>>16;
			ct[2]=j>>8;		ct[3]=j;
			ct[4]='M';		ct[5]='I';
			ct[6]='P';		ct[7]=(l<10)?('0'+l):('a'+l-10);
			sz+=j;
			
			BTIC1H_Tex_HalfSample(ctx->mtibuf, xs1, ys, clrs);
			xs1=(xs1+1)>>1; ys1=(ys1+1)>>1; l++;

			if(sz>dsz)break;
		}
	}
#endif

//	memcpy(ctx->lblks, ctx->blks, ctx->nblks*32);

	ct1=ctx->lblks;
	ctx->lblks=ctx->blks;
	ctx->blks=ct1;

	if(sz>dsz)
	{
		BT1H_TRAPCRASH
	}

	return(sz);
}

BTIC1H_Context btic1h_enc_tctx;

BTIC1H_API byte *BTIC1H_EncodeBlocksBuffer(byte *obuf,
	byte *blks, byte *lblks, int nblks, int stride,
	int obmsz, int *rnblks, int qf)
{
	BTIC1H_Context *ctx;
	int i;
	
	ctx=&btic1h_enc_tctx;
	memset(ctx, 0, sizeof(BTIC1H_Context));
	
	BTIC1H_SetupContextInitial(ctx);
	
	for(i=0; i<64; i++)
		ctx->cmdidx[i]=-1;
	
	BTIC1H_Rice_SetupWrite(ctx, obuf, obmsz);
	
	BTIC1H_EncodeBlocksCtx(ctx, blks, lblks, nblks, stride, rnblks, qf);
	
	BTIC1H_Rice_FlushBits(ctx);
	return(ctx->bs_ct);
}

void BTIC1H_ClearEncodeStats(BTIC1H_Context *ctx)
{
	int i;

	ctx->bits_total=0;
	ctx->cnt_statframes=0;
	ctx->cnt_totqfl=0;

	ctx->bs_bits=0;			ctx->cnt_cmds=0;
	ctx->bits_cmdidx=0;		ctx->bits_cmdabs=0;
	ctx->cnt_parms=0;		ctx->bits_parm=0;

	ctx->bits_dyuv=0;		ctx->bits_dy=0;
	ctx->bits_duv=0;		ctx->bits_ddy=0;
	ctx->bits_dumask=0;

	ctx->bits_pix4x4=0;		ctx->bits_pix4x4x1=0;
	ctx->bits_pix4x4x3=0;	ctx->bits_pix4x2=0;
	ctx->bits_pix2x2=0;		ctx->bits_pix2x1=0;
	
	for(i=0; i<4; i++)
		ctx->cnt_pred[i]=0;

	for(i=0; i<256; i++)
		ctx->stat_cmds[i]=0;
}

void BTIC1H_AccumEncodeStats(BTIC1H_Context *ctx, BTIC1H_Context *ctx1)
{
	int i;

	ctx->bits_total+=ctx1->bits_total;
	ctx->cnt_statframes+=ctx1->cnt_statframes;
	ctx->cnt_totqfl+=ctx1->cnt_totqfl;

	ctx->bs_bits+=ctx1->bs_bits;
	ctx->cnt_cmds+=ctx1->cnt_cmds;
	ctx->bits_cmdidx+=ctx1->bits_cmdidx;
	ctx->bits_cmdabs+=ctx1->bits_cmdabs;
	ctx->cnt_parms+=ctx1->cnt_parms;
	ctx->bits_parm+=ctx1->bits_parm;

	ctx->bits_dyuv+=ctx1->bits_dyuv;
	ctx->bits_dy+=ctx1->bits_dy;
	ctx->bits_duv+=ctx1->bits_duv;
	ctx->bits_ddy+=ctx1->bits_ddy;
	ctx->bits_dumask+=ctx1->bits_dumask;

	ctx->bits_pix4x4+=ctx1->bits_pix4x4;
	ctx->bits_pix4x4x1+=ctx1->bits_pix4x4x1;
	ctx->bits_pix4x4x3+=ctx1->bits_pix4x4x3;
	ctx->bits_pix4x2+=ctx1->bits_pix4x2;
	ctx->bits_pix2x2+=ctx1->bits_pix2x2;
	ctx->bits_pix2x1+=ctx1->bits_pix2x1;

	for(i=0; i<4; i++)
		ctx->cnt_pred[i]+=ctx1->cnt_pred[i];
	
	for(i=0; i<256; i++)
		ctx->stat_cmds[i]+=ctx1->stat_cmds[i];
}

void BTIC1H_DumpEncodeStats(BTIC1H_Context *ctx)
{
	double rfbits;
	int i, j, k;
	
	rfbits=100.0/(ctx->bits_total+1.0);
	
	printf("total_bits=%d, frames=%d, avg_bits_frame=%d avg_qfl=%d\n",
		ctx->bits_total, ctx->cnt_statframes,
		ctx->bits_total/(ctx->cnt_statframes+1),
		ctx->cnt_totqfl/(ctx->cnt_statframes+1));
	printf("ncmds=%d, bits=%d: cmdidx=%d cmdabs=%d, cmdavg=%f\n",
		ctx->cnt_cmds,
		ctx->bits_cmdidx+ctx->bits_cmdabs,
		ctx->bits_cmdidx, ctx->bits_cmdabs,
		(ctx->bits_cmdidx+ctx->bits_cmdabs)/(ctx->cnt_cmds+1.0));

	printf("nparms=%d, bits=%d parmavg=%f\n",
		ctx->cnt_parms,
		ctx->bits_parm,
		(ctx->bits_parm)/(ctx->cnt_parms+1.0));

	printf("dyuv=%d dy=%d duv=%d ddy=%d dumask=%d\n",
		ctx->bits_dyuv, ctx->bits_dy, ctx->bits_duv,
		ctx->bits_ddy, ctx->bits_dumask);
	printf("4x4=%d 4x4x1=%d 4x4x3=%d 4x2=%d 2x2=%d 2x1=%d\n",
		ctx->bits_pix4x4, ctx->bits_pix4x4x1,
		ctx->bits_pix4x4x3, ctx->bits_pix4x2,
		ctx->bits_pix2x2, ctx->bits_pix2x1);
	printf("\n");

	printf("ncmds=%d, bits=%2.02f%%: cmdidx=%2.02f%% cmdabs=%2.02f%%\n",
		ctx->cnt_cmds,
		(ctx->bits_cmdidx+ctx->bits_cmdabs)*rfbits,
		ctx->bits_cmdidx*rfbits,
		ctx->bits_cmdabs*rfbits);
	printf("nparms=%d, bits=%2.02f%%\n",
		ctx->cnt_parms, ctx->bits_parm*rfbits);
	printf(	"dyuvd=%2.02f%% dyuv=%2.02f%% "
			"dy=%2.02f%% duv=%2.02f%% ddy=%2.02f%% dumask=%2.02f%%\n",
		(ctx->bits_dyuv+ctx->bits_ddy)*rfbits,
		ctx->bits_dyuv*rfbits, ctx->bits_dy*rfbits,
		ctx->bits_duv*rfbits, ctx->bits_ddy*rfbits,
		ctx->bits_dumask*rfbits);
	printf("pixel_data=%2.02f%%\n",
		(ctx->bits_pix4x4+ctx->bits_pix4x4x1+
		ctx->bits_pix4x4x3+ctx->bits_pix4x2+
		ctx->bits_pix2x2+ctx->bits_pix2x1)*rfbits);
	printf(	"  4x4x2=%-2.02f%% 4x4x1=%-2.02f%% 4x4x3=%-2.02f%%\n"
			"  4x2x2=%-2.02f%% 2x2x2=%-2.02f%% 2x1x2=%-2.02f%%\n",
		ctx->bits_pix4x4*rfbits, ctx->bits_pix4x4x1*rfbits,
		ctx->bits_pix4x4x3*rfbits, ctx->bits_pix4x2*rfbits,
		ctx->bits_pix2x2*rfbits, ctx->bits_pix2x1*rfbits);
	
	printf("Rice K factors:\n");
	printf( "  cmdidx=%d cmdabs=%d cmdcnt=%d maskidx=%d\n",
		ctx->rk_cmdidx, ctx->rk_cmdabs, ctx->rk_cmdcnt,
		ctx->rk_maskidx);
	printf( "  parmxy=%d parmvar=%d parmval=%d parmix=%d\n",
		ctx->rk_parmxy, ctx->rk_parmvar,
		ctx->rk_parmval, ctx->rk_parmix);
	printf( "  dy=%d duv=%d ddy=%d dduv=%d\n",
		ctx->rk_dy, ctx->rk_duv, ctx->rk_ddy, ctx->rk_dduv);
	printf( "  ay=%d auv=%d ady=%d aduv=%d\n",
		ctx->rk_ay, ctx->rk_auv, ctx->rk_ady, ctx->rk_aduv);
	printf( "  qfy=%d qfuv=%d qfdy=%d qfduv=%d\n",
		ctx->rk_qfy, ctx->rk_qfuv, ctx->rk_qfdy, ctx->rk_qfduv);

	printf("\n");

	printf("Pred: ");
	for(i=0; i<4; i++)
	{
		printf("%d ", ctx->cnt_pred[i]);
	}
	printf("\n");

	printf("Command Stats:\n");
	printf("  ");
	j=0;
	for(i=0; i<256; i++)
	{
		if(!ctx->stat_cmds[i])
			continue;
		if(j>=4)
		{
			printf("\n  ");
			j=0;
		}
		printf("%02X:%7d,%.2f%% ",
			i, ctx->stat_cmds[i], 
			(100.0*ctx->stat_cmds[i])/(ctx->cnt_cmds+1.0));
		j++;
	}
	printf("\n");
}

void BTIC1H_DumpGeneralEncodeStats()
{
	BTIC1H_Context *ctx;
	int i;
	
	ctx=&btic1h_enc_tctx;
	BTIC1H_DumpEncodeStats(ctx);
}
