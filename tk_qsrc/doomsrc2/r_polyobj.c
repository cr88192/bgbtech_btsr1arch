#include "doomdef.h"

#include "m_bbox.h"

#include "i_system.h"

#include "r_main.h"
#include "r_plane.h"
#include "r_things.h"

#include "p_local.h"

// State.
#include "doomstat.h"
#include "r_state.h"

//#include "r_local.h"

typedef struct polyobj_s polyobj_t;

struct polyobj_s {
    seg_t	*segs[256];
    int		n_segs;
    int		tid;
    
    int bb_x1, bb_y1;
    int bb_x2, bb_y2;
};

polyobj_t	*r_polyobj_lut[256];


byte r_ispolyobj;		//rendering a PolyObj
byte r_usezbuff;		//need Z buffer?
int r_nseenpolyobj;		//number of polyobj seen this frame

byte r_activepolyobj;	//number of active PolyObj's


polyobj_t	*R_GetPolyObjInfoForId(int tid)
{
	polyobj_t	*tmp;
	
	tmp = r_polyobj_lut[tid];
	if(!tmp)
	{
		tmp=malloc(sizeof(polyobj_t));
		memset(tmp, 0, sizeof(polyobj_t));
		
		tmp->tid = tid;
		r_polyobj_lut[tid] = tmp;
	}
	
	return(tmp);
}

void	R_FlushPolyObj()
{
	polyobj_t	*tmp;
	int i;

	for(i=0; i<256; i++)
	{
		tmp=r_polyobj_lut[i];
		if(!tmp)
			continue;
		tmp->n_segs=0;
	}
	
	r_usezbuff = 0;
	r_activepolyobj = 0;
}

mobj_t	*EV_FindPolyObjForId(int tid)
{
	thinker_t*	thinker;
	mobj_t*	m;

	thinker = thinkercap.next;
	for (thinker = thinkercap.next;
	 thinker != &thinkercap;
	 thinker = thinker->next)
	{
		if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
			continue;	

		m = (mobj_t *)thinker;

//		if (m->spawnpoint.type != 9300)
		if (m->type != MT_POLYOBJ0)
			continue;

		if (m->spawnpoint.angle == tid)
			return(m);
	}
	return(NULL);
}

void R_RebuildPolyObjInfo(polyobj_t	*poi)
{
    seg_t	*seg;
    line_t	*line;
    seg_t	*seg1;
    seg_t	*seg2;
    line_t	*line2;
	int		i, j, k;
	
	poi->n_segs=0;
	poi->bb_x1=0;	poi->bb_y1=0;
	poi->bb_x2=0;	poi->bb_y2=0;
	
	for(i=0; i<numsegs; i++)
	{
		seg=segs+i;
		line=seg->linedef;
		
		if(line->acs_spec==5)
			if(line->arg1==poi->tid)
		{
			j=poi->n_segs++;
			poi->segs[j]=seg;
			continue;
		}

		if(line->acs_spec!=1)
			continue;
		if(line->arg1!=poi->tid)
			continue;
		j=poi->n_segs++;
		poi->segs[j]=seg;

		seg1=seg;
		while(1)
		{
			for(j=0; j<numsegs; j++)
			{
				seg2=segs+j;
				if(seg2->v1==seg1->v2)
					break;
			}
			
			if(j>=numsegs)
				break;

			if(seg2==seg)
				break;

//			line2=seg2->linedef;
			k=poi->n_segs++;
			poi->segs[k]=seg2;
			seg1=seg2;
		}
	}

	for(i=0; i<poi->n_segs; i++)
	{
		seg=poi->segs[i];
		line=seg->linedef;

		if(line->acs_spec!=5)
			continue;

		for(j=i+1; j<poi->n_segs; j++)
		{
			seg=poi->segs[i];
			line=seg->linedef;

			seg2=poi->segs[j];
			line2=seg2->linedef;

			if(line2->acs_spec!=5)
				continue;
			
			if(line2->arg2 < line2->arg2)
			{
				poi->segs[i]=seg2;
				poi->segs[j]=seg;
			}
		}
	}
}

