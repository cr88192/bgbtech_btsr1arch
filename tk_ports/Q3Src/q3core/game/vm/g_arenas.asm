export UpdateTournamentInfo
code
proc UpdateTournamentInfo 1104 44
file "../g_arenas.c"
line 40
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
;23://
;24:// g_arenas.c
;25://
;26:
;27:#include "g_local.h"
;28:
;29:
;30:gentity_t	*podium1;
;31:gentity_t	*podium2;
;32:gentity_t	*podium3;
;33:
;34:
;35:/*
;36:==================
;37:UpdateTournamentInfo
;38:==================
;39:*/
;40:void UpdateTournamentInfo( void ) {
line 54
;41:	int			i;
;42:	gentity_t	*player;
;43:	int			playerClientNum;
;44:	int			n, accuracy, perfect,	msglen;
;45:	int			buflen;
;46:#ifdef MISSIONPACK // bk001205
;47:  int score1, score2;
;48:	qboolean won;
;49:#endif
;50:	char		buf[32];
;51:	char		msg[MAX_STRING_CHARS];
;52:
;53:	// find the real player
;54:	player = NULL;
ADDRLP4 40
CNSTP4 0
ASGNP4
line 55
;55:	for (i = 0; i < level.maxclients; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $56
JUMPV
LABELV $53
line 56
;56:		player = &g_entities[i];
ADDRLP4 40
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 57
;57:		if ( !player->inuse ) {
ADDRLP4 40
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $58
line 58
;58:			continue;
ADDRGP4 $54
JUMPV
LABELV $58
line 60
;59:		}
;60:		if ( !( player->r.svFlags & SVF_BOT ) ) {
ADDRLP4 40
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $60
line 61
;61:			break;
ADDRGP4 $55
JUMPV
LABELV $60
line 63
;62:		}
;63:	}
LABELV $54
line 55
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $56
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $53
LABELV $55
line 65
;64:	// this should never happen!
;65:	if ( !player || i == level.maxclients ) {
ADDRLP4 40
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $65
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
NEI4 $62
LABELV $65
line 66
;66:		return;
ADDRGP4 $52
JUMPV
LABELV $62
line 68
;67:	}
;68:	playerClientNum = i;
ADDRLP4 1076
ADDRLP4 0
INDIRI4
ASGNI4
line 70
;69:
;70:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 72
;71:
;72:	if ( level.clients[playerClientNum].sess.sessionTeam == TEAM_SPECTATOR ) {
CNSTI4 776
ADDRLP4 1076
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $66
line 76
;73:#ifdef MISSIONPACK
;74:		Com_sprintf( msg, sizeof(msg), "postgame %i %i 0 0 0 0 0 0 0 0 0 0 0", level.numNonSpectatorClients, playerClientNum );
;75:#else
;76:		Com_sprintf( msg, sizeof(msg), "postgame %i %i 0 0 0 0 0 0", level.numNonSpectatorClients, playerClientNum );
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $68
ARGP4
ADDRGP4 level+76
INDIRI4
ARGI4
ADDRLP4 1076
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 78
;77:#endif
;78:	}
ADDRGP4 $67
JUMPV
LABELV $66
line 79
;79:	else {
line 80
;80:		if( player->client->accuracy_shots ) {
ADDRLP4 40
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $70
line 81
;81:			accuracy = player->client->accuracy_hits * 100 / player->client->accuracy_shots;
ADDRLP4 1088
ADDRLP4 40
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1080
CNSTI4 100
ADDRLP4 1088
INDIRP4
CNSTI4 716
ADDP4
INDIRI4
MULI4
ADDRLP4 1088
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
DIVI4
ASGNI4
line 82
;82:		}
ADDRGP4 $71
JUMPV
LABELV $70
line 83
;83:		else {
line 84
;84:			accuracy = 0;
ADDRLP4 1080
CNSTI4 0
ASGNI4
line 85
;85:		}
LABELV $71
line 117
;86:#ifdef MISSIONPACK
;87:		won = qfalse;
;88:		if (g_gametype.integer >= GT_CTF) {
;89:			score1 = level.teamScores[TEAM_RED];
;90:			score2 = level.teamScores[TEAM_BLUE];
;91:			if (level.clients[playerClientNum].sess.sessionTeam	== TEAM_RED) {
;92:				won = (level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE]);
;93:			} else {
;94:				won = (level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED]);
;95:			}
;96:		} else {
;97:			if (&level.clients[playerClientNum] == &level.clients[ level.sortedClients[0] ]) {
;98:				won = qtrue;
;99:				score1 = level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE];
;100:				score2 = level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE];
;101:			} else {
;102:				score2 = level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE];
;103:				score1 = level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE];
;104:			}
;105:		}
;106:		if (won && player->client->ps.persistant[PERS_KILLED] == 0) {
;107:			perfect = 1;
;108:		} else {
;109:			perfect = 0;
;110:		}
;111:		Com_sprintf( msg, sizeof(msg), "postgame %i %i %i %i %i %i %i %i %i %i %i %i %i %i", level.numNonSpectatorClients, playerClientNum, accuracy,
;112:			player->client->ps.persistant[PERS_IMPRESSIVE_COUNT], player->client->ps.persistant[PERS_EXCELLENT_COUNT],player->client->ps.persistant[PERS_DEFEND_COUNT],
;113:			player->client->ps.persistant[PERS_ASSIST_COUNT], player->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT], player->client->ps.persistant[PERS_SCORE],
;114:			perfect, score1, score2, level.time, player->client->ps.persistant[PERS_CAPTURES] );
;115:
;116:#else
;117:		perfect = ( level.clients[playerClientNum].ps.persistant[PERS_RANK] == 0 && player->client->ps.persistant[PERS_KILLED] == 0 ) ? 1 : 0;
ADDRLP4 1092
CNSTI4 0
ASGNI4
CNSTI4 776
ADDRLP4 1076
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 256
ADDP4
INDIRI4
ADDRLP4 1092
INDIRI4
NEI4 $73
ADDRLP4 40
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ADDRLP4 1092
INDIRI4
NEI4 $73
ADDRLP4 1088
CNSTI4 1
ASGNI4
ADDRGP4 $74
JUMPV
LABELV $73
ADDRLP4 1088
CNSTI4 0
ASGNI4
LABELV $74
ADDRLP4 1084
ADDRLP4 1088
INDIRI4
ASGNI4
line 118
;118:		Com_sprintf( msg, sizeof(msg), "postgame %i %i %i %i %i %i %i %i", level.numNonSpectatorClients, playerClientNum, accuracy,
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $75
ARGP4
ADDRGP4 level+76
INDIRI4
ARGI4
ADDRLP4 1076
INDIRI4
ARGI4
ADDRLP4 1080
INDIRI4
ARGI4
ADDRLP4 1096
ADDRLP4 40
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1096
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
ARGI4
ADDRLP4 1096
INDIRP4
CNSTI4 288
ADDP4
INDIRI4
ARGI4
ADDRLP4 1096
INDIRP4
CNSTI4 300
ADDP4
INDIRI4
ARGI4
ADDRLP4 1096
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ARGI4
ADDRLP4 1084
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 123
;119:			player->client->ps.persistant[PERS_IMPRESSIVE_COUNT], player->client->ps.persistant[PERS_EXCELLENT_COUNT],
;120:			player->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT], player->client->ps.persistant[PERS_SCORE],
;121:			perfect );
;122:#endif
;123:	}
LABELV $67
line 125
;124:
;125:	msglen = strlen( msg );
ADDRLP4 44
ARGP4
ADDRLP4 1088
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1072
ADDRLP4 1088
INDIRI4
ASGNI4
line 126
;126:	for( i = 0; i < level.numNonSpectatorClients; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $80
JUMPV
LABELV $77
line 127
;127:		n = level.sortedClients[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+84
ADDP4
INDIRI4
ASGNI4
line 128
;128:		Com_sprintf( buf, sizeof(buf), " %i %i %i", n, level.clients[n].ps.persistant[PERS_RANK], level.clients[n].ps.persistant[PERS_SCORE] );
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $83
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 1096
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
ADDRLP4 1096
INDIRP4
CNSTI4 256
ADDP4
INDIRI4
ARGI4
ADDRLP4 1096
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 129
;129:		buflen = strlen( buf );
ADDRLP4 8
ARGP4
ADDRLP4 1100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1068
ADDRLP4 1100
INDIRI4
ASGNI4
line 130
;130:		if( msglen + buflen + 1 >= sizeof(msg) ) {
ADDRLP4 1072
INDIRI4
ADDRLP4 1068
INDIRI4
ADDI4
CNSTI4 1
ADDI4
CVIU4 4
CNSTU4 1024
LTU4 $84
line 131
;131:			break;
ADDRGP4 $79
JUMPV
LABELV $84
line 133
;132:		}
;133:		strcat( msg, buf );
ADDRLP4 44
ARGP4
ADDRLP4 8
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 134
;134:	}
LABELV $78
line 126
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $80
ADDRLP4 0
INDIRI4
ADDRGP4 level+76
INDIRI4
LTI4 $77
LABELV $79
line 135
;135:	trap_SendConsoleCommand( EXEC_APPEND, msg );
CNSTI4 2
ARGI4
ADDRLP4 44
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 136
;136:}
LABELV $52
endproc UpdateTournamentInfo 1104 44
proc SpawnModelOnVictoryPad 108 16
line 139
;137:
;138:
;139:static gentity_t *SpawnModelOnVictoryPad( gentity_t *pad, vec3_t offset, gentity_t *ent, int place ) {
line 144
;140:	gentity_t	*body;
;141:	vec3_t		vec;
;142:	vec3_t		f, r, u;
;143:
;144:	body = G_Spawn();
ADDRLP4 52
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 52
INDIRP4
ASGNP4
line 145
;145:	if ( !body ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $87
line 146
;146:		G_Printf( S_COLOR_RED "ERROR: out of gentities\n" );
ADDRGP4 $89
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 147
;147:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $86
JUMPV
LABELV $87
line 150
;148:	}
;149:
;150:	body->classname = ent->client->pers.netname;
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ASGNP4
line 151
;151:	body->client = ent->client;
ADDRLP4 56
CNSTI4 516
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRP4
ASGNP4
line 152
;152:	body->s = ent->s;
ADDRLP4 0
INDIRP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 208
line 153
;153:	body->s.eType = ET_PLAYER;		// could be ET_INVISIBLE
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 1
ASGNI4
line 154
;154:	body->s.eFlags = 0;				// clear EF_TALK, etc
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 155
;155:	body->s.powerups = 0;			// clear powerups
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 156
;156:	body->s.loopSound = 0;			// clear lava burning
ADDRLP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 0
ASGNI4
line 157
;157:	body->s.number = body - g_entities;
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
line 158
;158:	body->timestamp = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 159
;159:	body->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTI4 1
ASGNI4
line 160
;160:	body->physicsBounce = 0;		// don't bounce
ADDRLP4 0
INDIRP4
CNSTI4 568
ADDP4
CNSTF4 0
ASGNF4
line 161
;161:	body->s.event = 0;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
CNSTI4 0
ASGNI4
line 162
;162:	body->s.pos.trType = TR_STATIONARY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 163
;163:	body->s.groundEntityNum = ENTITYNUM_WORLD;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1022
ASGNI4
line 164
;164:	body->s.legsAnim = LEGS_IDLE;
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
CNSTI4 22
ASGNI4
line 165
;165:	body->s.torsoAnim = TORSO_STAND;
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 11
ASGNI4
line 166
;166:	if( body->s.weapon == WP_NONE ) {
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 0
NEI4 $91
line 167
;167:		body->s.weapon = WP_MACHINEGUN;
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 2
ASGNI4
line 168
;168:	}
LABELV $91
line 169
;169:	if( body->s.weapon == WP_GAUNTLET) {
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
NEI4 $93
line 170
;170:		body->s.torsoAnim = TORSO_STAND2;
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 12
ASGNI4
line 171
;171:	}
LABELV $93
line 172
;172:	body->s.event = 0;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
CNSTI4 0
ASGNI4
line 173
;173:	body->r.svFlags = ent->r.svFlags;
ADDRLP4 64
CNSTI4 424
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 174
;174:	VectorCopy (ent->r.mins, body->r.mins);
ADDRLP4 68
CNSTI4 436
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 68
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 68
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 175
;175:	VectorCopy (ent->r.maxs, body->r.maxs);
ADDRLP4 72
CNSTI4 448
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 176
;176:	VectorCopy (ent->r.absmin, body->r.absmin);
ADDRLP4 76
CNSTI4 464
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 177
;177:	VectorCopy (ent->r.absmax, body->r.absmax);
ADDRLP4 80
CNSTI4 476
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 178
;178:	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 65537
ASGNI4
line 179
;179:	body->r.contents = CONTENTS_BODY;
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 33554432
ASGNI4
line 180
;180:	body->r.ownerNum = ent->r.ownerNum;
ADDRLP4 84
CNSTI4 512
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 181
;181:	body->takedamage = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 183
;182:
;183:	VectorSubtract( level.intermission_origin, pad->r.currentOrigin, vec );
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRGP4 level+9144
INDIRF4
ADDRLP4 88
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 88
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRGP4 level+9144+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
SUBF4
ASGNF4
line 184
;184:	vectoangles( vec, body->s.apos.trBase );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 185
;185:	body->s.apos.trBase[PITCH] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 186
;186:	body->s.apos.trBase[ROLL] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
CNSTF4 0
ASGNF4
line 188
;187:
;188:	AngleVectors( body->s.apos.trBase, f, r, u );
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 40
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 189
;189:	VectorMA( pad->r.currentOrigin, offset[0], f, vec );
ADDRLP4 92
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 96
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 92
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 16
INDIRF4
ADDRLP4 96
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 92
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 16+4
INDIRF4
ADDRLP4 96
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 16+8
INDIRF4
ADDRFP4 4
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 190
;190:	VectorMA( vec, offset[1], r, vec );
ADDRLP4 100
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 28
INDIRF4
ADDRLP4 100
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
ADDRLP4 28+4
INDIRF4
ADDRLP4 100
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
ADDRLP4 28+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 191
;191:	VectorMA( vec, offset[2], u, vec );
ADDRLP4 104
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 40
INDIRF4
ADDRLP4 104
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
ADDRLP4 40+4
INDIRF4
ADDRLP4 104
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
ADDRLP4 40+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 193
;192:
;193:	G_SetOrigin( body, vec );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 195
;194:
;195:	trap_LinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 197
;196:
;197:	body->count = place;
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
ADDRFP4 12
INDIRI4
ASGNI4
line 199
;198:
;199:	return body;
ADDRLP4 0
INDIRP4
RETP4
LABELV $86
endproc SpawnModelOnVictoryPad 108 16
proc CelebrateStop 12 0
line 203
;200:}
;201:
;202:
;203:static void CelebrateStop( gentity_t *player ) {
line 206
;204:	int		anim;
;205:
;206:	if( player->s.weapon == WP_GAUNTLET) {
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
NEI4 $119
line 207
;207:		anim = TORSO_STAND2;
ADDRLP4 0
CNSTI4 12
ASGNI4
line 208
;208:	}
ADDRGP4 $120
JUMPV
LABELV $119
line 209
;209:	else {
line 210
;210:		anim = TORSO_STAND;
ADDRLP4 0
CNSTI4 11
ASGNI4
line 211
;211:	}
LABELV $120
line 212
;212:	player->s.torsoAnim = ( ( player->s.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ASGNP4
ADDRLP4 8
CNSTI4 128
ASGNI4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
BANDI4
ADDRLP4 8
INDIRI4
BXORI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 213
;213:}
LABELV $118
endproc CelebrateStop 12 0
proc CelebrateStart 8 12
line 217
;214:
;215:
;216:#define	TIMER_GESTURE	(34*66+50)
;217:static void CelebrateStart( gentity_t *player ) {
line 218
;218:	player->s.torsoAnim = ( ( player->s.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | TORSO_GESTURE;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ASGNP4
ADDRLP4 4
CNSTI4 128
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
BANDI4
ADDRLP4 4
INDIRI4
BXORI4
CNSTI4 6
BORI4
ASGNI4
line 219
;219:	player->nextthink = level.time + TIMER_GESTURE;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2294
ADDI4
ASGNI4
line 220
;220:	player->think = CelebrateStop;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 CelebrateStop
ASGNP4
line 227
;221:
;222:	/*
;223:	player->client->ps.events[player->client->ps.eventSequence & (MAX_PS_EVENTS-1)] = EV_TAUNT;
;224:	player->client->ps.eventParms[player->client->ps.eventSequence & (MAX_PS_EVENTS-1)] = 0;
;225:	player->client->ps.eventSequence++;
;226:	*/
;227:	G_AddEvent(player, EV_TAUNT, 0);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 76
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 228
;228:}
LABELV $121
endproc CelebrateStart 8 12
data
align 4
LABELV offsetFirst
byte 4 0
byte 4 0
byte 4 1116995584
align 4
LABELV offsetSecond
byte 4 3240099840
byte 4 1114636288
byte 4 1113063424
align 4
LABELV offsetThird
byte 4 3247964160
byte 4 3262119936
byte 4 1110704128
code
proc PodiumPlacementThink 92 16
line 235
;229:
;230:
;231:static vec3_t	offsetFirst  = {0, 0, 74};
;232:static vec3_t	offsetSecond = {-10, 60, 54};
;233:static vec3_t	offsetThird  = {-19, -60, 45};
;234:
;235:static void PodiumPlacementThink( gentity_t *podium ) {
line 240
;236:	vec3_t		vec;
;237:	vec3_t		origin;
;238:	vec3_t		f, r, u;
;239:
;240:	podium->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 242
;241:
;242:	AngleVectors( level.intermission_angle, vec, NULL, NULL );
ADDRGP4 level+9156
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 60
CNSTP4 0
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 243
;243:	VectorMA( level.intermission_origin, trap_Cvar_VariableIntegerValue( "g_podiumDist" ), vec, origin );
ADDRGP4 $127
ARGP4
ADDRLP4 64
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 48
ADDRGP4 level+9144
INDIRF4
ADDRLP4 0
INDIRF4
ADDRLP4 64
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRGP4 $127
ARGP4
ADDRLP4 68
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 48+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 68
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRGP4 $127
ARGP4
ADDRLP4 72
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 48+8
ADDRGP4 level+9144+8
INDIRF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 72
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
line 244
;244:	origin[2] -= trap_Cvar_VariableIntegerValue( "g_podiumDrop" );
ADDRGP4 $137
ARGP4
ADDRLP4 76
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 48+8
ADDRLP4 48+8
INDIRF4
ADDRLP4 76
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 245
;245:	G_SetOrigin( podium, origin );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 247
;246:
;247:	if( podium1 ) {
ADDRGP4 podium1
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $138
line 248
;248:		VectorSubtract( level.intermission_origin, podium->r.currentOrigin, vec );
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRGP4 level+9144
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 level+9144+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
SUBF4
ASGNF4
line 249
;249:		vectoangles( vec, podium1->s.apos.trBase );
ADDRLP4 0
ARGP4
ADDRGP4 podium1
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 250
;250:		podium1->s.apos.trBase[PITCH] = 0;
ADDRGP4 podium1
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 251
;251:		podium1->s.apos.trBase[ROLL] = 0;
ADDRGP4 podium1
INDIRP4
CNSTI4 68
ADDP4
CNSTF4 0
ASGNF4
line 253
;252:
;253:		AngleVectors( podium1->s.apos.trBase, f, r, u );
ADDRGP4 podium1
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 36
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 254
;254:		VectorMA( podium->r.currentOrigin, offsetFirst[0], f, vec );
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
ADDRGP4 offsetFirst
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 84
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 84
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDRGP4 offsetFirst
INDIRF4
MULF4
ADDF4
ASGNF4
line 255
;255:		VectorMA( vec, offsetFirst[1], r, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRF4
ADDRGP4 offsetFirst+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24+4
INDIRF4
ADDRGP4 offsetFirst+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 24+8
INDIRF4
ADDRGP4 offsetFirst+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 256
;256:		VectorMA( vec, offsetFirst[2], u, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
ADDRGP4 offsetFirst+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
ADDRGP4 offsetFirst+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
ADDRGP4 offsetFirst+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 258
;257:
;258:		G_SetOrigin( podium1, vec );
ADDRGP4 podium1
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 259
;259:	}
LABELV $138
line 261
;260:
;261:	if( podium2 ) {
ADDRGP4 podium2
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $169
line 262
;262:		VectorSubtract( level.intermission_origin, podium->r.currentOrigin, vec );
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRGP4 level+9144
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 level+9144+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
SUBF4
ASGNF4
line 263
;263:		vectoangles( vec, podium2->s.apos.trBase );
ADDRLP4 0
ARGP4
ADDRGP4 podium2
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 264
;264:		podium2->s.apos.trBase[PITCH] = 0;
ADDRGP4 podium2
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 265
;265:		podium2->s.apos.trBase[ROLL] = 0;
ADDRGP4 podium2
INDIRP4
CNSTI4 68
ADDP4
CNSTF4 0
ASGNF4
line 267
;266:
;267:		AngleVectors( podium2->s.apos.trBase, f, r, u );
ADDRGP4 podium2
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 36
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 268
;268:		VectorMA( podium->r.currentOrigin, offsetSecond[0], f, vec );
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
ADDRGP4 offsetSecond
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 84
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 84
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDRGP4 offsetSecond
INDIRF4
MULF4
ADDF4
ASGNF4
line 269
;269:		VectorMA( vec, offsetSecond[1], r, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRF4
ADDRGP4 offsetSecond+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24+4
INDIRF4
ADDRGP4 offsetSecond+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 24+8
INDIRF4
ADDRGP4 offsetSecond+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 270
;270:		VectorMA( vec, offsetSecond[2], u, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
ADDRGP4 offsetSecond+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
ADDRGP4 offsetSecond+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
ADDRGP4 offsetSecond+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 272
;271:
;272:		G_SetOrigin( podium2, vec );
ADDRGP4 podium2
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 273
;273:	}
LABELV $169
line 275
;274:
;275:	if( podium3 ) {
ADDRGP4 podium3
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $200
line 276
;276:		VectorSubtract( level.intermission_origin, podium->r.currentOrigin, vec );
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRGP4 level+9144
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 level+9144+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
SUBF4
ASGNF4
line 277
;277:		vectoangles( vec, podium3->s.apos.trBase );
ADDRLP4 0
ARGP4
ADDRGP4 podium3
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 278
;278:		podium3->s.apos.trBase[PITCH] = 0;
ADDRGP4 podium3
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 279
;279:		podium3->s.apos.trBase[ROLL] = 0;
ADDRGP4 podium3
INDIRP4
CNSTI4 68
ADDP4
CNSTF4 0
ASGNF4
line 281
;280:
;281:		AngleVectors( podium3->s.apos.trBase, f, r, u );
ADDRGP4 podium3
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 36
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 282
;282:		VectorMA( podium->r.currentOrigin, offsetThird[0], f, vec );
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
ADDRGP4 offsetThird
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 84
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 84
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 88
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDRGP4 offsetThird
INDIRF4
MULF4
ADDF4
ASGNF4
line 283
;283:		VectorMA( vec, offsetThird[1], r, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRF4
ADDRGP4 offsetThird+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24+4
INDIRF4
ADDRGP4 offsetThird+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 24+8
INDIRF4
ADDRGP4 offsetThird+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 284
;284:		VectorMA( vec, offsetThird[2], u, vec );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
ADDRGP4 offsetThird+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
ADDRGP4 offsetThird+8
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
ADDRGP4 offsetThird+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 286
;285:
;286:		G_SetOrigin( podium3, vec );
ADDRGP4 podium3
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 287
;287:	}
LABELV $200
line 288
;288:}
LABELV $123
endproc PodiumPlacementThink 92 16
proc SpawnPodium 68 16
line 291
;289:
;290:
;291:static gentity_t *SpawnPodium( void ) {
line 296
;292:	gentity_t	*podium;
;293:	vec3_t		vec;
;294:	vec3_t		origin;
;295:
;296:	podium = G_Spawn();
ADDRLP4 28
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
line 297
;297:	if ( !podium ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $232
line 298
;298:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $231
JUMPV
LABELV $232
line 301
;299:	}
;300:
;301:	podium->classname = "podium";
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $234
ASGNP4
line 302
;302:	podium->s.eType = ET_GENERAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 303
;303:	podium->s.number = podium - g_entities;
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
line 304
;304:	podium->clipmask = CONTENTS_SOLID;
ADDRLP4 0
INDIRP4
CNSTI4 572
ADDP4
CNSTI4 1
ASGNI4
line 305
;305:	podium->r.contents = CONTENTS_SOLID;
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 1
ASGNI4
line 306
;306:	podium->s.modelindex = G_ModelIndex( SP_PODIUM_MODEL );
ADDRGP4 $235
ARGP4
ADDRLP4 36
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 160
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 308
;307:
;308:	AngleVectors( level.intermission_angle, vec, NULL, NULL );
ADDRGP4 level+9156
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 40
CNSTP4 0
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 309
;309:	VectorMA( level.intermission_origin, trap_Cvar_VariableIntegerValue( "g_podiumDist" ), vec, origin );
ADDRGP4 $127
ARGP4
ADDRLP4 44
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 16
ADDRGP4 level+9144
INDIRF4
ADDRLP4 4
INDIRF4
ADDRLP4 44
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRGP4 $127
ARGP4
ADDRLP4 48
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 16+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 48
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
ADDRGP4 $127
ARGP4
ADDRLP4 52
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 16+8
ADDRGP4 level+9144+8
INDIRF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 52
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
line 310
;310:	origin[2] -= trap_Cvar_VariableIntegerValue( "g_podiumDrop" );
ADDRGP4 $137
ARGP4
ADDRLP4 56
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 16+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 56
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 311
;311:	G_SetOrigin( podium, origin );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 16
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 313
;312:
;313:	VectorSubtract( level.intermission_origin, podium->r.currentOrigin, vec );
ADDRLP4 4
ADDRGP4 level+9144
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRGP4 level+9144+4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRGP4 level+9144+8
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
SUBF4
ASGNF4
line 314
;314:	podium->s.apos.trBase[YAW] = vectoyaw( vec );
ADDRLP4 4
ARGP4
ADDRLP4 64
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 64
INDIRF4
ASGNF4
line 315
;315:	trap_LinkEntity (podium);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 317
;316:
;317:	podium->think = PodiumPlacementThink;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 PodiumPlacementThink
ASGNP4
line 318
;318:	podium->nextthink = level.time + 100;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 319
;319:	return podium;
ADDRLP4 0
INDIRP4
RETP4
LABELV $231
endproc SpawnPodium 68 16
export SpawnModelsOnVictoryPads
proc SpawnModelsOnVictoryPads 24 16
line 328
;320:}
;321:
;322:
;323:/*
;324:==================
;325:SpawnModelsOnVictoryPads
;326:==================
;327:*/
;328:void SpawnModelsOnVictoryPads( void ) {
line 332
;329:	gentity_t	*player;
;330:	gentity_t	*podium;
;331:
;332:	podium1 = NULL;
ADDRGP4 podium1
CNSTP4 0
ASGNP4
line 333
;333:	podium2 = NULL;
ADDRGP4 podium2
CNSTP4 0
ASGNP4
line 334
;334:	podium3 = NULL;
ADDRGP4 podium3
CNSTP4 0
ASGNP4
line 336
;335:
;336:	podium = SpawnPodium();
ADDRLP4 8
ADDRGP4 SpawnPodium
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 338
;337:
;338:	player = SpawnModelOnVictoryPad( podium, offsetFirst, &g_entities[level.sortedClients[0]],
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 offsetFirst
ARGP4
CNSTI4 808
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
CNSTI4 776
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 256
ADDP4
INDIRI4
CNSTI4 -16385
BANDI4
ARGI4
ADDRLP4 12
ADDRGP4 SpawnModelOnVictoryPad
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 340
;339:				level.clients[ level.sortedClients[0] ].ps.persistant[PERS_RANK] &~ RANK_TIED_FLAG );
;340:	if ( player ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $258
line 341
;341:		player->nextthink = level.time + 2000;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 342
;342:		player->think = CelebrateStart;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 CelebrateStart
ASGNP4
line 343
;343:		podium1 = player;
ADDRGP4 podium1
ADDRLP4 0
INDIRP4
ASGNP4
line 344
;344:	}
LABELV $258
line 346
;345:
;346:	player = SpawnModelOnVictoryPad( podium, offsetSecond, &g_entities[level.sortedClients[1]],
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 offsetSecond
ARGP4
CNSTI4 808
ADDRGP4 level+84+4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
CNSTI4 776
ADDRGP4 level+84+4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 256
ADDP4
INDIRI4
CNSTI4 -16385
BANDI4
ARGI4
ADDRLP4 16
ADDRGP4 SpawnModelOnVictoryPad
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 348
;347:				level.clients[ level.sortedClients[1] ].ps.persistant[PERS_RANK] &~ RANK_TIED_FLAG );
;348:	if ( player ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $265
line 349
;349:		podium2 = player;
ADDRGP4 podium2
ADDRLP4 0
INDIRP4
ASGNP4
line 350
;350:	}
LABELV $265
line 352
;351:
;352:	if ( level.numNonSpectatorClients > 2 ) {
ADDRGP4 level+76
INDIRI4
CNSTI4 2
LEI4 $267
line 353
;353:		player = SpawnModelOnVictoryPad( podium, offsetThird, &g_entities[level.sortedClients[2]],
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 offsetThird
ARGP4
CNSTI4 808
ADDRGP4 level+84+8
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
CNSTI4 776
ADDRGP4 level+84+8
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 256
ADDP4
INDIRI4
CNSTI4 -16385
BANDI4
ARGI4
ADDRLP4 20
ADDRGP4 SpawnModelOnVictoryPad
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 355
;354:				level.clients[ level.sortedClients[2] ].ps.persistant[PERS_RANK] &~ RANK_TIED_FLAG );
;355:		if ( player ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $274
line 356
;356:			podium3 = player;
ADDRGP4 podium3
ADDRLP4 0
INDIRP4
ASGNP4
line 357
;357:		}
LABELV $274
line 358
;358:	}
LABELV $267
line 359
;359:}
LABELV $255
endproc SpawnModelsOnVictoryPads 24 16
export Svcmd_AbortPodium_f
proc Svcmd_AbortPodium_f 0 0
line 367
;360:
;361:
;362:/*
;363:===============
;364:Svcmd_AbortPodium_f
;365:===============
;366:*/
;367:void Svcmd_AbortPodium_f( void ) {
line 368
;368:	if( g_gametype.integer != GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
EQI4 $277
line 369
;369:		return;
ADDRGP4 $276
JUMPV
LABELV $277
line 372
;370:	}
;371:
;372:	if( podium1 ) {
ADDRGP4 podium1
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $280
line 373
;373:		podium1->nextthink = level.time;
ADDRGP4 podium1
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 374
;374:		podium1->think = CelebrateStop;
ADDRGP4 podium1
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 CelebrateStop
ASGNP4
line 375
;375:	}
LABELV $280
line 376
;376:}
LABELV $276
endproc Svcmd_AbortPodium_f 0 0
bss
export podium3
align 4
LABELV podium3
skip 4
export podium2
align 4
LABELV podium2
skip 4
export podium1
align 4
LABELV podium1
skip 4
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
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
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
import SpotWouldTelefrag
import CalculateRanks
import AddScore
import player_die
import ClientSpawn
import InitBodyQue
import InitClientResp
import InitClientPersistant
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
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
LABELV $235
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 109
byte 1 97
byte 1 112
byte 1 111
byte 1 98
byte 1 106
byte 1 101
byte 1 99
byte 1 116
byte 1 115
byte 1 47
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 47
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 52
byte 1 46
byte 1 109
byte 1 100
byte 1 51
byte 1 0
align 1
LABELV $234
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $137
byte 1 103
byte 1 95
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 68
byte 1 114
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $127
byte 1 103
byte 1 95
byte 1 112
byte 1 111
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 68
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $89
byte 1 94
byte 1 49
byte 1 69
byte 1 82
byte 1 82
byte 1 79
byte 1 82
byte 1 58
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 103
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $83
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $75
byte 1 112
byte 1 111
byte 1 115
byte 1 116
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $68
byte 1 112
byte 1 111
byte 1 115
byte 1 116
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 48
byte 1 32
byte 1 48
byte 1 32
byte 1 48
byte 1 32
byte 1 48
byte 1 32
byte 1 48
byte 1 32
byte 1 48
byte 1 0
