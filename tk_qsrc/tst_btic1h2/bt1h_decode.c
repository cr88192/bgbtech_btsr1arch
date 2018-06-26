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

#if 1

int btjpg_drv_defaultCodecDbfl;

int BTIC1H_ReadCommandCode(BTIC1H_Context *ctx)
{
	return(ctx->ReadCommandCode(ctx));
}

int BTIC1H_ReadCommandCodeBase(BTIC1H_Context *ctx)
{
	int i0, i1, i2, i3;
	int i, j, k, l;
	
//	i=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_cmdidx));
	i=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdidx));

//	if((i>0) && (i<=16))
//	if((i>=0) && (i<16))
	if((i>0) && (i<=256))
	{
		i--;

		if(!i)
		{
//			j=ctx->cmdwin[ctx->cmdwpos&15];
			j=ctx->cmdwin[ctx->cmdwpos&255];
#ifndef BTIC1H_VFWDRV
			if(j==0xFF)
				{ BT1H_TRAPCRASH }
#endif
			return(j);
		}

#if 0
		if(i>0)
		{
//			j=ctx->cmdwin[(ctx->cmdwpos+i+0)&15];
//			k=ctx->cmdwin[(ctx->cmdwpos+i-1)&15];
//			ctx->cmdwin[(ctx->cmdwpos+i-1)&15]=j;
//			ctx->cmdwin[(ctx->cmdwpos+i+0)&15]=k;

			i0=(ctx->cmdwpos+i+0)&255;
			i1=(ctx->cmdwpos+i-1)&255;

			j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
			ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
			ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;

			if(j==0xFF)
				{ BT1H_TRAPCRASH }

			return(j);
		}
#endif

#if 1
		if(i<32)
		{
//			j=ctx->cmdwin[(ctx->cmdwpos+i+0)&15];
//			k=ctx->cmdwin[(ctx->cmdwpos+i-1)&15];
//			ctx->cmdwin[(ctx->cmdwpos+i-1)&15]=j;
//			ctx->cmdwin[(ctx->cmdwpos+i+0)&15]=k;

			i0=(ctx->cmdwpos+i+0)&255;
			i1=(ctx->cmdwpos+i-1)&255;

			j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
			ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
			ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;

//			if(j==0xFF)
//				{ *(int *)-1=-1; }

			return(j);
		}else
		{
			i0=(ctx->cmdwpos+i)&255;
			i1=(ctx->cmdwpos-1)&255;
			ctx->cmdwpos--;

			j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
			ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
			ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;

//			if(j==0xFF)
//				{ *(int *)-1=-1; }

			return(j);
		}
#endif

#if 0
		if(i==1)
		{
			j=ctx->cmdwin[ctx->cmdwpos&15];
			k=ctx->cmdwin[(ctx->cmdwpos+1)&15];
			ctx->cmdwin[ctx->cmdwpos&15]=k;
			ctx->cmdwin[(ctx->cmdwpos+1)&15]=j;
			return(k);
		}

		if(i>0)
		{
			k=ctx->cmdwin[(ctx->cmdwpos+i)&15];
			j=(byte)(--ctx->cmdwpos);
			ctx->cmdwin[j&15]=k;
			return(k);
		}
#endif

		return(-1);
	}

//	if(i==(-1))
	if(i==0)
	{
		k=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdabs));
//		k=BTIC1H_Rice_Read8Bits(ctx);

//		k=(-i)-1;
//		k=BTIC1H_Rice_ReadNBits(ctx, -i);

		j=(--ctx->cmdwpos)&255;
//		ctx->cmdwin[j&15]=k;
		l=ctx->cmdwin[j];
		i0=ctx->cmdidx[k];
		ctx->cmdwin[j]=k;
		ctx->cmdidx[k]=j;
		if(i0!=j)
			ctx->cmdidx[l]=i0;
			
		return(k);	
	}
	
//	*(int *)-1=-1;
	return(-1);
}

int BTIC1H_ReadCommandCodeSMTF(BTIC1H_Context *ctx)
{
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	i=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdidx));
//	i=BTIC1H_Rice_ReadAdRiceDc(ctx, &(ctx->rk_cmdidx));

	if(!i)
	{
		j=ctx->cmdwin[ctx->cmdwpos&255];
		return(j);
	}

	if(i<32)
	{
		i0=(ctx->cmdwpos+i+0)&255;
		i1=(ctx->cmdwpos+i-1)&255;

		j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
		ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
//		ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;

		return(j);
	}else
	{
		i0=(ctx->cmdwpos+i)&255;
		i1=(ctx->cmdwpos-1)&255;
		ctx->cmdwpos--;

		j=ctx->cmdwin[i0];	k=ctx->cmdwin[i1];
		ctx->cmdwin[i1]=j;	ctx->cmdwin[i0]=k;
//		ctx->cmdidx[k]=i0;	ctx->cmdidx[j]=i1;

		return(j);
	}

	return(-1);
}
#endif

int BTIC1H_ReadMaskCode(BTIC1H_Context *ctx)
{
	int i, j, k;
	i=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_maskidx));
	if(i>0)
	{
		i--;
		if(i>0)
		{
			j=ctx->maskwin[i-1];
			k=ctx->maskwin[i  ];
			ctx->maskwin[i-1]=k;
			ctx->maskwin[i  ]=j;
		}else
		{
			k=ctx->maskwin[i  ];
		}
		return(k);
	}
	
//	k=ctx->updmask<<1;
	return(-1);
}

force_inline void BTIC1H_DecodeDeltaUpdateMask(BTIC1H_Context *ctx)
{
	int i;

	i=BTIC1H_ReadMaskCode(ctx);
	if(i>=0)
	{
		ctx->updmask=(i>>1)|128;
		if(i&1)
			{ ctx->nextupdmask=ctx->updmask; }
	}
}

force_inline void BTIC1H_DecodeAbsUpdateMask(BTIC1H_Context *ctx)
{
	int i;

	i=BTIC1H_ReadMaskCode(ctx);
	if(i>=0)
	{
		ctx->absupdmask=(i>>1)|128;
		if(i&1)
			{ ctx->nextabsupdmask=ctx->absupdmask; }
	}
}

force_inline void BTIC1H_DecodeUpdateMaskYUV(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeDeltaUpdateMask(ctx); }
force_inline void BTIC1H_DecodeUpdateMaskYUVD(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeDeltaUpdateMask(ctx); }
force_inline void BTIC1H_DecodeUpdateMaskYUVDyuv(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeDeltaUpdateMask(ctx); }

force_inline void BTIC1H_DecodeAbsUpdateMaskYUV(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeAbsUpdateMask(ctx); }
force_inline void BTIC1H_DecodeAbsUpdateMaskYUVD(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeAbsUpdateMask(ctx); }
force_inline void BTIC1H_DecodeAbsUpdateMaskYUVDyuv(BTIC1H_Context *ctx)
	{ BTIC1H_DecodeAbsUpdateMask(ctx); }

void BTIC1H_DecodeDeltaYUV(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;
	int py, pu, pv;
	
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
	}else
	{
		py=ctx->cy;
		pu=ctx->cu;
		pv=ctx->cv;
	}

	if(ctx->updmask)
	{
		BTIC1H_DecodeUpdateMaskYUV(ctx);
		
		if(ctx->updmask&1)
		{
			qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
			ctx->cy=py+qdy*ctx->qfy;
		}

		if(ctx->updmask&2)
		{
			qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
			ctx->cu=pu+qdu*ctx->qfuv;
		}

		if(ctx->updmask&4)
		{
			qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
			ctx->cv=pv+qdv*ctx->qfuv;
		}
		ctx->updmask=ctx->nextupdmask;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));

		ctx->cy=py+qdy*ctx->qfy;
		ctx->cu=pu+qdu*ctx->qfuv;
		ctx->cv=pv+qdv*ctx->qfuv;
	}
	
#ifndef BTIC1H_VFWDRV
#ifdef BT1H_DEBUG_TRAPRANGE
	if((ctx->cy<0) || (ctx->cy>255) ||
		(ctx->cu<0) || (ctx->cu>255) ||
		(ctx->cv<0) || (ctx->cv>255))
	{
		BT1H_TRAPCRASH
	}
#endif
#endif
}

void BTIC1H_DecodeDeltaYUVD(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;
	int dd, qdd;
	int py, pu, pv, pd;
	int cy, cu, cv, cd;

//	BTIC1H_DecodeDeltaYUV(ctx);

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
		{
			py=(3*ctx->cy-ctx->lcy)>>1;	pu=(3*ctx->cu-ctx->lcu)>>1;
			pv=(3*ctx->cv-ctx->lcv)>>1;	pd=(3*ctx->cd-ctx->lcd)>>1;
		}else if(ctx->colorpred==2)
		{
			py=2*ctx->cy-ctx->lcy;	pu=2*ctx->cu-ctx->lcu;
			pv=2*ctx->cv-ctx->lcv;	pd=2*ctx->cd-ctx->lcd;
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

//		ctx->lcy=ctx->cy;	ctx->lcu=ctx->cu;
//		ctx->lcv=ctx->cv;	ctx->lcd=ctx->cd;
	}else
	{
		py=ctx->cy;	pu=ctx->cu;
		pv=ctx->cv;	pd=ctx->cd;
	}

	if(ctx->updmask)
	{
		BTIC1H_DecodeUpdateMaskYUVD(ctx);
		
		if(ctx->updmask&1)
		{
			qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
//			ctx->cy=py+qdy*ctx->qfy;
			cy=py+qdy*ctx->qfy;
		}else
		{
			cy=ctx->cy;
		}

		if(ctx->updmask&2)
		{
			qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
//			ctx->cu=pu+qdu*ctx->qfuv;
			cu=pu+qdu*ctx->qfuv;
		}else
		{
			cu=ctx->cu;
		}

		if(ctx->updmask&4)
		{
			qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
//			ctx->cv=pv+qdv*ctx->qfuv;
			cv=pv+qdv*ctx->qfuv;
		}else
		{
			cv=ctx->cv;
		}

		if(ctx->updmask&8)
		{
			qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
//			ctx->cd=pd+qdd*ctx->qfd;
			cd=pd+qdd*ctx->qfd;
		}else
		{
			cd=ctx->cd;
		}

		ctx->updmask=ctx->nextupdmask;
		ctx->dyuv=0;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
		qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));

		cy=py+qdy*ctx->qfy;
		cu=pu+qdu*ctx->qfuv;
		cv=pv+qdv*ctx->qfuv;
		cd=pd+qdd*ctx->qfd;

