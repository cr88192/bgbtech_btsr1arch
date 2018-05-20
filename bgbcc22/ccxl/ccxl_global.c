#include <bgbccc.h>

BGBCC_CCXL_LiteralInfo *bgbcc_ccxl_freelits=NULL;

void BGBCC_CCXL_CheckExpandGlobals(
	BGBCC_TransState *ctx)
{
}

int BGBCC_CCXL_HashName(char *name)
{
	char *s;
	int hi;
	
	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi=((hi*251)>>8)&4095;
	return(hi);
}

int BGBCC_CCXL_HashNameCase(char *name)
{
	char *s;
	int hi, c;
	
	s=name; hi=0;
	while(*s)
	{
		c=(*s++);
		if(((u32)(c-'A'))<('Z'-'A'))
			c+='a'-'A';
		hi=(hi*251)+c;
	}
	hi=((hi*251)>>8)&4095;
	return(hi);
}

int BGBCC_CCXL_CheckNameNamesList(char *name, char *nameslst)
{
	char tb[256];
	char *sn, *t;
	
	if(!name || !nameslst)
		return(0);
	
	sn=nameslst;
	while(*sn)
	{
		if(*sn && (*sn<=' '))
			{ sn++; continue; }
		if(*sn==';')
			{ sn++; continue; }

		t=tb;
		while((*sn>' ') && (*sn!=';'))
			*t++=*sn++;
		*t++=0;
		if(!strcmp(tb, name))
			return(1);
	}
	return(0);
}

int BGBCC_CCXL_CheckFlagstrFlag(char *sig, char *flag)
{
	char *s;
	int hi, l;
	
	if(!sig)
		return(0);
	if(!strcmp(sig, flag))
		return(1);
	
	l=strlen(flag);
	s=sig;
	while(*s)
	{
		if(!strncmp(s, flag, l))
			return(1);
		if((*s>='a') && (*s<='z') && (*s!='u'))
			{ s++; continue; }
		if((*s>='A') && (*s<='Z') && (*s!='U'))
		{
			if((s[1]>='a') && (s[1]<='z') && (s[1]!='u'))
				{ s+=2; continue; }
			break;
		}

		if((*s=='u') || (*s=='U'))
		{
			s++;
			if((*s>='0') && (*s<='9'))
			{
				while((*s>='0') && (*s<='9'))
					s++;
				continue;
			}
			
			while(*s && (*s!=';'))
				s++;
			if(*s==';')s++;
			continue;
		}
		break;
	}
	
	return(0);
	
//	s=name; hi=0;
//	while(*s)hi=(hi*251)+(*s++);
//	hi=((hi*251)>>8)&4095;
//	return(hi);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_TryManifestLoadGlobal(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LiteralInfo *obj;
	BGBCC_CCXL_RegisterInfo *gbl;
	int i;

#if 0
	gbl=NULL;
	for(i=0; i<ctx->n_literals; i++)
	{
		obj=ctx->literals[i];
		if(!obj)
			continue;
		if(obj->littype!=CCXL_LITID_MANIFOBJ)
			continue;
		if(!BGBCC_CCXL_CheckNameNamesList(name, obj->decl->sig))
			continue;
		if(!(obj->decl->regflags&BGBCC_REGFL_LOADED))
		{
			obj->decl->regflags|=BGBCC_REGFL_DEMANDLOAD;
			gbl=BGBCC_CCXL_GetGlobal2I(ctx, name);
			break;
		}
	}
#endif
	
	gbl=NULL;
	i=ctx->manif_literal;
	while(i>0)
	{
		obj=ctx->literals[i];
		if(!obj)
			break;

		if(obj->littype!=CCXL_LITID_MANIFOBJ)
			{ i=obj->hnext_name; continue; }
		if(!BGBCC_CCXL_CheckNameNamesList(name, obj->decl->sig))
			{ i=obj->hnext_name; continue; }
		if(!(obj->decl->regflags&BGBCC_REGFL_LOADED))
		{
			obj->decl->regflags|=BGBCC_REGFL_DEMANDLOAD;
			gbl=BGBCC_CCXL_GetGlobal2I(ctx, name);
			break;
		}
	
		i=obj->hnext_name;
	}
	
	if(gbl)
	{
		if(BGBCC_CCXLR3_CheckCanLoadNow(ctx,
			obj->decl->text, obj->decl->sz_text))
		{
			obj->decl->regflags&=~BGBCC_REGFL_DEMANDLOAD;
			obj->decl->regflags|=BGBCC_REGFL_LOADED;
			BGBCC_CCXLR3_LoadBufferRIL(ctx,
				obj->decl->text, obj->decl->sz_text);
		}
	}
	
	return(gbl);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_LookupGlobal(
	BGBCC_TransState *ctx, char *name)
{
//	BGBCC_CCXL_RegisterInfo *cur, *nxt, *lst2, *sel;
	BGBCC_CCXL_RegisterInfo *cur, *nxt;
	BGBCC_CCXL_RegisterInfo *llst;
	BGBCC_CCXL_RegisterInfo *sel;
	int i;

	i=BGBCC_CCXL_HashName(name);
	cur=ctx->hash_globals[i];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->hashnext;
	}

	/* deal with unsorted globals */
	if(ctx->usort_globals)
	{
		cur=ctx->usort_globals; llst=NULL; sel=NULL;
		while(cur)
		{
			nxt=cur->hashnext;
			if(!cur->name)
			{
				cur->hashnext=llst;
				llst=cur;
				cur=nxt;
				continue;
			}
		
			if(!sel && !strcmp(cur->name, name))
				sel=cur;

			i=BGBCC_CCXL_HashName(cur->name);
			cur->hashnext=ctx->hash_globals[i];
			ctx->hash_globals[i]=cur;
			cur=nxt;
		}

		ctx->usort_globals=llst;
		
		if(sel)
			return(sel);
	}
	
	cur=BGBCC_CCXL_TryManifestLoadGlobal(ctx, name);
	if(cur)
		return(cur);

	return(NULL);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_GetGlobal2I(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *cur;
	int i, h;
	
	BGBCC_CCXL_CheckExpandGlobals(ctx);
	
	cur=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
	cur->name=bgbcc_strdup(name);
	
	i=ctx->n_reg_globals++;
	ctx->reg_globals[i]=cur;
	cur->regid=i;

	h=BGBCC_CCXL_HashName(name);
	cur->hashnext=ctx->hash_globals[h];
	ctx->hash_globals[h]=cur;
	
	return(cur);
}

BGBCC_CCXL_RegisterInfo *BGBCC_CCXL_GetGlobal(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_RegisterInfo *cur;
	int i, h;
	
	cur=BGBCC_CCXL_LookupGlobal(ctx, name);
	if(cur)return(cur);

	cur=BGBCC_CCXL_GetGlobal2I(ctx, name);
	return(cur);
}




void BGBCC_CCXL_AddGlobalDecl(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *decl)
{
	int h;

	if(!ctx->n_reg_globals)
		ctx->n_reg_globals++;
	decl->gblid=ctx->n_reg_globals;
	ctx->reg_globals[ctx->n_reg_globals++]=decl;

	if(decl->name)
	{
		h=BGBCC_CCXL_HashName(decl->name);
		decl->hashnext=ctx->hash_globals[h];
		ctx->hash_globals[h]=decl;
	}else
	{
		decl->hashnext=ctx->usort_globals;
		ctx->usort_globals=decl;
	}
}

void BGBCC_CCXL_AddFrameArg(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *frame,
	BGBCC_CCXL_RegisterInfo *decl)
{
	int i;

	if(!frame->args)
	{
		frame->args=bgbcc_malloc(64*sizeof(BGBCC_CCXL_RegisterInfo *));
		frame->n_args=0;
		frame->m_args=64;
	}

	if(frame->n_args>=frame->m_args)
	{
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_FORMAT_SANITY);
	}

	if(frame->n_eargs>0)
	{
		i=frame->n_vargs++;
		if(decl->name && frame->args[i])
		{
			frame->args[i]->name=
				decl->name;
			frame->args[i]->sig=
				decl->sig;
			frame->args[i]->type=
				decl->type;
		}else
		{
			decl->gblid=frame->n_args;
			frame->args[i]=decl;
		}
		return;
	}

	decl->gblid=frame->n_args;
	frame->args[frame->n_args++]=decl;
}

