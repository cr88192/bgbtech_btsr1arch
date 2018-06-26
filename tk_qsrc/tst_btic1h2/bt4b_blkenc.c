/*
 * 8x8 blocks: Nominal 384 Bits, Ext=512 bits
 * 
 * Bytes 0-15: Color Vector
 *    0: Tag
 *    4: Cy:S16
 *    6: Cu:S16
 *    8: Cv:S16
 *   10: Dy:S16
 *   12: Du:S16
 *   14: Dv:S16
 * Bytes 16-39: Y Data (3 bpp)
 * Bytes 40-43: U Dava (2 bpp)
 * Bytes 44-47: V Dava (2 bpp)
 */

force_inline int lqtvq_fmin(int x, int y)
{
	return((x<y)?x:y);
#if 0
	int m, z;
	m=(x-y)>>31;
	z=(x&m)|(y&(~m));
	return(z);
#endif
}

force_inline int lqtvq_fmax(int x, int y)
{
	return((x>y)?x:y);
#if 0
	int m, z;
	m=(x-y)>>31;
	z=(x&(~m))|(y&(m));
	return(z);
#endif
}

force_inline int lqtvq_clamp255(int x)
{
	return((x<0)?0:((x>255)?255:x));
}

force_inline int lqtvq_clamp2047(int x)
{
	return((x<0)?0:((x>2047)?2047:x));
}

force_inline int lqtvq_clamp65535(int x)
{
	return((x<0)?0:((x>65535)?65535:x));
}

force_inline int lqtvq_clamp32767S(int x)
{
	return((x<(-32767))?(-32767):((x>32767)?32767:x));
}

force_inline int lqtvq_clamp32767U(int x)
{
	return((x<0)?0:((x>32767)?32767:x));
}

void BTIC4B_SplitIbufBGRA(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf, int *abuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int *cta0, *cta1;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int mca, nca;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy=255; mcu= 255; mcv= 255;
	ncy=  0; ncu=-255; ncv=-255;
	mca=255; nca=-255;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	cta0=abuf;	cta1=abuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cr0=(byte)(i0>>16);	cg0=(byte)(i0>> 8);	cb0=(byte)(i0    );
			cr1=(byte)(i1>>16);	cg1=(byte)(i1>> 8);	cb1=(byte)(i1    );
			cr2=(byte)(i2>>16);	cg2=(byte)(i2>> 8);	cb2=(byte)(i2    );
			cr3=(byte)(i3>>16);	cg3=(byte)(i3>> 8);	cb3=(byte)(i3    );
			ca0=(byte)(i0>>24);	ca1=(byte)(i1>>24);
			ca2=(byte)(i2>>24);	ca3=(byte)(i3>>24);
			cs0+=8;		cs1+=8;

//			cg=cg0;
//			cr=cr0;
//			cb=cb0;

			cg=(cg0+cg1+cg2+cg3)>>2;
			cr=(cr0+cr1+cr2+cr3)>>2;
			cb=(cb0+cb1+cb2+cb3)>>2;

//			if(ctx->clrt==BTIC4B_CLRT_RCT)
			if(1)
			{
				cy0=(2*cg0+cr0+cb0)>>2;
				cy1=(2*cg1+cr1+cb1)>>2;
				cy2=(2*cg2+cr2+cb2)>>2;
				cy3=(2*cg3+cr3+cb3)>>2;
				cu=cb-cg;
				cv=cr-cg;
			}else
			{
				cy0=cg0;	cy1=cg1;
				cy2=cg2;	cy3=cg3;
//				cu=((cb-cg)>>1)+128;
//				cv=((cr-cg)>>1)+128;
				cu=cb-cg;
				cv=cr-cg;
			}

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

#if 1
			cta0[0]=ca0; 	cta0[1]=ca1;
			cta1[0]=ca2; 	cta1[1]=ca3;
			cta0+=2;		cta1+=2;
#endif

#if 1
//			if((ca0|ca1|ca2|ca3) || (mcy>ncy))
			if(((ca0&ca1&ca2&ca3)==(ca0|ca1|ca2|ca3)) && (ca0>=8))
			{
				if(cy0<mcy)mcy=cy0;
				if(cy0>ncy)ncy=cy0;
				if(cy3<mcy)mcy=cy3;
				if(cy3>ncy)ncy=cy3;
				if(cy1<mcy)mcy=cy1;
				if(cy1>ncy)ncy=cy1;
				if(cy2<mcy)mcy=cy2;
				if(cy2>ncy)ncy=cy2;
				if(cu<mcu)mcu=cu;
				if(cu>ncu)ncu=cu;
				if(cv<mcv)mcv=cv;
				if(cv>ncv)ncv=cv;
			}else if((ca0+ca1+ca2+ca3)>=8)
			{
				if(ca0>=8)
				{	if(cy0<mcy)mcy=cy0;
					if(cy0>ncy)ncy=cy0;	}
				if(ca1>=8)
				{	if(cy3<mcy)mcy=cy3;
					if(cy3>ncy)ncy=cy3;	}
				if(ca0>=8)
				{	if(cy1<mcy)mcy=cy1;
					if(cy1>ncy)ncy=cy1;	}
				if(ca0>=8)
				{	if(cy2<mcy)mcy=cy2;
					if(cy2>ncy)ncy=cy2;	}
				if(cu<mcu)mcu=cu;
				if(cu>ncu)ncu=cu;
				if(cv<mcv)mcv=cv;
				if(cv>ncv)ncv=cv;
			}else
			{
			}
#endif

#if 1
			if(ca0<mca)mca=ca0;
			if(ca0>nca)nca=ca0;
			if(ca3<mca)mca=ca3;
			if(ca3>nca)nca=ca3;
			if(ca1<mca)mca=ca1;
			if(ca1>nca)nca=ca1;
			if(ca2<mca)mca=ca2;
			if(ca2>nca)nca=ca2;
#endif

#if 0
			mcy=lqtvq_fmin(mcy, cy0);
			mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);
			mcy=lqtvq_fmin(mcy, cy2);

			ncy=lqtvq_fmax(ncy, cy0);
			ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);
			ncy=lqtvq_fmax(ncy, cy2);

			mcu=lqtvq_fmin(mcu, cu);
			mcv=lqtvq_fmin(mcv, cv);

			ncu=lqtvq_fmax(ncu, cu);
			ncv=lqtvq_fmax(ncv, cv);
#endif
		}
		cty0+=8;	cty1+=8;
		cta0+=8;	cta1+=8;
	}
	
	if(mcy>ncy)
	{
		mcy=cy0; ncy=cy0;
		mcu=cu; ncu=cu;
		mcv=cv; ncv=cv;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=mca;	ncyuv[3]=nca;
}

void BTIC4B_SplitIbufBGRX(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int mca, nca;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy=255; mcu= 255; mcv= 255;
	ncy=  0; ncu=-255; ncv=-255;
	mca=255; nca=-255;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cr0=(byte)(i0>>16);	cg0=(byte)(i0>> 8);	cb0=(byte)(i0    );
			cr1=(byte)(i1>>16);	cg1=(byte)(i1>> 8);	cb1=(byte)(i1    );
			cr2=(byte)(i2>>16);	cg2=(byte)(i2>> 8);	cb2=(byte)(i2    );
			cr3=(byte)(i3>>16);	cg3=(byte)(i3>> 8);	cb3=(byte)(i3    );
			cs0+=8;		cs1+=8;

//			cg=cg0;
//			cr=cr0;
//			cb=cb0;

			cg=(cg0+cg1+cg2+cg3)>>2;
			cr=(cr0+cr1+cr2+cr3)>>2;
			cb=(cb0+cb1+cb2+cb3)>>2;

//			if(ctx->clrt==BTIC4B_CLRT_RCT)
			if(1)
			{
				cy0=(2*cg0+cr0+cb0)>>2;
				cy1=(2*cg1+cr1+cb1)>>2;
				cy2=(2*cg2+cr2+cb2)>>2;
				cy3=(2*cg3+cr3+cb3)>>2;
				cu=cb-cg;
				cv=cr-cg;
			}else
			{
				cy0=cg0;	cy1=cg1;
				cy2=cg2;	cy3=cg3;
//				cu=((cb-cg)>>1)+128;
//				cv=((cr-cg)>>1)+128;
				cu=cb-cg;
				cv=cr-cg;
			}

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

#if 1
			if(cy0<mcy)mcy=cy0;
			if(cy0>ncy)ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			if(cu<mcu)mcu=cu;
			if(cu>ncu)ncu=cu;
			if(cv<mcv)mcv=cv;
			if(cv>ncv)ncv=cv;
#endif

#if 0
			mcy=lqtvq_fmin(mcy, cy0);
			mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);
			mcy=lqtvq_fmin(mcy, cy2);

			ncy=lqtvq_fmax(ncy, cy0);
			ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);
			ncy=lqtvq_fmax(ncy, cy2);

			mcu=lqtvq_fmin(mcu, cu);
			mcv=lqtvq_fmin(mcv, cv);

			ncu=lqtvq_fmax(ncu, cu);
			ncv=lqtvq_fmax(ncv, cv);
