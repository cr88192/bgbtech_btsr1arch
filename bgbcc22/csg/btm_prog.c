int btm_pgmtok_goto;
int btm_pgmtok_if;
int btm_pgmtok_then;
int btm_pgmtok_else;
int btm_pgmtok_end;
int btm_pgmtok_stop;
int btm_pgmtok_print;
int btm_pgmtok_rem;
int btm_pgmtok_dim;
int btm_pgmtok_let;
int btm_pgmtok_gosub;
int btm_pgmtok_return;
int btm_pgmtok_for;
int btm_pgmtok_next;
int btm_pgmtok_to;
int btm_pgmtok_as;
int btm_pgmtok_step;
int btm_pgmtok_temp;

int btm_pgmtok_vec;
int btm_pgmtok_vec2;
int btm_pgmtok_vec3;
int btm_pgmtok_vec4;

int btm_pgmtok_opr_add;
int btm_pgmtok_opr_sub;
int btm_pgmtok_opr_mul;
int btm_pgmtok_opr_div;
int btm_pgmtok_opr_mod;
int btm_pgmtok_opr_and;
int btm_pgmtok_opr_or;
int btm_pgmtok_opr_xor;
int btm_pgmtok_opr_shl;
int btm_pgmtok_opr_shr;
int btm_pgmtok_opr_not;
int btm_pgmtok_opr_bitnot;

int btm_pgmtok_opr_eq;
int btm_pgmtok_opr_ne;
int btm_pgmtok_opr_gt;
int btm_pgmtok_opr_le;
int btm_pgmtok_opr_lt;
int btm_pgmtok_opr_ge;

int btm_pgmtok_opr_set;
int btm_pgmtok_opr_comma;
int btm_pgmtok_opr_colon;
int btm_pgmtok_opr_semicolon;

int btm_pgmtok_br_lparen;
int btm_pgmtok_br_rparen;
int btm_pgmtok_br_lbrack;
int btm_pgmtok_br_rbrack;

u64 BTM_PgmDoEvalExpr(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs);
u64 BTM_PgmEvalExpr(BTM_ProgCtx *wrl, char *str);

BTM_ConPgm *btm_conpgms;
BTM_PgmExt *btm_pgm_exts;

BTM_ProgCtx *BTM_PgmAllocContext()
{
	BTM_ProgCtx *tmp;
	tmp=malloc(sizeof(BTM_ProgCtx));
	memset(tmp, 0, sizeof(BTM_ProgCtx));
	return(tmp);
}

BTM_PgmExt *BTM_PgmLookupExt(int tok, int lvl)
{
	BTM_PgmExt *cur;
	
	cur=btm_pgm_exts;
	while(cur)
	{
		if((cur->token==tok) && (cur->level==lvl))
			return(cur);
		cur=cur->next;
	}
	
	return(NULL);
}

BTM_PgmExt *BTM_PgmGetExt(int tok, int lvl)
{
	BTM_PgmExt *cur;
	
	cur=btm_pgm_exts;
	while(cur)
	{
		if((cur->token==tok) && (cur->level==lvl))
			return(cur);
		cur=cur->next;
	}
	
	cur=btm_malloc(sizeof(BTM_PgmExt));
	memset(cur, 0, sizeof(BTM_PgmExt));
	cur->token=tok;
	cur->level=lvl;
	cur->next=btm_pgm_exts;
	btm_pgm_exts=cur;
	return(cur);
}

BTM_PgmExt *BTM_PgmAddExt(int tok, int lvl, void *func)
{
	BTM_PgmExt *cur;
	cur=BTM_PgmGetExt(tok, lvl);
	cur->Run=func;
	return(cur);
}

BTM_PgmExt *BTM_PgmAddExtInfix(int tok, int lvl, void *func)
{
	BTM_PgmExt *cur;
	cur=BTM_PgmGetExt(tok, lvl);
	cur->Infix=func;
	return(cur);
}

