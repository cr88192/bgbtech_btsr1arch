export BotGetItemTeamGoal
code
proc BotGetItemTeamGoal 12 12
file "../ai_cmd.c"
line 160
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
;25: * name:		ai_cmd.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_cmd.c $
;30: *
;31: *****************************************************************************/
;32:
;33:#include "g_local.h"
;34:#include "botlib.h"
;35:#include "be_aas.h"
;36:#include "be_ea.h"
;37:#include "be_ai_char.h"
;38:#include "be_ai_chat.h"
;39:#include "be_ai_gen.h"
;40:#include "be_ai_goal.h"
;41:#include "be_ai_move.h"
;42:#include "be_ai_weap.h"
;43://
;44:#include "ai_main.h"
;45:#include "ai_dmq3.h"
;46:#include "ai_chat.h"
;47:#include "ai_cmd.h"
;48:#include "ai_dmnet.h"
;49:#include "ai_team.h"
;50://
;51:#include "chars.h"				//characteristics
;52:#include "inv.h"				//indexes into the inventory
;53:#include "syn.h"				//synonyms
;54:#include "match.h"				//string matching types and vars
;55:
;56:// for the voice chats
;57:// #include "../../ui/menudef.h"
;58:#include "../ui/menudef.h"
;59:
;60:int notleader[MAX_CLIENTS];
;61:
;62:#ifdef DEBUG
;63:/*
;64:==================
;65:BotPrintTeamGoal
;66:==================
;67:*/
;68:void BotPrintTeamGoal(bot_state_t *bs) {
;69:	char netname[MAX_NETNAME];
;70:	float t;
;71:
;72:	ClientName(bs->client, netname, sizeof(netname));
;73:	t = bs->teamgoal_time - FloatTime();
;74:	switch(bs->ltgtype) {
;75:		case LTG_TEAMHELP:
;76:		{
;77:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna help a team mate for %1.0f secs\n", netname, t);
;78:			break;
;79:		}
;80:		case LTG_TEAMACCOMPANY:
;81:		{
;82:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna accompany a team mate for %1.0f secs\n", netname, t);
;83:			break;
;84:		}
;85:		case LTG_GETFLAG:
;86:		{
;87:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna get the flag for %1.0f secs\n", netname, t);
;88:			break;
;89:		}
;90:		case LTG_RUSHBASE:
;91:		{
;92:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna rush to the base for %1.0f secs\n", netname, t);
;93:			break;
;94:		}
;95:		case LTG_RETURNFLAG:
;96:		{
;97:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna try to return the flag for %1.0f secs\n", netname, t);
;98:			break;
;99:		}
;100:#ifdef MISSIONPACK
;101:		case LTG_ATTACKENEMYBASE:
;102:		{
;103:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna attack the enemy base for %1.0f secs\n", netname, t);
;104:			break;
;105:		}
;106:		case LTG_HARVEST:
;107:		{
;108:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna harvest for %1.0f secs\n", netname, t);
;109:			break;
;110:		}
;111:#endif
;112:		case LTG_DEFENDKEYAREA:
;113:		{
;114:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna defend a key area for %1.0f secs\n", netname, t);
;115:			break;
;116:		}
;117:		case LTG_GETITEM:
;118:		{
;119:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna get an item for %1.0f secs\n", netname, t);
;120:			break;
;121:		}
;122:		case LTG_KILL:
;123:		{
;124:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna kill someone for %1.0f secs\n", netname, t);
;125:			break;
;126:		}
;127:		case LTG_CAMP:
;128:		case LTG_CAMPORDER:
;129:		{
;130:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna camp for %1.0f secs\n", netname, t);
;131:			break;
;132:		}
;133:		case LTG_PATROL:
;134:		{
;135:			BotAI_Print(PRT_MESSAGE, "%s: I'm gonna patrol for %1.0f secs\n", netname, t);
;136:			break;
;137:		}
;138:		default:
;139:		{
;140:			if (bs->ctfroam_time > FloatTime()) {
;141:				t = bs->ctfroam_time - FloatTime();
;142:				BotAI_Print(PRT_MESSAGE, "%s: I'm gonna roam for %1.0f secs\n", netname, t);
;143:			}
;144:			else {
;145:				BotAI_Print(PRT_MESSAGE, "%s: I've got a regular goal\n", netname);
;146:			}
;147:		}
;148:	}
;149:}
;150:#endif //DEBUG
;151:
;152:/*
;153:==================
;154:BotGetItemTeamGoal
;155:
;156:FIXME: add stuff like "upper rocket launcher"
;157:"the rl near the railgun", "lower grenade launcher" etc.
;158:==================
;159:*/
;160:int BotGetItemTeamGoal(char *goalname, bot_goal_t *goal) {
line 163
;161:	int i;
;162:
;163:	if (!strlen(goalname)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $54
CNSTI4 0
RETI4
ADDRGP4 $53
JUMPV
LABELV $54
line 164
;164:	i = -1;
ADDRLP4 0
CNSTI4 -1
ASGNI4
LABELV $56
line 165
;165:	do {
line 166
;166:		i = trap_BotGetLevelItemGoal(i, goalname, goal);
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 167
;167:		if (i > 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LEI4 $59
line 169
;168:			//do NOT defend dropped items
;169:			if (goal->flags & GFL_DROPPED)
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $61
line 170
;170:				continue;
ADDRGP4 $57
JUMPV
LABELV $61
line 171
;171:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $53
JUMPV
LABELV $59
line 173
;172:		}
;173:	} while(i > 0);
LABELV $57
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $56
line 174
;174:	return qfalse;
CNSTI4 0
RETI4
LABELV $53
endproc BotGetItemTeamGoal 12 12
export BotGetMessageTeamGoal
proc BotGetMessageTeamGoal 12 12
line 182
;175:}
;176:
;177:/*
;178:==================
;179:BotGetMessageTeamGoal
;180:==================
;181:*/
;182:int BotGetMessageTeamGoal(bot_state_t *bs, char *goalname, bot_goal_t *goal) {
line 185
;183:	bot_waypoint_t *cp;
;184:
;185:	if (BotGetItemTeamGoal(goalname, goal)) return qtrue;
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotGetItemTeamGoal
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $64
CNSTI4 1
RETI4
ADDRGP4 $63
JUMPV
LABELV $64
line 187
;186:
;187:	cp = BotFindWayPoint(bs->checkpoints, goalname);
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 BotFindWayPoint
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 188
;188:	if (cp) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $66
line 189
;189:		memcpy(goal, &cp->goal, sizeof(bot_goal_t));
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 190
;190:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $63
JUMPV
LABELV $66
line 192
;191:	}
;192:	return qfalse;
CNSTI4 0
RETI4
LABELV $63
endproc BotGetMessageTeamGoal 12 12
export BotGetTime
proc BotGetTime 600 16
line 200
;193:}
;194:
;195:/*
;196:==================
;197:BotGetTime
;198:==================
;199:*/
;200:float BotGetTime(bot_match_t *match) {
line 206
;201:	bot_match_t timematch;
;202:	char timestring[MAX_MESSAGE_SIZE];
;203:	float t;
;204:
;205:	//if the matched string has a time
;206:	if (match->subtype & ST_TIME) {
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $69
line 208
;207:		//get the time string
;208:		trap_BotMatchVariable(match, TIME, timestring, MAX_MESSAGE_SIZE);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 6
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 210
;209:		//match it to find out if the time is in seconds or minutes
;210:		if (trap_BotFindMatch(timestring, &timematch, MTCONTEXT_TIME)) {
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
CNSTU4 8
ARGU4
ADDRLP4 588
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 588
INDIRI4
CNSTI4 0
EQI4 $71
line 211
;211:			if (timematch.type == MSG_FOREVER) {
ADDRLP4 256+256
INDIRI4
CNSTI4 107
NEI4 $73
line 212
;212:				t = 99999999.0f;
ADDRLP4 584
CNSTF4 1287568416
ASGNF4
line 213
;213:			}
ADDRGP4 $74
JUMPV
LABELV $73
line 214
;214:			else if (timematch.type == MSG_FORAWHILE) {
ADDRLP4 256+256
INDIRI4
CNSTI4 109
NEI4 $76
line 215
;215:				t = 10 * 60; // 10 minutes
ADDRLP4 584
CNSTF4 1142292480
ASGNF4
line 216
;216:			}
ADDRGP4 $77
JUMPV
LABELV $76
line 217
;217:			else if (timematch.type == MSG_FORALONGTIME) {
ADDRLP4 256+256
INDIRI4
CNSTI4 108
NEI4 $79
line 218
;218:				t = 30 * 60; // 30 minutes
ADDRLP4 584
CNSTF4 1155596288
ASGNF4
line 219
;219:			}
ADDRGP4 $80
JUMPV
LABELV $79
line 220
;220:			else {
line 221
;221:				trap_BotMatchVariable(&timematch, TIME, timestring, MAX_MESSAGE_SIZE);
ADDRLP4 256
ARGP4
CNSTI4 6
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 222
;222:				if (timematch.type == MSG_MINUTES) t = atof(timestring) * 60;
ADDRLP4 256+256
INDIRI4
CNSTI4 105
NEI4 $82
ADDRLP4 0
ARGP4
ADDRLP4 592
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 584
CNSTF4 1114636288
ADDRLP4 592
INDIRF4
MULF4
ASGNF4
ADDRGP4 $83
JUMPV
LABELV $82
line 223
;223:				else if (timematch.type == MSG_SECONDS) t = atof(timestring);
ADDRLP4 256+256
INDIRI4
CNSTI4 106
NEI4 $85
ADDRLP4 0
ARGP4
ADDRLP4 596
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 584
ADDRLP4 596
INDIRF4
ASGNF4
ADDRGP4 $86
JUMPV
LABELV $85
line 224
;224:				else t = 0;
ADDRLP4 584
CNSTF4 0
ASGNF4
LABELV $86
LABELV $83
line 225
;225:			}
LABELV $80
LABELV $77
LABELV $74
line 227
;226:			//if there's a valid time
;227:			if (t > 0) return FloatTime() + t;
ADDRLP4 584
INDIRF4
CNSTF4 0
LEF4 $88
ADDRGP4 floattime
INDIRF4
ADDRLP4 584
INDIRF4
ADDF4
RETF4
ADDRGP4 $68
JUMPV
LABELV $88
line 228
;228:		}
LABELV $71
line 229
;229:	}
LABELV $69
line 230
;230:	return 0;
CNSTF4 0
RETF4
LABELV $68
endproc BotGetTime 600 16
bss
align 4
LABELV $91
skip 4
export FindClientByName
code
proc FindClientByName 1040 12
line 238
;231:}
;232:
;233:/*
;234:==================
;235:FindClientByName
;236:==================
;237:*/
;238:int FindClientByName(char *name) {
line 243
;239:	int i;
;240:	char buf[MAX_INFO_STRING];
;241:	static int maxclients;
;242:
;243:	if (!maxclients)
ADDRGP4 $91
INDIRI4
CNSTI4 0
NEI4 $92
line 244
;244:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $94
ARGP4
ADDRLP4 1028
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $91
ADDRLP4 1028
INDIRI4
ASGNI4
LABELV $92
line 245
;245:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $98
JUMPV
LABELV $95
line 246
;246:		ClientName(i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 247
;247:		if (!Q_stricmp(buf, name)) return i;
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $99
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $90
JUMPV
LABELV $99
line 248
;248:	}
LABELV $96
line 245
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $98
ADDRLP4 0
INDIRI4
ADDRGP4 $91
INDIRI4
GEI4 $101
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $95
LABELV $101
line 249
;249:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $102
line 250
;250:		ClientName(i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 251
;251:		if (stristr(buf, name)) return i;
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $106
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $90
JUMPV
LABELV $106
line 252
;252:	}
LABELV $103
line 249
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $105
ADDRLP4 0
INDIRI4
ADDRGP4 $91
INDIRI4
GEI4 $108
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $102
LABELV $108
line 253
;253:	return -1;
CNSTI4 -1
RETI4
LABELV $90
endproc FindClientByName 1040 12
bss
align 4
LABELV $110
skip 4
export FindEnemyByName
code
proc FindEnemyByName 1044 12
line 261
;254:}
;255:
;256:/*
;257:==================
;258:FindEnemyByName
;259:==================
;260:*/
;261:int FindEnemyByName(bot_state_t *bs, char *name) {
line 266
;262:	int i;
;263:	char buf[MAX_INFO_STRING];
;264:	static int maxclients;
;265:
;266:	if (!maxclients)
ADDRGP4 $110
INDIRI4
CNSTI4 0
NEI4 $111
line 267
;267:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $94
ARGP4
ADDRLP4 1028
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $110
ADDRLP4 1028
INDIRI4
ASGNI4
LABELV $111
line 268
;268:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $116
JUMPV
LABELV $113
line 269
;269:		if (BotSameTeam(bs, i)) continue;
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
EQI4 $117
ADDRGP4 $114
JUMPV
LABELV $117
line 270
;270:		ClientName(i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 271
;271:		if (!Q_stricmp(buf, name)) return i;
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $119
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $109
JUMPV
LABELV $119
line 272
;272:	}
LABELV $114
line 268
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $116
ADDRLP4 0
INDIRI4
ADDRGP4 $110
INDIRI4
GEI4 $121
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $113
LABELV $121
line 273
;273:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $125
JUMPV
LABELV $122
line 274
;274:		if (BotSameTeam(bs, i)) continue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1036
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $126
ADDRGP4 $123
JUMPV
LABELV $126
line 275
;275:		ClientName(i, buf, sizeof(buf));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 276
;276:		if (stristr(buf, name)) return i;
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1040
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $128
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $109
JUMPV
LABELV $128
line 277
;277:	}
LABELV $123
line 273
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $125
ADDRLP4 0
INDIRI4
ADDRGP4 $110
INDIRI4
GEI4 $130
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $122
LABELV $130
line 278
;278:	return -1;
CNSTI4 -1
RETI4
LABELV $109
endproc FindEnemyByName 1044 12
bss
align 4
LABELV $132
skip 4
export NumPlayersOnSameTeam
code
proc NumPlayersOnSameTeam 1044 12
line 286
;279:}
;280:
;281:/*
;282:==================
;283:NumPlayersOnSameTeam
;284:==================
;285:*/
;286:int NumPlayersOnSameTeam(bot_state_t *bs) {
line 291
;287:	int i, num;
;288:	char buf[MAX_INFO_STRING];
;289:	static int maxclients;
;290:
;291:	if (!maxclients)
ADDRGP4 $132
INDIRI4
CNSTI4 0
NEI4 $133
line 292
;292:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $94
ARGP4
ADDRLP4 1032
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $132
ADDRLP4 1032
INDIRI4
ASGNI4
LABELV $133
line 294
;293:
;294:	num = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 295
;295:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $138
JUMPV
LABELV $135
line 296
;296:		trap_GetConfigstring(CS_PLAYERS+i, buf, MAX_INFO_STRING);
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
line 297
;297:		if (strlen(buf)) {
ADDRLP4 4
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $139
line 298
;298:			if (BotSameTeam(bs, i+1)) num++;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1040
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $141
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $141
line 299
;299:		}
LABELV $139
line 300
;300:	}
LABELV $136
line 295
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $138
ADDRLP4 0
INDIRI4
ADDRGP4 $132
INDIRI4
GEI4 $143
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $135
LABELV $143
line 301
;301:	return num;
ADDRLP4 1028
INDIRI4
RETI4
LABELV $131
endproc NumPlayersOnSameTeam 1044 12
export BotGetPatrolWaypoints
proc BotGetPatrolWaypoints 676 16
line 309
;302:}
;303:
;304:/*
;305:==================
;306:TeamPlayIsOn
;307:==================
;308:*/
;309:int BotGetPatrolWaypoints(bot_state_t *bs, bot_match_t *match) {
line 316
;310:	char keyarea[MAX_MESSAGE_SIZE];
;311:	int patrolflags;
;312:	bot_waypoint_t *wp, *newwp, *newpatrolpoints;
;313:	bot_match_t keyareamatch;
;314:	bot_goal_t goal;
;315:
;316:	newpatrolpoints = NULL;
ADDRLP4 592
CNSTP4 0
ASGNP4
line 317
;317:	patrolflags = 0;
ADDRLP4 652
CNSTI4 0
ASGNI4
line 319
;318:	//
;319:	trap_BotMatchVariable(match, KEYAREA, keyarea, MAX_MESSAGE_SIZE);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
ADDRGP4 $146
JUMPV
LABELV $145
line 321
;320:	//
;321:	while(1) {
line 322
;322:		if (!trap_BotFindMatch(keyarea, &keyareamatch, MTCONTEXT_PATROLKEYAREA)) {
ADDRLP4 8
ARGP4
ADDRLP4 264
ARGP4
CNSTU4 64
ARGU4
ADDRLP4 656
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 656
INDIRI4
CNSTI4 0
NEI4 $148
line 323
;323:			trap_EA_SayTeam(bs->client, "what do you say?");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $150
ARGP4
ADDRGP4 trap_EA_SayTeam
CALLV
pop
line 324
;324:			BotFreeWaypoints(newpatrolpoints);
ADDRLP4 592
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 325
;325:			bs->patrolpoints = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
CNSTP4 0
ASGNP4
line 326
;326:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $144
JUMPV
LABELV $148
line 328
;327:		}
;328:		trap_BotMatchVariable(&keyareamatch, KEYAREA, keyarea, MAX_MESSAGE_SIZE);
ADDRLP4 264
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 329
;329:		if (!BotGetMessageTeamGoal(bs, keyarea, &goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 596
ARGP4
ADDRLP4 660
ADDRGP4 BotGetMessageTeamGoal
CALLI4
ASGNI4
ADDRLP4 660
INDIRI4
CNSTI4 0
NEI4 $151
line 332
;330:			//BotAI_BotInitialChat(bs, "cannotfind", keyarea, NULL);
;331:			//trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;332:			BotFreeWaypoints(newpatrolpoints);
ADDRLP4 592
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 333
;333:			bs->patrolpoints = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
CNSTP4 0
ASGNP4
line 334
;334:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $144
JUMPV
LABELV $151
line 337
;335:		}
;336:		//create a new waypoint
;337:		newwp = BotCreateWayPoint(keyarea, goal.origin, goal.areanum);
ADDRLP4 8
ARGP4
ADDRLP4 596
ARGP4
ADDRLP4 596+12
INDIRI4
ARGI4
ADDRLP4 664
ADDRGP4 BotCreateWayPoint
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 664
INDIRP4
ASGNP4
line 338
;338:		if (!newwp)
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $154
line 339
;339:			break;
ADDRGP4 $147
JUMPV
LABELV $154
line 341
;340:		//add the waypoint to the patrol points
;341:		newwp->next = NULL;
ADDRLP4 4
INDIRP4
CNSTI4 92
ADDP4
CNSTP4 0
ASGNP4
line 342
;342:		for (wp = newpatrolpoints; wp && wp->next; wp = wp->next);
ADDRLP4 0
ADDRLP4 592
INDIRP4
ASGNP4
ADDRGP4 $159
JUMPV
LABELV $156
LABELV $157
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
LABELV $159
ADDRLP4 672
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 672
INDIRU4
EQU4 $160
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 672
INDIRU4
NEU4 $156
LABELV $160
line 343
;343:		if (!wp) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $161
line 344
;344:			newpatrolpoints = newwp;
ADDRLP4 592
ADDRLP4 4
INDIRP4
ASGNP4
line 345
;345:			newwp->prev = NULL;
ADDRLP4 4
INDIRP4
CNSTI4 96
ADDP4
CNSTP4 0
ASGNP4
line 346
;346:		}
ADDRGP4 $162
JUMPV
LABELV $161
line 347
;347:		else {
line 348
;348:			wp->next = newwp;
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 349
;349:			newwp->prev = wp;
ADDRLP4 4
INDIRP4
CNSTI4 96
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 350
;350:		}
LABELV $162
line 352
;351:		//
;352:		if (keyareamatch.subtype & ST_BACK) {
ADDRLP4 264+260
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $163
line 353
;353:			patrolflags = PATROL_LOOP;
ADDRLP4 652
CNSTI4 1
ASGNI4
line 354
;354:			break;
ADDRGP4 $147
JUMPV
LABELV $163
line 356
;355:		}
;356:		else if (keyareamatch.subtype & ST_REVERSE) {
ADDRLP4 264+260
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $166
line 357
;357:			patrolflags = PATROL_REVERSE;
ADDRLP4 652
CNSTI4 2
ASGNI4
line 358
;358:			break;
ADDRGP4 $147
JUMPV
LABELV $166
line 360
;359:		}
;360:		else if (keyareamatch.subtype & ST_MORE) {
ADDRLP4 264+260
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $147
line 361
;361:			trap_BotMatchVariable(&keyareamatch, MORE, keyarea, MAX_MESSAGE_SIZE);
ADDRLP4 264
ARGP4
CNSTI4 6
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 362
;362:		}
line 363
;363:		else {
line 364
;364:			break;
LABELV $170
line 366
;365:		}
;366:	}
LABELV $146
line 321
ADDRGP4 $145
JUMPV
LABELV $147
line 368
;367:	//
;368:	if (!newpatrolpoints || !newpatrolpoints->next) {
ADDRLP4 660
CNSTU4 0
ASGNU4
ADDRLP4 592
INDIRP4
CVPU4 4
ADDRLP4 660
INDIRU4
EQU4 $174
ADDRLP4 592
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 660
INDIRU4
NEU4 $172
LABELV $174
line 369
;369:		trap_EA_SayTeam(bs->client, "I need more key points to patrol\n");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $175
ARGP4
ADDRGP4 trap_EA_SayTeam
CALLV
pop
line 370
;370:		BotFreeWaypoints(newpatrolpoints);
ADDRLP4 592
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 371
;371:		newpatrolpoints = NULL;
ADDRLP4 592
CNSTP4 0
ASGNP4
line 372
;372:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $144
JUMPV
LABELV $172
line 375
;373:	}
;374:	//
;375:	BotFreeWaypoints(bs->patrolpoints);
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
INDIRP4
ARGP4
ADDRGP4 BotFreeWaypoints
CALLV
pop
line 376
;376:	bs->patrolpoints = newpatrolpoints;
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
ADDRLP4 592
INDIRP4
ASGNP4
line 378
;377:	//
;378:	bs->curpatrolpoint = bs->patrolpoints;
ADDRLP4 664
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 664
INDIRP4
CNSTI4 9080
ADDP4
ADDRLP4 664
INDIRP4
CNSTI4 9076
ADDP4
INDIRP4
ASGNP4
line 379
;379:	bs->patrolflags = patrolflags;
ADDRFP4 0
INDIRP4
CNSTI4 9084
ADDP4
ADDRLP4 652
INDIRI4
ASGNI4
line 381
;380:	//
;381:	return qtrue;
CNSTI4 1
RETI4
LABELV $144
endproc BotGetPatrolWaypoints 676 16
export BotAddressedToBot
proc BotAddressedToBot 1572 16
line 389
;382:}
;383:
;384:/*
;385:==================
;386:BotAddressedToBot
;387:==================
;388:*/
;389:int BotAddressedToBot(bot_state_t *bs, bot_match_t *match) {
line 397
;390:	char addressedto[MAX_MESSAGE_SIZE];
;391:	char netname[MAX_MESSAGE_SIZE];
;392:	char name[MAX_MESSAGE_SIZE];
;393:	char botname[128];
;394:	int client;
;395:	bot_match_t addresseematch;
;396:
;397:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 840
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 398
;398:	client = ClientOnSameTeamFromName(bs, netname);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 840
ARGP4
ADDRLP4 1228
ADDRGP4 ClientOnSameTeamFromName
CALLI4
ASGNI4
ADDRLP4 1096
ADDRLP4 1228
INDIRI4
ASGNI4
line 399
;399:	if (client < 0) return qfalse;
ADDRLP4 1096
INDIRI4
CNSTI4 0
GEI4 $177
CNSTI4 0
RETI4
ADDRGP4 $176
JUMPV
LABELV $177
line 401
;400:	//if the message is addressed to someone
;401:	if (match->subtype & ST_ADDRESSED) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $179
line 402
;402:		trap_BotMatchVariable(match, ADDRESSEE, addressedto, sizeof(addressedto));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 2
ARGI4
ADDRLP4 584
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 404
;403:		//the name of this bot
;404:		ClientName(bs->client, botname, 128);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 1100
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 ClientName
CALLP4
pop
ADDRGP4 $182
JUMPV
LABELV $181
line 406
;405:		//
;406:		while(trap_BotFindMatch(addressedto, &addresseematch, MTCONTEXT_ADDRESSEE)) {
line 407
;407:			if (addresseematch.type == MSG_EVERYONE) {
ADDRLP4 0+256
INDIRI4
CNSTI4 101
NEI4 $184
line 408
;408:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $176
JUMPV
LABELV $184
line 410
;409:			}
;410:			else if (addresseematch.type == MSG_MULTIPLENAMES) {
ADDRLP4 0+256
INDIRI4
CNSTI4 102
NEI4 $187
line 411
;411:				trap_BotMatchVariable(&addresseematch, TEAMMATE, name, sizeof(name));
ADDRLP4 0
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 328
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 412
;412:				if (strlen(name)) {
ADDRLP4 328
ARGP4
ADDRLP4 1232
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 0
EQI4 $190
line 413
;413:					if (stristr(botname, name)) return qtrue;
ADDRLP4 1100
ARGP4
ADDRLP4 328
ARGP4
ADDRLP4 1236
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1236
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $192
CNSTI4 1
RETI4
ADDRGP4 $176
JUMPV
LABELV $192
line 414
;414:					if (stristr(bs->subteam, name)) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRLP4 328
ARGP4
ADDRLP4 1240
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1240
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $194
CNSTI4 1
RETI4
ADDRGP4 $176
JUMPV
LABELV $194
line 415
;415:				}
LABELV $190
line 416
;416:				trap_BotMatchVariable(&addresseematch, MORE, addressedto, MAX_MESSAGE_SIZE);
ADDRLP4 0
ARGP4
CNSTI4 6
ARGI4
ADDRLP4 584
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 417
;417:			}
ADDRGP4 $188
JUMPV
LABELV $187
line 418
;418:			else {
line 419
;419:				trap_BotMatchVariable(&addresseematch, TEAMMATE, name, MAX_MESSAGE_SIZE);
ADDRLP4 0
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 328
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 420
;420:				if (strlen(name)) {
ADDRLP4 328
ARGP4
ADDRLP4 1232
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 0
EQI4 $183
line 421
;421:					if (stristr(botname, name)) return qtrue;
ADDRLP4 1100
ARGP4
ADDRLP4 328
ARGP4
ADDRLP4 1236
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1236
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $198
CNSTI4 1
RETI4
ADDRGP4 $176
JUMPV
LABELV $198
line 422
;422:					if (stristr(bs->subteam, name)) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRLP4 328
ARGP4
ADDRLP4 1240
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 1240
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $183
CNSTI4 1
RETI4
ADDRGP4 $176
JUMPV
line 423
;423:				}
line 424
;424:				break;
LABELV $188
line 426
;425:			}
;426:		}
LABELV $182
line 406
ADDRLP4 584
ARGP4
ADDRLP4 0
ARGP4
CNSTU4 32
ARGU4
ADDRLP4 1232
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 0
NEI4 $181
LABELV $183
line 429
;427:		//Com_sprintf(buf, sizeof(buf), "not addressed to me but %s", addressedto);
;428:		//trap_EA_Say(bs->client, buf);
;429:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $176
JUMPV
LABELV $179
line 431
;430:	}
;431:	else {
line 434
;432:		bot_match_t tellmatch;
;433:
;434:		tellmatch.type = 0;
ADDRLP4 1232+256
CNSTI4 0
ASGNI4
line 436
;435:		//if this message wasn't directed solely to this bot
;436:		if (!trap_BotFindMatch(match->string, &tellmatch, MTCONTEXT_REPLYCHAT) ||
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1232
ARGP4
CNSTU4 128
ARGU4
ADDRLP4 1560
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 1560
INDIRI4
CNSTI4 0
EQI4 $206
ADDRLP4 1232+256
INDIRI4
CNSTI4 202
EQI4 $203
LABELV $206
line 437
;437:				tellmatch.type != MSG_CHATTELL) {
line 439
;438:			//make sure not everyone reacts to this message
;439:			if (random() > (float ) 1.0 / (NumPlayersOnSameTeam(bs)-1)) return qfalse;
ADDRLP4 1564
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1568
ADDRGP4 NumPlayersOnSameTeam
CALLI4
ASGNI4
ADDRLP4 1564
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1065353216
ADDRLP4 1568
INDIRI4
CNSTI4 1
SUBI4
CVIF4 4
DIVF4
LEF4 $207
CNSTI4 0
RETI4
ADDRGP4 $176
JUMPV
LABELV $207
line 440
;440:		}
LABELV $203
line 441
;441:	}
line 442
;442:	return qtrue;
CNSTI4 1
RETI4
LABELV $176
endproc BotAddressedToBot 1572 16
export BotGPSToPosition
proc BotGPSToPosition 20 12
line 450
;443:}
;444:
;445:/*
;446:==================
;447:BotGPSToPosition
;448:==================
;449:*/
;450:int BotGPSToPosition(char *buf, vec3_t position) {
line 451
;451:	int i, j = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 454
;452:	int num, sign;
;453:
;454:	for (i = 0; i < 3; i++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $210
line 455
;455:		num = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $215
JUMPV
LABELV $214
line 456
;456:		while(buf[j] == ' ') j++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $215
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 32
EQI4 $214
line 457
;457:		if (buf[j] == '-') {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 45
NEI4 $217
line 458
;458:			j++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 459
;459:			sign = -1;
ADDRLP4 12
CNSTI4 -1
ASGNI4
line 460
;460:		}
ADDRGP4 $220
JUMPV
LABELV $217
line 461
;461:		else {
line 462
;462:			sign = 1;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 463
;463:		}
ADDRGP4 $220
JUMPV
LABELV $219
line 464
;464:		while (buf[j]) {
line 465
;465:			if (buf[j] >= '0' && buf[j] <= '9') {
ADDRLP4 16
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 48
LTI4 $222
ADDRLP4 16
INDIRI4
CNSTI4 57
GTI4 $222
line 466
;466:				num = num * 10 + buf[j] - '0';
ADDRLP4 4
CNSTI4 10
ADDRLP4 4
INDIRI4
MULI4
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ADDI4
CNSTI4 48
SUBI4
ASGNI4
line 467
;467:				j++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 468
;468:			}
ADDRGP4 $223
JUMPV
LABELV $222
line 469
;469:			else {
line 470
;470:				j++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 471
;471:				break;
ADDRGP4 $221
JUMPV
LABELV $223
line 473
;472:			}
;473:		}
LABELV $220
line 464
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $219
LABELV $221
line 474
;474:		BotAI_Print(PRT_MESSAGE, "%d\n", sign * num);
CNSTI4 1
ARGI4
ADDRGP4 $224
ARGP4
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
ARGI4
ADDRGP4 BotAI_Print
CALLV
pop
line 475
;475:		position[i] = (float) sign * num;
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ADDRLP4 12
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 476
;476:	}
LABELV $211
line 454
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 3
LTI4 $210
line 477
;477:	return qtrue;
CNSTI4 1
RETI4
LABELV $209
endproc BotGPSToPosition 20 12
export BotMatch_HelpAccompany
proc BotMatch_HelpAccompany 1272 16
line 485
;478:}
;479:
;480:/*
;481:==================
;482:BotMatch_HelpAccompany
;483:==================
;484:*/
;485:void BotMatch_HelpAccompany(bot_state_t *bs, bot_match_t *match) {
line 493
;486:	int client, other, areanum;
;487:	char teammate[MAX_MESSAGE_SIZE];
;488:	char netname[MAX_MESSAGE_SIZE];
;489:	char itemname[MAX_MESSAGE_SIZE];
;490:	bot_match_t teammatematch;
;491:	aas_entityinfo_t entinfo;
;492:
;493:	if (!TeamPlayIsOn()) return;
ADDRLP4 1248
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 1248
INDIRI4
CNSTI4 0
NEI4 $226
ADDRGP4 $225
JUMPV
LABELV $226
line 495
;494:	//if not addressed to this bot
;495:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1252
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 1252
INDIRI4
CNSTI4 0
NEI4 $228
ADDRGP4 $225
JUMPV
LABELV $228
line 497
;496:	//get the team mate name
;497:	trap_BotMatchVariable(match, TEAMMATE, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 499
;498:	//get the client to help
;499:	if (trap_BotFindMatch(teammate, &teammatematch, MTCONTEXT_TEAMMATE) &&
ADDRLP4 260
ARGP4
ADDRLP4 656
ARGP4
CNSTU4 16
ARGU4
ADDRLP4 1256
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 1256
INDIRI4
CNSTI4 0
EQI4 $230
ADDRLP4 656+256
INDIRI4
CNSTI4 100
NEI4 $230
line 501
;500:			//if someone asks for him or herself
;501:			teammatematch.type == MSG_ME) {
line 503
;502:		//get the netname
;503:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 504
;504:		client = ClientFromName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 1260
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1260
INDIRI4
ASGNI4
line 505
;505:		other = qfalse;
ADDRLP4 984
CNSTI4 0
ASGNI4
line 506
;506:	}
ADDRGP4 $231
JUMPV
LABELV $230
line 507
;507:	else {
line 509
;508:		//asked for someone else
;509:		client = FindClientByName(teammate);
ADDRLP4 260
ARGP4
ADDRLP4 1260
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1260
INDIRI4
ASGNI4
line 511
;510:		//if this is the bot self
;511:		if (client == bs->client) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $233
line 512
;512:			other = qfalse;
ADDRLP4 984
CNSTI4 0
ASGNI4
line 513
;513:		}
ADDRGP4 $234
JUMPV
LABELV $233
line 514
;514:		else if (!BotSameTeam(bs, client)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1264
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1264
INDIRI4
CNSTI4 0
NEI4 $235
line 516
;515:			//FIXME: say "I don't help the enemy"
;516:			return;
ADDRGP4 $225
JUMPV
LABELV $235
line 518
;517:		}
;518:		else {
line 519
;519:			other = qtrue;
ADDRLP4 984
CNSTI4 1
ASGNI4
line 520
;520:		}
LABELV $234
line 521
;521:	}
LABELV $231
line 523
;522:	//if the bot doesn't know who to help (FindClientByName returned -1)
;523:	if (client < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $237
line 524
;524:		if (other) BotAI_BotInitialChat(bs, "whois", teammate, NULL);
ADDRLP4 984
INDIRI4
CNSTI4 0
EQI4 $239
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $240
JUMPV
LABELV $239
line 525
;525:		else BotAI_BotInitialChat(bs, "whois", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
LABELV $240
line 526
;526:		client = ClientFromName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 1260
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1260
INDIRI4
ASGNI4
line 527
;527:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 528
;528:		return;
ADDRGP4 $225
JUMPV
LABELV $237
line 531
;529:	}
;530:	//don't help or accompany yourself
;531:	if (client == bs->client) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $242
line 532
;532:		return;
ADDRGP4 $225
JUMPV
LABELV $242
line 535
;533:	}
;534:	//
;535:	bs->teamgoal.entitynum = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
CNSTI4 -1
ASGNI4
line 536
;536:	BotEntityInfo(client, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 516
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 538
;537:	//if info is valid (in PVS)
;538:	if (entinfo.valid) {
ADDRLP4 516
INDIRI4
CNSTI4 0
EQI4 $244
line 539
;539:		areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 516+24
ARGP4
ADDRLP4 1260
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 988
ADDRLP4 1260
INDIRI4
ASGNI4
line 540
;540:		if (areanum) {// && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 988
INDIRI4
CNSTI4 0
EQI4 $247
line 541
;541:			bs->teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 542
;542:			bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 988
INDIRI4
ASGNI4
line 543
;543:			VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 516+24
INDIRB
ASGNB 12
line 544
;544:			VectorSet(bs->teamgoal.mins, -8, -8, -8);
ADDRFP4 0
INDIRP4
CNSTI4 6640
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6644
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6648
ADDP4
CNSTF4 3238002688
ASGNF4
line 545
;545:			VectorSet(bs->teamgoal.maxs, 8, 8, 8);
ADDRFP4 0
INDIRP4
CNSTI4 6652
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6656
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6660
ADDP4
CNSTF4 1090519040
ASGNF4
line 546
;546:		}
LABELV $247
line 547
;547:	}
LABELV $244
line 549
;548:	//if no teamgoal yet
;549:	if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $250
line 551
;550:		//if near an item
;551:		if (match->subtype & ST_NEARITEM) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $252
line 553
;552:			//get the match variable
;553:			trap_BotMatchVariable(match, ITEM, itemname, sizeof(itemname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 992
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 555
;554:			//
;555:			if (!BotGetMessageTeamGoal(bs, itemname, &bs->teamgoal)) {
ADDRLP4 1260
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1260
INDIRP4
ARGP4
ADDRLP4 992
ARGP4
ADDRLP4 1260
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRLP4 1264
ADDRGP4 BotGetMessageTeamGoal
CALLI4
ASGNI4
ADDRLP4 1264
INDIRI4
CNSTI4 0
NEI4 $254
line 558
;556:				//BotAI_BotInitialChat(bs, "cannotfind", itemname, NULL);
;557:				//trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
;558:				return;
ADDRGP4 $225
JUMPV
LABELV $254
line 560
;559:			}
;560:		}
LABELV $252
line 561
;561:	}
LABELV $250
line 563
;562:	//
;563:	if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $256
line 564
;564:		if (other) BotAI_BotInitialChat(bs, "whereis", teammate, NULL);
ADDRLP4 984
INDIRI4
CNSTI4 0
EQI4 $258
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $260
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $259
JUMPV
LABELV $258
line 565
;565:		else BotAI_BotInitialChat(bs, "whereareyou", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $261
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
LABELV $259
line 566
;566:		client = ClientFromName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 1260
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1260
INDIRI4
ASGNI4
line 567
;567:		trap_BotEnterChat(bs->cs, client, CHAT_TEAM);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 568
;568:		return;
ADDRGP4 $225
JUMPV
LABELV $256
line 571
;569:	}
;570:	//the team mate
;571:	bs->teammate = client;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 573
;572:	//
;573:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 575
;574:	//
;575:	client = ClientFromName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 1260
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 1260
INDIRI4
ASGNI4
line 577
;576:	//the team mate who ordered
;577:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 578
;578:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 579
;579:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 581
;580:	//last time the team mate was assumed visible
;581:	bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 583
;582:	//set the time to send a message to the team mates
;583:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 1264
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
ADDRLP4 1264
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 585
;584:	//get the team goal time
;585:	bs->teamgoal_time = BotGetTime(match);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1268
ADDRGP4 BotGetTime
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRLP4 1268
INDIRF4
ASGNF4
line 587
;586:	//set the ltg type
;587:	if (match->type == MSG_HELP) {
ADDRFP4 4
INDIRP4
CNSTI4 256
ADDP4
INDIRI4
CNSTI4 3
NEI4 $262
line 588
;588:		bs->ltgtype = LTG_TEAMHELP;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 1
ASGNI4
line 589
;589:		if (!bs->teamgoal_time) bs->teamgoal_time = FloatTime() + TEAM_HELP_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
CNSTF4 0
NEF4 $263
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 590
;590:	}
ADDRGP4 $263
JUMPV
LABELV $262
line 591
;591:	else {
line 592
;592:		bs->ltgtype = LTG_TEAMACCOMPANY;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 2
ASGNI4
line 593
;593:		if (!bs->teamgoal_time) bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
CNSTF4 0
NEF4 $266
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
LABELV $266
line 594
;594:		bs->formation_dist = 3.5 * 32;		//3.5 meter
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
CNSTF4 1121976320
ASGNF4
line 595
;595:		bs->arrive_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 0
ASGNF4
line 597
;596:		//
;597:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 599
;598:		// remember last ordered task
;599:		BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 600
;600:	}
LABELV $263
line 604
;601:#ifdef DEBUG
;602:	BotPrintTeamGoal(bs);
;603:#endif //DEBUG
;604:}
LABELV $225
endproc BotMatch_HelpAccompany 1272 16
export BotMatch_DefendKeyArea
proc BotMatch_DefendKeyArea 544 16
line 611
;605:
;606:/*
;607:==================
;608:BotMatch_DefendKeyArea
;609:==================
;610:*/
;611:void BotMatch_DefendKeyArea(bot_state_t *bs, bot_match_t *match) {
line 616
;612:	char itemname[MAX_MESSAGE_SIZE];
;613:	char netname[MAX_MESSAGE_SIZE];
;614:	int client;
;615:
;616:	if (!TeamPlayIsOn()) return;
ADDRLP4 516
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $269
ADDRGP4 $268
JUMPV
LABELV $269
line 618
;617:	//if not addressed to this bot
;618:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 520
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $271
ADDRGP4 $268
JUMPV
LABELV $271
line 620
;619:	//get the match variable
;620:	trap_BotMatchVariable(match, KEYAREA, itemname, sizeof(itemname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 622
;621:	//
;622:	if (!BotGetMessageTeamGoal(bs, itemname, &bs->teamgoal)) {
ADDRLP4 524
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 524
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 524
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRLP4 528
ADDRGP4 BotGetMessageTeamGoal
CALLI4
ASGNI4
ADDRLP4 528
INDIRI4
CNSTI4 0
NEI4 $273
line 625
;623:		//BotAI_BotInitialChat(bs, "cannotfind", itemname, NULL);
;624:		//trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
;625:		return;
ADDRGP4 $268
JUMPV
LABELV $273
line 628
;626:	}
;627:	//
;628:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 630
;629:	//
;630:	client = ClientFromName(netname);
ADDRLP4 256
ARGP4
ADDRLP4 532
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 512
ADDRLP4 532
INDIRI4
ASGNI4
line 632
;631:	//the team mate who ordered
;632:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 512
INDIRI4
ASGNI4
line 633
;633:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 634
;634:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 636
;635:	//set the time to send a message to the team mates
;636:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 536
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
ADDRLP4 536
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 638
;637:	//set the ltg type
;638:	bs->ltgtype = LTG_DEFENDKEYAREA;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 3
ASGNI4
line 640
;639:	//get the team goal time
;640:	bs->teamgoal_time = BotGetTime(match);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 540
ADDRGP4 BotGetTime
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRLP4 540
INDIRF4
ASGNF4
line 642
;641:	//set the team goal time
;642:	if (!bs->teamgoal_time) bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
CNSTF4 0
NEF4 $275
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
LABELV $275
line 644
;643:	//away from defending
;644:	bs->defendaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6144
ADDP4
CNSTF4 0
ASGNF4
line 646
;645:	//
;646:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 648
;647:	// remember last ordered task
;648:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 652
;649:#ifdef DEBUG
;650:	BotPrintTeamGoal(bs);
;651:#endif //DEBUG
;652:}
LABELV $268
endproc BotMatch_DefendKeyArea 544 16
export BotMatch_GetItem
proc BotMatch_GetItem 540 16
line 659
;653:
;654:/*
;655:==================
;656:BotMatch_GetItem
;657:==================
;658:*/
;659:void BotMatch_GetItem(bot_state_t *bs, bot_match_t *match) {
line 664
;660:	char itemname[MAX_MESSAGE_SIZE];
;661:	char netname[MAX_MESSAGE_SIZE];
;662:	int client;
;663:
;664:	if (!TeamPlayIsOn()) return;
ADDRLP4 516
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $278
ADDRGP4 $277
JUMPV
LABELV $278
line 666
;665:	//if not addressed to this bot
;666:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 520
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $280
ADDRGP4 $277
JUMPV
LABELV $280
line 668
;667:	//get the match variable
;668:	trap_BotMatchVariable(match, ITEM, itemname, sizeof(itemname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 670
;669:	//
;670:	if (!BotGetMessageTeamGoal(bs, itemname, &bs->teamgoal)) {
ADDRLP4 524
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 524
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 524
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRLP4 528
ADDRGP4 BotGetMessageTeamGoal
CALLI4
ASGNI4
ADDRLP4 528
INDIRI4
CNSTI4 0
NEI4 $282
line 673
;671:		//BotAI_BotInitialChat(bs, "cannotfind", itemname, NULL);
;672:		//trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
;673:		return;
ADDRGP4 $277
JUMPV
LABELV $282
line 675
;674:	}
;675:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 676
;676:	client = ClientOnSameTeamFromName(bs, netname);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
ARGP4
ADDRLP4 532
ADDRGP4 ClientOnSameTeamFromName
CALLI4
ASGNI4
ADDRLP4 512
ADDRLP4 532
INDIRI4
ASGNI4
line 678
;677:	//
;678:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 512
INDIRI4
ASGNI4
line 679
;679:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 680
;680:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 682
;681:	//set the time to send a message to the team mates
;682:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 536
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
ADDRLP4 536
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 684
;683:	//set the ltg type
;684:	bs->ltgtype = LTG_GETITEM;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 10
ASGNI4
line 686
;685:	//set the team goal time
;686:	bs->teamgoal_time = FloatTime() + TEAM_GETITEM_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 688
;687:	//
;688:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 692
;689:#ifdef DEBUG
;690:	BotPrintTeamGoal(bs);
;691:#endif //DEBUG
;692:}
LABELV $277
endproc BotMatch_GetItem 540 16
export BotMatch_Camp
proc BotMatch_Camp 688 16
line 699
;693:
;694:/*
;695:==================
;696:BotMatch_Camp
;697:==================
;698:*/
;699:void BotMatch_Camp(bot_state_t *bs, bot_match_t *match) {
line 705
;700:	int client, areanum;
;701:	char netname[MAX_MESSAGE_SIZE];
;702:	char itemname[MAX_MESSAGE_SIZE];
;703:	aas_entityinfo_t entinfo;
;704:
;705:	if (!TeamPlayIsOn()) return;
ADDRLP4 660
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 660
INDIRI4
CNSTI4 0
NEI4 $285
ADDRGP4 $284
JUMPV
LABELV $285
line 707
;706:	//if not addressed to this bot
;707:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 664
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 664
INDIRI4
CNSTI4 0
NEI4 $287
ADDRGP4 $284
JUMPV
LABELV $287
line 709
;708:	//
;709:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 711
;710:	//asked for someone else
;711:	client = FindClientByName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 668
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 668
INDIRI4
ASGNI4
line 713
;712:	//if there's no valid client with this name
;713:	if (client < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $289
line 714
;714:		BotAI_BotInitialChat(bs, "whois", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 715
;715:		trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
ADDRLP4 672
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 672
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 672
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 716
;716:		return;
ADDRGP4 $284
JUMPV
LABELV $289
line 719
;717:	}
;718:	//get the match variable
;719:	trap_BotMatchVariable(match, KEYAREA, itemname, sizeof(itemname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 721
;720:	//in CTF it could be the base
;721:	if (match->subtype & ST_THERE) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
EQI4 $291
line 723
;722:		//camp at the spot the bot is currently standing
;723:		bs->teamgoal.entitynum = bs->entitynum;
ADDRLP4 672
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 672
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 672
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 724
;724:		bs->teamgoal.areanum = bs->areanum;
ADDRLP4 676
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 676
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 676
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ASGNI4
line 725
;725:		VectorCopy(bs->origin, bs->teamgoal.origin);
ADDRLP4 680
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 680
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 680
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 726
;726:		VectorSet(bs->teamgoal.mins, -8, -8, -8);
ADDRFP4 0
INDIRP4
CNSTI4 6640
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6644
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6648
ADDP4
CNSTF4 3238002688
ASGNF4
line 727
;727:		VectorSet(bs->teamgoal.maxs, 8, 8, 8);
ADDRFP4 0
INDIRP4
CNSTI4 6652
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6656
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6660
ADDP4
CNSTF4 1090519040
ASGNF4
line 728
;728:	}
ADDRGP4 $292
JUMPV
LABELV $291
line 729
;729:	else if (match->subtype & ST_HERE) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
EQI4 $293
line 731
;730:		//if this is the bot self
;731:		if (client == bs->client) return;
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $295
ADDRGP4 $284
JUMPV
LABELV $295
line 733
;732:		//
;733:		bs->teamgoal.entitynum = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
CNSTI4 -1
ASGNI4
line 734
;734:		BotEntityInfo(client, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 516
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 736
;735:		//if info is valid (in PVS)
;736:		if (entinfo.valid) {
ADDRLP4 516
INDIRI4
CNSTI4 0
EQI4 $297
line 737
;737:			areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 516+24
ARGP4
ADDRLP4 672
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 656
ADDRLP4 672
INDIRI4
ASGNI4
line 738
;738:			if (areanum) {// && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 656
INDIRI4
CNSTI4 0
EQI4 $300
line 741
;739:				//NOTE: just assume the bot knows where the person is
;740:				//if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, client)) {
;741:					bs->teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 742
;742:					bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 656
INDIRI4
ASGNI4
line 743
;743:					VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 516+24
INDIRB
ASGNB 12
line 744
;744:					VectorSet(bs->teamgoal.mins, -8, -8, -8);
ADDRFP4 0
INDIRP4
CNSTI4 6640
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6644
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6648
ADDP4
CNSTF4 3238002688
ASGNF4
line 745
;745:					VectorSet(bs->teamgoal.maxs, 8, 8, 8);
ADDRFP4 0
INDIRP4
CNSTI4 6652
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6656
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6660
ADDP4
CNSTF4 1090519040
ASGNF4
line 747
;746:				//}
;747:			}
LABELV $300
line 748
;748:		}
LABELV $297
line 750
;749:		//if the other is not visible
;750:		if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $294
line 751
;751:			BotAI_BotInitialChat(bs, "whereareyou", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $261
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 752
;752:			client = ClientFromName(netname);
ADDRLP4 4
ARGP4
ADDRLP4 672
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 672
INDIRI4
ASGNI4
line 753
;753:			trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 754
;754:			return;
ADDRGP4 $284
JUMPV
line 756
;755:		}
;756:	}
LABELV $293
line 757
;757:	else if (!BotGetMessageTeamGoal(bs, itemname, &bs->teamgoal)) {
ADDRLP4 672
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 672
INDIRP4
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 672
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
ADDRLP4 676
ADDRGP4 BotGetMessageTeamGoal
CALLI4
ASGNI4
ADDRLP4 676
INDIRI4
CNSTI4 0
NEI4 $305
line 761
;758:		//BotAI_BotInitialChat(bs, "cannotfind", itemname, NULL);
;759:		//client = ClientFromName(netname);
;760:		//trap_BotEnterChat(bs->cs, client, CHAT_TELL);
;761:		return;
ADDRGP4 $284
JUMPV
LABELV $305
LABELV $294
LABELV $292
line 764
;762:	}
;763:	//
;764:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 765
;765:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 766
;766:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 768
;767:	//set the time to send a message to the team mates
;768:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 680
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
ADDRLP4 680
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 770
;769:	//set the ltg type
;770:	bs->ltgtype = LTG_CAMPORDER;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 8
ASGNI4
line 772
;771:	//get the team goal time
;772:	bs->teamgoal_time = BotGetTime(match);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 684
ADDRGP4 BotGetTime
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRLP4 684
INDIRF4
ASGNF4
line 774
;773:	//set the team goal time
;774:	if (!bs->teamgoal_time) bs->teamgoal_time = FloatTime() + TEAM_CAMP_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
CNSTF4 0
NEF4 $307
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
LABELV $307
line 776
;775:	//not arrived yet
;776:	bs->arrive_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 0
ASGNF4
line 778
;777:	//
;778:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 780
;779:	// remember last ordered task
;780:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 784
;781:#ifdef DEBUG
;782:	BotPrintTeamGoal(bs);
;783:#endif //DEBUG
;784:}
LABELV $284
endproc BotMatch_Camp 688 16
export BotMatch_Patrol
proc BotMatch_Patrol 284 16
line 791
;785:
;786:/*
;787:==================
;788:BotMatch_Patrol
;789:==================
;790:*/
;791:void BotMatch_Patrol(bot_state_t *bs, bot_match_t *match) {
line 795
;792:	char netname[MAX_MESSAGE_SIZE];
;793:	int client;
;794:
;795:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $310
ADDRGP4 $309
JUMPV
LABELV $310
line 797
;796:	//if not addressed to this bot
;797:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 264
INDIRI4
CNSTI4 0
NEI4 $312
ADDRGP4 $309
JUMPV
LABELV $312
line 799
;798:	//get the patrol waypoints
;799:	if (!BotGetPatrolWaypoints(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 268
ADDRGP4 BotGetPatrolWaypoints
CALLI4
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 0
NEI4 $314
ADDRGP4 $309
JUMPV
LABELV $314
line 801
;800:	//
;801:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 803
;802:	//
;803:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 272
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 272
INDIRI4
ASGNI4
line 805
;804:	//
;805:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 806
;806:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 807
;807:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 809
;808:	//set the time to send a message to the team mates
;809:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 276
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
ADDRLP4 276
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 811
;810:	//set the ltg type
;811:	bs->ltgtype = LTG_PATROL;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 9
ASGNI4
line 813
;812:	//get the team goal time
;813:	bs->teamgoal_time = BotGetTime(match);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 280
ADDRGP4 BotGetTime
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRLP4 280
INDIRF4
ASGNF4
line 815
;814:	//set the team goal time if not set already
;815:	if (!bs->teamgoal_time) bs->teamgoal_time = FloatTime() + TEAM_PATROL_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
CNSTF4 0
NEF4 $316
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
LABELV $316
line 817
;816:	//
;817:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 819
;818:	// remember last ordered task
;819:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 823
;820:#ifdef DEBUG
;821:	BotPrintTeamGoal(bs);
;822:#endif //DEBUG
;823:}
LABELV $309
endproc BotMatch_Patrol 284 16
export BotMatch_GetFlag
proc BotMatch_GetFlag 276 16
line 830
;824:
;825:/*
;826:==================
;827:BotMatch_GetFlag
;828:==================
;829:*/
;830:void BotMatch_GetFlag(bot_state_t *bs, bot_match_t *match) {
line 834
;831:	char netname[MAX_MESSAGE_SIZE];
;832:	int client;
;833:
;834:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $318
line 835
;835:		if (!ctf_redflag.areanum || !ctf_blueflag.areanum)
ADDRLP4 260
CNSTI4 0
ASGNI4
ADDRGP4 ctf_redflag+12
INDIRI4
ADDRLP4 260
INDIRI4
EQI4 $325
ADDRGP4 ctf_blueflag+12
INDIRI4
ADDRLP4 260
INDIRI4
NEI4 $320
LABELV $325
line 836
;836:			return;
ADDRGP4 $318
JUMPV
line 837
;837:	}
line 844
;838:#ifdef MISSIONPACK
;839:	else if (gametype == GT_1FCTF) {
;840:		if (!ctf_neutralflag.areanum || !ctf_redflag.areanum || !ctf_blueflag.areanum)
;841:			return;
;842:	}
;843:#endif
;844:	else {
line 845
;845:		return;
LABELV $320
line 848
;846:	}
;847:	//if not addressed to this bot
;848:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $326
ADDRGP4 $318
JUMPV
LABELV $326
line 850
;849:	//
;850:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 852
;851:	//
;852:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 264
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 264
INDIRI4
ASGNI4
line 854
;853:	//
;854:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 855
;855:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 856
;856:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 858
;857:	//set the time to send a message to the team mates
;858:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 268
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
ADDRLP4 268
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 860
;859:	//set the ltg type
;860:	bs->ltgtype = LTG_GETFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 4
ASGNI4
line 862
;861:	//set the team goal time
;862:	bs->teamgoal_time = FloatTime() + CTF_GETFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 864
;863:	// get an alternate route in ctf
;864:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $328
line 866
;865:		//get an alternative route goal towards the enemy base
;866:		BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 272
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 272
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 867
;867:	}
LABELV $328
line 869
;868:	//
;869:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 871
;870:	// remember last ordered task
;871:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 875
;872:#ifdef DEBUG
;873:	BotPrintTeamGoal(bs);
;874:#endif //DEBUG
;875:}
LABELV $318
endproc BotMatch_GetFlag 276 16
export BotMatch_AttackEnemyBase
proc BotMatch_AttackEnemyBase 272 16
line 882
;876:
;877:/*
;878:==================
;879:BotMatch_AttackEnemyBase
;880:==================
;881:*/
;882:void BotMatch_AttackEnemyBase(bot_state_t *bs, bot_match_t *match) {
line 886
;883:	char netname[MAX_MESSAGE_SIZE];
;884:	int client;
;885:
;886:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $330
line 887
;887:		BotMatch_GetFlag(bs, match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotMatch_GetFlag
CALLV
pop
line 888
;888:	}
line 895
;889:#ifdef MISSIONPACK
;890:	else if (gametype == GT_1FCTF || gametype == GT_OBELISK || gametype == GT_HARVESTER) {
;891:		if (!redobelisk.areanum || !blueobelisk.areanum)
;892:			return;
;893:	}
;894:#endif
;895:	else {
line 896
;896:		return;
LABELV $332
line 899
;897:	}
;898:	//if not addressed to this bot
;899:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $333
ADDRGP4 $330
JUMPV
LABELV $333
line 901
;900:	//
;901:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 903
;902:	//
;903:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 264
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 264
INDIRI4
ASGNI4
line 905
;904:	//
;905:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 906
;906:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 907
;907:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 909
;908:	//set the time to send a message to the team mates
;909:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 268
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
ADDRLP4 268
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 911
;910:	//set the ltg type
;911:	bs->ltgtype = LTG_ATTACKENEMYBASE;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 13
ASGNI4
line 913
;912:	//set the team goal time
;913:	bs->teamgoal_time = FloatTime() + TEAM_ATTACKENEMYBASE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 914
;914:	bs->attackaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6156
ADDP4
CNSTF4 0
ASGNF4
line 916
;915:	//
;916:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 918
;917:	// remember last ordered task
;918:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 922
;919:#ifdef DEBUG
;920:	BotPrintTeamGoal(bs);
;921:#endif //DEBUG
;922:}
LABELV $330
endproc BotMatch_AttackEnemyBase 272 16
export BotMatch_RushBase
proc BotMatch_RushBase 272 16
line 973
;923:
;924:#ifdef MISSIONPACK
;925:/*
;926:==================
;927:BotMatch_Harvest
;928:==================
;929:*/
;930:void BotMatch_Harvest(bot_state_t *bs, bot_match_t *match) {
;931:	char netname[MAX_MESSAGE_SIZE];
;932:	int client;
;933:
;934:	if (gametype == GT_HARVESTER) {
;935:		if (!neutralobelisk.areanum || !redobelisk.areanum || !blueobelisk.areanum)
;936:			return;
;937:	}
;938:	else {
;939:		return;
;940:	}
;941:	//if not addressed to this bot
;942:	if (!BotAddressedToBot(bs, match)) return;
;943:	//
;944:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
;945:	//
;946:	client = FindClientByName(netname);
;947:	//
;948:	bs->decisionmaker = client;
;949:	bs->ordered = qtrue;
;950:	bs->order_time = FloatTime();
;951:	//set the time to send a message to the team mates
;952:	bs->teammessage_time = FloatTime() + 2 * random();
;953:	//set the ltg type
;954:	bs->ltgtype = LTG_HARVEST;
;955:	//set the team goal time
;956:	bs->teamgoal_time = FloatTime() + TEAM_HARVEST_TIME;
;957:	bs->harvestaway_time = 0;
;958:	//
;959:	BotSetTeamStatus(bs);
;960:	// remember last ordered task
;961:	BotRememberLastOrderedTask(bs);
;962:#ifdef DEBUG
;963:	BotPrintTeamGoal(bs);
;964:#endif //DEBUG
;965:}
;966:#endif
;967:
;968:/*
;969:==================
;970:BotMatch_RushBase
;971:==================
;972:*/
;973:void BotMatch_RushBase(bot_state_t *bs, bot_match_t *match) {
line 977
;974:	char netname[MAX_MESSAGE_SIZE];
;975:	int client;
;976:
;977:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $335
line 978
;978:		if (!ctf_redflag.areanum || !ctf_blueflag.areanum)
ADDRLP4 260
CNSTI4 0
ASGNI4
ADDRGP4 ctf_redflag+12
INDIRI4
ADDRLP4 260
INDIRI4
EQI4 $342
ADDRGP4 ctf_blueflag+12
INDIRI4
ADDRLP4 260
INDIRI4
NEI4 $337
LABELV $342
line 979
;979:			return;
ADDRGP4 $335
JUMPV
line 980
;980:	}
line 987
;981:#ifdef MISSIONPACK
;982:	else if (gametype == GT_1FCTF || gametype == GT_HARVESTER) {
;983:		if (!redobelisk.areanum || !blueobelisk.areanum)
;984:			return;
;985:	}
;986:#endif
;987:	else {
line 988
;988:		return;
LABELV $337
line 991
;989:	}
;990:	//if not addressed to this bot
;991:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $343
ADDRGP4 $335
JUMPV
LABELV $343
line 993
;992:	//
;993:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 995
;994:	//
;995:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 264
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 264
INDIRI4
ASGNI4
line 997
;996:	//
;997:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 998
;998:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 999
;999:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1001
;1000:	//set the time to send a message to the team mates
;1001:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 268
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
ADDRLP4 268
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1003
;1002:	//set the ltg type
;1003:	bs->ltgtype = LTG_RUSHBASE;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 5
ASGNI4
line 1005
;1004:	//set the team goal time
;1005:	bs->teamgoal_time = FloatTime() + CTF_RUSHBASE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1123024896
ADDF4
ASGNF4
line 1006
;1006:	bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 1008
;1007:	//
;1008:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 1012
;1009:#ifdef DEBUG
;1010:	BotPrintTeamGoal(bs);
;1011:#endif //DEBUG
;1012:}
LABELV $335
endproc BotMatch_RushBase 272 16
export BotMatch_TaskPreference
proc BotMatch_TaskPreference 320 16
line 1019
;1013:
;1014:/*
;1015:==================
;1016:BotMatch_TaskPreference
;1017:==================
;1018:*/
;1019:void BotMatch_TaskPreference(bot_state_t *bs, bot_match_t *match) {
line 1024
;1020:	char netname[MAX_NETNAME];
;1021:	char teammatename[MAX_MESSAGE_SIZE];
;1022:	int teammate, preference;
;1023:
;1024:	ClientName(bs->client, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 1025
;1025:	if (Q_stricmp(netname, bs->teamleader) != 0) return;
ADDRLP4 260
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 300
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 300
INDIRI4
CNSTI4 0
EQI4 $346
ADDRGP4 $345
JUMPV
LABELV $346
line 1027
;1026:
;1027:	trap_BotMatchVariable(match, NETNAME, teammatename, sizeof(teammatename));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1028
;1028:	teammate = ClientFromName(teammatename);
ADDRLP4 4
ARGP4
ADDRLP4 304
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 304
INDIRI4
ASGNI4
line 1029
;1029:	if (teammate < 0) return;
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $348
ADDRGP4 $345
JUMPV
LABELV $348
line 1031
;1030:
;1031:	preference = BotGetTeamMateTaskPreference(bs, teammate);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 308
ADDRGP4 BotGetTeamMateTaskPreference
CALLI4
ASGNI4
ADDRLP4 296
ADDRLP4 308
INDIRI4
ASGNI4
line 1032
;1032:	switch(match->subtype)
ADDRLP4 312
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
ASGNI4
ADDRLP4 312
INDIRI4
CNSTI4 1
EQI4 $353
ADDRLP4 312
INDIRI4
CNSTI4 2
EQI4 $354
ADDRLP4 312
INDIRI4
CNSTI4 4
EQI4 $355
ADDRGP4 $350
JUMPV
line 1033
;1033:	{
LABELV $353
line 1035
;1034:		case ST_DEFENDER:
;1035:		{
line 1036
;1036:			preference &= ~TEAMTP_ATTACKER;
ADDRLP4 296
ADDRLP4 296
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 1037
;1037:			preference |= TEAMTP_DEFENDER;
ADDRLP4 296
ADDRLP4 296
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1038
;1038:			break;
ADDRGP4 $351
JUMPV
LABELV $354
line 1041
;1039:		}
;1040:		case ST_ATTACKER:
;1041:		{
line 1042
;1042:			preference &= ~TEAMTP_DEFENDER;
ADDRLP4 296
ADDRLP4 296
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 1043
;1043:			preference |= TEAMTP_ATTACKER;
ADDRLP4 296
ADDRLP4 296
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1044
;1044:			break;
ADDRGP4 $351
JUMPV
LABELV $355
line 1047
;1045:		}
;1046:		case ST_ROAMER:
;1047:		{
line 1048
;1048:			preference &= ~(TEAMTP_ATTACKER|TEAMTP_DEFENDER);
ADDRLP4 296
ADDRLP4 296
INDIRI4
CNSTI4 -4
BANDI4
ASGNI4
line 1049
;1049:			break;
LABELV $350
LABELV $351
line 1052
;1050:		}
;1051:	}
;1052:	BotSetTeamMateTaskPreference(bs, teammate, preference);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 296
INDIRI4
ARGI4
ADDRGP4 BotSetTeamMateTaskPreference
CALLV
pop
line 1054
;1053:	//
;1054:	EasyClientName(teammate, teammatename, sizeof(teammatename));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1055
;1055:	BotAI_BotInitialChat(bs, "keepinmind", teammatename, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $356
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1056
;1056:	trap_BotEnterChat(bs->cs, teammate, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1057
;1057:	BotVoiceChatOnly(bs, teammate, VOICECHAT_YES);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $357
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 1058
;1058:	trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1048576
ARGI4
ADDRGP4 trap_EA_Action
CALLV
pop
line 1059
;1059:}
LABELV $345
endproc BotMatch_TaskPreference 320 16
export BotMatch_ReturnFlag
proc BotMatch_ReturnFlag 272 16
line 1066
;1060:
;1061:/*
;1062:==================
;1063:BotMatch_ReturnFlag
;1064:==================
;1065:*/
;1066:void BotMatch_ReturnFlag(bot_state_t *bs, bot_match_t *match) {
line 1072
;1067:	char netname[MAX_MESSAGE_SIZE];
;1068:	int client;
;1069:
;1070:	//if not in CTF mode
;1071:	if (
;1072:		gametype != GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
EQI4 $359
line 1077
;1073:#ifdef MISSIONPACK
;1074:		&& gametype != GT_1FCTF
;1075:#endif
;1076:		)
;1077:		return;
ADDRGP4 $358
JUMPV
LABELV $359
line 1079
;1078:	//if not addressed to this bot
;1079:	if (!BotAddressedToBot(bs, match))
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $361
line 1080
;1080:		return;
ADDRGP4 $358
JUMPV
LABELV $361
line 1082
;1081:	//
;1082:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1084
;1083:	//
;1084:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 264
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 264
INDIRI4
ASGNI4
line 1086
;1085:	//
;1086:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 1087
;1087:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 1088
;1088:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1090
;1089:	//set the time to send a message to the team mates
;1090:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 268
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
ADDRLP4 268
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1092
;1091:	//set the ltg type
;1092:	bs->ltgtype = LTG_RETURNFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 6
ASGNI4
line 1094
;1093:	//set the team goal time
;1094:	bs->teamgoal_time = FloatTime() + CTF_RETURNFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1127481344
ADDF4
ASGNF4
line 1095
;1095:	bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 1097
;1096:	//
;1097:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 1101
;1098:#ifdef DEBUG
;1099:	BotPrintTeamGoal(bs);
;1100:#endif //DEBUG
;1101:}
LABELV $358
endproc BotMatch_ReturnFlag 272 16
export BotMatch_JoinSubteam
proc BotMatch_JoinSubteam 528 16
line 1108
;1102:
;1103:/*
;1104:==================
;1105:BotMatch_JoinSubteam
;1106:==================
;1107:*/
;1108:void BotMatch_JoinSubteam(bot_state_t *bs, bot_match_t *match) {
line 1113
;1109:	char teammate[MAX_MESSAGE_SIZE];
;1110:	char netname[MAX_MESSAGE_SIZE];
;1111:	int client;
;1112:
;1113:	if (!TeamPlayIsOn()) return;
ADDRLP4 516
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $364
ADDRGP4 $363
JUMPV
LABELV $364
line 1115
;1114:	//if not addressed to this bot
;1115:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 520
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $366
ADDRGP4 $363
JUMPV
LABELV $366
line 1117
;1116:	//get the sub team name
;1117:	trap_BotMatchVariable(match, TEAMNAME, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1119
;1118:	//set the sub team name
;1119:	strncpy(bs->subteam, teammate, 32);
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 1120
;1120:	bs->subteam[31] = '\0';
ADDRFP4 0
INDIRP4
CNSTI4 7011
ADDP4
CNSTI1 0
ASGNI1
line 1122
;1121:	//
;1122:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1123
;1123:	BotAI_BotInitialChat(bs, "joinedteam", teammate, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $368
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1124
;1124:	client = ClientFromName(netname);
ADDRLP4 256
ARGP4
ADDRLP4 524
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 512
ADDRLP4 524
INDIRI4
ASGNI4
line 1125
;1125:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 512
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1126
;1126:}
LABELV $363
endproc BotMatch_JoinSubteam 528 16
export BotMatch_LeaveSubteam
proc BotMatch_LeaveSubteam 280 16
line 1133
;1127:
;1128:/*
;1129:==================
;1130:BotMatch_LeaveSubteam
;1131:==================
;1132:*/
;1133:void BotMatch_LeaveSubteam(bot_state_t *bs, bot_match_t *match) {
line 1137
;1134:	char netname[MAX_MESSAGE_SIZE];
;1135:	int client;
;1136:
;1137:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $370
ADDRGP4 $369
JUMPV
LABELV $370
line 1139
;1138:	//if not addressed to this bot
;1139:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 264
INDIRI4
CNSTI4 0
NEI4 $372
ADDRGP4 $369
JUMPV
LABELV $372
line 1141
;1140:	//
;1141:	if (strlen(bs->subteam))
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRLP4 268
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 0
EQI4 $374
line 1142
;1142:	{
line 1143
;1143:		BotAI_BotInitialChat(bs, "leftteam", bs->subteam, NULL);
ADDRLP4 272
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 272
INDIRP4
ARGP4
ADDRGP4 $376
ARGP4
ADDRLP4 272
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1144
;1144:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1145
;1145:		client = ClientFromName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 276
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 276
INDIRI4
ASGNI4
line 1146
;1146:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 256
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1147
;1147:	} //end if
LABELV $374
line 1148
;1148:	strcpy(bs->subteam, "");
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRGP4 $377
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1149
;1149:}
LABELV $369
endproc BotMatch_LeaveSubteam 280 16
export BotMatch_WhichTeam
proc BotMatch_WhichTeam 16 16
line 1156
;1150:
;1151:/*
;1152:==================
;1153:BotMatch_LeaveSubteam
;1154:==================
;1155:*/
;1156:void BotMatch_WhichTeam(bot_state_t *bs, bot_match_t *match) {
line 1157
;1157:	if (!TeamPlayIsOn()) return;
ADDRLP4 0
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $379
ADDRGP4 $378
JUMPV
LABELV $379
line 1159
;1158:	//if not addressed to this bot
;1159:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $381
ADDRGP4 $378
JUMPV
LABELV $381
line 1161
;1160:	//
;1161:	if (strlen(bs->subteam)) {
ADDRFP4 0
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $383
line 1162
;1162:		BotAI_BotInitialChat(bs, "inteam", bs->subteam, NULL);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $385
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 6980
ADDP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1163
;1163:	}
ADDRGP4 $384
JUMPV
LABELV $383
line 1164
;1164:	else {
line 1165
;1165:		BotAI_BotInitialChat(bs, "noteam", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $386
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1166
;1166:	}
LABELV $384
line 1167
;1167:	trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1168
;1168:}
LABELV $378
endproc BotMatch_WhichTeam 16 16
export BotMatch_CheckPoint
proc BotMatch_CheckPoint 572 24
line 1175
;1169:
;1170:/*
;1171:==================
;1172:BotMatch_CheckPoint
;1173:==================
;1174:*/
;1175:void BotMatch_CheckPoint(bot_state_t *bs, bot_match_t *match) {
line 1182
;1176:	int areanum, client;
;1177:	char buf[MAX_MESSAGE_SIZE];
;1178:	char netname[MAX_MESSAGE_SIZE];
;1179:	vec3_t position;
;1180:	bot_waypoint_t *cp;
;1181:
;1182:	if (!TeamPlayIsOn()) return;
ADDRLP4 536
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 536
INDIRI4
CNSTI4 0
NEI4 $388
ADDRGP4 $387
JUMPV
LABELV $388
line 1184
;1183:	//
;1184:	trap_BotMatchVariable(match, POSITION, buf, MAX_MESSAGE_SIZE);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1185
;1185:	VectorClear(position);
ADDRLP4 540
CNSTF4 0
ASGNF4
ADDRLP4 4+8
ADDRLP4 540
INDIRF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 540
INDIRF4
ASGNF4
ADDRLP4 4
ADDRLP4 540
INDIRF4
ASGNF4
line 1187
;1186:	//
;1187:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 276
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1188
;1188:	client = ClientFromName(netname);
ADDRLP4 276
ARGP4
ADDRLP4 544
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 532
ADDRLP4 544
INDIRI4
ASGNI4
line 1190
;1189:	//BotGPSToPosition(buf, position);
;1190:	sscanf(buf, "%f %f %f", &position[0], &position[1], &position[2]);
ADDRLP4 16
ARGP4
ADDRGP4 $392
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 4+4
ARGP4
ADDRLP4 4+8
ARGP4
ADDRGP4 sscanf
CALLI4
pop
line 1191
;1191:	position[2] += 0.5;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CNSTF4 1056964608
ADDF4
ASGNF4
line 1192
;1192:	areanum = BotPointAreaNum(position);
ADDRLP4 4
ARGP4
ADDRLP4 548
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 272
ADDRLP4 548
INDIRI4
ASGNI4
line 1193
;1193:	if (!areanum) {
ADDRLP4 272
INDIRI4
CNSTI4 0
NEI4 $396
line 1194
;1194:		if (BotAddressedToBot(bs, match)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 552
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 552
INDIRI4
CNSTI4 0
EQI4 $387
line 1195
;1195:			BotAI_BotInitialChat(bs, "checkpoint_invalid", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $400
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1196
;1196:			trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 532
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1197
;1197:		}
line 1198
;1198:		return;
ADDRGP4 $387
JUMPV
LABELV $396
line 1201
;1199:	}
;1200:	//
;1201:	trap_BotMatchVariable(match, NAME, buf, MAX_MESSAGE_SIZE);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 6
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1203
;1202:	//check if there already exists a checkpoint with this name
;1203:	cp = BotFindWayPoint(bs->checkpoints, buf);
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 552
ADDRGP4 BotFindWayPoint
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 552
INDIRP4
ASGNP4
line 1204
;1204:	if (cp) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $401
line 1205
;1205:		if (cp->next) cp->next->prev = cp->prev;
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $403
ADDRLP4 560
CNSTI4 96
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ADDRLP4 560
INDIRI4
ADDP4
ADDRLP4 0
INDIRP4
ADDRLP4 560
INDIRI4
ADDP4
INDIRP4
ASGNP4
LABELV $403
line 1206
;1206:		if (cp->prev) cp->prev->next = cp->next;
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $405
ADDRLP4 568
CNSTI4 92
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
ADDRLP4 568
INDIRI4
ADDP4
ADDRLP4 0
INDIRP4
ADDRLP4 568
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $406
JUMPV
LABELV $405
line 1207
;1207:		else bs->checkpoints = cp->next;
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
LABELV $406
line 1208
;1208:		cp->inuse = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1209
;1209:	}
LABELV $401
line 1211
;1210:	//create a new check point
;1211:	cp = BotCreateWayPoint(buf, position, areanum);
ADDRLP4 16
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 272
INDIRI4
ARGI4
ADDRLP4 556
ADDRGP4 BotCreateWayPoint
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 556
INDIRP4
ASGNP4
line 1213
;1212:	//add the check point to the bot's known chech points
;1213:	cp->next = bs->checkpoints;
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
ASGNP4
line 1214
;1214:	if (bs->checkpoints) bs->checkpoints->prev = cp;
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $407
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
INDIRP4
CNSTI4 96
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $407
line 1215
;1215:	bs->checkpoints = cp;
ADDRFP4 0
INDIRP4
CNSTI4 9072
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1217
;1216:	//
;1217:	if (BotAddressedToBot(bs, match)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 560
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 560
INDIRI4
CNSTI4 0
EQI4 $409
line 1218
;1218:		Com_sprintf(buf, sizeof(buf), "%1.0f %1.0f %1.0f", cp->goal.origin[0],
ADDRLP4 16
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $411
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRF4
ARGF4
ADDRGP4 Com_sprintf
CALLV
pop
line 1222
;1219:													cp->goal.origin[1],
;1220:													cp->goal.origin[2]);
;1221:
;1222:		BotAI_BotInitialChat(bs, "checkpoint_confirm", cp->name, buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $412
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 16
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1223
;1223:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 532
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1224
;1224:	}
LABELV $409
line 1225
;1225:}
LABELV $387
endproc BotMatch_CheckPoint 572 24
export BotMatch_FormationSpace
proc BotMatch_FormationSpace 280 16
line 1232
;1226:
;1227:/*
;1228:==================
;1229:BotMatch_FormationSpace
;1230:==================
;1231:*/
;1232:void BotMatch_FormationSpace(bot_state_t *bs, bot_match_t *match) {
line 1236
;1233:	char buf[MAX_MESSAGE_SIZE];
;1234:	float space;
;1235:
;1236:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $414
ADDRGP4 $413
JUMPV
LABELV $414
line 1238
;1237:	//if not addressed to this bot
;1238:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 264
INDIRI4
CNSTI4 0
NEI4 $416
ADDRGP4 $413
JUMPV
LABELV $416
line 1240
;1239:	//
;1240:	trap_BotMatchVariable(match, NUMBER, buf, MAX_MESSAGE_SIZE);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1242
;1241:	//if it's the distance in feet
;1242:	if (match->subtype & ST_FEET) space = 0.3048 * 32 * atof(buf);
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $418
ADDRLP4 4
ARGP4
ADDRLP4 268
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0
CNSTF4 1092357823
ADDRLP4 268
INDIRF4
MULF4
ASGNF4
ADDRGP4 $419
JUMPV
LABELV $418
line 1244
;1243:	//else it's in meters
;1244:	else space = 32 * atof(buf);
ADDRLP4 4
ARGP4
ADDRLP4 272
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0
CNSTF4 1107296256
ADDRLP4 272
INDIRF4
MULF4
ASGNF4
LABELV $419
line 1246
;1245:	//check if the formation intervening space is valid
;1246:	if (space < 48 || space > 500) space = 100;
ADDRLP4 0
INDIRF4
CNSTF4 1111490560
LTF4 $422
ADDRLP4 0
INDIRF4
CNSTF4 1140457472
LEF4 $420
LABELV $422
ADDRLP4 0
CNSTF4 1120403456
ASGNF4
LABELV $420
line 1247
;1247:	bs->formation_dist = space;
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
ADDRLP4 0
INDIRF4
ASGNF4
line 1248
;1248:}
LABELV $413
endproc BotMatch_FormationSpace 280 16
export BotMatch_Dismiss
proc BotMatch_Dismiss 272 16
line 1255
;1249:
;1250:/*
;1251:==================
;1252:BotMatch_Dismiss
;1253:==================
;1254:*/
;1255:void BotMatch_Dismiss(bot_state_t *bs, bot_match_t *match) {
line 1259
;1256:	char netname[MAX_MESSAGE_SIZE];
;1257:	int client;
;1258:
;1259:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $424
ADDRGP4 $423
JUMPV
LABELV $424
line 1261
;1260:	//if not addressed to this bot
;1261:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 264
INDIRI4
CNSTI4 0
NEI4 $426
ADDRGP4 $423
JUMPV
LABELV $426
line 1262
;1262:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1263
;1263:	client = ClientFromName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 268
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 268
INDIRI4
ASGNI4
line 1265
;1264:	//
;1265:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRLP4 256
INDIRI4
ASGNI4
line 1267
;1266:	//
;1267:	bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 1268
;1268:	bs->lead_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
CNSTF4 0
ASGNF4
line 1269
;1269:	bs->lastgoal_ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
CNSTI4 0
ASGNI4
line 1271
;1270:	//
;1271:	BotAI_BotInitialChat(bs, "dismissed", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $428
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1272
;1272:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 256
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1273
;1273:}
LABELV $423
endproc BotMatch_Dismiss 272 16
export BotMatch_Suicide
proc BotMatch_Suicide 272 16
line 1280
;1274:
;1275:/*
;1276:==================
;1277:BotMatch_Suicide
;1278:==================
;1279:*/
;1280:void BotMatch_Suicide(bot_state_t *bs, bot_match_t *match) {
line 1284
;1281:	char netname[MAX_MESSAGE_SIZE];
;1282:	int client;
;1283:
;1284:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $430
ADDRGP4 $429
JUMPV
LABELV $430
line 1286
;1285:	//if not addressed to this bot
;1286:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 264
INDIRI4
CNSTI4 0
NEI4 $432
ADDRGP4 $429
JUMPV
LABELV $432
line 1288
;1287:	//
;1288:	trap_EA_Command(bs->client, "kill");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $434
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 1290
;1289:	//
;1290:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1291
;1291:	client = ClientFromName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 268
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 268
INDIRI4
ASGNI4
line 1293
;1292:	//
;1293:	BotVoiceChat(bs, client, VOICECHAT_TAUNT);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
ARGI4
ADDRGP4 $435
ARGP4
ADDRGP4 BotVoiceChat
CALLV
pop
line 1294
;1294:	trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1048576
ARGI4
ADDRGP4 trap_EA_Action
CALLV
pop
line 1295
;1295:}
LABELV $429
endproc BotMatch_Suicide 272 16
export BotMatch_StartTeamLeaderShip
proc BotMatch_StartTeamLeaderShip 268 16
line 1302
;1296:
;1297:/*
;1298:==================
;1299:BotMatch_StartTeamLeaderShip
;1300:==================
;1301:*/
;1302:void BotMatch_StartTeamLeaderShip(bot_state_t *bs, bot_match_t *match) {
line 1306
;1303:	int client;
;1304:	char teammate[MAX_MESSAGE_SIZE];
;1305:
;1306:	if (!TeamPlayIsOn()) return;
ADDRLP4 260
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $437
ADDRGP4 $436
JUMPV
LABELV $437
line 1308
;1307:	//if chats for him or herself
;1308:	if (match->subtype & ST_I) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $439
line 1310
;1309:		//get the team mate that will be the team leader
;1310:		trap_BotMatchVariable(match, NETNAME, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1311
;1311:		strncpy(bs->teamleader, teammate, sizeof(bs->teamleader));
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 1312
;1312:		bs->teamleader[sizeof(bs->teamleader)] = '\0';
ADDRFP4 0
INDIRP4
CNSTI4 6900
CNSTU4 32
ADDI4
ADDP4
CNSTI1 0
ASGNI1
line 1313
;1313:	}
ADDRGP4 $440
JUMPV
LABELV $439
line 1315
;1314:	//chats for someone else
;1315:	else {
line 1317
;1316:		//get the team mate that will be the team leader
;1317:		trap_BotMatchVariable(match, TEAMMATE, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1318
;1318:		client = FindClientByName(teammate);
ADDRLP4 0
ARGP4
ADDRLP4 264
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 264
INDIRI4
ASGNI4
line 1319
;1319:		if (client >= 0) ClientName(client, bs->teamleader, sizeof(bs->teamleader));
ADDRLP4 256
INDIRI4
CNSTI4 0
LTI4 $441
ADDRLP4 256
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 ClientName
CALLP4
pop
LABELV $441
line 1320
;1320:	}
LABELV $440
line 1321
;1321:}
LABELV $436
endproc BotMatch_StartTeamLeaderShip 268 16
export BotMatch_StopTeamLeaderShip
proc BotMatch_StopTeamLeaderShip 528 16
line 1328
;1322:
;1323:/*
;1324:==================
;1325:BotMatch_StopTeamLeaderShip
;1326:==================
;1327:*/
;1328:void BotMatch_StopTeamLeaderShip(bot_state_t *bs, bot_match_t *match) {
line 1333
;1329:	int client;
;1330:	char teammate[MAX_MESSAGE_SIZE];
;1331:	char netname[MAX_MESSAGE_SIZE];
;1332:
;1333:	if (!TeamPlayIsOn()) return;
ADDRLP4 516
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $444
ADDRGP4 $443
JUMPV
LABELV $444
line 1335
;1334:	//get the team mate that stops being the team leader
;1335:	trap_BotMatchVariable(match, TEAMMATE, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1337
;1336:	//if chats for him or herself
;1337:	if (match->subtype & ST_I) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $446
line 1338
;1338:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1339
;1339:		client = FindClientByName(netname);
ADDRLP4 260
ARGP4
ADDRLP4 520
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 520
INDIRI4
ASGNI4
line 1340
;1340:	}
ADDRGP4 $447
JUMPV
LABELV $446
line 1342
;1341:	//chats for someone else
;1342:	else {
line 1343
;1343:		client = FindClientByName(teammate);
ADDRLP4 4
ARGP4
ADDRLP4 520
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 520
INDIRI4
ASGNI4
line 1344
;1344:	} //end else
LABELV $447
line 1345
;1345:	if (client >= 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $448
line 1346
;1346:		if (!Q_stricmp(bs->teamleader, ClientName(client, netname, sizeof(netname)))) {
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 520
ADDRGP4 ClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 520
INDIRP4
ARGP4
ADDRLP4 524
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 524
INDIRI4
CNSTI4 0
NEI4 $450
line 1347
;1347:			bs->teamleader[0] = '\0';
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
CNSTI1 0
ASGNI1
line 1348
;1348:			notleader[client] = qtrue;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 notleader
ADDP4
CNSTI4 1
ASGNI4
line 1349
;1349:		}
LABELV $450
line 1350
;1350:	}
LABELV $448
line 1351
;1351:}
LABELV $443
endproc BotMatch_StopTeamLeaderShip 528 16
export BotMatch_WhoIsTeamLeader
proc BotMatch_WhoIsTeamLeader 264 12
line 1358
;1352:
;1353:/*
;1354:==================
;1355:BotMatch_WhoIsTeamLeader
;1356:==================
;1357:*/
;1358:void BotMatch_WhoIsTeamLeader(bot_state_t *bs, bot_match_t *match) {
line 1361
;1359:	char netname[MAX_MESSAGE_SIZE];
;1360:
;1361:	if (!TeamPlayIsOn()) return;
ADDRLP4 256
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 256
INDIRI4
CNSTI4 0
NEI4 $453
ADDRGP4 $452
JUMPV
LABELV $453
line 1363
;1362:
;1363:	ClientName(bs->client, netname, sizeof(netname));
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
line 1365
;1364:	//if this bot IS the team leader
;1365:	if (!Q_stricmp(netname, bs->teamleader)) {
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 260
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $455
line 1366
;1366:		trap_EA_SayTeam(bs->client, "I'm the team leader\n");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $457
ARGP4
ADDRGP4 trap_EA_SayTeam
CALLV
pop
line 1367
;1367:	}
LABELV $455
line 1368
;1368:}
LABELV $452
endproc BotMatch_WhoIsTeamLeader 264 12
export BotMatch_WhatAreYouDoing
proc BotMatch_WhatAreYouDoing 532 16
line 1375
;1369:
;1370:/*
;1371:==================
;1372:BotMatch_WhatAreYouDoing
;1373:==================
;1374:*/
;1375:void BotMatch_WhatAreYouDoing(bot_state_t *bs, bot_match_t *match) {
line 1381
;1376:	char netname[MAX_MESSAGE_SIZE];
;1377:	char goalname[MAX_MESSAGE_SIZE];
;1378:	int client;
;1379:
;1380:	//if not addressed to this bot
;1381:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 516
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $459
ADDRGP4 $458
JUMPV
LABELV $459
line 1383
;1382:	//
;1383:	switch(bs->ltgtype) {
ADDRLP4 520
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 520
INDIRI4
CNSTI4 1
LTI4 $461
ADDRLP4 520
INDIRI4
CNSTI4 11
GTI4 $461
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $485-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $485
address $464
address $466
address $468
address $478
address $480
address $482
address $474
address $474
address $476
address $470
address $472
code
LABELV $464
line 1385
;1384:		case LTG_TEAMHELP:
;1385:		{
line 1386
;1386:			EasyClientName(bs->teammate, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1387
;1387:			BotAI_BotInitialChat(bs, "helping", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $465
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1388
;1388:			break;
ADDRGP4 $462
JUMPV
LABELV $466
line 1391
;1389:		}
;1390:		case LTG_TEAMACCOMPANY:
;1391:		{
line 1392
;1392:			EasyClientName(bs->teammate, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1393
;1393:			BotAI_BotInitialChat(bs, "accompanying", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $467
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1394
;1394:			break;
ADDRGP4 $462
JUMPV
LABELV $468
line 1397
;1395:		}
;1396:		case LTG_DEFENDKEYAREA:
;1397:		{
line 1398
;1398:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 1399
;1399:			BotAI_BotInitialChat(bs, "defending", goalname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $469
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1400
;1400:			break;
ADDRGP4 $462
JUMPV
LABELV $470
line 1403
;1401:		}
;1402:		case LTG_GETITEM:
;1403:		{
line 1404
;1404:			trap_BotGoalName(bs->teamgoal.number, goalname, sizeof(goalname));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 1405
;1405:			BotAI_BotInitialChat(bs, "gettingitem", goalname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $471
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1406
;1406:			break;
ADDRGP4 $462
JUMPV
LABELV $472
line 1409
;1407:		}
;1408:		case LTG_KILL:
;1409:		{
line 1410
;1410:			ClientName(bs->teamgoal.entitynum, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 6664
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
line 1411
;1411:			BotAI_BotInitialChat(bs, "killing", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $473
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1412
;1412:			break;
ADDRGP4 $462
JUMPV
LABELV $474
line 1416
;1413:		}
;1414:		case LTG_CAMP:
;1415:		case LTG_CAMPORDER:
;1416:		{
line 1417
;1417:			BotAI_BotInitialChat(bs, "camping", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $475
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1418
;1418:			break;
ADDRGP4 $462
JUMPV
LABELV $476
line 1421
;1419:		}
;1420:		case LTG_PATROL:
;1421:		{
line 1422
;1422:			BotAI_BotInitialChat(bs, "patrolling", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $477
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1423
;1423:			break;
ADDRGP4 $462
JUMPV
LABELV $478
line 1426
;1424:		}
;1425:		case LTG_GETFLAG:
;1426:		{
line 1427
;1427:			BotAI_BotInitialChat(bs, "capturingflag", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $479
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1428
;1428:			break;
ADDRGP4 $462
JUMPV
LABELV $480
line 1431
;1429:		}
;1430:		case LTG_RUSHBASE:
;1431:		{
line 1432
;1432:			BotAI_BotInitialChat(bs, "rushingbase", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $481
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1433
;1433:			break;
ADDRGP4 $462
JUMPV
LABELV $482
line 1436
;1434:		}
;1435:		case LTG_RETURNFLAG:
;1436:		{
line 1437
;1437:			BotAI_BotInitialChat(bs, "returningflag", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $483
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1438
;1438:			break;
ADDRGP4 $462
JUMPV
LABELV $461
line 1453
;1439:		}
;1440:#ifdef MISSIONPACK
;1441:		case LTG_ATTACKENEMYBASE:
;1442:		{
;1443:			BotAI_BotInitialChat(bs, "attackingenemybase", NULL);
;1444:			break;
;1445:		}
;1446:		case LTG_HARVEST:
;1447:		{
;1448:			BotAI_BotInitialChat(bs, "harvesting", NULL);
;1449:			break;
;1450:		}
;1451:#endif
;1452:		default:
;1453:		{
line 1454
;1454:			BotAI_BotInitialChat(bs, "roaming", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $484
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1455
;1455:			break;
LABELV $462
line 1459
;1456:		}
;1457:	}
;1458:	//chat what the bot is doing
;1459:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1460
;1460:	client = ClientFromName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 528
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 256
ADDRLP4 528
INDIRI4
ASGNI4
line 1461
;1461:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 256
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1462
;1462:}
LABELV $458
endproc BotMatch_WhatAreYouDoing 532 16
export BotMatch_WhatIsMyCommand
proc BotMatch_WhatIsMyCommand 40 12
line 1469
;1463:
;1464:/*
;1465:==================
;1466:BotMatch_WhatIsMyCommand
;1467:==================
;1468:*/
;1469:void BotMatch_WhatIsMyCommand(bot_state_t *bs, bot_match_t *match) {
line 1472
;1470:	char netname[MAX_NETNAME];
;1471:
;1472:	ClientName(bs->client, netname, sizeof(netname));
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
line 1473
;1473:	if (Q_stricmp(netname, bs->teamleader) != 0) return;
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $488
ADDRGP4 $487
JUMPV
LABELV $488
line 1474
;1474:	bs->forceorders = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6968
ADDP4
CNSTI4 1
ASGNI4
line 1475
;1475:}
LABELV $487
endproc BotMatch_WhatIsMyCommand 40 12
export BotNearestVisibleItem
proc BotNearestVisibleItem 252 28
line 1482
;1476:
;1477:/*
;1478:==================
;1479:BotNearestVisibleItem
;1480:==================
;1481:*/
;1482:float BotNearestVisibleItem(bot_state_t *bs, char *itemname, bot_goal_t *goal) {
line 1490
;1483:	int i;
;1484:	char name[64];
;1485:	bot_goal_t tmpgoal;
;1486:	float dist, bestdist;
;1487:	vec3_t dir;
;1488:	bsp_trace_t trace;
;1489:
;1490:	bestdist = 999999;
ADDRLP4 140
CNSTF4 1232348144
ASGNF4
line 1491
;1491:	i = -1;
ADDRLP4 68
CNSTI4 -1
ASGNI4
LABELV $491
line 1492
;1492:	do {
line 1493
;1493:		i = trap_BotGetLevelItemGoal(i, itemname, &tmpgoal);
ADDRLP4 68
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 228
ADDRGP4 trap_BotGetLevelItemGoal
CALLI4
ASGNI4
ADDRLP4 68
ADDRLP4 228
INDIRI4
ASGNI4
line 1494
;1494:		trap_BotGoalName(tmpgoal.number, name, sizeof(name));
ADDRLP4 0+44
INDIRI4
ARGI4
ADDRLP4 72
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 1495
;1495:		if (Q_stricmp(itemname, name) != 0)
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 72
ARGP4
ADDRLP4 232
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 232
INDIRI4
CNSTI4 0
EQI4 $495
line 1496
;1496:			continue;
ADDRGP4 $492
JUMPV
LABELV $495
line 1497
;1497:		VectorSubtract(tmpgoal.origin, bs->origin, dir);
ADDRLP4 236
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 0
INDIRF4
ADDRLP4 236
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 236
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 0+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1498
;1498:		dist = VectorLength(dir);
ADDRLP4 56
ARGP4
ADDRLP4 240
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 136
ADDRLP4 240
INDIRF4
ASGNF4
line 1499
;1499:		if (dist < bestdist) {
ADDRLP4 136
INDIRF4
ADDRLP4 140
INDIRF4
GEF4 $501
line 1501
;1500:			//trace from start to end
;1501:			BotAI_Trace(&trace, bs->eye, NULL, NULL, tmpgoal.origin, bs->client, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
ADDRLP4 144
ARGP4
ADDRLP4 244
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 244
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 248
CNSTP4 0
ASGNP4
ADDRLP4 248
INDIRP4
ARGP4
ADDRLP4 248
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 244
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
line 1502
;1502:			if (trace.fraction >= 1.0) {
ADDRLP4 144+8
INDIRF4
CNSTF4 1065353216
LTF4 $503
line 1503
;1503:				bestdist = dist;
ADDRLP4 140
ADDRLP4 136
INDIRF4
ASGNF4
line 1504
;1504:				memcpy(goal, &tmpgoal, sizeof(bot_goal_t));
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1505
;1505:			}
LABELV $503
line 1506
;1506:		}
LABELV $501
line 1507
;1507:	} while(i > 0);
LABELV $492
ADDRLP4 68
INDIRI4
CNSTI4 0
GTI4 $491
line 1508
;1508:	return bestdist;
ADDRLP4 140
INDIRF4
RETF4
LABELV $490
endproc BotNearestVisibleItem 252 28
lit
align 4
LABELV $507
address $508
address $509
address $510
address $511
address $512
address $513
address $514
address $515
address $516
address $517
address $518
address $519
address $520
address $521
address $522
address $523
address $524
byte 4 0
export BotMatch_WhereAreYou
code
proc BotMatch_WhereAreYou 444 20
line 1516
;1509:}
;1510:
;1511:/*
;1512:==================
;1513:BotMatch_WhereAreYou
;1514:==================
;1515:*/
;1516:void BotMatch_WhereAreYou(bot_state_t *bs, bot_match_t *match) {
line 1521
;1517:	float dist, bestdist;
;1518:	int i, bestitem, redtt, bluett, client;
;1519:	bot_goal_t goal;
;1520:	char netname[MAX_MESSAGE_SIZE];
;1521:	char *nearbyitems[] = {
ADDRLP4 8
ADDRGP4 $507
INDIRB
ASGNB 72
line 1555
;1522:		"Shotgun",
;1523:		"Grenade Launcher",
;1524:		"Rocket Launcher",
;1525:		"Plasmagun",
;1526:		"Railgun",
;1527:		"Lightning Gun",
;1528:		"BFG10K",
;1529:		"Quad Damage",
;1530:		"Regeneration",
;1531:		"Battle Suit",
;1532:		"Speed",
;1533:		"Invisibility",
;1534:		"Flight",
;1535:		"Armor",
;1536:		"Heavy Armor",
;1537:		"Red Flag",
;1538:		"Blue Flag",
;1539:#ifdef MISSIONPACK
;1540:		"Nailgun",
;1541:		"Prox Launcher",
;1542:		"Chaingun",
;1543:		"Scout",
;1544:		"Guard",
;1545:		"Doubler",
;1546:		"Ammo Regen",
;1547:		"Neutral Flag",
;1548:		"Red Obelisk",
;1549:		"Blue Obelisk",
;1550:		"Neutral Obelisk",
;1551:#endif
;1552:		NULL
;1553:	};
;1554:	//
;1555:	if (!TeamPlayIsOn())
ADDRLP4 412
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 412
INDIRI4
CNSTI4 0
NEI4 $525
line 1556
;1556:		return;
ADDRGP4 $506
JUMPV
LABELV $525
line 1558
;1557:	//if not addressed to this bot
;1558:	if (!BotAddressedToBot(bs, match))
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 416
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 416
INDIRI4
CNSTI4 0
NEI4 $527
line 1559
;1559:		return;
ADDRGP4 $506
JUMPV
LABELV $527
line 1561
;1560:
;1561:	bestitem = -1;
ADDRLP4 140
CNSTI4 -1
ASGNI4
line 1562
;1562:	bestdist = 999999;
ADDRLP4 80
CNSTF4 1232348144
ASGNF4
line 1563
;1563:	for (i = 0; nearbyitems[i]; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $532
JUMPV
LABELV $529
line 1564
;1564:		dist = BotNearestVisibleItem(bs, nearbyitems[i], &goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 420
ADDRGP4 BotNearestVisibleItem
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 420
INDIRF4
ASGNF4
line 1565
;1565:		if (dist < bestdist) {
ADDRLP4 4
INDIRF4
ADDRLP4 80
INDIRF4
GEF4 $533
line 1566
;1566:			bestdist = dist;
ADDRLP4 80
ADDRLP4 4
INDIRF4
ASGNF4
line 1567
;1567:			bestitem = i;
ADDRLP4 140
ADDRLP4 0
INDIRI4
ASGNI4
line 1568
;1568:		}
LABELV $533
line 1569
;1569:	}
LABELV $530
line 1563
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $532
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $529
line 1570
;1570:	if (bestitem != -1) {
ADDRLP4 140
INDIRI4
CNSTI4 -1
EQI4 $535
line 1571
;1571:		if (gametype == GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $537
line 1575
;1572:#ifdef MISSIONPACK
;1573:			|| gametype == GT_1FCTF
;1574:#endif
;1575:			) {
line 1576
;1576:			redtt = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, ctf_redflag.areanum, TFL_DEFAULT);
ADDRLP4 420
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 420
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 420
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRGP4 ctf_redflag+12
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 424
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 404
ADDRLP4 424
INDIRI4
ASGNI4
line 1577
;1577:			bluett = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, ctf_blueflag.areanum, TFL_DEFAULT);
ADDRLP4 428
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 428
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 428
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRGP4 ctf_blueflag+12
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 432
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 408
ADDRLP4 432
INDIRI4
ASGNI4
line 1578
;1578:			if (redtt < (redtt + bluett) * 0.4) {
ADDRLP4 436
ADDRLP4 404
INDIRI4
ASGNI4
ADDRLP4 436
INDIRI4
CVIF4 4
CNSTF4 1053609165
ADDRLP4 436
INDIRI4
ADDRLP4 408
INDIRI4
ADDI4
CVIF4 4
MULF4
GEF4 $541
line 1579
;1579:				BotAI_BotInitialChat(bs, "teamlocation", nearbyitems[bestitem], "red", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $543
ARGP4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
ARGP4
ADDRGP4 $544
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1580
;1580:			}
ADDRGP4 $538
JUMPV
LABELV $541
line 1581
;1581:			else if (bluett < (redtt + bluett) * 0.4) {
ADDRLP4 440
ADDRLP4 408
INDIRI4
ASGNI4
ADDRLP4 440
INDIRI4
CVIF4 4
CNSTF4 1053609165
ADDRLP4 404
INDIRI4
ADDRLP4 440
INDIRI4
ADDI4
CVIF4 4
MULF4
GEF4 $545
line 1582
;1582:				BotAI_BotInitialChat(bs, "teamlocation", nearbyitems[bestitem], "blue", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $543
ARGP4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
ARGP4
ADDRGP4 $547
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1583
;1583:			}
ADDRGP4 $538
JUMPV
LABELV $545
line 1584
;1584:			else {
line 1585
;1585:				BotAI_BotInitialChat(bs, "location", nearbyitems[bestitem], NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $548
ARGP4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1586
;1586:			}
line 1587
;1587:		}
ADDRGP4 $538
JUMPV
LABELV $537
line 1603
;1588:#ifdef MISSIONPACK
;1589:		else if (gametype == GT_OBELISK || gametype == GT_HARVESTER) {
;1590:			redtt = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, redobelisk.areanum, TFL_DEFAULT);
;1591:			bluett = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, blueobelisk.areanum, TFL_DEFAULT);
;1592:			if (redtt < (redtt + bluett) * 0.4) {
;1593:				BotAI_BotInitialChat(bs, "teamlocation", nearbyitems[bestitem], "red", NULL);
;1594:			}
;1595:			else if (bluett < (redtt + bluett) * 0.4) {
;1596:				BotAI_BotInitialChat(bs, "teamlocation", nearbyitems[bestitem], "blue", NULL);
;1597:			}
;1598:			else {
;1599:				BotAI_BotInitialChat(bs, "location", nearbyitems[bestitem], NULL);
;1600:			}
;1601:		}
;1602:#endif
;1603:		else {
line 1604
;1604:			BotAI_BotInitialChat(bs, "location", nearbyitems[bestitem], NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $548
ARGP4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1605
;1605:		}
LABELV $538
line 1606
;1606:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 144
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1607
;1607:		client = ClientFromName(netname);
ADDRLP4 144
ARGP4
ADDRLP4 420
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 400
ADDRLP4 420
INDIRI4
ASGNI4
line 1608
;1608:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 400
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1609
;1609:	}
LABELV $535
line 1610
;1610:}
LABELV $506
endproc BotMatch_WhereAreYou 444 20
export BotMatch_LeadTheWay
proc BotMatch_LeadTheWay 680 16
line 1617
;1611:
;1612:/*
;1613:==================
;1614:BotMatch_LeadTheWay
;1615:==================
;1616:*/
;1617:void BotMatch_LeadTheWay(bot_state_t *bs, bot_match_t *match) {
line 1622
;1618:	aas_entityinfo_t entinfo;
;1619:	char netname[MAX_MESSAGE_SIZE], teammate[MAX_MESSAGE_SIZE];
;1620:	int client, areanum, other;
;1621:
;1622:	if (!TeamPlayIsOn()) return;
ADDRLP4 664
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 664
INDIRI4
CNSTI4 0
NEI4 $550
ADDRGP4 $549
JUMPV
LABELV $550
line 1624
;1623:	//if not addressed to this bot
;1624:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 668
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 668
INDIRI4
CNSTI4 0
NEI4 $552
ADDRGP4 $549
JUMPV
LABELV $552
line 1626
;1625:	//if someone asks for someone else
;1626:	if (match->subtype & ST_SOMEONE) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $554
line 1628
;1627:		//get the team mate name
;1628:		trap_BotMatchVariable(match, TEAMMATE, teammate, sizeof(teammate));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 400
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1629
;1629:		client = FindClientByName(teammate);
ADDRLP4 400
ARGP4
ADDRLP4 672
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 672
INDIRI4
ASGNI4
line 1631
;1630:		//if this is the bot self
;1631:		if (client == bs->client) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $556
line 1632
;1632:			other = qfalse;
ADDRLP4 656
CNSTI4 0
ASGNI4
line 1633
;1633:		}
ADDRGP4 $555
JUMPV
LABELV $556
line 1634
;1634:		else if (!BotSameTeam(bs, client)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 676
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 676
INDIRI4
CNSTI4 0
NEI4 $558
line 1636
;1635:			//FIXME: say "I don't help the enemy"
;1636:			return;
ADDRGP4 $549
JUMPV
LABELV $558
line 1638
;1637:		}
;1638:		else {
line 1639
;1639:			other = qtrue;
ADDRLP4 656
CNSTI4 1
ASGNI4
line 1640
;1640:		}
line 1641
;1641:	}
ADDRGP4 $555
JUMPV
LABELV $554
line 1642
;1642:	else {
line 1644
;1643:		//get the netname
;1644:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 144
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1645
;1645:		client = ClientFromName(netname);
ADDRLP4 144
ARGP4
ADDRLP4 672
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 672
INDIRI4
ASGNI4
line 1646
;1646:		other = qfalse;
ADDRLP4 656
CNSTI4 0
ASGNI4
line 1647
;1647:	}
LABELV $555
line 1649
;1648:	//if the bot doesn't know who to help (FindClientByName returned -1)
;1649:	if (client < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $560
line 1650
;1650:		BotAI_BotInitialChat(bs, "whois", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 144
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1651
;1651:		trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
ADDRLP4 672
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 672
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 672
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1652
;1652:		return;
ADDRGP4 $549
JUMPV
LABELV $560
line 1655
;1653:	}
;1654:	//
;1655:	bs->lead_teamgoal.entitynum = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6868
ADDP4
CNSTI4 -1
ASGNI4
line 1656
;1656:	BotEntityInfo(client, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 1658
;1657:	//if info is valid (in PVS)
;1658:	if (entinfo.valid) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $562
line 1659
;1659:		areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 4+24
ARGP4
ADDRLP4 672
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 660
ADDRLP4 672
INDIRI4
ASGNI4
line 1660
;1660:		if (areanum) { // && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 660
INDIRI4
CNSTI4 0
EQI4 $565
line 1661
;1661:			bs->lead_teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6868
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1662
;1662:			bs->lead_teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6840
ADDP4
ADDRLP4 660
INDIRI4
ASGNI4
line 1663
;1663:			VectorCopy(entinfo.origin, bs->lead_teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6828
ADDP4
ADDRLP4 4+24
INDIRB
ASGNB 12
line 1664
;1664:			VectorSet(bs->lead_teamgoal.mins, -8, -8, -8);
ADDRFP4 0
INDIRP4
CNSTI4 6844
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6848
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6852
ADDP4
CNSTF4 3238002688
ASGNF4
line 1665
;1665:			VectorSet(bs->lead_teamgoal.maxs, 8, 8, 8);
ADDRFP4 0
INDIRP4
CNSTI4 6856
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6860
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6864
ADDP4
CNSTF4 1090519040
ASGNF4
line 1666
;1666:		}
LABELV $565
line 1667
;1667:	}
LABELV $562
line 1669
;1668:
;1669:	if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $568
line 1670
;1670:		if (other) BotAI_BotInitialChat(bs, "whereis", teammate, NULL);
ADDRLP4 656
INDIRI4
CNSTI4 0
EQI4 $570
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $260
ARGP4
ADDRLP4 400
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $571
JUMPV
LABELV $570
line 1671
;1671:		else BotAI_BotInitialChat(bs, "whereareyou", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $261
ARGP4
ADDRLP4 144
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
LABELV $571
line 1672
;1672:		trap_BotEnterChat(bs->cs, bs->client, CHAT_TEAM);
ADDRLP4 672
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 672
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 672
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1673
;1673:		return;
ADDRGP4 $549
JUMPV
LABELV $568
line 1675
;1674:	}
;1675:	bs->lead_teammate = client;
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1676
;1676:	bs->lead_time = FloatTime() + TEAM_LEAD_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 1677
;1677:	bs->leadvisible_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6888
ADDP4
CNSTF4 0
ASGNF4
line 1678
;1678:	bs->leadmessage_time = -(FloatTime() + 2 * random());
ADDRLP4 672
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
ADDRLP4 672
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
NEGF4
ASGNF4
line 1679
;1679:}
LABELV $549
endproc BotMatch_LeadTheWay 680 16
export BotMatch_Kill
proc BotMatch_Kill 532 16
line 1686
;1680:
;1681:/*
;1682:==================
;1683:BotMatch_Kill
;1684:==================
;1685:*/
;1686:void BotMatch_Kill(bot_state_t *bs, bot_match_t *match) {
line 1691
;1687:	char enemy[MAX_MESSAGE_SIZE];
;1688:	char netname[MAX_MESSAGE_SIZE];
;1689:	int client;
;1690:
;1691:	if (!TeamPlayIsOn()) return;
ADDRLP4 516
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 516
INDIRI4
CNSTI4 0
NEI4 $573
ADDRGP4 $572
JUMPV
LABELV $573
line 1693
;1692:	//if not addressed to this bot
;1693:	if (!BotAddressedToBot(bs, match)) return;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 520
ADDRGP4 BotAddressedToBot
CALLI4
ASGNI4
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $575
ADDRGP4 $572
JUMPV
LABELV $575
line 1695
;1694:
;1695:	trap_BotMatchVariable(match, ENEMY, enemy, sizeof(enemy));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1697
;1696:	//
;1697:	client = FindEnemyByName(bs, enemy);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 524
ADDRGP4 FindEnemyByName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 524
INDIRI4
ASGNI4
line 1698
;1698:	if (client < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $577
line 1699
;1699:		BotAI_BotInitialChat(bs, "whois", enemy, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1700
;1700:		trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1701
;1701:		client = ClientFromName(netname);
ADDRLP4 260
ARGP4
ADDRLP4 528
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 528
INDIRI4
ASGNI4
line 1702
;1702:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1703
;1703:		return;
ADDRGP4 $572
JUMPV
LABELV $577
line 1705
;1704:	}
;1705:	bs->teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1707
;1706:	//set the time to send a message to the team mates
;1707:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 528
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
ADDRLP4 528
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1709
;1708:	//set the ltg type
;1709:	bs->ltgtype = LTG_KILL;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 11
ASGNI4
line 1711
;1710:	//set the team goal time
;1711:	bs->teamgoal_time = FloatTime() + TEAM_KILL_SOMEONE;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1127481344
ADDF4
ASGNF4
line 1713
;1712:	//
;1713:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 1717
;1714:#ifdef DEBUG
;1715:	BotPrintTeamGoal(bs);
;1716:#endif //DEBUG
;1717:}
LABELV $572
endproc BotMatch_Kill 532 16
export BotMatch_CTF
proc BotMatch_CTF 176 16
line 1724
;1718:
;1719:/*
;1720:==================
;1721:BotMatch_CTF
;1722:==================
;1723:*/
;1724:void BotMatch_CTF(bot_state_t *bs, bot_match_t *match) {
line 1728
;1725:
;1726:	char flag[128], netname[MAX_NETNAME];
;1727:
;1728:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $580
line 1729
;1729:		trap_BotMatchVariable(match, FLAG, flag, sizeof(flag));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1730
;1730:		if (match->subtype & ST_GOTFLAG) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $582
line 1731
;1731:			if (!Q_stricmp(flag, "red")) {
ADDRLP4 0
ARGP4
ADDRGP4 $544
ARGP4
ADDRLP4 164
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
NEI4 $584
line 1732
;1732:				bs->redflagstatus = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 1
ASGNI4
line 1733
;1733:				if (BotTeam(bs) == TEAM_BLUE) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 168
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 2
NEI4 $585
line 1734
;1734:					trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 128
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1735
;1735:					bs->flagcarrier = ClientFromName(netname);
ADDRLP4 128
ARGP4
ADDRLP4 172
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
ADDRLP4 172
INDIRI4
ASGNI4
line 1736
;1736:				}
line 1737
;1737:			}
ADDRGP4 $585
JUMPV
LABELV $584
line 1738
;1738:			else {
line 1739
;1739:				bs->blueflagstatus = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 1
ASGNI4
line 1740
;1740:				if (BotTeam(bs) == TEAM_RED) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 168
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 1
NEI4 $588
line 1741
;1741:					trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 128
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1742
;1742:					bs->flagcarrier = ClientFromName(netname);
ADDRLP4 128
ARGP4
ADDRLP4 172
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
ADDRLP4 172
INDIRI4
ASGNI4
line 1743
;1743:				}
LABELV $588
line 1744
;1744:			}
LABELV $585
line 1745
;1745:			bs->flagstatuschanged = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 1746
;1746:			bs->lastflagcapture_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6944
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1747
;1747:		}
ADDRGP4 $583
JUMPV
LABELV $582
line 1748
;1748:		else if (match->subtype & ST_CAPTUREDFLAG) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $590
line 1749
;1749:			bs->redflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 0
ASGNI4
line 1750
;1750:			bs->blueflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 0
ASGNI4
line 1751
;1751:			bs->flagcarrier = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
CNSTI4 0
ASGNI4
line 1752
;1752:			bs->flagstatuschanged = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 1753
;1753:		}
ADDRGP4 $591
JUMPV
LABELV $590
line 1754
;1754:		else if (match->subtype & ST_RETURNEDFLAG) {
ADDRFP4 4
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $592
line 1755
;1755:			if (!Q_stricmp(flag, "red")) bs->redflagstatus = 0;
ADDRLP4 0
ARGP4
ADDRGP4 $544
ARGP4
ADDRLP4 164
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
NEI4 $594
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 0
ASGNI4
ADDRGP4 $595
JUMPV
LABELV $594
line 1756
;1756:			else bs->blueflagstatus = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 0
ASGNI4
LABELV $595
line 1757
;1757:			bs->flagstatuschanged = 1;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 1
ASGNI4
line 1758
;1758:		}
LABELV $592
LABELV $591
LABELV $583
line 1759
;1759:	}
LABELV $580
line 1768
;1760:#ifdef MISSIONPACK
;1761:	else if (gametype == GT_1FCTF) {
;1762:		if (match->subtype & ST_1FCTFGOTFLAG) {
;1763:			trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
;1764:			bs->flagcarrier = ClientFromName(netname);
;1765:		}
;1766:	}
;1767:#endif
;1768:}
LABELV $579
endproc BotMatch_CTF 176 16
export BotMatch_EnterGame
proc BotMatch_EnterGame 44 16
line 1770
;1769:
;1770:void BotMatch_EnterGame(bot_state_t *bs, bot_match_t *match) {
line 1774
;1771:	int client;
;1772:	char netname[MAX_NETNAME];
;1773:
;1774:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1775
;1775:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 40
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 36
ADDRLP4 40
INDIRI4
ASGNI4
line 1776
;1776:	if (client >= 0) {
ADDRLP4 36
INDIRI4
CNSTI4 0
LTI4 $597
line 1777
;1777:		notleader[client] = qfalse;
ADDRLP4 36
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 notleader
ADDP4
CNSTI4 0
ASGNI4
line 1778
;1778:	}
LABELV $597
line 1782
;1779:	//NOTE: eliza chats will catch this
;1780:	//Com_sprintf(buf, sizeof(buf), "heya %s", netname);
;1781:	//EA_Say(bs->client, buf);
;1782:}
LABELV $596
endproc BotMatch_EnterGame 44 16
export BotMatch_NewLeader
proc BotMatch_NewLeader 48 16
line 1784
;1783:
;1784:void BotMatch_NewLeader(bot_state_t *bs, bot_match_t *match) {
line 1788
;1785:	int client;
;1786:	char netname[MAX_NETNAME];
;1787:
;1788:	trap_BotMatchVariable(match, NETNAME, netname, sizeof(netname));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 trap_BotMatchVariable
CALLV
pop
line 1789
;1789:	client = FindClientByName(netname);
ADDRLP4 0
ARGP4
ADDRLP4 40
ADDRGP4 FindClientByName
CALLI4
ASGNI4
ADDRLP4 36
ADDRLP4 40
INDIRI4
ASGNI4
line 1790
;1790:	if (!BotSameTeam(bs, client))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $600
line 1791
;1791:		return;
ADDRGP4 $599
JUMPV
LABELV $600
line 1792
;1792:	Q_strncpyz(bs->teamleader, netname, sizeof(bs->teamleader));
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1793
;1793:}
LABELV $599
endproc BotMatch_NewLeader 48 16
export BotMatchMessage
proc BotMatchMessage 336 12
line 1800
;1794:
;1795:/*
;1796:==================
;1797:BotMatchMessage
;1798:==================
;1799:*/
;1800:int BotMatchMessage(bot_state_t *bs, char *message) {
line 1803
;1801:	bot_match_t match;
;1802:
;1803:	match.type = 0;
ADDRLP4 0+256
CNSTI4 0
ASGNI4
line 1805
;1804:	//if it is an unknown message
;1805:	if (!trap_BotFindMatch(message, &match, MTCONTEXT_MISC
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTU4 262
ARGU4
ADDRLP4 328
ADDRGP4 trap_BotFindMatch
CALLI4
ASGNI4
ADDRLP4 328
INDIRI4
CNSTI4 0
NEI4 $604
line 1807
;1806:											|MTCONTEXT_INITIALTEAMCHAT
;1807:											|MTCONTEXT_CTF)) {
line 1808
;1808:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $602
JUMPV
LABELV $604
line 1811
;1809:	}
;1810:	//react to the found message
;1811:	switch(match.type)
ADDRLP4 332
ADDRLP4 0+256
INDIRI4
ASGNI4
ADDRLP4 332
INDIRI4
CNSTI4 1
LTI4 $606
ADDRLP4 332
INDIRI4
CNSTI4 33
GTI4 $642
ADDRLP4 332
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $643-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $643
address $638
address $637
address $609
address $609
address $610
address $614
address $613
address $629
address $630
address $631
address $607
address $632
address $619
address $620
address $623
address $625
address $626
address $607
address $628
address $611
address $622
address $612
address $635
address $618
address $636
address $634
address $615
address $633
address $621
address $616
address $606
address $606
address $640
code
LABELV $642
ADDRLP4 0+256
INDIRI4
CNSTI4 300
EQI4 $617
ADDRGP4 $606
JUMPV
line 1812
;1812:	{
LABELV $609
line 1815
;1813:		case MSG_HELP:					//someone calling for help
;1814:		case MSG_ACCOMPANY:				//someone calling for company
;1815:		{
line 1816
;1816:			BotMatch_HelpAccompany(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_HelpAccompany
CALLV
pop
line 1817
;1817:			break;
ADDRGP4 $607
JUMPV
LABELV $610
line 1820
;1818:		}
;1819:		case MSG_DEFENDKEYAREA:			//teamplay defend a key area
;1820:		{
line 1821
;1821:			BotMatch_DefendKeyArea(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_DefendKeyArea
CALLV
pop
line 1822
;1822:			break;
ADDRGP4 $607
JUMPV
LABELV $611
line 1825
;1823:		}
;1824:		case MSG_CAMP:					//camp somewhere
;1825:		{
line 1826
;1826:			BotMatch_Camp(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_Camp
CALLV
pop
line 1827
;1827:			break;
ADDRGP4 $607
JUMPV
LABELV $612
line 1830
;1828:		}
;1829:		case MSG_PATROL:				//patrol between several key areas
;1830:		{
line 1831
;1831:			BotMatch_Patrol(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_Patrol
CALLV
pop
line 1832
;1832:			break;
ADDRGP4 $607
JUMPV
LABELV $613
line 1836
;1833:		}
;1834:		//CTF & 1FCTF
;1835:		case MSG_GETFLAG:				//ctf get the enemy flag
;1836:		{
line 1837
;1837:			BotMatch_GetFlag(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_GetFlag
CALLV
pop
line 1838
;1838:			break;
ADDRGP4 $607
JUMPV
LABELV $614
line 1856
;1839:		}
;1840:#ifdef MISSIONPACK
;1841:		//CTF & 1FCTF & Obelisk & Harvester
;1842:		case MSG_ATTACKENEMYBASE:
;1843:		{
;1844:			BotMatch_AttackEnemyBase(bs, &match);
;1845:			break;
;1846:		}
;1847:		//Harvester
;1848:		case MSG_HARVEST:
;1849:		{
;1850:			BotMatch_Harvest(bs, &match);
;1851:			break;
;1852:		}
;1853:#endif
;1854:		//CTF & 1FCTF & Harvester
;1855:		case MSG_RUSHBASE:				//ctf rush to the base
;1856:		{
line 1857
;1857:			BotMatch_RushBase(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_RushBase
CALLV
pop
line 1858
;1858:			break;
ADDRGP4 $607
JUMPV
LABELV $615
line 1862
;1859:		}
;1860:		//CTF & 1FCTF
;1861:		case MSG_RETURNFLAG:
;1862:		{
line 1863
;1863:			BotMatch_ReturnFlag(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_ReturnFlag
CALLV
pop
line 1864
;1864:			break;
ADDRGP4 $607
JUMPV
LABELV $616
line 1868
;1865:		}
;1866:		//CTF & 1FCTF & Obelisk & Harvester
;1867:		case MSG_TASKPREFERENCE:
;1868:		{
line 1869
;1869:			BotMatch_TaskPreference(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_TaskPreference
CALLV
pop
line 1870
;1870:			break;
ADDRGP4 $607
JUMPV
LABELV $617
line 1874
;1871:		}
;1872:		//CTF & 1FCTF
;1873:		case MSG_CTF:
;1874:		{
line 1875
;1875:			BotMatch_CTF(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_CTF
CALLV
pop
line 1876
;1876:			break;
ADDRGP4 $607
JUMPV
LABELV $618
line 1879
;1877:		}
;1878:		case MSG_GETITEM:
;1879:		{
line 1880
;1880:			BotMatch_GetItem(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_GetItem
CALLV
pop
line 1881
;1881:			break;
ADDRGP4 $607
JUMPV
LABELV $619
line 1884
;1882:		}
;1883:		case MSG_JOINSUBTEAM:			//join a sub team
;1884:		{
line 1885
;1885:			BotMatch_JoinSubteam(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_JoinSubteam
CALLV
pop
line 1886
;1886:			break;
ADDRGP4 $607
JUMPV
LABELV $620
line 1889
;1887:		}
;1888:		case MSG_LEAVESUBTEAM:			//leave a sub team
;1889:		{
line 1890
;1890:			BotMatch_LeaveSubteam(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_LeaveSubteam
CALLV
pop
line 1891
;1891:			break;
ADDRGP4 $607
JUMPV
LABELV $621
line 1894
;1892:		}
;1893:		case MSG_WHICHTEAM:
;1894:		{
line 1895
;1895:			BotMatch_WhichTeam(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_WhichTeam
CALLV
pop
line 1896
;1896:			break;
ADDRGP4 $607
JUMPV
LABELV $622
line 1899
;1897:		}
;1898:		case MSG_CHECKPOINT:			//remember a check point
;1899:		{
line 1900
;1900:			BotMatch_CheckPoint(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_CheckPoint
CALLV
pop
line 1901
;1901:			break;
ADDRGP4 $607
JUMPV
LABELV $623
line 1904
;1902:		}
;1903:		case MSG_CREATENEWFORMATION:	//start the creation of a new formation
;1904:		{
line 1905
;1905:			trap_EA_SayTeam(bs->client, "the part of my brain to create formations has been damaged");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $624
ARGP4
ADDRGP4 trap_EA_SayTeam
CALLV
pop
line 1906
;1906:			break;
ADDRGP4 $607
JUMPV
LABELV $625
line 1909
;1907:		}
;1908:		case MSG_FORMATIONPOSITION:		//tell someone his/her position in the formation
;1909:		{
line 1910
;1910:			trap_EA_SayTeam(bs->client, "the part of my brain to create formations has been damaged");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $624
ARGP4
ADDRGP4 trap_EA_SayTeam
CALLV
pop
line 1911
;1911:			break;
ADDRGP4 $607
JUMPV
LABELV $626
line 1914
;1912:		}
;1913:		case MSG_FORMATIONSPACE:		//set the formation space
;1914:		{
line 1915
;1915:			BotMatch_FormationSpace(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_FormationSpace
CALLV
pop
line 1916
;1916:			break;
ADDRGP4 $607
JUMPV
line 1919
;1917:		}
;1918:		case MSG_DOFORMATION:			//form a certain formation
;1919:		{
line 1920
;1920:			break;
LABELV $628
line 1923
;1921:		}
;1922:		case MSG_DISMISS:				//dismiss someone
;1923:		{
line 1924
;1924:			BotMatch_Dismiss(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_Dismiss
CALLV
pop
line 1925
;1925:			break;
ADDRGP4 $607
JUMPV
LABELV $629
line 1928
;1926:		}
;1927:		case MSG_STARTTEAMLEADERSHIP:	//someone will become the team leader
;1928:		{
line 1929
;1929:			BotMatch_StartTeamLeaderShip(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_StartTeamLeaderShip
CALLV
pop
line 1930
;1930:			break;
ADDRGP4 $607
JUMPV
LABELV $630
line 1933
;1931:		}
;1932:		case MSG_STOPTEAMLEADERSHIP:	//someone will stop being the team leader
;1933:		{
line 1934
;1934:			BotMatch_StopTeamLeaderShip(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_StopTeamLeaderShip
CALLV
pop
line 1935
;1935:			break;
ADDRGP4 $607
JUMPV
LABELV $631
line 1938
;1936:		}
;1937:		case MSG_WHOISTEAMLAEDER:
;1938:		{
line 1939
;1939:			BotMatch_WhoIsTeamLeader(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_WhoIsTeamLeader
CALLV
pop
line 1940
;1940:			break;
ADDRGP4 $607
JUMPV
LABELV $632
line 1943
;1941:		}
;1942:		case MSG_WHATAREYOUDOING:		//ask a bot what he/she is doing
;1943:		{
line 1944
;1944:			BotMatch_WhatAreYouDoing(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_WhatAreYouDoing
CALLV
pop
line 1945
;1945:			break;
ADDRGP4 $607
JUMPV
LABELV $633
line 1948
;1946:		}
;1947:		case MSG_WHATISMYCOMMAND:
;1948:		{
line 1949
;1949:			BotMatch_WhatIsMyCommand(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_WhatIsMyCommand
CALLV
pop
line 1950
;1950:			break;
ADDRGP4 $607
JUMPV
LABELV $634
line 1953
;1951:		}
;1952:		case MSG_WHEREAREYOU:
;1953:		{
line 1954
;1954:			BotMatch_WhereAreYou(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_WhereAreYou
CALLV
pop
line 1955
;1955:			break;
ADDRGP4 $607
JUMPV
LABELV $635
line 1958
;1956:		}
;1957:		case MSG_LEADTHEWAY:
;1958:		{
line 1959
;1959:			BotMatch_LeadTheWay(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_LeadTheWay
CALLV
pop
line 1960
;1960:			break;
ADDRGP4 $607
JUMPV
LABELV $636
line 1963
;1961:		}
;1962:		case MSG_KILL:
;1963:		{
line 1964
;1964:			BotMatch_Kill(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_Kill
CALLV
pop
line 1965
;1965:			break;
ADDRGP4 $607
JUMPV
LABELV $637
line 1968
;1966:		}
;1967:		case MSG_ENTERGAME:				//someone entered the game
;1968:		{
line 1969
;1969:			BotMatch_EnterGame(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_EnterGame
CALLV
pop
line 1970
;1970:			break;
ADDRGP4 $607
JUMPV
LABELV $638
line 1973
;1971:		}
;1972:		case MSG_NEWLEADER:
;1973:		{
line 1974
;1974:			BotMatch_NewLeader(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_NewLeader
CALLV
pop
line 1975
;1975:			break;
ADDRGP4 $607
JUMPV
line 1978
;1976:		}
;1977:		case MSG_WAIT:
;1978:		{
line 1979
;1979:			break;
LABELV $640
line 1982
;1980:		}
;1981:		case MSG_SUICIDE:
;1982:		{
line 1983
;1983:			BotMatch_Suicide(bs, &match);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotMatch_Suicide
CALLV
pop
line 1984
;1984:			break;
ADDRGP4 $607
JUMPV
LABELV $606
line 1987
;1985:		}
;1986:		default:
;1987:		{
line 1988
;1988:			BotAI_Print(PRT_MESSAGE, "unknown match type\n");
CNSTI4 1
ARGI4
ADDRGP4 $641
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 1989
;1989:			break;
LABELV $607
line 1992
;1990:		}
;1991:	}
;1992:	return qtrue;
CNSTI4 1
RETI4
LABELV $602
endproc BotMatchMessage 336 12
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
bss
export notleader
align 4
LABELV notleader
skip 256
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
LABELV $641
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $624
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 112
byte 1 97
byte 1 114
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 109
byte 1 121
byte 1 32
byte 1 98
byte 1 114
byte 1 97
byte 1 105
byte 1 110
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 102
byte 1 111
byte 1 114
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 98
byte 1 101
byte 1 101
byte 1 110
byte 1 32
byte 1 100
byte 1 97
byte 1 109
byte 1 97
byte 1 103
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $548
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $547
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $544
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $543
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $524
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
LABELV $523
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
LABELV $522
byte 1 72
byte 1 101
byte 1 97
byte 1 118
byte 1 121
byte 1 32
byte 1 65
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $521
byte 1 65
byte 1 114
byte 1 109
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $520
byte 1 70
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $519
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
LABELV $518
byte 1 83
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $517
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
LABELV $516
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
LABELV $515
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
LABELV $514
byte 1 66
byte 1 70
byte 1 71
byte 1 49
byte 1 48
byte 1 75
byte 1 0
align 1
LABELV $513
byte 1 76
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 71
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $512
byte 1 82
byte 1 97
byte 1 105
byte 1 108
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $511
byte 1 80
byte 1 108
byte 1 97
byte 1 115
byte 1 109
byte 1 97
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $510
byte 1 82
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 32
byte 1 76
byte 1 97
byte 1 117
byte 1 110
byte 1 99
byte 1 104
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $509
byte 1 71
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 100
byte 1 101
byte 1 32
byte 1 76
byte 1 97
byte 1 117
byte 1 110
byte 1 99
byte 1 104
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $508
byte 1 83
byte 1 104
byte 1 111
byte 1 116
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $484
byte 1 114
byte 1 111
byte 1 97
byte 1 109
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $483
byte 1 114
byte 1 101
byte 1 116
byte 1 117
byte 1 114
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $481
byte 1 114
byte 1 117
byte 1 115
byte 1 104
byte 1 105
byte 1 110
byte 1 103
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $479
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $477
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
LABELV $475
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $473
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $471
byte 1 103
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 0
align 1
LABELV $469
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $467
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
byte 1 0
align 1
LABELV $465
byte 1 104
byte 1 101
byte 1 108
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $457
byte 1 73
byte 1 39
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 10
byte 1 0
align 1
LABELV $435
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $434
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $428
byte 1 100
byte 1 105
byte 1 115
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $412
byte 1 99
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 95
byte 1 99
byte 1 111
byte 1 110
byte 1 102
byte 1 105
byte 1 114
byte 1 109
byte 1 0
align 1
LABELV $411
byte 1 37
byte 1 49
byte 1 46
byte 1 48
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 48
byte 1 102
byte 1 32
byte 1 37
byte 1 49
byte 1 46
byte 1 48
byte 1 102
byte 1 0
align 1
LABELV $400
byte 1 99
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 95
byte 1 105
byte 1 110
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 0
align 1
LABELV $392
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
LABELV $386
byte 1 110
byte 1 111
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $385
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $377
byte 1 0
align 1
LABELV $376
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $368
byte 1 106
byte 1 111
byte 1 105
byte 1 110
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $357
byte 1 121
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $356
byte 1 107
byte 1 101
byte 1 101
byte 1 112
byte 1 105
byte 1 110
byte 1 109
byte 1 105
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $261
byte 1 119
byte 1 104
byte 1 101
byte 1 114
byte 1 101
byte 1 97
byte 1 114
byte 1 101
byte 1 121
byte 1 111
byte 1 117
byte 1 0
align 1
LABELV $260
byte 1 119
byte 1 104
byte 1 101
byte 1 114
byte 1 101
byte 1 105
byte 1 115
byte 1 0
align 1
LABELV $241
byte 1 119
byte 1 104
byte 1 111
byte 1 105
byte 1 115
byte 1 0
align 1
LABELV $224
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $175
byte 1 73
byte 1 32
byte 1 110
byte 1 101
byte 1 101
byte 1 100
byte 1 32
byte 1 109
byte 1 111
byte 1 114
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 32
byte 1 112
byte 1 111
byte 1 105
byte 1 110
byte 1 116
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 10
byte 1 0
align 1
LABELV $150
byte 1 119
byte 1 104
byte 1 97
byte 1 116
byte 1 32
byte 1 100
byte 1 111
byte 1 32
byte 1 121
byte 1 111
byte 1 117
byte 1 32
byte 1 115
byte 1 97
byte 1 121
byte 1 63
byte 1 0
align 1
LABELV $94
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
