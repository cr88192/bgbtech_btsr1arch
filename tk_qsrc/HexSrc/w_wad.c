
//**************************************************************************
//**
//** w_wad.c : Heretic 2 : Raven Software, Corp.
//**
//** $RCSfile: w_wad.c,v $
//** $Revision: 1.6 $
//** $Date: 95/10/06 20:56:47 $
//** $Author: cjr $
//**
//**************************************************************************

// HEADER FILES ------------------------------------------------------------

#if 0
#ifdef NeXT
#include <libc.h>
#include <ctype.h>
#else
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
#endif

#include <ctype.h>

#include "h2def.h"

// MACROS ------------------------------------------------------------------

#ifdef NeXT
// NeXT doesn't need a binary flag in open call
#define O_BINARY 0
#define strcmpi strcasecmp
#endif

// TYPES -------------------------------------------------------------------

typedef struct
{
	char identification[4];
	int numlumps;
	int infotableofs;
} wadinfo_t;

typedef struct
{
	int filepos;
	int size;
	char name[8];
} filelump_t;

typedef struct
{
	int		filepos;
	int		csize;
	int		dsize;
	char	ety;		//Entry Type
	char	cmp;		//Compression
	short	chn;		//Chain (ExWAD)
	char	name[16];
} wad2lump_t;

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

// PUBLIC DATA DEFINITIONS -------------------------------------------------

lumpinfo_t *lumpinfo;
int numlumps;
void **lumpcache;

int			lumphash[64];

// PRIVATE DATA DEFINITIONS ------------------------------------------------

static lumpinfo_t *PrimaryLumpInfo;
static int PrimaryNumLumps;
static void **PrimaryLumpCache;
static lumpinfo_t *AuxiliaryLumpInfo;
static int AuxiliaryNumLumps;
static void **AuxiliaryLumpCache;
static int AuxiliaryHandle = 0;
dt_bool AuxiliaryOpened = false;

// CODE --------------------------------------------------------------------

// #ifdef NeXT
#if 0
//==========================================================================
//
// strupr
//
//==========================================================================

void strupr(char *s)
{
    while(*s)
	*s++ = toupper(*s);
}

//==========================================================================
//
// filelength
//
//==========================================================================

int filelength(int handle)
{
    struct stat fileinfo;

    if(fstat(handle, &fileinfo) == -1)
	{
		I_Error("Error fstating");
	}
    return fileinfo.st_size;
}
#endif

#if 1
void w_strupr (char *s)
{
	while (*s) { *s = toupper(*s); s++; }
}

void w_strupr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = toupper(*s++); }
	for(; i<n; i++)
		*t++=0;
}

int w_chkaccess(const char *name)
{
	FILE *fd;
	int st;
	
	fd=fopen(name, "rb");
	if(fd)
	{
		fclose(fd);
		return(0);
	}
	return(-1);
}

FILE *w_openfiles[32];
u32 w_m_openfiles;

int w_open(const char *name, const char *mode)
{
	FILE *fd;
	int i;
	
	fd=fopen(name, mode);
	if(!fd)
	{
		return(-1);
	}
		
	for(i=0; i<32; i++)
		if(!(w_m_openfiles&(1<<i)))
			break;
	
	if(i>=32)
		return(-1);
	
	w_m_openfiles|=(1<<i);
//	i=w_n_openfiles++;
	w_openfiles[i]=fd;
	return(i);
}

int w_read(int hdl, void *buf, int sz)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		{ DT_BREAKPOINT }
//		return(-1);
	if(hdl>=32)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		{ DT_BREAKPOINT }
//		return(-1);
	
	i=fread(buf, 1, sz, fd);
	return(i);
}

int w_write(int hdl, void *buf, int sz)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		{ DT_BREAKPOINT }
//		return(-1);
	if(hdl>=32)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		{ DT_BREAKPOINT }
//		return(-1);
	
	i=fwrite(buf, 1, sz, fd);
	return(i);
}

int w_lseek(int hdl, int ofs, int set)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		{ DT_BREAKPOINT }
//		return(-1);
	if(hdl>=32)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		{ DT_BREAKPOINT }
//		return(-1);
	
	i=fseek(fd, ofs, set);
	if(i<0)
		return(i);
	i=ftell(fd);
	return(i);
}

