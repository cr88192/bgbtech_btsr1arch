// Emacs style mode select	 -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Handles WAD file header, directory, lump I/O.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: w_wad.c,v 1.5 1997/02/03 16:47:57 b1 Exp $";


#include <ctype.h>
// #include <sys/types.h>
#include <string.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <malloc.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <alloca.h>

#include "doomtype.h"
#include "m_swap.h"
#include "i_system.h"
#include "z_zone.h"

#ifdef __GNUG__
#pragma implementation "w_wad.h"
#endif
#include "w_wad.h"






//
// GLOBALS
//

// Location of each lump on disk.
lumpinfo_t*		lumpinfo;		
int			numlumps;

int			lumphash[64];

void**			lumpcache;


// #define strcmpi	strcasecmp
 #define strcmpi	stricmp

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

// int filelength (int handle) 
// { 
//	struct stat	fileinfo;
//	if (fstat (handle,&fileinfo) == -1)
//	I_Error ("Error fstating");
//	return fileinfo.st_size;
// }


void
ExtractFileBase
( char*		path,
	char*		dest )
{
	char*	src;
	int		length;

	src = path + strlen(path) - 1;
	
	// back up until a \ or the start
	while (src != path
		 && *(src-1) != '\\'
		 && *(src-1) != '/')
	{
		src--;
	}
	
	// copy up to eight characters
	memset (dest,0,8);
	length = 0;
	
	while (*src && *src != '.')
	{
		if (++length == 9)
			I_Error ("Filename base of %s >8 chars",path);

		*dest++ = toupper((int)*src++);
	}
}





//
// LUMP BASED ROUTINES.
//

//
// W_AddFile
// All files are optional, but at least one file must be
//	found (PWAD, if all required lumps are present).
// Files with a .wad extension are wadlink files
//	with multiple lumps.
// Other files are single lumps with the base filename
//	for the lump name.
//
// If filename starts with a tilde, the file is handled
//	specially to allow map reloads.
// But: the reload feature is a fragile hack...

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

void W_AddFile (char *filename)
{
	wadinfo_t		header;
	lumpinfo_t*		lump_p;
	unsigned		i;
	int			handle;
	int			length;
	int			startlump;
	filelump_t*		fileinfo;
	filelump_t		singleinfo;
	int			storehandle;
	
	// open the file and add to directory

	// handle reload indicator.
	if (filename[0] == '~')
	{
		filename++;
		reloadname = filename;
		reloadlump = numlumps;
	}
		
//	if ( (handle = open (filename,O_RDONLY | O_BINARY)) == -1)
	if ( (handle = w_open (filename, "rb")) == -1)
	{
		printf (" couldn't open %s\n",filename);
		return;
	}

	printf (" adding %s\n",filename);
	startlump = numlumps;
	
	if (strcmpi (filename+strlen(filename)-3 , "wad" ) )
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
		if (strncmp(header.identification,"IWAD",4))
		{
			// Homebrew levels?
			if (strncmp(header.identification,"PWAD",4))
			{
			I_Error ("Wad file %s doesn't have IWAD "
				 "or PWAD id\n", filename);
			}
			
			// ???modifiedgame = true;		
		}
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps*sizeof(filelump_t);
//		fileinfo = alloca (length);
		fileinfo = malloc (length);
		w_lseek (handle, header.infotableofs, SEEK_SET);
		w_read (handle, fileinfo, length);
		numlumps += header.numlumps;
	}

	
	// Fill in lumpinfo
	lumpinfo = realloc (lumpinfo, numlumps*sizeof(lumpinfo_t));

	if (!lumpinfo)
		I_Error ("Couldn't realloc lumpinfo");

	lump_p = &lumpinfo[startlump];
	
	storehandle = reloadname ? -1 : handle;
	
	for (i=startlump ; i<numlumps ; i++,lump_p++, fileinfo++)
	{
		lump_p->handle = storehandle;
		lump_p->position = LONG(fileinfo->filepos);
		lump_p->size = LONG(fileinfo->size);
		strncpy (lump_p->name, fileinfo->name, 8);
	}
	
	if (reloadname)
		w_close (handle);
}