void BGBCC_CCXL_AddFrameLocal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *frame,
	BGBCC_CCXL_RegisterInfo *decl)
{
	if(!frame->locals)
	{
		frame->locals=bgbcc_malloc(256*sizeof(BGBCC_CCXL_RegisterInfo *));
		frame->n_locals=0;
		frame->m_locals=256;
	}

	if(frame->n_locals>=frame->m_locals)
	{
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_FORMAT_SANITY);
	}

	decl->gblid=frame->n_locals;
	frame->locals[frame->n_locals++]=decl;
}

void BGBCC_CCXL_AddFrameField(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *frame,
	BGBCC_CCXL_RegisterInfo *decl)
{
	if(!frame->fields)
	{
		frame->fields=bgbcc_malloc(256*sizeof(BGBCC_CCXL_RegisterInfo *));
		frame->n_fields=0;
		frame->m_fields=256;
	}

	if(frame->n_fields>=frame->m_fields)
	{
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_FORMAT_SANITY);
	}

	decl->gblid=frame->n_fields;
	frame->fields[frame->n_fields++]=decl;
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_AllocLiteral(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_LiteralInfo *obj;
	
	if(bgbcc_ccxl_freelits)
	{
		obj=bgbcc_ccxl_freelits;
		bgbcc_ccxl_freelits=obj->parent;
		memset(obj, 0, sizeof(BGBCC_CCXL_LiteralInfo));
		return(obj);
	}

	obj=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	return(obj);
}

void BGBCC_CCXL_CheckFreeLiteral(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	if(!obj)return;

	if(obj->litid>0)
		return;
		
	obj->parent=bgbcc_ccxl_freelits;
	bgbcc_ccxl_freelits=obj;
}

void BGBCC_CCXL_AddLiteral(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	int i, h;
	if(!ctx->n_literals)
		ctx->n_literals++;
		
	i=ctx->n_literals++;
	ctx->literals[i]=obj;
	obj->litid=i;

	if(obj->name)
	{
		h=BGBCC_CCXL_HashName(obj->name);
		obj->hnext_name=ctx->hash_literals[h];
		ctx->hash_literals[h]=i;
	}else if(obj->littype==CCXL_LITID_MANIFOBJ)
	{
		obj->hnext_name=ctx->manif_literal;
		ctx->manif_literal=i;
	}else
	{
		obj->hnext_name=ctx->usort_literal;
		ctx->usort_literal=i;

//		obj->hnext_name=-1;
//		BGBCC_DBGBREAK
	}
}

char *BGBCC_CCXL_GetParentLiteralSig(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *obj)
{
	char *sig;

	if(!obj->parent)
		return(NULL);

	sig=NULL;
	switch(obj->parent->littype)
	{
	case CCXL_LITID_ARGS:
		break;
	case CCXL_LITID_LOCALS:
		break;
	case CCXL_LITID_FUNCTION:
		sig=obj->parent->decl->sig;
		break;
	case CCXL_LITID_VAR:
	case CCXL_LITID_GLOBALVAR:
	case CCXL_LITID_STATICVAR:
		sig=obj->parent->decl->sig;
		break;
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_UNION:
		break;
	case CCXL_LITID_LIST:
		sig=obj->parent->decl->sig;
		break;
	case CCXL_LITID_VALUE:
		sig=BGBCC_CCXL_GetParentLiteralSig(ctx, obj->parent);
		break;
	}
	return(sig);
}

char *BGBCC_CCXL_GetParentLiteralSigDeref(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *obj)
{
	char tb[256];
	char *sig, *s, *t;
	
	sig=BGBCC_CCXL_GetParentLiteralSig(ctx, obj);
	if(!sig)
		return(sig);
	
	if(*sig=='P')
		return(sig+1);

	if(*sig=='A')
	{
		s=sig+1;
		while((*s) && (*s>='0') && (*s<='9'))
			s++;
		if(*s==';')
			return(s+1);
		if((*s>='A') && *s<='Z')
			return(s);
		if((*s>='a') && *s<='z')
			return(s);
			
		if(*s==',')
		{
			t=tb; s++;
			*t++='A';
			while(*s)
				*t++=*s++;
			*t++=0;
			
			return(bgbcc_strdup(tb));
		}
	}

	if(*sig=='X')
	{
			s=sig;
			t=tb; s++;
			*t++='Y';
			while(*s)
				*t++=*s++;
			*t++=0;
			return(bgbcc_strdup(tb));
	}

	return(sig);
}

char *BGBCC_CCXL_GetObjQNameR_I(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *obj, char *vt)
{
	char *t;

	if(obj->parent)
	{
		t=BGBCC_CCXL_GetObjQNameR_I(ctx, obj->parent, vt);
		if(obj->decl && obj->decl->name)
		{
			if(t!=vt) { *t++='/'; }
			strcpy(t, obj->decl->name);
			t=t+strlen(t);
		}
		return(t);
	}

	if(obj->decl && obj->decl->name)
	{
		t=vt;
		strcpy(t, obj->decl->name);
		t=t+strlen(t);
		return(t);
	}
	
	return(vt);
}

char *BGBCC_CCXL_GetObjQName(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *obj)
{
	char tb[256];
	
	if(!obj)
		return(NULL);
	
	memset(tb, 0, 256);
	BGBCC_CCXL_GetObjQNameR_I(ctx, obj, tb);
	if(!tb[0])
		return(NULL);
	return(bgbcc_strdup(tb));
}

