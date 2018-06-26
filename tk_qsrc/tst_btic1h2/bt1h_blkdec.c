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

#ifdef BT1H_ENABLE_AX
void BTIC1H_DecodeBlockMB2B_Alpha(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	byte clr[16];
	byte *ct;
	int i0, i1, i2, i3;
	int cd, bt, cy, cya, cyb;
	int ci, pxb;
	int i, j, k, l;

	i=block[1];
	cd=i; bt=6;
	
	if(i==0)
	{
		if(block[4]==0)
		{
			if(xstride==4)
			{
				for(i=0; i<4; i++)
				{
					ct=rgba+(i*ystride);
					ct[ 0]=255;	ct[ 4]=255;
					ct[ 8]=255;	ct[12]=255;
				}
				return;
			}

			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
					{ rgba[i*ystride+j*xstride]=255; }
			return;
		}

		if(block[4]==7)
		{
			cy=block[0];

			if(xstride==4)
			{
				for(i=0; i<4; i++)
				{
					ct=rgba+(i*ystride);
					ct[ 0]=cy;	ct[ 4]=cy;
					ct[ 8]=cy;	ct[12]=cy;
				}
				return;
			}

			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
			{
				k=i*ystride+j*xstride;
				rgba[k]=cy;
			}
			return;
		}	

		if(i==1)
			{ cd=block[2]; bt=1; }
		else if(i==2)
			{ cd=block[2]; bt=2; }
		else if(i==6)
			{ cd=block[2]; bt=0; }
	}

	cy=block[0];

	cya=cy-(cd>>1);
	cyb=cya+cd;

	if(bt==6)
	{
		clr[0]=clamp255(cya);
		clr[7]=clamp255(cyb);
		clr[1]=(clr[0]*14+clr[7]* 2)>>4;
		clr[2]=(clr[0]*11+clr[7]* 5)>>4;
		clr[3]=(clr[0]* 9+clr[7]* 7)>>4;
		clr[4]=(clr[0]* 7+clr[7]* 9)>>4;
		clr[5]=(clr[0]* 5+clr[7]*11)>>4;
		clr[6]=(clr[0]* 2+clr[7]*14)>>4;
	}else
	{
		clr[0]=clamp255(cya);
		clr[3]=clamp255(cyb);
		clr[1]=(clr[0]*11+clr[3]* 5)>>4;
		clr[2]=(clr[0]* 5+clr[3]*11)>>4;
	}
	
	if(bt==0)
	{
		for(i=0; i<4; i++)
		{
			ct=rgba+(i*ystride);
			i0=(block[4+i]>>6)&3;	i1=(block[4+i]>>4)&3;
			i2=(block[4+i]>>2)&3;	i3=(block[4+i]   )&3;
			ct[0]=clr[i0];	ct[ 4]=clr[i1];
			ct[8]=clr[i2];	ct[12]=clr[i3];
		}
		return;
	}else if(bt==1)
	{
		pxb=block[6];
		i0=(block[4+i]>>6)&3;	i1=(block[4+i]>>4)&3;
		i2=(block[4+i]>>2)&3;	i3=(block[4+i]   )&3;
		ct=rgba;
		ct[0]=clr[i0];  ct[ 4]=clr[i0];
		ct[8]=clr[i1];  ct[12]=clr[i1];
		ct=ct+ystride;
		ct[0]=clr[i0];  ct[ 4]=clr[i0];
		ct[8]=clr[i1];  ct[12]=clr[i1];
		ct=ct+ystride;
		ct[0]=clr[i2];  ct[ 4]=clr[i2];
		ct[8]=clr[i3];  ct[12]=clr[i3];
		ct=ct+ystride;
		ct[0]=clr[i2];  ct[ 4]=clr[i2];
		ct[8]=clr[i3];  ct[12]=clr[i3];
		return;
	}else if(bt==2)
	{
		pxb=(block[6]<<8)|block[7];
		for(i=0; i<4; i++)
		{
			ct=rgba+(i*ystride);
			j=pxb>>((3-i)*4);
			i0=(j>>3)&1;	i1=(j>>2)&1;
			i2=(j>>1)&1;	i3=(j   )&1;
			ct[0]=clr[i0];  ct[ 4]=clr[i1];
			ct[8]=clr[i2];  ct[12]=clr[i3];
		}
		return;
	}else if(bt==6)
	{	
		i=(block[2]<<16)|(block[3]<<8)|block[4];
		j=(block[5]<<16)|(block[6]<<8)|block[7];
		ct=rgba;
		i0=(i>>21)&7; i1=(i>>18)&7;  i2=(i>>15)&7; i3=(i>>12)&7;
		ct[0]=clr[i0]; ct[4]=clr[i1]; ct[8]=clr[i2]; ct[12]=clr[i3];
		ct=ct+ystride;
		i0=(i>>9)&7; i1=(i>>6)&7;  i2=(i>>3)&7; i3=i&7;
		ct[0]=clr[i0]; ct[4]=clr[i1]; ct[8]=clr[i2]; ct[12]=clr[i3];
		ct=ct+ystride;
		i0=(j>>21)&7; i1=(j>>18)&7;  i2=(j>>15)&7; i3=(j>>12)&7;
		ct[0]=clr[i0]; ct[4]=clr[i1]; ct[8]=clr[i2]; ct[12]=clr[i3];
		ct=ct+ystride;
		i0=(j>>9)&7; i1=(j>>6)&7;  i2=(j>>3)&7;  i3=j&7;
		ct[0]=clr[i0]; ct[4]=clr[i1]; ct[8]=clr[i2]; ct[12]=clr[i3];
		return;
	}
}
#endif

#if 0
// void BTIC1H_DecodeBlock_TransColor1Nc(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	int cy1, cu1, cv1;
	int cr1, cg1, cb1;

	cy1=cy; cu1=cu-128; cv1=cv-128;
	cr1=(256*cy1        +359*cv1+128)>>8;
	cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb1=(256*cy1+454*cu1        +128)>>8;
	*rcr=cr1;	*rcg=cg1;	*rcb=cb1;
}
#endif

void BTIC1H_DecodeBlock_TransColor1YCbCr(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	int cy1, cu1, cv1;
	int cr1, cg1, cb1;

	cy1=cy; cu1=cu-128; cv1=cv-128;
	cr1=(256*cy1        +359*cv1+128)>>8;
	cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb1=(256*cy1+454*cu1        +128)>>8;

	if((cr1|cg1|cb1)>>8)
	{
		cr1=clamp255(cr1);
		cg1=clamp255(cg1);
		cb1=clamp255(cb1);
	}

	*rcr=cr1;	*rcg=cg1;	*rcb=cb1;
}

void BTIC1H_DecodeBlock_TransColor1ABYCbCr(BTIC1H_Context *ctx,
	int cya, int cua, int cva, int *rcra, int *rcga, int *rcba,
	int cyb, int cub, int cvb, int *rcrb, int *rcgb, int *rcbb)
{
	int cy1, cu1, cv1;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;

	cy1=cya; cu1=cua-128; cv1=cva-128;
	cr1=(256*cy1        +359*cv1+128)>>8;
	cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb1=(256*cy1+454*cu1        +128)>>8;

	cy1=cyb; cu1=cub-128; cv1=cvb-128;
	cr2=(256*cy1        +359*cv1+128)>>8;
	cg2=(256*cy1- 88*cu1-183*cv1+128)>>8;
	cb2=(256*cy1+454*cu1        +128)>>8;

	if((cr1|cg1|cb1|cr2|cg2|cb2)>>8)
	{
		cr1=clamp255(cr1);	cg1=clamp255(cg1);
		cb1=clamp255(cb1);	cr2=clamp255(cr2);
		cg2=clamp255(cg2);	cb2=clamp255(cb2);
	}

	*rcra=cr1;	*rcga=cg1;	*rcba=cb1;
	*rcrb=cr2;	*rcgb=cg2;	*rcbb=cb2;
}

void BTIC1H_DecodeBlock_TransColor2YCbCr(BTIC1H_Context *ctx,
	int cy0, int cy1, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1)
{
	int cu1, cv1, cu2, cv2, cuv2;
	int cy0t, cy1t, cy2t, cy3t;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;

	cu1=cu-128; cv1=cv-128;

	cu2=454*cu1;	cv2=359*cv1;	cuv2=-88*cu1-183*cv1;
	cy0t=(cy0<<8)+128;	cy1t=(cy1<<8)+128;
	cr0=(cy0t+cv2 )>>8;		cg0=(cy0t+cuv2)>>8;
	cb0=(cy0t+cu2 )>>8;		cr1=(cy1t+cv2 )>>8;
	cg1=(cy1t+cuv2)>>8;		cb1=(cy1t+cu2 )>>8;

	if((cr0|cr1)>>8)
	{	cr0=clamp255(cr0);	cr1=clamp255(cr1);	}
	if((cg0|cg1)>>8)
	{	cg0=clamp255(cg0);	cg1=clamp255(cg1);	}
	if((cb0|cb1)>>8)
	{	cb0=clamp255(cb0);	cb1=clamp255(cb1);	}

	*rcr0=cr0;	*rcg0=cg0;	*rcb0=cb0;
	*rcr1=cr1;	*rcg1=cg1;	*rcb1=cb1;
}

void BTIC1H_DecodeBlock_TransColor420YCbCr(BTIC1H_Context *ctx,
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1,
	int *rcr2, int *rcg2, int *rcb2,
	int *rcr3, int *rcg3, int *rcb3)
{
	int cu1, cv1, cu2, cv2, cuv2;
	int cy0t, cy1t, cy2t, cy3t;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;

	cu1=cu-128; cv1=cv-128;

	cu2=454*cu1;	cv2=359*cv1;	cuv2=-88*cu1-183*cv1;
	cy0t=(cy0<<8)+128;	cy1t=(cy1<<8)+128;
	cy2t=(cy2<<8)+128;	cy3t=(cy3<<8)+128;
	cr0=(cy0t+cv2 )>>8;		cg0=(cy0t+cuv2)>>8;
	cb0=(cy0t+cu2 )>>8;		cr1=(cy1t+cv2 )>>8;
	cg1=(cy1t+cuv2)>>8;		cb1=(cy1t+cu2 )>>8;
	cr2=(cy2t+cv2 )>>8;		cg2=(cy2t+cuv2)>>8;
	cb2=(cy2t+cu2 )>>8;		cr3=(cy3t+cv2 )>>8;
	cg3=(cy3t+cuv2)>>8;		cb3=(cy3t+cu2 )>>8;
	
	if((cr0|cr1|cr2|cr3)>>8)
	{	cr0=clamp255(cr0);	cr1=clamp255(cr1);
		cr2=clamp255(cr2);	cr3=clamp255(cr3);	}
	if((cg0|cg1|cg2|cg3)>>8)
	{	cg0=clamp255(cg0);	cg1=clamp255(cg1);
		cg2=clamp255(cg2);	cg3=clamp255(cg3);	}
	if((cb0|cb1|cb2|cb3)>>8)
	{	cb0=clamp255(cb0);	cb1=clamp255(cb1);
		cb2=clamp255(cb2);	cb3=clamp255(cb3);	}

	*rcr0=cr0;	*rcg0=cg0;	*rcb0=cb0;
	*rcr1=cr1;	*rcg1=cg1;	*rcb1=cb1;
	*rcr2=cr2;	*rcg2=cg2;	*rcb2=cb2;
	*rcr3=cr3;	*rcg3=cg3;	*rcb3=cb3;
}

#if 1
void BTIC1H_DecodeBlock_TransColor1GDbDr(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	int cy1, cu1, cv1;
	int cr1, cg1, cb1;

	cy1=cy; cu1=cu-128; cv1=cv-128;
	cr1=cy+(cv1<<1);
	cg1=cy;
	cb1=cy+(cu1<<1);

	if((cr1|cg1|cb1)>>8)
	{
		cr1=clamp255(cr1);
		cg1=clamp255(cg1);
		cb1=clamp255(cb1);
	}

	*rcr=cr1;	*rcg=cg1;	*rcb=cb1;
}

void BTIC1H_DecodeBlock_TransColor1ABGDbDr(BTIC1H_Context *ctx,
	int cya, int cua, int cva, int *rcra, int *rcga, int *rcba,
	int cyb, int cub, int cvb, int *rcrb, int *rcgb, int *rcbb)
{
	int cy1, cu1, cv1;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;

	cy1=cya; cu1=cua-128; cv1=cva-128;
	cr1=cy1+(cv1<<1);
	cg1=cy1;
	cb1=cy1+(cu1<<1);

	cy1=cyb; cu1=cub-128; cv1=cvb-128;
	cr2=cy1+(cv1<<1);
	cg2=cy1;
	cb2=cy1+(cu1<<1);

	if((cr1|cg1|cb1|cr2|cg2|cb2)>>8)
	{
		cr1=clamp255(cr1);	cg1=clamp255(cg1);
		cb1=clamp255(cb1);	cr2=clamp255(cr2);
		cg2=clamp255(cg2);	cb2=clamp255(cb2);
	}

	*rcra=cr1;	*rcga=cg1;	*rcba=cb1;
	*rcrb=cr2;	*rcgb=cg2;	*rcbb=cb2;
}

