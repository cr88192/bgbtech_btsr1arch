byte *BGBCC_Img_DecodePPM_ReadNumber(byte *cs, int *rval)
{
	byte tb[32];
	byte *ct, *cte;
	
	while(*cs && (*cs<=' '))
		cs++;
	
	ct=tb; cte=tb+30;
	while((*cs>='0') && (*cs<='9') && (ct<cte))
		*ct++=*cs++;
	*ct++=0;

	if(rval)
		*rval=atoi(tb);

	return(cs);
}

byte *BGBCC_Img_DecodePPM_ReadLine(byte *cs, byte *lbuf)
{
	byte *ct;
	
	while(*cs && (*cs<=' '))
		cs++;
	
	ct=lbuf;
	while(*cs && (*cs!='\r') && (*cs!='\n'))
		*ct++=*cs++;
	*ct=0;
	
	return(cs);
}

byte *BGBCC_Img_DecodePPM(byte *imgbuf, int *rw, int *rh)
{
	byte tb[256];
	byte *ibuf;
	byte *cs, *ct, *cte;
	int cr, cg, cb;
	int xs, ys, mv, pw, asc, mvsc, flip;
	int x, y, y1;
	
	cs=imgbuf;
	if(*cs!='P')
		return(NULL);
	if((cs[1]<'0') || (cs[1]>'7'))
		return(NULL);

	xs=0; ys=0;
	pw=0; mv=0; asc=0;
	flip=1;
	
	if(cs[1]=='1')
		{ pw=1; mv=0; asc=1; }
	if(cs[1]=='2')
		{ pw=1; mv=0; asc=1; }
	if(cs[1]=='3')
		{ pw=3; mv=0; asc=1; }
	if(cs[1]=='4')
		{ pw=1; mv=0; asc=0; }
	if(cs[1]=='5')
		{ pw=1; mv=0; asc=0; }
	if(cs[1]=='6')
		{ pw=3; mv=0; asc=0; }
	cs+=2;
	while(*cs && (*cs<=' '))cs++;
	
	while(*cs)
	{
		if((xs>=1) && (ys>=1) && (pw>=1) && (mv>=1))
			break;

		ct=tb; cte=tb+250;
		while(*cs && (*cs<=' '))
			 { cs++; }
		while(*cs && (*cs!='\r') && (*cs!='\n') && (ct<cte))
			{ *ct++=*cs++; }
		if(*cs=='\r')
			{ cs++; }
		if(*cs=='\n')
			{ cs++; }
		*ct=0;
		
		if(tb[0]=='#')
			{ continue; }
		
		if(!xs)
		{
			sscanf(tb, "%u %u %u", &xs, &ys, &mv);
			continue;
		}
		if(!mv)
		{
			sscanf(tb, "%u", &mv);
			continue;
		}
		break;
	}
	
	if((xs<1) || (ys<1) || (pw<1) || (mv<1))
		return(NULL);

	if(mv>=256)
	{
		mvsc=65535/mv;
	}else
	{
		mvsc=255/mv;
	}
	
	*rw=xs;
	*rh=ys;
	ibuf=bgbcc_malloc(xs*ys*4);
	
	if(asc)
	{
		for(y=0; y<ys; y++)
		{
			y1=flip?(ys-y-1):y;
			for(x=0; x<xs; x++)
			{
				if(pw==3)
				{
					cs=BGBCC_Img_DecodePPM_ReadNumber(cs, &cr);
					cs=BGBCC_Img_DecodePPM_ReadNumber(cs, &cg);
					cs=BGBCC_Img_DecodePPM_ReadNumber(cs, &cb);
				}else
				{
					cs=BGBCC_Img_DecodePPM_ReadNumber(cs, &cg);
					cr=cg;
					cb=cg;
				}
				
				if(mvsc>1)
				{
					cr*=mvsc;
					cg*=mvsc;
					cb*=mvsc;
				}
				if(mv>=256)
				{
					cr>>=8;
					cg>>=8;
					cb>>=8;
				}
			
				ibuf[(y1*xs+x)*4+0]=cr;
				ibuf[(y1*xs+x)*4+1]=cg;
				ibuf[(y1*xs+x)*4+2]=cb;
				ibuf[(y1*xs+x)*4+3]=255;
			}
		}
		
		return(ibuf);
	}
	
	for(y=0; y<ys; y++)
	{
		y1=flip?(ys-y-1):y;
		for(x=0; x<xs; x++)
		{
			if(mv>255)
			{
				if(pw==3)
				{
					cr=*cs++;
					cr=(cr<<8)|(*cs++);
					cg=*cs++;
					cg=(cg<<8)|(*cs++);
					cb=*cs++;
					cb=(cb<<8)|(*cs++);
				}else
				{
					cg=*cs++;
					cg=(cg<<8)|(*cs++);
					cr=cg;
					cb=cg;
				}
			}else
			{
				if(pw==3)
				{
					cr=*cs++;
					cg=*cs++;
					cb=*cs++;
				}else
				{
					cg=*cs++;
					cr=cg;
					cb=cg;
				}
				
			}
		
			if(mvsc>1)
			{
				cr*=mvsc;
				cg*=mvsc;
				cb*=mvsc;
			}
			if(mv>=256)
			{
				cr>>=8;
				cg>>=8;
				cb>>=8;
			}
			ibuf[(y1*xs+x)*4+0]=cr;
			ibuf[(y1*xs+x)*4+1]=cg;
			ibuf[(y1*xs+x)*4+2]=cb;
			ibuf[(y1*xs+x)*4+3]=255;
		}
	}
	
	return(ibuf);
}