int BTM_PgmInterpInit()
{
	if(btm_pgmtok_print>0)
		return(0);

	BTM_VarInit();

	btm_pgmtok_goto				=BTM_PgmInternToken("Igoto");
	btm_pgmtok_if				=BTM_PgmInternToken("Iif");
	btm_pgmtok_then				=BTM_PgmInternToken("Ithen");
	btm_pgmtok_else				=BTM_PgmInternToken("Ielse");
	btm_pgmtok_end				=BTM_PgmInternToken("Iend");
	btm_pgmtok_stop				=BTM_PgmInternToken("Istop");
	btm_pgmtok_print			=BTM_PgmInternToken("Iprint");
	btm_pgmtok_rem				=BTM_PgmInternToken("Irem");
	btm_pgmtok_dim				=BTM_PgmInternToken("Idim");
	btm_pgmtok_let				=BTM_PgmInternToken("Ilet");
	btm_pgmtok_gosub			=BTM_PgmInternToken("Igosub");
	btm_pgmtok_return			=BTM_PgmInternToken("Ireturn");
	btm_pgmtok_for				=BTM_PgmInternToken("Ifor");
	btm_pgmtok_next				=BTM_PgmInternToken("Inext");
	btm_pgmtok_to				=BTM_PgmInternToken("Ito");
	btm_pgmtok_as				=BTM_PgmInternToken("Ias");
	btm_pgmtok_step				=BTM_PgmInternToken("Istep");
	btm_pgmtok_temp				=BTM_PgmInternToken("Itemp");

	btm_pgmtok_vec				=BTM_PgmInternToken("Ivec");
//	btm_pgmtok_vec2				=BTM_PgmInternToken("Ivec2");
//	btm_pgmtok_vec3				=BTM_PgmInternToken("Ivec3");
//	btm_pgmtok_vec4				=BTM_PgmInternToken("Ivec4");

	btm_pgmtok_opr_add			=BTM_PgmInternToken("O+");
	btm_pgmtok_opr_sub			=BTM_PgmInternToken("O-");
	btm_pgmtok_opr_mul			=BTM_PgmInternToken("O*");
	btm_pgmtok_opr_div			=BTM_PgmInternToken("O/");
	btm_pgmtok_opr_mod			=BTM_PgmInternToken("O%");
	btm_pgmtok_opr_and			=BTM_PgmInternToken("O&");
	btm_pgmtok_opr_or			=BTM_PgmInternToken("O|");
	btm_pgmtok_opr_xor			=BTM_PgmInternToken("O^");
	btm_pgmtok_opr_shl			=BTM_PgmInternToken("O<<");
	btm_pgmtok_opr_shr			=BTM_PgmInternToken("O>>");

	btm_pgmtok_opr_eq			=BTM_PgmInternToken("O==");
	btm_pgmtok_opr_ne			=BTM_PgmInternToken("O!=");
	btm_pgmtok_opr_gt			=BTM_PgmInternToken("O>");
	btm_pgmtok_opr_le			=BTM_PgmInternToken("O<=");
	btm_pgmtok_opr_lt			=BTM_PgmInternToken("O<");
	btm_pgmtok_opr_ge			=BTM_PgmInternToken("O>=");

	btm_pgmtok_opr_not			=BTM_PgmInternToken("O!");
	btm_pgmtok_opr_bitnot		=BTM_PgmInternToken("O~");

	btm_pgmtok_opr_set			=BTM_PgmInternToken("O=");
	btm_pgmtok_opr_comma		=BTM_PgmInternToken("O,");
	btm_pgmtok_opr_colon		=BTM_PgmInternToken("O:");
	btm_pgmtok_opr_semicolon	=BTM_PgmInternToken("O;");

	btm_pgmtok_br_lparen		=BTM_PgmInternToken("B(");
	btm_pgmtok_br_rparen		=BTM_PgmInternToken("B)");
	btm_pgmtok_br_lbrack		=BTM_PgmInternToken("B[");
	btm_pgmtok_br_rbrack		=BTM_PgmInternToken("B]");

	return(1);
}

int BTM_PgmInternToken(char *str)
{
	char *keywords[]={
		"Iabs", "Iatn",
		"Icall", "Icos",
		"Idim", "Ido",
		"Ielse", "Iend", "Iexp",
		"Ifor",
		"Igosub", "Igoto",
		"Iif", "Iint",
		"Ilet", "Ilog", "Iloop",
		"Inext",
		"Iprint",
		"Ireturn", "Irnd",
		"Isin", "Isqr", "Istop",
		"Itan", "Ithen", "Ito",
		NULL};
	char tbuf[256];
	char *s0, *str1, *cs, *ct;
	int i, j, k, c;

	if(str[0]=='I')
	{
		c=str[1];
		if((c>='A') && (c<='Z'))
		{
			cs=str; ct=tbuf;
			*ct++=*cs++;
			while(*cs)
				{ *ct++=tolower(*cs++); }
			*ct++=0;
			str1=NULL;
			
			for(i=0; ; i++)
			{
				s0=keywords[i];
				if(!s0)
					break;
				if(!strcmp(tbuf, s0))
					{ str1=s0; break; }
			}

			if(str1)
				return(bccx_strdup_i(str1));
		}
	}

	return(bccx_strdup_i(str));
}

int BTM_PgmFlushPrograms()
{
	btm_conpgms=NULL;
	return(0);
}

BTM_ConPgm *BTM_PgmLookupProgram(char *name)
{
	BTM_ConPgm *cmd;
	
	cmd=btm_conpgms;
	while(cmd)
	{
		if(!strcmp(cmd->name, name))
			return(cmd);
		cmd=cmd->next;
	}
	return(NULL);
}

BTM_ConPgm *BTM_PgmGetProgram(char *name)
{
	BTM_ConPgm *cmd;
	
	cmd=BTM_PgmLookupProgram(name);
	if(cmd)
		return(cmd);

	cmd=btm_malloc(sizeof(BTM_ConPgm));
	memset(cmd, 0, sizeof(BTM_ConPgm));
	cmd->name=bccx_strdup(name);

	cmd->next=btm_conpgms;
	btm_conpgms=cmd;

	return(cmd);
}	

BTM_ConPgm *BTM_PgmGetReInitProgram(char *name)
{
	BTM_ConPgm *cmd;

	cmd=BTM_PgmGetProgram(name);

	if(cmd->ltokb)
	{
		free(cmd->ltokb);
		free(cmd->ltoki);
		free(cmd->lbl_id);
		free(cmd->lbl_ix);

		cmd->ltokb=NULL;
		cmd->ltoki=NULL;
		cmd->lbl_id=NULL;
		cmd->lbl_ix=NULL;
	}
	
	return(cmd);
}