void BGBCC_CCXL_AddAsmBlob(BGBCC_TransState *ctx, char *text)
{
	BGBCC_CCXL_RegisterInfo *decl;
	int ln;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ASMBLOB);
	BGBCC_CCXLR3_EmitArgTextBlob(ctx, text);

	decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
	decl->regtype=CCXL_LITID_ASMBLOB;
	
	ln=strlen(text)+1;
	decl->text=bgbcc_malloc(ln);
	decl->sz_text=ln;
	memcpy(decl->text, text, ln);

	BGBCC_CCXL_AddGlobalDecl(ctx, decl);
}

void BGBCC_CCXL_Begin(BGBCC_TransState *ctx, int tag)
{
	BGBCC_CCXL_BeginName(ctx, tag, NULL);
}

int BGBCC_CCXL_CheckDefinedContextName(
	BGBCC_TransState *ctx, int tag, char *name)
{
	BGBCC_CCXL_RegisterInfo *decl;

	if((tag==CCXL_CMD_VARDECL) && !ctx->cur_obj)
	{
		decl=BGBCC_CCXL_LookupGlobal(ctx, name);
		if(decl)
			return(1);
	}
	return(0);
}

void BGBCC_CCXL_BeginName(BGBCC_TransState *ctx, int tag, char *name)
{
	BGBCC_CCXL_LiteralInfo *obj;
	BGBCC_CCXL_RegisterInfo *decl;

	if(name)
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_BEGINNAME);
		BGBCC_CCXLR3_EmitArgTag(ctx, tag);
		BGBCC_CCXLR3_EmitArgString(ctx, name);

		if(tag==CCXL_CMD_STRUCT)
		{
			obj=BGBCC_CCXL_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=CCXL_LITID_STRUCT;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
					obj->decl->regtype=CCXL_LITID_STRUCT;
				}
			
				obj->decl->n_fields=0;
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if(tag==CCXL_CMD_UNION)
		{
			obj=BGBCC_CCXL_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=CCXL_LITID_UNION;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
					obj->decl->regtype=CCXL_LITID_UNION;
				}
			
				obj->decl->n_fields=0;
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if(tag==CCXL_CMD_FUNCTION)
		{
			decl=BGBCC_CCXL_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				
				obj->decl=decl;

				obj->littype=CCXL_LITID_FUNCTION;
				obj->decl->regtype=CCXL_LITID_FUNCTION;

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_locals=0;
				obj->decl->n_vargs=0;
				obj->decl->n_goto=0;

				ctx->regstackpos=0;
				ctx->uregstackpos=0;
				ctx->markstackpos=0;
//				ctx->ip=ctx->ips;
//				ctx->n_goto=0;
//				ctx->n_lbl=0;
				return;
			}
		}

		if(tag==CCXL_CMD_PROTOTYPE)
		{
			decl=BGBCC_CCXL_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=BGBCC_CCXL_AllocLiteral(ctx);
//				obj=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;

//				ctx->ip=ctx->ips;
//				ctx->n_goto=0;
//				ctx->n_lbl=0;

				obj->decl=decl;
				obj->littype=CCXL_LITID_PROTOTYPE;
				obj->decl->regtype=CCXL_LITID_FUNCTION;

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_vargs=0;
				return;
			}
		}

		if(tag==CCXL_CMD_S_PROTOTYPE)
		{
			obj=BGBCC_CCXL_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=CCXL_LITID_FUNCTION;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
					obj->decl->regtype=CCXL_LITID_FUNCTION;
				}

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_vargs=0;

//				ctx->ip=ctx->ips;
//				ctx->n_goto=0;
//				ctx->n_lbl=0;

				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if((tag==CCXL_CMD_VARDECL) && !ctx->cur_obj)
		{
			decl=BGBCC_CCXL_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=BGBCC_CCXL_AllocLiteral(ctx);
//				obj=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;

				obj->decl=decl;
				obj->littype=CCXL_LITID_GLOBALVAR;
				obj->decl->regtype=CCXL_LITID_GLOBALVAR;
				return;
			}
		}

		if((tag==CCXL_CMD_TYPEDEF) && !ctx->cur_obj)
		{
			obj=BGBCC_CCXL_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=CCXL_LITID_TYPEDEF;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
					obj->decl->regtype=CCXL_LITID_TYPEDEF;
				}
			
//				obj->decl->n_fields=0;
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}
	}else
	{
		BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_BEGIN);
		BGBCC_CCXLR3_EmitArgTag(ctx, tag);
//		BGBCC_CCXLR3_EmitArgString(ctx, name);
	}

	obj=BGBCC_CCXL_AllocLiteral(ctx);