int w_close(int hdl)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		{ DT_BREAKPOINT }
//		return(-1);
	if(hdl>=32)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fclose(w_openfiles[hdl]);
	w_m_openfiles&=~(1<<hdl);
	w_openfiles[hdl]=NULL;
	return(0);
}

int w_filelength (int hdl) 
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		{ DT_BREAKPOINT }
//		return(-1);
	if(hdl>=32)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		{ DT_BREAKPOINT }
//		return(-1);
	
	fseek(fd, 0, 2);
	i=ftell(fd);
	fseek(fd, 0, 0);
	return(i);
}
#endif

//==========================================================================
//
// W_AddFile
//
// Files with a .wad extension are wadlink files with multiple lumps,
// other files are single lumps with the base filename for the lump name.
//
//==========================================================================

void W_AddFile(char *filename)
{
	wadinfo_t header;
	lumpinfo_t *lump_p;
	unsigned i;
	int handle, length;
	int startlump;
	filelump_t *fileinfo, singleinfo;
	filelump_t *freeFileInfo;
	wad2lump_t		*wad2info;

//	if((handle = open(filename, O_RDONLY|O_BINARY)) == -1)
	if((handle = w_open(filename, "rb")) == -1)
	{ // Didn't find file
		printf("W_AddFile: fail open '%s'\n", filename);
		return;
	}

	wad2info=NULL;
	fileinfo=NULL;

	startlump = numlumps;
	if(strcmpi(filename+strlen(filename)-3, "wad"))
	{ // Single lump file
		fileinfo = &singleinfo;
		freeFileInfo = NULL;
		singleinfo.filepos = 0;
		singleinfo.size = LONG(w_filelength(handle));
		M_ExtractFileBase(filename, singleinfo.name);
		numlumps++;
	}
	else
	{ // WAD file
#if 0
		w_read(handle, &header, sizeof(header));
		if(strncmp(header.identification, "IWAD", 4))
		{
			if(strncmp(header.identification, "PWAD", 4))
			{ // Bad file id
				I_Error("Wad file %s doesn't have IWAD or PWAD id\n",
					filename);
			}
		}
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps*sizeof(filelump_t);
//		fileinfo = alloca(length);
		if(!(fileinfo = malloc(length)))
		{
			I_Error("W_AddFile:  fileinfo malloc failed\n");
		}
		freeFileInfo = fileinfo;
		w_lseek(handle, header.infotableofs, SEEK_SET);
		w_read(handle, fileinfo, length);
		numlumps += header.numlumps;
#endif

#if 1
		// WAD file
		w_read (handle, &header, sizeof(header));

		if (!strncmp(header.identification, "IWAD", 4) ||
			!strncmp(header.identification, "PWAD", 4))
		{
			header.numlumps = LONG(header.numlumps);
			header.infotableofs = LONG(header.infotableofs);
			length = header.numlumps*sizeof(filelump_t);
			fileinfo = malloc (length);
			freeFileInfo = fileinfo;
			w_lseek (handle, header.infotableofs, SEEK_SET);
			w_read (handle, fileinfo, length);
			numlumps += header.numlumps;
		}else
			if(!strncmp(header.identification, "WAD2", 4))
		{
			header.numlumps = LONG(header.numlumps);
			header.infotableofs = LONG(header.infotableofs);
			length = header.numlumps*sizeof(wad2lump_t);
			wad2info = malloc (length);
			freeFileInfo = (filelump_t *)wad2info;
			w_lseek (handle, header.infotableofs, SEEK_SET);
			w_read (handle, wad2info, length);
			numlumps += header.numlumps;
		}else
		{
			I_Error ("Wad file %s doesn't have "
				"IWAD, PWAD, or WAD2 id\n", filename);
		}
#endif
	}

	// Fill in lumpinfo
	lumpinfo = realloc(lumpinfo, numlumps*sizeof(lumpinfo_t));
	if(!lumpinfo)
	{
		I_Error("Couldn't realloc lumpinfo");
	}
	lump_p = &lumpinfo[startlump];

	if(wad2info)
	{
		for (i=startlump ; i<numlumps ; i++, lump_p++, wad2info++)
		{
			lump_p->handle = handle;
			lump_p->position = LONG(wad2info->filepos);
			lump_p->size = LONG(wad2info->dsize);
			lump_p->csize = LONG(wad2info->csize);
			lump_p->ety = wad2info->ety;
			lump_p->cmp = wad2info->cmp;
//			strncpy (lump_p->name, wad2info->name, 8);
			w_strupr_n (lump_p->name, wad2info->name, 16);
		}
	}else
	{
		for(i = startlump; i < numlumps; i++, lump_p++, fileinfo++)
		{
			memset(lump_p, 0, sizeof(*lump_p));
			lump_p->handle = handle;
			lump_p->position = LONG(fileinfo->filepos);
			lump_p->size = LONG(fileinfo->size);
			lump_p->csize = LONG(fileinfo->size);
			strncpy(lump_p->name, fileinfo->name, 8);
		}
	}

	if(freeFileInfo)
	{
		free(freeFileInfo);
	}
}

