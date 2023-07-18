// TK_CONWIDTH

void tkgdi_con_drawcell(_tkgdi_conparm *con, int x, int y)
{
	u16 *pixb;
	u64 q0, q1, pix;
	int i0, i1, fgc, bgc;
	int px, py, pz;
	int i, j, k;

	i0=(y*TK_CONWIDTH+x)*2;
	q0=con->conbuf[i0+0];
	q1=con->conbuf[i0+1];

	px=x*8;
	py=y*8;
	pz=(py*(TK_CONWIDTH*8))+px;

	if(((q0>>30)&3)==0)
	{
		fgc=tk_con_clr64to555[(q0>>16)&63];
		bgc=tk_con_clr64to555[(q0>>22)&63];
		pix=TK_Con_GlyphForCodepoint(q0&1023);
	}else
		if(((q0>>30)&3)==2)
	{
		fgc=(q0>> 0)&0x7FFF;
		bgc=(q0>>15)&0x7FFF;
		pix=q1;
	}else
	{
	}


	pixb=con->pixbuf;
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			k=bgc;
			if((pix>>(63-(i*8+j)))&1)
				k=fgc;
			pixb[pz+j]=k;
		}
		pz+=TK_CONWIDTH*8;
	}
}

void tkgdi_con_redrawbuffer(_tkgdi_conparm *con)
{
	u64 q, qrm;
	int i, j, k;
	
	qrm=con->conrowmask;
	if(!qrm)
		return;
	
	for(i=0; i<TK_CONHEIGHT; i++)
	{
		if(!((qrm>>i)&1))
			continue;

		for(j=0; j<TK_CONWIDTH; j++)
		{
			k=i*TK_CONWIDTH+j;
			q=con->conmask[k>>6];
			if(!q)
			{
				j=((j+64)&(~63))-1;
				continue;
			}
			
			if(!((q>>(k&63))&1))
				continue;
			
			q&=~(1LL<<(k&63));
			con->conmask[k>>6]=q;
			
			tkgdi_con_drawcell(con, j, i);
		}
	}
	
	con->conrowmask=0;
}

void TKGDI_Con_UpdateHwCursor(_tkgdi_conparm *con)
{
	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_clear(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	u64 q0, q1, tv;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	i1=con->text_attr;

	buf=(volatile u32 *)(con->conbuf);

	q1=0;
	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

	for(i=0; i<25; i++)
	{
		i0=(i*TK_CONWIDTH)*4;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			*(u64 *)(buf+i0+0)=q0;
			*(u64 *)(buf+i0+2)=q1;
			i0+=4;
			
			i1=i0>>2;
			con->conmask[i1>>6]|=1<<(i1&63);
		}
	}
	
	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void TKGDI_Con_SetCursorPos(_tkgdi_conparm *con, int x, int y)
{
	con->x=x;
	con->y=y;
	TKGDI_Con_UpdateHwCursor(con);
}

void TKGDI_Con_SetColorFg(_tkgdi_conparm *con, int x)
{
	int cr, cg, cb, cc;
	con->text_attr=(con->text_attr&0xFFC0FFFFU)|((x&0x3F)<<16);
	
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	con->fgclr_555=cc;
}

void TKGDI_Con_SetColorBg(_tkgdi_conparm *con, int x)
{
	int cr, cg, cb, cc;
	con->text_attr=(con->text_attr&0xF03FFFFFU)|((x&0x3F)<<22);
	cr=x&0x30; cg=x&0x0C; cb=x&0x03;
	cc=(cr<<9)|(cr<<7)|(cg<<6)|(cg<<4)|(cb<<3)|(cb<<1);
	con->bgclr_555=cc;
}

void tkgdi_con_init(_tkgdi_conparm *con)
{
	int i, j, k;
	con->text_attr_dfl=0x002A0000;
	con->text_attr=con->text_attr_dfl;

	TKGDI_Con_SetColorBg(con, 0);
	TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
	
	con->conbuf=tk_malloc(80*25*(2*8));
	con->pixbuf=tk_malloc((80*8)*(25*8)*2);
	
	j=((80*25+63)>>6);
	con->conmask=tk_malloc(j*8);
}

void tkgdi_con_reset(_tkgdi_conparm *con)
{
	tkgdi_con_init(con);
	tkgdi_con_clear(con);
	TKGDI_Con_SetCursorPos(con, 0, 0);
	con->text_attr=con->text_attr_dfl;
	TKGDI_Con_SetColorBg(con, 0);
	TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
}

void tkgdi_con_chkreset(_tkgdi_conparm *con)
{
	if(!con->ena)
	{
		tkgdi_con_init(con);
		tkgdi_con_clear(con);
		TKGDI_Con_SetCursorPos(con, 0, 0);

		con->text_attr=con->text_attr_dfl;
		TKGDI_Con_SetColorBg(con, 0);
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[7]);
	}
}

