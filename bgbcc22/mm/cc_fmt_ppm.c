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