void BTIC1H_DecodeBlock_TransColor2GDbDr(BTIC1H_Context *ctx,
	int cy0, int cy1, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1)
{
	int cu1, cv1, cu2, cv2, cuv2;
	int cy0t, cy1t, cy2t, cy3t;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;

	cu1=cu-128; cv1=cv-128;

	cr0=cy0+(cv1<<1);
	cg0=cy0;
	cb0=cy0+(cu1<<1);

	cr1=cy1+(cv1<<1);
	cg1=cy1;
	cb1=cy1+(cu1<<1);

//	cu2=454*cu1;	cv2=359*cv1;	cuv2=-88*cu1-183*cv1;
//	cy0t=(cy0<<8)+128;	cy1t=(cy1<<8)+128;
//	cr0=(cy0t+cv2 )>>8;		cg0=(cy0t+cuv2)>>8;
//	cb0=(cy0t+cu2 )>>8;		cr1=(cy1t+cv2 )>>8;
//	cg1=(cy1t+cuv2)>>8;		cb1=(cy1t+cu2 )>>8;

//	if((cr0|cr1)>>8)
//	{	cr0=clamp255(cr0);	cr1=clamp255(cr1);	}
//	if((cg0|cg1)>>8)
//	{	cg0=clamp255(cg0);	cg1=clamp255(cg1);	}
//	if((cb0|cb1)>>8)
//	{	cb0=clamp255(cb0);	cb1=clamp255(cb1);	}

	if((cr0|cg0|cb0|cr1|cg1|cb1)>>8)
	{
		cr0=clamp255(cr0);	cg1=clamp255(cg0);
		cb0=clamp255(cb0);	cr2=clamp255(cr1);
		cg1=clamp255(cg1);	cb2=clamp255(cb1);
	}

	*rcr0=cr0;	*rcg0=cg0;	*rcb0=cb0;
	*rcr1=cr1;	*rcg1=cg1;	*rcb1=cb1;
}

void BTIC1H_DecodeBlock_TransColor420GDbDr(BTIC1H_Context *ctx,
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1,
	int *rcr2, int *rcg2, int *rcb2,
	int *rcr3, int *rcg3, int *rcb3)
{
	int cu1, cv1, cu2, cv2, cuv2;
	int cy0t, cy1t, cy2t, cy3t;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;

	cu1=cu-128; cv1=cv-128;

//	cu2=454*cu1;	cv2=359*cv1;	cuv2=-88*cu1-183*cv1;
//	cy0t=(cy0<<8)+128;	cy1t=(cy1<<8)+128;
//	cy2t=(cy2<<8)+128;	cy3t=(cy3<<8)+128;
//	cr0=(cy0t+cv2 )>>8;		cg0=(cy0t+cuv2)>>8;
//	cb0=(cy0t+cu2 )>>8;		cr1=(cy1t+cv2 )>>8;
//	cg1=(cy1t+cuv2)>>8;		cb1=(cy1t+cu2 )>>8;
//	cr2=(cy2t+cv2 )>>8;		cg2=(cy2t+cuv2)>>8;
//	cb2=(cy2t+cu2 )>>8;		cr3=(cy3t+cv2 )>>8;
//	cg3=(cy3t+cuv2)>>8;		cb3=(cy3t+cu2 )>>8;

	cr0=cy0+(cv1<<1);	cg0=cy0;	cb0=cy0+(cu1<<1);
	cr1=cy1+(cv1<<1);	cg1=cy1;	cb1=cy1+(cu1<<1);
	cr2=cy2+(cv1<<1);	cg2=cy2;	cb2=cy2+(cu1<<1);
	cr3=cy3+(cv1<<1);	cg3=cy3;	cb3=cy3+(cu1<<1);

	if((cr0|cr1|cr2|cr3)>>8)
	{	cr0=clamp255(cr0);	cr1=clamp255(cr1);
		cr2=clamp255(cr2);	cr3=clamp255(cr3);	}
	if((cg0|cg1|cg2|cg3)>>8)
	{	cg0=clamp255(cg0);	cg1=clamp255(cg1);
		cg2=clamp255(cg2);	cg3=clamp255(cg3);	}
	if((cb0|cb1|cb2|cb3)>>8)
	{	cb0=clamp255(cb0);	cb1=clamp255(cb1);
		cb2=clamp255(cb2);	cb3=clamp255(cb3);	}

	*rcr0=cr0;	*rcg0=cg0;	*rcb0=cb0;
	*rcr1=cr1;	*rcg1=cg1;	*rcb1=cb1;
	*rcr2=cr2;	*rcg2=cg2;	*rcb2=cb2;
	*rcr3=cr3;	*rcg3=cg3;	*rcb3=cb3;
}
#endif


void BTIC1H_DecodeBlock_TransColor1(BTIC1H_Context *ctx,
	int cy, int cu, int cv,
	int *rcr, int *rcg, int *rcb)
{
	ctx->DecTransColor1(ctx, cy, cu, cv, rcr, rcg, rcb);
}

void BTIC1H_DecodeBlock_TransColor1AB(BTIC1H_Context *ctx,
	int cya, int cua, int cva, int *rcra, int *rcga, int *rcba,
	int cyb, int cub, int cvb, int *rcrb, int *rcgb, int *rcbb)
{
	ctx->DecTransColor1AB(ctx,
		cya, cua, cva, rcra, rcga, rcba,
		cyb, cub, cvb, rcrb, rcgb, rcbb);
}

void BTIC1H_DecodeBlock_TransColor2(BTIC1H_Context *ctx,
	int cy0, int cy1, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1)
{
	ctx->DecTransColor2(ctx, cy0, cy1, cu, cv,
		rcr0, rcg0, rcb0, rcr1, rcg1, rcb1);
}

void BTIC1H_DecodeBlock_TransColor420(BTIC1H_Context *ctx,
	int cy0, int cy1, int cy2, int cy3, int cu, int cv,
	int *rcr0, int *rcg0, int *rcb0,
	int *rcr1, int *rcg1, int *rcb1,
	int *rcr2, int *rcg2, int *rcb2,
	int *rcr3, int *rcg3, int *rcb3)
{
	ctx->DecTransColor420(ctx,
		cy0, cy1, cy2, cy3, cu, cv,
		rcr0, rcg0, rcb0, rcr1, rcg1, rcb1,
		rcr2, rcg2, rcb2, rcr3, rcg3, rcb3);
}

void BTIC1H_DecodeBlockMB2B_PYUV_RGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	int cy, cu, cv, cd;
	int cy0, cy1, cy2, cy3;
	int cy0t, cy1t, cy2t, cy3t;
	int cu1, cv1, cu2, cv2, cuv2;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int bt, pxb, pxb1;
	int i, j, k, l;

	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		cu=block[ 8+(i*2+j)];
		cv=block[12+(i*2+j)];
//		cu1=cu-128; cv1=cv-128;

		k=i*8+j*2;
		cy0=block[16+k];	cy1=block[17+k];
		cy2=block[20+k];	cy3=block[21+k];

#if 0
		cu2=454*cu1;	cv2=359*cv1;	cuv2=-88*cu1-183*cv1;
		cy0t=(cy0<<8)+128;	cy1t=(cy1<<8)+128;
		cy2t=(cy2<<8)+128;	cy3t=(cy3<<8)+128;
		cr0=(cy0t+cv2 )>>8;		cg0=(cy0t+cuv2)>>8;
		cb0=(cy0t+cu2 )>>8;		cr1=(cy1t+cv2 )>>8;
		cg1=(cy1t+cuv2)>>8;		cb1=(cy1t+cu2 )>>8;
		cr2=(cy2t+cv2 )>>8;		cg2=(cy2t+cuv2)>>8;
		cb2=(cy2t+cu2 )>>8;		cr3=(cy3t+cv2 )>>8;
		cg3=(cy3t+cuv2)>>8;		cb3=(cy3t+cu2 )>>8;
#endif

		BTIC1H_DecodeBlock_TransColor420(ctx,
			cy0, cy1, cy2, cy3, cu, cv,
			&cr0, &cg0, &cb0, &cr1, &cg1, &cb1,
			&cr2, &cg2, &cb2, &cr3, &cg3, &cb3);

		if(tflip&1)
		{
			k=((i*2+0)*ystride)+((j*2+0)*xstride);
			rgba[k+0]=cb0;	rgba[k+1]=cg0;	rgba[k+2]=cr0;
			k=((i*2+0)*ystride)+((j*2+1)*xstride);
			rgba[k+0]=cb1;	rgba[k+1]=cg1;	rgba[k+2]=cr1;
			k=((i*2+1)*ystride)+((j*2+0)*xstride);
			rgba[k+0]=cb2;	rgba[k+1]=cg2;	rgba[k+2]=cr2;
			k=((i*2+1)*ystride)+((j*2+1)*xstride);
			rgba[k+0]=cb3;	rgba[k+1]=cg3;	rgba[k+2]=cr3;
		}else
		{
			k=((i*2+0)*ystride)+((j*2+0)*xstride);
			rgba[k+0]=cr0;	rgba[k+1]=cg0;	rgba[k+2]=cb0;
			k=((i*2+0)*ystride)+((j*2+1)*xstride);
			rgba[k+0]=cr1;	rgba[k+1]=cg1;	rgba[k+2]=cb1;
			k=((i*2+1)*ystride)+((j*2+0)*xstride);
			rgba[k+0]=cr2;	rgba[k+1]=cg2;	rgba[k+2]=cb2;
			k=((i*2+1)*ystride)+((j*2+1)*xstride);
			rgba[k+0]=cr3;	rgba[k+1]=cg3;	rgba[k+2]=cb3;
		}
	}
}

void BTIC1H_DecodeBlockMB2B_P420_RGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	byte clr[4][4*4];
	int clri[4][4];
	byte *ct;
	int cy0, cu0, cv0, cd0, cya0, cyb0;
	int cy1, cu1, cv1, cd1, cya1, cyb1;
	int cy2, cu2, cv2, cd2, cya2, cyb2;
	int cy3, cu3, cv3, cd3, cya3, cyb3;
	int cra0, cga0, cba0, crb0, cgb0, cbb0;
	int cra1, cga1, cba1, crb1, cgb1, cbb1;
	int cra2, cga2, cba2, crb2, cgb2, cbb2;
	int cra3, cga3, cba3, crb3, cgb3, cbb3;
	int cyat, cybt, cu0t, cv0t, cu1t, cv1t, cuvt;
	int ci, pxb;
	int i, j, k, l;
	
	cy0=block[ 0];	cu0=block[ 1];
	cv0=block[ 2];	cd0=block[ 5];
	cy1=block[ 8];	cu1=block[ 9];
	cv1=block[10];	cd1=block[11];
	cy2=block[16];	cu2=block[17];
	cv2=block[18];	cd2=block[19];
	cy3=block[20];	cu3=block[21];
	cv3=block[22];	cd3=block[23];

	cya0=cy0-(cd0>>1);	cyb0=cya0+cd0;
	cya1=cy1-(cd1>>1);	cyb1=cya1+cd1;
	cya2=cy2-(cd2>>1);	cyb2=cya2+cd2;
	cya3=cy3-(cd3>>1);	cyb3=cya3+cd3;

//	cyat=(cya0<<8)+128;		cybt=(cyb0<<8)+128;
//	cu0t=cu0-128;			cv0t=cv0-128;
//	cu1t=454*cu0t;			cv1t=359*cv0t;
//	cuvt=0-88*cu0t-183*cv0t;
//	cra0=(cyat+cv1t)>>8;	cga0=(cyat+cuvt)>>8;
//	cba0=(cyat+cu1t)>>8;	crb0=(cybt+cv1t)>>8;
//	cgb0=(cybt+cuvt)>>8;	cbb0=(cybt+cu1t)>>8;

//	cyat=(cya1<<8)+128;		cybt=(cyb1<<8)+128;
//	cu0t=cu1-128;			cv0t=cv1-128;
//	cu1t=454*cu0t;			cv1t=359*cv0t;
//	cuvt=0-88*cu0t-183*cv0t;
//	cra1=(cyat+cv1t)>>8;	cga1=(cyat+cuvt)>>8;
//	cba1=(cyat+cu1t)>>8;	crb1=(cybt+cv1t)>>8;
//	cgb1=(cybt+cuvt)>>8;	cbb1=(cybt+cu1t)>>8;

