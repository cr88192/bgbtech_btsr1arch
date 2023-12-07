#include <stdlib.h>

#include "doomdef.h"
#include "doomstat.h"

#include "i_system.h"
#include "z_zone.h"
#include "m_argv.h"
#include "m_random.h"
#include "w_wad.h"

#include "r_local.h"
#include "p_local.h"

#include "g_game.h"

#include "s_sound.h"

// State.
#include "r_state.h"

// Data.
#include "sounds.h"

extern line_t		*fakelines;		//BGB: Fake Lines, for ACS stuff
line_t		*trigline;		//trigger line

sector_t	*P_AcsNextSectorForTag(int *ridx, int tag)
{
	int i;

	for (i = *ridx; i < numsectors; i++)
	{
		if (sectors[ i ].tag == tag )
		{
			*ridx=i+1;
			return(sectors+i);
		}
	}
	return(NULL);
}

void P_AcsAdjustSectorFloorForTag(int tag, int adj)
{
	sector_t	*sect;
	int idx;
	
	if(tag<=0)
		return;

	idx=0;
	while(1)
	{
		sect=P_AcsNextSectorForTag(&idx, tag);
		if(!sect)
			break;
		sect->floorheight+=adj*FRACUNIT;
	}
}

void P_AcsAdjustSectorCeilForTag(int tag, int adj)
{
	sector_t	*sect;
	int idx;
	
	if(tag<=0)
		return;
	
	idx=0;
	while(1)
	{
		sect=P_AcsNextSectorForTag(&idx, tag);
		if(!sect)
			break;
		sect->ceilingheight+=adj*FRACUNIT;
	}
}

void P_AcsThingActivate(int tid)
{
	mobj_t*	obj;
	void *idx;
	
	idx=NULL;
	obj=EV_FindMObjForTid2(&idx, tid);
	while(obj)
	{
		obj->flags &= ~MF_DORMANT;
	
		obj=EV_FindMObjForTid2(&idx, tid);
	}
}

void P_AcsThingDeactivate(int tid)
{
	mobj_t*	obj;
	void *idx;
	
	idx=NULL;
	obj=EV_FindMObjForTid2(&idx, tid);
	while(obj)
	{
		obj->flags |= MF_DORMANT;
	
		obj=EV_FindMObjForTid2(&idx, tid);
	}
}

void P_AcsThingRemove(int tid)
{
	mobj_t*	obj;
	void *idx;
	
	idx=NULL;
	obj=EV_FindMObjForTid2(&idx, tid);
	while(obj)
	{
//		mobj->flags |= MF_DORMANT;
		P_RemoveMobj(obj);
	
		obj=EV_FindMObjForTid2(&idx, tid);
	}
}

void P_AcsThingDestroy(int tid, int extreme)
{
	mobj_t*	obj;
	void *idx;
	int dmg;
	
	idx=NULL;
	obj=EV_FindMObjForTid2(&idx, tid);
	while(obj)
	{
//		mobj->flags |= MF_DORMANT;
//		P_RemoveMobj(obj);

		dmg=obj->health+10;
		if(extreme)
			dmg+=100;

		P_DamageMobj(obj, NULL, NULL, dmg);
	
		obj=EV_FindMObjForTid2(&idx, tid);
	}
}

