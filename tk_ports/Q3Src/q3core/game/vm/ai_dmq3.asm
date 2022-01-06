export BotSetUserInfo
code
proc BotSetUserInfo 1024 12
file "../ai_dmq3.c"
line 117
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
;25: * name:		ai_dmq3.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_dmq3.c $
;30: *
;31: *****************************************************************************/
;32:
;33:
;34:#include "g_local.h"
;35:#include "botlib.h"
;36:#include "be_aas.h"
;37:#include "be_ea.h"
;38:#include "be_ai_char.h"
;39:#include "be_ai_chat.h"
;40:#include "be_ai_gen.h"
;41:#include "be_ai_goal.h"
;42:#include "be_ai_move.h"
;43:#include "be_ai_weap.h"
;44://
;45:#include "ai_main.h"
;46:#include "ai_dmq3.h"
;47:#include "ai_chat.h"
;48:#include "ai_cmd.h"
;49:#include "ai_dmnet.h"
;50:#include "ai_team.h"
;51://
;52:#include "chars.h"				//characteristics
;53:#include "inv.h"				//indexes into the inventory
;54:#include "syn.h"				//synonyms
;55:#include "match.h"				//string matching types and vars
;56:
;57:// for the voice chats
;58:// #include "../../ui/menudef.h" // sos001205 - for q3_ui also
;59:#include "../ui/menudef.h" // sos001205 - for q3_ui also
;60:
;61:// from aasfile.h
;62:#define AREACONTENTS_MOVER				1024
;63:#define AREACONTENTS_MODELNUMSHIFT		24
;64:#define AREACONTENTS_MAXMODELNUM		0xFF
;65:#define AREACONTENTS_MODELNUM			(AREACONTENTS_MAXMODELNUM << AREACONTENTS_MODELNUMSHIFT)
;66:
;67:#define IDEAL_ATTACKDIST			140
;68:
;69:#define MAX_WAYPOINTS		128
;70://
;71:bot_waypoint_t botai_waypoints[MAX_WAYPOINTS];
;72:bot_waypoint_t *botai_freewaypoints;
;73:
;74://NOTE: not using a cvars which can be updated because the game should be reloaded anyway
;75:int gametype;		//game type
;76:int maxclients;		//maximum number of clients
;77:
;78:vmCvar_t bot_grapple;
;79:vmCvar_t bot_rocketjump;
;80:vmCvar_t bot_fastchat;
;81:vmCvar_t bot_nochat;
;82:vmCvar_t bot_testrchat;
;83:vmCvar_t bot_challenge;
;84:vmCvar_t bot_predictobstacles;
;85:vmCvar_t g_spSkill;
;86:
;87:extern vmCvar_t bot_developer;
;88:
;89:vec3_t lastteleport_origin;		//last teleport event origin
;90:float lastteleport_time;		//last teleport event time
;91:int max_bspmodelindex;			//maximum BSP model index
;92:
;93://CTF flag goals
;94:bot_goal_t ctf_redflag;
;95:bot_goal_t ctf_blueflag;
;96:#ifdef MISSIONPACK
;97:bot_goal_t ctf_neutralflag;
;98:bot_goal_t redobelisk;
;99:bot_goal_t blueobelisk;
;100:bot_goal_t neutralobelisk;
;101:#endif
;102:
;103:#define MAX_ALTROUTEGOALS		32
;104:
;105:int altroutegoals_setup;
;106:aas_altroutegoal_t red_altroutegoals[MAX_ALTROUTEGOALS];
;107:int red_numaltroutegoals;
;108:aas_altroutegoal_t blue_altroutegoals[MAX_ALTROUTEGOALS];
;109:int blue_numaltroutegoals;
;110:
;111:
;112:/*
;113:==================
;114:BotSetUserInfo
;115:==================
;116:*/
;117:void BotSetUserInfo(bot_state_t *bs, char *key, char *value) {
line 120
;118:	char userinfo[MAX_INFO_STRING];
;119:
;120:	trap_GetUserinfo(bs->client, userinfo, sizeof(userinfo));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 121
;121:	Info_SetValueForKey(userinfo, key, value);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 122
;122:	trap_SetUserinfo(bs->client, userinfo);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 123
;123:	ClientUserinfoChanged( bs->client );
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 124
;124:}
LABELV $53
endproc BotSetUserInfo 1024 12
export BotCTFCarryingFlag
proc BotCTFCarryingFlag 0 0
line 131
;125:
;126:/*
;127:==================
;128:BotCTFCarryingFlag
;129:==================
;130:*/
;131:int BotCTFCarryingFlag(bot_state_t *bs) {
line 132
;132:	if (gametype != GT_CTF) return CTF_FLAG_NONE;
ADDRGP4 gametype
INDIRI4
CNSTI4 4
EQI4 $55
CNSTI4 0
RETI4
ADDRGP4 $54
JUMPV
LABELV $55
line 134
;133:
;134:	if (bs->inventory[INVENTORY_REDFLAG] > 0) return CTF_FLAG_RED;
ADDRFP4 0
INDIRP4
CNSTI4 5132
ADDP4
INDIRI4
CNSTI4 0
LEI4 $57
CNSTI4 1
RETI4
ADDRGP4 $54
JUMPV
LABELV $57
line 135
;135:	else if (bs->inventory[INVENTORY_BLUEFLAG] > 0) return CTF_FLAG_BLUE;
ADDRFP4 0
INDIRP4
CNSTI4 5136
ADDP4
INDIRI4
CNSTI4 0
LEI4 $59
CNSTI4 2
RETI4
ADDRGP4 $54
JUMPV
LABELV $59
line 136
;136:	return CTF_FLAG_NONE;
CNSTI4 0
RETI4
LABELV $54
endproc BotCTFCarryingFlag 0 0
export BotTeam
proc BotTeam 1044 12
line 144
;137:}
;138:
;139:/*
;140:==================
;141:BotTeam
;142:==================
;143:*/
;144:int BotTeam(bot_state_t *bs) {
line 147
;145:	char info[1024];
;146:
;147:	if (bs->client < 0 || bs->client >= MAX_CLIENTS) {
ADDRLP4 1024
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
LTI4 $64
ADDRLP4 1024
INDIRI4
CNSTI4 64
LTI4 $62
LABELV $64
line 149
;148:		//BotAI_Print(PRT_ERROR, "BotCTFTeam: client out of range\n");
;149:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $61
JUMPV
LABELV $62
line 151
;150:	}
;151:	trap_GetConfigstring(CS_PLAYERS+bs->client, info, sizeof(info));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 153
;152:	//
;153:	if (atoi(Info_ValueForKey(info, "t")) == TEAM_RED) return TEAM_RED;
ADDRLP4 0
ARGP4
ADDRGP4 $67
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 1032
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 1
NEI4 $65
CNSTI4 1
RETI4
ADDRGP4 $61
JUMPV
LABELV $65
line 154
;154:	else if (atoi(Info_ValueForKey(info, "t")) == TEAM_BLUE) return TEAM_BLUE;
ADDRLP4 0
ARGP4
ADDRGP4 $67
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 2
NEI4 $68
CNSTI4 2
RETI4
ADDRGP4 $61
JUMPV
LABELV $68
line 155
;155:	return TEAM_FREE;
CNSTI4 0
RETI4
LABELV $61
endproc BotTeam 1044 12
export BotOppositeTeam
proc BotOppositeTeam 12 4
line 163
;156:}
;157:
;158:/*
;159:==================
;160:BotOppositeTeam
;161:==================
;162:*/
;163:int BotOppositeTeam(bot_state_t *bs) {
line 164
;164:	switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
EQI4 $74
ADDRLP4 0
INDIRI4
CNSTI4 2
EQI4 $75
ADDRGP4 $71
JUMPV
LABELV $74
line 165
;165:		case TEAM_RED: return TEAM_BLUE;
CNSTI4 2
RETI4
ADDRGP4 $70
JUMPV
LABELV $75
line 166
;166:		case TEAM_BLUE: return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $70
JUMPV
LABELV $71
line 167
;167:		default: return TEAM_FREE;
CNSTI4 0
RETI4
LABELV $70
endproc BotOppositeTeam 12 4
export BotEnemyFlag
proc BotEnemyFlag 4 4
line 176
;168:	}
;169:}
;170:
;171:/*
;172:==================
;173:BotEnemyFlag
;174:==================
;175:*/
;176:bot_goal_t *BotEnemyFlag(bot_state_t *bs) {
line 177
;177:	if (BotTeam(bs) == TEAM_RED) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $77
line 178
;178:		return &ctf_blueflag;
ADDRGP4 ctf_blueflag
RETP4
ADDRGP4 $76
JUMPV
LABELV $77
line 180
;179:	}
;180:	else {
line 181
;181:		return &ctf_redflag;
ADDRGP4 ctf_redflag
RETP4
LABELV $76
endproc BotEnemyFlag 4 4
export BotTeamFlag
proc BotTeamFlag 4 4
line 190
;182:	}
;183:}
;184:
;185:/*
;186:==================
;187:BotTeamFlag
;188:==================
;189:*/
;190:bot_goal_t *BotTeamFlag(bot_state_t *bs) {
line 191
;191:	if (BotTeam(bs) == TEAM_RED) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $80
line 192
;192:		return &ctf_redflag;
ADDRGP4 ctf_redflag
RETP4
ADDRGP4 $79
JUMPV
LABELV $80
line 194
;193:	}
;194:	else {
line 195
;195:		return &ctf_blueflag;
ADDRGP4 ctf_blueflag
RETP4
LABELV $79
endproc BotTeamFlag 4 4
export EntityIsDead
proc EntityIsDead 472 8
line 205
;196:	}
;197:}
;198:
;199:
;200:/*
;201:==================
;202:EntityIsDead
;203:==================
;204:*/
;205:qboolean EntityIsDead(aas_entityinfo_t *entinfo) {
line 208
;206:	playerState_t ps;
;207:
;208:	if (entinfo->number >= 0 && entinfo->number < MAX_CLIENTS) {
ADDRLP4 468
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 468
INDIRI4
CNSTI4 0
LTI4 $83
ADDRLP4 468
INDIRI4
CNSTI4 64
GEI4 $83
line 210
;209:		//retrieve the current client state
;210:		BotAI_GetClientState( entinfo->number, &ps );
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 211
;211:		if (ps.pm_type != PM_NORMAL) return qtrue;
ADDRLP4 0+4
INDIRI4
CNSTI4 0
EQI4 $85
CNSTI4 1
RETI4
ADDRGP4 $82
JUMPV
LABELV $85
line 212
;212:	}
LABELV $83
line 213
;213:	return qfalse;
CNSTI4 0
RETI4
LABELV $82
endproc EntityIsDead 472 8
export EntityCarriesFlag
proc EntityCarriesFlag 0 0
line 221
;214:}
;215:
;216:/*
;217:==================
;218:EntityCarriesFlag
;219:==================
;220:*/
;221:qboolean EntityCarriesFlag(aas_entityinfo_t *entinfo) {
line 222
;222:	if ( entinfo->powerups & ( 1 << PW_REDFLAG ) )
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $89
line 223
;223:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $88
JUMPV
LABELV $89
line 224
;224:	if ( entinfo->powerups & ( 1 << PW_BLUEFLAG ) )
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $91
line 225
;225:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $88
JUMPV
LABELV $91
line 230
;226:#ifdef MISSIONPACK
;227:	if ( entinfo->powerups & ( 1 << PW_NEUTRALFLAG ) )
;228:		return qtrue;
;229:#endif
;230:	return qfalse;
CNSTI4 0
RETI4
LABELV $88
endproc EntityCarriesFlag 0 0
export EntityIsInvisible
proc EntityIsInvisible 4 4
line 238
;231:}
;232:
;233:/*
;234:==================
;235:EntityIsInvisible
;236:==================
;237:*/
;238:qboolean EntityIsInvisible(aas_entityinfo_t *entinfo) {
line 240
;239:	// the flag is always visible
;240:	if (EntityCarriesFlag(entinfo)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $94
line 241
;241:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $93
JUMPV
LABELV $94
line 243
;242:	}
;243:	if (entinfo->powerups & (1 << PW_INVIS)) {
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $96
line 244
;244:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $93
JUMPV
LABELV $96
line 246
;245:	}
;246:	return qfalse;
CNSTI4 0
RETI4
LABELV $93
endproc EntityIsInvisible 4 4
export EntityIsShooting
proc EntityIsShooting 0 0
line 254
;247:}
;248:
;249:/*
;250:==================
;251:EntityIsShooting
;252:==================
;253:*/
;254:qboolean EntityIsShooting(aas_entityinfo_t *entinfo) {
line 255
;255:	if (entinfo->flags & EF_FIRING) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $99
line 256
;256:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $98
JUMPV
LABELV $99
line 258
;257:	}
;258:	return qfalse;
CNSTI4 0
RETI4
LABELV $98
endproc EntityIsShooting 0 0
export EntityIsChatting
proc EntityIsChatting 0 0
line 266
;259:}
;260:
;261:/*
;262:==================
;263:EntityIsChatting
;264:==================
;265:*/
;266:qboolean EntityIsChatting(aas_entityinfo_t *entinfo) {
line 267
;267:	if (entinfo->flags & EF_TALK) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $102
line 268
;268:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $101
JUMPV
LABELV $102
line 270
;269:	}
;270:	return qfalse;
CNSTI4 0
RETI4
LABELV $101
endproc EntityIsChatting 0 0
export EntityHasQuad
proc EntityHasQuad 0 0
line 278
;271:}
;272:
;273:/*
;274:==================
;275:EntityHasQuad
;276:==================
;277:*/
;278:qboolean EntityHasQuad(aas_entityinfo_t *entinfo) {
line 279
;279:	if (entinfo->powerups & (1 << PW_QUAD)) {
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $105
line 280
;280:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $104
JUMPV
LABELV $105
line 282
;281:	}
;282:	return qfalse;
CNSTI4 0
RETI4
LABELV $104
endproc EntityHasQuad 0 0
export BotRememberLastOrderedTask
proc BotRememberLastOrderedTask 16 12
line 346
;283:}
;284:
;285:#ifdef MISSIONPACK
;286:/*
;287:==================
;288:EntityHasKamikze
;289:==================
;290:*/
;291:qboolean EntityHasKamikaze(aas_entityinfo_t *entinfo) {
;292:	if (entinfo->flags & EF_KAMIKAZE) {
;293:		return qtrue;
;294:	}
;295:	return qfalse;
;296:}
;297:
;298:/*
;299:==================
;300:EntityCarriesCubes
;301:==================
;302:*/
;303:qboolean EntityCarriesCubes(aas_entityinfo_t *entinfo) {
;304:	entityState_t state;
;305:
;306:	if (gametype != GT_HARVESTER)
;307:		return qfalse;
;308:	//FIXME: get this info from the aas_entityinfo_t ?
;309:	BotAI_GetEntityState(entinfo->number, &state);
;310:	if (state.generic1 > 0)
;311:		return qtrue;
;312:	return qfalse;
;313:}
;314:
;315:/*
;316:==================
;317:Bot1FCTFCarryingFlag
;318:==================
;319:*/
;320:int Bot1FCTFCarryingFlag(bot_state_t *bs) {
;321:	if (gametype != GT_1FCTF) return qfalse;
;322:
;323:	if (bs->inventory[INVENTORY_NEUTRALFLAG] > 0) return qtrue;
;324:	return qfalse;
;325:}
;326:
;327:/*
;328:==================
;329:BotHarvesterCarryingCubes
;330:==================
;331:*/
;332:int BotHarvesterCarryingCubes(bot_state_t *bs) {
;333:	if (gametype != GT_HARVESTER) return qfalse;
;334:
;335:	if (bs->inventory[INVENTORY_REDCUBE] > 0) return qtrue;
;336:	if (bs->inventory[INVENTORY_BLUECUBE] > 0) return qtrue;
;337:	return qfalse;
;338:}
;339:#endif
;340:
;341:/*
;342:==================
;343:BotRememberLastOrderedTask
;344:==================
;345:*/
;346:void BotRememberLastOrderedTask(bot_state_t *bs) {
line 347
;347:	if (!bs->ordered) {
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
INDIRI4
CNSTI4 0
NEI4 $108
line 348
;348:		return;
ADDRGP4 $107
JUMPV
LABELV $108
line 350
;349:	}
;350:	bs->lastgoal_decisionmaker = bs->decisionmaker;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 6756
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ASGNI4
line 351
;351:	bs->lastgoal_ltgtype = bs->ltgtype;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6760
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
line 352
;352:	memcpy(&bs->lastgoal_teamgoal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 6768
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 353
;353:	bs->lastgoal_teammate = bs->teammate;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 6764
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ASGNI4
line 354
;354:}
LABELV $107
endproc BotRememberLastOrderedTask 16 12
export BotSetTeamStatus
proc BotSetTeamStatus 0 0
line 361
;355:
;356:/*
;357:==================
;358:BotSetTeamStatus
;359:==================
;360:*/
;361:void BotSetTeamStatus(bot_state_t *bs) {
line 418
;362:#ifdef MISSIONPACK
;363:	int teamtask;
;364:	aas_entityinfo_t entinfo;
;365:
;366:	teamtask = TEAMTASK_PATROL;
;367:
;368:	switch(bs->ltgtype) {
;369:		case LTG_TEAMHELP:
;370:			break;
;371:		case LTG_TEAMACCOMPANY:
;372:			BotEntityInfo(bs->teammate, &entinfo);
;373:			if ( ( (gametype == GT_CTF || gametype == GT_1FCTF) && EntityCarriesFlag(&entinfo))
;374:				|| ( gametype == GT_HARVESTER && EntityCarriesCubes(&entinfo)) ) {
;375:				teamtask = TEAMTASK_ESCORT;
;376:			}
;377:			else {
;378:				teamtask = TEAMTASK_FOLLOW;
;379:			}
;380:			break;
;381:		case LTG_DEFENDKEYAREA:
;382:			teamtask = TEAMTASK_DEFENSE;
;383:			break;
;384:		case LTG_GETFLAG:
;385:			teamtask = TEAMTASK_OFFENSE;
;386:			break;
;387:		case LTG_RUSHBASE:
;388:			teamtask = TEAMTASK_DEFENSE;
;389:			break;
;390:		case LTG_RETURNFLAG:
;391:			teamtask = TEAMTASK_RETRIEVE;
;392:			break;
;393:		case LTG_CAMP:
;394:		case LTG_CAMPORDER:
;395:			teamtask = TEAMTASK_CAMP;
;396:			break;
;397:		case LTG_PATROL:
;398:			teamtask = TEAMTASK_PATROL;
;399:			break;
;400:		case LTG_GETITEM:
;401:			teamtask = TEAMTASK_PATROL;
;402:			break;
;403:		case LTG_KILL:
;404:			teamtask = TEAMTASK_PATROL;
;405:			break;
;406:		case LTG_HARVEST:
;407:			teamtask = TEAMTASK_OFFENSE;
;408:			break;
;409:		case LTG_ATTACKENEMYBASE:
;410:			teamtask = TEAMTASK_OFFENSE;
;411:			break;
;412:		default:
;413:			teamtask = TEAMTASK_PATROL;
;414:			break;
;415:	}
;416:	BotSetUserInfo(bs, "teamtask", va("%d", teamtask));
;417:#endif
;418:}
LABELV $110
endproc BotSetTeamStatus 0 0
export BotSetLastOrderedTask
proc BotSetLastOrderedTask 60 16
line 425
;419:
;420:/*
;421:==================
;422:BotSetLastOrderedTask
;423:==================
;424:*/
;425:int BotSetLastOrderedTask(bot_state_t *bs) {
line 427
;426:
;427:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $112
line 429
;428:		// don't go back to returning the flag if it's at the base
;429:		if ( bs->lastgoal_ltgtype == LTG_RETURNFLAG ) {
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
CNSTI4 6
NEI4 $114
line 430
;430:			if ( BotTeam(bs) == TEAM_RED ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $116
line 431
;431:				if ( bs->redflagstatus == 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
CNSTI4 0
NEI4 $117
line 432
;432:					bs->lastgoal_ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
CNSTI4 0
ASGNI4
line 433
;433:				}
line 434
;434:			}
ADDRGP4 $117
JUMPV
LABELV $116
line 435
;435:			else {
line 436
;436:				if ( bs->blueflagstatus == 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
CNSTI4 0
NEI4 $120
line 437
;437:					bs->lastgoal_ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
CNSTI4 0
ASGNI4
line 438
;438:				}
LABELV $120
line 439
;439:			}
LABELV $117
line 440
;440:		}
LABELV $114
line 441
;441:	}
LABELV $112
line 443
;442:
;443:	if ( bs->lastgoal_ltgtype ) {
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
CNSTI4 0
EQI4 $122
line 444
;444:		bs->decisionmaker = bs->lastgoal_decisionmaker;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 6756
ADDP4
INDIRI4
ASGNI4
line 445
;445:		bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 446
;446:		bs->ltgtype = bs->lastgoal_ltgtype;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6600
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
ASGNI4
line 447
;447:		memcpy(&bs->teamgoal, &bs->lastgoal_teamgoal, sizeof(bot_goal_t));
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 6768
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 448
;448:		bs->teammate = bs->lastgoal_teammate;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 6604
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 6764
ADDP4
INDIRI4
ASGNI4
line 449
;449:		bs->teamgoal_time = FloatTime() + 300;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1133903872
ADDF4
ASGNF4
line 450
;450:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 452
;451:		//
;452:		if ( gametype == GT_CTF ) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $124
line 453
;453:			if ( bs->ltgtype == LTG_GETFLAG ) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 4
NEI4 $126
line 457
;454:				bot_goal_t *tb, *eb;
;455:				int tt, et;
;456:
;457:				tb = BotTeamFlag(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 BotTeamFlag
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 32
INDIRP4
ASGNP4
line 458
;458:				eb = BotEnemyFlag(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 BotEnemyFlag
CALLP4
ASGNP4
ADDRLP4 20
ADDRLP4 36
INDIRP4
ASGNP4
line 459
;459:				tt = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, tb->areanum, TFL_DEFAULT);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 40
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 44
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 44
INDIRI4
ASGNI4
line 460
;460:				et = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, eb->areanum, TFL_DEFAULT);
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 48
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 52
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 28
ADDRLP4 52
INDIRI4
ASGNI4
line 462
;461:				// if the travel time towards the enemy base is larger than towards our base
;462:				if (et > tt) {
ADDRLP4 28
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $128
line 464
;463:					//get an alternative route goal towards the enemy base
;464:					BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 465
;465:				}
LABELV $128
line 466
;466:			}
LABELV $126
line 467
;467:		}
LABELV $124
line 468
;468:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $111
JUMPV
LABELV $122
line 470
;469:	}
;470:	return qfalse;
CNSTI4 0
RETI4
LABELV $111
endproc BotSetLastOrderedTask 60 16
export BotRefuseOrder
proc BotRefuseOrder 8 12
line 478
;471:}
;472:
;473:/*
;474:==================
;475:BotRefuseOrder
;476:==================
;477:*/
;478:void BotRefuseOrder(bot_state_t *bs) {
line 479
;479:	if (!bs->ordered)
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
INDIRI4
CNSTI4 0
NEI4 $131
line 480
;480:		return;
ADDRGP4 $130
JUMPV
LABELV $131
line 482
;481:	// if the bot was ordered to do something
;482:	if ( bs->order_time && bs->order_time > FloatTime() - 10 ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 0
EQF4 $133
ADDRLP4 0
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
SUBF4
LEF4 $133
line 483
;483:		trap_EA_Action(bs->client, ACTION_NEGATIVE);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 2097152
ARGI4
ADDRGP4 trap_EA_Action
CALLV
pop
line 484
;484:		BotVoiceChat(bs, bs->decisionmaker, VOICECHAT_NO);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $135
ARGP4
ADDRGP4 BotVoiceChat
CALLV
pop
line 485
;485:		bs->order_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
CNSTF4 0
ASGNF4
line 486
;486:	}
LABELV $133
line 487
;487:}
LABELV $130
endproc BotRefuseOrder 8 12
export BotCTFSeekGoals
proc BotCTFSeekGoals 240 12
line 494
;488:
;489:/*
;490:==================
;491:BotCTFSeekGoals
;492:==================
;493:*/
;494:void BotCTFSeekGoals(bot_state_t *bs) {
line 501
;495:	float rnd, l1, l2;
;496:	int flagstatus, c;
;497:	vec3_t dir;
;498:	aas_entityinfo_t entinfo;
;499:
;500:	//when carrying a flag in ctf the bot should rush to the base
;501:	if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 172
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 172
INDIRI4
CNSTI4 0
EQI4 $137
line 503
;502:		//if not already rushing to the base
;503:		if (bs->ltgtype != LTG_RUSHBASE) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 5
EQI4 $139
line 504
;504:			BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 505
;505:			bs->ltgtype = LTG_RUSHBASE;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 5
ASGNI4
line 506
;506:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1123024896
ADDF4
ASGNF4
line 507
;507:			bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 508
;508:			bs->decisionmaker = bs->client;
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 176
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 509
;509:			bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 511
;510:			//
;511:			switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 184
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 180
ADDRLP4 184
INDIRI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 1
EQI4 $144
ADDRLP4 180
INDIRI4
CNSTI4 2
EQI4 $149
ADDRGP4 $141
JUMPV
LABELV $144
line 512
;512:				case TEAM_RED: VectorSubtract(bs->origin, ctf_blueflag.origin, dir); break;
ADDRLP4 192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 160
ADDRLP4 192
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRGP4 ctf_blueflag
INDIRF4
SUBF4
ASGNF4
ADDRLP4 160+4
ADDRLP4 192
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRGP4 ctf_blueflag+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 160+8
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRGP4 ctf_blueflag+8
INDIRF4
SUBF4
ASGNF4
ADDRGP4 $142
JUMPV
LABELV $149
line 513
;513:				case TEAM_BLUE: VectorSubtract(bs->origin, ctf_redflag.origin, dir); break;
ADDRLP4 196
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 160
ADDRLP4 196
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRGP4 ctf_redflag
INDIRF4
SUBF4
ASGNF4
ADDRLP4 160+4
ADDRLP4 196
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRGP4 ctf_redflag+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 160+8
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRGP4 ctf_redflag+8
INDIRF4
SUBF4
ASGNF4
ADDRGP4 $142
JUMPV
LABELV $141
line 514
;514:				default: VectorSet(dir, 999, 999, 999); break;
ADDRLP4 200
CNSTF4 1148829696
ASGNF4
ADDRLP4 160
ADDRLP4 200
INDIRF4
ASGNF4
ADDRLP4 160+4
ADDRLP4 200
INDIRF4
ASGNF4
ADDRLP4 160+8
CNSTF4 1148829696
ASGNF4
LABELV $142
line 517
;515:			}
;516:			// if the bot picked up the flag very close to the enemy base
;517:			if ( VectorLength(dir) < 128 ) {
ADDRLP4 160
ARGP4
ADDRLP4 192
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 192
INDIRF4
CNSTF4 1124073472
GEF4 $156
line 519
;518:				// get an alternative route goal through the enemy base
;519:				BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 196
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 196
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 520
;520:			} else {
ADDRGP4 $157
JUMPV
LABELV $156
line 522
;521:				// don't use any alt route goal, just get the hell out of the base
;522:				bs->altroutegoal.areanum = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6692
ADDP4
CNSTI4 0
ASGNI4
line 523
;523:			}
LABELV $157
line 524
;524:			BotSetUserInfo(bs, "teamtask", va("%d", TEAMTASK_OFFENSE));
ADDRGP4 $159
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 196
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $158
ARGP4
ADDRLP4 196
INDIRP4
ARGP4
ADDRGP4 BotSetUserInfo
CALLV
pop
line 525
;525:			BotVoiceChat(bs, -1, VOICECHAT_IHAVEFLAG);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $160
ARGP4
ADDRGP4 BotVoiceChat
CALLV
pop
line 526
;526:		}
ADDRGP4 $136
JUMPV
LABELV $139
line 527
;527:		else if (bs->rushbaseaway_time > FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $136
line 528
;528:			if (BotTeam(bs) == TEAM_RED) flagstatus = bs->redflagstatus;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 176
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 1
NEI4 $163
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $164
JUMPV
LABELV $163
line 529
;529:			else flagstatus = bs->blueflagstatus;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
ASGNI4
LABELV $164
line 531
;530:			//if the flag is back
;531:			if (flagstatus == 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $136
line 532
;532:				bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 533
;533:			}
line 534
;534:		}
line 535
;535:		return;
ADDRGP4 $136
JUMPV
LABELV $137
line 538
;536:	}
;537:	// if the bot decided to follow someone
;538:	if ( bs->ltgtype == LTG_TEAMACCOMPANY && !bs->ordered ) {
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 2
NEI4 $167
ADDRLP4 176
INDIRP4
CNSTI4 6612
ADDP4
INDIRI4
CNSTI4 0
NEI4 $167
line 540
;539:		// if the team mate being accompanied no longer carries the flag
;540:		BotEntityInfo(bs->teammate, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 541
;541:		if (!EntityCarriesFlag(&entinfo)) {
ADDRLP4 16
ARGP4
ADDRLP4 180
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 0
NEI4 $169
line 542
;542:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 543
;543:		}
LABELV $169
line 544
;544:	}
LABELV $167
line 546
;545:	//
;546:	if (BotTeam(bs) == TEAM_RED) flagstatus = bs->redflagstatus * 2 + bs->blueflagstatus;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 180
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 1
NEI4 $171
ADDRLP4 184
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 184
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 184
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
ADDI4
ASGNI4
ADDRGP4 $172
JUMPV
LABELV $171
line 547
;547:	else flagstatus = bs->blueflagstatus * 2 + bs->redflagstatus;
ADDRLP4 188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 188
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 188
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
ADDI4
ASGNI4
LABELV $172
line 549
;548:	//if our team has the enemy flag and our flag is at the base
;549:	if (flagstatus == 1) {
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $173
line 551
;550:		//
;551:		if (bs->owndecision_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 floattime
INDIRF4
GEF4 $136
line 553
;552:			//if Not defending the base already
;553:			if (!(bs->ltgtype == LTG_DEFENDKEYAREA &&
ADDRLP4 192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 192
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 3
NEI4 $181
ADDRLP4 196
ADDRLP4 192
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ASGNI4
ADDRLP4 196
INDIRI4
ADDRGP4 ctf_redflag+44
INDIRI4
EQI4 $136
ADDRLP4 196
INDIRI4
ADDRGP4 ctf_blueflag+44
INDIRI4
EQI4 $136
LABELV $181
line 555
;554:					(bs->teamgoal.number == ctf_redflag.number ||
;555:					bs->teamgoal.number == ctf_blueflag.number))) {
line 557
;556:				//if there is a visible team mate flag carrier
;557:				c = BotTeamFlagCarrierVisible(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 200
ADDRGP4 BotTeamFlagCarrierVisible
CALLI4
ASGNI4
ADDRLP4 156
ADDRLP4 200
INDIRI4
ASGNI4
line 558
;558:				if (c >= 0 &&
ADDRLP4 204
ADDRLP4 156
INDIRI4
ASGNI4
ADDRLP4 204
INDIRI4
CNSTI4 0
LTI4 $136
ADDRLP4 208
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 208
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 2
NEI4 $184
ADDRLP4 208
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ADDRLP4 204
INDIRI4
EQI4 $136
LABELV $184
line 560
;559:						// and not already following the team mate flag carrier
;560:						(bs->ltgtype != LTG_TEAMACCOMPANY || bs->teammate != c)) {
line 562
;561:					//
;562:					BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 564
;563:					//follow the flag carrier
;564:					bs->decisionmaker = bs->client;
ADDRLP4 212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 212
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 212
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 565
;565:					bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 567
;566:					//the team mate
;567:					bs->teammate = c;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
ADDRLP4 156
INDIRI4
ASGNI4
line 569
;568:					//last time the team mate was visible
;569:					bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 571
;570:					//no message
;571:					bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 573
;572:					//no arrive message
;573:					bs->arrive_time = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 1065353216
ASGNF4
line 575
;574:					//
;575:					BotVoiceChat(bs, bs->teammate, VOICECHAT_ONFOLLOW);
ADDRLP4 216
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 216
INDIRP4
ARGP4
ADDRLP4 216
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRGP4 $185
ARGP4
ADDRGP4 BotVoiceChat
CALLV
pop
line 577
;576:					//get the team goal time
;577:					bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 578
;578:					bs->ltgtype = LTG_TEAMACCOMPANY;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 2
ASGNI4
line 579
;579:					bs->formation_dist = 3.5 * 32;		//3.5 meter
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
CNSTF4 1121976320
ASGNF4
line 580
;580:					BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 581
;581:					bs->owndecision_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CVFI4 4
ASGNI4
line 582
;582:				}
line 583
;583:			}
line 584
;584:		}
line 585
;585:		return;
ADDRGP4 $136
JUMPV
LABELV $173
line 588
;586:	}
;587:	//if the enemy has our flag
;588:	else if (flagstatus == 2) {
ADDRLP4 0
INDIRI4
CNSTI4 2
NEI4 $186
line 590
;589:		//
;590:		if (bs->owndecision_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 floattime
INDIRF4
GEF4 $136
line 592
;591:			//if enemy flag carrier is visible
;592:			c = BotEnemyFlagCarrierVisible(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 192
ADDRGP4 BotEnemyFlagCarrierVisible
CALLI4
ASGNI4
ADDRLP4 156
ADDRLP4 192
INDIRI4
ASGNI4
line 593
;593:			if (c >= 0) {
ADDRLP4 156
INDIRI4
CNSTI4 0
LTI4 $190
line 595
;594:				//FIXME: fight enemy flag carrier
;595:			}
LABELV $190
line 597
;596:			//if not already doing something important
;597:			if (bs->ltgtype != LTG_GETFLAG &&
ADDRLP4 196
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 4
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 6
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 1
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 2
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 8
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 9
EQI4 $136
ADDRLP4 196
INDIRI4
CNSTI4 10
EQI4 $136
line 603
;598:				bs->ltgtype != LTG_RETURNFLAG &&
;599:				bs->ltgtype != LTG_TEAMHELP &&
;600:				bs->ltgtype != LTG_TEAMACCOMPANY &&
;601:				bs->ltgtype != LTG_CAMPORDER &&
;602:				bs->ltgtype != LTG_PATROL &&
;603:				bs->ltgtype != LTG_GETITEM) {
line 605
;604:
;605:				BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 606
;606:				bs->decisionmaker = bs->client;
ADDRLP4 200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 200
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 200
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 607
;607:				bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 609
;608:				//
;609:				if (random() < 0.5) {
ADDRLP4 204
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 204
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
GEF4 $194
line 611
;610:					//go for the enemy flag
;611:					bs->ltgtype = LTG_GETFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 4
ASGNI4
line 612
;612:				}
ADDRGP4 $195
JUMPV
LABELV $194
line 613
;613:				else {
line 614
;614:					bs->ltgtype = LTG_RETURNFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 6
ASGNI4
line 615
;615:				}
LABELV $195
line 617
;616:				//no team message
;617:				bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 619
;618:				//set the time the bot will stop getting the flag
;619:				bs->teamgoal_time = FloatTime() + CTF_GETFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 621
;620:				//get an alternative route goal towards the enemy base
;621:				BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 623
;622:				//
;623:				BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 624
;624:				bs->owndecision_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CVFI4 4
ASGNI4
line 625
;625:			}
line 626
;626:		}
line 627
;627:		return;
ADDRGP4 $136
JUMPV
LABELV $186
line 630
;628:	}
;629:	//if both flags Not at their bases
;630:	else if (flagstatus == 3) {
ADDRLP4 0
INDIRI4
CNSTI4 3
NEI4 $196
line 632
;631:		//
;632:		if (bs->owndecision_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 floattime
INDIRF4
GEF4 $136
line 634
;633:			// if not trying to return the flag and not following the team flag carrier
;634:			if ( bs->ltgtype != LTG_RETURNFLAG && bs->ltgtype != LTG_TEAMACCOMPANY ) {
ADDRLP4 192
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 6
EQI4 $136
ADDRLP4 192
INDIRI4
CNSTI4 2
EQI4 $136
line 636
;635:				//
;636:				c = BotTeamFlagCarrierVisible(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 196
ADDRGP4 BotTeamFlagCarrierVisible
CALLI4
ASGNI4
ADDRLP4 156
ADDRLP4 196
INDIRI4
ASGNI4
line 638
;637:				// if there is a visible team mate flag carrier
;638:				if (c >= 0) {
ADDRLP4 156
INDIRI4
CNSTI4 0
LTI4 $202
line 639
;639:					BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 641
;640:					//follow the flag carrier
;641:					bs->decisionmaker = bs->client;
ADDRLP4 200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 200
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 200
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 642
;642:					bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 644
;643:					//the team mate
;644:					bs->teammate = c;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
ADDRLP4 156
INDIRI4
ASGNI4
line 646
;645:					//last time the team mate was visible
;646:					bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 648
;647:					//no message
;648:					bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 650
;649:					//no arrive message
;650:					bs->arrive_time = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 1065353216
ASGNF4
line 652
;651:					//
;652:					BotVoiceChat(bs, bs->teammate, VOICECHAT_ONFOLLOW);
ADDRLP4 204
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 204
INDIRP4
ARGP4
ADDRLP4 204
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRGP4 $185
ARGP4
ADDRGP4 BotVoiceChat
CALLV
pop
line 654
;653:					//get the team goal time
;654:					bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 655
;655:					bs->ltgtype = LTG_TEAMACCOMPANY;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 2
ASGNI4
line 656
;656:					bs->formation_dist = 3.5 * 32;		//3.5 meter
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
CNSTF4 1121976320
ASGNF4
line 658
;657:					//
;658:					BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 659
;659:					bs->owndecision_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CVFI4 4
ASGNI4
line 660
;660:				}
ADDRGP4 $136
JUMPV
LABELV $202
line 661
;661:				else {
line 662
;662:					BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 663
;663:					bs->decisionmaker = bs->client;
ADDRLP4 200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 200
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 200
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 664
;664:					bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 666
;665:					//get the enemy flag
;666:					bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 204
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
ADDRLP4 204
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 668
;667:					//get the flag
;668:					bs->ltgtype = LTG_RETURNFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 6
ASGNI4
line 670
;669:					//set the time the bot will stop getting the flag
;670:					bs->teamgoal_time = FloatTime() + CTF_RETURNFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1127481344
ADDF4
ASGNF4
line 672
;671:					//get an alternative route goal towards the enemy base
;672:					BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 674
;673:					//
;674:					BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 675
;675:					bs->owndecision_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CVFI4 4
ASGNI4
line 676
;676:				}
line 677
;677:			}
line 678
;678:		}
line 679
;679:		return;
ADDRGP4 $136
JUMPV
LABELV $196
line 682
;680:	}
;681:	// don't just do something wait for the bot team leader to give orders
;682:	if (BotTeamLeader(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 192
ADDRGP4 BotTeamLeader
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 0
EQI4 $204
line 683
;683:		return;
ADDRGP4 $136
JUMPV
LABELV $204
line 686
;684:	}
;685:	// if the bot is ordered to do something
;686:	if ( bs->lastgoal_ltgtype ) {
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
CNSTI4 0
EQI4 $206
line 687
;687:		bs->teamgoal_time += 60;
ADDRLP4 196
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ASGNP4
ADDRLP4 196
INDIRP4
ADDRLP4 196
INDIRP4
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 688
;688:	}
LABELV $206
line 690
;689:	// if the bot decided to do something on it's own and has a last ordered goal
;690:	if ( !bs->ordered && bs->lastgoal_ltgtype ) {
ADDRLP4 196
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 200
CNSTI4 0
ASGNI4
ADDRLP4 196
INDIRP4
CNSTI4 6612
ADDP4
INDIRI4
ADDRLP4 200
INDIRI4
NEI4 $208
ADDRLP4 196
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
ADDRLP4 200
INDIRI4
EQI4 $208
line 691
;691:		bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 692
;692:	}
LABELV $208
line 694
;693:	//if already a CTF or team goal
;694:	if (bs->ltgtype == LTG_TEAMHELP ||
ADDRLP4 204
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 204
INDIRI4
CNSTI4 1
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 2
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 3
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 4
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 5
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 6
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 8
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 9
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 10
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 14
EQI4 $221
ADDRLP4 204
INDIRI4
CNSTI4 15
NEI4 $210
LABELV $221
line 704
;695:			bs->ltgtype == LTG_TEAMACCOMPANY ||
;696:			bs->ltgtype == LTG_DEFENDKEYAREA ||
;697:			bs->ltgtype == LTG_GETFLAG ||
;698:			bs->ltgtype == LTG_RUSHBASE ||
;699:			bs->ltgtype == LTG_RETURNFLAG ||
;700:			bs->ltgtype == LTG_CAMPORDER ||
;701:			bs->ltgtype == LTG_PATROL ||
;702:			bs->ltgtype == LTG_GETITEM ||
;703:			bs->ltgtype == LTG_MAKELOVE_UNDER ||
;704:			bs->ltgtype == LTG_MAKELOVE_ONTOP) {
line 705
;705:		return;
ADDRGP4 $136
JUMPV
LABELV $210
line 708
;706:	}
;707:	//
;708:	if (BotSetLastOrderedTask(bs))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
ADDRGP4 BotSetLastOrderedTask
CALLI4
ASGNI4
ADDRLP4 208
INDIRI4
CNSTI4 0
EQI4 $222
line 709
;709:		return;
ADDRGP4 $136
JUMPV
LABELV $222
line 711
;710:	//
;711:	if (bs->owndecision_time > FloatTime())
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 floattime
INDIRF4
LEF4 $224
line 712
;712:		return;;
ADDRGP4 $136
JUMPV
LABELV $224
line 714
;713:	//if the bot is roaming
;714:	if (bs->ctfroam_time > FloatTime())
ADDRFP4 0
INDIRP4
CNSTI4 6164
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $226
line 715
;715:		return;
ADDRGP4 $136
JUMPV
LABELV $226
line 717
;716:	//if the bot has anough aggression to decide what to do
;717:	if (BotAggression(bs) < 50)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 212
ADDRGP4 BotAggression
CALLF4
ASGNF4
ADDRLP4 212
INDIRF4
CNSTF4 1112014848
GEF4 $228
line 718
;718:		return;
ADDRGP4 $136
JUMPV
LABELV $228
line 720
;719:	//set the time to send a message to the team mates
;720:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 216
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
ADDRLP4 216
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 722
;721:	//
;722:	if (bs->teamtaskpreference & (TEAMTP_ATTACKER|TEAMTP_DEFENDER)) {
ADDRFP4 0
INDIRP4
CNSTI4 6752
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $230
line 723
;723:		if (bs->teamtaskpreference & TEAMTP_ATTACKER) {
ADDRFP4 0
INDIRP4
CNSTI4 6752
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $232
line 724
;724:			l1 = 0.7f;
ADDRLP4 8
CNSTF4 1060320051
ASGNF4
line 725
;725:		}
ADDRGP4 $233
JUMPV
LABELV $232
line 726
;726:		else {
line 727
;727:			l1 = 0.2f;
ADDRLP4 8
CNSTF4 1045220557
ASGNF4
line 728
;728:		}
LABELV $233
line 729
;729:		l2 = 0.9f;
ADDRLP4 12
CNSTF4 1063675494
ASGNF4
line 730
;730:	}
ADDRGP4 $231
JUMPV
LABELV $230
line 731
;731:	else {
line 732
;732:		l1 = 0.4f;
ADDRLP4 8
CNSTF4 1053609165
ASGNF4
line 733
;733:		l2 = 0.7f;
ADDRLP4 12
CNSTF4 1060320051
ASGNF4
line 734
;734:	}
LABELV $231
line 736
;735:	//get the flag or defend the base
;736:	rnd = random();
ADDRLP4 220
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 220
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ASGNF4
line 737
;737:	if (rnd < l1 && ctf_redflag.areanum && ctf_blueflag.areanum) {
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRF4
GEF4 $234
ADDRLP4 224
CNSTI4 0
ASGNI4
ADDRGP4 ctf_redflag+12
INDIRI4
ADDRLP4 224
INDIRI4
EQI4 $234
ADDRGP4 ctf_blueflag+12
INDIRI4
ADDRLP4 224
INDIRI4
EQI4 $234
line 738
;738:		bs->decisionmaker = bs->client;
ADDRLP4 228
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 228
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 228
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 739
;739:		bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 740
;740:		bs->ltgtype = LTG_GETFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 4
ASGNI4
line 742
;741:		//set the time the bot will stop getting the flag
;742:		bs->teamgoal_time = FloatTime() + CTF_GETFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 744
;743:		//get an alternative route goal towards the enemy base
;744:		BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 232
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 232
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 745
;745:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 746
;746:	}
ADDRGP4 $235
JUMPV
LABELV $234
line 747
;747:	else if (rnd < l2 && ctf_redflag.areanum && ctf_blueflag.areanum) {
ADDRLP4 4
INDIRF4
ADDRLP4 12
INDIRF4
GEF4 $238
ADDRLP4 228
CNSTI4 0
ASGNI4
ADDRGP4 ctf_redflag+12
INDIRI4
ADDRLP4 228
INDIRI4
EQI4 $238
ADDRGP4 ctf_blueflag+12
INDIRI4
ADDRLP4 228
INDIRI4
EQI4 $238
line 748
;748:		bs->decisionmaker = bs->client;
ADDRLP4 232
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 232
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 232
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 749
;749:		bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 751
;750:		//
;751:		if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &ctf_redflag, sizeof(bot_goal_t));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 236
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 236
INDIRI4
CNSTI4 1
NEI4 $242
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRGP4 ctf_redflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $243
JUMPV
LABELV $242
line 752
;752:		else memcpy(&bs->teamgoal, &ctf_blueflag, sizeof(bot_goal_t));
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRGP4 ctf_blueflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
LABELV $243
line 754
;753:		//set the ltg type
;754:		bs->ltgtype = LTG_DEFENDKEYAREA;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 3
ASGNI4
line 756
;755:		//set the time the bot stops defending the base
;756:		bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 757
;757:		bs->defendaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6144
ADDP4
CNSTF4 0
ASGNF4
line 758
;758:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 759
;759:	}
ADDRGP4 $239
JUMPV
LABELV $238
line 760
;760:	else {
line 761
;761:		bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 763
;762:		//set the time the bot will stop roaming
;763:		bs->ctfroam_time = FloatTime() + CTF_ROAM_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6164
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 764
;764:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 765
;765:	}
LABELV $239
LABELV $235
line 766
;766:	bs->owndecision_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6620
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CVFI4 4
ASGNI4
line 770
;767:#ifdef DEBUG
;768:	BotPrintTeamGoal(bs);
;769:#endif //DEBUG
;770:}
LABELV $136
endproc BotCTFSeekGoals 240 12
export BotCTFRetreatGoals
proc BotCTFRetreatGoals 8 4
line 777
;771:
;772:/*
;773:==================
;774:BotCTFRetreatGoals
;775:==================
;776:*/
;777:void BotCTFRetreatGoals(bot_state_t *bs) {
line 779
;778:	//when carrying a flag in ctf the bot should rush to the base
;779:	if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $245
line 781
;780:		//if not already rushing to the base
;781:		if (bs->ltgtype != LTG_RUSHBASE) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 5
EQI4 $247
line 782
;782:			BotRefuseOrder(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRefuseOrder
CALLV
pop
line 783
;783:			bs->ltgtype = LTG_RUSHBASE;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 5
ASGNI4
line 784
;784:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1123024896
ADDF4
ASGNF4
line 785
;785:			bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 786
;786:			bs->decisionmaker = bs->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 787
;787:			bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 788
;788:			BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 789
;789:		}
LABELV $247
line 790
;790:	}
LABELV $245
line 791
;791:}
LABELV $244
endproc BotCTFRetreatGoals 8 4
export BotTeamGoals
proc BotTeamGoals 0 4
line 1327
;792:
;793:#ifdef MISSIONPACK
;794:/*
;795:==================
;796:Bot1FCTFSeekGoals
;797:==================
;798:*/
;799:void Bot1FCTFSeekGoals(bot_state_t *bs) {
;800:	aas_entityinfo_t entinfo;
;801:	float rnd, l1, l2;
;802:	int c;
;803:
;804:	//when carrying a flag in ctf the bot should rush to the base
;805:	if (Bot1FCTFCarryingFlag(bs)) {
;806:		//if not already rushing to the base
;807:		if (bs->ltgtype != LTG_RUSHBASE) {
;808:			BotRefuseOrder(bs);
;809:			bs->ltgtype = LTG_RUSHBASE;
;810:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
;811:			bs->rushbaseaway_time = 0;
;812:			bs->decisionmaker = bs->client;
;813:			bs->ordered = qfalse;
;814:			//get an alternative route goal towards the enemy base
;815:			BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
;816:			//
;817:			BotSetTeamStatus(bs);
;818:			BotVoiceChat(bs, -1, VOICECHAT_IHAVEFLAG);
;819:		}
;820:		return;
;821:	}
;822:	// if the bot decided to follow someone
;823:	if ( bs->ltgtype == LTG_TEAMACCOMPANY && !bs->ordered ) {
;824:		// if the team mate being accompanied no longer carries the flag
;825:		BotEntityInfo(bs->teammate, &entinfo);
;826:		if (!EntityCarriesFlag(&entinfo)) {
;827:			bs->ltgtype = 0;
;828:		}
;829:	}
;830:	//our team has the flag
;831:	if (bs->neutralflagstatus == 1) {
;832:		if (bs->owndecision_time < FloatTime()) {
;833:			// if not already following someone
;834:			if (bs->ltgtype != LTG_TEAMACCOMPANY) {
;835:				//if there is a visible team mate flag carrier
;836:				c = BotTeamFlagCarrierVisible(bs);
;837:				if (c >= 0) {
;838:					BotRefuseOrder(bs);
;839:					//follow the flag carrier
;840:					bs->decisionmaker = bs->client;
;841:					bs->ordered = qfalse;
;842:					//the team mate
;843:					bs->teammate = c;
;844:					//last time the team mate was visible
;845:					bs->teammatevisible_time = FloatTime();
;846:					//no message
;847:					bs->teammessage_time = 0;
;848:					//no arrive message
;849:					bs->arrive_time = 1;
;850:					//
;851:					BotVoiceChat(bs, bs->teammate, VOICECHAT_ONFOLLOW);
;852:					//get the team goal time
;853:					bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
;854:					bs->ltgtype = LTG_TEAMACCOMPANY;
;855:					bs->formation_dist = 3.5 * 32;		//3.5 meter
;856:					BotSetTeamStatus(bs);
;857:					bs->owndecision_time = FloatTime() + 5;
;858:					return;
;859:				}
;860:			}
;861:			//if already a CTF or team goal
;862:			if (bs->ltgtype == LTG_TEAMHELP ||
;863:					bs->ltgtype == LTG_TEAMACCOMPANY ||
;864:					bs->ltgtype == LTG_DEFENDKEYAREA ||
;865:					bs->ltgtype == LTG_GETFLAG ||
;866:					bs->ltgtype == LTG_RUSHBASE ||
;867:					bs->ltgtype == LTG_CAMPORDER ||
;868:					bs->ltgtype == LTG_PATROL ||
;869:					bs->ltgtype == LTG_ATTACKENEMYBASE ||
;870:					bs->ltgtype == LTG_GETITEM ||
;871:					bs->ltgtype == LTG_MAKELOVE_UNDER ||
;872:					bs->ltgtype == LTG_MAKELOVE_ONTOP) {
;873:				return;
;874:			}
;875:			//if not already attacking the enemy base
;876:			if (bs->ltgtype != LTG_ATTACKENEMYBASE) {
;877:				BotRefuseOrder(bs);
;878:				bs->decisionmaker = bs->client;
;879:				bs->ordered = qfalse;
;880:				//
;881:				if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &ctf_blueflag, sizeof(bot_goal_t));
;882:				else memcpy(&bs->teamgoal, &ctf_redflag, sizeof(bot_goal_t));
;883:				//set the ltg type
;884:				bs->ltgtype = LTG_ATTACKENEMYBASE;
;885:				//set the time the bot will stop getting the flag
;886:				bs->teamgoal_time = FloatTime() + TEAM_ATTACKENEMYBASE_TIME;
;887:				BotSetTeamStatus(bs);
;888:				bs->owndecision_time = FloatTime() + 5;
;889:			}
;890:		}
;891:		return;
;892:	}
;893:	//enemy team has the flag
;894:	else if (bs->neutralflagstatus == 2) {
;895:		if (bs->owndecision_time < FloatTime()) {
;896:			c = BotEnemyFlagCarrierVisible(bs);
;897:			if (c >= 0) {
;898:				//FIXME: attack enemy flag carrier
;899:			}
;900:			//if already a CTF or team goal
;901:			if (bs->ltgtype == LTG_TEAMHELP ||
;902:					bs->ltgtype == LTG_TEAMACCOMPANY ||
;903:					bs->ltgtype == LTG_CAMPORDER ||
;904:					bs->ltgtype == LTG_PATROL ||
;905:					bs->ltgtype == LTG_GETITEM) {
;906:				return;
;907:			}
;908:			// if not already defending the base
;909:			if (bs->ltgtype != LTG_DEFENDKEYAREA) {
;910:				BotRefuseOrder(bs);
;911:				bs->decisionmaker = bs->client;
;912:				bs->ordered = qfalse;
;913:				//
;914:				if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &ctf_redflag, sizeof(bot_goal_t));
;915:				else memcpy(&bs->teamgoal, &ctf_blueflag, sizeof(bot_goal_t));
;916:				//set the ltg type
;917:				bs->ltgtype = LTG_DEFENDKEYAREA;
;918:				//set the time the bot stops defending the base
;919:				bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
;920:				bs->defendaway_time = 0;
;921:				BotSetTeamStatus(bs);
;922:				bs->owndecision_time = FloatTime() + 5;
;923:			}
;924:		}
;925:		return;
;926:	}
;927:	// don't just do something wait for the bot team leader to give orders
;928:	if (BotTeamLeader(bs)) {
;929:		return;
;930:	}
;931:	// if the bot is ordered to do something
;932:	if ( bs->lastgoal_ltgtype ) {
;933:		bs->teamgoal_time += 60;
;934:	}
;935:	// if the bot decided to do something on it's own and has a last ordered goal
;936:	if ( !bs->ordered && bs->lastgoal_ltgtype ) {
;937:		bs->ltgtype = 0;
;938:	}
;939:	//if already a CTF or team goal
;940:	if (bs->ltgtype == LTG_TEAMHELP ||
;941:			bs->ltgtype == LTG_TEAMACCOMPANY ||
;942:			bs->ltgtype == LTG_DEFENDKEYAREA ||
;943:			bs->ltgtype == LTG_GETFLAG ||
;944:			bs->ltgtype == LTG_RUSHBASE ||
;945:			bs->ltgtype == LTG_RETURNFLAG ||
;946:			bs->ltgtype == LTG_CAMPORDER ||
;947:			bs->ltgtype == LTG_PATROL ||
;948:			bs->ltgtype == LTG_ATTACKENEMYBASE ||
;949:			bs->ltgtype == LTG_GETITEM ||
;950:			bs->ltgtype == LTG_MAKELOVE_UNDER ||
;951:			bs->ltgtype == LTG_MAKELOVE_ONTOP) {
;952:		return;
;953:	}
;954:	//
;955:	if (BotSetLastOrderedTask(bs))
;956:		return;
;957:	//
;958:	if (bs->owndecision_time > FloatTime())
;959:		return;;
;960:	//if the bot is roaming
;961:	if (bs->ctfroam_time > FloatTime())
;962:		return;
;963:	//if the bot has anough aggression to decide what to do
;964:	if (BotAggression(bs) < 50)
;965:		return;
;966:	//set the time to send a message to the team mates
;967:	bs->teammessage_time = FloatTime() + 2 * random();
;968:	//
;969:	if (bs->teamtaskpreference & (TEAMTP_ATTACKER|TEAMTP_DEFENDER)) {
;970:		if (bs->teamtaskpreference & TEAMTP_ATTACKER) {
;971:			l1 = 0.7f;
;972:		}
;973:		else {
;974:			l1 = 0.2f;
;975:		}
;976:		l2 = 0.9f;
;977:	}
;978:	else {
;979:		l1 = 0.4f;
;980:		l2 = 0.7f;
;981:	}
;982:	//get the flag or defend the base
;983:	rnd = random();
;984:	if (rnd < l1 && ctf_neutralflag.areanum) {
;985:		bs->decisionmaker = bs->client;
;986:		bs->ordered = qfalse;
;987:		bs->ltgtype = LTG_GETFLAG;
;988:		//set the time the bot will stop getting the flag
;989:		bs->teamgoal_time = FloatTime() + CTF_GETFLAG_TIME;
;990:		BotSetTeamStatus(bs);
;991:	}
;992:	else if (rnd < l2 && ctf_redflag.areanum && ctf_blueflag.areanum) {
;993:		bs->decisionmaker = bs->client;
;994:		bs->ordered = qfalse;
;995:		//
;996:		if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &ctf_redflag, sizeof(bot_goal_t));
;997:		else memcpy(&bs->teamgoal, &ctf_blueflag, sizeof(bot_goal_t));
;998:		//set the ltg type
;999:		bs->ltgtype = LTG_DEFENDKEYAREA;
;1000:		//set the time the bot stops defending the base
;1001:		bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
;1002:		bs->defendaway_time = 0;
;1003:		BotSetTeamStatus(bs);
;1004:	}
;1005:	else {
;1006:		bs->ltgtype = 0;
;1007:		//set the time the bot will stop roaming
;1008:		bs->ctfroam_time = FloatTime() + CTF_ROAM_TIME;
;1009:		BotSetTeamStatus(bs);
;1010:	}
;1011:	bs->owndecision_time = FloatTime() + 5;
;1012:#ifdef DEBUG
;1013:	BotPrintTeamGoal(bs);
;1014:#endif //DEBUG
;1015:}
;1016:
;1017:/*
;1018:==================
;1019:Bot1FCTFRetreatGoals
;1020:==================
;1021:*/
;1022:void Bot1FCTFRetreatGoals(bot_state_t *bs) {
;1023:	//when carrying a flag in ctf the bot should rush to the enemy base
;1024:	if (Bot1FCTFCarryingFlag(bs)) {
;1025:		//if not already rushing to the base
;1026:		if (bs->ltgtype != LTG_RUSHBASE) {
;1027:			BotRefuseOrder(bs);
;1028:			bs->ltgtype = LTG_RUSHBASE;
;1029:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
;1030:			bs->rushbaseaway_time = 0;
;1031:			bs->decisionmaker = bs->client;
;1032:			bs->ordered = qfalse;
;1033:			//get an alternative route goal towards the enemy base
;1034:			BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
;1035:			BotSetTeamStatus(bs);
;1036:		}
;1037:	}
;1038:}
;1039:
;1040:/*
;1041:==================
;1042:BotObeliskSeekGoals
;1043:==================
;1044:*/
;1045:void BotObeliskSeekGoals(bot_state_t *bs) {
;1046:	float rnd, l1, l2;
;1047:
;1048:	// don't just do something wait for the bot team leader to give orders
;1049:	if (BotTeamLeader(bs)) {
;1050:		return;
;1051:	}
;1052:	// if the bot is ordered to do something
;1053:	if ( bs->lastgoal_ltgtype ) {
;1054:		bs->teamgoal_time += 60;
;1055:	}
;1056:	//if already a team goal
;1057:	if (bs->ltgtype == LTG_TEAMHELP ||
;1058:			bs->ltgtype == LTG_TEAMACCOMPANY ||
;1059:			bs->ltgtype == LTG_DEFENDKEYAREA ||
;1060:			bs->ltgtype == LTG_GETFLAG ||
;1061:			bs->ltgtype == LTG_RUSHBASE ||
;1062:			bs->ltgtype == LTG_RETURNFLAG ||
;1063:			bs->ltgtype == LTG_CAMPORDER ||
;1064:			bs->ltgtype == LTG_PATROL ||
;1065:			bs->ltgtype == LTG_ATTACKENEMYBASE ||
;1066:			bs->ltgtype == LTG_GETITEM ||
;1067:			bs->ltgtype == LTG_MAKELOVE_UNDER ||
;1068:			bs->ltgtype == LTG_MAKELOVE_ONTOP) {
;1069:		return;
;1070:	}
;1071:	//
;1072:	if (BotSetLastOrderedTask(bs))
;1073:		return;
;1074:	//if the bot is roaming
;1075:	if (bs->ctfroam_time > FloatTime())
;1076:		return;
;1077:	//if the bot has anough aggression to decide what to do
;1078:	if (BotAggression(bs) < 50)
;1079:		return;
;1080:	//set the time to send a message to the team mates
;1081:	bs->teammessage_time = FloatTime() + 2 * random();
;1082:	//
;1083:	if (bs->teamtaskpreference & (TEAMTP_ATTACKER|TEAMTP_DEFENDER)) {
;1084:		if (bs->teamtaskpreference & TEAMTP_ATTACKER) {
;1085:			l1 = 0.7f;
;1086:		}
;1087:		else {
;1088:			l1 = 0.2f;
;1089:		}
;1090:		l2 = 0.9f;
;1091:	}
;1092:	else {
;1093:		l1 = 0.4f;
;1094:		l2 = 0.7f;
;1095:	}
;1096:	//get the flag or defend the base
;1097:	rnd = random();
;1098:	if (rnd < l1 && redobelisk.areanum && blueobelisk.areanum) {
;1099:		bs->decisionmaker = bs->client;
;1100:		bs->ordered = qfalse;
;1101:		//
;1102:		if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &blueobelisk, sizeof(bot_goal_t));
;1103:		else memcpy(&bs->teamgoal, &redobelisk, sizeof(bot_goal_t));
;1104:		//set the ltg type
;1105:		bs->ltgtype = LTG_ATTACKENEMYBASE;
;1106:		//set the time the bot will stop attacking the enemy base
;1107:		bs->teamgoal_time = FloatTime() + TEAM_ATTACKENEMYBASE_TIME;
;1108:		//get an alternate route goal towards the enemy base
;1109:		BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
;1110:		BotSetTeamStatus(bs);
;1111:	}
;1112:	else if (rnd < l2 && redobelisk.areanum && blueobelisk.areanum) {
;1113:		bs->decisionmaker = bs->client;
;1114:		bs->ordered = qfalse;
;1115:		//
;1116:		if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &redobelisk, sizeof(bot_goal_t));
;1117:		else memcpy(&bs->teamgoal, &blueobelisk, sizeof(bot_goal_t));
;1118:		//set the ltg type
;1119:		bs->ltgtype = LTG_DEFENDKEYAREA;
;1120:		//set the time the bot stops defending the base
;1121:		bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
;1122:		bs->defendaway_time = 0;
;1123:		BotSetTeamStatus(bs);
;1124:	}
;1125:	else {
;1126:		bs->ltgtype = 0;
;1127:		//set the time the bot will stop roaming
;1128:		bs->ctfroam_time = FloatTime() + CTF_ROAM_TIME;
;1129:		BotSetTeamStatus(bs);
;1130:	}
;1131:}
;1132:
;1133:/*
;1134:==================
;1135:BotGoHarvest
;1136:==================
;1137:*/
;1138:void BotGoHarvest(bot_state_t *bs) {
;1139:	//
;1140:	if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &blueobelisk, sizeof(bot_goal_t));
;1141:	else memcpy(&bs->teamgoal, &redobelisk, sizeof(bot_goal_t));
;1142:	//set the ltg type
;1143:	bs->ltgtype = LTG_HARVEST;
;1144:	//set the time the bot will stop harvesting
;1145:	bs->teamgoal_time = FloatTime() + TEAM_HARVEST_TIME;
;1146:	bs->harvestaway_time = 0;
;1147:	BotSetTeamStatus(bs);
;1148:}
;1149:
;1150:/*
;1151:==================
;1152:BotObeliskRetreatGoals
;1153:==================
;1154:*/
;1155:void BotObeliskRetreatGoals(bot_state_t *bs) {
;1156:	//nothing special
;1157:}
;1158:
;1159:/*
;1160:==================
;1161:BotHarvesterSeekGoals
;1162:==================
;1163:*/
;1164:void BotHarvesterSeekGoals(bot_state_t *bs) {
;1165:	aas_entityinfo_t entinfo;
;1166:	float rnd, l1, l2;
;1167:	int c;
;1168:
;1169:	//when carrying cubes in harvester the bot should rush to the base
;1170:	if (BotHarvesterCarryingCubes(bs)) {
;1171:		//if not already rushing to the base
;1172:		if (bs->ltgtype != LTG_RUSHBASE) {
;1173:			BotRefuseOrder(bs);
;1174:			bs->ltgtype = LTG_RUSHBASE;
;1175:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
;1176:			bs->rushbaseaway_time = 0;
;1177:			bs->decisionmaker = bs->client;
;1178:			bs->ordered = qfalse;
;1179:			//get an alternative route goal towards the enemy base
;1180:			BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
;1181:			//
;1182:			BotSetTeamStatus(bs);
;1183:		}
;1184:		return;
;1185:	}
;1186:	// don't just do something wait for the bot team leader to give orders
;1187:	if (BotTeamLeader(bs)) {
;1188:		return;
;1189:	}
;1190:	// if the bot decided to follow someone
;1191:	if ( bs->ltgtype == LTG_TEAMACCOMPANY && !bs->ordered ) {
;1192:		// if the team mate being accompanied no longer carries the flag
;1193:		BotEntityInfo(bs->teammate, &entinfo);
;1194:		if (!EntityCarriesCubes(&entinfo)) {
;1195:			bs->ltgtype = 0;
;1196:		}
;1197:	}
;1198:	// if the bot is ordered to do something
;1199:	if ( bs->lastgoal_ltgtype ) {
;1200:		bs->teamgoal_time += 60;
;1201:	}
;1202:	//if not yet doing something
;1203:	if (bs->ltgtype == LTG_TEAMHELP ||
;1204:			bs->ltgtype == LTG_TEAMACCOMPANY ||
;1205:			bs->ltgtype == LTG_DEFENDKEYAREA ||
;1206:			bs->ltgtype == LTG_GETFLAG ||
;1207:			bs->ltgtype == LTG_CAMPORDER ||
;1208:			bs->ltgtype == LTG_PATROL ||
;1209:			bs->ltgtype == LTG_ATTACKENEMYBASE ||
;1210:			bs->ltgtype == LTG_HARVEST ||
;1211:			bs->ltgtype == LTG_GETITEM ||
;1212:			bs->ltgtype == LTG_MAKELOVE_UNDER ||
;1213:			bs->ltgtype == LTG_MAKELOVE_ONTOP) {
;1214:		return;
;1215:	}
;1216:	//
;1217:	if (BotSetLastOrderedTask(bs))
;1218:		return;
;1219:	//if the bot is roaming
;1220:	if (bs->ctfroam_time > FloatTime())
;1221:		return;
;1222:	//if the bot has anough aggression to decide what to do
;1223:	if (BotAggression(bs) < 50)
;1224:		return;
;1225:	//set the time to send a message to the team mates
;1226:	bs->teammessage_time = FloatTime() + 2 * random();
;1227:	//
;1228:	c = BotEnemyCubeCarrierVisible(bs);
;1229:	if (c >= 0) {
;1230:		//FIXME: attack enemy cube carrier
;1231:	}
;1232:	if (bs->ltgtype != LTG_TEAMACCOMPANY) {
;1233:		//if there is a visible team mate carrying cubes
;1234:		c = BotTeamCubeCarrierVisible(bs);
;1235:		if (c >= 0) {
;1236:			//follow the team mate carrying cubes
;1237:			bs->decisionmaker = bs->client;
;1238:			bs->ordered = qfalse;
;1239:			//the team mate
;1240:			bs->teammate = c;
;1241:			//last time the team mate was visible
;1242:			bs->teammatevisible_time = FloatTime();
;1243:			//no message
;1244:			bs->teammessage_time = 0;
;1245:			//no arrive message
;1246:			bs->arrive_time = 1;
;1247:			//
;1248:			BotVoiceChat(bs, bs->teammate, VOICECHAT_ONFOLLOW);
;1249:			//get the team goal time
;1250:			bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
;1251:			bs->ltgtype = LTG_TEAMACCOMPANY;
;1252:			bs->formation_dist = 3.5 * 32;		//3.5 meter
;1253:			BotSetTeamStatus(bs);
;1254:			return;
;1255:		}
;1256:	}
;1257:	//
;1258:	if (bs->teamtaskpreference & (TEAMTP_ATTACKER|TEAMTP_DEFENDER)) {
;1259:		if (bs->teamtaskpreference & TEAMTP_ATTACKER) {
;1260:			l1 = 0.7f;
;1261:		}
;1262:		else {
;1263:			l1 = 0.2f;
;1264:		}
;1265:		l2 = 0.9f;
;1266:	}
;1267:	else {
;1268:		l1 = 0.4f;
;1269:		l2 = 0.7f;
;1270:	}
;1271:	//
;1272:	rnd = random();
;1273:	if (rnd < l1 && redobelisk.areanum && blueobelisk.areanum) {
;1274:		bs->decisionmaker = bs->client;
;1275:		bs->ordered = qfalse;
;1276:		BotGoHarvest(bs);
;1277:	}
;1278:	else if (rnd < l2 && redobelisk.areanum && blueobelisk.areanum) {
;1279:		bs->decisionmaker = bs->client;
;1280:		bs->ordered = qfalse;
;1281:		//
;1282:		if (BotTeam(bs) == TEAM_RED) memcpy(&bs->teamgoal, &redobelisk, sizeof(bot_goal_t));
;1283:		else memcpy(&bs->teamgoal, &blueobelisk, sizeof(bot_goal_t));
;1284:		//set the ltg type
;1285:		bs->ltgtype = LTG_DEFENDKEYAREA;
;1286:		//set the time the bot stops defending the base
;1287:		bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
;1288:		bs->defendaway_time = 0;
;1289:		BotSetTeamStatus(bs);
;1290:	}
;1291:	else {
;1292:		bs->ltgtype = 0;
;1293:		//set the time the bot will stop roaming
;1294:		bs->ctfroam_time = FloatTime() + CTF_ROAM_TIME;
;1295:		BotSetTeamStatus(bs);
;1296:	}
;1297:}
;1298:
;1299:/*
;1300:==================
;1301:BotHarvesterRetreatGoals
;1302:==================
;1303:*/
;1304:void BotHarvesterRetreatGoals(bot_state_t *bs) {
;1305:	//when carrying cubes in harvester the bot should rush to the base
;1306:	if (BotHarvesterCarryingCubes(bs)) {
;1307:		//if not already rushing to the base
;1308:		if (bs->ltgtype != LTG_RUSHBASE) {
;1309:			BotRefuseOrder(bs);
;1310:			bs->ltgtype = LTG_RUSHBASE;
;1311:			bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
;1312:			bs->rushbaseaway_time = 0;
;1313:			bs->decisionmaker = bs->client;
;1314:			bs->ordered = qfalse;
;1315:			BotSetTeamStatus(bs);
;1316:		}
;1317:		return;
;1318:	}
;1319:}
;1320:#endif
;1321:
;1322:/*
;1323:==================
;1324:BotTeamGoals
;1325:==================
;1326:*/
;1327:void BotTeamGoals(bot_state_t *bs, int retreat) {
line 1329
;1328:
;1329:	if ( retreat ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $250
line 1330
;1330:		if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $251
line 1331
;1331:			BotCTFRetreatGoals(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFRetreatGoals
CALLV
pop
line 1332
;1332:		}
line 1344
;1333:#ifdef MISSIONPACK
;1334:		else if (gametype == GT_1FCTF) {
;1335:			Bot1FCTFRetreatGoals(bs);
;1336:		}
;1337:		else if (gametype == GT_OBELISK) {
;1338:			BotObeliskRetreatGoals(bs);
;1339:		}
;1340:		else if (gametype == GT_HARVESTER) {
;1341:			BotHarvesterRetreatGoals(bs);
;1342:		}
;1343:#endif
;1344:	}
ADDRGP4 $251
JUMPV
LABELV $250
line 1345
;1345:	else {
line 1346
;1346:		if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $254
line 1348
;1347:			//decide what to do in CTF mode
;1348:			BotCTFSeekGoals(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFSeekGoals
CALLV
pop
line 1349
;1349:		}
LABELV $254
line 1361
;1350:#ifdef MISSIONPACK
;1351:		else if (gametype == GT_1FCTF) {
;1352:			Bot1FCTFSeekGoals(bs);
;1353:		}
;1354:		else if (gametype == GT_OBELISK) {
;1355:			BotObeliskSeekGoals(bs);
;1356:		}
;1357:		else if (gametype == GT_HARVESTER) {
;1358:			BotHarvesterSeekGoals(bs);
;1359:		}
;1360:#endif
;1361:	}
LABELV $251
line 1364
;1362:	// reset the order time which is used to see if
;1363:	// we decided to refuse an order
;1364:	bs->order_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
CNSTF4 0
ASGNF4
line 1365
;1365:}
LABELV $249
endproc BotTeamGoals 0 4
export BotPointAreaNum
proc BotPointAreaNum 68 20
line 1372
;1366:
;1367:/*
;1368:==================
;1369:BotPointAreaNum
;1370:==================
;1371:*/
;1372:int BotPointAreaNum(vec3_t origin) {
line 1376
;1373:	int areanum, numareas, areas[10];
;1374:	vec3_t end;
;1375:
;1376:	areanum = trap_AAS_PointAreaNum(origin);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 trap_AAS_PointAreaNum
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 60
INDIRI4
ASGNI4
line 1377
;1377:	if (areanum) return areanum;
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $257
ADDRLP4 12
INDIRI4
RETI4
ADDRGP4 $256
JUMPV
LABELV $257
line 1378
;1378:	VectorCopy(origin, end);
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 1379
;1379:	end[2] += 10;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1092616192
ADDF4
ASGNF4
line 1380
;1380:	numareas = trap_AAS_TraceAreas(origin, end, areas, NULL, 10);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 20
ARGP4
CNSTP4 0
ARGP4
CNSTI4 10
ARGI4
ADDRLP4 64
ADDRGP4 trap_AAS_TraceAreas
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 64
INDIRI4
ASGNI4
line 1381
;1381:	if (numareas > 0) return areas[0];
ADDRLP4 16
INDIRI4
CNSTI4 0
LEI4 $260
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $256
JUMPV
LABELV $260
line 1382
;1382:	return 0;
CNSTI4 0
RETI4
LABELV $256
endproc BotPointAreaNum 68 20
export ClientName
proc ClientName 1032 12
line 1390
;1383:}
;1384:
;1385:/*
;1386:==================
;1387:ClientName
;1388:==================
;1389:*/
;1390:char *ClientName(int client, char *name, int size) {
line 1393
;1391:	char buf[MAX_INFO_STRING];
;1392:
;1393:	if (client < 0 || client >= MAX_CLIENTS) {
ADDRLP4 1024
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
LTI4 $265
ADDRLP4 1024
INDIRI4
CNSTI4 64
LTI4 $263
LABELV $265
line 1394
;1394:		BotAI_Print(PRT_ERROR, "ClientName: client out of range\n");
CNSTI4 3
ARGI4
ADDRGP4 $266
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 1395
;1395:		return "[client out of range]";
ADDRGP4 $267
RETP4
ADDRGP4 $262
JUMPV
LABELV $263
line 1397
;1396:	}
;1397:	trap_GetConfigstring(CS_PLAYERS+client, buf, sizeof(buf));
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 1398
;1398:	strncpy(name, Info_ValueForKey(buf, "n"), size-1);
ADDRLP4 0
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 1399
;1399:	name[size-1] = '\0';
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRFP4 4
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 1400
;1400:	Q_CleanStr( name );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1401
;1401:	return name;
ADDRFP4 4
INDIRP4
RETP4
LABELV $262
endproc ClientName 1032 12
export ClientSkin
proc ClientSkin 1032 12
line 1409
;1402:}
;1403:
;1404:/*
;1405:==================
;1406:ClientSkin
;1407:==================
;1408:*/
;1409:char *ClientSkin(int client, char *skin, int size) {
line 1412
;1410:	char buf[MAX_INFO_STRING];
;1411:
;1412:	if (client < 0 || client >= MAX_CLIENTS) {
ADDRLP4 1024
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
LTI4 $272
ADDRLP4 1024
INDIRI4
CNSTI4 64
LTI4 $270
LABELV $272
line 1413
;1413:		BotAI_Print(PRT_ERROR, "ClientSkin: client out of range\n");
CNSTI4 3
ARGI4
ADDRGP4 $273
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 1414
;1414:		return "[client out of range]";
ADDRGP4 $267
RETP4
ADDRGP4 $269
JUMPV
LABELV $270
line 1416
;1415:	}
;1416:	trap_GetConfigstring(CS_PLAYERS+client, buf, sizeof(buf));
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 1417
;1417:	strncpy(skin, Info_ValueForKey(buf, "model"), size-1);
ADDRLP4 0
ARGP4
ADDRGP4 $274
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 1418
;1418:	skin[size-1] = '\0';
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRFP4 4
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 1419
;1419:	return skin;
ADDRFP4 4
INDIRP4
RETP4
LABELV $269
endproc ClientSkin 1032 12
bss
align 4
LABELV $276
skip 4
export ClientFromName
code
proc ClientFromName 1040 12
line 1427
;1420:}
;1421:
;1422:/*
;1423:==================
;1424:ClientFromName
;1425:==================
;1426:*/
;1427:int ClientFromName(char *name) {
line 1432
;1428:	int i;
;1429:	char buf[MAX_INFO_STRING];
;1430:	static int maxclients;
;1431:
;1432:	if (!maxclients)
ADDRGP4 $276
INDIRI4
CNSTI4 0
NEI4 $277
line 1433
;1433:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $279
ARGP4
ADDRLP4 1028
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $276
ADDRLP4 1028
INDIRI4
ASGNI4
LABELV $277
line 1434
;1434:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $283
JUMPV
LABELV $280
line 1435
;1435:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 1436
;1436:		Q_CleanStr( buf );
ADDRLP4 4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1437
;1437:		if (!Q_stricmp(Info_ValueForKey(buf, "n"), name)) return i;
ADDRLP4 4
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 1032
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1032
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $284
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $275
JUMPV
LABELV $284
line 1438
;1438:	}
LABELV $281
line 1434
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $283
ADDRLP4 0
INDIRI4
ADDRGP4 $276
INDIRI4
GEI4 $286
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $280
LABELV $286
line 1439
;1439:	return -1;
CNSTI4 -1
RETI4
LABELV $275
endproc ClientFromName 1040 12
bss
align 4
LABELV $288
skip 4
export ClientOnSameTeamFromName
code
proc ClientOnSameTeamFromName 1044 12
line 1447
;1440:}
;1441:
;1442:/*
;1443:==================
;1444:ClientOnSameTeamFromName
;1445:==================
;1446:*/
;1447:int ClientOnSameTeamFromName(bot_state_t *bs, char *name) {
line 1452
;1448:	int i;
;1449:	char buf[MAX_INFO_STRING];
;1450:	static int maxclients;
;1451:
;1452:	if (!maxclients)
ADDRGP4 $288
INDIRI4
CNSTI4 0
NEI4 $289
line 1453
;1453:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $279
ARGP4
ADDRLP4 1028
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $288
ADDRLP4 1028
INDIRI4
ASGNI4
LABELV $289
line 1454
;1454:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $294
JUMPV
LABELV $291
line 1455
;1455:		if (!BotSameTeam(bs, i))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1032
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $295
line 1456
;1456:			continue;
ADDRGP4 $292
JUMPV
LABELV $295
line 1457
;1457:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 1458
;1458:		Q_CleanStr( buf );
ADDRLP4 4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1459
;1459:		if (!Q_stricmp(Info_ValueForKey(buf, "n"), name)) return i;
ADDRLP4 4
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $297
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $287
JUMPV
LABELV $297
line 1460
;1460:	}
LABELV $292
line 1454
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $294
ADDRLP4 0
INDIRI4
ADDRGP4 $288
INDIRI4
GEI4 $299
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $291
LABELV $299
line 1461
;1461:	return -1;
CNSTI4 -1
RETI4
LABELV $287
endproc ClientOnSameTeamFromName 1044 12
export stristr
proc stristr 12 4
line 1469
;1462:}
;1463:
;1464:/*
;1465:==================
;1466:stristr
;1467:==================
;1468:*/
;1469:char *stristr(char *str, char *charset) {
ADDRGP4 $302
JUMPV
LABELV $301
line 1472
;1470:	int i;
;1471:
;1472:	while(*str) {
line 1473
;1473:		for (i = 0; charset[i] && str[i]; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $307
JUMPV
LABELV $304
line 1474
;1474:			if (toupper(charset[i]) != toupper(str[i])) break;
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 4
ADDRGP4 toupper
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 toupper
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $308
ADDRGP4 $306
JUMPV
LABELV $308
line 1475
;1475:		}
LABELV $305
line 1473
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $307
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
ADDRLP4 8
INDIRI4
EQI4 $310
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ADDRLP4 8
INDIRI4
NEI4 $304
LABELV $310
LABELV $306
line 1476
;1476:		if (!charset[i]) return str;
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $311
ADDRFP4 0
INDIRP4
RETP4
ADDRGP4 $300
JUMPV
LABELV $311
line 1477
;1477:		str++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1478
;1478:	}
LABELV $302
line 1472
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $301
line 1479
;1479:	return NULL;
CNSTP4 0
RETP4
LABELV $300
endproc stristr 12 4
export EasyClientName
proc EasyClientName 204 12
line 1487
;1480:}
;1481:
;1482:/*
;1483:==================
;1484:EasyClientName
;1485:==================
;1486:*/
;1487:char *EasyClientName(int client, char *buf, int size) {
line 1492
;1488:	int i;
;1489:	char *str1, *str2, *ptr, c;
;1490:	char name[128];
;1491:
;1492:	strcpy(name, ClientName(client, name, sizeof(name)));
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 5
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 148
ADDRGP4 ClientName
CALLP4
ASGNP4
ADDRLP4 5
ARGP4
ADDRLP4 148
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1493
;1493:	for (i = 0; name[i]; i++) name[i] &= 127;
ADDRLP4 136
CNSTI4 0
ASGNI4
ADDRGP4 $317
JUMPV
LABELV $314
ADDRLP4 152
ADDRLP4 136
INDIRI4
ADDRLP4 5
ADDP4
ASGNP4
ADDRLP4 152
INDIRP4
ADDRLP4 152
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
BANDI4
CVII1 4
ASGNI1
LABELV $315
ADDRLP4 136
ADDRLP4 136
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $317
ADDRLP4 136
INDIRI4
ADDRLP4 5
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $314
line 1495
;1494:	//remove all spaces
;1495:	for (ptr = strstr(name, " "); ptr; ptr = strstr(name, " ")) {
ADDRLP4 5
ARGP4
ADDRGP4 $322
ARGP4
ADDRLP4 156
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 156
INDIRP4
ASGNP4
ADDRGP4 $321
JUMPV
LABELV $318
line 1496
;1496:		memmove(ptr, ptr+1, strlen(ptr+1)+1);
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 160
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 168
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 168
INDIRI4
ADDP4
ARGP4
ADDRLP4 160
INDIRI4
ADDRLP4 168
INDIRI4
ADDI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 1497
;1497:	}
LABELV $319
line 1495
ADDRLP4 5
ARGP4
ADDRGP4 $322
ARGP4
ADDRLP4 160
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 160
INDIRP4
ASGNP4
LABELV $321
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $318
line 1499
;1498:	//check for [x] and ]x[ clan names
;1499:	str1 = strstr(name, "[");
ADDRLP4 5
ARGP4
ADDRGP4 $323
ARGP4
ADDRLP4 164
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 140
ADDRLP4 164
INDIRP4
ASGNP4
line 1500
;1500:	str2 = strstr(name, "]");
ADDRLP4 5
ARGP4
ADDRGP4 $324
ARGP4
ADDRLP4 168
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 144
ADDRLP4 168
INDIRP4
ASGNP4
line 1501
;1501:	if (str1 && str2) {
ADDRLP4 172
CNSTU4 0
ASGNU4
ADDRLP4 140
INDIRP4
CVPU4 4
ADDRLP4 172
INDIRU4
EQU4 $325
ADDRLP4 144
INDIRP4
CVPU4 4
ADDRLP4 172
INDIRU4
EQU4 $325
line 1502
;1502:		if (str2 > str1) memmove(str1, str2+1, strlen(str2+1)+1);
ADDRLP4 144
INDIRP4
CVPU4 4
ADDRLP4 140
INDIRP4
CVPU4 4
LEU4 $327
ADDRLP4 144
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 176
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 140
INDIRP4
ARGP4
ADDRLP4 180
CNSTI4 1
ASGNI4
ADDRLP4 144
INDIRP4
ADDRLP4 180
INDIRI4
ADDP4
ARGP4
ADDRLP4 176
INDIRI4
ADDRLP4 180
INDIRI4
ADDI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
ADDRGP4 $328
JUMPV
LABELV $327
line 1503
;1503:		else memmove(str2, str1+1, strlen(str1+1)+1);
ADDRLP4 140
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 184
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 144
INDIRP4
ARGP4
ADDRLP4 188
CNSTI4 1
ASGNI4
ADDRLP4 140
INDIRP4
ADDRLP4 188
INDIRI4
ADDP4
ARGP4
ADDRLP4 184
INDIRI4
ADDRLP4 188
INDIRI4
ADDI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
LABELV $328
line 1504
;1504:	}
LABELV $325
line 1506
;1505:	//remove Mr prefix
;1506:	if ((name[0] == 'm' || name[0] == 'M') &&
ADDRLP4 176
ADDRLP4 5
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 109
EQI4 $333
ADDRLP4 176
INDIRI4
CNSTI4 77
NEI4 $329
LABELV $333
ADDRLP4 5+1
INDIRI1
CVII4 1
CNSTI4 114
EQI4 $334
ADDRLP4 5+1
INDIRI1
CVII4 1
CNSTI4 82
NEI4 $329
LABELV $334
line 1507
;1507:			(name[1] == 'r' || name[1] == 'R')) {
line 1508
;1508:		memmove(name, name+2, strlen(name+2)+1);
ADDRLP4 5+2
ARGP4
ADDRLP4 180
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 5
ARGP4
ADDRLP4 5+2
ARGP4
ADDRLP4 180
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 1509
;1509:	}
LABELV $329
line 1511
;1510:	//only allow lower case alphabet characters
;1511:	ptr = name;
ADDRLP4 0
ADDRLP4 5
ASGNP4
ADDRGP4 $338
JUMPV
LABELV $337
line 1512
;1512:	while(*ptr) {
line 1513
;1513:		c = *ptr;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
ASGNI1
line 1514
;1514:		if ((c >= 'a' && c <= 'z') ||
ADDRLP4 180
ADDRLP4 4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 97
LTI4 $343
ADDRLP4 180
INDIRI4
CNSTI4 122
LEI4 $344
LABELV $343
ADDRLP4 184
ADDRLP4 4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 184
INDIRI4
CNSTI4 48
LTI4 $345
ADDRLP4 184
INDIRI4
CNSTI4 57
LEI4 $344
LABELV $345
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 95
NEI4 $340
LABELV $344
line 1515
;1515:				(c >= '0' && c <= '9') || c == '_') {
line 1516
;1516:			ptr++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1517
;1517:		}
ADDRGP4 $341
JUMPV
LABELV $340
line 1518
;1518:		else if (c >= 'A' && c <= 'Z') {
ADDRLP4 188
ADDRLP4 4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 65
LTI4 $346
ADDRLP4 188
INDIRI4
CNSTI4 90
GTI4 $346
line 1519
;1519:			*ptr += 'a' - 'A';
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 32
ADDI4
CVII1 4
ASGNI1
line 1520
;1520:			ptr++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1521
;1521:		}
ADDRGP4 $347
JUMPV
LABELV $346
line 1522
;1522:		else {
line 1523
;1523:			memmove(ptr, ptr+1, strlen(ptr + 1)+1);
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 192
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 200
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 200
INDIRI4
ADDP4
ARGP4
ADDRLP4 192
INDIRI4
ADDRLP4 200
INDIRI4
ADDI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 1524
;1524:		}
LABELV $347
LABELV $341
line 1525
;1525:	}
LABELV $338
line 1512
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $337
line 1526
;1526:	strncpy(buf, name, size-1);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 5
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 1527
;1527:	buf[size-1] = '\0';
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRFP4 4
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 1528
;1528:	return buf;
ADDRFP4 4
INDIRP4
RETP4
LABELV $313
endproc EasyClientName 204 12
export BotSynonymContext
proc BotSynonymContext 8 4
line 1536
;1529:}
;1530:
;1531:/*
;1532:==================
;1533:BotSynonymContext
;1534:==================
;1535:*/
;1536:int BotSynonymContext(bot_state_t *bs) {
line 1539
;1537:	int context;
;1538:
;1539:	context = CONTEXT_NORMAL|CONTEXT_NEARBYITEM|CONTEXT_NAMES;
ADDRLP4 0
CNSTI4 1027
ASGNI4
line 1541
;1540:	//
;1541:	if (gametype == GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $349
line 1545
;1542:#ifdef MISSIONPACK
;1543:		|| gametype == GT_1FCTF
;1544:#endif
;1545:		) {
line 1546
;1546:		if (BotTeam(bs) == TEAM_RED) context |= CONTEXT_CTFREDTEAM;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $351
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 4
BORI4
ASGNI4
ADDRGP4 $352
JUMPV
LABELV $351
line 1547
;1547:		else context |= CONTEXT_CTFBLUETEAM;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 8
BORI4
ASGNI4
LABELV $352
line 1548
;1548:	}
LABELV $349
line 1559
;1549:#ifdef MISSIONPACK
;1550:	else if (gametype == GT_OBELISK) {
;1551:		if (BotTeam(bs) == TEAM_RED) context |= CONTEXT_OBELISKREDTEAM;
;1552:		else context |= CONTEXT_OBELISKBLUETEAM;
;1553:	}
;1554:	else if (gametype == GT_HARVESTER) {
;1555:		if (BotTeam(bs) == TEAM_RED) context |= CONTEXT_HARVESTERREDTEAM;
;1556:		else context |= CONTEXT_HARVESTERBLUETEAM;
;1557:	}
;1558:#endif
;1559:	return context;
ADDRLP4 0
INDIRI4
RETI4
LABELV $348
endproc BotSynonymContext 8 4
export BotChooseWeapon
proc BotChooseWeapon 20 8
line 1567
;1560:}
;1561:
;1562:/*
;1563:==================
;1564:BotChooseWeapon
;1565:==================
;1566:*/
;1567:void BotChooseWeapon(bot_state_t *bs) {
line 1570
;1568:	int newweaponnum;
;1569:
;1570:	if (bs->cur_ps.weaponstate == WEAPON_RAISING ||
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 164
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
EQI4 $356
ADDRLP4 4
INDIRI4
CNSTI4 2
NEI4 $354
LABELV $356
line 1571
;1571:			bs->cur_ps.weaponstate == WEAPON_DROPPING) {
line 1572
;1572:		trap_EA_SelectWeapon(bs->client, bs->weaponnum);
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_SelectWeapon
CALLV
pop
line 1573
;1573:	}
ADDRGP4 $355
JUMPV
LABELV $354
line 1574
;1574:	else {
line 1575
;1575:		newweaponnum = trap_BotChooseBestFightWeapon(bs->ws, bs->inventory);
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 4952
ADDP4
ARGP4
ADDRLP4 12
ADDRGP4 trap_BotChooseBestFightWeapon
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 1576
;1576:		if (bs->weaponnum != newweaponnum) bs->weaponchange_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $357
ADDRFP4 0
INDIRP4
CNSTI4 6192
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
LABELV $357
line 1577
;1577:		bs->weaponnum = newweaponnum;
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1579
;1578:		//BotAI_Print(PRT_MESSAGE, "bs->weaponnum = %d\n", bs->weaponnum);
;1579:		trap_EA_SelectWeapon(bs->client, bs->weaponnum);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_SelectWeapon
CALLV
pop
line 1580
;1580:	}
LABELV $355
line 1581
;1581:}
LABELV $353
endproc BotChooseWeapon 20 8
export BotSetupForMovement
proc BotSetupForMovement 88 12
line 1588
;1582:
;1583:/*
;1584:==================
;1585:BotSetupForMovement
;1586:==================
;1587:*/
;1588:void BotSetupForMovement(bot_state_t *bs) {
line 1591
;1589:	bot_initmove_t initmove;
;1590:
;1591:	memset(&initmove, 0, sizeof(bot_initmove_t));
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 68
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1592
;1592:	VectorCopy(bs->cur_ps.origin, initmove.origin);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRB
ASGNB 12
line 1593
;1593:	VectorCopy(bs->cur_ps.velocity, initmove.velocity);
ADDRLP4 0+12
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRB
ASGNB 12
line 1594
;1594:	VectorClear(initmove.viewoffset);
ADDRLP4 68
CNSTF4 0
ASGNF4
ADDRLP4 0+24+8
ADDRLP4 68
INDIRF4
ASGNF4
ADDRLP4 0+24+4
ADDRLP4 68
INDIRF4
ASGNF4
ADDRLP4 0+24
ADDRLP4 68
INDIRF4
ASGNF4
line 1595
;1595:	initmove.viewoffset[2] += bs->cur_ps.viewheight;
ADDRLP4 0+24+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 1596
;1596:	initmove.entitynum = bs->entitynum;
ADDRLP4 0+36
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1597
;1597:	initmove.client = bs->client;
ADDRLP4 0+40
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1598
;1598:	initmove.thinktime = bs->thinktime;
ADDRLP4 0+44
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
ASGNF4
line 1600
;1599:	//set the onground flag
;1600:	if (bs->cur_ps.groundEntityNum != ENTITYNUM_NONE) initmove.or_moveflags |= MFL_ONGROUND;
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
CNSTI4 1023
EQI4 $371
ADDRLP4 0+64
ADDRLP4 0+64
INDIRI4
CNSTI4 2
BORI4
ASGNI4
LABELV $371
line 1602
;1601:	//set the teleported flag
;1602:	if ((bs->cur_ps.pm_flags & PMF_TIME_KNOCKBACK) && (bs->cur_ps.pm_time > 0)) {
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
CNSTI4 64
BANDI4
ADDRLP4 76
INDIRI4
EQI4 $374
ADDRLP4 72
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDRLP4 76
INDIRI4
LEI4 $374
line 1603
;1603:		initmove.or_moveflags |= MFL_TELEPORTED;
ADDRLP4 0+64
ADDRLP4 0+64
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1604
;1604:	}
LABELV $374
line 1606
;1605:	//set the waterjump flag
;1606:	if ((bs->cur_ps.pm_flags & PMF_TIME_WATERJUMP) && (bs->cur_ps.pm_time > 0)) {
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 84
CNSTI4 0
ASGNI4
ADDRLP4 80
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
CNSTI4 256
BANDI4
ADDRLP4 84
INDIRI4
EQI4 $377
ADDRLP4 80
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDRLP4 84
INDIRI4
LEI4 $377
line 1607
;1607:		initmove.or_moveflags |= MFL_WATERJUMP;
ADDRLP4 0+64
ADDRLP4 0+64
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1608
;1608:	}
LABELV $377
line 1610
;1609:	//set presence type
;1610:	if (bs->cur_ps.pm_flags & PMF_DUCKED) initmove.presencetype = PRESENCE_CROUCH;
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $380
ADDRLP4 0+48
CNSTI4 4
ASGNI4
ADDRGP4 $381
JUMPV
LABELV $380
line 1611
;1611:	else initmove.presencetype = PRESENCE_NORMAL;
ADDRLP4 0+48
CNSTI4 2
ASGNI4
LABELV $381
line 1613
;1612:	//
;1613:	if (bs->walker > 0.5) initmove.or_moveflags |= MFL_WALK;
ADDRFP4 0
INDIRP4
CNSTI4 6056
ADDP4
INDIRF4
CNSTF4 1056964608
LEF4 $384
ADDRLP4 0+64
ADDRLP4 0+64
INDIRI4
CNSTI4 512
BORI4
ASGNI4
LABELV $384
line 1615
;1614:	//
;1615:	VectorCopy(bs->viewangles, initmove.viewangles);
ADDRLP4 0+52
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
INDIRB
ASGNB 12
line 1617
;1616:	//
;1617:	trap_BotInitMoveState(bs->ms, &initmove);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotInitMoveState
CALLV
pop
line 1618
;1618:}
LABELV $359
endproc BotSetupForMovement 88 12
export BotCheckItemPickup
proc BotCheckItemPickup 0 0
line 1625
;1619:
;1620:/*
;1621:==================
;1622:BotCheckItemPickup
;1623:==================
;1624:*/
;1625:void BotCheckItemPickup(bot_state_t *bs, int *oldinventory) {
line 1712
;1626:#ifdef MISSIONPACK
;1627:	int offence, leader;
;1628:
;1629:	if (gametype <= GT_TEAM)
;1630:		return;
;1631:
;1632:	offence = -1;
;1633:	// go into offence if picked up the kamikaze or invulnerability
;1634:	if (!oldinventory[INVENTORY_KAMIKAZE] && bs->inventory[INVENTORY_KAMIKAZE] >= 1) {
;1635:		offence = qtrue;
;1636:	}
;1637:	if (!oldinventory[INVENTORY_INVULNERABILITY] && bs->inventory[INVENTORY_INVULNERABILITY] >= 1) {
;1638:		offence = qtrue;
;1639:	}
;1640:	// if not already wearing the kamikaze or invulnerability
;1641:	if (!bs->inventory[INVENTORY_KAMIKAZE] && !bs->inventory[INVENTORY_INVULNERABILITY]) {
;1642:		if (!oldinventory[INVENTORY_SCOUT] && bs->inventory[INVENTORY_SCOUT] >= 1) {
;1643:			offence = qtrue;
;1644:		}
;1645:		if (!oldinventory[INVENTORY_GUARD] && bs->inventory[INVENTORY_GUARD] >= 1) {
;1646:			offence = qtrue;
;1647:		}
;1648:		if (!oldinventory[INVENTORY_DOUBLER] && bs->inventory[INVENTORY_DOUBLER] >= 1) {
;1649:			offence = qfalse;
;1650:		}
;1651:		if (!oldinventory[INVENTORY_AMMOREGEN] && bs->inventory[INVENTORY_AMMOREGEN] >= 1) {
;1652:			offence = qfalse;
;1653:		}
;1654:	}
;1655:
;1656:	if (offence >= 0) {
;1657:		leader = ClientFromName(bs->teamleader);
;1658:		if (offence) {
;1659:			if (!(bs->teamtaskpreference & TEAMTP_ATTACKER)) {
;1660:				// if we have a bot team leader
;1661:				if (BotTeamLeader(bs)) {
;1662:					// tell the leader we want to be on offence
;1663:					BotVoiceChat(bs, leader, VOICECHAT_WANTONOFFENSE);
;1664:					//BotAI_BotInitialChat(bs, "wantoffence", NULL);
;1665:					//trap_BotEnterChat(bs->cs, leader, CHAT_TELL);
;1666:				}
;1667:				else if (g_spSkill.integer <= 3) {
;1668:					if ( bs->ltgtype != LTG_GETFLAG &&
;1669:						 bs->ltgtype != LTG_ATTACKENEMYBASE &&
;1670:						 bs->ltgtype != LTG_HARVEST ) {
;1671:						//
;1672:						if ((gametype != GT_CTF || (bs->redflagstatus == 0 && bs->blueflagstatus == 0)) &&
;1673:							(gametype != GT_1FCTF || bs->neutralflagstatus == 0) ) {
;1674:							// tell the leader we want to be on offence
;1675:							BotVoiceChat(bs, leader, VOICECHAT_WANTONOFFENSE);
;1676:							//BotAI_BotInitialChat(bs, "wantoffence", NULL);
;1677:							//trap_BotEnterChat(bs->cs, leader, CHAT_TELL);
;1678:						}
;1679:					}
;1680:					bs->teamtaskpreference |= TEAMTP_ATTACKER;
;1681:				}
;1682:			}
;1683:			bs->teamtaskpreference &= ~TEAMTP_DEFENDER;
;1684:		}
;1685:		else {
;1686:			if (!(bs->teamtaskpreference & TEAMTP_DEFENDER)) {
;1687:				// if we have a bot team leader
;1688:				if (BotTeamLeader(bs)) {
;1689:					// tell the leader we want to be on defense
;1690:					BotVoiceChat(bs, -1, VOICECHAT_WANTONDEFENSE);
;1691:					//BotAI_BotInitialChat(bs, "wantdefence", NULL);
;1692:					//trap_BotEnterChat(bs->cs, leader, CHAT_TELL);
;1693:				}
;1694:				else if (g_spSkill.integer <= 3) {
;1695:					if ( bs->ltgtype != LTG_DEFENDKEYAREA ) {
;1696:						//
;1697:						if ((gametype != GT_CTF || (bs->redflagstatus == 0 && bs->blueflagstatus == 0)) &&
;1698:							(gametype != GT_1FCTF || bs->neutralflagstatus == 0) ) {
;1699:							// tell the leader we want to be on defense
;1700:							BotVoiceChat(bs, -1, VOICECHAT_WANTONDEFENSE);
;1701:							//BotAI_BotInitialChat(bs, "wantdefence", NULL);
;1702:							//trap_BotEnterChat(bs->cs, leader, CHAT_TELL);
;1703:						}
;1704:					}
;1705:				}
;1706:				bs->teamtaskpreference |= TEAMTP_DEFENDER;
;1707:			}
;1708:			bs->teamtaskpreference &= ~TEAMTP_ATTACKER;
;1709:		}
;1710:	}
;1711:#endif
;1712:}
LABELV $388
endproc BotCheckItemPickup 0 0
export BotUpdateInventory
proc BotUpdateInventory 1224 12
line 1719
;1713:
;1714:/*
;1715:==================
;1716:BotUpdateInventory
;1717:==================
;1718:*/
;1719:void BotUpdateInventory(bot_state_t *bs) {
line 1722
;1720:	int oldinventory[MAX_ITEMS];
;1721:
;1722:	memcpy(oldinventory, bs->inventory, sizeof(oldinventory));
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 4952
ADDP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1724
;1723:	//armor
;1724:	bs->inventory[INVENTORY_ARMOR] = bs->cur_ps.stats[STAT_ARMOR];
ADDRLP4 1024
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1024
INDIRP4
CNSTI4 4956
ADDP4
ADDRLP4 1024
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
ASGNI4
line 1726
;1725:	//weapons
;1726:	bs->inventory[INVENTORY_GAUNTLET] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_GAUNTLET)) != 0;
ADDRLP4 1032
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1032
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $391
ADDRLP4 1028
CNSTI4 1
ASGNI4
ADDRGP4 $392
JUMPV
LABELV $391
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $392
ADDRLP4 1032
INDIRP4
CNSTI4 4968
ADDP4
ADDRLP4 1028
INDIRI4
ASGNI4
line 1727
;1727:	bs->inventory[INVENTORY_SHOTGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_SHOTGUN)) != 0;
ADDRLP4 1040
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1040
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $394
ADDRLP4 1036
CNSTI4 1
ASGNI4
ADDRGP4 $395
JUMPV
LABELV $394
ADDRLP4 1036
CNSTI4 0
ASGNI4
LABELV $395
ADDRLP4 1040
INDIRP4
CNSTI4 4972
ADDP4
ADDRLP4 1036
INDIRI4
ASGNI4
line 1728
;1728:	bs->inventory[INVENTORY_MACHINEGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_MACHINEGUN)) != 0;
ADDRLP4 1048
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1048
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $397
ADDRLP4 1044
CNSTI4 1
ASGNI4
ADDRGP4 $398
JUMPV
LABELV $397
ADDRLP4 1044
CNSTI4 0
ASGNI4
LABELV $398
ADDRLP4 1048
INDIRP4
CNSTI4 4976
ADDP4
ADDRLP4 1044
INDIRI4
ASGNI4
line 1729
;1729:	bs->inventory[INVENTORY_GRENADELAUNCHER] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_GRENADE_LAUNCHER)) != 0;
ADDRLP4 1056
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1056
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $400
ADDRLP4 1052
CNSTI4 1
ASGNI4
ADDRGP4 $401
JUMPV
LABELV $400
ADDRLP4 1052
CNSTI4 0
ASGNI4
LABELV $401
ADDRLP4 1056
INDIRP4
CNSTI4 4980
ADDP4
ADDRLP4 1052
INDIRI4
ASGNI4
line 1730
;1730:	bs->inventory[INVENTORY_ROCKETLAUNCHER] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_ROCKET_LAUNCHER)) != 0;
ADDRLP4 1064
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1064
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
EQI4 $403
ADDRLP4 1060
CNSTI4 1
ASGNI4
ADDRGP4 $404
JUMPV
LABELV $403
ADDRLP4 1060
CNSTI4 0
ASGNI4
LABELV $404
ADDRLP4 1064
INDIRP4
CNSTI4 4984
ADDP4
ADDRLP4 1060
INDIRI4
ASGNI4
line 1731
;1731:	bs->inventory[INVENTORY_LIGHTNING] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_LIGHTNING)) != 0;
ADDRLP4 1072
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1072
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
EQI4 $406
ADDRLP4 1068
CNSTI4 1
ASGNI4
ADDRGP4 $407
JUMPV
LABELV $406
ADDRLP4 1068
CNSTI4 0
ASGNI4
LABELV $407
ADDRLP4 1072
INDIRP4
CNSTI4 4988
ADDP4
ADDRLP4 1068
INDIRI4
ASGNI4
line 1732
;1732:	bs->inventory[INVENTORY_RAILGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_RAILGUN)) != 0;
ADDRLP4 1080
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $409
ADDRLP4 1076
CNSTI4 1
ASGNI4
ADDRGP4 $410
JUMPV
LABELV $409
ADDRLP4 1076
CNSTI4 0
ASGNI4
LABELV $410
ADDRLP4 1080
INDIRP4
CNSTI4 4992
ADDP4
ADDRLP4 1076
INDIRI4
ASGNI4
line 1733
;1733:	bs->inventory[INVENTORY_PLASMAGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_PLASMAGUN)) != 0;
ADDRLP4 1088
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1088
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $412
ADDRLP4 1084
CNSTI4 1
ASGNI4
ADDRGP4 $413
JUMPV
LABELV $412
ADDRLP4 1084
CNSTI4 0
ASGNI4
LABELV $413
ADDRLP4 1088
INDIRP4
CNSTI4 4996
ADDP4
ADDRLP4 1084
INDIRI4
ASGNI4
line 1734
;1734:	bs->inventory[INVENTORY_BFG10K] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_BFG)) != 0;
ADDRLP4 1096
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1096
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $415
ADDRLP4 1092
CNSTI4 1
ASGNI4
ADDRGP4 $416
JUMPV
LABELV $415
ADDRLP4 1092
CNSTI4 0
ASGNI4
LABELV $416
ADDRLP4 1096
INDIRP4
CNSTI4 5004
ADDP4
ADDRLP4 1092
INDIRI4
ASGNI4
line 1735
;1735:	bs->inventory[INVENTORY_GRAPPLINGHOOK] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_GRAPPLING_HOOK)) != 0;
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1104
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $418
ADDRLP4 1100
CNSTI4 1
ASGNI4
ADDRGP4 $419
JUMPV
LABELV $418
ADDRLP4 1100
CNSTI4 0
ASGNI4
LABELV $419
ADDRLP4 1104
INDIRP4
CNSTI4 5008
ADDP4
ADDRLP4 1100
INDIRI4
ASGNI4
line 1742
;1736:#ifdef MISSIONPACK
;1737:	bs->inventory[INVENTORY_NAILGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_NAILGUN)) != 0;;
;1738:	bs->inventory[INVENTORY_PROXLAUNCHER] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_PROX_LAUNCHER)) != 0;;
;1739:	bs->inventory[INVENTORY_CHAINGUN] = (bs->cur_ps.stats[STAT_WEAPONS] & (1 << WP_CHAINGUN)) != 0;;
;1740:#endif
;1741:	//ammo
;1742:	bs->inventory[INVENTORY_SHELLS] = bs->cur_ps.ammo[WP_SHOTGUN];
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 5024
ADDP4
ADDRLP4 1108
INDIRP4
CNSTI4 404
ADDP4
INDIRI4
ASGNI4
line 1743
;1743:	bs->inventory[INVENTORY_BULLETS] = bs->cur_ps.ammo[WP_MACHINEGUN];
ADDRLP4 1112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1112
INDIRP4
CNSTI4 5028
ADDP4
ADDRLP4 1112
INDIRP4
CNSTI4 400
ADDP4
INDIRI4
ASGNI4
line 1744
;1744:	bs->inventory[INVENTORY_GRENADES] = bs->cur_ps.ammo[WP_GRENADE_LAUNCHER];
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1116
INDIRP4
CNSTI4 5032
ADDP4
ADDRLP4 1116
INDIRP4
CNSTI4 408
ADDP4
INDIRI4
ASGNI4
line 1745
;1745:	bs->inventory[INVENTORY_CELLS] = bs->cur_ps.ammo[WP_PLASMAGUN];
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
CNSTI4 5036
ADDP4
ADDRLP4 1120
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
ASGNI4
line 1746
;1746:	bs->inventory[INVENTORY_LIGHTNINGAMMO] = bs->cur_ps.ammo[WP_LIGHTNING];
ADDRLP4 1124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1124
INDIRP4
CNSTI4 5040
ADDP4
ADDRLP4 1124
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
ASGNI4
line 1747
;1747:	bs->inventory[INVENTORY_ROCKETS] = bs->cur_ps.ammo[WP_ROCKET_LAUNCHER];
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CNSTI4 5044
ADDP4
ADDRLP4 1128
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
ASGNI4
line 1748
;1748:	bs->inventory[INVENTORY_SLUGS] = bs->cur_ps.ammo[WP_RAILGUN];
ADDRLP4 1132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRP4
CNSTI4 5048
ADDP4
ADDRLP4 1132
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
ASGNI4
line 1749
;1749:	bs->inventory[INVENTORY_BFGAMMO] = bs->cur_ps.ammo[WP_BFG];
ADDRLP4 1136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1136
INDIRP4
CNSTI4 5052
ADDP4
ADDRLP4 1136
INDIRP4
CNSTI4 428
ADDP4
INDIRI4
ASGNI4
line 1756
;1750:#ifdef MISSIONPACK
;1751:	bs->inventory[INVENTORY_NAILS] = bs->cur_ps.ammo[WP_NAILGUN];
;1752:	bs->inventory[INVENTORY_MINES] = bs->cur_ps.ammo[WP_PROX_LAUNCHER];
;1753:	bs->inventory[INVENTORY_BELT] = bs->cur_ps.ammo[WP_CHAINGUN];
;1754:#endif
;1755:	//powerups
;1756:	bs->inventory[INVENTORY_HEALTH] = bs->cur_ps.stats[STAT_HEALTH];
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1140
INDIRP4
CNSTI4 5068
ADDP4
ADDRLP4 1140
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ASGNI4
line 1757
;1757:	bs->inventory[INVENTORY_TELEPORTER] = bs->cur_ps.stats[STAT_HOLDABLE_ITEM] == MODELINDEX_TELEPORTER;
ADDRLP4 1148
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1148
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 26
NEI4 $421
ADDRLP4 1144
CNSTI4 1
ASGNI4
ADDRGP4 $422
JUMPV
LABELV $421
ADDRLP4 1144
CNSTI4 0
ASGNI4
LABELV $422
ADDRLP4 1148
INDIRP4
CNSTI4 5072
ADDP4
ADDRLP4 1144
INDIRI4
ASGNI4
line 1758
;1758:	bs->inventory[INVENTORY_MEDKIT] = bs->cur_ps.stats[STAT_HOLDABLE_ITEM] == MODELINDEX_MEDKIT;
ADDRLP4 1156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1156
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 27
NEI4 $424
ADDRLP4 1152
CNSTI4 1
ASGNI4
ADDRGP4 $425
JUMPV
LABELV $424
ADDRLP4 1152
CNSTI4 0
ASGNI4
LABELV $425
ADDRLP4 1156
INDIRP4
CNSTI4 5076
ADDP4
ADDRLP4 1152
INDIRI4
ASGNI4
line 1764
;1759:#ifdef MISSIONPACK
;1760:	bs->inventory[INVENTORY_KAMIKAZE] = bs->cur_ps.stats[STAT_HOLDABLE_ITEM] == MODELINDEX_KAMIKAZE;
;1761:	bs->inventory[INVENTORY_PORTAL] = bs->cur_ps.stats[STAT_HOLDABLE_ITEM] == MODELINDEX_PORTAL;
;1762:	bs->inventory[INVENTORY_INVULNERABILITY] = bs->cur_ps.stats[STAT_HOLDABLE_ITEM] == MODELINDEX_INVULNERABILITY;
;1763:#endif
;1764:	bs->inventory[INVENTORY_QUAD] = bs->cur_ps.powerups[PW_QUAD] != 0;
ADDRLP4 1164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1164
INDIRP4
CNSTI4 332
ADDP4
INDIRI4
CNSTI4 0
EQI4 $427
ADDRLP4 1160
CNSTI4 1
ASGNI4
ADDRGP4 $428
JUMPV
LABELV $427
ADDRLP4 1160
CNSTI4 0
ASGNI4
LABELV $428
ADDRLP4 1164
INDIRP4
CNSTI4 5092
ADDP4
ADDRLP4 1160
INDIRI4
ASGNI4
line 1765
;1765:	bs->inventory[INVENTORY_ENVIRONMENTSUIT] = bs->cur_ps.powerups[PW_BATTLESUIT] != 0;
ADDRLP4 1172
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1172
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
CNSTI4 0
EQI4 $430
ADDRLP4 1168
CNSTI4 1
ASGNI4
ADDRGP4 $431
JUMPV
LABELV $430
ADDRLP4 1168
CNSTI4 0
ASGNI4
LABELV $431
ADDRLP4 1172
INDIRP4
CNSTI4 5096
ADDP4
ADDRLP4 1168
INDIRI4
ASGNI4
line 1766
;1766:	bs->inventory[INVENTORY_HASTE] = bs->cur_ps.powerups[PW_HASTE] != 0;
ADDRLP4 1180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1180
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $433
ADDRLP4 1176
CNSTI4 1
ASGNI4
ADDRGP4 $434
JUMPV
LABELV $433
ADDRLP4 1176
CNSTI4 0
ASGNI4
LABELV $434
ADDRLP4 1180
INDIRP4
CNSTI4 5100
ADDP4
ADDRLP4 1176
INDIRI4
ASGNI4
line 1767
;1767:	bs->inventory[INVENTORY_INVISIBILITY] = bs->cur_ps.powerups[PW_INVIS] != 0;
ADDRLP4 1188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1188
INDIRP4
CNSTI4 344
ADDP4
INDIRI4
CNSTI4 0
EQI4 $436
ADDRLP4 1184
CNSTI4 1
ASGNI4
ADDRGP4 $437
JUMPV
LABELV $436
ADDRLP4 1184
CNSTI4 0
ASGNI4
LABELV $437
ADDRLP4 1188
INDIRP4
CNSTI4 5104
ADDP4
ADDRLP4 1184
INDIRI4
ASGNI4
line 1768
;1768:	bs->inventory[INVENTORY_REGEN] = bs->cur_ps.powerups[PW_REGEN] != 0;
ADDRLP4 1196
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1196
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $439
ADDRLP4 1192
CNSTI4 1
ASGNI4
ADDRGP4 $440
JUMPV
LABELV $439
ADDRLP4 1192
CNSTI4 0
ASGNI4
LABELV $440
ADDRLP4 1196
INDIRP4
CNSTI4 5108
ADDP4
ADDRLP4 1192
INDIRI4
ASGNI4
line 1769
;1769:	bs->inventory[INVENTORY_FLIGHT] = bs->cur_ps.powerups[PW_FLIGHT] != 0;
ADDRLP4 1204
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1204
INDIRP4
CNSTI4 352
ADDP4
INDIRI4
CNSTI4 0
EQI4 $442
ADDRLP4 1200
CNSTI4 1
ASGNI4
ADDRGP4 $443
JUMPV
LABELV $442
ADDRLP4 1200
CNSTI4 0
ASGNI4
LABELV $443
ADDRLP4 1204
INDIRP4
CNSTI4 5112
ADDP4
ADDRLP4 1200
INDIRI4
ASGNI4
line 1776
;1770:#ifdef MISSIONPACK
;1771:	bs->inventory[INVENTORY_SCOUT] = bs->cur_ps.stats[STAT_PERSISTANT_POWERUP] == MODELINDEX_SCOUT;
;1772:	bs->inventory[INVENTORY_GUARD] = bs->cur_ps.stats[STAT_PERSISTANT_POWERUP] == MODELINDEX_GUARD;
;1773:	bs->inventory[INVENTORY_DOUBLER] = bs->cur_ps.stats[STAT_PERSISTANT_POWERUP] == MODELINDEX_DOUBLER;
;1774:	bs->inventory[INVENTORY_AMMOREGEN] = bs->cur_ps.stats[STAT_PERSISTANT_POWERUP] == MODELINDEX_AMMOREGEN;
;1775:#endif
;1776:	bs->inventory[INVENTORY_REDFLAG] = bs->cur_ps.powerups[PW_REDFLAG] != 0;
ADDRLP4 1212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1212
INDIRP4
CNSTI4 356
ADDP4
INDIRI4
CNSTI4 0
EQI4 $445
ADDRLP4 1208
CNSTI4 1
ASGNI4
ADDRGP4 $446
JUMPV
LABELV $445
ADDRLP4 1208
CNSTI4 0
ASGNI4
LABELV $446
ADDRLP4 1212
INDIRP4
CNSTI4 5132
ADDP4
ADDRLP4 1208
INDIRI4
ASGNI4
line 1777
;1777:	bs->inventory[INVENTORY_BLUEFLAG] = bs->cur_ps.powerups[PW_BLUEFLAG] != 0;
ADDRLP4 1220
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1220
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
CNSTI4 0
EQI4 $448
ADDRLP4 1216
CNSTI4 1
ASGNI4
ADDRGP4 $449
JUMPV
LABELV $448
ADDRLP4 1216
CNSTI4 0
ASGNI4
LABELV $449
ADDRLP4 1220
INDIRP4
CNSTI4 5136
ADDP4
ADDRLP4 1216
INDIRI4
ASGNI4
line 1789
;1778:#ifdef MISSIONPACK
;1779:	bs->inventory[INVENTORY_NEUTRALFLAG] = bs->cur_ps.powerups[PW_NEUTRALFLAG] != 0;
;1780:	if (BotTeam(bs) == TEAM_RED) {
;1781:		bs->inventory[INVENTORY_REDCUBE] = bs->cur_ps.generic1;
;1782:		bs->inventory[INVENTORY_BLUECUBE] = 0;
;1783:	}
;1784:	else {
;1785:		bs->inventory[INVENTORY_REDCUBE] = 0;
;1786:		bs->inventory[INVENTORY_BLUECUBE] = bs->cur_ps.generic1;
;1787:	}
;1788:#endif
;1789:	BotCheckItemPickup(bs, oldinventory);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotCheckItemPickup
CALLV
pop
line 1790
;1790:}
LABELV $389
endproc BotUpdateInventory 1224 12
export BotUpdateBattleInventory
proc BotUpdateBattleInventory 160 8
line 1797
;1791:
;1792:/*
;1793:==================
;1794:BotUpdateBattleInventory
;1795:==================
;1796:*/
;1797:void BotUpdateBattleInventory(bot_state_t *bs, int enemy) {
line 1801
;1798:	vec3_t dir;
;1799:	aas_entityinfo_t entinfo;
;1800:
;1801:	BotEntityInfo(enemy, &entinfo);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 1802
;1802:	VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12+24
INDIRF4
ADDRLP4 152
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 12+24+4
INDIRF4
ADDRLP4 152
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 12+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1803
;1803:	bs->inventory[ENEMY_HEIGHT] = (int) dir[2];
ADDRFP4 0
INDIRP4
CNSTI4 5756
ADDP4
ADDRLP4 0+8
INDIRF4
CVFI4 4
ASGNI4
line 1804
;1804:	dir[2] = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 1805
;1805:	bs->inventory[ENEMY_HORIZONTAL_DIST] = (int) VectorLength(dir);
ADDRLP4 0
ARGP4
ADDRLP4 156
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 5752
ADDP4
ADDRLP4 156
INDIRF4
CVFI4 4
ASGNI4
line 1807
;1806:	//FIXME: add num visible enemies and num visible team mates to the inventory
;1807:}
LABELV $450
endproc BotUpdateBattleInventory 160 8
export BotBattleUseItems
proc BotBattleUseItems 4 4
line 2034
;1808:
;1809:#ifdef MISSIONPACK
;1810:/*
;1811:==================
;1812:BotUseKamikaze
;1813:==================
;1814:*/
;1815:#define KAMIKAZE_DIST		1024
;1816:
;1817:void BotUseKamikaze(bot_state_t *bs) {
;1818:	int c, teammates, enemies;
;1819:	aas_entityinfo_t entinfo;
;1820:	vec3_t dir, target;
;1821:	bot_goal_t *goal;
;1822:	bsp_trace_t trace;
;1823:
;1824:	//if the bot has no kamikaze
;1825:	if (bs->inventory[INVENTORY_KAMIKAZE] <= 0)
;1826:		return;
;1827:	if (bs->kamikaze_time > FloatTime())
;1828:		return;
;1829:	bs->kamikaze_time = FloatTime() + 0.2;
;1830:	if (gametype == GT_CTF) {
;1831:		//never use kamikaze if the team flag carrier is visible
;1832:		if (BotCTFCarryingFlag(bs))
;1833:			return;
;1834:		c = BotTeamFlagCarrierVisible(bs);
;1835:		if (c >= 0) {
;1836:			BotEntityInfo(c, &entinfo);
;1837:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1838:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST))
;1839:				return;
;1840:		}
;1841:		c = BotEnemyFlagCarrierVisible(bs);
;1842:		if (c >= 0) {
;1843:			BotEntityInfo(c, &entinfo);
;1844:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1845:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST)) {
;1846:				trap_EA_Use(bs->client);
;1847:				return;
;1848:			}
;1849:		}
;1850:	}
;1851:	else if (gametype == GT_1FCTF) {
;1852:		//never use kamikaze if the team flag carrier is visible
;1853:		if (Bot1FCTFCarryingFlag(bs))
;1854:			return;
;1855:		c = BotTeamFlagCarrierVisible(bs);
;1856:		if (c >= 0) {
;1857:			BotEntityInfo(c, &entinfo);
;1858:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1859:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST))
;1860:				return;
;1861:		}
;1862:		c = BotEnemyFlagCarrierVisible(bs);
;1863:		if (c >= 0) {
;1864:			BotEntityInfo(c, &entinfo);
;1865:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1866:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST)) {
;1867:				trap_EA_Use(bs->client);
;1868:				return;
;1869:			}
;1870:		}
;1871:	}
;1872:	else if (gametype == GT_OBELISK) {
;1873:		switch(BotTeam(bs)) {
;1874:			case TEAM_RED: goal = &blueobelisk; break;
;1875:			default: goal = &redobelisk; break;
;1876:		}
;1877:		//if the obelisk is visible
;1878:		VectorCopy(goal->origin, target);
;1879:		target[2] += 1;
;1880:		VectorSubtract(bs->origin, target, dir);
;1881:		if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST * 0.9)) {
;1882:			BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;1883:			if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;1884:				trap_EA_Use(bs->client);
;1885:				return;
;1886:			}
;1887:		}
;1888:	}
;1889:	else if (gametype == GT_HARVESTER) {
;1890:		//
;1891:		if (BotHarvesterCarryingCubes(bs))
;1892:			return;
;1893:		//never use kamikaze if a team mate carrying cubes is visible
;1894:		c = BotTeamCubeCarrierVisible(bs);
;1895:		if (c >= 0) {
;1896:			BotEntityInfo(c, &entinfo);
;1897:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1898:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST))
;1899:				return;
;1900:		}
;1901:		c = BotEnemyCubeCarrierVisible(bs);
;1902:		if (c >= 0) {
;1903:			BotEntityInfo(c, &entinfo);
;1904:			VectorSubtract(entinfo.origin, bs->origin, dir);
;1905:			if (VectorLengthSquared(dir) < Square(KAMIKAZE_DIST)) {
;1906:				trap_EA_Use(bs->client);
;1907:				return;
;1908:			}
;1909:		}
;1910:	}
;1911:	//
;1912:	BotVisibleTeamMatesAndEnemies(bs, &teammates, &enemies, KAMIKAZE_DIST);
;1913:	//
;1914:	if (enemies > 2 && enemies > teammates+1) {
;1915:		trap_EA_Use(bs->client);
;1916:		return;
;1917:	}
;1918:}
;1919:
;1920:/*
;1921:==================
;1922:BotUseInvulnerability
;1923:==================
;1924:*/
;1925:void BotUseInvulnerability(bot_state_t *bs) {
;1926:	int c;
;1927:	vec3_t dir, target;
;1928:	bot_goal_t *goal;
;1929:	bsp_trace_t trace;
;1930:
;1931:	//if the bot has no invulnerability
;1932:	if (bs->inventory[INVENTORY_INVULNERABILITY] <= 0)
;1933:		return;
;1934:	if (bs->invulnerability_time > FloatTime())
;1935:		return;
;1936:	bs->invulnerability_time = FloatTime() + 0.2;
;1937:	if (gametype == GT_CTF) {
;1938:		//never use kamikaze if the team flag carrier is visible
;1939:		if (BotCTFCarryingFlag(bs))
;1940:			return;
;1941:		c = BotEnemyFlagCarrierVisible(bs);
;1942:		if (c >= 0)
;1943:			return;
;1944:		//if near enemy flag and the flag is visible
;1945:		switch(BotTeam(bs)) {
;1946:			case TEAM_RED: goal = &ctf_blueflag; break;
;1947:			default: goal = &ctf_redflag; break;
;1948:		}
;1949:		//if the obelisk is visible
;1950:		VectorCopy(goal->origin, target);
;1951:		target[2] += 1;
;1952:		VectorSubtract(bs->origin, target, dir);
;1953:		if (VectorLengthSquared(dir) < Square(200)) {
;1954:			BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;1955:			if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;1956:				trap_EA_Use(bs->client);
;1957:				return;
;1958:			}
;1959:		}
;1960:	}
;1961:	else if (gametype == GT_1FCTF) {
;1962:		//never use kamikaze if the team flag carrier is visible
;1963:		if (Bot1FCTFCarryingFlag(bs))
;1964:			return;
;1965:		c = BotEnemyFlagCarrierVisible(bs);
;1966:		if (c >= 0)
;1967:			return;
;1968:		//if near enemy flag and the flag is visible
;1969:		switch(BotTeam(bs)) {
;1970:			case TEAM_RED: goal = &ctf_blueflag; break;
;1971:			default: goal = &ctf_redflag; break;
;1972:		}
;1973:		//if the obelisk is visible
;1974:		VectorCopy(goal->origin, target);
;1975:		target[2] += 1;
;1976:		VectorSubtract(bs->origin, target, dir);
;1977:		if (VectorLengthSquared(dir) < Square(200)) {
;1978:			BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;1979:			if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;1980:				trap_EA_Use(bs->client);
;1981:				return;
;1982:			}
;1983:		}
;1984:	}
;1985:	else if (gametype == GT_OBELISK) {
;1986:		switch(BotTeam(bs)) {
;1987:			case TEAM_RED: goal = &blueobelisk; break;
;1988:			default: goal = &redobelisk; break;
;1989:		}
;1990:		//if the obelisk is visible
;1991:		VectorCopy(goal->origin, target);
;1992:		target[2] += 1;
;1993:		VectorSubtract(bs->origin, target, dir);
;1994:		if (VectorLengthSquared(dir) < Square(300)) {
;1995:			BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;1996:			if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;1997:				trap_EA_Use(bs->client);
;1998:				return;
;1999:			}
;2000:		}
;2001:	}
;2002:	else if (gametype == GT_HARVESTER) {
;2003:		//
;2004:		if (BotHarvesterCarryingCubes(bs))
;2005:			return;
;2006:		c = BotEnemyCubeCarrierVisible(bs);
;2007:		if (c >= 0)
;2008:			return;
;2009:		//if near enemy base and enemy base is visible
;2010:		switch(BotTeam(bs)) {
;2011:			case TEAM_RED: goal = &blueobelisk; break;
;2012:			default: goal = &redobelisk; break;
;2013:		}
;2014:		//if the obelisk is visible
;2015:		VectorCopy(goal->origin, target);
;2016:		target[2] += 1;
;2017:		VectorSubtract(bs->origin, target, dir);
;2018:		if (VectorLengthSquared(dir) < Square(200)) {
;2019:			BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;2020:			if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;2021:				trap_EA_Use(bs->client);
;2022:				return;
;2023:			}
;2024:		}
;2025:	}
;2026:}
;2027:#endif
;2028:
;2029:/*
;2030:==================
;2031:BotBattleUseItems
;2032:==================
;2033:*/
;2034:void BotBattleUseItems(bot_state_t *bs) {
line 2035
;2035:	if (bs->inventory[INVENTORY_HEALTH] < 40) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 40
GEI4 $461
line 2036
;2036:		if (bs->inventory[INVENTORY_TELEPORTER] > 0) {
ADDRFP4 0
INDIRP4
CNSTI4 5072
ADDP4
INDIRI4
CNSTI4 0
LEI4 $463
line 2037
;2037:			if (!BotCTFCarryingFlag(bs)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $465
line 2042
;2038:#ifdef MISSIONPACK
;2039:				&& !Bot1FCTFCarryingFlag(bs)
;2040:				&& !BotHarvesterCarryingCubes(bs)
;2041:#endif
;2042:				) {
line 2043
;2043:				trap_EA_Use(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Use
CALLV
pop
line 2044
;2044:			}
LABELV $465
line 2045
;2045:		}
LABELV $463
line 2046
;2046:	}
LABELV $461
line 2047
;2047:	if (bs->inventory[INVENTORY_HEALTH] < 60) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 60
GEI4 $467
line 2048
;2048:		if (bs->inventory[INVENTORY_MEDKIT] > 0) {
ADDRFP4 0
INDIRP4
CNSTI4 5076
ADDP4
INDIRI4
CNSTI4 0
LEI4 $469
line 2049
;2049:			trap_EA_Use(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Use
CALLV
pop
line 2050
;2050:		}
LABELV $469
line 2051
;2051:	}
LABELV $467
line 2056
;2052:#ifdef MISSIONPACK
;2053:	BotUseKamikaze(bs);
;2054:	BotUseInvulnerability(bs);
;2055:#endif
;2056:}
LABELV $460
endproc BotBattleUseItems 4 4
export BotSetTeleportTime
proc BotSetTeleportTime 8 0
line 2063
;2057:
;2058:/*
;2059:==================
;2060:BotSetTeleportTime
;2061:==================
;2062:*/
;2063:void BotSetTeleportTime(bot_state_t *bs) {
line 2064
;2064:	if ((bs->cur_ps.eFlags ^ bs->last_eFlags) & EF_TELEPORT_BIT) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 484
ADDP4
INDIRI4
BXORI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $472
line 2065
;2065:		bs->teleport_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6180
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2066
;2066:	}
LABELV $472
line 2067
;2067:	bs->last_eFlags = bs->cur_ps.eFlags;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 484
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
ASGNI4
line 2068
;2068:}
LABELV $471
endproc BotSetTeleportTime 8 0
export BotIsDead
proc BotIsDead 4 0
line 2075
;2069:
;2070:/*
;2071:==================
;2072:BotIsDead
;2073:==================
;2074:*/
;2075:qboolean BotIsDead(bot_state_t *bs) {
line 2076
;2076:	return (bs->cur_ps.pm_type == PM_DEAD);
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 3
NEI4 $476
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $477
JUMPV
LABELV $476
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $477
ADDRLP4 0
INDIRI4
RETI4
LABELV $474
endproc BotIsDead 4 0
export BotIsObserver
proc BotIsObserver 1032 12
line 2084
;2077:}
;2078:
;2079:/*
;2080:==================
;2081:BotIsObserver
;2082:==================
;2083:*/
;2084:qboolean BotIsObserver(bot_state_t *bs) {
line 2086
;2085:	char buf[MAX_INFO_STRING];
;2086:	if (bs->cur_ps.pm_type == PM_SPECTATOR) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 2
NEI4 $479
CNSTI4 1
RETI4
ADDRGP4 $478
JUMPV
LABELV $479
line 2087
;2087:	trap_GetConfigstring(CS_PLAYERS+bs->client, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 2088
;2088:	if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) return qtrue;
ADDRLP4 0
ARGP4
ADDRGP4 $67
ARGP4
ADDRLP4 1024
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1028
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 3
NEI4 $481
CNSTI4 1
RETI4
ADDRGP4 $478
JUMPV
LABELV $481
line 2089
;2089:	return qfalse;
CNSTI4 0
RETI4
LABELV $478
endproc BotIsObserver 1032 12
export BotIntermission
proc BotIntermission 8 0
line 2097
;2090:}
;2091:
;2092:/*
;2093:==================
;2094:BotIntermission
;2095:==================
;2096:*/
;2097:qboolean BotIntermission(bot_state_t *bs) {
line 2099
;2098:	//NOTE: we shouldn't be looking at the game code...
;2099:	if (level.intermissiontime) return qtrue;
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $484
CNSTI4 1
RETI4
ADDRGP4 $483
JUMPV
LABELV $484
line 2100
;2100:	return (bs->cur_ps.pm_type == PM_FREEZE || bs->cur_ps.pm_type == PM_INTERMISSION);
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 4
EQI4 $490
ADDRLP4 4
INDIRI4
CNSTI4 5
NEI4 $488
LABELV $490
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $489
JUMPV
LABELV $488
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $489
ADDRLP4 0
INDIRI4
RETI4
LABELV $483
endproc BotIntermission 8 0
export BotInLavaOrSlime
proc BotInLavaOrSlime 16 4
line 2108
;2101:}
;2102:
;2103:/*
;2104:==================
;2105:BotInLavaOrSlime
;2106:==================
;2107:*/
;2108:qboolean BotInLavaOrSlime(bot_state_t *bs) {
line 2111
;2109:	vec3_t feet;
;2110:
;2111:	VectorCopy(bs->origin, feet);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 2112
;2112:	feet[2] -= 23;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1102577664
SUBF4
ASGNF4
line 2113
;2113:	return (trap_AAS_PointContents(feet) & (CONTENTS_LAVA|CONTENTS_SLIME));
ADDRLP4 0
ARGP4
ADDRLP4 12
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 24
BANDI4
RETI4
LABELV $491
endproc BotInLavaOrSlime 16 4
lit
align 4
LABELV $494
byte 4 3238002688
byte 4 3238002688
byte 4 3238002688
align 4
LABELV $495
byte 4 1090519040
byte 4 1090519040
byte 4 1090519040
export BotCreateWayPoint
code
proc BotCreateWayPoint 32 12
line 2121
;2114:}
;2115:
;2116:/*
;2117:==================
;2118:BotCreateWayPoint
;2119:==================
;2120:*/
;2121:bot_waypoint_t *BotCreateWayPoint(char *name, vec3_t origin, int areanum) {
line 2123
;2122:	bot_waypoint_t *wp;
;2123:	vec3_t waypointmins = {-8, -8, -8}, waypointmaxs = {8, 8, 8};
ADDRLP4 4
ADDRGP4 $494
INDIRB
ASGNB 12
ADDRLP4 16
ADDRGP4 $495
INDIRB
ASGNB 12
line 2125
;2124:
;2125:	wp = botai_freewaypoints;
ADDRLP4 0
ADDRGP4 botai_freewaypoints
INDIRP4
ASGNP4
line 2126
;2126:	if ( !wp ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $496
line 2127
;2127:		BotAI_Print( PRT_WARNING, "BotCreateWayPoint: Out of waypoints\n" );
CNSTI4 2
ARGI4
ADDRGP4 $498
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 2128
;2128:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $493
JUMPV
LABELV $496
line 2130
;2129:	}
;2130:	botai_freewaypoints = botai_freewaypoints->next;
ADDRLP4 28
ADDRGP4 botai_freewaypoints
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
line 2132
;2131:
;2132:	Q_strncpyz( wp->name, name, sizeof(wp->name) );
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 2133
;2133:	VectorCopy(origin, wp->goal.origin);
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2134
;2134:	VectorCopy(waypointmins, wp->goal.mins);
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRLP4 4
INDIRB
ASGNB 12
line 2135
;2135:	VectorCopy(waypointmaxs, wp->goal.maxs);
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 16
INDIRB
ASGNB 12
line 2136
;2136:	wp->goal.areanum = areanum;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 2137
;2137:	wp->next = NULL;
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
CNSTP4 0
ASGNP4
line 2138
;2138:	wp->prev = NULL;
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
CNSTP4 0
ASGNP4
line 2139
;2139:	return wp;
ADDRLP4 0
INDIRP4
RETP4
LABELV $493
endproc BotCreateWayPoint 32 12
export BotFindWayPoint
proc BotFindWayPoint 8 8
line 2147
;2140:}
;2141:
;2142:/*
;2143:==================
;2144:BotFindWayPoint
;2145:==================
;2146:*/
;2147:bot_waypoint_t *BotFindWayPoint(bot_waypoint_t *waypoints, char *name) {
line 2150
;2148:	bot_waypoint_t *wp;
;2149:
;2150:	for (wp = waypoints; wp; wp = wp->next) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $503
JUMPV
LABELV $500
line 2151
;2151:		if (!Q_stricmp(wp->name, name)) return wp;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $504
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $499
JUMPV
LABELV $504
line 2152
;2152:	}
LABELV $501
line 2150
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
LABELV $503
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $500
line 2153
;2153:	return NULL;
CNSTP4 0
RETP4
LABELV $499
endproc BotFindWayPoint 8 8
export BotFreeWaypoints
proc BotFreeWaypoints 4 0
line 2161
;2154:}
;2155:
;2156:/*
;2157:==================
;2158:BotFreeWaypoints
;2159:==================
;2160:*/
;2161:void BotFreeWaypoints(bot_waypoint_t *wp) {
line 2164
;2162:	bot_waypoint_t *nextwp;
;2163:
;2164:	for (; wp; wp = nextwp) {
ADDRGP4 $510
JUMPV
LABELV $507
line 2165
;2165:		nextwp = wp->next;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
line 2166
;2166:		wp->next = botai_freewaypoints;
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRGP4 botai_freewaypoints
INDIRP4
ASGNP4
line 2167
;2167:		botai_freewaypoints = wp;
ADDRGP4 botai_freewaypoints
ADDRFP4 0
INDIRP4
ASGNP4
line 2168
;2168:	}
LABELV $508
line 2164
ADDRFP4 0
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $510
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $507
line 2169
;2169:}
LABELV $506
endproc BotFreeWaypoints 4 0
export BotInitWaypoints
proc BotInitWaypoints 4 0
line 2176
;2170:
;2171:/*
;2172:==================
;2173:BotInitWaypoints
;2174:==================
;2175:*/
;2176:void BotInitWaypoints(void) {
line 2179
;2177:	int i;
;2178:
;2179:	botai_freewaypoints = NULL;
ADDRGP4 botai_freewaypoints
CNSTP4 0
ASGNP4
line 2180
;2180:	for (i = 0; i < MAX_WAYPOINTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $512
line 2181
;2181:		botai_waypoints[i].next = botai_freewaypoints;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botai_waypoints+92
ADDP4
ADDRGP4 botai_freewaypoints
INDIRP4
ASGNP4
line 2182
;2182:		botai_freewaypoints = &botai_waypoints[i];
ADDRGP4 botai_freewaypoints
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botai_waypoints
ADDP4
ASGNP4
line 2183
;2183:	}
LABELV $513
line 2180
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 128
LTI4 $512
line 2184
;2184:}
LABELV $511
endproc BotInitWaypoints 4 0
export TeamPlayIsOn
proc TeamPlayIsOn 4 0
line 2191
;2185:
;2186:/*
;2187:==================
;2188:TeamPlayIsOn
;2189:==================
;2190:*/
;2191:int TeamPlayIsOn(void) {
line 2192
;2192:	return ( gametype >= GT_TEAM );
ADDRGP4 gametype
INDIRI4
CNSTI4 3
LTI4 $519
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $520
JUMPV
LABELV $519
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $520
ADDRLP4 0
INDIRI4
RETI4
LABELV $517
endproc TeamPlayIsOn 4 0
export BotAggression
proc BotAggression 28 0
line 2200
;2193:}
;2194:
;2195:/*
;2196:==================
;2197:BotAggression
;2198:==================
;2199:*/
;2200:float BotAggression(bot_state_t *bs) {
line 2202
;2201:	//if the bot has quad
;2202:	if (bs->inventory[INVENTORY_QUAD]) {
ADDRFP4 0
INDIRP4
CNSTI4 5092
ADDP4
INDIRI4
CNSTI4 0
EQI4 $522
line 2204
;2203:		//if the bot is not holding the gauntlet or the enemy is really nearby
;2204:		if (bs->weaponnum != WP_GAUNTLET ||
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
CNSTI4 1
NEI4 $526
ADDRLP4 0
INDIRP4
CNSTI4 5752
ADDP4
INDIRI4
CNSTI4 80
GEI4 $524
LABELV $526
line 2205
;2205:			bs->inventory[ENEMY_HORIZONTAL_DIST] < 80) {
line 2206
;2206:			return 70;
CNSTF4 1116471296
RETF4
ADDRGP4 $521
JUMPV
LABELV $524
line 2208
;2207:		}
;2208:	}
LABELV $522
line 2210
;2209:	//if the enemy is located way higher than the bot
;2210:	if (bs->inventory[ENEMY_HEIGHT] > 200) return 0;
ADDRFP4 0
INDIRP4
CNSTI4 5756
ADDP4
INDIRI4
CNSTI4 200
LEI4 $527
CNSTF4 0
RETF4
ADDRGP4 $521
JUMPV
LABELV $527
line 2212
;2211:	//if the bot is very low on health
;2212:	if (bs->inventory[INVENTORY_HEALTH] < 60) return 0;
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 60
GEI4 $529
CNSTF4 0
RETF4
ADDRGP4 $521
JUMPV
LABELV $529
line 2214
;2213:	//if the bot is low on health
;2214:	if (bs->inventory[INVENTORY_HEALTH] < 80) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 80
GEI4 $531
line 2216
;2215:		//if the bot has insufficient armor
;2216:		if (bs->inventory[INVENTORY_ARMOR] < 40) return 0;
ADDRFP4 0
INDIRP4
CNSTI4 4956
ADDP4
INDIRI4
CNSTI4 40
GEI4 $533
CNSTF4 0
RETF4
ADDRGP4 $521
JUMPV
LABELV $533
line 2217
;2217:	}
LABELV $531
line 2219
;2218:	//if the bot can use the bfg
;2219:	if (bs->inventory[INVENTORY_BFG10K] > 0 &&
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 5004
ADDP4
INDIRI4
CNSTI4 0
LEI4 $535
ADDRLP4 0
INDIRP4
CNSTI4 5052
ADDP4
INDIRI4
CNSTI4 7
LEI4 $535
line 2220
;2220:			bs->inventory[INVENTORY_BFGAMMO] > 7) return 100;
CNSTF4 1120403456
RETF4
ADDRGP4 $521
JUMPV
LABELV $535
line 2222
;2221:	//if the bot can use the railgun
;2222:	if (bs->inventory[INVENTORY_RAILGUN] > 0 &&
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 4992
ADDP4
INDIRI4
CNSTI4 0
LEI4 $537
ADDRLP4 4
INDIRP4
CNSTI4 5048
ADDP4
INDIRI4
CNSTI4 5
LEI4 $537
line 2223
;2223:			bs->inventory[INVENTORY_SLUGS] > 5) return 95;
CNSTF4 1119748096
RETF4
ADDRGP4 $521
JUMPV
LABELV $537
line 2225
;2224:	//if the bot can use the lightning gun
;2225:	if (bs->inventory[INVENTORY_LIGHTNING] > 0 &&
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 4988
ADDP4
INDIRI4
CNSTI4 0
LEI4 $539
ADDRLP4 8
INDIRP4
CNSTI4 5040
ADDP4
INDIRI4
CNSTI4 50
LEI4 $539
line 2226
;2226:			bs->inventory[INVENTORY_LIGHTNINGAMMO] > 50) return 90;
CNSTF4 1119092736
RETF4
ADDRGP4 $521
JUMPV
LABELV $539
line 2228
;2227:	//if the bot can use the rocketlauncher
;2228:	if (bs->inventory[INVENTORY_ROCKETLAUNCHER] > 0 &&
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
CNSTI4 0
LEI4 $541
ADDRLP4 12
INDIRP4
CNSTI4 5044
ADDP4
INDIRI4
CNSTI4 5
LEI4 $541
line 2229
;2229:			bs->inventory[INVENTORY_ROCKETS] > 5) return 90;
CNSTF4 1119092736
RETF4
ADDRGP4 $521
JUMPV
LABELV $541
line 2231
;2230:	//if the bot can use the plasmagun
;2231:	if (bs->inventory[INVENTORY_PLASMAGUN] > 0 &&
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 4996
ADDP4
INDIRI4
CNSTI4 0
LEI4 $543
ADDRLP4 16
INDIRP4
CNSTI4 5036
ADDP4
INDIRI4
CNSTI4 40
LEI4 $543
line 2232
;2232:			bs->inventory[INVENTORY_CELLS] > 40) return 85;
CNSTF4 1118437376
RETF4
ADDRGP4 $521
JUMPV
LABELV $543
line 2234
;2233:	//if the bot can use the grenade launcher
;2234:	if (bs->inventory[INVENTORY_GRENADELAUNCHER] > 0 &&
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 4980
ADDP4
INDIRI4
CNSTI4 0
LEI4 $545
ADDRLP4 20
INDIRP4
CNSTI4 5032
ADDP4
INDIRI4
CNSTI4 10
LEI4 $545
line 2235
;2235:			bs->inventory[INVENTORY_GRENADES] > 10) return 80;
CNSTF4 1117782016
RETF4
ADDRGP4 $521
JUMPV
LABELV $545
line 2237
;2236:	//if the bot can use the shotgun
;2237:	if (bs->inventory[INVENTORY_SHOTGUN] > 0 &&
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 4972
ADDP4
INDIRI4
CNSTI4 0
LEI4 $547
ADDRLP4 24
INDIRP4
CNSTI4 5024
ADDP4
INDIRI4
CNSTI4 10
LEI4 $547
line 2238
;2238:			bs->inventory[INVENTORY_SHELLS] > 10) return 50;
CNSTF4 1112014848
RETF4
ADDRGP4 $521
JUMPV
LABELV $547
line 2240
;2239:	//otherwise the bot is not feeling too good
;2240:	return 0;
CNSTF4 0
RETF4
LABELV $521
endproc BotAggression 28 0
export BotFeelingBad
proc BotFeelingBad 0 0
line 2248
;2241:}
;2242:
;2243:/*
;2244:==================
;2245:BotFeelingBad
;2246:==================
;2247:*/
;2248:float BotFeelingBad(bot_state_t *bs) {
line 2249
;2249:	if (bs->weaponnum == WP_GAUNTLET) {
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
CNSTI4 1
NEI4 $550
line 2250
;2250:		return 100;
CNSTF4 1120403456
RETF4
ADDRGP4 $549
JUMPV
LABELV $550
line 2252
;2251:	}
;2252:	if (bs->inventory[INVENTORY_HEALTH] < 40) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 40
GEI4 $552
line 2253
;2253:		return 100;
CNSTF4 1120403456
RETF4
ADDRGP4 $549
JUMPV
LABELV $552
line 2255
;2254:	}
;2255:	if (bs->weaponnum == WP_MACHINEGUN) {
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
CNSTI4 2
NEI4 $554
line 2256
;2256:		return 90;
CNSTF4 1119092736
RETF4
ADDRGP4 $549
JUMPV
LABELV $554
line 2258
;2257:	}
;2258:	if (bs->inventory[INVENTORY_HEALTH] < 60) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 60
GEI4 $556
line 2259
;2259:		return 80;
CNSTF4 1117782016
RETF4
ADDRGP4 $549
JUMPV
LABELV $556
line 2261
;2260:	}
;2261:	return 0;
CNSTF4 0
RETF4
LABELV $549
endproc BotFeelingBad 0 0
export BotWantsToRetreat
proc BotWantsToRetreat 144 8
line 2269
;2262:}
;2263:
;2264:/*
;2265:==================
;2266:BotWantsToRetreat
;2267:==================
;2268:*/
;2269:int BotWantsToRetreat(bot_state_t *bs) {
line 2272
;2270:	aas_entityinfo_t entinfo;
;2271:
;2272:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $559
line 2274
;2273:		//always retreat when carrying a CTF flag
;2274:		if (BotCTFCarryingFlag(bs))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $561
line 2275
;2275:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $558
JUMPV
LABELV $561
line 2276
;2276:	}
LABELV $559
line 2302
;2277:#ifdef MISSIONPACK
;2278:	else if (gametype == GT_1FCTF) {
;2279:		//if carrying the flag then always retreat
;2280:		if (Bot1FCTFCarryingFlag(bs))
;2281:			return qtrue;
;2282:	}
;2283:	else if (gametype == GT_OBELISK) {
;2284:		//the bots should be dedicated to attacking the enemy obelisk
;2285:		if (bs->ltgtype == LTG_ATTACKENEMYBASE) {
;2286:			if (bs->enemy != redobelisk.entitynum ||
;2287:						bs->enemy != blueobelisk.entitynum) {
;2288:				return qtrue;
;2289:			}
;2290:		}
;2291:		if (BotFeelingBad(bs) > 50) {
;2292:			return qtrue;
;2293:		}
;2294:		return qfalse;
;2295:	}
;2296:	else if (gametype == GT_HARVESTER) {
;2297:		//if carrying cubes then always retreat
;2298:		if (BotHarvesterCarryingCubes(bs)) return qtrue;
;2299:	}
;2300:#endif
;2301:	//
;2302:	if (bs->enemy >= 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
LTI4 $563
line 2304
;2303:		//if the enemy is carrying a flag
;2304:		BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2305
;2305:		if (EntityCarriesFlag(&entinfo))
ADDRLP4 0
ARGP4
ADDRLP4 140
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $565
line 2306
;2306:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $558
JUMPV
LABELV $565
line 2307
;2307:	}
LABELV $563
line 2309
;2308:	//if the bot is getting the flag
;2309:	if (bs->ltgtype == LTG_GETFLAG)
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 4
NEI4 $567
line 2310
;2310:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $558
JUMPV
LABELV $567
line 2312
;2311:	//
;2312:	if (BotAggression(bs) < 50)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotAggression
CALLF4
ASGNF4
ADDRLP4 140
INDIRF4
CNSTF4 1112014848
GEF4 $569
line 2313
;2313:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $558
JUMPV
LABELV $569
line 2314
;2314:	return qfalse;
CNSTI4 0
RETI4
LABELV $558
endproc BotWantsToRetreat 144 8
export BotWantsToChase
proc BotWantsToChase 148 8
line 2322
;2315:}
;2316:
;2317:/*
;2318:==================
;2319:BotWantsToChase
;2320:==================
;2321:*/
;2322:int BotWantsToChase(bot_state_t *bs) {
line 2325
;2323:	aas_entityinfo_t entinfo;
;2324:
;2325:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $572
line 2327
;2326:		//never chase when carrying a CTF flag
;2327:		if (BotCTFCarryingFlag(bs))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $574
line 2328
;2328:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $571
JUMPV
LABELV $574
line 2330
;2329:		//always chase if the enemy is carrying a flag
;2330:		BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2331
;2331:		if (EntityCarriesFlag(&entinfo))
ADDRLP4 0
ARGP4
ADDRLP4 144
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
EQI4 $576
line 2332
;2332:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $571
JUMPV
LABELV $576
line 2333
;2333:	}
LABELV $572
line 2360
;2334:#ifdef MISSIONPACK
;2335:	else if (gametype == GT_1FCTF) {
;2336:		//never chase if carrying the flag
;2337:		if (Bot1FCTFCarryingFlag(bs))
;2338:			return qfalse;
;2339:		//always chase if the enemy is carrying a flag
;2340:		BotEntityInfo(bs->enemy, &entinfo);
;2341:		if (EntityCarriesFlag(&entinfo))
;2342:			return qtrue;
;2343:	}
;2344:	else if (gametype == GT_OBELISK) {
;2345:		//the bots should be dedicated to attacking the enemy obelisk
;2346:		if (bs->ltgtype == LTG_ATTACKENEMYBASE) {
;2347:			if (bs->enemy != redobelisk.entitynum ||
;2348:						bs->enemy != blueobelisk.entitynum) {
;2349:				return qfalse;
;2350:			}
;2351:		}
;2352:	}
;2353:	else if (gametype == GT_HARVESTER) {
;2354:		//never chase if carrying cubes
;2355:		if (BotHarvesterCarryingCubes(bs))
;2356:			return qfalse;
;2357:	}
;2358:#endif
;2359:	//if the bot is getting the flag
;2360:	if (bs->ltgtype == LTG_GETFLAG)
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 4
NEI4 $578
line 2361
;2361:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $571
JUMPV
LABELV $578
line 2363
;2362:	//
;2363:	if (BotAggression(bs) > 50)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotAggression
CALLF4
ASGNF4
ADDRLP4 140
INDIRF4
CNSTF4 1112014848
LEF4 $580
line 2364
;2364:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $571
JUMPV
LABELV $580
line 2365
;2365:	return qfalse;
CNSTI4 0
RETI4
LABELV $571
endproc BotWantsToChase 148 8
export BotWantsToHelp
proc BotWantsToHelp 0 0
line 2373
;2366:}
;2367:
;2368:/*
;2369:==================
;2370:BotWantsToHelp
;2371:==================
;2372:*/
;2373:int BotWantsToHelp(bot_state_t *bs) {
line 2374
;2374:	return qtrue;
CNSTI4 1
RETI4
LABELV $582
endproc BotWantsToHelp 0 0
export BotCanAndWantsToRocketJump
proc BotCanAndWantsToRocketJump 8 16
line 2382
;2375:}
;2376:
;2377:/*
;2378:==================
;2379:BotCanAndWantsToRocketJump
;2380:==================
;2381:*/
;2382:int BotCanAndWantsToRocketJump(bot_state_t *bs) {
line 2386
;2383:	float rocketjumper;
;2384:
;2385:	//if rocket jumping is disabled
;2386:	if (!bot_rocketjump.integer) return qfalse;
ADDRGP4 bot_rocketjump+12
INDIRI4
CNSTI4 0
NEI4 $584
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $584
line 2388
;2387:	//if no rocket launcher
;2388:	if (bs->inventory[INVENTORY_ROCKETLAUNCHER] <= 0) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
CNSTI4 0
GTI4 $587
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $587
line 2390
;2389:	//if low on rockets
;2390:	if (bs->inventory[INVENTORY_ROCKETS] < 3) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 5044
ADDP4
INDIRI4
CNSTI4 3
GEI4 $589
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $589
line 2392
;2391:	//never rocket jump with the Quad
;2392:	if (bs->inventory[INVENTORY_QUAD]) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 5092
ADDP4
INDIRI4
CNSTI4 0
EQI4 $591
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $591
line 2394
;2393:	//if low on health
;2394:	if (bs->inventory[INVENTORY_HEALTH] < 60) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 60
GEI4 $593
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $593
line 2396
;2395:	//if not full health
;2396:	if (bs->inventory[INVENTORY_HEALTH] < 90) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 90
GEI4 $595
line 2398
;2397:		//if the bot has insufficient armor
;2398:		if (bs->inventory[INVENTORY_ARMOR] < 40) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 4956
ADDP4
INDIRI4
CNSTI4 40
GEI4 $597
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $597
line 2399
;2399:	}
LABELV $595
line 2400
;2400:	rocketjumper = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_WEAPONJUMPING, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 38
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 4
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
ASGNF4
line 2401
;2401:	if (rocketjumper < 0.5) return qfalse;
ADDRLP4 0
INDIRF4
CNSTF4 1056964608
GEF4 $599
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $599
line 2402
;2402:	return qtrue;
CNSTI4 1
RETI4
LABELV $583
endproc BotCanAndWantsToRocketJump 8 16
export BotHasPersistantPowerupAndWeapon
proc BotHasPersistantPowerupAndWeapon 32 0
line 2410
;2403:}
;2404:
;2405:/*
;2406:==================
;2407:BotHasPersistantPowerupAndWeapon
;2408:==================
;2409:*/
;2410:int BotHasPersistantPowerupAndWeapon(bot_state_t *bs) {
line 2421
;2411:#ifdef MISSIONPACK
;2412:	// if the bot does not have a persistant powerup
;2413:	if (!bs->inventory[INVENTORY_SCOUT] &&
;2414:		!bs->inventory[INVENTORY_GUARD] &&
;2415:		!bs->inventory[INVENTORY_DOUBLER] &&
;2416:		!bs->inventory[INVENTORY_AMMOREGEN] ) {
;2417:		return qfalse;
;2418:	}
;2419:#endif
;2420:	//if the bot is very low on health
;2421:	if (bs->inventory[INVENTORY_HEALTH] < 60) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 60
GEI4 $602
CNSTI4 0
RETI4
ADDRGP4 $601
JUMPV
LABELV $602
line 2423
;2422:	//if the bot is low on health
;2423:	if (bs->inventory[INVENTORY_HEALTH] < 80) {
ADDRFP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
CNSTI4 80
GEI4 $604
line 2425
;2424:		//if the bot has insufficient armor
;2425:		if (bs->inventory[INVENTORY_ARMOR] < 40) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 4956
ADDP4
INDIRI4
CNSTI4 40
GEI4 $606
CNSTI4 0
RETI4
ADDRGP4 $601
JUMPV
LABELV $606
line 2426
;2426:	}
LABELV $604
line 2428
;2427:	//if the bot can use the bfg
;2428:	if (bs->inventory[INVENTORY_BFG10K] > 0 &&
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 5004
ADDP4
INDIRI4
CNSTI4 0
LEI4 $608
ADDRLP4 0
INDIRP4
CNSTI4 5052
ADDP4
INDIRI4
CNSTI4 7
LEI4 $608
line 2429
;2429:			bs->inventory[INVENTORY_BFGAMMO] > 7) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $608
line 2431
;2430:	//if the bot can use the railgun
;2431:	if (bs->inventory[INVENTORY_RAILGUN] > 0 &&
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 4992
ADDP4
INDIRI4
CNSTI4 0
LEI4 $610
ADDRLP4 4
INDIRP4
CNSTI4 5048
ADDP4
INDIRI4
CNSTI4 5
LEI4 $610
line 2432
;2432:			bs->inventory[INVENTORY_SLUGS] > 5) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $610
line 2434
;2433:	//if the bot can use the lightning gun
;2434:	if (bs->inventory[INVENTORY_LIGHTNING] > 0 &&
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 4988
ADDP4
INDIRI4
CNSTI4 0
LEI4 $612
ADDRLP4 8
INDIRP4
CNSTI4 5040
ADDP4
INDIRI4
CNSTI4 50
LEI4 $612
line 2435
;2435:			bs->inventory[INVENTORY_LIGHTNINGAMMO] > 50) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $612
line 2437
;2436:	//if the bot can use the rocketlauncher
;2437:	if (bs->inventory[INVENTORY_ROCKETLAUNCHER] > 0 &&
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
CNSTI4 0
LEI4 $614
ADDRLP4 12
INDIRP4
CNSTI4 5044
ADDP4
INDIRI4
CNSTI4 5
LEI4 $614
line 2438
;2438:			bs->inventory[INVENTORY_ROCKETS] > 5) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $614
line 2440
;2439:	//
;2440:	if (bs->inventory[INVENTORY_NAILGUN] > 0 &&
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 5012
ADDP4
INDIRI4
CNSTI4 0
LEI4 $616
ADDRLP4 16
INDIRP4
CNSTI4 5056
ADDP4
INDIRI4
CNSTI4 5
LEI4 $616
line 2441
;2441:			bs->inventory[INVENTORY_NAILS] > 5) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $616
line 2443
;2442:	//
;2443:	if (bs->inventory[INVENTORY_PROXLAUNCHER] > 0 &&
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 5016
ADDP4
INDIRI4
CNSTI4 0
LEI4 $618
ADDRLP4 20
INDIRP4
CNSTI4 5060
ADDP4
INDIRI4
CNSTI4 5
LEI4 $618
line 2444
;2444:			bs->inventory[INVENTORY_MINES] > 5) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $618
line 2446
;2445:	//
;2446:	if (bs->inventory[INVENTORY_CHAINGUN] > 0 &&
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 5020
ADDP4
INDIRI4
CNSTI4 0
LEI4 $620
ADDRLP4 24
INDIRP4
CNSTI4 5064
ADDP4
INDIRI4
CNSTI4 40
LEI4 $620
line 2447
;2447:			bs->inventory[INVENTORY_BELT] > 40) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $620
line 2449
;2448:	//if the bot can use the plasmagun
;2449:	if (bs->inventory[INVENTORY_PLASMAGUN] > 0 &&
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 4996
ADDP4
INDIRI4
CNSTI4 0
LEI4 $622
ADDRLP4 28
INDIRP4
CNSTI4 5036
ADDP4
INDIRI4
CNSTI4 20
LEI4 $622
line 2450
;2450:			bs->inventory[INVENTORY_CELLS] > 20) return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $622
line 2451
;2451:	return qfalse;
CNSTI4 0
RETI4
LABELV $601
endproc BotHasPersistantPowerupAndWeapon 32 0
export BotGoCamp
proc BotGoCamp 16 16
line 2459
;2452:}
;2453:
;2454:/*
;2455:==================
;2456:BotGoCamp
;2457:==================
;2458:*/
;2459:void BotGoCamp(bot_state_t *bs, bot_goal_t *goal) {
line 2462
;2460:	float camper;
;2461:
;2462:	bs->decisionmaker = bs->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 2464
;2463:	//set message time to zero so bot will NOT show any message
;2464:	bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 2466
;2465:	//set the ltg type
;2466:	bs->ltgtype = LTG_CAMP;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 7
ASGNI4
line 2468
;2467:	//set the team goal
;2468:	memcpy(&bs->teamgoal, goal, sizeof(bot_goal_t));
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 2470
;2469:	//get the team goal time
;2470:	camper = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CAMPER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 44
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 8
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 8
INDIRF4
ASGNF4
line 2471
;2471:	if (camper > 0.99) bs->teamgoal_time = FloatTime() + 99999;
ADDRLP4 0
INDIRF4
CNSTF4 1065185444
LEF4 $625
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1203982208
ADDF4
ASGNF4
ADDRGP4 $626
JUMPV
LABELV $625
line 2472
;2472:	else bs->teamgoal_time = FloatTime() + 120 + 180 * camper + random() * 15;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1123024896
ADDF4
CNSTF4 1127481344
ADDRLP4 0
INDIRF4
MULF4
ADDF4
CNSTF4 1097859072
ADDRLP4 12
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
LABELV $626
line 2474
;2473:	//set the last time the bot started camping
;2474:	bs->camp_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6184
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2476
;2475:	//the teammate that requested the camping
;2476:	bs->teammate = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
CNSTI4 0
ASGNI4
line 2478
;2477:	//do NOT type arrive message
;2478:	bs->arrive_time = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 1065353216
ASGNF4
line 2479
;2479:}
LABELV $624
endproc BotGoCamp 16 16
export BotWantsToCamp
proc BotWantsToCamp 176 16
line 2486
;2480:
;2481:/*
;2482:==================
;2483:BotWantsToCamp
;2484:==================
;2485:*/
;2486:int BotWantsToCamp(bot_state_t *bs) {
line 2491
;2487:	float camper;
;2488:	int cs, traveltime, besttraveltime;
;2489:	bot_goal_t goal, bestgoal;
;2490:
;2491:	camper = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CAMPER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 44
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 128
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 124
ADDRLP4 128
INDIRF4
ASGNF4
line 2492
;2492:	if (camper < 0.1) return qfalse;
ADDRLP4 124
INDIRF4
CNSTF4 1036831949
GEF4 $628
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $628
line 2494
;2493:	//if the bot has a team goal
;2494:	if (bs->ltgtype == LTG_TEAMHELP ||
ADDRLP4 132
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 1
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 2
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 3
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 4
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 5
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 7
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 8
EQI4 $638
ADDRLP4 132
INDIRI4
CNSTI4 9
NEI4 $630
LABELV $638
line 2501
;2495:			bs->ltgtype == LTG_TEAMACCOMPANY ||
;2496:			bs->ltgtype == LTG_DEFENDKEYAREA ||
;2497:			bs->ltgtype == LTG_GETFLAG ||
;2498:			bs->ltgtype == LTG_RUSHBASE ||
;2499:			bs->ltgtype == LTG_CAMP ||
;2500:			bs->ltgtype == LTG_CAMPORDER ||
;2501:			bs->ltgtype == LTG_PATROL) {
line 2502
;2502:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $630
line 2505
;2503:	}
;2504:	//if camped recently
;2505:	if (bs->camp_time > FloatTime() - 60 + 300 * (1-camper)) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6184
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1114636288
SUBF4
CNSTF4 1133903872
CNSTF4 1065353216
ADDRLP4 124
INDIRF4
SUBF4
MULF4
ADDF4
LEF4 $639
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $639
line 2507
;2506:	//
;2507:	if (random() > camper) {
ADDRLP4 136
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 124
INDIRF4
LEF4 $641
line 2508
;2508:		bs->camp_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6184
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2509
;2509:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $641
line 2512
;2510:	}
;2511:	//if the bot isn't healthy anough
;2512:	if (BotAggression(bs) < 50) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotAggression
CALLF4
ASGNF4
ADDRLP4 140
INDIRF4
CNSTF4 1112014848
GEF4 $643
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $643
line 2514
;2513:	//the bot should have at least have the rocket launcher, the railgun or the bfg10k with some ammo
;2514:	if ((bs->inventory[INVENTORY_ROCKETLAUNCHER] <= 0 || bs->inventory[INVENTORY_ROCKETS < 10]) &&
ADDRLP4 144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 148
CNSTI4 0
ASGNI4
ADDRLP4 144
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
ADDRLP4 148
INDIRI4
LEI4 $647
ADDRLP4 144
INDIRP4
CNSTI4 4952
ADDP4
INDIRI4
ADDRLP4 148
INDIRI4
EQI4 $645
LABELV $647
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 152
INDIRP4
CNSTI4 4992
ADDP4
INDIRI4
CNSTI4 0
LEI4 $648
ADDRLP4 152
INDIRP4
CNSTI4 5048
ADDP4
INDIRI4
CNSTI4 10
GEI4 $645
LABELV $648
ADDRLP4 156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 156
INDIRP4
CNSTI4 5004
ADDP4
INDIRI4
CNSTI4 0
LEI4 $649
ADDRLP4 156
INDIRP4
CNSTI4 5052
ADDP4
INDIRI4
CNSTI4 10
GEI4 $645
LABELV $649
line 2516
;2515:		(bs->inventory[INVENTORY_RAILGUN] <= 0 || bs->inventory[INVENTORY_SLUGS] < 10) &&
;2516:		(bs->inventory[INVENTORY_BFG10K] <= 0 || bs->inventory[INVENTORY_BFGAMMO] < 10)) {
line 2517
;2517:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $645
line 2520
;2518:	}
;2519:	//find the closest camp spot
;2520:	besttraveltime = 99999;
ADDRLP4 64
CNSTI4 99999
ASGNI4
line 2521
;2521:	for (cs = trap_BotGetNextCampSpotGoal(0, &goal); cs; cs = trap_BotGetNextCampSpotGoal(cs, &goal)) {
CNSTI4 0
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 160
ADDRGP4 trap_BotGetNextCampSpotGoal
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 160
INDIRI4
ASGNI4
ADDRGP4 $653
JUMPV
LABELV $650
line 2522
;2522:		traveltime = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, goal.areanum, TFL_DEFAULT);
ADDRLP4 164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 164
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 164
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 8+12
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 168
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 168
INDIRI4
ASGNI4
line 2523
;2523:		if (traveltime && traveltime < besttraveltime) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $655
ADDRLP4 0
INDIRI4
ADDRLP4 64
INDIRI4
GEI4 $655
line 2524
;2524:			besttraveltime = traveltime;
ADDRLP4 64
ADDRLP4 0
INDIRI4
ASGNI4
line 2525
;2525:			memcpy(&bestgoal, &goal, sizeof(bot_goal_t));
ADDRLP4 68
ARGP4
ADDRLP4 8
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 2526
;2526:		}
LABELV $655
line 2527
;2527:	}
LABELV $651
line 2521
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 164
ADDRGP4 trap_BotGetNextCampSpotGoal
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 164
INDIRI4
ASGNI4
LABELV $653
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $650
line 2528
;2528:	if (besttraveltime > 150) return qfalse;
ADDRLP4 64
INDIRI4
CNSTI4 150
LEI4 $657
CNSTI4 0
RETI4
ADDRGP4 $627
JUMPV
LABELV $657
line 2530
;2529:	//ok found a camp spot, go camp there
;2530:	BotGoCamp(bs, &bestgoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ARGP4
ADDRGP4 BotGoCamp
CALLV
pop
line 2531
;2531:	bs->ordered = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 0
ASGNI4
line 2533
;2532:	//
;2533:	return qtrue;
CNSTI4 1
RETI4
LABELV $627
endproc BotWantsToCamp 176 16
export BotDontAvoid
proc BotDontAvoid 68 12
line 2541
;2534:}
;2535:
;2536:/*
;2537:==================
;2538:BotDontAvoid
;2539:==================
;2540:*/
;2541:void BotDontAvoid(bot_state_t *bs, char *itemname) {
line 2545
;2542:	bot_goal_t goal;
;2543:	int num;
;2544:
;2545:	num = trap_BotGetLevelItemGoal(-1, itemname, &goal);
CNSTI4 -1
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 60
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 60
INDIRI4
ASGNI4
ADDRGP4 $661
JUMPV
LABELV $660
line 2546
;2546:	while(num >= 0) {
line 2547
;2547:		trap_BotRemoveFromAvoidGoals(bs->gs, goal.number);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 4+44
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveFromAvoidGoals
CALLV
pop
line 2548
;2548:		num = trap_BotGetLevelItemGoal(num, itemname, &goal);
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 64
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 64
INDIRI4
ASGNI4
line 2549
;2549:	}
LABELV $661
line 2546
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $660
line 2550
;2550:}
LABELV $659
endproc BotDontAvoid 68 12
export BotGoForPowerups
proc BotGoForPowerups 0 8
line 2557
;2551:
;2552:/*
;2553:==================
;2554:BotGoForPowerups
;2555:==================
;2556:*/
;2557:void BotGoForPowerups(bot_state_t *bs) {
line 2560
;2558:
;2559:	//don't avoid any of the powerups anymore
;2560:	BotDontAvoid(bs, "Quad Damage");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $665
ARGP4
ADDRGP4 BotDontAvoid
CALLV
pop
line 2561
;2561:	BotDontAvoid(bs, "Regeneration");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $666
ARGP4
ADDRGP4 BotDontAvoid
CALLV
pop
line 2562
;2562:	BotDontAvoid(bs, "Battle Suit");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $667
ARGP4
ADDRGP4 BotDontAvoid
CALLV
pop
line 2563
;2563:	BotDontAvoid(bs, "Speed");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $668
ARGP4
ADDRGP4 BotDontAvoid
CALLV
pop
line 2564
;2564:	BotDontAvoid(bs, "Invisibility");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $669
ARGP4
ADDRGP4 BotDontAvoid
CALLV
pop
line 2568
;2565:	//BotDontAvoid(bs, "Flight");
;2566:	//reset the long term goal time so the bot will go for the powerup
;2567:	//NOTE: the long term goal type doesn't change
;2568:	bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 2569
;2569:}
LABELV $664
endproc BotGoForPowerups 0 8
export BotRoamGoal
proc BotRoamGoal 180 28
line 2576
;2570:
;2571:/*
;2572:==================
;2573:BotRoamGoal
;2574:==================
;2575:*/
;2576:void BotRoamGoal(bot_state_t *bs, vec3_t goal) {
line 2582
;2577:	int pc, i;
;2578:	float len, rnd;
;2579:	vec3_t dir, bestorg, belowbestorg;
;2580:	bsp_trace_t trace;
;2581:
;2582:	for (i = 0; i < 10; i++) {
ADDRLP4 116
CNSTI4 0
ASGNI4
LABELV $671
line 2584
;2583:		//start at the bot origin
;2584:		VectorCopy(bs->origin, bestorg);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 2585
;2585:		rnd = random();
ADDRLP4 136
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 112
ADDRLP4 136
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ASGNF4
line 2586
;2586:		if (rnd > 0.25) {
ADDRLP4 112
INDIRF4
CNSTF4 1048576000
LEF4 $675
line 2588
;2587:			//add a random value to the x-coordinate
;2588:			if (random() < 0.5) bestorg[0] -= 800 * random() + 100;
ADDRLP4 140
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
GEF4 $677
ADDRLP4 144
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRF4
CNSTF4 1145569280
ADDRLP4 144
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1120403456
ADDF4
SUBF4
ASGNF4
ADDRGP4 $678
JUMPV
LABELV $677
line 2589
;2589:			else bestorg[0] += 800 * random() + 100;
ADDRLP4 148
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRF4
CNSTF4 1145569280
ADDRLP4 148
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1120403456
ADDF4
ADDF4
ASGNF4
LABELV $678
line 2590
;2590:		}
LABELV $675
line 2591
;2591:		if (rnd < 0.75) {
ADDRLP4 112
INDIRF4
CNSTF4 1061158912
GEF4 $679
line 2593
;2592:			//add a random value to the y-coordinate
;2593:			if (random() < 0.5) bestorg[1] -= 800 * random() + 100;
ADDRLP4 140
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
GEF4 $681
ADDRLP4 144
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
CNSTF4 1145569280
ADDRLP4 144
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1120403456
ADDF4
SUBF4
ASGNF4
ADDRGP4 $682
JUMPV
LABELV $681
line 2594
;2594:			else bestorg[1] += 800 * random() + 100;
ADDRLP4 148
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
CNSTF4 1145569280
ADDRLP4 148
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1120403456
ADDF4
ADDF4
ASGNF4
LABELV $682
line 2595
;2595:		}
LABELV $679
line 2597
;2596:		//add a random value to the z-coordinate (NOTE: 48 = maxjump?)
;2597:		bestorg[2] += 2 * 48 * crandom();
ADDRLP4 140
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
CNSTF4 1119879168
CNSTF4 1073741824
ADDRLP4 140
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
ASGNF4
line 2599
;2598:		//trace a line from the origin to the roam target
;2599:		BotAI_Trace(&trace, bs->origin, NULL, NULL, bestorg, bs->entitynum, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRLP4 144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 144
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 148
CNSTP4 0
ASGNP4
ADDRLP4 148
INDIRP4
ARGP4
ADDRLP4 148
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 144
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2601
;2600:		//direction and length towards the roam target
;2601:		VectorSubtract(trace.endpos, bs->origin, dir);
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 24+12
INDIRF4
ADDRLP4 152
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 24+12+4
INDIRF4
ADDRLP4 152
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 24+12+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2602
;2602:		len = VectorNormalize(dir);
ADDRLP4 0
ARGP4
ADDRLP4 156
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 108
ADDRLP4 156
INDIRF4
ASGNF4
line 2604
;2603:		//if the roam target is far away anough
;2604:		if (len > 200) {
ADDRLP4 108
INDIRF4
CNSTF4 1128792064
LEF4 $693
line 2606
;2605:			//the roam target is in the given direction before walls
;2606:			VectorScale(dir, len * trace.fraction - 40, dir);
ADDRLP4 164
CNSTF4 1109393408
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 24+8
INDIRF4
MULF4
ADDRLP4 164
INDIRF4
SUBF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 24+8
INDIRF4
MULF4
ADDRLP4 164
INDIRF4
SUBF4
MULF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 24+8
INDIRF4
MULF4
CNSTF4 1109393408
SUBF4
MULF4
ASGNF4
line 2607
;2607:			VectorAdd(bs->origin, dir, bestorg);
ADDRLP4 168
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 168
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 168
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+8
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
ADDF4
ASGNF4
line 2609
;2608:			//get the coordinates of the floor below the roam target
;2609:			belowbestorg[0] = bestorg[0];
ADDRLP4 120
ADDRLP4 12
INDIRF4
ASGNF4
line 2610
;2610:			belowbestorg[1] = bestorg[1];
ADDRLP4 120+4
ADDRLP4 12+4
INDIRF4
ASGNF4
line 2611
;2611:			belowbestorg[2] = bestorg[2] - 800;
ADDRLP4 120+8
ADDRLP4 12+8
INDIRF4
CNSTF4 1145569280
SUBF4
ASGNF4
line 2612
;2612:			BotAI_Trace(&trace, bestorg, NULL, NULL, belowbestorg, bs->entitynum, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 172
CNSTP4 0
ASGNP4
ADDRLP4 172
INDIRP4
ARGP4
ADDRLP4 172
INDIRP4
ARGP4
ADDRLP4 120
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2614
;2613:			//
;2614:			if (!trace.startsolid) {
ADDRLP4 24+4
INDIRI4
CNSTI4 0
NEI4 $710
line 2615
;2615:				trace.endpos[2]++;
ADDRLP4 24+12+8
ADDRLP4 24+12+8
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2616
;2616:				pc = trap_PointContents(trace.endpos, bs->entitynum);
ADDRLP4 24+12
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 176
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 132
ADDRLP4 176
INDIRI4
ASGNI4
line 2617
;2617:				if (!(pc & (CONTENTS_LAVA | CONTENTS_SLIME))) {
ADDRLP4 132
INDIRI4
CNSTI4 24
BANDI4
CNSTI4 0
NEI4 $716
line 2618
;2618:					VectorCopy(bestorg, goal);
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRB
ASGNB 12
line 2619
;2619:					return;
ADDRGP4 $670
JUMPV
LABELV $716
line 2621
;2620:				}
;2621:			}
LABELV $710
line 2622
;2622:		}
LABELV $693
line 2623
;2623:	}
LABELV $672
line 2582
ADDRLP4 116
ADDRLP4 116
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 116
INDIRI4
CNSTI4 10
LTI4 $671
line 2624
;2624:	VectorCopy(bestorg, goal);
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRB
ASGNB 12
line 2625
;2625:}
LABELV $670
endproc BotRoamGoal 180 28
lit
align 4
LABELV $720
byte 4 0
byte 4 0
byte 4 1065353216
export BotAttackMove
code
proc BotAttackMove 392 16
line 2632
;2626:
;2627:/*
;2628:==================
;2629:BotAttackMove
;2630:==================
;2631:*/
;2632:bot_moveresult_t BotAttackMove(bot_state_t *bs, int tfl) {
line 2636
;2633:	int movetype, i, attackentity;
;2634:	float attack_skill, jumper, croucher, dist, strafechange_time;
;2635:	float attack_dist, attack_range;
;2636:	vec3_t forward, backward, sideward, hordir, up = {0, 0, 1};
ADDRLP4 56
ADDRGP4 $720
INDIRB
ASGNB 12
line 2641
;2637:	aas_entityinfo_t entinfo;
;2638:	bot_moveresult_t moveresult;
;2639:	bot_goal_t goal;
;2640:
;2641:	attackentity = bs->enemy;
ADDRLP4 332
ADDRFP4 4
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ASGNI4
line 2643
;2642:	//
;2643:	if (bs->attackchase_time > FloatTime()) {
ADDRFP4 4
INDIRP4
CNSTI4 6124
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $721
line 2645
;2644:		//create the chase goal
;2645:		goal.entitynum = attackentity;
ADDRLP4 136+40
ADDRLP4 332
INDIRI4
ASGNI4
line 2646
;2646:		goal.areanum = bs->lastenemyareanum;
ADDRLP4 136+12
ADDRFP4 4
INDIRP4
CNSTI4 6544
ADDP4
INDIRI4
ASGNI4
line 2647
;2647:		VectorCopy(bs->lastenemyorigin, goal.origin);
ADDRLP4 136
ADDRFP4 4
INDIRP4
CNSTI4 6548
ADDP4
INDIRB
ASGNB 12
line 2648
;2648:		VectorSet(goal.mins, -8, -8, -8);
ADDRLP4 136+16
CNSTF4 3238002688
ASGNF4
ADDRLP4 136+16+4
CNSTF4 3238002688
ASGNF4
ADDRLP4 136+16+8
CNSTF4 3238002688
ASGNF4
line 2649
;2649:		VectorSet(goal.maxs, 8, 8, 8);
ADDRLP4 136+28
CNSTF4 1090519040
ASGNF4
ADDRLP4 136+28+4
CNSTF4 1090519040
ASGNF4
ADDRLP4 136+28+8
CNSTF4 1090519040
ASGNF4
line 2651
;2650:		//initialize the movement state
;2651:		BotSetupForMovement(bs);
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 2653
;2652:		//move towards the goal
;2653:		trap_BotMoveToGoal(&moveresult, bs->ms, &goal, tfl);
ADDRLP4 80
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 136
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 2654
;2654:		return moveresult;
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $721
line 2657
;2655:	}
;2656:	//
;2657:	memset(&moveresult, 0, sizeof(bot_moveresult_t));
ADDRLP4 80
ARGP4
CNSTI4 0
ARGI4
CNSTI4 52
ARGI4
ADDRGP4 memset
CALLP4
pop
line 2659
;2658:	//
;2659:	attack_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_ATTACK_SKILL, 0, 1);
ADDRFP4 4
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 348
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 132
ADDRLP4 348
INDIRF4
ASGNF4
line 2660
;2660:	jumper = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_JUMPER, 0, 1);
ADDRFP4 4
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 37
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 352
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 340
ADDRLP4 352
INDIRF4
ASGNF4
line 2661
;2661:	croucher = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CROUCHER, 0, 1);
ADDRFP4 4
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 36
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 356
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 344
ADDRLP4 356
INDIRF4
ASGNF4
line 2663
;2662:	//if the bot is really stupid
;2663:	if (attack_skill < 0.2) return moveresult;
ADDRLP4 132
INDIRF4
CNSTF4 1045220557
GEF4 $735
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $735
line 2665
;2664:	//initialize the movement state
;2665:	BotSetupForMovement(bs);
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 2667
;2666:	//get the enemy entity info
;2667:	BotEntityInfo(attackentity, &entinfo);
ADDRLP4 332
INDIRI4
ARGI4
ADDRLP4 192
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2669
;2668:	//direction towards the enemy
;2669:	VectorSubtract(entinfo.origin, bs->origin, forward);
ADDRLP4 360
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 192+24
INDIRF4
ADDRLP4 360
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 24+4
ADDRLP4 192+24+4
INDIRF4
ADDRLP4 360
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 24+8
ADDRLP4 192+24+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2671
;2670:	//the distance towards the enemy
;2671:	dist = VectorNormalize(forward);
ADDRLP4 24
ARGP4
ADDRLP4 364
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 68
ADDRLP4 364
INDIRF4
ASGNF4
line 2672
;2672:	VectorNegate(forward, backward);
ADDRLP4 36
ADDRLP4 24
INDIRF4
NEGF4
ASGNF4
ADDRLP4 36+4
ADDRLP4 24+4
INDIRF4
NEGF4
ASGNF4
ADDRLP4 36+8
ADDRLP4 24+8
INDIRF4
NEGF4
ASGNF4
line 2674
;2673:	//walk, crouch or jump
;2674:	movetype = MOVE_WALK;
ADDRLP4 52
CNSTI4 1
ASGNI4
line 2676
;2675:	//
;2676:	if (bs->attackcrouch_time < FloatTime() - 1) {
ADDRFP4 4
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
GEF4 $748
line 2677
;2677:		if (random() < jumper) {
ADDRLP4 368
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 368
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 340
INDIRF4
GEF4 $750
line 2678
;2678:			movetype = MOVE_JUMP;
ADDRLP4 52
CNSTI4 4
ASGNI4
line 2679
;2679:		}
ADDRGP4 $751
JUMPV
LABELV $750
line 2681
;2680:		//wait at least one second before crouching again
;2681:		else if (bs->attackcrouch_time < FloatTime() - 1 && random() < croucher) {
ADDRFP4 4
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
GEF4 $752
ADDRLP4 372
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 372
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 344
INDIRF4
GEF4 $752
line 2682
;2682:			bs->attackcrouch_time = FloatTime() + croucher * 5;
ADDRFP4 4
INDIRP4
CNSTI4 6120
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDRLP4 344
INDIRF4
MULF4
ADDF4
ASGNF4
line 2683
;2683:		}
LABELV $752
LABELV $751
line 2684
;2684:	}
LABELV $748
line 2685
;2685:	if (bs->attackcrouch_time > FloatTime()) movetype = MOVE_CROUCH;
ADDRFP4 4
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $754
ADDRLP4 52
CNSTI4 2
ASGNI4
LABELV $754
line 2687
;2686:	//if the bot should jump
;2687:	if (movetype == MOVE_JUMP) {
ADDRLP4 52
INDIRI4
CNSTI4 4
NEI4 $756
line 2689
;2688:		//if jumped last frame
;2689:		if (bs->attackjump_time > FloatTime()) {
ADDRFP4 4
INDIRP4
CNSTI4 6128
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $758
line 2690
;2690:			movetype = MOVE_WALK;
ADDRLP4 52
CNSTI4 1
ASGNI4
line 2691
;2691:		}
ADDRGP4 $759
JUMPV
LABELV $758
line 2692
;2692:		else {
line 2693
;2693:			bs->attackjump_time = FloatTime() + 1;
ADDRFP4 4
INDIRP4
CNSTI4 6128
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2694
;2694:		}
LABELV $759
line 2695
;2695:	}
LABELV $756
line 2696
;2696:	if (bs->cur_ps.weapon == WP_GAUNTLET) {
ADDRFP4 4
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
CNSTI4 1
NEI4 $760
line 2697
;2697:		attack_dist = 0;
ADDRLP4 72
CNSTF4 0
ASGNF4
line 2698
;2698:		attack_range = 0;
ADDRLP4 76
CNSTF4 0
ASGNF4
line 2699
;2699:	}
ADDRGP4 $761
JUMPV
LABELV $760
line 2700
;2700:	else {
line 2701
;2701:		attack_dist = IDEAL_ATTACKDIST;
ADDRLP4 72
CNSTF4 1124859904
ASGNF4
line 2702
;2702:		attack_range = 40;
ADDRLP4 76
CNSTF4 1109393408
ASGNF4
line 2703
;2703:	}
LABELV $761
line 2705
;2704:	//if the bot is stupid
;2705:	if (attack_skill <= 0.4) {
ADDRLP4 132
INDIRF4
CNSTF4 1053609165
GTF4 $762
line 2707
;2706:		//just walk to or away from the enemy
;2707:		if (dist > attack_dist + attack_range) {
ADDRLP4 68
INDIRF4
ADDRLP4 72
INDIRF4
ADDRLP4 76
INDIRF4
ADDF4
LEF4 $764
line 2708
;2708:			if (trap_BotMoveInDirection(bs->ms, forward, 400, movetype)) return moveresult;
ADDRFP4 4
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 52
INDIRI4
ARGI4
ADDRLP4 368
ADDRGP4 trap_BotMoveInDirection
CALLI4
ASGNI4
ADDRLP4 368
INDIRI4
CNSTI4 0
EQI4 $766
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $766
line 2709
;2709:		}
LABELV $764
line 2710
;2710:		if (dist < attack_dist - attack_range) {
ADDRLP4 68
INDIRF4
ADDRLP4 72
INDIRF4
ADDRLP4 76
INDIRF4
SUBF4
GEF4 $768
line 2711
;2711:			if (trap_BotMoveInDirection(bs->ms, backward, 400, movetype)) return moveresult;
ADDRFP4 4
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 36
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 52
INDIRI4
ARGI4
ADDRLP4 368
ADDRGP4 trap_BotMoveInDirection
CALLI4
ASGNI4
ADDRLP4 368
INDIRI4
CNSTI4 0
EQI4 $770
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $770
line 2712
;2712:		}
LABELV $768
line 2713
;2713:		return moveresult;
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $762
line 2716
;2714:	}
;2715:	//increase the strafe time
;2716:	bs->attackstrafe_time += bs->thinktime;
ADDRLP4 368
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 372
ADDRLP4 368
INDIRP4
CNSTI4 6116
ADDP4
ASGNP4
ADDRLP4 372
INDIRP4
ADDRLP4 372
INDIRP4
INDIRF4
ADDRLP4 368
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2718
;2717:	//get the strafe change time
;2718:	strafechange_time = 0.4 + (1 - attack_skill) * 0.2;
ADDRLP4 336
CNSTF4 1045220557
CNSTF4 1065353216
ADDRLP4 132
INDIRF4
SUBF4
MULF4
CNSTF4 1053609165
ADDF4
ASGNF4
line 2719
;2719:	if (attack_skill > 0.7) strafechange_time += crandom() * 0.2;
ADDRLP4 132
INDIRF4
CNSTF4 1060320051
LEF4 $772
ADDRLP4 376
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 336
ADDRLP4 336
INDIRF4
CNSTF4 1045220557
CNSTF4 1073741824
ADDRLP4 376
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
ASGNF4
LABELV $772
line 2721
;2720:	//if the strafe direction should be changed
;2721:	if (bs->attackstrafe_time > strafechange_time) {
ADDRFP4 4
INDIRP4
CNSTI4 6116
ADDP4
INDIRF4
ADDRLP4 336
INDIRF4
LEF4 $774
line 2723
;2722:		//some magic number :)
;2723:		if (random() > 0.935) {
ADDRLP4 380
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 380
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1064262697
LEF4 $776
line 2725
;2724:			//flip the strafe direction
;2725:			bs->flags ^= BFL_STRAFERIGHT;
ADDRLP4 384
ADDRFP4 4
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 384
INDIRP4
ADDRLP4 384
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 2726
;2726:			bs->attackstrafe_time = 0;
ADDRFP4 4
INDIRP4
CNSTI4 6116
ADDP4
CNSTF4 0
ASGNF4
line 2727
;2727:		}
LABELV $776
line 2728
;2728:	}
LABELV $774
line 2730
;2729:	//
;2730:	for (i = 0; i < 2; i++) {
ADDRLP4 48
CNSTI4 0
ASGNI4
LABELV $778
line 2731
;2731:		hordir[0] = forward[0];
ADDRLP4 12
ADDRLP4 24
INDIRF4
ASGNF4
line 2732
;2732:		hordir[1] = forward[1];
ADDRLP4 12+4
ADDRLP4 24+4
INDIRF4
ASGNF4
line 2733
;2733:		hordir[2] = 0;
ADDRLP4 12+8
CNSTF4 0
ASGNF4
line 2734
;2734:		VectorNormalize(hordir);
ADDRLP4 12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2736
;2735:		//get the sideward vector
;2736:		CrossProduct(hordir, up, sideward);
ADDRLP4 12
ARGP4
ADDRLP4 56
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 2738
;2737:		//reverse the vector depending on the strafe direction
;2738:		if (bs->flags & BFL_STRAFERIGHT) VectorNegate(sideward, sideward);
ADDRFP4 4
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $785
ADDRLP4 0
ADDRLP4 0
INDIRF4
NEGF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
NEGF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
NEGF4
ASGNF4
LABELV $785
line 2740
;2739:		//randomly go back a little
;2740:		if (random() > 0.9) {
ADDRLP4 380
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 380
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1063675494
LEF4 $791
line 2741
;2741:			VectorAdd(sideward, backward, sideward);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
ADDF4
ASGNF4
line 2742
;2742:		}
ADDRGP4 $792
JUMPV
LABELV $791
line 2743
;2743:		else {
line 2745
;2744:			//walk forward or backward to get at the ideal attack distance
;2745:			if (dist > attack_dist + attack_range) {
ADDRLP4 68
INDIRF4
ADDRLP4 72
INDIRF4
ADDRLP4 76
INDIRF4
ADDF4
LEF4 $799
line 2746
;2746:				VectorAdd(sideward, forward, sideward);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 24+8
INDIRF4
ADDF4
ASGNF4
line 2747
;2747:			}
ADDRGP4 $800
JUMPV
LABELV $799
line 2748
;2748:			else if (dist < attack_dist - attack_range) {
ADDRLP4 68
INDIRF4
ADDRLP4 72
INDIRF4
ADDRLP4 76
INDIRF4
SUBF4
GEF4 $807
line 2749
;2749:				VectorAdd(sideward, backward, sideward);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
ADDF4
ASGNF4
line 2750
;2750:			}
LABELV $807
LABELV $800
line 2751
;2751:		}
LABELV $792
line 2753
;2752:		//perform the movement
;2753:		if (trap_BotMoveInDirection(bs->ms, sideward, 400, movetype))
ADDRFP4 4
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 52
INDIRI4
ARGI4
ADDRLP4 384
ADDRGP4 trap_BotMoveInDirection
CALLI4
ASGNI4
ADDRLP4 384
INDIRI4
CNSTI4 0
EQI4 $815
line 2754
;2754:			return moveresult;
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
ADDRGP4 $718
JUMPV
LABELV $815
line 2756
;2755:		//movement failed, flip the strafe direction
;2756:		bs->flags ^= BFL_STRAFERIGHT;
ADDRLP4 388
ADDRFP4 4
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 388
INDIRP4
ADDRLP4 388
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 2757
;2757:		bs->attackstrafe_time = 0;
ADDRFP4 4
INDIRP4
CNSTI4 6116
ADDP4
CNSTF4 0
ASGNF4
line 2758
;2758:	}
LABELV $779
line 2730
ADDRLP4 48
ADDRLP4 48
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 2
LTI4 $778
line 2761
;2759:	//bot couldn't do any usefull movement
;2760://	bs->attackchase_time = AAS_Time() + 6;
;2761:	return moveresult;
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRB
ASGNB 52
LABELV $718
endproc BotAttackMove 392 16
export BotSameTeam
proc BotSameTeam 2072 12
line 2769
;2762:}
;2763:
;2764:/*
;2765:==================
;2766:BotSameTeam
;2767:==================
;2768:*/
;2769:int BotSameTeam(bot_state_t *bs, int entnum) {
line 2772
;2770:	char info1[1024], info2[1024];
;2771:
;2772:	if (bs->client < 0 || bs->client >= MAX_CLIENTS) {
ADDRLP4 2048
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 2048
INDIRI4
CNSTI4 0
LTI4 $820
ADDRLP4 2048
INDIRI4
CNSTI4 64
LTI4 $818
LABELV $820
line 2774
;2773:		//BotAI_Print(PRT_ERROR, "BotSameTeam: client out of range\n");
;2774:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $817
JUMPV
LABELV $818
line 2776
;2775:	}
;2776:	if (entnum < 0 || entnum >= MAX_CLIENTS) {
ADDRLP4 2052
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 2052
INDIRI4
CNSTI4 0
LTI4 $823
ADDRLP4 2052
INDIRI4
CNSTI4 64
LTI4 $821
LABELV $823
line 2778
;2777:		//BotAI_Print(PRT_ERROR, "BotSameTeam: client out of range\n");
;2778:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $817
JUMPV
LABELV $821
line 2780
;2779:	}
;2780:	if ( gametype >= GT_TEAM ) {
ADDRGP4 gametype
INDIRI4
CNSTI4 3
LTI4 $824
line 2781
;2781:		trap_GetConfigstring(CS_PLAYERS+bs->client, info1, sizeof(info1));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 2782
;2782:		trap_GetConfigstring(CS_PLAYERS+entnum, info2, sizeof(info2));
ADDRFP4 4
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 2784
;2783:		//
;2784:		if (atoi(Info_ValueForKey(info1, "t")) == atoi(Info_ValueForKey(info2, "t"))) return qtrue;
ADDRLP4 0
ARGP4
ADDRGP4 $67
ARGP4
ADDRLP4 2056
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2056
INDIRP4
ARGP4
ADDRLP4 2060
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1024
ARGP4
ADDRGP4 $67
ARGP4
ADDRLP4 2064
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2064
INDIRP4
ARGP4
ADDRLP4 2068
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2060
INDIRI4
ADDRLP4 2068
INDIRI4
NEI4 $826
CNSTI4 1
RETI4
ADDRGP4 $817
JUMPV
LABELV $826
line 2785
;2785:	}
LABELV $824
line 2786
;2786:	return qfalse;
CNSTI4 0
RETI4
LABELV $817
endproc BotSameTeam 2072 12
export InFieldOfVision
proc InFieldOfVision 24 4
line 2795
;2787:}
;2788:
;2789:/*
;2790:==================
;2791:InFieldOfVision
;2792:==================
;2793:*/
;2794:qboolean InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles)
;2795:{
line 2799
;2796:	int i;
;2797:	float diff, angle;
;2798:
;2799:	for (i = 0; i < 2; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $829
line 2800
;2800:		angle = AngleMod(viewangles[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 12
INDIRF4
ASGNF4
line 2801
;2801:		angles[i] = AngleMod(angles[i]);
ADDRLP4 16
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 20
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 16
INDIRP4
ADDRLP4 20
INDIRF4
ASGNF4
line 2802
;2802:		diff = angles[i] - angle;
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
SUBF4
ASGNF4
line 2803
;2803:		if (angles[i] > angle) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $833
line 2804
;2804:			if (diff > 180.0) diff -= 360.0;
ADDRLP4 4
INDIRF4
CNSTF4 1127481344
LEF4 $834
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
line 2805
;2805:		}
ADDRGP4 $834
JUMPV
LABELV $833
line 2806
;2806:		else {
line 2807
;2807:			if (diff < -180.0) diff += 360.0;
ADDRLP4 4
INDIRF4
CNSTF4 3274964992
GEF4 $837
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1135869952
ADDF4
ASGNF4
LABELV $837
line 2808
;2808:		}
LABELV $834
line 2809
;2809:		if (diff > 0) {
ADDRLP4 4
INDIRF4
CNSTF4 0
LEF4 $839
line 2810
;2810:			if (diff > fov * 0.5) return qfalse;
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
ADDRFP4 4
INDIRF4
MULF4
LEF4 $840
CNSTI4 0
RETI4
ADDRGP4 $828
JUMPV
line 2811
;2811:		}
LABELV $839
line 2812
;2812:		else {
line 2813
;2813:			if (diff < -fov * 0.5) return qfalse;
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
ADDRFP4 4
INDIRF4
NEGF4
MULF4
GEF4 $843
CNSTI4 0
RETI4
ADDRGP4 $828
JUMPV
LABELV $843
line 2814
;2814:		}
LABELV $840
line 2815
;2815:	}
LABELV $830
line 2799
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LTI4 $829
line 2816
;2816:	return qtrue;
CNSTI4 1
RETI4
LABELV $828
endproc InFieldOfVision 24 4
export BotEntityVisible
proc BotEntityVisible 376 28
line 2826
;2817:}
;2818:
;2819:/*
;2820:==================
;2821:BotEntityVisible
;2822:
;2823:returns visibility in the range [0, 1] taking fog and water surfaces into account
;2824:==================
;2825:*/
;2826:float BotEntityVisible(int viewer, vec3_t eye, vec3_t viewangles, float fov, int ent) {
line 2834
;2827:	int i, contents_mask, passent, hitent, infog, inwater, otherinfog, pc;
;2828:	float squaredfogdist, waterfactor, vis, bestvis;
;2829:	bsp_trace_t trace;
;2830:	aas_entityinfo_t entinfo;
;2831:	vec3_t dir, entangles, start, end, middle;
;2832:
;2833:	//calculate middle of bounding box
;2834:	BotEntityInfo(ent, &entinfo);
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 148
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2835
;2835:	VectorAdd(entinfo.mins, entinfo.maxs, middle);
ADDRLP4 84
ADDRLP4 148+72
INDIRF4
ADDRLP4 148+84
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84+4
ADDRLP4 148+72+4
INDIRF4
ADDRLP4 148+84+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84+8
ADDRLP4 148+72+8
INDIRF4
ADDRLP4 148+84+8
INDIRF4
ADDF4
ASGNF4
line 2836
;2836:	VectorScale(middle, 0.5, middle);
ADDRLP4 332
CNSTF4 1056964608
ASGNF4
ADDRLP4 84
ADDRLP4 332
INDIRF4
ADDRLP4 84
INDIRF4
MULF4
ASGNF4
ADDRLP4 84+4
ADDRLP4 332
INDIRF4
ADDRLP4 84+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 84+8
CNSTF4 1056964608
ADDRLP4 84+8
INDIRF4
MULF4
ASGNF4
line 2837
;2837:	VectorAdd(entinfo.origin, middle, middle);
ADDRLP4 84
ADDRLP4 148+24
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84+4
ADDRLP4 148+24+4
INDIRF4
ADDRLP4 84+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 84+8
ADDRLP4 148+24+8
INDIRF4
ADDRLP4 84+8
INDIRF4
ADDF4
ASGNF4
line 2839
;2838:	//check if entity is within field of vision
;2839:	VectorSubtract(middle, eye, dir);
ADDRLP4 336
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 136
ADDRLP4 84
INDIRF4
ADDRLP4 336
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+4
ADDRLP4 84+4
INDIRF4
ADDRLP4 336
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+8
ADDRLP4 84+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2840
;2840:	vectoangles(dir, entangles);
ADDRLP4 136
ARGP4
ADDRLP4 320
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2841
;2841:	if (!InFieldOfVision(viewangles, fov, entangles)) return 0;
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 320
ARGP4
ADDRLP4 340
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 340
INDIRI4
CNSTI4 0
NEI4 $875
CNSTF4 0
RETF4
ADDRGP4 $845
JUMPV
LABELV $875
line 2843
;2842:	//
;2843:	pc = trap_AAS_PointContents(eye);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 344
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 316
ADDRLP4 344
INDIRI4
ASGNI4
line 2844
;2844:	infog = (pc & CONTENTS_FOG);
ADDRLP4 312
ADDRLP4 316
INDIRI4
CNSTI4 64
BANDI4
ASGNI4
line 2845
;2845:	inwater = (pc & (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER));
ADDRLP4 308
ADDRLP4 316
INDIRI4
CNSTI4 56
BANDI4
ASGNI4
line 2847
;2846:	//
;2847:	bestvis = 0;
ADDRLP4 296
CNSTF4 0
ASGNF4
line 2848
;2848:	for (i = 0; i < 3; i++) {
ADDRLP4 100
CNSTI4 0
ASGNI4
LABELV $877
line 2852
;2849:		//if the point is not in potential visible sight
;2850:		//if (!AAS_inPVS(eye, middle)) continue;
;2851:		//
;2852:		contents_mask = CONTENTS_SOLID|CONTENTS_PLAYERCLIP;
ADDRLP4 96
CNSTI4 65537
ASGNI4
line 2853
;2853:		passent = viewer;
ADDRLP4 116
ADDRFP4 0
INDIRI4
ASGNI4
line 2854
;2854:		hitent = ent;
ADDRLP4 132
ADDRFP4 16
INDIRI4
ASGNI4
line 2855
;2855:		VectorCopy(eye, start);
ADDRLP4 120
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2856
;2856:		VectorCopy(middle, end);
ADDRLP4 104
ADDRLP4 84
INDIRB
ASGNB 12
line 2858
;2857:		//if the entity is in water, lava or slime
;2858:		if (trap_AAS_PointContents(middle) & (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER)) {
ADDRLP4 84
ARGP4
ADDRLP4 348
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 348
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $881
line 2859
;2859:			contents_mask |= (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER);
ADDRLP4 96
ADDRLP4 96
INDIRI4
CNSTI4 56
BORI4
ASGNI4
line 2860
;2860:		}
LABELV $881
line 2862
;2861:		//if eye is in water, lava or slime
;2862:		if (inwater) {
ADDRLP4 308
INDIRI4
CNSTI4 0
EQI4 $883
line 2863
;2863:			if (!(contents_mask & (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER))) {
ADDRLP4 96
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
NEI4 $885
line 2864
;2864:				passent = ent;
ADDRLP4 116
ADDRFP4 16
INDIRI4
ASGNI4
line 2865
;2865:				hitent = viewer;
ADDRLP4 132
ADDRFP4 0
INDIRI4
ASGNI4
line 2866
;2866:				VectorCopy(middle, start);
ADDRLP4 120
ADDRLP4 84
INDIRB
ASGNB 12
line 2867
;2867:				VectorCopy(eye, end);
ADDRLP4 104
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2868
;2868:			}
LABELV $885
line 2869
;2869:			contents_mask ^= (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER);
ADDRLP4 96
ADDRLP4 96
INDIRI4
CNSTI4 56
BXORI4
ASGNI4
line 2870
;2870:		}
LABELV $883
line 2872
;2871:		//trace from start to end
;2872:		BotAI_Trace(&trace, start, NULL, NULL, end, passent, contents_mask);
ADDRLP4 0
ARGP4
ADDRLP4 120
ARGP4
ADDRLP4 352
CNSTP4 0
ASGNP4
ADDRLP4 352
INDIRP4
ARGP4
ADDRLP4 352
INDIRP4
ARGP4
ADDRLP4 104
ARGP4
ADDRLP4 116
INDIRI4
ARGI4
ADDRLP4 96
INDIRI4
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2874
;2873:		//if water was hit
;2874:		waterfactor = 1.0;
ADDRLP4 288
CNSTF4 1065353216
ASGNF4
line 2875
;2875:		if (trace.contents & (CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER)) {
ADDRLP4 0+76
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $887
line 2877
;2876:			//if the water surface is translucent
;2877:			if (1) {
line 2879
;2878:				//trace through the water
;2879:				contents_mask &= ~(CONTENTS_LAVA|CONTENTS_SLIME|CONTENTS_WATER);
ADDRLP4 96
ADDRLP4 96
INDIRI4
CNSTI4 -57
BANDI4
ASGNI4
line 2880
;2880:				BotAI_Trace(&trace, trace.endpos, NULL, NULL, end, passent, contents_mask);
ADDRLP4 0
ARGP4
ADDRLP4 0+12
ARGP4
ADDRLP4 356
CNSTP4 0
ASGNP4
ADDRLP4 356
INDIRP4
ARGP4
ADDRLP4 356
INDIRP4
ARGP4
ADDRLP4 104
ARGP4
ADDRLP4 116
INDIRI4
ARGI4
ADDRLP4 96
INDIRI4
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2881
;2881:				waterfactor = 0.5;
ADDRLP4 288
CNSTF4 1056964608
ASGNF4
line 2882
;2882:			}
LABELV $890
line 2883
;2883:		}
LABELV $887
line 2885
;2884:		//if a full trace or the hitent was hit
;2885:		if (trace.fraction >= 1 || trace.ent == hitent) {
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
GEF4 $897
ADDRLP4 0+80
INDIRI4
ADDRLP4 132
INDIRI4
NEI4 $893
LABELV $897
line 2888
;2886:			//check for fog, assuming there's only one fog brush where
;2887:			//either the viewer or the entity is in or both are in
;2888:			otherinfog = (trap_AAS_PointContents(middle) & CONTENTS_FOG);
ADDRLP4 84
ARGP4
ADDRLP4 356
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 304
ADDRLP4 356
INDIRI4
CNSTI4 64
BANDI4
ASGNI4
line 2889
;2889:			if (infog && otherinfog) {
ADDRLP4 360
CNSTI4 0
ASGNI4
ADDRLP4 312
INDIRI4
ADDRLP4 360
INDIRI4
EQI4 $898
ADDRLP4 304
INDIRI4
ADDRLP4 360
INDIRI4
EQI4 $898
line 2890
;2890:				VectorSubtract(trace.endpos, eye, dir);
ADDRLP4 364
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 136
ADDRLP4 0+12
INDIRF4
ADDRLP4 364
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+4
ADDRLP4 0+12+4
INDIRF4
ADDRLP4 364
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+8
ADDRLP4 0+12+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2891
;2891:				squaredfogdist = VectorLengthSquared(dir);
ADDRLP4 136
ARGP4
ADDRLP4 368
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 300
ADDRLP4 368
INDIRF4
ASGNF4
line 2892
;2892:			}
ADDRGP4 $899
JUMPV
LABELV $898
line 2893
;2893:			else if (infog) {
ADDRLP4 312
INDIRI4
CNSTI4 0
EQI4 $907
line 2894
;2894:				VectorCopy(trace.endpos, start);
ADDRLP4 120
ADDRLP4 0+12
INDIRB
ASGNB 12
line 2895
;2895:				BotAI_Trace(&trace, start, NULL, NULL, eye, viewer, CONTENTS_FOG);
ADDRLP4 0
ARGP4
ADDRLP4 120
ARGP4
ADDRLP4 364
CNSTP4 0
ASGNP4
ADDRLP4 364
INDIRP4
ARGP4
ADDRLP4 364
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2896
;2896:				VectorSubtract(eye, trace.endpos, dir);
ADDRLP4 368
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 136
ADDRLP4 368
INDIRP4
INDIRF4
ADDRLP4 0+12
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+4
ADDRLP4 368
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 0+12+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 0+12+8
INDIRF4
SUBF4
ASGNF4
line 2897
;2897:				squaredfogdist = VectorLengthSquared(dir);
ADDRLP4 136
ARGP4
ADDRLP4 372
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 300
ADDRLP4 372
INDIRF4
ASGNF4
line 2898
;2898:			}
ADDRGP4 $908
JUMPV
LABELV $907
line 2899
;2899:			else if (otherinfog) {
ADDRLP4 304
INDIRI4
CNSTI4 0
EQI4 $917
line 2900
;2900:				VectorCopy(trace.endpos, end);
ADDRLP4 104
ADDRLP4 0+12
INDIRB
ASGNB 12
line 2901
;2901:				BotAI_Trace(&trace, eye, NULL, NULL, end, viewer, CONTENTS_FOG);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 364
CNSTP4 0
ASGNP4
ADDRLP4 364
INDIRP4
ARGP4
ADDRLP4 364
INDIRP4
ARGP4
ADDRLP4 104
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 2902
;2902:				VectorSubtract(end, trace.endpos, dir);
ADDRLP4 136
ADDRLP4 104
INDIRF4
ADDRLP4 0+12
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+4
ADDRLP4 104+4
INDIRF4
ADDRLP4 0+12+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 136+8
ADDRLP4 104+8
INDIRF4
ADDRLP4 0+12+8
INDIRF4
SUBF4
ASGNF4
line 2903
;2903:				squaredfogdist = VectorLengthSquared(dir);
ADDRLP4 136
ARGP4
ADDRLP4 368
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 300
ADDRLP4 368
INDIRF4
ASGNF4
line 2904
;2904:			}
ADDRGP4 $918
JUMPV
LABELV $917
line 2905
;2905:			else {
line 2907
;2906:				//if the entity and the viewer are not in fog assume there's no fog in between
;2907:				squaredfogdist = 0;
ADDRLP4 300
CNSTF4 0
ASGNF4
line 2908
;2908:			}
LABELV $918
LABELV $908
LABELV $899
line 2910
;2909:			//decrease visibility with the view distance through fog
;2910:			vis = 1 / ((squaredfogdist * 0.001) < 1 ? 1 : (squaredfogdist * 0.001));
ADDRLP4 368
CNSTF4 1065353216
ASGNF4
CNSTF4 981668463
ADDRLP4 300
INDIRF4
MULF4
ADDRLP4 368
INDIRF4
GEF4 $930
ADDRLP4 364
CNSTF4 1065353216
ASGNF4
ADDRGP4 $931
JUMPV
LABELV $930
ADDRLP4 364
CNSTF4 981668463
ADDRLP4 300
INDIRF4
MULF4
ASGNF4
LABELV $931
ADDRLP4 292
ADDRLP4 368
INDIRF4
ADDRLP4 364
INDIRF4
DIVF4
ASGNF4
line 2912
;2911:			//if entering water visibility is reduced
;2912:			vis *= waterfactor;
ADDRLP4 292
ADDRLP4 292
INDIRF4
ADDRLP4 288
INDIRF4
MULF4
ASGNF4
line 2914
;2913:			//
;2914:			if (vis > bestvis) bestvis = vis;
ADDRLP4 292
INDIRF4
ADDRLP4 296
INDIRF4
LEF4 $932
ADDRLP4 296
ADDRLP4 292
INDIRF4
ASGNF4
LABELV $932
line 2916
;2915:			//if pretty much no fog
;2916:			if (bestvis >= 0.95) return bestvis;
ADDRLP4 296
INDIRF4
CNSTF4 1064514355
LTF4 $934
ADDRLP4 296
INDIRF4
RETF4
ADDRGP4 $845
JUMPV
LABELV $934
line 2917
;2917:		}
LABELV $893
line 2919
;2918:		//check bottom and top of bounding box as well
;2919:		if (i == 0) middle[2] += entinfo.mins[2];
ADDRLP4 100
INDIRI4
CNSTI4 0
NEI4 $936
ADDRLP4 84+8
ADDRLP4 84+8
INDIRF4
ADDRLP4 148+72+8
INDIRF4
ADDF4
ASGNF4
ADDRGP4 $937
JUMPV
LABELV $936
line 2920
;2920:		else if (i == 1) middle[2] += entinfo.maxs[2] - entinfo.mins[2];
ADDRLP4 100
INDIRI4
CNSTI4 1
NEI4 $941
ADDRLP4 84+8
ADDRLP4 84+8
INDIRF4
ADDRLP4 148+84+8
INDIRF4
ADDRLP4 148+72+8
INDIRF4
SUBF4
ADDF4
ASGNF4
LABELV $941
LABELV $937
line 2921
;2921:	}
LABELV $878
line 2848
ADDRLP4 100
ADDRLP4 100
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 100
INDIRI4
CNSTI4 3
LTI4 $877
line 2922
;2922:	return bestvis;
ADDRLP4 296
INDIRF4
RETF4
LABELV $845
endproc BotEntityVisible 376 28
export BotFindEnemy
proc BotFindEnemy 432 20
line 2930
;2923:}
;2924:
;2925:/*
;2926:==================
;2927:BotFindEnemy
;2928:==================
;2929:*/
;2930:int BotFindEnemy(bot_state_t *bs, int curenemy) {
line 2937
;2931:	int i, healthdecrease;
;2932:	float f, alertness, easyfragger, vis;
;2933:	float squaredist, cursquaredist;
;2934:	aas_entityinfo_t entinfo, curenemyinfo;
;2935:	vec3_t dir, angles;
;2936:
;2937:	alertness = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_ALERTNESS, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 46
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 336
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 164
ADDRLP4 336
INDIRF4
ASGNF4
line 2938
;2938:	easyfragger = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_EASY_FRAGGER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 45
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 340
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 176
ADDRLP4 340
INDIRF4
ASGNF4
line 2940
;2939:	//check if the health decreased
;2940:	healthdecrease = bs->lasthealth > bs->inventory[INVENTORY_HEALTH];
ADDRLP4 348
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 348
INDIRP4
CNSTI4 5988
ADDP4
INDIRI4
ADDRLP4 348
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
LEI4 $950
ADDRLP4 344
CNSTI4 1
ASGNI4
ADDRGP4 $951
JUMPV
LABELV $950
ADDRLP4 344
CNSTI4 0
ASGNI4
LABELV $951
ADDRLP4 160
ADDRLP4 344
INDIRI4
ASGNI4
line 2942
;2941:	//remember the current health value
;2942:	bs->lasthealth = bs->inventory[INVENTORY_HEALTH];
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 352
INDIRP4
CNSTI4 5988
ADDP4
ADDRLP4 352
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
ASGNI4
line 2944
;2943:	//
;2944:	if (curenemy >= 0) {
ADDRFP4 4
INDIRI4
CNSTI4 0
LTI4 $952
line 2945
;2945:		BotEntityInfo(curenemy, &curenemyinfo);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 196
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2946
;2946:		if (EntityCarriesFlag(&curenemyinfo)) return qfalse;
ADDRLP4 196
ARGP4
ADDRLP4 356
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 356
INDIRI4
CNSTI4 0
EQI4 $954
CNSTI4 0
RETI4
ADDRGP4 $948
JUMPV
LABELV $954
line 2947
;2947:		VectorSubtract(curenemyinfo.origin, bs->origin, dir);
ADDRLP4 360
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 144
ADDRLP4 196+24
INDIRF4
ADDRLP4 360
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+4
ADDRLP4 196+24+4
INDIRF4
ADDRLP4 360
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+8
ADDRLP4 196+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2948
;2948:		cursquaredist = VectorLengthSquared(dir);
ADDRLP4 144
ARGP4
ADDRLP4 364
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 192
ADDRLP4 364
INDIRF4
ASGNF4
line 2949
;2949:	}
ADDRGP4 $953
JUMPV
LABELV $952
line 2950
;2950:	else {
line 2951
;2951:		cursquaredist = 0;
ADDRLP4 192
CNSTF4 0
ASGNF4
line 2952
;2952:	}
LABELV $953
line 2981
;2953:#ifdef MISSIONPACK
;2954:	if (gametype == GT_OBELISK) {
;2955:		vec3_t target;
;2956:		bot_goal_t *goal;
;2957:		bsp_trace_t trace;
;2958:
;2959:		if (BotTeam(bs) == TEAM_RED)
;2960:			goal = &blueobelisk;
;2961:		else
;2962:			goal = &redobelisk;
;2963:		//if the obelisk is visible
;2964:		VectorCopy(goal->origin, target);
;2965:		target[2] += 1;
;2966:		BotAI_Trace(&trace, bs->eye, NULL, NULL, target, bs->client, CONTENTS_SOLID);
;2967:		if (trace.fraction >= 1 || trace.ent == goal->entitynum) {
;2968:			if (goal->entitynum == bs->enemy) {
;2969:				return qfalse;
;2970:			}
;2971:			bs->enemy = goal->entitynum;
;2972:			bs->enemysight_time = FloatTime();
;2973:			bs->enemysuicide = qfalse;
;2974:			bs->enemydeath_time = 0;
;2975:			bs->enemyvisible_time = FloatTime();
;2976:			return qtrue;
;2977:		}
;2978:	}
;2979:#endif
;2980:	//
;2981:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRGP4 $966
JUMPV
LABELV $963
line 2983
;2982:
;2983:		if (i == bs->client) continue;
ADDRLP4 140
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $967
ADDRGP4 $964
JUMPV
LABELV $967
line 2985
;2984:		//if it's the current enemy
;2985:		if (i == curenemy) continue;
ADDRLP4 140
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $969
ADDRGP4 $964
JUMPV
LABELV $969
line 2987
;2986:		//
;2987:		BotEntityInfo(i, &entinfo);
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2989
;2988:		//
;2989:		if (!entinfo.valid) continue;
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $971
ADDRGP4 $964
JUMPV
LABELV $971
line 2991
;2990:		//if the enemy isn't dead and the enemy isn't the bot self
;2991:		if (EntityIsDead(&entinfo) || entinfo.number == bs->entitynum) continue;
ADDRLP4 0
ARGP4
ADDRLP4 356
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 356
INDIRI4
CNSTI4 0
NEI4 $976
ADDRLP4 0+20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
NEI4 $973
LABELV $976
ADDRGP4 $964
JUMPV
LABELV $973
line 2993
;2992:		//if the enemy is invisible and not shooting
;2993:		if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
ADDRLP4 0
ARGP4
ADDRLP4 360
ADDRGP4 EntityIsInvisible
CALLI4
ASGNI4
ADDRLP4 360
INDIRI4
CNSTI4 0
EQI4 $977
ADDRLP4 0
ARGP4
ADDRLP4 364
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 364
INDIRI4
CNSTI4 0
NEI4 $977
line 2994
;2994:			continue;
ADDRGP4 $964
JUMPV
LABELV $977
line 2997
;2995:		}
;2996:		//if not an easy fragger don't shoot at chatting players
;2997:		if (easyfragger < 0.5 && EntityIsChatting(&entinfo)) continue;
ADDRLP4 176
INDIRF4
CNSTF4 1056964608
GEF4 $979
ADDRLP4 0
ARGP4
ADDRLP4 368
ADDRGP4 EntityIsChatting
CALLI4
ASGNI4
ADDRLP4 368
INDIRI4
CNSTI4 0
EQI4 $979
ADDRGP4 $964
JUMPV
LABELV $979
line 2999
;2998:		//
;2999:		if (lastteleport_time > FloatTime() - 3) {
ADDRGP4 lastteleport_time
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1077936128
SUBF4
LEF4 $981
line 3000
;3000:			VectorSubtract(entinfo.origin, lastteleport_origin, dir);
ADDRLP4 144
ADDRLP4 0+24
INDIRF4
ADDRGP4 lastteleport_origin
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+4
ADDRLP4 0+24+4
INDIRF4
ADDRGP4 lastteleport_origin+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+8
ADDRLP4 0+24+8
INDIRF4
ADDRGP4 lastteleport_origin+8
INDIRF4
SUBF4
ASGNF4
line 3001
;3001:			if (VectorLengthSquared(dir) < Square(70)) continue;
ADDRLP4 144
ARGP4
ADDRLP4 372
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 372
INDIRF4
CNSTF4 1167663104
GEF4 $992
ADDRGP4 $964
JUMPV
LABELV $992
line 3002
;3002:		}
LABELV $981
line 3004
;3003:		//calculate the distance towards the enemy
;3004:		VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 372
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 144
ADDRLP4 0+24
INDIRF4
ADDRLP4 372
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 372
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3005
;3005:		squaredist = VectorLengthSquared(dir);
ADDRLP4 144
ARGP4
ADDRLP4 376
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 376
INDIRF4
ASGNF4
line 3007
;3006:		//if this entity is not carrying a flag
;3007:		if (!EntityCarriesFlag(&entinfo))
ADDRLP4 0
ARGP4
ADDRLP4 380
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 380
INDIRI4
CNSTI4 0
NEI4 $1001
line 3008
;3008:		{
line 3010
;3009:			//if this enemy is further away than the current one
;3010:			if (curenemy >= 0 && squaredist > cursquaredist) continue;
ADDRFP4 4
INDIRI4
CNSTI4 0
LTI4 $1003
ADDRLP4 156
INDIRF4
ADDRLP4 192
INDIRF4
LEF4 $1003
ADDRGP4 $964
JUMPV
LABELV $1003
line 3011
;3011:		} //end if
LABELV $1001
line 3013
;3012:		//if the bot has no
;3013:		if (squaredist > Square(900.0 + alertness * 4000.0)) continue;
ADDRLP4 384
CNSTF4 1165623296
ADDRLP4 164
INDIRF4
MULF4
CNSTF4 1147207680
ADDF4
ASGNF4
ADDRLP4 156
INDIRF4
ADDRLP4 384
INDIRF4
ADDRLP4 384
INDIRF4
MULF4
LEF4 $1005
ADDRGP4 $964
JUMPV
LABELV $1005
line 3015
;3014:		//if on the same team
;3015:		if (BotSameTeam(bs, i)) continue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 388
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 388
INDIRI4
CNSTI4 0
EQI4 $1007
ADDRGP4 $964
JUMPV
LABELV $1007
line 3017
;3016:		//if the bot's health decreased or the enemy is shooting
;3017:		if (curenemy < 0 && (healthdecrease || EntityIsShooting(&entinfo)))
ADDRLP4 392
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRI4
ADDRLP4 392
INDIRI4
GEI4 $1009
ADDRLP4 160
INDIRI4
ADDRLP4 392
INDIRI4
NEI4 $1011
ADDRLP4 0
ARGP4
ADDRLP4 396
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 396
INDIRI4
CNSTI4 0
EQI4 $1009
LABELV $1011
line 3018
;3018:			f = 360;
ADDRLP4 168
CNSTF4 1135869952
ASGNF4
ADDRGP4 $1010
JUMPV
LABELV $1009
line 3020
;3019:		else
;3020:			f = 90 + 90 - (90 - (squaredist > Square(810) ? Square(810) : squaredist) / (810 * 9));
ADDRLP4 156
INDIRF4
CNSTF4 1226845760
LEF4 $1013
ADDRLP4 400
CNSTF4 1226845760
ASGNF4
ADDRGP4 $1014
JUMPV
LABELV $1013
ADDRLP4 400
ADDRLP4 156
INDIRF4
ASGNF4
LABELV $1014
ADDRLP4 168
CNSTF4 1127481344
CNSTF4 1119092736
ADDRLP4 400
INDIRF4
CNSTF4 1172557824
DIVF4
SUBF4
SUBF4
ASGNF4
LABELV $1010
line 3022
;3021:		//check if the enemy is visible
;3022:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, f, i);
ADDRLP4 404
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 404
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 404
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 404
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRLP4 168
INDIRF4
ARGF4
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 408
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 172
ADDRLP4 408
INDIRF4
ASGNF4
line 3023
;3023:		if (vis <= 0) continue;
ADDRLP4 172
INDIRF4
CNSTF4 0
GTF4 $1015
ADDRGP4 $964
JUMPV
LABELV $1015
line 3025
;3024:		//if the enemy is quite far away, not shooting and the bot is not damaged
;3025:		if (curenemy < 0 && squaredist > Square(100) && !healthdecrease && !EntityIsShooting(&entinfo))
ADDRLP4 412
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRI4
ADDRLP4 412
INDIRI4
GEI4 $1017
ADDRLP4 156
INDIRF4
CNSTF4 1176256512
LEF4 $1017
ADDRLP4 160
INDIRI4
ADDRLP4 412
INDIRI4
NEI4 $1017
ADDRLP4 0
ARGP4
ADDRLP4 416
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 416
INDIRI4
CNSTI4 0
NEI4 $1017
line 3026
;3026:		{
line 3028
;3027:			//check if we can avoid this enemy
;3028:			VectorSubtract(bs->origin, entinfo.origin, dir);
ADDRLP4 420
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 144
ADDRLP4 420
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRLP4 0+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+4
ADDRLP4 420
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRLP4 0+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+8
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRLP4 0+24+8
INDIRF4
SUBF4
ASGNF4
line 3029
;3029:			vectoangles(dir, angles);
ADDRLP4 144
ARGP4
ADDRLP4 180
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3031
;3030:			//if the bot isn't in the fov of the enemy
;3031:			if (!InFieldOfVision(entinfo.angles, 90, angles)) {
ADDRLP4 0+36
ARGP4
CNSTF4 1119092736
ARGF4
ADDRLP4 180
ARGP4
ADDRLP4 424
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 424
INDIRI4
CNSTI4 0
NEI4 $1026
line 3033
;3032:				//update some stuff for this enemy
;3033:				BotUpdateBattleInventory(bs, i);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
INDIRI4
ARGI4
ADDRGP4 BotUpdateBattleInventory
CALLV
pop
line 3035
;3034:				//if the bot doesn't really want to fight
;3035:				if (BotWantsToRetreat(bs)) continue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 428
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 428
INDIRI4
CNSTI4 0
EQI4 $1029
ADDRGP4 $964
JUMPV
LABELV $1029
line 3036
;3036:			}
LABELV $1026
line 3037
;3037:		}
LABELV $1017
line 3039
;3038:		//found an enemy
;3039:		bs->enemy = entinfo.number;
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
ADDRLP4 0+20
INDIRI4
ASGNI4
line 3040
;3040:		if (curenemy >= 0) bs->enemysight_time = FloatTime() - 2;
ADDRFP4 4
INDIRI4
CNSTI4 0
LTI4 $1032
ADDRFP4 0
INDIRP4
CNSTI4 6132
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
ADDRGP4 $1033
JUMPV
LABELV $1032
line 3041
;3041:		else bs->enemysight_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6132
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
LABELV $1033
line 3042
;3042:		bs->enemysuicide = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6012
ADDP4
CNSTI4 0
ASGNI4
line 3043
;3043:		bs->enemydeath_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6136
ADDP4
CNSTF4 0
ASGNF4
line 3044
;3044:		bs->enemyvisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6088
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 3045
;3045:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $948
JUMPV
LABELV $964
line 2981
ADDRLP4 140
ADDRLP4 140
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $966
ADDRLP4 140
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1034
ADDRLP4 140
INDIRI4
CNSTI4 64
LTI4 $963
LABELV $1034
line 3047
;3046:	}
;3047:	return qfalse;
CNSTI4 0
RETI4
LABELV $948
endproc BotFindEnemy 432 20
export BotTeamFlagCarrierVisible
proc BotTeamFlagCarrierVisible 164 20
line 3055
;3048:}
;3049:
;3050:/*
;3051:==================
;3052:BotTeamFlagCarrierVisible
;3053:==================
;3054:*/
;3055:int BotTeamFlagCarrierVisible(bot_state_t *bs) {
line 3060
;3056:	int i;
;3057:	float vis;
;3058:	aas_entityinfo_t entinfo;
;3059:
;3060:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1039
JUMPV
LABELV $1036
line 3061
;3061:		if (i == bs->client)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $1040
line 3062
;3062:			continue;
ADDRGP4 $1037
JUMPV
LABELV $1040
line 3064
;3063:		//
;3064:		BotEntityInfo(i, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3066
;3065:		//if this player is active
;3066:		if (!entinfo.valid)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1042
line 3067
;3067:			continue;
ADDRGP4 $1037
JUMPV
LABELV $1042
line 3069
;3068:		//if this player is carrying a flag
;3069:		if (!EntityCarriesFlag(&entinfo))
ADDRLP4 4
ARGP4
ADDRLP4 148
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $1044
line 3070
;3070:			continue;
ADDRGP4 $1037
JUMPV
LABELV $1044
line 3072
;3071:		//if the flag carrier is not on the same team
;3072:		if (!BotSameTeam(bs, i))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 152
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
NEI4 $1046
line 3073
;3073:			continue;
ADDRGP4 $1037
JUMPV
LABELV $1046
line 3075
;3074:		//if the flag carrier is not visible
;3075:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
ADDRLP4 156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 156
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 156
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 156
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 160
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 144
ADDRLP4 160
INDIRF4
ASGNF4
line 3076
;3076:		if (vis <= 0)
ADDRLP4 144
INDIRF4
CNSTF4 0
GTF4 $1048
line 3077
;3077:			continue;
ADDRGP4 $1037
JUMPV
LABELV $1048
line 3079
;3078:		//
;3079:		return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1035
JUMPV
LABELV $1037
line 3060
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1039
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1050
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $1036
LABELV $1050
line 3081
;3080:	}
;3081:	return -1;
CNSTI4 -1
RETI4
LABELV $1035
endproc BotTeamFlagCarrierVisible 164 20
export BotTeamFlagCarrier
proc BotTeamFlagCarrier 152 8
line 3089
;3082:}
;3083:
;3084:/*
;3085:==================
;3086:BotTeamFlagCarrier
;3087:==================
;3088:*/
;3089:int BotTeamFlagCarrier(bot_state_t *bs) {
line 3093
;3090:	int i;
;3091:	aas_entityinfo_t entinfo;
;3092:
;3093:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1055
JUMPV
LABELV $1052
line 3094
;3094:		if (i == bs->client)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $1056
line 3095
;3095:			continue;
ADDRGP4 $1053
JUMPV
LABELV $1056
line 3097
;3096:		//
;3097:		BotEntityInfo(i, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3099
;3098:		//if this player is active
;3099:		if (!entinfo.valid)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1058
line 3100
;3100:			continue;
ADDRGP4 $1053
JUMPV
LABELV $1058
line 3102
;3101:		//if this player is carrying a flag
;3102:		if (!EntityCarriesFlag(&entinfo))
ADDRLP4 4
ARGP4
ADDRLP4 144
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
NEI4 $1060
line 3103
;3103:			continue;
ADDRGP4 $1053
JUMPV
LABELV $1060
line 3105
;3104:		//if the flag carrier is not on the same team
;3105:		if (!BotSameTeam(bs, i))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 148
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $1062
line 3106
;3106:			continue;
ADDRGP4 $1053
JUMPV
LABELV $1062
line 3108
;3107:		//
;3108:		return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1051
JUMPV
LABELV $1053
line 3093
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1055
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1064
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $1052
LABELV $1064
line 3110
;3109:	}
;3110:	return -1;
CNSTI4 -1
RETI4
LABELV $1051
endproc BotTeamFlagCarrier 152 8
export BotEnemyFlagCarrierVisible
proc BotEnemyFlagCarrierVisible 164 20
line 3118
;3111:}
;3112:
;3113:/*
;3114:==================
;3115:BotEnemyFlagCarrierVisible
;3116:==================
;3117:*/
;3118:int BotEnemyFlagCarrierVisible(bot_state_t *bs) {
line 3123
;3119:	int i;
;3120:	float vis;
;3121:	aas_entityinfo_t entinfo;
;3122:
;3123:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1069
JUMPV
LABELV $1066
line 3124
;3124:		if (i == bs->client)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $1070
line 3125
;3125:			continue;
ADDRGP4 $1067
JUMPV
LABELV $1070
line 3127
;3126:		//
;3127:		BotEntityInfo(i, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3129
;3128:		//if this player is active
;3129:		if (!entinfo.valid)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1072
line 3130
;3130:			continue;
ADDRGP4 $1067
JUMPV
LABELV $1072
line 3132
;3131:		//if this player is carrying a flag
;3132:		if (!EntityCarriesFlag(&entinfo))
ADDRLP4 4
ARGP4
ADDRLP4 148
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $1074
line 3133
;3133:			continue;
ADDRGP4 $1067
JUMPV
LABELV $1074
line 3135
;3134:		//if the flag carrier is on the same team
;3135:		if (BotSameTeam(bs, i))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 152
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $1076
line 3136
;3136:			continue;
ADDRGP4 $1067
JUMPV
LABELV $1076
line 3138
;3137:		//if the flag carrier is not visible
;3138:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
ADDRLP4 156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 156
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 156
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 156
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 160
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 144
ADDRLP4 160
INDIRF4
ASGNF4
line 3139
;3139:		if (vis <= 0)
ADDRLP4 144
INDIRF4
CNSTF4 0
GTF4 $1078
line 3140
;3140:			continue;
ADDRGP4 $1067
JUMPV
LABELV $1078
line 3142
;3141:		//
;3142:		return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1065
JUMPV
LABELV $1067
line 3123
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1069
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1080
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $1066
LABELV $1080
line 3144
;3143:	}
;3144:	return -1;
CNSTI4 -1
RETI4
LABELV $1065
endproc BotEnemyFlagCarrierVisible 164 20
export BotVisibleTeamMatesAndEnemies
proc BotVisibleTeamMatesAndEnemies 192 20
line 3152
;3145:}
;3146:
;3147:/*
;3148:==================
;3149:BotVisibleTeamMatesAndEnemies
;3150:==================
;3151:*/
;3152:void BotVisibleTeamMatesAndEnemies(bot_state_t *bs, int *teammates, int *enemies, float range) {
line 3158
;3153:	int i;
;3154:	float vis;
;3155:	aas_entityinfo_t entinfo;
;3156:	vec3_t dir;
;3157:
;3158:	if (teammates)
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1082
line 3159
;3159:		*teammates = 0;
ADDRFP4 4
INDIRP4
CNSTI4 0
ASGNI4
LABELV $1082
line 3160
;3160:	if (enemies)
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1084
line 3161
;3161:		*enemies = 0;
ADDRFP4 8
INDIRP4
CNSTI4 0
ASGNI4
LABELV $1084
line 3162
;3162:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1089
JUMPV
LABELV $1086
line 3163
;3163:		if (i == bs->client)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $1090
line 3164
;3164:			continue;
ADDRGP4 $1087
JUMPV
LABELV $1090
line 3166
;3165:		//
;3166:		BotEntityInfo(i, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3168
;3167:		//if this player is active
;3168:		if (!entinfo.valid)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1092
line 3169
;3169:			continue;
ADDRGP4 $1087
JUMPV
LABELV $1092
line 3171
;3170:		//if this player is carrying a flag
;3171:		if (!EntityCarriesFlag(&entinfo))
ADDRLP4 4
ARGP4
ADDRLP4 160
ADDRGP4 EntityCarriesFlag
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
NEI4 $1094
line 3172
;3172:			continue;
ADDRGP4 $1087
JUMPV
LABELV $1094
line 3174
;3173:		//if not within range
;3174:		VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 144
ADDRLP4 4+24
INDIRF4
ADDRLP4 164
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+4
ADDRLP4 4+24+4
INDIRF4
ADDRLP4 164
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 144+8
ADDRLP4 4+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3175
;3175:		if (VectorLengthSquared(dir) > Square(range))
ADDRLP4 144
ARGP4
ADDRLP4 168
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 172
ADDRFP4 12
INDIRF4
ASGNF4
ADDRLP4 168
INDIRF4
ADDRLP4 172
INDIRF4
ADDRLP4 172
INDIRF4
MULF4
LEF4 $1103
line 3176
;3176:			continue;
ADDRGP4 $1087
JUMPV
LABELV $1103
line 3178
;3177:		//if the flag carrier is not visible
;3178:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 176
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 176
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 180
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 180
INDIRF4
ASGNF4
line 3179
;3179:		if (vis <= 0)
ADDRLP4 156
INDIRF4
CNSTF4 0
GTF4 $1105
line 3180
;3180:			continue;
ADDRGP4 $1087
JUMPV
LABELV $1105
line 3182
;3181:		//if the flag carrier is on the same team
;3182:		if (BotSameTeam(bs, i)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 184
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 184
INDIRI4
CNSTI4 0
EQI4 $1107
line 3183
;3183:			if (teammates)
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1108
line 3184
;3184:				(*teammates)++;
ADDRLP4 188
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 188
INDIRP4
ADDRLP4 188
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3185
;3185:		}
ADDRGP4 $1108
JUMPV
LABELV $1107
line 3186
;3186:		else {
line 3187
;3187:			if (enemies)
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1111
line 3188
;3188:				(*enemies)++;
ADDRLP4 188
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 188
INDIRP4
ADDRLP4 188
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1111
line 3189
;3189:		}
LABELV $1108
line 3190
;3190:	}
LABELV $1087
line 3162
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1089
ADDRLP4 0
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1113
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $1086
LABELV $1113
line 3191
;3191:}
LABELV $1081
endproc BotVisibleTeamMatesAndEnemies 192 20
lit
align 4
LABELV $1115
byte 4 3229614080
byte 4 3229614080
byte 4 3229614080
align 4
LABELV $1116
byte 4 1082130432
byte 4 1082130432
byte 4 1082130432
export BotAimAtEnemy
code
proc BotAimAtEnemy 1144 52
line 3262
;3192:
;3193:#ifdef MISSIONPACK
;3194:/*
;3195:==================
;3196:BotTeamCubeCarrierVisible
;3197:==================
;3198:*/
;3199:int BotTeamCubeCarrierVisible(bot_state_t *bs) {
;3200:	int i;
;3201:	float vis;
;3202:	aas_entityinfo_t entinfo;
;3203:
;3204:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
;3205:		if (i == bs->client) continue;
;3206:		//
;3207:		BotEntityInfo(i, &entinfo);
;3208:		//if this player is active
;3209:		if (!entinfo.valid) continue;
;3210:		//if this player is carrying a flag
;3211:		if (!EntityCarriesCubes(&entinfo)) continue;
;3212:		//if the flag carrier is not on the same team
;3213:		if (!BotSameTeam(bs, i)) continue;
;3214:		//if the flag carrier is not visible
;3215:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
;3216:		if (vis <= 0) continue;
;3217:		//
;3218:		return i;
;3219:	}
;3220:	return -1;
;3221:}
;3222:
;3223:/*
;3224:==================
;3225:BotEnemyCubeCarrierVisible
;3226:==================
;3227:*/
;3228:int BotEnemyCubeCarrierVisible(bot_state_t *bs) {
;3229:	int i;
;3230:	float vis;
;3231:	aas_entityinfo_t entinfo;
;3232:
;3233:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
;3234:		if (i == bs->client)
;3235:			continue;
;3236:		//
;3237:		BotEntityInfo(i, &entinfo);
;3238:		//if this player is active
;3239:		if (!entinfo.valid)
;3240:			continue;
;3241:		//if this player is carrying a flag
;3242:		if (!EntityCarriesCubes(&entinfo)) continue;
;3243:		//if the flag carrier is on the same team
;3244:		if (BotSameTeam(bs, i))
;3245:			continue;
;3246:		//if the flag carrier is not visible
;3247:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
;3248:		if (vis <= 0)
;3249:			continue;
;3250:		//
;3251:		return i;
;3252:	}
;3253:	return -1;
;3254:}
;3255:#endif
;3256:
;3257:/*
;3258:==================
;3259:BotAimAtEnemy
;3260:==================
;3261:*/
;3262:void BotAimAtEnemy(bot_state_t *bs) {
line 3266
;3263:	int i, enemyvisible;
;3264:	float dist, f, aim_skill, aim_accuracy, speed, reactiontime;
;3265:	vec3_t dir, bestorigin, end, start, groundtarget, cmdmove, enemyvelocity;
;3266:	vec3_t mins = {-4,-4,-4}, maxs = {4, 4, 4};
ADDRLP4 860
ADDRGP4 $1115
INDIRB
ASGNB 12
ADDRLP4 872
ADDRGP4 $1116
INDIRB
ASGNB 12
line 3274
;3267:	weaponinfo_t wi;
;3268:	aas_entityinfo_t entinfo;
;3269:	bot_goal_t goal;
;3270:	bsp_trace_t trace;
;3271:	vec3_t target;
;3272:
;3273:	//if the bot has no enemy
;3274:	if (bs->enemy < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
GEI4 $1117
line 3275
;3275:		return;
ADDRGP4 $1114
JUMPV
LABELV $1117
line 3278
;3276:	}
;3277:	//get the enemy entity information
;3278:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3280
;3279:	//if this is not a player (should be an obelisk)
;3280:	if (bs->enemy >= MAX_CLIENTS) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 64
LTI4 $1119
line 3282
;3281:		//if the obelisk is visible
;3282:		VectorCopy(entinfo.origin, target);
ADDRLP4 828
ADDRLP4 0+24
INDIRB
ASGNB 12
line 3291
;3283:#ifdef MISSIONPACK
;3284:		// if attacking an obelisk
;3285:		if ( bs->enemy == redobelisk.entitynum ||
;3286:			bs->enemy == blueobelisk.entitynum ) {
;3287:			target[2] += 32;
;3288:		}
;3289:#endif
;3290:		//aim at the obelisk
;3291:		VectorSubtract(target, bs->eye, dir);
ADDRLP4 984
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 828
INDIRF4
ADDRLP4 984
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 828+4
INDIRF4
ADDRLP4 984
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 828+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3292
;3292:		vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 140
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3294
;3293:		//set the aim target before trying to attack
;3294:		VectorCopy(target, bs->aimtarget);
ADDRFP4 0
INDIRP4
CNSTI4 6220
ADDP4
ADDRLP4 828
INDIRB
ASGNB 12
line 3295
;3295:		return;
ADDRGP4 $1114
JUMPV
LABELV $1119
line 3300
;3296:	}
;3297:	//
;3298:	//BotAI_Print(PRT_MESSAGE, "client %d: aiming at client %d\n", bs->entitynum, bs->enemy);
;3299:	//
;3300:	aim_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_SKILL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 16
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 984
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 736
ADDRLP4 984
INDIRF4
ASGNF4
line 3301
;3301:	aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 7
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 988
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 988
INDIRF4
ASGNF4
line 3303
;3302:	//
;3303:	if (aim_skill > 0.95) {
ADDRLP4 736
INDIRF4
CNSTF4 1064514355
LEF4 $1126
line 3305
;3304:		//don't aim too early
;3305:		reactiontime = 0.5 * trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_REACTIONTIME, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 6
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 992
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 856
CNSTF4 1056964608
ADDRLP4 992
INDIRF4
MULF4
ASGNF4
line 3306
;3306:		if (bs->enemysight_time > FloatTime() - reactiontime) return;
ADDRFP4 0
INDIRP4
CNSTI4 6132
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
ADDRLP4 856
INDIRF4
SUBF4
LEF4 $1128
ADDRGP4 $1114
JUMPV
LABELV $1128
line 3307
;3307:		if (bs->teleport_time > FloatTime() - reactiontime) return;
ADDRFP4 0
INDIRP4
CNSTI4 6180
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
ADDRLP4 856
INDIRF4
SUBF4
LEF4 $1130
ADDRGP4 $1114
JUMPV
LABELV $1130
line 3308
;3308:	}
LABELV $1126
line 3311
;3309:
;3310:	//get the weapon information
;3311:	trap_BotGetWeaponInfo(bs->ws, bs->weaponnum, &wi);
ADDRLP4 992
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 992
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRLP4 992
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ARGI4
ADDRLP4 160
ARGP4
ADDRGP4 trap_BotGetWeaponInfo
CALLV
pop
line 3313
;3312:	//get the weapon specific aim accuracy and or aim skill
;3313:	if (wi.number == WP_MACHINEGUN) {
ADDRLP4 160+4
INDIRI4
CNSTI4 2
NEI4 $1132
line 3314
;3314:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_MACHINEGUN, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 8
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3315
;3315:	}
ADDRGP4 $1133
JUMPV
LABELV $1132
line 3316
;3316:	else if (wi.number == WP_SHOTGUN) {
ADDRLP4 160+4
INDIRI4
CNSTI4 3
NEI4 $1135
line 3317
;3317:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_SHOTGUN, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 9
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3318
;3318:	}
ADDRGP4 $1136
JUMPV
LABELV $1135
line 3319
;3319:	else if (wi.number == WP_GRENADE_LAUNCHER) {
ADDRLP4 160+4
INDIRI4
CNSTI4 4
NEI4 $1138
line 3320
;3320:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_GRENADELAUNCHER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 11
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3321
;3321:		aim_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_SKILL_GRENADELAUNCHER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 18
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 1000
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 736
ADDRLP4 1000
INDIRF4
ASGNF4
line 3322
;3322:	}
ADDRGP4 $1139
JUMPV
LABELV $1138
line 3323
;3323:	else if (wi.number == WP_ROCKET_LAUNCHER) {
ADDRLP4 160+4
INDIRI4
CNSTI4 5
NEI4 $1141
line 3324
;3324:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_ROCKETLAUNCHER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 10
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3325
;3325:		aim_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_SKILL_ROCKETLAUNCHER, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 17
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 1000
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 736
ADDRLP4 1000
INDIRF4
ASGNF4
line 3326
;3326:	}
ADDRGP4 $1142
JUMPV
LABELV $1141
line 3327
;3327:	else if (wi.number == WP_LIGHTNING) {
ADDRLP4 160+4
INDIRI4
CNSTI4 6
NEI4 $1144
line 3328
;3328:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_LIGHTNING, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 12
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3329
;3329:	}
ADDRGP4 $1145
JUMPV
LABELV $1144
line 3330
;3330:	else if (wi.number == WP_RAILGUN) {
ADDRLP4 160+4
INDIRI4
CNSTI4 7
NEI4 $1147
line 3331
;3331:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_RAILGUN, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 14
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3332
;3332:	}
ADDRGP4 $1148
JUMPV
LABELV $1147
line 3333
;3333:	else if (wi.number == WP_PLASMAGUN) {
ADDRLP4 160+4
INDIRI4
CNSTI4 8
NEI4 $1150
line 3334
;3334:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_PLASMAGUN, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 13
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3335
;3335:		aim_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_SKILL_PLASMAGUN, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 19
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 1000
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 736
ADDRLP4 1000
INDIRF4
ASGNF4
line 3336
;3336:	}
ADDRGP4 $1151
JUMPV
LABELV $1150
line 3337
;3337:	else if (wi.number == WP_BFG) {
ADDRLP4 160+4
INDIRI4
CNSTI4 9
NEI4 $1153
line 3338
;3338:		aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY_BFG10K, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 15
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 996
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 156
ADDRLP4 996
INDIRF4
ASGNF4
line 3339
;3339:		aim_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_SKILL_BFG10K, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 20
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 1000
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 736
ADDRLP4 1000
INDIRF4
ASGNF4
line 3340
;3340:	}
LABELV $1153
LABELV $1151
LABELV $1148
LABELV $1145
LABELV $1142
LABELV $1139
LABELV $1136
LABELV $1133
line 3342
;3341:	//
;3342:	if (aim_accuracy <= 0) aim_accuracy = 0.0001f;
ADDRLP4 156
INDIRF4
CNSTF4 0
GTF4 $1156
ADDRLP4 156
CNSTF4 953267991
ASGNF4
LABELV $1156
line 3344
;3343:	//get the enemy entity information
;3344:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3346
;3345:	//if the enemy is invisible then shoot crappy most of the time
;3346:	if (EntityIsInvisible(&entinfo)) {
ADDRLP4 0
ARGP4
ADDRLP4 996
ADDRGP4 EntityIsInvisible
CALLI4
ASGNI4
ADDRLP4 996
INDIRI4
CNSTI4 0
EQI4 $1158
line 3347
;3347:		if (random() > 0.1) aim_accuracy *= 0.4f;
ADDRLP4 1000
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1000
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1036831949
LEF4 $1160
ADDRLP4 156
CNSTF4 1053609165
ADDRLP4 156
INDIRF4
MULF4
ASGNF4
LABELV $1160
line 3348
;3348:	}
LABELV $1158
line 3350
;3349:	//
;3350:	VectorSubtract(entinfo.origin, entinfo.lastvisorigin, enemyvelocity);
ADDRLP4 712
ADDRLP4 0+24
INDIRF4
ADDRLP4 0+60
INDIRF4
SUBF4
ASGNF4
ADDRLP4 712+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 0+60+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 712+8
ADDRLP4 0+24+8
INDIRF4
ADDRLP4 0+60+8
INDIRF4
SUBF4
ASGNF4
line 3351
;3351:	VectorScale(enemyvelocity, 1 / entinfo.update_time, enemyvelocity);
ADDRLP4 1000
CNSTF4 1065353216
ASGNF4
ADDRLP4 712
ADDRLP4 712
INDIRF4
ADDRLP4 1000
INDIRF4
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 712+4
ADDRLP4 712+4
INDIRF4
ADDRLP4 1000
INDIRF4
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 712+8
ADDRLP4 712+8
INDIRF4
CNSTF4 1065353216
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
line 3353
;3352:	//enemy origin and velocity is remembered every 0.5 seconds
;3353:	if (bs->enemyposition_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6140
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1181
line 3355
;3354:		//
;3355:		bs->enemyposition_time = FloatTime() + 0.5;
ADDRFP4 0
INDIRP4
CNSTI4 6140
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1056964608
ADDF4
ASGNF4
line 3356
;3356:		VectorCopy(enemyvelocity, bs->enemyvelocity);
ADDRFP4 0
INDIRP4
CNSTI4 6232
ADDP4
ADDRLP4 712
INDIRB
ASGNB 12
line 3357
;3357:		VectorCopy(entinfo.origin, bs->enemyorigin);
ADDRFP4 0
INDIRP4
CNSTI4 6244
ADDP4
ADDRLP4 0+24
INDIRB
ASGNB 12
line 3358
;3358:	}
LABELV $1181
line 3360
;3359:	//if not extremely skilled
;3360:	if (aim_skill < 0.9) {
ADDRLP4 736
INDIRF4
CNSTF4 1063675494
GEF4 $1184
line 3361
;3361:		VectorSubtract(entinfo.origin, bs->enemyorigin, dir);
ADDRLP4 1004
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 1004
INDIRP4
CNSTI4 6244
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1004
INDIRP4
CNSTI4 6248
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 6252
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3363
;3362:		//if the enemy moved a bit
;3363:		if (VectorLengthSquared(dir) > Square(48)) {
ADDRLP4 140
ARGP4
ADDRLP4 1008
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1008
INDIRF4
CNSTF4 1158676480
LEF4 $1193
line 3365
;3364:			//if the enemy changed direction
;3365:			if (DotProduct(bs->enemyvelocity, enemyvelocity) < 0) {
ADDRLP4 1012
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1012
INDIRP4
CNSTI4 6232
ADDP4
INDIRF4
ADDRLP4 712
INDIRF4
MULF4
ADDRLP4 1012
INDIRP4
CNSTI4 6236
ADDP4
INDIRF4
ADDRLP4 712+4
INDIRF4
MULF4
ADDF4
ADDRLP4 1012
INDIRP4
CNSTI4 6240
ADDP4
INDIRF4
ADDRLP4 712+8
INDIRF4
MULF4
ADDF4
CNSTF4 0
GEF4 $1195
line 3367
;3366:				//aim accuracy should be worse now
;3367:				aim_accuracy *= 0.7f;
ADDRLP4 156
CNSTF4 1060320051
ADDRLP4 156
INDIRF4
MULF4
ASGNF4
line 3368
;3368:			}
LABELV $1195
line 3369
;3369:		}
LABELV $1193
line 3370
;3370:	}
LABELV $1184
line 3372
;3371:	//check visibility of enemy
;3372:	enemyvisible = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy);
ADDRLP4 1004
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1004
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 1004
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 1004
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 1004
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 1008
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 824
ADDRLP4 1008
INDIRF4
CVFI4 4
ASGNI4
line 3374
;3373:	//if the enemy is visible
;3374:	if (enemyvisible) {
ADDRLP4 824
INDIRI4
CNSTI4 0
EQI4 $1199
line 3376
;3375:		//
;3376:		VectorCopy(entinfo.origin, bestorigin);
ADDRLP4 724
ADDRLP4 0+24
INDIRB
ASGNB 12
line 3377
;3377:		bestorigin[2] += 8;
ADDRLP4 724+8
ADDRLP4 724+8
INDIRF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 3380
;3378:		//get the start point shooting from
;3379:		//NOTE: the x and y projectile start offsets are ignored
;3380:		VectorCopy(bs->origin, start);
ADDRLP4 844
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 3381
;3381:		start[2] += bs->cur_ps.viewheight;
ADDRLP4 844+8
ADDRLP4 844+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 3382
;3382:		start[2] += wi.offset[2];
ADDRLP4 844+8
ADDRLP4 844+8
INDIRF4
ADDRLP4 160+292+8
INDIRF4
ADDF4
ASGNF4
line 3384
;3383:		//
;3384:		BotAI_Trace(&trace, start, mins, maxs, bestorigin, bs->entitynum, MASK_SHOT);
ADDRLP4 740
ARGP4
ADDRLP4 844
ARGP4
ADDRLP4 860
ARGP4
ADDRLP4 872
ARGP4
ADDRLP4 724
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3386
;3385:		//if the enemy is NOT hit
;3386:		if (trace.fraction <= 1 && trace.ent != entinfo.number) {
ADDRLP4 740+8
INDIRF4
CNSTF4 1065353216
GTF4 $1207
ADDRLP4 740+80
INDIRI4
ADDRLP4 0+20
INDIRI4
EQI4 $1207
line 3387
;3387:			bestorigin[2] += 16;
ADDRLP4 724+8
ADDRLP4 724+8
INDIRF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 3388
;3388:		}
LABELV $1207
line 3390
;3389:		//if it is not an instant hit weapon the bot might want to predict the enemy
;3390:		if (wi.speed) {
ADDRLP4 160+272
INDIRF4
CNSTF4 0
EQF4 $1213
line 3392
;3391:			//
;3392:			VectorSubtract(bestorigin, bs->origin, dir);
ADDRLP4 1012
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 724
INDIRF4
ADDRLP4 1012
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 724+4
INDIRF4
ADDRLP4 1012
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 724+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3393
;3393:			dist = VectorLength(dir);
ADDRLP4 140
ARGP4
ADDRLP4 1016
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 840
ADDRLP4 1016
INDIRF4
ASGNF4
line 3394
;3394:			VectorSubtract(entinfo.origin, bs->enemyorigin, dir);
ADDRLP4 1020
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 1020
INDIRP4
CNSTI4 6244
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1020
INDIRP4
CNSTI4 6248
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 6252
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3396
;3395:			//if the enemy is NOT pretty far away and strafing just small steps left and right
;3396:			if (!(dist > 100 && VectorLengthSquared(dir) < Square(32))) {
ADDRLP4 840
INDIRF4
CNSTF4 1120403456
LEF4 $1229
ADDRLP4 140
ARGP4
ADDRLP4 1024
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1024
INDIRF4
CNSTF4 1149239296
LTF4 $1227
LABELV $1229
line 3398
;3397:				//if skilled anough do exact prediction
;3398:				if (aim_skill > 0.8 &&
ADDRLP4 736
INDIRF4
CNSTF4 1061997773
LEF4 $1230
ADDRFP4 0
INDIRP4
CNSTI4 164
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1230
line 3400
;3399:						//if the weapon is ready to fire
;3400:						bs->cur_ps.weaponstate == WEAPON_READY) {
line 3404
;3401:					aas_clientmove_t move;
;3402:					vec3_t origin;
;3403:
;3404:					VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 1124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 1124
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1124
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3406
;3405:					//distance towards the enemy
;3406:					dist = VectorLength(dir);
ADDRLP4 140
ARGP4
ADDRLP4 1128
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 840
ADDRLP4 1128
INDIRF4
ASGNF4
line 3408
;3407:					//direction the enemy is moving in
;3408:					VectorSubtract(entinfo.origin, entinfo.lastvisorigin, dir);
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 0+60
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 0+60+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRLP4 0+60+8
INDIRF4
SUBF4
ASGNF4
line 3410
;3409:					//
;3410:					VectorScale(dir, 1 / entinfo.update_time, dir);
ADDRLP4 1132
CNSTF4 1065353216
ASGNF4
ADDRLP4 140
ADDRLP4 140
INDIRF4
ADDRLP4 1132
INDIRF4
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 140+4
INDIRF4
ADDRLP4 1132
INDIRF4
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 140+8
INDIRF4
CNSTF4 1065353216
ADDRLP4 0+16
INDIRF4
DIVF4
MULF4
ASGNF4
line 3412
;3411:					//
;3412:					VectorCopy(entinfo.origin, origin);
ADDRLP4 1028
ADDRLP4 0+24
INDIRB
ASGNB 12
line 3413
;3413:					origin[2] += 1;
ADDRLP4 1028+8
ADDRLP4 1028+8
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3415
;3414:					//
;3415:					VectorClear(cmdmove);
ADDRLP4 1136
CNSTF4 0
ASGNF4
ADDRLP4 968+8
ADDRLP4 1136
INDIRF4
ASGNF4
ADDRLP4 968+4
ADDRLP4 1136
INDIRF4
ASGNF4
ADDRLP4 968
ADDRLP4 1136
INDIRF4
ASGNF4
line 3417
;3416:					//AAS_ClearShownDebugLines();
;3417:					trap_AAS_PredictClientMovement(&move, bs->enemy, origin,
ADDRLP4 1040
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 1028
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 1140
CNSTI4 0
ASGNI4
ADDRLP4 1140
INDIRI4
ARGI4
ADDRLP4 140
ARGP4
ADDRLP4 968
ARGP4
ADDRLP4 1140
INDIRI4
ARGI4
CNSTF4 1092616192
ADDRLP4 840
INDIRF4
MULF4
ADDRLP4 160+272
INDIRF4
DIVF4
CVFI4 4
ARGI4
CNSTF4 1036831949
ARGF4
ADDRLP4 1140
INDIRI4
ARGI4
ADDRLP4 1140
INDIRI4
ARGI4
ADDRLP4 1140
INDIRI4
ARGI4
ADDRGP4 trap_AAS_PredictClientMovement
CALLI4
pop
line 3421
;3418:														PRESENCE_CROUCH, qfalse,
;3419:														dir, cmdmove, 0,
;3420:														dist * 10 / wi.speed, 0.1f, 0, 0, qfalse);
;3421:					VectorCopy(move.endpos, bestorigin);
ADDRLP4 724
ADDRLP4 1040
INDIRB
ASGNB 12
line 3423
;3422:					//BotAI_Print(PRT_MESSAGE, "%1.1f predicted speed = %f, frames = %f\n", FloatTime(), VectorLength(dir), dist * 10 / wi.speed);
;3423:				}
ADDRGP4 $1231
JUMPV
LABELV $1230
line 3425
;3424:				//if not that skilled do linear prediction
;3425:				else if (aim_skill > 0.4) {
ADDRLP4 736
INDIRF4
CNSTF4 1053609165
LEF4 $1263
line 3426
;3426:					VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 1028
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1028
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3428
;3427:					//distance towards the enemy
;3428:					dist = VectorLength(dir);
ADDRLP4 140
ARGP4
ADDRLP4 1032
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 840
ADDRLP4 1032
INDIRF4
ASGNF4
line 3430
;3429:					//direction the enemy is moving in
;3430:					VectorSubtract(entinfo.origin, entinfo.lastvisorigin, dir);
ADDRLP4 140
ADDRLP4 0+24
INDIRF4
ADDRLP4 0+60
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 0+60+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 0+24+8
INDIRF4
ADDRLP4 0+60+8
INDIRF4
SUBF4
ASGNF4
line 3431
;3431:					dir[2] = 0;
ADDRLP4 140+8
CNSTF4 0
ASGNF4
line 3433
;3432:					//
;3433:					speed = VectorNormalize(dir) / entinfo.update_time;
ADDRLP4 140
ARGP4
ADDRLP4 1036
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 980
ADDRLP4 1036
INDIRF4
ADDRLP4 0+16
INDIRF4
DIVF4
ASGNF4
line 3436
;3434:					//botimport.Print(PRT_MESSAGE, "speed = %f, wi->speed = %f\n", speed, wi->speed);
;3435:					//best spot to aim at
;3436:					VectorMA(entinfo.origin, (dist / wi.speed) * speed, dir, bestorigin);
ADDRLP4 1040
ADDRLP4 840
INDIRF4
ASGNF4
ADDRLP4 1044
ADDRLP4 980
INDIRF4
ASGNF4
ADDRLP4 724
ADDRLP4 0+24
INDIRF4
ADDRLP4 140
INDIRF4
ADDRLP4 1040
INDIRF4
ADDRLP4 160+272
INDIRF4
DIVF4
ADDRLP4 1044
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRLP4 724+4
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 140+4
INDIRF4
ADDRLP4 1040
INDIRF4
ADDRLP4 160+272
INDIRF4
DIVF4
ADDRLP4 1044
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRLP4 724+8
ADDRLP4 0+24+8
INDIRF4
ADDRLP4 140+8
INDIRF4
ADDRLP4 840
INDIRF4
ADDRLP4 160+272
INDIRF4
DIVF4
ADDRLP4 980
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
line 3437
;3437:				}
LABELV $1263
LABELV $1231
line 3438
;3438:			}
LABELV $1227
line 3439
;3439:		}
LABELV $1213
line 3441
;3440:		//if the projectile does radial damage
;3441:		if (aim_skill > 0.6 && wi.proj.damagetype & DAMAGETYPE_RADIAL) {
ADDRLP4 736
INDIRF4
CNSTF4 1058642330
LEF4 $1298
ADDRLP4 160+344+180
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1298
line 3443
;3442:			//if the enemy isn't standing significantly higher than the bot
;3443:			if (entinfo.origin[2] < bs->origin[2] + 16) {
ADDRLP4 0+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
CNSTF4 1098907648
ADDF4
GEF4 $1302
line 3445
;3444:				//try to aim at the ground in front of the enemy
;3445:				VectorCopy(entinfo.origin, end);
ADDRLP4 956
ADDRLP4 0+24
INDIRB
ASGNB 12
line 3446
;3446:				end[2] -= 64;
ADDRLP4 956+8
ADDRLP4 956+8
INDIRF4
CNSTF4 1115684864
SUBF4
ASGNF4
line 3447
;3447:				BotAI_Trace(&trace, entinfo.origin, NULL, NULL, end, entinfo.number, MASK_SHOT);
ADDRLP4 740
ARGP4
ADDRLP4 0+24
ARGP4
ADDRLP4 1012
CNSTP4 0
ASGNP4
ADDRLP4 1012
INDIRP4
ARGP4
ADDRLP4 1012
INDIRP4
ARGP4
ADDRLP4 956
ARGP4
ADDRLP4 0+20
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3449
;3448:				//
;3449:				VectorCopy(bestorigin, groundtarget);
ADDRLP4 944
ADDRLP4 724
INDIRB
ASGNB 12
line 3450
;3450:				if (trace.startsolid) groundtarget[2] = entinfo.origin[2] - 16;
ADDRLP4 740+4
INDIRI4
CNSTI4 0
EQI4 $1310
ADDRLP4 944+8
ADDRLP4 0+24+8
INDIRF4
CNSTF4 1098907648
SUBF4
ASGNF4
ADDRGP4 $1311
JUMPV
LABELV $1310
line 3451
;3451:				else groundtarget[2] = trace.endpos[2] - 8;
ADDRLP4 944+8
ADDRLP4 740+12+8
INDIRF4
CNSTF4 1090519040
SUBF4
ASGNF4
LABELV $1311
line 3453
;3452:				//trace a line from projectile start to ground target
;3453:				BotAI_Trace(&trace, start, NULL, NULL, groundtarget, bs->entitynum, MASK_SHOT);
ADDRLP4 740
ARGP4
ADDRLP4 844
ARGP4
ADDRLP4 1016
CNSTP4 0
ASGNP4
ADDRLP4 1016
INDIRP4
ARGP4
ADDRLP4 1016
INDIRP4
ARGP4
ADDRLP4 944
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3455
;3454:				//if hitpoint is not vertically too far from the ground target
;3455:				if (fabs(trace.endpos[2] - groundtarget[2]) < 50) {
ADDRLP4 740+12+8
INDIRF4
ADDRLP4 944+8
INDIRF4
SUBF4
ARGF4
ADDRLP4 1020
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 1020
INDIRF4
CNSTF4 1112014848
GEF4 $1319
line 3456
;3456:					VectorSubtract(trace.endpos, groundtarget, dir);
ADDRLP4 140
ADDRLP4 740+12
INDIRF4
ADDRLP4 944
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 740+12+4
INDIRF4
ADDRLP4 944+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 740+12+8
INDIRF4
ADDRLP4 944+8
INDIRF4
SUBF4
ASGNF4
line 3458
;3457:					//if the hitpoint is near anough the ground target
;3458:					if (VectorLengthSquared(dir) < Square(60)) {
ADDRLP4 140
ARGP4
ADDRLP4 1024
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1024
INDIRF4
CNSTF4 1163984896
GEF4 $1333
line 3459
;3459:						VectorSubtract(trace.endpos, start, dir);
ADDRLP4 140
ADDRLP4 740+12
INDIRF4
ADDRLP4 844
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 740+12+4
INDIRF4
ADDRLP4 844+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 740+12+8
INDIRF4
ADDRLP4 844+8
INDIRF4
SUBF4
ASGNF4
line 3461
;3460:						//if the hitpoint is far anough from the bot
;3461:						if (VectorLengthSquared(dir) > Square(100)) {
ADDRLP4 140
ARGP4
ADDRLP4 1028
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1028
INDIRF4
CNSTF4 1176256512
LEF4 $1344
line 3463
;3462:							//check if the bot is visible from the ground target
;3463:							trace.endpos[2] += 1;
ADDRLP4 740+12+8
ADDRLP4 740+12+8
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3464
;3464:							BotAI_Trace(&trace, trace.endpos, NULL, NULL, entinfo.origin, entinfo.number, MASK_SHOT);
ADDRLP4 740
ARGP4
ADDRLP4 740+12
ARGP4
ADDRLP4 1032
CNSTP4 0
ASGNP4
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0+24
ARGP4
ADDRLP4 0+20
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3465
;3465:							if (trace.fraction >= 1) {
ADDRLP4 740+8
INDIRF4
CNSTF4 1065353216
LTF4 $1351
line 3467
;3466:								//botimport.Print(PRT_MESSAGE, "%1.1f aiming at ground\n", AAS_Time());
;3467:								VectorCopy(groundtarget, bestorigin);
ADDRLP4 724
ADDRLP4 944
INDIRB
ASGNB 12
line 3468
;3468:							}
LABELV $1351
line 3469
;3469:						}
LABELV $1344
line 3470
;3470:					}
LABELV $1333
line 3471
;3471:				}
LABELV $1319
line 3472
;3472:			}
LABELV $1302
line 3473
;3473:		}
LABELV $1298
line 3474
;3474:		bestorigin[0] += 20 * crandom() * (1 - aim_accuracy);
ADDRLP4 1012
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 724
ADDRLP4 724
INDIRF4
CNSTF4 1101004800
CNSTF4 1073741824
ADDRLP4 1012
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3475
;3475:		bestorigin[1] += 20 * crandom() * (1 - aim_accuracy);
ADDRLP4 1016
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 724+4
ADDRLP4 724+4
INDIRF4
CNSTF4 1101004800
CNSTF4 1073741824
ADDRLP4 1016
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3476
;3476:		bestorigin[2] += 10 * crandom() * (1 - aim_accuracy);
ADDRLP4 1020
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 724+8
ADDRLP4 724+8
INDIRF4
CNSTF4 1092616192
CNSTF4 1073741824
ADDRLP4 1020
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3477
;3477:	}
ADDRGP4 $1200
JUMPV
LABELV $1199
line 3478
;3478:	else {
line 3480
;3479:		//
;3480:		VectorCopy(bs->lastenemyorigin, bestorigin);
ADDRLP4 724
ADDRFP4 0
INDIRP4
CNSTI4 6548
ADDP4
INDIRB
ASGNB 12
line 3481
;3481:		bestorigin[2] += 8;
ADDRLP4 724+8
ADDRLP4 724+8
INDIRF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 3483
;3482:		//if the bot is skilled anough
;3483:		if (aim_skill > 0.5) {
ADDRLP4 736
INDIRF4
CNSTF4 1056964608
LEF4 $1357
line 3485
;3484:			//do prediction shots around corners
;3485:			if (wi.number == WP_BFG ||
ADDRLP4 160+4
INDIRI4
CNSTI4 9
EQI4 $1365
ADDRLP4 160+4
INDIRI4
CNSTI4 5
EQI4 $1365
ADDRLP4 160+4
INDIRI4
CNSTI4 4
NEI4 $1359
LABELV $1365
line 3487
;3486:				wi.number == WP_ROCKET_LAUNCHER ||
;3487:				wi.number == WP_GRENADE_LAUNCHER) {
line 3489
;3488:				//create the chase goal
;3489:				goal.entitynum = bs->client;
ADDRLP4 884+40
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 3490
;3490:				goal.areanum = bs->areanum;
ADDRLP4 884+12
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ASGNI4
line 3491
;3491:				VectorCopy(bs->eye, goal.origin);
ADDRLP4 884
ADDRFP4 0
INDIRP4
CNSTI4 4936
ADDP4
INDIRB
ASGNB 12
line 3492
;3492:				VectorSet(goal.mins, -8, -8, -8);
ADDRLP4 884+16
CNSTF4 3238002688
ASGNF4
ADDRLP4 884+16+4
CNSTF4 3238002688
ASGNF4
ADDRLP4 884+16+8
CNSTF4 3238002688
ASGNF4
line 3493
;3493:				VectorSet(goal.maxs, 8, 8, 8);
ADDRLP4 884+28
CNSTF4 1090519040
ASGNF4
ADDRLP4 884+28+4
CNSTF4 1090519040
ASGNF4
ADDRLP4 884+28+8
CNSTF4 1090519040
ASGNF4
line 3495
;3494:				//
;3495:				if (trap_BotPredictVisiblePosition(bs->lastenemyorigin, bs->lastenemyareanum, &goal, TFL_DEFAULT, target)) {
ADDRLP4 1012
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1012
INDIRP4
CNSTI4 6548
ADDP4
ARGP4
ADDRLP4 1012
INDIRP4
CNSTI4 6544
ADDP4
INDIRI4
ARGI4
ADDRLP4 884
ARGP4
CNSTI4 18616254
ARGI4
ADDRLP4 828
ARGP4
ADDRLP4 1016
ADDRGP4 trap_BotPredictVisiblePosition
CALLI4
ASGNI4
ADDRLP4 1016
INDIRI4
CNSTI4 0
EQI4 $1378
line 3496
;3496:					VectorSubtract(target, bs->eye, dir);
ADDRLP4 1020
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 828
INDIRF4
ADDRLP4 1020
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 828+4
INDIRF4
ADDRLP4 1020
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 828+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3497
;3497:					if (VectorLengthSquared(dir) > Square(80)) {
ADDRLP4 140
ARGP4
ADDRLP4 1024
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1024
INDIRF4
CNSTF4 1170735104
LEF4 $1384
line 3498
;3498:						VectorCopy(target, bestorigin);
ADDRLP4 724
ADDRLP4 828
INDIRB
ASGNB 12
line 3499
;3499:						bestorigin[2] -= 20;
ADDRLP4 724+8
ADDRLP4 724+8
INDIRF4
CNSTF4 1101004800
SUBF4
ASGNF4
line 3500
;3500:					}
LABELV $1384
line 3501
;3501:				}
LABELV $1378
line 3502
;3502:				aim_accuracy = 1;
ADDRLP4 156
CNSTF4 1065353216
ASGNF4
line 3503
;3503:			}
LABELV $1359
line 3504
;3504:		}
LABELV $1357
line 3505
;3505:	}
LABELV $1200
line 3507
;3506:	//
;3507:	if (enemyvisible) {
ADDRLP4 824
INDIRI4
CNSTI4 0
EQI4 $1387
line 3508
;3508:		BotAI_Trace(&trace, bs->eye, NULL, NULL, bestorigin, bs->entitynum, MASK_SHOT);
ADDRLP4 740
ARGP4
ADDRLP4 1012
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1012
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 1016
CNSTP4 0
ASGNP4
ADDRLP4 1016
INDIRP4
ARGP4
ADDRLP4 1016
INDIRP4
ARGP4
ADDRLP4 724
ARGP4
ADDRLP4 1012
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3509
;3509:		VectorCopy(trace.endpos, bs->aimtarget);
ADDRFP4 0
INDIRP4
CNSTI4 6220
ADDP4
ADDRLP4 740+12
INDIRB
ASGNB 12
line 3510
;3510:	}
ADDRGP4 $1388
JUMPV
LABELV $1387
line 3511
;3511:	else {
line 3512
;3512:		VectorCopy(bestorigin, bs->aimtarget);
ADDRFP4 0
INDIRP4
CNSTI4 6220
ADDP4
ADDRLP4 724
INDIRB
ASGNB 12
line 3513
;3513:	}
LABELV $1388
line 3515
;3514:	//get aim direction
;3515:	VectorSubtract(bestorigin, bs->eye, dir);
ADDRLP4 1012
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 724
INDIRF4
ADDRLP4 1012
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 724+4
INDIRF4
ADDRLP4 1012
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 140+8
ADDRLP4 724+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3517
;3516:	//
;3517:	if (wi.number == WP_MACHINEGUN ||
ADDRLP4 160+4
INDIRI4
CNSTI4 2
EQI4 $1402
ADDRLP4 160+4
INDIRI4
CNSTI4 3
EQI4 $1402
ADDRLP4 160+4
INDIRI4
CNSTI4 6
EQI4 $1402
ADDRLP4 160+4
INDIRI4
CNSTI4 7
NEI4 $1394
LABELV $1402
line 3520
;3518:		wi.number == WP_SHOTGUN ||
;3519:		wi.number == WP_LIGHTNING ||
;3520:		wi.number == WP_RAILGUN) {
line 3522
;3521:		//distance towards the enemy
;3522:		dist = VectorLength(dir);
ADDRLP4 140
ARGP4
ADDRLP4 1016
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 840
ADDRLP4 1016
INDIRF4
ASGNF4
line 3523
;3523:		if (dist > 150) dist = 150;
ADDRLP4 840
INDIRF4
CNSTF4 1125515264
LEF4 $1403
ADDRLP4 840
CNSTF4 1125515264
ASGNF4
LABELV $1403
line 3524
;3524:		f = 0.6 + dist / 150 * 0.4;
ADDRLP4 940
CNSTF4 1053609165
ADDRLP4 840
INDIRF4
CNSTF4 1125515264
DIVF4
MULF4
CNSTF4 1058642330
ADDF4
ASGNF4
line 3525
;3525:		aim_accuracy *= f;
ADDRLP4 156
ADDRLP4 156
INDIRF4
ADDRLP4 940
INDIRF4
MULF4
ASGNF4
line 3526
;3526:	}
LABELV $1394
line 3528
;3527:	//add some random stuff to the aim direction depending on the aim accuracy
;3528:	if (aim_accuracy < 0.8) {
ADDRLP4 156
INDIRF4
CNSTF4 1061997773
GEF4 $1405
line 3529
;3529:		VectorNormalize(dir);
ADDRLP4 140
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 3530
;3530:		for (i = 0; i < 3; i++) dir[i] += 0.3 * crandom() * (1 - aim_accuracy);
ADDRLP4 152
CNSTI4 0
ASGNI4
LABELV $1407
ADDRLP4 1016
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1020
ADDRLP4 152
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 140
ADDP4
ASGNP4
ADDRLP4 1020
INDIRP4
ADDRLP4 1020
INDIRP4
INDIRF4
CNSTF4 1050253722
CNSTF4 1073741824
ADDRLP4 1016
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
LABELV $1408
ADDRLP4 152
ADDRLP4 152
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 3
LTI4 $1407
line 3531
;3531:	}
LABELV $1405
line 3533
;3532:	//set the ideal view angles
;3533:	vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 140
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3535
;3534:	//take the weapon spread into account for lower skilled bots
;3535:	bs->ideal_viewangles[PITCH] += 6 * wi.vspread * crandom() * (1 - aim_accuracy);
ADDRLP4 1016
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1020
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 1020
INDIRP4
ADDRLP4 1020
INDIRP4
INDIRF4
CNSTF4 1086324736
ADDRLP4 160+268
INDIRF4
MULF4
CNSTF4 1073741824
ADDRLP4 1016
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3536
;3536:	bs->ideal_viewangles[PITCH] = AngleMod(bs->ideal_viewangles[PITCH]);
ADDRLP4 1024
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 1024
INDIRP4
INDIRF4
ARGF4
ADDRLP4 1028
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1024
INDIRP4
ADDRLP4 1028
INDIRF4
ASGNF4
line 3537
;3537:	bs->ideal_viewangles[YAW] += 6 * wi.hspread * crandom() * (1 - aim_accuracy);
ADDRLP4 1032
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1036
ADDRFP4 0
INDIRP4
CNSTI4 6580
ADDP4
ASGNP4
ADDRLP4 1036
INDIRP4
ADDRLP4 1036
INDIRP4
INDIRF4
CNSTF4 1086324736
ADDRLP4 160+264
INDIRF4
MULF4
CNSTF4 1073741824
ADDRLP4 1032
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
CNSTF4 1065353216
ADDRLP4 156
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3538
;3538:	bs->ideal_viewangles[YAW] = AngleMod(bs->ideal_viewangles[YAW]);
ADDRLP4 1040
ADDRFP4 0
INDIRP4
CNSTI4 6580
ADDP4
ASGNP4
ADDRLP4 1040
INDIRP4
INDIRF4
ARGF4
ADDRLP4 1044
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1040
INDIRP4
ADDRLP4 1044
INDIRF4
ASGNF4
line 3540
;3539:	//if the bots should be really challenging
;3540:	if (bot_challenge.integer) {
ADDRGP4 bot_challenge+12
INDIRI4
CNSTI4 0
EQI4 $1413
line 3542
;3541:		//if the bot is really accurate and has the enemy in view for some time
;3542:		if (aim_accuracy > 0.9 && bs->enemysight_time < FloatTime() - 1) {
ADDRLP4 156
INDIRF4
CNSTF4 1063675494
LEF4 $1416
ADDRFP4 0
INDIRP4
CNSTI4 6132
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
GEF4 $1416
line 3544
;3543:			//set the view angles directly
;3544:			if (bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
INDIRF4
CNSTF4 1127481344
LEF4 $1418
ADDRLP4 1048
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 1048
INDIRP4
ADDRLP4 1048
INDIRP4
INDIRF4
CNSTF4 1135869952
SUBF4
ASGNF4
LABELV $1418
line 3545
;3545:			VectorCopy(bs->ideal_viewangles, bs->viewangles);
ADDRLP4 1052
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1052
INDIRP4
CNSTI4 6564
ADDP4
ADDRLP4 1052
INDIRP4
CNSTI4 6576
ADDP4
INDIRB
ASGNB 12
line 3546
;3546:			trap_EA_View(bs->client, bs->viewangles);
ADDRLP4 1056
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1056
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 1056
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRGP4 trap_EA_View
CALLV
pop
line 3547
;3547:		}
LABELV $1416
line 3548
;3548:	}
LABELV $1413
line 3549
;3549:}
LABELV $1114
endproc BotAimAtEnemy 1144 52
lit
align 4
LABELV $1421
byte 4 3238002688
byte 4 3238002688
byte 4 3238002688
align 4
LABELV $1422
byte 4 1090519040
byte 4 1090519040
byte 4 1090519040
export BotCheckAttack
code
proc BotCheckAttack 1028 28
line 3556
;3550:
;3551:/*
;3552:==================
;3553:BotCheckAttack
;3554:==================
;3555:*/
;3556:void BotCheckAttack(bot_state_t *bs) {
line 3565
;3557:	float points, reactiontime, fov, firethrottle;
;3558:	int attackentity;
;3559:	bsp_trace_t bsptrace;
;3560:	//float selfpreservation;
;3561:	vec3_t forward, right, start, end, dir, angles;
;3562:	weaponinfo_t wi;
;3563:	bsp_trace_t trace;
;3564:	aas_entityinfo_t entinfo;
;3565:	vec3_t mins = {-8, -8, -8}, maxs = {8, 8, 8};
ADDRLP4 808
ADDRGP4 $1421
INDIRB
ASGNB 12
ADDRLP4 820
ADDRGP4 $1422
INDIRB
ASGNB 12
line 3567
;3566:
;3567:	attackentity = bs->enemy;
ADDRLP4 576
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ASGNI4
line 3569
;3568:	//
;3569:	BotEntityInfo(attackentity, &entinfo);
ADDRLP4 576
INDIRI4
ARGI4
ADDRLP4 832
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3571
;3570:	// if not attacking a player
;3571:	if (attackentity >= MAX_CLIENTS) {
ADDRLP4 576
INDIRI4
CNSTI4 64
LTI4 $1423
line 3583
;3572:#ifdef MISSIONPACK
;3573:		// if attacking an obelisk
;3574:		if ( entinfo.number == redobelisk.entitynum ||
;3575:			entinfo.number == blueobelisk.entitynum ) {
;3576:			// if obelisk is respawning return
;3577:			if ( g_entities[entinfo.number].activator &&
;3578:				g_entities[entinfo.number].activator->s.frame == 2 ) {
;3579:				return;
;3580:			}
;3581:		}
;3582:#endif
;3583:	}
LABELV $1423
line 3585
;3584:	//
;3585:	reactiontime = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_REACTIONTIME, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 6
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 976
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 700
ADDRLP4 976
INDIRF4
ASGNF4
line 3586
;3586:	if (bs->enemysight_time > FloatTime() - reactiontime) return;
ADDRFP4 0
INDIRP4
CNSTI4 6132
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
ADDRLP4 700
INDIRF4
SUBF4
LEF4 $1425
ADDRGP4 $1420
JUMPV
LABELV $1425
line 3587
;3587:	if (bs->teleport_time > FloatTime() - reactiontime) return;
ADDRFP4 0
INDIRP4
CNSTI4 6180
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
ADDRLP4 700
INDIRF4
SUBF4
LEF4 $1427
ADDRGP4 $1420
JUMPV
LABELV $1427
line 3589
;3588:	//if changing weapons
;3589:	if (bs->weaponchange_time > FloatTime() - 0.1) return;
ADDRFP4 0
INDIRP4
CNSTI4 6192
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1036831949
SUBF4
LEF4 $1429
ADDRGP4 $1420
JUMPV
LABELV $1429
line 3591
;3590:	//check fire throttle characteristic
;3591:	if (bs->firethrottlewait_time > FloatTime()) return;
ADDRFP4 0
INDIRP4
CNSTI4 6196
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $1431
ADDRGP4 $1420
JUMPV
LABELV $1431
line 3592
;3592:	firethrottle = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_FIRETHROTTLE, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 47
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 980
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 792
ADDRLP4 980
INDIRF4
ASGNF4
line 3593
;3593:	if (bs->firethrottleshoot_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6200
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1433
line 3594
;3594:		if (random() > firethrottle) {
ADDRLP4 984
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 984
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 792
INDIRF4
LEF4 $1435
line 3595
;3595:			bs->firethrottlewait_time = FloatTime() + firethrottle;
ADDRFP4 0
INDIRP4
CNSTI4 6196
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 792
INDIRF4
ADDF4
ASGNF4
line 3596
;3596:			bs->firethrottleshoot_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6200
ADDP4
CNSTF4 0
ASGNF4
line 3597
;3597:		}
ADDRGP4 $1436
JUMPV
LABELV $1435
line 3598
;3598:		else {
line 3599
;3599:			bs->firethrottleshoot_time = FloatTime() + 1 - firethrottle;
ADDRFP4 0
INDIRP4
CNSTI4 6200
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ADDRLP4 792
INDIRF4
SUBF4
ASGNF4
line 3600
;3600:			bs->firethrottlewait_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6196
ADDP4
CNSTF4 0
ASGNF4
line 3601
;3601:		}
LABELV $1436
line 3602
;3602:	}
LABELV $1433
line 3605
;3603:	//
;3604:	//
;3605:	VectorSubtract(bs->aimtarget, bs->eye, dir);
ADDRLP4 984
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 580
ADDRLP4 984
INDIRP4
CNSTI4 6220
ADDP4
INDIRF4
ADDRLP4 984
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 580+4
ADDRLP4 984
INDIRP4
CNSTI4 6224
ADDP4
INDIRF4
ADDRLP4 984
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 988
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 580+8
ADDRLP4 988
INDIRP4
CNSTI4 6228
ADDP4
INDIRF4
ADDRLP4 988
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3607
;3606:	//
;3607:	if (bs->weaponnum == WP_GAUNTLET) {
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1439
line 3608
;3608:		if (VectorLengthSquared(dir) > Square(60)) {
ADDRLP4 580
ARGP4
ADDRLP4 992
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 992
INDIRF4
CNSTF4 1163984896
LEF4 $1441
line 3609
;3609:			return;
ADDRGP4 $1420
JUMPV
LABELV $1441
line 3611
;3610:		}
;3611:	}
LABELV $1439
line 3612
;3612:	if (VectorLengthSquared(dir) < Square(100))
ADDRLP4 580
ARGP4
ADDRLP4 992
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 992
INDIRF4
CNSTF4 1176256512
GEF4 $1443
line 3613
;3613:		fov = 120;
ADDRLP4 788
CNSTF4 1123024896
ASGNF4
ADDRGP4 $1444
JUMPV
LABELV $1443
line 3615
;3614:	else
;3615:		fov = 50;
ADDRLP4 788
CNSTF4 1112014848
ASGNF4
LABELV $1444
line 3617
;3616:	//
;3617:	vectoangles(dir, angles);
ADDRLP4 580
ARGP4
ADDRLP4 796
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3618
;3618:	if (!InFieldOfVision(bs->viewangles, fov, angles))
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRLP4 788
INDIRF4
ARGF4
ADDRLP4 796
ARGP4
ADDRLP4 996
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 996
INDIRI4
CNSTI4 0
NEI4 $1445
line 3619
;3619:		return;
ADDRGP4 $1420
JUMPV
LABELV $1445
line 3620
;3620:	BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, bs->aimtarget, bs->client, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
ADDRLP4 704
ARGP4
ADDRLP4 1000
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1000
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 1004
CNSTP4 0
ASGNP4
ADDRLP4 1004
INDIRP4
ARGP4
ADDRLP4 1004
INDIRP4
ARGP4
ADDRLP4 1000
INDIRP4
CNSTI4 6220
ADDP4
ARGP4
ADDRLP4 1000
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 65537
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3621
;3621:	if (bsptrace.fraction < 1 && bsptrace.ent != attackentity)
ADDRLP4 704+8
INDIRF4
CNSTF4 1065353216
GEF4 $1447
ADDRLP4 704+80
INDIRI4
ADDRLP4 576
INDIRI4
EQI4 $1447
line 3622
;3622:		return;
ADDRGP4 $1420
JUMPV
LABELV $1447
line 3625
;3623:
;3624:	//get the weapon info
;3625:	trap_BotGetWeaponInfo(bs->ws, bs->weaponnum, &wi);
ADDRLP4 1008
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1008
INDIRP4
CNSTI4 6536
ADDP4
INDIRI4
ARGI4
ADDRLP4 1008
INDIRP4
CNSTI4 6560
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
ARGP4
ADDRGP4 trap_BotGetWeaponInfo
CALLV
pop
line 3627
;3626:	//get the start point shooting from
;3627:	VectorCopy(bs->origin, start);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 3628
;3628:	start[2] += bs->cur_ps.viewheight;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 3629
;3629:	AngleVectors(bs->viewangles, forward, right, NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 676
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 3630
;3630:	start[0] += forward[0] * wi.offset[0] + right[0] * wi.offset[1];
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 24+292
INDIRF4
MULF4
ADDRLP4 676
INDIRF4
ADDRLP4 24+292+4
INDIRF4
MULF4
ADDF4
ADDF4
ASGNF4
line 3631
;3631:	start[1] += forward[1] * wi.offset[0] + right[1] * wi.offset[1];
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 24+292
INDIRF4
MULF4
ADDRLP4 676+4
INDIRF4
ADDRLP4 24+292+4
INDIRF4
MULF4
ADDF4
ADDF4
ASGNF4
line 3632
;3632:	start[2] += forward[2] * wi.offset[0] + right[2] * wi.offset[1] + wi.offset[2];
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 12+8
INDIRF4
ADDRLP4 24+292
INDIRF4
MULF4
ADDRLP4 676+8
INDIRF4
ADDRLP4 24+292+4
INDIRF4
MULF4
ADDF4
ADDRLP4 24+292+8
INDIRF4
ADDF4
ADDF4
ASGNF4
line 3634
;3633:	//end point aiming at
;3634:	VectorMA(start, 1000, forward, end);
ADDRLP4 1012
CNSTF4 1148846080
ASGNF4
ADDRLP4 688
ADDRLP4 0
INDIRF4
ADDRLP4 1012
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 688+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1012
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 688+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1148846080
ADDRLP4 12+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 3636
;3635:	//a little back to make sure not inside a very close enemy
;3636:	VectorMA(start, -12, forward, start);
ADDRLP4 1016
CNSTF4 3242196992
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 1016
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1016
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 3242196992
ADDRLP4 12+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 3637
;3637:	BotAI_Trace(&trace, start, mins, maxs, end, bs->entitynum, MASK_SHOT);
ADDRLP4 592
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 808
ARGP4
ADDRLP4 820
ARGP4
ADDRLP4 688
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3639
;3638:	//if the entity is a client
;3639:	if (trace.ent > 0 && trace.ent <= MAX_CLIENTS) {
ADDRLP4 592+80
INDIRI4
CNSTI4 0
LEI4 $1481
ADDRLP4 592+80
INDIRI4
CNSTI4 64
GTI4 $1481
line 3640
;3640:		if (trace.ent != attackentity) {
ADDRLP4 592+80
INDIRI4
ADDRLP4 576
INDIRI4
EQI4 $1485
line 3642
;3641:			//if a teammate is hit
;3642:			if (BotSameTeam(bs, trace.ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 592+80
INDIRI4
ARGI4
ADDRLP4 1020
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1020
INDIRI4
CNSTI4 0
EQI4 $1488
line 3643
;3643:				return;
ADDRGP4 $1420
JUMPV
LABELV $1488
line 3644
;3644:		}
LABELV $1485
line 3645
;3645:	}
LABELV $1481
line 3647
;3646:	//if won't hit the enemy or not attacking a player (obelisk)
;3647:	if (trace.ent != attackentity || attackentity >= MAX_CLIENTS) {
ADDRLP4 592+80
INDIRI4
ADDRLP4 576
INDIRI4
NEI4 $1494
ADDRLP4 576
INDIRI4
CNSTI4 64
LTI4 $1491
LABELV $1494
line 3649
;3648:		//if the projectile does radial damage
;3649:		if (wi.proj.damagetype & DAMAGETYPE_RADIAL) {
ADDRLP4 24+344+180
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1495
line 3650
;3650:			if (trace.fraction * 1000 < wi.proj.radius) {
CNSTF4 1148846080
ADDRLP4 592+8
INDIRF4
MULF4
ADDRLP4 24+344+172
INDIRF4
GEF4 $1499
line 3651
;3651:				points = (wi.proj.damage - 0.5 * trace.fraction * 1000) * 0.5;
ADDRLP4 1024
CNSTF4 1056964608
ASGNF4
ADDRLP4 972
ADDRLP4 1024
INDIRF4
ADDRLP4 24+344+168
INDIRI4
CVIF4 4
CNSTF4 1148846080
ADDRLP4 1024
INDIRF4
ADDRLP4 592+8
INDIRF4
MULF4
MULF4
SUBF4
MULF4
ASGNF4
line 3652
;3652:				if (points > 0) {
ADDRLP4 972
INDIRF4
CNSTF4 0
LEF4 $1507
line 3653
;3653:					return;
ADDRGP4 $1420
JUMPV
LABELV $1507
line 3655
;3654:				}
;3655:			}
LABELV $1499
line 3657
;3656:			//FIXME: check if a teammate gets radial damage
;3657:		}
LABELV $1495
line 3658
;3658:	}
LABELV $1491
line 3660
;3659:	//if fire has to be release to activate weapon
;3660:	if (wi.flags & WFL_FIRERELEASED) {
ADDRLP4 24+176
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $1509
line 3661
;3661:		if (bs->flags & BFL_ATTACKED) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1510
line 3662
;3662:			trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 3663
;3663:		}
line 3664
;3664:	}
ADDRGP4 $1510
JUMPV
LABELV $1509
line 3665
;3665:	else {
line 3666
;3666:		trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 3667
;3667:	}
LABELV $1510
line 3668
;3668:	bs->flags ^= BFL_ATTACKED;
ADDRLP4 1024
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 1024
INDIRP4
ADDRLP4 1024
INDIRP4
INDIRI4
CNSTI4 2
BXORI4
ASGNI4
line 3669
;3669:}
LABELV $1420
endproc BotCheckAttack 1028 28
lit
align 4
LABELV $1520
byte 4 1143930880
byte 4 1129054208
byte 4 1143472128
align 4
LABELV $1521
byte 4 1148256256
byte 4 1139408896
byte 4 1143603200
align 4
LABELV $1522
byte 4 1134034944
byte 4 1135607808
byte 4 1147535360
export BotMapScripts
code
proc BotMapScripts 1424 16
line 3676
;3670:
;3671:/*
;3672:==================
;3673:BotMapScripts
;3674:==================
;3675:*/
;3676:void BotMapScripts(bot_state_t *bs) {
line 3684
;3677:	char info[1024];
;3678:	char mapname[128];
;3679:	int i, shootbutton;
;3680:	float aim_accuracy;
;3681:	aas_entityinfo_t entinfo;
;3682:	vec3_t dir;
;3683:
;3684:	trap_GetServerinfo(info, sizeof(info));
ADDRLP4 272
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetServerinfo
CALLV
pop
line 3686
;3685:
;3686:	strncpy(mapname, Info_ValueForKey( info, "mapname" ), sizeof(mapname)-1);
ADDRLP4 272
ARGP4
ADDRGP4 $1515
ARGP4
ADDRLP4 1316
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 144
ARGP4
ADDRLP4 1316
INDIRP4
ARGP4
CNSTI4 127
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 3687
;3687:	mapname[sizeof(mapname)-1] = '\0';
ADDRLP4 144+127
CNSTI1 0
ASGNI1
line 3689
;3688:
;3689:	if (!Q_stricmp(mapname, "q3tourney6")) {
ADDRLP4 144
ARGP4
ADDRGP4 $1519
ARGP4
ADDRLP4 1320
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1320
INDIRI4
CNSTI4 0
NEI4 $1517
line 3690
;3690:		vec3_t mins = {700, 204, 672}, maxs = {964, 468, 680};
ADDRLP4 1324
ADDRGP4 $1520
INDIRB
ASGNB 12
ADDRLP4 1336
ADDRGP4 $1521
INDIRB
ASGNB 12
line 3691
;3691:		vec3_t buttonorg = {304, 352, 920};
ADDRLP4 1348
ADDRGP4 $1522
INDIRB
ASGNB 12
line 3693
;3692:		//NOTE: NEVER use the func_bobbing in q3tourney6
;3693:		bs->tfl &= ~TFL_FUNCBOB;
ADDRLP4 1360
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 1360
INDIRP4
ADDRLP4 1360
INDIRP4
INDIRI4
CNSTI4 -16777217
BANDI4
ASGNI4
line 3695
;3694:		//if the bot is below the bounding box
;3695:		if (bs->origin[0] > mins[0] && bs->origin[0] < maxs[0]) {
ADDRLP4 1364
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ASGNF4
ADDRLP4 1364
INDIRF4
ADDRLP4 1324
INDIRF4
LEF4 $1523
ADDRLP4 1364
INDIRF4
ADDRLP4 1336
INDIRF4
GEF4 $1523
line 3696
;3696:			if (bs->origin[1] > mins[1] && bs->origin[1] < maxs[1]) {
ADDRLP4 1368
ADDRFP4 0
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ASGNF4
ADDRLP4 1368
INDIRF4
ADDRLP4 1324+4
INDIRF4
LEF4 $1525
ADDRLP4 1368
INDIRF4
ADDRLP4 1336+4
INDIRF4
GEF4 $1525
line 3697
;3697:				if (bs->origin[2] < mins[2]) {
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRLP4 1324+8
INDIRF4
GEF4 $1529
line 3698
;3698:					return;
ADDRGP4 $1514
JUMPV
LABELV $1529
line 3700
;3699:				}
;3700:			}
LABELV $1525
line 3701
;3701:		}
LABELV $1523
line 3702
;3702:		shootbutton = qfalse;
ADDRLP4 1296
CNSTI4 0
ASGNI4
line 3704
;3703:		//if an enemy is below this bounding box then shoot the button
;3704:		for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRGP4 $1535
JUMPV
LABELV $1532
line 3706
;3705:
;3706:			if (i == bs->client) continue;
ADDRLP4 140
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $1536
ADDRGP4 $1533
JUMPV
LABELV $1536
line 3708
;3707:			//
;3708:			BotEntityInfo(i, &entinfo);
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 3710
;3709:			//
;3710:			if (!entinfo.valid) continue;
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1538
ADDRGP4 $1533
JUMPV
LABELV $1538
line 3712
;3711:			//if the enemy isn't dead and the enemy isn't the bot self
;3712:			if (EntityIsDead(&entinfo) || entinfo.number == bs->entitynum) continue;
ADDRLP4 0
ARGP4
ADDRLP4 1368
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 1368
INDIRI4
CNSTI4 0
NEI4 $1543
ADDRLP4 0+20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
NEI4 $1540
LABELV $1543
ADDRGP4 $1533
JUMPV
LABELV $1540
line 3714
;3713:			//
;3714:			if (entinfo.origin[0] > mins[0] && entinfo.origin[0] < maxs[0]) {
ADDRLP4 0+24
INDIRF4
ADDRLP4 1324
INDIRF4
LEF4 $1544
ADDRLP4 0+24
INDIRF4
ADDRLP4 1336
INDIRF4
GEF4 $1544
line 3715
;3715:				if (entinfo.origin[1] > mins[1] && entinfo.origin[1] < maxs[1]) {
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1324+4
INDIRF4
LEF4 $1548
ADDRLP4 0+24+4
INDIRF4
ADDRLP4 1336+4
INDIRF4
GEF4 $1548
line 3716
;3716:					if (entinfo.origin[2] < mins[2]) {
ADDRLP4 0+24+8
INDIRF4
ADDRLP4 1324+8
INDIRF4
GEF4 $1556
line 3718
;3717:						//if there's a team mate below the crusher
;3718:						if (BotSameTeam(bs, i)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
INDIRI4
ARGI4
ADDRLP4 1372
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1372
INDIRI4
CNSTI4 0
EQI4 $1561
line 3719
;3719:							shootbutton = qfalse;
ADDRLP4 1296
CNSTI4 0
ASGNI4
line 3720
;3720:							break;
ADDRGP4 $1534
JUMPV
LABELV $1561
line 3722
;3721:						}
;3722:						else {
line 3723
;3723:							shootbutton = qtrue;
ADDRLP4 1296
CNSTI4 1
ASGNI4
line 3724
;3724:						}
line 3725
;3725:					}
LABELV $1556
line 3726
;3726:				}
LABELV $1548
line 3727
;3727:			}
LABELV $1544
line 3728
;3728:		}
LABELV $1533
line 3704
ADDRLP4 140
ADDRLP4 140
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1535
ADDRLP4 140
INDIRI4
ADDRGP4 maxclients
INDIRI4
GEI4 $1563
ADDRLP4 140
INDIRI4
CNSTI4 64
LTI4 $1532
LABELV $1563
LABELV $1534
line 3729
;3729:		if (shootbutton) {
ADDRLP4 1296
INDIRI4
CNSTI4 0
EQI4 $1518
line 3730
;3730:			bs->flags |= BFL_IDEALVIEWSET;
ADDRLP4 1372
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 1372
INDIRP4
ADDRLP4 1372
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 3731
;3731:			VectorSubtract(buttonorg, bs->eye, dir);
ADDRLP4 1376
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1300
ADDRLP4 1348
INDIRF4
ADDRLP4 1376
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1300+4
ADDRLP4 1348+4
INDIRF4
ADDRLP4 1376
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1300+8
ADDRLP4 1348+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3732
;3732:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 1300
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3733
;3733:			aim_accuracy = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_AIM_ACCURACY, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 7
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 1380
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 1312
ADDRLP4 1380
INDIRF4
ASGNF4
line 3734
;3734:			bs->ideal_viewangles[PITCH] += 8 * crandom() * (1 - aim_accuracy);
ADDRLP4 1384
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1388
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 1388
INDIRP4
ADDRLP4 1388
INDIRP4
INDIRF4
CNSTF4 1090519040
CNSTF4 1073741824
ADDRLP4 1384
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
CNSTF4 1065353216
ADDRLP4 1312
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3735
;3735:			bs->ideal_viewangles[PITCH] = AngleMod(bs->ideal_viewangles[PITCH]);
ADDRLP4 1392
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ASGNP4
ADDRLP4 1392
INDIRP4
INDIRF4
ARGF4
ADDRLP4 1396
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1392
INDIRP4
ADDRLP4 1396
INDIRF4
ASGNF4
line 3736
;3736:			bs->ideal_viewangles[YAW] += 8 * crandom() * (1 - aim_accuracy);
ADDRLP4 1400
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1404
ADDRFP4 0
INDIRP4
CNSTI4 6580
ADDP4
ASGNP4
ADDRLP4 1404
INDIRP4
ADDRLP4 1404
INDIRP4
INDIRF4
CNSTF4 1090519040
CNSTF4 1073741824
ADDRLP4 1400
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
CNSTF4 1065353216
ADDRLP4 1312
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 3737
;3737:			bs->ideal_viewangles[YAW] = AngleMod(bs->ideal_viewangles[YAW]);
ADDRLP4 1408
ADDRFP4 0
INDIRP4
CNSTI4 6580
ADDP4
ASGNP4
ADDRLP4 1408
INDIRP4
INDIRF4
ARGF4
ADDRLP4 1412
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 1408
INDIRP4
ADDRLP4 1412
INDIRF4
ASGNF4
line 3739
;3738:			//
;3739:			if (InFieldOfVision(bs->viewangles, 20, bs->ideal_viewangles)) {
ADDRLP4 1416
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1416
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1101004800
ARGF4
ADDRLP4 1416
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRLP4 1420
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 1420
INDIRI4
CNSTI4 0
EQI4 $1518
line 3740
;3740:				trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 3741
;3741:			}
line 3742
;3742:		}
line 3743
;3743:	}
ADDRGP4 $1518
JUMPV
LABELV $1517
line 3744
;3744:	else if (!Q_stricmp(mapname, "mpq3tourney6")) {
ADDRLP4 144
ARGP4
ADDRGP4 $1574
ARGP4
ADDRLP4 1324
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1324
INDIRI4
CNSTI4 0
NEI4 $1572
line 3746
;3745:		//NOTE: NEVER use the func_bobbing in mpq3tourney6
;3746:		bs->tfl &= ~TFL_FUNCBOB;
ADDRLP4 1328
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 1328
INDIRP4
ADDRLP4 1328
INDIRP4
INDIRI4
CNSTI4 -16777217
BANDI4
ASGNI4
line 3747
;3747:	}
LABELV $1572
LABELV $1518
line 3748
;3748:}
LABELV $1514
endproc BotMapScripts 1424 16
data
align 4
LABELV VEC_UP
byte 4 0
byte 4 3212836864
byte 4 0
align 4
LABELV MOVEDIR_UP
byte 4 0
byte 4 0
byte 4 1065353216
align 4
LABELV VEC_DOWN
byte 4 0
byte 4 3221225472
byte 4 0
align 4
LABELV MOVEDIR_DOWN
byte 4 0
byte 4 0
byte 4 3212836864
export BotSetMovedir
code
proc BotSetMovedir 12 16
line 3761
;3749:
;3750:/*
;3751:==================
;3752:BotSetMovedir
;3753:==================
;3754:*/
;3755:// bk001205 - made these static
;3756:static vec3_t VEC_UP		= {0, -1,  0};
;3757:static vec3_t MOVEDIR_UP	= {0,  0,  1};
;3758:static vec3_t VEC_DOWN		= {0, -2,  0};
;3759:static vec3_t MOVEDIR_DOWN	= {0,  0, -1};
;3760:
;3761:void BotSetMovedir(vec3_t angles, vec3_t movedir) {
line 3762
;3762:	if (VectorCompare(angles, VEC_UP)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 VEC_UP
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $1576
line 3763
;3763:		VectorCopy(MOVEDIR_UP, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 MOVEDIR_UP
INDIRB
ASGNB 12
line 3764
;3764:	}
ADDRGP4 $1577
JUMPV
LABELV $1576
line 3765
;3765:	else if (VectorCompare(angles, VEC_DOWN)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 VEC_DOWN
ARGP4
ADDRLP4 4
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $1578
line 3766
;3766:		VectorCopy(MOVEDIR_DOWN, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 MOVEDIR_DOWN
INDIRB
ASGNB 12
line 3767
;3767:	}
ADDRGP4 $1579
JUMPV
LABELV $1578
line 3768
;3768:	else {
line 3769
;3769:		AngleVectors(angles, movedir, NULL, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
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
ADDRGP4 AngleVectors
CALLV
pop
line 3770
;3770:	}
LABELV $1579
LABELV $1577
line 3771
;3771:}
LABELV $1575
endproc BotSetMovedir 12 16
export BotModelMinsMaxs
proc BotModelMinsMaxs 40 0
line 3780
;3772:
;3773:/*
;3774:==================
;3775:BotModelMinsMaxs
;3776:
;3777:this is ugly
;3778:==================
;3779:*/
;3780:int BotModelMinsMaxs(int modelindex, int eType, int contents, vec3_t mins, vec3_t maxs) {
line 3784
;3781:	gentity_t *ent;
;3782:	int i;
;3783:
;3784:	ent = &g_entities[0];
ADDRLP4 0
ADDRGP4 g_entities
ASGNP4
line 3785
;3785:	for (i = 0; i < level.num_entities; i++, ent++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $1584
JUMPV
LABELV $1581
line 3786
;3786:		if ( !ent->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1586
line 3787
;3787:			continue;
ADDRGP4 $1582
JUMPV
LABELV $1586
line 3789
;3788:		}
;3789:		if ( eType && ent->s.eType != eType) {
ADDRLP4 8
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $1588
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $1588
line 3790
;3790:			continue;
ADDRGP4 $1582
JUMPV
LABELV $1588
line 3792
;3791:		}
;3792:		if ( contents && ent->r.contents != contents) {
ADDRLP4 12
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $1590
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $1590
line 3793
;3793:			continue;
ADDRGP4 $1582
JUMPV
LABELV $1590
line 3795
;3794:		}
;3795:		if (ent->s.modelindex == modelindex) {
ADDRLP4 0
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $1592
line 3796
;3796:			if (mins)
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1594
line 3797
;3797:				VectorAdd(ent->r.currentOrigin, ent->r.mins, mins);
ADDRFP4 12
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 436
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRFP4 12
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 440
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 444
ADDP4
INDIRF4
ADDF4
ASGNF4
LABELV $1594
line 3798
;3798:			if (maxs)
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1596
line 3799
;3799:				VectorAdd(ent->r.currentOrigin, ent->r.maxs, maxs);
ADDRFP4 16
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 448
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 492
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 452
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 456
ADDP4
INDIRF4
ADDF4
ASGNF4
LABELV $1596
line 3800
;3800:			return i;
ADDRLP4 4
INDIRI4
RETI4
ADDRGP4 $1580
JUMPV
LABELV $1592
line 3802
;3801:		}
;3802:	}
LABELV $1582
line 3785
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $1584
ADDRLP4 4
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $1581
line 3803
;3803:	if (mins)
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1598
line 3804
;3804:		VectorClear(mins);
ADDRLP4 8
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
LABELV $1598
line 3805
;3805:	if (maxs)
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1600
line 3806
;3806:		VectorClear(maxs);
ADDRLP4 16
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
ADDRLP4 20
INDIRF4
ASGNF4
LABELV $1600
line 3807
;3807:	return 0;
CNSTI4 0
RETI4
LABELV $1580
endproc BotModelMinsMaxs 40 0
lit
align 4
LABELV $1603
byte 4 1065353216
byte 4 1065353216
byte 4 1065353216
align 4
LABELV $1604
byte 4 3212836864
byte 4 3212836864
byte 4 3212836864
export BotFuncButtonActivateGoal
code
proc BotFuncButtonActivateGoal 648 28
line 3815
;3808:}
;3809:
;3810:/*
;3811:==================
;3812:BotFuncButtonGoal
;3813:==================
;3814:*/
;3815:int BotFuncButtonActivateGoal(bot_state_t *bs, int bspent, bot_activategoal_t *activategoal) {
line 3821
;3816:	int i, areas[10], numareas, modelindex, entitynum;
;3817:	char model[128];
;3818:	float lip, dist, health, angle;
;3819:	vec3_t size, start, end, mins, maxs, angles, points[10];
;3820:	vec3_t movedir, origin, goalorigin, bboxmins, bboxmaxs;
;3821:	vec3_t extramins = {1, 1, 1}, extramaxs = {-1, -1, -1};
ADDRLP4 304
ADDRGP4 $1603
INDIRB
ASGNB 12
ADDRLP4 316
ADDRGP4 $1604
INDIRB
ASGNB 12
line 3824
;3822:	bsp_trace_t bsptrace;
;3823:
;3824:	activategoal->shoot = qfalse;
ADDRFP4 8
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 0
ASGNI4
line 3825
;3825:	VectorClear(activategoal->target);
ADDRLP4 560
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 564
CNSTF4 0
ASGNF4
ADDRLP4 560
INDIRP4
CNSTI4 88
ADDP4
ADDRLP4 564
INDIRF4
ASGNF4
ADDRLP4 560
INDIRP4
CNSTI4 84
ADDP4
ADDRLP4 564
INDIRF4
ASGNF4
ADDRLP4 560
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 564
INDIRF4
ASGNF4
line 3827
;3826:	//create a bot goal towards the button
;3827:	trap_AAS_ValueForBSPEpairKey(bspent, "model", model, sizeof(model));
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 160
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 3828
;3828:	if (!*model)
ADDRLP4 160
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1605
line 3829
;3829:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1605
line 3830
;3830:	modelindex = atoi(model+1);
ADDRLP4 160+1
ARGP4
ADDRLP4 568
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 288
ADDRLP4 568
INDIRI4
ASGNI4
line 3831
;3831:	if (!modelindex)
ADDRLP4 288
INDIRI4
CNSTI4 0
NEI4 $1608
line 3832
;3832:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1608
line 3833
;3833:	VectorClear(angles);
ADDRLP4 572
CNSTF4 0
ASGNF4
ADDRLP4 96+8
ADDRLP4 572
INDIRF4
ASGNF4
ADDRLP4 96+4
ADDRLP4 572
INDIRF4
ASGNF4
ADDRLP4 96
ADDRLP4 572
INDIRF4
ASGNF4
line 3834
;3834:	entitynum = BotModelMinsMaxs(modelindex, ET_MOVER, 0, mins, maxs);
ADDRLP4 288
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 72
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 576
ADDRGP4 BotModelMinsMaxs
CALLI4
ASGNI4
ADDRLP4 344
ADDRLP4 576
INDIRI4
ASGNI4
line 3836
;3835:	//get the lip of the button
;3836:	trap_AAS_FloatForBSPEpairKey(bspent, "lip", &lip);
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $1612
ARGP4
ADDRLP4 328
ARGP4
ADDRGP4 trap_AAS_FloatForBSPEpairKey
CALLI4
pop
line 3837
;3837:	if (!lip) lip = 4;
ADDRLP4 328
INDIRF4
CNSTF4 0
NEF4 $1613
ADDRLP4 328
CNSTF4 1082130432
ASGNF4
LABELV $1613
line 3839
;3838:	//get the move direction from the angle
;3839:	trap_AAS_FloatForBSPEpairKey(bspent, "angle", &angle);
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $1615
ARGP4
ADDRLP4 352
ARGP4
ADDRGP4 trap_AAS_FloatForBSPEpairKey
CALLI4
pop
line 3840
;3840:	VectorSet(angles, 0, angle, 0);
ADDRLP4 96
CNSTF4 0
ASGNF4
ADDRLP4 96+4
ADDRLP4 352
INDIRF4
ASGNF4
ADDRLP4 96+8
CNSTF4 0
ASGNF4
line 3841
;3841:	BotSetMovedir(angles, movedir);
ADDRLP4 96
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 BotSetMovedir
CALLV
pop
line 3843
;3842:	//button size
;3843:	VectorSubtract(maxs, mins, size);
ADDRLP4 112
ADDRLP4 84
INDIRF4
ADDRLP4 72
INDIRF4
SUBF4
ASGNF4
ADDRLP4 112+4
ADDRLP4 84+4
INDIRF4
ADDRLP4 72+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 112+8
ADDRLP4 84+8
INDIRF4
ADDRLP4 72+8
INDIRF4
SUBF4
ASGNF4
line 3845
;3844:	//button origin
;3845:	VectorAdd(mins, maxs, origin);
ADDRLP4 16
ADDRLP4 72
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 72+4
INDIRF4
ADDRLP4 84+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 16+8
ADDRLP4 72+8
INDIRF4
ADDRLP4 84+8
INDIRF4
ADDF4
ASGNF4
line 3846
;3846:	VectorScale(origin, 0.5, origin);
ADDRLP4 580
CNSTF4 1056964608
ASGNF4
ADDRLP4 16
ADDRLP4 580
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 580
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 16+8
CNSTF4 1056964608
ADDRLP4 16+8
INDIRF4
MULF4
ASGNF4
line 3848
;3847:	//touch distance of the button
;3848:	dist = fabs(movedir[0]) * size[0] + fabs(movedir[1]) * size[1] + fabs(movedir[2]) * size[2];
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 584
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4+4
INDIRF4
ARGF4
ADDRLP4 588
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4+8
INDIRF4
ARGF4
ADDRLP4 592
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 584
INDIRF4
ADDRLP4 112
INDIRF4
MULF4
ADDRLP4 588
INDIRF4
ADDRLP4 112+4
INDIRF4
MULF4
ADDF4
ADDRLP4 592
INDIRF4
ADDRLP4 112+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 3849
;3849:	dist *= 0.5;
ADDRLP4 28
CNSTF4 1056964608
ADDRLP4 28
INDIRF4
MULF4
ASGNF4
line 3851
;3850:	//
;3851:	trap_AAS_FloatForBSPEpairKey(bspent, "health", &health);
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $1638
ARGP4
ADDRLP4 348
ARGP4
ADDRGP4 trap_AAS_FloatForBSPEpairKey
CALLI4
pop
line 3853
;3852:	//if the button is shootable
;3853:	if (health) {
ADDRLP4 348
INDIRF4
CNSTF4 0
EQF4 $1639
line 3855
;3854:		//calculate the shoot target
;3855:		VectorMA(origin, -dist, movedir, goalorigin);
ADDRLP4 596
ADDRLP4 28
INDIRF4
NEGF4
ASGNF4
ADDRLP4 124
ADDRLP4 16
INDIRF4
ADDRLP4 4
INDIRF4
ADDRLP4 596
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 596
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 28
INDIRF4
NEGF4
MULF4
ADDF4
ASGNF4
line 3857
;3856:		//
;3857:		VectorCopy(goalorigin, activategoal->target);
ADDRFP4 8
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 124
INDIRB
ASGNB 12
line 3858
;3858:		activategoal->shoot = qtrue;
ADDRFP4 8
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 1
ASGNI4
line 3860
;3859:		//
;3860:		BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, goalorigin, bs->entitynum, MASK_SHOT);
ADDRLP4 356
ARGP4
ADDRLP4 600
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 600
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 604
CNSTP4 0
ASGNP4
ADDRLP4 604
INDIRP4
ARGP4
ADDRLP4 604
INDIRP4
ARGP4
ADDRLP4 124
ARGP4
ADDRLP4 600
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 100663297
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 3862
;3861:		// if the button is visible from the current position
;3862:		if (bsptrace.fraction >= 1.0 || bsptrace.ent == entitynum) {
ADDRLP4 356+8
INDIRF4
CNSTF4 1065353216
GEF4 $1651
ADDRLP4 356+80
INDIRI4
ADDRLP4 344
INDIRI4
NEI4 $1647
LABELV $1651
line 3864
;3863:			//
;3864:			activategoal->goal.entitynum = entitynum; //NOTE: this is the entity number of the shootable button
ADDRFP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 344
INDIRI4
ASGNI4
line 3865
;3865:			activategoal->goal.number = 0;
ADDRFP4 8
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 3866
;3866:			activategoal->goal.flags = 0;
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 3867
;3867:			VectorCopy(bs->origin, activategoal->goal.origin);
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 3868
;3868:			activategoal->goal.areanum = bs->areanum;
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ASGNI4
line 3869
;3869:			VectorSet(activategoal->goal.mins, -8, -8, -8);
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 3238002688
ASGNF4
line 3870
;3870:			VectorSet(activategoal->goal.maxs, 8, 8, 8);
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1090519040
ASGNF4
line 3872
;3871:			//
;3872:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1647
line 3874
;3873:		}
;3874:		else {
line 3877
;3875:			//create a goal from where the button is visible and shoot at the button from there
;3876:			//add bounding box size to the dist
;3877:			trap_AAS_PresenceTypeBoundingBox(PRESENCE_CROUCH, bboxmins, bboxmaxs);
CNSTI4 4
ARGI4
ADDRLP4 136
ARGP4
ADDRLP4 148
ARGP4
ADDRGP4 trap_AAS_PresenceTypeBoundingBox
CALLV
pop
line 3878
;3878:			for (i = 0; i < 3; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1652
line 3879
;3879:				if (movedir[i] < 0) dist += fabs(movedir[i]) * fabs(bboxmaxs[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $1656
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 608
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 148
ADDP4
INDIRF4
ARGF4
ADDRLP4 612
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 28
INDIRF4
ADDRLP4 608
INDIRF4
ADDRLP4 612
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 $1657
JUMPV
LABELV $1656
line 3880
;3880:				else dist += fabs(movedir[i]) * fabs(bboxmins[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 616
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 136
ADDP4
INDIRF4
ARGF4
ADDRLP4 620
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 28
INDIRF4
ADDRLP4 616
INDIRF4
ADDRLP4 620
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $1657
line 3881
;3881:			}
LABELV $1653
line 3878
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $1652
line 3883
;3882:			//calculate the goal origin
;3883:			VectorMA(origin, -dist, movedir, goalorigin);
ADDRLP4 608
ADDRLP4 28
INDIRF4
NEGF4
ASGNF4
ADDRLP4 124
ADDRLP4 16
INDIRF4
ADDRLP4 4
INDIRF4
ADDRLP4 608
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 608
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 28
INDIRF4
NEGF4
MULF4
ADDF4
ASGNF4
line 3885
;3884:			//
;3885:			VectorCopy(goalorigin, start);
ADDRLP4 292
ADDRLP4 124
INDIRB
ASGNB 12
line 3886
;3886:			start[2] += 24;
ADDRLP4 292+8
ADDRLP4 292+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 3887
;3887:			VectorCopy(start, end);
ADDRLP4 332
ADDRLP4 292
INDIRB
ASGNB 12
line 3888
;3888:			end[2] -= 512;
ADDRLP4 332+8
ADDRLP4 332+8
INDIRF4
CNSTF4 1140850688
SUBF4
ASGNF4
line 3889
;3889:			numareas = trap_AAS_TraceAreas(start, end, areas, points, 10);
ADDRLP4 292
ARGP4
ADDRLP4 332
ARGP4
ADDRLP4 32
ARGP4
ADDRLP4 440
ARGP4
CNSTI4 10
ARGI4
ADDRLP4 612
ADDRGP4 trap_AAS_TraceAreas
CALLI4
ASGNI4
ADDRLP4 108
ADDRLP4 612
INDIRI4
ASGNI4
line 3891
;3890:			//
;3891:			for (i = numareas-1; i >= 0; i--) {
ADDRLP4 0
ADDRLP4 108
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRGP4 $1669
JUMPV
LABELV $1666
line 3892
;3892:				if (trap_AAS_AreaReachability(areas[i])) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 616
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 616
INDIRI4
CNSTI4 0
EQI4 $1670
line 3893
;3893:					break;
ADDRGP4 $1668
JUMPV
LABELV $1670
line 3895
;3894:				}
;3895:			}
LABELV $1667
line 3891
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $1669
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $1666
LABELV $1668
line 3896
;3896:			if (i < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $1672
line 3898
;3897:				// FIXME: trace forward and maybe in other directions to find a valid area
;3898:			}
LABELV $1672
line 3899
;3899:			if (i >= 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1674
line 3901
;3900:				//
;3901:				VectorCopy(points[i], activategoal->goal.origin);
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 440
ADDP4
INDIRB
ASGNB 12
line 3902
;3902:				activategoal->goal.areanum = areas[i];
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
ADDP4
INDIRI4
ASGNI4
line 3903
;3903:				VectorSet(activategoal->goal.mins, 8, 8, 8);
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1090519040
ASGNF4
line 3904
;3904:				VectorSet(activategoal->goal.maxs, -8, -8, -8);
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 3238002688
ASGNF4
line 3906
;3905:				//
;3906:				for (i = 0; i < 3; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1676
line 3907
;3907:				{
line 3908
;3908:					if (movedir[i] < 0) activategoal->goal.maxs[i] += fabs(movedir[i]) * fabs(extramaxs[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $1680
ADDRLP4 616
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 616
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 620
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 316
ADDP4
INDIRF4
ARGF4
ADDRLP4 624
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 628
ADDRLP4 616
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ASGNP4
ADDRLP4 628
INDIRP4
ADDRLP4 628
INDIRP4
INDIRF4
ADDRLP4 620
INDIRF4
ADDRLP4 624
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 $1681
JUMPV
LABELV $1680
line 3909
;3909:					else activategoal->goal.mins[i] += fabs(movedir[i]) * fabs(extramins[i]);
ADDRLP4 632
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 632
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 636
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 304
ADDP4
INDIRF4
ARGF4
ADDRLP4 640
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 644
ADDRLP4 632
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
ADDP4
ASGNP4
ADDRLP4 644
INDIRP4
ADDRLP4 644
INDIRP4
INDIRF4
ADDRLP4 636
INDIRF4
ADDRLP4 640
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $1681
line 3910
;3910:				} //end for
LABELV $1677
line 3906
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $1676
line 3912
;3911:				//
;3912:				activategoal->goal.entitynum = entitynum;
ADDRFP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 344
INDIRI4
ASGNI4
line 3913
;3913:				activategoal->goal.number = 0;
ADDRFP4 8
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 3914
;3914:				activategoal->goal.flags = 0;
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 3915
;3915:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1674
line 3917
;3916:			}
;3917:		}
line 3918
;3918:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1639
line 3920
;3919:	}
;3920:	else {
line 3922
;3921:		//add bounding box size to the dist
;3922:		trap_AAS_PresenceTypeBoundingBox(PRESENCE_CROUCH, bboxmins, bboxmaxs);
CNSTI4 4
ARGI4
ADDRLP4 136
ARGP4
ADDRLP4 148
ARGP4
ADDRGP4 trap_AAS_PresenceTypeBoundingBox
CALLV
pop
line 3923
;3923:		for (i = 0; i < 3; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1682
line 3924
;3924:			if (movedir[i] < 0) dist += fabs(movedir[i]) * fabs(bboxmaxs[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $1686
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 596
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 148
ADDP4
INDIRF4
ARGF4
ADDRLP4 600
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 28
INDIRF4
ADDRLP4 596
INDIRF4
ADDRLP4 600
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 $1687
JUMPV
LABELV $1686
line 3925
;3925:			else dist += fabs(movedir[i]) * fabs(bboxmins[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 604
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 136
ADDP4
INDIRF4
ARGF4
ADDRLP4 608
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 28
INDIRF4
ADDRLP4 604
INDIRF4
ADDRLP4 608
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $1687
line 3926
;3926:		}
LABELV $1683
line 3923
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $1682
line 3928
;3927:		//calculate the goal origin
;3928:		VectorMA(origin, -dist, movedir, goalorigin);
ADDRLP4 596
ADDRLP4 28
INDIRF4
NEGF4
ASGNF4
ADDRLP4 124
ADDRLP4 16
INDIRF4
ADDRLP4 4
INDIRF4
ADDRLP4 596
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 596
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 28
INDIRF4
NEGF4
MULF4
ADDF4
ASGNF4
line 3930
;3929:		//
;3930:		VectorCopy(goalorigin, start);
ADDRLP4 292
ADDRLP4 124
INDIRB
ASGNB 12
line 3931
;3931:		start[2] += 24;
ADDRLP4 292+8
ADDRLP4 292+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 3932
;3932:		VectorCopy(start, end);
ADDRLP4 332
ADDRLP4 292
INDIRB
ASGNB 12
line 3933
;3933:		end[2] -= 100;
ADDRLP4 332+8
ADDRLP4 332+8
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 3934
;3934:		numareas = trap_AAS_TraceAreas(start, end, areas, NULL, 10);
ADDRLP4 292
ARGP4
ADDRLP4 332
ARGP4
ADDRLP4 32
ARGP4
CNSTP4 0
ARGP4
CNSTI4 10
ARGI4
ADDRLP4 600
ADDRGP4 trap_AAS_TraceAreas
CALLI4
ASGNI4
ADDRLP4 108
ADDRLP4 600
INDIRI4
ASGNI4
line 3936
;3935:		//
;3936:		for (i = 0; i < numareas; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1699
JUMPV
LABELV $1696
line 3937
;3937:			if (trap_AAS_AreaReachability(areas[i])) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 604
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 604
INDIRI4
CNSTI4 0
EQI4 $1700
line 3938
;3938:				break;
ADDRGP4 $1698
JUMPV
LABELV $1700
line 3940
;3939:			}
;3940:		}
LABELV $1697
line 3936
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1699
ADDRLP4 0
INDIRI4
ADDRLP4 108
INDIRI4
LTI4 $1696
LABELV $1698
line 3941
;3941:		if (i < numareas) {
ADDRLP4 0
INDIRI4
ADDRLP4 108
INDIRI4
GEI4 $1702
line 3943
;3942:			//
;3943:			VectorCopy(origin, activategoal->goal.origin);
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 16
INDIRB
ASGNB 12
line 3944
;3944:			activategoal->goal.areanum = areas[i];
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
ADDP4
INDIRI4
ASGNI4
line 3945
;3945:			VectorSubtract(mins, origin, activategoal->goal.mins);
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 72
INDIRF4
ADDRLP4 16
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 72+4
INDIRF4
ADDRLP4 16+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 72+8
INDIRF4
ADDRLP4 16+8
INDIRF4
SUBF4
ASGNF4
line 3946
;3946:			VectorSubtract(maxs, origin, activategoal->goal.maxs);
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 84
INDIRF4
ADDRLP4 16
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 84+4
INDIRF4
ADDRLP4 16+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 84+8
INDIRF4
ADDRLP4 16+8
INDIRF4
SUBF4
ASGNF4
line 3948
;3947:			//
;3948:			for (i = 0; i < 3; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1712
line 3949
;3949:			{
line 3950
;3950:				if (movedir[i] < 0) activategoal->goal.maxs[i] += fabs(movedir[i]) * fabs(extramaxs[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $1716
ADDRLP4 604
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 604
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 608
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 316
ADDP4
INDIRF4
ARGF4
ADDRLP4 612
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 616
ADDRLP4 604
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ASGNP4
ADDRLP4 616
INDIRP4
ADDRLP4 616
INDIRP4
INDIRF4
ADDRLP4 608
INDIRF4
ADDRLP4 612
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 $1717
JUMPV
LABELV $1716
line 3951
;3951:				else activategoal->goal.mins[i] += fabs(movedir[i]) * fabs(extramins[i]);
ADDRLP4 620
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 620
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 624
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 304
ADDP4
INDIRF4
ARGF4
ADDRLP4 628
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 632
ADDRLP4 620
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
ADDP4
ASGNP4
ADDRLP4 632
INDIRP4
ADDRLP4 632
INDIRP4
INDIRF4
ADDRLP4 624
INDIRF4
ADDRLP4 628
INDIRF4
MULF4
ADDF4
ASGNF4
LABELV $1717
line 3952
;3952:			} //end for
LABELV $1713
line 3948
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $1712
line 3954
;3953:			//
;3954:			activategoal->goal.entitynum = entitynum;
ADDRFP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 344
INDIRI4
ASGNI4
line 3955
;3955:			activategoal->goal.number = 0;
ADDRFP4 8
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 3956
;3956:			activategoal->goal.flags = 0;
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 3957
;3957:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1602
JUMPV
LABELV $1702
line 3959
;3958:		}
;3959:	}
line 3960
;3960:	return qfalse;
CNSTI4 0
RETI4
LABELV $1602
endproc BotFuncButtonActivateGoal 648 28
export BotFuncDoorActivateGoal
proc BotFuncDoorActivateGoal 1096 20
line 3968
;3961:}
;3962:
;3963:/*
;3964:==================
;3965:BotFuncDoorGoal
;3966:==================
;3967:*/
;3968:int BotFuncDoorActivateGoal(bot_state_t *bs, int bspent, bot_activategoal_t *activategoal) {
line 3974
;3969:	int modelindex, entitynum;
;3970:	char model[MAX_INFO_STRING];
;3971:	vec3_t mins, maxs, origin, angles;
;3972:
;3973:	//shoot at the shootable door
;3974:	trap_AAS_ValueForBSPEpairKey(bspent, "model", model, sizeof(model));
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 3975
;3975:	if (!*model)
ADDRLP4 12
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1719
line 3976
;3976:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1718
JUMPV
LABELV $1719
line 3977
;3977:	modelindex = atoi(model+1);
ADDRLP4 12+1
ARGP4
ADDRLP4 1080
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1060
ADDRLP4 1080
INDIRI4
ASGNI4
line 3978
;3978:	if (!modelindex)
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $1722
line 3979
;3979:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1718
JUMPV
LABELV $1722
line 3980
;3980:	VectorClear(angles);
ADDRLP4 1084
CNSTF4 0
ASGNF4
ADDRLP4 1064+8
ADDRLP4 1084
INDIRF4
ASGNF4
ADDRLP4 1064+4
ADDRLP4 1084
INDIRF4
ASGNF4
ADDRLP4 1064
ADDRLP4 1084
INDIRF4
ASGNF4
line 3981
;3981:	entitynum = BotModelMinsMaxs(modelindex, ET_MOVER, 0, mins, maxs);
ADDRLP4 1060
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 1048
ARGP4
ADDRLP4 1088
ADDRGP4 BotModelMinsMaxs
CALLI4
ASGNI4
ADDRLP4 1076
ADDRLP4 1088
INDIRI4
ASGNI4
line 3983
;3982:	//door origin
;3983:	VectorAdd(mins, maxs, origin);
ADDRLP4 0
ADDRLP4 1036
INDIRF4
ADDRLP4 1048
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1036+4
INDIRF4
ADDRLP4 1048+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 1036+8
INDIRF4
ADDRLP4 1048+8
INDIRF4
ADDF4
ASGNF4
line 3984
;3984:	VectorScale(origin, 0.5, origin);
ADDRLP4 1092
CNSTF4 1056964608
ASGNF4
ADDRLP4 0
ADDRLP4 1092
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1092
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
line 3985
;3985:	VectorCopy(origin, activategoal->target);
ADDRFP4 8
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 0
INDIRB
ASGNB 12
line 3986
;3986:	activategoal->shoot = qtrue;
ADDRFP4 8
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 1
ASGNI4
line 3988
;3987:	//
;3988:	activategoal->goal.entitynum = entitynum; //NOTE: this is the entity number of the shootable door
ADDRFP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 1076
INDIRI4
ASGNI4
line 3989
;3989:	activategoal->goal.number = 0;
ADDRFP4 8
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 3990
;3990:	activategoal->goal.flags = 0;
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 3991
;3991:	VectorCopy(bs->origin, activategoal->goal.origin);
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 3992
;3992:	activategoal->goal.areanum = bs->areanum;
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ASGNI4
line 3993
;3993:	VectorSet(activategoal->goal.mins, -8, -8, -8);
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 3238002688
ASGNF4
line 3994
;3994:	VectorSet(activategoal->goal.maxs, 8, 8, 8);
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1090519040
ASGNF4
line 3995
;3995:	return qtrue;
CNSTI4 1
RETI4
LABELV $1718
endproc BotFuncDoorActivateGoal 1096 20
export BotTriggerMultipleActivateGoal
proc BotTriggerMultipleActivateGoal 300 20
line 4003
;3996:}
;3997:
;3998:/*
;3999:==================
;4000:BotTriggerMultipleGoal
;4001:==================
;4002:*/
;4003:int BotTriggerMultipleActivateGoal(bot_state_t *bs, int bspent, bot_activategoal_t *activategoal) {
line 4009
;4004:	int i, areas[10], numareas, modelindex, entitynum;
;4005:	char model[128];
;4006:	vec3_t start, end, mins, maxs, angles;
;4007:	vec3_t origin, goalorigin;
;4008:
;4009:	activategoal->shoot = qfalse;
ADDRFP4 8
INDIRP4
CNSTI4 72
ADDP4
CNSTI4 0
ASGNI4
line 4010
;4010:	VectorClear(activategoal->target);
ADDRLP4 268
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 272
CNSTF4 0
ASGNF4
ADDRLP4 268
INDIRP4
CNSTI4 88
ADDP4
ADDRLP4 272
INDIRF4
ASGNF4
ADDRLP4 268
INDIRP4
CNSTI4 84
ADDP4
ADDRLP4 272
INDIRF4
ASGNF4
ADDRLP4 268
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 272
INDIRF4
ASGNF4
line 4012
;4011:	//create a bot goal towards the trigger
;4012:	trap_AAS_ValueForBSPEpairKey(bspent, "model", model, sizeof(model));
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 84
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 4013
;4013:	if (!*model)
ADDRLP4 84
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1737
line 4014
;4014:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1736
JUMPV
LABELV $1737
line 4015
;4015:	modelindex = atoi(model+1);
ADDRLP4 84+1
ARGP4
ADDRLP4 276
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 224
ADDRLP4 276
INDIRI4
ASGNI4
line 4016
;4016:	if (!modelindex)
ADDRLP4 224
INDIRI4
CNSTI4 0
NEI4 $1740
line 4017
;4017:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1736
JUMPV
LABELV $1740
line 4018
;4018:	VectorClear(angles);
ADDRLP4 280
CNSTF4 0
ASGNF4
ADDRLP4 240+8
ADDRLP4 280
INDIRF4
ASGNF4
ADDRLP4 240+4
ADDRLP4 280
INDIRF4
ASGNF4
ADDRLP4 240
ADDRLP4 280
INDIRF4
ASGNF4
line 4019
;4019:	entitynum = BotModelMinsMaxs(modelindex, 0, CONTENTS_TRIGGER, mins, maxs);
ADDRLP4 224
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 1073741824
ARGI4
ADDRLP4 60
ARGP4
ADDRLP4 72
ARGP4
ADDRLP4 284
ADDRGP4 BotModelMinsMaxs
CALLI4
ASGNI4
ADDRLP4 264
ADDRLP4 284
INDIRI4
ASGNI4
line 4021
;4020:	//trigger origin
;4021:	VectorAdd(mins, maxs, origin);
ADDRLP4 4
ADDRLP4 60
INDIRF4
ADDRLP4 72
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 60+4
INDIRF4
ADDRLP4 72+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 60+8
INDIRF4
ADDRLP4 72+8
INDIRF4
ADDF4
ASGNF4
line 4022
;4022:	VectorScale(origin, 0.5, origin);
ADDRLP4 288
CNSTF4 1056964608
ASGNF4
ADDRLP4 4
ADDRLP4 288
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 288
INDIRF4
ADDRLP4 4+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 4+8
CNSTF4 1056964608
ADDRLP4 4+8
INDIRF4
MULF4
ASGNF4
line 4023
;4023:	VectorCopy(origin, goalorigin);
ADDRLP4 252
ADDRLP4 4
INDIRB
ASGNB 12
line 4025
;4024:	//
;4025:	VectorCopy(goalorigin, start);
ADDRLP4 212
ADDRLP4 252
INDIRB
ASGNB 12
line 4026
;4026:	start[2] += 24;
ADDRLP4 212+8
ADDRLP4 212+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 4027
;4027:	VectorCopy(start, end);
ADDRLP4 228
ADDRLP4 212
INDIRB
ASGNB 12
line 4028
;4028:	end[2] -= 100;
ADDRLP4 228+8
ADDRLP4 228+8
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 4029
;4029:	numareas = trap_AAS_TraceAreas(start, end, areas, NULL, 10);
ADDRLP4 212
ARGP4
ADDRLP4 228
ARGP4
ADDRLP4 20
ARGP4
CNSTP4 0
ARGP4
CNSTI4 10
ARGI4
ADDRLP4 292
ADDRGP4 trap_AAS_TraceAreas
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 292
INDIRI4
ASGNI4
line 4031
;4030:	//
;4031:	for (i = 0; i < numareas; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1759
JUMPV
LABELV $1756
line 4032
;4032:		if (trap_AAS_AreaReachability(areas[i])) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 296
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 296
INDIRI4
CNSTI4 0
EQI4 $1760
line 4033
;4033:			break;
ADDRGP4 $1758
JUMPV
LABELV $1760
line 4035
;4034:		}
;4035:	}
LABELV $1757
line 4031
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1759
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $1756
LABELV $1758
line 4036
;4036:	if (i < numareas) {
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
GEI4 $1762
line 4037
;4037:		VectorCopy(origin, activategoal->goal.origin);
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 4
INDIRB
ASGNB 12
line 4038
;4038:		activategoal->goal.areanum = areas[i];
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 20
ADDP4
INDIRI4
ASGNI4
line 4039
;4039:		VectorSubtract(mins, origin, activategoal->goal.mins);
ADDRFP4 8
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 60
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 60+4
INDIRF4
ADDRLP4 4+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 60+8
INDIRF4
ADDRLP4 4+8
INDIRF4
SUBF4
ASGNF4
line 4040
;4040:		VectorSubtract(maxs, origin, activategoal->goal.maxs);
ADDRFP4 8
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 72
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 72+4
INDIRF4
ADDRLP4 4+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 72+8
INDIRF4
ADDRLP4 4+8
INDIRF4
SUBF4
ASGNF4
line 4042
;4041:		//
;4042:		activategoal->goal.entitynum = entitynum;
ADDRFP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 264
INDIRI4
ASGNI4
line 4043
;4043:		activategoal->goal.number = 0;
ADDRFP4 8
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 4044
;4044:		activategoal->goal.flags = 0;
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 4045
;4045:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1736
JUMPV
LABELV $1762
line 4047
;4046:	}
;4047:	return qfalse;
CNSTI4 0
RETI4
LABELV $1736
endproc BotTriggerMultipleActivateGoal 300 20
export BotPopFromActivateGoalStack
proc BotPopFromActivateGoalStack 4 8
line 4055
;4048:}
;4049:
;4050:/*
;4051:==================
;4052:BotPopFromActivateGoalStack
;4053:==================
;4054:*/
;4055:int BotPopFromActivateGoalStack(bot_state_t *bs) {
line 4056
;4056:	if (!bs->activatestack)
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1773
line 4057
;4057:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1772
JUMPV
LABELV $1773
line 4058
;4058:	BotEnableActivateGoalAreas(bs->activatestack, qtrue);
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4059
;4059:	bs->activatestack->inuse = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 0
ASGNI4
line 4060
;4060:	bs->activatestack->justused_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 68
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4061
;4061:	bs->activatestack = bs->activatestack->next;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
ASGNP4
line 4062
;4062:	return qtrue;
CNSTI4 1
RETI4
LABELV $1772
endproc BotPopFromActivateGoalStack 4 8
export BotPushOntoActivateGoalStack
proc BotPushOntoActivateGoalStack 24 12
line 4070
;4063:}
;4064:
;4065:/*
;4066:==================
;4067:BotPushOntoActivateGoalStack
;4068:==================
;4069:*/
;4070:int BotPushOntoActivateGoalStack(bot_state_t *bs, bot_activategoal_t *activategoal) {
line 4074
;4071:	int i, best;
;4072:	float besttime;
;4073:
;4074:	best = -1;
ADDRLP4 8
CNSTI4 -1
ASGNI4
line 4075
;4075:	besttime = FloatTime() + 9999;
ADDRLP4 4
ADDRGP4 floattime
INDIRF4
CNSTF4 1176255488
ADDF4
ASGNF4
line 4077
;4076:	//
;4077:	for (i = 0; i < MAX_ACTIVATESTACK; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1776
line 4078
;4078:		if (!bs->activategoalheap[i].inuse) {
CNSTI4 244
ADDRLP4 0
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1780
line 4079
;4079:			if (bs->activategoalheap[i].justused_time < besttime) {
CNSTI4 244
ADDRLP4 0
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 68
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
GEF4 $1782
line 4080
;4080:				besttime = bs->activategoalheap[i].justused_time;
ADDRLP4 4
CNSTI4 244
ADDRLP4 0
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 68
ADDP4
INDIRF4
ASGNF4
line 4081
;4081:				best = i;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 4082
;4082:			}
LABELV $1782
line 4083
;4083:		}
LABELV $1780
line 4084
;4084:	}
LABELV $1777
line 4077
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 8
LTI4 $1776
line 4085
;4085:	if (best != -1) {
ADDRLP4 8
INDIRI4
CNSTI4 -1
EQI4 $1784
line 4086
;4086:		memcpy(&bs->activategoalheap[best], activategoal, sizeof(bot_activategoal_t));
ADDRLP4 12
CNSTI4 244
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 8
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 4087
;4087:		bs->activategoalheap[best].inuse = qtrue;
CNSTI4 244
ADDRLP4 8
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 1
ASGNI4
line 4088
;4088:		bs->activategoalheap[best].next = bs->activatestack;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
CNSTI4 244
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 16
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 240
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
line 4089
;4089:		bs->activatestack = &bs->activategoalheap[best];
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 7116
ADDP4
CNSTI4 244
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 20
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
ASGNP4
line 4090
;4090:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1775
JUMPV
LABELV $1784
line 4092
;4091:	}
;4092:	return qfalse;
CNSTI4 0
RETI4
LABELV $1775
endproc BotPushOntoActivateGoalStack 24 12
export BotClearActivateGoalStack
proc BotClearActivateGoalStack 0 4
line 4100
;4093:}
;4094:
;4095:/*
;4096:==================
;4097:BotClearActivateGoalStack
;4098:==================
;4099:*/
;4100:void BotClearActivateGoalStack(bot_state_t *bs) {
ADDRGP4 $1788
JUMPV
LABELV $1787
line 4102
;4101:	while(bs->activatestack)
;4102:		BotPopFromActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotPopFromActivateGoalStack
CALLI4
pop
LABELV $1788
line 4101
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1787
line 4103
;4103:}
LABELV $1786
endproc BotClearActivateGoalStack 0 4
export BotEnableActivateGoalAreas
proc BotEnableActivateGoalAreas 12 8
line 4110
;4104:
;4105:/*
;4106:==================
;4107:BotEnableActivateGoalAreas
;4108:==================
;4109:*/
;4110:void BotEnableActivateGoalAreas(bot_activategoal_t *activategoal, int enable) {
line 4113
;4111:	int i;
;4112:
;4113:	if (activategoal->areasdisabled == !enable)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $1794
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $1795
JUMPV
LABELV $1794
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $1795
ADDRFP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $1791
line 4114
;4114:		return;
ADDRGP4 $1790
JUMPV
LABELV $1791
line 4115
;4115:	for (i = 0; i < activategoal->numareas; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1799
JUMPV
LABELV $1796
line 4116
;4116:		trap_AAS_EnableRoutingArea( activategoal->areas[i], enable );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 trap_AAS_EnableRoutingArea
CALLI4
pop
LABELV $1797
line 4115
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1799
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
LTI4 $1796
line 4117
;4117:	activategoal->areasdisabled = !enable;
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $1801
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRGP4 $1802
JUMPV
LABELV $1801
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $1802
ADDRFP4 0
INDIRP4
CNSTI4 236
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 4118
;4118:}
LABELV $1790
endproc BotEnableActivateGoalAreas 12 8
export BotIsGoingToActivateEntity
proc BotIsGoingToActivateEntity 8 0
line 4125
;4119:
;4120:/*
;4121:==================
;4122:BotIsGoingToActivateEntity
;4123:==================
;4124:*/
;4125:int BotIsGoingToActivateEntity(bot_state_t *bs, int entitynum) {
line 4129
;4126:	bot_activategoal_t *a;
;4127:	int i;
;4128:
;4129:	for (a = bs->activatestack; a; a = a->next) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $1807
JUMPV
LABELV $1804
line 4130
;4130:		if (a->time < FloatTime())
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1808
line 4131
;4131:			continue;
ADDRGP4 $1805
JUMPV
LABELV $1808
line 4132
;4132:		if (a->goal.entitynum == entitynum)
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $1810
line 4133
;4133:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1803
JUMPV
LABELV $1810
line 4134
;4134:	}
LABELV $1805
line 4129
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
ASGNP4
LABELV $1807
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1804
line 4135
;4135:	for (i = 0; i < MAX_ACTIVATESTACK; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $1812
line 4136
;4136:		if (bs->activategoalheap[i].inuse)
CNSTI4 244
ADDRLP4 4
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $1816
line 4137
;4137:			continue;
ADDRGP4 $1813
JUMPV
LABELV $1816
line 4139
;4138:		//
;4139:		if (bs->activategoalheap[i].goal.entitynum == entitynum) {
CNSTI4 244
ADDRLP4 4
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 44
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $1818
line 4141
;4140:			// if the bot went for this goal less than 2 seconds ago
;4141:			if (bs->activategoalheap[i].justused_time > FloatTime() - 2)
CNSTI4 244
ADDRLP4 4
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 7120
ADDP4
ADDP4
CNSTI4 68
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
LEF4 $1820
line 4142
;4142:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1803
JUMPV
LABELV $1820
line 4143
;4143:		}
LABELV $1818
line 4144
;4144:	}
LABELV $1813
line 4135
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 8
LTI4 $1812
line 4145
;4145:	return qfalse;
CNSTI4 0
RETI4
LABELV $1803
endproc BotIsGoingToActivateEntity 8 0
export BotGetActivateGoal
proc BotGetActivateGoal 3320 20
line 4158
;4146:}
;4147:
;4148:/*
;4149:==================
;4150:BotGetActivateGoal
;4151:
;4152:  returns the number of the bsp entity to activate
;4153:  goal->entitynum will be set to the game entity to activate
;4154:==================
;4155:*/
;4156://#define OBSTACLEDEBUG
;4157:
;4158:int BotGetActivateGoal(bot_state_t *bs, int entitynum, bot_activategoal_t *activategoal) {
line 4168
;4159:	int i, ent, cur_entities[10], spawnflags, modelindex, areas[MAX_ACTIVATEAREAS*2], numareas, t;
;4160:	char model[MAX_INFO_STRING], tmpmodel[128];
;4161:	char target[128], classname[128];
;4162:	float health;
;4163:	char targetname[10][128];
;4164:	aas_entityinfo_t entinfo;
;4165:	aas_areainfo_t areainfo;
;4166:	vec3_t origin, angles, absmins, absmaxs;
;4167:
;4168:	memset(activategoal, 0, sizeof(bot_activategoal_t));
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 244
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4169
;4169:	BotEntityInfo(entitynum, &entinfo);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 3052
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 4170
;4170:	Com_sprintf(model, sizeof( model ), "*%d", entinfo.modelindex);
ADDRLP4 1712
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $1823
ARGP4
ADDRLP4 3052+104
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4171
;4171:	for (ent = trap_AAS_NextBSPEntity(0); ent; ent = trap_AAS_NextBSPEntity(ent)) {
CNSTI4 0
ARGI4
ADDRLP4 3252
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 3252
INDIRI4
ASGNI4
ADDRGP4 $1828
JUMPV
LABELV $1825
line 4172
;4172:		if (!trap_AAS_ValueForBSPEpairKey(ent, "model", tmpmodel, sizeof(tmpmodel))) continue;
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 1584
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 3256
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3256
INDIRI4
CNSTI4 0
NEI4 $1829
ADDRGP4 $1826
JUMPV
LABELV $1829
line 4173
;4173:		if (!strcmp(model, tmpmodel)) break;
ADDRLP4 1712
ARGP4
ADDRLP4 1584
ARGP4
ADDRLP4 3260
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3260
INDIRI4
CNSTI4 0
NEI4 $1831
ADDRGP4 $1827
JUMPV
LABELV $1831
line 4174
;4174:	}
LABELV $1826
line 4171
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 3256
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 3256
INDIRI4
ASGNI4
LABELV $1828
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1825
LABELV $1827
line 4175
;4175:	if (!ent) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1833
line 4176
;4176:		BotAI_Print(PRT_ERROR, "BotGetActivateGoal: no entity found with model %s\n", model);
CNSTI4 3
ARGI4
ADDRGP4 $1835
ARGP4
ADDRLP4 1712
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4177
;4177:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1833
line 4179
;4178:	}
;4179:	trap_AAS_ValueForBSPEpairKey(ent, "classname", classname, sizeof(classname));
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1836
ARGP4
ADDRLP4 1456
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 4180
;4180:	if (!classname) {
ADDRLP4 1456
CVPU4 4
CNSTU4 0
NEU4 $1837
line 4181
;4181:		BotAI_Print(PRT_ERROR, "BotGetActivateGoal: entity with model %s has no classname\n", model);
CNSTI4 3
ARGI4
ADDRGP4 $1839
ARGP4
ADDRLP4 1712
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4182
;4182:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1837
line 4185
;4183:	}
;4184:	//if it is a door
;4185:	if (!strcmp(classname, "func_door")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1842
ARGP4
ADDRLP4 3260
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3260
INDIRI4
CNSTI4 0
NEI4 $1840
line 4186
;4186:		if (trap_AAS_FloatForBSPEpairKey(ent, "health", &health)) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1638
ARGP4
ADDRLP4 3208
ARGP4
ADDRLP4 3264
ADDRGP4 trap_AAS_FloatForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3264
INDIRI4
CNSTI4 0
EQI4 $1843
line 4188
;4187:			//if the door has health then the door must be shot to open
;4188:			if (health) {
ADDRLP4 3208
INDIRF4
CNSTF4 0
EQF4 $1845
line 4189
;4189:				BotFuncDoorActivateGoal(bs, ent, activategoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 BotFuncDoorActivateGoal
CALLI4
pop
line 4190
;4190:				return ent;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1845
line 4192
;4191:			}
;4192:		}
LABELV $1843
line 4194
;4193:		//
;4194:		trap_AAS_IntForBSPEpairKey(ent, "spawnflags", &spawnflags);
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1847
ARGP4
ADDRLP4 3204
ARGP4
ADDRGP4 trap_AAS_IntForBSPEpairKey
CALLI4
pop
line 4196
;4195:		// if the door starts open then just wait for the door to return
;4196:		if ( spawnflags & 1 )
ADDRLP4 3204
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $1848
line 4197
;4197:			return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1848
line 4199
;4198:		//get the door origin
;4199:		if (!trap_AAS_VectorForBSPEpairKey(ent, "origin", origin)) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1852
ARGP4
ADDRLP4 3192
ARGP4
ADDRLP4 3268
ADDRGP4 trap_AAS_VectorForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3268
INDIRI4
CNSTI4 0
NEI4 $1850
line 4200
;4200:			VectorClear(origin);
ADDRLP4 3272
CNSTF4 0
ASGNF4
ADDRLP4 3192+8
ADDRLP4 3272
INDIRF4
ASGNF4
ADDRLP4 3192+4
ADDRLP4 3272
INDIRF4
ASGNF4
ADDRLP4 3192
ADDRLP4 3272
INDIRF4
ASGNF4
line 4201
;4201:		}
LABELV $1850
line 4203
;4202:		//if the door is open or opening already
;4203:		if (!VectorCompare(origin, entinfo.origin))
ADDRLP4 3192
ARGP4
ADDRLP4 3052+24
ARGP4
ADDRLP4 3272
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 3272
INDIRI4
CNSTI4 0
NEI4 $1855
line 4204
;4204:			return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1855
line 4206
;4205:		// store all the areas the door is in
;4206:		trap_AAS_ValueForBSPEpairKey(ent, "model", model, sizeof(model));
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 1712
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 4207
;4207:		if (*model) {
ADDRLP4 1712
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $1858
line 4208
;4208:			modelindex = atoi(model+1);
ADDRLP4 1712+1
ARGP4
ADDRLP4 3276
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 3212
ADDRLP4 3276
INDIRI4
ASGNI4
line 4209
;4209:			if (modelindex) {
ADDRLP4 3212
INDIRI4
CNSTI4 0
EQI4 $1861
line 4210
;4210:				VectorClear(angles);
ADDRLP4 3280
CNSTF4 0
ASGNF4
ADDRLP4 3216+8
ADDRLP4 3280
INDIRF4
ASGNF4
ADDRLP4 3216+4
ADDRLP4 3280
INDIRF4
ASGNF4
ADDRLP4 3216
ADDRLP4 3280
INDIRF4
ASGNF4
line 4211
;4211:				BotModelMinsMaxs(modelindex, ET_MOVER, 0, absmins, absmaxs);
ADDRLP4 3212
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 3228
ARGP4
ADDRLP4 3240
ARGP4
ADDRGP4 BotModelMinsMaxs
CALLI4
pop
line 4213
;4212:				//
;4213:				numareas = trap_AAS_BBoxAreas(absmins, absmaxs, areas, MAX_ACTIVATEAREAS*2);
ADDRLP4 3228
ARGP4
ADDRLP4 3240
ARGP4
ADDRLP4 2740
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 3284
ADDRGP4 trap_AAS_BBoxAreas
CALLI4
ASGNI4
ADDRLP4 3048
ADDRLP4 3284
INDIRI4
ASGNI4
line 4215
;4214:				// store the areas with reachabilities first
;4215:				for (i = 0; i < numareas; i++) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $1868
JUMPV
LABELV $1865
line 4216
;4216:					if (activategoal->numareas >= MAX_ACTIVATEAREAS)
ADDRFP4 8
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 32
LTI4 $1869
line 4217
;4217:						break;
ADDRGP4 $1867
JUMPV
LABELV $1869
line 4218
;4218:					if ( !trap_AAS_AreaReachability(areas[i]) ) {
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ARGI4
ADDRLP4 3288
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 3288
INDIRI4
CNSTI4 0
NEI4 $1871
line 4219
;4219:						continue;
ADDRGP4 $1866
JUMPV
LABELV $1871
line 4221
;4220:					}
;4221:					trap_AAS_AreaInfo(areas[i], &areainfo);
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ARGI4
ADDRLP4 2996
ARGP4
ADDRGP4 trap_AAS_AreaInfo
CALLI4
pop
line 4222
;4222:					if (areainfo.contents & AREACONTENTS_MOVER) {
ADDRLP4 2996
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1873
line 4223
;4223:						activategoal->areas[activategoal->numareas++] = areas[i];
ADDRLP4 3296
ADDRFP4 8
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 3292
ADDRLP4 3296
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 3296
INDIRP4
ADDRLP4 3292
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 3300
CNSTI4 2
ASGNI4
ADDRLP4 3292
INDIRI4
ADDRLP4 3300
INDIRI4
LSHI4
ADDRFP4 8
INDIRP4
CNSTI4 104
ADDP4
ADDP4
ADDRLP4 132
INDIRI4
ADDRLP4 3300
INDIRI4
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ASGNI4
line 4224
;4224:					}
LABELV $1873
line 4225
;4225:				}
LABELV $1866
line 4215
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1868
ADDRLP4 132
INDIRI4
ADDRLP4 3048
INDIRI4
LTI4 $1865
LABELV $1867
line 4227
;4226:				// store any remaining areas
;4227:				for (i = 0; i < numareas; i++) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $1878
JUMPV
LABELV $1875
line 4228
;4228:					if (activategoal->numareas >= MAX_ACTIVATEAREAS)
ADDRFP4 8
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 32
LTI4 $1879
line 4229
;4229:						break;
ADDRGP4 $1877
JUMPV
LABELV $1879
line 4230
;4230:					if ( trap_AAS_AreaReachability(areas[i]) ) {
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ARGI4
ADDRLP4 3288
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 3288
INDIRI4
CNSTI4 0
EQI4 $1881
line 4231
;4231:						continue;
ADDRGP4 $1876
JUMPV
LABELV $1881
line 4233
;4232:					}
;4233:					trap_AAS_AreaInfo(areas[i], &areainfo);
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ARGI4
ADDRLP4 2996
ARGP4
ADDRGP4 trap_AAS_AreaInfo
CALLI4
pop
line 4234
;4234:					if (areainfo.contents & AREACONTENTS_MOVER) {
ADDRLP4 2996
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1883
line 4235
;4235:						activategoal->areas[activategoal->numareas++] = areas[i];
ADDRLP4 3296
ADDRFP4 8
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 3292
ADDRLP4 3296
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 3296
INDIRP4
ADDRLP4 3292
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 3300
CNSTI4 2
ASGNI4
ADDRLP4 3292
INDIRI4
ADDRLP4 3300
INDIRI4
LSHI4
ADDRFP4 8
INDIRP4
CNSTI4 104
ADDP4
ADDP4
ADDRLP4 132
INDIRI4
ADDRLP4 3300
INDIRI4
LSHI4
ADDRLP4 2740
ADDP4
INDIRI4
ASGNI4
line 4236
;4236:					}
LABELV $1883
line 4237
;4237:				}
LABELV $1876
line 4227
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1878
ADDRLP4 132
INDIRI4
ADDRLP4 3048
INDIRI4
LTI4 $1875
LABELV $1877
line 4238
;4238:			}
LABELV $1861
line 4239
;4239:		}
LABELV $1858
line 4240
;4240:	}
LABELV $1840
line 4242
;4241:	// if the bot is blocked by or standing on top of a button
;4242:	if (!strcmp(classname, "func_button")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1887
ARGP4
ADDRLP4 3264
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3264
INDIRI4
CNSTI4 0
NEI4 $1885
line 4243
;4243:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1885
line 4246
;4244:	}
;4245:	// get the targetname so we can find an entity with a matching target
;4246:	if (!trap_AAS_ValueForBSPEpairKey(ent, "targetname", targetname[0], sizeof(targetname[0]))) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1890
ARGP4
ADDRLP4 136
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 3268
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3268
INDIRI4
CNSTI4 0
NEI4 $1888
line 4247
;4247:		if (bot_developer.integer) {
ADDRGP4 bot_developer+12
INDIRI4
CNSTI4 0
EQI4 $1891
line 4248
;4248:			BotAI_Print(PRT_ERROR, "BotGetActivateGoal: entity with model \"%s\" has no targetname\n", model);
CNSTI4 3
ARGI4
ADDRGP4 $1894
ARGP4
ADDRLP4 1712
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4249
;4249:		}
LABELV $1891
line 4250
;4250:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1888
line 4253
;4251:	}
;4252:	// allow tree-like activation
;4253:	cur_entities[0] = trap_AAS_NextBSPEntity(0);
CNSTI4 0
ARGI4
ADDRLP4 3272
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 1416
ADDRLP4 3272
INDIRI4
ASGNI4
line 4254
;4254:	for (i = 0; i >= 0 && i < 10;) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $1898
JUMPV
LABELV $1895
line 4255
;4255:		for (ent = cur_entities[i]; ent; ent = trap_AAS_NextBSPEntity(ent)) {
ADDRLP4 0
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1416
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $1902
JUMPV
LABELV $1899
line 4256
;4256:			if (!trap_AAS_ValueForBSPEpairKey(ent, "target", target, sizeof(target))) continue;
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1905
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 3276
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3276
INDIRI4
CNSTI4 0
NEI4 $1903
ADDRGP4 $1900
JUMPV
LABELV $1903
line 4257
;4257:			if (!strcmp(targetname[i], target)) {
ADDRLP4 132
INDIRI4
CNSTI4 7
LSHI4
ADDRLP4 136
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 3280
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3280
INDIRI4
CNSTI4 0
NEI4 $1906
line 4258
;4258:				cur_entities[i] = trap_AAS_NextBSPEntity(ent);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 3284
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1416
ADDP4
ADDRLP4 3284
INDIRI4
ASGNI4
line 4259
;4259:				break;
ADDRGP4 $1901
JUMPV
LABELV $1906
line 4261
;4260:			}
;4261:		}
LABELV $1900
line 4255
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 3276
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 3276
INDIRI4
ASGNI4
LABELV $1902
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1899
LABELV $1901
line 4262
;4262:		if (!ent) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1908
line 4263
;4263:			if (bot_developer.integer) {
ADDRGP4 bot_developer+12
INDIRI4
CNSTI4 0
EQI4 $1910
line 4264
;4264:				BotAI_Print(PRT_ERROR, "BotGetActivateGoal: no entity with target \"%s\"\n", targetname[i]);
CNSTI4 3
ARGI4
ADDRGP4 $1913
ARGP4
ADDRLP4 132
INDIRI4
CNSTI4 7
LSHI4
ADDRLP4 136
ADDP4
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4265
;4265:			}
LABELV $1910
line 4266
;4266:			i--;
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 4267
;4267:			continue;
ADDRGP4 $1896
JUMPV
LABELV $1908
line 4269
;4268:		}
;4269:		if (!trap_AAS_ValueForBSPEpairKey(ent, "classname", classname, sizeof(classname))) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1836
ARGP4
ADDRLP4 1456
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 3280
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3280
INDIRI4
CNSTI4 0
NEI4 $1914
line 4270
;4270:			if (bot_developer.integer) {
ADDRGP4 bot_developer+12
INDIRI4
CNSTI4 0
EQI4 $1896
line 4271
;4271:				BotAI_Print(PRT_ERROR, "BotGetActivateGoal: entity with target \"%s\" has no classname\n", targetname[i]);
CNSTI4 3
ARGI4
ADDRGP4 $1919
ARGP4
ADDRLP4 132
INDIRI4
CNSTI4 7
LSHI4
ADDRLP4 136
ADDP4
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4272
;4272:			}
line 4273
;4273:			continue;
ADDRGP4 $1896
JUMPV
LABELV $1914
line 4276
;4274:		}
;4275:		// BSP button model
;4276:		if (!strcmp(classname, "func_button")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1887
ARGP4
ADDRLP4 3284
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3284
INDIRI4
CNSTI4 0
NEI4 $1920
line 4278
;4277:			//
;4278:			if (!BotFuncButtonActivateGoal(bs, ent, activategoal))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 3288
ADDRGP4 BotFuncButtonActivateGoal
CALLI4
ASGNI4
ADDRLP4 3288
INDIRI4
CNSTI4 0
NEI4 $1922
line 4279
;4279:				continue;
ADDRGP4 $1896
JUMPV
LABELV $1922
line 4281
;4280:			// if the bot tries to activate this button already
;4281:			if ( bs->activatestack && bs->activatestack->inuse &&
ADDRLP4 3292
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
ADDRLP4 3292
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1924
ADDRLP4 3292
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $1924
ADDRLP4 3296
CNSTI4 44
ASGNI4
ADDRLP4 3292
INDIRP4
ADDRLP4 3296
INDIRI4
ADDP4
INDIRI4
ADDRFP4 8
INDIRP4
ADDRLP4 3296
INDIRI4
ADDP4
INDIRI4
NEI4 $1924
ADDRLP4 3300
ADDRGP4 floattime
INDIRF4
ASGNF4
ADDRLP4 3292
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
ADDRLP4 3300
INDIRF4
LEF4 $1924
ADDRLP4 3292
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDRLP4 3300
INDIRF4
CNSTF4 1073741824
SUBF4
GEF4 $1924
line 4285
;4282:				 bs->activatestack->goal.entitynum == activategoal->goal.entitynum &&
;4283:				 bs->activatestack->time > FloatTime() &&
;4284:				 bs->activatestack->start_time < FloatTime() - 2)
;4285:				continue;
ADDRGP4 $1896
JUMPV
LABELV $1924
line 4287
;4286:			// if the bot is in a reachability area
;4287:			if ( trap_AAS_AreaReachability(bs->areanum) ) {
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 3304
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 3304
INDIRI4
CNSTI4 0
EQI4 $1926
line 4289
;4288:				// disable all areas the blocking entity is in
;4289:				BotEnableActivateGoalAreas( activategoal, qfalse );
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4291
;4290:				//
;4291:				t = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, activategoal->goal.areanum, bs->tfl);
ADDRLP4 3308
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 3308
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 3308
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRLP4 3308
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 3312
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 2736
ADDRLP4 3312
INDIRI4
ASGNI4
line 4293
;4292:				// if the button is not reachable
;4293:				if (!t) {
ADDRLP4 2736
INDIRI4
CNSTI4 0
NEI4 $1928
line 4294
;4294:					continue;
ADDRGP4 $1896
JUMPV
LABELV $1928
line 4296
;4295:				}
;4296:				activategoal->time = FloatTime() + t * 0.01 + 5;
ADDRFP4 8
INDIRP4
CNSTI4 60
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1008981770
ADDRLP4 2736
INDIRI4
CVIF4 4
MULF4
ADDF4
CNSTF4 1084227584
ADDF4
ASGNF4
line 4297
;4297:			}
LABELV $1926
line 4298
;4298:			return ent;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1920
line 4301
;4299:		}
;4300:		// invisible trigger multiple box
;4301:		else if (!strcmp(classname, "trigger_multiple")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1932
ARGP4
ADDRLP4 3288
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3288
INDIRI4
CNSTI4 0
NEI4 $1930
line 4303
;4302:			//
;4303:			if (!BotTriggerMultipleActivateGoal(bs, ent, activategoal))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 3292
ADDRGP4 BotTriggerMultipleActivateGoal
CALLI4
ASGNI4
ADDRLP4 3292
INDIRI4
CNSTI4 0
NEI4 $1933
line 4304
;4304:				continue;
ADDRGP4 $1896
JUMPV
LABELV $1933
line 4306
;4305:			// if the bot tries to activate this trigger already
;4306:			if ( bs->activatestack && bs->activatestack->inuse &&
ADDRLP4 3296
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
ADDRLP4 3296
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1935
ADDRLP4 3296
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $1935
ADDRLP4 3300
CNSTI4 44
ASGNI4
ADDRLP4 3296
INDIRP4
ADDRLP4 3300
INDIRI4
ADDP4
INDIRI4
ADDRFP4 8
INDIRP4
ADDRLP4 3300
INDIRI4
ADDP4
INDIRI4
NEI4 $1935
ADDRLP4 3304
ADDRGP4 floattime
INDIRF4
ASGNF4
ADDRLP4 3296
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
ADDRLP4 3304
INDIRF4
LEF4 $1935
ADDRLP4 3296
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDRLP4 3304
INDIRF4
CNSTF4 1073741824
SUBF4
GEF4 $1935
line 4310
;4307:				 bs->activatestack->goal.entitynum == activategoal->goal.entitynum &&
;4308:				 bs->activatestack->time > FloatTime() &&
;4309:				 bs->activatestack->start_time < FloatTime() - 2)
;4310:				continue;
ADDRGP4 $1896
JUMPV
LABELV $1935
line 4312
;4311:			// if the bot is in a reachability area
;4312:			if ( trap_AAS_AreaReachability(bs->areanum) ) {
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 3308
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 3308
INDIRI4
CNSTI4 0
EQI4 $1937
line 4314
;4313:				// disable all areas the blocking entity is in
;4314:				BotEnableActivateGoalAreas( activategoal, qfalse );
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4316
;4315:				//
;4316:				t = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, activategoal->goal.areanum, bs->tfl);
ADDRLP4 3312
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 3312
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 3312
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRLP4 3312
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 3316
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 2736
ADDRLP4 3316
INDIRI4
ASGNI4
line 4318
;4317:				// if the trigger is not reachable
;4318:				if (!t) {
ADDRLP4 2736
INDIRI4
CNSTI4 0
NEI4 $1939
line 4319
;4319:					continue;
ADDRGP4 $1896
JUMPV
LABELV $1939
line 4321
;4320:				}
;4321:				activategoal->time = FloatTime() + t * 0.01 + 5;
ADDRFP4 8
INDIRP4
CNSTI4 60
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1008981770
ADDRLP4 2736
INDIRI4
CVIF4 4
MULF4
ADDF4
CNSTF4 1084227584
ADDF4
ASGNF4
line 4322
;4322:			}
LABELV $1937
line 4323
;4323:			return ent;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1822
JUMPV
LABELV $1930
line 4325
;4324:		}
;4325:		else if (!strcmp(classname, "func_timer")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1943
ARGP4
ADDRLP4 3292
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3292
INDIRI4
CNSTI4 0
NEI4 $1941
line 4327
;4326:			// just skip the func_timer
;4327:			continue;
ADDRGP4 $1896
JUMPV
LABELV $1941
line 4330
;4328:		}
;4329:		// the actual button or trigger might be linked through a target_relay or target_delay
;4330:		else if (!strcmp(classname, "target_relay") || !strcmp(classname, "target_delay")) {
ADDRLP4 1456
ARGP4
ADDRGP4 $1946
ARGP4
ADDRLP4 3296
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3296
INDIRI4
CNSTI4 0
EQI4 $1948
ADDRLP4 1456
ARGP4
ADDRGP4 $1947
ARGP4
ADDRLP4 3300
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 3300
INDIRI4
CNSTI4 0
NEI4 $1944
LABELV $1948
line 4331
;4331:			if (trap_AAS_ValueForBSPEpairKey(ent, "targetname", targetname[i+1], sizeof(targetname[0]))) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1890
ARGP4
ADDRLP4 132
INDIRI4
CNSTI4 7
LSHI4
ADDRLP4 136+128
ADDP4
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 3304
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 3304
INDIRI4
CNSTI4 0
EQI4 $1949
line 4332
;4332:				i++;
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4333
;4333:				cur_entities[i] = trap_AAS_NextBSPEntity(0);
CNSTI4 0
ARGI4
ADDRLP4 3308
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1416
ADDP4
ADDRLP4 3308
INDIRI4
ASGNI4
line 4334
;4334:			}
LABELV $1949
line 4335
;4335:		}
LABELV $1944
line 4336
;4336:	}
LABELV $1896
line 4254
LABELV $1898
ADDRLP4 132
INDIRI4
CNSTI4 0
LTI4 $1952
ADDRLP4 132
INDIRI4
CNSTI4 10
LTI4 $1895
LABELV $1952
line 4340
;4337:#ifdef OBSTACLEDEBUG
;4338:	BotAI_Print(PRT_ERROR, "BotGetActivateGoal: no valid activator for entity with target \"%s\"\n", targetname[0]);
;4339:#endif
;4340:	return 0;
CNSTI4 0
RETI4
LABELV $1822
endproc BotGetActivateGoal 3320 20
export BotGoForActivateGoal
proc BotGoForActivateGoal 144 8
line 4348
;4341:}
;4342:
;4343:/*
;4344:==================
;4345:BotGoForActivateGoal
;4346:==================
;4347:*/
;4348:int BotGoForActivateGoal(bot_state_t *bs, bot_activategoal_t *activategoal) {
line 4351
;4349:	aas_entityinfo_t activateinfo;
;4350:
;4351:	activategoal->inuse = qtrue;
ADDRFP4 4
INDIRP4
CNSTI4 1
ASGNI4
line 4352
;4352:	if (!activategoal->time)
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
CNSTF4 0
NEF4 $1954
line 4353
;4353:		activategoal->time = FloatTime() + 10;
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
ADDF4
ASGNF4
LABELV $1954
line 4354
;4354:	activategoal->start_time = FloatTime();
ADDRFP4 4
INDIRP4
CNSTI4 64
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4355
;4355:	BotEntityInfo(activategoal->goal.entitynum, &activateinfo);
ADDRFP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 4356
;4356:	VectorCopy(activateinfo.origin, activategoal->origin);
ADDRFP4 4
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0+24
INDIRB
ASGNB 12
line 4358
;4357:	//
;4358:	if (BotPushOntoActivateGoalStack(bs, activategoal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotPushOntoActivateGoalStack
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $1957
line 4360
;4359:		// enter the activate entity AI node
;4360:		AIEnter_Seek_ActivateEntity(bs, "BotGoForActivateGoal");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1959
ARGP4
ADDRGP4 AIEnter_Seek_ActivateEntity
CALLV
pop
line 4361
;4361:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1953
JUMPV
LABELV $1957
line 4363
;4362:	}
;4363:	else {
line 4365
;4364:		// enable any routing areas that were disabled
;4365:		BotEnableActivateGoalAreas(activategoal, qtrue);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4366
;4366:		return qfalse;
CNSTI4 0
RETI4
LABELV $1953
endproc BotGoForActivateGoal 144 8
export BotPrintActivateGoalInfo
proc BotPrintActivateGoalInfo 296 36
line 4375
;4367:	}
;4368:}
;4369:
;4370:/*
;4371:==================
;4372:BotPrintActivateGoalInfo
;4373:==================
;4374:*/
;4375:void BotPrintActivateGoalInfo(bot_state_t *bs, bot_activategoal_t *activategoal, int bspent) {
line 4380
;4376:	char netname[MAX_NETNAME];
;4377:	char classname[128];
;4378:	char buf[128];
;4379:
;4380:	ClientName(bs->client, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 4381
;4381:	trap_AAS_ValueForBSPEpairKey(bspent, "classname", classname, sizeof(classname));
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 $1836
ARGP4
ADDRLP4 36
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
pop
line 4382
;4382:	if (activategoal->shoot) {
ADDRFP4 4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
EQI4 $1961
line 4383
;4383:		Com_sprintf(buf, sizeof(buf), "%s: I have to shoot at a %s from %1.1f %1.1f %1.1f in area %d\n",
ADDRLP4 164
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 $1963
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 292
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 292
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4389
;4384:						netname, classname,
;4385:						activategoal->goal.origin[0],
;4386:						activategoal->goal.origin[1],
;4387:						activategoal->goal.origin[2],
;4388:						activategoal->goal.areanum);
;4389:	}
ADDRGP4 $1962
JUMPV
LABELV $1961
line 4390
;4390:	else {
line 4391
;4391:		Com_sprintf(buf, sizeof(buf), "%s: I have to activate a %s at %1.1f %1.1f %1.1f in area %d\n",
ADDRLP4 164
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 $1964
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 292
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 292
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 292
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4397
;4392:						netname, classname,
;4393:						activategoal->goal.origin[0],
;4394:						activategoal->goal.origin[1],
;4395:						activategoal->goal.origin[2],
;4396:						activategoal->goal.areanum);
;4397:	}
LABELV $1962
line 4398
;4398:	trap_EA_Say(bs->client, buf);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 164
ARGP4
ADDRGP4 trap_EA_Say
CALLV
pop
line 4399
;4399:}
LABELV $1960
endproc BotPrintActivateGoalInfo 296 36
export BotRandomMove
proc BotRandomMove 32 16
line 4406
;4400:
;4401:/*
;4402:==================
;4403:BotRandomMove
;4404:==================
;4405:*/
;4406:void BotRandomMove(bot_state_t *bs, bot_moveresult_t *moveresult) {
line 4409
;4407:	vec3_t dir, angles;
;4408:
;4409:	angles[0] = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 4410
;4410:	angles[1] = random() * 360;
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
CNSTF4 1135869952
ADDRLP4 24
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ASGNF4
line 4411
;4411:	angles[2] = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 4412
;4412:	AngleVectors(angles, dir, NULL, NULL);
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
CNSTP4 0
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 4414
;4413:
;4414:	trap_BotMoveInDirection(bs->ms, dir, 400, MOVE_WALK);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTF4 1137180672
ARGF4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotMoveInDirection
CALLI4
pop
line 4416
;4415:
;4416:	moveresult->failure = qfalse;
ADDRFP4 4
INDIRP4
CNSTI4 0
ASGNI4
line 4417
;4417:	VectorCopy(dir, moveresult->movedir);
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 12
INDIRB
ASGNB 12
line 4418
;4418:}
LABELV $1965
endproc BotRandomMove 32 16
lit
align 4
LABELV $1969
byte 4 0
byte 4 0
byte 4 1065353216
export BotAIBlocked
code
proc BotAIBlocked 528 16
line 4431
;4419:
;4420:/*
;4421:==================
;4422:BotAIBlocked
;4423:
;4424:Very basic handling of bots being blocked by other entities.
;4425:Check what kind of entity is blocking the bot and try to activate
;4426:it. If that's not an option then try to walk around or over the entity.
;4427:Before the bot ends in this part of the AI it should predict which doors to
;4428:open, which buttons to activate etc.
;4429:==================
;4430:*/
;4431:void BotAIBlocked(bot_state_t *bs, bot_moveresult_t *moveresult, int activate) {
line 4433
;4432:	int movetype, bspent;
;4433:	vec3_t hordir, start, end, mins, maxs, sideward, angles, up = {0, 0, 1};
ADDRLP4 228
ADDRGP4 $1969
INDIRB
ASGNB 12
line 4438
;4434:	aas_entityinfo_t entinfo;
;4435:	bot_activategoal_t activategoal;
;4436:
;4437:	// if the bot is not blocked by anything
;4438:	if (!moveresult->blocked) {
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1970
line 4439
;4439:		bs->notblocked_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6204
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4440
;4440:		return;
ADDRGP4 $1968
JUMPV
LABELV $1970
line 4443
;4441:	}
;4442:	// if stuck in a solid area
;4443:	if ( moveresult->type == RESULTTYPE_INSOLIDAREA ) {
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 8
NEI4 $1972
line 4445
;4444:		// move in a random direction in the hope to get out
;4445:		BotRandomMove(bs, moveresult);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRandomMove
CALLV
pop
line 4447
;4446:		//
;4447:		return;
ADDRGP4 $1968
JUMPV
LABELV $1972
line 4450
;4448:	}
;4449:	// get info for the entity that is blocking the bot
;4450:	BotEntityInfo(moveresult->blockentity, &entinfo);
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 40
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 4456
;4451:#ifdef OBSTACLEDEBUG
;4452:	ClientName(bs->client, netname, sizeof(netname));
;4453:	BotAI_Print(PRT_MESSAGE, "%s: I'm blocked by model %d\n", netname, entinfo.modelindex);
;4454:#endif // OBSTACLEDEBUG
;4455:	// if blocked by a bsp model and the bot wants to activate it
;4456:	if (activate && entinfo.modelindex > 0 && entinfo.modelindex <= max_bspmodelindex) {
ADDRLP4 488
CNSTI4 0
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 488
INDIRI4
EQI4 $1974
ADDRLP4 40+104
INDIRI4
ADDRLP4 488
INDIRI4
LEI4 $1974
ADDRLP4 40+104
INDIRI4
ADDRGP4 max_bspmodelindex
INDIRI4
GTI4 $1974
line 4458
;4457:		// find the bsp entity which should be activated in order to get the blocking entity out of the way
;4458:		bspent = BotGetActivateGoal(bs, entinfo.number, &activategoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40+20
INDIRI4
ARGI4
ADDRLP4 240
ARGP4
ADDRLP4 492
ADDRGP4 BotGetActivateGoal
CALLI4
ASGNI4
ADDRLP4 484
ADDRLP4 492
INDIRI4
ASGNI4
line 4459
;4459:		if (bspent) {
ADDRLP4 484
INDIRI4
CNSTI4 0
EQI4 $1979
line 4461
;4460:			//
;4461:			if (bs->activatestack && !bs->activatestack->inuse)
ADDRLP4 496
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
ADDRLP4 496
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1981
ADDRLP4 496
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $1981
line 4462
;4462:				bs->activatestack = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
CNSTP4 0
ASGNP4
LABELV $1981
line 4464
;4463:			// if not already trying to activate this entity
;4464:			if (!BotIsGoingToActivateEntity(bs, activategoal.goal.entitynum)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 240+4+40
INDIRI4
ARGI4
ADDRLP4 500
ADDRGP4 BotIsGoingToActivateEntity
CALLI4
ASGNI4
ADDRLP4 500
INDIRI4
CNSTI4 0
NEI4 $1983
line 4466
;4465:				//
;4466:				BotGoForActivateGoal(bs, &activategoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 240
ARGP4
ADDRGP4 BotGoForActivateGoal
CALLI4
pop
line 4467
;4467:			}
LABELV $1983
line 4471
;4468:			// if ontop of an obstacle or
;4469:			// if the bot is not in a reachability area it'll still
;4470:			// need some dynamic obstacle avoidance, otherwise return
;4471:			if (!(moveresult->flags & MOVERESULT_ONTOPOFOBSTACLE) &&
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $1980
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 504
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 504
INDIRI4
CNSTI4 0
EQI4 $1980
line 4473
;4472:				trap_AAS_AreaReachability(bs->areanum))
;4473:				return;
ADDRGP4 $1968
JUMPV
line 4474
;4474:		}
LABELV $1979
line 4475
;4475:		else {
line 4477
;4476:			// enable any routing areas that were disabled
;4477:			BotEnableActivateGoalAreas(&activategoal, qtrue);
ADDRLP4 240
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4478
;4478:		}
LABELV $1980
line 4479
;4479:	}
LABELV $1974
line 4481
;4480:	// just some basic dynamic obstacle avoidance code
;4481:	hordir[0] = moveresult->movedir[0];
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
line 4482
;4482:	hordir[1] = moveresult->movedir[1];
ADDRLP4 0+4
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ASGNF4
line 4483
;4483:	hordir[2] = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 4485
;4484:	// if no direction just take a random direction
;4485:	if (VectorNormalize(hordir) < 0.1) {
ADDRLP4 0
ARGP4
ADDRLP4 492
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 492
INDIRF4
CNSTF4 1036831949
GEF4 $1991
line 4486
;4486:		VectorSet(angles, 0, 360 * random(), 0);
ADDRLP4 216
CNSTF4 0
ASGNF4
ADDRLP4 496
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 216+4
CNSTF4 1135869952
ADDRLP4 496
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ASGNF4
ADDRLP4 216+8
CNSTF4 0
ASGNF4
line 4487
;4487:		AngleVectors(angles, hordir, NULL, NULL);
ADDRLP4 216
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 500
CNSTP4 0
ASGNP4
ADDRLP4 500
INDIRP4
ARGP4
ADDRLP4 500
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 4488
;4488:	}
LABELV $1991
line 4492
;4489:	//
;4490:	//if (moveresult->flags & MOVERESULT_ONTOPOFOBSTACLE) movetype = MOVE_JUMP;
;4491:	//else
;4492:	movetype = MOVE_WALK;
ADDRLP4 36
CNSTI4 1
ASGNI4
line 4495
;4493:	// if there's an obstacle at the bot's feet and head then
;4494:	// the bot might be able to crouch through
;4495:	VectorCopy(bs->origin, start);
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 4496
;4496:	start[2] += 18;
ADDRLP4 24+8
ADDRLP4 24+8
INDIRF4
CNSTF4 1099956224
ADDF4
ASGNF4
line 4497
;4497:	VectorMA(start, 5, hordir, end);
ADDRLP4 496
CNSTF4 1084227584
ASGNF4
ADDRLP4 180
ADDRLP4 24
INDIRF4
ADDRLP4 496
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 180+4
ADDRLP4 24+4
INDIRF4
ADDRLP4 496
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 180+8
ADDRLP4 24+8
INDIRF4
CNSTF4 1084227584
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 4498
;4498:	VectorSet(mins, -16, -16, -24);
ADDRLP4 500
CNSTF4 3246391296
ASGNF4
ADDRLP4 192
ADDRLP4 500
INDIRF4
ASGNF4
ADDRLP4 192+4
ADDRLP4 500
INDIRF4
ASGNF4
ADDRLP4 192+8
CNSTF4 3250585600
ASGNF4
line 4499
;4499:	VectorSet(maxs, 16, 16, 4);
ADDRLP4 504
CNSTF4 1098907648
ASGNF4
ADDRLP4 204
ADDRLP4 504
INDIRF4
ASGNF4
ADDRLP4 204+4
ADDRLP4 504
INDIRF4
ASGNF4
ADDRLP4 204+8
CNSTF4 1082130432
ASGNF4
line 4504
;4500:	//
;4501:	//bsptrace = AAS_Trace(start, mins, maxs, end, bs->entitynum, MASK_PLAYERSOLID);
;4502:	//if (bsptrace.fraction >= 1) movetype = MOVE_CROUCH;
;4503:	// get the sideward vector
;4504:	CrossProduct(hordir, up, sideward);
ADDRLP4 0
ARGP4
ADDRLP4 228
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 4506
;4505:	//
;4506:	if (bs->flags & BFL_AVOIDRIGHT) VectorNegate(sideward, sideward);
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $2006
ADDRLP4 12
ADDRLP4 12
INDIRF4
NEGF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
NEGF4
ASGNF4
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
NEGF4
ASGNF4
LABELV $2006
line 4508
;4507:	// try to crouch straight forward?
;4508:	if (movetype != MOVE_CROUCH || !trap_BotMoveInDirection(bs->ms, hordir, 400, movetype)) {
ADDRLP4 36
INDIRI4
CNSTI4 2
NEI4 $2014
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 512
ADDRGP4 trap_BotMoveInDirection
CALLI4
ASGNI4
ADDRLP4 512
INDIRI4
CNSTI4 0
NEI4 $2012
LABELV $2014
line 4510
;4509:		// perform the movement
;4510:		if (!trap_BotMoveInDirection(bs->ms, sideward, 400, movetype)) {
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 516
ADDRGP4 trap_BotMoveInDirection
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $2015
line 4512
;4511:			// flip the avoid direction flag
;4512:			bs->flags ^= BFL_AVOIDRIGHT;
ADDRLP4 520
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 520
INDIRP4
ADDRLP4 520
INDIRP4
INDIRI4
CNSTI4 16
BXORI4
ASGNI4
line 4515
;4513:			// flip the direction
;4514:			// VectorNegate(sideward, sideward);
;4515:			VectorMA(sideward, -1, hordir, sideward);
ADDRLP4 524
CNSTF4 3212836864
ASGNF4
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 524
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
ADDRLP4 524
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
CNSTF4 3212836864
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 4517
;4516:			// move in the other direction
;4517:			trap_BotMoveInDirection(bs->ms, sideward, 400, movetype);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 36
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveInDirection
CALLI4
pop
line 4518
;4518:		}
LABELV $2015
line 4519
;4519:	}
LABELV $2012
line 4521
;4520:	//
;4521:	if (bs->notblocked_time < FloatTime() - 0.4) {
ADDRFP4 0
INDIRP4
CNSTI4 6204
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1053609165
SUBF4
GEF4 $2023
line 4524
;4522:		// just reset goals and hope the bot will go into another direction?
;4523:		// is this still needed??
;4524:		if (bs->ainode == AINode_Seek_NBG) bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 AINode_Seek_NBG
CVPU4 4
NEU4 $2025
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
ADDRGP4 $2026
JUMPV
LABELV $2025
line 4525
;4525:		else if (bs->ainode == AINode_Seek_LTG) bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 AINode_Seek_LTG
CVPU4 4
NEU4 $2027
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
LABELV $2027
LABELV $2026
line 4526
;4526:	}
LABELV $2023
line 4527
;4527:}
LABELV $1968
endproc BotAIBlocked 528 16
export BotAIPredictObstacles
proc BotAIPredictObstacles 324 44
line 4539
;4528:
;4529:/*
;4530:==================
;4531:BotAIPredictObstacles
;4532:
;4533:Predict the route towards the goal and check if the bot
;4534:will be blocked by certain obstacles. When the bot has obstacles
;4535:on it's path the bot should figure out if they can be removed
;4536:by activating certain entities.
;4537:==================
;4538:*/
;4539:int BotAIPredictObstacles(bot_state_t *bs, bot_goal_t *goal) {
line 4544
;4540:	int modelnum, entitynum, bspent;
;4541:	bot_activategoal_t activategoal;
;4542:	aas_predictroute_t route;
;4543:
;4544:	if (!bot_predictobstacles.integer)
ADDRGP4 bot_predictobstacles+12
INDIRI4
CNSTI4 0
NEI4 $2030
line 4545
;4545:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2029
JUMPV
LABELV $2030
line 4548
;4546:
;4547:	// always predict when the goal change or at regular intervals
;4548:	if (bs->predictobstacles_goalareanum == goal->areanum &&
ADDRLP4 292
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 292
INDIRP4
CNSTI4 6216
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
NEI4 $2033
ADDRLP4 292
INDIRP4
CNSTI4 6212
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1086324736
SUBF4
LEF4 $2033
line 4549
;4549:		bs->predictobstacles_time > FloatTime() - 6) {
line 4550
;4550:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2029
JUMPV
LABELV $2033
line 4552
;4551:	}
;4552:	bs->predictobstacles_goalareanum = goal->areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6216
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 4553
;4553:	bs->predictobstacles_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6212
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4556
;4554:
;4555:	// predict at most 100 areas or 10 seconds ahead
;4556:	trap_AAS_PredictRoute(&route, bs->areanum, bs->origin,
ADDRLP4 0
ARGP4
ADDRLP4 296
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 296
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 296
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 296
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTI4 100
ARGI4
CNSTI4 1000
ARGI4
CNSTI4 6
ARGI4
CNSTI4 1024
ARGI4
CNSTI4 67108864
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_AAS_PredictRoute
CALLI4
pop
line 4561
;4557:							goal->areanum, bs->tfl, 100, 1000,
;4558:							RSE_USETRAVELTYPE|RSE_ENTERCONTENTS,
;4559:							AREACONTENTS_MOVER, TFL_BRIDGE, 0);
;4560:	// if bot has to travel through an area with a mover
;4561:	if (route.stopevent & RSE_ENTERCONTENTS) {
ADDRLP4 0+16
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $2035
line 4563
;4562:		// if the bot will run into a mover
;4563:		if (route.endcontents & AREACONTENTS_MOVER) {
ADDRLP4 0+20
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $2036
line 4565
;4564:			//NOTE: this only works with bspc 2.1 or higher
;4565:			modelnum = (route.endcontents & AREACONTENTS_MODELNUM) >> AREACONTENTS_MODELNUMSHIFT;
ADDRLP4 300
CNSTI4 24
ASGNI4
ADDRLP4 36
ADDRLP4 0+20
INDIRI4
CNSTI4 255
ADDRLP4 300
INDIRI4
LSHI4
BANDI4
ADDRLP4 300
INDIRI4
RSHI4
ASGNI4
line 4566
;4566:			if (modelnum) {
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $2036
line 4568
;4567:				//
;4568:				entitynum = BotModelMinsMaxs(modelnum, ET_MOVER, 0, NULL, NULL);
ADDRLP4 36
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 304
CNSTP4 0
ASGNP4
ADDRLP4 304
INDIRP4
ARGP4
ADDRLP4 304
INDIRP4
ARGP4
ADDRLP4 308
ADDRGP4 BotModelMinsMaxs
CALLI4
ASGNI4
ADDRLP4 40
ADDRLP4 308
INDIRI4
ASGNI4
line 4569
;4569:				if (entitynum) {
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $2036
line 4571
;4570:					//NOTE: BotGetActivateGoal already checks if the door is open or not
;4571:					bspent = BotGetActivateGoal(bs, entitynum, &activategoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 48
ARGP4
ADDRLP4 312
ADDRGP4 BotGetActivateGoal
CALLI4
ASGNI4
ADDRLP4 44
ADDRLP4 312
INDIRI4
ASGNI4
line 4572
;4572:					if (bspent) {
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $2036
line 4574
;4573:						//
;4574:						if (bs->activatestack && !bs->activatestack->inuse)
ADDRLP4 316
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
ASGNP4
ADDRLP4 316
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2048
ADDRLP4 316
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $2048
line 4575
;4575:							bs->activatestack = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
CNSTP4 0
ASGNP4
LABELV $2048
line 4577
;4576:						// if not already trying to activate this entity
;4577:						if (!BotIsGoingToActivateEntity(bs, activategoal.goal.entitynum)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48+4+40
INDIRI4
ARGI4
ADDRLP4 320
ADDRGP4 BotIsGoingToActivateEntity
CALLI4
ASGNI4
ADDRLP4 320
INDIRI4
CNSTI4 0
NEI4 $2050
line 4581
;4578:							//
;4579:							//BotAI_Print(PRT_MESSAGE, "blocked by mover model %d, entity %d ?\n", modelnum, entitynum);
;4580:							//
;4581:							BotGoForActivateGoal(bs, &activategoal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 BotGoForActivateGoal
CALLI4
pop
line 4582
;4582:							return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2029
JUMPV
LABELV $2050
line 4584
;4583:						}
;4584:						else {
line 4586
;4585:							// enable any routing areas that were disabled
;4586:							BotEnableActivateGoalAreas(&activategoal, qtrue);
ADDRLP4 48
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotEnableActivateGoalAreas
CALLV
pop
line 4587
;4587:						}
line 4588
;4588:					}
line 4589
;4589:				}
line 4590
;4590:			}
line 4591
;4591:		}
line 4592
;4592:	}
ADDRGP4 $2036
JUMPV
LABELV $2035
line 4593
;4593:	else if (route.stopevent & RSE_USETRAVELTYPE) {
ADDRLP4 0+16
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $2054
line 4594
;4594:		if (route.endtravelflags & TFL_BRIDGE) {
ADDRLP4 0+24
INDIRI4
CNSTI4 67108864
BANDI4
CNSTI4 0
EQI4 $2057
line 4596
;4595:			//FIXME: check if the bridge is available to travel over
;4596:		}
LABELV $2057
line 4597
;4597:	}
LABELV $2054
LABELV $2036
line 4598
;4598:	return qfalse;
CNSTI4 0
RETI4
LABELV $2029
endproc BotAIPredictObstacles 324 44
export BotCheckConsoleMessages
proc BotCheckConsoleMessages 1012 48
line 4606
;4599:}
;4600:
;4601:/*
;4602:==================
;4603:BotCheckConsoleMessages
;4604:==================
;4605:*/
;4606:void BotCheckConsoleMessages(bot_state_t *bs) {
line 4614
;4607:	char botname[MAX_NETNAME], message[MAX_MESSAGE_SIZE], netname[MAX_NETNAME], *ptr;
;4608:	float chat_reply;
;4609:	int context, handle;
;4610:	bot_consolemessage_t m;
;4611:	bot_match_t match;
;4612:
;4613:	//the name of this bot
;4614:	ClientName(bs->client, botname, sizeof(botname));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 908
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
ADDRGP4 $2062
JUMPV
LABELV $2061
line 4616
;4615:	//
;4616:	while((handle = trap_BotNextConsoleMessage(bs->cs, &m)) != 0) {
line 4618
;4617:		//if the chat state is flooded with messages the bot will read them quickly
;4618:		if (trap_BotNumConsoleMessages(bs->cs) < 10) {
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 948
ADDRGP4 trap_BotNumConsoleMessages
CALLI4
ASGNI4
ADDRLP4 948
INDIRI4
CNSTI4 10
GEI4 $2064
line 4620
;4619:			//if it is a chat message the bot needs some time to read it
;4620:			if (m.type == CMS_CHAT && m.time > FloatTime() - (1 + random())) break;
ADDRLP4 0+8
INDIRI4
CNSTI4 1
NEI4 $2066
ADDRLP4 952
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
INDIRF4
ADDRGP4 floattime
INDIRF4
ADDRLP4 952
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1065353216
ADDF4
SUBF4
LEF4 $2066
ADDRGP4 $2063
JUMPV
LABELV $2066
line 4621
;4621:		}
LABELV $2064
line 4623
;4622:		//
;4623:		ptr = m.message;
ADDRLP4 276
ADDRLP4 0+12
ASGNP4
line 4626
;4624:		//if it is a chat message then don't unify white spaces and don't
;4625:		//replace synonyms in the netname
;4626:		if (m.type == CMS_CHAT) {
ADDRLP4 0+8
INDIRI4
CNSTI4 1
NEI4 $2071
line 4628
;4627:			//
;4628:			if (trap_BotFindMatch(m.message, &match, MTCONTEXT_REPLYCHAT)) {
ADDRLP4 0+12
ARGP4
ADDRLP4 288
ARGP4
CNSTU4 128
ARGU4
ADDRLP4 952
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 952
INDIRI4
CNSTI4 0
EQI4 $2074
line 4629
;4629:				ptr = m.message + match.variables[MESSAGE].offset;
ADDRLP4 276
ADDRLP4 288+264+16
INDIRI1
CVII4 1
ADDRLP4 0+12
ADDP4
ASGNP4
line 4630
;4630:			}
LABELV $2074
line 4631
;4631:		}
LABELV $2071
line 4633
;4632:		//unify the white spaces in the message
;4633:		trap_UnifyWhiteSpaces(ptr);
ADDRLP4 276
INDIRP4
ARGP4
ADDRGP4 trap_UnifyWhiteSpaces
CALLV
pop
line 4635
;4634:		//replace synonyms in the right context
;4635:		context = BotSynonymContext(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 952
ADDRGP4 BotSynonymContext
CALLI4
ASGNI4
ADDRLP4 284
ADDRLP4 952
INDIRI4
ASGNI4
line 4636
;4636:		trap_BotReplaceSynonyms(ptr, context);
ADDRLP4 276
INDIRP4
ARGP4
ADDRLP4 284
INDIRI4
CVIU4 4
ARGU4
ADDRGP4 trap_BotReplaceSynonyms
CALLV
pop
line 4638
;4637:		//if there's no match
;4638:		if (!BotMatchMessage(bs, m.message)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+12
ARGP4
ADDRLP4 956
ADDRGP4 BotMatchMessage
CALLI4
ASGNI4
ADDRLP4 956
INDIRI4
CNSTI4 0
NEI4 $2080
line 4640
;4639:			//if it is a chat message
;4640:			if (m.type == CMS_CHAT && !bot_nochat.integer) {
ADDRLP4 0+8
INDIRI4
CNSTI4 1
NEI4 $2083
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
NEI4 $2083
line 4642
;4641:				//
;4642:				if (!trap_BotFindMatch(m.message, &match, MTCONTEXT_REPLYCHAT)) {
ADDRLP4 0+12
ARGP4
ADDRLP4 288
ARGP4
CNSTU4 128
ARGU4
ADDRLP4 960
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 960
INDIRI4
CNSTI4 0
NEI4 $2087
line 4643
;4643:					trap_BotRemoveConsoleMessage(bs->cs, handle);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveConsoleMessage
CALLV
pop
line 4644
;4644:					continue;
ADDRGP4 $2062
JUMPV
LABELV $2087
line 4647
;4645:				}
;4646:				//don't use eliza chats with team messages
;4647:				if (match.subtype & ST_TEAM) {
ADDRLP4 288+260
INDIRI4
CNSTI4 32768
BANDI4
CNSTI4 0
EQI4 $2090
line 4648
;4648:					trap_BotRemoveConsoleMessage(bs->cs, handle);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveConsoleMessage
CALLV
pop
line 4649
;4649:					continue;
ADDRGP4 $2062
JUMPV
LABELV $2090
line 4652
;4650:				}
;4651:				//
;4652:				trap_BotMatchVariable(&match, NETNAME, netname, sizeof(netname));
ADDRLP4 288
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 872
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 4653
;4653:				trap_BotMatchVariable(&match, MESSAGE, message, sizeof(message));
ADDRLP4 288
ARGP4
CNSTI4 2
ARGI4
ADDRLP4 616
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 4655
;4654:				//if this is a message from the bot self
;4655:				if (bs->client == ClientFromName(netname)) {
ADDRLP4 872
ARGP4
ADDRLP4 964
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDRLP4 964
INDIRI4
NEI4 $2093
line 4656
;4656:					trap_BotRemoveConsoleMessage(bs->cs, handle);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveConsoleMessage
CALLV
pop
line 4657
;4657:					continue;
ADDRGP4 $2062
JUMPV
LABELV $2093
line 4660
;4658:				}
;4659:				//unify the message
;4660:				trap_UnifyWhiteSpaces(message);
ADDRLP4 616
ARGP4
ADDRGP4 trap_UnifyWhiteSpaces
CALLV
pop
line 4662
;4661:				//
;4662:				trap_Cvar_Update(&bot_testrchat);
ADDRGP4 bot_testrchat
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 4663
;4663:				if (bot_testrchat.integer) {
ADDRGP4 bot_testrchat+12
INDIRI4
CNSTI4 0
EQI4 $2095
line 4665
;4664:					//
;4665:					trap_BotLibVarSet("bot_testrchat", "1");
ADDRGP4 $2098
ARGP4
ADDRGP4 $2099
ARGP4
ADDRGP4 trap_BotLibVarSet
CALLI4
pop
line 4667
;4666:					//if bot replies with a chat message
;4667:					if (trap_BotReplyChat(bs->cs, message, context, CONTEXT_REPLY,
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 616
ARGP4
ADDRLP4 284
INDIRI4
ARGI4
CNSTI4 16
ARGI4
ADDRLP4 968
CNSTP4 0
ASGNP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 908
ARGP4
ADDRLP4 872
ARGP4
ADDRLP4 972
ADDRGP4 trap_BotReplyChat
CALLI4
ASGNI4
ADDRLP4 972
INDIRI4
CNSTI4 0
EQI4 $2100
line 4671
;4668:															NULL, NULL,
;4669:															NULL, NULL,
;4670:															NULL, NULL,
;4671:															botname, netname)) {
line 4672
;4672:						BotAI_Print(PRT_MESSAGE, "------------------------\n");
CNSTI4 1
ARGI4
ADDRGP4 $2102
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4673
;4673:					}
ADDRGP4 $2096
JUMPV
LABELV $2100
line 4674
;4674:					else {
line 4675
;4675:						BotAI_Print(PRT_MESSAGE, "**** no valid reply ****\n");
CNSTI4 1
ARGI4
ADDRGP4 $2103
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 4676
;4676:					}
line 4677
;4677:				}
ADDRGP4 $2096
JUMPV
LABELV $2095
line 4679
;4678:				//if at a valid chat position and not chatting already and not in teamplay
;4679:				else if (bs->ainode != AINode_Stand && BotValidChatPosition(bs) && !TeamPlayIsOn()) {
ADDRLP4 968
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 968
INDIRP4
CNSTI4 4900
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 AINode_Stand
CVPU4 4
EQU4 $2104
ADDRLP4 968
INDIRP4
ARGP4
ADDRLP4 972
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 972
INDIRI4
CNSTI4 0
EQI4 $2104
ADDRLP4 976
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 976
INDIRI4
CNSTI4 0
NEI4 $2104
line 4680
;4680:					chat_reply = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_REPLY, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 35
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 980
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 944
ADDRLP4 980
INDIRF4
ASGNF4
line 4681
;4681:					if (random() < 1.5 / (NumBots()+1) && random() < chat_reply) {
ADDRLP4 984
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 988
ADDRGP4 NumBots
CALLI4
ASGNI4
ADDRLP4 984
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1069547520
ADDRLP4 988
INDIRI4
CNSTI4 1
ADDI4
CVIF4 4
DIVF4
GEF4 $2106
ADDRLP4 992
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 992
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 944
INDIRF4
GEF4 $2106
line 4683
;4682:						//if bot replies with a chat message
;4683:						if (trap_BotReplyChat(bs->cs, message, context, CONTEXT_REPLY,
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 616
ARGP4
ADDRLP4 284
INDIRI4
ARGI4
CNSTI4 16
ARGI4
ADDRLP4 996
CNSTP4 0
ASGNP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 996
INDIRP4
ARGP4
ADDRLP4 908
ARGP4
ADDRLP4 872
ARGP4
ADDRLP4 1000
ADDRGP4 trap_BotReplyChat
CALLI4
ASGNI4
ADDRLP4 1000
INDIRI4
CNSTI4 0
EQI4 $2108
line 4687
;4684:																NULL, NULL,
;4685:																NULL, NULL,
;4686:																NULL, NULL,
;4687:																botname, netname)) {
line 4689
;4688:							//remove the console message
;4689:							trap_BotRemoveConsoleMessage(bs->cs, handle);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveConsoleMessage
CALLV
pop
line 4690
;4690:							bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 1004
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1004
INDIRP4
ARGP4
ADDRLP4 1008
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 1004
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 1008
INDIRF4
ADDF4
ASGNF4
line 4691
;4691:							AIEnter_Stand(bs, "BotCheckConsoleMessages: reply chat");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2110
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 4693
;4692:							//EA_Say(bs->client, bs->cs.chatmessage);
;4693:							break;
ADDRGP4 $2063
JUMPV
LABELV $2108
line 4695
;4694:						}
;4695:					}
LABELV $2106
line 4696
;4696:				}
LABELV $2104
LABELV $2096
line 4697
;4697:			}
LABELV $2083
line 4698
;4698:		}
LABELV $2080
line 4700
;4699:		//remove the console message
;4700:		trap_BotRemoveConsoleMessage(bs->cs, handle);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRI4
ARGI4
ADDRGP4 trap_BotRemoveConsoleMessage
CALLV
pop
line 4701
;4701:	}
LABELV $2062
line 4616
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 948
ADDRGP4 trap_BotNextConsoleMessage
CALLI4
ASGNI4
ADDRLP4 280
ADDRLP4 948
INDIRI4
ASGNI4
ADDRLP4 948
INDIRI4
CNSTI4 0
NEI4 $2061
LABELV $2063
line 4702
;4702:}
LABELV $2060
endproc BotCheckConsoleMessages 1012 48
export BotCheckForGrenades
proc BotCheckForGrenades 8 16
line 4709
;4703:
;4704:/*
;4705:==================
;4706:BotCheckEvents
;4707:==================
;4708:*/
;4709:void BotCheckForGrenades(bot_state_t *bs, entityState_t *state) {
line 4711
;4710:	// if this is not a grenade
;4711:	if (state->eType != ET_MISSILE || state->weapon != WP_GRENADE_LAUNCHER)
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRI4
CNSTI4 3
NEI4 $2114
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $2112
LABELV $2114
line 4712
;4712:		return;
ADDRGP4 $2111
JUMPV
LABELV $2112
line 4714
;4713:	// try to avoid the grenade
;4714:	trap_BotAddAvoidSpot(bs->ms, state->pos.trBase, 160, AVOID_ALWAYS);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTF4 1126170624
ARGF4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotAddAvoidSpot
CALLV
pop
line 4715
;4715:}
LABELV $2111
endproc BotCheckForGrenades 8 16
export BotCheckEvents
proc BotCheckEvents 164 12
line 4767
;4716:
;4717:#ifdef MISSIONPACK
;4718:/*
;4719:==================
;4720:BotCheckForProxMines
;4721:==================
;4722:*/
;4723:void BotCheckForProxMines(bot_state_t *bs, entityState_t *state) {
;4724:	// if this is not a prox mine
;4725:	if (state->eType != ET_MISSILE || state->weapon != WP_PROX_LAUNCHER)
;4726:		return;
;4727:	// if this prox mine is from someone on our own team
;4728:	if (state->generic1 == BotTeam(bs))
;4729:		return;
;4730:	// if the bot doesn't have a weapon to deactivate the mine
;4731:	if (!(bs->inventory[INVENTORY_PLASMAGUN] > 0 && bs->inventory[INVENTORY_CELLS] > 0) &&
;4732:		!(bs->inventory[INVENTORY_ROCKETLAUNCHER] > 0 && bs->inventory[INVENTORY_ROCKETS] > 0) &&
;4733:		!(bs->inventory[INVENTORY_BFG10K] > 0 && bs->inventory[INVENTORY_BFGAMMO] > 0) ) {
;4734:		return;
;4735:	}
;4736:	// try to avoid the prox mine
;4737:	trap_BotAddAvoidSpot(bs->ms, state->pos.trBase, 160, AVOID_ALWAYS);
;4738:	//
;4739:	if (bs->numproxmines >= MAX_PROXMINES)
;4740:		return;
;4741:	bs->proxmines[bs->numproxmines] = state->number;
;4742:	bs->numproxmines++;
;4743:}
;4744:
;4745:/*
;4746:==================
;4747:BotCheckForKamikazeBody
;4748:==================
;4749:*/
;4750:void BotCheckForKamikazeBody(bot_state_t *bs, entityState_t *state) {
;4751:	// if this entity is not wearing the kamikaze
;4752:	if (!(state->eFlags & EF_KAMIKAZE))
;4753:		return;
;4754:	// if this entity isn't dead
;4755:	if (!(state->eFlags & EF_DEAD))
;4756:		return;
;4757:	//remember this kamikaze body
;4758:	bs->kamikazebody = state->number;
;4759:}
;4760:#endif
;4761:
;4762:/*
;4763:==================
;4764:BotCheckEvents
;4765:==================
;4766:*/
;4767:void BotCheckEvents(bot_state_t *bs, entityState_t *state) {
line 4776
;4768:	int event;
;4769:	char buf[128];
;4770:#ifdef MISSIONPACK
;4771:	aas_entityinfo_t entinfo;
;4772:#endif
;4773:
;4774:	//NOTE: this sucks, we're accessing the gentity_t directly
;4775:	//but there's no other fast way to do it right now
;4776:	if (bs->entityeventTime[state->number] == g_entities[state->number].eventTime) {
ADDRLP4 132
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
ADDP4
INDIRI4
CNSTI4 808
ADDRLP4 132
INDIRI4
MULI4
ADDRGP4 g_entities+552
ADDP4
INDIRI4
NEI4 $2116
line 4777
;4777:		return;
ADDRGP4 $2115
JUMPV
LABELV $2116
line 4779
;4778:	}
;4779:	bs->entityeventTime[state->number] = g_entities[state->number].eventTime;
ADDRLP4 136
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
ADDP4
CNSTI4 808
ADDRLP4 136
INDIRI4
MULI4
ADDRGP4 g_entities+552
ADDP4
INDIRI4
ASGNI4
line 4781
;4780:	//if it's an event only entity
;4781:	if (state->eType > ET_EVENTS) {
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 13
LEI4 $2120
line 4782
;4782:		event = (state->eType - ET_EVENTS) & ~EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 13
SUBI4
CNSTI4 -769
BANDI4
ASGNI4
line 4783
;4783:	}
ADDRGP4 $2121
JUMPV
LABELV $2120
line 4784
;4784:	else {
line 4785
;4785:		event = state->event & ~EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CNSTI4 -769
BANDI4
ASGNI4
line 4786
;4786:	}
LABELV $2121
line 4788
;4787:	//
;4788:	switch(event) {
ADDRLP4 140
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 1
LTI4 $2122
ADDRLP4 140
INDIRI4
CNSTI4 76
GTI4 $2122
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $2171-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $2171
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2123
address $2122
address $2122
address $2122
address $2156
address $2122
address $2122
address $2157
address $2135
address $2143
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2124
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2122
address $2123
code
LABELV $2124
line 4791
;4789:		//client obituary event
;4790:		case EV_OBITUARY:
;4791:		{
line 4794
;4792:			int target, attacker, mod;
;4793:
;4794:			target = state->otherEntityNum;
ADDRLP4 144
ADDRFP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
ASGNI4
line 4795
;4795:			attacker = state->otherEntityNum2;
ADDRLP4 148
ADDRFP4 4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 4796
;4796:			mod = state->eventParm;
ADDRLP4 152
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
line 4798
;4797:			//
;4798:			if (target == bs->client) {
ADDRLP4 144
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $2125
line 4799
;4799:				bs->botdeathtype = mod;
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
ADDRLP4 152
INDIRI4
ASGNI4
line 4800
;4800:				bs->lastkilledby = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 5996
ADDP4
ADDRLP4 148
INDIRI4
ASGNI4
line 4802
;4801:				//
;4802:				if (target == attacker ||
ADDRLP4 156
ADDRLP4 144
INDIRI4
ASGNI4
ADDRLP4 156
INDIRI4
ADDRLP4 148
INDIRI4
EQI4 $2130
ADDRLP4 156
INDIRI4
CNSTI4 1023
EQI4 $2130
ADDRLP4 156
INDIRI4
CNSTI4 1022
NEI4 $2127
LABELV $2130
line 4804
;4803:					target == ENTITYNUM_NONE ||
;4804:					target == ENTITYNUM_WORLD) bs->botsuicide = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6008
ADDP4
CNSTI4 1
ASGNI4
ADDRGP4 $2128
JUMPV
LABELV $2127
line 4805
;4805:				else bs->botsuicide = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6008
ADDP4
CNSTI4 0
ASGNI4
LABELV $2128
line 4807
;4806:				//
;4807:				bs->num_deaths++;
ADDRLP4 160
ADDRFP4 0
INDIRP4
CNSTI4 6028
ADDP4
ASGNP4
ADDRLP4 160
INDIRP4
ADDRLP4 160
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4808
;4808:			}
ADDRGP4 $2123
JUMPV
LABELV $2125
line 4810
;4809:			//else if this client was killed by the bot
;4810:			else if (attacker == bs->client) {
ADDRLP4 148
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $2131
line 4811
;4811:				bs->enemydeathtype = mod;
ADDRFP4 0
INDIRP4
CNSTI4 6004
ADDP4
ADDRLP4 152
INDIRI4
ASGNI4
line 4812
;4812:				bs->lastkilledplayer = target;
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
ADDRLP4 144
INDIRI4
ASGNI4
line 4813
;4813:				bs->killedenemy_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6168
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4815
;4814:				//
;4815:				bs->num_kills++;
ADDRLP4 156
ADDRFP4 0
INDIRP4
CNSTI4 6032
ADDP4
ASGNP4
ADDRLP4 156
INDIRP4
ADDRLP4 156
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4816
;4816:			}
ADDRGP4 $2123
JUMPV
LABELV $2131
line 4817
;4817:			else if (attacker == bs->enemy && target == attacker) {
ADDRLP4 156
ADDRLP4 148
INDIRI4
ASGNI4
ADDRLP4 156
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
NEI4 $2123
ADDRLP4 144
INDIRI4
ADDRLP4 156
INDIRI4
NEI4 $2123
line 4818
;4818:				bs->enemysuicide = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6012
ADDP4
CNSTI4 1
ASGNI4
line 4819
;4819:			}
line 4833
;4820:			//
;4821:#ifdef MISSIONPACK			
;4822:			if (gametype == GT_1FCTF) {
;4823:				//
;4824:				BotEntityInfo(target, &entinfo);
;4825:				if ( entinfo.powerups & ( 1 << PW_NEUTRALFLAG ) ) {
;4826:					if (!BotSameTeam(bs, target)) {
;4827:						bs->neutralflagstatus = 3;	//enemy dropped the flag
;4828:						bs->flagstatuschanged = qtrue;
;4829:					}
;4830:				}
;4831:			}
;4832:#endif
;4833:			break;
ADDRGP4 $2123
JUMPV
LABELV $2135
line 4836
;4834:		}
;4835:		case EV_GLOBAL_SOUND:
;4836:		{
line 4837
;4837:			if (state->eventParm < 0 || state->eventParm > MAX_SOUNDS) {
ADDRLP4 144
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
LTI4 $2138
ADDRLP4 144
INDIRI4
CNSTI4 256
LEI4 $2136
LABELV $2138
line 4838
;4838:				BotAI_Print(PRT_ERROR, "EV_GLOBAL_SOUND: eventParm (%d) out of range\n", state->eventParm);
CNSTI4 3
ARGI4
ADDRGP4 $2139
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 4839
;4839:				break;
ADDRGP4 $2123
JUMPV
LABELV $2136
line 4841
;4840:			}
;4841:			trap_GetConfigstring(CS_SOUNDS + state->eventParm, buf, sizeof(buf));
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 288
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 4861
;4842:			/*
;4843:			if (!strcmp(buf, "sound/teamplay/flagret_red.wav")) {
;4844:				//red flag is returned
;4845:				bs->redflagstatus = 0;
;4846:				bs->flagstatuschanged = qtrue;
;4847:			}
;4848:			else if (!strcmp(buf, "sound/teamplay/flagret_blu.wav")) {
;4849:				//blue flag is returned
;4850:				bs->blueflagstatus = 0;
;4851:				bs->flagstatuschanged = qtrue;
;4852:			}
;4853:			else*/
;4854:#ifdef MISSIONPACK
;4855:			if (!strcmp(buf, "sound/items/kamikazerespawn.wav" )) {
;4856:				//the kamikaze respawned so dont avoid it
;4857:				BotDontAvoid(bs, "Kamikaze");
;4858:			}
;4859:			else
;4860:#endif
;4861:				if (!strcmp(buf, "sound/items/poweruprespawn.wav")) {
ADDRLP4 4
ARGP4
ADDRGP4 $2142
ARGP4
ADDRLP4 148
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $2123
line 4863
;4862:				//powerup respawned... go get it
;4863:				BotGoForPowerups(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotGoForPowerups
CALLV
pop
line 4864
;4864:			}
line 4865
;4865:			break;
ADDRGP4 $2123
JUMPV
LABELV $2143
line 4868
;4866:		}
;4867:		case EV_GLOBAL_TEAM_SOUND:
;4868:		{
line 4869
;4869:			if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $2123
line 4870
;4870:				switch(state->eventParm) {
ADDRLP4 144
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
LTI4 $2123
ADDRLP4 144
INDIRI4
CNSTI4 5
GTI4 $2123
ADDRLP4 144
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $2155
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $2155
address $2149
address $2150
address $2151
address $2152
address $2153
address $2154
code
LABELV $2149
line 4872
;4871:					case GTS_RED_CAPTURE:
;4872:						bs->blueflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 0
ASGNI4
line 4873
;4873:						bs->redflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 0
ASGNI4
line 4874
;4874:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4875
;4875:						break; //see BotMatch_CTF
ADDRGP4 $2123
JUMPV
LABELV $2150
line 4877
;4876:					case GTS_BLUE_CAPTURE:
;4877:						bs->blueflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 0
ASGNI4
line 4878
;4878:						bs->redflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 0
ASGNI4
line 4879
;4879:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4880
;4880:						break; //see BotMatch_CTF
ADDRGP4 $2123
JUMPV
LABELV $2151
line 4883
;4881:					case GTS_RED_RETURN:
;4882:						//blue flag is returned
;4883:						bs->blueflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 0
ASGNI4
line 4884
;4884:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4885
;4885:						break;
ADDRGP4 $2123
JUMPV
LABELV $2152
line 4888
;4886:					case GTS_BLUE_RETURN:
;4887:						//red flag is returned
;4888:						bs->redflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 0
ASGNI4
line 4889
;4889:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4890
;4890:						break;
ADDRGP4 $2123
JUMPV
LABELV $2153
line 4893
;4891:					case GTS_RED_TAKEN:
;4892:						//blue flag is taken
;4893:						bs->blueflagstatus = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 1
ASGNI4
line 4894
;4894:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4895
;4895:						break; //see BotMatch_CTF
ADDRGP4 $2123
JUMPV
LABELV $2154
line 4898
;4896:					case GTS_BLUE_TAKEN:
;4897:						//red flag is taken
;4898:						bs->redflagstatus = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 1
ASGNI4
line 4899
;4899:						bs->flagstatuschanged = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 4900
;4900:						break; //see BotMatch_CTF
line 4902
;4901:				}
;4902:			}
line 4935
;4903:#ifdef MISSIONPACK
;4904:			else if (gametype == GT_1FCTF) {
;4905:				switch(state->eventParm) {
;4906:					case GTS_RED_CAPTURE:
;4907:						bs->neutralflagstatus = 0;
;4908:						bs->flagstatuschanged = qtrue;
;4909:						break;
;4910:					case GTS_BLUE_CAPTURE:
;4911:						bs->neutralflagstatus = 0;
;4912:						bs->flagstatuschanged = qtrue;
;4913:						break;
;4914:					case GTS_RED_RETURN:
;4915:						//flag has returned
;4916:						bs->neutralflagstatus = 0;
;4917:						bs->flagstatuschanged = qtrue;
;4918:						break;
;4919:					case GTS_BLUE_RETURN:
;4920:						//flag has returned
;4921:						bs->neutralflagstatus = 0;
;4922:						bs->flagstatuschanged = qtrue;
;4923:						break;
;4924:					case GTS_RED_TAKEN:
;4925:						bs->neutralflagstatus = BotTeam(bs) == TEAM_RED ? 2 : 1; //FIXME: check Team_TakeFlagSound in g_team.c
;4926:						bs->flagstatuschanged = qtrue;
;4927:						break;
;4928:					case GTS_BLUE_TAKEN:
;4929:						bs->neutralflagstatus = BotTeam(bs) == TEAM_BLUE ? 2 : 1; //FIXME: check Team_TakeFlagSound in g_team.c
;4930:						bs->flagstatuschanged = qtrue;
;4931:						break;
;4932:				}
;4933:			}
;4934:#endif
;4935:			break;
ADDRGP4 $2123
JUMPV
LABELV $2156
line 4938
;4936:		}
;4937:		case EV_PLAYER_TELEPORT_IN:
;4938:		{
line 4939
;4939:			VectorCopy(state->origin, lastteleport_origin);
ADDRGP4 lastteleport_origin
ADDRFP4 4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 4940
;4940:			lastteleport_time = FloatTime();
ADDRGP4 lastteleport_time
ADDRGP4 floattime
INDIRF4
ASGNF4
line 4941
;4941:			break;
ADDRGP4 $2123
JUMPV
LABELV $2157
line 4944
;4942:		}
;4943:		case EV_GENERAL_SOUND:
;4944:		{
line 4946
;4945:			//if this sound is played on the bot
;4946:			if (state->number == bs->client) {
ADDRFP4 4
INDIRP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $2123
line 4947
;4947:				if (state->eventParm < 0 || state->eventParm > MAX_SOUNDS) {
ADDRLP4 144
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
LTI4 $2162
ADDRLP4 144
INDIRI4
CNSTI4 256
LEI4 $2160
LABELV $2162
line 4948
;4948:					BotAI_Print(PRT_ERROR, "EV_GENERAL_SOUND: eventParm (%d) out of range\n", state->eventParm);
CNSTI4 3
ARGI4
ADDRGP4 $2163
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 4949
;4949:					break;
ADDRGP4 $2123
JUMPV
LABELV $2160
line 4952
;4950:				}
;4951:				//check out the sound
;4952:				trap_GetConfigstring(CS_SOUNDS + state->eventParm, buf, sizeof(buf));
ADDRFP4 4
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 288
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 4954
;4953:				//if falling into a death pit
;4954:				if (!strcmp(buf, "*falling1.wav")) {
ADDRLP4 4
ARGP4
ADDRGP4 $2166
ARGP4
ADDRLP4 148
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $2123
line 4956
;4955:					//if the bot has a personal teleporter
;4956:					if (bs->inventory[INVENTORY_TELEPORTER] > 0) {
ADDRFP4 0
INDIRP4
CNSTI4 5072
ADDP4
INDIRI4
CNSTI4 0
LEI4 $2123
line 4958
;4957:						//use the holdable item
;4958:						trap_EA_Use(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Use
CALLV
pop
line 4959
;4959:					}
line 4960
;4960:				}
line 4961
;4961:			}
line 4962
;4962:			break;
line 4989
;4963:		}
;4964:		case EV_FOOTSTEP:
;4965:		case EV_FOOTSTEP_METAL:
;4966:		case EV_FOOTSPLASH:
;4967:		case EV_FOOTWADE:
;4968:		case EV_SWIM:
;4969:		case EV_FALL_SHORT:
;4970:		case EV_FALL_MEDIUM:
;4971:		case EV_FALL_FAR:
;4972:		case EV_STEP_4:
;4973:		case EV_STEP_8:
;4974:		case EV_STEP_12:
;4975:		case EV_STEP_16:
;4976:		case EV_JUMP_PAD:
;4977:		case EV_JUMP:
;4978:		case EV_TAUNT:
;4979:		case EV_WATER_TOUCH:
;4980:		case EV_WATER_LEAVE:
;4981:		case EV_WATER_UNDER:
;4982:		case EV_WATER_CLEAR:
;4983:		case EV_ITEM_PICKUP:
;4984:		case EV_GLOBAL_ITEM_PICKUP:
;4985:		case EV_NOAMMO:
;4986:		case EV_CHANGE_WEAPON:
;4987:		case EV_FIRE_WEAPON:
;4988:			//FIXME: either add to sound queue or mark player as someone making noise
;4989:			break;
line 5005
;4990:		case EV_USE_ITEM0:
;4991:		case EV_USE_ITEM1:
;4992:		case EV_USE_ITEM2:
;4993:		case EV_USE_ITEM3:
;4994:		case EV_USE_ITEM4:
;4995:		case EV_USE_ITEM5:
;4996:		case EV_USE_ITEM6:
;4997:		case EV_USE_ITEM7:
;4998:		case EV_USE_ITEM8:
;4999:		case EV_USE_ITEM9:
;5000:		case EV_USE_ITEM10:
;5001:		case EV_USE_ITEM11:
;5002:		case EV_USE_ITEM12:
;5003:		case EV_USE_ITEM13:
;5004:		case EV_USE_ITEM14:
;5005:			break;
LABELV $2122
LABELV $2123
line 5007
;5006:	}
;5007:}
LABELV $2115
endproc BotCheckEvents 164 12
export BotCheckSnapshot
proc BotCheckSnapshot 216 16
line 5014
;5008:
;5009:/*
;5010:==================
;5011:BotCheckSnapshot
;5012:==================
;5013:*/
;5014:void BotCheckSnapshot(bot_state_t *bs) {
line 5019
;5015:	int ent;
;5016:	entityState_t state;
;5017:
;5018:	//remove all avoid spots
;5019:	trap_BotAddAvoidSpot(bs->ms, vec3_origin, 0, AVOID_CLEAR);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 vec3_origin
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 trap_BotAddAvoidSpot
CALLV
pop
line 5021
;5020:	//reset kamikaze body
;5021:	bs->kamikazebody = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6256
ADDP4
CNSTI4 0
ASGNI4
line 5023
;5022:	//reset number of proxmines
;5023:	bs->numproxmines = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6516
ADDP4
CNSTI4 0
ASGNI4
line 5025
;5024:	//
;5025:	ent = 0;
ADDRLP4 208
CNSTI4 0
ASGNI4
ADDRGP4 $2175
JUMPV
LABELV $2174
line 5026
;5026:	while( ( ent = BotAI_GetSnapshotEntity( bs->client, ent, &state ) ) != -1 ) {
line 5028
;5027:		//check the entity state for events
;5028:		BotCheckEvents(bs, &state);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotCheckEvents
CALLV
pop
line 5030
;5029:		//check for grenades the bot should avoid
;5030:		BotCheckForGrenades(bs, &state);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotCheckForGrenades
CALLV
pop
line 5038
;5031:		//
;5032:#ifdef MISSIONPACK
;5033:		//check for proximity mines which the bot should deactivate
;5034:		BotCheckForProxMines(bs, &state);
;5035:		//check for dead bodies with the kamikaze effect which should be gibbed
;5036:		BotCheckForKamikazeBody(bs, &state);
;5037:#endif
;5038:	}
LABELV $2175
line 5026
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 208
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 212
ADDRGP4 BotAI_GetSnapshotEntity
CALLI4
ASGNI4
ADDRLP4 208
ADDRLP4 212
INDIRI4
ASGNI4
ADDRLP4 212
INDIRI4
CNSTI4 -1
NEI4 $2174
line 5040
;5039:	//check the player state for events
;5040:	BotAI_GetEntityState(bs->client, &state);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotAI_GetEntityState
CALLI4
pop
line 5042
;5041:	//copy the player state events to the entity state
;5042:	state.event = bs->cur_ps.externalEvent;
ADDRLP4 0+180
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 5043
;5043:	state.eventParm = bs->cur_ps.externalEventParm;
ADDRLP4 0+184
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ASGNI4
line 5045
;5044:	//
;5045:	BotCheckEvents(bs, &state);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotCheckEvents
CALLV
pop
line 5046
;5046:}
LABELV $2173
endproc BotCheckSnapshot 216 16
export BotCheckAir
proc BotCheckAir 4 4
line 5053
;5047:
;5048:/*
;5049:==================
;5050:BotCheckAir
;5051:==================
;5052:*/
;5053:void BotCheckAir(bot_state_t *bs) {
line 5054
;5054:	if (bs->inventory[INVENTORY_ENVIRONMENTSUIT] <= 0) {
ADDRFP4 0
INDIRP4
CNSTI4 5096
ADDP4
INDIRI4
CNSTI4 0
GTI4 $2180
line 5055
;5055:		if (trap_AAS_PointContents(bs->eye) & (CONTENTS_WATER|CONTENTS_SLIME|CONTENTS_LAVA)) {
ADDRFP4 0
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $2182
line 5056
;5056:			return;
ADDRGP4 $2179
JUMPV
LABELV $2182
line 5058
;5057:		}
;5058:	}
LABELV $2180
line 5059
;5059:	bs->lastair_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6176
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 5060
;5060:}
LABELV $2179
endproc BotCheckAir 4 4
export BotAlternateRoute
proc BotAlternateRoute 16 16
line 5067
;5061:
;5062:/*
;5063:==================
;5064:BotAlternateRoute
;5065:==================
;5066:*/
;5067:bot_goal_t *BotAlternateRoute(bot_state_t *bs, bot_goal_t *goal) {
line 5071
;5068:	int t;
;5069:
;5070:	// if the bot has an alternative route goal
;5071:	if (bs->altroutegoal.areanum) {
ADDRFP4 0
INDIRP4
CNSTI4 6692
ADDP4
INDIRI4
CNSTI4 0
EQI4 $2185
line 5073
;5072:		//
;5073:		if (bs->reachedaltroutegoal_time)
ADDRFP4 0
INDIRP4
CNSTI4 6736
ADDP4
INDIRF4
CNSTF4 0
EQF4 $2187
line 5074
;5074:			return goal;
ADDRFP4 4
INDIRP4
RETP4
ADDRGP4 $2184
JUMPV
LABELV $2187
line 5076
;5075:		// travel time towards alternative route goal
;5076:		t = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, bs->altroutegoal.areanum, bs->tfl);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 6692
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 5077
;5077:		if (t && t < 20) {
ADDRLP4 12
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2189
ADDRLP4 12
INDIRI4
CNSTI4 20
GEI4 $2189
line 5079
;5078:			//BotAI_Print(PRT_MESSAGE, "reached alternate route goal\n");
;5079:			bs->reachedaltroutegoal_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6736
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 5080
;5080:		}
LABELV $2189
line 5081
;5081:		memcpy(goal, &bs->altroutegoal, sizeof(bot_goal_t));
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6680
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 5082
;5082:		return &bs->altroutegoal;
ADDRFP4 0
INDIRP4
CNSTI4 6680
ADDP4
RETP4
ADDRGP4 $2184
JUMPV
LABELV $2185
line 5084
;5083:	}
;5084:	return goal;
ADDRFP4 4
INDIRP4
RETP4
LABELV $2184
endproc BotAlternateRoute 16 16
export BotGetAlternateRouteGoal
proc BotGetAlternateRouteGoal 24 0
line 5092
;5085:}
;5086:
;5087:/*
;5088:==================
;5089:BotGetAlternateRouteGoal
;5090:==================
;5091:*/
;5092:int BotGetAlternateRouteGoal(bot_state_t *bs, int base) {
line 5097
;5093:	aas_altroutegoal_t *altroutegoals;
;5094:	bot_goal_t *goal;
;5095:	int numaltroutegoals, rnd;
;5096:
;5097:	if (base == TEAM_RED) {
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $2192
line 5098
;5098:		altroutegoals = red_altroutegoals;
ADDRLP4 12
ADDRGP4 red_altroutegoals
ASGNP4
line 5099
;5099:		numaltroutegoals = red_numaltroutegoals;
ADDRLP4 4
ADDRGP4 red_numaltroutegoals
INDIRI4
ASGNI4
line 5100
;5100:	}
ADDRGP4 $2193
JUMPV
LABELV $2192
line 5101
;5101:	else {
line 5102
;5102:		altroutegoals = blue_altroutegoals;
ADDRLP4 12
ADDRGP4 blue_altroutegoals
ASGNP4
line 5103
;5103:		numaltroutegoals = blue_numaltroutegoals;
ADDRLP4 4
ADDRGP4 blue_numaltroutegoals
INDIRI4
ASGNI4
line 5104
;5104:	}
LABELV $2193
line 5105
;5105:	if (!numaltroutegoals)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2194
line 5106
;5106:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2191
JUMPV
LABELV $2194
line 5107
;5107:	rnd = (float) random() * numaltroutegoals;
ADDRLP4 16
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 5108
;5108:	if (rnd >= numaltroutegoals)
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $2196
line 5109
;5109:		rnd = numaltroutegoals-1;
ADDRLP4 8
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $2196
line 5110
;5110:	goal = &bs->altroutegoal;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6680
ADDP4
ASGNP4
line 5111
;5111:	goal->areanum = altroutegoals[rnd].areanum;
ADDRLP4 20
CNSTI4 12
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
CNSTI4 24
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 12
INDIRP4
ADDP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 5112
;5112:	VectorCopy(altroutegoals[rnd].origin, goal->origin);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 12
INDIRP4
ADDP4
INDIRB
ASGNB 12
line 5113
;5113:	VectorSet(goal->mins, -8, -8, -8);
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
CNSTF4 3238002688
ASGNF4
line 5114
;5114:	VectorSet(goal->maxs, 8, 8, 8);
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1090519040
ASGNF4
line 5115
;5115:	goal->entitynum = 0;
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTI4 0
ASGNI4
line 5116
;5116:	goal->iteminfo = 0;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 5117
;5117:	goal->number = 0;
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 5118
;5118:	goal->flags = 0;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 5120
;5119:	//
;5120:	bs->reachedaltroutegoal_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6736
ADDP4
CNSTF4 0
ASGNF4
line 5121
;5121:	return qtrue;
CNSTI4 1
RETI4
LABELV $2191
endproc BotGetAlternateRouteGoal 24 0
export BotSetupAlternativeRouteGoals
proc BotSetupAlternativeRouteGoals 0 0
line 5129
;5122:}
;5123:
;5124:/*
;5125:==================
;5126:BotSetupAlternateRouteGoals
;5127:==================
;5128:*/
;5129:void BotSetupAlternativeRouteGoals(void) {
line 5131
;5130:
;5131:	if (altroutegoals_setup)
ADDRGP4 altroutegoals_setup
INDIRI4
CNSTI4 0
EQI4 $2199
line 5132
;5132:		return;
ADDRGP4 $2198
JUMPV
LABELV $2199
line 5201
;5133:#ifdef MISSIONPACK
;5134:	if (gametype == GT_CTF) {
;5135:		if (trap_BotGetLevelItemGoal(-1, "Neutral Flag", &ctf_neutralflag) < 0)
;5136:			BotAI_Print(PRT_WARNING, "no alt routes without Neutral Flag\n");
;5137:		if (ctf_neutralflag.areanum) {
;5138:			//
;5139:			red_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5140:										ctf_neutralflag.origin, ctf_neutralflag.areanum,
;5141:										ctf_redflag.origin, ctf_redflag.areanum, TFL_DEFAULT,
;5142:										red_altroutegoals, MAX_ALTROUTEGOALS,
;5143:										ALTROUTEGOAL_CLUSTERPORTALS|
;5144:										ALTROUTEGOAL_VIEWPORTALS);
;5145:			blue_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5146:										ctf_neutralflag.origin, ctf_neutralflag.areanum,
;5147:										ctf_blueflag.origin, ctf_blueflag.areanum, TFL_DEFAULT,
;5148:										blue_altroutegoals, MAX_ALTROUTEGOALS,
;5149:										ALTROUTEGOAL_CLUSTERPORTALS|
;5150:										ALTROUTEGOAL_VIEWPORTALS);
;5151:		}
;5152:	}
;5153:	else if (gametype == GT_1FCTF) {
;5154:		//
;5155:		red_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5156:									ctf_neutralflag.origin, ctf_neutralflag.areanum,
;5157:									ctf_redflag.origin, ctf_redflag.areanum, TFL_DEFAULT,
;5158:									red_altroutegoals, MAX_ALTROUTEGOALS,
;5159:									ALTROUTEGOAL_CLUSTERPORTALS|
;5160:									ALTROUTEGOAL_VIEWPORTALS);
;5161:		blue_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5162:									ctf_neutralflag.origin, ctf_neutralflag.areanum,
;5163:									ctf_blueflag.origin, ctf_blueflag.areanum, TFL_DEFAULT,
;5164:									blue_altroutegoals, MAX_ALTROUTEGOALS,
;5165:									ALTROUTEGOAL_CLUSTERPORTALS|
;5166:									ALTROUTEGOAL_VIEWPORTALS);
;5167:	}
;5168:	else if (gametype == GT_OBELISK) {
;5169:		if (trap_BotGetLevelItemGoal(-1, "Neutral Obelisk", &neutralobelisk) < 0)
;5170:			BotAI_Print(PRT_WARNING, "Harvester without neutral obelisk\n");
;5171:		//
;5172:		red_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5173:									neutralobelisk.origin, neutralobelisk.areanum,
;5174:									redobelisk.origin, redobelisk.areanum, TFL_DEFAULT,
;5175:									red_altroutegoals, MAX_ALTROUTEGOALS,
;5176:									ALTROUTEGOAL_CLUSTERPORTALS|
;5177:									ALTROUTEGOAL_VIEWPORTALS);
;5178:		blue_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5179:									neutralobelisk.origin, neutralobelisk.areanum,
;5180:									blueobelisk.origin, blueobelisk.areanum, TFL_DEFAULT,
;5181:									blue_altroutegoals, MAX_ALTROUTEGOALS,
;5182:									ALTROUTEGOAL_CLUSTERPORTALS|
;5183:									ALTROUTEGOAL_VIEWPORTALS);
;5184:	}
;5185:	else if (gametype == GT_HARVESTER) {
;5186:		//
;5187:		red_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5188:									neutralobelisk.origin, neutralobelisk.areanum,
;5189:									redobelisk.origin, redobelisk.areanum, TFL_DEFAULT,
;5190:									red_altroutegoals, MAX_ALTROUTEGOALS,
;5191:									ALTROUTEGOAL_CLUSTERPORTALS|
;5192:									ALTROUTEGOAL_VIEWPORTALS);
;5193:		blue_numaltroutegoals = trap_AAS_AlternativeRouteGoals(
;5194:									neutralobelisk.origin, neutralobelisk.areanum,
;5195:									blueobelisk.origin, blueobelisk.areanum, TFL_DEFAULT,
;5196:									blue_altroutegoals, MAX_ALTROUTEGOALS,
;5197:									ALTROUTEGOAL_CLUSTERPORTALS|
;5198:									ALTROUTEGOAL_VIEWPORTALS);
;5199:	}
;5200:#endif
;5201:	altroutegoals_setup = qtrue;
ADDRGP4 altroutegoals_setup
CNSTI4 1
ASGNI4
line 5202
;5202:}
LABELV $2198
endproc BotSetupAlternativeRouteGoals 0 0
export BotDeathmatchAI
proc BotDeathmatchAI 1492 20
line 5209
;5203:
;5204:/*
;5205:==================
;5206:BotDeathmatchAI
;5207:==================
;5208:*/
;5209:void BotDeathmatchAI(bot_state_t *bs, float thinktime) {
line 5215
;5210:	char gender[144], name[144], buf[144];
;5211:	char userinfo[MAX_INFO_STRING];
;5212:	int i;
;5213:
;5214:	//if the bot has just been setup
;5215:	if (bs->setupcount > 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6016
ADDP4
INDIRI4
CNSTI4 0
LEI4 $2202
line 5216
;5216:		bs->setupcount--;
ADDRLP4 1460
ADDRFP4 0
INDIRP4
CNSTI4 6016
ADDP4
ASGNP4
ADDRLP4 1460
INDIRP4
ADDRLP4 1460
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 5217
;5217:		if (bs->setupcount > 0) return;
ADDRFP4 0
INDIRP4
CNSTI4 6016
ADDP4
INDIRI4
CNSTI4 0
LEI4 $2204
ADDRGP4 $2201
JUMPV
LABELV $2204
line 5219
;5218:		//get the gender characteristic
;5219:		trap_Characteristic_String(bs->character, CHARACTERISTIC_GENDER, gender, sizeof(gender));
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 148
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_Characteristic_String
CALLV
pop
line 5221
;5220:		//set the bot gender
;5221:		trap_GetUserinfo(bs->client, userinfo, sizeof(userinfo));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 5222
;5222:		Info_SetValueForKey(userinfo, "sex", gender);
ADDRLP4 292
ARGP4
ADDRGP4 $2206
ARGP4
ADDRLP4 148
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 5223
;5223:		trap_SetUserinfo(bs->client, userinfo);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 5225
;5224:		//set the team
;5225:		if ( !bs->map_restart && g_gametype.integer != GT_TOURNAMENT ) {
ADDRFP4 0
INDIRP4
CNSTI4 6020
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2207
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
EQI4 $2207
line 5226
;5226:			Com_sprintf(buf, sizeof(buf), "team %s", bs->settings.team);
ADDRLP4 1316
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 $2210
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 4756
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 5227
;5227:			trap_EA_Command(bs->client, buf);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 1316
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 5228
;5228:		}
LABELV $2207
line 5230
;5229:		//set the chat gender
;5230:		if (gender[0] == 'm') trap_BotSetChatGender(bs->cs, CHAT_GENDERMALE);
ADDRLP4 148
INDIRI1
CVII4 1
CNSTI4 109
NEI4 $2211
ADDRFP4 0
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
ADDRGP4 $2212
JUMPV
LABELV $2211
line 5231
;5231:		else if (gender[0] == 'f')  trap_BotSetChatGender(bs->cs, CHAT_GENDERFEMALE);
ADDRLP4 148
INDIRI1
CVII4 1
CNSTI4 102
NEI4 $2213
ADDRFP4 0
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
ADDRGP4 $2214
JUMPV
LABELV $2213
line 5232
;5232:		else  trap_BotSetChatGender(bs->cs, CHAT_GENDERLESS);
ADDRFP4 0
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
LABELV $2214
LABELV $2212
line 5234
;5233:		//set the chat name
;5234:		ClientName(bs->client, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 5235
;5235:		trap_BotSetChatName(bs->cs, name, bs->client);
ADDRLP4 1464
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1464
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 1464
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotSetChatName
CALLV
pop
line 5237
;5236:		//
;5237:		bs->lastframe_health = bs->inventory[INVENTORY_HEALTH];
ADDRLP4 1468
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1468
INDIRP4
CNSTI4 6044
ADDP4
ADDRLP4 1468
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
ASGNI4
line 5238
;5238:		bs->lasthitcount = bs->cur_ps.persistant[PERS_HITS];
ADDRLP4 1472
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1472
INDIRP4
CNSTI4 6048
ADDP4
ADDRLP4 1472
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ASGNI4
line 5240
;5239:		//
;5240:		bs->setupcount = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6016
ADDP4
CNSTI4 0
ASGNI4
line 5242
;5241:		//
;5242:		BotSetupAlternativeRouteGoals();
ADDRGP4 BotSetupAlternativeRouteGoals
CALLV
pop
line 5243
;5243:	}
LABELV $2202
line 5245
;5244:	//no ideal view set
;5245:	bs->flags &= ~BFL_IDEALVIEWSET;
ADDRLP4 1460
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 1460
INDIRP4
ADDRLP4 1460
INDIRP4
INDIRI4
CNSTI4 -33
BANDI4
ASGNI4
line 5247
;5246:	//
;5247:	if (!BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1464
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 1464
INDIRI4
CNSTI4 0
NEI4 $2215
line 5249
;5248:		//set the teleport time
;5249:		BotSetTeleportTime(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeleportTime
CALLV
pop
line 5251
;5250:		//update some inventory values
;5251:		BotUpdateInventory(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotUpdateInventory
CALLV
pop
line 5253
;5252:		//check out the snapshot
;5253:		BotCheckSnapshot(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckSnapshot
CALLV
pop
line 5255
;5254:		//check for air
;5255:		BotCheckAir(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckAir
CALLV
pop
line 5256
;5256:	}
LABELV $2215
line 5258
;5257:	//check the console messages
;5258:	BotCheckConsoleMessages(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckConsoleMessages
CALLV
pop
line 5260
;5259:	//if not in the intermission and not in observer mode
;5260:	if (!BotIntermission(bs) && !BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1468
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 1468
INDIRI4
CNSTI4 0
NEI4 $2217
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1472
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 1472
INDIRI4
CNSTI4 0
NEI4 $2217
line 5262
;5261:		//do team AI
;5262:		BotTeamAI(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotTeamAI
CALLV
pop
line 5263
;5263:	}
LABELV $2217
line 5265
;5264:	//if the bot has no ai node
;5265:	if (!bs->ainode) {
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2219
line 5266
;5266:		AIEnter_Seek_LTG(bs, "BotDeathmatchAI: no ai node");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2221
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 5267
;5267:	}
LABELV $2219
line 5269
;5268:	//if the bot entered the game less than 8 seconds ago
;5269:	if (!bs->entergamechat && bs->entergame_time > FloatTime() - 8) {
ADDRLP4 1476
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1476
INDIRP4
CNSTI4 6024
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2222
ADDRLP4 1476
INDIRP4
CNSTI4 6064
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1090519040
SUBF4
LEF4 $2222
line 5270
;5270:		if (BotChat_EnterGame(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1480
ADDRGP4 BotChat_EnterGame
CALLI4
ASGNI4
ADDRLP4 1480
INDIRI4
CNSTI4 0
EQI4 $2224
line 5271
;5271:			bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 1484
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1484
INDIRP4
ARGP4
ADDRLP4 1488
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 1484
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 1488
INDIRF4
ADDF4
ASGNF4
line 5272
;5272:			AIEnter_Stand(bs, "BotDeathmatchAI: chat enter game");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2226
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 5273
;5273:		}
LABELV $2224
line 5274
;5274:		bs->entergamechat = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6024
ADDP4
CNSTI4 1
ASGNI4
line 5275
;5275:	}
LABELV $2222
line 5277
;5276:	//reset the node switches from the previous frame
;5277:	BotResetNodeSwitches();
ADDRGP4 BotResetNodeSwitches
CALLV
pop
line 5279
;5278:	//execute AI nodes
;5279:	for (i = 0; i < MAX_NODESWITCHES; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2227
line 5280
;5280:		if (bs->ainode(bs)) break;
ADDRLP4 1480
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1480
INDIRP4
ARGP4
ADDRLP4 1484
ADDRLP4 1480
INDIRP4
CNSTI4 4900
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1484
INDIRI4
CNSTI4 0
EQI4 $2231
ADDRGP4 $2229
JUMPV
LABELV $2231
line 5281
;5281:	}
LABELV $2228
line 5279
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 50
LTI4 $2227
LABELV $2229
line 5283
;5282:	//if the bot removed itself :)
;5283:	if (!bs->inuse) return;
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $2233
ADDRGP4 $2201
JUMPV
LABELV $2233
line 5285
;5284:	//if the bot executed too many AI nodes
;5285:	if (i >= MAX_NODESWITCHES) {
ADDRLP4 0
INDIRI4
CNSTI4 50
LTI4 $2235
line 5286
;5286:		trap_BotDumpGoalStack(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotDumpGoalStack
CALLV
pop
line 5287
;5287:		trap_BotDumpAvoidGoals(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotDumpAvoidGoals
CALLV
pop
line 5288
;5288:		BotDumpNodeSwitches(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotDumpNodeSwitches
CALLV
pop
line 5289
;5289:		ClientName(bs->client, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 5290
;5290:		BotAI_Print(PRT_ERROR, "%s at %1.1f switched more than %d AI nodes\n", name, FloatTime(), MAX_NODESWITCHES);
CNSTI4 3
ARGI4
ADDRGP4 $2237
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 floattime
INDIRF4
ARGF4
CNSTI4 50
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 5291
;5291:	}
LABELV $2235
line 5293
;5292:	//
;5293:	bs->lastframe_health = bs->inventory[INVENTORY_HEALTH];
ADDRLP4 1480
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1480
INDIRP4
CNSTI4 6044
ADDP4
ADDRLP4 1480
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
ASGNI4
line 5294
;5294:	bs->lasthitcount = bs->cur_ps.persistant[PERS_HITS];
ADDRLP4 1484
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1484
INDIRP4
CNSTI4 6048
ADDP4
ADDRLP4 1484
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ASGNI4
line 5295
;5295:}
LABELV $2201
endproc BotDeathmatchAI 1492 20
export BotSetEntityNumForGoalWithModel
proc BotSetEntityNumForGoalWithModel 44 4
line 5302
;5296:
;5297:/*
;5298:==================
;5299:BotSetEntityNumForGoalWithModel
;5300:==================
;5301:*/
;5302:void BotSetEntityNumForGoalWithModel(bot_goal_t *goal, int eType, char *modelname) {
line 5307
;5303:	gentity_t *ent;
;5304:	int i, modelindex;
;5305:	vec3_t dir;
;5306:
;5307:	modelindex = G_ModelIndex( modelname );
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 20
ADDRLP4 24
INDIRI4
ASGNI4
line 5308
;5308:	ent = &g_entities[0];
ADDRLP4 0
ADDRGP4 g_entities
ASGNP4
line 5309
;5309:	for (i = 0; i < level.num_entities; i++, ent++) {
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 $2242
JUMPV
LABELV $2239
line 5310
;5310:		if ( !ent->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2244
line 5311
;5311:			continue;
ADDRGP4 $2240
JUMPV
LABELV $2244
line 5313
;5312:		}
;5313:		if ( eType && ent->s.eType != eType) {
ADDRLP4 28
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $2246
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
EQI4 $2246
line 5314
;5314:			continue;
ADDRGP4 $2240
JUMPV
LABELV $2246
line 5316
;5315:		}
;5316:		if (ent->s.modelindex != modelindex) {
ADDRLP4 0
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $2248
line 5317
;5317:			continue;
ADDRGP4 $2240
JUMPV
LABELV $2248
line 5319
;5318:		}
;5319:		VectorSubtract(goal->origin, ent->s.origin, dir);
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 5320
;5320:		if (VectorLengthSquared(dir) < Square(10)) {
ADDRLP4 4
ARGP4
ADDRLP4 40
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 40
INDIRF4
CNSTF4 1120403456
GEF4 $2252
line 5321
;5321:			goal->entitynum = i;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 5322
;5322:			return;
ADDRGP4 $2238
JUMPV
LABELV $2252
line 5324
;5323:		}
;5324:	}
LABELV $2240
line 5309
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $2242
ADDRLP4 16
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $2239
line 5325
;5325:}
LABELV $2238
endproc BotSetEntityNumForGoalWithModel 44 4
export BotSetEntityNumForGoal
proc BotSetEntityNumForGoal 36 8
line 5332
;5326:
;5327:/*
;5328:==================
;5329:BotSetEntityNumForGoal
;5330:==================
;5331:*/
;5332:void BotSetEntityNumForGoal(bot_goal_t *goal, char *classname) {
line 5337
;5333:	gentity_t *ent;
;5334:	int i;
;5335:	vec3_t dir;
;5336:
;5337:	ent = &g_entities[0];
ADDRLP4 0
ADDRGP4 g_entities
ASGNP4
line 5338
;5338:	for (i = 0; i < level.num_entities; i++, ent++) {
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 $2258
JUMPV
LABELV $2255
line 5339
;5339:		if ( !ent->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2260
line 5340
;5340:			continue;
ADDRGP4 $2256
JUMPV
LABELV $2260
line 5342
;5341:		}
;5342:		if ( !Q_stricmp(ent->classname, classname) ) {
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $2262
line 5343
;5343:			continue;
ADDRGP4 $2256
JUMPV
LABELV $2262
line 5345
;5344:		}
;5345:		VectorSubtract(goal->origin, ent->s.origin, dir);
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 24
INDIRP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 24
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 5346
;5346:		if (VectorLengthSquared(dir) < Square(10)) {
ADDRLP4 4
ARGP4
ADDRLP4 32
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 32
INDIRF4
CNSTF4 1120403456
GEF4 $2266
line 5347
;5347:			goal->entitynum = i;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 5348
;5348:			return;
ADDRGP4 $2254
JUMPV
LABELV $2266
line 5350
;5349:		}
;5350:	}
LABELV $2256
line 5338
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $2258
ADDRLP4 16
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $2255
line 5351
;5351:}
LABELV $2254
endproc BotSetEntityNumForGoal 36 8
export BotGoalForBSPEntity
proc BotGoalForBSPEntity 1128 20
line 5358
;5352:
;5353:/*
;5354:==================
;5355:BotGoalForBSPEntity
;5356:==================
;5357:*/
;5358:int BotGoalForBSPEntity( char *classname, bot_goal_t *goal ) {
line 5363
;5359:	char value[MAX_INFO_STRING];
;5360:	vec3_t origin, start, end;
;5361:	int ent, numareas, areas[10];
;5362:
;5363:	memset(goal, 0, sizeof(bot_goal_t));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 memset
CALLP4
pop
line 5364
;5364:	for (ent = trap_AAS_NextBSPEntity(0); ent; ent = trap_AAS_NextBSPEntity(ent)) {
CNSTI4 0
ARGI4
ADDRLP4 1108
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1108
INDIRI4
ASGNI4
ADDRGP4 $2272
JUMPV
LABELV $2269
line 5365
;5365:		if (!trap_AAS_ValueForBSPEpairKey(ent, "classname", value, sizeof(value)))
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1836
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1112
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
NEI4 $2273
line 5366
;5366:			continue;
ADDRGP4 $2270
JUMPV
LABELV $2273
line 5367
;5367:		if (!strcmp(value, classname)) {
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1116
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 0
NEI4 $2275
line 5368
;5368:			if (!trap_AAS_VectorForBSPEpairKey(ent, "origin", origin))
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $1852
ARGP4
ADDRLP4 1028
ARGP4
ADDRLP4 1120
ADDRGP4 trap_AAS_VectorForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 1120
INDIRI4
CNSTI4 0
NEI4 $2277
line 5369
;5369:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2268
JUMPV
LABELV $2277
line 5370
;5370:			VectorCopy(origin, goal->origin);
ADDRFP4 4
INDIRP4
ADDRLP4 1028
INDIRB
ASGNB 12
line 5371
;5371:			VectorCopy(origin, start);
ADDRLP4 1040
ADDRLP4 1028
INDIRB
ASGNB 12
line 5372
;5372:			start[2] -= 32;
ADDRLP4 1040+8
ADDRLP4 1040+8
INDIRF4
CNSTF4 1107296256
SUBF4
ASGNF4
line 5373
;5373:			VectorCopy(origin, end);
ADDRLP4 1052
ADDRLP4 1028
INDIRB
ASGNB 12
line 5374
;5374:			end[2] += 32;
ADDRLP4 1052+8
ADDRLP4 1052+8
INDIRF4
CNSTF4 1107296256
ADDF4
ASGNF4
line 5375
;5375:			numareas = trap_AAS_TraceAreas(start, end, areas, NULL, 10);
ADDRLP4 1040
ARGP4
ADDRLP4 1052
ARGP4
ADDRLP4 1068
ARGP4
CNSTP4 0
ARGP4
CNSTI4 10
ARGI4
ADDRLP4 1124
ADDRGP4 trap_AAS_TraceAreas
CALLI4
ASGNI4
ADDRLP4 1064
ADDRLP4 1124
INDIRI4
ASGNI4
line 5376
;5376:			if (!numareas)
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $2281
line 5377
;5377:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2268
JUMPV
LABELV $2281
line 5378
;5378:			goal->areanum = areas[0];
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 1068
INDIRI4
ASGNI4
line 5379
;5379:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2268
JUMPV
LABELV $2275
line 5381
;5380:		}
;5381:	}
LABELV $2270
line 5364
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1112
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1112
INDIRI4
ASGNI4
LABELV $2272
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2269
line 5382
;5382:	return qfalse;
CNSTI4 0
RETI4
LABELV $2268
endproc BotGoalForBSPEntity 1128 20
export BotSetupDeathmatchAI
proc BotSetupDeathmatchAI 156 16
line 5390
;5383:}
;5384:
;5385:/*
;5386:==================
;5387:BotSetupDeathmatchAI
;5388:==================
;5389:*/
;5390:void BotSetupDeathmatchAI(void) {
line 5394
;5391:	int ent, modelnum;
;5392:	char model[128];
;5393:
;5394:	gametype = trap_Cvar_VariableIntegerValue("g_gametype");
ADDRGP4 $2284
ARGP4
ADDRLP4 136
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 gametype
ADDRLP4 136
INDIRI4
ASGNI4
line 5395
;5395:	maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $279
ARGP4
ADDRLP4 140
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 maxclients
ADDRLP4 140
INDIRI4
ASGNI4
line 5397
;5396:
;5397:	trap_Cvar_Register(&bot_rocketjump, "bot_rocketjump", "1", 0);
ADDRGP4 bot_rocketjump
ARGP4
ADDRGP4 $2285
ARGP4
ADDRGP4 $2099
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5398
;5398:	trap_Cvar_Register(&bot_grapple, "bot_grapple", "0", 0);
ADDRGP4 bot_grapple
ARGP4
ADDRGP4 $2286
ARGP4
ADDRGP4 $2287
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5399
;5399:	trap_Cvar_Register(&bot_fastchat, "bot_fastchat", "0", 0);
ADDRGP4 bot_fastchat
ARGP4
ADDRGP4 $2288
ARGP4
ADDRGP4 $2287
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5400
;5400:	trap_Cvar_Register(&bot_nochat, "bot_nochat", "0", 0);
ADDRGP4 bot_nochat
ARGP4
ADDRGP4 $2289
ARGP4
ADDRGP4 $2287
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5401
;5401:	trap_Cvar_Register(&bot_testrchat, "bot_testrchat", "0", 0);
ADDRGP4 bot_testrchat
ARGP4
ADDRGP4 $2098
ARGP4
ADDRGP4 $2287
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5402
;5402:	trap_Cvar_Register(&bot_challenge, "bot_challenge", "0", 0);
ADDRGP4 bot_challenge
ARGP4
ADDRGP4 $2290
ARGP4
ADDRGP4 $2287
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5403
;5403:	trap_Cvar_Register(&bot_predictobstacles, "bot_predictobstacles", "1", 0);
ADDRGP4 bot_predictobstacles
ARGP4
ADDRGP4 $2291
ARGP4
ADDRGP4 $2099
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5404
;5404:	trap_Cvar_Register(&g_spSkill, "g_spSkill", "2", 0);
ADDRGP4 g_spSkill
ARGP4
ADDRGP4 $2292
ARGP4
ADDRGP4 $2293
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5406
;5405:	//
;5406:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $2294
line 5407
;5407:		if (trap_BotGetLevelItemGoal(-1, "Red Flag", &ctf_redflag) < 0)
CNSTI4 -1
ARGI4
ADDRGP4 $2298
ARGP4
ADDRGP4 ctf_redflag
ARGP4
ADDRLP4 144
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
GEI4 $2296
line 5408
;5408:			BotAI_Print(PRT_WARNING, "CTF without Red Flag\n");
CNSTI4 2
ARGI4
ADDRGP4 $2299
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
LABELV $2296
line 5409
;5409:		if (trap_BotGetLevelItemGoal(-1, "Blue Flag", &ctf_blueflag) < 0)
CNSTI4 -1
ARGI4
ADDRGP4 $2302
ARGP4
ADDRGP4 ctf_blueflag
ARGP4
ADDRLP4 148
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
GEI4 $2300
line 5410
;5410:			BotAI_Print(PRT_WARNING, "CTF without Blue Flag\n");
CNSTI4 2
ARGI4
ADDRGP4 $2303
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
LABELV $2300
line 5411
;5411:	}
LABELV $2294
line 5442
;5412:#ifdef MISSIONPACK
;5413:	else if (gametype == GT_1FCTF) {
;5414:		if (trap_BotGetLevelItemGoal(-1, "Neutral Flag", &ctf_neutralflag) < 0)
;5415:			BotAI_Print(PRT_WARNING, "One Flag CTF without Neutral Flag\n");
;5416:		if (trap_BotGetLevelItemGoal(-1, "Red Flag", &ctf_redflag) < 0)
;5417:			BotAI_Print(PRT_WARNING, "CTF without Red Flag\n");
;5418:		if (trap_BotGetLevelItemGoal(-1, "Blue Flag", &ctf_blueflag) < 0)
;5419:			BotAI_Print(PRT_WARNING, "CTF without Blue Flag\n");
;5420:	}
;5421:	else if (gametype == GT_OBELISK) {
;5422:		if (trap_BotGetLevelItemGoal(-1, "Red Obelisk", &redobelisk) < 0)
;5423:			BotAI_Print(PRT_WARNING, "Obelisk without red obelisk\n");
;5424:		BotSetEntityNumForGoal(&redobelisk, "team_redobelisk");
;5425:		if (trap_BotGetLevelItemGoal(-1, "Blue Obelisk", &blueobelisk) < 0)
;5426:			BotAI_Print(PRT_WARNING, "Obelisk without blue obelisk\n");
;5427:		BotSetEntityNumForGoal(&blueobelisk, "team_blueobelisk");
;5428:	}
;5429:	else if (gametype == GT_HARVESTER) {
;5430:		if (trap_BotGetLevelItemGoal(-1, "Red Obelisk", &redobelisk) < 0)
;5431:			BotAI_Print(PRT_WARNING, "Harvester without red obelisk\n");
;5432:		BotSetEntityNumForGoal(&redobelisk, "team_redobelisk");
;5433:		if (trap_BotGetLevelItemGoal(-1, "Blue Obelisk", &blueobelisk) < 0)
;5434:			BotAI_Print(PRT_WARNING, "Harvester without blue obelisk\n");
;5435:		BotSetEntityNumForGoal(&blueobelisk, "team_blueobelisk");
;5436:		if (trap_BotGetLevelItemGoal(-1, "Neutral Obelisk", &neutralobelisk) < 0)
;5437:			BotAI_Print(PRT_WARNING, "Harvester without neutral obelisk\n");
;5438:		BotSetEntityNumForGoal(&neutralobelisk, "team_neutralobelisk");
;5439:	}
;5440:#endif
;5441:
;5442:	max_bspmodelindex = 0;
ADDRGP4 max_bspmodelindex
CNSTI4 0
ASGNI4
line 5443
;5443:	for (ent = trap_AAS_NextBSPEntity(0); ent; ent = trap_AAS_NextBSPEntity(ent)) {
CNSTI4 0
ARGI4
ADDRLP4 144
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 144
INDIRI4
ASGNI4
ADDRGP4 $2307
JUMPV
LABELV $2304
line 5444
;5444:		if (!trap_AAS_ValueForBSPEpairKey(ent, "model", model, sizeof(model))) continue;
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $274
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 148
ADDRGP4 trap_AAS_ValueForBSPEpairKey
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $2308
ADDRGP4 $2305
JUMPV
LABELV $2308
line 5445
;5445:		if (model[0] == '*') {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $2310
line 5446
;5446:			modelnum = atoi(model+1);
ADDRLP4 4+1
ARGP4
ADDRLP4 152
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 132
ADDRLP4 152
INDIRI4
ASGNI4
line 5447
;5447:			if (modelnum > max_bspmodelindex)
ADDRLP4 132
INDIRI4
ADDRGP4 max_bspmodelindex
INDIRI4
LEI4 $2313
line 5448
;5448:				max_bspmodelindex = modelnum;
ADDRGP4 max_bspmodelindex
ADDRLP4 132
INDIRI4
ASGNI4
LABELV $2313
line 5449
;5449:		}
LABELV $2310
line 5450
;5450:	}
LABELV $2305
line 5443
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 148
ADDRGP4 trap_AAS_NextBSPEntity
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 148
INDIRI4
ASGNI4
LABELV $2307
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2304
line 5452
;5451:	//initialize the waypoint heap
;5452:	BotInitWaypoints();
ADDRGP4 BotInitWaypoints
CALLV
pop
line 5453
;5453:}
LABELV $2283
endproc BotSetupDeathmatchAI 156 16
export BotShutdownDeathmatchAI
proc BotShutdownDeathmatchAI 0 0
line 5460
;5454:
;5455:/*
;5456:==================
;5457:BotShutdownDeathmatchAI
;5458:==================
;5459:*/
;5460:void BotShutdownDeathmatchAI(void) {
line 5461
;5461:	altroutegoals_setup = qfalse;
ADDRGP4 altroutegoals_setup
CNSTI4 0
ASGNI4
line 5462
;5462:}
LABELV $2315
endproc BotShutdownDeathmatchAI 0 0
bss
export blue_numaltroutegoals
align 4
LABELV blue_numaltroutegoals
skip 4
export blue_altroutegoals
align 4
LABELV blue_altroutegoals
skip 768
export red_numaltroutegoals
align 4
LABELV red_numaltroutegoals
skip 4
export red_altroutegoals
align 4
LABELV red_altroutegoals
skip 768
export altroutegoals_setup
align 4
LABELV altroutegoals_setup
skip 4
export max_bspmodelindex
align 4
LABELV max_bspmodelindex
skip 4
export lastteleport_time
align 4
LABELV lastteleport_time
skip 4
export lastteleport_origin
align 4
LABELV lastteleport_origin
skip 12
import bot_developer
export g_spSkill
align 4
LABELV g_spSkill
skip 272
export bot_predictobstacles
align 4
LABELV bot_predictobstacles
skip 272
export botai_freewaypoints
align 4
LABELV botai_freewaypoints
skip 4
export botai_waypoints
align 4
LABELV botai_waypoints
skip 12800
import BotVoiceChatOnly
import BotVoiceChat
import BotSetTeamMateTaskPreference
import BotGetTeamMateTaskPreference
import BotTeamAI
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
export ctf_blueflag
align 4
LABELV ctf_blueflag
skip 56
export ctf_redflag
align 4
LABELV ctf_redflag
skip 56
export bot_challenge
align 4
LABELV bot_challenge
skip 272
export bot_testrchat
align 4
LABELV bot_testrchat
skip 272
export bot_nochat
align 4
LABELV bot_nochat
skip 272
export bot_fastchat
align 4
LABELV bot_fastchat
skip 272
export bot_rocketjump
align 4
LABELV bot_rocketjump
skip 272
export bot_grapple
align 4
LABELV bot_grapple
skip 272
export maxclients
align 4
LABELV maxclients
skip 4
export gametype
align 4
LABELV gametype
skip 4
import BotTeamLeader
import BotAI_GetSnapshotEntity
import BotAI_GetEntityState
import BotAI_GetClientState
import BotAI_Trace
import BotAI_BotInitialChat
import BotAI_Print
import floattime
import BotEntityInfo
import NumBots
import BotResetState
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
LABELV $2303
byte 1 67
byte 1 84
byte 1 70
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $2302
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $2299
byte 1 67
byte 1 84
byte 1 70
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 82
byte 1 101
byte 1 100
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $2298
byte 1 82
byte 1 101
byte 1 100
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $2293
byte 1 50
byte 1 0
align 1
LABELV $2292
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $2291
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 112
byte 1 114
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 116
byte 1 111
byte 1 98
byte 1 115
byte 1 116
byte 1 97
byte 1 99
byte 1 108
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $2290
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 99
byte 1 104
byte 1 97
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $2289
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
LABELV $2288
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 102
byte 1 97
byte 1 115
byte 1 116
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $2287
byte 1 48
byte 1 0
align 1
LABELV $2286
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 103
byte 1 114
byte 1 97
byte 1 112
byte 1 112
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2285
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 114
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 106
byte 1 117
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $2284
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
LABELV $2237
byte 1 37
byte 1 115
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 115
byte 1 119
byte 1 105
byte 1 116
byte 1 99
byte 1 104
byte 1 101
byte 1 100
byte 1 32
byte 1 109
byte 1 111
byte 1 114
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 97
byte 1 110
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 65
byte 1 73
byte 1 32
byte 1 110
byte 1 111
byte 1 100
byte 1 101
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2226
byte 1 66
byte 1 111
byte 1 116
byte 1 68
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 65
byte 1 73
byte 1 58
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2221
byte 1 66
byte 1 111
byte 1 116
byte 1 68
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 65
byte 1 73
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 97
byte 1 105
byte 1 32
byte 1 110
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $2210
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $2206
byte 1 115
byte 1 101
byte 1 120
byte 1 0
align 1
LABELV $2166
byte 1 42
byte 1 102
byte 1 97
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 49
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $2163
byte 1 69
byte 1 86
byte 1 95
byte 1 71
byte 1 69
byte 1 78
byte 1 69
byte 1 82
byte 1 65
byte 1 76
byte 1 95
byte 1 83
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 58
byte 1 32
byte 1 101
byte 1 118
byte 1 101
byte 1 110
byte 1 116
byte 1 80
byte 1 97
byte 1 114
byte 1 109
byte 1 32
byte 1 40
byte 1 37
byte 1 100
byte 1 41
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $2142
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 47
byte 1 112
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 117
byte 1 112
byte 1 114
byte 1 101
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $2139
byte 1 69
byte 1 86
byte 1 95
byte 1 71
byte 1 76
byte 1 79
byte 1 66
byte 1 65
byte 1 76
byte 1 95
byte 1 83
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 58
byte 1 32
byte 1 101
byte 1 118
byte 1 101
byte 1 110
byte 1 116
byte 1 80
byte 1 97
byte 1 114
byte 1 109
byte 1 32
byte 1 40
byte 1 37
byte 1 100
byte 1 41
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $2110
byte 1 66
byte 1 111
byte 1 116
byte 1 67
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 67
byte 1 111
byte 1 110
byte 1 115
byte 1 111
byte 1 108
byte 1 101
byte 1 77
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 115
byte 1 58
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 121
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $2103
byte 1 42
byte 1 42
byte 1 42
byte 1 42
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 121
byte 1 32
byte 1 42
byte 1 42
byte 1 42
byte 1 42
byte 1 10
byte 1 0
align 1
LABELV $2102
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 10
byte 1 0
align 1
LABELV $2099
byte 1 49
byte 1 0
align 1
LABELV $2098
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 114
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $1964
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 73
byte 1 32
byte 1 104
byte 1 97
byte 1 118
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 97
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 32
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $1963
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 73
byte 1 32
byte 1 104
byte 1 97
byte 1 118
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 115
byte 1 104
byte 1 111
byte 1 111
byte 1 116
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 97
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 102
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 32
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $1959
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 111
byte 1 70
byte 1 111
byte 1 114
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $1947
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 100
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $1946
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 95
byte 1 114
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $1943
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
byte 1 0
align 1
LABELV $1932
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
byte 1 0
align 1
LABELV $1919
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $1913
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 10
byte 1 0
align 1
LABELV $1905
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $1894
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $1890
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $1887
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $1852
byte 1 111
byte 1 114
byte 1 105
byte 1 103
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $1847
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 115
byte 1 0
align 1
LABELV $1842
byte 1 102
byte 1 117
byte 1 110
byte 1 99
byte 1 95
byte 1 100
byte 1 111
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $1839
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $1836
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $1835
byte 1 66
byte 1 111
byte 1 116
byte 1 71
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $1823
byte 1 42
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $1638
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $1615
byte 1 97
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $1612
byte 1 108
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $1574
byte 1 109
byte 1 112
byte 1 113
byte 1 51
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 54
byte 1 0
align 1
LABELV $1519
byte 1 113
byte 1 51
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 54
byte 1 0
align 1
LABELV $1515
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $669
byte 1 73
byte 1 110
byte 1 118
byte 1 105
byte 1 115
byte 1 105
byte 1 98
byte 1 105
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $668
byte 1 83
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $667
byte 1 66
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 83
byte 1 117
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $666
byte 1 82
byte 1 101
byte 1 103
byte 1 101
byte 1 110
byte 1 101
byte 1 114
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $665
byte 1 81
byte 1 117
byte 1 97
byte 1 100
byte 1 32
byte 1 68
byte 1 97
byte 1 109
byte 1 97
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $498
byte 1 66
byte 1 111
byte 1 116
byte 1 67
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 87
byte 1 97
byte 1 121
byte 1 80
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 58
byte 1 32
byte 1 79
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 119
byte 1 97
byte 1 121
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $324
byte 1 93
byte 1 0
align 1
LABELV $323
byte 1 91
byte 1 0
align 1
LABELV $322
byte 1 32
byte 1 0
align 1
LABELV $279
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
LABELV $274
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $273
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 83
byte 1 107
byte 1 105
byte 1 110
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $268
byte 1 110
byte 1 0
align 1
LABELV $267
byte 1 91
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 93
byte 1 0
align 1
LABELV $266
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 58
byte 1 32
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $185
byte 1 111
byte 1 110
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $160
byte 1 105
byte 1 104
byte 1 97
byte 1 118
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $159
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $158
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
LABELV $135
byte 1 110
byte 1 111
byte 1 0
align 1
LABELV $67
byte 1 116
byte 1 0