#endif
		}
		cty0+=8;	cty1+=8;
	}
	
	if(mcy>ncy)
	{
		mcy=cy0; ncy=cy0;
		mcu=cu; ncu=cu;
		mcv=cv; ncv=cv;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufBGR(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int ca0, ca1, ca2, ca3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int mca, nca;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy=255; mcu= 255; mcv= 255;
	ncy=  0; ncu=-255; ncv=-255;
	mca=255; nca=-255;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=*((u32 *)(cs0+0));	i1=*((u32 *)(cs0+3));
			i2=*((u32 *)(cs1+0));	i3=*((u32 *)(cs1+3));
			cr0=(byte)(i0>>16);	cg0=(byte)(i0>> 8);	cb0=(byte)(i0    );
			cr1=(byte)(i1>>16);	cg1=(byte)(i1>> 8);	cb1=(byte)(i1    );
			cr2=(byte)(i2>>16);	cg2=(byte)(i2>> 8);	cb2=(byte)(i2    );
			cr3=(byte)(i3>>16);	cg3=(byte)(i3>> 8);	cb3=(byte)(i3    );
			cs0+=6;		cs1+=6;

			cg=(cg0+cg1+cg2+cg3)>>2;
			cr=(cr0+cr1+cr2+cr3)>>2;
			cb=(cb0+cb1+cb2+cb3)>>2;

//			if(ctx->clrt==BTIC4B_CLRT_RCT)
			if(1)
			{
				cy0=(2*cg0+cr0+cb0)>>2;
				cy1=(2*cg1+cr1+cb1)>>2;
				cy2=(2*cg2+cr2+cb2)>>2;
				cy3=(2*cg3+cr3+cb3)>>2;
				cu=cb-cg;
				cv=cr-cg;
			}else
			{
				cy0=cg0;	cy1=cg1;
				cy2=cg2;	cy3=cg3;
				cu=cb-cg;
				cv=cr-cg;
			}

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

#if 1
			if(cy0<mcy)mcy=cy0;
			if(cy0>ncy)ncy=cy0;
			if(cy3<mcy)mcy=cy3;
			if(cy3>ncy)ncy=cy3;
			if(cy1<mcy)mcy=cy1;
			if(cy1>ncy)ncy=cy1;
			if(cy2<mcy)mcy=cy2;
			if(cy2>ncy)ncy=cy2;
			if(cu<mcu)mcu=cu;
			if(cu>ncu)ncu=cu;
			if(cv<mcv)mcv=cv;
			if(cv>ncv)ncv=cv;
#endif
		}
		cty0+=8;	cty1+=8;
	}
	
	if(mcy>ncy)
	{
		mcy=cy0; ncy=cy0;
		mcu=cu; ncu=cu;
		mcv=cv; ncv=cv;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufBGRA444(BTIC4B_Context *ctx,
	byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf, int *abuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1;
	int *ctu0, *ctu1;
	int *ctv0, *ctv1;
	int *cta0, *cta1;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int ca0, ca1, ca2, ca3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int mca, nca;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy=255; mcu= 255; mcv= 255;
	ncy=  0; ncu=-255; ncv=-255;
	mca=255; nca=-255;
	cty0=ybuf;	cty1=ybuf+8;
	ctu0=ubuf;	ctu1=ubuf+8;
	ctv0=vbuf;	ctv1=vbuf+8;
	cta0=abuf;	cta1=abuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cr0=(byte)(i0>>16);	cg0=(byte)(i0>> 8);	cb0=(byte)(i0    );
			cr1=(byte)(i1>>16);	cg1=(byte)(i1>> 8);	cb1=(byte)(i1    );
			cr2=(byte)(i2>>16);	cg2=(byte)(i2>> 8);	cb2=(byte)(i2    );
			cr3=(byte)(i3>>16);	cg3=(byte)(i3>> 8);	cb3=(byte)(i3    );
			ca0=(byte)(i0>>24);	ca1=(byte)(i1>>24);
			ca2=(byte)(i2>>24);	ca3=(byte)(i3>>24);
			cs0+=8;		cs1+=8;

			cg=(cg0+cg1+cg2+cg3)>>2;
			cr=(cr0+cr1+cr2+cr3)>>2;
			cb=(cb0+cb1+cb2+cb3)>>2;

			if(ctx->clrt==BTIC4B_CLRT_RCT)
			{
				cy0=(2*cg0+cr0+cb0)>>2;
				cy1=(2*cg1+cr1+cb1)>>2;
				cy2=(2*cg2+cr2+cb2)>>2;
				cy3=(2*cg3+cr3+cb3)>>2;

				cu0=cb0-cg0;	cv0=cr0-cg0;
				cu1=cb1-cg1;	cv1=cr1-cg1;
				cu2=cb2-cg2;	cv2=cr2-cg2;
				cu3=cb3-cg3;	cv3=cr3-cg3;
			}else
			{
				cy0=cg0;	cy1=cg1;
				cy2=cg2;	cy3=cg3;

				cu0=cb0-cg0;	cv0=cr0-cg0;
				cu1=cb1-cg1;	cv1=cr1-cg1;
				cu2=cb2-cg2;	cv2=cr2-cg2;
				cu3=cb3-cg3;	cv3=cr3-cg3;
			}

//			cy0=cg0;	cy1=cg1;
//			cy2=cg2;	cy3=cg3;

//			cu=((cb-cg)>>1)+128;
//			cv=((cr-cg)>>1)+128;
//			cu=cb-cg;
//			cv=cr-cg;

//			cu0=(3*cu0+cu)>>2;		cv0=(3*cv0+cv)>>2;
//			cu1=(3*cu1+cu)>>2;		cv1=(3*cv1+cv)>>2;
//			cu2=(3*cu2+cu)>>2;		cv2=(3*cv2+cv)>>2;
//			cu3=(3*cu3+cu)>>2;		cv3=(3*cv3+cv)>>2;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			ctu0[0]=cu0; 	ctu0[1]=cu1;
			ctu1[0]=cu2; 	ctu1[1]=cu3;
			ctv0[0]=cv0; 	ctv0[1]=cv1;
			ctv1[0]=cv2; 	ctv1[1]=cv3;
			cta0[0]=ca0; 	cta0[1]=ca1;
			cta1[0]=ca2; 	cta1[1]=ca3;
			cty0+=2;		cty1+=2;
			ctu0+=2;		ctu1+=2;
			ctv0+=2;		ctv1+=2;
			cta0+=2;		cta1+=2;

#if 1
//			if((ca0|ca1|ca2|ca3) || (mcy>ncy))
			if(((ca0&ca1&ca2&ca3)==(ca0|ca1|ca2|ca3)) && (ca0>=8))
			{
				if(cy0<mcy)mcy=cy0;
				if(cy0>ncy)ncy=cy0;
				if(cy3<mcy)mcy=cy3;
				if(cy3>ncy)ncy=cy3;
				if(cy1<mcy)mcy=cy1;
				if(cy1>ncy)ncy=cy1;
				if(cy2<mcy)mcy=cy2;
				if(cy2>ncy)ncy=cy2;

				if(cu0<mcu)mcu=cu0;
				if(cu0>ncu)ncu=cu0;
				if(cu3<mcu)mcu=cu3;
				if(cu3>ncu)ncu=cu3;
				if(cu1<mcu)mcu=cu1;
				if(cu1>ncu)ncu=cu1;
				if(cu2<mcu)mcu=cu2;
				if(cu2>ncu)ncu=cu2;

				if(cv0<mcv)mcv=cv0;
				if(cv0>ncv)ncv=cv0;
				if(cv3<mcv)mcv=cv3;
				if(cv3>ncv)ncv=cv3;
				if(cv1<mcv)mcv=cv1;
				if(cv1>ncv)ncv=cv1;
				if(cv2<mcv)mcv=cv2;
				if(cv2>ncv)ncv=cv2;

//				if(cu<mcu)mcu=cu;
//				if(cu>ncu)ncu=cu;
//				if(cv<mcv)mcv=cv;
//				if(cv>ncv)ncv=cv;
			}else if((ca0+ca1+ca2+ca3)>=8)
			{
				if(ca0>=8)
				{
					if(cy0<mcy)mcy=cy0;
					if(cy0>ncy)ncy=cy0;
					if(cu0<mcu)mcu=cu0;
					if(cu0>ncu)ncu=cu0;
					if(cv0<mcv)mcv=cv0;
					if(cv0>ncv)ncv=cv0;
				}
				if(ca1>=8)
				{
					if(cy3<mcy)mcy=cy3;
					if(cy3>ncy)ncy=cy3;
					if(cu3<mcu)mcu=cu3;
					if(cu3>ncu)ncu=cu3;
					if(cv3<mcv)mcv=cv3;
					if(cv3>ncv)ncv=cv3;
				}
				if(ca0>=8)
				{
					if(cy1<mcy)mcy=cy1;
					if(cy1>ncy)ncy=cy1;
					if(cu1<mcu)mcu=cu1;
					if(cu1>ncu)ncu=cu1;
					if(cv1<mcv)mcv=cv1;
					if(cv1>ncv)ncv=cv1;
				}
				if(ca0>=8)
				{
					if(cy2<mcy)mcy=cy2;
					if(cy2>ncy)ncy=cy2;
					if(cu2<mcu)mcu=cu2;
					if(cu2>ncu)ncu=cu2;
					if(cv2<mcv)mcv=cv2;
					if(cv2>ncv)ncv=cv2;
				}
			}else
			{
			}
#endif

#if 1
			if(ca0<mca)mca=ca0;
			if(ca0>nca)nca=ca0;
			if(ca3<mca)mca=ca3;
			if(ca3>nca)nca=ca3;
			if(ca1<mca)mca=ca1;
			if(ca1>nca)nca=ca1;
			if(ca2<mca)mca=ca2;
			if(ca2>nca)nca=ca2;
#endif
		}
		cty0+=8;	cty1+=8;
		ctu0+=8;	ctu1+=8;
		ctv0+=8;	ctv1+=8;
		cta0+=8;	cta1+=8;
	}
	
	if(mcy>ncy)
	{
		mcy=cy0; ncy=cy0;
		mcu=cu0; ncu=cu0;
		mcv=cv0; ncv=cv0;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=mca;	ncyuv[3]=nca;
}


void BTIC4B_SplitIbufRGB30(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy=1023; mcu= 1023; mcv= 1023;
	ncy=   0; ncu=-1023; ncv=-1023;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cs0+=8;		cs1+=8;

			cr0=(i0>>20)&1023;	cg0=(i0>>8)&1023;	cb0=(i0)&1023;
			cr1=(i1>>20)&1023;	cg1=(i1>>8)&1023;	cb1=(i1)&1023;
			cr2=(i2>>20)&1023;	cg2=(i2>>8)&1023;	cb2=(i2)&1023;
			cr3=(i3>>20)&1023;	cg3=(i3>>8)&1023;	cb3=(i3)&1023;

			cg=cg0;
			cr=cr0;
			cb=cb0;

			cy0=cg0;	cy1=cg1;
			cy2=cg2;	cy3=cg3;

//			cu=((cb-cg)>>1)+512;
//			cv=((cr-cg)>>1)+512;
			cu=cb-cg;
			cv=cr-cg;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

			mcy=lqtvq_fmin(mcy, cy0);
			mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);
			mcy=lqtvq_fmin(mcy, cy2);

			ncy=lqtvq_fmax(ncy, cy0);
			ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);
			ncy=lqtvq_fmax(ncy, cy2);

			mcu=lqtvq_fmin(mcu, cu);
			mcv=lqtvq_fmin(mcv, cv);

			ncu=lqtvq_fmax(ncu, cu);
			ncv=lqtvq_fmax(ncv, cv);
		}
		cty0+=8;
		cty1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufRGB48(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy= 32767; mcu= 32767; mcv= 32767;
	ncy=-32767; ncu=-32767; ncv=-32767;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			cr0=((u16 *)cs0)[0]; cg0=((u16 *)cs0)[1]; cb0=((u16 *)cs0)[2];
			cr1=((u16 *)cs0)[3]; cg1=((u16 *)cs0)[4]; cb1=((u16 *)cs0)[5];
			cr2=((u16 *)cs1)[0]; cg2=((u16 *)cs1)[1]; cb2=((u16 *)cs1)[2];
			cr3=((u16 *)cs1)[3]; cg3=((u16 *)cs1)[4]; cb3=((u16 *)cs1)[5];
			cs0+=12;	cs1+=12;

//			cg=cg0;
//			cr=cr0;
//			cb=cb0;

			cg=(cg0+cg1+cg2+cg3)>>2;
			cr=(cr0+cr1+cr2+cr3)>>2;
			cb=(cb0+cb1+cb2+cb3)>>2;
			
//			cy0=cg0;	cy1=cg1;
//			cy2=cg2;	cy3=cg3;

			cy0=(2*cg0+cr+cb)>>2;
			cy1=(2*cg1+cr+cb)>>2;
			cy2=(2*cg2+cr+cb)>>2;
			cy3=(2*cg3+cr+cb)>>2;

//			cu=((cb-cg)>>1)+32768;
//			cv=((cr-cg)>>1)+32768;
			cu=cb-cg;
			cv=cr-cg;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;
			
			mcy=lqtvq_fmin(mcy, cy0);
			mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);
			mcy=lqtvq_fmin(mcy, cy2);

			ncy=lqtvq_fmax(ncy, cy0);
			ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);
			ncy=lqtvq_fmax(ncy, cy2);

			mcu=lqtvq_fmin(mcu, cu);
			mcv=lqtvq_fmin(mcv, cv);

			ncu=lqtvq_fmax(ncu, cu);
			ncv=lqtvq_fmax(ncv, cv);
		}
		cty0+=8;
		cty1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufRGB_12R11F(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy= 4095; mcu= 4095; mcv= 4095;
	ncy=-4095; ncu=-4095; ncv=-4095;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cs0+=8;		cs1+=8;

			cb0=(i0>>22)&1023;	cg0=(i0>>11)&2047;	cr0=(i0)&2047;
			cb1=(i1>>22)&1023;	cg1=(i1>>11)&2047;	cr1=(i1)&2047;
			cb2=(i2>>22)&1023;	cg2=(i2>>11)&2047;	cr2=(i2)&2047;
			cb3=(i3>>22)&1023;	cg3=(i3>>11)&2047;	cr3=(i3)&2047;
			
//			cb0=(cb0<<1)|(cb0>>9);	cb1=(cb1<<1)|(cb1>>9);
//			cb2=(cb2<<1)|(cb2>>9);	cb3=(cb3<<1)|(cb3>>9);
			cb0=cb0<<1;	cb1=cb1<<1;	cb2=cb2<<1;	cb3=cb3<<1;

//			cg=cg0;	cr=cr0;	cb=cb0;
			cg=(cg0+cg1+cg2+cg3+1)>>2;
			cr=(cr0+cr1+cr2+cr3+1)>>2;
			cb=(cb0+cb1+cb2+cb3+1)>>2;

//			cy0=(2*cg0+cr0+cb0)>>2;
//			cy1=(2*cg1+cr1+cb1)>>2;
//			cy2=(2*cg2+cr2+cb2)>>2;
//			cy3=(2*cg3+cr3+cb3)>>2;

			cy0=(2*cg0+cr+cb)>>2;
			cy1=(2*cg1+cr+cb)>>2;
			cy2=(2*cg2+cr+cb)>>2;
			cy3=(2*cg3+cr+cb)>>2;

//			cy0=cg0;	cy1=cg1;
//			cy2=cg2;	cy3=cg3;
			cu=cb-cg;	cv=cr-cg;

//			cu0=cb0-cg0; cv0=cr0-cg0;
//			cu1=cb1-cg1; cv1=cr1-cg1;
//			cu2=cb2-cg2; cv2=cr2-cg2;
//			cu3=cb3-cg3; cv3=cr3-cg3;
//			cu=(cu0+cu1+cu2+cu3)>>2;
//			cv=(cv0+cv1+cv2+cv3)>>2;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

			mcy=lqtvq_fmin(mcy, cy0);	mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);	mcy=lqtvq_fmin(mcy, cy2);
			ncy=lqtvq_fmax(ncy, cy0);	ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);	ncy=lqtvq_fmax(ncy, cy2);
			mcu=lqtvq_fmin(mcu, cu);	mcv=lqtvq_fmin(mcv, cv);
			ncu=lqtvq_fmax(ncu, cu);	ncv=lqtvq_fmax(ncv, cv);
		}
		cty0+=8;
		cty1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufRGB_16R11F(byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu, *ctv;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy= 32767; mcu= 32767; mcv= 32767;
	ncy=-32767; ncu=-32767; ncv=-32767;
	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cs0+=8;		cs1+=8;

			cb0=(i0>>22)&1023;	cg0=(i0>>11)&2047;	cr0=(i0)&2047;
			cb1=(i1>>22)&1023;	cg1=(i1>>11)&2047;	cr1=(i1)&2047;
			cb2=(i2>>22)&1023;	cg2=(i2>>11)&2047;	cr2=(i2)&2047;
			cb3=(i3>>22)&1023;	cg3=(i3>>11)&2047;	cr3=(i3)&2047;
			
			cr0=(cr0<<4)|(cr0>>7);	cr1=(cr1<<4)|(cr1>>7);
			cr2=(cr2<<4)|(cr2>>7);	cr3=(cr3<<4)|(cr3>>7);
			cg0=(cg0<<4)|(cg0>>7);	cg1=(cg1<<4)|(cg1>>7);
			cg2=(cg2<<4)|(cg2>>7);	cg3=(cg3<<4)|(cg3>>7);
			cb0=(cb0<<5)|(cb0>>5);	cb1=(cb1<<5)|(cb1>>5);
			cb2=(cb2<<5)|(cb2>>5);	cb3=(cb3<<5)|(cb3>>5);

			cg=(cg0+cg1+cg2+cg3+1)>>2;
			cr=(cr0+cr1+cr2+cr3+1)>>2;
			cb=(cb0+cb1+cb2+cb3+1)>>2;

//			cy0=(2*cg0+cr0+cb0)>>2;
//			cy1=(2*cg1+cr1+cb1)>>2;
//			cy2=(2*cg2+cr2+cb2)>>2;
//			cy3=(2*cg3+cr3+cb3)>>2;

			cy0=(2*cg0+cr+cb)>>2;
			cy1=(2*cg1+cr+cb)>>2;
			cy2=(2*cg2+cr+cb)>>2;
			cy3=(2*cg3+cr+cb)>>2;

			cu=cb-cg;	cv=cr-cg;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			*ctu++=cu;		*ctv++=cv;
			cty0+=2;		cty1+=2;

			mcy=lqtvq_fmin(mcy, cy0);	mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);	mcy=lqtvq_fmin(mcy, cy2);
			ncy=lqtvq_fmax(ncy, cy0);	ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);	ncy=lqtvq_fmax(ncy, cy2);
			mcu=lqtvq_fmin(mcu, cu);	mcv=lqtvq_fmin(mcv, cv);
			ncu=lqtvq_fmax(ncu, cu);	ncv=lqtvq_fmax(ncv, cv);
		}
		cty0+=8;
		cty1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}

