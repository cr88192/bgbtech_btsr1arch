#include <bgbrasw.h>

void BGBRASW_UnpackYYPixelComponents(
	bgbrasw_yypixel pix,
	int *rr0, int *rg0, int *rb0, int *ra0,
	int *rr1, int *rg1, int *rb1, int *ra1,
	int *rr2, int *rg2, int *rb2, int *ra2,
	int *rr3, int *rg3, int *rb3, int *ra3)
{
	int cy0, cy1, cy2, cy3, cu, cv, ca, cx;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr2, cg2, cb2, cr3, cg3, cb3;
	int k;

	cy0=BGBRASW_YYPIXEL_Y0(pix);
	cy1=BGBRASW_YYPIXEL_Y1(pix);
	cy2=BGBRASW_YYPIXEL_Y2(pix);
	cy3=BGBRASW_YYPIXEL_Y3(pix);
	cu=BGBRASW_YYPIXEL_U(pix);
	cv=BGBRASW_YYPIXEL_V(pix);
	ca=BGBRASW_YYPIXEL_A(pix);
	cx=BGBRASW_YYPIXEL_X2(pix);

	cy0|=cx;	cy1|=cx;
	cy2|=cx;	cy3|=cx;

//	cu=(cu-128)<<1;	cv=(cv-128)<<1;
	cu=(cu-128);	cv=(cv-128);
	k=(cu+cv)>>2;
	cg0=cy0-k;	cb0=cg0+cu;	cr0=cg0+cv;
	cg1=cy1-k;	cb1=cg1+cu;	cr1=cg1+cv;
	cg2=cy2-k;	cb2=cg2+cu;	cr2=cg2+cv;
	cg3=cy3-k;	cb3=cg3+cu;	cr3=cg3+cv;

	if((cr0|cr1|cr2|cr3)&(~255))
	{
		cr0=(cr0<0)?0:((cr0>255)?255:cr0);
		cr1=(cr1<0)?0:((cr1>255)?255:cr1);
		cr2=(cr2<0)?0:((cr2>255)?255:cr2);
		cr3=(cr3<0)?0:((cr3>255)?255:cr3);
	}
	if((cg0|cg1|cg2|cg3)&(~255))
	{
		cg0=(cg0<0)?0:((cg0>255)?255:cg0);
		cg1=(cg1<0)?0:((cg1>255)?255:cg1);
		cg2=(cg2<0)?0:((cg2>255)?255:cg2);
		cg3=(cg3<0)?0:((cg3>255)?255:cg3);
	}
	if((cb0|cb1|cb2|cb3)&(~255))
	{
		cb0=(cb0<0)?0:((cb0>255)?255:cb0);
		cb1=(cb1<0)?0:((cb1>255)?255:cb1);
		cb2=(cb2<0)?0:((cb2>255)?255:cb2);
		cb3=(cb3<0)?0:((cb3>255)?255:cb3);
	}
	*rr0=cr0;	*rg0=cg0;	*rb0=cb0;	*ra0=ca;
	*rr1=cr1;	*rg1=cg1;	*rb1=cb1;	*ra1=ca;
	*rr2=cr2;	*rg2=cg2;	*rb2=cb2;	*ra2=ca;
	*rr3=cr3;	*rg3=cg3;	*rb3=cb3;	*ra3=ca;
}

bgbrasw_yypixel BGBRASW_PackYYPixelComponents(
	int cr0, int cg0, int cb0, int ca0,
	int cr1, int cg1, int cb1, int ca1,
	int cr2, int cg2, int cb2, int ca2,
	int cr3, int cg3, int cb3, int ca3)
{
	bgbrasw_yypixel ypix;
	int cy0, cu0, cv0, cy1, cu1, cv1;
	int cy2, cu2, cv2, cy3, cu3, cv3;
	int cr, cg, cb, ca;
	int cy, cu, cv, cx;
	
	cy0=(cr0+2*cg0+cb0)>>2;
	cy1=(cr1+2*cg1+cb1)>>2;
	cy2=(cr2+2*cg2+cb2)>>2;
	cy3=(cr3+2*cg3+cb3)>>2;
//	cu0=((cb0-cg0)>>1)+128;	cv0=((cr0-cg0)>>1)+128;
//	cu1=((cb1-cg1)>>1)+128;	cv1=((cr1-cg1)>>1)+128;
//	cu2=((cb2-cg2)>>1)+128;	cv2=((cr2-cg2)>>1)+128;
//	cu3=((cb3-cg3)>>1)+128;	cv3=((cr3-cg3)>>1)+128;
	cu0=(cb0-cg0)+128;	cv0=(cr0-cg0)+128;
	cu1=(cb1-cg1)+128;	cv1=(cr1-cg1)+128;
	cu2=(cb2-cg2)+128;	cv2=(cr2-cg2)+128;
	cu3=(cb3-cg3)+128;	cv3=(cr3-cg3)+128;

//	cu0=(cu0<0)?0:((cu0>255)?255:cu0);
//	cv0=(cv0<0)?0:((cv0>255)?255:cv0);
//	cu1=(cu1<0)?0:((cu1>255)?255:cu1);
//	cv1=(cv1<0)?0:((cv1>255)?255:cv1);
//	cu2=(cu2<0)?0:((cu2>255)?255:cu2);
//	cv2=(cv2<0)?0:((cv2>255)?255:cv2);
//	cu3=(cu3<0)?0:((cu3>255)?255:cu3);
//	cv3=(cv3<0)?0:((cv3>255)?255:cv3);
	
	cy=(cy0+cy1+cy2+cy3)>>2;
	cu=(cu0+cu1+cu2+cu3)>>2;
	cv=(cv0+cv1+cv2+cv3)>>2;
	cu+=8; cv+=8;
	cy=(cy<0)?0:((cy>255)?255:cy);
	cu=(cu<0)?0:((cu>255)?255:cu);
	cv=(cv<0)?0:((cv>255)?255:cv);
	cx=cy&15;

	ypix=BGBRASW_MAKEYYPIXEL(cy0, cy1, cy2, cy3, cu, cv, ca, cx);
	return(ypix);
}

