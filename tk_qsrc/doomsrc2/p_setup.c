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
//	Do all the WAD I/O, get map description,
//	set up initial state and misc. LUTs.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: p_setup.c,v 1.5 1997/02/03 22:45:12 b1 Exp $";


#include <math.h>

#include "z_zone.h"

#include "m_swap.h"
#include "m_bbox.h"

#include "g_game.h"

#include "i_system.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

#include "doomstat.h"


// void	P_SpawnMapThing (mapthing_t*	mthing);
void	P_SpawnMapThing (mapthing2_t*	mthing);


//
// MAP related Lookup tables.
// Store VERTEXES, LINEDEFS, SIDEDEFS, etc.
//
int		numvertexes;
vertex_t*	vertexes;

int		numsegs;
seg_t*		segs;

int		numsectors;
sector_t*	sectors;

int		numsubsectors;
subsector_t*	subsectors;

int		numnodes;
node_t*		nodes;

int		numlines;
line_t*		lines;

line_t*		fakelines;		//BGB: Fake Lines, for ACS stuff

int		numsides;
side_t*		sides;


// BLOCKMAP
// Created from axis aligned bounding box
// of the map, a rectangular array of
// blocks of size ...
// Used to speed up collision detection
// by spatial subdivision in 2D.
//
// Blockmap size.
int		bmapwidth;
int		bmapheight;	// size in mapblocks
short*		blockmap;	// int for larger maps
// offsets in blockmap are from here
short*		blockmaplump;		
// origin of block map
fixed_t		bmaporgx;
fixed_t		bmaporgy;
// for thing chains
mobj_t**	blocklinks;		


// REJECT
// For fast sight rejection.
// Speeds up enemy AI by skipping detailed
//  LineOf Sight calculation.
// Without special effect, this could be
//  used as a PVS lookup as well.
//
byte*		rejectmatrix;


// Maintain single and multi player starting spots.
#define MAX_DEATHMATCH_STARTS	10

//mapthing_t	deathmatchstarts[MAX_DEATHMATCH_STARTS];
//mapthing_t*	deathmatch_p;
//mapthing_t	playerstarts[MAXPLAYERS];

mapthing2_t	deathmatchstarts[MAX_DEATHMATCH_STARTS];
mapthing2_t*	deathmatch_p;
mapthing2_t	playerstarts[MAXPLAYERS];

int p_map_hexenfmt;





//
// P_LoadVertexes
//
void P_LoadVertexes (int lump)
{
    byte*		data;
    int			i, lsz;
    mapvertex_t*	ml;
    vertex_t*		li;

	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapvertex_t))
		__debugbreak();

    // Determine number of lumps:
    //  total lump length / vertex record length.
    numvertexes = lsz / sizeof(mapvertex_t);

    // Allocate zone memory for buffer.
    vertexes = Z_Malloc (numvertexes*sizeof(vertex_t),PU_LEVEL,0);	

    // Load data into cache.
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    ml = (mapvertex_t *)data;
    li = vertexes;

    // Copy and convert vertex coordinates,
    // internal representation as fixed.
    for (i=0 ; i<numvertexes ; i++, li++, ml++)
    {
	li->x = SHORT(ml->x)<<FRACBITS;
	li->y = SHORT(ml->y)<<FRACBITS;
    }

    // Free buffer memory.
    Z_Free (data);
}



//
// P_LoadSegs
//
void P_LoadSegs (int lump)
{
    byte*		data;
    int			i, lsz;
    mapseg_t*		ml;
    seg_t*		li;
    line_t*		ldef;
    int			linedef;
    int			side;
	
	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapseg_t))
		__debugbreak();

    numsegs = lsz / sizeof(mapseg_t);
    segs = Z_Malloc (numsegs*sizeof(seg_t),PU_LEVEL,0);	
    memset (segs, 0, numsegs*sizeof(seg_t));
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    ml = (mapseg_t *)data;
    li = segs;
    for (i=0 ; i<numsegs ; i++, li++, ml++)
    {
	li->v1 = &vertexes[SHORT(ml->v1)];
	li->v2 = &vertexes[SHORT(ml->v2)];
					
	li->angle = (SHORT(ml->angle))<<16;
	li->offset = (SHORT(ml->offset))<<16;
	linedef = SHORT(ml->linedef);
	ldef = &lines[linedef];
	li->linedef = ldef;
	side = SHORT(ml->side);
	li->sidedef = &sides[ldef->sidenum[side]];
	li->frontsector = sides[ldef->sidenum[side]].sector;
	if (ldef-> flags & ML_TWOSIDED)
	    li->backsector = sides[ldef->sidenum[side^1]].sector;
	else
	    li->backsector = 0;
    }
	
    Z_Free (data);
}


