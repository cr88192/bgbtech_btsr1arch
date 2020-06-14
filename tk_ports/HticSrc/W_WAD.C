// W_wad.c

// #ifdef NeXT
// #include <libc.h>
// #include <ctype.h>

// next doesn't need a binary flag in open call
// #define	O_BINARY	0

// #else

//#include <malloc.h>
//#include <io.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#endif

#include "DoomDef.h"

//===============
//   TYPES
//===============


typedef struct
{
	char		identification[4];		// should be IWAD
	int			numlumps;
	int			infotableofs;
} wadinfo_t;


typedef struct
{
	int			filepos;
	int			size;
	char		name[8];
} filelump_t;

typedef struct
{
	int		filepos;
	int		csize;
	int		dsize;
	byte	ety;		//Entry Type
	byte	cmp;		//Compression
	short	chn;		//Chain (ExWAD)
	char	name[16];
} wad2lump_t;



//=============
// GLOBALS
//=============

lumpinfo_t	*lumpinfo;		// location of each lump on disk
int			numlumps;

int			lumphash[64];

void		**lumpcache;


//===================

// #ifdef NeXT
#if 0

#define strcmpi strcasecmp

void strupr (char *s)
{
    while (*s)
	*s++ = toupper(*s);
}

/*
================
=
= filelength
=
================
*/

#if 0
int filelength (int handle)
{
    struct stat	fileinfo;
    
    if (fstat (handle,&fileinfo) == -1)
		I_Error ("Error fstating");

    return fileinfo.st_size;
}
#endif

#endif

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

int w_chkaccess(char *name)
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

void ExtractFileBase (char *path, char *dest)
{
	char	*src;
	int		length;

	src = path + strlen(path) - 1;

//
// back up until a \ or the start
//
	while (src != path && *(src-1) != '\\' && *(src-1) != '/')
		src--;

//
// copy up to eight characters
//
	memset (dest,0,8);
	length = 0;
	while (*src && *src != '.')
	{
		if (++length == 9)
			I_Error ("Filename base of %s >8 chars",path);
		*dest++ = toupper((int)*src++);
	}
}


int			reloadlump;
char*		reloadname;

FILE *w_openfiles[32];
u32 w_m_openfiles;

int w_open(char *name, char *mode)
{
	FILE *fd;
	int i;
	
	fd=fopen(name, mode);
	if(!fd)
		return(-1);
		
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
		__debugbreak();
//		return(-1);
	if(hdl>=32)
		__debugbreak();
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		__debugbreak();
//		return(-1);
	
	i=fread(buf, 1, sz, fd);
	return(i);
}

int w_write(int hdl, void *buf, int sz)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		__debugbreak();
//		return(-1);
	if(hdl>=32)
		__debugbreak();
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		__debugbreak();
//		return(-1);
	
	i=fwrite(buf, 1, sz, fd);
	return(i);
}

int w_lseek(int hdl, int ofs, int set)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		__debugbreak();
//		return(-1);
	if(hdl>=32)
		__debugbreak();
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		__debugbreak();
//		return(-1);
	
	i=fseek(fd, ofs, set);
	return(i);
}

int w_close(int hdl)
{
	FILE *fd;
	int i;
	
	if(hdl<0)
		__debugbreak();
//		return(-1);
	if(hdl>=32)
		__debugbreak();
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		__debugbreak();
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
		__debugbreak();
//		return(-1);
	if(hdl>=32)
		__debugbreak();
//		return(-1);
	
	fd=w_openfiles[hdl];
	if(!fd)
		__debugbreak();
//		return(-1);
	
	fseek(fd, 0, 2);
	i=ftell(fd);
	fseek(fd, 0, 0);
	return(i);
}


/*
============================================================================

						LUMP BASED ROUTINES

============================================================================
*/

/*
====================
=
= W_AddFile
=
= All files are optional, but at least one file must be found
= Files with a .wad extension are wadlink files with multiple lumps
= Other files are single lumps with the base filename for the lump name
=
====================
*/