void BGBRASW_ConvUnpackYUVAToRGBA(
	bgbrasw_pixel *dst, bgbrasw_yypixel *src,
	int xs, int ys,
	int ysdst, int yssrc)
{
	bgbrasw_pixel *ct0, *ct1;
	bgbrasw_yypixel *cs, *cse;
	bgbrasw_yypixel spix;
	bgbrasw_pixel dpix0, dpix1, dpix2, dpix3;
	int cy0, cy1, cy2, cy3, cu, cv, ca, cx;
	int cr0, cg0, cb0, cr1, cg1, cb1;
	int cr2, cg2, cb2, cr3, cg3, cb3;
	int xs1, ys1;
	int i, j, k;
	
	xs1=xs>>1;
	ys1=ys>>1;
	
	for(i=0; i<ys1; i++)
	{
		cs=src+i*yssrc; cse=cs+xs1;
		ct0=dst+2*i*ysdst; ct1=ct0+ysdst;

		while(cs<cse)
		{
			spix=*cs++;
			cy0=BGBRASW_YYPIXEL_Y0(spix);
			cy1=BGBRASW_YYPIXEL_Y1(spix);
			cy2=BGBRASW_YYPIXEL_Y2(spix);
			cy3=BGBRASW_YYPIXEL_Y3(spix);
			cu=BGBRASW_YYPIXEL_U(spix);
			cv=BGBRASW_YYPIXEL_V(spix);
			ca=BGBRASW_YYPIXEL_A(spix);
			cx=BGBRASW_YYPIXEL_X2(spix);

			cy0|=cx;	cy1|=cx;
			cy2|=cx;	cy3|=cx;

//			cu=(cu-128)<<1;
//			cv=(cv-128)<<1;
			cu=(cu-128);
			cv=(cv-128);
			k=(cu+cv)>>2;
			cg0=cy0-k;	cb0=cg0+cu;	cr0=cg0+cv;
			cg1=cy1-k;	cb1=cg1+cu;	cr1=cg1+cv;
			cg2=cy2-k;	cb2=cg2+cu;	cr2=cg2+cv;
			cg3=cy3-k;	cb3=cg3+cu;	cr3=cg3+cv;

			if((cr0|cr1|cr2|cr3)&(~255))
			{
				cr0=(cr0<0)?0:((cr0>255)?255:cr0);
				cr1=(cr1<0)?0:((cr1>255)?255:cr1);
				cr2=(cr2<0)?0:((cr2>255)?255:cr2);
				cr3=(cr3<0)?0:((cr3>255)?255:cr3);
			}
			if((cg0|cg1|cg2|cg3)&(~255))
			{
				cg0=(cg0<0)?0:((cg0>255)?255:cg0);
				cg1=(cg1<0)?0:((cg1>255)?255:cg1);
				cg2=(cg2<0)?0:((cg2>255)?255:cg2);
				cg3=(cg3<0)?0:((cg3>255)?255:cg3);
			}
			if((cb0|cb1|cb2|cb3)&(~255))
			{
				cb0=(cb0<0)?0:((cb0>255)?255:cb0);
				cb1=(cb1<0)?0:((cb1>255)?255:cb1);
				cb2=(cb2<0)?0:((cb2>255)?255:cb2);
				cb3=(cb3<0)?0:((cb3>255)?255:cb3);
			}

			dpix0=BGBRASW_MAKEPIXEL(cr0, cg0, cb0, ca);
			dpix1=BGBRASW_MAKEPIXEL(cr1, cg1, cb1, ca);
			dpix2=BGBRASW_MAKEPIXEL(cr2, cg2, cb2, ca);
			dpix3=BGBRASW_MAKEPIXEL(cr3, cg3, cb3, ca);
			
			*ct0++=dpix0;	*ct0++=dpix1;
			*ct1++=dpix2;	*ct1++=dpix3;
		}
	}
}