void tkgdi_con_scroll_up(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	volatile u64 *qb0, *qb1;
	u64 q0, q1, q2, q3;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

	buf=(volatile u32 *)(con->conbuf);

	for(i=0; i<TK_CONHEIGHTN1; i++)
	{
		i0=((i+0)*TK_CONWIDTH)*4;
		i1=((i+1)*TK_CONWIDTH)*4;

		qb0=(u64 *)(buf+i0);
		qb1=(u64 *)(buf+i1);
		for(j=0; j<TK_CONWIDTH; j+=4)
		{
			q0=qb1[0];		q1=qb1[1];
			q2=qb1[2];		q3=qb1[3];
			qb0[0]=q0;		qb0[1]=q1;
			qb0[2]=q2;		qb0[3]=q3;

			q0=qb1[4];		q1=qb1[5];
			q2=qb1[6];		q3=qb1[7];
			qb0[4]=q0;		qb0[5]=q1;
			qb0[6]=q2;		qb0[7]=q3;

			qb0+=8;		qb1+=8;

			k=(i0>>2)+j;
			con->conmask[k>>6]|=15<<(k&63);
		}
	}

	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;
	q1=0;

	i0=(TK_CONHEIGHTN1*TK_CONWIDTH)*4;
	i1=con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
	{
		k=(i0>>2)+j;
		con->conmask[k>>6]|=1<<(k&63);

		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;
		i0+=4;
	}

	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_scroll_down(_tkgdi_conparm *con)
{
	volatile u32 *buf;
	u64 q0, q1;
//	u32 p0, p1, p2, p3;
	int i0, i1;
	int i, j, k;

//	buf=con->buf;
	buf=(volatile u32 *)(con->conbuf);

	for(i=TK_CONHEIGHTN1; i>0; i--)
	{
		i0=((i+0)*TK_CONWIDTH)*2;
		i1=((i-1)*TK_CONWIDTH)*2;
		for(j=0; j<TK_CONWIDTH; j++)
		{
			q0=((u64 *)(buf+i1))[0];
			q1=((u64 *)(buf+i1))[1];
			((u64 *)(buf+i0))[0]=q0;
			((u64 *)(buf+i0))[1]=q1;

			k=(i0>>2)+j;
			con->conmask[k>>6]|=15<<(k&63);

			i0+=4;
			i1+=4;
		}
	}

	q1=0;
	q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

	i0=0;
	i1=con->text_attr;
	for(j=0; j<TK_CONWIDTH; j++)
	{
		((u64 *)(buf+i0))[0]=q0;
		((u64 *)(buf+i0))[1]=q1;

		k=(i0>>2)+j;
		con->conmask[k>>6]|=1<<(k&63);

		i0+=4;
	}

	con->conrowmask=-1;

	tkgdi_con_redrawbuffer(con);
}

void tkgdi_con_newline(_tkgdi_conparm *con)
{
	con->y++;
	if(con->y>=25)
	{
		tkgdi_con_scroll_up(con);
		con->y--;
	}
}

void TKGDI_Con_PutcEscapeSgr(_tkgdi_conparm *con, int mode)
{
	switch(mode)
	{
	case 0:
		con->text_attr=con->text_attr_dfl;
		break;
	case 1:
		con->text_attr|=0x003F0000;
		break;
	case 2:
		break;
	case 3:
	case 7:
		con->text_attr|=0x00002000;
		break;
	case 4:
		con->text_attr|=0x00000800;
		break;
	case 5:
		con->text_attr&=~0x0000C000;
		con->text_attr|= 0x00008000;
		break;
	case 6:
		con->text_attr&=~0x0000C000;
		con->text_attr|= 0x00004000;
		break;
	case 9:
		con->text_attr|=0x00000400;
		break;
	case 10:
		con->text_attr&=~0x00000300;
		break;
	
	case 22:
		con->text_attr=
			(con->text_attr&(~0x003F0000))|
			(con->text_attr_dfl&0x003F0000);
		break;
	case 23:
		con->text_attr&=~0x00002000;
		break;
	case 24:
		con->text_attr&=~0x00000800;
		break;
	case 25:
		con->text_attr&=~0x0000C000;
		break;
	case 29:
		con->text_attr&=~0x00000400;
		break;
	
	case 30:	case 31:
	case 32:	case 33:
	case 34:	case 35:
	case 36:	case 37:
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[mode-30]);
		break;
	case 39:
		TKGDI_Con_SetColorFg(con, 0x2A);
		break;		
	case 40:	case 41:
	case 42:	case 43:
	case 44:	case 45:
	case 46:	case 47:
		TKGDI_Con_SetColorBg(con, tk_con_clr16to64[mode-40]);
		break;
	case 49:
		TKGDI_Con_SetColorBg(con, 0);
		break;		

	case 90:	case 91:
	case 92:	case 93:
	case 94:	case 95:
	case 96:	case 97:
		TKGDI_Con_SetColorFg(con, tk_con_clr16to64[8+(mode-90)]);
		break;
	case 100:	case 101:
	case 102:	case 103:
	case 104:	case 105:
	case 106:	case 107:
		TKGDI_Con_SetColorBg(con, tk_con_clr16to64[8+(mode-100)]);
		break;
	}
}