void P_AcsDoSpecialCmd(
	int sp, int arg1, int arg2, int arg3, int arg4, int arg5)
{
	line_t	*ld;
	
	ld=fakelines;
	ld->sidenum[0]=-1;
	ld->sidenum[1]=-1;
	
	ld->acs_spec=sp;
	ld->arg1=arg1;
	ld->arg2=arg2;
	ld->arg3=arg3;
	ld->arg4=arg4;
	ld->arg5=arg5;

	switch(sp)
	{
	case 10: /* Door_Close */
		ld->tag = arg1;
		ld->special = 50;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 11: /* Door_Open */
		ld->tag = arg1;
//		ld->special = 1;
//		ld->special = 29;
		ld->special = 103;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 12: /* Door_Raise */
		ld->tag = arg1;
//		ld->special = 31;
//		ld->special = 29;
		ld->special = 103;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 13: /* Door_LockedRaise */
		ld->tag = arg1;
//		ld->special = 31;
//		ld->special = 29;
		ld->special = 103;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	
	case 20:	/* Floor_LowerByValue */
		P_AcsAdjustSectorFloorForTag(arg1, -arg3);
		break;
	case 21: /* Floor_LowerToLowest */
		ld->tag = arg1;
		ld->special = 38;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 22: /* Floor_LowerToNearest */
		ld->tag = arg1;
		ld->special = 19;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 23:	/* Floor_RaiseByValue */
		P_AcsAdjustSectorFloorForTag(arg1, arg3);
		break;
	case 24: /* Floor_RaiseToHighest */
		ld->tag = arg1;
		ld->special = 94;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 25: /* Floor_RaiseToNearest */
		ld->tag = arg1;
		ld->special = 119;
		P_UseSpecialLine (NULL, ld, 0);
		break;

	case 28: /* Floor_RaiseAndCrush */
		ld->tag = arg1;
//		ld->special = 56;
		ld->special = 55;
		P_UseSpecialLine (NULL, ld, 0);
		break;

	case 33: /* ForceField, ZDoom */
	case 34: /* ClearForceField, ZDoom */
		break;
	case 35:	/* Floor_RaiseByValueTimes8 */
		P_AcsAdjustSectorFloorForTag(arg1, arg3*8);
		break;
	case 36:	/* Floor_LowerByValueTimes8 */
		P_AcsAdjustSectorFloorForTag(arg1, -arg3*8);
		break;
	case 37: /* Floor_MoveToValue, ZDoom */
	case 38: /* Ceiling_Waggle, ZDoom */
	case 39: /* Teleport_ZombieChanger, ZDoom */
		break;
	case 40:	/* Ceiling_LowerByValue */
		P_AcsAdjustSectorCeilForTag(arg1, -arg3);
		break;
	case 41:	/* Ceiling_RaiseByValue */
		P_AcsAdjustSectorCeilForTag(arg1, arg3);
		break;
	case 42: /* Ceiling_CrushAndRaise */
		ld->tag = arg1;
		ld->special = 141;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 43: /* Ceiling_LowerAndCrush */
		ld->tag = arg1;
		ld->special = 44;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 44: /* Ceiling_CrushStop */
		ld->tag = arg1;
		ld->special = 57;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 45: /* Ceiling_CrushRaiseAndStay */
		ld->tag = arg1;
		ld->special = 73;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 46: /* Floor_CrushStop */
		ld->tag = arg1;
		ld->special = 54;
		P_UseSpecialLine (NULL, ld, 0);
		break;

	case 47: /* Ceiling_MoveToValue, ZDoom */
	case 48: /* Sector_Attach3dMidtex, ZDoom */
	case 49: /* GlassBreak, ZDoom */
	case 50: /* ExtraFloor_LightOnly, ZDoom */
	case 51: /* Sector_SetLink, ZDoom */
	case 52: /* Scroll_Wall, ZDoom */
	case 53: /* Line_SetTextureOffset, ZDoom */
	case 54: /* Sector_ChangeFlags, ZDoom */
	case 55: /* Line_SetBlocking, ZDoom */
	case 56: /* Line_SetTextureScale, ZDoom */
	case 57: /* Sector_SetPortal, ZDoom */
	case 58: /* Sector_CopyScroller, ZDoom */
	case 59: /* Polyobj_OR_MoveToSpot, ZDoom */
		break;

	case 60: /* Plat_PerpetualRaise */
		ld->tag = arg1;
		ld->special = 18;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 61: /* Plat_Stop */
		ld->tag = arg1;
		ld->special = 54;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 62: /* Plat_DownWaitUpStay */
		ld->tag = arg1;
		ld->special = 21;
		P_UseSpecialLine (NULL, ld, 0);
		break;
	case 63:	/* Plat_LowerByValue */
		P_AcsAdjustSectorFloorForTag(arg1, -arg3*8);
		break;

//	case 64: /* Door_UpWaitDownStay */
//		ld->tag = arg1;
//		ld->special = 21;
//		P_UseSpecialLine (NULL, ld, 0);
//		break;

	case 65:	/* Plat_UpByValue */
		P_AcsAdjustSectorFloorForTag(arg1, arg3*8);
		break;
	case 66:	/* Floor_LowerInstant */
		P_AcsAdjustSectorFloorForTag(arg1, -arg3*8);
		break;
	case 67:	/* Floor_RaiseInstant */
		P_AcsAdjustSectorFloorForTag(arg1, arg3*8);
		break;

	case 70: /* Teleport */
	case 71: /* Teleport_NoFog */
//		ld->special = 39;
//		P_UseSpecialLine (NULL, ld, 0);
		break;

	case 74: /* Teleport_NewMap */
		ld->special = 11;
		P_UseSpecialLine (NULL, ld, 0);
		break;

	case 80:	/* ACS_Execute */
		P_AcsRunScript(arg1, arg2, arg3, arg4, arg5);
		break;

	case 94:	/* Floor_RaiseByValue */
		P_AcsAdjustSectorFloorForTag(arg1, arg3);
		P_AcsAdjustSectorCeilForTag(arg1, -arg3);
		break;

	case 121:	/* Line_SetIdentification */
		/* No-Op, Gives an ACS ID number and flags. */
		break;
		
	case 130:
		P_AcsThingActivate(arg1);
		break;
	case 131:
		P_AcsThingDeactivate(arg1);
		break;
	case 132:
		P_AcsThingRemove(arg1);
		break;
	case 133:
		P_AcsThingDestroy(arg1, arg2);
		break;

	default:
		printf("P_AcsDoSpecialCmd: Unhandled "
				"sp=%d a1=%d a2=%d a3=%d a4=%d a5=%d\n",
			sp, arg1, arg2, arg3, arg4, arg5);
		break;
	}
}

void
P_AcsDoSpecialLine
( mobj_t*	thing,
	line_t*	line )
{
	trigline = line;
	P_AcsDoSpecialCmd(
		line->acs_spec,		line->arg1,
		line->arg2,			line->arg3,
		line->arg4,			line->arg5);
	
	switch(line->acs_spec)
	{

	case 62: /* Plat_DownWaitUpStay */
		break;

	default:
		if(!(line->flags&0x0200))
			line->special = 0;
		break;
	}

#if 0
	switch(line->acs_spec)
	{
	case 20:
		P_AcsAdjustSectorFloorForTag(line->arg1, -line->arg3);
		line->special = 0;
		break;
	case 23:
		P_AcsAdjustSectorFloorForTag(line->arg1,  line->arg3);
		line->special = 0;
		break;

	case 35:
		P_AcsAdjustSectorFloorForTag(line->arg1, line->arg3*8);
		line->special = 0;
		break;
	case 36:
		P_AcsAdjustSectorFloorForTag(line->arg1, -line->arg3*8);
		line->special = 0;
		break;

	case 40:
		P_AcsAdjustSectorCeilForTag(line->arg1, -line->arg3);
		line->special = 0;
		break;
	case 41:
		P_AcsAdjustSectorCeilForTag(line->arg1, line->arg3);
		line->special = 0;
		break;
	
	case 80:
		P_AcsRunScript(line->arg1, line->arg2,
			line->arg3, line->arg4, line->arg5);
		line->special = 0;
		break;
	}
#endif
}

