// #include "../game/q_shared.h"
// #include "qcommon.h"

// #define TGVLZ_NOMAIN
// #include "fs_tgvlz1.c"

#define HAVE_STDINT_H	/* Probably safe to assume at this point... */

#ifdef HAVE_STDINT_H
#include <stdint.h>

typedef uint8_t	byte;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#else

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#endif

#include "fs_w4rp2.c"

#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define	FCC_WAD4	BGBCC_FOURCC('W', 'A', 'D', '4')

// typedef unsigned short word;
// typedef struct wad4context_s	wad4context_t;

typedef struct
{
	u32		ident;			//00: Header Magic
	u32		numlumps;		//04: Number of lumps in the directory.
	u32		diroffs;		//08: Offset of Directory.
 	u32		hashoffs;		//0C: Offset of Directory Hash.
 	u32		bmpoffs;		//10: Offset of Allocation Bitmap.
 	u32		bmpsize;		//14: Bitmap Size
 	u32		resv4;			//18: Reserved
 	u32		resv5;			//1C: Reserved
} wad4head_t;

typedef struct
{
	u32		foffs;		//00: offset
	u32		csize;		//04: compressed size
	u32		dsize;		//08: uncompressed size
	byte	ety;		//0C: Entry Type
	byte	cmp;		//0D: Compression
	u16		chn;		//0E: Chain (ExWAD)
 	u16		dirid;		//10: Directory ID / Parent Directory (DirEnt Index)
 	u16		dirnext;	//12: Next in Directory (DirEnt Index)
 	u16		uid;		//14: UID
 	u16		gid;		//16: GID
 	u16		mode;		//18: Mode Flags (---Srwxrwxrwx)
 	u16		hibits;		//1A: Some extra bits
 	u32		mtime;		//1C: File timestamp
 	byte	name[32];	//20: name
} wad4lump_t;

struct wad4context_s {
char		*name;
FILE		*fd;
wad4lump_t	*dir;
int			szdir;
int			seq;
wad4head_t	*head;

wad4head_t	t_head;
};

static int fs_wadseq=1;

//Lump Cache, Big Lumps
byte	*fs_biglumpcache_data[8];
int		fs_biglumpcache_lumpid[8];

//Lump Cache, Small Lumps
byte	*fs_lumpcache_data[256];
int		fs_lumpcache_lumpid[256];

//Lump Cache, Fragmented Files
int		*fs_fragcache_data[64];
int		fs_fragcache_cnt[64];
int		fs_fragcache_lumpid[64];

void *FS_W_Malloc(int sz)
{
	return(Z_Malloc(sz));
//	return(Q_Malloc(sz));
//	return(Q_MallocBulk(sz));
}

void FS_W_Free(void *ptr)
{
	Z_Free(ptr);
}

int *FS_Wad4GetFragsForLump(wad4context_t *ctx, int lump, int *rcnt)
{
	wad4lump_t	*de;
	int *dat;
	int	cnt, lid, lid1, ix;

	if((lump<2) || (lump>=ctx->szdir))
		{ __debugbreak(); }

	de=ctx->dir+lump;
	
	if(de->cmp!=1)
		return(NULL);

	lid=(ctx->seq<<24)|lump;
	ix=((lid*251*65521)>>24)&63;

	lid1 = fs_fragcache_lumpid[ix];
	dat = fs_fragcache_data[ix];
	cnt = fs_fragcache_cnt[ix];
	
	if((lid1 == lid) && dat)
	{
		*rcnt = cnt;
		return(dat);
	}
	
	if(!dat)
	{
		dat = FS_W_Malloc(128*sizeof(int));
		fs_fragcache_data[ix] = dat;
	}

	cnt = de->csize / 4;
	
	if(cnt>128)
		return(NULL);
	
	fs_fragcache_lumpid[ix] = lid;
	fs_fragcache_cnt[ix] = cnt;

	memset(dat, 0, 64*sizeof(int));

	fseek(ctx->fd, de->foffs*64, SEEK_SET);
	fread(dat, 1, de->csize, ctx->fd);

	*rcnt = cnt;
	return(dat);
}