void BTIC4B_SplitIbufRGB444_12R11F(
	BTIC4B_Context *ctx,
	byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu0, *ctu1, *ctv0, *ctv1;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy= 4095; mcu= 4095; mcv= 4095;
	ncy=-4095; ncu=-4095; ncv=-4095;
//	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	ctu0=ubuf;	ctu1=ubuf+8;
	ctv0=vbuf;	ctv1=vbuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cs0+=8;		cs1+=8;

			cb0=(i0>>22)&1023;	cg0=(i0>>11)&2047;	cr0=(i0)&2047;
			cb1=(i1>>22)&1023;	cg1=(i1>>11)&2047;	cr1=(i1)&2047;
			cb2=(i2>>22)&1023;	cg2=(i2>>11)&2047;	cr2=(i2)&2047;
			cb3=(i3>>22)&1023;	cg3=(i3>>11)&2047;	cr3=(i3)&2047;
			
			cb0=cb0<<1;	cb1=cb1<<1;	cb2=cb2<<1;	cb3=cb3<<1;

			cy0=(2*cg0+cr0+cb0)>>2;
			cy1=(2*cg1+cr1+cb1)>>2;
			cy2=(2*cg2+cr2+cb2)>>2;
			cy3=(2*cg3+cr3+cb3)>>2;

			cu0=cb0-cg0; cv0=cr0-cg0;
			cu1=cb1-cg1; cv1=cr1-cg1;
			cu2=cb2-cg2; cv2=cr2-cg2;
			cu3=cb3-cg3; cv3=cr3-cg3;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			ctu0[0]=cu0; 	ctu0[1]=cu1;
			ctu1[0]=cu2; 	ctu1[1]=cu3;
			ctv0[0]=cv0; 	ctv0[1]=cv1;
			ctv1[0]=cv2; 	ctv1[1]=cv3;
			cty0+=2;		cty1+=2;
			ctu0+=2;		ctu1+=2;
			ctv0+=2;		ctv1+=2;

			mcy=lqtvq_fmin(mcy, cy0);	mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);	mcy=lqtvq_fmin(mcy, cy2);
			ncy=lqtvq_fmax(ncy, cy0);	ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);	ncy=lqtvq_fmax(ncy, cy2);
			mcu=lqtvq_fmin(mcu, cu0);	mcu=lqtvq_fmin(mcu, cu3);
			mcu=lqtvq_fmin(mcu, cu1);	mcu=lqtvq_fmin(mcu, cu2);
			mcv=lqtvq_fmin(mcv, cv0);	mcv=lqtvq_fmin(mcv, cv3);
			mcv=lqtvq_fmin(mcv, cv1);	mcv=lqtvq_fmin(mcv, cv2);
			ncu=lqtvq_fmax(ncu, cu0);	ncu=lqtvq_fmax(ncu, cu3);
			ncu=lqtvq_fmax(ncu, cu1);	ncu=lqtvq_fmax(ncu, cu2);
			ncv=lqtvq_fmax(ncv, cv0);	ncv=lqtvq_fmax(ncv, cv3);
			ncv=lqtvq_fmax(ncv, cv1);	ncv=lqtvq_fmax(ncv, cv2);
		}
		cty0+=8;	cty1+=8;
		ctu0+=8;	ctu1+=8;
		ctv0+=8;	ctv1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}


u32 btic4b_img_rgbevtor16(int fb, int eb)
{
	int cb;
	if(fb)
		{ while(!(fb>>10)) { fb<<=1; eb--; } }
	else
		{ eb=0; }
	cb=(eb<<10)|(fb&0x3FF);
	return(cb);
}

void BTIC4B_SplitIbufRGB444_16RGB8E8(
	BTIC4B_Context *ctx,
	byte *ibuf, int ystr,
	int *ybuf, int *ubuf, int *vbuf,
	int *mcyuv, int *ncyuv)
{
	byte *cs0, *cs1;
	int *cty0, *cty1, *ctu0, *ctu1, *ctv0, *ctv1;
	int i0, i1, i2, i3;
	int cr0, cr1, cr2, cr3;
	int cg0, cg1, cg2, cg3;
	int cb0, cb1, cb2, cb3;
	int ce0, ce1, ce2, ce3;
	int cy0, cy1, cy2, cy3;
	int cu0, cu1, cu2, cu3;
	int cv0, cv1, cv2, cv3;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int cr, cg, cb;
	int cy, cu, cv;
	
	int i, j, k;
	
	mcy= 32767; mcu= 32767; mcv= 32767;
	ncy=-32767; ncu=-32767; ncv=-32767;
//	ctu=ubuf;	ctv=vbuf;
	cty0=ybuf;	cty1=ybuf+8;
	ctu0=ubuf;	ctu1=ubuf+8;
	ctv0=vbuf;	ctv1=vbuf+8;
	for(i=0; i<4; i++)
	{
		cs0=ibuf+(i*2+0)*ystr;
		cs1=ibuf+(i*2+1)*ystr;
		for(j=0; j<4; j++)
		{
			i0=((u32 *)cs0)[0];		i1=((u32 *)cs0)[1];
			i2=((u32 *)cs1)[0];		i3=((u32 *)cs1)[1];
			cs0+=8;		cs1+=8;

			cb0=(i0>>16)&255;	cg0=(i0>> 8)&255;	cr0=(i0)&255;
			cb1=(i1>>16)&255;	cg1=(i1>> 8)&255;	cr1=(i1)&255;
			cb2=(i2>>16)&255;	cg2=(i2>> 8)&255;	cr2=(i2)&255;
			cb3=(i3>>16)&255;	cg3=(i3>> 8)&255;	cr3=(i3)&255;
			ce0=(i0>>24)&255;	ce1=(i1>>24)&255;
			ce2=(i2>>24)&255;	ce3=(i3>>24)&255;
	
			cr0=btic4b_img_rgbevtor16(cr0<<3, ce0);
			cr1=btic4b_img_rgbevtor16(cr1<<3, ce1);
			cr2=btic4b_img_rgbevtor16(cr2<<3, ce2);
			cr3=btic4b_img_rgbevtor16(cr3<<3, ce3);
			cg0=btic4b_img_rgbevtor16(cg0<<3, ce0);
			cg1=btic4b_img_rgbevtor16(cg1<<3, ce1);
			cg2=btic4b_img_rgbevtor16(cg2<<3, ce2);
			cg3=btic4b_img_rgbevtor16(cg3<<3, ce3);
			cb0=btic4b_img_rgbevtor16(cb0<<3, ce0);
			cb1=btic4b_img_rgbevtor16(cb1<<3, ce1);
			cb2=btic4b_img_rgbevtor16(cb2<<3, ce2);
			cb3=btic4b_img_rgbevtor16(cb3<<3, ce3);
			
//			cb0=cb0<<1;	cb1=cb1<<1;	cb2=cb2<<1;	cb3=cb3<<1;

			cy0=(2*cg0+cr0+cb0)>>2;
			cy1=(2*cg1+cr1+cb1)>>2;
			cy2=(2*cg2+cr2+cb2)>>2;
			cy3=(2*cg3+cr3+cb3)>>2;

			cu0=cb0-cg0; cv0=cr0-cg0;
			cu1=cb1-cg1; cv1=cr1-cg1;
			cu2=cb2-cg2; cv2=cr2-cg2;
			cu3=cb3-cg3; cv3=cr3-cg3;

			cty0[0]=cy0; 	cty0[1]=cy1;
			cty1[0]=cy2; 	cty1[1]=cy3;
			ctu0[0]=cu0; 	ctu0[1]=cu1;
			ctu1[0]=cu2; 	ctu1[1]=cu3;
			ctv0[0]=cv0; 	ctv0[1]=cv1;
			ctv1[0]=cv2; 	ctv1[1]=cv3;
			cty0+=2;		cty1+=2;
			ctu0+=2;		ctu1+=2;
			ctv0+=2;		ctv1+=2;

			mcy=lqtvq_fmin(mcy, cy0);	mcy=lqtvq_fmin(mcy, cy3);
			mcy=lqtvq_fmin(mcy, cy1);	mcy=lqtvq_fmin(mcy, cy2);
			ncy=lqtvq_fmax(ncy, cy0);	ncy=lqtvq_fmax(ncy, cy3);
			ncy=lqtvq_fmax(ncy, cy1);	ncy=lqtvq_fmax(ncy, cy2);
			mcu=lqtvq_fmin(mcu, cu0);	mcu=lqtvq_fmin(mcu, cu3);
			mcu=lqtvq_fmin(mcu, cu1);	mcu=lqtvq_fmin(mcu, cu2);
			mcv=lqtvq_fmin(mcv, cv0);	mcv=lqtvq_fmin(mcv, cv3);
			mcv=lqtvq_fmin(mcv, cv1);	mcv=lqtvq_fmin(mcv, cv2);
			ncu=lqtvq_fmax(ncu, cu0);	ncu=lqtvq_fmax(ncu, cu3);
			ncu=lqtvq_fmax(ncu, cu1);	ncu=lqtvq_fmax(ncu, cu2);
			ncv=lqtvq_fmax(ncv, cv0);	ncv=lqtvq_fmax(ncv, cv3);
			ncv=lqtvq_fmax(ncv, cv1);	ncv=lqtvq_fmax(ncv, cv2);
		}
		cty0+=8;	cty1+=8;
		ctu0+=8;	ctu1+=8;
		ctv0+=8;	ctv1+=8;
	}
	
	mcyuv[0]=mcy;	mcyuv[1]=mcu;	mcyuv[2]=mcv;
	ncyuv[0]=ncy;	ncyuv[1]=ncu;	ncyuv[2]=ncv;
	mcyuv[3]=255;	ncyuv[3]=255;
}


