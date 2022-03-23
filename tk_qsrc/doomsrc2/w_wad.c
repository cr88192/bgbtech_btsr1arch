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

void		**lumpcache;
void		**patchcache;
void		**utxcache;


// #define strcmpi	strcasecmp
 #define strcmpi	stricmp

void w_strupr (char *s)
{
	while (*s) { *s = toupper(*s); s++; }
}

void w_strupr_n (char *t, const char *s, int n)
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
	wad2lump_t		*wad2info;
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
	
	fileinfo = NULL;
	wad2info = NULL;
	
//	if (strcmpi (filename+strlen(filename)-3 , "wad" ) )
	if (strcmp (filename+strlen(filename)-3 , "wad" ) )
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
#if 0
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

		lump_p = malloc (numlumps*sizeof(lumpinfo_t));
		if(lumpinfo)
		{
			memcpy(lump_p, lumpinfo, startlump*sizeof(lumpinfo_t));
			free(lumpinfo);
		}
		lumpinfo = lump_p;
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
#endif
	}

	
	// Fill in lumpinfo
	lumpinfo = realloc (lumpinfo, numlumps*sizeof(lumpinfo_t));

	if (!lumpinfo)
		I_Error ("Couldn't realloc lumpinfo");

//	lump_p = &lumpinfo[startlump];
	lump_p = lumpinfo + startlump;
	
	storehandle = reloadname ? -1 : handle;
	
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
	//	for (i=startlump ; i<numlumps ; i++,lump_p++, fileinfo++)
		for (i=startlump ; i<numlumps ; i++)
		{
			memset(lump_p, 0, sizeof(*lump_p));
			lump_p->handle = storehandle;
			lump_p->position = LONG(fileinfo->filepos);
			lump_p->size = LONG(fileinfo->size);
			lump_p->csize = LONG(fileinfo->size);
			strncpy (lump_p->name, fileinfo->name, 8);
			lump_p++;
			fileinfo++;
		}
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
	TEST	R22, R22
	BREAK?T

	MOV.Q	(R18), R16
	ADD		R20, R18
	MOV.Q	R16, (R19)
	ADD		R20, R19
	SUB		R19, R22, R6
	ADD		R19, R21, R7
	CMPQGT	15, R22
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
	l = 0;
	d = 0;
	rl = 0;
	
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
			__debugbreak();
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
	
	printf("W_InitFiles: numlumps=%d\n", numlumps);
	
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
//	size = (numlumps + 64) * sizeof(*lumpcache);
	size = (numlumps + 64) * sizeof(void *);
	lumpcache = malloc (size);
	patchcache = malloc (size);
	utxcache = malloc (size);
	
	if (!lumpcache || !patchcache || !utxcache)
		I_Error ("Couldn't allocate lumpcache");

	memset (lumpcache,0, size);
	memset (patchcache,0, size);
	memset (utxcache,0, size);
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

#if 0
int W_CheckNumForNameBase (int base, const char* name)
{
	union {
//	char	s[9];
	char	s[12];
	int	x[2];
	
	} name8;
	
	int		v1;
	int		v2;
	int 	i, j, h, n;
	lumpinfo_t	*lump_p;
	char		*s;

	if(base<=0)
		return(-1);

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
#if 0
		if(i >= base)
		{
			i = lump_p->chain;
			continue;
		}
#endif
		
		s=lump_p->name;
//		if ( *(int *)lump_p->name == v1
//			 && *(int *)&lump_p->name[4] == v2)
//		if (	(*(int *)(&lump_p->name[0]) == v1) &&
//				(*(int *)(&lump_p->name[4]) == v2))
		if (	(*(int *)(s  ) == v1) &&
				(*(int *)(s+4) == v2))
		{
//			return(i);
			break;
		}
//		i = lump_p->chain;
		j = lump_p->chain;
		if(j>=i)
			{ i=-999; break; }
		i = j;
	}
	if(i>=0)
		return(i);
		
#endif

#if 1
	if(i == -999)
	{
		// scan backwards so patch lump files take precedence
	//	lump_p = lumpinfo + numlumps;
		lump_p = lumpinfo + base;

//		while (lump_p-- != lumpinfo)
		while (lump_p-- > lumpinfo)
		{
			if ( *(int *)lump_p->name == v1
				 && *(int *)&lump_p->name[4] == v2)
			{
				return lump_p - lumpinfo;
			}
		}
	}
#endif

	// TFB. Not found.
	return -1;
}
#endif

