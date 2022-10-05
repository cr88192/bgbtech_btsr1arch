/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

u32 btm_conbuf[40*25];
byte btm_condown;
byte btm_con_x;
byte btm_con_y;

u32 btm_lastmsg[40*4];

char btm_inputcmd[256];
int btm_input_sx;
int btm_input_x;
short btm_con_tdt;
short btm_con_msgdt;

BTM_ConCmd	*btm_concmds;

int BTM_ConAddCvar(char *name, void *ptr, int pty)
{
	BTM_ConCmd *cmd;
	
	cmd=btm_malloc(sizeof(BTM_ConCmd));
	memset(cmd, 0, sizeof(BTM_ConCmd));
	cmd->name=bccx_strdup(name);
	cmd->cvar=ptr;
	cmd->cvty=pty;
	cmd->uity=pty>>8;
	
	cmd->next=btm_concmds;
	btm_concmds=cmd;
	return(0);
}

int BTM_ConAddCommand(char *name, void *ptr)
{
	BTM_ConCmd *cmd;
	
	cmd=btm_malloc(sizeof(BTM_ConCmd));
	memset(cmd, 0, sizeof(BTM_ConCmd));
	cmd->name=bccx_strdup(name);
	cmd->Run=ptr;
	
	cmd->next=btm_concmds;
	btm_concmds=cmd;
	return(0);
}

int BTM_ConAddCmdVar(char *name, void *run, void *ptr, int pty)
{
	BTM_ConCmd *cmd;
	
	cmd=btm_malloc(sizeof(BTM_ConCmd));
	memset(cmd, 0, sizeof(BTM_ConCmd));
	cmd->name=bccx_strdup(name);
	cmd->Run=run;
	cmd->cvar=ptr;
	cmd->cvty=pty;
	
	cmd->next=btm_concmds;
	btm_concmds=cmd;
	return(0);
}

int BTM_ConScrollUp()
{
	int i;
	
	for(i=0; i<24; i++)
	{
		memcpy(
			btm_conbuf+((i+0)*40),
			btm_conbuf+((i+1)*40),
			40*sizeof(u32));
	}
	memset(btm_conbuf+(24*40), 0, 40*sizeof(u32));
	return(0);
}

int BTM_ConMsgScrollUp()
{
	int i;
	
	btm_con_msgdt=0;
	for(i=0; i<3; i++)
	{
		memcpy(
			btm_lastmsg+((i+0)*40),
			btm_lastmsg+((i+1)*40),
			40*sizeof(u32));
	}
	memset(btm_lastmsg+(3*40), 0, 40*sizeof(u32));
	return(0);
}

int BTM_ConPrintChar(int ch)
{
	u32 blk;

	if(ch<' ')
	{
		if(ch=='\r')
		{
			btm_con_x=0;
			return(0);
		}

		if(ch=='\n')
		{
			btm_con_x=0;
			btm_con_y++;
			BTM_ConMsgScrollUp();
			
			while(btm_con_y>=25)
			{
				BTM_ConScrollUp();
				btm_con_y--;
			}
			return(0);
		}

		return(0);
	}

	blk=ch;
	btm_conbuf[btm_con_y*40+btm_con_x]=blk;
	btm_lastmsg[3*40+btm_con_x]=blk;
	btm_con_x++;
	
	while(btm_con_x>=40)
	{
		btm_con_y++;
		BTM_ConMsgScrollUp();

		if(btm_con_y>=25)
		{
			BTM_ConScrollUp();
			btm_con_y--;
		}
	}

	return(0);
}

int BTM_ConPrintString(char *str)
{
	char *s;
	
	s=str;
	while(*s)
	{
		BTM_ConPrintChar(*s++);
	}
	return(0);
}

int BTM_ConPuts(char *str)
{
	BTM_ConPrintString(str);
	puts(str);
	return(0);
}

int BTM_ConPrintf(char *str, ...)
{
	char tb[512];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	
	BTM_ConPrintString(tb);
	puts(tb);
	return(0);
}