BTM_ConPgm *BTM_PgmLoadParse(char *name, char *sbuf, int szbuf)
{
	char tbuf[256];
	u16 toka[128];
	u16 *tokb, *toki, *lblid, *lblix;
	BTM_ConPgm *cmd;
	char **a;
	char *cs, *ct, *cse;
	int mtokb, ntokb;
	int mtoki, ntoki;
	int mlbl, nlbl;
	int tok_colon;
	int i, j, k;

	BTM_PgmInterpInit();

	cmd=BTM_PgmGetReInitProgram(name);
	
	mtokb=16384;
	mtoki=4096;
	tokb=malloc(mtokb*sizeof(u16));
	toki=malloc(mtoki*sizeof(u16));
	ntokb=0;
	ntoki=0;

	mlbl=256;
	lblid=malloc(mlbl*sizeof(u16));
	lblix=malloc(mlbl*sizeof(u16));
	nlbl=0;

	cs=sbuf;
	cse=sbuf+szbuf;
	while((cs<cse) && *cs)
	{
		while((cs<cse) && *cs && *cs<=' ')
			cs++;
		ct=tbuf;
		while((cs<cse) && (*cs!='\r') && (*cs!='\n'))
		{
			if((*cs=='\\') && ((cs[1]=='\r') || (cs[1]=='\n')))
			{
				cs++;
				if(*cs=='\r')	cs++;
				if(*cs=='\n')	cs++;
				continue;
			}
			*ct++=*cs++;
		}
		if(*cs='\r')	cs++;
		if(*cs='\n')	cs++;
		*ct++=0;
		
		a=bccx_splitb(tbuf);
		for(i=0; i<128; i++)
		{
			if(!a[i])
				break;
			toka[i]=BTM_PgmInternToken(a[i]);
		}
		
		if((ntokb+i+16)>=mtokb)
		{
			k=mtokb+(mtokb>>1);
			tokb=realloc(tokb, k*sizeof(u16));
			mtokb=k;
		}
		if((ntoki+8)>=mtoki)
		{
			k=mtoki+(mtoki>>1);
			toki=realloc(toki, k*sizeof(u16));
			mtoki=k;
		}
		
		toki[ntoki++]=ntokb;
		for(j=0; j<i; j++)
			{ tokb[ntokb++]=toka[j]; }
	}
	toki[ntoki++]=ntokb;
	
	tok_colon=BTM_PgmInternToken("O:");
	for(i=0; i<ntoki; i++)
	{
		if((nlbl+8)>=mlbl)
		{
			k=mlbl+(mlbl>>1);
			lblid=realloc(lblid, k*sizeof(u16));
			lblix=realloc(lblix, k*sizeof(u16));
			mlbl=k;
		}
		
		j=toki[i];
		if(tokb[j+1]==tok_colon)
		{
			lblid[nlbl]=tokb[j+0];
			lblix[nlbl]=i;
			nlbl++;
		}
	}
	
	cmd->ltokb=tokb;
	cmd->ltoki=toki;
	cmd->lbl_id=lblid;
	cmd->lbl_ix=lblix;
	cmd->n_lines=ntoki;
	cmd->n_tokens=ntokb;
	cmd->n_lbls=nlbl;
	
	return(cmd);
}

int BTM_PgmDoGotoTok(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, int lblid)
{
	int i;

	for(i=0; i<pgm->n_lbls; i++)
		if(pgm->lbl_id[i]==lblid)
			break;
	if(i>=pgm->n_lbls)
		return(-1);
	pgm->cur_line=pgm->lbl_ix[i];
	pgm->nxt_line=pgm->cur_line+1;
	return(0);
}

int BTM_PgmTokIsString(u16 tok)
{
	char *s;
	s=bccx_stridx_i(tok);
	return(*s=='S');
}

int BTM_PgmTokIsIdent(u16 tok)
{
	char *s;
	s=bccx_stridx_i(tok);
	return(*s=='I');
}

int BTM_PgmTokIsInt(u16 tok)
{
	char *s;
	s=bccx_stridx_i(tok);
	return(*s=='N');
}

int BTM_PgmTokIsReal(u16 tok)
{
	char *s;
	s=bccx_stridx_i(tok);
	return(*s=='R');
}

u64 BTM_PgmGetGlobal(BTM_ProgCtx *wrl, int tok)
{
	char *s0, *s1;
	u64 val;
	s64 lx;
	double f;
	int i, j, k;
	
	for(i=0; i<wrl->pgm_n_gbls; i++)
	{
		if(wrl->pgm_gbl_vars[i]==tok)
			return(wrl->pgm_gbl_vals[i]);
	}

#if 0
	s0=bccx_stridx_i(tok);
	s1=BTM_CvarGetStr(s0+1);
	if(s1)
	{
		if((*s1>='0') && (*s1<='9'))
		{
			lx=bccx_atoll(s1);
			f=bccx_atof(s1);
			if(f==((s64)f))
			{
				val=BTM_VarWrapInt(lx);
			}else
			{
				val=BTM_VarWrapFloat(f);
			}
		}else
		{
			val=BTM_VarWrapString(s1);
		}
		return(val);
	}
#endif

	return(BRM_PGMVAR_UNDEFINED);
}

u64 BTM_PgmSetGlobal(BTM_ProgCtx *wrl, int tok, u64 val)
{
	char *s0, *s1;
	int i, j, k;
	
	if(!wrl->pgm_gbl_vars)
	{
		k=256;
		wrl->pgm_gbl_vars=btm_malloc(k*sizeof(u16));
		wrl->pgm_gbl_vals=btm_malloc(k*sizeof(u64));
		wrl->pgm_m_gbls=k;
	}
	
	for(i=0; i<wrl->pgm_n_gbls; i++)
	{
		if(wrl->pgm_gbl_vars[i]==tok)
		{
			wrl->pgm_gbl_vals[i]=val;
			return(val);
		}
	}

#if 0
	s0=bccx_stridx_i(tok);
	if(BTM_LookupCvar(s0+1))
	{
//		s1=BTM_CvarGetStr(s0+1);
		if(BTM_VarIsInt(val))
		{
			BTM_CvarSetInt(s0+1, BTM_VarUnwrapInt(val));
		}else if(BTM_VarIsFloat(val))
		{
			BTM_CvarSetFloat(s0+1, BTM_VarUnwrapFloat(val));
		}else if(BTM_VarIsString(val))
		{
			BTM_CvarSetStr(s0+1, BTM_VarUnwrapString(val));
		}
		return(val);
	}
#endif

	if((wrl->pgm_n_gbls+1)>=wrl->pgm_m_gbls)
	{
		k=wrl->pgm_m_gbls;
		k+=k>>1;
		wrl->pgm_gbl_vars=btm_realloc(wrl->pgm_gbl_vars, k*sizeof(u16));
		wrl->pgm_gbl_vals=btm_realloc(wrl->pgm_gbl_vals, k*sizeof(u64));
		wrl->pgm_m_gbls=k;
	}
	
	i=wrl->pgm_n_gbls++;
	wrl->pgm_gbl_vars[i]=tok;
	wrl->pgm_gbl_vals[i]=val;
	return(val);

//	return(BRM_PGMVAR_UNDEFINED);
}