int lqtvq_fsctab[256];
u16 lqtvq_btohf[256];
byte lqtvq_hftob[2048];
byte lqtvq_fsctab_init=0;

void BTIC4B_InitScTables()
{
	int i, j, k, e;

	if(lqtvq_fsctab_init)
		return;
	lqtvq_fsctab_init=1;

	for(i=0; i<256; i++)
	{
//		lqtvq_fsctab[i]=(32768-1024)/(i+1);
//		lqtvq_fsctab[i]=32768/(i+1);
		lqtvq_fsctab[i]=8388608/(i+1);
	}

//	lqtvq_btohf[0]=0;
//	lqtvq_btohf[0]=0x1C00;
//	lqtvq_btohf[0]=0x1800;
	lqtvq_btohf[0]=0x1A00;
	for(i=1; i<256; i++)
	{
//		j=i; e=15+2;
		j=i*(1024.0/255.0)+0.5; e=15;
		while(!(j>>10))
			{ j=j<<1; e--; }
		k=(j&0x3FF)|(e<<10);
		lqtvq_btohf[i]=k;
	}

	for(i=0; i<2048; i++)
	{
//		j=(i<<6)|(i>>3);
		j=(i<<4)|(i>>7);
		e=(j>>10)&0x1F;
//		e=(e-15)-2;
		e=(e-15);
		j=(j&0x3FF)|0x400;
		if(e<0)
			{ j=j>>(-e); }
		else
			{ j=j<<e; }
		j=j*(255.0/1024.0)+0.5;
//		j=j>>2;
//		j=(j+1)>>2;
		lqtvq_hftob[i]=lqtvq_clamp255(j);
	}
}

force_inline int btic4f_btohf(int v)
{
	return(lqtvq_btohf[lqtvq_clamp255(v)]);
}

force_inline int btic4f_hftob(int v)
{
	int i;
//	i=v>>6;
//	i=(v+31)>>6;
	i=v>>4;
	if(i<0)i=0;
	if(i>2047)i=2047;
	return(lqtvq_hftob[i]);
}

#if 1

#if 1
//force_inline int BTIC4B_SAT1(int x)
//{
//	static const int tab[8]=
//		{0,0,0,0,1,1,1,1};
//	return(tab[x+3]);
//	return((x<0)?0:((x>1)?1:x));
//}

const int lqtvq_sat1_tab[8]=
		{0,0,0,0, 1,1,1,1};
#define BTIC4B_SAT1(x)	lqtvq_sat1_tab[(x)+3]

const int lqtvq_sat2_tab[8]=
		{0,0,0,1, 2,3,3,3};
#define BTIC4B_SAT2(x)	lqtvq_sat2_tab[(x)+2]

const int lqtvq_sat3_tab[16]=
		{0,0,0,0, 0,1,2,3, 4,5,6,7, 7,7,7,7};
#define BTIC4B_SAT3(x)	lqtvq_sat3_tab[(x)+4]

const int lqtvq_sat4_tab[32]=
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 1, 2, 3, 4, 5, 6, 7,
		 8, 9,10,11,12,13,14,15,
		15,15,15,15,15,15,15,15};
#define BTIC4B_SAT4(x)	lqtvq_sat4_tab[(x)+8]

#else
#define BTIC4B_SAT1(x)	(x)
#define BTIC4B_SAT2(x)	(x)
#endif

// #define BTIC4B_BIAS1		8191
// #define BTIC4B_BIAS1		16383
#define BTIC4B_BIAS1			65535
// #define BTIC4B_BIAS1			131071
// #define BTIC4B_BIAS1			262143
// #define BTIC4B_BIAS1			((1<<20)-1)

force_inline void BTIC4B_EncUVBits2x2x1(
	byte *oblk, int *ubuf, int lsu, int acu)
{
	static const int lc2=8388608+4095;
	static const int lc3=8388608-4095;
	int u0, u1, u2, u3;
	u32 i2;

	u0=(ubuf[ 0]+ubuf[ 1]+ubuf[ 4]+ubuf[ 5])>>2;
	u1=(ubuf[ 2]+ubuf[ 3]+ubuf[ 6]+ubuf[ 7])>>2;
	u2=(ubuf[ 8]+ubuf[ 9]+ubuf[12]+ubuf[13])>>2;
	u3=(ubuf[10]+ubuf[11]+ubuf[14]+ubuf[15])>>2;
	i2=        BTIC4B_SAT1(((u3-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((u2-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((u1-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((u0-acu)*lsu+lc2)>>23);
	*(byte *)oblk=i2;
}

force_inline void BTIC4B_EncUVBits4x4x1(
	byte *oblk, int *ubuf, int lsu, int acu)
{
	static const int lc2=8388608+BTIC4B_BIAS1;
	static const int lc3=8388608-BTIC4B_BIAS1;
	u32 i2;

	i2=        BTIC4B_SAT1(((ubuf[15]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[14]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[13]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[12]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[11]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[10]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 9]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 8]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 7]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 6]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 5]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 4]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 3]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 2]-acu)*lsu+lc2)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 1]-acu)*lsu+lc3)>>23);
	i2=(i2<<1)|BTIC4B_SAT1(((ubuf[ 0]-acu)*lsu+lc2)>>23);
	*(u16 *)oblk=i2;
}

force_inline void BTIC4B_EncYBits2x2x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	int y0, y1, y2, y3;
	u32 i0, i1;
	int i;

	y0=(ybuf[ 0]+ybuf[ 2]+ybuf[16]+ybuf[18])>>2;
	y1=(ybuf[ 4]+ybuf[ 6]+ybuf[20]+ybuf[22])>>2;
	y2=(ybuf[32]+ybuf[34]+ybuf[48]+ybuf[50])>>2;
	y3=(ybuf[36]+ybuf[38]+ybuf[52]+ybuf[54])>>2;

	i0=        BTIC4B_SAT1(((y3-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((y2-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((y1-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((y0-acy)*lsy+lc1)>>23);
	*(byte *)oblk=i0;
}


force_inline void BTIC4B_EncYBits4x2x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	u32 i0, i1;
	int i;

	i0=        BTIC4B_SAT1(((ybuf[38]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[36]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[34]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[32]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 6]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 4]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 2]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 0]-acy)*lsy+lc0)>>23);
	*(byte *)oblk=i0;
}

force_inline void BTIC4B_EncYBits2x4x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	u32 i0, i1;
	int i;

	i0=        BTIC4B_SAT1(((ybuf[52]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[48]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[36]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[32]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[20]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[16]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 4]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 0]-acy)*lsy+lc1)>>23);
	*(byte *)oblk=i0;
}

force_inline void BTIC4B_EncYBits4x4x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	u32 i0, i1;
	int i;

	i0=        BTIC4B_SAT1(((ybuf[54]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[52]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[50]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[48]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[38]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[36]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[34]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[32]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[22]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[20]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[18]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[16]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 6]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 4]-acy)*lsy+lc0)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 2]-acy)*lsy+lc1)>>23);
	i0=(i0<<1)|BTIC4B_SAT1(((ybuf[ 0]-acy)*lsy+lc0)>>23);
	*(u16 *)oblk=i0;
}

force_inline void BTIC4B_EncYBits4x8x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<2; i++)
	{
		i0=        BTIC4B_SAT1(((csy[30]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[28]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[26]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[24]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[22]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[20]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[18]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[16]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[14]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[12]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[10]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 8]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 6]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 4]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 2]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 0]-acy)*lsy+lc0)>>23);
		*(u16 *)(oblk+i*2)=i0;
		csy+=32;
	}
}

force_inline void BTIC4B_EncYBits8x4x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<2; i++)
	{
		i0=        BTIC4B_SAT1(((csy[23]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[22]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[21]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[20]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[19]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[18]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[17]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[16]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 7]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 6]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 5]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 4]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 3]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 2]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 1]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 0]-acy)*lsy+lc0)>>23);
		*(u16 *)(oblk+i*2)=i0;
		csy+=32;
	}
}

force_inline void BTIC4B_EncYBits8x8x1(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+BTIC4B_BIAS1;
	static const int lc1=8388608-BTIC4B_BIAS1;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<4; i++)
	{
		i0=        BTIC4B_SAT1(((csy[15]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[14]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[13]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[12]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[11]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[10]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 9]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 8]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 7]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 6]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 5]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 4]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 3]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 2]-acy)*lsy+lc0)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 1]-acy)*lsy+lc1)>>23);
		i0=(i0<<1)|BTIC4B_SAT1(((csy[ 0]-acy)*lsy+lc0)>>23);
		*(u16 *)(oblk+i*2)=i0;
		csy+=16;
	}
}
#endif

#if 1
force_inline void BTIC4B_EncUVBits2x2x2(
	byte *oblk, int *ubuf, int lsu, int acu)
{
	static const int lc2=8388608+4095;
	static const int lc3=8388608-4095;
	int u0, u1, u2, u3;
	u32 i2;

	u0=(ubuf[ 0]+ubuf[ 1]+ubuf[ 4]+ubuf[ 5])>>2;
	u1=(ubuf[ 2]+ubuf[ 3]+ubuf[ 6]+ubuf[ 7])>>2;
	u2=(ubuf[ 8]+ubuf[ 9]+ubuf[12]+ubuf[13])>>2;
	u3=(ubuf[10]+ubuf[11]+ubuf[14]+ubuf[15])>>2;
	i2=        BTIC4B_SAT2(((u3-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((u2-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((u1-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((u0-acu)*lsu+lc2)>>22);
	*(byte *)oblk=i2;
}

force_inline void BTIC4B_EncUVBits4x4x2(
	byte *oblk, int *ubuf, int lsu, int acu)
{
	static const int lc2=8388608+4095;
	static const int lc3=8388608-4095;
	u32 i2;

	i2=        BTIC4B_SAT2(((ubuf[15]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[14]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[13]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[12]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[11]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[10]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 9]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 8]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 7]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 6]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 5]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 4]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 3]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 2]-acu)*lsu+lc2)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 1]-acu)*lsu+lc3)>>22);
	i2=(i2<<2)|BTIC4B_SAT2(((ubuf[ 0]-acu)*lsu+lc2)>>22);
	*(u32 *)oblk=i2;
}

force_inline void BTIC4B_EncUVBits4x4x4(
	byte *oblk, int *ubuf, int lsu, int acu)
{
	static const int lc2=8388608+4095;
	static const int lc3=8388608-4095;
	u32 i0, i1;

	i1=        (((ubuf[15]-acu)*lsu+lc2)>>20);
	i1=(i1<<4)|(((ubuf[14]-acu)*lsu+lc3)>>20);
	i1=(i1<<4)|(((ubuf[13]-acu)*lsu+lc2)>>20);
	i1=(i1<<4)|(((ubuf[12]-acu)*lsu+lc3)>>20);
	i1=(i1<<4)|(((ubuf[11]-acu)*lsu+lc3)>>20);
	i1=(i1<<4)|(((ubuf[10]-acu)*lsu+lc2)>>20);
	i1=(i1<<4)|(((ubuf[ 9]-acu)*lsu+lc3)>>20);
	i1=(i1<<4)|(((ubuf[ 8]-acu)*lsu+lc2)>>20);
	i0=        (((ubuf[ 7]-acu)*lsu+lc2)>>20);
	i0=(i0<<4)|(((ubuf[ 6]-acu)*lsu+lc3)>>20);
	i0=(i0<<4)|(((ubuf[ 5]-acu)*lsu+lc2)>>20);
	i0=(i0<<4)|(((ubuf[ 4]-acu)*lsu+lc3)>>20);
	i0=(i0<<4)|(((ubuf[ 3]-acu)*lsu+lc3)>>20);
	i0=(i0<<4)|(((ubuf[ 2]-acu)*lsu+lc2)>>20);
	i0=(i0<<4)|(((ubuf[ 1]-acu)*lsu+lc3)>>20);
	i0=(i0<<4)|(((ubuf[ 0]-acu)*lsu+lc2)>>20);
	*(u32 *)(oblk+0)=i0;
	*(u32 *)(oblk+4)=i1;
}

