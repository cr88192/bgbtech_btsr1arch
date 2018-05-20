#include <bgbccc.h>

typedef struct {
// VFILE *fd;
byte *cs;

int c;					//next char
char *ns_mru[32];		//namespaces MRU
char *tag_mru[64];		//tags MRU
char *attr_mru[64];		//attribute name MRU
char *str_mru[128];		//strings MRU

byte *lzm_win;			//lz+markov window
u16 *lzm_chain;			//chains
u16 *lzm_hash;			//hash table
int lzm_pos;			//window pos
int lzm_ctx;			//context

char *typens;			//namespace prefix for types
}SBXE_Context;


void *sbxe_malloc(int sz)
{
	void *p;
	p=malloc(sz);
	memset(p, 0, sz);
	return(p);
}

void *sbxe_tmalloc(char *ty, int sz)
{
	return(sbxe_malloc(sz));
}

void sbxe_free(void *ptr)
{
	free(ptr);
}

//LZ+Markov

void SBXE_LZMUpdateByte(SBXE_Context *ctx, int v) //AH:ignore
{
	ctx->lzm_chain[ctx->lzm_pos]=ctx->lzm_hash[ctx->lzm_ctx];
	ctx->lzm_hash[ctx->lzm_ctx]=ctx->lzm_pos;

	ctx->lzm_win[ctx->lzm_pos]=v;
	ctx->lzm_pos=(ctx->lzm_pos+1)&16383;
	ctx->lzm_ctx=((ctx->lzm_ctx<<4)^v)&4095;
//	ctx->lzm_ctx=v;
}

void SBXE_LZMUpdateString(SBXE_Context *ctx,
	byte *s, int l) //AH:ignore
{
	while(l--)SBXE_LZMUpdateByte(ctx, *s++);
}

int SBXE_LZMFindMatch(SBXE_Context *ctx, byte *str,
	int *ri, int *rl) //AH:ignore
{
	int i, j, k, bi, bl;
	byte *s, *t;

	bi=0; bl=0;
	i=0; j=ctx->lzm_hash[ctx->lzm_ctx];
	while(i<16)
	{
		k=j;
		s=str;
		t=ctx->lzm_win+k;
		while(*s && (*s==*t)){s++; k=(k+1)&16383; t=ctx->lzm_win+k;}

		k=s-str;
		if(k>bl) { bi=i; bl=k; }

		i++;
		j=ctx->lzm_chain[j];
	}
	if(bl<3)return(0);
	*ri=bi; *rl=bl;
	return(1);
}

void SBXE_LZMEncodeString(SBXE_Context *ctx, char *str) //AH:ignore
{
	int i, j, k;
	byte *s;

	if(!ctx->lzm_win)
	{
		ctx->lzm_win=sbxe_malloc(16384);
		ctx->lzm_chain=sbxe_malloc(16384*sizeof(u16));
		ctx->lzm_hash=sbxe_malloc(4096*sizeof(u16));
		ctx->lzm_pos=0;
	}
	ctx->lzm_ctx=0x0000;	//initial context

	*ctx->cs++=0x11;

	s=str;
	while(*s)
	{
		i=SBXE_LZMFindMatch(ctx, s, &j, &k);
		if(i)
		{
			*ctx->cs++=0xF0+j;
			*ctx->cs++=k;
			SBXE_LZMUpdateString(ctx, s, k);
			s+=k;
		}else
		{
			*ctx->cs++=*s;
			if(*s>=0xF0)*ctx->cs++=0x00;
			SBXE_LZMUpdateByte(ctx, *s);
			s++;
		}
	}
	*ctx->cs++=0;
}

