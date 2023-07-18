/*
Copyright (C) 1996-1997 Id Software, Inc.

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

// draw.c -- this is the only file outside the refresh that touches the
// vid buffer

#include "quakedef.h"

#define GL_COLOR_INDEX8_EXT	 0x80E5

extern unsigned char d_15to8table[65536];

cvar_t		gl_nobind = {"gl_nobind", "0"};
cvar_t		gl_max_size = {"gl_max_size", "1024"};
cvar_t		gl_picmip = {"gl_picmip", "0"};

byte		*draw_chars;				// 8*8 graphic characters
qpic_t		*draw_disc;
qpic_t		*draw_backtile;

int			translate_texture;
int			char_texture;

typedef struct
{
	int		texnum;
	float	sl, tl, sh, th;
} glpic_t;

byte		conback_buffer[sizeof(qpic_t) + sizeof(glpic_t)];
qpic_t		*conback = (qpic_t *)&conback_buffer;

int		gl_lightmap_format = 4;
int		gl_solid_format = 3;
int		gl_alpha_format = 4;

// int		gl_filter_min = GL_LINEAR_MIPMAP_NEAREST;
int		gl_filter_min = GL_NEAREST_MIPMAP_NEAREST;
//int		gl_filter_max = GL_LINEAR;
int		gl_filter_max = GL_NEAREST;

qboolean	gl_force_square = false;

int		texels;

typedef struct
{
	int		texnum;
	char	identifier[64];
	int		width, height;
	qboolean	mipmap;
} gltexture_t;

#define	MAX_GLTEXTURES	1024
gltexture_t	gltextures[MAX_GLTEXTURES];
int			numgltextures;

extern int		gl_nvadptris;
extern int		gl_nvadpquads;


void GL_Bind (int texnum)
{
	if (gl_nobind.value)
		texnum = char_texture;
	if (currenttexture == texnum)
		return;

	if(gl_nvadptris || gl_nvadpquads)
	{
		R_RenderDelayPolyTris();
	}

	currenttexture = texnum;
//#ifdef _WIN32
//	bindTexFunc (GL_TEXTURE_2D, texnum);
//#else

	if(texnum>0)
		{ qglEnable (GL_TEXTURE_2D); }
	else
		{ qglDisable (GL_TEXTURE_2D); }

	qglBindTexture(GL_TEXTURE_2D, texnum);
//#endif
}


/*
=============================================================================

  scrap allocation

  Allocate all the little status bar obejcts into a single texture
  to crutch up stupid hardware / drivers

=============================================================================
*/

#define	MAX_SCRAPS		2
#define	BLOCK_WIDTH		256
#define	BLOCK_HEIGHT	256

int			scrap_allocated[MAX_SCRAPS][BLOCK_WIDTH];
byte		scrap_texels[MAX_SCRAPS][BLOCK_WIDTH*BLOCK_HEIGHT*4];
qboolean	scrap_dirty;
int			scrap_texnum;

// returns a texture number and the position inside it
int Scrap_AllocBlock (int w, int h, int *x, int *y)
{
	int		i, j;
	int		best, best2;
	int		bestx;
	int		texnum;

	for (texnum=0 ; texnum<MAX_SCRAPS ; texnum++)
	{
		best = BLOCK_HEIGHT;

		for (i=0 ; i<BLOCK_WIDTH-w ; i++)
		{
			best2 = 0;

			for (j=0 ; j<w ; j++)
			{
				if (scrap_allocated[texnum][i+j] >= best)
					break;
				if (scrap_allocated[texnum][i+j] > best2)
					best2 = scrap_allocated[texnum][i+j];
			}
			if (j == w)
			{	// this is a valid spot
				*x = i;
				*y = best = best2;
			}
		}

		if (best + h > BLOCK_HEIGHT)
			continue;

		for (i=0 ; i<w ; i++)
			scrap_allocated[texnum][*x + i] = best + h;

		return texnum;
	}

	Sys_Error ("Scrap_AllocBlock: full");
}

int	scrap_uploads;

void Scrap_Upload (void)
{
	int		texnum;

	scrap_uploads++;

	for (texnum=0 ; texnum<MAX_SCRAPS ; texnum++) {
		GL_Bind(scrap_texnum + texnum);
		GL_Upload8 (scrap_texels[texnum], BLOCK_WIDTH, BLOCK_HEIGHT, false, true);
	}
	scrap_dirty = false;
}

//=============================================================================
/* Support Routines */

typedef struct cachepic_s
{
	char		name[MAX_QPATH];
	qpic_t		pic;
	byte		padding[32];	// for appended glpic
} cachepic_t;

#define	MAX_CACHED_PICS		128
cachepic_t	menu_cachepics[MAX_CACHED_PICS];
int			menu_numcachepics;

byte		menuplyr_pixels[4096];

int		pic_texels;
int		pic_count;

qpic_t *Draw_PicFromWad (char *name)
{
	qpic_t	*p;
	glpic_t	*gl;

	p = W_GetLumpName (name);
	gl = (glpic_t *)p->data;

	// load little ones into the scrap
	if (p->width < 64 && p->height < 64)
	{
		int		x, y;
		int		i, j, k;
		int		texnum;

		x=0;		y=0;
		texnum = Scrap_AllocBlock (p->width, p->height, &x, &y);
		scrap_dirty = true;
		k = 0;
		for (i=0 ; i<p->height ; i++)
			for (j=0 ; j<p->width ; j++, k++)
				scrap_texels[texnum][(y+i)*BLOCK_WIDTH + x + j] = p->data[k];
		texnum += scrap_texnum;
		gl->texnum = texnum;
		gl->sl = (x+0.01)/(float)BLOCK_WIDTH;
		gl->sh = (x+p->width-0.01)/(float)BLOCK_WIDTH;
		gl->tl = (y+0.01)/(float)BLOCK_WIDTH;
		gl->th = (y+p->height-0.01)/(float)BLOCK_WIDTH;

		pic_count++;
		pic_texels += p->width*p->height;
	}
	else
	{
//		gl->texnum = GL_LoadPicTexture (p);
		gl->texnum = GL_LoadPicTexture (p, name);
		gl->sl = 0;
		gl->sh = 1;
		gl->tl = 0;
		gl->th = 1;
	}
	return p;
}


/*
================
Draw_CachePic
================
*/
qpic_t	*Draw_CachePic (char *path)
{
	cachepic_t	*pic;
	int			i;
	qpic_t		*dat;
	glpic_t		*gl;

	for (pic=menu_cachepics, i=0 ; i<menu_numcachepics ; pic++, i++)
		if (!strcmp (path, pic->name))
			return &pic->pic;

	if (menu_numcachepics == MAX_CACHED_PICS)
		Sys_Error ("menu_numcachepics == MAX_CACHED_PICS");
	menu_numcachepics++;
	strcpy (pic->name, path);

//
// load the pic from disk
//
	dat = (qpic_t *)COM_LoadTempFile (path);	
	if (!dat)
		Sys_Error ("Draw_CachePic: failed to load %s", path);
	SwapPic (dat);

	// HACK HACK HACK --- we need to keep the bytes for
	// the translatable player picture just for the menu
	// configuration dialog
	if (!strcmp (path, "gfx/menuplyr.lmp"))
		memcpy (menuplyr_pixels, dat->data, dat->width*dat->height);

	pic->pic.width = dat->width;
	pic->pic.height = dat->height;

	gl = (glpic_t *)pic->pic.data;
//	gl->texnum = GL_LoadPicTexture (dat);
	gl->texnum = GL_LoadPicTexture (dat, path);
	gl->sl = 0;
	gl->sh = 1;
	gl->tl = 0;
	gl->th = 1;

	return &pic->pic;
}