//	cyat=(cya2<<8)+128;		cybt=(cyb2<<8)+128;
//	cu0t=cu2-128;			cv0t=cv2-128;
//	cu1t=454*cu0t;			cv1t=359*cv0t;
//	cuvt=0-88*cu0t-183*cv0t;
//	cra2=(cyat+cv1t)>>8;	cga2=(cyat+cuvt)>>8;
//	cba2=(cyat+cu1t)>>8;	crb2=(cybt+cv1t)>>8;
//	cgb2=(cybt+cuvt)>>8;	cbb2=(cybt+cu1t)>>8;

//	cyat=(cya3<<8)+128;		cybt=(cyb3<<8)+128;
//	cu0t=cu3-128;			cv0t=cv3-128;
//	cu1t=454*cu0t;			cv1t=359*cv0t;
//	cuvt=0-88*cu0t-183*cv0t;
//	cra3=(cyat+cv1t)>>8;	cga3=(cyat+cuvt)>>8;
//	cba3=(cyat+cu1t)>>8;	crb3=(cybt+cv1t)>>8;
//	cgb3=(cybt+cuvt)>>8;	cbb3=(cybt+cu1t)>>8;
	
//	if((cra0|cga0|cba0|crb0|cgb0|cbb0)>>8)
//	{	cra0=clamp255(cra0);	cga0=clamp255(cga0);
//		cba0=clamp255(cba0);	crb0=clamp255(crb0);
//		cgb0=clamp255(cgb0);	cbb0=clamp255(cbb0);	}
//	if((cra1|cga1|cba1|crb1|cgb1|cbb1)>>8)
//	{	cra1=clamp255(cra1);	cga1=clamp255(cga1);
//		cba1=clamp255(cba1);	crb1=clamp255(crb1);
//		cgb1=clamp255(cgb1);	cbb1=clamp255(cbb1);	}
//	if((cra2|cga2|cba2|crb2|cgb2|cbb2)>>8)
//	{	cra2=clamp255(cra2);	cga2=clamp255(cga2);
//		cba2=clamp255(cba2);	crb2=clamp255(crb2);
//		cgb2=clamp255(cgb2);	cbb2=clamp255(cbb2);	}
//	if((cra3|cga3|cba3|crb3|cgb3|cbb3)>>8)
//	{	cra3=clamp255(cra3);	cga3=clamp255(cga3);
//		cba3=clamp255(cba3);	crb3=clamp255(crb3);
//		cgb3=clamp255(cgb3);	cbb3=clamp255(cbb3);	}

	BTIC1H_DecodeBlock_TransColor2(ctx,
		cya0, cyb0, cu0, cv0,
		&cra0, &cga0, &cba0,
		&crb0, &cgb0, &cbb0);
	BTIC1H_DecodeBlock_TransColor2(ctx,
		cya1, cyb1, cu1, cv1,
		&cra1, &cga1, &cba1,
		&crb1, &cgb1, &cbb1);
	BTIC1H_DecodeBlock_TransColor2(ctx,
		cya2, cyb2, cu2, cv2,
		&cra2, &cga2, &cba2,
		&crb2, &cgb2, &cbb2);
	BTIC1H_DecodeBlock_TransColor2(ctx,
		cya3, cyb3, cu3, cv3,
		&cra3, &cga3, &cba3,
		&crb3, &cgb3, &cbb3);

	if(tflip&1)
	{
		clr[0][ 0]=cba0; clr[0][ 1]=cga0; clr[0][ 2]=cra0; clr[0][ 3]=255;
		clr[0][12]=cbb0; clr[0][13]=cgb0; clr[0][14]=crb0; clr[0][15]=255;
		clr[1][ 0]=cba1; clr[1][ 1]=cga1; clr[1][ 2]=cra1; clr[1][ 3]=255;
		clr[1][12]=cbb1; clr[1][13]=cgb1; clr[1][14]=crb1; clr[1][15]=255;
		clr[2][ 0]=cba2; clr[2][ 1]=cga2; clr[2][ 2]=cra2; clr[2][ 3]=255;
		clr[2][12]=cbb2; clr[2][13]=cgb2; clr[2][14]=crb2; clr[2][15]=255;
		clr[3][ 0]=cba3; clr[3][ 1]=cga3; clr[3][ 2]=cra3; clr[3][ 3]=255;
		clr[3][12]=cbb3; clr[3][13]=cgb3; clr[3][14]=crb3; clr[3][15]=255;
	}else
	{
		clr[0][ 0]=cra0; clr[0][ 1]=cga0; clr[0][ 2]=cba0; clr[0][ 3]=255;
		clr[0][12]=crb0; clr[0][13]=cgb0; clr[0][14]=cbb0; clr[0][15]=255;
		clr[1][ 0]=cra1; clr[1][ 1]=cga1; clr[1][ 2]=cba1; clr[1][ 3]=255;
		clr[1][12]=crb1; clr[1][13]=cgb1; clr[1][14]=cbb1; clr[1][15]=255;
		clr[2][ 0]=cra2; clr[2][ 1]=cga2; clr[2][ 2]=cba2; clr[2][ 3]=255;
		clr[2][12]=crb2; clr[2][13]=cgb2; clr[2][14]=cbb2; clr[2][15]=255;
		clr[3][ 0]=cra3; clr[3][ 1]=cga3; clr[3][ 2]=cba3; clr[3][ 3]=255;
		clr[3][12]=crb3; clr[3][13]=cgb3; clr[3][14]=cbb3; clr[3][15]=255;
	}

	for(i=0; i<4; i++)
	{
		clr[i][ 4]=(clr[i][0]*11+clr[i][12]*5)>>4;
		clr[i][ 5]=(clr[i][1]*11+clr[i][13]*5)>>4;
		clr[i][ 6]=(clr[i][2]*11+clr[i][14]*5)>>4;
		clr[i][ 7]=255;
		clr[i][ 8]=(clr[i][0]*5+clr[i][12]*11)>>4;
		clr[i][ 9]=(clr[i][1]*5+clr[i][13]*11)>>4;
		clr[i][10]=(clr[i][2]*5+clr[i][14]*11)>>4;
		clr[i][11]=255;
	
		clri[i][0]=*(u32 *)(clr[i]+ 0);
		clri[i][1]=*(u32 *)(clr[i]+ 4);
		clri[i][2]=*(u32 *)(clr[i]+ 8);
		clri[i][3]=*(u32 *)(clr[i]+12);
	}

	if(xstride==4)
	{
		ct=rgba;			pxb=block[12];
		*(u32 *)(ct+ 0)=clri[0][(pxb>>6)&3];
		*(u32 *)(ct+ 4)=clri[0][(pxb>>4)&3];
		*(u32 *)(ct+ 8)=clri[1][(pxb>>2)&3];
		*(u32 *)(ct+12)=clri[1][(pxb   )&3];
		ct=ct+ystride;		pxb=block[13];
		*(u32 *)(ct+ 0)=clri[0][(pxb>>6)&3];
		*(u32 *)(ct+ 4)=clri[0][(pxb>>4)&3];
		*(u32 *)(ct+ 8)=clri[1][(pxb>>2)&3];
		*(u32 *)(ct+12)=clri[1][(pxb   )&3];
		ct=ct+ystride;		pxb=block[14];
		*(u32 *)(ct+ 0)=clri[2][(pxb>>6)&3];
		*(u32 *)(ct+ 4)=clri[2][(pxb>>4)&3];
		*(u32 *)(ct+ 8)=clri[3][(pxb>>2)&3];
		*(u32 *)(ct+12)=clri[3][(pxb   )&3];
		ct=ct+ystride;		pxb=block[15];
		*(u32 *)(ct+ 0)=clri[2][(pxb>>6)&3];
		*(u32 *)(ct+ 4)=clri[2][(pxb>>4)&3];
		*(u32 *)(ct+ 8)=clri[3][(pxb>>2)&3];
		*(u32 *)(ct+12)=clri[3][(pxb   )&3];
		return;
	}
	
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		ci=(i&2)|(j>>1);
		k=i*ystride+j*xstride;
		l=((block[12+i]>>(6-2*j))&3)*4;
		rgba[k+0]=clr[ci][l+0];
		rgba[k+1]=clr[ci][l+1];
		rgba[k+2]=clr[ci][l+2];
	}
}

void BTIC1H_DecodeBlockMB2B_I4XX_RGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
//	byte clr[8*4];
//	u32 clri[8];
//	byte *clr;

	byte clry[8];
	byte clru[4];
	byte clrv[4];

	u64 lpxb;
	byte *ct;
	int cy, cu, cv, cd, cdu, cdv;
	int cya, cyb, cua, cub, cva, cvb;
	int cy0, cy1, cy2, cy3;
	int cu1, cv1;
//	int cy1, cu1, cv1;
	int cr, cg, cb, ca;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int bt, pxb, pxb1, pxb2;
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int i, j, k, l;
	
//	return;
	
//	clr=(byte *)clri;
	i=block[3];
	cd=i; bt=0;
	cdu=0; cdv=0;
	
	cy=block[0];
	cu=block[1];
	cv=block[2];
//	cd=block[3];
	bt=block[4];
	cd=((block[5]-128)<<1);
	cdu=((block[8]-128)<<1);
	cdv=((block[9]-128)<<1);

	cya=cy-(cd>>1);		cyb=cya+cd;
	cua=cu-(cdu>>1);	cub=cua+cdu;
	cva=cv-(cdv>>1);	cvb=cva+cdv;
	
	if((cya|cyb|cua|cub|cva|cvb)>>8)
	{
		cya=clamp255(cya);
		cyb=clamp255(cyb);
		cua=clamp255(cua);
		cub=clamp255(cub);
		cva=clamp255(cva);
		cvb=clamp255(cvb);
	}
	
	if((bt==21) || (bt==22))
	{
		clry[0]=cya;	clry[7]=cyb;
		clru[0]=cua;	clrv[0]=cva;
		clru[3]=cub;	clrv[3]=cvb;

		clry[1]=(clry[0]*14+clry[7]* 2)>>4;
		clry[2]=(clry[0]*11+clry[7]* 5)>>4;
		clry[3]=(clry[0]* 9+clry[7]* 7)>>4;
		clry[4]=(clry[0]* 7+clry[7]* 9)>>4;
		clry[5]=(clry[0]* 5+clry[7]*11)>>4;
		clry[6]=(clry[0]* 2+clry[7]*14)>>4;

		clru[1]=(clru[0]*11+clru[3]* 5)>>4;
		clrv[1]=(clrv[0]*11+clrv[3]* 5)>>4;
		clru[2]=(clru[0]* 5+clru[3]*11)>>4;
		clrv[2]=(clrv[0]* 5+clrv[3]*11)>>4;
	}
	else
	{
		clry[0]=cya;	clru[0]=cua;	clrv[0]=cva;
		clry[3]=cyb;	clru[3]=cub;	clrv[3]=cvb;

		clry[1]=(clry[0]*11+clry[3]* 5)>>4;
		clru[1]=(clru[0]*11+clru[3]* 5)>>4;
		clrv[1]=(clrv[0]*11+clrv[3]* 5)>>4;

		clry[2]=(clry[0]* 5+clry[3]*11)>>4;
		clru[2]=(clru[0]* 5+clru[3]*11)>>4;
		clrv[2]=(clrv[0]* 5+clrv[3]*11)>>4;
	}
			
	if(bt==20)
	{
		pxb=block[10];
		pxb1=block[11];

#if 1
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			i0=(block[12+(2*i+0)]>>(6-2*(2*j+0)))&3;
			i1=(block[12+(2*i+0)]>>(6-2*(2*j+1)))&3;
			i2=(block[12+(2*i+1)]>>(6-2*(2*j+0)))&3;
			i3=(block[12+(2*i+1)]>>(6-2*(2*j+1)))&3;

			l=2*i+j;
			i4=(pxb>>(6-2*l))&3;
			i5=(pxb1>>(6-2*l))&3;

			cy0=clry[i0];	cy1=clry[i1];
			cy2=clry[i2];	cy3=clry[i3];
			cu =clru[i4];	cv =clrv[i5];
			
//			cu1=cu-128; cv1=cv-128;

//			cr=0        +359*cv1+128;
//			cg=0- 88*cu1-183*cv1+128;
//			cb=0+454*cu1        +128;

//			ca=cy0<<8; cr0=(ca+cr)>>8; cg0=(ca+cg)>>8; cb0=(ca+cb)>>8;
//			ca=cy1<<8; cr1=(ca+cr)>>8; cg1=(ca+cg)>>8; cb1=(ca+cb)>>8;
//			ca=cy2<<8; cr2=(ca+cr)>>8; cg2=(ca+cg)>>8; cb2=(ca+cb)>>8;
//			ca=cy3<<8; cr3=(ca+cr)>>8; cg3=(ca+cg)>>8; cb3=(ca+cb)>>8;

			BTIC1H_DecodeBlock_TransColor420(ctx,
				cy0, cy1, cy2, cy3, cu, cv,
				&cr0, &cg0, &cb0, &cr1, &cg1, &cb1,
				&cr2, &cg2, &cb2, &cr3, &cg3, &cb3);

//			k=i*ystride+j*xstride;

//			if((cr0|cr1|cr2|cr3)>>8)
//			{	cr0=clamp255(cr0);	cr1=clamp255(cr1);
//				cr2=clamp255(cr2);	cr3=clamp255(cr3);	}
//			if((cg0|cg1|cg2|cg3)>>8)
//			{	cg0=clamp255(cg0);	cg1=clamp255(cg1);
//				cg2=clamp255(cg2);	cg3=clamp255(cg3);	}
//			if((cb0|cb1|cb2|cb3)>>8)
//			{	cb0=clamp255(cb0);	cb1=clamp255(cb1);
//				cb2=clamp255(cb2);	cb3=clamp255(cb3);	}

			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cb0;	rgba[i0+1]=cg0;	rgba[i0+2]=cr0;
				rgba[i1+0]=cb1;	rgba[i1+1]=cg1;	rgba[i1+2]=cr1;
				rgba[i2+0]=cb2;	rgba[i2+1]=cg2;	rgba[i2+2]=cr2;
				rgba[i3+0]=cb3;	rgba[i3+1]=cg3;	rgba[i3+2]=cr3;
			}else
			{
				rgba[i0+0]=cr0;	rgba[i0+1]=cg0;	rgba[i0+2]=cb0;
				rgba[i1+0]=cr1;	rgba[i1+1]=cg1;	rgba[i1+2]=cb1;
				rgba[i2+0]=cr2;	rgba[i2+1]=cg2;	rgba[i2+2]=cb2;
				rgba[i3+0]=cr3;	rgba[i3+1]=cg3;	rgba[i3+2]=cb3;
			}
		}
		return;