//
// W_Reload
// Flushes any of the reloadable lumps in memory
//	and reloads the directory.
//
void W_Reload (void)
{
	wadinfo_t		header;
	int			lumpcount;
	lumpinfo_t*		lump_p;
	unsigned		i;
	int			handle;
	int			length;
	filelump_t*		fileinfo;
	
	if (!reloadname)
		return;
		
//	if ( (handle = open (reloadname,O_RDONLY | O_BINARY)) == -1)
	if ( (handle = w_open (reloadname, "rb")) == -1)
		I_Error ("W_Reload: couldn't open %s",reloadname);

	w_read (handle, &header, sizeof(header));
	lumpcount = LONG(header.numlumps);
	header.infotableofs = LONG(header.infotableofs);
	length = lumpcount*sizeof(filelump_t);
//	fileinfo = alloca (length);
	fileinfo = malloc (length);
	w_lseek (handle, header.infotableofs, SEEK_SET);
	w_read (handle, fileinfo, length);
	
	// Fill in lumpinfo
	lump_p = &lumpinfo[reloadlump];
	
	for (i=reloadlump ;
	 i<reloadlump+lumpcount ;
	 i++,lump_p++, fileinfo++)
	{
	if (lumpcache[i])
		Z_Free (lumpcache[i]);

	lump_p->position = LONG(fileinfo->filepos);
	lump_p->size = LONG(fileinfo->size);
	}
	
	w_close (handle);
}

int W_HashIndexForName(char *s)
{
	int j, h;
	j = *(int *)(s);
//	h = ((j*65521)>>16)&63;
	h = ((j*16777213)>>24)&63;
	return(h);
}

//
// W_InitMultipleFiles
// Pass a null terminated list of files to use.
// All files are optional, but at least one file
//	must be found.
// Files with a .wad extension are idlink files
//	with multiple lumps.
// Other files are single lumps with the base filename
//	for the lump name.
// Lump names can appear multiple times.
// The name searcher looks backwards, so a later file
//	does override all earlier ones.
//
void W_InitMultipleFiles (char** filenames)
{	
	lumpinfo_t*		lump_p;
	int		size;
	int		i, j, h;
	
	// open all the files, load headers, and count lumps
	numlumps = 0;

	// will be realloced as lumps are added
	lumpinfo = malloc(1);	

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
	
	// set up caching
//	size = numlumps * sizeof(*lumpcache);
	size = (numlumps + 64) * sizeof(*lumpcache);
	lumpcache = malloc (size);
	
	if (!lumpcache)
		I_Error ("Couldn't allocate lumpcache");

	memset (lumpcache,0, size);
}




//
// W_InitFile
// Just initialize from a single file.
//
void W_InitFile (char* filename)
{
	char*	names[2];

	names[0] = filename;
	names[1] = NULL;
	W_InitMultipleFiles (names);
}



//
// W_NumLumps
//
int W_NumLumps (void)
{
	return numlumps;
}



//
// W_CheckNumForName
// Returns -1 if name not found.
//

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
	strncpy (name8.s,name,8);

	// in case the name was a fill 8 chars
	name8.s[8] = 0;

	// case insensitive
	w_strupr (name8.s);		

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
#if 0
		if(i >= base)
		{
			i = lump_p->chain;
			continue;
		}
#endif
		
		if ( *(int *)lump_p->name == v1
			 && *(int *)&lump_p->name[4] == v2)
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




//
// W_GetNumForName
// Calls W_CheckNumForName, but bombs out if not found.
//
int W_GetNumForName (char* name)
{
	int	i;

//	if(!name || !*name || (name[0]<' '))
	if(!name || (*name<' '))
		__debugbreak();

	i = W_CheckNumForName (name);
	
	if (i == -1)
	{
//		I_Error ("W_GetNumForName: %s not found!", name);
		printf("W_GetNumForName: %s not found!\n", name);
//		__debugbreak();
		return(-1);
	}
		
	return i;
}

int W_GetNumForNameBase (int base, char *name)
{
	int	i;

	i = W_CheckNumForNameBase (base, name);
	
	if (i == -1)
	{
//		I_Error ("W_GetNumForName: %s not found!", name);
//		printf("W_GetNumForNameBase: %s not found!\n", name);
		return(-1);
	}
		
	return i;
}


