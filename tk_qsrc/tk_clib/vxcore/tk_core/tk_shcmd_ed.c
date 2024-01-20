#ifdef __BGBCC__
#include "tk_shbasic.c"
#endif

typedef struct TKSH_EditCtx_s TKSH_EditCtx;

struct TKSH_EditCtx_s {
char **editlines;
int num_editlines;
int max_editlines;
int cur_editline;
int cur_editmode;

char *edittextbuf;
int max_edittextbuf;
int pos_edittextbuf;

char *edit_fname;

int edit_base_x;
int edit_base_y;
int edit_cur_x;
int edit_cur_y;
byte edit_redraw;
byte edit_ctrl;

int edit_sel_sx;
int edit_sel_sy;
int edit_sel_ex;
int edit_sel_ey;

#ifdef __BGBCC__
TKSH_BasicCtx *basic;
#endif
};


#if 0
char **tksh_editlines;
int tksh_num_editlines;
int ctx->max_editlines;
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
#endif

TKSH_EditCtx *TKSH_EdAllocContext()
{
	TKSH_EditCtx *ctx;
	ctx=tk_malloc(sizeof(TKSH_EditCtx));
	memset(ctx, 0, sizeof(TKSH_EditCtx));
	return(ctx);
}

int TKSH_EdCheckExpandLines(TKSH_EditCtx *ctx, int max)
{
	int n;

	if(!ctx->editlines)
	{
		n=1024;
		while(max>n)
			n=n+(n>>1);
		ctx->editlines=tk_malloc(n*sizeof(char *));
		ctx->max_editlines=n;
	}
	
	if(max>ctx->max_editlines)
	{
		n=ctx->max_editlines;
		while(max>n)
			n=n+(n>>1);
		ctx->editlines=tk_realloc(ctx->editlines, n*sizeof(char *));
		ctx->max_editlines=n;
	}
	return(0);
}

int TKSH_EdCheckExpandEditbuf(TKSH_EditCtx *ctx, int max)
{
	int n;

	if(!ctx->edittextbuf)
	{
		n=32768;
		while(max>n)
			n=n+(n>>1);
		ctx->edittextbuf=tk_malloc(n);
		ctx->max_edittextbuf=n;
	}
	
	if(max>ctx->max_edittextbuf)
	{
		n=ctx->max_edittextbuf;
		while(max>n)
			n=n+(n>>1);
		ctx->edittextbuf=tk_realloc(ctx->edittextbuf, n*sizeof(char *));
		ctx->max_edittextbuf=n;
	}
	return(0);
}

char *TKSH_EdStrdupLineBuffer(TKSH_EditCtx *ctx, char *str)
{
	char *ctstr;
	int pos, l;
	
	l=strlen(str);
	
	pos=ctx->pos_edittextbuf;
	TKSH_EdCheckExpandEditbuf(ctx, pos+l+256);
	
	ctstr=ctx->edittextbuf+pos;
	memcpy(ctstr, str, l+1);
	ctx->pos_edittextbuf=pos+l+1;
	return(ctstr);
}

byte *TKSH_LoadFileBuf(TKSH_EditCtx *ctx, char *name, int *rsz)
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

int TKSH_EdLoadFile(TKSH_EditCtx *ctx, char *name)
{
	char ttb[256];
	byte *tbuf, *cs, *cse, *ct, *ct1;
	TK_FILE *fd;
	int sz, ch, nl;

	sz=0;
	tbuf=TKSH_LoadFileBuf(ctx, name, &sz);
	if(!tbuf)
		return(0);

	cs=tbuf; cse=cs+sz; nl=1;
	while(cs<cse)
	{
		ch=*cs++;
		if(ch=='\n')
			nl++;
	}

	ctx->edit_fname=strdup(name);

	TKSH_EdCheckExpandLines(ctx, nl);
	TKSH_EdCheckExpandEditbuf(ctx, sz*2);

	cs=tbuf; cse=cs+sz; nl=0;
	ct=ttb;
	while(cs<cse)
	{
		ch=*cs++;

		if(ch=='\n')
		{
			*ct++=0;
			
			ct1=TKSH_EdStrdupLineBuffer(ctx, ttb);
			ctx->editlines[nl]=ct1;
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
	ctx->num_editlines=nl;

	tk_free(tbuf);

	return(0);
}

int TKSH_EdStoreFile(TKSH_EditCtx *ctx, char *name, int sl, int el)
{
	TK_FILE *fd;
	int i;
	
	fd=tk_fopen(name, "wb");
	if(!fd)
		return(-1);
	
	for(i=0; i<=el; i++)
	{
		tk_fputs(ctx->editlines[i], fd);
		tk_fputc('\n', fd);
	}
	
	tk_fclose(fd);
}

int TKSH_EdInsertLine(TKSH_EditCtx *ctx, int line, char *str)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>ctx->num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(ctx, ctx->num_editlines+16);
	
	i=ctx->num_editlines++;
	while(i>line)
	{
		cs=ctx->editlines[i-1];
		ctx->editlines[i]=cs;
		i--;
	}

	cs=TKSH_EdStrdupLineBuffer(ctx, str);
	ctx->editlines[line]=cs;
	return(0);
}

int TKSH_EdUpdateLine(TKSH_EditCtx *ctx, int line, char *str)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>ctx->num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(ctx, ctx->num_editlines+16);

	cs=TKSH_EdStrdupLineBuffer(ctx, str);
	ctx->editlines[line]=cs;
	return(0);
}