void Draw_CharToConback (int num, byte *dest)
{
	int		row, col;
	byte	*source;
	int		drawline;
	int		x;

	row = num>>4;
	col = num&15;
	source = draw_chars + (row<<10) + (col<<3);

	drawline = 8;

	while (drawline--)
	{
		for (x=0 ; x<8 ; x++)
			if (source[x] != 255)
				dest[x] = 0x60 + source[x];
		source += 128;
		dest += 320;
	}

}

typedef struct
{
	char *name;
	int	minimize, maximize;
} glmode_t;

glmode_t modes[] = {
	{"GL_NEAREST", GL_NEAREST, GL_NEAREST},
	{"GL_LINEAR", GL_LINEAR, GL_LINEAR},
	{"GL_LINEAR_MIPMAP_NEAREST_2", GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR},
	{"GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST},
	{"GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR}
};

/*
===============
Draw_TextureMode_f
===============
*/
void Draw_TextureMode_f (void)
{
	int		i;
	gltexture_t	*glt;

	if (Cmd_Argc() == 1)
	{
		for (i=0 ; i< 6 ; i++)
		if (	(gl_filter_min == modes[i].minimize) &&
				(gl_filter_max == modes[i].maximize) )
		{
			Con_Printf ("%s\n", modes[i].name);
			return;
		}

		for (i=0 ; i< 6 ; i++)
			if (gl_filter_min == modes[i].minimize)
			{
				Con_Printf ("%s\n", modes[i].name);
				return;
			}
		Con_Printf ("current filter is unknown???\n");
		return;
	}

	for (i=0 ; i< 6 ; i++)
	{
		if (!Q_strcasecmp (modes[i].name, Cmd_Argv(1) ) )
			break;
	}
	if (i == 6)
	{
		Con_Printf ("bad filter name\n");
		return;
	}

	gl_filter_min = modes[i].minimize;
	gl_filter_max = modes[i].maximize;

	// change all the existing mipmap texture objects
	for (i=0, glt=gltextures ; i<numgltextures ; i++, glt++)
	{
		if (glt->mipmap)
		{
			GL_Bind (glt->texnum);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
			qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
		}
	}
}

/*
===============
Draw_Init
===============
*/
void Draw_Init (void)
{
	int		i;
	qpic_t	*cb;
	byte	*dest, *src;
	int		x, y;
	char	ver[40];
	glpic_t	*gl;
	int		start;
	byte	*ncdata;
	int		f, fstep;


	Cvar_RegisterVariable (&gl_nobind);
	Cvar_RegisterVariable (&gl_max_size);
	Cvar_RegisterVariable (&gl_picmip);

	// 3dfx can only handle 256 wide textures
	if (!Q_strncasecmp ((char *)gl_renderer, "3dfx",4) ||
		strstr((char *)gl_renderer, "Glide"))
		Cvar_Set ("gl_max_size", "256");

	if (!Q_strcasecmp ((char *)gl_renderer, "TKRastGL"))
	{
		Cvar_Set ("gl_max_size", "256");
		Cvar_Set ("gl_flashblend", "0");
		Cvar_Set ("gl_polyblend", "0");
		Cvar_Set ("r_vertex", "1");
		gl_force_square = 1;
	}

	Cmd_AddCommand ("gl_texturemode", &Draw_TextureMode_f);

	// load the console background and the charset
	// by hand, because we need to write the version
	// string into the background before turning
	// it into a texture
	draw_chars = W_GetLumpName ("conchars");
	for (i=0 ; i<256*64 ; i++)
		if (draw_chars[i] == 0)
			draw_chars[i] = 255;	// proper transparent color

	// now turn them into textures
	char_texture = GL_LoadTexture ("charset",
		128, 128, draw_chars, false, true);

	start = Hunk_LowMark();

	cb = (qpic_t *)COM_LoadTempFile ("gfx/conback.lmp");	
	if (!cb)
		Sys_Error ("Couldn't load gfx/conback.lmp");
	SwapPic (cb);

	// hack the version number directly into the pic
#if defined(__linux__)
	sprintf (ver, "(Linux %2.2f, gl %4.2f) %4.2f", (float)LINUX_VERSION, (float)GLQUAKE_VERSION, (float)VERSION);
#else
	sprintf (ver, "(gl %4.2f) %4.2f", (float)GLQUAKE_VERSION, (float)VERSION);
#endif
	dest = cb->data + 320*186 + 320 - 11 - 8*strlen(ver);
	y = strlen(ver);
	for (x=0 ; x<y ; x++)
		Draw_CharToConback (ver[x], dest+(x<<3));

#if 0
	conback->width = vid.conwidth;
	conback->height = vid.conheight;

 	// scale console to vid size
 	dest = ncdata = Hunk_AllocName(vid.conwidth * vid.conheight, "conback");
 
 	for (y=0 ; y<vid.conheight ; y++, dest += vid.conwidth)
 	{
 		src = cb->data + cb->width * (y*cb->height/vid.conheight);
 		if (vid.conwidth == cb->width)
 			memcpy (dest, src, vid.conwidth);
 		else
 		{
 			f = 0;
 			fstep = cb->width*0x10000/vid.conwidth;
 			for (x=0 ; x<vid.conwidth ; x+=4)
 			{
 				dest[x] = src[f>>16];
 				f += fstep;
 				dest[x+1] = src[f>>16];
 				f += fstep;
 				dest[x+2] = src[f>>16];
 				f += fstep;
 				dest[x+3] = src[f>>16];
 				f += fstep;
 			}
 		}
 	}
#else
	conback->width = cb->width;
	conback->height = cb->height;
	ncdata = cb->data;
#endif

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	gl = (glpic_t *)conback->data;
	gl->texnum = GL_LoadTexture ("conback", conback->width, conback->height, ncdata, false, false);
	gl->sl = 0;
	gl->sh = 1;
	gl->tl = 0;
	gl->th = 1;
	conback->width = vid.width;
	conback->height = vid.height;

	// free loaded console
	Hunk_FreeToLowMark(start);

	// save a texture slot for translated picture
	translate_texture = texture_extension_number++;

	// save slots for scraps
	scrap_texnum = texture_extension_number;
	texture_extension_number += MAX_SCRAPS;

	//
	// get the other pics we need
	//
	draw_disc = Draw_PicFromWad ("disc");
	draw_backtile = Draw_PicFromWad ("backtile");
}



/*
================
Draw_Character

Draws one 8*8 graphics character with 0 being transparent.
It can be clipped to the top of the screen to allow the console to be
smoothly scrolled off.
================
*/
void Draw_Character (int x, int y, int num)
{
	byte			*dest;
	byte			*source;
	unsigned short	*pusdest;
	qgl_hfloat		*v;
	int				drawline;	
	int				row, col;
	float			frow, fcol, size;

	if (num == 32)
		return;		// space

	num &= 255;
	
	if (y <= -8)
		return;			// totally off screen

	row = num>>4;
	col = num&15;

	frow = row*0.0625;
	fcol = col*0.0625;
	size = 0.0625;

#if 0
//	qglEnable (GL_TEXTURE_2D);

	GL_Bind (char_texture);

	qglBegin (GL_QUADS);
	qglTexCoord2f (fcol, frow);
	qglVertex2f (x, y);
	qglTexCoord2f (fcol + size, frow);
	qglVertex2f (x+8, y);
	qglTexCoord2f (fcol + size, frow + size);
	qglVertex2f (x+8, y+8);
	qglTexCoord2f (fcol, frow + size);
	qglVertex2f (x, y+8);
	qglEnd ();
#endif

#if 1
	GL_Bind (char_texture);

	v=R_CheckExpandDelayPolyQuads(4);
	
	v[0]=x; v[1]=y; v[2]=0; v[3]=fcol; v[4]=frow;
	*(u32 *)(v+VERTEX_RGBA)=0xFFFFFFFFU;
	v+=VERTEXSIZE;

	v[0]=x+8; v[1]=y; v[2]=0; v[3]=fcol+size; v[4]=frow;
	*(u32 *)(v+VERTEX_RGBA)=0xFFFFFFFFU;
	v+=VERTEXSIZE;

	v[0]=x+8; v[1]=y+8; v[2]=0; v[3]=fcol+size; v[4]=frow+size;
	*(u32 *)(v+VERTEX_RGBA)=0xFFFFFFFFU;
	v+=VERTEXSIZE;

	v[0]=x; v[1]=y+8; v[2]=0; v[3]=fcol; v[4]=frow+size;
	*(u32 *)(v+VERTEX_RGBA)=0xFFFFFFFFU;
	v+=VERTEXSIZE;
	
	gl_nvadpquads+=4;
#endif
}

/*
================
Draw_String
================
*/
void Draw_String (int x, int y, char *str)
{
	GL_Bind (char_texture);

	while (*str)
	{
		Draw_Character (x, y, *str);
		str++;
		x += 8;
	}

	R_RenderDelayPolyTris();
}

/*
================
Draw_DebugChar

Draws a single character directly to the upper right corner of the screen.
This is for debugging lockups by drawing different chars in different parts
of the code.
================
*/
void Draw_DebugChar (char num)
{
}

/*
=============
Draw_AlphaPic
=============
*/
void Draw_AlphaPic (int x, int y, qpic_t *pic, float alpha)
{
	byte			*dest, *source;
	unsigned short	*pusdest;
	int				v, u;
	glpic_t			*gl;

	if (scrap_dirty)
		Scrap_Upload ();
	gl = (glpic_t *)pic->data;
	qglEnable (GL_TEXTURE_2D);
	qglDisable(GL_ALPHA_TEST);
	qglEnable (GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glCullFace(GL_FRONT);
	qglColor4f (1,1,1,alpha);
	GL_Bind (gl->texnum);
	qglBegin (GL_QUADS);
	qglTexCoord2f (gl->sl, gl->tl);
	qglVertex2f (x, y);
	qglTexCoord2f (gl->sh, gl->tl);
	qglVertex2f (x+pic->width, y);
	qglTexCoord2f (gl->sh, gl->th);
	qglVertex2f (x+pic->width, y+pic->height);
	qglTexCoord2f (gl->sl, gl->th);
	qglVertex2f (x, y+pic->height);
	qglEnd ();
	qglColor4f (1,1,1,1);
	qglEnable(GL_ALPHA_TEST);
	qglDisable (GL_BLEND);
}


/*
=============
Draw_Pic
=============
*/
void Draw_Pic (int x, int y, qpic_t *pic)
{
	byte			*dest, *source;
	unsigned short	*pusdest;
	int				v, u;
	glpic_t			*gl;

	if (scrap_dirty)
		Scrap_Upload ();
	gl = (glpic_t *)pic->data;
	qglColor4f (1,1,1,1);
	qglEnable (GL_TEXTURE_2D);
	GL_Bind (gl->texnum);
	qglBegin (GL_QUADS);
	qglTexCoord2f (gl->sl, gl->tl);
	qglVertex2f (x, y);
	qglTexCoord2f (gl->sh, gl->tl);
	qglVertex2f (x+pic->width, y);
	qglTexCoord2f (gl->sh, gl->th);
	qglVertex2f (x+pic->width, y+pic->height);
	qglTexCoord2f (gl->sl, gl->th);
	qglVertex2f (x, y+pic->height);
	qglEnd ();
}


/*
=============
Draw_TransPic
=============
*/
void Draw_TransPic (int x, int y, qpic_t *pic)
{
	byte	*dest, *source, tbyte;
	unsigned short	*pusdest;
	int				v, u;

	if (x < 0 || (unsigned)(x + pic->width) > vid.width || y < 0 ||
		 (unsigned)(y + pic->height) > vid.height)
	{
		Sys_Error ("Draw_TransPic: bad coordinates");
	}
		
	Draw_Pic (x, y, pic);
}


/*
=============
Draw_TransPicTranslate

Only used for the player color selection menu
=============
*/
void Draw_TransPicTranslate (int x, int y, qpic_t *pic, byte *translation)
{
	int				v, u, c;
//	static
//	unsigned		trans[64*64];
	static unsigned *trans=NULL;
	unsigned		*dest;
	byte			*src;
	int				p;

	if(!trans)
		trans=malloc(64*64*sizeof(unsigned));

	GL_Bind (translate_texture);

	c = pic->width * pic->height;

	dest = trans;
	for (v=0 ; v<64 ; v++, dest += 64)
	{
		src = &menuplyr_pixels[ ((v*pic->height)>>6) *pic->width];
		for (u=0 ; u<64 ; u++)
		{
			p = src[(u*pic->width)>>6];
			if (p == 255)
				dest[u] = p;
			else
				dest[u] =  d_8to24table[translation[p]];
		}
	}

	qglTexImage2D (GL_TEXTURE_2D, 0, gl_alpha_format, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, trans);

//	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	qglColor3f (1,1,1);
	qglBegin (GL_QUADS);
	qglTexCoord2f (0, 0);
	qglVertex2f (x, y);
	qglTexCoord2f (1, 0);
	qglVertex2f (x+pic->width, y);
	qglTexCoord2f (1, 1);
	qglVertex2f (x+pic->width, y+pic->height);
	qglTexCoord2f (0, 1);
	qglVertex2f (x, y+pic->height);
	qglEnd ();
}


/*
================
Draw_ConsoleBackground

================
*/
void Draw_ConsoleBackground (int lines)
{
	int y = (vid.height * 3) >> 2;

	if (lines > y)
		Draw_Pic(0, lines - vid.height, conback);
	else
		Draw_AlphaPic (0, lines - vid.height, conback, (float)(1.2 * lines)/y);
}


/*
=============
Draw_TileClear

This repeats a 64*64 tile graphic to fill the screen around a sized down
refresh window.
=============
*/
void Draw_TileClear (int x, int y, int w, int h)
{
	qglColor3f (1,1,1);
	GL_Bind (*(int *)draw_backtile->data);
	qglBegin (GL_QUADS);
	qglTexCoord2f (x/64.0, y/64.0);
	qglVertex2f (x, y);
	qglTexCoord2f ( (x+w)/64.0, y/64.0);
	qglVertex2f (x+w, y);
	qglTexCoord2f ( (x+w)/64.0, (y+h)/64.0);
	qglVertex2f (x+w, y+h);
	qglTexCoord2f ( x/64.0, (y+h)/64.0 );
	qglVertex2f (x, y+h);
	qglEnd ();
}


/*
=============
Draw_Fill

Fills a box of pixels with a single color
=============
*/
void Draw_Fill (int x, int y, int w, int h, int c)
{
	qglDisable (GL_TEXTURE_2D);
	qglColor3f (
		host_basepal[c*3+0]/255.0,
		host_basepal[c*3+1]/255.0,
		host_basepal[c*3+2]/255.0);

	qglBegin (GL_QUADS);

	qglVertex2f (x,y);
	qglVertex2f (x+w, y);
	qglVertex2f (x+w, y+h);
	qglVertex2f (x, y+h);

	qglEnd ();
	qglColor3f (1,1,1);
	qglEnable (GL_TEXTURE_2D);
}
//=============================================================================

/*
================
Draw_FadeScreen

================
*/
void Draw_FadeScreen (void)
{
	qglEnable (GL_BLEND);
	qglDisable (GL_TEXTURE_2D);
	qglColor4f (0, 0, 0, 0.8);
	qglBegin (GL_QUADS);

	qglVertex2f (0,0);
	qglVertex2f (vid.width, 0);
	qglVertex2f (vid.width, vid.height);
	qglVertex2f (0, vid.height);

	qglEnd ();
	qglColor4f (1,1,1,1);
	qglEnable (GL_TEXTURE_2D);
	qglDisable (GL_BLEND);

	Sbar_Changed();
}

//=============================================================================

/*
================
Draw_BeginDisc

Draws the little blue disc in the corner of the screen.
Call before beginning any disc IO.
================
*/
void Draw_BeginDisc (void)
{
	if (!draw_disc)
		return;
	qglDrawBuffer  (GL_FRONT);
	Draw_Pic (vid.width - 24, 0, draw_disc);
	qglDrawBuffer  (GL_BACK);
}


/*
================
Draw_EndDisc

Erases the disc icon.
Call after completing any disc IO
================
*/
void Draw_EndDisc (void)
{
}

/*
================
GL_Set2D

Setup as if the screen was 320*200
================
*/
void GL_Set2D (void)
{
	qglViewport (glx, gly, glwidth, glheight);

	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity ();
	qglOrtho  (0, vid.width, vid.height, 0, -99999, 99999);

	qglMatrixMode(GL_MODELVIEW);
	qglLoadIdentity ();

	qglDisable (GL_DEPTH_TEST);
	qglDisable (GL_CULL_FACE);
	qglDisable (GL_BLEND);
	qglEnable (GL_ALPHA_TEST);
//	qglDisable (GL_ALPHA_TEST);

	qglColor4f (1,1,1,1);
}

//====================================================================

/*
================
GL_FindTexture
================
*/
int GL_FindTexture (char *identifier)
{
	int		i;
	gltexture_t	*glt;

	for (i=0, glt=gltextures ; i<numgltextures ; i++, glt++)
	{
		if (!strcmp (identifier, glt->identifier))
			return gltextures[i].texnum;
	}

	return -1;
}

/*
================
GL_ResampleTexture
================
*/
void GL_ResampleTexture (unsigned *in, int inwidth, int inheight, unsigned *out,  int outwidth, int outheight)
{
	int		i, j;
	unsigned	*inrow;
	unsigned	frac, fracstep;

	fracstep = inwidth*0x10000/outwidth;
	for (i=0 ; i<outheight ; i++, out += outwidth)
	{
		inrow = in + inwidth*(i*inheight/outheight);
		frac = fracstep >> 1;
		for (j=0 ; j<outwidth ; j+=4)
		{
			out[j] = inrow[frac>>16];
			frac += fracstep;
			out[j+1] = inrow[frac>>16];
			frac += fracstep;
			out[j+2] = inrow[frac>>16];
			frac += fracstep;
			out[j+3] = inrow[frac>>16];
			frac += fracstep;
		}
	}
}

/*
================
GL_Resample8BitTexture -- JACK
================
*/
void GL_Resample8BitTexture (unsigned char *in, int inwidth, int inheight, unsigned char *out,  int outwidth, int outheight)
{
	int		i, j;
	unsigned	char *inrow;
	unsigned	frac, fracstep;

	fracstep = inwidth*0x10000/outwidth;
	for (i=0 ; i<outheight ; i++, out += outwidth)
	{
		inrow = in + inwidth*(i*inheight/outheight);
		frac = fracstep >> 1;
		for (j=0 ; j<outwidth ; j+=4)
		{
			out[j] = inrow[frac>>16];
			frac += fracstep;
			out[j+1] = inrow[frac>>16];
			frac += fracstep;
			out[j+2] = inrow[frac>>16];
			frac += fracstep;
			out[j+3] = inrow[frac>>16];
			frac += fracstep;
		}
	}
}


/*
================
GL_MipMap

Operates in place, quartering the size of the texture
================
*/
void GL_MipMap (byte *in, int width, int height)
{
	int		i, j;
	byte	*out;

	width <<=2;
	height >>= 1;
	out = in;
	for (i=0 ; i<height ; i++, in+=width)
	{
		for (j=0 ; j<width ; j+=8, out+=4, in+=8)
		{
			out[0] = (in[0] + in[4] + in[width+0] + in[width+4])>>2;
			out[1] = (in[1] + in[5] + in[width+1] + in[width+5])>>2;
			out[2] = (in[2] + in[6] + in[width+2] + in[width+6])>>2;
			out[3] = (in[3] + in[7] + in[width+3] + in[width+7])>>2;
		}
	}
}

/*
================
GL_MipMap8Bit

Mipping for 8 bit textures
================
*/
void GL_MipMap8Bit (byte *in, int width, int height)
{
	int		i, j;
	unsigned short	 r,g,b;
	byte	*out, *at1, *at2, *at3, *at4;

//	width <<=2;
	height >>= 1;
	out = in;
	for (i=0 ; i<height ; i++, in+=width)
	{
		for (j=0 ; j<width ; j+=2, out+=1, in+=2)
		{
			at1 = (byte *) (d_8to24table + in[0]);
			at2 = (byte *) (d_8to24table + in[1]);
			at3 = (byte *) (d_8to24table + in[width+0]);
			at4 = (byte *) (d_8to24table + in[width+1]);

 			r = (at1[0]+at2[0]+at3[0]+at4[0]); r>>=5;
 			g = (at1[1]+at2[1]+at3[1]+at4[1]); g>>=5;
 			b = (at1[2]+at2[2]+at3[2]+at4[2]); b>>=5;

			out[0] = d_15to8table[(r<<0) + (g<<5) + (b<<10)];
		}
	}
}

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT  0x83F0
#endif

char *gl_savetexname = NULL;
byte *gl_savetexbuf = NULL;

int GL_LumaForPix24(unsigned pix)
{
	int cr, cg, cb, cy;
	cr=(pix>> 0)&255;
	cg=(pix>> 8)&255;
	cb=(pix>>16)&255;
//	cy=(cr+2*cg+cb)/4;
	cy=(11*cr+16*cg+5*cb)/32;
	return(cy);
}

int GL_LumaForPix24Ax(unsigned pix, int ax)
{
	int cr, cg, cb, cy;
	cr=(pix>> 0)&255;
	cg=(pix>> 8)&255;
	cb=(pix>>16)&255;
//	cy=(cr+2*cg+cb)/4;
	if(ax==0)
		{ cy=(cr+2*cg+cb)/4; }
	else if(ax==1)
		{ cy=(85*cr+85*cg+85*cb)/256; }
	else if(ax==2)
		{ cy=(11*cr+16*cg+ 5*cb)/32; }
	else if(ax==3)
		{ cy=(16*cr+11*cg+ 5*cb)/32; }
	else if(ax==4)
		{ cy=(16*cr+ 5*cg+11*cb)/32; }
	else if(ax==5)
		{ cy=( 5*cr+16*cg+11*cb)/32; }
	else if(ax==6)
		{ cy=(11*cr+ 5*cg+16*cb)/32; }
	else if(ax==7)
		{ cy=( 5*cr+11*cg+16*cb)/32; }
	return(cy);
}

int GL_ConvPix24To565(unsigned pix)
{
	int cr, cg, cb, cy;
	cr=(pix>> 0)&255;
	cg=(pix>> 8)&255;
	cb=(pix>>16)&255;
	cy=((cr<<8)&0xF800)|((cg<<3)&0x07E0)|((cb>>3)&0x001F);
	return(cy);
}

void GL_EncodeBlockDxt1(unsigned *src, byte *dst, int ystr)
{
	byte pxy[16];
	byte pxa[16];
	int x, y, px, cy, acy, acy0, acy1, acy2;
	int ma, na, ca, flip;
	int my, ny, mpx, npx, apx, mcx, ncx;
	int mpx1, npx1, ax;
	int i, j, k;

	ax=0; apx=0;
	for(i=0; i<8; i++)
	{
		my=256; ny=-1; acy=0;
		for(y=0; y<4; y++)
			for(x=0; x<4; x++)
		{
			px=src[y*ystr+x];
			cy=GL_LumaForPix24Ax(px, i);
			if(cy<my)my=cy;
			if(cy>ny)ny=cy;
			acy+=cy;
		}
		
		acy/=16;

		ca=0;
		for(y=0; y<4; y++)
			for(x=0; x<4; x++)
		{
			px=src[y*ystr+x];
			cy=GL_LumaForPix24Ax(px, i);
			cy=cy-acy; cy=cy*cy;
			ca+=cy;
		}

		if(ca>apx)
			{ ax=i; apx=ca; }
		
//		cy=ny-my;
//		if(cy>apx)
//			{ ax=i; apx=cy; }
	}
	
	my=256; ny=-1; ma=256; na=256;
	apx=0; acy=0;
	for(y=0; y<4; y++)
		for(x=0; x<4; x++)
	{
		px=src[y*ystr+x];
//		cy=GL_LumaForPix24(px);
		cy=GL_LumaForPix24Ax(px, ax);
		if(cy<my)
			{ my=cy; mpx=px; }
		if(cy>ny)
			{ ny=cy; npx=px; }
		
		ca=(px>>24)&255;
		if(ca<ma)ma=ca;
		if(ca>na)na=ca;
		
		pxy[y*4+x] = cy;
		pxa[y*4+x] = ca;
		apx += (px>>4)&0x0F0F0F0F;
		acy += cy;
	}
	acy /= 16;
	
	mpx1 = ((mpx>>1)&0x7F7F7F7F) + ((apx >>1)&0x7F7F7F7F);
	npx1 = ((npx>>1)&0x7F7F7F7F) + ((apx >>1)&0x7F7F7F7F);
	mpx1 = ((mpx>>1)&0x7F7F7F7F) + ((mpx1>>1)&0x7F7F7F7F);
	npx1 = ((npx>>1)&0x7F7F7F7F) + ((npx1>>1)&0x7F7F7F7F);
	mcx = GL_ConvPix24To565 (mpx1);
	ncx = GL_ConvPix24To565 (npx1);

	my=GL_LumaForPix24Ax(mpx1, ax);
	ny=GL_LumaForPix24Ax(npx1, ax);
	acy0 = (my+ny)/2;

//	acy1 = (my+acy)/2;
//	acy2 = (ny+acy)/2;

	acy1 = (11*my+5*acy)/16;
	acy2 = (11*ny+5*acy)/16;
	acy0 = (my+ny)/2;


	flip = 0;
	if(ma<128)
		flip = 1;
	if(mcx==ncx)
		flip = 1;

	if((ncx>mcx) ^ flip)
	{
		*(short *)(dst+0)=ncx;
		*(short *)(dst+2)=mcx;
		
		px=0;
		for(i=0; i<16; i++)
		{
			cy=pxy[i];
			ca=pxa[i];
			if(flip) j=(cy<=acy0);
			else if(cy>=acy2)j=0;
			else if(cy<=acy1)j=1;
			else if((cy<=acy0) && !flip)j=3;
			else 	j=2;

			if(ca<128)
				j=3;

			px|=j<<(i*2);
		}
	}else
	{
		*(short *)(dst+0)=mcx;
		*(short *)(dst+2)=ncx;
		
		px=0;
		for(i=0; i<16; i++)
		{
			cy=pxy[i];
			ca=pxa[i];
			if(flip) j=(cy>=acy0);
			else if(cy>=acy2)j=1;
			else if(cy<=acy1)j=0;
			else if((cy>=acy0) && !flip)	j=3;
			else j=2;

			if(ca<128)
				j=3;

			px|=j<<(i*2);
		}
	}

	*(int *)(dst+4)=px;
}

void GL_EncodeSquareDxt1(unsigned *src, byte *dst, int szlg2)
{
	unsigned *cs;
	byte *ct;
	int nblk, ystr;
	int x, y;
	
	if(szlg2<0)
		szlg2=0;
	
	ct=dst;
	ystr=1<<szlg2;
	nblk=((1<<szlg2)+3)/4;
	for(y=0; y<nblk; y++)
		for(x=0; x<nblk; x++)
	{
		cs=src+((y*4*ystr)+(x*4));
		GL_EncodeBlockDxt1(cs, ct, ystr);
		ct+=8;
	}
}

/*
===============
GL_Upload32
===============
*/
void GL_Upload32 (unsigned *data, int width, int height,  qboolean mipmap, qboolean alpha)
{
	static unsigned *scaled = NULL;
	QGL_DDS_HEADER *tdds;
	byte		*ctbuf;
	int			samples, szlog2;
// static	unsigned	scaled[1024*512];	// [512*256];
	int			scaled_width, scaled_height, isz;
	int			do_square, max_size;
	int			i, j, k;

	do_square = gl_force_square;
	max_size = gl_max_size.value;

	ctbuf = NULL;
	if(gl_savetexname)
	{
		do_square = 1;
		max_size = 256;
	
		if(!gl_savetexbuf)
		{
			samples = gl_max_size.value * gl_max_size.value*4;
	//		samples /= 2;	
			gl_savetexbuf = malloc(samples);
		}
		
//		ctbuf = gl_savetexbuf + 16;
		ctbuf = gl_savetexbuf + 128;
	}

	for (scaled_width = 1 ; scaled_width < width ; scaled_width<<=1)
		;
	for (scaled_height = 1 ; scaled_height < height ; scaled_height<<=1)
		;

//	mipmap=0;

	scaled_width >>= (int)gl_picmip.value;
	scaled_height >>= (int)gl_picmip.value;
	
//	if(gl_force_square && (scaled_width != scaled_height))
	if(do_square && (scaled_width != scaled_height))
	{
		if(scaled_height > scaled_width)
			scaled_width = scaled_height;
		scaled_height = scaled_width;
	}

//	if (scaled_width > gl_max_size.value)
//		scaled_width = gl_max_size.value;
//	if (scaled_height > gl_max_size.value)
//		scaled_height = gl_max_size.value;

	if (scaled_width > max_size)
		scaled_width = max_size;
	if (scaled_height > max_size)
		scaled_height = max_size;

//	if (scaled_width * scaled_height > sizeof(scaled)/4)
//		Sys_Error ("GL_LoadTexture: too big");

	if(!scaled)
	{
		samples = gl_max_size.value * gl_max_size.value;
		scaled = malloc(samples * sizeof(unsigned));
	}

#if 0
	if(ctbuf)
	{
		memset(gl_savetexbuf, 0, 16);

		gl_savetexbuf[0]='T';
		gl_savetexbuf[1]='X';
		gl_savetexbuf[2]='C';
		gl_savetexbuf[3]='0';
		
		szlog2 = 0;
		i = scaled_width;
		while(i>1)
			{ szlog2++; i=i>>1; }

		gl_savetexbuf[4]=szlog2;
		gl_savetexbuf[5]=szlog2;
		
		i=0;
		if(mipmap)	i|=1;
		if(alpha)	i|=2;
		gl_savetexbuf[6]=i;
	}
#endif

#if 1
	if(ctbuf)
	{
		memset(gl_savetexbuf, 0, 128);

		gl_savetexbuf[0]='D';
		gl_savetexbuf[1]='D';
		gl_savetexbuf[2]='S';
		gl_savetexbuf[3]=' ';
		
		tdds = (void *)(gl_savetexbuf + 4);

		szlog2 = 0;
		i = scaled_width;
		while(i>1)
			{ szlog2++; i=i>>1; }

		j=szlog2-2;
		if(j<0)j=0;
		isz=1<<(j+j+3);

		i=0x81007;
		if(mipmap)
			i|=0x20000;
		
		tdds->dwSize=124;
		tdds->dwFlags=0;
		tdds->dwHeight=scaled_height;
		tdds->dwWidth=scaled_width;
		tdds->dwPitchOrLinearSize=isz;
		tdds->dwDepth=0;
		tdds->dwMipMapCount=mipmap?szlog2:0;
		tdds->dwCaps=mipmap?0x401008:0x001008;
		tdds->dwCaps2=0;
		tdds->dwCaps3=0;
		tdds->dwCaps4=0;
		tdds->dwReserved2=0;
		
		tdds->ddspf.dwSize=32;
		tdds->ddspf.dwFlags=0x0004;
		tdds->ddspf.dwFourCC=QGL_FOURCC('D', 'X', 'T', '1');
		tdds->ddspf.dwRGBBitCount=16;
		tdds->ddspf.dwRBitMask=0x001F;
		tdds->ddspf.dwGBitMask=0x07E0;
		tdds->ddspf.dwBBitMask=0xF800;
		tdds->ddspf.dwABitMask=0x0000;
	}
#endif

	samples = alpha ? gl_alpha_format : gl_solid_format;

#if 0
	if (mipmap)
		qgluBuild2DMipmaps (GL_TEXTURE_2D,
			samples, width, height, GL_RGBA, GL_UNSIGNED_BYTE, trans);
	else if (scaled_width == width && scaled_height == height)
		qglTexImage2D (GL_TEXTURE_2D, 0,
			samples, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, trans);
	else
	{
		qgluScaleImage (GL_RGBA, width, height, GL_UNSIGNED_BYTE, trans,
			scaled_width, scaled_height, GL_UNSIGNED_BYTE, scaled);
		qglTexImage2D (GL_TEXTURE_2D, 0,
			samples, scaled_width, scaled_height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, scaled);
	}
#else
	texels += scaled_width * scaled_height;

	if (scaled_width == width && scaled_height == height)
	{
		if (!mipmap)
		{
			if(ctbuf)
			{
				j=szlog2-2;
				if(j<0)j=0;
				isz=1<<(j+j+3);

				GL_EncodeSquareDxt1(data, ctbuf, szlog2);
				qglCompressedTexImage2D(
					GL_TEXTURE_2D, 0, 
					GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
					1<<szlog2, 1<<szlog2, 0, isz, ctbuf);
				ctbuf+=isz;
			}else
			{
				qglTexImage2D (GL_TEXTURE_2D, 0,
					samples, scaled_width, scaled_height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			goto done;
		}
		memcpy (scaled, data, width*height*4);
	}
	else
	{
		GL_ResampleTexture (data, width, height,
			scaled, scaled_width, scaled_height);
	}

	if(ctbuf)
	{
		j=szlog2-2;
		if(j<0)j=0;
		isz=1<<(j+j+3);
//		isz=1<<(szlog2+szlog2+3);
//		isz=1<<(szlog2+szlog2-1);
		GL_EncodeSquareDxt1(scaled, ctbuf, szlog2);
		qglCompressedTexImage2D(
			GL_TEXTURE_2D, 0, 
			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
			1<<szlog2, 1<<szlog2, 0, isz, ctbuf);
		ctbuf+=isz;
		szlog2--;
	}else
	{
		qglTexImage2D (GL_TEXTURE_2D, 0, samples,
			scaled_width, scaled_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, scaled);
	}

	if (mipmap)
	{
		int		miplevel;

		miplevel = 0;
		while (scaled_width > 1 || scaled_height > 1)
		{
			GL_MipMap ((byte *)scaled, scaled_width, scaled_height);
			scaled_width >>= 1;
			scaled_height >>= 1;
			if (scaled_width < 1)
				scaled_width = 1;
			if (scaled_height < 1)
				scaled_height = 1;
			miplevel++;

			if(ctbuf)
			{
				j=szlog2-2;
				if(j<0)j=0;
				isz=1<<(j+j+3);

//				isz=1<<(szlog2+szlog2+3);
				GL_EncodeSquareDxt1(scaled, ctbuf, szlog2);
				qglCompressedTexImage2D(
					GL_TEXTURE_2D, miplevel, 
					GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
					1<<szlog2, 1<<szlog2, 0, isz, ctbuf);
				ctbuf+=isz;
				szlog2--;
			}else
			{
				qglTexImage2D (GL_TEXTURE_2D, miplevel,
					samples, scaled_width, scaled_height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, scaled);
			}
		}
	}
	done:

	if(gl_savetexname && ctbuf)
	{
		COM_WriteFile(gl_savetexname, gl_savetexbuf, ctbuf - gl_savetexbuf);
	}
#endif


	if (mipmap)
	{
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, gl_filter_min);
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
	else
	{
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, gl_filter_max);
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
}

void GL_Upload8_EXT (
	byte *data, int width, int height,
	qboolean mipmap, qboolean alpha) 
{
	__debugbreak();

#if 0
	int			i, s;
	qboolean	noalpha;
	int			p;
	static unsigned j;
	int			samples;
//	static	unsigned char scaled[1024*512];	// [512*256];
	int			scaled_width, scaled_height;

	__debugbreak();

	s = width*height;
	// if there are no transparent pixels, make it a 3 component
	// texture even if it was specified as otherwise
	if (alpha)
	{
		noalpha = true;
		for (i=0 ; i<s ; i++)
		{
			if (data[i] == 255)
				noalpha = false;
		}

		if (alpha && noalpha)
			alpha = false;
	}
	for (scaled_width = 1 ; scaled_width < width ; scaled_width<<=1)
		;
	for (scaled_height = 1 ; scaled_height < height ; scaled_height<<=1)
		;

	scaled_width >>= (int)gl_picmip.value;
	scaled_height >>= (int)gl_picmip.value;

	if (scaled_width > gl_max_size.value)
		scaled_width = gl_max_size.value;
	if (scaled_height > gl_max_size.value)
		scaled_height = gl_max_size.value;

	if (scaled_width * scaled_height > sizeof(scaled))
		Sys_Error ("GL_LoadTexture: too big");

	samples = 1; // alpha ? gl_alpha_format : gl_solid_format;

	texels += scaled_width * scaled_height;

	if (scaled_width == width && scaled_height == height)
	{
		if (!mipmap)
		{
			qglTexImage2D (GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT, scaled_width, scaled_height, 0, GL_COLOR_INDEX , GL_UNSIGNED_BYTE, data);
			goto done;
		}
		memcpy (scaled, data, width*height);
	}
	else
		GL_Resample8BitTexture (data, width, height, scaled, scaled_width, scaled_height);

	qglTexImage2D (GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT, scaled_width, scaled_height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, scaled);
	if (mipmap)
	{
		int		miplevel;

		miplevel = 0;
		while (scaled_width > 1 || scaled_height > 1)
		{
			GL_MipMap8Bit ((byte *)scaled, scaled_width, scaled_height);
			scaled_width >>= 1;
			scaled_height >>= 1;
			if (scaled_width < 1)
				scaled_width = 1;
			if (scaled_height < 1)
				scaled_height = 1;
			miplevel++;
			qglTexImage2D (GL_TEXTURE_2D, miplevel, GL_COLOR_INDEX8_EXT, scaled_width, scaled_height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, scaled);
		}
	}
done: ;


	if (mipmap)
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_min);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
	else
	{
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter_max);
		qglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
#endif
}

/*
===============
GL_Upload8
===============
*/
void GL_Upload8 (byte *data, int width, int height,
	qboolean mipmap, qboolean alpha)
{
	static	unsigned	*trans = NULL;
//	static	unsigned	trans[640*480];		// FIXME, temporary
	int			i, s;
	qboolean	noalpha;
	int			p;

	if(!trans)
	{
		trans = malloc(640*480*sizeof(int));
	}

	s = width*height;
	// if there are no transparent pixels, make it a 3 component
	// texture even if it was specified as otherwise
	if (alpha)
	{
		noalpha = true;
		for (i=0 ; i<s ; i++)
		{
			p = data[i];
			if (p == 255)
				noalpha = false;
			trans[i] = d_8to24table[p];
		}

		if (alpha && noalpha)
			alpha = false;
	}
	else
	{
		if (s&3)
			Sys_Error ("GL_Upload8: s&3");
		for (i=0 ; i<s ; i+=4)
		{
			trans[i+0] = d_8to24table[data[i+0]];
			trans[i+1] = d_8to24table[data[i+1]];
			trans[i+2] = d_8to24table[data[i+2]];
			trans[i+3] = d_8to24table[data[i+3]];
		}
	}

// 	if (VID_Is8bit() && !alpha && (data!=scrap_texels[0])) {
// 		GL_Upload8_EXT (data, width, height, mipmap, alpha);
// 		return;
//	}
	GL_Upload32 (trans, width, height, mipmap, alpha);
}

/*
Upload compressed texture.
Format is fairly minimal.

  0000: Magic
  0004: Image X Size (Log2)
  0005: Image Y Size (Log2)
  0006: Flags
  0010: Start of Image Data
 */
void GL_UploadCompressed (
	byte *data, int width, int height,
	qboolean mipmap, qboolean alpha)
{
	QGL_DDS_HEADER *tdds;
	byte *cs, *css;
	int xs, ys;
	int xshl, xshl1, isz, mip, txc;
	int i, j, k;
	
	if(!memcmp(data, "TXC0", 4))
	{
		css=data+16;
		xshl=data[4];
		
		if((xshl<0) || (xshl>8))
		{
			__debugbreak();
		}
	}

	if(!memcmp(data, "DDS ", 4))
	{
		tdds = (void *)(data+4);
		css=data+4+(tdds->dwSize);
//		xshl=data[4];

		xs=tdds->dwWidth;
		i=xs; xshl=0;
		while(i>1)
			{ i>>=1; xshl++; }
	}

	txc=GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	if(!alpha)
		txc=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;

	cs=css; mip=0;
	while(xshl>=0)
	{
		if(mip && !mipmap)
			break;
	
		xshl1=xshl-2;
		if(xshl1<0)
			xshl1=0;
	
		isz=1<<(xshl1+xshl1+3);
		qglCompressedTexImage2D(
			GL_TEXTURE_2D, mip, 
//			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
			txc,
			1<<xshl, 1<<xshl, 0, isz, cs);
		cs+=isz;
		xshl--; mip++;
	}

	if (mipmap)
	{
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, gl_filter_min);
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
	else
	{
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, gl_filter_max);
		qglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, gl_filter_max);
	}
}

/*
================
GL_LoadTexture
================
*/
int GL_LoadTexture (char *identifier,
	int width, int height, byte *data,
	qboolean mipmap, qboolean alpha)
{
	u64 h;
	char		tname[256], tid1[64];
	qboolean	noalpha;
	byte		*imgbuf;
	char		*cs;
	int			i, p, s, imgbufsz;
	gltexture_t	*glt;

	// see if the texture is allready present
	if (identifier[0])
	{
		for (i=0, glt=gltextures ; i<numgltextures ; i++, glt++)
		{
			if (!strcmp (identifier, glt->identifier))
			{
				if (width != glt->width || height != glt->height)
					Sys_Error ("GL_LoadTexture: cache mismatch");
				return gltextures[i].texnum;
			}
		}
	}
	else {
//		glt = &gltextures[numgltextures];
//		numgltextures++;
	}

	gl_savetexname = NULL;
	imgbuf = NULL;

	if(identifier && (strlen(identifier)>2))
	{
		strcpy(tid1, identifier);
		cs=tid1;
		while(*cs)
		{
			i=*cs;
			if((i>='a') && (i<='z'))
				{ cs++; continue; }
			if((i>='A') && (i<='Z'))
				{ cs++; continue; }
			if((i>='0') && (i<='9'))
				{ cs++; continue; }
			if((i=='_') || (i=='-'))
				{ cs++; continue; }
			*cs++='_';
		}
		
		if(strlen(tid1)>12)
		{
			cs=identifier; h=0;
			while(*cs)
				{ h=(h*65521)+(*cs++); }
			i=4;
			while(i--)
				{ h=(h*65521)+1; }
			i=(h>>16)&0x7FFFFFFFU;
			sprintf(tid1, "x%08x", i);
		}
		
		imgbufsz=0;
//		sprintf(tname, "texcache/%s.lmp", tid1);
		sprintf(tname, "texcache/%s.dds", tid1);
//		imgbuf = COM_LoadFileSz(tname, 1, &imgbufsz);
		imgbuf = COM_LoadFileSz(tname, 5, &imgbufsz);
#ifndef __BJX2__
		gl_savetexname = tname;
#endif
	}

//	if(identifier && !strncmp(identifier, "progs/", 6))
//		{ alpha = 0; }

	if(!imgbuf && !data)
	{
		return(-1);
	}

	glt = &gltextures[numgltextures];
	numgltextures++;

	strcpy (glt->identifier, identifier);
	glt->texnum = texture_extension_number;
	glt->width = width;
	glt->height = height;
	glt->mipmap = mipmap;

	GL_Bind( texture_extension_number );

	if(imgbuf)
	{
		GL_UploadCompressed (imgbuf, width, height, mipmap, alpha);

		free(imgbuf);
	}
	else
	{
		GL_Upload8 (data, width, height, mipmap, alpha);
	}

	gl_savetexname = NULL;

	texture_extension_number++;

	return texture_extension_number-1;
}

/*
================
GL_LoadTexture32
================
*/
int GL_LoadTexture32 (char *identifier,
	int width, int height, byte *data,
	qboolean mipmap, qboolean alpha)
{
	u64 h;
	char		tname[256], tid1[64];
	qboolean	noalpha;
	byte		*imgbuf;
	char		*cs;
	int			i, p, s, imgbufsz;
	gltexture_t	*glt;

	// see if the texture is allready present
	if (identifier[0])
	{
		for (i=0, glt=gltextures ; i<numgltextures ; i++, glt++)
		{
			if (!strcmp (identifier, glt->identifier))
			{
				if (width != glt->width || height != glt->height)
					Sys_Error ("GL_LoadTexture: cache mismatch");
				return gltextures[i].texnum;
			}
		}
	}
	else {
//		glt = &gltextures[numgltextures];
//		numgltextures++;
	}

	gl_savetexname = NULL;
	imgbuf = NULL;

	if(identifier && (strlen(identifier)>2))
	{
		strcpy(tid1, identifier);
		cs=tid1;
		while(*cs)
		{
			i=*cs;
			if((i>='a') && (i<='z'))
				{ cs++; continue; }
			if((i>='A') && (i<='Z'))
				{ cs++; continue; }
			if((i>='0') && (i<='9'))
				{ cs++; continue; }
			if((i=='_') || (i=='-'))
				{ cs++; continue; }
			*cs++='_';
		}
		
		if(strlen(tid1)>12)
		{
			cs=identifier; h=0;
			while(*cs)
				{ h=(h*65521)+(*cs++); }
			i=4;
			while(i--)
				{ h=(h*65521)+1; }
			i=(h>>16)&0x7FFFFFFFU;
			sprintf(tid1, "x%08x", i);
		}
		
		imgbufsz=0;
//		sprintf(tname, "texcache/%s.lmp", tid1);
		sprintf(tname, "texcache/%s.dds", tid1);
//		imgbuf = COM_LoadFileSz(tname, 1, &imgbufsz);
		imgbuf = COM_LoadFileSz(tname, 5, &imgbufsz);
#ifndef __BJX2__
		gl_savetexname = tname;
#endif
	}

//	if(identifier && !strncmp(identifier, "progs/", 6))
//		{ alpha = 0; }

	if(!imgbuf && !data)
	{
		return(-1);
	}

	glt = &gltextures[numgltextures];
	numgltextures++;

	strcpy (glt->identifier, identifier);
	glt->texnum = texture_extension_number;
	glt->width = width;
	glt->height = height;
	glt->mipmap = mipmap;

	GL_Bind( texture_extension_number );

	if(imgbuf)
	{
		GL_UploadCompressed (imgbuf, width, height, mipmap, alpha);

		free(imgbuf);
	}
	else
	{
		GL_Upload32 (data, width, height, mipmap, alpha);
	}

	gl_savetexname = NULL;

	texture_extension_number++;

	return texture_extension_number-1;
}

/*
================
GL_LoadPicTexture
================
*/
int GL_LoadPicTexture (qpic_t *pic, char *name)
{
//	return GL_LoadTexture ("",
	return GL_LoadTexture (name,
		pic->width, pic->height, pic->data,
		false, true);
}

/****************************************/

static GLenum oldtarget = TEXTURE0_SGIS;

void GL_SelectTexture (GLenum target) 
{
	if (!gl_mtexable)
		return;
	qglSelectTextureSGIS(target);
	if (target == oldtarget) 
		return;
	cnttextures[oldtarget-TEXTURE0_SGIS] = currenttexture;
	currenttexture = cnttextures[target-TEXTURE0_SGIS];
	oldtarget = target;
}