void
P_AcsCrossSpecialLine
( int		linenum,
	int		side,
	mobj_t*	thing )
{
	line_t*	line;
	int		ok, isproj;
	int 	spop;

	line = &lines[linenum];

	spop = line->special;

	switch(thing->type)
	{
		case MT_ROCKET:
		case MT_PLASMA:
		case MT_BFG:
		case MT_TROOPSHOT:
		case MT_HEADSHOT:
		case MT_BRUISERSHOT:
			isproj=1;
			break;
		
		default:
			isproj=0;
			break;
	}

	switch(line->flags&0x3C00)
	{
	case 0x0400:
		return;
	case 0x0800:
		if (thing->player || isproj)
			return;
		break;
	case 0x0C00:
		return;
	case 0x1000:
		return;
	case 0x1400:
		if (!isproj)
			return;
		break;
	case 0x1800:
		if (!thing->player || isproj)
			return;
		break;
	case 0x2800:
	case 0x3800:
		if (isproj)
			return;
		break;
	default:
		if (!thing->player || isproj)
			return;
		break;
	}

#if 0
	if (!thing->player)
	{
		switch(thing->type)
		{
			case MT_ROCKET:
			case MT_PLASMA:
			case MT_BFG:
			case MT_TROOPSHOT:
			case MT_HEADSHOT:
			case MT_BRUISERSHOT:
				if((line->flags&0x1C00)!=0x1400)
					return;
				break;
			
			default:
				if((line->flags&0x1C00)!=0x0800)
					return;
				break;
		}
	}else
	{
		if((line->flags&0x1C00)!=0x1800)
			return;
	}
#endif

	printf("P_AcsCrossSpecialLine: %d\n", line->acs_spec);

	P_AcsDoSpecialLine(thing, line);
}

void
P_AcsShootSpecialLine
( mobj_t*	thing,
	line_t*	line )
{
	int		ok, spop;

	if (!thing->player)
	{
		return;
	}

	switch(line->flags&0x3C00)
	{
	case 0x0400:
		break;
	case 0x0800:
		return;
	case 0x0C00:
		break;
	case 0x1000:
		return;
	case 0x1400:
		return;
	case 0x1800:
		return;
	case 0x2800:
	case 0x3800:
		return;
	default:
		break;
	}

	spop = line->special;

	printf("P_AcsShootSpecialLine: %d\n", line->acs_spec);	

	P_AcsDoSpecialLine(thing, line);
}


boolean
P_AcsUseSpecialLine
( mobj_t*	thing,
	line_t*	line,
	int		side )
{

//	if (!thing->player)
//	{
//		return false;
//	}

	switch(line->flags&0x3C00)
	{
	case 0x0400:
		if (!thing->player)
			return(false);
		break;
	case 0x0800:
			return(false);
	case 0x0C00:
		return(false);
	case 0x1000:
		return(false);
	case 0x1400:
		return(false);
	case 0x1800:
		return(false);
	case 0x2400:
		break;
	case 0x2800:
	case 0x3800:
		return(false);
	default:
		break;
	}

	printf("P_AcsUseSpecialLine: %d\n", line->acs_spec);	

	P_AcsDoSpecialLine(thing, line);

	return false;
}


byte *p_acsvm_imgdat;
int p_acsvm_imgsz;

int p_acsvm_scr_idn[1024];
int p_acsvm_scr_ofs[1024];
int p_acsvm_scr_arn[1024];
int p_acsvm_scr_num;

byte p_acsvm_scr_term[1024];

int p_acsvm_strs_ofs[4096];
char *p_acsvm_strs_ptr[4096];
int p_acsvm_strs_num;

int p_acsvm_mapvar[256];
int p_acsvm_wrlvar[256];

p_acsrun_t p_acsvm_run[256];
int p_acsvm_nrun;

int p_acsvm_newmap;

void P_SetupAcsImage(byte *data, int sz)
{
	char *s;
	int dofs, sofs;
	int i, j, k;

	if(p_acsvm_imgdat)
	{
		Z_Free (p_acsvm_imgdat);
		p_acsvm_imgdat = NULL;
	}

	if((data[0]!='A') || (data[1]!='C') ||
		(data[2]!='S') || (data[3]!=0))
	{
		return;
	}

	p_acsvm_imgdat = data;
	p_acsvm_imgsz = sz;
	
	dofs=*(int *)(data+4);
	p_acsvm_scr_num=*(int *)(data+dofs);
	
	for(i=0; i<p_acsvm_scr_num; i++)
	{
		p_acsvm_scr_idn[i]=((int *)(data+dofs+4))[i*3+0];
		p_acsvm_scr_ofs[i]=((int *)(data+dofs+4))[i*3+1];
		p_acsvm_scr_arn[i]=((int *)(data+dofs+4))[i*3+2];
	}
	
	sofs = dofs+4+(p_acsvm_scr_num*12);
	p_acsvm_strs_num=*(int *)(data+sofs);

	for(i=0; i<p_acsvm_strs_num; i++)
	{
		j=((int *)(data+sofs+4))[i];
		p_acsvm_strs_ofs[i]=j;
		
		if((j>0) && (j<sz))
		{
			s=(char *)(data+j);
			p_acsvm_strs_ptr[i]=s;
		}else
		{
			p_acsvm_strs_ptr[i]=NULL;
		}
	}
	
	p_acsvm_newmap=1;
}