//	obj=bgbcc_malloc(sizeof(BGBCC_CCXL_LiteralInfo));
	obj->parent=ctx->cur_obj;
	
	ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
	ctx->cur_obj=obj;
	
	switch(tag)
	{
	case CCXL_CMD_VARDECL:
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_VAR;

#if 1
		if(obj->parent)
		{
			obj->littype=CCXL_LITID_VAR;
			obj->decl->regtype=CCXL_LITID_VAR;
		}else
		{
			obj->littype=CCXL_LITID_GLOBALVAR;
			obj->decl->regtype=CCXL_LITID_GLOBALVAR;
		}
#endif

#if 1
		if(obj->parent)
		{
			obj->littype=CCXL_LITID_VAR;
			switch(obj->parent->littype)
			{
			case CCXL_LITID_ARGS:
//				BGBCC_CCXL_AddFrameArg(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				break;
			case CCXL_LITID_LOCALS:
				BGBCC_CCXL_AddFrameLocal(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case CCXL_LITID_FUNCTION:
				obj->parent->decl->defv=obj->decl;
				break;
			case CCXL_LITID_STRUCT:
			case CCXL_LITID_UNION:
				BGBCC_CCXL_AddFrameField(ctx,
					obj->parent->decl,
					obj->decl);
				break;
			}
		}else
		{
			obj->littype=CCXL_LITID_GLOBALVAR;
			obj->decl->regtype=CCXL_LITID_GLOBALVAR;
			if(name && obj->decl && !obj->decl->name)
				{ obj->decl->name=bgbcc_strdup(name); }
			BGBCC_CCXL_AddGlobalDecl(ctx, obj->decl);
		}
#endif
		break;
	case CCXL_CMD_STATICVARDECL:
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_STATICVAR;
		obj->littype=CCXL_LITID_STATICVAR;
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddGlobalDecl(ctx, obj->decl);

		if(obj->parent)
		{
			switch(obj->parent->littype)
			{
			case CCXL_LITID_ARGS:
//				BGBCC_CCXL_AddFrameArg(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				break;
			case CCXL_LITID_LOCALS:
//				BGBCC_CCXL_AddFrameLocal(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				obj->decl->defp=obj->parent->parent->decl;
				break;
			case CCXL_LITID_FUNCTION:
//				obj->parent->decl->defv=obj->decl;
				break;
			case CCXL_LITID_STRUCT:
			case CCXL_LITID_UNION:
//				BGBCC_CCXL_AddFrameField(ctx,
//					obj->parent->decl,
//					obj->decl);
				break;
			}
		}
		break;

	case CCXL_CMD_FUNCTION:
		obj->littype=CCXL_LITID_FUNCTION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_FUNCTION;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddGlobalDecl(ctx, obj->decl);
		BGBCC_CCXL_AddLiteral(ctx, obj);

//		obj->decl->type.val=256+obj->decl->gblid;
//		obj->decl->type.val=256+obj->litid;
		obj->decl->type=BGBCC_CCXL_MakeTypeID(ctx, 256+obj->litid);

		obj->decl->n_goto=0;

		ctx->regstackpos=0;
		ctx->uregstackpos=0;
		ctx->markstackpos=0;
//		ctx->ip=ctx->ips;
//		ctx->n_goto=0;
//		ctx->n_lbl=0;
		break;

	case CCXL_CMD_ARGS:
		obj->littype=CCXL_LITID_ARGS;
//		obj->parent->decl->n_args=0;
		break;
	case CCXL_CMD_LOCALS:
		obj->littype=CCXL_LITID_LOCALS;
//		obj->parent->decl->n_locals=0;
		break;
	case CCXL_CMD_VARVALUE:
		obj->littype=CCXL_LITID_VALUE;
		break;

	case CCXL_CMD_PROTOTYPE:
		obj->littype=CCXL_LITID_PROTOTYPE;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_FUNCTION;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddGlobalDecl(ctx, obj->decl);
		BGBCC_CCXL_AddLiteral(ctx, obj);

//		obj->decl->type.val=256+obj->decl->gblid;
//		obj->decl->type.val=256+obj->litid;
		obj->decl->type=BGBCC_CCXL_MakeTypeID(ctx, 256+obj->litid);

//		ctx->ip=ctx->ips;
//		ctx->n_goto=0;
//		ctx->n_lbl=0;
		break;

	case CCXL_CMD_S_PROTOTYPE:
		obj->littype=CCXL_LITID_FUNCTION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_FUNCTION;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;

	case CCXL_CMD_STRUCT:
		obj->littype=CCXL_LITID_STRUCT;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_STRUCT;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;
	case CCXL_CMD_UNION:
		obj->littype=CCXL_LITID_UNION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_UNION;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;

	case CCXL_CMD_TYPEDEF:
		obj->littype=CCXL_LITID_TYPEDEF;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_TYPEDEF;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;

	case CCXL_CMD_LIST:
		obj->littype=CCXL_LITID_LIST;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_LIST;
		obj->decl->sig=BGBCC_CCXL_GetParentLiteralSigDeref(ctx, obj);
		obj->decl->defp=obj->parent->decl;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;

	case CCXL_CMD_BODY:
		ctx->cur_func=obj->parent->decl;
		ctx->cur_func->n_goto=0;

		ctx->regstackpos=0;
		ctx->markstackpos=0;
//		ctx->ip=ctx->ips;
//		ctx->n_goto=0;
//		ctx->n_lbl=0;
		ctx->n_vop=0;
		ctx->n_vtr=0;
		ctx->s_vop=0;
//		ctx->b_vop=0;
		break;

	case CCXL_CMD_MANIFOBJ:
		obj->littype=CCXL_LITID_MANIFOBJ;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_CCXL_RegisterInfo));
		obj->decl->regtype=CCXL_LITID_MANIFOBJ;
		if(name && !obj->name)
			{ obj->name=bgbcc_strdup(name); }
		if(name && obj->decl && !obj->decl->name)
			{ obj->decl->name=bgbcc_strdup(name); }
		BGBCC_CCXL_AddLiteral(ctx, obj);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}

//	BGBCC_CCXL_StubError(ctx);
}

#if 0
// int BGBCC_CCXL_LookupCurFunctionLabel(BGBCC_TransState *ctx, char *name)
{
	int i, j, k;
	
	for(i=0; i<ctx->n_lbl; i++)
	{
		if(!strcmp(ctx->lbl_name[i], name))
			return(ctx->lbl_dest[i]);
	}
	return(-1);
}
#endif

void BGBCC_CCXL_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	byte *rla, *rld;
	int i, j, k;

#if 0
	for(i=0; i<ctx->n_goto; i++)
	{
		switch(ctx->goto_type[i])
		{
		case CCXL_JMPMODE_A16:
			j=BGBCC_CCXL_LookupCurFunctionLabel(ctx,
				ctx->goto_name[i]);
			rla=ctx->ips+ctx->goto_dest[i];
			rld=ctx->ips+j;
			k=(rla[-2]<<8)|(rla[-1]);
			k+=rld-rla;
			rla[-2]=(k>>8)&255;
			rla[-1]=(k   )&255;
			break;
		case CCXL_JMPMODE_A32:
			j=BGBCC_CCXL_LookupCurFunctionLabel(ctx,
				ctx->goto_name[i]);
			rla=ctx->ips+ctx->goto_dest[i];
			rld=ctx->ips+j;
			k=(rla[-4]<<24)|(rla[-3]<<16)|(rla[-2]<<8)|(rla[-1]);
			k+=rld-rla;
			rla[-4]=(k>>24)&255;
			rla[-3]=(k>>16)&255;
			rla[-2]=(k>> 8)&255;
			rla[-1]=(k    )&255;
			break;
		default:
			break;
		}
	}
#endif

	if(obj->decl->defv)
	{
		obj->decl->name=obj->decl->defv->name;
		obj->decl->sig=obj->decl->defv->sig;
		obj->decl->flagstr=obj->decl->defv->flagstr;
	}

//	if(ctx->back_vt && ctx->back_vt->EndFunction)
//	{
//		ctx->back_vt->EndFunction(ctx, obj);
//	}else

//	if(1)
	if(obj->littype==CCXL_LITID_FUNCTION)
	{
#if 1
		if(ctx->n_vop>0)
		{
			obj->decl->vop=bgbcc_malloc(
				ctx->n_vop*sizeof(BGBCC_CCXL_VirtOp *));
			obj->decl->n_vop=ctx->n_vop;
			obj->decl->m_vop=ctx->m_vop;
			memcpy(obj->decl->vop, ctx->vop,
				ctx->n_vop*sizeof(BGBCC_CCXL_VirtOp *));
		}

		if(ctx->n_vtr>0)
		{
			obj->decl->vtr=bgbcc_malloc(
				ctx->n_vtr*sizeof(BGBCC_CCXL_VirtTr *));
			obj->decl->n_vtr=ctx->n_vtr;
			obj->decl->m_vtr=ctx->m_vtr;
			memcpy(obj->decl->vtr, ctx->vtr,
				ctx->n_vtr*sizeof(BGBCC_CCXL_VirtTr *));
		}
#endif

		ctx->n_vop=0;
		ctx->n_vtr=0;
		ctx->s_vop=0;
	}