//		ctx->cy=py+qdy*ctx->qfy;
//		ctx->cu=pu+qdu*ctx->qfuv;
//		ctx->cv=pv+qdv*ctx->qfuv;
//		ctx->cd=pd+qdd*ctx->qfd;
		ctx->dyuv=0;
	}

#ifndef BTIC1H_VFWDRV
#ifdef BT1H_DEBUG_TRAPRANGE
//	if(	((ctx->cy<0) || (ctx->cy>255)) ||
//		((ctx->cu<0) || (ctx->cu>255)) ||
//		((ctx->cv<0) || (ctx->cv>255)) ||
//		((ctx->cd<0) || (ctx->cd>255)))
	if(	((cy<0) || (cy>255)) ||
		((cu<0) || (cu>255)) ||
		((cv<0) || (cv>255)) ||
		((cd<0) || (cd>255)))
	{
		BT1H_TRAPCRASH
	}
#endif
#endif

	ctx->lcy=ctx->cy;	ctx->lcu=ctx->cu;
	ctx->lcv=ctx->cv;	ctx->lcd=ctx->cd;
	ctx->cy=cy;	ctx->cu=cu;
	ctx->cv=cv;	ctx->cd=cd;
}


void BTIC1H_DecodeDeltaY(BTIC1H_Context *ctx)
{
	int dy, qdy, py;

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
			{ py=(3*ctx->cy-ctx->lcy)>>1; }
		else if(ctx->colorpred==2)
			{ py=2*ctx->cy-ctx->lcy; }
		ctx->lcy=ctx->cy;
	}else
		{ py=ctx->cy; }

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
	ctx->cy=py+qdy*ctx->qfy;
}

void BTIC1H_DecodeDeltaUV(BTIC1H_Context *ctx)
{
	int du, dv;
	int qdu, qdv;
	int pu, pv;

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
			{ pu=(3*ctx->cu-ctx->lcu)>>1; pv=(3*ctx->cv-ctx->lcv)>>1; }
		else if(ctx->colorpred==2)
			{ pu=2*ctx->cu-ctx->lcu; pv=2*ctx->cv-ctx->lcv; }
		ctx->lcu=ctx->cu;
		ctx->lcv=ctx->cv;
	}else
		{ pu=ctx->cu; pv=ctx->cv; }

	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
	ctx->cu=pu+qdu*ctx->qfuv;
	ctx->cv=pv+qdv*ctx->qfuv;
}


void BTIC1H_DecodeDeltaYUVDyuv(BTIC1H_Context *ctx)
{
//	int dy, du, dv;
//	int qdy, qdu, qdv;

	int dy, du, dv;
	int qdy, qdu, qdv;
	int dd, qdd;
	int py, pu, pv;
	int pdy, pdu, pdv;

//	BTIC1H_DecodeDeltaYUV(ctx);

	if(ctx->colorpred)
	{
		if(ctx->colorpred==1)
		{
			py=(3*ctx->cy-ctx->lcy)>>1;		pu=(3*ctx->cu-ctx->lcu)>>1;
			pv=(3*ctx->cv-ctx->lcv)>>1;		pdy=(3*ctx->cdy-ctx->lcdy)>>1;
			pdu=(3*ctx->cdu-ctx->lcdu)>>1;	pdv=(3*ctx->cdv-ctx->lcdv)>>1;
		}else if(ctx->colorpred==2)
		{
			py=2*ctx->cy-ctx->lcy; pu=2*ctx->cu-ctx->lcu;
			pv=2*ctx->cv-ctx->lcv;	pdy=2*ctx->cdy-ctx->lcdy;
			pdu=2*ctx->cdu-ctx->lcdu;	pdv=2*ctx->cdv-ctx->lcdv;
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

		ctx->lcy=ctx->cy; ctx->lcu=ctx->cu; ctx->lcv=ctx->cv;
		ctx->lcdy=ctx->cdy; ctx->lcdu=ctx->cdu; ctx->lcdv=ctx->cdv;
	}else
	{
		py=ctx->cy; pu=ctx->cu; pv=ctx->cv;
		pdy=ctx->cdy; pdu=ctx->cdu; pdv=ctx->cdv;
	}

	if(ctx->updmask)
	{
		BTIC1H_DecodeUpdateMaskYUVDyuv(ctx);
		
		if(ctx->updmask&1)
		{	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
			ctx->cy=py+qdy*ctx->qfy;	}
		if(ctx->updmask&2)
		{	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
			ctx->cu=pu+qdu*ctx->qfuv;	}
		if(ctx->updmask&4)
		{	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
			ctx->cv=pv+qdv*ctx->qfuv;	}
		if(ctx->updmask&8)
		{	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
			ctx->cdy=pdy+qdy*ctx->qfdy;	}
		if(ctx->updmask&16)
		{	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));
			ctx->cdu=pdu+qdu*ctx->qfduv;	}
		if(ctx->updmask&32)
		{	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));
			ctx->cdv=pdv+qdv*ctx->qfduv;	}

		ctx->updmask=ctx->nextupdmask;
		ctx->dyuv=1;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_duv));
//		qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));

		ctx->cy=py+qdy*ctx->qfy;
		ctx->cu=pu+qdu*ctx->qfuv;
		ctx->cv=pv+qdv*ctx->qfuv;
//		ctx->cd+=qdd*ctx->qfd;

		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));

		ctx->cdy=pdy+qdy*ctx->qfdy;
		ctx->cdu=pdu+qdu*ctx->qfduv;
		ctx->cdv=pdv+qdv*ctx->qfduv;

		ctx->dyuv=1;
	}

#if 0
	BTIC1H_DecodeDeltaYUV(ctx);

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));
	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dduv));

	ctx->cdy+=qdy*ctx->qfdy;
	ctx->cdu+=qdu*ctx->qfduv;
	ctx->cdv+=qdv*ctx->qfduv;
	ctx->dyuv=1;
#endif

#ifdef BT1H_DEBUG_TRAPRANGE
#ifndef BTIC1H_VFWDRV
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
#endif
}

void BTIC1H_DecodeAbsYUV(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;

	if(ctx->absupdmask)
	{
		BTIC1H_DecodeAbsUpdateMaskYUV(ctx);
		
		if(ctx->absupdmask&1)
		{
			qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
			ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
		}

		if(ctx->absupdmask&2)
		{
			qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
		}

		if(ctx->absupdmask&4)
		{
			qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;
		}
		ctx->absupdmask=ctx->nextabsupdmask;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));

		ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
		ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
		ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;
	}

	ctx->lcy=ctx->cy;
	ctx->lcu=ctx->cu;
	ctx->lcv=ctx->cv;
}

void BTIC1H_DecodeAbsY(BTIC1H_Context *ctx)
{
	int dy, qdy;
	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
	ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
	ctx->lcy=ctx->cy;
}

void BTIC1H_DecodeAbsUV(BTIC1H_Context *ctx)
{
	int du, dv;
	int qdu, qdv;
	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
	ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
	ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;
	ctx->lcu=ctx->cu;	ctx->lcv=ctx->cv;
}

void BTIC1H_DecodeAbsYUVD(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;
	int dd, qdd;

	if(ctx->absupdmask)
	{
		BTIC1H_DecodeAbsUpdateMaskYUVD(ctx);
		
		if(ctx->absupdmask&1)
		{
			qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
			ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
		}

		if(ctx->absupdmask&2)
		{
			qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
		}

		if(ctx->absupdmask&4)
		{
			qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;
		}

		if(ctx->absupdmask&8)
		{
			qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ady));
			ctx->cd=qdd*ctx->qfad;
		}

		ctx->absupdmask=ctx->nextabsupdmask;
		ctx->dyuv=0;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));

		ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
		ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
		ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;

		qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ady));
		ctx->cd=qdd*ctx->qfad;
		ctx->dyuv=0;
	}

	ctx->lcy=ctx->cy;	ctx->lcu=ctx->cu;
	ctx->lcv=ctx->cv;	ctx->lcd=ctx->cd;
}

void BTIC1H_DecodeAbsYUVDyuv(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;

	if(ctx->absupdmask)
	{
		BTIC1H_DecodeUpdateMaskYUVDyuv(ctx);
		
		if(ctx->absupdmask&1)
		{	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
			ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;		}
		if(ctx->absupdmask&2)
		{	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;		}
		if(ctx->absupdmask&4)
		{	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
			ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;		}

		if(ctx->absupdmask&8)
		{	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ady));
			ctx->cdy=qdy*ctx->qfady;	}
		if(ctx->absupdmask&16)
		{	qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_aduv));
			ctx->cdu=qdu*ctx->qfaduv;	}
		if(ctx->absupdmask&32)
		{	qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_aduv));
			ctx->cdv=qdv*ctx->qfaduv;	}

		ctx->absupdmask=ctx->nextabsupdmask;
		ctx->dyuv=0;
	}else
	{
		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_auv));

		ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
		ctx->cu=ctx->absyuvbias+qdu*ctx->qfauv;
		ctx->cv=ctx->absyuvbias+qdv*ctx->qfauv;

		qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ady));
		qdu=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_aduv));
		qdv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_aduv));

		ctx->cdy=qdy*ctx->qfady;
		ctx->cdu=qdu*ctx->qfaduv;
		ctx->cdv=qdv*ctx->qfaduv;
		ctx->dyuv=0;
	}

	ctx->lcy=ctx->cy;	ctx->lcu=ctx->cu;
	ctx->lcv=ctx->cv;	ctx->lcdy=ctx->cdy;
	ctx->lcdu=ctx->cdu;	ctx->lcdv=ctx->cdv;
}

