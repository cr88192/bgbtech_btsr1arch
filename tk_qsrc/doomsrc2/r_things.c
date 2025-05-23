// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Refresh of things, i.e. objects represented by sprites.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: r_things.c,v 1.5 1997/02/03 16:47:56 b1 Exp $";


#include <stdio.h>
#include <stdlib.h>


#include "doomdef.h"
#include "m_swap.h"

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "r_local.h"

#include "doomstat.h"



#define MINZ				(FRACUNIT*4)
#define BASEYCENTER			100

//void R_DrawColumn (void);
//void R_DrawFuzzColumn (void);



typedef struct
{
	int		x1;
	int		x2;
	
	int		column;
	int		topclip;
	int		bottomclip;

} maskdraw_t;



//
// Sprite rotation 0 is facing the viewer,
//  rotation 1 is one angle turn CLOCKWISE around the axis.
// This is not the same as the angle,
//  which increases counter clockwise (protractor).
// There was a lot of stuff grabbed wrong, so I changed it...
//
fixed_t		pspritescale;
fixed_t		pspriteiscale;

lighttable_t**	spritelights;

// constant arrays
//  used for psprite clipping and initializing clipping
short		negonearray[SCREENWIDTH+64];
short		screenheightarray[SCREENWIDTH+64];


//
// INITIALIZATION FUNCTIONS
//

// variables used to look up
//  and range check thing_t sprites patches
spritedef_t*	sprites;
int		numsprites;

spriteframe_t	sprtemp[29];
int		maxframe;
char*		spritename;




//
// R_InstallSpriteLump
// Local function for R_InitSprites.
//
void
R_InstallSpriteLump
( int		lump,
  unsigned	frame,
  unsigned	rotation,
  boolean	flipped )
{
	int		r;
	
	if (frame >= 29 || rotation > 8)
		I_Error("R_InstallSpriteLump: "
			"Bad frame characters in lump %i", lump);
	
	if ((int)frame > maxframe)
	maxframe = frame;
		
	if (rotation == 0)
	{
		// the lump should be used for all rotations
		if (sprtemp[frame].rotate == false)
			I_Error ("R_InitSprites: Sprite %s frame %c has "
				 "multip rot=0 lump", spritename, 'A'+frame);

		if (sprtemp[frame].rotate == true)
			I_Error ("R_InitSprites: Sprite %s frame %c has rotations "
				 "and a rot=0 lump", spritename, 'A'+frame);
				
		sprtemp[frame].rotate = false;
		for (r=0 ; r<8 ; r++)
		{
			sprtemp[frame].lump[r] = lump - firstspritelump;
			sprtemp[frame].flip[r] = (byte)flipped;
		}
		return;
	}
	
	// the lump is only used for one rotation
	if (sprtemp[frame].rotate == false)
		I_Error ("R_InitSprites: Sprite %s frame %c has rotations "
			 "and a rot=0 lump", spritename, 'A'+frame);
		
	sprtemp[frame].rotate = true;

	// make 0 based
	rotation--;		
	if (sprtemp[frame].lump[rotation] != -1)
		I_Error ("R_InitSprites: Sprite %s : %c : %c "
			 "has two lumps mapped to it",
			 spritename, 'A'+frame, '1'+rotation);
		
	sprtemp[frame].lump[rotation] = lump - firstspritelump;
	sprtemp[frame].flip[rotation] = (byte)flipped;
}