force_inline void BTIC4B_EncYBits2x2x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	int y0, y1, y2, y3;
	u32 i0, i1;
	int i;

	y0=(ybuf[ 0]+ybuf[ 2]+ybuf[16]+ybuf[18])>>2;
	y1=(ybuf[ 4]+ybuf[ 6]+ybuf[20]+ybuf[22])>>2;
	y2=(ybuf[32]+ybuf[34]+ybuf[48]+ybuf[50])>>2;
	y3=(ybuf[36]+ybuf[38]+ybuf[52]+ybuf[54])>>2;

	i0=        BTIC4B_SAT2(((y3-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y1-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y0-acy)*lsy+lc1)>>22);
	*(u32 *)oblk=i0;
}


force_inline void BTIC4B_EncYBits4x2x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	u32 i0, i1;
	int i;

	i0=        BTIC4B_SAT2(((ybuf[38]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[36]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[34]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[32]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 6]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 4]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 2]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 0]-acy)*lsy+lc0)>>22);

	*(u16 *)oblk=i0;
}

force_inline void BTIC4B_EncYBits2x4x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	u32 i0, i1;
	int i;

	i0=        BTIC4B_SAT2(((ybuf[52]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[48]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[36]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[32]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[20]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[16]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 4]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 0]-acy)*lsy+lc1)>>22);
	*(u16 *)oblk=i0;
}

force_inline void BTIC4B_EncYBits4x4x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	int y2buf[16];
	u32 i0, i1;
	int i;
	
#if 1
	y2buf[ 0]=(ybuf[ 0]+ybuf[ 1]+ybuf[ 8]+ybuf[ 9])>>2;
	y2buf[ 1]=(ybuf[ 2]+ybuf[ 3]+ybuf[10]+ybuf[11])>>2;
	y2buf[ 2]=(ybuf[ 4]+ybuf[ 5]+ybuf[12]+ybuf[13])>>2;
	y2buf[ 3]=(ybuf[ 6]+ybuf[ 7]+ybuf[14]+ybuf[15])>>2;
	y2buf[ 4]=(ybuf[16]+ybuf[17]+ybuf[24]+ybuf[25])>>2;
	y2buf[ 5]=(ybuf[18]+ybuf[19]+ybuf[26]+ybuf[27])>>2;
	y2buf[ 6]=(ybuf[20]+ybuf[21]+ybuf[28]+ybuf[29])>>2;
	y2buf[ 7]=(ybuf[22]+ybuf[23]+ybuf[30]+ybuf[31])>>2;
	y2buf[ 8]=(ybuf[32]+ybuf[33]+ybuf[40]+ybuf[41])>>2;
	y2buf[ 9]=(ybuf[34]+ybuf[35]+ybuf[42]+ybuf[43])>>2;
	y2buf[10]=(ybuf[36]+ybuf[37]+ybuf[44]+ybuf[45])>>2;
	y2buf[11]=(ybuf[38]+ybuf[39]+ybuf[46]+ybuf[47])>>2;
	y2buf[12]=(ybuf[48]+ybuf[49]+ybuf[56]+ybuf[56])>>2;
	y2buf[13]=(ybuf[50]+ybuf[51]+ybuf[58]+ybuf[59])>>2;
	y2buf[14]=(ybuf[52]+ybuf[53]+ybuf[60]+ybuf[61])>>2;
	y2buf[15]=(ybuf[54]+ybuf[55]+ybuf[62]+ybuf[62])>>2;

	i0=        BTIC4B_SAT2(((y2buf[15]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[14]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[13]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[12]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[11]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[10]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 9]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 8]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 7]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 6]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 5]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 4]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 3]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 2]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 1]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((y2buf[ 0]-acy)*lsy+lc0)>>22);
	*(u32 *)oblk=i0;
#endif

#if 0
	i0=        BTIC4B_SAT2(((ybuf[54]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[52]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[50]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[48]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[38]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[36]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[34]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[32]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[22]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[20]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[18]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[16]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 6]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 4]-acy)*lsy+lc0)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 2]-acy)*lsy+lc1)>>22);
	i0=(i0<<2)|BTIC4B_SAT2(((ybuf[ 0]-acy)*lsy+lc0)>>22);
	*(u32 *)oblk=i0;
#endif
}

force_inline void BTIC4B_EncYBits4x8x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<2; i++)
	{
		i0=        BTIC4B_SAT2(((csy[30]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[28]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[26]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[24]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[22]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[20]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[18]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[16]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[14]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[12]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[10]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 8]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 6]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 4]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 2]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 0]-acy)*lsy+lc0)>>22);
		*(u32 *)(oblk+i*4)=i0;
		csy+=32;
	}
}

force_inline void BTIC4B_EncYBits8x4x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<2; i++)
	{
		i0=        BTIC4B_SAT2(((csy[23]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[22]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[21]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[20]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[19]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[18]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[17]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[16]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 7]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 6]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 5]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 4]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 3]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 2]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 1]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 0]-acy)*lsy+lc0)>>22);
		*(u32 *)(oblk+i*4)=i0;
		csy+=32;
	}
}

force_inline void BTIC4B_EncYBits4x8x2EO(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<2; i++)
	{
		i0=        BTIC4B_SAT2(((csy[31]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[29]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[27]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[25]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[22]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[20]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[18]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[16]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[15]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[13]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[11]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 9]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 6]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 4]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 2]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 0]-acy)*lsy+lc0)>>22);
		*(u32 *)(oblk+i*4)=i0;
		csy+=32;
	}
}

force_inline void BTIC4B_EncYBits8x8x2(
	byte *oblk, int *ybuf, int lsy, int acy)
{
//	static const int lc0=8388608+4095;
//	static const int lc1=8388608-4095;
	static const int lc0=8388608+1023;
	static const int lc1=8388608-1023;
	int *csy;
	u32 i0, i1;
	int i;

	csy=ybuf;
	for(i=0; i<4; i++)
	{
		i0=        BTIC4B_SAT2(((csy[15]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[14]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[13]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[12]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[11]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[10]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 9]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 8]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 7]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 6]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 5]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 4]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 3]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 2]-acy)*lsy+lc0)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 1]-acy)*lsy+lc1)>>22);
		i0=(i0<<2)|BTIC4B_SAT2(((csy[ 0]-acy)*lsy+lc0)>>22);
		*(u32 *)(oblk+i*4)=i0;
		csy+=16;
	}
}

force_inline void BTIC4B_EncYBits8x8x3(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
//	static const int lc0=8388608+2047;
//	static const int lc1=8388608-2047;
	int *csy, *csye;
	u32 i0, i1;
	int i;

	csy=ybuf; csye=ybuf+64;
//	for(i=0; i<4; i++)
//	while(csy<csye)
//	for(i=3; i>=0; i--)
	for(i=0; i<4; i++)
	{
//		csy=ybuf+i*16;

		i1=        BTIC4B_SAT3(((csy[15]-acy)*lsy+lc1)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[14]-acy)*lsy+lc0)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[13]-acy)*lsy+lc1)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[12]-acy)*lsy+lc0)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[11]-acy)*lsy+lc1)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[10]-acy)*lsy+lc0)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[ 9]-acy)*lsy+lc1)>>21);
		i1=(i1<<3)|BTIC4B_SAT3(((csy[ 8]-acy)*lsy+lc0)>>21);
		i0=        BTIC4B_SAT3(((csy[ 7]-acy)*lsy+lc0)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 6]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 5]-acy)*lsy+lc0)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 4]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 3]-acy)*lsy+lc0)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 2]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 1]-acy)*lsy+lc0)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 0]-acy)*lsy+lc1)>>21);
		*(u32 *)(oblk+i*6+0)=i0;
		*(u32 *)(oblk+i*6+3)=i1;
		csy+=16;
	}
}

force_inline void BTIC4B_EncYBits8x8x4(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
//	static const int lc0=8388608+1023;
//	static const int lc1=8388608-1023;
	int *csy, *csye;
	u32 i0, i1;
	int i;

	csy=ybuf; csye=ybuf+64;
	for(i=0; i<4; i++)
//	while(csy<csye)
	{
		i1=        BTIC4B_SAT4(((csy[15]-acy)*lsy+lc1)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[14]-acy)*lsy+lc0)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[13]-acy)*lsy+lc1)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[12]-acy)*lsy+lc0)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[11]-acy)*lsy+lc1)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[10]-acy)*lsy+lc0)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[ 9]-acy)*lsy+lc1)>>20);
		i1=(i1<<4)|BTIC4B_SAT4(((csy[ 8]-acy)*lsy+lc0)>>20);
		i0=        BTIC4B_SAT4(((csy[ 7]-acy)*lsy+lc0)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 6]-acy)*lsy+lc1)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 5]-acy)*lsy+lc0)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 4]-acy)*lsy+lc1)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 3]-acy)*lsy+lc0)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 2]-acy)*lsy+lc1)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 1]-acy)*lsy+lc0)>>20);
		i0=(i0<<4)|BTIC4B_SAT4(((csy[ 0]-acy)*lsy+lc1)>>20);
		*(u32 *)(oblk+i*8+0)=i0;
		*(u32 *)(oblk+i*8+4)=i1;
		csy+=16;
	}
}


force_inline void BTIC4B_EncYBits4x8x3EO(
	byte *oblk, int *ybuf, int lsy, int acy)
{
	static const int lc0=8388608+4095;
	static const int lc1=8388608-4095;
//	static const int lc0=8388608+2047;
//	static const int lc1=8388608-2047;
	int *csy, *csye;
	u32 i0, i1;
	int i;

	csy=ybuf; csye=ybuf+64;
	for(i=0; i<4; i++)
	{
		i0=        BTIC4B_SAT3(((csy[15]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[13]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[11]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 9]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 6]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 4]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 2]-acy)*lsy+lc1)>>21);
		i0=(i0<<3)|BTIC4B_SAT3(((csy[ 0]-acy)*lsy+lc1)>>21);
		*(u32 *)(oblk+i*3)=i0;
		csy+=16;
	}
}

#endif

