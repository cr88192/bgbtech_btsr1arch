u16 btic4b_grad16_ipattab0[64]={
 0x3333, 0x1111, 0x7777, 0x1337, 0x0113, 0x377F, 0x137F, 0x0137,
 0x0013, 0x37FF, 0x017F, 0x0017, 0x17FF, 0x00FF, 0x0FFF, 0x000F,
 0x08EF, 0x7100, 0x008E, 0x7310, 0x3100, 0x08CE, 0x008C, 0x7331,
 0x3110, 0x088C, 0x6666, 0x366C, 0x17E8, 0x0FF0, 0x718E, 0x399C,
 0x5555, 0x0F0F, 0x5A5A, 0x33CC, 0x3C3C, 0x55AA, 0x6969, 0x5AA5,
 0x73CE, 0x13C8, 0x324C, 0x3BDC, 0x6996, 0x3CC3, 0x6699, 0x0660,
 0x4E40, 0x2720, 0x0272, 0x04E4, 0x6C93, 0x36C9, 0x639C, 0x39C6,
 0x6CC9, 0x6339, 0x7E81, 0x18E7, 0x0F33, 0x33F0, 0x22EE, 0x4477};

u16 btic4b_grad16_dbltab0[16]={
 0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F,
 0xC0, 0xC3, 0xCC, 0xCF, 0xF0, 0xF3, 0xFC, 0xFF};

u64 btic4b_grad16_ipattab1[256];

u32 *btic4b_grad16_ipattab2;

int BTIC4B_Grad16_Init()
{
	static int init=0;
	byte tpix[64], tpix1[64];
	int i0, i1, i2, i3;
	int i4, i5, i6, i7;
	int j0, j1, j2, j3;
	int k0, k1, k2, k3;
	int i0r1, i1r1, i2r1, i3r1;
	int i0l1, i1l1, i2l1, i3l1;
	u64 li, lj, lk;
	int nb;
	int i, j, k;
	
	if(init)
		return(0);
	init=1;
	
	btic4b_grad16_ipattab2=malloc(65536*4*4);
	
	for(i=0; i<64; i++)
	{
		j=btic4b_grad16_ipattab0[i];
		i0=btic4b_grad16_dbltab0[(j>>12)&15];
		i1=btic4b_grad16_dbltab0[(j>> 8)&15];
		i2=btic4b_grad16_dbltab0[(j>> 4)&15];
		i3=btic4b_grad16_dbltab0[(j    )&15];
		li=(i0<<24)|(i0<<16)|(i1<<8)|i1;
		li=(li<<32)|(i2<<24)|(i2<<16)|(i3<<8)|i3;
		btic4b_grad16_ipattab1[i]=li;
		
		i0r1=(i0>>1)|(i0&0x80);	i1r1=(i1>>1)|(i1&0x80);
		i2r1=(i2>>1)|(i2&0x80);	i3r1=(i3>>1)|(i3&0x80);
		i0l1=(i0<<1)|(i0&0x01);	i1l1=(i1<<1)|(i1&0x01);
		i2l1=(i2<<1)|(i2&0x01);	i3l1=(i3<<1)|(i3&0x01);

		i4=(i0&i1)|(i0r1&i1l1)|(i0l1&i1r1);
		i5=(i1&i2)|(i1r1&i2l1)|(i1l1&i2r1);
		i6=(i2&i3)|(i2r1&i3l1)|(i2l1&i3r1);
		i7=(i3&i0)|(i3r1&i0l1)|(i3l1&i0r1);

		lj=(i0<<24)|(i4<<16)|(i1<<8)|i5;
		lj=(lj<<32)|(i2<<24)|(i6<<16)|(i3<<8)|i7;
		btic4b_grad16_ipattab1[64+i]=lj;

		btic4b_grad16_ipattab1[128+i]=~li;
		btic4b_grad16_ipattab1[192+i]=~lj;
	}

	for(i=0; i<256; i++)
		for(j=0; j<256; j++)
	{
		li=btic4b_grad16_ipattab1[j];

		for(i1=0; i1<8; i1++)
			for(j1=0; j1<8; j1++)
		{
			tpix[i1*8+j1]=((li>>((7-i1)*8+(7-j1)))&1)?255:0;
		}

		nb=((i>>4)&3);
		if(!nb && (((i>>6)&3)>=2))
		{
			k=j+1;			k=k*65521+j+1;
			k=k*65521+j+1;	k=k*65521+j+1;

			for(j1=0; j1<64; j1++)
			{
				tpix[j1]=(k>>16)&255;
				k=k*65521+j+1;
			}
			nb=(((i>>6)&3)==3)?2:0;
		}

		if(i&0x03)
		{
			for(i1=0; i1<8; i1++)
				for(j1=0; j1<8; j1++)
					{ tpix1[i1*8+j1]=tpix[i1*8+((j1+(i&3))&7)]; }
			for(k=0; k<64; k++)
				tpix[k]=tpix1[k];
		}

		if(((i>>2)&3)==1)
		{
			for(i1=0; i1<8; i1++)
				for(j1=0; j1<8; j1++)
					{ tpix1[i1*8+j1]=tpix[j1*8+(7-i1)]; }
			for(k=0; k<64; k++)
				tpix[k]=tpix1[k];
		}else if(((i>>2)&3)==3)
		{
			for(i1=0; i1<8; i1++)
				for(j1=0; j1<8; j1++)
					{ tpix1[i1*8+j1]=tpix[(7-j1)*8+i1]; }
			for(k=0; k<64; k++)
				tpix[k]=tpix1[k];
		}else if(((i>>2)&3)==2)
		{
			for(k=0; k<64; k++)
				tpix1[k]=tpix[63-k];
			for(k=0; k<64; k++)
				tpix[k]=tpix1[k];
		}

		for(i0=0; i0<nb; i0++)
		{
			for(i1=0; i1<8; i1++)
				for(j1=0; j1<8; j1++)
			{
				i2=i1+1; if(i2>=8)i2=7;
				j2=j1+1; if(j2>=8)j2=7;
				i4=tpix[i1*8+j1];	i5=tpix[i1*8+j2];
				i6=tpix[i2*8+j1];	i7=tpix[i2*8+j2];
				tpix1[i1*8+j1]=(i4+i5+i6+i7)/4;
			}
			for(k=0; k<64; k++)
				tpix[k]=tpix1[k];
		}
		
		for(i1=0; i1<8; i1++)
		{
			k1=0;
			for(j1=0; j1<8; j1++)
			{
				k=tpix[i1*8+j1];
				k=((k+31)>>6);
				k1|=k<<(j1*2);
			}
			
			if(((i>>6)&3)==1)
				{ k1=~k1; }
			if(((i>>6)&3)==2)
			{
				k1=~k1;
			}
			
			((u16 *)(btic4b_grad16_ipattab2+(i*256+j)*4))[i1]=k1;
		}
	}
	
	return(1);
}
