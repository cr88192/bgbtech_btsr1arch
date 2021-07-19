/*

This logic should be kept in-sync with the runtime library.
This will be used to compose (hopefully valid) tagged references at compile time.

*/


int ccxl_lvatyi_tagarray;
int ccxl_lvatyi_variant;
int ccxl_lvatyi_pointer;
int ccxl_lvatyi_byte;
int ccxl_lvatyi_sbyte;
int ccxl_lvatyi_short;
int ccxl_lvatyi_ushort;
int ccxl_lvatyi_int;
int ccxl_lvatyi_uint;
int ccxl_lvatyi_long;
int ccxl_lvatyi_ulong;
int ccxl_lvatyi_float;
int ccxl_lvatyi_double;

int ccxl_lvatyi_string;
int ccxl_lvatyi_wstring;
int ccxl_lvatyi_ustring;
int ccxl_lvatyi_classobj;

int ccxl_lvatyi_int128;
int ccxl_lvatyi_float128;

int ccxl_lvatyi_string;
int ccxl_lvatyi_wstring;
int ccxl_lvatyi_ustring;
int ccxl_lvatyi_classobj;

int ccxl_lvatyi_fixnum;
int ccxl_lvatyi_flonum;
int ccxl_lvatyi_tagobj;
int ccxl_lvatyi_cons;

int ccxl_lvatyi_arrmt[16];
int ccxl_lvatyi_arrmsc[16];


BGBCC_CCXL_LvaTagInfo *ccxl_lva_tagarr[1024];
short ccxl_lva_taghash[64];
int ccxl_lva_ntag=0;

void CCXL_LVA_TagInit(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	int i;

	if(ccxl_lva_ntag)
		return;

	i=ccxl_lva_ntag++;

	inf=bgbcc_malloc(sizeof(BGBCC_CCXL_LvaTagInfo));
	inf->name=NULL;
	inf->idx=i;
	ccxl_lva_tagarr[i]=inf;
}

int CCXL_LVA_HashName(char *str)
{
	char *s;
	int h;
	
	s=str; h=1;
	while(*s)
		{ h=(h*65521)+(*s++); }
	return(h*65521+1);
}

BGBCC_CCXL_LvaTagInfo *CCXL_LVA_LookupTagInfoForName(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	int h, c, n;
	int i;

	h=CCXL_LVA_HashName(name)&63;
	c=ccxl_lva_taghash[h];
	while(c>0)
	{
		inf=ccxl_lva_tagarr[c];
		if(!strcmp(inf->name, name))
			return(inf);
		c=inf->chain;
	}
	
	return(NULL);
}

BGBCC_CCXL_LvaTagInfo *CCXL_LVA_GetTagInfoForName(
	BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	int h, c, n;
	int i;

	h=CCXL_LVA_HashName(name)&63;
	c=ccxl_lva_taghash[h];
	while(c>0)
	{
		inf=ccxl_lva_tagarr[c];
		if(!strcmp(inf->name, name))
			return(inf);
		c=inf->chain;
	}

	i=ccxl_lva_ntag++;
	inf=bgbcc_malloc(sizeof(BGBCC_CCXL_LvaTagInfo));
	inf->name=bgbcc_strdup(name);
	inf->idx=i;
	ccxl_lva_tagarr[i]=inf;
	inf->chain=ccxl_lva_taghash[h];
	ccxl_lva_taghash[h]=i;
	return(inf);

	return(NULL);
}

int CCXL_LVA_LookupTagIndexForName(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	
	CCXL_LVA_Init(ctx);
	inf=CCXL_LVA_LookupTagInfoForName(ctx, name);
	return(inf->idx);
}

int CCXL_LVA_GetTagIndexForName(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	
	CCXL_LVA_Init(ctx);
	inf=CCXL_LVA_GetTagInfoForName(ctx, name);
	return(inf->idx);
}

char *CCXL_LVA_GetTagNameForIndex(BGBCC_TransState *ctx, int idx)
{
	BGBCC_CCXL_LvaTagInfo *inf;
	
	inf=ccxl_lva_tagarr[idx&1023];
	if(!inf)
		return(NULL);
	return(inf->name);
}

void CCXL_LVA_Init(BGBCC_TransState *ctx)
{
	byte *ptr, *ct;
	char *sig;
	
//	if(ccxl_lvatyi_tagarray)
	if(ccxl_lva_ntag)
		return;

	CCXL_LVA_TagInit(ctx);

	ccxl_lvatyi_tagarray	=CCXL_LVA_GetTagIndexForName(ctx, "_TagArray");	//01
	ccxl_lvatyi_variant		=CCXL_LVA_GetTagIndexForName(ctx, "_Variant");	//02
	ccxl_lvatyi_string		=CCXL_LVA_GetTagIndexForName(ctx, "_String");	//03
	ccxl_lvatyi_wstring		=CCXL_LVA_GetTagIndexForName(ctx, "_WString");	//04
	ccxl_lvatyi_ustring		=CCXL_LVA_GetTagIndexForName(ctx, "_UString");	//05
	ccxl_lvatyi_classobj	=CCXL_LVA_GetTagIndexForName(ctx, "_ClassObj");	//06
	ccxl_lvatyi_pointer		=CCXL_LVA_GetTagIndexForName(ctx, "_Pointer");	//07
	ccxl_lvatyi_byte		=CCXL_LVA_GetTagIndexForName(ctx, "_Byte");		//08
	ccxl_lvatyi_sbyte		=CCXL_LVA_GetTagIndexForName(ctx, "_SByte");	//09
	ccxl_lvatyi_short		=CCXL_LVA_GetTagIndexForName(ctx, "_Short");	//0A
	ccxl_lvatyi_ushort		=CCXL_LVA_GetTagIndexForName(ctx, "_UShort");	//0B
	ccxl_lvatyi_int			=CCXL_LVA_GetTagIndexForName(ctx, "_Int");		//0C
	ccxl_lvatyi_uint		=CCXL_LVA_GetTagIndexForName(ctx, "_UInt");		//0D
	ccxl_lvatyi_long		=CCXL_LVA_GetTagIndexForName(ctx, "_Long");		//0E
	ccxl_lvatyi_ulong		=CCXL_LVA_GetTagIndexForName(ctx, "_ULong");	//0F
	ccxl_lvatyi_float		=CCXL_LVA_GetTagIndexForName(ctx, "_Float");	//10
	ccxl_lvatyi_double		=CCXL_LVA_GetTagIndexForName(ctx, "_Double");	//11
	ccxl_lvatyi_int128		=CCXL_LVA_GetTagIndexForName(ctx, "_Int128");	//12
	ccxl_lvatyi_float128	=CCXL_LVA_GetTagIndexForName(ctx, "_Float128");	//13
	ccxl_lvatyi_fixnum		=CCXL_LVA_GetTagIndexForName(ctx, "_Fixnum");	//14
	ccxl_lvatyi_flonum		=CCXL_LVA_GetTagIndexForName(ctx, "_Flonum");	//15
	ccxl_lvatyi_tagobj		=CCXL_LVA_GetTagIndexForName(ctx, "_TagObj");	//16
	ccxl_lvatyi_cons		=CCXL_LVA_GetTagIndexForName(ctx, "_Cons");		//17
}