#endif

#if 0
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			i0=(block[12+i]>>(6-2*j))&3;

			l=2*(i>>1)+(j>>1);
			i1=(pxb>>(6-2*l))&3;
			i2=(pxb1>>(6-2*l))&3;

			cy=clry[i0];
			cu=clru[i1];
			cv=clrv[i2];
			
			cy1=cy; cu1=cu-128; cv1=cv-128;
			cr1=(256*cy1        +359*cv1+128)>>8;
			cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
			cb1=(256*cy1+454*cu1        +128)>>8;

			if(tflip&1)
			{
				rgba[k+0]=clamp255(cb1);
				rgba[k+1]=clamp255(cg1);
				rgba[k+2]=clamp255(cr1);
			}else
			{
				rgba[k+0]=clamp255(cr1);
				rgba[k+1]=clamp255(cg1);
				rgba[k+2]=clamp255(cb1);
			}
		}
		return;
#endif
	}

	if(bt==21)
	{	
		i=(block[10]<<8)|block[11];
		j=(block[12]<<8)|block[13];
		k=(block[14]<<8)|block[15];
		lpxb=(((u64)i)<<32)|(((u64)j)<<16)|k;

		pxb=(block[16]<<24)|(block[17]<<16)|(block[18]<<8)|(block[19]);
		pxb1=(block[20]<<24)|(block[21]<<16)|(block[22]<<8)|(block[23]);
	
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			i0=(lpxb>>(45-3*(i*4+j)))&7;
			i1=(pxb>>(30-2*(i*4+j)))&3;
			i2=(pxb1>>(30-2*(i*4+j)))&3;

			cy=clry[i0];
			cu=clru[i1];
			cv=clrv[i2];
			
//			cy1=cy; cu1=cu-128; cv1=cv-128;
//			cr1=(256*cy1        +359*cv1+128)>>8;
//			cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
//			cb1=(256*cy1+454*cu1        +128)>>8;
			BTIC1H_DecodeBlock_TransColor1(
				ctx, cy, cu, cv, &cr1, &cg1, &cb1);

			if(tflip&1)
				{ rgba[k+0]=cb1; rgba[k+1]=cg1; rgba[k+2]=cr1; }
			else
				{ rgba[k+0]=cr1; rgba[k+1]=cg1; rgba[k+2]=cb1; }
		}
		return;
	}

	if(bt==22)
	{	
		i=(block[10]<<8)|block[11];
		j=(block[12]<<8)|block[13];
		k=(block[14]<<8)|block[15];
		lpxb=(((u64)i)<<32)|(((u64)j)<<16)|k;

		pxb=block[6];
		pxb1=block[7];
	
#if 1
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
//			i0=(pxb2>>(45-3*((i*2+0)*4+(j*2+0))))&7;
//			i1=(pxb2>>(45-3*((i*2+0)*4+(j*2+1))))&7;
//			i2=(pxb2>>(45-3*((i*2+1)*4+(j*2+0))))&7;
//			i3=(pxb2>>(45-3*((i*2+1)*4+(j*2+1))))&7;

			i4=(i*2+0)*4;	i5=(i*2+1)*4;
			i6=(j*2+0);		i7=(j*2+1);
			i0=(lpxb>>(45-3*(i4+i6)))&7;
			i1=(lpxb>>(45-3*(i4+i7)))&7;
			i2=(lpxb>>(45-3*(i5+i6)))&7;
			i3=(lpxb>>(45-3*(i5+i7)))&7;

			l=2*i+j;
			i4=(pxb>>(6-2*l))&3;
			i5=(pxb1>>(6-2*l))&3;

			cy0=clry[i0];	cy1=clry[i1];
			cy2=clry[i2];	cy3=clry[i3];
			cu =clru[i4];	cv =clrv[i5];
			
//			cu1=cu-128; cv1=cv-128;

//			cr=0        +359*cv1+128;
//			cg=0- 88*cu1-183*cv1+128;
//			cb=0+454*cu1        +128;

//			ca=cy0<<8; cr0=(ca+cr)>>8; cg0=(ca+cg)>>8; cb0=(ca+cb)>>8;
//			ca=cy1<<8; cr1=(ca+cr)>>8; cg1=(ca+cg)>>8; cb1=(ca+cb)>>8;
//			ca=cy2<<8; cr2=(ca+cr)>>8; cg2=(ca+cg)>>8; cb2=(ca+cb)>>8;
//			ca=cy3<<8; cr3=(ca+cr)>>8; cg3=(ca+cg)>>8; cb3=(ca+cb)>>8;

			BTIC1H_DecodeBlock_TransColor420(ctx,
				cy0, cy1, cy2, cy3, cu, cv,
				&cr0, &cg0, &cb0, &cr1, &cg1, &cb1,
				&cr2, &cg2, &cb2, &cr3, &cg3, &cb3);

//			k=i*ystride+j*xstride;

//			if((cr0|cr1|cr2|cr3)>>8)
//			{	cr0=clamp255(cr0);	cr1=clamp255(cr1);
//				cr2=clamp255(cr2);	cr3=clamp255(cr3);	}
//			if((cg0|cg1|cg2|cg3)>>8)
//			{	cg0=clamp255(cg0);	cg1=clamp255(cg1);
//				cg2=clamp255(cg2);	cg3=clamp255(cg3);	}
//			if((cb0|cb1|cb2|cb3)>>8)
//			{	cb0=clamp255(cb0);	cb1=clamp255(cb1);
//				cb2=clamp255(cb2);	cb3=clamp255(cb3);	}

			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cb0;	rgba[i0+1]=cg0;	rgba[i0+2]=cr0;
				rgba[i1+0]=cb1;	rgba[i1+1]=cg1;	rgba[i1+2]=cr1;
				rgba[i2+0]=cb2;	rgba[i2+1]=cg2;	rgba[i2+2]=cr2;
				rgba[i3+0]=cb3;	rgba[i3+1]=cg3;	rgba[i3+2]=cr3;
			}else
			{
				rgba[i0+0]=cr0;	rgba[i0+1]=cg0;	rgba[i0+2]=cb0;
				rgba[i1+0]=cr1;	rgba[i1+1]=cg1;	rgba[i1+2]=cb1;
				rgba[i2+0]=cr2;	rgba[i2+1]=cg2;	rgba[i2+2]=cb2;
				rgba[i3+0]=cr3;	rgba[i3+1]=cg3;	rgba[i3+2]=cb3;
			}
		}
		return;
#endif

#if 0
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			i0=(pxb2>>(45-3*(i*4+j)))&7;

			l=2*(i>>1)+(j>>1);
			i1=(pxb>>(6-2*l))&3;
			i2=(pxb1>>(6-2*l))&3;

			cy=clry[i0];
			cu=clru[i1];
			cv=clrv[i2];
			
			cy1=cy; cu1=cu-128; cv1=cv-128;
			cr1=(256*cy1        +359*cv1+128)>>8;
			cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
			cb1=(256*cy1+454*cu1        +128)>>8;

			if(tflip&1)
			{
				rgba[k+0]=clamp255(cb1);
				rgba[k+1]=clamp255(cg1);
				rgba[k+2]=clamp255(cr1);
			}else
			{
				rgba[k+0]=clamp255(cr1);
				rgba[k+1]=clamp255(cg1);
				rgba[k+2]=clamp255(cb1);
			}
		}
		return;
#endif
	}


	if(bt==23)
	{
		pxb =block[ 6];
		pxb1=block[10];
		pxb2=block[11];

#if 1
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			l=2*i+j;
			i0=(pxb >>(6-2*l))&3;
			i1=(pxb1>>(6-2*l))&3;
			i2=(pxb2>>(6-2*l))&3;

			cy=clry[i0];
			cu=clru[i1];
			cv=clrv[i2];
			
//			cu1=cu-128; cv1=cv-128;
//			cr=0        +359*cv1+128;
//			cg=0- 88*cu1-183*cv1+128;
//			cb=0+454*cu1        +128;

			BTIC1H_DecodeBlock_TransColor1(
				ctx, cy, cu, cv, &cr0, &cg0, &cb0);

//			ca=cy<<8; cr0=(ca+cr)>>8; cg0=(ca+cg)>>8; cb0=(ca+cb)>>8;

//			if((cr0|cg0|cb0)>>8)
//			{	cr0=clamp255(cr0);	cg0=clamp255(cg0);
//				cb0=clamp255(cb0);	}

			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cb0;	rgba[i0+1]=cg0;	rgba[i0+2]=cr0;
				rgba[i1+0]=cb0;	rgba[i1+1]=cg0;	rgba[i1+2]=cr0;
				rgba[i2+0]=cb0;	rgba[i2+1]=cg0;	rgba[i2+2]=cr0;
				rgba[i3+0]=cb0;	rgba[i3+1]=cg0;	rgba[i3+2]=cr0;
			}else
			{
				rgba[i0+0]=cr0;	rgba[i0+1]=cg0;	rgba[i0+2]=cb0;
				rgba[i1+0]=cr0;	rgba[i1+1]=cg0;	rgba[i1+2]=cb0;
				rgba[i2+0]=cr0;	rgba[i2+1]=cg0;	rgba[i2+2]=cb0;
				rgba[i3+0]=cr0;	rgba[i3+1]=cg0;	rgba[i3+2]=cb0;
			}
		}
		return;
#endif
	}
}

void BTIC1H_DecodeBlockMB2B_RGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
//	byte clr[8*4];
	u32 clri[8];
	byte *clr;
	u64 pxb2;
	byte *ct;
	int cy, cu, cv, cd, cdu, cdv;
	int cya, cyb, cua, cub, cva, cvb;
	int cy1, cu1, cv1;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int bt, pxb, pxb1;
	int i0, i1, i2, i3;
	int i, j, k, l;

//	return;

#if 0
	i=block[3];

	switch(block[5])
	{
	case 0:
	
	case 1: case 2:
	case 6:
	
	case 8: case 9:
	case 10: case 11:
	
	case 7: case 17:
	case 12: case 13:
	case 14: case 15:
	case 18: case 19:
	case 20:
	case 21:	case 22:
		j=1; break;
	default: j=0; break;
	}
	if((i==0) && j)
//	if(1)
//	if(i!=0)
	{
		i0=255;	i1=0;
		i2=255;	i3=0;
	
		ct=rgba;
		ct[ 0]=i0; ct[ 1]=i2;  ct[ 2]=i1; ct[ 3]=i3;
		ct[ 4]=i1; ct[ 5]=i3;  ct[ 6]=i0; ct[ 7]=i2;
		ct[ 8]=i0; ct[ 9]=i2;  ct[10]=i1; ct[11]=i3;
		ct[12]=i1; ct[13]=i3;  ct[14]=i0; ct[15]=i2;
		ct=ct+ystride;
		ct[ 0]=i0; ct[ 1]=i2;  ct[ 2]=i1; ct[ 3]=i3;
		ct[ 4]=i1; ct[ 5]=i3;  ct[ 6]=i0; ct[ 7]=i2;
		ct[ 8]=i0; ct[ 9]=i2;  ct[10]=i1; ct[11]=i3;
		ct[12]=i1; ct[13]=i3;  ct[14]=i0; ct[15]=i2;
		ct=ct+ystride;
		ct[ 0]=i0; ct[ 1]=i2;  ct[ 2]=i1; ct[ 3]=i3;
		ct[ 4]=i1; ct[ 5]=i3;  ct[ 6]=i0; ct[ 7]=i2;
		ct[ 8]=i0; ct[ 9]=i2;  ct[10]=i1; ct[11]=i3;
		ct[12]=i1; ct[13]=i3;  ct[14]=i0; ct[15]=i2;
		ct=ct+ystride;
		ct[ 0]=i0; ct[ 1]=i2;  ct[ 2]=i1; ct[ 3]=i3;
		ct[ 4]=i1; ct[ 5]=i3;  ct[ 6]=i0; ct[ 7]=i2;
		ct[ 8]=i0; ct[ 9]=i2;  ct[10]=i1; ct[11]=i3;
		ct[12]=i1; ct[13]=i3;  ct[14]=i0; ct[15]=i2;
		return;
	}