BTM_ConCmd *BTM_LookupCvar(char *name)
{
	BTM_ConCmd	*cmd;

	cmd=btm_concmds;
	while(cmd)
	{
		if(!strcmp(cmd->name, name))
		{
			if(cmd->cvar)
				return(cmd);
		}
		cmd=cmd->next;
	}
	return(NULL);
}

char *BTM_CvarGetStr(char *name)
{
	char tb[256];
	BTM_ConCmd	*cmd;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(NULL);

	switch(cmd->cvty)
	{
	case 0:
		sprintf(tb, "%d", *(s32 *)cmd->cvar);
		break;
	case 1:
		sprintf(tb, "%lld", *(s64 *)cmd->cvar);
		break;
	case 2:
		sprintf(tb, "%f", *(float *)cmd->cvar);
		break;
	case 3:
		sprintf(tb, "%f", *(double *)cmd->cvar);
		break;
	case 4:
		sprintf(tb, "%s", *(char **)cmd->cvar);
		break;

	case 8:
		sprintf(tb, "%d", *(sbyte *)cmd->cvar);
		break;
	case 9:
		sprintf(tb, "%d", *(byte *)cmd->cvar);
		break;
	case 10:
		sprintf(tb, "%d", *(s16 *)cmd->cvar);
		break;
	case 11:
		sprintf(tb, "%d", *(u16 *)cmd->cvar);
		break;

	case 0x3F:
		sprintf(tb, "%s", (*(byte *)cmd->cvar)?"true":"false");
		break;
	}
	return(bccx_strdup(tb));
}

s64 BTM_CvarGetInt(char *name)
{
	char tb[256];
	BTM_ConCmd	*cmd;
	s64 li;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(0);

	li=0;
	switch(cmd->cvty)
	{
	case 0:
		li=*(s32 *)cmd->cvar;
		break;
	case 1:
		li=*(s64 *)cmd->cvar;
		break;
	case 2:
		li=*(float *)cmd->cvar;
		break;
	case 3:
		li=*(double *)cmd->cvar;
		break;
	case 4:
		break;

	case 8:
		li=*(sbyte *)cmd->cvar;
		break;
	case 9:
		li=*(byte *)cmd->cvar;
		break;
	case 10:
		li=*(s16 *)cmd->cvar;
		break;
	case 11:
		li=*(u16 *)cmd->cvar;
		break;

	case 0x3F:
		li=(*(byte *)cmd->cvar);
		break;
	}
	return(li);
}

char *BTM_CvarGetStrUI(char *name)
{
	char tb[256];
	BTM_ConCmd	*cmd;
	s64 li;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(0);

	if(cmd->uity==1)
	{
		li=BTM_CvarGetInt(name);
		sprintf(tb, "%d%%", (int)li);
		return(bccx_strdup(tb));
	}
	
	if(cmd->uity==2)
	{
		li=BTM_CvarGetInt(name);
		sprintf(tb, "%d%%", (((int)li)*100)/255);
		return(bccx_strdup(tb));
	}
	
	return(BTM_CvarGetStr(name));
}

int BTM_CvarSetStr(char *name, char *val)
{
	char tb[256];
	BTM_ConCmd	*cmd;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(0);

	switch(cmd->cvty)
	{
	case 0:
		*(s32 *)cmd->cvar=bccx_atoll(val);
		break;
	case 1:
		*(s64 *)cmd->cvar=bccx_atoll(val);
		break;
	case 2:
		*(float *)cmd->cvar=bccx_atof(val);
		break;
	case 3:
		*(double *)cmd->cvar=bccx_atof(val);
		break;
	case 4:
		*(char **)cmd->cvar=bccx_strdup(val);
		break;

	case 8:		case 9:
		*(byte *)cmd->cvar=bccx_atoll(val);
		break;
	case 10:	case 11:
		*(u16 *)cmd->cvar=bccx_atoll(val);
		break;

	case 0x3F:
		if(	!strcmp(val, "false") ||
			!strcmp(val, "0") ||
			!strcmp(val, "no"))
		{
			*(byte *)cmd->cvar=0;
		}else
		{
			*(byte *)cmd->cvar=1;
		}
		break;
	}
	return(0);
}