void W_AddFile (char *filename)
{
	wadinfo_t		header;
	lumpinfo_t		*lump_p;
	unsigned		i;
	int				handle, length;
	int				startlump;
	filelump_t		*fileinfo, singleinfo;
	wad2lump_t		*wad2info;
	
//
// open the file and add to directory
//	
//	if ( (handle = open (filename,O_RDONLY | O_BINARY)) == -1)
//	if ( (handle = w_open (filename, "rb")) == -1)
	if ( (handle = w_open (filename, "rb")) < 0)
		return;

	startlump = numlumps;
	
	fileinfo = NULL;
	wad2info = NULL;
	
	if (stricmp (filename+strlen(filename)-3 , "wad" ) )
	{
	// single lump file
		fileinfo = &singleinfo;
		singleinfo.filepos = 0;
		singleinfo.size = LONG(w_filelength(handle));
		ExtractFileBase (filename, singleinfo.name);
		numlumps++;
	}
	else 
	{
	// WAD file
		w_read (handle, &header, sizeof(header));
#if 0
		if (strncmp(header.identification,"IWAD",4))
		{
			if (strncmp(header.identification,"PWAD",4))
				I_Error ("Wad file %s doesn't have IWAD or PWAD id\n"
				,filename);
		}
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps*sizeof(filelump_t);
//		fileinfo = alloca (length);
		fileinfo = malloc (length);
		w_lseek (handle, header.infotableofs, SEEK_SET);
		w_read (handle, fileinfo, length);
		numlumps += header.numlumps;
#endif

		if (!strncmp(header.identification, "IWAD", 4) ||
			!strncmp(header.identification, "PWAD", 4))
		{
			header.numlumps = LONG(header.numlumps);
			header.infotableofs = LONG(header.infotableofs);
			length = header.numlumps*sizeof(filelump_t);
			fileinfo = malloc (length);
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
			w_lseek (handle, header.infotableofs, SEEK_SET);
			w_read (handle, wad2info, length);
			numlumps += header.numlumps;
		}else
		{
			I_Error ("Wad file %s doesn't have "
				"IWAD, PWAD, or WAD2 id\n", filename);
		}
	}

//
// Fill in lumpinfo
//
	lumpinfo = realloc (lumpinfo, numlumps*sizeof(lumpinfo_t));
	if (!lumpinfo)
		I_Error ("Couldn't realloc lumpinfo");
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
		for (i=startlump ; i<numlumps ; i++,lump_p++, fileinfo++)
		{
			memset(lump_p, 0, sizeof(*lump_p));
			lump_p->handle = handle;
			lump_p->position = LONG(fileinfo->filepos);
			lump_p->size = LONG(fileinfo->size);
			lump_p->csize = LONG(fileinfo->size);
			lump_p->ety = 0;
			lump_p->cmp = 0;
//			strncpy (lump_p->name, fileinfo->name, 8);
			w_strupr_n (lump_p->name, fileinfo->name, 8);
		}
	}
}



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
			__debugbreak();
		}

		*(u64 *)ct=*(u64 *)cs;
		cs+=rl;
		ct+=rl;
		W_MatchCopy2(ct, l, d);
		ct+=l;
	}
	
	return(ct-obuf);
}


int W_HashIndexForName(char *s)
{
	int j, h;
	j = *(int *)(s);
//	h = ((j*65521)>>16)&63;
	h = ((j*16777213)>>24)&63;
	return(h);
}

/*
====================
=
= W_InitMultipleFiles
=
= Pass a null terminated list of files to use.
=
= All files are optional, but at least one file must be found
=
= Files with a .wad extension are idlink files with multiple lumps
=
= Other files are single lumps with the base filename for the lump name
=
= Lump names can appear multiple times. The name searcher looks backwards,
= so a later file can override an earlier one.
=
====================
*/