#if 1
int W_CheckNumForNameBase (int base, const char* name)
{
	union {
//	char	s[9];
	char	s[20];
	u64		x[2];
	
	} name8;
	
	u64		v1;
	u64		v2;
	int 	i, j, k, h, n;
	lumpinfo_t	*lump_p;
	char		*s;

	if(base <= 0)
		return(-1);

	// make the name into two integers for easy compares
//	strncpy (name8.s,name,8);
	w_strupr_n(name8.s, name, 16);

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
		lump_p = lumpinfo + i;
		
		s=lump_p->name;
		if (	(*(u64 *)(s  ) == v1) &&
				(*(u64 *)(s+8) == v2))
//		if (	(*(u64 *)(s  ) == v1))
		{
//			if((*(u64 *)(s+8) != v2))
//			{
//				k=-1;
//			}
		
			break;
		}
		j = lump_p->chain;
		if(j>=i)
			{ i=-999; break; }
		i = j;
	}
	if(i>=0)
		return(i);
		
#endif

#if 1
	if(i == -999)
	{
		// scan backwards so patch lump files take precedence
	//	lump_p = lumpinfo + numlumps;
		lump_p = lumpinfo + base;

//		while (lump_p-- != lumpinfo)
		while (lump_p-- > lumpinfo)
		{
			s=lump_p->name;
			if (	(*(u64 *)(s  ) == v1) &&
					(*(u64 *)(s+8) == v2))
//			if (	*(u64 *)(&lump_p->name[0]) == v1 &&
//					*(u64 *)(&lump_p->name[8]) == v2)
			{
				return lump_p - lumpinfo;
			}
		}
	}
#endif

	// TFB. Not found.
	return -1;
}
#endif

int W_CheckNumForName (const char* name)
{
	return(W_CheckNumForNameBase(numlumps, name));
}


void W_PrintLumps (void);