u64 BTM_PgmGetLocal(BTM_ProgCtx *wrl, int tok)
{
	u64 val;
	int i, j, k;
	
	for(i=wrl->pgm_dyn_pos-1; i>=0; i--)
	{
		if(wrl->pgm_dyn_vars[i]==tok)
		{
			val=wrl->pgm_dyn_vals[i];
			return(val);
		}
	}

	return(BTM_PgmGetGlobal(wrl, tok));
}

u64 BTM_PgmSetLocal(BTM_ProgCtx *wrl, int tok, u64 val)
{
	int i, j, k;
	
	for(i=wrl->pgm_dyn_pos-1; i>=0; i--)
	{
		if(wrl->pgm_dyn_vars[i]==tok)
		{
			wrl->pgm_dyn_vals[i]=val;
			return(val);
		}
	}

	return(BTM_PgmSetGlobal(wrl, tok, val));
}

u64 BTM_PgmBindDynamic(BTM_ProgCtx *wrl, int tok, u64 val)
{
	int i;

	i=wrl->pgm_dyn_pos++;
	wrl->pgm_dyn_vars[i]=tok;
	wrl->pgm_dyn_vals[i]=val;
	return(val);
//	return(BTM_PgmSetGlobal(wrl, tok, val));
}

u64 BTM_PgmGetGlobalName(BTM_ProgCtx *wrl, char *name)
{
	char tb[64];
	int tok;
	sprintf(tb, "I%s", name);
	tok=BTM_PgmInternToken(tb);
	return(BTM_PgmGetGlobal(wrl, tok));
}

char *BTM_PgmGetGlobalNameStr(BTM_ProgCtx *wrl, char *name)
{
	char tb[64];
	u64 val;
	s64 li;
	double f;
	int tok;

	sprintf(tb, "I%s", name);
	tok=BTM_PgmInternToken(tb);
	val=BTM_PgmGetGlobal(wrl, tok);

	if(val==BRM_PGMVAR_UNDEFINED)
		return(NULL);

	if(BTM_VarIsString(val))
	{
		return(BTM_VarUnwrapString(val));
	}

	if(BTM_VarIsInt(val))
	{
		li=BTM_VarUnwrapInt(val);
		sprintf(tb, "%lld", li);
		return(bccx_strdup(tb));
	}

	if(BTM_VarIsFloat(val))
	{
		f=BTM_VarUnwrapFloat(val);
		sprintf(tb, "%f", f);
		return(bccx_strdup(tb));
	}

	return(NULL);
}

int BTM_PgmDoEvalCommaList(BTM_ProgCtx *wrl, BTM_ConPgm *pgm,
	u16 **rtcs, u64 *args)
{
	u16 *tcs;
	u64 val;
	int t0, t1;
	int i, n;

	tcs=*rtcs;
	n=0;
	
	while(1)
	{
		t0=tcs[0];
		if((t0==btm_pgmtok_br_rparen) || (t0==btm_pgmtok_br_rbrack))
			break;
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		args[n++]=val;
		if(*tcs==btm_pgmtok_opr_comma)
		{
			tcs++;
			continue;
		}
		break;
	}

	*rtcs=tcs;
	return(n);
}

int BTM_PgmDoEvalArgsList(BTM_ProgCtx *wrl, BTM_ConPgm *pgm,
	u16 **rtcs, u64 *args)
{
	u16 *tcs;
	u64 val;
	int t0, t1;
	int i, n;

	tcs=*rtcs;
	n=BTM_PgmDoEvalCommaList(wrl, pgm, &tcs, args);

	t0=tcs[0];
	if((t0==btm_pgmtok_br_rparen) || (t0==btm_pgmtok_br_rbrack))
	{
		tcs++;
	}
	
	*rtcs=tcs;
	return(n);
}

int BTM_PgmDoEvalNamedArgsList(BTM_ProgCtx *wrl, BTM_ConPgm *pgm,
	u16 **rtcs, u16 *argn, u64 *argv)
{
	u16 *tcs;
	u16 tvar;
	u64 val;
	int t0, t1;
	int i, n;

	tcs=*rtcs;
	n=0;
	
	while(1)
	{
		t0=tcs[0];
		t1=tcs[1];
		if((t0==btm_pgmtok_br_rparen) || (t0==btm_pgmtok_br_rbrack))
			break;

		if(t1!=btm_pgmtok_opr_set)
		{
			val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
			argn[n]=0;
			argv[n++]=val;
			if(*tcs==btm_pgmtok_opr_comma)
			{
				tcs++;
				continue;
			}
			break;
		}
		tvar=tcs[0];
		tcs+=2;
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		argn[n]=tvar;
		argv[n++]=val;
		if(*tcs==btm_pgmtok_opr_comma)
		{
			tcs++;
			continue;
		}
		break;
	}

	*rtcs=tcs;
	return(n);
}