int BTM_CvarSetInt(char *name, int val)
{
	char tb[256];
	BTM_ConCmd	*cmd;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(0);

	switch(cmd->cvty)
	{
	case 0:
		*(s32 *)cmd->cvar=val;
		break;
	case 1:
		*(s64 *)cmd->cvar=val;
		break;
	case 2:
		*(float *)cmd->cvar=val;
		break;
	case 3:
		*(double *)cmd->cvar=val;
		break;
	case 4:
		break;

	case 8:
		*(sbyte *)cmd->cvar=val;
		break;
	case 9:
		*(byte *)cmd->cvar=val;
		break;
	case 10:
		*(s16 *)cmd->cvar=val;
		break;
	case 11:
		*(u16 *)cmd->cvar=val;
		break;

	case 0x3F:
		*(byte *)cmd->cvar=val;
		break;
	}
	return(0);
}

int BTM_CvarNudge(char *name, int key)
{
	char tb[256];
	BTM_ConCmd	*cmd;
	s64 li;

	cmd=BTM_LookupCvar(name);
	if(!cmd)
		return(0);

	if(cmd->cvty==0x3F)
	{
		*(byte *)cmd->cvar=!(*(byte *)cmd->cvar);
		return(0);
	}
	
	if(cmd->uity==1)
	{
		li=BTM_CvarGetInt(name);
		if(key==K_LEFTARROW)
		{
			li=li-5;
			if(li<0)li=0;
		}
		if(key==K_RIGHTARROW)
		{
			li=li+5;
			if(li>100)li=100;
		}
		BTM_CvarSetInt(name, li);
		return(0);
	}

	if(cmd->uity==2)
	{
		li=BTM_CvarGetInt(name);
		if(key==K_LEFTARROW)
		{
			li=li-12;
			if(li<0)li=0;
		}
		if(key==K_RIGHTARROW)
		{
			li=li+12;
			if(li>255)li=255;
		}
		BTM_CvarSetInt(name, li);
		return(0);
	}
	
	
	return(0);
}

int BTM_ConsoleCommand(char *str)
{
	char tb[256];
	BTM_ConCmd	*cmd;
	char **a;
	
	a=bccx_split(str);
	if(!a[0])
		return(0);

	cmd=btm_concmds;
	while(cmd)
	{
		if(!strcmp(cmd->name, a[0]))
		{
			if(cmd->Run)
				return(cmd->Run(cmd, a));

			if(cmd->cvar)
			{
				if(a[1])
				{
					BTM_CvarSetStr(cmd->name, a[1]);
					return(0);
				}else
				{
					BTM_ConPrintf("%s\n", BTM_CvarGetStr(cmd->name));
					return(0);
				}
			}
		}
		cmd=cmd->next;
	}

	sprintf(tb, "unhandled '%s'\n", str);
	BTM_ConPrintString(tb);
	return(0);
}

int BTM_ConHandleKey(u16 key)
{
	if(key=='`')
	{
		btm_condown=!btm_condown;
		return(0);
	}

	if(!btm_condown)
		return(0);
	
	if((key>=' ') && (key<='~'))
	{
		memmove(
			btm_inputcmd+btm_input_x+1, 
			btm_inputcmd+btm_input_x+0, (254-btm_input_x));
		btm_inputcmd[btm_input_x]=key;
		btm_input_x++;
	}
	
	if(key==K_ENTER)
	{
		BTM_ConsoleCommand(btm_inputcmd);
	
		btm_input_x=0;
		btm_input_sx=0;
		memset(btm_inputcmd, 0, 256);
	}
	
	if(key==K_HOME)
	{
		btm_input_x=0;
	}

	if(key==K_END)
	{
		btm_input_x=strlen(btm_inputcmd);
	}
	
	if(key==K_LEFTARROW)
	{
		if(btm_input_x>0)
			btm_input_x--;
	}
	
	if(key==K_RIGHTARROW)
	{
		if(btm_input_x<strlen(btm_inputcmd))
			btm_input_x++;
	}

	if(key==K_BACKSPACE)
	{
		if(btm_input_x>0)
		{
			btm_input_x--;
			memmove(
				btm_inputcmd+btm_input_x+0, 
				btm_inputcmd+btm_input_x+1, (254-btm_input_x));
		}
	}

	if(key==K_DEL)
	{
		memmove(
			btm_inputcmd+btm_input_x+0, 
			btm_inputcmd+btm_input_x+1, (254-btm_input_x));
	}


	while(btm_input_x<btm_input_sx)
		btm_input_sx--;
	while(btm_input_x>=(btm_input_sx+40))
		btm_input_sx++;

	return(0);
}