void BTIC1H_DecodeQfDeltaYUVD(BTIC1H_Context *ctx)
{
	int qdy, qduv, qdd;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfuv));
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));

	ctx->qfy+=qdy;
	ctx->qfuv+=qduv;
	ctx->qfd+=qdd;
}

void BTIC1H_DecodeQfDeltaYUVDyuv(BTIC1H_Context *ctx)
{
	int qdy, qduv, qddy, qdduv;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfuv));
	qddy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));
	qdduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfduv));

	ctx->qfy+=qdy;
	ctx->qfuv+=qduv;
	ctx->qfdy+=qddy;
	ctx->qfduv+=qdduv;
}

void BTIC1H_DecodeQfAbsYUVD(BTIC1H_Context *ctx)
{
	int qdy, qduv, qdd;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfuv));
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));

	ctx->qfay+=qdy;
	ctx->qfauv+=qduv;
	ctx->qfad+=qdd;
}

void BTIC1H_DecodeQfAbsYUVDyuv(BTIC1H_Context *ctx)
{
	int qdy, qduv, qddy, qdduv;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfuv));
	qddy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));
	qdduv=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfduv));

	ctx->qfay+=qdy;
	ctx->qfauv+=qduv;
	ctx->qfady+=qddy;
	ctx->qfaduv+=qdduv;
}

int BTIC1H_ReadPix8_2x2(BTIC1H_Context *ctx)
{
	int i;
	i=BTIC1H_Rice_Read8Bits(ctx);
	return(i);
}

int BTIC1H_ReadPix8_2x1(BTIC1H_Context *ctx)
{
	int i;
	i=BTIC1H_Rice_Read4Bits(ctx);
	i=i|(i<<4);
	return(i);
}

int BTIC1H_ReadPix8_1x2(BTIC1H_Context *ctx)
{
	int i;
	i=BTIC1H_Rice_Read4Bits(ctx);
//	i=i|(i<<4);
	i=(i&3)|((i&3)<<2)|((i&12)<<2)|((i&12)<<4);
	return(i);
}

int BTIC1H_ReadPix16_4x4x1(BTIC1H_Context *ctx)
{
	int i;
	i=BTIC1H_Rice_Read16Bits(ctx);
	return(i);
}

int BTIC1H_ReadPix16_2x2x1(BTIC1H_Context *ctx)
{
	static const int tab[16]={
		0x0000, 0x0033, 0x00CC, 0x00FF,
		0x3300, 0x3333, 0x33CC, 0x33FF,
		0xCC00, 0xCC33, 0xCCCC, 0xCCFF,
		0xFF00, 0xFF33, 0xFFCC, 0xFFFF};
	int i, j;
	i=BTIC1H_Rice_Read4Bits(ctx);
	j=tab[i];
	return(j);
}

u32 BTIC1H_ReadPix32_4x4(BTIC1H_Context *ctx)
{
	u32 i;
	i=BTIC1H_Rice_Read32Bits(ctx);
	return(i);
}

u32 BTIC1H_ReadPix32_4x2(BTIC1H_Context *ctx)
{
	int i, j, k;
	i=BTIC1H_Rice_Read8Bits(ctx);
	j=BTIC1H_Rice_Read8Bits(ctx);
	k=(i<<24)|(i<<16)|(j<<8)|j;
	return(k);
}

u32 BTIC1H_ReadPix32_2x4(BTIC1H_Context *ctx)
{
	static const byte remap[16]={
		0x00, 0x05, 0x0A, 0x0F,
		0x50, 0x55, 0x5A, 0x5F,
		0xA0, 0xA5, 0xAA, 0xAF,
		0xF0, 0xF5, 0xFA, 0xFF};
	int i0, i1, i2, i3;
	int i, j, k;

	i=BTIC1H_Rice_Read16Bits(ctx);
	i0=remap[(i>>12)&15];
	i1=remap[(i>> 8)&15];
	i2=remap[(i>> 4)&15];
	i3=remap[(i    )&15];
	
	k=(i0<<24)|(i1<<16)|(i2<<8)|i3;
	return(k);
}

u32 BTIC1H_ReadPix24_2x4x3(BTIC1H_Context *ctx)
{
	u32 i;
	i=BTIC1H_Rice_Read24Bits(ctx);
	return(i);
}

u64 BTIC1H_ReadPix48_4x4x3(BTIC1H_Context *ctx)
{
	u64 i;
	i=BTIC1H_Rice_Read48Bits(ctx);
	return(i);
}

static const int btic1h_dec_hilbert4x4[16]=
	{ 0, 1, 5, 4, 8, 12, 13, 9, 10, 14, 15, 11, 7, 6, 2, 3 };
static const int btic1h_dec_hilbert2x2[4]=
	{ 0, 2, 3, 1 };

void BTIC1H_ReadPixDeltaY4x4(BTIC1H_Context *ctx, byte *yblk)
{
	int i;

	for(i=0; i<16; i++)
	{
		BTIC1H_DecodeDeltaY(ctx);
		yblk[btic1h_dec_hilbert4x4[i]]=ctx->cy;
	}
}