#endif
	
	clr=(byte *)clri;
	i=block[3];
	cd=i; bt=0;
	cdu=0; cdv=0;
	
	if(i==0)
	{
		if(block[4]==0)
		{
			cy=block[0];
			cu=block[1];
			cv=block[2];

//			cu1=cu-128; cv1=cv-128;
//			cr=65536*cy           +91881*cv1;
//			cg=65536*cy- 22554*cu1-46802*cv1;
//			cb=65536*cy+116130*cu1;
//			cr>>=16; cg>>=16; cb>>=16;
//			cr=(cr+32768)>>16;
//			cg=(cg+32768)>>16;
//			cb=(cb+32768)>>16;

//			cr=(256*cy        +359*cv1+128)>>8;
//			cg=(256*cy- 88*cu1-183*cv1+128)>>8;
//			cb=(256*cy+454*cu1        +128)>>8;

			BTIC1H_DecodeBlock_TransColor1(
				ctx, cy, cu, cv, &cr, &cg, &cb);

			if(tflip&1)
			{
				clr[0]=cb;	clr[1]=cg;
				clr[2]=cr;	clr[3]=255;
			}else
			{
				clr[0]=cr;	clr[1]=cg;
				clr[2]=cb;	clr[3]=255;
			}

			if(xstride==4)
			{
				l=*(u32 *)(clr+0);

#if 0
				if(tflip&16)
				{
					ct=rgba+ystride;
					i1=*(u32 *)(ct-4);
					ct=rgba-ystride;
					i2=*(u32 *)(ct+4);
				}else
				{
					i1=l; i2=l;
				}
#endif

#ifdef X86_64
				pxb2=(((u64)l)<<32)|((u32)l);

				ct=rgba;
				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				ct=ct+ystride;
				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				ct=ct+ystride;
				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				ct=ct+ystride;
				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
#else
//				pxb2=(((u64)l)<<32)|l;
#if 1		
				ct=rgba;
//				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
				ct=ct+ystride;
//				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
				ct=ct+ystride;
//				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
				ct=ct+ystride;
//				*(u64 *)(ct+ 0)=pxb2;	*(u64 *)(ct+ 8)=pxb2;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
#endif
#endif

#if 0	
				ct=rgba;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=i2;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=i2;
				ct=ct+ystride;
				*(u32 *)(ct+ 0)=i1;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
				ct=ct+ystride;
				*(u32 *)(ct+ 0)=l;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
				ct=ct+ystride;
				*(u32 *)(ct+ 0)=i1;	*(u32 *)(ct+ 4)=l;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
#endif

				return;
			}

			if(xstride==3)
			{
				for(i=0; i<4; i++)
				{
					ct=rgba+(i*ystride);
					if(tflip&1)
					{
						ct[0]=cb; ct[ 1]=cg; ct[ 2]=cr;
						ct[3]=cb; ct[ 4]=cg; ct[ 5]=cr;
						ct[6]=cb; ct[ 7]=cg; ct[ 8]=cr;
						ct[9]=cb; ct[10]=cg; ct[11]=cr;
					}else
					{
						ct[0]=cr; ct[ 1]=cg; ct[ 2]=cb;
						ct[3]=cr; ct[ 4]=cg; ct[ 5]=cb;
						ct[6]=cr; ct[ 7]=cg; ct[ 8]=cb;
						ct[9]=cr; ct[10]=cg; ct[11]=cb;
					}
				}
				return;
			}

#if 1
			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
			{
				k=i*ystride+j*xstride;
				if(tflip&1)
				{
					rgba[k+0]=cb;
					rgba[k+1]=cg;
					rgba[k+2]=cr;
				}else
				{
					rgba[k+0]=cr;
					rgba[k+1]=cg;
					rgba[k+2]=cb;
				}
			}
#endif
			return;
		}	

		if(block[4]==1)
			{ cd=block[5]; bt=1; }
		else if(block[4]==2)
			{ cd=block[5]; bt=2; }
		else if(block[4]==6)
			{ cd=block[5]; bt=6; }
		else if(block[4]==17)
		{
			BTIC1H_DecodeBlockMB2B_P420_RGBI(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}else if(block[4]==7)
		{
			BTIC1H_DecodeBlockMB2B_PYUV_RGBI(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}else if((block[4]==14) || (block[4]==15) ||
			(block[4]==19))
		{
			bt=block[4];
			cd=((block[5]-128)<<1);
			cdu=((block[8]-128)<<1);
			cdv=((block[9]-128)<<1);
			
			if(bt==15)bt=6;
			if(bt==19)bt=1;
		}else if((block[4]==20) || (block[4]==21) ||
			(block[4]==22) || (block[4]==23))
		{
			BTIC1H_DecodeBlockMB2B_I4XX_RGBI(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}
		else
		{
		}
	}

	cy=block[0];
	cu=block[1];
	cv=block[2];
//	cd=block[3];

	cya=cy-(cd>>1);		cyb=cya+cd;
	cua=cu-(cdu>>1);	cub=cua+cdu;
	cva=cv-(cdv>>1);	cvb=cva+cdv;
	
//	cua=cu;	cva=cv;
//	cub=cu;	cvb=cv;

//	if((bt!=20) && (bt!=21))
	if(1)
	{
//		cy1=cya; cu1=cua-128; cv1=cva-128;
//		cr1=(256*cy1        +359*cv1+128)>>8;
//		cg1=(256*cy1- 88*cu1-183*cv1+128)>>8;
//		cb1=(256*cy1+454*cu1        +128)>>8;

//		cy1=cyb; cu1=cub-128; cv1=cvb-128;
//		cr2=(256*cy1        +359*cv1+128)>>8;
//		cg2=(256*cy1- 88*cu1-183*cv1+128)>>8;
//		cb2=(256*cy1+454*cu1        +128)>>8;

		BTIC1H_DecodeBlock_TransColor1AB(ctx,
			cya, cua, cva, &cr1, &cg1, &cb1,
			cyb, cub, cvb, &cr2, &cg2, &cb2);
//		BTIC1H_DecodeBlock_TransColor1(
//			ctx, cyb, cub, cvb, &cr2, &cg2, &cb2);

#if 0
		if((cr1|cg1|cb1|cr2|cg2|cb2)>>8)
		{
			cr1=clamp255(cr1);
			cg1=clamp255(cg1);
			cb1=clamp255(cb1);

			cr2=clamp255(cr2);
			cg2=clamp255(cg2);
			cb2=clamp255(cb2);
		}
#endif
	}

	if(bt==6)
	{
		if(tflip&1)
		{
			clr[ 0]=cb1;	clr[ 1]=cg1;
			clr[ 2]=cr1;	clr[ 3]=255;
			clr[28]=cb2;	clr[29]=cg2;
			clr[30]=cr2;	clr[31]=255;
		}else
		{
			clr[ 0]=cr1;	clr[ 1]=cg1;
			clr[ 2]=cb1;	clr[ 3]=255;
			clr[28]=cr2;	clr[29]=cg2;
			clr[30]=cb2;	clr[31]=255;
		}

		clr[ 4]=(clr[0]*14+clr[28]*2)>>4;
		clr[ 5]=(clr[1]*14+clr[29]*2)>>4;
		clr[ 6]=(clr[2]*14+clr[30]*2)>>4;
		clr[ 7]=255;
		clr[ 8]=(clr[0]*11+clr[28]*5)>>4;
		clr[ 9]=(clr[1]*11+clr[29]*5)>>4;
		clr[10]=(clr[2]*11+clr[30]*5)>>4;
		clr[11]=255;
		clr[12]=(clr[0]* 9+clr[28]*7)>>4;
		clr[13]=(clr[1]* 9+clr[29]*7)>>4;
		clr[14]=(clr[2]* 9+clr[30]*7)>>4;
		clr[15]=255;
		clr[16]=(clr[0]* 7+clr[28]*9)>>4;
		clr[17]=(clr[1]* 7+clr[29]*9)>>4;
		clr[18]=(clr[2]* 7+clr[30]*9)>>4;
		clr[19]=255;
		clr[20]=(clr[0]* 5+clr[28]*11)>>4;
		clr[21]=(clr[1]* 5+clr[29]*11)>>4;
		clr[22]=(clr[2]* 5+clr[30]*11)>>4;
		clr[23]=255;
		clr[24]=(clr[0]* 2+clr[28]*14)>>4;
		clr[25]=(clr[1]* 2+clr[29]*14)>>4;
		clr[26]=(clr[2]* 2+clr[30]*14)>>4;
		clr[27]=255;
	}else
	{
		if(tflip&1)
		{
			clr[ 0]=cb1;	clr[ 1]=cg1;
			clr[ 2]=cr1;	clr[ 3]=255;
			clr[12]=cb2;	clr[13]=cg2;
			clr[14]=cr2;	clr[15]=255;
		}else
		{
			clr[ 0]=cr1;	clr[ 1]=cg1;
			clr[ 2]=cb1;	clr[ 3]=255;
			clr[12]=cr2;	clr[13]=cg2;
			clr[14]=cb2;	clr[15]=255;
		}

		clr[ 4]=(clr[0]*11+clr[12]*5)>>4;
		clr[ 5]=(clr[1]*11+clr[13]*5)>>4;
		clr[ 6]=(clr[2]*11+clr[14]*5)>>4;
		clr[ 7]=255;
		clr[ 8]=(clr[0]*5+clr[12]*11)>>4;
		clr[ 9]=(clr[1]*5+clr[13]*11)>>4;
		clr[10]=(clr[2]*5+clr[14]*11)>>4;
		clr[11]=255;
	}
	
	if(bt==0)
	{
		if(xstride==4)
		{
			ct=rgba;		pxb=block[4];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[5];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[6];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[7];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			return;
		}
	
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=((block[4+i]>>(6-2*j))&3)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
//			rgba[k+3]=clr[l+3];
//			l=((block[4+i]>>(6-2*j))&3);
//			*(u32 *)(rgba+k)=clri[l];
		}
		return;
	}

	if(bt==1)
	{
		pxb=block[6];
		if(xstride==4)
		{
#if 0
			if(tflip&16)
			{
				ct=rgba+ystride;
				i0=*(u32 *)(ct-4);
				ct=rgba+3*ystride;
				i1=*(u32 *)(ct-4);
				ct=rgba-ystride;
				i2=*(u32 *)(ct+4);
				i3=*(u32 *)(ct+12);

				i=clri[(pxb>>6)&3];
				j=clri[(pxb>>4)&3];
				k=clri[(pxb>>2)&3];
				l=clri[(pxb   )&3];
			
				ct=rgba;
				*(u32 *)(ct+ 0)=i;	*(u32 *)(ct+ 4)=i2;
				*(u32 *)(ct+ 8)=j;	*(u32 *)(ct+12)=i3;
				ct+=ystride;
				*(u32 *)(ct+ 0)=i0;	*(u32 *)(ct+ 4)=i;
				*(u32 *)(ct+ 8)=i;	*(u32 *)(ct+12)=j;
				ct+=ystride;
				*(u32 *)(ct+ 0)=k;	*(u32 *)(ct+ 4)=i;
				*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=j;
				ct+=ystride;
				*(u32 *)(ct+ 0)=i1;	*(u32 *)(ct+ 4)=k;
				*(u32 *)(ct+ 8)=k;	*(u32 *)(ct+12)=l;
				return;
			}
#endif

			i=clri[(pxb>>6)&3];
			j=clri[(pxb>>4)&3];
			k=clri[(pxb>>2)&3];
			l=clri[(pxb   )&3];
			
			ct=rgba;
			*(u32 *)(ct+ 0)=i;	*(u32 *)(ct+ 4)=i;
			*(u32 *)(ct+ 8)=j;	*(u32 *)(ct+12)=j;
			ct+=ystride;
			*(u32 *)(ct+ 0)=i;	*(u32 *)(ct+ 4)=i;
			*(u32 *)(ct+ 8)=j;	*(u32 *)(ct+12)=j;
			ct+=ystride;
			*(u32 *)(ct+ 0)=k;	*(u32 *)(ct+ 4)=k;
			*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
			ct+=ystride;
			*(u32 *)(ct+ 0)=k;	*(u32 *)(ct+ 4)=k;
			*(u32 *)(ct+ 8)=l;	*(u32 *)(ct+12)=l;
			return;
		}
		
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=2*(i>>1)+(j>>1);
			l=((pxb>>(6-2*l))&3)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
//			rgba[k+3]=clr[l+3];
//			l=((pxb>>(6-2*l))&3);
//			*(u32 *)(rgba+k)=clri[l];
		}
		return;
	}
	
	if(bt==2)
	{
		pxb=(block[6]<<8)|block[7];
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=(pxb>>((3-i)*4+(3-j)))&1;
//			l=(l?3:0)*4;
			l=(l<<3)|(l<<2);
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
//			rgba[k+3]=clr[l+3];
//			l=(l<<1)|l;
//			*(u32 *)(rgba+k)=clri[l];
		}
		return;
	}
	
	if(bt==6)
	{
		if(xstride==4)
		{
			pxb=(block[10]<<16)|(block[11]<<8)|block[12];
			pxb1=(block[13]<<16)|(block[14]<<8)|block[15];

			ct=rgba;
			*(u32 *)(ct+ 0)=clri[(pxb>>21)&7];
			*(u32 *)(ct+ 4)=clri[(pxb>>18)&7];
			*(u32 *)(ct+ 8)=clri[(pxb>>15)&7];
			*(u32 *)(ct+12)=clri[(pxb>>12)&7];
			ct=ct+ystride;
			*(u32 *)(ct+ 0)=clri[(pxb>> 9)&7];
			*(u32 *)(ct+ 4)=clri[(pxb>> 6)&7];
			*(u32 *)(ct+ 8)=clri[(pxb>> 3)&7];
			*(u32 *)(ct+12)=clri[(pxb>> 0)&7];
			ct=ct+ystride;
			*(u32 *)(ct+ 0)=clri[(pxb1>>21)&7];
			*(u32 *)(ct+ 4)=clri[(pxb1>>18)&7];
			*(u32 *)(ct+ 8)=clri[(pxb1>>15)&7];
			*(u32 *)(ct+12)=clri[(pxb1>>12)&7];
			ct=ct+ystride;
			*(u32 *)(ct+ 0)=clri[(pxb1>> 9)&7];
			*(u32 *)(ct+ 4)=clri[(pxb1>> 6)&7];
			*(u32 *)(ct+ 8)=clri[(pxb1>> 3)&7];
			*(u32 *)(ct+12)=clri[(pxb1    )&7];
			return;
		}
	
		i=(block[10]<<8)|block[11];
		j=(block[12]<<8)|block[13];
		k=(block[14]<<8)|block[15];
		pxb2=(((u64)i)<<32)|(((u64)j)<<16)|k;
	
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=(pxb2>>(45-3*(i*4+j)))&7;
			l=l<<2;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
//			*(u32 *)(rgba+k)=clri[l];
		}
		return;
	}
	
	if(bt==14)
	{
		if(xstride==4)
		{
			ct=rgba;		pxb=block[12];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[13];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[14];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			ct=ct+ystride;		pxb=block[15];
			*(u32 *)(ct+ 0)=clri[(pxb>>6)&3];
			*(u32 *)(ct+ 4)=clri[(pxb>>4)&3];
			*(u32 *)(ct+ 8)=clri[(pxb>>2)&3];
			*(u32 *)(ct+12)=clri[(pxb   )&3];
			return;
		}
	
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			l=((block[12+i]>>(6-2*j))&3)*4;
			rgba[k+0]=clr[l+0];
			rgba[k+1]=clr[l+1];
			rgba[k+2]=clr[l+2];
		}
		return;
	}
}

