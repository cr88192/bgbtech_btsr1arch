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
//	Teleportation.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: p_telept.c,v 1.3 1997/01/28 22:08:29 b1 Exp $";



#include "doomdef.h"

#include "s_sound.h"

#include "p_local.h"


// Data.
#include "sounds.h"

// State.
#include "r_state.h"

int EV_CheckThinkerIsMObjP(thinker_t*	thinker)
{
	long lv0, lv1;
	if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
	{
		lv0=(long)(thinker->function.acp1);
		lv1=(long)((actionf_p1)P_MobjThinker);
		lv0&=0x0000FFFFFFFFFFFEULL;
		lv1&=0x0000FFFFFFFFFFFEULL;
		if(lv0==lv1)
		{
			__debugbreak();
			return(1);
		}

		return(0);
	}
	return(1);
}

mobj_t	*EV_FindMObjForTid(int tid)
{
	thinker_t*	thinker;
	mobj_t*	m;

	thinker = thinkercap.next;
	for (thinker = thinkercap.next;
	 thinker != &thinkercap;
	 thinker = thinker->next)
	{
//		if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
		if (!EV_CheckThinkerIsMObjP(thinker))
			continue;	

		m = (mobj_t *)thinker;
		if (m->spawnpoint.tid == tid)
			return(m);
	}
	return(NULL);
}

mobj_t	*EV_FindMObjForTid2(void **rptr, int tid)
{
	thinker_t*	thinker;
	mobj_t*	m;

	thinker = *rptr;
	if(!thinker)
		thinker = thinkercap.next;
	for ( ;
	 thinker != &thinkercap;
	 thinker = thinker->next)
	{
//		if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
		if (!EV_CheckThinkerIsMObjP(thinker))
			continue;	

		m = (mobj_t *)thinker;
		if (m->spawnpoint.tid == tid)
		{
			*rptr = thinker->next;
			return(m);
		}
	}
	return(NULL);
}

int EV_CountMObjForTid(int type, int tid)
{
	thinker_t*	thinker;
	mobj_t*	m;
	int cnt;

	cnt = 0;

	thinker = thinkercap.next;
	for (thinker = thinkercap.next;
	 thinker != &thinkercap;
	 thinker = thinker->next)
	{
//		if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
		if (!EV_CheckThinkerIsMObjP(thinker))
			continue;	

		m = (mobj_t *)thinker;
		
		if(m->health<=0)
			continue;

		if ((m->spawnpoint.type == type) || !type)
		{
			if ((m->spawnpoint.tid == tid) || !tid)
			{
				cnt++;
			}
		}
	}
	return(cnt);
}

//
// TELEPORTATION
//
int
EV_Teleport
( line_t*	line,
  int		side,
  mobj_t*	thing )
{
	int		i;
	int		tag;
	mobj_t*	m;
	mobj_t*	fog;
	unsigned	an;
	thinker_t*	thinker;
	sector_t*	sector;
	fixed_t	oldx;
	fixed_t	oldy;
	fixed_t	oldz;

	// don't teleport missiles
	if (thing->flags & MF_MISSILE)
		return 0;

	// Don't teleport if hit back of line,
	//  so you can get out of teleporter.
	if (side == 1)		
		return 0;	

	if(line->acs_spec)
	{
		thinker = thinkercap.next;
		for (thinker = thinkercap.next;
		 thinker != &thinkercap;
		 thinker = thinker->next)
		{
			// not a mobj
//			if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
			if (!EV_CheckThinkerIsMObjP(thinker))
				continue;	

			m = (mobj_t *)thinker;
			
			// not a teleportman
			if (m->type != MT_TELEPORTMAN )
				continue;

			if (m->spawnpoint.tid != line->arg1)
				continue;

//			sector = m->subsector->sector;
//			// wrong sector
//			if (sector-sectors != i )
//				continue;	

			oldx = thing->x;
			oldy = thing->y;
			oldz = thing->z;
					
			if (!P_TeleportMove (thing, m->x, m->y))
				return 0;
			
			thing->z = thing->floorz;  //fixme: not needed?

			if (thing->player)
				thing->player->viewz = thing->z+thing->player->viewheight;
			
			//teleport fog if not no-fog
			if(line->acs_spec!=71)
			{
				// spawn teleport fog at source and destination
				fog = P_SpawnMobj (oldx, oldy, oldz, MT_TFOG);
				S_StartSound (fog, sfx_telept);
				an = m->angle >> ANGLETOFINESHIFT;
				fog = P_SpawnMobj (m->x+20*finecosine[an], m->y+20*finesine[an]
						   , thing->z, MT_TFOG);

				// emit sound, where?
				S_StartSound (fog, sfx_telept);
			}
			
			// don't move for a bit
			if (thing->player)
				thing->reactiontime = 18;	

			thing->angle = m->angle;
			thing->momx = thing->momy = thing->momz = 0;
			return 1;
		}	
	}
	
	tag = line->tag;
	for (i = 0; i < numsectors; i++)
	{
		if (sectors[ i ].tag == tag )
		{
			thinker = thinkercap.next;
			for (thinker = thinkercap.next;
			 thinker != &thinkercap;
			 thinker = thinker->next)
			{
			// not a mobj
//			if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
			if (!EV_CheckThinkerIsMObjP(thinker))
				continue;	

			m = (mobj_t *)thinker;
			
			// not a teleportman
			if (m->type != MT_TELEPORTMAN )
				continue;		

			sector = m->subsector->sector;
			// wrong sector
			if (sector-sectors != i )
				continue;	

			oldx = thing->x;
			oldy = thing->y;
			oldz = thing->z;
					
			if (!P_TeleportMove (thing, m->x, m->y))
				return 0;
			
			thing->z = thing->floorz;  //fixme: not needed?
			if (thing->player)
				thing->player->viewz = thing->z+thing->player->viewheight;
					
			// spawn teleport fog at source and destination
			fog = P_SpawnMobj (oldx, oldy, oldz, MT_TFOG);
			S_StartSound (fog, sfx_telept);
			an = m->angle >> ANGLETOFINESHIFT;
			fog = P_SpawnMobj (m->x+20*finecosine[an], m->y+20*finesine[an]
					   , thing->z, MT_TFOG);

			// emit sound, where?
			S_StartSound (fog, sfx_telept);
			
			// don't move for a bit
			if (thing->player)
				thing->reactiontime = 18;	

			thing->angle = m->angle;
			thing->momx = thing->momy = thing->momz = 0;
			return 1;
			}	
		}
	}
	return 0;
}

