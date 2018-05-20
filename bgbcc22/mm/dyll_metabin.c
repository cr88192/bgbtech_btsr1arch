#include <bgbccc.h>

extern BGBDYC_MetaNode *dyll_metapath_root;

void DYLL_MetaBin_EmitByte(BGBDYC_MetaBinary *ctx, int val)
{
	int i, j;
	if(!ctx->buf)
	{
		i=65536;
		ctx->buf=malloc(i);
		ctx->ip=ctx->buf;
		ctx->ipe=ctx->buf+i;
	}
	
	if((ctx->ip+1)>=ctx->ipe)
	{
		i=ctx->ipe-ctx->buf;
		j=ctx->ip-ctx->buf;
		i=i+(i>>1);
		ctx->buf=realloc(ctx->buf, i);
		ctx->ip=ctx->buf+j;
		ctx->ipe=ctx->buf+i;
	}

	*ctx->ip++=val;
}

void DYLL_MetaBin_EmitVLI(BGBDYC_MetaBinary *ctx, int val)
{
	if(val<128)
	{
		DYLL_MetaBin_EmitByte(ctx, val);
	}else if(val<16384)
	{
		DYLL_MetaBin_EmitByte(ctx, 0x80|(val>>8));
		DYLL_MetaBin_EmitByte(ctx, val&0xFF);
	}else if(val<2097152)
	{
		DYLL_MetaBin_EmitByte(ctx, 0xC0|(val>>16));
		DYLL_MetaBin_EmitByte(ctx, (val>>8)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, val&0xFF);
	}else if(val<268435456)
	{
		DYLL_MetaBin_EmitByte(ctx, 0xE0|(val>>24));
		DYLL_MetaBin_EmitByte(ctx, (val>>16)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, (val>>8)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, val&0xFF);
	}else
	{
		DYLL_MetaBin_EmitByte(ctx, 0xF0);
		DYLL_MetaBin_EmitByte(ctx, (val>>24)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, (val>>16)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, (val>>8)&0xFF);
		DYLL_MetaBin_EmitByte(ctx, val&0xFF);
	}
}

void DYLL_MetaBin_EmitSVLI(BGBDYC_MetaBinary *ctx, int val)
{
	int i;
	
	i=(val>0)?(val<<1):(((-val)<<1)-1);
	DYLL_MetaBin_EmitVLI(ctx, i);
}

void DYLL_MetaBin_EncodeString(BGBDYC_MetaBinary *ctx, char *str)
{
	char *s;
	int i, j;

	if(!str)
	{
		DYLL_MetaBin_EmitSVLI(ctx, -1);
		DYLL_MetaBin_EmitByte(ctx, 0);
		return;
	}

	//look for string in dictionary
	for(i=0; i<65536; i++)
	{
		if(!ctx->strs[i])
			{ i=65536; break; }
		if(!strcmp(ctx->strs[i], str))
			break;
	}
	
	//check found match
	if(i<65536)
	{
		//move to front
		s=ctx->strs[i];
		for(j=i; j>0; j--)
			ctx->strs[j]=ctx->strs[j-1];
		ctx->strs[0]=s;
		
		//emit string index
		DYLL_MetaBin_EmitSVLI(ctx, i);
		return;
	}

	//shift everything back
	//add string to front
	for(i=0; i<65535; i++)
		if(!ctx->strs[i])break;
	for(j=i; j>0; j--)
		ctx->strs[j]=ctx->strs[j-1];
	ctx->strs[0]=bgbcc_strdup(str);

	//emit string
	DYLL_MetaBin_EmitSVLI(ctx, -1);
	s=str;
	while(*s)DYLL_MetaBin_EmitByte(ctx, *s++);
	DYLL_MetaBin_EmitByte(ctx, 0);
	
	return;
}

void DYLL_MetaBin_SaveBinaryNode(
	BGBDYC_MetaBinary *ctx, BGBDYC_MetaNode *node)
{
	char tb[1024];
	BGBDYC_MetaLeaf *lcur;
	BGBDYC_MetaNode *ncur;
	char *s0;
	int i;

	s0=node->name;
	if(!s0)s0="";
	DYLL_MetaBin_EncodeString(ctx, s0);

	lcur=node->leaf; i=0;
	while(lcur)
	{
		if(!lcur->val || (lcur->val==NULL))
			{ lcur=lcur->next; continue; }
		i++; lcur=lcur->next;
	}
	DYLL_MetaBin_EmitVLI(ctx, i);

	lcur=node->leaf;
	while(lcur)
	{
		if(!lcur->val || (lcur->val==NULL))
		{
			lcur=lcur->next;
			continue;
		}

		DYLL_MetaBin_EncodeString(ctx, lcur->key);
		DYLL_MetaBin_EncodeString(ctx, lcur->val);
		lcur=lcur->next;
	}

	ncur=node->first; i=0;
	while(ncur)
		{ i++; ncur=ncur->next; }
	DYLL_MetaBin_EmitVLI(ctx, i);

	ncur=node->first;
	while(ncur)
	{
		DYLL_MetaBin_SaveBinaryNode(ctx, ncur);
		ncur=ncur->next;
	}
}

byte *DYLL_MetaBin_SaveBinaryBuf(char *name, int *rsz)
{
	BGBDYC_MetaBinary *ctx;
	byte *buf;
	FILE *fd;
	int i, sz;

	if(!dyll_metapath_root)
		return(NULL);

	ctx=malloc(sizeof(BGBDYC_MetaBinary));
	memset(ctx, 0, sizeof(BGBDYC_MetaBinary));

	DYLL_MetaBin_SaveBinaryNode(ctx, dyll_metapath_root);
	
	buf=ctx->buf;
	sz=ctx->ip-ctx->buf;
	
	free(ctx);
	
	if(rsz)*rsz=sz;
	return(buf);
}