#if 0
	if(ctx->ip!=ctx->ips)
	{
		i=ctx->ip-ctx->ips;
		obj->decl->text=bgbcc_malloc(i);
		obj->decl->sz_text=i;
		memcpy(obj->decl->text, ctx->ips, i);
	
//		BGBCC_CCXL_DbgDisAsmFunction(ctx, obj);
	}
#endif
}

void BGBCC_CCXL_SanityObjSize(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj, int flag)
{
//	BGBCC_CCXL_LiteralInfo *obj2;
	ccxl_register reg;
	ccxl_type tty, tty2;
	int msz, nsz, msz2, nsz2, sz;
	int mal, nal, mal2, nal2, al;
	int i, j, k;

	if(!obj)
		return;

	switch(obj->littype)
	{
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_CLASS:
		if(!obj->decl)
			{ BGBCC_DBGBREAK }

		msz=obj->decl->fxmsize;
		nsz=obj->decl->fxnsize;
//		if((msz>0) && (nsz>0))
//			return;

		if(!obj->decl->n_fields)
			break;

		if((msz<=0) || (nsz<=0))
			{ BGBCC_DBGBREAK }

		if(msz!=nsz)
			{ BGBCC_DBGBREAK }

		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;
			
			if((msz2<=0) || (nsz2<=0))
				{ BGBCC_DBGBREAK }

			if((mal2<=0) || (nal2<=0))
				{ BGBCC_DBGBREAK }
			
			if((msz2!=nsz2) || (mal2!=nal2))
				{ BGBCC_DBGBREAK }
			
			msz=(msz+(mal2-1))&(~(mal2-1));
			nsz=(nsz+(nal2-1))&(~(nal2-1));

			if(obj->decl->fields[i]->fxmoffs!=msz)
				{ BGBCC_DBGBREAK }
			if(obj->decl->fields[i]->fxnoffs!=nsz)
				{ BGBCC_DBGBREAK }

			if(msz==nsz)
			{
				if(obj->decl->fields[i]->fxoffs!=msz)
					{ BGBCC_DBGBREAK }
			}else
			{
				BGBCC_DBGBREAK
			}

			msz+=msz2;	nsz+=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
		}
		
		if(i<obj->decl->n_fields)
		{
			BGBCC_DBGBREAK
			break;
		}
		
		al=ctx->arch_align_objmin;
		if((al>0) && (mal==nal) && (mal<al))
			{ mal=al; nal=al; }
		
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		if(obj->decl->fxmsize!=msz)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxnsize!=nsz)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxmalgn!=mal)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxnalgn!=nal)
			{ BGBCC_DBGBREAK }

		msz=0;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxoffs;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			if(msz2<msz)
				{ BGBCC_DBGBREAK }
//			msz=(msz+(mal2-1))&(~(mal2-1));
			msz=msz2+nsz2;
		}

		break;


	case CCXL_LITID_UNION:
		if(!obj->decl)
			{ BGBCC_DBGBREAK }

		msz=obj->decl->fxmsize;
		nsz=obj->decl->fxnsize;

		if(!obj->decl->n_fields)
			break;

		if((msz<=0) || (nsz<=0))
			{ BGBCC_DBGBREAK }

		if(msz!=nsz)
			{ BGBCC_DBGBREAK }

		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;
			
			if((msz2<=0) || (nsz2<=0))
				{ BGBCC_DBGBREAK }

			if((mal2<=0) || (nal2<=0))
				{ BGBCC_DBGBREAK }
			
			if((msz2!=nsz2) || (mal2!=nal2))
				{ BGBCC_DBGBREAK }
			
			msz=(msz+(mal2-1))&(~(mal2-1));
			nsz=(nsz+(nal2-1))&(~(nal2-1));

//			if(obj->decl->fields[i]->fxmoffs!=msz)
//				{ BGBCC_DBGBREAK }
//			if(obj->decl->fields[i]->fxnoffs!=nsz)
//				{ BGBCC_DBGBREAK }

			if(obj->decl->fields[i]->fxmoffs!=0)
				{ BGBCC_DBGBREAK }
			if(obj->decl->fields[i]->fxnoffs!=0)
				{ BGBCC_DBGBREAK }

			if(msz==nsz)
			{
				if(obj->decl->fields[i]->fxoffs!=0)
					{ BGBCC_DBGBREAK }
			}else
			{
				BGBCC_DBGBREAK
			}

//			msz+=msz2;	nsz+=nsz2;
			if(msz2>msz)
				msz=msz2;
			if(nsz2>nsz)
				nsz=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
		}
		
		if(i<obj->decl->n_fields)
		{
			BGBCC_DBGBREAK
			break;
		}
		
		al=ctx->arch_align_objmin;
		if((al>0) && (mal==nal) && (mal<al))
			{ mal=al; nal=al; }
		
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		if(obj->decl->fxmsize!=msz)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxnsize!=nsz)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxmalgn!=mal)
			{ BGBCC_DBGBREAK }
		if(obj->decl->fxnalgn!=nal)
			{ BGBCC_DBGBREAK }

#if 0
		msz=0;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxoffs;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			if(msz2<msz)
				{ BGBCC_DBGBREAK }
//			msz=(msz+(mal2-1))&(~(mal2-1));
			msz=msz2+nsz2;
		}
#endif

		break;
	}
}