u64 BTM_PgmDoEvalExprLit(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u64 args[16];
	u16 *tcs;
	u16 t0, t1, t2;
	u64 val;
	double fv;
	char *s0;
	int i, j, k;
		
	tcs=*rtcs;

	t0=tcs[0];
	t1=tcs[1];
	val=BRM_PGMVAR_UNDEFINED;
	s0=bccx_stridx_i(t0);
	
	if(t0==btm_pgmtok_br_lparen)
	{
		tcs++;
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		if(*tcs==btm_pgmtok_br_rparen)
			tcs++;
		
		*rtcs=tcs;
		return(val);
	}
	
	if(t0==btm_pgmtok_br_lbrack)
	{
		tcs++;
		k=BTM_PgmDoEvalArgsList(wrl, pgm, &tcs, args);
		val=BTM_VarMakeVectorArgs(args, k);

		*rtcs=tcs;
		return(val);
	}

	if(*s0=='I')
	{
//		if(	(t0==btm_pgmtok_vec2) ||
//			(t0==btm_pgmtok_vec3) ||
//			(t0==btm_pgmtok_vec4))
		if(t0==btm_pgmtok_vec)
		{
			tcs++;
			k=BTM_PgmDoEvalCommaList(wrl, pgm, &tcs, args);
			val=BTM_VarMakeVectorArgs(args, k);

			*rtcs=tcs;
			return(val);
		}
	
		tcs++;
		val=BTM_PgmGetLocal(wrl, t0);
		if(t1==btm_pgmtok_br_lparen)
		{
			tcs++;
			k=BTM_PgmDoEvalArgsList(wrl, pgm, &tcs, args);
			val=BTM_VarGetIndexArgs(val, args, k);
		}
	}else
		if(*s0=='S')
	{
		tcs++;
		val=BTM_VarWrapString(s0+1);
//		val=BRM_PGMVAR_SMALLSTRA+t0;
	}else
		if(*s0=='N')
	{
		tcs++;
		val=BTM_VarWrapInt(bccx_atoll(s0+1));
	}else
		if(*s0=='R')
	{
		tcs++;
		fv=bccx_atof(s0+1);
		val=BTM_VarWrapFloat(fv);
	}
	
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprUnary(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	BTM_PgmExt *ext;
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;

	opr=-1;
	tok=tcs[0];
	if(tok==btm_pgmtok_opr_add)		opr=BRM_PGMOPR_ADD;
	if(tok==btm_pgmtok_opr_sub)		opr=BRM_PGMOPR_SUB;
	if(tok==btm_pgmtok_opr_not)		opr=BRM_PGMOPR_NOT;
	if(tok==btm_pgmtok_opr_bitnot)	opr=BRM_PGMOPR_BITNOT;

	if(opr>=0)
	{
		tcs++;
		val=BTM_PgmDoEvalExprUnary(wrl, pgm, &tcs);
		val=BTM_VarUnaryOp(val, opr);
	}else
	{
		ext=BTM_PgmLookupExt(tok, 1);
		if(ext && ext->Run)
		{
			val=ext->Run(wrl, pgm, &tcs);
		}else
		{
			val=BTM_PgmDoEvalExprLit(wrl, pgm, &tcs);
		}
	}
		
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprMD(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	BTM_PgmExt *ext;
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	val=BTM_PgmDoEvalExprUnary(wrl, pgm, &tcs);
	
	while(1)
	{
		opr=-1;
		tok=tcs[0];
		if(tok==btm_pgmtok_opr_mul)		opr=BRM_PGMOPR_MUL;
		if(tok==btm_pgmtok_opr_div)		opr=BRM_PGMOPR_DIV;
		if(tok==btm_pgmtok_opr_mod)		opr=BRM_PGMOPR_MOD;
		if(opr>=0)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprUnary(wrl, pgm, &tcs);
			val=BTM_VarBinaryOp(val, valb, opr);
			continue;
		}

		ext=BTM_PgmLookupExt(tok, 2);
		if(ext && ext->Infix)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprUnary(wrl, pgm, &tcs);
			val=ext->Infix(wrl, pgm, tok, val, valb);
		}
		break;
	}
	
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprAS(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	val=BTM_PgmDoEvalExprMD(wrl, pgm, &tcs);
	
	while(1)
	{
		opr=-1;
		tok=tcs[0];
		if(tok==btm_pgmtok_opr_add)		opr=BRM_PGMOPR_ADD;
		if(tok==btm_pgmtok_opr_sub)		opr=BRM_PGMOPR_SUB;
		if(opr>=0)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprMD(wrl, pgm, &tcs);
			val=BTM_VarBinaryOp(val, valb, opr);
			continue;
		}
		break;
	}
	
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprSHLR(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	val=BTM_PgmDoEvalExprAS(wrl, pgm, &tcs);
	
	while(1)
	{
		opr=-1;
		tok=tcs[0];
		if(tok==btm_pgmtok_opr_shl)		opr=BRM_PGMOPR_SHL;
		if(tok==btm_pgmtok_opr_shr)		opr=BRM_PGMOPR_SHR;
		if(opr>=0)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprAS(wrl, pgm, &tcs);
			val=BTM_VarBinaryOp(val, valb, opr);
			continue;
		}
		break;
	}
	
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprBitOp(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	val=BTM_PgmDoEvalExprSHLR(wrl, pgm, &tcs);
	
	while(1)
	{
		opr=-1;
		tok=tcs[0];
		if(tok==btm_pgmtok_opr_and)		opr=BRM_PGMOPR_AND;
		if(tok==btm_pgmtok_opr_or)		opr=BRM_PGMOPR_OR;
		if(tok==btm_pgmtok_opr_xor)		opr=BRM_PGMOPR_XOR;
		if(opr>=0)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprSHLR(wrl, pgm, &tcs);
			val=BTM_VarBinaryOp(val, valb, opr);
			continue;
		}
		break;
	}
	
	*rtcs=tcs;
	return(val);
}