//==========================================================================
//
// W_InitMultipleFiles
//
// Pass a null terminated list of files to use.  All files are optional,
// but at least one file must be found.  Lump names can appear multiple
// times.  The name searcher looks backwards, so a later file can
// override an earlier one.
//
//==========================================================================

void W_InitMultipleFiles(char **filenames)
{
	lumpinfo_t*		lump_p;
	int size;
	int		i, j, h;

	// Open all the files, load headers, and count lumps
	numlumps = 0;
	lumpinfo = malloc(1); // Will be realloced as lumps are added

	for(; *filenames; filenames++)
	{
		printf("%s\n", *filenames);
		W_AddFile(*filenames);
	}
	if(!numlumps)
	{
		I_Error("W_InitMultipleFiles: no files found");
	}

	for(i=0; i<64; i++)
	{
		lumphash[i] = -1;
	}
	
	for(i=0; i<numlumps; i++)
	{
		lump_p = lumpinfo + i;
//		j = *(int *)(lump_p->name);
//		h = ((j*65521)>>16)&63;
		h = W_HashIndexForName(lump_p->name);
		lump_p->chain = lumphash[h];
		lumphash[h] = i;
	}
	
	// Set up caching
	size = numlumps*sizeof(*lumpcache);
	lumpcache = malloc(size);
	if(!lumpcache)
	{
		I_Error("Couldn't allocate lumpcache");
	}
	memset(lumpcache, 0, size);

	PrimaryLumpInfo = lumpinfo;
	PrimaryLumpCache = lumpcache;
	PrimaryNumLumps = numlumps;
}

//==========================================================================
//
// W_InitFile
//
// Initialize the primary from a single file.
//
//==========================================================================

void W_InitFile(char *filename)
{
	char *names[2];

	names[0] = filename;
	names[1] = NULL;
	W_InitMultipleFiles(names);
}

//==========================================================================
//
// W_OpenAuxiliary
//
//==========================================================================

void W_OpenAuxiliary(char *filename)
{
	int i;
	int size;
	wadinfo_t header;
	int handle;
	int length;
	filelump_t *fileinfo;
	filelump_t *sourceLump;
	lumpinfo_t *destLump;

	if(AuxiliaryOpened)
	{
		W_CloseAuxiliary();
	}
//	if((handle = open(filename, O_RDONLY|O_BINARY)) == -1)
	if((handle = w_open(filename, "rb")) == -1)
	{
		I_Error("W_OpenAuxiliary: %s not found.", filename);
		return;
	}
	AuxiliaryHandle = handle;
	w_read(handle, &header, sizeof(header));
	if(strncmp(header.identification, "IWAD", 4))
	{
		if(strncmp(header.identification, "PWAD", 4))
		{ // Bad file id
			I_Error("Wad file %s doesn't have IWAD or PWAD id\n",
				filename);
		}
	}
	header.numlumps = LONG(header.numlumps);
	header.infotableofs = LONG(header.infotableofs);
	length = header.numlumps*sizeof(filelump_t);
	fileinfo = Z_Malloc(length, PU_STATIC, 0);
	w_lseek(handle, header.infotableofs, SEEK_SET);
	w_read(handle, fileinfo, length);
	numlumps = header.numlumps;

	// Init the auxiliary lumpinfo array
	lumpinfo = Z_Malloc(numlumps*sizeof(lumpinfo_t), PU_STATIC, 0);
	sourceLump = fileinfo;
	destLump = lumpinfo;
	for(i = 0; i < numlumps; i++, destLump++, sourceLump++)
	{
		destLump->handle = handle;
		destLump->position = LONG(sourceLump->filepos);
		destLump->size = LONG(sourceLump->size);
		strncpy(destLump->name, sourceLump->name, 8);
	}
	Z_Free(fileinfo);

	// Allocate the auxiliary lumpcache array
	size = numlumps*sizeof(*lumpcache);
	lumpcache = Z_Malloc(size, PU_STATIC, 0);
	memset(lumpcache, 0, size);

	AuxiliaryLumpInfo = lumpinfo;
	AuxiliaryLumpCache = lumpcache;
	AuxiliaryNumLumps = numlumps;
	AuxiliaryOpened = true;
}

