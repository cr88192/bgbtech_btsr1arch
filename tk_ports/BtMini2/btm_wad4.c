/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define	FCC_WAD4	BGBCC_FOURCC('W', 'A', 'D', '4')

typedef struct wad4context_s	wad4context_t;

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
	wad4context_t	*next;

	char			*name;
	FILE			*fd;
	wad4lump_t		*dir;
	int				szdir;
	int				seq;
	wad4head_t		*head;

	wad4head_t		t_head;
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
	return(btm_malloc(sz));
}

void FS_W_Free(void *ptr)
{
	free(ptr);
}

int W_DecodeBufferRP2(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferRP2(ibuf, obuf, ibsz, obsz));
}

int W_DecodeBufferLZ4(
	byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	return(TgvLz_DecodeBufferLZ4(ibuf, obuf, ibsz, obsz));
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
		j=0;
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
		fcnt = 0;
		fdata = FS_Wad4GetFragsForLump(ctx, lump, &fcnt);
		fidx = offs >> 17;
		foffs = offs & 131071;

		psz = 0;
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

wad4context_t *FS_LoadWad4(char *wadfile)
{
//	char			tb[512];
	wad4head_t		head;
	FILE			*wadfd;
	wad4context_t	*wctx;
//	wad4lump_t		*de;
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
	
	wctx = FS_W_Malloc( sizeof(wad4context_t) );
	wctx->head = &(wctx->t_head);
	memcpy(wctx->head, &head, sizeof(head));
	
	wctx->name = strdup(wadfile);
	
	wctx->fd = wadfd;
	wctx->szdir = wctx->head->numlumps;

	wctx->dir = FS_W_Malloc( wctx->szdir * sizeof( wad4lump_t ) );
	fseek(wadfd, wctx->head->diroffs * 64, SEEK_SET);
	fread(wctx->dir, 1, wctx->szdir * sizeof( wad4lump_t ), wadfd);
	
	wctx->seq = fs_wadseq++;
	
	return(wctx);
}

int FS_Wad4LookupLumpForName(wad4context_t *wctx,
	char *name)
{
	char			tb[512];
	wad4lump_t		*de;
	int				nl;
	int				i, j, k;

	nl = wctx->head->numlumps;
	for(i=2; i<nl; i++)
	{
		de=(wctx->dir)+i;
		if(de->name[0]=='$')
			continue;
		if(de->ety!=1)
			continue;
		FS_Wad4PathForLump(wctx, i, tb);
		
		if(!strcmp(tb, name))
			return(i);
	}

	return(0);
}

wad4context_t *fs_wadfiles;

int FS_Init()
{
	char			tb[256];
	wad4context_t	*wctx;
	int				i;

	if(fs_wadfiles)
		return(0);

	for(i=0; i<10; i++)
	{
		sprintf(tb, "resource/wad%u.wad", i);
		wctx=FS_LoadWad4(tb);
		if(!wctx)
			continue;
		wctx->next=fs_wadfiles;
		fs_wadfiles=wctx;
	}
	
	return(1);
}

byte *FS_LoadFile(char *name, int *rsz, int pad)
{
	wad4context_t	*wctx;
	byte			*buf;
	int				sz;
	int				i, j, k;
	
	wctx=fs_wadfiles;
	while(wctx)
	{
		i=FS_Wad4LookupLumpForName(wctx, name);
		if(i>0)
		{
			sz=FS_GetWad4LumpSize(wctx, i);

			if(pad&4)
				{ buf=BTM_AllocTempBuffer(sz+24); }
			else if(pad&2)
				{ buf=BTM_AllocMul64K((sz+65535)>>16); }
			else if(pad&1)
				{ buf=btm_malloc(sz+24); }
			else
				{ buf=btm_malloc(sz); }
			
			FS_Wad4ReadFromLump(wctx, buf, 0, sz, i);

			if(pad&1)
			{
				memset(buf+sz, 0, 16);
			}
			
			*rsz=sz;
			return(buf);
		}

		wctx=wctx->next;
	}
	
	return(NULL);
}