//
// W_GetNumForName
// Calls W_CheckNumForName, but bombs out if not found.
//
int W_GetNumForName (const char* name)
{
	int	i;

//	if(!name || !*name || (name[0]<' '))
	if(!name || (*name<' '))
		__debugbreak();

	i = W_CheckNumForName (name);
	
//	if (i == -1)
	if(i < 0)
	{
//		I_Error ("W_GetNumForName: %s not found!", name);
		printf("W_GetNumForName: %s not found!\n", name);

//		W_PrintLumps();

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


int W_LumpHandle (int lump)
{
	if(lump<0)
		__debugbreak();

	if (lump >= numlumps)
		I_Error ("W_LumpLength: %i >= numlumps",lump);

	return lumpinfo[lump].handle;
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
	static byte	*tdbuf=NULL;
	static byte	*tcbuf=NULL;
	static int		tcsize;
	static int		tdsize;
	byte *ct;
	int n;
	
	int		c, csz, ofs, ofs1, ofs2;
	lumpinfo_t*	l;
	int		handle;

	if (lump < 0)
		I_Error ("W_ReadLump: %i < 0",lump);
	
	if (lump >= numlumps)
		I_Error ("W_ReadLump: %i >= numlumps (%i)", lump, numlumps);

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

	if(l->cmp)
	{
		if(l->cmp==2)
		{
			ofs = l->position;
			ofs1 = (ofs>>16)&65535;
			ofs2 = (ofs&65535)*64;
			ct = W_CacheLumpNum(ofs1, PU_CACHE);
			
			if(!ct)
				return;
			
			memcpy(dest, ct+ofs2, l->size);
			return;
		}
	
		c = l->size + 256;
		if(!tdbuf || (c > tdsize))
		{
			if(tdbuf)
			{
				csz=tdsize;
				while(c > csz)
					csz=csz+(csz>>1);
				tdbuf=realloc(tdbuf, csz);
				tdsize=csz;
			}else
			{
				csz=131072;
				while(c > csz)
					csz=csz+(csz>>1);
				tdbuf=malloc(csz);
				tdsize=csz;
			}
		}

	
		if(!tcbuf || (l->csize>tcsize))
		{
			if(tcbuf)
			{
//				csz=tcbuf?tcsize:131072;
				csz=tcsize;
				while(l->csize>csz)
					csz=csz+(csz>>1);
				tcbuf=realloc(tcbuf, csz);
				tcsize=csz;
			}else
			{
				csz=131072;
				while(l->csize>csz)
					csz=csz+(csz>>1);
				tcbuf=malloc(csz);
				tcsize=csz;
			}
		}

		w_lseek (l->handle, l->position, SEEK_SET);
		c = w_read (l->handle, tcbuf, l->csize);
		if (c < l->csize)
			I_Error ("W_ReadLump: only read %i of %i on lump %i",
				c,l->csize,lump);
		if(l->cmp==3)
		{
//			W_DecodeBufferRP2(tcbuf, dest, l->csize, l->size);
			W_DecodeBufferRP2(tcbuf, tdbuf, l->csize, l->size);
			memcpy(dest, tdbuf, l->size);
		}
		else if(l->cmp==4)
		{
//			W_DecodeBufferLZ4(tcbuf, dest, l->csize, l->size);
			W_DecodeBufferLZ4(tcbuf, tdbuf, l->csize, l->size);
			memcpy(dest, tdbuf, l->size);
		}
		
		return;
	}

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

	if(lump < 0)
		return(NULL);

	if(tag == PU_CACHE)		//BGB
		tag = PU_CACHELUMP;
//	if(tag == PU_STATIC)		//BGB
//		tag = PU_STATICLUMP;

	if ((unsigned)lump >= numlumps)
		I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
		
	if (!lumpcache[lump])
	{
		// read the lump in
		
		//printf ("cache miss on lump %i\n",lump);
		sz = W_LumpLength (lump);
		ptr = Z_Malloc (sz+256, tag, &lumpcache[lump]);
//		ptr = Z_Malloc (sz+1024, tag, &lumpcache[lump]);
//		memset(ptr, 0, sz+256);
		
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
( const char*		name,
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
// #ifndef __BGBCC__		//BGB: Don't bother in this case
#if 0
	int		i, lhdl;
	memblock_t*	block;
	void*	ptr;
	char	ch;
	FILE*	f;
	int		j;
	int 	xs, ys, xo, yo;
	char	name[9];
	byte	tbuf[256];
	
	
	for (i=0 ; i<numlumps ; i++)
	{	
		ptr = lumpcache[i];
		if (!ptr)
		{
			ch = ' ';
//			continue;
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
//	name[8] = 0;

	lhdl=-1;

	for (i=0 ; i<numlumps ; i++)
	{
		if(lumpinfo[i].handle != lhdl)
		{
			lhdl = lumpinfo[i].handle;
			fprintf (f, "\nHandle=%d\n", lhdl);
			continue;
		}

		memcpy (name,lumpinfo[i].name,8);

		for (j=0 ; j<8 ; j++)
			if (!name[j])
				break;

		for ( ; j<8 ; j++)
			name[j] = ' ';
		name[8] = 0;

		fprintf (f,"%s ",name);

		for (j=0 ; j<profilecount ; j++)
			fprintf (f,"	%c",info[i][j]);

		if(lumpinfo[i].handle < 0)
		{
			fprintf (f,"\n");
			continue;
		}

		fprintf (f,"ofs=%08X, sz=%d; ",
			lumpinfo[i].position, lumpinfo[i].size);

		w_lseek(lumpinfo[i].handle, lumpinfo[i].position, 0);
		w_read(lumpinfo[i].handle, tbuf, 256);
		xs=(short)(tbuf[0] | (tbuf[1]<<8));
		ys=(short)(tbuf[2] | (tbuf[3]<<8));
		xo=(short)(tbuf[4] | (tbuf[5]<<8));
		yo=(short)(tbuf[6] | (tbuf[7]<<8));
		
		if((xs>0) && (xs<1024) && (ys>0) && (ys<1024))
		{
			fprintf (f,"PIC? xs=%d,ys=%d,xo=%d,yo=%d", xs, ys, xo, yo);
			
		}

		fprintf (f,"\n");
	}
	fclose (f);
#endif
}



void W_PrintLumps (void)
{
	int		i;
	memblock_t*	block;
	void*	ptr;
	char	ch;
//	FILE*	f;
	int		j;
	char	name[9];
	
#if 0
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
#endif
	
//	f = fopen ("waddump.txt","w");
//	name[8] = 0;

	for (i=0 ; i<numlumps ; i++)
	{
		memcpy (name,lumpinfo[i].name,8);

		for (j=0 ; j<8 ; j++)
			if (!name[j])
			break;

		for ( ; j<8 ; j++)
			name[j] = ' ';

		printf ("%s ",name);

//		for (j=0 ; j<profilecount ; j++)
//			printf ("	%c",info[i][j]);

		printf ("\n");
	}
//	fclose (f);
}