//
// P_LoadSubsectors
//
void P_LoadSubsectors (int lump)
{
    byte*		data;
    int			i, lsz;
    mapsubsector_t*	ms;
    subsector_t*	ss;
	
	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapsubsector_t))
		__debugbreak();

    numsubsectors = lsz / sizeof(mapsubsector_t);
    subsectors = Z_Malloc (numsubsectors*sizeof(subsector_t),PU_LEVEL,0);	
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    ms = (mapsubsector_t *)data;
    memset (subsectors,0, numsubsectors*sizeof(subsector_t));
    ss = subsectors;
    
    for (i=0 ; i<numsubsectors ; i++, ss++, ms++)
    {
		ss->numlines = SHORT(ms->numsegs);
		ss->firstline = SHORT(ms->firstseg);
    }
	
    Z_Free (data);
}



//
// P_LoadSectors
//
void P_LoadSectors (int lump)
{
    byte*		data;
    int			i, lsz;
    mapsector_t*	ms;
    sector_t*		ss;
	
	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapsector_t))
		__debugbreak();

    numsectors = lsz / sizeof(mapsector_t);
    sectors = Z_Malloc (numsectors*sizeof(sector_t),PU_LEVEL,0);	
    memset (sectors, 0, numsectors*sizeof(sector_t));
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    ms = (mapsector_t *)data;
    ss = sectors;
    for (i=0 ; i<numsectors ; i++, ss++, ms++)
    {
		ss->floorheight = SHORT(ms->floorheight)<<FRACBITS;
		ss->ceilingheight = SHORT(ms->ceilingheight)<<FRACBITS;
		ss->floorpic = R_FlatNumForName(ms->floorpic);
		ss->ceilingpic = R_FlatNumForName(ms->ceilingpic);
		ss->lightlevel = SHORT(ms->lightlevel);
		ss->special = SHORT(ms->special);
		ss->tag = SHORT(ms->tag);
		ss->thinglist = NULL;
    }
	
    Z_Free (data);
}


//
// P_LoadNodes
//
void P_LoadNodes (int lump)
{
    byte*	data;
    int		i;
    int		j;
    int		k;
    int		lsz;
    mapnode_t*	mn;
    node_t*	no;
	
	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapnode_t))
		__debugbreak();

    numnodes = lsz / sizeof(mapnode_t);
    nodes = Z_Malloc (numnodes*sizeof(node_t),PU_LEVEL,0);	
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    mn = (mapnode_t *)data;
    no = nodes;
    
    for (i=0 ; i<numnodes ; i++, no++, mn++)
    {
	no->x = SHORT(mn->x)<<FRACBITS;
	no->y = SHORT(mn->y)<<FRACBITS;
	no->dx = SHORT(mn->dx)<<FRACBITS;
	no->dy = SHORT(mn->dy)<<FRACBITS;
	for (j=0 ; j<2 ; j++)
	{
	    no->children[j] = SHORT(mn->children[j]);
	    for (k=0 ; k<4 ; k++)
		no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
	}
    }
	
    Z_Free (data);
}