void BGBCC_CCXL_FixupObjSize(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj, int flag)
{
	int sza[2], ala[2];
	BGBCC_CCXL_LiteralInfo *obj2;
	ccxl_register reg;
	ccxl_type tty, tty2;
	int msz, nsz, msz2, nsz2, sz;
	int mal, nal, mal2, nal2, al;
	int i, j, k;

	if(!obj)
		return;

	switch(obj->littype)
	{
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_CLASS:
		if(!obj->decl)
			return;

		msz=obj->decl->fxmsize;
		nsz=obj->decl->fxnsize;
		if((msz>0) && (nsz>0))
			return;

		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;
			
			if((msz2<=0) || (nsz2<=0))
			{
				if(flag&1)
					{ k=-1; }

				tty=obj->decl->fields[i]->type;
				if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
				{
					obj2=BGBCC_CCXL_LookupStructureForType(ctx, tty);
					if(!obj2->decl)
					{
						if(flag&1)
							{ BGBCC_DBGBREAK }
						break;
					}

					BGBCC_CCXL_FixupObjSize(ctx, obj2, flag);

					msz2=obj2->decl->fxmsize;	nsz2=obj2->decl->fxnsize;
					mal2=obj2->decl->fxmalgn;	nal2=obj2->decl->fxnalgn;
					
					if((msz2<=0) || (nsz2<=0))
					{
						if(flag&1)
							{ BGBCC_DBGBREAK }
						break;
					}
					
					obj->decl->fields[i]->fxmsize=msz2;
					obj->decl->fields[i]->fxnsize=nsz2;
					obj->decl->fields[i]->fxmalgn=mal2;
					obj->decl->fields[i]->fxnalgn=nal2;
				}else if(BGBCC_CCXL_TypeArrayP(ctx, tty))
				{
					BGBCC_CCXL_TypeDerefType(ctx, tty, &tty2);
					while(BGBCC_CCXL_TypeArrayP(ctx, tty2))
						BGBCC_CCXL_TypeDerefType(ctx, tty2, &tty2);
					if(BGBCC_CCXL_TypeValueObjectP(ctx, tty2))
					{
						obj2=BGBCC_CCXL_LookupStructureForType(ctx, tty2);
						BGBCC_CCXL_FixupObjSize(ctx, obj2, flag);
					}
					
					sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
					al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
					if(sz<=0)
					{
						if(flag&1)
							{ BGBCC_DBGBREAK }
						break;
					}
					if(al<=0)al=1;
					msz2=sz; nsz2=sz;
					mal2=al; nal2=al;

					obj->decl->fields[i]->fxmsize=msz2;
					obj->decl->fields[i]->fxnsize=nsz2;
					obj->decl->fields[i]->fxmalgn=mal2;
					obj->decl->fields[i]->fxnalgn=nal2;
				}else
				{
					BGBCC_DBGBREAK
				}
			}

			if((msz2<=0) || (nsz2<=0))
			{
				if(flag&1)
					{ BGBCC_DBGBREAK }
			}
			
			msz=(msz+(mal2-1))&(~(mal2-1));
			nsz=(nsz+(nal2-1))&(~(nal2-1));

			obj->decl->fields[i]->fxmoffs=msz;
			obj->decl->fields[i]->fxnoffs=nsz;
			if(msz==nsz)
			{
				obj->decl->fields[i]->fxoffs=msz;
			}else
			{
				BGBCC_DBGBREAK
			}

			msz+=msz2;	nsz+=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
		}
		
		if(i<obj->decl->n_fields)
		{
			if(flag&1)
				{ BGBCC_DBGBREAK }
			break;
		}
		
		al=ctx->arch_align_objmin;
		if((al>0) && (mal==nal) && (mal<al))
			{ mal=al; nal=al; }
		
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		obj->decl->fxmsize=msz;	obj->decl->fxnsize=nsz;
		obj->decl->fxmalgn=mal;	obj->decl->fxnalgn=nal;
		break;

	case CCXL_LITID_UNION:
		if(!obj->decl)
			return;

		msz=obj->decl->fxmsize;
		nsz=obj->decl->fxnsize;
		if((msz>0) && (nsz>0))
			return;

		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;

			if((msz2<=0) || (nsz2<=0))
			{
				tty=obj->decl->fields[i]->type;
				if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
				{
					obj2=BGBCC_CCXL_LookupStructureForType(ctx, tty);
					if(!obj2->decl)
						break;

					BGBCC_CCXL_FixupObjSize(ctx, obj2, flag);

					msz2=obj2->decl->fxmsize;	nsz2=obj2->decl->fxnsize;
					mal2=obj2->decl->fxmalgn;	nal2=obj2->decl->fxnalgn;

					if((msz2<=0) || (nsz2<=0))
						break;

					obj->decl->fields[i]->fxmsize=msz2;
					obj->decl->fields[i]->fxnsize=nsz2;
					obj->decl->fields[i]->fxmalgn=mal2;
					obj->decl->fields[i]->fxnalgn=nal2;
				}else if(BGBCC_CCXL_TypeArrayP(ctx, tty))
				{
					BGBCC_CCXL_TypeDerefType(ctx, tty, &tty2);
					while(BGBCC_CCXL_TypeArrayP(ctx, tty2))
						BGBCC_CCXL_TypeDerefType(ctx, tty2, &tty2);
					if(BGBCC_CCXL_TypeValueObjectP(ctx, tty2))
					{
						obj2=BGBCC_CCXL_LookupStructureForType(ctx, tty);
						BGBCC_CCXL_FixupObjSize(ctx, obj2, flag);
					}
					
					sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
					al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
					if(sz<=0)
						break;
					if(al<=0)al=1;
					msz=sz; nsz=sz;
					mal=al; nal=al;
				}else
				{
					BGBCC_DBGBREAK
				}
			}

			if(msz2>msz)msz=msz2;
			if(nsz2>nsz)nsz=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
			obj->decl->fields[i]->fxmoffs=0;
			obj->decl->fields[i]->fxnoffs=0;
			obj->decl->fields[i]->fxoffs=0;
		}

		if(i<obj->decl->n_fields)
		{
			if(flag&1)
				{ BGBCC_DBGBREAK }
			break;
		}

		al=ctx->arch_align_objmin;
		if((al>0) && (mal==nal) && (mal<al))
			{ mal=al; nal=al; }
		
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		obj->decl->fxmsize=msz;	obj->decl->fxnsize=nsz;
		obj->decl->fxmalgn=mal;	obj->decl->fxnalgn=nal;
		break;

	default:
		break;
	}
}

void BGBCC_CCXL_End(BGBCC_TransState *ctx)
{
	int sza[2], ala[2];
	BGBCC_CCXL_LiteralInfo *obj, *obj2;
	ccxl_register reg;
	int msz, nsz, msz2, nsz2;
	int mal, nal, mal2, nal2;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_END);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_FUNCTION:
		BGBCC_CCXL_EmitCallRetDefault(ctx);
		BGBCC_CCXL_EndFunction(ctx, obj);
		break;
	case CCXL_LITID_PROTOTYPE:
