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
// #include <dos.h>
// #include <io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include <conio.h>
#include <ctype.h>

#include "rt_def.h"
#include "rt_main.h"
#include "rt_game.h"
#include "_rt_game.h"
#include "rt_menu.h"
#include "z_zone.h"
#include "w_wad.h"
#include "lumpy.h"
#include "rt_playr.h"
#include "rt_util.h"
#include "rt_ted.h"
#include "rt_draw.h"
#include "rt_view.h"
#include "rt_vid.h"
#include "rt_door.h"
#include "rt_in.h"
#include "rt_str.h"
#include "isr.h"
#include "rt_build.h"
#include "rt_rand.h"
#include "rt_cfg.h"
#include "rt_sound.h"
#include "version.h"
#include "rt_crc.h"
#include "modexlib.h"
#include "engine.h"
#include "gmove.h"
#include "rt_com.h"
#include "rt_net.h"
#include "watcom.h"
#include "rt_floor.h"
#include "rt_msg.h"
#include "rt_scale.h"
#include "develop.h"
//MED
// #include "memcheck.h"


#if (SHAREWARE == 1)
#define NUMAMMOGRAPHICS 10
#else
#define NUMAMMOGRAPHICS 20
#endif

//******************************************************************************
//
// GLOBALS
//
//******************************************************************************

int PlayerSnds[5] = {SD_PLAYERTCSND, SD_PLAYERTBSND, SD_PLAYERDWSND,
							SD_PLAYERLNSND, SD_PLAYERIPFSND};


int SHAKETICS	= 0xFFFF;
int damagecount = 0;
HighScore	Scores[MaxScores] =
				{
					{"Tom",70000,7,1},
					{"Chuck",60000,6,1},
					{"Mark",50000,5,1},
					{"The Joes",40000,4,1},
					{"William",30000,3,1},
					{"Jim",20000,2,1},
					{"Steve",10000,1,1},
				};


//******************************************************************************
//
// LOCALS
//
//******************************************************************************

static int KeyX[4]  = {KEY1_X, KEY2_X, KEY3_X, KEY4_X};

// static char *Names[ 5 ] =
static char *Names[ 10 ] =
{
	"Taradino", "Thi",	  "Doug",  "Lorelei", "Ian Paul",
	"B.J.", NULL
};

//static char *LastNames[ 5 ] =
static char *LastNames[ 10 ] =
{
	"Cassatt",  "Barrett", "Wendt", "Ni",		"Freeley",
//	"Blazkowicz", NULL
	"Blaz'", NULL
};

static STR ScoreStr;
static STR LivesStr;
static STR TriadStr;
static STR KillStr;

static pic_t *lifeptnums[10];
static pic_t *lifenums[10];
static pic_t *timenums[10];
static pic_t *scorenums[10];
static pic_t *keys[4];
//static pic_t *men[5];
static pic_t *men[10];

static pic_t *health[6];
static pic_t *ammo[26];
static pic_t *erase;
static pic_t *eraseb;

static pic_t *fragpic[ 10 ];
static pic_t *frag100pic[ 10 ];
static pic_t *negfragpic[ 10 ];
// static pic_t *menneg[ 5 ];
static pic_t *menneg[ 10 ];
static pic_t *blankfragpic;

static int powerpics;
static int poweradjust;

static int poweruptime;
static int powerupheight;
static int protectiontime;
static int protectionheight;


static boolean EndLevelStuff = false;
static boolean borderset	  = false;
static int oldsec				= -1;

static pic_t *BkPic;
int SaveTime = 0;
int oldhealth;

static int oldplayerhealth;
static int oldpercenthealth;

static int playeruniformcolor;

#define NUMBONUSES	11
#define BONUSBONUS	100000

//******************************************************************************
//
// V_ReDrawBkgnd ()
//
//******************************************************************************

void V_ReDrawBkgnd (int x, int y, int width, int height, boolean shade)
{
	byte *src;
//	byte *dest;
	unsigned dest;
//	byte *origdest;
	unsigned origdest;
	int j,
		 k,
		 planes,
		 mask,
		 m;

	m = (x&3);
	mask = (1 << m);

//	origdest = (byte *)(bufferofs+ylookup[y]+(x>>2));
	origdest = (bufferofs+ylookup[y]+(x>>2));


	if (VW_MarkUpdateBlock (x, y, x+width-1, y+height-1))
	{
		for (planes = 0; planes < 4; planes++)
		{
			src = (&(BkPic->data)+((80*200)*m)+(80*y)+(x>>2));
			dest = origdest;

			VGAMAPMASK (mask);

			for (j = 0; j < height; j++)
			{
				for (k = 0; k < (width/4); k++)
					if (shade)
//						*dest++ = *(colormap + ((MENUSHADELEVEL>>2)<<8) + *src++);
						VGAWRITE(dest++, *(colormap + ((MENUSHADELEVEL>>2)<<8) + *src++));
					else
//						*dest++ = *src++;
						VGAWRITE(dest++, *src++);

				src += (80-(width/4));
				dest += (linewidth-(width/4));
			}

			m++;
			mask <<= 1;
			if (mask == 16)
			{
				x+=4;
				mask = 1;
				m = 0;
				origdest++;
			}
		}
	}
}

//******************************************************************************
//
// CacheLumpGroup ()
//
//******************************************************************************
void CacheLumpGroup
	(
	char	*startlump,
	pic_t **lumparray,
	int	  numberoflumps
	)
{
	int lumpnum;
	int i;

	lumpnum = W_GetNumForName( startlump );

	for( i = 0; i < numberoflumps; i++ )
	{
		lumparray[ i ] = ( pic_t * )W_CacheLumpNum( lumpnum + i, PU_LEVEL );
	}
}

//******************************************************************************
//
// SetupPlayScreen ()
//
//******************************************************************************
void SetupPlayScreen( void )
{
	int i;
	int j;
	int num;

	erase  = ( pic_t * )W_CacheLumpName( "erase", PU_LEVEL );
	eraseb = ( pic_t * )W_CacheLumpName( "eraseb", PU_LEVEL );

	CacheLumpGroup( "tmnum0", timenums, 10 );
	CacheLumpGroup( "lfnum0", lifeptnums, 10 );
	CacheLumpGroup( "lvnum0", lifenums, 10 );
	CacheLumpGroup( "health1b", health, 6 );
	CacheLumpGroup( "key1", keys, 4 );

	for(i=0; i<10; i++)
		men[ i ] = ( pic_t * )W_CacheLumpNum(
			W_GetNumForName( "MAN1" ), PU_LEVEL  );

	if ( !BATTLEMODE )
	{
		CacheLumpGroup( "scnum0", scorenums, 10 );

		num = locplayerstate->player;
		men[ num ] = ( pic_t * )W_CacheLumpNum( W_GetNumForName( "MAN1" ) +
			num, PU_LEVEL );
	}
	else
	{
		int  man;
		int  num100;
		int  negnum;
		int  negman;

		CacheLumpGroup( "kilnum0", scorenums, 10 );

		negnum  = W_GetNumForName( "botnpic1" );
		num	  = W_GetNumForName( "botpic0" );
		num100  = W_GetNumForName( "botopic1" );
		negman  = W_GetNumForName( "negman1" );
		man	  = W_GetNumForName( "man1" );

		blankfragpic = ( pic_t * )W_CacheLumpNum( num, PU_LEVEL );
		num++;

		for( i = 0; i < numplayers; i++ )
		{
			j = PLAYERSTATE[ i ].player;
			if ( !gamestate.teamplay )
			{
				fragpic[ j ]	 = ( pic_t * )W_CacheLumpNum( num + j, PU_LEVEL );
				frag100pic[ j ] = ( pic_t * )W_CacheLumpNum( num100 + j, PU_LEVEL );
				negfragpic[ j ] = ( pic_t * )W_CacheLumpNum( negnum + j, PU_LEVEL );
			}
			else
			{
				negfragpic[ j ] = ( pic_t * )W_CacheLumpName( "teamnpic", PU_LEVEL );
				fragpic[ j ]	 = ( pic_t * )W_CacheLumpName( "teampic", PU_LEVEL );
				frag100pic[ j ] = fragpic[ j ];
			}

			menneg[ j ]	  = ( pic_t * )W_CacheLumpNum( negman + j, PU_LEVEL );
			men[ j ]		  = ( pic_t * )W_CacheLumpNum( man + j, PU_LEVEL );
		}
	}

	powerpics	= W_GetNumForName( "GDMODEP" );
	poweradjust = POWERUPTICS / 16;

	num	= W_GetNumForName( "INF_B" );

	// bullet weapons
	ammo[0] = ( pic_t * )W_CacheLumpNum( num, PU_LEVEL );
	ammo[1] = ( pic_t * )W_CacheLumpNum( num, PU_LEVEL );
	ammo[2] = ( pic_t * )W_CacheLumpNum( num++, PU_LEVEL );

	for(i=3;i < 13; i++ )
	{
		ammo[ i ] = ( pic_t * )W_CacheLumpNum( num++, PU_LEVEL );
	}

	ammo[13] = ( pic_t * )W_CacheLumpNum( num, PU_LEVEL );
	ammo[14] = ( pic_t * )W_CacheLumpNum( num, PU_LEVEL );
	ammo[15] = ( pic_t * )W_CacheLumpNum( num++, PU_LEVEL );


	for(i=16;i < 26; i++ )
	{
		ammo[ i ] = ( pic_t * )W_CacheLumpNum( num++, PU_LEVEL );
	}


	oldplayerhealth  = -1;
	oldpercenthealth = -1;
}


//******************************************************************************
//
// GameMemToScreen()
//
//******************************************************************************

void GameMemToScreen
	(
	pic_t *source,
	int	x,
	int	y,
	int	bufferofsonly
	)

{
	if ( bufferofsonly )
	{
		VL_MemToScreen( ( byte * )&source->data, source->width,
			source->height, x, y );
	}
	else
	{
		GM_MemToScreen( ( byte * )&source->data, source->width,
			source->height, x, y );
	}
}


//******************************************************************************
//
// DrawPlayScreen ()
//
//******************************************************************************
void DrawPlayScreen
	(
	boolean bufferofsonly
	)

{
	pic_t *shape;
	int	 shapenum;

	if ( SHOW_TOP_STATUS_BAR() )
	{
		shape = W_CacheLumpName( "stat_bar", PU_CACHE );
		GameMemToScreen( shape, 0, 0, bufferofsonly );
	}

	if ( BATTLEMODE )
	{
		DrawKills( bufferofsonly );
	}

	if ( SHOW_BOTTOM_STATUS_BAR() )
	{
		shape = ( pic_t * ) W_CacheLumpName( "bottbar", PU_CACHE );

		if ( SHOW_KILLS() )
		{
			GameMemToScreen( shape, 0, 160, bufferofsonly );
		}
		else
		{
			GameMemToScreen( shape, 0, 184, bufferofsonly );
		}

		DrawBarAmmo( bufferofsonly );
		DrawBarHealth( bufferofsonly );

		if ( demoplayback )
		{
			shape = ( pic_t * )W_CacheLumpName( "demo", PU_CACHE );
			DrawPPic( 148, 185, shape->width, shape->height,
				( byte * )&shape->data, 1, true, bufferofsonly );
		}
	}

	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}


	oldsec = -1;

	DrawTime( bufferofsonly );

	if ( !BATTLEMODE )
	{
		int character;
		int width;
		int height;

		width=0;
		height=0;

		character = locplayerstate->player;
		if(rott_iswolf)
			character = 5;
		
		GameMemToScreen( men[ character ], MEN_X, MEN_Y,
			bufferofsonly );

		CurrentFont = tinyfont;

		// Draw player's name
		DrawGameString ( MEN_X + 3, MEN_Y + 2, Names[ character ], bufferofsonly );

		VW_MeasurePropString( LastNames[ character ], &width, &height );

		//BGB: ? Past end of array ?
//		DrawGameString ( MEN_X + 44 - width, MEN_Y + 8,
//			Names[ character + 5 ], bufferofsonly );
		DrawGameString ( MEN_X + 44 - width, MEN_Y + 8,
			LastNames[ character ], bufferofsonly );

		UpdateLives( locplayerstate->lives );
		UpdateScore( gamestate.score );
		DrawTriads( bufferofsonly );
		DrawLives( bufferofsonly );
		DrawScore( bufferofsonly );
	}

	DrawKeys( bufferofsonly );

	if ( locplayerstate->poweruptime )
	{
		if ( player->flags & FL_GODMODE )
		{
			shapenum = powerpics;
		}
		else if ( player->flags & FL_DOGMODE )
		{
			shapenum = powerpics + 1;
		}
		else if ( player->flags & FL_FLEET )
		{
			shapenum = powerpics + 2;
		}
		else if ( player->flags & FL_ELASTO )
		{
			shapenum = powerpics + 3;
		}
		else if ( player->flags & FL_SHROOMS )
		{
			shapenum = powerpics + 4;
		}

		shape = ( pic_t * )W_CacheLumpNum ( shapenum, PU_CACHE );

		GameMemToScreen( eraseb, POWERUP1X, POWERUPY, bufferofsonly );

		DrawMPPic( POWERUP1X, POWERUPY + powerupheight, shape->width,
			shape->height - powerupheight, powerupheight,
			( byte * )&shape->data, bufferofsonly );
	}

	if ( locplayerstate->protectiontime )
	{
		if ( player->flags & FL_BPV )
		{
			shapenum = powerpics + 6;
		}
		else if ( player->flags & FL_GASMASK )
		{
			shapenum = powerpics + 5;
		}
		else if ( player->flags & FL_AV )
		{
			shapenum = powerpics + 7;
		}

		shape = ( pic_t * )W_CacheLumpNum( shapenum, PU_CACHE );

		GameMemToScreen( eraseb, POWERUP2X, POWERUPY, bufferofsonly );

		DrawMPPic( POWERUP2X, POWERUPY + protectionheight, shape->width,
			shape->height - protectionheight, protectionheight,
			( byte * )&shape->data, bufferofsonly );
	}
}


//******************************************************************************
//
// ShortenCodeName()
//
//******************************************************************************
void GetShortCodeName
	(
	char *dest,
	char *source,
	int  maxwidth
	)

{
	int width;
	int height;
	int length;

	width=0; height=0;

	strcpy( dest, source );

	// Shorten name to fit
	length = strlen( dest );
	VW_MeasurePropString( dest, &width, &height );
	while( width > maxwidth )
	{
		dest[ length ] = 0;
		length--;
		VW_MeasurePropString( dest, &width, &height );
	}
}