//
// R_InitSpriteDefs
// Pass a null terminated list of sprite names
//  (4 chars exactly) to be used.
// Builds the sprite rotation matrixes to account
//  for horizontally flipped sprites.
// Will report an error if the lumps are inconsistant. 
// Only called at startup.
//
// Sprite lump names are 4 characters for the actor,
//  a letter for the frame, and a number for the rotation.
// A sprite that is flippable will have an additional
//  letter/number appended.
// The rotation character can be 0 to signify no rotations.
//
void R_InitSpriteDefs (char** namelist) 
{ 
	char	**check;
	char	*s;
	int		i, n;
	int		l;
	int		intname;
	int		frame;
	int		rotation, rotn1;
	int		start;
	int		end;
	int		patched;
		
	// count the number of sprite names
	check = namelist;
//	while (*check != NULL)
//	{
//		check++;
//	}
//	numsprites = check-namelist;

	for(i=0; i<8192; i++)
	{
		s=namelist[i];
		if(!s)
			break;
	}
	numsprites=i;


//	__debugbreak();
	
	if (!numsprites)
		return;
	
	printf("R_InitSpriteDefs: numsprites=%d\n", numsprites);
	
//	sprites = Z_Malloc(numsprites *sizeof(*sprites), PU_STATIC, NULL);
	sprites = Z_Malloc((numsprites+16) *sizeof(*sprites), PU_STATIC, NULL);
	
	start = firstspritelump-1;
	end = lastspritelump+1;
	
	// scan all the lump names for each of the names,
	//  noting the highest frame letter.
	// Just compare 4 characters as ints
	for (i=0 ; i<numsprites ; i++)
	{
		spritename = namelist[i];
		memset (sprtemp,-1, sizeof(sprtemp));
		
		if(!spritename)
			__debugbreak();
		
		maxframe = -1;
//		intname = *(int *)(namelist[i]);
		intname = *(int *)(spritename);
	//	intname = (nlint)(namelist[i]);
		
		// scan the lumps,
		//  filling in the frames for whatever is found
		for (l=start+1 ; l<end ; l++)
		{
			if (*(int *)(lumpinfo[l].name) == intname)
			{
				frame = lumpinfo[l].name[4] - 'A';
				rotation = lumpinfo[l].name[5] - '0';

				patched = W_GetNumForName (lumpinfo[l].name);

//				if (modifiedgame)
//					patched = W_GetNumForName (lumpinfo[l].name);
//				else
//					patched = l;

				R_InstallSpriteLump (patched, frame, rotation, false);

				if (lumpinfo[l].name[6])
				{
					frame = lumpinfo[l].name[6] - 'A';
					rotation = lumpinfo[l].name[7] - '0';
					R_InstallSpriteLump (l, frame, rotation, true);
				}
			}
		}
		
		// check the frames that were found for completeness
		if (maxframe == -1)
		{
			sprites[i].numframes = 0;
			continue;
		}
			
		maxframe++;
		
		for (frame = 0 ; frame < maxframe ; frame++)
		{
			switch ((int)sprtemp[frame].rotate)
			{
			case -1:
				// no rotations were found for that frame at all
				I_Error ("R_InitSprites: No patches found "
					 "for %s frame %c", namelist[i], frame+'A');
				break;
			
			case 0:
				// only the first rotation is needed
				break;
				
			case 1:
				// must have all 8 frames
				for (rotation=0 ; rotation<8 ; rotation++)
					if (sprtemp[frame].lump[rotation] == -1)
				{
					if(rotation>=4)
						rotn1 = (rotation+1)&7;
					else
						rotn1 = (rotation-1)&7;
				
					if((rotation&1) &&
						(sprtemp[frame].lump[rotn1]>0))
					{
						//BGB: Allow 4-angle sprites.
						sprtemp[frame].lump[rotation]=
							sprtemp[frame].lump[rotn1];
						sprtemp[frame].flip[rotation]=
							sprtemp[frame].flip[rotn1];
						continue;
					}
					
					I_Error ("R_InitSprites: Sprite %s frame %c "
						 "is missing rotations",
						 namelist[i], frame+'A');
				}
				break;
			}
		}
		
		// allocate space for the frames present and copy sprtemp to it
		sprites[i].numframes = maxframe;
		sprites[i].spriteframes = 
			Z_Malloc ((maxframe+4) * sizeof(spriteframe_t), PU_STATIC, NULL);
		memcpy (sprites[i].spriteframes, sprtemp,
			maxframe*sizeof(spriteframe_t));
	}

}




//
// GAME FUNCTIONS
//
vissprite_t	vissprites[MAXVISSPRITES];
vissprite_t*	vissprite_p;
int		newvissprite;



//
// R_InitSprites
// Called at program start.
//
void R_InitSprites (char** namelist)
{
	int		i;
	
	for (i=0 ; i<SCREENWIDTH ; i++)
	{
		negonearray[i] = -1;
	}
	
	R_InitSpriteDefs (namelist);
}



//
// R_ClearSprites
// Called at frame start.
//
void R_ClearSprites (void)
{
	vissprite_p = vissprites;
}


//
// R_NewVisSprite
//
vissprite_t	overflowsprite;

vissprite_t* R_NewVisSprite (void)
{
	if (vissprite_p == &vissprites[MAXVISSPRITES])
		return &overflowsprite;
	
	vissprite_p++;
	return vissprite_p-1;
}