void BTIC1H_ReadPixDeltaUV2x2(BTIC1H_Context *ctx, byte *ublk, byte *vblk)
{
	int i;

	for(i=0; i<4; i++)
	{
		BTIC1H_DecodeDeltaUV(ctx);
		ublk[btic1h_dec_hilbert2x2[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert2x2[i]]=ctx->cv;
	}
}

void BTIC1H_ReadPixDeltaYUV4x4(BTIC1H_Context *ctx,
	byte *yblk, byte *ublk, byte *vblk)
{
	int i;

	for(i=0; i<16; i++)
	{
		BTIC1H_DecodeDeltaYUV(ctx);
		yblk[btic1h_dec_hilbert4x4[i]]=ctx->cy;
		ublk[btic1h_dec_hilbert4x4[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert4x4[i]]=ctx->cv;
	}
}

void BTIC1H_ReadPixDeltaYUV2x2(BTIC1H_Context *ctx,
	byte *yblk, byte *ublk, byte *vblk)
{
	int i;

	for(i=0; i<4; i++)
	{
		BTIC1H_DecodeDeltaYUV(ctx);
		yblk[btic1h_dec_hilbert2x2[i]]=ctx->cy;
		ublk[btic1h_dec_hilbert2x2[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert2x2[i]]=ctx->cv;
	}
}

void BTIC1H_ReadPixDeltaYUVD2x2(BTIC1H_Context *ctx,
	byte *yblk, byte *ublk, byte *vblk, byte *dblk)
{
	int i;

	for(i=0; i<4; i++)
	{
		BTIC1H_DecodeDeltaYUVD(ctx);
		yblk[btic1h_dec_hilbert2x2[i]]=ctx->cy;
		ublk[btic1h_dec_hilbert2x2[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert2x2[i]]=ctx->cv;
		dblk[btic1h_dec_hilbert2x2[i]]=ctx->cd;
	}
}


void BTIC1H_ReadPixAbsY4x4(BTIC1H_Context *ctx, byte *yblk)
{
	int i;

	for(i=0; i<16; i++)
	{
		BTIC1H_DecodeAbsY(ctx);
		yblk[btic1h_dec_hilbert4x4[i]]=ctx->cy;
	}
}

void BTIC1H_ReadPixAbsUV2x2(BTIC1H_Context *ctx, byte *ublk, byte *vblk)
{
	int i;

	for(i=0; i<4; i++)
	{
		BTIC1H_DecodeAbsUV(ctx);
		ublk[btic1h_dec_hilbert2x2[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert2x2[i]]=ctx->cv;
	}
}

void BTIC1H_ReadPixAbsYUV4x4(BTIC1H_Context *ctx,
	byte *yblk, byte *ublk, byte *vblk)
{
	int i;

	for(i=0; i<16; i++)
	{
		BTIC1H_DecodeAbsYUV(ctx);
		yblk[btic1h_dec_hilbert4x4[i]]=ctx->cy;
		ublk[btic1h_dec_hilbert4x4[i]]=ctx->cu;
		vblk[btic1h_dec_hilbert4x4[i]]=ctx->cv;
	}
}


byte *BTIC1H_FillBlockFlat(BTIC1H_Context *ctx, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=0;
	ct[4]=0;			ct[5]=0;
	ct[6]=0;			ct[7]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock2x2(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=0;
	ct[4]=1;			ct[5]=ctx->cd;
	ct[6]=px;			ct[7]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=ctx->cd;
	ct[4]=(px>>24);		ct[5]=(px>>16);
	ct[6]=(px>> 8);		ct[7]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x1(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=0;
	ct[4]=2;			ct[5]=ctx->cd;
	ct[6]=(px>>8);		ct[7]=px;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x3(BTIC1H_Context *ctx,
	u64 px, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=6;			ct[ 5]=ctx->cd;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=0;			ct[ 9]=0;
	ct[10]=(px>>40);	ct[11]=(px>>32);
	ct[12]=(px>>24);	ct[13]=(px>>16);
	ct[14]=(px>> 8);	ct[15]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock2x2B(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=0;
	ct[4]=19;			ct[5]=(ctx->cdy>>1)+128;
	ct[6]=px;			ct[7]=0;
	ct[8]=(ctx->cdu>>1)+128;
	ct[9]=(ctx->cdv>>1)+128;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4B(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=ctx->cu;
	ct[2]=ctx->cv;		ct[3]=0;
	ct[4]=14;			ct[5]=(ctx->cdy>>1)+128;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;

	ct[12]=(px>>24);	ct[13]=(px>>16);
	ct[14]=(px>> 8);	ct[15]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x3B(BTIC1H_Context *ctx,
	u64 px, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=15;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=(px>>40);	ct[11]=(px>>32);
	ct[12]=(px>>24);	ct[13]=(px>>16);
	ct[14]=(px>> 8);	ct[15]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4B420(BTIC1H_Context *ctx,
	byte *ypix, byte *upix, byte *vpix, byte *dpix,
	int px, byte *ct, int stride)
{
	ct[ 0]=ypix[0];		ct[ 1]=upix[0];
	ct[ 2]=vpix[0];		ct[ 3]=0;
	ct[ 4]=17;			ct[ 5]=dpix[0];
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=ypix[1];		ct[ 9]=upix[1];
	ct[10]=vpix[1];		ct[11]=dpix[1];
	ct[12]=(px>>24);	ct[13]=(px>>16);
	ct[14]=(px>> 8);	ct[15]=(px    );
	ct[16]=ypix[2];		ct[17]=upix[2];
	ct[18]=vpix[2];		ct[19]=dpix[2];
	ct[20]=ypix[3];		ct[21]=upix[3];
	ct[22]=vpix[3];		ct[23]=dpix[3];

	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4YUV420(BTIC1H_Context *ctx,
	byte *ypix, byte *upix, byte *vpix,
	byte *ct, int stride)
{
	if(stride>=32)
	{
		ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
		ct[ 2]=ctx->cv;		ct[ 3]=0;
		ct[ 4]=7;			ct[ 5]=ctx->cd;
		ct[ 6]=0;			ct[ 7]=0;
		ct[ 8]=upix[ 0];	ct[ 9]=upix[ 1];
		ct[10]=upix[ 2];	ct[11]=upix[ 3];
		ct[12]=vpix[ 0];	ct[13]=vpix[ 1];
		ct[14]=vpix[ 2];	ct[15]=vpix[ 3];
		ct[16]=ypix[ 0];	ct[17]=ypix[ 1];
		ct[18]=ypix[ 2];	ct[19]=ypix[ 3];
		ct[20]=ypix[ 4];	ct[21]=ypix[ 5];
		ct[22]=ypix[ 6];	ct[23]=ypix[ 7];
		ct[24]=ypix[ 8];	ct[25]=ypix[ 9];
		ct[26]=ypix[10];	ct[27]=ypix[11];
		ct[28]=ypix[12];	ct[29]=ypix[13];
		ct[30]=ypix[14];	ct[31]=ypix[15];

		ct+=stride;
		return(ct);
	}

	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=2;			ct[ 5]=ctx->cdy;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=0;			ct[ 9]=0;
//	ct[10]=(px>>40);	ct[11]=(px>>32);
//	ct[12]=(px>>24);	ct[13]=(px>>16);
//	ct[14]=(px>> 8);	ct[15]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4YUV444(BTIC1H_Context *ctx,
	byte *ypix, byte *upix, byte *vpix,
	byte *ct, int stride)
{
	byte upix2[4], vpix2[4];
	
	upix2[0]=(upix[ 0]+upix[ 1]+upix[ 4]+upix[ 5])>>2;
	upix2[1]=(upix[ 2]+upix[ 3]+upix[ 6]+upix[ 7])>>2;
	upix2[2]=(upix[ 8]+upix[ 9]+upix[12]+upix[13])>>2;
	upix2[3]=(upix[10]+upix[11]+upix[14]+upix[15])>>2;
	vpix2[0]=(vpix[ 0]+vpix[ 1]+vpix[ 4]+vpix[ 5])>>2;
	vpix2[1]=(vpix[ 2]+vpix[ 3]+vpix[ 6]+vpix[ 7])>>2;
	vpix2[2]=(vpix[ 8]+vpix[ 9]+vpix[12]+vpix[13])>>2;
	vpix2[3]=(vpix[10]+vpix[11]+vpix[14]+vpix[15])>>2;
	
	ct=BTIC1H_FillBlock4x4YUV420(ctx, ypix, upix2, vpix2, ct, stride);
	return(ct);
}

byte *BTIC1H_FillBlock4x4B_UV2x2(BTIC1H_Context *ctx,
	int px, int pxu, int pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=20;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=pxu;			ct[11]=pxv;
	ct[12]=(px>>24);	ct[13]=(px>>16);
	ct[14]=(px>> 8);	ct[15]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x3B_UV4x4(BTIC1H_Context *ctx,
	u64 pxy, u32 pxu, u32 pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=21;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=(pxy>>40);	ct[11]=(pxy>>32);
	ct[12]=(pxy>>24);	ct[13]=(pxy>>16);
	ct[14]=(pxy>> 8);	ct[15]=(pxy    );
	ct[16]=(pxu>>24);	ct[17]=(pxu>>16);
	ct[18]=(pxu>> 8);	ct[19]=(pxu    );
	ct[20]=(pxv>>24);	ct[21]=(pxv>>16);
	ct[22]=(pxv>> 8);	ct[23]=(pxv    );

	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x3B_UV2x2(BTIC1H_Context *ctx,
	u64 pxy, int pxu, int pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=22;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=pxu;			ct[ 7]=pxv;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=(pxy>>40);	ct[11]=(pxy>>32);
	ct[12]=(pxy>>24);	ct[13]=(pxy>>16);
	ct[14]=(pxy>> 8);	ct[15]=(pxy    );

	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock2x2B_UV2x2(BTIC1H_Context *ctx,
	int px, int pxu, int pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=23;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=px;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=pxu;			ct[11]=pxv;
	ct[12]=0;			ct[13]=0;
	ct[14]=0;			ct[15]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4x3B_UV2x4x3(BTIC1H_Context *ctx,
	u64 pxy, u32 pxu, u32 pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=26;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
	ct[10]=(pxy>>40);	ct[11]=(pxy>>32);
	ct[12]=(pxy>>24);	ct[13]=(pxy>>16);
	ct[14]=(pxy>> 8);	ct[15]=(pxy    );
	ct[16]=(pxu>>24);	ct[17]=(pxu>>16);
	ct[18]=(pxu>> 8);	ct[19]=(pxu    );
	ct[20]=(pxv>>24);	ct[21]=(pxv>>16);
	ct[22]=(pxv>> 8);	ct[23]=(pxv    );

	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillBlock4x4B_UV4x4(BTIC1H_Context *ctx,
	u32 pxy, u32 pxu, u32 pxv, byte *ct, int stride)
{
	ct[ 0]=ctx->cy;		ct[ 1]=ctx->cu;
	ct[ 2]=ctx->cv;		ct[ 3]=0;
	ct[ 4]=27;			ct[ 5]=(ctx->cdy>>1)+128;
	ct[ 6]=0;			ct[ 7]=0;
	ct[ 8]=(ctx->cdu>>1)+128;
	ct[ 9]=(ctx->cdv>>1)+128;
//	ct[10]=pxu;			ct[11]=pxv;
	ct[12]=(pxy>>24);	ct[13]=(pxy>>16);
	ct[14]=(pxy>> 8);	ct[15]=(pxy    );
	ct[16]=(pxu>>24);	ct[17]=(pxu>>16);
	ct[18]=(pxu>> 8);	ct[19]=(pxu    );
	ct[20]=(pxv>>24);	ct[21]=(pxv>>16);
	ct[22]=(pxv>> 8);	ct[23]=(pxv    );
	ct+=stride;
	return(ct);
}

u64 BTIC1H_FillBlockGr2x2_Repack(int px)
{
	int p0, p1, p2, p3;
	int qa0, qa1, qa2, qa3;
	int qb0, qb1, qb2, qb3;
	int qc0, qc1, qc2, qc3;
	int qd0, qd1, qd2, qd3;
	int pxr0, pxr1, pxr2, pxr3;
	u64 px1;

	p0=(px>>6)&3;			p1=(px>>4)&3;
	p2=(px>>2)&3;			p3=(px   )&3;
	p0=(p0<<1)|(p0>>1);		p1=(p1<<1)|(p1>>1);
	p2=(p2<<1)|(p2>>1);		p3=(p3<<1)|(p3>>1);

	qa0=p0; qa3=p1; qa1=(11*qa0+5*qa3+8)>>4; qa2=(5*qa0+11*qa3+8)>>4;
	qd0=p0; qd3=p1; qd1=(11*qd0+5*qd3+8)>>4; qd2=(5*qd0+11*qd3+8)>>4;
	qb0=(11*qa0+ 5*qd0+8)>>4;	qb1=(11*qa1+ 5*qd1+8)>>4;
	qb2=(11*qa2+ 5*qd2+8)>>4;	qb3=(11*qa3+ 5*qd3+8)>>4;
	qc0=( 5*qa0+11*qd0+8)>>4;	qc1=( 5*qa1+11*qd1+8)>>4;
	qc2=( 5*qa2+11*qd2+8)>>4;	qc3=( 5*qa3+11*qd3+8)>>4;

	pxr0=(qa0<<9)|(qa1<<6)|(qa2<<3)|qa3;
	pxr1=(qb0<<9)|(qb1<<6)|(qb2<<3)|qb3;
	pxr2=(qc0<<9)|(qc1<<6)|(qc2<<3)|qc3;
	pxr3=(qd0<<9)|(qd1<<6)|(qd2<<3)|qd3;

	px1=(((u64)pxr0)<<36)|(((u64)pxr1)<<24)|(pxr2<<12)|pxr3;
	
	return(px1);
}

byte *BTIC1H_FillBlockGr2x2(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	u64 px1;
	px1=BTIC1H_FillBlockGr2x2_Repack(px);
	ct=BTIC1H_FillBlock4x4x3(ctx, px1, ct, stride);
	return(ct);
}

byte *BTIC1H_FillBlockGr2x2B(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	u64 px1;
	px1=BTIC1H_FillBlockGr2x2_Repack(px);
	ct=BTIC1H_FillBlock4x4x3B(ctx, px1, ct, stride);
	return(ct);
}

byte *BTIC1H_FillBlock4x4AB(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	if(ctx->dyuv)
		{ return(BTIC1H_FillBlock4x4B(ctx, px, ct, stride)); }
	return(BTIC1H_FillBlock4x4(ctx, px, ct, stride));
}

byte *BTIC1H_FillBlock2x2AB(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	if(ctx->dyuv)
		{ return(BTIC1H_FillBlock2x2B(ctx, px, ct, stride)); }
	return(BTIC1H_FillBlock2x2(ctx, px, ct, stride));
}

int BTIC1H_DecodeCopyBlocks(BTIC1H_Context *ctx,
	byte *dblk, byte *sblk, int nblks, int stride)
{
	byte *ct, *cs, *cse;
	if(stride==32)
	{
#ifdef BT1H_ENABLE_AX
		if(!ctx->hasax)
		{
//			memcpy(dblk, sblk, nblks*stride);
//			return(0);

			ct=dblk; cs=sblk; cse=sblk+nblks*32;
			while(cs<cse)
			{
//				memcpy(ct, cs, 32);
				((u64 *)ct)[0]=((u64 *)cs)[0];
				((u64 *)ct)[1]=((u64 *)cs)[1];
				((u64 *)ct)[2]=((u64 *)cs)[2];
				((u64 *)ct)[3]=((u64 *)cs)[3];
				ct+=32; cs+=32;
			}
			return(0);
		}
#endif

		ct=dblk; cs=sblk; cse=sblk+nblks*32;
		while(cs<cse)
		{
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			((u64 *)ct)[2]=((u64 *)cs)[2];

//			((u32 *)ct)[0]=((u32 *)cs)[0];
//			((u32 *)ct)[1]=((u32 *)cs)[1];
//			((u32 *)ct)[2]=((u32 *)cs)[2];
//			((u32 *)ct)[3]=((u32 *)cs)[3];
//			((u32 *)ct)[4]=((u32 *)cs)[4];
//			((u32 *)ct)[5]=((u32 *)cs)[5];
#ifdef BT1H_ENABLE_AX
			if((cs[3]==0) && (cs[4]==7))
			{	((u32 *)ct)[6]=((u32 *)cs)[6];
				((u32 *)ct)[7]=((u32 *)cs)[7];	}
#else
			((u64 *)ct)[3]=((u64 *)cs)[3];
//			((u32 *)ct)[6]=((u32 *)cs)[6];
//			((u32 *)ct)[7]=((u32 *)cs)[7];
#endif
			ct+=32; cs+=32;
		}
	}else
	{
		memcpy(dblk, sblk, nblks*stride);
	}
	return(0);
}

#ifdef BT1H_ENABLE_AX
void BTIC1H_DecodeDeltaA(BTIC1H_Context *ctx)
{
	int dy, du, dv;
	int qdy, qdu, qdv;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_dy));
	ctx->cy+=qdy*ctx->qfy;
}

void BTIC1H_DecodeDeltaAD(BTIC1H_Context *ctx)
{
	int dd, qdd;

	BTIC1H_DecodeDeltaA(ctx);
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
	ctx->cd+=qdd*ctx->qfd;
}

void BTIC1H_DecodeAbsA(BTIC1H_Context *ctx)
{
	int dy, qdy;
	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ay));
	ctx->cy=ctx->absyuvbias+qdy*ctx->qfay;
}

void BTIC1H_DecodeAbsAD(BTIC1H_Context *ctx)
{
	int dd, qdd;

	BTIC1H_DecodeAbsA(ctx);
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_ddy));
	ctx->cd=qdd*ctx->qfd;
}

void BTIC1H_DecodeQfDeltaAD(BTIC1H_Context *ctx)
{
	int qdy, qduv, qdd;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));

	ctx->qfy+=qdy;
	ctx->qfd+=qdd;
}

void BTIC1H_DecodeQfAbsAD(BTIC1H_Context *ctx)
{
	int qdy, qduv, qdd;

	qdy=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfy));
	qdd=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_qfdy));

	ctx->qfay+=qdy;
	ctx->qfad+=qdd;
}

byte *BTIC1H_FillAlphaBlockNoAlpha(BTIC1H_Context *ctx, byte *ct, int stride)
{
	byte *ct1;
	if(stride==32)
	{
		ct1=ct-24;
		if((ct1[3]==0) && (ct1[4]==7))
		{
			ct+=stride;
			return(ct);
		}
	}

	ct[0]=ctx->cy;		ct[1]=0;
	ct[2]=0;			ct[3]=0;
	ct[4]=0;			ct[5]=0;
	ct[6]=0;			ct[7]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillAlphaBlockFlat(BTIC1H_Context *ctx, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=0;
	ct[2]=0;			ct[3]=7;
	ct[4]=0;			ct[5]=0;
	ct[6]=0;			ct[7]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillAlphaBlock2x2(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=0;
	ct[2]=ctx->cd;		ct[3]=1;
	ct[4]=0;			ct[5]=0;
	ct[6]=px;			ct[7]=0;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillAlphaBlock4x4(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=0;
	ct[2]=ctx->cd;		ct[3]=7;
	ct[4]=(px>>24);		ct[5]=(px>>16);
	ct[6]=(px>> 8);		ct[7]=(px    );
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillAlphaBlock4x4x1(BTIC1H_Context *ctx,
	int px, byte *ct, int stride)
{
	ct[0]=ctx->cy;		ct[1]=0;
	ct[2]=ctx->cd;		ct[3]=2;
	ct[4]=0;			ct[5]=0;
	ct[6]=(px>>8);		ct[7]=px;
	ct+=stride;
	return(ct);
}

byte *BTIC1H_FillAlphaBlock4x4x3(BTIC1H_Context *ctx,
	u64 px, byte *ct, int stride)
{
	ct[0]=ctx->cy;	ct[1]=ctx->cd;
	ct[2]=(px>>40);	ct[3]=(px>>32);
	ct[4]=(px>>24);	ct[5]=(px>>16);
	ct[6]=(px>> 8);	ct[7]=(px    );
	ct+=stride;
	return(ct);
}

int BTIC1H_DecodeCopyAlphaBlocks(
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
	}
	return(0);
}
#endif

int BTIC1H_DecodeBlocksCtx(BTIC1H_Context *ctx,
	byte *blks, byte *lblks, int nblks, int stride,
	int *rnblks)
{
	byte ypix[16], upix[16], vpix[16], dpix[16];
	byte *ct, *cte, *csl, *csle, *lblks2, *csle2, *cfl, *blksfl;
	u64 lpx;
	int cmd, ret, blkshr;
	int n, xo, yo;
	int i, j, k, l;

	ct=blks; cte=ct+nblks*stride;
	csl=lblks; csle=csl+nblks*stride;
	
//	blksfl=ctx->blksfl;

	i=stride; j=0;
	while(i>1)
		{ i=i>>1; j++; }
	blkshr=j;

	blksfl=ctx->blksfl+((blks-ctx->blks)>>blkshr);
	
	if(lblks && ctx->lblks && (lblks>=ctx->lblks) &&
		(lblks<(ctx->lblks+(ctx->nblks*stride))))
	{
		lblks2=ctx->lblks;
		csle2=ctx->lblks+(ctx->nblks*stride);
		blksfl=ctx->blksfl+((lblks-ctx->lblks)>>blkshr);
	}else
	{
		lblks2=lblks;
		csle2=csle;
		blksfl=NULL;
	}

	if(blksfl)
		memset(blksfl, 0, nblks);
//	memset(ctx->blksfl, 0, ctx->nblks);

	ret=0;
	while((ct<cte) && !ret)
	{
		cmd=BTIC1H_ReadCommandCode(ctx);
		switch(cmd)
		{
		case 0x00:
			BTIC1H_DecodeDeltaYUV(ctx);
			ct=BTIC1H_FillBlockFlat(ctx, ct, stride);
			break;
		case 0x01:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock2x2(ctx, i, ct, stride);
			break;
		case 0x02:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix8_2x1(ctx);
			ct=BTIC1H_FillBlock2x2(ctx, i, ct, stride);
			break;
		case 0x03:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix8_1x2(ctx);
			ct=BTIC1H_FillBlock2x2(ctx, i, ct, stride);
			break;
		case 0x04:
			BTIC1H_DecodeQfDeltaYUVD(ctx);
			break;
		case 0x05:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4(ctx, i, ct, stride);
			break;
		case 0x06:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix32_4x2(ctx);
			ct=BTIC1H_FillBlock4x4(ctx, i, ct, stride);
			break;
		case 0x07:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix32_2x4(ctx);
			ct=BTIC1H_FillBlock4x4(ctx, i, ct, stride);
			break;
		case 0x08:
			BTIC1H_DecodeQfDeltaYUVDyuv(ctx);
			break;
		case 0x09:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4B(ctx, i, ct, stride);
			break;
		case 0x0A:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			i=BTIC1H_ReadPix32_4x2(ctx);
			ct=BTIC1H_FillBlock4x4B(ctx, i, ct, stride);
			break;
		case 0x0B:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			i=BTIC1H_ReadPix32_2x4(ctx);
			ct=BTIC1H_FillBlock4x4B(ctx, i, ct, stride);
			break;
		case 0x0C:
			BTIC1H_DecodeAbsYUVD(ctx);
			i=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock2x2(ctx, i, ct, stride);
			break;
		case 0x0D:
			BTIC1H_DecodeAbsYUVD(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4(ctx, i, ct, stride);
			break;
		case 0x0E:
			BTIC1H_DecodeAbsYUVDyuv(ctx);
			i=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock2x2B(ctx, i, ct, stride);
			break;
		case 0x0F:
			BTIC1H_DecodeAbsYUVDyuv(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4B(ctx, i, ct, stride);
			ct+=stride;
			break;
		case 0x10:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
				{ ct=BTIC1H_FillBlockFlat(ctx, ct, stride); }
			break;
		case 0x11:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlock2x2AB(ctx, j, ct, stride);
			}
			break;
		case 0x12:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix8_2x1(ctx);
				ct=BTIC1H_FillBlock2x2AB(ctx, j, ct, stride);
			}
			break;
		case 0x13:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix8_1x2(ctx);
				ct=BTIC1H_FillBlock2x2AB(ctx, j, ct, stride);
			}
			break;
		case 0x14:
			BTIC1H_DecodeQfAbsYUVD(ctx);
			break;
		case 0x15:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_4x4(ctx);
				ct=BTIC1H_FillBlock4x4AB(ctx, j, ct, stride);
			}
			break;
		case 0x16:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_4x2(ctx);
				ct=BTIC1H_FillBlock4x4AB(ctx, j, ct, stride);
			}
			break;
		case 0x17:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_2x4(ctx);
				ct=BTIC1H_FillBlock4x4AB(ctx, j, ct, stride);
			}
			break;
		case 0x18:
			BTIC1H_DecodeQfAbsYUVDyuv(ctx);
			break;
		case 0x19:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix16_2x2x1(ctx);
				ct=BTIC1H_FillBlock4x4x1(ctx, j, ct, stride);
			}
			break;
		case 0x1A:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix16_4x4x1(ctx);
				ct=BTIC1H_FillBlock4x4x1(ctx, j, ct, stride);
			}
			break;
		case 0x1B:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			for(i=0; i<n; i++)
			{
				BTIC1H_DecodeDeltaYUV(ctx);
				ct=BTIC1H_FillBlockFlat(ctx, ct, stride);
			}
			break;
		case 0x1C:
			BTIC1H_DecodeDeltaYUV(ctx);
			ypix[0]=ctx->cy;	upix[0]=ctx->cu;
			vpix[0]=ctx->cv;	dpix[0]=0;
			BTIC1H_DecodeDeltaYUV(ctx);
			ypix[2]=ctx->cy;	upix[2]=ctx->cu;
			vpix[2]=ctx->cv;	dpix[2]=0;
			BTIC1H_DecodeDeltaYUV(ctx);
			ypix[3]=ctx->cy;	upix[3]=ctx->cu;
			vpix[3]=ctx->cv;	dpix[3]=0;
			BTIC1H_DecodeDeltaYUV(ctx);
			ypix[1]=ctx->cy;	upix[1]=ctx->cu;
			vpix[1]=ctx->cv;	dpix[1]=0;
			ct=BTIC1H_FillBlock4x4B420(ctx,
				ypix, upix, vpix, dpix,
				0, ct, stride);
			break;
		case 0x1D:
			BTIC1H_DecodeDeltaYUVD(ctx);
			ypix[0]=ctx->cy;	upix[0]=ctx->cu;
			vpix[0]=ctx->cv;	dpix[0]=ctx->cd;
			BTIC1H_DecodeDeltaYUVD(ctx);
			ypix[2]=ctx->cy;	upix[2]=ctx->cu;
			vpix[2]=ctx->cv;	dpix[2]=ctx->cd;
			BTIC1H_DecodeDeltaYUVD(ctx);
			ypix[3]=ctx->cy;	upix[3]=ctx->cu;
			vpix[3]=ctx->cv;	dpix[3]=ctx->cd;
			BTIC1H_DecodeDeltaYUVD(ctx);
			ypix[1]=ctx->cy;	upix[1]=ctx->cu;
			vpix[1]=ctx->cv;	dpix[1]=ctx->cd;
			j=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4B420(ctx,
				ypix, upix, vpix, dpix,
				j, ct, stride);
			break;
		case 0x1E:
			BTIC1H_ReadPixDeltaYUV4x4(ctx, ypix, upix, vpix);
			ct=BTIC1H_FillBlock4x4YUV444(ctx,
				ypix, upix, vpix, ct, stride);
			break;
		case 0x1F:
			BTIC1H_ReadPixAbsYUV4x4(ctx, ypix, upix, vpix);
			ct=BTIC1H_FillBlock4x4YUV444(ctx,
				ypix, upix, vpix, ct, stride);
			break;
		case 0x20:
			ret=1;
			break;
		case 0x21:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if(!lblks)break;
			csl=lblks+(ct-blks);
//			memcpy(ct, csl, n*stride);
			if((n<=0) || (ct+(n*stride))>cte)
				{ ret=-1; break; }
			BTIC1H_DecodeCopyBlocks(ctx, ct, csl, n, stride);
			if(blksfl)
			{
				cfl=blksfl+((ct-blks)>>blkshr);
				for(i=0; i<n; i++)
					{ cfl[i]|=1; }
			}
			ct=ct+(n*stride);
			break;
		case 0x22:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmxy));
			yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmxy));
			if(!lblks)break;
			csl=lblks+(ct-blks)+((yo*ctx->xbsz+xo)*stride);
			if((csl<lblks2) || ((csl+(n*stride))>csle2))
				{ ret=-1; break; }
//			memcpy(ct, csl, n*stride);
			BTIC1H_DecodeCopyBlocks(ctx, ct, csl, n, stride);
			ct=ct+(n*stride);
			break;
		case 0x23:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmval));
			if(xo<0)
			{
				if(xo==-1)
					{ if(yo==0)break; }
				if(xo==-2)
				{
					if(!ctx->colorpred)
					{
						ctx->lcy=ctx->cy; ctx->lcu=ctx->cu;
						ctx->lcv=ctx->cv; ctx->lcd=ctx->cd;
						ctx->lcdy=ctx->cdy; ctx->lcdu=ctx->cdu;
						ctx->lcdv=ctx->cdv;
					}
					ctx->colorpred=-yo;
					break;
				}
				ret=-1; break;
			}
			break;
		case 0x24:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			k=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmix));
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
				{ yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmval)); }
			if(xo<0) { ret=-1; break; }
			break;

		case 0x25:
			ret=-1; break;

		case 0x26:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			if(xo<0)
			{
				if(xo==-1)
				{
					if(!ctx->updmask)
					{
						ctx->updmask=255;
						ctx->nextupdmask=255;
					}
					break;
				}
				if(xo==-2)
				{
					if(!ctx->absupdmask)
					{
						ctx->absupdmask=255;
						ctx->nextabsupdmask=255;
					}
					break;
				}
				ret=-1; break;
			}
			break;
		case 0x27:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			if(xo<0)
			{
				if(xo==-1)
				{
					ctx->updmask=0;
					ctx->nextupdmask=0;
					break;
				}
				if(xo==-1)
				{
					ctx->absupdmask=0;
					ctx->nextabsupdmask=0;
					break;
				}
				ret=-1; break;
			}
			break;
		case 0x28:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				BTIC1H_DecodeDeltaYUVD(ctx);
				j=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlockGr2x2(ctx, j, ct, stride);
			}
			break;
		case 0x29:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				BTIC1H_DecodeDeltaYUVDyuv(ctx);
				j=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlockGr2x2B(ctx, j, ct, stride);
			}
			break;

		case 0x2A:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix16_4x4x1(ctx);
			ct=BTIC1H_FillBlock4x4x1(ctx, i, ct, stride);
			break;
		case 0x2B:
			BTIC1H_DecodeDeltaYUVD(ctx);
			i=BTIC1H_ReadPix16_2x2x1(ctx);
			ct=BTIC1H_FillBlock4x4x1(ctx, i, ct, stride);
			break;
		case 0x2C:
			BTIC1H_DecodeDeltaYUVD(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			ct=BTIC1H_FillBlock4x4x3(ctx, lpx, ct, stride);
			break;
		case 0x2D:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			ct=BTIC1H_FillBlock4x4x3B(ctx, lpx, ct, stride);
			break;
		case 0x2E:
			BTIC1H_ReadPixDeltaY4x4(ctx, ypix);
			BTIC1H_ReadPixDeltaUV2x2(ctx, upix, vpix);
			ct=BTIC1H_FillBlock4x4YUV420(ctx,
				ypix, upix, vpix, ct, stride);
			break;
		case 0x2F:
			BTIC1H_ReadPixAbsY4x4(ctx, ypix);
			BTIC1H_ReadPixAbsUV2x2(ctx, upix, vpix);
			ct=BTIC1H_FillBlock4x4YUV420(ctx,
				ypix, upix, vpix, ct, stride);
			break;

		case 0x30:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			j=BTIC1H_ReadPix32_4x4(ctx);
			k=BTIC1H_ReadPix8_2x2(ctx);
			l=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock4x4B_UV2x2(ctx, j, k, l, ct, stride);
			break;
		case 0x31:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			j=BTIC1H_ReadPix32_4x4(ctx);
			k=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillBlock4x4x3B_UV4x4(ctx, lpx, j, k, ct, stride);
			break;
		case 0x32:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			j=BTIC1H_ReadPix8_2x2(ctx);
			k=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock4x4x3B_UV2x2(ctx, lpx, j, k, ct, stride);
			break;
		case 0x33:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			j=BTIC1H_ReadPix8_2x2(ctx);
			k=BTIC1H_ReadPix8_2x2(ctx);
			l=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillBlock2x2B_UV2x2(ctx, j, k, l, ct, stride);
			break;

		case 0x34:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_4x4(ctx);
				k=BTIC1H_ReadPix8_2x2(ctx);
				l=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlock4x4B_UV2x2(ctx, j, k, l, ct, stride);
			}
			break;
		case 0x35:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				lpx=BTIC1H_ReadPix48_4x4x3(ctx);
				j=BTIC1H_ReadPix32_4x4(ctx);
				k=BTIC1H_ReadPix32_4x4(ctx);
				ct=BTIC1H_FillBlock4x4x3B_UV4x4(ctx,
					lpx, j, k, ct, stride);
			}
			break;
		case 0x36:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				lpx=BTIC1H_ReadPix48_4x4x3(ctx);
				j=BTIC1H_ReadPix8_2x2(ctx);
				k=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlock4x4x3B_UV2x2(ctx,
					lpx, j, k, ct, stride);
			}
			break;
		case 0x37:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix8_2x2(ctx);
				k=BTIC1H_ReadPix8_2x2(ctx);
				l=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillBlock2x2B_UV2x2(ctx, j, k, l, ct, stride);
			}
			break;

		case 0x38:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			j=BTIC1H_ReadPix32_4x4(ctx);
			k=BTIC1H_ReadPix32_2x4(ctx);
			l=BTIC1H_ReadPix32_2x4(ctx);
			ct=BTIC1H_FillBlock4x4B_UV4x4(ctx, j, k, l, ct, stride);
			break;
		case 0x39:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			j=BTIC1H_ReadPix32_2x4(ctx);
			k=BTIC1H_ReadPix32_2x4(ctx);
			ct=BTIC1H_FillBlock4x4x3B_UV4x4(ctx, lpx, j, k, ct, stride);
			break;
		case 0x3A:
			BTIC1H_DecodeDeltaYUVDyuv(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			j=BTIC1H_ReadPix24_2x4x3(ctx);
			k=BTIC1H_ReadPix24_2x4x3(ctx);
			ct=BTIC1H_FillBlock4x4x3B_UV2x4x3(ctx,
				lpx, j, k, ct, stride);
			break;

		case 0x3C:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_4x4(ctx);
				k=BTIC1H_ReadPix32_2x4(ctx);
				l=BTIC1H_ReadPix32_2x4(ctx);
				ct=BTIC1H_FillBlock4x4B_UV4x4(ctx,
					j, k, l, ct, stride);
			}
			break;
		case 0x3D:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				lpx=BTIC1H_ReadPix48_4x4x3(ctx);
				j=BTIC1H_ReadPix32_2x4(ctx);
				k=BTIC1H_ReadPix32_2x4(ctx);
				ct=BTIC1H_FillBlock4x4x3B_UV4x4(ctx,
					lpx, j, k, ct, stride);
			}
			break;
		case 0x3E:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				lpx=BTIC1H_ReadPix48_4x4x3(ctx);
				j=BTIC1H_ReadPix24_2x4x3(ctx);
				k=BTIC1H_ReadPix24_2x4x3(ctx);
				ct=BTIC1H_FillBlock4x4x3B_UV2x4x3(ctx,
					lpx, j, k, ct, stride);
			}
			break;

		case 0x40:
			BTIC1H_DecodeAbsYUVD(ctx);
			break;
		case 0x41:
			BTIC1H_DecodeAbsYUVDyuv(ctx);
			break;

		default:
			printf("BTIC1H_DecodeBlocksCtx: Bad Command %02X\n", cmd);