//******************************************************************************
//
// DrawKills ()
//
//******************************************************************************
void DrawKills
	(
	boolean bufferofsonly
	)
{
	int  rank;
	int  xpos;
	char codename[ MAXCODENAMELENGTH ];
	int  width;
	int  playernum;
	int  playerimage;
	int  temp;
	pic_t *pic;

	CurrentFont = tinyfont;

	if ( SHOW_TOP_STATUS_BAR() )
	{
		playernum	= BATTLE_Team[ consoleplayer ];
		playerimage = BATTLE_TeamLeader[ playernum ];

		// Set uniformcolor
		playeruniformcolor = PLAYERSTATE[ playerimage ].uniformcolor;

		// Draw player's point box
		pic = men[ PLAYERSTATE[ playerimage ].player ];
		if ( ( gamestate.ShowScores ) && ( BATTLE_Points[ playernum ] < 0 ) )
		{
			pic = menneg[ PLAYERSTATE[ playerimage ].player ];
		}

		DrawPPic( MEN_X, MEN_Y, pic->width, pic->height,
			( byte * )&pic->data, 1, true, bufferofsonly );

		// Draw player's name
		if ( gamestate.teamplay )
		{
			GetShortCodeName( codename, colorname[ playeruniformcolor ],
				42 );
		}
		else
		{
			GetShortCodeName( codename, PLAYERSTATE[ playerimage ].codename,
				42 );
		}

		DrawGameString ( MEN_X + 2, MEN_Y + 2, codename, bufferofsonly );
		// Draw "It" if player is 'it'
		if ( ( ( gamestate.battlemode == battle_Tag ) ||
			( gamestate.battlemode == battle_Hunter ) ) &&
			( BATTLE_It == BATTLE_Team[ consoleplayer ] ) )
		{
			DrawGameString ( MEN_X + 22, MEN_Y + 8,
				"It", bufferofsonly);
		}

		// Draw triad if player is 'it' or has caught a triad
		if ( PLAYER[ consoleplayer ]->flags & FL_DESIGNATED )
		{
			pic = W_CacheLumpName( "smalltri", PU_CACHE );
			DrawPPic( TRIAD_X - 1, TRIAD_Y - 2, pic->width, pic->height,
				( byte * )&pic->data, 1, true, bufferofsonly );
		}
		else if ( ( gamestate.ShowScores ) &&
			( DisplayPoints != bo_kills_infinite ) )
		{
			// Draw Kill goal
			if ( ( gamestate.battlemode == battle_Collector ) ||
				( gamestate.battlemode == battle_StandAloneGame ) )
			{
				temp = BATTLE_NumCollectorItems;
			}
			else
			{
				temp = DisplayPoints;
			}

			rt_ltoa ( temp % 1000, KillStr.str, 10);
			KillStr.length = strlen (KillStr.str);
			DrawNumber (TRIAD_X - 6, TRIAD_Y, 3, 5, bufferofsonly);
		}

		// Set uniformcolor
		playeruniformcolor = PLAYERSTATE[ consoleplayer ].uniformcolor;

		if ( gamestate.ShowScores )
		{
			// Draw local player's points
			temp = BATTLE_Points[ playernum ] % 1000;
			if ( temp < 0 )
			{
				temp = -temp;
			}
			rt_ltoa ( temp, KillStr.str, 10);
			KillStr.length = strlen (KillStr.str);

			DrawNumber( LIVES_X - 12, LIVES_Y, 3, 4, bufferofsonly);
		}
		else
		{
			pic = W_CacheLumpName( "minus", PU_CACHE );
			StatusDrawColoredPic( LIVES_X - 12, LIVES_Y, pic, bufferofsonly, playeruniformcolor );
			StatusDrawColoredPic( LIVES_X - 4, LIVES_Y, pic, bufferofsonly, playeruniformcolor );
		}

		// Draw whoever is 'It'
		playernum	= BATTLE_It;
		playerimage = BATTLE_TeamLeader[ playernum ];

		// Set uniformcolor
		playeruniformcolor = PLAYERSTATE[ playerimage ].uniformcolor;

		// Draw player's point box
		pic = men[ PLAYERSTATE[ playerimage ].player ];
		if ( ( gamestate.ShowScores ) && ( BATTLE_Points[ playernum ] < 0 ) )
		{
			pic = menneg[ PLAYERSTATE[ playerimage ].player ];
		}

		DrawPPic( LEADER_X, LEADER_Y, pic->width, pic->height,
			(byte *)&pic->data, 1, true, bufferofsonly );

		if ( ( gamestate.battlemode == battle_Tag ) ||
			( gamestate.battlemode == battle_Hunter ) )
		{
			DrawGameString ( LEADER_X + 22, LEADER_Y + 8,
				"It", bufferofsonly);
		}

		if ( gamestate.ShowScores )
		{
			// Draw number of points
			temp = BATTLE_Points[ playernum ] % 1000;
			if ( temp < 0 )
			{
				temp = -temp;
			}
			rt_ltoa ( temp, KillStr.str, 10);
			KillStr.length = strlen (KillStr.str);
			DrawNumber ( LEADER_NUM_X, LEADER_NUM_Y, 3, 4, bufferofsonly);
		}
		else
		{
			pic = W_CacheLumpName( "minus", PU_CACHE );
			StatusDrawColoredPic( LEADER_NUM_X, LEADER_NUM_Y, pic, bufferofsonly, playeruniformcolor );
			StatusDrawColoredPic( LEADER_NUM_X + 8, LEADER_NUM_Y, pic, bufferofsonly, playeruniformcolor );
		}

		// Draw name
		if ( gamestate.teamplay )
		{
			DrawGameString ( LEADER_NAME_X, LEADER_NAME_Y - 1,
				colorname[ playeruniformcolor ], bufferofsonly);
		}
		else
		{
			GetShortCodeName( codename, PLAYERSTATE[ playerimage ].codename,
				42 );
			DrawGameString ( LEADER_NAME_X - 1, LEADER_NAME_Y,
				codename, bufferofsonly);
		}
	}

	// Only draw the rest of the rifraff when the kill count is selected
	if ( !SHOW_KILLS() )
	{
		return;
	}

	// Draw all the other losers
	xpos = KILLS_X;
	for( rank = 0; rank < BATTLE_NumberOfTeams; rank++ )
	{
		playernum	= BATTLE_PlayerOrder[ rank ];
		playerimage = BATTLE_TeamLeader[ playernum ];

		if ( ( playernum == BATTLE_It ) && SHOW_TOP_STATUS_BAR() )
		{
			continue;
		}

		// Set uniformcolor
		playeruniformcolor = PLAYERSTATE[ playerimage ].uniformcolor;

		// Draw player's point box
		pic = fragpic[ PLAYERSTATE[ playerimage ].player ];
		if ( gamestate.ShowScores )
		{
			if ( BATTLE_Points[ playernum ] < 0 )
			{
				pic = negfragpic[ PLAYERSTATE[ playerimage ].player ];
			}
			else if ( BATTLE_Points[ playernum ] >= 100 )
			{
				pic = frag100pic[ PLAYERSTATE[ playerimage ].player ];
			}
		}
		DrawPPic( xpos, KILLS_Y, pic->width, pic->height,
			(byte *)&pic->data, 1, true, bufferofsonly );

		// Draw number of points
		if ( gamestate.ShowScores )
		{
			temp = BATTLE_Points[ playernum ] % 1000;
			if ( temp < 0 )
			{
				temp = -temp;
			}
			rt_ltoa ( temp, KillStr.str, 10);
			KillStr.length = strlen (KillStr.str);
			width = 2;
			if ( temp > 99 )
			{
				width = 3;
			}
			DrawNumber( xpos + KILLS_OFFSET + 16 - ( 8 * width ),
				KILLS_Y, width, 4, bufferofsonly);
		}
		else
		{
			pic = W_CacheLumpName( "minus", PU_CACHE );
			StatusDrawColoredPic( ( xpos + KILLS_OFFSET ),
				KILLS_Y, pic, bufferofsonly, playeruniformcolor );
			StatusDrawColoredPic( ( xpos + KILLS_OFFSET + 8 ),
				KILLS_Y, pic, bufferofsonly, playeruniformcolor );
		}

		// Get codename
		if ( gamestate.teamplay )
		{
			GetShortCodeName( codename, colorname[ playeruniformcolor ],
				KILLS_WIDTH - 2 );
		}
		else
		{
			GetShortCodeName( codename, PLAYERSTATE[ playerimage ].codename,
				KILLS_WIDTH - 2 );
		}

		// Draw name
		DrawGameString (xpos + 1, KILLS_NAME_Y, codename, bufferofsonly);

		// Advance to next position
		xpos += KILLS_WIDTH;

		if ( xpos >= 320 )
		{
			break;
		}
	}

	for( rank = BATTLE_NumberOfTeams; rank <= MAXKILLBOXES; rank++ )
	{
		if ( xpos >= 320 )
		{
			break;
		}

		pic = blankfragpic;
		DrawPPic( xpos, KILLS_Y, pic->width, pic->height,
			(byte *)&pic->data, 1, true, bufferofsonly );

		// Advance to next position
		xpos += KILLS_WIDTH;
	}
}


//******************************************************************************
//
// DrawPlayers ()
//
//******************************************************************************
void DrawPlayers
	(
	void
	)

{
	int	 num;
	int	 xpos;
	char	codename[ MAXCODENAMELENGTH ];
	int	 length;
	int	 width;
	int	 height;
	int	 team;
	int	 player;
	int	 character;
	pic_t *pic;
	pic_t *enemy;
	pic_t *friend;

	width=0; height=0;

	num = W_GetNumForName( "botpic1" );

	scorenums[ 0 ]= ( pic_t * )W_CacheLumpName( "kilnum0", PU_CACHE );
	friend = ( pic_t * )W_CacheLumpName( "t_friend", PU_CACHE );
	enemy  = ( pic_t * )W_CacheLumpName( "t_enemy", PU_CACHE );

	// Draw all the losers
	CurrentFont = tinyfont;

	xpos = ( 320 - min( numplayers, MAXKILLBOXES ) * KILLS_WIDTH ) / 2;

	for( team = 0; team < BATTLE_NumberOfTeams; team++ )
	{
		for( player = 0; player < numplayers; player++ )
		{
			if ( BATTLE_Team[ player ] == team )
			{
				character = PLAYERSTATE[ player ].player;

				fragpic[ character ] = ( pic_t * )W_CacheLumpNum( num +
					character, PU_CACHE );

				if ( ( numplayers <= MAXKILLBOXES ) ||
					( player != consoleplayer ) )
				{
					// Set uniformcolor
					playeruniformcolor = PLAYERSTATE[ player ].uniformcolor;

					// Draw player's point box
					pic = fragpic[ PLAYERSTATE[ player ].player ];

					VWB_DrawPic ( xpos, PLAYERS_Y, pic );
					if ( gamestate.teamplay )
					{
						if ( BATTLE_Team[ player ] == BATTLE_Team[ consoleplayer ] )
						{
							VWB_DrawPic ( xpos, PLAYERS_TEAM_Y, friend );
						}
						else
						{
							VWB_DrawPic ( xpos, PLAYERS_TEAM_Y, enemy );
						}
					}

					strcpy( KillStr.str, "00" );
					KillStr.length = strlen ( KillStr.str );
					DrawNumber( xpos + KILLS_OFFSET, PLAYERS_Y, 2, 4, true );

					// Get codename
					strcpy( codename, PLAYERSTATE[ player ].codename );

					// Shorten name to fit into point count
					length = strlen( codename );
					US_MeasureStr( &width, &height, codename );
					while( width > KILLS_WIDTH )
					{
						codename[ length ] = 0;
						length--;
						US_MeasureStr( &width, &height, codename );
					}

					// Draw name
					PrintX = xpos;
					PrintY = PLAYERS_NAME_Y;
					US_Print( codename );

					// Advance to next position
					xpos += KILLS_WIDTH;
				}
			}
			if ( xpos >= 320 )
			{
				break;
			}
		}
		if ( xpos >= 320 )
		{
			break;
		}
	}
}

//******************************************************************************
//
// StatusDrawPic ()
//
//******************************************************************************

void StatusDrawPic (unsigned x, unsigned y, pic_t *nums, boolean bufferofsonly)
{
	DrawMPPic (x, y, nums->width, nums->height, 0,
				 (byte *)&nums->data, bufferofsonly);
}

//******************************************************************************
//
// StatusDrawColoredPic ()
//
//******************************************************************************

void StatusDrawColoredPic (unsigned x, unsigned y, pic_t *nums, boolean bufferofsonly, int color)
{
	DrawColoredMPPic (x, y, nums->width, nums->height, 0,
				 (byte *)&nums->data, bufferofsonly, color);
}

//******************************************************************************
//
// DrawGameString ()
//
// draw string to game screen at x,y
//
//******************************************************************************

void DrawGameString (int x, int y, char * str, boolean bufferofsonly)
{
	unsigned tempbuf;

	px=x;
	py=y;

	VW_DrawPropString (str);

#if 0
	if (bufferofsonly==true)
		VW_DrawPropString (str);
	else
	{
		tempbuf=bufferofs;
		bufferofs=page1start;
		VW_DrawPropString (str);
		px=x;
		py=y;
		bufferofs=page2start;
		VW_DrawPropString (str);
		px=x;
		py=y;
		bufferofs=page3start;
		VW_DrawPropString (str);
		bufferofs=tempbuf;
	}
#endif
}


//******************************************************************************
//
// DrawNumber ()
//
// right justifies and pads with zeros
//
//******************************************************************************

void DrawNumber (int x, int y, int width, int which, boolean bufferofsonly)
{
	unsigned length,c;
	char  *str;
	byte z;

	switch (which)
	{
		case 1:
			str = ScoreStr.str;
			length = ScoreStr.length;
		break;

		case 2:
			str = LivesStr.str;
			length = LivesStr.length;
		break;

		case 3:
			str = TriadStr.str;
			length = TriadStr.length;
		break;

		case 4:
		case 5:
		case 6:
			str = KillStr.str;
			length = KillStr.length;
		break;
	}

	z = width - length;	  // Num zeros

	while (z)
	{
		switch (which)
		{
			case 1: StatusDrawPic (x, y, scorenums[0], bufferofsonly);  x+=8; break;
			case 2: StatusDrawPic (x, y, lifenums[0], bufferofsonly);	x+=8; break;
			case 3: StatusDrawPic (x, y, lifeptnums[0], bufferofsonly); x+=6; break;
			case 4: StatusDrawColoredPic (x, y, scorenums[0], bufferofsonly, playeruniformcolor);	x+=8; break;
			case 5: StatusDrawPic (x, y, lifeptnums[0], bufferofsonly); x+=6; break;
			case 6: StatusDrawPic (x, y, lifenums[0], bufferofsonly);	x+=8; break;
		}
		z--;
	}

	c = length <= width ? 0 : length-width;
	while (c < length)
	{
		switch (which)
		{
			case 1: StatusDrawPic (x, y, scorenums[str[c]-'0'], bufferofsonly);  x+=8; break;
			case 2: StatusDrawPic (x, y, lifenums[str[c]-'0'], bufferofsonly);	x+=8; break;
			case 3: StatusDrawPic (x, y, lifeptnums[str[c]-'0'], bufferofsonly); x+=6; break;
			case 4: StatusDrawColoredPic (x, y, scorenums[str[c]-'0'], bufferofsonly, playeruniformcolor);	x+=8; break;
			case 5: StatusDrawPic (x, y, lifeptnums[str[c]-'0'], bufferofsonly); x+=6; break;
			case 6: StatusDrawPic (x, y, lifenums[str[c]-'0'], bufferofsonly);	x+=8; break;
		}
		c++;
	}
}



//******************************************************************************
//
// HealPlayer ()
//
//******************************************************************************

void HealPlayer
	(
	int		points,
	objtype *ob
	)
{
	playertype *pstate;
	int maxhitpoints;

	M_LINKSTATE( ob, pstate );

	pstate->health += points;
	maxhitpoints = MaxHitpointsForCharacter( pstate );
	if ( pstate->health > maxhitpoints )
	{
		pstate->health = maxhitpoints;
	}

	if ( ( SHOW_BOTTOM_STATUS_BAR() ) && ( ob == player ) )
	{
		DrawBarHealth( false );
	}
}

//******************************************************************************
//
// DrawLives ()
//
//******************************************************************************

void DrawLives
	(
	boolean bufferofsonly
	)

{
	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if ( !EndLevelStuff )
	{
		DrawNumber( LIVES_X, LIVES_Y, 2, 2, bufferofsonly );
	}
}


//******************************************************************************
//
// GiveExtraMan ()
//
//******************************************************************************

void  GiveExtraMan (void)
{
	if (locplayerstate->lives < 99)
		locplayerstate->lives++;
	UpdateLives (locplayerstate->lives);
	DrawLives (false);
//	SD_PlaySound (BONUS1UPSND);
}



//******************************************************************************
//
// DrawScore ()
//
//******************************************************************************

void DrawScore
	(
	boolean bufferofsonly
	)
{
	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if ( !BATTLEMODE )
	{
		DrawNumber( SCORE_X, SCORE_Y, 10, 1, bufferofsonly );
	}
}


//******************************************************************************
//
// GivePoints ()
//
//******************************************************************************

void  GivePoints (long points)
{
	gamestate.score += points;

	UpdateScore (gamestate.score);

	if (!EndLevelStuff)
		DrawScore (false);
}


//******************************************************************************
//
// GiveKey ()
//
//******************************************************************************

void GiveKey (int key)
{
	locplayerstate->keys |= (1<<key);
	DrawKeys (false);
}