void BTIC1H_DecodeBlockMB2B_RGB(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride)
{
	BTIC1H_DecodeBlockMB2B_RGBI(ctx, block, rgba,
		xstride, ystride, 0);
}

void BTIC1H_DecodeBlockMB2B_BGR(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride)
{
	BTIC1H_DecodeBlockMB2B_RGBI(ctx, block, rgba,
		xstride, ystride, 1);
}

void BTIC1H_DecodeBlockMB2B_EdgeRGBI(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride,
	int xfrac, int yfrac, int tflip)
{
	byte tblk[16*4];
	byte *cs, *ct;
	int i, j, xn;

//	return;

	BTIC1H_DecodeBlockMB2B_RGBI(
		ctx, block, tblk, xstride, 4*xstride, tflip);

	xn=xfrac*xstride;
	for(i=0; i<yfrac; i++)
	{
		cs=tblk+i*4*xstride;
		ct=rgba+i*ystride;
		for(j=0; j<xn; j++)
			*ct++=*cs++;
	}
}

void BTIC1H_DecodeBlockMB2B_PYUV_YUY(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	int cy, cu, cv, cd;
	int cy0, cy1, cy2, cy3;
	int cu1, cv1;
	int cr, cg, cb;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int bt, pxb, pxb1;
	int i, j, k, l;

	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		cu=block[ 8+(i*2+j)];
		cv=block[12+(i*2+j)];
		cu1=cu-128; cv1=cv-128;

		k=i*8+j*2;
		cy0=block[16+k];	cy1=block[17+k];
		cy2=block[20+k];	cy3=block[21+k];

		if(tflip&1)
		{
			k=((i*2+0)*ystride)+(j*4);
			rgba[k+0]=cu;	rgba[k+1]=cy0;
			rgba[k+2]=cv;	rgba[k+3]=cy1;
			k=((i*2+1)*ystride)+(j*4);
			rgba[k+0]=cu;	rgba[k+1]=cy2;
			rgba[k+2]=cv;	rgba[k+3]=cy3;
		}else
		{
			k=((i*2+0)*ystride)+(j*4);
			rgba[k+0]=cy0;	rgba[k+1]=cu;
			rgba[k+2]=cy1;	rgba[k+3]=cv;
			k=((i*2+1)*ystride)+(j*4);
			rgba[k+0]=cy2;	rgba[k+1]=cu;
			rgba[k+2]=cy3;	rgba[k+3]=cv;
		}
	}
}

void BTIC1H_DecodeBlockMB2B_P420_YUV(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	byte clr[4][4];
	byte *ct;
	int cy0, cu0, cv0, cd0, cya0, cyb0;
	int cy1, cu1, cv1, cd1, cya1, cyb1;
	int cy2, cu2, cv2, cd2, cya2, cyb2;
	int cy3, cu3, cv3, cd3, cya3, cyb3;
	int i0, i1, i2, i3;
	int ci, pxb, cf;
	int i, j, k, l;
	
	cy0=block[ 0];	cu0=block[ 1];
	cv0=block[ 2];	cd0=block[ 5];
	cy1=block[ 8];	cu1=block[ 9];
	cv1=block[10];	cd1=block[11];
	cy2=block[16];	cu2=block[17];
	cv2=block[18];	cd2=block[19];
	cy3=block[20];	cu3=block[21];
	cv3=block[22];	cd3=block[23];

	cya0=cy0-(cd0>>1);	cyb0=cya0+cd0;
	cya1=cy1-(cd1>>1);	cyb1=cya1+cd1;
	cya2=cy2-(cd2>>1);	cyb2=cya2+cd2;
	cya3=cy3-(cd3>>1);	cyb3=cya3+cd3;

	clr[0][0]=clamp255(cya0);	clr[0][3]=clamp255(cyb0);
	clr[1][0]=clamp255(cya1);	clr[1][3]=clamp255(cyb1);
	clr[2][0]=clamp255(cya2);	clr[2][3]=clamp255(cyb2);
	clr[3][0]=clamp255(cya3);	clr[3][3]=clamp255(cyb3);
	
	for(i=0; i<4; i++)
	{
		clr[i][1]=(clr[i][0]*11+clr[i][3]*5)>>4;
		clr[i][2]=(clr[i][0]*5+clr[i][3]*11)>>4;
	}

//	cu0=128; cu1=128; cu2=128; cu3=128;
//	cv0=128; cv1=128; cv2=128; cv3=128;

	if(tflip&1)
	{
		ct=rgba; pxb=block[12];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=cu0; ct[1]=clr[0][i0];  ct[2]=cv0; ct[3]=clr[0][i1];
		ct[4]=cu1; ct[5]=clr[1][i2];  ct[6]=cv1; ct[7]=clr[1][i3];
		ct=ct+ystride; pxb=block[13];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=cu0; ct[1]=clr[0][i0];  ct[2]=cv0; ct[3]=clr[0][i1];
		ct[4]=cu1; ct[5]=clr[1][i2];  ct[6]=cv1; ct[7]=clr[1][i3];
		ct=ct+ystride; pxb=block[14];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=cu2; ct[1]=clr[2][i0];  ct[2]=cv2; ct[3]=clr[2][i1];
		ct[4]=cu3; ct[5]=clr[3][i2];  ct[6]=cv3; ct[7]=clr[3][i3];
		ct=ct+ystride; pxb=block[15];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=cu2; ct[1]=clr[2][i0];  ct[2]=cv2; ct[3]=clr[2][i1];
		ct[4]=cu3; ct[5]=clr[3][i2];  ct[6]=cv3; ct[7]=clr[3][i3];
		return;
	}else
	{
		ct=rgba; pxb=block[12];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=clr[0][i0]; ct[1]=cu0;  ct[2]=clr[0][i1]; ct[3]=cv0;
		ct[4]=clr[1][i2]; ct[5]=cu1;  ct[6]=clr[1][i3]; ct[7]=cv1;
		ct=ct+ystride; pxb=block[13];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=clr[0][i0]; ct[1]=cu0;  ct[2]=clr[0][i1]; ct[3]=cv0;
		ct[4]=clr[1][i2]; ct[5]=cu1;  ct[6]=clr[1][i3]; ct[7]=cv1;
		ct=ct+ystride; pxb=block[14];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=clr[2][i0]; ct[1]=cu2;  ct[2]=clr[2][i1]; ct[3]=cv2;
		ct[4]=clr[3][i2]; ct[5]=cu3;  ct[6]=clr[3][i3]; ct[7]=cv3;
		ct=ct+ystride; pxb=block[15];
		i0=(pxb>>6)&3; i1=(pxb>>4)&3;  i2=(pxb>>2)&3; i3=pxb&3;
		ct[0]=clr[2][i0]; ct[1]=cu2;  ct[2]=clr[2][i1]; ct[3]=cv2;
		ct[4]=clr[3][i2]; ct[5]=cu3;  ct[6]=clr[3][i3]; ct[7]=cv3;
		return;
	}
}