byte *FS_Wad4GetCacheForLump(wad4context_t *ctx, int lump, int *rsz)
{
	static byte	*tmpbuf=NULL;

	wad4lump_t	*de;
	byte *ptr;
	int i, j, k, lid, lid1, ix;
	
	if(lump<2)
		return(NULL);
	if(lump>=ctx->szdir)
		return(NULL);
	
	lid=(ctx->seq<<24)|lump;
	
	de=ctx->dir+lump;
	
	if(de->cmp==1)
		return(NULL);

	if(de->cmp==2)
	{
		ix=((de->foffs)>>16)&65535;
		k=(de->foffs)&65535;
		ptr=FS_Wad4GetCacheForLump(ctx, ix, &j);
		if(!ptr)
			return(ptr);
		*rsz=de->dsize;
		ptr+=k*64;
		return(ptr);
	}
	
	if(de->dsize>256)
	{
		ix=((lid*(251*65521))>>24)&7;
		ptr=fs_biglumpcache_data[ix];
		lid1=fs_biglumpcache_lumpid[ix];
		
		if(!ptr)
		{
			ptr=FS_W_Malloc((1<<17)+64);
			lid1=-1;
			fs_biglumpcache_data[ix]=ptr;
		}
		
		if(lid==lid1)
		{
			*rsz=de->dsize;
			return(ptr);
		}

		fs_biglumpcache_lumpid[ix]=lid;
	}else
	{
		ix=((lid*251*65521)>>24)&255;
		ptr=fs_lumpcache_data[ix];
		lid1=fs_lumpcache_lumpid[ix];
		
		if(!ptr)
		{
			ptr=FS_W_Malloc(256+32);
			lid1=-1;
			fs_lumpcache_data[ix]=ptr;
		}
		
		if(lid==lid1)
		{
			*rsz=de->dsize;
			return(ptr);
		}

		fs_lumpcache_lumpid[ix]=lid;
	}
	
	if(!tmpbuf)
		tmpbuf=FS_W_Malloc((1<<17)+256);
	
//	if(!ctx->fd)
//		ctx->fd=fopen(ctx->name, "rb");
	
	if(de->cmp>=3)
	{
		k=fseek(ctx->fd, de->foffs*64, SEEK_SET);
		if(k<0)
		{
			__debugbreak();
		}

		k=fread(tmpbuf, 1, de->csize, ctx->fd);

		if(k!=de->csize)
		{
			__debugbreak();
		}
	}
	
	if(de->cmp==3)
	{
		W_DecodeBufferRP2(tmpbuf, ptr, de->csize, de->dsize);
	}else
		if((de->cmp==4) || (de->cmp==6))
	{
		W_DecodeBufferLZ4(tmpbuf, ptr, de->csize, de->dsize);
	}else if(de->cmp<3)
	{
		fseek(ctx->fd, de->foffs*64, SEEK_SET);
		fread(ptr, 1, de->csize, ctx->fd);
	}

	*rsz=de->dsize;
	return(ptr);
}

int FS_Wad4PathForLump(wad4context_t *ctx, int lump, char *path)
{
	wad4lump_t	*de;
	int pfx;

	if(lump<0)
	{
		strcpy(path, "");
		return(0);
	}

	if((lump<2) || (lump>=ctx->szdir))
		{ __debugbreak(); }

	de=ctx->dir+lump;
	pfx=de->dirid;
	if(pfx>0)
	{
		FS_Wad4PathForLump(ctx, de->dirid, path);
		strcat(path, "/");
		strcat(path, de->name);
	}else
	{
		strcpy(path, de->name);
	}
	return(0);
}

int FS_GetWad4LumpSize(wad4context_t *ctx, int lump)
{
	if((lump<2) || (lump>=ctx->szdir))
		{ __debugbreak(); }

	return(ctx->dir[lump].dsize);
}