//******************************************************************************
//
// GiveLives ()
//
//******************************************************************************

void GiveLives (int newlives)
{
	if ((locplayerstate->lives + newlives) <= 99)
		locplayerstate->lives += newlives;
	else
		locplayerstate->lives = 99;
	UpdateLives (locplayerstate->lives);
	DrawLives (false);
}


#define EnableOldWeapon(pstate)	 \
	{										 \
	LASTSTAT->flags |= FL_ABP;		\
	LASTSTAT->flags &= ~FL_RESPAWN; \
	MakeStatActive(LASTSTAT);		 \
	pstate->weaponx = ob->tilex;	 \
	pstate->weapony = ob->tiley;	 \
	}


//******************************************************************************
//
// GiveWeapon ()
//
//******************************************************************************

void GiveWeapon
	(
	objtype *ob,
	int weapon
	)
{
	playertype *pstate;

	M_LINKSTATE( ob, pstate );

	if ( pstate->weapon == weapon )
	{
		return;
	}

	pstate->HASBULLETWEAPON[ weapon ] = 1;
	if ( ( pstate->weapon == pstate->bulletweapon ) &&
		( pstate->weapon < weapon ) )
	{
		pstate->new_weapon = weapon;
		pstate->weapondowntics = WEAPONS[ pstate->weapon ].screenheight / GMOVE;
		if ( ( ob == player ) && ( SHOW_BOTTOM_STATUS_BAR() ) )
		{
			DrawBarAmmo( false );
		}
	}

	if ( gamestate.BattleOptions.WeaponPersistence )
	{
		SpawnStatic(ob->tilex,ob->tiley,GetItemForWeapon(weapon),9);
		EnableOldWeapon(pstate);
	}

	if ( weapon > pstate->bulletweapon )
	{
		pstate->bulletweapon = weapon;
	}
}


//******************************************************************************
//
// GiveMissileWeapon ()
//
//******************************************************************************

void GiveMissileWeapon(objtype * ob, int which)
{
	playertype * pstate;


	//pstate = (ob==player)?(&playerstate):(&remoteplayerstate);
	M_LINKSTATE(ob,pstate);

	if (!gamestate.BattleOptions.WeaponPersistence)
	{
		if (pstate->ammo &&
			 (pstate->missileweapon != -1) &&
			 (!(WEAPON_IS_MAGICAL(which))) &&
			 (!(WEAPON_IS_MAGICAL(pstate->missileweapon)))
			)
		{
			int nx,ny;

			nx = ob->tilex;
			ny = ob->tiley;
			//FindEmptyTile(&nx,&ny);

			if (IsPlatform(nx,ny))
				SpawnStatic(nx,ny,GetItemForWeapon(pstate->missileweapon),9);
			else
			{
				int newz = sprites[ob->tilex][ob->tiley]->z;
				SpawnStatic(nx,ny,GetItemForWeapon(pstate->missileweapon),-1);
				LASTSTAT->z = newz;
			}
			LASTSTAT->ammo = pstate->ammo;
			EnableOldWeapon(pstate);

		}
	}
	else if (!WEAPON_IS_MAGICAL(which))
	{
		int newz = sprites[ob->tilex][ob->tiley]->z;
		SpawnStatic(ob->tilex,ob->tiley,GetItemForWeapon(which),9);
		LASTSTAT->z = newz;
		EnableOldWeapon(pstate);

	}
	pstate->new_weapon = pstate->missileweapon = which;
	pstate->weapondowntics = WEAPONS[pstate->weapon].screenheight/GMOVE;
}


//******************************************************************************
//
// DrawKeys ()
//
//******************************************************************************

void DrawKeys
	(
	boolean bufferofsonly
	)

{
	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if ( locplayerstate->keys & 1 )
	{
		GameMemToScreen( keys[ 0 ], KeyX[ 0 ], KEY_Y, bufferofsonly );
	}

	if ( locplayerstate->keys & 2 )
	{
		GameMemToScreen( keys[ 1 ], KeyX[ 1 ], KEY_Y, bufferofsonly );
	}

	if ( locplayerstate->keys & 4 )
	{
		GameMemToScreen( keys[ 2 ], KeyX[ 2 ], KEY_Y, bufferofsonly );
	}

	if ( locplayerstate->keys & 8 )
	{
		GameMemToScreen( keys[ 3 ], KeyX[ 3 ], KEY_Y, bufferofsonly );
	}
}


//******************************************************************************
//
// StatusDrawTime ()
//
//******************************************************************************

void StatusDrawTime
	(
	unsigned x,
	unsigned y,
	unsigned num,
	boolean  bufferofsonly
	)
{
	if((num>=10) || (num<0))
		return;

	DrawMPPic( x, y, timenums[ num ]->width, timenums[ num ]->height, 0,
		( byte * )&timenums[ num ]->data, bufferofsonly );
}


//******************************************************************************
//
// DrawTimeNumber ()
//
// right justifies and pads with blanks
//
//******************************************************************************

void DrawTimeNumber (
	int x, int y, int number,
	boolean seconds, boolean bufferofsonly)
{
	char  str[20];

	rt_ltoa (number,str,10);

	if (seconds)
	{
		if (number < 10)
		{
			StatusDrawTime (x,	y, 0, bufferofsonly);
			StatusDrawTime (x+8, y, str[0]-'0', bufferofsonly);
		}
		else
		{
			StatusDrawTime (x,	y, str[0]-'0', bufferofsonly);
			StatusDrawTime (x+8, y, str[1]-'0', bufferofsonly);
		}
	}
	else
	{
		if (number < 10)
			StatusDrawTime (x+8, y, str[0]-'0', bufferofsonly);
		else
		{
			StatusDrawTime (x,	y, str[0]-'0', bufferofsonly);
			StatusDrawTime (x+8, y, str[1]-'0', bufferofsonly);
		}
	}
}


//******************************************************************************
//
// DrawTimeXY ()
//
//******************************************************************************

void DrawTimeXY
	(
	int x,
	int y,
	int sec,
	boolean bufferofsonly
	)

{
	int min;
	int hour;

	while (sec > ( ( 9 * 3600 ) + 3599 ) )
	{
		sec -= ( ( 9 * 3600 ) + 3599 );
	}

	hour  = sec / 3600;
	min	= ( sec / 60 ) - ( hour * 60 );
	sec  %= 60;

	DrawTimeNumber ( x + HOUR_X, y, hour, false, bufferofsonly );
	DrawTimeNumber ( x + MIN_X, y, min, true, bufferofsonly );
	DrawTimeNumber ( x + SEC_X, y, sec, true, bufferofsonly );
}


//******************************************************************************
//
// DrawTime ()
//
//******************************************************************************

void DrawTime ( boolean bufferofsonly )
{
	int sec;

	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if (timelimitenabled == true)
	{
		sec = (timelimit-gamestate.TimeCount) / VBLCOUNTER;
	}
	else
	{
		sec = gamestate.TimeCount / VBLCOUNTER;
	}

	if ( oldsec != sec )
	{
		oldsec = sec;
		DrawTimeXY( GAMETIME_X, GAMETIME_Y, sec, bufferofsonly) ;
	}
}


//******************************************************************************
//
// DrawMPPic ()
//
// Purpose
//	 Draws a masked, planer pic at xpos, ypos.
//
// Parms
//	 xpos	- x position.
//	 ypos	- y position.
//	 width  - width of pic : should be << 2.
//	 height - height of pic.
//	 src	 - data to draw.
//
// Returns
//	 Nothing.
//
//******************************************************************************

void DrawMPPic (
	int xpos, int ypos, int width, int height,
	int heightmod, byte *src, boolean bufferofsonly)
{
//	byte *olddest;
	unsigned olddest;
//	byte *dest;
	unsigned dest;
	int x;
	int y;
	int planes;
	byte mask;
	byte pixel;

	mask = 1 << (xpos&3);

//	olddest = (byte *)(ylookup[ypos] + (xpos>>2));
	olddest = (ylookup[ypos] + (xpos>>2));

	for (planes = 0; planes < 4; planes++)
	{
		VGAMAPMASK (mask);

//		olddest = (byte *)(ylookup[ypos] + (xpos>>2));

		dest = olddest;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = *src++;

				if (pixel != 255)
				{
//					*(dest+bufferofs) = pixel;

#if 1
					if (bufferofsonly)
						VGAWRITE(dest+bufferofs, pixel);
					else
					{
						VGAWRITE(dest+page1start, pixel);
						VGAWRITE(dest+page2start, pixel);
						VGAWRITE(dest+page3start, pixel);
					}
#endif
				}

				dest++;
			}

			dest += (linewidth-width);
		}

		if (heightmod)
			src += (heightmod*width);

		mask <<= 1;
		if (mask == 16)
		{
			mask = 1;
			olddest++;
		}
	}
}


//******************************************************************************
//
// DrawColoredMPPic ()
//
// Purpose
//	 Draws a masked, planer pic at xpos, ypos.
//
// Parms
//	 xpos	- x position.
//	 ypos	- y position.
//	 width  - width of pic : should be << 2.
//	 height - height of pic.
//	 src	 - data to draw.
//
// Returns
//	 Nothing.
//
//******************************************************************************

void DrawColoredMPPic (
	int xpos, int ypos, int width, int height,
	int heightmod, byte *src, boolean bufferofsonly, int color)
{
//	byte *olddest;
	unsigned olddest;
//	byte *dest;
	unsigned dest;
	int x;
	int y;
	int planes;
	byte mask;
	byte pixel;
	byte * cmap;

	cmap=playermaps[color]+(1<<12);

	mask = 1 << (xpos&3);

//	olddest = (byte *)(ylookup[ypos] + (xpos>>2));
	olddest = (ylookup[ypos] + (xpos>>2));

	for (planes = 0; planes < 4; planes++)
	{
		VGAMAPMASK (mask);

//		olddest = (byte *)(ylookup[ypos] + (xpos>>2));
		dest = olddest;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = *src++;

				pixel = *(cmap+pixel);

				if (pixel != 255)
				{
//					*(dest+bufferofs) = pixel;

#if 1
					if (bufferofsonly)
						VGAWRITE(dest+bufferofs, pixel);
					else
					{
						VGAWRITE(dest+page1start, pixel);
						VGAWRITE(dest+page2start, pixel);
						VGAWRITE(dest+page3start, pixel);
					}
#endif
				}

				dest++;
			}

			dest += (linewidth-width);
		}

		if (heightmod)
			src += (heightmod*width);

		mask <<= 1;
		if (mask == 16)
		{
			mask = 1;
			olddest++;
		}
	}
}


//******************************************************************************
//
// UpdateScore ()
//
//******************************************************************************

void UpdateScore (int num)
{
	if (num > 999999999)
	{
		num = 999999999;
		gamestate.score = 999999999;
	}

	rt_ltoa (num, ScoreStr.str, 10);
	ScoreStr.length = strlen (ScoreStr.str);
}


//******************************************************************************
//
// UpdateLives ()
//
//******************************************************************************

void UpdateLives (int num)
{
	rt_ltoa (num, LivesStr.str, 10);
	LivesStr.length = strlen (LivesStr.str);
}

//****************************************************************************
//
// ClearTriads ()
//
//****************************************************************************
void ClearTriads (playertype * pstate)
{
	pstate->triads = 0;
	rt_ltoa (pstate->triads, TriadStr.str, 10);
	TriadStr.length = strlen (TriadStr.str);
}

//****************************************************************************
//
// UpdateTriads ()
//
//****************************************************************************

void UpdateTriads (objtype * ob, int num)
{
	playertype * pstate;

	M_LINKSTATE(ob,pstate);

	pstate->triads += num;

	if (pstate->triads >= 100)
	{
		GiveLives(1);
		if (ob==player)
		{
			AddMessage("100 Life Item Points!  Extra Life!\n",MSG_BONUS);
			SD_PlaySoundRTP (SD_GET1UPSND, player->x, player->y);
		}
		pstate->triads -= 100;
	}

	if (ob==player)
	{
		rt_ltoa (pstate->triads, TriadStr.str, 10);
		TriadStr.length = strlen (TriadStr.str);
	}
}

//****************************************************************************
//
// DrawTriads ()
//
//****************************************************************************

void DrawTriads( boolean bufferofsonly )
{
	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if ( !EndLevelStuff )
	{
		DrawNumber( TRIAD_X, TRIAD_Y, 2, 3, bufferofsonly );
	}
}


//******************************************************************************
//
// DrawPPic ()
//
//******************************************************************************

void DrawPPic (
	int xpos, int ypos, int width, int height,
	byte *src, int num, boolean up, boolean bufferofsonly)
{
//	byte *olddest;
	unsigned olddest;
//	byte *dest;
	unsigned dest;
	int x;
	int y;
	int planes;
	byte mask;
	byte pixel;
	int k;
	int amt;

	if (up)
		amt = 2;
	else
		amt = -2;

	mask = 1;

//	olddest = (byte *)(ylookup[ypos] + (xpos>>2));
	olddest = (ylookup[ypos] + (xpos>>2));

	for (planes = 0; planes < 4; planes++)
	{
		VGAMAPMASK (mask);

//		olddest = (byte *)(ylookup[ypos] + (xpos>>2));
		dest = olddest;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = *src++;

				if (pixel != 255)
				{
					for (k = 0; k < num; k++)
					{
//						*(dest+bufferofs+(amt*k)) = pixel;

#if 1
						if (bufferofsonly)
							VGAWRITE(dest+bufferofs+(amt*k), pixel);
						else
						{
							VGAWRITE(dest+page1start+(amt*k), pixel);
							VGAWRITE(dest+page2start+(amt*k), pixel);
							VGAWRITE(dest+page3start+(amt*k), pixel);
						}
#endif
					}
				}

				dest++;
			}

			dest += (linewidth-width);
		}

		mask <<= 1;
	}
}


//****************************************************************************
//
// DrawBarHealth ()
//
//****************************************************************************

void DrawBarHealth( boolean bufferofsonly )
{
	int percenthealth;
	int health_y;

	if ( !SHOW_BOTTOM_STATUS_BAR() )
	{
		return;
	}

	health_y = HEALTH_Y;
	if ( SHOW_KILLS() )
	{
		health_y -= KILLS_HEIGHT;
	}

	percenthealth = ( locplayerstate->health * 10 ) /
		MaxHitpointsForCharacter( locplayerstate );

	oldpercenthealth = percenthealth + 1;

	if ( playstate == ex_died )
	{
		DrawPPic( HEALTH_X, health_y, 8 >> 2, 16, ( byte * )&erase->data,
			10, true, bufferofsonly );

		return;
	}

	if ( locplayerstate->health <= 0 )
	{
		oldpercenthealth = 0;
	}

	if ( oldpercenthealth >= 11 )
	{
		oldpercenthealth = 10;
	}

	if ( oldpercenthealth < 4 )
	{
		DrawPPic( HEALTH_X, health_y, 8 >> 2, 16,
			( byte * )&health[ 0 ]->data, oldpercenthealth,
			true, bufferofsonly );
	}
	else if ( oldpercenthealth < 5 )
	{
		DrawPPic( HEALTH_X, health_y, 8 >> 2, 16,
			(byte *)&health[ 1 ]->data, oldpercenthealth,
			true, bufferofsonly );
	}
	else
	{
		DrawPPic( HEALTH_X, health_y, 8 >> 2, 16,
			( byte * )&health[ 2 ]->data, oldpercenthealth,
			true, bufferofsonly );
	}

	if ( oldpercenthealth < 10 )
	{
		DrawPPic( HEALTH_X + ( 8 * oldpercenthealth ), health_y,
			8 >> 2, 16, ( byte * )&erase->data, 10 - oldpercenthealth,
			true, bufferofsonly );
	}
}


//****************************************************************************
//
// DrawBarAmmo ()
//
//****************************************************************************