//
// R_DrawMaskedColumn
// Used for sprites and masked mid textures.
// Masked means: partly transparent, i.e. stored
//  in posts/runs of opaque pixels.
//
short*		mfloorclip;
short*		mceilingclip;

fixed_t		spryscale;
fixed_t		sprtopscreen;

void R_DrawMaskedColumn (column_t* column)
{
	int		topscreen;
	int 	bottomscreen;
	fixed_t	basetexturemid;
	
	basetexturemid = dc_texturemid;
	
	for ( ; column->topdelta != 0xff ; ) 
	{
		// calculate unclipped screen coordinates
		//  for post
		topscreen = sprtopscreen + spryscale*column->topdelta;
		bottomscreen = topscreen + spryscale*column->length;

		dc_yl = (topscreen+FRACUNIT-1)>>FRACBITS;
		dc_yh = (bottomscreen-1)>>FRACBITS;
			
		if (dc_yh >= mfloorclip[dc_x])
			dc_yh = mfloorclip[dc_x]-1;
		if (dc_yl <= mceilingclip[dc_x])
			dc_yl = mceilingclip[dc_x]+1;

		if (dc_yl <= dc_yh)
		{
			dc_source = (byte *)column + 3;
			dc_texturemid = basetexturemid - (column->topdelta<<FRACBITS);
			// dc_source = (byte *)column + 3 - column->topdelta;

			// Drawn by either R_DrawColumn
			//  or (SHADOW) R_DrawFuzzColumn.
			colfunc ();	
		}
		column = (column_t *)(  (byte *)column + column->length + 4);
	}
	
	dc_texturemid = basetexturemid;
}

#ifdef UTXSPRITE

void R_DrawMaskedColumnUtx (u64 *blka, int len)
{
	int		topscreen;
	int 	bottomscreen;
	fixed_t	basetexturemid;
	
	basetexturemid = dc_texturemid;
	
	topscreen = sprtopscreen + spryscale*0;
	bottomscreen = topscreen + spryscale*len;

	dc_yl = (topscreen+FRACUNIT-1)>>FRACBITS;
	dc_yh = (bottomscreen-1)>>FRACBITS;
		
	if (dc_yh >= mfloorclip[dc_x])
		dc_yh = mfloorclip[dc_x]-1;
	if (dc_yl <= mceilingclip[dc_x])
		dc_yl = mceilingclip[dc_x]+1;

	if (dc_yl <= dc_yh)
	{
		dc_source = (byte *)blka;
		dc_texturemid = basetexturemid - (0<<FRACBITS);
//		colfunc ();	
		wallcolfunc ();	
	}
	
	dc_texturemid = basetexturemid;
}

void
R_DrawVisSprite
( vissprite_t*		vis,
  int			x1,
  int			x2 )
{
	u64			*blka;
	column_t	*column;
	int			texturecolumn;
	fixed_t		frac;
	patch_t		*patch;
	int			k, l, ys1;
	
	
	patch = W_CachePatchNumUtx (vis->patch+firstspritelump, PU_CACHE);

	dc_colormap = vis->colormap;

	k = (dc_colormap - colormaps)>>8;
	l = cmap_luma[k];
	l = l << 8;

	if(l < 0x0000) l = 0x0000;
	if(l > 0xFFFF) l = 0xFFFF;
	dc_color = (((u64)l)<<0) | (((u64)l)<<16) |
		(((u64)l)<<32) | 0xFFFF000000000000ULL;
	if(k==32)
	{
		dc_color = 0xFFFFAAAA55551111ULL;
	}

	if (!dc_colormap)
	{
		// NULL colormap = shadow draw
		colfunc = fuzzcolfunc;
	}

	dc_colormap = R_ColormapRemapForBlend(dc_colormap);

	dc_iscale = abs(vis->xiscale)>>detailshift;
	dc_texturemid = vis->texturemid;
	frac = vis->startfrac;
	spryscale = vis->scale;
    dc_scale = spryscale;
    dc_zdist = vis->tz;
	sprtopscreen = centeryfrac - FixedMul(dc_texturemid,spryscale);
	dc_isspr = 1;
	
//	ys1=(patch->height+3)>>2;
	ys1=(patch->height+7)>>2;
	blka = (u64 *)(patch+1);
	
	for (dc_x=vis->x1 ; dc_x<=vis->x2 ; dc_x++, frac += vis->xiscale)
	{
		texturecolumn = frac>>FRACBITS;
		if(texturecolumn >= (patch->width))
			break;
		if (texturecolumn < 0)
			break;
		k = texturecolumn >> 2;
		
		dc_col = texturecolumn;
		R_DrawMaskedColumnUtx(blka+(k*ys1), patch->height);
	}

	colfunc = basecolfunc;
}

