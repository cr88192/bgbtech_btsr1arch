char **tksh_editlines;
int tksh_num_editlines;
int tksh_max_editlines;
int tksh_cur_editline;
int tksh_cur_editmode;

char *tksh_edittextbuf;
int tksh_max_edittextbuf;
int tksh_pos_edittextbuf;

char *tksh_edit_fname;

int tksh_edit_base_x;
int tksh_edit_base_y;
int tksh_edit_cur_x;
int tksh_edit_cur_y;
byte tksh_edit_redraw;
byte tksh_edit_ctrl;

int tksh_edit_sel_sx;
int tksh_edit_sel_sy;
int tksh_edit_sel_ex;
int tksh_edit_sel_ey;

int TKSH_EdCheckExpandLines(int max)
{
	int n;

	if(!tksh_editlines)
	{
		n=1024;
		while(max>n)
			n=n+(n>>1);
		tksh_editlines=tk_malloc(n*sizeof(char *));
		tksh_max_editlines=n;
	}
	
	if(max>tksh_max_editlines)
	{
		n=tksh_max_editlines;
		while(max>n)
			n=n+(n>>1);
		tksh_editlines=tk_realloc(tksh_editlines, n*sizeof(char *));
		tksh_max_editlines=n;
	}
	return(0);
}

int TKSH_EdCheckExpandEditbuf(int max)
{
	int n;

	if(!tksh_edittextbuf)
	{
		n=32768;
		while(max>n)
			n=n+(n>>1);
		tksh_edittextbuf=tk_malloc(n);
		tksh_max_edittextbuf=n;
	}
	
	if(max>tksh_max_edittextbuf)
	{
		n=tksh_max_edittextbuf;
		while(max>n)
			n=n+(n>>1);
		tksh_edittextbuf=tk_realloc(tksh_edittextbuf, n*sizeof(char *));
		tksh_max_edittextbuf=n;
	}
	return(0);
}

char *TKSH_EdStrdupLineBuffer(char *str)
{
	char *ctstr;
	int pos, l;
	
	l=strlen(str);
	
	pos=tksh_pos_edittextbuf;
	TKSH_EdCheckExpandEditbuf(pos+l+256);
	
	ctstr=tksh_edittextbuf+pos;
	memcpy(ctstr, str, l+1);
	tksh_pos_edittextbuf=pos+l+1;
	return(ctstr);
}

byte *TKSH_LoadFileBuf(char *name, int *rsz)
{
	TK_FILE *fd;
	byte *tbuf;
	int sz;

	fd=tk_fopen(name, "rb");
	if(!fd)
		return(NULL);
	
	tk_fseek(fd, 0, 2);
	sz=tk_ftell(fd);
	tk_fseek(fd, 0, 0);

	tbuf=tk_malloc(sz+1);
	tk_fread(tbuf, 1, sz, fd);
	tk_fclose(fd);

	*rsz=sz;
	return(tbuf);
}

int TKSH_EdLoadFile(char *name)
{
	char ttb[256];
	byte *tbuf, *cs, *cse, *ct, *ct1;
	TK_FILE *fd;
	int sz, ch, nl;

	sz=0;
	tbuf=TKSH_LoadFileBuf(name, &sz);
	if(!tbuf)
		return(0);

	cs=tbuf; cse=cs+sz; nl=1;
	while(cs<cse)
	{
		ch=*cs++;
		if(ch=='\n')
			nl++;
	}

	tksh_edit_fname=strdup(name);

	TKSH_EdCheckExpandLines(nl);
	TKSH_EdCheckExpandEditbuf(sz*2);

	cs=tbuf; cse=cs+sz; nl=0;
	ct=ttb;
	while(cs<cse)
	{
		ch=*cs++;

		if(ch=='\n')
		{
			*ct++=0;
			
			ct1=TKSH_EdStrdupLineBuffer(ttb);
			tksh_editlines[nl]=ct1;
			nl++;
			
			ct=ttb;
			continue;
		}

		if(ch=='\r')
		{
			continue;
		}
		
		*ct++=ch;
	}
	tksh_num_editlines=nl;

	tk_free(tbuf);

	return(0);
}