//
// W_LumpLength
// Returns the buffer size needed to load the given lump.
//
int W_LumpLength (int lump)
{
	if(lump<0)
		__debugbreak();

	if (lump >= numlumps)
		I_Error ("W_LumpLength: %i >= numlumps",lump);

	return lumpinfo[lump].size;
}


// #define W_READLUMP_CHUNKSZ	4096

//
// W_ReadLump
// Loads the lump into the given buffer,
//	which must be >= W_LumpLength().
//
void
W_ReadLump
( int		lump,
	void*		dest )
{
	static void *tbuf=NULL;
	byte *ct;
	int n;
	
	int		c;
	lumpinfo_t*	l;
	int		handle;
	
	if (lump >= numlumps)
	I_Error ("W_ReadLump: %i >= numlumps",lump);

	l = lumpinfo+lump;
	
	// ??? I_BeginRead ();
	
	if (l->handle == -1)
	{
	// reloadable file, so use open / read / close
//		if ( (handle = open (reloadname,O_RDONLY | O_BINARY)) == -1)
		if ( (handle = w_open (reloadname, "rb")) == -1)
			I_Error ("W_ReadLump: couldn't open %s",reloadname);
	}
	else
		handle = l->handle;
		
	w_lseek (handle, l->position, SEEK_SET);

// #if 0
#ifndef W_READLUMP_CHUNKSZ
	c = w_read (handle, dest, l->size);
//	if (c < l->size)
	if (c != l->size)
		I_Error ("W_ReadLump: only read %i of %i on lump %i",
			 c,l->size,lump);	
#endif

// #if 1
#ifdef W_READLUMP_CHUNKSZ
	//BGB debug
	if(!tbuf)
	{
		tbuf=malloc(W_READLUMP_CHUNKSZ+256);
	}
	
	ct=dest; n=l->size;	
	while(n>W_READLUMP_CHUNKSZ)
	{
		w_read (handle, tbuf, W_READLUMP_CHUNKSZ);
		memcpy(ct, tbuf, W_READLUMP_CHUNKSZ);
		ct+=W_READLUMP_CHUNKSZ;
		n-=W_READLUMP_CHUNKSZ;
	}

	w_read (handle, tbuf, n);
	memcpy(ct, tbuf, n);
#endif

	if (l->handle == -1)
		w_close (handle);
		
	// ??? I_EndRead ();
}




//
// W_CacheLumpNum
//
void*
W_CacheLumpNum
( int		lump,
	int		tag )
{
	byte*	ptr;
	int		sz;

	if(lump<0)
		return(NULL);

	if(tag == PU_CACHE)		//BGB
		tag = PU_CACHELUMP;

	if ((unsigned)lump >= numlumps)
	I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
		
	if (!lumpcache[lump])
	{
		// read the lump in
		
		//printf ("cache miss on lump %i\n",lump);
		sz = W_LumpLength (lump);
		ptr = Z_Malloc (sz+256, tag, &lumpcache[lump]);
		memset(ptr, 0, sz);
		
		W_ReadLump (lump, lumpcache[lump]);
	}
	else
	{
		//printf ("cache hit on lump %i\n",lump);
		Z_ChangeTag (lumpcache[lump],tag);
	}
	
	return lumpcache[lump];
}

int W_GetNumForCache (void *ptr)
{
	int i;
	
	for(i=0; i<numlumps; i++)
	{
		if (lumpcache[i]==ptr)
			return(i);
	}
	return(-1);
}


//
// W_CacheLumpName
//
void*
W_CacheLumpName
( char*		name,
	int		tag )
{
	return W_CacheLumpNum (W_GetNumForName(name), tag);
}

//
// W_CacheLumpName
//
void*
W_CacheLumpNameBase
(
	int		base,
	char	*name,
	int		tag )
{
	return W_CacheLumpNum (W_GetNumForNameBase(base, name), tag);
}


//
// W_Profile
//
int		info[2500][10];
int		profilecount;

void W_Profile (void)
{
// #ifndef __BGBCC		//BGB: Don't bother in this case
#if 0
	int		i;
	memblock_t*	block;
	void*	ptr;
	char	ch;
	FILE*	f;
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
		fprintf (f,"	%c",info[i][j]);

	fprintf (f,"\n");
	}
	fclose (f);
#endif
}