void DrawBarAmmo( boolean bufferofsonly )
{
	int ammo_y;

	if ( ( !SHOW_BOTTOM_STATUS_BAR() ) || ( playstate == ex_died ) )
	{
		return;
	}

	ammo_y = AMMO_Y;
	if ( SHOW_KILLS() )
	{
		ammo_y -= KILLS_HEIGHT;
	}

	DrawPPic ( AMMO_X, ammo_y + 1, 8 >> 2, 16, ( byte * )&erase->data,
		10, false, bufferofsonly );

	if ( !ARMED( player->dirchoosetime ) )
	{
		return;
	}
	if ((locplayerstate->new_weapon < wp_bazooka) ||
		 (locplayerstate->new_weapon == wp_godhand) ||
		 ( gamestate.BattleOptions.Ammo == bo_infinite_shots )
		)
	{
		DrawPPic( AMMO_X - 16, ammo_y, 24 >> 2, 16,
			( byte * )&ammo[ 0 ]->data, 1, true, bufferofsonly);

		DrawPPic( AMMO_X - 32, ammo_y + 1, 8 >> 2, 16,
			( byte * )&erase->data, 2, true, bufferofsonly );
	}
#if (SHAREWARE == 0)
	else if ( locplayerstate->new_weapon == wp_dog )
	{
		DrawPPic( AMMO_X - 16, ammo_y, 24 >> 2, 16,
			( byte * )&ammo[12]->data, 1, true, bufferofsonly );

		DrawPPic( AMMO_X - 32, ammo_y + 1, 8 >> 2, 16,
			( byte * )&erase->data, 2, true, bufferofsonly );
	}
#endif
	else
	{
		DrawPPic( AMMO_X, ammo_y + 1, 8 >> 2, 16,
			( byte * )&ammo[ locplayerstate->new_weapon]->data,
			locplayerstate->ammo, false, bufferofsonly );
	}
}


//******************************************************************************
//
// SingleDrawPPic ()
//
//******************************************************************************

void SingleDrawPPic (
	int xpos, int ypos, int width, int height,
	byte *src, int num, boolean up)
{
//	byte *olddest;
	unsigned olddest;
//	byte *dest;
	unsigned dest;
	int x;
	int y;
	int planes;
	byte mask;
	byte pixel;
	int k;
	int amt;

	if (up)
		amt = 2;
	else
		amt = -2;

	mask = 1;

//	olddest = (byte *)(bufferofs - screenofs + ylookup[ypos] + (xpos>>2));
	olddest = (bufferofs - screenofs + ylookup[ypos] + (xpos>>2));

	for (planes = 0; planes < 4; planes++)
	{
		VGAMAPMASK (mask);

		dest = olddest;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = *src++;

				if (pixel != 255)
				{
					for (k = 0; k < num; k++)
					{
//						*(dest+(amt*k)) = pixel;
						VGAWRITE(dest+(amt*k), pixel);
					}
				}

				dest++;
			}

			dest += (linewidth-width);
		}

		mask <<= 1;
	}
}



//****************************************************************************
//
// DrawStats ()
//
//****************************************************************************

void DrawStats( void )
{
	int percenthealth;
	int health_y;
	int ammo_y;

	if ( ( !SHOW_PLAYER_STATS() ) || ( playstate == ex_died ) ||
		( locplayerstate->health <= 0 ) )
	{
		return;
	}

// Uncomment this if we want transparent health only on sizes < 16
//	if ( viewsize < 16 )
//		{
//		pic_t *shape;
//
//		shape = W_CacheLumpName( "backtile", PU_CACHE );
//		DrawTiledRegion( 0, 160, 320, 24, shape );
//		}

	health_y = HEALTH_Y;
	ammo_y	= AMMO_Y;

	if ( SHOW_KILLS() )
	{
		health_y -= KILLS_HEIGHT;
		ammo_y	-= KILLS_HEIGHT;
	}

	if ( oldplayerhealth != locplayerstate->health )
	{
		oldplayerhealth = locplayerstate->health;

		percenthealth = ( locplayerstate->health * 10 ) /
			MaxHitpointsForCharacter( locplayerstate );

		oldpercenthealth = percenthealth + 1;
	}

	if ( oldpercenthealth > 10 )
	{
		oldpercenthealth = 10;
	}

	if ( oldpercenthealth < 4 )
	{
		SingleDrawPPic( HEALTH_X - 16, health_y, 8 >> 2, 16,
			( byte * )&health[ 3 ]->data, oldpercenthealth, true);
	}
	else if ( oldpercenthealth < 5 )
	{
		SingleDrawPPic( HEALTH_X - 16, health_y, 8 >> 2, 16,
			( byte * )&health[ 4 ]->data, oldpercenthealth, true );
	}
	else
	{
		SingleDrawPPic( HEALTH_X - 16, health_y, 8 >> 2, 16,
			( byte * )&health[ 5 ]->data, oldpercenthealth, true );
	}

	if ( ARMED( consoleplayer ) )
	{
		if ((locplayerstate->new_weapon < wp_bazooka) ||
			 (locplayerstate->new_weapon == wp_godhand) ||
			 (gamestate.BattleOptions.Ammo == bo_infinite_shots )
			)
		{
			SingleDrawPPic( AMMO_X - 16, ammo_y, 24 >> 2, 16,
				( byte * )&ammo[13]->data, 1, true );
		}
#if (SHAREWARE == 0)
		else if ( locplayerstate->new_weapon == wp_dog )
		{
			SingleDrawPPic( AMMO_X - 16, ammo_y + 1, 24 >> 2, 16,
				( byte * )&ammo[25]->data, 1, true );
		}
#endif
		else
		{
			SingleDrawPPic( AMMO_X, ammo_y + 1, 8 >> 2, 16,
				( byte * )&ammo[13 + locplayerstate->new_weapon]->data,
				locplayerstate->ammo, false );
		}
	}
}


//****************************************************************************
//
// DrawPauseXY ()
//
//****************************************************************************

void DrawPauseXY (int x, int y)
{
	pic_t *p;

	if (GamePaused == true)
	{
		p = (pic_t *) W_CacheLumpNum (W_GetNumForName ("paused"), PU_CACHE);
		VWB_DrawPic (x, y, p);
		DrawEpisodeLevel (x,y);
	}
	else
	{
		p = (pic_t *) W_CacheLumpNum (W_GetNumForName ("wait"), PU_CACHE);
		VWB_DrawPic (x, y, p);
	}
}

//****************************************************************************
//
// DrawPause ()
//
//****************************************************************************

void DrawPause (void)
{
	pic_t *p;
	int bufftemp = bufferofs;

	bufferofs -= screenofs;

	if (GamePaused == true)
	{
		p = (pic_t *) W_CacheLumpNum (W_GetNumForName ("paused"), PU_CACHE);
		DrawPauseXY( (320-(p->width<<2) ) >>1, (200-p->height)>>1);
	}
	else
	{
		p = (pic_t *) W_CacheLumpNum (W_GetNumForName ("wait"), PU_CACHE);
		DrawPauseXY( (320-(p->width<<2) ) >>1, (200-p->height)>>1);
	}

	bufferofs = bufftemp;
}

//****************************************************************************
//
// GM_DrawBonus ()
//
//****************************************************************************

void GM_DrawBonus( int which )
{
	int	 x;

	if ( which < stat_gasmask )
	{
		x = POWERUP1X;
		poweruptime = GetBonusTimeForItem(which);
		poweradjust = (poweruptime >> 4);
		powerupheight  = 0;
		GM_UpdateBonus(poweruptime-poweradjust - 1,true);
	}
	else
	{
		x = POWERUP2X;
		protectiontime = GetBonusTimeForItem(which);
		poweradjust = (protectiontime >> 4);
		protectionheight = 0;
		GM_UpdateBonus(protectiontime-poweradjust-1,false);
	}
}


//******************************************************************************
//
// GM_UpdateBonus ()
//
//******************************************************************************

void GM_UpdateBonus( int time, int powerup )
{
	pic_t *shape;
	int	 shapenum;

	if ( powerup )
	{
		if ( time < ( poweruptime - poweradjust ) )
		{
			powerupheight++;
			if ( !SHOW_TOP_STATUS_BAR() )
			{
				poweruptime = time;
			}
		}
	}
	else
	{
		if ( time < ( protectiontime - poweradjust ) )
		{
			protectionheight++;
			if ( !SHOW_TOP_STATUS_BAR() )
			{
				protectiontime = time;
			}
		}
	}


	if ( !SHOW_TOP_STATUS_BAR() )
	{
		return;
	}

	if ( !time )
	{
		if ( powerup == 1 )
		{
			shapenum = POWERUP1X;
		}
		else
		{
			shapenum = POWERUP2X;
		}

		GM_MemToScreen( ( byte * )&eraseb->data, eraseb->width,
			eraseb->height, shapenum, POWERUPY );

		return;
	}

	if ( powerup )
	{
		if ( time < ( poweruptime - poweradjust ) )
		{
			if ( player->flags & FL_GODMODE )
			{
				shapenum = powerpics;
			}
			else if ( player->flags & FL_DOGMODE )
			{
				shapenum = powerpics + 1;
			}
			else if ( player->flags & FL_FLEET )
			{
				shapenum = powerpics + 2;
			}
			else if ( player->flags & FL_ELASTO )
			{
				shapenum = powerpics + 3;
			}
			else if ( player->flags & FL_SHROOMS )
			{
				shapenum = powerpics + 4;
			}
			else
			{
				GM_MemToScreen( ( byte * )&eraseb->data,
					eraseb->width, eraseb->height, POWERUP1X, POWERUPY );
				return;
			}

			poweruptime = time;

			shape = ( pic_t * )W_CacheLumpNum( shapenum, PU_CACHE );

			GM_MemToScreen( ( byte * )&eraseb->data, eraseb->width,
				eraseb->height, POWERUP1X, POWERUPY );

			DrawMPPic( POWERUP1X, POWERUPY + powerupheight, shape->width,
				shape->height - powerupheight, powerupheight,
				( byte * )&shape->data, false );
		}
	}
	else
	{
		if ( time < ( protectiontime - poweradjust ) )
		{
			if ( player->flags & FL_BPV )
			{
				shapenum = powerpics + 6;
			}
			else if ( player->flags & FL_GASMASK )
			{
				shapenum = powerpics + 5;
			}
			else if ( player->flags & FL_AV )
			{
				shapenum = powerpics + 7;
			}

			protectiontime = time;

			shape = ( pic_t * )W_CacheLumpNum( shapenum, PU_CACHE );

			GM_MemToScreen( ( byte * )&eraseb->data, eraseb->width,
				eraseb->height, POWERUP2X, POWERUPY );

			DrawMPPic( POWERUP2X, POWERUPY + protectionheight,
				shape->width, shape->height - protectionheight,
				protectionheight, ( byte * )&shape->data, false );
		}
	}
}


//******************************************************************************
//
// Drawpic ()
//
// Purpose
//	 Draws a masked, planer pic at xpos, ypos.
//
// Parms
//	 xpos	- x position.
//	 ypos	- y position.
//	 width  - width of pic : should be << 2.
//	 height - height of pic.
//	 src	 - data to draw.
//
// Returns
//	 Nothing.
//
//******************************************************************************

void Drawpic (int xpos, int ypos, int width, int height, byte *src)
{
//	byte *olddest;
	unsigned olddest;
//	byte *dest;
	unsigned dest;
	int x;
	int y;
	int planes;
	byte mask;
	byte pixel;

	mask = 1 << (xpos&3);

//	olddest = (byte *)(bufferofs + ylookup[ypos] + (xpos>>2));
	olddest = (bufferofs + ylookup[ypos] + (xpos>>2));

	for (planes = 0; planes < 4; planes++)
	{
		VGAMAPMASK (mask);

		dest = olddest;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				pixel = *src++;

				if (pixel != 255)
//					*(dest) = pixel;
					VGAWRITE(dest, pixel);

				dest++;
			}

			dest += (linewidth-width);
		}

		mask <<= 1;
		if (mask == 16)
		{
			mask = 1;
			olddest++;
		}
	}
}


//******************************************************************************
//
// DrawEpisodeLevel ()
//
// right justifies and pads with blanks
//
//******************************************************************************

void  DrawEpisodeLevel (int x, int y)
{
	int level;
	char  str[20];
	pic_t *p;

	if (!BATTLEMODE)
	{
		rt_ltoa (gamestate.episode, str, 10);

		Drawpic (x+29, y+16, 8>>2, 16, (byte *)&timenums[str[0]-'0']->data);

		if ((gamestate.mapon == 6) || (gamestate.mapon == 14) ||
			 (gamestate.mapon == 22) || (gamestate.mapon == 32) ||
			 (gamestate.mapon == 33))
		{
			p = (pic_t *) W_CacheLumpName ("tnumb", PU_CACHE);
			Drawpic (x+40, y+16, 8>>2, 16, (byte *)&p->data);

			if (gamestate.mapon == 6)
				level = 1;
			else
				if (gamestate.mapon == 14)
					level = 2;
				else
					if (gamestate.mapon == 22)
						level = 3;
					else
						if (gamestate.mapon == 32)
							level = 4;
						else
							level = 5;
		}
		else
			level = GetLevel (gamestate.episode, gamestate.mapon);

		level = abs(level);
		rt_ltoa (level, str, 10);

		if (level < 10)
			Drawpic (x+49, y+16, 8>>2, 16, (byte *)&timenums[str[0]-'0']->data);
		else
		{
			Drawpic (x+49, y+16, 8>>2, 16, (byte *)&timenums[str[0]-'0']->data);
			Drawpic (x+57, y+16, 8>>2, 16, (byte *)&timenums[str[1]-'0']->data);
		}
	}
	else
	{
		p = (pic_t *) W_CacheLumpName ("battp", PU_CACHE);
		Drawpic (x+16, y+15, 32>>2, 16, (byte *)&p->data);

		level = abs(gamestate.mapon + 1);
		rt_ltoa (level, str, 10);

		if (level < 10)
			Drawpic (x+49, y+16, 8>>2, 16, (byte *)&timenums[str[0]-'0']->data);
		else
		{
			Drawpic (x+49, y+16, 8>>2, 16, (byte *)&timenums[str[0]-'0']->data);
			Drawpic (x+57, y+16, 8>>2, 16, (byte *)&timenums[str[1]-'0']->data);
		}
	}
}


//******************************************************************************
//
// GM_MemToScreen ()
//
//******************************************************************************

void GM_MemToScreen (byte *source, int width, int height, int x, int y)
{
#if 1
	unsigned dest, dest1, dest2, dest3, mask;
	unsigned screen1, screen2, screen3;
	int  plane;

	dest = (ylookup[y]+(x>>2));
	mask = 1 << (x&3);

#if 1
	dest1 = (dest+page1start);
	dest2 = (dest+page2start);
	dest3 = (dest+page3start);
#endif

	for (plane = 0; plane<4; plane++)
	{
		VGAMAPMASK (mask);

		screen1 = dest1;
		screen2 = dest2;
		screen3 = dest3;
		for (y = 0; y < height; y++,
			screen1 += linewidth,
			screen2 += linewidth,
			screen3 += linewidth,
			source += width)
		{
			VGAWRITEBUF (screen1, source, width);
			VGAWRITEBUF (screen2, source, width);
			VGAWRITEBUF (screen3, source, width);
		}

		mask <<= 1;

		if (mask == 16)
		{
			mask = 1;
			dest1++;
			dest2++;
			dest3++;
		}
	}
#endif

//	__debugbreak();
}


//==========================================================================

/*
==================
=
= ScreenShake
=
==================
*/

void ScreenShake (void)
{
	static int which = 0;

	if (SHAKETICS != 0xFFFF)
	{
		SHAKETICS -= tics;

		which = (RandomNumber ("ScreenShake",0) & 3);

		switch (which)
		{
			case 0:
				displayofs += 1;
			break;

			case 1:
				displayofs -= 1;
			break;

			case 2:
				displayofs += 3*SCREENBWIDE;
			break;

			case 3:
				displayofs -= 3*SCREENBWIDE;
			break;
		}
	}
}

//******************************************************************************
//
// DoBorderShifts ()
//
//******************************************************************************

void DoBorderShifts (void)
{
	if (damagecount)
	{
		if (damagecount > 100)
			damagecount = 100;

		damagecount -= 6;

		if (damagecount < 0)
			damagecount = 0;

		SetBorderColor (*(colormap+(((100-damagecount)>>2)<<8)+48));

		borderset = true;
	}
	else
		if (borderset)
		{
			SetBorderColor (0);
			borderset = false;
		}
}


