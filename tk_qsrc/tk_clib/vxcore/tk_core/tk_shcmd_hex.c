byte *tksh_hex_buf;
int tksh_hex_bufsz;

char *tksh_hex_fname;

int tksh_hex_base_x;
int tksh_hex_base_y;
int tksh_hex_cur_x;
int tksh_hex_cur_y;
byte tksh_hex_redraw;
byte tksh_hex_ctrl;

int TKSH_HexLoadFile(char *fname)
{
	byte *buf;
	int sz;
	sz=0;
	buf=TKSH_LoadFileBuf(NULL, fname, &sz);
	tksh_hex_buf=buf;
	tksh_hex_bufsz=sz;
	return(1);
}

int TKSH_HexGetRow(int row, byte *tbuf)
{
	memcpy(tbuf, tksh_hex_buf+(row*16), 16);
	return(0);
}

int TKSH_HexSetRow(int row, byte *tbuf)
{
	memcpy(tksh_hex_buf+(row*16), tbuf, 16);
	return(0);
}

int TKSH_HexRedraw()
{
	char tb[128];
	byte tbu0[16], tbu1[36];
	byte *s, *t;
	int i, j, k;

	if(tksh_hex_cur_x>=32)
	{
		tksh_hex_cur_y++;
		tksh_hex_cur_x=0;
	}

	if(tksh_hex_cur_x<0)
	{
		tksh_hex_cur_y--;
		tksh_hex_cur_x=31;
	}
	
	while((tksh_hex_cur_y*16)>=tksh_hex_bufsz)
		tksh_hex_cur_y--;

	while(((tksh_hex_cur_y*16)+(tksh_hex_cur_x/2))>=tksh_hex_bufsz)
		tksh_hex_cur_x--;
	
	if(tksh_hex_cur_y<0)
		tksh_hex_cur_y=0;

	if(tksh_hex_cur_y<tksh_hex_base_y)
		{ tksh_hex_base_y=tksh_hex_cur_y; }
	if(tksh_hex_cur_y>(tksh_hex_base_y+23))
		{ tksh_hex_base_y=tksh_hex_cur_y-23; }


	for(i=0; i<24; i++)
	{
		tk_puts("\x1B[44m");
		tk_puts("\x1B[37m");

		sprintf(tb, "\x1B[%d;1H", i+1);
		tk_puts(tb);

#if 0
		tk_puts(
			"                    "
			"                    "
			"                    "
			"                    "
//			"                    "
			);
#endif

		sprintf(tb, "\x1B[%d;1H", i+1);
		tk_puts(tb);

		j=tksh_hex_base_y+i;
		if((j*16)>=tksh_hex_bufsz)
		{
#if 1
			tk_puts(
				"                    "
				"                    "
				"                    "
				"                    "
				);
#endif

			continue;
		}
		
		TKSH_HexGetRow(j, tbu0);

#if 0	
		memcpy(tbu1, tbu0, 16);
		for(j=0; j<16; j++)
		{
			k=tbu1[j];
			if((k<' ') || (k>'~'))
				k='.';
			tbu1[j]=k;
		}
		tbu1[16]=0;
#endif

#if 1
//		memcpy(tbu1, tbu0, 16);

		s=tbu0;
		t=tbu1;
		
		for(j=0; j<16; j++)
		{
			k=*s++;
			if((k<' ') || (k>'~'))
			{
				k=k+0x0600;
				*t++=0xC0|((k>>6)&0x1F);
				*t++=0x80|((k>>0)&0x3F);
				continue;
//				k='.';
			}
			*t++=k;
		}
		*t++=0;
#endif
		
		s=tbu1;
		
		sprintf(tb, "%08X "
			"%02X %02X %02X %02X  %02X %02X %02X %02X  "
			"%02X %02X %02X %02X  %02X %02X %02X %02X  "
			"%s   ",
			(tksh_hex_base_y+i)*16,
			tbu0[ 0], tbu0[ 1], tbu0[ 2], tbu0[ 3],
			tbu0[ 4], tbu0[ 5], tbu0[ 6], tbu0[ 7],
			tbu0[ 8], tbu0[ 9], tbu0[10], tbu0[11],
			tbu0[12], tbu0[13], tbu0[14], tbu0[15],
			s);
		if(((tksh_hex_base_y+i+1)*16)>tksh_hex_bufsz)
		{
			k=tksh_hex_bufsz-((tksh_hex_base_y+i)*16);
			k=k+k;
			j=k+(k>>1)+(k>>3);
			j=j+9;
			memset(tb+j, ' ', 61-j);
			memset(tb+61+(k/2), ' ', 16-(k/2));
		}
		tk_puts(tb);
	}

	j=tksh_hex_cur_x+(tksh_hex_cur_x>>1)+(tksh_hex_cur_x>>3);
	sprintf(tb, "\x1B[%d;%dH",
		(tksh_hex_cur_y-tksh_hex_base_y)+1,
		j+10);
	tk_puts(tb);
}