char *SBXE_LZMDecodeString(SBXE_Context *ctx) //AH:ignore
{
	byte buf[256], *s, *t;
	int i, j, k, c;

	if(!ctx->lzm_win)
	{
		ctx->lzm_win=sbxe_malloc(16384);
		ctx->lzm_chain=sbxe_malloc(16384*sizeof(u16));
		ctx->lzm_hash=sbxe_malloc(4096*sizeof(u16));
		ctx->lzm_pos=0;
	}
	ctx->lzm_ctx=0x0000;	//initial context

	t=buf;
	c=(*ctx->cs++);
	while(c)
	{
		if(c>=0xF0)
		{
			i=(*ctx->cs++);
			if(!i)
			{
				*t++=c;
				SBXE_LZMUpdateByte(ctx, c);
				c=(*ctx->cs++);
				continue;
			}

			j=ctx->lzm_hash[ctx->lzm_ctx];
			k=c-0xF0;
			while(k--)j=ctx->lzm_chain[j];

			for(k=0; k<i; k++)t[k]=ctx->lzm_win[(j+k)&16383];
			SBXE_LZMUpdateString(ctx, t, i);
			t+=i;

			c=(*ctx->cs++);
			continue;
		}

		*t++=c;
		SBXE_LZMUpdateByte(ctx, c);
		c=(*ctx->cs++);
	}
	*t++=0;

	return(bgbcc_strdup(buf));
}

//Misc Stuff

void SBXE_EncodeUVLI(SBXE_Context *ctx, int v) //AH:ignore
{
	int i;
	for(i=1; v>>(i*7); i++);
	while((i--)>1)*ctx->cs++=((v>>(i*7))&0x7F)|0x80;
	*ctx->cs++=v&0x7F;
}

int SBXE_DecodeUVLI(SBXE_Context *ctx) //AH:ignore
{
	int i, c;

	i=0;
	c=(*ctx->cs++);
	while(c&0x80)
	{
		i=(i<<7)|(c&0x7F);
		c=(*ctx->cs++);
	}
	i=(i<<7)|(c&0x7F);
	return(i);
}

void SBXE_EncodeVLI(SBXE_Context *ctx, int v) //AH:ignore
{
	SBXE_EncodeUVLI(ctx, (v<0)?(((-v)<<1)|1):(v<<1));
}

int SBXE_DecodeVLI(SBXE_Context *ctx) //AH:ignore
{
	int i;
	i=SBXE_DecodeUVLI(ctx);
	return((i&1)?(-(i>>1)):(i>>1));
}