int TKSH_EdStoreFile(char *name, int sl, int el)
{
	TK_FILE *fd;
	int i;
	
	fd=tk_fopen(name, "wb");
	if(!fd)
		return(-1);
	
	for(i=0; i<=el; i++)
	{
		tk_fputs(tksh_editlines[i], fd);
		tk_fputc('\n', fd);
	}
	
	tk_fclose(fd);
}

int TKSH_EdInsertLine(int line, char *str)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>tksh_num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(tksh_num_editlines+16);
	
	i=tksh_num_editlines++;
	while(i>line)
	{
		cs=tksh_editlines[i-1];
		tksh_editlines[i]=cs;
		i--;
	}

	cs=TKSH_EdStrdupLineBuffer(str);
	tksh_editlines[line]=cs;
	return(0);
}

int TKSH_EdUpdateLine(int line, char *str)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>tksh_num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(tksh_num_editlines+16);

	cs=TKSH_EdStrdupLineBuffer(str);
	tksh_editlines[line]=cs;
	return(0);
}

int TKSH_EdDeleteLine(int line)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>tksh_num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(tksh_num_editlines+16);
	
	for(i=line; i<tksh_num_editlines; i++)
	{
		cs=tksh_editlines[i+1];
		tksh_editlines[i]=cs;
	}
	tksh_num_editlines--;
	return(0);
}

int TKSH_EdParseCommand(char *cmd)
{
	int stk[16];
	char *cs, *cs1;
	int stkpos, sval, ch;
	int i, j, k;
	
	if(tksh_cur_editmode)
	{
		if(!strcmp(cmd, "."))
		{
			tksh_cur_editmode=0;
			return(0);
		}

		TKSH_EdInsertLine(tksh_cur_editline++, cmd);
		return(0);
	}
	
	stkpos=0;
	sval=0;
	cs=cmd;
	while(*cs)
	{
		ch=*cs;
		if((ch>='0') && (ch<='9'))
		{
			sval=(sval*10)+(ch-'0');
			cs++;
			continue;
		}
		if(ch==',')
		{
			stk[stkpos++]=sval;
			cs++;
			continue;
		}
		
		if(ch=='.')
		{
			sval=tksh_cur_editline;
			cs++;
			continue;
		}
		
		if(ch=='$')
		{
			sval=tksh_num_editlines-1;
			cs++;
			continue;
		}
		
		break;
	}
	stk[stkpos++]=sval;
	
	ch=*cs;
	if(!ch)
		return(0);

	if((ch=='l') || (ch=='p'))
	{
//		if(!stk[0])
//			stk[0]=1;
		if(!stk[1])
			stk[1]=tksh_num_editlines-1;
		for(i=stk[0]; i<=stk[1]; i++)
		{
			cs1=tksh_editlines[i];
			tk_printf("%s\n", cs1);
		}
		tksh_cur_editline=stk[1];
		return(0);
	}

	if(ch=='n')
	{
//		if(!stk[0])
//			stk[0]=1;
		if(!stk[1])
			stk[1]=tksh_num_editlines-1;
		for(i=stk[0]; i<=stk[1]; i++)
		{
			cs1=tksh_editlines[i];
			tk_printf("%d\t%s\n", i, cs1);
		}
		tksh_cur_editline=stk[1];
		return(0);
	}

	if((ch=='a') || (ch=='i'))
	{
		tksh_cur_editline=stk[0];
		tksh_cur_editmode=1;
		return(0);
	}

	if(ch=='w')
	{
		if(!stk[1])
			stk[1]=tksh_num_editlines-1;
		while(*cs && (*cs>' '))
			cs++;
		while(*cs && *cs<=' ')
			cs++;
			
		cs1=cs;
		
		TKSH_EdStoreFile(cs1, stk[0], stk[1]);
		return(0);
	}

	tk_printf("?\n");
	return(0);
}