#ifndef BTIC1H_VFWDRV
			BT1H_TRAPCRASH
#endif
			ret=-1;
			break;
		}
	}
	return(ret);
}


#ifdef BT1H_ENABLE_AX
int BTIC1H_DecodeAlphaBlocksCtx(BTIC1H_Context *ctx,
	byte *blks, byte *lblks, int nblks, int stride,
	int *rnblks)
{
	byte ypix[16], upix[16], vpix[16], dpix[16];
	byte *ct, *cte, *csl;
	u64 lpx;
	int cmd, ret;
	int n, xo, yo;
	int i, j, k;

	ct=blks; cte=ct+nblks*stride; csl=lblks;
	
	ret=0;
	while((ct<cte) && !ret)
	{
		cmd=BTIC1H_ReadCommandCode(ctx);
		switch(cmd)
		{
		case 0x00:
			BTIC1H_DecodeDeltaA(ctx);
			ct=BTIC1H_FillAlphaBlockFlat(ctx, ct, stride);
			break;
		case 0x01:
			BTIC1H_DecodeDeltaAD(ctx);
			i=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillAlphaBlock2x2(ctx, i, ct, stride);
			break;
		case 0x02:
			BTIC1H_DecodeDeltaAD(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillAlphaBlock4x4(ctx, i, ct, stride);
			break;
		case 0x03:
			BTIC1H_DecodeDeltaAD(ctx);
			lpx=BTIC1H_ReadPix48_4x4x3(ctx);
			ct=BTIC1H_FillAlphaBlock4x4x3(ctx, lpx, ct, stride);
			break;
		case 0x04:
			BTIC1H_DecodeQfDeltaAD(ctx);
			break;
		case 0x05:
			BTIC1H_DecodeDeltaAD(ctx);
			i=BTIC1H_ReadPix16_4x4x1(ctx);
			ct=BTIC1H_FillAlphaBlock4x4x1(ctx, i, ct, stride);
			break;
		case 0x06:
			BTIC1H_DecodeAbsAD(ctx);
			i=BTIC1H_ReadPix8_2x2(ctx);
			ct=BTIC1H_FillAlphaBlock2x2(ctx, i, ct, stride);
			break;
		case 0x07:
			BTIC1H_DecodeAbsAD(ctx);
			i=BTIC1H_ReadPix32_4x4(ctx);
			ct=BTIC1H_FillAlphaBlock4x4(ctx, i, ct, stride);
			break;
		case 0x08:
			BTIC1H_DecodeQfAbsAD(ctx);
			break;
		case 0x09:
			BTIC1H_DecodeDeltaAD(ctx);
			i=BTIC1H_ReadPix16_2x2x1(ctx);
			ct=BTIC1H_FillAlphaBlock4x4x1(ctx, i, ct, stride);
			break;
		case 0x0A:
			ct=BTIC1H_FillAlphaBlockNoAlpha(ctx, ct, stride);
			break;
		case 0x0B:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
				{ ct=BTIC1H_FillAlphaBlockNoAlpha(ctx, ct, stride); }
			break;
		case 0x0C:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
				{ ct=BTIC1H_FillAlphaBlockFlat(ctx, ct, stride); }
			break;
		case 0x0D:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix8_2x2(ctx);
				ct=BTIC1H_FillAlphaBlock2x2(ctx, j, ct, stride);
			}
			break;
		case 0x0E:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix32_4x4(ctx);
				ct=BTIC1H_FillAlphaBlock4x4(ctx, j, ct, stride);
			}
			break;
		case 0x0F:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			for(i=0; i<n; i++)
			{
				j=BTIC1H_ReadPix16_4x4x1(ctx);
				ct=BTIC1H_FillAlphaBlock4x4x1(ctx, j, ct, stride);
			}
			break;


//		case 0x19:
//			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
//			for(i=0; i<n; i++)
//			{
//				j=BTIC1H_ReadPix16_2x2x1(ctx);
//				ct=BTIC1H_FillBlock4x4x1(ctx, j, ct, stride);
//			}
//			break;

		case 0x20:
			ret=1;
			break;
		case 0x21:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			if(!lblks)break;
			csl=lblks+(ct-blks);
			BTIC1H_DecodeCopyAlphaBlocks(ct, csl, n, stride);
			ct=ct+(n*stride);
			break;
		case 0x22:
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_cmdcnt));
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmxy));
			yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmxy));
			if(!lblks)break;
			csl=lblks+(ct-blks)+((yo*ctx->xbsz+xo)*stride);
			BTIC1H_DecodeCopyAlphaBlocks(ct, csl, n, stride);
			ct=ct+(n*stride);
			break;
		case 0x23:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmval));
			if(xo<0)
			{
				if(xo==-1)
					{ if(yo==0)break; }
				ret=-1; break;
			}
			break;
		case 0x24:
			xo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmvar));
			k=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmix));
			n=BTIC1H_Rice_ReadAdRice(ctx, &(ctx->rk_parmix));
			for(i=0; i<n; i++)
			{
				yo=BTIC1H_Rice_ReadAdSRice(ctx, &(ctx->rk_parmval));
			}
			if(xo<0) { ret=-1; break; }
			break;

		default:
			ret=-1;
			break;
		}
	}
	return(ret);
}
#endif

