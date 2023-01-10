int bjx2_dbgcon_cx;
int bjx2_dbgcon_cy;

char bjx2_dbgcon_cmd[256];
byte bjx2_dbgcon_cmdpos;


int BJX2_DebugConScrollUp(BJX2_Context *ctx)
{
	int x, y, z;
	
	for(y=0; y<49; y++)
	{
		for(x=0; x<80; x++)
		{
			for(z=0; z<8; z++)
			{
				jx2i_gfxcon_dbgconbuf[((y+0)*80+x)*8+z] =
					jx2i_gfxcon_dbgconbuf[((y+1)*80+x)*8+z];
			}
		}
	}

	for(x=0; x<80; x++)
	{
		for(z=0; z<8; z++)
		{
			jx2i_gfxcon_dbgconbuf[(49*80+x)*8+z] = 0;
		}
	}
	return(0);
}

int BJX2_DebugConScrollAdjust(BJX2_Context *ctx)
{
	while(bjx2_dbgcon_cy>=50)
	{
		BJX2_DebugConScrollUp(ctx);
		bjx2_dbgcon_cy--;
	}
	return(0);
}

int BJX2_DebugConClear(BJX2_Context *ctx)
{
	bjx2_dbgcon_cy=100;
	BJX2_DebugConScrollAdjust(ctx);

	bjx2_dbgcon_cx=0;
	bjx2_dbgcon_cy=0;
}

int BJX2_DebugConPutc(BJX2_Context *ctx, int ch)
{
	int v, x, y;

	jx2i_gfxcon_dirty=1;

	if(ch<' ')
	{
		if(ch=='\r')
		{
			bjx2_dbgcon_cx=0;
			jx2i_gfxcon_dbgcursor=(bjx2_dbgcon_cy<<8)|(bjx2_dbgcon_cx);
			return(0);
		}

		if(ch=='\b')
		{
			if(bjx2_dbgcon_cx>0)
				bjx2_dbgcon_cx--;
			jx2i_gfxcon_dbgcursor=(bjx2_dbgcon_cy<<8)|(bjx2_dbgcon_cx);
			return(0);
		}

		if(ch=='\n')
		{
			bjx2_dbgcon_cx=0;
			bjx2_dbgcon_cy++;
			BJX2_DebugConScrollAdjust(ctx);
			jx2i_gfxcon_dbgcursor=(bjx2_dbgcon_cy<<8)|(bjx2_dbgcon_cx);
			return(0);
		}
	
		return(0);
	}
	
	y=bjx2_dbgcon_cy;
	x=bjx2_dbgcon_cx++;
	if(x>=80)
	{
		bjx2_dbgcon_cx=0;
		x=bjx2_dbgcon_cx++;
		bjx2_dbgcon_cy++;
		BJX2_DebugConScrollAdjust(ctx);
		y=bjx2_dbgcon_cy;
	}
	
	v=0x003F0000|ch;
	
//	if(ch=='\x7F')
//	{
//		v=0x003F0200|'_';
//	}
	
	jx2i_gfxcon_dbgconbuf[(y*80+x)*8+0]=v;

	jx2i_gfxcon_dbgcursor=(y<<8)|(x+1);
	return(0);
}

int BJX2_DebugConPuts(BJX2_Context *ctx, char *str)
{
	char *s;
	s=str;
	while(*s)
	{
		BJX2_DebugConPutc(ctx, *s++);
	}
	return(0);
}

int BJX2_DebugConVPrintf(BJX2_Context *ctx, char *str, va_list lst)
{
	char tb[256];
	vsprintf(tb, str, lst);
	BJX2_DebugConPuts(ctx, tb);
	return(0);
}

int BJX2_DebugConPrintf(BJX2_Context *ctx, char *str, ...)
{
	char tb[256];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	BJX2_DebugConPuts(ctx, tb);
	return(0);
}