//==========================================================================
//
// W_CloseAuxiliary
//
//==========================================================================

void W_CloseAuxiliary(void)
{
	int i;

	if(AuxiliaryOpened)
	{
		W_UseAuxiliary();
		for(i = 0; i < numlumps; i++)
		{
			if(lumpcache[i])
			{
				Z_Free(lumpcache[i]);
			}
		}
		Z_Free(AuxiliaryLumpInfo);
		Z_Free(AuxiliaryLumpCache);
		W_CloseAuxiliaryFile();
		AuxiliaryOpened = false;
	}
	W_UsePrimary();
}

//==========================================================================
//
// W_CloseAuxiliaryFile
//
// WARNING: W_CloseAuxiliary() must be called before any further
// auxiliary lump processing.
//
//==========================================================================

void W_CloseAuxiliaryFile(void)
{
	if(AuxiliaryHandle)
	{
		w_close(AuxiliaryHandle);
		AuxiliaryHandle = 0;
	}
}

//==========================================================================
//
// W_UsePrimary
//
//==========================================================================

void W_UsePrimary(void)
{
	lumpinfo = PrimaryLumpInfo;
	numlumps = PrimaryNumLumps;
	lumpcache = PrimaryLumpCache;
}

//==========================================================================
//
// W_UseAuxiliary
//
//==========================================================================

void W_UseAuxiliary(void)
{
	if(AuxiliaryOpened == false)
	{
		I_Error("W_UseAuxiliary: WAD not opened.");
	}
	lumpinfo = AuxiliaryLumpInfo;
	numlumps = AuxiliaryNumLumps;
	lumpcache = AuxiliaryLumpCache;
}

//==========================================================================
//
// W_NumLumps
//
//==========================================================================

int	W_NumLumps(void)
{
	return numlumps;
}

//==========================================================================
//
// W_CheckNumForName
//
// Returns -1 if name not found.
//
//==========================================================================

#if 0
int W_CheckNumForName(char *name)
{
	char name8[9];
	int v1, v2;
	lumpinfo_t *lump_p;

	// Make the name into two integers for easy compares
	strncpy(name8, name, 8);
	name8[8] = 0; // in case the name was a full 8 chars
	w_strupr(name8); // case insensitive
	v1 = *(int *)name8;
	v2 = *(int *)&name8[4];

	// Scan backwards so patch lump files take precedence
	lump_p = lumpinfo+numlumps;
	while(lump_p-- != lumpinfo)
	{
		if(*(int *)lump_p->name == v1 && *(int *)&lump_p->name[4] == v2)
		{
			return lump_p-lumpinfo;
		}
	}
	return -1;
}
#endif


void W_MatchCopy2(byte *dst, int sz, int d)
{
	byte *cs, *ct, *cte;
	u64 v;
	
	if(d<8)
	{
		if(d==1)
		{
			v=*(dst-d);
			v=v|(v<<8);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==2)
		{
			v=*(u16 *)(dst-d);
			v=v|(v<<16);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
			if(d==4)
		{
			v=*(u32 *)(dst-d);
			v=v|(v<<32);

			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=8;
			}
		}else
		{
			v=*(u64 *)(dst-d);
			ct=dst; cte=dst+sz;
			while(ct<cte)
			{
				*(u64 *)ct=v;
				ct+=d;
			}
		}
	}else
		if(sz<=16)
	{
		cs=dst-d;
		((u64 *)dst)[0]=((u64 *)cs)[0];
		((u64 *)dst)[1]=((u64 *)cs)[1];
	}else
	{
		cs=dst-d;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			ct+=16; cs+=16;
		}
	}
}