u64 BTM_PgmDoEvalExprCompare(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	val=BTM_PgmDoEvalExprBitOp(wrl, pgm, &tcs);
	
	while(1)
	{
		opr=-1;
		tok=tcs[0];
		if(tok==btm_pgmtok_opr_eq)		opr=BRM_PGMOPR_EQ;
		if(tok==btm_pgmtok_opr_ne)		opr=BRM_PGMOPR_NE;
		if(tok==btm_pgmtok_opr_gt)		opr=BRM_PGMOPR_GT;
		if(tok==btm_pgmtok_opr_le)		opr=BRM_PGMOPR_LE;
		if(tok==btm_pgmtok_opr_lt)		opr=BRM_PGMOPR_LT;
		if(tok==btm_pgmtok_opr_ge)		opr=BRM_PGMOPR_GE;
		if(tok==btm_pgmtok_opr_set)		opr=BRM_PGMOPR_EQ;
		if(opr>=0)
		{
			tcs++;
			valb=BTM_PgmDoEvalExprBitOp(wrl, pgm, &tcs);
			val=BTM_VarBinaryOp(val, valb, opr);
			continue;
		}
		break;
	}
	
	*rtcs=tcs;
	return(val);
}

#if 0
u64 BTM_PgmDoEvalExprAssign(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala, valb, val;
	int opr, tok;
	
	tcs=*rtcs;
	vala=BTM_PgmDoEvalExprAS(wrl, pgm, &tcs);
	val=vala;
	
	opr=-1;
	tok=tcs[0];
	if(tok==btm_pgmtok_opr_add)		opr=BRM_PGMOPR_ADD;
	if(tok==btm_pgmtok_opr_sub)		opr=BRM_PGMOPR_SUB;
	
	if(opr>=0)
	{
		tcs++;
		valb=BTM_PgmDoEvalExprAssign(wrl, pgm, &tcs);
		val=BTM_VarBinaryOp(vala, valb, opr);
	}
	
	*rtcs=tcs;
	return(val);
}
#endif


u64 BTM_PgmDoEvalExpr(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 **rtcs)
{
	u16 *tcs;
	u64 vala;
	
	tcs=*rtcs;
	vala=BTM_PgmDoEvalExprCompare(wrl, pgm, &tcs);
	*rtcs=tcs;
	return(vala);
}

#if 0
int BTM_PgmDoConCmd(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 *tok, u16 *toke)
{
	char tbuf[256];
	u16 *tcs;
	char *cs, *ct, *s0;
	s64 li, lj;
	double fi, fj;
	u64 val;
	int i, j, k, n;

	tcs=tok;
	ct=tbuf;
	s0=bccx_stridx_i(tok[0]);
	strcpy(ct, s0+1);
	ct+=strlen(ct);
	tcs++;
	if(tcs<toke)
	{
		*ct++=' ';
		*ct=0;
	}
	
	while(tcs<toke)
	{
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		if(*tcs==btm_pgmtok_opr_comma)
			tcs++;

		if(BTM_VarIsInt(val))
		{
			li=BTM_VarUnwrapInt(val);
			sprintf(ct, "%lld", li);
		}else
			if(BTM_VarIsFloat(val))
		{
			fi=BTM_VarUnwrapFloat(val);
			sprintf(ct, "%f", fi);
		}else
			if(BTM_VarIsString(val))
		{
			s0=BTM_VarUnwrapString(val);
			sprintf(ct, "\"%s\"", s0);
		}

//		s0=bccx_stridx_i(tok[i]);
//		strcpy(ct, s0+1);

		ct+=strlen(ct);
		if(tcs<toke)
		{
			*ct++=' ';
			*ct=0;
		}
	}
	*ct=0;
	
	BTM_ConsoleCommand(tbuf);
	return(0);
}
#endif

#if 1
int BTM_PgmDoConPrint(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 *tok, u16 *toke)
{
	char tbuf[256];
	u16 *tcs;
	char *cs, *ct, *s0;
	s64 li, lj;
	double fi, fj;
	u64 val;
	int wassemi;
	int i, j, k, n;

	wassemi=0;
	tcs=tok; ct=tbuf;
	while(tcs<toke)
	{
		wassemi=0;
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		
		if(*tcs==btm_pgmtok_opr_comma)
			tcs++;
		if(*tcs==btm_pgmtok_opr_semicolon)
		{
			wassemi=1;
			tcs++;
		}

		if(BTM_VarIsInt(val))
		{
			li=BTM_VarUnwrapInt(val);
			sprintf(ct, "%lld", li);
		}else
			if(BTM_VarIsFloat(val))
		{
			fi=BTM_VarUnwrapFloat(val);
			sprintf(ct, "%f", fi);
		}else
			if(BTM_VarIsString(val))
		{
			s0=BTM_VarUnwrapString(val);
			strcpy(ct, s0);
//			sprintf(ct, "%s", s0);
		}

		ct+=strlen(ct);
		if((tcs<toke) && !wassemi)
		{
			*ct++=' ';
			*ct=0;
		}
	}
	
	if(!wassemi)
		*ct++='\n';

	*ct=0;
	
	if(wrl->pgm_print_stdout_ct)
	{
		strcpy(wrl->pgm_print_stdout_ct, tbuf);
		wrl->pgm_print_stdout_ct+=strlen(wrl->pgm_print_stdout_ct);
		return(0);
	}
	
	puts(tbuf);
//	BTM_ConPrintString(tbuf);
	return(0);
}
#endif

int BTM_PgmDoGosubArgs(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 *tok, u16 *toke)
{
	u16 *tcs;
	u16 tvar;
	u64 vala;
	
	if(tok>=toke)
		return(0);

	tcs=tok;
	while((tcs+2)<toke)
	{
		if(tcs[1]!=btm_pgmtok_opr_set)
			break;
		tvar=tcs[0];
		tcs+=2;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		BTM_PgmBindDynamic(wrl, tvar, vala);
		if((tcs<toke) && (tcs[0]==btm_pgmtok_opr_comma))
			{ tcs++; continue; }
		break;
	}

	return(0);
}