void BTIC4B_EncBlock0Inner(
	BTIC4B_Context *ctx,
	byte *blkbuf,
	int *ybuf, int *ubuf, int *vbuf,
	int *ubuf2, int *vbuf2,
	int *mcyuv, int *ncyuv)
{
	int aby[16];
	int amcy[4], ancy[4];
	int mbyl, nbyl, mbyr, nbyr, mby, nby;
	int mbyu, nbyu, mbyd, nbyd;
	int dbyh, dbyv, dcy1, dcy2, dbyhvp;
	int dey, dpey, dcenexp, alexp, dcesc;
	int mcy, mcu, mcv, ncy, ncu, ncv;
	int acy, acu, acv, dcy, dcu, dcv;
	int acy2, acu2, acv2;
	int dcy1b, dcu1b, dcv1b;
	int mca, nca, dcuv, bt, fl;
	int ls0, ls0b, ls1, ls2, lc0, lc1, lc2, lc3;
	int l0, l1, l2, l3;
	int i0, i1, i2, i3;
	int i, j, k, l;
	
	mcy=mcyuv[0];	mcu=mcyuv[1];	mcv=mcyuv[2];
	ncy=ncyuv[0];	ncu=ncyuv[1];	ncv=ncyuv[2];

//	mcu=128;	ncu=128;
//	mcv=128;	ncv=128;

	acy=(mcy+ncy)>>1;
	acu=(mcu+ncu)>>1;
	acv=(mcv+ncv)>>1;
	dcy=ncy-mcy;
	dcu=ncu-mcu;
	dcv=ncv-mcv;

	dcenexp=0;

	mca=mcyuv[3];
	nca=ncyuv[3];
	alexp=((mca!=nca)&&(mca<240))||(mca>>8);

#if 0
	if(ubuf2)
	{
		acy2=0; acu2=0; acv2=0;
		for(i=0; i<64; i++)
		{
			acy2+=ybuf[i];
			acu2+=ubuf2[i];
			acv2+=vbuf2[i];
		}
		acy2=acy2>>6;
		acu2=acu2>>6;
		acv2=acv2>>6;
		
		l0=ncy-acy; l1=acy-mcy; l2=(l0+l1)>>1;
		
		acy=(29*acy+3*acy2)>>5;
		acu=(29*acu+3*acu2)>>5;
		acv=(29*acv+3*acv2)>>5;
	}
#endif

#if 1
	dey=0;
	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		k=i*4*8+j*4;
		l0=(ybuf[k+ 0]+ybuf[k+ 1]+ybuf[k+ 8]+ybuf[k+ 9])>>2;
		l1=(ybuf[k+ 2]+ybuf[k+ 3]+ybuf[k+10]+ybuf[k+11])>>2;
		l2=(ybuf[k+16]+ybuf[k+17]+ybuf[k+24]+ybuf[k+25])>>2;
		l3=(ybuf[k+18]+ybuf[k+19]+ybuf[k+26]+ybuf[k+27])>>2;
		
		mby=l0; nby=l0;
		if(l3<mby) { mby=l3; }
		if(l3>nby) { nby=l3; }
		if(l1<mby) { mby=l1; }
		if(l1>nby) { nby=l1; }
		if(l2<mby) { mby=l2; }
		if(l2>nby) { nby=l2; }

#if 1
		lc0=l0-acy;	lc1=l1-acy;
		lc2=l2-acy;	lc3=l3-acy;
//		dey+=lc0*lc0;	dey+=lc1*lc1;
//		dey+=lc2*lc2;	dey+=lc3*lc3;

		dey+=lc0^(lc0>>31);	dey+=lc1^(lc1>>31);
		dey+=lc2^(lc2>>31);	dey+=lc3^(lc3>>31);
#endif

		k=i*2+j;
		amcy[k]=mby;
		ancy[k]=nby;
	}

#if 1
//	if(dcy>8)
	if(1)
//	if((ctx->imgt==BTIC4B_IMGT_LDR8) ||
//		(ctx->imgt==BTIC4B_IMGT_LDR8A))
	{
//		dpey=(dcy*dcy*ctx->qdce_sc)>>4;
//		dpey=(dcy*ctx->qdce_sc)>>4;
		dpey=(dcy*ctx->qdce_sc)>>2;

//		dpey=(dcy>>2)*(dcy>>2)*16;
//		dpey=(dcy>>1)*(dcy>>1)*16;
//		dpey=(dcy>>1)*(dcy>>2)*16;
		dcenexp=dey>dpey;
//		dey=dey/64;
//		dey=dey/(dcy>>1);
//		dcenexp=dey>(dcy>>1);

		if(dcenexp)
		{
//			i=(dcy*7)>>4; i=i*i*16;
//			i=dcy>>1; i=i*i*16;
			i=dcy>>1; i=i*16;
//			dcesc=(224*dey)/(i+1);
//			dcesc=(240*dey)/(i+1);
//			dcesc=(256*dey)/(i+1);
//			dcesc=(288*dey)/(i+1);
			dcesc=(320*dey)/(i+1);
//			dcesc=(384*dey)/(i+1);
//			dcesc=(350*dey)/(i+1);
			if(dcesc>256)
				dcesc=256;
		}
	}
#endif
	

	mbyl=amcy[0];	nbyl=ancy[0];
	mbyr=amcy[1];	nbyr=ancy[1];
	if(amcy[2]<mbyl) { mbyl=amcy[2]; }
	if(ancy[2]>nbyl) { nbyl=ancy[2]; }
	if(amcy[3]<mbyr) { mbyr=amcy[3]; }
	if(ancy[3]>nbyr) { nbyr=ancy[3]; }

	mbyu=amcy[0];	nbyu=ancy[0];
	mbyd=amcy[2];	nbyd=ancy[2];
	if(amcy[1]<mbyu) { mbyu=amcy[1]; }
	if(ancy[1]>nbyu) { nbyu=ancy[1]; }
	if(amcy[3]<mbyd) { mbyd=amcy[3]; }
	if(ancy[3]>nbyd) { nbyd=ancy[3]; }

	dbyh=nbyu-mbyu;	dbyv=nbyl-mbyl;
	j=nbyd-mbyd;	k=nbyr-mbyr;
	if(j>dbyh)dbyh=j;
	if(k>dbyv)dbyv=k;

	dcy1=(dcy*3)>>2;
//	dcy1=(dcy*7)>>3;
	dbyhvp=(dbyh<dcy1)|(dbyv<dcy1);
	dcy2=(dcy*5)>>3;
#endif

//	*(u32 *)(blkbuf+ 0)=  0x0B;
//	dcenexp=1;
//	dcenexp=0;

	if(dcenexp)
	{
//		k=1;
//		dcy=(dcy*3)>>2;
//		dcu=(dcu*3)>>2;
//		dcv=(dcv*3)>>2;
//		dcy=(dcy*7)>>3;
//		dcu=(dcu*7)>>3;
//		dcv=(dcv*7)>>3;

		dcy1b=(dcy*dcesc)>>8;
//		dcu1b=(dcu*dcesc)>>8;
//		dcv1b=(dcv*dcesc)>>8;
//		dcy1b=dcy;
		dcu1b=dcu;
		dcv1b=dcv;
	}

	*(s16 *)(blkbuf+ 4)=lqtvq_clamp32767S(acy);
	*(s16 *)(blkbuf+ 6)=lqtvq_clamp32767S(acu);
	*(s16 *)(blkbuf+ 8)=lqtvq_clamp32767S(acv);
	*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy);
	*(s16 *)(blkbuf+12)=lqtvq_clamp32767S(dcu);
	*(s16 *)(blkbuf+14)=lqtvq_clamp32767S(dcv);

	if(((ncy-acy)|(ncu-acu)|(ncv-acv))>>8)
	{
		ls0=8388608/(ncy-acy+1);
		ls1=8388608/(ncu-acu+1);
		ls2=8388608/(ncv-acv+1);
	}else
	{
		ls0=lqtvq_fsctab[ncy-acy];
		ls1=lqtvq_fsctab[ncu-acu];
		ls2=lqtvq_fsctab[ncv-acv];
	}
	
//	ls0=(ls0*248)>>8;
//	ls0b=(ls0*248)>>8;
//	ls0b=(ls0*240)>>8;
	ls0b=ls0;
	ls0=(ls0*231)>>8;
//	ls1=(ls1*248)>>8;
//	ls2=(ls2*248)>>8;
	ls1=(ls1*230)>>8;
	ls2=(ls2*230)>>8;
	
	lc0=8388608+4095;
	lc1=8388608-4095;

	lc2=8388608+4095;
	lc3=8388608-4095;

#if 1
	bt=-1;
	dcuv=dcu|dcv;
	
#if 0
	if(ctx->qfl&BTIC4B_QFL_USEBFQ)
	{
		fl=0;
		if(dbyhvp)fl|=1;
		if(dcenexp)fl|=2;
		if(alexp)fl|=4;

		for(i=0; i<32; i++)
		{
			if(dcy>ctx->bfq_qdy[i])
				continue;
			if(dcuv>ctx->bfq_qduv[i])
				continue;
			if((fl&ctx->bfq_rqfl[i])!=ctx->bfq_rqfl[i])
				continue;
			if((fl&ctx->bfq_exfl[i])!=0)
				continue;
			if(ctx->bfq_cost[i]>=200)
				continue;
			if((bt<0) || (ctx->bfq_cost[i]<=ctx->bfq_cost[bt]))
				bt=i;
//			if(bt>=0)break;
		}

//		if(bt<0)bt=0x19;
		if(bt<0)bt=0x11;
	}
#endif	

	if(dcuv>ctx->qduv_flat)
	{
		if((dcuv<ctx->qduv_2x2) || (ctx->qfl&BTIC4B_QFL_OPTBCN))
		{
			if(dcy<ctx->qdy_2x2x2)
				{ if(bt<0)bt=0x14; }
			if(dcy<ctx->qdy_4x4x2)
				{ if(bt<0)bt=0x15; }
			if((dcy<ctx->qdy_8x8x2) || dcenexp)
				{ if(bt<0)bt=0x17; }
		}else
		{
			if((dcu>dcy) || (dcv>dcy))
			{
				if((dcuv<ctx->qdy_4x4x2) ||
					((dcy<ctx->qdy_4x4x2) && alexp))
						{ if(bt<0)bt=0x16; }
				if((dcy<ctx->qdy_8x8x2) && !alexp && ubuf2)
					{ if(bt<0)bt=0x1B; }
				if(!alexp && ubuf2)
				{
//					if(bt<0)bt=0x19;
					if(bt<0)bt=0x1E;
				}
			}
			if(dcy<ctx->qdy_4x4x2)
				{ if(bt<0)bt=0x16; }
		}
		
		if(ctx->qfl&BTIC4B_QFL_OPTBCN)
			{ if(bt<0)bt=0x17; }
		
		if((dcy<ctx->qdy_8x8x2) || dcenexp)
			{ if(bt<0)bt=0x18; }
		
		if(1)
			{ if(bt<0)bt=0x1C; }
		if(bt<0)bt=0x1D;
	}else
	{
		if(dcy<ctx->qdy_flat)
			{ if(bt<0)bt=0x00; }
		if(dcy<ctx->qdy_2x2x2)
		{
			if(dcenexp)
				{ if(bt<0)bt=0x01; }
			if(bt<0)bt=0x09;
		}
		
		if(dbyhvp && (dcy1<=ctx->qdy_2x2x2))
		{
			if(dbyv<=dbyh)
			{
				if(dcenexp)
					{ if(bt<0)bt=0x02; }
				if(bt<0)bt=0x0A;
			}else
			{
				if(dcenexp)
					{ if(bt<0)bt=0x03; }
				if(bt<0)bt=0x0B;
			}
		}

		if(dcy<ctx->qdy_4x4x2)
		{
			if(dcenexp)
				{ if(bt<0)bt=0x04; }
			if(bt<0)bt=0x0C;
		}

		if(dbyhvp && (dcy2<=ctx->qdy_4x4x2))
		{
			if(dbyv<=dbyh)
			{
				if(dcenexp)
					{ if(bt<0)bt=0x05; }
				if(bt<0)bt=0x0D;
			}else
			{
				if(dcenexp)
					{ if(bt<0)bt=0x06; }
				if(bt<0)bt=0x0E;
			}
		}

		if(dcy<ctx->qdy_8x8x2)
		{
			if(dcenexp)
				{ if(bt<0)bt=0x07; }
			if(bt<0)bt=0x0F;
		}

		if(dcenexp)
			{ if(bt<0)bt=0x0F; }

		if((dcy<ctx->qdy_8x8x3) ||
			((ctx->qfl&127)<80))
				{ if(bt<0)bt=0x10; }
		if(bt<0)bt=0x11;
	}
#endif