int TKSH_EditRedraw()
{
	char tb[128];
	char *cs1;
	int i, j, k, l;

	if(tksh_edit_cur_x<0)
	{
		if(tksh_edit_cur_y>0)
		{
			tksh_edit_cur_y--;

			cs1=tksh_editlines[tksh_edit_cur_y];
			l=0;
			if(cs1)
				l=strlen(cs1);

			tksh_edit_cur_x=l;
		}else
		{
			tksh_edit_cur_x=0;
		}
	}
//	if(tksh_edit_cur_y>=tksh_num_editlines)
//		tksh_edit_cur_y=tksh_num_editlines-1;
	
	if(tksh_edit_cur_y<0)
		tksh_edit_cur_y=0;
	if(tksh_edit_cur_y>tksh_num_editlines)
		tksh_edit_cur_y=tksh_num_editlines;

	cs1=tksh_editlines[tksh_edit_cur_y];
	l=0;
	if(cs1)
		l=strlen(cs1);
	if(tksh_edit_cur_x>l)
	{
		tksh_edit_cur_x=0;
		tksh_edit_cur_y++;
		if(tksh_edit_cur_y>tksh_num_editlines)
			tksh_edit_cur_y=tksh_num_editlines;
	}

	if(tksh_edit_cur_y<tksh_edit_base_y)
		tksh_edit_base_y=tksh_edit_cur_y;
	if(tksh_edit_cur_y>(tksh_edit_base_y+24))
		tksh_edit_base_y=tksh_edit_cur_y-24;

	if(tksh_edit_base_y<0)
		tksh_edit_base_y=0;
	if(tksh_edit_base_y>tksh_num_editlines)
		tksh_edit_base_y=tksh_num_editlines;

	for(i=0; i<24; i++)
	{
		tk_puts("\x1B[44m");
		tk_puts("\x1B[37m");

		sprintf(tb, "\x1B[%d;1H", i+1);
		tk_puts(tb);
		tk_puts(
			"                    "
			"                    "
			"                    "
			"                    "
//			"                    "
			);

		sprintf(tb, "\x1B[%d;1H", i+1);
		tk_puts(tb);

		j=tksh_edit_base_y+i;
		cs1="";
		if(j<=tksh_num_editlines)
			cs1=tksh_editlines[j];
		if(!cs1)
			cs1="";
			
		if(tksh_edit_base_x)
		{
			l=strlen(cs1);
			if(tksh_edit_base_x<l)
				{ cs1+=tksh_edit_base_x; }
			else
				{ cs1=""; }
		}

		if((j>tksh_edit_sel_sy) && (j<tksh_edit_sel_ey))
		{
			tk_puts("\x1B[46m");
		}

		sprintf(tb, "%04d %s", tksh_edit_base_y+i+1, cs1);
		tk_puts(tb);
		
		if((j==tksh_edit_sel_sy) && (tksh_edit_sel_sy!=tksh_edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH",
				i+1,
				(tksh_edit_sel_sx-tksh_edit_base_x)+6);
			tk_puts(tb);

			tk_puts(cs1+(tksh_edit_sel_sx-tksh_edit_base_x));
		}

		if((j==tksh_edit_sel_ey) && (tksh_edit_sel_sy!=tksh_edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH", i+1, 6);
			tk_puts(tb);

			memcpy(tb, cs1, tksh_edit_sel_ex-tksh_edit_base_x);
			tb[tksh_edit_sel_ex]=0;
			tk_puts(tb);
		}

		if((j==tksh_edit_sel_sy) && (tksh_edit_sel_sy==tksh_edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH",
				i+1,
				(tksh_edit_sel_sx-tksh_edit_base_x)+6);
			tk_puts(tb);

			memcpy(tb, cs1+
				(tksh_edit_sel_sx-tksh_edit_base_x),
				tksh_edit_sel_ex-tksh_edit_sel_sx);
			tb[tksh_edit_sel_ex-tksh_edit_sel_sx]=0;
			tk_puts(tb);
		}
	}

	sprintf(tb, "\x1B[%d;%dH",
		(tksh_edit_cur_y-tksh_edit_base_y)+1,
		(tksh_edit_cur_x-tksh_edit_base_x)+6);
	tk_puts(tb);
}

int TKSH_EditUpdateLoop()
{
	char tbuf[256];
	char *cs1, *cs2;
	int key, dn, kk;
	int i, j, k, l, brk;

	TKSH_EdCheckExpandLines(1024);
	TKSH_EdCheckExpandEditbuf(1024);
	tksh_edit_redraw=1;

	tksh_edit_base_x=0;
	tksh_edit_base_y=0;

	tksh_edit_cur_x=0;
	tksh_edit_cur_y=0;

	brk=0;
	while(!brk)
	{
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
				tksh_edit_ctrl=dn;
				continue;
			}
			
			if(tksh_edit_ctrl && (key=='q'))
			{
				brk=1;
				break;
			}

			if(tksh_edit_ctrl && (key=='s'))
			{
				if(tksh_edit_fname)
				{
					TKSH_EdStoreFile(tksh_edit_fname,
						0, tksh_num_editlines-1);
				}
				continue;
			}
			
			if(!dn)
				continue;

			if((key>=' ') && (key<='~') && !tksh_edit_ctrl)
			{
				cs1=tksh_editlines[tksh_edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=tksh_edit_cur_x;
					memcpy(tbuf, cs1, l+1);
					memcpy(tbuf+j+1, cs1+j, (l-j)+1);
				}else
				{
					memset(tbuf, 0, 16);
					l=0;
					j=0;
				}
				tbuf[j]=key;
				tksh_edit_cur_x++;
				TKSH_EdUpdateLine(tksh_edit_cur_y, tbuf);
				tksh_edit_redraw=1;
				continue;
			}

			if((key=='\b') && !tksh_edit_ctrl)
			{
				if(tksh_edit_cur_x<=0)
				{
					if(tksh_edit_cur_y>0)
					{
						cs1=tksh_editlines[tksh_edit_cur_y-1];
						cs2=tksh_editlines[tksh_edit_cur_y];
						strcpy(tbuf, cs1);
						strcat(tbuf, cs2);
						TKSH_EdUpdateLine(tksh_edit_cur_y-1, tbuf);
						TKSH_EdDeleteLine(tksh_edit_cur_y);
						tksh_edit_cur_x=strlen(cs1);
						tksh_edit_cur_y--;
						tksh_edit_redraw=1;
					}
					continue;
				}
			
				cs1=tksh_editlines[tksh_edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=tksh_edit_cur_x;
					memcpy(tbuf, cs1, l+1);
					memcpy(tbuf+j-1, cs1+j, (l-j)+1);
				}else
				{
					memset(tbuf, 0, 16);
					l=0;
					j=0;
				}
				tksh_edit_cur_x--;
				TKSH_EdUpdateLine(tksh_edit_cur_y, tbuf);
				tksh_edit_redraw=1;
				continue;
			}

			if((key=='\r') && !tksh_edit_ctrl)
			{
				cs1=tksh_editlines[tksh_edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=tksh_edit_cur_x;
					memcpy(tbuf, cs1, l+1);
				}else
				{
					l=0;
					j=0;
				}
				tbuf[j]=0;
				cs2=tbuf+128;
				memcpy(cs2, cs1+j, (l-j)+1);
				if(tksh_edit_cur_y>=tksh_num_editlines)
					TKSH_EdInsertLine(tksh_edit_cur_y, tbuf);
				else
					TKSH_EdUpdateLine(tksh_edit_cur_y, tbuf);
				TKSH_EdInsertLine(tksh_edit_cur_y+1, cs2);
				tksh_edit_cur_y++;
				tksh_edit_cur_x=0;
				tksh_edit_redraw=1;
				continue;
			}

			if(key==TK_K_UPARROW)
			{
				tksh_edit_cur_y--;
				tksh_edit_redraw=1;
				continue;
			}
			if(key==TK_K_DOWNARROW)
			{
				tksh_edit_cur_y++;
				tksh_edit_redraw=1;
				continue;
			}
			if(key==TK_K_LEFTARROW)
			{
				tksh_edit_cur_x--;
				tksh_edit_redraw=1;
				continue;
			}
			if(key==TK_K_RIGHTARROW)
			{
				tksh_edit_cur_x++;
				tksh_edit_redraw=1;
				continue;
			}

			if(key==TK_K_HOME)
			{
				tksh_edit_cur_x=0;
				tksh_edit_redraw=1;
				continue;
			}
			if(key==TK_K_END)
			{
				cs1=tksh_editlines[tksh_edit_cur_y];
				l=strlen(cs1);
				tksh_edit_cur_x=l;
				tksh_edit_redraw=1;
				continue;
			}
		}
		
		if(tksh_edit_redraw)
		{
			tksh_edit_redraw=0;
			TKSH_EditRedraw();
		}
	}
	return(0);
}
