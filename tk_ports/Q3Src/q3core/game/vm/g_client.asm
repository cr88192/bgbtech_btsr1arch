data
align 4
LABELV playerMins
byte 4 3245342720
byte 4 3245342720
byte 4 3250585600
align 4
LABELV playerMaxs
byte 4 1097859072
byte 4 1097859072
byte 4 1107296256
export SP_info_player_deathmatch
code
proc SP_info_player_deathmatch 8 12
file "../g_client.c"
line 37
;1:/*
;2:===========================================================================
;3:Copyright (C) 1999-2005 Id Software, Inc.
;4:
;5:This file is part of Quake III Arena source code.
;6:
;7:Quake III Arena source code is free software; you can redistribute it
;8:and/or modify it under the terms of the GNU General Public License as
;9:published by the Free Software Foundation; either version 2 of the License,
;10:or (at your option) any later version.
;11:
;12:Quake III Arena source code is distributed in the hope that it will be
;13:useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;14:MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;15:GNU General Public License for more details.
;16:
;17:You should have received a copy of the GNU General Public License
;18:along with Foobar; if not, write to the Free Software
;19:Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
;20:===========================================================================
;21:*/
;22://
;23:#include "g_local.h"
;24:
;25:// g_client.c -- client functions that don't happen every frame
;26:
;27:static vec3_t	playerMins = {-15, -15, -24};
;28:static vec3_t	playerMaxs = {15, 15, 32};
;29:
;30:/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
;31:potential spawning position for deathmatch games.
;32:The first time a player enters the game, they will be at an 'initial' spot.
;33:Targets will be fired when someone spawns in on them.
;34:"nobots" will prevent bots from using this spot.
;35:"nohumans" will prevent non-bots from using this spot.
;36:*/
;37:void SP_info_player_deathmatch( gentity_t *ent ) {
line 40
;38:	int		i;
;39:
;40:	G_SpawnInt( "nobots", "0", &i);
ADDRGP4 $53
ARGP4
ADDRGP4 $54
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 41
;41:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $55
line 42
;42:		ent->flags |= FL_NO_BOTS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 8192
BORI4
ASGNI4
line 43
;43:	}
LABELV $55
line 44
;44:	G_SpawnInt( "nohumans", "0", &i );
ADDRGP4 $57
ARGP4
ADDRGP4 $54
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 45
;45:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $58
line 46
;46:		ent->flags |= FL_NO_HUMANS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 47
;47:	}
LABELV $58
line 48
;48:}
LABELV $52
endproc SP_info_player_deathmatch 8 12
export SP_info_player_start
proc SP_info_player_start 0 4
line 53
;49:
;50:/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
;51:equivelant to info_player_deathmatch
;52:*/
;53:void SP_info_player_start(gentity_t *ent) {
line 54
;54:	ent->classname = "info_player_deathmatch";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $61
ASGNP4
line 55
;55:	SP_info_player_deathmatch( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SP_info_player_deathmatch
CALLV
pop
line 56
;56:}
LABELV $60
endproc SP_info_player_start 0 4
export SP_info_player_intermission
proc SP_info_player_intermission 0 0
line 61
;57:
;58:/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
;59:The intermission will be viewed from this point.  Target an info_notnull for the view direction.
;60:*/
;61:void SP_info_player_intermission( gentity_t *ent ) {
line 63
;62:
;63:}
LABELV $62
endproc SP_info_player_intermission 0 0
export SpotWouldTelefrag
proc SpotWouldTelefrag 4144 16
line 81
;64:
;65:
;66:
;67:/*
;68:=======================================================================
;69:
;70:  SelectSpawnPoint
;71:
;72:=======================================================================
;73:*/
;74:
;75:/*
;76:================
;77:SpotWouldTelefrag
;78:
;79:================
;80:*/
;81:qboolean SpotWouldTelefrag( gentity_t *spot ) {
line 87
;82:	int			i, num;
;83:	int			touch[MAX_GENTITIES];
;84:	gentity_t	*hit;
;85:	vec3_t		mins, maxs;
;86:
;87:	VectorAdd( spot->s.origin, playerMins, mins );
ADDRLP4 4132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4132
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMins
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+4
ADDRLP4 4132
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMins+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMins+8
INDIRF4
ADDF4
ASGNF4
line 88
;88:	VectorAdd( spot->s.origin, playerMaxs, maxs );
ADDRLP4 4136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4136
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMaxs
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+4
ADDRLP4 4136
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMaxs+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMaxs+8
INDIRF4
ADDF4
ASGNF4
line 89
;89:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 4108
ARGP4
ADDRLP4 4120
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4140
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4140
INDIRI4
ASGNI4
line 91
;90:
;91:	for (i=0 ; i<num ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $75
JUMPV
LABELV $72
line 92
;92:		hit = &g_entities[touch[i]];
ADDRLP4 4
CNSTI4 808
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 94
;93:		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
;94:		if ( hit->client) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $76
line 95
;95:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $63
JUMPV
LABELV $76
line 98
;96:		}
;97:
;98:	}
LABELV $73
line 91
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $75
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $72
line 100
;99:
;100:	return qfalse;
CNSTI4 0
RETI4
LABELV $63
endproc SpotWouldTelefrag 4144 16
export SelectNearestDeathmatchSpawnPoint
proc SelectNearestDeathmatchSpawnPoint 40 12
line 111
;101:}
;102:
;103:/*
;104:================
;105:SelectNearestDeathmatchSpawnPoint
;106:
;107:Find the spot that we DON'T want to use
;108:================
;109:*/
;110:#define	MAX_SPAWN_POINTS	128
;111:gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
line 117
;112:	gentity_t	*spot;
;113:	vec3_t		delta;
;114:	float		dist, nearestDist;
;115:	gentity_t	*nearestSpot;
;116:
;117:	nearestDist = 999999;
ADDRLP4 20
CNSTF4 1232348144
ASGNF4
line 118
;118:	nearestSpot = NULL;
ADDRLP4 24
CNSTP4 0
ASGNP4
line 119
;119:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $80
JUMPV
LABELV $79
line 121
;120:
;121:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 123
;122:
;123:		VectorSubtract( spot->s.origin, from, delta );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 124
;124:		dist = VectorLength( delta );
ADDRLP4 4
ARGP4
ADDRLP4 36
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 36
INDIRF4
ASGNF4
line 125
;125:		if ( dist < nearestDist ) {
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
GEF4 $84
line 126
;126:			nearestDist = dist;
ADDRLP4 20
ADDRLP4 16
INDIRF4
ASGNF4
line 127
;127:			nearestSpot = spot;
ADDRLP4 24
ADDRLP4 0
INDIRP4
ASGNP4
line 128
;128:		}
LABELV $84
line 129
;129:	}
LABELV $80
line 121
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 28
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $79
line 131
;130:
;131:	return nearestSpot;
ADDRLP4 24
INDIRP4
RETP4
LABELV $78
endproc SelectNearestDeathmatchSpawnPoint 40 12
export SelectRandomDeathmatchSpawnPoint
proc SelectRandomDeathmatchSpawnPoint 532 12
line 143
;132:}
;133:
;134:
;135:/*
;136:================
;137:SelectRandomDeathmatchSpawnPoint
;138:
;139:go to a random point that doesn't telefrag
;140:================
;141:*/
;142:#define	MAX_SPAWN_POINTS	128
;143:gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
line 149
;144:	gentity_t	*spot;
;145:	int			count;
;146:	int			selection;
;147:	gentity_t	*spots[MAX_SPAWN_POINTS];
;148:
;149:	count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 150
;150:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $88
JUMPV
LABELV $87
line 152
;151:
;152:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 153
;153:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 524
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 524
INDIRI4
CNSTI4 0
EQI4 $90
line 154
;154:			continue;
ADDRGP4 $88
JUMPV
LABELV $90
line 156
;155:		}
;156:		spots[ count ] = spot;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 157
;157:		count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 158
;158:	}
LABELV $88
line 152
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 524
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 524
INDIRP4
ASGNP4
ADDRLP4 524
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $87
line 160
;159:
;160:	if ( !count ) {	// no spots that won't telefrag
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $92
line 161
;161:		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 528
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $86
JUMPV
LABELV $92
line 164
;162:	}
;163:
;164:	selection = rand() % count;
ADDRLP4 528
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 520
ADDRLP4 528
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
ASGNI4
line 165
;165:	return spots[ selection ];
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
RETP4
LABELV $86
endproc SelectRandomDeathmatchSpawnPoint 532 12
export SelectRandomFurthestSpawnPoint
proc SelectRandomFurthestSpawnPoint 572 12
line 175
;166:}
;167:
;168:/*
;169:===========
;170:SelectRandomFurthestSpawnPoint
;171:
;172:Chooses a player start, deathmatch start, etc
;173:============
;174:*/
;175:gentity_t *SelectRandomFurthestSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 183
;176:	gentity_t	*spot;
;177:	vec3_t		delta;
;178:	float		dist;
;179:	float		list_dist[64];
;180:	gentity_t	*list_spot[64];
;181:	int			numSpots, rnd, i, j;
;182:
;183:	numSpots = 0;
ADDRLP4 520
CNSTI4 0
ASGNI4
line 184
;184:	spot = NULL;
ADDRLP4 528
CNSTP4 0
ASGNP4
ADDRGP4 $96
JUMPV
LABELV $95
line 186
;185:
;186:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 187
;187:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 548
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 548
INDIRI4
CNSTI4 0
EQI4 $98
line 188
;188:			continue;
ADDRGP4 $96
JUMPV
LABELV $98
line 190
;189:		}
;190:		VectorSubtract( spot->s.origin, avoidPoint, delta );
ADDRLP4 556
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 532
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+4
ADDRLP4 528
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+8
ADDRLP4 528
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 191
;191:		dist = VectorLength( delta );
ADDRLP4 532
ARGP4
ADDRLP4 560
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 524
ADDRLP4 560
INDIRF4
ASGNF4
line 192
;192:		for (i = 0; i < numSpots; i++) {
ADDRLP4 516
CNSTI4 0
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $102
line 193
;193:			if ( dist > list_dist[i] ) {
ADDRLP4 524
INDIRF4
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
LEF4 $106
line 194
;194:				if ( numSpots >= 64 )
ADDRLP4 520
INDIRI4
CNSTI4 64
LTI4 $108
line 195
;195:					numSpots = 64-1;
ADDRLP4 520
CNSTI4 63
ASGNI4
LABELV $108
line 196
;196:				for (j = numSpots; j > i; j--) {
ADDRLP4 0
ADDRLP4 520
INDIRI4
ASGNI4
ADDRGP4 $113
JUMPV
LABELV $110
line 197
;197:					list_dist[j] = list_dist[j-1];
ADDRLP4 564
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 564
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 564
INDIRI4
ADDRLP4 4-4
ADDP4
INDIRF4
ASGNF4
line 198
;198:					list_spot[j] = list_spot[j-1];
ADDRLP4 568
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 568
INDIRI4
ADDRLP4 260
ADDP4
ADDRLP4 568
INDIRI4
ADDRLP4 260-4
ADDP4
INDIRP4
ASGNP4
line 199
;199:				}
LABELV $111
line 196
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $113
ADDRLP4 0
INDIRI4
ADDRLP4 516
INDIRI4
GTI4 $110
line 200
;200:				list_dist[i] = dist;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 201
;201:				list_spot[i] = spot;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 202
;202:				numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 203
;203:				if (numSpots > 64)
ADDRLP4 520
INDIRI4
CNSTI4 64
LEI4 $104
line 204
;204:					numSpots = 64;
ADDRLP4 520
CNSTI4 64
ASGNI4
line 205
;205:				break;
ADDRGP4 $104
JUMPV
LABELV $106
line 207
;206:			}
;207:		}
LABELV $103
line 192
ADDRLP4 516
ADDRLP4 516
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $105
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $102
LABELV $104
line 208
;208:		if (i >= numSpots && numSpots < 64) {
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $118
ADDRLP4 520
INDIRI4
CNSTI4 64
GEI4 $118
line 209
;209:			list_dist[numSpots] = dist;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 210
;210:			list_spot[numSpots] = spot;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 211
;211:			numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 212
;212:		}
LABELV $118
line 213
;213:	}
LABELV $96
line 186
ADDRLP4 528
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 548
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 548
INDIRP4
ASGNP4
ADDRLP4 548
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $95
line 214
;214:	if (!numSpots) {
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $120
line 215
;215:		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 552
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 552
INDIRP4
ASGNP4
line 216
;216:		if (!spot)
ADDRLP4 528
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $122
line 217
;217:			G_Error( "Couldn't find a spawn point" );
ADDRGP4 $124
ARGP4
ADDRGP4 G_Error
CALLV
pop
LABELV $122
line 218
;218:		VectorCopy (spot->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 219
;219:		origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 220
;220:		VectorCopy (spot->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 221
;221:		return spot;
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $94
JUMPV
LABELV $120
line 225
;222:	}
;223:
;224:	// select a random spot from the spawn points furthest away
;225:	rnd = random() * (numSpots / 2);
ADDRLP4 552
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 544
ADDRLP4 552
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 520
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 227
;226:
;227:	VectorCopy (list_spot[rnd]->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 228
;228:	origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 229
;229:	VectorCopy (list_spot[rnd]->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 231
;230:
;231:	return list_spot[rnd];
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
RETP4
LABELV $94
endproc SelectRandomFurthestSpawnPoint 572 12
export SelectSpawnPoint
proc SelectSpawnPoint 4 12
line 241
;232:}
;233:
;234:/*
;235:===========
;236:SelectSpawnPoint
;237:
;238:Chooses a player start, deathmatch start, etc
;239:============
;240:*/
;241:gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 242
;242:	return SelectRandomFurthestSpawnPoint( avoidPoint, origin, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 SelectRandomFurthestSpawnPoint
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $125
endproc SelectSpawnPoint 4 12
export SelectInitialSpawnPoint
proc SelectInitialSpawnPoint 20 12
line 281
;243:
;244:	/*
;245:	gentity_t	*spot;
;246:	gentity_t	*nearestSpot;
;247:
;248:	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );
;249:
;250:	spot = SelectRandomDeathmatchSpawnPoint ( );
;251:	if ( spot == nearestSpot ) {
;252:		// roll again if it would be real close to point of death
;253:		spot = SelectRandomDeathmatchSpawnPoint ( );
;254:		if ( spot == nearestSpot ) {
;255:			// last try
;256:			spot = SelectRandomDeathmatchSpawnPoint ( );
;257:		}		
;258:	}
;259:
;260:	// find a single player start spot
;261:	if (!spot) {
;262:		G_Error( "Couldn't find a spawn point" );
;263:	}
;264:
;265:	VectorCopy (spot->s.origin, origin);
;266:	origin[2] += 9;
;267:	VectorCopy (spot->s.angles, angles);
;268:
;269:	return spot;
;270:	*/
;271:}
;272:
;273:/*
;274:===========
;275:SelectInitialSpawnPoint
;276:
;277:Try to find a spawn point marked 'initial', otherwise
;278:use normal spawn selection.
;279:============
;280:*/
;281:gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles ) {
line 284
;282:	gentity_t	*spot;
;283:
;284:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $128
JUMPV
LABELV $127
line 285
;285:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 286
;286:		if ( spot->spawnflags & 1 ) {
ADDRLP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $130
line 287
;287:			break;
ADDRGP4 $129
JUMPV
LABELV $130
line 289
;288:		}
;289:	}
LABELV $128
line 285
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRGP4 $61
ARGP4
ADDRLP4 4
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $127
LABELV $129
line 291
;290:
;291:	if ( !spot || SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $134
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $132
LABELV $134
line 292
;292:		return SelectSpawnPoint( vec3_origin, origin, angles );
ADDRGP4 vec3_origin
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
RETP4
ADDRGP4 $126
JUMPV
LABELV $132
line 295
;293:	}
;294:
;295:	VectorCopy (spot->s.origin, origin);
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 296
;296:	origin[2] += 9;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 297
;297:	VectorCopy (spot->s.angles, angles);
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 299
;298:
;299:	return spot;
ADDRLP4 0
INDIRP4
RETP4
LABELV $126
endproc SelectInitialSpawnPoint 20 12
export SelectSpectatorSpawnPoint
proc SelectSpectatorSpawnPoint 0 0
line 308
;300:}
;301:
;302:/*
;303:===========
;304:SelectSpectatorSpawnPoint
;305:
;306:============
;307:*/
;308:gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
line 309
;309:	FindIntermissionPoint();
ADDRGP4 FindIntermissionPoint
CALLV
pop
line 311
;310:
;311:	VectorCopy( level.intermission_origin, origin );
ADDRFP4 0
INDIRP4
ADDRGP4 level+9144
INDIRB
ASGNB 12
line 312
;312:	VectorCopy( level.intermission_angle, angles );
ADDRFP4 4
INDIRP4
ADDRGP4 level+9156
INDIRB
ASGNB 12
line 314
;313:
;314:	return NULL;
CNSTP4 0
RETP4
LABELV $135
endproc SelectSpectatorSpawnPoint 0 0
export InitBodyQue
proc InitBodyQue 12 0
line 330
;315:}
;316:
;317:/*
;318:=======================================================================
;319:
;320:BODYQUE
;321:
;322:=======================================================================
;323:*/
;324:
;325:/*
;326:===============
;327:InitBodyQue
;328:===============
;329:*/
;330:void InitBodyQue (void) {
line 334
;331:	int		i;
;332:	gentity_t	*ent;
;333:
;334:	level.bodyQueIndex = 0;
ADDRGP4 level+9176
CNSTI4 0
ASGNI4
line 335
;335:	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $140
line 336
;336:		ent = G_Spawn();
ADDRLP4 8
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 337
;337:		ent->classname = "bodyque";
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $144
ASGNP4
line 338
;338:		ent->neverFree = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 1
ASGNI4
line 339
;339:		level.bodyQue[i] = ent;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9180
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 340
;340:	}
LABELV $141
line 335
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 8
LTI4 $140
line 341
;341:}
LABELV $138
endproc InitBodyQue 12 0
export BodySink
proc BodySink 4 4
line 350
;342:
;343:/*
;344:=============
;345:BodySink
;346:
;347:After sitting around for five seconds, fall into the ground and dissapear
;348:=============
;349:*/
;350:void BodySink( gentity_t *ent ) {
line 351
;351:	if ( level.time - ent->timestamp > 6500 ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
SUBI4
CNSTI4 6500
LEI4 $147
line 353
;352:		// the body ques are never actually freed, they are just unlinked
;353:		trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 354
;354:		ent->physicsObject = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTI4 0
ASGNI4
line 355
;355:		return;	
ADDRGP4 $146
JUMPV
LABELV $147
line 357
;356:	}
;357:	ent->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 358
;358:	ent->s.pos.trBase[2] -= 1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 359
;359:}
LABELV $146
endproc BodySink 4 4
export CopyToBodyQue
proc CopyToBodyQue 48 8
line 369
;360:
;361:/*
;362:=============
;363:CopyToBodyQue
;364:
;365:A player is respawning, so make an entity that looks
;366:just like the existing corpse to leave behind.
;367:=============
;368:*/
;369:void CopyToBodyQue( gentity_t *ent ) {
line 377
;370:#ifdef MISSIONPACK
;371:	gentity_t	*e;
;372:	int i;
;373:#endif
;374:	gentity_t		*body;
;375:	int			contents;
;376:
;377:	trap_UnlinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 380
;378:
;379:	// if client is in a nodrop area, don't leave the body
;380:	contents = trap_PointContents( ent->s.origin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 8
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 381
;381:	if ( contents & CONTENTS_NODROP ) {
ADDRLP4 4
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
EQU4 $152
line 382
;382:		return;
ADDRGP4 $151
JUMPV
LABELV $152
line 386
;383:	}
;384:
;385:	// grab a body que and cycle to the next one
;386:	body = level.bodyQue[ level.bodyQueIndex ];
ADDRLP4 0
ADDRGP4 level+9176
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9180
ADDP4
INDIRP4
ASGNP4
line 387
;387:	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;
ADDRGP4 level+9176
ADDRGP4 level+9176
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 8
MODI4
ASGNI4
line 389
;388:
;389:	trap_UnlinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 391
;390:
;391:	body->s = ent->s;
ADDRLP4 0
INDIRP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 208
line 392
;392:	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 411
;393:#ifdef MISSIONPACK
;394:	if ( ent->s.eFlags & EF_KAMIKAZE ) {
;395:		body->s.eFlags |= EF_KAMIKAZE;
;396:
;397:		// check if there is a kamikaze timer around for this owner
;398:		for (i = 0; i < MAX_GENTITIES; i++) {
;399:			e = &g_entities[i];
;400:			if (!e->inuse)
;401:				continue;
;402:			if (e->activator != ent)
;403:				continue;
;404:			if (strcmp(e->classname, "kamikaze timer"))
;405:				continue;
;406:			e->activator = body;
;407:			break;
;408:		}
;409:	}
;410:#endif
;411:	body->s.powerups = 0;	// clear powerups
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 412
;412:	body->s.loopSound = 0;	// clear lava burning
ADDRLP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 0
ASGNI4
line 413
;413:	body->s.number = body - g_entities;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ASGNI4
line 414
;414:	body->timestamp = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 415
;415:	body->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTI4 1
ASGNI4
line 416
;416:	body->physicsBounce = 0;		// don't bounce
ADDRLP4 0
INDIRP4
CNSTI4 568
ADDP4
CNSTF4 0
ASGNF4
line 417
;417:	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $159
line 418
;418:		body->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 419
;419:		body->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 420
;420:		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 421
;421:	} else {
ADDRGP4 $160
JUMPV
LABELV $159
line 422
;422:		body->s.pos.trType = TR_STATIONARY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 423
;423:	}
LABELV $160
line 424
;424:	body->s.event = 0;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
CNSTI4 0
ASGNI4
line 428
;425:
;426:	// change the animation to the last-frame only, so the sequence
;427:	// doesn't repeat anew for the body
;428:	switch ( body->s.legsAnim & ~ANIM_TOGGLEBIT ) {
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $162
ADDRLP4 16
INDIRI4
CNSTI4 5
GTI4 $162
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $168
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $168
address $165
address $165
address $166
address $166
address $167
address $167
code
LABELV $165
line 431
;429:	case BOTH_DEATH1:
;430:	case BOTH_DEAD1:
;431:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD1;
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 432
;432:		break;
ADDRGP4 $163
JUMPV
LABELV $166
line 435
;433:	case BOTH_DEATH2:
;434:	case BOTH_DEAD2:
;435:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD2;
ADDRLP4 36
CNSTI4 3
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 436
;436:		break;
ADDRGP4 $163
JUMPV
LABELV $167
LABELV $162
line 440
;437:	case BOTH_DEATH3:
;438:	case BOTH_DEAD3:
;439:	default:
;440:		body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD3;
ADDRLP4 44
CNSTI4 5
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 441
;441:		break;
LABELV $163
line 444
;442:	}
;443:
;444:	body->r.svFlags = ent->r.svFlags;
ADDRLP4 24
CNSTI4 424
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 445
;445:	VectorCopy (ent->r.mins, body->r.mins);
ADDRLP4 28
CNSTI4 436
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 446
;446:	VectorCopy (ent->r.maxs, body->r.maxs);
ADDRLP4 32
CNSTI4 448
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 447
;447:	VectorCopy (ent->r.absmin, body->r.absmin);
ADDRLP4 36
CNSTI4 464
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 448
;448:	VectorCopy (ent->r.absmax, body->r.absmax);
ADDRLP4 40
CNSTI4 476
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 450
;449:
;450:	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 65537
ASGNI4
line 451
;451:	body->r.contents = CONTENTS_CORPSE;
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 67108864
ASGNI4
line 452
;452:	body->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 454
;453:
;454:	body->nextthink = level.time + 5000;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 455
;455:	body->think = BodySink;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 BodySink
ASGNP4
line 457
;456:
;457:	body->die = body_die;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 body_die
ASGNP4
line 460
;458:
;459:	// don't take more damage if already gibbed
;460:	if ( ent->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $170
line 461
;461:		body->takedamage = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 462
;462:	} else {
ADDRGP4 $171
JUMPV
LABELV $170
line 463
;463:		body->takedamage = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 464
;464:	}
LABELV $171
line 467
;465:
;466:
;467:	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 468
;468:	trap_LinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 469
;469:}
LABELV $151
endproc CopyToBodyQue 48 8
export SetClientViewAngle
proc SetClientViewAngle 16 0
line 480
;470:
;471://======================================================================
;472:
;473:
;474:/*
;475:==================
;476:SetClientViewAngle
;477:
;478:==================
;479:*/
;480:void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
line 484
;481:	int			i;
;482:
;483:	// set the delta angle
;484:	for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $173
line 487
;485:		int		cmdAngle;
;486:
;487:		cmdAngle = ANGLE2SHORT(angle[i]);
ADDRLP4 4
CNSTF4 1199570944
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 488
;488:		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 56
ADDP4
ADDP4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 489
;489:	}
LABELV $174
line 484
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $173
line 490
;490:	VectorCopy( angle, ent->s.angles );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 491
;491:	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 152
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 492
;492:}
LABELV $172
endproc SetClientViewAngle 16 0
export respawn
proc respawn 12 8
line 499
;493:
;494:/*
;495:================
;496:respawn
;497:================
;498:*/
;499:void respawn( gentity_t *ent ) {
line 502
;500:	gentity_t	*tent;
;501:
;502:	CopyToBodyQue (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CopyToBodyQue
CALLV
pop
line 503
;503:	ClientSpawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 506
;504:
;505:	// add a teleportation effect
;506:	tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 42
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 507
;507:	tent->s.clientNum = ent->s.clientNum;
ADDRLP4 8
CNSTI4 168
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 508
;508:}
LABELV $177
endproc respawn 12 8
export TeamCount
proc TeamCount 8 0
line 517
;509:
;510:/*
;511:================
;512:TeamCount
;513:
;514:Returns number of players on a team
;515:================
;516:*/
;517:team_t TeamCount( int ignoreClientNum, int team ) {
line 519
;518:	int		i;
;519:	int		count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 521
;520:
;521:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $182
JUMPV
LABELV $179
line 522
;522:		if ( i == ignoreClientNum ) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $184
line 523
;523:			continue;
ADDRGP4 $180
JUMPV
LABELV $184
line 525
;524:		}
;525:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $186
line 526
;526:			continue;
ADDRGP4 $180
JUMPV
LABELV $186
line 528
;527:		}
;528:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $188
line 529
;529:			count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 530
;530:		}
LABELV $188
line 531
;531:	}
LABELV $180
line 521
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $182
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $179
line 533
;532:
;533:	return count;
ADDRLP4 4
INDIRI4
RETI4
LABELV $178
endproc TeamCount 8 0
export TeamLeader
proc TeamLeader 4 0
line 543
;534:}
;535:
;536:/*
;537:================
;538:TeamLeader
;539:
;540:Returns the client number of the team leader
;541:================
;542:*/
;543:int TeamLeader( int team ) {
line 546
;544:	int		i;
;545:
;546:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $194
JUMPV
LABELV $191
line 547
;547:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
NEI4 $196
line 548
;548:			continue;
ADDRGP4 $192
JUMPV
LABELV $196
line 550
;549:		}
;550:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $198
line 551
;551:			if ( level.clients[i].sess.teamLeader )
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 640
ADDP4
INDIRI4
CNSTI4 0
EQI4 $200
line 552
;552:				return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $190
JUMPV
LABELV $200
line 553
;553:		}
LABELV $198
line 554
;554:	}
LABELV $192
line 546
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $194
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $191
line 556
;555:
;556:	return -1;
CNSTI4 -1
RETI4
LABELV $190
endproc TeamLeader 4 0
export PickTeam
proc PickTeam 24 8
line 566
;557:}
;558:
;559:
;560:/*
;561:================
;562:PickTeam
;563:
;564:================
;565:*/
;566:team_t PickTeam( int ignoreClientNum ) {
line 569
;567:	int		counts[TEAM_NUM_TEAMS];
;568:
;569:	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 16
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+8
ADDRLP4 16
INDIRI4
ASGNI4
line 570
;570:	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 20
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 20
INDIRI4
ASGNI4
line 572
;571:
;572:	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
ADDRLP4 0+8
INDIRI4
ADDRLP4 0+4
INDIRI4
LEI4 $205
line 573
;573:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $202
JUMPV
LABELV $205
line 575
;574:	}
;575:	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
ADDRLP4 0+4
INDIRI4
ADDRLP4 0+8
INDIRI4
LEI4 $209
line 576
;576:		return TEAM_BLUE;
CNSTI4 2
RETI4
ADDRGP4 $202
JUMPV
LABELV $209
line 579
;577:	}
;578:	// equal team count, so join the team with the lowest score
;579:	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
ADDRGP4 level+44+8
INDIRI4
ADDRGP4 level+44+4
INDIRI4
LEI4 $213
line 580
;580:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $202
JUMPV
LABELV $213
line 582
;581:	}
;582:	return TEAM_BLUE;
CNSTI4 2
RETI4
LABELV $202
endproc PickTeam 24 8
proc ClientCleanName 40 12
line 610
;583:}
;584:
;585:/*
;586:===========
;587:ForceClientSkin
;588:
;589:Forces a client's skin (for teamplay)
;590:===========
;591:*/
;592:/*
;593:static void ForceClientSkin( gclient_t *client, char *model, const char *skin ) {
;594:	char *p;
;595:
;596:	if ((p = Q_strrchr(model, '/')) != 0) {
;597:		*p = 0;
;598:	}
;599:
;600:	Q_strcat(model, MAX_QPATH, "/");
;601:	Q_strcat(model, MAX_QPATH, skin);
;602:}
;603:*/
;604:
;605:/*
;606:===========
;607:ClientCheckName
;608:============
;609:*/
;610:static void ClientCleanName( const char *in, char *out, int outSize ) {
line 617
;611:	int		len, colorlessLen;
;612:	char	ch;
;613:	char	*p;
;614:	int		spaces;
;615:
;616:	//save room for trailing null byte
;617:	outSize--;
ADDRFP4 8
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 619
;618:
;619:	len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 620
;620:	colorlessLen = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 621
;621:	p = out;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
line 622
;622:	*p = 0;
ADDRLP4 12
INDIRP4
CNSTI1 0
ASGNI1
line 623
;623:	spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $221
JUMPV
LABELV $220
line 625
;624:
;625:	while( 1 ) {
line 626
;626:		ch = *in++;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 20
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
INDIRI1
ASGNI1
line 627
;627:		if( !ch ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $223
line 628
;628:			break;
ADDRGP4 $222
JUMPV
LABELV $223
line 632
;629:		}
;630:
;631:		// don't allow leading spaces
;632:		if( !*p && ch == ' ' ) {
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $225
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $225
line 633
;633:			continue;
ADDRGP4 $221
JUMPV
LABELV $225
line 637
;634:		}
;635:
;636:		// check colors
;637:		if( ch == Q_COLOR_ESCAPE ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 94
NEI4 $227
line 639
;638:			// solo trailing carat is not a color prefix
;639:			if( !*in ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $229
line 640
;640:				break;
ADDRGP4 $222
JUMPV
LABELV $229
line 644
;641:			}
;642:
;643:			// don't allow black in a name, period
;644:			if( ColorIndex(*in) == 0 ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 0
NEI4 $231
line 645
;645:				in++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 646
;646:				continue;
ADDRGP4 $221
JUMPV
LABELV $231
line 650
;647:			}
;648:
;649:			// make sure room in dest for both chars
;650:			if( len > outSize - 2 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 2
SUBI4
LEI4 $233
line 651
;651:				break;
ADDRGP4 $222
JUMPV
LABELV $233
line 654
;652:			}
;653:
;654:			*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 655
;655:			*out++ = *in++;
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 1
ASGNI4
ADDRFP4 4
ADDRLP4 28
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI1
ASGNI1
line 656
;656:			len += 2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 2
ADDI4
ASGNI4
line 657
;657:			continue;
ADDRGP4 $221
JUMPV
LABELV $227
line 661
;658:		}
;659:
;660:		// don't allow too many consecutive spaces
;661:		if( ch == ' ' ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $235
line 662
;662:			spaces++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 663
;663:			if( spaces > 3 ) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LEI4 $236
line 664
;664:				continue;
ADDRGP4 $221
JUMPV
line 666
;665:			}
;666:		}
LABELV $235
line 667
;667:		else {
line 668
;668:			spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 669
;669:		}
LABELV $236
line 671
;670:
;671:		if( len > outSize - 1 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
LEI4 $239
line 672
;672:			break;
ADDRGP4 $222
JUMPV
LABELV $239
line 675
;673:		}
;674:
;675:		*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 676
;676:		colorlessLen++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 677
;677:		len++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 678
;678:	}
LABELV $221
line 625
ADDRGP4 $220
JUMPV
LABELV $222
line 679
;679:	*out = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 682
;680:
;681:	// don't allow empty names
;682:	if( *p == 0 || colorlessLen == 0 ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 20
INDIRI4
EQI4 $243
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $241
LABELV $243
line 683
;683:		Q_strncpyz( p, "UnnamedPlayer", outSize );
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $244
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 684
;684:	}
LABELV $241
line 685
;685:}
LABELV $219
endproc ClientCleanName 40 12
export ClientUserinfoChanged
proc ClientUserinfoChanged 6388 56
line 699
;686:
;687:
;688:/*
;689:===========
;690:ClientUserInfoChanged
;691:
;692:Called from ClientConnect when the player first connects and
;693:directly by the server system when the player updates a userinfo variable.
;694:
;695:The game can override any of the settings and call trap_SetUserinfo
;696:if desired.
;697:============
;698:*/
;699:void ClientUserinfoChanged( int clientNum ) {
line 713
;700:	gentity_t *ent;
;701:	int		teamTask, teamLeader, team, health;
;702:	char	*s;
;703:	char	model[MAX_QPATH];
;704:	char	headModel[MAX_QPATH];
;705:	char	oldname[MAX_STRING_CHARS];
;706:	gclient_t	*client;
;707:	char	c1[MAX_INFO_STRING];
;708:	char	c2[MAX_INFO_STRING];
;709:	char	redTeam[MAX_INFO_STRING];
;710:	char	blueTeam[MAX_INFO_STRING];
;711:	char	userinfo[MAX_INFO_STRING];
;712:
;713:	ent = g_entities + clientNum;
ADDRLP4 1032
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 714
;714:	client = ent->client;
ADDRLP4 0
ADDRLP4 1032
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 716
;715:
;716:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 719
;717:
;718:	// check for malformed or illegal info strings
;719:	if ( !Info_Validate(userinfo) ) {
ADDRLP4 4
ARGP4
ADDRLP4 6300
ADDRGP4 Info_Validate
CALLI4
ASGNI4
ADDRLP4 6300
INDIRI4
CNSTI4 0
NEI4 $246
line 720
;720:		strcpy (userinfo, "\\name\\badinfo");
ADDRLP4 4
ARGP4
ADDRGP4 $248
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 721
;721:	}
LABELV $246
line 724
;722:
;723:	// check for local client
;724:	s = Info_ValueForKey( userinfo, "ip" );
ADDRLP4 4
ARGP4
ADDRGP4 $249
ARGP4
ADDRLP4 6304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6304
INDIRP4
ASGNP4
line 725
;725:	if ( !strcmp( s, "localhost" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $252
ARGP4
ADDRLP4 6308
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6308
INDIRI4
CNSTI4 0
NEI4 $250
line 726
;726:		client->pers.localClient = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 496
ADDP4
CNSTI4 1
ASGNI4
line 727
;727:	}
LABELV $250
line 730
;728:
;729:	// check the item prediction
;730:	s = Info_ValueForKey( userinfo, "cg_predictItems" );
ADDRLP4 4
ARGP4
ADDRGP4 $253
ARGP4
ADDRLP4 6312
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6312
INDIRP4
ASGNP4
line 731
;731:	if ( !atoi( s ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6316
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6316
INDIRI4
CNSTI4 0
NEI4 $254
line 732
;732:		client->pers.predictItemPickup = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 0
ASGNI4
line 733
;733:	} else {
ADDRGP4 $255
JUMPV
LABELV $254
line 734
;734:		client->pers.predictItemPickup = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 1
ASGNI4
line 735
;735:	}
LABELV $255
line 738
;736:
;737:	// set name
;738:	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 739
;739:	s = Info_ValueForKey (userinfo, "name");
ADDRLP4 4
ARGP4
ADDRGP4 $256
ARGP4
ADDRLP4 6320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6320
INDIRP4
ASGNP4
line 740
;740:	ClientCleanName( s, client->pers.netname, sizeof(client->pers.netname) );
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientCleanName
CALLV
pop
line 742
;741:
;742:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $257
line 743
;743:		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $259
line 744
;744:			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRGP4 $261
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 745
;745:		}
LABELV $259
line 746
;746:	}
LABELV $257
line 748
;747:
;748:	if ( client->pers.connected == CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $262
line 749
;749:		if ( strcmp( oldname, client->pers.netname ) ) {
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 6324
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6324
INDIRI4
CNSTI4 0
EQI4 $264
line 750
;750:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " renamed to %s\n\"", oldname, 
ADDRGP4 $266
ARGP4
ADDRLP4 1164
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 6328
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 6328
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 752
;751:				client->pers.netname) );
;752:		}
LABELV $264
line 753
;753:	}
LABELV $262
line 767
;754:
;755:	// set max health
;756:#ifdef MISSIONPACK
;757:	if (client->ps.powerups[PW_GUARD]) {
;758:		client->pers.maxHealth = 200;
;759:	} else {
;760:		health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
;761:		client->pers.maxHealth = health;
;762:		if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
;763:			client->pers.maxHealth = 100;
;764:		}
;765:	}
;766:#else
;767:	health = atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 6324
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 6324
INDIRP4
ARGP4
ADDRLP4 6328
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2196
ADDRLP4 6328
INDIRI4
ASGNI4
line 768
;768:	client->pers.maxHealth = health;
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 2196
INDIRI4
ASGNI4
line 769
;769:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 6332
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
ADDRLP4 6332
INDIRI4
CNSTI4 1
LTI4 $270
ADDRLP4 6332
INDIRI4
CNSTI4 100
LEI4 $268
LABELV $270
line 770
;770:		client->pers.maxHealth = 100;
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
CNSTI4 100
ASGNI4
line 771
;771:	}
LABELV $268
line 773
;772:#endif
;773:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
line 776
;774:
;775:	// set model
;776:	if( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $271
line 777
;777:		Q_strncpyz( model, Info_ValueForKey (userinfo, "team_model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $274
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6340
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 778
;778:		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "team_headmodel"), sizeof( headModel ) );
ADDRLP4 4
ARGP4
ADDRGP4 $275
ARGP4
ADDRLP4 6344
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1100
ARGP4
ADDRLP4 6344
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 779
;779:	} else {
ADDRGP4 $272
JUMPV
LABELV $271
line 780
;780:		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $276
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6340
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 781
;781:		Q_strncpyz( headModel, Info_ValueForKey (userinfo, "headmodel"), sizeof( headModel ) );
ADDRLP4 4
ARGP4
ADDRGP4 $277
ARGP4
ADDRLP4 6344
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1100
ARGP4
ADDRLP4 6344
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 782
;782:	}
LABELV $272
line 785
;783:
;784:	// bots set their team a few frames later
;785:	if (g_gametype.integer >= GT_TEAM && g_entities[clientNum].r.svFlags & SVF_BOT) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $278
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $278
line 786
;786:		s = Info_ValueForKey( userinfo, "team" );
ADDRLP4 4
ARGP4
ADDRGP4 $283
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6340
INDIRP4
ASGNP4
line 787
;787:		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 6344
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6344
INDIRI4
CNSTI4 0
EQI4 $288
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $287
ARGP4
ADDRLP4 6348
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6348
INDIRI4
CNSTI4 0
NEI4 $284
LABELV $288
line 788
;788:			team = TEAM_RED;
ADDRLP4 4248
CNSTI4 1
ASGNI4
line 789
;789:		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
ADDRGP4 $279
JUMPV
LABELV $284
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $291
ARGP4
ADDRLP4 6352
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6352
INDIRI4
CNSTI4 0
EQI4 $293
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $292
ARGP4
ADDRLP4 6356
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6356
INDIRI4
CNSTI4 0
NEI4 $289
LABELV $293
line 790
;790:			team = TEAM_BLUE;
ADDRLP4 4248
CNSTI4 2
ASGNI4
line 791
;791:		} else {
ADDRGP4 $279
JUMPV
LABELV $289
line 793
;792:			// pick the team with the least number of players
;793:			team = PickTeam( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 6360
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 4248
ADDRLP4 6360
INDIRI4
ASGNI4
line 794
;794:		}
line 795
;795:	}
ADDRGP4 $279
JUMPV
LABELV $278
line 796
;796:	else {
line 797
;797:		team = client->sess.sessionTeam;
ADDRLP4 4248
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ASGNI4
line 798
;798:	}
LABELV $279
line 834
;799:
;800:/*	NOTE: all client side now
;801:
;802:	// team
;803:	switch( team ) {
;804:	case TEAM_RED:
;805:		ForceClientSkin(client, model, "red");
;806://		ForceClientSkin(client, headModel, "red");
;807:		break;
;808:	case TEAM_BLUE:
;809:		ForceClientSkin(client, model, "blue");
;810://		ForceClientSkin(client, headModel, "blue");
;811:		break;
;812:	}
;813:	// don't ever use a default skin in teamplay, it would just waste memory
;814:	// however bots will always join a team but they spawn in as spectator
;815:	if ( g_gametype.integer >= GT_TEAM && team == TEAM_SPECTATOR) {
;816:		ForceClientSkin(client, model, "red");
;817://		ForceClientSkin(client, headModel, "red");
;818:	}
;819:*/
;820:
;821:#ifdef MISSIONPACK
;822:	if (g_gametype.integer >= GT_TEAM) {
;823:		client->pers.teamInfo = qtrue;
;824:	} else {
;825:		s = Info_ValueForKey( userinfo, "teamoverlay" );
;826:		if ( ! *s || atoi( s ) != 0 ) {
;827:			client->pers.teamInfo = qtrue;
;828:		} else {
;829:			client->pers.teamInfo = qfalse;
;830:		}
;831:	}
;832:#else
;833:	// teamInfo
;834:	s = Info_ValueForKey( userinfo, "teamoverlay" );
ADDRLP4 4
ARGP4
ADDRGP4 $294
ARGP4
ADDRLP4 6340
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6340
INDIRP4
ASGNP4
line 835
;835:	if ( ! *s || atoi( s ) != 0 ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $297
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6348
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6348
INDIRI4
CNSTI4 0
EQI4 $295
LABELV $297
line 836
;836:		client->pers.teamInfo = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 612
ADDP4
CNSTI4 1
ASGNI4
line 837
;837:	} else {
ADDRGP4 $296
JUMPV
LABELV $295
line 838
;838:		client->pers.teamInfo = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 612
ADDP4
CNSTI4 0
ASGNI4
line 839
;839:	}
LABELV $296
line 852
;840:#endif
;841:	/*
;842:	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
;843:	if ( !*s || atoi( s ) == 0 ) {
;844:		client->pers.pmoveFixed = qfalse;
;845:	}
;846:	else {
;847:		client->pers.pmoveFixed = qtrue;
;848:	}
;849:	*/
;850:
;851:	// team task (0 = none, 1 = offence, 2 = defence)
;852:	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
ADDRLP4 4
ARGP4
ADDRGP4 $298
ARGP4
ADDRLP4 6352
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 6352
INDIRP4
ARGP4
ADDRLP4 6356
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2188
ADDRLP4 6356
INDIRI4
ASGNI4
line 854
;853:	// team Leader (1 = leader, 0 is normal player)
;854:	teamLeader = client->sess.teamLeader;
ADDRLP4 2192
ADDRLP4 0
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
ASGNI4
line 857
;855:
;856:	// colors
;857:	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
ADDRLP4 4
ARGP4
ADDRGP4 $299
ARGP4
ADDRLP4 6360
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2200
ARGP4
ADDRLP4 6360
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 858
;858:	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));
ADDRLP4 4
ARGP4
ADDRGP4 $300
ARGP4
ADDRLP4 6364
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3224
ARGP4
ADDRLP4 6364
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 860
;859:
;860:	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $301
ARGP4
ADDRLP4 6368
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4252
ARGP4
ADDRLP4 6368
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 861
;861:	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $302
ARGP4
ADDRLP4 6372
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 5276
ARGP4
ADDRLP4 6372
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 865
;862:
;863:	// send over a subset of the userinfo keys so other clients can
;864:	// print scoreboards, display models, and play custom sounds
;865:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 1032
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $303
line 866
;866:		s = va("n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d",
ADDRLP4 4
ARGP4
ADDRGP4 $306
ARGP4
ADDRLP4 6376
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $305
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 4248
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
INDIRI4
ARGI4
ADDRLP4 6376
INDIRP4
ARGP4
ADDRLP4 2188
INDIRI4
ARGI4
ADDRLP4 2192
INDIRI4
ARGI4
ADDRLP4 6384
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6384
INDIRP4
ASGNP4
line 870
;867:			client->pers.netname, team, model, headModel, c1, c2, 
;868:			client->pers.maxHealth, client->sess.wins, client->sess.losses,
;869:			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader );
;870:	} else {
ADDRGP4 $304
JUMPV
LABELV $303
line 871
;871:		s = va("n\\%s\\t\\%i\\model\\%s\\hmodel\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d",
ADDRGP4 $307
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 4252
ARGP4
ADDRLP4 5276
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
INDIRI4
ARGI4
ADDRLP4 2188
INDIRI4
ARGI4
ADDRLP4 2192
INDIRI4
ARGI4
ADDRLP4 6380
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6380
INDIRP4
ASGNP4
line 874
;872:			client->pers.netname, client->sess.sessionTeam, model, headModel, redTeam, blueTeam, c1, c2, 
;873:			client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader);
;874:	}
LABELV $304
line 876
;875:
;876:	trap_SetConfigstring( CS_PLAYERS+clientNum, s );
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 879
;877:
;878:	// this is not the userinfo, more like the configstring actually
;879:	G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
ADDRGP4 $308
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 880
;880:}
LABELV $245
endproc ClientUserinfoChanged 6388 56
export ClientConnect
proc ClientConnect 1064 12
line 903
;881:
;882:
;883:/*
;884:===========
;885:ClientConnect
;886:
;887:Called when a player begins connecting to the server.
;888:Called again for every map change or tournement restart.
;889:
;890:The session information will be valid after exit.
;891:
;892:Return NULL if the client should be allowed, otherwise return
;893:a string with the reason for denial.
;894:
;895:Otherwise, the client will be sent the current gamestate
;896:and will eventually get to ClientBegin.
;897:
;898:firstTime will be qtrue the very first time a client connects
;899:to the server machine, but qfalse on map changes and tournement
;900:restarts.
;901:============
;902:*/
;903:char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
line 910
;904:	char		*value;
;905://	char		*areabits;
;906:	gclient_t	*client;
;907:	char		userinfo[MAX_INFO_STRING];
;908:	gentity_t	*ent;
;909:
;910:	ent = &g_entities[ clientNum ];
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 912
;911:
;912:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 918
;913:
;914: 	// IP filtering
;915: 	// https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=500
;916: 	// recommanding PB based IP / GUID banning, the builtin system is pretty limited
;917: 	// check to see if they are on the banned IP list
;918:	value = Info_ValueForKey (userinfo, "ip");
ADDRLP4 12
ARGP4
ADDRGP4 $249
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1036
INDIRP4
ASGNP4
line 919
;919:	if ( G_FilterPacket( value ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 G_FilterPacket
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $310
line 920
;920:		return "You are banned from this server.";
ADDRGP4 $312
RETP4
ADDRGP4 $309
JUMPV
LABELV $310
line 926
;921:	}
;922:
;923:  // we don't check password for bots and local client
;924:  // NOTE: local client <-> "ip" "localhost"
;925:  //   this means this client is not running in our current process
;926:	if ( !( ent->r.svFlags & SVF_BOT ) && (strcmp(value, "localhost") != 0)) {
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $313
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $252
ARGP4
ADDRLP4 1044
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $313
line 928
;927:		// check for a password
;928:		value = Info_ValueForKey (userinfo, "password");
ADDRLP4 12
ARGP4
ADDRGP4 $315
ARGP4
ADDRLP4 1048
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1048
INDIRP4
ASGNP4
line 929
;929:		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) &&
ADDRGP4 g_password+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $316
ADDRGP4 g_password+16
ARGP4
ADDRGP4 $320
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
EQI4 $316
ADDRGP4 g_password+16
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $316
line 930
;930:			strcmp( g_password.string, value) != 0) {
line 931
;931:			return "Invalid password";
ADDRGP4 $322
RETP4
ADDRGP4 $309
JUMPV
LABELV $316
line 933
;932:		}
;933:	}
LABELV $313
line 936
;934:
;935:	// they can connect
;936:	ent->client = level.clients + clientNum;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
CNSTI4 776
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 937
;937:	client = ent->client;
ADDRLP4 0
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 941
;938:
;939://	areabits = client->areabits;
;940:
;941:	memset( client, 0, sizeof(*client) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 776
ARGI4
ADDRGP4 memset
CALLP4
pop
line 943
;942:
;943:	client->pers.connected = CON_CONNECTING;
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 1
ASGNI4
line 946
;944:
;945:	// read or initialize the session data
;946:	if ( firstTime || level.newSession ) {
ADDRLP4 1048
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRI4
ADDRLP4 1048
INDIRI4
NEI4 $326
ADDRGP4 level+64
INDIRI4
ADDRLP4 1048
INDIRI4
EQI4 $323
LABELV $326
line 947
;947:		G_InitSessionData( client, userinfo );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 G_InitSessionData
CALLV
pop
line 948
;948:	}
LABELV $323
line 949
;949:	G_ReadSessionData( client );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_ReadSessionData
CALLV
pop
line 951
;950:
;951:	if( isBot ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $327
line 952
;952:		ent->r.svFlags |= SVF_BOT;
ADDRLP4 1052
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
ADDRLP4 1052
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 953
;953:		ent->inuse = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 954
;954:		if( !G_BotConnect( clientNum, !firstTime ) ) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $332
ADDRLP4 1056
CNSTI4 1
ASGNI4
ADDRGP4 $333
JUMPV
LABELV $332
ADDRLP4 1056
CNSTI4 0
ASGNI4
LABELV $333
ADDRLP4 1056
INDIRI4
ARGI4
ADDRLP4 1060
ADDRGP4 G_BotConnect
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $329
line 955
;955:			return "BotConnectfailed";
ADDRGP4 $334
RETP4
ADDRGP4 $309
JUMPV
LABELV $329
line 957
;956:		}
;957:	}
LABELV $327
line 960
;958:
;959:	// get and distribute relevent paramters
;960:	G_LogPrintf( "ClientConnect: %i\n", clientNum );
ADDRGP4 $335
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 961
;961:	ClientUserinfoChanged( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 964
;962:
;963:	// don't do the "xxx connected" messages if they were caried over from previous level
;964:	if ( firstTime ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $336
line 965
;965:		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname) );
ADDRGP4 $338
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 1052
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 966
;966:	}
LABELV $336
line 968
;967:
;968:	if ( g_gametype.integer >= GT_TEAM &&
ADDRLP4 1052
CNSTI4 3
ASGNI4
ADDRGP4 g_gametype+12
INDIRI4
ADDRLP4 1052
INDIRI4
LTI4 $339
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 1052
INDIRI4
EQI4 $339
line 969
;969:		client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 970
;970:		BroadcastTeamChange( client, -1 );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 BroadcastTeamChange
CALLV
pop
line 971
;971:	}
LABELV $339
line 974
;972:
;973:	// count current clients and rank for scoreboard
;974:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 981
;975:
;976:	// for statistics
;977://	client->areabits = areabits;
;978://	if ( !client->areabits )
;979://		client->areabits = G_Alloc( (trap_AAS_PointReachabilityAreaIndex( NULL ) + 7) / 8 );
;980:
;981:	return NULL;
CNSTP4 0
RETP4
LABELV $309
endproc ClientConnect 1064 12
export ClientBegin
proc ClientBegin 28 12
line 993
;982:}
;983:
;984:/*
;985:===========
;986:ClientBegin
;987:
;988:called when a client has finished connecting, and is ready
;989:to be placed into the level.  This will happen every level load,
;990:and on transition between teams, but doesn't happen on respawns
;991:============
;992:*/
;993:void ClientBegin( int clientNum ) {
line 999
;994:	gentity_t	*ent;
;995:	gclient_t	*client;
;996:	gentity_t	*tent;
;997:	int			flags;
;998:
;999:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1001
;1000:
;1001:	client = level.clients + clientNum;
ADDRLP4 0
CNSTI4 776
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1003
;1002:
;1003:	if ( ent->r.linked ) {
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
CNSTI4 0
EQI4 $343
line 1004
;1004:		trap_UnlinkEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 1005
;1005:	}
LABELV $343
line 1006
;1006:	G_InitGentity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 1007
;1007:	ent->touch = 0;
ADDRLP4 4
INDIRP4
CNSTI4 704
ADDP4
CNSTP4 0
ASGNP4
line 1008
;1008:	ent->pain = 0;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
CNSTP4 0
ASGNP4
line 1009
;1009:	ent->client = client;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1011
;1010:
;1011:	client->pers.connected = CON_CONNECTED;
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 2
ASGNI4
line 1012
;1012:	client->pers.enterTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1013
;1013:	client->pers.teamState.state = TEAM_BEGIN;
ADDRLP4 0
INDIRP4
CNSTI4 556
ADDP4
CNSTI4 0
ASGNI4
line 1020
;1014:
;1015:	// save eflags around this, because changing teams will
;1016:	// cause this to happen with a valid entity, and we
;1017:	// want to make sure the teleport bit is set right
;1018:	// so the viewpoint doesn't interpolate through the
;1019:	// world to the new position
;1020:	flags = client->ps.eFlags;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
ASGNI4
line 1021
;1021:	memset( &client->ps, 0, sizeof( client->ps ) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 468
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1022
;1022:	client->ps.eFlags = flags;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1025
;1023:
;1024:	// locate ent at a spawn point
;1025:	ClientSpawn( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 1027
;1026:
;1027:	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $346
line 1029
;1028:		// send event
;1029:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 42
ARGI4
ADDRLP4 16
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 16
INDIRP4
ASGNP4
line 1030
;1030:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 20
CNSTI4 168
ASGNI4
ADDRLP4 12
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1032
;1031:
;1032:		if ( g_gametype.integer != GT_TOURNAMENT  ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
EQI4 $348
line 1033
;1033:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " entered the game\n\"", client->pers.netname) );
ADDRGP4 $351
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1034
;1034:		}
LABELV $348
line 1035
;1035:	}
LABELV $346
line 1036
;1036:	G_LogPrintf( "ClientBegin: %i\n", clientNum );
ADDRGP4 $352
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1039
;1037:
;1038:	// count current clients and rank for scoreboard
;1039:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1040
;1040:}
LABELV $342
endproc ClientBegin 28 12
export ClientSpawn
proc ClientSpawn 1376 16
line 1051
;1041:
;1042:/*
;1043:===========
;1044:ClientSpawn
;1045:
;1046:Called every time a client is placed fresh in the world:
;1047:after the first ClientBegin, and after each respawn
;1048:Initializes all non-persistant parts of playerState
;1049:============
;1050:*/
;1051:void ClientSpawn(gentity_t *ent) {
line 1067
;1052:	int		index;
;1053:	vec3_t	spawn_origin, spawn_angles;
;1054:	gclient_t	*client;
;1055:	int		i;
;1056:	clientPersistant_t	saved;
;1057:	clientSession_t		savedSess;
;1058:	int		persistant[MAX_PERSISTANT];
;1059:	gentity_t	*spawnPoint;
;1060:	int		flags;
;1061:	int		savedPing;
;1062://	char	*savedAreaBits;
;1063:	int		accuracy_hits, accuracy_shots;
;1064:	int		eventSequence;
;1065:	char	userinfo[MAX_INFO_STRING];
;1066:
;1067:	index = ent - g_entities;
ADDRLP4 100
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ASGNI4
line 1068
;1068:	client = ent->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 1073
;1069:
;1070:	// find a spawn point
;1071:	// do it before setting health back up, so farthest
;1072:	// ranging doesn't count this client
;1073:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $354
line 1074
;1074:		spawnPoint = SelectSpectatorSpawnPoint ( 
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1324
ADDRGP4 SelectSpectatorSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1324
INDIRP4
ASGNP4
line 1076
;1075:						spawn_origin, spawn_angles);
;1076:	} else if (g_gametype.integer >= GT_CTF ) {
ADDRGP4 $355
JUMPV
LABELV $354
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
LTI4 $356
line 1078
;1077:		// all base oriented team games use the CTF spawn points
;1078:		spawnPoint = SelectCTFSpawnPoint ( 
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 556
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1328
ADDRGP4 SelectCTFSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1328
INDIRP4
ASGNP4
line 1082
;1079:						client->sess.sessionTeam, 
;1080:						client->pers.teamState.state, 
;1081:						spawn_origin, spawn_angles);
;1082:	} else {
ADDRGP4 $357
JUMPV
LABELV $356
LABELV $359
line 1083
;1083:		do {
line 1085
;1084:			// the first spawn should be at a good looking spot
;1085:			if ( !client->pers.initialSpawn && client->pers.localClient ) {
ADDRLP4 1328
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 500
ADDP4
INDIRI4
ADDRLP4 1328
INDIRI4
NEI4 $362
ADDRLP4 4
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 1328
INDIRI4
EQI4 $362
line 1086
;1086:				client->pers.initialSpawn = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 1087
;1087:				spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles );
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1332
ADDRGP4 SelectInitialSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1332
INDIRP4
ASGNP4
line 1088
;1088:			} else {
ADDRGP4 $363
JUMPV
LABELV $362
line 1090
;1089:				// don't spawn near existing origin if possible
;1090:				spawnPoint = SelectSpawnPoint ( 
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 88
ARGP4
ADDRLP4 1332
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 1332
INDIRP4
ASGNP4
line 1093
;1091:					client->ps.origin, 
;1092:					spawn_origin, spawn_angles);
;1093:			}
LABELV $363
line 1097
;1094:
;1095:			// Tim needs to prevent bots from spawning at the initial point
;1096:			// on q3dm0...
;1097:			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 1332
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
ADDRLP4 1332
INDIRI4
EQI4 $364
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 1332
INDIRI4
EQI4 $364
line 1098
;1098:				continue;	// try again
ADDRGP4 $360
JUMPV
LABELV $364
line 1101
;1099:			}
;1100:			// just to be symetric, we have a nohumans option...
;1101:			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 1336
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
ADDRLP4 1336
INDIRI4
EQI4 $361
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 1336
INDIRI4
NEI4 $361
line 1102
;1102:				continue;	// try again
line 1105
;1103:			}
;1104:
;1105:			break;
LABELV $360
line 1107
;1106:
;1107:		} while ( 1 );
ADDRGP4 $359
JUMPV
LABELV $361
line 1108
;1108:	}
LABELV $357
LABELV $355
line 1109
;1109:	client->pers.teamState.state = TEAM_ACTIVE;
ADDRLP4 4
INDIRP4
CNSTI4 556
ADDP4
CNSTI4 1
ASGNI4
line 1112
;1110:
;1111:	// always clear the kamikaze flag
;1112:	ent->s.eFlags &= ~EF_KAMIKAZE;
ADDRLP4 1324
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1324
INDIRP4
ADDRLP4 1324
INDIRP4
INDIRI4
CNSTI4 -513
BANDI4
ASGNI4
line 1116
;1113:
;1114:	// toggle the teleport bit so the client knows to not lerp
;1115:	// and never clear the voted flag
;1116:	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT | EF_VOTED | EF_TEAMVOTED);
ADDRLP4 104
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CNSTI4 540676
BANDI4
ASGNI4
line 1117
;1117:	flags ^= EF_TELEPORT_BIT;
ADDRLP4 104
ADDRLP4 104
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 1121
;1118:
;1119:	// clear everything but the persistant data
;1120:
;1121:	saved = client->pers;
ADDRLP4 1132
ADDRLP4 4
INDIRP4
CNSTI4 468
ADDP4
INDIRB
ASGNB 148
line 1122
;1122:	savedSess = client->sess;
ADDRLP4 1280
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRB
ASGNB 28
line 1123
;1123:	savedPing = client->ps.ping;
ADDRLP4 1308
ADDRLP4 4
INDIRP4
CNSTI4 452
ADDP4
INDIRI4
ASGNI4
line 1125
;1124://	savedAreaBits = client->areabits;
;1125:	accuracy_hits = client->accuracy_hits;
ADDRLP4 1312
ADDRLP4 4
INDIRP4
CNSTI4 716
ADDP4
INDIRI4
ASGNI4
line 1126
;1126:	accuracy_shots = client->accuracy_shots;
ADDRLP4 1316
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1127
;1127:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $368
line 1128
;1128:		persistant[i] = client->ps.persistant[i];
ADDRLP4 1328
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1328
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 1328
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1129
;1129:	}
LABELV $369
line 1127
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $368
line 1130
;1130:	eventSequence = client->ps.eventSequence;
ADDRLP4 1320
ADDRLP4 4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 1132
;1131:
;1132:	memset (client, 0, sizeof(*client)); // bk FIXME: Com_Memset?
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 776
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1134
;1133:
;1134:	client->pers = saved;
ADDRLP4 4
INDIRP4
CNSTI4 468
ADDP4
ADDRLP4 1132
INDIRB
ASGNB 148
line 1135
;1135:	client->sess = savedSess;
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
ADDRLP4 1280
INDIRB
ASGNB 28
line 1136
;1136:	client->ps.ping = savedPing;
ADDRLP4 4
INDIRP4
CNSTI4 452
ADDP4
ADDRLP4 1308
INDIRI4
ASGNI4
line 1138
;1137://	client->areabits = savedAreaBits;
;1138:	client->accuracy_hits = accuracy_hits;
ADDRLP4 4
INDIRP4
CNSTI4 716
ADDP4
ADDRLP4 1312
INDIRI4
ASGNI4
line 1139
;1139:	client->accuracy_shots = accuracy_shots;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
ADDRLP4 1316
INDIRI4
ASGNI4
line 1140
;1140:	client->lastkilled_client = -1;
ADDRLP4 4
INDIRP4
CNSTI4 720
ADDP4
CNSTI4 -1
ASGNI4
line 1142
;1141:
;1142:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $372
line 1143
;1143:		client->ps.persistant[i] = persistant[i];
ADDRLP4 1328
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1328
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
ADDP4
ADDRLP4 1328
INDIRI4
ADDRLP4 8
ADDP4
INDIRI4
ASGNI4
line 1144
;1144:	}
LABELV $373
line 1142
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $372
line 1145
;1145:	client->ps.eventSequence = eventSequence;
ADDRLP4 4
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 1320
INDIRI4
ASGNI4
line 1147
;1146:	// increment the spawncount so the client will detect the respawn
;1147:	client->ps.persistant[PERS_SPAWN_COUNT]++;
ADDRLP4 1328
ADDRLP4 4
INDIRP4
CNSTI4 264
ADDP4
ASGNP4
ADDRLP4 1328
INDIRP4
ADDRLP4 1328
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1148
;1148:	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;
ADDRLP4 4
INDIRP4
CNSTI4 260
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ASGNI4
line 1150
;1149:
;1150:	client->airOutTime = level.time + 12000;
ADDRLP4 4
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 1152
;1151:
;1152:	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );
ADDRLP4 100
INDIRI4
ARGI4
ADDRLP4 108
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1154
;1153:	// set max health
;1154:	client->pers.maxHealth = atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 108
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 1336
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1336
INDIRP4
ARGP4
ADDRLP4 1340
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 1340
INDIRI4
ASGNI4
line 1155
;1155:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 1344
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1344
INDIRI4
CNSTI4 1
LTI4 $379
ADDRLP4 1344
INDIRI4
CNSTI4 100
LEI4 $377
LABELV $379
line 1156
;1156:		client->pers.maxHealth = 100;
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
CNSTI4 100
ASGNI4
line 1157
;1157:	}
LABELV $377
line 1159
;1158:	// clear entity values
;1159:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 4
INDIRP4
CNSTI4 208
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
line 1160
;1160:	client->ps.eFlags = flags;
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 104
INDIRI4
ASGNI4
line 1162
;1161:
;1162:	ent->s.groundEntityNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1023
ASGNI4
line 1163
;1163:	ent->client = &level.clients[index];
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
CNSTI4 776
ADDRLP4 100
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1164
;1164:	ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 1165
;1165:	ent->inuse = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 1166
;1166:	ent->classname = "player";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $380
ASGNP4
line 1167
;1167:	ent->r.contents = CONTENTS_BODY;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 33554432
ASGNI4
line 1168
;1168:	ent->clipmask = MASK_PLAYERSOLID;
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 33619969
ASGNI4
line 1169
;1169:	ent->die = player_die;
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 player_die
ASGNP4
line 1170
;1170:	ent->waterlevel = 0;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
CNSTI4 0
ASGNI4
line 1171
;1171:	ent->watertype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 0
ASGNI4
line 1172
;1172:	ent->flags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
CNSTI4 0
ASGNI4
line 1174
;1173:	
;1174:	VectorCopy (playerMins, ent->r.mins);
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
ADDRGP4 playerMins
INDIRB
ASGNB 12
line 1175
;1175:	VectorCopy (playerMaxs, ent->r.maxs);
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
ADDRGP4 playerMaxs
INDIRB
ASGNB 12
line 1177
;1176:
;1177:	client->ps.clientNum = index;
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
ADDRLP4 100
INDIRI4
ASGNI4
line 1179
;1178:
;1179:	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_MACHINEGUN );
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 4
ASGNI4
line 1180
;1180:	if ( g_gametype.integer == GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $381
line 1181
;1181:		client->ps.ammo[WP_MACHINEGUN] = 50;
ADDRLP4 4
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 50
ASGNI4
line 1182
;1182:	} else {
ADDRGP4 $382
JUMPV
LABELV $381
line 1183
;1183:		client->ps.ammo[WP_MACHINEGUN] = 100;
ADDRLP4 4
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 100
ASGNI4
line 1184
;1184:	}
LABELV $382
line 1186
;1185:
;1186:	client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_GAUNTLET );
ADDRLP4 1352
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
ASGNP4
ADDRLP4 1352
INDIRP4
ADDRLP4 1352
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1187
;1187:	client->ps.ammo[WP_GAUNTLET] = -1;
ADDRLP4 4
INDIRP4
CNSTI4 380
ADDP4
CNSTI4 -1
ASGNI4
line 1188
;1188:	client->ps.ammo[WP_GRAPPLING_HOOK] = -1;
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
CNSTI4 -1
ASGNI4
line 1191
;1189:
;1190:	// health will count down towards max_health
;1191:	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] + 25;
ADDRLP4 1360
ADDRLP4 4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 1360
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 1360
INDIRI4
ASGNI4
line 1193
;1192:
;1193:	G_SetOrigin( ent, spawn_origin );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 76
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1194
;1194:	VectorCopy( spawn_origin, client->ps.origin );
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 76
INDIRB
ASGNB 12
line 1197
;1195:
;1196:	// the respawned flag will be cleared after the attack and jump keys come up
;1197:	client->ps.pm_flags |= PMF_RESPAWNED;
ADDRLP4 1364
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 1364
INDIRP4
ADDRLP4 1364
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 1199
;1198:
;1199:	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 776
DIVI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ARGP4
ADDRGP4 trap_GetUsercmd
CALLV
pop
line 1200
;1200:	SetClientViewAngle( ent, spawn_angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 88
ARGP4
ADDRGP4 SetClientViewAngle
CALLV
pop
line 1202
;1201:
;1202:	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $384
line 1204
;1203:
;1204:	} else {
ADDRGP4 $385
JUMPV
LABELV $384
line 1205
;1205:		G_KillBox( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_KillBox
CALLV
pop
line 1206
;1206:		trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1209
;1207:
;1208:		// force the base weapon up
;1209:		client->ps.weapon = WP_MACHINEGUN;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 2
ASGNI4
line 1210
;1210:		client->ps.weaponstate = WEAPON_READY;
ADDRLP4 4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 1212
;1211:
;1212:	}
LABELV $385
line 1215
;1213:
;1214:	// don't allow full run speed for a bit
;1215:	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 1368
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 1368
INDIRP4
ADDRLP4 1368
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1216
;1216:	client->ps.pm_time = 100;
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 100
ASGNI4
line 1218
;1217:
;1218:	client->respawnTime = level.time;
ADDRLP4 4
INDIRP4
CNSTI4 732
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1219
;1219:	client->inactivityTime = level.time + g_inactivity.integer * 1000;
ADDRLP4 4
INDIRP4
CNSTI4 736
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDRGP4 g_inactivity+12
INDIRI4
MULI4
ADDI4
ASGNI4
line 1220
;1220:	client->latched_buttons = 0;
ADDRLP4 4
INDIRP4
CNSTI4 664
ADDP4
CNSTI4 0
ASGNI4
line 1223
;1221:
;1222:	// set default animations
;1223:	client->ps.torsoAnim = TORSO_STAND;
ADDRLP4 4
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 11
ASGNI4
line 1224
;1224:	client->ps.legsAnim = LEGS_IDLE;
ADDRLP4 4
INDIRP4
CNSTI4 76
ADDP4
CNSTI4 22
ASGNI4
line 1226
;1225:
;1226:	if ( level.intermissiontime ) {
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $389
line 1227
;1227:		MoveClientToIntermission( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MoveClientToIntermission
CALLV
pop
line 1228
;1228:	} else {
ADDRGP4 $390
JUMPV
LABELV $389
line 1230
;1229:		// fire the targets of the spawn point
;1230:		G_UseTargets( spawnPoint, ent );
ADDRLP4 72
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 1234
;1231:
;1232:		// select the highest weapon number available, after any
;1233:		// spawn given items have fired
;1234:		client->ps.weapon = 1;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1
ASGNI4
line 1235
;1235:		for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
ADDRLP4 0
CNSTI4 10
ASGNI4
LABELV $392
line 1236
;1236:			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $396
line 1237
;1237:				client->ps.weapon = i;
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1238
;1238:				break;
ADDRGP4 $394
JUMPV
LABELV $396
line 1240
;1239:			}
;1240:		}
LABELV $393
line 1235
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $392
LABELV $394
line 1241
;1241:	}
LABELV $390
line 1245
;1242:
;1243:	// run a client frame to drop exactly to the floor,
;1244:	// initialize animations and other things
;1245:	client->ps.commandTime = level.time - 100;
ADDRLP4 4
INDIRP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
SUBI4
ASGNI4
line 1246
;1246:	ent->client->pers.cmd.serverTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1247
;1247:	ClientThink( ent-g_entities );
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ARGI4
ADDRGP4 ClientThink
CALLV
pop
line 1250
;1248:
;1249:	// positively link the client, even if the command times are weird
;1250:	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $400
line 1251
;1251:		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1252
;1252:		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
ADDRLP4 1372
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1372
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 1372
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1253
;1253:		trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1254
;1254:	}
LABELV $400
line 1257
;1255:
;1256:	// run the presend to set anything else
;1257:	ClientEndFrame( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientEndFrame
CALLV
pop
line 1260
;1258:
;1259:	// clear entity state values
;1260:	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1261
;1261:}
LABELV $353
endproc ClientSpawn 1376 16
export ClientDisconnect
proc ClientDisconnect 24 8
line 1276
;1262:
;1263:
;1264:/*
;1265:===========
;1266:ClientDisconnect
;1267:
;1268:Called when a player drops from the server.
;1269:Will not be called between levels.
;1270:
;1271:This should NOT be called directly by any game logic,
;1272:call trap_DropClient(), which will call this and do
;1273:server system housekeeping.
;1274:============
;1275:*/
;1276:void ClientDisconnect( int clientNum ) {
line 1283
;1277:	gentity_t	*ent;
;1278:	gentity_t	*tent;
;1279:	int			i;
;1280:
;1281:	// cleanup if we are kicking a bot that
;1282:	// hasn't spawned yet
;1283:	G_RemoveQueuedBotBegin( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_RemoveQueuedBotBegin
CALLV
pop
line 1285
;1284:
;1285:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1286
;1286:	if ( !ent->client ) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $403
line 1287
;1287:		return;
ADDRGP4 $402
JUMPV
LABELV $403
line 1291
;1288:	}
;1289:
;1290:	// stop any following clients
;1291:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $408
JUMPV
LABELV $405
line 1292
;1292:		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
ADDRLP4 12
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $410
ADDRLP4 12
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
NEI4 $410
ADDRLP4 12
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $410
line 1294
;1293:			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
;1294:			&& level.clients[i].sess.spectatorClient == clientNum ) {
line 1295
;1295:			StopFollowing( &g_entities[i] );
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 StopFollowing
CALLV
pop
line 1296
;1296:		}
LABELV $410
line 1297
;1297:	}
LABELV $406
line 1291
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $408
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $405
line 1300
;1298:
;1299:	// send effect if they were completely connected
;1300:	if ( ent->client->pers.connected == CON_CONNECTED 
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $412
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $412
line 1301
;1301:		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 1302
;1302:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 43
ARGI4
ADDRLP4 16
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 1303
;1303:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 20
CNSTI4 168
ASGNI4
ADDRLP4 8
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1307
;1304:
;1305:		// They don't get to take powerups with them!
;1306:		// Especially important for stuff like CTF flags
;1307:		TossClientItems( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 1315
;1308:#ifdef MISSIONPACK
;1309:		TossClientPersistantPowerups( ent );
;1310:		if( g_gametype.integer == GT_HARVESTER ) {
;1311:			TossClientCubes( ent );
;1312:		}
;1313:#endif
;1314:
;1315:	}
LABELV $412
line 1317
;1316:
;1317:	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );
ADDRGP4 $414
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1320
;1318:
;1319:	// if we are playing in tourney mode and losing, give a win to the other player
;1320:	if ( (g_gametype.integer == GT_TOURNAMENT )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
NEI4 $415
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 level+9128
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $415
ADDRGP4 level+16
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $415
ADDRGP4 level+84+4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $415
line 1322
;1321:		&& !level.intermissiontime
;1322:		&& !level.warmupTime && level.sortedClients[1] == clientNum ) {
line 1323
;1323:		level.clients[ level.sortedClients[0] ].sess.wins++;
ADDRLP4 20
CNSTI4 776
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 632
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1324
;1324:		ClientUserinfoChanged( level.sortedClients[0] );
ADDRGP4 level+84
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 1325
;1325:	}
LABELV $415
line 1327
;1326:
;1327:	trap_UnlinkEntity (ent);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 1328
;1328:	ent->s.modelindex = 0;
ADDRLP4 4
INDIRP4
CNSTI4 160
ADDP4
CNSTI4 0
ASGNI4
line 1329
;1329:	ent->inuse = qfalse;
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 1330
;1330:	ent->classname = "disconnected";
ADDRLP4 4
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $424
ASGNP4
line 1331
;1331:	ent->client->pers.connected = CON_DISCONNECTED;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 0
ASGNI4
line 1332
;1332:	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 260
ADDP4
CNSTI4 0
ASGNI4
line 1333
;1333:	ent->client->sess.sessionTeam = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
CNSTI4 0
ASGNI4
line 1335
;1334:
;1335:	trap_SetConfigstring( CS_PLAYERS + clientNum, "");
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRGP4 $425
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 1337
;1336:
;1337:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1339
;1338:
;1339:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $426
line 1340
;1340:		BotAIShutdownClient( clientNum, qfalse );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 BotAIShutdownClient
CALLI4
pop
line 1341
;1341:	}
LABELV $426
line 1342
;1342:}
LABELV $402
endproc ClientDisconnect 24 8
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_Seek
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_proxMineTimeout
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_cubeTimeout
import g_obeliskRespawnDelay
import g_obeliskRegenAmount
import g_obeliskRegenPeriod
import g_obeliskHealth
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlyFire
import g_capturelimit
import g_timelimit
import g_fraglimit
import g_dmflags
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotTestAAS
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import BotInterbreedEndMatch
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
import Svcmd_AbortPodium_f
import SpawnModelsOnVictoryPads
import UpdateTournamentInfo
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import CheckObeliskAttack
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import ClientCommand
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import FireWeapon
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
import CalculateRanks
import AddScore
import player_die
import InitClientResp
import InitClientPersistant
import BeginIntermission
import Weapon_HookThink
import Weapon_HookFree
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import fire_grapple
import fire_bfg
import fire_rocket
import fire_grenade
import fire_plasma
import fire_blaster
import G_RunMissile
import TossClientCubes
import TossClientItems
import body_die
import G_InvulnerabilityEffect
import G_RadiusDamage
import G_Damage
import CanDamage
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vectoyaw
import vtos
import tv
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_Sound
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_Find
import G_KillBox
import G_TeamCommand
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import BG_PlayerTouchesItem
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $425
byte 1 0
align 1
LABELV $424
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $414
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 68
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $380
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $352
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 66
byte 1 101
byte 1 103
byte 1 105
byte 1 110
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $351
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $338
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $335
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $334
byte 1 66
byte 1 111
byte 1 116
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 102
byte 1 97
byte 1 105
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $322
byte 1 73
byte 1 110
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 32
byte 1 112
byte 1 97
byte 1 115
byte 1 115
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $320
byte 1 110
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $315
byte 1 112
byte 1 97
byte 1 115
byte 1 115
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $312
byte 1 89
byte 1 111
byte 1 117
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 32
byte 1 98
byte 1 97
byte 1 110
byte 1 110
byte 1 101
byte 1 100
byte 1 32
byte 1 102
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 105
byte 1 115
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 46
byte 1 0
align 1
LABELV $308
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 85
byte 1 115
byte 1 101
byte 1 114
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 67
byte 1 104
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $307
byte 1 110
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 103
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 49
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 50
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 99
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 119
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 108
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 116
byte 1 116
byte 1 92
byte 1 37
byte 1 100
byte 1 92
byte 1 116
byte 1 108
byte 1 92
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $306
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $305
byte 1 110
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 49
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 50
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 104
byte 1 99
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 119
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 108
byte 1 92
byte 1 37
byte 1 105
byte 1 92
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 116
byte 1 116
byte 1 92
byte 1 37
byte 1 100
byte 1 92
byte 1 116
byte 1 108
byte 1 92
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $302
byte 1 103
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $301
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $300
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $299
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $298
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $294
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $292
byte 1 98
byte 1 0
align 1
LABELV $291
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $287
byte 1 114
byte 1 0
align 1
LABELV $286
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $283
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $277
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $276
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $275
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $274
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $267
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $266
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 94
byte 1 55
byte 1 32
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $261
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 98
byte 1 111
byte 1 97
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $256
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $253
byte 1 99
byte 1 103
byte 1 95
byte 1 112
byte 1 114
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 116
byte 1 73
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 0
align 1
LABELV $252
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $249
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $248
byte 1 92
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 92
byte 1 98
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 0
align 1
LABELV $244
byte 1 85
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $144
byte 1 98
byte 1 111
byte 1 100
byte 1 121
byte 1 113
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 67
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 32
byte 1 97
byte 1 32
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 32
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $61
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $57
byte 1 110
byte 1 111
byte 1 104
byte 1 117
byte 1 109
byte 1 97
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $54
byte 1 48
byte 1 0
align 1
LABELV $53
byte 1 110
byte 1 111
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