int BJX2_GfxDebugPrintHexdump(BJX2_Context *ctx, bjx2_addr pc, int sz)
{
	byte tba[16];
	char tb[256];
	u64 v0, v1;
	char *s, *t;
	int i, j, k, n;
	
	n=(sz+15)/16;
	for(i=0; i<n; i++)
	{
		v0=BJX2_MemGetQWord(ctx, pc+i*16+0);
		v1=BJX2_MemGetQWord(ctx, pc+i*16+8);
		memcpy(tba+0, &v0, 8);
		memcpy(tba+8, &v1, 8);
		
		t=tb;
		sprintf(t, "%08X  ", (pc+i*16)&0xFFFFFFFF);
		t+=strlen(t);

		for(j=0; j<16; j++)
		{
			if((i*16+j)>=sz)
			{
				sprintf(t, "   ");
			}else
			{
				sprintf(t, "%02X ", tba[j]);
			}
			t+=strlen(t);
			if(j && !(j&3))
				*t++=' ';
			if(j && !(j&7))
				*t++=' ';
		}

		for(j=0; j<16; j++)
		{
			k=tba[j];
			if((k<' ') || (k>'~'))
				k='.';
			*t++=k;
		}
		*t=0;

		BJX2_DbgPrintf(ctx, "%s\n", tb);
	}
	
	return(0);
}

s64 JX2R_Ratoi(char *str);

int BJX2_GfxDebugRegForName(BJX2_Context *ctx, char *name)
{
	char *s;
	int j, k;
	
	s=name;
	k=-1;

	if(((s[0]=='r') || (s[0]=='R')) &&
		((s[1]>='0') || (s[1]<='9')))
	{
		j=atoi(s+1);
		if((j>=0) && (j<64))
		{
			k=BJX2_REG_R0+j;
			if(j==0)	k=BJX2_REG_DLR;
			if(j==1)	k=BJX2_REG_DHR;
			if(j==15)	k=BJX2_REG_SP;
		}
	}
	
	if(!strcmp(s, "sp"))
		k=BJX2_REG_SP;
	if(!strcmp(s, "gbr"))
		k=BJX2_REG_GBR;
	if(!strcmp(s, "dlr"))
		k=BJX2_REG_DLR;
	if(!strcmp(s, "dhr"))
		k=BJX2_REG_DHR;
	return(k);
}

s64 BJX2_GfxDebugValueForName(BJX2_Context *ctx, char *name)
{
	char tb1[256];
	char tb2[256];
	bjx2_addr pc;
	s64 v1, v2, v3;
	char *s;
	int k;
	
	s=name;
	while(*s)
	{
		if(	(*s=='+') || (*s=='-') ||
			(*s=='*') || (*s=='/') ||
			(*s=='&') || (*s=='|') ||
			(*s=='^') )
				break;
		s++;
	}
	
	if(*s)
	{
		memcpy(tb1, name, s-name);
		strcpy(tb2, s+1);
		tb1[s-name]=0;
		
		v1=BJX2_GfxDebugValueForName(ctx, tb1);
		v2=BJX2_GfxDebugValueForName(ctx, tb2);
		
		if(*s=='+')
			v3=v1+v2;
		if(*s=='-')
			v3=v1-v2;
		if(*s=='*')
			v3=v1*v2;
		if(*s=='/')
			v3=v1/v2;
		if(*s=='&')
			v3=v1&v2;
		if(*s=='|')
			v3=v1|v2;
		if(*s=='^')
			v3=v1^v2;
		return(v3);
	}
	
	
	if((name[0]>='0') && (name[0]<='9'))
	{
		return(JX2R_Ratoi(name));
	}
	
	k=BJX2_GfxDebugRegForName(ctx, name);
	if(k>=0)
	{
		return(ctx->regs[k]);
	}

	pc=BJX2_DbgAddrForName(ctx, name);
	if(pc>0)
	{
		return(pc);
	}
	
	return(0);
}

