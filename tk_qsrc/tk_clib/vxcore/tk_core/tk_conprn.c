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
#define TK_CONHEIGHT	25
#define TK_CONHEIGHTN1	24

void *TK_ConGetCtxV(void)
{
	TK_SysArg ar[4];
	void *p;
	
//	ar[0].i=sz;
	p=NULL;
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
		if(!tk_con)
			tk_con=&tk_con_bss;
	}

	tk_con->buf=(u32 *)0xF00A0000;
	tk_con->x=0;
	tk_con->y=0;
	tk_con->ena=1;

//	((u32 *)0xF00BFF00)[0]=0x0015;		//320x200x16bpp
//	((u32 *)0xF00BFF00)[0]=0x0005;		//
	((u32 *)0xF00BFF00)[0]=0x0001;		//
	
	if(tk_iskernel())
	{
		tk_con_clear();
	}
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

void tk_con_chkreset()
{
	if(!tk_con || !tk_con->ena)
	{
		tk_con_init();
		tk_con_clear();
//		((u32 *)0xF009F000)[0]=0x0029;
		((u32 *)0xF009F000)[0]=0x0000;
	}
}

void tk_con_scroll_up()
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=tk_con->buf;

#if 1
	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i+1)*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i1);
			*(u64 *)(buf+i0)=q0;
			i0+=8;
			i1+=8;
		}
	}

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*8;
	for(j=0; j<TK_CONWIDTH; j++)
		{ *(u64 *)(buf+i0)=0; i0+=8; }
#endif

#if 0
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
#endif
}

void tk_con_newline()
{
//	if(!tk_con)
//		tk_con_init();
	
//	tk_con->x=0;
	tk_con->y++;
	if(tk_con->y>=25)
	{
		tk_con_scroll_up();
		tk_con->y--;
	}
}

void tk_con_putc(int ch)
{
	int tx, ty, tz;
	u32 px, py;
	
	if(!tk_con)
		tk_con_init();
	
	if(!tk_con->ena)
		return;

	if(ch<' ')
	{
		if(ch=='\b')
		{
			if(tk_con->x>0)tk_con->x--;

#if 0
			px=0x003F8000|'_';
			py=0x003F0000|' ';
			
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			tk_con->buf[tz+0]=px;
			tk_con->buf[tz+8]=py;
#endif
			return;
		}

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

		if(ch=='\x1f')
		{
#if 1
			px=0x003F8000|'_';
			ty=tk_con->y;
			tx=tk_con->x;
			tz=(ty*TK_CONWIDTH+tx)*8;
			tk_con->buf[tz+0]=px;
			tx++;
			tk_con->x=tx;
#endif
		}
		return;
	}
	
//	px=0x0FC00000|ch;
	px=0x003F0000|ch;
//	py=0x003F8000|'_';
	
	ty=tk_con->y;
	tx=tk_con->x;
	tz=(ty*TK_CONWIDTH+tx)*8;
	tk_con->buf[tz]=px;
//	tk_con->buf[tz+8]=py;

//	__debugbreak();
	
	tx++;
	tk_con->x=tx;
//	if(tk_con->x>=TK_CONWIDTH)
	if(tx>=TK_CONWIDTH)
	{
		tk_con_newline();
	}
}