void TKGDI_Con_PutcEscape(_tkgdi_conparm *con, int ch)
{
	int i, j, k;

	if(con->isesc==1)
	{
		if(ch=='[')
		{
			con->escval3=0;
			con->escval2=0;
			con->escval1=0;
			con->escval0=0;
			con->isesc=2;
			return;
		}
		
		con->isesc=0;
		return;
	}

	if(con->isesc==2)
	{
		switch(ch)
		{
		case '0':	case '1':
		case '2':	case '3':
		case '4':	case '5':
		case '6':	case '7':
		case '8':	case '9':
			con->escval0=(con->escval0*10)+(ch-'0');
			break;
		case ';':
			con->escval3=con->escval2;
			con->escval2=con->escval1;
			con->escval1=con->escval0;
			con->escval0=0;
			break;
		case 'A':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x, con->y-i);
			con->isesc=0;
			break;
		case 'B':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x, con->y+i);
			con->isesc=0;
			break;
		case 'C':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x+i, con->y);
			con->isesc=0;
			break;
		case 'D':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, con->x-i, con->y);
			con->isesc=0;
			break;

		case 'E':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, 0, con->y+i);
			con->isesc=0;
			break;
		case 'F':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, 0, con->y-i);
			con->isesc=0;
			break;
		case 'G':
			i=con->escval0;
			if(!i)i=1;
			TKGDI_Con_SetCursorPos(con, i-1, con->y);
			con->isesc=0;
			break;
		case 'H':
		case 'f':
			i=con->escval0;
			j=con->escval1;
			if(!i)i=1;