int BJX2_GfxDebugRunCommand(BJX2_Context *ctx, char *cmd)
{
	bjx2_addr pc;
	BJX2_Trace *tr;
	char **a;
	char *s, *t;
	u64 val, v0, v1;
	int i, j, k, sz;
	
	a=JX2R_SplitLine(cmd);
	if(!a[0])
		return(0);
	
	if(!strcmp(a[0], "quit"))
	{
		ctx->req_kill=1;
		return(0);
	}

	if(!strcmp(a[0], "c") ||
		!strcmp(a[0], "continue"))
	{
		jx2i_gfxcon_isdbg=0;
		return(0);
	}

	if(!strcmp(a[0], "list"))
	{
		if(a[1])
		{
			pc=BJX2_GfxDebugValueForName(ctx, a[1]);
		}else
		{
			pc=ctx->regs[BJX2_REG_PC];
		}

		tr=BJX2_GetTraceForAddr(ctx, pc, 0);
		BJX2_DbgPrintTrace(ctx, tr);
		return(0);
	}

	if(!strcmp(a[0], "next"))
	{
		BJX2_RunLimit(ctx, 2);
		pc=ctx->regs[BJX2_REG_PC];
		BJX2_DebugConPrintf(ctx, "Debug PC=%012llX\n", pc);
		return(0);
	}

	if(!strcmp(a[0], "step"))
	{
		BJX2_RunLimit(ctx, 1);
		pc=ctx->regs[BJX2_REG_PC];
		BJX2_DebugConPrintf(ctx, "Debug PC=%012llX\n", pc);
		return(0);
	}

	if(!strcmp(a[0], "regs"))
	{
		BJX2_DbgPrintRegs(ctx);
		return(0);
	}

	if(!strcmp(a[0], "print"))
	{
		s=a[1]; k=-1;
		
		if(!s)
			return(0);
		
		if(((s[0]=='r') || (s[0]=='R')) &&
			((s[1]>='0') || (s[1]<='9')))
		{
			j=atoi(s+1);
			if((j>=0) && (j<64))
			{
				k=BJX2_REG_R0+j;
				if(j==0)	k=BJX2_REG_DLR;
				if(j==1)	k=BJX2_REG_DHR;
				if(j==15)	k=BJX2_REG_SP;
			}
		}
		
		if(!strcmp(s, "sp"))
			k=BJX2_REG_SP;
		if(!strcmp(s, "gbr"))
			k=BJX2_REG_GBR;
		if(!strcmp(s, "dlr"))
			k=BJX2_REG_DLR;
		if(!strcmp(s, "dhr"))
			k=BJX2_REG_DHR;
		
		if(k>=0)
		{
			pc=ctx->regs[k];
			BJX2_DbgPrintf(ctx, "%016llX\n", pc);
			return(0);
		}
		
		pc=BJX2_DbgAddrForName(ctx, s);
		if(pc>0)
		{
			sz=0;
			BJX2_DbgNameForAddrSz(ctx, pc, NULL, &sz);
			
			switch(sz)
			{
			case 1:
				j=BJX2_MemGetByte(ctx, pc);
				BJX2_DbgPrintf(ctx, "%02X\n", j);
				break;
			case 2:
				j=BJX2_MemGetWord(ctx, pc);
				BJX2_DbgPrintf(ctx, "%04X\n", j);
				break;
			case 4:
				j=BJX2_MemGetDWord(ctx, pc);
				BJX2_DbgPrintf(ctx, "%08X\n", j);
				break;
			case 8:
				val=BJX2_MemGetQWord(ctx, pc);
				BJX2_DbgPrintf(ctx, "%016llX\n", val);
				break;
			case 16:
				if(!(pc&15))
				{
					BJX2_MemGetXWord(ctx, pc, &v0, &v1);
					BJX2_DbgPrintf(ctx, "%016llX-%016llX\n", v1, v0);
					break;
				}
			default:
				k=sz;
				if(k<=0)
					k=256;
				if(k>256)
					k=256;
			
				BJX2_GfxDebugPrintHexdump(ctx, pc, k);
				break;
			}
			return(0);
		}
	}

	if(!strcmp(a[0], "dump"))
	{
//		pc=JX2R_Ratoi(a[1]);
		pc=BJX2_GfxDebugValueForName(ctx, a[1]);
		if(a[2])
		{
			k=JX2R_Ratoi(a[2]);
		}else
		{
			k=256;
		}
		BJX2_GfxDebugPrintHexdump(ctx, pc, k);
		return(0);
	}

	
	if(	!strcmp(a[0], "help")	||
		!strcmp(a[0], "h")		)
	{
		BJX2_DbgPrintf(ctx, "next           Run current trace\n");
		BJX2_DbgPrintf(ctx, "step           Step one instruction\n");
		BJX2_DbgPrintf(ctx, "list [addr]    List current trace\n");
		BJX2_DbgPrintf(ctx, "regs           Dump registers\n");
		BJX2_DbgPrintf(ctx, "dump addr [sz] Hex Dump\n");
		BJX2_DbgPrintf(ctx, "quit           Exit VM\n");
		BJX2_DbgPrintf(ctx, "c              Continue running\n");
		BJX2_DbgPrintf(ctx, "print [var]    Print a variable\n");
		BJX2_DbgPrintf(ctx, "help [topic]   Help on a given command\n");
	
//		BJX2_DbgPrintRegs(ctx);
		return(0);
	}
	
	BJX2_DbgPrintf(ctx, "Unhandled Command '%s'\n", cmd);
	return(0);
}

