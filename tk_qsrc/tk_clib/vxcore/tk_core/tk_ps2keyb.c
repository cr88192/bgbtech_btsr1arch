static byte tk_ps2_kbuf[256];
static byte tk_ps2_kbposs;
static byte tk_ps2_kbpose;

static byte tk_ps2st_e0;
static byte tk_ps2st_f0;
static byte tk_ps2st_shift;
static byte tk_ps2st_alt;
static byte tk_ps2st_ctrl;

static u16 tk_ps2_scan2key[256] = {
0,			TK_K_F9,	0,			TK_K_F5,	//00..03
TK_K_F3,	TK_K_F1,	TK_K_F2,	TK_K_F12,	//04..07
0,			TK_K_F10,	TK_K_F8,	TK_K_F6,	//08..0B
TK_K_F4,	TK_K_TAB,	'`',		0,			//0C..0F
0,			TK_K_ALT,	TK_K_SHIFT,	0,			//10..13
TK_K_CTRL,	'q',		'1',		0,			//14..17
0,			0,			'z',		's',		//18..1B
'a',		'w',		'2',		0,			//1C..1F
0,			'c',		'x',		'd',		//20..23
'e',		'4',		'3',		0,			//24..27
0,			' ',		'v',		'f',		//28..2B
't',		'r',		'5',		0,			//2C..2F
0,			'n',		'b',		'h',		//30..33
'g',		'y',		'6',		0,			//34..37
0,			0,			'm',		'j',		//38..3B
'u',		'7',		'8',		0,			//3C..3F
0,			',',		'k',		'i',		//40..43
'o',		'0',		'9',		0,			//44..47
0,			'.',		'/',		'l',		//48..4B
';',		'p',		'-',		0,			//4C..4F
0,			0,			'\'',		0,			//50..53
'[',		'=',		0,			0,			//54..57
0,			TK_K_SHIFT,	TK_K_ENTER,	']',		//58..5B
0,			'\\',		0,			0,			//5C..5F
0,			0,			0,			0,			//60..63
0,			0,			TK_K_BKSP,	0,			//64..67
// 0,			TK_K_END,	0,			TK_K_LEFT,	//68..6B
// TK_K_HOME,	0,			0,			0,			//6C..6F
// TK_K_INS,	TK_K_DEL,	TK_K_DOWN,	TK_K_NP5,	//70..73
// TK_K_RIGHT,	TK_K_UP,	TK_K_ESC,	TK_K_NUMLK,	//74..77
// TK_K_F11,	TK_K_NPADD,	TK_K_PGDN,	TK_K_NPSUB,	//78..7B
// TK_K_NPMUL,	TK_K_PGUP,	TK_K_SCRLK,	0,			//7C..7F

0,			TK_K_NP1,	0,			TK_K_NP4,	//68..6B
TK_K_NP7,	0,			0,			0,			//6C..6F
TK_K_NP0,	TK_K_DEL,	TK_K_NP2,	TK_K_NP5,	//70..73
TK_K_NP6,	TK_K_NP8,	TK_K_ESC,	TK_K_NUMLK,	//74..77
TK_K_F11,	TK_K_NPADD,	TK_K_NP3,	TK_K_NPSUB,	//78..7B
TK_K_NPMUL,	TK_K_NP9,	TK_K_SCRLK,	0,			//7C..7F

0,			0,			0,			TK_K_F7,	//80..83
0,			0,			0,			0,			//84..87
0,			0,			0,			0,			//88..8B
0,			0,			0,			0,			//8C..8F
0,			0,			0,			0,			//90..93
0,			0,			0,			0,			//94..97
0,			0,			0,			0,			//98..9B
0,			0,			0,			0,			//9C..9F
0,			0,			0,			0,			//A0..A3
0,			0,			0,			0,			//A4..A7
0,			0,			0,			0,			//A8..AB
0,			0,			0,			0,			//AC..AF
0,			0,			0,			0,			//B0..B3
0,			0,			0,			0,			//B4..B7
0,			0,			0,			0,			//B8..BB
0,			0,			0,			0,			//BC..BF
0,			0,			0,			0,			//C0..C3
0,			0,			0,			0,			//C4..C7
0,			0,			0,			0,			//C8..CB
0,			0,			0,			0,			//CC..CF
0,			0,			0,			0,			//D0..D3
0,			0,			0,			0,			//D4..D7
0,			0,			0,			0,			//D8..DB
0,			0,			0,			0,			//DC..DF
0,			0,			0,			0,			//E0..E3
0,			0,			0,			0,			//E4..E7
0,			0,			0,			0,			//E8..EB
0,			0,			0,			0,			//EC..EF
0,			0,			0,			0,			//F0..F3
0,			0,			0,			0,			//F4..F7
0,			0,			0,			0,			//F8..FB
0,			0,			0,			0			//FC..FF
};