#else
//
// R_DrawVisSprite
//  mfloorclip and mceilingclip should also be set.
//
void
R_DrawVisSprite
( vissprite_t*		vis,
  int			x1,
  int			x2 )
{
	column_t*		column;
	int			texturecolumn;
	fixed_t		frac;
	patch_t*		patch;
	int			k, l;
	
	
//	patch = W_CacheLumpNum (vis->patch+firstspritelump, PU_CACHE);
	patch = W_CachePatchNum (vis->patch+firstspritelump, PU_CACHE);

	dc_colormap = vis->colormap;

	if(dc_colormap)
	{
		k = (dc_colormap - colormaps)>>8;
		l = cmap_luma[k];
		l = l << 8;

		if(l < 0x0000) l = 0x0000;
		if(l > 0xFFFF) l = 0xFFFF;
		dc_color = (((u64)l)<<0) | (((u64)l)<<16) |
			(((u64)l)<<32) | 0xFFFF000000000000ULL;
		if(k==32)
		{
			dc_color = 0xFFFFAAAA55551111ULL;
		}
	}else
	{
		dc_color = 0x3FFFAAAA55551111ULL;
	}

	if (!dc_colormap)
	{
		// NULL colormap = shadow draw
		colfunc = fuzzcolfunc;
	}
	else if (vis->mobjflags & MF_TRANSLATION)
	{
		colfunc = R_DrawTranslatedColumn;
		dc_translation = translationtables - 256 +
			( (vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT-8) );
	}

	dc_colormap = R_ColormapRemapForBlend(dc_colormap);

	dc_iscale = abs(vis->xiscale)>>detailshift;
	dc_texturemid = vis->texturemid;
	frac = vis->startfrac;
	spryscale = vis->scale;
    dc_scale = spryscale;
    dc_zdist = vis->tz;
	sprtopscreen = centeryfrac - FixedMul(dc_texturemid,spryscale);
	dc_isspr = 1;
	
	for (dc_x=vis->x1 ; dc_x<=vis->x2 ; dc_x++, frac += vis->xiscale)
	{
		texturecolumn = frac>>FRACBITS;
//		if(texturecolumn >= SHORT(patch->width))
		if(texturecolumn >= (patch->width))
			break;
		if (texturecolumn < 0)
			break;

//#ifdef RANGECHECK
//		if (texturecolumn < 0 || texturecolumn >= SHORT(patch->width))
//			I_Error ("R_DrawSpriteRange: bad texturecolumn");
//#endif

		column = (column_t *) ((byte *)patch +
//					   LONG(patch->columnofs[texturecolumn]));
					   (patch->columnofs[texturecolumn]));
		R_DrawMaskedColumn (column);
	}

	colfunc = basecolfunc;
}

#endif


int lightlevel;

lighttable_t *R_ColormapForLump(int lump, int lvl);