int P_SetupRemapThingType(int type)
{
	int ttype;
	
	ttype=type;
	
	if ( gamemode == hexen)
	{
		switch(ttype)
		{
		case 1:					break; //Player Spawn
		case 2:					break; //Player Spawn
		case 3:					break; //Player Spawn
		case 4:					break; //Player Spawn
		case 5:		ttype=10;	break;	//Winged Statue
		case 6:		ttype=29;	break;	//Rock
		case 7:		ttype=29;	break;	//Rock
		case 8:					break; //BagOfHolding -> Backpack
		case 9:		ttype=29;	break;	//Rock
		case 10:	ttype=2007;	break;	//Wand Ammo -> Clip
		case 11:				break;	//Deathmatch Start
		case 12:	ttype=5010;	break;	//Gold Wand -> Pistol
		case 13:	ttype=2048;	break;	//Mace Ammo
		case 14:				break;	//Teleport Dest
		case 15:	ttype=29;	break;	//Rock
		case 16:	ttype=2002;	break;	//Weapon Piece
		case 17:	ttype=35;	break;	//Chandalier
		case 18:	ttype=2049;	break;	//Weapon Piece
		case 19:	ttype=2001;	break;	//Weapon Piece
		case 20:	ttype=2047;	break;	//Weapon Piece
		case 21:	ttype=2004;	break;	//Weapon Piece
		case 22:	ttype=2046;	break;	//Weapon Piece
		case 23:	ttype=2003;	break;	//Weapon Piece

		case 24:	ttype=54;	break;	//Dead Tree
		case 25:	ttype=54;	break;	//Tree
		case 26:	ttype=54;	break;	//Swamp Tree
		case 27:	ttype=54;	break;	//Swamp Tree
		case 28:	ttype=43;	break;	//Burned Stump
		case 29:	ttype=47;	break;	//Bare Stump
		case 30:	ttype=10;	break;	//Artifact Pork

		case 31:	ttype=3001;	break;	//Demon

		case 34:	ttype=3005;	break;	//Wraith
		case 35:	ttype=2026;	break;	//Super Map

		case 37:	ttype=47;	break;	//Stump
		case 38:	ttype=47;	break;	//Stump
		case 39:	ttype=85;	break;	//Shroom Large
		case 40:	ttype=85;	break;	//Shroom Large
		case 41:	ttype=85;	break;	//Shroom Large
		case 42:	ttype=86;	break;	//Shroom Small
		case 43:	ttype=86;	break;	//Shroom Small
		case 44:	ttype=86;	break;	//Shroom Small
		case 45:	ttype=86;	break;	//Shroom Small
		case 46:	ttype=86;	break;	//Shroom Small
		case 47:	ttype=86;	break;	//Shroom Small
		case 48:	ttype=48;	break;	//Stalagmite Pillar
		case 49:	ttype=10;	break;	//Stalagmite Large
		case 50:	ttype=10;	break;	//Stalagmite Medium
		case 51:	ttype=10;	break;	//Stalagmite Small
		case 52:	ttype=10;	break;	//Stalactite Large
		case 53:	ttype=10;	break;	//Weapon Frost
		case 54:	ttype=34;	break;	//Wall Torch
		case 55:	ttype=34;	break;	//Wall Torch (Unlit)
		case 56:	ttype=10;	break;	//Stalactite Medium
		case 57:	ttype=10;	break;	//Stalactite Small
		case 58:	ttype=60;	break;	//Hanging Moss
		case 59:	ttype=60;	break;	//Hanging Moss
		case 60:	ttype=62;	break;	//Swamp Vine
		case 61:	ttype=26;	break;	//Corpse Kabob
		case 62:	ttype=15;	break;	//Corpse Sleeping
		case 63:	ttype=29;	break;	//Tombstone
		
		case 64:	ttype=29;	break;	//Tombstone
		case 65:	ttype=29;	break;	//Tombstone
		case 66:	ttype=29;	break;	//Tombstone
		case 68:	ttype=29;	break;	//Tombstone
		case 69:	ttype=20;	break;	//Tombstone
		case 70:	ttype=10;	break;	//?
		case 71:	ttype=49;	break;	//Hanging Corpse
		case 72:	ttype=10;	break;	//Gargoyle Statue
		case 73:	ttype=10;	break;	//Gargoyle Statue
		case 74:	ttype=10;	break;	//Gargoyle Statue
		case 75:	ttype=10;	break;	//?
		case 76:	ttype=50;	break;	//Hanging Corpse
		case 77:	ttype=10;	break;	//Tattered Banner
		case 78:	ttype=54;	break;	//Large Tree
		case 79:	ttype=54;	break;	//Large Tree
		case 80:	ttype=54;	break;	//Gnarled Tree
		case 81:	ttype=2014;	break;	//Crystal Vial
		case 82:	ttype=10;	break;	//?
		case 83:	ttype=10;	break;	//?

		case 84:	ttype=2022;	break;	//Invulnerability
		case 85:	ttype=2018;	break;	//Silver Sheild
		case 86:	ttype=2023;	break;	//Tome of Power
		case 87:	ttype=54;	break;	//Gnarled Tree
		case 88:	ttype=10;	break;	//Log
		case 89:	ttype=10;	break;	//Stalactite Large
		case 90:	ttype=10;	break;	//Stalactite Medium
		case 91:	ttype=48;	break;	//Stalagmiticle Small
		case 92:	ttype=48;	break;	//Stalagmiticle Tiny
		case 93:	ttype=48;	break;	//Stalagmiticle Large
		case 94:	ttype=48;	break;	//Stalagmiticle Medium
		case 95:	ttype=48;	break;	//Stalagmiticle Small
		case 96:	ttype=48;	break;	//Stalagmiticle Tiny
		case 97:	ttype=29;	break;	//Rock
		case 98:	ttype=29;	break;	//Rock
		case 99:	ttype=29;	break;	//Rock
		case 100:	ttype=29;	break;	//Rubble
		case 101:	ttype=29;	break;	//Rubble
		case 102:	ttype=29;	break;	//Rubble
		case 103:	ttype=37;	break;	//Vase Pillar
		case 104:	ttype=10;	break;	//Pottery
		case 105:	ttype=10;	break;	//Pottery
		case 106:	ttype=10;	break;	//Pottery

		case 107:	ttype=3004;	break;	//Centaur

		case 108:	ttype=51;	break;	//Lynched Corpse
		case 109:	ttype=49;	break;	//Lynched Corpse
		case 110:	ttype=18;	break;	//Sitting Corpse
		case 111:	ttype=10;	break;	//Blood Pool
		case 112:	ttype=10;	break;	//Little Fly
		case 113:	ttype=10;	break;	//Leaf Spawner

		case 114:	ttype=3004;	break;	//Bishop
		case 115:	ttype=3002;	break;	//Centaur Leader

		case 116:	ttype=34;	break;	//Twined Torch
		case 117:	ttype=34;	break;	//Twined Torch Unlit
		case 118:	ttype=10;	break;	//?
		case 119:	ttype=34;	break;	//Candle

		case 120:	ttype=3001;	break;	//Stalker Boss
		case 121:	ttype=3001;	break;	//Stalker

		case 122:	ttype=2048;	break;	//Mana
		case 123:	ttype=2046;	break;	//Hammer
		case 124:	ttype=2049;	break;	//Mana

		case 140:	ttype=10;	break;	//Tele Smoke

		case 254:	ttype=3004;	break;	//Dragon

		case 1410:	ttype=10;	break;	//Sound Wind

		case 2001:				break;	//Crossbow / Shotgun
		case 2002:				break;	//Mace / Chaingun
		case 2003:				break;	//Rocket Launcher / Pheonix Rod
		case 2004:				break;	//Plasma Gun / Skull Rod
		case 2005:				break;	//Chainsaw / Gauntlets

		case 2035:	ttype=2035;	break;	//Bell

		case 3000:	ttype=9300;	break;	//PolyObj
		case 3001:	ttype=9301;	break;	//PolyObj
		case 3002:	ttype=9302;	break;	//PolyObj
		case 3003:	ttype=9303;	break;	//PolyObj

		case 8004:	ttype=2046;	break;	//Combined Mana

		case 8020:	ttype=9;	break;	//Wendigo
		case 8080:	ttype=3001;	break;	//Chaos Serpent

		case 8065:	ttype=2035;	break;	//Bell

		case 8100:	ttype=2035;	break;	//Barrel

		case 10011:	ttype=71;	break;	//Reiver (Buried)
		case 10030:	ttype=3004;	break;	//Ettin
		case 10060:	ttype=3006;	break;	//Afrit
		case 10080:	ttype=3001;	break;	//Heresiarch
		case 10100:	ttype=3001;	break;	//Zedek
		case 10101:	ttype=3001;	break;	//Traductus
		case 10102:	ttype=3001;	break;	//Menelkir
		case 10200:	ttype=3001;	break;	//Korax

		default:
			ttype=10;
			break;
		}
	}else

	if ( gamemode == heretic)
	{
		switch(ttype)
		{
		case 1:						break; //Player Spawn
		case 2:						break; //Player Spawn
		case 3:						break; //Player Spawn
		case 4:						break; //Player Spawn
		case 5:		ttype=3001;	break;	//ImpLeader -> Imp
		case 6:		ttype=3004;	break;	//IronLich -> Baron
		case 7:		ttype=3003;	break;	//Sorcerer -> Baron Of Hell
		case 8:						break; //BagOfHolding -> Backpack
		case 9:		ttype=3002;	break;	//Minotaur -> Demon
		case 10:	ttype=2007;	break;	//Wand Ammo -> Clip
		case 11:					break;	//Deathmatch Start
		case 12:	ttype=5010;	break;	//Gold Wand -> Pistol
		case 13:	ttype=2048;	break;	//Mace Ammo
		case 15:					break;	//Teleport Dest
		case 16:	ttype=2002;	break;	//Mace
		case 17:	ttype=10;	break;	//Hanging Skull
		case 18:	ttype=2049;	break;	//Crossbow Ammo
		case 19:	ttype=2001;	break;	//Crossbow
		case 20:	ttype=2047;	break;	//Skull Rod Ammo
		case 21:	ttype=2004;	break;	//Skull Rod
		case 22:	ttype=2046;	break;	//Pheonix Rod Ammo
		case 23:	ttype=2003;	break;	//Pheonix Rod

		case 24:	ttype=10;	break;	//Hanging Skull
		case 25:	ttype=10;	break;	//Hanging Skull
		case 26:	ttype=10;	break;	//Hanging Skull
		case 27:	ttype=35;	break;	//Serpent Torch
		case 28:	ttype=10;	break;	//Chandelier
		case 29:	ttype=10;	break;	//Small Pillar
		case 30:	ttype=10;	break;	//Artifact Egg

		case 31:	ttype=10;	break;	//Enchanted Shield

		case 34:	ttype=10;	break;	//Artifact Time Bomb
		case 35:	ttype=2026;	break;	//Super Map

		case 37:	ttype=5050;	break;	//Stalagmite Small
		case 38:	ttype=5050;	break;	//Stalagmite Large
		case 39:	ttype=5050;	break;	//Stalactite Small
		case 40:	ttype=5050;	break;	//Stalactite Large
		case 41:	ttype=10;	break;	//Waterfall Sound
		case 42:	ttype=10;	break;	//Wind Sound
		case 43:	ttype=10;	break;	//Pod Generator
		case 44:	ttype=10;	break;	//Wooden Barrel
		case 45:	ttype=10;	break;	//Mummy Leader
		case 46:	ttype=10;	break;	//Mummy Leader Ghost
		case 47:	ttype=10;	break;	//Brown Pillar
		case 48:	ttype=10;	break;	//Moss
		case 49:	ttype=10;	break;	//Moss
		case 50:	ttype=10;	break;	//Wall Torch
		case 51:	ttype=10;	break;	//Hanging Corpse
		case 52:	ttype=10;	break;	//Teleport Glitter
		case 53:	ttype=10;	break;	//Blaster
		case 54:	ttype=2007;	break;	//Blaster Ammo
		case 55:	ttype=10;	break;	//Blaster
		case 56:	ttype=	0;	break;	//Boss Spot

		case 64:	ttype=3001;	break;	//Knight
		case 65:	ttype=3001;	break;	//Knight Ghost
		case 66:	ttype=3004;	break;	//Imp
		case 68:	ttype=3004;	break;	//Mummy
		case 69:	ttype=3004;	break;	//Mummy Ghost
		case 70:	ttype=3004;	break;	//Beast

		case 73:	ttype=  13;	break;	//Green Key

		case 75:	ttype=2024;	break;	//Invisibility Artifact
		case 76:	ttype=56;	break;	//Fire Brazier

		case 79:	ttype=	5;	break;	//Blue Key
		case 80:	ttype=	6;	break;	//Yellow Key

		case 84:	ttype=2022;	break;	//Invulnerability
		case 85:	ttype=2018;	break;	//Silver Sheild
		case 86:	ttype=2023;	break;	//Tome of Power
		case 87:	ttype=10;	break;	//Volcano

		case 90:	ttype=3002;	break;	//Clink

		case 92:	ttype=3001;	break;	//Snake

		case 94:	ttype=	5;	break;	//Blue Key
		case 95:	ttype=  13;	break;	//Green Key
		case 96:	ttype=	6;	break;	//Yellow Key

		case 2001:					break;	//Crossbow / Shotgun
		case 2002:					break;	//Mace / Chaingun
		case 2003:					break;	//Rocket Launcher / Pheonix Rod
		case 2004:					break;	//Plasma Gun / Skull Rod
		case 2005:					break;	//Chainsaw / Gauntlets

		default:
			ttype=10;
			break;
		}
	}
	
	return(ttype);
}

