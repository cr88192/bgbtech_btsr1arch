#include <bgbccc.h>

#if 0
BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForType(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		return(obj);
	}
	return(NULL);
}
#endif

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_CCXL_LookupStructureForSig2(ctx, s));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
//	while(*s && (*s=='P'))
//		{ s++; }
	if((*s!='X') && (*s!='Y'))
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_CCXL_LookupStructure(ctx, tb);
	return(cur);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetStructureForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_CCXL_GetStructureForSig2(ctx, s));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetStructureForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
	if((*s!='X') && (*s!='Y'))
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_CCXL_LookupStructure(ctx, tb);
	if(!cur)
		{ cur=BGBCC_CCXL_GetStruct(ctx, tb); }
	return(cur);
}

int BGBCC_CCXL_LookupStructureIDForSig(
	BGBCC_TransState *ctx, char *sig)
{
	BGBCC_CCXL_LiteralInfo *cur;
	
	cur=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
	if(!cur)return(-1);
	return(cur->litid);
}


BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupTypedefForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_CCXL_LookupTypedefForSig2(ctx, s));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupTypedefForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
//	while(*s && (*s=='P'))
//		{ s++; }
	if(*s!='U')
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_CCXL_LookupTypedef(ctx, tb, NULL);
	return(cur);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetTypedefForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_CCXL_GetTypedefForSig2(ctx, s));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_GetTypedefForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
	if(*s!='U')
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_CCXL_LookupTypedef(ctx, tb, NULL);
	if(!cur)
		{ cur=BGBCC_CCXL_GetTypedef2(ctx, tb); }
	return(cur);
}



BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForType(
	BGBCC_TransState *ctx, ccxl_type type)
{
	BGBCC_CCXL_TypeOverflow ovf;
	BGBCC_CCXL_LiteralInfo *st;
	int bt;

	if((type.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		bt=type.val&CCXL_TY_BASEMASK;
		if(bt<256)
		{
//			BGBCC_CCXL_TagError(ctx,
//				CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
			return(NULL);
		}

		st=ctx->literals[bt-256];
		return(st);
	}

	if((type.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
//		BGBCC_DBGBREAK
		bt=type.val&CCXL_TYB2_BASEMASK;
		if(bt<256)
		{
//			BGBCC_CCXL_TagError(ctx,
//				CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
			return(NULL);
		}

		st=ctx->literals[bt-256];
		return(st);
	}

	if((type.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		bt=type.val&CCXL_TYB3_BASEMASK;
		if(bt<256)
		{
//			BGBCC_CCXL_TagError(ctx,
//				CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
			return(NULL);
		}

		st=ctx->literals[bt-256];
		return(st);
	}

	if((type.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[type.val&CCXL_TYOVF_IDXMASK]);
//		bt=type.val&CCXL_TYB2_BASEMASK;
		bt=ovf.base;
		if(bt<256)
		{
//			BGBCC_CCXL_TagError(ctx,
//				CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
			return(NULL);
		}

		st=ctx->literals[bt-256];
		return(st);
	}
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(NULL);
}

int BGBCC_CCXL_LookupStructureIDForType(
	BGBCC_TransState *ctx, ccxl_type type)
{
	BGBCC_CCXL_LiteralInfo *cur;
	
	cur=BGBCC_CCXL_LookupStructureForType(ctx, type);
	if(!cur)return(-1);
	return(cur->litid);
}

int BGBCC_CCXL_LookupStructContainsFieldID(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_LiteralInfo *st2;
	int i, j;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
		{
			st2=BGBCC_CCXL_LookupStructureForType(ctx,
				st->decl->fields[i]->type);
			if(!st2)
				continue;
			
			j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st2, name);
			if(j>=0)return(i);

			continue;
		}

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_CCXL_LookupStructFieldID(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name)
{
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

ccxl_status BGBCC_CCXL_LookupStructFieldType(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name,
	ccxl_type *rty)
{
	ccxl_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
		{
			BGBCC_CCXL_TypeFromSig(ctx, &bty,
				st->decl->fields[i]->sig);
			*rty=bty;
			return(1);
		}
	}
	
//	printf("BGBCC_CCXL_LookupStructFieldType: No Field '%s'\n", name);
	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
//	*rty=bty;	
	return(0);
}



ccxl_status BGBCC_CCXL_LookupStructSuperFieldType(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *st, char *name,
	int *rsi, int *rfi,
	ccxl_type *rty)
{
	BGBCC_CCXL_LiteralInfo *sobj;
	ccxl_type bty, tty;
	int i0, i1;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
		{
			BGBCC_CCXL_TypeFromSig(ctx, &bty,
				st->decl->fields[i]->sig);
			*rty=bty;
			*rsi=0;
			*rfi=i;
			return(1);
		}
	}
	
	if(st->decl->n_args>0)
	{
		tty=st->decl->args[0]->type;
		sobj=BGBCC_CCXL_LookupStructureForType(ctx, tty);
		i=BGBCC_CCXL_LookupStructSuperFieldType(ctx, sobj, name,
			&i0, &i1, &bty);
		if(i>0)
		{
			*rty=bty;
			*rsi=i0+1;
			*rfi=i1;
			return(1);
		}
	}
	
//	printf("BGBCC_CCXL_LookupStructFieldType: No Field '%s'\n", name);
	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
//	*rty=bty;	
	return(0);
}


ccxl_status BGBCC_CCXL_LookupStructFieldIdType(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, int idx,
	ccxl_type *rty)
{
	ccxl_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	if((idx>=0) && (idx<st->decl->n_fields))
	{
		BGBCC_CCXL_TypeFromSig(ctx, &bty,
			st->decl->fields[idx]->sig);
		*rty=bty;
		return(1);
	}
	
	bty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
	return(-1);
}


int BGBCC_CCXL_GetArraySizeForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;
	s=sig;
	while(*s && (*s=='P'))
		{ s++; }
	return(BGBCC_CCXL_GetArraySizeForSig2(ctx, s));
}

int BGBCC_CCXL_GetArraySizeForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	return(BGBCC_CCXL_GetArraySizeForSig2R(ctx, &sig));
}