int BTIC1H_DecodeBlocksBuffer(
	byte *ibuf, byte *blks, byte *lblks, int nblks, int stride,
	int xbsz, int ybsz, int ibsz)
{
	BTIC1H_Context tctx;
	BTIC1H_Context *ctx;
	int i;
	
	ctx=&tctx;
	memset(ctx, 0, sizeof(BTIC1H_Context));

	BTIC1H_SetupDecodeContextInitial(ctx);

	ctx->xbsz=xbsz;
	ctx->ybsz=ybsz;

	BTIC1H_Rice_SetupRead(ctx, ibuf, ibsz);
	BTIC1H_DecodeBlocksCtx(ctx, blks, lblks, nblks, stride, &i);
	return(i);
}

int BTIC1H_DecodeWorkSliceCtx(BTIC1H_Context *ctx)
{
	byte *cs, *cse;
	int sys, yb, nblks;
	int str;
	int i, j, k;
	
	cs=ctx->bs_css;
	cse=ctx->bs_cse;
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

	ctx->ReadCommandCode=BTIC1H_ReadCommandCodeSMTF;

	BTIC1H_SetupDecodeContextInitial(ctx);
	BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);

	BTIC1H_DecodeBlocksCtx(ctx,
		ctx->blks+j, ctx->lblks+j,
		nblks, 32, &i);

	if(btjpg_drv_defaultCodecDbfl&BTIC1H_DBFL_CLEARSKIP)
	{
		memset(ctx->blksfl+(j>>5), 0, (nblks+7)>>3);
	}

	BTIC1H_DecodeImageMB2B_ClrsBfl(
		ctx, ctx->blks+j, ctx->blksfl+(j>>5),
		32, ctx->ibuf+k,
		ctx->xs, sys, ctx->clrs);
	return(0);
}