p_acsrun_t *P_AcsAllocRun(void)
{
	p_acsrun_t *tmp;
	int i, j, k;

	for(i=0; i<p_acsvm_nrun; i++)
	{
		if(p_acsvm_run[i].map<0)
		{
			tmp=p_acsvm_run+i;
			memset(tmp, 0, sizeof(p_acsrun_t));
			return(tmp);
		}
	}
	
	if(p_acsvm_nrun<256)
	{
		i=p_acsvm_nrun++;
		tmp=p_acsvm_run+i;
		memset(tmp, 0, sizeof(p_acsrun_t));
		return(tmp);
	}
	
	return(NULL);
}

p_acsrun_t *P_AcsLookupRun(int idx, int map)
{
	p_acsrun_t *tmp;
	int i, j, k;

	if(map<=0)
		map=gamemap;

	for(i=0; i<p_acsvm_nrun; i++)
	{
		if((p_acsvm_run[i].map==map) &&
			(p_acsvm_run[i].idx==idx))
		{
			tmp=p_acsvm_run+i;
			return(tmp);
		}
	}
	
	return(NULL);
}

void P_AcsTryStartRun(p_acsrun_t *run)
{
	int i, j, k;

	for(i=0; i<p_acsvm_scr_num; i++)
	{
		if((p_acsvm_scr_idn[i]%1000)!=run->idx)
			continue;
			
		run->css=(int *)(p_acsvm_imgdat+p_acsvm_scr_ofs[i]);
		run->cs=run->css;
		switch(p_acsvm_scr_arn[i]&7)
		{
		case 0:
			break;
		case 1:
			run->stack[run->stackpos++]=run->arg1;
			break;
		case 2:
			run->stack[run->stackpos++]=run->arg1;
			run->stack[run->stackpos++]=run->arg2;
			break;
		case 3:
			run->stack[run->stackpos++]=run->arg1;
			run->stack[run->stackpos++]=run->arg2;
			run->stack[run->stackpos++]=run->arg3;
			break;
		}
		break;
	}
}

void P_AcsRun_Push(p_acsrun_t *run, int val)
{
	run->stack[run->stackpos++]=val;
}

int P_AcsRun_Pop(p_acsrun_t *run)
{
	run->stackpos--;
	return(run->stack[run->stackpos]);
}

int P_AcsRun_SetLocalVar(p_acsrun_t *run, int idx, int val)
{
	run->stack[idx]=val;
	return(0);
}

int P_AcsRun_SetMapVar(p_acsrun_t *run, int idx, int val)
{
	p_acsvm_mapvar[idx]=val;
	return(0);
}

int P_AcsRun_SetWorldVar(p_acsrun_t *run, int idx, int val)
{
	p_acsvm_wrlvar[idx]=val;
	return(0);
}


int P_AcsRun_GetLocalVar(p_acsrun_t *run, int idx)
{
	return(run->stack[idx]);
}

int P_AcsRun_GetMapVar(p_acsrun_t *run, int idx)
{
	return(p_acsvm_mapvar[idx]);
}

int P_AcsRun_GetWorldVar(p_acsrun_t *run, int idx)
{
	return(p_acsvm_wrlvar[idx]);
}

void P_AcsRun_LpSec(p_acsrun_t *run,
	int sp, int a1, int a2, int a3, int a4, int a5)
{
//	printf("P_AcsRun_LpSec: sp=%d a1=%d a2=%d a3=%d a4=%d a5=%d\n",
//		sp, a1, a2, a3, a4, a5);

	P_AcsDoSpecialCmd(sp, a1, a2, a3, a4, a5);
}