int FS_Wad4ReadFromLump(wad4context_t *ctx,
	byte *buf, int offs, int size, int lump)
{
	wad4lump_t	*de;
	byte *ptr;
	int *fdata;
	int fcnt, fidx, foffs, fsz, remsz, psz;
	int i, j, k;
	
	if((lump<2) || (lump>=ctx->szdir))
		{ __debugbreak(); }
	
	de=ctx->dir+lump;
	
	if(de->cmp==1)
	{
		fdata = FS_Wad4GetFragsForLump(ctx, lump, &fcnt);
		fidx = offs >> 17;
		foffs = offs & 131071;

		ptr = FS_Wad4GetCacheForLump(ctx, fdata[fidx], &psz);
		if(!ptr)
			return(0);

		if((foffs+size)<=131072)
		{
			memcpy(buf, ptr+foffs, size);
			return(size);
		}

		remsz=size;
		fsz=131072-foffs;
		memcpy(buf, ptr+foffs, fsz);
		buf += fsz;
		remsz -= fsz;
		fidx++;
		
		while(remsz>=131072)
		{
			ptr = FS_Wad4GetCacheForLump(ctx, fdata[fidx], &psz);
			if(!ptr)
				return(0);
			memcpy(buf, ptr, 131072);

			buf += 131072;
			remsz -= 131072;
			fidx++;
		}

		if(remsz>0)
		{
			ptr = FS_Wad4GetCacheForLump(ctx, fdata[fidx], &psz);
			if(!ptr)
				return(0);
			memcpy(buf, ptr, remsz);
		}

		return(size);
	}

	ptr = FS_Wad4GetCacheForLump(ctx, lump, &psz);
	if(!ptr)
		return(0);
	
	fsz=size;
	if((offs+size)>psz)
	{
		fsz=psz-offs;
		if(fsz<=0)
			return(0);
	}

	memcpy(buf, ptr+offs, fsz);
	return(fsz);
}

char *NameForEty(int ty)
{
	char *s0;
	switch(ty)
	{
	case 0: s0="Fre"; break;
	case 1: s0="Reg"; break;
	case 2: s0="Dir"; break;
	case 3: s0="Lnk"; break;
	}
	return(s0);
}

char *NameForCmp(int ty)
{
	char *s0;
	switch(ty)
	{
	case 0: s0="Sto";		break;
	case 1: s0="Frg";		break;
	case 2: s0="Pck";		break;
	case 3: s0="RP2";		break;
	case 4: s0="LZ4";		break;
	}
	return(s0);
}

