export BotValidTeamLeader
code
proc BotValidTeamLeader 8 4
file "../ai_team.c"
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
;25: * name:		ai_team.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_team.c $
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
;51:
;52:#include "match.h"
;53:
;54:// for the voice chats
;55:// #include "../../ui/menudef.h"
;56:#include "../ui/menudef.h"
;57:
;58://ctf task preferences for a client
;59:typedef struct bot_ctftaskpreference_s
;60:{
;61:	char		name[36];
;62:	int			preference;
;63:} bot_ctftaskpreference_t;
;64:
;65:bot_ctftaskpreference_t ctftaskpreferences[MAX_CLIENTS];
;66:
;67:
;68:/*
;69:==================
;70:BotValidTeamLeader
;71:==================
;72:*/
;73:int BotValidTeamLeader(bot_state_t *bs) {
line 74
;74:	if (!strlen(bs->teamleader)) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $54
CNSTI4 0
RETI4
ADDRGP4 $53
JUMPV
LABELV $54
line 75
;75:	if (ClientFromName(bs->teamleader) == -1) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 ClientFromName
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 -1
NEI4 $56
CNSTI4 0
RETI4
ADDRGP4 $53
JUMPV
LABELV $56
line 76
;76:	return qtrue;
CNSTI4 1
RETI4
LABELV $53
endproc BotValidTeamLeader 8 4
bss
align 4
LABELV $59
skip 4
export BotNumTeamMates
code
proc BotNumTeamMates 1060 12
line 84
;77:}
;78:
;79:/*
;80:==================
;81:BotNumTeamMates
;82:==================
;83:*/
;84:int BotNumTeamMates(bot_state_t *bs) {
line 89
;85:	int i, numplayers;
;86:	char buf[MAX_INFO_STRING];
;87:	static int maxclients;
;88:
;89:	if (!maxclients)
ADDRGP4 $59
INDIRI4
CNSTI4 0
NEI4 $60
line 90
;90:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $62
ARGP4
ADDRLP4 1032
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $59
ADDRLP4 1032
INDIRI4
ASGNI4
LABELV $60
line 92
;91:
;92:	numplayers = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 93
;93:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $66
JUMPV
LABELV $63
line 94
;94:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 96
;95:		//if no config string or no name
;96:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $70
ADDRLP4 4
ARGP4
ADDRGP4 $69
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
NEI4 $67
LABELV $70
ADDRGP4 $64
JUMPV
LABELV $67
line 98
;97:		//skip spectators
;98:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $73
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
CNSTI4 3
NEI4 $71
ADDRGP4 $64
JUMPV
LABELV $71
line 100
;99:		//
;100:		if (BotSameTeam(bs, i)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1056
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $74
line 101
;101:			numplayers++;
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 102
;102:		}
LABELV $74
line 103
;103:	}
LABELV $64
line 93
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $66
ADDRLP4 0
INDIRI4
ADDRGP4 $59
INDIRI4
GEI4 $76
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $63
LABELV $76
line 104
;104:	return numplayers;
ADDRLP4 1028
INDIRI4
RETI4
LABELV $58
endproc BotNumTeamMates 1060 12
export BotClientTravelTimeToGoal
proc BotClientTravelTimeToGoal 480 16
line 112
;105:}
;106:
;107:/*
;108:==================
;109:BotClientTravelTimeToGoal
;110:==================
;111:*/
;112:int BotClientTravelTimeToGoal(int client, bot_goal_t *goal) {
line 116
;113:	playerState_t ps;
;114:	int areanum;
;115:
;116:	BotAI_GetClientState(client, &ps);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 117
;117:	areanum = BotPointAreaNum(ps.origin);
ADDRLP4 0+20
ARGP4
ADDRLP4 472
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 468
ADDRLP4 472
INDIRI4
ASGNI4
line 118
;118:	if (!areanum) return 1;
ADDRLP4 468
INDIRI4
CNSTI4 0
NEI4 $79
CNSTI4 1
RETI4
ADDRGP4 $77
JUMPV
LABELV $79
line 119
;119:	return trap_AAS_AreaTravelTimeToGoalArea(areanum, ps.origin, goal->areanum, TFL_DEFAULT);
ADDRLP4 468
INDIRI4
ARGI4
ADDRLP4 0+20
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 476
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 476
INDIRI4
RETI4
LABELV $77
endproc BotClientTravelTimeToGoal 480 16
bss
align 4
LABELV $83
skip 4
export BotSortTeamMatesByBaseTravelTime
code
proc BotSortTeamMatesByBaseTravelTime 1352 12
line 127
;120:}
;121:
;122:/*
;123:==================
;124:BotSortTeamMatesByBaseTravelTime
;125:==================
;126:*/
;127:int BotSortTeamMatesByBaseTravelTime(bot_state_t *bs, int *teammates, int maxteammates) {
line 133
;128:
;129:	int i, j, k, numteammates, traveltime;
;130:	char buf[MAX_INFO_STRING];
;131:	static int maxclients;
;132:	int traveltimes[MAX_CLIENTS];
;133:	bot_goal_t *goal = NULL;
ADDRLP4 1300
CNSTP4 0
ASGNP4
line 135
;134:
;135:	if (gametype == GT_CTF || gametype == GT_1FCTF) {
ADDRLP4 1304
ADDRGP4 gametype
INDIRI4
ASGNI4
ADDRLP4 1304
INDIRI4
CNSTI4 4
EQI4 $86
ADDRLP4 1304
INDIRI4
CNSTI4 5
NEI4 $84
LABELV $86
line 136
;136:		if (BotTeam(bs) == TEAM_RED)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1308
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 1308
INDIRI4
CNSTI4 1
NEI4 $87
line 137
;137:			goal = &ctf_redflag;
ADDRLP4 1300
ADDRGP4 ctf_redflag
ASGNP4
ADDRGP4 $88
JUMPV
LABELV $87
line 139
;138:		else
;139:			goal = &ctf_blueflag;
ADDRLP4 1300
ADDRGP4 ctf_blueflag
ASGNP4
LABELV $88
line 140
;140:	}
LABELV $84
line 149
;141:#ifdef MISSIONPACK
;142:	else {
;143:		if (BotTeam(bs) == TEAM_RED)
;144:			goal = &redobelisk;
;145:		else
;146:			goal = &blueobelisk;
;147:	}
;148:#endif
;149:	if (!maxclients)
ADDRGP4 $83
INDIRI4
CNSTI4 0
NEI4 $89
line 150
;150:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $62
ARGP4
ADDRLP4 1308
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $83
ADDRLP4 1308
INDIRI4
ASGNI4
LABELV $89
line 152
;151:
;152:	numteammates = 0;
ADDRLP4 264
CNSTI4 0
ASGNI4
line 153
;153:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 268
CNSTI4 0
ASGNI4
ADDRGP4 $94
JUMPV
LABELV $91
line 154
;154:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
ADDRLP4 268
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 276
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 156
;155:		//if no config string or no name
;156:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 276
ARGP4
ADDRLP4 1312
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1312
INDIRI4
CNSTI4 0
EQI4 $97
ADDRLP4 276
ARGP4
ADDRGP4 $69
ARGP4
ADDRLP4 1316
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1316
INDIRP4
ARGP4
ADDRLP4 1320
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1320
INDIRI4
CNSTI4 0
NEI4 $95
LABELV $97
ADDRGP4 $92
JUMPV
LABELV $95
line 158
;157:		//skip spectators
;158:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 276
ARGP4
ADDRGP4 $73
ARGP4
ADDRLP4 1324
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1324
INDIRP4
ARGP4
ADDRLP4 1328
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1328
INDIRI4
CNSTI4 3
NEI4 $98
ADDRGP4 $92
JUMPV
LABELV $98
line 160
;159:		//
;160:		if (BotSameTeam(bs, i)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 268
INDIRI4
ARGI4
ADDRLP4 1332
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1332
INDIRI4
CNSTI4 0
EQI4 $100
line 162
;161:			//
;162:			traveltime = BotClientTravelTimeToGoal(i, goal);
ADDRLP4 268
INDIRI4
ARGI4
ADDRLP4 1300
INDIRP4
ARGP4
ADDRLP4 1336
ADDRGP4 BotClientTravelTimeToGoal
CALLI4
ASGNI4
ADDRLP4 272
ADDRLP4 1336
INDIRI4
ASGNI4
line 164
;163:			//
;164:			for (j = 0; j < numteammates; j++) {
ADDRLP4 260
CNSTI4 0
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $102
line 165
;165:				if (traveltime < traveltimes[j]) {
ADDRLP4 272
INDIRI4
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
GEI4 $106
line 166
;166:					for (k = numteammates; k > j; k--) {
ADDRLP4 0
ADDRLP4 264
INDIRI4
ASGNI4
ADDRGP4 $111
JUMPV
LABELV $108
line 167
;167:						traveltimes[k] = traveltimes[k-1];
ADDRLP4 1340
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1340
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 1340
INDIRI4
ADDRLP4 4-4
ADDP4
INDIRI4
ASGNI4
line 168
;168:						teammates[k] = teammates[k-1];
ADDRLP4 1344
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1348
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 1344
INDIRI4
ADDRLP4 1348
INDIRP4
ADDP4
ADDRLP4 1344
INDIRI4
CNSTI4 4
SUBI4
ADDRLP4 1348
INDIRP4
ADDP4
INDIRI4
ASGNI4
line 169
;169:					}
LABELV $109
line 166
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $111
ADDRLP4 0
INDIRI4
ADDRLP4 260
INDIRI4
GTI4 $108
line 170
;170:					break;
ADDRGP4 $104
JUMPV
LABELV $106
line 172
;171:				}
;172:			}
LABELV $103
line 164
ADDRLP4 260
ADDRLP4 260
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $105
ADDRLP4 260
INDIRI4
ADDRLP4 264
INDIRI4
LTI4 $102
LABELV $104
line 173
;173:			traveltimes[j] = traveltime;
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 272
INDIRI4
ASGNI4
line 174
;174:			teammates[j] = i;
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ADDRLP4 268
INDIRI4
ASGNI4
line 175
;175:			numteammates++;
ADDRLP4 264
ADDRLP4 264
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 176
;176:			if (numteammates >= maxteammates) break;
ADDRLP4 264
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $113
ADDRGP4 $93
JUMPV
LABELV $113
line 177
;177:		}
LABELV $100
line 178
;178:	}
LABELV $92
line 153
ADDRLP4 268
ADDRLP4 268
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $94
ADDRLP4 268
INDIRI4
ADDRGP4 $83
INDIRI4
GEI4 $115
ADDRLP4 268
INDIRI4
CNSTI4 64
LTI4 $91
LABELV $115
LABELV $93
line 179
;179:	return numteammates;
ADDRLP4 264
INDIRI4
RETI4
LABELV $82
endproc BotSortTeamMatesByBaseTravelTime 1352 12
export BotSetTeamMateTaskPreference
proc BotSetTeamMateTaskPreference 36 12
line 187
;180:}
;181:
;182:/*
;183:==================
;184:BotSetTeamMateTaskPreference
;185:==================
;186:*/
;187:void BotSetTeamMateTaskPreference(bot_state_t *bs, int teammate, int preference) {
line 190
;188:	char teammatename[MAX_NETNAME];
;189:
;190:	ctftaskpreferences[teammate].preference = preference;
CNSTI4 40
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 ctftaskpreferences+36
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 191
;191:	ClientName(teammate, teammatename, sizeof(teammatename));
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 192
;192:	strcpy(ctftaskpreferences[teammate].name, teammatename);
CNSTI4 40
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 ctftaskpreferences
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 193
;193:}
LABELV $116
endproc BotSetTeamMateTaskPreference 36 12
export BotGetTeamMateTaskPreference
proc BotGetTeamMateTaskPreference 40 12
line 200
;194:
;195:/*
;196:==================
;197:BotGetTeamMateTaskPreference
;198:==================
;199:*/
;200:int BotGetTeamMateTaskPreference(bot_state_t *bs, int teammate) {
line 203
;201:	char teammatename[MAX_NETNAME];
;202:
;203:	if (!ctftaskpreferences[teammate].preference) return 0;
CNSTI4 40
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 ctftaskpreferences+36
ADDP4
INDIRI4
CNSTI4 0
NEI4 $119
CNSTI4 0
RETI4
ADDRGP4 $118
JUMPV
LABELV $119
line 204
;204:	ClientName(teammate, teammatename, sizeof(teammatename));
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 205
;205:	if (Q_stricmp(teammatename, ctftaskpreferences[teammate].name)) return 0;
ADDRLP4 0
ARGP4
CNSTI4 40
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 ctftaskpreferences
ADDP4
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $122
CNSTI4 0
RETI4
ADDRGP4 $118
JUMPV
LABELV $122
line 206
;206:	return ctftaskpreferences[teammate].preference;
CNSTI4 40
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 ctftaskpreferences+36
ADDP4
INDIRI4
RETI4
LABELV $118
endproc BotGetTeamMateTaskPreference 40 12
export BotSortTeamMatesByTaskPreference
proc BotSortTeamMatesByTaskPreference 804 12
line 214
;207:}
;208:
;209:/*
;210:==================
;211:BotSortTeamMatesByTaskPreference
;212:==================
;213:*/
;214:int BotSortTeamMatesByTaskPreference(bot_state_t *bs, int *teammates, int numteammates) {
line 220
;215:	int defenders[MAX_CLIENTS], numdefenders;
;216:	int attackers[MAX_CLIENTS], numattackers;
;217:	int roamers[MAX_CLIENTS], numroamers;
;218:	int i, preference;
;219:
;220:	numdefenders = numattackers = numroamers = 0;
ADDRLP4 788
CNSTI4 0
ASGNI4
ADDRLP4 272
ADDRLP4 788
INDIRI4
ASGNI4
ADDRLP4 268
ADDRLP4 788
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 788
INDIRI4
ASGNI4
line 221
;221:	for (i = 0; i < numteammates; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $129
JUMPV
LABELV $126
line 222
;222:		preference = BotGetTeamMateTaskPreference(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ARGI4
ADDRLP4 792
ADDRGP4 BotGetTeamMateTaskPreference
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 792
INDIRI4
ASGNI4
line 223
;223:		if (preference & TEAMTP_DEFENDER) {
ADDRLP4 4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $130
line 224
;224:			defenders[numdefenders++] = teammates[i];
ADDRLP4 796
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 796
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 800
CNSTI4 2
ASGNI4
ADDRLP4 796
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRLP4 12
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ASGNI4
line 225
;225:		}
ADDRGP4 $131
JUMPV
LABELV $130
line 226
;226:		else if (preference & TEAMTP_ATTACKER) {
ADDRLP4 4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $132
line 227
;227:			attackers[numattackers++] = teammates[i];
ADDRLP4 796
ADDRLP4 268
INDIRI4
ASGNI4
ADDRLP4 268
ADDRLP4 796
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 800
CNSTI4 2
ASGNI4
ADDRLP4 796
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRLP4 276
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ASGNI4
line 228
;228:		}
ADDRGP4 $133
JUMPV
LABELV $132
line 229
;229:		else {
line 230
;230:			roamers[numroamers++] = teammates[i];
ADDRLP4 796
ADDRLP4 272
INDIRI4
ASGNI4
ADDRLP4 272
ADDRLP4 796
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 800
CNSTI4 2
ASGNI4
ADDRLP4 796
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRLP4 532
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ASGNI4
line 231
;231:		}
LABELV $133
LABELV $131
line 232
;232:	}
LABELV $127
line 221
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $129
ADDRLP4 0
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $126
line 233
;233:	numteammates = 0;
ADDRFP4 8
CNSTI4 0
ASGNI4
line 235
;234:	//defenders at the front of the list
;235:	memcpy(&teammates[numteammates], defenders, numdefenders * sizeof(int));
ADDRLP4 792
CNSTI4 2
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 792
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRI4
CVIU4 4
ADDRLP4 792
INDIRI4
LSHU4
CVUI4 4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 236
;236:	numteammates += numdefenders;
ADDRFP4 8
ADDRFP4 8
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 238
;237:	//roamers in the middle
;238:	memcpy(&teammates[numteammates], roamers, numroamers * sizeof(int));
ADDRLP4 796
CNSTI4 2
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 796
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ARGP4
ADDRLP4 532
ARGP4
ADDRLP4 272
INDIRI4
CVIU4 4
ADDRLP4 796
INDIRI4
LSHU4
CVUI4 4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 239
;239:	numteammates += numroamers;
ADDRFP4 8
ADDRFP4 8
INDIRI4
ADDRLP4 272
INDIRI4
ADDI4
ASGNI4
line 241
;240:	//attacker in the back of the list
;241:	memcpy(&teammates[numteammates], attackers, numattackers * sizeof(int));
ADDRLP4 800
CNSTI4 2
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 800
INDIRI4
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ARGP4
ADDRLP4 276
ARGP4
ADDRLP4 268
INDIRI4
CVIU4 4
ADDRLP4 800
INDIRI4
LSHU4
CVUI4 4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 242
;242:	numteammates += numattackers;
ADDRFP4 8
ADDRFP4 8
INDIRI4
ADDRLP4 268
INDIRI4
ADDI4
ASGNI4
line 244
;243:
;244:	return numteammates;
ADDRFP4 8
INDIRI4
RETI4
LABELV $125
endproc BotSortTeamMatesByTaskPreference 804 12
export BotSayTeamOrderAlways
proc BotSayTeamOrderAlways 548 20
line 252
;245:}
;246:
;247:/*
;248:==================
;249:BotSayTeamOrders
;250:==================
;251:*/
;252:void BotSayTeamOrderAlways(bot_state_t *bs, int toclient) {
line 258
;253:	char teamchat[MAX_MESSAGE_SIZE];
;254:	char buf[MAX_MESSAGE_SIZE];
;255:	char name[MAX_NETNAME];
;256:
;257:	//if the bot is talking to itself
;258:	if (bs->client == toclient) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $135
line 260
;259:		//don't show the message just put it in the console message queue
;260:		trap_BotGetChatMessage(bs->cs, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGetChatMessage
CALLV
pop
line 261
;261:		ClientName(bs->client, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 512
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 262
;262:		Com_sprintf(teamchat, sizeof(teamchat), EC"(%s"EC")"EC": %s", name, buf);
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $137
ARGP4
ADDRLP4 512
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 263
;263:		trap_BotQueueConsoleMessage(bs->cs, CMS_CHAT, teamchat);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotQueueConsoleMessage
CALLV
pop
line 264
;264:	}
ADDRGP4 $136
JUMPV
LABELV $135
line 265
;265:	else {
line 266
;266:		trap_BotEnterChat(bs->cs, toclient, CHAT_TELL);
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
line 267
;267:	}
LABELV $136
line 268
;268:}
LABELV $134
endproc BotSayTeamOrderAlways 548 20
export BotSayTeamOrder
proc BotSayTeamOrder 0 8
line 275
;269:
;270:/*
;271:==================
;272:BotSayTeamOrders
;273:==================
;274:*/
;275:void BotSayTeamOrder(bot_state_t *bs, int toclient) {
line 282
;276:#ifdef MISSIONPACK
;277:	// voice chats only
;278:	char buf[MAX_MESSAGE_SIZE];
;279:
;280:	trap_BotGetChatMessage(bs->cs, buf, sizeof(buf));
;281:#else
;282:	BotSayTeamOrderAlways(bs, toclient);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrderAlways
CALLV
pop
line 284
;283:#endif
;284:}
LABELV $138
endproc BotSayTeamOrder 0 8
export BotVoiceChat
proc BotVoiceChat 0 0
line 291
;285:
;286:/*
;287:==================
;288:BotVoiceChat
;289:==================
;290:*/
;291:void BotVoiceChat(bot_state_t *bs, int toclient, char *voicechat) {
line 300
;292:#ifdef MISSIONPACK
;293:	if (toclient == -1)
;294:		// voice only say team
;295:		trap_EA_Command(bs->client, va("vsay_team %s", voicechat));
;296:	else
;297:		// voice only tell single player
;298:		trap_EA_Command(bs->client, va("vtell %d %s", toclient, voicechat));
;299:#endif
;300:}
LABELV $139
endproc BotVoiceChat 0 0
export BotVoiceChatOnly
proc BotVoiceChatOnly 0 0
line 307
;301:
;302:/*
;303:==================
;304:BotVoiceChatOnly
;305:==================
;306:*/
;307:void BotVoiceChatOnly(bot_state_t *bs, int toclient, char *voicechat) {
line 316
;308:#ifdef MISSIONPACK
;309:	if (toclient == -1)
;310:		// voice only say team
;311:		trap_EA_Command(bs->client, va("vosay_team %s", voicechat));
;312:	else
;313:		// voice only tell single player
;314:		trap_EA_Command(bs->client, va("votell %d %s", toclient, voicechat));
;315:#endif
;316:}
LABELV $140
endproc BotVoiceChatOnly 0 0
export BotSayVoiceTeamOrder
proc BotSayVoiceTeamOrder 0 0
line 323
;317:
;318:/*
;319:==================
;320:BotSayVoiceTeamOrder
;321:==================
;322:*/
;323:void BotSayVoiceTeamOrder(bot_state_t *bs, int toclient, char *voicechat) {
line 327
;324:#ifdef MISSIONPACK
;325:	BotVoiceChat(bs, toclient, voicechat);
;326:#endif
;327:}
LABELV $141
endproc BotSayVoiceTeamOrder 0 0
export BotCTFOrders_BothFlagsNotAtBase
proc BotCTFOrders_BothFlagsNotAtBase 368 20
line 334
;328:
;329:/*
;330:==================
;331:BotCTFOrders
;332:==================
;333:*/
;334:void BotCTFOrders_BothFlagsNotAtBase(bot_state_t *bs) {
line 339
;335:	int numteammates, defenders, attackers, i, other;
;336:	int teammates[MAX_CLIENTS];
;337:	char name[MAX_NETNAME], carriername[MAX_NETNAME];
;338:
;339:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 348
ADDRGP4 BotSortTeamMatesByBaseTravelTime
CALLI4
ASGNI4
ADDRLP4 296
ADDRLP4 348
INDIRI4
ASGNI4
line 340
;340:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 296
INDIRI4
ARGI4
ADDRGP4 BotSortTeamMatesByTaskPreference
CALLI4
pop
line 342
;341:	//different orders based on the number of team mates
;342:	switch(bs->numteammates) {
ADDRLP4 352
ADDRFP4 0
INDIRP4
CNSTI4 6948
ADDP4
INDIRI4
ASGNI4
ADDRLP4 352
INDIRI4
CNSTI4 1
EQI4 $144
ADDRLP4 352
INDIRI4
CNSTI4 2
EQI4 $147
ADDRLP4 352
INDIRI4
CNSTI4 3
EQI4 $153
ADDRGP4 $143
JUMPV
line 343
;343:		case 1: break;
LABELV $147
line 345
;344:		case 2:
;345:		{
line 347
;346:			//tell the one not carrying the flag to attack the enemy base
;347:			if (teammates[0] != bs->flagcarrier) other = teammates[0];
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
EQI4 $148
ADDRLP4 308
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $149
JUMPV
LABELV $148
line 348
;348:			else other = teammates[1];
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
LABELV $149
line 349
;349:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 350
;350:			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 351
;351:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 352
;352:			BotSayVoiceTeamOrder(bs, other, VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 353
;353:			break;
ADDRGP4 $144
JUMPV
LABELV $153
line 356
;354:		}
;355:		case 3:
;356:		{
line 358
;357:			//tell the one closest to the base not carrying the flag to accompany the flag carrier
;358:			if (teammates[0] != bs->flagcarrier) other = teammates[0];
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
EQI4 $154
ADDRLP4 308
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $155
JUMPV
LABELV $154
line 359
;359:			else other = teammates[1];
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
LABELV $155
line 360
;360:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 361
;361:			if ( bs->flagcarrier != -1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $157
line 362
;362:				ClientName(bs->flagcarrier, carriername, sizeof(carriername));
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ARGI4
ADDRLP4 312
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 363
;363:				if (bs->flagcarrier == bs->client) {
ADDRLP4 360
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 360
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ADDRLP4 360
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $159
line 364
;364:					BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $161
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 365
;365:					BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWME);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $162
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 366
;366:				}
ADDRGP4 $158
JUMPV
LABELV $159
line 367
;367:				else {
line 368
;368:					BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $163
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 312
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 369
;369:					BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWFLAGCARRIER);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $164
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 370
;370:				}
line 371
;371:			}
ADDRGP4 $158
JUMPV
LABELV $157
line 372
;372:			else {
line 374
;373:				//
;374:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 375
;375:				BotSayVoiceTeamOrder(bs, other, VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 376
;376:			}
LABELV $158
line 377
;377:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 379
;378:			//tell the one furthest from the the base not carrying the flag to get the enemy flag
;379:			if (teammates[2] != bs->flagcarrier) other = teammates[2];
ADDRLP4 4+8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
EQI4 $165
ADDRLP4 308
ADDRLP4 4+8
INDIRI4
ASGNI4
ADDRGP4 $166
JUMPV
LABELV $165
line 380
;380:			else other = teammates[1];
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
LABELV $166
line 381
;381:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 382
;382:			BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 383
;383:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 384
;384:			BotSayVoiceTeamOrder(bs, other, VOICECHAT_RETURNFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $170
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 385
;385:			break;
ADDRGP4 $144
JUMPV
LABELV $143
line 388
;386:		}
;387:		default:
;388:		{
line 389
;389:			defenders = (int) (float) numteammates * 0.4 + 0.5;
ADDRLP4 300
CNSTF4 1053609165
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 390
;390:			if (defenders > 4) defenders = 4;
ADDRLP4 300
INDIRI4
CNSTI4 4
LEI4 $171
ADDRLP4 300
CNSTI4 4
ASGNI4
LABELV $171
line 391
;391:			attackers = (int) (float) numteammates * 0.5 + 0.5;
ADDRLP4 360
CNSTF4 1056964608
ASGNF4
ADDRLP4 304
ADDRLP4 360
INDIRF4
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
ADDRLP4 360
INDIRF4
ADDF4
CVFI4 4
ASGNI4
line 392
;392:			if (attackers > 5) attackers = 5;
ADDRLP4 304
INDIRI4
CNSTI4 5
LEI4 $173
ADDRLP4 304
CNSTI4 5
ASGNI4
LABELV $173
line 393
;393:			if (bs->flagcarrier != -1) {
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $175
line 394
;394:				ClientName(bs->flagcarrier, carriername, sizeof(carriername));
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ARGI4
ADDRLP4 312
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 395
;395:				for (i = 0; i < defenders; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $180
JUMPV
LABELV $177
line 397
;396:					//
;397:					if (teammates[i] == bs->flagcarrier) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $181
line 398
;398:						continue;
ADDRGP4 $178
JUMPV
LABELV $181
line 401
;399:					}
;400:					//
;401:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
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
line 402
;402:					if (bs->flagcarrier == bs->client) {
ADDRLP4 364
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 364
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ADDRLP4 364
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $183
line 403
;403:						BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $161
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 404
;404:						BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_FOLLOWME);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $162
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 405
;405:					}
ADDRGP4 $184
JUMPV
LABELV $183
line 406
;406:					else {
line 407
;407:						BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $163
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 312
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 408
;408:						BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_FOLLOWFLAGCARRIER);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $164
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 409
;409:					}
LABELV $184
line 410
;410:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 411
;411:				}
LABELV $178
line 395
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $180
ADDRLP4 0
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $177
line 412
;412:			}
ADDRGP4 $176
JUMPV
LABELV $175
line 413
;413:			else {
line 414
;414:				for (i = 0; i < defenders; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $188
JUMPV
LABELV $185
line 416
;415:					//
;416:					if (teammates[i] == bs->flagcarrier) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $189
line 417
;417:						continue;
ADDRGP4 $186
JUMPV
LABELV $189
line 420
;418:					}
;419:					//
;420:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
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
line 421
;421:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 422
;422:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 423
;423:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 424
;424:				}
LABELV $186
line 414
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $188
ADDRLP4 0
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $185
line 425
;425:			}
LABELV $176
line 426
;426:			for (i = 0; i < attackers; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $194
JUMPV
LABELV $191
line 428
;427:				//
;428:				if (teammates[numteammates - i - 1] == bs->flagcarrier) {
ADDRLP4 296
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $195
line 429
;429:					continue;
ADDRGP4 $192
JUMPV
LABELV $195
line 432
;430:				}
;431:				//
;432:				ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 296
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
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
line 433
;433:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 434
;434:				BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 435
;435:				BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_RETURNFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $170
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 436
;436:			}
LABELV $192
line 426
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $194
ADDRLP4 0
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $191
line 438
;437:			//
;438:			break;
LABELV $144
line 441
;439:		}
;440:	}
;441:}
LABELV $142
endproc BotCTFOrders_BothFlagsNotAtBase 368 20
export BotCTFOrders_FlagNotAtBase
proc BotCTFOrders_FlagNotAtBase 320 16
line 448
;442:
;443:/*
;444:==================
;445:BotCTFOrders
;446:==================
;447:*/
;448:void BotCTFOrders_FlagNotAtBase(bot_state_t *bs) {
line 453
;449:	int numteammates, defenders, attackers, i;
;450:	int teammates[MAX_CLIENTS];
;451:	char name[MAX_NETNAME];
;452:
;453:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 308
ADDRGP4 BotSortTeamMatesByBaseTravelTime
CALLI4
ASGNI4
ADDRLP4 296
ADDRLP4 308
INDIRI4
ASGNI4
line 454
;454:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 296
INDIRI4
ARGI4
ADDRGP4 BotSortTeamMatesByTaskPreference
CALLI4
pop
line 456
;455:	//passive strategy
;456:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
ADDRFP4 0
INDIRP4
CNSTI4 6976
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $202
line 458
;457:		//different orders based on the number of team mates
;458:		switch(bs->numteammates) {
ADDRLP4 312
ADDRFP4 0
INDIRP4
CNSTI4 6948
ADDP4
INDIRI4
ASGNI4
ADDRLP4 312
INDIRI4
CNSTI4 1
EQI4 $203
ADDRLP4 312
INDIRI4
CNSTI4 2
EQI4 $208
ADDRLP4 312
INDIRI4
CNSTI4 3
EQI4 $213
ADDRGP4 $204
JUMPV
line 459
;459:			case 1: break;
LABELV $208
line 461
;460:			case 2:
;461:			{
line 463
;462:				//both will go for the enemy flag
;463:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 464
;464:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 465
;465:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 466
;466:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 468
;467:				//
;468:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 469
;469:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 470
;470:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 471
;471:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 472
;472:				break;
ADDRGP4 $203
JUMPV
LABELV $213
line 475
;473:			}
;474:			case 3:
;475:			{
line 477
;476:				//keep one near the base for when the flag is returned
;477:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 478
;478:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 479
;479:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 480
;480:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 482
;481:				//the other two get the flag
;482:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 483
;483:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 484
;484:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 485
;485:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 487
;486:				//
;487:				ClientName(teammates[2], name, sizeof(name));
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 488
;488:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 489
;489:				BotSayTeamOrder(bs, teammates[2]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 490
;490:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 491
;491:				break;
ADDRGP4 $203
JUMPV
LABELV $204
line 494
;492:			}
;493:			default:
;494:			{
line 496
;495:				//keep some people near the base for when the flag is returned
;496:				defenders = (int) (float) numteammates * 0.3 + 0.5;
ADDRLP4 300
CNSTF4 1050253722
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 497
;497:				if (defenders > 3) defenders = 3;
ADDRLP4 300
INDIRI4
CNSTI4 3
LEI4 $221
ADDRLP4 300
CNSTI4 3
ASGNI4
LABELV $221
line 498
;498:				attackers = (int) (float) numteammates * 0.7 + 0.5;
ADDRLP4 304
CNSTF4 1060320051
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 499
;499:				if (attackers > 6) attackers = 6;
ADDRLP4 304
INDIRI4
CNSTI4 6
LEI4 $223
ADDRLP4 304
CNSTI4 6
ASGNI4
LABELV $223
line 500
;500:				for (i = 0; i < defenders; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $228
JUMPV
LABELV $225
line 502
;501:					//
;502:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
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
line 503
;503:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 504
;504:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 505
;505:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 506
;506:				}
LABELV $226
line 500
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $228
ADDRLP4 256
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $225
line 507
;507:				for (i = 0; i < attackers; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $232
JUMPV
LABELV $229
line 509
;508:					//
;509:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
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
line 510
;510:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 511
;511:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 512
;512:					BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 513
;513:				}
LABELV $230
line 507
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $232
ADDRLP4 256
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $229
line 515
;514:				//
;515:				break;
line 518
;516:			}
;517:		}
;518:	}
ADDRGP4 $203
JUMPV
LABELV $202
line 519
;519:	else {
line 521
;520:		//different orders based on the number of team mates
;521:		switch(bs->numteammates) {
ADDRLP4 312
ADDRFP4 0
INDIRP4
CNSTI4 6948
ADDP4
INDIRI4
ASGNI4
ADDRLP4 312
INDIRI4
CNSTI4 1
EQI4 $236
ADDRLP4 312
INDIRI4
CNSTI4 2
EQI4 $239
ADDRLP4 312
INDIRI4
CNSTI4 3
EQI4 $243
ADDRGP4 $235
JUMPV
line 522
;522:			case 1: break;
LABELV $239
line 524
;523:			case 2:
;524:			{
line 526
;525:				//both will go for the enemy flag
;526:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 527
;527:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 528
;528:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 529
;529:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 531
;530:				//
;531:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 532
;532:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 533
;533:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 534
;534:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 535
;535:				break;
ADDRGP4 $236
JUMPV
LABELV $243
line 538
;536:			}
;537:			case 3:
;538:			{
line 540
;539:				//everyone go for the flag
;540:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 541
;541:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 542
;542:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 543
;543:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 545
;544:				//
;545:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 546
;546:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 547
;547:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 548
;548:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 550
;549:				//
;550:				ClientName(teammates[2], name, sizeof(name));
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 551
;551:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 552
;552:				BotSayTeamOrder(bs, teammates[2]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 553
;553:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 554
;554:				break;
ADDRGP4 $236
JUMPV
LABELV $235
line 557
;555:			}
;556:			default:
;557:			{
line 559
;558:				//keep some people near the base for when the flag is returned
;559:				defenders = (int) (float) numteammates * 0.2 + 0.5;
ADDRLP4 300
CNSTF4 1045220557
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 560
;560:				if (defenders > 2) defenders = 2;
ADDRLP4 300
INDIRI4
CNSTI4 2
LEI4 $250
ADDRLP4 300
CNSTI4 2
ASGNI4
LABELV $250
line 561
;561:				attackers = (int) (float) numteammates * 0.7 + 0.5;
ADDRLP4 304
CNSTF4 1060320051
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 562
;562:				if (attackers > 7) attackers = 7;
ADDRLP4 304
INDIRI4
CNSTI4 7
LEI4 $252
ADDRLP4 304
CNSTI4 7
ASGNI4
LABELV $252
line 563
;563:				for (i = 0; i < defenders; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $257
JUMPV
LABELV $254
line 565
;564:					//
;565:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
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
line 566
;566:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 567
;567:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 568
;568:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 569
;569:				}
LABELV $255
line 563
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $257
ADDRLP4 256
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $254
line 570
;570:				for (i = 0; i < attackers; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $261
JUMPV
LABELV $258
line 572
;571:					//
;572:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
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
line 573
;573:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 574
;574:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 575
;575:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 576
;576:				}
LABELV $259
line 570
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $261
ADDRLP4 256
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $258
line 578
;577:				//
;578:				break;
LABELV $236
line 581
;579:			}
;580:		}
;581:	}
LABELV $203
line 582
;582:}
LABELV $201
endproc BotCTFOrders_FlagNotAtBase 320 16
export BotCTFOrders_EnemyFlagNotAtBase
proc BotCTFOrders_EnemyFlagNotAtBase 360 20
line 589
;583:
;584:/*
;585:==================
;586:BotCTFOrders
;587:==================
;588:*/
;589:void BotCTFOrders_EnemyFlagNotAtBase(bot_state_t *bs) {
line 594
;590:	int numteammates, defenders, attackers, i, other;
;591:	int teammates[MAX_CLIENTS];
;592:	char name[MAX_NETNAME], carriername[MAX_NETNAME];
;593:
;594:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 348
ADDRGP4 BotSortTeamMatesByBaseTravelTime
CALLI4
ASGNI4
ADDRLP4 260
ADDRLP4 348
INDIRI4
ASGNI4
line 595
;595:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 260
INDIRI4
ARGI4
ADDRGP4 BotSortTeamMatesByTaskPreference
CALLI4
pop
line 597
;596:	//different orders based on the number of team mates
;597:	switch(numteammates) {
ADDRLP4 260
INDIRI4
CNSTI4 1
EQI4 $267
ADDRLP4 260
INDIRI4
CNSTI4 2
EQI4 $269
ADDRLP4 260
INDIRI4
CNSTI4 3
EQI4 $273
ADDRGP4 $266
JUMPV
line 598
;598:		case 1: break;
LABELV $269
line 600
;599:		case 2:
;600:		{
line 602
;601:			//tell the one not carrying the flag to defend the base
;602:			if (teammates[0] == bs->flagcarrier) other = teammates[1];
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $270
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
ADDRGP4 $271
JUMPV
LABELV $270
line 603
;603:			else other = teammates[0];
ADDRLP4 308
ADDRLP4 4
INDIRI4
ASGNI4
LABELV $271
line 604
;604:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 605
;605:			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 606
;606:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 607
;607:			BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 608
;608:			break;
ADDRGP4 $267
JUMPV
LABELV $273
line 611
;609:		}
;610:		case 3:
;611:		{
line 613
;612:			//tell the one closest to the base not carrying the flag to defend the base
;613:			if (teammates[0] != bs->flagcarrier) other = teammates[0];
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
EQI4 $274
ADDRLP4 308
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $275
JUMPV
LABELV $274
line 614
;614:			else other = teammates[1];
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
LABELV $275
line 615
;615:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 616
;616:			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 617
;617:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 618
;618:			BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 620
;619:			//tell the other also to defend the base
;620:			if (teammates[2] != bs->flagcarrier) other = teammates[2];
ADDRLP4 4+8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
EQI4 $277
ADDRLP4 308
ADDRLP4 4+8
INDIRI4
ASGNI4
ADDRGP4 $278
JUMPV
LABELV $277
line 621
;621:			else other = teammates[1];
ADDRLP4 308
ADDRLP4 4+4
INDIRI4
ASGNI4
LABELV $278
line 622
;622:			ClientName(other, name, sizeof(name));
ADDRLP4 308
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 623
;623:			BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 624
;624:			BotSayTeamOrder(bs, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 625
;625:			BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 626
;626:			break;
ADDRGP4 $267
JUMPV
LABELV $266
line 629
;627:		}
;628:		default:
;629:		{
line 631
;630:			//60% will defend the base
;631:			defenders = (int) (float) numteammates * 0.6 + 0.5;
ADDRLP4 300
CNSTF4 1058642330
ADDRLP4 260
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 632
;632:			if (defenders > 6) defenders = 6;
ADDRLP4 300
INDIRI4
CNSTI4 6
LEI4 $282
ADDRLP4 300
CNSTI4 6
ASGNI4
LABELV $282
line 634
;633:			//30% accompanies the flag carrier
;634:			attackers = (int) (float) numteammates * 0.3 + 0.5;
ADDRLP4 304
CNSTF4 1050253722
ADDRLP4 260
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 635
;635:			if (attackers > 3) attackers = 3;
ADDRLP4 304
INDIRI4
CNSTI4 3
LEI4 $284
ADDRLP4 304
CNSTI4 3
ASGNI4
LABELV $284
line 636
;636:			for (i = 0; i < defenders; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $289
JUMPV
LABELV $286
line 638
;637:				//
;638:				if (teammates[i] == bs->flagcarrier) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $290
line 639
;639:					continue;
ADDRGP4 $287
JUMPV
LABELV $290
line 641
;640:				}
;641:				ClientName(teammates[i], name, sizeof(name));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 642
;642:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 643
;643:				BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 644
;644:				BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 645
;645:			}
LABELV $287
line 636
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $289
ADDRLP4 0
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $286
line 647
;646:			// if we have a flag carrier
;647:			if ( bs->flagcarrier != -1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $292
line 648
;648:				ClientName(bs->flagcarrier, carriername, sizeof(carriername));
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ARGI4
ADDRLP4 312
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 649
;649:				for (i = 0; i < attackers; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $297
JUMPV
LABELV $294
line 651
;650:					//
;651:					if (teammates[numteammates - i - 1] == bs->flagcarrier) {
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $298
line 652
;652:						continue;
ADDRGP4 $295
JUMPV
LABELV $298
line 655
;653:					}
;654:					//
;655:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 656
;656:					if (bs->flagcarrier == bs->client) {
ADDRLP4 356
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 356
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
ADDRLP4 356
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $302
line 657
;657:						BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $161
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 658
;658:						BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWME);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $162
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 659
;659:					}
ADDRGP4 $303
JUMPV
LABELV $302
line 660
;660:					else {
line 661
;661:						BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $163
ARGP4
ADDRLP4 264
ARGP4
ADDRLP4 312
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 662
;662:						BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWFLAGCARRIER);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $164
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 663
;663:					}
LABELV $303
line 664
;664:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 665
;665:				}
LABELV $295
line 649
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $297
ADDRLP4 0
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $294
line 666
;666:			}
ADDRGP4 $267
JUMPV
LABELV $292
line 667
;667:			else {
line 668
;668:				for (i = 0; i < attackers; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $310
JUMPV
LABELV $307
line 670
;669:					//
;670:					if (teammates[numteammates - i - 1] == bs->flagcarrier) {
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6972
ADDP4
INDIRI4
NEI4 $311
line 671
;671:						continue;
ADDRGP4 $308
JUMPV
LABELV $311
line 674
;672:					}
;673:					//
;674:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRLP4 264
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 675
;675:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 264
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 676
;676:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 677
;677:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 4-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 678
;678:				}
LABELV $308
line 668
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $310
ADDRLP4 0
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $307
line 679
;679:			}
line 681
;680:			//
;681:			break;
LABELV $267
line 684
;682:		}
;683:	}
;684:}
LABELV $265
endproc BotCTFOrders_EnemyFlagNotAtBase 360 20
export BotCTFOrders_BothFlagsAtBase
proc BotCTFOrders_BothFlagsAtBase 320 16
line 692
;685:
;686:
;687:/*
;688:==================
;689:BotCTFOrders
;690:==================
;691:*/
;692:void BotCTFOrders_BothFlagsAtBase(bot_state_t *bs) {
line 698
;693:	int numteammates, defenders, attackers, i;
;694:	int teammates[MAX_CLIENTS];
;695:	char name[MAX_NETNAME];
;696:
;697:	//sort team mates by travel time to base
;698:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 308
ADDRGP4 BotSortTeamMatesByBaseTravelTime
CALLI4
ASGNI4
ADDRLP4 296
ADDRLP4 308
INDIRI4
ASGNI4
line 700
;699:	//sort team mates by CTF preference
;700:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 296
INDIRI4
ARGI4
ADDRGP4 BotSortTeamMatesByTaskPreference
CALLI4
pop
line 702
;701:	//passive strategy
;702:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
ADDRFP4 0
INDIRP4
CNSTI4 6976
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $318
line 704
;703:		//different orders based on the number of team mates
;704:		switch(numteammates) {
ADDRLP4 296
INDIRI4
CNSTI4 1
EQI4 $319
ADDRLP4 296
INDIRI4
CNSTI4 2
EQI4 $323
ADDRLP4 296
INDIRI4
CNSTI4 3
EQI4 $327
ADDRGP4 $320
JUMPV
line 705
;705:			case 1: break;
LABELV $323
line 707
;706:			case 2:
;707:			{
line 709
;708:				//the one closest to the base will defend the base
;709:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 710
;710:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 711
;711:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 712
;712:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 714
;713:				//the other will get the flag
;714:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 715
;715:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 716
;716:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 717
;717:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 718
;718:				break;
ADDRGP4 $319
JUMPV
LABELV $327
line 721
;719:			}
;720:			case 3:
;721:			{
line 723
;722:				//the one closest to the base will defend the base
;723:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 724
;724:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 725
;725:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 726
;726:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 728
;727:				//the second one closest to the base will defend the base
;728:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 729
;729:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 730
;730:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 731
;731:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 733
;732:				//the other will get the flag
;733:				ClientName(teammates[2], name, sizeof(name));
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 734
;734:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 735
;735:				BotSayTeamOrder(bs, teammates[2]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 736
;736:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 737
;737:				break;
ADDRGP4 $319
JUMPV
LABELV $320
line 740
;738:			}
;739:			default:
;740:			{
line 741
;741:				defenders = (int) (float) numteammates * 0.5 + 0.5;
ADDRLP4 316
CNSTF4 1056964608
ASGNF4
ADDRLP4 300
ADDRLP4 316
INDIRF4
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
ADDRLP4 316
INDIRF4
ADDF4
CVFI4 4
ASGNI4
line 742
;742:				if (defenders > 5) defenders = 5;
ADDRLP4 300
INDIRI4
CNSTI4 5
LEI4 $334
ADDRLP4 300
CNSTI4 5
ASGNI4
LABELV $334
line 743
;743:				attackers = (int) (float) numteammates * 0.4 + 0.5;
ADDRLP4 304
CNSTF4 1053609165
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 744
;744:				if (attackers > 4) attackers = 4;
ADDRLP4 304
INDIRI4
CNSTI4 4
LEI4 $336
ADDRLP4 304
CNSTI4 4
ASGNI4
LABELV $336
line 745
;745:				for (i = 0; i < defenders; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $341
JUMPV
LABELV $338
line 747
;746:					//
;747:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
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
line 748
;748:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 749
;749:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 750
;750:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 751
;751:				}
LABELV $339
line 745
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $341
ADDRLP4 256
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $338
line 752
;752:				for (i = 0; i < attackers; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $345
JUMPV
LABELV $342
line 754
;753:					//
;754:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
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
line 755
;755:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 756
;756:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 757
;757:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 758
;758:				}
LABELV $343
line 752
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $345
ADDRLP4 256
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $342
line 760
;759:				//
;760:				break;
line 763
;761:			}
;762:		}
;763:	}
ADDRGP4 $319
JUMPV
LABELV $318
line 764
;764:	else {
line 766
;765:		//different orders based on the number of team mates
;766:		switch(numteammates) {
ADDRLP4 296
INDIRI4
CNSTI4 1
EQI4 $350
ADDRLP4 296
INDIRI4
CNSTI4 2
EQI4 $352
ADDRLP4 296
INDIRI4
CNSTI4 3
EQI4 $356
ADDRGP4 $349
JUMPV
line 767
;767:			case 1: break;
LABELV $352
line 769
;768:			case 2:
;769:			{
line 771
;770:				//the one closest to the base will defend the base
;771:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 772
;772:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 773
;773:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 774
;774:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 776
;775:				//the other will get the flag
;776:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 777
;777:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 778
;778:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 779
;779:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 780
;780:				break;
ADDRGP4 $350
JUMPV
LABELV $356
line 783
;781:			}
;782:			case 3:
;783:			{
line 785
;784:				//the one closest to the base will defend the base
;785:				ClientName(teammates[0], name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 786
;786:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 787
;787:				BotSayTeamOrder(bs, teammates[0]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 788
;788:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 790
;789:				//the others should go for the enemy flag
;790:				ClientName(teammates[1], name, sizeof(name));
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 791
;791:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 792
;792:				BotSayTeamOrder(bs, teammates[1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 793
;793:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 795
;794:				//
;795:				ClientName(teammates[2], name, sizeof(name));
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRLP4 260
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 796
;796:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 797
;797:				BotSayTeamOrder(bs, teammates[2]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 798
;798:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 799
;799:				break;
ADDRGP4 $350
JUMPV
LABELV $349
line 802
;800:			}
;801:			default:
;802:			{
line 803
;803:				defenders = (int) (float) numteammates * 0.4 + 0.5;
ADDRLP4 300
CNSTF4 1053609165
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
CNSTF4 1056964608
ADDF4
CVFI4 4
ASGNI4
line 804
;804:				if (defenders > 4) defenders = 4;
ADDRLP4 300
INDIRI4
CNSTI4 4
LEI4 $363
ADDRLP4 300
CNSTI4 4
ASGNI4
LABELV $363
line 805
;805:				attackers = (int) (float) numteammates * 0.5 + 0.5;
ADDRLP4 316
CNSTF4 1056964608
ASGNF4
ADDRLP4 304
ADDRLP4 316
INDIRF4
ADDRLP4 296
INDIRI4
CVIF4 4
CVFI4 4
CVIF4 4
MULF4
ADDRLP4 316
INDIRF4
ADDF4
CVFI4 4
ASGNI4
line 806
;806:				if (attackers > 5) attackers = 5;
ADDRLP4 304
INDIRI4
CNSTI4 5
LEI4 $365
ADDRLP4 304
CNSTI4 5
ASGNI4
LABELV $365
line 807
;807:				for (i = 0; i < defenders; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $370
JUMPV
LABELV $367
line 809
;808:					//
;809:					ClientName(teammates[i], name, sizeof(name));
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
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
line 810
;810:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $209
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 811
;811:					BotSayTeamOrder(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 812
;812:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
ADDP4
INDIRI4
ARGI4
ADDRGP4 $214
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 813
;813:				}
LABELV $368
line 807
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $370
ADDRLP4 256
INDIRI4
ADDRLP4 300
INDIRI4
LTI4 $367
line 814
;814:				for (i = 0; i < attackers; i++) {
ADDRLP4 256
CNSTI4 0
ASGNI4
ADDRGP4 $374
JUMPV
LABELV $371
line 816
;815:					//
;816:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
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
line 817
;817:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $151
ARGP4
ADDRLP4 260
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 818
;818:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrder
CALLV
pop
line 819
;819:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
INDIRI4
ADDRLP4 256
INDIRI4
SUBI4
CNSTI4 2
LSHI4
ADDRLP4 0-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 $152
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 820
;820:				}
LABELV $372
line 814
ADDRLP4 256
ADDRLP4 256
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $374
ADDRLP4 256
INDIRI4
ADDRLP4 304
INDIRI4
LTI4 $371
line 822
;821:				//
;822:				break;
LABELV $350
line 825
;823:			}
;824:		}
;825:	}
LABELV $319
line 826
;826:}
LABELV $317
endproc BotCTFOrders_BothFlagsAtBase 320 16
export BotCTFOrders
proc BotCTFOrders 20 4
line 833
;827:
;828:/*
;829:==================
;830:BotCTFOrders
;831:==================
;832:*/
;833:void BotCTFOrders(bot_state_t *bs) {
line 837
;834:	int flagstatus;
;835:
;836:	//
;837:	if (BotTeam(bs) == TEAM_RED) flagstatus = bs->redflagstatus * 2 + bs->blueflagstatus;
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
NEI4 $379
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
ADDI4
ASGNI4
ADDRGP4 $380
JUMPV
LABELV $379
line 838
;838:	else flagstatus = bs->blueflagstatus * 2 + bs->redflagstatus;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
CNSTI4 6956
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 12
INDIRP4
CNSTI4 6952
ADDP4
INDIRI4
ADDI4
ASGNI4
LABELV $380
line 840
;839:	//
;840:	switch(flagstatus) {
ADDRLP4 16
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $381
ADDRLP4 16
INDIRI4
CNSTI4 3
GTI4 $381
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $387
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $387
address $383
address $384
address $385
address $386
code
LABELV $383
line 841
;841:		case 0: BotCTFOrders_BothFlagsAtBase(bs); break;
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFOrders_BothFlagsAtBase
CALLV
pop
ADDRGP4 $382
JUMPV
LABELV $384
line 842
;842:		case 1: BotCTFOrders_EnemyFlagNotAtBase(bs); break;
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFOrders_EnemyFlagNotAtBase
CALLV
pop
ADDRGP4 $382
JUMPV
LABELV $385
line 843
;843:		case 2: BotCTFOrders_FlagNotAtBase(bs); break;
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFOrders_FlagNotAtBase
CALLV
pop
ADDRGP4 $382
JUMPV
LABELV $386
line 844
;844:		case 3: BotCTFOrders_BothFlagsNotAtBase(bs); break;
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFOrders_BothFlagsNotAtBase
CALLV
pop
LABELV $381
LABELV $382
line 846
;845:	}
;846:}
LABELV $378
endproc BotCTFOrders 20 4
export BotCreateGroup
proc BotCreateGroup 76 20
line 854
;847:
;848:
;849:/*
;850:==================
;851:BotCreateGroup
;852:==================
;853:*/
;854:void BotCreateGroup(bot_state_t *bs, int *teammates, int groupsize) {
line 859
;855:	char name[MAX_NETNAME], leadername[MAX_NETNAME];
;856:	int i;
;857:
;858:	// the others in the group will follow the teammates[0]
;859:	ClientName(teammates[0], leadername, sizeof(leadername));
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRLP4 40
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 860
;860:	for (i = 1; i < groupsize; i++)
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $392
JUMPV
LABELV $389
line 861
;861:	{
line 862
;862:		ClientName(teammates[i], name, sizeof(name));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 863
;863:		if (teammates[0] == bs->client) {
ADDRFP4 4
INDIRP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $393
line 864
;864:			BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $161
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 865
;865:		}
ADDRGP4 $394
JUMPV
LABELV $393
line 866
;866:		else {
line 867
;867:			BotAI_BotInitialChat(bs, "cmd_accompany", name, leadername, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $163
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 40
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 868
;868:		}
LABELV $394
line 869
;869:		BotSayTeamOrderAlways(bs, teammates[i]);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotSayTeamOrderAlways
CALLV
pop
line 870
;870:	}
LABELV $390
line 860
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $392
ADDRLP4 0
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $389
line 871
;871:}
LABELV $388
endproc BotCreateGroup 76 20
bss
align 4
LABELV $396
skip 4
export BotTeamOrders
code
proc BotTeamOrders 1316 12
line 880
;872:
;873:/*
;874:==================
;875:BotTeamOrders
;876:
;877:  FIXME: defend key areas?
;878:==================
;879:*/
;880:void BotTeamOrders(bot_state_t *bs) {
line 886
;881:	int teammates[MAX_CLIENTS];
;882:	int numteammates, i;
;883:	char buf[MAX_INFO_STRING];
;884:	static int maxclients;
;885:
;886:	if (!maxclients)
ADDRGP4 $396
INDIRI4
CNSTI4 0
NEI4 $397
line 887
;887:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $62
ARGP4
ADDRLP4 1288
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $396
ADDRLP4 1288
INDIRI4
ASGNI4
LABELV $397
line 889
;888:
;889:	numteammates = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 890
;890:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $402
JUMPV
LABELV $399
line 891
;891:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 893
;892:		//if no config string or no name
;893:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1292
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1292
INDIRI4
CNSTI4 0
EQI4 $405
ADDRLP4 4
ARGP4
ADDRGP4 $69
ARGP4
ADDRLP4 1296
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1296
INDIRP4
ARGP4
ADDRLP4 1300
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1300
INDIRI4
CNSTI4 0
NEI4 $403
LABELV $405
ADDRGP4 $400
JUMPV
LABELV $403
line 895
;894:		//skip spectators
;895:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $73
ARGP4
ADDRLP4 1304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1304
INDIRP4
ARGP4
ADDRLP4 1308
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1308
INDIRI4
CNSTI4 3
NEI4 $406
ADDRGP4 $400
JUMPV
LABELV $406
line 897
;896:		//
;897:		if (BotSameTeam(bs, i)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1312
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1312
INDIRI4
CNSTI4 0
EQI4 $408
line 898
;898:			teammates[numteammates] = i;
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1032
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 899
;899:			numteammates++;
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 900
;900:		}
LABELV $408
line 901
;901:	}
LABELV $400
line 890
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $402
ADDRLP4 0
INDIRI4
ADDRGP4 $396
INDIRI4
GEI4 $410
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $399
LABELV $410
line 903
;902:	//
;903:	switch(numteammates) {
ADDRLP4 1028
INDIRI4
CNSTI4 1
LTI4 $411
ADDRLP4 1028
INDIRI4
CNSTI4 5
GTI4 $411
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $426-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $426
address $412
address $412
address $415
address $416
address $418
code
line 904
;904:		case 1: break;
line 906
;905:		case 2:
;906:		{
line 908
;907:			//nothing special
;908:			break;
LABELV $415
line 911
;909:		}
;910:		case 3:
;911:		{
line 913
;912:			//have one follow another and one free roaming
;913:			BotCreateGroup(bs, teammates, 2);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 914
;914:			break;
ADDRGP4 $412
JUMPV
LABELV $416
line 917
;915:		}
;916:		case 4:
;917:		{
line 918
;918:			BotCreateGroup(bs, teammates, 2);		//a group of 2
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 919
;919:			BotCreateGroup(bs, &teammates[2], 2);	//a group of 2
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032+8
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 920
;920:			break;
ADDRGP4 $412
JUMPV
LABELV $418
line 923
;921:		}
;922:		case 5:
;923:		{
line 924
;924:			BotCreateGroup(bs, teammates, 2);		//a group of 2
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 925
;925:			BotCreateGroup(bs, &teammates[2], 3);	//a group of 3
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032+8
ARGP4
CNSTI4 3
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 926
;926:			break;
ADDRGP4 $412
JUMPV
LABELV $411
line 929
;927:		}
;928:		default:
;929:		{
line 930
;930:			if (numteammates <= 10) {
ADDRLP4 1028
INDIRI4
CNSTI4 10
GTI4 $412
line 931
;931:				for (i = 0; i < numteammates / 2; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $425
JUMPV
LABELV $422
line 932
;932:					BotCreateGroup(bs, &teammates[i*2], 2);	//groups of 2
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1300
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
LSHI4
ADDRLP4 1300
INDIRI4
LSHI4
ADDRLP4 1032
ADDP4
ARGP4
ADDRLP4 1300
INDIRI4
ARGI4
ADDRGP4 BotCreateGroup
CALLV
pop
line 933
;933:				}
LABELV $423
line 931
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $425
ADDRLP4 0
INDIRI4
ADDRLP4 1028
INDIRI4
CNSTI4 2
DIVI4
LTI4 $422
line 934
;934:			}
line 935
;935:			break;
LABELV $412
line 938
;936:		}
;937:	}
;938:}
LABELV $395
endproc BotTeamOrders 1316 12
export FindHumanTeamLeader
proc FindHumanTeamLeader 12 12
line 1900
;939:
;940:#ifdef MISSIONPACK
;941:
;942:/*
;943:==================
;944:Bot1FCTFOrders_FlagAtCenter
;945:
;946:  X% defend the base, Y% get the flag
;947:==================
;948:*/
;949:void Bot1FCTFOrders_FlagAtCenter(bot_state_t *bs) {
;950:	int numteammates, defenders, attackers, i;
;951:	int teammates[MAX_CLIENTS];
;952:	char name[MAX_NETNAME];
;953:
;954:	//sort team mates by travel time to base
;955:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;956:	//sort team mates by CTF preference
;957:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;958:	//passive strategy
;959:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;960:		//different orders based on the number of team mates
;961:		switch(numteammates) {
;962:			case 1: break;
;963:			case 2:
;964:			{
;965:				//the one closest to the base will defend the base
;966:				ClientName(teammates[0], name, sizeof(name));
;967:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;968:				BotSayTeamOrder(bs, teammates[0]);
;969:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;970:				//the other will get the flag
;971:				ClientName(teammates[1], name, sizeof(name));
;972:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;973:				BotSayTeamOrder(bs, teammates[1]);
;974:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;975:				break;
;976:			}
;977:			case 3:
;978:			{
;979:				//the one closest to the base will defend the base
;980:				ClientName(teammates[0], name, sizeof(name));
;981:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;982:				BotSayTeamOrder(bs, teammates[0]);
;983:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;984:				//the second one closest to the base will defend the base
;985:				ClientName(teammates[1], name, sizeof(name));
;986:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;987:				BotSayTeamOrder(bs, teammates[1]);
;988:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;989:				//the other will get the flag
;990:				ClientName(teammates[2], name, sizeof(name));
;991:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;992:				BotSayTeamOrder(bs, teammates[2]);
;993:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
;994:				break;
;995:			}
;996:			default:
;997:			{
;998:				//50% defend the base
;999:				defenders = (int) (float) numteammates * 0.5 + 0.5;
;1000:				if (defenders > 5) defenders = 5;
;1001:				//40% get the flag
;1002:				attackers = (int) (float) numteammates * 0.4 + 0.5;
;1003:				if (attackers > 4) attackers = 4;
;1004:				for (i = 0; i < defenders; i++) {
;1005:					//
;1006:					ClientName(teammates[i], name, sizeof(name));
;1007:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1008:					BotSayTeamOrder(bs, teammates[i]);
;1009:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1010:				}
;1011:				for (i = 0; i < attackers; i++) {
;1012:					//
;1013:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1014:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1015:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1016:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1017:				}
;1018:				//
;1019:				break;
;1020:			}
;1021:		}
;1022:	}
;1023:	else { //agressive
;1024:		//different orders based on the number of team mates
;1025:		switch(numteammates) {
;1026:			case 1: break;
;1027:			case 2:
;1028:			{
;1029:				//the one closest to the base will defend the base
;1030:				ClientName(teammates[0], name, sizeof(name));
;1031:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1032:				BotSayTeamOrder(bs, teammates[0]);
;1033:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1034:				//the other will get the flag
;1035:				ClientName(teammates[1], name, sizeof(name));
;1036:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1037:				BotSayTeamOrder(bs, teammates[1]);
;1038:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;1039:				break;
;1040:			}
;1041:			case 3:
;1042:			{
;1043:				//the one closest to the base will defend the base
;1044:				ClientName(teammates[0], name, sizeof(name));
;1045:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1046:				BotSayTeamOrder(bs, teammates[0]);
;1047:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1048:				//the others should go for the enemy flag
;1049:				ClientName(teammates[1], name, sizeof(name));
;1050:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1051:				BotSayTeamOrder(bs, teammates[1]);
;1052:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;1053:				//
;1054:				ClientName(teammates[2], name, sizeof(name));
;1055:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1056:				BotSayTeamOrder(bs, teammates[2]);
;1057:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
;1058:				break;
;1059:			}
;1060:			default:
;1061:			{
;1062:				//30% defend the base
;1063:				defenders = (int) (float) numteammates * 0.3 + 0.5;
;1064:				if (defenders > 3) defenders = 3;
;1065:				//60% get the flag
;1066:				attackers = (int) (float) numteammates * 0.6 + 0.5;
;1067:				if (attackers > 6) attackers = 6;
;1068:				for (i = 0; i < defenders; i++) {
;1069:					//
;1070:					ClientName(teammates[i], name, sizeof(name));
;1071:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1072:					BotSayTeamOrder(bs, teammates[i]);
;1073:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1074:				}
;1075:				for (i = 0; i < attackers; i++) {
;1076:					//
;1077:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1078:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1079:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1080:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1081:				}
;1082:				//
;1083:				break;
;1084:			}
;1085:		}
;1086:	}
;1087:}
;1088:
;1089:/*
;1090:==================
;1091:Bot1FCTFOrders_TeamHasFlag
;1092:
;1093:  X% towards neutral flag, Y% go towards enemy base and accompany flag carrier if visible
;1094:==================
;1095:*/
;1096:void Bot1FCTFOrders_TeamHasFlag(bot_state_t *bs) {
;1097:	int numteammates, defenders, attackers, i, other;
;1098:	int teammates[MAX_CLIENTS];
;1099:	char name[MAX_NETNAME], carriername[MAX_NETNAME];
;1100:
;1101:	//sort team mates by travel time to base
;1102:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;1103:	//sort team mates by CTF preference
;1104:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;1105:	//passive strategy
;1106:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;1107:		//different orders based on the number of team mates
;1108:		switch(numteammates) {
;1109:			case 1: break;
;1110:			case 2:
;1111:			{
;1112:				//tell the one not carrying the flag to attack the enemy base
;1113:				if (teammates[0] == bs->flagcarrier) other = teammates[1];
;1114:				else other = teammates[0];
;1115:				ClientName(other, name, sizeof(name));
;1116:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1117:				BotSayTeamOrder(bs, other);
;1118:				BotSayVoiceTeamOrder(bs, other, VOICECHAT_OFFENSE);
;1119:				break;
;1120:			}
;1121:			case 3:
;1122:			{
;1123:				//tell the one closest to the base not carrying the flag to defend the base
;1124:				if (teammates[0] != bs->flagcarrier) other = teammates[0];
;1125:				else other = teammates[1];
;1126:				ClientName(other, name, sizeof(name));
;1127:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1128:				BotSayTeamOrder(bs, other);
;1129:				BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
;1130:				//tell the one furthest from the base not carrying the flag to accompany the flag carrier
;1131:				if (teammates[2] != bs->flagcarrier) other = teammates[2];
;1132:				else other = teammates[1];
;1133:				ClientName(other, name, sizeof(name));
;1134:				if ( bs->flagcarrier != -1 ) {
;1135:					ClientName(bs->flagcarrier, carriername, sizeof(carriername));
;1136:					if (bs->flagcarrier == bs->client) {
;1137:						BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
;1138:						BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWME);
;1139:					}
;1140:					else {
;1141:						BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
;1142:						BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWFLAGCARRIER);
;1143:					}
;1144:				}
;1145:				else {
;1146:					//
;1147:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1148:					BotSayVoiceTeamOrder(bs, other, VOICECHAT_GETFLAG);
;1149:				}
;1150:				BotSayTeamOrder(bs, other);
;1151:				break;
;1152:			}
;1153:			default:
;1154:			{
;1155:				//30% will defend the base
;1156:				defenders = (int) (float) numteammates * 0.3 + 0.5;
;1157:				if (defenders > 3) defenders = 3;
;1158:				//70% accompanies the flag carrier
;1159:				attackers = (int) (float) numteammates * 0.7 + 0.5;
;1160:				if (attackers > 7) attackers = 7;
;1161:				for (i = 0; i < defenders; i++) {
;1162:					//
;1163:					if (teammates[i] == bs->flagcarrier) {
;1164:						continue;
;1165:					}
;1166:					ClientName(teammates[i], name, sizeof(name));
;1167:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1168:					BotSayTeamOrder(bs, teammates[i]);
;1169:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1170:				}
;1171:				if (bs->flagcarrier != -1) {
;1172:					ClientName(bs->flagcarrier, carriername, sizeof(carriername));
;1173:					for (i = 0; i < attackers; i++) {
;1174:						//
;1175:						if (teammates[numteammates - i - 1] == bs->flagcarrier) {
;1176:							continue;
;1177:						}
;1178:						//
;1179:						ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1180:						if (bs->flagcarrier == bs->client) {
;1181:							BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
;1182:							BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWME);
;1183:						}
;1184:						else {
;1185:							BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
;1186:							BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWFLAGCARRIER);
;1187:						}
;1188:						BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1189:					}
;1190:				}
;1191:				else {
;1192:					for (i = 0; i < attackers; i++) {
;1193:						//
;1194:						if (teammates[numteammates - i - 1] == bs->flagcarrier) {
;1195:							continue;
;1196:						}
;1197:						//
;1198:						ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1199:						BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1200:						BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1201:						BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1202:					}
;1203:				}
;1204:				//
;1205:				break;
;1206:			}
;1207:		}
;1208:	}
;1209:	else { //agressive
;1210:		//different orders based on the number of team mates
;1211:		switch(numteammates) {
;1212:			case 1: break;
;1213:			case 2:
;1214:			{
;1215:				//tell the one not carrying the flag to defend the base
;1216:				if (teammates[0] == bs->flagcarrier) other = teammates[1];
;1217:				else other = teammates[0];
;1218:				ClientName(other, name, sizeof(name));
;1219:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1220:				BotSayTeamOrder(bs, other);
;1221:				BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
;1222:				break;
;1223:			}
;1224:			case 3:
;1225:			{
;1226:				//tell the one closest to the base not carrying the flag to defend the base
;1227:				if (teammates[0] != bs->flagcarrier) other = teammates[0];
;1228:				else other = teammates[1];
;1229:				ClientName(other, name, sizeof(name));
;1230:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1231:				BotSayTeamOrder(bs, other);
;1232:				BotSayVoiceTeamOrder(bs, other, VOICECHAT_DEFEND);
;1233:				//tell the one furthest from the base not carrying the flag to accompany the flag carrier
;1234:				if (teammates[2] != bs->flagcarrier) other = teammates[2];
;1235:				else other = teammates[1];
;1236:				ClientName(other, name, sizeof(name));
;1237:				ClientName(bs->flagcarrier, carriername, sizeof(carriername));
;1238:				if (bs->flagcarrier == bs->client) {
;1239:					BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
;1240:					BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWME);
;1241:				}
;1242:				else {
;1243:					BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
;1244:					BotSayVoiceTeamOrder(bs, other, VOICECHAT_FOLLOWFLAGCARRIER);
;1245:				}
;1246:				BotSayTeamOrder(bs, other);
;1247:				break;
;1248:			}
;1249:			default:
;1250:			{
;1251:				//20% will defend the base
;1252:				defenders = (int) (float) numteammates * 0.2 + 0.5;
;1253:				if (defenders > 2) defenders = 2;
;1254:				//80% accompanies the flag carrier
;1255:				attackers = (int) (float) numteammates * 0.8 + 0.5;
;1256:				if (attackers > 8) attackers = 8;
;1257:				for (i = 0; i < defenders; i++) {
;1258:					//
;1259:					if (teammates[i] == bs->flagcarrier) {
;1260:						continue;
;1261:					}
;1262:					ClientName(teammates[i], name, sizeof(name));
;1263:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1264:					BotSayTeamOrder(bs, teammates[i]);
;1265:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1266:				}
;1267:				ClientName(bs->flagcarrier, carriername, sizeof(carriername));
;1268:				for (i = 0; i < attackers; i++) {
;1269:					//
;1270:					if (teammates[numteammates - i - 1] == bs->flagcarrier) {
;1271:						continue;
;1272:					}
;1273:					//
;1274:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1275:					if (bs->flagcarrier == bs->client) {
;1276:						BotAI_BotInitialChat(bs, "cmd_accompanyme", name, NULL);
;1277:						BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWME);
;1278:					}
;1279:					else {
;1280:						BotAI_BotInitialChat(bs, "cmd_accompany", name, carriername, NULL);
;1281:						BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_FOLLOWFLAGCARRIER);
;1282:					}
;1283:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1284:				}
;1285:				//
;1286:				break;
;1287:			}
;1288:		}
;1289:	}
;1290:}
;1291:
;1292:/*
;1293:==================
;1294:Bot1FCTFOrders_EnemyHasFlag
;1295:
;1296:  X% defend the base, Y% towards neutral flag
;1297:==================
;1298:*/
;1299:void Bot1FCTFOrders_EnemyHasFlag(bot_state_t *bs) {
;1300:	int numteammates, defenders, attackers, i;
;1301:	int teammates[MAX_CLIENTS];
;1302:	char name[MAX_NETNAME];
;1303:
;1304:	//sort team mates by travel time to base
;1305:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;1306:	//sort team mates by CTF preference
;1307:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;1308:	//passive strategy
;1309:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;1310:		//different orders based on the number of team mates
;1311:		switch(numteammates) {
;1312:			case 1: break;
;1313:			case 2:
;1314:			{
;1315:				//both defend the base
;1316:				ClientName(teammates[0], name, sizeof(name));
;1317:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1318:				BotSayTeamOrder(bs, teammates[0]);
;1319:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1320:				//
;1321:				ClientName(teammates[1], name, sizeof(name));
;1322:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1323:				BotSayTeamOrder(bs, teammates[1]);
;1324:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1325:				break;
;1326:			}
;1327:			case 3:
;1328:			{
;1329:				//the one closest to the base will defend the base
;1330:				ClientName(teammates[0], name, sizeof(name));
;1331:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1332:				BotSayTeamOrder(bs, teammates[0]);
;1333:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1334:				//the second one closest to the base will defend the base
;1335:				ClientName(teammates[1], name, sizeof(name));
;1336:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1337:				BotSayTeamOrder(bs, teammates[1]);
;1338:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1339:				//the other will also defend the base
;1340:				ClientName(teammates[2], name, sizeof(name));
;1341:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1342:				BotSayTeamOrder(bs, teammates[2]);
;1343:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_DEFEND);
;1344:				break;
;1345:			}
;1346:			default:
;1347:			{
;1348:				//80% will defend the base
;1349:				defenders = (int) (float) numteammates * 0.8 + 0.5;
;1350:				if (defenders > 8) defenders = 8;
;1351:				//10% will try to return the flag
;1352:				attackers = (int) (float) numteammates * 0.1 + 0.5;
;1353:				if (attackers > 2) attackers = 2;
;1354:				for (i = 0; i < defenders; i++) {
;1355:					//
;1356:					ClientName(teammates[i], name, sizeof(name));
;1357:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1358:					BotSayTeamOrder(bs, teammates[i]);
;1359:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1360:				}
;1361:				for (i = 0; i < attackers; i++) {
;1362:					//
;1363:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1364:					BotAI_BotInitialChat(bs, "cmd_returnflag", name, NULL);
;1365:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1366:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1367:				}
;1368:				//
;1369:				break;
;1370:			}
;1371:		}
;1372:	}
;1373:	else { //agressive
;1374:		//different orders based on the number of team mates
;1375:		switch(numteammates) {
;1376:			case 1: break;
;1377:			case 2:
;1378:			{
;1379:				//the one closest to the base will defend the base
;1380:				ClientName(teammates[0], name, sizeof(name));
;1381:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1382:				BotSayTeamOrder(bs, teammates[0]);
;1383:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1384:				//the other will get the flag
;1385:				ClientName(teammates[1], name, sizeof(name));
;1386:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1387:				BotSayTeamOrder(bs, teammates[1]);
;1388:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1389:				break;
;1390:			}
;1391:			case 3:
;1392:			{
;1393:				//the one closest to the base will defend the base
;1394:				ClientName(teammates[0], name, sizeof(name));
;1395:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1396:				BotSayTeamOrder(bs, teammates[0]);
;1397:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1398:				//the others should go for the enemy flag
;1399:				ClientName(teammates[1], name, sizeof(name));
;1400:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1401:				BotSayTeamOrder(bs, teammates[1]);
;1402:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1403:				//
;1404:				ClientName(teammates[2], name, sizeof(name));
;1405:				BotAI_BotInitialChat(bs, "cmd_returnflag", name, NULL);
;1406:				BotSayTeamOrder(bs, teammates[2]);
;1407:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
;1408:				break;
;1409:			}
;1410:			default:
;1411:			{
;1412:				//70% defend the base
;1413:				defenders = (int) (float) numteammates * 0.7 + 0.5;
;1414:				if (defenders > 8) defenders = 8;
;1415:				//20% try to return the flag
;1416:				attackers = (int) (float) numteammates * 0.2 + 0.5;
;1417:				if (attackers > 2) attackers = 2;
;1418:				for (i = 0; i < defenders; i++) {
;1419:					//
;1420:					ClientName(teammates[i], name, sizeof(name));
;1421:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1422:					BotSayTeamOrder(bs, teammates[i]);
;1423:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1424:				}
;1425:				for (i = 0; i < attackers; i++) {
;1426:					//
;1427:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1428:					BotAI_BotInitialChat(bs, "cmd_returnflag", name, NULL);
;1429:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1430:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1431:				}
;1432:				//
;1433:				break;
;1434:			}
;1435:		}
;1436:	}
;1437:}
;1438:
;1439:/*
;1440:==================
;1441:Bot1FCTFOrders_EnemyDroppedFlag
;1442:
;1443:  X% defend the base, Y% get the flag
;1444:==================
;1445:*/
;1446:void Bot1FCTFOrders_EnemyDroppedFlag(bot_state_t *bs) {
;1447:	int numteammates, defenders, attackers, i;
;1448:	int teammates[MAX_CLIENTS];
;1449:	char name[MAX_NETNAME];
;1450:
;1451:	//sort team mates by travel time to base
;1452:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;1453:	//sort team mates by CTF preference
;1454:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;1455:	//passive strategy
;1456:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;1457:		//different orders based on the number of team mates
;1458:		switch(numteammates) {
;1459:			case 1: break;
;1460:			case 2:
;1461:			{
;1462:				//the one closest to the base will defend the base
;1463:				ClientName(teammates[0], name, sizeof(name));
;1464:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1465:				BotSayTeamOrder(bs, teammates[0]);
;1466:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1467:				//the other will get the flag
;1468:				ClientName(teammates[1], name, sizeof(name));
;1469:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1470:				BotSayTeamOrder(bs, teammates[1]);
;1471:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;1472:				break;
;1473:			}
;1474:			case 3:
;1475:			{
;1476:				//the one closest to the base will defend the base
;1477:				ClientName(teammates[0], name, sizeof(name));
;1478:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1479:				BotSayTeamOrder(bs, teammates[0]);
;1480:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1481:				//the second one closest to the base will defend the base
;1482:				ClientName(teammates[1], name, sizeof(name));
;1483:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1484:				BotSayTeamOrder(bs, teammates[1]);
;1485:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1486:				//the other will get the flag
;1487:				ClientName(teammates[2], name, sizeof(name));
;1488:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1489:				BotSayTeamOrder(bs, teammates[2]);
;1490:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
;1491:				break;
;1492:			}
;1493:			default:
;1494:			{
;1495:				//50% defend the base
;1496:				defenders = (int) (float) numteammates * 0.5 + 0.5;
;1497:				if (defenders > 5) defenders = 5;
;1498:				//40% get the flag
;1499:				attackers = (int) (float) numteammates * 0.4 + 0.5;
;1500:				if (attackers > 4) attackers = 4;
;1501:				for (i = 0; i < defenders; i++) {
;1502:					//
;1503:					ClientName(teammates[i], name, sizeof(name));
;1504:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1505:					BotSayTeamOrder(bs, teammates[i]);
;1506:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1507:				}
;1508:				for (i = 0; i < attackers; i++) {
;1509:					//
;1510:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1511:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1512:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1513:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_GETFLAG);
;1514:				}
;1515:				//
;1516:				break;
;1517:			}
;1518:		}
;1519:	}
;1520:	else { //agressive
;1521:		//different orders based on the number of team mates
;1522:		switch(numteammates) {
;1523:			case 1: break;
;1524:			case 2:
;1525:			{
;1526:				//the one closest to the base will defend the base
;1527:				ClientName(teammates[0], name, sizeof(name));
;1528:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1529:				BotSayTeamOrder(bs, teammates[0]);
;1530:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1531:				//the other will get the flag
;1532:				ClientName(teammates[1], name, sizeof(name));
;1533:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1534:				BotSayTeamOrder(bs, teammates[1]);
;1535:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;1536:				break;
;1537:			}
;1538:			case 3:
;1539:			{
;1540:				//the one closest to the base will defend the base
;1541:				ClientName(teammates[0], name, sizeof(name));
;1542:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1543:				BotSayTeamOrder(bs, teammates[0]);
;1544:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1545:				//the others should go for the enemy flag
;1546:				ClientName(teammates[1], name, sizeof(name));
;1547:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1548:				BotSayTeamOrder(bs, teammates[1]);
;1549:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_GETFLAG);
;1550:				//
;1551:				ClientName(teammates[2], name, sizeof(name));
;1552:				BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1553:				BotSayTeamOrder(bs, teammates[2]);
;1554:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_GETFLAG);
;1555:				break;
;1556:			}
;1557:			default:
;1558:			{
;1559:				//30% defend the base
;1560:				defenders = (int) (float) numteammates * 0.3 + 0.5;
;1561:				if (defenders > 3) defenders = 3;
;1562:				//60% get the flag
;1563:				attackers = (int) (float) numteammates * 0.6 + 0.5;
;1564:				if (attackers > 6) attackers = 6;
;1565:				for (i = 0; i < defenders; i++) {
;1566:					//
;1567:					ClientName(teammates[i], name, sizeof(name));
;1568:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1569:					BotSayTeamOrder(bs, teammates[i]);
;1570:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1571:				}
;1572:				for (i = 0; i < attackers; i++) {
;1573:					//
;1574:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1575:					BotAI_BotInitialChat(bs, "cmd_getflag", name, NULL);
;1576:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1577:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_DEFEND);
;1578:				}
;1579:				//
;1580:				break;
;1581:			}
;1582:		}
;1583:	}
;1584:}
;1585:
;1586:/*
;1587:==================
;1588:Bot1FCTFOrders
;1589:==================
;1590:*/
;1591:void Bot1FCTFOrders(bot_state_t *bs) {
;1592:	switch(bs->neutralflagstatus) {
;1593:		case 0: Bot1FCTFOrders_FlagAtCenter(bs); break;
;1594:		case 1: Bot1FCTFOrders_TeamHasFlag(bs); break;
;1595:		case 2: Bot1FCTFOrders_EnemyHasFlag(bs); break;
;1596:		case 3: Bot1FCTFOrders_EnemyDroppedFlag(bs); break;
;1597:	}
;1598:}
;1599:
;1600:/*
;1601:==================
;1602:BotObeliskOrders
;1603:
;1604:  X% in defence Y% in offence
;1605:==================
;1606:*/
;1607:void BotObeliskOrders(bot_state_t *bs) {
;1608:	int numteammates, defenders, attackers, i;
;1609:	int teammates[MAX_CLIENTS];
;1610:	char name[MAX_NETNAME];
;1611:
;1612:	//sort team mates by travel time to base
;1613:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;1614:	//sort team mates by CTF preference
;1615:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;1616:	//passive strategy
;1617:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;1618:		//different orders based on the number of team mates
;1619:		switch(numteammates) {
;1620:			case 1: break;
;1621:			case 2:
;1622:			{
;1623:				//the one closest to the base will defend the base
;1624:				ClientName(teammates[0], name, sizeof(name));
;1625:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1626:				BotSayTeamOrder(bs, teammates[0]);
;1627:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1628:				//the other will attack the enemy base
;1629:				ClientName(teammates[1], name, sizeof(name));
;1630:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1631:				BotSayTeamOrder(bs, teammates[1]);
;1632:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1633:				break;
;1634:			}
;1635:			case 3:
;1636:			{
;1637:				//the one closest to the base will defend the base
;1638:				ClientName(teammates[0], name, sizeof(name));
;1639:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1640:				BotSayTeamOrder(bs, teammates[0]);
;1641:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1642:				//the one second closest to the base also defends the base
;1643:				ClientName(teammates[1], name, sizeof(name));
;1644:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1645:				BotSayTeamOrder(bs, teammates[1]);
;1646:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1647:				//the other one attacks the enemy base
;1648:				ClientName(teammates[2], name, sizeof(name));
;1649:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1650:				BotSayTeamOrder(bs, teammates[2]);
;1651:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_OFFENSE);
;1652:				break;
;1653:			}
;1654:			default:
;1655:			{
;1656:				//50% defend the base
;1657:				defenders = (int) (float) numteammates * 0.5 + 0.5;
;1658:				if (defenders > 5) defenders = 5;
;1659:				//40% attack the enemy base
;1660:				attackers = (int) (float) numteammates * 0.4 + 0.5;
;1661:				if (attackers > 4) attackers = 4;
;1662:				for (i = 0; i < defenders; i++) {
;1663:					//
;1664:					ClientName(teammates[i], name, sizeof(name));
;1665:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1666:					BotSayTeamOrder(bs, teammates[i]);
;1667:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1668:				}
;1669:				for (i = 0; i < attackers; i++) {
;1670:					//
;1671:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1672:					BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1673:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1674:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_OFFENSE);
;1675:				}
;1676:				//
;1677:				break;
;1678:			}
;1679:		}
;1680:	}
;1681:	else {
;1682:		//different orders based on the number of team mates
;1683:		switch(numteammates) {
;1684:			case 1: break;
;1685:			case 2:
;1686:			{
;1687:				//the one closest to the base will defend the base
;1688:				ClientName(teammates[0], name, sizeof(name));
;1689:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1690:				BotSayTeamOrder(bs, teammates[0]);
;1691:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1692:				//the other will attack the enemy base
;1693:				ClientName(teammates[1], name, sizeof(name));
;1694:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1695:				BotSayTeamOrder(bs, teammates[1]);
;1696:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1697:				break;
;1698:			}
;1699:			case 3:
;1700:			{
;1701:				//the one closest to the base will defend the base
;1702:				ClientName(teammates[0], name, sizeof(name));
;1703:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1704:				BotSayTeamOrder(bs, teammates[0]);
;1705:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1706:				//the others attack the enemy base
;1707:				ClientName(teammates[1], name, sizeof(name));
;1708:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1709:				BotSayTeamOrder(bs, teammates[1]);
;1710:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1711:				//
;1712:				ClientName(teammates[2], name, sizeof(name));
;1713:				BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1714:				BotSayTeamOrder(bs, teammates[2]);
;1715:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_OFFENSE);
;1716:				break;
;1717:			}
;1718:			default:
;1719:			{
;1720:				//30% defend the base
;1721:				defenders = (int) (float) numteammates * 0.3 + 0.5;
;1722:				if (defenders > 3) defenders = 3;
;1723:				//70% attack the enemy base
;1724:				attackers = (int) (float) numteammates * 0.7 + 0.5;
;1725:				if (attackers > 7) attackers = 7;
;1726:				for (i = 0; i < defenders; i++) {
;1727:					//
;1728:					ClientName(teammates[i], name, sizeof(name));
;1729:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1730:					BotSayTeamOrder(bs, teammates[i]);
;1731:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1732:				}
;1733:				for (i = 0; i < attackers; i++) {
;1734:					//
;1735:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1736:					BotAI_BotInitialChat(bs, "cmd_attackenemybase", name, NULL);
;1737:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1738:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_OFFENSE);
;1739:				}
;1740:				//
;1741:				break;
;1742:			}
;1743:		}
;1744:	}
;1745:}
;1746:
;1747:/*
;1748:==================
;1749:BotHarvesterOrders
;1750:
;1751:  X% defend the base, Y% harvest
;1752:==================
;1753:*/
;1754:void BotHarvesterOrders(bot_state_t *bs) {
;1755:	int numteammates, defenders, attackers, i;
;1756:	int teammates[MAX_CLIENTS];
;1757:	char name[MAX_NETNAME];
;1758:
;1759:	//sort team mates by travel time to base
;1760:	numteammates = BotSortTeamMatesByBaseTravelTime(bs, teammates, sizeof(teammates));
;1761:	//sort team mates by CTF preference
;1762:	BotSortTeamMatesByTaskPreference(bs, teammates, numteammates);
;1763:	//passive strategy
;1764:	if (!(bs->ctfstrategy & CTFS_AGRESSIVE)) {
;1765:		//different orders based on the number of team mates
;1766:		switch(numteammates) {
;1767:			case 1: break;
;1768:			case 2:
;1769:			{
;1770:				//the one closest to the base will defend the base
;1771:				ClientName(teammates[0], name, sizeof(name));
;1772:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1773:				BotSayTeamOrder(bs, teammates[0]);
;1774:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1775:				//the other will harvest
;1776:				ClientName(teammates[1], name, sizeof(name));
;1777:				BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1778:				BotSayTeamOrder(bs, teammates[1]);
;1779:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1780:				break;
;1781:			}
;1782:			case 3:
;1783:			{
;1784:				//the one closest to the base will defend the base
;1785:				ClientName(teammates[0], name, sizeof(name));
;1786:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1787:				BotSayTeamOrder(bs, teammates[0]);
;1788:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1789:				//the one second closest to the base also defends the base
;1790:				ClientName(teammates[1], name, sizeof(name));
;1791:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1792:				BotSayTeamOrder(bs, teammates[1]);
;1793:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_DEFEND);
;1794:				//the other one goes harvesting
;1795:				ClientName(teammates[2], name, sizeof(name));
;1796:				BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1797:				BotSayTeamOrder(bs, teammates[2]);
;1798:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_OFFENSE);
;1799:				break;
;1800:			}
;1801:			default:
;1802:			{
;1803:				//50% defend the base
;1804:				defenders = (int) (float) numteammates * 0.5 + 0.5;
;1805:				if (defenders > 5) defenders = 5;
;1806:				//40% goes harvesting
;1807:				attackers = (int) (float) numteammates * 0.4 + 0.5;
;1808:				if (attackers > 4) attackers = 4;
;1809:				for (i = 0; i < defenders; i++) {
;1810:					//
;1811:					ClientName(teammates[i], name, sizeof(name));
;1812:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1813:					BotSayTeamOrder(bs, teammates[i]);
;1814:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1815:				}
;1816:				for (i = 0; i < attackers; i++) {
;1817:					//
;1818:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1819:					BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1820:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1821:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_OFFENSE);
;1822:				}
;1823:				//
;1824:				break;
;1825:			}
;1826:		}
;1827:	}
;1828:	else {
;1829:		//different orders based on the number of team mates
;1830:		switch(numteammates) {
;1831:			case 1: break;
;1832:			case 2:
;1833:			{
;1834:				//the one closest to the base will defend the base
;1835:				ClientName(teammates[0], name, sizeof(name));
;1836:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1837:				BotSayTeamOrder(bs, teammates[0]);
;1838:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1839:				//the other will harvest
;1840:				ClientName(teammates[1], name, sizeof(name));
;1841:				BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1842:				BotSayTeamOrder(bs, teammates[1]);
;1843:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1844:				break;
;1845:			}
;1846:			case 3:
;1847:			{
;1848:				//the one closest to the base will defend the base
;1849:				ClientName(teammates[0], name, sizeof(name));
;1850:				BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1851:				BotSayTeamOrder(bs, teammates[0]);
;1852:				BotSayVoiceTeamOrder(bs, teammates[0], VOICECHAT_DEFEND);
;1853:				//the others go harvesting
;1854:				ClientName(teammates[1], name, sizeof(name));
;1855:				BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1856:				BotSayTeamOrder(bs, teammates[1]);
;1857:				BotSayVoiceTeamOrder(bs, teammates[1], VOICECHAT_OFFENSE);
;1858:				//
;1859:				ClientName(teammates[2], name, sizeof(name));
;1860:				BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1861:				BotSayTeamOrder(bs, teammates[2]);
;1862:				BotSayVoiceTeamOrder(bs, teammates[2], VOICECHAT_OFFENSE);
;1863:				break;
;1864:			}
;1865:			default:
;1866:			{
;1867:				//30% defend the base
;1868:				defenders = (int) (float) numteammates * 0.3 + 0.5;
;1869:				if (defenders > 3) defenders = 3;
;1870:				//70% go harvesting
;1871:				attackers = (int) (float) numteammates * 0.7 + 0.5;
;1872:				if (attackers > 7) attackers = 7;
;1873:				for (i = 0; i < defenders; i++) {
;1874:					//
;1875:					ClientName(teammates[i], name, sizeof(name));
;1876:					BotAI_BotInitialChat(bs, "cmd_defendbase", name, NULL);
;1877:					BotSayTeamOrder(bs, teammates[i]);
;1878:					BotSayVoiceTeamOrder(bs, teammates[i], VOICECHAT_DEFEND);
;1879:				}
;1880:				for (i = 0; i < attackers; i++) {
;1881:					//
;1882:					ClientName(teammates[numteammates - i - 1], name, sizeof(name));
;1883:					BotAI_BotInitialChat(bs, "cmd_harvest", name, NULL);
;1884:					BotSayTeamOrder(bs, teammates[numteammates - i - 1]);
;1885:					BotSayVoiceTeamOrder(bs, teammates[numteammates - i - 1], VOICECHAT_OFFENSE);
;1886:				}
;1887:				//
;1888:				break;
;1889:			}
;1890:		}
;1891:	}
;1892:}
;1893:#endif
;1894:
;1895:/*
;1896:==================
;1897:FindHumanTeamLeader
;1898:==================
;1899:*/
;1900:int FindHumanTeamLeader(bot_state_t *bs) {
line 1903
;1901:	int i;
;1902:
;1903:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $429
line 1904
;1904:		if ( g_entities[i].inuse ) {
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+520
ADDP4
INDIRI4
CNSTI4 0
EQI4 $433
line 1906
;1905:			// if this player is not a bot
;1906:			if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
CNSTI4 808
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $436
line 1908
;1907:				// if this player is ok with being the leader
;1908:				if (!notleader[i]) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 notleader
ADDP4
INDIRI4
CNSTI4 0
NEI4 $440
line 1910
;1909:					// if this player is on the same team
;1910:					if ( BotSameTeam(bs, i) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $442
line 1911
;1911:						ClientName(i, bs->teamleader, sizeof(bs->teamleader));
ADDRLP4 0
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
line 1913
;1912:						// if not yet ordered to do anything
;1913:						if ( !BotSetLastOrderedTask(bs) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 BotSetLastOrderedTask
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $444
line 1915
;1914:							// go on defense by default
;1915:							BotVoiceChat_Defend(bs, i, SAY_TELL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 BotVoiceChat_Defend
CALLV
pop
line 1916
;1916:						}
LABELV $444
line 1917
;1917:						return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $428
JUMPV
LABELV $442
line 1919
;1918:					}
;1919:				}
LABELV $440
line 1920
;1920:			}
LABELV $436
line 1921
;1921:		}
LABELV $433
line 1922
;1922:	}
LABELV $430
line 1903
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $429
line 1923
;1923:	return qfalse;
CNSTI4 0
RETI4
LABELV $428
endproc FindHumanTeamLeader 12 12
export BotTeamAI
proc BotTeamAI 72 12
line 1931
;1924:}
;1925:
;1926:/*
;1927:==================
;1928:BotTeamAI
;1929:==================
;1930:*/
;1931:void BotTeamAI(bot_state_t *bs) {
line 1936
;1932:	int numteammates;
;1933:	char netname[MAX_NETNAME];
;1934:
;1935:	//
;1936:	if ( gametype < GT_TEAM  )
ADDRGP4 gametype
INDIRI4
CNSTI4 3
GEI4 $447
line 1937
;1937:		return;
ADDRGP4 $446
JUMPV
LABELV $447
line 1939
;1938:	// make sure we've got a valid team leader
;1939:	if (!BotValidTeamLeader(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 BotValidTeamLeader
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
NEI4 $449
line 1941
;1940:		//
;1941:		if (!FindHumanTeamLeader(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 FindHumanTeamLeader
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $451
line 1943
;1942:			//
;1943:			if (!bs->askteamleader_time && !bs->becometeamleader_time) {
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
CNSTF4 0
ASGNF4
ADDRLP4 48
INDIRP4
CNSTI4 6932
ADDP4
INDIRF4
ADDRLP4 52
INDIRF4
NEF4 $453
ADDRLP4 48
INDIRP4
CNSTI4 6936
ADDP4
INDIRF4
ADDRLP4 52
INDIRF4
NEF4 $453
line 1944
;1944:				if (bs->entergame_time + 10 > FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6064
ADDP4
INDIRF4
CNSTF4 1092616192
ADDF4
ADDRGP4 floattime
INDIRF4
LEF4 $455
line 1945
;1945:					bs->askteamleader_time = FloatTime() + 5 + random() * 10;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6932
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CNSTF4 1092616192
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1946
;1946:				}
ADDRGP4 $456
JUMPV
LABELV $455
line 1947
;1947:				else {
line 1948
;1948:					bs->becometeamleader_time = FloatTime() + 5 + random() * 10;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6936
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CNSTF4 1092616192
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1949
;1949:				}
LABELV $456
line 1950
;1950:			}
LABELV $453
line 1951
;1951:			if (bs->askteamleader_time && bs->askteamleader_time < FloatTime()) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 6932
ADDP4
INDIRF4
ASGNF4
ADDRLP4 56
INDIRF4
CNSTF4 0
EQF4 $457
ADDRLP4 56
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $457
line 1953
;1952:				// if asked for a team leader and no response
;1953:				BotAI_BotInitialChat(bs, "whoisteamleader", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $459
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1954
;1954:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 1955
;1955:				bs->askteamleader_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6932
ADDP4
CNSTF4 0
ASGNF4
line 1956
;1956:				bs->becometeamleader_time = FloatTime() + 8 + random() * 10;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6936
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1090519040
ADDF4
CNSTF4 1092616192
ADDRLP4 60
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 1957
;1957:			}
LABELV $457
line 1958
;1958:			if (bs->becometeamleader_time && bs->becometeamleader_time < FloatTime()) {
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 6936
ADDP4
INDIRF4
ASGNF4
ADDRLP4 60
INDIRF4
CNSTF4 0
EQF4 $446
ADDRLP4 60
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $446
line 1959
;1959:				BotAI_BotInitialChat(bs, "iamteamleader", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $462
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1960
;1960:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 1961
;1961:				BotSayVoiceTeamOrder(bs, -1, VOICECHAT_STARTLEADER);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $463
ARGP4
ADDRGP4 BotSayVoiceTeamOrder
CALLV
pop
line 1962
;1962:				ClientName(bs->client, netname, sizeof(netname));
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
line 1963
;1963:				strncpy(bs->teamleader, netname, sizeof(bs->teamleader));
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
line 1964
;1964:				bs->teamleader[sizeof(bs->teamleader)] = '\0';
ADDRFP4 0
INDIRP4
CNSTI4 6900
CNSTU4 32
ADDI4
ADDP4
CNSTI1 0
ASGNI1
line 1965
;1965:				bs->becometeamleader_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6936
ADDP4
CNSTF4 0
ASGNF4
line 1966
;1966:			}
line 1967
;1967:			return;
ADDRGP4 $446
JUMPV
LABELV $451
line 1969
;1968:		}
;1969:	}
LABELV $449
line 1970
;1970:	bs->askteamleader_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6932
ADDP4
CNSTF4 0
ASGNF4
line 1971
;1971:	bs->becometeamleader_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6936
ADDP4
CNSTF4 0
ASGNF4
line 1974
;1972:
;1973:	//return if this bot is NOT the team leader
;1974:	ClientName(bs->client, netname, sizeof(netname));
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
line 1975
;1975:	if (Q_stricmp(netname, bs->teamleader) != 0) return;
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6900
ADDP4
ARGP4
ADDRLP4 44
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $464
ADDRGP4 $446
JUMPV
LABELV $464
line 1977
;1976:	//
;1977:	numteammates = BotNumTeamMates(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 BotNumTeamMates
CALLI4
ASGNI4
ADDRLP4 36
ADDRLP4 48
INDIRI4
ASGNI4
line 1979
;1978:	//give orders
;1979:	switch(gametype) {
ADDRLP4 52
ADDRGP4 gametype
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 3
EQI4 $468
ADDRLP4 52
INDIRI4
CNSTI4 4
EQI4 $474
ADDRGP4 $466
JUMPV
LABELV $468
line 1981
;1980:		case GT_TEAM:
;1981:		{
line 1982
;1982:			if (bs->numteammates != numteammates || bs->forceorders) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 6948
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
NEI4 $471
ADDRLP4 56
INDIRP4
CNSTI4 6968
ADDP4
INDIRI4
CNSTI4 0
EQI4 $469
LABELV $471
line 1983
;1983:				bs->teamgiveorders_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1984
;1984:				bs->numteammates = numteammates;
ADDRFP4 0
INDIRP4
CNSTI4 6948
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 1985
;1985:				bs->forceorders = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6968
ADDP4
CNSTI4 0
ASGNI4
line 1986
;1986:			}
LABELV $469
line 1988
;1987:			//if it's time to give orders
;1988:			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 5) {
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
INDIRF4
ASGNF4
ADDRLP4 60
INDIRF4
CNSTF4 0
EQF4 $467
ADDRLP4 60
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
SUBF4
GEF4 $467
line 1989
;1989:				BotTeamOrders(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotTeamOrders
CALLV
pop
line 1991
;1990:				//give orders again after 120 seconds
;1991:				bs->teamgiveorders_time = FloatTime() + 120;
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1123024896
ADDF4
ASGNF4
line 1992
;1992:			}
line 1993
;1993:			break;
ADDRGP4 $467
JUMPV
LABELV $474
line 1996
;1994:		}
;1995:		case GT_CTF:
;1996:		{
line 1999
;1997:			//if the number of team mates changed or the flag status changed
;1998:			//or someone wants to know what to do
;1999:			if (bs->numteammates != numteammates || bs->flagstatuschanged || bs->forceorders) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 6948
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
NEI4 $478
ADDRLP4 60
CNSTI4 0
ASGNI4
ADDRLP4 56
INDIRP4
CNSTI4 6964
ADDP4
INDIRI4
ADDRLP4 60
INDIRI4
NEI4 $478
ADDRLP4 56
INDIRP4
CNSTI4 6968
ADDP4
INDIRI4
ADDRLP4 60
INDIRI4
EQI4 $475
LABELV $478
line 2000
;2000:				bs->teamgiveorders_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2001
;2001:				bs->numteammates = numteammates;
ADDRFP4 0
INDIRP4
CNSTI4 6948
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 2002
;2002:				bs->flagstatuschanged = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6964
ADDP4
CNSTI4 0
ASGNI4
line 2003
;2003:				bs->forceorders = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6968
ADDP4
CNSTI4 0
ASGNI4
line 2004
;2004:			}
LABELV $475
line 2006
;2005:			//if there were no flag captures the last 3 minutes
;2006:			if (bs->lastflagcapture_time < FloatTime() - 240) {
ADDRFP4 0
INDIRP4
CNSTI4 6944
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1131413504
SUBF4
GEF4 $479
line 2007
;2007:				bs->lastflagcapture_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6944
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2009
;2008:				//randomly change the CTF strategy
;2009:				if (random() < 0.4) {
ADDRLP4 64
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1053609165
GEF4 $481
line 2010
;2010:					bs->ctfstrategy ^= CTFS_AGRESSIVE;
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 6976
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 2011
;2011:					bs->teamgiveorders_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2012
;2012:				}
LABELV $481
line 2013
;2013:			}
LABELV $479
line 2015
;2014:			//if it's time to give orders
;2015:			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 3) {
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
INDIRF4
ASGNF4
ADDRLP4 64
INDIRF4
CNSTF4 0
EQF4 $467
ADDRLP4 64
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1077936128
SUBF4
GEF4 $467
line 2016
;2016:				BotCTFOrders(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCTFOrders
CALLV
pop
line 2018
;2017:				//
;2018:				bs->teamgiveorders_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6940
ADDP4
CNSTF4 0
ASGNF4
line 2019
;2019:			}
line 2020
;2020:			break;
LABELV $466
LABELV $467
line 2080
;2021:		}
;2022:#ifdef MISSIONPACK
;2023:		case GT_1FCTF:
;2024:		{
;2025:			if (bs->numteammates != numteammates || bs->flagstatuschanged || bs->forceorders) {
;2026:				bs->teamgiveorders_time = FloatTime();
;2027:				bs->numteammates = numteammates;
;2028:				bs->flagstatuschanged = qfalse;
;2029:				bs->forceorders = qfalse;
;2030:			}
;2031:			//if there were no flag captures the last 4 minutes
;2032:			if (bs->lastflagcapture_time < FloatTime() - 240) {
;2033:				bs->lastflagcapture_time = FloatTime();
;2034:				//randomly change the CTF strategy
;2035:				if (random() < 0.4) {
;2036:					bs->ctfstrategy ^= CTFS_AGRESSIVE;
;2037:					bs->teamgiveorders_time = FloatTime();
;2038:				}
;2039:			}
;2040:			//if it's time to give orders
;2041:			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 2) {
;2042:				Bot1FCTFOrders(bs);
;2043:				//
;2044:				bs->teamgiveorders_time = 0;
;2045:			}
;2046:			break;
;2047:		}
;2048:		case GT_OBELISK:
;2049:		{
;2050:			if (bs->numteammates != numteammates || bs->forceorders) {
;2051:				bs->teamgiveorders_time = FloatTime();
;2052:				bs->numteammates = numteammates;
;2053:				bs->forceorders = qfalse;
;2054:			}
;2055:			//if it's time to give orders
;2056:			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 5) {
;2057:				BotObeliskOrders(bs);
;2058:				//give orders again after 30 seconds
;2059:				bs->teamgiveorders_time = FloatTime() + 30;
;2060:			}
;2061:			break;
;2062:		}
;2063:		case GT_HARVESTER:
;2064:		{
;2065:			if (bs->numteammates != numteammates || bs->forceorders) {
;2066:				bs->teamgiveorders_time = FloatTime();
;2067:				bs->numteammates = numteammates;
;2068:				bs->forceorders = qfalse;
;2069:			}
;2070:			//if it's time to give orders
;2071:			if (bs->teamgiveorders_time && bs->teamgiveorders_time < FloatTime() - 5) {
;2072:				BotHarvesterOrders(bs);
;2073:				//give orders again after 30 seconds
;2074:				bs->teamgiveorders_time = FloatTime() + 30;
;2075:			}
;2076:			break;
;2077:		}
;2078:#endif
;2079:	}
;2080:}
LABELV $446
endproc BotTeamAI 72 12
bss
export ctftaskpreferences
align 4
LABELV ctftaskpreferences
skip 2560
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
LABELV $463
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
LABELV $462
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
LABELV $459
byte 1 119
byte 1 104
byte 1 111
byte 1 105
byte 1 115
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
LABELV $214
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $209
byte 1 99
byte 1 109
byte 1 100
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $170
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
LABELV $164
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
LABELV $163
byte 1 99
byte 1 109
byte 1 100
byte 1 95
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 0
align 1
LABELV $162
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
LABELV $161
byte 1 99
byte 1 109
byte 1 100
byte 1 95
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $152
byte 1 103
byte 1 101
byte 1 116
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $151
byte 1 99
byte 1 109
byte 1 100
byte 1 95
byte 1 103
byte 1 101
byte 1 116
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $137
byte 1 25
byte 1 40
byte 1 37
byte 1 115
byte 1 25
byte 1 41
byte 1 25
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $73
byte 1 116
byte 1 0
align 1
LABELV $69
byte 1 110
byte 1 0
align 1
LABELV $62
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
