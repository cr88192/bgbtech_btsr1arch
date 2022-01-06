export P_DamageFeedback
code
proc P_DamageFeedback 36 12
file "../g_active.c"
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
;23:
;24:#include "g_local.h"
;25:
;26:
;27:/*
;28:===============
;29:G_DamageFeedback
;30:
;31:Called just before a snapshot is sent to the given player.
;32:Totals up all damage and generates both the player_state_t
;33:damage values to that client for pain blends and kicks, and
;34:global pain sound events for all clients.
;35:===============
;36:*/
;37:void P_DamageFeedback( gentity_t *player ) {
line 42
;38:	gclient_t	*client;
;39:	float	count;
;40:	vec3_t	angles;
;41:
;42:	client = player->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 43
;43:	if ( client->ps.pm_type == PM_DEAD ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $53
line 44
;44:		return;
ADDRGP4 $52
JUMPV
LABELV $53
line 48
;45:	}
;46:
;47:	// total points of damage shot at the player this frame
;48:	count = client->damage_blood + client->damage_armor;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 684
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDI4
CVIF4 4
ASGNF4
line 49
;49:	if ( count == 0 ) {
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $55
line 50
;50:		return;		// didn't take any damage
ADDRGP4 $52
JUMPV
LABELV $55
line 53
;51:	}
;52:
;53:	if ( count > 255 ) {
ADDRLP4 4
INDIRF4
CNSTF4 1132396544
LEF4 $57
line 54
;54:		count = 255;
ADDRLP4 4
CNSTF4 1132396544
ASGNF4
line 55
;55:	}
LABELV $57
line 61
;56:
;57:	// send the information to the client
;58:
;59:	// world damage (falling, slime, etc) uses a special code
;60:	// to make the blend blob centered instead of positional
;61:	if ( client->damage_fromWorld ) {
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRI4
CNSTI4 0
EQI4 $59
line 62
;62:		client->ps.damagePitch = 255;
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTI4 255
ASGNI4
line 63
;63:		client->ps.damageYaw = 255;
ADDRLP4 0
INDIRP4
CNSTI4 172
ADDP4
CNSTI4 255
ASGNI4
line 65
;64:
;65:		client->damage_fromWorld = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 0
ASGNI4
line 66
;66:	} else {
ADDRGP4 $60
JUMPV
LABELV $59
line 67
;67:		vectoangles( client->damage_from, angles );
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 68
;68:		client->ps.damagePitch = angles[PITCH]/360.0 * 256;
ADDRLP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTF4 1132462080
ADDRLP4 8
INDIRF4
CNSTF4 1135869952
DIVF4
MULF4
CVFI4 4
ASGNI4
line 69
;69:		client->ps.damageYaw = angles[YAW]/360.0 * 256;
ADDRLP4 0
INDIRP4
CNSTI4 172
ADDP4
CNSTF4 1132462080
ADDRLP4 8+4
INDIRF4
CNSTF4 1135869952
DIVF4
MULF4
CVFI4 4
ASGNI4
line 70
;70:	}
LABELV $60
line 73
;71:
;72:	// play an apropriate pain sound
;73:	if ( (level.time > player->pain_debounce_time) && !(player->flags & FL_GODMODE) ) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 720
ADDP4
INDIRI4
LEI4 $62
ADDRLP4 24
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $62
line 74
;74:		player->pain_debounce_time = level.time + 700;
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 700
ADDI4
ASGNI4
line 75
;75:		G_AddEvent( player, EV_PAIN, player->health );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
CNSTI4 56
ARGI4
ADDRLP4 28
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 76
;76:		client->ps.damageEvent++;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 168
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 77
;77:	}
LABELV $62
line 80
;78:
;79:
;80:	client->ps.damageCount = count;
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 85
;81:
;82:	//
;83:	// clear totals
;84:	//
;85:	client->damage_blood = 0;
ADDRLP4 0
INDIRP4
CNSTI4 684
ADDP4
CNSTI4 0
ASGNI4
line 86
;86:	client->damage_armor = 0;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 87
;87:	client->damage_knockback = 0;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 0
ASGNI4
line 88
;88:}
LABELV $52
endproc P_DamageFeedback 36 12
export P_WorldEffects
proc P_WorldEffects 40 32
line 99
;89:
;90:
;91:
;92:/*
;93:=============
;94:P_WorldEffects
;95:
;96:Check for lava / slime contents and drowning
;97:=============
;98:*/
;99:void P_WorldEffects( gentity_t *ent ) {
line 103
;100:	qboolean	envirosuit;
;101:	int			waterlevel;
;102:
;103:	if ( ent->client->noclip ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $67
line 104
;104:		ent->client->airOutTime = level.time + 12000;	// don't need air
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 105
;105:		return;
ADDRGP4 $66
JUMPV
LABELV $67
line 108
;106:	}
;107:
;108:	waterlevel = ent->waterlevel;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
ASGNI4
line 110
;109:
;110:	envirosuit = ent->client->ps.powerups[PW_BATTLESUIT] > level.time;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 320
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $72
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRGP4 $73
JUMPV
LABELV $72
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $73
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 115
;111:
;112:	//
;113:	// check for drowning
;114:	//
;115:	if ( waterlevel == 3 ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
NEI4 $74
line 117
;116:		// envirosuit give air
;117:		if ( envirosuit ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $76
line 118
;118:			ent->client->airOutTime = level.time + 10000;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 119
;119:		}
LABELV $76
line 122
;120:
;121:		// if out of air, start drowning
;122:		if ( ent->client->airOutTime < level.time) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 748
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $75
line 124
;123:			// drown!
;124:			ent->client->airOutTime += 1000;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 748
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 125
;125:			if ( ent->health > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
LEI4 $75
line 127
;126:				// take more damage the longer underwater
;127:				ent->damage += 2;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 2
ADDI4
ASGNI4
line 128
;128:				if (ent->damage > 15)
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
CNSTI4 15
LEI4 $84
line 129
;129:					ent->damage = 15;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 15
ASGNI4
LABELV $84
line 132
;130:
;131:				// play a gurp sound instead of a normal pain sound
;132:				if (ent->health <= ent->damage) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
GTI4 $86
line 133
;133:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("*drown.wav"));
ADDRGP4 $88
ARGP4
ADDRLP4 24
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 134
;134:				} else if (rand()&1) {
ADDRGP4 $87
JUMPV
LABELV $86
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $89
line 135
;135:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp1.wav"));
ADDRGP4 $91
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 136
;136:				} else {
ADDRGP4 $90
JUMPV
LABELV $89
line 137
;137:					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp2.wav"));
ADDRGP4 $92
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 138
;138:				}
LABELV $90
LABELV $87
line 141
;139:
;140:				// don't play a normal pain sound
;141:				ent->pain_debounce_time = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 143
;142:
;143:				G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
CNSTP4 0
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 36
CNSTP4 0
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
CNSTI4 14
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 145
;144:					ent->damage, DAMAGE_NO_ARMOR, MOD_WATER);
;145:			}
line 146
;146:		}
line 147
;147:	} else {
ADDRGP4 $75
JUMPV
LABELV $74
line 148
;148:		ent->client->airOutTime = level.time + 12000;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 748
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 149
;149:		ent->damage = 2;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 2
ASGNI4
line 150
;150:	}
LABELV $75
line 155
;151:
;152:	//
;153:	// check for sizzle damage (move to pmove?)
;154:	//
;155:	if (waterlevel && 
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $95
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 24
BANDI4
ADDRLP4 12
INDIRI4
EQI4 $95
line 156
;156:		(ent->watertype&(CONTENTS_LAVA|CONTENTS_SLIME)) ) {
line 157
;157:		if (ent->health > 0
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CNSTI4 0
LEI4 $97
ADDRLP4 16
INDIRP4
CNSTI4 720
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GTI4 $97
line 158
;158:			&& ent->pain_debounce_time <= level.time	) {
line 160
;159:
;160:			if ( envirosuit ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $100
line 161
;161:				G_AddEvent( ent, EV_POWERUP_BATTLESUIT, 0 );
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
line 162
;162:			} else {
ADDRGP4 $101
JUMPV
LABELV $100
line 163
;163:				if (ent->watertype & CONTENTS_LAVA) {
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $102
line 164
;164:					G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTI4 30
ADDRLP4 0
INDIRI4
MULI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 16
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 166
;165:						30*waterlevel, 0, MOD_LAVA);
;166:				}
LABELV $102
line 168
;167:
;168:				if (ent->watertype & CONTENTS_SLIME) {
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $104
line 169
;169:					G_Damage (ent, NULL, NULL, NULL, NULL, 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
CNSTP4 0
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTI4 10
ADDRLP4 0
INDIRI4
MULI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 15
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 171
;170:						10*waterlevel, 0, MOD_SLIME);
;171:				}
LABELV $104
line 172
;172:			}
LABELV $101
line 173
;173:		}
LABELV $97
line 174
;174:	}
LABELV $95
line 175
;175:}
LABELV $66
endproc P_WorldEffects 40 32
export G_SetClientSound
proc G_SetClientSound 8 0
line 184
;176:
;177:
;178:
;179:/*
;180:===============
;181:G_SetClientSound
;182:===============
;183:*/
;184:void G_SetClientSound( gentity_t *ent ) {
line 191
;185:#ifdef MISSIONPACK
;186:	if( ent->s.eFlags & EF_TICKING ) {
;187:		ent->client->ps.loopSound = G_SoundIndex( "sound/weapons/proxmine/wstbtick.wav");
;188:	}
;189:	else
;190:#endif
;191:	if (ent->waterlevel && (ent->watertype&(CONTENTS_LAVA|CONTENTS_SLIME)) ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $107
ADDRLP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 24
BANDI4
ADDRLP4 4
INDIRI4
EQI4 $107
line 192
;192:		ent->client->ps.loopSound = level.snd_fry;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 444
ADDP4
ADDRGP4 level+348
INDIRI4
ASGNI4
line 193
;193:	} else {
ADDRGP4 $108
JUMPV
LABELV $107
line 194
;194:		ent->client->ps.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 444
ADDP4
CNSTI4 0
ASGNI4
line 195
;195:	}
LABELV $108
line 196
;196:}
LABELV $106
endproc G_SetClientSound 8 0
export ClientImpacts
proc ClientImpacts 76 12
line 207
;197:
;198:
;199:
;200://==============================================================
;201:
;202:/*
;203:==============
;204:ClientImpacts
;205:==============
;206:*/
;207:void ClientImpacts( gentity_t *ent, pmove_t *pm ) {
line 212
;208:	int		i, j;
;209:	trace_t	trace;
;210:	gentity_t	*other;
;211:
;212:	memset( &trace, 0, sizeof( trace ) );
ADDRLP4 12
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
line 213
;213:	for (i=0 ; i<pm->numtouch ; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $114
JUMPV
LABELV $111
line 214
;214:		for (j=0 ; j<i ; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $118
JUMPV
LABELV $115
line 215
;215:			if (pm->touchents[j] == pm->touchents[i] ) {
ADDRLP4 68
CNSTI4 2
ASGNI4
ADDRLP4 72
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
LSHI4
ADDRLP4 72
INDIRP4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
LSHI4
ADDRLP4 72
INDIRP4
ADDP4
INDIRI4
NEI4 $119
line 216
;216:				break;
ADDRGP4 $117
JUMPV
LABELV $119
line 218
;217:			}
;218:		}
LABELV $116
line 214
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $118
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $115
LABELV $117
line 219
;219:		if (j != i) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $121
line 220
;220:			continue;	// duplicated
ADDRGP4 $112
JUMPV
LABELV $121
line 222
;221:		}
;222:		other = &g_entities[ pm->touchents[i] ];
ADDRLP4 8
CNSTI4 808
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 224
;223:
;224:		if ( ( ent->r.svFlags & SVF_BOT ) && ( ent->touch ) ) {
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $123
ADDRLP4 68
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $123
line 225
;225:			ent->touch( ent, other, &trace );
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 72
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 226
;226:		}
LABELV $123
line 228
;227:
;228:		if ( !other->touch ) {
ADDRLP4 8
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $125
line 229
;229:			continue;
ADDRGP4 $112
JUMPV
LABELV $125
line 232
;230:		}
;231:
;232:		other->touch( other, ent, &trace );
ADDRLP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 233
;233:	}
LABELV $112
line 213
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $114
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
LTI4 $111
line 235
;234:
;235:}
LABELV $110
endproc ClientImpacts 76 12
data
align 4
LABELV $128
byte 4 1109393408
byte 4 1109393408
byte 4 1112539136
export G_TouchTriggers
code
proc G_TouchTriggers 4240 16
line 245
;236:
;237:/*
;238:============
;239:G_TouchTriggers
;240:
;241:Find all trigger entities that ent's current position touches.
;242:Spectators will only interact with teleporters.
;243:============
;244:*/
;245:void	G_TouchTriggers( gentity_t *ent ) {
line 253
;246:	int			i, num;
;247:	int			touch[MAX_GENTITIES];
;248:	gentity_t	*hit;
;249:	trace_t		trace;
;250:	vec3_t		mins, maxs;
;251:	static vec3_t	range = { 40, 40, 52 };
;252:
;253:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $129
line 254
;254:		return;
ADDRGP4 $127
JUMPV
LABELV $129
line 258
;255:	}
;256:
;257:	// dead clients don't activate triggers!
;258:	if ( ent->client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $131
line 259
;259:		return;
ADDRGP4 $127
JUMPV
LABELV $131
line 262
;260:	}
;261:
;262:	VectorSubtract( ent->client->ps.origin, range, mins );
ADDRLP4 4188
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 64
ADDRLP4 4188
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRGP4 $128
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64+4
ADDRLP4 4188
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRGP4 $128+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRGP4 $128+8
INDIRF4
SUBF4
ASGNF4
line 263
;263:	VectorAdd( ent->client->ps.origin, range, maxs );
ADDRLP4 4192
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 76
ADDRLP4 4192
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRGP4 $128
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+4
ADDRLP4 4192
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRGP4 $128+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRGP4 $128+8
INDIRF4
ADDF4
ASGNF4
line 265
;264:
;265:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 64
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4196
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 88
ADDRLP4 4196
INDIRI4
ASGNI4
line 268
;266:
;267:	// can't use ent->absmin, because that has a one unit pad
;268:	VectorAdd( ent->client->ps.origin, ent->r.mins, mins );
ADDRLP4 4200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4204
ADDRLP4 4200
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 64
ADDRLP4 4204
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4200
INDIRP4
CNSTI4 436
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 64+4
ADDRLP4 4204
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4200
INDIRP4
CNSTI4 440
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4208
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64+8
ADDRLP4 4208
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4208
INDIRP4
CNSTI4 444
ADDP4
INDIRF4
ADDF4
ASGNF4
line 269
;269:	VectorAdd( ent->client->ps.origin, ent->r.maxs, maxs );
ADDRLP4 4212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4216
ADDRLP4 4212
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 76
ADDRLP4 4216
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4212
INDIRP4
CNSTI4 448
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 76+4
ADDRLP4 4216
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4212
INDIRP4
CNSTI4 452
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4220
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76+8
ADDRLP4 4220
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4220
INDIRP4
CNSTI4 456
ADDP4
INDIRF4
ADDF4
ASGNF4
line 271
;270:
;271:	for ( i=0 ; i<num ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $148
JUMPV
LABELV $145
line 272
;272:		hit = &g_entities[touch[i]];
ADDRLP4 0
CNSTI4 808
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 92
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 274
;273:
;274:		if ( !hit->touch && !ent->touch ) {
ADDRLP4 4224
CNSTI4 704
ASGNI4
ADDRLP4 4228
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
ADDRLP4 4224
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4228
INDIRU4
NEU4 $149
ADDRFP4 0
INDIRP4
ADDRLP4 4224
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4228
INDIRU4
NEU4 $149
line 275
;275:			continue;
ADDRGP4 $146
JUMPV
LABELV $149
line 277
;276:		}
;277:		if ( !( hit->r.contents & CONTENTS_TRIGGER ) ) {
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
CNSTI4 1073741824
BANDI4
CNSTI4 0
NEI4 $151
line 278
;278:			continue;
ADDRGP4 $146
JUMPV
LABELV $151
line 282
;279:		}
;280:
;281:		// ignore most entities if a spectator
;282:		if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $153
line 283
;283:			if ( hit->s.eType != ET_TELEPORT_TRIGGER &&
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 9
EQI4 $155
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 Touch_DoorTrigger
CVPU4 4
EQU4 $155
line 286
;284:				// this is ugly but adding a new ET_? type will
;285:				// most likely cause network incompatibilities
;286:				hit->touch != Touch_DoorTrigger) {
line 287
;287:				continue;
ADDRGP4 $146
JUMPV
LABELV $155
line 289
;288:			}
;289:		}
LABELV $153
line 293
;290:
;291:		// use seperate code for determining if an item is picked up
;292:		// so you don't have to actually contact its bounding box
;293:		if ( hit->s.eType == ET_ITEM ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
NEI4 $157
line 294
;294:			if ( !BG_PlayerTouchesItem( &ent->client->ps, &hit->s, level.time ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 4232
ADDRGP4 BG_PlayerTouchesItem
CALLI4
ASGNI4
ADDRLP4 4232
INDIRI4
CNSTI4 0
NEI4 $158
line 295
;295:				continue;
ADDRGP4 $146
JUMPV
line 297
;296:			}
;297:		} else {
LABELV $157
line 298
;298:			if ( !trap_EntityContact( mins, maxs, hit ) ) {
ADDRLP4 64
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4232
ADDRGP4 trap_EntityContact
CALLI4
ASGNI4
ADDRLP4 4232
INDIRI4
CNSTI4 0
NEI4 $162
line 299
;299:				continue;
ADDRGP4 $146
JUMPV
LABELV $162
line 301
;300:			}
;301:		}
LABELV $158
line 303
;302:
;303:		memset( &trace, 0, sizeof(trace) );
ADDRLP4 8
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
line 305
;304:
;305:		if ( hit->touch ) {
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $164
line 306
;306:			hit->touch (hit, ent, &trace);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 307
;307:		}
LABELV $164
line 309
;308:
;309:		if ( ( ent->r.svFlags & SVF_BOT ) && ( ent->touch ) ) {
ADDRLP4 4232
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4232
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $166
ADDRLP4 4232
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $166
line 310
;310:			ent->touch( ent, hit, &trace );
ADDRLP4 4236
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4236
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 4236
INDIRP4
CNSTI4 704
ADDP4
INDIRP4
CALLV
pop
line 311
;311:		}
LABELV $166
line 312
;312:	}
LABELV $146
line 271
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $148
ADDRLP4 4
INDIRI4
ADDRLP4 88
INDIRI4
LTI4 $145
line 315
;313:
;314:	// if we didn't touch a jump pad this pmove frame
;315:	if ( ent->client->ps.jumppad_frame != ent->client->ps.pmove_framecount ) {
ADDRLP4 4224
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4224
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
ADDRLP4 4224
INDIRP4
CNSTI4 456
ADDP4
INDIRI4
EQI4 $168
line 316
;316:		ent->client->ps.jumppad_frame = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 0
ASGNI4
line 317
;317:		ent->client->ps.jumppad_ent = 0;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 448
ADDP4
CNSTI4 0
ASGNI4
line 318
;318:	}
LABELV $168
line 319
;319:}
LABELV $127
endproc G_TouchTriggers 4240 16
export SpectatorThink
proc SpectatorThink 252 12
line 326
;320:
;321:/*
;322:=================
;323:SpectatorThink
;324:=================
;325:*/
;326:void SpectatorThink( gentity_t *ent, usercmd_t *ucmd ) {
line 330
;327:	pmove_t	pm;
;328:	gclient_t	*client;
;329:
;330:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 332
;331:
;332:	if ( client->sess.spectatorState != SPECTATOR_FOLLOW ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
EQI4 $171
line 333
;333:		client->ps.pm_type = PM_SPECTATOR;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 2
ASGNI4
line 334
;334:		client->ps.speed = 400;	// faster than normal
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 400
ASGNI4
line 337
;335:
;336:		// set up for pmove
;337:		memset (&pm, 0, sizeof(pm));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 232
ARGI4
ADDRGP4 memset
CALLP4
pop
line 338
;338:		pm.ps = &client->ps;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 339
;339:		pm.cmd = *ucmd;
ADDRLP4 4+4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 24
line 340
;340:		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;	// spectators can fly through bodies
ADDRLP4 4+28
CNSTI4 65537
ASGNI4
line 341
;341:		pm.trace = trap_Trace;
ADDRLP4 4+224
ADDRGP4 trap_Trace
ASGNP4
line 342
;342:		pm.pointcontents = trap_PointContents;
ADDRLP4 4+228
ADDRGP4 trap_PointContents
ASGNP4
line 345
;343:
;344:		// perform a pmove
;345:		Pmove (&pm);
ADDRLP4 4
ARGP4
ADDRGP4 Pmove
CALLV
pop
line 347
;346:		// save results of pmove
;347:		VectorCopy( client->ps.origin, ent->s.origin );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 349
;348:
;349:		G_TouchTriggers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_TouchTriggers
CALLV
pop
line 350
;350:		trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 351
;351:	}
LABELV $171
line 353
;352:
;353:	client->oldbuttons = client->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 354
;354:	client->buttons = ucmd->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 357
;355:
;356:	// attack button cycles through spectators
;357:	if ( ( client->buttons & BUTTON_ATTACK ) && ! ( client->oldbuttons & BUTTON_ATTACK ) ) {
ADDRLP4 244
CNSTI4 1
ASGNI4
ADDRLP4 248
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
BANDI4
ADDRLP4 248
INDIRI4
EQI4 $177
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
BANDI4
ADDRLP4 248
INDIRI4
NEI4 $177
line 358
;358:		Cmd_FollowCycle_f( ent, 1 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Cmd_FollowCycle_f
CALLV
pop
line 359
;359:	}
LABELV $177
line 360
;360:}
LABELV $170
endproc SpectatorThink 252 12
export ClientInactivityTimer
proc ClientInactivityTimer 12 8
line 371
;361:
;362:
;363:
;364:/*
;365:=================
;366:ClientInactivityTimer
;367:
;368:Returns qfalse if the client is dropped
;369:=================
;370:*/
;371:qboolean ClientInactivityTimer( gclient_t *client ) {
line 372
;372:	if ( ! g_inactivity.integer ) {
ADDRGP4 g_inactivity+12
INDIRI4
CNSTI4 0
NEI4 $180
line 375
;373:		// give everyone some time, so if the operator sets g_inactivity during
;374:		// gameplay, everyone isn't kicked
;375:		client->inactivityTime = level.time + 60 * 1000;
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 60000
ADDI4
ASGNI4
line 376
;376:		client->inactivityWarning = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 0
ASGNI4
line 377
;377:	} else if ( client->pers.cmd.forwardmove || 
ADDRGP4 $181
JUMPV
LABELV $180
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 493
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 494
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 495
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $188
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 4
INDIRI4
EQI4 $184
LABELV $188
line 380
;378:		client->pers.cmd.rightmove || 
;379:		client->pers.cmd.upmove ||
;380:		(client->pers.cmd.buttons & BUTTON_ATTACK) ) {
line 381
;381:		client->inactivityTime = level.time + g_inactivity.integer * 1000;
ADDRFP4 0
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
line 382
;382:		client->inactivityWarning = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 0
ASGNI4
line 383
;383:	} else if ( !client->pers.localClient ) {
ADDRGP4 $185
JUMPV
LABELV $184
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 0
NEI4 $191
line 384
;384:		if ( level.time > client->inactivityTime ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
LEI4 $193
line 385
;385:			trap_DropClient( client - level.clients, "Dropped due to inactivity" );
ADDRFP4 0
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
ADDRGP4 $196
ARGP4
ADDRGP4 trap_DropClient
CALLV
pop
line 386
;386:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $179
JUMPV
LABELV $193
line 388
;387:		}
;388:		if ( level.time > client->inactivityTime - 10000 && !client->inactivityWarning ) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 10000
SUBI4
LEI4 $197
ADDRLP4 8
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
CNSTI4 0
NEI4 $197
line 389
;389:			client->inactivityWarning = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 1
ASGNI4
line 390
;390:			trap_SendServerCommand( client - level.clients, "cp \"Ten seconds until inactivity drop!\n\"" );
ADDRFP4 0
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
ADDRGP4 $200
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 391
;391:		}
LABELV $197
line 392
;392:	}
LABELV $191
LABELV $185
LABELV $181
line 393
;393:	return qtrue;
CNSTI4 1
RETI4
LABELV $179
endproc ClientInactivityTimer 12 8
export ClientTimerActions
proc ClientTimerActions 20 12
line 403
;394:}
;395:
;396:/*
;397:==================
;398:ClientTimerActions
;399:
;400:Actions that happen once a second
;401:==================
;402:*/
;403:void ClientTimerActions( gentity_t *ent, int msec ) {
line 409
;404:	gclient_t	*client;
;405:#ifdef MISSIONPACK
;406:	int			maxHealth;
;407:#endif
;408:
;409:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 410
;410:	client->timeResidual += msec;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 768
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
ADDRGP4 $203
JUMPV
LABELV $202
line 412
;411:
;412:	while ( client->timeResidual >= 1000 ) {
line 413
;413:		client->timeResidual -= 1000;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 768
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1000
SUBI4
ASGNI4
line 441
;414:
;415:		// regenerate
;416:#ifdef MISSIONPACK
;417:		if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;418:			maxHealth = client->ps.stats[STAT_MAX_HEALTH] / 2;
;419:		}
;420:		else if ( client->ps.powerups[PW_REGEN] ) {
;421:			maxHealth = client->ps.stats[STAT_MAX_HEALTH];
;422:		}
;423:		else {
;424:			maxHealth = 0;
;425:		}
;426:		if( maxHealth ) {
;427:			if ( ent->health < maxHealth ) {
;428:				ent->health += 15;
;429:				if ( ent->health > maxHealth * 1.1 ) {
;430:					ent->health = maxHealth * 1.1;
;431:				}
;432:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
;433:			} else if ( ent->health < maxHealth * 2) {
;434:				ent->health += 5;
;435:				if ( ent->health > maxHealth * 2 ) {
;436:					ent->health = maxHealth * 2;
;437:				}
;438:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
;439:			}
;440:#else
;441:		if ( client->ps.powerups[PW_REGEN] ) {
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRI4
CNSTI4 0
EQI4 $205
line 442
;442:			if ( ent->health < client->ps.stats[STAT_MAX_HEALTH]) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
GEI4 $207
line 443
;443:				ent->health += 15;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 15
ADDI4
ASGNI4
line 444
;444:				if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] * 1.1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1066192077
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
MULF4
LEF4 $209
line 445
;445:					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 1.1;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTF4 1066192077
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 446
;446:				}
LABELV $209
line 447
;447:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 63
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 448
;448:			} else if ( ent->health < client->ps.stats[STAT_MAX_HEALTH] * 2) {
ADDRGP4 $206
JUMPV
LABELV $207
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
GEI4 $206
line 449
;449:				ent->health += 5;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 5
ADDI4
ASGNI4
line 450
;450:				if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] * 2 ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
LEI4 $213
line 451
;451:					ent->health = client->ps.stats[STAT_MAX_HEALTH] * 2;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 452
;452:				}
LABELV $213
line 453
;453:				G_AddEvent( ent, EV_POWERUP_REGEN, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 63
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 454
;454:			}
line 456
;455:#endif
;456:		} else {
ADDRGP4 $206
JUMPV
LABELV $205
line 458
;457:			// count down health when over max
;458:			if ( ent->health > client->ps.stats[STAT_MAX_HEALTH] ) {
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
LEI4 $215
line 459
;459:				ent->health--;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 460
;460:			}
LABELV $215
line 461
;461:		}
LABELV $206
line 464
;462:
;463:		// count down armor when over max
;464:		if ( client->ps.stats[STAT_ARMOR] > client->ps.stats[STAT_MAX_HEALTH] ) {
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
LEI4 $217
line 465
;465:			client->ps.stats[STAT_ARMOR]--;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 466
;466:		}
LABELV $217
line 467
;467:	}
LABELV $203
line 412
ADDRLP4 0
INDIRP4
CNSTI4 768
ADDP4
INDIRI4
CNSTI4 1000
GEI4 $202
line 506
;468:#ifdef MISSIONPACK
;469:	if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN ) {
;470:		int w, max, inc, t, i;
;471:    int weapList[]={WP_MACHINEGUN,WP_SHOTGUN,WP_GRENADE_LAUNCHER,WP_ROCKET_LAUNCHER,WP_LIGHTNING,WP_RAILGUN,WP_PLASMAGUN,WP_BFG,WP_NAILGUN,WP_PROX_LAUNCHER,WP_CHAINGUN};
;472:    int weapCount = sizeof(weapList) / sizeof(int);
;473:		//
;474:    for (i = 0; i < weapCount; i++) {
;475:		  w = weapList[i];
;476:
;477:		  switch(w) {
;478:			  case WP_MACHINEGUN: max = 50; inc = 4; t = 1000; break;
;479:			  case WP_SHOTGUN: max = 10; inc = 1; t = 1500; break;
;480:			  case WP_GRENADE_LAUNCHER: max = 10; inc = 1; t = 2000; break;
;481:			  case WP_ROCKET_LAUNCHER: max = 10; inc = 1; t = 1750; break;
;482:			  case WP_LIGHTNING: max = 50; inc = 5; t = 1500; break;
;483:			  case WP_RAILGUN: max = 10; inc = 1; t = 1750; break;
;484:			  case WP_PLASMAGUN: max = 50; inc = 5; t = 1500; break;
;485:			  case WP_BFG: max = 10; inc = 1; t = 4000; break;
;486:			  case WP_NAILGUN: max = 10; inc = 1; t = 1250; break;
;487:			  case WP_PROX_LAUNCHER: max = 5; inc = 1; t = 2000; break;
;488:			  case WP_CHAINGUN: max = 100; inc = 5; t = 1000; break;
;489:			  default: max = 0; inc = 0; t = 1000; break;
;490:		  }
;491:		  client->ammoTimes[w] += msec;
;492:		  if ( client->ps.ammo[w] >= max ) {
;493:			  client->ammoTimes[w] = 0;
;494:		  }
;495:		  if ( client->ammoTimes[w] >= t ) {
;496:			  while ( client->ammoTimes[w] >= t )
;497:				  client->ammoTimes[w] -= t;
;498:			  client->ps.ammo[w] += inc;
;499:			  if ( client->ps.ammo[w] > max ) {
;500:				  client->ps.ammo[w] = max;
;501:			  }
;502:		  }
;503:    }
;504:	}
;505:#endif
;506:}
LABELV $201
endproc ClientTimerActions 20 12
export ClientIntermissionThink
proc ClientIntermissionThink 24 0
line 513
;507:
;508:/*
;509:====================
;510:ClientIntermissionThink
;511:====================
;512:*/
;513:void ClientIntermissionThink( gclient_t *client ) {
line 514
;514:	client->ps.eFlags &= ~EF_TALK;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -4097
BANDI4
ASGNI4
line 515
;515:	client->ps.eFlags &= ~EF_FIRING;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -257
BANDI4
ASGNI4
line 520
;516:
;517:	// the level will exit when everyone wants to or after timeouts
;518:
;519:	// swap and latch button actions
;520:	client->oldbuttons = client->buttons;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 521
;521:	client->buttons = client->pers.cmd.buttons;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 656
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 488
ADDP4
INDIRI4
ASGNI4
line 522
;522:	if ( client->buttons & ( BUTTON_ATTACK | BUTTON_USE_HOLDABLE ) & ( client->oldbuttons ^ client->buttons ) ) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 5
BANDI4
ADDRLP4 16
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
BXORI4
BANDI4
CNSTI4 0
EQI4 $220
line 524
;523:		// this used to be an ^1 but once a player says ready, it should stick
;524:		client->readyToExit = 1;
ADDRFP4 0
INDIRP4
CNSTI4 644
ADDP4
CNSTI4 1
ASGNI4
line 525
;525:	}
LABELV $220
line 526
;526:}
LABELV $219
endproc ClientIntermissionThink 24 0
export ClientEvents
proc ClientEvents 92 32
line 537
;527:
;528:
;529:/*
;530:================
;531:ClientEvents
;532:
;533:Events will be passed on to the clients for presentation,
;534:but any server game effects are handled here
;535:================
;536:*/
;537:void ClientEvents( gentity_t *ent, int oldEventSequence ) {
line 548
;538:	int		i, j;
;539:	int		event;
;540:	gclient_t *client;
;541:	int		damage;
;542:	vec3_t	dir;
;543:	vec3_t	origin, angles;
;544://	qboolean	fired;
;545:	gitem_t *item;
;546:	gentity_t *drop;
;547:
;548:	client = ent->client;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 550
;549:
;550:	if ( oldEventSequence < client->ps.eventSequence - MAX_PS_EVENTS ) {
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 2
SUBI4
GEI4 $223
line 551
;551:		oldEventSequence = client->ps.eventSequence - MAX_PS_EVENTS;
ADDRFP4 4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 552
;552:	}
LABELV $223
line 553
;553:	for ( i = oldEventSequence ; i < client->ps.eventSequence ; i++ ) {
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 $228
JUMPV
LABELV $225
line 554
;554:		event = client->ps.events[ i & (MAX_PS_EVENTS-1) ];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 112
ADDP4
ADDP4
INDIRI4
ASGNI4
line 556
;555:
;556:		switch ( event ) {
ADDRLP4 68
CNSTI4 11
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
EQI4 $231
ADDRLP4 4
INDIRI4
CNSTI4 12
EQI4 $231
ADDRLP4 4
INDIRI4
ADDRLP4 68
INDIRI4
LTI4 $230
LABELV $256
ADDRLP4 4
INDIRI4
CNSTI4 23
EQI4 $242
ADDRLP4 4
INDIRI4
CNSTI4 25
EQI4 $243
ADDRLP4 4
INDIRI4
CNSTI4 26
EQI4 $255
ADDRGP4 $230
JUMPV
LABELV $231
line 559
;557:		case EV_FALL_MEDIUM:
;558:		case EV_FALL_FAR:
;559:			if ( ent->s.eType != ET_PLAYER ) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 1
EQI4 $232
line 560
;560:				break;		// not in the player model
ADDRGP4 $230
JUMPV
LABELV $232
line 562
;561:			}
;562:			if ( g_dmflags.integer & DF_NO_FALLING ) {
ADDRGP4 g_dmflags+12
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $234
line 563
;563:				break;
ADDRGP4 $230
JUMPV
LABELV $234
line 565
;564:			}
;565:			if ( event == EV_FALL_FAR ) {
ADDRLP4 4
INDIRI4
CNSTI4 12
NEI4 $237
line 566
;566:				damage = 10;
ADDRLP4 32
CNSTI4 10
ASGNI4
line 567
;567:			} else {
ADDRGP4 $238
JUMPV
LABELV $237
line 568
;568:				damage = 5;
ADDRLP4 32
CNSTI4 5
ASGNI4
line 569
;569:			}
LABELV $238
line 570
;570:			VectorSet (dir, 0, 0, 1);
ADDRLP4 76
CNSTF4 0
ASGNF4
ADDRLP4 16
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 16+8
CNSTF4 1065353216
ASGNF4
line 571
;571:			ent->pain_debounce_time = level.time + 200;	// no normal pain sound
ADDRFP4 0
INDIRP4
CNSTI4 720
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 572
;572:			G_Damage (ent, NULL, NULL, NULL, NULL, damage, 0, MOD_FALLING);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 80
CNSTP4 0
ASGNP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 84
CNSTP4 0
ASGNP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 32
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 19
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 573
;573:			break;
ADDRGP4 $230
JUMPV
LABELV $242
line 576
;574:
;575:		case EV_FIRE_WEAPON:
;576:			FireWeapon( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 FireWeapon
CALLV
pop
line 577
;577:			break;
ADDRGP4 $230
JUMPV
LABELV $243
line 581
;578:
;579:		case EV_USE_ITEM1:		// teleporter
;580:			// drop flags in CTF
;581:			item = NULL;
ADDRLP4 12
CNSTP4 0
ASGNP4
line 582
;582:			j = 0;
ADDRLP4 28
CNSTI4 0
ASGNI4
line 584
;583:
;584:			if ( ent->client->ps.powerups[ PW_REDFLAG ] ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $244
line 585
;585:				item = BG_FindItemForPowerup( PW_REDFLAG );
CNSTI4 7
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 586
;586:				j = PW_REDFLAG;
ADDRLP4 28
CNSTI4 7
ASGNI4
line 587
;587:			} else if ( ent->client->ps.powerups[ PW_BLUEFLAG ] ) {
ADDRGP4 $245
JUMPV
LABELV $244
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $246
line 588
;588:				item = BG_FindItemForPowerup( PW_BLUEFLAG );
CNSTI4 8
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 589
;589:				j = PW_BLUEFLAG;
ADDRLP4 28
CNSTI4 8
ASGNI4
line 590
;590:			} else if ( ent->client->ps.powerups[ PW_NEUTRALFLAG ] ) {
ADDRGP4 $247
JUMPV
LABELV $246
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $248
line 591
;591:				item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
CNSTI4 9
ARGI4
ADDRLP4 88
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 88
INDIRP4
ASGNP4
line 592
;592:				j = PW_NEUTRALFLAG;
ADDRLP4 28
CNSTI4 9
ASGNI4
line 593
;593:			}
LABELV $248
LABELV $247
LABELV $245
line 595
;594:
;595:			if ( item ) {
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $250
line 596
;596:				drop = Drop_Item( ent, item, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 88
ADDRGP4 Drop_Item
CALLP4
ASGNP4
ADDRLP4 60
ADDRLP4 88
INDIRP4
ASGNP4
line 598
;597:				// decide how many seconds it has left
;598:				drop->count = ( ent->client->ps.powerups[ j ] - level.time ) / 1000;
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 28
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
line 599
;599:				if ( drop->count < 1 ) {
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
INDIRI4
CNSTI4 1
GEI4 $253
line 600
;600:					drop->count = 1;
ADDRLP4 60
INDIRP4
CNSTI4 760
ADDP4
CNSTI4 1
ASGNI4
line 601
;601:				}
LABELV $253
line 603
;602:
;603:				ent->client->ps.powerups[ j ] = 0;
ADDRLP4 28
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
CNSTI4 0
ASGNI4
line 604
;604:			}
LABELV $250
line 628
;605:
;606:#ifdef MISSIONPACK
;607:			if ( g_gametype.integer == GT_HARVESTER ) {
;608:				if ( ent->client->ps.generic1 > 0 ) {
;609:					if ( ent->client->sess.sessionTeam == TEAM_RED ) {
;610:						item = BG_FindItem( "Blue Cube" );
;611:					} else {
;612:						item = BG_FindItem( "Red Cube" );
;613:					}
;614:					if ( item ) {
;615:						for ( j = 0; j < ent->client->ps.generic1; j++ ) {
;616:							drop = Drop_Item( ent, item, 0 );
;617:							if ( ent->client->sess.sessionTeam == TEAM_RED ) {
;618:								drop->spawnflags = TEAM_BLUE;
;619:							} else {
;620:								drop->spawnflags = TEAM_RED;
;621:							}
;622:						}
;623:					}
;624:					ent->client->ps.generic1 = 0;
;625:				}
;626:			}
;627:#endif
;628:			SelectSpawnPoint( ent->client->ps.origin, origin, angles );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 SelectSpawnPoint
CALLP4
pop
line 629
;629:			TeleportPlayer( ent, origin, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 TeleportPlayer
CALLV
pop
line 630
;630:			break;
ADDRGP4 $230
JUMPV
LABELV $255
line 633
;631:
;632:		case EV_USE_ITEM2:		// medkit
;633:			ent->health = ent->client->ps.stats[STAT_MAX_HEALTH] + 25;
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 88
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
line 635
;634:
;635:			break;
line 659
;636:
;637:#ifdef MISSIONPACK
;638:		case EV_USE_ITEM3:		// kamikaze
;639:			// make sure the invulnerability is off
;640:			ent->client->invulnerabilityTime = 0;
;641:			// start the kamikze
;642:			G_StartKamikaze( ent );
;643:			break;
;644:
;645:		case EV_USE_ITEM4:		// portal
;646:			if( ent->client->portalID ) {
;647:				DropPortalSource( ent );
;648:			}
;649:			else {
;650:				DropPortalDestination( ent );
;651:			}
;652:			break;
;653:		case EV_USE_ITEM5:		// invulnerability
;654:			ent->client->invulnerabilityTime = level.time + 10000;
;655:			break;
;656:#endif
;657:
;658:		default:
;659:			break;
LABELV $230
line 661
;660:		}
;661:	}
LABELV $226
line 553
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $228
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LTI4 $225
line 663
;662:
;663:}
LABELV $222
endproc ClientEvents 92 32
export SendPendingPredictableEvents
proc SendPendingPredictableEvents 44 12
line 715
;664:
;665:#ifdef MISSIONPACK
;666:/*
;667:==============
;668:StuckInOtherClient
;669:==============
;670:*/
;671:static int StuckInOtherClient(gentity_t *ent) {
;672:	int i;
;673:	gentity_t	*ent2;
;674:
;675:	ent2 = &g_entities[0];
;676:	for ( i = 0; i < MAX_CLIENTS; i++, ent2++ ) {
;677:		if ( ent2 == ent ) {
;678:			continue;
;679:		}
;680:		if ( !ent2->inuse ) {
;681:			continue;
;682:		}
;683:		if ( !ent2->client ) {
;684:			continue;
;685:		}
;686:		if ( ent2->health <= 0 ) {
;687:			continue;
;688:		}
;689:		//
;690:		if (ent2->r.absmin[0] > ent->r.absmax[0])
;691:			continue;
;692:		if (ent2->r.absmin[1] > ent->r.absmax[1])
;693:			continue;
;694:		if (ent2->r.absmin[2] > ent->r.absmax[2])
;695:			continue;
;696:		if (ent2->r.absmax[0] < ent->r.absmin[0])
;697:			continue;
;698:		if (ent2->r.absmax[1] < ent->r.absmin[1])
;699:			continue;
;700:		if (ent2->r.absmax[2] < ent->r.absmin[2])
;701:			continue;
;702:		return qtrue;
;703:	}
;704:	return qfalse;
;705:}
;706:#endif
;707:
;708:void BotTestSolid(vec3_t origin);
;709:
;710:/*
;711:==============
;712:SendPendingPredictableEvents
;713:==============
;714:*/
;715:void SendPendingPredictableEvents( playerState_t *ps ) {
line 721
;716:	gentity_t *t;
;717:	int event, seq;
;718:	int extEvent, number;
;719:
;720:	// if there are still events pending
;721:	if ( ps->entityEventSequence < ps->eventSequence ) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
GEI4 $258
line 724
;722:		// create a temporary entity for this event which is sent to everyone
;723:		// except the client who generated the event
;724:		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ASGNI4
line 725
;725:		event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 24
INDIRP4
CNSTI4 112
ADDP4
ADDP4
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 8
LSHI4
BORI4
ASGNI4
line 727
;726:		// set external event to zero before calling BG_PlayerStateToEntityState
;727:		extEvent = ps->externalEvent;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
INDIRI4
ASGNI4
line 728
;728:		ps->externalEvent = 0;
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
CNSTI4 0
ASGNI4
line 730
;729:		// create temporary entity for event
;730:		t = G_TempEntity( ps->origin, event );
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
line 731
;731:		number = t->s.number;
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 732
;732:		BG_PlayerStateToEntityState( ps, &t->s, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 733
;733:		t->s.number = number;
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ASGNI4
line 734
;734:		t->s.eType = ET_EVENTS + event;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 13
ADDI4
ASGNI4
line 735
;735:		t->s.eFlags |= EF_PLAYER_EVENT;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 736
;736:		t->s.otherEntityNum = ps->clientNum;
ADDRLP4 36
CNSTI4 140
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
INDIRI4
ASGNI4
line 738
;737:		// send to everyone except the client who generated the event
;738:		t->r.svFlags |= SVF_NOTSINGLECLIENT;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 2048
BORI4
ASGNI4
line 739
;739:		t->r.singleClient = ps->clientNum;
ADDRLP4 0
INDIRP4
CNSTI4 428
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ASGNI4
line 741
;740:		// set back external event
;741:		ps->externalEvent = extEvent;
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 742
;742:	}
LABELV $258
line 743
;743:}
LABELV $257
endproc SendPendingPredictableEvents 44 12
export ClientThink_real
proc ClientThink_real 304 16
line 756
;744:
;745:/*
;746:==============
;747:ClientThink
;748:
;749:This will be called once for each client frame, which will
;750:usually be a couple times for each server frame on fast clients.
;751:
;752:If "g_synchronousClients 1" is set, this will be called exactly
;753:once for each server frame, which makes for smooth demo recording.
;754:==============
;755:*/
;756:void ClientThink_real( gentity_t *ent ) {
line 763
;757:	gclient_t	*client;
;758:	pmove_t		pm;
;759:	int			oldEventSequence;
;760:	int			msec;
;761:	usercmd_t	*ucmd;
;762:
;763:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
line 766
;764:
;765:	// don't think if the client is not yet connected (and thus not yet spawned in)
;766:	if (client->pers.connected != CON_CONNECTED) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $261
line 767
;767:		return;
ADDRGP4 $260
JUMPV
LABELV $261
line 770
;768:	}
;769:	// mark the time, so the connection sprite can be removed
;770:	ucmd = &ent->client->pers.cmd;
ADDRLP4 236
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 472
ADDP4
ASGNP4
line 773
;771:
;772:	// sanity check the command time to prevent speedup cheating
;773:	if ( ucmd->serverTime > level.time + 200 ) {
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
LEI4 $263
line 774
;774:		ucmd->serverTime = level.time + 200;
ADDRLP4 236
INDIRP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 776
;775://		G_Printf("serverTime <<<<<\n" );
;776:	}
LABELV $263
line 777
;777:	if ( ucmd->serverTime < level.time - 1000 ) {
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
SUBI4
GEI4 $267
line 778
;778:		ucmd->serverTime = level.time - 1000;
ADDRLP4 236
INDIRP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
SUBI4
ASGNI4
line 780
;779://		G_Printf("serverTime >>>>>\n" );
;780:	} 
LABELV $267
line 782
;781:
;782:	msec = ucmd->serverTime - client->ps.commandTime;
ADDRLP4 240
ADDRLP4 236
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
SUBI4
ASGNI4
line 785
;783:	// following others may result in bad times, but we still want
;784:	// to check for follow toggles
;785:	if ( msec < 1 && client->sess.spectatorState != SPECTATOR_FOLLOW ) {
ADDRLP4 240
INDIRI4
CNSTI4 1
GEI4 $271
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
EQI4 $271
line 786
;786:		return;
ADDRGP4 $260
JUMPV
LABELV $271
line 788
;787:	}
;788:	if ( msec > 200 ) {
ADDRLP4 240
INDIRI4
CNSTI4 200
LEI4 $273
line 789
;789:		msec = 200;
ADDRLP4 240
CNSTI4 200
ASGNI4
line 790
;790:	}
LABELV $273
line 792
;791:
;792:	if ( pmove_msec.integer < 8 ) {
ADDRGP4 pmove_msec+12
INDIRI4
CNSTI4 8
GEI4 $275
line 793
;793:		trap_Cvar_Set("pmove_msec", "8");
ADDRGP4 $278
ARGP4
ADDRGP4 $279
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 794
;794:	}
ADDRGP4 $276
JUMPV
LABELV $275
line 795
;795:	else if (pmove_msec.integer > 33) {
ADDRGP4 pmove_msec+12
INDIRI4
CNSTI4 33
LEI4 $280
line 796
;796:		trap_Cvar_Set("pmove_msec", "33");
ADDRGP4 $278
ARGP4
ADDRGP4 $283
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 797
;797:	}
LABELV $280
LABELV $276
line 799
;798:
;799:	if ( pmove_fixed.integer || client->pers.pmoveFixed ) {
ADDRLP4 248
CNSTI4 0
ASGNI4
ADDRGP4 pmove_fixed+12
INDIRI4
ADDRLP4 248
INDIRI4
NEI4 $287
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
ADDRLP4 248
INDIRI4
EQI4 $284
LABELV $287
line 800
;800:		ucmd->serverTime = ((ucmd->serverTime + pmove_msec.integer-1) / pmove_msec.integer) * pmove_msec.integer;
ADDRLP4 236
INDIRP4
ADDRLP4 236
INDIRP4
INDIRI4
ADDRGP4 pmove_msec+12
INDIRI4
ADDI4
CNSTI4 1
SUBI4
ADDRGP4 pmove_msec+12
INDIRI4
DIVI4
ADDRGP4 pmove_msec+12
INDIRI4
MULI4
ASGNI4
line 803
;801:		//if (ucmd->serverTime - client->ps.commandTime <= 0)
;802:		//	return;
;803:	}
LABELV $284
line 808
;804:
;805:	//
;806:	// check for exiting intermission
;807:	//
;808:	if ( level.intermissiontime ) {
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $291
line 809
;809:		ClientIntermissionThink( client );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 ClientIntermissionThink
CALLV
pop
line 810
;810:		return;
ADDRGP4 $260
JUMPV
LABELV $291
line 814
;811:	}
;812:
;813:	// spectators don't do much
;814:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $294
line 815
;815:		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $296
line 816
;816:			return;
ADDRGP4 $260
JUMPV
LABELV $296
line 818
;817:		}
;818:		SpectatorThink( ent, ucmd );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 236
INDIRP4
ARGP4
ADDRGP4 SpectatorThink
CALLV
pop
line 819
;819:		return;
ADDRGP4 $260
JUMPV
LABELV $294
line 823
;820:	}
;821:
;822:	// check for inactivity timer, but never drop the local client of a non-dedicated server
;823:	if ( !ClientInactivityTimer( client ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 252
ADDRGP4 ClientInactivityTimer
CALLI4
ASGNI4
ADDRLP4 252
INDIRI4
CNSTI4 0
NEI4 $298
line 824
;824:		return;
ADDRGP4 $260
JUMPV
LABELV $298
line 828
;825:	}
;826:
;827:	// clear the rewards if time
;828:	if ( level.time > client->rewardTime ) {
ADDRGP4 level+32
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 744
ADDP4
INDIRI4
LEI4 $300
line 829
;829:		client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 256
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 256
INDIRP4
ADDRLP4 256
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 830
;830:	}
LABELV $300
line 832
;831:
;832:	if ( client->noclip ) {
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
EQI4 $303
line 833
;833:		client->ps.pm_type = PM_NOCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 1
ASGNI4
line 834
;834:	} else if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRGP4 $304
JUMPV
LABELV $303
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $305
line 835
;835:		client->ps.pm_type = PM_DEAD;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 836
;836:	} else {
ADDRGP4 $306
JUMPV
LABELV $305
line 837
;837:		client->ps.pm_type = PM_NORMAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 838
;838:	}
LABELV $306
LABELV $304
line 840
;839:
;840:	client->ps.gravity = g_gravity.value;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
ADDRGP4 g_gravity+8
INDIRF4
CVFI4 4
ASGNI4
line 843
;841:
;842:	// set speed
;843:	client->ps.speed = g_speed.value;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 g_speed+8
INDIRF4
CVFI4 4
ASGNI4
line 851
;844:
;845:#ifdef MISSIONPACK
;846:	if( bg_itemlist[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
;847:		client->ps.speed *= 1.5;
;848:	}
;849:	else
;850:#endif
;851:	if ( client->ps.powerups[PW_HASTE] ) {
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRI4
CNSTI4 0
EQI4 $309
line 852
;852:		client->ps.speed *= 1.3;
ADDRLP4 256
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
ADDRLP4 256
INDIRP4
CNSTF4 1067869798
ADDRLP4 256
INDIRP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 853
;853:	}
LABELV $309
line 856
;854:
;855:	// Let go of the hook if we aren't firing
;856:	if ( client->ps.weapon == WP_GRAPPLING_HOOK &&
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 10
NEI4 $311
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $311
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $311
line 857
;857:		client->hook && !( ucmd->buttons & BUTTON_ATTACK ) ) {
line 858
;858:		Weapon_HookFree(client->hook);
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRP4
ARGP4
ADDRGP4 Weapon_HookFree
CALLV
pop
line 859
;859:	}
LABELV $311
line 862
;860:
;861:	// set up for pmove
;862:	oldEventSequence = client->ps.eventSequence;
ADDRLP4 244
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 864
;863:
;864:	memset (&pm, 0, sizeof(pm));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 232
ARGI4
ADDRGP4 memset
CALLP4
pop
line 868
;865:
;866:	// check for the hit-scan gauntlet, don't let the action
;867:	// go through as an attack unless it actually hits something
;868:	if ( client->ps.weapon == WP_GAUNTLET && !( ucmd->buttons & BUTTON_TALK ) &&
ADDRLP4 264
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ADDRLP4 264
INDIRI4
NEI4 $313
ADDRLP4 268
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 272
CNSTI4 0
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 272
INDIRI4
NEI4 $313
ADDRLP4 268
INDIRI4
ADDRLP4 264
INDIRI4
BANDI4
ADDRLP4 272
INDIRI4
EQI4 $313
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRLP4 272
INDIRI4
GTI4 $313
line 869
;869:		( ucmd->buttons & BUTTON_ATTACK ) && client->ps.weaponTime <= 0 ) {
line 870
;870:		pm.gauntletHit = CheckGauntletAttack( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 276
ADDRGP4 CheckGauntletAttack
CALLI4
ASGNI4
ADDRLP4 4+40
ADDRLP4 276
INDIRI4
ASGNI4
line 871
;871:	}
LABELV $313
line 873
;872:
;873:	if ( ent->flags & FL_FORCE_GESTURE ) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRI4
CNSTI4 32768
BANDI4
CNSTI4 0
EQI4 $316
line 874
;874:		ent->flags &= ~FL_FORCE_GESTURE;
ADDRLP4 276
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ASGNP4
ADDRLP4 276
INDIRP4
ADDRLP4 276
INDIRP4
INDIRI4
CNSTI4 -32769
BANDI4
ASGNI4
line 875
;875:		ent->client->pers.cmd.buttons |= BUTTON_GESTURE;
ADDRLP4 280
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 488
ADDP4
ASGNP4
ADDRLP4 280
INDIRP4
ADDRLP4 280
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 876
;876:	}
LABELV $316
line 905
;877:
;878:#ifdef MISSIONPACK
;879:	// check for invulnerability expansion before doing the Pmove
;880:	if (client->ps.powerups[PW_INVULNERABILITY] ) {
;881:		if ( !(client->ps.pm_flags & PMF_INVULEXPAND) ) {
;882:			vec3_t mins = { -42, -42, -42 };
;883:			vec3_t maxs = { 42, 42, 42 };
;884:			vec3_t oldmins, oldmaxs;
;885:
;886:			VectorCopy (ent->r.mins, oldmins);
;887:			VectorCopy (ent->r.maxs, oldmaxs);
;888:			// expand
;889:			VectorCopy (mins, ent->r.mins);
;890:			VectorCopy (maxs, ent->r.maxs);
;891:			trap_LinkEntity(ent);
;892:			// check if this would get anyone stuck in this player
;893:			if ( !StuckInOtherClient(ent) ) {
;894:				// set flag so the expanded size will be set in PM_CheckDuck
;895:				client->ps.pm_flags |= PMF_INVULEXPAND;
;896:			}
;897:			// set back
;898:			VectorCopy (oldmins, ent->r.mins);
;899:			VectorCopy (oldmaxs, ent->r.maxs);
;900:			trap_LinkEntity(ent);
;901:		}
;902:	}
;903:#endif
;904:
;905:	pm.ps = &client->ps;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 906
;906:	pm.cmd = *ucmd;
ADDRLP4 4+4
ADDRLP4 236
INDIRP4
INDIRB
ASGNB 24
line 907
;907:	if ( pm.ps->pm_type == PM_DEAD ) {
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $319
line 908
;908:		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
ADDRLP4 4+28
CNSTI4 65537
ASGNI4
line 909
;909:	}
ADDRGP4 $320
JUMPV
LABELV $319
line 910
;910:	else if ( ent->r.svFlags & SVF_BOT ) {
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $322
line 911
;911:		pm.tracemask = MASK_PLAYERSOLID | CONTENTS_BOTCLIP;
ADDRLP4 4+28
CNSTI4 37814273
ASGNI4
line 912
;912:	}
ADDRGP4 $323
JUMPV
LABELV $322
line 913
;913:	else {
line 914
;914:		pm.tracemask = MASK_PLAYERSOLID;
ADDRLP4 4+28
CNSTI4 33619969
ASGNI4
line 915
;915:	}
LABELV $323
LABELV $320
line 916
;916:	pm.trace = trap_Trace;
ADDRLP4 4+224
ADDRGP4 trap_Trace
ASGNP4
line 917
;917:	pm.pointcontents = trap_PointContents;
ADDRLP4 4+228
ADDRGP4 trap_PointContents
ASGNP4
line 918
;918:	pm.debugLevel = g_debugMove.integer;
ADDRLP4 4+32
ADDRGP4 g_debugMove+12
INDIRI4
ASGNI4
line 919
;919:	pm.noFootsteps = ( g_dmflags.integer & DF_NO_FOOTSTEPS ) > 0;
ADDRGP4 g_dmflags+12
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
LEI4 $333
ADDRLP4 276
CNSTI4 1
ASGNI4
ADDRGP4 $334
JUMPV
LABELV $333
ADDRLP4 276
CNSTI4 0
ASGNI4
LABELV $334
ADDRLP4 4+36
ADDRLP4 276
INDIRI4
ASGNI4
line 921
;920:
;921:	pm.pmove_fixed = pmove_fixed.integer | client->pers.pmoveFixed;
ADDRLP4 4+216
ADDRGP4 pmove_fixed+12
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
BORI4
ASGNI4
line 922
;922:	pm.pmove_msec = pmove_msec.integer;
ADDRLP4 4+220
ADDRGP4 pmove_msec+12
INDIRI4
ASGNI4
line 924
;923:
;924:	VectorCopy( client->ps.origin, client->oldOrigin );
ADDRLP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 941
;925:
;926:#ifdef MISSIONPACK
;927:		if (level.intermissionQueued != 0 && g_singlePlayer.integer) {
;928:			if ( level.time - level.intermissionQueued >= 1000  ) {
;929:				pm.cmd.buttons = 0;
;930:				pm.cmd.forwardmove = 0;
;931:				pm.cmd.rightmove = 0;
;932:				pm.cmd.upmove = 0;
;933:				if ( level.time - level.intermissionQueued >= 2000 && level.time - level.intermissionQueued <= 2500 ) {
;934:					trap_SendConsoleCommand( EXEC_APPEND, "centerview\n");
;935:				}
;936:				ent->client->ps.pm_type = PM_SPINTERMISSION;
;937:			}
;938:		}
;939:		Pmove (&pm);
;940:#else
;941:		Pmove (&pm);
ADDRLP4 4
ARGP4
ADDRGP4 Pmove
CALLV
pop
line 945
;942:#endif
;943:
;944:	// save results of pmove
;945:	if ( ent->client->ps.eventSequence != oldEventSequence ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
EQI4 $339
line 946
;946:		ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 947
;947:	}
LABELV $339
line 948
;948:	if (g_smoothClients.integer) {
ADDRGP4 g_smoothClients+12
INDIRI4
CNSTI4 0
EQI4 $342
line 949
;949:		BG_PlayerStateToEntityStateExtraPolate( &ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 288
ADDRLP4 284
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 284
INDIRP4
ARGP4
ADDRLP4 288
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityStateExtraPolate
CALLV
pop
line 950
;950:	}
ADDRGP4 $343
JUMPV
LABELV $342
line 951
;951:	else {
line 952
;952:		BG_PlayerStateToEntityState( &ent->client->ps, &ent->s, qtrue );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 284
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 953
;953:	}
LABELV $343
line 954
;954:	SendPendingPredictableEvents( &ent->client->ps );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRGP4 SendPendingPredictableEvents
CALLV
pop
line 956
;955:
;956:	if ( !( ent->client->ps.eFlags & EF_FIRING ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
NEI4 $345
line 957
;957:		client->fireHeld = qfalse;		// for grapple
ADDRLP4 0
INDIRP4
CNSTI4 756
ADDP4
CNSTI4 0
ASGNI4
line 958
;958:	}
LABELV $345
line 961
;959:
;960:	// use the snapped origin for linking so it matches client predicted versions
;961:	VectorCopy( ent->s.pos.trBase, ent->r.currentOrigin );
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 284
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 963
;962:
;963:	VectorCopy (pm.mins, ent->r.mins);
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
ADDRLP4 4+180
INDIRB
ASGNB 12
line 964
;964:	VectorCopy (pm.maxs, ent->r.maxs);
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
ADDRLP4 4+192
INDIRB
ASGNB 12
line 966
;965:
;966:	ent->waterlevel = pm.waterlevel;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
ADDRLP4 4+208
INDIRI4
ASGNI4
line 967
;967:	ent->watertype = pm.watertype;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRLP4 4+204
INDIRI4
ASGNI4
line 970
;968:
;969:	// execute client events
;970:	ClientEvents( ent, oldEventSequence );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 244
INDIRI4
ARGI4
ADDRGP4 ClientEvents
CALLV
pop
line 973
;971:
;972:	// link entity now, after any personal teleporters have been used
;973:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 974
;974:	if ( !ent->client->noclip ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 648
ADDP4
INDIRI4
CNSTI4 0
NEI4 $351
line 975
;975:		G_TouchTriggers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_TouchTriggers
CALLV
pop
line 976
;976:	}
LABELV $351
line 979
;977:
;978:	// NOTE: now copy the exact origin over otherwise clients can be snapped into solid
;979:	VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
ADDRLP4 288
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 288
INDIRP4
CNSTI4 488
ADDP4
ADDRLP4 288
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 982
;980:
;981:	//test for solid areas in the AAS file
;982:	BotTestAAS(ent->r.currentOrigin);
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
ADDRGP4 BotTestAAS
CALLV
pop
line 985
;983:
;984:	// touch other objects
;985:	ClientImpacts( ent, &pm );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 ClientImpacts
CALLV
pop
line 988
;986:
;987:	// save results of triggers and client events
;988:	if (ent->client->ps.eventSequence != oldEventSequence) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 244
INDIRI4
EQI4 $353
line 989
;989:		ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 990
;990:	}
LABELV $353
line 993
;991:
;992:	// swap and latch button actions
;993:	client->oldbuttons = client->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ASGNI4
line 994
;994:	client->buttons = ucmd->buttons;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 995
;995:	client->latched_buttons |= client->buttons & ~client->oldbuttons;
ADDRLP4 300
ADDRLP4 0
INDIRP4
CNSTI4 664
ADDP4
ASGNP4
ADDRLP4 300
INDIRP4
ADDRLP4 300
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
INDIRI4
BCOMI4
BANDI4
BORI4
ASGNI4
line 998
;996:
;997:	// check for respawning
;998:	if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $356
line 1000
;999:		// wait for the attack button to be pressed
;1000:		if ( level.time > client->respawnTime ) {
ADDRGP4 level+32
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
LEI4 $260
line 1002
;1001:			// forcerespawn is to prevent users from waiting out powerups
;1002:			if ( g_forcerespawn.integer > 0 && 
ADDRGP4 g_forcerespawn+12
INDIRI4
CNSTI4 0
LEI4 $361
ADDRGP4 level+32
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
SUBI4
CNSTI4 1000
ADDRGP4 g_forcerespawn+12
INDIRI4
MULI4
LEI4 $361
line 1003
;1003:				( level.time - client->respawnTime ) > g_forcerespawn.integer * 1000 ) {
line 1004
;1004:				respawn( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 1005
;1005:				return;
ADDRGP4 $260
JUMPV
LABELV $361
line 1009
;1006:			}
;1007:		
;1008:			// pressing attack or use is the normal respawn method
;1009:			if ( ucmd->buttons & ( BUTTON_ATTACK | BUTTON_USE_HOLDABLE ) ) {
ADDRLP4 236
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 5
BANDI4
CNSTI4 0
EQI4 $260
line 1010
;1010:				respawn( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 1011
;1011:			}
line 1012
;1012:		}
line 1013
;1013:		return;
ADDRGP4 $260
JUMPV
LABELV $356
line 1017
;1014:	}
;1015:
;1016:	// perform once-a-second actions
;1017:	ClientTimerActions( ent, msec );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 240
INDIRI4
ARGI4
ADDRGP4 ClientTimerActions
CALLV
pop
line 1018
;1018:}
LABELV $260
endproc ClientThink_real 304 16
export ClientThink
proc ClientThink 8 8
line 1027
;1019:
;1020:/*
;1021:==================
;1022:ClientThink
;1023:
;1024:A new command has arrived from the client
;1025:==================
;1026:*/
;1027:void ClientThink( int clientNum ) {
line 1030
;1028:	gentity_t *ent;
;1029:
;1030:	ent = g_entities + clientNum;
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1031
;1031:	trap_GetUsercmd( clientNum, &ent->client->pers.cmd );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
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
line 1035
;1032:
;1033:	// mark the time we got info, so we can display the
;1034:	// phone jack if they don't get any for a while
;1035:	ent->client->lastCmdTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1037
;1036:
;1037:	if ( !(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 4
INDIRI4
NEI4 $370
ADDRGP4 g_synchronousClients+12
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $370
line 1038
;1038:		ClientThink_real( ent );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 ClientThink_real
CALLV
pop
line 1039
;1039:	}
LABELV $370
line 1040
;1040:}
LABELV $368
endproc ClientThink 8 8
export G_RunClient
proc G_RunClient 4 4
line 1043
;1041:
;1042:
;1043:void G_RunClient( gentity_t *ent ) {
line 1044
;1044:	if ( !(ent->r.svFlags & SVF_BOT) && !g_synchronousClients.integer ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 0
INDIRI4
NEI4 $374
ADDRGP4 g_synchronousClients+12
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $374
line 1045
;1045:		return;
ADDRGP4 $373
JUMPV
LABELV $374
line 1047
;1046:	}
;1047:	ent->client->pers.cmd.serverTime = level.time;
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
line 1048
;1048:	ClientThink_real( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientThink_real
CALLV
pop
line 1049
;1049:}
LABELV $373
endproc G_RunClient 4 4
export SpectatorClientEndFrame
proc SpectatorClientEndFrame 24 4
line 1058
;1050:
;1051:
;1052:/*
;1053:==================
;1054:SpectatorClientEndFrame
;1055:
;1056:==================
;1057:*/
;1058:void SpectatorClientEndFrame( gentity_t *ent ) {
line 1062
;1059:	gclient_t	*cl;
;1060:
;1061:	// if we are doing a chase cam or a remote view, grab the latest info
;1062:	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 2
NEI4 $379
line 1065
;1063:		int		clientNum, flags;
;1064:
;1065:		clientNum = ent->client->sess.spectatorClient;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ASGNI4
line 1068
;1066:
;1067:		// team follow1 and team follow2 go to whatever clients are playing
;1068:		if ( clientNum == -1 ) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
NEI4 $381
line 1069
;1069:			clientNum = level.follow1;
ADDRLP4 4
ADDRGP4 level+340
INDIRI4
ASGNI4
line 1070
;1070:		} else if ( clientNum == -2 ) {
ADDRGP4 $382
JUMPV
LABELV $381
ADDRLP4 4
INDIRI4
CNSTI4 -2
NEI4 $384
line 1071
;1071:			clientNum = level.follow2;
ADDRLP4 4
ADDRGP4 level+344
INDIRI4
ASGNI4
line 1072
;1072:		}
LABELV $384
LABELV $382
line 1073
;1073:		if ( clientNum >= 0 ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $387
line 1074
;1074:			cl = &level.clients[ clientNum ];
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1075
;1075:			if ( cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $389
ADDRLP4 12
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $389
line 1076
;1076:				flags = (cl->ps.eFlags & ~(EF_VOTED | EF_TEAMVOTED)) | (ent->client->ps.eFlags & (EF_VOTED | EF_TEAMVOTED));
ADDRLP4 16
CNSTI4 104
ASGNI4
ADDRLP4 8
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
CNSTI4 -540673
BANDI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
CNSTI4 540672
BANDI4
BORI4
ASGNI4
line 1077
;1077:				ent->client->ps = cl->ps;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ADDRLP4 0
INDIRP4
INDIRB
ASGNB 468
line 1078
;1078:				ent->client->ps.pm_flags |= PMF_FOLLOW;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1079
;1079:				ent->client->ps.eFlags = flags;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1080
;1080:				return;
ADDRGP4 $378
JUMPV
LABELV $389
line 1081
;1081:			} else {
line 1083
;1082:				// drop them to free spectators unless they are dedicated camera followers
;1083:				if ( ent->client->sess.spectatorClient >= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
CNSTI4 0
LTI4 $391
line 1084
;1084:					ent->client->sess.spectatorState = SPECTATOR_FREE;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
CNSTI4 1
ASGNI4
line 1085
;1085:					ClientBegin( ent->client - level.clients );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
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
ADDRGP4 ClientBegin
CALLV
pop
line 1086
;1086:				}
LABELV $391
line 1087
;1087:			}
line 1088
;1088:		}
LABELV $387
line 1089
;1089:	}
LABELV $379
line 1091
;1090:
;1091:	if ( ent->client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
CNSTI4 3
NEI4 $393
line 1092
;1092:		ent->client->ps.pm_flags |= PMF_SCOREBOARD;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
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
line 1093
;1093:	} else {
ADDRGP4 $394
JUMPV
LABELV $393
line 1094
;1094:		ent->client->ps.pm_flags &= ~PMF_SCOREBOARD;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -8193
BANDI4
ASGNI4
line 1095
;1095:	}
LABELV $394
line 1096
;1096:}
LABELV $378
endproc SpectatorClientEndFrame 24 4
export ClientEndFrame
proc ClientEndFrame 20 16
line 1107
;1097:
;1098:/*
;1099:==============
;1100:ClientEndFrame
;1101:
;1102:Called at the end of each server frame for each connected client
;1103:A fast client will have multiple ClientThink for each ClientEdFrame,
;1104:while a slow client may have multiple ClientEndFrame between ClientThink.
;1105:==============
;1106:*/
;1107:void ClientEndFrame( gentity_t *ent ) {
line 1111
;1108:	int			i;
;1109:	clientPersistant_t	*pers;
;1110:
;1111:	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
NEI4 $396
line 1112
;1112:		SpectatorClientEndFrame( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SpectatorClientEndFrame
CALLV
pop
line 1113
;1113:		return;
ADDRGP4 $395
JUMPV
LABELV $396
line 1116
;1114:	}
;1115:
;1116:	pers = &ent->client->pers;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
ASGNP4
line 1119
;1117:
;1118:	// turn off any expired powerups
;1119:	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $398
line 1120
;1120:		if ( ent->client->ps.powerups[ i ] < level.time ) {
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
GEI4 $402
line 1121
;1121:			ent->client->ps.powerups[ i ] = 0;
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
CNSTI4 0
ASGNI4
line 1122
;1122:		}
LABELV $402
line 1123
;1123:	}
LABELV $399
line 1119
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $398
line 1156
;1124:
;1125:#ifdef MISSIONPACK
;1126:	// set powerup for player animation
;1127:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
;1128:		ent->client->ps.powerups[PW_GUARD] = level.time;
;1129:	}
;1130:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
;1131:		ent->client->ps.powerups[PW_SCOUT] = level.time;
;1132:	}
;1133:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_DOUBLER ) {
;1134:		ent->client->ps.powerups[PW_DOUBLER] = level.time;
;1135:	}
;1136:	if( bg_itemlist[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN ) {
;1137:		ent->client->ps.powerups[PW_AMMOREGEN] = level.time;
;1138:	}
;1139:	if ( ent->client->invulnerabilityTime > level.time ) {
;1140:		ent->client->ps.powerups[PW_INVULNERABILITY] = level.time;
;1141:	}
;1142:#endif
;1143:
;1144:	// save network bandwidth
;1145:#if 0
;1146:	if ( !g_synchronousClients->integer && ent->client->ps.pm_type == PM_NORMAL ) {
;1147:		// FIXME: this must change eventually for non-sync demo recording
;1148:		VectorClear( ent->client->ps.viewangles );
;1149:	}
;1150:#endif
;1151:
;1152:	//
;1153:	// If the end of unit layout is displayed, don't give
;1154:	// the player any normal movement attributes
;1155:	//
;1156:	if ( level.intermissiontime ) {
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $405
line 1157
;1157:		return;
ADDRGP4 $395
JUMPV
LABELV $405
line 1161
;1158:	}
;1159:
;1160:	// burn from lava, etc
;1161:	P_WorldEffects (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 P_WorldEffects
CALLV
pop
line 1164
;1162:
;1163:	// apply all the damage taken this frame
;1164:	P_DamageFeedback (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 P_DamageFeedback
CALLV
pop
line 1167
;1165:
;1166:	// add the EF_CONNECTION flag if we haven't gotten commands recently
;1167:	if ( level.time - ent->client->lastCmdTime > 1000 ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 652
ADDP4
INDIRI4
SUBI4
CNSTI4 1000
LEI4 $408
line 1168
;1168:		ent->s.eFlags |= EF_CONNECTION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 8192
BORI4
ASGNI4
line 1169
;1169:	} else {
ADDRGP4 $409
JUMPV
LABELV $408
line 1170
;1170:		ent->s.eFlags &= ~EF_CONNECTION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -8193
BANDI4
ASGNI4
line 1171
;1171:	}
LABELV $409
line 1173
;1172:
;1173:	ent->client->ps.stats[STAT_HEALTH] = ent->health;	// FIXME: get rid of ent->health...
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 732
ADDP4
INDIRI4
ASGNI4
line 1175
;1174:
;1175:	G_SetClientSound (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_SetClientSound
CALLV
pop
line 1178
;1176:
;1177:	// set the latest infor
;1178:	if (g_smoothClients.integer) {
ADDRGP4 g_smoothClients+12
INDIRI4
CNSTI4 0
EQI4 $411
line 1179
;1179:		BG_PlayerStateToEntityStateExtraPolate( &ent->client->ps, &ent->s, ent->client->ps.commandTime, qtrue );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityStateExtraPolate
CALLV
pop
line 1180
;1180:	}
ADDRGP4 $412
JUMPV
LABELV $411
line 1181
;1181:	else {
line 1182
;1182:		BG_PlayerStateToEntityState( &ent->client->ps, &ent->s, qtrue );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1183
;1183:	}
LABELV $412
line 1184
;1184:	SendPendingPredictableEvents( &ent->client->ps );
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRGP4 SendPendingPredictableEvents
CALLV
pop
line 1189
;1185:
;1186:	// set the bit for the reachability area the client is currently in
;1187://	i = trap_AAS_PointReachabilityAreaIndex( ent->client->ps.origin );
;1188://	ent->client->areabits[i >> 3] |= 1 << (i & 7);
;1189:}
LABELV $395
endproc ClientEndFrame 20 16
import BotTestSolid
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
LABELV $283
byte 1 51
byte 1 51
byte 1 0
align 1
LABELV $279
byte 1 56
byte 1 0
align 1
LABELV $278
byte 1 112
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 95
byte 1 109
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $200
byte 1 99
byte 1 112
byte 1 32
byte 1 34
byte 1 84
byte 1 101
byte 1 110
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 111
byte 1 110
byte 1 100
byte 1 115
byte 1 32
byte 1 117
byte 1 110
byte 1 116
byte 1 105
byte 1 108
byte 1 32
byte 1 105
byte 1 110
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 100
byte 1 114
byte 1 111
byte 1 112
byte 1 33
byte 1 10
byte 1 34
byte 1 0
align 1
LABELV $196
byte 1 68
byte 1 114
byte 1 111
byte 1 112
byte 1 112
byte 1 101
byte 1 100
byte 1 32
byte 1 100
byte 1 117
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 105
byte 1 110
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $92
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 47
byte 1 103
byte 1 117
byte 1 114
byte 1 112
byte 1 50
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $91
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 47
byte 1 103
byte 1 117
byte 1 114
byte 1 112
byte 1 49
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $88
byte 1 42
byte 1 100
byte 1 114
byte 1 111
byte 1 119
byte 1 110
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