//			if(!j)j=1;
			if(!j) { j=i; i=1; }
			TKGDI_Con_SetCursorPos(con, i-1, j-1);
			con->isesc=0;
			break;

		case 'J':
			switch(con->escval0)
			{
			case 2:
			case 3:
				tkgdi_con_clear(con);
				break;
			}
			con->isesc=0;
			break;

		case 'S':
			i=con->escval0;
			if(!i)i=1;
			while(i--)
				{ tkgdi_con_scroll_up(con); }
			con->isesc=0;
			break;
		case 'T':
			i=con->escval0;
			if(!i)i=1;
			while(i--)
				{ tkgdi_con_scroll_down(con); }
			con->isesc=0;
			break;

		case 'm':
			TKGDI_Con_PutcEscapeSgr(con, con->escval0);
			con->isesc=0;
			break;

		default:
			con->isesc=0;
			break;
		}
		return;
	}

	con->isesc=0;
}

void tkgdi_con_putc(_tkgdi_conparm *con, int ch)
{
	volatile u32 *buf;
	int tx, ty, tz;
	u64 q0, q1;
	u32 px, py;
	int i, j, k;
	
	if(!con->ena)
		return;

	buf=(volatile u32 *)(con->conbuf);

	if(con->isesc)
	{
		TKGDI_Con_PutcEscape(con, ch);
		return;
	}

	if(ch<' ')
	{
		if(ch=='\b')
		{
			if(con->x>0)con->x--;
			TKGDI_Con_UpdateHwCursor(con);

			return;
		}

		if(ch=='\r')
		{
			con->x=0;
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}
		if(ch=='\n')
		{
			tkgdi_con_newline(con);
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}
		if(ch=='\t')
		{
			con->x=(con->x+8)&(~7);
			if(con->x>=TK_CONWIDTH)
				{ tkgdi_con_newline(con); con->x=0; }
			TKGDI_Con_UpdateHwCursor(con);
			return;
		}

		if(ch=='\x1B')
		{
			con->isesc=1;
			return;
		}

		if(ch=='\x1f')
		{
			px=0x003F8000|'_';
			ty=con->y;
			tx=con->x;
			tz=(ty*TK_CONWIDTH+tx)*4;
			buf[tz+0]=px;
			tx++;
			con->x=tx;

			k=tz>>2;
			con->conmask[k>>6]|=1<<(k&63);
			con->conrowmask|=1LL<<ty;
			return;
		}

		return;
	}

	px=(con->text_attr)|ch;

	ty=con->y;
	tx=con->x;

	con->conrowmask|=1LL<<ty;

	if(ch<0x100)
	{
		tz=(ty*TK_CONWIDTH+tx)*2;
		((u64 *)buf)[tz]=px;

		k=tz>>2;
		con->conmask[k>>6]|=1<<(k&63);
	}else
	{
		if(ch>=0x100)
		{
			ch=tk_con_doremap(ch);
		}

		if((ch>=0x00) && (ch<=0x7F))
		{
			q1=tk_gfxcon_glyphs[ch];
		}else
			if((ch>=0x80) && (ch<=0xFF))
		{
			q1=tk_gfxcon_glyphs_lat1ext[ch&0x7F];
		}else
			if((ch>=0x100) && (ch<=0x17F))
		{
			q1=tk_gfxcon_glyphs_cyril0[ch&0x7F];
		}else
			if((ch>=0x180) && (ch<=0x1FF))
		{
			q1=tk_gfxcon_glyphs_437ext[ch&0x7F];
		}else
			if((ch>=0x0600) && (ch<=0x06FF))
		{
			q1=tk_gfxcon_hexblock[ch&0xFF];
		}else
		{
			q1=tk_gfxcon_glyphs[0xBF];
		}

		q0=(2ULL<<30)|(con->bgclr_555<<15)|con->fgclr_555;

		tz=(ty*TK_CONWIDTH+tx)*2;
		((u64 *)buf)[tz+0]=q0;
		((u64 *)buf)[tz+1]=q1;

		k=tz>>2;
		con->conmask[k>>6]|=1<<(k&63);
	}

	tx++;
	con->x=tx;
	if(tx>=TK_CONWIDTH)
	{
		tkgdi_con_newline(con);
		con->x=0;
	}

	TKGDI_Con_UpdateHwCursor(con);
}
