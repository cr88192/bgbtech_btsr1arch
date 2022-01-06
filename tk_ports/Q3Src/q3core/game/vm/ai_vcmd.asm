export BotVoiceChat_GetFlag
code
proc BotVoiceChat_GetFlag 8 8
file "../ai_vcmd.c"
line 73
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
;25: * name:		ai_vcmd.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_vcmd.c $
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
;50:#include "ai_vcmd.h"
;51://
;52:#include "chars.h"				//characteristics
;53:#include "inv.h"				//indexes into the inventory
;54:#include "syn.h"				//synonyms
;55:#include "match.h"				//string matching types and vars
;56:
;57:// for the voice chats
;58:// #include "../../ui/menudef.h"
;59:#include "../ui/menudef.h"
;60:
;61:
;62:typedef struct voiceCommand_s
;63:{
;64:	char *cmd;
;65:	void (*func)(bot_state_t *bs, int client, int mode);
;66:} voiceCommand_t;
;67:
;68:/*
;69:==================
;70:BotVoiceChat_GetFlag
;71:==================
;72:*/
;73:void BotVoiceChat_GetFlag(bot_state_t *bs, int client, int mode) {
line 75
;74:	//
;75:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $53
line 76
;76:		if (!ctf_redflag.areanum || !ctf_blueflag.areanum)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 ctf_redflag+12
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $60
ADDRGP4 ctf_blueflag+12
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $55
LABELV $60
line 77
;77:			return;
ADDRGP4 $53
JUMPV
line 78
;78:	}
line 85
;79:#ifdef MISSIONPACK
;80:	else if (gametype == GT_1FCTF) {
;81:		if (!ctf_neutralflag.areanum || !ctf_redflag.areanum || !ctf_blueflag.areanum)
;82:			return;
;83:	}
;84:#endif
;85:	else {
line 86
;86:		return;
LABELV $55
line 89
;87:	}
;88:	//
;89:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 90
;90:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 91
;91:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 93
;92:	//set the time to send a message to the team mates
;93:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 0
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
ADDRLP4 0
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 95
;94:	//set the ltg type
;95:	bs->ltgtype = LTG_GETFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 4
ASGNI4
line 97
;96:	//set the team goal time
;97:	bs->teamgoal_time = FloatTime() + CTF_GETFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 99
;98:	// get an alternate route in ctf
;99:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $61
line 101
;100:		//get an alternative route goal towards the enemy base
;101:		BotGetAlternateRouteGoal(bs, BotOppositeTeam(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotOppositeTeam
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 BotGetAlternateRouteGoal
CALLI4
pop
line 102
;102:	}
LABELV $61
line 104
;103:	//
;104:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 106
;105:	// remember last ordered task
;106:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 110
;107:#ifdef DEBUG
;108:	BotPrintTeamGoal(bs);
;109:#endif //DEBUG
;110:}
LABELV $53
endproc BotVoiceChat_GetFlag 8 8
export BotVoiceChat_Offense
proc BotVoiceChat_Offense 4 12
line 117
;111:
;112:/*
;113:==================
;114:BotVoiceChat_Offense
;115:==================
;116:*/
;117:void BotVoiceChat_Offense(bot_state_t *bs, int client, int mode) {
line 118
;118:	if ( gametype == GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $64
line 122
;119:#ifdef MISSIONPACK
;120:		|| gametype == GT_1FCTF
;121:#endif
;122:		) {
line 123
;123:		BotVoiceChat_GetFlag(bs, client, mode);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 BotVoiceChat_GetFlag
CALLV
pop
line 124
;124:		return;
ADDRGP4 $63
JUMPV
LABELV $64
line 146
;125:	}
;126:#ifdef MISSIONPACK
;127:	if (gametype == GT_HARVESTER) {
;128:		//
;129:		bs->decisionmaker = client;
;130:		bs->ordered = qtrue;
;131:		bs->order_time = FloatTime();
;132:		//set the time to send a message to the team mates
;133:		bs->teammessage_time = FloatTime() + 2 * random();
;134:		//set the ltg type
;135:		bs->ltgtype = LTG_HARVEST;
;136:		//set the team goal time
;137:		bs->teamgoal_time = FloatTime() + TEAM_HARVEST_TIME;
;138:		bs->harvestaway_time = 0;
;139:		//
;140:		BotSetTeamStatus(bs);
;141:		// remember last ordered task
;142:		BotRememberLastOrderedTask(bs);
;143:	}
;144:	else
;145:#endif
;146:	{
line 148
;147:		//
;148:		bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 149
;149:		bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 150
;150:		bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 152
;151:		//set the time to send a message to the team mates
;152:		bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 0
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
ADDRLP4 0
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 154
;153:		//set the ltg type
;154:		bs->ltgtype = LTG_ATTACKENEMYBASE;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 13
ASGNI4
line 156
;155:		//set the team goal time
;156:		bs->teamgoal_time = FloatTime() + TEAM_ATTACKENEMYBASE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 157
;157:		bs->attackaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6156
ADDP4
CNSTF4 0
ASGNF4
line 159
;158:		//
;159:		BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 161
;160:		// remember last ordered task
;161:		BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 162
;162:	}
line 166
;163:#ifdef DEBUG
;164:	BotPrintTeamGoal(bs);
;165:#endif //DEBUG
;166:}
LABELV $63
endproc BotVoiceChat_Offense 4 12
export BotVoiceChat_Defend
proc BotVoiceChat_Defend 12 12
line 173
;167:
;168:/*
;169:==================
;170:BotVoiceChat_Defend
;171:==================
;172:*/
;173:void BotVoiceChat_Defend(bot_state_t *bs, int client, int mode) {
line 185
;174:#ifdef MISSIONPACK
;175:	if ( gametype == GT_OBELISK || gametype == GT_HARVESTER) {
;176:		//
;177:		switch(BotTeam(bs)) {
;178:			case TEAM_RED: memcpy(&bs->teamgoal, &redobelisk, sizeof(bot_goal_t)); break;
;179:			case TEAM_BLUE: memcpy(&bs->teamgoal, &blueobelisk, sizeof(bot_goal_t)); break;
;180:			default: return;
;181:		}
;182:	}
;183:	else
;184:#endif
;185:		if (gametype == GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $66
line 189
;186:#ifdef MISSIONPACK
;187:			|| gametype == GT_1FCTF
;188:#endif
;189:			) {
line 191
;190:		//
;191:		switch(BotTeam(bs)) {
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
EQI4 $72
ADDRLP4 0
INDIRI4
CNSTI4 2
EQI4 $73
ADDRGP4 $66
JUMPV
LABELV $72
line 192
;192:			case TEAM_RED: memcpy(&bs->teamgoal, &ctf_redflag, sizeof(bot_goal_t)); break;
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
ADDRGP4 $68
JUMPV
LABELV $73
line 193
;193:			case TEAM_BLUE: memcpy(&bs->teamgoal, &ctf_blueflag, sizeof(bot_goal_t)); break;
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
line 194
;194:			default: return;
line 196
;195:		}
;196:	}
line 197
;197:	else {
line 198
;198:		return;
LABELV $68
line 201
;199:	}
;200:	//
;201:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 202
;202:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 203
;203:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 205
;204:	//set the time to send a message to the team mates
;205:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 0
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
ADDRLP4 0
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 207
;206:	//set the ltg type
;207:	bs->ltgtype = LTG_DEFENDKEYAREA;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 3
ASGNI4
line 209
;208:	//get the team goal time
;209:	bs->teamgoal_time = FloatTime() + TEAM_DEFENDKEYAREA_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 211
;210:	//away from defending
;211:	bs->defendaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6144
ADDP4
CNSTF4 0
ASGNF4
line 213
;212:	//
;213:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 215
;214:	// remember last ordered task
;215:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 219
;216:#ifdef DEBUG
;217:	BotPrintTeamGoal(bs);
;218:#endif //DEBUG
;219:}
LABELV $66
endproc BotVoiceChat_Defend 12 12
export BotVoiceChat_DefendFlag
proc BotVoiceChat_DefendFlag 0 12
line 226
;220:
;221:/*
;222:==================
;223:BotVoiceChat_DefendFlag
;224:==================
;225:*/
;226:void BotVoiceChat_DefendFlag(bot_state_t *bs, int client, int mode) {
line 227
;227:	BotVoiceChat_Defend(bs, client, mode);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 BotVoiceChat_Defend
CALLV
pop
line 228
;228:}
LABELV $74
endproc BotVoiceChat_DefendFlag 0 12
export BotVoiceChat_Patrol
proc BotVoiceChat_Patrol 0 12
line 235
;229:
;230:/*
;231:==================
;232:BotVoiceChat_Patrol
;233:==================
;234:*/
;235:void BotVoiceChat_Patrol(bot_state_t *bs, int client, int mode) {
line 237
;236:	//
;237:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 239
;238:	//
;239:	bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 240
;240:	bs->lead_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
CNSTF4 0
ASGNF4
line 241
;241:	bs->lastgoal_ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
CNSTI4 0
ASGNI4
line 243
;242:	//
;243:	BotAI_BotInitialChat(bs, "dismissed", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $76
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 244
;244:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 245
;245:	BotVoiceChatOnly(bs, -1, VOICECHAT_ONPATROL);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $77
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 247
;246:	//
;247:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 251
;248:#ifdef DEBUG
;249:	BotPrintTeamGoal(bs);
;250:#endif //DEBUG
;251:}
LABELV $75
endproc BotVoiceChat_Patrol 0 12
export BotVoiceChat_Camp
proc BotVoiceChat_Camp 184 16
line 258
;252:
;253:/*
;254:==================
;255:BotVoiceChat_Camp
;256:==================
;257:*/
;258:void BotVoiceChat_Camp(bot_state_t *bs, int client, int mode) {
line 264
;259:	int areanum;
;260:	aas_entityinfo_t entinfo;
;261:	char netname[MAX_NETNAME];
;262:
;263:	//
;264:	bs->teamgoal.entitynum = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
CNSTI4 -1
ASGNI4
line 265
;265:	BotEntityInfo(client, &entinfo);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 267
;266:	//if info is valid (in PVS)
;267:	if (entinfo.valid) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $79
line 268
;268:		areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 0+24
ARGP4
ADDRLP4 180
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 180
INDIRI4
ASGNI4
line 269
;269:		if (areanum) { // && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $82
line 272
;270:			//NOTE: just assume the bot knows where the person is
;271:			//if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, client)) {
;272:				bs->teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 273
;273:				bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 140
INDIRI4
ASGNI4
line 274
;274:				VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 0+24
INDIRB
ASGNB 12
line 275
;275:				VectorSet(bs->teamgoal.mins, -8, -8, -8);
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
line 276
;276:				VectorSet(bs->teamgoal.maxs, 8, 8, 8);
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
line 278
;277:			//}
;278:		}
LABELV $82
line 279
;279:	}
LABELV $79
line 281
;280:	//if the other is not visible
;281:	if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $85
line 282
;282:		BotAI_BotInitialChat(bs, "whereareyou", EasyClientName(client, netname, sizeof(netname)), NULL);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 144
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 180
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $87
ARGP4
ADDRLP4 180
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 283
;283:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 284
;284:		return;
ADDRGP4 $78
JUMPV
LABELV $85
line 287
;285:	}
;286:	//
;287:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 288
;288:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 289
;289:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 291
;290:	//set the time to send a message to the team mates
;291:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 180
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
ADDRLP4 180
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 293
;292:	//set the ltg type
;293:	bs->ltgtype = LTG_CAMPORDER;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 8
ASGNI4
line 295
;294:	//get the team goal time
;295:	bs->teamgoal_time = FloatTime() + TEAM_CAMP_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 297
;296:	//the teammate that requested the camping
;297:	bs->teammate = client;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 299
;298:	//not arrived yet
;299:	bs->arrive_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 0
ASGNF4
line 301
;300:	//
;301:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 303
;302:	// remember last ordered task
;303:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 307
;304:#ifdef DEBUG
;305:	BotPrintTeamGoal(bs);
;306:#endif //DEBUG
;307:}
LABELV $78
endproc BotVoiceChat_Camp 184 16
export BotVoiceChat_FollowMe
proc BotVoiceChat_FollowMe 184 16
line 314
;308:
;309:/*
;310:==================
;311:BotVoiceChat_FollowMe
;312:==================
;313:*/
;314:void BotVoiceChat_FollowMe(bot_state_t *bs, int client, int mode) {
line 319
;315:	int areanum;
;316:	aas_entityinfo_t entinfo;
;317:	char netname[MAX_NETNAME];
;318:
;319:	bs->teamgoal.entitynum = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
CNSTI4 -1
ASGNI4
line 320
;320:	BotEntityInfo(client, &entinfo);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 322
;321:	//if info is valid (in PVS)
;322:	if (entinfo.valid) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $89
line 323
;323:		areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 0+24
ARGP4
ADDRLP4 180
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 180
INDIRI4
ASGNI4
line 324
;324:		if (areanum) { // && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $92
line 325
;325:			bs->teamgoal.entitynum = client;
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 326
;326:			bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 140
INDIRI4
ASGNI4
line 327
;327:			VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 0+24
INDIRB
ASGNB 12
line 328
;328:			VectorSet(bs->teamgoal.mins, -8, -8, -8);
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
line 329
;329:			VectorSet(bs->teamgoal.maxs, 8, 8, 8);
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
line 330
;330:		}
LABELV $92
line 331
;331:	}
LABELV $89
line 333
;332:	//if the other is not visible
;333:	if (bs->teamgoal.entitynum < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
CNSTI4 0
GEI4 $95
line 334
;334:		BotAI_BotInitialChat(bs, "whereareyou", EasyClientName(client, netname, sizeof(netname)), NULL);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 144
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 180
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $87
ARGP4
ADDRLP4 180
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 335
;335:		trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 336
;336:		return;
ADDRGP4 $88
JUMPV
LABELV $95
line 339
;337:	}
;338:	//
;339:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 340
;340:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 341
;341:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 343
;342:	//the team mate
;343:	bs->teammate = client;
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 345
;344:	//last time the team mate was assumed visible
;345:	bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 347
;346:	//set the time to send a message to the team mates
;347:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 180
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
ADDRLP4 180
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 349
;348:	//get the team goal time
;349:	bs->teamgoal_time = FloatTime() + TEAM_ACCOMPANY_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1142292480
ADDF4
ASGNF4
line 351
;350:	//set the ltg type
;351:	bs->ltgtype = LTG_TEAMACCOMPANY;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 2
ASGNI4
line 352
;352:	bs->formation_dist = 3.5 * 32;		//3.5 meter
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
CNSTF4 1121976320
ASGNF4
line 353
;353:	bs->arrive_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
CNSTF4 0
ASGNF4
line 355
;354:	//
;355:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 357
;356:	// remember last ordered task
;357:	BotRememberLastOrderedTask(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotRememberLastOrderedTask
CALLV
pop
line 361
;358:#ifdef DEBUG
;359:	BotPrintTeamGoal(bs);
;360:#endif //DEBUG
;361:}
LABELV $88
endproc BotVoiceChat_FollowMe 184 16
export BotVoiceChat_FollowFlagCarrier
proc BotVoiceChat_FollowFlagCarrier 8 12
line 368
;362:
;363:/*
;364:==================
;365:BotVoiceChat_FollowFlagCarrier
;366:==================
;367:*/
;368:void BotVoiceChat_FollowFlagCarrier(bot_state_t *bs, int client, int mode) {
line 371
;369:	int carrier;
;370:
;371:	carrier = BotTeamFlagCarrier(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotTeamFlagCarrier
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 372
;372:	if (carrier >= 0)
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $98
line 373
;373:		BotVoiceChat_FollowMe(bs, carrier, mode);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 BotVoiceChat_FollowMe
CALLV
pop
LABELV $98
line 377
;374:#ifdef DEBUG
;375:	BotPrintTeamGoal(bs);
;376:#endif //DEBUG
;377:}
LABELV $97
endproc BotVoiceChat_FollowFlagCarrier 8 12
export BotVoiceChat_ReturnFlag
proc BotVoiceChat_ReturnFlag 4 4
line 384
;378:
;379:/*
;380:==================
;381:BotVoiceChat_ReturnFlag
;382:==================
;383:*/
;384:void BotVoiceChat_ReturnFlag(bot_state_t *bs, int client, int mode) {
line 387
;385:	//if not in CTF mode
;386:	if (
;387:		gametype != GT_CTF
ADDRGP4 gametype
INDIRI4
CNSTI4 4
EQI4 $101
line 391
;388:#ifdef MISSIONPACK
;389:		&& gametype != GT_1FCTF
;390:#endif
;391:		) {
line 392
;392:		return;
ADDRGP4 $100
JUMPV
LABELV $101
line 395
;393:	}
;394:	//
;395:	bs->decisionmaker = client;
ADDRFP4 0
INDIRP4
CNSTI4 6608
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 396
;396:	bs->ordered = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6612
ADDP4
CNSTI4 1
ASGNI4
line 397
;397:	bs->order_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6616
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 399
;398:	//set the time to send a message to the team mates
;399:	bs->teammessage_time = FloatTime() + 2 * random();
ADDRLP4 0
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
ADDRLP4 0
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 401
;400:	//set the ltg type
;401:	bs->ltgtype = LTG_RETURNFLAG;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 6
ASGNI4
line 403
;402:	//set the team goal time
;403:	bs->teamgoal_time = FloatTime() + CTF_RETURNFLAG_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1127481344
ADDF4
ASGNF4
line 404
;404:	bs->rushbaseaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
CNSTF4 0
ASGNF4
line 405
;405:	BotSetTeamStatus(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetTeamStatus
CALLV
pop
line 409
;406:#ifdef DEBUG
;407:	BotPrintTeamGoal(bs);
;408:#endif //DEBUG
;409:}
LABELV $100
endproc BotVoiceChat_ReturnFlag 4 4
export BotVoiceChat_StartLeader
proc BotVoiceChat_StartLeader 0 12
line 416
;410:
;411:/*
;412:==================
;413:BotVoiceChat_StartLeader
;414:==================
;415:*/
;416:void BotVoiceChat_StartLeader(bot_state_t *bs, int client, int mode) {
line 417
;417:	ClientName(client, bs->teamleader, sizeof(bs->teamleader));
ADDRFP4 4
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
line 418
;418:}
LABELV $103
endproc BotVoiceChat_StartLeader 0 12
export BotVoiceChat_StopLeader
proc BotVoiceChat_StopLeader 264 12
line 425
;419:
;420:/*
;421:==================
;422:BotVoiceChat_StopLeader
;423:==================
;424:*/
;425:void BotVoiceChat_StopLeader(bot_state_t *bs, int client, int mode) {
line 428
;426:	char netname[MAX_MESSAGE_SIZE];
;427:
;428:	if (!Q_stricmp(bs->teamleader, ClientName(client, netname, sizeof(netname)))) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 256
ADDRGP4 ClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 256
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
NEI4 $105
line 429
;429:		bs->teamleader[0] = '\0';
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
CNSTI1 0
ASGNI1
line 430
;430:		notleader[client] = qtrue;
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 notleader
ADDP4
CNSTI4 1
ASGNI4
line 431
;431:	}
LABELV $105
line 432
;432:}
LABELV $104
endproc BotVoiceChat_StopLeader 264 12
export BotVoiceChat_WhoIsLeader
proc BotVoiceChat_WhoIsLeader 264 12
line 439
;433:
;434:/*
;435:==================
;436:BotVoiceChat_WhoIsLeader
;437:==================
;438:*/
;439:void BotVoiceChat_WhoIsLeader(bot_state_t *bs, int client, int mode) {
line 442
;440:	char netname[MAX_MESSAGE_SIZE];
;441:
;442:	if (!TeamPlayIsOn()) return;
ADDRLP4 256
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 256
INDIRI4
CNSTI4 0
NEI4 $108
ADDRGP4 $107
JUMPV
LABELV $108
line 444
;443:
;444:	ClientName(bs->client, netname, sizeof(netname));
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
line 446
;445:	//if this bot IS the team leader
;446:	if (!Q_stricmp(netname, bs->teamleader)) {
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
NEI4 $110
line 447
;447:		BotAI_BotInitialChat(bs, "iamteamleader", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $112
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 448
;448:		trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 449
;449:		BotVoiceChatOnly(bs, -1, VOICECHAT_STARTLEADER);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $113
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 450
;450:	}
LABELV $110
line 451
;451:}
LABELV $107
endproc BotVoiceChat_WhoIsLeader 264 12
export BotVoiceChat_WantOnDefense
proc BotVoiceChat_WantOnDefense 44 16
line 458
;452:
;453:/*
;454:==================
;455:BotVoiceChat_WantOnDefense
;456:==================
;457:*/
;458:void BotVoiceChat_WantOnDefense(bot_state_t *bs, int client, int mode) {
line 462
;459:	char netname[MAX_NETNAME];
;460:	int preference;
;461:
;462:	preference = BotGetTeamMateTaskPreference(bs, client);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 BotGetTeamMateTaskPreference
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 40
INDIRI4
ASGNI4
line 463
;463:	preference &= ~TEAMTP_ATTACKER;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 464
;464:	preference |= TEAMTP_DEFENDER;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 465
;465:	BotSetTeamMateTaskPreference(bs, client, preference);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSetTeamMateTaskPreference
CALLV
pop
line 467
;466:	//
;467:	EasyClientName(client, netname, sizeof(netname));
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 468
;468:	BotAI_BotInitialChat(bs, "keepinmind", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $115
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 469
;469:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 470
;470:	BotVoiceChatOnly(bs, client, VOICECHAT_YES);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $116
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 471
;471:	trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 472
;472:}
LABELV $114
endproc BotVoiceChat_WantOnDefense 44 16
export BotVoiceChat_WantOnOffense
proc BotVoiceChat_WantOnOffense 44 16
line 479
;473:
;474:/*
;475:==================
;476:BotVoiceChat_WantOnOffense
;477:==================
;478:*/
;479:void BotVoiceChat_WantOnOffense(bot_state_t *bs, int client, int mode) {
line 483
;480:	char netname[MAX_NETNAME];
;481:	int preference;
;482:
;483:	preference = BotGetTeamMateTaskPreference(bs, client);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 BotGetTeamMateTaskPreference
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 40
INDIRI4
ASGNI4
line 484
;484:	preference &= ~TEAMTP_DEFENDER;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 485
;485:	preference |= TEAMTP_ATTACKER;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 486
;486:	BotSetTeamMateTaskPreference(bs, client, preference);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSetTeamMateTaskPreference
CALLV
pop
line 488
;487:	//
;488:	EasyClientName(client, netname, sizeof(netname));
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 489
;489:	BotAI_BotInitialChat(bs, "keepinmind", netname, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $115
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 490
;490:	trap_BotEnterChat(bs->cs, client, CHAT_TELL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 491
;491:	BotVoiceChatOnly(bs, client, VOICECHAT_YES);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $116
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 492
;492:	trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 493
;493:}
LABELV $117
endproc BotVoiceChat_WantOnOffense 44 16
export BotVoiceChat_Dummy
proc BotVoiceChat_Dummy 0 0
line 495
;494:
;495:void BotVoiceChat_Dummy(bot_state_t *bs, int client, int mode) {
line 496
;496:}
LABELV $118
endproc BotVoiceChat_Dummy 0 0
data
export voiceCommands
align 4
LABELV voiceCommands
address $119
address BotVoiceChat_GetFlag
address $120
address BotVoiceChat_Offense
address $121
address BotVoiceChat_Defend
address $122
address BotVoiceChat_DefendFlag
address $123
address BotVoiceChat_Patrol
address $124
address BotVoiceChat_Camp
address $125
address BotVoiceChat_FollowMe
address $126
address BotVoiceChat_FollowFlagCarrier
address $127
address BotVoiceChat_ReturnFlag
address $113
address BotVoiceChat_StartLeader
address $128
address BotVoiceChat_StopLeader
address $129
address BotVoiceChat_WhoIsLeader
address $130
address BotVoiceChat_WantOnDefense
address $131
address BotVoiceChat_WantOnOffense
byte 4 0
address BotVoiceChat_Dummy
export BotVoiceChatCommand
code
proc BotVoiceChatCommand 340 12
line 516
;497:
;498:voiceCommand_t voiceCommands[] = {
;499:	{VOICECHAT_GETFLAG, BotVoiceChat_GetFlag},
;500:	{VOICECHAT_OFFENSE, BotVoiceChat_Offense },
;501:	{VOICECHAT_DEFEND, BotVoiceChat_Defend },
;502:	{VOICECHAT_DEFENDFLAG, BotVoiceChat_DefendFlag },
;503:	{VOICECHAT_PATROL, BotVoiceChat_Patrol },
;504:	{VOICECHAT_CAMP, BotVoiceChat_Camp },
;505:	{VOICECHAT_FOLLOWME, BotVoiceChat_FollowMe },
;506:	{VOICECHAT_FOLLOWFLAGCARRIER, BotVoiceChat_FollowFlagCarrier },
;507:	{VOICECHAT_RETURNFLAG, BotVoiceChat_ReturnFlag },
;508:	{VOICECHAT_STARTLEADER, BotVoiceChat_StartLeader },
;509:	{VOICECHAT_STOPLEADER, BotVoiceChat_StopLeader },
;510:	{VOICECHAT_WHOISLEADER, BotVoiceChat_WhoIsLeader },
;511:	{VOICECHAT_WANTONDEFENSE, BotVoiceChat_WantOnDefense },
;512:	{VOICECHAT_WANTONOFFENSE, BotVoiceChat_WantOnOffense },
;513:	{NULL, BotVoiceChat_Dummy}
;514:};
;515:
;516:int BotVoiceChatCommand(bot_state_t *bs, int mode, char *voiceChat) {
line 520
;517:	int i, voiceOnly, clientNum, color;
;518:	char *ptr, buf[MAX_MESSAGE_SIZE], *cmd;
;519:
;520:	if (!TeamPlayIsOn()) {
ADDRLP4 280
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 280
INDIRI4
CNSTI4 0
NEI4 $133
line 521
;521:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $132
JUMPV
LABELV $133
line 524
;522:	}
;523:
;524:	if ( mode == SAY_ALL ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $135
line 525
;525:		return qfalse;	// don't do anything with voice chats to everyone
CNSTI4 0
RETI4
ADDRGP4 $132
JUMPV
LABELV $135
line 528
;526:	}
;527:
;528:	Q_strncpyz(buf, voiceChat, sizeof(buf));
ADDRLP4 16
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 529
;529:	cmd = buf;
ADDRLP4 0
ADDRLP4 16
ASGNP4
line 530
;530:	for (ptr = cmd; *cmd && *cmd > ' '; cmd++);
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $140
JUMPV
LABELV $137
LABELV $138
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $140
ADDRLP4 284
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 284
INDIRI4
CNSTI4 0
EQI4 $141
ADDRLP4 284
INDIRI4
CNSTI4 32
GTI4 $137
LABELV $141
ADDRGP4 $143
JUMPV
LABELV $142
line 531
;531:	while (*cmd && *cmd <= ' ') *cmd++ = '\0';
ADDRLP4 288
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 288
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 288
INDIRP4
CNSTI1 0
ASGNI1
LABELV $143
ADDRLP4 292
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 292
INDIRI4
CNSTI4 0
EQI4 $145
ADDRLP4 292
INDIRI4
CNSTI4 32
LEI4 $142
LABELV $145
line 532
;532:	voiceOnly = atoi(ptr);
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 296
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 272
ADDRLP4 296
INDIRI4
ASGNI4
line 533
;533:	for (ptr = cmd; *cmd && *cmd > ' '; cmd++);
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $149
JUMPV
LABELV $146
LABELV $147
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $149
ADDRLP4 300
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 300
INDIRI4
CNSTI4 0
EQI4 $150
ADDRLP4 300
INDIRI4
CNSTI4 32
GTI4 $146
LABELV $150
ADDRGP4 $152
JUMPV
LABELV $151
line 534
;534:	while (*cmd && *cmd <= ' ') *cmd++ = '\0';
ADDRLP4 304
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 304
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 304
INDIRP4
CNSTI1 0
ASGNI1
LABELV $152
ADDRLP4 308
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 308
INDIRI4
CNSTI4 0
EQI4 $154
ADDRLP4 308
INDIRI4
CNSTI4 32
LEI4 $151
LABELV $154
line 535
;535:	clientNum = atoi(ptr);
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 312
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 312
INDIRI4
ASGNI4
line 536
;536:	for (ptr = cmd; *cmd && *cmd > ' '; cmd++);
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $158
JUMPV
LABELV $155
LABELV $156
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $158
ADDRLP4 316
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 316
INDIRI4
CNSTI4 0
EQI4 $159
ADDRLP4 316
INDIRI4
CNSTI4 32
GTI4 $155
LABELV $159
ADDRGP4 $161
JUMPV
LABELV $160
line 537
;537:	while (*cmd && *cmd <= ' ') *cmd++ = '\0';
ADDRLP4 320
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 320
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 320
INDIRP4
CNSTI1 0
ASGNI1
LABELV $161
ADDRLP4 324
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 324
INDIRI4
CNSTI4 0
EQI4 $163
ADDRLP4 324
INDIRI4
CNSTI4 32
LEI4 $160
LABELV $163
line 538
;538:	color = atoi(ptr);
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 328
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 276
ADDRLP4 328
INDIRI4
ASGNI4
line 540
;539:
;540:	if (!BotSameTeam(bs, clientNum)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 332
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 332
INDIRI4
CNSTI4 0
NEI4 $164
line 541
;541:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $132
JUMPV
LABELV $164
line 544
;542:	}
;543:
;544:	for (i = 0; voiceCommands[i].cmd; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $169
JUMPV
LABELV $166
line 545
;545:		if (!Q_stricmp(cmd, voiceCommands[i].cmd)) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 voiceCommands
ADDP4
INDIRP4
ARGP4
ADDRLP4 336
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 336
INDIRI4
CNSTI4 0
NEI4 $170
line 546
;546:			voiceCommands[i].func(bs, clientNum, mode);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 voiceCommands+4
ADDP4
INDIRP4
CALLV
pop
line 547
;547:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $132
JUMPV
LABELV $170
line 549
;548:		}
;549:	}
LABELV $167
line 544
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $169
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 voiceCommands
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $166
line 550
;550:	return qfalse;
CNSTI4 0
RETI4
LABELV $132
endproc BotVoiceChatCommand 340 12
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
LABELV $131
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 111
byte 1 110
byte 1 111
byte 1 102
byte 1 102
byte 1 101
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $130
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 111
byte 1 110
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $129
byte 1 119
byte 1 104
byte 1 111
byte 1 105
byte 1 115
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $128
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $127
byte 1 114
byte 1 101
byte 1 116
byte 1 117
byte 1 114
byte 1 110
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $126
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 99
byte 1 97
byte 1 114
byte 1 114
byte 1 105
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $125
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $123
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $122
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $121
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $120
byte 1 111
byte 1 102
byte 1 102
byte 1 101
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $119
byte 1 103
byte 1 101
byte 1 116
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $116
byte 1 121
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $115
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
LABELV $113
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $112
byte 1 105
byte 1 97
byte 1 109
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $87
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
LABELV $77
byte 1 111
byte 1 110
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $76
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