#if 1
	switch(bt)
	{
	case 0x00:
		*(u32 *)(blkbuf+ 0)=  0x00|0x00FF0000;
		break;
	case 0x01:
		*(u32 *)(blkbuf+ 0)=  0x01|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits2x2x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x02:
		*(u32 *)(blkbuf+ 0)=  0x02|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits4x2x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x03:
		*(u32 *)(blkbuf+ 0)=  0x03|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits2x4x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x04:
		*(u32 *)(blkbuf+ 0)=  0x04|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits4x4x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x05:
		*(u32 *)(blkbuf+ 0)=  0x05|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits8x4x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x06:
		*(u32 *)(blkbuf+ 0)=  0x06|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits4x8x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x07:
		*(u32 *)(blkbuf+ 0)=  0x07|0x00FF0000;
		*(s16 *)(blkbuf+10)=lqtvq_clamp32767S(dcy1b);
		BTIC4B_EncYBits8x8x1(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x08:
		break;
	case 0x09:
		*(u32 *)(blkbuf+ 0)=  0x09|0x00FF0000;
		BTIC4B_EncYBits2x2x2(blkbuf+16, ybuf, ls0, acy);
		return;
	case 0x0A:
		*(u32 *)(blkbuf+ 0)=  0x0A|0x00FF0000;
		BTIC4B_EncYBits4x2x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x0B:
		*(u32 *)(blkbuf+ 0)=  0x0B|0x00FF0000;
		BTIC4B_EncYBits2x4x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x0C:
		*(u32 *)(blkbuf+ 0)=  0x0C|0x00FF0000;
		BTIC4B_EncYBits4x4x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x0D:
		*(u32 *)(blkbuf+ 0)=  0x0D|0x00FF0000;
		BTIC4B_EncYBits8x4x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x0E:
		*(u32 *)(blkbuf+ 0)=  0x0E|0x00FF0000;
		BTIC4B_EncYBits4x8x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x0F:
		*(u32 *)(blkbuf+ 0)=  0x0F|0x00FF0000;
		BTIC4B_EncYBits8x8x2(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x10:
		*(u32 *)(blkbuf+ 0)=  0x10|0x00FF0000;
		BTIC4B_EncYBits8x8x3(blkbuf+16, ybuf, ls0, acy);
		break;
	case 0x11:
		*(u32 *)(blkbuf+ 0)=  0x11|0x00FF0000;
		BTIC4B_EncYBits8x8x4(blkbuf+16, ybuf, ls0b, acy);
		break;
	case 0x12:
		break;
	case 0x13:
		blkbuf[0]=0x13;
		blkbuf[1]=0x00;
		blkbuf[2]=acy;
		blkbuf[3]=lqtvq_clamp255((acu>>1)+128);
		blkbuf[4]=lqtvq_clamp255((acv>>1)+128);
		blkbuf[5]=dcy;
		blkbuf[6]=lqtvq_clamp255((dcu>>1)+128);
		blkbuf[7]=lqtvq_clamp255((dcv>>1)+128);
		BTIC4B_EncYBits8x8x2(blkbuf+8, ybuf, ls0, acy);
		BTIC4B_EncUVBits4x4x2(blkbuf+24, ubuf2, ls1, acu);
		BTIC4B_EncUVBits4x4x2(blkbuf+28, vbuf2, ls2, acv);
		break;
	case 0x14:
		*(u32 *)(blkbuf+ 0)=  0x14|0x00FF0000;
		BTIC4B_EncYBits2x2x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits2x2x1(blkbuf+18, ubuf, ls1, acu);
		BTIC4B_EncUVBits2x2x1(blkbuf+19, vbuf, ls2, acv);
		blkbuf[17]=blkbuf[18]|(blkbuf[19]<<4);
		break;
	case 0x15:
		*(u32 *)(blkbuf+ 0)=  0x15|0x00FF0000;
		BTIC4B_EncYBits4x4x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits2x2x2(blkbuf+20, ubuf, ls1, acu);
		BTIC4B_EncUVBits2x2x2(blkbuf+21, vbuf, ls2, acv);
		break;
	case 0x16:
		*(u32 *)(blkbuf+ 0)=  0x16|0x00FF0000;
		BTIC4B_EncYBits4x4x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits4x4x2(blkbuf+20, ubuf, ls1, acu);
		BTIC4B_EncUVBits4x4x2(blkbuf+24, vbuf, ls2, acv);
		break;
	case 0x17:
		*(u32 *)(blkbuf+ 0)=  0x17|0x00FF0000;
		BTIC4B_EncYBits8x8x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits2x2x2(blkbuf+32, ubuf, ls1, acu);
		BTIC4B_EncUVBits2x2x2(blkbuf+33, vbuf, ls2, acv);
		break;
	case 0x18:
		*(u32 *)(blkbuf+ 0)=  0x18|0x00FF0000;
		BTIC4B_EncYBits8x8x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits4x4x2(blkbuf+32, ubuf, ls1, acu);
		BTIC4B_EncUVBits4x4x2(blkbuf+36, vbuf, ls2, acv);
		break;
	case 0x19:
		blkbuf[0]=0x19;
		blkbuf[1]=0x00;
		blkbuf[2]=acy;
		blkbuf[3]=lqtvq_clamp255((acu>>1)+128);
		blkbuf[4]=lqtvq_clamp255((acv>>1)+128);
		blkbuf[5]=dcy;
		blkbuf[6]=lqtvq_clamp255((dcu>>1)+128);
		blkbuf[7]=lqtvq_clamp255((dcv>>1)+128);
		BTIC4B_EncYBits8x8x3(blkbuf+8, ybuf, ls0, acy);
		BTIC4B_EncYBits8x8x2(blkbuf+32, ubuf2, ls1, acu);
		BTIC4B_EncYBits8x8x2(blkbuf+48, vbuf2, ls2, acv);
		break;
	case 0x1A:
		*(u32 *)(blkbuf+ 0)=  0x1A|0x00FF0000;
		BTIC4B_EncYBits8x8x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncYBits4x8x2EO(blkbuf+32, ubuf2, ls1, acu);
		BTIC4B_EncYBits4x8x2EO(blkbuf+40, vbuf2, ls2, acv);
		break;
	case 0x1B:
		*(u32 *)(blkbuf+ 0)=  0x1B|0x00FF0000;
		BTIC4B_EncYBits8x8x2(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncYBits8x8x2(blkbuf+32, ubuf2, ls1, acu);
		BTIC4B_EncYBits8x8x2(blkbuf+48, vbuf2, ls2, acv);
		break;
	case 0x1C:
		*(u32 *)(blkbuf+ 0)=  0x1C|0x00FF0000;
		BTIC4B_EncYBits8x8x3(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits4x4x2(blkbuf+40, ubuf, ls1, acu);
		BTIC4B_EncUVBits4x4x2(blkbuf+44, vbuf, ls2, acv);
		break;
	case 0x1D:
		*(u32 *)(blkbuf+ 0)=  0x1D|0x00FF0000;
		BTIC4B_EncYBits8x8x4(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncUVBits4x4x4(blkbuf+48, ubuf, ls1, acu);
		BTIC4B_EncUVBits4x4x4(blkbuf+56, vbuf, ls2, acv);
		break;
	case 0x1E:
		*(u32 *)(blkbuf+ 0)=  0x1E|0x00FF0000;
		BTIC4B_EncYBits8x8x3(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncYBits4x8x3EO(blkbuf+40, ubuf2, ls1, acu);
		BTIC4B_EncYBits4x8x3EO(blkbuf+52, vbuf2, ls2, acv);
		break;
	case 0x1F:
		*(u32 *)(blkbuf+ 0)=  0x1F|0x00FF0000;
		BTIC4B_EncYBits8x8x4(blkbuf+16, ybuf, ls0, acy);
		BTIC4B_EncYBits8x8x3(blkbuf+48, ubuf2, ls1, acu);
		BTIC4B_EncYBits8x8x3(blkbuf+72, vbuf2, ls2, acv);
		break;
	default:
		break;
	}
#endif

}

void BTIC4B_EncBlockAInner(
	BTIC4B_Context *ctx,
	byte *blkbuf, int *abuf, int mca, int nca)
{
	int l0, l1, l2, l3;
	int lc0, lc1, lc2, lc3;
	int aca, dca;
	int ls0, dey, dcenexp;
	int i, j, k;

	if(mca==255)
		return;
	
	if((blkbuf[0]&0x1F)==0x19)
		return;
	
	if((mca==nca) && (mca>=0) && (mca<=255))
	{
		blkbuf[1]=0;
		blkbuf[2]=mca;
		blkbuf[3]=0;
		return;
	}

	if((mca>=240) && (nca<=255))
	{
		blkbuf[1]=0;
		blkbuf[2]=255;
		blkbuf[3]=0;
		return;
	}

	aca=(mca+nca)>>1;
	dca=nca-mca;

	if(dca<ctx->qdy_flat)
	{
		if((aca>=0) && (aca<=255))
		{
			blkbuf[1]=0;
			blkbuf[2]=aca;
			blkbuf[3]=0;
			return;
		}
		blkbuf[1]=8;	blkbuf[2]=0;	blkbuf[3]=1;
		*(u16 *)(blkbuf+48+0)=aca;
		*(u16 *)(blkbuf+48+2)=0;
		return;
	}


	dey=0;
	for(i=0; i<2; i++)
		for(j=0; j<2; j++)
	{
		k=i*4*8+j*4;
		l0=(abuf[k+ 0]+abuf[k+ 1]+abuf[k+ 8]+abuf[k+ 9])>>2;
		l1=(abuf[k+ 2]+abuf[k+ 3]+abuf[k+10]+abuf[k+11])>>2;
		l2=(abuf[k+16]+abuf[k+17]+abuf[k+24]+abuf[k+25])>>2;
		l3=(abuf[k+18]+abuf[k+19]+abuf[k+26]+abuf[k+27])>>2;

		lc0=l0-aca;	lc1=l1-aca;
		lc2=l2-aca;	lc3=l3-aca;
		dey+=lc0*lc0;	dey+=lc1*lc1;
		dey+=lc2*lc2;	dey+=lc3*lc3;
	}

#if 1
//	if(dcy>8)
	if(1)
	{
		i=(dca*dca*ctx->qdce_sc)>>4;
		dcenexp=dey>i;
	}
#endif

	blkbuf[1]=8;
	blkbuf[2]=0;
	blkbuf[3]=1;
	*(u16 *)(blkbuf+48+0)=aca;
	*(u16 *)(blkbuf+48+2)=dca;

	if((nca-aca)>>8)
		{ ls0=8388608/(nca-aca+1); }
	else
		{ ls0=lqtvq_fsctab[nca-aca]; }

#if 1
	if(dca<ctx->qdy_2x2x2)
	{
		if(dcenexp)
		{
			blkbuf[1]=0x01;
			BTIC4B_EncYBits2x2x1(blkbuf+48+8, abuf, ls0, aca);
			return;
		}

		blkbuf[1]=0x04;
		BTIC4B_EncYBits2x2x2(blkbuf+48+8, abuf, ls0, aca);
		return;
	}
	
	if(dca<ctx->qdy_4x4x2)
	{
		if(dcenexp)
		{
			blkbuf[1]=0x02;
			BTIC4B_EncYBits4x4x1(blkbuf+48+8, abuf, ls0, aca);
			return;
		}

		blkbuf[1]=0x05;
		BTIC4B_EncYBits4x4x2(blkbuf+48+8, abuf, ls0, aca);
		return;
	}
#endif

//	if(1)
	if(dcenexp)
	{
		blkbuf[1]=0x03;
		BTIC4B_EncYBits8x8x1(blkbuf+48+8, abuf, ls0, aca);
		return;
	}

	blkbuf[1]=0x0C;
	blkbuf[2]=aca;
	blkbuf[3]=dca;
	BTIC4B_EncYBits8x8x2(blkbuf+48, abuf, ls0, aca);
	return;
}

void BTIC4B_EncBlockBGRA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64], abuf[64];
	int ubuf2[16], vbuf2[16];
	int mcyuv[4], ncyuv[4];
	int i0, i1;
	int cu, cv;
	int i, j, k;

#if 1
	BTIC4B_SplitIbufBGRA444(ctx, ibuf, ystr,
		ybuf, ubuf, vbuf, abuf,
		mcyuv, ncyuv);

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		i0=i*16+j*2;
		i1=i*4+j;
		
		cu=(ubuf[i0+0]+ubuf[i0+1]+ubuf[i0+8]+ubuf[i0+9])>>2;
		cv=(vbuf[i0+0]+vbuf[i0+1]+vbuf[i0+8]+vbuf[i0+9])>>2;
		ubuf2[i1]=cu;
		vbuf2[i1]=cv;
	}

	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf2, vbuf2, ubuf, vbuf,
		mcyuv, ncyuv);

//	BTIC4B_EncBlock0Inner(ctx, blkbuf,
//		ybuf, ubuf2, vbuf2, NULL, vbuf,
//		mcyuv, ncyuv);
#endif
	
#if 0
	BTIC4B_SplitIbufBGRA(ibuf, ystr,
		ybuf, ubuf, vbuf, abuf,
		mcyuv, ncyuv);
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
#endif

	BTIC4B_EncBlockAInner(ctx, blkbuf, abuf, mcyuv[3], ncyuv[3]);
}

void BTIC4B_EncBlockBGRX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[16], vbuf[16];
	int mcyuv[4], ncyuv[4];
	
	BTIC4B_SplitIbufBGRX(ibuf, ystr,
		ybuf, ubuf, vbuf,
		mcyuv, ncyuv);
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
}

