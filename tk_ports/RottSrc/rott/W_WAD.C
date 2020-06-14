/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// W_wad.c

#include <stdio.h>
// #include <conio.h>
#include <stdlib.h>
#include <string.h>
// #include <malloc.h>
// #include <io.h>
// #include <fcntl.h>
// #include <sys/stat.h>
#include "rt_def.h"
#include "rt_util.h"
#include "_w_wad.h"
#include "w_wad.h"
#include "z_zone.h"
#include "isr.h"
#include "develop.h"

#include "rt_crc.h"
#include "rt_main.h"


//=============
// GLOBALS
//=============

int			numlumps;
void			**lumpcache;

int			lumphash[64];


//=============
// STATICS
//=============

static lumpinfo_t	*lumpinfo;			// location of each lump on disk


#if (DATACORRUPTIONTEST == 1)
static byte *lumpcheck;
#endif


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

// int filelength (int handle)
{
	struct stat fileinfo;

	if (fstat (handle,&fileinfo) == -1)
		Error ("Error fstating");

	return fileinfo.st_size;
}

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

FILE *w_openfiles[32];
unsigned int w_m_openfiles;

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
	char				tb[16];
	wadinfo_t			header;
	lumpinfo_t			*lump_p;
	unsigned			i;
	int					handle, length;
	int					startlump;
	filelump_t			*fileinfo, singleinfo;
	wad2lump_t			*wad2info;
	FILE				*tfd;

//
// read the entire file in
//	FIXME: shared opens

//		if ( (handle = open (filename,O_RDWR | O_BINARY)) == -1)
	if ( (handle = w_open (filename, "rb")) == -1)
		return;

	startlump = numlumps;

	wad2info=NULL;
	fileinfo=NULL;

	if ( (strcmpi (filename+strlen(filename)-3 , "wad" ) ) &&
		(strcmpi (filename+strlen(filename)-3 , "rts" ) ) )
	{
	// single lump file
		if (!quiet)
			printf("	Adding single file %s.\n",filename);
		fileinfo = &singleinfo;
		singleinfo.filepos = 0;
		singleinfo.size = LONG(w_filelength(handle));
		ExtractFileBase (filename, singleinfo.name);
		numlumps++;
	}
	else
	{
	// WAD file
		if (!quiet)
			printf("	Adding %s.\n",filename);

#if 0
		w_read (handle, &header, sizeof(header));
		if (strncmp(header.identification,"IWAD",4) &&
			strncmp(header.identification,"PWAD",4))
			Error ("Wad file %s doesn't have IWAD or PWAD id\n",filename);
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps*sizeof(filelump_t);
//		fileinfo = alloca (length);
		fileinfo = malloc (length);
		if (!fileinfo)
		Error ("Wad file could not allocate header info on stack");
		w_lseek (handle, header.infotableofs, SEEK_SET);
		w_read (handle, fileinfo, length);
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
//			freeFileInfo = fileinfo;
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
//			freeFileInfo = (filelump_t *)wad2info;
			w_lseek (handle, header.infotableofs, SEEK_SET);
			w_read (handle, wad2info, length);
			numlumps += header.numlumps;
		}else
		{
			Error ("Wad file %s doesn't have "
				"IWAD, PWAD, or WAD2 id\n", filename);
		}
#endif

	}

//
// Fill in lumpinfo
//
	Z_Realloc(&lumpinfo,numlumps*sizeof(lumpinfo_t));
//		lumpinfo = realloc (lumpinfo, numlumps*sizeof(lumpinfo_t));
//		if (!lumpinfo)
//		Error("W_AddFile: Could not realloc %ld bytes",numlumps*sizeof(lumpinfo_t));
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
	//		strncpy (lump_p->name, fileinfo->name, 8);
			w_strupr_n (lump_p->name, fileinfo->name, 8);
		}
	}

#if 0	//BGB: dump WAD list
	lump_p = &lumpinfo[startlump];
	tfd=fopen("t_wadlist.txt", "wt");
	if(tfd)
	{
		for (i=startlump ; i<numlumps ; i++,lump_p++)
		{
			strncpy (tb, lump_p->name, 8);
			tb[8]=0;
			fprintf(tfd, "\"%s\",\n", tb);
		}
		fclose(tfd);
	}
#endif
}



/*
====================
=
= W_CheckWADIntegrity
=
====================
*/