//******************************************************************************
//
// DrawHighScores ()
//
//******************************************************************************

void DrawHighScores (void)
{
	char buffer[16];
#if (SHAREWARE == 0)
	char buffer1[5];
#endif
	int  i,
		  w,
		  h;
	HighScore *s;

	w=0; h=0;

	for (i = 0, s = Scores; i < MaxScores; i++, s++)
	{
		PrintY = 25 + (16 * i);

		//
		// name
		//
		PrintX = 3*8;
		DrawMenuBufPropString (PrintX, PrintY, s->name);

		//
		// level
		//
		rt_ultoa (s->completed, buffer, 10);

		PrintX = (17 * 8)-10;
#if (SHAREWARE == 0)
		rt_itoa (s->episode, buffer1, 10);

		DrawMenuBufPropString (PrintX, PrintY, buffer1);
#else
		DrawMenuBufPropString (PrintX, PrintY, "S");
#endif

		DrawMenuBufPropString (PrintX, PrintY, "-");

#if (SHAREWARE == 0)
		if (s->completed == 7)
			DrawMenuBufPropString (PrintX, PrintY, "B");
		else if (s->completed == 8)
			DrawMenuBufPropString (PrintX, PrintY, "S");
		else if (s->completed == 9)
			DrawMenuBufPropString (PrintX, PrintY, "C");
		else if (s->completed == 10)
			DrawMenuBufPropString (PrintX, PrintY, "D");
		else
			DrawMenuBufPropString (PrintX, PrintY, buffer);
#else
		DrawMenuBufPropString (PrintX, PrintY, buffer);
#endif

		//
		// score
		//
		rt_ultoa(s->score,buffer,10);

		VW_MeasurePropString (buffer, &w, &h);
		PrintX = (33 * 8) - w;
		DrawMenuBufPropString (PrintX, PrintY, buffer);
	}
}



//******************************************************************************
//
// CheckHighScore ()
//
//******************************************************************************

void CheckHighScore (long score, word other, boolean INMENU)
{
	word		  i,j;
	int			n;
	HighScore	myscore;
	int			level;

	MenuFadeIn();
	if (!INMENU)
		SetupMenuBuf ();

	strcpy (myscore.name,"");
	myscore.score	  = score;

	level = GetLevel (gamestate.episode, other-1);

	myscore.episode	= gamestate.episode;
	myscore.completed = level;

	CurrentFont = smallfont;

	for (i = 0, n = -1; i < MaxScores; i++)
	{
		if ((myscore.score > Scores[i].score)  ||
			((myscore.score == Scores[i].score) &&
			 (myscore.completed > Scores[i].completed)))
		{
			for (j = MaxScores; --j > i;)
				Scores[j] = Scores[j - 1];
			Scores[i] = myscore;
			n = i;
			break;
		}
	}

	if (INMENU)
	{
		SetAlternateMenuBuf();
		SetMenuTitle ("High Scores");
		ClearMenuBuf();
		DrawHighScores ();
		if (n != -1)
			DisplayInfo (6);
		else
			DisplayInfo (5);
		FlipMenuBuf ();
	}
	else
	{
		ClearMenuBuf ();
		SetMenuTitle ("High Scores");
		DrawHighScores ();
		if (n != -1)
			DisplayInfo (6);
		else
			DisplayInfo (5);
		RefreshMenuBuf (0);
	}

	if (n != -1)
	{
		PrintY = 25 + (16 * n);
		PrintX = 3*8;
		US_LineInput (PrintX, PrintY, Scores[n].name, NULL,
						  true, 10, 98, 0);
	}
	else
	{
		IN_ClearKeysDown ();
		if ( INMENU )
		{
			while( !IN_CheckAck () )
			{
				RefreshMenuBuf (0);
			}
		}
		else
		{
			for( i = 0; i <= 150; i += tics )
			{
				RefreshMenuBuf (0);
				if (IN_CheckAck ())
				{
					break;
				}
			}
		}
	}

	if (INMENU)
	{
		SD_Play (SD_ESCPRESSEDSND);
	}
	else
	{
		ShutdownMenuBuf ();
	}
}


//===========================================================================

//#define HEADERX		140
//#define BONERNAMEX	170
#define HEADERX		152
#define BONERNAMEX	166


/*
==================
=
= DrawEOLHeader ()
=
==================
*/
void DrawEOLHeader( int playstate )
{
	int  health;
	char tempstr[ 15 ];
	char *string;
	int  level;
	int  w;
	int  h;

	VWB_TBar( 30, 5, 250, 75 );

	switch( playstate )
	{
		case ex_skiplevel :
			if ( ( gamestate.violence >= vl_high ) &&
				( gamestate.difficulty >= gd_hard ) )
			{
				string = "LEVEL WUSSED OUT ON!";
			}
			else
			{
				string = "LEVEL SKIPPED.";
			}
			break;

		case ex_secretdone :
			string = "SECRET LEVEL COMPLETED!";
			break;

		case ex_secretlevel :
			string = "SECRET EXIT TAKEN!";
			break;

		case ex_gameover :
			string = "GAME COMPLETED!";
			break;

		case ex_bossdied :
			string = "BOSS DEFEATED!";
			break;

		default :
			string = "LEVEL COMPLETED!";
			break;
	}

	w=0; h=0;
	VW_MeasurePropString( string, &w, &h );

	px = ( 320 - w ) / 2;
	py = 10;
	VWB_DrawPropString( string );

	// draw episode number
	string = "EPISODE";
	VW_MeasurePropString( string, &w, &h );
	px = HEADERX - w;
	py = 25;
	VWB_DrawPropString( string );

	rt_itoa( gamestate.episode, tempstr, 10 );
	px = BONERNAMEX;
	VWB_DrawPropString( tempstr );

	// draw area number
	level = GetLevel( gamestate.episode, gamestate.mapon );
	rt_itoa( level, tempstr, 10 );

	py = 35;

	if ( playstate == ex_secretdone )
	{
		string = "SECRET AREA";
	}
	else if ( playstate == ex_bossdied )
	{
		string = "BOSS AREA";
	}
	else if ( gamestate.mapon == 32 )
	{
		string = "CHASE AREA";
	}
	else
	{
		string = "AREA";
	}

	VW_MeasurePropString( string, &w, &h);
	px = HEADERX - w;
	VWB_DrawPropString( string );

	if ( gamestate.mapon != 33 )
	{
		px = BONERNAMEX;
		VWB_DrawPropString( tempstr );
	}

	string = "SCORE";
	VW_MeasurePropString( string, &w, &h);
	px = HEADERX - w;
	py = 45;
	VWB_DrawPropString( string );

	px = BONERNAMEX;
	rt_itoa( gamestate.score, tempstr, 10 );
	VWB_DrawPropString( tempstr );

	string = "HEALTH";
	VW_MeasurePropString( string, &w, &h );
	px = HEADERX - w;
	py = 55;
	VWB_DrawPropString( string );

	px = BONERNAMEX;
	health = ( ( locplayerstate->health * 100 ) /
		MaxHitpointsForCharacter( locplayerstate ) );

	rt_itoa( health, tempstr, 10 );
	VWB_DrawPropString( tempstr );
	VWB_DrawPropString( "%" );

	//
	// Secret count
	//

	{
		char str1[10];
		char str2[10];

		rt_itoa(gamestate.secretcount,&(str1[0]),10);
		strcat(str1," / ");
		rt_itoa(gamestate.secrettotal,&(str2[0]),10);
		strcat(str1,str2);
		string = "SECRET WALLS";
		VW_MeasurePropString( string, &w, &h );
		px = HEADERX - w;
		py = 65;
		VWB_DrawPropString( string );

		px = BONERNAMEX;
		VWB_DrawPropString( str1 );
	}

	VW_UpdateScreen ();
}

boolean EndBonusFirst;
boolean EndBonusSkip;
int	  EndBonusNumBonuses;
int	  EndBonusVoice;
int	  EndBonusStartY;

void DrawEndBonus
	(
	char *string,
	char *bonusstring,
	int	type
	)

{
	int w;
	int h;
	int  health;
	char tempstr[ 15 ];

	if ( EndBonusFirst )
	{
		VWB_TBar( 5, EndBonusStartY - 2, 310, 4 );
		EndBonusFirst = false;
	}

	w=0; h=0;

	VWB_TBar( 5, EndBonusStartY + 2, 310, 10 );
	VW_MeasurePropString( string, &w, &h );

	py = EndBonusStartY;
	if ( bonusstring == NULL )
	{
		px = ( 320 - w ) / 2;
		VWB_DrawPropString( string );
	}
	else
	{
		px = BONERNAMEX - w;
		VWB_DrawPropString( string );

		EndBonusNumBonuses++;
		VW_MeasurePropString( bonusstring, &w, &h );
		px = 310 - w;
		py = EndBonusStartY;
		VWB_DrawPropString( bonusstring );
	}

	// Update Score
	py = 45;
	px = BONERNAMEX;
	V_ReDrawBkgnd( px, py, 107, 11, false );
	VWB_TBar( px, py, 107, 11 );
	rt_itoa( gamestate.score, tempstr, 10 );
	VWB_DrawPropString( tempstr );

	// Update Health
	py = 55;
	px = BONERNAMEX;
	V_ReDrawBkgnd( px, py, 107, 11, false );
	VWB_TBar( px, py, 107, 11 );
	health = ( ( locplayerstate->health * 100 ) /
		MaxHitpointsForCharacter( locplayerstate ) );
	rt_itoa( health, tempstr, 10 );
	VWB_DrawPropString( tempstr );
	VWB_DrawPropString( "%" );

	switch( type )
	{
		case 0 :
			EndBonusVoice = SD_Play( SD_ENDBONUS1SND );
			break;

		case 1 :
			EndBonusVoice = SD_Play( SD_NOBONUSSND );
			break;

		case 2 :
			VL_FillPalette(255,255,255);
			VW_UpdateScreen();
			VL_FadeIn(0,255,origpal,10);
			EndBonusVoice = SD_Play( SD_LIGHTNINGSND );
			break;
	}

	EndBonusStartY += 10;

	VW_UpdateScreen();
	while( SD_SoundActive( EndBonusVoice ) && !EndBonusSkip )
	{
		VW_UpdateScreen();
		if ( IN_CheckAck() )
		{
//			__debugbreak();
			EndBonusSkip = true;
		}
	}
}



/*
==================
=
= LevelCompleted
=
= Exit with the screen faded out
=
==================
*/

extern int OLDLMWEAPON;
extern int OLDLWEAPON;

void LevelCompleted( exit_t playstate )
{
	objtype *obj;
	boolean dobonus;
	int i;
	int kr;
	int sr;
	int tr;
	int missileratio;
	int superratio;
	int healthratio;
	int democraticratio;
	int plantratio;
	int cnt;

	EndBonusNumBonuses	= 0;
	EndBonusFirst		= true;
	EndBonusSkip		= false;
	EndBonusStartY		= 90;

	IN_StartAck();
	EndBonusVoice = 0;
	if ( playstate != ex_bossdied )
	{
		EndBonusVoice = SD_Play( SD_LEVELDONESND );
		VL_FillPalette( 255, 255, 255 );
		VL_FadeIn( 0, 255, origpal, 10 );
		if ( player->flags & FL_DOGMODE )
		{
			MU_StartSong( song_dogend );
		}
		else
		{
			MU_StartSong( song_endlevel );
		}
	}

	BkPic = ( pic_t * )W_CacheLumpName( "mmbk", PU_CACHE );
	VWB_DrawPic( 0, 0, BkPic );

	CheckHolidays();
	CurrentFont = smallfont;

	// Kill powerups
	if ( player->flags & FL_ELASTO )
	{
		player->flags &= ~FL_NOFRICTION;
	}

	player->flags &= ~( FL_FLEET | FL_SHROOMS | FL_ELASTO | FL_GODMODE |
		FL_DOGMODE | FL_BPV | FL_AV | FL_GASMASK );

	// Turn off quickload for next level
	pickquick = false;

	//
	// FIGURE RATIOS OUT BEFOREHAND
	//
	kr = 0;
	tr = 0;
	tr = 0;
	superratio		= 0;
	missileratio	 = 0;
	healthratio	  = 0;
	democraticratio = 0;
	plantratio		= 0;

	if ( gamestate.killtotal )
	{
		kr = ( int )( ( ( int )gamestate.killcount ) * 100 ) /
			( ( int )gamestate.killtotal );
	}

	if ( gamestate.secrettotal )
	{
		sr = ( int )( ( ( int )gamestate.secretcount ) * 100 ) /
			( ( int )gamestate.secrettotal );
	}

	if ( gamestate.treasuretotal )
	{
		tr = ( int )( ( ( int )gamestate.treasurecount ) * 100 ) /
			( ( int )gamestate.treasuretotal );
	}

	if ( gamestate.supertotal )
	{
		superratio = ( int )( ( ( int )gamestate.supercount ) * 100 ) /
			( ( int )gamestate.supertotal );
	}

	if ( gamestate.missiletotal )
	{
		missileratio = ( int )( ( ( int )gamestate.missilecount ) * 100 ) /
			( ( int )gamestate.missiletotal );
	}

	if ( gamestate.healthtotal )
	{
		healthratio = ( int )( ( ( int )gamestate.healthcount ) * 100 ) /
			( ( int )gamestate.healthtotal );
	}

	if ( gamestate.democratictotal )
	{
		democraticratio = ( int )( ( ( int )gamestate.democraticcount ) *
			100 ) / ( ( int )gamestate.democratictotal );
	}

	if ( gamestate.planttotal )
	{
		plantratio = ( int )( ( ( int )gamestate.plantcount ) * 100 ) /
			( ( int )gamestate.planttotal );
	}

	DrawEOLHeader( playstate );

	while( SD_SoundActive( EndBonusVoice ) && !EndBonusSkip )
	{
		VW_UpdateScreen();
		if ( IN_CheckAck() )
		{
//			__debugbreak();
			EndBonusSkip = true;
		}
	}

	if ( GetNextMap(player->tilex,player->tiley) == -1)
	{
		if ( gamestate.dipballs == 3 )
		{
			gamestate.score += 100000;
			DrawEndBonus( "DIP BONUS", "100000 POINTS", 0 );
			EndBonusStartY += 10;
		}

		if ( locplayerstate->lives > 0 )
		{
			char str[20];
			char str2[60];

			DrawEndBonus( "EXTRA LIVES BONUS", "\0", 0 );
			rt_itoa(locplayerstate->lives,str,10);
			strcpy(str2,str);
			strcat(str2," EXTRA LIVES =");
			DrawEndBonus( "\0", str2, 0 );
			rt_itoa(locplayerstate->lives,str,10);
			strcpy(str2,str);
			strcat(str2," X 10000 = ");
			rt_itoa(locplayerstate->lives*10000,str,10);
			strcat(str2,str);
			strcat(str2," POINTS");
			gamestate.score += 10000*locplayerstate->lives;
			DrawEndBonus( "\0", str2, 0 );
		}
	}
	else
	{
		//
		// Check for SKIN OF YO TEETH
		//
		if ( locplayerstate->health <= 10 )
		{
			locplayerstate->health = MaxHitpointsForCharacter( locplayerstate );
			DrawEndBonus( "SKIN OF YOUR TEETH", "100% HEALTH", 0 );
		}

		// BULL IN CHINA SHOP BONUS
		if ( tr == 100 )
		{
			gamestate.score += 10000;
			DrawEndBonus( "BULL IN CHINA SHOP", "10000 POINTS", 0 );
		}

		// SUPERCHARE BONUS
		if ( superratio == 100 )
		{
			gamestate.score += 10000;
			DrawEndBonus( "SUPERCHARGE BONUS", "10000 POINTS", 0 );
		}

		// BLEEDER BONUS
		if ( healthratio == 100 )
		{
			gamestate.score += 10000;
			DrawEndBonus( "BLEEDER BONUS", "10000 POINTS", 0 );
		}

		// ADRENALINE BONUS
		if ( kr == 100 )
		{
			gamestate.score += 10000;
			DrawEndBonus( "ADRENALINE BONUS", "10000 POINTS", 0 );
		}

		// CURIOSITY BONUS
		dobonus = true;

		//
		// Check switches
		cnt = lastswitch - &switches[ 0 ];
		if ( cnt != 0 )
		{
			for ( i = 0; i < cnt; i++ )
			{
				if ( ( switches[ i ].flags & FL_S_FLIPPED ) == 0 )
				{
					dobonus = false;
					break;
				}
			}
		}

		//
		// Check pillars
		for ( obj = FIRSTACTOR; obj != NULL; obj = obj->next )
		{
			if ( ( obj->obclass == pillarobj ) &&
				( ( obj->flags & FL_FLIPPED ) == 0 ) )
			{
				dobonus = false;
			}
		}

		if ( ( gamestate.secrettotal ) && ( sr != 100 ) )
		{
			dobonus = false;
		}

		if ( dobonus )
		{
			gamestate.score += 10000;
			DrawEndBonus( "CURIOSITY BONUS", "10000 POINTS", 0 );
		}

		// GROUND ZERO BONUS
		if ( gamestate.DOGROUNDZEROBONUS )
		{
			gamestate.score += 10000;
			DrawEndBonus( "GROUND ZERO BONUS", "10000 POINTS", 0 );
		}

		// REPUBLICAN BONUS 1
		if ( missileratio == 100 )
		{
			gamestate.score += 5000;
			DrawEndBonus( "REPUBLICAN BONUS 1", " 5000 POINTS", 0 );
		}

		// REPUBLICAN BONUS 2
		if (plantratio == 100)
		{
			gamestate.score += 5000;
			DrawEndBonus( "REPUBLICAN BONUS 2", " 5000 POINTS", 0 );
		}

		// DEMOCRATIC BONUS 1
		if ( gamestate.DODEMOCRATICBONUS1 )
		{
			gamestate.score += 5000;
			DrawEndBonus( "DEMOCRATIC BONUS 1", " 5000 POINTS", 0 );
		}

		// DEMOCRATIC BONUS 2
		if (democraticratio == 100)
		{
			gamestate.score += 5000;
			DrawEndBonus( "DEMOCRATIC BONUS 2", " 5000 POINTS", 0 );
		}
	}

	if ( EndBonusNumBonuses == 0 )
	{
		DrawEndBonus( "NO BONUS!", NULL, 1 );
	}

	if ( ( EndBonusNumBonuses != 0 ) || ( playstate == ex_gameover ) )
	{
		SD_Play( PlayerSnds[ locplayerstate->player ] );

		// DO BONUS BONUS
		if ( EndBonusNumBonuses == NUMBONUSES )
		{
			IN_StartAck();
			while( !IN_CheckAck() )
			{
				VW_UpdateScreen();
			}

			BkPic = ( pic_t * )W_CacheLumpName( "mmbk", PU_CACHE );
			VWB_DrawPic( 0, 0, BkPic );

			gamestate.score += BONUSBONUS;
			DrawEOLHeader( playstate );
			EndBonusFirst = true;
			EndBonusStartY = 110;
			EndBonusSkip = true;
			DrawEndBonus( "BONUS BONUS!  1,000,000 POINTS!", NULL, 2 );
			}
		else if ( ( kr == 100 ) && ( dobonus ) )
		{
			IN_StartAck();
			while( !IN_CheckAck() )
			{
				VW_UpdateScreen();
			}

			BkPic = ( pic_t * )W_CacheLumpName( "mmbk", PU_CACHE );
			VWB_DrawPic( 0, 0, BkPic );

			DrawEOLHeader( playstate );
			EndBonusFirst = true;
			EndBonusStartY = 110;
			DrawEndBonus( "You have done well.", NULL, 3 );
#if (SHAREWARE==1)
			EndBonusVoice = SD_Play( SD_RICOCHET3SND );
#else
			EndBonusVoice = SD_Play( SD_PERCENT100SND );
#endif
			EndBonusSkip = false;
			DrawEndBonus( "This level is toast.", NULL, 3 );
		}
	}

	IN_StartAck();
	while( !IN_CheckAck() )
	{
		VW_UpdateScreen();
	}

	EndLevelStuff = false;
	CurrentFont = smallfont;
}