void SBXE_TextMRU(SBXE_Context *ctx, char *str) //AH:ignore
{
	char *s;
	int i, j;

	for(i=0; i<127; i++)
		if(ctx->str_mru[i] && !strcmp(ctx->str_mru[i], str))
	{
		s=ctx->str_mru[i];
		for(j=i; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
	}
	for(i=0; i<127; i++)ctx->str_mru[i]=ctx->str_mru[i+1];
	ctx->str_mru[126]=bgbcc_strdup(str);
}

void SBXE_EmitString(SBXE_Context *ctx, char *str) //AH:ignore
{
	memcpy(ctx->cs, str, strlen(str)+1);
	ctx->cs+=strlen(str)+1;
}


//Encoder

void SBXE_EncodeText(SBXE_Context *ctx, char *str) //AH:ignore
{
	char *s;
	int i, j;

	for(i=0; i<127; i++)
		if(ctx->str_mru[i] && !strcmp(ctx->str_mru[i], str))
	{
		s=ctx->str_mru[i];
		for(j=i; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
		*ctx->cs++=0x80+i;
		return;
	}

#if 1
	//special case: integer string in probably sane range
	s=str;
	if(*s=='-')s++;
	while(*s && (*s>='0') && (*s<='9'))s++;

	if(!*s && (strlen(str)<=8))
	{
		i=atoi(str);
		*ctx->cs++=0x10;
		SBXE_EncodeVLI(ctx, i);
		SBXE_TextMRU(ctx, str);
		return;
	}
#endif

#if 1
	SBXE_LZMEncodeString(ctx, str);
	SBXE_TextMRU(ctx, str);
	return;
#endif

	for(i=0; i<127; i++)ctx->str_mru[i]=ctx->str_mru[i+1];
	ctx->str_mru[126]=bgbcc_strdup(str);
	*ctx->cs++=0xFF;
	memcpy(ctx->cs, str, strlen(str)+1);
	ctx->cs+=strlen(str)+1;
}

void SBXE_EncodeTag(SBXE_Context *ctx, char *str) //AH:ignore
{
	char *s;
	int i, j, k, l;

	for(i=0; i<63; i++)
		if(ctx->tag_mru[i] && !strcmp(ctx->tag_mru[i], str))
	{
		s=ctx->tag_mru[i];
		for(j=i; j<63; j++)ctx->tag_mru[j]=ctx->tag_mru[j+1];
		ctx->tag_mru[62]=s;
		*ctx->cs++=0x40+i;
		return;
	}
	for(i=0; i<63; i++)ctx->tag_mru[i]=ctx->tag_mru[i+1];
	ctx->tag_mru[62]=bgbcc_strdup(str);
	*ctx->cs++=0x7F;
	memcpy(ctx->cs, str, strlen(str)+1);
	ctx->cs+=strlen(str)+1;
}

void SBXE_EncodeAttr(SBXE_Context *ctx, char *str) //AH:ignore
{
	char *s;
	int i, j;

	for(i=0; i<63; i++)
		if(ctx->attr_mru[i] && !strcmp(ctx->attr_mru[i], str))
	{
		s=ctx->attr_mru[i];
		for(j=i; j<63; j++)ctx->attr_mru[j]=ctx->attr_mru[j+1];
		ctx->attr_mru[62]=s;
		*ctx->cs++=0x40+i;
		return;
	}
	for(i=0; i<63; i++)ctx->attr_mru[i]=ctx->attr_mru[i+1];
	ctx->attr_mru[62]=bgbcc_strdup(str);
	*ctx->cs++=0x7F;
	memcpy(ctx->cs, str, strlen(str)+1);
	ctx->cs+=strlen(str)+1;
}

void SBXE_EncodeNS(SBXE_Context *ctx, char *str) //AH:ignore
{
	char *s;
	int i, j;

	for(i=0; i<31; i++)
		if(ctx->ns_mru[i] && !strcmp(ctx->ns_mru[i], str))
	{
		s=ctx->ns_mru[i];
		for(j=i; j<31; j++)ctx->ns_mru[j]=ctx->ns_mru[j+1];
		ctx->ns_mru[30]=s;
		*ctx->cs++=0x20+i;
		return;
	}
	for(i=0; i<31; i++)ctx->ns_mru[i]=ctx->ns_mru[i+1];
	ctx->ns_mru[30]=bgbcc_strdup(str);
	*ctx->cs++=0x3F;
	memcpy(ctx->cs, str, strlen(str)+1);
	ctx->cs+=strlen(str)+1;
}

void SBXE_EncodeAttrList(SBXE_Context *ctx,
	BCCX_Attr *first) //AH:ignore
{
	BCCX_Attr *acur;

	acur=first;
	while(acur)
	{
		if(!acur->var || !acur->val)
			{ acur=acur->next; continue; }
		if(acur->ns)SBXE_EncodeNS(ctx, acur->ns);
		SBXE_EncodeAttr(ctx, acur->var);
		SBXE_EncodeText(ctx, acur->val);
		acur=acur->next;
	}
	*ctx->cs++=0;
}

void SBXE_EncodeNode(SBXE_Context *ctx, BCCX_Node *expr) //AH:ignore
{
	BCCX_Node *ncur;
	BCCX_Attr *acur;

	char buf[256], *s, *otns;
	int i;

#if 0
	if(expr->type==NETPARSE_NODE_BINARY)
	{
		*ctx->cs++=0x13;
		if(expr->ns)SBXE_EncodeNS(ctx, expr->ns);
		SBXE_EncodeTag(ctx, expr->key);
//		SBXE_EncodeAttrList(ctx, expr->attr);

//		s=NetParse_GetNodeNamespacePrefix(expr, NETPARSE_XMLNS_TYPES);
		s=BCCX_URINS(expr, NETPARSE_XMLNS_TYPES);

#if 1	//skip binary size attribute (remade on load)
		acur=expr->attr;
		while(acur)
		{
			if(s && acur->ns && !strcmp(acur->ns, s))
			{
				if(!strcmp(acur->key, "size"))
				{
					acur=acur->next;
					continue;
				}
				if(!strcmp(acur->key, "type") &&
					!strcmp(acur->value, "binary.base64"))
				{
					acur=acur->next;
					continue;
				}
			}

			if(acur->ns)SBXE_EncodeNS(ctx, acur->ns);
			SBXE_EncodeAttr(ctx, acur->key);
			SBXE_EncodeText(ctx, acur->value);
			acur=acur->next;
		}
		*ctx->cs++=0;
#endif

//		i=NetParse_GetNodeIntAttrNS(expr, "dt", "size");
		i=NetParse_GetNodeIntAttrNS(expr, s, "size");

		SBXE_EncodeUVLI(ctx, i);
		memcpy(ctx->cs, expr->text, i);
		ctx->cs+=i;

		return;
	}
#endif

#if 1
	if(expr->tag && !strcmp(expr->tag, "!BDATA"))
	{
		*ctx->cs++=0x13;
		SBXE_EncodeTag(ctx, "!BDATA");

#if 1	//skip binary size attribute (remade on load)
		acur=expr->attr;
		while(acur)
		{
			if(!strcmp(acur->var, "size"))
			{
				acur=acur->next;
				continue;
			}

			if(!strcmp(acur->val, "type") &&
				!strcmp(acur->val, "binary.base64"))
			{
				acur=acur->next;
				continue;
			}

			if(acur->ns)SBXE_EncodeNS(ctx, acur->ns);
			SBXE_EncodeAttr(ctx, acur->var);
			SBXE_EncodeText(ctx, acur->val);
			acur=acur->next;
		}
		*ctx->cs++=0;
#endif

		i=BCCX_GetInt(expr, "size");
		SBXE_EncodeUVLI(ctx, i);
		memcpy(ctx->cs, expr->text, i);
		ctx->cs+=i;

		return;
	}
#endif

#if 1
//	if(expr->type==NETPARSE_NODE_CDATA_SECTION)
	if(expr->tag && !strcmp(expr->tag, "!CDATA"))
	{
		*ctx->cs++=0x12;
		s=expr->text;
		while(strlen(s)>255)
		{
//			strncpy(s, buf, 256);
			strncpy(buf, s, 256);
			buf[255]=0;
			SBXE_EncodeText(ctx, buf);
			s+=255;
		}
		SBXE_EncodeText(ctx, s);
		*ctx->cs++=0;
		return;
	}
#endif

	if(!expr->tag)
	{
		if(!expr->text)
		{
//			kprint("SBXE_EncodeNode: Bad Node\n");
			return;
		}

		s=expr->text;
		while(strlen(s)>255)
		{
//			strncpy(s, buf, 256);
			strncpy(buf, s, 256);
			buf[255]=0;
			SBXE_EncodeText(ctx, buf);
			s+=255;
		}
		SBXE_EncodeText(ctx, s);

		return;
	}

	if(expr->text)
		BCCX_Set(expr, "!TEXT", expr->text);

	if(expr->ns)SBXE_EncodeNS(ctx, expr->ns);
//	SBXE_EncodeTag(ctx, expr->key);
//	SBXE_EncodeAttrList(ctx, expr->attr);

	s=buf;
	if(!expr->attr && !expr->down){*s++='=';}
		else if(!expr->attr){*s++=':';}
		else if(!expr->down){*s++='/';}
	strcpy(s, expr->tag);
	SBXE_EncodeTag(ctx, buf);

	if(expr->attr)SBXE_EncodeAttrList(ctx, expr->attr);
	if(!expr->down)
		return;

	if(expr->text)
		BCCX_Set(expr, "!TEXT", NULL);

	ncur=expr->down;
	while(ncur)
	{
		SBXE_EncodeNode(ctx, ncur);
		ncur=ncur->next;
	}
	*ctx->cs++=0;
}

void SBXE_EncodeNodeList(SBXE_Context *ctx, BCCX_Node *expr) //AH:ignore
{
	BCCX_Node *ncur;

	ncur=expr;
	while(ncur)
	{
		SBXE_EncodeNode(ctx, ncur);
		ncur=ncur->next;
	}
	*ctx->cs++=0;
}


//Decoder

char *SBXE_DecodeText(SBXE_Context *ctx) //AH:ignore
{
	char buf[256], *s;
	int i, j;

	if(ctx->c==0x10)
	{
		i=SBXE_DecodeVLI(ctx);
		sprintf(buf, "%d", i);
		s=bgbcc_strdup(buf);
		for(j=0; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
		return(s);
	}

	if(ctx->c==0x11)
	{
		s=SBXE_LZMDecodeString(ctx);
		for(j=0; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
		return(s);
	}

	if(ctx->c==0xFF)
	{
		s=buf; j=-1;
		while(j){ j=(*ctx->cs++); *s++=j; }

		s=bgbcc_strdup(buf);
		for(j=0; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
		return(s);
	}
	if(ctx->c>=0x80)
	{
		i=ctx->c-0x80;
		s=ctx->str_mru[i];
		for(j=i; j<127; j++)ctx->str_mru[j]=ctx->str_mru[j+1];
		ctx->str_mru[126]=s;
		return(s);
	}
	return(NULL);
}

char *SBXE_DecodeTag(SBXE_Context *ctx) //AH:ignore
{
	char buf[256], *s;
	int i, j;

	if(ctx->c==0x7F)
	{
		s=buf; j=-1;
		while(j){ j=(*ctx->cs++); *s++=j; }
		s=bgbcc_strdup(buf);
		for(j=0; j<63; j++)ctx->tag_mru[j]=ctx->tag_mru[j+1];
		ctx->tag_mru[62]=s;
		return(s);
	}
	if(ctx->c>=0x40)
	{
		i=ctx->c-0x40; s=ctx->tag_mru[i];
		for(j=i; j<63; j++)ctx->tag_mru[j]=ctx->tag_mru[j+1];
		ctx->tag_mru[62]=s;
		return(s);
	}
	return(NULL);
}

char *SBXE_DecodeAttr(SBXE_Context *ctx) //AH:ignore
{
	char buf[256], *s;
	int i, j;

	if(ctx->c==0x7F)
	{
		s=buf; j=-1;
		while(j){ j=(*ctx->cs++); *s++=j; }
		s=bgbcc_strdup(buf);
		for(j=0; j<63; j++)ctx->attr_mru[j]=ctx->attr_mru[j+1];
		ctx->attr_mru[62]=s;
		return(s);
	}
	if(ctx->c>=0x40)
	{
		i=ctx->c-0x40; s=ctx->attr_mru[i];
		for(j=i; j<63; j++)ctx->attr_mru[j]=ctx->attr_mru[j+1];
		ctx->attr_mru[62]=s;
		return(s);
	}
	return(NULL);
}

char *SBXE_DecodeNS(SBXE_Context *ctx) //AH:ignore
{
	char buf[256], *s;
	int i, j;

	if(ctx->c==0x3F)
	{
		s=buf; j=-1;
		while(j){ j=(*ctx->cs++); *s++=j; }
		s=bgbcc_strdup(buf);
		for(j=0; j<31; j++)ctx->ns_mru[j]=ctx->ns_mru[j+1];
		ctx->ns_mru[30]=s;
		return(s);
	}
	if((ctx->c>=0x20) && (ctx->c<0x3F))
	{
		i=ctx->c-0x20; s=ctx->ns_mru[i];
		for(j=i; j<31; j++)ctx->ns_mru[j]=ctx->ns_mru[j+1];
		ctx->ns_mru[30]=s;
		return(s);
	}
	return(NULL);
}


BCCX_Node *SBXE_DecodeTextNode(SBXE_Context *ctx) //AH:ignore
{
	static char buf[16384];
	BCCX_Node *n;
	char *s, *t;
	int i, cd;

	if(ctx->c==0x12)
	{
		ctx->c=(*ctx->cs++);
		cd=1;
	}else cd=0;

	i=0;
	t=buf;
	while(1)
	{
		if((ctx->c>=0x80) || (ctx->c==0x10) || (ctx->c==0x11))
		{
			s=SBXE_DecodeText(ctx);
			strcpy(t, s);
			t+=strlen(s);

			i++;
			ctx->c=(*ctx->cs++);
			continue;
		}

		*t=0;
		break;
	}

	n=BCCX_NewText(buf);
//	if(cd)n->type=NETPARSE_NODE_CDATA_SECTION;
//		else n->type=NETPARSE_NODE_TEXT;
	if(cd)n->tag=bgbcc_strdup("!CDATA");
	return(n);
}

BCCX_Attr *SBXE_DecodeAttribute(SBXE_Context *ctx) //AH:ignore
{
	static char buf[16384];
	BCCX_Attr *tmp;
	char *s, *t;
	int i, j;

	tmp=BCCX_NewAttr();

	tmp->ns=SBXE_DecodeNS(ctx);
	if(tmp->ns)ctx->c=(*ctx->cs++);

	tmp->var=SBXE_DecodeAttr(ctx);
	ctx->c=(*ctx->cs++);

	i=0;
	t=buf;
	while(1)
	{
		if((ctx->c>=0x80) || (ctx->c==0x10) || (ctx->c==0x11))
		{
			s=SBXE_DecodeText(ctx);
			strcpy(t, s);
			t+=strlen(s);

			i++;
			ctx->c=(*ctx->cs++);
			continue;
		}

		*t=0;
		break;
	}

	if(i==1)tmp->val=s;
		else tmp->val=bgbcc_strdup(buf);
	return(tmp);
}

BCCX_Attr *SBXE_DecodeAttributeList(SBXE_Context *ctx) //AH:ignore
{
	BCCX_Attr *atmp, *alst, *afst;

	afst=NULL; alst=NULL;
	while(ctx->c > 0)
	{
		atmp=SBXE_DecodeAttribute(ctx);
		if(afst)
		{
			alst->next=atmp;
			alst=atmp;
		}else
		{
			afst=atmp;
			alst=atmp;
		}
	}

	return(afst);
}

#if 0
BCCX_Node *SBXE_DecodeDataNode(SBXE_Context *ctx,
	BCCX_Node *up) //AH:ignore
{
	BCCX_Node *tmp;
	byte *buf;
	char *s, *t;
	int i;

	tmp=BCCX_New(NULL);
	tmp->up=up;
	ctx->c=(*ctx->cs++);

	tmp->ns=SBXE_DecodeNS(ctx);
	if(tmp->ns)ctx->c=(*ctx->cs++);
	tmp->key=SBXE_DecodeTag(ctx);

	ctx->c=(*ctx->cs++);
	tmp->attr=SBXE_DecodeAttributeList(ctx);

	i=SBXE_DecodeUVLI(ctx);
	buf=kalloc(i);
	memcpy(buf, i, ctx->cs);
	ctx->cs+=i;

	tmp->text=buf;
//	NetParse_SetNodeIntAttrNS(tmp, "dt", "size", i);
	s=NetParse_GetNodeNamespacePrefix(tmp, NETPARSE_XMLNS_TYPES);

	//hack: binary node but no namespace, come up with one
	if(!s)
	{
		NetParse_SetNodeAttrNS(tmp, "xmlns", "dtx",
			NETPARSE_XMLNS_TYPES);
		s="dtx";
	}

	if(s)
	{
		NetParse_SetNodeIntAttrNS(tmp, s, "size", i);
		t=NetParse_GetNodeAttrNS(tmp, s, "type");
		if(!t)NetParse_SetNodeAttrNS(tmp, s, "type", "binary.base64");
	}

	ctx->c=(*ctx->cs++);
	return(tmp);
}
#endif

#if 1
BCCX_Node *SBXE_DecodeDataNode(SBXE_Context *ctx,
	BCCX_Node *up) //AH:ignore
{
	BCCX_Node *tmp;
	byte *buf;
	char *s, *t;
	int i;

	tmp=BCCX_New(NULL);
	tmp->up=up;
	ctx->c=(*ctx->cs++);

	tmp->ns=SBXE_DecodeNS(ctx);
	if(tmp->ns)ctx->c=(*ctx->cs++);
	tmp->tag=SBXE_DecodeTag(ctx);

	if(tmp->tag)
	{
		if(strcmp(tmp->tag, "!BDATA"))
		{
			BCCX_Set(tmp, "tag", tmp->tag);
			tmp->tag=bgbcc_strdup("!BDATA");
		}

		ctx->c=(*ctx->cs++);
		tmp->attr=SBXE_DecodeAttributeList(ctx);
	}else
	{
		tmp->tag=bgbcc_strdup("!BDATA");
	}

	i=SBXE_DecodeUVLI(ctx);
	buf=sbxe_malloc(i);
	memcpy(buf, ctx->cs, i);
	ctx->cs+=i;

	tmp->text=buf;

	BCCX_SetInt(tmp, "size", i);
	t=BCCX_Get(tmp, "type");
	if(!t)BCCX_Set(tmp, "type", "binary.base64");

	ctx->c=(*ctx->cs++);
	return(tmp);
}
#endif

BCCX_Node *SBXE_DecodeNodeList(SBXE_Context *ctx,
	BCCX_Node *up); //AH:ignore

BCCX_Node *SBXE_DecodeNode(SBXE_Context *ctx,
	BCCX_Node *up) //AH:ignore
{
	char *s, *t;
	BCCX_Node *tmp, *n, *lst;
	int i;

#if 1
	if(ctx->c==0x13)
	{
		tmp=SBXE_DecodeDataNode(ctx, up);
		return(tmp);
	}
#endif

	if((ctx->c>=0x80) || (ctx->c==0x10) ||
		(ctx->c==0x11) || (ctx->c==0x12))
	{
		tmp=SBXE_DecodeTextNode(ctx);
		return(tmp);
	}

	tmp=BCCX_New(NULL);
	tmp->up=up;
	tmp->ns=SBXE_DecodeNS(ctx);
	if(tmp->ns)ctx->c=(*ctx->cs++);

	s=SBXE_DecodeTag(ctx);
	t=s;
	if((*t=='/') || (*t==':') || (*t=='='))
		t=bgbcc_strdup(s+1);	//keep interned
		//t++;
	tmp->tag=t;

	if((*s!=':') && (*s!='='))
	{
		ctx->c=(*ctx->cs++);
		tmp->attr=SBXE_DecodeAttributeList(ctx);
	}

	if((*s!='/') && (*s!='='))
	{
		lst=SBXE_DecodeNodeList(ctx, tmp);
		tmp->down=lst;
	}

	t=BCCX_Get(tmp, "!TEXT");
	if(t)
	{
		BCCX_Set(tmp, "!TEXT", NULL);
		tmp->text=t;
	}

	ctx->c=(*ctx->cs++);
	return(tmp);
}

BCCX_Node *SBXE_DecodeNodeList(SBXE_Context *ctx, BCCX_Node *up) //AH:ignore
{
	BCCX_Node *tmp, *lst;

	lst=NULL;
	ctx->c=(*ctx->cs++);
	while(ctx->c > 0)
	{
		tmp=SBXE_DecodeNode(ctx, up);
		lst=BCCX_AddEnd(lst, tmp);
	}
	return(lst);
}


//general

SBXE_Context *SBXE_NewContext(byte *buf) //AH:ignore
{
	SBXE_Context *tmp;

	tmp=sbxe_tmalloc("_sbxe_context_t", sizeof(SBXE_Context));
	tmp->cs=buf;
	return(tmp);
}

int SBXE_DestroyContext(SBXE_Context *ctx) //AH:ignore
{
	if(ctx->lzm_win)sbxe_free(ctx->lzm_win);
	if(ctx->lzm_chain)sbxe_free(ctx->lzm_chain);
	if(ctx->lzm_hash)sbxe_free(ctx->lzm_hash);
	sbxe_free(ctx);
	return(0);
}


int SBXE_WriteNodeList(byte *buf, BCCX_Node *expr)
{
	SBXE_Context *ctx;
	int sz;

	ctx=SBXE_NewContext(buf);
	SBXE_EmitString(ctx, "SBXE");
	SBXE_EncodeNodeList(ctx, expr);
	sz=ctx->cs-buf;
	SBXE_DestroyContext(ctx);
	return(sz);
}

BCCX_Node *SBXE_ReadNodeList(byte *buf)
{
	SBXE_Context *ctx;
	BCCX_Node *tmp;

	if(!strncmp(buf, "SBXE", 4))
		buf+=strlen(buf)+1;

	ctx=SBXE_NewContext(buf);
	tmp=SBXE_DecodeNodeList(ctx, NULL);
	SBXE_DestroyContext(ctx);
	return(tmp);
}