int BTM_PgmDoStepTok(BTM_ProgCtx *wrl, BTM_ConPgm *pgm, u16 *tok, u16 *toke)
{
	u64 args[16];
	BTM_PgmExt *ext;
	u16 *tcs;
	char *s0;
	u64 vala, valb, valc;
	int i, j, k;

	if(tok>=toke)
		return(0);

	if(tok[0]==btm_pgmtok_end)
	{
		pgm->nxt_line=-1;
		pgm->gosub_retpos=0;
		wrl->pgm_dyn_pos=0;
		return(0);
	}

	if(tok[0]==btm_pgmtok_stop)
	{
		pgm->nxt_line=-1;
		pgm->gosub_retpos=0;
		wrl->pgm_dyn_pos=0;
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_rem)
	{
		/* does nothing */
		return(0);
	}

	if(tok[0]==btm_pgmtok_goto)
	{
		BTM_PgmDoGotoTok(wrl, pgm, tok[1]);
		return(0);
	}

	if(tok[0]==btm_pgmtok_if)
	{
		tcs=tok+1;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		if(BTM_VarUnwrapInt(vala)!=0)
		{
			if(tcs[0]==btm_pgmtok_then)
			{
				return(BTM_PgmDoStepTok(wrl, pgm, tcs+1, toke));
			}

			if(tcs[0]==btm_pgmtok_goto)
			{
				return(BTM_PgmDoStepTok(wrl, pgm, tcs, toke));
			}
		}
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_print)
	{
		tcs=tok+1;
		BTM_PgmDoConPrint(wrl, pgm, tcs, toke);
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_dim)
	{
		tcs=tok+3;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		j=BTM_VarUnwrapInt(vala);
		vala=BTM_VarAllocArray(j);
		BTM_PgmSetLocal(wrl, tok[1], vala);
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_let)
	{
		tcs=tok+3;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		BTM_PgmSetLocal(wrl, tok[1], vala);
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_temp)
	{
		if(tok[2]==btm_pgmtok_opr_comma)
		{
			vala=BTM_VarWrapInt(0);
			
			tcs=tok+1;
			while(tcs[1]==btm_pgmtok_opr_comma)
			{
				BTM_PgmBindDynamic(wrl, tcs[0], vala);
				tcs+=2;
			}
			BTM_PgmBindDynamic(wrl, tcs[0], vala);
			return(0);
		}

		if(tok[2]==btm_pgmtok_opr_set)
		{
			if(tok[3]==btm_pgmtok_gosub)
			{
				BTM_PgmBindDynamic(wrl, tok[1], BRM_PGMVAR_UNDEFINED);

				if(pgm->gosub_retpos>=16)
				{
					return(0);
				}

				pgm->gosub_retidx[pgm->gosub_retpos]=pgm->nxt_line;
				pgm->gosub_dynidx[pgm->gosub_retpos]=wrl->pgm_dyn_pos;
				pgm->gosub_retdst[pgm->gosub_retpos]=tok[1];
				pgm->gosub_retpos++;
				BTM_PgmDoGosubArgs(wrl, pgm, tok+5, toke);
				BTM_PgmDoGotoTok(wrl, pgm, tok[4]);
				return(0);
			}

			tcs=tok+3;
			vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		}else
		{
			vala=BTM_VarWrapInt(0);
		}
		BTM_PgmBindDynamic(wrl, tok[1], vala);
		return(0);
	}
	
	if(tok[0]==btm_pgmtok_gosub)
	{
		if(pgm->gosub_retpos>=16)
		{
			return(0);
		}

		pgm->gosub_retidx[pgm->gosub_retpos]=pgm->nxt_line;
		pgm->gosub_dynidx[pgm->gosub_retpos]=wrl->pgm_dyn_pos;
		pgm->gosub_retdst[pgm->gosub_retpos]=0;
		pgm->gosub_retpos++;
		BTM_PgmDoGosubArgs(wrl, pgm, tok+2, toke);
		BTM_PgmDoGotoTok(wrl, pgm, tok[1]);
		return(0);
	}

	if(tok[0]==btm_pgmtok_return)
	{
		if(toke>(tok+1))
		{
			tcs=tok+1;
			vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		}else
		{
//			vala=BRM_PGMVAR_UNDEFINED;
			vala=BRM_PGMVAR_NULL;
		}

		if(pgm->gosub_retpos<=0)
		{
			pgm->nxt_line=-1;
			pgm->gosub_retpos=0;
			pgm->retval=vala;
			wrl->pgm_dyn_pos=0;
			return(0);
		}

		i=pgm->gosub_retpos-1;
		j=pgm->gosub_retidx[i];
		wrl->pgm_dyn_pos=pgm->gosub_dynidx[i];
		if(pgm->gosub_retdst[i]>0)
			{ BTM_PgmSetLocal(wrl, pgm->gosub_retdst[i], vala); }
		pgm->gosub_retpos--;
		pgm->nxt_line=j;
		return(0);
	}


	if(tok[0]==btm_pgmtok_for)
	{
		tcs=tok+3;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		BTM_PgmSetLocal(wrl, tok[1], vala);
		valb=vala;
		valc=BTM_VarWrapInt(1);

		if(tcs[0]==btm_pgmtok_to)
		{
			tcs++;
			valb=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		}

		if(tcs[0]==btm_pgmtok_step)
		{
			tcs++;
			valc=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		}

		pgm->loop_retidx[pgm->loop_retpos]=pgm->nxt_line;
		pgm->loop_endval[pgm->loop_retpos]=valb;
		pgm->loop_stpval[pgm->loop_retpos]=valc;
		pgm->loop_retpos++;

		return(0);
	}

	if(tok[0]==btm_pgmtok_next)
	{
		vala=BTM_PgmGetLocal(wrl, tok[1]);
		vala=BTM_VarBinaryOp(vala,
			pgm->loop_stpval[pgm->loop_retpos-1],
			BRM_PGMOPR_ADD);
		j=BTM_VarBinaryOpAsInt(vala,
			pgm->loop_endval[pgm->loop_retpos-1],
			BRM_PGMOPR_LE);
		if(j)
		{
			BTM_PgmSetLocal(wrl, tok[1], vala);
			pgm->nxt_line=pgm->loop_retidx[pgm->loop_retpos-1];
		}else
		{
			pgm->loop_retpos--;
		}
		return(0);
	}

	ext=BTM_PgmLookupExt(tok[0], 0);
	if(ext && ext->Run)
	{
		tcs=tok;
		ext->Run(wrl, pgm, &tcs);
		return(0);
	}
	
	if(tok[1]==btm_pgmtok_opr_set)
	{
		if(tok[2]==btm_pgmtok_gosub)
		{
			if(pgm->gosub_retpos>=16)
			{
				return(0);
			}

			pgm->gosub_retidx[pgm->gosub_retpos]=pgm->nxt_line;
			pgm->gosub_dynidx[pgm->gosub_retpos]=wrl->pgm_dyn_pos;
			pgm->gosub_retdst[pgm->gosub_retpos]=tok[0];
			pgm->gosub_retpos++;
			BTM_PgmDoGosubArgs(wrl, pgm, tok+4, toke);
			BTM_PgmDoGotoTok(wrl, pgm, tok[3]);
			return(0);
		}

		tcs=tok+2;
		vala=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
		BTM_PgmSetLocal(wrl, tok[0], vala);
		return(0);
	}

	s0=bccx_stridx_i(tok[0]);

	if(s0 && (*s0=='I') && (tok[1]==btm_pgmtok_br_lparen))
	{
		tcs=tok+2;
		k=BTM_PgmDoEvalArgsList(wrl, pgm, &tcs, args);

		if(tcs[0]==btm_pgmtok_opr_set)
		{
			tcs++;
			vala=BTM_PgmGetLocal(wrl, tok[0]);
			valb=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
			BTM_VarSetIndexArgs(vala, args, k, valb);
		}
	}
	
//	if(s0 && (*s0=='I') && BTM_LookupCvar(s0+1))
//	{
//		return(BTM_PgmDoConCmd(wrl, pgm, tok, toke));
//	}
	
	return(-1);
}