//
// P_LoadThings
//
void P_LoadThings (int lump)
{
    mapthing2_t	mtt;

    byte*		data;
    int			i, ttype;
    mapthing_t	*mt;
    mapthing2_t	*mt2;
    int			numthings;
    boolean		spawn;
	
    data = W_CacheLumpNum (lump,PU_STATIC);
    
    if(p_map_hexenfmt)
    {
		numthings = W_LumpLength (lump) / sizeof(mapthing2_t);
    }else
    {
		numthings = W_LumpLength (lump) / sizeof(mapthing_t);
    }
	
    mt = (mapthing_t *)data;
    mt2 = (mapthing2_t *)data;
    for (i=0 ; i<numthings ; i++, mt++, mt2++)
    {
		spawn = true;
		
		if(p_map_hexenfmt)
		{
			ttype = SHORT(mt2->type);

//			if(mt2->ac_spec)
//			{
//				ttype=2035;
//			}
		}else
		{
			ttype = SHORT(mt->type);
		}

		ttype = P_SetupRemapThingType(ttype);

		// Do not spawn cool, new monsters if !commercial
		if ( gamemode != commercial)
		{
			switch(ttype)
			{
			  case 68:	// Arachnotron
			  case 64:	// Archvile
			  case 88:	// Boss Brain
			  case 89:	// Boss Shooter
			  case 69:	// Hell Knight
			  case 67:	// Mancubus
			  case 71:	// Pain Elemental
			  case 65:	// Former Human Commando
			  case 66:	// Revenant
			  case 84:	// Wolf SS
			spawn = false;
			break;
			}
		}
		if (spawn == false)
		{
	//	    break;
			continue;
		}

//		if(!ttype)
//			continue;

#if 0
		// Do spawn all other stuff. 
		mt->x = SHORT(mt->x);
		mt->y = SHORT(mt->y);
		mt->angle = SHORT(mt->angle);
//		mt->type = SHORT(mt->type);
		mt->options = SHORT(mt->options);
		
		P_SpawnMapThing (mt);
#endif

		

#if 1
		// Do spawn all other stuff. 
		if(p_map_hexenfmt)
		{
			mtt.tid		= SHORT(mt2->tid);
			mtt.x		= SHORT(mt2->x);
			mtt.y		= SHORT(mt2->y);
			mtt.z		= SHORT(mt2->y);
			mtt.angle	= SHORT(mt2->angle);
			mtt.options	= SHORT(mt2->options);
			
			mtt.ac_spec	= mt2->ac_spec;
			mtt.arg1	= mt2->arg1;
			mtt.arg2	= mt2->arg2;
			mtt.arg3	= mt2->arg3;
			mtt.arg4	= mt2->arg4;
			mtt.arg5	= mt2->arg5;
		}else
		{
			mtt.tid		= 0;
			mtt.x		= SHORT(mt->x);
			mtt.y		= SHORT(mt->y);
			mtt.z		= 0;
			mtt.angle	= SHORT(mt->angle);
			mtt.options	= SHORT(mt->options);
			
			mtt.ac_spec	= 0;
			mtt.arg1	= 0;
			mtt.arg2	= 0;
			mtt.arg3	= 0;
			mtt.arg4	= 0;
			mtt.arg5	= 0;
		}

		mtt.type = ttype;
		
		P_SpawnMapThing (&mtt);
#endif
    }
	
    Z_Free (data);
}


