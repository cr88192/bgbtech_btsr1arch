bss
align 4
LABELV $54
skip 4
export BotNumActivePlayers
code
proc BotNumActivePlayers 1056 12
file "../ai_chat.c"
line 68
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
;25: * name:		ai_chat.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_chat.c $
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
;49://
;50:#include "chars.h"				//characteristics
;51:#include "inv.h"				//indexes into the inventory
;52:#include "syn.h"				//synonyms
;53:#include "match.h"				//string matching types and vars
;54:
;55:// for the voice chats
;56:#ifdef MISSIONPACK // bk001205
;57:#include "../../ui/menudef.h"
;58:#endif
;59:
;60:#define TIME_BETWEENCHATTING	25
;61:
;62:
;63:/*
;64:==================
;65:BotNumActivePlayers
;66:==================
;67:*/
;68:int BotNumActivePlayers(void) {
line 73
;69:	int i, num;
;70:	char buf[MAX_INFO_STRING];
;71:	static int maxclients;
;72:
;73:	if (!maxclients)
ADDRGP4 $54
INDIRI4
CNSTI4 0
NEI4 $55
line 74
;74:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1032
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $54
ADDRLP4 1032
INDIRI4
ASGNI4
LABELV $55
line 76
;75:
;76:	num = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 77
;77:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 1024
CNSTI4 0
ASGNI4
ADDRGP4 $61
JUMPV
LABELV $58
line 78
;78:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
ADDRLP4 1024
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
line 80
;79:		//if no config string or no name
;80:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 0
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $65
ADDRLP4 0
ARGP4
ADDRGP4 $64
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
NEI4 $62
LABELV $65
ADDRGP4 $59
JUMPV
LABELV $62
line 82
;81:		//skip spectators
;82:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 0
ARGP4
ADDRGP4 $68
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
NEI4 $66
ADDRGP4 $59
JUMPV
LABELV $66
line 84
;83:		//
;84:		num++;
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 85
;85:	}
LABELV $59
line 77
ADDRLP4 1024
ADDRLP4 1024
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $61
ADDRLP4 1024
INDIRI4
ADDRGP4 $54
INDIRI4
GEI4 $69
ADDRLP4 1024
INDIRI4
CNSTI4 64
LTI4 $58
LABELV $69
line 86
;86:	return num;
ADDRLP4 1028
INDIRI4
RETI4
LABELV $53
endproc BotNumActivePlayers 1056 12
bss
align 4
LABELV $71
skip 4
export BotIsFirstInRankings
code
proc BotIsFirstInRankings 1524 12
line 94
;87:}
;88:
;89:/*
;90:==================
;91:BotIsFirstInRankings
;92:==================
;93:*/
;94:int BotIsFirstInRankings(bot_state_t *bs) {
line 100
;95:	int i, score;
;96:	char buf[MAX_INFO_STRING];
;97:	static int maxclients;
;98:	playerState_t ps;
;99:
;100:	if (!maxclients)
ADDRGP4 $71
INDIRI4
CNSTI4 0
NEI4 $72
line 101
;101:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1500
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $71
ADDRLP4 1500
INDIRI4
ASGNI4
LABELV $72
line 103
;102:
;103:	score = bs->cur_ps.persistant[PERS_SCORE];
ADDRLP4 1496
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRI4
ASGNI4
line 104
;104:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $77
JUMPV
LABELV $74
line 105
;105:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 107
;106:		//if no config string or no name
;107:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1504
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1504
INDIRI4
CNSTI4 0
EQI4 $80
ADDRLP4 4
ARGP4
ADDRGP4 $64
ARGP4
ADDRLP4 1508
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1508
INDIRP4
ARGP4
ADDRLP4 1512
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1512
INDIRI4
CNSTI4 0
NEI4 $78
LABELV $80
ADDRGP4 $75
JUMPV
LABELV $78
line 109
;108:		//skip spectators
;109:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $68
ARGP4
ADDRLP4 1516
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1516
INDIRP4
ARGP4
ADDRLP4 1520
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1520
INDIRI4
CNSTI4 3
NEI4 $81
ADDRGP4 $75
JUMPV
LABELV $81
line 111
;110:		//
;111:		BotAI_GetClientState(i, &ps);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1028
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 112
;112:		if (score < ps.persistant[PERS_SCORE]) return qfalse;
ADDRLP4 1496
INDIRI4
ADDRLP4 1028+248
INDIRI4
GEI4 $83
CNSTI4 0
RETI4
ADDRGP4 $70
JUMPV
LABELV $83
line 113
;113:	}
LABELV $75
line 104
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $77
ADDRLP4 0
INDIRI4
ADDRGP4 $71
INDIRI4
GEI4 $86
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $74
LABELV $86
line 114
;114:	return qtrue;
CNSTI4 1
RETI4
LABELV $70
endproc BotIsFirstInRankings 1524 12
bss
align 4
LABELV $88
skip 4
export BotIsLastInRankings
code
proc BotIsLastInRankings 1524 12
line 122
;115:}
;116:
;117:/*
;118:==================
;119:BotIsLastInRankings
;120:==================
;121:*/
;122:int BotIsLastInRankings(bot_state_t *bs) {
line 128
;123:	int i, score;
;124:	char buf[MAX_INFO_STRING];
;125:	static int maxclients;
;126:	playerState_t ps;
;127:
;128:	if (!maxclients)
ADDRGP4 $88
INDIRI4
CNSTI4 0
NEI4 $89
line 129
;129:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1500
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $88
ADDRLP4 1500
INDIRI4
ASGNI4
LABELV $89
line 131
;130:
;131:	score = bs->cur_ps.persistant[PERS_SCORE];
ADDRLP4 1496
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRI4
ASGNI4
line 132
;132:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $94
JUMPV
LABELV $91
line 133
;133:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 135
;134:		//if no config string or no name
;135:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1504
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1504
INDIRI4
CNSTI4 0
EQI4 $97
ADDRLP4 4
ARGP4
ADDRGP4 $64
ARGP4
ADDRLP4 1508
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1508
INDIRP4
ARGP4
ADDRLP4 1512
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1512
INDIRI4
CNSTI4 0
NEI4 $95
LABELV $97
ADDRGP4 $92
JUMPV
LABELV $95
line 137
;136:		//skip spectators
;137:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $68
ARGP4
ADDRLP4 1516
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1516
INDIRP4
ARGP4
ADDRLP4 1520
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1520
INDIRI4
CNSTI4 3
NEI4 $98
ADDRGP4 $92
JUMPV
LABELV $98
line 139
;138:		//
;139:		BotAI_GetClientState(i, &ps);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1028
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 140
;140:		if (score > ps.persistant[PERS_SCORE]) return qfalse;
ADDRLP4 1496
INDIRI4
ADDRLP4 1028+248
INDIRI4
LEI4 $100
CNSTI4 0
RETI4
ADDRGP4 $87
JUMPV
LABELV $100
line 141
;141:	}
LABELV $92
line 132
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $94
ADDRLP4 0
INDIRI4
ADDRGP4 $88
INDIRI4
GEI4 $103
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $91
LABELV $103
line 142
;142:	return qtrue;
CNSTI4 1
RETI4
LABELV $87
endproc BotIsLastInRankings 1524 12
bss
align 1
LABELV $105
skip 32
align 4
LABELV $106
skip 4
export BotFirstClientInRankings
code
proc BotFirstClientInRankings 1528 12
line 150
;143:}
;144:
;145:/*
;146:==================
;147:BotFirstClientInRankings
;148:==================
;149:*/
;150:char *BotFirstClientInRankings(void) {
line 157
;151:	int i, bestscore, bestclient;
;152:	char buf[MAX_INFO_STRING];
;153:	static char name[32];
;154:	static int maxclients;
;155:	playerState_t ps;
;156:
;157:	if (!maxclients)
ADDRGP4 $106
INDIRI4
CNSTI4 0
NEI4 $107
line 158
;158:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1504
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $106
ADDRLP4 1504
INDIRI4
ASGNI4
LABELV $107
line 160
;159:
;160:	bestscore = -999999;
ADDRLP4 1496
CNSTI4 -999999
ASGNI4
line 161
;161:	bestclient = 0;
ADDRLP4 1500
CNSTI4 0
ASGNI4
line 162
;162:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $112
JUMPV
LABELV $109
line 163
;163:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 165
;164:		//if no config string or no name
;165:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1508
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1508
INDIRI4
CNSTI4 0
EQI4 $115
ADDRLP4 4
ARGP4
ADDRGP4 $64
ARGP4
ADDRLP4 1512
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1512
INDIRP4
ARGP4
ADDRLP4 1516
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1516
INDIRI4
CNSTI4 0
NEI4 $113
LABELV $115
ADDRGP4 $110
JUMPV
LABELV $113
line 167
;166:		//skip spectators
;167:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $68
ARGP4
ADDRLP4 1520
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1520
INDIRP4
ARGP4
ADDRLP4 1524
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1524
INDIRI4
CNSTI4 3
NEI4 $116
ADDRGP4 $110
JUMPV
LABELV $116
line 169
;168:		//
;169:		BotAI_GetClientState(i, &ps);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1028
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 170
;170:		if (ps.persistant[PERS_SCORE] > bestscore) {
ADDRLP4 1028+248
INDIRI4
ADDRLP4 1496
INDIRI4
LEI4 $118
line 171
;171:			bestscore = ps.persistant[PERS_SCORE];
ADDRLP4 1496
ADDRLP4 1028+248
INDIRI4
ASGNI4
line 172
;172:			bestclient = i;
ADDRLP4 1500
ADDRLP4 0
INDIRI4
ASGNI4
line 173
;173:		}
LABELV $118
line 174
;174:	}
LABELV $110
line 162
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $112
ADDRLP4 0
INDIRI4
ADDRGP4 $106
INDIRI4
GEI4 $122
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $109
LABELV $122
line 175
;175:	EasyClientName(bestclient, name, 32);
ADDRLP4 1500
INDIRI4
ARGI4
ADDRGP4 $105
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 176
;176:	return name;
ADDRGP4 $105
RETP4
LABELV $104
endproc BotFirstClientInRankings 1528 12
bss
align 1
LABELV $124
skip 32
align 4
LABELV $125
skip 4
export BotLastClientInRankings
code
proc BotLastClientInRankings 1528 12
line 184
;177:}
;178:
;179:/*
;180:==================
;181:BotLastClientInRankings
;182:==================
;183:*/
;184:char *BotLastClientInRankings(void) {
line 191
;185:	int i, worstscore, bestclient;
;186:	char buf[MAX_INFO_STRING];
;187:	static char name[32];
;188:	static int maxclients;
;189:	playerState_t ps;
;190:
;191:	if (!maxclients)
ADDRGP4 $125
INDIRI4
CNSTI4 0
NEI4 $126
line 192
;192:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1504
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $125
ADDRLP4 1504
INDIRI4
ASGNI4
LABELV $126
line 194
;193:
;194:	worstscore = 999999;
ADDRLP4 1496
CNSTI4 999999
ASGNI4
line 195
;195:	bestclient = 0;
ADDRLP4 1500
CNSTI4 0
ASGNI4
line 196
;196:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $131
JUMPV
LABELV $128
line 197
;197:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 199
;198:		//if no config string or no name
;199:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1508
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1508
INDIRI4
CNSTI4 0
EQI4 $134
ADDRLP4 4
ARGP4
ADDRGP4 $64
ARGP4
ADDRLP4 1512
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1512
INDIRP4
ARGP4
ADDRLP4 1516
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1516
INDIRI4
CNSTI4 0
NEI4 $132
LABELV $134
ADDRGP4 $129
JUMPV
LABELV $132
line 201
;200:		//skip spectators
;201:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $68
ARGP4
ADDRLP4 1520
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1520
INDIRP4
ARGP4
ADDRLP4 1524
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1524
INDIRI4
CNSTI4 3
NEI4 $135
ADDRGP4 $129
JUMPV
LABELV $135
line 203
;202:		//
;203:		BotAI_GetClientState(i, &ps);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1028
ARGP4
ADDRGP4 BotAI_GetClientState
CALLI4
pop
line 204
;204:		if (ps.persistant[PERS_SCORE] < worstscore) {
ADDRLP4 1028+248
INDIRI4
ADDRLP4 1496
INDIRI4
GEI4 $137
line 205
;205:			worstscore = ps.persistant[PERS_SCORE];
ADDRLP4 1496
ADDRLP4 1028+248
INDIRI4
ASGNI4
line 206
;206:			bestclient = i;
ADDRLP4 1500
ADDRLP4 0
INDIRI4
ASGNI4
line 207
;207:		}
LABELV $137
line 208
;208:	}
LABELV $129
line 196
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $131
ADDRLP4 0
INDIRI4
ADDRGP4 $125
INDIRI4
GEI4 $141
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $128
LABELV $141
line 209
;209:	EasyClientName(bestclient, name, 32);
ADDRLP4 1500
INDIRI4
ARGI4
ADDRGP4 $124
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 210
;210:	return name;
ADDRGP4 $124
RETP4
LABELV $123
endproc BotLastClientInRankings 1528 12
bss
align 4
LABELV $143
skip 4
align 1
LABELV $144
skip 32
export BotRandomOpponentName
code
proc BotRandomOpponentName 1320 12
line 218
;211:}
;212:
;213:/*
;214:==================
;215:BotRandomOpponentName
;216:==================
;217:*/
;218:char *BotRandomOpponentName(bot_state_t *bs) {
line 225
;219:	int i, count;
;220:	char buf[MAX_INFO_STRING];
;221:	int opponents[MAX_CLIENTS], numopponents;
;222:	static int maxclients;
;223:	static char name[32];
;224:
;225:	if (!maxclients)
ADDRGP4 $143
INDIRI4
CNSTI4 0
NEI4 $145
line 226
;226:		maxclients = trap_Cvar_VariableIntegerValue("sv_maxclients");
ADDRGP4 $57
ARGP4
ADDRLP4 1292
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRGP4 $143
ADDRLP4 1292
INDIRI4
ASGNI4
LABELV $145
line 228
;227:
;228:	numopponents = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 229
;229:	opponents[0] = 0;
ADDRLP4 1036
CNSTI4 0
ASGNI4
line 230
;230:	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $150
JUMPV
LABELV $147
line 231
;231:		if (i == bs->client) continue;
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $151
ADDRGP4 $148
JUMPV
LABELV $151
line 233
;232:		//
;233:		trap_GetConfigstring(CS_PLAYERS+i, buf, sizeof(buf));
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
line 235
;234:		//if no config string or no name
;235:		if (!strlen(buf) || !strlen(Info_ValueForKey(buf, "n"))) continue;
ADDRLP4 4
ARGP4
ADDRLP4 1296
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1296
INDIRI4
CNSTI4 0
EQI4 $155
ADDRLP4 4
ARGP4
ADDRGP4 $64
ARGP4
ADDRLP4 1300
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1300
INDIRP4
ARGP4
ADDRLP4 1304
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1304
INDIRI4
CNSTI4 0
NEI4 $153
LABELV $155
ADDRGP4 $148
JUMPV
LABELV $153
line 237
;236:		//skip spectators
;237:		if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) continue;
ADDRLP4 4
ARGP4
ADDRGP4 $68
ARGP4
ADDRLP4 1308
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1308
INDIRP4
ARGP4
ADDRLP4 1312
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1312
INDIRI4
CNSTI4 3
NEI4 $156
ADDRGP4 $148
JUMPV
LABELV $156
line 239
;238:		//skip team mates
;239:		if (BotSameTeam(bs, i)) continue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1316
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 1316
INDIRI4
CNSTI4 0
EQI4 $158
ADDRGP4 $148
JUMPV
LABELV $158
line 241
;240:		//
;241:		opponents[numopponents] = i;
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1036
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 242
;242:		numopponents++;
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 243
;243:	}
LABELV $148
line 230
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $150
ADDRLP4 0
INDIRI4
ADDRGP4 $143
INDIRI4
GEI4 $160
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $147
LABELV $160
line 244
;244:	count = random() * numopponents;
ADDRLP4 1300
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 1300
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 1028
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 245
;245:	for (i = 0; i < numopponents; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $164
JUMPV
LABELV $161
line 246
;246:		count--;
ADDRLP4 1032
ADDRLP4 1032
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 247
;247:		if (count <= 0) {
ADDRLP4 1032
INDIRI4
CNSTI4 0
GTI4 $165
line 248
;248:			EasyClientName(opponents[i], name, sizeof(name));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1036
ADDP4
INDIRI4
ARGI4
ADDRGP4 $144
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 249
;249:			return name;
ADDRGP4 $144
RETP4
ADDRGP4 $142
JUMPV
LABELV $165
line 251
;250:		}
;251:	}
LABELV $162
line 245
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $164
ADDRLP4 0
INDIRI4
ADDRLP4 1028
INDIRI4
LTI4 $161
line 252
;252:	EasyClientName(opponents[0], name, sizeof(name));
ADDRLP4 1036
INDIRI4
ARGI4
ADDRGP4 $144
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 253
;253:	return name;
ADDRGP4 $144
RETP4
LABELV $142
endproc BotRandomOpponentName 1320 12
bss
align 1
LABELV $168
skip 128
export BotMapTitle
code
proc BotMapTitle 1028 12
line 262
;254:}
;255:
;256:/*
;257:==================
;258:BotMapTitle
;259:==================
;260:*/
;261:
;262:char *BotMapTitle(void) {
line 266
;263:	char info[1024];
;264:	static char mapname[128];
;265:
;266:	trap_GetServerinfo(info, sizeof(info));
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetServerinfo
CALLV
pop
line 268
;267:
;268:	strncpy(mapname, Info_ValueForKey( info, "mapname" ), sizeof(mapname)-1);
ADDRLP4 0
ARGP4
ADDRGP4 $169
ARGP4
ADDRLP4 1024
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $168
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
CNSTI4 127
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 269
;269:	mapname[sizeof(mapname)-1] = '\0';
ADDRGP4 $168+127
CNSTI1 0
ASGNI1
line 271
;270:
;271:	return mapname;
ADDRGP4 $168
RETP4
LABELV $167
endproc BotMapTitle 1028 12
export BotWeaponNameForMeansOfDeath
proc BotWeaponNameForMeansOfDeath 4 0
line 281
;272:}
;273:
;274:
;275:/*
;276:==================
;277:BotWeaponNameForMeansOfDeath
;278:==================
;279:*/
;280:
;281:char *BotWeaponNameForMeansOfDeath(int mod) {
line 282
;282:	switch(mod) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
LTI4 $172
ADDRLP4 0
INDIRI4
CNSTI4 23
GTI4 $172
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $195-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $195
address $174
address $176
address $178
address $180
address $180
address $182
address $182
address $184
address $184
address $186
address $188
address $190
address $190
address $172
address $172
address $172
address $172
address $172
address $172
address $172
address $172
address $172
address $192
code
LABELV $174
line 283
;283:		case MOD_SHOTGUN: return "Shotgun";
ADDRGP4 $175
RETP4
ADDRGP4 $171
JUMPV
LABELV $176
line 284
;284:		case MOD_GAUNTLET: return "Gauntlet";
ADDRGP4 $177
RETP4
ADDRGP4 $171
JUMPV
LABELV $178
line 285
;285:		case MOD_MACHINEGUN: return "Machinegun";
ADDRGP4 $179
RETP4
ADDRGP4 $171
JUMPV
LABELV $180
line 287
;286:		case MOD_GRENADE:
;287:		case MOD_GRENADE_SPLASH: return "Grenade Launcher";
ADDRGP4 $181
RETP4
ADDRGP4 $171
JUMPV
LABELV $182
line 289
;288:		case MOD_ROCKET:
;289:		case MOD_ROCKET_SPLASH: return "Rocket Launcher";
ADDRGP4 $183
RETP4
ADDRGP4 $171
JUMPV
LABELV $184
line 291
;290:		case MOD_PLASMA:
;291:		case MOD_PLASMA_SPLASH: return "Plasmagun";
ADDRGP4 $185
RETP4
ADDRGP4 $171
JUMPV
LABELV $186
line 292
;292:		case MOD_RAILGUN: return "Railgun";
ADDRGP4 $187
RETP4
ADDRGP4 $171
JUMPV
LABELV $188
line 293
;293:		case MOD_LIGHTNING: return "Lightning Gun";
ADDRGP4 $189
RETP4
ADDRGP4 $171
JUMPV
LABELV $190
line 295
;294:		case MOD_BFG:
;295:		case MOD_BFG_SPLASH: return "BFG10K";
ADDRGP4 $191
RETP4
ADDRGP4 $171
JUMPV
LABELV $192
line 303
;296:#ifdef MISSIONPACK
;297:		case MOD_NAIL: return "Nailgun";
;298:		case MOD_CHAINGUN: return "Chaingun";
;299:		case MOD_PROXIMITY_MINE: return "Proximity Launcher";
;300:		case MOD_KAMIKAZE: return "Kamikaze";
;301:		case MOD_JUICED: return "Prox mine";
;302:#endif
;303:		case MOD_GRAPPLE: return "Grapple";
ADDRGP4 $193
RETP4
ADDRGP4 $171
JUMPV
LABELV $172
line 304
;304:		default: return "[unknown weapon]";
ADDRGP4 $194
RETP4
LABELV $171
endproc BotWeaponNameForMeansOfDeath 4 0
export BotRandomWeaponName
proc BotRandomWeaponName 12 0
line 313
;305:	}
;306:}
;307:
;308:/*
;309:==================
;310:BotRandomWeaponName
;311:==================
;312:*/
;313:char *BotRandomWeaponName(void) {
line 319
;314:	int rnd;
;315:
;316:#ifdef MISSIONPACK
;317:	rnd = random() * 11.9;
;318:#else
;319:	rnd = random() * 8.9;
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
CNSTF4 1091462758
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CVFI4 4
ASGNI4
line 321
;320:#endif
;321:	switch(rnd) {
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $198
ADDRLP4 8
INDIRI4
CNSTI4 7
GTI4 $198
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $208
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $208
address $200
address $201
address $202
address $203
address $204
address $205
address $206
address $207
code
LABELV $200
line 322
;322:		case 0: return "Gauntlet";
ADDRGP4 $177
RETP4
ADDRGP4 $197
JUMPV
LABELV $201
line 323
;323:		case 1: return "Shotgun";
ADDRGP4 $175
RETP4
ADDRGP4 $197
JUMPV
LABELV $202
line 324
;324:		case 2: return "Machinegun";
ADDRGP4 $179
RETP4
ADDRGP4 $197
JUMPV
LABELV $203
line 325
;325:		case 3: return "Grenade Launcher";
ADDRGP4 $181
RETP4
ADDRGP4 $197
JUMPV
LABELV $204
line 326
;326:		case 4: return "Rocket Launcher";
ADDRGP4 $183
RETP4
ADDRGP4 $197
JUMPV
LABELV $205
line 327
;327:		case 5: return "Plasmagun";
ADDRGP4 $185
RETP4
ADDRGP4 $197
JUMPV
LABELV $206
line 328
;328:		case 6: return "Railgun";
ADDRGP4 $187
RETP4
ADDRGP4 $197
JUMPV
LABELV $207
line 329
;329:		case 7: return "Lightning Gun";
ADDRGP4 $189
RETP4
ADDRGP4 $197
JUMPV
LABELV $198
line 335
;330:#ifdef MISSIONPACK
;331:		case 8: return "Nailgun";
;332:		case 9: return "Chaingun";
;333:		case 10: return "Proximity Launcher";
;334:#endif
;335:		default: return "BFG10K";
ADDRGP4 $191
RETP4
LABELV $197
endproc BotRandomWeaponName 12 0
export BotVisibleEnemies
proc BotVisibleEnemies 172 20
line 344
;336:	}
;337:}
;338:
;339:/*
;340:==================
;341:BotVisibleEnemies
;342:==================
;343:*/
;344:int BotVisibleEnemies(bot_state_t *bs) {
line 349
;345:	float vis;
;346:	int i;
;347:	aas_entityinfo_t entinfo;
;348:
;349:	for (i = 0; i < MAX_CLIENTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $210
line 351
;350:
;351:		if (i == bs->client) continue;
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $214
ADDRGP4 $211
JUMPV
LABELV $214
line 353
;352:		//
;353:		BotEntityInfo(i, &entinfo);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 355
;354:		//
;355:		if (!entinfo.valid) continue;
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $216
ADDRGP4 $211
JUMPV
LABELV $216
line 357
;356:		//if the enemy isn't dead and the enemy isn't the bot self
;357:		if (EntityIsDead(&entinfo) || entinfo.number == bs->entitynum) continue;
ADDRLP4 4
ARGP4
ADDRLP4 148
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $221
ADDRLP4 4+20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
NEI4 $218
LABELV $221
ADDRGP4 $211
JUMPV
LABELV $218
line 359
;358:		//if the enemy is invisible and not shooting
;359:		if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
ADDRLP4 4
ARGP4
ADDRLP4 152
ADDRGP4 EntityIsInvisible
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $222
ADDRLP4 4
ARGP4
ADDRLP4 156
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 0
NEI4 $222
line 360
;360:			continue;
ADDRGP4 $211
JUMPV
LABELV $222
line 363
;361:		}
;362:		//if on the same team
;363:		if (BotSameTeam(bs, i)) continue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 160
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
EQI4 $224
ADDRGP4 $211
JUMPV
LABELV $224
line 365
;364:		//check if the enemy is visible
;365:		vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
ADDRLP4 164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 164
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 164
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 164
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 168
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 144
ADDRLP4 168
INDIRF4
ASGNF4
line 366
;366:		if (vis > 0) return qtrue;
ADDRLP4 144
INDIRF4
CNSTF4 0
LEF4 $226
CNSTI4 1
RETI4
ADDRGP4 $209
JUMPV
LABELV $226
line 367
;367:	}
LABELV $211
line 349
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $210
line 368
;368:	return qfalse;
CNSTI4 0
RETI4
LABELV $209
endproc BotVisibleEnemies 172 20
export BotValidChatPosition
proc BotValidChatPosition 164 28
line 376
;369:}
;370:
;371:/*
;372:==================
;373:BotValidChatPosition
;374:==================
;375:*/
;376:int BotValidChatPosition(bot_state_t *bs) {
line 381
;377:	vec3_t point, start, end, mins, maxs;
;378:	bsp_trace_t trace;
;379:
;380:	//if the bot is dead all positions are valid
;381:	if (BotIsDead(bs)) return qtrue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 144
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 0
EQI4 $229
CNSTI4 1
RETI4
ADDRGP4 $228
JUMPV
LABELV $229
line 383
;382:	//never start chatting with a powerup
;383:	if (bs->inventory[INVENTORY_QUAD] ||
ADDRLP4 148
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 152
CNSTI4 0
ASGNI4
ADDRLP4 148
INDIRP4
CNSTI4 5092
ADDP4
INDIRI4
ADDRLP4 152
INDIRI4
NEI4 $236
ADDRLP4 148
INDIRP4
CNSTI4 5100
ADDP4
INDIRI4
ADDRLP4 152
INDIRI4
NEI4 $236
ADDRLP4 148
INDIRP4
CNSTI4 5104
ADDP4
INDIRI4
ADDRLP4 152
INDIRI4
NEI4 $236
ADDRLP4 148
INDIRP4
CNSTI4 5108
ADDP4
INDIRI4
ADDRLP4 152
INDIRI4
NEI4 $236
ADDRLP4 148
INDIRP4
CNSTI4 5112
ADDP4
INDIRI4
ADDRLP4 152
INDIRI4
EQI4 $231
LABELV $236
line 387
;384:		bs->inventory[INVENTORY_HASTE] ||
;385:		bs->inventory[INVENTORY_INVISIBILITY] ||
;386:		bs->inventory[INVENTORY_REGEN] ||
;387:		bs->inventory[INVENTORY_FLIGHT]) return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $228
JUMPV
LABELV $231
line 391
;388:	//must be on the ground
;389:	//if (bs->cur_ps.groundEntityNum != ENTITYNUM_NONE) return qfalse;
;390:	//do not chat if in lava or slime
;391:	VectorCopy(bs->origin, point);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 392
;392:	point[2] -= 24;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1103101952
SUBF4
ASGNF4
line 393
;393:	if (trap_PointContents(point,bs->entitynum) & (CONTENTS_LAVA|CONTENTS_SLIME)) return qfalse;
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 156
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 24
BANDI4
CNSTI4 0
EQI4 $238
CNSTI4 0
RETI4
ADDRGP4 $228
JUMPV
LABELV $238
line 395
;394:	//do not chat if under water
;395:	VectorCopy(bs->origin, point);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 396
;396:	point[2] += 32;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1107296256
ADDF4
ASGNF4
line 397
;397:	if (trap_PointContents(point,bs->entitynum) & MASK_WATER) return qfalse;
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 160
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $241
CNSTI4 0
RETI4
ADDRGP4 $228
JUMPV
LABELV $241
line 399
;398:	//must be standing on the world entity
;399:	VectorCopy(bs->origin, start);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 400
;400:	VectorCopy(bs->origin, end);
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 401
;401:	start[2] += 1;
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 402
;402:	end[2] -= 10;
ADDRLP4 24+8
ADDRLP4 24+8
INDIRF4
CNSTF4 1092616192
SUBF4
ASGNF4
line 403
;403:	trap_AAS_PresenceTypeBoundingBox(PRESENCE_CROUCH, mins, maxs);
CNSTI4 4
ARGI4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 trap_AAS_PresenceTypeBoundingBox
CALLV
pop
line 404
;404:	BotAI_Trace(&trace, start, mins, maxs, end, bs->client, MASK_SOLID);
ADDRLP4 60
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 48
ARGP4
ADDRLP4 24
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 405
;405:	if (trace.ent != ENTITYNUM_WORLD) return qfalse;
ADDRLP4 60+80
INDIRI4
CNSTI4 1022
EQI4 $245
CNSTI4 0
RETI4
ADDRGP4 $228
JUMPV
LABELV $245
line 407
;406:	//the bot is in a position where it can chat
;407:	return qtrue;
CNSTI4 1
RETI4
LABELV $228
endproc BotValidChatPosition 164 28
export BotChat_EnterGame
proc BotChat_EnterGame 68 32
line 415
;408:}
;409:
;410:/*
;411:==================
;412:BotChat_EnterGame
;413:==================
;414:*/
;415:int BotChat_EnterGame(bot_state_t *bs) {
line 419
;416:	char name[32];
;417:	float rnd;
;418:
;419:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $249
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $249
line 420
;420:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $252
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $252
line 422
;421:	//don't chat in teamplay
;422:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 36
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $254
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $254
line 424
;423:	// don't chat in tournament mode
;424:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $256
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $256
line 425
;425:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_ENTEREXITGAME, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 27
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 40
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 40
INDIRF4
ASGNF4
line 426
;426:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $258
line 427
;427:		if (random() > rnd) return qfalse;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 0
INDIRF4
LEF4 $261
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $261
line 428
;428:	}
LABELV $258
line 429
;429:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 44
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 1
GTI4 $263
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $263
line 430
;430:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $265
CNSTI4 0
RETI4
ADDRGP4 $248
JUMPV
LABELV $265
line 431
;431:	BotAI_BotInitialChat(bs, "game_enter",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 52
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 60
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 $268
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 438
;432:				EasyClientName(bs->client, name, 32),	// 0
;433:				BotRandomOpponentName(bs),				// 1
;434:				"[invalid var]",						// 2
;435:				"[invalid var]",						// 3
;436:				BotMapTitle(),							// 4
;437:				NULL);
;438:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 439
;439:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 440
;440:	return qtrue;
CNSTI4 1
RETI4
LABELV $248
endproc BotChat_EnterGame 68 32
export BotChat_ExitGame
proc BotChat_ExitGame 64 32
line 448
;441:}
;442:
;443:/*
;444:==================
;445:BotChat_ExitGame
;446:==================
;447:*/
;448:int BotChat_ExitGame(bot_state_t *bs) {
line 452
;449:	char name[32];
;450:	float rnd;
;451:
;452:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $270
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $270
line 453
;453:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $273
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $273
line 455
;454:	//don't chat in teamplay
;455:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 36
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $275
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $275
line 457
;456:	// don't chat in tournament mode
;457:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $277
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $277
line 458
;458:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_ENTEREXITGAME, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 27
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 40
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 40
INDIRF4
ASGNF4
line 459
;459:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $279
line 460
;460:		if (random() > rnd) return qfalse;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 0
INDIRF4
LEF4 $282
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $282
line 461
;461:	}
LABELV $279
line 462
;462:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 44
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 1
GTI4 $284
CNSTI4 0
RETI4
ADDRGP4 $269
JUMPV
LABELV $284
line 464
;463:	//
;464:	BotAI_BotInitialChat(bs, "game_exit",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 48
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 56
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 $268
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 471
;465:				EasyClientName(bs->client, name, 32),	// 0
;466:				BotRandomOpponentName(bs),				// 1
;467:				"[invalid var]",						// 2
;468:				"[invalid var]",						// 3
;469:				BotMapTitle(),							// 4
;470:				NULL);
;471:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 472
;472:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 473
;473:	return qtrue;
CNSTI4 1
RETI4
LABELV $269
endproc BotChat_ExitGame 64 32
export BotChat_StartLevel
proc BotChat_StartLevel 56 16
line 481
;474:}
;475:
;476:/*
;477:==================
;478:BotChat_StartLevel
;479:==================
;480:*/
;481:int BotChat_StartLevel(bot_state_t *bs) {
line 485
;482:	char name[32];
;483:	float rnd;
;484:
;485:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $288
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $288
line 486
;486:	if (BotIsObserver(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $291
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $291
line 487
;487:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $293
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $293
line 489
;488:	//don't chat in teamplay
;489:	if (TeamPlayIsOn()) {
ADDRLP4 40
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $295
line 490
;490:	    trap_EA_Command(bs->client, "vtaunt");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 491
;491:	    return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $295
line 494
;492:	}
;493:	// don't chat in tournament mode
;494:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $298
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $298
line 495
;495:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_STARTENDLEVEL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 26
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 44
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 44
INDIRF4
ASGNF4
line 496
;496:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $300
line 497
;497:		if (random() > rnd) return qfalse;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 0
INDIRF4
LEF4 $303
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $303
line 498
;498:	}
LABELV $300
line 499
;499:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 48
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 1
GTI4 $305
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $305
line 500
;500:	BotAI_BotInitialChat(bs, "level_start",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 52
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $307
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 503
;501:				EasyClientName(bs->client, name, 32),	// 0
;502:				NULL);
;503:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 504
;504:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 505
;505:	return qtrue;
CNSTI4 1
RETI4
LABELV $287
endproc BotChat_StartLevel 56 16
export BotChat_EndLevel
proc BotChat_EndLevel 80 32
line 513
;506:}
;507:
;508:/*
;509:==================
;510:BotChat_EndLevel
;511:==================
;512:*/
;513:int BotChat_EndLevel(bot_state_t *bs) {
line 517
;514:	char name[32];
;515:	float rnd;
;516:
;517:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $309
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $309
line 518
;518:	if (BotIsObserver(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $312
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $312
line 519
;519:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $314
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $314
line 521
;520:	// teamplay
;521:	if (TeamPlayIsOn()) 
ADDRLP4 40
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $316
line 522
;522:	{
line 523
;523:		if (BotIsFirstInRankings(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 BotIsFirstInRankings
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $318
line 524
;524:			trap_EA_Command(bs->client, "vtaunt");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 525
;525:		}
LABELV $318
line 526
;526:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $308
JUMPV
LABELV $316
line 529
;527:	}
;528:	// don't chat in tournament mode
;529:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $320
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $320
line 530
;530:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_STARTENDLEVEL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 26
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 44
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 44
INDIRF4
ASGNF4
line 531
;531:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $322
line 532
;532:		if (random() > rnd) return qfalse;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 0
INDIRF4
LEF4 $325
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $325
line 533
;533:	}
LABELV $322
line 534
;534:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 48
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 1
GTI4 $327
CNSTI4 0
RETI4
ADDRGP4 $308
JUMPV
LABELV $327
line 536
;535:	//
;536:	if (BotIsFirstInRankings(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotIsFirstInRankings
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $329
line 537
;537:		BotAI_BotInitialChat(bs, "level_end_victory",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 56
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 64
ADDRGP4 BotLastClientInRankings
CALLP4
ASGNP4
ADDRLP4 68
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $331
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 544
;538:				EasyClientName(bs->client, name, 32),	// 0
;539:				BotRandomOpponentName(bs),				// 1
;540:				"[invalid var]",						// 2
;541:				BotLastClientInRankings(),				// 3
;542:				BotMapTitle(),							// 4
;543:				NULL);
;544:	}
ADDRGP4 $330
JUMPV
LABELV $329
line 545
;545:	else if (BotIsLastInRankings(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 BotIsLastInRankings
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $332
line 546
;546:		BotAI_BotInitialChat(bs, "level_end_lose",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 60
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 68
ADDRGP4 BotFirstClientInRankings
CALLP4
ASGNP4
ADDRLP4 72
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $334
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 553
;547:				EasyClientName(bs->client, name, 32),	// 0
;548:				BotRandomOpponentName(bs),				// 1
;549:				BotFirstClientInRankings(),				// 2
;550:				"[invalid var]",						// 3
;551:				BotMapTitle(),							// 4
;552:				NULL);
;553:	}
ADDRGP4 $333
JUMPV
LABELV $332
line 554
;554:	else {
line 555
;555:		BotAI_BotInitialChat(bs, "level_end",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 60
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 68
ADDRGP4 BotFirstClientInRankings
CALLP4
ASGNP4
ADDRLP4 72
ADDRGP4 BotLastClientInRankings
CALLP4
ASGNP4
ADDRLP4 76
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $335
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 562
;556:				EasyClientName(bs->client, name, 32),	// 0
;557:				BotRandomOpponentName(bs),				// 1
;558:				BotFirstClientInRankings(),				// 2
;559:				BotLastClientInRankings(),				// 3
;560:				BotMapTitle(),							// 4
;561:				NULL);
;562:	}
LABELV $333
LABELV $330
line 563
;563:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 564
;564:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 565
;565:	return qtrue;
CNSTI4 1
RETI4
LABELV $308
endproc BotChat_EndLevel 80 32
export BotChat_Death
proc BotChat_Death 116 20
line 573
;566:}
;567:
;568:/*
;569:==================
;570:BotChat_Death
;571:==================
;572:*/
;573:int BotChat_Death(bot_state_t *bs) {
line 577
;574:	char name[32];
;575:	float rnd;
;576:
;577:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $337
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $337
line 578
;578:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $340
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $340
line 579
;579:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_DEATH, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 29
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 36
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 36
INDIRF4
ASGNF4
line 581
;580:	// don't chat in tournament mode
;581:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $342
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $342
line 583
;582:	//if fast chatting is off
;583:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $344
line 584
;584:		if (random() > rnd) return qfalse;
ADDRLP4 40
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 32
INDIRF4
LEF4 $347
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $347
line 585
;585:	}
LABELV $344
line 586
;586:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 40
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 1
GTI4 $349
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $349
line 588
;587:	//
;588:	if (bs->lastkilledby >= 0 && bs->lastkilledby < MAX_CLIENTS)
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 5996
ADDP4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
LTI4 $351
ADDRLP4 44
INDIRI4
CNSTI4 64
GEI4 $351
line 589
;589:		EasyClientName(bs->lastkilledby, name, 32);
ADDRFP4 0
INDIRP4
CNSTI4 5996
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
ADDRGP4 $352
JUMPV
LABELV $351
line 591
;590:	else
;591:		strcpy(name, "[world]");
ADDRLP4 0
ARGP4
ADDRGP4 $353
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $352
line 593
;592:	//
;593:	if (TeamPlayIsOn() && BotSameTeam(bs, bs->lastkilledby)) {
ADDRLP4 48
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $354
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
CNSTI4 5996
ADDP4
INDIRI4
ARGI4
ADDRLP4 56
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $354
line 594
;594:		if (bs->lastkilledby == bs->client) return qfalse;
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 5996
ADDP4
INDIRI4
ADDRLP4 60
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $356
CNSTI4 0
RETI4
ADDRGP4 $336
JUMPV
LABELV $356
line 595
;595:		BotAI_BotInitialChat(bs, "death_teammate", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $358
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 596
;596:		bs->chatto = CHAT_TEAM;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 1
ASGNI4
line 597
;597:	}
ADDRGP4 $355
JUMPV
LABELV $354
line 599
;598:	else
;599:	{
line 601
;600:		//teamplay
;601:		if (TeamPlayIsOn()) {
ADDRLP4 60
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $359
line 602
;602:			trap_EA_Command(bs->client, "vtaunt");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 603
;603:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $336
JUMPV
LABELV $359
line 606
;604:		}
;605:		//
;606:		if (bs->botdeathtype == MOD_WATER)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 14
NEI4 $361
line 607
;607:			BotAI_BotInitialChat(bs, "death_drown", BotRandomOpponentName(bs), NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $363
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $362
JUMPV
LABELV $361
line 608
;608:		else if (bs->botdeathtype == MOD_SLIME)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 15
NEI4 $364
line 609
;609:			BotAI_BotInitialChat(bs, "death_slime", BotRandomOpponentName(bs), NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $366
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $365
JUMPV
LABELV $364
line 610
;610:		else if (bs->botdeathtype == MOD_LAVA)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 16
NEI4 $367
line 611
;611:			BotAI_BotInitialChat(bs, "death_lava", BotRandomOpponentName(bs), NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $369
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $368
JUMPV
LABELV $367
line 612
;612:		else if (bs->botdeathtype == MOD_FALLING)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 19
NEI4 $370
line 613
;613:			BotAI_BotInitialChat(bs, "death_cratered", BotRandomOpponentName(bs), NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 76
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $372
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $371
JUMPV
LABELV $370
line 614
;614:		else if (bs->botsuicide || //all other suicides by own weapon
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 84
CNSTI4 0
ASGNI4
ADDRLP4 80
INDIRP4
CNSTI4 6008
ADDP4
INDIRI4
ADDRLP4 84
INDIRI4
NEI4 $379
ADDRLP4 88
ADDRLP4 80
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ASGNI4
ADDRLP4 88
INDIRI4
CNSTI4 17
EQI4 $379
ADDRLP4 88
INDIRI4
CNSTI4 20
EQI4 $379
ADDRLP4 88
INDIRI4
CNSTI4 21
EQI4 $379
ADDRLP4 88
INDIRI4
CNSTI4 22
EQI4 $379
ADDRLP4 88
INDIRI4
ADDRLP4 84
INDIRI4
NEI4 $373
LABELV $379
line 620
;615:				bs->botdeathtype == MOD_CRUSH ||
;616:				bs->botdeathtype == MOD_SUICIDE ||
;617:				bs->botdeathtype == MOD_TARGET_LASER ||
;618:				bs->botdeathtype == MOD_TRIGGER_HURT ||
;619:				bs->botdeathtype == MOD_UNKNOWN)
;620:			BotAI_BotInitialChat(bs, "death_suicide", BotRandomOpponentName(bs), NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $374
JUMPV
LABELV $373
line 621
;621:		else if (bs->botdeathtype == MOD_TELEFRAG)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 18
NEI4 $381
line 622
;622:			BotAI_BotInitialChat(bs, "death_telefrag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $383
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $382
JUMPV
LABELV $381
line 627
;623:#ifdef MISSIONPACK
;624:		else if (bs->botdeathtype == MOD_KAMIKAZE && trap_BotNumInitialChats(bs->cs, "death_kamikaze"))
;625:			BotAI_BotInitialChat(bs, "death_kamikaze", name, NULL);
;626:#endif
;627:		else {
line 628
;628:			if ((bs->botdeathtype == MOD_GAUNTLET ||
ADDRLP4 96
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ASGNI4
ADDRLP4 96
INDIRI4
CNSTI4 2
EQI4 $388
ADDRLP4 96
INDIRI4
CNSTI4 10
EQI4 $388
ADDRLP4 96
INDIRI4
CNSTI4 12
EQI4 $388
ADDRLP4 96
INDIRI4
CNSTI4 13
NEI4 $384
LABELV $388
ADDRLP4 100
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 100
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
GEF4 $384
line 631
;629:				bs->botdeathtype == MOD_RAILGUN ||
;630:				bs->botdeathtype == MOD_BFG ||
;631:				bs->botdeathtype == MOD_BFG_SPLASH) && random() < 0.5) {
line 633
;632:
;633:				if (bs->botdeathtype == MOD_GAUNTLET)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 2
NEI4 $389
line 634
;634:					BotAI_BotInitialChat(bs, "death_gauntlet",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 104
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $391
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $385
JUMPV
LABELV $389
line 638
;635:							name,												// 0
;636:							BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;637:							NULL);
;638:				else if (bs->botdeathtype == MOD_RAILGUN)
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
CNSTI4 10
NEI4 $392
line 639
;639:					BotAI_BotInitialChat(bs, "death_rail",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $394
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
ADDRGP4 $385
JUMPV
LABELV $392
line 644
;640:							name,												// 0
;641:							BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;642:							NULL);
;643:				else
;644:					BotAI_BotInitialChat(bs, "death_bfg",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $395
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 648
;645:							name,												// 0
;646:							BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;647:							NULL);
;648:			}
ADDRGP4 $385
JUMPV
LABELV $384
line 650
;649:			//choose between insult and praise
;650:			else if (random() < trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_INSULT, 0, 1)) {
ADDRLP4 104
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 24
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 108
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 104
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 108
INDIRF4
GEF4 $396
line 651
;651:				BotAI_BotInitialChat(bs, "death_insult",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $398
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 655
;652:							name,												// 0
;653:							BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;654:							NULL);
;655:			}
ADDRGP4 $397
JUMPV
LABELV $396
line 656
;656:			else {
line 657
;657:				BotAI_BotInitialChat(bs, "death_praise",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $399
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 661
;658:							name,												// 0
;659:							BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;660:							NULL);
;661:			}
LABELV $397
LABELV $385
line 662
;662:		}
LABELV $382
LABELV $374
LABELV $371
LABELV $368
LABELV $365
LABELV $362
line 663
;663:		bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 664
;664:	}
LABELV $355
line 665
;665:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 666
;666:	return qtrue;
CNSTI4 1
RETI4
LABELV $336
endproc BotChat_Death 116 20
export BotChat_Kill
proc BotChat_Kill 80 16
line 674
;667:}
;668:
;669:/*
;670:==================
;671:BotChat_Kill
;672:==================
;673:*/
;674:int BotChat_Kill(bot_state_t *bs) {
line 678
;675:	char name[32];
;676:	float rnd;
;677:
;678:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $401
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $401
line 679
;679:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $404
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $404
line 680
;680:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_KILL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 28
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 36
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 36
INDIRF4
ASGNF4
line 682
;681:	// don't chat in tournament mode
;682:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $406
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $406
line 684
;683:	//if fast chat is off
;684:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $408
line 685
;685:		if (random() > rnd) return qfalse;
ADDRLP4 40
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 32
INDIRF4
LEF4 $411
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $411
line 686
;686:	}
LABELV $408
line 687
;687:	if (bs->lastkilledplayer == bs->client) return qfalse;
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $413
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $413
line 688
;688:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 44
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 1
GTI4 $415
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $415
line 689
;689:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $417
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $417
line 691
;690:	//
;691:	if (BotVisibleEnemies(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotVisibleEnemies
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $419
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $419
line 693
;692:	//
;693:	EasyClientName(bs->lastkilledplayer, name, 32);
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 695
;694:	//
;695:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 696
;696:	if (TeamPlayIsOn() && BotSameTeam(bs, bs->lastkilledplayer)) {
ADDRLP4 56
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $421
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ARGI4
ADDRLP4 64
ADDRGP4 BotSameTeam
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
EQI4 $421
line 697
;697:		BotAI_BotInitialChat(bs, "kill_teammate", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $423
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 698
;698:		bs->chatto = CHAT_TEAM;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 1
ASGNI4
line 699
;699:	}
ADDRGP4 $422
JUMPV
LABELV $421
line 701
;700:	else
;701:	{
line 703
;702:		//don't chat in teamplay
;703:		if (TeamPlayIsOn()) {
ADDRLP4 68
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $424
line 704
;704:			trap_EA_Command(bs->client, "vtaunt");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 705
;705:			return qfalse;			// don't wait
CNSTI4 0
RETI4
ADDRGP4 $400
JUMPV
LABELV $424
line 708
;706:		}
;707:		//
;708:		if (bs->enemydeathtype == MOD_GAUNTLET) {
ADDRFP4 0
INDIRP4
CNSTI4 6004
ADDP4
INDIRI4
CNSTI4 2
NEI4 $426
line 709
;709:			BotAI_BotInitialChat(bs, "kill_gauntlet", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $428
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 710
;710:		}
ADDRGP4 $427
JUMPV
LABELV $426
line 711
;711:		else if (bs->enemydeathtype == MOD_RAILGUN) {
ADDRFP4 0
INDIRP4
CNSTI4 6004
ADDP4
INDIRI4
CNSTI4 10
NEI4 $429
line 712
;712:			BotAI_BotInitialChat(bs, "kill_rail", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $431
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 713
;713:		}
ADDRGP4 $430
JUMPV
LABELV $429
line 714
;714:		else if (bs->enemydeathtype == MOD_TELEFRAG) {
ADDRFP4 0
INDIRP4
CNSTI4 6004
ADDP4
INDIRI4
CNSTI4 18
NEI4 $432
line 715
;715:			BotAI_BotInitialChat(bs, "kill_telefrag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $434
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 716
;716:		}
ADDRGP4 $433
JUMPV
LABELV $432
line 722
;717:#ifdef MISSIONPACK
;718:		else if (bs->botdeathtype == MOD_KAMIKAZE && trap_BotNumInitialChats(bs->cs, "kill_kamikaze"))
;719:			BotAI_BotInitialChat(bs, "kill_kamikaze", name, NULL);
;720:#endif
;721:		//choose between insult and praise
;722:		else if (random() < trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_INSULT, 0, 1)) {
ADDRLP4 72
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 24
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 76
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 72
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 76
INDIRF4
GEF4 $435
line 723
;723:			BotAI_BotInitialChat(bs, "kill_insult", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $437
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 724
;724:		}
ADDRGP4 $436
JUMPV
LABELV $435
line 725
;725:		else {
line 726
;726:			BotAI_BotInitialChat(bs, "kill_praise", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $438
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 727
;727:		}
LABELV $436
LABELV $433
LABELV $430
LABELV $427
line 728
;728:	}
LABELV $422
line 729
;729:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 730
;730:	return qtrue;
CNSTI4 1
RETI4
LABELV $400
endproc BotChat_Kill 80 16
export BotChat_EnemySuicide
proc BotChat_EnemySuicide 56 16
line 738
;731:}
;732:
;733:/*
;734:==================
;735:BotChat_EnemySuicide
;736:==================
;737:*/
;738:int BotChat_EnemySuicide(bot_state_t *bs) {
line 742
;739:	char name[32];
;740:	float rnd;
;741:
;742:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $440
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $440
line 743
;743:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $443
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $443
line 744
;744:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 36
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 1
GTI4 $445
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $445
line 746
;745:	//
;746:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_KILL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 28
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 40
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 40
INDIRF4
ASGNF4
line 748
;747:	//don't chat in teamplay
;748:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 44
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $447
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $447
line 750
;749:	// don't chat in tournament mode
;750:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $449
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $449
line 752
;751:	//if fast chat is off
;752:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $451
line 753
;753:		if (random() > rnd) return qfalse;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 32
INDIRF4
LEF4 $454
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $454
line 754
;754:	}
LABELV $451
line 755
;755:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $456
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $456
line 757
;756:	//
;757:	if (BotVisibleEnemies(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotVisibleEnemies
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $458
CNSTI4 0
RETI4
ADDRGP4 $439
JUMPV
LABELV $458
line 759
;758:	//
;759:	if (bs->enemy >= 0) EasyClientName(bs->enemy, name, 32);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
LTI4 $460
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
ADDRGP4 $461
JUMPV
LABELV $460
line 760
;760:	else strcpy(name, "");
ADDRLP4 0
ARGP4
ADDRGP4 $462
ARGP4
ADDRGP4 strcpy
CALLP4
pop
LABELV $461
line 761
;761:	BotAI_BotInitialChat(bs, "enemy_suicide", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $463
ARGP4
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 762
;762:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 763
;763:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 764
;764:	return qtrue;
CNSTI4 1
RETI4
LABELV $439
endproc BotChat_EnemySuicide 56 16
export BotChat_HitTalking
proc BotChat_HitTalking 68 20
line 772
;765:}
;766:
;767:/*
;768:==================
;769:BotChat_HitTalking
;770:==================
;771:*/
;772:int BotChat_HitTalking(bot_state_t *bs) {
line 777
;773:	char name[32], *weap;
;774:	int lasthurt_client;
;775:	float rnd;
;776:
;777:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $465
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $465
line 778
;778:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $468
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $468
line 779
;779:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 44
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 1
GTI4 $470
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $470
line 780
;780:	lasthurt_client = g_entities[bs->client].client->lasthurt_client;
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ASGNI4
line 781
;781:	if (!lasthurt_client) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $473
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $473
line 782
;782:	if (lasthurt_client == bs->client) return qfalse;
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $475
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $475
line 784
;783:	//
;784:	if (lasthurt_client < 0 || lasthurt_client >= MAX_CLIENTS) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $479
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $477
LABELV $479
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $477
line 786
;785:	//
;786:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_HITTALKING, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 31
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 52
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 52
INDIRF4
ASGNF4
line 788
;787:	//don't chat in teamplay
;788:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 56
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $480
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $480
line 790
;789:	// don't chat in tournament mode
;790:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $482
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $482
line 792
;791:	//if fast chat is off
;792:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $484
line 793
;793:		if (random() > rnd * 0.5) return qfalse;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
LEF4 $487
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $487
line 794
;794:	}
LABELV $484
line 795
;795:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
NEI4 $489
CNSTI4 0
RETI4
ADDRGP4 $464
JUMPV
LABELV $489
line 797
;796:	//
;797:	ClientName(g_entities[bs->client].client->lasthurt_client, name, sizeof(name));
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 798
;798:	weap = BotWeaponNameForMeansOfDeath(g_entities[bs->client].client->lasthurt_client);
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ARGI4
ADDRLP4 64
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRLP4 36
ADDRLP4 64
INDIRP4
ASGNP4
line 800
;799:	//
;800:	BotAI_BotInitialChat(bs, "hit_talking", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $493
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 801
;801:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 802
;802:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 803
;803:	return qtrue;
CNSTI4 1
RETI4
LABELV $464
endproc BotChat_HitTalking 68 20
export BotChat_HitNoDeath
proc BotChat_HitNoDeath 216 20
line 811
;804:}
;805:
;806:/*
;807:==================
;808:BotChat_HitNoDeath
;809:==================
;810:*/
;811:int BotChat_HitNoDeath(bot_state_t *bs) {
line 817
;812:	char name[32], *weap;
;813:	float rnd;
;814:	int lasthurt_client;
;815:	aas_entityinfo_t entinfo;
;816:
;817:	lasthurt_client = g_entities[bs->client].client->lasthurt_client;
ADDRLP4 0
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ASGNI4
line 818
;818:	if (!lasthurt_client) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $496
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $496
line 819
;819:	if (lasthurt_client == bs->client) return qfalse;
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $498
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $498
line 821
;820:	//
;821:	if (lasthurt_client < 0 || lasthurt_client >= MAX_CLIENTS) return qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $502
ADDRLP4 0
INDIRI4
CNSTI4 64
LTI4 $500
LABELV $502
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $500
line 823
;822:	//
;823:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $503
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $503
line 824
;824:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $506
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $506
line 825
;825:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 188
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 1
GTI4 $508
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $508
line 826
;826:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_HITNODEATH, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 32
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 192
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 180
ADDRLP4 192
INDIRF4
ASGNF4
line 828
;827:	//don't chat in teamplay
;828:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 196
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 0
EQI4 $510
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $510
line 830
;829:	// don't chat in tournament mode
;830:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $512
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $512
line 832
;831:	//if fast chat is off
;832:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $514
line 833
;833:		if (random() > rnd * 0.5) return qfalse;
ADDRLP4 200
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
ADDRLP4 180
INDIRF4
MULF4
LEF4 $517
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $517
line 834
;834:	}
LABELV $514
line 835
;835:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 200
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 0
NEI4 $519
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $519
line 837
;836:	//
;837:	if (BotVisibleEnemies(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 204
ADDRGP4 BotVisibleEnemies
CALLI4
ASGNI4
ADDRLP4 204
INDIRI4
CNSTI4 0
EQI4 $521
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $521
line 839
;838:	//
;839:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 40
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 840
;840:	if (EntityIsShooting(&entinfo)) return qfalse;
ADDRLP4 40
ARGP4
ADDRLP4 208
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 208
INDIRI4
CNSTI4 0
EQI4 $523
CNSTI4 0
RETI4
ADDRGP4 $494
JUMPV
LABELV $523
line 842
;841:	//
;842:	ClientName(lasthurt_client, name, sizeof(name));
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 843
;843:	weap = BotWeaponNameForMeansOfDeath(g_entities[bs->client].client->lasthurt_mod);
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ARGI4
ADDRLP4 212
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRLP4 36
ADDRLP4 212
INDIRP4
ASGNP4
line 845
;844:	//
;845:	BotAI_BotInitialChat(bs, "hit_nodeath", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $526
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 846
;846:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 847
;847:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 848
;848:	return qtrue;
CNSTI4 1
RETI4
LABELV $494
endproc BotChat_HitNoDeath 216 20
export BotChat_HitNoKill
proc BotChat_HitNoKill 208 20
line 856
;849:}
;850:
;851:/*
;852:==================
;853:BotChat_HitNoKill
;854:==================
;855:*/
;856:int BotChat_HitNoKill(bot_state_t *bs) {
line 861
;857:	char name[32], *weap;
;858:	float rnd;
;859:	aas_entityinfo_t entinfo;
;860:
;861:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $528
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $528
line 862
;862:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $531
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $531
line 863
;863:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 180
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 1
GTI4 $533
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $533
line 864
;864:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_HITNOKILL, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 33
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 184
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 176
ADDRLP4 184
INDIRF4
ASGNF4
line 866
;865:	//don't chat in teamplay
;866:	if (TeamPlayIsOn()) return qfalse;
ADDRLP4 188
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 0
EQI4 $535
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $535
line 868
;867:	// don't chat in tournament mode
;868:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $537
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $537
line 870
;869:	//if fast chat is off
;870:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $539
line 871
;871:		if (random() > rnd * 0.5) return qfalse;
ADDRLP4 192
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
ADDRLP4 176
INDIRF4
MULF4
LEF4 $542
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $542
line 872
;872:	}
LABELV $539
line 873
;873:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 192
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 0
NEI4 $544
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $544
line 875
;874:	//
;875:	if (BotVisibleEnemies(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 196
ADDRGP4 BotVisibleEnemies
CALLI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 0
EQI4 $546
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $546
line 877
;876:	//
;877:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 36
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 878
;878:	if (EntityIsShooting(&entinfo)) return qfalse;
ADDRLP4 36
ARGP4
ADDRLP4 200
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 0
EQI4 $548
CNSTI4 0
RETI4
ADDRGP4 $527
JUMPV
LABELV $548
line 880
;879:	//
;880:	ClientName(bs->enemy, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 881
;881:	weap = BotWeaponNameForMeansOfDeath(g_entities[bs->enemy].client->lasthurt_mod);
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ARGI4
ADDRLP4 204
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRLP4 32
ADDRLP4 204
INDIRP4
ASGNP4
line 883
;882:	//
;883:	BotAI_BotInitialChat(bs, "hit_nokill", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $551
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 884
;884:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 885
;885:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 886
;886:	return qtrue;
CNSTI4 1
RETI4
LABELV $527
endproc BotChat_HitNoKill 208 20
export BotChat_Random
proc BotChat_Random 96 36
line 894
;887:}
;888:
;889:/*
;890:==================
;891:BotChat_Random
;892:==================
;893:*/
;894:int BotChat_Random(bot_state_t *bs) {
line 898
;895:	float rnd;
;896:	char name[32];
;897:
;898:	if (bot_nochat.integer) return qfalse;
ADDRGP4 bot_nochat+12
INDIRI4
CNSTI4 0
EQI4 $553
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $553
line 899
;899:	if (BotIsObserver(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $556
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $556
line 900
;900:	if (bs->lastchat_time > FloatTime() - TIME_BETWEENCHATTING) return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1103626240
SUBF4
LEF4 $558
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $558
line 902
;901:	// don't chat in tournament mode
;902:	if (gametype == GT_TOURNAMENT) return qfalse;
ADDRGP4 gametype
INDIRI4
CNSTI4 1
NEI4 $560
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $560
line 904
;903:	//don't chat when doing something important :)
;904:	if (bs->ltgtype == LTG_TEAMHELP ||
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 1
EQI4 $565
ADDRLP4 40
INDIRI4
CNSTI4 2
EQI4 $565
ADDRLP4 40
INDIRI4
CNSTI4 5
NEI4 $562
LABELV $565
line 906
;905:		bs->ltgtype == LTG_TEAMACCOMPANY ||
;906:		bs->ltgtype == LTG_RUSHBASE) return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $562
line 908
;907:	//
;908:	rnd = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_RANDOM, 0, 1);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 34
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 44
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 44
INDIRF4
ASGNF4
line 909
;909:	if (random() > bs->thinktime * 0.1) return qfalse;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1036831949
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
LEF4 $566
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $566
line 910
;910:	if (!bot_fastchat.integer) {
ADDRGP4 bot_fastchat+12
INDIRI4
CNSTI4 0
NEI4 $568
line 911
;911:		if (random() > rnd) return qfalse;
ADDRLP4 52
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 32
INDIRF4
LEF4 $571
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $571
line 912
;912:		if (random() > 0.25) return qfalse;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1048576000
LEF4 $573
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $573
line 913
;913:	}
LABELV $568
line 914
;914:	if (BotNumActivePlayers() <= 1) return qfalse;
ADDRLP4 52
ADDRGP4 BotNumActivePlayers
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 1
GTI4 $575
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $575
line 916
;915:	//
;916:	if (!BotValidChatPosition(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 BotValidChatPosition
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $577
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $577
line 918
;917:	//
;918:	if (BotVisibleEnemies(bs)) return qfalse;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 BotVisibleEnemies
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $579
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $579
line 920
;919:	//
;920:	if (bs->lastkilledplayer == bs->client) {
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ADDRLP4 64
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $581
line 921
;921:		strcpy(name, BotRandomOpponentName(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 922
;922:	}
ADDRGP4 $582
JUMPV
LABELV $581
line 923
;923:	else {
line 924
;924:		EasyClientName(bs->lastkilledplayer, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 925
;925:	}
LABELV $582
line 926
;926:	if (TeamPlayIsOn()) {
ADDRLP4 68
ADDRGP4 TeamPlayIsOn
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $583
line 927
;927:		trap_EA_Command(bs->client, "vtaunt");
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 $297
ARGP4
ADDRGP4 trap_EA_Command
CALLV
pop
line 928
;928:		return qfalse;			// don't wait
CNSTI4 0
RETI4
ADDRGP4 $552
JUMPV
LABELV $583
line 931
;929:	}
;930:	//
;931:	if (random() < trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CHAT_MISC, 0, 1)) {
ADDRLP4 72
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 25
ARGI4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 76
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 72
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 76
INDIRF4
GEF4 $585
line 932
;932:		BotAI_BotInitialChat(bs, "random_misc",
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 84
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRLP4 88
ADDRGP4 BotRandomWeaponName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $587
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 92
ADDRGP4 $268
ASGNP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 940
;933:					BotRandomOpponentName(bs),	// 0
;934:					name,						// 1
;935:					"[invalid var]",			// 2
;936:					"[invalid var]",			// 3
;937:					BotMapTitle(),				// 4
;938:					BotRandomWeaponName(),		// 5
;939:					NULL);
;940:	}
ADDRGP4 $586
JUMPV
LABELV $585
line 941
;941:	else {
line 942
;942:		BotAI_BotInitialChat(bs, "random_insult",
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 84
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRLP4 88
ADDRGP4 BotRandomWeaponName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $588
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 92
ADDRGP4 $268
ASGNP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 950
;943:					BotRandomOpponentName(bs),	// 0
;944:					name,						// 1
;945:					"[invalid var]",			// 2
;946:					"[invalid var]",			// 3
;947:					BotMapTitle(),				// 4
;948:					BotRandomWeaponName(),		// 5
;949:					NULL);
;950:	}
LABELV $586
line 951
;951:	bs->lastchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6100
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 952
;952:	bs->chatto = CHAT_ALL;
ADDRFP4 0
INDIRP4
CNSTI4 6052
ADDP4
CNSTI4 0
ASGNI4
line 953
;953:	return qtrue;
CNSTI4 1
RETI4
LABELV $552
endproc BotChat_Random 96 36
export BotChatTime
proc BotChatTime 8 16
line 961
;954:}
;955:
;956:/*
;957:==================
;958:BotChatTime
;959:==================
;960:*/
;961:float BotChatTime(bot_state_t *bs) {
line 964
;962:	int cpm;
;963:
;964:	cpm = trap_Characteristic_BInteger(bs->character, CHARACTERISTIC_CHAT_CPM, 1, 4000);
ADDRFP4 0
INDIRP4
CNSTI4 6520
ADDP4
INDIRI4
ARGI4
CNSTI4 23
ARGI4
CNSTI4 1
ARGI4
CNSTI4 4000
ARGI4
ADDRLP4 4
ADDRGP4 trap_Characteristic_BInteger
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 966
;965:
;966:	return 2.0;	//(float) trap_BotChatLength(bs->cs) * 30 / cpm;
CNSTF4 1073741824
RETF4
LABELV $589
endproc BotChatTime 8 16
export BotChatTest
proc BotChatTest 184 36
line 974
;967:}
;968:
;969:/*
;970:==================
;971:BotChatTest
;972:==================
;973:*/
;974:void BotChatTest(bot_state_t *bs) {
line 980
;975:
;976:	char name[32];
;977:	char *weap;
;978:	int num, i;
;979:
;980:	num = trap_BotNumInitialChats(bs->cs, "game_enter");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $267
ARGP4
ADDRLP4 44
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 44
INDIRI4
ASGNI4
line 981
;981:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $594
JUMPV
LABELV $591
line 982
;982:	{
line 983
;983:		BotAI_BotInitialChat(bs, "game_enter",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 48
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 56
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $267
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 $268
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 990
;984:					EasyClientName(bs->client, name, 32),	// 0
;985:					BotRandomOpponentName(bs),				// 1
;986:					"[invalid var]",						// 2
;987:					"[invalid var]",						// 3
;988:					BotMapTitle(),							// 4
;989:					NULL);
;990:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 64
CNSTI4 0
ASGNI4
ADDRLP4 64
INDIRI4
ARGI4
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 991
;991:	}
LABELV $592
line 981
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $594
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $591
line 992
;992:	num = trap_BotNumInitialChats(bs->cs, "game_exit");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $286
ARGP4
ADDRLP4 48
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 48
INDIRI4
ASGNI4
line 993
;993:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $598
JUMPV
LABELV $595
line 994
;994:	{
line 995
;995:		BotAI_BotInitialChat(bs, "game_exit",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 52
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 60
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 $268
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1002
;996:					EasyClientName(bs->client, name, 32),	// 0
;997:					BotRandomOpponentName(bs),				// 1
;998:					"[invalid var]",						// 2
;999:					"[invalid var]",						// 3
;1000:					BotMapTitle(),							// 4
;1001:					NULL);
;1002:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
CNSTI4 0
ASGNI4
ADDRLP4 68
INDIRI4
ARGI4
ADDRLP4 68
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1003
;1003:	}
LABELV $596
line 993
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $598
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $595
line 1004
;1004:	num = trap_BotNumInitialChats(bs->cs, "level_start");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $307
ARGP4
ADDRLP4 52
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 52
INDIRI4
ASGNI4
line 1005
;1005:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $602
JUMPV
LABELV $599
line 1006
;1006:	{
line 1007
;1007:		BotAI_BotInitialChat(bs, "level_start",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 56
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $307
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1010
;1008:					EasyClientName(bs->client, name, 32),	// 0
;1009:					NULL);
;1010:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
CNSTI4 0
ASGNI4
ADDRLP4 60
INDIRI4
ARGI4
ADDRLP4 60
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1011
;1011:	}
LABELV $600
line 1005
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $602
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $599
line 1012
;1012:	num = trap_BotNumInitialChats(bs->cs, "level_end_victory");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $331
ARGP4
ADDRLP4 56
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 56
INDIRI4
ASGNI4
line 1013
;1013:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $606
JUMPV
LABELV $603
line 1014
;1014:	{
line 1015
;1015:		BotAI_BotInitialChat(bs, "level_end_victory",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 60
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 68
ADDRGP4 BotFirstClientInRankings
CALLP4
ASGNP4
ADDRLP4 72
ADDRGP4 BotLastClientInRankings
CALLP4
ASGNP4
ADDRLP4 76
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $331
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1022
;1016:				EasyClientName(bs->client, name, 32),	// 0
;1017:				BotRandomOpponentName(bs),				// 1
;1018:				BotFirstClientInRankings(),				// 2
;1019:				BotLastClientInRankings(),				// 3
;1020:				BotMapTitle(),							// 4
;1021:				NULL);
;1022:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1023
;1023:	}
LABELV $604
line 1013
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $606
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $603
line 1024
;1024:	num = trap_BotNumInitialChats(bs->cs, "level_end_lose");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $334
ARGP4
ADDRLP4 60
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 60
INDIRI4
ASGNI4
line 1025
;1025:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $610
JUMPV
LABELV $607
line 1026
;1026:	{
line 1027
;1027:		BotAI_BotInitialChat(bs, "level_end_lose",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 64
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 72
ADDRGP4 BotFirstClientInRankings
CALLP4
ASGNP4
ADDRLP4 76
ADDRGP4 BotLastClientInRankings
CALLP4
ASGNP4
ADDRLP4 80
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $334
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1034
;1028:				EasyClientName(bs->client, name, 32),	// 0
;1029:				BotRandomOpponentName(bs),				// 1
;1030:				BotFirstClientInRankings(),				// 2
;1031:				BotLastClientInRankings(),				// 3
;1032:				BotMapTitle(),							// 4
;1033:				NULL);
;1034:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 84
CNSTI4 0
ASGNI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1035
;1035:	}
LABELV $608
line 1025
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $610
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $607
line 1036
;1036:	num = trap_BotNumInitialChats(bs->cs, "level_end");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $335
ARGP4
ADDRLP4 64
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 64
INDIRI4
ASGNI4
line 1037
;1037:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $614
JUMPV
LABELV $611
line 1038
;1038:	{
line 1039
;1039:		BotAI_BotInitialChat(bs, "level_end",
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 68
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 76
ADDRGP4 BotFirstClientInRankings
CALLP4
ASGNP4
ADDRLP4 80
ADDRGP4 BotLastClientInRankings
CALLP4
ASGNP4
ADDRLP4 84
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $335
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1046
;1040:				EasyClientName(bs->client, name, 32),	// 0
;1041:				BotRandomOpponentName(bs),				// 1
;1042:				BotFirstClientInRankings(),				// 2
;1043:				BotLastClientInRankings(),				// 3
;1044:				BotMapTitle(),							// 4
;1045:				NULL);
;1046:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 88
CNSTI4 0
ASGNI4
ADDRLP4 88
INDIRI4
ARGI4
ADDRLP4 88
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1047
;1047:	}
LABELV $612
line 1037
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $614
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $611
line 1048
;1048:	EasyClientName(bs->lastkilledby, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 5996
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1049
;1049:	num = trap_BotNumInitialChats(bs->cs, "death_drown");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $363
ARGP4
ADDRLP4 68
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 68
INDIRI4
ASGNI4
line 1050
;1050:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $618
JUMPV
LABELV $615
line 1051
;1051:	{
line 1053
;1052:		//
;1053:		BotAI_BotInitialChat(bs, "death_drown", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $363
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1054
;1054:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRI4
ARGI4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1055
;1055:	}
LABELV $616
line 1050
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $618
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $615
line 1056
;1056:	num = trap_BotNumInitialChats(bs->cs, "death_slime");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $366
ARGP4
ADDRLP4 72
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 72
INDIRI4
ASGNI4
line 1057
;1057:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $622
JUMPV
LABELV $619
line 1058
;1058:	{
line 1059
;1059:		BotAI_BotInitialChat(bs, "death_slime", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $366
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1060
;1060:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRLP4 76
INDIRI4
ARGI4
ADDRLP4 76
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1061
;1061:	}
LABELV $620
line 1057
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $622
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $619
line 1062
;1062:	num = trap_BotNumInitialChats(bs->cs, "death_lava");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $369
ARGP4
ADDRLP4 76
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 76
INDIRI4
ASGNI4
line 1063
;1063:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $626
JUMPV
LABELV $623
line 1064
;1064:	{
line 1065
;1065:		BotAI_BotInitialChat(bs, "death_lava", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $369
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1066
;1066:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRLP4 80
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1067
;1067:	}
LABELV $624
line 1063
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $626
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $623
line 1068
;1068:	num = trap_BotNumInitialChats(bs->cs, "death_cratered");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $372
ARGP4
ADDRLP4 80
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 80
INDIRI4
ASGNI4
line 1069
;1069:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $630
JUMPV
LABELV $627
line 1070
;1070:	{
line 1071
;1071:		BotAI_BotInitialChat(bs, "death_cratered", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $372
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1072
;1072:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 84
CNSTI4 0
ASGNI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1073
;1073:	}
LABELV $628
line 1069
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $630
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $627
line 1074
;1074:	num = trap_BotNumInitialChats(bs->cs, "death_suicide");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $380
ARGP4
ADDRLP4 84
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 84
INDIRI4
ASGNI4
line 1075
;1075:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $634
JUMPV
LABELV $631
line 1076
;1076:	{
line 1077
;1077:		BotAI_BotInitialChat(bs, "death_suicide", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1078
;1078:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 88
CNSTI4 0
ASGNI4
ADDRLP4 88
INDIRI4
ARGI4
ADDRLP4 88
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1079
;1079:	}
LABELV $632
line 1075
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $634
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $631
line 1080
;1080:	num = trap_BotNumInitialChats(bs->cs, "death_telefrag");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $383
ARGP4
ADDRLP4 88
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 88
INDIRI4
ASGNI4
line 1081
;1081:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $638
JUMPV
LABELV $635
line 1082
;1082:	{
line 1083
;1083:		BotAI_BotInitialChat(bs, "death_telefrag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $383
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1084
;1084:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 92
CNSTI4 0
ASGNI4
ADDRLP4 92
INDIRI4
ARGI4
ADDRLP4 92
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1085
;1085:	}
LABELV $636
line 1081
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $638
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $635
line 1086
;1086:	num = trap_BotNumInitialChats(bs->cs, "death_gauntlet");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $391
ARGP4
ADDRLP4 92
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 92
INDIRI4
ASGNI4
line 1087
;1087:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $642
JUMPV
LABELV $639
line 1088
;1088:	{
line 1089
;1089:		BotAI_BotInitialChat(bs, "death_gauntlet",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 96
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $391
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 96
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1093
;1090:				name,												// 0
;1091:				BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;1092:				NULL);
;1093:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 100
CNSTI4 0
ASGNI4
ADDRLP4 100
INDIRI4
ARGI4
ADDRLP4 100
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1094
;1094:	}
LABELV $640
line 1087
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $642
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $639
line 1095
;1095:	num = trap_BotNumInitialChats(bs->cs, "death_rail");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $394
ARGP4
ADDRLP4 96
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 96
INDIRI4
ASGNI4
line 1096
;1096:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $646
JUMPV
LABELV $643
line 1097
;1097:	{
line 1098
;1098:		BotAI_BotInitialChat(bs, "death_rail",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 100
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $394
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 100
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1102
;1099:				name,												// 0
;1100:				BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;1101:				NULL);
;1102:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 104
CNSTI4 0
ASGNI4
ADDRLP4 104
INDIRI4
ARGI4
ADDRLP4 104
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1103
;1103:	}
LABELV $644
line 1096
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $646
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $643
line 1104
;1104:	num = trap_BotNumInitialChats(bs->cs, "death_bfg");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $395
ARGP4
ADDRLP4 100
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 100
INDIRI4
ASGNI4
line 1105
;1105:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $650
JUMPV
LABELV $647
line 1106
;1106:	{
line 1107
;1107:		BotAI_BotInitialChat(bs, "death_bfg",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 104
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $395
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1111
;1108:				name,												// 0
;1109:				BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;1110:				NULL);
;1111:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
CNSTI4 0
ASGNI4
ADDRLP4 108
INDIRI4
ARGI4
ADDRLP4 108
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1112
;1112:	}
LABELV $648
line 1105
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $650
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $647
line 1113
;1113:	num = trap_BotNumInitialChats(bs->cs, "death_insult");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $398
ARGP4
ADDRLP4 104
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 104
INDIRI4
ASGNI4
line 1114
;1114:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $654
JUMPV
LABELV $651
line 1115
;1115:	{
line 1116
;1116:		BotAI_BotInitialChat(bs, "death_insult",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $398
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1120
;1117:					name,												// 0
;1118:					BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;1119:					NULL);
;1120:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
CNSTI4 0
ASGNI4
ADDRLP4 112
INDIRI4
ARGI4
ADDRLP4 112
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1121
;1121:	}
LABELV $652
line 1114
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $654
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $651
line 1122
;1122:	num = trap_BotNumInitialChats(bs->cs, "death_praise");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $399
ARGP4
ADDRLP4 108
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 108
INDIRI4
ASGNI4
line 1123
;1123:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $658
JUMPV
LABELV $655
line 1124
;1124:	{
line 1125
;1125:		BotAI_BotInitialChat(bs, "death_praise",
ADDRFP4 0
INDIRP4
CNSTI4 6000
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $399
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1129
;1126:					name,												// 0
;1127:					BotWeaponNameForMeansOfDeath(bs->botdeathtype),		// 1
;1128:					NULL);
;1129:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 116
CNSTI4 0
ASGNI4
ADDRLP4 116
INDIRI4
ARGI4
ADDRLP4 116
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1130
;1130:	}
LABELV $656
line 1123
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $658
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $655
line 1132
;1131:	//
;1132:	EasyClientName(bs->lastkilledplayer, name, 32);
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1134
;1133:	//
;1134:	num = trap_BotNumInitialChats(bs->cs, "kill_gauntlet");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $428
ARGP4
ADDRLP4 112
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 112
INDIRI4
ASGNI4
line 1135
;1135:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $662
JUMPV
LABELV $659
line 1136
;1136:	{
line 1138
;1137:		//
;1138:		BotAI_BotInitialChat(bs, "kill_gauntlet", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $428
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1139
;1139:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 116
CNSTI4 0
ASGNI4
ADDRLP4 116
INDIRI4
ARGI4
ADDRLP4 116
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1140
;1140:	}
LABELV $660
line 1135
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $662
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $659
line 1141
;1141:	num = trap_BotNumInitialChats(bs->cs, "kill_rail");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $431
ARGP4
ADDRLP4 116
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 116
INDIRI4
ASGNI4
line 1142
;1142:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $666
JUMPV
LABELV $663
line 1143
;1143:	{
line 1144
;1144:		BotAI_BotInitialChat(bs, "kill_rail", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $431
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1145
;1145:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 120
CNSTI4 0
ASGNI4
ADDRLP4 120
INDIRI4
ARGI4
ADDRLP4 120
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1146
;1146:	}
LABELV $664
line 1142
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $666
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $663
line 1147
;1147:	num = trap_BotNumInitialChats(bs->cs, "kill_telefrag");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $434
ARGP4
ADDRLP4 120
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 120
INDIRI4
ASGNI4
line 1148
;1148:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $670
JUMPV
LABELV $667
line 1149
;1149:	{
line 1150
;1150:		BotAI_BotInitialChat(bs, "kill_telefrag", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $434
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1151
;1151:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 124
CNSTI4 0
ASGNI4
ADDRLP4 124
INDIRI4
ARGI4
ADDRLP4 124
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1152
;1152:	}
LABELV $668
line 1148
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $670
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $667
line 1153
;1153:	num = trap_BotNumInitialChats(bs->cs, "kill_insult");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $437
ARGP4
ADDRLP4 124
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 124
INDIRI4
ASGNI4
line 1154
;1154:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $674
JUMPV
LABELV $671
line 1155
;1155:	{
line 1156
;1156:		BotAI_BotInitialChat(bs, "kill_insult", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $437
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1157
;1157:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 128
CNSTI4 0
ASGNI4
ADDRLP4 128
INDIRI4
ARGI4
ADDRLP4 128
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1158
;1158:	}
LABELV $672
line 1154
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $674
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $671
line 1159
;1159:	num = trap_BotNumInitialChats(bs->cs, "kill_praise");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $438
ARGP4
ADDRLP4 128
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 128
INDIRI4
ASGNI4
line 1160
;1160:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $678
JUMPV
LABELV $675
line 1161
;1161:	{
line 1162
;1162:		BotAI_BotInitialChat(bs, "kill_praise", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $438
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1163
;1163:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRLP4 132
INDIRI4
ARGI4
ADDRLP4 132
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1164
;1164:	}
LABELV $676
line 1160
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $678
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $675
line 1165
;1165:	num = trap_BotNumInitialChats(bs->cs, "enemy_suicide");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $463
ARGP4
ADDRLP4 132
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 132
INDIRI4
ASGNI4
line 1166
;1166:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $682
JUMPV
LABELV $679
line 1167
;1167:	{
line 1168
;1168:		BotAI_BotInitialChat(bs, "enemy_suicide", name, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $463
ARGP4
ADDRLP4 8
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1169
;1169:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 136
CNSTI4 0
ASGNI4
ADDRLP4 136
INDIRI4
ARGI4
ADDRLP4 136
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1170
;1170:	}
LABELV $680
line 1166
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $682
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $679
line 1171
;1171:	ClientName(g_entities[bs->client].client->lasthurt_client, name, sizeof(name));
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 ClientName
CALLP4
pop
line 1172
;1172:	weap = BotWeaponNameForMeansOfDeath(g_entities[bs->client].client->lasthurt_client);
CNSTI4 808
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+516
ADDP4
INDIRP4
CNSTI4 724
ADDP4
INDIRI4
ARGI4
ADDRLP4 136
ADDRGP4 BotWeaponNameForMeansOfDeath
CALLP4
ASGNP4
ADDRLP4 40
ADDRLP4 136
INDIRP4
ASGNP4
line 1173
;1173:	num = trap_BotNumInitialChats(bs->cs, "hit_talking");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $493
ARGP4
ADDRLP4 140
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 140
INDIRI4
ASGNI4
line 1174
;1174:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $688
JUMPV
LABELV $685
line 1175
;1175:	{
line 1176
;1176:		BotAI_BotInitialChat(bs, "hit_talking", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $493
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1177
;1177:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 144
CNSTI4 0
ASGNI4
ADDRLP4 144
INDIRI4
ARGI4
ADDRLP4 144
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1178
;1178:	}
LABELV $686
line 1174
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $688
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $685
line 1179
;1179:	num = trap_BotNumInitialChats(bs->cs, "hit_nodeath");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $526
ARGP4
ADDRLP4 144
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 144
INDIRI4
ASGNI4
line 1180
;1180:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $692
JUMPV
LABELV $689
line 1181
;1181:	{
line 1182
;1182:		BotAI_BotInitialChat(bs, "hit_nodeath", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $526
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1183
;1183:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 148
CNSTI4 0
ASGNI4
ADDRLP4 148
INDIRI4
ARGI4
ADDRLP4 148
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1184
;1184:	}
LABELV $690
line 1180
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $692
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $689
line 1185
;1185:	num = trap_BotNumInitialChats(bs->cs, "hit_nokill");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $551
ARGP4
ADDRLP4 148
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 148
INDIRI4
ASGNI4
line 1186
;1186:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $696
JUMPV
LABELV $693
line 1187
;1187:	{
line 1188
;1188:		BotAI_BotInitialChat(bs, "hit_nokill", name, weap, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $551
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1189
;1189:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
CNSTI4 0
ASGNI4
ADDRLP4 152
INDIRI4
ARGI4
ADDRLP4 152
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1190
;1190:	}
LABELV $694
line 1186
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $696
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $693
line 1192
;1191:	//
;1192:	if (bs->lastkilledplayer == bs->client) {
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 152
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ADDRLP4 152
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
NEI4 $697
line 1193
;1193:		strcpy(name, BotRandomOpponentName(bs));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 8
ARGP4
ADDRLP4 156
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1194
;1194:	}
ADDRGP4 $698
JUMPV
LABELV $697
line 1195
;1195:	else {
line 1196
;1196:		EasyClientName(bs->lastkilledplayer, name, sizeof(name));
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 1197
;1197:	}
LABELV $698
line 1199
;1198:	//
;1199:	num = trap_BotNumInitialChats(bs->cs, "random_misc");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $587
ARGP4
ADDRLP4 156
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 156
INDIRI4
ASGNI4
line 1200
;1200:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $702
JUMPV
LABELV $699
line 1201
;1201:	{
line 1203
;1202:		//
;1203:		BotAI_BotInitialChat(bs, "random_misc",
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 160
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 164
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRLP4 168
ADDRGP4 BotRandomWeaponName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $587
ARGP4
ADDRLP4 160
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 172
ADDRGP4 $268
ASGNP4
ADDRLP4 172
INDIRP4
ARGP4
ADDRLP4 172
INDIRP4
ARGP4
ADDRLP4 164
INDIRP4
ARGP4
ADDRLP4 168
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1211
;1204:					BotRandomOpponentName(bs),	// 0
;1205:					name,						// 1
;1206:					"[invalid var]",			// 2
;1207:					"[invalid var]",			// 3
;1208:					BotMapTitle(),				// 4
;1209:					BotRandomWeaponName(),		// 5
;1210:					NULL);
;1211:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 176
CNSTI4 0
ASGNI4
ADDRLP4 176
INDIRI4
ARGI4
ADDRLP4 176
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1212
;1212:	}
LABELV $700
line 1200
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $702
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $699
line 1213
;1213:	num = trap_BotNumInitialChats(bs->cs, "random_insult");
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRGP4 $588
ARGP4
ADDRLP4 160
ADDRGP4 trap_BotNumInitialChats
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 160
INDIRI4
ASGNI4
line 1214
;1214:	for (i = 0; i < num; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $706
JUMPV
LABELV $703
line 1215
;1215:	{
line 1216
;1216:		BotAI_BotInitialChat(bs, "random_insult",
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 164
ADDRGP4 BotRandomOpponentName
CALLP4
ASGNP4
ADDRLP4 168
ADDRGP4 BotMapTitle
CALLP4
ASGNP4
ADDRLP4 172
ADDRGP4 BotRandomWeaponName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $588
ARGP4
ADDRLP4 164
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 176
ADDRGP4 $268
ASGNP4
ADDRLP4 176
INDIRP4
ARGP4
ADDRLP4 176
INDIRP4
ARGP4
ADDRLP4 168
INDIRP4
ARGP4
ADDRLP4 172
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1224
;1217:					BotRandomOpponentName(bs),	// 0
;1218:					name,						// 1
;1219:					"[invalid var]",			// 2
;1220:					"[invalid var]",			// 3
;1221:					BotMapTitle(),				// 4
;1222:					BotRandomWeaponName(),		// 5
;1223:					NULL);
;1224:		trap_BotEnterChat(bs->cs, 0, CHAT_ALL);
ADDRFP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 180
CNSTI4 0
ASGNI4
ADDRLP4 180
INDIRI4
ARGI4
ADDRLP4 180
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1225
;1225:	}
LABELV $704
line 1214
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $706
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $703
line 1226
;1226:}
LABELV $590
endproc BotChatTest 184 36
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
LABELV $588
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 95
byte 1 105
byte 1 110
byte 1 115
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $587
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 95
byte 1 109
byte 1 105
byte 1 115
byte 1 99
byte 1 0
align 1
LABELV $551
byte 1 104
byte 1 105
byte 1 116
byte 1 95
byte 1 110
byte 1 111
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $526
byte 1 104
byte 1 105
byte 1 116
byte 1 95
byte 1 110
byte 1 111
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $493
byte 1 104
byte 1 105
byte 1 116
byte 1 95
byte 1 116
byte 1 97
byte 1 108
byte 1 107
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $463
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 95
byte 1 115
byte 1 117
byte 1 105
byte 1 99
byte 1 105
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $462
byte 1 0
align 1
LABELV $438
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 112
byte 1 114
byte 1 97
byte 1 105
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $437
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 105
byte 1 110
byte 1 115
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $434
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $431
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 114
byte 1 97
byte 1 105
byte 1 108
byte 1 0
align 1
LABELV $428
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 103
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $423
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 109
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $399
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 112
byte 1 114
byte 1 97
byte 1 105
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $398
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 105
byte 1 110
byte 1 115
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $395
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 98
byte 1 102
byte 1 103
byte 1 0
align 1
LABELV $394
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 114
byte 1 97
byte 1 105
byte 1 108
byte 1 0
align 1
LABELV $391
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 103
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $383
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 116
byte 1 101
byte 1 108
byte 1 101
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $380
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 115
byte 1 117
byte 1 105
byte 1 99
byte 1 105
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $372
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 99
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $369
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 108
byte 1 97
byte 1 118
byte 1 97
byte 1 0
align 1
LABELV $366
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 115
byte 1 108
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $363
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 100
byte 1 114
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $358
byte 1 100
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 109
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $353
byte 1 91
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 93
byte 1 0
align 1
LABELV $335
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 101
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $334
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 101
byte 1 110
byte 1 100
byte 1 95
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $331
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 101
byte 1 110
byte 1 100
byte 1 95
byte 1 118
byte 1 105
byte 1 99
byte 1 116
byte 1 111
byte 1 114
byte 1 121
byte 1 0
align 1
LABELV $307
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $297
byte 1 118
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $286
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 95
byte 1 101
byte 1 120
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $268
byte 1 91
byte 1 105
byte 1 110
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 32
byte 1 118
byte 1 97
byte 1 114
byte 1 93
byte 1 0
align 1
LABELV $267
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 95
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $194
byte 1 91
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 93
byte 1 0
align 1
LABELV $193
byte 1 71
byte 1 114
byte 1 97
byte 1 112
byte 1 112
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $191
byte 1 66
byte 1 70
byte 1 71
byte 1 49
byte 1 48
byte 1 75
byte 1 0
align 1
LABELV $189
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
LABELV $187
byte 1 82
byte 1 97
byte 1 105
byte 1 108
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $185
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
LABELV $183
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
LABELV $181
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
LABELV $179
byte 1 77
byte 1 97
byte 1 99
byte 1 104
byte 1 105
byte 1 110
byte 1 101
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $177
byte 1 71
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $175
byte 1 83
byte 1 104
byte 1 111
byte 1 116
byte 1 103
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $169
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $68
byte 1 116
byte 1 0
align 1
LABELV $64
byte 1 110
byte 1 0
align 1
LABELV $57
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
