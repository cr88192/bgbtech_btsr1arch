data
export pm_stopspeed
align 4
LABELV pm_stopspeed
byte 4 1120403456
export pm_duckScale
align 4
LABELV pm_duckScale
byte 4 1048576000
export pm_swimScale
align 4
LABELV pm_swimScale
byte 4 1056964608
export pm_wadeScale
align 4
LABELV pm_wadeScale
byte 4 1060320051
export pm_accelerate
align 4
LABELV pm_accelerate
byte 4 1092616192
export pm_airaccelerate
align 4
LABELV pm_airaccelerate
byte 4 1065353216
export pm_wateraccelerate
align 4
LABELV pm_wateraccelerate
byte 4 1082130432
export pm_flyaccelerate
align 4
LABELV pm_flyaccelerate
byte 4 1090519040
export pm_friction
align 4
LABELV pm_friction
byte 4 1086324736
export pm_waterfriction
align 4
LABELV pm_waterfriction
byte 4 1065353216
export pm_flightfriction
align 4
LABELV pm_flightfriction
byte 4 1077936128
export pm_spectatorfriction
align 4
LABELV pm_spectatorfriction
byte 4 1084227584
export c_pmove
align 4
LABELV c_pmove
byte 4 0
export PM_AddEvent
code
proc PM_AddEvent 0 12
file "../bg_pmove.c"
line 58
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
;23:// bg_pmove.c -- both games player movement code
;24:// takes a playerstate and a usercmd as input and returns a modifed playerstate
;25:
;26:#include "q_shared.h"
;27:#include "bg_public.h"
;28:#include "bg_local.h"
;29:
;30:pmove_t		*pm;
;31:pml_t		pml;
;32:
;33:// movement parameters
;34:float	pm_stopspeed = 100.0f;
;35:float	pm_duckScale = 0.25f;
;36:float	pm_swimScale = 0.50f;
;37:float	pm_wadeScale = 0.70f;
;38:
;39:float	pm_accelerate = 10.0f;
;40:float	pm_airaccelerate = 1.0f;
;41:float	pm_wateraccelerate = 4.0f;
;42:float	pm_flyaccelerate = 8.0f;
;43:
;44:float	pm_friction = 6.0f;
;45:float	pm_waterfriction = 1.0f;
;46:float	pm_flightfriction = 3.0f;
;47:float	pm_spectatorfriction = 5.0f;
;48:
;49:int		c_pmove = 0;
;50:
;51:
;52:/*
;53:===============
;54:PM_AddEvent
;55:
;56:===============
;57:*/
;58:void PM_AddEvent( int newEvent ) {
line 59
;59:	BG_AddPredictableEventToPlayerstate( newEvent, 0, pm->ps );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 pm
INDIRP4
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 60
;60:}
LABELV $41
endproc PM_AddEvent 0 12
export PM_AddTouchEnt
proc PM_AddTouchEnt 12 0
line 67
;61:
;62:/*
;63:===============
;64:PM_AddTouchEnt
;65:===============
;66:*/
;67:void PM_AddTouchEnt( int entityNum ) {
line 70
;68:	int		i;
;69:
;70:	if ( entityNum == ENTITYNUM_WORLD ) {
ADDRFP4 0
INDIRI4
CNSTI4 1022
NEI4 $43
line 71
;71:		return;
ADDRGP4 $42
JUMPV
LABELV $43
line 73
;72:	}
;73:	if ( pm->numtouch == MAXTOUCH ) {
ADDRGP4 pm
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 32
NEI4 $45
line 74
;74:		return;
ADDRGP4 $42
JUMPV
LABELV $45
line 78
;75:	}
;76:
;77:	// see if it is already added
;78:	for ( i = 0 ; i < pm->numtouch ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $50
JUMPV
LABELV $47
line 79
;79:		if ( pm->touchents[ i ] == entityNum ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 pm
INDIRP4
CNSTI4 52
ADDP4
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $51
line 80
;80:			return;
ADDRGP4 $42
JUMPV
LABELV $51
line 82
;81:		}
;82:	}
LABELV $48
line 78
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $50
ADDRLP4 0
INDIRI4
ADDRGP4 pm
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
LTI4 $47
line 85
;83:
;84:	// add it
;85:	pm->touchents[pm->numtouch] = entityNum;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 52
ADDP4
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 86
;86:	pm->numtouch++;
ADDRLP4 8
ADDRGP4 pm
INDIRP4
CNSTI4 48
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 87
;87:}
LABELV $42
endproc PM_AddTouchEnt 12 0
proc PM_StartTorsoAnim 8 0
line 94
;88:
;89:/*
;90:===================
;91:PM_StartTorsoAnim
;92:===================
;93:*/
;94:static void PM_StartTorsoAnim( int anim ) {
line 95
;95:	if ( pm->ps->pm_type >= PM_DEAD ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
LTI4 $54
line 96
;96:		return;
ADDRGP4 $53
JUMPV
LABELV $54
line 98
;97:	}
;98:	pm->ps->torsoAnim = ( ( pm->ps->torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT )
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 84
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
ADDRFP4 0
INDIRI4
BORI4
ASGNI4
line 100
;99:		| anim;
;100:}
LABELV $53
endproc PM_StartTorsoAnim 8 0
proc PM_StartLegsAnim 8 0
line 101
;101:static void PM_StartLegsAnim( int anim ) {
line 102
;102:	if ( pm->ps->pm_type >= PM_DEAD ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
LTI4 $57
line 103
;103:		return;
ADDRGP4 $56
JUMPV
LABELV $57
line 105
;104:	}
;105:	if ( pm->ps->legsTimer > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
LEI4 $59
line 106
;106:		return;		// a high priority animation is running
ADDRGP4 $56
JUMPV
LABELV $59
line 108
;107:	}
;108:	pm->ps->legsAnim = ( ( pm->ps->legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT )
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 76
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
ADDRFP4 0
INDIRI4
BORI4
ASGNI4
line 110
;109:		| anim;
;110:}
LABELV $56
endproc PM_StartLegsAnim 8 0
proc PM_ContinueLegsAnim 0 4
line 112
;111:
;112:static void PM_ContinueLegsAnim( int anim ) {
line 113
;113:	if ( ( pm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == anim ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
CNSTI4 -129
BANDI4
ADDRFP4 0
INDIRI4
NEI4 $62
line 114
;114:		return;
ADDRGP4 $61
JUMPV
LABELV $62
line 116
;115:	}
;116:	if ( pm->ps->legsTimer > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
LEI4 $64
line 117
;117:		return;		// a high priority animation is running
ADDRGP4 $61
JUMPV
LABELV $64
line 119
;118:	}
;119:	PM_StartLegsAnim( anim );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 PM_StartLegsAnim
CALLV
pop
line 120
;120:}
LABELV $61
endproc PM_ContinueLegsAnim 0 4
proc PM_ContinueTorsoAnim 0 4
line 122
;121:
;122:static void PM_ContinueTorsoAnim( int anim ) {
line 123
;123:	if ( ( pm->ps->torsoAnim & ~ANIM_TOGGLEBIT ) == anim ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
CNSTI4 -129
BANDI4
ADDRFP4 0
INDIRI4
NEI4 $67
line 124
;124:		return;
ADDRGP4 $66
JUMPV
LABELV $67
line 126
;125:	}
;126:	if ( pm->ps->torsoTimer > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CNSTI4 0
LEI4 $69
line 127
;127:		return;		// a high priority animation is running
ADDRGP4 $66
JUMPV
LABELV $69
line 129
;128:	}
;129:	PM_StartTorsoAnim( anim );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 130
;130:}
LABELV $66
endproc PM_ContinueTorsoAnim 0 4
proc PM_ForceLegsAnim 0 4
line 132
;131:
;132:static void PM_ForceLegsAnim( int anim ) {
line 133
;133:	pm->ps->legsTimer = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 0
ASGNI4
line 134
;134:	PM_StartLegsAnim( anim );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 PM_StartLegsAnim
CALLV
pop
line 135
;135:}
LABELV $71
endproc PM_ForceLegsAnim 0 4
export PM_ClipVelocity
proc PM_ClipVelocity 32 0
line 145
;136:
;137:
;138:/*
;139:==================
;140:PM_ClipVelocity
;141:
;142:Slide off of the impacting surface
;143:==================
;144:*/
;145:void PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce ) {
line 150
;146:	float	backoff;
;147:	float	change;
;148:	int		i;
;149:	
;150:	backoff = DotProduct (in, normal);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 20
CNSTI4 4
ASGNI4
ADDRLP4 24
CNSTI4 8
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRP4
INDIRF4
ADDRLP4 16
INDIRP4
INDIRF4
MULF4
ADDRLP4 12
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRF4
ADDRLP4 16
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 12
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRF4
ADDRLP4 16
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 152
;151:	
;152:	if ( backoff < 0 ) {
ADDRLP4 8
INDIRF4
CNSTF4 0
GEF4 $73
line 153
;153:		backoff *= overbounce;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRFP4 12
INDIRF4
MULF4
ASGNF4
line 154
;154:	} else {
ADDRGP4 $74
JUMPV
LABELV $73
line 155
;155:		backoff /= overbounce;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRFP4 12
INDIRF4
DIVF4
ASGNF4
line 156
;156:	}
LABELV $74
line 158
;157:
;158:	for ( i=0 ; i<3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $75
line 159
;159:		change = normal[i]*backoff;
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
MULF4
ASGNF4
line 160
;160:		out[i] = in[i] - change;
ADDRLP4 28
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 28
INDIRI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 28
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ASGNF4
line 161
;161:	}
LABELV $76
line 158
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $75
line 162
;162:}
LABELV $72
endproc PM_ClipVelocity 32 0
proc PM_Friction 48 4
line 172
;163:
;164:
;165:/*
;166:==================
;167:PM_Friction
;168:
;169:Handles both ground friction and water friction
;170:==================
;171:*/
;172:static void PM_Friction( void ) {
line 178
;173:	vec3_t	vec;
;174:	float	*vel;
;175:	float	speed, newspeed, control;
;176:	float	drop;
;177:	
;178:	vel = pm->ps->velocity;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
line 180
;179:	
;180:	VectorCopy( vel, vec );
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRB
ASGNB 12
line 181
;181:	if ( pml.walking ) {
ADDRGP4 pml+44
INDIRI4
CNSTI4 0
EQI4 $80
line 182
;182:		vec[2] = 0;	// ignore slope movement
ADDRLP4 16+8
CNSTF4 0
ASGNF4
line 183
;183:	}
LABELV $80
line 185
;184:
;185:	speed = VectorLength(vec);
ADDRLP4 16
ARGP4
ADDRLP4 32
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 32
INDIRF4
ASGNF4
line 186
;186:	if (speed < 1) {
ADDRLP4 8
INDIRF4
CNSTF4 1065353216
GEF4 $84
line 187
;187:		vel[0] = 0;
ADDRLP4 0
INDIRP4
CNSTF4 0
ASGNF4
line 188
;188:		vel[1] = 0;		// allow sinking underwater
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTF4 0
ASGNF4
line 190
;189:		// FIXME: still have z friction underwater?
;190:		return;
ADDRGP4 $79
JUMPV
LABELV $84
line 193
;191:	}
;192:
;193:	drop = 0;
ADDRLP4 12
CNSTF4 0
ASGNF4
line 196
;194:
;195:	// apply ground friction
;196:	if ( pm->waterlevel <= 1 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
GTI4 $86
line 197
;197:		if ( pml.walking && !(pml.groundTrace.surfaceFlags & SURF_SLICK) ) {
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRGP4 pml+44
INDIRI4
ADDRLP4 36
INDIRI4
EQI4 $88
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 36
INDIRI4
NEI4 $88
line 199
;198:			// if getting knocked back, no friction
;199:			if ( ! (pm->ps->pm_flags & PMF_TIME_KNOCKBACK) ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
NEI4 $93
line 200
;200:				control = speed < pm_stopspeed ? pm_stopspeed : speed;
ADDRLP4 8
INDIRF4
ADDRGP4 pm_stopspeed
INDIRF4
GEF4 $96
ADDRLP4 40
ADDRGP4 pm_stopspeed
INDIRF4
ASGNF4
ADDRGP4 $97
JUMPV
LABELV $96
ADDRLP4 40
ADDRLP4 8
INDIRF4
ASGNF4
LABELV $97
ADDRLP4 28
ADDRLP4 40
INDIRF4
ASGNF4
line 201
;201:				drop += control*pm_friction*pml.frametime;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 28
INDIRF4
ADDRGP4 pm_friction
INDIRF4
MULF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 202
;202:			}
LABELV $93
line 203
;203:		}
LABELV $88
line 204
;204:	}
LABELV $86
line 207
;205:
;206:	// apply water friction even if just wading
;207:	if ( pm->waterlevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 0
EQI4 $99
line 208
;208:		drop += speed*pm_waterfriction*pm->waterlevel*pml.frametime;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
ADDRGP4 pm_waterfriction
INDIRF4
MULF4
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 209
;209:	}
LABELV $99
line 212
;210:
;211:	// apply flying friction
;212:	if ( pm->ps->powerups[PW_FLIGHT]) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
CNSTI4 0
EQI4 $102
line 213
;213:		drop += speed*pm_flightfriction*pml.frametime;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
ADDRGP4 pm_flightfriction
INDIRF4
MULF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 214
;214:	}
LABELV $102
line 216
;215:
;216:	if ( pm->ps->pm_type == PM_SPECTATOR) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
NEI4 $105
line 217
;217:		drop += speed*pm_spectatorfriction*pml.frametime;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
ADDRGP4 pm_spectatorfriction
INDIRF4
MULF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 218
;218:	}
LABELV $105
line 221
;219:
;220:	// scale the velocity
;221:	newspeed = speed - drop;
ADDRLP4 4
ADDRLP4 8
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ASGNF4
line 222
;222:	if (newspeed < 0) {
ADDRLP4 4
INDIRF4
CNSTF4 0
GEF4 $108
line 223
;223:		newspeed = 0;
ADDRLP4 4
CNSTF4 0
ASGNF4
line 224
;224:	}
LABELV $108
line 225
;225:	newspeed /= speed;
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
DIVF4
ASGNF4
line 227
;226:
;227:	vel[0] = vel[0] * newspeed;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 228
;228:	vel[1] = vel[1] * newspeed;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 229
;229:	vel[2] = vel[2] * newspeed;
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 230
;230:}
LABELV $79
endproc PM_Friction 48 4
proc PM_Accelerate 32 0
line 240
;231:
;232:
;233:/*
;234:==============
;235:PM_Accelerate
;236:
;237:Handles user intended acceleration
;238:==============
;239:*/
;240:static void PM_Accelerate( vec3_t wishdir, float wishspeed, float accel ) {
line 246
;241:#if 1
;242:	// q2 style
;243:	int			i;
;244:	float		addspeed, accelspeed, currentspeed;
;245:
;246:	currentspeed = DotProduct (pm->ps->velocity, wishdir);
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 16
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
INDIRF4
MULF4
ADDRLP4 16
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 16
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 247
;247:	addspeed = wishspeed - currentspeed;
ADDRLP4 8
ADDRFP4 4
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ASGNF4
line 248
;248:	if (addspeed <= 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
GTF4 $111
line 249
;249:		return;
ADDRGP4 $110
JUMPV
LABELV $111
line 251
;250:	}
;251:	accelspeed = accel*pml.frametime*wishspeed;
ADDRLP4 4
ADDRFP4 8
INDIRF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDRFP4 4
INDIRF4
MULF4
ASGNF4
line 252
;252:	if (accelspeed > addspeed) {
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $114
line 253
;253:		accelspeed = addspeed;
ADDRLP4 4
ADDRLP4 8
INDIRF4
ASGNF4
line 254
;254:	}
LABELV $114
line 256
;255:	
;256:	for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $116
line 257
;257:		pm->ps->velocity[i] += accelspeed*wishdir[i];	
ADDRLP4 24
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 28
ADDRLP4 24
INDIRI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
ADDRLP4 24
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 258
;258:	}
LABELV $117
line 256
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $116
line 277
;259:#else
;260:	// proper way (avoids strafe jump maxspeed bug), but feels bad
;261:	vec3_t		wishVelocity;
;262:	vec3_t		pushDir;
;263:	float		pushLen;
;264:	float		canPush;
;265:
;266:	VectorScale( wishdir, wishspeed, wishVelocity );
;267:	VectorSubtract( wishVelocity, pm->ps->velocity, pushDir );
;268:	pushLen = VectorNormalize( pushDir );
;269:
;270:	canPush = accel*pml.frametime*wishspeed;
;271:	if (canPush > pushLen) {
;272:		canPush = pushLen;
;273:	}
;274:
;275:	VectorMA( pm->ps->velocity, canPush, pushDir, pm->ps->velocity );
;276:#endif
;277:}
LABELV $110
endproc PM_Accelerate 32 0
proc PM_CmdScale 44 4
line 290
;278:
;279:
;280:
;281:/*
;282:============
;283:PM_CmdScale
;284:
;285:Returns the scale factor to apply to cmd movements
;286:This allows the clients to use axial -127 to 127 values for all directions
;287:without getting a sqrt(2) distortion in speed.
;288:============
;289:*/
;290:static float PM_CmdScale( usercmd_t *cmd ) {
line 295
;291:	int		max;
;292:	float	total;
;293:	float	scale;
;294:
;295:	max = abs( cmd->forwardmove );
ADDRFP4 0
INDIRP4
CNSTI4 21
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 12
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 296
;296:	if ( abs( cmd->rightmove ) > max ) {
ADDRFP4 0
INDIRP4
CNSTI4 22
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 16
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRLP4 0
INDIRI4
LEI4 $121
line 297
;297:		max = abs( cmd->rightmove );
ADDRFP4 0
INDIRP4
CNSTI4 22
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 20
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 20
INDIRI4
ASGNI4
line 298
;298:	}
LABELV $121
line 299
;299:	if ( abs( cmd->upmove ) > max ) {
ADDRFP4 0
INDIRP4
CNSTI4 23
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 20
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRLP4 0
INDIRI4
LEI4 $123
line 300
;300:		max = abs( cmd->upmove );
ADDRFP4 0
INDIRP4
CNSTI4 23
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 24
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 24
INDIRI4
ASGNI4
line 301
;301:	}
LABELV $123
line 302
;302:	if ( !max ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $125
line 303
;303:		return 0;
CNSTF4 0
RETF4
ADDRGP4 $120
JUMPV
LABELV $125
line 306
;304:	}
;305:
;306:	total = sqrt( cmd->forwardmove * cmd->forwardmove
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 21
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
ADDRLP4 24
INDIRP4
CNSTI4 22
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 36
ADDRLP4 24
INDIRP4
CNSTI4 23
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 28
INDIRI4
ADDRLP4 28
INDIRI4
MULI4
ADDRLP4 32
INDIRI4
ADDRLP4 32
INDIRI4
MULI4
ADDI4
ADDRLP4 36
INDIRI4
ADDRLP4 36
INDIRI4
MULI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 40
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 40
INDIRF4
ASGNF4
line 308
;307:		+ cmd->rightmove * cmd->rightmove + cmd->upmove * cmd->upmove );
;308:	scale = (float)pm->ps->speed * max / ( 127.0 * total );
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 0
INDIRI4
CVIF4 4
MULF4
CNSTF4 1123942400
ADDRLP4 4
INDIRF4
MULF4
DIVF4
ASGNF4
line 310
;309:
;310:	return scale;
ADDRLP4 8
INDIRF4
RETF4
LABELV $120
endproc PM_CmdScale 44 4
proc PM_SetMovementDir 72 0
line 322
;311:}
;312:
;313:
;314:/*
;315:================
;316:PM_SetMovementDir
;317:
;318:Determine the rotation of the legs reletive
;319:to the facing dir
;320:================
;321:*/
;322:static void PM_SetMovementDir( void ) {
line 323
;323:	if ( pm->cmd.forwardmove || pm->cmd.rightmove ) {
ADDRLP4 0
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
NEI4 $130
ADDRLP4 0
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
EQI4 $128
LABELV $130
line 324
;324:		if ( pm->cmd.rightmove == 0 && pm->cmd.forwardmove > 0 ) {
ADDRLP4 8
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
NEI4 $131
ADDRLP4 8
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
LEI4 $131
line 325
;325:			pm->ps->movementDir = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 0
ASGNI4
line 326
;326:		} else if ( pm->cmd.rightmove < 0 && pm->cmd.forwardmove > 0 ) {
ADDRGP4 $129
JUMPV
LABELV $131
ADDRLP4 16
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 20
INDIRI4
GEI4 $133
ADDRLP4 16
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 20
INDIRI4
LEI4 $133
line 327
;327:			pm->ps->movementDir = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 1
ASGNI4
line 328
;328:		} else if ( pm->cmd.rightmove < 0 && pm->cmd.forwardmove == 0 ) {
ADDRGP4 $129
JUMPV
LABELV $133
ADDRLP4 24
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 28
INDIRI4
GEI4 $135
ADDRLP4 24
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 28
INDIRI4
NEI4 $135
line 329
;329:			pm->ps->movementDir = 2;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 2
ASGNI4
line 330
;330:		} else if ( pm->cmd.rightmove < 0 && pm->cmd.forwardmove < 0 ) {
ADDRGP4 $129
JUMPV
LABELV $135
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 36
INDIRI4
GEI4 $137
ADDRLP4 32
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 36
INDIRI4
GEI4 $137
line 331
;331:			pm->ps->movementDir = 3;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 3
ASGNI4
line 332
;332:		} else if ( pm->cmd.rightmove == 0 && pm->cmd.forwardmove < 0 ) {
ADDRGP4 $129
JUMPV
LABELV $137
ADDRLP4 40
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 44
INDIRI4
NEI4 $139
ADDRLP4 40
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 44
INDIRI4
GEI4 $139
line 333
;333:			pm->ps->movementDir = 4;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 4
ASGNI4
line 334
;334:		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove < 0 ) {
ADDRGP4 $129
JUMPV
LABELV $139
ADDRLP4 48
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 52
CNSTI4 0
ASGNI4
ADDRLP4 48
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 52
INDIRI4
LEI4 $141
ADDRLP4 48
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 52
INDIRI4
GEI4 $141
line 335
;335:			pm->ps->movementDir = 5;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 5
ASGNI4
line 336
;336:		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove == 0 ) {
ADDRGP4 $129
JUMPV
LABELV $141
ADDRLP4 56
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 60
CNSTI4 0
ASGNI4
ADDRLP4 56
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 60
INDIRI4
LEI4 $143
ADDRLP4 56
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 60
INDIRI4
NEI4 $143
line 337
;337:			pm->ps->movementDir = 6;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 6
ASGNI4
line 338
;338:		} else if ( pm->cmd.rightmove > 0 && pm->cmd.forwardmove > 0 ) {
ADDRGP4 $129
JUMPV
LABELV $143
ADDRLP4 64
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 68
CNSTI4 0
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 68
INDIRI4
LEI4 $129
ADDRLP4 64
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 68
INDIRI4
LEI4 $129
line 339
;339:			pm->ps->movementDir = 7;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 7
ASGNI4
line 340
;340:		}
line 341
;341:	} else {
ADDRGP4 $129
JUMPV
LABELV $128
line 345
;342:		// if they aren't actively going directly sideways,
;343:		// change the animation to the diagonal so they
;344:		// don't stop too crooked
;345:		if ( pm->ps->movementDir == 2 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
INDIRI4
CNSTI4 2
NEI4 $147
line 346
;346:			pm->ps->movementDir = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 1
ASGNI4
line 347
;347:		} else if ( pm->ps->movementDir == 6 ) {
ADDRGP4 $148
JUMPV
LABELV $147
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
INDIRI4
CNSTI4 6
NEI4 $149
line 348
;348:			pm->ps->movementDir = 7;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 88
ADDP4
CNSTI4 7
ASGNI4
line 349
;349:		} 
LABELV $149
LABELV $148
line 350
;350:	}
LABELV $129
line 351
;351:}
LABELV $127
endproc PM_SetMovementDir 72 0
proc PM_CheckJump 8 4
line 359
;352:
;353:
;354:/*
;355:=============
;356:PM_CheckJump
;357:=============
;358:*/
;359:static qboolean PM_CheckJump( void ) {
line 360
;360:	if ( pm->ps->pm_flags & PMF_RESPAWNED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $152
line 361
;361:		return qfalse;		// don't allow jump until all buttons are up
CNSTI4 0
RETI4
ADDRGP4 $151
JUMPV
LABELV $152
line 364
;362:	}
;363:
;364:	if ( pm->cmd.upmove < 10 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CNSTI4 10
GEI4 $154
line 366
;365:		// not holding jump
;366:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $151
JUMPV
LABELV $154
line 370
;367:	}
;368:
;369:	// must wait for jump to be released
;370:	if ( pm->ps->pm_flags & PMF_JUMP_HELD ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $156
line 372
;371:		// clear upmove so cmdscale doesn't lower running speed
;372:		pm->cmd.upmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
CNSTI1 0
ASGNI1
line 373
;373:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $151
JUMPV
LABELV $156
line 376
;374:	}
;375:
;376:	pml.groundPlane = qfalse;		// jumping away
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 377
;377:	pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 378
;378:	pm->ps->pm_flags |= PMF_JUMP_HELD;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 380
;379:
;380:	pm->ps->groundEntityNum = ENTITYNUM_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
CNSTI4 1023
ASGNI4
line 381
;381:	pm->ps->velocity[2] = JUMP_VELOCITY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1132920832
ASGNF4
line 382
;382:	PM_AddEvent( EV_JUMP );
CNSTI4 14
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 384
;383:
;384:	if ( pm->cmd.forwardmove >= 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LTI4 $160
line 385
;385:		PM_ForceLegsAnim( LEGS_JUMP );
CNSTI4 18
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 386
;386:		pm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -9
BANDI4
ASGNI4
line 387
;387:	} else {
ADDRGP4 $161
JUMPV
LABELV $160
line 388
;388:		PM_ForceLegsAnim( LEGS_JUMPB );
CNSTI4 20
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 389
;389:		pm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 390
;390:	}
LABELV $161
line 392
;391:
;392:	return qtrue;
CNSTI4 1
RETI4
LABELV $151
endproc PM_CheckJump 8 4
proc PM_CheckWaterJump 56 8
line 400
;393:}
;394:
;395:/*
;396:=============
;397:PM_CheckWaterJump
;398:=============
;399:*/
;400:static qboolean	PM_CheckWaterJump( void ) {
line 405
;401:	vec3_t	spot;
;402:	int		cont;
;403:	vec3_t	flatforward;
;404:
;405:	if (pm->ps->pm_time) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
EQI4 $163
line 406
;406:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $162
JUMPV
LABELV $163
line 410
;407:	}
;408:
;409:	// check for water jump
;410:	if ( pm->waterlevel != 2 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 2
EQI4 $165
line 411
;411:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $162
JUMPV
LABELV $165
line 414
;412:	}
;413:
;414:	flatforward[0] = pml.forward[0];
ADDRLP4 12
ADDRGP4 pml
INDIRF4
ASGNF4
line 415
;415:	flatforward[1] = pml.forward[1];
ADDRLP4 12+4
ADDRGP4 pml+4
INDIRF4
ASGNF4
line 416
;416:	flatforward[2] = 0;
ADDRLP4 12+8
CNSTF4 0
ASGNF4
line 417
;417:	VectorNormalize (flatforward);
ADDRLP4 12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 419
;418:
;419:	VectorMA (pm->ps->origin, 30, flatforward, spot);
ADDRLP4 28
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
CNSTF4 1106247680
ASGNF4
ADDRLP4 0
ADDRLP4 28
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1106247680
ADDRLP4 12+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 420
;420:	spot[2] += 4;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1082130432
ADDF4
ASGNF4
line 421
;421:	cont = pm->pointcontents (spot, pm->ps->clientNum );
ADDRLP4 0
ARGP4
ADDRLP4 36
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 40
ADDRLP4 36
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 40
INDIRI4
ASGNI4
line 422
;422:	if ( !(cont & CONTENTS_SOLID) ) {
ADDRLP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $175
line 423
;423:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $162
JUMPV
LABELV $175
line 426
;424:	}
;425:
;426:	spot[2] += 16;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 427
;427:	cont = pm->pointcontents (spot, pm->ps->clientNum );
ADDRLP4 0
ARGP4
ADDRLP4 44
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 48
ADDRLP4 44
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 48
INDIRI4
ASGNI4
line 428
;428:	if ( cont ) {
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $178
line 429
;429:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $162
JUMPV
LABELV $178
line 433
;430:	}
;431:
;432:	// jump out of water
;433:	VectorScale (pml.forward, 200, pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1128792064
ADDRGP4 pml
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1128792064
ADDRGP4 pml+4
INDIRF4
MULF4
ASGNF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1128792064
ADDRGP4 pml+8
INDIRF4
MULF4
ASGNF4
line 434
;434:	pm->ps->velocity[2] = 350;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1135542272
ASGNF4
line 436
;435:
;436:	pm->ps->pm_flags |= PMF_TIME_WATERJUMP;
ADDRLP4 52
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 437
;437:	pm->ps->pm_time = 2000;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 2000
ASGNI4
line 439
;438:
;439:	return qtrue;
CNSTI4 1
RETI4
LABELV $162
endproc PM_CheckWaterJump 56 8
proc PM_WaterJumpMove 12 4
line 452
;440:}
;441:
;442://============================================================================
;443:
;444:
;445:/*
;446:===================
;447:PM_WaterJumpMove
;448:
;449:Flying out of the water
;450:===================
;451:*/
;452:static void PM_WaterJumpMove( void ) {
line 455
;453:	// waterjump has no control, but falls
;454:
;455:	PM_StepSlideMove( qtrue );
CNSTI4 1
ARGI4
ADDRGP4 PM_StepSlideMove
CALLV
pop
line 457
;456:
;457:	pm->ps->velocity[2] -= pm->ps->gravity * pml.frametime;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pml+36
INDIRF4
MULF4
SUBF4
ASGNF4
line 458
;458:	if (pm->ps->velocity[2] < 0) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 0
GEF4 $184
line 460
;459:		// cancel as soon as we are falling down again
;460:		pm->ps->pm_flags &= ~PMF_ALL_TIMES;
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -353
BANDI4
ASGNI4
line 461
;461:		pm->ps->pm_time = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 0
ASGNI4
line 462
;462:	}
LABELV $184
line 463
;463:}
LABELV $182
endproc PM_WaterJumpMove 12 4
proc PM_WaterMove 76 16
line 471
;464:
;465:/*
;466:===================
;467:PM_WaterMove
;468:
;469:===================
;470:*/
;471:static void PM_WaterMove( void ) {
line 479
;472:	int		i;
;473:	vec3_t	wishvel;
;474:	float	wishspeed;
;475:	vec3_t	wishdir;
;476:	float	scale;
;477:	float	vel;
;478:
;479:	if ( PM_CheckWaterJump() ) {
ADDRLP4 40
ADDRGP4 PM_CheckWaterJump
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $187
line 480
;480:		PM_WaterJumpMove();
ADDRGP4 PM_WaterJumpMove
CALLV
pop
line 481
;481:		return;
ADDRGP4 $186
JUMPV
LABELV $187
line 497
;482:	}
;483:#if 0
;484:	// jump = head for surface
;485:	if ( pm->cmd.upmove >= 10 ) {
;486:		if (pm->ps->velocity[2] > -300) {
;487:			if ( pm->watertype == CONTENTS_WATER ) {
;488:				pm->ps->velocity[2] = 100;
;489:			} else if (pm->watertype == CONTENTS_SLIME) {
;490:				pm->ps->velocity[2] = 80;
;491:			} else {
;492:				pm->ps->velocity[2] = 50;
;493:			}
;494:		}
;495:	}
;496:#endif
;497:	PM_Friction ();
ADDRGP4 PM_Friction
CALLV
pop
line 499
;498:
;499:	scale = PM_CmdScale( &pm->cmd );
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 44
ADDRGP4 PM_CmdScale
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 44
INDIRF4
ASGNF4
line 503
;500:	//
;501:	// user intentions
;502:	//
;503:	if ( !scale ) {
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $189
line 504
;504:		wishvel[0] = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 505
;505:		wishvel[1] = 0;
ADDRLP4 8+4
CNSTF4 0
ASGNF4
line 506
;506:		wishvel[2] = -60;		// sink towards bottom
ADDRLP4 8+8
CNSTF4 3262119936
ASGNF4
line 507
;507:	} else {
ADDRGP4 $190
JUMPV
LABELV $189
line 508
;508:		for (i=0 ; i<3 ; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $193
line 509
;509:			wishvel[i] = scale * pml.forward[i]*pm->cmd.forwardmove + scale * pml.right[i]*pm->cmd.rightmove;
ADDRLP4 48
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 56
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 48
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4
INDIRF4
ADDRLP4 48
INDIRI4
ADDRGP4 pml
ADDP4
INDIRF4
MULF4
ADDRLP4 56
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDRLP4 4
INDIRF4
ADDRLP4 48
INDIRI4
ADDRGP4 pml+12
ADDP4
INDIRF4
MULF4
ADDRLP4 56
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDF4
ASGNF4
LABELV $194
line 508
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $193
line 511
;510:
;511:		wishvel[2] += scale * pm->cmd.upmove;
ADDRLP4 8+8
ADDRLP4 8+8
INDIRF4
ADDRLP4 4
INDIRF4
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDF4
ASGNF4
line 512
;512:	}
LABELV $190
line 514
;513:
;514:	VectorCopy (wishvel, wishdir);
ADDRLP4 24
ADDRLP4 8
INDIRB
ASGNB 12
line 515
;515:	wishspeed = VectorNormalize(wishdir);
ADDRLP4 24
ARGP4
ADDRLP4 48
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 48
INDIRF4
ASGNF4
line 517
;516:
;517:	if ( wishspeed > pm->ps->speed * pm_swimScale ) {
ADDRLP4 20
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pm_swimScale
INDIRF4
MULF4
LEF4 $199
line 518
;518:		wishspeed = pm->ps->speed * pm_swimScale;
ADDRLP4 20
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pm_swimScale
INDIRF4
MULF4
ASGNF4
line 519
;519:	}
LABELV $199
line 521
;520:
;521:	PM_Accelerate (wishdir, wishspeed, pm_wateraccelerate);
ADDRLP4 24
ARGP4
ADDRLP4 20
INDIRF4
ARGF4
ADDRGP4 pm_wateraccelerate
INDIRF4
ARGF4
ADDRGP4 PM_Accelerate
CALLV
pop
line 524
;522:
;523:	// make sure we can go up slopes easily under water
;524:	if ( pml.groundPlane && DotProduct( pm->ps->velocity, pml.groundTrace.plane.normal ) < 0 ) {
ADDRGP4 pml+48
INDIRI4
CNSTI4 0
EQI4 $201
ADDRLP4 52
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRGP4 pml+52+24
INDIRF4
MULF4
ADDRLP4 52
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRGP4 pml+52+24+4
INDIRF4
MULF4
ADDF4
ADDRLP4 52
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRGP4 pml+52+24+8
INDIRF4
MULF4
ADDF4
CNSTF4 0
GEF4 $201
line 525
;525:		vel = VectorLength(pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 56
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 36
ADDRLP4 56
INDIRF4
ASGNF4
line 527
;526:		// slide along the ground plane
;527:		PM_ClipVelocity (pm->ps->velocity, pml.groundTrace.plane.normal, 
ADDRLP4 60
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 530
;528:			pm->ps->velocity, OVERCLIP );
;529:
;530:		VectorNormalize(pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 531
;531:		VectorScale(pm->ps->velocity, vel, pm->ps->velocity);
ADDRLP4 64
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ASGNF4
ADDRLP4 68
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ASGNF4
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ASGNF4
line 532
;532:	}
LABELV $201
line 534
;533:
;534:	PM_SlideMove( qfalse );
CNSTI4 0
ARGI4
ADDRGP4 PM_SlideMove
CALLI4
pop
line 535
;535:}
LABELV $186
endproc PM_WaterMove 76 16
proc PM_FlyMove 52 12
line 560
;536:
;537:#ifdef MISSIONPACK
;538:/*
;539:===================
;540:PM_InvulnerabilityMove
;541:
;542:Only with the invulnerability powerup
;543:===================
;544:*/
;545:static void PM_InvulnerabilityMove( void ) {
;546:	pm->cmd.forwardmove = 0;
;547:	pm->cmd.rightmove = 0;
;548:	pm->cmd.upmove = 0;
;549:	VectorClear(pm->ps->velocity);
;550:}
;551:#endif
;552:
;553:/*
;554:===================
;555:PM_FlyMove
;556:
;557:Only with the flight powerup
;558:===================
;559:*/
;560:static void PM_FlyMove( void ) {
line 568
;561:	int		i;
;562:	vec3_t	wishvel;
;563:	float	wishspeed;
;564:	vec3_t	wishdir;
;565:	float	scale;
;566:
;567:	// normal slowdown
;568:	PM_Friction ();
ADDRGP4 PM_Friction
CALLV
pop
line 570
;569:
;570:	scale = PM_CmdScale( &pm->cmd );
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 36
ADDRGP4 PM_CmdScale
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 36
INDIRF4
ASGNF4
line 574
;571:	//
;572:	// user intentions
;573:	//
;574:	if ( !scale ) {
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $215
line 575
;575:		wishvel[0] = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 576
;576:		wishvel[1] = 0;
ADDRLP4 8+4
CNSTF4 0
ASGNF4
line 577
;577:		wishvel[2] = 0;
ADDRLP4 8+8
CNSTF4 0
ASGNF4
line 578
;578:	} else {
ADDRGP4 $216
JUMPV
LABELV $215
line 579
;579:		for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $219
line 580
;580:			wishvel[i] = scale * pml.forward[i]*pm->cmd.forwardmove + scale * pml.right[i]*pm->cmd.rightmove;
ADDRLP4 40
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 48
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 40
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4
INDIRF4
ADDRLP4 40
INDIRI4
ADDRGP4 pml
ADDP4
INDIRF4
MULF4
ADDRLP4 48
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDRLP4 4
INDIRF4
ADDRLP4 40
INDIRI4
ADDRGP4 pml+12
ADDP4
INDIRF4
MULF4
ADDRLP4 48
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDF4
ASGNF4
line 581
;581:		}
LABELV $220
line 579
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $219
line 583
;582:
;583:		wishvel[2] += scale * pm->cmd.upmove;
ADDRLP4 8+8
ADDRLP4 8+8
INDIRF4
ADDRLP4 4
INDIRF4
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CVIF4 4
MULF4
ADDF4
ASGNF4
line 584
;584:	}
LABELV $216
line 586
;585:
;586:	VectorCopy (wishvel, wishdir);
ADDRLP4 20
ADDRLP4 8
INDIRB
ASGNB 12
line 587
;587:	wishspeed = VectorNormalize(wishdir);
ADDRLP4 20
ARGP4
ADDRLP4 40
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 40
INDIRF4
ASGNF4
line 589
;588:
;589:	PM_Accelerate (wishdir, wishspeed, pm_flyaccelerate);
ADDRLP4 20
ARGP4
ADDRLP4 32
INDIRF4
ARGF4
ADDRGP4 pm_flyaccelerate
INDIRF4
ARGF4
ADDRGP4 PM_Accelerate
CALLV
pop
line 591
;590:
;591:	PM_StepSlideMove( qfalse );
CNSTI4 0
ARGI4
ADDRGP4 PM_StepSlideMove
CALLV
pop
line 592
;592:}
LABELV $214
endproc PM_FlyMove 52 12
proc PM_AirMove 80 16
line 601
;593:
;594:
;595:/*
;596:===================
;597:PM_AirMove
;598:
;599:===================
;600:*/
;601:static void PM_AirMove( void ) {
line 610
;602:	int			i;
;603:	vec3_t		wishvel;
;604:	float		fmove, smove;
;605:	vec3_t		wishdir;
;606:	float		wishspeed;
;607:	float		scale;
;608:	usercmd_t	cmd;
;609:
;610:	PM_Friction();
ADDRGP4 PM_Friction
CALLV
pop
line 612
;611:
;612:	fmove = pm->cmd.forwardmove;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 613
;613:	smove = pm->cmd.rightmove;
ADDRLP4 20
ADDRGP4 pm
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 615
;614:
;615:	cmd = pm->cmd;
ADDRLP4 44
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
INDIRB
ASGNB 24
line 616
;616:	scale = PM_CmdScale( &cmd );
ADDRLP4 44
ARGP4
ADDRLP4 68
ADDRGP4 PM_CmdScale
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 68
INDIRF4
ASGNF4
line 619
;617:
;618:	// set the movementDir so clients can rotate the legs for strafing
;619:	PM_SetMovementDir();
ADDRGP4 PM_SetMovementDir
CALLV
pop
line 622
;620:
;621:	// project moves down to flat plane
;622:	pml.forward[2] = 0;
ADDRGP4 pml+8
CNSTF4 0
ASGNF4
line 623
;623:	pml.right[2] = 0;
ADDRGP4 pml+12+8
CNSTF4 0
ASGNF4
line 624
;624:	VectorNormalize (pml.forward);
ADDRGP4 pml
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 625
;625:	VectorNormalize (pml.right);
ADDRGP4 pml+12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 627
;626:
;627:	for ( i = 0 ; i < 2 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $230
line 628
;628:		wishvel[i] = pml.forward[i]*fmove + pml.right[i]*smove;
ADDRLP4 72
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 72
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 72
INDIRI4
ADDRGP4 pml
ADDP4
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 72
INDIRI4
ADDRGP4 pml+12
ADDP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ADDF4
ASGNF4
line 629
;629:	}
LABELV $231
line 627
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LTI4 $230
line 630
;630:	wishvel[2] = 0;
ADDRLP4 4+8
CNSTF4 0
ASGNF4
line 632
;631:
;632:	VectorCopy (wishvel, wishdir);
ADDRLP4 24
ADDRLP4 4
INDIRB
ASGNB 12
line 633
;633:	wishspeed = VectorNormalize(wishdir);
ADDRLP4 24
ARGP4
ADDRLP4 72
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 36
ADDRLP4 72
INDIRF4
ASGNF4
line 634
;634:	wishspeed *= scale;
ADDRLP4 36
ADDRLP4 36
INDIRF4
ADDRLP4 40
INDIRF4
MULF4
ASGNF4
line 637
;635:
;636:	// not on ground, so little effect on velocity
;637:	PM_Accelerate (wishdir, wishspeed, pm_airaccelerate);
ADDRLP4 24
ARGP4
ADDRLP4 36
INDIRF4
ARGF4
ADDRGP4 pm_airaccelerate
INDIRF4
ARGF4
ADDRGP4 PM_Accelerate
CALLV
pop
line 642
;638:
;639:	// we may have a ground plane that is very steep, even
;640:	// though we don't have a groundentity
;641:	// slide along the steep plane
;642:	if ( pml.groundPlane ) {
ADDRGP4 pml+48
INDIRI4
CNSTI4 0
EQI4 $236
line 643
;643:		PM_ClipVelocity (pm->ps->velocity, pml.groundTrace.plane.normal, 
ADDRLP4 76
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 645
;644:			pm->ps->velocity, OVERCLIP );
;645:	}
LABELV $236
line 657
;646:
;647:#if 0
;648:	//ZOID:  If we are on the grapple, try stair-stepping
;649:	//this allows a player to use the grapple to pull himself
;650:	//over a ledge
;651:	if (pm->ps->pm_flags & PMF_GRAPPLE_PULL)
;652:		PM_StepSlideMove ( qtrue );
;653:	else
;654:		PM_SlideMove ( qtrue );
;655:#endif
;656:
;657:	PM_StepSlideMove ( qtrue );
CNSTI4 1
ARGI4
ADDRGP4 PM_StepSlideMove
CALLV
pop
line 658
;658:}
LABELV $225
endproc PM_AirMove 80 16
proc PM_GrappleMove 52 4
line 666
;659:
;660:/*
;661:===================
;662:PM_GrappleMove
;663:
;664:===================
;665:*/
;666:static void PM_GrappleMove( void ) {
line 670
;667:	vec3_t vel, v;
;668:	float vlen;
;669:
;670:	VectorScale(pml.forward, -16, v);
ADDRLP4 28
CNSTF4 3246391296
ASGNF4
ADDRLP4 12
ADDRLP4 28
INDIRF4
ADDRGP4 pml
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 28
INDIRF4
ADDRGP4 pml+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
CNSTF4 3246391296
ADDRGP4 pml+8
INDIRF4
MULF4
ASGNF4
line 671
;671:	VectorAdd(pm->ps->grapplePoint, v, v);
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 32
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 32
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDF4
ASGNF4
line 672
;672:	VectorSubtract(v, pm->ps->origin, vel);
ADDRLP4 36
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRF4
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 12+4
INDIRF4
ADDRLP4 36
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 12+8
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 673
;673:	vlen = VectorLength(vel);
ADDRLP4 0
ARGP4
ADDRLP4 40
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 40
INDIRF4
ASGNF4
line 674
;674:	VectorNormalize( vel );
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 676
;675:
;676:	if (vlen <= 100)
ADDRLP4 24
INDIRF4
CNSTF4 1120403456
GTF4 $254
line 677
;677:		VectorScale(vel, 10 * vlen, vel);
ADDRLP4 44
CNSTF4 1092616192
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1092616192
ADDRLP4 24
INDIRF4
MULF4
MULF4
ASGNF4
ADDRGP4 $255
JUMPV
LABELV $254
line 679
;678:	else
;679:		VectorScale(vel, 800, vel);
ADDRLP4 48
CNSTF4 1145569280
ASGNF4
ADDRLP4 0
ADDRLP4 48
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 48
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1145569280
ADDRLP4 0+8
INDIRF4
MULF4
ASGNF4
LABELV $255
line 681
;680:
;681:	VectorCopy(vel, pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 0
INDIRB
ASGNB 12
line 683
;682:
;683:	pml.groundPlane = qfalse;
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 684
;684:}
LABELV $241
endproc PM_GrappleMove 52 4
proc PM_WalkMove 128 16
line 692
;685:
;686:/*
;687:===================
;688:PM_WalkMove
;689:
;690:===================
;691:*/
;692:static void PM_WalkMove( void ) {
line 703
;693:	int			i;
;694:	vec3_t		wishvel;
;695:	float		fmove, smove;
;696:	vec3_t		wishdir;
;697:	float		wishspeed;
;698:	float		scale;
;699:	usercmd_t	cmd;
;700:	float		accelerate;
;701:	float		vel;
;702:
;703:	if ( pm->waterlevel > 2 && DotProduct( pml.forward, pml.groundTrace.plane.normal ) > 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 2
LEI4 $266
ADDRGP4 pml
INDIRF4
ADDRGP4 pml+52+24
INDIRF4
MULF4
ADDRGP4 pml+4
INDIRF4
ADDRGP4 pml+52+24+4
INDIRF4
MULF4
ADDF4
ADDRGP4 pml+8
INDIRF4
ADDRGP4 pml+52+24+8
INDIRF4
MULF4
ADDF4
CNSTF4 0
LEF4 $266
line 705
;704:		// begin swimming
;705:		PM_WaterMove();
ADDRGP4 PM_WaterMove
CALLV
pop
line 706
;706:		return;
ADDRGP4 $265
JUMPV
LABELV $266
line 710
;707:	}
;708:
;709:
;710:	if ( PM_CheckJump () ) {
ADDRLP4 76
ADDRGP4 PM_CheckJump
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
EQI4 $278
line 712
;711:		// jumped away
;712:		if ( pm->waterlevel > 1 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LEI4 $280
line 713
;713:			PM_WaterMove();
ADDRGP4 PM_WaterMove
CALLV
pop
line 714
;714:		} else {
ADDRGP4 $265
JUMPV
LABELV $280
line 715
;715:			PM_AirMove();
ADDRGP4 PM_AirMove
CALLV
pop
line 716
;716:		}
line 717
;717:		return;
ADDRGP4 $265
JUMPV
LABELV $278
line 720
;718:	}
;719:
;720:	PM_Friction ();
ADDRGP4 PM_Friction
CALLV
pop
line 722
;721:
;722:	fmove = pm->cmd.forwardmove;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 723
;723:	smove = pm->cmd.rightmove;
ADDRLP4 20
ADDRGP4 pm
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 725
;724:
;725:	cmd = pm->cmd;
ADDRLP4 48
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
INDIRB
ASGNB 24
line 726
;726:	scale = PM_CmdScale( &cmd );
ADDRLP4 48
ARGP4
ADDRLP4 80
ADDRGP4 PM_CmdScale
CALLF4
ASGNF4
ADDRLP4 44
ADDRLP4 80
INDIRF4
ASGNF4
line 729
;727:
;728:	// set the movementDir so clients can rotate the legs for strafing
;729:	PM_SetMovementDir();
ADDRGP4 PM_SetMovementDir
CALLV
pop
line 732
;730:
;731:	// project moves down to flat plane
;732:	pml.forward[2] = 0;
ADDRGP4 pml+8
CNSTF4 0
ASGNF4
line 733
;733:	pml.right[2] = 0;
ADDRGP4 pml+12+8
CNSTF4 0
ASGNF4
line 736
;734:
;735:	// project the forward and right directions onto the ground plane
;736:	PM_ClipVelocity (pml.forward, pml.groundTrace.plane.normal, pml.forward, OVERCLIP );
ADDRLP4 84
ADDRGP4 pml
ASGNP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 737
;737:	PM_ClipVelocity (pml.right, pml.groundTrace.plane.normal, pml.right, OVERCLIP );
ADDRGP4 pml+12
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRGP4 pml+12
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 739
;738:	//
;739:	VectorNormalize (pml.forward);
ADDRGP4 pml
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 740
;740:	VectorNormalize (pml.right);
ADDRGP4 pml+12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 742
;741:
;742:	for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $292
line 743
;743:		wishvel[i] = pml.forward[i]*fmove + pml.right[i]*smove;
ADDRLP4 88
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 88
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 88
INDIRI4
ADDRGP4 pml
ADDP4
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 88
INDIRI4
ADDRGP4 pml+12
ADDP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ADDF4
ASGNF4
line 744
;744:	}
LABELV $293
line 742
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $292
line 748
;745:	// when going up or down slopes the wish velocity should Not be zero
;746://	wishvel[2] = 0;
;747:
;748:	VectorCopy (wishvel, wishdir);
ADDRLP4 32
ADDRLP4 4
INDIRB
ASGNB 12
line 749
;749:	wishspeed = VectorNormalize(wishdir);
ADDRLP4 32
ARGP4
ADDRLP4 88
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 88
INDIRF4
ASGNF4
line 750
;750:	wishspeed *= scale;
ADDRLP4 24
ADDRLP4 24
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ASGNF4
line 753
;751:
;752:	// clamp the speed lower if ducking
;753:	if ( pm->ps->pm_flags & PMF_DUCKED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $297
line 754
;754:		if ( wishspeed > pm->ps->speed * pm_duckScale ) {
ADDRLP4 24
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pm_duckScale
INDIRF4
MULF4
LEF4 $299
line 755
;755:			wishspeed = pm->ps->speed * pm_duckScale;
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pm_duckScale
INDIRF4
MULF4
ASGNF4
line 756
;756:		}
LABELV $299
line 757
;757:	}
LABELV $297
line 760
;758:
;759:	// clamp the speed lower if wading or walking on the bottom
;760:	if ( pm->waterlevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 0
EQI4 $301
line 763
;761:		float	waterScale;
;762:
;763:		waterScale = pm->waterlevel / 3.0;
ADDRLP4 92
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1077936128
DIVF4
ASGNF4
line 764
;764:		waterScale = 1.0 - ( 1.0 - pm_swimScale ) * waterScale;
ADDRLP4 96
CNSTF4 1065353216
ASGNF4
ADDRLP4 92
ADDRLP4 96
INDIRF4
ADDRLP4 96
INDIRF4
ADDRGP4 pm_swimScale
INDIRF4
SUBF4
ADDRLP4 92
INDIRF4
MULF4
SUBF4
ASGNF4
line 765
;765:		if ( wishspeed > pm->ps->speed * waterScale ) {
ADDRLP4 24
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 92
INDIRF4
MULF4
LEF4 $303
line 766
;766:			wishspeed = pm->ps->speed * waterScale;
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 92
INDIRF4
MULF4
ASGNF4
line 767
;767:		}
LABELV $303
line 768
;768:	}
LABELV $301
line 772
;769:
;770:	// when a player gets hit, they temporarily lose
;771:	// full control, which allows them to be moved a bit
;772:	if ( ( pml.groundTrace.surfaceFlags & SURF_SLICK ) || pm->ps->pm_flags & PMF_TIME_KNOCKBACK ) {
ADDRLP4 92
CNSTI4 0
ASGNI4
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 92
INDIRI4
NEI4 $309
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 64
BANDI4
ADDRLP4 92
INDIRI4
EQI4 $305
LABELV $309
line 773
;773:		accelerate = pm_airaccelerate;
ADDRLP4 72
ADDRGP4 pm_airaccelerate
INDIRF4
ASGNF4
line 774
;774:	} else {
ADDRGP4 $306
JUMPV
LABELV $305
line 775
;775:		accelerate = pm_accelerate;
ADDRLP4 72
ADDRGP4 pm_accelerate
INDIRF4
ASGNF4
line 776
;776:	}
LABELV $306
line 778
;777:
;778:	PM_Accelerate (wishdir, wishspeed, accelerate);
ADDRLP4 32
ARGP4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 72
INDIRF4
ARGF4
ADDRGP4 PM_Accelerate
CALLV
pop
line 783
;779:
;780:	//Com_Printf("velocity = %1.1f %1.1f %1.1f\n", pm->ps->velocity[0], pm->ps->velocity[1], pm->ps->velocity[2]);
;781:	//Com_Printf("velocity1 = %1.1f\n", VectorLength(pm->ps->velocity));
;782:
;783:	if ( ( pml.groundTrace.surfaceFlags & SURF_SLICK ) || pm->ps->pm_flags & PMF_TIME_KNOCKBACK ) {
ADDRLP4 96
CNSTI4 0
ASGNI4
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 96
INDIRI4
NEI4 $314
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 64
BANDI4
ADDRLP4 96
INDIRI4
EQI4 $310
LABELV $314
line 784
;784:		pm->ps->velocity[2] -= pm->ps->gravity * pml.frametime;
ADDRLP4 100
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 104
ADDRLP4 100
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 104
INDIRP4
ADDRLP4 104
INDIRP4
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pml+36
INDIRF4
MULF4
SUBF4
ASGNF4
line 785
;785:	} else {
LABELV $310
line 788
;786:		// don't reset the z velocity for slopes
;787://		pm->ps->velocity[2] = 0;
;788:	}
LABELV $311
line 790
;789:
;790:	vel = VectorLength(pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 100
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 100
INDIRF4
ASGNF4
line 793
;791:
;792:	// slide along the ground plane
;793:	PM_ClipVelocity (pm->ps->velocity, pml.groundTrace.plane.normal, 
ADDRLP4 104
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 797
;794:		pm->ps->velocity, OVERCLIP );
;795:
;796:	// don't decrease velocity when going up or down a slope
;797:	VectorNormalize(pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 798
;798:	VectorScale(pm->ps->velocity, vel, pm->ps->velocity);
ADDRLP4 108
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 108
INDIRP4
ADDRLP4 108
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
ADDRLP4 112
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 112
INDIRP4
ADDRLP4 112
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
ADDRLP4 116
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 116
INDIRP4
ADDRLP4 116
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
line 801
;799:
;800:	// don't do anything if standing still
;801:	if (!pm->ps->velocity[0] && !pm->ps->velocity[1]) {
ADDRLP4 120
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 124
CNSTF4 0
ASGNF4
ADDRLP4 120
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 124
INDIRF4
NEF4 $318
ADDRLP4 120
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 124
INDIRF4
NEF4 $318
line 802
;802:		return;
ADDRGP4 $265
JUMPV
LABELV $318
line 805
;803:	}
;804:
;805:	PM_StepSlideMove( qfalse );
CNSTI4 0
ARGI4
ADDRGP4 PM_StepSlideMove
CALLV
pop
line 809
;806:
;807:	//Com_Printf("velocity2 = %1.1f\n", VectorLength(pm->ps->velocity));
;808:
;809:}
LABELV $265
endproc PM_WalkMove 128 16
proc PM_DeadMove 20 4
line 817
;810:
;811:
;812:/*
;813:==============
;814:PM_DeadMove
;815:==============
;816:*/
;817:static void PM_DeadMove( void ) {
line 820
;818:	float	forward;
;819:
;820:	if ( !pml.walking ) {
ADDRGP4 pml+44
INDIRI4
CNSTI4 0
NEI4 $321
line 821
;821:		return;
ADDRGP4 $320
JUMPV
LABELV $321
line 826
;822:	}
;823:
;824:	// extra friction
;825:
;826:	forward = VectorLength (pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
ASGNF4
line 827
;827:	forward -= 20;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1101004800
SUBF4
ASGNF4
line 828
;828:	if ( forward <= 0 ) {
ADDRLP4 0
INDIRF4
CNSTF4 0
GTF4 $324
line 829
;829:		VectorClear (pm->ps->velocity);
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 830
;830:	} else {
ADDRGP4 $325
JUMPV
LABELV $324
line 831
;831:		VectorNormalize (pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 832
;832:		VectorScale (pm->ps->velocity, forward, pm->ps->velocity);
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 833
;833:	}
LABELV $325
line 834
;834:}
LABELV $320
endproc PM_DeadMove 20 4
proc PM_NoclipMove 104 12
line 842
;835:
;836:
;837:/*
;838:===============
;839:PM_NoclipMove
;840:===============
;841:*/
;842:static void PM_NoclipMove( void ) {
line 851
;843:	float	speed, drop, friction, control, newspeed;
;844:	int			i;
;845:	vec3_t		wishvel;
;846:	float		fmove, smove;
;847:	vec3_t		wishdir;
;848:	float		wishspeed;
;849:	float		scale;
;850:
;851:	pm->ps->viewheight = DEFAULT_VIEWHEIGHT;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
CNSTI4 26
ASGNI4
line 855
;852:
;853:	// friction
;854:
;855:	speed = VectorLength (pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 64
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 64
INDIRF4
ASGNF4
line 856
;856:	if (speed < 1)
ADDRLP4 24
INDIRF4
CNSTF4 1065353216
GEF4 $327
line 857
;857:	{
line 858
;858:		VectorCopy (vec3_origin, pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRGP4 vec3_origin
INDIRB
ASGNB 12
line 859
;859:	}
ADDRGP4 $328
JUMPV
LABELV $327
line 861
;860:	else
;861:	{
line 862
;862:		drop = 0;
ADDRLP4 52
CNSTF4 0
ASGNF4
line 864
;863:
;864:		friction = pm_friction*1.5;	// extra friction
ADDRLP4 56
CNSTF4 1069547520
ADDRGP4 pm_friction
INDIRF4
MULF4
ASGNF4
line 865
;865:		control = speed < pm_stopspeed ? pm_stopspeed : speed;
ADDRLP4 24
INDIRF4
ADDRGP4 pm_stopspeed
INDIRF4
GEF4 $330
ADDRLP4 68
ADDRGP4 pm_stopspeed
INDIRF4
ASGNF4
ADDRGP4 $331
JUMPV
LABELV $330
ADDRLP4 68
ADDRLP4 24
INDIRF4
ASGNF4
LABELV $331
ADDRLP4 60
ADDRLP4 68
INDIRF4
ASGNF4
line 866
;866:		drop += control*friction*pml.frametime;
ADDRLP4 52
ADDRLP4 52
INDIRF4
ADDRLP4 60
INDIRF4
ADDRLP4 56
INDIRF4
MULF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 869
;867:
;868:		// scale the velocity
;869:		newspeed = speed - drop;
ADDRLP4 28
ADDRLP4 24
INDIRF4
ADDRLP4 52
INDIRF4
SUBF4
ASGNF4
line 870
;870:		if (newspeed < 0)
ADDRLP4 28
INDIRF4
CNSTF4 0
GEF4 $333
line 871
;871:			newspeed = 0;
ADDRLP4 28
CNSTF4 0
ASGNF4
LABELV $333
line 872
;872:		newspeed /= speed;
ADDRLP4 28
ADDRLP4 28
INDIRF4
ADDRLP4 24
INDIRF4
DIVF4
ASGNF4
line 874
;873:
;874:		VectorScale (pm->ps->velocity, newspeed, pm->ps->velocity);
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
ADDRLP4 76
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
ADDRLP4 80
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
line 875
;875:	}
LABELV $328
line 878
;876:
;877:	// accelerate
;878:	scale = PM_CmdScale( &pm->cmd );
ADDRGP4 pm
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 68
ADDRGP4 PM_CmdScale
CALLF4
ASGNF4
ADDRLP4 48
ADDRLP4 68
INDIRF4
ASGNF4
line 880
;879:
;880:	fmove = pm->cmd.forwardmove;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 881
;881:	smove = pm->cmd.rightmove;
ADDRLP4 20
ADDRGP4 pm
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ASGNF4
line 883
;882:	
;883:	for (i=0 ; i<3 ; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $335
line 884
;884:		wishvel[i] = pml.forward[i]*fmove + pml.right[i]*smove;
ADDRLP4 72
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 72
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 72
INDIRI4
ADDRGP4 pml
ADDP4
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 72
INDIRI4
ADDRGP4 pml+12
ADDP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $336
line 883
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $335
line 885
;885:	wishvel[2] += pm->cmd.upmove;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CVIF4 4
ADDF4
ASGNF4
line 887
;886:
;887:	VectorCopy (wishvel, wishdir);
ADDRLP4 32
ADDRLP4 4
INDIRB
ASGNB 12
line 888
;888:	wishspeed = VectorNormalize(wishdir);
ADDRLP4 32
ARGP4
ADDRLP4 76
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 44
ADDRLP4 76
INDIRF4
ASGNF4
line 889
;889:	wishspeed *= scale;
ADDRLP4 44
ADDRLP4 44
INDIRF4
ADDRLP4 48
INDIRF4
MULF4
ASGNF4
line 891
;890:
;891:	PM_Accelerate( wishdir, wishspeed, pm_accelerate );
ADDRLP4 32
ARGP4
ADDRLP4 44
INDIRF4
ARGF4
ADDRGP4 pm_accelerate
INDIRF4
ARGF4
ADDRGP4 PM_Accelerate
CALLV
pop
line 894
;892:
;893:	// move
;894:	VectorMA (pm->ps->origin, pml.frametime, pm->ps->velocity, pm->ps->origin);
ADDRLP4 80
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 84
ADDRLP4 80
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 88
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 92
ADDRLP4 88
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 92
INDIRP4
ADDRLP4 92
INDIRP4
INDIRF4
ADDRLP4 88
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 96
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 100
ADDRLP4 96
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 100
INDIRP4
ADDRLP4 100
INDIRP4
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRGP4 pml+36
INDIRF4
MULF4
ADDF4
ASGNF4
line 895
;895:}
LABELV $326
endproc PM_NoclipMove 104 12
proc PM_FootstepForSurface 0 0
line 906
;896:
;897://============================================================================
;898:
;899:/*
;900:================
;901:PM_FootstepForSurface
;902:
;903:Returns an event number apropriate for the groundsurface
;904:================
;905:*/
;906:static int PM_FootstepForSurface( void ) {
line 907
;907:	if ( pml.groundTrace.surfaceFlags & SURF_NOSTEPS ) {
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $345
line 908
;908:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $344
JUMPV
LABELV $345
line 910
;909:	}
;910:	if ( pml.groundTrace.surfaceFlags & SURF_METALSTEPS ) {
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $349
line 911
;911:		return EV_FOOTSTEP_METAL;
CNSTI4 2
RETI4
ADDRGP4 $344
JUMPV
LABELV $349
line 913
;912:	}
;913:	return EV_FOOTSTEP;
CNSTI4 1
RETI4
LABELV $344
endproc PM_FootstepForSurface 0 0
proc PM_CrashLand 52 4
line 924
;914:}
;915:
;916:
;917:/*
;918:=================
;919:PM_CrashLand
;920:
;921:Check for hard landings that generate sound events
;922:=================
;923:*/
;924:static void PM_CrashLand( void ) {
line 932
;925:	float		delta;
;926:	float		dist;
;927:	float		vel, acc;
;928:	float		t;
;929:	float		a, b, c, den;
;930:
;931:	// decide which landing animation to use
;932:	if ( pm->ps->pm_flags & PMF_BACKWARDS_JUMP ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $354
line 933
;933:		PM_ForceLegsAnim( LEGS_LANDB );
CNSTI4 21
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 934
;934:	} else {
ADDRGP4 $355
JUMPV
LABELV $354
line 935
;935:		PM_ForceLegsAnim( LEGS_LAND );
CNSTI4 19
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 936
;936:	}
LABELV $355
line 938
;937:
;938:	pm->ps->legsTimer = TIMER_LAND;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 130
ASGNI4
line 941
;939:
;940:	// calculate the exact velocity on landing
;941:	dist = pm->ps->origin[2] - pml.previous_origin[2];
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRGP4 pml+112+8
INDIRF4
SUBF4
ASGNF4
line 942
;942:	vel = pml.previous_velocity[2];
ADDRLP4 8
ADDRGP4 pml+124+8
INDIRF4
ASGNF4
line 943
;943:	acc = -pm->ps->gravity;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
NEGI4
CVIF4 4
ASGNF4
line 945
;944:
;945:	a = acc / 2;
ADDRLP4 16
ADDRLP4 12
INDIRF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 946
;946:	b = vel;
ADDRLP4 4
ADDRLP4 8
INDIRF4
ASGNF4
line 947
;947:	c = -dist;
ADDRLP4 32
ADDRLP4 24
INDIRF4
NEGF4
ASGNF4
line 949
;948:
;949:	den =  b * b - 4 * a * c;
ADDRLP4 20
ADDRLP4 4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
CNSTF4 1082130432
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 32
INDIRF4
MULF4
SUBF4
ASGNF4
line 950
;950:	if ( den < 0 ) {
ADDRLP4 20
INDIRF4
CNSTF4 0
GEF4 $360
line 951
;951:		return;
ADDRGP4 $353
JUMPV
LABELV $360
line 953
;952:	}
;953:	t = (-b - sqrt( den ) ) / ( 2 * a );
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 40
INDIRF4
SUBF4
CNSTF4 1073741824
ADDRLP4 16
INDIRF4
MULF4
DIVF4
ASGNF4
line 955
;954:
;955:	delta = vel + t * acc;
ADDRLP4 0
ADDRLP4 8
INDIRF4
ADDRLP4 28
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
line 956
;956:	delta = delta*delta * 0.0001;
ADDRLP4 0
CNSTF4 953267991
ADDRLP4 0
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
MULF4
ASGNF4
line 959
;957:
;958:	// ducking while falling doubles damage
;959:	if ( pm->ps->pm_flags & PMF_DUCKED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $362
line 960
;960:		delta *= 2;
ADDRLP4 0
CNSTF4 1073741824
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 961
;961:	}
LABELV $362
line 964
;962:
;963:	// never take falling damage if completely underwater
;964:	if ( pm->waterlevel == 3 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 3
NEI4 $364
line 965
;965:		return;
ADDRGP4 $353
JUMPV
LABELV $364
line 969
;966:	}
;967:
;968:	// reduce falling damage if there is standing water
;969:	if ( pm->waterlevel == 2 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 2
NEI4 $366
line 970
;970:		delta *= 0.25;
ADDRLP4 0
CNSTF4 1048576000
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 971
;971:	}
LABELV $366
line 972
;972:	if ( pm->waterlevel == 1 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
NEI4 $368
line 973
;973:		delta *= 0.5;
ADDRLP4 0
CNSTF4 1056964608
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 974
;974:	}
LABELV $368
line 976
;975:
;976:	if ( delta < 1 ) {
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
GEF4 $370
line 977
;977:		return;
ADDRGP4 $353
JUMPV
LABELV $370
line 984
;978:	}
;979:
;980:	// create a local entity event to play the sound
;981:
;982:	// SURF_NODAMAGE is used for bounce pads where you don't ever
;983:	// want to take damage or play a crunch sound
;984:	if ( !(pml.groundTrace.surfaceFlags & SURF_NODAMAGE) )  {
ADDRGP4 pml+52+44
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $372
line 985
;985:		if ( delta > 60 ) {
ADDRLP4 0
INDIRF4
CNSTF4 1114636288
LEF4 $376
line 986
;986:			PM_AddEvent( EV_FALL_FAR );
CNSTI4 12
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 987
;987:		} else if ( delta > 40 ) {
ADDRGP4 $377
JUMPV
LABELV $376
ADDRLP4 0
INDIRF4
CNSTF4 1109393408
LEF4 $378
line 989
;988:			// this is a pain grunt, so don't play it if dead
;989:			if ( pm->ps->stats[STAT_HEALTH] > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
LEI4 $379
line 990
;990:				PM_AddEvent( EV_FALL_MEDIUM );
CNSTI4 11
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 991
;991:			}
line 992
;992:		} else if ( delta > 7 ) {
ADDRGP4 $379
JUMPV
LABELV $378
ADDRLP4 0
INDIRF4
CNSTF4 1088421888
LEF4 $382
line 993
;993:			PM_AddEvent( EV_FALL_SHORT );
CNSTI4 10
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 994
;994:		} else {
ADDRGP4 $383
JUMPV
LABELV $382
line 995
;995:			PM_AddEvent( PM_FootstepForSurface() );
ADDRLP4 48
ADDRGP4 PM_FootstepForSurface
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 996
;996:		}
LABELV $383
LABELV $379
LABELV $377
line 997
;997:	}
LABELV $372
line 1000
;998:
;999:	// start footstep cycle over
;1000:	pm->ps->bobCycle = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 1001
;1001:}
LABELV $353
endproc PM_CrashLand 52 4
proc PM_CorrectAllSolid 36 28
line 1024
;1002:
;1003:/*
;1004:=============
;1005:PM_CheckStuck
;1006:=============
;1007:*/
;1008:/*
;1009:void PM_CheckStuck(void) {
;1010:	trace_t trace;
;1011:
;1012:	pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, pm->ps->origin, pm->ps->clientNum, pm->tracemask);
;1013:	if (trace.allsolid) {
;1014:		//int shit = qtrue;
;1015:	}
;1016:}
;1017:*/
;1018:
;1019:/*
;1020:=============
;1021:PM_CorrectAllSolid
;1022:=============
;1023:*/
;1024:static int PM_CorrectAllSolid( trace_t *trace ) {
line 1028
;1025:	int			i, j, k;
;1026:	vec3_t		point;
;1027:
;1028:	if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
EQI4 $385
line 1029
;1029:		Com_Printf("%i:allsolid\n", c_pmove);
ADDRGP4 $387
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 1030
;1030:	}
LABELV $385
line 1033
;1031:
;1032:	// jitter around
;1033:	for (i = -1; i <= 1; i++) {
ADDRLP4 20
CNSTI4 -1
ASGNI4
LABELV $388
line 1034
;1034:		for (j = -1; j <= 1; j++) {
ADDRLP4 16
CNSTI4 -1
ASGNI4
LABELV $392
line 1035
;1035:			for (k = -1; k <= 1; k++) {
ADDRLP4 12
CNSTI4 -1
ASGNI4
LABELV $396
line 1036
;1036:				VectorCopy(pm->ps->origin, point);
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1037
;1037:				point[0] += (float) i;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 20
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1038
;1038:				point[1] += (float) j;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 16
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1039
;1039:				point[2] += (float) k;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1040
;1040:				pm->trace (trace, point, pm->mins, pm->maxs, point, pm->ps->clientNum, pm->tracemask);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 24
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 24
INDIRP4
CNSTI4 192
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CALLV
pop
line 1041
;1041:				if ( !trace->allsolid ) {
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $402
line 1042
;1042:					point[0] = pm->ps->origin[0];
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ASGNF4
line 1043
;1043:					point[1] = pm->ps->origin[1];
ADDRLP4 0+4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
line 1044
;1044:					point[2] = pm->ps->origin[2] - 0.25;
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1048576000
SUBF4
ASGNF4
line 1046
;1045:
;1046:					pm->trace (trace, pm->ps->origin, pm->mins, pm->maxs, point, pm->ps->clientNum, pm->tracemask);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 28
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 192
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CALLV
pop
line 1047
;1047:					pml.groundTrace = *trace;
ADDRGP4 pml+52
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 56
line 1048
;1048:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $384
JUMPV
LABELV $402
line 1050
;1049:				}
;1050:			}
LABELV $397
line 1035
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 1
LEI4 $396
line 1051
;1051:		}
LABELV $393
line 1034
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 1
LEI4 $392
line 1052
;1052:	}
LABELV $389
line 1033
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 1
LEI4 $388
line 1054
;1053:
;1054:	pm->ps->groundEntityNum = ENTITYNUM_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
CNSTI4 1023
ASGNI4
line 1055
;1055:	pml.groundPlane = qfalse;
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 1056
;1056:	pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 1058
;1057:
;1058:	return qfalse;
CNSTI4 0
RETI4
LABELV $384
endproc PM_CorrectAllSolid 36 28
proc PM_GroundTraceMissed 80 28
line 1069
;1059:}
;1060:
;1061:
;1062:/*
;1063:=============
;1064:PM_GroundTraceMissed
;1065:
;1066:The ground trace didn't hit a surface, so we are in freefall
;1067:=============
;1068:*/
;1069:static void PM_GroundTraceMissed( void ) {
line 1073
;1070:	trace_t		trace;
;1071:	vec3_t		point;
;1072:
;1073:	if ( pm->ps->groundEntityNum != ENTITYNUM_NONE ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1023
EQI4 $410
line 1075
;1074:		// we just transitioned into freefall
;1075:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
EQI4 $412
line 1076
;1076:			Com_Printf("%i:lift\n", c_pmove);
ADDRGP4 $414
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 1077
;1077:		}
LABELV $412
line 1081
;1078:
;1079:		// if they aren't in a jumping animation and the ground is a ways away, force into it
;1080:		// if we didn't do the trace, the player would be backflipping down staircases
;1081:		VectorCopy( pm->ps->origin, point );
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1082
;1082:		point[2] -= 64;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1115684864
SUBF4
ASGNF4
line 1084
;1083:
;1084:		pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, point, pm->ps->clientNum, pm->tracemask);
ADDRLP4 12
ARGP4
ADDRLP4 68
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 72
ADDRLP4 68
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 192
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 72
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CALLV
pop
line 1085
;1085:		if ( trace.fraction == 1.0 ) {
ADDRLP4 12+8
INDIRF4
CNSTF4 1065353216
NEF4 $416
line 1086
;1086:			if ( pm->cmd.forwardmove >= 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LTI4 $419
line 1087
;1087:				PM_ForceLegsAnim( LEGS_JUMP );
CNSTI4 18
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 1088
;1088:				pm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
ADDRLP4 76
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 -9
BANDI4
ASGNI4
line 1089
;1089:			} else {
ADDRGP4 $420
JUMPV
LABELV $419
line 1090
;1090:				PM_ForceLegsAnim( LEGS_JUMPB );
CNSTI4 20
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 1091
;1091:				pm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
ADDRLP4 76
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1092
;1092:			}
LABELV $420
line 1093
;1093:		}
LABELV $416
line 1094
;1094:	}
LABELV $410
line 1096
;1095:
;1096:	pm->ps->groundEntityNum = ENTITYNUM_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
CNSTI4 1023
ASGNI4
line 1097
;1097:	pml.groundPlane = qfalse;
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 1098
;1098:	pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 1099
;1099:}
LABELV $409
endproc PM_GroundTraceMissed 80 28
proc PM_GroundTrace 88 28
line 1107
;1100:
;1101:
;1102:/*
;1103:=============
;1104:PM_GroundTrace
;1105:=============
;1106:*/
;1107:static void PM_GroundTrace( void ) {
line 1111
;1108:	vec3_t		point;
;1109:	trace_t		trace;
;1110:
;1111:	point[0] = pm->ps->origin[0];
ADDRLP4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ASGNF4
line 1112
;1112:	point[1] = pm->ps->origin[1];
ADDRLP4 56+4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
line 1113
;1113:	point[2] = pm->ps->origin[2] - 0.25;
ADDRLP4 56+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1048576000
SUBF4
ASGNF4
line 1115
;1114:
;1115:	pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, point, pm->ps->clientNum, pm->tracemask);
ADDRLP4 0
ARGP4
ADDRLP4 68
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 72
ADDRLP4 68
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 192
ADDP4
ARGP4
ADDRLP4 56
ARGP4
ADDRLP4 72
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CALLV
pop
line 1116
;1116:	pml.groundTrace = trace;
ADDRGP4 pml+52
ADDRLP4 0
INDIRB
ASGNB 56
line 1119
;1117:
;1118:	// do something corrective if the trace starts in a solid...
;1119:	if ( trace.allsolid ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $427
line 1120
;1120:		if ( !PM_CorrectAllSolid(&trace) )
ADDRLP4 0
ARGP4
ADDRLP4 76
ADDRGP4 PM_CorrectAllSolid
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
NEI4 $429
line 1121
;1121:			return;
ADDRGP4 $423
JUMPV
LABELV $429
line 1122
;1122:	}
LABELV $427
line 1125
;1123:
;1124:	// if the trace didn't hit anything, we are in free fall
;1125:	if ( trace.fraction == 1.0 ) {
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
NEF4 $431
line 1126
;1126:		PM_GroundTraceMissed();
ADDRGP4 PM_GroundTraceMissed
CALLV
pop
line 1127
;1127:		pml.groundPlane = qfalse;
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 1128
;1128:		pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 1129
;1129:		return;
ADDRGP4 $423
JUMPV
LABELV $431
line 1133
;1130:	}
;1131:
;1132:	// check if getting thrown off the ground
;1133:	if ( pm->ps->velocity[2] > 0 && DotProduct( pm->ps->velocity, trace.plane.normal ) > 10 ) {
ADDRLP4 76
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 80
ADDRLP4 76
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ASGNF4
ADDRLP4 80
INDIRF4
CNSTF4 0
LEF4 $436
ADDRLP4 76
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0+24
INDIRF4
MULF4
ADDRLP4 76
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 0+24+4
INDIRF4
MULF4
ADDF4
ADDRLP4 80
INDIRF4
ADDRLP4 0+24+8
INDIRF4
MULF4
ADDF4
CNSTF4 1092616192
LEF4 $436
line 1134
;1134:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
EQI4 $443
line 1135
;1135:			Com_Printf("%i:kickoff\n", c_pmove);
ADDRGP4 $445
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 1136
;1136:		}
LABELV $443
line 1138
;1137:		// go into jump animation
;1138:		if ( pm->cmd.forwardmove >= 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
LTI4 $446
line 1139
;1139:			PM_ForceLegsAnim( LEGS_JUMP );
CNSTI4 18
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 1140
;1140:			pm->ps->pm_flags &= ~PMF_BACKWARDS_JUMP;
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 -9
BANDI4
ASGNI4
line 1141
;1141:		} else {
ADDRGP4 $447
JUMPV
LABELV $446
line 1142
;1142:			PM_ForceLegsAnim( LEGS_JUMPB );
CNSTI4 20
ARGI4
ADDRGP4 PM_ForceLegsAnim
CALLV
pop
line 1143
;1143:			pm->ps->pm_flags |= PMF_BACKWARDS_JUMP;
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
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
line 1144
;1144:		}
LABELV $447
line 1146
;1145:
;1146:		pm->ps->groundEntityNum = ENTITYNUM_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
CNSTI4 1023
ASGNI4
line 1147
;1147:		pml.groundPlane = qfalse;
ADDRGP4 pml+48
CNSTI4 0
ASGNI4
line 1148
;1148:		pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 1149
;1149:		return;
ADDRGP4 $423
JUMPV
LABELV $436
line 1153
;1150:	}
;1151:	
;1152:	// slopes that are too steep will not be considered onground
;1153:	if ( trace.plane.normal[2] < MIN_WALK_NORMAL ) {
ADDRLP4 0+24+8
INDIRF4
CNSTF4 1060320051
GEF4 $450
line 1154
;1154:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
EQI4 $454
line 1155
;1155:			Com_Printf("%i:steep\n", c_pmove);
ADDRGP4 $456
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 1156
;1156:		}
LABELV $454
line 1159
;1157:		// FIXME: if they can't slide down the slope, let them
;1158:		// walk (sharp crevices)
;1159:		pm->ps->groundEntityNum = ENTITYNUM_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
CNSTI4 1023
ASGNI4
line 1160
;1160:		pml.groundPlane = qtrue;
ADDRGP4 pml+48
CNSTI4 1
ASGNI4
line 1161
;1161:		pml.walking = qfalse;
ADDRGP4 pml+44
CNSTI4 0
ASGNI4
line 1162
;1162:		return;
ADDRGP4 $423
JUMPV
LABELV $450
line 1165
;1163:	}
;1164:
;1165:	pml.groundPlane = qtrue;
ADDRGP4 pml+48
CNSTI4 1
ASGNI4
line 1166
;1166:	pml.walking = qtrue;
ADDRGP4 pml+44
CNSTI4 1
ASGNI4
line 1169
;1167:
;1168:	// hitting solid ground will end a waterjump
;1169:	if (pm->ps->pm_flags & PMF_TIME_WATERJUMP)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $461
line 1170
;1170:	{
line 1171
;1171:		pm->ps->pm_flags &= ~(PMF_TIME_WATERJUMP | PMF_TIME_LAND);
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 -289
BANDI4
ASGNI4
line 1172
;1172:		pm->ps->pm_time = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 0
ASGNI4
line 1173
;1173:	}
LABELV $461
line 1175
;1174:
;1175:	if ( pm->ps->groundEntityNum == ENTITYNUM_NONE ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $463
line 1177
;1176:		// just hit the ground
;1177:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 0
EQI4 $465
line 1178
;1178:			Com_Printf("%i:Land\n", c_pmove);
ADDRGP4 $467
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 1179
;1179:		}
LABELV $465
line 1181
;1180:		
;1181:		PM_CrashLand();
ADDRGP4 PM_CrashLand
CALLV
pop
line 1184
;1182:
;1183:		// don't do landing time if we were just going down a slope
;1184:		if ( pml.previous_velocity[2] < -200 ) {
ADDRGP4 pml+124+8
INDIRF4
CNSTF4 3276275712
GEF4 $468
line 1186
;1185:			// don't allow another jump for a little while
;1186:			pm->ps->pm_flags |= PMF_TIME_LAND;
ADDRLP4 84
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1187
;1187:			pm->ps->pm_time = 250;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 250
ASGNI4
line 1188
;1188:		}
LABELV $468
line 1189
;1189:	}
LABELV $463
line 1191
;1190:
;1191:	pm->ps->groundEntityNum = trace.entityNum;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 0+52
INDIRI4
ASGNI4
line 1196
;1192:
;1193:	// don't reset the z velocity for slopes
;1194://	pm->ps->velocity[2] = 0;
;1195:
;1196:	PM_AddTouchEnt( trace.entityNum );
ADDRLP4 0+52
INDIRI4
ARGI4
ADDRGP4 PM_AddTouchEnt
CALLV
pop
line 1197
;1197:}
LABELV $423
endproc PM_GroundTrace 88 28
proc PM_SetWaterLevel 48 8
line 1205
;1198:
;1199:
;1200:/*
;1201:=============
;1202:PM_SetWaterLevel	FIXME: avoid this twice?  certainly if not moving
;1203:=============
;1204:*/
;1205:static void PM_SetWaterLevel( void ) {
line 1214
;1206:	vec3_t		point;
;1207:	int			cont;
;1208:	int			sample1;
;1209:	int			sample2;
;1210:
;1211:	//
;1212:	// get waterlevel, accounting for ducking
;1213:	//
;1214:	pm->waterlevel = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 0
ASGNI4
line 1215
;1215:	pm->watertype = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 0
ASGNI4
line 1217
;1216:
;1217:	point[0] = pm->ps->origin[0];
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ASGNF4
line 1218
;1218:	point[1] = pm->ps->origin[1];
ADDRLP4 0+4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
line 1219
;1219:	point[2] = pm->ps->origin[2] + MINS_Z + 1;	
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 3250585600
ADDF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 1220
;1220:	cont = pm->pointcontents( point, pm->ps->clientNum );
ADDRLP4 0
ARGP4
ADDRLP4 24
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 28
INDIRI4
ASGNI4
line 1222
;1221:
;1222:	if ( cont & MASK_WATER ) {
ADDRLP4 12
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $477
line 1223
;1223:		sample2 = pm->ps->viewheight - MINS_Z;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
INDIRI4
CNSTI4 -24
SUBI4
ASGNI4
line 1224
;1224:		sample1 = sample2 / 2;
ADDRLP4 20
ADDRLP4 16
INDIRI4
CNSTI4 2
DIVI4
ASGNI4
line 1226
;1225:
;1226:		pm->watertype = cont;
ADDRGP4 pm
INDIRP4
CNSTI4 204
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1227
;1227:		pm->waterlevel = 1;
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 1
ASGNI4
line 1228
;1228:		point[2] = pm->ps->origin[2] + MINS_Z + sample1;
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 3250585600
ADDF4
ADDRLP4 20
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1229
;1229:		cont = pm->pointcontents (point, pm->ps->clientNum );
ADDRLP4 0
ARGP4
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 36
INDIRI4
ASGNI4
line 1230
;1230:		if ( cont & MASK_WATER ) {
ADDRLP4 12
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $480
line 1231
;1231:			pm->waterlevel = 2;
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 2
ASGNI4
line 1232
;1232:			point[2] = pm->ps->origin[2] + MINS_Z + sample2;
ADDRLP4 0+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 3250585600
ADDF4
ADDRLP4 16
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1233
;1233:			cont = pm->pointcontents (point, pm->ps->clientNum );
ADDRLP4 0
ARGP4
ADDRLP4 40
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 44
ADDRLP4 40
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 44
INDIRI4
ASGNI4
line 1234
;1234:			if ( cont & MASK_WATER ){
ADDRLP4 12
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $483
line 1235
;1235:				pm->waterlevel = 3;
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 3
ASGNI4
line 1236
;1236:			}
LABELV $483
line 1237
;1237:		}
LABELV $480
line 1238
;1238:	}
LABELV $477
line 1240
;1239:
;1240:}
LABELV $474
endproc PM_SetWaterLevel 48 8
proc PM_CheckDuck 76 28
line 1250
;1241:
;1242:/*
;1243:==============
;1244:PM_CheckDuck
;1245:
;1246:Sets mins, maxs, and pm->ps->viewheight
;1247:==============
;1248:*/
;1249:static void PM_CheckDuck (void)
;1250:{
line 1253
;1251:	trace_t	trace;
;1252:
;1253:	if ( pm->ps->powerups[PW_INVULNERABILITY] ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 368
ADDP4
INDIRI4
CNSTI4 0
EQI4 $486
line 1254
;1254:		if ( pm->ps->pm_flags & PMF_INVULEXPAND ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $488
line 1256
;1255:			// invulnerability sphere has a 42 units radius
;1256:			VectorSet( pm->mins, -42, -42, -42 );
ADDRGP4 pm
INDIRP4
CNSTI4 180
ADDP4
CNSTF4 3257401344
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 184
ADDP4
CNSTF4 3257401344
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 188
ADDP4
CNSTF4 3257401344
ASGNF4
line 1257
;1257:			VectorSet( pm->maxs, 42, 42, 42 );
ADDRGP4 pm
INDIRP4
CNSTI4 192
ADDP4
CNSTF4 1109917696
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 196
ADDP4
CNSTF4 1109917696
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 1109917696
ASGNF4
line 1258
;1258:		}
ADDRGP4 $489
JUMPV
LABELV $488
line 1259
;1259:		else {
line 1260
;1260:			VectorSet( pm->mins, -15, -15, MINS_Z );
ADDRGP4 pm
INDIRP4
CNSTI4 180
ADDP4
CNSTF4 3245342720
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 184
ADDP4
CNSTF4 3245342720
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 188
ADDP4
CNSTF4 3250585600
ASGNF4
line 1261
;1261:			VectorSet( pm->maxs, 15, 15, 16 );
ADDRGP4 pm
INDIRP4
CNSTI4 192
ADDP4
CNSTF4 1097859072
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 196
ADDP4
CNSTF4 1097859072
ASGNF4
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 1098907648
ASGNF4
line 1262
;1262:		}
LABELV $489
line 1263
;1263:		pm->ps->pm_flags |= PMF_DUCKED;
ADDRLP4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1264
;1264:		pm->ps->viewheight = CROUCH_VIEWHEIGHT;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
CNSTI4 12
ASGNI4
line 1265
;1265:		return;
ADDRGP4 $485
JUMPV
LABELV $486
line 1267
;1266:	}
;1267:	pm->ps->pm_flags &= ~PMF_INVULEXPAND;
ADDRLP4 56
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 -16385
BANDI4
ASGNI4
line 1269
;1268:
;1269:	pm->mins[0] = -15;
ADDRGP4 pm
INDIRP4
CNSTI4 180
ADDP4
CNSTF4 3245342720
ASGNF4
line 1270
;1270:	pm->mins[1] = -15;
ADDRGP4 pm
INDIRP4
CNSTI4 184
ADDP4
CNSTF4 3245342720
ASGNF4
line 1272
;1271:
;1272:	pm->maxs[0] = 15;
ADDRGP4 pm
INDIRP4
CNSTI4 192
ADDP4
CNSTF4 1097859072
ASGNF4
line 1273
;1273:	pm->maxs[1] = 15;
ADDRGP4 pm
INDIRP4
CNSTI4 196
ADDP4
CNSTF4 1097859072
ASGNF4
line 1275
;1274:
;1275:	pm->mins[2] = MINS_Z;
ADDRGP4 pm
INDIRP4
CNSTI4 188
ADDP4
CNSTF4 3250585600
ASGNF4
line 1277
;1276:
;1277:	if (pm->ps->pm_type == PM_DEAD)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $490
line 1278
;1278:	{
line 1279
;1279:		pm->maxs[2] = -8;
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 3238002688
ASGNF4
line 1280
;1280:		pm->ps->viewheight = DEAD_VIEWHEIGHT;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
CNSTI4 -16
ASGNI4
line 1281
;1281:		return;
ADDRGP4 $485
JUMPV
LABELV $490
line 1284
;1282:	}
;1283:
;1284:	if (pm->cmd.upmove < 0)
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $492
line 1285
;1285:	{	// duck
line 1286
;1286:		pm->ps->pm_flags |= PMF_DUCKED;
ADDRLP4 60
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1287
;1287:	}
ADDRGP4 $493
JUMPV
LABELV $492
line 1289
;1288:	else
;1289:	{	// stand up if possible
line 1290
;1290:		if (pm->ps->pm_flags & PMF_DUCKED)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $494
line 1291
;1291:		{
line 1293
;1292:			// try to stand up
;1293:			pm->maxs[2] = 32;
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 1107296256
ASGNF4
line 1294
;1294:			pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, pm->ps->origin, pm->ps->clientNum, pm->tracemask );
ADDRLP4 0
ARGP4
ADDRLP4 60
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 64
ADDRLP4 60
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 68
ADDRLP4 64
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 192
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CALLV
pop
line 1295
;1295:			if (!trace.allsolid)
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $496
line 1296
;1296:				pm->ps->pm_flags &= ~PMF_DUCKED;
ADDRLP4 72
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
LABELV $496
line 1297
;1297:		}
LABELV $494
line 1298
;1298:	}
LABELV $493
line 1300
;1299:
;1300:	if (pm->ps->pm_flags & PMF_DUCKED)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $498
line 1301
;1301:	{
line 1302
;1302:		pm->maxs[2] = 16;
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 1098907648
ASGNF4
line 1303
;1303:		pm->ps->viewheight = CROUCH_VIEWHEIGHT;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
CNSTI4 12
ASGNI4
line 1304
;1304:	}
ADDRGP4 $499
JUMPV
LABELV $498
line 1306
;1305:	else
;1306:	{
line 1307
;1307:		pm->maxs[2] = 32;
ADDRGP4 pm
INDIRP4
CNSTI4 200
ADDP4
CNSTF4 1107296256
ASGNF4
line 1308
;1308:		pm->ps->viewheight = DEFAULT_VIEWHEIGHT;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 164
ADDP4
CNSTI4 26
ASGNI4
line 1309
;1309:	}
LABELV $499
line 1310
;1310:}
LABELV $485
endproc PM_CheckDuck 76 28
proc PM_Footsteps 52 4
line 1322
;1311:
;1312:
;1313:
;1314://===================================================================
;1315:
;1316:
;1317:/*
;1318:===============
;1319:PM_Footsteps
;1320:===============
;1321:*/
;1322:static void PM_Footsteps( void ) {
line 1331
;1323:	float		bobmove;
;1324:	int			old;
;1325:	qboolean	footstep;
;1326:
;1327:	//
;1328:	// calculate speed and cycle to be used for
;1329:	// all cyclic walking effects
;1330:	//
;1331:	pm->xyspeed = sqrt( pm->ps->velocity[0] * pm->ps->velocity[0]
ADDRLP4 12
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ASGNF4
ADDRLP4 24
ADDRLP4 16
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ASGNF4
ADDRLP4 20
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ADDRLP4 24
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
ADDF4
ARGF4
ADDRLP4 28
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 12
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 28
INDIRF4
ASGNF4
line 1334
;1332:		+  pm->ps->velocity[1] * pm->ps->velocity[1] );
;1333:
;1334:	if ( pm->ps->groundEntityNum == ENTITYNUM_NONE ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $501
line 1336
;1335:
;1336:		if ( pm->ps->powerups[PW_INVULNERABILITY] ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 368
ADDP4
INDIRI4
CNSTI4 0
EQI4 $503
line 1337
;1337:			PM_ContinueLegsAnim( LEGS_IDLECR );
CNSTI4 23
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1338
;1338:		}
LABELV $503
line 1340
;1339:		// airborne leaves position in cycle intact, but doesn't advance
;1340:		if ( pm->waterlevel > 1 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LEI4 $500
line 1341
;1341:			PM_ContinueLegsAnim( LEGS_SWIM );
CNSTI4 17
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1342
;1342:		}
line 1343
;1343:		return;
ADDRGP4 $500
JUMPV
LABELV $501
line 1347
;1344:	}
;1345:
;1346:	// if not trying to move
;1347:	if ( !pm->cmd.forwardmove && !pm->cmd.rightmove ) {
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ADDRLP4 36
INDIRI4
NEI4 $507
ADDRLP4 32
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 36
INDIRI4
NEI4 $507
line 1348
;1348:		if (  pm->xyspeed < 5 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
CNSTF4 1084227584
GEF4 $500
line 1349
;1349:			pm->ps->bobCycle = 0;	// start at beginning of cycle again
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 1350
;1350:			if ( pm->ps->pm_flags & PMF_DUCKED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $511
line 1351
;1351:				PM_ContinueLegsAnim( LEGS_IDLECR );
CNSTI4 23
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1352
;1352:			} else {
ADDRGP4 $500
JUMPV
LABELV $511
line 1353
;1353:				PM_ContinueLegsAnim( LEGS_IDLE );
CNSTI4 22
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1354
;1354:			}
line 1355
;1355:		}
line 1356
;1356:		return;
ADDRGP4 $500
JUMPV
LABELV $507
line 1360
;1357:	}
;1358:	
;1359:
;1360:	footstep = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1362
;1361:
;1362:	if ( pm->ps->pm_flags & PMF_DUCKED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $513
line 1363
;1363:		bobmove = 0.5;	// ducked characters bob much faster
ADDRLP4 4
CNSTF4 1056964608
ASGNF4
line 1364
;1364:		if ( pm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $515
line 1365
;1365:			PM_ContinueLegsAnim( LEGS_BACKCR );
CNSTI4 32
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1366
;1366:		}
ADDRGP4 $514
JUMPV
LABELV $515
line 1367
;1367:		else {
line 1368
;1368:			PM_ContinueLegsAnim( LEGS_WALKCR );
CNSTI4 13
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1369
;1369:		}
line 1381
;1370:		// ducked characters never play footsteps
;1371:	/*
;1372:	} else 	if ( pm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
;1373:		if ( !( pm->cmd.buttons & BUTTON_WALKING ) ) {
;1374:			bobmove = 0.4;	// faster speeds bob faster
;1375:			footstep = qtrue;
;1376:		} else {
;1377:			bobmove = 0.3;
;1378:		}
;1379:		PM_ContinueLegsAnim( LEGS_BACK );
;1380:	*/
;1381:	} else {
ADDRGP4 $514
JUMPV
LABELV $513
line 1382
;1382:		if ( !( pm->cmd.buttons & BUTTON_WALKING ) ) {
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $517
line 1383
;1383:			bobmove = 0.4f;	// faster speeds bob faster
ADDRLP4 4
CNSTF4 1053609165
ASGNF4
line 1384
;1384:			if ( pm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $519
line 1385
;1385:				PM_ContinueLegsAnim( LEGS_BACK );
CNSTI4 16
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1386
;1386:			}
ADDRGP4 $520
JUMPV
LABELV $519
line 1387
;1387:			else {
line 1388
;1388:				PM_ContinueLegsAnim( LEGS_RUN );
CNSTI4 15
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1389
;1389:			}
LABELV $520
line 1390
;1390:			footstep = qtrue;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1391
;1391:		} else {
ADDRGP4 $518
JUMPV
LABELV $517
line 1392
;1392:			bobmove = 0.3f;	// walking bobs slow
ADDRLP4 4
CNSTF4 1050253722
ASGNF4
line 1393
;1393:			if ( pm->ps->pm_flags & PMF_BACKWARDS_RUN ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $521
line 1394
;1394:				PM_ContinueLegsAnim( LEGS_BACKWALK );
CNSTI4 33
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1395
;1395:			}
ADDRGP4 $522
JUMPV
LABELV $521
line 1396
;1396:			else {
line 1397
;1397:				PM_ContinueLegsAnim( LEGS_WALK );
CNSTI4 14
ARGI4
ADDRGP4 PM_ContinueLegsAnim
CALLV
pop
line 1398
;1398:			}
LABELV $522
line 1399
;1399:		}
LABELV $518
line 1400
;1400:	}
LABELV $514
line 1403
;1401:
;1402:	// check for footstep / splash sounds
;1403:	old = pm->ps->bobCycle;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1404
;1404:	pm->ps->bobCycle = (int)( old + bobmove * pml.msec ) & 255;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
ADDRGP4 pml+40
INDIRI4
CVIF4 4
MULF4
ADDF4
CVFI4 4
CNSTI4 255
BANDI4
ASGNI4
line 1407
;1405:
;1406:	// if we just crossed a cycle boundary, play an apropriate footstep event
;1407:	if ( ( ( old + 64 ) ^ ( pm->ps->bobCycle + 64 ) ) & 128 ) {
ADDRLP4 40
CNSTI4 64
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
BXORI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $524
line 1408
;1408:		if ( pm->waterlevel == 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 0
NEI4 $526
line 1410
;1409:			// on ground will only play sounds if running
;1410:			if ( footstep && !pm->noFootsteps ) {
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRI4
ADDRLP4 44
INDIRI4
EQI4 $527
ADDRGP4 pm
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
NEI4 $527
line 1411
;1411:				PM_AddEvent( PM_FootstepForSurface() );
ADDRLP4 48
ADDRGP4 PM_FootstepForSurface
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1412
;1412:			}
line 1413
;1413:		} else if ( pm->waterlevel == 1 ) {
ADDRGP4 $527
JUMPV
LABELV $526
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
NEI4 $530
line 1415
;1414:			// splashing
;1415:			PM_AddEvent( EV_FOOTSPLASH );
CNSTI4 3
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1416
;1416:		} else if ( pm->waterlevel == 2 ) {
ADDRGP4 $531
JUMPV
LABELV $530
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 2
NEI4 $532
line 1418
;1417:			// wading / swimming at surface
;1418:			PM_AddEvent( EV_SWIM );
CNSTI4 5
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1419
;1419:		} else if ( pm->waterlevel == 3 ) {
ADDRGP4 $533
JUMPV
LABELV $532
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 3
NEI4 $534
line 1422
;1420:			// no sound when completely underwater
;1421:
;1422:		}
LABELV $534
LABELV $533
LABELV $531
LABELV $527
line 1423
;1423:	}
LABELV $524
line 1424
;1424:}
LABELV $500
endproc PM_Footsteps 52 4
proc PM_WaterEvents 16 4
line 1433
;1425:
;1426:/*
;1427:==============
;1428:PM_WaterEvents
;1429:
;1430:Generate sound events for entering and leaving water
;1431:==============
;1432:*/
;1433:static void PM_WaterEvents( void ) {		// FIXME?
line 1437
;1434:	//
;1435:	// if just entered a water volume, play a sound
;1436:	//
;1437:	if (!pml.previous_waterlevel && pm->waterlevel) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 pml+136
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $537
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $537
line 1438
;1438:		PM_AddEvent( EV_WATER_TOUCH );
CNSTI4 15
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1439
;1439:	}
LABELV $537
line 1444
;1440:
;1441:	//
;1442:	// if just completely exited a water volume, play a sound
;1443:	//
;1444:	if (pml.previous_waterlevel && !pm->waterlevel) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 pml+136
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $540
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $540
line 1445
;1445:		PM_AddEvent( EV_WATER_LEAVE );
CNSTI4 16
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1446
;1446:	}
LABELV $540
line 1451
;1447:
;1448:	//
;1449:	// check for head just going under water
;1450:	//
;1451:	if (pml.previous_waterlevel != 3 && pm->waterlevel == 3) {
ADDRLP4 8
CNSTI4 3
ASGNI4
ADDRGP4 pml+136
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $543
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
NEI4 $543
line 1452
;1452:		PM_AddEvent( EV_WATER_UNDER );
CNSTI4 17
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1453
;1453:	}
LABELV $543
line 1458
;1454:
;1455:	//
;1456:	// check for head just coming out of water
;1457:	//
;1458:	if (pml.previous_waterlevel == 3 && pm->waterlevel != 3) {
ADDRLP4 12
CNSTI4 3
ASGNI4
ADDRGP4 pml+136
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $546
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $546
line 1459
;1459:		PM_AddEvent( EV_WATER_CLEAR );
CNSTI4 18
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1460
;1460:	}
LABELV $546
line 1461
;1461:}
LABELV $536
endproc PM_WaterEvents 16 4
proc PM_BeginWeaponChange 8 4
line 1469
;1462:
;1463:
;1464:/*
;1465:===============
;1466:PM_BeginWeaponChange
;1467:===============
;1468:*/
;1469:static void PM_BeginWeaponChange( int weapon ) {
line 1470
;1470:	if ( weapon <= WP_NONE || weapon >= WP_NUM_WEAPONS ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LEI4 $552
ADDRLP4 0
INDIRI4
CNSTI4 11
LTI4 $550
LABELV $552
line 1471
;1471:		return;
ADDRGP4 $549
JUMPV
LABELV $550
line 1474
;1472:	}
;1473:
;1474:	if ( !( pm->ps->stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1
ADDRFP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $553
line 1475
;1475:		return;
ADDRGP4 $549
JUMPV
LABELV $553
line 1478
;1476:	}
;1477:	
;1478:	if ( pm->ps->weaponstate == WEAPON_DROPPING ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $555
line 1479
;1479:		return;
ADDRGP4 $549
JUMPV
LABELV $555
line 1482
;1480:	}
;1481:
;1482:	PM_AddEvent( EV_CHANGE_WEAPON );
CNSTI4 22
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1483
;1483:	pm->ps->weaponstate = WEAPON_DROPPING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 2
ASGNI4
line 1484
;1484:	pm->ps->weaponTime += 200;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 1485
;1485:	PM_StartTorsoAnim( TORSO_DROP );
CNSTI4 9
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1486
;1486:}
LABELV $549
endproc PM_BeginWeaponChange 8 4
proc PM_FinishWeaponChange 12 4
line 1494
;1487:
;1488:
;1489:/*
;1490:===============
;1491:PM_FinishWeaponChange
;1492:===============
;1493:*/
;1494:static void PM_FinishWeaponChange( void ) {
line 1497
;1495:	int		weapon;
;1496:
;1497:	weapon = pm->cmd.weapon;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
CNSTI4 24
ADDP4
INDIRU1
CVUI4 1
ASGNI4
line 1498
;1498:	if ( weapon < WP_NONE || weapon >= WP_NUM_WEAPONS ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $560
ADDRLP4 0
INDIRI4
CNSTI4 11
LTI4 $558
LABELV $560
line 1499
;1499:		weapon = WP_NONE;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1500
;1500:	}
LABELV $558
line 1502
;1501:
;1502:	if ( !( pm->ps->stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
ADDRGP4 pm
INDIRP4
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
NEI4 $561
line 1503
;1503:		weapon = WP_NONE;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1504
;1504:	}
LABELV $561
line 1506
;1505:
;1506:	pm->ps->weapon = weapon;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1507
;1507:	pm->ps->weaponstate = WEAPON_RAISING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 1508
;1508:	pm->ps->weaponTime += 250;
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 250
ADDI4
ASGNI4
line 1509
;1509:	PM_StartTorsoAnim( TORSO_RAISE );
CNSTI4 10
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1510
;1510:}
LABELV $557
endproc PM_FinishWeaponChange 12 4
proc PM_TorsoAnimation 0 4
line 1519
;1511:
;1512:
;1513:/*
;1514:==============
;1515:PM_TorsoAnimation
;1516:
;1517:==============
;1518:*/
;1519:static void PM_TorsoAnimation( void ) {
line 1520
;1520:	if ( pm->ps->weaponstate == WEAPON_READY ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 0
NEI4 $564
line 1521
;1521:		if ( pm->ps->weapon == WP_GAUNTLET ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 1
NEI4 $566
line 1522
;1522:			PM_ContinueTorsoAnim( TORSO_STAND2 );
CNSTI4 12
ARGI4
ADDRGP4 PM_ContinueTorsoAnim
CALLV
pop
line 1523
;1523:		} else {
ADDRGP4 $563
JUMPV
LABELV $566
line 1524
;1524:			PM_ContinueTorsoAnim( TORSO_STAND );
CNSTI4 11
ARGI4
ADDRGP4 PM_ContinueTorsoAnim
CALLV
pop
line 1525
;1525:		}
line 1526
;1526:		return;
LABELV $564
line 1528
;1527:	}
;1528:}
LABELV $563
endproc PM_TorsoAnimation 0 4
proc PM_Weapon 28 4
line 1538
;1529:
;1530:
;1531:/*
;1532:==============
;1533:PM_Weapon
;1534:
;1535:Generates weapon events and modifes the weapon counter
;1536:==============
;1537:*/
;1538:static void PM_Weapon( void ) {
line 1542
;1539:	int		addTime;
;1540:
;1541:	// don't allow attack until all buttons are up
;1542:	if ( pm->ps->pm_flags & PMF_RESPAWNED ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $569
line 1543
;1543:		return;
ADDRGP4 $568
JUMPV
LABELV $569
line 1547
;1544:	}
;1545:
;1546:	// ignore if spectator
;1547:	if ( pm->ps->persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 3
NEI4 $571
line 1548
;1548:		return;
ADDRGP4 $568
JUMPV
LABELV $571
line 1552
;1549:	}
;1550:
;1551:	// check for dead player
;1552:	if ( pm->ps->stats[STAT_HEALTH] <= 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $573
line 1553
;1553:		pm->ps->weapon = WP_NONE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 0
ASGNI4
line 1554
;1554:		return;
ADDRGP4 $568
JUMPV
LABELV $573
line 1558
;1555:	}
;1556:
;1557:	// check for item using
;1558:	if ( pm->cmd.buttons & BUTTON_USE_HOLDABLE ) {
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $575
line 1559
;1559:		if ( ! ( pm->ps->pm_flags & PMF_USE_ITEM_HELD ) ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
NEI4 $576
line 1560
;1560:			if ( bg_itemlist[pm->ps->stats[STAT_HOLDABLE_ITEM]].giTag == HI_MEDKIT
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
CNSTI4 52
ADDRLP4 4
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
MULI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
CNSTI4 2
NEI4 $579
ADDRLP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 25
ADDI4
LTI4 $579
line 1561
;1561:				&& pm->ps->stats[STAT_HEALTH] >= (pm->ps->stats[STAT_MAX_HEALTH] + 25) ) {
line 1563
;1562:				// don't use medkit if at max health
;1563:			} else {
ADDRGP4 $568
JUMPV
LABELV $579
line 1564
;1564:				pm->ps->pm_flags |= PMF_USE_ITEM_HELD;
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1024
BORI4
ASGNI4
line 1565
;1565:				PM_AddEvent( EV_USE_ITEM0 + bg_itemlist[pm->ps->stats[STAT_HOLDABLE_ITEM]].giTag );
CNSTI4 52
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
MULI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
CNSTI4 24
ADDI4
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1566
;1566:				pm->ps->stats[STAT_HOLDABLE_ITEM] = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 188
ADDP4
CNSTI4 0
ASGNI4
line 1567
;1567:			}
line 1568
;1568:			return;
ADDRGP4 $568
JUMPV
line 1570
;1569:		}
;1570:	} else {
LABELV $575
line 1571
;1571:		pm->ps->pm_flags &= ~PMF_USE_ITEM_HELD;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -1025
BANDI4
ASGNI4
line 1572
;1572:	}
LABELV $576
line 1576
;1573:
;1574:
;1575:	// make weapon function
;1576:	if ( pm->ps->weaponTime > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $583
line 1577
;1577:		pm->ps->weaponTime -= pml.msec;
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1578
;1578:	}
LABELV $583
line 1583
;1579:
;1580:	// check for weapon change
;1581:	// can't change if weapon is firing, but can change
;1582:	// again if lowering or raising
;1583:	if ( pm->ps->weaponTime <= 0 || pm->ps->weaponstate != WEAPON_FIRING ) {
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $588
ADDRLP4 4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 3
EQI4 $586
LABELV $588
line 1584
;1584:		if ( pm->ps->weapon != pm->cmd.weapon ) {
ADDRLP4 8
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRU1
CVUI4 1
EQI4 $589
line 1585
;1585:			PM_BeginWeaponChange( pm->cmd.weapon );
ADDRGP4 pm
INDIRP4
CNSTI4 24
ADDP4
INDIRU1
CVUI4 1
ARGI4
ADDRGP4 PM_BeginWeaponChange
CALLV
pop
line 1586
;1586:		}
LABELV $589
line 1587
;1587:	}
LABELV $586
line 1589
;1588:
;1589:	if ( pm->ps->weaponTime > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $591
line 1590
;1590:		return;
ADDRGP4 $568
JUMPV
LABELV $591
line 1594
;1591:	}
;1592:
;1593:	// change weapon if time
;1594:	if ( pm->ps->weaponstate == WEAPON_DROPPING ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $593
line 1595
;1595:		PM_FinishWeaponChange();
ADDRGP4 PM_FinishWeaponChange
CALLV
pop
line 1596
;1596:		return;
ADDRGP4 $568
JUMPV
LABELV $593
line 1599
;1597:	}
;1598:
;1599:	if ( pm->ps->weaponstate == WEAPON_RAISING ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 1
NEI4 $595
line 1600
;1600:		pm->ps->weaponstate = WEAPON_READY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 1601
;1601:		if ( pm->ps->weapon == WP_GAUNTLET ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 1
NEI4 $597
line 1602
;1602:			PM_StartTorsoAnim( TORSO_STAND2 );
CNSTI4 12
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1603
;1603:		} else {
ADDRGP4 $568
JUMPV
LABELV $597
line 1604
;1604:			PM_StartTorsoAnim( TORSO_STAND );
CNSTI4 11
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1605
;1605:		}
line 1606
;1606:		return;
ADDRGP4 $568
JUMPV
LABELV $595
line 1610
;1607:	}
;1608:
;1609:	// check for fire
;1610:	if ( ! (pm->cmd.buttons & BUTTON_ATTACK) ) {
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $599
line 1611
;1611:		pm->ps->weaponTime = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 1612
;1612:		pm->ps->weaponstate = WEAPON_READY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 1613
;1613:		return;
ADDRGP4 $568
JUMPV
LABELV $599
line 1617
;1614:	}
;1615:
;1616:	// start the animation even if out of ammo
;1617:	if ( pm->ps->weapon == WP_GAUNTLET ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 1
NEI4 $601
line 1619
;1618:		// the guantlet only "fires" when it actually hits something
;1619:		if ( !pm->gauntletHit ) {
ADDRGP4 pm
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 0
NEI4 $603
line 1620
;1620:			pm->ps->weaponTime = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 1621
;1621:			pm->ps->weaponstate = WEAPON_READY;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 1622
;1622:			return;
ADDRGP4 $568
JUMPV
LABELV $603
line 1624
;1623:		}
;1624:		PM_StartTorsoAnim( TORSO_ATTACK2 );
CNSTI4 8
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1625
;1625:	} else {
ADDRGP4 $602
JUMPV
LABELV $601
line 1626
;1626:		PM_StartTorsoAnim( TORSO_ATTACK );
CNSTI4 7
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1627
;1627:	}
LABELV $602
line 1629
;1628:
;1629:	pm->ps->weaponstate = WEAPON_FIRING;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 3
ASGNI4
line 1632
;1630:
;1631:	// check for out of ammo
;1632:	if ( ! pm->ps->ammo[ pm->ps->weapon ] ) {
ADDRLP4 8
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 376
ADDP4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $605
line 1633
;1633:		PM_AddEvent( EV_NOAMMO );
CNSTI4 21
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1634
;1634:		pm->ps->weaponTime += 500;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 1635
;1635:		return;
ADDRGP4 $568
JUMPV
LABELV $605
line 1639
;1636:	}
;1637:
;1638:	// take an ammo away if not infinite
;1639:	if ( pm->ps->ammo[ pm->ps->weapon ] != -1 ) {
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
INDIRP4
CNSTI4 376
ADDP4
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $607
line 1640
;1640:		pm->ps->ammo[ pm->ps->weapon ]--;
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
INDIRP4
CNSTI4 376
ADDP4
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1641
;1641:	}
LABELV $607
line 1644
;1642:
;1643:	// fire weapon
;1644:	PM_AddEvent( EV_FIRE_WEAPON );
CNSTI4 23
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1646
;1645:
;1646:	switch( pm->ps->weapon ) {
ADDRLP4 16
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 1
LTI4 $609
ADDRLP4 16
INDIRI4
CNSTI4 10
GTI4 $609
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $622-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $622
address $612
address $615
address $614
address $616
address $617
address $613
address $619
address $618
address $620
address $621
code
LABELV $609
LABELV $612
line 1649
;1647:	default:
;1648:	case WP_GAUNTLET:
;1649:		addTime = 400;
ADDRLP4 0
CNSTI4 400
ASGNI4
line 1650
;1650:		break;
ADDRGP4 $610
JUMPV
LABELV $613
line 1652
;1651:	case WP_LIGHTNING:
;1652:		addTime = 50;
ADDRLP4 0
CNSTI4 50
ASGNI4
line 1653
;1653:		break;
ADDRGP4 $610
JUMPV
LABELV $614
line 1655
;1654:	case WP_SHOTGUN:
;1655:		addTime = 1000;
ADDRLP4 0
CNSTI4 1000
ASGNI4
line 1656
;1656:		break;
ADDRGP4 $610
JUMPV
LABELV $615
line 1658
;1657:	case WP_MACHINEGUN:
;1658:		addTime = 100;
ADDRLP4 0
CNSTI4 100
ASGNI4
line 1659
;1659:		break;
ADDRGP4 $610
JUMPV
LABELV $616
line 1661
;1660:	case WP_GRENADE_LAUNCHER:
;1661:		addTime = 800;
ADDRLP4 0
CNSTI4 800
ASGNI4
line 1662
;1662:		break;
ADDRGP4 $610
JUMPV
LABELV $617
line 1664
;1663:	case WP_ROCKET_LAUNCHER:
;1664:		addTime = 800;
ADDRLP4 0
CNSTI4 800
ASGNI4
line 1665
;1665:		break;
ADDRGP4 $610
JUMPV
LABELV $618
line 1667
;1666:	case WP_PLASMAGUN:
;1667:		addTime = 100;
ADDRLP4 0
CNSTI4 100
ASGNI4
line 1668
;1668:		break;
ADDRGP4 $610
JUMPV
LABELV $619
line 1670
;1669:	case WP_RAILGUN:
;1670:		addTime = 1500;
ADDRLP4 0
CNSTI4 1500
ASGNI4
line 1671
;1671:		break;
ADDRGP4 $610
JUMPV
LABELV $620
line 1673
;1672:	case WP_BFG:
;1673:		addTime = 200;
ADDRLP4 0
CNSTI4 200
ASGNI4
line 1674
;1674:		break;
ADDRGP4 $610
JUMPV
LABELV $621
line 1676
;1675:	case WP_GRAPPLING_HOOK:
;1676:		addTime = 400;
ADDRLP4 0
CNSTI4 400
ASGNI4
line 1677
;1677:		break;
LABELV $610
line 1701
;1678:#ifdef MISSIONPACK
;1679:	case WP_NAILGUN:
;1680:		addTime = 1000;
;1681:		break;
;1682:	case WP_PROX_LAUNCHER:
;1683:		addTime = 800;
;1684:		break;
;1685:	case WP_CHAINGUN:
;1686:		addTime = 30;
;1687:		break;
;1688:#endif
;1689:	}
;1690:
;1691:#ifdef MISSIONPACK
;1692:	if( bg_itemlist[pm->ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT ) {
;1693:		addTime /= 1.5;
;1694:	}
;1695:	else
;1696:	if( bg_itemlist[pm->ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN ) {
;1697:		addTime /= 1.3;
;1698:  }
;1699:  else
;1700:#endif
;1701:	if ( pm->ps->powerups[PW_HASTE] ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 324
ADDP4
INDIRI4
CNSTI4 0
EQI4 $624
line 1702
;1702:		addTime /= 1.3;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1067869798
DIVF4
CVFI4 4
ASGNI4
line 1703
;1703:	}
LABELV $624
line 1705
;1704:
;1705:	pm->ps->weaponTime += addTime;
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 1706
;1706:}
LABELV $568
endproc PM_Weapon 28 4
proc PM_Animate 0 4
line 1714
;1707:
;1708:/*
;1709:================
;1710:PM_Animate
;1711:================
;1712:*/
;1713:
;1714:static void PM_Animate( void ) {
line 1715
;1715:	if ( pm->cmd.buttons & BUTTON_GESTURE ) {
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $627
line 1716
;1716:		if ( pm->ps->torsoTimer == 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CNSTI4 0
NEI4 $629
line 1717
;1717:			PM_StartTorsoAnim( TORSO_GESTURE );
CNSTI4 6
ARGI4
ADDRGP4 PM_StartTorsoAnim
CALLV
pop
line 1718
;1718:			pm->ps->torsoTimer = TIMER_GESTURE;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
CNSTI4 2294
ASGNI4
line 1719
;1719:			PM_AddEvent( EV_TAUNT );
CNSTI4 76
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 1720
;1720:		}
LABELV $629
line 1753
;1721:#ifdef MISSIONPACK
;1722:	} else if ( pm->cmd.buttons & BUTTON_GETFLAG ) {
;1723:		if ( pm->ps->torsoTimer == 0 ) {
;1724:			PM_StartTorsoAnim( TORSO_GETFLAG );
;1725:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1726:		}
;1727:	} else if ( pm->cmd.buttons & BUTTON_GUARDBASE ) {
;1728:		if ( pm->ps->torsoTimer == 0 ) {
;1729:			PM_StartTorsoAnim( TORSO_GUARDBASE );
;1730:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1731:		}
;1732:	} else if ( pm->cmd.buttons & BUTTON_PATROL ) {
;1733:		if ( pm->ps->torsoTimer == 0 ) {
;1734:			PM_StartTorsoAnim( TORSO_PATROL );
;1735:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1736:		}
;1737:	} else if ( pm->cmd.buttons & BUTTON_FOLLOWME ) {
;1738:		if ( pm->ps->torsoTimer == 0 ) {
;1739:			PM_StartTorsoAnim( TORSO_FOLLOWME );
;1740:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1741:		}
;1742:	} else if ( pm->cmd.buttons & BUTTON_AFFIRMATIVE ) {
;1743:		if ( pm->ps->torsoTimer == 0 ) {
;1744:			PM_StartTorsoAnim( TORSO_AFFIRMATIVE);
;1745:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1746:		}
;1747:	} else if ( pm->cmd.buttons & BUTTON_NEGATIVE ) {
;1748:		if ( pm->ps->torsoTimer == 0 ) {
;1749:			PM_StartTorsoAnim( TORSO_NEGATIVE );
;1750:			pm->ps->torsoTimer = 600;	//TIMER_GESTURE;
;1751:		}
;1752:#endif
;1753:	}
LABELV $627
line 1754
;1754:}
LABELV $626
endproc PM_Animate 0 4
proc PM_DropTimers 4 0
line 1762
;1755:
;1756:
;1757:/*
;1758:================
;1759:PM_DropTimers
;1760:================
;1761:*/
;1762:static void PM_DropTimers( void ) {
line 1764
;1763:	// drop misc timing counter
;1764:	if ( pm->ps->pm_time ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
EQI4 $632
line 1765
;1765:		if ( pml.msec >= pm->ps->pm_time ) {
ADDRGP4 pml+40
INDIRI4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
LTI4 $634
line 1766
;1766:			pm->ps->pm_flags &= ~PMF_ALL_TIMES;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -353
BANDI4
ASGNI4
line 1767
;1767:			pm->ps->pm_time = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 0
ASGNI4
line 1768
;1768:		} else {
ADDRGP4 $635
JUMPV
LABELV $634
line 1769
;1769:			pm->ps->pm_time -= pml.msec;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1770
;1770:		}
LABELV $635
line 1771
;1771:	}
LABELV $632
line 1774
;1772:
;1773:	// drop animation counter
;1774:	if ( pm->ps->legsTimer > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
LEI4 $638
line 1775
;1775:		pm->ps->legsTimer -= pml.msec;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1776
;1776:		if ( pm->ps->legsTimer < 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
GEI4 $641
line 1777
;1777:			pm->ps->legsTimer = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 0
ASGNI4
line 1778
;1778:		}
LABELV $641
line 1779
;1779:	}
LABELV $638
line 1781
;1780:
;1781:	if ( pm->ps->torsoTimer > 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CNSTI4 0
LEI4 $643
line 1782
;1782:		pm->ps->torsoTimer -= pml.msec;
ADDRLP4 0
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRGP4 pml+40
INDIRI4
SUBI4
ASGNI4
line 1783
;1783:		if ( pm->ps->torsoTimer < 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CNSTI4 0
GEI4 $646
line 1784
;1784:			pm->ps->torsoTimer = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
CNSTI4 0
ASGNI4
line 1785
;1785:		}
LABELV $646
line 1786
;1786:	}
LABELV $643
line 1787
;1787:}
LABELV $631
endproc PM_DropTimers 4 0
export PM_UpdateViewAngles
proc PM_UpdateViewAngles 24 0
line 1797
;1788:
;1789:/*
;1790:================
;1791:PM_UpdateViewAngles
;1792:
;1793:This can be used as another entry point when only the viewangles
;1794:are being updated isntead of a full move
;1795:================
;1796:*/
;1797:void PM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd ) {
line 1801
;1798:	short		temp;
;1799:	int		i;
;1800:
;1801:	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPINTERMISSION) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 5
EQI4 $651
ADDRLP4 8
INDIRI4
CNSTI4 6
NEI4 $649
LABELV $651
line 1802
;1802:		return;		// no view changes at all
ADDRGP4 $648
JUMPV
LABELV $649
line 1805
;1803:	}
;1804:
;1805:	if ( ps->pm_type != PM_SPECTATOR && ps->stats[STAT_HEALTH] <= 0 ) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
EQI4 $652
ADDRLP4 12
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $652
line 1806
;1806:		return;		// no view changes at all
ADDRGP4 $648
JUMPV
LABELV $652
line 1810
;1807:	}
;1808:
;1809:	// circularly clamp the angles with deltas
;1810:	for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $654
line 1811
;1811:		temp = cmd->angles[i] + ps->delta_angles[i];
ADDRLP4 16
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDP4
INDIRI4
ADDI4
CVII2 4
ASGNI2
line 1812
;1812:		if ( i == PITCH ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $658
line 1814
;1813:			// don't let the player look up or down more than 90 degrees
;1814:			if ( temp > 16000 ) {
ADDRLP4 4
INDIRI2
CVII4 2
CNSTI4 16000
LEI4 $660
line 1815
;1815:				ps->delta_angles[i] = 16000 - cmd->angles[i];
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDP4
CNSTI4 16000
ADDRLP4 20
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1816
;1816:				temp = 16000;
ADDRLP4 4
CNSTI2 16000
ASGNI2
line 1817
;1817:			} else if ( temp < -16000 ) {
ADDRGP4 $661
JUMPV
LABELV $660
ADDRLP4 4
INDIRI2
CVII4 2
CNSTI4 -16000
GEI4 $662
line 1818
;1818:				ps->delta_angles[i] = -16000 - cmd->angles[i];
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
ADDP4
CNSTI4 -16000
ADDRLP4 20
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1819
;1819:				temp = -16000;
ADDRLP4 4
CNSTI2 -16000
ASGNI2
line 1820
;1820:			}
LABELV $662
LABELV $661
line 1821
;1821:		}
LABELV $658
line 1822
;1822:		ps->viewangles[i] = SHORT2ANGLE(temp);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
ADDP4
CNSTF4 1001652224
ADDRLP4 4
INDIRI2
CVII4 2
CVIF4 4
MULF4
ASGNF4
line 1823
;1823:	}
LABELV $655
line 1810
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $654
line 1825
;1824:
;1825:}
LABELV $648
endproc PM_UpdateViewAngles 24 0
export PmoveSingle
proc PmoveSingle 56 16
line 1836
;1826:
;1827:
;1828:/*
;1829:================
;1830:PmoveSingle
;1831:
;1832:================
;1833:*/
;1834:void trap_SnapVector( float *v );
;1835:
;1836:void PmoveSingle (pmove_t *pmove) {
line 1837
;1837:	pm = pmove;
ADDRGP4 pm
ADDRFP4 0
INDIRP4
ASGNP4
line 1841
;1838:
;1839:	// this counter lets us debug movement problems with a journal
;1840:	// by setting a conditional breakpoint fot the previous frame
;1841:	c_pmove++;
ADDRLP4 0
ADDRGP4 c_pmove
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1844
;1842:
;1843:	// clear results
;1844:	pm->numtouch = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 1845
;1845:	pm->watertype = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 0
ASGNI4
line 1846
;1846:	pm->waterlevel = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 0
ASGNI4
line 1848
;1847:
;1848:	if ( pm->ps->stats[STAT_HEALTH] <= 0 ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
GTI4 $665
line 1849
;1849:		pm->tracemask &= ~CONTENTS_BODY;	// corpses can fly through bodies
ADDRLP4 4
ADDRGP4 pm
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -33554433
BANDI4
ASGNI4
line 1850
;1850:	}
LABELV $665
line 1854
;1851:
;1852:	// make sure walking button is clear if they are running, to avoid
;1853:	// proxy no-footsteps cheats
;1854:	if ( abs( pm->cmd.forwardmove ) > 64 || abs( pm->cmd.rightmove ) > 64 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 4
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 64
GTI4 $669
ADDRGP4 pm
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 64
LEI4 $667
LABELV $669
line 1855
;1855:		pm->cmd.buttons &= ~BUTTON_WALKING;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 -17
BANDI4
ASGNI4
line 1856
;1856:	}
LABELV $667
line 1859
;1857:
;1858:	// set the talk balloon flag
;1859:	if ( pm->cmd.buttons & BUTTON_TALK ) {
ADDRGP4 pm
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $670
line 1860
;1860:		pm->ps->eFlags |= EF_TALK;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1861
;1861:	} else {
ADDRGP4 $671
JUMPV
LABELV $670
line 1862
;1862:		pm->ps->eFlags &= ~EF_TALK;
ADDRLP4 12
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 -4097
BANDI4
ASGNI4
line 1863
;1863:	}
LABELV $671
line 1866
;1864:
;1865:	// set the firing flag for continuous beam weapons
;1866:	if ( !(pm->ps->pm_flags & PMF_RESPAWNED) && pm->ps->pm_type != PM_INTERMISSION
ADDRLP4 12
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 512
BANDI4
ADDRLP4 20
INDIRI4
NEI4 $672
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 5
EQI4 $672
ADDRLP4 12
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 20
INDIRI4
EQI4 $672
ADDRLP4 16
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
INDIRP4
CNSTI4 376
ADDP4
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $672
line 1867
;1867:		&& ( pm->cmd.buttons & BUTTON_ATTACK ) && pm->ps->ammo[ pm->ps->weapon ] ) {
line 1868
;1868:		pm->ps->eFlags |= EF_FIRING;
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 1869
;1869:	} else {
ADDRGP4 $673
JUMPV
LABELV $672
line 1870
;1870:		pm->ps->eFlags &= ~EF_FIRING;
ADDRLP4 24
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 -257
BANDI4
ASGNI4
line 1871
;1871:	}
LABELV $673
line 1874
;1872:
;1873:	// clear the respawned flag if attack and use are cleared
;1874:	if ( pm->ps->stats[STAT_HEALTH] > 0 && 
ADDRLP4 24
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRP4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
LEI4 $674
ADDRLP4 24
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 5
BANDI4
ADDRLP4 28
INDIRI4
NEI4 $674
line 1875
;1875:		!( pm->cmd.buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE) ) ) {
line 1876
;1876:		pm->ps->pm_flags &= ~PMF_RESPAWNED;
ADDRLP4 32
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 -513
BANDI4
ASGNI4
line 1877
;1877:	}
LABELV $674
line 1882
;1878:
;1879:	// if talk button is down, dissallow all other input
;1880:	// this is to prevent any possible intercept proxy from
;1881:	// adding fake talk balloons
;1882:	if ( pmove->cmd.buttons & BUTTON_TALK ) {
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $676
line 1885
;1883:		// keep the talk button set tho for when the cmd.serverTime > 66 msec
;1884:		// and the same cmd is used multiple times in Pmove
;1885:		pmove->cmd.buttons = BUTTON_TALK;
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
CNSTI4 2
ASGNI4
line 1886
;1886:		pmove->cmd.forwardmove = 0;
ADDRFP4 0
INDIRP4
CNSTI4 25
ADDP4
CNSTI1 0
ASGNI1
line 1887
;1887:		pmove->cmd.rightmove = 0;
ADDRFP4 0
INDIRP4
CNSTI4 26
ADDP4
CNSTI1 0
ASGNI1
line 1888
;1888:		pmove->cmd.upmove = 0;
ADDRFP4 0
INDIRP4
CNSTI4 27
ADDP4
CNSTI1 0
ASGNI1
line 1889
;1889:	}
LABELV $676
line 1892
;1890:
;1891:	// clear all pmove local vars
;1892:	memset (&pml, 0, sizeof(pml));
ADDRGP4 pml
ARGP4
CNSTI4 0
ARGI4
CNSTI4 140
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1895
;1893:
;1894:	// determine the time
;1895:	pml.msec = pmove->cmd.serverTime - pm->ps->commandTime;
ADDRGP4 pml+40
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRGP4 pm
INDIRP4
INDIRP4
INDIRI4
SUBI4
ASGNI4
line 1896
;1896:	if ( pml.msec < 1 ) {
ADDRGP4 pml+40
INDIRI4
CNSTI4 1
GEI4 $679
line 1897
;1897:		pml.msec = 1;
ADDRGP4 pml+40
CNSTI4 1
ASGNI4
line 1898
;1898:	} else if ( pml.msec > 200 ) {
ADDRGP4 $680
JUMPV
LABELV $679
ADDRGP4 pml+40
INDIRI4
CNSTI4 200
LEI4 $683
line 1899
;1899:		pml.msec = 200;
ADDRGP4 pml+40
CNSTI4 200
ASGNI4
line 1900
;1900:	}
LABELV $683
LABELV $680
line 1901
;1901:	pm->ps->commandTime = pmove->cmd.serverTime;
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 1904
;1902:
;1903:	// save old org in case we get stuck
;1904:	VectorCopy (pm->ps->origin, pml.previous_origin);
ADDRGP4 pml+112
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1907
;1905:
;1906:	// save old velocity for crashlanding
;1907:	VectorCopy (pm->ps->velocity, pml.previous_velocity);
ADDRGP4 pml+124
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 1909
;1908:
;1909:	pml.frametime = pml.msec * 0.001;
ADDRGP4 pml+36
CNSTF4 981668463
ADDRGP4 pml+40
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1912
;1910:
;1911:	// update the viewangles
;1912:	PM_UpdateViewAngles( pm->ps, &pm->cmd );
ADDRLP4 32
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRGP4 PM_UpdateViewAngles
CALLV
pop
line 1914
;1913:
;1914:	AngleVectors (pm->ps->viewangles, pml.forward, pml.right, pml.up);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 152
ADDP4
ARGP4
ADDRGP4 pml
ARGP4
ADDRGP4 pml+12
ARGP4
ADDRGP4 pml+24
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1916
;1915:
;1916:	if ( pm->cmd.upmove < 10 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
INDIRI1
CVII4 1
CNSTI4 10
GEI4 $693
line 1918
;1917:		// not holding jump
;1918:		pm->ps->pm_flags &= ~PMF_JUMP_HELD;
ADDRLP4 36
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 1919
;1919:	}
LABELV $693
line 1922
;1920:
;1921:	// decide if backpedaling animations should be used
;1922:	if ( pm->cmd.forwardmove < 0 ) {
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
GEI4 $695
line 1923
;1923:		pm->ps->pm_flags |= PMF_BACKWARDS_RUN;
ADDRLP4 36
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1924
;1924:	} else if ( pm->cmd.forwardmove > 0 || ( pm->cmd.forwardmove == 0 && pm->cmd.rightmove ) ) {
ADDRGP4 $696
JUMPV
LABELV $695
ADDRLP4 36
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 40
ADDRLP4 36
INDIRP4
CNSTI4 25
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRI4
ADDRLP4 44
INDIRI4
GTI4 $699
ADDRLP4 40
INDIRI4
ADDRLP4 44
INDIRI4
NEI4 $697
ADDRLP4 36
INDIRP4
CNSTI4 26
ADDP4
INDIRI1
CVII4 1
ADDRLP4 44
INDIRI4
EQI4 $697
LABELV $699
line 1925
;1925:		pm->ps->pm_flags &= ~PMF_BACKWARDS_RUN;
ADDRLP4 48
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 -17
BANDI4
ASGNI4
line 1926
;1926:	}
LABELV $697
LABELV $696
line 1928
;1927:
;1928:	if ( pm->ps->pm_type >= PM_DEAD ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
LTI4 $700
line 1929
;1929:		pm->cmd.forwardmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 25
ADDP4
CNSTI1 0
ASGNI1
line 1930
;1930:		pm->cmd.rightmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 26
ADDP4
CNSTI1 0
ASGNI1
line 1931
;1931:		pm->cmd.upmove = 0;
ADDRGP4 pm
INDIRP4
CNSTI4 27
ADDP4
CNSTI1 0
ASGNI1
line 1932
;1932:	}
LABELV $700
line 1934
;1933:
;1934:	if ( pm->ps->pm_type == PM_SPECTATOR ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
NEI4 $702
line 1935
;1935:		PM_CheckDuck ();
ADDRGP4 PM_CheckDuck
CALLV
pop
line 1936
;1936:		PM_FlyMove ();
ADDRGP4 PM_FlyMove
CALLV
pop
line 1937
;1937:		PM_DropTimers ();
ADDRGP4 PM_DropTimers
CALLV
pop
line 1938
;1938:		return;
ADDRGP4 $664
JUMPV
LABELV $702
line 1941
;1939:	}
;1940:
;1941:	if ( pm->ps->pm_type == PM_NOCLIP ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 1
NEI4 $704
line 1942
;1942:		PM_NoclipMove ();
ADDRGP4 PM_NoclipMove
CALLV
pop
line 1943
;1943:		PM_DropTimers ();
ADDRGP4 PM_DropTimers
CALLV
pop
line 1944
;1944:		return;
ADDRGP4 $664
JUMPV
LABELV $704
line 1947
;1945:	}
;1946:
;1947:	if (pm->ps->pm_type == PM_FREEZE) {
ADDRLP4 48
CNSTI4 4
ASGNI4
ADDRGP4 pm
INDIRP4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRI4
ADDRLP4 48
INDIRI4
NEI4 $706
line 1948
;1948:		return;		// no movement at all
ADDRGP4 $664
JUMPV
LABELV $706
line 1951
;1949:	}
;1950:
;1951:	if ( pm->ps->pm_type == PM_INTERMISSION || pm->ps->pm_type == PM_SPINTERMISSION) {
ADDRLP4 52
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 5
EQI4 $710
ADDRLP4 52
INDIRI4
CNSTI4 6
NEI4 $708
LABELV $710
line 1952
;1952:		return;		// no movement at all
ADDRGP4 $664
JUMPV
LABELV $708
line 1956
;1953:	}
;1954:
;1955:	// set watertype, and waterlevel
;1956:	PM_SetWaterLevel();
ADDRGP4 PM_SetWaterLevel
CALLV
pop
line 1957
;1957:	pml.previous_waterlevel = pmove->waterlevel;
ADDRGP4 pml+136
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ASGNI4
line 1960
;1958:
;1959:	// set mins, maxs, and viewheight
;1960:	PM_CheckDuck ();
ADDRGP4 PM_CheckDuck
CALLV
pop
line 1963
;1961:
;1962:	// set groundentity
;1963:	PM_GroundTrace();
ADDRGP4 PM_GroundTrace
CALLV
pop
line 1965
;1964:
;1965:	if ( pm->ps->pm_type == PM_DEAD ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $712
line 1966
;1966:		PM_DeadMove ();
ADDRGP4 PM_DeadMove
CALLV
pop
line 1967
;1967:	}
LABELV $712
line 1969
;1968:
;1969:	PM_DropTimers();
ADDRGP4 PM_DropTimers
CALLV
pop
line 1976
;1970:
;1971:#ifdef MISSIONPACK
;1972:	if ( pm->ps->powerups[PW_INVULNERABILITY] ) {
;1973:		PM_InvulnerabilityMove();
;1974:	} else
;1975:#endif
;1976:	if ( pm->ps->powerups[PW_FLIGHT] ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
CNSTI4 0
EQI4 $714
line 1978
;1977:		// flight powerup doesn't allow jump and has different friction
;1978:		PM_FlyMove();
ADDRGP4 PM_FlyMove
CALLV
pop
line 1979
;1979:	} else if (pm->ps->pm_flags & PMF_GRAPPLE_PULL) {
ADDRGP4 $715
JUMPV
LABELV $714
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $716
line 1980
;1980:		PM_GrappleMove();
ADDRGP4 PM_GrappleMove
CALLV
pop
line 1982
;1981:		// We can wiggle a bit
;1982:		PM_AirMove();
ADDRGP4 PM_AirMove
CALLV
pop
line 1983
;1983:	} else if (pm->ps->pm_flags & PMF_TIME_WATERJUMP) {
ADDRGP4 $717
JUMPV
LABELV $716
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $718
line 1984
;1984:		PM_WaterJumpMove();
ADDRGP4 PM_WaterJumpMove
CALLV
pop
line 1985
;1985:	} else if ( pm->waterlevel > 1 ) {
ADDRGP4 $719
JUMPV
LABELV $718
ADDRGP4 pm
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1
LEI4 $720
line 1987
;1986:		// swimming
;1987:		PM_WaterMove();
ADDRGP4 PM_WaterMove
CALLV
pop
line 1988
;1988:	} else if ( pml.walking ) {
ADDRGP4 $721
JUMPV
LABELV $720
ADDRGP4 pml+44
INDIRI4
CNSTI4 0
EQI4 $722
line 1990
;1989:		// walking on ground
;1990:		PM_WalkMove();
ADDRGP4 PM_WalkMove
CALLV
pop
line 1991
;1991:	} else {
ADDRGP4 $723
JUMPV
LABELV $722
line 1993
;1992:		// airborne
;1993:		PM_AirMove();
ADDRGP4 PM_AirMove
CALLV
pop
line 1994
;1994:	}
LABELV $723
LABELV $721
LABELV $719
LABELV $717
LABELV $715
line 1996
;1995:
;1996:	PM_Animate();
ADDRGP4 PM_Animate
CALLV
pop
line 1999
;1997:
;1998:	// set groundentity, watertype, and waterlevel
;1999:	PM_GroundTrace();
ADDRGP4 PM_GroundTrace
CALLV
pop
line 2000
;2000:	PM_SetWaterLevel();
ADDRGP4 PM_SetWaterLevel
CALLV
pop
line 2003
;2001:
;2002:	// weapons
;2003:	PM_Weapon();
ADDRGP4 PM_Weapon
CALLV
pop
line 2006
;2004:
;2005:	// torso animation
;2006:	PM_TorsoAnimation();
ADDRGP4 PM_TorsoAnimation
CALLV
pop
line 2009
;2007:
;2008:	// footstep events / legs animations
;2009:	PM_Footsteps();
ADDRGP4 PM_Footsteps
CALLV
pop
line 2012
;2010:
;2011:	// entering / leaving water splashes
;2012:	PM_WaterEvents();
ADDRGP4 PM_WaterEvents
CALLV
pop
line 2015
;2013:
;2014:	// snap some parts of playerstate to save network bandwidth
;2015:	trap_SnapVector( pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRGP4 trap_SnapVector
CALLV
pop
line 2016
;2016:}
LABELV $664
endproc PmoveSingle 56 16
export Pmove
proc Pmove 16 4
line 2026
;2017:
;2018:
;2019:/*
;2020:================
;2021:Pmove
;2022:
;2023:Can be called by either the server or the client
;2024:================
;2025:*/
;2026:void Pmove (pmove_t *pmove) {
line 2029
;2027:	int			finalTime;
;2028:
;2029:	finalTime = pmove->cmd.serverTime;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 2031
;2030:
;2031:	if ( finalTime < pmove->ps->commandTime ) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
INDIRP4
INDIRI4
GEI4 $726
line 2032
;2032:		return;	// should not happen
ADDRGP4 $725
JUMPV
LABELV $726
line 2035
;2033:	}
;2034:
;2035:	if ( finalTime > pmove->ps->commandTime + 1000 ) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
INDIRP4
INDIRI4
CNSTI4 1000
ADDI4
LEI4 $728
line 2036
;2036:		pmove->ps->commandTime = finalTime - 1000;
ADDRFP4 0
INDIRP4
INDIRP4
ADDRLP4 0
INDIRI4
CNSTI4 1000
SUBI4
ASGNI4
line 2037
;2037:	}
LABELV $728
line 2039
;2038:
;2039:	pmove->ps->pmove_framecount = (pmove->ps->pmove_framecount+1) & ((1<<PS_PMOVEFRAMECOUNTBITS)-1);
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRP4
CNSTI4 456
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 63
BANDI4
ASGNI4
ADDRGP4 $731
JUMPV
LABELV $730
line 2043
;2040:
;2041:	// chop the move up if it is too long, to prevent framerate
;2042:	// dependent behavior
;2043:	while ( pmove->ps->commandTime != finalTime ) {
line 2046
;2044:		int		msec;
;2045:
;2046:		msec = finalTime - pmove->ps->commandTime;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
INDIRP4
INDIRI4
SUBI4
ASGNI4
line 2048
;2047:
;2048:		if ( pmove->pmove_fixed ) {
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
EQI4 $733
line 2049
;2049:			if ( msec > pmove->pmove_msec ) {
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
LEI4 $734
line 2050
;2050:				msec = pmove->pmove_msec;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ASGNI4
line 2051
;2051:			}
line 2052
;2052:		}
ADDRGP4 $734
JUMPV
LABELV $733
line 2053
;2053:		else {
line 2054
;2054:			if ( msec > 66 ) {
ADDRLP4 8
INDIRI4
CNSTI4 66
LEI4 $737
line 2055
;2055:				msec = 66;
ADDRLP4 8
CNSTI4 66
ASGNI4
line 2056
;2056:			}
LABELV $737
line 2057
;2057:		}
LABELV $734
line 2058
;2058:		pmove->cmd.serverTime = pmove->ps->commandTime + msec;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 12
INDIRP4
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 2059
;2059:		PmoveSingle( pmove );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 PmoveSingle
CALLV
pop
line 2061
;2060:
;2061:		if ( pmove->ps->pm_flags & PMF_JUMP_HELD ) {
ADDRFP4 0
INDIRP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $739
line 2062
;2062:			pmove->cmd.upmove = 20;
ADDRFP4 0
INDIRP4
CNSTI4 27
ADDP4
CNSTI1 20
ASGNI1
line 2063
;2063:		}
LABELV $739
line 2064
;2064:	}
LABELV $731
line 2043
ADDRFP4 0
INDIRP4
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $730
line 2068
;2065:
;2066:	//PM_CheckStuck();
;2067:
;2068:}
LABELV $725
endproc Pmove 16 4
import trap_SnapVector
import PM_StepSlideMove
import PM_SlideMove
bss
export pml
align 4
LABELV pml
skip 140
export pm
align 4
LABELV pm
skip 4
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
LABELV $467
byte 1 37
byte 1 105
byte 1 58
byte 1 76
byte 1 97
byte 1 110
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $456
byte 1 37
byte 1 105
byte 1 58
byte 1 115
byte 1 116
byte 1 101
byte 1 101
byte 1 112
byte 1 10
byte 1 0
align 1
LABELV $445
byte 1 37
byte 1 105
byte 1 58
byte 1 107
byte 1 105
byte 1 99
byte 1 107
byte 1 111
byte 1 102
byte 1 102
byte 1 10
byte 1 0
align 1
LABELV $414
byte 1 37
byte 1 105
byte 1 58
byte 1 108
byte 1 105
byte 1 102
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $387
byte 1 37
byte 1 105
byte 1 58
byte 1 97
byte 1 108
byte 1 108
byte 1 115
byte 1 111
byte 1 108
byte 1 105
byte 1 100
byte 1 10
byte 1 0