void W_RawCopy(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	if(sz>8)
	{
		cs=src;
		ct=dst; cte=dst+sz;
		while(ct<cte)
		{
			((u64 *)ct)[0]=((u64 *)cs)[0];
			((u64 *)ct)[1]=((u64 *)cs)[1];
			ct+=16; cs+=16;
		}
	}else
	{
		*(u64 *)dst=*(u64 *)src;
	}
}

void W_RawCopyB(byte *dst, byte *src, int sz)
{
	byte *cs, *ct, *cte;

	cs=src;
	ct=dst; cte=dst+sz;
	while(ct<cte)
	{
		((u64 *)ct)[0]=((u64 *)cs)[0];
		((u64 *)ct)[1]=((u64 *)cs)[1];
		ct+=16; cs+=16;
	}
}

int W_DecodeBufferLZ4(byte *ibuf, byte *obuf, int isz, int osz)
{
	byte *ct, *cs, *cse;
	byte *cs1, *cs1e, *ct1;
	u64 tv;
	int tg, lr, ll, ld;
	int i;
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	ct=obuf;
	cs=ibuf; cse=ibuf+isz;
	while(cs<cse)
	{
		tg=*cs++;
		lr=(tg>>4)&15;
		if(lr==15)
		{
			i=*cs++;
			while(i==255)
				{ lr+=255; i=*cs++; }
			lr+=i;
		}

		if(lr)
		{
			W_RawCopy(ct, cs, lr);
			cs+=lr;	ct+=lr;
		}

		if((cs+1)>=cse)
			{ break; }
		
		ld=*(u16 *)cs;
		cs+=2;
		if(!ld)
		{
			printf("TKPE_UnpackL4: End Of Image\n");
			break;
		}
		ll=(tg&15)+4;
		if(ll==19)
		{
			i=*cs++;
			while(i==255)
				{ ll+=255; i=*cs++; }
			ll+=i;
		}

		W_MatchCopy2(ct, ll, ld);
		ct+=ll;
	}
	
	tg=0;	lr=0;
	ll=0;	ld=0;
	
	return(ct-obuf);
}

#if 0
int W_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse;
	int pl, pd;
	int rl, l, d;
	u64 t0;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
	while(1)
	{
		t0=*(u64 *)cs;
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
			*(u32 *)ct=*(u32 *)cs;
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
			W_RawCopyB(ct, cs, rl);
			cs+=rl;
			ct+=rl;
			continue;
		}else
		{
			{ DT_BREAKPOINT }
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		W_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}
#endif

#ifdef __BJX2__
// #if 0
int W_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz);