int BTM_ConDownP()
{
	return(btm_condown);
}

int BTM_DrawConsole()
{
	float f0, f1, f2, f3;
	int i, j, k;

	if(!btm_condown)
	{
		BTM_DrawCharPrim(0, 0, 0, 0, 0, 0, 0);

		for(i=0; i<4; i++)
			for(j=0; j<40; j++)
		{
			k=btm_lastmsg[i*40+j];
			if(!(k&255))
				continue;
			BTM_DrawCharPrim(
				(-160)+j*8, 100-((i+1)*8), 8, 8,
				k&255, btm_texfont_8px, 0xFFFFFFFFU);
		}

		BTM_DrawCharPrim(0, 0, 0, 0, 0, 0, 0);
		return(0);
	}

	BTM_DrawCharPrim(0, 0, 0, 0, 0, 0, 0);

//	pglEnable(GL_BLEND);
//	pglDisable(GL_ALPHA_TEST);

	f0=(9.0/16.0)+(1.0/256.0);
	f1=(10.0/16.0)-(1.0/256.0);
	f2=(3.0/16.0)+(1.0/256.0);
	f3=(4.0/16.0)-(1.0/256.0);

	pglColor4f(0.2, 0.2, 0.2, 0.75);
	pglBindTexture(GL_TEXTURE_2D, 2);
	pglBegin(GL_QUADS);
	pglTexCoord2f(f0, f2);
	pglVertex3f(-160, 100, 10);
	pglTexCoord2f(f0, f3);
	pglVertex3f(-160, -100, 10);
	pglTexCoord2f(f1, f3);
	pglVertex3f(160, -100, 10);
	pglTexCoord2f(f1, f2);
	pglVertex3f(160, 100, 10);

	pglEnd();

	pglDisable(GL_BLEND);
	pglEnable(GL_ALPHA_TEST);

	for(i=0; i<25; i++)
		for(j=0; j<40; j++)
	{
		k=btm_conbuf[i*40+j];
		if(!(k&255))
			continue;
		BTM_DrawCharPrim(
			(-160)+j*8, 100-((i+1)*8), 8, 8,
			k&255, btm_texfont_8px, 0xFFFFFFFFU);
	}
	
	for(j=0; j<40; j++)
	{
		k=btm_inputcmd[btm_input_sx+j];

		BTM_DrawCharPrim(
			(-160)+j*8, 100-(24*8), 8, 8,
			k&255, btm_texfont_8px, 0xFFFFFFFFU);
	}
	
	j=btm_input_x-btm_input_sx;

	if(btm_con_tdt&512)
	{
		BTM_DrawCharPrim(
			(-160)+j*8-2, 100-(24*8)-2, 8, 12,
			'I', btm_texfont_8px, 0xFFFFFFFFU);
	}

	BTM_DrawCharPrim(0, 0, 0, 0, 0, 0, 0);
	return(0);
}

int BTM_ConDoInput(int dt)
{
	btm_con_tdt+=dt;

	btm_con_msgdt+=dt;
	if(btm_con_msgdt>=4000)
	{
		BTM_ConMsgScrollUp();
	}
	return(0);
}