void DrawTallyHeader( int which )
{
	pic_t *Name;
	pic_t *KillCount;
	pic_t *TimesYouKilledPerson;
	pic_t *TimesPersonKilledYou;
	pic_t *Suicides;
	pic_t *Score;
	pic_t *Blank;
	pic_t *TopBar;

	Name		  = ( pic_t * )W_CacheLumpName( "t_name",	 PU_CACHE );
	Blank		 = ( pic_t * )W_CacheLumpName( "t_blnk",	 PU_CACHE );
	KillCount	= ( pic_t * )W_CacheLumpName( "t_kcount",  PU_CACHE );
	TimesYouKilledPerson = ( pic_t * )W_CacheLumpName( "t_kilper",  PU_CACHE );
	TimesPersonKilledYou = ( pic_t * )W_CacheLumpName( "t_perkil" , PU_CACHE );
	Suicides	 = ( pic_t * )W_CacheLumpName( "t_suicid",  PU_CACHE );
	Score		 = ( pic_t * )W_CacheLumpName( "t_score",	PU_CACHE );
	TopBar		= ( pic_t * )W_CacheLumpName( "t_bar",	  PU_CACHE );

	IFont = ( cfont_t * )W_CacheLumpName( "sifont", PU_CACHE );

	switch( which )
		{
		case 0 :
			VWB_DrawPic (	8,  8, TopBar );
			DrawIntensityString( 12, 11, "FINAL SCORE", 20 );
			VWB_DrawPic (	8, 24, Name );
			VWB_DrawPic ( 136, 24, KillCount );
			VWB_DrawPic ( 184, 24, Suicides );
			VWB_DrawPic ( 272, 24, Score );
			break;

		case 1 :
			VWB_DrawPic (	8,  8, TopBar );
			DrawIntensityString( 12, 11, "FINAL SCORE", 20 );
			VWB_DrawPic (	8, 24, Name );
			VWB_DrawPic ( 136, 24, Blank );
			VWB_DrawPic ( 272, 24, Score );
			break;

		case 2 :
			VWB_DrawPic (	8,  8, TopBar );
			DrawIntensityString( 12, 11, "YOUR KILLS", 20 );
			VWB_DrawPic (	8, 24, Name );
			VWB_DrawPic ( 136, 24, KillCount );
			VWB_DrawPic ( 184, 24, TimesYouKilledPerson );
			break;

		case 3 :
			VWB_DrawPic (	8,  8, TopBar );
			DrawIntensityString( 12, 11, "YOUR DEATHS", 20 );
			VWB_DrawPic (	8, 24, Name );
			VWB_DrawPic ( 136, 24, TimesPersonKilledYou );
			break;
		}

	DrawTimeXY( TALLYTIME_X, TALLYTIME_Y, gamestate.TimeCount / VBLCOUNTER,
		true );
	}


#define BT_RANK_X	 23
#define BT_PLAYER_X  30
#define BT_KILLS_X	( 139 + ( ( 40 + 20 ) / 2 ) )
#define BT_DEATHS_X  ( 193 + ( ( 56 + 20 ) / 2 ) )
//#define BT_SCORE_X	( 263 + ( ( 46 + 20 ) / 2 ) )
#define BT_SCORE_X	( 273 + ( ( 46 + 20 ) / 2 ) )


void ShowKills( int localplayer )
{
	int  w;
	int  h;
	int  i;
	int  j;
	int  temp;
	int  rank;
	int  player;
	int  killer;
	int  victim;
	int  color;
	char tempstr[15];
	int  KillCount[ MAXPLAYERS ];
	int  Order[ MAXPLAYERS ];
	int  NumPlayers;

	// show at the most 11 players
	NumPlayers = min( numplayers, 11 );

	// Count kills
	for( killer = 0; killer < NumPlayers; killer++ )
		{
		Order[ killer ] = killer;
		KillCount[ killer ] = 0;
		for( victim = 0; victim < NumPlayers; victim++ )
			{
			if ( BATTLE_Team[ victim ] != BATTLE_Team[ killer ] )
				{
				KillCount[ killer ] += WhoKilledWho[ killer ][ victim ];
				}
			}
		}

	for( i = 0; i < NumPlayers - 1; i++ )
		{
		for( j = i + 1; j < NumPlayers; j++ )
			{
			if ( KillCount[ Order[ i ] ] < KillCount[ Order[ j ] ] )
				{
				temp = Order[ i ];
				Order[ i ] = Order[ j ];
				Order[ j ] = temp;
				}
			}
		}

	DrawTallyHeader( 2 );

	IFont = (cfont_t * )W_CacheLumpNum (W_GetNumForName ("sifont"), PU_CACHE);
	CurrentFont = smallfont;
	py = 43;

	for( rank = 0; rank < NumPlayers; rank++ )
		{
		player = Order[ rank ];

		color = 21;

		// Highlight the your score
		if ( player == localplayer )
			{
			// Change to Intensity
			color = 241;
			}

		// Draw rank if not tied with previous rank
		if ( ( rank == 0 ) || ( KillCount[ player ] !=
			KillCount[ Order[ rank - 1 ] ] ) )
			{
			rt_itoa( rank + 1, tempstr, 10 );
			}
		else
			{
			strcpy( tempstr, "Tie" );
			}

		w=0; h=0;

		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_RANK_X - w, py, tempstr, color );

		// Draw name
		DrawIntensityString( BT_PLAYER_X, py, PLAYERSTATE[ player ].codename, color );

		// Draw kills
		rt_itoa( KillCount[ player ], tempstr, 10 );
		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_KILLS_X - w, py, tempstr, color );

		// Draw times you killed that person
		if ( player != localplayer )
			{
			rt_itoa( WhoKilledWho[ localplayer ][ player ], tempstr, 10 );
			}
		else
			{
			strcpy( tempstr, "-" );
			}

		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_DEATHS_X - w, py, tempstr, color );

		if ( gamestate.teamplay )
			{
			DrawIntensityString( BT_DEATHS_X + 16, py,
				colorname[ PLAYERSTATE[ player ].uniformcolor ], color );
			}

		py += h;
		}
	}


void ShowDeaths( int localplayer )
	{
	int  w;
	int  h;
	int  i;
	int  j;
	int  temp;
	int  rank;
	int  player;
	int  killer;
	int  victim;
	int  color;
	char tempstr[15];
	int  DeathCount[ MAXPLAYERS ];
	int  Order[ MAXPLAYERS ];
	int  NumPlayers;

	w=0; h=0;

	// show at the most 11 players
	NumPlayers = min( numplayers, 11 );

	// Count Deaths
	for( victim = 0; victim < NumPlayers; victim++ )
		{
		Order[ victim ] = victim;
		DeathCount[ victim ] = 0;
		for( killer = 0; killer < NumPlayers; killer++ )
			{
			DeathCount[ victim ] += WhoKilledWho[ killer ][ victim ];
			}
		}

	for( i = 0; i < NumPlayers - 1; i++ )
		{
		for( j = i + 1; j < NumPlayers; j++ )
			{
			if ( DeathCount[ Order[ i ] ] < DeathCount[ Order[ j ] ] )
				{
				temp = Order[ i ];
				Order[ i ] = Order[ j ];
				Order[ j ] = temp;
				}
			}
		}

	DrawTallyHeader( 3 );

	IFont = (cfont_t * )W_CacheLumpNum (W_GetNumForName ("sifont"), PU_CACHE);
	CurrentFont = smallfont;
	py = 43;

	for( rank = 0; rank < NumPlayers; rank++ )
		{
		player = Order[ rank ];
		color = 21;

		// Highlight the your score
		if ( player == localplayer )
			{
			// Change to Intensity
			color = 241;
			}

		// Draw rank if not tied with previous rank
		if ( ( rank == 0 ) || ( DeathCount[ player ] !=
			DeathCount[ Order[ rank - 1 ] ] ) )
			{
			rt_itoa( rank + 1, tempstr, 10 );
			}
		else
			{
			strcpy( tempstr, "Tie" );
			}

		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_RANK_X - w, py, tempstr, color );

		// Draw name
		DrawIntensityString( BT_PLAYER_X, py, PLAYERSTATE[ player ].codename, color );

		// Draw deaths
		rt_itoa( DeathCount[ player ], tempstr, 10 );
		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_KILLS_X - w, py, tempstr, color );

		// Draw times you were killed by that person
		rt_itoa( WhoKilledWho[ player ][ localplayer ], tempstr, 10 );
		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_DEATHS_X - w, py, tempstr, color );

		if ( gamestate.teamplay )
			{
			DrawIntensityString( BT_DEATHS_X + 16, py,
				colorname[ PLAYERSTATE[ player ].uniformcolor ], color );
			}

		py += h;
		}
	}


void ShowEndScore( int localplayer )
{
	int  w;
	int  h;
	int  rank;
	int  leader;
	int  team;
	int  color;
	int  killer;
	int  victim;
	int  killcount;
	int  suicidecount;
	char tempstr[15];
	boolean dofullstats;
	int  NumPlayers;

	w=0; h=0;

	// show at the most 11 players
	NumPlayers = min( numplayers, 11 );

	dofullstats = false;
	switch( gamestate.battlemode )
	{
		case battle_Normal :
		case battle_ScoreMore :
		case battle_Hunter :
			dofullstats = true;
			DrawTallyHeader( 0 );
			break;

		case battle_Collector :
		case battle_Scavenger :
		case battle_Tag :
		case battle_Eluder :
		case battle_Deluder :
		case battle_CaptureTheTriad :
			dofullstats = false;
			DrawTallyHeader( 1 );
			break;
	}

	IFont = (cfont_t * )W_CacheLumpNum (W_GetNumForName ("sifont"), PU_CACHE);
	CurrentFont = smallfont;
	py = 43;

	for( rank = 0; rank < BATTLE_NumberOfTeams; rank++ )
	{
		team = BATTLE_PlayerOrder[ rank ];

		color = 21;
		if ( team == BATTLE_Team[ localplayer ] )
		{
			// Change to Intensity
			color = 241;
		}

		// Draw rank if not tied with previous rank
		if ( ( rank == 0 ) || ( BATTLE_Points[ team ] !=
			BATTLE_Points[ BATTLE_PlayerOrder[ rank - 1 ] ] ) )
		{
			rt_itoa( rank + 1, tempstr, 10 );
		}
		else
		{
			strcpy( tempstr, "Tie" );
		}

		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_RANK_X - w, py, tempstr, color );

		// Draw name of team leader
		leader = BATTLE_TeamLeader[ team ];
		if ( gamestate.teamplay )
		{
			DrawIntensityString( BT_PLAYER_X, py,
				colorname[ PLAYERSTATE[ leader ].uniformcolor ], color );
		}
		else
		{
			DrawIntensityString( BT_PLAYER_X, py,
				PLAYERSTATE[ leader ].codename, color );
		}

		if ( dofullstats )
		{
			// Count how many kills each person on the team got
			killcount = 0;
			suicidecount = 0;
			for( killer = 0; killer < NumPlayers; killer++ )
			{
				if ( BATTLE_Team[ killer ] == team )
				{
					for( victim = 0; victim < NumPlayers; victim++ )
					{
						if ( BATTLE_Team[ victim ] != team )
						{
							killcount += WhoKilledWho[ killer ][ victim ];
						}
						else
						{
							suicidecount += WhoKilledWho[ killer ][ victim ];
						}
					}
				}
			}

			// Draw kills
			rt_itoa( killcount, tempstr, 10 );
			VW_MeasureIntensityPropString ( tempstr, &w, &h);
			DrawIntensityString( BT_KILLS_X - w, py, tempstr, color );

			// Draw suicides
			rt_itoa( suicidecount, tempstr, 10 );
			VW_MeasureIntensityPropString ( tempstr, &w, &h);
			DrawIntensityString( BT_DEATHS_X - w, py, tempstr, color );
		}

		// Draw Score
		rt_itoa( BATTLE_Points[ team ], tempstr, 10 );
		VW_MeasureIntensityPropString ( tempstr, &w, &h);
		DrawIntensityString( BT_SCORE_X - w, py, tempstr, color );

		py += h;
	}
}