__asm {
W_DecodeBufferRP2:
// R4=ibuf
// R5=obuf
// R6=ibsz / scratch
// R7=obsz / scratch
// R16=scratch
// R17=scratch
// R18=cs
// R19=ct
// R20=rl
// R21=l
// R22=d

	MOV R4, R18
	MOV R5, R19
	
.L0:
	MOV.Q	(R18), R16
	TEST	0x01, R16
	BT		.L2
	TEST	0x02, R16
	BT		.L3
	TEST	0x04, R16
	BT		.L4
	TEST	0x08, R16
	BT		.L5
	TEST	0x10, R16
	BT		.L6
	TEST	0x20, R16
	BT		.L7
	TEST	0x40, R16
	BT		.L8
	BREAK
.L2:
	ADD		2, R18		|	SHLD	R16, -1, R20
	AND		7, R20		|	SHLD	R16, -4, R21
	AND		7, R21		|	SHLD	R16, -7, R22
	AND		511, R22	|	ADD		3, R21	
	BRA		.L9
.L3:
	MOV		8191, R3
	ADD		3, R18	|	SHLD	R16, -2, R20
	AND		7, R20	|	SHLD	R16, -5, R21
	AND		63, R21	|	SHLD	R16, -11, R22
	AND		R3, R22	|	ADD		4, R21	
	BRA		.L9
.L4:
	ADD		4, R18		|	SHLD	R16, -3, R20
	AND		7, R20		|	SHLD	R16, -6, R21
	AND		511, R21	|	SHLD	R16, -15, R22
	ADD		4, R21
	BRA		.L9
.L5:
	ADD		1, R18		|	SHAD	R16, -1, R20
	AND		0x78, R20
	ADD		8, R20
.L5_1:
	ADD		R18, R20, R6	|	ADD		R19, R20, R7
.L5_0:
	MOV.Q	(R18, 0), R16
	MOV.Q	(R18, 8), R17
	ADD		16, R18
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		16, R19
	CMPQGT	R19, R7
	BT		.L5_0
	MOV		R6, R18		|	MOV		R7, R19
	BRA		.L0
.L6:
	MOV		0x3FFF, R1
	MOV		0x7FFF, R2
	MOV		0x3FFFFF, R3
	ADD			1, R18			|	SHLD		R16, -5, R20
	AND			7, R20			|	TEST		256, R16
	ADD?T		1, R18			|	ADD?F		2, R18
	SHLD?T		R16, -9, R21	|	SHLD?F		R16, -10, R21
	SHLD.Q?T	R16, -16, R7	|	SHLD.Q?F	R16, -24, R7
	AND?T		127, R21		|	AND?F		R1, R21
	ADD			4, R21			|	TEST		1, R7
	ADD?T		2, R18			|	ADD?F		3, R18
	SHLD?T		R7, -1, R22		|	SHLD?F		R7, -2, R22
	AND?T		R2, R22			|	AND?F		R3, R22
	BRA		.L9
.L7:
	ADD		1, R18		|	SHLD	R16, -6, R20
	AND		3, R20
	TEST	R20, R20
	BT		.L1
	MOV.L	(R18), R16
	ADD		R20, R18
	MOV.L	R16, (R19)
	ADD		R20, R19
	BRA		.L0
.L8:
	ADD		2, R18		|	SHLD	R16, -7, R20
	AND		511, R20
	ADD		1, R20
	SHLD	R20, 3, R20
	BRA		.L5_1
.L9:
	MOV.Q	(R18), R16
	ADD		R20, R18
	MOV.Q	R16, (R19)
	ADD		R20, R19
	SUB		R19, R22, R6
	ADD		R19, R21, R7
	CMPGT	15, R22
	BT		.L11
	MOV.Q	(R6, 0), R16
	MOV.Q	(R6, 8), R17
.L12:
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		R22, R19
	CMPQGT	R19, R7
	BT		.L12
	BRA		.L10
.L11:
	MOV.Q	(R6, 0), R16
	MOV.Q	(R6, 8), R17
	ADD		16, R6
	MOV.Q	R16, (R19, 0)
	MOV.Q	R17, (R19, 8)
	ADD		16, R19
	CMPQGT	R19, R7
	BT		.L11
.L10:
	MOV		R7, R19
	BRA		.L0
.L1:
	SUB		R19, R5, R2
	RTS
};
#endif

