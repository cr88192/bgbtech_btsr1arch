// byte tk_con_x;
// byte tk_con_y;
// byte tk_con_ena;
// volatile u32 *tk_con->buf;

struct conparm_s {
byte x;
byte y;
byte ena;
byte resv_1;
//byte attr;
//byte fgclr;
//byte bgclr;
//byte resv_2;
//byte resv_3;
volatile u32 *buf;
u32 text_attr;
u32 text_attr_dfl;
};

struct conparm_s tk_con_bss;

struct conparm_s *tk_con=NULL;


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
//		tk_con=&tk_con_bss;
		tk_con=NULL;
	}else
	{
		tk_con=TK_ConGetCtxV();
//		if(!tk_con)
//			tk_con=&tk_con_bss;
	}

	if(!tk_con)
	{
		tk_con=&tk_con_bss;
		tk_con->buf=(u32 *)0xF00A0000;
		tk_con->x=0;
		tk_con->y=0;
		tk_con->ena=1;
//		tk_con->attr=0;
//		tk_con->fgclr=0x3F;
//		tk_con->bgclr=0x00;
//		tk_con->text_attr=0x003F0000;
//		tk_con->text_attr_dfl=0x003F0000;
		tk_con->text_attr_dfl=0x002A0000;
		tk_con->text_attr=tk_con->text_attr_dfl;
	}

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

	i1=tk_con->text_attr;

	for(i=0; i<24; i++)
		for(j=0; j<TK_CONWIDTH; j++)
	{
		i0=((i+0)*TK_CONWIDTH+j)*8;
//		*(u64 *)(tk_con->buf+i0)=0;
		*(u64 *)(tk_con->buf+i0)=i1;
	}

	for(j=0; j<TK_CONWIDTH; j++)
	{
//		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=0;
		tk_con->buf[((24*TK_CONWIDTH)+j)*8]=i1;
	}
}