void P_AcsRun_SectorSound(p_acsrun_t *run, int a1, int a2)
{
	printf("P_AcsRun_SectorSound: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_AmbientSound(p_acsrun_t *run, int a1, int a2)
{
	printf("P_AcsRun_AmbientSound: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_SoundSequence(p_acsrun_t *run, int a1)
{
	printf("P_AcsRun_SoundSequence: a1=%d\n",
		a1);
}

line_t	*P_AcsQueryLine (int *ridx, int lineid)
{
    line_t	*ld;
    int i, j, k;

    for (i=*ridx ; i<numlines ; i++)
    {
		ld = lines+i;
		if(ld->acs_spec!=121)
			continue;
		if(ld->arg1==lineid)
		{
			*ridx=i+1;
			return(ld);
		}
    }

    return(NULL);
}

void P_AcsRun_SetLineTexture(p_acsrun_t *run, int a1, int a2, int a3, int a4)
{
    line_t	*ld;
    side_t	*sd;
    int idx, sn, tex;

	printf("P_AcsRun_SetLineTexture: a1=%d a2=%d a3=%d a4=%d\n",
		a1, a2, a3, a4);
		
	tex=R_TextureNumForName(p_acsvm_strs_ptr[a4]);

	idx=0;
	ld=P_AcsQueryLine(&idx, a1);
	while(ld)
	{
		sn=ld->sidenum[a2];
		if(((u16)sn)==0xFFFF)
		{
			ld=P_AcsQueryLine(&idx, a1);
			continue;
		}
		
		sd=sides+sn;

		switch(a3)
		{
		case 0:
			sd->toptexture=tex;
			break;
		case 1:
			sd->midtexture=tex;
			break;
		case 2:
			sd->bottomtexture=tex;
			break;
		}

		ld=P_AcsQueryLine(&idx, a1);
	}
}

void P_AcsRun_SetLineBlocking(p_acsrun_t *run, int a1, int a2)
{
    line_t	*ld;
    int idx, sn;

	printf("P_AcsRun_SetLineBlocking: a1=%d a2=%d\n",
		a1, a2);

	idx=0;
	ld=P_AcsQueryLine(&idx, a1);
	while(ld)
	{
		if(a2)
		{
			ld->flags |= ML_BLOCKING;
		}else
		{
			ld->flags &= ~(ML_BLOCKING|ML_BLOCKMONSTERS);
		}

		ld=P_AcsQueryLine(&idx, a1);
	}
}

void P_AcsRun_SetLineSpecial(p_acsrun_t *run,
	int a1, int a2, int a3,
	int a4, int a5, int a6, int a7)
{
    line_t	*ld;
    int idx, sn;

	printf("P_AcsRun_SetLineSpecial: a1=%d a2=%d a3=%d "
			"a4=%d a5=%d a6=%d a7=%d\n",
		a1, a2, a3, a4, a5, a6, a7);

	idx=0;
	ld=P_AcsQueryLine(&idx, a1);
	while(ld)
	{
		ld->acs_spec=a2;
		ld->arg1=a3;
		ld->arg2=a4;
		ld->arg3=a5;
		ld->arg4=a6;
		ld->arg5=a7;

		if(ld->acs_spec)
		{
			ld->special=1024;
		}else
		{
			ld->special=0;
		}
	
		ld=P_AcsQueryLine(&idx, a1);
	}
}

void P_AcsRun_ThingSound(p_acsrun_t *run,
	int a1, int a2, int a3)
{
	mobj_t	*obj;

	printf("P_AcsRun_ThingSound: a1=%d a2=%d a3=%d\n",
		a1, a2, a3);
	
	obj=EV_FindMObjForTid(a1);
	if(!obj)
	{
		printf("P_AcsRun_ThingSound: No Thing %d\n", a1);
		return;
	}
	
	if(!a2)
		a2=sfx_rxplod;
		
	S_StartSound (obj, a2);
}

int P_AcsRun_ThingCount(p_acsrun_t *run,
	int a1, int a2)
{
	int v;

	printf("P_AcsRun_ThingCount: a1=%d a2=%d\n",
		a1, a2);
		
	a1 = P_SetupRemapThingType(a1);
	
	v = EV_CountMObjForTid (a1, a2);

	return(v);
}

void P_AcsRun_ChangeFloor(p_acsrun_t *run,
	int a1, int a2)
{
	printf("P_AcsRun_ChangeFloor: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_ChangeCeiling(p_acsrun_t *run,
	int a1, int a2)
{
	printf("P_AcsRun_ChangeCeiling: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_TagWait(p_acsrun_t *run, int a1)
{
	printf("P_AcsRun_TagWait: a1=%d\n", a1);
	run->status=4;
}

void P_AcsRun_PolyWait(p_acsrun_t *run, int a1)
{
	printf("P_AcsRun_PolyWait: a1=%d\n", a1);
	run->status=4;
}

void P_AcsRun_ScriptWait(p_acsrun_t *run, int a1)
{
	printf("P_AcsRun_ScriptWait: a1=%d\n", a1);
	run->status=4;
}

void P_AcsRun_ClearLineSpecial(p_acsrun_t *run)
{
	if(run->trigline)
	{
		run->trigline->special=0;
	}
}

void P_AcsRun_PrintString(p_acsrun_t *run, int val)
{
	char *s, *t;
	
	s=NULL;
	if((val>=0) && (val<p_acsvm_strs_num))
		s=p_acsvm_strs_ptr[val];
	if(!s)s="(NULL)";
	
	t=run->prnbuf+run->prnpos;
	sprintf(t, "%s", s);
	run->prnpos+=strlen(t);
}

void P_AcsRun_PrintNumber(p_acsrun_t *run, int val)
{
	char *t;
	t=run->prnbuf+run->prnpos;
	sprintf(t, "%d", val);
	run->prnpos+=strlen(t);
}

void P_AcsRun_PrintChar(p_acsrun_t *run, int val)
{
	char *t;
	t=run->prnbuf+run->prnpos;
	sprintf(t, "%c", val);
	run->prnpos+=strlen(t);
}

void P_AcsRun_BeginPrint(p_acsrun_t *run)
{
	run->prnpos=0;
}

void P_AcsRun_EndPrint(p_acsrun_t *run)
{
	printf("Print: %s\n", run->prnbuf);
	run->prnpos=0;
}

void P_AcsRun_EndPrintBold(p_acsrun_t *run)
{
	printf("PrintBold: %s\n", run->prnbuf);
	run->prnpos=0;
}

void P_AcsRun_ActivatorSound(p_acsrun_t *run,
	int a1, int a2)
{
	printf("P_AcsRun_ActivatorSound: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_LocalAmbientSound(p_acsrun_t *run,
	int a1, int a2)
{
	printf("P_AcsRun_LocalAmbientSound: a1=%d a2=%d\n",
		a1, a2);
}

void P_AcsRun_SetLineMonsterBlocking(p_acsrun_t *run,
	int a1, int a2)
{
	printf("P_AcsRun_SetLineMonsterBlocking: a1=%d a2=%d\n",
		a1, a2);
}


void P_AcsRunLoop(p_acsrun_t *run)
{
	int op, lim;
	int sp, a1, a2, a3, a4, a5, a6, a7;
	int i, j, k;
	
	lim=4096;
//	lim=1<<18;
	while(((lim--)>0) && !(run->status))
	{
		op=*run->cs++;
		switch(op)
		{
		case 0:
			break;
		case 1:
			run->status=1;	//terminate
			break;
		case 2:
			run->status=2;	//suspend
			break;
		case 3:
			i=*run->cs++;
			P_AcsRun_Push(run, i);
			break;

		case 4:
			sp=*run->cs++;			a1=P_AcsRun_Pop(run);
			P_AcsRun_LpSec(run, sp, a1, 0, 0, 0, 0);
			break;
		case 5:
			sp=*run->cs++;			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_LpSec(run, sp, a1, a2, 0, 0, 0);
			break;
		case 6:
			sp=*run->cs++;			a3=P_AcsRun_Pop(run);
			a2=P_AcsRun_Pop(run);	a1=P_AcsRun_Pop(run);
			P_AcsRun_LpSec(run, sp, a1, a2, a3, 0, 0);
			break;
		case 7:
			sp=*run->cs++;			a4=P_AcsRun_Pop(run);
			a3=P_AcsRun_Pop(run);	a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_LpSec(run, sp, a1, a2, a3, a4, 0);
			break;
		case 8:
			sp=*run->cs++;			a5=P_AcsRun_Pop(run);
			a4=P_AcsRun_Pop(run);	a3=P_AcsRun_Pop(run);
			a2=P_AcsRun_Pop(run);	a1=P_AcsRun_Pop(run);
			P_AcsRun_LpSec(run, sp, a1, a2, a3, a4, a5);
			break;

		case 9:
			sp=*run->cs++;			a1=*run->cs++;
			P_AcsRun_LpSec(run, sp, a1, 0, 0, 0, 0);
			break;
		case 10:
			sp=*run->cs++;			a1=*run->cs++;
			a2=*run->cs++;
			P_AcsRun_LpSec(run, sp, a1, a2, 0, 0, 0);
			break;
		case 11:
			sp=*run->cs++;			a1=*run->cs++;
			a2=*run->cs++;			a3=*run->cs++;
			P_AcsRun_LpSec(run, sp, a1, a2, a3, 0, 0);
			break;
		case 12:
			sp=*run->cs++;			a1=*run->cs++;
			a2=*run->cs++;			a3=*run->cs++;
			a4=*run->cs++;
			P_AcsRun_LpSec(run, sp, a1, a2, a3, a4, 0);
			break;
		case 13:
			sp=*run->cs++;			a1=*run->cs++;
			a2=*run->cs++;			a3=*run->cs++;
			a4=*run->cs++;			a5=*run->cs++;
			P_AcsRun_LpSec(run, sp, a1, a2, a3, a4, a5);
			break;

		case 14:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1+a2);
			break;
		case 15:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1-a2);
			break;
		case 16:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1*a2);
			break;
		case 17:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			if(!a2)
			{
				run->status=-2;
				break;
			}
			P_AcsRun_Push(run, a1/a2);
			break;
		case 18:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			if(!a2)
			{
				run->status=-2;
				break;
			}
			P_AcsRun_Push(run, a1%a2);
			break;
		case 19:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1==a2);
			break;
		case 20:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1!=a2);
			break;
		case 21:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
//			P_AcsRun_Push(run, a1<a2);
			P_AcsRun_Push(run, a2<a1);
			break;
		case 22:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
//			P_AcsRun_Push(run, a1>a2);
			P_AcsRun_Push(run, a2>a1);
			break;
		case 23:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
//			P_AcsRun_Push(run, a1<=a2);
			P_AcsRun_Push(run, a2<=a1);
			break;
		case 24:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
//			P_AcsRun_Push(run, a1>=a2);
			P_AcsRun_Push(run, a2>=a1);
			break;

		case 25:
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetLocalVar(run, sp, a1);
			break;
		case 26:
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetMapVar(run, sp, a1);
			break;
		case 27:
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetWorldVar(run, sp, a1);
			break;

		case 28:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			P_AcsRun_Push(run, a1);
			break;
		case 29:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			P_AcsRun_Push(run, a1);
			break;
		case 30:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			P_AcsRun_Push(run, a1);
			break;

		case 31:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetLocalVar(run, sp, a1+a2);
			break;
		case 32:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetMapVar(run, sp, a1+a2);
			break;
		case 33:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetWorldVar(run, sp, a1+a2);
			break;

		case 34:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetLocalVar(run, sp, a1-a2);
			break;
		case 35:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetMapVar(run, sp, a1-a2);
			break;
		case 36:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetWorldVar(run, sp, a1-a2);
			break;

		case 37:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetLocalVar(run, sp, a1*a2);
			break;
		case 38:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetMapVar(run, sp, a1*a2);
			break;
		case 39:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			a2=P_AcsRun_Pop(run);
			P_AcsRun_SetWorldVar(run, sp, a1*a2);
			break;

		case 40:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetLocalVar(run, sp, a1/a2);
			break;
		case 41:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetMapVar(run, sp, a1/a2);
			break;
		case 42:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetWorldVar(run, sp, a1/a2);
			break;

		case 43:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetLocalVar(run, sp, a1%a2);
			break;
		case 44:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetMapVar(run, sp, a1%a2);
			break;
		case 45:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			a2=P_AcsRun_Pop(run);
			if(!a2)
			{	run->status=-2;
				break;		}
			P_AcsRun_SetWorldVar(run, sp, a1%a2);
			break;


		case 46:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			P_AcsRun_SetLocalVar(run, sp, a1+1);
			break;
		case 47:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			P_AcsRun_SetMapVar(run, sp, a1+1);
			break;
		case 48:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			P_AcsRun_SetWorldVar(run, sp, a1+1);
			break;

		case 49:
			sp=*run->cs++;
			a1=P_AcsRun_GetLocalVar(run, sp);
			P_AcsRun_SetLocalVar(run, sp, a1-1);
			break;
		case 50:
			sp=*run->cs++;
			a1=P_AcsRun_GetMapVar(run, sp);
			P_AcsRun_SetMapVar(run, sp, a1-1);
			break;
		case 51:
			sp=*run->cs++;
			a1=P_AcsRun_GetWorldVar(run, sp);
			P_AcsRun_SetWorldVar(run, sp, a1-1);
			break;
		
		case 52:
			sp=*run->cs++;
			run->cs=(int *)(p_acsvm_imgdat+sp);
			break;
		case 53:
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			if(a1)
			{
				run->cs=(int *)(p_acsvm_imgdat+sp);
			}
			break;
		case 54:	/* drop item */
			P_AcsRun_Pop(run);
			break;
		case 55:	/* delay */
			a1=P_AcsRun_Pop(run);
			run->delay=a1;
			run->status=3;
			break;
		case 56:	/* delaydirect */
			a1=*run->cs++;
			run->delay=a1;
			run->status=3;
			break;
		case 57:	/* random(lo, hi) */
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			if(a1>a2)
				{ a3=a1; a1=a2; a2=a3; }
			if(a2>a1)
				a3=a1+(rand()%(a2-a1));
			else
				a3=a1;
			P_AcsRun_Push(run, a3);
			break;
		case 58:
			a1=*run->cs++;
			a2=*run->cs++;
			if(a1>a2)
				{ a3=a1; a1=a2; a2=a3; }
			if(a2>a1)
				a3=a1+(rand()%(a2-a1));
			else
				a3=a1;
			P_AcsRun_Push(run, a3);
			break;
		case 59:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			a3=P_AcsRun_ThingCount(run, a1, a2);
			P_AcsRun_Push(run, a3);
			break;
		case 60:
			a1=*run->cs++;
			a2=*run->cs++;
			a3=P_AcsRun_ThingCount(run, a1, a2);
			P_AcsRun_Push(run, a3);
			break;
		case 61:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_TagWait(run, a1);
			break;
		case 62:
			a1=*run->cs++;
			P_AcsRun_TagWait(run, a1);
			break;
		case 63:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_PolyWait(run, a1);
			break;
		case 64:
			a1=*run->cs++;
			P_AcsRun_PolyWait(run, a1);
			break;
		case 65:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_ChangeFloor(run, a1, a2);
			break;
		case 66:
			a1=*run->cs++;
			a2=*run->cs++;
			P_AcsRun_ChangeFloor(run, a1, a2);
			break;
		case 67:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_ChangeCeiling(run, a1, a2);
			break;
		case 68:
			a1=*run->cs++;
			a2=*run->cs++;
			P_AcsRun_ChangeCeiling(run, a1, a2);
			break;
		case 69:
			P_AcsTryStartRun(run);
			break;
		case 70:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, (a1&&a2)?1:0);
			break;
		case 71:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, (a1||a2)?1:0);
			break;
		case 72:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1&a2);
			break;
		case 73:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1|a2);
			break;
		case 74:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1^a2);
			break;
		case 75:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, !a1);
			break;
		case 76:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1<<a2);
			break;
		case 77:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, a1>>a2);
			break;
		case 78:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_Push(run, -a1);
			break;
		case 79:
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			if(!a1)
			{
				run->cs=(int *)(p_acsvm_imgdat+sp);
			}
			break;


		case 81:	/* scriptwait */
			a1=P_AcsRun_Pop(run);
			P_AcsRun_ScriptWait(run, a1);
			break;
		case 82:	/* scriptwaitdirect */
			a1=*run->cs++;
			P_AcsRun_ScriptWait(run, a1);
			break;
		case 83:
			P_AcsRun_ClearLineSpecial(run);
			break;
		case 84:
			a2=*run->cs++;
			sp=*run->cs++;
			a1=P_AcsRun_Pop(run);
			if(a1==a2)
			{
				run->cs=(int *)(p_acsvm_imgdat+sp);
			}
			break;
		case 85:
			P_AcsRun_BeginPrint(run);
			break;
		case 86:
			P_AcsRun_EndPrint(run);
			break;
		case 87:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_PrintString(run, a1);
			break;
		case 88:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_PrintNumber(run, a1);
			break;
		case 89:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_PrintChar(run, a1);
			break;
		case 90:	/* Player Count */
			P_AcsRun_Push(run, 1);
			break;
		case 91:	/* Game Type */
			P_AcsRun_Push(run, 0);
			break;
		case 92:	/* Get Skill */
			P_AcsRun_Push(run, 0);
			break;
		case 93:	/* Timer Tics */