#ifndef __BJX2__
//#if 1
int W_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	u32 tag;
	byte *cs, *ct, *cse, *cs1, *cs1e, *ct1e;
	int pl, pd;
	int rl, l, d;
	u64 t0, v0, v1;
	int t1, t2;
	
	cs=ibuf; cse=ibuf+ibsz;
	ct=obuf;
	pl=0; pd=0;
	
	while(1)
	{
		t0=*(u64 *)cs;
		if(!(t0&0x01))
		{
			cs+=2;
			rl=(t0>>1)&7;
			l=((t0>>4)&7)+3;
			d=(t0>>7)&511;
		}else
			if(!(t0&0x02))
		{
			cs+=3;
			rl=(t0>>2)&7;
			l=((t0>>5)&63)+4;
			d=(t0>>11)&8191;
		}else
			if(!(t0&0x04))
		{
			cs+=4;
			rl=(t0>>3)&7;
			l=((t0>>6)&511)+4;
			d=(t0>>15)&131071;
		}else
			if(!(t0&0x08))
		{
			cs++;
			t1=(t0>>4)&15;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
//			cs+=rl;
//			ct+=rl;

			cs1e=cs+rl;
			ct1e=ct+rl;
			while(ct<ct1e)
			{
				v0=((u64 *)cs)[0];
				v1=((u64 *)cs)[1];
				cs+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}			
			cs=cs1e;
			ct=ct1e;

			continue;
		}else
			if(!(t0&0x10))
		{
			/* Long Match */
			cs++;
			rl=(t0>>5)&7;
			t1=t0>>8;
			if(!(t1&1))
				{ l=((t1>>1)&0x007F)+4; cs+=1; t2=t0>>16; }
			else
				{ l=((t1>>2)&0x3FFF)+4; cs+=2; t2=t0>>24; }
			if(!(t2&1))
				{ d=((t2>>1)&0x007FFF); cs+=2; }
			else
				{ d=((t2>>2)&0x3FFFFF); cs+=3; }
		}else
			if(!(t0&0x20))
		{
			cs++;
			rl=(t0>>6)&3;
			if(!rl)break;
			*(u32 *)ct=*(u32 *)cs;
			cs+=rl;
			ct+=rl;
			continue;
		}else
			if(!(t0&0x40))
		{
			/* Long Raw */
			cs+=2;
			t1=(t0>>7)&511;
			rl=(t1+1)*8;
//			W_RawCopyB(ct, cs, rl);
//			cs+=rl;
//			ct+=rl;

			cs1e=cs+rl;
			ct1e=ct+rl;
			while(ct<ct1e)
			{
				v0=((u64 *)cs)[0];
				v1=((u64 *)cs)[1];
				cs+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}			
			cs=cs1e;
			ct=ct1e;
			continue;
		}else
		{
			{ DT_BREAKPOINT }
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		
		cs1=ct-d;
		ct1e=ct+l;
		
		if(d<16)
		{
			v0=((u64 *)cs1)[0];
			v1=((u64 *)cs1)[1];
			while(ct<ct1e)
			{
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=d;
			}
		}else
		{
			while(ct<ct1e)
			{
				v0=((u64 *)cs1)[0];
				v1=((u64 *)cs1)[1];
				cs1+=16;
				((u64 *)ct)[0]=v0;
				((u64 *)ct)[1]=v1;
				ct+=16;
			}
		}
		
		ct=ct1e;
		
//		W_MatchCopy2(ct, l, d);
//		ct+=l;
	}
	
	return(ct-obuf);
}
#endif


int W_HashIndexForName(char *s)
{
	int j, h;
//	j = *(int *)(s);
	j = (((int *)(s))[0])+(((int *)(s))[1]);
//	h = ((j*65521)>>16)&63;
//	h = ((j*16777213)>>24)&63;
	h = ((j*0xF14A83)>>24)&63;
	return(h);
}

int W_CheckNumForName (char* name)
{
	return(W_CheckNumForNameBase(numlumps, name));
}

int W_CheckNumForNameBase (int base, char* name)
{
	union {
//	char	s[9];
	char	s[12];
	int	x[2];
	
	} name8;
	
	int		v1;
	int		v2;
	int 	i, h;
	lumpinfo_t*	lump_p;

	// make the name into two integers for easy compares
	w_strupr_n(name8.s, name, 8);

	v1 = name8.x[0];
	v2 = name8.x[1];

#if 1
	h = W_HashIndexForName(name8.s);
	i=lumphash[h];

#if 1
	while(i >= base)
	{
		lump_p = lumpinfo + i;
		i = lump_p->chain;
		continue;
	}
#endif

	while(i >= 0)
	{
		lump_p = lumpinfo + i;
		
		if ( *(int *)lump_p->name == v1
			 && *(int *)&lump_p->name[4] == v2)
		{
			break;
		}
		i = lump_p->chain;
	}
	if(i>=0)
		return(i);
		
#endif

#if 0
	// scan backwards so patch lump files take precedence
//	lump_p = lumpinfo + numlumps;
	lump_p = lumpinfo + base;

	while (lump_p-- != lumpinfo)
	{
		if ( *(int *)lump_p->name == v1
			 && *(int *)&lump_p->name[4] == v2)
		{
			return lump_p - lumpinfo;
		}
	}
#endif

	// TFB. Not found.
	return -1;
}


//==========================================================================
//
// W_GetNumForName
//
// Calls W_CheckNumForName, but bombs out if not found.
//
//==========================================================================

int	W_GetNumForName (char *name)
{
	int	i;

	if((name[0]<=' ') || (name[0]>'~'))
		{ DT_BREAKPOINT }

	i = W_CheckNumForName(name);
	if(i != -1)
	{
		return i;
	}
	
	DT_BREAKPOINT
	I_Error("W_GetNumForName: %s not found!", name);
	return -1;
}

//==========================================================================
//
// W_LumpLength
//
// Returns the buffer size needed to load the given lump.
//
//==========================================================================

