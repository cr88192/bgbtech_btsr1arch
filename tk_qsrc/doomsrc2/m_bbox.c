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
//	Main loop menu stuff.
//	Random number LUT.
//	Default Config File.
//	PCX Screenshots.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include "doomtype.h"

#include "m_bbox.h"




void M_ClearBox (fixed_t *box)
{
	box[BOXTOP] = box[BOXRIGHT] = MININT;
	box[BOXBOTTOM] = box[BOXLEFT] = MAXINT;

//	box[0] = box[3] = MININT;
//	box[1] = box[2] = MAXINT;
}

void
M_AddToBox
( fixed_t*	box,
  fixed_t	x,
  fixed_t	y )
{
//	if (x<box[BOXLEFT])
//		box[BOXLEFT] = x;
//	else if (x>box[BOXRIGHT])
//		box[BOXRIGHT] = x;
//	if (y<box[BOXBOTTOM])
//		box[BOXBOTTOM] = y;
//	else if (y>box[BOXTOP])
//		box[BOXTOP] = y;

	if (x<box[BOXLEFT])
		box[BOXLEFT] = x;
	if (x>box[BOXRIGHT])
		box[BOXRIGHT] = x;
	if (y<box[BOXBOTTOM])
		box[BOXBOTTOM] = y;
	if (y>box[BOXTOP])
		box[BOXTOP] = y;

//	if (x<box[2])
//		box[2] = x;
//	if (x>box[3])
//		box[3] = x;
//	if (y<box[1])
//		box[1] = y;
//	if (y>box[0])
//		box[0] = y;
}


int
M_CheckBoxCollide
( fixed_t	*box1,
  fixed_t	*box2)
{
	if(box1[BOXTOP] < box2[BOXBOTTOM])
		return(false);
	if(box1[BOXBOTTOM] > box2[BOXTOP])
		return(false);

	if(box1[BOXRIGHT] < box2[BOXLEFT])
		return(false);
	if(box1[BOXLEFT] > box2[BOXRIGHT])
		return(false);

	return(true);
}



