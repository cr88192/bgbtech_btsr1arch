export ScorePlum
code
proc ScorePlum 12 8
file "../g_combat.c"
line 33
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
;23:// g_combat.c
;24:
;25:#include "g_local.h"
;26:
;27:
;28:/*
;29:============
;30:ScorePlum
;31:============
;32:*/
;33:void ScorePlum( gentity_t *ent, vec3_t origin, int score ) {
line 36
;34:	gentity_t *plum;
;35:
;36:	plum = G_TempEntity( origin, EV_SCOREPLUM );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 65
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 38
;37:	// only send this temp entity to a single client
;38:	plum->r.svFlags |= SVF_SINGLECLIENT;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 39
;39:	plum->r.singleClient = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 428
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 41
;40:	//
;41:	plum->s.otherEntityNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 42
;42:	plum->s.time = score;
ADDRLP4 0
INDIRP4
CNSTI4 84
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 43
;43:}
LABELV $52
endproc ScorePlum 12 8
export AddScore
proc AddScore 8 12
line 52
;44:
;45:/*
;46:============
;47:AddScore
;48:
;49:Adds score to both the client and his team
;50:============
;51:*/
;52:void AddScore( gentity_t *ent, vec3_t origin, int score ) {
line 53
;53:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $54
line 54
;54:		return;
ADDRGP4 $53
JUMPV
LABELV $54
line 57
;55:	}
;56:	// no scoring during pre-match warmup
;57:	if ( level.warmupTime ) {
ADDRGP4 level+16
INDIRI4
CNSTI4 0
EQI4 $56
line 58
;58:		return;
ADDRGP4 $53
JUMPV
LABELV $56
line 61
;59:	}
;60:	// show score plum
;61:	ScorePlum(ent, origin, score);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 ScorePlum
CALLV
pop
line 63
;62:	//
;63:	ent->client->ps.persistant[PERS_SCORE] += score;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 248
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
line 64
;64:	if ( g_gametype.integer == GT_TEAM )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $59
line 65
;65:		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+44
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
LABELV $59
line 66
;66:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 67
;67:}
LABELV $53
endproc AddScore 8 12
export TossClientItems
proc TossClientItems 40 12
line 76
;68:
;69:/*
;70:=================
;71:TossClientItems
;72:
;73:Toss the weapon and powerups for the killed player
;74:=================
;75:*/
;76:void TossClientItems( gentity_t *self ) {
line 84
;77:	gitem_t		*item;
;78:	int			weapon;
;79:	float		angle;
;80:	int			i;
;81:	gentity_t	*drop;
;82:
;83:	// drop the weapon if not a gauntlet or machinegun
;84:	weapon = self->s.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
ASGNI4
line 90
;85:
;86:	// make a special check to see if they are changing to a new
;87:	// weapon that isn't the mg or gauntlet.  Without this, a client
;88:	// can pick up a weapon, be killed, and not drop the weapon because
;89:	// their weapon change hasn't completed yet and they are still holding the MG.
;90:	if ( weapon == WP_MACHINEGUN || weapon == WP_GRAPPLING_HOOK ) {
ADDRLP4 12
INDIRI4
CNSTI4 2
EQI4 $66
ADDRLP4 12
INDIRI4
CNSTI4 10
NEI4 $64
LABELV $66
line 91
;91:		if ( self->client->ps.weaponstate == WEAPON_DROPPING ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $67
line 92
;92:			weapon = self->client->pers.cmd.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 492
ADDP4
INDIRU1
CVUI4 1
ASGNI4
line 93
;93:		}
LABELV $67
line 94
;94:		if ( !( self->client->ps.stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 12
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $69
line 95
;95:			weapon = WP_NONE;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 96
;96:		}
LABELV $69
line 97
;97:	}
LABELV $64
line 99
;98:
;99:	if ( weapon > WP_MACHINEGUN && weapon != WP_GRAPPLING_HOOK && 
ADDRLP4 28
CNSTI4 2
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 28
INDIRI4
LEI4 $71
ADDRLP4 12
INDIRI4
CNSTI4 10
EQI4 $71
ADDRLP4 12
INDIRI4
ADDRLP4 28
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 376
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $71
line 100
;100:		self->client->ps.ammo[ weapon ] ) {
line 102
;101:		// find the item type for this weapon
;102:		item = BG_FindItemForWeapon( weapon );
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 32
INDIRP4
ASGNP4
line 105
;103:
;104:		// spawn the item
;105:		Drop_Item( self, item, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Drop_Item
CALLP4
pop
line 106
;106:	}
LABELV $71
line 109
;107:
;108:	// drop all the powerups if not in teamplay
;109:	if ( g_gametype.integer != GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
EQI4 $73
line 110
;110:		angle = 45;
ADDRLP4 16
CNSTF4 1110704128
ASGNF4
line 111
;111:		for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $76
line 112
;112:			if ( self->client->ps.powerups[ i ] > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $80
line 113
;113:				item = BG_FindItemForPowerup( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 32
INDIRP4
ASGNP4
line 114
;114:				if ( !item ) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $83
line 115
;115:					continue;
ADDRGP4 $77
JUMPV
LABELV $83
line 117
;116:				}
;117:				drop = Drop_Item( self, item, angle );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 Drop_Item
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 36
INDIRP4
ASGNP4
line 119
;118:				// decide how many seconds it has left
;119:				drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
ASGNI4
line 120
;120:				if ( drop->count < 1 ) {
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
CNSTI4 1
GEI4 $86
line 121
;121:					drop->count = 1;
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
CNSTI4 1
ASGNI4
line 122
;122:				}
LABELV $86
line 123
;123:				angle += 45;
ADDRLP4 16
ADDRLP4 16
INDIRF4
CNSTF4 1110704128
ADDF4
ASGNF4
line 124
;124:			}
LABELV $80
line 125
;125:		}
LABELV $77
line 111
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 15
LTI4 $76
line 126
;126:	}
LABELV $73
line 127
;127:}
LABELV $63
endproc TossClientItems 40 12
export LookAtKiller
proc LookAtKiller 52 4
line 217
;128:
;129:#ifdef MISSIONPACK
;130:
;131:/*
;132:=================
;133:TossClientCubes
;134:=================
;135:*/
;136:extern gentity_t	*neutralObelisk;
;137:
;138:void TossClientCubes( gentity_t *self ) {
;139:	gitem_t		*item;
;140:	gentity_t	*drop;
;141:	vec3_t		velocity;
;142:	vec3_t		angles;
;143:	vec3_t		origin;
;144:
;145:	self->client->ps.generic1 = 0;
;146:
;147:	// this should never happen but we should never
;148:	// get the server to crash due to skull being spawned in
;149:	if (!G_EntitiesFree()) {
;150:		return;
;151:	}
;152:
;153:	if( self->client->sess.sessionTeam == TEAM_RED ) {
;154:		item = BG_FindItem( "Red Cube" );
;155:	}
;156:	else {
;157:		item = BG_FindItem( "Blue Cube" );
;158:	}
;159:
;160:	angles[YAW] = (float)(level.time % 360);
;161:	angles[PITCH] = 0;	// always forward
;162:	angles[ROLL] = 0;
;163:
;164:	AngleVectors( angles, velocity, NULL, NULL );
;165:	VectorScale( velocity, 150, velocity );
;166:	velocity[2] += 200 + crandom() * 50;
;167:
;168:	if( neutralObelisk ) {
;169:		VectorCopy( neutralObelisk->s.pos.trBase, origin );
;170:		origin[2] += 44;
;171:	} else {
;172:		VectorClear( origin ) ;
;173:	}
;174:
;175:	drop = LaunchItem( item, origin, velocity );
;176:
;177:	drop->nextthink = level.time + g_cubeTimeout.integer * 1000;
;178:	drop->think = G_FreeEntity;
;179:	drop->spawnflags = self->client->sess.sessionTeam;
;180:}
;181:
;182:
;183:/*
;184:=================
;185:TossClientPersistantPowerups
;186:=================
;187:*/
;188:void TossClientPersistantPowerups( gentity_t *ent ) {
;189:	gentity_t	*powerup;
;190:
;191:	if( !ent->client ) {
;192:		return;
;193:	}
;194:
;195:	if( !ent->client->persistantPowerup ) {
;196:		return;
;197:	}
;198:
;199:	powerup = ent->client->persistantPowerup;
;200:
;201:	powerup->r.svFlags &= ~SVF_NOCLIENT;
;202:	powerup->s.eFlags &= ~EF_NODRAW;
;203:	powerup->r.contents = CONTENTS_TRIGGER;
;204:	trap_LinkEntity( powerup );
;205:
;206:	ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
;207:	ent->client->persistantPowerup = NULL;
;208:}
;209:#endif
;210:
;211:
;212:/*
;213:==================
;214:LookAtKiller
;215:==================
;216:*/
;217:void LookAtKiller( gentity_t *self, gentity_t *inflictor, gentity_t *attacker ) {
line 221
;218:	vec3_t		dir;
;219:	vec3_t		angles;
;220:
;221:	if ( attacker && attacker != self ) {
ADDRLP4 24
ADDRFP4 8
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 24
INDIRU4
CNSTU4 0
EQU4 $89
ADDRLP4 24
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $89
line 222
;222:		VectorSubtract (attacker->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 28
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 32
CNSTI4 24
ASGNI4
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 40
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 8
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 223
;223:	} else if ( inflictor && inflictor != self ) {
ADDRGP4 $90
JUMPV
LABELV $89
ADDRLP4 28
ADDRFP4 4
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 28
INDIRU4
CNSTU4 0
EQU4 $93
ADDRLP4 28
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $93
line 224
;224:		VectorSubtract (inflictor->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 32
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 24
ASGNI4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 32
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 225
;225:	} else {
ADDRGP4 $94
JUMPV
LABELV $93
line 226
;226:		self->client->ps.stats[STAT_DEAD_YAW] = self->s.angles[YAW];
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 227
;227:		return;
ADDRGP4 $88
JUMPV
LABELV $94
LABELV $90
line 230
;228:	}
;229:
;230:	self->client->ps.stats[STAT_DEAD_YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 32
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 232
;231:
;232:	angles[YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 36
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 36
INDIRF4
ASGNF4
line 233
;233:	angles[PITCH] = 0; 
ADDRLP4 12
CNSTF4 0
ASGNF4
line 234
;234:	angles[ROLL] = 0;
ADDRLP4 12+8
CNSTF4 0
ASGNF4
line 235
;235:}
LABELV $88
endproc LookAtKiller 52 4
export GibEntity
proc GibEntity 12 12
line 242
;236:
;237:/*
;238:==================
;239:GibEntity
;240:==================
;241:*/
;242:void GibEntity( gentity_t *self, int killer ) {
line 247
;243:	gentity_t *ent;
;244:	int i;
;245:
;246:	//if this entity still has kamikaze
;247:	if (self->s.eFlags & EF_KAMIKAZE) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $100
line 249
;248:		// check if there is a kamikaze timer around for this owner
;249:		for (i = 0; i < MAX_GENTITIES; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $102
line 250
;250:			ent = &g_entities[i];
ADDRLP4 0
CNSTI4 808
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 251
;251:			if (!ent->inuse)
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $106
line 252
;252:				continue;
ADDRGP4 $103
JUMPV
LABELV $106
line 253
;253:			if (ent->activator != self)
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $108
line 254
;254:				continue;
ADDRGP4 $103
JUMPV
LABELV $108
line 255
;255:			if (strcmp(ent->classname, "kamikaze timer"))
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRGP4 $112
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $110
line 256
;256:				continue;
ADDRGP4 $103
JUMPV
LABELV $110
line 257
;257:			G_FreeEntity(ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 258
;258:			break;
ADDRGP4 $104
JUMPV
LABELV $103
line 249
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $102
LABELV $104
line 260
;259:		}
;260:	}
LABELV $100
line 261
;261:	G_AddEvent( self, EV_GIB_PLAYER, killer );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 262
;262:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 0
ASGNI4
line 263
;263:	self->s.eType = ET_INVISIBLE;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 10
ASGNI4
line 264
;264:	self->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 0
ASGNI4
line 265
;265:}
LABELV $99
endproc GibEntity 12 12
export body_die
proc body_die 0 8
line 272
;266:
;267:/*
;268:==================
;269:body_die
;270:==================
;271:*/
;272:void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 273
;273:	if ( self->health > GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
LEI4 $114
line 274
;274:		return;
ADDRGP4 $113
JUMPV
LABELV $114
line 276
;275:	}
;276:	if ( !g_blood.integer ) {
ADDRGP4 g_blood+12
INDIRI4
CNSTI4 0
NEI4 $116
line 277
;277:		self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -39
ASGNI4
line 278
;278:		return;
ADDRGP4 $113
JUMPV
LABELV $116
line 281
;279:	}
;280:
;281:	GibEntity( self, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 GibEntity
CALLV
pop
line 282
;282:}
LABELV $113
endproc body_die 0 8
data
export modNames
align 4
LABELV modNames
address $119
address $120
address $121
address $122
address $123
address $124
address $125
address $126
address $127
address $128
address $129
address $130
address $131
address $132
address $133
address $134
address $135
address $136
address $137
address $138
address $139
address $140
address $141
address $142
export CheckAlmostCapture
code
proc CheckAlmostCapture 56 12
line 356
;283:
;284:
;285:// these are just for logging, the client prints its own messages
;286:char	*modNames[] = {
;287:	"MOD_UNKNOWN",
;288:	"MOD_SHOTGUN",
;289:	"MOD_GAUNTLET",
;290:	"MOD_MACHINEGUN",
;291:	"MOD_GRENADE",
;292:	"MOD_GRENADE_SPLASH",
;293:	"MOD_ROCKET",
;294:	"MOD_ROCKET_SPLASH",
;295:	"MOD_PLASMA",
;296:	"MOD_PLASMA_SPLASH",
;297:	"MOD_RAILGUN",
;298:	"MOD_LIGHTNING",
;299:	"MOD_BFG",
;300:	"MOD_BFG_SPLASH",
;301:	"MOD_WATER",
;302:	"MOD_SLIME",
;303:	"MOD_LAVA",
;304:	"MOD_CRUSH",
;305:	"MOD_TELEFRAG",
;306:	"MOD_FALLING",
;307:	"MOD_SUICIDE",
;308:	"MOD_TARGET_LASER",
;309:	"MOD_TRIGGER_HURT",
;310:#ifdef MISSIONPACK
;311:	"MOD_NAIL",
;312:	"MOD_CHAINGUN",
;313:	"MOD_PROXIMITY_MINE",
;314:	"MOD_KAMIKAZE",
;315:	"MOD_JUICED",
;316:#endif
;317:	"MOD_GRAPPLE"
;318:};
;319:
;320:#ifdef MISSIONPACK
;321:/*
;322:==================
;323:Kamikaze_DeathActivate
;324:==================
;325:*/
;326:void Kamikaze_DeathActivate( gentity_t *ent ) {
;327:	G_StartKamikaze(ent);
;328:	G_FreeEntity(ent);
;329:}
;330:
;331:/*
;332:==================
;333:Kamikaze_DeathTimer
;334:==================
;335:*/
;336:void Kamikaze_DeathTimer( gentity_t *self ) {
;337:	gentity_t *ent;
;338:
;339:	ent = G_Spawn();
;340:	ent->classname = "kamikaze timer";
;341:	VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
;342:	ent->r.svFlags |= SVF_NOCLIENT;
;343:	ent->think = Kamikaze_DeathActivate;
;344:	ent->nextthink = level.time + 5 * 1000;
;345:
;346:	ent->activator = self;
;347:}
;348:
;349:#endif
;350:
;351:/*
;352:==================
;353:CheckAlmostCapture
;354:==================
;355:*/
;356:void CheckAlmostCapture( gentity_t *self, gentity_t *attacker ) {
line 362
;357:	gentity_t	*ent;
;358:	vec3_t		dir;
;359:	char		*classname;
;360:
;361:	// if this player was carrying a flag
;362:	if ( self->client->ps.powerups[PW_REDFLAG] ||
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $147
ADDRLP4 20
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $147
ADDRLP4 20
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $144
LABELV $147
line 364
;363:		self->client->ps.powerups[PW_BLUEFLAG] ||
;364:		self->client->ps.powerups[PW_NEUTRALFLAG] ) {
line 366
;365:		// get the goal flag this player should have been going for
;366:		if ( g_gametype.integer == GT_CTF ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $148
line 367
;367:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $151
line 368
;368:				classname = "team_CTF_blueflag";
ADDRLP4 4
ADDRGP4 $153
ASGNP4
line 369
;369:			}
ADDRGP4 $149
JUMPV
LABELV $151
line 370
;370:			else {
line 371
;371:				classname = "team_CTF_redflag";
ADDRLP4 4
ADDRGP4 $154
ASGNP4
line 372
;372:			}
line 373
;373:		}
ADDRGP4 $149
JUMPV
LABELV $148
line 374
;374:		else {
line 375
;375:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $155
line 376
;376:				classname = "team_CTF_redflag";
ADDRLP4 4
ADDRGP4 $154
ASGNP4
line 377
;377:			}
ADDRGP4 $156
JUMPV
LABELV $155
line 378
;378:			else {
line 379
;379:				classname = "team_CTF_blueflag";
ADDRLP4 4
ADDRGP4 $153
ASGNP4
line 380
;380:			}
LABELV $156
line 381
;381:		}
LABELV $149
line 382
;382:		ent = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
LABELV $157
line 384
;383:		do
;384:		{
line 385
;385:			ent = G_Find(ent, FOFS(classname), classname);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 524
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
line 386
;386:		} while (ent && (ent->flags & FL_DROPPED_ITEM));
LABELV $158
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $160
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
NEI4 $157
LABELV $160
line 388
;387:		// if we found the destination flag and it's not picked up
;388:		if (ent && !(ent->r.svFlags & SVF_NOCLIENT) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $161
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $161
line 390
;389:			// if the player was *very* close
;390:			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+4
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 8+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 391
;391:			if ( VectorLength(dir) < 200 ) {
ADDRLP4 8
ARGP4
ADDRLP4 44
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 44
INDIRF4
CNSTF4 1128792064
GEF4 $165
line 392
;392:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 393
;393:				if ( attacker->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $167
line 394
;394:					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 52
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 395
;395:				}
LABELV $167
line 396
;396:			}
LABELV $165
line 397
;397:		}
LABELV $161
line 398
;398:	}
LABELV $144
line 399
;399:}
LABELV $143
endproc CheckAlmostCapture 56 12
export CheckAlmostScored
proc CheckAlmostScored 44 12
line 406
;400:
;401:/*
;402:==================
;403:CheckAlmostScored
;404:==================
;405:*/
;406:void CheckAlmostScored( gentity_t *self, gentity_t *attacker ) {
line 412
;407:	gentity_t	*ent;
;408:	vec3_t		dir;
;409:	char		*classname;
;410:
;411:	// if the player was carrying cubes
;412:	if ( self->client->ps.generic1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 440
ADDP4
INDIRI4
CNSTI4 0
EQI4 $170
line 413
;413:		if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 2
NEI4 $172
line 414
;414:			classname = "team_redobelisk";
ADDRLP4 16
ADDRGP4 $174
ASGNP4
line 415
;415:		}
ADDRGP4 $173
JUMPV
LABELV $172
line 416
;416:		else {
line 417
;417:			classname = "team_blueobelisk";
ADDRLP4 16
ADDRGP4 $175
ASGNP4
line 418
;418:		}
LABELV $173
line 419
;419:		ent = G_Find(NULL, FOFS(classname), classname);
CNSTP4 0
ARGP4
CNSTI4 524
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 421
;420:		// if we found the destination obelisk
;421:		if ( ent ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $176
line 423
;422:			// if the player was *very* close
;423:			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 28
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 424
;424:			if ( VectorLength(dir) < 200 ) {
ADDRLP4 4
ARGP4
ADDRLP4 32
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 32
INDIRF4
CNSTF4 1128792064
GEF4 $180
line 425
;425:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 426
;426:				if ( attacker->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $182
line 427
;427:					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
ADDRLP4 40
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 428
;428:				}
LABELV $182
line 429
;429:			}
LABELV $180
line 430
;430:		}
LABELV $176
line 431
;431:	}
LABELV $170
line 432
;432:}
LABELV $169
endproc CheckAlmostScored 44 12
bss
align 4
LABELV $253
skip 4
export player_die
code
proc player_die 96 28
line 439
;433:
;434:/*
;435:==================
;436:player_die
;437:==================
;438:*/
;439:void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 447
;440:	gentity_t	*ent;
;441:	int			anim;
;442:	int			contents;
;443:	int			killer;
;444:	int			i;
;445:	char		*killerName, *obit;
;446:
;447:	if ( self->client->ps.pm_type == PM_DEAD ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $185
line 448
;448:		return;
ADDRGP4 $184
JUMPV
LABELV $185
line 451
;449:	}
;450:
;451:	if ( level.intermissiontime ) {
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $187
line 452
;452:		return;
ADDRGP4 $184
JUMPV
LABELV $187
line 456
;453:	}
;454:
;455:	// check for an almost capture
;456:	CheckAlmostCapture( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 CheckAlmostCapture
CALLV
pop
line 458
;457:	// check for a player that almost brought in cubes
;458:	CheckAlmostScored( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 CheckAlmostScored
CALLV
pop
line 460
;459:
;460:	if (self->client && self->client->hook) {
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 32
CNSTU4 0
ASGNU4
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $190
ADDRLP4 28
INDIRP4
CNSTI4 760
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $190
line 461
;461:		Weapon_HookFree(self->client->hook);
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 760
ADDP4
INDIRP4
ARGP4
ADDRGP4 Weapon_HookFree
CALLV
pop
line 462
;462:	}
LABELV $190
line 470
;463:#ifdef MISSIONPACK
;464:	if ((self->client->ps.eFlags & EF_TICKING) && self->activator) {
;465:		self->client->ps.eFlags &= ~EF_TICKING;
;466:		self->activator->think = G_FreeEntity;
;467:		self->activator->nextthink = level.time;
;468:	}
;469:#endif
;470:	self->client->ps.pm_type = PM_DEAD;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 472
;471:
;472:	if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $192
line 473
;473:		killer = attacker->s.number;
ADDRLP4 4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 474
;474:		if ( attacker->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $194
line 475
;475:			killerName = attacker->client->pers.netname;
ADDRLP4 16
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ASGNP4
line 476
;476:		} else {
ADDRGP4 $193
JUMPV
LABELV $194
line 477
;477:			killerName = "<non-client>";
ADDRLP4 16
ADDRGP4 $196
ASGNP4
line 478
;478:		}
line 479
;479:	} else {
ADDRGP4 $193
JUMPV
LABELV $192
line 480
;480:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 481
;481:		killerName = "<world>";
ADDRLP4 16
ADDRGP4 $197
ASGNP4
line 482
;482:	}
LABELV $193
line 484
;483:
;484:	if ( killer < 0 || killer >= MAX_CLIENTS ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $200
ADDRLP4 4
INDIRI4
CNSTI4 64
LTI4 $198
LABELV $200
line 485
;485:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 486
;486:		killerName = "<world>";
ADDRLP4 16
ADDRGP4 $197
ASGNP4
line 487
;487:	}
LABELV $198
line 489
;488:
;489:	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
ADDRLP4 40
ADDRFP4 16
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
LTI4 $203
ADDRLP4 40
INDIRI4
CVIU4 4
CNSTU4 24
LTU4 $201
LABELV $203
line 490
;490:		obit = "<bad obituary>";
ADDRLP4 20
ADDRGP4 $204
ASGNP4
line 491
;491:	} else {
ADDRGP4 $202
JUMPV
LABELV $201
line 492
;492:		obit = modNames[ meansOfDeath ];
ADDRLP4 20
ADDRFP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 modNames
ADDP4
INDIRP4
ASGNP4
line 493
;493:	}
LABELV $202
line 495
;494:
;495:	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n", 
ADDRGP4 $205
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 500
;496:		killer, self->s.number, meansOfDeath, killerName, 
;497:		self->client->pers.netname, obit );
;498:
;499:	// broadcast the death event to everyone
;500:	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 60
ARGI4
ADDRLP4 48
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 48
INDIRP4
ASGNP4
line 501
;501:	ent->s.eventParm = meansOfDeath;
ADDRLP4 8
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 502
;502:	ent->s.otherEntityNum = self->s.number;
ADDRLP4 8
INDIRP4
CNSTI4 140
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 503
;503:	ent->s.otherEntityNum2 = killer;
ADDRLP4 8
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 504
;504:	ent->r.svFlags = SVF_BROADCAST;	// send to everyone
ADDRLP4 8
INDIRP4
CNSTI4 424
ADDP4
CNSTI4 32
ASGNI4
line 506
;505:
;506:	self->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 508
;507:
;508:	self->client->ps.persistant[PERS_KILLED]++;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 280
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 510
;509:
;510:	if (attacker && attacker->client) {
ADDRLP4 56
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
CNSTU4 0
ASGNU4
ADDRLP4 56
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $206
ADDRLP4 56
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $206
line 511
;511:		attacker->client->lastkilled_client = self->s.number;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 720
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 513
;512:
;513:		if ( attacker == self || OnSameTeam (self, attacker ) ) {
ADDRLP4 64
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
ADDRLP4 68
INDIRP4
CVPU4 4
EQU4 $210
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
EQI4 $208
LABELV $210
line 514
;514:			AddScore( attacker, self->r.currentOrigin, -1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 515
;515:		} else {
ADDRGP4 $207
JUMPV
LABELV $208
line 516
;516:			AddScore( attacker, self->r.currentOrigin, 1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 518
;517:
;518:			if( meansOfDeath == MOD_GAUNTLET ) {
ADDRFP4 16
INDIRI4
CNSTI4 2
NEI4 $211
line 521
;519:				
;520:				// play humiliation on player
;521:				attacker->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]++;
ADDRLP4 76
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 300
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 524
;522:
;523:				// add the sprite over the player's head
;524:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 80
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 525
;525:				attacker->client->ps.eFlags |= EF_AWARD_GAUNTLET;
ADDRLP4 84
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 526
;526:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 744
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 529
;527:
;528:				// also play humiliation on target
;529:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 268
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 2
BXORI4
ASGNI4
line 530
;530:			}
LABELV $211
line 534
;531:
;532:			// check for two kills in a short amount of time
;533:			// if this is close enough to the last kill, give a reward sound
;534:			if ( level.time - attacker->client->lastKillTime < CARNAGE_REWARD_TIME ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
SUBI4
CNSTI4 3000
GEI4 $214
line 536
;535:				// play excellent on player
;536:				attacker->client->ps.persistant[PERS_EXCELLENT_COUNT]++;
ADDRLP4 76
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 288
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 539
;537:
;538:				// add the sprite over the player's head
;539:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 80
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 540
;540:				attacker->client->ps.eFlags |= EF_AWARD_EXCELLENT;
ADDRLP4 84
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 541
;541:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 744
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 542
;542:			}
LABELV $214
line 543
;543:			attacker->client->lastKillTime = level.time;
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 752
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 545
;544:
;545:		}
line 546
;546:	} else {
ADDRGP4 $207
JUMPV
LABELV $206
line 547
;547:		AddScore( self, self->r.currentOrigin, -1 );
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 548
;548:	}
LABELV $207
line 551
;549:
;550:	// Add team bonuses
;551:	Team_FragBonuses(self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_FragBonuses
CALLV
pop
line 554
;552:
;553:	// if I committed suicide, the flag does not fall, it returns.
;554:	if (meansOfDeath == MOD_SUICIDE) {
ADDRFP4 16
INDIRI4
CNSTI4 20
NEI4 $219
line 555
;555:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $221
line 556
;556:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 557
;557:			self->client->ps.powerups[PW_NEUTRALFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
CNSTI4 0
ASGNI4
line 558
;558:		}
ADDRGP4 $222
JUMPV
LABELV $221
line 559
;559:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $223
line 560
;560:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 561
;561:			self->client->ps.powerups[PW_REDFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 562
;562:		}
ADDRGP4 $224
JUMPV
LABELV $223
line 563
;563:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $225
line 564
;564:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 565
;565:			self->client->ps.powerups[PW_BLUEFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
CNSTI4 0
ASGNI4
line 566
;566:		}
LABELV $225
LABELV $224
LABELV $222
line 567
;567:	}
LABELV $219
line 570
;568:
;569:	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
;570:	contents = trap_PointContents( self->r.currentOrigin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 64
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 64
INDIRI4
ASGNI4
line 571
;571:	if ( !( contents & CONTENTS_NODROP )) {
ADDRLP4 12
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
NEU4 $227
line 572
;572:		TossClientItems( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 573
;573:	}
ADDRGP4 $228
JUMPV
LABELV $227
line 574
;574:	else {
line 575
;575:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $229
line 576
;576:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 577
;577:		}
ADDRGP4 $230
JUMPV
LABELV $229
line 578
;578:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $231
line 579
;579:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 580
;580:		}
ADDRGP4 $232
JUMPV
LABELV $231
line 581
;581:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $233
line 582
;582:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 583
;583:		}
LABELV $233
LABELV $232
LABELV $230
line 584
;584:	}
LABELV $228
line 592
;585:#ifdef MISSIONPACK
;586:	TossClientPersistantPowerups( self );
;587:	if( g_gametype.integer == GT_HARVESTER ) {
;588:		TossClientCubes( self );
;589:	}
;590:#endif
;591:
;592:	Cmd_Score_f( self );		// show scores
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 595
;593:	// send updated scores to any clients that are following this one,
;594:	// or they would get stale scoreboards
;595:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $238
JUMPV
LABELV $235
line 598
;596:		gclient_t	*client;
;597:
;598:		client = &level.clients[i];
ADDRLP4 68
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 599
;599:		if ( client->pers.connected != CON_CONNECTED ) {
ADDRLP4 68
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $240
line 600
;600:			continue;
ADDRGP4 $236
JUMPV
LABELV $240
line 602
;601:		}
;602:		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 68
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $242
line 603
;603:			continue;
ADDRGP4 $236
JUMPV
LABELV $242
line 605
;604:		}
;605:		if ( client->sess.spectatorClient == self->s.number ) {
ADDRLP4 68
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $244
line 606
;606:			Cmd_Score_f( g_entities + i );
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 607
;607:		}
LABELV $244
line 608
;608:	}
LABELV $236
line 595
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $238
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $235
line 610
;609:
;610:	self->takedamage = qtrue;	// can still be gibbed
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
CNSTI4 1
ASGNI4
line 612
;611:
;612:	self->s.weapon = WP_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 0
ASGNI4
line 613
;613:	self->s.powerups = 0;
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 614
;614:	self->r.contents = CONTENTS_CORPSE;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 67108864
ASGNI4
line 616
;615:
;616:	self->s.angles[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 0
ASGNF4
line 617
;617:	self->s.angles[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
CNSTF4 0
ASGNF4
line 618
;618:	LookAtKiller (self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 LookAtKiller
CALLV
pop
line 620
;619:
;620:	VectorCopy( self->s.angles, self->client->ps.viewangles );
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 152
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 622
;621:
;622:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 0
ASGNI4
line 624
;623:
;624:	self->r.maxs[2] = -8;
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
CNSTF4 3238002688
ASGNF4
line 628
;625:
;626:	// don't allow respawn until the death anim is done
;627:	// g_forcerespawn may force spawning at some later time
;628:	self->client->respawnTime = level.time + 1700;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 732
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1700
ADDI4
ASGNI4
line 631
;629:
;630:	// remove powerups
;631:	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 634
;632:
;633:	// never gib in a nodrop
;634:	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $252
ADDRLP4 12
INDIRI4
CVIU4 4
CNSTU4 2147483648
BANDU4
CNSTU4 0
NEU4 $252
ADDRGP4 g_blood+12
INDIRI4
CNSTI4 0
NEI4 $250
LABELV $252
ADDRFP4 16
INDIRI4
CNSTI4 20
NEI4 $247
LABELV $250
line 636
;635:		// gib death
;636:		GibEntity( self, killer );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 GibEntity
CALLV
pop
line 637
;637:	} else {
ADDRGP4 $248
JUMPV
LABELV $247
line 641
;638:		// normal death
;639:		static int i;
;640:
;641:		switch ( i ) {
ADDRLP4 72
ADDRGP4 $253
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
EQI4 $256
ADDRLP4 72
INDIRI4
CNSTI4 1
EQI4 $257
ADDRLP4 72
INDIRI4
CNSTI4 2
EQI4 $258
ADDRGP4 $254
JUMPV
LABELV $256
line 643
;642:		case 0:
;643:			anim = BOTH_DEATH1;
ADDRLP4 24
CNSTI4 0
ASGNI4
line 644
;644:			break;
ADDRGP4 $255
JUMPV
LABELV $257
line 646
;645:		case 1:
;646:			anim = BOTH_DEATH2;
ADDRLP4 24
CNSTI4 2
ASGNI4
line 647
;647:			break;
ADDRGP4 $255
JUMPV
LABELV $258
LABELV $254
line 650
;648:		case 2:
;649:		default:
;650:			anim = BOTH_DEATH3;
ADDRLP4 24
CNSTI4 4
ASGNI4
line 651
;651:			break;
LABELV $255
line 656
;652:		}
;653:
;654:		// for the no-blood option, we need to prevent the health
;655:		// from going to gib level
;656:		if ( self->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $259
line 657
;657:			self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -39
ASGNI4
line 658
;658:		}
LABELV $259
line 660
;659:
;660:		self->client->ps.legsAnim = 
ADDRLP4 76
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 76
ADDP4
ASGNP4
ADDRLP4 80
CNSTI4 128
ASGNI4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
ADDRLP4 80
INDIRI4
BANDI4
ADDRLP4 80
INDIRI4
BXORI4
ADDRLP4 24
INDIRI4
BORI4
ASGNI4
line 662
;661:			( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;662:		self->client->ps.torsoAnim = 
ADDRLP4 84
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 84
ADDP4
ASGNP4
ADDRLP4 88
CNSTI4 128
ASGNI4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
ADDRLP4 88
INDIRI4
BANDI4
ADDRLP4 88
INDIRI4
BXORI4
ADDRLP4 24
INDIRI4
BORI4
ASGNI4
line 665
;663:			( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;664:
;665:		G_AddEvent( self, EV_DEATH1 + i, killer );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $253
INDIRI4
CNSTI4 57
ADDI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 668
;666:
;667:		// the body can still be gibbed
;668:		self->die = body_die;
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 body_die
ASGNP4
line 671
;669:
;670:		// globally cycle through the different death animations
;671:		i = ( i + 1 ) % 3;
ADDRLP4 92
ADDRGP4 $253
ASGNP4
ADDRLP4 92
INDIRP4
ADDRLP4 92
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 3
MODI4
ASGNI4
line 678
;672:
;673:#ifdef MISSIONPACK
;674:		if (self->s.eFlags & EF_KAMIKAZE) {
;675:			Kamikaze_DeathTimer( self );
;676:		}
;677:#endif
;678:	}
LABELV $248
line 680
;679:
;680:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 682
;681:
;682:}
LABELV $184
endproc player_die 96 28
export CheckArmor
proc CheckArmor 20 4
line 691
;683:
;684:
;685:/*
;686:================
;687:CheckArmor
;688:================
;689:*/
;690:int CheckArmor (gentity_t *ent, int damage, int dflags)
;691:{
line 696
;692:	gclient_t	*client;
;693:	int			save;
;694:	int			count;
;695:
;696:	if (!damage)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $262
line 697
;697:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $261
JUMPV
LABELV $262
line 699
;698:
;699:	client = ent->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 701
;700:
;701:	if (!client)
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $264
line 702
;702:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $261
JUMPV
LABELV $264
line 704
;703:
;704:	if (dflags & DAMAGE_NO_ARMOR)
ADDRFP4 8
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $266
line 705
;705:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $261
JUMPV
LABELV $266
line 708
;706:
;707:	// armor
;708:	count = client->ps.stats[STAT_ARMOR];
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
line 709
;709:	save = ceil( damage * ARMOR_PROTECTION );
CNSTF4 1059648963
ADDRFP4 4
INDIRI4
CVIF4 4
MULF4
ARGF4
ADDRLP4 12
ADDRGP4 ceil
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 710
;710:	if (save >= count)
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $268
line 711
;711:		save = count;
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $268
line 713
;712:
;713:	if (!save)
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $270
line 714
;714:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $261
JUMPV
LABELV $270
line 716
;715:
;716:	client->ps.stats[STAT_ARMOR] -= save;
ADDRLP4 16
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 718
;717:
;718:	return save;
ADDRLP4 0
INDIRI4
RETI4
LABELV $261
endproc CheckArmor 20 4
export RaySphereIntersections
proc RaySphereIntersections 96 4
line 726
;719:}
;720:
;721:/*
;722:================
;723:RaySphereIntersections
;724:================
;725:*/
;726:int RaySphereIntersections( vec3_t origin, float radius, vec3_t point, vec3_t dir, vec3_t intersections[2] ) {
line 735
;727:	float b, c, d, t;
;728:
;729:	//	| origin - (point + t * dir) | = radius
;730:	//	a = dir[0]^2 + dir[1]^2 + dir[2]^2;
;731:	//	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
;732:	//	c = (point[0] - origin[0])^2 + (point[1] - origin[1])^2 + (point[2] - origin[2])^2 - radius^2;
;733:
;734:	// normalize dir so a = 1
;735:	VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 736
;736:	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
ADDRLP4 16
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 20
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 4
ASGNI4
ADDRLP4 32
CNSTI4 8
ASGNI4
ADDRLP4 4
CNSTF4 1073741824
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRLP4 24
INDIRP4
INDIRF4
SUBF4
MULF4
ADDRLP4 16
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
ADDRLP4 16
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
MULF4
ASGNF4
line 737
;737:	c = (point[0] - origin[0]) * (point[0] - origin[0]) +
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 4
ASGNI4
ADDRLP4 52
ADDRLP4 36
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56
CNSTI4 8
ASGNI4
ADDRLP4 60
ADDRLP4 36
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 12
ADDRLP4 44
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDRLP4 52
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ADDF4
ADDRLP4 60
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
ADDF4
ADDRLP4 64
INDIRF4
ADDRLP4 64
INDIRF4
MULF4
SUBF4
ASGNF4
line 742
;738:		(point[1] - origin[1]) * (point[1] - origin[1]) +
;739:		(point[2] - origin[2]) * (point[2] - origin[2]) -
;740:		radius * radius;
;741:
;742:	d = b * b - 4 * c;
ADDRLP4 8
ADDRLP4 4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
CNSTF4 1082130432
ADDRLP4 12
INDIRF4
MULF4
SUBF4
ASGNF4
line 743
;743:	if (d > 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
LEF4 $273
line 744
;744:		t = (- b + sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 72
INDIRF4
ADDF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 745
;745:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 80
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 746
;746:		t = (- b - sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 84
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 84
INDIRF4
SUBF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 747
;747:		VectorMA(point, t, dir, intersections[1]);
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 88
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 92
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 748
;748:		return 2;
CNSTI4 2
RETI4
ADDRGP4 $272
JUMPV
LABELV $273
line 750
;749:	}
;750:	else if (d == 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
NEF4 $275
line 751
;751:		t = (- b ) / 2;
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 752
;752:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 72
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 753
;753:		return 1;
CNSTI4 1
RETI4
ADDRGP4 $272
JUMPV
LABELV $275
line 755
;754:	}
;755:	return 0;
CNSTI4 0
RETI4
LABELV $272
endproc RaySphereIntersections 96 4
export G_Damage
proc G_Damage 80 24
line 822
;756:}
;757:
;758:#ifdef MISSIONPACK
;759:/*
;760:================
;761:G_InvulnerabilityEffect
;762:================
;763:*/
;764:int G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir ) {
;765:	gentity_t	*impact;
;766:	vec3_t		intersections[2], vec;
;767:	int			n;
;768:
;769:	if ( !targ->client ) {
;770:		return qfalse;
;771:	}
;772:	VectorCopy(dir, vec);
;773:	VectorInverse(vec);
;774:	// sphere model radius = 42 units
;775:	n = RaySphereIntersections( targ->client->ps.origin, 42, point, vec, intersections);
;776:	if (n > 0) {
;777:		impact = G_TempEntity( targ->client->ps.origin, EV_INVUL_IMPACT );
;778:		VectorSubtract(intersections[0], targ->client->ps.origin, vec);
;779:		vectoangles(vec, impact->s.angles);
;780:		impact->s.angles[0] += 90;
;781:		if (impact->s.angles[0] > 360)
;782:			impact->s.angles[0] -= 360;
;783:		if ( impactpoint ) {
;784:			VectorCopy( intersections[0], impactpoint );
;785:		}
;786:		if ( bouncedir ) {
;787:			VectorCopy( vec, bouncedir );
;788:			VectorNormalize( bouncedir );
;789:		}
;790:		return qtrue;
;791:	}
;792:	else {
;793:		return qfalse;
;794:	}
;795:}
;796:#endif
;797:/*
;798:============
;799:T_Damage
;800:
;801:targ		entity that is being damaged
;802:inflictor	entity that is causing the damage
;803:attacker	entity that caused the inflictor to damage targ
;804:	example: targ=monster, inflictor=rocket, attacker=player
;805:
;806:dir			direction of the attack for knockback
;807:point		point at which the damage is being inflicted, used for headshots
;808:damage		amount of damage being inflicted
;809:knockback	force to be applied against targ as a result of the damage
;810:
;811:inflictor, attacker, dir, and point can be NULL for environmental effects
;812:
;813:dflags		these flags are used to control how T_Damage works
;814:	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
;815:	DAMAGE_NO_ARMOR			armor does not protect from this damage
;816:	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
;817:	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
;818:============
;819:*/
;820:
;821:void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
;822:			   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) {
line 833
;823:	gclient_t	*client;
;824:	int			take;
;825:	int			save;
;826:	int			asave;
;827:	int			knockback;
;828:	int			max;
;829:#ifdef MISSIONPACK
;830:	vec3_t		bouncedir, impactpoint;
;831:#endif
;832:
;833:	if (!targ->takedamage) {
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 0
NEI4 $278
line 834
;834:		return;
ADDRGP4 $277
JUMPV
LABELV $278
line 839
;835:	}
;836:
;837:	// the intermission has allready been qualified for, so don't
;838:	// allow any extra scoring
;839:	if ( level.intermissionQueued ) {
ADDRGP4 level+9124
INDIRI4
CNSTI4 0
EQI4 $280
line 840
;840:		return;
ADDRGP4 $277
JUMPV
LABELV $280
line 852
;841:	}
;842:#ifdef MISSIONPACK
;843:	if ( targ->client && mod != MOD_JUICED) {
;844:		if ( targ->client->invulnerabilityTime > level.time) {
;845:			if ( dir && point ) {
;846:				G_InvulnerabilityEffect( targ, dir, point, impactpoint, bouncedir );
;847:			}
;848:			return;
;849:		}
;850:	}
;851:#endif
;852:	if ( !inflictor ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $283
line 853
;853:		inflictor = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 4
ADDRGP4 g_entities+825776
ASGNP4
line 854
;854:	}
LABELV $283
line 855
;855:	if ( !attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $286
line 856
;856:		attacker = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 8
ADDRGP4 g_entities+825776
ASGNP4
line 857
;857:	}
LABELV $286
line 860
;858:
;859:	// shootable doors / buttons don't actually have any health
;860:	if ( targ->s.eType == ET_MOVER ) {
ADDRLP4 24
CNSTI4 4
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $289
line 861
;861:		if ( targ->use && targ->moverState == MOVER_POS1 ) {
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $277
ADDRLP4 28
INDIRP4
CNSTI4 576
ADDP4
INDIRI4
CNSTI4 0
NEI4 $277
line 862
;862:			targ->use( targ, inflictor, attacker );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CALLV
pop
line 863
;863:		}
line 864
;864:		return;
ADDRGP4 $277
JUMPV
LABELV $289
line 873
;865:	}
;866:#ifdef MISSIONPACK
;867:	if( g_gametype.integer == GT_OBELISK && CheckObeliskAttack( targ, attacker ) ) {
;868:		return;
;869:	}
;870:#endif
;871:	// reduce damage by the attacker's handicap value
;872:	// unless they are rocket jumping
;873:	if ( attacker->client && attacker != targ ) {
ADDRLP4 28
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $293
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $293
line 874
;874:		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 20
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ASGNI4
line 880
;875:#ifdef MISSIONPACK
;876:		if( bg_itemlist[attacker->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;877:			max /= 2;
;878:		}
;879:#endif
;880:		damage = damage * max / 100;
ADDRFP4 20
ADDRFP4 20
INDIRI4
ADDRLP4 20
INDIRI4
MULI4
CNSTI4 100
DIVI4
ASGNI4
line 881
;881:	}
LABELV $293
line 883
;882:
;883:	client = targ->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 885
;884:
;885:	if ( client ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $295
line 886
;886:		if ( client->noclip ) {
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $297
line 887
;887:			return;
ADDRGP4 $277
JUMPV
LABELV $297
line 889
;888:		}
;889:	}
LABELV $295
line 891
;890:
;891:	if ( !dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $299
line 892
;892:		dflags |= DAMAGE_NO_KNOCKBACK;
ADDRFP4 24
ADDRFP4 24
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 893
;893:	} else {
ADDRGP4 $300
JUMPV
LABELV $299
line 894
;894:		VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 895
;895:	}
LABELV $300
line 897
;896:
;897:	knockback = damage;
ADDRLP4 4
ADDRFP4 20
INDIRI4
ASGNI4
line 898
;898:	if ( knockback > 200 ) {
ADDRLP4 4
INDIRI4
CNSTI4 200
LEI4 $301
line 899
;899:		knockback = 200;
ADDRLP4 4
CNSTI4 200
ASGNI4
line 900
;900:	}
LABELV $301
line 901
;901:	if ( targ->flags & FL_NO_KNOCKBACK ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $303
line 902
;902:		knockback = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 903
;903:	}
LABELV $303
line 904
;904:	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
ADDRFP4 24
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $305
line 905
;905:		knockback = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 906
;906:	}
LABELV $305
line 909
;907:
;908:	// figure momentum add, even if the damage won't be taken
;909:	if ( knockback && targ->client ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $307
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $307
line 913
;910:		vec3_t	kvel;
;911:		float	mass;
;912:
;913:		mass = 200;
ADDRLP4 44
CNSTF4 1128792064
ASGNF4
line 915
;914:
;915:		VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
ADDRLP4 48
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 56
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 32
ADDRLP4 48
INDIRP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ADDRLP4 56
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 32+4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ADDRLP4 56
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 32+8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
ADDRLP4 44
INDIRF4
DIVF4
MULF4
ASGNF4
line 916
;916:		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDF4
ASGNF4
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 32+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
ADDRLP4 32+8
INDIRF4
ADDF4
ASGNF4
line 920
;917:
;918:		// set the timer so that the other client can't cancel
;919:		// out the movement immediately
;920:		if ( !targ->client->ps.pm_time ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
NEI4 $316
line 923
;921:			int		t;
;922:
;923:			t = knockback * 2;
ADDRLP4 72
ADDRLP4 4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 924
;924:			if ( t < 50 ) {
ADDRLP4 72
INDIRI4
CNSTI4 50
GEI4 $318
line 925
;925:				t = 50;
ADDRLP4 72
CNSTI4 50
ASGNI4
line 926
;926:			}
LABELV $318
line 927
;927:			if ( t > 200 ) {
ADDRLP4 72
INDIRI4
CNSTI4 200
LEI4 $320
line 928
;928:				t = 200;
ADDRLP4 72
CNSTI4 200
ASGNI4
line 929
;929:			}
LABELV $320
line 930
;930:			targ->client->ps.pm_time = t;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 72
INDIRI4
ASGNI4
line 931
;931:			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 76
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 932
;932:		}
LABELV $316
line 933
;933:	}
LABELV $307
line 936
;934:
;935:	// check for completely getting out of the damage
;936:	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {
ADDRFP4 24
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $322
line 943
;937:
;938:		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
;939:		// if the attacker was on the same team
;940:#ifdef MISSIONPACK
;941:		if ( mod != MOD_JUICED && targ != attacker && !(dflags & DAMAGE_NO_TEAM_PROTECTION) && OnSameTeam (targ, attacker)  ) {
;942:#else	
;943:		if ( targ != attacker && OnSameTeam (targ, attacker)  ) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CVPU4 4
ADDRLP4 36
INDIRP4
CVPU4 4
EQU4 $324
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $324
line 945
;944:#endif
;945:			if ( !g_friendlyFire.integer ) {
ADDRGP4 g_friendlyFire+12
INDIRI4
CNSTI4 0
NEI4 $326
line 946
;946:				return;
ADDRGP4 $277
JUMPV
LABELV $326
line 948
;947:			}
;948:		}
LABELV $324
line 961
;949:#ifdef MISSIONPACK
;950:		if (mod == MOD_PROXIMITY_MINE) {
;951:			if (inflictor && inflictor->parent && OnSameTeam(targ, inflictor->parent)) {
;952:				return;
;953:			}
;954:			if (targ == attacker) {
;955:				return;
;956:			}
;957:		}
;958:#endif
;959:
;960:		// check for godmode
;961:		if ( targ->flags & FL_GODMODE ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $329
line 962
;962:			return;
ADDRGP4 $277
JUMPV
LABELV $329
line 964
;963:		}
;964:	}
LABELV $322
line 968
;965:
;966:	// battlesuit protects from all radius damage (but takes knockback)
;967:	// and protects 50% against all damage
;968:	if ( client && client->ps.powerups[PW_BATTLESUIT] ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $331
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
INDIRI4
CNSTI4 0
EQI4 $331
line 969
;969:		G_AddEvent( targ, EV_POWERUP_BATTLESUIT, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 62
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 970
;970:		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) ) {
ADDRFP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $335
ADDRFP4 28
INDIRI4
CNSTI4 19
NEI4 $333
LABELV $335
line 971
;971:			return;
ADDRGP4 $277
JUMPV
LABELV $333
line 973
;972:		}
;973:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 974
;974:	}
LABELV $331
line 977
;975:
;976:	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
;977:	if ( attacker->client && targ != attacker && targ->health > 0
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $336
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CVPU4 4
ADDRLP4 36
INDIRP4
CVPU4 4
EQU4 $336
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
LEI4 $336
ADDRLP4 48
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 3
EQI4 $336
ADDRLP4 48
INDIRI4
ADDRLP4 44
INDIRI4
EQI4 $336
line 979
;978:			&& targ->s.eType != ET_MISSILE
;979:			&& targ->s.eType != ET_GENERAL) {
line 980
;980:		if ( OnSameTeam( targ, attacker ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $338
line 981
;981:			attacker->client->ps.persistant[PERS_HITS]--;
ADDRLP4 56
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 252
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 982
;982:		} else {
ADDRGP4 $339
JUMPV
LABELV $338
line 983
;983:			attacker->client->ps.persistant[PERS_HITS]++;
ADDRLP4 56
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 252
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 984
;984:		}
LABELV $339
line 985
;985:		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (targ->health<<8)|(client->ps.stats[STAT_ARMOR]);
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 8
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
BORI4
ASGNI4
line 986
;986:	}
LABELV $336
line 990
;987:
;988:	// always give half damage if hurting self
;989:	// calculated after knockback, so rocket jumping works
;990:	if ( targ == attacker) {
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
NEU4 $340
line 991
;991:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 992
;992:	}
LABELV $340
line 994
;993:
;994:	if ( damage < 1 ) {
ADDRFP4 20
INDIRI4
CNSTI4 1
GEI4 $342
line 995
;995:		damage = 1;
ADDRFP4 20
CNSTI4 1
ASGNI4
line 996
;996:	}
LABELV $342
line 997
;997:	take = damage;
ADDRLP4 8
ADDRFP4 20
INDIRI4
ASGNI4
line 998
;998:	save = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 1001
;999:
;1000:	// save some from armor
;1001:	asave = CheckArmor (targ, take, dflags);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
ARGI4
ADDRLP4 52
ADDRGP4 CheckArmor
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 52
INDIRI4
ASGNI4
line 1002
;1002:	take -= asave;
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
line 1004
;1003:
;1004:	if ( g_debugDamage.integer ) {
ADDRGP4 g_debugDamage+12
INDIRI4
CNSTI4 0
EQI4 $344
line 1005
;1005:		G_Printf( "%i: client:%i health:%i damage:%i armor:%i\n", level.time, targ->s.number,
ADDRGP4 $347
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
INDIRI4
ARGI4
ADDRLP4 56
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 1007
;1006:			targ->health, take, asave );
;1007:	}
LABELV $344
line 1012
;1008:
;1009:	// add to the damage inflicted on a player this frame
;1010:	// the total will be turned into screen blends and view angle kicks
;1011:	// at the end of the frame
;1012:	if ( client ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $349
line 1013
;1013:		if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $351
line 1014
;1014:			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 272
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1015
;1015:		} else {
ADDRGP4 $352
JUMPV
LABELV $351
line 1016
;1016:			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
ADDRLP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 1022
ASGNI4
line 1017
;1017:		}
LABELV $352
line 1018
;1018:		client->damage_armor += asave;
ADDRLP4 56
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
ASGNI4
line 1019
;1019:		client->damage_blood += take;
ADDRLP4 60
ADDRLP4 0
INDIRP4
CNSTI4 684
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 1020
;1020:		client->damage_knockback += knockback;
ADDRLP4 64
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 1021
;1021:		if ( dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $353
line 1022
;1022:			VectorCopy ( dir, client->damage_from );
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRFP4 12
INDIRP4
INDIRB
ASGNB 12
line 1023
;1023:			client->damage_fromWorld = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 0
ASGNI4
line 1024
;1024:		} else {
ADDRGP4 $354
JUMPV
LABELV $353
line 1025
;1025:			VectorCopy ( targ->r.currentOrigin, client->damage_from );
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRB
ASGNB 12
line 1026
;1026:			client->damage_fromWorld = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 1
ASGNI4
line 1027
;1027:		}
LABELV $354
line 1028
;1028:	}
LABELV $349
line 1034
;1029:
;1030:	// See if it's the player hurting the emeny flag carrier
;1031:#ifdef MISSIONPACK
;1032:	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_1FCTF ) {
;1033:#else	
;1034:	if( g_gametype.integer == GT_CTF) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $355
line 1036
;1035:#endif
;1036:		Team_CheckHurtCarrier(targ, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_CheckHurtCarrier
CALLV
pop
line 1037
;1037:	}
LABELV $355
line 1039
;1038:
;1039:	if (targ->client) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $358
line 1041
;1040:		// set the last client who damaged the target
;1041:		targ->client->lasthurt_client = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1042
;1042:		targ->client->lasthurt_mod = mod;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 28
INDIRI4
ASGNI4
line 1043
;1043:	}
LABELV $358
line 1046
;1044:
;1045:	// do the damage
;1046:	if (take) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $360
line 1047
;1047:		targ->health = targ->health - take;
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ASGNI4
line 1048
;1048:		if ( targ->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $362
line 1049
;1049:			targ->client->ps.stats[STAT_HEALTH] = targ->health;
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 60
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1050
;1050:		}
LABELV $362
line 1052
;1051:			
;1052:		if ( targ->health <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
GTI4 $364
line 1053
;1053:			if ( client )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $366
line 1054
;1054:				targ->flags |= FL_NO_KNOCKBACK;
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
CNSTI4 2048
BORI4
ASGNI4
LABELV $366
line 1056
;1055:
;1056:			if (targ->health < -999)
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 -999
GEI4 $368
line 1057
;1057:				targ->health = -999;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTI4 -999
ASGNI4
LABELV $368
line 1059
;1058:
;1059:			targ->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 1060
;1060:			targ->die (targ, inflictor, attacker, take, mod);
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRFP4 28
INDIRI4
ARGI4
ADDRLP4 64
INDIRP4
CNSTI4 716
ADDP4
INDIRP4
CALLV
pop
line 1061
;1061:			return;
ADDRGP4 $277
JUMPV
LABELV $364
line 1062
;1062:		} else if ( targ->pain ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $370
line 1063
;1063:			targ->pain (targ, attacker, take);
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 712
ADDP4
INDIRP4
CALLV
pop
line 1064
;1064:		}
LABELV $370
line 1065
;1065:	}
LABELV $360
line 1067
;1066:
;1067:}
LABELV $277
endproc G_Damage 80 24
export CanDamage
proc CanDamage 112 28
line 1078
;1068:
;1069:
;1070:/*
;1071:============
;1072:CanDamage
;1073:
;1074:Returns qtrue if the inflictor can directly damage the target.  Used for
;1075:explosions and melee attacks.
;1076:============
;1077:*/
;1078:qboolean CanDamage (gentity_t *targ, vec3_t origin) {
line 1085
;1079:	vec3_t	dest;
;1080:	trace_t	tr;
;1081:	vec3_t	midpoint;
;1082:
;1083:	// use the midpoint of the bounds instead of the origin, because
;1084:	// bmodels may have their origin is 0,0,0
;1085:	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 80
INDIRP4
CNSTI4 464
ADDP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 476
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 80
INDIRP4
CNSTI4 468
ADDP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 480
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12+8
ADDRLP4 84
INDIRP4
CNSTI4 472
ADDP4
INDIRF4
ADDRLP4 84
INDIRP4
CNSTI4 484
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1086
;1086:	VectorScale (midpoint, 0.5, midpoint);
ADDRLP4 88
CNSTF4 1056964608
ASGNF4
ADDRLP4 12
ADDRLP4 88
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 88
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
CNSTF4 1056964608
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 1088
;1087:
;1088:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1089
;1089:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1090
;1090:	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
EQF4 $383
ADDRLP4 24+52
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $379
LABELV $383
line 1091
;1091:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $372
JUMPV
LABELV $379
line 1095
;1092:
;1093:	// this should probably check in the plane of projection, 
;1094:	// rather than in world coordinate, and also include Z
;1095:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1096
;1096:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1097
;1097:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1098
;1098:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 96
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 96
INDIRP4
ARGP4
ADDRLP4 96
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1099
;1099:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $385
line 1100
;1100:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $372
JUMPV
LABELV $385
line 1102
;1101:
;1102:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1103
;1103:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1104
;1104:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1105
;1105:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 100
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1106
;1106:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $389
line 1107
;1107:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $372
JUMPV
LABELV $389
line 1109
;1108:
;1109:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1110
;1110:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1111
;1111:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 1112
;1112:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 104
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1113
;1113:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $393
line 1114
;1114:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $372
JUMPV
LABELV $393
line 1116
;1115:
;1116:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 1117
;1117:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1118
;1118:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 1119
;1119:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 108
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1120
;1120:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $397
line 1121
;1121:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $372
JUMPV
LABELV $397
line 1124
;1122:
;1123:
;1124:	return qfalse;
CNSTI4 0
RETI4
LABELV $372
endproc CanDamage 112 28
export G_RadiusDamage
proc G_RadiusDamage 4196 32
line 1134
;1125:}
;1126:
;1127:
;1128:/*
;1129:============
;1130:G_RadiusDamage
;1131:============
;1132:*/
;1133:qboolean G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
;1134:					 gentity_t *ignore, int mod) {
line 1143
;1135:	float		points, dist;
;1136:	gentity_t	*ent;
;1137:	int			entityList[MAX_GENTITIES];
;1138:	int			numListedEntities;
;1139:	vec3_t		mins, maxs;
;1140:	vec3_t		v;
;1141:	vec3_t		dir;
;1142:	int			i, e;
;1143:	qboolean	hitClient = qfalse;
ADDRLP4 4168
CNSTI4 0
ASGNI4
line 1145
;1144:
;1145:	if ( radius < 1 ) {
ADDRFP4 12
INDIRF4
CNSTF4 1065353216
GEF4 $401
line 1146
;1146:		radius = 1;
ADDRFP4 12
CNSTF4 1065353216
ASGNF4
line 1147
;1147:	}
LABELV $401
line 1149
;1148:
;1149:	for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $403
line 1150
;1150:		mins[i] = origin[i] - radius;
ADDRLP4 4172
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4172
INDIRI4
ADDRLP4 4144
ADDP4
ADDRLP4 4172
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
SUBF4
ASGNF4
line 1151
;1151:		maxs[i] = origin[i] + radius;
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRLP4 4156
ADDP4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ASGNF4
line 1152
;1152:	}
LABELV $404
line 1149
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $403
line 1154
;1153:
;1154:	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
ADDRLP4 4144
ARGP4
ADDRLP4 4156
ARGP4
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4172
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4140
ADDRLP4 4172
INDIRI4
ASGNI4
line 1156
;1155:
;1156:	for ( e = 0 ; e < numListedEntities ; e++ ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRGP4 $410
JUMPV
LABELV $407
line 1157
;1157:		ent = &g_entities[entityList[ e ]];
ADDRLP4 4
CNSTI4 808
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 44
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1159
;1158:
;1159:		if (ent == ignore)
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRFP4 16
INDIRP4
CVPU4 4
NEU4 $411
line 1160
;1160:			continue;
ADDRGP4 $408
JUMPV
LABELV $411
line 1161
;1161:		if (!ent->takedamage)
ADDRLP4 4
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 0
NEI4 $413
line 1162
;1162:			continue;
ADDRGP4 $408
JUMPV
LABELV $413
line 1165
;1163:
;1164:		// find the distance from the edge of the bounding box
;1165:		for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $415
line 1166
;1166:			if ( origin[i] < ent->r.absmin[i] ) {
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4176
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 464
ADDP4
ADDP4
INDIRF4
GEF4 $419
line 1167
;1167:				v[i] = ent->r.absmin[i] - origin[i];
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 464
ADDP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1168
;1168:			} else if ( origin[i] > ent->r.absmax[i] ) {
ADDRGP4 $420
JUMPV
LABELV $419
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRF4
LEF4 $421
line 1169
;1169:				v[i] = origin[i] - ent->r.absmax[i];
ADDRLP4 4184
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4184
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4184
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4184
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 476
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1170
;1170:			} else {
ADDRGP4 $422
JUMPV
LABELV $421
line 1171
;1171:				v[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTF4 0
ASGNF4
line 1172
;1172:			}
LABELV $422
LABELV $420
line 1173
;1173:		}
LABELV $416
line 1165
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $415
line 1175
;1174:
;1175:		dist = VectorLength( v );
ADDRLP4 8
ARGP4
ADDRLP4 4176
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 4176
INDIRF4
ASGNF4
line 1176
;1176:		if ( dist >= radius ) {
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
LTF4 $423
line 1177
;1177:			continue;
ADDRGP4 $408
JUMPV
LABELV $423
line 1180
;1178:		}
;1179:
;1180:		points = damage * ( 1.0 - dist / radius );
ADDRLP4 40
ADDRFP4 8
INDIRF4
CNSTF4 1065353216
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
DIVF4
SUBF4
MULF4
ASGNF4
line 1182
;1181:
;1182:		if( CanDamage (ent, origin) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4180
ADDRGP4 CanDamage
CALLI4
ASGNI4
ADDRLP4 4180
INDIRI4
CNSTI4 0
EQI4 $425
line 1183
;1183:			if( LogAccuracyHit( ent, attacker ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4184
ADDRGP4 LogAccuracyHit
CALLI4
ASGNI4
ADDRLP4 4184
INDIRI4
CNSTI4 0
EQI4 $427
line 1184
;1184:				hitClient = qtrue;
ADDRLP4 4168
CNSTI4 1
ASGNI4
line 1185
;1185:			}
LABELV $427
line 1186
;1186:			VectorSubtract (ent->r.currentOrigin, origin, dir);
ADDRLP4 4192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 4
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 4
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+8
ADDRLP4 4
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1189
;1187:			// push the center of mass higher than the origin so players
;1188:			// get knocked into the air more
;1189:			dir[2] += 24;
ADDRLP4 28+8
ADDRLP4 28+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 1190
;1190:			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
ADDRLP4 4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
INDIRF4
CVFI4 4
ARGI4
CNSTI4 1
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 1191
;1191:		}
LABELV $425
line 1192
;1192:	}
LABELV $408
line 1156
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $410
ADDRLP4 20
INDIRI4
ADDRLP4 4140
INDIRI4
LTI4 $407
line 1194
;1193:
;1194:	return hitClient;
ADDRLP4 4168
INDIRI4
RETI4
LABELV $400
endproc G_RadiusDamage 4196 32
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
import G_InvulnerabilityEffect
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
LABELV $347
byte 1 37
byte 1 105
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 100
byte 1 97
byte 1 109
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 37
byte 1 105
byte 1 32
byte 1 97
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 58
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $205
byte 1 75
byte 1 105
byte 1 108
byte 1 108
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 98
byte 1 121
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $204
byte 1 60
byte 1 98
byte 1 97
byte 1 100
byte 1 32
byte 1 111
byte 1 98
byte 1 105
byte 1 116
byte 1 117
byte 1 97
byte 1 114
byte 1 121
byte 1 62
byte 1 0
align 1
LABELV $197
byte 1 60
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 62
byte 1 0
align 1
LABELV $196
byte 1 60
byte 1 110
byte 1 111
byte 1 110
byte 1 45
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 62
byte 1 0
align 1
LABELV $175
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 111
byte 1 98
byte 1 101
byte 1 108
byte 1 105
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $174
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 111
byte 1 98
byte 1 101
byte 1 108
byte 1 105
byte 1 115
byte 1 107
byte 1 0
align 1
LABELV $154
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $153
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 67
byte 1 84
byte 1 70
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $142
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 65
byte 1 80
byte 1 80
byte 1 76
byte 1 69
byte 1 0
align 1
LABELV $141
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 82
byte 1 73
byte 1 71
byte 1 71
byte 1 69
byte 1 82
byte 1 95
byte 1 72
byte 1 85
byte 1 82
byte 1 84
byte 1 0
align 1
LABELV $140
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 65
byte 1 82
byte 1 71
byte 1 69
byte 1 84
byte 1 95
byte 1 76
byte 1 65
byte 1 83
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $139
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 85
byte 1 73
byte 1 67
byte 1 73
byte 1 68
byte 1 69
byte 1 0
align 1
LABELV $138
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 70
byte 1 65
byte 1 76
byte 1 76
byte 1 73
byte 1 78
byte 1 71
byte 1 0
align 1
LABELV $137
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 84
byte 1 69
byte 1 76
byte 1 69
byte 1 70
byte 1 82
byte 1 65
byte 1 71
byte 1 0
align 1
LABELV $136
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 67
byte 1 82
byte 1 85
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $135
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 76
byte 1 65
byte 1 86
byte 1 65
byte 1 0
align 1
LABELV $134
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 76
byte 1 73
byte 1 77
byte 1 69
byte 1 0
align 1
LABELV $133
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 87
byte 1 65
byte 1 84
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $132
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 66
byte 1 70
byte 1 71
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $131
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 66
byte 1 70
byte 1 71
byte 1 0
align 1
LABELV $130
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 76
byte 1 73
byte 1 71
byte 1 72
byte 1 84
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 0
align 1
LABELV $129
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 65
byte 1 73
byte 1 76
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $128
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 77
byte 1 65
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $127
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 77
byte 1 65
byte 1 0
align 1
LABELV $126
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 79
byte 1 67
byte 1 75
byte 1 69
byte 1 84
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $125
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 82
byte 1 79
byte 1 67
byte 1 75
byte 1 69
byte 1 84
byte 1 0
align 1
LABELV $124
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 68
byte 1 69
byte 1 95
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 83
byte 1 72
byte 1 0
align 1
LABELV $123
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 68
byte 1 69
byte 1 0
align 1
LABELV $122
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 77
byte 1 65
byte 1 67
byte 1 72
byte 1 73
byte 1 78
byte 1 69
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $121
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 71
byte 1 65
byte 1 85
byte 1 78
byte 1 84
byte 1 76
byte 1 69
byte 1 84
byte 1 0
align 1
LABELV $120
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 83
byte 1 72
byte 1 79
byte 1 84
byte 1 71
byte 1 85
byte 1 78
byte 1 0
align 1
LABELV $119
byte 1 77
byte 1 79
byte 1 68
byte 1 95
byte 1 85
byte 1 78
byte 1 75
byte 1 78
byte 1 79
byte 1 87
byte 1 78
byte 1 0
align 1
LABELV $112
byte 1 107
byte 1 97
byte 1 109
byte 1 105
byte 1 107
byte 1 97
byte 1 122
byte 1 101
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