int W_LumpLength(int lump)
{
	if(lump >= numlumps)
	{
		I_Error("W_LumpLength: %i >= numlumps", lump);
	}
	return lumpinfo[lump].size;
}

char *W_GetNameForNum(int lump)
{
	static char tn[17];
	memcpy(tn, lumpinfo[lump].name, 16);
	tn[16]=0;
	return(tn);
}

//==========================================================================
//
// W_ReadLump
//
// Loads the lump into the given buffer, which must be >= W_LumpLength().
//
//==========================================================================

void W_ReadLump(int lump, void *dest)
{
	static byte	*tcbuf=NULL;
	static int		tcsize;
	int				c, csz;
	lumpinfo_t		*l;

	if(lump >= numlumps)
	{
		I_Error("W_ReadLump: %i >= numlumps", lump);
	}
	l = lumpinfo+lump;

	if(l->cmp)
	{
		if(!tcbuf || (l->csize>tcsize))
		{
			csz=tcbuf?tcsize:16384;
			while(l->csize>csz)
				csz=csz+(csz>>1);
			tcbuf=realloc(tcbuf, csz);
			tcsize=csz;
		}

		w_lseek (l->handle, l->position, SEEK_SET);
		c = w_read (l->handle, tcbuf, l->csize);
		if (c < l->csize)
			I_Error ("W_ReadLump: only read %i of %i on lump %i",
				c,l->csize,lump);
		if(l->cmp==3)
			{ W_DecodeBufferRP2(tcbuf, dest, l->csize, l->size); }
		else if(l->cmp==4)
			{ W_DecodeBufferLZ4(tcbuf, dest, l->csize, l->size); }
		
		return;
	}

	//I_BeginRead();
	w_lseek(l->handle, l->position, SEEK_SET);
	c = w_read(l->handle, dest, l->size);
	if(c < l->size)
	{
		I_Error("W_ReadLump: only read %i of %i on lump %i",
			c, l->size, lump);
	}
	//I_EndRead();
}

//==========================================================================
//
// W_CacheLumpNum
//
//==========================================================================

void *W_CacheLumpNum(int lump, int tag)
{
	byte *ptr;

	if((unsigned)lump >= numlumps)
	{
		I_Error("W_CacheLumpNum: %i >= numlumps", lump);
	}

	if(!lumpcache[lump])
	{
	 // Need to read the lump in
//		ptr = Z_Malloc(W_LumpLength(lump)+24, tag, &lumpcache[lump]);
		ptr = Z_Malloc(W_LumpLength(lump)+32, tag, &lumpcache[lump]);
		W_ReadLump(lump, lumpcache[lump]);

		Z_CheckIntact (ptr);

//		printf("W_CacheLumpNum: %d %s\n", lump, W_GetNameForNum(lump));
	}
	else
	{
		Z_ChangeTag(lumpcache[lump], tag);
	}
	return lumpcache[lump];
}

//==========================================================================
//
// W_CacheLumpName
//
//==========================================================================

void *W_CacheLumpName(char *name, int tag)
{
	return W_CacheLumpNum(W_GetNumForName(name), tag);
}

//==========================================================================
//
// W_Profile
//
//==========================================================================

// Ripped out for Heretic
/*
int	info[2500][10];
int	profilecount;

void W_Profile (void)
{
	int		i;
	memblock_t	*block;
	void	*ptr;
	char	ch;
	FILE	*f;
	int		j;
	char	name[9];
	
	
	for (i=0 ; i<numlumps ; i++)
	{	
		ptr = lumpcache[i];
		if (!ptr)
		{
			ch = ' ';
			continue;
		}
		else
		{
			block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));
			if (block->tag < PU_PURGELEVEL)
				ch = 'S';
			else
				ch = 'P';
		}
		info[i][profilecount] = ch;
	}
	profilecount++;
	
	f = fopen ("waddump.txt","w");
	name[8] = 0;
	for (i=0 ; i<numlumps ; i++)
	{
		memcpy (name,lumpinfo[i].name,8);
		for (j=0 ; j<8 ; j++)
			if (!name[j])
				break;
		for ( ; j<8 ; j++)
			name[j] = ' ';
		fprintf (f,"%s ",name);
		for (j=0 ; j<profilecount ; j++)
			fprintf (f,"    %c",info[i][j]);
		fprintf (f,"\n");
	}
	fclose (f);
}
*/