void BTIC1H_DecodeBlockMB2B_I4XX_YUY(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	byte clry[8];
	byte clru[4];
	byte clrv[4];

	u64 lpxb;
	byte *ct;
	int cy, cu, cv, cd, cdu, cdv;
	int cya, cyb, cua, cub, cva, cvb;
	int cy0, cy1, cy2, cy3;
	int cu1, cv1;
	int cr, cg, cb, ca;
	int cr0, cg0, cb0;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int cr3, cg3, cb3;
	int bt, pxb, pxb1, pxb2;
	int i0, i1, i2, i3, i4, i5, i6, i7;
	int i, j, k, l;
		
	i=block[3];
	cd=i; bt=0;
	cdu=0; cdv=0;
	
	cy=block[0];	cu=block[1];
	cv=block[2];	bt=block[4];
	cd=((block[5]-128)<<1);
	cdu=((block[8]-128)<<1);
	cdv=((block[9]-128)<<1);

	cya=cy-(cd>>1);		cyb=cya+cd;
	cua=cu-(cdu>>1);	cub=cua+cdu;
	cva=cv-(cdv>>1);	cvb=cva+cdv;
	
	if((cya|cyb|cua|cub|cva|cvb)>>8)
	{
		cya=clamp255(cya);
		cyb=clamp255(cyb);
		cua=clamp255(cua);
		cub=clamp255(cub);
		cva=clamp255(cva);
		cvb=clamp255(cvb);
	}
	
	if((bt==21) || (bt==22))
	{
		clry[0]=cya;	clry[7]=cyb;
		clru[0]=cua;	clrv[0]=cva;
		clru[3]=cub;	clrv[3]=cvb;

		clry[1]=(clry[0]*14+clry[7]* 2)>>4;
		clry[2]=(clry[0]*11+clry[7]* 5)>>4;
		clry[3]=(clry[0]* 9+clry[7]* 7)>>4;
		clry[4]=(clry[0]* 7+clry[7]* 9)>>4;
		clry[5]=(clry[0]* 5+clry[7]*11)>>4;
		clry[6]=(clry[0]* 2+clry[7]*14)>>4;

		clru[1]=(clru[0]*11+clru[3]* 5)>>4;
		clrv[1]=(clrv[0]*11+clrv[3]* 5)>>4;
		clru[2]=(clru[0]* 5+clru[3]*11)>>4;
		clrv[2]=(clrv[0]* 5+clrv[3]*11)>>4;
	}
	else
	{
		clry[0]=cya;	clru[0]=cua;	clrv[0]=cva;
		clry[3]=cyb;	clru[3]=cub;	clrv[3]=cvb;

		clry[1]=(clry[0]*11+clry[3]* 5)>>4;
		clru[1]=(clru[0]*11+clru[3]* 5)>>4;
		clrv[1]=(clrv[0]*11+clrv[3]* 5)>>4;

		clry[2]=(clry[0]* 5+clry[3]*11)>>4;
		clru[2]=(clru[0]* 5+clru[3]*11)>>4;
		clrv[2]=(clrv[0]* 5+clrv[3]*11)>>4;
	}

	if(bt==20)
	{
//		return;
	
		pxb=block[10];
		pxb1=block[11];

		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			i0=(block[12+(2*i+0)]>>(6-2*(2*j+0)))&3;
			i1=(block[12+(2*i+0)]>>(6-2*(2*j+1)))&3;
			i2=(block[12+(2*i+1)]>>(6-2*(2*j+0)))&3;
			i3=(block[12+(2*i+1)]>>(6-2*(2*j+1)))&3;

			l=2*i+j;
			i4=(pxb>>(6-2*l))&3;
			i5=(pxb1>>(6-2*l))&3;

			cy0=clry[i0];	cy1=clry[i1];
			cy2=clry[i2];	cy3=clry[i3];
			cu =clru[i4];	cv =clrv[i5];
			
			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cu;	rgba[i0+1]=cy0;
				rgba[i1+0]=cv;	rgba[i1+1]=cy1;
				rgba[i2+0]=cu;	rgba[i2+1]=cy2;
				rgba[i3+0]=cv;	rgba[i3+1]=cy3;
			}else
			{
				rgba[i0+0]=cy0;	rgba[i0+1]=cu;
				rgba[i1+0]=cy1;	rgba[i1+1]=cv;
				rgba[i2+0]=cy2;	rgba[i2+1]=cu;
				rgba[i3+0]=cy3;	rgba[i3+1]=cv;
			}
		}
		return;
	}

	if(bt==21)
	{	
		i=(block[10]<<8)|block[11];
		j=(block[12]<<8)|block[13];
		k=(block[14]<<8)|block[15];
		lpxb=(((u64)i)<<32)|(((u64)j)<<16)|k;

		pxb=(block[16]<<24)|(block[17]<<16)|(block[18]<<8)|(block[19]);
		pxb1=(block[20]<<24)|(block[21]<<16)|(block[22]<<8)|(block[23]);
	
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
		{
			k=i*ystride+j*xstride;
			i0=(lpxb>>(45-3*(i*4+j)))&7;
			i1=(pxb>>(30-2*(i*4+j)))&3;
			i2=(pxb1>>(30-2*(i*4+j)))&3;

			cy=clry[i0];
			cu=clru[i1];
			cv=clrv[i2];

			if(tflip&1)
			{
				rgba[k+0]=(j&1)?cv:cu;
				rgba[k+1]=cy;
			}else
			{
				rgba[k+0]=cy;
				rgba[k+1]=(j&1)?cv:cu;
			}
		}
		return;
	}

	if(bt==22)
	{
//		return;

		i=(block[10]<<8)|block[11];
		j=(block[12]<<8)|block[13];
		k=(block[14]<<8)|block[15];
		lpxb=(((u64)i)<<32)|(((u64)j)<<16)|k;

		pxb=block[6];
		pxb1=block[7];
	
		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			i4=(i*2+0)*4;	i5=(i*2+1)*4;
			i6=(j*2+0);		i7=(j*2+1);
			i0=(lpxb>>(45-3*(i4+i6)))&7;
			i1=(lpxb>>(45-3*(i4+i7)))&7;
			i2=(lpxb>>(45-3*(i5+i6)))&7;
			i3=(lpxb>>(45-3*(i5+i7)))&7;

			l=2*i+j;
			i4=(pxb>>(6-2*l))&3;
			i5=(pxb1>>(6-2*l))&3;

			cy0=clry[i0];	cy1=clry[i1];
			cy2=clry[i2];	cy3=clry[i3];
			cu =clru[i4];	cv =clrv[i5];

			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cu;	rgba[i0+1]=cy0;
				rgba[i1+0]=cv;	rgba[i1+1]=cy1;
				rgba[i2+0]=cu;	rgba[i2+1]=cy2;
				rgba[i3+0]=cv;	rgba[i3+1]=cy3;
			}else
			{
				rgba[i0+0]=cy0;	rgba[i0+1]=cu;
				rgba[i1+0]=cy1;	rgba[i1+1]=cv;
				rgba[i2+0]=cy2;	rgba[i2+1]=cu;
				rgba[i3+0]=cy3;	rgba[i3+1]=cv;
			}
		}
		return;
	}

	if(bt==23)
	{
//		return;

		pxb =block[ 6];
		pxb1=block[10];
		pxb2=block[11];

		for(i=0; i<2; i++)
			for(j=0; j<2; j++)
		{
			l=2*i+j;
			i0=(pxb >>(6-2*l))&3;
			i1=(pxb1>>(6-2*l))&3;
			i2=(pxb2>>(6-2*l))&3;
			cy=clry[i0]; cu=clru[i1]; cv=clrv[i2];
			
			i0=(i*2+0)*ystride+(j*2+0)*xstride;
			i1=(i*2+0)*ystride+(j*2+1)*xstride;
			i2=(i*2+1)*ystride+(j*2+0)*xstride;
			i3=(i*2+1)*ystride+(j*2+1)*xstride;

			if(tflip&1)
			{
				rgba[i0+0]=cu;	rgba[i0+1]=cy;
				rgba[i1+0]=cv;	rgba[i1+1]=cy;
				rgba[i2+0]=cu;	rgba[i2+1]=cy;
				rgba[i3+0]=cv;	rgba[i3+1]=cy;
			}else
			{
				rgba[i0+0]=cy;	rgba[i0+1]=cu;
				rgba[i1+0]=cy;	rgba[i1+1]=cv;
				rgba[i2+0]=cy;	rgba[i2+1]=cu;
				rgba[i3+0]=cy;	rgba[i3+1]=cv;
			}
		}
		return;
	}
}

void BTIC1H_DecodeBlockMB2B_YUY(
	BTIC1H_Context *ctx, byte *block,
	byte *rgba, int xstride, int ystride, int tflip)
{
	byte clr[8*4];
//	u32 clri[8];
	u64 pxb2;
	byte *ct;
	int cy, cu, cv, cd, cya, cyb, cdu, cdv;
	int cy1, cu1, cv1;
	int cr, cg, cb;
	int cr1, cg1, cb1;
	int cr2, cg2, cb2;
	int bt, pxb, pxb1;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	i=block[3];
	cd=i; bt=0;

#if 0
	switch(block[5])
	{
	case 6:
	case 7: case 17:
	case 14: case 15:
	case 18: case 19:
	case 20:
	case 21:	case 22:
		j=1; break;
	default: j=0; break;
	}
//	if((i==0) && j)
	if(1)
	{
		i0=255;	i1=0;
		i2=255;	i3=0;
	
		ct=rgba;
		ct[0]=i0; ct[1]=i2;  ct[2]=i1; ct[3]=i3;
		ct[4]=i1; ct[5]=i3;  ct[6]=i0; ct[7]=i2;
		ct=ct+ystride;
		ct[0]=i1; ct[1]=i2;  ct[2]=i0; ct[3]=i3;
		ct[4]=i0; ct[5]=i3;  ct[6]=i1; ct[7]=i2;
		ct=ct+ystride;
		ct[0]=i0; ct[1]=i3;  ct[2]=i1; ct[3]=i2;
		ct[4]=i1; ct[5]=i2;  ct[6]=i0; ct[7]=i3;
		ct=ct+ystride;
		ct[0]=i1; ct[1]=i3;  ct[2]=i0; ct[3]=i2;
		ct[4]=i0; ct[5]=i2;  ct[6]=i1; ct[7]=i3;
		return;
	}
#endif
	
	if(i==0)
	{
		if(block[4]==0)
		{
			cy=block[0];
			cu=block[1];
			cv=block[2];

//			cu=128; cv=128;

			if(tflip&1)
			{
				clr[0]=cu;	clr[1]=cy;
				clr[2]=cv;	clr[3]=cy;
				l=*(u32 *)(clr+0);
			}else
			{
				clr[0]=cy;	clr[1]=cu;
				clr[2]=cy;	clr[3]=cv;
				l=*(u32 *)(clr+0);
			}

			for(i=0; i<4; i++)
			{
				k=i*ystride;
				((u32 *)(rgba+k))[0]=l;
				((u32 *)(rgba+k))[1]=l;
			}
			return;
		}	

		bt=-1;

		if(block[4]==1)
			{ cd=block[5]; bt=1; }
		else if(block[4]==2)
			{ cd=block[5]; bt=2; }
		else if(block[4]==6)
			{ cd=block[5]; bt=6; }
		else if(block[4]==17)
		{
			BTIC1H_DecodeBlockMB2B_P420_YUV(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}else if(block[4]==7)
		{
			BTIC1H_DecodeBlockMB2B_PYUV_YUY(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}else if((block[4]==14) || (block[4]==15) ||
			(block[4]==19))
		{
			bt=block[4];
			cd=((block[5]-128)<<1);
			cdu=((block[8]-128)<<1);
			cdv=((block[9]-128)<<1);
			
			if(bt==15)bt=6;
			if(bt==19)bt=1;
		}else if((block[4]==20) || (block[4]==21) ||
			(block[4]==22) || (block[4]==23))
		{
			BTIC1H_DecodeBlockMB2B_I4XX_YUY(ctx, block, rgba,
				xstride, ystride, tflip);
			return;
		}
		else
		{
		}
	}

	cy=block[0];
	cu=block[1];
	cv=block[2];
//	cd=block[3];

	cya=cy-(cd>>1);
	cyb=cya+cd;

	if(bt==6)
	{
		clr[0]=clamp255(cya);
		clr[7]=clamp255(cyb);
		clr[1]=(clr[0]*14+clr[7]* 2)>>4;
		clr[2]=(clr[0]*11+clr[7]* 5)>>4;
		clr[3]=(clr[0]* 9+clr[7]* 7)>>4;
		clr[4]=(clr[0]* 7+clr[7]* 9)>>4;
		clr[5]=(clr[0]* 5+clr[7]*11)>>4;
		clr[6]=(clr[0]* 2+clr[7]*14)>>4;
	}else
	{
		clr[0]=clamp255(cya);
		clr[3]=clamp255(cyb);
		clr[1]=(clr[0]*11+clr[3]* 5)>>4;
		clr[2]=(clr[0]* 5+clr[3]*11)>>4;
	}
	
//	cu=128; cv=128;
	
//	if(bt==6)
//		bt=-1;
	
	if(bt==0)
	{
		for(i=0; i<4; i++)
		{
//			k=i*ystride;
			ct=rgba+(i*ystride);
			i0=(block[4+i]>>6)&3;
			i1=(block[4+i]>>4)&3;
			i2=(block[4+i]>>2)&3;
			i3=(block[4+i]   )&3;
			if(tflip&1)
			{
				ct[0]=cu; ct[1]=clr[i0];
				ct[2]=cv; ct[3]=clr[i1];
				ct[4]=cu; ct[5]=clr[i2];
				ct[6]=cv; ct[7]=clr[i3];
			}else
			{
				ct[0]=clr[i0]; ct[1]=cu;
				ct[2]=clr[i1]; ct[3]=cv;
				ct[4]=clr[i2]; ct[5]=cu;
				ct[6]=clr[i3]; ct[7]=cv;
			}
		}
		return;
	}else if(bt==1)
	{
		pxb=block[6];

		i0=(pxb>>6)&3;
		i1=(pxb>>4)&3;
		i2=(pxb>>2)&3;
		i3=(pxb   )&3;

//		i0=(block[4+i]>>6)&3;
//		i1=(block[4+i]>>4)&3;
//		i2=(block[4+i]>>2)&3;
//		i3=(block[4+i]   )&3;

		if(tflip&1)
		{
			ct=rgba;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i0];
			ct[4]=cu; ct[5]=clr[i1];  ct[6]=cv; ct[7]=clr[i1];
			ct=ct+ystride;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i0];
			ct[4]=cu; ct[5]=clr[i1];  ct[6]=cv; ct[7]=clr[i1];
			ct=ct+ystride;
			ct[0]=cu; ct[1]=clr[i2];  ct[2]=cv; ct[3]=clr[i2];
			ct[4]=cu; ct[5]=clr[i3];  ct[6]=cv; ct[7]=clr[i3];
			ct=ct+ystride;
			ct[0]=cu; ct[1]=clr[i2];  ct[2]=cv; ct[3]=clr[i2];
			ct[4]=cu; ct[5]=clr[i3];  ct[6]=cv; ct[7]=clr[i3];
		}else
		{
			ct=rgba;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i0]; ct[3]=cv;
			ct[4]=clr[i1]; ct[5]=cu;  ct[6]=clr[i1]; ct[7]=cv;
			ct=ct+ystride;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i0]; ct[3]=cv;
			ct[4]=clr[i1]; ct[5]=cu;  ct[6]=clr[i1]; ct[7]=cv;
			ct=ct+ystride;
			ct[0]=clr[i2]; ct[1]=cu;  ct[2]=clr[i2]; ct[3]=cv;
			ct[4]=clr[i3]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
			ct=ct+ystride;
			ct[0]=clr[i2]; ct[1]=cu;  ct[2]=clr[i2]; ct[3]=cv;
			ct[4]=clr[i3]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
		}
		return;
	}else if(bt==2)
	{
		pxb=(block[6]<<8)|block[7];
		for(i=0; i<4; i++)
		{
			ct=rgba+(i*ystride);
			j=pxb>>((3-i)*4);
			i0=(j>>3)&1;	i1=(j>>2)&1;
			i2=(j>>1)&1;	i3=(j   )&1;
			if(tflip&1)
			{
				ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i1];
				ct[4]=cu; ct[5]=clr[i2];  ct[6]=cv; ct[7]=clr[i3];
			}else
			{
				ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i1]; ct[3]=cv;
				ct[4]=clr[i2]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
			}
		}
		return;
	}else if(bt==6)
	{	
		i=(block[10]<<16)|(block[11]<<8)|block[12];
		j=(block[13]<<16)|(block[14]<<8)|block[15];

		if(tflip&1)
		{
			ct=rgba;
			i0=(i>>21)&7; i1=(i>>18)&7;  i2=(i>>15)&7; i3=(i>>12)&7;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i1];
			ct[4]=cu; ct[5]=clr[i2];  ct[6]=cv; ct[7]=clr[i3];
			ct=ct+ystride;
			i0=(i>>9)&7; i1=(i>>6)&7;  i2=(i>>3)&7; i3=i&7;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i1];
			ct[4]=cu; ct[5]=clr[i2];  ct[6]=cv; ct[7]=clr[i3];
			ct=ct+ystride;
			i0=(j>>21)&7; i1=(j>>18)&7;  i2=(j>>15)&7; i3=(j>>12)&7;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i1];
			ct[4]=cu; ct[5]=clr[i2];  ct[6]=cv; ct[7]=clr[i3];
			ct=ct+ystride;
			i0=(j>>9)&7; i1=(j>>6)&7;  i2=(j>>3)&7;  i3=j&7;
			ct[0]=cu; ct[1]=clr[i0];  ct[2]=cv; ct[3]=clr[i1];
			ct[4]=cu; ct[5]=clr[i2];  ct[6]=cv; ct[7]=clr[i3];
		}else
		{
			ct=rgba;
			i0=(i>>21)&7; i1=(i>>18)&7;  i2=(i>>15)&7; i3=(i>>12)&7;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i1]; ct[3]=cv;
			ct[4]=clr[i2]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
			ct=ct+ystride;
			i0=(i>>9)&7; i1=(i>>6)&7;  i2=(i>>3)&7; i3=i&7;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i1]; ct[3]=cv;
			ct[4]=clr[i2]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
			ct=ct+ystride;
			i0=(j>>21)&7; i1=(j>>18)&7;  i2=(j>>15)&7; i3=(j>>12)&7;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i1]; ct[3]=cv;
			ct[4]=clr[i2]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
			ct=ct+ystride;
			i0=(j>>9)&7; i1=(j>>6)&7;  i2=(j>>3)&7;  i3=j&7;
			ct[0]=clr[i0]; ct[1]=cu;  ct[2]=clr[i1]; ct[3]=cv;
			ct[4]=clr[i2]; ct[5]=cu;  ct[6]=clr[i3]; ct[7]=cv;
		}
		return;
	}else if(bt==14)
	{
		for(i=0; i<4; i++)
		{
			ct=rgba+(i*ystride);
			i0=(block[12+i]>>6)&3;
			i1=(block[12+i]>>4)&3;
			i2=(block[12+i]>>2)&3;
			i3=(block[12+i]   )&3;
			if(tflip&1)
			{
				ct[0]=cu; ct[1]=clr[i0];
				ct[2]=cv; ct[3]=clr[i1];
				ct[4]=cu; ct[5]=clr[i2];
				ct[6]=cv; ct[7]=clr[i3];
			}else
			{
				ct[0]=clr[i0]; ct[1]=cu;
				ct[2]=clr[i1]; ct[3]=cv;
				ct[4]=clr[i2]; ct[5]=cu;
				ct[6]=clr[i3]; ct[7]=cv;
			}
		}
		return;
	}
	
	if(1)
	{
		i0=255;
		i1=0;
		i2=255;
		i3=0;
	
		ct=rgba;
		ct[0]=i0; ct[1]=i2;  ct[2]=i1; ct[3]=i3;
		ct[4]=i1; ct[5]=i3;  ct[6]=i0; ct[7]=i2;
		ct=ct+ystride;
		ct[0]=i1; ct[1]=i2;  ct[2]=i0; ct[3]=i3;
		ct[4]=i0; ct[5]=i3;  ct[6]=i1; ct[7]=i2;
		ct=ct+ystride;
		ct[0]=i0; ct[1]=i3;  ct[2]=i1; ct[3]=i2;
		ct[4]=i1; ct[5]=i2;  ct[6]=i0; ct[7]=i3;
		ct=ct+ystride;
		ct[0]=i1; ct[1]=i3;  ct[2]=i0; ct[3]=i2;
		ct[4]=i0; ct[5]=i2;  ct[6]=i1; ct[7]=i3;
	}
}