//
// P_LoadLineDefs
// Also counts secret lines for intermissions.
//
void P_LoadLineDefs (int lump)
{
    byte*		data;
    int			i, lsz, acpr;
    maplinedef_t	*mld;
    maplinedef2_t	*mld2;
    line_t*		ld;
    vertex_t*		v1;
    vertex_t*		v2;
		
	lsz = W_LumpLength (lump);

	if(!p_map_hexenfmt)
	{
		if((lsz % sizeof(maplinedef_t)) || (gamemode==hexen))
		{
			if(lsz % sizeof(maplinedef2_t))
			{
				I_Error("Bad Lindedef Size\n");
			}

			p_map_hexenfmt=1;
		}
	}else
	{
		if(lsz % sizeof(maplinedef2_t))
		{
			I_Error("Bad Lindedef Size\n");
		}
	}

	if(p_map_hexenfmt)
	{
		numlines = lsz / sizeof(maplinedef2_t);
	}else
	{
		numlines = lsz / sizeof(maplinedef_t);
    }

    lines = Z_Malloc ((numlines+10)*sizeof(line_t),PU_LEVEL,0);	
    memset (lines, 0, (numlines+10)*sizeof(line_t));
    data = W_CacheLumpNum (lump,PU_STATIC);
    
    fakelines = lines+numlines;
	
    mld = (maplinedef_t *)data;
    mld2 = (maplinedef2_t *)data;
    ld = lines;
    for (i=0 ; i<numlines ; i++, mld++, mld2++, ld++)
    {
		if(p_map_hexenfmt)
		{
//			ld->special = SHORT(mld2->special);
//			ld->tag = SHORT(mld2->tag);

			ld->flags		= SHORT(mld2->flags);
//			ld->special		= 0;
			ld->special		= mld2->acs_spec ? 1024:0;
			ld->tag			= 0;
			ld->acs_spec	= mld2->acs_spec;
			ld->arg1		= mld2->arg1;
			ld->arg2		= mld2->arg2;
			ld->arg3		= mld2->arg3;
			ld->arg4		= mld2->arg4;
			ld->arg5		= mld2->arg5;

			acpr=0;

			switch(mld2->acs_spec)
			{
			case 0:		/* NOP */
				break;

			case 11: /* Door_Open */
				ld->special = 1;
				break;
			case 12: /* Door_Raise */
				ld->special = 31;
				break;
			case 13: /* Door_LockedRaise */
				ld->special = 31;
				break;

			case 70: /* Teleport */
			case 71: /* Teleport_NoFog */
				acpr=1;
				ld->special = 39;
//				ld->tag		= ld->arg1;
				break;

			case 74: /* Teleport_NewMap */
				ld->special = 52;
				break;
				
			case 80:
				break;
			
			default:
				acpr=1;
				break;
			}

			if(acpr)
			{
				printf("sp=%d a1=%d a2=%d a3=%d a4=%d a5=%d\n",
					ld->acs_spec,
					ld->arg1,
					ld->arg2,
					ld->arg3,
					ld->arg4,
					ld->arg5);
			}


			v1 = ld->v1		= &vertexes[SHORT(mld2->v1)];
			v2 = ld->v2		= &vertexes[SHORT(mld2->v2)];

			ld->sidenum[0] = SHORT(mld2->sidenum[0]);
			ld->sidenum[1] = SHORT(mld2->sidenum[1]);
		}else
		{
			ld->flags = SHORT(mld->flags);
			ld->special = SHORT(mld->special);
			ld->tag = SHORT(mld->tag);
			ld->acs_spec	= 0;
			ld->arg1		= 0;
			ld->arg2		= 0;
			ld->arg3		= 0;
			ld->arg4		= 0;
			ld->arg5		= 0;

			v1 = ld->v1 = &vertexes[SHORT(mld->v1)];
			v2 = ld->v2 = &vertexes[SHORT(mld->v2)];
			ld->sidenum[0] = SHORT(mld->sidenum[0]);
			ld->sidenum[1] = SHORT(mld->sidenum[1]);
		}
		ld->dx = v2->x - v1->x;
		ld->dy = v2->y - v1->y;
		
		if (!ld->dx)
			ld->slopetype = ST_VERTICAL;
		else if (!ld->dy)
			ld->slopetype = ST_HORIZONTAL;
		else
		{
			if (FixedDiv (ld->dy , ld->dx) > 0)
			ld->slopetype = ST_POSITIVE;
			else
			ld->slopetype = ST_NEGATIVE;
		}
			
		if (v1->x < v2->x)
		{
			ld->bbox[BOXLEFT] = v1->x;
			ld->bbox[BOXRIGHT] = v2->x;
		}
		else
		{
			ld->bbox[BOXLEFT] = v2->x;
			ld->bbox[BOXRIGHT] = v1->x;
		}

		if (v1->y < v2->y)
		{
			ld->bbox[BOXBOTTOM] = v1->y;
			ld->bbox[BOXTOP] = v2->y;
		}
		else
		{
			ld->bbox[BOXBOTTOM] = v2->y;
			ld->bbox[BOXTOP] = v1->y;
		}

//		ld->sidenum[0] = SHORT(mld->sidenum[0]);
//		ld->sidenum[1] = SHORT(mld->sidenum[1]);

		if (ld->sidenum[0] != -1)
			ld->frontsector = sides[ld->sidenum[0]].sector;
		else
			ld->frontsector = 0;

		if (ld->sidenum[1] != -1)
			ld->backsector = sides[ld->sidenum[1]].sector;
		else
			ld->backsector = 0;
    }
	
    Z_Free (data);
}