//
// R_ProjectSprite
// Generates a vissprite for a thing
//  if it might be visible.
//
void R_ProjectSprite (mobj_t* thing)
{
	fixed_t		tr_x;
	fixed_t		tr_y;
	
	fixed_t		gxt;
	fixed_t		gyt;
	
	fixed_t		tx;
	fixed_t		tz;

	fixed_t		xscale;
	
	int			x1;
	int			x2;

	lighttable_t *tcol;

	spritedef_t*	sprdef;
	spriteframe_t*	sprframe;
	int			lump;
	
	unsigned		rot;
	boolean		flip;
	
	int			index;

	vissprite_t*	vis;
	
	angle_t		ang;
	fixed_t		iscale;

//	puts("R_ProjectSprite: A0\n");

#if 0
//	if((thing->spawnpoint.type>=0x9300) &&
//		(thing->spawnpoint.type<=0x9303))
	if((thing->type>=MT_POLYOBJ0) &&
		(thing->type<=MT_POLYOBJ3))
	{
		R_ProjectSprite_PolyObj(thing);
		return;
	}
#endif
	
	// transform the origin point
	tr_x = thing->x - viewx;
	tr_y = thing->y - viewy;
	
	gxt = FixedMul(tr_x,viewcos); 
	gyt = -FixedMul(tr_y,viewsin);
	
	tz = gxt-gyt; 

	// thing is behind view plane?
	if (tz < MINZ)
		return;
	
//	xscale = FixedDiv(projection, tz);
	xscale = FixedDivSoft(projection, tz);
	
	gxt = -FixedMul(tr_x,viewsin); 
	gyt = FixedMul(tr_y,viewcos); 
	tx = -(gyt+gxt); 

	// too far off the side?
	if (abs(tx)>(tz<<2))
		return;

//	puts("R_ProjectSprite: A1\n");

	// decide which patch to use for sprite relative to player
#ifdef RANGECHECK
	if ((unsigned)thing->sprite >= numsprites)
	{
//		I_Error ("R_ProjectSprite: invalid sprite number %i ",
		printf ("R_ProjectSprite: invalid sprite number %i\n",
			thing->sprite);
		return;
	}
#endif
	sprdef = &sprites[thing->sprite];

	if(sprdef->numframes<=0)
		return;

//	puts("R_ProjectSprite: A2\n");

#ifdef RANGECHECK
	if ( (thing->frame&FF_FRAMEMASK) >= sprdef->numframes )
	{
//		I_Error ("R_ProjectSprite: invalid sprite frame %i : %i ",
		printf ("R_ProjectSprite: invalid sprite frame %i : %i\n",
			thing->sprite, thing->frame);
		return;
	}
#endif
	sprframe = &sprdef->spriteframes[ thing->frame & FF_FRAMEMASK];

	if (sprframe->rotate)
	{
		// choose a different rotation based on player view
		ang = R_PointToAngle (thing->x, thing->y);
		rot = (ang-thing->angle+(unsigned)(ANG45/2)*9)>>29;
		lump = sprframe->lump[rot];
		flip = (boolean)sprframe->flip[rot];
	}
	else
	{
		// use single rotation for all views
		lump = sprframe->lump[0];
		flip = (boolean)sprframe->flip[0];
	}
	
	R_CheckSpriteLump(lump);
	
	// calculate edges of the shape
	tx -= spriteoffset[lump];	
	x1 = (centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS;

	// off the right side?
	if (x1 > viewwidth)
		return;

//	puts("R_ProjectSprite: A3\n");

	tx +=  spritewidth[lump];
	x2 = ((centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS) - 1;

	// off the left side
	if (x2 < 0)
		return;
	
//	puts("R_ProjectSprite: A4\n");

	// store information in a vissprite
	vis = R_NewVisSprite ();
	vis->mobjflags = thing->flags;
	vis->scale = xscale<<detailshift;
	vis->gx = thing->x;
	vis->gy = thing->y;
	vis->gz = thing->z;
	vis->gzt = thing->z + spritetopoffset[lump];
	vis->texturemid = vis->gzt - viewz;
	vis->x1 = x1 < 0 ? 0 : x1;
	vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;
	vis->tz = tz;
//	iscale = FixedDiv (FRACUNIT, xscale);
	iscale = FixedDivSoft (FRACUNIT, xscale);

	if (flip)
	{
		vis->startfrac = spritewidth[lump]-1;
		vis->xiscale = -iscale;
	}
	else
	{
		vis->startfrac = 0;
		vis->xiscale = iscale;
	}

	if (vis->x1 > x1)
		vis->startfrac += vis->xiscale*(vis->x1-x1);
	vis->patch = lump;
	
	// get light level
	if (thing->flags & MF_SHADOW)
	{
		// shadow draw
		vis->colormap = NULL;
	}
	else if (fixedcolormap)
	{
		tcol = R_ColormapForLump(lump, 0);
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// fixed map
			vis->colormap = fixedcolormap;
		}
	}
	else if (thing->frame & FF_FULLBRIGHT)
	{
		tcol = R_ColormapForLump(lump, 0);
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// full bright
			vis->colormap = colormaps;
		}
	}
	
	else
	{
		tcol = R_ColormapForLump(lump, lightlevel);
		
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// diminished light
			index = xscale>>(LIGHTSCALESHIFT-detailshift);

			if (index >= MAXLIGHTSCALE) 
				index = MAXLIGHTSCALE-1;

			vis->colormap = spritelights[index];
		}
	}	
}