byte *BGBCC_Img_DecodeDrawCmds(byte *srcbuf, int *rw, int *rh)
{
	byte tb[256];
	char **a;
	byte *cs;
	byte *dstbuf;
	u32 clr_rgb, ref_rgb;
	int xs, ys, x0, y0, x1, y1;
	int i, j, k, n;
	
	dstbuf=NULL;
	xs=0; ys=0;
	clr_rgb=0xFFFFFFFFU;
	
	cs=srcbuf;
	while(*cs)
	{
		tb[0]=0;
		cs=BGBCC_Img_DecodePPM_ReadLine(cs, tb);
		if(tb[0]=='#')
			continue;
		
		a=bgbcc_split(tb);
		if(!a[0])
			continue;
			
		if(!bgbcc_stricmp(a[0], "canvas"))
		{
			xs=atoi(a[1]);
			ys=atoi(a[2]);
			dstbuf=malloc(xs*ys*4);
//			memset(dstbuf, 255, xs*ys*4);
			
			n=xs*ys;
			for(i=0; i<n; i++)
			{
				dstbuf[i*4+0]=(clr_rgb>>16)&255;
				dstbuf[i*4+1]=(clr_rgb>> 8)&255;
				dstbuf[i*4+2]=(clr_rgb>> 0)&255;
				dstbuf[i*4+3]=(clr_rgb>>24)&255;
			}
			continue;
		}

		if(!bgbcc_stricmp(a[0], "color") || !bgbcc_stricmp(a[0], "color3"))
		{
			if(a[1][0]=='#')
			{
				sscanf(a[1], "#%06X", &clr_rgb);
				clr_rgb|=0xFF000000;
				continue;
			}
			
			clr_rgb=0xFF000000|(atoi(a[1])<<16)|
				(atoi(a[2])<<8)|(atoi(a[3])<<0);
			continue;
		}

		if(!bgbcc_stricmp(a[0], "color4"))
		{
			if(a[1][0]=='#')
			{
				sscanf(a[1], "#%08X", &clr_rgb);
				continue;
			}
			
			clr_rgb=
				(atoi(a[1])<<16)|(atoi(a[2])<< 8) |
				(atoi(a[3])<< 0)|(atoi(a[4])<<24) ;
			continue;
		}

		if(!bgbcc_stricmp(a[0], "line"))
		{
			x0=atoi(a[1]);	y0=atoi(a[2]);
			x1=atoi(a[3]);	y1=atoi(a[4]);
			BGBCC_ImgUtil_DrawLine(
				dstbuf, xs, ys,
				x0, y0, x1, y1,
				clr_rgb);
			continue;
		}

		if(!bgbcc_stricmp(a[0], "floodfill"))
		{
			x0=atoi(a[1]);	y0=atoi(a[2]);
			BGBCC_ImgUtil_DrawFloodFill(
				dstbuf, xs, ys,
				x0, y0, clr_rgb);
			continue;
		}

		if(!bgbcc_stricmp(a[0], "basetext"))
		{
			x0=atoi(a[1]);	y0=atoi(a[2]);
			BGBCC_ImgUtil_DrawTextString(
				dstbuf, xs, ys,
				x0, y0, a[3], clr_rgb);
			continue;
		}
	}
	
	*rw=xs;
	*rh=ys;
	return(dstbuf);
}