BTIC1H_API int BTIC1H_DecodeCtx(BTIC1H_Context *ctx,
	byte *src, byte *dst, int ssz, int dsz,
	int *rxy, int *rys, int clrsfl)
{
	BTIC1H_Context *encjob[128];
	BTIC1H_Context *etctx;
	byte *cs, *cse, *csi, *csax, *csmtx;
	byte *cs1, *cs2, *ct;
	int clrs, clrsfl1, xs0, ys0, xs1, ys1;
	int csim, slys, slbs, sltid, csbs;
	int i, j, k, l;

	if(ssz==0)
		return(0);

	clrs=clrsfl&255;

	if(!ctx->blks)
	{
		ctx->xbsz=(ctx->xs+3)>>2;
		ctx->ybsz=(ctx->ys+3)>>2;
		ctx->nblks=ctx->xbsz*ctx->ybsz;
		
		ctx->blks=malloc(ctx->nblks*32);
		ctx->lblks=malloc(ctx->nblks*32);
		ctx->blksfl=malloc(ctx->nblks);
		
		memset(ctx->blks, 0, ctx->nblks*32);
		memset(ctx->lblks, 0, ctx->nblks*32);
		memset(ctx->blksfl, 0, ctx->nblks);
	}else if(clrsfl&(BTIC1H_QFL_USEMIP|BTIC1H_QFL_ISMIPLVL))
	{
		ctx->xbsz=(ctx->xs+3)>>2;
		ctx->ybsz=(ctx->ys+3)>>2;
		ctx->nblks=ctx->xbsz*ctx->ybsz;
	}

	cs=src; cse=src+ssz; csi=NULL;
	csax=NULL; csmtx=NULL; csim=0;
	while(cs<cse)
	{
		if(*cs==0xE0)
			break;
		if(*cs==0xE1)
		{
			csim=ctx->lcsim;
			csi=cs;
			i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs=cs+i;
			continue;
		}else if(*cs==0xE2)
		{
			csim=ctx->lcsim;
			csi=cs;
			cs=cs+cs[1];
			continue;
		}else if(*cs==0xE3)
		{
			i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			if((cs[4]=='A') && (cs[5]=='X'))
				{ csax=cs; }

			if((cs[4]=='I') && (cs[5]>='0') && (cs[5]<='9'))
				{ csi=cs; csim=cs[5]-'0'; }

			cs=cs+i;
			continue;
		}else if(*cs==0xE4)
		{
			if((cs[4]=='M') && (cs[5]=='I') &&
					(cs[6]=='P') && (cs[7]=='1'))
				{ csmtx=cs; }

			i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs=cs+i;
			continue;
		}else if(*cs==0xE5)
		{
			if((cs[2]=='A') && (cs[3]=='X'))
				{ csax=cs; }
			cs=cs+cs[1];
			continue;
		}
		
		return(-1);
	}

	if(!csi)
		return(-1);

	if(csi[0]==0xE1)
	{
		i=(csi[1]<<16)|(csi[2]<<8)|csi[3];
		cs=csi+4; cse=csi+i;
	}else if(csi[0]==0xE2)
	{
		i=csi[1];
		cs=csi+2; cse=csi+i;
	}else if(csi[0]==0xE3)
	{
		i=(csi[1]<<16)|(csi[2]<<8)|csi[3];
		cs=csi+6; cse=csi+i;
	}else
	{
		return(-1);
	}

	BTIC1H_SetupDecodeContextInitial(ctx);

//	memset(ctx->blksfl, 0, ctx->nblks);

//	BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);
//	BTIC1H_Rice_SetupRead(ctx, src+4, ssz-4);
	
	ctx->lcsim=csim;

	switch(csim&15)
	{
	case 0:
		ctx->ReadCommandCode=BTIC1H_ReadCommandCodeBase;
		BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);
		break;
	case 1:
		ctx->ReadCommandCode=BTIC1H_ReadCommandCodeSMTF;
//		BTIC1H_Rice_SetupReadRange(ctx, cs, cse-cs);
		BTIC1H_Rice_SetupRead2(ctx, cs, cse-cs);
		break;
	default:
		return(-1);
		break;
	}

	ctx->hasax=(csax!=NULL);

	if(ctx->slscl)
	{
		slys=(ctx->ys+(ctx->slscl-1))/ctx->slscl;
		slbs=((ctx->xs+3)>>2)*((ctx->slscl+3)>>2);
		cs=ctx->bs_css;
		
		if(btic1h_workqueue_defaultworkers>0)
		{
			for(i=0; i<slys; i++)
			{
				cs1=cs;
				j=BTIC1H_DecodeReadVLI(ctx, &cs1);
				cs2=cs+j;

				etctx=BTIC1H_AllocContext();
				encjob[i]=etctx;

				etctx->xs=ctx->xs;
				etctx->ys=ctx->ys;
				etctx->clrs=clrs;
//				etctx->qfl=qfl;
				etctx->clryuv=ctx->clryuv;
				etctx->clrdty=ctx->clrdty;
				etctx->flip=ctx->flip;
				etctx->hasax=ctx->hasax;

				etctx->xbsz=ctx->xbsz;
				etctx->ybsz=ctx->ybsz;
				etctx->nblks=ctx->nblks;

				etctx->ibuf=dst;
				etctx->blks=ctx->blks;
				etctx->lblks=ctx->lblks;
				etctx->blksfl=ctx->blksfl;

				etctx->bs_css=cs1;
				etctx->bs_cse=cs2;
				
				etctx->slscl=ctx->slscl;
				etctx->slys=slys;
				etctx->slbs=slbs;
				etctx->slix=i;
				
				etctx->DoWork=BTIC1H_DecodeWorkSliceCtx;

				cs=cs2;
			}

			btic1h_thLockQueue();
			sltid=BTIC1H_Work_AllocTidNb();
			for(i=0; i<slys; i++)
				{ BTIC1H_Work_QueueJobNb(encjob[i], sltid); }
			btic1h_thUnlockQueue();
			BTIC1H_Work_CheckSpawnWorkers(
				btic1h_workqueue_defaultworkers);
			
			BTIC1H_Work_WaitTaskComplete(sltid);
			
//			while(BTIC1H_Work_GetTidCountNb(sltid)>0)
//				{ thSleep(1); }
			BTIC1H_Work_FreeTidNb(sltid);
			for(i=0; i<slys; i++)
				{ BTIC1H_FreeContextF(encjob[i]); }
		}else
		{
			for(i=0; i<slys; i++)
			{
				cs1=cs;
				j=BTIC1H_DecodeReadVLI(ctx, &cs1);
				cs2=cs+j;

				BTIC1H_SetupDecodeContextInitial(ctx);
				BTIC1H_Rice_SetupRead(ctx, cs1, j);

				j=i*slbs*32;
				BTIC1H_DecodeBlocksCtx(ctx,
					ctx->blks+j, ctx->lblks+j,
					slbs, 32, &k);
				cs=cs2;
			}

			BTIC1H_DecodeImageMB2B_ClrsBfl(
				ctx, ctx->blks, ctx->blksfl,
				32, dst,
				ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), clrs);
		}
	}else
	{
		BTIC1H_DecodeBlocksCtx(ctx,
			ctx->blks, ctx->lblks,
			ctx->nblks, 32, &i);

		if(csax && (clrs!=BTIC1H_PXF_RGBA) && (clrs!=BTIC1H_PXF_BGRA))
			{ csax=NULL; }
		
#ifdef BT1H_ENABLE_AX
		if(csax)
		{
			if(csax[0]==0xE3)
			{
				i=(csax[1]<<16)|(csax[2]<<8)|csax[3];
				cs=csax+6; cse=csax+i;
			}else if(csax[0]==0xE5)
			{
				i=csax[1];
				cs=csax+4; cse=csax+i;
			}

			BTIC1H_SetupContextInitial(ctx);

			BTIC1H_Rice_SetupRead(ctx, cs, cse-cs);
			BTIC1H_DecodeAlphaBlocksCtx(ctx,
				ctx->blks+24, ctx->lblks+24,
				ctx->nblks, 32, &i);
		}
#endif

		BTIC1H_DecodeImageMB2B_ClrsBfl(
			ctx, ctx->blks, ctx->blksfl,
			32, dst,
			ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), clrs);
	