void BTIC1H_DecodeImageMB2B(
	BTIC1H_Context *ctx, byte *block, int blkstride,
	byte *rgba, int xs, int ys, int stride)
{
	int xs1, ys1;
	int i, j;
	
	xs1=xs>>2; ys1=ys>>2;
	for(i=0; i<ys1; i++)
		for(j=0; j<xs1; j++)
	{
		BTIC1H_DecodeBlockMB2B_RGB(ctx,
			block+(i*xs1+j)*blkstride,
			rgba+(i*4*xs+j*4)*stride,
			stride, xs*stride);
	}
}

void BTIC1H_DecodeImageMB2B_ClrsBfl(
	BTIC1H_Context *ctx,
	byte *block, byte *blockfl, int blkstride,
	byte *rgba, int xs, int ys, int clrs)
{
	byte *rgba2;
	int xstr, ystr, tflip, tfl2;
	int xs1, ys1, xs2, ys2, xf, yf;
	int i, j;
	
	switch(ctx->clryuv)
	{
	case 2:
		ctx->DecTransColor1=BTIC1H_DecodeBlock_TransColor1GDbDr;
		ctx->DecTransColor1AB=BTIC1H_DecodeBlock_TransColor1ABGDbDr;
		ctx->DecTransColor2=BTIC1H_DecodeBlock_TransColor2GDbDr;
		ctx->DecTransColor420=BTIC1H_DecodeBlock_TransColor420GDbDr;
		break;

	case 0:
	default:
		ctx->DecTransColor1=BTIC1H_DecodeBlock_TransColor1YCbCr;
		ctx->DecTransColor1AB=BTIC1H_DecodeBlock_TransColor1ABYCbCr;
		ctx->DecTransColor2=BTIC1H_DecodeBlock_TransColor2YCbCr;
		ctx->DecTransColor420=BTIC1H_DecodeBlock_TransColor420YCbCr;
		break;
	}
	
//	if(ctx->flip)
//		ys=-ys;
	
	if((clrs>=BTIC1H_PXF_BC1) && (clrs<=BTIC1H_PXF_BC5))
	{
		i=(clrs-BTIC1H_PXF_BC1)+1;
		BTIC1H_ConvImageS2TCn(ctx, block, blkstride, rgba, i, xs, ys);
		return;
	}
	
	if(clrs==BTIC1H_PXF_BC7)
	{
		BTIC1H_ConvImageBC7n(ctx, block, blkstride, rgba, 7, xs, ys);
		return;
	}

	if((clrs==BTIC1H_PXF_YUYV) ||
		(clrs==BTIC1H_PXF_UYVY))
	{
		if(ys<0)
		{
			ys=-ys;
			xstr=2;
			ystr=-2*xs;
			rgba2=rgba+xs*(ys-1)*xstr;
		}else
		{
			xstr=2;
			ystr=2*xs;
			rgba2=rgba;
		}
		
		tflip=(clrs==BTIC1H_PXF_UYVY)?1:0;
		tfl2=tflip;

		xs1=xs>>2; ys1=ys>>2;
		xs2=(xs+3)>>2; ys2=(ys+3)>>2;
		for(i=0; i<ys1; i++)
		{
			for(j=0; j<xs1; j++)
			{
				if(blockfl && (blockfl[i*xs2+j]&1))
					continue;

//				tfl2=tflip|16;
//				if(!i || !j || ((i+1)>=ys1) || ((j+1)>=xs1))
//					tfl2=tflip;

				BTIC1H_DecodeBlockMB2B_YUY(ctx,
					block+(i*xs2+j)*blkstride,
					rgba2+(i*4*ystr)+(j*4*xstr),
					xstr, ystr, tfl2);
			}
		
		}
		return;
	}
	
	if(clrs==BTIC1H_PXF_RGBA)
		{ xstr=4; ystr=xstr*xs; tflip=2; }
	if(clrs==BTIC1H_PXF_BGRA)
		{ xstr=4; ystr=xstr*xs; tflip=3; }
	if(clrs==BTIC1H_PXF_RGBX)
		{ xstr=4; ystr=xstr*xs; tflip=0; }
	if(clrs==BTIC1H_PXF_BGRX)
		{ xstr=4; ystr=xstr*xs; tflip=1; }
	if(clrs==BTIC1H_PXF_RGB)
		{ xstr=3; ystr=xstr*xs; tflip=0; }
	if(clrs==BTIC1H_PXF_BGR)
		{ xstr=3; ystr=xstr*xs; tflip=1; }
	
	if(ys<0)
	{
		ys=-ys;
		ystr=-xstr*xs;
		rgba2=rgba+xs*(ys-1)*xstr;
	}else
	{
		rgba2=rgba;
	}

	xs1=xs>>2; ys1=ys>>2;
	xs2=(xs+3)>>2; ys2=(ys+3)>>2;
	xf=xs&3; yf=ys&3;
	for(i=0; i<ys1; i++)
	{
//		tfl2=tflip|16;
//		if(!i || ((i+1)>=ys1))
//			tfl2=tflip;
		tfl2=tflip;

		for(j=0; j<xs1; j++)
		{
			if(blockfl && (blockfl[i*xs2+j]&1))
				continue;

//			tfl2=tflip|16;
//			if(!i || !j || ((i+1)>=ys1) || ((j+1)>=xs1))
//				tfl2=tflip;

			BTIC1H_DecodeBlockMB2B_RGBI(ctx,
				block+(i*xs2+j)*blkstride,
				rgba2+(i*4*ystr)+(j*4*xstr),
				xstr, ystr, tfl2);

#ifdef BT1H_ENABLE_AX
			if(tflip&2)
			{
				BTIC1H_DecodeBlockMB2B_Alpha(ctx,
					block+(i*xs2+j)*blkstride+24,
					rgba2+(i*4*ystr)+(j*4*xstr)+3,
					xstr, ystr, tflip);
			}
#endif
		}
		if(xf)
		{
			if(blockfl && (blockfl[i*xs2+j]&1))
				continue;
#if 1
			BTIC1H_DecodeBlockMB2B_EdgeRGBI(ctx,
				block+(i*xs2+j)*blkstride,
				rgba2+(i*4*ystr)+(j*4*xstr),
				xstr, ystr, xf, 4, tflip);
#endif
		}
	}
	if(yf)
	{
		for(j=0; j<xs1; j++)
		{
			if(blockfl && (blockfl[i*xs2+j]&1))
				continue;
			BTIC1H_DecodeBlockMB2B_EdgeRGBI(ctx,
				block+(i*xs2+j)*blkstride,
				rgba2+(i*4*ystr)+(j*4*xstr),
				xstr, ystr, 4, yf, tflip);
		}
		if(xf)
		{
			if(blockfl && (blockfl[i*xs2+j]&1))
				return;
#if 1
			BTIC1H_DecodeBlockMB2B_EdgeRGBI(ctx,
				block+(i*xs2+j)*blkstride,
				rgba2+(i*4*ystr)+(j*4*xstr),
				xstr, ystr, xf, yf, tflip);
#endif
		}
	}
}

void BTIC1H_DecodeImageMB2B_Clrs(
	BTIC1H_Context *ctx, byte *block, int blkstride,
	byte *rgba, int xs, int ys, int clrs)
{
	BTIC1H_DecodeImageMB2B_ClrsBfl(
		ctx, block, NULL, blkstride, rgba, xs, ys, clrs);
}