static byte tk_ps2_keyshift[256]={
 0,   0,   0,   0,   0,   0,   0,   0,   //00..07
 0,   0,   0,   0,   0,   0,   0,   0,   //08..0F
 0,   0,   0,   0,   0,   0,   0,   0,   //10..17
 0,   0,   0,   0,   0,   0,   0,   0,   //18..1F
 0,   0,   0,   0,   0,   0,   0,  '"',  //20..27
 0,   0,   0,   0,  '<', '_', '>', '?',  //28..2F
')', '!', '@', '#', '$', '%', '^', '&',  //30..37
'*', '(',  0,  ':',  0,  '+',  0,   0,   //38..3F
 0,   0,   0,   0,   0,   0,   0,   0,   //40..47
 0,   0,   0,   0,   0,   0,   0,   0,   //48..4F
 0,   0,   0,   0,   0,   0,   0,   0,   //50..57
 0,   0,   0,  '{', '|', '}',  0,   0,   //58..5F
'~', 'A', 'B', 'C', 'D', 'E', 'F', 'G',  //60..67
'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',  //68..6F
'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',  //70..77
'X', 'Y', 'Z',  0,   0,   0,   0,   0,  //78..7F
};


int tk_ps2kb_scanhit(void)
	{ return(P_PS2KB_STAT&1); }

int tk_ps2kb_scanch(void)
{
	while(!(P_PS2KB_STAT&1))
		sleep_0();
	return(P_PS2KB_RX);
}

int tk_ps2kb_tryscanch(void)
{
	if(!(P_PS2KB_STAT&1))
		return(0);
	return(P_PS2KB_RX);
}

int tk_ps2addkeyevbyte(int key)
{
	tk_ps2_kbuf[tk_ps2_kbpose]=key;
	tk_ps2_kbpose++;
}

int tk_ps2addkeyevword(int key)
{
	int j, k;
	
//	printf("PS2 AddKey %04X\n", key);
	
	k=key;

	if(k&0x8000)
	{
		j=k&4095;
		if(j<0x7F)
		{
			tk_ps2addkeyevbyte(j|0x80);
		}else if(j<0xFF)
		{
			tk_ps2addkeyevbyte(0xFF);
			tk_ps2addkeyevbyte(j);
		}
	}else
	{
		j=k&4095;
		if(j<0x7F)
		{
			tk_ps2addkeyevbyte(j);
		}else if(j<0xFF)
		{
			tk_ps2addkeyevbyte(0x7F);
			tk_ps2addkeyevbyte(j);
		}
	}
}

int tk_ps2kb_updatei(void)
{
//	static byte numshift[10]={')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
	int c, c1, j, k;
	
	c=tk_ps2kb_tryscanch();

	if(!c)
		return(0);

	if(c==0xE0)
	{
		tk_ps2st_e0=1;
		return(0);
	}

	if(c==0xF0)
	{
		tk_ps2st_f0=1;
		return(0);
	}
	
	k=tk_ps2_scan2key[c];
	if(tk_ps2st_f0)
		k|=0x8000;
	
	j=k&4095;

	if(tk_ps2st_e0)
	{
		switch(j&255)
		{
		case TK_K_ENTER:	c1=TK_K_NPENTR;	break;
		case	'/':		c1=TK_K_NPDIV;	break;
		case TK_K_NP0:		c1=TK_K_INS;	break;
		case TK_K_NP1:		c1=TK_K_END;	break;
		case TK_K_NP2:		c1=TK_K_DOWN;	break;
		case TK_K_NP3:		c1=TK_K_PGDN;	break;
		case TK_K_NP4:		c1=TK_K_LEFT;	break;
		case TK_K_NP6:		c1=TK_K_RIGHT;	break;
		case TK_K_NP7:		c1=TK_K_HOME;	break;
		case TK_K_NP8:		c1=TK_K_UP;		break;
		case TK_K_NP9:		c1=TK_K_PGUP;	break;
		default:			c1=j;			break;
		}
		k=(k&0xF000)|c1;
		j=c1;
	}
	
	if(tk_ps2st_shift)
	{
		c1=tk_ps2_keyshift[j&255];
		if(c1)
		{
			k=(k&0xF000)|c1;
		}
		
#if 0
		if((j>='a') && (j<='z'))
		{
			j=(j-'a')+'A';
			k=(k&0xF000)|j;
		}

		if((j>='0') && (j<='9'))
		{
			j=numshift[j-'0'];
			k=(k&0xF000)|j;
		}
#endif
	}

	j=k&4095;
	if(j>0)
	{
		tk_ps2addkeyevword(k);

		if(j==TK_K_SHIFT)
		{
			if(k&0x8000)
				tk_ps2st_shift=0;
			else
				tk_ps2st_shift=1;
		}

		if(j==TK_K_CTRL)
		{
			if(k&0x8000)
				tk_ps2st_ctrl=0;
			else
				tk_ps2st_ctrl=1;
		}

		if(j==TK_K_ALT)
		{
			if(k&0x8000)
				tk_ps2st_alt=0;
			else
				tk_ps2st_alt=1;
		}
	}

	tk_ps2st_e0=0;
	tk_ps2st_f0=0;
}