int TKSH_EdDeleteLine(TKSH_EditCtx *ctx, int line)
{
	char *cs;
	int i;

	if(line<0)
		return(-1);
	if(line>ctx->num_editlines)
		return(-1);

	TKSH_EdCheckExpandLines(ctx, ctx->num_editlines+16);
	
	for(i=line; i<ctx->num_editlines; i++)
	{
		cs=ctx->editlines[i+1];
		ctx->editlines[i]=cs;
	}
	ctx->num_editlines--;
	return(0);
}

int TKSH_EdParseCommand(TKSH_EditCtx *ctx, char *cmd)
{
	int stk[16];
	char *cs, *cs1;
	int stkpos, sval, ch;
	int i, j, k;
	
	if(ctx->cur_editmode)
	{
		if(!strcmp(cmd, "."))
		{
			ctx->cur_editmode=0;
			return(0);
		}

		TKSH_EdInsertLine(ctx, ctx->cur_editline++, cmd);
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
			sval=ctx->cur_editline;
			cs++;
			continue;
		}
		
		if(ch=='$')
		{
			sval=ctx->num_editlines-1;
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
			stk[1]=ctx->num_editlines-1;
		for(i=stk[0]; i<=stk[1]; i++)
		{
			cs1=ctx->editlines[i];
			tk_printf("%s\n", cs1);
		}
		ctx->cur_editline=stk[1];
		return(0);
	}

	if(ch=='n')
	{
//		if(!stk[0])
//			stk[0]=1;
		if(!stk[1])
			stk[1]=ctx->num_editlines-1;
		for(i=stk[0]; i<=stk[1]; i++)
		{
			cs1=ctx->editlines[i];
			tk_printf("%d\t%s\n", i, cs1);
		}
		ctx->cur_editline=stk[1];
		return(0);
	}

	if((ch=='a') || (ch=='i'))
	{
		ctx->cur_editline=stk[0];
		ctx->cur_editmode=1;
		return(0);
	}

	if(ch=='w')
	{
		if(!stk[1])
			stk[1]=ctx->num_editlines-1;
		while(*cs && (*cs>' '))
			cs++;
		while(*cs && *cs<=' ')
			cs++;
			
		cs1=cs;
		
		TKSH_EdStoreFile(ctx, cs1, stk[0], stk[1]);
		return(0);
	}

	tk_printf("?\n");
	return(0);
}