void BattleLevelCompleted ( int localplayer )
{
	ControlInfo ci;
	int w;
	int h;
	int key;
	int Screen;
	int LastScreen;
	int Player;
	char text[80];

	w=0;
	h=0;

	IN_ClearKeysDown ();

	Player = localplayer;
	Screen = 1;
	LastScreen = 0;
	key = -1;
	while( 1 )
	{
		if ( Screen != LastScreen )
		{
			VL_DrawPostPic (W_GetNumForName("trilogo"));

			switch( Screen )
			{
				case 1 :
					ShowEndScore( Player );
					break;

				case 2 :
					ShowKills( Player );
					break;

				case 3 :
					ShowDeaths( Player );
					break;
			}

			CurrentFont = tinyfont;

			sprintf ( text, "Page %d of 3.  Use arrows to switch stats.  "
				"Press Esc to quit.", Screen );
			VW_MeasurePropString ( text, &w, &h);
			py = 192;
			px = ( 320 - w ) / 2;
			VWB_DrawPropString ( text );
			VW_UpdateScreen ();

			do
			{
				ReadAnyControl (&ci);
			}
			while( ci.dir == key );
		}

		LastScreen = Screen;
		ReadAnyControl ( &ci );
		key = ci.dir;
		if ( ( Screen > 1 ) && ( key == dir_West ) )
		{
			Screen--;
			MN_PlayMenuSnd (SD_MOVECURSORSND);
		}
		else if ( ( Screen < 3 ) && ( key == dir_East ) )
		{
			Screen++;
			MN_PlayMenuSnd (SD_MOVECURSORSND);
		}
		// Allow us to select which player to view
		if ( Keyboard[ sc_RShift ] && ( key == dir_South ) )
		{
			Player++;
			if ( Player >= numplayers )
			{
				Player = 0;
			}
			LastScreen = 0;
			MN_PlayMenuSnd (SD_SELECTSND);
		}

		if ( Keyboard[ sc_RShift ] && ( key == dir_North ) )
		{
			Player--;
			if ( Player < 0 )
			{
				Player = numplayers - 1;
			}
			LastScreen = 0;
			MN_PlayMenuSnd (SD_SELECTSND);
		}

		if ( Keyboard[sc_Escape] )
		{
			break;
		}
	}

	while ( Keyboard[sc_Escape] )
	{
		IN_UpdateKeyboard ();
	}

	MN_PlayMenuSnd (SD_ESCPRESSEDSND);

	CurrentFont = smallfont;
}

//==========================================================================

/*
==================
=
= FindAngleToWindow
=
==================
*/
int FindAngleToWindow ( int tx, int ty )
{
	if (!IsWindow(tx+1,ty))
		return ANG180;
	else if (!IsWindow(tx-1,ty))
		return 0;
	else if (!IsWindow(tx,ty+1))
		return ANG90;
	else
		return ANG270;
}

#define STARTRADIUS (0xa000)
#define STOPRADIUS (0x14000)
#define DEATHRADIUS (STOPRADIUS-STARTRADIUS)
#define ROTRATE		(5)
#define TOTALDEATHROT (FINEANGLES<<1)
#define RADIUSINC	((DEATHRADIUS)/(TOTALDEATHROT))

/*
==================
=
= ZoomDeathOkay
=
==================
*/
boolean ZoomDeathOkay ( void )
{
	int x,y;
	int radius;

	if (
		  !(
		  (player->state==&s_ashwait) ||
		  ((player->flags & FL_HBM) && (gamestate.violence >= vl_high))
		  )
		)
		return false;

	radius=STOPRADIUS;
	x=player->x;
	y=player->y;
	while (radius>0)
		{
		if (tilemap[x>>16][(y+radius)>>16])
			return false;
		if (tilemap[x>>16][(y-radius)>>16])
			return false;
		if (tilemap[(x-radius)>>16][y>>16])
			return false;
		if (tilemap[(x+radius)>>16][y>>16])
			return false;
		if (tilemap[(x+radius)>>16][(y+radius)>>16])
			return false;
		if (tilemap[(x+radius)>>16][(y-radius)>>16])
			return false;
		if (tilemap[(x-radius)>>16][(y+radius)>>16])
			return false;
		if (tilemap[(x-radius)>>16][(y-radius)>>16])
			return false;
		radius-=0x10000;
		}
	return true;
}

/*
==================
=
= Died
=
==================
*/

#define DEATHROTATE 6

extern boolean dopefish;
void Died (void)
{
	long  dx,dy;
	int		iangle,curangle,clockwise,change;
	int	da;
	int	rate;
	lbm_t *LBM;
	int	slowrate;
	playertype *pstate;
	objtype * killerobj=(objtype *)player->target;


	if (killerobj == NULL)
		killerobj = player;

	if (CheckParm("slowdeath"))
		slowrate=3;
	else
		slowrate=0;

	M_LINKSTATE (player, pstate);

	if ( (ZoomDeathOkay()==true) && (pstate->falling==false))
		{
		int x,y,z,radius,heightoffset;
		int endangle,startangle,killangle;
		boolean deadflagset;
		objtype * dummy;

		x=player->x;
		y=player->y;
		z=player->z;
		dummy=player;
		SpawnPlayerobj (x>>16, y>>16, 0, 0);
		player=dummy;
		dummy=new;
		dummy->x=x;
		dummy->drawx=x;
		dummy->y=y;
		dummy->z=z;
		dummy->drawy=y;
		dummy->flags=player->flags;
		player->momentumx=0;
		player->momentumy=0;
		player->speed=0;
		radius=STARTRADIUS;
		heightoffset=pstate->heightoffset;
		deadflagset=false;
		startangle=(player->angle+ANG180)&(FINEANGLES-1);
		endangle=startangle+TOTALDEATHROT;
		killangle=startangle+(TOTALDEATHROT>>1);
		if (dopefish==true)
			{
			AddMessage("Dopefish Death Cam\n",MSG_SYSTEM);
			}
		for (iangle=startangle;;)
			{
			if ( iangle > killangle )
				{
				if ( deadflagset==false )
					{
					dummy->hitpoints=0;
					pstate->health=0;
					dummy->flags &= ~FL_DYING;
					dummy->flags |= FL_SHOOTABLE;
					if (player->state==&s_ashwait)
						dummy->flags |= FL_SKELETON;
					Collision(dummy,(objtype*)NULL,0,0);
					deadflagset=true;
					if ( ( killerobj==player ) && ( gamestate.violence >=
						vl_high ) && ( gamestate.difficulty >= gd_hard ) )
						{
						SD_Play( SD_YOUSUCKSND );
						}
					else
						{
						SD_Play (SD_PLAYERTCDEATHSND+(pstate->player));
						}
					}
				}
			else
				{
				dummy->flags &= ~FL_DYING;
				}
			if (dopefish==true)
				{
				dummy->momentumx+=(RandomNumber("Died",0)<<6)-(256<<5);
				dummy->momentumy+=(RandomNumber("Died",0)<<6)-(256<<5);
				}
			player->x=x+FixedMul(radius,costable[iangle&(FINEANGLES-1)]);
			player->y=y-FixedMul(radius,sintable[iangle&(FINEANGLES-1)]);
			player->z=dummy->z;
			player->angle=(iangle+ANG180)&(FINEANGLES-1);
			if (dopefish==true)
			{
				int dx2,dy2;

				dx2 = dummy->x - player->x;
				dy2 = player->y - dummy->y;

				if (dx2 && dy2)
					player->angle = atan2_appx (dx2,dy2);
			}
			pstate->heightoffset=heightoffset;
			player->yzangle=0;
			UpdateGameObjects();
			player->momentumx=0;
			player->momentumy=0;
			player->speed=0;
			ThreeDRefresh ();
			AnimateWalls();
			DoSprites();
			DoAnimatedMaskedWalls();
			UpdatePlayers();
			UpdateLightLevel(player->areanumber);

			if (iangle<endangle)
				{
				iangle+=(tics<<ROTRATE);
				radius+=tics*(RADIUSINC<<ROTRATE);
				}
			if ( (dummy->state==dummy->state->next) && (iangle>=endangle) )
				break;
			}
		}
	else if (pstate->falling==false)
		{

		//
		// swing around to face attacker
		//

		rate=DEATHROTATE-slowrate;
			{
			if (killerobj==player)
				{
				iangle=player->angle;
				if ( ( gamestate.violence >= vl_high ) &&
					( gamestate.difficulty >= gd_hard ) )
					{
					SD_Play( SD_YOUSUCKSND );
					}
				}
			else
				{
				SD_Play (SD_PLAYERTCDEATHSND+(pstate->player));
				if (killerobj->which==PWALL)
				{
					dx = ((pwallobj_t *)killerobj)->x - player->x;
					dy = player->y - ((pwallobj_t *)killerobj)->y;
				}
				else
				{
					dx = killerobj->x - player->x;
					dy = player->y - killerobj->y;
				}

				iangle = atan2_appx (dx,dy);		 // returns -pi to pi
				}
			}

		da = iangle-player->angle;

		if (da>0)
			clockwise=1;
		else
			clockwise=0;
		da=abs(da);
		if (da>ANG180)
			{
			clockwise^=1;
			da=ANGLES-da;
			}

		curangle = player->angle;

		do
			{
			DoBorderShifts ();
			change = tics<<rate;
			if (clockwise==1)
				curangle+=change;
			else
				curangle-=change;
			da-=change;
			if (curangle >= ANGLES)
				curangle -= ANGLES;
			if (curangle < 0)
				curangle += ANGLES;
			player->angle = (curangle & (FINEANGLES-1));
			ThreeDRefresh ();
			CalcTics ();
			} while (da>0);
		}
	else
		{
		DrawFullSky();
		FlipPage();
		}

	while (damagecount)
		DoBorderShifts ();
	DoBorderShifts ();

	locplayerstate->weapon = -1;		  // take away weapon

	if (
		  (tedlevel == false) && // SO'S YA DON'T GET KILLED WHILE LAUNCHING!
		  (timelimitenabled == false)
		)
		locplayerstate->lives--;

	if (pstate->falling==false)
		{
		ThreeDRefresh ();
		}

	FlipPage();
	FlipPage();

	if (locplayerstate->lives > -1)
	{
		int rng;

		rng = RandomNumber ("Died",0);

		if (pstate->falling==true)
			{
			RotateBuffer (0, 0, (FINEANGLES), (FINEANGLES>>6), (VBLCOUNTER*(1+slowrate)));
			SD_Play (SD_PLAYERTCDEATHSND+(pstate->player));
			pstate->falling=false;
			}

		else if (rng < 64)
			RotateBuffer (0, 0, (FINEANGLES), (FINEANGLES*64), (VBLCOUNTER*(2+slowrate)));
		else if (rng < 128)
			{
			RotateBuffer (0, 0, (FINEANGLES), (FINEANGLES>>6), (VBLCOUNTER*(1+slowrate)));
			}
		else if (rng < 192)
			RotateBuffer(0, (FINEANGLES*4), (FINEANGLES), (FINEANGLES*64), (VBLCOUNTER*(3+slowrate)));
		else
			VL_FadeToColor (VBLCOUNTER*2, 100, 0, 0);

		screenfaded=false;

		VL_FadeOut (0, 255, 0,0,0,VBLCOUNTER>>1);
		gamestate.episode = 1;
		player->flags &= ~FL_DONE;

		InitializeWeapons (locplayerstate);
		ResetPlayerstate(locplayerstate);

		UpdateLives (locplayerstate->lives);
		UpdateScore (gamestate.score);

		DrawTriads(true);
		DrawLives (true);
		DrawKeys (true);
		DrawScore (true);
	}
	else
	{
		int rng;

		SD_Play (SD_GAMEOVERSND);
		rng=RandomNumber("Died",0);
		if (rng<64)
			RotateBuffer(0,(FINEANGLES>>1),(FINEANGLES),(FINEANGLES*64),(VBLCOUNTER*(3+slowrate)));
		else if (rng<128)
			VL_FadeToColor (VBLCOUNTER*3, 255, 255, 255);
		else if (rng<192)
			RotateBuffer(0,(FINEANGLES*2),(FINEANGLES),(FINEANGLES*64),(VBLCOUNTER*(3+slowrate)));
		else
			RotateBuffer(0,(FINEANGLES*2),(FINEANGLES),(FINEANGLES*64),(VBLCOUNTER*(3+slowrate)));

		screenfaded=false;

		VL_FadeOut (0, 255, 0,0,0,VBLCOUNTER>>1);

		MU_StartSong(song_gameover);

#if (SHAREWARE==0)
		if (gamestate.violence==vl_excessive)
			LBM = (lbm_t *) W_CacheLumpNum (W_GetNumForName ("bootblod"), PU_CACHE);
		else
			LBM = (lbm_t *) W_CacheLumpNum (W_GetNumForName ("bootnorm"), PU_CACHE);
#else
		LBM = (lbm_t *) W_CacheLumpNum (W_GetNumForName ("bootblod"), PU_CACHE);
#endif
		VL_DecompressLBM (LBM,true);

		StopWind();

		IN_UserInput (VBLCOUNTER*60);

		MainMenu[savegame].active = 0;
		MainMenu[viewscores].routine = (void *)CP_ViewScores;
		MainMenu[viewscores].texture[6] = '7';
		MainMenu[viewscores].texture[7] = '\0';
		MainMenu[viewscores].letter	  = 'V';
	}
	ClearGraphicsScreen();

	VL_FadeIn (0, 255, origpal, 15);
}


//******************************************************************************
//
// DoLoadGameAction ()
//
//******************************************************************************

static byte whichstr = 0;

void DoLoadGameAction (void)
{
	if ((SaveTime+1) < ticcount)
	{
		int temp = bufferofs;

		bufferofs = displayofs;
		SaveTime = ticcount;

		CurrentFont=tinyfont;

		px = 92;
		py = 152;
		if (whichstr)
		{
			VW_DrawPropString ("�");
			whichstr = 0;
		}
		else
		{
			VW_DrawPropString ("�");
			whichstr = 1;
		}
		bufferofs = temp;
	}
}

//******************************************************************************
//
// DoCheckSum ()
//
//******************************************************************************

long DoCheckSum (byte *source, int size, long csum)
{
	int i;
	long checksum;

	checksum = csum;

	for (i = 0; i < size; i++)
		checksum=updatecrc(checksum,*(source+i));

	return checksum;
}

//******************************************************************************
//
// CaculateSaveGameCheckSum ()
//
//******************************************************************************

#define SAVECHECKSUMSIZE (10000)
long CalculateSaveGameCheckSum (char * filename)
{
	int handle;
	int lengthleft;
	int length;
	byte * altbuffer;
	long checksum;

	altbuffer=SafeMalloc(SAVECHECKSUMSIZE);

	checksum = 0;

	// Open the savegame file

	handle = SafeOpenRead (filename);
	lengthleft = w_filelength (handle);

	while (lengthleft>0)
		{
		length=SAVECHECKSUMSIZE;
		if (length>lengthleft)
			length=lengthleft;

		SafeRead(handle,altbuffer,length);
		checksum = DoCheckSum (altbuffer, length, checksum);

		lengthleft-=length;
		}

	SafeFree(altbuffer);

	w_close (handle);

	return checksum;
}

//******************************************************************************
//
// StoreBuffer
//
//******************************************************************************
void StoreBuffer (int handle, byte * src, int size)
{
	SafeWrite(handle,&size,sizeof(size));
	SafeWrite(handle,src,size);
}

//******************************************************************************
//
// SaveTag
//
//******************************************************************************
void SaveTag (int handle, char * tag, int size)
{
	SafeWrite(handle,tag,size);
}


//******************************************************************************
//
// SaveTheGame ()
//
// Expects game to be premalloced
//
//******************************************************************************

