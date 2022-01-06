export BotResetNodeSwitches
code
proc BotResetNodeSwitches 0 0
file "../ai_dmnet.c"
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
;25: * name:		ai_dmnet.c
;26: *
;27: * desc:		Quake3 bot AI
;28: *
;29: * $Archive: /MissionPack/code/game/ai_dmnet.c $
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
;50://data file headers
;51:#include "chars.h"			//characteristics
;52:#include "inv.h"			//indexes into the inventory
;53:#include "syn.h"			//synonyms
;54:#include "match.h"			//string matching types and vars
;55:
;56:// for the voice chats
;57:// #include "../../ui/menudef.h"
;58:#include "../ui/menudef.h"
;59:
;60://goal flag, see be_ai_goal.h for the other GFL_*
;61:#define GFL_AIR			128
;62:
;63:int numnodeswitches;
;64:char nodeswitch[MAX_NODESWITCHES+1][144];
;65:
;66:#define LOOKAHEAD_DISTANCE			300
;67:
;68:/*
;69:==================
;70:BotResetNodeSwitches
;71:==================
;72:*/
;73:void BotResetNodeSwitches(void) {
line 74
;74:	numnodeswitches = 0;
ADDRGP4 numnodeswitches
CNSTI4 0
ASGNI4
line 75
;75:}
LABELV $53
endproc BotResetNodeSwitches 0 0
export BotDumpNodeSwitches
proc BotDumpNodeSwitches 40 20
line 82
;76:
;77:/*
;78:==================
;79:BotDumpNodeSwitches
;80:==================
;81:*/
;82:void BotDumpNodeSwitches(bot_state_t *bs) {
line 86
;83:	int i;
;84:	char netname[MAX_NETNAME];
;85:
;86:	ClientName(bs->client, netname, sizeof(netname));
ADDRFP4 0
INDIRP4
CNSTI4 8
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
line 87
;87:	BotAI_Print(PRT_MESSAGE, "%s at %1.1f switched more than %d AI nodes\n", netname, FloatTime(), MAX_NODESWITCHES);
CNSTI4 1
ARGI4
ADDRGP4 $55
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
line 88
;88:	for (i = 0; i < numnodeswitches; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $59
JUMPV
LABELV $56
line 89
;89:		BotAI_Print(PRT_MESSAGE, nodeswitch[i]);
CNSTI4 1
ARGI4
CNSTI4 144
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 nodeswitch
ADDP4
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 90
;90:	}
LABELV $57
line 88
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $59
ADDRLP4 0
INDIRI4
ADDRGP4 numnodeswitches
INDIRI4
LTI4 $56
line 91
;91:	BotAI_Print(PRT_FATAL, "");
CNSTI4 4
ARGI4
ADDRGP4 $60
ARGP4
ADDRGP4 BotAI_Print
CALLV
pop
line 92
;92:}
LABELV $54
endproc BotDumpNodeSwitches 40 20
export BotRecordNodeSwitch
proc BotRecordNodeSwitch 44 32
line 99
;93:
;94:/*
;95:==================
;96:BotRecordNodeSwitch
;97:==================
;98:*/
;99:void BotRecordNodeSwitch(bot_state_t *bs, char *node, char *str, char *s) {
line 102
;100:	char netname[MAX_NETNAME];
;101:
;102:	ClientName(bs->client, netname, sizeof(netname));
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
line 103
;103:	Com_sprintf(nodeswitch[numnodeswitches], 144, "%s at %2.1f entered %s: %s from %s\n", netname, FloatTime(), node, str, s);
ADDRLP4 36
CNSTI4 144
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 numnodeswitches
INDIRI4
MULI4
ADDRGP4 nodeswitch
ADDP4
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRGP4 $62
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 floattime
INDIRF4
ARGF4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 109
;104:#ifdef DEBUG
;105:	if (0) {
;106:		BotAI_Print(PRT_MESSAGE, nodeswitch[numnodeswitches]);
;107:	}
;108:#endif //DEBUG
;109:	numnodeswitches++;
ADDRLP4 40
ADDRGP4 numnodeswitches
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 110
;110:}
LABELV $61
endproc BotRecordNodeSwitch 44 32
lit
align 4
LABELV $64
byte 4 3245342720
byte 4 3245342720
byte 4 3221225472
align 4
LABELV $65
byte 4 1097859072
byte 4 1097859072
byte 4 1073741824
export BotGetAirGoal
code
proc BotGetAirGoal 140 28
line 117
;111:
;112:/*
;113:==================
;114:BotGetAirGoal
;115:==================
;116:*/
;117:int BotGetAirGoal(bot_state_t *bs, bot_goal_t *goal) {
line 119
;118:	bsp_trace_t bsptrace;
;119:	vec3_t end, mins = {-15, -15, -2}, maxs = {15, 15, 2};
ADDRLP4 96
ADDRGP4 $64
INDIRB
ASGNB 12
ADDRLP4 108
ADDRGP4 $65
INDIRB
ASGNB 12
line 123
;120:	int areanum;
;121:
;122:	//trace up until we hit solid
;123:	VectorCopy(bs->origin, end);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRB
ASGNB 12
line 124
;124:	end[2] += 1000;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1148846080
ADDF4
ASGNF4
line 125
;125:	BotAI_Trace(&bsptrace, bs->origin, mins, maxs, end, bs->entitynum, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
ADDRLP4 12
ARGP4
ADDRLP4 124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 96
ARGP4
ADDRLP4 108
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 124
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 65537
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 127
;126:	//trace down until we hit water
;127:	VectorCopy(bsptrace.endpos, end);
ADDRLP4 0
ADDRLP4 12+12
INDIRB
ASGNB 12
line 128
;128:	BotAI_Trace(&bsptrace, end, mins, maxs, bs->origin, bs->entitynum, CONTENTS_WATER|CONTENTS_SLIME|CONTENTS_LAVA);
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 96
ARGP4
ADDRLP4 108
ARGP4
ADDRLP4 128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 128
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 128
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 56
ARGI4
ADDRGP4 BotAI_Trace
CALLV
pop
line 130
;129:	//if we found the water surface
;130:	if (bsptrace.fraction > 0) {
ADDRLP4 12+8
INDIRF4
CNSTF4 0
LEF4 $68
line 131
;131:		areanum = BotPointAreaNum(bsptrace.endpos);
ADDRLP4 12+12
ARGP4
ADDRLP4 132
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 120
ADDRLP4 132
INDIRI4
ASGNI4
line 132
;132:		if (areanum) {
ADDRLP4 120
INDIRI4
CNSTI4 0
EQI4 $72
line 133
;133:			VectorCopy(bsptrace.endpos, goal->origin);
ADDRFP4 4
INDIRP4
ADDRLP4 12+12
INDIRB
ASGNB 12
line 134
;134:			goal->origin[2] -= 2;
ADDRLP4 136
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 136
INDIRP4
ADDRLP4 136
INDIRP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 135
;135:			goal->areanum = areanum;
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 120
INDIRI4
ASGNI4
line 136
;136:			goal->mins[0] = -15;
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
CNSTF4 3245342720
ASGNF4
line 137
;137:			goal->mins[1] = -15;
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
CNSTF4 3245342720
ASGNF4
line 138
;138:			goal->mins[2] = -1;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTF4 3212836864
ASGNF4
line 139
;139:			goal->maxs[0] = 15;
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
CNSTF4 1097859072
ASGNF4
line 140
;140:			goal->maxs[1] = 15;
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1097859072
ASGNF4
line 141
;141:			goal->maxs[2] = 1;
ADDRFP4 4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1065353216
ASGNF4
line 142
;142:			goal->flags = GFL_AIR;
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 128
ASGNI4
line 143
;143:			goal->number = 0;
ADDRFP4 4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 0
ASGNI4
line 144
;144:			goal->iteminfo = 0;
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
CNSTI4 0
ASGNI4
line 145
;145:			goal->entitynum = 0;
ADDRFP4 4
INDIRP4
CNSTI4 40
ADDP4
CNSTI4 0
ASGNI4
line 146
;146:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $63
JUMPV
LABELV $72
line 148
;147:		}
;148:	}
LABELV $68
line 149
;149:	return qfalse;
CNSTI4 0
RETI4
LABELV $63
endproc BotGetAirGoal 140 28
export BotGoForAir
proc BotGoForAir 68 24
line 157
;150:}
;151:
;152:/*
;153:==================
;154:BotGoForAir
;155:==================
;156:*/
;157:int BotGoForAir(bot_state_t *bs, int tfl, bot_goal_t *ltg, float range) {
line 161
;158:	bot_goal_t goal;
;159:
;160:	//if the bot needs air
;161:	if (bs->lastair_time < FloatTime() - 6) {
ADDRFP4 0
INDIRP4
CNSTI4 6176
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1086324736
SUBF4
GEF4 $76
line 167
;162:		//
;163:#ifdef DEBUG
;164:		//BotAI_Print(PRT_MESSAGE, "going for air\n");
;165:#endif //DEBUG
;166:		//if we can find an air goal
;167:		if (BotGetAirGoal(bs, &goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 56
ADDRGP4 BotGetAirGoal
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $81
line 168
;168:			trap_BotPushGoal(bs->gs, &goal);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotPushGoal
CALLV
pop
line 169
;169:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $75
JUMPV
line 171
;170:		}
;171:		else {
LABELV $80
line 173
;172:			//get a nearby goal outside the water
;173:			while(trap_BotChooseNBGItem(bs->gs, bs->origin, bs->inventory, tfl, ltg, range)) {
line 174
;174:				trap_BotGetTopGoal(bs->gs, &goal);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_BotGetTopGoal
CALLI4
pop
line 176
;175:				//if the goal is not in water
;176:				if (!(trap_AAS_PointContents(goal.origin) & (CONTENTS_WATER|CONTENTS_SLIME|CONTENTS_LAVA))) {
ADDRLP4 0
ARGP4
ADDRLP4 60
ADDRGP4 trap_AAS_PointContents
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
NEI4 $83
line 177
;177:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $75
JUMPV
LABELV $83
line 179
;178:				}
;179:				trap_BotPopGoal(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotPopGoal
CALLV
pop
line 180
;180:			}
LABELV $81
line 173
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 4952
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 64
ADDRGP4 trap_BotChooseNBGItem
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
NEI4 $80
line 181
;181:			trap_BotResetAvoidGoals(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidGoals
CALLV
pop
line 182
;182:		}
line 183
;183:	}
LABELV $76
line 184
;184:	return qfalse;
CNSTI4 0
RETI4
LABELV $75
endproc BotGoForAir 68 24
export BotNearbyGoal
proc BotNearbyGoal 20 24
line 192
;185:}
;186:
;187:/*
;188:==================
;189:BotNearbyGoal
;190:==================
;191:*/
;192:int BotNearbyGoal(bot_state_t *bs, int tfl, bot_goal_t *ltg, float range) {
line 196
;193:	int ret;
;194:
;195:	//check if the bot should go for air
;196:	if (BotGoForAir(bs, tfl, ltg, range)) return qtrue;
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 BotGoForAir
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $86
CNSTI4 1
RETI4
ADDRGP4 $85
JUMPV
LABELV $86
line 198
;197:	//if the bot is carrying the enemy flag
;198:	if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $88
line 200
;199:		//if the bot is just a few secs away from the base 
;200:		if (trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin,
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 6636
ADDP4
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 16
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 300
GEI4 $90
line 201
;201:				bs->teamgoal.areanum, TFL_DEFAULT) < 300) {
line 203
;202:			//make the range really small
;203:			range = 50;
ADDRFP4 12
CNSTF4 1112014848
ASGNF4
line 204
;204:		}
LABELV $90
line 205
;205:	}
LABELV $88
line 207
;206:	//
;207:	ret = trap_BotChooseNBGItem(bs->gs, bs->origin, bs->inventory, tfl, ltg, range);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 4952
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 trap_BotChooseNBGItem
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 16
INDIRI4
ASGNI4
line 218
;208:	/*
;209:	if (ret)
;210:	{
;211:		char buf[128];
;212:		//get the goal at the top of the stack
;213:		trap_BotGetTopGoal(bs->gs, &goal);
;214:		trap_BotGoalName(goal.number, buf, sizeof(buf));
;215:		BotAI_Print(PRT_MESSAGE, "%1.1f: new nearby goal %s\n", FloatTime(), buf);
;216:	}
;217:    */
;218:	return ret;
ADDRLP4 0
INDIRI4
RETI4
LABELV $85
endproc BotNearbyGoal 20 24
export BotReachedGoal
proc BotReachedGoal 40 16
line 226
;219:}
;220:
;221:/*
;222:==================
;223:BotReachedGoal
;224:==================
;225:*/
;226:int BotReachedGoal(bot_state_t *bs, bot_goal_t *goal) {
line 227
;227:	if (goal->flags & GFL_ITEM) {
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $93
line 229
;228:		//if touching the goal
;229:		if (trap_BotTouchingGoal(bs->origin, goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $95
line 230
;230:			if (!(goal->flags & GFL_DROPPED)) {
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $97
line 231
;231:				trap_BotSetAvoidGoalTime(bs->gs, goal->number, -1);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
CNSTF4 3212836864
ARGF4
ADDRGP4 trap_BotSetAvoidGoalTime
CALLV
pop
line 232
;232:			}
LABELV $97
line 233
;233:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
LABELV $95
line 236
;234:		}
;235:		//if the goal isn't there
;236:		if (trap_BotItemGoalInVisButNotVisible(bs->entitynum, bs->eye, bs->viewangles, goal)) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 trap_BotItemGoalInVisButNotVisible
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $99
line 248
;237:			/*
;238:			float avoidtime;
;239:			int t;
;240:
;241:			avoidtime = trap_BotAvoidGoalTime(bs->gs, goal->number);
;242:			if (avoidtime > 0) {
;243:				t = trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin, goal->areanum, bs->tfl);
;244:				if ((float) t * 0.009 < avoidtime)
;245:					return qtrue;
;246:			}
;247:			*/
;248:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
LABELV $99
line 251
;249:		}
;250:		//if in the goal area and below or above the goal and not swimming
;251:		if (bs->areanum == goal->areanum) {
ADDRFP4 0
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
NEI4 $94
line 252
;252:			if (bs->origin[0] > goal->origin[0] + goal->mins[0] && bs->origin[0] < goal->origin[0] + goal->maxs[0]) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ASGNF4
ADDRLP4 16
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
INDIRF4
ASGNF4
ADDRLP4 12
INDIRF4
ADDRLP4 20
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDF4
LEF4 $94
ADDRLP4 12
INDIRF4
ADDRLP4 20
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDF4
GEF4 $94
line 253
;253:				if (bs->origin[1] > goal->origin[1] + goal->mins[1] && bs->origin[1] < goal->origin[1] + goal->maxs[1]) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ASGNF4
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
ADDRLP4 24
INDIRF4
ADDRLP4 32
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
LEF4 $94
ADDRLP4 24
INDIRF4
ADDRLP4 32
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDF4
GEF4 $94
line 254
;254:					if (!trap_AAS_Swimming(bs->origin)) {
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 36
ADDRGP4 trap_AAS_Swimming
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $94
line 255
;255:						return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
line 257
;256:					}
;257:				}
line 258
;258:			}
line 259
;259:		}
line 260
;260:	}
LABELV $93
line 261
;261:	else if (goal->flags & GFL_AIR) {
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $109
line 263
;262:		//if touching the goal
;263:		if (trap_BotTouchingGoal(bs->origin, goal)) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $111
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
LABELV $111
line 265
;264:		//if the bot got air
;265:		if (bs->lastair_time > FloatTime() - 1) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 6176
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
LEF4 $110
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
line 266
;266:	}
LABELV $109
line 267
;267:	else {
line 269
;268:		//if touching the goal
;269:		if (trap_BotTouchingGoal(bs->origin, goal)) return qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $115
CNSTI4 1
RETI4
ADDRGP4 $92
JUMPV
LABELV $115
line 270
;270:	}
LABELV $110
LABELV $94
line 271
;271:	return qfalse;
CNSTI4 0
RETI4
LABELV $92
endproc BotReachedGoal 40 16
export BotGetItemLongTermGoal
proc BotGetItemLongTermGoal 20 16
line 279
;272:}
;273:
;274:/*
;275:==================
;276:BotGetItemLongTermGoal
;277:==================
;278:*/
;279:int BotGetItemLongTermGoal(bot_state_t *bs, int tfl, bot_goal_t *goal) {
line 281
;280:	//if the bot has no goal
;281:	if (!trap_BotGetTopGoal(bs->gs, goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $118
line 283
;282:		//BotAI_Print(PRT_MESSAGE, "no ltg on stack\n");
;283:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 284
;284:	}
ADDRGP4 $119
JUMPV
LABELV $118
line 286
;285:	//if the bot touches the current goal
;286:	else if (BotReachedGoal(bs, goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotReachedGoal
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $120
line 287
;287:		BotChooseWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChooseWeapon
CALLV
pop
line 288
;288:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 289
;289:	}
LABELV $120
LABELV $119
line 291
;290:	//if it is time to find a new long term goal
;291:	if (bs->ltg_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $122
line 293
;292:		//pop the current goal from the stack
;293:		trap_BotPopGoal(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotPopGoal
CALLV
pop
line 297
;294:		//BotAI_Print(PRT_MESSAGE, "%s: choosing new ltg\n", ClientName(bs->client, netname, sizeof(netname)));
;295:		//choose a new goal
;296:		//BotAI_Print(PRT_MESSAGE, "%6.1f client %d: BotChooseLTGItem\n", FloatTime(), bs->client);
;297:		if (trap_BotChooseLTGItem(bs->gs, bs->origin, bs->inventory, tfl)) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 4952
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 trap_BotChooseLTGItem
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $124
line 305
;298:			/*
;299:			char buf[128];
;300:			//get the goal at the top of the stack
;301:			trap_BotGetTopGoal(bs->gs, goal);
;302:			trap_BotGoalName(goal->number, buf, sizeof(buf));
;303:			BotAI_Print(PRT_MESSAGE, "%1.1f: new long term goal %s\n", FloatTime(), buf);
;304:            */
;305:			bs->ltg_time = FloatTime() + 20;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1101004800
ADDF4
ASGNF4
line 306
;306:		}
ADDRGP4 $125
JUMPV
LABELV $124
line 307
;307:		else {//the bot gets sorta stuck with all the avoid timings, shouldn't happen though
line 316
;308:			//
;309:#ifdef DEBUG
;310:			char netname[128];
;311:
;312:			BotAI_Print(PRT_MESSAGE, "%s: no valid ltg (probably stuck)\n", ClientName(bs->client, netname, sizeof(netname)));
;313:#endif
;314:			//trap_BotDumpAvoidGoals(bs->gs);
;315:			//reset the avoid goals and the avoid reach
;316:			trap_BotResetAvoidGoals(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidGoals
CALLV
pop
line 317
;317:			trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 318
;318:		}
LABELV $125
line 320
;319:		//get the goal at the top of the stack
;320:		return trap_BotGetTopGoal(bs->gs, goal);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
RETI4
ADDRGP4 $117
JUMPV
LABELV $122
line 322
;321:	}
;322:	return qtrue;
CNSTI4 1
RETI4
LABELV $117
endproc BotGetItemLongTermGoal 20 16
export BotGetLongTermGoal
proc BotGetLongTermGoal 664 20
line 333
;323:}
;324:
;325:/*
;326:==================
;327:BotGetLongTermGoal
;328:
;329:we could also create a seperate AI node for every long term goal type
;330:however this saves us a lot of code
;331:==================
;332:*/
;333:int BotGetLongTermGoal(bot_state_t *bs, int tfl, int retreat, bot_goal_t *goal) {
line 342
;334:	vec3_t target, dir, dir2;
;335:	char netname[MAX_NETNAME];
;336:	char buf[MAX_MESSAGE_SIZE];
;337:	int areanum;
;338:	float croucher;
;339:	aas_entityinfo_t entinfo, botinfo;
;340:	bot_waypoint_t *wp;
;341:
;342:	if (bs->ltgtype == LTG_TEAMHELP && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 1
NEI4 $127
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $127
line 344
;343:		//check for bot typing status message
;344:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 620
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 620
INDIRF4
CNSTF4 0
EQF4 $129
ADDRLP4 620
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $129
line 345
;345:			BotAI_BotInitialChat(bs, "help_start", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 624
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $131
ARGP4
ADDRLP4 624
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 346
;346:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 347
;347:			BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_YES);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
ARGP4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 348
;348:			trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 349
;349:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 350
;350:		}
LABELV $129
line 352
;351:		//if trying to help the team mate for more than a minute
;352:		if (bs->teamgoal_time < FloatTime())
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $133
line 353
;353:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
LABELV $133
line 355
;354:		//if the team mate IS visible for quite some time
;355:		if (bs->teammatevisible_time < FloatTime() - 10) bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
SUBF4
GEF4 $135
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
LABELV $135
line 357
;356:		//get entity information of the companion
;357:		BotEntityInfo(bs->teammate, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 272
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 359
;358:		//if the team mate is visible
;359:		if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->teammate)) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 624
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 624
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 624
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 624
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 628
INDIRF4
CNSTF4 0
EQF4 $137
line 361
;360:			//if close just stand still there
;361:			VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 272+24
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 272+24+4
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRLP4 272+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 362
;362:			if (VectorLengthSquared(dir) < Square(100)) {
ADDRLP4 260
ARGP4
ADDRLP4 636
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 636
INDIRF4
CNSTF4 1176256512
GEF4 $138
line 363
;363:				trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 364
;364:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
line 366
;365:			}
;366:		}
LABELV $137
line 367
;367:		else {
line 369
;368:			//last time the bot was NOT visible
;369:			bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 370
;370:		}
LABELV $138
line 372
;371:		//if the entity information is valid (entity in PVS)
;372:		if (entinfo.valid) {
ADDRLP4 272
INDIRI4
CNSTI4 0
EQI4 $148
line 373
;373:			areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 272+24
ARGP4
ADDRLP4 632
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 448
ADDRLP4 632
INDIRI4
ASGNI4
line 374
;374:			if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 636
ADDRLP4 448
INDIRI4
ASGNI4
ADDRLP4 636
INDIRI4
CNSTI4 0
EQI4 $151
ADDRLP4 636
INDIRI4
ARGI4
ADDRLP4 640
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 640
INDIRI4
CNSTI4 0
EQI4 $151
line 376
;375:				//update team goal
;376:				bs->teamgoal.entitynum = bs->teammate;
ADDRLP4 644
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 644
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 644
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ASGNI4
line 377
;377:				bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 448
INDIRI4
ASGNI4
line 378
;378:				VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 272+24
INDIRB
ASGNB 12
line 379
;379:				VectorSet(bs->teamgoal.mins, -8, -8, -8);
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
line 380
;380:				VectorSet(bs->teamgoal.maxs, 8, 8, 8);
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
line 381
;381:			}
LABELV $151
line 382
;382:		}
LABELV $148
line 383
;383:		memcpy(goal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 384
;384:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $127
line 387
;385:	}
;386:	//if the bot accompanies someone
;387:	if (bs->ltgtype == LTG_TEAMACCOMPANY && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 2
NEI4 $154
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $154
line 389
;388:		//check for bot typing status message
;389:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 620
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 620
INDIRF4
CNSTF4 0
EQF4 $156
ADDRLP4 620
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $156
line 390
;390:			BotAI_BotInitialChat(bs, "accompany_start", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 624
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $158
ARGP4
ADDRLP4 624
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 391
;391:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 392
;392:			BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_YES);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
ARGP4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 393
;393:			trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 394
;394:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 395
;395:		}
LABELV $156
line 397
;396:		//if accompanying the companion for 3 minutes
;397:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $159
line 398
;398:			BotAI_BotInitialChat(bs, "accompany_stop", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 624
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $161
ARGP4
ADDRLP4 624
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 399
;399:			trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 400
;400:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 401
;401:		}
LABELV $159
line 403
;402:		//get entity information of the companion
;403:		BotEntityInfo(bs->teammate, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 272
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 405
;404:		//if the companion is visible
;405:		if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->teammate)) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 624
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 624
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 624
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 624
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 628
INDIRF4
CNSTF4 0
EQF4 $162
line 407
;406:			//update visible time
;407:			bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 408
;408:			VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 272+24
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 272+24+4
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRLP4 272+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 409
;409:			if (VectorLengthSquared(dir) < Square(bs->formation_dist)) {
ADDRLP4 260
ARGP4
ADDRLP4 636
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 640
ADDRFP4 0
INDIRP4
CNSTI4 7012
ADDP4
INDIRF4
ASGNF4
ADDRLP4 636
INDIRF4
ADDRLP4 640
INDIRF4
ADDRLP4 640
INDIRF4
MULF4
GEF4 $171
line 413
;410:				//
;411:				// if the client being followed bumps into this bot then
;412:				// the bot should back up
;413:				BotEntityInfo(bs->entitynum, &botinfo);
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 452
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 415
;414:				// if the followed client is not standing ontop of the bot
;415:				if (botinfo.origin[2] + botinfo.maxs[2] > entinfo.origin[2] + entinfo.mins[2]) {
ADDRLP4 452+24+8
INDIRF4
ADDRLP4 452+84+8
INDIRF4
ADDF4
ADDRLP4 272+24+8
INDIRF4
ADDRLP4 272+72+8
INDIRF4
ADDF4
LEF4 $173
line 417
;416:					// if the bounding boxes touch each other
;417:					if (botinfo.origin[0] + botinfo.maxs[0] > entinfo.origin[0] + entinfo.mins[0] - 4&&
ADDRLP4 644
CNSTF4 1082130432
ASGNF4
ADDRLP4 452+24
INDIRF4
ADDRLP4 452+84
INDIRF4
ADDF4
ADDRLP4 272+24
INDIRF4
ADDRLP4 272+72
INDIRF4
ADDF4
ADDRLP4 644
INDIRF4
SUBF4
LEF4 $183
ADDRLP4 452+24
INDIRF4
ADDRLP4 452+72
INDIRF4
ADDF4
ADDRLP4 272+24
INDIRF4
ADDRLP4 272+84
INDIRF4
ADDF4
ADDRLP4 644
INDIRF4
ADDF4
GEF4 $183
line 418
;418:						botinfo.origin[0] + botinfo.mins[0] < entinfo.origin[0] + entinfo.maxs[0] + 4) {
line 419
;419:						if (botinfo.origin[1] + botinfo.maxs[1] > entinfo.origin[1] + entinfo.mins[1] - 4 &&
ADDRLP4 648
CNSTF4 1082130432
ASGNF4
ADDRLP4 452+24+4
INDIRF4
ADDRLP4 452+84+4
INDIRF4
ADDF4
ADDRLP4 272+24+4
INDIRF4
ADDRLP4 272+72+4
INDIRF4
ADDF4
ADDRLP4 648
INDIRF4
SUBF4
LEF4 $193
ADDRLP4 452+24+4
INDIRF4
ADDRLP4 452+72+4
INDIRF4
ADDF4
ADDRLP4 272+24+4
INDIRF4
ADDRLP4 272+84+4
INDIRF4
ADDF4
ADDRLP4 648
INDIRF4
ADDF4
GEF4 $193
line 420
;420:							botinfo.origin[1] + botinfo.mins[1] < entinfo.origin[1] + entinfo.maxs[1] + 4) {
line 421
;421:							if (botinfo.origin[2] + botinfo.maxs[2] > entinfo.origin[2] + entinfo.mins[2] - 4 &&
ADDRLP4 652
CNSTF4 1082130432
ASGNF4
ADDRLP4 452+24+8
INDIRF4
ADDRLP4 452+84+8
INDIRF4
ADDF4
ADDRLP4 272+24+8
INDIRF4
ADDRLP4 272+72+8
INDIRF4
ADDF4
ADDRLP4 652
INDIRF4
SUBF4
LEF4 $211
ADDRLP4 452+24+8
INDIRF4
ADDRLP4 452+72+8
INDIRF4
ADDF4
ADDRLP4 272+24+8
INDIRF4
ADDRLP4 272+84+8
INDIRF4
ADDF4
ADDRLP4 652
INDIRF4
ADDF4
GEF4 $211
line 422
;422:								botinfo.origin[2] + botinfo.mins[2] < entinfo.origin[2] + entinfo.maxs[2] + 4) {
line 424
;423:								// if the followed client looks in the direction of this bot
;424:								AngleVectors(entinfo.angles, dir, NULL, NULL);
ADDRLP4 272+36
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 656
CNSTP4 0
ASGNP4
ADDRLP4 656
INDIRP4
ARGP4
ADDRLP4 656
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 425
;425:								dir[2] = 0;
ADDRLP4 260+8
CNSTF4 0
ASGNF4
line 426
;426:								VectorNormalize(dir);
ADDRLP4 260
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 428
;427:								//VectorSubtract(entinfo.origin, entinfo.lastvisorigin, dir);
;428:								VectorSubtract(bs->origin, entinfo.origin, dir2);
ADDRLP4 660
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 608
ADDRLP4 660
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRLP4 272+24
INDIRF4
SUBF4
ASGNF4
ADDRLP4 608+4
ADDRLP4 660
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRLP4 272+24+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 608+8
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRLP4 272+24+8
INDIRF4
SUBF4
ASGNF4
line 429
;429:								VectorNormalize(dir2);
ADDRLP4 608
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 430
;430:								if (DotProduct(dir, dir2) > 0.7) {
ADDRLP4 260
INDIRF4
ADDRLP4 608
INDIRF4
MULF4
ADDRLP4 260+4
INDIRF4
ADDRLP4 608+4
INDIRF4
MULF4
ADDF4
ADDRLP4 260+8
INDIRF4
ADDRLP4 608+8
INDIRF4
MULF4
ADDF4
CNSTF4 1060320051
LEF4 $238
line 432
;431:									// back up
;432:									BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 433
;433:									trap_BotMoveInDirection(bs->ms, dir2, 400, MOVE_WALK);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 608
ARGP4
CNSTF4 1137180672
ARGF4
CNSTI4 1
ARGI4
ADDRGP4 trap_BotMoveInDirection
CALLI4
pop
line 434
;434:								}
LABELV $238
line 435
;435:							}
LABELV $211
line 436
;436:						}
LABELV $193
line 437
;437:					}
LABELV $183
line 438
;438:				}
LABELV $173
line 441
;439:				//check if the bot wants to crouch
;440:				//don't crouch if crouched less than 5 seconds ago
;441:				if (bs->attackcrouch_time < FloatTime() - 5) {
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
SUBF4
GEF4 $244
line 442
;442:					croucher = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CROUCHER, 0, 1);
ADDRFP4 0
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
ADDRLP4 644
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 604
ADDRLP4 644
INDIRF4
ASGNF4
line 443
;443:					if (random() < bs->thinktime * croucher) {
ADDRLP4 648
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
ADDRLP4 604
INDIRF4
MULF4
GEF4 $246
line 444
;444:						bs->attackcrouch_time = FloatTime() + 5 + croucher * 15;
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CNSTF4 1097859072
ADDRLP4 604
INDIRF4
MULF4
ADDF4
ASGNF4
line 445
;445:					}
LABELV $246
line 446
;446:				}
LABELV $244
line 448
;447:				//don't crouch when swimming
;448:				if (trap_AAS_Swimming(bs->origin)) bs->attackcrouch_time = FloatTime() - 1;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 644
ADDRGP4 trap_AAS_Swimming
CALLI4
ASGNI4
ADDRLP4 644
INDIRI4
CNSTI4 0
EQI4 $248
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
ASGNF4
LABELV $248
line 450
;449:				//if not arrived yet or arived some time ago
;450:				if (bs->arrive_time < FloatTime() - 2) {
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
GEF4 $250
line 452
;451:					//if not arrived yet
;452:					if (!bs->arrive_time) {
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
INDIRF4
CNSTF4 0
NEF4 $252
line 453
;453:						trap_EA_Gesture(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Gesture
CALLV
pop
line 454
;454:						BotAI_BotInitialChat(bs, "accompany_arrive", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 648
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $254
ARGP4
ADDRLP4 648
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 455
;455:						trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 652
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 652
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 652
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 456
;456:						bs->arrive_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 457
;457:					}
ADDRGP4 $253
JUMPV
LABELV $252
line 459
;458:					//if the bot wants to crouch
;459:					else if (bs->attackcrouch_time > FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $255
line 460
;460:						trap_EA_Crouch(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Crouch
CALLV
pop
line 461
;461:					}
ADDRGP4 $256
JUMPV
LABELV $255
line 463
;462:					//else do some model taunts
;463:					else if (random() < bs->thinktime * 0.05) {
ADDRLP4 648
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1028443341
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $257
line 465
;464:						//do a gesture :)
;465:						trap_EA_Gesture(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Gesture
CALLV
pop
line 466
;466:					}
LABELV $257
LABELV $256
LABELV $253
line 467
;467:				}
LABELV $250
line 469
;468:				//if just arrived look at the companion
;469:				if (bs->arrive_time > FloatTime() - 2) {
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
LEF4 $259
line 470
;470:					VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 648
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 272+24
INDIRF4
ADDRLP4 648
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 272+24+4
INDIRF4
ADDRLP4 648
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRLP4 272+24+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 471
;471:					vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 260
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 472
;472:					bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 652
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 652
INDIRP4
CNSTF4 1056964608
ADDRLP4 652
INDIRP4
INDIRF4
MULF4
ASGNF4
line 473
;473:				}
ADDRGP4 $260
JUMPV
LABELV $259
line 475
;474:				//else look strategically around for enemies
;475:				else if (random() < bs->thinktime * 0.8) {
ADDRLP4 648
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $268
line 476
;476:					BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 592
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 477
;477:					VectorSubtract(target, bs->origin, dir);
ADDRLP4 652
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 592
INDIRF4
ADDRLP4 652
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 592+4
INDIRF4
ADDRLP4 652
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRLP4 592+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 478
;478:					vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 260
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 479
;479:					bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 656
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 656
INDIRP4
CNSTF4 1056964608
ADDRLP4 656
INDIRP4
INDIRF4
MULF4
ASGNF4
line 480
;480:				}
LABELV $268
LABELV $260
line 482
;481:				//check if the bot wants to go for air
;482:				if (BotGoForAir(bs, bs->tfl, &bs->teamgoal, 400)) {
ADDRLP4 652
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 652
INDIRP4
ARGP4
ADDRLP4 652
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 652
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTF4 1137180672
ARGF4
ADDRLP4 656
ADDRGP4 BotGoForAir
CALLI4
ASGNI4
ADDRLP4 656
INDIRI4
CNSTI4 0
EQI4 $274
line 483
;483:					trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 489
;484:					//get the goal at the top of the stack
;485:					//trap_BotGetTopGoal(bs->gs, &tmpgoal);
;486:					//trap_BotGoalName(tmpgoal.number, buf, 144);
;487:					//BotAI_Print(PRT_MESSAGE, "new nearby goal %s\n", buf);
;488:					//time the bot gets to pick up the nearby goal item
;489:					bs->nbg_time = FloatTime() + 8;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 490
;490:					AIEnter_Seek_NBG(bs, "BotLongTermGoal: go for air");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $276
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 491
;491:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $274
line 494
;492:				}
;493:				//
;494:				trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 495
;495:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $171
line 497
;496:			}
;497:		}
LABELV $162
line 499
;498:		//if the entity information is valid (entity in PVS)
;499:		if (entinfo.valid) {
ADDRLP4 272
INDIRI4
CNSTI4 0
EQI4 $277
line 500
;500:			areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 272+24
ARGP4
ADDRLP4 632
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 448
ADDRLP4 632
INDIRI4
ASGNI4
line 501
;501:			if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 636
ADDRLP4 448
INDIRI4
ASGNI4
ADDRLP4 636
INDIRI4
CNSTI4 0
EQI4 $280
ADDRLP4 636
INDIRI4
ARGI4
ADDRLP4 640
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 640
INDIRI4
CNSTI4 0
EQI4 $280
line 503
;502:				//update team goal
;503:				bs->teamgoal.entitynum = bs->teammate;
ADDRLP4 644
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 644
INDIRP4
CNSTI4 6664
ADDP4
ADDRLP4 644
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ASGNI4
line 504
;504:				bs->teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6636
ADDP4
ADDRLP4 448
INDIRI4
ASGNI4
line 505
;505:				VectorCopy(entinfo.origin, bs->teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ADDRLP4 272+24
INDIRB
ASGNB 12
line 506
;506:				VectorSet(bs->teamgoal.mins, -8, -8, -8);
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
line 507
;507:				VectorSet(bs->teamgoal.maxs, 8, 8, 8);
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
line 508
;508:			}
LABELV $280
line 509
;509:		}
LABELV $277
line 511
;510:		//the goal the bot should go for
;511:		memcpy(goal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 513
;512:		//if the companion is NOT visible for too long
;513:		if (bs->teammatevisible_time < FloatTime() - 60) {
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1114636288
SUBF4
GEF4 $283
line 514
;514:			BotAI_BotInitialChat(bs, "accompany_cannotfind", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 632
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $285
ARGP4
ADDRLP4 632
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 515
;515:			trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 636
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 636
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 636
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 516
;516:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 518
;517:			// just to make sure the bot won't spam this message
;518:			bs->teammatevisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6748
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 519
;519:		}
LABELV $283
line 520
;520:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $154
line 523
;521:	}
;522:	//
;523:	if (bs->ltgtype == LTG_DEFENDKEYAREA) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 3
NEI4 $286
line 524
;524:		if (trap_AAS_AreaTravelTimeToGoalArea(bs->areanum, bs->origin,
ADDRLP4 620
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 620
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ARGI4
ADDRLP4 620
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 620
INDIRP4
CNSTI4 6636
ADDP4
INDIRI4
ARGI4
CNSTI4 18616254
ARGI4
ADDRLP4 624
ADDRGP4 trap_AAS_AreaTravelTimeToGoalArea
CALLI4
ASGNI4
ADDRLP4 624
INDIRI4
CVIF4 4
ADDRFP4 0
INDIRP4
CNSTI4 6148
ADDP4
INDIRF4
LEF4 $288
line 525
;525:				bs->teamgoal.areanum, TFL_DEFAULT) > bs->defendaway_range) {
line 526
;526:			bs->defendaway_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6144
ADDP4
CNSTF4 0
ASGNF4
line 527
;527:		}
LABELV $288
line 528
;528:	}
LABELV $286
line 530
;529:	//if defending a key area
;530:	if (bs->ltgtype == LTG_DEFENDKEYAREA && !retreat &&
ADDRLP4 620
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 620
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 3
NEI4 $290
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $290
ADDRLP4 620
INDIRP4
CNSTI4 6144
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $290
line 531
;531:				bs->defendaway_time < FloatTime()) {
line 533
;532:		//check for bot typing status message
;533:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 624
INDIRF4
CNSTF4 0
EQF4 $292
ADDRLP4 624
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $292
line 534
;534:			trap_BotGoalName(bs->teamgoal.number, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 535
;535:			BotAI_BotInitialChat(bs, "defend_start", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $294
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 536
;536:			trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 537
;537:			BotVoiceChatOnly(bs, -1, VOICECHAT_ONDEFENSE);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $295
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 538
;538:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 539
;539:		}
LABELV $292
line 541
;540:		//set the bot goal
;541:		memcpy(goal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 543
;542:		//stop after 2 minutes
;543:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $296
line 544
;544:			trap_BotGoalName(bs->teamgoal.number, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 545
;545:			BotAI_BotInitialChat(bs, "defend_stop", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $298
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 546
;546:			trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 547
;547:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 548
;548:		}
LABELV $296
line 550
;549:		//if very close... go away for some time
;550:		VectorSubtract(goal->origin, bs->origin, dir);
ADDRLP4 628
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 628
INDIRP4
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 628
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 632
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 551
;551:		if (VectorLengthSquared(dir) < Square(70)) {
ADDRLP4 260
ARGP4
ADDRLP4 636
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 636
INDIRF4
CNSTF4 1167663104
GEF4 $301
line 552
;552:			trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 553
;553:			bs->defendaway_time = FloatTime() + 3 + 3 * random();
ADDRLP4 640
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 644
CNSTF4 1077936128
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 6144
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 644
INDIRF4
ADDF4
ADDRLP4 644
INDIRF4
ADDRLP4 640
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 554
;554:			if (BotHasPersistantPowerupAndWeapon(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 648
ADDRGP4 BotHasPersistantPowerupAndWeapon
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 0
EQI4 $303
line 555
;555:				bs->defendaway_range = 100;
ADDRFP4 0
INDIRP4
CNSTI4 6148
ADDP4
CNSTF4 1120403456
ASGNF4
line 556
;556:			}
ADDRGP4 $304
JUMPV
LABELV $303
line 557
;557:			else {
line 558
;558:				bs->defendaway_range = 350;
ADDRFP4 0
INDIRP4
CNSTI4 6148
ADDP4
CNSTF4 1135542272
ASGNF4
line 559
;559:			}
LABELV $304
line 560
;560:		}
LABELV $301
line 561
;561:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $290
line 564
;562:	}
;563:	//going to kill someone
;564:	if (bs->ltgtype == LTG_KILL && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 11
NEI4 $305
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $305
line 566
;565:		//check for bot typing status message
;566:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 624
INDIRF4
CNSTF4 0
EQF4 $307
ADDRLP4 624
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $307
line 567
;567:			EasyClientName(bs->teamgoal.entitynum, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 568
;568:			BotAI_BotInitialChat(bs, "kill_start", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $309
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 569
;569:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 570
;570:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 571
;571:		}
LABELV $307
line 573
;572:		//
;573:		if (bs->lastkilledplayer == bs->teamgoal.entitynum) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ADDRLP4 628
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
NEI4 $310
line 574
;574:			EasyClientName(bs->teamgoal.entitynum, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6664
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 EasyClientName
CALLP4
pop
line 575
;575:			BotAI_BotInitialChat(bs, "kill_done", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $312
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 576
;576:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 577
;577:			bs->lastkilledplayer = -1;
ADDRFP4 0
INDIRP4
CNSTI4 5992
ADDP4
CNSTI4 -1
ASGNI4
line 578
;578:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 579
;579:		}
LABELV $310
line 581
;580:		//
;581:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $313
line 582
;582:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 583
;583:		}
LABELV $313
line 585
;584:		//just roam around
;585:		return BotGetItemLongTermGoal(bs, tfl, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 632
ADDRGP4 BotGetItemLongTermGoal
CALLI4
ASGNI4
ADDRLP4 632
INDIRI4
RETI4
ADDRGP4 $126
JUMPV
LABELV $305
line 588
;586:	}
;587:	//get an item
;588:	if (bs->ltgtype == LTG_GETITEM && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 10
NEI4 $315
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $315
line 590
;589:		//check for bot typing status message
;590:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 624
INDIRF4
CNSTF4 0
EQF4 $317
ADDRLP4 624
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $317
line 591
;591:			trap_BotGoalName(bs->teamgoal.number, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 592
;592:			BotAI_BotInitialChat(bs, "getitem_start", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $319
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 593
;593:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 594
;594:			BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_YES);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
ARGP4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 595
;595:			trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 596
;596:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 597
;597:		}
LABELV $317
line 599
;598:		//set the bot goal
;599:		memcpy(goal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 601
;600:		//stop after some time
;601:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $320
line 602
;602:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 603
;603:		}
LABELV $320
line 605
;604:		//
;605:		if (trap_BotItemGoalInVisButNotVisible(bs->entitynum, bs->eye, bs->viewangles, goal)) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 628
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 628
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 632
ADDRGP4 trap_BotItemGoalInVisButNotVisible
CALLI4
ASGNI4
ADDRLP4 632
INDIRI4
CNSTI4 0
EQI4 $322
line 606
;606:			trap_BotGoalName(bs->teamgoal.number, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 607
;607:			BotAI_BotInitialChat(bs, "getitem_notthere", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $324
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 608
;608:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 636
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 636
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 636
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 609
;609:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 610
;610:		}
ADDRGP4 $323
JUMPV
LABELV $322
line 611
;611:		else if (BotReachedGoal(bs, goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 636
ADDRGP4 BotReachedGoal
CALLI4
ASGNI4
ADDRLP4 636
INDIRI4
CNSTI4 0
EQI4 $325
line 612
;612:			trap_BotGoalName(bs->teamgoal.number, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
CNSTI4 6668
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 613
;613:			BotAI_BotInitialChat(bs, "getitem_gotit", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $327
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 614
;614:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 640
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 640
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 640
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 615
;615:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 616
;616:		}
LABELV $325
LABELV $323
line 617
;617:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $315
line 620
;618:	}
;619:	//if camping somewhere
;620:	if ((bs->ltgtype == LTG_CAMP || bs->ltgtype == LTG_CAMPORDER) && !retreat) {
ADDRLP4 624
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
ASGNI4
ADDRLP4 624
INDIRI4
CNSTI4 7
EQI4 $330
ADDRLP4 624
INDIRI4
CNSTI4 8
NEI4 $328
LABELV $330
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $328
line 622
;621:		//check for bot typing status message
;622:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 628
INDIRF4
CNSTF4 0
EQF4 $331
ADDRLP4 628
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $331
line 623
;623:			if (bs->ltgtype == LTG_CAMPORDER) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 8
NEI4 $333
line 624
;624:				BotAI_BotInitialChat(bs, "camp_start", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 632
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $335
ARGP4
ADDRLP4 632
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 625
;625:				trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 636
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 636
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 636
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 626
;626:				BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_YES);
ADDRLP4 640
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 640
INDIRP4
ARGP4
ADDRLP4 640
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 627
;627:				trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 628
;628:			}
LABELV $333
line 629
;629:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 630
;630:		}
LABELV $331
line 632
;631:		//set the bot goal
;632:		memcpy(goal, &bs->teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6624
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 634
;633:		//
;634:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $336
line 635
;635:			if (bs->ltgtype == LTG_CAMPORDER) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 8
NEI4 $338
line 636
;636:				BotAI_BotInitialChat(bs, "camp_stop", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $340
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 637
;637:				trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 638
;638:			}
LABELV $338
line 639
;639:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 640
;640:		}
LABELV $336
line 642
;641:		//if really near the camp spot
;642:		VectorSubtract(goal->origin, bs->origin, dir);
ADDRLP4 632
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 636
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 632
INDIRP4
INDIRF4
ADDRLP4 636
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 632
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 636
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 643
;643:		if (VectorLengthSquared(dir) < Square(60))
ADDRLP4 260
ARGP4
ADDRLP4 640
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 640
INDIRF4
CNSTF4 1163984896
GEF4 $343
line 644
;644:		{
line 646
;645:			//if not arrived yet
;646:			if (!bs->arrive_time) {
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
INDIRF4
CNSTF4 0
NEF4 $345
line 647
;647:				if (bs->ltgtype == LTG_CAMPORDER) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 8
NEI4 $347
line 648
;648:					BotAI_BotInitialChat(bs, "camp_arrive", EasyClientName(bs->teammate, netname, sizeof(netname)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
ADDRLP4 412
ARGP4
CNSTI4 36
ARGI4
ADDRLP4 644
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $349
ARGP4
ADDRLP4 644
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 649
;649:					trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 648
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 648
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 648
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 650
;650:					BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_INPOSITION);
ADDRLP4 652
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 652
INDIRP4
ARGP4
ADDRLP4 652
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $350
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 651
;651:				}
LABELV $347
line 652
;652:				bs->arrive_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6172
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 653
;653:			}
LABELV $345
line 655
;654:			//look strategically around for enemies
;655:			if (random() < bs->thinktime * 0.8) {
ADDRLP4 644
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 644
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $351
line 656
;656:				BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 592
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 657
;657:				VectorSubtract(target, bs->origin, dir);
ADDRLP4 648
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
ADDRLP4 592
INDIRF4
ADDRLP4 648
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 592+4
INDIRF4
ADDRLP4 648
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 260+8
ADDRLP4 592+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 658
;658:				vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 260
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 659
;659:				bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 652
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 652
INDIRP4
CNSTF4 1056964608
ADDRLP4 652
INDIRP4
INDIRF4
MULF4
ASGNF4
line 660
;660:			}
LABELV $351
line 663
;661:			//check if the bot wants to crouch
;662:			//don't crouch if crouched less than 5 seconds ago
;663:			if (bs->attackcrouch_time < FloatTime() - 5) {
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
SUBF4
GEF4 $357
line 664
;664:				croucher = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_CROUCHER, 0, 1);
ADDRFP4 0
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
ADDRLP4 648
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 604
ADDRLP4 648
INDIRF4
ASGNF4
line 665
;665:				if (random() < bs->thinktime * croucher) {
ADDRLP4 652
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 652
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
ADDRLP4 604
INDIRF4
MULF4
GEF4 $359
line 666
;666:					bs->attackcrouch_time = FloatTime() + 5 + croucher * 15;
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CNSTF4 1097859072
ADDRLP4 604
INDIRF4
MULF4
ADDF4
ASGNF4
line 667
;667:				}
LABELV $359
line 668
;668:			}
LABELV $357
line 670
;669:			//if the bot wants to crouch
;670:			if (bs->attackcrouch_time > FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $361
line 671
;671:				trap_EA_Crouch(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Crouch
CALLV
pop
line 672
;672:			}
LABELV $361
line 674
;673:			//don't crouch when swimming
;674:			if (trap_AAS_Swimming(bs->origin)) bs->attackcrouch_time = FloatTime() - 1;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 648
ADDRGP4 trap_AAS_Swimming
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 0
EQI4 $363
ADDRFP4 0
INDIRP4
CNSTI4 6120
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
ASGNF4
LABELV $363
line 676
;675:			//make sure the bot is not gonna drown
;676:			if (trap_PointContents(bs->eye,bs->entitynum) & (CONTENTS_WATER|CONTENTS_SLIME|CONTENTS_LAVA)) {
ADDRLP4 652
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 652
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 652
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 656
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 656
INDIRI4
CNSTI4 56
BANDI4
CNSTI4 0
EQI4 $365
line 677
;677:				if (bs->ltgtype == LTG_CAMPORDER) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 8
NEI4 $367
line 678
;678:					BotAI_BotInitialChat(bs, "camp_stop", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $340
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 679
;679:					trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 660
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 660
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 660
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 681
;680:					//
;681:					if (bs->lastgoal_ltgtype == LTG_CAMPORDER) {
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
INDIRI4
CNSTI4 8
NEI4 $369
line 682
;682:						bs->lastgoal_ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6760
ADDP4
CNSTI4 0
ASGNI4
line 683
;683:					}
LABELV $369
line 684
;684:				}
LABELV $367
line 685
;685:				bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 686
;686:			}
LABELV $365
line 688
;687:			//
;688:			if (bs->camp_range > 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6188
ADDP4
INDIRF4
CNSTF4 0
LEF4 $371
line 690
;689:				//FIXME: move around a bit
;690:			}
LABELV $371
line 692
;691:			//
;692:			trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 693
;693:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $343
line 695
;694:		}
;695:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $328
line 698
;696:	}
;697:	//patrolling along several waypoints
;698:	if (bs->ltgtype == LTG_PATROL && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 9
NEI4 $373
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $373
line 700
;699:		//check for bot typing status message
;700:		if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 628
INDIRF4
CNSTF4 0
EQF4 $375
ADDRLP4 628
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $375
line 701
;701:			strcpy(buf, "");
ADDRLP4 4
ARGP4
ADDRGP4 $60
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 702
;702:			for (wp = bs->patrolpoints; wp; wp = wp->next) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 9076
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $380
JUMPV
LABELV $377
line 703
;703:				strcat(buf, wp->name);
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 704
;704:				if (wp->next) strcat(buf, " to ");
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $381
ADDRLP4 4
ARGP4
ADDRGP4 $383
ARGP4
ADDRGP4 strcat
CALLP4
pop
LABELV $381
line 705
;705:			}
LABELV $378
line 702
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
LABELV $380
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $377
line 706
;706:			BotAI_BotInitialChat(bs, "patrol_start", buf, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $384
ARGP4
ADDRLP4 4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 707
;707:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 632
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 708
;708:			BotVoiceChatOnly(bs, bs->decisionmaker, VOICECHAT_YES);
ADDRLP4 636
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 636
INDIRP4
ARGP4
ADDRLP4 636
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 709
;709:			trap_EA_Action(bs->client, ACTION_AFFIRMATIVE);
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
line 710
;710:			bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 711
;711:		}
LABELV $375
line 713
;712:		//
;713:		if (!bs->curpatrolpoint) {
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $385
line 714
;714:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 715
;715:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $385
line 718
;716:		}
;717:		//if the bot touches the current goal
;718:		if (trap_BotTouchingGoal(bs->origin, &bs->curpatrolpoint->goal)) {
ADDRLP4 632
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 632
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 632
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ARGP4
ADDRLP4 636
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 636
INDIRI4
CNSTI4 0
EQI4 $387
line 719
;719:			if (bs->patrolflags & PATROL_BACK) {
ADDRFP4 0
INDIRP4
CNSTI4 9084
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $389
line 720
;720:				if (bs->curpatrolpoint->prev) {
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $391
line 721
;721:					bs->curpatrolpoint = bs->curpatrolpoint->prev;
ADDRLP4 640
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
ASGNP4
ADDRLP4 640
INDIRP4
ADDRLP4 640
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
ASGNP4
line 722
;722:				}
ADDRGP4 $390
JUMPV
LABELV $391
line 723
;723:				else {
line 724
;724:					bs->curpatrolpoint = bs->curpatrolpoint->next;
ADDRLP4 640
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
ASGNP4
ADDRLP4 640
INDIRP4
ADDRLP4 640
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
line 725
;725:					bs->patrolflags &= ~PATROL_BACK;
ADDRLP4 644
ADDRFP4 0
INDIRP4
CNSTI4 9084
ADDP4
ASGNP4
ADDRLP4 644
INDIRP4
ADDRLP4 644
INDIRP4
INDIRI4
CNSTI4 -5
BANDI4
ASGNI4
line 726
;726:				}
line 727
;727:			}
ADDRGP4 $390
JUMPV
LABELV $389
line 728
;728:			else {
line 729
;729:				if (bs->curpatrolpoint->next) {
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $393
line 730
;730:					bs->curpatrolpoint = bs->curpatrolpoint->next;
ADDRLP4 640
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
ASGNP4
ADDRLP4 640
INDIRP4
ADDRLP4 640
INDIRP4
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
ASGNP4
line 731
;731:				}
ADDRGP4 $394
JUMPV
LABELV $393
line 732
;732:				else {
line 733
;733:					bs->curpatrolpoint = bs->curpatrolpoint->prev;
ADDRLP4 640
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
ASGNP4
ADDRLP4 640
INDIRP4
ADDRLP4 640
INDIRP4
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
ASGNP4
line 734
;734:					bs->patrolflags |= PATROL_BACK;
ADDRLP4 644
ADDRFP4 0
INDIRP4
CNSTI4 9084
ADDP4
ASGNP4
ADDRLP4 644
INDIRP4
ADDRLP4 644
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 735
;735:				}
LABELV $394
line 736
;736:			}
LABELV $390
line 737
;737:		}
LABELV $387
line 739
;738:		//stop after 5 minutes
;739:		if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $395
line 740
;740:			BotAI_BotInitialChat(bs, "patrol_stop", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $397
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 741
;741:			trap_BotEnterChat(bs->cs, bs->decisionmaker, CHAT_TELL);
ADDRLP4 640
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 640
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 640
INDIRP4
CNSTI4 6608
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 742
;742:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 743
;743:		}
LABELV $395
line 744
;744:		if (!bs->curpatrolpoint) {
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $398
line 745
;745:			bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 746
;746:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $398
line 748
;747:		}
;748:		memcpy(goal, &bs->curpatrolpoint->goal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 9080
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 749
;749:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $373
line 752
;750:	}
;751:#ifdef CTF
;752:	if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $400
line 754
;753:		//if going for enemy flag
;754:		if (bs->ltgtype == LTG_GETFLAG) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 4
NEI4 $402
line 756
;755:			//check for bot typing status message
;756:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 628
INDIRF4
CNSTF4 0
EQF4 $404
ADDRLP4 628
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $404
line 757
;757:				BotAI_BotInitialChat(bs, "captureflag_start", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $406
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 758
;758:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 759
;759:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONGETFLAG);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $407
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 760
;760:				bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 761
;761:			}
LABELV $404
line 763
;762:			//
;763:			switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 636
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 632
ADDRLP4 636
INDIRI4
ASGNI4
ADDRLP4 632
INDIRI4
CNSTI4 1
EQI4 $411
ADDRLP4 632
INDIRI4
CNSTI4 2
EQI4 $412
ADDRGP4 $408
JUMPV
LABELV $411
line 764
;764:				case TEAM_RED: memcpy(goal, &ctf_blueflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_blueflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $409
JUMPV
LABELV $412
line 765
;765:				case TEAM_BLUE: memcpy(goal, &ctf_redflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_redflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $409
JUMPV
LABELV $408
line 766
;766:				default: bs->ltgtype = 0; return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $409
line 769
;767:			}
;768:			//if touching the flag
;769:			if (trap_BotTouchingGoal(bs->origin, goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 644
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 644
INDIRI4
CNSTI4 0
EQI4 $413
line 771
;770:				// make sure the bot knows the flag isn't there anymore
;771:				switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 652
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 648
ADDRLP4 652
INDIRI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 1
EQI4 $418
ADDRLP4 648
INDIRI4
CNSTI4 2
EQI4 $419
ADDRGP4 $415
JUMPV
LABELV $418
line 772
;772:					case TEAM_RED: bs->blueflagstatus = 1; break;
ADDRFP4 0
INDIRP4
CNSTI4 6956
ADDP4
CNSTI4 1
ASGNI4
ADDRGP4 $416
JUMPV
LABELV $419
line 773
;773:					case TEAM_BLUE: bs->redflagstatus = 1; break;
ADDRFP4 0
INDIRP4
CNSTI4 6952
ADDP4
CNSTI4 1
ASGNI4
LABELV $415
LABELV $416
line 775
;774:				}
;775:				bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 776
;776:			}
LABELV $413
line 778
;777:			//stop after 3 minutes
;778:			if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $420
line 779
;779:				bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 780
;780:			}
LABELV $420
line 781
;781:			BotAlternateRoute(bs, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 BotAlternateRoute
CALLP4
pop
line 782
;782:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $402
line 785
;783:		}
;784:		//if rushing to the base
;785:		if (bs->ltgtype == LTG_RUSHBASE && bs->rushbaseaway_time < FloatTime()) {
ADDRLP4 628
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 628
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 5
NEI4 $422
ADDRLP4 628
INDIRP4
CNSTI4 6152
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $422
line 786
;786:			switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 636
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 632
ADDRLP4 636
INDIRI4
ASGNI4
ADDRLP4 632
INDIRI4
CNSTI4 1
EQI4 $427
ADDRLP4 632
INDIRI4
CNSTI4 2
EQI4 $428
ADDRGP4 $424
JUMPV
LABELV $427
line 787
;787:				case TEAM_RED: memcpy(goal, &ctf_redflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_redflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $425
JUMPV
LABELV $428
line 788
;788:				case TEAM_BLUE: memcpy(goal, &ctf_blueflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_blueflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $425
JUMPV
LABELV $424
line 789
;789:				default: bs->ltgtype = 0; return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $425
line 792
;790:			}
;791:			//if not carrying the flag anymore
;792:			if (!BotCTFCarryingFlag(bs)) bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 644
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 644
INDIRI4
CNSTI4 0
NEI4 $429
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
LABELV $429
line 794
;793:			//quit rushing after 2 minutes
;794:			if (bs->teamgoal_time < FloatTime()) bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $431
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
LABELV $431
line 796
;795:			//if touching the base flag the bot should loose the enemy flag
;796:			if (trap_BotTouchingGoal(bs->origin, goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 648
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 0
EQI4 $433
line 799
;797:				//if the bot is still carrying the enemy flag then the
;798:				//base flag is gone, now just walk near the base a bit
;799:				if (BotCTFCarryingFlag(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 652
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 652
INDIRI4
CNSTI4 0
EQI4 $435
line 800
;800:					trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 801
;801:					bs->rushbaseaway_time = FloatTime() + 5 + 10 * random();
ADDRLP4 656
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6152
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
CNSTF4 1092616192
ADDRLP4 656
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
ASGNF4
line 803
;802:					//FIXME: add chat to tell the others to get back the flag
;803:				}
ADDRGP4 $436
JUMPV
LABELV $435
line 804
;804:				else {
line 805
;805:					bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 806
;806:				}
LABELV $436
line 807
;807:			}
LABELV $433
line 808
;808:			BotAlternateRoute(bs, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 BotAlternateRoute
CALLP4
pop
line 809
;809:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $422
line 812
;810:		}
;811:		//returning flag
;812:		if (bs->ltgtype == LTG_RETURNFLAG) {
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 6
NEI4 $437
line 814
;813:			//check for bot typing status message
;814:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
ADDRLP4 632
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
INDIRF4
ASGNF4
ADDRLP4 632
INDIRF4
CNSTF4 0
EQF4 $439
ADDRLP4 632
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $439
line 815
;815:				BotAI_BotInitialChat(bs, "returnflag_start", NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $441
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 816
;816:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
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
line 817
;817:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONRETURNFLAG);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 $442
ARGP4
ADDRGP4 BotVoiceChatOnly
CALLV
pop
line 818
;818:				bs->teammessage_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6740
ADDP4
CNSTF4 0
ASGNF4
line 819
;819:			}
LABELV $439
line 821
;820:			//
;821:			switch(BotTeam(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 640
ADDRGP4 BotTeam
CALLI4
ASGNI4
ADDRLP4 636
ADDRLP4 640
INDIRI4
ASGNI4
ADDRLP4 636
INDIRI4
CNSTI4 1
EQI4 $446
ADDRLP4 636
INDIRI4
CNSTI4 2
EQI4 $447
ADDRGP4 $443
JUMPV
LABELV $446
line 822
;822:				case TEAM_RED: memcpy(goal, &ctf_blueflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_blueflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $444
JUMPV
LABELV $447
line 823
;823:				case TEAM_BLUE: memcpy(goal, &ctf_redflag, sizeof(bot_goal_t)); break;
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 ctf_redflag
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
ADDRGP4 $444
JUMPV
LABELV $443
line 824
;824:				default: bs->ltgtype = 0; return qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
CNSTI4 0
RETI4
ADDRGP4 $126
JUMPV
LABELV $444
line 827
;825:			}
;826:			//if touching the flag
;827:			if (trap_BotTouchingGoal(bs->origin, goal)) bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 648
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 648
INDIRI4
CNSTI4 0
EQI4 $448
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
LABELV $448
line 829
;828:			//stop after 3 minutes
;829:			if (bs->teamgoal_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6744
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $450
line 830
;830:				bs->ltgtype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
CNSTI4 0
ASGNI4
line 831
;831:			}
LABELV $450
line 832
;832:			BotAlternateRoute(bs, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 BotAlternateRoute
CALLP4
pop
line 833
;833:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $126
JUMPV
LABELV $437
line 835
;834:		}
;835:	}
LABELV $400
line 1035
;836:#endif //CTF
;837:#ifdef MISSIONPACK
;838:	else if (gametype == GT_1FCTF) {
;839:		if (bs->ltgtype == LTG_GETFLAG) {
;840:			//check for bot typing status message
;841:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;842:				BotAI_BotInitialChat(bs, "captureflag_start", NULL);
;843:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;844:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONGETFLAG);
;845:				bs->teammessage_time = 0;
;846:			}
;847:			memcpy(goal, &ctf_neutralflag, sizeof(bot_goal_t));
;848:			//if touching the flag
;849:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;850:				bs->ltgtype = 0;
;851:			}
;852:			//stop after 3 minutes
;853:			if (bs->teamgoal_time < FloatTime()) {
;854:				bs->ltgtype = 0;
;855:			}
;856:			return qtrue;
;857:		}
;858:		//if rushing to the base
;859:		if (bs->ltgtype == LTG_RUSHBASE) {
;860:			switch(BotTeam(bs)) {
;861:				case TEAM_RED: memcpy(goal, &ctf_blueflag, sizeof(bot_goal_t)); break;
;862:				case TEAM_BLUE: memcpy(goal, &ctf_redflag, sizeof(bot_goal_t)); break;
;863:				default: bs->ltgtype = 0; return qfalse;
;864:			}
;865:			//if not carrying the flag anymore
;866:			if (!Bot1FCTFCarryingFlag(bs)) {
;867:				bs->ltgtype = 0;
;868:			}
;869:			//quit rushing after 2 minutes
;870:			if (bs->teamgoal_time < FloatTime()) {
;871:				bs->ltgtype = 0;
;872:			}
;873:			//if touching the base flag the bot should loose the enemy flag
;874:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;875:				bs->ltgtype = 0;
;876:			}
;877:			BotAlternateRoute(bs, goal);
;878:			return qtrue;
;879:		}
;880:		//attack the enemy base
;881:		if (bs->ltgtype == LTG_ATTACKENEMYBASE &&
;882:				bs->attackaway_time < FloatTime()) {
;883:			//check for bot typing status message
;884:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;885:				BotAI_BotInitialChat(bs, "attackenemybase_start", NULL);
;886:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;887:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONOFFENSE);
;888:				bs->teammessage_time = 0;
;889:			}
;890:			switch(BotTeam(bs)) {
;891:				case TEAM_RED: memcpy(goal, &ctf_blueflag, sizeof(bot_goal_t)); break;
;892:				case TEAM_BLUE: memcpy(goal, &ctf_redflag, sizeof(bot_goal_t)); break;
;893:				default: bs->ltgtype = 0; return qfalse;
;894:			}
;895:			//quit rushing after 2 minutes
;896:			if (bs->teamgoal_time < FloatTime()) {
;897:				bs->ltgtype = 0;
;898:			}
;899:			//if touching the base flag the bot should loose the enemy flag
;900:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;901:				bs->attackaway_time = FloatTime() + 2 + 5 * random();
;902:			}
;903:			return qtrue;
;904:		}
;905:		//returning flag
;906:		if (bs->ltgtype == LTG_RETURNFLAG) {
;907:			//check for bot typing status message
;908:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;909:				BotAI_BotInitialChat(bs, "returnflag_start", NULL);
;910:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;911:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONRETURNFLAG);
;912:				bs->teammessage_time = 0;
;913:			}
;914:			//
;915:			if (bs->teamgoal_time < FloatTime()) {
;916:				bs->ltgtype = 0;
;917:			}
;918:			//just roam around
;919:			return BotGetItemLongTermGoal(bs, tfl, goal);
;920:		}
;921:	}
;922:	else if (gametype == GT_OBELISK) {
;923:		if (bs->ltgtype == LTG_ATTACKENEMYBASE &&
;924:				bs->attackaway_time < FloatTime()) {
;925:
;926:			//check for bot typing status message
;927:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;928:				BotAI_BotInitialChat(bs, "attackenemybase_start", NULL);
;929:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;930:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONOFFENSE);
;931:				bs->teammessage_time = 0;
;932:			}
;933:			switch(BotTeam(bs)) {
;934:				case TEAM_RED: memcpy(goal, &blueobelisk, sizeof(bot_goal_t)); break;
;935:				case TEAM_BLUE: memcpy(goal, &redobelisk, sizeof(bot_goal_t)); break;
;936:				default: bs->ltgtype = 0; return qfalse;
;937:			}
;938:			//if the bot no longer wants to attack the obelisk
;939:			if (BotFeelingBad(bs) > 50) {
;940:				return BotGetItemLongTermGoal(bs, tfl, goal);
;941:			}
;942:			//if touching the obelisk
;943:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;944:				bs->attackaway_time = FloatTime() + 3 + 5 * random();
;945:			}
;946:			// or very close to the obelisk
;947:			VectorSubtract(bs->origin, goal->origin, dir);
;948:			if (VectorLengthSquared(dir) < Square(60)) {
;949:				bs->attackaway_time = FloatTime() + 3 + 5 * random();
;950:			}
;951:			//quit rushing after 2 minutes
;952:			if (bs->teamgoal_time < FloatTime()) {
;953:				bs->ltgtype = 0;
;954:			}
;955:			BotAlternateRoute(bs, goal);
;956:			//just move towards the obelisk
;957:			return qtrue;
;958:		}
;959:	}
;960:	else if (gametype == GT_HARVESTER) {
;961:		//if rushing to the base
;962:		if (bs->ltgtype == LTG_RUSHBASE) {
;963:			switch(BotTeam(bs)) {
;964:				case TEAM_RED: memcpy(goal, &blueobelisk, sizeof(bot_goal_t)); break;
;965:				case TEAM_BLUE: memcpy(goal, &redobelisk, sizeof(bot_goal_t)); break;
;966:				default: BotGoHarvest(bs); return qfalse;
;967:			}
;968:			//if not carrying any cubes
;969:			if (!BotHarvesterCarryingCubes(bs)) {
;970:				BotGoHarvest(bs);
;971:				return qfalse;
;972:			}
;973:			//quit rushing after 2 minutes
;974:			if (bs->teamgoal_time < FloatTime()) {
;975:				BotGoHarvest(bs);
;976:				return qfalse;
;977:			}
;978:			//if touching the base flag the bot should loose the enemy flag
;979:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;980:				BotGoHarvest(bs);
;981:				return qfalse;
;982:			}
;983:			BotAlternateRoute(bs, goal);
;984:			return qtrue;
;985:		}
;986:		//attack the enemy base
;987:		if (bs->ltgtype == LTG_ATTACKENEMYBASE &&
;988:				bs->attackaway_time < FloatTime()) {
;989:			//check for bot typing status message
;990:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;991:				BotAI_BotInitialChat(bs, "attackenemybase_start", NULL);
;992:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;993:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONOFFENSE);
;994:				bs->teammessage_time = 0;
;995:			}
;996:			switch(BotTeam(bs)) {
;997:				case TEAM_RED: memcpy(goal, &blueobelisk, sizeof(bot_goal_t)); break;
;998:				case TEAM_BLUE: memcpy(goal, &redobelisk, sizeof(bot_goal_t)); break;
;999:				default: bs->ltgtype = 0; return qfalse;
;1000:			}
;1001:			//quit rushing after 2 minutes
;1002:			if (bs->teamgoal_time < FloatTime()) {
;1003:				bs->ltgtype = 0;
;1004:			}
;1005:			//if touching the base flag the bot should loose the enemy flag
;1006:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;1007:				bs->attackaway_time = FloatTime() + 2 + 5 * random();
;1008:			}
;1009:			return qtrue;
;1010:		}
;1011:		//harvest cubes
;1012:		if (bs->ltgtype == LTG_HARVEST &&
;1013:			bs->harvestaway_time < FloatTime()) {
;1014:			//check for bot typing status message
;1015:			if (bs->teammessage_time && bs->teammessage_time < FloatTime()) {
;1016:				BotAI_BotInitialChat(bs, "harvest_start", NULL);
;1017:				trap_BotEnterChat(bs->cs, 0, CHAT_TEAM);
;1018:				BotVoiceChatOnly(bs, -1, VOICECHAT_ONOFFENSE);
;1019:				bs->teammessage_time = 0;
;1020:			}
;1021:			memcpy(goal, &neutralobelisk, sizeof(bot_goal_t));
;1022:			//
;1023:			if (bs->teamgoal_time < FloatTime()) {
;1024:				bs->ltgtype = 0;
;1025:			}
;1026:			//
;1027:			if (trap_BotTouchingGoal(bs->origin, goal)) {
;1028:				bs->harvestaway_time = FloatTime() + 4 + 3 * random();
;1029:			}
;1030:			return qtrue;
;1031:		}
;1032:	}
;1033:#endif
;1034:	//normal goal stuff
;1035:	return BotGetItemLongTermGoal(bs, tfl, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 628
ADDRGP4 BotGetItemLongTermGoal
CALLI4
ASGNI4
ADDRLP4 628
INDIRI4
RETI4
LABELV $126
endproc BotGetLongTermGoal 664 20
export BotLongTermGoal
proc BotLongTermGoal 448 20
line 1043
;1036:}
;1037:
;1038:/*
;1039:==================
;1040:BotLongTermGoal
;1041:==================
;1042:*/
;1043:int BotLongTermGoal(bot_state_t *bs, int tfl, int retreat, bot_goal_t *goal) {
line 1053
;1044:	aas_entityinfo_t entinfo;
;1045:	char teammate[MAX_MESSAGE_SIZE];
;1046:	float squaredist;
;1047:	int areanum;
;1048:	vec3_t dir;
;1049:
;1050:	//FIXME: also have air long term goals?
;1051:	//
;1052:	//if the bot is leading someone and not retreating
;1053:	if (bs->lead_time > 0 && !retreat) {
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
INDIRF4
CNSTF4 0
LEF4 $453
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $453
line 1054
;1054:		if (bs->lead_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $455
line 1055
;1055:			BotAI_BotInitialChat(bs, "lead_stop", EasyClientName(bs->lead_teammate, teammate, sizeof(teammate)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 416
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $457
ARGP4
ADDRLP4 416
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1056
;1056:			trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 420
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 420
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 420
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1057
;1057:			bs->lead_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6884
ADDP4
CNSTF4 0
ASGNF4
line 1058
;1058:			return BotGetLongTermGoal(bs, tfl, retreat, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 424
ADDRGP4 BotGetLongTermGoal
CALLI4
ASGNI4
ADDRLP4 424
INDIRI4
RETI4
ADDRGP4 $452
JUMPV
LABELV $455
line 1061
;1059:		}
;1060:		//
;1061:		if (bs->leadmessage_time < 0 && -bs->leadmessage_time < FloatTime()) {
ADDRLP4 416
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
INDIRF4
ASGNF4
ADDRLP4 416
INDIRF4
CNSTF4 0
GEF4 $458
ADDRLP4 416
INDIRF4
NEGF4
ADDRGP4 floattime
INDIRF4
GEF4 $458
line 1062
;1062:			BotAI_BotInitialChat(bs, "followme", EasyClientName(bs->lead_teammate, teammate, sizeof(teammate)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 420
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $460
ARGP4
ADDRLP4 420
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1063
;1063:			trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 424
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 424
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 424
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1064
;1064:			bs->leadmessage_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1065
;1065:		}
LABELV $458
line 1067
;1066:		//get entity information of the companion
;1067:		BotEntityInfo(bs->lead_teammate, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 1069
;1068:		//
;1069:		if (entinfo.valid) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $461
line 1070
;1070:			areanum = BotPointAreaNum(entinfo.origin);
ADDRLP4 12+24
ARGP4
ADDRLP4 420
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 412
ADDRLP4 420
INDIRI4
ASGNI4
line 1071
;1071:			if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 424
ADDRLP4 412
INDIRI4
ASGNI4
ADDRLP4 424
INDIRI4
CNSTI4 0
EQI4 $464
ADDRLP4 424
INDIRI4
ARGI4
ADDRLP4 428
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 428
INDIRI4
CNSTI4 0
EQI4 $464
line 1073
;1072:				//update team goal
;1073:				bs->lead_teamgoal.entitynum = bs->lead_teammate;
ADDRLP4 432
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 432
INDIRP4
CNSTI4 6868
ADDP4
ADDRLP4 432
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ASGNI4
line 1074
;1074:				bs->lead_teamgoal.areanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6840
ADDP4
ADDRLP4 412
INDIRI4
ASGNI4
line 1075
;1075:				VectorCopy(entinfo.origin, bs->lead_teamgoal.origin);
ADDRFP4 0
INDIRP4
CNSTI4 6828
ADDP4
ADDRLP4 12+24
INDIRB
ASGNB 12
line 1076
;1076:				VectorSet(bs->lead_teamgoal.mins, -8, -8, -8);
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
line 1077
;1077:				VectorSet(bs->lead_teamgoal.maxs, 8, 8, 8);
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
line 1078
;1078:			}
LABELV $464
line 1079
;1079:		}
LABELV $461
line 1081
;1080:		//if the team mate is visible
;1081:		if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->lead_teammate)) {
ADDRLP4 420
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 420
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 420
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 420
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 420
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 424
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 424
INDIRF4
CNSTF4 0
EQF4 $467
line 1082
;1082:			bs->leadvisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6888
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1083
;1083:		}
LABELV $467
line 1085
;1084:		//if the team mate is not visible for 1 seconds
;1085:		if (bs->leadvisible_time < FloatTime() - 1) {
ADDRFP4 0
INDIRP4
CNSTI4 6888
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
GEF4 $469
line 1086
;1086:			bs->leadbackup_time = FloatTime() + 2;
ADDRFP4 0
INDIRP4
CNSTI4 6896
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
ADDF4
ASGNF4
line 1087
;1087:		}
LABELV $469
line 1089
;1088:		//distance towards the team mate
;1089:		VectorSubtract(bs->origin, bs->lead_teamgoal.origin, dir);
ADDRLP4 428
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 428
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
ADDRLP4 428
INDIRP4
CNSTI4 6828
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 428
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
ADDRLP4 428
INDIRP4
CNSTI4 6832
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 432
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 432
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
ADDRLP4 432
INDIRP4
CNSTI4 6836
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1090
;1090:		squaredist = VectorLengthSquared(dir);
ADDRLP4 0
ARGP4
ADDRLP4 436
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 408
ADDRLP4 436
INDIRF4
ASGNF4
line 1092
;1091:		//if backing up towards the team mate
;1092:		if (bs->leadbackup_time > FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6896
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $473
line 1093
;1093:			if (bs->leadmessage_time < FloatTime() - 20) {
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1101004800
SUBF4
GEF4 $475
line 1094
;1094:				BotAI_BotInitialChat(bs, "followme", EasyClientName(bs->lead_teammate, teammate, sizeof(teammate)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 440
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $460
ARGP4
ADDRLP4 440
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1095
;1095:				trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 444
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 444
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 444
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1096
;1096:				bs->leadmessage_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1097
;1097:			}
LABELV $475
line 1099
;1098:			//if very close to the team mate
;1099:			if (squaredist < Square(100)) {
ADDRLP4 408
INDIRF4
CNSTF4 1176256512
GEF4 $477
line 1100
;1100:				bs->leadbackup_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6896
ADDP4
CNSTF4 0
ASGNF4
line 1101
;1101:			}
LABELV $477
line 1103
;1102:			//the bot should go back to the team mate
;1103:			memcpy(goal, &bs->lead_teamgoal, sizeof(bot_goal_t));
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6828
ADDP4
ARGP4
CNSTI4 56
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1104
;1104:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $452
JUMPV
LABELV $473
line 1106
;1105:		}
;1106:		else {
line 1108
;1107:			//if quite distant from the team mate
;1108:			if (squaredist > Square(500)) {
ADDRLP4 408
INDIRF4
CNSTF4 1215570944
LEF4 $479
line 1109
;1109:				if (bs->leadmessage_time < FloatTime() - 20) {
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1101004800
SUBF4
GEF4 $481
line 1110
;1110:					BotAI_BotInitialChat(bs, "followme", EasyClientName(bs->lead_teammate, teammate, sizeof(teammate)), NULL);
ADDRFP4 0
INDIRP4
CNSTI4 6824
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 440
ADDRGP4 EasyClientName
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $460
ARGP4
ADDRLP4 440
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 BotAI_BotInitialChat
CALLV
pop
line 1111
;1111:					trap_BotEnterChat(bs->cs, bs->teammate, CHAT_TELL);
ADDRLP4 444
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 444
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
ADDRLP4 444
INDIRP4
CNSTI4 6604
ADDP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1112
;1112:					bs->leadmessage_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6892
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1113
;1113:				}
LABELV $481
line 1115
;1114:				//look at the team mate
;1115:				VectorSubtract(entinfo.origin, bs->origin, dir);
ADDRLP4 440
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12+24
INDIRF4
ADDRLP4 440
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 12+24+4
INDIRF4
ADDRLP4 440
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
line 1116
;1116:				vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1117
;1117:				bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 444
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 444
INDIRP4
CNSTF4 1056964608
ADDRLP4 444
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1119
;1118:				//just wait for the team mate
;1119:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $452
JUMPV
LABELV $479
line 1121
;1120:			}
;1121:		}
line 1122
;1122:	}
LABELV $453
line 1123
;1123:	return BotGetLongTermGoal(bs, tfl, retreat, goal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 416
ADDRGP4 BotGetLongTermGoal
CALLI4
ASGNI4
ADDRLP4 416
INDIRI4
RETI4
LABELV $452
endproc BotLongTermGoal 448 20
export AIEnter_Intermission
proc AIEnter_Intermission 8 16
line 1131
;1124:}
;1125:
;1126:/*
;1127:==================
;1128:AIEnter_Intermission
;1129:==================
;1130:*/
;1131:void AIEnter_Intermission(bot_state_t *bs, char *s) {
line 1132
;1132:	BotRecordNodeSwitch(bs, "intermission", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $491
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1134
;1133:	//reset the bot state
;1134:	BotResetState(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotResetState
CALLV
pop
line 1136
;1135:	//check for end level chat
;1136:	if (BotChat_EndLevel(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotChat_EndLevel
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $492
line 1137
;1137:		trap_BotEnterChat(bs->cs, 0, bs->chatto);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 6052
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1138
;1138:	}
LABELV $492
line 1139
;1139:	bs->ainode = AINode_Intermission;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Intermission
ASGNP4
line 1140
;1140:}
LABELV $490
endproc AIEnter_Intermission 8 16
export AINode_Intermission
proc AINode_Intermission 16 8
line 1147
;1141:
;1142:/*
;1143:==================
;1144:AINode_Intermission
;1145:==================
;1146:*/
;1147:int AINode_Intermission(bot_state_t *bs) {
line 1149
;1148:	//if the intermission ended
;1149:	if (!BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $495
line 1150
;1150:		if (BotChat_StartLevel(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotChat_StartLevel
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $497
line 1151
;1151:			bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 12
INDIRF4
ADDF4
ASGNF4
line 1152
;1152:		}
ADDRGP4 $498
JUMPV
LABELV $497
line 1153
;1153:		else {
line 1154
;1154:			bs->stand_time = FloatTime() + 2;
ADDRFP4 0
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
ADDF4
ASGNF4
line 1155
;1155:		}
LABELV $498
line 1156
;1156:		AIEnter_Stand(bs, "intermission: chat");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $499
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 1157
;1157:	}
LABELV $495
line 1158
;1158:	return qtrue;
CNSTI4 1
RETI4
LABELV $494
endproc AINode_Intermission 16 8
export AIEnter_Observer
proc AIEnter_Observer 0 16
line 1166
;1159:}
;1160:
;1161:/*
;1162:==================
;1163:AIEnter_Observer
;1164:==================
;1165:*/
;1166:void AIEnter_Observer(bot_state_t *bs, char *s) {
line 1167
;1167:	BotRecordNodeSwitch(bs, "observer", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $501
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1169
;1168:	//reset the bot state
;1169:	BotResetState(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotResetState
CALLV
pop
line 1170
;1170:	bs->ainode = AINode_Observer;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Observer
ASGNP4
line 1171
;1171:}
LABELV $500
endproc AIEnter_Observer 0 16
export AINode_Observer
proc AINode_Observer 4 8
line 1178
;1172:
;1173:/*
;1174:==================
;1175:AINode_Observer
;1176:==================
;1177:*/
;1178:int AINode_Observer(bot_state_t *bs) {
line 1180
;1179:	//if the bot left observer mode
;1180:	if (!BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $503
line 1181
;1181:		AIEnter_Stand(bs, "observer: left observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $505
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 1182
;1182:	}
LABELV $503
line 1183
;1183:	return qtrue;
CNSTI4 1
RETI4
LABELV $502
endproc AINode_Observer 4 8
export AIEnter_Stand
proc AIEnter_Stand 0 16
line 1191
;1184:}
;1185:
;1186:/*
;1187:==================
;1188:AIEnter_Stand
;1189:==================
;1190:*/
;1191:void AIEnter_Stand(bot_state_t *bs, char *s) {
line 1192
;1192:	BotRecordNodeSwitch(bs, "stand", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $507
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1193
;1193:	bs->standfindenemy_time = FloatTime() + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6112
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 1194
;1194:	bs->ainode = AINode_Stand;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Stand
ASGNP4
line 1195
;1195:}
LABELV $506
endproc AIEnter_Stand 0 16
export AINode_Stand
proc AINode_Stand 24 12
line 1202
;1196:
;1197:/*
;1198:==================
;1199:AINode_Stand
;1200:==================
;1201:*/
;1202:int AINode_Stand(bot_state_t *bs) {
line 1205
;1203:
;1204:	//if the bot's health decreased
;1205:	if (bs->lastframe_health > bs->inventory[INVENTORY_HEALTH]) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 6044
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
LEI4 $509
line 1206
;1206:		if (BotChat_HitTalking(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 BotChat_HitTalking
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $511
line 1207
;1207:			bs->standfindenemy_time = FloatTime() + BotChatTime(bs) + 0.1;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 6112
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 12
INDIRF4
ADDF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 1208
;1208:			bs->stand_time = FloatTime() + BotChatTime(bs) + 0.1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 20
INDIRF4
ADDF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 1209
;1209:		}
LABELV $511
line 1210
;1210:	}
LABELV $509
line 1211
;1211:	if (bs->standfindenemy_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6112
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $513
line 1212
;1212:		if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 4
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $515
line 1213
;1213:			AIEnter_Battle_Fight(bs, "stand: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $517
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 1214
;1214:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $508
JUMPV
LABELV $515
line 1216
;1215:		}
;1216:		bs->standfindenemy_time = FloatTime() + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6112
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 1217
;1217:	}
LABELV $513
line 1219
;1218:	// put up chat icon
;1219:	trap_EA_Talk(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Talk
CALLV
pop
line 1221
;1220:	// when done standing
;1221:	if (bs->stand_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6096
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $518
line 1222
;1222:		trap_BotEnterChat(bs->cs, 0, bs->chatto);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 6052
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1223
;1223:		AIEnter_Seek_LTG(bs, "stand: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $520
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 1224
;1224:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $508
JUMPV
LABELV $518
line 1227
;1225:	}
;1226:	//
;1227:	return qtrue;
CNSTI4 1
RETI4
LABELV $508
endproc AINode_Stand 24 12
export AIEnter_Respawn
proc AIEnter_Respawn 12 16
line 1235
;1228:}
;1229:
;1230:/*
;1231:==================
;1232:AIEnter_Respawn
;1233:==================
;1234:*/
;1235:void AIEnter_Respawn(bot_state_t *bs, char *s) {
line 1236
;1236:	BotRecordNodeSwitch(bs, "respawn", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $522
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1238
;1237:	//reset some states
;1238:	trap_BotResetMoveState(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetMoveState
CALLV
pop
line 1239
;1239:	trap_BotResetGoalState(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetGoalState
CALLV
pop
line 1240
;1240:	trap_BotResetAvoidGoals(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidGoals
CALLV
pop
line 1241
;1241:	trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 1243
;1242:	//if the bot wants to chat
;1243:	if (BotChat_Death(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotChat_Death
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $523
line 1244
;1244:		bs->respawn_time = FloatTime() + BotChatTime(bs);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 6076
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 8
INDIRF4
ADDF4
ASGNF4
line 1245
;1245:		bs->respawnchat_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6080
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 1246
;1246:	}
ADDRGP4 $524
JUMPV
LABELV $523
line 1247
;1247:	else {
line 1248
;1248:		bs->respawn_time = FloatTime() + 1 + random();
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 6076
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDF4
ASGNF4
line 1249
;1249:		bs->respawnchat_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6080
ADDP4
CNSTF4 0
ASGNF4
line 1250
;1250:	}
LABELV $524
line 1252
;1251:	//set respawn state
;1252:	bs->respawn_wait = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 5984
ADDP4
CNSTI4 0
ASGNI4
line 1253
;1253:	bs->ainode = AINode_Respawn;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Respawn
ASGNP4
line 1254
;1254:}
LABELV $521
endproc AIEnter_Respawn 12 16
export AINode_Respawn
proc AINode_Respawn 4 12
line 1261
;1255:
;1256:/*
;1257:==================
;1258:AINode_Respawn
;1259:==================
;1260:*/
;1261:int AINode_Respawn(bot_state_t *bs) {
line 1263
;1262:	// if waiting for the actual respawn
;1263:	if (bs->respawn_wait) {
ADDRFP4 0
INDIRP4
CNSTI4 5984
ADDP4
INDIRI4
CNSTI4 0
EQI4 $526
line 1264
;1264:		if (!BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $528
line 1265
;1265:			AIEnter_Seek_LTG(bs, "respawn: respawned");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $530
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 1266
;1266:		}
ADDRGP4 $527
JUMPV
LABELV $528
line 1267
;1267:		else {
line 1268
;1268:			trap_EA_Respawn(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Respawn
CALLV
pop
line 1269
;1269:		}
line 1270
;1270:	}
ADDRGP4 $527
JUMPV
LABELV $526
line 1271
;1271:	else if (bs->respawn_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6076
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $531
line 1273
;1272:		// wait until respawned
;1273:		bs->respawn_wait = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 5984
ADDP4
CNSTI4 1
ASGNI4
line 1275
;1274:		// elementary action respawn
;1275:		trap_EA_Respawn(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Respawn
CALLV
pop
line 1277
;1276:		//
;1277:		if (bs->respawnchat_time) {
ADDRFP4 0
INDIRP4
CNSTI4 6080
ADDP4
INDIRF4
CNSTF4 0
EQF4 $533
line 1278
;1278:			trap_BotEnterChat(bs->cs, 0, bs->chatto);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 6532
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 6052
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEnterChat
CALLV
pop
line 1279
;1279:			bs->enemy = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
CNSTI4 -1
ASGNI4
line 1280
;1280:		}
LABELV $533
line 1281
;1281:	}
LABELV $531
LABELV $527
line 1282
;1282:	if (bs->respawnchat_time && bs->respawnchat_time < FloatTime() - 0.5) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6080
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 0
EQF4 $535
ADDRLP4 0
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1056964608
SUBF4
GEF4 $535
line 1283
;1283:		trap_EA_Talk(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Talk
CALLV
pop
line 1284
;1284:	}
LABELV $535
line 1286
;1285:	//
;1286:	return qtrue;
CNSTI4 1
RETI4
LABELV $525
endproc AINode_Respawn 4 12
export BotSelectActivateWeapon
proc BotSelectActivateWeapon 56 0
line 1294
;1287:}
;1288:
;1289:/*
;1290:==================
;1291:BotSelectActivateWeapon
;1292:==================
;1293:*/
;1294:int BotSelectActivateWeapon(bot_state_t *bs) {
line 1296
;1295:	//
;1296:	if (bs->inventory[INVENTORY_MACHINEGUN] > 0 && bs->inventory[INVENTORY_BULLETS] > 0)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 4976
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $538
ADDRLP4 0
INDIRP4
CNSTI4 5028
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $538
line 1297
;1297:		return WEAPONINDEX_MACHINEGUN;
CNSTI4 2
RETI4
ADDRGP4 $537
JUMPV
LABELV $538
line 1298
;1298:	else if (bs->inventory[INVENTORY_SHOTGUN] > 0 && bs->inventory[INVENTORY_SHELLS] > 0)
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 4972
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $540
ADDRLP4 8
INDIRP4
CNSTI4 5024
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $540
line 1299
;1299:		return WEAPONINDEX_SHOTGUN;
CNSTI4 3
RETI4
ADDRGP4 $537
JUMPV
LABELV $540
line 1300
;1300:	else if (bs->inventory[INVENTORY_PLASMAGUN] > 0 && bs->inventory[INVENTORY_CELLS] > 0)
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRP4
CNSTI4 4996
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
LEI4 $542
ADDRLP4 16
INDIRP4
CNSTI4 5036
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
LEI4 $542
line 1301
;1301:		return WEAPONINDEX_PLASMAGUN;
CNSTI4 8
RETI4
ADDRGP4 $537
JUMPV
LABELV $542
line 1302
;1302:	else if (bs->inventory[INVENTORY_LIGHTNING] > 0 && bs->inventory[INVENTORY_LIGHTNINGAMMO] > 0)
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRP4
CNSTI4 4988
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
LEI4 $544
ADDRLP4 24
INDIRP4
CNSTI4 5040
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
LEI4 $544
line 1303
;1303:		return WEAPONINDEX_LIGHTNING;
CNSTI4 6
RETI4
ADDRGP4 $537
JUMPV
LABELV $544
line 1310
;1304:#ifdef MISSIONPACK
;1305:	else if (bs->inventory[INVENTORY_CHAINGUN] > 0 && bs->inventory[INVENTORY_BELT] > 0)
;1306:		return WEAPONINDEX_CHAINGUN;
;1307:	else if (bs->inventory[INVENTORY_NAILGUN] > 0 && bs->inventory[INVENTORY_NAILS] > 0)
;1308:		return WEAPONINDEX_NAILGUN;
;1309:#endif
;1310:	else if (bs->inventory[INVENTORY_RAILGUN] > 0 && bs->inventory[INVENTORY_SLUGS] > 0)
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRP4
CNSTI4 4992
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
LEI4 $546
ADDRLP4 32
INDIRP4
CNSTI4 5048
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
LEI4 $546
line 1311
;1311:		return WEAPONINDEX_RAILGUN;
CNSTI4 7
RETI4
ADDRGP4 $537
JUMPV
LABELV $546
line 1312
;1312:	else if (bs->inventory[INVENTORY_ROCKETLAUNCHER] > 0 && bs->inventory[INVENTORY_ROCKETS] > 0)
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 40
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
LEI4 $548
ADDRLP4 40
INDIRP4
CNSTI4 5044
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
LEI4 $548
line 1313
;1313:		return WEAPONINDEX_ROCKET_LAUNCHER;
CNSTI4 5
RETI4
ADDRGP4 $537
JUMPV
LABELV $548
line 1314
;1314:	else if (bs->inventory[INVENTORY_BFG10K] > 0 && bs->inventory[INVENTORY_BFGAMMO] > 0)
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
CNSTI4 0
ASGNI4
ADDRLP4 48
INDIRP4
CNSTI4 5004
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
LEI4 $550
ADDRLP4 48
INDIRP4
CNSTI4 5052
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
LEI4 $550
line 1315
;1315:		return WEAPONINDEX_BFG;
CNSTI4 9
RETI4
ADDRGP4 $537
JUMPV
LABELV $550
line 1316
;1316:	else {
line 1317
;1317:		return -1;
CNSTI4 -1
RETI4
LABELV $537
endproc BotSelectActivateWeapon 56 0
export BotClearPath
proc BotClearPath 376 28
line 1328
;1318:	}
;1319:}
;1320:
;1321:/*
;1322:==================
;1323:BotClearPath
;1324:
;1325: try to deactivate obstacles like proximity mines on the bot's path
;1326:==================
;1327:*/
;1328:void BotClearPath(bot_state_t *bs, bot_moveresult_t *moveresult) {
line 1336
;1329:	int i, bestmine;
;1330:	float dist, bestdist;
;1331:	vec3_t target, dir;
;1332:	bsp_trace_t bsptrace;
;1333:	entityState_t state;
;1334:
;1335:	// if there is a dead body wearing kamikze nearby
;1336:	if (bs->kamikazebody) {
ADDRFP4 0
INDIRP4
CNSTI4 6256
ADDP4
INDIRI4
CNSTI4 0
EQI4 $553
line 1338
;1337:		// if the bot's view angles and weapon are not used for movement
;1338:		if ( !(moveresult->flags & (MOVERESULT_MOVEMENTVIEW | MOVERESULT_MOVEMENTWEAPON)) ) {
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 17
BANDI4
CNSTI4 0
NEI4 $555
line 1340
;1339:			//
;1340:			BotAI_GetEntityState(bs->kamikazebody, &state);
ADDRFP4 0
INDIRP4
CNSTI4 6256
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 BotAI_GetEntityState
CALLI4
pop
line 1341
;1341:			VectorCopy(state.pos.trBase, target);
ADDRLP4 236
ADDRLP4 12+12+12
INDIRB
ASGNB 12
line 1342
;1342:			target[2] += 8;
ADDRLP4 236+8
ADDRLP4 236+8
INDIRF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 1343
;1343:			VectorSubtract(target, bs->eye, dir);
ADDRLP4 332
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 236
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 236+4
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 236+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1344
;1344:			vectoangles(dir, moveresult->ideal_viewangles);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1346
;1345:			//
;1346:			moveresult->weapon = BotSelectActivateWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 336
ADDRGP4 BotSelectActivateWeapon
CALLI4
ASGNI4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 336
INDIRI4
ASGNI4
line 1347
;1347:			if (moveresult->weapon == -1) {
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $564
line 1349
;1348:				// FIXME: run away!
;1349:				moveresult->weapon = 0;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 0
ASGNI4
line 1350
;1350:			}
LABELV $564
line 1351
;1351:			if (moveresult->weapon) {
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 0
EQI4 $566
line 1353
;1352:				//
;1353:				moveresult->flags |= MOVERESULT_MOVEMENTWEAPON | MOVERESULT_MOVEMENTVIEW;
ADDRLP4 340
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 340
INDIRP4
ADDRLP4 340
INDIRP4
INDIRI4
CNSTI4 17
BORI4
ASGNI4
line 1355
;1354:				// if holding the right weapon
;1355:				if (bs->cur_ps.weapon == moveresult->weapon) {
ADDRFP4 0
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
NEI4 $568
line 1357
;1356:					// if the bot is pretty close with it's aim
;1357:					if (InFieldOfVision(bs->viewangles, 20, moveresult->ideal_viewangles)) {
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1101004800
ARGF4
ADDRFP4 4
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRLP4 344
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $570
line 1359
;1358:						//
;1359:						BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, target, bs->entitynum, MASK_SHOT);
ADDRLP4 248
ARGP4
ADDRLP4 348
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 348
INDIRP4
CNSTI4 4936
ADDP4
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
ADDRLP4 236
ARGP4
ADDRLP4 348
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
line 1361
;1360:						// if the mine is visible from the current position
;1361:						if (bsptrace.fraction >= 1.0 || bsptrace.ent == state.number) {
ADDRLP4 248+8
INDIRF4
CNSTF4 1065353216
GEF4 $576
ADDRLP4 248+80
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $572
LABELV $576
line 1363
;1362:							// shoot at the mine
;1363:							trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 1364
;1364:						}
LABELV $572
line 1365
;1365:					}
LABELV $570
line 1366
;1366:				}
LABELV $568
line 1367
;1367:			}
LABELV $566
line 1368
;1368:		}
LABELV $555
line 1369
;1369:	}
LABELV $553
line 1370
;1370:	if (moveresult->flags & MOVERESULT_BLOCKEDBYAVOIDSPOT) {
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $577
line 1371
;1371:		bs->blockedbyavoidspot_time = FloatTime() + 5;
ADDRFP4 0
INDIRP4
CNSTI4 6208
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1084227584
ADDF4
ASGNF4
line 1372
;1372:	}
LABELV $577
line 1374
;1373:	// if blocked by an avoid spot and the view angles and weapon are used for movement
;1374:	if (bs->blockedbyavoidspot_time > FloatTime() &&
ADDRFP4 0
INDIRP4
CNSTI4 6208
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
LEF4 $579
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 17
BANDI4
CNSTI4 0
NEI4 $579
line 1375
;1375:		!(moveresult->flags & (MOVERESULT_MOVEMENTVIEW | MOVERESULT_MOVEMENTWEAPON)) ) {
line 1376
;1376:		bestdist = 300;
ADDRLP4 228
CNSTF4 1133903872
ASGNF4
line 1377
;1377:		bestmine = -1;
ADDRLP4 232
CNSTI4 -1
ASGNI4
line 1378
;1378:		for (i = 0; i < bs->numproxmines; i++) {
ADDRLP4 220
CNSTI4 0
ASGNI4
ADDRGP4 $584
JUMPV
LABELV $581
line 1379
;1379:			BotAI_GetEntityState(bs->proxmines[i], &state);
ADDRLP4 220
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6260
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 BotAI_GetEntityState
CALLI4
pop
line 1380
;1380:			VectorSubtract(state.pos.trBase, bs->origin, dir);
ADDRLP4 332
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12+12+12
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 12+12+12+4
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 12+12+12+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1381
;1381:			dist = VectorLength(dir);
ADDRLP4 0
ARGP4
ADDRLP4 336
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 224
ADDRLP4 336
INDIRF4
ASGNF4
line 1382
;1382:			if (dist < bestdist) {
ADDRLP4 224
INDIRF4
ADDRLP4 228
INDIRF4
GEF4 $595
line 1383
;1383:				bestdist = dist;
ADDRLP4 228
ADDRLP4 224
INDIRF4
ASGNF4
line 1384
;1384:				bestmine = i;
ADDRLP4 232
ADDRLP4 220
INDIRI4
ASGNI4
line 1385
;1385:			}
LABELV $595
line 1386
;1386:		}
LABELV $582
line 1378
ADDRLP4 220
ADDRLP4 220
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $584
ADDRLP4 220
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 6516
ADDP4
INDIRI4
LTI4 $581
line 1387
;1387:		if (bestmine != -1) {
ADDRLP4 232
INDIRI4
CNSTI4 -1
EQI4 $597
line 1393
;1388:			//
;1389:			// state->generic1 == TEAM_RED || state->generic1 == TEAM_BLUE
;1390:			//
;1391:			// deactivate prox mines in the bot's path by shooting
;1392:			// rockets or plasma cells etc. at them
;1393:			BotAI_GetEntityState(bs->proxmines[bestmine], &state);
ADDRLP4 232
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 6260
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 BotAI_GetEntityState
CALLI4
pop
line 1394
;1394:			VectorCopy(state.pos.trBase, target);
ADDRLP4 236
ADDRLP4 12+12+12
INDIRB
ASGNB 12
line 1395
;1395:			target[2] += 2;
ADDRLP4 236+8
ADDRLP4 236+8
INDIRF4
CNSTF4 1073741824
ADDF4
ASGNF4
line 1396
;1396:			VectorSubtract(target, bs->eye, dir);
ADDRLP4 332
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 236
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 236+4
INDIRF4
ADDRLP4 332
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 236+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1397
;1397:			vectoangles(dir, moveresult->ideal_viewangles);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1399
;1398:			// if the bot has a weapon that does splash damage
;1399:			if (bs->inventory[INVENTORY_PLASMAGUN] > 0 && bs->inventory[INVENTORY_CELLS] > 0)
ADDRLP4 336
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 340
CNSTI4 0
ASGNI4
ADDRLP4 336
INDIRP4
CNSTI4 4996
ADDP4
INDIRI4
ADDRLP4 340
INDIRI4
LEI4 $606
ADDRLP4 336
INDIRP4
CNSTI4 5036
ADDP4
INDIRI4
ADDRLP4 340
INDIRI4
LEI4 $606
line 1400
;1400:				moveresult->weapon = WEAPONINDEX_PLASMAGUN;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 8
ASGNI4
ADDRGP4 $607
JUMPV
LABELV $606
line 1401
;1401:			else if (bs->inventory[INVENTORY_ROCKETLAUNCHER] > 0 && bs->inventory[INVENTORY_ROCKETS] > 0)
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 348
CNSTI4 0
ASGNI4
ADDRLP4 344
INDIRP4
CNSTI4 4984
ADDP4
INDIRI4
ADDRLP4 348
INDIRI4
LEI4 $608
ADDRLP4 344
INDIRP4
CNSTI4 5044
ADDP4
INDIRI4
ADDRLP4 348
INDIRI4
LEI4 $608
line 1402
;1402:				moveresult->weapon = WEAPONINDEX_ROCKET_LAUNCHER;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 5
ASGNI4
ADDRGP4 $609
JUMPV
LABELV $608
line 1403
;1403:			else if (bs->inventory[INVENTORY_BFG10K] > 0 && bs->inventory[INVENTORY_BFGAMMO] > 0)
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 356
CNSTI4 0
ASGNI4
ADDRLP4 352
INDIRP4
CNSTI4 5004
ADDP4
INDIRI4
ADDRLP4 356
INDIRI4
LEI4 $610
ADDRLP4 352
INDIRP4
CNSTI4 5052
ADDP4
INDIRI4
ADDRLP4 356
INDIRI4
LEI4 $610
line 1404
;1404:				moveresult->weapon = WEAPONINDEX_BFG;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 9
ASGNI4
ADDRGP4 $611
JUMPV
LABELV $610
line 1405
;1405:			else {
line 1406
;1406:				moveresult->weapon = 0;
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 0
ASGNI4
line 1407
;1407:			}
LABELV $611
LABELV $609
LABELV $607
line 1408
;1408:			if (moveresult->weapon) {
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 0
EQI4 $612
line 1410
;1409:				//
;1410:				moveresult->flags |= MOVERESULT_MOVEMENTWEAPON | MOVERESULT_MOVEMENTVIEW;
ADDRLP4 360
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 360
INDIRP4
ADDRLP4 360
INDIRP4
INDIRI4
CNSTI4 17
BORI4
ASGNI4
line 1412
;1411:				// if holding the right weapon
;1412:				if (bs->cur_ps.weapon == moveresult->weapon) {
ADDRFP4 0
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
NEI4 $614
line 1414
;1413:					// if the bot is pretty close with it's aim
;1414:					if (InFieldOfVision(bs->viewangles, 20, moveresult->ideal_viewangles)) {
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1101004800
ARGF4
ADDRFP4 4
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRLP4 364
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 364
INDIRI4
CNSTI4 0
EQI4 $616
line 1416
;1415:						//
;1416:						BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, target, bs->entitynum, MASK_SHOT);
ADDRLP4 248
ARGP4
ADDRLP4 368
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 368
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 372
CNSTP4 0
ASGNP4
ADDRLP4 372
INDIRP4
ARGP4
ADDRLP4 372
INDIRP4
ARGP4
ADDRLP4 236
ARGP4
ADDRLP4 368
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
line 1418
;1417:						// if the mine is visible from the current position
;1418:						if (bsptrace.fraction >= 1.0 || bsptrace.ent == state.number) {
ADDRLP4 248+8
INDIRF4
CNSTF4 1065353216
GEF4 $622
ADDRLP4 248+80
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $618
LABELV $622
line 1420
;1419:							// shoot at the mine
;1420:							trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 1421
;1421:						}
LABELV $618
line 1422
;1422:					}
LABELV $616
line 1423
;1423:				}
LABELV $614
line 1424
;1424:			}
LABELV $612
line 1425
;1425:		}
LABELV $597
line 1426
;1426:	}
LABELV $579
line 1427
;1427:}
LABELV $552
endproc BotClearPath 376 28
export AIEnter_Seek_ActivateEntity
proc AIEnter_Seek_ActivateEntity 0 16
line 1434
;1428:
;1429:/*
;1430:==================
;1431:AIEnter_Seek_ActivateEntity
;1432:==================
;1433:*/
;1434:void AIEnter_Seek_ActivateEntity(bot_state_t *bs, char *s) {
line 1435
;1435:	BotRecordNodeSwitch(bs, "activate entity", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $624
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1436
;1436:	bs->ainode = AINode_Seek_ActivateEntity;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Seek_ActivateEntity
ASGNP4
line 1437
;1437:}
LABELV $623
endproc AIEnter_Seek_ActivateEntity 0 16
export AINode_Seek_ActivateEntity
proc AINode_Seek_ActivateEntity 372 28
line 1444
;1438:
;1439:/*
;1440:==================
;1441:AINode_Seek_Activate_Entity
;1442:==================
;1443:*/
;1444:int AINode_Seek_ActivateEntity(bot_state_t *bs) {
line 1452
;1445:	bot_goal_t *goal;
;1446:	vec3_t target, dir, ideal_viewangles;
;1447:	bot_moveresult_t moveresult;
;1448:	int targetvisible;
;1449:	bsp_trace_t bsptrace;
;1450:	aas_entityinfo_t entinfo;
;1451:
;1452:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 320
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 320
INDIRI4
CNSTI4 0
EQI4 $626
line 1453
;1453:		BotClearActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1454
;1454:		AIEnter_Observer(bs, "active entity: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $628
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 1455
;1455:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $626
line 1458
;1456:	}
;1457:	//if in the intermission
;1458:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 324
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 324
INDIRI4
CNSTI4 0
EQI4 $629
line 1459
;1459:		BotClearActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1460
;1460:		AIEnter_Intermission(bs, "activate entity: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $631
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 1461
;1461:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $629
line 1464
;1462:	}
;1463:	//respawn if dead
;1464:	if (BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 328
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 328
INDIRI4
CNSTI4 0
EQI4 $632
line 1465
;1465:		BotClearActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1466
;1466:		AIEnter_Respawn(bs, "activate entity: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $634
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 1467
;1467:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $632
line 1470
;1468:	}
;1469:	//
;1470:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 1471
;1471:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $635
ADDRLP4 332
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 332
INDIRP4
ADDRLP4 332
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $635
line 1473
;1472:	// if in lava or slime the bot should be able to get out
;1473:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 336
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 336
INDIRI4
CNSTI4 0
EQI4 $638
ADDRLP4 340
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 340
INDIRP4
ADDRLP4 340
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $638
line 1475
;1474:	// map specific code
;1475:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 1477
;1476:	// no enemy
;1477:	bs->enemy = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
CNSTI4 -1
ASGNI4
line 1479
;1478:	// if the bot has no activate goal
;1479:	if (!bs->activatestack) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $640
line 1480
;1480:		BotClearActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1481
;1481:		AIEnter_Seek_NBG(bs, "activate entity: no goal");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $642
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 1482
;1482:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $640
line 1485
;1483:	}
;1484:	//
;1485:	goal = &bs->activatestack->goal;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
line 1487
;1486:	// initialize target being visible to false
;1487:	targetvisible = qfalse;
ADDRLP4 68
CNSTI4 0
ASGNI4
line 1489
;1488:	// if the bot has to shoot at a target to activate something
;1489:	if (bs->activatestack->shoot) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
EQI4 $643
line 1491
;1490:		//
;1491:		BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, bs->activatestack->target, bs->entitynum, MASK_SHOT);
ADDRLP4 72
ARGP4
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 348
CNSTP4 0
ASGNP4
ADDRLP4 348
INDIRP4
ARGP4
ADDRLP4 348
INDIRP4
ARGP4
ADDRLP4 344
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 80
ADDP4
ARGP4
ADDRLP4 344
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
line 1493
;1492:		// if the shootable entity is visible from the current position
;1493:		if (bsptrace.fraction >= 1.0 || bsptrace.ent == goal->entitynum) {
ADDRLP4 72+8
INDIRF4
CNSTF4 1065353216
GEF4 $649
ADDRLP4 72+80
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
NEI4 $645
LABELV $649
line 1494
;1494:			targetvisible = qtrue;
ADDRLP4 68
CNSTI4 1
ASGNI4
line 1496
;1495:			// if holding the right weapon
;1496:			if (bs->cur_ps.weapon == bs->activatestack->weapon) {
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 352
INDIRP4
CNSTI4 160
ADDP4
INDIRI4
ADDRLP4 352
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
NEI4 $650
line 1497
;1497:				VectorSubtract(bs->activatestack->target, bs->eye, dir);
ADDRLP4 356
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 360
ADDRLP4 356
INDIRP4
CNSTI4 7116
ADDP4
ASGNP4
ADDRLP4 56
ADDRLP4 360
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
ADDRLP4 356
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 360
INDIRP4
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
ADDRLP4 356
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 364
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56+8
ADDRLP4 364
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 88
ADDP4
INDIRF4
ADDRLP4 364
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1498
;1498:				vectoangles(dir, ideal_viewangles);
ADDRLP4 56
ARGP4
ADDRLP4 308
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1500
;1499:				// if the bot is pretty close with it's aim
;1500:				if (InFieldOfVision(bs->viewangles, 20, ideal_viewangles)) {
ADDRFP4 0
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1101004800
ARGF4
ADDRLP4 308
ARGP4
ADDRLP4 368
ADDRGP4 InFieldOfVision
CALLI4
ASGNI4
ADDRLP4 368
INDIRI4
CNSTI4 0
EQI4 $654
line 1501
;1501:					trap_EA_Attack(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Attack
CALLV
pop
line 1502
;1502:				}
LABELV $654
line 1503
;1503:			}
LABELV $650
line 1504
;1504:		}
LABELV $645
line 1505
;1505:	}
LABELV $643
line 1507
;1506:	// if the shoot target is visible
;1507:	if (targetvisible) {
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $656
line 1509
;1508:		// get the entity info of the entity the bot is shooting at
;1509:		BotEntityInfo(goal->entitynum, &entinfo);
ADDRLP4 52
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 156
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 1511
;1510:		// if the entity the bot shoots at moved
;1511:		if (!VectorCompare(bs->activatestack->origin, entinfo.origin)) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 156+24
ARGP4
ADDRLP4 344
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
NEI4 $658
line 1515
;1512:#ifdef DEBUG
;1513:			BotAI_Print(PRT_MESSAGE, "hit shootable button or trigger\n");
;1514:#endif //DEBUG
;1515:			bs->activatestack->time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 1516
;1516:		}
LABELV $658
line 1518
;1517:		// if the activate goal has been activated or the bot takes too long
;1518:		if (bs->activatestack->time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $661
line 1519
;1519:			BotPopFromActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotPopFromActivateGoalStack
CALLI4
pop
line 1521
;1520:			// if there are more activate goals on the stack
;1521:			if (bs->activatestack) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $663
line 1522
;1522:				bs->activatestack->time = FloatTime() + 10;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
ADDF4
ASGNF4
line 1523
;1523:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $663
line 1525
;1524:			}
;1525:			AIEnter_Seek_NBG(bs, "activate entity: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $665
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 1526
;1526:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $661
line 1528
;1527:		}
;1528:		memset(&moveresult, 0, sizeof(bot_moveresult_t));
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 52
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1529
;1529:	}
ADDRGP4 $657
JUMPV
LABELV $656
line 1530
;1530:	else {
line 1532
;1531:		// if the bot has no goal
;1532:		if (!goal) {
ADDRLP4 52
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $666
line 1533
;1533:			bs->activatestack->time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 1534
;1534:		}
ADDRGP4 $667
JUMPV
LABELV $666
line 1536
;1535:		// if the bot does not have a shoot goal
;1536:		else if (!bs->activatestack->shoot) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
NEI4 $668
line 1538
;1537:			//if the bot touches the current goal
;1538:			if (trap_BotTouchingGoal(bs->origin, goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 344
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $670
line 1542
;1539:#ifdef DEBUG
;1540:				BotAI_Print(PRT_MESSAGE, "touched button or trigger\n");
;1541:#endif //DEBUG
;1542:				bs->activatestack->time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 1543
;1543:			}
LABELV $670
line 1544
;1544:		}
LABELV $668
LABELV $667
line 1546
;1545:		// if the activate goal has been activated or the bot takes too long
;1546:		if (bs->activatestack->time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $672
line 1547
;1547:			BotPopFromActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotPopFromActivateGoalStack
CALLI4
pop
line 1549
;1548:			// if there are more activate goals on the stack
;1549:			if (bs->activatestack) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $674
line 1550
;1550:				bs->activatestack->time = FloatTime() + 10;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
ADDF4
ASGNF4
line 1551
;1551:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $674
line 1553
;1552:			}
;1553:			AIEnter_Seek_NBG(bs, "activate entity: activated");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $676
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 1554
;1554:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $672
line 1557
;1555:		}
;1556:		//predict obstacles
;1557:		if (BotAIPredictObstacles(bs, goal))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 344
ADDRGP4 BotAIPredictObstacles
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $677
line 1558
;1558:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $625
JUMPV
LABELV $677
line 1560
;1559:		//initialize the movement state
;1560:		BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 1562
;1561:		//move towards the goal
;1562:		trap_BotMoveToGoal(&moveresult, bs->ms, goal, bs->tfl);
ADDRLP4 0
ARGP4
ADDRLP4 348
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 348
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 348
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 1564
;1563:		//if the movement failed
;1564:		if (moveresult.failure) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $679
line 1566
;1565:			//reset the avoid reach, otherwise bot is stuck in current area
;1566:			trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 1568
;1567:			//
;1568:			bs->activatestack->time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 60
ADDP4
CNSTF4 0
ASGNF4
line 1569
;1569:		}
LABELV $679
line 1571
;1570:		//check if the bot is blocked
;1571:		BotAIBlocked(bs, &moveresult, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 1572
;1572:	}
LABELV $657
line 1574
;1573:	//
;1574:	BotClearPath(bs, &moveresult);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotClearPath
CALLV
pop
line 1576
;1575:	// if the bot has to shoot to activate
;1576:	if (bs->activatestack->shoot) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
EQI4 $681
line 1578
;1577:		// if the view angles aren't yet used for the movement
;1578:		if (!(moveresult.flags & MOVERESULT_MOVEMENTVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $683
line 1579
;1579:			VectorSubtract(bs->activatestack->target, bs->eye, dir);
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 348
ADDRLP4 344
INDIRP4
CNSTI4 7116
ADDP4
ASGNP4
ADDRLP4 56
ADDRLP4 348
INDIRP4
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
ADDRLP4 344
INDIRP4
CNSTI4 4936
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 348
INDIRP4
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
ADDRLP4 344
INDIRP4
CNSTI4 4940
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56+8
ADDRLP4 352
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 88
ADDP4
INDIRF4
ADDRLP4 352
INDIRP4
CNSTI4 4944
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1580
;1580:			vectoangles(dir, moveresult.ideal_viewangles);
ADDRLP4 56
ARGP4
ADDRLP4 0+40
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1581
;1581:			moveresult.flags |= MOVERESULT_MOVEMENTVIEW;
ADDRLP4 0+20
ADDRLP4 0+20
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1582
;1582:		}
LABELV $683
line 1584
;1583:		// if there's no weapon yet used for the movement
;1584:		if (!(moveresult.flags & MOVERESULT_MOVEMENTWEAPON)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $690
line 1585
;1585:			moveresult.flags |= MOVERESULT_MOVEMENTWEAPON;
ADDRLP4 0+20
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1587
;1586:			//
;1587:			bs->activatestack->weapon = BotSelectActivateWeapon(bs);
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
ARGP4
ADDRLP4 348
ADDRGP4 BotSelectActivateWeapon
CALLI4
ASGNI4
ADDRLP4 344
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 348
INDIRI4
ASGNI4
line 1588
;1588:			if (bs->activatestack->weapon == -1) {
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $694
line 1590
;1589:				//FIXME: find a decent weapon first
;1590:				bs->activatestack->weapon = 0;
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 76
ADDP4
CNSTI4 0
ASGNI4
line 1591
;1591:			}
LABELV $694
line 1592
;1592:			moveresult.weapon = bs->activatestack->weapon;
ADDRLP4 0+24
ADDRFP4 0
INDIRP4
CNSTI4 7116
ADDP4
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
ASGNI4
line 1593
;1593:		}
LABELV $690
line 1594
;1594:	}
LABELV $681
line 1596
;1595:	// if the ideal view angles are set for movement
;1596:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET|MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 11
BANDI4
CNSTI4 0
EQI4 $697
line 1597
;1597:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 0+40
INDIRB
ASGNB 12
line 1598
;1598:	}
ADDRGP4 $698
JUMPV
LABELV $697
line 1600
;1599:	// if waiting for something
;1600:	else if (moveresult.flags & MOVERESULT_WAITING) {
ADDRLP4 0+20
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $701
line 1601
;1601:		if (random() < bs->thinktime * 0.8) {
ADDRLP4 344
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $702
line 1602
;1602:			BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 296
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 1603
;1603:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 348
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 296
INDIRF4
ADDRLP4 348
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 296+4
INDIRF4
ADDRLP4 348
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 296+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1604
;1604:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 56
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1605
;1605:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 352
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 352
INDIRP4
CNSTF4 1056964608
ADDRLP4 352
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1606
;1606:		}
line 1607
;1607:	}
ADDRGP4 $702
JUMPV
LABELV $701
line 1608
;1608:	else if (!(bs->flags & BFL_IDEALVIEWSET)) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $710
line 1609
;1609:		if (trap_BotMovementViewTarget(bs->ms, goal, bs->tfl, 300, target)) {
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 344
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 296
ARGP4
ADDRLP4 348
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 348
INDIRI4
CNSTI4 0
EQI4 $712
line 1610
;1610:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 296
INDIRF4
ADDRLP4 352
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 296+4
INDIRF4
ADDRLP4 352
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+8
ADDRLP4 296+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1611
;1611:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 56
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1612
;1612:		}
ADDRGP4 $713
JUMPV
LABELV $712
line 1613
;1613:		else {
line 1614
;1614:			vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 0+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1615
;1615:		}
LABELV $713
line 1616
;1616:		bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 352
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 352
INDIRP4
CNSTF4 1056964608
ADDRLP4 352
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1617
;1617:	}
LABELV $710
LABELV $702
LABELV $698
line 1619
;1618:	// if the weapon is used for the bot movement
;1619:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON)
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $719
line 1620
;1620:		bs->weaponnum = moveresult.weapon;
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0+24
INDIRI4
ASGNI4
LABELV $719
line 1622
;1621:	// if there is an enemy
;1622:	if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 344
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $723
line 1623
;1623:		if (BotWantsToRetreat(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 348
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 348
INDIRI4
CNSTI4 0
EQI4 $725
line 1625
;1624:			//keep the current long term goal and retreat
;1625:			AIEnter_Battle_NBG(bs, "activate entity: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $727
ARGP4
ADDRGP4 AIEnter_Battle_NBG
CALLV
pop
line 1626
;1626:		}
ADDRGP4 $726
JUMPV
LABELV $725
line 1627
;1627:		else {
line 1628
;1628:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1630
;1629:			//empty the goal stack
;1630:			trap_BotEmptyGoalStack(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEmptyGoalStack
CALLV
pop
line 1632
;1631:			//go fight
;1632:			AIEnter_Battle_Fight(bs, "activate entity: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $727
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 1633
;1633:		}
LABELV $726
line 1634
;1634:		BotClearActivateGoalStack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotClearActivateGoalStack
CALLV
pop
line 1635
;1635:	}
LABELV $723
line 1636
;1636:	return qtrue;
CNSTI4 1
RETI4
LABELV $625
endproc AINode_Seek_ActivateEntity 372 28
export AIEnter_Seek_NBG
proc AIEnter_Seek_NBG 204 16
line 1644
;1637:}
;1638:
;1639:/*
;1640:==================
;1641:AIEnter_Seek_NBG
;1642:==================
;1643:*/
;1644:void AIEnter_Seek_NBG(bot_state_t *bs, char *s) {
line 1648
;1645:	bot_goal_t goal;
;1646:	char buf[144];
;1647:
;1648:	if (trap_BotGetTopGoal(bs->gs, &goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 200
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 0
EQI4 $729
line 1649
;1649:		trap_BotGoalName(goal.number, buf, 144);
ADDRLP4 0+44
INDIRI4
ARGI4
ADDRLP4 56
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 1650
;1650:		BotRecordNodeSwitch(bs, "seek NBG", buf, s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $732
ARGP4
ADDRLP4 56
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1651
;1651:	}
ADDRGP4 $730
JUMPV
LABELV $729
line 1652
;1652:	else {
line 1653
;1653:		BotRecordNodeSwitch(bs, "seek NBG", "no goal", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $732
ARGP4
ADDRGP4 $733
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1654
;1654:	}
LABELV $730
line 1655
;1655:	bs->ainode = AINode_Seek_NBG;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Seek_NBG
ASGNP4
line 1656
;1656:}
LABELV $728
endproc AIEnter_Seek_NBG 204 16
export AINode_Seek_NBG
proc AINode_Seek_NBG 192 20
line 1663
;1657:
;1658:/*
;1659:==================
;1660:AINode_Seek_NBG
;1661:==================
;1662:*/
;1663:int AINode_Seek_NBG(bot_state_t *bs) {
line 1668
;1664:	bot_goal_t goal;
;1665:	vec3_t target, dir;
;1666:	bot_moveresult_t moveresult;
;1667:
;1668:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 132
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 0
EQI4 $735
line 1669
;1669:		AIEnter_Observer(bs, "seek nbg: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $737
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 1670
;1670:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $734
JUMPV
LABELV $735
line 1673
;1671:	}
;1672:	//if in the intermission
;1673:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 136
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 0
EQI4 $738
line 1674
;1674:		AIEnter_Intermission(bs, "seek nbg: intermision");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $740
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 1675
;1675:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $734
JUMPV
LABELV $738
line 1678
;1676:	}
;1677:	//respawn if dead
;1678:	if (BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $741
line 1679
;1679:		AIEnter_Respawn(bs, "seek nbg: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $743
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 1680
;1680:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $734
JUMPV
LABELV $741
line 1683
;1681:	}
;1682:	//
;1683:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 1684
;1684:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $744
ADDRLP4 144
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 144
INDIRP4
ADDRLP4 144
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $744
line 1686
;1685:	//if in lava or slime the bot should be able to get out
;1686:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 148
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $747
ADDRLP4 152
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 152
INDIRP4
ADDRLP4 152
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $747
line 1688
;1687:	//
;1688:	if (BotCanAndWantsToRocketJump(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 BotCanAndWantsToRocketJump
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 0
EQI4 $749
line 1689
;1689:		bs->tfl |= TFL_ROCKETJUMP;
ADDRLP4 160
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 160
INDIRP4
ADDRLP4 160
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1690
;1690:	}
LABELV $749
line 1692
;1691:	//map specific code
;1692:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 1694
;1693:	//no enemy
;1694:	bs->enemy = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
CNSTI4 -1
ASGNI4
line 1696
;1695:	//if the bot has no goal
;1696:	if (!trap_BotGetTopGoal(bs->gs, &goal)) bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 160
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
NEI4 $751
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
ADDRGP4 $752
JUMPV
LABELV $751
line 1698
;1697:	//if the bot touches the current goal
;1698:	else if (BotReachedGoal(bs, &goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ARGP4
ADDRLP4 164
ADDRGP4 BotReachedGoal
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
EQI4 $753
line 1699
;1699:		BotChooseWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChooseWeapon
CALLV
pop
line 1700
;1700:		bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
line 1701
;1701:	}
LABELV $753
LABELV $752
line 1703
;1702:	//
;1703:	if (bs->nbg_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $755
line 1705
;1704:		//pop the current goal from the stack
;1705:		trap_BotPopGoal(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotPopGoal
CALLV
pop
line 1708
;1706:		//check for new nearby items right away
;1707:		//NOTE: we canNOT reset the check_time to zero because it would create an endless loop of node switches
;1708:		bs->check_time = FloatTime() + 0.05;
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1028443341
ADDF4
ASGNF4
line 1710
;1709:		//go back to seek ltg
;1710:		AIEnter_Seek_LTG(bs, "seek nbg: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $757
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 1711
;1711:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $734
JUMPV
LABELV $755
line 1714
;1712:	}
;1713:	//predict obstacles
;1714:	if (BotAIPredictObstacles(bs, &goal))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ARGP4
ADDRLP4 168
ADDRGP4 BotAIPredictObstacles
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 0
EQI4 $758
line 1715
;1715:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $734
JUMPV
LABELV $758
line 1717
;1716:	//initialize the movement state
;1717:	BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 1719
;1718:	//move towards the goal
;1719:	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
ADDRLP4 0
ARGP4
ADDRLP4 172
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 172
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 172
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 1721
;1720:	//if the movement failed
;1721:	if (moveresult.failure) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $760
line 1723
;1722:		//reset the avoid reach, otherwise bot is stuck in current area
;1723:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 1724
;1724:		bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
line 1725
;1725:	}
LABELV $760
line 1727
;1726:	//check if the bot is blocked
;1727:	BotAIBlocked(bs, &moveresult, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 1729
;1728:	//
;1729:	BotClearPath(bs, &moveresult);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotClearPath
CALLV
pop
line 1731
;1730:	//if the viewangles are used for the movement
;1731:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET|MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 11
BANDI4
CNSTI4 0
EQI4 $762
line 1732
;1732:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 0+40
INDIRB
ASGNB 12
line 1733
;1733:	}
ADDRGP4 $763
JUMPV
LABELV $762
line 1735
;1734:	//if waiting for something
;1735:	else if (moveresult.flags & MOVERESULT_WAITING) {
ADDRLP4 0+20
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $766
line 1736
;1736:		if (random() < bs->thinktime * 0.8) {
ADDRLP4 176
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $767
line 1737
;1737:			BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 108
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 1738
;1738:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 120
ADDRLP4 108
INDIRF4
ADDRLP4 180
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 120+4
ADDRLP4 108+4
INDIRF4
ADDRLP4 180
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 120+8
ADDRLP4 108+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1739
;1739:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 120
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1740
;1740:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 184
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 184
INDIRP4
CNSTF4 1056964608
ADDRLP4 184
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1741
;1741:		}
line 1742
;1742:	}
ADDRGP4 $767
JUMPV
LABELV $766
line 1743
;1743:	else if (!(bs->flags & BFL_IDEALVIEWSET)) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $775
line 1744
;1744:		if (!trap_BotGetSecondGoal(bs->gs, &goal)) trap_BotGetTopGoal(bs->gs, &goal);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 176
ADDRGP4 trap_BotGetSecondGoal
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 0
NEI4 $777
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRGP4 trap_BotGetTopGoal
CALLI4
pop
LABELV $777
line 1745
;1745:		if (trap_BotMovementViewTarget(bs->ms, &goal, bs->tfl, 300, target)) {
ADDRLP4 180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 180
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 180
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 108
ARGP4
ADDRLP4 184
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 184
INDIRI4
CNSTI4 0
EQI4 $779
line 1746
;1746:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 120
ADDRLP4 108
INDIRF4
ADDRLP4 188
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 120+4
ADDRLP4 108+4
INDIRF4
ADDRLP4 188
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 120+8
ADDRLP4 108+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1747
;1747:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 120
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1748
;1748:		}
ADDRGP4 $780
JUMPV
LABELV $779
line 1750
;1749:		//FIXME: look at cluster portals?
;1750:		else vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 0+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
LABELV $780
line 1751
;1751:		bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 188
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 188
INDIRP4
CNSTF4 1056964608
ADDRLP4 188
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1752
;1752:	}
LABELV $775
LABELV $767
LABELV $763
line 1754
;1753:	//if the weapon is used for the bot movement
;1754:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs->weaponnum = moveresult.weapon;
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $786
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0+24
INDIRI4
ASGNI4
LABELV $786
line 1756
;1755:	//if there is an enemy
;1756:	if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 176
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 0
EQI4 $790
line 1757
;1757:		if (BotWantsToRetreat(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 180
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 0
EQI4 $792
line 1759
;1758:			//keep the current long term goal and retreat
;1759:			AIEnter_Battle_NBG(bs, "seek nbg: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $794
ARGP4
ADDRGP4 AIEnter_Battle_NBG
CALLV
pop
line 1760
;1760:		}
ADDRGP4 $793
JUMPV
LABELV $792
line 1761
;1761:		else {
line 1762
;1762:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1764
;1763:			//empty the goal stack
;1764:			trap_BotEmptyGoalStack(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEmptyGoalStack
CALLV
pop
line 1766
;1765:			//go fight
;1766:			AIEnter_Battle_Fight(bs, "seek nbg: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $794
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 1767
;1767:		}
LABELV $793
line 1768
;1768:	}
LABELV $790
line 1769
;1769:	return qtrue;
CNSTI4 1
RETI4
LABELV $734
endproc AINode_Seek_NBG 192 20
export AIEnter_Seek_LTG
proc AIEnter_Seek_LTG 204 16
line 1777
;1770:}
;1771:
;1772:/*
;1773:==================
;1774:AIEnter_Seek_LTG
;1775:==================
;1776:*/
;1777:void AIEnter_Seek_LTG(bot_state_t *bs, char *s) {
line 1781
;1778:	bot_goal_t goal;
;1779:	char buf[144];
;1780:
;1781:	if (trap_BotGetTopGoal(bs->gs, &goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 200
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 200
INDIRI4
CNSTI4 0
EQI4 $796
line 1782
;1782:		trap_BotGoalName(goal.number, buf, 144);
ADDRLP4 0+44
INDIRI4
ARGI4
ADDRLP4 56
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 trap_BotGoalName
CALLV
pop
line 1783
;1783:		BotRecordNodeSwitch(bs, "seek LTG", buf, s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $799
ARGP4
ADDRLP4 56
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1784
;1784:	}
ADDRGP4 $797
JUMPV
LABELV $796
line 1785
;1785:	else {
line 1786
;1786:		BotRecordNodeSwitch(bs, "seek LTG", "no goal", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $799
ARGP4
ADDRGP4 $733
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1787
;1787:	}
LABELV $797
line 1788
;1788:	bs->ainode = AINode_Seek_LTG;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Seek_LTG
ASGNP4
line 1789
;1789:}
LABELV $795
endproc AIEnter_Seek_LTG 204 16
export AINode_Seek_LTG
proc AINode_Seek_LTG 212 20
line 1797
;1790:
;1791:/*
;1792:==================
;1793:AINode_Seek_LTG
;1794:==================
;1795:*/
;1796:int AINode_Seek_LTG(bot_state_t *bs)
;1797:{
line 1805
;1798:	bot_goal_t goal;
;1799:	vec3_t target, dir;
;1800:	bot_moveresult_t moveresult;
;1801:	int range;
;1802:	//char buf[128];
;1803:	//bot_goal_t tmpgoal;
;1804:
;1805:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 136
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 0
EQI4 $801
line 1806
;1806:		AIEnter_Observer(bs, "seek ltg: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $803
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 1807
;1807:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $801
line 1810
;1808:	}
;1809:	//if in the intermission
;1810:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $804
line 1811
;1811:		AIEnter_Intermission(bs, "seek ltg: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $806
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 1812
;1812:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $804
line 1815
;1813:	}
;1814:	//respawn if dead
;1815:	if (BotIsDead(bs)) {
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
EQI4 $807
line 1816
;1816:		AIEnter_Respawn(bs, "seek ltg: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $809
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 1817
;1817:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $807
line 1820
;1818:	}
;1819:	//
;1820:	if (BotChat_Random(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 148
ADDRGP4 BotChat_Random
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
EQI4 $810
line 1821
;1821:		bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 152
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 152
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 156
INDIRF4
ADDF4
ASGNF4
line 1822
;1822:		AIEnter_Stand(bs, "seek ltg: random chat");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $812
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 1823
;1823:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $810
line 1826
;1824:	}
;1825:	//
;1826:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 1827
;1827:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $813
ADDRLP4 152
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 152
INDIRP4
ADDRLP4 152
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $813
line 1829
;1828:	//if in lava or slime the bot should be able to get out
;1829:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 0
EQI4 $816
ADDRLP4 160
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 160
INDIRP4
ADDRLP4 160
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $816
line 1831
;1830:	//
;1831:	if (BotCanAndWantsToRocketJump(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 164
ADDRGP4 BotCanAndWantsToRocketJump
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
EQI4 $818
line 1832
;1832:		bs->tfl |= TFL_ROCKETJUMP;
ADDRLP4 168
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 168
INDIRP4
ADDRLP4 168
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1833
;1833:	}
LABELV $818
line 1835
;1834:	//map specific code
;1835:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 1837
;1836:	//no enemy
;1837:	bs->enemy = -1;
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
CNSTI4 -1
ASGNI4
line 1839
;1838:	//
;1839:	if (bs->killedenemy_time > FloatTime() - 2) {
ADDRFP4 0
INDIRP4
CNSTI4 6168
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
LEF4 $820
line 1840
;1840:		if (random() < bs->thinktime * 1) {
ADDRLP4 168
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1065353216
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $822
line 1841
;1841:			trap_EA_Gesture(bs->client);
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_EA_Gesture
CALLV
pop
line 1842
;1842:		}
LABELV $822
line 1843
;1843:	}
LABELV $820
line 1845
;1844:	//if there is an enemy
;1845:	if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 168
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 0
EQI4 $824
line 1846
;1846:		if (BotWantsToRetreat(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 172
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 172
INDIRI4
CNSTI4 0
EQI4 $826
line 1848
;1847:			//keep the current long term goal and retreat
;1848:			AIEnter_Battle_Retreat(bs, "seek ltg: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $828
ARGP4
ADDRGP4 AIEnter_Battle_Retreat
CALLV
pop
line 1849
;1849:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $826
line 1851
;1850:		}
;1851:		else {
line 1852
;1852:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1854
;1853:			//empty the goal stack
;1854:			trap_BotEmptyGoalStack(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEmptyGoalStack
CALLV
pop
line 1856
;1855:			//go fight
;1856:			AIEnter_Battle_Fight(bs, "seek ltg: found enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $828
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 1857
;1857:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $824
line 1861
;1858:		}
;1859:	}
;1860:	//
;1861:	BotTeamGoals(bs, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotTeamGoals
CALLV
pop
line 1863
;1862:	//get the current long term goal
;1863:	if (!BotLongTermGoal(bs, bs->tfl, qfalse, &goal)) {
ADDRLP4 172
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 172
INDIRP4
ARGP4
ADDRLP4 172
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 176
ADDRGP4 BotLongTermGoal
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 0
NEI4 $829
line 1864
;1864:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $800
JUMPV
LABELV $829
line 1867
;1865:	}
;1866:	//check for nearby goals periodicly
;1867:	if (bs->check_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $831
line 1868
;1868:		bs->check_time = FloatTime() + 0.5;
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1056964608
ADDF4
ASGNF4
line 1870
;1869:		//check if the bot wants to camp
;1870:		BotWantsToCamp(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotWantsToCamp
CALLI4
pop
line 1872
;1871:		//
;1872:		if (bs->ltgtype == LTG_DEFENDKEYAREA) range = 400;
ADDRFP4 0
INDIRP4
CNSTI4 6600
ADDP4
INDIRI4
CNSTI4 3
NEI4 $833
ADDRLP4 108
CNSTI4 400
ASGNI4
ADDRGP4 $834
JUMPV
LABELV $833
line 1873
;1873:		else range = 150;
ADDRLP4 108
CNSTI4 150
ASGNI4
LABELV $834
line 1876
;1874:		//
;1875:#ifdef CTF
;1876:		if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $835
line 1878
;1877:			//if carrying a flag the bot shouldn't be distracted too much
;1878:			if (BotCTFCarryingFlag(bs))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 180
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 0
EQI4 $837
line 1879
;1879:				range = 50;
ADDRLP4 108
CNSTI4 50
ASGNI4
LABELV $837
line 1880
;1880:		}
LABELV $835
line 1893
;1881:#endif //CTF
;1882:#ifdef MISSIONPACK
;1883:		else if (gametype == GT_1FCTF) {
;1884:			if (Bot1FCTFCarryingFlag(bs))
;1885:				range = 50;
;1886:		}
;1887:		else if (gametype == GT_HARVESTER) {
;1888:			if (BotHarvesterCarryingCubes(bs))
;1889:				range = 80;
;1890:		}
;1891:#endif
;1892:		//
;1893:		if (BotNearbyGoal(bs, bs->tfl, &goal, range)) {
ADDRLP4 180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 180
INDIRP4
ARGP4
ADDRLP4 180
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 108
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 184
ADDRGP4 BotNearbyGoal
CALLI4
ASGNI4
ADDRLP4 184
INDIRI4
CNSTI4 0
EQI4 $839
line 1894
;1894:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1900
;1895:			//get the goal at the top of the stack
;1896:			//trap_BotGetTopGoal(bs->gs, &tmpgoal);
;1897:			//trap_BotGoalName(tmpgoal.number, buf, 144);
;1898:			//BotAI_Print(PRT_MESSAGE, "new nearby goal %s\n", buf);
;1899:			//time the bot gets to pick up the nearby goal item
;1900:			bs->nbg_time = FloatTime() + 4 + range * 0.01;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1082130432
ADDF4
CNSTF4 1008981770
ADDRLP4 108
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
line 1901
;1901:			AIEnter_Seek_NBG(bs, "ltg seek: nbg");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $841
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 1902
;1902:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $839
line 1904
;1903:		}
;1904:	}
LABELV $831
line 1906
;1905:	//predict obstacles
;1906:	if (BotAIPredictObstacles(bs, &goal))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ARGP4
ADDRLP4 180
ADDRGP4 BotAIPredictObstacles
CALLI4
ASGNI4
ADDRLP4 180
INDIRI4
CNSTI4 0
EQI4 $842
line 1907
;1907:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $800
JUMPV
LABELV $842
line 1909
;1908:	//initialize the movement state
;1909:	BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 1911
;1910:	//move towards the goal
;1911:	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
ADDRLP4 0
ARGP4
ADDRLP4 184
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 184
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 184
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 1913
;1912:	//if the movement failed
;1913:	if (moveresult.failure) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $844
line 1915
;1914:		//reset the avoid reach, otherwise bot is stuck in current area
;1915:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 1917
;1916:		//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
;1917:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 1918
;1918:	}
LABELV $844
line 1920
;1919:	//
;1920:	BotAIBlocked(bs, &moveresult, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 1922
;1921:	//
;1922:	BotClearPath(bs, &moveresult);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 BotClearPath
CALLV
pop
line 1924
;1923:	//if the viewangles are used for the movement
;1924:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET|MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 11
BANDI4
CNSTI4 0
EQI4 $846
line 1925
;1925:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 0+40
INDIRB
ASGNB 12
line 1926
;1926:	}
ADDRGP4 $847
JUMPV
LABELV $846
line 1928
;1927:	//if waiting for something
;1928:	else if (moveresult.flags & MOVERESULT_WAITING) {
ADDRLP4 0+20
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $850
line 1929
;1929:		if (random() < bs->thinktime * 0.8) {
ADDRLP4 188
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $851
line 1930
;1930:			BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 112
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 1931
;1931:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
ADDRLP4 112
INDIRF4
ADDRLP4 192
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 112+4
INDIRF4
ADDRLP4 192
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 112+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1932
;1932:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 124
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1933
;1933:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 196
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 196
INDIRP4
CNSTF4 1056964608
ADDRLP4 196
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1934
;1934:		}
line 1935
;1935:	}
ADDRGP4 $851
JUMPV
LABELV $850
line 1936
;1936:	else if (!(bs->flags & BFL_IDEALVIEWSET)) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $859
line 1937
;1937:		if (trap_BotMovementViewTarget(bs->ms, &goal, bs->tfl, 300, target)) {
ADDRLP4 188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 188
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 188
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 112
ARGP4
ADDRLP4 192
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 0
EQI4 $861
line 1938
;1938:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 196
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
ADDRLP4 112
INDIRF4
ADDRLP4 196
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 112+4
INDIRF4
ADDRLP4 196
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 112+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1939
;1939:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 124
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1940
;1940:		}
ADDRGP4 $862
JUMPV
LABELV $861
line 1942
;1941:		//FIXME: look at cluster portals?
;1942:		else if (VectorLengthSquared(moveresult.movedir)) {
ADDRLP4 0+28
ARGP4
ADDRLP4 196
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 196
INDIRF4
CNSTF4 0
EQF4 $867
line 1943
;1943:			vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 0+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1944
;1944:		}
ADDRGP4 $868
JUMPV
LABELV $867
line 1945
;1945:		else if (random() < bs->thinktime * 0.8) {
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
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 4904
ADDP4
INDIRF4
MULF4
GEF4 $871
line 1946
;1946:			BotRoamGoal(bs, target);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 112
ARGP4
ADDRGP4 BotRoamGoal
CALLV
pop
line 1947
;1947:			VectorSubtract(target, bs->origin, dir);
ADDRLP4 204
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
ADDRLP4 112
INDIRF4
ADDRLP4 204
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 112+4
INDIRF4
ADDRLP4 204
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 112+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1948
;1948:			vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 124
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1949
;1949:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 208
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 208
INDIRP4
CNSTF4 1056964608
ADDRLP4 208
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1950
;1950:		}
LABELV $871
LABELV $868
LABELV $862
line 1951
;1951:		bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 204
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 204
INDIRP4
CNSTF4 1056964608
ADDRLP4 204
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1952
;1952:	}
LABELV $859
LABELV $851
LABELV $847
line 1954
;1953:	//if the weapon is used for the bot movement
;1954:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs->weaponnum = moveresult.weapon;
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $877
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0+24
INDIRI4
ASGNI4
LABELV $877
line 1956
;1955:	//
;1956:	return qtrue;
CNSTI4 1
RETI4
LABELV $800
endproc AINode_Seek_LTG 212 20
export AIEnter_Battle_Fight
proc AIEnter_Battle_Fight 0 16
line 1964
;1957:}
;1958:
;1959:/*
;1960:==================
;1961:AIEnter_Battle_Fight
;1962:==================
;1963:*/
;1964:void AIEnter_Battle_Fight(bot_state_t *bs, char *s) {
line 1965
;1965:	BotRecordNodeSwitch(bs, "battle fight", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $882
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1966
;1966:	trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1967
;1967:	bs->ainode = AINode_Battle_Fight;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Battle_Fight
ASGNP4
line 1968
;1968:}
LABELV $881
endproc AIEnter_Battle_Fight 0 16
export AIEnter_Battle_SuicidalFight
proc AIEnter_Battle_SuicidalFight 4 16
line 1975
;1969:
;1970:/*
;1971:==================
;1972:AIEnter_Battle_Fight
;1973:==================
;1974:*/
;1975:void AIEnter_Battle_SuicidalFight(bot_state_t *bs, char *s) {
line 1976
;1976:	BotRecordNodeSwitch(bs, "battle fight", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $882
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 1977
;1977:	trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 1978
;1978:	bs->ainode = AINode_Battle_Fight;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Battle_Fight
ASGNP4
line 1979
;1979:	bs->flags |= BFL_FIGHTSUICIDAL;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1980
;1980:}
LABELV $883
endproc AIEnter_Battle_SuicidalFight 4 16
export AINode_Battle_Fight
proc AINode_Battle_Fight 292 20
line 1987
;1981:
;1982:/*
;1983:==================
;1984:AINode_Battle_Fight
;1985:==================
;1986:*/
;1987:int AINode_Battle_Fight(bot_state_t *bs) {
line 1993
;1988:	int areanum;
;1989:	vec3_t target;
;1990:	aas_entityinfo_t entinfo;
;1991:	bot_moveresult_t moveresult;
;1992:
;1993:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 208
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 208
INDIRI4
CNSTI4 0
EQI4 $885
line 1994
;1994:		AIEnter_Observer(bs, "battle fight: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $887
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 1995
;1995:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $885
line 1999
;1996:	}
;1997:
;1998:	//if in the intermission
;1999:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 212
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 212
INDIRI4
CNSTI4 0
EQI4 $888
line 2000
;2000:		AIEnter_Intermission(bs, "battle fight: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $890
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 2001
;2001:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $888
line 2004
;2002:	}
;2003:	//respawn if dead
;2004:	if (BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 216
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 216
INDIRI4
CNSTI4 0
EQI4 $891
line 2005
;2005:		AIEnter_Respawn(bs, "battle fight: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $893
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 2006
;2006:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $891
line 2009
;2007:	}
;2008:	//if there is another better enemy
;2009:	if (BotFindEnemy(bs, bs->enemy)) {
ADDRLP4 220
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 220
INDIRP4
ARGP4
ADDRLP4 220
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 224
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 224
INDIRI4
CNSTI4 0
EQI4 $894
line 2013
;2010:#ifdef DEBUG
;2011:		BotAI_Print(PRT_MESSAGE, "found new better enemy\n");
;2012:#endif
;2013:	}
LABELV $894
line 2015
;2014:	//if no enemy
;2015:	if (bs->enemy < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
GEI4 $896
line 2016
;2016:		AIEnter_Seek_LTG(bs, "battle fight: no enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $898
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2017
;2017:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $896
line 2020
;2018:	}
;2019:	//
;2020:	BotEntityInfo(bs->enemy, &entinfo);
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
line 2022
;2021:	//if the enemy is dead
;2022:	if (bs->enemydeath_time) {
ADDRFP4 0
INDIRP4
CNSTI4 6136
ADDP4
INDIRF4
CNSTF4 0
EQF4 $899
line 2023
;2023:		if (bs->enemydeath_time < FloatTime() - 1.0) {
ADDRFP4 0
INDIRP4
CNSTI4 6136
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
SUBF4
GEF4 $900
line 2024
;2024:			bs->enemydeath_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6136
ADDP4
CNSTF4 0
ASGNF4
line 2025
;2025:			if (bs->enemysuicide) {
ADDRFP4 0
INDIRP4
CNSTI4 6012
ADDP4
INDIRI4
CNSTI4 0
EQI4 $903
line 2026
;2026:				BotChat_EnemySuicide(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChat_EnemySuicide
CALLI4
pop
line 2027
;2027:			}
LABELV $903
line 2028
;2028:			if (bs->lastkilledplayer == bs->enemy && BotChat_Kill(bs)) {
ADDRLP4 228
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 228
INDIRP4
CNSTI4 5992
ADDP4
INDIRI4
ADDRLP4 228
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
NEI4 $905
ADDRLP4 228
INDIRP4
ARGP4
ADDRLP4 232
ADDRGP4 BotChat_Kill
CALLI4
ASGNI4
ADDRLP4 232
INDIRI4
CNSTI4 0
EQI4 $905
line 2029
;2029:				bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 236
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 236
INDIRP4
ARGP4
ADDRLP4 240
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 236
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 240
INDIRF4
ADDF4
ASGNF4
line 2030
;2030:				AIEnter_Stand(bs, "battle fight: enemy dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $907
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 2031
;2031:			}
ADDRGP4 $906
JUMPV
LABELV $905
line 2032
;2032:			else {
line 2033
;2033:				bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 2034
;2034:				AIEnter_Seek_LTG(bs, "battle fight: enemy dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $907
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2035
;2035:			}
LABELV $906
line 2036
;2036:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
line 2038
;2037:		}
;2038:	}
LABELV $899
line 2039
;2039:	else {
line 2040
;2040:		if (EntityIsDead(&entinfo)) {
ADDRLP4 0
ARGP4
ADDRLP4 228
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 228
INDIRI4
CNSTI4 0
EQI4 $908
line 2041
;2041:			bs->enemydeath_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6136
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2042
;2042:		}
LABELV $908
line 2043
;2043:	}
LABELV $900
line 2045
;2044:	//if the enemy is invisible and not shooting the bot looses track easily
;2045:	if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
ADDRLP4 0
ARGP4
ADDRLP4 228
ADDRGP4 EntityIsInvisible
CALLI4
ASGNI4
ADDRLP4 228
INDIRI4
CNSTI4 0
EQI4 $910
ADDRLP4 0
ARGP4
ADDRLP4 232
ADDRGP4 EntityIsShooting
CALLI4
ASGNI4
ADDRLP4 232
INDIRI4
CNSTI4 0
NEI4 $910
line 2046
;2046:		if (random() < 0.2) {
ADDRLP4 236
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 236
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1045220557
GEF4 $912
line 2047
;2047:			AIEnter_Seek_LTG(bs, "battle fight: invisible");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $914
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2048
;2048:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $912
line 2050
;2049:		}
;2050:	}
LABELV $910
line 2052
;2051:	//
;2052:	VectorCopy(entinfo.origin, target);
ADDRLP4 196
ADDRLP4 0+24
INDIRB
ASGNB 12
line 2054
;2053:	// if not a player enemy
;2054:	if (bs->enemy >= MAX_CLIENTS) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 64
LTI4 $916
line 2062
;2055:#ifdef MISSIONPACK
;2056:		// if attacking an obelisk
;2057:		if ( bs->enemy == redobelisk.entitynum ||
;2058:			bs->enemy == blueobelisk.entitynum ) {
;2059:			target[2] += 16;
;2060:		}
;2061:#endif
;2062:	}
LABELV $916
line 2064
;2063:	//update the reachability area and origin if possible
;2064:	areanum = BotPointAreaNum(target);
ADDRLP4 196
ARGP4
ADDRLP4 236
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 192
ADDRLP4 236
INDIRI4
ASGNI4
line 2065
;2065:	if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 192
INDIRI4
CNSTI4 0
EQI4 $918
ADDRLP4 192
INDIRI4
ARGI4
ADDRLP4 244
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 244
INDIRI4
CNSTI4 0
EQI4 $918
line 2066
;2066:		VectorCopy(target, bs->lastenemyorigin);
ADDRFP4 0
INDIRP4
CNSTI4 6548
ADDP4
ADDRLP4 196
INDIRB
ASGNB 12
line 2067
;2067:		bs->lastenemyareanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6544
ADDP4
ADDRLP4 192
INDIRI4
ASGNI4
line 2068
;2068:	}
LABELV $918
line 2070
;2069:	//update the attack inventory values
;2070:	BotUpdateBattleInventory(bs, bs->enemy);
ADDRLP4 248
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 248
INDIRP4
ARGP4
ADDRLP4 248
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotUpdateBattleInventory
CALLV
pop
line 2072
;2071:	//if the bot's health decreased
;2072:	if (bs->lastframe_health > bs->inventory[INVENTORY_HEALTH]) {
ADDRLP4 252
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 252
INDIRP4
CNSTI4 6044
ADDP4
INDIRI4
ADDRLP4 252
INDIRP4
CNSTI4 5068
ADDP4
INDIRI4
LEI4 $920
line 2073
;2073:		if (BotChat_HitNoDeath(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 256
ADDRGP4 BotChat_HitNoDeath
CALLI4
ASGNI4
ADDRLP4 256
INDIRI4
CNSTI4 0
EQI4 $922
line 2074
;2074:			bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 260
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
INDIRP4
ARGP4
ADDRLP4 264
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 260
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 264
INDIRF4
ADDF4
ASGNF4
line 2075
;2075:			AIEnter_Stand(bs, "battle fight: chat health decreased");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $924
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 2076
;2076:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $922
line 2078
;2077:		}
;2078:	}
LABELV $920
line 2080
;2079:	//if the bot hit someone
;2080:	if (bs->cur_ps.persistant[PERS_HITS] > bs->lasthitcount) {
ADDRLP4 256
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 256
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ADDRLP4 256
INDIRP4
CNSTI4 6048
ADDP4
INDIRI4
LEI4 $925
line 2081
;2081:		if (BotChat_HitNoKill(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 260
ADDRGP4 BotChat_HitNoKill
CALLI4
ASGNI4
ADDRLP4 260
INDIRI4
CNSTI4 0
EQI4 $927
line 2082
;2082:			bs->stand_time = FloatTime() + BotChatTime(bs);
ADDRLP4 264
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 264
INDIRP4
ARGP4
ADDRLP4 268
ADDRGP4 BotChatTime
CALLF4
ASGNF4
ADDRLP4 264
INDIRP4
CNSTI4 6096
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 268
INDIRF4
ADDF4
ASGNF4
line 2083
;2083:			AIEnter_Stand(bs, "battle fight: chat hit someone");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $929
ARGP4
ADDRGP4 AIEnter_Stand
CALLV
pop
line 2084
;2084:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $927
line 2086
;2085:		}
;2086:	}
LABELV $925
line 2088
;2087:	//if the enemy is not visible
;2088:	if (!BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy)) {
ADDRLP4 260
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 260
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 260
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 260
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 260
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 264
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 264
INDIRF4
CNSTF4 0
NEF4 $930
line 2089
;2089:		if (BotWantsToChase(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 268
ADDRGP4 BotWantsToChase
CALLI4
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 0
EQI4 $932
line 2090
;2090:			AIEnter_Battle_Chase(bs, "battle fight: enemy out of sight");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $934
ARGP4
ADDRGP4 AIEnter_Battle_Chase
CALLV
pop
line 2091
;2091:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $932
line 2093
;2092:		}
;2093:		else {
line 2094
;2094:			AIEnter_Seek_LTG(bs, "battle fight: enemy out of sight");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $934
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2095
;2095:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $884
JUMPV
LABELV $930
line 2099
;2096:		}
;2097:	}
;2098:	//use holdable items
;2099:	BotBattleUseItems(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotBattleUseItems
CALLV
pop
line 2101
;2100:	//
;2101:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 2102
;2102:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $935
ADDRLP4 268
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 268
INDIRP4
ADDRLP4 268
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $935
line 2104
;2103:	//if in lava or slime the bot should be able to get out
;2104:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 272
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 272
INDIRI4
CNSTI4 0
EQI4 $938
ADDRLP4 276
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 276
INDIRP4
ADDRLP4 276
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $938
line 2106
;2105:	//
;2106:	if (BotCanAndWantsToRocketJump(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 280
ADDRGP4 BotCanAndWantsToRocketJump
CALLI4
ASGNI4
ADDRLP4 280
INDIRI4
CNSTI4 0
EQI4 $940
line 2107
;2107:		bs->tfl |= TFL_ROCKETJUMP;
ADDRLP4 284
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 284
INDIRP4
ADDRLP4 284
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 2108
;2108:	}
LABELV $940
line 2110
;2109:	//choose the best weapon to fight with
;2110:	BotChooseWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChooseWeapon
CALLV
pop
line 2112
;2111:	//do attack movements
;2112:	moveresult = BotAttackMove(bs, bs->tfl);
ADDRLP4 140
ARGP4
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
ARGP4
ADDRLP4 284
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotAttackMove
CALLV
pop
line 2114
;2113:	//if the movement failed
;2114:	if (moveresult.failure) {
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $942
line 2116
;2115:		//reset the avoid reach, otherwise bot is stuck in current area
;2116:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 2118
;2117:		//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
;2118:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 2119
;2119:	}
LABELV $942
line 2121
;2120:	//
;2121:	BotAIBlocked(bs, &moveresult, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 2123
;2122:	//aim at the enemy
;2123:	BotAimAtEnemy(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotAimAtEnemy
CALLV
pop
line 2125
;2124:	//attack the enemy if possible
;2125:	BotCheckAttack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckAttack
CALLV
pop
line 2127
;2126:	//if the bot wants to retreat
;2127:	if (!(bs->flags & BFL_FIGHTSUICIDAL)) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
NEI4 $944
line 2128
;2128:		if (BotWantsToRetreat(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 288
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 288
INDIRI4
CNSTI4 0
EQI4 $946
line 2129
;2129:			AIEnter_Battle_Retreat(bs, "battle fight: wants to retreat");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $948
ARGP4
ADDRGP4 AIEnter_Battle_Retreat
CALLV
pop
line 2130
;2130:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $884
JUMPV
LABELV $946
line 2132
;2131:		}
;2132:	}
LABELV $944
line 2133
;2133:	return qtrue;
CNSTI4 1
RETI4
LABELV $884
endproc AINode_Battle_Fight 292 20
export AIEnter_Battle_Chase
proc AIEnter_Battle_Chase 0 16
line 2141
;2134:}
;2135:
;2136:/*
;2137:==================
;2138:AIEnter_Battle_Chase
;2139:==================
;2140:*/
;2141:void AIEnter_Battle_Chase(bot_state_t *bs, char *s) {
line 2142
;2142:	BotRecordNodeSwitch(bs, "battle chase", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $950
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 2143
;2143:	bs->chase_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6084
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2144
;2144:	bs->ainode = AINode_Battle_Chase;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Battle_Chase
ASGNP4
line 2145
;2145:}
LABELV $949
endproc AIEnter_Battle_Chase 0 16
export AINode_Battle_Chase
proc AINode_Battle_Chase 204 20
line 2153
;2146:
;2147:/*
;2148:==================
;2149:AINode_Battle_Chase
;2150:==================
;2151:*/
;2152:int AINode_Battle_Chase(bot_state_t *bs)
;2153:{
line 2159
;2154:	bot_goal_t goal;
;2155:	vec3_t target, dir;
;2156:	bot_moveresult_t moveresult;
;2157:	float range;
;2158:
;2159:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 136
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 0
EQI4 $952
line 2160
;2160:		AIEnter_Observer(bs, "battle chase: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $954
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 2161
;2161:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $952
line 2164
;2162:	}
;2163:	//if in the intermission
;2164:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 140
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
EQI4 $955
line 2165
;2165:		AIEnter_Intermission(bs, "battle chase: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $957
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 2166
;2166:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $955
line 2169
;2167:	}
;2168:	//respawn if dead
;2169:	if (BotIsDead(bs)) {
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
EQI4 $958
line 2170
;2170:		AIEnter_Respawn(bs, "battle chase: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $960
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 2171
;2171:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $958
line 2174
;2172:	}
;2173:	//if no enemy
;2174:	if (bs->enemy < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
GEI4 $961
line 2175
;2175:		AIEnter_Seek_LTG(bs, "battle chase: no enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $963
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2176
;2176:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $961
line 2179
;2177:	}
;2178:	//if the enemy is visible
;2179:	if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy)) {
ADDRLP4 148
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 148
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 148
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 148
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 148
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 152
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 152
INDIRF4
CNSTF4 0
EQF4 $964
line 2180
;2180:		AIEnter_Battle_Fight(bs, "battle chase");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $950
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 2181
;2181:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $964
line 2184
;2182:	}
;2183:	//if there is another enemy
;2184:	if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 156
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 156
INDIRI4
CNSTI4 0
EQI4 $966
line 2185
;2185:		AIEnter_Battle_Fight(bs, "battle chase: better enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $968
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 2186
;2186:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $966
line 2189
;2187:	}
;2188:	//there is no last enemy area
;2189:	if (!bs->lastenemyareanum) {
ADDRFP4 0
INDIRP4
CNSTI4 6544
ADDP4
INDIRI4
CNSTI4 0
NEI4 $969
line 2190
;2190:		AIEnter_Seek_LTG(bs, "battle chase: no enemy area");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $971
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2191
;2191:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $969
line 2194
;2192:	}
;2193:	//
;2194:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 2195
;2195:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $972
ADDRLP4 160
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 160
INDIRP4
ADDRLP4 160
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $972
line 2197
;2196:	//if in lava or slime the bot should be able to get out
;2197:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 164
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
EQI4 $975
ADDRLP4 168
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 168
INDIRP4
ADDRLP4 168
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $975
line 2199
;2198:	//
;2199:	if (BotCanAndWantsToRocketJump(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 172
ADDRGP4 BotCanAndWantsToRocketJump
CALLI4
ASGNI4
ADDRLP4 172
INDIRI4
CNSTI4 0
EQI4 $977
line 2200
;2200:		bs->tfl |= TFL_ROCKETJUMP;
ADDRLP4 176
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 176
INDIRP4
ADDRLP4 176
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 2201
;2201:	}
LABELV $977
line 2203
;2202:	//map specific code
;2203:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 2205
;2204:	//create the chase goal
;2205:	goal.entitynum = bs->enemy;
ADDRLP4 0+40
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ASGNI4
line 2206
;2206:	goal.areanum = bs->lastenemyareanum;
ADDRLP4 0+12
ADDRFP4 0
INDIRP4
CNSTI4 6544
ADDP4
INDIRI4
ASGNI4
line 2207
;2207:	VectorCopy(bs->lastenemyorigin, goal.origin);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 6548
ADDP4
INDIRB
ASGNB 12
line 2208
;2208:	VectorSet(goal.mins, -8, -8, -8);
ADDRLP4 0+16
CNSTF4 3238002688
ASGNF4
ADDRLP4 0+16+4
CNSTF4 3238002688
ASGNF4
ADDRLP4 0+16+8
CNSTF4 3238002688
ASGNF4
line 2209
;2209:	VectorSet(goal.maxs, 8, 8, 8);
ADDRLP4 0+28
CNSTF4 1090519040
ASGNF4
ADDRLP4 0+28+4
CNSTF4 1090519040
ASGNF4
ADDRLP4 0+28+8
CNSTF4 1090519040
ASGNF4
line 2211
;2210:	//if the last seen enemy spot is reached the enemy could not be found
;2211:	if (trap_BotTouchingGoal(bs->origin, &goal)) bs->chase_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4908
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 176
ADDRGP4 trap_BotTouchingGoal
CALLI4
ASGNI4
ADDRLP4 176
INDIRI4
CNSTI4 0
EQI4 $991
ADDRFP4 0
INDIRP4
CNSTI4 6084
ADDP4
CNSTF4 0
ASGNF4
LABELV $991
line 2213
;2212:	//if there's no chase time left
;2213:	if (!bs->chase_time || bs->chase_time < FloatTime() - 10) {
ADDRLP4 180
ADDRFP4 0
INDIRP4
CNSTI4 6084
ADDP4
INDIRF4
ASGNF4
ADDRLP4 180
INDIRF4
CNSTF4 0
EQF4 $995
ADDRLP4 180
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1092616192
SUBF4
GEF4 $993
LABELV $995
line 2214
;2214:		AIEnter_Seek_LTG(bs, "battle chase: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $996
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2215
;2215:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $993
line 2218
;2216:	}
;2217:	//check for nearby goals periodicly
;2218:	if (bs->check_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $997
line 2219
;2219:		bs->check_time = FloatTime() + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2220
;2220:		range = 150;
ADDRLP4 108
CNSTF4 1125515264
ASGNF4
line 2222
;2221:		//
;2222:		if (BotNearbyGoal(bs, bs->tfl, &goal, range)) {
ADDRLP4 184
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 184
INDIRP4
ARGP4
ADDRLP4 184
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 108
INDIRF4
ARGF4
ADDRLP4 188
ADDRGP4 BotNearbyGoal
CALLI4
ASGNI4
ADDRLP4 188
INDIRI4
CNSTI4 0
EQI4 $999
line 2224
;2223:			//the bot gets 5 seconds to pick up the nearby goal item
;2224:			bs->nbg_time = FloatTime() + 0.1 * range + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1036831949
ADDRLP4 108
INDIRF4
MULF4
ADDF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2225
;2225:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 2226
;2226:			AIEnter_Battle_NBG(bs, "battle chase: nbg");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1001
ARGP4
ADDRGP4 AIEnter_Battle_NBG
CALLV
pop
line 2227
;2227:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $951
JUMPV
LABELV $999
line 2229
;2228:		}
;2229:	}
LABELV $997
line 2231
;2230:	//
;2231:	BotUpdateBattleInventory(bs, bs->enemy);
ADDRLP4 184
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 184
INDIRP4
ARGP4
ADDRLP4 184
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotUpdateBattleInventory
CALLV
pop
line 2233
;2232:	//initialize the movement state
;2233:	BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 2235
;2234:	//move towards the goal
;2235:	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
ADDRLP4 56
ARGP4
ADDRLP4 188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 188
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 188
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 2237
;2236:	//if the movement failed
;2237:	if (moveresult.failure) {
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $1002
line 2239
;2238:		//reset the avoid reach, otherwise bot is stuck in current area
;2239:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 2241
;2240:		//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
;2241:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 2242
;2242:	}
LABELV $1002
line 2244
;2243:	//
;2244:	BotAIBlocked(bs, &moveresult, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 56
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 2246
;2245:	//
;2246:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEWSET|MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 56+20
INDIRI4
CNSTI4 11
BANDI4
CNSTI4 0
EQI4 $1004
line 2247
;2247:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 56+40
INDIRB
ASGNB 12
line 2248
;2248:	}
ADDRGP4 $1005
JUMPV
LABELV $1004
line 2249
;2249:	else if (!(bs->flags & BFL_IDEALVIEWSET)) {
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $1008
line 2250
;2250:		if (bs->chase_time > FloatTime() - 2) {
ADDRFP4 0
INDIRP4
CNSTI4 6084
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1073741824
SUBF4
LEF4 $1010
line 2251
;2251:			BotAimAtEnemy(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotAimAtEnemy
CALLV
pop
line 2252
;2252:		}
ADDRGP4 $1011
JUMPV
LABELV $1010
line 2253
;2253:		else {
line 2254
;2254:			if (trap_BotMovementViewTarget(bs->ms, &goal, bs->tfl, 300, target)) {
ADDRLP4 192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 192
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 192
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 112
ARGP4
ADDRLP4 196
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 0
EQI4 $1012
line 2255
;2255:				VectorSubtract(target, bs->origin, dir);
ADDRLP4 200
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
ADDRLP4 112
INDIRF4
ADDRLP4 200
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+4
ADDRLP4 112+4
INDIRF4
ADDRLP4 200
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 124+8
ADDRLP4 112+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2256
;2256:				vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 124
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2257
;2257:			}
ADDRGP4 $1013
JUMPV
LABELV $1012
line 2258
;2258:			else {
line 2259
;2259:				vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 56+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2260
;2260:			}
LABELV $1013
line 2261
;2261:		}
LABELV $1011
line 2262
;2262:		bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 192
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 192
INDIRP4
CNSTF4 1056964608
ADDRLP4 192
INDIRP4
INDIRF4
MULF4
ASGNF4
line 2263
;2263:	}
LABELV $1008
LABELV $1005
line 2265
;2264:	//if the weapon is used for the bot movement
;2265:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs->weaponnum = moveresult.weapon;
ADDRLP4 56+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $1019
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 56+24
INDIRI4
ASGNI4
LABELV $1019
line 2267
;2266:	//if the bot is in the area the enemy was last seen in
;2267:	if (bs->areanum == bs->lastenemyareanum) bs->chase_time = 0;
ADDRLP4 192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 192
INDIRP4
CNSTI4 4948
ADDP4
INDIRI4
ADDRLP4 192
INDIRP4
CNSTI4 6544
ADDP4
INDIRI4
NEI4 $1023
ADDRFP4 0
INDIRP4
CNSTI4 6084
ADDP4
CNSTF4 0
ASGNF4
LABELV $1023
line 2269
;2268:	//if the bot wants to retreat (the bot could have been damage during the chase)
;2269:	if (BotWantsToRetreat(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 196
ADDRGP4 BotWantsToRetreat
CALLI4
ASGNI4
ADDRLP4 196
INDIRI4
CNSTI4 0
EQI4 $1025
line 2270
;2270:		AIEnter_Battle_Retreat(bs, "battle chase: wants to retreat");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1027
ARGP4
ADDRGP4 AIEnter_Battle_Retreat
CALLV
pop
line 2271
;2271:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $951
JUMPV
LABELV $1025
line 2273
;2272:	}
;2273:	return qtrue;
CNSTI4 1
RETI4
LABELV $951
endproc AINode_Battle_Chase 204 20
export AIEnter_Battle_Retreat
proc AIEnter_Battle_Retreat 0 16
line 2281
;2274:}
;2275:
;2276:/*
;2277:==================
;2278:AIEnter_Battle_Retreat
;2279:==================
;2280:*/
;2281:void AIEnter_Battle_Retreat(bot_state_t *bs, char *s) {
line 2282
;2282:	BotRecordNodeSwitch(bs, "battle retreat", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1029
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 2283
;2283:	bs->ainode = AINode_Battle_Retreat;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Battle_Retreat
ASGNP4
line 2284
;2284:}
LABELV $1028
endproc AIEnter_Battle_Retreat 0 16
export AINode_Battle_Retreat
proc AINode_Battle_Retreat 368 20
line 2291
;2285:
;2286:/*
;2287:==================
;2288:AINode_Battle_Retreat
;2289:==================
;2290:*/
;2291:int AINode_Battle_Retreat(bot_state_t *bs) {
line 2299
;2292:	bot_goal_t goal;
;2293:	aas_entityinfo_t entinfo;
;2294:	bot_moveresult_t moveresult;
;2295:	vec3_t target, dir;
;2296:	float attack_skill, range;
;2297:	int areanum;
;2298:
;2299:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 284
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 284
INDIRI4
CNSTI4 0
EQI4 $1031
line 2300
;2300:		AIEnter_Observer(bs, "battle retreat: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1033
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 2301
;2301:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1031
line 2304
;2302:	}
;2303:	//if in the intermission
;2304:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 288
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 288
INDIRI4
CNSTI4 0
EQI4 $1034
line 2305
;2305:		AIEnter_Intermission(bs, "battle retreat: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1036
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 2306
;2306:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1034
line 2309
;2307:	}
;2308:	//respawn if dead
;2309:	if (BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 292
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 292
INDIRI4
CNSTI4 0
EQI4 $1037
line 2310
;2310:		AIEnter_Respawn(bs, "battle retreat: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1039
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 2311
;2311:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1037
line 2314
;2312:	}
;2313:	//if no enemy
;2314:	if (bs->enemy < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
GEI4 $1040
line 2315
;2315:		AIEnter_Seek_LTG(bs, "battle retreat: no enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1042
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2316
;2316:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1040
line 2319
;2317:	}
;2318:	//
;2319:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2320
;2320:	if (EntityIsDead(&entinfo)) {
ADDRLP4 108
ARGP4
ADDRLP4 296
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 296
INDIRI4
CNSTI4 0
EQI4 $1043
line 2321
;2321:		AIEnter_Seek_LTG(bs, "battle retreat: enemy dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1045
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2322
;2322:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1043
line 2325
;2323:	}
;2324:	//if there is another better enemy
;2325:	if (BotFindEnemy(bs, bs->enemy)) {
ADDRLP4 300
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 300
INDIRP4
ARGP4
ADDRLP4 300
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 304
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 304
INDIRI4
CNSTI4 0
EQI4 $1046
line 2329
;2326:#ifdef DEBUG
;2327:		BotAI_Print(PRT_MESSAGE, "found new better enemy\n");
;2328:#endif
;2329:	}
LABELV $1046
line 2331
;2330:	//
;2331:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 2332
;2332:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $1048
ADDRLP4 308
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 308
INDIRP4
ADDRLP4 308
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $1048
line 2334
;2333:	//if in lava or slime the bot should be able to get out
;2334:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 312
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 312
INDIRI4
CNSTI4 0
EQI4 $1051
ADDRLP4 316
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 316
INDIRP4
ADDRLP4 316
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $1051
line 2336
;2335:	//map specific code
;2336:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 2338
;2337:	//update the attack inventory values
;2338:	BotUpdateBattleInventory(bs, bs->enemy);
ADDRLP4 320
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 320
INDIRP4
ARGP4
ADDRLP4 320
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotUpdateBattleInventory
CALLV
pop
line 2340
;2339:	//if the bot doesn't want to retreat anymore... probably picked up some nice items
;2340:	if (BotWantsToChase(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 324
ADDRGP4 BotWantsToChase
CALLI4
ASGNI4
ADDRLP4 324
INDIRI4
CNSTI4 0
EQI4 $1053
line 2342
;2341:		//empty the goal stack, when chasing, only the enemy is the goal
;2342:		trap_BotEmptyGoalStack(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotEmptyGoalStack
CALLV
pop
line 2344
;2343:		//go chase the enemy
;2344:		AIEnter_Battle_Chase(bs, "battle retreat: wants to chase");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1055
ARGP4
ADDRGP4 AIEnter_Battle_Chase
CALLV
pop
line 2345
;2345:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1053
line 2348
;2346:	}
;2347:	//update the last time the enemy was visible
;2348:	if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy)) {
ADDRLP4 328
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 328
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 328
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 328
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 328
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 332
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 332
INDIRF4
CNSTF4 0
EQF4 $1056
line 2349
;2349:		bs->enemyvisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6088
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2350
;2350:		VectorCopy(entinfo.origin, target);
ADDRLP4 252
ADDRLP4 108+24
INDIRB
ASGNB 12
line 2352
;2351:		// if not a player enemy
;2352:		if (bs->enemy >= MAX_CLIENTS) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 64
LTI4 $1059
line 2360
;2353:#ifdef MISSIONPACK
;2354:			// if attacking an obelisk
;2355:			if ( bs->enemy == redobelisk.entitynum ||
;2356:				bs->enemy == blueobelisk.entitynum ) {
;2357:				target[2] += 16;
;2358:			}
;2359:#endif
;2360:		}
LABELV $1059
line 2362
;2361:		//update the reachability area and origin if possible
;2362:		areanum = BotPointAreaNum(target);
ADDRLP4 252
ARGP4
ADDRLP4 336
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 248
ADDRLP4 336
INDIRI4
ASGNI4
line 2363
;2363:		if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 340
ADDRLP4 248
INDIRI4
ASGNI4
ADDRLP4 340
INDIRI4
CNSTI4 0
EQI4 $1061
ADDRLP4 340
INDIRI4
ARGI4
ADDRLP4 344
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $1061
line 2364
;2364:			VectorCopy(target, bs->lastenemyorigin);
ADDRFP4 0
INDIRP4
CNSTI4 6548
ADDP4
ADDRLP4 252
INDIRB
ASGNB 12
line 2365
;2365:			bs->lastenemyareanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6544
ADDP4
ADDRLP4 248
INDIRI4
ASGNI4
line 2366
;2366:		}
LABELV $1061
line 2367
;2367:	}
LABELV $1056
line 2369
;2368:	//if the enemy is NOT visible for 4 seconds
;2369:	if (bs->enemyvisible_time < FloatTime() - 4) {
ADDRFP4 0
INDIRP4
CNSTI4 6088
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
CNSTF4 1082130432
SUBF4
GEF4 $1063
line 2370
;2370:		AIEnter_Seek_LTG(bs, "battle retreat: lost enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1065
ARGP4
ADDRGP4 AIEnter_Seek_LTG
CALLV
pop
line 2371
;2371:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1063
line 2374
;2372:	}
;2373:	//else if the enemy is NOT visible
;2374:	else if (bs->enemyvisible_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6088
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1066
line 2376
;2375:		//if there is another enemy
;2376:		if (BotFindEnemy(bs, -1)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 336
ADDRGP4 BotFindEnemy
CALLI4
ASGNI4
ADDRLP4 336
INDIRI4
CNSTI4 0
EQI4 $1068
line 2377
;2377:			AIEnter_Battle_Fight(bs, "battle retreat: another enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1070
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
line 2378
;2378:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1068
line 2380
;2379:		}
;2380:	}
LABELV $1066
line 2382
;2381:	//
;2382:	BotTeamGoals(bs, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BotTeamGoals
CALLV
pop
line 2384
;2383:	//use holdable items
;2384:	BotBattleUseItems(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotBattleUseItems
CALLV
pop
line 2386
;2385:	//get the current long term goal while retreating
;2386:	if (!BotLongTermGoal(bs, bs->tfl, qtrue, &goal)) {
ADDRLP4 336
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 336
INDIRP4
ARGP4
ADDRLP4 336
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 340
ADDRGP4 BotLongTermGoal
CALLI4
ASGNI4
ADDRLP4 340
INDIRI4
CNSTI4 0
NEI4 $1071
line 2387
;2387:		AIEnter_Battle_SuicidalFight(bs, "battle retreat: no way out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1073
ARGP4
ADDRGP4 AIEnter_Battle_SuicidalFight
CALLV
pop
line 2388
;2388:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1071
line 2391
;2389:	}
;2390:	//check for nearby goals periodicly
;2391:	if (bs->check_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1074
line 2392
;2392:		bs->check_time = FloatTime() + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6092
ADDP4
ADDRGP4 floattime
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2393
;2393:		range = 150;
ADDRLP4 264
CNSTF4 1125515264
ASGNF4
line 2395
;2394:#ifdef CTF
;2395:		if (gametype == GT_CTF) {
ADDRGP4 gametype
INDIRI4
CNSTI4 4
NEI4 $1076
line 2397
;2396:			//if carrying a flag the bot shouldn't be distracted too much
;2397:			if (BotCTFCarryingFlag(bs))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 344
ADDRGP4 BotCTFCarryingFlag
CALLI4
ASGNI4
ADDRLP4 344
INDIRI4
CNSTI4 0
EQI4 $1078
line 2398
;2398:				range = 50;
ADDRLP4 264
CNSTF4 1112014848
ASGNF4
LABELV $1078
line 2399
;2399:		}
LABELV $1076
line 2412
;2400:#endif //CTF
;2401:#ifdef MISSIONPACK
;2402:		else if (gametype == GT_1FCTF) {
;2403:			if (Bot1FCTFCarryingFlag(bs))
;2404:				range = 50;
;2405:		}
;2406:		else if (gametype == GT_HARVESTER) {
;2407:			if (BotHarvesterCarryingCubes(bs))
;2408:				range = 80;
;2409:		}
;2410:#endif
;2411:		//
;2412:		if (BotNearbyGoal(bs, bs->tfl, &goal, range)) {
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
ARGP4
ADDRLP4 344
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 264
INDIRF4
ARGF4
ADDRLP4 348
ADDRGP4 BotNearbyGoal
CALLI4
ASGNI4
ADDRLP4 348
INDIRI4
CNSTI4 0
EQI4 $1080
line 2413
;2413:			trap_BotResetLastAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetLastAvoidReach
CALLV
pop
line 2415
;2414:			//time the bot gets to pick up the nearby goal item
;2415:			bs->nbg_time = FloatTime() + range / 100 + 1;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
ADDRGP4 floattime
INDIRF4
ADDRLP4 264
INDIRF4
CNSTF4 1120403456
DIVF4
ADDF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2416
;2416:			AIEnter_Battle_NBG(bs, "battle retreat: nbg");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1082
ARGP4
ADDRGP4 AIEnter_Battle_NBG
CALLV
pop
line 2417
;2417:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1030
JUMPV
LABELV $1080
line 2419
;2418:		}
;2419:	}
LABELV $1074
line 2421
;2420:	//initialize the movement state
;2421:	BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 2423
;2422:	//move towards the goal
;2423:	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
ADDRLP4 0
ARGP4
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 344
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 2425
;2424:	//if the movement failed
;2425:	if (moveresult.failure) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $1083
line 2427
;2426:		//reset the avoid reach, otherwise bot is stuck in current area
;2427:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 2429
;2428:		//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
;2429:		bs->ltg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6068
ADDP4
CNSTF4 0
ASGNF4
line 2430
;2430:	}
LABELV $1083
line 2432
;2431:	//
;2432:	BotAIBlocked(bs, &moveresult, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 2434
;2433:	//choose the best weapon to fight with
;2434:	BotChooseWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChooseWeapon
CALLV
pop
line 2436
;2435:	//if the view is fixed for the movement
;2436:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $1085
line 2437
;2437:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 0+40
INDIRB
ASGNB 12
line 2438
;2438:	}
ADDRGP4 $1086
JUMPV
LABELV $1085
line 2439
;2439:	else if (!(moveresult.flags & MOVERESULT_MOVEMENTVIEWSET)
ADDRLP4 348
CNSTI4 0
ASGNI4
ADDRLP4 0+20
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 348
INDIRI4
NEI4 $1089
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 348
INDIRI4
NEI4 $1089
line 2440
;2440:				&& !(bs->flags & BFL_IDEALVIEWSET) ) {
line 2441
;2441:		attack_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_ATTACK_SKILL, 0, 1);
ADDRFP4 0
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
ADDRLP4 352
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 268
ADDRLP4 352
INDIRF4
ASGNF4
line 2443
;2442:		//if the bot is skilled anough
;2443:		if (attack_skill > 0.3) {
ADDRLP4 268
INDIRF4
CNSTF4 1050253722
LEF4 $1092
line 2444
;2444:			BotAimAtEnemy(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotAimAtEnemy
CALLV
pop
line 2445
;2445:		}
ADDRGP4 $1093
JUMPV
LABELV $1092
line 2446
;2446:		else {
line 2447
;2447:			if (trap_BotMovementViewTarget(bs->ms, &goal, bs->tfl, 300, target)) {
ADDRLP4 356
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 356
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 356
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 252
ARGP4
ADDRLP4 360
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 360
INDIRI4
CNSTI4 0
EQI4 $1094
line 2448
;2448:				VectorSubtract(target, bs->origin, dir);
ADDRLP4 364
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 272
ADDRLP4 252
INDIRF4
ADDRLP4 364
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 272+4
ADDRLP4 252+4
INDIRF4
ADDRLP4 364
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 272+8
ADDRLP4 252+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2449
;2449:				vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 272
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2450
;2450:			}
ADDRGP4 $1095
JUMPV
LABELV $1094
line 2451
;2451:			else {
line 2452
;2452:				vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 0+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2453
;2453:			}
LABELV $1095
line 2454
;2454:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 364
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 364
INDIRP4
CNSTF4 1056964608
ADDRLP4 364
INDIRP4
INDIRF4
MULF4
ASGNF4
line 2455
;2455:		}
LABELV $1093
line 2456
;2456:	}
LABELV $1089
LABELV $1086
line 2458
;2457:	//if the weapon is used for the bot movement
;2458:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs->weaponnum = moveresult.weapon;
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $1101
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0+24
INDIRI4
ASGNI4
LABELV $1101
line 2460
;2459:	//attack the enemy if possible
;2460:	BotCheckAttack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckAttack
CALLV
pop
line 2462
;2461:	//
;2462:	return qtrue;
CNSTI4 1
RETI4
LABELV $1030
endproc AINode_Battle_Retreat 368 20
export AIEnter_Battle_NBG
proc AIEnter_Battle_NBG 0 16
line 2470
;2463:}
;2464:
;2465:/*
;2466:==================
;2467:AIEnter_Battle_NBG
;2468:==================
;2469:*/
;2470:void AIEnter_Battle_NBG(bot_state_t *bs, char *s) {
line 2471
;2471:	BotRecordNodeSwitch(bs, "battle NBG", "", s);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1106
ARGP4
ADDRGP4 $60
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotRecordNodeSwitch
CALLV
pop
line 2472
;2472:	bs->ainode = AINode_Battle_NBG;
ADDRFP4 0
INDIRP4
CNSTI4 4900
ADDP4
ADDRGP4 AINode_Battle_NBG
ASGNP4
line 2473
;2473:}
LABELV $1105
endproc AIEnter_Battle_NBG 0 16
export AINode_Battle_NBG
proc AINode_Battle_NBG 356 20
line 2480
;2474:
;2475:/*
;2476:==================
;2477:AINode_Battle_NBG
;2478:==================
;2479:*/
;2480:int AINode_Battle_NBG(bot_state_t *bs) {
line 2488
;2481:	int areanum;
;2482:	bot_goal_t goal;
;2483:	aas_entityinfo_t entinfo;
;2484:	bot_moveresult_t moveresult;
;2485:	float attack_skill;
;2486:	vec3_t target, dir;
;2487:
;2488:	if (BotIsObserver(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 280
ADDRGP4 BotIsObserver
CALLI4
ASGNI4
ADDRLP4 280
INDIRI4
CNSTI4 0
EQI4 $1108
line 2489
;2489:		AIEnter_Observer(bs, "battle nbg: observer");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1110
ARGP4
ADDRGP4 AIEnter_Observer
CALLV
pop
line 2490
;2490:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1108
line 2493
;2491:	}
;2492:	//if in the intermission
;2493:	if (BotIntermission(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 284
ADDRGP4 BotIntermission
CALLI4
ASGNI4
ADDRLP4 284
INDIRI4
CNSTI4 0
EQI4 $1111
line 2494
;2494:		AIEnter_Intermission(bs, "battle nbg: intermission");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1113
ARGP4
ADDRGP4 AIEnter_Intermission
CALLV
pop
line 2495
;2495:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1111
line 2498
;2496:	}
;2497:	//respawn if dead
;2498:	if (BotIsDead(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 288
ADDRGP4 BotIsDead
CALLI4
ASGNI4
ADDRLP4 288
INDIRI4
CNSTI4 0
EQI4 $1114
line 2499
;2499:		AIEnter_Respawn(bs, "battle nbg: bot dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1116
ARGP4
ADDRGP4 AIEnter_Respawn
CALLV
pop
line 2500
;2500:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1114
line 2503
;2501:	}
;2502:	//if no enemy
;2503:	if (bs->enemy < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 0
GEI4 $1117
line 2504
;2504:		AIEnter_Seek_NBG(bs, "battle nbg: no enemy");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1119
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 2505
;2505:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1117
line 2508
;2506:	}
;2507:	//
;2508:	BotEntityInfo(bs->enemy, &entinfo);
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
ARGP4
ADDRGP4 BotEntityInfo
CALLV
pop
line 2509
;2509:	if (EntityIsDead(&entinfo)) {
ADDRLP4 108
ARGP4
ADDRLP4 292
ADDRGP4 EntityIsDead
CALLI4
ASGNI4
ADDRLP4 292
INDIRI4
CNSTI4 0
EQI4 $1120
line 2510
;2510:		AIEnter_Seek_NBG(bs, "battle nbg: enemy dead");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1122
ARGP4
ADDRGP4 AIEnter_Seek_NBG
CALLV
pop
line 2511
;2511:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1120
line 2514
;2512:	}
;2513:	//
;2514:	bs->tfl = TFL_DEFAULT;
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
CNSTI4 18616254
ASGNI4
line 2515
;2515:	if (bot_grapple.integer) bs->tfl |= TFL_GRAPPLEHOOK;
ADDRGP4 bot_grapple+12
INDIRI4
CNSTI4 0
EQI4 $1123
ADDRLP4 296
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 296
INDIRP4
ADDRLP4 296
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
LABELV $1123
line 2517
;2516:	//if in lava or slime the bot should be able to get out
;2517:	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 300
ADDRGP4 BotInLavaOrSlime
CALLI4
ASGNI4
ADDRLP4 300
INDIRI4
CNSTI4 0
EQI4 $1126
ADDRLP4 304
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 304
INDIRP4
ADDRLP4 304
INDIRP4
INDIRI4
CNSTI4 6291456
BORI4
ASGNI4
LABELV $1126
line 2519
;2518:	//
;2519:	if (BotCanAndWantsToRocketJump(bs)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 308
ADDRGP4 BotCanAndWantsToRocketJump
CALLI4
ASGNI4
ADDRLP4 308
INDIRI4
CNSTI4 0
EQI4 $1128
line 2520
;2520:		bs->tfl |= TFL_ROCKETJUMP;
ADDRLP4 312
ADDRFP4 0
INDIRP4
CNSTI4 5976
ADDP4
ASGNP4
ADDRLP4 312
INDIRP4
ADDRLP4 312
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 2521
;2521:	}
LABELV $1128
line 2523
;2522:	//map specific code
;2523:	BotMapScripts(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotMapScripts
CALLV
pop
line 2525
;2524:	//update the last time the enemy was visible
;2525:	if (BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy)) {
ADDRLP4 312
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 312
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 312
INDIRP4
CNSTI4 4936
ADDP4
ARGP4
ADDRLP4 312
INDIRP4
CNSTI4 6564
ADDP4
ARGP4
CNSTF4 1135869952
ARGF4
ADDRLP4 312
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRLP4 316
ADDRGP4 BotEntityVisible
CALLF4
ASGNF4
ADDRLP4 316
INDIRF4
CNSTF4 0
EQF4 $1130
line 2526
;2526:		bs->enemyvisible_time = FloatTime();
ADDRFP4 0
INDIRP4
CNSTI4 6088
ADDP4
ADDRGP4 floattime
INDIRF4
ASGNF4
line 2527
;2527:		VectorCopy(entinfo.origin, target);
ADDRLP4 252
ADDRLP4 108+24
INDIRB
ASGNB 12
line 2529
;2528:		// if not a player enemy
;2529:		if (bs->enemy >= MAX_CLIENTS) {
ADDRFP4 0
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
CNSTI4 64
LTI4 $1133
line 2537
;2530:#ifdef MISSIONPACK
;2531:			// if attacking an obelisk
;2532:			if ( bs->enemy == redobelisk.entitynum ||
;2533:				bs->enemy == blueobelisk.entitynum ) {
;2534:				target[2] += 16;
;2535:			}
;2536:#endif
;2537:		}
LABELV $1133
line 2539
;2538:		//update the reachability area and origin if possible
;2539:		areanum = BotPointAreaNum(target);
ADDRLP4 252
ARGP4
ADDRLP4 320
ADDRGP4 BotPointAreaNum
CALLI4
ASGNI4
ADDRLP4 248
ADDRLP4 320
INDIRI4
ASGNI4
line 2540
;2540:		if (areanum && trap_AAS_AreaReachability(areanum)) {
ADDRLP4 324
ADDRLP4 248
INDIRI4
ASGNI4
ADDRLP4 324
INDIRI4
CNSTI4 0
EQI4 $1135
ADDRLP4 324
INDIRI4
ARGI4
ADDRLP4 328
ADDRGP4 trap_AAS_AreaReachability
CALLI4
ASGNI4
ADDRLP4 328
INDIRI4
CNSTI4 0
EQI4 $1135
line 2541
;2541:			VectorCopy(target, bs->lastenemyorigin);
ADDRFP4 0
INDIRP4
CNSTI4 6548
ADDP4
ADDRLP4 252
INDIRB
ASGNB 12
line 2542
;2542:			bs->lastenemyareanum = areanum;
ADDRFP4 0
INDIRP4
CNSTI4 6544
ADDP4
ADDRLP4 248
INDIRI4
ASGNI4
line 2543
;2543:		}
LABELV $1135
line 2544
;2544:	}
LABELV $1130
line 2546
;2545:	//if the bot has no goal or touches the current goal
;2546:	if (!trap_BotGetTopGoal(bs->gs, &goal)) {
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 320
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 320
INDIRI4
CNSTI4 0
NEI4 $1137
line 2547
;2547:		bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
line 2548
;2548:	}
ADDRGP4 $1138
JUMPV
LABELV $1137
line 2549
;2549:	else if (BotReachedGoal(bs, &goal)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ARGP4
ADDRLP4 324
ADDRGP4 BotReachedGoal
CALLI4
ASGNI4
ADDRLP4 324
INDIRI4
CNSTI4 0
EQI4 $1139
line 2550
;2550:		bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
line 2551
;2551:	}
LABELV $1139
LABELV $1138
line 2553
;2552:	//
;2553:	if (bs->nbg_time < FloatTime()) {
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
INDIRF4
ADDRGP4 floattime
INDIRF4
GEF4 $1141
line 2555
;2554:		//pop the current goal from the stack
;2555:		trap_BotPopGoal(bs->gs);
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotPopGoal
CALLV
pop
line 2557
;2556:		//if the bot still has a goal
;2557:		if (trap_BotGetTopGoal(bs->gs, &goal))
ADDRFP4 0
INDIRP4
CNSTI4 6528
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 328
ADDRGP4 trap_BotGetTopGoal
CALLI4
ASGNI4
ADDRLP4 328
INDIRI4
CNSTI4 0
EQI4 $1143
line 2558
;2558:			AIEnter_Battle_Retreat(bs, "battle nbg: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1145
ARGP4
ADDRGP4 AIEnter_Battle_Retreat
CALLV
pop
ADDRGP4 $1144
JUMPV
LABELV $1143
line 2560
;2559:		else
;2560:			AIEnter_Battle_Fight(bs, "battle nbg: time out");
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $1145
ARGP4
ADDRGP4 AIEnter_Battle_Fight
CALLV
pop
LABELV $1144
line 2562
;2561:		//
;2562:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1107
JUMPV
LABELV $1141
line 2565
;2563:	}
;2564:	//initialize the movement state
;2565:	BotSetupForMovement(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotSetupForMovement
CALLV
pop
line 2567
;2566:	//move towards the goal
;2567:	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, bs->tfl);
ADDRLP4 0
ARGP4
ADDRLP4 328
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 328
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 328
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotMoveToGoal
CALLV
pop
line 2569
;2568:	//if the movement failed
;2569:	if (moveresult.failure) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $1146
line 2571
;2570:		//reset the avoid reach, otherwise bot is stuck in current area
;2571:		trap_BotResetAvoidReach(bs->ms);
ADDRFP4 0
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_BotResetAvoidReach
CALLV
pop
line 2573
;2572:		//BotAI_Print(PRT_MESSAGE, "movement failure %d\n", moveresult.traveltype);
;2573:		bs->nbg_time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 6072
ADDP4
CNSTF4 0
ASGNF4
line 2574
;2574:	}
LABELV $1146
line 2576
;2575:	//
;2576:	BotAIBlocked(bs, &moveresult, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 BotAIBlocked
CALLV
pop
line 2578
;2577:	//update the attack inventory values
;2578:	BotUpdateBattleInventory(bs, bs->enemy);
ADDRLP4 332
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 332
INDIRP4
ARGP4
ADDRLP4 332
INDIRP4
CNSTI4 6540
ADDP4
INDIRI4
ARGI4
ADDRGP4 BotUpdateBattleInventory
CALLV
pop
line 2580
;2579:	//choose the best weapon to fight with
;2580:	BotChooseWeapon(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotChooseWeapon
CALLV
pop
line 2582
;2581:	//if the view is fixed for the movement
;2582:	if (moveresult.flags & (MOVERESULT_MOVEMENTVIEW|MOVERESULT_SWIMVIEW)) {
ADDRLP4 0+20
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $1148
line 2583
;2583:		VectorCopy(moveresult.ideal_viewangles, bs->ideal_viewangles);
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ADDRLP4 0+40
INDIRB
ASGNB 12
line 2584
;2584:	}
ADDRGP4 $1149
JUMPV
LABELV $1148
line 2585
;2585:	else if (!(moveresult.flags & MOVERESULT_MOVEMENTVIEWSET)
ADDRLP4 336
CNSTI4 0
ASGNI4
ADDRLP4 0+20
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 336
INDIRI4
NEI4 $1152
ADDRFP4 0
INDIRP4
CNSTI4 5980
ADDP4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 336
INDIRI4
NEI4 $1152
line 2586
;2586:				&& !(bs->flags & BFL_IDEALVIEWSET)) {
line 2587
;2587:		attack_skill = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_ATTACK_SKILL, 0, 1);
ADDRFP4 0
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
ADDRLP4 340
ADDRGP4 trap_Characteristic_BFloat
CALLF4
ASGNF4
ADDRLP4 264
ADDRLP4 340
INDIRF4
ASGNF4
line 2589
;2588:		//if the bot is skilled anough and the enemy is visible
;2589:		if (attack_skill > 0.3) {
ADDRLP4 264
INDIRF4
CNSTF4 1050253722
LEF4 $1155
line 2591
;2590:			//&& BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy)
;2591:			BotAimAtEnemy(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotAimAtEnemy
CALLV
pop
line 2592
;2592:		}
ADDRGP4 $1156
JUMPV
LABELV $1155
line 2593
;2593:		else {
line 2594
;2594:			if (trap_BotMovementViewTarget(bs->ms, &goal, bs->tfl, 300, target)) {
ADDRLP4 344
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 344
INDIRP4
CNSTI4 6524
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ARGP4
ADDRLP4 344
INDIRP4
CNSTI4 5976
ADDP4
INDIRI4
ARGI4
CNSTF4 1133903872
ARGF4
ADDRLP4 252
ARGP4
ADDRLP4 348
ADDRGP4 trap_BotMovementViewTarget
CALLI4
ASGNI4
ADDRLP4 348
INDIRI4
CNSTI4 0
EQI4 $1157
line 2595
;2595:				VectorSubtract(target, bs->origin, dir);
ADDRLP4 352
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 268
ADDRLP4 252
INDIRF4
ADDRLP4 352
INDIRP4
CNSTI4 4908
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 268+4
ADDRLP4 252+4
INDIRF4
ADDRLP4 352
INDIRP4
CNSTI4 4912
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 268+8
ADDRLP4 252+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 4916
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2596
;2596:				vectoangles(dir, bs->ideal_viewangles);
ADDRLP4 268
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2597
;2597:			}
ADDRGP4 $1158
JUMPV
LABELV $1157
line 2598
;2598:			else {
line 2599
;2599:				vectoangles(moveresult.movedir, bs->ideal_viewangles);
ADDRLP4 0+28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 6576
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2600
;2600:			}
LABELV $1158
line 2601
;2601:			bs->ideal_viewangles[2] *= 0.5;
ADDRLP4 352
ADDRFP4 0
INDIRP4
CNSTI4 6584
ADDP4
ASGNP4
ADDRLP4 352
INDIRP4
CNSTF4 1056964608
ADDRLP4 352
INDIRP4
INDIRF4
MULF4
ASGNF4
line 2602
;2602:		}
LABELV $1156
line 2603
;2603:	}
LABELV $1152
LABELV $1149
line 2605
;2604:	//if the weapon is used for the bot movement
;2605:	if (moveresult.flags & MOVERESULT_MOVEMENTWEAPON) bs->weaponnum = moveresult.weapon;
ADDRLP4 0+20
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $1164
ADDRFP4 0
INDIRP4
CNSTI4 6560
ADDP4
ADDRLP4 0+24
INDIRI4
ASGNI4
LABELV $1164
line 2607
;2606:	//attack the enemy if possible
;2607:	BotCheckAttack(bs);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BotCheckAttack
CALLV
pop
line 2609
;2608:	//
;2609:	return qtrue;
CNSTI4 1
RETI4
LABELV $1107
endproc AINode_Battle_NBG 356 20
bss
export nodeswitch
align 1
LABELV nodeswitch
skip 7344
export numnodeswitches
align 4
LABELV numnodeswitches
skip 4
import BotVoiceChatOnly
import BotVoiceChat
import BotSetTeamMateTaskPreference
import BotGetTeamMateTaskPreference
import BotTeamAI
import AIEnter_Seek_Camp
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
LABELV $1145
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $1122
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $1119
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $1116
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $1113
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $1110
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1106
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 78
byte 1 66
byte 1 71
byte 1 0
align 1
LABELV $1082
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 0
align 1
LABELV $1073
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 119
byte 1 97
byte 1 121
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $1070
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 97
byte 1 110
byte 1 111
byte 1 116
byte 1 104
byte 1 101
byte 1 114
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $1065
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 108
byte 1 111
byte 1 115
byte 1 116
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $1055
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $1045
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $1042
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $1039
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $1036
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $1033
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1029
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $1027
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $1001
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 0
align 1
LABELV $996
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $971
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 97
byte 1 0
align 1
LABELV $968
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 98
byte 1 101
byte 1 116
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $963
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $960
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $957
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $954
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $950
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $948
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 114
byte 1 101
byte 1 116
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $934
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 115
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $929
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 32
byte 1 104
byte 1 105
byte 1 116
byte 1 32
byte 1 115
byte 1 111
byte 1 109
byte 1 101
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $924
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 32
byte 1 104
byte 1 101
byte 1 97
byte 1 108
byte 1 116
byte 1 104
byte 1 32
byte 1 100
byte 1 101
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 115
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $914
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 118
byte 1 105
byte 1 115
byte 1 105
byte 1 98
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $907
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $898
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $893
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $890
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $887
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $882
byte 1 98
byte 1 97
byte 1 116
byte 1 116
byte 1 108
byte 1 101
byte 1 32
byte 1 102
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $841
byte 1 108
byte 1 116
byte 1 103
byte 1 32
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 58
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 0
align 1
LABELV $828
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 108
byte 1 116
byte 1 103
byte 1 58
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $812
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 108
byte 1 116
byte 1 103
byte 1 58
byte 1 32
byte 1 114
byte 1 97
byte 1 110
byte 1 100
byte 1 111
byte 1 109
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $809
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 108
byte 1 116
byte 1 103
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $806
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 108
byte 1 116
byte 1 103
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $803
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 108
byte 1 116
byte 1 103
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $799
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 76
byte 1 84
byte 1 71
byte 1 0
align 1
LABELV $794
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $757
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $743
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $740
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $737
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 110
byte 1 98
byte 1 103
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $733
byte 1 110
byte 1 111
byte 1 32
byte 1 103
byte 1 111
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $732
byte 1 115
byte 1 101
byte 1 101
byte 1 107
byte 1 32
byte 1 78
byte 1 66
byte 1 71
byte 1 0
align 1
LABELV $727
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $676
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $665
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $642
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 103
byte 1 111
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $634
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $631
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $628
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $624
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 97
byte 1 116
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $530
byte 1 114
byte 1 101
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 58
byte 1 32
byte 1 114
byte 1 101
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $522
byte 1 114
byte 1 101
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $520
byte 1 115
byte 1 116
byte 1 97
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $517
byte 1 115
byte 1 116
byte 1 97
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 0
align 1
LABELV $507
byte 1 115
byte 1 116
byte 1 97
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $505
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 58
byte 1 32
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 32
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $501
byte 1 111
byte 1 98
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $499
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 58
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $491
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $460
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
LABELV $457
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 95
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $442
byte 1 111
byte 1 110
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
LABELV $441
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
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $407
byte 1 111
byte 1 110
byte 1 103
byte 1 101
byte 1 116
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $406
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $397
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 95
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $384
byte 1 112
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $383
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 0
align 1
LABELV $350
byte 1 105
byte 1 110
byte 1 112
byte 1 111
byte 1 115
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $349
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $340
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 95
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $335
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $327
byte 1 103
byte 1 101
byte 1 116
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 95
byte 1 103
byte 1 111
byte 1 116
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $324
byte 1 103
byte 1 101
byte 1 116
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 95
byte 1 110
byte 1 111
byte 1 116
byte 1 116
byte 1 104
byte 1 101
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $319
byte 1 103
byte 1 101
byte 1 116
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $312
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $309
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $298
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 95
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $295
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
LABELV $294
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $285
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 95
byte 1 99
byte 1 97
byte 1 110
byte 1 110
byte 1 111
byte 1 116
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $276
byte 1 66
byte 1 111
byte 1 116
byte 1 76
byte 1 111
byte 1 110
byte 1 103
byte 1 84
byte 1 101
byte 1 114
byte 1 109
byte 1 71
byte 1 111
byte 1 97
byte 1 108
byte 1 58
byte 1 32
byte 1 103
byte 1 111
byte 1 32
byte 1 102
byte 1 111
byte 1 114
byte 1 32
byte 1 97
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $254
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $161
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 95
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $158
byte 1 97
byte 1 99
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 97
byte 1 110
byte 1 121
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $132
byte 1 121
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $131
byte 1 104
byte 1 101
byte 1 108
byte 1 112
byte 1 95
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $62
byte 1 37
byte 1 115
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 50
byte 1 46
byte 1 49
byte 1 102
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 58
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
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $60
byte 1 0
align 1
LABELV $55
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