//
// P_LoadSideDefs
//
void P_LoadSideDefs (int lump)
{
    byte*		data;
    int			i, lsz;
    mapsidedef_t*	msd;
    side_t*		sd;
	
	lsz = W_LumpLength (lump);
	if(lsz % sizeof(mapsidedef_t))
		__debugbreak();

    numsides = lsz / sizeof(mapsidedef_t);
    sides = Z_Malloc (numsides*sizeof(side_t),PU_LEVEL,0);	
    memset (sides, 0, numsides*sizeof(side_t));
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    msd = (mapsidedef_t *)data;
    sd = sides;
    for (i=0 ; i<numsides ; i++, msd++, sd++)
    {
		sd->textureoffset = SHORT(msd->textureoffset)<<FRACBITS;
		sd->rowoffset = SHORT(msd->rowoffset)<<FRACBITS;
		sd->toptexture = R_TextureNumForName(msd->toptexture);
		sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
		sd->midtexture = R_TextureNumForName(msd->midtexture);
		sd->sector = &sectors[SHORT(msd->sector)];
    }
	
    Z_Free (data);
}


//
// P_LoadBlockMap
//
void P_LoadBlockMap (int lump)
{
    int		i;
    int		count;
	
    blockmaplump = W_CacheLumpNum (lump,PU_LEVEL);
    blockmap = blockmaplump+4;
    count = W_LumpLength (lump)/2;

    for (i=0 ; i<count ; i++)
	blockmaplump[i] = SHORT(blockmaplump[i]);
		
    bmaporgx = blockmaplump[0]<<FRACBITS;
    bmaporgy = blockmaplump[1]<<FRACBITS;
    bmapwidth = blockmaplump[2];
    bmapheight = blockmaplump[3];
	
    // clear out mobj chains
    count = sizeof(*blocklinks)* bmapwidth*bmapheight;
    blocklinks = Z_Malloc (count,PU_LEVEL, 0);
    memset (blocklinks, 0, count);
}