//		BTIC1H_DecodeImageMB2B_Clrs(ctx->blks, 32, dst,
//			ctx->xs, ctx->flip?ctx->ys:(-ctx->ys), clrs);
	}
	
//	memcpy(ctx->lblks, ctx->blks, ctx->nblks*32);

	cs1=ctx->lblks;
	ctx->lblks=ctx->blks;
	ctx->blks=cs1;

	if(btjpg_drv_defaultCodecDbfl&BTIC1H_DBFL_CLEARSKIP)
	{
		memset(ctx->lblks, 0, ctx->nblks*32);
	}
	
	if((clrsfl&BTIC1H_QFL_USEMIP) && csmtx &&
		((clrs==BTIC1H_PXF_BC1)||
		 (clrs==BTIC1H_PXF_BC3)||
		 (clrs==BTIC1H_PXF_BC7)))
	{
		xs0=ctx->xs;
		ys0=ctx->ys;
		
		csbs=16;
		if(clrs==BTIC1H_PXF_BC1)
			csbs=8;
		
		cs=csmtx;
		clrsfl1=clrsfl&(~BTIC1H_QFL_USEMIP);
		clrsfl1|=BTIC1H_QFL_ISMIPLVL;
		
		xs1=(xs0+1)>>1; ys1=(ys0+1)>>1;
		ct=dst; ct+=ctx->nblks*csbs;
		
		while((xs1>1) || (ys1>2))
		{
			i=(cs[1]<<16)|(cs[2]<<8)|cs[3];
			cs1=cs+i;
		
			ctx->xs=xs1; ctx->ys=ys1;
			BTIC1H_DecodeCtx(ctx, cs+8, ct, i-8, dsz-(ct-dst),
				NULL, NULL, clrsfl1);
			
			cs=cs1;
			ct+=ctx->nblks*csbs;
			xs1=(xs1+1)>>1; ys1=(ys1+1)>>1;
		}

		ctx->xs=xs0;
		ctx->ys=ys0;
	}

	return(0);
}
