#ifdef __BGBCC__

typedef struct
{
    u32 mFlags;                       //00, Flags
    u32 mDateTime;                    //04, Data/Time Compiled
    u32 fccMagic;                     //08, 'RWAD'
    u32 mMagicZero;                   //0C, Magic (Set to 0)
    u32 rvaDir;                       //10, RVA of Directory
    u32 szDir;                        //14, Size of Directory (Bytes)
    u32 rvaTypes;                     //18, RVA of Types
    u32 szTypes;                      //1C, Size of Types
    u32 rvaHead;                      //20, RVA of Header (Self-Reference)
    u32 szHead;                       //24, Size of Header (Self-Refernece)
} rwadhead_t;

typedef struct
{
    u32  rvaData;                      //RVA of Lump Data
    u32  cszData;                      //Compressed Size of Lump Data
    u32  dszData;                      //Uncompressed Size of Lump Data
    byte mType;                        //Lump Type
    byte mMethod;                      //Lump Compression Method
    u16  mChain;                       //Lump Lookup Chain.
    byte mName[16];                    //Name of Lump
} rwadlump_t;

extern void __rsrc_start;
extern void __rsrc_dir;
extern void __rsrc_types;

rwadhead_t *tk_rwad_head;
rwadlump_t *tk_rwad_dir;
u32 *tk_rwad_types;
u16 *tk_rwad_hash;

int TKPE_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);
byte *TKPE_UnpackL4(byte *ct, byte *ibuf, int isz);


void TK_RWad_Init()
{
	if(tk_rwad_head)
		return;
	
	tk_rwad_head=(&__rsrc_start);
	tk_rwad_dir=(&__rsrc_dir);
	tk_rwad_types=(&__rsrc_types);
	tk_rwad_hash=(u16 *)(((byte *)(&__rsrc_dir))+(tk_rwad_head->szDir));
}

void TK_RWad_FillName(char *tname, char *name)
{
	char *s, *t, *te;
	int i;

	((u64 *)tname)[0]=0;
	((u64 *)tname)[1]=0;	
	s=name; t=tname; te=t+16;
	while(*s && (t<te))
	{
		i=*s++;
		if(((byte)(i-'A'))<=('Z'-'A'))
			i+='a'-'A';
		*t++=i;
	}
}

int TK_RWad_HashName(char *name)
{
	u32 *pn;
	int v, h;
	pn=(u32 *)name;
	v=pn[0]+pn[1]+pn[2]+pn[3];
	h=((v*15813251)>>24)&63;
	return(h);
}

int TK_RWad_GetLumpNumForName(char *name)
{
	char tn[17];
	rwadlump_t *de;
	char *s;
	int h;
	int i, j, k;

	TK_RWad_Init();
	TK_RWad_FillName(tn, name);
	h=TK_RWad_HashName(tn);
	
	i=tk_rwad_hash[h];
	while(i!=0xFFFF)
	{
		de=tk_rwad_dir+i;
		s=de->mName;
		if(	((((u64 *)tn)[0])==(((u64 *)s)[0]))	&&
			((((u64 *)tn)[1])==(((u64 *)s)[1]))	)
				return(i);
		i=de->mChain;
	}
	return(-1);
}

int TK_RWad_GetLumpSize(int lump)
{
	rwadlump_t *de;
	de=tk_rwad_dir+lump;
	return(de->dszData);
}

int TK_RWad_ReadLump(int lump, void *dest)
{
	rwadlump_t *de;
	byte *ibuf;
	byte *cbuf;
	
	de=tk_rwad_dir+lump;
	ibuf=((byte *)tk_rwad_head)-(tk_rwad_head->rvaHead);
	cbuf=ibuf+de->rvaData;

	if(de->mMethod==0)
	{
		memcpy(dest, cbuf, de->dszData);
		return(0);
	}

	if(de->mMethod==3)
	{
		TKPE_DecodeBufferRP2(cbuf, dest, de->cszData, de->dszData);
		return(0);
	}

	if(de->mMethod==4)
	{
		TKPE_UnpackL4(dest, cbuf, de->cszData);
		return(0);
	}
	
	return(-1);
}

#endif