//		BGBCC_CCXL_EmitCallRetDefault(ctx);
		BGBCC_CCXL_EndFunction(ctx, obj);
		break;
	case CCXL_LITID_VALUE:
		break;
	case CCXL_LITID_GLOBALVAR:
	case CCXL_LITID_STATICVAR:
		if(!obj->decl)
			break;

		obj->decl->pbname=BGBCC_CCXL_GetObjQName(ctx, obj->parent);
		obj->decl->qname=BGBCC_CCXL_GetObjQName(ctx, obj);

		if(obj->decl->sig)
		{
			BGBCC_CCXL_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);

			BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		if(obj->decl->value.val)
		{
//			i=BGBCC_CCXL_GetRegImmLitIndex(ctx,
//				obj->decl->value, obj->decl->type);
//			obj->decl->validx=i;
		}else
		{
			obj->decl->validx=0;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->decl->type) &&
			(obj->decl->validx>0))
		{
			i=BGBCC_CCXL_TypeGetArraySize(ctx, obj->decl->type);
			obj2=ctx->literals[obj->decl->validx>>2];
			if((i<=0) && (obj2->decl->n_listdata>0))
			{
				BGBCC_CCXL_TypeArrayAdjustSize(ctx, 
					obj->decl->type, &(obj->decl->type),
					obj2->decl->n_listdata);
				obj->decl->sig=
					BGBCC_CCXL_TypeGetSig(ctx, obj->decl->type);

				BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
				obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
				obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
			}
		}

		if(BGBCC_CCXL_TypeIsObjectP(ctx, obj->decl->type))
		{
			obj->decl->sig=	BGBCC_CCXL_TypeGetSig(ctx, obj->decl->type);
			BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		break;
	case CCXL_LITID_VAR:
		if(obj->decl && obj->decl->sig)
		{
			BGBCC_CCXL_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);

			BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		if(obj->decl->value.val)
		{
//			i=BGBCC_CCXL_GetRegImmLitIndex(ctx,
//				obj->decl->value, obj->decl->type);
//			obj->decl->validx=i;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->decl->type) &&
			(obj->decl->validx>0))
		{
			i=BGBCC_CCXL_TypeGetArraySize(ctx, obj->decl->type);
			obj2=ctx->literals[obj->decl->validx>>2];
			if((i<=0) && (obj2->decl->n_listdata>0))
			{
				BGBCC_CCXL_TypeArrayAdjustSize(ctx, 
					obj->decl->type, &(obj->decl->type),
					obj2->decl->n_listdata);
				obj->decl->sig=
					BGBCC_CCXL_TypeGetSig(ctx, obj->decl->type);

				BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
				obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
				obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
			}
		}

		if(BGBCC_CCXL_TypeIsObjectP(ctx, obj->decl->type))
		{
			obj->decl->sig=	BGBCC_CCXL_TypeGetSig(ctx, obj->decl->type);

			BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}

#if 1
		if(obj->parent && (obj->parent->littype==CCXL_LITID_ARGS))
		{
			BGBCC_CCXL_AddFrameArg(ctx,
				obj->parent->parent->decl,
				obj->decl);
		}
#endif

#if 0
		if(obj->parent)
		{
			obj->littype=CCXL_LITID_VAR;
			switch(obj->parent->littype)
			{
			case CCXL_LITID_ARGS:
				BGBCC_CCXL_AddFrameArg(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case CCXL_LITID_LOCALS:
				BGBCC_CCXL_AddFrameLocal(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case CCXL_LITID_FUNCTION:
				obj->parent->decl->defv=obj->decl;
				break;
			case CCXL_LITID_STRUCT:
			case CCXL_LITID_UNION:
				BGBCC_CCXL_AddFrameField(ctx,
					obj->parent->decl,
					obj->decl);
				break;
			}
		}else
		{
			obj->littype=CCXL_LITID_GLOBALVAR;
			obj->decl->regtype=CCXL_LITID_GLOBALVAR;
			BGBCC_CCXL_AddGlobalDecl(ctx, obj->decl);
		}
#endif
		break;

	case CCXL_LITID_TYPEDEF:
		if(obj->decl && obj->decl->sig)
		{
			BGBCC_CCXL_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);

			BGBCC_CCXL_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		break;

	case CCXL_LITID_STRUCT:
	case CCXL_LITID_CLASS:
	case CCXL_LITID_UNION:
		BGBCC_CCXL_FixupObjSize(ctx, obj, 0);
		break;

	case CCXL_LITID_LIST:
		if(obj->decl && obj->decl->sig)
		{
			BGBCC_CCXL_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);
		}

		switch(obj->parent->littype)
		{
		case CCXL_LITID_LIST:
			BGBCC_CCXL_GetRegForLiteralValue(ctx, &reg, obj->litid);
			BGBCC_CCXL_ListAddLiteral(ctx, obj->parent, reg);
			break;
		case CCXL_LITID_VALUE:
			BGBCC_CCXL_GetRegForLiteralValue(ctx, &reg, obj->litid);
			obj->parent->parent->decl->value=reg;
			break;
		default:
			break;
		}
		break;
	case CCXL_LITID_ARGS:
		obj->parent->decl->n_eargs=
			obj->parent->decl->n_args;
		break;
	}

	BGBCC_CCXL_CheckFreeLiteral(ctx, obj);

	ctx->cur_objstackpos--;
	if(ctx->cur_objstackpos>0)
	{
		ctx->cur_obj=ctx->cur_objstack[ctx->cur_objstackpos-1];
	}else
	{
		ctx->cur_obj=NULL;
	}

//	BGBCC_CCXL_StubError(ctx);
}