void W_InitMultipleFiles (char **filenames)
{	
	lumpinfo_t*		lump_p;
	int		size;
	int		i, j, h;

//
// open all the files, load headers, and count lumps
//
	numlumps = 0;
	lumpinfo = malloc(1);	// will be realloced as lumps are added

	for ( ; *filenames ; filenames++)
		W_AddFile (*filenames);

	if (!numlumps)
		I_Error ("W_InitFiles: no files found");

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

//
// set up caching
//
	size = numlumps * sizeof(*lumpcache);
	lumpcache = malloc (size);
	if (!lumpcache)
		I_Error ("Couldn't allocate lumpcache");
	memset (lumpcache,0, size);
}



/*
====================
=
= W_InitFile
=
= Just initialize from a single file
=
====================
*/

void W_InitFile (char *filename)
{
	char	*names[2];

	names[0] = filename;
	names[1] = NULL;
	W_InitMultipleFiles (names);
}



/*
====================
=
= W_NumLumps
=
====================
*/

int	W_NumLumps (void)
{
	return numlumps;
}



/*
====================
=
= W_CheckNumForName
=
= Returns -1 if name not found
=
====================
*/

int	W_CheckNumForName (char *name)
{
	char	name8[9];
	int		v1,v2;
	lumpinfo_t	*lump_p;

// make the name into two integers for easy compares

	strncpy (name8,name,8);
	name8[8] = 0;			// in case the name was a fill 8 chars
	strupr (name8);			// case insensitive

	v1 = *(int *)name8;
	v2 = *(int *)&name8[4];


// scan backwards so patch lump files take precedence

	lump_p = lumpinfo + numlumps;

	while (lump_p-- != lumpinfo)
		if ( *(int *)lump_p->name == v1 && *(int *)&lump_p->name[4] == v2)
			return lump_p - lumpinfo;


	return -1;
}


/*
====================
=
= W_GetNumForName
=
= Calls W_CheckNumForName, but bombs out if not found
=
====================
*/

int	W_GetNumForName (char *name)
{
	int	i;

	i = W_CheckNumForName (name);
	if (i != -1)
		return i;

	I_Error ("W_GetNumForName: %s not found!",name);
	return -1;
}


/*
====================
=
= W_LumpLength
=
= Returns the buffer size needed to load the given lump
=
====================
*/

int W_LumpLength (int lump)
{
	if (lump >= numlumps)
		I_Error ("W_LumpLength: %i >= numlumps",lump);
	return lumpinfo[lump].size;
}


/*
====================
=
= W_ReadLump
=
= Loads the lump into the given buffer, which must be >= W_LumpLength()
=
====================
*/

void W_ReadLump (int lump, void *dest)
{
	static byte	*tcbuf;
	static int		tcsize;
	int			c, csz;
	lumpinfo_t	*l;
	
	if (lump >= numlumps)
		I_Error ("W_ReadLump: %i >= numlumps",lump);
	l = lumpinfo+lump;
	
	I_BeginRead ();
	
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
	}else
	{
		w_lseek (l->handle, l->position, SEEK_SET);
		c = w_read (l->handle, dest, l->size);
		if (c < l->size)
			I_Error ("W_ReadLump: only read %i of %i on lump %i",
			c,l->size,lump);
	}
	I_EndRead ();
}



/*
====================
=
= W_CacheLumpNum
=
====================
*/

void	*W_CacheLumpNum (int lump, int tag)
{
	byte *ptr;

	if ((unsigned)lump >= numlumps)
		I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
		
	if (!lumpcache[lump])
	{	// read the lump in
//printf ("cache miss on lump %i\n",lump);
		ptr = Z_Malloc (W_LumpLength (lump)+32, tag, &lumpcache[lump]);
		W_ReadLump (lump, lumpcache[lump]);
	}
	else
	{
//printf ("cache hit on lump %i\n",lump);
		Z_ChangeTag (lumpcache[lump],tag);
	}
	
	return lumpcache[lump];
}


/*
====================
=
= W_CacheLumpName
=
====================
*/

void	*W_CacheLumpName (char *name, int tag)
{
	return W_CacheLumpNum (W_GetNumForName(name), tag);
}


/*
====================
=
= W_Profile
=
====================
*/

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