int BJX2_GfxDebugAddKey(BJX2_Context *ctx, int key)
{
	if((key>=' ') && (key<='~'))
	{
		if(bjx2_dbgcon_cmdpos>=75)
			return(0);
	
		bjx2_dbgcon_cx=bjx2_dbgcon_cmdpos+2;
		BJX2_DebugConPutc(ctx, key);
//		BJX2_DebugConPutc(ctx, '\x7F');
		BJX2_DebugConPuts(ctx, " \b");

		bjx2_dbgcon_cmd[bjx2_dbgcon_cmdpos++]=key;
		bjx2_dbgcon_cmd[bjx2_dbgcon_cmdpos]=0;
	}
	
	if(key=='\b')
	{
		if(bjx2_dbgcon_cmdpos>0)
		{
			bjx2_dbgcon_cx=bjx2_dbgcon_cmdpos+2;
//			BJX2_DebugConPutc(ctx, ' ');
			BJX2_DebugConPuts(ctx, " \b");

			bjx2_dbgcon_cmdpos--;
			bjx2_dbgcon_cmd[bjx2_dbgcon_cmdpos]=0;

			bjx2_dbgcon_cx=bjx2_dbgcon_cmdpos+2;
//			BJX2_DebugConPutc(ctx, '\x7F');
			BJX2_DebugConPuts(ctx, " \b");
		}
	}

	if(key=='\r')
	{
		bjx2_dbgcon_cx=bjx2_dbgcon_cmdpos+2;
		BJX2_DebugConPutc(ctx, ' ');
		BJX2_DebugConPutc(ctx, '\n');

//		BJX2_DebugConPrintf(ctx, "Command %s\n", bjx2_dbgcon_cmd);
		BJX2_GfxDebugRunCommand(ctx, bjx2_dbgcon_cmd);
		
		BJX2_DebugConPrintf(ctx, "$  \b");
		bjx2_dbgcon_cmdpos=0;
	}
}

int BJX2_GfxDebugRefresh(BJX2_Context *ctx)
{
}

int BJX2_GfxDebugActivate(BJX2_Context *ctx)
{
	bjx2_addr pc;

	if(!jx2i_gfxcon_isdbg)
		return(0);

	BJX2_DebugConClear(ctx);

	pc=ctx->regs[BJX2_REG_PC];
	BJX2_DebugConPrintf(ctx, "Debug PC=%012llX\n", pc);
	
//	BJX2_DebugConPrintf(ctx, "$ \x7F");
	BJX2_DebugConPrintf(ctx, "$  \b");
	return(0);
}