pack_t *FS_LoadWad4File( char *wadfile, const char *basename )
{
	char			tb[512];
	wad4head_t		head;
	FILE			*wadfd;
	wad4context_t	*wctx;
	wad4lump_t		*de;
	fileInPack_t	*buildBuffer, *buildEnd, *bde;
	pack_t			*pack;
	char			*namePtr, *nameBuf, *nameBufEnd;
	int				i, j, k, l, nl, len, nl2, hash;
	
	wadfd=fopen(wadfile, "rb");
	if(!wadfd)
		return(NULL);
	
	fread(&head, 1, sizeof(head), wadfd);
	
	if(head.ident!=FCC_WAD4)
	{
		fclose(wadfd);
		return(NULL);
	}
	
//	wctx = FS_W_Malloc( sizeof(wad4context_t) + sizeof(wad4head_t));
//	wctx->head = (wad4head_t *)(wctx+1);
	wctx = FS_W_Malloc( sizeof(wad4context_t) );
//	wctx->head = FS_W_Malloc( sizeof(wad4head_t) );
	wctx->head = &(wctx->t_head);
	memcpy(wctx->head, &head, sizeof(head));
	
	wctx->name = strdup(wadfile);
	
	wctx->fd = wadfd;
	wctx->szdir = wctx->head->numlumps;

	wctx->dir = FS_W_Malloc( wctx->szdir * sizeof( wad4lump_t ) );
	fseek(wadfd, wctx->head->diroffs * 64, SEEK_SET);
	fread(wctx->dir, 1, wctx->szdir * sizeof( wad4lump_t ), wadfd);

//	fseek(wadfd, 0, SEEK_SET);
	
	wctx->seq = fs_wadseq++;
	
	l = 256;
	pack = Z_Malloc( sizeof( pack_t ) + (l * sizeof(fileInPack_t *)) );
	pack->hashTable = (fileInPack_t **) (((char *) pack) + sizeof( pack_t ));

//	pack = FS_W_Malloc( sizeof( pack_t ) );
//	pack = Z_Malloc( sizeof( pack_t ) );
	pack->hashSize = l;
//	pack->hashTable = FS_W_Malloc(l * sizeof(fileInPack_t *));
	memset(pack->hashTable, 0, l * sizeof(fileInPack_t *));

//	for(i = 0; i < pack->hashSize; i++)
//		{ pack->hashTable[i] = NULL; }

	Q_strncpyz( pack->pakFilename, wadfile, sizeof( pack->pakFilename ) );
	Q_strncpyz( pack->pakBasename, basename, sizeof( pack->pakBasename ) );

	l = strlen( pack->pakBasename );
	if ( l > 4 && !Q_stricmp( pack->pakBasename + l - 4, ".wad" ) )
	{
		pack->pakBasename[l - 4] = 0;
	}

	nl = wctx->head->numlumps;
	len = 0;
	nl2 = 0;

	for(i=2; i<nl; i++)
	{
		de=(wctx->dir)+i;
		if(de->name[0]=='$')
			continue;
		if(de->ety!=1)
			continue;
		FS_Wad4PathForLump(wctx, i, tb);
		if(strlen(tb)>256)
			__debugbreak();
		
		len += strlen(tb) + 1;
		nl2++;
	}

	buildBuffer = FS_W_Malloc( (nl2 * sizeof( fileInPack_t )) + len );
	nameBuf = ((char *) buildBuffer) + (nl2 * sizeof( fileInPack_t ));

//	buildBuffer = FS_W_Malloc( (nl2 + 4) * sizeof( fileInPack_t ));
	buildEnd = buildBuffer + nl2;
//	nameBuf = FS_W_Malloc (len + 256);

	namePtr = nameBuf;
	nameBufEnd = nameBuf + len;
	
	pack->wad4 = wctx;
	pack->buildBuffer = buildBuffer;
	pack->numfiles = nl2;


	j=0; bde=buildBuffer;
	for(i=2; i<nl; i++)
	{
		de=(wctx->dir)+i;
		if(de->name[0]=='$')
			continue;
		if(de->ety!=1)
			continue;

		FS_Wad4PathForLump(wctx, i, tb);

		Q_strlwr( tb );
		hash = FS_HashFileName(tb, pack->hashSize);
//		buildBuffer[j].name = namePtr;
//		buildBuffer[j].wadlump = i;
//		strcpy( buildBuffer[j].name, tb );

		bde->wadlump = i;
		bde->name = namePtr;
		strcpy( bde->name, tb );		
		namePtr += strlen(namePtr) + 1;

//		buildBuffer[j].next = pack->hashTable[hash];
//		pack->hashTable[hash] = &buildBuffer[j];

		bde->next = pack->hashTable[hash];
		pack->hashTable[hash] = bde;
		bde++;

//		j++;
	}

	if(bde > buildEnd)
		{ __debugbreak(); }
	
	if(namePtr > nameBufEnd)
		{ __debugbreak(); }

//	fclose(wadfd);
//	wctx->fd=NULL;

#if 0
	fclose(wadfd);

	wadfd=fopen(wadfile, "rb");
	wctx->fd=wadfd;
#endif

	Q_MallocCheckSane();

	return(pack);
}

int FS_DestroyWad4File( pack_t *pack )
{
	FS_W_Free(pack->wad4->dir);
	FS_W_Free(pack->wad4);

	FS_W_Free(pack->buildBuffer);
	Z_Free(pack);
}