void W_CheckWADIntegrity ( void )
{
   int crc;

   crc = CalculateCRC ((byte *)lumpinfo, numlumps*sizeof(lumpinfo_t) );
   if (crc != WADCHECKSUM)
	{
	printf("==============================================================================\n");
	printf("ATTENTION: This version of ROTT has been modified.  If you would like to get\n");
	printf("a copy of the original game, call 1-800-APOGEE1 or run ROTTHELP.EXE.\n");
	printf("	You will not receive technical support for this modified version.\n");
	printf("						Press any key to continue\n");
	printf("==============================================================================\n");
//	printf("crc=%ld\n",crc);
	getchar();
	}
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

int W_HashIndexForName(char *s)
{
	int j, h;
	j = *(int *)(s);
//	h = ((j*65521)>>16)&63;
	h = ((j*16777213)>>24)&63;
	return(h);
}

void W_InitMultipleFiles (char **filenames)
{
	lumpinfo_t*		lump_p;
	int i, j, k, h;
//
// open all the files, load headers, and count lumps
//
	numlumps = 0;
	lumpinfo = SafeMalloc(5);   // will be realloced as lumps are added

	for ( ; *filenames ; filenames++)
		W_AddFile (*filenames);

	if (!numlumps)
		Error ("W_InitFiles: no files found");

	for(i=0; i<64; i++)
	{
		lumphash[i] = -1;
	}
	
//	for(i=0; i<numlumps; i++)
	for(i=numlumps-1; i>=0; i--)
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
	lumpcache = calloc (numlumps, sizeof(*lumpcache));
	if (!lumpcache)
		Error("W_InitFiles: lumpcache malloc failed size=%ld\n",numlumps<<2);

	if (!quiet)
		printf("W_Wad: Wad Manager Started NUMLUMPS=%ld\n",numlumps);

#if (DATACORRUPTIONTEST == 1)
	lumpcheck=SafeMalloc(numlumps);
	memset(lumpcheck,255,numlumps);
#endif
//	if (!SOUNDSETUP)
//		W_CheckWADIntegrity ();
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

#if 0
int	W_CheckNumForName (char *name)
{
		char	name8[9];
		int			v1,v2;
		lumpinfo_t	*lump_p;
		lumpinfo_t	*endlump;

// make the name into two integers for easy compares

		strncpy (name8,name,8);
		name8[8] = 0;				// in case the name was a fill 8 chars
		strupr (name8);				// case insensitive

		v1 = *(int *)name8;
		v2 = *(int *)&name8[4];

// scan backwards so patch lump files take precedence

		lump_p = lumpinfo;
		endlump = lumpinfo + numlumps;

		while (lump_p != endlump)
		{
			if ( *(int *)lump_p->name == v1 && *(int *)&lump_p->name[4] == v2)
				return lump_p - lumpinfo;
			lump_p++;
		}

		return -1;
}
#endif

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
//	strncpy (name8.s,name,8);
	w_strupr_n(name8.s, name, 8);

	// in case the name was a fill 8 chars
//	name8.s[8] = 0;

	// case insensitive
//	w_strupr (name8.s);		

	v1 = name8.x[0];
	v2 = name8.x[1];

#if 1
	h = W_HashIndexForName(name8.s);
//	h=((v1*65521)>>16)&63;
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
//		if(i >= numlumps)
//			__debugbreak();
	
		lump_p = lumpinfo + i;
		
		if (	(*(int *)(&lump_p->name[0]) == v1)	&&
				(*(int *)(&lump_p->name[4]) == v2)	)
		{
//			return(i);
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

		Error ("W_GetNumForName: %s not found!",name);
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
				Error ("W_LumpLength: %i >= numlumps",lump);
		return lumpinfo[lump].size;
}

/*
====================
=
= W_GetNameForNum
=
====================
*/

char *  W_GetNameForNum (int i)
{

		if (i>=numlumps)
		Error ("W_GetNameForNum: %i >= numlumps",i);
		return &(lumpinfo[i].name[0]);
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

/*
====================
=
= W_ReadLump
=
= Loads the lump into the given buffer, which must be >= W_LumpLength()
=
====================
*/
int readinglump;
byte * lumpdest;
void W_ReadLump (int lump, void *dest)
{
	static byte	*tcbuf=NULL;
	static int		tcsize;
	int				c, csz;
	lumpinfo_t		*l;

	readinglump=lump;
	lumpdest=dest;
	if (lump >= numlumps)
			Error ("W_ReadLump: %i >= numlumps",lump);
	if (lump < 0)
			Error ("W_ReadLump: %i < 0",lump);
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
			Error ("W_ReadLump: only read %i of %i on lump %i",
				c,l->csize,lump);
		if(l->cmp==3)
			{ W_DecodeBufferRP2(tcbuf, dest, l->csize, l->size); }
		else if(l->cmp==4)
			{ W_DecodeBufferLZ4(tcbuf, dest, l->csize, l->size); }
		
		return;
	}

	w_lseek (l->handle, l->position, SEEK_SET);
	c = w_read (l->handle, dest, l->size);
	if (c < l->size)
		Error ("W_ReadLump: only read %i of %i on lump %i",
			c,l->size,lump);
}


/*
====================
=
= W_WriteLump
=
= Writes the lump into the given buffer, which must be >= W_LumpLength()
=
====================
*/

void W_WriteLump (int lump, void *src)
{
   int		c;
   lumpinfo_t *l;

   if (lump >= numlumps)
	Error ("W_WriteLump: %i >= numlumps",lump);
   if (lump < 0)
	Error ("W_WriteLump: %i < 0",lump);
   l = lumpinfo+lump;

   w_lseek (l->handle, l->position, SEEK_SET);
   c = w_write (l->handle, src, l->size);
   if (c < l->size)
	Error ("W_WriteLump: only wrote %i of %i on lump %i",c,l->size,lump);
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
		if ((unsigned)lump >= numlumps)
				Error ("W_CacheLumpNum: %i >= numlumps",lump);

		else if (lump < 0)
				Error ("W_CacheLumpNum: %i < 0  Taglevel: %i",lump,tag);

		if (!lumpcache[lump])
		{
				// read the lump in
#if (PRECACHETEST == 1)
				char str[9];
				strncpy(&str[0],W_GetNameForNum(lump),8);
				str[8]=0;
				SoftError("Lump #%d, %s cached at %ld tics size=%ld tag=%ld\n",lump,str,ticcount,W_LumpLength (lump),tag);
				if (W_LumpLength(lump)==0)
				SoftError("Caching in zero length lump #%d, %s cached at %ld tics size=%ld tag=%ld\n",lump,str,ticcount,W_LumpLength (lump),tag);
#endif

#if (DATACORRUPTIONTEST == 1)
				{
				int length;

				*(lumpcheck+lump)=CHECKPERIOD;
				length=W_LumpLength(lump);
				Z_Malloc (length+sizeof(word)+24, tag, &lumpcache[lump]);
				W_ReadLump (lump, lumpcache[lump]);

				*( (word *) ((byte *)lumpcache[lump]+length) ) =
					CalculateCRC (lumpcache[lump], length);
				}
#else
				Z_Malloc (W_LumpLength (lump)+24, tag, &lumpcache[lump]);
				W_ReadLump (lump, lumpcache[lump]);
#endif
		}
		else
		{
#if (DATACORRUPTIONTEST == 1)

			if (*(lumpcheck+lump)==255)
				Error("Tried using lump%ld before reading it in\n",lump);
			(*(lumpcheck+lump))--;
			if (*(lumpcheck+lump)==0)
				{
				word storedcrc;
				word crc;
				int length;

				*(lumpcheck+lump)=CHECKPERIOD;

				length=W_LumpLength(lump);
				storedcrc = *( (word *) ((byte *)lumpcache[lump]+length) );
				crc = CalculateCRC (lumpcache[lump], length);
				if (crc!=storedcrc)
					Error("Data corruption lump=%ld\n",lump);
				}
#endif
			Z_ChangeTag (lumpcache[lump],tag);
		}

		return lumpcache[lump];
}

extern int rott_wallcache[256];
extern int rott_wallstrt;
extern int rott_wallstop;

// extern int rott_sndcache[512];
// extern int rott_sndstrt;
// extern int rott_sndstop;

// int rott_shapecache[4096];
// int rott_shapestrt;
// int rott_shapestop;


void	*W_CacheWallLumpNum (int lump, int tag)
{
//	static int wallcache[256];
	int lump1;

#if 0
	if((lump>rott_wallstrt) && (lump<rott_wallstop))
	{
		lump1=rott_wallcache[lump-rott_wallstrt];
		return(W_CacheLumpNum(lump1, tag));
	}
#endif

#if 0
	if((lump>rott_shapestrt) && (lump<rott_shapestop))
	{
		lump1=rott_shapecache[lump-rott_shapestrt];
		return(W_CacheLumpNum(lump1, tag));
	}
#endif

	return(W_CacheLumpNum(lump, tag));
}

void	*W_CacheSoundLumpNum (int lump, int tag)
{
	return(W_CacheWallLumpNum(lump, tag));
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

int		W_CheckLumpIsNameP(int lump, char *name)
{
	char *s;
	s=W_GetNameForNum(lump&0x0FFF);
	if(s && !strcmp(s, name))
		return(true);
	return(false);
}