int BGBCC_CCXL_GetArraySizeForSig2R(
	BGBCC_TransState *ctx, char **rsig)
{
	char *s, *t;
	int i;

	s=*rsig;
	if(*s=='A')
	{
		s++; i=0;
		while(*s && (*s>='0') && (*s<='9'))
			{ i=(i*10)+((*s++)-'0'); }
		if(*s==',')
			return(-1);		
		if(*s==';')
			s++;
		*rsig=s;
		return(i);
	}
	
	return(-1);
}

int BGBCC_CCXL_GetArraySizeForSig3R(
	BGBCC_TransState *ctx, char **rsig)
{
	char *s, *t;
	int i, j;

	s=*rsig;
	if(*s=='A')
	{
		s++; i=1;
		while(*s)
		{
			j=0;
			while(*s && (*s>='0') && (*s<='9'))
				{ j=(j*10)+((*s++)-'0'); }
			if(*s==',')
			{
				i=i*j;
				s++;
				continue;
			}
			if(*s==';')
			{
				i=i*j;
				s++;
				break;
			}
			i=i*j;
			break;
		}
		
		*rsig=s;
		return(i);
	}
	
	return(-1);
}

ccxl_status BGBCC_CCXL_GetStructSigFixedSize(
	BGBCC_TransState *ctx, char *sig, int *rsz)
{
	BGBCC_CCXL_LiteralInfo *str;

	str=BGBCC_CCXL_LookupStructureForSig2(ctx, sig);
	if(str)
	{
		*rsz=str->decl->fxsize;
		return(0);
	}

	*rsz=0;
	return(-1);
}

ccxl_status BGBCC_CCXL_GetSigFixedSize(
	BGBCC_TransState *ctx, char *sig, int *rsz)
{
	int asz[16];
	char *s;
	int sz, na;
	int i, j, k;

	switch(*sig)
	{
	case 'a':	case 'b':
	case 'c':	case 'h':
		sz=1;	break;
	case 'i':	case 'j':
		sz=4;	break;
	case 'l':	case 'm':
		if(ctx->arch_sizeof_long)
			{ sz=ctx->arch_sizeof_long; break; }
		sz=4;
		break;
	case 's':	case 't':
	case 'w':	case 'k':
		sz=2;	break;
	case 'x':	case 'y':
		sz=8;	break;
	case 'f':
		sz=4;	break;
	case 'd':
		sz=8;	break;
//	case 'P':
//		sz=4;	break;
	case 'A':
		s=sig+1; na=0; i=0;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ i=i*10+((*s++)-'0'); continue; }
			if(*s==',')
				{ asz[na++]=i; i=0; s++; continue; }
			if(*s==';')
				{ asz[na++]=i; i=0; s++; break; }
			asz[na++]=i;
			break;
		}
		BGBCC_CCXL_GetSigFixedSize(ctx, s, &sz);
		j=1;
		for(i=0; i<na; i++)
			j*=asz[i];
		sz=j*sz;
		break;

	case 'P':
		if(ctx->arch_sizeof_ptr)
			{ sz=ctx->arch_sizeof_ptr; break; }
		sz=4;
		break;

	case 'X':
		BGBCC_CCXL_GetStructSigFixedSize(ctx, sig, &sz);
		break;
	default:
		sz=4;	break;
	}
	
	*rsz=sz;
	return(0);
}