//			a1=I_GetTime ();
			a1=leveltime;
			P_AcsRun_Push(run, a1);
			break;

		case 94:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SectorSound(run, a1, a2);
			break;
		case 95:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_AmbientSound(run, a1, a2);
			break;
		case 96:
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SoundSequence(run, a1);
			break;
		case 97:
			a4=P_AcsRun_Pop(run);
			a3=P_AcsRun_Pop(run);
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetLineTexture(run, a1, a2, a3, a4);
			break;
		case 98:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetLineBlocking(run, a1, a2);
			break;
		case 99:
			a7=P_AcsRun_Pop(run);
			a6=P_AcsRun_Pop(run);
			a5=P_AcsRun_Pop(run);
			a4=P_AcsRun_Pop(run);
			a3=P_AcsRun_Pop(run);
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetLineSpecial(run, a1, a2, a3, a4, a5, a6, a7);
			break;
		case 100:
			a3=P_AcsRun_Pop(run);
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_ThingSound(run, a1, a2, a3);
			break;
		case 101:
			P_AcsRun_EndPrintBold(run);
			break;
		case 102:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_ActivatorSound(run, a1, a2);
			break;
		case 103:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_LocalAmbientSound(run, a1, a2);
			break;
		case 104:
			a2=P_AcsRun_Pop(run);
			a1=P_AcsRun_Pop(run);
			P_AcsRun_SetLineMonsterBlocking(run, a1, a2);
			break;

		default:
			run->status=-1;	//fault
			break;
			
		}
	}
}