int tk_ps2kb_kbhit(void)
{
	while(tk_ps2kb_scanhit())
		{ tk_ps2kb_updatei(); }
	return(tk_ps2_kbposs!=tk_ps2_kbpose);
}

int tk_ps2trygetch(void)
{
	int c;

	while(tk_ps2kb_scanhit())
		{ tk_ps2kb_updatei(); }

	c=0;
	if(tk_ps2_kbposs!=tk_ps2_kbpose)
	{
		c=tk_ps2_kbuf[tk_ps2_kbposs];
		tk_ps2_kbposs++;
	}

	return(c);
}

int tk_ps2getch(void)
{
	int c;

	while(1)
	{
		c=tk_ps2trygetch();
		if(c)break;
		sleep_0();
	}

	return(c);
}



int tk_ps2ms_scanhit(void)
	{ return(P_PS2MS_STAT&1); }

int tk_ps2ms_scanch(void)
{
	while(!(P_PS2MS_STAT&1))
		sleep_0();
	return(P_PS2MS_RX);
}

int tk_ps2ms_tryscanch(void)
{
	if(!(P_PS2MS_STAT&1))
		return(0);
	return(P_PS2MS_RX);
}

int tk_ps2ms_sendcmd(byte val)
{
	P_PS2MS_TX=val;
	return(0);
}

int tk_ps2ms_tryscanch_to(void)
{
	u64 t0, t0e, tt;
	
	tt=TK_GetTimeUs();
	t0=tt;
	t0e=t0+10000;

	while(!(P_PS2MS_STAT&1) && (tt<t0e))
	{
		sleep_0();
		tt=TK_GetTimeUs();
	}

	return(P_PS2MS_RX);
}


int tk_ps2ms_x;
int tk_ps2ms_y;
int tk_ps2ms_b;

int tk_ps2ms_c0, tk_ps2ms_c1, tk_ps2ms_c2, tk_ps2ms_c3;
int tk_ps2ms_init;

int tk_ps2ms_pollupdates(void)
{
	if(!tk_ps2ms_init)
	{
		tk_ps2ms_sendcmd(0xFF);
		tk_ps2ms_tryscanch_to();
		tk_ps2ms_tryscanch_to();
		tk_ps2ms_sendcmd(0xF4);
		tk_ps2ms_tryscanch_to();
		tk_ps2ms_init=1;
	}

	while(tk_ps2ms_scanhit())
	{
		tk_ps2ms_c3=tk_ps2ms_c2;
		tk_ps2ms_c2=tk_ps2ms_c1;
		tk_ps2ms_c1=tk_ps2ms_c0;
		tk_ps2ms_c0=tk_ps2ms_scanch();
		
		if(
			((tk_ps2ms_c2&0xC8)==0x08)	&&
			(((tk_ps2ms_c2<<2)&0x80)==(tk_ps2ms_c0&0x80))	&&
			(((tk_ps2ms_c2<<3)&0x80)==(tk_ps2ms_c1&0x80))
			)
		{
			tk_ps2ms_x+=(signed char)tk_ps2ms_c1;
			tk_ps2ms_y+=(signed char)tk_ps2ms_c0;
			tk_ps2ms_b=tk_ps2ms_c2&7;
		}
	}
}

int tk_ps2ms_getpos(int *rx, int *ry, int *rb)
{
	tk_ps2ms_pollupdates();
	*rx=tk_ps2ms_x;
	*ry=tk_ps2ms_y;
	*rb=tk_ps2ms_b;
	return(0);
}

int tk_ps2ms_setpos(int x, int y)
{
	tk_ps2ms_x=x;
	tk_ps2ms_y=y;
	return(0);
}