void P_LoadBehavior (int lump)
{
    byte*		data;
    int			i, lsz;
//    mapsidedef_t*	msd;
 //   side_t*		sd;
	
	lsz = W_LumpLength (lump);

	if(lsz<8)
	{
		p_map_hexenfmt = 0;
		return;
	}

//    numsides = lsz / sizeof(mapsidedef_t);
    data = W_CacheLumpNum (lump,PU_STATIC);

	if(!data)
	{
		p_map_hexenfmt = 0;
		return;
	}
	
	if((data[0]!='A') || (data[1]!='C') || (data[2]!='S'))
	{
		p_map_hexenfmt = 0;
		Z_Free (data);

		return;
	}

	p_map_hexenfmt = 1;
	P_SetupAcsImage(data, lsz);
//    Z_Free (data);
	return;
}



//
// P_GroupLines
// Builds sector line lists and subsector sector numbers.
// Finds block bounding boxes for sectors.
//
void P_GroupLines (void)
{
    line_t**		linebuffer;
    int			i;
    int			j;
    int			total;
    line_t*		li;
    sector_t*		sector;
    subsector_t*	ss;
    seg_t*		seg;
    fixed_t		bbox[4];
    int			block;
	
    // look up sector number for each subsector
    ss = subsectors;
    for (i=0 ; i<numsubsectors ; i++, ss++)
    {
		seg = &segs[ss->firstline];
		ss->sector = seg->sidedef->sector;
    }

    // count number of lines in each sector
    li = lines;
    total = 0;
    for (i=0 ; i<numlines ; i++, li++)
    {
		total++;
		li->frontsector->linecount++;

		if (li->backsector && li->backsector != li->frontsector)
		{
			li->backsector->linecount++;
			total++;
		}
    }
	
    // build line tables for each sector	
    linebuffer = Z_Malloc (total*4, PU_LEVEL, 0);
    sector = sectors;
    for (i=0 ; i<numsectors ; i++, sector++)
    {
		M_ClearBox (bbox);
		sector->lines = linebuffer;
		li = lines;
		for (j=0 ; j<numlines ; j++, li++)
		{
			if (li->frontsector == sector || li->backsector == sector)
			{
				*linebuffer++ = li;
				M_AddToBox (bbox, li->v1->x, li->v1->y);
				M_AddToBox (bbox, li->v2->x, li->v2->y);
			}
		}
		if (linebuffer - sector->lines != sector->linecount)
			I_Error ("P_GroupLines: miscounted");
				
		// set the degenmobj_t to the middle of the bounding box
		sector->soundorg.x = (bbox[BOXRIGHT]+bbox[BOXLEFT])/2;
		sector->soundorg.y = (bbox[BOXTOP]+bbox[BOXBOTTOM])/2;
			
		// adjust bounding box to map blocks
		block = (bbox[BOXTOP]-bmaporgy+MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block >= bmapheight ? bmapheight-1 : block;
		sector->blockbox[BOXTOP]=block;

		block = (bbox[BOXBOTTOM]-bmaporgy-MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block < 0 ? 0 : block;
		sector->blockbox[BOXBOTTOM]=block;

		block = (bbox[BOXRIGHT]-bmaporgx+MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block >= bmapwidth ? bmapwidth-1 : block;
		sector->blockbox[BOXRIGHT]=block;

		block = (bbox[BOXLEFT]-bmaporgx-MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block < 0 ? 0 : block;
		sector->blockbox[BOXLEFT]=block;
    }
	
}


//
// P_SetupLevel
//
void
P_SetupLevel
( int		episode,
  int		map,
  int		playermask,
  skill_t	skill)
{
    int		i;
    char	lumpname[9];
    int		lumpnum;
	
    totalkills = totalitems = totalsecret = wminfo.maxfrags = 0;
    wminfo.partime = 180;
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	players[i].killcount = players[i].secretcount 
	    = players[i].itemcount = 0;
    }

    // Initial height of PointOfView
    // will be set by player think.
    players[consoleplayer].viewz = 1; 

    // Make sure all sounds are stopped before Z_FreeTags.
    S_Start ();			

    
#if 0 // UNUSED
    if (debugfile)
    {
	Z_FreeTags (PU_LEVEL, MAXINT);
	Z_FileDumpHeap (debugfile);
    }
    else
#endif
	Z_FreeTags (PU_LEVEL, PU_PURGELEVEL-1);


    // W_Profile ();	//BGB: Enabled

    P_InitThinkers ();

    // if working with a devlopment map, reload it
    W_Reload ();			
	   
    // find map name
//    if ( gamemode == commercial)
    if ( (gamemode == commercial) ||
		(gamemode == hexen))
    {
	if (map<10)
	    sprintf (lumpname,"map0%i", map);
	else
	    sprintf (lumpname,"map%i", map);
    }
    else
    {
		lumpname[0] = 'E';
		lumpname[1] = '0' + episode;
		lumpname[2] = 'M';
		lumpname[3] = '0' + map;
		lumpname[4] = 0;
    }

    lumpnum = W_GetNumForName (lumpname);
    
    if(lumpnum<0)
    {
		return;
    }
	
    leveltime = 0;

	p_map_hexenfmt = 0;		//BGB: Assume map not in Hexen format.
	
    // note: most of this ordering is important	
    P_LoadBlockMap (lumpnum+ML_BLOCKMAP);
    P_LoadVertexes (lumpnum+ML_VERTEXES);
    P_LoadSectors (lumpnum+ML_SECTORS);
    P_LoadSideDefs (lumpnum+ML_SIDEDEFS);

    P_LoadBehavior (lumpnum+ML_BEHAVIOR);	//BGB: Hints map format

    P_LoadLineDefs (lumpnum+ML_LINEDEFS);
    P_LoadSubsectors (lumpnum+ML_SSECTORS);
    P_LoadNodes (lumpnum+ML_NODES);
    P_LoadSegs (lumpnum+ML_SEGS);
	
    rejectmatrix = W_CacheLumpNum (lumpnum+ML_REJECT,PU_LEVEL);
    P_GroupLines ();

    bodyqueslot = 0;
    deathmatch_p = deathmatchstarts;
    P_LoadThings (lumpnum+ML_THINGS);
    
    // if deathmatch, randomly spawn the active players
    if (deathmatch)
    {
		for (i=0 ; i<MAXPLAYERS ; i++)
		{
			if (playeringame[i])
			{
				players[i].mo = NULL;
				G_DeathMatchSpawnPlayer (i);
			}
		}
    }

    // clear special respawning que
    iquehead = iquetail = 0;		
	
    // set up world state
    P_SpawnSpecials ();
	
    // build subsector connect matrix
    //	UNUSED P_ConnectSubsectors ();

    // preload graphics
    if (precache)
    {
		R_PrecacheLevel ();
	}

    //printf ("free memory: 0x%x\n", Z_FreeMemory());

}



//
// P_Init
//
void P_Init (void)
{
    P_InitSwitchList ();
    P_InitPicAnims ();
    R_InitSprites ((char **)sprnames);
}