int TKSH_EditRedraw(TKSH_EditCtx *ctx)
{
	char tb[128];
	char *cs1;
	int i, j, k, l;

	if(ctx->edit_cur_x<0)
	{
		if(ctx->edit_cur_y>0)
		{
			ctx->edit_cur_y--;

			cs1=ctx->editlines[ctx->edit_cur_y];
			l=0;
			if(cs1)
				l=strlen(cs1);

			ctx->edit_cur_x=l;
		}else
		{
			ctx->edit_cur_x=0;
		}
	}
//	if(ctx->edit_cur_y>=ctx->num_editlines)
//		ctx->edit_cur_y=ctx->num_editlines-1;
	
	if(ctx->edit_cur_y<0)
		ctx->edit_cur_y=0;
	if(ctx->edit_cur_y>ctx->num_editlines)
		ctx->edit_cur_y=ctx->num_editlines;

	cs1=ctx->editlines[ctx->edit_cur_y];
	l=0;
	if(cs1)
		l=strlen(cs1);
	if(ctx->edit_cur_x>l)
	{
		ctx->edit_cur_x=0;
		ctx->edit_cur_y++;
		if(ctx->edit_cur_y>ctx->num_editlines)
			ctx->edit_cur_y=ctx->num_editlines;
	}

	if(ctx->edit_cur_y<ctx->edit_base_y)
		ctx->edit_base_y=ctx->edit_cur_y;
	if(ctx->edit_cur_y>(ctx->edit_base_y+24))
		ctx->edit_base_y=ctx->edit_cur_y-24;

	if(ctx->edit_base_y<0)
		ctx->edit_base_y=0;
	if(ctx->edit_base_y>ctx->num_editlines)
		ctx->edit_base_y=ctx->num_editlines;

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

		j=ctx->edit_base_y+i;
		cs1="";
		if(j<=ctx->num_editlines)
			cs1=ctx->editlines[j];
		if(!cs1)
			cs1="";
			
		if(ctx->edit_base_x)
		{
			l=strlen(cs1);
			if(ctx->edit_base_x<l)
				{ cs1+=ctx->edit_base_x; }
			else
				{ cs1=""; }
		}

		if((j>ctx->edit_sel_sy) && (j<ctx->edit_sel_ey))
		{
			tk_puts("\x1B[46m");
		}

		sprintf(tb, "%04d %s", ctx->edit_base_y+i+1, cs1);
		tk_puts(tb);
		
		if((j==ctx->edit_sel_sy) && (ctx->edit_sel_sy!=ctx->edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH",
				i+1,
				(ctx->edit_sel_sx-ctx->edit_base_x)+6);
			tk_puts(tb);

			tk_puts(cs1+(ctx->edit_sel_sx-ctx->edit_base_x));
		}

		if((j==ctx->edit_sel_ey) && (ctx->edit_sel_sy!=ctx->edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH", i+1, 6);
			tk_puts(tb);

			memcpy(tb, cs1, ctx->edit_sel_ex-ctx->edit_base_x);
			tb[ctx->edit_sel_ex]=0;
			tk_puts(tb);
		}

		if((j==ctx->edit_sel_sy) && (ctx->edit_sel_sy==ctx->edit_sel_ey))
		{
			sprintf(tb, "\x1B[%d;%dH",
				i+1,
				(ctx->edit_sel_sx-ctx->edit_base_x)+6);
			tk_puts(tb);

			memcpy(tb, cs1+
				(ctx->edit_sel_sx-ctx->edit_base_x),
				ctx->edit_sel_ex-ctx->edit_sel_sx);
			tb[ctx->edit_sel_ex-ctx->edit_sel_sx]=0;
			tk_puts(tb);
		}
	}

	sprintf(tb, "\x1B[%d;%dH",
		(ctx->edit_cur_y-ctx->edit_base_y)+1,
		(ctx->edit_cur_x-ctx->edit_base_x)+6);
	tk_puts(tb);
}