void BGBCC_CCXL_AttribStr(BGBCC_TransState *ctx, int attr, char *str)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ATTRSTR);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgString(ctx, str);

	obj=ctx->cur_obj;
	if(!obj)
	{
		switch(attr)
		{
		default:
			break;
		}
		return;
	}
	
	switch(obj->littype)
	{
	case CCXL_LITID_VAR:
	case CCXL_LITID_FUNCTION:
	case CCXL_LITID_PROTOTYPE:
	case CCXL_LITID_GLOBALVAR:
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_UNION:
	case CCXL_LITID_CLASS:
	case CCXL_LITID_STATICVAR:
	case CCXL_LITID_TYPEDEF:
		switch(attr)
		{
		case CCXL_ATTR_NAME:
			obj->decl->name=bgbcc_strdup(str);
			if(!obj->name)obj->name=obj->decl->name;
			break;
		case CCXL_ATTR_SIG:
			obj->decl->sig=bgbcc_strdup(str);
			if(!obj->sig)obj->sig=obj->decl->sig;
			break;
		case CCXL_ATTR_FLAGS:
			if(str && (*str))
				{ obj->decl->flagstr=bgbcc_strdup(str); }
			else
				{ obj->decl->flagstr=NULL; }
			break;
		default:
			break;
		}
		break;

	case CCXL_LITID_MANIFOBJ:
		switch(attr)
		{
		case CCXL_ATTR_NAME:
			obj->decl->name=bgbcc_strdup(str);
			if(!obj->name)obj->name=obj->decl->name;
			break;
		case CCXL_ATTR_SIG:
			obj->decl->sig=bgbcc_strdup(str);
			if(!obj->sig)obj->sig=obj->decl->sig;
			break;
		case CCXL_CMD_ARGS:
		case CCXL_CMD_IMPLEMENTS:
			obj->decl->sig=bgbcc_strdup(str);
			if(!obj->sig)obj->sig=obj->decl->sig;
			break;
		}
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_AttribInt(BGBCC_TransState *ctx, int attr, int val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ATTRINT);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	obj=ctx->cur_obj;
	if(!obj)
	{
		switch(attr)
		{
		case CCXL_ATTR_TUIDX:
			ctx->tuidx=val;
			break;
		default:
			break;
		}
		return;
	}

	switch(obj->littype)
	{
	case CCXL_LITID_VAR:
	case CCXL_LITID_FUNCTION:
	case CCXL_LITID_PROTOTYPE:
	case CCXL_LITID_GLOBALVAR:
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_UNION:
	case CCXL_LITID_CLASS:
	case CCXL_LITID_STATICVAR:
	case CCXL_LITID_TYPEDEF:
		switch(attr)
		{
		case CCXL_ATTR_FLAGS:
			obj->decl->flagsint=val;
			break;
		case CCXL_ATTR_SRCTOK:
			obj->decl->srctok=val;
			break;
		default:
			break;
		}
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_AttribLong(BGBCC_TransState *ctx, int attr, s64 val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_ATTRLONG);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	obj=ctx->cur_obj;
	if(!obj)
	{
		switch(attr)
		{
		case CCXL_ATTR_TUIDX:
			ctx->tuidx=val;
			break;
		default:
			break;
		}
		return;
	}

	switch(obj->littype)
	{
	case CCXL_LITID_VAR:
	case CCXL_LITID_FUNCTION:
	case CCXL_LITID_PROTOTYPE:
	case CCXL_LITID_GLOBALVAR:
	case CCXL_LITID_STRUCT:
	case CCXL_LITID_UNION:
	case CCXL_LITID_CLASS:
	case CCXL_LITID_STATICVAR:
	case CCXL_LITID_TYPEDEF:
		switch(attr)
		{
		case CCXL_ATTR_FLAGS:
			obj->decl->flagsint=val;
			break;
		default:
			break;
		}
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}


void BGBCC_CCXL_Marker(BGBCC_TransState *ctx, int tag)
{
	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_MARKER);
	BGBCC_CCXLR3_EmitArgTag(ctx, tag);

	switch(tag)
	{
	case CCXL_CMD_VARARGS:
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
	}
}

void BGBCC_CCXL_ListAddLiteral(BGBCC_TransState *ctx, 
	BGBCC_CCXL_LiteralInfo *list, ccxl_register val)
{
	BGBCC_CCXL_RegisterInfo *def;
	BGBCC_CCXL_LiteralInfo *str;
	int i;

	def=list->decl;
	if(!def->listdata)
	{
		def->listdata=bgbcc_malloc(64*sizeof(ccxl_register));
		def->n_listdata=0;
		def->m_listdata=64;
	}

	if((def->n_listdata+1)>=def->m_listdata)
	{
		i=def->m_listdata;
		i=i+(i>>1);
		def->listdata=bgbcc_realloc(def->listdata,
			i*sizeof(ccxl_register));
		def->m_listdata=i;
	}

	i=def->n_listdata++;
	def->listdata[i]=val;

	if(def->sig && (def->sig[0]=='Y'))
	{
		str=BGBCC_CCXL_LookupStructureForSig2(ctx, def->sig);
		if(str && str->decl)
		{
			if(str->decl->fields[i])
				str->decl->fields[i]->regflags|=
					BGBCC_REGFL_INITIALIZED;
			str->decl->regflags|=BGBCC_REGFL_INITIALIZED;
		}
	}

//	BGBCC_CCXL_StubError(ctx);
}

void BGBCC_CCXL_LiteralInt(BGBCC_TransState *ctx, int attr, s32 val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITINT);
	BGBCC_CCXLR3_EmitArgInt(ctx, attr);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForIntValue(ctx, &reg, val);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForIntValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralLong(BGBCC_TransState *ctx, int attr, s64 val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITLONG);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgInt(ctx, val);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForLongValue(ctx, &reg, val);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForLongValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralFloat(BGBCC_TransState *ctx, int attr, double val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITFLOAT);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgFloat(ctx, val);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForFloatValue(ctx, &reg, val);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForFloatValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralDouble(BGBCC_TransState *ctx, int attr, double val)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITDOUBLE);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgFloat(ctx, val);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &reg, val);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralStr(BGBCC_TransState *ctx, int attr, char *str)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITSTR);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgString(ctx, str);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForStringValue(ctx, &reg, str);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForStringValue(ctx, &reg, str);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralWStr(BGBCC_TransState *ctx, int attr, char *str)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITWSTR);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgString(ctx, str);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForWStringValue(ctx, &reg, str);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForWStringValue(ctx, &reg, str);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

void BGBCC_CCXL_LiteralGlobalAddr(BGBCC_TransState *ctx,
	int attr, s32 gblid)
{
	BGBCC_CCXL_RegisterInfo *decl;
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	decl=ctx->reg_globals[gblid];
	if(decl)
	{
		if(decl->name)
		{
			BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITNAME);
			BGBCC_CCXLR3_EmitArgTag(ctx, attr);
			BGBCC_CCXLR3_EmitArgString(ctx, decl->name);
		}else
		{
			__debugbreak();
		}
	}

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_LIST:
		BGBCC_CCXL_GetRegForGlobalAddrValue(ctx, &reg, gblid);
		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case CCXL_LITID_VALUE:
		BGBCC_CCXL_GetRegForGlobalAddrValue(ctx, &reg, gblid);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}


void BGBCC_CCXL_LiteralData(BGBCC_TransState *ctx, int attr,
	byte *buf, int sz)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_register reg;
	int i, j, k;

	BGBCC_CCXLR3_EmitOp(ctx, BGBCC_RIL3OP_LITDATA);
	BGBCC_CCXLR3_EmitArgTag(ctx, attr);
	BGBCC_CCXLR3_EmitArgDataBlob(ctx, buf, sz);

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case CCXL_LITID_MANIFOBJ:
		if(attr==CCXL_CMD_BODY)
		{
			obj->decl->text=bgbcc_malloc(sz+1);
			obj->decl->sz_text=sz;
			memcpy(obj->decl->text, buf, sz);
			break;
		}
		BGBCC_CCXL_StubError(ctx);
		break;

//	case CCXL_LITID_LIST:
//		BGBCC_CCXL_GetRegForStringValue(ctx, &reg, str);
//		BGBCC_CCXL_ListAddLiteral(ctx, ctx->cur_obj, reg);
//		break;
//	case CCXL_LITID_VALUE:
//		BGBCC_CCXL_GetRegForStringValue(ctx, &reg, str);
//		obj->parent->decl->value=reg;
//		break;
	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
}

int BGBCC_CCXL_HandleMissingProto(BGBCC_TransState *ctx, char *name)
{
	int i;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_PROTOTYPE, name);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, name);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, "(z)i");
//	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);
	
	i=BGBCC_CCXL_LookupGlobalIndex(ctx, name);
	return(i);
}