//
// R_AddSprites
// During BSP traversal, this adds sprites by sector.
//
void R_AddSprites (sector_t* sec)
{
	mobj_t*		thing;
	int			lightnum;

	// BSP is traversed by subsector.
	// A sector might have been split into several
	//  subsectors during BSP building.
	// Thus we check whether its already added.
	if (sec->validcount == validcount)
		return;		

	// Well, now it will be done.
	sec->validcount = validcount;
	
	lightnum = (sec->lightlevel >> LIGHTSEGSHIFT)+extralight;

	if (lightnum < 0)		
	{
		spritelights = scalelight[0];
		lightlevel = 0;
	}
	else if (lightnum >= LIGHTLEVELS)
	{
		spritelights = scalelight[LIGHTLEVELS-1];
		lightlevel = LIGHTLEVELS-1;
	}
	else
	{
		spritelights = scalelight[lightnum];
		lightlevel = lightnum;
	}

	// Handle all things in sector.
	for (thing = sec->thinglist ; thing ; thing = thing->snext)
	{
		R_ProjectSprite (thing);
	}
}


//
// R_DrawPSprite
//
void R_DrawPSprite (pspdef_t* psp)
{
	fixed_t		tx;
	int			x1;
	int			x2;
	spritedef_t*	sprdef;
	spriteframe_t*	sprframe;
	lighttable_t	*tcol;
	int			lump;
	boolean		flip;
	vissprite_t*	vis;
	vissprite_t		avis;
		
	// decide which patch to use
#ifdef RANGECHECK
	if ( (unsigned)psp->state->sprite >= numsprites)
	I_Error ("R_ProjectSprite: invalid sprite number %i ",
		 psp->state->sprite);
#endif
	sprdef = &sprites[psp->state->sprite];

	if(sprdef->numframes<=0)
		return;

#ifdef RANGECHECK
	if ( (psp->state->frame & FF_FRAMEMASK)  >= sprdef->numframes)
	I_Error ("R_ProjectSprite: invalid sprite frame %i : %i ",
		 psp->state->sprite, psp->state->frame);
#endif
	sprframe = &sprdef->spriteframes[ psp->state->frame & FF_FRAMEMASK ];

	lump = sprframe->lump[0];
	flip = (boolean)sprframe->flip[0];
	
	// calculate edges of the shape
	tx = psp->sx-160*FRACUNIT;
	
	R_CheckSpriteLump(lump);
	
	tx -= spriteoffset[lump];	
	x1 = (centerxfrac + FixedMul (tx,pspritescale) ) >>FRACBITS;

	// off the right side
	if (x1 > viewwidth)
		return;		

	tx +=  spritewidth[lump];
	x2 = ((centerxfrac + FixedMul (tx, pspritescale) ) >>FRACBITS) - 1;

	// off the left side
	if (x2 < 0)
		return;
	
	// store information in a vissprite
	vis = &avis;
	vis->mobjflags = 0;
	vis->texturemid =
		(BASEYCENTER<<FRACBITS)+FRACUNIT/2-
			(psp->sy-spritetopoffset[lump]);
	vis->x1 = x1 < 0 ? 0 : x1;
	vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
	vis->scale	= pspritescale<<detailshift;
	vis->tz		= 0;
	
	if (flip)
	{
		vis->xiscale = -pspriteiscale;
		vis->startfrac = spritewidth[lump]-1;
	}
	else
	{
		vis->xiscale = pspriteiscale;
		vis->startfrac = 0;
	}
	
	if (vis->x1 > x1)
		vis->startfrac += vis->xiscale*(vis->x1-x1);

	vis->patch = lump;

	if (viewplayer->powers[pw_invisibility] > 4*32
	|| viewplayer->powers[pw_invisibility] & 8)
	{
		// shadow draw
		vis->colormap = NULL;
	}
	else if (fixedcolormap)
	{
		tcol = R_ColormapForLump(lump, 0);
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// fixed color
			vis->colormap = fixedcolormap;
		}
	}
	else if (psp->state->frame & FF_FULLBRIGHT)
	{
		tcol = R_ColormapForLump(lump, 0);
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// full bright
			vis->colormap = colormaps;
		}
	}
	else
	{
		tcol = R_ColormapForLump(lump, lightlevel);
		if(tcol)
		{
			vis->colormap = tcol;
		}else
		{
			// local light
			vis->colormap = spritelights[MAXLIGHTSCALE-1];
		}
	}
	
	R_DrawVisSprite (vis, vis->x1, vis->x2);
}