void TK_Con_UpdateHwCursor()
{
	u32 py;
	py=((tk_con->y)<<8)|(tk_con->x);
	((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
}

void TK_Con_SetCursorPos(int x, int y)
{
	tk_con->x=x;
	tk_con->y=y;
	TK_Con_UpdateHwCursor();
}

void TK_Con_SetColorFg(int x)
{
	tk_con->text_attr=(tk_con->text_attr&0xFFC0FFFFU)|((x&0x3F)<<16);
}

void TK_Con_SetColorBg(int x)
{
	tk_con->text_attr=(tk_con->text_attr&0xF03FFFFFU)|((x&0x3F)<<22);
}

void tk_con_reset()
{
	tk_con_init();
	tk_con_clear();
	TK_Con_SetCursorPos(0, 0);
	tk_con->text_attr=tk_con->text_attr_dfl;
}

void tk_con_chkreset()
{
	if(!tk_con || !tk_con->ena)
	{
		tk_con_init();
		tk_con_clear();
//		((u32 *)0xF009F000)[0]=0x0029;
//		((u32 *)0xF009F000)[0]=0x0000;
		((u32 *)0xF00BFF00)[0]=0x0001;		//
		TK_Con_SetCursorPos(0, 0);
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
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
		{ *(u64 *)(buf+i0)=i1; i0+=8; }
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

void tk_con_scroll_down()
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=tk_con->buf;

#if 1
	for(i=TK_CONHEIGHTN1; i>0; i--)
	{
		i0=((i+0)*TK_CONWIDTH)*8;
		i1=((i-1)*TK_CONWIDTH)*8;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=*(u64 *)(buf+i1);
			*(u64 *)(buf+i0)=q0;
			i0+=8;
			i1+=8;
		}
	}

	i0=0;
	i1=tk_con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
//		{ *(u64 *)(buf+i0)=0; i0+=8; }
		{ *(u64 *)(buf+i0)=i1; i0+=8; }
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

byte tk_con_clr16to64[16]={
	0x00, 0x20, 0x08, 0x28, 0x02, 0x22, 0x0A, 0x2A,
	0x00, 0x30, 0x0C, 0x3C, 0x03, 0x33, 0x0F, 0x3F };

static tk_con_isesc=0;
static tk_con_escval0;
static tk_con_escval1;
static tk_con_escval2;
static tk_con_escval3;

void TK_Con_PutcEscapeSgr(int mode)
{
	switch(mode)
	{
	case 0:
		tk_con->text_attr=tk_con->text_attr_dfl;
		break;
	case 1:
		tk_con->text_attr|=0x003F0000;
		break;
	case 2:
		break;
	case 3:
	case 7:
		tk_con->text_attr|=0x00002000;
		break;
	case 4:
		tk_con->text_attr|=0x00000800;
		break;
	case 5:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00008000;
		break;
	case 6:
		tk_con->text_attr&=~0x0000C000;
		tk_con->text_attr|= 0x00004000;
		break;
	case 9:
		tk_con->text_attr|=0x00000400;
		break;
	case 10:
		tk_con->text_attr&=~0x00000300;
		break;
	
	case 22:
		tk_con->text_attr=
			(tk_con->text_attr&(~0x003F0000))|
			(tk_con->text_attr_dfl&0x003F0000);
		break;
	case 23:
		tk_con->text_attr&=~0x00002000;
		break;
	case 24:
		tk_con->text_attr&=~0x00000800;
		break;
	case 25:
		tk_con->text_attr&=~0x0000C000;
		break;
	case 29:
		tk_con->text_attr&=~0x00000400;
		break;
	
	case 30:	case 31:
	case 32:	case 33:
	case 34:	case 35:
	case 36:	case 37:
		TK_Con_SetColorFg(tk_con_clr16to64[mode-30]);
		break;
	case 39:
		TK_Con_SetColorFg(0x2A);
		break;		
	case 40:	case 41:
	case 42:	case 43:
	case 44:	case 45:
	case 46:	case 47:
		TK_Con_SetColorBg(tk_con_clr16to64[mode-40]);
		break;
	case 49:
		TK_Con_SetColorBg(0);
		break;		

	case 90:	case 91:
	case 92:	case 93:
	case 94:	case 95:
	case 96:	case 97:
		TK_Con_SetColorFg(tk_con_clr16to64[8+(mode-90)]);
		break;
	case 100:	case 101:
	case 102:	case 103:
	case 104:	case 105:
	case 106:	case 107:
		TK_Con_SetColorBg(tk_con_clr16to64[8+(mode-100)]);
		break;
	}
}

void TK_Con_PutcEscape(int ch)
{
	int i, j, k;

	if(tk_con_isesc==1)
	{
		if(ch=='[')
		{
			tk_con_escval3=0;
			tk_con_escval2=0;
			tk_con_escval1=0;
			tk_con_escval0=0;
			tk_con_isesc=2;
			return;
		}
		
		tk_con_isesc=0;
		return;
	}

	if(tk_con_isesc==2)
	{
		switch(ch)
		{
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			tk_con_escval0=(tk_con_escval0*10)+(ch-'0');
			break;
		case ';':
			tk_con_escval3=tk_con_escval2;
			tk_con_escval2=tk_con_escval1;
			tk_con_escval1=tk_con_escval0;
			tk_con_escval0=0;
			break;
		case 'A':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'B':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'C':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x+i, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'D':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(tk_con->x-i, tk_con->y);
			tk_con_isesc=0;
			break;

		case 'E':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y+i);
			tk_con_isesc=0;
			break;
		case 'F':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(0, tk_con->y-i);
			tk_con_isesc=0;
			break;
		case 'G':
			i=tk_con_escval0;
			if(!i)i=1;
			TK_Con_SetCursorPos(i-1, tk_con->y);
			tk_con_isesc=0;
			break;
		case 'H':
		case 'f':
			i=tk_con_escval0;
			j=tk_con_escval1;
			if(!i)i=1;
//			if(!j)j=1;
			if(!j) { j=i; i=1; }
			TK_Con_SetCursorPos(i-1, j-1);
			tk_con_isesc=0;
			break;

		case 'J':
			switch(tk_con_escval0)
			{
			case 2:
			case 3:
				tk_con_clear();
				break;
			}
			tk_con_isesc=0;
			break;

		case 'S':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_up(); }
			tk_con_isesc=0;
			break;
		case 'T':
			i=tk_con_escval0;
			if(!i)i=1;
			while(i--)
				{ tk_con_scroll_down(); }
			tk_con_isesc=0;
			break;

		case 'm':
			TK_Con_PutcEscapeSgr(tk_con_escval0);
			tk_con_isesc=0;
			break;

		default:
			tk_con_isesc=0;
			break;
		}
		return;
	}

	tk_con_isesc=0;
}

void tk_con_putc(int ch)
{
	int tx, ty, tz;
	u32 px, py;
	
	if(!tk_con)
		tk_con_init();
	
	if(!tk_con->ena)
		return;
	
	if(tk_con_isesc)
	{
		TK_Con_PutcEscape(ch);
		return;
	}

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

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();

			return;
		}

		if(ch=='\r')
		{
			tk_con->x=0;
//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\n')
		{
			tk_con_newline();

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}
		if(ch=='\t')
		{
			tk_con->x=(tk_con->x+8)&(~7);
			if(tk_con->x>=TK_CONWIDTH)
				{ tk_con_newline(); }

//			py=((tk_con->y)<<8)|(tk_con->x);
//			((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
			TK_Con_UpdateHwCursor();
			return;
		}

		if(ch=='\x1B')
		{
			tk_con_isesc=1;
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
			return;
		}

		return;
	}
	
//	px=0x0FC00000|ch;
//	px=0x003F0000|ch;
	px=(tk_con->text_attr)|ch;
//	px=ch|((tk_con->fgclr)<<16)|((tk_con->bgclr)<<22);
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

//	py=((tk_con->y)<<8)|(tk_con->x);
//	((u32 *)0xF00BFF00)[1]=py;		//move hardware cursor
	TK_Con_UpdateHwCursor();
}
