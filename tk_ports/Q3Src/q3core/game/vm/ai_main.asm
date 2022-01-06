export BotAI_Print
code
proc BotAI_Print 2056 12
file "../ai_main.c"
line 95
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
;24:/*****************************************************************************
;25: * name:		ai_main.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_main.c $
;30: *
;31: *****************************************************************************/
;32:
;33:
;34:#include "g_local.h"
;35:#include "q_shared.h"
;36:#include "botlib.h"		//bot lib interface
;37:#include "be_aas.h"
;38:#include "be_ea.h"
;39:#include "be_ai_char.h"
;40:#include "be_ai_chat.h"
;41:#include "be_ai_gen.h"
;42:#include "be_ai_goal.h"
;43:#include "be_ai_move.h"
;44:#include "be_ai_weap.h"
;45://
;46:#include "ai_main.h"
;47:#include "ai_dmq3.h"
;48:#include "ai_chat.h"
;49:#include "ai_cmd.h"
;50:#include "ai_dmnet.h"
;51:#include "ai_vcmd.h"
;52:
;53://
;54:#include "chars.h"
;55:#include "inv.h"
;56:#include "syn.h"
;57:
;58:#define MAX_PATH		144
;59:
;60:
;61://bot states
;62:bot_state_t	*botstates[MAX_CLIENTS];
;63://number of bots
;64:int numbots;
;65://floating point time
;66:float floattime;
;67://time to do a regular update
;68:float regularupdate_time;
;69://
;70:int bot_interbreed;
;71:int bot_interbreedmatchcount;
;72://
;73:vmCvar_t bot_thinktime;
;74:vmCvar_t bot_memorydump;
;75:vmCvar_t bot_saveroutingcache;
;76:vmCvar_t bot_pause;
;77:vmCvar_t bot_report;
;78:vmCvar_t bot_testsolid;
;79:vmCvar_t bot_testclusters;
;80:vmCvar_t bot_developer;
;81:vmCvar_t bot_interbreedchar;
;82:vmCvar_t bot_interbreedbots;
;83:vmCvar_t bot_interbreedcycle;
;84:vmCvar_t bot_interbreedwrite;
;85:
;86:
;87:void ExitLevel( void );
;88:
;89:
;90:/*
;91:==================
;92:BotAI_Print
;93:==================
;94:*/
;95:void QDECL BotAI_Print(int type, char *fmt, ...) {
line 99
;96:	char str[2048];
;97:	va_list ap;
;98:
;99:	va_start(ap, fmt);
ADDRLP4 0
ADDRFP4 4+4
ASGNP4
line 100
;100:	vsprintf(str, fmt, ap);
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 101
;101:	va_end(ap);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 103
;102:
;103:	switch(type) {
ADDRLP4 2052
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 2052
INDIRI4
CNSTI4 1
LTI4 $55
ADDRLP4 2052
INDIRI4
CNSTI4 5
GTI4 $55
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $68-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $68
address $57
address $59
address $61
address $63
address $65
code
LABELV $57
line 104
;104:		case PRT_MESSAGE: {
line 105
;105:			G_Printf("%s", str);
ADDRGP4 $58
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 106
;106:			break;
ADDRGP4 $56
JUMPV
LABELV $59
line 108
;107:		}
;108:		case PRT_WARNING: {
line 109
;109:			G_Printf( S_COLOR_YELLOW "Warning: %s", str );
ADDRGP4 $60
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 110
;110:			break;
ADDRGP4 $56
JUMPV
LABELV $61
line 112
;111:		}
;112:		case PRT_ERROR: {
line 113
;113:			G_Printf( S_COLOR_RED "Error: %s", str );
ADDRGP4 $62
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 114
;114:			break;
ADDRGP4 $56
JUMPV
LABELV $63
line 116
;115:		}
;116:		case PRT_FATAL: {
line 117
;117:			G_Printf( S_COLOR_RED "Fatal: %s", str );
ADDRGP4 $64
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 118
;118:			break;
ADDRGP4 $56
JUMPV
LABELV $65
line 120
;119:		}
;120:		case PRT_EXIT: {
line 121
;121:			G_Error( S_COLOR_RED "Exit: %s", str );
ADDRGP4 $66
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 122
;122:			break;
ADDRGP4 $56
JUMPV
LABELV $55
line 124
;123:		}
;124:		default: {
line 125
;125:			G_Printf( "unknown print type\n" );
ADDRGP4 $67
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 126
;126:			break;
LABELV $56
line 129
;127:		}
;128:	}
;129:}
LABELV $53
endproc BotAI_Print 2056 12
export BotAI_Trace
proc BotAI_Trace 56 28
line 137
;130:
;131:
;132:/*
;133:==================
;134:BotAI_Trace
;135:==================
;136:*/
;137:void BotAI_Trace(bsp_trace_t *bsptrace, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask) {
line 140
;138:	trace_t trace;
;139:
;140:	trap_Trace(&trace, start, mins, maxs, end, passent, contentmask);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 142
;141:	//copy the trace information
;142:	bsptrace->allsolid = trace.allsolid;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ASGNI4
line 143
;143:	bsptrace->startsolid = trace.startsolid;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0+4
INDIRI4
ASGNI4
line 144
;144:	bsptrace->fraction = trace.fraction;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0+8
INDIRF4
ASGNF4
line 145
;145:	VectorCopy(trace.endpos, bsptrace->endpos);
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 146
;146:	bsptrace->plane.dist = trace.plane.dist;
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 0+24+12
INDIRF4
ASGNF4
line 147
;147:	VectorCopy(trace.plane.normal, bsptrace->plane.normal);
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 0+24
INDIRB
ASGNB 12
line 148
;148:	bsptrace->plane.signbits = trace.plane.signbits;
ADDRFP4 0
INDIRP4
CNSTI4 41
ADDP4
ADDRLP4 0+24+17
INDIRU1
ASGNU1
line 149
;149:	bsptrace->plane.type = trace.plane.type;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 0+24+16
INDIRU1
ASGNU1
line 150
;150:	bsptrace->surface.value = trace.surfaceFlags;
ADDRFP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 0+44
INDIRI4
ASGNI4
line 151
;151:	bsptrace->ent = trace.entityNum;
ADDRFP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 0+52
INDIRI4
ASGNI4
line 152
;152:	bsptrace->exp_dist = 0;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 0
ASGNF4
line 153
;153:	bsptrace->sidenum = 0;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 154
;154:	bsptrace->contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 76
ADDP4
CNSTI4 0
ASGNI4
line 155
;155:}
LABELV $70
endproc BotAI_Trace 56 28
export BotAI_GetClientState
proc BotAI_GetClientState 4 12
line 162
;156:
;157:/*
;158:==================
;159:BotAI_GetClientState
;160:==================
;161:*/
;162:int BotAI_GetClientState( int clientNum, playerState_t *state ) {
line 165
;163:	gentity_t	*ent;
;164:
;165:	ent = &g_entities[clientNum];
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 166
;166:	if ( !ent->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $84
line 167
;167:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $83
JUMPV
LABELV $84
line 169
;168:	}
;169:	if ( !ent->client ) {
ADDRLP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $86
line 170
;170:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $83
JUMPV
LABELV $86
line 173
;171:	}
;172:
;173:	memcpy( state, &ent->client->ps, sizeof(playerState_t) );
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
CNSTI4 468
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 174
;174:	return qtrue;
CNSTI4 1
RETI4
LABELV $83
endproc BotAI_GetClientState 4 12
export BotAI_GetEntityState
proc BotAI_GetEntityState 4 12
line 182
;175:}
;176:
;177:/*
;178:==================
;179:BotAI_GetEntityState
;180:==================
;181:*/
;182:int BotAI_GetEntityState( int entityNum, entityState_t *state ) {
line 185
;183:	gentity_t	*ent;
;184:
;185:	ent = &g_entities[entityNum];
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 186
;186:	memset( state, 0, sizeof(entityState_t) );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 208
ARGI4
ADDRGP4 memset
CALLP4
pop
line 187
;187:	if (!ent->inuse) return qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $89
CNSTI4 0
RETI4
ADDRGP4 $88
JUMPV
LABELV $89
line 188
;188:	if (!ent->r.linked) return qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
CNSTI4 0
NEI4 $91
CNSTI4 0
RETI4
ADDRGP4 $88
JUMPV
LABELV $91
line 189
;189:	if (ent->r.svFlags & SVF_NOCLIENT) return qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $93
CNSTI4 0
RETI4
ADDRGP4 $88
JUMPV
LABELV $93
line 190
;190:	memcpy( state, &ent->s, sizeof(entityState_t) );
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 208
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 191
;191:	return qtrue;
CNSTI4 1
RETI4
LABELV $88
endproc BotAI_GetEntityState 4 12
export BotAI_GetSnapshotEntity
proc BotAI_GetSnapshotEntity 8 12
line 199
;192:}
;193:
;194:/*
;195:==================
;196:BotAI_GetSnapshotEntity
;197:==================
;198:*/
;199:int BotAI_GetSnapshotEntity( int clientNum, int sequence, entityState_t *state ) {
line 202
;200:	int		entNum;
;201:
;202:	entNum = trap_BotGetSnapshotEntity( clientNum, sequence );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 trap_BotGetSnapshotEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 203
;203:	if ( entNum == -1 ) {
ADDRLP4 0
INDIRI4
CNSTI4 -1
NEI4 $96
line 204
;204:		memset(state, 0, sizeof(entityState_t));
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 208
ARGI4
ADDRGP4 memset
CALLP4
pop
line 205
;205:		return -1;
CNSTI4 -1
RETI4
ADDRGP4 $95
JUMPV
LABELV $96
line 208
;206:	}
;207:
;208:	BotAI_GetEntityState( entNum, state );
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 BotAI_GetEntityState
CALLI4
pop
line 210
;209:
;210:	return sequence + 1;
ADDRFP4 4
INDIRI4
CNSTI4 1
ADDI4
RETI4
LABELV $95
endproc BotAI_GetSnapshotEntity 8 12
export BotAI_BotInitialChat
proc BotAI_BotInitialChat 56 44
line 218
;211:}
;212:
;213:/*
;214:==================
;215:BotAI_BotInitialChat
;216:==================
;217:*/
;218:void QDECL BotAI_BotInitialChat( bot_state_t *bs, char *type, ... ) {
line 224
;219:	int		i, mcontext;
;220:	va_list	ap;
;221:	char	*p;
;222:	char	*vars[MAX_MATCHVARIABLES];
;223:
;224:	memset(vars, 0, sizeof(vars));
ADDRLP4 8
ARGP4
CNSTI4 0
ARGI4
CNSTI4 32
ARGI4
ADDRGP4 memset
CALLP4
pop
line 225
;225:	va_start(ap, type);
ADDRLP4 40
ADDRFP4 4+4
ASGNP4
line 226
;226:	p = va_arg(ap, char *);
ADDRLP4 48
ADDRLP4 40
INDIRP4
CNSTU4 4
ADDP4
ASGNP4
ADDRLP4 40
ADDRLP4 48
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 48
INDIRP4
CNSTI4 -4
ADDP4
INDIRP4
ASGNP4
line 227
;227:	for (i = 0; i < MAX_MATCHVARIABLES; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $100
line 228
;228:		if( !p ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $104
line 229
;229:			break;
ADDRGP4 $102
JUMPV
LABELV $104
line 231
;230:		}
;231:		vars[i] = p;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 232
;232:		p = va_arg(ap, char *);
ADDRLP4 52
ADDRLP4 40
INDIRP4
CNSTU4 4
ADDP4
ASGNP4
ADDRLP4 40
ADDRLP4 52
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 52
INDIRP4
CNSTI4 -4
ADDP4
INDIRP4
ASGNP4
line 233
;233:	}
LABELV $101
line 227
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 8
LTI4 $100
LABELV $102
line 234
;234:	va_end(ap);
ADDRLP4 40
CNSTP4 0
ASGNP4
line 236
;235:
;236:	mcontext = BotSynonymContext(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotSynonymContext
CALLI4
ASGNI4
ADDRLP4 44
ADDRLP4 52
INDIRI4
ASGNI4
line 238
;237:
;238:	trap_BotInitialChat( bs->cs, type, mcontext, vars[0], vars[1], vars[2], vars[3], vars[4], vars[5], vars[6], vars[7] );
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 44
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8+4
INDIRP4
ARGP4
ADDRLP4 8+8
INDIRP4
ARGP4
ADDRLP4 8+12
INDIRP4
ARGP4
ADDRLP4 8+16
INDIRP4
ARGP4
ADDRLP4 8+20
INDIRP4
ARGP4
ADDRLP4 8+24
INDIRP4
ARGP4
ADDRLP4 8+28
INDIRP4
ARGP4
ADDRGP4 trap_BotInitialChat
CALLV
pop
line 239
;239:}
LABELV $98
endproc BotAI_BotInitialChat 56 44
export BotTestAAS
proc BotTestAAS 64 16
line 247
;240:
;241:
;242:/*
;243:==================
;244:BotTestAAS
;245:==================
;246:*/
;247:void BotTestAAS(vec3_t origin) {
line 251
;248:	int areanum;
;249:	aas_areainfo_t info;
;250:
;251:	trap_Cvar_Update(&bot_testsolid);
ADDRGP4 bot_testsolid
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 252
;252:	trap_Cvar_Update(&bot_testclusters);
ADDRGP4 bot_testclusters
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 253
;253:	if (bot_testsolid.integer) {
ADDRGP4 bot_testsolid+12
INDIRI4
CNSTI4 0
EQI4 $114
line 254
;254:		if (!trap_AAS_Initialized()) return;
ADDRLP4 56
ADDRGP4 trap_AAS_Initialized
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $117
ADDRGP4 $113
JUMPV
LABELV $117
line 255
;255:		areanum = BotPointAreaNum(origin);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 60
INDIRI4
ASGNI4
line 256
;256:		if (areanum) BotAI_Print(PRT_MESSAGE, "\remtpy area");
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $119
CNSTI4 1
ARGI4
ADDRGP4 $121
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
ADDRGP4 $115
JUMPV
LABELV $119
line 257
;257:		else BotAI_Print(PRT_MESSAGE, "\r^1SOLID area");
CNSTI4 1
ARGI4
ADDRGP4 $122
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 258
;258:	}
ADDRGP4 $115
JUMPV
LABELV $114
line 259
;259:	else if (bot_testclusters.integer) {
ADDRGP4 bot_testclusters+12
INDIRI4
CNSTI4 0
EQI4 $123
line 260
;260:		if (!trap_AAS_Initialized()) return;
ADDRLP4 56
ADDRGP4 trap_AAS_Initialized
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $126
ADDRGP4 $113
JUMPV
LABELV $126
line 261
;261:		areanum = BotPointAreaNum(origin);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 60
INDIRI4
ASGNI4
line 262
;262:		if (!areanum)
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $128
line 263
;263:			BotAI_Print(PRT_MESSAGE, "\r^1Solid!                              ");
CNSTI4 1
ARGI4
ADDRGP4 $130
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
ADDRGP4 $129
JUMPV
LABELV $128
line 264
;264:		else {
line 265
;265:			trap_AAS_AreaInfo(areanum, &info);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_AAS_AreaInfo
CALLI4
pop
line 266
;266:			BotAI_Print(PRT_MESSAGE, "\rarea %d, cluster %d       ", areanum, info.cluster);
CNSTI4 1
ARGI4
ADDRGP4 $131
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4+12
INDIRI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 267
;267:		}
LABELV $129
line 268
;268:	}
LABELV $123
LABELV $115
line 269
;269:}
LABELV $113
endproc BotTestAAS 64 16
export BotReportStatus
proc BotReportStatus 560 24
line 276
;270:
;271:/*
;272:==================
;273:BotReportStatus
;274:==================
;275:*/
;276:void BotReportStatus(bot_state_t *bs) {
line 281
;277:	char goalname[MAX_MESSAGE_SIZE];
;278:	char netname[MAX_MESSAGE_SIZE];
;279:	char *leader, flagstatus[32];
;280:	//
;281:	ClientName(bs->client, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 282
;282:	if (Q_stricmp(netname, bs->teamleader) == 0) leader = "L";
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 548
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 548
INDIRI4
CNSTI4 0
NEI4 $134
ADDRLP4 288
ADDRGP4 $136
ASGNP4
ADDRGP4 $135
JUMPV
LABELV $134
line 283
;283:	else leader = " ";
ADDRLP4 288
ADDRGP4 $137
ASGNP4
LABELV $135
line 285
;284:
;285:	strcpy(flagstatus, "  ");
ADDRLP4 256
ARGP4
ADDRGP4 $138
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 286
;286:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $139
line 287
;287:		if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 552
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 552
INDIRI4
CNSTI4 0
EQI4 $141
line 288
;288:			if (BotTeam(bs) == TEAM_RED) strcpy(flagstatus, S_COLOR_RED"F ");
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 556
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 556
INDIRI4
CNSTI4 1
NEI4 $143
ADDRLP4 256
ARGP4
ADDRGP4 $145
ARGP4
ADDRGP4 strcpy
CALLP4
pop
ADDRGP4 $144
JUMPV
LABELV $143
line 289
;289:			else strcpy(flagstatus, S_COLOR_BLUE"F ");
ADDRLP4 256
ARGP4
ADDRGP4 $146
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $144
line 290
;290:		}
LABELV $141
line 291
;291:	}
LABELV $139
line 307
;292:#ifdef MISSIONPACK
;293:	else if (gametype == GT_1FCTF) {
;294:		if (Bot1FCTFCarryingFlag(bs)) {
;295:			if (BotTeam(bs) == TEAM_RED) strcpy(flagstatus, S_COLOR_RED"F ");
;296:			else strcpy(flagstatus, S_COLOR_BLUE"F ");
;297:		}
;298:	}
;299:	else if (gametype == GT_HARVESTER) {
;300:		if (BotHarvesterCarryingCubes(bs)) {
;301:			if (BotTeam(bs) == TEAM_RED) Com_sprintf(flagstatus, sizeof(flagstatus), S_COLOR_RED"%2d", bs->inventory[INVENTORY_REDCUBE]);
;302:			else Com_sprintf(flagstatus, sizeof(flagstatus), S_COLOR_BLUE"%2d", bs->inventory[INVENTORY_BLUECUBE]);
;303:		}
;304:	}
;305:#endif
;306:
;307:	switch(bs->ltgtype) {
ADDRLP4 552
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 552
INDIRI4
CNSTI4 1
LTI4 $147
ADDRLP4 552
INDIRI4
CNSTI4 13
GTI4 $147
ADDRLP4 552
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $175-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $175
address $150
address $152
address $154
address $164
address $166
address $168
address $160
address $160
address $162
address $156
address $158
address $172
address $170
code
LABELV $150
line 309
;308:		case LTG_TEAMHELP:
;309:		{
line 310
;310:			EasyClientName(bs->teammate, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 311
;311:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: helping %s\n", netname, leader, flagstatus, goalname);
CNSTI4 1
ARGI4
ADDRGP4 $151
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 292
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 312
;312:			break;
ADDRGP4 $148
JUMPV
LABELV $152
line 315
;313:		}
;314:		case LTG_TEAMACCOMPANY:
;315:		{
line 316
;316:			EasyClientName(bs->teammate, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 317
;317:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: accompanying %s\n", netname, leader, flagstatus, goalname);
CNSTI4 1
ARGI4
ADDRGP4 $153
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 292
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 318
;318:			break;
ADDRGP4 $148
JUMPV
LABELV $154
line 321
;319:		}
;320:		case LTG_DEFENDKEYAREA:
;321:		{
line 322
;322:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 323
;323:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: defending %s\n", netname, leader, flagstatus, goalname);
CNSTI4 1
ARGI4
ADDRGP4 $155
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 292
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 324
;324:			break;
ADDRGP4 $148
JUMPV
LABELV $156
line 327
;325:		}
;326:		case LTG_GETITEM:
;327:		{
line 328
;328:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 329
;329:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: getting item %s\n", netname, leader, flagstatus, goalname);
CNSTI4 1
ARGI4
ADDRGP4 $157
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 292
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 330
;330:			break;
ADDRGP4 $148
JUMPV
LABELV $158
line 333
;331:		}
;332:		case LTG_KILL:
;333:		{
line 334
;334:			ClientName(bs->teamgoal.entitynum, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 335
;335:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: killing %s\n", netname, leader, flagstatus, goalname);
CNSTI4 1
ARGI4
ADDRGP4 $159
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 292
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 336
;336:			break;
ADDRGP4 $148
JUMPV
LABELV $160
line 340
;337:		}
;338:		case LTG_CAMP:
;339:		case LTG_CAMPORDER:
;340:		{
line 341
;341:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: camping\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $161
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 342
;342:			break;
ADDRGP4 $148
JUMPV
LABELV $162
line 345
;343:		}
;344:		case LTG_PATROL:
;345:		{
line 346
;346:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: patrolling\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $163
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 347
;347:			break;
ADDRGP4 $148
JUMPV
LABELV $164
line 350
;348:		}
;349:		case LTG_GETFLAG:
;350:		{
line 351
;351:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: capturing flag\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $165
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 352
;352:			break;
ADDRGP4 $148
JUMPV
LABELV $166
line 355
;353:		}
;354:		case LTG_RUSHBASE:
;355:		{
line 356
;356:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: rushing base\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $167
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 357
;357:			break;
ADDRGP4 $148
JUMPV
LABELV $168
line 360
;358:		}
;359:		case LTG_RETURNFLAG:
;360:		{
line 361
;361:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: returning flag\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $169
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 362
;362:			break;
ADDRGP4 $148
JUMPV
LABELV $170
line 365
;363:		}
;364:		case LTG_ATTACKENEMYBASE:
;365:		{
line 366
;366:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: attacking the enemy base\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $171
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 367
;367:			break;
ADDRGP4 $148
JUMPV
LABELV $172
line 370
;368:		}
;369:		case LTG_HARVEST:
;370:		{
line 371
;371:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: harvesting\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $173
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 372
;372:			break;
ADDRGP4 $148
JUMPV
LABELV $147
line 375
;373:		}
;374:		default:
;375:		{
line 376
;376:			BotAI_Print(PRT_MESSAGE, "%-20s%s%s: roaming\n", netname, leader, flagstatus);
CNSTI4 1
ARGI4
ADDRGP4 $174
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 288
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 377
;377:			break;
LABELV $148
line 380
;378:		}
;379:	}
;380:}
LABELV $133
endproc BotReportStatus 560 24
export BotTeamplayReport
proc BotTeamplayReport 1056 12
line 387
;381:
;382:/*
;383:==================
;384:BotTeamplayReport
;385:==================
;386:*/
;387:void BotTeamplayReport(void) {
line 391
;388:	int i;
;389:	char buf[MAX_INFO_STRING];
;390:
;391:	BotAI_Print(PRT_MESSAGE, S_COLOR_RED"RED\n");
CNSTI4 1
ARGI4
ADDRGP4 $178
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 392
;392:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $182
JUMPV
LABELV $179
line 394
;393:		//
;394:		if ( !botstates[i] || !botstates[i]->inuse ) continue;
ADDRLP4 1028
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $185
ADDRLP4 1028
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $183
LABELV $185
ADDRGP4 $180
JUMPV
LABELV $183
line 396
;395:		//
;396:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 398
;397:		//if no config string or no name
;398:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
EQI4 $189
ADDRLP4 4
ARGP4
ADDRGP4 $188
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $186
LABELV $189
ADDRGP4 $180
JUMPV
LABELV $186
line 400
;399:		//skip spectators
;400:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_RED) {
ADDRLP4 4
ARGP4
ADDRGP4 $192
ARGP4
ADDRLP4 1044
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1044
INDIRP4
ARGP4
ADDRLP4 1048
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 1
NEI4 $190
line 401
;401:			BotReportStatus(botstates[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotReportStatus
CALLV
pop
line 402
;402:		}
LABELV $190
line 403
;403:	}
LABELV $180
line 392
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $182
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $193
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $179
LABELV $193
line 404
;404:	BotAI_Print(PRT_MESSAGE, S_COLOR_BLUE"BLUE\n");
CNSTI4 1
ARGI4
ADDRGP4 $194
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 405
;405:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $198
JUMPV
LABELV $195
line 407
;406:		//
;407:		if ( !botstates[i] || !botstates[i]->inuse ) continue;
ADDRLP4 1032
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1032
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $201
ADDRLP4 1032
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $199
LABELV $201
ADDRGP4 $196
JUMPV
LABELV $199
line 409
;408:		//
;409:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 411
;410:		//if no config string or no name
;411:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $204
ADDRLP4 4
ARGP4
ADDRGP4 $188
ARGP4
ADDRLP4 1040
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1044
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $202
LABELV $204
ADDRGP4 $196
JUMPV
LABELV $202
line 413
;412:		//skip spectators
;413:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_BLUE) {
ADDRLP4 4
ARGP4
ADDRGP4 $192
ARGP4
ADDRLP4 1048
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRLP4 1052
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 2
NEI4 $205
line 414
;414:			BotReportStatus(botstates[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotReportStatus
CALLV
pop
line 415
;415:		}
LABELV $205
line 416
;416:	}
LABELV $196
line 405
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $198
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $207
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $195
LABELV $207
line 417
;417:}
LABELV $177
endproc BotTeamplayReport 1056 12
export BotSetInfoConfigString
proc BotSetInfoConfigString 880 16
line 424
;418:
;419:/*
;420:==================
;421:BotSetInfoConfigString
;422:==================
;423:*/
;424:void BotSetInfoConfigString(bot_state_t *bs) {
line 431
;425:	char goalname[MAX_MESSAGE_SIZE];
;426:	char netname[MAX_MESSAGE_SIZE];
;427:	char action[MAX_MESSAGE_SIZE];
;428:	char *leader, carrying[32], *cs;
;429:	bot_goal_t goal;
;430:	//
;431:	ClientName(bs->client, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 432
;432:	if (Q_stricmp(netname, bs->teamleader) == 0) leader = "L";
ADDRLP4 256
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 864
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 864
INDIRI4
CNSTI4 0
NEI4 $209
ADDRLP4 544
ADDRGP4 $136
ASGNP4
ADDRGP4 $210
JUMPV
LABELV $209
line 433
;433:	else leader = " ";
ADDRLP4 544
ADDRGP4 $137
ASGNP4
LABELV $210
line 435
;434:
;435:	strcpy(carrying, "  ");
ADDRLP4 512
ARGP4
ADDRGP4 $138
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 436
;436:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $211
line 437
;437:		if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 868
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 868
INDIRI4
CNSTI4 0
EQI4 $213
line 438
;438:			strcpy(carrying, "F ");
ADDRLP4 512
ARGP4
ADDRGP4 $215
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 439
;439:		}
LABELV $213
line 440
;440:	}
LABELV $211
line 455
;441:#ifdef MISSIONPACK
;442:	else if (gametype == GT_1FCTF) {
;443:		if (Bot1FCTFCarryingFlag(bs)) {
;444:			strcpy(carrying, "F ");
;445:		}
;446:	}
;447:	else if (gametype == GT_HARVESTER) {
;448:		if (BotHarvesterCarryingCubes(bs)) {
;449:			if (BotTeam(bs) == TEAM_RED) Com_sprintf(carrying, sizeof(carrying), "%2d", bs->inventory[INVENTORY_REDCUBE]);
;450:			else Com_sprintf(carrying, sizeof(carrying), "%2d", bs->inventory[INVENTORY_BLUECUBE]);
;451:		}
;452:	}
;453:#endif
;454:
;455:	switch(bs->ltgtype) {
ADDRLP4 868
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 868
INDIRI4
CNSTI4 1
LTI4 $216
ADDRLP4 868
INDIRI4
CNSTI4 13
GTI4 $216
ADDRLP4 868
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $245-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $245
address $219
address $221
address $223
address $233
address $235
address $237
address $229
address $229
address $231
address $225
address $227
address $241
address $239
code
LABELV $219
line 457
;456:		case LTG_TEAMHELP:
;457:		{
line 458
;458:			EasyClientName(bs->teammate, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 459
;459:			Com_sprintf(action, sizeof(action), "helping %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $220
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 460
;460:			break;
ADDRGP4 $217
JUMPV
LABELV $221
line 463
;461:		}
;462:		case LTG_TEAMACCOMPANY:
;463:		{
line 464
;464:			EasyClientName(bs->teammate, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 465
;465:			Com_sprintf(action, sizeof(action), "accompanying %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $222
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 466
;466:			break;
ADDRGP4 $217
JUMPV
LABELV $223
line 469
;467:		}
;468:		case LTG_DEFENDKEYAREA:
;469:		{
line 470
;470:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 471
;471:			Com_sprintf(action, sizeof(action), "defending %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $224
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 472
;472:			break;
ADDRGP4 $217
JUMPV
LABELV $225
line 475
;473:		}
;474:		case LTG_GETITEM:
;475:		{
line 476
;476:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 477
;477:			Com_sprintf(action, sizeof(action), "getting item %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $226
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 478
;478:			break;
ADDRGP4 $217
JUMPV
LABELV $227
line 481
;479:		}
;480:		case LTG_KILL:
;481:		{
line 482
;482:			ClientName(bs->teamgoal.entitynum, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 483
;483:			Com_sprintf(action, sizeof(action), "killing %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $228
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 484
;484:			break;
ADDRGP4 $217
JUMPV
LABELV $229
line 488
;485:		}
;486:		case LTG_CAMP:
;487:		case LTG_CAMPORDER:
;488:		{
line 489
;489:			Com_sprintf(action, sizeof(action), "camping");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $230
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 490
;490:			break;
ADDRGP4 $217
JUMPV
LABELV $231
line 493
;491:		}
;492:		case LTG_PATROL:
;493:		{
line 494
;494:			Com_sprintf(action, sizeof(action), "patrolling");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $232
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 495
;495:			break;
ADDRGP4 $217
JUMPV
LABELV $233
line 498
;496:		}
;497:		case LTG_GETFLAG:
;498:		{
line 499
;499:			Com_sprintf(action, sizeof(action), "capturing flag");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $234
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 500
;500:			break;
ADDRGP4 $217
JUMPV
LABELV $235
line 503
;501:		}
;502:		case LTG_RUSHBASE:
;503:		{
line 504
;504:			Com_sprintf(action, sizeof(action), "rushing base");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $236
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 505
;505:			break;
ADDRGP4 $217
JUMPV
LABELV $237
line 508
;506:		}
;507:		case LTG_RETURNFLAG:
;508:		{
line 509
;509:			Com_sprintf(action, sizeof(action), "returning flag");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $238
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 510
;510:			break;
ADDRGP4 $217
JUMPV
LABELV $239
line 513
;511:		}
;512:		case LTG_ATTACKENEMYBASE:
;513:		{
line 514
;514:			Com_sprintf(action, sizeof(action), "attacking the enemy base");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $240
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 515
;515:			break;
ADDRGP4 $217
JUMPV
LABELV $241
line 518
;516:		}
;517:		case LTG_HARVEST:
;518:		{
line 519
;519:			Com_sprintf(action, sizeof(action), "harvesting");
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $242
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 520
;520:			break;
ADDRGP4 $217
JUMPV
LABELV $216
line 523
;521:		}
;522:		default:
;523:		{
line 524
;524:			trap_BotGetTopGoal(bs->gs, &goal);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 808
ARGP4
ADDRGP4 trap_BotGetTopGoal
CALLI4
pop
line 525
;525:			trap_BotGoalName(goal.number, goalname, sizeof(goalname));
ADDRLP4 808+44
INDIRI4
ARGI4
ADDRLP4 552
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 526
;526:			Com_sprintf(action, sizeof(action), "roaming %s", goalname);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $244
ARGP4
ADDRLP4 552
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 527
;527:			break;
LABELV $217
line 530
;528:		}
;529:	}
;530:  	cs = va("l\\%s\\c\\%s\\a\\%s",
ADDRGP4 $247
ARGP4
ADDRLP4 544
INDIRP4
ARGP4
ADDRLP4 512
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 876
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 548
ADDRLP4 876
INDIRP4
ASGNP4
line 534
;531:				leader,
;532:				carrying,
;533:				action);
;534:  	trap_SetConfigstring (CS_BOTINFO + bs->client, cs);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 25
ADDI4
ARGI4
ADDRLP4 548
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 535
;535:}
LABELV $208
endproc BotSetInfoConfigString 880 16
export BotUpdateInfoConfigStrings
proc BotUpdateInfoConfigStrings 1044 12
line 542
;536:
;537:/*
;538:==============
;539:BotUpdateInfoConfigStrings
;540:==============
;541:*/
;542:void BotUpdateInfoConfigStrings(void) {
line 546
;543:	int i;
;544:	char buf[MAX_INFO_STRING];
;545:
;546:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $252
JUMPV
LABELV $249
line 548
;547:		//
;548:		if ( !botstates[i] || !botstates[i]->inuse )
ADDRLP4 1028
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $255
ADDRLP4 1028
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $253
LABELV $255
line 549
;549:			continue;
ADDRGP4 $250
JUMPV
LABELV $253
line 551
;550:		//
;551:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 553
;552:		//if no config string or no name
;553:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n")))
ADDRLP4 4
ARGP4
ADDRLP4 1032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
EQI4 $258
ADDRLP4 4
ARGP4
ADDRGP4 $188
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $256
LABELV $258
line 554
;554:			continue;
ADDRGP4 $250
JUMPV
LABELV $256
line 555
;555:		BotSetInfoConfigString(botstates[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotSetInfoConfigString
CALLV
pop
line 556
;556:	}
LABELV $250
line 546
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $252
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $259
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $249
LABELV $259
line 557
;557:}
LABELV $248
endproc BotUpdateInfoConfigStrings 1044 12
export BotInterbreedBots
proc BotInterbreedBots 288 20
line 564
;558:
;559:/*
;560:==============
;561:BotInterbreedBots
;562:==============
;563:*/
;564:void BotInterbreedBots(void) {
line 570
;565:	float ranks[MAX_CLIENTS];
;566:	int parent1, parent2, child;
;567:	int i;
;568:
;569:	// get rankings for all the bots
;570:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $261
line 571
;571:		if ( botstates[i] && botstates[i]->inuse ) {
ADDRLP4 272
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 272
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $265
ADDRLP4 272
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $265
line 572
;572:			ranks[i] = botstates[i]->num_kills * 2 - botstates[i]->num_deaths;
ADDRLP4 276
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 280
ADDRLP4 276
INDIRI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 276
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 280
INDIRP4
CNSTI4 6032
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 280
INDIRP4
CNSTI4 6028
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 573
;573:		}
ADDRGP4 $266
JUMPV
LABELV $265
line 574
;574:		else {
line 575
;575:			ranks[i] = -1;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
CNSTF4 3212836864
ASGNF4
line 576
;576:		}
LABELV $266
line 577
;577:	}
LABELV $262
line 570
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $261
line 579
;578:
;579:	if (trap_GeneticParentsAndChildSelection(MAX_CLIENTS, ranks, &parent1, &parent2, &child)) {
CNSTI4 64
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 264
ARGP4
ADDRLP4 268
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 272
ADDRGP4 trap_GeneticParentsAndChildSelection
CALLI4
ASGNI4
ADDRLP4 272
INDIRI4
CNSTI4 0
EQI4 $267
line 580
;580:		trap_BotInterbreedGoalFuzzyLogic(botstates[parent1]->gs, botstates[parent2]->gs, botstates[child]->gs);
ADDRLP4 276
CNSTI4 2
ASGNI4
ADDRLP4 280
ADDRGP4 botstates
ASGNP4
ADDRLP4 284
CNSTI4 6528
ASGNI4
ADDRLP4 264
INDIRI4
ADDRLP4 276
INDIRI4
LSHI4
ADDRLP4 280
INDIRP4
ADDP4
INDIRP4
ADDRLP4 284
INDIRI4
ADDP4
INDIRI4
ARGI4
ADDRLP4 268
INDIRI4
ADDRLP4 276
INDIRI4
LSHI4
ADDRLP4 280
INDIRP4
ADDP4
INDIRP4
ADDRLP4 284
INDIRI4
ADDP4
INDIRI4
ARGI4
ADDRLP4 260
INDIRI4
ADDRLP4 276
INDIRI4
LSHI4
ADDRLP4 280
INDIRP4
ADDP4
INDIRP4
ADDRLP4 284
INDIRI4
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotInterbreedGoalFuzzyLogic
CALLV
pop
line 581
;581:		trap_BotMutateGoalFuzzyLogic(botstates[child]->gs, 1);
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_BotMutateGoalFuzzyLogic
CALLV
pop
line 582
;582:	}
LABELV $267
line 584
;583:	// reset the kills and deaths
;584:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $269
line 585
;585:		if (botstates[i] && botstates[i]->inuse) {
ADDRLP4 276
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 276
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $273
ADDRLP4 276
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $273
line 586
;586:			botstates[i]->num_kills = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 6032
ADDP4
CNSTI4 0
ASGNI4
line 587
;587:			botstates[i]->num_deaths = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 6028
ADDP4
CNSTI4 0
ASGNI4
line 588
;588:		}
LABELV $273
line 589
;589:	}
LABELV $270
line 584
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $269
line 590
;590:}
LABELV $260
endproc BotInterbreedBots 288 20
export BotWriteInterbreeded
proc BotWriteInterbreeded 24 8
line 597
;591:
;592:/*
;593:==============
;594:BotWriteInterbreeded
;595:==============
;596:*/
;597:void BotWriteInterbreeded(char *filename) {
line 601
;598:	float rank, bestrank;
;599:	int i, bestbot;
;600:
;601:	bestrank = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 602
;602:	bestbot = -1;
ADDRLP4 12
CNSTI4 -1
ASGNI4
line 604
;603:	// get the best bot
;604:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $276
line 605
;605:		if ( botstates[i] && botstates[i]->inuse ) {
ADDRLP4 16
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $280
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $280
line 606
;606:			rank = botstates[i]->num_kills * 2 - botstates[i]->num_deaths;
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
CNSTI4 6032
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 20
INDIRP4
CNSTI4 6028
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 607
;607:		}
ADDRGP4 $281
JUMPV
LABELV $280
line 608
;608:		else {
line 609
;609:			rank = -1;
ADDRLP4 4
CNSTF4 3212836864
ASGNF4
line 610
;610:		}
LABELV $281
line 611
;611:		if (rank > bestrank) {
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $282
line 612
;612:			bestrank = rank;
ADDRLP4 8
ADDRLP4 4
INDIRF4
ASGNF4
line 613
;613:			bestbot = i;
ADDRLP4 12
ADDRLP4 0
INDIRI4
ASGNI4
line 614
;614:		}
LABELV $282
line 615
;615:	}
LABELV $277
line 604
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $276
line 616
;616:	if (bestbot >= 0) {
ADDRLP4 12
INDIRI4
CNSTI4 0
LTI4 $284
line 618
;617:		//write out the new goal fuzzy logic
;618:		trap_BotSaveGoalFuzzyLogic(botstates[bestbot]->gs, filename);
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_BotSaveGoalFuzzyLogic
CALLV
pop
line 619
;619:	}
LABELV $284
line 620
;620:}
LABELV $275
endproc BotWriteInterbreeded 24 8
export BotInterbreedEndMatch
proc BotInterbreedEndMatch 8 8
line 629
;621:
;622:/*
;623:==============
;624:BotInterbreedEndMatch
;625:
;626:add link back into ExitLevel?
;627:==============
;628:*/
;629:void BotInterbreedEndMatch(void) {
line 631
;630:
;631:	if (!bot_interbreed) return;
ADDRGP4 bot_interbreed
INDIRI4
CNSTI4 0
NEI4 $287
ADDRGP4 $286
JUMPV
LABELV $287
line 632
;632:	bot_interbreedmatchcount++;
ADDRLP4 0
ADDRGP4 bot_interbreedmatchcount
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 633
;633:	if (bot_interbreedmatchcount >= bot_interbreedcycle.integer) {
ADDRGP4 bot_interbreedmatchcount
INDIRI4
ADDRGP4 bot_interbreedcycle+12
INDIRI4
LTI4 $289
line 634
;634:		bot_interbreedmatchcount = 0;
ADDRGP4 bot_interbreedmatchcount
CNSTI4 0
ASGNI4
line 636
;635:		//
;636:		trap_Cvar_Update(&bot_interbreedwrite);
ADDRGP4 bot_interbreedwrite
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 637
;637:		if (strlen(bot_interbreedwrite.string)) {
ADDRGP4 bot_interbreedwrite+16
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $292
line 638
;638:			BotWriteInterbreeded(bot_interbreedwrite.string);
ADDRGP4 bot_interbreedwrite+16
ARGP4
ADDRGP4 BotWriteInterbreeded
CALLV
pop
line 639
;639:			trap_Cvar_Set("bot_interbreedwrite", "");
ADDRGP4 $296
ARGP4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 640
;640:		}
LABELV $292
line 641
;641:		BotInterbreedBots();
ADDRGP4 BotInterbreedBots
CALLV
pop
line 642
;642:	}
LABELV $289
line 643
;643:}
LABELV $286
endproc BotInterbreedEndMatch 8 8
export BotInterbreeding
proc BotInterbreeding 16 20
line 650
;644:
;645:/*
;646:==============
;647:BotInterbreeding
;648:==============
;649:*/
;650:void BotInterbreeding(void) {
line 653
;651:	int i;
;652:
;653:	trap_Cvar_Update(&bot_interbreedchar);
ADDRGP4 bot_interbreedchar
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 654
;654:	if (!strlen(bot_interbreedchar.string)) return;
ADDRGP4 bot_interbreedchar+16
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $299
ADDRGP4 $298
JUMPV
LABELV $299
line 656
;655:	//make sure we are in tournament mode
;656:	if (gametype != GT_TOURNAMENT) {
ADDRGP4 gametype
INDIRI4
CNSTI4 1
EQI4 $302
line 657
;657:		trap_Cvar_Set("g_gametype", va("%d", GT_TOURNAMENT));
ADDRGP4 $305
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $304
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 658
;658:		ExitLevel();
ADDRGP4 ExitLevel
CALLV
pop
line 659
;659:		return;
ADDRGP4 $298
JUMPV
LABELV $302
line 662
;660:	}
;661:	//shutdown all the bots
;662:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $306
line 663
;663:		if (botstates[i] && botstates[i]->inuse) {
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $310
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $310
line 664
;664:			BotAIShutdownClient(botstates[i]->client, qfalse);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 BotAIShutdownClient
CALLI4
pop
line 665
;665:		}
LABELV $310
line 666
;666:	}
LABELV $307
line 662
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $306
line 668
;667:	//make sure all item weight configs are reloaded and Not shared
;668:	trap_BotLibVarSet("bot_reloadcharacters", "1");
ADDRGP4 $312
ARGP4
ADDRGP4 $313
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 670
;669:	//add a number of bots using the desired bot character
;670:	for (i = 0; i < bot_interbreedbots.integer; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $317
JUMPV
LABELV $314
line 671
;671:		trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s 4 free %i %s%d\n",
ADDRGP4 $319
ARGP4
ADDRGP4 bot_interbreedchar+16
ARGP4
CNSTI4 50
ADDRLP4 0
INDIRI4
MULI4
ARGI4
ADDRGP4 bot_interbreedchar+16
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 673
;672:						bot_interbreedchar.string, i * 50, bot_interbreedchar.string, i) );
;673:	}
LABELV $315
line 670
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $317
ADDRLP4 0
INDIRI4
ADDRGP4 bot_interbreedbots+12
INDIRI4
LTI4 $314
line 675
;674:	//
;675:	trap_Cvar_Set("bot_interbreedchar", "");
ADDRGP4 $322
ARGP4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 676
;676:	bot_interbreed = qtrue;
ADDRGP4 bot_interbreed
CNSTI4 1
ASGNI4
line 677
;677:}
LABELV $298
endproc BotInterbreeding 16 20
export BotEntityInfo
proc BotEntityInfo 0 8
line 684
;678:
;679:/*
;680:==============
;681:BotEntityInfo
;682:==============
;683:*/
;684:void BotEntityInfo(int entnum, aas_entityinfo_t *info) {
line 685
;685:	trap_AAS_EntityInfo(entnum, info);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_AAS_EntityInfo
CALLV
pop
line 686
;686:}
LABELV $323
endproc BotEntityInfo 0 8
export NumBots
proc NumBots 0 0
line 693
;687:
;688:/*
;689:==============
;690:NumBots
;691:==============
;692:*/
;693:int NumBots(void) {
line 694
;694:	return numbots;
ADDRGP4 numbots
INDIRI4
RETI4
LABELV $324
endproc NumBots 0 0
export BotTeamLeader
proc BotTeamLeader 12 4
line 702
;695:}
;696:
;697:/*
;698:==============
;699:BotTeamLeader
;700:==============
;701:*/
;702:int BotTeamLeader(bot_state_t *bs) {
line 705
;703:	int leader;
;704:
;705:	leader = ClientFromName(bs->teamleader);
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 706
;706:	if (leader < 0) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $326
CNSTI4 0
RETI4
ADDRGP4 $325
JUMPV
LABELV $326
line 707
;707:	if (!botstates[leader] || !botstates[leader]->inuse) return qfalse;
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $330
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $328
LABELV $330
CNSTI4 0
RETI4
ADDRGP4 $325
JUMPV
LABELV $328
line 708
;708:	return qtrue;
CNSTI4 1
RETI4
LABELV $325
endproc BotTeamLeader 12 4
export AngleDifference
proc AngleDifference 4 0
line 716
;709:}
;710:
;711:/*
;712:==============
;713:AngleDifference
;714:==============
;715:*/
;716:float AngleDifference(float ang1, float ang2) {
line 719
;717:	float diff;
;718:
;719:	diff = ang1 - ang2;
ADDRLP4 0
ADDRFP4 0
INDIRF4
ADDRFP4 4
INDIRF4
SUBF4
ASGNF4
line 720
;720:	if (ang1 > ang2) {
ADDRFP4 0
INDIRF4
ADDRFP4 4
INDIRF4
LEF4 $332
line 721
;721:		if (diff > 180.0) diff -= 360.0;
ADDRLP4 0
INDIRF4
CNSTF4 1127481344
LEF4 $333
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
line 722
;722:	}
ADDRGP4 $333
JUMPV
LABELV $332
line 723
;723:	else {
line 724
;724:		if (diff < -180.0) diff += 360.0;
ADDRLP4 0
INDIRF4
CNSTF4 3274964992
GEF4 $336
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
ADDF4
ASGNF4
LABELV $336
line 725
;725:	}
LABELV $333
line 726
;726:	return diff;
ADDRLP4 0
INDIRF4
RETF4
LABELV $331
endproc AngleDifference 4 0
export BotChangeViewAngle
proc BotChangeViewAngle 16 4
line 734
;727:}
;728:
;729:/*
;730:==============
;731:BotChangeViewAngle
;732:==============
;733:*/
;734:float BotChangeViewAngle(float angle, float ideal_angle, float speed) {
line 737
;735:	float move;
;736:
;737:	angle = AngleMod(angle);
ADDRFP4 0
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRFP4 0
ADDRLP4 4
INDIRF4
ASGNF4
line 738
;738:	ideal_angle = AngleMod(ideal_angle);
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 8
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRFP4 4
ADDRLP4 8
INDIRF4
ASGNF4
line 739
;739:	if (angle == ideal_angle) return angle;
ADDRFP4 0
INDIRF4
ADDRFP4 4
INDIRF4
NEF4 $339
ADDRFP4 0
INDIRF4
RETF4
ADDRGP4 $338
JUMPV
LABELV $339
line 740
;740:	move = ideal_angle - angle;
ADDRLP4 0
ADDRFP4 4
INDIRF4
ADDRFP4 0
INDIRF4
SUBF4
ASGNF4
line 741
;741:	if (ideal_angle > angle) {
ADDRFP4 4
INDIRF4
ADDRFP4 0
INDIRF4
LEF4 $341
line 742
;742:		if (move > 180.0) move -= 360.0;
ADDRLP4 0
INDIRF4
CNSTF4 1127481344
LEF4 $342
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
line 743
;743:	}
ADDRGP4 $342
JUMPV
LABELV $341
line 744
;744:	else {
line 745
;745:		if (move < -180.0) move += 360.0;
ADDRLP4 0
INDIRF4
CNSTF4 3274964992
GEF4 $345
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
ADDF4
ASGNF4
LABELV $345
line 746
;746:	}
LABELV $342
line 747
;747:	if (move > 0) {
ADDRLP4 0
INDIRF4
CNSTF4 0
LEF4 $347
line 748
;748:		if (move > speed) move = speed;
ADDRLP4 0
INDIRF4
ADDRFP4 8
INDIRF4
LEF4 $348
ADDRLP4 0
ADDRFP4 8
INDIRF4
ASGNF4
line 749
;749:	}
ADDRGP4 $348
JUMPV
LABELV $347
line 750
;750:	else {
line 751
;751:		if (move < -speed) move = -speed;
ADDRLP4 0
INDIRF4
ADDRFP4 8
INDIRF4
NEGF4
GEF4 $351
ADDRLP4 0
ADDRFP4 8
INDIRF4
NEGF4
ASGNF4
LABELV $351
line 752
;752:	}
LABELV $348
line 753
;753:	return AngleMod(angle + move);
ADDRFP4 0
INDIRF4
ADDRLP4 0
INDIRF4
ADDF4
ARGF4
ADDRLP4 12
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 12
INDIRF4
RETF4
LABELV $338
endproc BotChangeViewAngle 16 4
export BotChangeViewAngles
proc BotChangeViewAngles 80 16
line 761
;754:}
;755:
;756:/*
;757:==============
;758:BotChangeViewAngles
;759:==============
;760:*/
;761:void BotChangeViewAngles(bot_state_t *bs, float thinktime) {
line 765
;762:	float diff, factor, maxchange, anglespeed, disired_speed;
;763:	int i;
;764:
;765:	if (bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
INDIRF4
CNSTF4 1127481344
LEF4 $354
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
LABELV $354
line 767
;766:	//
;767:	if (bs->enemy >= 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
LTI4 $356
line 768
;768:		factor = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_FACTOR, 0.01f, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTF4 1008981770
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 28
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 28
INDIRF4
ASGNF4
line 769
;769:		maxchange = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_MAXCHANGE, 1, 1800);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 5
ARGI4
CNSTF4 1065353216
ARGF4
CNSTF4 1155596288
ARGF4
ADDRLP4 32
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 32
INDIRF4
ASGNF4
line 770
;770:	}
ADDRGP4 $357
JUMPV
LABELV $356
line 771
;771:	else {
line 772
;772:		factor = 0.05f;
ADDRLP4 16
CNSTF4 1028443341
ASGNF4
line 773
;773:		maxchange = 360;
ADDRLP4 8
CNSTF4 1135869952
ASGNF4
line 774
;774:	}
LABELV $357
line 775
;775:	if (maxchange < 240) maxchange = 240;
ADDRLP4 8
INDIRF4
CNSTF4 1131413504
GEF4 $358
ADDRLP4 8
CNSTF4 1131413504
ASGNF4
LABELV $358
line 776
;776:	maxchange *= thinktime;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRFP4 4
INDIRF4
MULF4
ASGNF4
line 777
;777:	for (i = 0; i < 2; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $360
line 779
;778:		//
;779:		if (bot_challenge.integer) {
ADDRGP4 bot_challenge+12
INDIRI4
CNSTI4 0
EQI4 $364
line 781
;780:			//smooth slowdown view model
;781:			diff = abs(AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]));
ADDRLP4 28
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 28
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 6576
ADDP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 AngleDifference
CALLF4
ASGNF4
ADDRLP4 36
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 40
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 40
INDIRI4
CVIF4 4
ASGNF4
line 782
;782:			anglespeed = diff * factor;
ADDRLP4 4
ADDRLP4 12
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ASGNF4
line 783
;783:			if (anglespeed > maxchange) anglespeed = maxchange;
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $367
ADDRLP4 4
ADDRLP4 8
INDIRF4
ASGNF4
LABELV $367
line 784
;784:			bs->viewangles[i] = BotChangeViewAngle(bs->viewangles[i],
ADDRLP4 44
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 6576
ADDP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 56
ADDRGP4 BotChangeViewAngle
CALLF4
ASGNF4
ADDRLP4 52
INDIRP4
ADDRLP4 56
INDIRF4
ASGNF4
line 786
;785:											bs->ideal_viewangles[i], anglespeed);
;786:		}
ADDRGP4 $365
JUMPV
LABELV $364
line 787
;787:		else {
line 789
;788:			//over reaction view model
;789:			bs->viewangles[i] = AngleMod(bs->viewangles[i]);
ADDRLP4 28
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
INDIRF4
ARGF4
ADDRLP4 32
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRF4
ASGNF4
line 790
;790:			bs->ideal_viewangles[i] = AngleMod(bs->ideal_viewangles[i]);
ADDRLP4 36
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 36
INDIRP4
ADDRLP4 40
INDIRF4
ASGNF4
line 791
;791:			diff = AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]);
ADDRLP4 44
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 6576
ADDP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
ADDRGP4 AngleDifference
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 52
INDIRF4
ASGNF4
line 792
;792:			disired_speed = diff * factor;
ADDRLP4 20
ADDRLP4 12
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ASGNF4
line 793
;793:			bs->viewanglespeed[i] += (bs->viewanglespeed[i] - disired_speed);
ADDRLP4 56
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
ASGNP4
ADDRLP4 60
ADDRLP4 56
INDIRP4
INDIRF4
ASGNF4
ADDRLP4 56
INDIRP4
ADDRLP4 60
INDIRF4
ADDRLP4 60
INDIRF4
ADDRLP4 20
INDIRF4
SUBF4
ADDF4
ASGNF4
line 794
;794:			if (bs->viewanglespeed[i] > 180) bs->viewanglespeed[i] = maxchange;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
INDIRF4
CNSTF4 1127481344
LEF4 $369
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
LABELV $369
line 795
;795:			if (bs->viewanglespeed[i] < -180) bs->viewanglespeed[i] = -maxchange;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
INDIRF4
CNSTF4 3274964992
GEF4 $371
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
ADDRLP4 8
INDIRF4
NEGF4
ASGNF4
LABELV $371
line 796
;796:			anglespeed = bs->viewanglespeed[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
INDIRF4
ASGNF4
line 797
;797:			if (anglespeed > maxchange) anglespeed = maxchange;
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $373
ADDRLP4 4
ADDRLP4 8
INDIRF4
ASGNF4
LABELV $373
line 798
;798:			if (anglespeed < -maxchange) anglespeed = -maxchange;
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
NEGF4
GEF4 $375
ADDRLP4 4
ADDRLP4 8
INDIRF4
NEGF4
ASGNF4
LABELV $375
line 799
;799:			bs->viewangles[i] += anglespeed;
ADDRLP4 64
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
ADDF4
ASGNF4
line 800
;800:			bs->viewangles[i] = AngleMod(bs->viewangles[i]);
ADDRLP4 68
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 68
INDIRP4
ADDRLP4 72
INDIRF4
ASGNF4
line 802
;801:			//demping
;802:			bs->viewanglespeed[i] *= 0.45 * (1 - factor);
ADDRLP4 76
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6588
ADDP4
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRF4
CNSTF4 1055286886
CNSTF4 1065353216
ADDRLP4 16
INDIRF4
SUBF4
MULF4
MULF4
ASGNF4
line 803
;803:		}
LABELV $365
line 806
;804:		//BotAI_Print(PRT_MESSAGE, "ideal_angles %f %f\n", bs->ideal_viewangles[0], bs->ideal_viewangles[1], bs->ideal_viewangles[2]);`
;805:		//bs->viewangles[i] = bs->ideal_viewangles[i];
;806:	}
LABELV $361
line 777
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LTI4 $360
line 808
;807:	//bs->viewangles[PITCH] = 0;
;808:	if (bs->viewangles[PITCH] > 180) bs->viewangles[PITCH] -= 360;
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
INDIRF4
CNSTF4 1127481344
LEF4 $377
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
LABELV $377
line 810
;809:	//elementary action: view
;810:	trap_EA_View(bs->client, bs->viewangles);
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 32
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRGP4 trap_EA_View
CALLV
pop
line 811
;811:}
LABELV $353
endproc BotChangeViewAngles 80 16
export BotInputToUserCommand
proc BotInputToUserCommand 136 16
line 818
;812:
;813:/*
;814:==============
;815:BotInputToUserCommand
;816:==============
;817:*/
;818:void BotInputToUserCommand(bot_input_t *bi, usercmd_t *ucmd, int delta_angles[3], int time) {
line 824
;819:	vec3_t angles, forward, right;
;820:	short temp;
;821:	int j;
;822:
;823:	//clear the whole structure
;824:	memset(ucmd, 0, sizeof(usercmd_t));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 24
ARGI4
ADDRGP4 memset
CALLP4
pop
line 828
;825:	//
;826:	//Com_Printf("dir = %f %f %f speed = %f\n", bi->dir[0], bi->dir[1], bi->dir[2], bi->speed);
;827:	//the duration for the user command in milli seconds
;828:	ucmd->serverTime = time;
ADDRFP4 4
INDIRP4
ADDRFP4 12
INDIRI4
ASGNI4
line 830
;829:	//
;830:	if (bi->actionflags & ACTION_DELAYEDJUMP) {
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 32768
BANDI4
CNSTI4 0
EQI4 $380
line 831
;831:		bi->actionflags |= ACTION_JUMP;
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 832
;832:		bi->actionflags &= ~ACTION_DELAYEDJUMP;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 -32769
BANDI4
ASGNI4
line 833
;833:	}
LABELV $380
line 835
;834:	//set the buttons
;835:	if (bi->actionflags & ACTION_RESPAWN) ucmd->buttons = BUTTON_ATTACK;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $382
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 1
ASGNI4
LABELV $382
line 836
;836:	if (bi->actionflags & ACTION_ATTACK) ucmd->buttons |= BUTTON_ATTACK;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $384
ADDRLP4 44
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
LABELV $384
line 837
;837:	if (bi->actionflags & ACTION_TALK) ucmd->buttons |= BUTTON_TALK;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 65536
BANDI4
CNSTI4 0
EQI4 $386
ADDRLP4 48
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
LABELV $386
line 838
;838:	if (bi->actionflags & ACTION_GESTURE) ucmd->buttons |= BUTTON_GESTURE;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 131072
BANDI4
CNSTI4 0
EQI4 $388
ADDRLP4 52
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
LABELV $388
line 839
;839:	if (bi->actionflags & ACTION_USE) ucmd->buttons |= BUTTON_USE_HOLDABLE;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $390
ADDRLP4 56
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
LABELV $390
line 840
;840:	if (bi->actionflags & ACTION_WALK) ucmd->buttons |= BUTTON_WALKING;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 524288
BANDI4
CNSTI4 0
EQI4 $392
ADDRLP4 60
CNSTI4 16
ASGNI4
ADDRLP4 64
ADDRFP4 4
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
ADDRLP4 60
INDIRI4
BORI4
ASGNI4
LABELV $392
line 841
;841:	if (bi->actionflags & ACTION_AFFIRMATIVE) ucmd->buttons |= BUTTON_AFFIRMATIVE;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 1048576
BANDI4
CNSTI4 0
EQI4 $394
ADDRLP4 68
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
LABELV $394
line 842
;842:	if (bi->actionflags & ACTION_NEGATIVE) ucmd->buttons |= BUTTON_NEGATIVE;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 2097152
BANDI4
CNSTI4 0
EQI4 $396
ADDRLP4 72
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
LABELV $396
line 843
;843:	if (bi->actionflags & ACTION_GETFLAG) ucmd->buttons |= BUTTON_GETFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 8388608
BANDI4
CNSTI4 0
EQI4 $398
ADDRLP4 76
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
LABELV $398
line 844
;844:	if (bi->actionflags & ACTION_GUARDBASE) ucmd->buttons |= BUTTON_GUARDBASE;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 16777216
BANDI4
CNSTI4 0
EQI4 $400
ADDRLP4 80
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
LABELV $400
line 845
;845:	if (bi->actionflags & ACTION_PATROL) ucmd->buttons |= BUTTON_PATROL;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 33554432
BANDI4
CNSTI4 0
EQI4 $402
ADDRLP4 84
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
LABELV $402
line 846
;846:	if (bi->actionflags & ACTION_FOLLOWME) ucmd->buttons |= BUTTON_FOLLOWME;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 134217728
BANDI4
CNSTI4 0
EQI4 $404
ADDRLP4 88
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 1024
BORI4
ASGNI4
LABELV $404
line 848
;847:	//
;848:	ucmd->weapon = bi->weapon;
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CVIU4 4
CVUU1 4
ASGNU1
line 851
;849:	//set the view angles
;850:	//NOTE: the ucmd->angles are the angles WITHOUT the delta angles
;851:	ucmd->angles[PITCH] = ANGLE2SHORT(bi->viewangles[PITCH]);
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTF4 1199570944
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 852
;852:	ucmd->angles[YAW] = ANGLE2SHORT(bi->viewangles[YAW]);
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
CNSTF4 1199570944
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 853
;853:	ucmd->angles[ROLL] = ANGLE2SHORT(bi->viewangles[ROLL]);
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
CNSTF4 1199570944
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 855
;854:	//subtract the delta angles
;855:	for (j = 0; j < 3; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $406
line 856
;856:		temp = ucmd->angles[j] - delta_angles[j];
ADDRLP4 92
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
ADDRLP4 92
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDP4
INDIRI4
ADDRLP4 92
INDIRI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRI4
SUBI4
CVII2 4
ASGNI2
line 864
;857:		/*NOTE: disabled because temp should be mod first
;858:		if ( j == PITCH ) {
;859:			// don't let the player look up or down more than 90 degrees
;860:			if ( temp > 16000 ) temp = 16000;
;861:			else if ( temp < -16000 ) temp = -16000;
;862:		}
;863:		*/
;864:		ucmd->angles[j] = temp;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDP4
ADDRLP4 4
INDIRI2
CVII4 2
ASGNI4
line 865
;865:	}
LABELV $407
line 855
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $406
line 869
;866:	//NOTE: movement is relative to the REAL view angles
;867:	//get the horizontal forward and right vector
;868:	//get the pitch in the range [-180, 180]
;869:	if (bi->dir[2]) angles[PITCH] = bi->viewangles[PITCH];
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 0
EQF4 $410
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ASGNF4
ADDRGP4 $411
JUMPV
LABELV $410
line 870
;870:	else angles[PITCH] = 0;
ADDRLP4 20
CNSTF4 0
ASGNF4
LABELV $411
line 871
;871:	angles[YAW] = bi->viewangles[YAW];
ADDRLP4 20+4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
line 872
;872:	angles[ROLL] = 0;
ADDRLP4 20+8
CNSTF4 0
ASGNF4
line 873
;873:	AngleVectors(angles, forward, right, NULL);
ADDRLP4 20
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 32
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 875
;874:	//bot input speed is in the range [0, 400]
;875:	bi->speed = bi->speed * 127 / 400;
ADDRLP4 92
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTF4 1123942400
ADDRLP4 92
INDIRP4
INDIRF4
MULF4
CNSTF4 1137180672
DIVF4
ASGNF4
line 877
;876:	//set the view independent movement
;877:	ucmd->forwardmove = DotProduct(forward, bi->dir) * bi->speed;
ADDRLP4 96
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 21
ADDP4
ADDRLP4 8
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 8+4
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 8+8
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 96
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
MULF4
CVFI4 4
CVII1 4
ASGNI1
line 878
;878:	ucmd->rightmove = DotProduct(right, bi->dir) * bi->speed;
ADDRLP4 100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 22
ADDP4
ADDRLP4 32
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 32+4
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 32+8
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 100
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
MULF4
CVFI4 4
CVII1 4
ASGNI1
line 879
;879:	ucmd->upmove = abs(forward[2]) * bi->dir[2] * bi->speed;
ADDRLP4 8+8
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 104
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 23
ADDP4
ADDRLP4 104
INDIRI4
CVIF4 4
ADDRLP4 108
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
MULF4
ADDRLP4 108
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
MULF4
CVFI4 4
CVII1 4
ASGNI1
line 881
;880:	//normal keyboard movement
;881:	if (bi->actionflags & ACTION_MOVEFORWARD) ucmd->forwardmove += 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $419
ADDRLP4 112
ADDRFP4 4
INDIRP4
CNSTI4 21
ADDP4
ASGNP4
ADDRLP4 112
INDIRP4
ADDRLP4 112
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
ADDI4
CVII1 4
ASGNI1
LABELV $419
line 882
;882:	if (bi->actionflags & ACTION_MOVEBACK) ucmd->forwardmove -= 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $421
ADDRLP4 116
ADDRFP4 4
INDIRP4
CNSTI4 21
ADDP4
ASGNP4
ADDRLP4 116
INDIRP4
ADDRLP4 116
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
SUBI4
CVII1 4
ASGNI1
LABELV $421
line 883
;883:	if (bi->actionflags & ACTION_MOVELEFT) ucmd->rightmove -= 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $423
ADDRLP4 120
ADDRFP4 4
INDIRP4
CNSTI4 22
ADDP4
ASGNP4
ADDRLP4 120
INDIRP4
ADDRLP4 120
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
SUBI4
CVII1 4
ASGNI1
LABELV $423
line 884
;884:	if (bi->actionflags & ACTION_MOVERIGHT) ucmd->rightmove += 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $425
ADDRLP4 124
ADDRFP4 4
INDIRP4
CNSTI4 22
ADDP4
ASGNP4
ADDRLP4 124
INDIRP4
ADDRLP4 124
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
ADDI4
CVII1 4
ASGNI1
LABELV $425
line 886
;885:	//jump/moveup
;886:	if (bi->actionflags & ACTION_JUMP) ucmd->upmove += 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $427
ADDRLP4 128
ADDRFP4 4
INDIRP4
CNSTI4 23
ADDP4
ASGNP4
ADDRLP4 128
INDIRP4
ADDRLP4 128
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
ADDI4
CVII1 4
ASGNI1
LABELV $427
line 888
;887:	//crouch/movedown
;888:	if (bi->actionflags & ACTION_CROUCH) ucmd->upmove -= 127;
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $429
ADDRLP4 132
ADDRFP4 4
INDIRP4
CNSTI4 23
ADDP4
ASGNP4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
SUBI4
CVII1 4
ASGNI1
LABELV $429
line 892
;889:	//
;890:	//Com_Printf("forward = %d right = %d up = %d\n", ucmd.forwardmove, ucmd.rightmove, ucmd.upmove);
;891:	//Com_Printf("ucmd->serverTime = %d\n", ucmd->serverTime);
;892:}
LABELV $379
endproc BotInputToUserCommand 136 16
export BotUpdateInput
proc BotUpdateInput 64 16
line 899
;893:
;894:/*
;895:==============
;896:BotUpdateInput
;897:==============
;898:*/
;899:void BotUpdateInput(bot_state_t *bs, int time, int elapsed_time) {
line 904
;900:	bot_input_t bi;
;901:	int j;
;902:
;903:	//add the delta angles to the bot's current view angles
;904:	for (j = 0; j < 3; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $432
line 905
;905:		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
ADDRLP4 44
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
INDIRF4
CNSTF4 1001652224
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 72
ADDP4
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ARGF4
ADDRLP4 56
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 52
INDIRP4
ADDRLP4 56
INDIRF4
ASGNF4
line 906
;906:	}
LABELV $433
line 904
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $432
line 908
;907:	//change the bot view angles
;908:	BotChangeViewAngles(bs, (float) elapsed_time / 1000);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ARGF4
ADDRGP4 BotChangeViewAngles
CALLV
pop
line 910
;909:	//retrieve the bot input
;910:	trap_EA_GetInput(bs->client, (float) time / 1000, &bi);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ARGF4
ADDRLP4 4
ARGP4
ADDRGP4 trap_EA_GetInput
CALLV
pop
line 912
;911:	//respawn hack
;912:	if (bi.actionflags & ACTION_RESPAWN) {
ADDRLP4 4+32
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $436
line 913
;913:		if (bs->lastucmd.buttons & BUTTON_ATTACK) bi.actionflags &= ~(ACTION_RESPAWN|ACTION_ATTACK);
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $439
ADDRLP4 4+32
ADDRLP4 4+32
INDIRI4
CNSTI4 -10
BANDI4
ASGNI4
LABELV $439
line 914
;914:	}
LABELV $436
line 916
;915:	//convert the bot input to a usercmd
;916:	BotInputToUserCommand(&bi, &bs->lastucmd, bs->cur_ps.delta_angles, time);
ADDRLP4 4
ARGP4
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 488
ADDP4
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 72
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 BotInputToUserCommand
CALLV
pop
line 918
;917:	//subtract the delta angles
;918:	for (j = 0; j < 3; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $442
line 919
;919:		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
ADDRLP4 48
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 48
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
INDIRF4
CNSTF4 1001652224
ADDRLP4 48
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 72
ADDP4
ADDP4
INDIRI4
CVIF4 4
MULF4
SUBF4
ARGF4
ADDRLP4 60
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 56
INDIRP4
ADDRLP4 60
INDIRF4
ASGNF4
line 920
;920:	}
LABELV $443
line 918
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $442
line 921
;921:}
LABELV $431
endproc BotUpdateInput 64 16
export BotAIRegularUpdate
proc BotAIRegularUpdate 0 0
line 928
;922:
;923:/*
;924:==============
;925:BotAIRegularUpdate
;926:==============
;927:*/
;928:void BotAIRegularUpdate(void) {
line 929
;929:	if (regularupdate_time < FloatTime()) {
ADDRGP4 regularupdate_time
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $447
line 930
;930:		trap_BotUpdateEntityItems();
ADDRGP4 trap_BotUpdateEntityItems
CALLV
pop
line 931
;931:		regularupdate_time = FloatTime() + 0.3;
ADDRGP4 regularupdate_time
ADDRGP4 floattime
INDIRF4
CNSTF4 1050253722
ADDF4
ASGNF4
line 932
;932:	}
LABELV $447
line 933
;933:}
LABELV $446
endproc BotAIRegularUpdate 0 0
export RemoveColorEscapeSequences
proc RemoveColorEscapeSequences 28 0
line 940
;934:
;935:/*
;936:==============
;937:RemoveColorEscapeSequences
;938:==============
;939:*/
;940:void RemoveColorEscapeSequences( char *text ) {
line 943
;941:	int i, l;
;942:
;943:	l = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 944
;944:	for ( i = 0; text[i]; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $453
JUMPV
LABELV $450
line 945
;945:		if (Q_IsColorString(&text[i])) {
ADDRLP4 8
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $454
ADDRLP4 12
CNSTI4 94
ASGNI4
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
NEI4 $454
ADDRLP4 16
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $454
ADDRLP4 16
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $454
line 946
;946:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 947
;947:			continue;
ADDRGP4 $451
JUMPV
LABELV $454
line 949
;948:		}
;949:		if (text[i] > 0x7E)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 126
LEI4 $456
line 950
;950:			continue;
ADDRGP4 $451
JUMPV
LABELV $456
line 951
;951:		text[l++] = text[i];
ADDRLP4 20
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRP4
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 952
;952:	}
LABELV $451
line 944
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $453
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $450
line 953
;953:	text[l] = '\0';
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 954
;954:}
LABELV $449
endproc RemoveColorEscapeSequences 28 0
export BotAI
proc BotAI 1092 12
line 961
;955:
;956:/*
;957:==============
;958:BotAI
;959:==============
;960:*/
;961:int BotAI(int client, float thinktime) {
line 966
;962:	bot_state_t *bs;
;963:	char buf[1024], *args;
;964:	int j;
;965:
;966:	trap_EA_ResetInput(client);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 trap_EA_ResetInput
CALLV
pop
line 968
;967:	//
;968:	bs = botstates[client];
ADDRLP4 4
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
line 969
;969:	if (!bs || !bs->inuse) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $461
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $459
LABELV $461
line 970
;970:		BotAI_Print(PRT_FATAL, "BotAI: client %d is not setup\n", client);
CNSTI4 4
ARGI4
ADDRGP4 $462
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 971
;971:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $458
JUMPV
LABELV $459
line 975
;972:	}
;973:
;974:	//retrieve the current client state
;975:	BotAI_GetClientState( client, &bs->cur_ps );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
ADDRGP4 $464
JUMPV
LABELV $463
line 978
;976:
;977:	//retrieve any waiting server commands
;978:	while( trap_BotGetServerCommand(client, buf, sizeof(buf)) ) {
line 980
;979:		//have buf point to the command and args to the command arguments
;980:		args = strchr( buf, ' ');
ADDRLP4 12
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 1040
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1040
INDIRP4
ASGNP4
line 981
;981:		if (!args) continue;
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $466
ADDRGP4 $464
JUMPV
LABELV $466
line 982
;982:		*args++ = '\0';
ADDRLP4 1044
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 1044
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1044
INDIRP4
CNSTI1 0
ASGNI1
line 985
;983:
;984:		//remove color espace sequences from the arguments
;985:		RemoveColorEscapeSequences( args );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 RemoveColorEscapeSequences
CALLV
pop
line 987
;986:
;987:		if (!Q_stricmp(buf, "cp "))
ADDRLP4 12
ARGP4
ADDRGP4 $470
ARGP4
ADDRLP4 1048
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $468
line 988
;988:			{ /*CenterPrintf*/ }
ADDRGP4 $469
JUMPV
LABELV $468
line 989
;989:		else if (!Q_stricmp(buf, "cs"))
ADDRLP4 12
ARGP4
ADDRGP4 $473
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $471
line 990
;990:			{ /*ConfigStringModified*/ }
ADDRGP4 $472
JUMPV
LABELV $471
line 991
;991:		else if (!Q_stricmp(buf, "print")) {
ADDRLP4 12
ARGP4
ADDRGP4 $476
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $474
line 993
;992:			//remove first and last quote from the chat message
;993:			memmove(args, args+1, strlen(args));
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 1060
INDIRI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 994
;994:			args[strlen(args)-1] = '\0';
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1068
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 8
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 995
;995:			trap_BotQueueConsoleMessage(bs->cs, CMS_NORMAL, args);
ADDRLP4 4
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_BotQueueConsoleMessage
CALLV
pop
line 996
;996:		}
ADDRGP4 $475
JUMPV
LABELV $474
line 997
;997:		else if (!Q_stricmp(buf, "chat")) {
ADDRLP4 12
ARGP4
ADDRGP4 $479
ARGP4
ADDRLP4 1060
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $477
line 999
;998:			//remove first and last quote from the chat message
;999:			memmove(args, args+1, strlen(args));
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 1064
INDIRI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 1000
;1000:			args[strlen(args)-1] = '\0';
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 8
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 1001
;1001:			trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, args);
ADDRLP4 4
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_BotQueueConsoleMessage
CALLV
pop
line 1002
;1002:		}
ADDRGP4 $478
JUMPV
LABELV $477
line 1003
;1003:		else if (!Q_stricmp(buf, "tchat")) {
ADDRLP4 12
ARGP4
ADDRGP4 $482
ARGP4
ADDRLP4 1064
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $480
line 1005
;1004:			//remove first and last quote from the chat message
;1005:			memmove(args, args+1, strlen(args));
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 1068
INDIRI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 1006
;1006:			args[strlen(args)-1] = '\0';
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1076
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 8
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 1007
;1007:			trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, args);
ADDRLP4 4
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_BotQueueConsoleMessage
CALLV
pop
line 1008
;1008:		}
ADDRGP4 $481
JUMPV
LABELV $480
line 1020
;1009:#ifdef MISSIONPACK
;1010:		else if (!Q_stricmp(buf, "vchat")) {
;1011:			BotVoiceChatCommand(bs, SAY_ALL, args);
;1012:		}
;1013:		else if (!Q_stricmp(buf, "vtchat")) {
;1014:			BotVoiceChatCommand(bs, SAY_TEAM, args);
;1015:		}
;1016:		else if (!Q_stricmp(buf, "vtell")) {
;1017:			BotVoiceChatCommand(bs, SAY_TELL, args);
;1018:		}
;1019:#endif
;1020:		else if (!Q_stricmp(buf, "scores"))
ADDRLP4 12
ARGP4
ADDRGP4 $485
ARGP4
ADDRLP4 1068
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1068
INDIRI4
CNSTI4 0
NEI4 $483
line 1021
;1021:			{ /*FIXME: parse scores?*/ }
ADDRGP4 $484
JUMPV
LABELV $483
line 1022
;1022:		else if (!Q_stricmp(buf, "clientLevelShot"))
ADDRLP4 12
ARGP4
ADDRGP4 $488
ARGP4
ADDRLP4 1072
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $486
line 1023
;1023:			{ /*ignore*/ }
LABELV $486
LABELV $484
LABELV $481
LABELV $478
LABELV $475
LABELV $472
LABELV $469
line 1024
;1024:	}
LABELV $464
line 978
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1040
ADDRGP4 trap_BotGetServerCommand
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $463
line 1026
;1025:	//add the delta angles to the bot's current view angles
;1026:	for (j = 0; j < 3; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $489
line 1027
;1027:		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
ADDRLP4 1044
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1052
ADDRLP4 1044
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
INDIRF4
CNSTF4 1001652224
ADDRLP4 1044
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 72
ADDP4
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ARGF4
ADDRLP4 1056
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1052
INDIRP4
ADDRLP4 1056
INDIRF4
ASGNF4
line 1028
;1028:	}
LABELV $490
line 1026
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $489
line 1030
;1029:	//increase the local time of the bot
;1030:	bs->ltime += thinktime;
ADDRLP4 1044
ADDRLP4 4
INDIRP4
CNSTI4 6060
ADDP4
ASGNP4
ADDRLP4 1044
INDIRP4
ADDRLP4 1044
INDIRP4
INDIRF4
ADDRFP4 4
INDIRF4
ADDF4
ASGNF4
line 1032
;1031:	//
;1032:	bs->thinktime = thinktime;
ADDRLP4 4
INDIRP4
CNSTI4 4904
ADDP4
ADDRFP4 4
INDIRF4
ASGNF4
line 1034
;1033:	//origin of the bot
;1034:	VectorCopy(bs->cur_ps.origin, bs->origin);
ADDRLP4 4
INDIRP4
CNSTI4 4908
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRB
ASGNB 12
line 1036
;1035:	//eye coordinates of the bot
;1036:	VectorCopy(bs->cur_ps.origin, bs->eye);
ADDRLP4 4
INDIRP4
CNSTI4 4936
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRB
ASGNB 12
line 1037
;1037:	bs->eye[2] += bs->cur_ps.viewheight;
ADDRLP4 1060
ADDRLP4 4
INDIRP4
CNSTI4 4944
ADDP4
ASGNP4
ADDRLP4 1060
INDIRP4
ADDRLP4 1060
INDIRP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1039
;1038:	//get the area the bot is in
;1039:	bs->areanum = BotPointAreaNum(bs->origin);
ADDRLP4 4
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 1068
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 4948
ADDP4
ADDRLP4 1068
INDIRI4
ASGNI4
line 1041
;1040:	//the real AI
;1041:	BotDeathmatchAI(bs, thinktime);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRGP4 BotDeathmatchAI
CALLV
pop
line 1043
;1042:	//set the weapon selection every AI frame
;1043:	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_SelectWeapon
CALLV
pop
line 1045
;1044:	//subtract the delta angles
;1045:	for (j = 0; j < 3; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $493
line 1046
;1046:		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
ADDRLP4 1076
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1084
ADDRLP4 1076
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 6564
ADDP4
ADDP4
ASGNP4
ADDRLP4 1084
INDIRP4
INDIRF4
CNSTF4 1001652224
ADDRLP4 1076
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 72
ADDP4
ADDP4
INDIRI4
CVIF4 4
MULF4
SUBF4
ARGF4
ADDRLP4 1088
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1084
INDIRP4
ADDRLP4 1088
INDIRF4
ASGNF4
line 1047
;1047:	}
LABELV $494
line 1045
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $493
line 1049
;1048:	//everything was ok
;1049:	return qtrue;
CNSTI4 1
RETI4
LABELV $458
endproc BotAI 1092 12
export BotScheduleBotThink
proc BotScheduleBotThink 12 0
line 1057
;1050:}
;1051:
;1052:/*
;1053:==================
;1054:BotScheduleBotThink
;1055:==================
;1056:*/
;1057:void BotScheduleBotThink(void) {
line 1060
;1058:	int i, botnum;
;1059:
;1060:	botnum = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1062
;1061:
;1062:	for( i = 0; i < MAX_CLIENTS; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $498
line 1063
;1063:		if( !botstates[i] || !botstates[i]->inuse ) {
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $504
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $502
LABELV $504
line 1064
;1064:			continue;
ADDRGP4 $499
JUMPV
LABELV $502
line 1067
;1065:		}
;1066:		//initialize the bot think residual time
;1067:		botstates[i]->botthink_residual = bot_thinktime.integer * botnum / numbots;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 bot_thinktime+12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 numbots
INDIRI4
DIVI4
ASGNI4
line 1068
;1068:		botnum++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1069
;1069:	}
LABELV $499
line 1062
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $498
line 1070
;1070:}
LABELV $497
endproc BotScheduleBotThink 12 0
export BotWriteSessionData
proc BotWriteSessionData 20 72
line 1077
;1071:
;1072:/*
;1073:==============
;1074:BotWriteSessionData
;1075:==============
;1076:*/
;1077:void BotWriteSessionData(bot_state_t *bs) {
line 1081
;1078:	const char	*s;
;1079:	const char	*var;
;1080:
;1081:	s = va(
ADDRGP4 $507
ARGP4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 6756
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6764
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6780
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6808
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6816
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6820
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6812
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6768
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6772
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6776
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6784
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6788
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6792
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6796
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6800
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 6804
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 1105
;1082:			"%i %i %i %i %i %i %i %i"
;1083:			" %f %f %f"
;1084:			" %f %f %f"
;1085:			" %f %f %f",
;1086:		bs->lastgoal_decisionmaker,
;1087:		bs->lastgoal_ltgtype,
;1088:		bs->lastgoal_teammate,
;1089:		bs->lastgoal_teamgoal.areanum,
;1090:		bs->lastgoal_teamgoal.entitynum,
;1091:		bs->lastgoal_teamgoal.flags,
;1092:		bs->lastgoal_teamgoal.iteminfo,
;1093:		bs->lastgoal_teamgoal.number,
;1094:		bs->lastgoal_teamgoal.origin[0],
;1095:		bs->lastgoal_teamgoal.origin[1],
;1096:		bs->lastgoal_teamgoal.origin[2],
;1097:		bs->lastgoal_teamgoal.mins[0],
;1098:		bs->lastgoal_teamgoal.mins[1],
;1099:		bs->lastgoal_teamgoal.mins[2],
;1100:		bs->lastgoal_teamgoal.maxs[0],
;1101:		bs->lastgoal_teamgoal.maxs[1],
;1102:		bs->lastgoal_teamgoal.maxs[2]
;1103:		);
;1104:
;1105:	var = va( "botsession%i", bs->client );
ADDRGP4 $508
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ASGNP4
line 1107
;1106:
;1107:	trap_Cvar_Set( var, s );
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1108
;1108:}
LABELV $506
endproc BotWriteSessionData 20 72
export BotReadSessionData
proc BotReadSessionData 1036 76
line 1115
;1109:
;1110:/*
;1111:==============
;1112:BotReadSessionData
;1113:==============
;1114:*/
;1115:void BotReadSessionData(bot_state_t *bs) {
line 1119
;1116:	char	s[MAX_STRING_CHARS];
;1117:	const char	*var;
;1118:
;1119:	var = va( "botsession%i", bs->client );
ADDRGP4 $508
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1024
ADDRLP4 1028
INDIRP4
ASGNP4
line 1120
;1120:	trap_Cvar_VariableStringBuffer( var, s, sizeof(s) );
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1122
;1121:
;1122:	sscanf(s,
ADDRLP4 0
ARGP4
ADDRGP4 $507
ARGP4
ADDRLP4 1032
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1032
INDIRP4
CNSTI4 6756
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6760
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6764
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6780
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6808
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6816
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6820
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6812
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6768
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6772
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6776
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6784
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6788
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6792
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6796
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6800
ADDP4
ARGP4
ADDRLP4 1032
INDIRP4
CNSTI4 6804
ADDP4
ARGP4
ADDRGP4 sscanf
CALLI4
pop
line 1145
;1123:			"%i %i %i %i %i %i %i %i"
;1124:			" %f %f %f"
;1125:			" %f %f %f"
;1126:			" %f %f %f",
;1127:		&bs->lastgoal_decisionmaker,
;1128:		&bs->lastgoal_ltgtype,
;1129:		&bs->lastgoal_teammate,
;1130:		&bs->lastgoal_teamgoal.areanum,
;1131:		&bs->lastgoal_teamgoal.entitynum,
;1132:		&bs->lastgoal_teamgoal.flags,
;1133:		&bs->lastgoal_teamgoal.iteminfo,
;1134:		&bs->lastgoal_teamgoal.number,
;1135:		&bs->lastgoal_teamgoal.origin[0],
;1136:		&bs->lastgoal_teamgoal.origin[1],
;1137:		&bs->lastgoal_teamgoal.origin[2],
;1138:		&bs->lastgoal_teamgoal.mins[0],
;1139:		&bs->lastgoal_teamgoal.mins[1],
;1140:		&bs->lastgoal_teamgoal.mins[2],
;1141:		&bs->lastgoal_teamgoal.maxs[0],
;1142:		&bs->lastgoal_teamgoal.maxs[1],
;1143:		&bs->lastgoal_teamgoal.maxs[2]
;1144:		);
;1145:}
LABELV $509
endproc BotReadSessionData 1036 76
export BotAISetupClient
proc BotAISetupClient 512 16
line 1152
;1146:
;1147:/*
;1148:==============
;1149:BotAISetupClient
;1150:==============
;1151:*/
;1152:int BotAISetupClient(int client, struct bot_settings_s *settings, qboolean restart) {
line 1157
;1153:	char filename[MAX_PATH], name[MAX_PATH], gender[MAX_PATH];
;1154:	bot_state_t *bs;
;1155:	int errnum;
;1156:
;1157:	if (!botstates[client]) botstates[client] = G_Alloc(sizeof(bot_state_t));
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $511
CNSTI4 9088
ARGI4
ADDRLP4 440
ADDRGP4 G_Alloc
CALLP4
ASGNP4
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
ADDRLP4 440
INDIRP4
ASGNP4
LABELV $511
line 1158
;1158:	bs = botstates[client];
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
line 1160
;1159:
;1160:	if (bs && bs->inuse) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $513
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $513
line 1161
;1161:		BotAI_Print(PRT_FATAL, "BotAISetupClient: client %d already setup\n", client);
CNSTI4 4
ARGI4
ADDRGP4 $515
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 1162
;1162:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $513
line 1165
;1163:	}
;1164:
;1165:	if (!trap_AAS_Initialized()) {
ADDRLP4 448
ADDRGP4 trap_AAS_Initialized
CALLI4
ASGNI4
ADDRLP4 448
INDIRI4
CNSTI4 0
NEI4 $516
line 1166
;1166:		BotAI_Print(PRT_FATAL, "AAS not initialized\n");
CNSTI4 4
ARGI4
ADDRGP4 $518
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 1167
;1167:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $516
line 1171
;1168:	}
;1169:
;1170:	//load the bot character
;1171:	bs->character = trap_BotLoadCharacter(settings->characterfile, settings->skill);
ADDRLP4 452
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 452
INDIRP4
ARGP4
ADDRLP4 452
INDIRP4
CNSTI4 144
ADDP4
INDIRF4
ARGF4
ADDRLP4 456
ADDRGP4 trap_BotLoadCharacter
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
ADDRLP4 456
INDIRI4
ASGNI4
line 1172
;1172:	if (!bs->character) {
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $519
line 1173
;1173:		BotAI_Print(PRT_FATAL, "couldn't load skill %f from %s\n", settings->skill, settings->characterfile);
CNSTI4 4
ARGI4
ADDRGP4 $521
ARGP4
ADDRLP4 460
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 460
INDIRP4
CNSTI4 144
ADDP4
INDIRF4
ARGF4
ADDRLP4 460
INDIRP4
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 1174
;1174:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $519
line 1177
;1175:	}
;1176:	//copy the settings
;1177:	memcpy(&bs->settings, settings, sizeof(bot_settings_t));
ADDRLP4 0
INDIRP4
CNSTI4 4608
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 292
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1179
;1178:	//allocate a goal state
;1179:	bs->gs = trap_BotAllocGoalState(client);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 460
ADDRGP4 trap_BotAllocGoalState
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
ADDRLP4 460
INDIRI4
ASGNI4
line 1181
;1180:	//load the item weights
;1181:	trap_Characteristic_String(bs->character, CHARACTERISTIC_ITEMWEIGHTS, filename, MAX_PATH);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 40
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 1182
;1182:	errnum = trap_BotLoadItemWeights(bs->gs, filename);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 464
ADDRGP4 trap_BotLoadItemWeights
CALLI4
ASGNI4
ADDRLP4 148
ADDRLP4 464
INDIRI4
ASGNI4
line 1183
;1183:	if (errnum != BLERR_NOERROR) {
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $522
line 1184
;1184:		trap_BotFreeGoalState(bs->gs);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeGoalState
CALLV
pop
line 1185
;1185:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $522
line 1188
;1186:	}
;1187:	//allocate a weapon state
;1188:	bs->ws = trap_BotAllocWeaponState();
ADDRLP4 468
ADDRGP4 trap_BotAllocWeaponState
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 6536
ADDP4
ADDRLP4 468
INDIRI4
ASGNI4
line 1190
;1189:	//load the weapon weights
;1190:	trap_Characteristic_String(bs->character, CHARACTERISTIC_WEAPONWEIGHTS, filename, MAX_PATH);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 1191
;1191:	errnum = trap_BotLoadWeaponWeights(bs->ws, filename);
ADDRLP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 472
ADDRGP4 trap_BotLoadWeaponWeights
CALLI4
ASGNI4
ADDRLP4 148
ADDRLP4 472
INDIRI4
ASGNI4
line 1192
;1192:	if (errnum != BLERR_NOERROR) {
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $524
line 1193
;1193:		trap_BotFreeGoalState(bs->gs);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeGoalState
CALLV
pop
line 1194
;1194:		trap_BotFreeWeaponState(bs->ws);
ADDRLP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeWeaponState
CALLV
pop
line 1195
;1195:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $524
line 1198
;1196:	}
;1197:	//allocate a chat state
;1198:	bs->cs = trap_BotAllocChatState();
ADDRLP4 476
ADDRGP4 trap_BotAllocChatState
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
ADDRLP4 476
INDIRI4
ASGNI4
line 1200
;1199:	//load the chat file
;1200:	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_FILE, filename, MAX_PATH);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 21
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 1201
;1201:	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_NAME, name, MAX_PATH);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 22
ARGI4
ADDRLP4 296
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 1202
;1202:	errnum = trap_BotLoadChatFile(bs->cs, filename, name);
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 296
ARGP4
ADDRLP4 480
ADDRGP4 trap_BotLoadChatFile
CALLI4
ASGNI4
ADDRLP4 148
ADDRLP4 480
INDIRI4
ASGNI4
line 1203
;1203:	if (errnum != BLERR_NOERROR) {
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $526
line 1204
;1204:		trap_BotFreeChatState(bs->cs);
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeChatState
CALLV
pop
line 1205
;1205:		trap_BotFreeGoalState(bs->gs);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeGoalState
CALLV
pop
line 1206
;1206:		trap_BotFreeWeaponState(bs->ws);
ADDRLP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeWeaponState
CALLV
pop
line 1207
;1207:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $510
JUMPV
LABELV $526
line 1210
;1208:	}
;1209:	//get the gender characteristic
;1210:	trap_Characteristic_String(bs->character, CHARACTERISTIC_GENDER, gender, MAX_PATH);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 152
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 1212
;1211:	//set the chat gender
;1212:	if (*gender == 'f' || *gender == 'F') trap_BotSetChatGender(bs->cs, CHAT_GENDERFEMALE);
ADDRLP4 484
ADDRLP4 152
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 484
INDIRI4
CNSTI4 102
EQI4 $530
ADDRLP4 484
INDIRI4
CNSTI4 70
NEI4 $528
LABELV $530
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotSetChatGender
CALLV
pop
ADDRGP4 $529
JUMPV
LABELV $528
line 1213
;1213:	else if (*gender == 'm' || *gender == 'M') trap_BotSetChatGender(bs->cs, CHAT_GENDERMALE);
ADDRLP4 488
ADDRLP4 152
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 488
INDIRI4
CNSTI4 109
EQI4 $533
ADDRLP4 488
INDIRI4
CNSTI4 77
NEI4 $531
LABELV $533
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotSetChatGender
CALLV
pop
ADDRGP4 $532
JUMPV
LABELV $531
line 1214
;1214:	else trap_BotSetChatGender(bs->cs, CHAT_GENDERLESS);
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_BotSetChatGender
CALLV
pop
LABELV $532
LABELV $529
line 1216
;1215:
;1216:	bs->inuse = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1
ASGNI4
line 1217
;1217:	bs->client = client;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 1218
;1218:	bs->entitynum = client;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 1219
;1219:	bs->setupcount = 4;
ADDRLP4 0
INDIRP4
CNSTI4 6016
ADDP4
CNSTI4 4
ASGNI4
line 1220
;1220:	bs->entergame_time = FloatTime();
ADDRLP4 0
INDIRP4
CNSTI4 6064
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1221
;1221:	bs->ms = trap_BotAllocMoveState();
ADDRLP4 492
ADDRGP4 trap_BotAllocMoveState
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 6524
ADDP4
ADDRLP4 492
INDIRI4
ASGNI4
line 1222
;1222:	bs->walker = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_WALKER, 0, 1);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 48
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 500
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 6056
ADDP4
ADDRLP4 500
INDIRF4
ASGNF4
line 1223
;1223:	numbots++;
ADDRLP4 504
ADDRGP4 numbots
ASGNP4
ADDRLP4 504
INDIRP4
ADDRLP4 504
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1225
;1224:
;1225:	if (trap_Cvar_VariableIntegerValue("bot_testichat")) {
ADDRGP4 $536
ARGP4
ADDRLP4 508
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 508
INDIRI4
CNSTI4 0
EQI4 $534
line 1226
;1226:		trap_BotLibVarSet("bot_testichat", "1");
ADDRGP4 $536
ARGP4
ADDRGP4 $313
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1227
;1227:		BotChatTest(bs);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 BotChatTest
CALLV
pop
line 1228
;1228:	}
LABELV $534
line 1230
;1229:	//NOTE: reschedule the bot thinking
;1230:	BotScheduleBotThink();
ADDRGP4 BotScheduleBotThink
CALLV
pop
line 1232
;1231:	//if interbreeding start with a mutation
;1232:	if (bot_interbreed) {
ADDRGP4 bot_interbreed
INDIRI4
CNSTI4 0
EQI4 $537
line 1233
;1233:		trap_BotMutateGoalFuzzyLogic(bs->gs, 1);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_BotMutateGoalFuzzyLogic
CALLV
pop
line 1234
;1234:	}
LABELV $537
line 1236
;1235:	// if we kept the bot client
;1236:	if (restart) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $539
line 1237
;1237:		BotReadSessionData(bs);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 BotReadSessionData
CALLV
pop
line 1238
;1238:	}
LABELV $539
line 1240
;1239:	//bot has been setup succesfully
;1240:	return qtrue;
CNSTI4 1
RETI4
LABELV $510
endproc BotAISetupClient 512 16
export BotAIShutdownClient
proc BotAIShutdownClient 16 12
line 1248
;1241:}
;1242:
;1243:/*
;1244:==============
;1245:BotAIShutdownClient
;1246:==============
;1247:*/
;1248:int BotAIShutdownClient(int client, qboolean restart) {
line 1251
;1249:	bot_state_t *bs;
;1250:
;1251:	bs = botstates[client];
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
line 1252
;1252:	if (!bs || !bs->inuse) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $544
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $542
LABELV $544
line 1254
;1253:		//BotAI_Print(PRT_ERROR, "BotAIShutdownClient: client %d already shutdown\n", client);
;1254:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $541
JUMPV
LABELV $542
line 1257
;1255:	}
;1256:
;1257:	if (restart) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $545
line 1258
;1258:		BotWriteSessionData(bs);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 BotWriteSessionData
CALLV
pop
line 1259
;1259:	}
LABELV $545
line 1261
;1260:
;1261:	if (BotChat_ExitGame(bs)) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 BotChat_ExitGame
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $547
line 1262
;1262:		trap_BotEnterChat(bs->cs, bs->client, CHAT_ALL);
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1263
;1263:	}
LABELV $547
line 1265
;1264:
;1265:	trap_BotFreeMoveState(bs->ms);
ADDRLP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeMoveState
CALLV
pop
line 1267
;1266:	//free the goal state`			
;1267:	trap_BotFreeGoalState(bs->gs);
ADDRLP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeGoalState
CALLV
pop
line 1269
;1268:	//free the chat file
;1269:	trap_BotFreeChatState(bs->cs);
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeChatState
CALLV
pop
line 1271
;1270:	//free the weapon weights
;1271:	trap_BotFreeWeaponState(bs->ws);
ADDRLP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeWeaponState
CALLV
pop
line 1273
;1272:	//free the bot character
;1273:	trap_BotFreeCharacter(bs->character);
ADDRLP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotFreeCharacter
CALLV
pop
line 1275
;1274:	//
;1275:	BotFreeWaypoints(bs->checkpoints);
ADDRLP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 1276
;1276:	BotFreeWaypoints(bs->patrolpoints);
ADDRLP4 0
INDIRP4
CNSTI4 9076
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 1278
;1277:	//clear activate goal stack
;1278:	BotClearActivateGoalStack(bs);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1280
;1279:	//clear the bot state
;1280:	memset(bs, 0, sizeof(bot_state_t));
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 9088
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1282
;1281:	//set the inuse flag to qfalse
;1282:	bs->inuse = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1284
;1283:	//there's one bot less
;1284:	numbots--;
ADDRLP4 12
ADDRGP4 numbots
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1286
;1285:	//everything went ok
;1286:	return qtrue;
CNSTI4 1
RETI4
LABELV $541
endproc BotAIShutdownClient 16 12
export BotResetState
proc BotResetState 796 12
line 1297
;1287:}
;1288:
;1289:/*
;1290:==============
;1291:BotResetState
;1292:
;1293:called when a bot enters the intermission or observer mode and
;1294:when the level is changed
;1295:==============
;1296:*/
;1297:void BotResetState(bot_state_t *bs) {
line 1306
;1298:	int client, entitynum, inuse;
;1299:	int movestate, goalstate, chatstate, weaponstate;
;1300:	bot_settings_t settings;
;1301:	int character;
;1302:	playerState_t ps;							//current player state
;1303:	float entergame_time;
;1304:
;1305:	//save some things that should not be reset here
;1306:	memcpy(&settings, &bs->settings, sizeof(bot_settings_t));
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 4608
ADDP4
ARGP4
CNSTI4 292
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1307
;1307:	memcpy(&ps, &bs->cur_ps, sizeof(playerState_t));
ADDRLP4 324
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ARGP4
CNSTI4 468
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1308
;1308:	inuse = bs->inuse;
ADDRLP4 8
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1309
;1309:	client = bs->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1310
;1310:	entitynum = bs->entitynum;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1311
;1311:	character = bs->character;
ADDRLP4 320
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ASGNI4
line 1312
;1312:	movestate = bs->ms;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ASGNI4
line 1313
;1313:	goalstate = bs->gs;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ASGNI4
line 1314
;1314:	chatstate = bs->cs;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ASGNI4
line 1315
;1315:	weaponstate = bs->ws;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ASGNI4
line 1316
;1316:	entergame_time = bs->entergame_time;
ADDRLP4 792
ADDRFP4 0
INDIRP4
CNSTI4 6064
ADDP4
INDIRF4
ASGNF4
line 1318
;1317:	//free checkpoints and patrol points
;1318:	BotFreeWaypoints(bs->checkpoints);
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 1319
;1319:	BotFreeWaypoints(bs->patrolpoints);
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 1321
;1320:	//reset the whole state
;1321:	memset(bs, 0, sizeof(bot_state_t));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 9088
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1323
;1322:	//copy back some state stuff that should not be reset
;1323:	bs->ms = movestate;
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1324
;1324:	bs->gs = goalstate;
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1325
;1325:	bs->cs = chatstate;
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 1326
;1326:	bs->ws = weaponstate;
ADDRFP4 0
INDIRP4
CNSTI4 6536
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 1327
;1327:	memcpy(&bs->cur_ps, &ps, sizeof(playerState_t));
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ARGP4
ADDRLP4 324
ARGP4
CNSTI4 468
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1328
;1328:	memcpy(&bs->settings, &settings, sizeof(bot_settings_t));
ADDRFP4 0
INDIRP4
CNSTI4 4608
ADDP4
ARGP4
ADDRLP4 28
ARGP4
CNSTI4 292
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1329
;1329:	bs->inuse = inuse;
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1330
;1330:	bs->client = client;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1331
;1331:	bs->entitynum = entitynum;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1332
;1332:	bs->character = character;
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
ADDRLP4 320
INDIRI4
ASGNI4
line 1333
;1333:	bs->entergame_time = entergame_time;
ADDRFP4 0
INDIRP4
CNSTI4 6064
ADDP4
ADDRLP4 792
INDIRF4
ASGNF4
line 1335
;1334:	//reset several states
;1335:	if (bs->ms) trap_BotResetMoveState(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
CNSTI4 0
EQI4 $550
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetMoveState
CALLV
pop
LABELV $550
line 1336
;1336:	if (bs->gs) trap_BotResetGoalState(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
CNSTI4 0
EQI4 $552
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetGoalState
CALLV
pop
LABELV $552
line 1337
;1337:	if (bs->ws) trap_BotResetWeaponState(bs->ws);
ADDRFP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
CNSTI4 0
EQI4 $554
ADDRFP4 0
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetWeaponState
CALLV
pop
LABELV $554
line 1338
;1338:	if (bs->gs) trap_BotResetAvoidGoals(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
CNSTI4 0
EQI4 $556
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidGoals
CALLV
pop
LABELV $556
line 1339
;1339:	if (bs->ms) trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
CNSTI4 0
EQI4 $558
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
LABELV $558
line 1340
;1340:}
LABELV $549
endproc BotResetState 796 12
export BotAILoadMap
proc BotAILoadMap 280 16
line 1347
;1341:
;1342:/*
;1343:==============
;1344:BotAILoadMap
;1345:==============
;1346:*/
;1347:int BotAILoadMap( int restart ) {
line 1351
;1348:	int			i;
;1349:	vmCvar_t	mapname;
;1350:
;1351:	if (!restart) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $561
line 1352
;1352:		trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );
ADDRLP4 4
ARGP4
ADDRGP4 $563
ARGP4
ADDRGP4 $297
ARGP4
CNSTI4 68
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1353
;1353:		trap_BotLibLoadMap( mapname.string );
ADDRLP4 4+16
ARGP4
ADDRGP4 trap_BotLibLoadMap
CALLI4
pop
line 1354
;1354:	}
LABELV $561
line 1356
;1355:
;1356:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $565
line 1357
;1357:		if (botstates[i] && botstates[i]->inuse) {
ADDRLP4 276
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 276
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $569
ADDRLP4 276
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $569
line 1358
;1358:			BotResetState( botstates[i] );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotResetState
CALLV
pop
line 1359
;1359:			botstates[i]->setupcount = 4;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 6016
ADDP4
CNSTI4 4
ASGNI4
line 1360
;1360:		}
LABELV $569
line 1361
;1361:	}
LABELV $566
line 1356
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $565
line 1363
;1362:
;1363:	BotSetupDeathmatchAI();
ADDRGP4 BotSetupDeathmatchAI
CALLV
pop
line 1365
;1364:
;1365:	return qtrue;
CNSTI4 1
RETI4
LABELV $560
endproc BotAILoadMap 280 16
bss
align 4
LABELV $572
skip 4
align 4
LABELV $573
skip 4
align 4
LABELV $574
skip 4
export BotAIStartFrame
code
proc BotAIStartFrame 152 12
line 1377
;1366:}
;1367:
;1368:#ifdef MISSIONPACK
;1369:void ProximityMine_Trigger( gentity_t *trigger, gentity_t *other, trace_t *trace );
;1370:#endif
;1371:
;1372:/*
;1373:==================
;1374:BotAIStartFrame
;1375:==================
;1376:*/
;1377:int BotAIStartFrame(int time) {
line 1386
;1378:	int i;
;1379:	gentity_t	*ent;
;1380:	bot_entitystate_t state;
;1381:	int elapsed_time, thinktime;
;1382:	static int local_time;
;1383:	static int botlib_residual;
;1384:	static int lastbotthink_time;
;1385:
;1386:	G_CheckBotSpawn();
ADDRGP4 G_CheckBotSpawn
CALLV
pop
line 1388
;1387:
;1388:	trap_Cvar_Update(&bot_rocketjump);
ADDRGP4 bot_rocketjump
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1389
;1389:	trap_Cvar_Update(&bot_grapple);
ADDRGP4 bot_grapple
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1390
;1390:	trap_Cvar_Update(&bot_fastchat);
ADDRGP4 bot_fastchat
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1391
;1391:	trap_Cvar_Update(&bot_nochat);
ADDRGP4 bot_nochat
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1392
;1392:	trap_Cvar_Update(&bot_testrchat);
ADDRGP4 bot_testrchat
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1393
;1393:	trap_Cvar_Update(&bot_thinktime);
ADDRGP4 bot_thinktime
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1394
;1394:	trap_Cvar_Update(&bot_memorydump);
ADDRGP4 bot_memorydump
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1395
;1395:	trap_Cvar_Update(&bot_saveroutingcache);
ADDRGP4 bot_saveroutingcache
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1396
;1396:	trap_Cvar_Update(&bot_pause);
ADDRGP4 bot_pause
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1397
;1397:	trap_Cvar_Update(&bot_report);
ADDRGP4 bot_report
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 1399
;1398:
;1399:	if (bot_report.integer) {
ADDRGP4 bot_report+12
INDIRI4
CNSTI4 0
EQI4 $575
line 1402
;1400://		BotTeamplayReport();
;1401://		trap_Cvar_Set("bot_report", "0");
;1402:		BotUpdateInfoConfigStrings();
ADDRGP4 BotUpdateInfoConfigStrings
CALLV
pop
line 1403
;1403:	}
LABELV $575
line 1405
;1404:
;1405:	if (bot_pause.integer) {
ADDRGP4 bot_pause+12
INDIRI4
CNSTI4 0
EQI4 $578
line 1407
;1406:		// execute bot user commands every frame
;1407:		for( i = 0; i < MAX_CLIENTS; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $581
line 1408
;1408:			if( !botstates[i] || !botstates[i]->inuse ) {
ADDRLP4 128
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 128
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $587
ADDRLP4 128
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $585
LABELV $587
line 1409
;1409:				continue;
ADDRGP4 $582
JUMPV
LABELV $585
line 1411
;1410:			}
;1411:			if( g_entities[i].client->pers.connected != CON_CONNECTED ) {
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $588
line 1412
;1412:				continue;
ADDRGP4 $582
JUMPV
LABELV $588
line 1414
;1413:			}
;1414:			botstates[i]->lastucmd.forwardmove = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 509
ADDP4
CNSTI1 0
ASGNI1
line 1415
;1415:			botstates[i]->lastucmd.rightmove = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 510
ADDP4
CNSTI1 0
ASGNI1
line 1416
;1416:			botstates[i]->lastucmd.upmove = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 511
ADDP4
CNSTI1 0
ASGNI1
line 1417
;1417:			botstates[i]->lastucmd.buttons = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 0
ASGNI4
line 1418
;1418:			botstates[i]->lastucmd.serverTime = time;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 488
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 1419
;1419:			trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
ADDRLP4 132
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 132
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 132
INDIRP4
CNSTI4 488
ADDP4
ARGP4
ADDRGP4 trap_BotUserCommand
CALLV
pop
line 1420
;1420:		}
LABELV $582
line 1407
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $581
line 1421
;1421:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $571
JUMPV
LABELV $578
line 1424
;1422:	}
;1423:
;1424:	if (bot_memorydump.integer) {
ADDRGP4 bot_memorydump+12
INDIRI4
CNSTI4 0
EQI4 $591
line 1425
;1425:		trap_BotLibVarSet("memorydump", "1");
ADDRGP4 $594
ARGP4
ADDRGP4 $313
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1426
;1426:		trap_Cvar_Set("bot_memorydump", "0");
ADDRGP4 $595
ARGP4
ADDRGP4 $596
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1427
;1427:	}
LABELV $591
line 1428
;1428:	if (bot_saveroutingcache.integer) {
ADDRGP4 bot_saveroutingcache+12
INDIRI4
CNSTI4 0
EQI4 $597
line 1429
;1429:		trap_BotLibVarSet("saveroutingcache", "1");
ADDRGP4 $600
ARGP4
ADDRGP4 $313
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1430
;1430:		trap_Cvar_Set("bot_saveroutingcache", "0");
ADDRGP4 $601
ARGP4
ADDRGP4 $596
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1431
;1431:	}
LABELV $597
line 1433
;1432:	//check if bot interbreeding is activated
;1433:	BotInterbreeding();
ADDRGP4 BotInterbreeding
CALLV
pop
line 1435
;1434:	//cap the bot think time
;1435:	if (bot_thinktime.integer > 200) {
ADDRGP4 bot_thinktime+12
INDIRI4
CNSTI4 200
LEI4 $602
line 1436
;1436:		trap_Cvar_Set("bot_thinktime", "200");
ADDRGP4 $605
ARGP4
ADDRGP4 $606
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1437
;1437:	}
LABELV $602
line 1439
;1438:	//if the bot think time changed we should reschedule the bots
;1439:	if (bot_thinktime.integer != lastbotthink_time) {
ADDRGP4 bot_thinktime+12
INDIRI4
ADDRGP4 $574
INDIRI4
EQI4 $607
line 1440
;1440:		lastbotthink_time = bot_thinktime.integer;
ADDRGP4 $574
ADDRGP4 bot_thinktime+12
INDIRI4
ASGNI4
line 1441
;1441:		BotScheduleBotThink();
ADDRGP4 BotScheduleBotThink
CALLV
pop
line 1442
;1442:	}
LABELV $607
line 1444
;1443:
;1444:	elapsed_time = time - local_time;
ADDRLP4 120
ADDRFP4 0
INDIRI4
ADDRGP4 $572
INDIRI4
SUBI4
ASGNI4
line 1445
;1445:	local_time = time;
ADDRGP4 $572
ADDRFP4 0
INDIRI4
ASGNI4
line 1447
;1446:
;1447:	botlib_residual += elapsed_time;
ADDRLP4 128
ADDRGP4 $573
ASGNP4
ADDRLP4 128
INDIRP4
ADDRLP4 128
INDIRP4
INDIRI4
ADDRLP4 120
INDIRI4
ADDI4
ASGNI4
line 1449
;1448:
;1449:	if (elapsed_time > bot_thinktime.integer) thinktime = elapsed_time;
ADDRLP4 120
INDIRI4
ADDRGP4 bot_thinktime+12
INDIRI4
LEI4 $611
ADDRLP4 124
ADDRLP4 120
INDIRI4
ASGNI4
ADDRGP4 $612
JUMPV
LABELV $611
line 1450
;1450:	else thinktime = bot_thinktime.integer;
ADDRLP4 124
ADDRGP4 bot_thinktime+12
INDIRI4
ASGNI4
LABELV $612
line 1453
;1451:
;1452:	// update the bot library
;1453:	if ( botlib_residual >= thinktime ) {
ADDRGP4 $573
INDIRI4
ADDRLP4 124
INDIRI4
LTI4 $615
line 1454
;1454:		botlib_residual -= thinktime;
ADDRLP4 132
ADDRGP4 $573
ASGNP4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRI4
ADDRLP4 124
INDIRI4
SUBI4
ASGNI4
line 1456
;1455:
;1456:		trap_BotLibStartFrame((float) time / 1000);
ADDRFP4 0
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ARGF4
ADDRGP4 trap_BotLibStartFrame
CALLI4
pop
line 1458
;1457:
;1458:		if (!trap_AAS_Initialized()) return qfalse;
ADDRLP4 136
ADDRGP4 trap_AAS_Initialized
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 0
NEI4 $617
CNSTI4 0
RETI4
ADDRGP4 $571
JUMPV
LABELV $617
line 1461
;1459:
;1460:		//update entities in the botlib
;1461:		for (i = 0; i < MAX_GENTITIES; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $619
line 1462
;1462:			ent = &g_entities[i];
ADDRLP4 4
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1463
;1463:			if (!ent->inuse) {
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $623
line 1464
;1464:				trap_BotLibUpdateEntity(i, NULL);
ADDRLP4 0
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1465
;1465:				continue;
ADDRGP4 $620
JUMPV
LABELV $623
line 1467
;1466:			}
;1467:			if (!ent->r.linked) {
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
CNSTI4 0
NEI4 $625
line 1468
;1468:				trap_BotLibUpdateEntity(i, NULL);
ADDRLP4 0
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1469
;1469:				continue;
ADDRGP4 $620
JUMPV
LABELV $625
line 1471
;1470:			}
;1471:			if (ent->r.svFlags & SVF_NOCLIENT) {
ADDRLP4 4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $627
line 1472
;1472:				trap_BotLibUpdateEntity(i, NULL);
ADDRLP4 0
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1473
;1473:				continue;
ADDRGP4 $620
JUMPV
LABELV $627
line 1476
;1474:			}
;1475:			// do not update missiles
;1476:			if (ent->s.eType == ET_MISSILE && ent->s.weapon != WP_GRAPPLING_HOOK) {
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $629
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 10
EQI4 $629
line 1477
;1477:				trap_BotLibUpdateEntity(i, NULL);
ADDRLP4 0
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1478
;1478:				continue;
ADDRGP4 $620
JUMPV
LABELV $629
line 1481
;1479:			}
;1480:			// do not update event only entities
;1481:			if (ent->s.eType > ET_EVENTS) {
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 13
LEI4 $631
line 1482
;1482:				trap_BotLibUpdateEntity(i, NULL);
ADDRLP4 0
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1483
;1483:				continue;
ADDRGP4 $620
JUMPV
LABELV $631
line 1495
;1484:			}
;1485:#ifdef MISSIONPACK
;1486:			// never link prox mine triggers
;1487:			if (ent->r.contents == CONTENTS_TRIGGER) {
;1488:				if (ent->touch == ProximityMine_Trigger) {
;1489:					trap_BotLibUpdateEntity(i, NULL);
;1490:					continue;
;1491:				}
;1492:			}
;1493:#endif
;1494:			//
;1495:			memset(&state, 0, sizeof(bot_entitystate_t));
ADDRLP4 8
ARGP4
CNSTI4 0
ARGI4
CNSTI4 112
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1497
;1496:			//
;1497:			VectorCopy(ent->r.currentOrigin, state.origin);
ADDRLP4 8+8
ADDRLP4 4
INDIRP4
CNSTI4 488
ADDP4
INDIRB
ASGNB 12
line 1498
;1498:			if (i < MAX_CLIENTS) {
ADDRLP4 0
INDIRI4
CNSTI4 64
GEI4 $634
line 1499
;1499:				VectorCopy(ent->s.apos.trBase, state.angles);
ADDRLP4 8+20
ADDRLP4 4
INDIRP4
CNSTI4 60
ADDP4
INDIRB
ASGNB 12
line 1500
;1500:			} else {
ADDRGP4 $635
JUMPV
LABELV $634
line 1501
;1501:				VectorCopy(ent->r.currentAngles, state.angles);
ADDRLP4 8+20
ADDRLP4 4
INDIRP4
CNSTI4 500
ADDP4
INDIRB
ASGNB 12
line 1502
;1502:			}
LABELV $635
line 1503
;1503:			VectorCopy(ent->s.origin2, state.old_origin);
ADDRLP4 8+32
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 1504
;1504:			VectorCopy(ent->r.mins, state.mins);
ADDRLP4 8+44
ADDRLP4 4
INDIRP4
CNSTI4 436
ADDP4
INDIRB
ASGNB 12
line 1505
;1505:			VectorCopy(ent->r.maxs, state.maxs);
ADDRLP4 8+56
ADDRLP4 4
INDIRP4
CNSTI4 448
ADDP4
INDIRB
ASGNB 12
line 1506
;1506:			state.type = ent->s.eType;
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 1507
;1507:			state.flags = ent->s.eFlags;
ADDRLP4 8+4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1508
;1508:			if (ent->r.bmodel) state.solid = SOLID_BSP;
ADDRLP4 4
INDIRP4
CNSTI4 432
ADDP4
INDIRI4
CNSTI4 0
EQI4 $642
ADDRLP4 8+72
CNSTI4 3
ASGNI4
ADDRGP4 $643
JUMPV
LABELV $642
line 1509
;1509:			else state.solid = SOLID_BBOX;
ADDRLP4 8+72
CNSTI4 2
ASGNI4
LABELV $643
line 1510
;1510:			state.groundent = ent->s.groundEntityNum;
ADDRLP4 8+68
ADDRLP4 4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ASGNI4
line 1511
;1511:			state.modelindex = ent->s.modelindex;
ADDRLP4 8+76
ADDRLP4 4
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ASGNI4
line 1512
;1512:			state.modelindex2 = ent->s.modelindex2;
ADDRLP4 8+80
ADDRLP4 4
INDIRP4
CNSTI4 164
ADDP4
INDIRI4
ASGNI4
line 1513
;1513:			state.frame = ent->s.frame;
ADDRLP4 8+84
ADDRLP4 4
INDIRP4
CNSTI4 172
ADDP4
INDIRI4
ASGNI4
line 1514
;1514:			state.event = ent->s.event;
ADDRLP4 8+88
ADDRLP4 4
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
ASGNI4
line 1515
;1515:			state.eventParm = ent->s.eventParm;
ADDRLP4 8+92
ADDRLP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
line 1516
;1516:			state.powerups = ent->s.powerups;
ADDRLP4 8+96
ADDRLP4 4
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
ASGNI4
line 1517
;1517:			state.legsAnim = ent->s.legsAnim;
ADDRLP4 8+104
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
line 1518
;1518:			state.torsoAnim = ent->s.torsoAnim;
ADDRLP4 8+108
ADDRLP4 4
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ASGNI4
line 1519
;1519:			state.weapon = ent->s.weapon;
ADDRLP4 8+100
ADDRLP4 4
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
ASGNI4
line 1521
;1520:			//
;1521:			trap_BotLibUpdateEntity(i, &state);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRGP4 trap_BotLibUpdateEntity
CALLI4
pop
line 1522
;1522:		}
LABELV $620
line 1461
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1024
LTI4 $619
line 1524
;1523:
;1524:		BotAIRegularUpdate();
ADDRGP4 BotAIRegularUpdate
CALLV
pop
line 1525
;1525:	}
LABELV $615
line 1527
;1526:
;1527:	floattime = trap_AAS_Time();
ADDRLP4 132
ADDRGP4 trap_AAS_Time
CALLF4
ASGNF4
ADDRGP4 floattime
ADDRLP4 132
INDIRF4
ASGNF4
line 1530
;1528:
;1529:	// execute scheduled bot AI
;1530:	for( i = 0; i < MAX_CLIENTS; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $656
line 1531
;1531:		if( !botstates[i] || !botstates[i]->inuse ) {
ADDRLP4 136
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 136
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $662
ADDRLP4 136
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $660
LABELV $662
line 1532
;1532:			continue;
ADDRGP4 $657
JUMPV
LABELV $660
line 1535
;1533:		}
;1534:		//
;1535:		botstates[i]->botthink_residual += elapsed_time;
ADDRLP4 140
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 140
INDIRP4
ADDRLP4 140
INDIRP4
INDIRI4
ADDRLP4 120
INDIRI4
ADDI4
ASGNI4
line 1537
;1536:		//
;1537:		if ( botstates[i]->botthink_residual >= thinktime ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 124
INDIRI4
LTI4 $663
line 1538
;1538:			botstates[i]->botthink_residual -= thinktime;
ADDRLP4 144
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 144
INDIRP4
ADDRLP4 144
INDIRP4
INDIRI4
ADDRLP4 124
INDIRI4
SUBI4
ASGNI4
line 1540
;1539:
;1540:			if (!trap_AAS_Initialized()) return qfalse;
ADDRLP4 148
ADDRGP4 trap_AAS_Initialized
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $665
CNSTI4 0
RETI4
ADDRGP4 $571
JUMPV
LABELV $665
line 1542
;1541:
;1542:			if (g_entities[i].client->pers.connected == CON_CONNECTED) {
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $667
line 1543
;1543:				BotAI(i, (float) thinktime / 1000);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 124
INDIRI4
CVIF4 4
CNSTF4 1148846080
DIVF4
ARGF4
ADDRGP4 BotAI
CALLI4
pop
line 1544
;1544:			}
LABELV $667
line 1545
;1545:		}
LABELV $663
line 1546
;1546:	}
LABELV $657
line 1530
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $656
line 1550
;1547:
;1548:
;1549:	// execute bot user commands every frame
;1550:	for( i = 0; i < MAX_CLIENTS; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $670
line 1551
;1551:		if( !botstates[i] || !botstates[i]->inuse ) {
ADDRLP4 136
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 136
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $676
ADDRLP4 136
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $674
LABELV $676
line 1552
;1552:			continue;
ADDRGP4 $671
JUMPV
LABELV $674
line 1554
;1553:		}
;1554:		if( g_entities[i].client->pers.connected != CON_CONNECTED ) {
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $677
line 1555
;1555:			continue;
ADDRGP4 $671
JUMPV
LABELV $677
line 1558
;1556:		}
;1557:
;1558:		BotUpdateInput(botstates[i], time, elapsed_time);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 120
INDIRI4
ARGI4
ADDRGP4 BotUpdateInput
CALLV
pop
line 1559
;1559:		trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
ADDRLP4 140
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 140
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 140
INDIRP4
CNSTI4 488
ADDP4
ARGP4
ADDRGP4 trap_BotUserCommand
CALLV
pop
line 1560
;1560:	}
LABELV $671
line 1550
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $670
line 1562
;1561:
;1562:	return qtrue;
CNSTI4 1
RETI4
LABELV $571
endproc BotAIStartFrame 152 12
export BotInitLibrary
proc BotInitLibrary 212 16
line 1570
;1563:}
;1564:
;1565:/*
;1566:==============
;1567:BotInitLibrary
;1568:==============
;1569:*/
;1570:int BotInitLibrary(void) {
line 1574
;1571:	char buf[144];
;1572:
;1573:	//set the maxclients and maxentities library variables before calling BotSetupLibrary
;1574:	trap_Cvar_VariableStringBuffer("sv_maxclients", buf, sizeof(buf));
ADDRGP4 $681
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1575
;1575:	if (!strlen(buf)) strcpy(buf, "8");
ADDRLP4 0
ARGP4
ADDRLP4 144
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
NEI4 $682
ADDRLP4 0
ARGP4
ADDRGP4 $684
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $682
line 1576
;1576:	trap_BotLibVarSet("maxclients", buf);
ADDRGP4 $685
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1577
;1577:	Com_sprintf(buf, sizeof(buf), "%d", MAX_GENTITIES);
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 $305
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1578
;1578:	trap_BotLibVarSet("maxentities", buf);
ADDRGP4 $686
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1580
;1579:	//bsp checksum
;1580:	trap_Cvar_VariableStringBuffer("sv_mapChecksum", buf, sizeof(buf));
ADDRGP4 $687
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1581
;1581:	if (strlen(buf)) trap_BotLibVarSet("sv_mapChecksum", buf);
ADDRLP4 0
ARGP4
ADDRLP4 148
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $688
ADDRGP4 $687
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $688
line 1583
;1582:	//maximum number of aas links
;1583:	trap_Cvar_VariableStringBuffer("max_aaslinks", buf, sizeof(buf));
ADDRGP4 $690
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1584
;1584:	if (strlen(buf)) trap_BotLibVarSet("max_aaslinks", buf);
ADDRLP4 0
ARGP4
ADDRLP4 152
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $691
ADDRGP4 $690
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $691
line 1586
;1585:	//maximum number of items in a level
;1586:	trap_Cvar_VariableStringBuffer("max_levelitems", buf, sizeof(buf));
ADDRGP4 $693
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1587
;1587:	if (strlen(buf)) trap_BotLibVarSet("max_levelitems", buf);
ADDRLP4 0
ARGP4
ADDRLP4 156
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 0
EQI4 $694
ADDRGP4 $693
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $694
line 1589
;1588:	//game type
;1589:	trap_Cvar_VariableStringBuffer("g_gametype", buf, sizeof(buf));
ADDRGP4 $304
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1590
;1590:	if (!strlen(buf)) strcpy(buf, "0");
ADDRLP4 0
ARGP4
ADDRLP4 160
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
NEI4 $696
ADDRLP4 0
ARGP4
ADDRGP4 $596
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $696
line 1591
;1591:	trap_BotLibVarSet("g_gametype", buf);
ADDRGP4 $304
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1593
;1592:	//bot developer mode and log file
;1593:	trap_BotLibVarSet("bot_developer", bot_developer.string);
ADDRGP4 $698
ARGP4
ADDRGP4 bot_developer+16
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1594
;1594:	trap_BotLibVarSet("log", buf);
ADDRGP4 $700
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1596
;1595:	//no chatting
;1596:	trap_Cvar_VariableStringBuffer("bot_nochat", buf, sizeof(buf));
ADDRGP4 $701
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1597
;1597:	if (strlen(buf)) trap_BotLibVarSet("nochat", "0");
ADDRLP4 0
ARGP4
ADDRLP4 164
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
EQI4 $702
ADDRGP4 $704
ARGP4
ADDRGP4 $596
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $702
line 1599
;1598:	//visualize jump pads
;1599:	trap_Cvar_VariableStringBuffer("bot_visualizejumppads", buf, sizeof(buf));
ADDRGP4 $705
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1600
;1600:	if (strlen(buf)) trap_BotLibVarSet("bot_visualizejumppads", buf);
ADDRLP4 0
ARGP4
ADDRLP4 168
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 0
EQI4 $706
ADDRGP4 $705
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $706
line 1602
;1601:	//forced clustering calculations
;1602:	trap_Cvar_VariableStringBuffer("bot_forceclustering", buf, sizeof(buf));
ADDRGP4 $708
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1603
;1603:	if (strlen(buf)) trap_BotLibVarSet("forceclustering", buf);
ADDRLP4 0
ARGP4
ADDRLP4 172
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 172
INDIRI4
CNSTI4 0
EQI4 $709
ADDRGP4 $711
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $709
line 1605
;1604:	//forced reachability calculations
;1605:	trap_Cvar_VariableStringBuffer("bot_forcereachability", buf, sizeof(buf));
ADDRGP4 $712
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1606
;1606:	if (strlen(buf)) trap_BotLibVarSet("forcereachability", buf);
ADDRLP4 0
ARGP4
ADDRLP4 176
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 0
EQI4 $713
ADDRGP4 $715
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $713
line 1608
;1607:	//force writing of AAS to file
;1608:	trap_Cvar_VariableStringBuffer("bot_forcewrite", buf, sizeof(buf));
ADDRGP4 $716
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1609
;1609:	if (strlen(buf)) trap_BotLibVarSet("forcewrite", buf);
ADDRLP4 0
ARGP4
ADDRLP4 180
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 0
EQI4 $717
ADDRGP4 $719
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $717
line 1611
;1610:	//no AAS optimization
;1611:	trap_Cvar_VariableStringBuffer("bot_aasoptimize", buf, sizeof(buf));
ADDRGP4 $720
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1612
;1612:	if (strlen(buf)) trap_BotLibVarSet("aasoptimize", buf);
ADDRLP4 0
ARGP4
ADDRLP4 184
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 184
INDIRI4
CNSTI4 0
EQI4 $721
ADDRGP4 $723
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $721
line 1614
;1613:	//
;1614:	trap_Cvar_VariableStringBuffer("bot_saveroutingcache", buf, sizeof(buf));
ADDRGP4 $601
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1615
;1615:	if (strlen(buf)) trap_BotLibVarSet("saveroutingcache", buf);
ADDRLP4 0
ARGP4
ADDRLP4 188
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 0
EQI4 $724
ADDRGP4 $600
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $724
line 1617
;1616:	//reload instead of cache bot character files
;1617:	trap_Cvar_VariableStringBuffer("bot_reloadcharacters", buf, sizeof(buf));
ADDRGP4 $312
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1618
;1618:	if (!strlen(buf)) strcpy(buf, "0");
ADDRLP4 0
ARGP4
ADDRLP4 192
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 0
NEI4 $726
ADDRLP4 0
ARGP4
ADDRGP4 $596
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $726
line 1619
;1619:	trap_BotLibVarSet("bot_reloadcharacters", buf);
ADDRGP4 $312
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 1621
;1620:	//base directory
;1621:	trap_Cvar_VariableStringBuffer("fs_basepath", buf, sizeof(buf));
ADDRGP4 $728
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1622
;1622:	if (strlen(buf)) trap_BotLibVarSet("basedir", buf);
ADDRLP4 0
ARGP4
ADDRLP4 196
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 0
EQI4 $729
ADDRGP4 $731
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $729
line 1624
;1623:	//game directory
;1624:	trap_Cvar_VariableStringBuffer("fs_game", buf, sizeof(buf));
ADDRGP4 $732
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1625
;1625:	if (strlen(buf)) trap_BotLibVarSet("gamedir", buf);
ADDRLP4 0
ARGP4
ADDRLP4 200
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 0
EQI4 $733
ADDRGP4 $735
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $733
line 1627
;1626:	//cd directory
;1627:	trap_Cvar_VariableStringBuffer("fs_cdpath", buf, sizeof(buf));
ADDRGP4 $736
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1628
;1628:	if (strlen(buf)) trap_BotLibVarSet("cddir", buf);
ADDRLP4 0
ARGP4
ADDRLP4 204
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 204
INDIRI4
CNSTI4 0
EQI4 $737
ADDRGP4 $739
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
LABELV $737
line 1634
;1629:	//
;1630:#ifdef MISSIONPACK
;1631:	trap_BotLibDefine("MISSIONPACK");
;1632:#endif
;1633:	//setup the bot library
;1634:	return trap_BotLibSetup();
ADDRLP4 208
ADDRGP4 trap_BotLibSetup
CALLI4
ASGNI4
ADDRLP4 208
INDIRI4
RETI4
LABELV $680
endproc BotInitLibrary 212 16
export BotAISetup
proc BotAISetup 8 16
line 1642
;1635:}
;1636:
;1637:/*
;1638:==============
;1639:BotAISetup
;1640:==============
;1641:*/
;1642:int BotAISetup( int restart ) {
line 1645
;1643:	int			errnum;
;1644:
;1645:	trap_Cvar_Register(&bot_thinktime, "bot_thinktime", "100", CVAR_CHEAT);
ADDRGP4 bot_thinktime
ARGP4
ADDRGP4 $605
ARGP4
ADDRGP4 $741
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1646
;1646:	trap_Cvar_Register(&bot_memorydump, "bot_memorydump", "0", CVAR_CHEAT);
ADDRGP4 bot_memorydump
ARGP4
ADDRGP4 $595
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1647
;1647:	trap_Cvar_Register(&bot_saveroutingcache, "bot_saveroutingcache", "0", CVAR_CHEAT);
ADDRGP4 bot_saveroutingcache
ARGP4
ADDRGP4 $601
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1648
;1648:	trap_Cvar_Register(&bot_pause, "bot_pause", "0", CVAR_CHEAT);
ADDRGP4 bot_pause
ARGP4
ADDRGP4 $742
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1649
;1649:	trap_Cvar_Register(&bot_report, "bot_report", "0", CVAR_CHEAT);
ADDRGP4 bot_report
ARGP4
ADDRGP4 $743
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1650
;1650:	trap_Cvar_Register(&bot_testsolid, "bot_testsolid", "0", CVAR_CHEAT);
ADDRGP4 bot_testsolid
ARGP4
ADDRGP4 $744
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1651
;1651:	trap_Cvar_Register(&bot_testclusters, "bot_testclusters", "0", CVAR_CHEAT);
ADDRGP4 bot_testclusters
ARGP4
ADDRGP4 $745
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1652
;1652:	trap_Cvar_Register(&bot_developer, "bot_developer", "0", CVAR_CHEAT);
ADDRGP4 bot_developer
ARGP4
ADDRGP4 $698
ARGP4
ADDRGP4 $596
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1653
;1653:	trap_Cvar_Register(&bot_interbreedchar, "bot_interbreedchar", "", 0);
ADDRGP4 bot_interbreedchar
ARGP4
ADDRGP4 $322
ARGP4
ADDRGP4 $297
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1654
;1654:	trap_Cvar_Register(&bot_interbreedbots, "bot_interbreedbots", "10", 0);
ADDRGP4 bot_interbreedbots
ARGP4
ADDRGP4 $746
ARGP4
ADDRGP4 $747
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1655
;1655:	trap_Cvar_Register(&bot_interbreedcycle, "bot_interbreedcycle", "20", 0);
ADDRGP4 bot_interbreedcycle
ARGP4
ADDRGP4 $748
ARGP4
ADDRGP4 $749
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1656
;1656:	trap_Cvar_Register(&bot_interbreedwrite, "bot_interbreedwrite", "", 0);
ADDRGP4 bot_interbreedwrite
ARGP4
ADDRGP4 $296
ARGP4
ADDRGP4 $297
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1659
;1657:
;1658:	//if the game is restarted for a tournament
;1659:	if (restart) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $750
line 1660
;1660:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $740
JUMPV
LABELV $750
line 1664
;1661:	}
;1662:
;1663:	//initialize the bot states
;1664:	memset( botstates, 0, sizeof(botstates) );
ADDRGP4 botstates
ARGP4
CNSTI4 0
ARGI4
CNSTI4 256
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1666
;1665:
;1666:	errnum = BotInitLibrary();
ADDRLP4 4
ADDRGP4 BotInitLibrary
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1667
;1667:	if (errnum != BLERR_NOERROR) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $752
CNSTI4 0
RETI4
ADDRGP4 $740
JUMPV
LABELV $752
line 1668
;1668:	return qtrue;
CNSTI4 1
RETI4
LABELV $740
endproc BotAISetup 8 16
export BotAIShutdown
proc BotAIShutdown 8 8
line 1676
;1669:}
;1670:
;1671:/*
;1672:==============
;1673:BotAIShutdown
;1674:==============
;1675:*/
;1676:int BotAIShutdown( int restart ) {
line 1681
;1677:
;1678:	int i;
;1679:
;1680:	//if the game is restarted for a tournament
;1681:	if ( restart ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $755
line 1683
;1682:		//shutdown all the bots in the botlib
;1683:		for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $757
line 1684
;1684:			if (botstates[i] && botstates[i]->inuse) {
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $761
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $761
line 1685
;1685:				BotAIShutdownClient(botstates[i]->client, restart);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botstates
ADDP4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 BotAIShutdownClient
CALLI4
pop
line 1686
;1686:			}
LABELV $761
line 1687
;1687:		}
LABELV $758
line 1683
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $757
line 1689
;1688:		//don't shutdown the bot library
;1689:	}
ADDRGP4 $756
JUMPV
LABELV $755
line 1690
;1690:	else {
line 1691
;1691:		trap_BotLibShutdown();
ADDRGP4 trap_BotLibShutdown
CALLI4
pop
line 1692
;1692:	}
LABELV $756
line 1693
;1693:	return qtrue;
CNSTI4 1
RETI4
LABELV $754
endproc BotAIShutdown 8 8
import ExitLevel
bss
export bot_interbreedwrite
align 4
LABELV bot_interbreedwrite
skip 272
export bot_interbreedcycle
align 4
LABELV bot_interbreedcycle
skip 272
export bot_interbreedbots
align 4
LABELV bot_interbreedbots
skip 272
export bot_interbreedchar
align 4
LABELV bot_interbreedchar
skip 272
export bot_developer
align 4
LABELV bot_developer
skip 272
export bot_testclusters
align 4
LABELV bot_testclusters
skip 272
export bot_testsolid
align 4
LABELV bot_testsolid
skip 272
export bot_report
align 4
LABELV bot_report
skip 272
export bot_pause
align 4
LABELV bot_pause
skip 272
export bot_saveroutingcache
align 4
LABELV bot_saveroutingcache
skip 272
export bot_memorydump
align 4
LABELV bot_memorydump
skip 272
export bot_thinktime
align 4
LABELV bot_thinktime
skip 272
export bot_interbreedmatchcount
align 4
LABELV bot_interbreedmatchcount
skip 4
export bot_interbreed
align 4
LABELV bot_interbreed
skip 4
export regularupdate_time
align 4
LABELV regularupdate_time
skip 4
export numbots
align 4
LABELV numbots
skip 4
export botstates
align 4
LABELV botstates
skip 256
import BotVoiceChat_Defend
import BotVoiceChatCommand
import BotDumpNodeSwitches
import BotResetNodeSwitches
import AINode_Battle_NBG
import AINode_Battle_Retreat
import AINode_Battle_Chase
import AINode_Battle_Fight
import AINode_Seek_LTG
import AINode_Seek_NBG
import AINode_Seek_ActivateEntity
import AINode_Stand
import AINode_Respawn
import AINode_Observer
import AINode_Intermission
import AIEnter_Battle_NBG
import AIEnter_Battle_Retreat
import AIEnter_Battle_Chase
import AIEnter_Battle_Fight
import AIEnter_Seek_Camp
import AIEnter_Seek_LTG
import AIEnter_Seek_NBG
import AIEnter_Seek_ActivateEntity
import AIEnter_Stand
import AIEnter_Respawn
import AIEnter_Observer
import AIEnter_Intermission
import BotPrintTeamGoal
import BotMatchMessage
import notleader
import BotChatTest
import BotValidChatPosition
import BotChatTime
import BotChat_Random
import BotChat_EnemySuicide
import BotChat_Kill
import BotChat_Death
import BotChat_HitNoKill
import BotChat_HitNoDeath
import BotChat_HitTalking
import BotChat_EndLevel
import BotChat_StartLevel
import BotChat_ExitGame
import BotChat_EnterGame
import ctf_blueflag
import ctf_redflag
import bot_challenge
import bot_testrchat
import bot_nochat
import bot_fastchat
import bot_rocketjump
import bot_grapple
import maxclients
import gametype
import BotMapScripts
import BotPointAreaNum
import ClientOnSameTeamFromName
import ClientFromName
import stristr
import BotFindWayPoint
import BotCreateWayPoint
import BotAlternateRoute
import BotGetAlternateRouteGoal
import BotCTFRetreatGoals
import BotCTFSeekGoals
import BotRememberLastOrderedTask
import BotCTFCarryingFlag
import BotOppositeTeam
import BotTeam
import BotClearActivateGoalStack
import BotPopFromActivateGoalStack
import BotEnableActivateGoalAreas
import BotAIPredictObstacles
import BotAIBlocked
import BotCheckAttack
import BotAimAtEnemy
import BotEntityVisible
import BotRoamGoal
import BotFindEnemy
import InFieldOfVision
import BotVisibleTeamMatesAndEnemies
import BotEnemyFlagCarrierVisible
import BotTeamFlagCarrierVisible
import BotTeamFlagCarrier
import TeamPlayIsOn
import BotSameTeam
import BotAttackMove
import BotWantsToCamp
import BotHasPersistantPowerupAndWeapon
import BotCanAndWantsToRocketJump
import BotWantsToHelp
import BotWantsToChase
import BotWantsToRetreat
import BotFeelingBad
import BotAggression
import BotTeamGoals
import BotSetLastOrderedTask
import BotSynonymContext
import ClientSkin
import EasyClientName
import ClientName
import BotSetTeamStatus
import BotSetUserInfo
import EntityIsShooting
import EntityIsInvisible
import EntityIsDead
import BotInLavaOrSlime
import BotIntermission
import BotIsObserver
import BotIsDead
import BotBattleUseItems
import BotUpdateBattleInventory
import BotUpdateInventory
import BotSetupForMovement
import BotChooseWeapon
import BotFreeWaypoints
import BotDeathmatchAI
import BotShutdownDeathmatchAI
import BotSetupDeathmatchAI
export floattime
align 4
LABELV floattime
skip 4
import BotResetWeaponState
import BotFreeWeaponState
import BotAllocWeaponState
import BotLoadWeaponWeights
import BotGetWeaponInfo
import BotChooseBestFightWeapon
import BotShutdownWeaponAI
import BotSetupWeaponAI
import BotShutdownMoveAI
import BotSetupMoveAI
import BotSetBrushModelTypes
import BotAddAvoidSpot
import BotInitMoveState
import BotFreeMoveState
import BotAllocMoveState
import BotPredictVisiblePosition
import BotMovementViewTarget
import BotReachabilityArea
import BotResetLastAvoidReach
import BotResetAvoidReach
import BotMoveInDirection
import BotMoveToGoal
import BotResetMoveState
import BotShutdownGoalAI
import BotSetupGoalAI
import BotFreeGoalState
import BotAllocGoalState
import BotFreeItemWeights
import BotLoadItemWeights
import BotMutateGoalFuzzyLogic
import BotSaveGoalFuzzyLogic
import BotInterbreedGoalFuzzyLogic
import BotUpdateEntityItems
import BotInitLevelItems
import BotSetAvoidGoalTime
import BotAvoidGoalTime
import BotGetMapLocationGoal
import BotGetNextCampSpotGoal
import BotGetLevelItemGoal
import BotItemGoalInVisButNotVisible
import BotTouchingGoal
import BotChooseNBGItem
import BotChooseLTGItem
import BotGetSecondGoal
import BotGetTopGoal
import BotGoalName
import BotDumpGoalStack
import BotDumpAvoidGoals
import BotEmptyGoalStack
import BotPopGoal
import BotPushGoal
import BotRemoveFromAvoidGoals
import BotResetAvoidGoals
import BotResetGoalState
import GeneticParentsAndChildSelection
import BotSetChatName
import BotSetChatGender
import BotLoadChatFile
import BotReplaceSynonyms
import UnifyWhiteSpaces
import BotMatchVariable
import BotFindMatch
import StringContains
import BotGetChatMessage
import BotEnterChat
import BotChatLength
import BotReplyChat
import BotNumInitialChats
import BotInitialChat
import BotNumConsoleMessages
import BotNextConsoleMessage
import BotRemoveConsoleMessage
import BotQueueConsoleMessage
import BotFreeChatState
import BotAllocChatState
import BotShutdownChatAI
import BotSetupChatAI
import BotShutdownCharacters
import Characteristic_String
import Characteristic_BInteger
import Characteristic_Integer
import Characteristic_BFloat
import Characteristic_Float
import BotFreeCharacter
import BotLoadCharacter
import EA_Shutdown
import EA_Setup
import EA_ResetInput
import EA_GetInput
import EA_EndRegular
import EA_View
import EA_Move
import EA_DelayedJump
import EA_Jump
import EA_SelectWeapon
import EA_Use
import EA_Gesture
import EA_Talk
import EA_Respawn
import EA_Attack
import EA_MoveRight
import EA_MoveLeft
import EA_MoveBack
import EA_MoveForward
import EA_MoveDown
import EA_MoveUp
import EA_Walk
import EA_Crouch
import EA_Action
import EA_Command
import EA_SayTeam
import EA_Say
import GetBotLibAPI
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
LABELV $749
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $748
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 98
byte 1 114
byte 1 101
byte 1 101
byte 1 100
byte 1 99
byte 1 121
byte 1 99
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $747
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $746
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 98
byte 1 114
byte 1 101
byte 1 101
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $745
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 99
byte 1 108
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $744
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 115
byte 1 111
byte 1 108
byte 1 105
byte 1 100
byte 1 0
align 1
LABELV $743
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 114
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $742
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 112
byte 1 97
byte 1 117
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $741
byte 1 49
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $739
byte 1 99
byte 1 100
byte 1 100
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $736
byte 1 102
byte 1 115
byte 1 95
byte 1 99
byte 1 100
byte 1 112
byte 1 97
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $735
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $732
byte 1 102
byte 1 115
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $731
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 100
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $728
byte 1 102
byte 1 115
byte 1 95
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 112
byte 1 97
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $723
byte 1 97
byte 1 97
byte 1 115
byte 1 111
byte 1 112
byte 1 116
byte 1 105
byte 1 109
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $720
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 97
byte 1 97
byte 1 115
byte 1 111
byte 1 112
byte 1 116
byte 1 105
byte 1 109
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $719
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 119
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $716
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 119
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $715
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 114
byte 1 101
byte 1 97
byte 1 99
byte 1 104
byte 1 97
byte 1 98
byte 1 105
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $712
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 114
byte 1 101
byte 1 97
byte 1 99
byte 1 104
byte 1 97
byte 1 98
byte 1 105
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $711
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 99
byte 1 108
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $708
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 99
byte 1 108
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $705
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 118
byte 1 105
byte 1 115
byte 1 117
byte 1 97
byte 1 108
byte 1 105
byte 1 122
byte 1 101
byte 1 106
byte 1 117
byte 1 109
byte 1 112
byte 1 112
byte 1 97
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $704
byte 1 110
byte 1 111
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $701
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 110
byte 1 111
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $700
byte 1 108
byte 1 111
byte 1 103
byte 1 0
align 1
LABELV $698
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 100
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 111
byte 1 112
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $693
byte 1 109
byte 1 97
byte 1 120
byte 1 95
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 0
align 1
LABELV $690
byte 1 109
byte 1 97
byte 1 120
byte 1 95
byte 1 97
byte 1 97
byte 1 115
byte 1 108
byte 1 105
byte 1 110
byte 1 107
byte 1 115
byte 1 0
align 1
LABELV $687
byte 1 115
byte 1 118
byte 1 95
byte 1 109
byte 1 97
byte 1 112
byte 1 67
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 115
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $686
byte 1 109
byte 1 97
byte 1 120
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $685
byte 1 109
byte 1 97
byte 1 120
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $684
byte 1 56
byte 1 0
align 1
LABELV $681
byte 1 115
byte 1 118
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $606
byte 1 50
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $605
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 116
byte 1 104
byte 1 105
byte 1 110
byte 1 107
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $601
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 114
byte 1 111
byte 1 117
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 99
byte 1 97
byte 1 99
byte 1 104
byte 1 101
byte 1 0
align 1
LABELV $600
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 114
byte 1 111
byte 1 117
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 99
byte 1 97
byte 1 99
byte 1 104
byte 1 101
byte 1 0
align 1
LABELV $596
byte 1 48
byte 1 0
align 1
LABELV $595
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 109
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 100
byte 1 117
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $594
byte 1 109
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 100
byte 1 117
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $563
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $536
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $521
byte 1 99
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 32
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 102
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $518
byte 1 65
byte 1 65
byte 1 83
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 105
byte 1 110
byte 1 105
byte 1 116
byte 1 105
byte 1 97
byte 1 108
byte 1 105
byte 1 122
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $515
byte 1 66
byte 1 111
byte 1 116
byte 1 65
byte 1 73
byte 1 83
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 97
byte 1 108
byte 1 114
byte 1 101
byte 1 97
byte 1 100
byte 1 121
byte 1 32
byte 1 115
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 10
byte 1 0
align 1
LABELV $508
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $507
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
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 102
byte 1 0
align 1
LABELV $488
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 76
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 83
byte 1 104
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $485
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $482
byte 1 116
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $479
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $476
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $473
byte 1 99
byte 1 115
byte 1 0
align 1
LABELV $470
byte 1 99
byte 1 112
byte 1 32
byte 1 0
align 1
LABELV $462
byte 1 66
byte 1 111
byte 1 116
byte 1 65
byte 1 73
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 115
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 10
byte 1 0
align 1
LABELV $322
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 98
byte 1 114
byte 1 101
byte 1 101
byte 1 100
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $319
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 52
byte 1 32
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $313
byte 1 49
byte 1 0
align 1
LABELV $312
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 114
byte 1 101
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $305
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $304
byte 1 103
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $297
byte 1 0
align 1
LABELV $296
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 98
byte 1 114
byte 1 101
byte 1 101
byte 1 100
byte 1 119
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $247
byte 1 108
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 99
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 97
byte 1 92
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $244
byte 1 114
byte 1 111
byte 1 97
byte 1 109
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $242
byte 1 104
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $240
byte 1 97
byte 1 116
byte 1 116
byte 1 97
byte 1 99
byte 1 107
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $238
byte 1 114
byte 1 101
byte 1 116
byte 1 117
byte 1 114
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $236
byte 1 114
byte 1 117
byte 1 115
byte 1 104
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $234
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $232
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $230
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $228
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $226
byte 1 103
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $224
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $222
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $220
byte 1 104
byte 1 101
byte 1 108
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $215
byte 1 70
byte 1 32
byte 1 0
align 1
LABELV $194
byte 1 94
byte 1 52
byte 1 66
byte 1 76
byte 1 85
byte 1 69
byte 1 10
byte 1 0
align 1
LABELV $192
byte 1 116
byte 1 0
align 1
LABELV $188
byte 1 110
byte 1 0
align 1
LABELV $178
byte 1 94
byte 1 49
byte 1 82
byte 1 69
byte 1 68
byte 1 10
byte 1 0
align 1
LABELV $174
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 114
byte 1 111
byte 1 97
byte 1 109
byte 1 105
byte 1 110
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $173
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 104
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $171
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 97
byte 1 116
byte 1 116
byte 1 97
byte 1 99
byte 1 107
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $169
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 117
byte 1 114
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $167
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 114
byte 1 117
byte 1 115
byte 1 104
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $165
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $163
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $161
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $159
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $157
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 103
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $155
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $153
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $151
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 104
byte 1 101
byte 1 108
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $146
byte 1 94
byte 1 52
byte 1 70
byte 1 32
byte 1 0
align 1
LABELV $145
byte 1 94
byte 1 49
byte 1 70
byte 1 32
byte 1 0
align 1
LABELV $138
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $137
byte 1 32
byte 1 0
align 1
LABELV $136
byte 1 76
byte 1 0
align 1
LABELV $131
byte 1 13
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 32
byte 1 37
byte 1 100
byte 1 44
byte 1 32
byte 1 99
byte 1 108
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $130
byte 1 13
byte 1 94
byte 1 49
byte 1 83
byte 1 111
byte 1 108
byte 1 105
byte 1 100
byte 1 33
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $122
byte 1 13
byte 1 94
byte 1 49
byte 1 83
byte 1 79
byte 1 76
byte 1 73
byte 1 68
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 0
align 1
LABELV $121
byte 1 13
byte 1 101
byte 1 109
byte 1 116
byte 1 112
byte 1 121
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 0
align 1
LABELV $67
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $66
byte 1 94
byte 1 49
byte 1 69
byte 1 120
byte 1 105
byte 1 116
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $64
byte 1 94
byte 1 49
byte 1 70
byte 1 97
byte 1 116
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $62
byte 1 94
byte 1 49
byte 1 69
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $60
byte 1 94
byte 1 51
byte 1 87
byte 1 97
byte 1 114
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $58
byte 1 37
byte 1 115
byte 1 0