boolean SaveTheGame (int num, gamestorage_t * game)
{
//	struct diskfree_t dfree;
	char	loadname[45]="ROTTGAM0.ROT";
	char	filename[ 128 ];
	byte	* altbuffer;
	int	 size;
	int	 avail;
	int	 savehandle;
	int	 crc;
	int	 i;
	char	letter;

	if (num > 15 || num < 0)
		Error("Illegal Saved game value=%ld\n",num);

	altbuffer=NULL;

	//
	// Save Alternate Game Level information for reloading game
	//
	memset (&game->info, 0, sizeof (game->info));
	if (GameLevels.avail == true)
	{
		memcpy (&game->info.path[0], &GameLevels.path[0], sizeof (GameLevels.path));
		memcpy (&game->info.file[0], &GameLevels.file[0], sizeof (GameLevels.file));
		game->info.avail = true;
	}

	game->mapcrc=GetMapCRC (gamestate.mapon);

	// Create the proper file name

	rt_itoa(num,&loadname[7],16);
	loadname[8]='.';


	GetPathFromEnvironment( filename, ApogeePath, loadname );

	// Determine available disk space

#if 0
	letter = toupper(filename[0]);
	if (
		 (letter >= 'A') &&
		 (letter <= 'Q')
		)
		{
		if (_dos_getdiskfree ((letter-'A'+1), &dfree))
			Error ("Error in _dos_getdiskfree call\n");
		}
	else
		{
		if (_dos_getdiskfree (0, &dfree))
			Error ("Error in _dos_getdiskfree call\n");
		}

	avail = (int) dfree.avail_clusters *
					  dfree.bytes_per_sector *
					  dfree.sectors_per_cluster;
	avail -= 8192;
#endif

	avail = 99999999;

	// Check to see if we have enough

	if (avail < MAXSAVEDGAMESIZE)
		{
		CP_DisplayMsg ("There is not enough\nspace on your disk\nto Save Game!\nPress any key to continue", 13);
		return (false);
		}

	// Open the savegame file

	savehandle = SafeOpenWrite (filename);

	// Save out file tag

	size=4;
	SaveTag(savehandle,"ROTT",size);

	// Save out header

	size=sizeof(*game);

	SafeWrite(savehandle,game,size);

/////////////////////////////////////////////////////////////////////////////
// Save out rest of save game file beyond this point
/////////////////////////////////////////////////////////////////////////////

	// Door Tag

	size=4;
	SaveTag(savehandle,"DOOR",size);

	// Doors

	SaveDoors(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Elevator Tag

	size = 9;
	SaveTag(savehandle,"ELEVATORS",size);

	// Elevators

	SaveElevators(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Pushwall Tag

	size=5;
	SaveTag(savehandle,"PWALL",size);

	// PushWalls

	SavePushWalls(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// MaskedWalls Tag

	size=5;
	SaveTag(savehandle,"MWALL",size);

	// Masked Walls

	SaveMaskedWalls(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Switches Tag

	size=6;
	SaveTag(savehandle,"SWITCH",size);

	// Switches

	SaveSwitches(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Statics Tag

	size=6;
	SaveTag(savehandle,"STATIC",size);

	// Statics

	SaveStatics(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Actors Tag

	size=5;
	SaveTag(savehandle,"ACTOR",size);

	// Actors

	SaveActors(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// TouchPlates Tag

	size=5;
	SaveTag(savehandle,"TOUCH",size);

	// TouchPlates

	SaveTouchPlates(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// GameState Tag

	size=9;
	SaveTag(savehandle,"GAMESTATE",size);

	// GameState

	size=sizeof(gamestate);
	SafeWrite(savehandle,&gamestate,size);

	// PlayerState Tag

	size=12;
	SaveTag(savehandle,"PLAYERSTATES",size);

	// PlayerStates
	size=sizeof(playertype);
	for(i=0;i<numplayers;i++)
	  {
		SafeWrite(savehandle,&PLAYERSTATE[i],size);
	  }

	// Mapseen Tag

	size=7;
	SaveTag(savehandle,"MAPSEEN",size);

	// MapSeen

	size=sizeof(mapseen);
	SafeWrite(savehandle,&mapseen,size);

	// Song Tag

	size=4;
	SaveTag(savehandle,"SONG",size);

	// Song info

	MU_SaveMusic(&altbuffer,&size);
	StoreBuffer(savehandle,altbuffer,size);
	SafeFree(altbuffer);

	// Misc Tag

	size=4;
	SaveTag(savehandle,"MISC",size);

	// Misc

	// ticcount
	size=sizeof(ticcount);
	SafeWrite(savehandle,(int *)(&ticcount),size);

	// shaketics
	size=sizeof(SHAKETICS);
	SafeWrite(savehandle,&SHAKETICS,size);

	// damagecount
	size=sizeof(damagecount);
	SafeWrite(savehandle,&damagecount,size);

	// viewsize
	size=sizeof(viewsize);
	SafeWrite(savehandle,&viewsize,size);

	// poweruptimes
	size = sizeof (poweruptime);
	SafeWrite(savehandle,&poweruptime,size);

	size = sizeof (protectiontime);
	SafeWrite(savehandle,&protectiontime,size);

	size = sizeof (powerupheight);
	SafeWrite(savehandle,&powerupheight,size);

	size = sizeof (protectionheight);
	SafeWrite(savehandle,&protectionheight,size);

	size = sizeof (poweradjust);
	SafeWrite(savehandle,&poweradjust,size);

	w_close (savehandle);

	// Calculate CRC

	crc = CalculateSaveGameCheckSum (filename);

	// Append the crc

	savehandle = SafeOpenAppend (filename);

	size=sizeof(crc);
	SafeWrite(savehandle,&crc,size);

	w_close (savehandle);

	pickquick = true;
	return (true);
}


//******************************************************************************
//
// LoadTag
//
//******************************************************************************

void LoadTag (byte ** src, char * tag, int size)
{
	if (StringsNotEqual((char *)*src,(char *)tag,size)==true)
		Error("Could not locate %s header in saved game file\n",tag);
	*src+=size;
}

//******************************************************************************
//
// LoadBuffer
//
//******************************************************************************
int LoadBuffer (byte ** dest, byte ** src)
{
	int size;

	size=0;
	memcpy(&size,*src,sizeof(size));
	*src+=sizeof(size);
	*dest=SafeMalloc(size);
	memcpy(*dest,*src,size);
	*src+=size;
	return size;
}


//******************************************************************************
//
// LoadTheGame ()
//
// Expects game to be premalloced
//
//******************************************************************************

boolean LoadTheGame (int num, gamestorage_t * game)
{
	char	loadname[45]="ROTTGAM0.ROT";
	char	filename[128];
	byte	* loadbuffer;
	byte	* bufptr;
	byte	* altbuffer;
	int	 size;
	int	 totalsize;
	int	 checksum;
	int	 savedchecksum;
	int	 i;
	word	mapcrc;

	loadbuffer=NULL;
	altbuffer=NULL;
	savedchecksum=0;

	if (num>15 || num<0)
		Error("Illegal Load game value=%ld\n",num);

	// Create the proper file name

	rt_itoa(num,&loadname[7],16);
	loadname[8]='.';

	GetPathFromEnvironment( filename, ApogeePath, loadname );

	// Load the file

	totalsize=LoadFile(filename,(void **)(&loadbuffer));
	bufptr=loadbuffer;

	// Calculate checksum

	checksum = DoCheckSum (loadbuffer, totalsize-sizeof(checksum), 0);

	// Retrieve saved checksum

	memcpy (&savedchecksum,loadbuffer+(totalsize-sizeof(savedchecksum)),sizeof(savedchecksum));

	// Compare the two checksums;

	if (checksum!=savedchecksum)
		{
		if (CP_DisplayMsg ("Your Saved Game file is\n"
								 "shall we say, \"corrupted\".\n"
								 "Would you like to\n"
								 "continue anyway (Y/N)?\n", 12)==false)
			{
			return false;
			}
		}

	// Load in file tag

	size=4;
	LoadTag(&bufptr,"ROTT",size);

	// Load in header

	size=sizeof(*game);
	memcpy(game,bufptr,size);
	bufptr+=size;

	if (game->version!=ROTTVERSION)
		return false;

	memcpy (&GameLevels, &game->info, sizeof (GameLevels));

	gamestate.episode=game->episode;
	gamestate.mapon=game->area;

	mapcrc=GetMapCRC (gamestate.mapon);

	if (mapcrc!=game->mapcrc)
		return false;

/////////////////////////////////////////////////////////////////////////////
// Load in rest of saved game file beyond this point
/////////////////////////////////////////////////////////////////////////////

	// Free up the current level
	Z_FreeTags (PU_LEVELSTRUCT, PU_LEVELEND);		 // Free current level

	gamestate.battlemode = battle_StandAloneGame;
	BATTLE_SetOptions( &BATTLE_Options[ battle_StandAloneGame ] );
	BATTLE_Init( gamestate.battlemode, 1 );

	DoLoadGameAction ();
	SetupGameLevel();

	// Door Tag

	size=4;
	LoadTag(&bufptr,"DOOR",size);

	// Doors

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadDoors(altbuffer,size);
	SafeFree(altbuffer);

	// Elevator Tag

	size = 9;
	LoadTag(&bufptr,"ELEVATORS",size);


	// Elevators

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadElevators(altbuffer,size);
	SafeFree(altbuffer);

	// Pushwall Tag

	size=5;
	LoadTag(&bufptr,"PWALL",size);

	// PushWalls

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadPushWalls(altbuffer,size);
	SafeFree(altbuffer);
#if 0
	// Animated Walls Tag

	size=5;
	LoadTag(&bufptr,"AWALL",size);

	// Animated Walls
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadAnimWalls(altbuffer,size);
	SafeFree(altbuffer);
#endif

	// MaskedWalls Tag

	size=5;
	LoadTag(&bufptr,"MWALL",size);

	// Masked Walls

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadMaskedWalls(altbuffer,size);
	SafeFree(altbuffer);

	// Switches Tag

	size=6;
	LoadTag(&bufptr,"SWITCH",size);

	// Switches

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadSwitches(altbuffer,size);
	SafeFree(altbuffer);


	// Statics Tag

	size=6;
	LoadTag(&bufptr,"STATIC",size);

	// Statics

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadStatics(altbuffer,size);
	SafeFree(altbuffer);

	// Actors Tag

	size=5;
	LoadTag(&bufptr,"ACTOR",size);

	// Actors

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadActors(altbuffer,size);
	SafeFree(altbuffer);

	// TouchPlates Tag

	size=5;
	LoadTag(&bufptr,"TOUCH",size);

	// TouchPlates

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	LoadTouchPlates(altbuffer,size);
	SafeFree(altbuffer);

	// SetupWindows

	SetupWindows();

	// GameState Tag

	size=9;
	LoadTag(&bufptr,"GAMESTATE",size);

	// GameState

	DoLoadGameAction ();
	size=sizeof(gamestate);
	memcpy(&gamestate,bufptr,size);
	bufptr+=size;

	// PlayerState Tag

	size=12;
	LoadTag(&bufptr,"PLAYERSTATES",size);

	// PlayerState

	DoLoadGameAction ();
	size=sizeof(playertype);
	for(i=0;i<numplayers;i++)
	 {memcpy(&PLAYERSTATE[i],bufptr,size);
	  bufptr+=size;
	 }

	// Zero out player targets

	locplayerstate->guntarget=0;
	locplayerstate->targettime=0;

	// Mapseen Tag

	size=7;
	LoadTag(&bufptr,"MAPSEEN",size);

	// MapSeen

	DoLoadGameAction ();
	size=sizeof(mapseen);
	memcpy(&mapseen,bufptr,size);
	bufptr+=size;

	// Song Tag

	size=4;
	LoadTag(&bufptr,"SONG",size);

	// Song info

	DoLoadGameAction ();
	size=LoadBuffer(&altbuffer,&bufptr);
	MU_LoadMusic(altbuffer,size);
	SafeFree(altbuffer);

	// Misc Tag

	size=4;
	LoadTag(&bufptr,"MISC",size);

	// Misc

	// ticcount
	DoLoadGameAction ();
	size=sizeof(ticcount);
//	memcpy(&ticcount,bufptr,size);
	memcpy((void *)(&ticcount),bufptr,size);
	bufptr+=size;
	SaveTime = ticcount;

	// shaketics
	DoLoadGameAction ();
	size=sizeof(SHAKETICS);
	memcpy(&SHAKETICS,bufptr,size);
	bufptr+=size;

	// damagecount
	DoLoadGameAction ();
	size=sizeof(damagecount);
	memcpy(&damagecount,bufptr,size);
	bufptr+=size;

	// viewsize
	DoLoadGameAction ();
	size=sizeof(viewsize);
	memcpy(&viewsize,bufptr,size);
	bufptr+=size;

	// powerup times
	DoLoadGameAction ();
	size = sizeof (poweruptime);
	memcpy (&poweruptime, bufptr, size);
	bufptr += size;
	size = sizeof (protectiontime);
	memcpy (&protectiontime, bufptr, size);
	bufptr += size;
	size = sizeof (powerupheight);
	memcpy (&powerupheight, bufptr, size);
	bufptr += size;
	size = sizeof (protectionheight);
	memcpy (&protectionheight, bufptr, size);
	bufptr += size;
	size = sizeof (poweradjust);
	memcpy (&poweradjust, bufptr, size);
	bufptr += size;

  // Set the viewsize

	SetViewSize(viewsize);
	DoLoadGameAction ();

	// Connect areas

	ConnectAreas ();
	DoLoadGameAction ();

	// Free up the loadbuffer

	SafeFree (loadbuffer);
	DoLoadGameAction ();

	Illuminate();

	IN_UpdateKeyboard ();
	LoadPlayer ();
	DoLoadGameAction ();
	SetupPlayScreen();
	UpdateScore (gamestate.score);
	UpdateLives (locplayerstate->lives);
	UpdateTriads (player, 0);
	PreCache ();
	InitializeMessages();

	for (i=0;i<100;i++)
		UpdateLightLevel(player->areanumber);

	CalcTics();
	CalcTics();

	pickquick = true;

	return (true);
}


//******************************************************************************
//
// GetSavedMessage ()
//
// Expects message to be premalloced
//
//******************************************************************************

void GetSavedMessage (int num, char * message)
{
	gamestorage_t game;
	char	loadname[45]="ROTTGAM0.ROT";
	char	filename[128];
	byte	* loadbuffer;
	byte	* bufptr;
	int	 size;

	if (num>15 || num<0)
		Error("Illegal Load game value=%ld\n",num);

	// Create the proper file name

	rt_itoa(num,&loadname[7],16);
	loadname[8]='.';

	GetPathFromEnvironment( filename, ApogeePath, loadname );

	// Load the file

	size=LoadFile(filename,(void **)(&loadbuffer));
	bufptr=loadbuffer;

	size=4;
	LoadTag(&bufptr,"ROTT",size);

	// Load in header

	size=sizeof(game);
	memcpy(&game,bufptr,size);
	strcpy(message,game.message);
	SafeFree(loadbuffer);
}

//******************************************************************************
//
// GetSavedHeader ()
//
// Expects game to be premalloced
//
//******************************************************************************

void GetSavedHeader (int num, gamestorage_t * game)
{
	char	loadname[45]="ROTTGAM0.ROT";
	char	filename[128];
	byte	* loadbuffer;
	byte	* bufptr;
	int	 size;

	if (num>15 || num<0)
		Error("Illegal Load game value=%ld\n",num);

	// Create the proper file name

	rt_itoa(num,&loadname[7],16);
	loadname[8]='.';

	GetPathFromEnvironment( filename, ApogeePath, loadname );

	// Load the file

	loadbuffer=NULL;

	size=LoadFile(filename,(void **)(&loadbuffer));
	bufptr=loadbuffer;

	size=4;
	LoadTag(&bufptr,"ROTT",size);

	// Load in header

	size=sizeof(*game);
	memcpy(game,bufptr,size);
	SafeFree(loadbuffer);
}



//******************************************************************************
//
// GetLevel ()
//
//******************************************************************************

int GetLevel (int episode, int mapon)
{
	int level;
	level = (mapon+1) - ((episode-1) << 3);
	return (level);
}