int BTM_PgmDoStepLine(BTM_ProgCtx *wrl, BTM_ConPgm *pgm)
{
	u16 *tok, *toke;
	
	tok=pgm->ltokb+pgm->ltoki[pgm->cur_line];
	toke=pgm->ltokb+pgm->ltoki[pgm->cur_line+1];
	return(BTM_PgmDoStepTok(wrl, pgm, tok, toke));
}

u64 BTM_PgmRun(BTM_ProgCtx *wrl, char *pgmname, char *lblname)
{
	char tb[256];
	byte *ibuf;
	BTM_ConPgm *pgm;
	int isz, tok_lbl;
	int i, j, k;

	pgm=BTM_PgmLookupProgram(pgmname);
	if(!pgm)
	{
		isz=0;
		sprintf(tb, "scripts/%s.bas", pgmname);
		ibuf=BTM_LoadFile(tb, &isz);
		
		if(!ibuf)
			return(BRM_PGMVAR_UNDEFINED);
		pgm=BTM_PgmLoadParse(pgmname, ibuf, isz);
		if(!pgm)
			return(BRM_PGMVAR_UNDEFINED);
	}

	pgm->retval=BRM_PGMVAR_NULL;

	if(lblname)
	{
		sprintf(tb, "I%s", lblname);
		tok_lbl=BTM_PgmInternToken(tb);
		BTM_PgmDoGotoTok(wrl, pgm, tok_lbl);
	}else
	{
		pgm->cur_line=0;
		pgm->nxt_line=1;
	}

	while((pgm->cur_line>=0) && (pgm->cur_line<pgm->n_lines))
	{
		pgm->nxt_line=pgm->cur_line+1;
		BTM_PgmDoStepLine(wrl, pgm);
		pgm->cur_line=pgm->nxt_line;
	}
	return(pgm->retval);
}

int BTM_PgmEval(BTM_ProgCtx *wrl, char *str)
{
	u16 toka[128];
	BTM_ConPgm *pgm;
	u16 *tcs;
	char **a;
	char *s0;
	u64 val;
	int i;

	BTM_PgmInterpInit();
	pgm=BTM_PgmGetProgram("$eval");

	a=bccx_splitb(str);
	for(i=0; i<128; i++)
	{
		if(!a[i])
			break;
		toka[i]=BTM_PgmInternToken(a[i]);
	}
	
	if(toka[0]==btm_pgmtok_br_lparen)
	{
		tcs=toka+1;
		val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
//		s0=BTM_VarGetAsPrintString(val);
//		BTM_ConPrintf("> %s\n", s0);
		return(0);
	}
	
	BTM_PgmDoStepTok(wrl, pgm, toka, toka+i);
	return(0);
}

u64 BTM_PgmEvalExpr(BTM_ProgCtx *wrl, char *str)
{
	u16 toka[128];
	u16 *tcs;
	BTM_ConPgm *pgm;
	char **a;
	u64 val;
	int i;

	BTM_PgmInterpInit();
	pgm=BTM_PgmGetProgram("$eval");

	a=bccx_splitb(str);
	for(i=0; i<128; i++)
	{
		if(!a[i])
			break;
		toka[i]=BTM_PgmInternToken(a[i]);
	}
	
	tcs=toka;
	val=BTM_PgmDoEvalExpr(wrl, pgm, &tcs);
//	BTM_PgmDoStepTok(wrl, pgm, toka, toka+i);
	return(val);
}