char *BGBCC_CCXL_SigGetReturnSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s;
	int i;
	
	s=sig;
	while(*s=='P')s++;
	if(*s=='(')
	{
		i=0;
		while(*s)
		{
			if(*s=='(') { i++; s++; continue; }
			if(*s==')')
			{
				i--; s++;
				if(!i)break;
				continue;
			}
			s++;
		}
		return(s);
	}
	
	return(sig);
}


ccxl_status BGBCC_CCXL_GetStructSigMinMaxSize(
	BGBCC_TransState *ctx, char *sig, int *rsz, int *ral)
{
	BGBCC_CCXL_LiteralInfo *str;

	str=BGBCC_CCXL_LookupStructureForSig2(ctx, sig);
	if(str && str->decl)
	{
		rsz[0]=str->decl->fxmsize;
		rsz[1]=str->decl->fxnsize;
		ral[0]=str->decl->fxmalgn;
		ral[1]=str->decl->fxnalgn;
		return(0);
	}

	rsz[0]=0;	rsz[1]=0;
	ral[0]=1;	ral[1]=1;
	return(-1);
}

ccxl_status BGBCC_CCXL_GetSigMinMaxSize(
	BGBCC_TransState *ctx, char *sig, int *rsz, int *ral)
{
	int asz[16];
	char *s;
	int sza[2], ala[2];
	int sz, na, ret;
	int i, j, k;

	ret=0;
	switch(*sig)
	{
	case 'a':	case 'b':
	case 'c':	case 'h':
		sza[0]=1; sza[1]=1;
		ala[0]=1; ala[1]=1;
		break;
	case 'i':	case 'j':
		sza[0]=4; sza[1]=4;
		ala[0]=4; ala[1]=4;
		break;
	case 'l':	case 'm':
		if(ctx->arch_sizeof_long)
		{
			i=ctx->arch_sizeof_long;
			sza[0]=i; sza[1]=i;
			ala[0]=i; ala[1]=i;
			break;
		}
		sza[0]=4; sza[1]=8;
		ala[0]=4; ala[1]=8;
		break;
	case 's':	case 't':
	case 'w':	case 'k':
		sza[0]=2; sza[1]=2;
		ala[0]=2; ala[1]=2;
		break;
	case 'x':	case 'y':
		sza[0]=8; sza[1]=8;
		ala[0]=8; ala[1]=8;
		break;
	case 'f':
		sza[0]=4; sza[1]=4;
		ala[0]=4; ala[1]=4;
		break;
	case 'd':
		sza[0]=8; sza[1]=8;
		ala[0]=8; ala[1]=8;
		break;
	case 'P':
		if(ctx->arch_sizeof_ptr)
		{
			i=ctx->arch_sizeof_ptr;
			sza[0]=i; sza[1]=i;
			ala[0]=i; ala[1]=i;
			break;
		}
		sza[0]=4; sza[1]=8;
		ala[0]=4; ala[1]=8;
		break;
	case 'A':
		s=sig+1; na=0; i=0;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ i=i*10+((*s++)-'0'); continue; }
			if(*s==',')
				{ asz[na++]=i; i=0; s++; continue; }
			if(*s==';')
				{ asz[na++]=i; i=0; s++; break; }
			asz[na++]=i;
			break;
		}
		BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
		j=1;
		for(i=0; i<na; i++)
			j*=asz[i];
		sza[0]=j*sza[0];
		sza[1]=j*sza[1];
		break;
	case 'X':
		ret=BGBCC_CCXL_GetStructSigMinMaxSize(ctx, sig, sza, ala);
		break;
	default:
		sza[0]=0; sza[1]=0;
		ala[0]=1; ala[1]=1;
		ret=-1;
		break;
	}
	
//	if(sza[0]!=sza[1])
//		{ BGBCC_DBGBREAK }
	
	rsz[0]=sza[0];
	rsz[1]=sza[1];
	ral[0]=ala[0];
	ral[1]=ala[1];
	return(ret);
}

ccxl_status BGBCC_CCXL_MarkTypeAccessed(
	BGBCC_TransState *ctx, ccxl_type type)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i, j, k, n;

	i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, type);
	if(i<0)return(0);
	obj=ctx->literals[i];
	if(!obj || !obj->decl)
		return(0);

	if(obj->decl->regflags&BGBCC_REGFL_ACCESSED)
		return(0);

	obj->decl->regflags|=BGBCC_REGFL_ACCESSED;

	for(i=0; i<obj->decl->n_args; i++)
		{ BGBCC_CCXL_MarkTypeAccessed(ctx, obj->decl->args[i]->type); }
	for(i=0; i<obj->decl->n_locals; i++)
		{ BGBCC_CCXL_MarkTypeAccessed(ctx, obj->decl->locals[i]->type); }

	n=obj->decl->n_fields;
	for(i=0; i<n; i++)
	{
		BGBCC_CCXL_MarkTypeAccessed(ctx,
			obj->decl->fields[i]->type);
	}

	return(1);
}