int R_BlkMapCheckOversizeP (int x, int y)
{
	if(!r_activepolyobj)
		return(0);
	return(1);
}

int R_BlkMapCheckBlockablesP (void)
{
	if(!r_activepolyobj)
		return(0);
	return(1);
}

int R_ThingIsPolyObjP (mobj_t* thing)
{
	if((thing->type>=MT_POLYOBJ1) &&
		(thing->type<=MT_POLYOBJ3))
			return(true);

	return(false);
}

int R_PolyObjGetBox (mobj_t* thing,
	fixed_t *rx1, fixed_t *ry1, fixed_t *rx2, fixed_t *ry2)
{
	mobj_t		*pobj;
	polyobj_t	*poi;
    seg_t		*seg;
    line_t		*line;
    int			tid;
    fixed_t		dx, dy, tx, ty;
    fixed_t		x1, y1, x2, y2;
    int			i;

	tid = thing->spawnpoint.angle;
	pobj = EV_FindPolyObjForId(tid);
	if(!pobj)
	{
		*rx1=0;
		*ry1=0;
		*rx2=0;
		*ry2=0;
		return(false);
	}

	poi = R_GetPolyObjInfoForId(tid);

	if(poi->bb_x1 || poi->bb_y2)
	{
		*rx1=poi->bb_x1;
		*ry1=poi->bb_y1;
		*rx2=poi->bb_x2;
		*ry2=poi->bb_y2;
		return(true);
	}

	dx = thing->x - pobj->x;
	dy = thing->y - pobj->y;

	x1=MAXINT;		y1=MAXINT;
	x2=-MAXINT;		y2=-MAXINT;

	for(i=0; i<poi->n_segs; i++)
	{
		seg=poi->segs[i];
		line=seg->linedef;
		
		tx = seg->v1->x + dx;
		ty = seg->v1->y + dy;
		x1=r_int_min(x1, tx);
		y1=r_int_min(y1, ty);
		x2=r_int_max(x2, tx);
		y2=r_int_max(y2, ty);

		tx = seg->v2->x + dx;
		ty = seg->v2->y + dy;
		x1=r_int_min(x1, tx);
		y1=r_int_min(y1, ty);
		x2=r_int_max(x2, tx);
		y2=r_int_max(y2, ty);
	}

	poi->bb_x1=x1;
	poi->bb_y1=y1;
	poi->bb_x2=x2;
	poi->bb_y2=y2;

	*rx1=x1;
	*ry1=y1;
	*rx2=x2;
	*ry2=y2;
	return(true);
}

void R_ProjectSprite_PolyObj (mobj_t* thing)
{
    subsector_t		*ss;
	mobj_t			*pobj;
	polyobj_t		*poi;
    seg_t			*line;
    int				count;
    int		tid;
    int		i;
    
    int		oldviewx;
    int		oldviewy;

//	if((thing->spawnpoint.type>=9301) &&
//		(thing->spawnpoint.type>=9303))
	if((thing->type>=MT_POLYOBJ1) &&
		(thing->type<=MT_POLYOBJ3))
	{
		tid = thing->spawnpoint.angle;
		pobj = EV_FindPolyObjForId(tid);
		if(!pobj)
			return;
		
		r_usezbuff = 1;
		r_nseenpolyobj++;

		poi = R_GetPolyObjInfoForId(tid);
		if(!poi->n_segs)
		{
			R_RebuildPolyObjInfo(poi);
			r_activepolyobj++;
		}
			
		ss = pobj->subsector;

		count = ss->numlines;
		line = &segs[ss->firstline];

		oldviewx = viewx;
		oldviewy = viewy;
		
		viewx -= thing->x - pobj->x;
		viewy -= thing->y - pobj->y;
		r_ispolyobj = 1;

#if 0
		while (count--)
		{
			R_AddLine (line);
			line++;
		}
#endif

		for(i=0; i<poi->n_segs; i++)
		{
			line=poi->segs[i];
			R_AddLine (line);
		}

		viewx = oldviewx;
		viewy = oldviewy;
		r_ispolyobj = 0;
		
		return;
	}
}