void P_AcsCheckDestroy(p_acsrun_t *run)
{
//	printf("P_AcsCheckDestroy: %d\n",
//		run->status);

	if(run->status==1)
	{
		p_acsvm_scr_term[run->idx]=1;
		run->map=-1;
	}

//	if(run->status==-1)
	if(run->status<0)
	{
		run->map=-1;
	}
}

void P_AcsRunScript(int idx, int map, int arg1, int arg2, int arg3)
{
	p_acsrun_t *run;

	if((map>0) && (map!=gamemap))
	{
		run=P_AcsLookupRun(idx, map);
		if(run)
			return;

		run=P_AcsAllocRun();
		if(!run)
			return;

		run->idx=idx;
		run->map=map;
		run->arg1=arg1;
		run->arg2=arg2;
		run->arg3=arg3;
		return;
	}

	run=P_AcsLookupRun(idx, map);
	if(run)
	{
		if(run->status==2)
			run->status=0;
		P_AcsRunLoop(run);
		P_AcsCheckDestroy(run);
		return;
	}

	run=P_AcsAllocRun();
	if(!run)
		return;
	
	if(!map)
		map=gamemap;
	
	run->idx=idx;
	run->map=map;
	run->arg1=arg1;
	run->arg2=arg2;
	run->arg3=arg3;

	run->trigline=trigline;

	P_AcsTryStartRun(run);
	P_AcsRunLoop(run);
	P_AcsCheckDestroy(run);

	return;
}