void BTIC4B_EncBlockRGBA(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64], abuf[64];
	int ubuf2[16], vbuf2[16];
	int mcyuv[4], ncyuv[4];
	int i0, i1;
	int cu, cv;
	int i, j, k;

#if 1
	BTIC4B_SplitIbufBGRA444(ctx, ibuf, ystr,
		ybuf, vbuf, ubuf, abuf,
		mcyuv, ncyuv);

	i0=mcyuv[1]; i1=mcyuv[2];	mcyuv[1]=i1; mcyuv[2]=i0;
	i0=ncyuv[1]; i1=ncyuv[2];	ncyuv[1]=i1; ncyuv[2]=i0;
//	i0=mcyuv[4]; i1=mcyuv[5];	mcyuv[4]=i1; mcyuv[5]=i0;
//	i0=ncyuv[4]; i1=ncyuv[5];	ncyuv[4]=i1; ncyuv[5]=i0;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		i0=i*16+j*2;
		i1=i*4+j;
		
		cu=(ubuf[i0+0]+ubuf[i0+1]+ubuf[i0+8]+ubuf[i0+9])>>2;
		cv=(vbuf[i0+0]+vbuf[i0+1]+vbuf[i0+8]+vbuf[i0+9])>>2;
		ubuf2[i1]=cu;
		vbuf2[i1]=cv;
	}

	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf2, vbuf2, ubuf, vbuf,
		mcyuv, ncyuv);

	BTIC4B_EncBlockAInner(ctx, blkbuf, abuf, mcyuv[3], ncyuv[3]);
#endif

#if 0
	byte tbuf[128];
	int cu, cv, du, dv;
	int i, j;

	BTIC4B_EncBlockBGRA(ctx, tbuf, ibuf, ystr);
	cu=*(s16 *)(tbuf+ 6);
	cv=*(s16 *)(tbuf+ 8);
	du=*(s16 *)(tbuf+12);
	dv=*(s16 *)(tbuf+14);
	*(s16 *)(tbuf+ 6)=cv;
	*(s16 *)(tbuf+ 8)=cu;
	*(s16 *)(tbuf+12)=dv;
	*(s16 *)(tbuf+14)=du;
	memcpy(blkbuf, tbuf, 64);
#endif
}

void BTIC4B_EncBlockRGBX(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[16], vbuf[16];
	int mcyuv[4], ncyuv[4];
	int i0, i1;
	
	BTIC4B_SplitIbufBGRX(ibuf, ystr,
		ybuf, vbuf, ubuf,
		mcyuv, ncyuv);
	i0=mcyuv[1]; i1=mcyuv[2];	mcyuv[1]=i1; mcyuv[2]=i0;
	i0=ncyuv[1]; i1=ncyuv[2];	ncyuv[1]=i1; ncyuv[2]=i0;
//	i0=mcyuv[4]; i1=mcyuv[5];	mcyuv[4]=i1; mcyuv[5]=i0;
//	i0=ncyuv[4]; i1=ncyuv[5];	ncyuv[4]=i1; ncyuv[5]=i0;

	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);

#if 0
	byte tbuf[128];
	int cu, cv, du, dv;
	int i, j;

	BTIC4B_EncBlockBGRX(ctx, tbuf, ibuf, ystr);
	cu=*(s16 *)(tbuf+ 6);
	cv=*(s16 *)(tbuf+ 8);
	du=*(s16 *)(tbuf+12);
	dv=*(s16 *)(tbuf+14);
	*(s16 *)(tbuf+ 6)=cv;
	*(s16 *)(tbuf+ 8)=cu;
	*(s16 *)(tbuf+12)=dv;
	*(s16 *)(tbuf+14)=du;
	memcpy(blkbuf, tbuf, 64);
#endif
}

void BTIC4B_EncBlockBGR(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[16], vbuf[16];
	int mcyuv[4], ncyuv[4];
	int i0, i1;
	
	BTIC4B_SplitIbufBGR(ibuf, ystr,
		ybuf, ubuf, vbuf,
		mcyuv, ncyuv);
//	i0=mcyuv[1]; i1=mcyuv[2];	mcyuv[1]=i1; mcyuv[2]=i0;
//	i0=ncyuv[1]; i1=ncyuv[2];	ncyuv[1]=i1; ncyuv[2]=i0;
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
}

void BTIC4B_EncBlockRGB(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[16], vbuf[16];
	int mcyuv[4], ncyuv[4];
	int i0, i1;
	
	BTIC4B_SplitIbufBGR(ibuf, ystr,
		ybuf, vbuf, ubuf,
		mcyuv, ncyuv);
	i0=mcyuv[1]; i1=mcyuv[2];	mcyuv[1]=i1; mcyuv[2]=i0;
	i0=ncyuv[1]; i1=ncyuv[2];	ncyuv[1]=i1; ncyuv[2]=i0;
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
}

void BTIC4B_EncBlockRGB_12R11F(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64];
	int ubuf2[16], vbuf2[16];
	int mcyuv[4], ncyuv[4];
	int cu, cv;
	int i0, i1;
	int i, j, k;

#if 1
	BTIC4B_SplitIbufRGB444_12R11F(ctx, ibuf, ystr,
		ybuf, ubuf, vbuf, mcyuv, ncyuv);

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		i0=i*16+j*2;
		i1=i*4+j;
		
		cu=(ubuf[i0+0]+ubuf[i0+1]+ubuf[i0+8]+ubuf[i0+9])>>2;
		cv=(vbuf[i0+0]+vbuf[i0+1]+vbuf[i0+8]+vbuf[i0+9])>>2;
		ubuf2[i1]=cu;
		vbuf2[i1]=cv;
	}

	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf2, vbuf2, ubuf, vbuf,
		mcyuv, ncyuv);
#endif

#if 0	
	BTIC4B_SplitIbufRGB_12R11F(ibuf, ystr,
		ybuf, ubuf, vbuf, mcyuv, ncyuv);
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);

//	mcyuv[1]=mcyuv[2]; ncyuv[1]=ncyuv[2];
//	BTIC4B_EncBlock0Inner(ctx, blkbuf,
//		ybuf, vbuf, vbuf, NULL, NULL,
//		mcyuv, ncyuv);
#endif
}

void BTIC4B_EncBlockRGB_16R11F(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64];
	int mcyuv[4], ncyuv[4];
	
	BTIC4B_SplitIbufRGB_16R11F(ibuf, ystr,
		ybuf, ubuf, vbuf, mcyuv, ncyuv);
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
}

void BTIC4B_EncBlockRGB48F(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64];
	int mcyuv[4], ncyuv[4];
	
	BTIC4B_SplitIbufRGB48(ibuf, ystr,
		ybuf, ubuf, vbuf, mcyuv, ncyuv);
	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf, vbuf, NULL, NULL,
		mcyuv, ncyuv);
}

void BTIC4B_EncBlockRGB_16RGB8E8(BTIC4B_Context *ctx,
	byte *blkbuf, byte *ibuf, int ystr)
{
	int ybuf[64], ubuf[64], vbuf[64];
	int ubuf2[16], vbuf2[16];
	int mcyuv[4], ncyuv[4];
	int cu, cv;
	int i0, i1;
	int i, j, k;

	BTIC4B_SplitIbufRGB444_16RGB8E8(ctx, ibuf, ystr,
		ybuf, ubuf, vbuf, mcyuv, ncyuv);

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		i0=i*16+j*2;
		i1=i*4+j;
		
		cu=(ubuf[i0+0]+ubuf[i0+1]+ubuf[i0+8]+ubuf[i0+9])>>2;
		cv=(vbuf[i0+0]+vbuf[i0+1]+vbuf[i0+8]+vbuf[i0+9])>>2;
		ubuf2[i1]=cu;
		vbuf2[i1]=cv;
	}

	BTIC4B_EncBlock0Inner(ctx, blkbuf,
		ybuf, ubuf2, vbuf2, ubuf, vbuf,
		mcyuv, ncyuv);
}

#if 1
void BTIC4B_EncImageBGRA(BTIC4B_Context *ctx,
	byte *blks, byte *ibuf, int xs, int ys)
{
	byte *cs, *ct;
	int xs1, ys1, ystr;
	int bi;
	int i, j, k;
	
	BTIC4B_InitScTables();
	
	ystr=xs*4;
	xs1=xs>>3;
	ys1=ys>>3;
	
	for(i=0; i<ys1; i++)
	{
		cs=ibuf+(i*8)*ystr;
		ct=blks+((i*xs1)<<6);
		for(j=0; j<xs1; j++)
		{
			BTIC4B_EncBlockBGRA(ctx, ct, cs, ystr);
			ct+=64; cs+=32;
		}
	}
}
#endif

void BTIC4B_EncImageClrs(BTIC4B_Context *ctx,
	byte *blks, byte *ibuf, int xs, int ys, int clrs)
{
	void (*EncBlock)(BTIC4B_Context *ctx,
		byte *blkbuf, byte *ibuf, int ystr);
	byte *cs, *ct, *ibuf1;
	int xs1, ys1, xs2, ys2, xf, yf, xstr, ystr;
	int bi, vf;
	int i, j, k;
	
	BTIC4B_InitScTables();
	
	switch(clrs&127)
	{
	case BTIC4B_CLRS_RGBA:
		EncBlock=BTIC4B_EncBlockRGBA;
		xstr=4;
		break;
	case BTIC4B_CLRS_BGRA:
		EncBlock=BTIC4B_EncBlockBGRA;
		xstr=4;
		break;
	case BTIC4B_CLRS_RGBX:
		EncBlock=BTIC4B_EncBlockRGBX;
		xstr=4;
		break;
	case BTIC4B_CLRS_BGRX:
		EncBlock=BTIC4B_EncBlockBGRX;
		xstr=4;
		break;
	case BTIC4B_CLRS_RGB:
		EncBlock=BTIC4B_EncBlockRGB;
		xstr=3;
		break;
	case BTIC4B_CLRS_BGR:
		EncBlock=BTIC4B_EncBlockBGR;
		xstr=3;
		break;

	case BTIC4B_CLRS_RGB11F:
		if(ctx->imgt==BTIC4B_IMGT_HDR12)
			{ EncBlock=BTIC4B_EncBlockRGB_12R11F; }
		else
			{ EncBlock=BTIC4B_EncBlockRGB_16R11F; }
		xstr=4;
		break;
	case BTIC4B_CLRS_RGB48F:
		EncBlock=BTIC4B_EncBlockRGB48F;
		xstr=6;
		break;
	case BTIC4B_CLRS_RGB8E8:
		EncBlock=BTIC4B_EncBlockRGB_16RGB8E8;
		xstr=4;
		break;

	default:
		EncBlock=BTIC4B_EncBlockRGBA;
		xstr=4;
		break;
	}

	vf=ctx->flip;
	if(ys<0)
	{
		ys=-ys;
		vf=!vf;
	}

	ystr=xs*xstr;
	ystr=(ystr+3)&(~3);
	
	xs1=xs>>3;
	ys1=ys>>3;
	xf=xs&7;
	yf=ys&7;

	xs2=(xs+7)>>3;
	ys2=(ys+7)>>3;
		
	if(vf)
	{
		ibuf1=ibuf+(ys-1)*ystr;
		ystr=-ystr;
	}else
	{
		ibuf1=ibuf;
	}
	
	for(i=0; i<ys1; i++)
	{
		cs=ibuf1+(i*8)*ystr;
//		ct=blks+((i*xs2)<<6);
		ct=blks+((i*xs2)*ctx->blksz);
		for(j=0; j<xs1; j++)
		{
			EncBlock(ctx, ct, cs, ystr);
//			ct+=64;
//			cs+=32;
			ct+=ctx->blksz;
			cs+=8*xstr;
		}
		if(xf)
		{
			if((i>0) && ((i+1)<ys2))
				{ EncBlock(ctx, ct, cs, ystr); }
			else
				{ memset(ct, 0, 16); }
//			ct+=64;
			ct+=ctx->blksz;
			cs+=xf*xstr;
		}
	}
	if(yf)
	{
		for(j=0; j<xs2; j++)
		{
//			EncBlock(ctx, ct, cs, ystr);
//			ct+=64;
//			cs+=32;
			memset(ct, 0, 16);
			ct+=ctx->blksz;
			cs+=8*xstr;
		}
	}
}
