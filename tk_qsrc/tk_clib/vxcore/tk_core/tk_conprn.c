// byte tk_con_x;
// byte tk_con_y;
// byte tk_con_ena;
// volatile u32 *tk_con->buf;

struct conparm_s {
byte x;
byte y;
byte ena;
volatile u32 *buf;
};

struct conparm_s tk_con_bss;

struct conparm_s *tk_con;


// #define TK_CONWIDTH		40
#define TK_CONWIDTH		80

void *TK_ConGetCtxV(void)
{
	TK_SysArg ar[4];
	void *p;
	
//	ar[0].i=sz;
	tk_syscall(NULL, TK_UMSG_GETCONPARAM, &p, ar);
	return(p);
}

void *tk_con_getctx(void)
{
	return(tk_con);
}

void tk_con_init()
{
	if(tk_iskernel())
	{
		tk_con=&tk_con_bss;
	}else
	{
		tk_con=TK_ConGetCtxV();
	}

	tk_con->buf=(u32 *)0xF00A0000;
	tk_con->x=0;
	tk_con->y=0;
	tk_con->ena=1;

//	((u32 *)0xF00BFF00)[0]=0x0015;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0005;		//
	((u32 *)0xF00BFF00)[0]=0x0001;		//
}

void tk_con_disable()
{
	tk_con->ena=0;
}

void tk_con_clear()
{
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;
		*(u64 *)(tk_con->buf+i0)=0;
	}

	for(j=0; j<TK_CONWIDTH; j++)
	{
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
	}
}

void tk_con_reset()
{
	tk_con_init();
	tk_con_clear();
}

void tk_con_scroll_up()
{
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;		i1=((i+1)*TK_CONWIDTH+j)*8;

		q0=*(u64 *)(tk_con->buf+i1);
		*(u64 *)(tk_con->buf+i0)=q0;

//		p0=tk_con->buf[i1+0];
//		p1=tk_con->buf[i1+1];
//		p2=tk_con->buf[i1+2];	p3=tk_con->buf[i1+3];
//		tk_con->buf[i0+0]=p0;
//		tk_con->buf[i0+1]=p1;
//		tk_con->buf[i0+2]=p2;	tk_con->buf[i0+3]=p3;
//		p0=tk_con->buf[i1+4];
//		p1=tk_con->buf[i1+5];
//		p2=tk_con->buf[i1+6];	p3=tk_con->buf[i1+7];
//		tk_con->buf[i0+4]=p0;
//		tk_con->buf[i0+5]=p1;
//		tk_con->buf[i0+6]=p2;	tk_con->buf[i0+7]=p3;
	}

	for(j=0; j<TK_CONWIDTH; j++)
	{
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
	}
}

void tk_con_newline()
{
	if(!tk_con)
		tk_con_init();
	
	tk_con->x=0;
	tk_con->y++;
	if(tk_con->y>=25)
	{
		tk_con_scroll_up();
		tk_con->y--;
	}
}

void tk_con_putc(int ch)
{
	u32 px;
	
	if(!tk_con)
		tk_con_init();
	
	if(!tk_con->ena)
		return;
	
	if(ch<' ')
	{
		if(ch=='\b')
			{ if(tk_con->x>0)tk_con->x--; return; }

		if(ch=='\r')
			{ tk_con->x=0; return; }
		if(ch=='\n')
		{
			tk_con_newline();
			return;
		}
		if(ch=='\t')
		{
			tk_con->x=(tk_con->x+8)&(~7);
			if(tk_con->x>=TK_CONWIDTH)
				{ tk_con_newline(); }
			return;
		}
		return;
	}
	
//	px=0x0FC00000|ch;
	px=0x003F0000|ch;
	tk_con->buf[(tk_con->y*TK_CONWIDTH+tk_con->x)*8+0]=px;

	tk_con->x++;
	if(tk_con->x>=TK_CONWIDTH)
	{
		tk_con_newline();
	}
}