int TKSH_EditUpdateLoop(TKSH_EditCtx *ctx)
{
	char tbuf[256];
	char *cs1, *cs2;
	int key, dn, kk;
	int i, j, k, l, brk, dorun;
	int runseq, nxtseq;

	TKSH_EdCheckExpandLines(ctx, 1024);
	TKSH_EdCheckExpandEditbuf(ctx, 1024);
	ctx->edit_redraw=1;

	ctx->edit_base_x=0;
	ctx->edit_base_y=0;

	ctx->edit_cur_x=0;
	ctx->edit_cur_y=0;

	brk=0; dorun=0;
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
				ctx->edit_ctrl=dn;
				continue;
			}

			if(key==TK_K_F5)
			{
				dorun=1;
				continue;
			}
			
			if(ctx->edit_ctrl && (key=='q'))
			{
				brk=1;
				break;
			}

			if(ctx->edit_ctrl && (key=='s'))
			{
				if(ctx->edit_fname)
				{
					TKSH_EdStoreFile(ctx, ctx->edit_fname,
						0, ctx->num_editlines-1);
				}
				continue;
			}
			
			if(!dn)
				continue;

			if((key>=' ') && (key<='~') && !ctx->edit_ctrl)
			{
				cs1=ctx->editlines[ctx->edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=ctx->edit_cur_x;
					memcpy(tbuf, cs1, l+1);
					memcpy(tbuf+j+1, cs1+j, (l-j)+1);
				}else
				{
					memset(tbuf, 0, 16);
					l=0;
					j=0;
				}
				tbuf[j]=key;
				ctx->edit_cur_x++;
				TKSH_EdUpdateLine(ctx, ctx->edit_cur_y, tbuf);
				ctx->edit_redraw=1;
				continue;
			}

			if((key=='\b') && !ctx->edit_ctrl)
			{
				if(ctx->edit_cur_x<=0)
				{
					if(ctx->edit_cur_y>0)
					{
						cs1=ctx->editlines[ctx->edit_cur_y-1];
						cs2=ctx->editlines[ctx->edit_cur_y];
						strcpy(tbuf, cs1);
						strcat(tbuf, cs2);
						TKSH_EdUpdateLine(ctx, ctx->edit_cur_y-1, tbuf);
						TKSH_EdDeleteLine(ctx, ctx->edit_cur_y);
						ctx->edit_cur_x=strlen(cs1);
						ctx->edit_cur_y--;
						ctx->edit_redraw=1;
					}
					continue;
				}
			
				cs1=ctx->editlines[ctx->edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=ctx->edit_cur_x;
					memcpy(tbuf, cs1, l+1);
					memcpy(tbuf+j-1, cs1+j, (l-j)+1);
				}else
				{
					memset(tbuf, 0, 16);
					l=0;
					j=0;
				}
				ctx->edit_cur_x--;
				TKSH_EdUpdateLine(ctx, ctx->edit_cur_y, tbuf);
				ctx->edit_redraw=1;
				continue;
			}

			if((key=='\r') && !ctx->edit_ctrl)
			{
				cs1=ctx->editlines[ctx->edit_cur_y];
				if(cs1)
				{
					l=strlen(cs1);
					j=ctx->edit_cur_x;
					memcpy(tbuf, cs1, l+1);
				}else
				{
					l=0;
					j=0;
				}
				tbuf[j]=0;
				cs2=tbuf+128;
				memcpy(cs2, cs1+j, (l-j)+1);
				if(ctx->edit_cur_y>=ctx->num_editlines)
					TKSH_EdInsertLine(ctx, ctx->edit_cur_y, tbuf);
				else
					TKSH_EdUpdateLine(ctx, ctx->edit_cur_y, tbuf);
				TKSH_EdInsertLine(ctx, ctx->edit_cur_y+1, cs2);
				ctx->edit_cur_y++;
				ctx->edit_cur_x=0;
				ctx->edit_redraw=1;
				continue;
			}

			if(key==TK_K_UPARROW)
			{
				ctx->edit_cur_y--;
				ctx->edit_redraw=1;
				continue;
			}
			if(key==TK_K_DOWNARROW)
			{
				ctx->edit_cur_y++;
				ctx->edit_redraw=1;
				continue;
			}
			if(key==TK_K_LEFTARROW)
			{
				ctx->edit_cur_x--;
				ctx->edit_redraw=1;
				continue;
			}
			if(key==TK_K_RIGHTARROW)
			{
				ctx->edit_cur_x++;
				ctx->edit_redraw=1;
				continue;
			}

			if(key==TK_K_HOME)
			{
				ctx->edit_cur_x=0;
				ctx->edit_redraw=1;
				continue;
			}
			if(key==TK_K_END)
			{
				cs1=ctx->editlines[ctx->edit_cur_y];
				l=strlen(cs1);
				ctx->edit_cur_x=l;
				ctx->edit_redraw=1;
				continue;
			}
		}
		
		if(ctx->edit_redraw)
		{
			ctx->edit_redraw=0;
			TKSH_EditRedraw(ctx);
		}
		
		if(dorun)
		{
#ifdef __BGBCC__
			if(!ctx->basic)
				ctx->basic=TKSH_BasAllocContext();

			TKSH_BasPreScanCommand(ctx->basic, 0, NULL);
			for(j=1; j<=ctx->num_editlines; j++)
			{
				TKSH_BasPreScanCommand(
					ctx->basic, j,
					ctx->editlines[j-1]);
			}

			runseq=1;
			while((runseq>0) && (runseq<=ctx->num_editlines))
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
						ctx->edit_ctrl=dn;
						continue;
					}

					if(ctx->edit_ctrl && (key=='c'))
					{
						runseq=-10;
						continue;
					}
				}

				runseq=TKSH_BasRunCommand(
					ctx->basic, runseq,
					ctx->editlines[runseq-1]);
			}
#endif

			dorun=0;
			ctx->edit_redraw=1;
		}
	}
	return(0);
}