int TKSH_HexUpdateLoop()
{
	char tbuf[256];
	byte tbu[16];
	char *cs1, *cs2;
	int key, dn, kk;
	int i, j, k, l, brk;

	if(tk_issyscall())
	{
		return(-1);
	}

	tksh_hex_base_y=0;

	tksh_hex_cur_x=0;
	tksh_hex_cur_y=0;
	tksh_hex_redraw=1;

	brk=0;
	while(!brk)
	{
		if(!tk_kbhit())
		{
			TK_YieldCurrentThread();
			continue;
		}
	
		while(tk_kbhit())
		{
			kk=tk_getch();
			if(kk==0x7F)
				{ key=tk_getch(); dn=1; }
			else if(kk==0xFF)
				{ key=tk_getch(); dn=0; }
			else if(kk==0x80)
			{
				key=tk_getch();
				key=(key<<8)|tk_getch();
				dn=!(key&0x8000);
			}else
			{
				key=kk&0x7F;
				dn=!(kk&0x80);
			}

			if(key==TK_K_CTRL)
			{
				tksh_hex_ctrl=dn;
				continue;
			}
			
			if(tksh_hex_ctrl && (key=='q'))
			{
				brk=1;
				break;
			}

			if(tksh_hex_ctrl && (key=='s'))
			{
				if(tksh_hex_fname)
				{
//					TKSH_EdStoreFile(tksh_hex_fname,
//						0, tksh_num_editlines-1);
				}
				continue;
			}
			
			if(!dn)
				continue;

			if((key=='q') || (key=='Q'))
				brk=1;

			if((key>=' ') && (key<='~') && !tksh_hex_ctrl)
			{
				TKSH_HexGetRow(tksh_hex_cur_y, tbu);
				
				if((key>='0') && (key<='9'))
				{
					j=tksh_hex_cur_x;
					tbu[j>>1]&=~(0xF0>>((j&1)*4));
					tbu[j>>1]|=((key-'0')*16)>>((j&1)*4);
				}

				if((key>='a') && (key<='f'))
				{
					j=tksh_hex_cur_x;
					tbu[j>>1]&=~(0xF0>>((j&1)*4));
					tbu[j>>1]|=((10+key-'a')*16)>>((j&1)*4);
				}

				if((key>='A') && (key<='F'))
				{
					j=tksh_hex_cur_x;
					tbu[j>>1]&=~(0xF0>>((j&1)*4));
					tbu[j>>1]|=((10+key-'A')*16)>>((j&1)*4);
				}
				
				tksh_hex_cur_x++;
				
				TKSH_HexSetRow(tksh_hex_cur_y, tbu);
			
#if 0
				cs1=tksh_editlines[tksh_hex_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=tksh_hex_cur_x;
					memcpy(tbuf, cs1, l+1);
					memcpy(tbuf+j+1, cs1+j, (l-j)+1);
				}else
				{
					memset(tbuf, 0, 16);
					l=0;
					j=0;
				}
				tbuf[j]=key;
				tksh_hex_cur_x++;
				TKSH_EdUpdateLine(tksh_hex_cur_y, tbuf);
#endif
				tksh_hex_redraw=1;
				continue;
			}

			if(key==TK_K_UPARROW)
			{
				tksh_hex_cur_y--;
				tksh_hex_redraw=1;
				continue;
			}
			if(key==TK_K_DOWNARROW)
			{
				tksh_hex_cur_y++;
				tksh_hex_redraw=1;
				continue;
			}
			if(key==TK_K_LEFTARROW)
			{
				tksh_hex_cur_x--;
				tksh_hex_redraw=1;
				continue;
			}
			if(key==TK_K_RIGHTARROW)
			{
				tksh_hex_cur_x++;
				tksh_hex_redraw=1;
				continue;
			}

			if(key==TK_K_HOME)
			{
				tksh_hex_cur_x=0;
				tksh_hex_redraw=1;
				continue;
			}

			if(key==TK_K_END)
			{
				tksh_hex_cur_x=31;
//				cs1=tksh_editlines[tksh_hex_cur_y];
//				l=strlen(cs1);
//				tksh_hex_cur_x=l;
				tksh_hex_redraw=1;
				continue;
			}

			if(key==TK_K_PGUP)
			{
				tksh_hex_cur_y-=23;
				tksh_hex_redraw=1;
				continue;
			}

			if(key==TK_K_PGDN)
			{
				tksh_hex_cur_y+=23;
				tksh_hex_redraw=1;
				continue;
			}
		}
		
		if(tksh_hex_redraw)
		{
			tksh_hex_redraw=0;
			TKSH_HexRedraw();
		}
	}
	return(0);
}