//
// R_DrawPlayerSprites
//
void R_DrawPlayerSprites (void)
{
	int		i;
	int		lightnum;
	pspdef_t*	psp;
	
	if(!viewplayer)
		return;
	
	// get light level
	lightnum =
	(viewplayer->mo->subsector->sector->lightlevel >> LIGHTSEGSHIFT) 
	+extralight;

	if (lightnum < 0)		
	{
		spritelights = scalelight[0];
		lightlevel = 0;
	}
	else if (lightnum >= LIGHTLEVELS)
	{
		spritelights = scalelight[LIGHTLEVELS-1];
		lightlevel = LIGHTLEVELS-1;
	}
	else
	{
		spritelights = scalelight[lightnum];
		lightlevel = lightnum;
	}
	
	// clip to screen bounds
	mfloorclip = screenheightarray;
	mceilingclip = negonearray;
	
	// add all active psprites
	for (i=0, psp=viewplayer->psprites;
	 i<NUMPSPRITES;
	 i++,psp++)
	{
		if (psp->state)
			R_DrawPSprite (psp);
	}
}




//
// R_SortVisSprites
//
vissprite_t	vsprsortedhead;


void R_SortVisSprites (void)
{
	int			i, j;
	int			count;
	vissprite_t*	ds;
	vissprite_t*	best;
	vissprite_t*	spr;
	static vissprite_t		unsorted;
	fixed_t		bestscale;

	best = NULL;
	count = vissprite_p - vissprites;
	
	unsorted.next = unsorted.prev = &unsorted;

	if (!count)
	{
//		puts("R_SortVisSprites: No Sprites\n");
		return;
	}

#ifdef __riscv
//		__debugbreak();
#endif

	for (ds=vissprites ; ds<vissprite_p ; ds++)
	{
		ds->next = ds+1;
		if(ds!=vissprites)
			ds->prev = ds-1;
	}

#if 0
	vissprites[0].prev = &vsprsortedhead;
	vsprsortedhead.next = &vissprites[0];
	(vissprite_p-1)->next = &vsprsortedhead;
	vsprsortedhead.prev = vissprite_p-1;

	for (i=0 ; i<count ; i++)
	{
		for (spr = vsprsortedhead.next ;
			 spr != &vsprsortedhead ;
			 spr=spr->next)
		{
			ds=spr->next;
			if(ds==&vsprsortedhead)
				break;

//			j=ds->scale-spr->scale;
			if(ds->scale<spr->scale)
//			if(j<0)
			{
				spr->prev->next=ds;
				ds->next->prev=spr;
				ds->prev=spr->prev;
				spr->next=ds->next;
				spr->prev=ds;
				ds->next=spr;
				
				spr=ds;
			}
#ifdef __riscv
			__debugbreak();
#endif
		}
	}
#endif

#if 1
	vissprites[0].prev = &unsorted;
	unsorted.next = &vissprites[0];
	(vissprite_p-1)->next = &unsorted;
	unsorted.prev = vissprite_p-1;
	
	// pull the vissprites out by scale
	//best = 0;		// shut up the compiler warning
	vsprsortedhead.next = vsprsortedhead.prev = &vsprsortedhead;
	for (i=0 ; i<count ; i++)
	{
		bestscale = MAXINT;
//		bestscale = 1999999;
		best=NULL;
		
		if(bestscale<0)
			{ __debugbreak(); }
		
		for (ds=unsorted.next ; ds!= &unsorted ; ds=ds->next)
		{
			if (ds->scale < bestscale)
			{
				bestscale = ds->scale;
				best = ds;
			}
		}
		
		if(!best)
		{
			__debugbreak();
		}
		
		best->next->prev = best->prev;
		best->prev->next = best->next;
		best->next = &vsprsortedhead;
		best->prev = vsprsortedhead.prev;
		vsprsortedhead.prev->next = best;
		vsprsortedhead.prev = best;
	}
#endif
}



