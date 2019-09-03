byte tk_con_x;
byte tk_con_y;
volatile u32 *tk_con_buf;

// #define TK_CONWIDTH		40
#define TK_CONWIDTH		80

void tk_con_init()
{
	tk_con_buf=(u32 *)0xF00A0000;
	tk_con_x=0;
	tk_con_y=0;

//	((u32 *)0xF00BFF00)[0]=0x0015;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0005;		//
	((u32 *)0xF00BFF00)[0]=0x0001;		//
}

void tk_con_scroll_up()
{
	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;
		i1=((i+1)*TK_CONWIDTH+j)*8;
		p0=tk_con_buf[i1+0];
//		p1=tk_con_buf[i1+1];
//		p2=tk_con_buf[i1+2];	p3=tk_con_buf[i1+3];
		tk_con_buf[i0+0]=p0;
//		tk_con_buf[i0+1]=p1;
//		tk_con_buf[i0+2]=p2;	tk_con_buf[i0+3]=p3;
//		p0=tk_con_buf[i1+4];	p1=tk_con_buf[i1+5];
//		p2=tk_con_buf[i1+6];	p3=tk_con_buf[i1+7];
//		tk_con_buf[i0+4]=p0;	tk_con_buf[i0+5]=p1;
//		tk_con_buf[i0+6]=p2;	tk_con_buf[i0+7]=p3;
	}

	for(j=0; j<40; j++)
	{
//		tk_con_buf[((24*40)+j)*8]=0;
		tk_con_buf[((24*TK_CONWIDTH)+j)*8]=0;
	}
}

void tk_con_newline()
{
	tk_con_x=0;
	tk_con_y++;
	if(tk_con_y>=25)
	{
		tk_con_scroll_up();
		tk_con_y--;
	}
}

void tk_con_putc(int ch)
{
	u32 px;
	
	if(ch<' ')
	{
		if(ch=='\r')
			{ tk_con_x=0; return; }
		if(ch=='\n')
		{
			tk_con_newline();
			return;
		}
		if(ch=='\t')
		{
			tk_con_x=(tk_con_x+8)&(~7);
			if(tk_con_x>=TK_CONWIDTH)
				{ tk_con_newline(); }
			return;
		}
		return;
	}
	
//	px=0x0FC00000|ch;
	px=0x003F0000|ch;
	tk_con_buf[(tk_con_y*TK_CONWIDTH+tk_con_x)*8+0]=px;

	tk_con_x++;
	if(tk_con_x>=TK_CONWIDTH)
	{
		tk_con_newline();
	}
}
