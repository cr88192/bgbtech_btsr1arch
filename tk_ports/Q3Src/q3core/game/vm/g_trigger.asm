export InitTrigger
code
proc InitTrigger 12 8
file "../g_trigger.c"
line 26
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
;25:
;26:void InitTrigger( gentity_t *self ) {
line 27
;27:	if (!VectorCompare (self->s.angles, vec3_origin))
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $53
line 28
;28:		G_SetMovedir (self->s.angles, self->movedir);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 676
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
LABELV $53
line 30
;29:
;30:	trap_SetBrushModel( self, self->model );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 540
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_SetBrushModel
CALLV
pop
line 31
;31:	self->r.contents = CONTENTS_TRIGGER;		// replaces the -1 from trap_SetBrushModel
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 1073741824
ASGNI4
line 32
;32:	self->r.svFlags = SVF_NOCLIENT;
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
CNSTI4 1
ASGNI4
line 33
;33:}
LABELV $52
endproc InitTrigger 12 8
export multi_wait
proc multi_wait 0 0
line 37
;34:
;35:
;36:// the wait time has passed, so set back up for another activation
;37:void multi_wait( gentity_t *ent ) {
line 38
;38:	ent->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 0
ASGNI4
line 39
;39:}
LABELV $55
endproc multi_wait 0 0
export multi_trigger
proc multi_trigger 12 8
line 45
;40:
;41:
;42:// the trigger was just activated
;43:// ent->activator should be set to the activator so it can be held through a delay
;44:// so wait for the delay time before firing
;45:void multi_trigger( gentity_t *ent, gentity_t *activator ) {
line 46
;46:	ent->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 772
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 47
;47:	if ( ent->nextthink ) {
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 0
EQI4 $57
line 48
;48:		return;		// can't retrigger until the wait is over
ADDRGP4 $56
JUMPV
LABELV $57
line 51
;49:	}
;50:
;51:	if ( activator->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $59
line 52
;52:		if ( ( ent->spawnflags & 1 ) &&
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
BANDI4
CNSTI4 0
EQI4 $61
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $61
line 53
;53:			activator->client->sess.sessionTeam != TEAM_RED ) {
line 54
;54:			return;
ADDRGP4 $56
JUMPV
LABELV $61
line 56
;55:		}
;56:		if ( ( ent->spawnflags & 2 ) &&
ADDRLP4 4
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $63
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $63
line 57
;57:			activator->client->sess.sessionTeam != TEAM_BLUE ) {
line 58
;58:			return;
ADDRGP4 $56
JUMPV
LABELV $63
line 60
;59:		}
;60:	}
LABELV $59
line 62
;61:
;62:	G_UseTargets (ent, ent->activator);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 64
;63:
;64:	if ( ent->wait > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
CNSTF4 0
LEF4 $65
line 65
;65:		ent->think = multi_wait;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 multi_wait
ASGNP4
line 66
;66:		ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CVIF4 4
CNSTF4 1148846080
ADDRLP4 8
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 800
ADDP4
INDIRF4
CNSTF4 1073741824
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
ADDF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 67
;67:	} else {
ADDRGP4 $66
JUMPV
LABELV $65
line 70
;68:		// we can't just remove (self) here, because this is a touch function
;69:		// called while looping through area links...
;70:		ent->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTP4 0
ASGNP4
line 71
;71:		ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 72
;72:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 73
;73:	}
LABELV $66
line 74
;74:}
LABELV $56
endproc multi_trigger 12 8
export Use_Multi
proc Use_Multi 0 8
line 76
;75:
;76:void Use_Multi( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 77
;77:	multi_trigger( ent, activator );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
line 78
;78:}
LABELV $69
endproc Use_Multi 0 8
export Touch_Multi
proc Touch_Multi 0 8
line 80
;79:
;80:void Touch_Multi( gentity_t *self, gentity_t *other, trace_t *trace ) {
line 81
;81:	if( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $71
line 82
;82:		return;
ADDRGP4 $70
JUMPV
LABELV $71
line 84
;83:	}
;84:	multi_trigger( self, other );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
line 85
;85:}
LABELV $70
endproc Touch_Multi 0 8
export SP_trigger_multiple
proc SP_trigger_multiple 12 12
line 94
;86:
;87:/*QUAKED trigger_multiple (.5 .5 .5) ?
;88:"wait" : Seconds between triggerings, 0.5 default, -1 = one time only.
;89:"random"	wait variance, default is 0
;90:Variable sized repeatable trigger.  Must be targeted at one or more entities.
;91:so, the basic time between firing is a random time between
;92:(wait - random) and (wait + random)
;93:*/
;94:void SP_trigger_multiple( gentity_t *ent ) {
line 95
;95:	G_SpawnFloat( "wait", "0.5", &ent->wait );
ADDRGP4 $74
ARGP4
ADDRGP4 $75
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 796
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 96
;96:	G_SpawnFloat( "random", "0", &ent->random );
ADDRGP4 $76
ARGP4
ADDRGP4 $77
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 800
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 98
;97:
;98:	if ( ent->random >= ent->wait && ent->wait >= 0 ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 800
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
LTF4 $78
ADDRLP4 4
INDIRF4
CNSTF4 0
LTF4 $78
line 99
;99:		ent->random = ent->wait - FRAMETIME;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 800
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 100
;100:		G_Printf( "trigger_multiple has random >= wait\n" );
ADDRGP4 $80
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 101
;101:	}
LABELV $78
line 103
;102:
;103:	ent->touch = Touch_Multi;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRGP4 Touch_Multi
ASGNP4
line 104
;104:	ent->use = Use_Multi;
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 Use_Multi
ASGNP4
line 106
;105:
;106:	InitTrigger( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 107
;107:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 108
;108:}
LABELV $73
endproc SP_trigger_multiple 12 12
export trigger_always_think
proc trigger_always_think 4 8
line 120
;109:
;110:
;111:
;112:/*
;113:==============================================================================
;114:
;115:trigger_always
;116:
;117:==============================================================================
;118:*/
;119:
;120:void trigger_always_think( gentity_t *ent ) {
line 121
;121:	G_UseTargets(ent, ent);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 122
;122:	G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 123
;123:}
LABELV $81
endproc trigger_always_think 4 8
export SP_trigger_always
proc SP_trigger_always 0 0
line 128
;124:
;125:/*QUAKED trigger_always (.5 .5 .5) (-8 -8 -8) (8 8 8)
;126:This trigger will always fire.  It is activated by the world.
;127:*/
;128:void SP_trigger_always (gentity_t *ent) {
line 130
;129:	// we must have some delay to make sure our use targets are present
;130:	ent->nextthink = level.time + 300;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 300
ADDI4
ASGNI4
line 131
;131:	ent->think = trigger_always_think;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 trigger_always_think
ASGNP4
line 132
;132:}
LABELV $82
endproc SP_trigger_always 0 0
export trigger_push_touch
proc trigger_push_touch 0 8
line 143
;133:
;134:
;135:/*
;136:==============================================================================
;137:
;138:trigger_push
;139:
;140:==============================================================================
;141:*/
;142:
;143:void trigger_push_touch (gentity_t *self, gentity_t *other, trace_t *trace ) {
line 145
;144:
;145:	if ( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $85
line 146
;146:		return;
ADDRGP4 $84
JUMPV
LABELV $85
line 149
;147:	}
;148:
;149:	BG_TouchJumpPad( &other->client->ps, &self->s );
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BG_TouchJumpPad
CALLV
pop
line 150
;150:}
LABELV $84
endproc trigger_push_touch 0 8
export AimAtTarget
proc AimAtTarget 72 4
line 160
;151:
;152:
;153:/*
;154:=================
;155:AimAtTarget
;156:
;157:Calculate origin2 so the target apogee will be hit
;158:=================
;159:*/
;160:void AimAtTarget( gentity_t *self ) {
line 166
;161:	gentity_t	*ent;
;162:	vec3_t		origin;
;163:	float		height, gravity, time, forward;
;164:	float		dist;
;165:
;166:	VectorAdd( self->r.absmin, self->r.absmax, origin );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 36
INDIRP4
CNSTI4 464
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 476
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 36
INDIRP4
CNSTI4 468
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 480
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 40
INDIRP4
CNSTI4 472
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 484
ADDP4
INDIRF4
ADDF4
ASGNF4
line 167
;167:	VectorScale ( origin, 0.5, origin );
ADDRLP4 44
CNSTF4 1056964608
ASGNF4
ADDRLP4 0
ADDRLP4 44
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 44
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1056964608
ADDRLP4 0+8
INDIRF4
MULF4
ASGNF4
line 169
;168:
;169:	ent = G_PickTarget( self->target );
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 48
INDIRP4
ASGNP4
line 170
;170:	if ( !ent ) {
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $94
line 171
;171:		G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 172
;172:		return;
ADDRGP4 $87
JUMPV
LABELV $94
line 175
;173:	}
;174:
;175:	height = ent->s.origin[2] - origin[2];
ADDRLP4 28
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
line 176
;176:	gravity = g_gravity.value;
ADDRLP4 24
ADDRGP4 g_gravity+8
INDIRF4
ASGNF4
line 177
;177:	time = sqrt( height / ( .5 * gravity ) );
ADDRLP4 28
INDIRF4
CNSTF4 1056964608
ADDRLP4 24
INDIRF4
MULF4
DIVF4
ARGF4
ADDRLP4 52
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 52
INDIRF4
ASGNF4
line 178
;178:	if ( !time ) {
ADDRLP4 16
INDIRF4
CNSTF4 0
NEF4 $98
line 179
;179:		G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 180
;180:		return;
ADDRGP4 $87
JUMPV
LABELV $98
line 184
;181:	}
;182:
;183:	// set s.origin2 to the push velocity
;184:	VectorSubtract ( ent->s.origin, origin, self->s.origin2 );
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
line 185
;185:	self->s.origin2[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
CNSTF4 0
ASGNF4
line 186
;186:	dist = VectorNormalize( self->s.origin2);
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRLP4 56
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 56
INDIRF4
ASGNF4
line 188
;187:
;188:	forward = dist / time;
ADDRLP4 20
ADDRLP4 32
INDIRF4
ADDRLP4 16
INDIRF4
DIVF4
ASGNF4
line 189
;189:	VectorScale( self->s.origin2, forward, self->s.origin2 );
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
line 191
;190:
;191:	self->s.origin2[2] = time * gravity;
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 16
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
line 192
;192:}
LABELV $87
endproc AimAtTarget 72 4
export SP_trigger_push
proc SP_trigger_push 4 4
line 199
;193:
;194:
;195:/*QUAKED trigger_push (.5 .5 .5) ?
;196:Must point at a target_position, which will be the apex of the leap.
;197:This will be client side predicted, unlike target_push
;198:*/
;199:void SP_trigger_push( gentity_t *self ) {
line 200
;200:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 203
;201:
;202:	// unlike other triggers, we need to send this one to the client
;203:	self->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 206
;204:
;205:	// make sure the client precaches this sound
;206:	G_SoundIndex("sound/world/jumppad.wav");
ADDRGP4 $103
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 208
;207:
;208:	self->s.eType = ET_PUSH_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 8
ASGNI4
line 209
;209:	self->touch = trigger_push_touch;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRGP4 trigger_push_touch
ASGNP4
line 210
;210:	self->think = AimAtTarget;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
line 211
;211:	self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 212
;212:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 213
;213:}
LABELV $102
endproc SP_trigger_push 4 4
export Use_target_push
proc Use_target_push 0 12
line 216
;214:
;215:
;216:void Use_target_push( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 217
;217:	if ( !activator->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $106
line 218
;218:		return;
ADDRGP4 $105
JUMPV
LABELV $106
line 221
;219:	}
;220:
;221:	if ( activator->client->ps.pm_type != PM_NORMAL ) {
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $108
line 222
;222:		return;
ADDRGP4 $105
JUMPV
LABELV $108
line 224
;223:	}
;224:	if ( activator->client->ps.powerups[PW_FLIGHT] ) {
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
CNSTI4 0
EQI4 $110
line 225
;225:		return;
ADDRGP4 $105
JUMPV
LABELV $110
line 228
;226:	}
;227:
;228:	VectorCopy (self->s.origin2, activator->client->ps.velocity);
ADDRFP4 8
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 231
;229:
;230:	// play fly sound every 1.5 seconds
;231:	if ( activator->fly_sound_debounce_time < level.time ) {
ADDRFP4 8
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $112
line 232
;232:		activator->fly_sound_debounce_time = level.time + 1500;
ADDRFP4 8
INDIRP4
CNSTI4 724
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 233
;233:		G_Sound( activator, CHAN_AUTO, self->noise_index );
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 234
;234:	}
LABELV $112
line 235
;235:}
LABELV $105
endproc Use_target_push 0 12
export SP_target_push
proc SP_target_push 36 8
line 242
;236:
;237:/*QUAKED target_push (.5 .5 .5) (-8 -8 -8) (8 8 8) bouncepad
;238:Pushes the activator in the direction.of angle, or towards a target apex.
;239:"speed"		defaults to 1000
;240:if "bouncepad", play bounce noise instead of windfly
;241:*/
;242:void SP_target_push( gentity_t *self ) {
line 243
;243:	if (!self->speed) {
ADDRFP4 0
INDIRP4
CNSTI4 672
ADDP4
INDIRF4
CNSTF4 0
NEF4 $117
line 244
;244:		self->speed = 1000;
ADDRFP4 0
INDIRP4
CNSTI4 672
ADDP4
CNSTF4 1148846080
ASGNF4
line 245
;245:	}
LABELV $117
line 246
;246:	G_SetMovedir (self->s.angles, self->s.origin2);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
line 247
;247:	VectorScale (self->s.origin2, self->speed, self->s.origin2);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 672
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 12
INDIRP4
CNSTI4 672
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 20
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 672
ADDP4
INDIRF4
MULF4
ASGNF4
line 249
;248:
;249:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $119
line 250
;250:		self->noise_index = G_SoundIndex("sound/world/jumppad.wav");
ADDRGP4 $103
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 251
;251:	} else {
ADDRGP4 $120
JUMPV
LABELV $119
line 252
;252:		self->noise_index = G_SoundIndex("sound/misc/windfly.wav");
ADDRGP4 $121
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 253
;253:	}
LABELV $120
line 254
;254:	if ( self->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $122
line 255
;255:		VectorCopy( self->s.origin, self->r.absmin );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 464
ADDP4
ADDRLP4 28
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 256
;256:		VectorCopy( self->s.origin, self->r.absmax );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 476
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 257
;257:		self->think = AimAtTarget;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
line 258
;258:		self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 259
;259:	}
LABELV $122
line 260
;260:	self->use = Use_target_push;
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 Use_target_push
ASGNP4
line 261
;261:}
LABELV $116
endproc SP_target_push 36 8
export trigger_teleporter_touch
proc trigger_teleporter_touch 12 12
line 271
;262:
;263:/*
;264:==============================================================================
;265:
;266:trigger_teleport
;267:
;268:==============================================================================
;269:*/
;270:
;271:void trigger_teleporter_touch (gentity_t *self, gentity_t *other, trace_t *trace ) {
line 274
;272:	gentity_t	*dest;
;273:
;274:	if ( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $126
line 275
;275:		return;
ADDRGP4 $125
JUMPV
LABELV $126
line 277
;276:	}
;277:	if ( other->client->ps.pm_type == PM_DEAD ) {
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $128
line 278
;278:		return;
ADDRGP4 $125
JUMPV
LABELV $128
line 281
;279:	}
;280:	// Spectators only?
;281:	if ( ( self->spawnflags & 1 ) && 
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $130
ADDRFP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
CNSTI4 3
EQI4 $130
line 282
;282:		other->client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 283
;283:		return;
ADDRGP4 $125
JUMPV
LABELV $130
line 287
;284:	}
;285:
;286:
;287:	dest = 	G_PickTarget( self->target );
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 288
;288:	if (!dest) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $132
line 289
;289:		G_Printf ("Couldn't find teleporter destination\n");
ADDRGP4 $134
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 290
;290:		return;
ADDRGP4 $125
JUMPV
LABELV $132
line 293
;291:	}
;292:
;293:	TeleportPlayer( other, dest->s.origin, dest->s.angles );
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 TeleportPlayer
CALLV
pop
line 294
;294:}
LABELV $125
endproc trigger_teleporter_touch 12 12
export SP_trigger_teleport
proc SP_trigger_teleport 4 4
line 305
;295:
;296:
;297:/*QUAKED trigger_teleport (.5 .5 .5) ? SPECTATOR
;298:Allows client side prediction of teleportation events.
;299:Must point at a target_position, which will be the teleport destination.
;300:
;301:If spectator is set, only spectators can use this teleport
;302:Spectator teleporters are not normally placed in the editor, but are created
;303:automatically near doors to allow spectators to move through them
;304:*/
;305:void SP_trigger_teleport( gentity_t *self ) {
line 306
;306:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 310
;307:
;308:	// unlike other triggers, we need to send this one to the client
;309:	// unless is a spectator trigger
;310:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $136
line 311
;311:		self->r.svFlags |= SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 312
;312:	} else {
ADDRGP4 $137
JUMPV
LABELV $136
line 313
;313:		self->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 314
;314:	}
LABELV $137
line 317
;315:
;316:	// make sure the client precaches this sound
;317:	G_SoundIndex("sound/world/jumppad.wav");
ADDRGP4 $103
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 319
;318:
;319:	self->s.eType = ET_TELEPORT_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 9
ASGNI4
line 320
;320:	self->touch = trigger_teleporter_touch;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRGP4 trigger_teleporter_touch
ASGNP4
line 322
;321:
;322:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 323
;323:}
LABELV $135
endproc SP_trigger_teleport 4 4
export hurt_use
proc hurt_use 0 4
line 346
;324:
;325:
;326:/*
;327:==============================================================================
;328:
;329:trigger_hurt
;330:
;331:==============================================================================
;332:*/
;333:
;334:/*QUAKED trigger_hurt (.5 .5 .5) ? START_OFF - SILENT NO_PROTECTION SLOW
;335:Any entity that touches this will be hurt.
;336:It does dmg points of damage each server frame
;337:Targeting the trigger will toggle its on / off state.
;338:
;339:SILENT			supresses playing the sound
;340:SLOW			changes the damage rate to once per second
;341:NO_PROTECTION	*nothing* stops the damage
;342:
;343:"dmg"			default 5 (whole numbers only)
;344:
;345:*/
;346:void hurt_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 347
;347:	if ( self->r.linked ) {
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
CNSTI4 0
EQI4 $139
line 348
;348:		trap_UnlinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 349
;349:	} else {
ADDRGP4 $140
JUMPV
LABELV $139
line 350
;350:		trap_LinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 351
;351:	}
LABELV $140
line 352
;352:}
LABELV $138
endproc hurt_use 0 4
export hurt_touch
proc hurt_touch 12 32
line 354
;353:
;354:void hurt_touch( gentity_t *self, gentity_t *other, trace_t *trace ) {
line 357
;355:	int		dflags;
;356:
;357:	if ( !other->takedamage ) {
ADDRFP4 4
INDIRP4
CNSTI4 736
ADDP4
INDIRI4
CNSTI4 0
NEI4 $142
line 358
;358:		return;
ADDRGP4 $141
JUMPV
LABELV $142
line 361
;359:	}
;360:
;361:	if ( self->timestamp > level.time ) {
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $144
line 362
;362:		return;
ADDRGP4 $141
JUMPV
LABELV $144
line 365
;363:	}
;364:
;365:	if ( self->spawnflags & 16 ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $147
line 366
;366:		self->timestamp = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 367
;367:	} else {
ADDRGP4 $148
JUMPV
LABELV $147
line 368
;368:		self->timestamp = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 369
;369:	}
LABELV $148
line 372
;370:
;371:	// play sound
;372:	if ( !(self->spawnflags & 4) ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $151
line 373
;373:		G_Sound( other, CHAN_AUTO, self->noise_index );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 374
;374:	}
LABELV $151
line 376
;375:
;376:	if (self->spawnflags & 8)
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $153
line 377
;377:		dflags = DAMAGE_NO_PROTECTION;
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRGP4 $154
JUMPV
LABELV $153
line 379
;378:	else
;379:		dflags = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $154
line 380
;380:	G_Damage (other, self, self, NULL, NULL, self->damage, dflags, MOD_TRIGGER_HURT);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 22
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 381
;381:}
LABELV $141
endproc hurt_touch 12 32
export SP_trigger_hurt
proc SP_trigger_hurt 4 4
line 383
;382:
;383:void SP_trigger_hurt( gentity_t *self ) {
line 384
;384:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 386
;385:
;386:	self->noise_index = G_SoundIndex( "sound/world/electro.wav" );
ADDRGP4 $156
ARGP4
ADDRLP4 0
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 387
;387:	self->touch = hurt_touch;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRGP4 hurt_touch
ASGNP4
line 389
;388:
;389:	if ( !self->damage ) {
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
INDIRI4
CNSTI4 0
NEI4 $157
line 390
;390:		self->damage = 5;
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
CNSTI4 5
ASGNI4
line 391
;391:	}
LABELV $157
line 393
;392:
;393:	self->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
CNSTI4 1073741824
ASGNI4
line 395
;394:
;395:	if ( self->spawnflags & 2 ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $159
line 396
;396:		self->use = hurt_use;
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 hurt_use
ASGNP4
line 397
;397:	}
LABELV $159
line 400
;398:
;399:	// link in to the world if starting active
;400:	if ( ! (self->spawnflags & 1) ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $161
line 401
;401:		trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 402
;402:	}
LABELV $161
line 403
;403:}
LABELV $155
endproc SP_trigger_hurt 4 4
export func_timer_think
proc func_timer_think 12 8
line 426
;404:
;405:
;406:/*
;407:==============================================================================
;408:
;409:timer
;410:
;411:==============================================================================
;412:*/
;413:
;414:
;415:/*QUAKED func_timer (0.3 0.1 0.6) (-8 -8 -8) (8 8 8) START_ON
;416:This should be renamed trigger_timer...
;417:Repeatedly fires its targets.
;418:Can be turned on or off by using.
;419:
;420:"wait"			base time between triggering all targets, default is 1
;421:"random"		wait variance, default is 0
;422:so, the basic time between firing is a random time between
;423:(wait - random) and (wait + random)
;424:
;425:*/
;426:void func_timer_think( gentity_t *self ) {
line 427
;427:	G_UseTargets (self, self->activator);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 429
;428:	// set time before next firing
;429:	self->nextthink = level.time + 1000 * ( self->wait + crandom() * self->random );
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CVIF4 4
CNSTF4 1148846080
ADDRLP4 8
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
CNSTF4 1073741824
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 800
ADDP4
INDIRF4
MULF4
ADDF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 430
;430:}
LABELV $163
endproc func_timer_think 12 8
export func_timer_use
proc func_timer_use 0 4
line 432
;431:
;432:void func_timer_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 433
;433:	self->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 772
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 436
;434:
;435:	// if on, turn it off
;436:	if ( self->nextthink ) {
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 0
EQI4 $166
line 437
;437:		self->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 0
ASGNI4
line 438
;438:		return;
ADDRGP4 $165
JUMPV
LABELV $166
line 442
;439:	}
;440:
;441:	// turn it on
;442:	func_timer_think (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 func_timer_think
CALLV
pop
line 443
;443:}
LABELV $165
endproc func_timer_use 0 4
export SP_func_timer
proc SP_func_timer 12 12
line 445
;444:
;445:void SP_func_timer( gentity_t *self ) {
line 446
;446:	G_SpawnFloat( "random", "1", &self->random);
ADDRGP4 $76
ARGP4
ADDRGP4 $169
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 800
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 447
;447:	G_SpawnFloat( "wait", "1", &self->wait );
ADDRGP4 $74
ARGP4
ADDRGP4 $169
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 796
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 449
;448:
;449:	self->use = func_timer_use;
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 func_timer_use
ASGNP4
line 450
;450:	self->think = func_timer_think;
ADDRFP4 0
INDIRP4
CNSTI4 692
ADDP4
ADDRGP4 func_timer_think
ASGNP4
line 452
;451:
;452:	if ( self->random >= self->wait ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 800
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
LTF4 $170
line 453
;453:		self->random = self->wait - FRAMETIME;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 800
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 796
ADDP4
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 454
;454:		G_Printf( "func_timer at %s has random >= wait\n", vtos( self->s.origin ) );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $172
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 455
;455:	}
LABELV $170
line 457
;456:
;457:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $173
line 458
;458:		self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 459
;459:		self->activator = self;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 772
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 460
;460:	}
LABELV $173
line 462
;461:
;462:	self->r.svFlags = SVF_NOCLIENT;
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
CNSTI4 1
ASGNI4
line 463
;463:}
LABELV $168
endproc SP_func_timer 12 12
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
LABELV $172
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 32
byte 1 62
byte 1 61
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $169
byte 1 49
byte 1 0
align 1
LABELV $156
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 47
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 114
byte 1 111
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $134
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
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 100
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 110
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 10
byte 1 0
align 1
LABELV $121
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 47
byte 1 119
byte 1 105
byte 1 110
byte 1 100
byte 1 102
byte 1 108
byte 1 121
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $103
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 47
byte 1 106
byte 1 117
byte 1 109
byte 1 112
byte 1 112
byte 1 97
byte 1 100
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $80
byte 1 116
byte 1 114
byte 1 105
byte 1 103
byte 1 103
byte 1 101
byte 1 114
byte 1 95
byte 1 109
byte 1 117
byte 1 108
byte 1 116
byte 1 105
byte 1 112
byte 1 108
byte 1 101
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 32
byte 1 62
byte 1 61
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $77
byte 1 48
byte 1 0
align 1
LABELV $76
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $75
byte 1 48
byte 1 46
byte 1 53
byte 1 0
align 1
LABELV $74
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 0