//
// R_DrawSprite
//
void R_DrawSprite (vissprite_t* spr)
{
	drawseg_t*		ds;
	short		clipbot[SCREENWIDTH];
	short		cliptop[SCREENWIDTH];
	int			x;
	int			r1, x1;
	int			r2, x2;
	fixed_t		scale;
	fixed_t		lowscale;
	int			silhouette;
	
//	printf("R_DrawSprite A0 %p\n", spr);
	
	x1=spr->x1;
	x2=spr->x2;
	
	if(x1<0)x1=0;
	if(x2<0)
		return;
	if(x1>=SCREENWIDTH)
		return;
	if(x2>=SCREENWIDTH)
		x2=SCREENWIDTH-1;
	
//	printf("R_DrawSprite A1 %p\n", spr);
	
//	for (x = spr->x1 ; x<=spr->x2 ; x++)
	for (x = x1 ; x<=x2 ; x++)
		clipbot[x] = cliptop[x] = -2;
	
	// Scan drawsegs from end to start for obscuring segs.
	// The first drawseg that has a greater scale
	//  is the clip seg.
	for (ds=ds_p-1 ; ds >= drawsegs ; ds--)
	{
		// determine if the drawseg obscures the sprite
		if (ds->x1 > spr->x2
			|| ds->x2 < spr->x1
			|| (!ds->silhouette
			&& !ds->maskedtexturecol) )
		{
			// does not cover sprite
			continue;
		}
				
//		r1 = ds->x1 < spr->x1 ? spr->x1 : ds->x1;
//		r2 = ds->x2 > spr->x2 ? spr->x2 : ds->x2;

		r1 = ds->x1 < x1 ? x1 : ds->x1;
		r2 = ds->x2 > x2 ? x2 : ds->x2;

		if (ds->scale1 > ds->scale2)
		{
			lowscale = ds->scale2;
			scale = ds->scale1;
		}
		else
		{
			lowscale = ds->scale1;
			scale = ds->scale2;
		}
			
		if (scale < spr->scale
			|| ( lowscale < spr->scale
			 && !R_PointOnSegSide (spr->gx, spr->gy, ds->curline) ) )
		{
			// masked mid texture?
			if (ds->maskedtexturecol)
			{	
//				dc_isspr = 1;
				R_RenderMaskedSegRange (ds, r1, r2);
			}
			// seg is behind sprite
			continue;			
		}

		
		// clip this piece of the sprite
		silhouette = ds->silhouette;
		
		if (spr->gz >= ds->bsilheight)
			silhouette &= ~SIL_BOTTOM;

		if (spr->gzt <= ds->tsilheight)
			silhouette &= ~SIL_TOP;
				
		if (silhouette == 1)
		{
			// bottom sil
			for (x=r1 ; x<=r2 ; x++)
			if (clipbot[x] == -2)
				clipbot[x] = ds->sprbottomclip[x];
		}
		else if (silhouette == 2)
		{
			// top sil
			for (x=r1 ; x<=r2 ; x++)
			if (cliptop[x] == -2)
				cliptop[x] = ds->sprtopclip[x];
		}
		else if (silhouette == 3)
		{
			// both
			for (x=r1 ; x<=r2 ; x++)
			{
			if (clipbot[x] == -2)
				clipbot[x] = ds->sprbottomclip[x];
			if (cliptop[x] == -2)
				cliptop[x] = ds->sprtopclip[x];
			}
		}
		
	}
	
	// all clipping has been performed, so draw the sprite

	// check for unclipped columns
//	for (x = spr->x1 ; x<=spr->x2 ; x++)
	for (x = x1 ; x<=x2 ; x++)
	{
	if (clipbot[x] == -2)		
		clipbot[x] = viewheight;

	if (cliptop[x] == -2)
		cliptop[x] = -1;
	}
		
	mfloorclip = clipbot;
	mceilingclip = cliptop;

#if 0
	// clip to screen bounds
	mfloorclip = screenheightarray;
	mceilingclip = negonearray;
#endif

//	R_DrawVisSprite (spr, spr->x1, spr->x2);
	R_DrawVisSprite (spr, x1, x2);
}




//
// R_DrawMasked
//
void R_DrawMasked (void)
{
	vissprite_t*	spr;
	drawseg_t*		ds;
	
	R_SortVisSprites ();

	if (vissprite_p > vissprites)
	{
#if 0
		for (spr=vissprites ; spr<vissprite_p ; spr++)
		{
			R_DrawSprite (spr);
		}
#endif

#if 1
		// draw all vissprites back to front
		for (spr = vsprsortedhead.next ;
			 spr != &vsprsortedhead ;
			 spr=spr->next)
		{
			
			R_DrawSprite (spr);
		}
#endif
	}
	
	// render any remaining masked mid textures
	for (ds=ds_p-1 ; ds >= drawsegs ; ds--)
	if (ds->maskedtexturecol)
		R_RenderMaskedSegRange (ds, ds->x1, ds->x2);
	
	// draw the psprites on top of everything
	//  but does not draw on side views
	if (!viewangleoffset)		
		R_DrawPlayerSprites ();
}