void P_AcsTick()
{
	p_acsrun_t *run;
	int mt, mi;
	int i, j, k;

	if(p_acsvm_newmap)
	{
		for(i=0; i<p_acsvm_nrun; i++)
		{
			run=p_acsvm_run+i;
			if(run->map<0)
				continue;

			if(run->map==gamemap)
			{
				run->css=NULL;
				run->cs=NULL;
				P_AcsTryStartRun(run);
				P_AcsRunLoop(run);
				P_AcsCheckDestroy(run);
			}else
			{
				if(run->status || run->css)
				{
					run->map=-1;
				}

				run->css=NULL;
				run->cs=NULL;
			}
		}
	}

	for(i=0; i<p_acsvm_nrun; i++)
	{
		run=p_acsvm_run+i;
		if(run->map<0)
			continue;
		
		if(run->status==3)
		{
			run->delay--;
			if(run->delay<=0)
			{
				run->status=0;
				P_AcsRunLoop(run);
				P_AcsCheckDestroy(run);
				continue;
			}
		}

		if(run->status==4)
		{
			run->status=0;
			P_AcsRunLoop(run);
			P_AcsCheckDestroy(run);
			continue;
		}
	}
	
	if(p_acsvm_newmap)
	{
		p_acsvm_newmap = 0;

		for(i=0; i<p_acsvm_scr_num; i++)
		{
//			if((p_acsvm_scr_idn[i]%1000)!=run->idx)
//				continue;

			j=p_acsvm_scr_idn[i];
			if(j<1000)
				continue;
			
			mt=j/1000;
			mi=j%1000;
			if(mt==1)	//First entering map
			{
				P_AcsRunScript(mi, 0,  0, 0, 0);
			}				

			if(mt==4)	//Player enters map
			{
				P_AcsRunScript(mi, 0,  0, 0, 0);
			}				
		}
	}
}
