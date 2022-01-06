code
proc UI_RemoveBotsMenu_SetBotNames 1032 12
file "../ui_removebots.c"
line 87
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
;23:/*
;24:=======================================================================
;25:
;26:REMOVE BOTS MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ART_BACKGROUND		"menu/art/addbotframe"
;36:#define ART_BACK0			"menu/art/back_0"
;37:#define ART_BACK1			"menu/art/back_1"	
;38:#define ART_DELETE0			"menu/art/delete_0"
;39:#define ART_DELETE1			"menu/art/delete_1"
;40:#define ART_ARROWS			"menu/art/arrows_vert_0"
;41:#define ART_ARROWUP			"menu/art/arrows_vert_top"
;42:#define ART_ARROWDOWN		"menu/art/arrows_vert_bot"
;43:
;44:#define ID_UP				10
;45:#define ID_DOWN				11
;46:#define ID_DELETE			12
;47:#define ID_BACK				13
;48:#define ID_BOTNAME0			20
;49:#define ID_BOTNAME1			21
;50:#define ID_BOTNAME2			22
;51:#define ID_BOTNAME3			23
;52:#define ID_BOTNAME4			24
;53:#define ID_BOTNAME5			25
;54:#define ID_BOTNAME6			26
;55:
;56:
;57:typedef struct {
;58:	menuframework_s	menu;
;59:
;60:	menutext_s		banner;
;61:	menubitmap_s	background;
;62:
;63:	menubitmap_s	arrows;
;64:	menubitmap_s	up;
;65:	menubitmap_s	down;
;66:
;67:	menutext_s		bots[7];
;68:
;69:	menubitmap_s	delete;
;70:	menubitmap_s	back;
;71:
;72:	int				numBots;
;73:	int				baseBotNum;
;74:	int				selectedBotNum;
;75:	char			botnames[7][32];
;76:	int				botClientNums[MAX_BOTS];
;77:} removeBotsMenuInfo_t;
;78:
;79:static removeBotsMenuInfo_t	removeBotsMenuInfo;
;80:
;81:
;82:/*
;83:=================
;84:UI_RemoveBotsMenu_SetBotNames
;85:=================
;86:*/
;87:static void UI_RemoveBotsMenu_SetBotNames( void ) {
line 91
;88:	int		n;
;89:	char	info[MAX_INFO_STRING];
;90:
;91:	for ( n = 0; (n < 7) && (removeBotsMenuInfo.baseBotNum + n < removeBotsMenuInfo.numBots); n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $72
JUMPV
LABELV $69
line 92
;92:		trap_GetConfigString( CS_PLAYERS + removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + n], info, MAX_INFO_STRING );
ADDRGP4 removeBotsMenuInfo+1396
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CNSTI4 2
LSHI4
ADDRGP4 removeBotsMenuInfo+1628
ADDP4
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 93
;93:		Q_strncpyz( removeBotsMenuInfo.botnames[n], Info_ValueForKey( info, "n" ), sizeof(removeBotsMenuInfo.botnames[n]) );
ADDRLP4 4
ARGP4
ADDRGP4 $78
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 removeBotsMenuInfo+1404
ADDP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 94
;94:		Q_CleanStr( removeBotsMenuInfo.botnames[n] );
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 removeBotsMenuInfo+1404
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 95
;95:	}
LABELV $70
line 91
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $72
ADDRLP4 0
INDIRI4
CNSTI4 7
GEI4 $81
ADDRGP4 removeBotsMenuInfo+1396
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ADDRGP4 removeBotsMenuInfo+1392
INDIRI4
LTI4 $69
LABELV $81
line 97
;96:
;97:}
LABELV $68
endproc UI_RemoveBotsMenu_SetBotNames 1032 12
proc UI_RemoveBotsMenu_DeleteEvent 4 8
line 105
;98:
;99:
;100:/*
;101:=================
;102:UI_RemoveBotsMenu_DeleteEvent
;103:=================
;104:*/
;105:static void UI_RemoveBotsMenu_DeleteEvent( void* ptr, int event ) {
line 106
;106:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $83
line 107
;107:		return;
ADDRGP4 $82
JUMPV
LABELV $83
line 110
;108:	}
;109:
;110:	trap_Cmd_ExecuteText( EXEC_APPEND, va("clientkick %i\n", removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + removeBotsMenuInfo.selectedBotNum]) );
ADDRGP4 $85
ARGP4
ADDRGP4 removeBotsMenuInfo+1396
INDIRI4
ADDRGP4 removeBotsMenuInfo+1400
INDIRI4
ADDI4
CNSTI4 2
LSHI4
ADDRGP4 removeBotsMenuInfo+1628
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 111
;111:}
LABELV $82
endproc UI_RemoveBotsMenu_DeleteEvent 4 8
proc UI_RemoveBotsMenu_BotEvent 0 0
line 119
;112:
;113:
;114:/*
;115:=================
;116:UI_RemoveBotsMenu_BotEvent
;117:=================
;118:*/
;119:static void UI_RemoveBotsMenu_BotEvent( void* ptr, int event ) {
line 120
;120:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $90
line 121
;121:		return;
ADDRGP4 $89
JUMPV
LABELV $90
line 124
;122:	}
;123:
;124:	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = color_orange;
CNSTI4 72
ADDRGP4 removeBotsMenuInfo+1400
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 125
;125:	removeBotsMenuInfo.selectedBotNum = ((menucommon_s*)ptr)->id - ID_BOTNAME0;
ADDRGP4 removeBotsMenuInfo+1400
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 20
SUBI4
ASGNI4
line 126
;126:	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = color_white;
CNSTI4 72
ADDRGP4 removeBotsMenuInfo+1400
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+68
ADDP4
ADDRGP4 color_white
ASGNP4
line 127
;127:}
LABELV $89
endproc UI_RemoveBotsMenu_BotEvent 0 0
proc UI_RemoveBotsMenu_BackEvent 0 0
line 135
;128:
;129:
;130:/*
;131:=================
;132:UI_RemoveAddBotsMenu_BackEvent
;133:=================
;134:*/
;135:static void UI_RemoveBotsMenu_BackEvent( void* ptr, int event ) {
line 136
;136:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $100
line 137
;137:		return;
ADDRGP4 $99
JUMPV
LABELV $100
line 139
;138:	}
;139:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 140
;140:}
LABELV $99
endproc UI_RemoveBotsMenu_BackEvent 0 0
proc UI_RemoveBotsMenu_UpEvent 4 0
line 148
;141:
;142:
;143:/*
;144:=================
;145:UI_RemoveBotsMenu_UpEvent
;146:=================
;147:*/
;148:static void UI_RemoveBotsMenu_UpEvent( void* ptr, int event ) {
line 149
;149:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $103
line 150
;150:		return;
ADDRGP4 $102
JUMPV
LABELV $103
line 153
;151:	}
;152:
;153:	if( removeBotsMenuInfo.baseBotNum > 0 ) {
ADDRGP4 removeBotsMenuInfo+1396
INDIRI4
CNSTI4 0
LEI4 $105
line 154
;154:		removeBotsMenuInfo.baseBotNum--;
ADDRLP4 0
ADDRGP4 removeBotsMenuInfo+1396
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 155
;155:		UI_RemoveBotsMenu_SetBotNames();
ADDRGP4 UI_RemoveBotsMenu_SetBotNames
CALLV
pop
line 156
;156:	}
LABELV $105
line 157
;157:}
LABELV $102
endproc UI_RemoveBotsMenu_UpEvent 4 0
proc UI_RemoveBotsMenu_DownEvent 4 0
line 165
;158:
;159:
;160:/*
;161:=================
;162:UI_RemoveBotsMenu_DownEvent
;163:=================
;164:*/
;165:static void UI_RemoveBotsMenu_DownEvent( void* ptr, int event ) {
line 166
;166:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $110
line 167
;167:		return;
ADDRGP4 $109
JUMPV
LABELV $110
line 170
;168:	}
;169:
;170:	if( removeBotsMenuInfo.baseBotNum + 7 < removeBotsMenuInfo.numBots ) {
ADDRGP4 removeBotsMenuInfo+1396
INDIRI4
CNSTI4 7
ADDI4
ADDRGP4 removeBotsMenuInfo+1392
INDIRI4
GEI4 $112
line 171
;171:		removeBotsMenuInfo.baseBotNum++;
ADDRLP4 0
ADDRGP4 removeBotsMenuInfo+1396
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 172
;172:		UI_RemoveBotsMenu_SetBotNames();
ADDRGP4 UI_RemoveBotsMenu_SetBotNames
CALLV
pop
line 173
;173:	}
LABELV $112
line 174
;174:}
LABELV $109
endproc UI_RemoveBotsMenu_DownEvent 4 0
proc UI_RemoveBotsMenu_GetBots 1056 12
line 182
;175:
;176:
;177:/*
;178:=================
;179:UI_RemoveBotsMenu_GetBots
;180:=================
;181:*/
;182:static void UI_RemoveBotsMenu_GetBots( void ) {
line 188
;183:	int		numPlayers;
;184:	int		isBot;
;185:	int		n;
;186:	char	info[MAX_INFO_STRING];
;187:
;188:	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 189
;189:	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $118
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
ADDRLP4 1032
ADDRLP4 1040
INDIRI4
ASGNI4
line 190
;190:	removeBotsMenuInfo.numBots = 0;
ADDRGP4 removeBotsMenuInfo+1392
CNSTI4 0
ASGNI4
line 192
;191:
;192:	for( n = 0; n < numPlayers; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $123
JUMPV
LABELV $120
line 193
;193:		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );
ADDRLP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 195
;194:
;195:		isBot = atoi( Info_ValueForKey( info, "skill" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $124
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
ADDRLP4 1028
ADDRLP4 1048
INDIRI4
ASGNI4
line 196
;196:		if( !isBot ) {
ADDRLP4 1028
INDIRI4
CNSTI4 0
NEI4 $125
line 197
;197:			continue;
ADDRGP4 $121
JUMPV
LABELV $125
line 200
;198:		}
;199:
;200:		removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.numBots] = n;
ADDRGP4 removeBotsMenuInfo+1392
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 removeBotsMenuInfo+1628
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 201
;201:		removeBotsMenuInfo.numBots++;
ADDRLP4 1052
ADDRGP4 removeBotsMenuInfo+1392
ASGNP4
ADDRLP4 1052
INDIRP4
ADDRLP4 1052
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 202
;202:	}
LABELV $121
line 192
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $123
ADDRLP4 0
INDIRI4
ADDRLP4 1032
INDIRI4
LTI4 $120
line 203
;203:}
LABELV $117
endproc UI_RemoveBotsMenu_GetBots 1056 12
export UI_RemoveBots_Cache
proc UI_RemoveBots_Cache 0 4
line 211
;204:
;205:
;206:/*
;207:=================
;208:UI_RemoveBots_Cache
;209:=================
;210:*/
;211:void UI_RemoveBots_Cache( void ) {
line 212
;212:	trap_R_RegisterShaderNoMip( ART_BACKGROUND );
ADDRGP4 $131
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 213
;213:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $132
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 214
;214:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $133
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 215
;215:	trap_R_RegisterShaderNoMip( ART_DELETE0 );
ADDRGP4 $134
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 216
;216:	trap_R_RegisterShaderNoMip( ART_DELETE1 );
ADDRGP4 $135
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 217
;217:}
LABELV $130
endproc UI_RemoveBots_Cache 0 4
proc UI_RemoveBotsMenu_Init 24 12
line 225
;218:
;219:
;220:/*
;221:=================
;222:UI_RemoveBotsMenu_Init
;223:=================
;224:*/
;225:static void UI_RemoveBotsMenu_Init( void ) {
line 230
;226:	int		n;
;227:	int		count;
;228:	int		y;
;229:
;230:	memset( &removeBotsMenuInfo, 0 ,sizeof(removeBotsMenuInfo) );
ADDRGP4 removeBotsMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 5724
ARGI4
ADDRGP4 memset
CALLP4
pop
line 231
;231:	removeBotsMenuInfo.menu.fullscreen = qfalse;
ADDRGP4 removeBotsMenuInfo+280
CNSTI4 0
ASGNI4
line 232
;232:	removeBotsMenuInfo.menu.wrapAround = qtrue;
ADDRGP4 removeBotsMenuInfo+276
CNSTI4 1
ASGNI4
line 234
;233:
;234:	UI_RemoveBots_Cache();
ADDRGP4 UI_RemoveBots_Cache
CALLV
pop
line 236
;235:
;236:	UI_RemoveBotsMenu_GetBots();
ADDRGP4 UI_RemoveBotsMenu_GetBots
CALLV
pop
line 237
;237:	UI_RemoveBotsMenu_SetBotNames();
ADDRGP4 UI_RemoveBotsMenu_SetBotNames
CALLV
pop
line 238
;238:	count = removeBotsMenuInfo.numBots < 7 ? removeBotsMenuInfo.numBots : 7;
ADDRGP4 removeBotsMenuInfo+1392
INDIRI4
CNSTI4 7
GEI4 $142
ADDRLP4 12
ADDRGP4 removeBotsMenuInfo+1392
INDIRI4
ASGNI4
ADDRGP4 $143
JUMPV
LABELV $142
ADDRLP4 12
CNSTI4 7
ASGNI4
LABELV $143
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
line 240
;239:
;240:	removeBotsMenuInfo.banner.generic.type		= MTYPE_BTEXT;
ADDRGP4 removeBotsMenuInfo+288
CNSTI4 10
ASGNI4
line 241
;241:	removeBotsMenuInfo.banner.generic.x			= 320;
ADDRGP4 removeBotsMenuInfo+288+12
CNSTI4 320
ASGNI4
line 242
;242:	removeBotsMenuInfo.banner.generic.y			= 16;
ADDRGP4 removeBotsMenuInfo+288+16
CNSTI4 16
ASGNI4
line 243
;243:	removeBotsMenuInfo.banner.string			= "REMOVE BOTS";
ADDRGP4 removeBotsMenuInfo+288+60
ADDRGP4 $151
ASGNP4
line 244
;244:	removeBotsMenuInfo.banner.color				= color_white;
ADDRGP4 removeBotsMenuInfo+288+68
ADDRGP4 color_white
ASGNP4
line 245
;245:	removeBotsMenuInfo.banner.style				= UI_CENTER;
ADDRGP4 removeBotsMenuInfo+288+64
CNSTI4 1
ASGNI4
line 247
;246:
;247:	removeBotsMenuInfo.background.generic.type	= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+360
CNSTI4 6
ASGNI4
line 248
;248:	removeBotsMenuInfo.background.generic.name	= ART_BACKGROUND;
ADDRGP4 removeBotsMenuInfo+360+4
ADDRGP4 $131
ASGNP4
line 249
;249:	removeBotsMenuInfo.background.generic.flags	= QMF_INACTIVE;
ADDRGP4 removeBotsMenuInfo+360+44
CNSTU4 16384
ASGNU4
line 250
;250:	removeBotsMenuInfo.background.generic.x		= 320-233;
ADDRGP4 removeBotsMenuInfo+360+12
CNSTI4 87
ASGNI4
line 251
;251:	removeBotsMenuInfo.background.generic.y		= 240-166;
ADDRGP4 removeBotsMenuInfo+360+16
CNSTI4 74
ASGNI4
line 252
;252:	removeBotsMenuInfo.background.width			= 466;
ADDRGP4 removeBotsMenuInfo+360+76
CNSTI4 466
ASGNI4
line 253
;253:	removeBotsMenuInfo.background.height		= 332;
ADDRGP4 removeBotsMenuInfo+360+80
CNSTI4 332
ASGNI4
line 255
;254:
;255:	removeBotsMenuInfo.arrows.generic.type		= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+448
CNSTI4 6
ASGNI4
line 256
;256:	removeBotsMenuInfo.arrows.generic.name		= ART_ARROWS;
ADDRGP4 removeBotsMenuInfo+448+4
ADDRGP4 $172
ASGNP4
line 257
;257:	removeBotsMenuInfo.arrows.generic.flags		= QMF_INACTIVE;
ADDRGP4 removeBotsMenuInfo+448+44
CNSTU4 16384
ASGNU4
line 258
;258:	removeBotsMenuInfo.arrows.generic.x			= 200;
ADDRGP4 removeBotsMenuInfo+448+12
CNSTI4 200
ASGNI4
line 259
;259:	removeBotsMenuInfo.arrows.generic.y			= 128;
ADDRGP4 removeBotsMenuInfo+448+16
CNSTI4 128
ASGNI4
line 260
;260:	removeBotsMenuInfo.arrows.width				= 64;
ADDRGP4 removeBotsMenuInfo+448+76
CNSTI4 64
ASGNI4
line 261
;261:	removeBotsMenuInfo.arrows.height			= 128;
ADDRGP4 removeBotsMenuInfo+448+80
CNSTI4 128
ASGNI4
line 263
;262:
;263:	removeBotsMenuInfo.up.generic.type			= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+536
CNSTI4 6
ASGNI4
line 264
;264:	removeBotsMenuInfo.up.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 removeBotsMenuInfo+536+44
CNSTU4 260
ASGNU4
line 265
;265:	removeBotsMenuInfo.up.generic.x				= 200;
ADDRGP4 removeBotsMenuInfo+536+12
CNSTI4 200
ASGNI4
line 266
;266:	removeBotsMenuInfo.up.generic.y				= 128;
ADDRGP4 removeBotsMenuInfo+536+16
CNSTI4 128
ASGNI4
line 267
;267:	removeBotsMenuInfo.up.generic.id			= ID_UP;
ADDRGP4 removeBotsMenuInfo+536+8
CNSTI4 10
ASGNI4
line 268
;268:	removeBotsMenuInfo.up.generic.callback		= UI_RemoveBotsMenu_UpEvent;
ADDRGP4 removeBotsMenuInfo+536+48
ADDRGP4 UI_RemoveBotsMenu_UpEvent
ASGNP4
line 269
;269:	removeBotsMenuInfo.up.width					= 64;
ADDRGP4 removeBotsMenuInfo+536+76
CNSTI4 64
ASGNI4
line 270
;270:	removeBotsMenuInfo.up.height				= 64;
ADDRGP4 removeBotsMenuInfo+536+80
CNSTI4 64
ASGNI4
line 271
;271:	removeBotsMenuInfo.up.focuspic				= ART_ARROWUP;
ADDRGP4 removeBotsMenuInfo+536+60
ADDRGP4 $200
ASGNP4
line 273
;272:
;273:	removeBotsMenuInfo.down.generic.type		= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+624
CNSTI4 6
ASGNI4
line 274
;274:	removeBotsMenuInfo.down.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 removeBotsMenuInfo+624+44
CNSTU4 260
ASGNU4
line 275
;275:	removeBotsMenuInfo.down.generic.x			= 200;
ADDRGP4 removeBotsMenuInfo+624+12
CNSTI4 200
ASGNI4
line 276
;276:	removeBotsMenuInfo.down.generic.y			= 128+64;
ADDRGP4 removeBotsMenuInfo+624+16
CNSTI4 192
ASGNI4
line 277
;277:	removeBotsMenuInfo.down.generic.id			= ID_DOWN;
ADDRGP4 removeBotsMenuInfo+624+8
CNSTI4 11
ASGNI4
line 278
;278:	removeBotsMenuInfo.down.generic.callback	= UI_RemoveBotsMenu_DownEvent;
ADDRGP4 removeBotsMenuInfo+624+48
ADDRGP4 UI_RemoveBotsMenu_DownEvent
ASGNP4
line 279
;279:	removeBotsMenuInfo.down.width				= 64;
ADDRGP4 removeBotsMenuInfo+624+76
CNSTI4 64
ASGNI4
line 280
;280:	removeBotsMenuInfo.down.height				= 64;
ADDRGP4 removeBotsMenuInfo+624+80
CNSTI4 64
ASGNI4
line 281
;281:	removeBotsMenuInfo.down.focuspic			= ART_ARROWDOWN;
ADDRGP4 removeBotsMenuInfo+624+60
ADDRGP4 $218
ASGNP4
line 283
;282:
;283:	for( n = 0, y = 120; n < count; n++, y += 20 ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 8
CNSTI4 120
ASGNI4
ADDRGP4 $222
JUMPV
LABELV $219
line 284
;284:		removeBotsMenuInfo.bots[n].generic.type		= MTYPE_PTEXT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712
ADDP4
CNSTI4 9
ASGNI4
line 285
;285:		removeBotsMenuInfo.bots[n].generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+44
ADDP4
CNSTU4 260
ASGNU4
line 286
;286:		removeBotsMenuInfo.bots[n].generic.id		= ID_BOTNAME0 + n;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
line 287
;287:		removeBotsMenuInfo.bots[n].generic.x		= 320 - 56;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+12
ADDP4
CNSTI4 264
ASGNI4
line 288
;288:		removeBotsMenuInfo.bots[n].generic.y		= y;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+16
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 289
;289:		removeBotsMenuInfo.bots[n].generic.callback	= UI_RemoveBotsMenu_BotEvent;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+48
ADDP4
ADDRGP4 UI_RemoveBotsMenu_BotEvent
ASGNP4
line 290
;290:		removeBotsMenuInfo.bots[n].string			= removeBotsMenuInfo.botnames[n];
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+60
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 removeBotsMenuInfo+1404
ADDP4
ASGNP4
line 291
;291:		removeBotsMenuInfo.bots[n].color			= color_orange;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 292
;292:		removeBotsMenuInfo.bots[n].style			= UI_LEFT|UI_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712+64
ADDP4
CNSTI4 16
ASGNI4
line 293
;293:	}
LABELV $220
line 283
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
LABELV $222
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $219
line 295
;294:
;295:	removeBotsMenuInfo.delete.generic.type		= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+1216
CNSTI4 6
ASGNI4
line 296
;296:	removeBotsMenuInfo.delete.generic.name		= ART_DELETE0;
ADDRGP4 removeBotsMenuInfo+1216+4
ADDRGP4 $134
ASGNP4
line 297
;297:	removeBotsMenuInfo.delete.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 removeBotsMenuInfo+1216+44
CNSTU4 260
ASGNU4
line 298
;298:	removeBotsMenuInfo.delete.generic.id		= ID_DELETE;
ADDRGP4 removeBotsMenuInfo+1216+8
CNSTI4 12
ASGNI4
line 299
;299:	removeBotsMenuInfo.delete.generic.callback	= UI_RemoveBotsMenu_DeleteEvent;
ADDRGP4 removeBotsMenuInfo+1216+48
ADDRGP4 UI_RemoveBotsMenu_DeleteEvent
ASGNP4
line 300
;300:	removeBotsMenuInfo.delete.generic.x			= 320+128-128;
ADDRGP4 removeBotsMenuInfo+1216+12
CNSTI4 320
ASGNI4
line 301
;301:	removeBotsMenuInfo.delete.generic.y			= 256+128-64;
ADDRGP4 removeBotsMenuInfo+1216+16
CNSTI4 320
ASGNI4
line 302
;302:	removeBotsMenuInfo.delete.width  			= 128;
ADDRGP4 removeBotsMenuInfo+1216+76
CNSTI4 128
ASGNI4
line 303
;303:	removeBotsMenuInfo.delete.height  			= 64;
ADDRGP4 removeBotsMenuInfo+1216+80
CNSTI4 64
ASGNI4
line 304
;304:	removeBotsMenuInfo.delete.focuspic			= ART_DELETE1;
ADDRGP4 removeBotsMenuInfo+1216+60
ADDRGP4 $135
ASGNP4
line 306
;305:
;306:	removeBotsMenuInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 removeBotsMenuInfo+1304
CNSTI4 6
ASGNI4
line 307
;307:	removeBotsMenuInfo.back.generic.name		= ART_BACK0;
ADDRGP4 removeBotsMenuInfo+1304+4
ADDRGP4 $132
ASGNP4
line 308
;308:	removeBotsMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 removeBotsMenuInfo+1304+44
CNSTU4 260
ASGNU4
line 309
;309:	removeBotsMenuInfo.back.generic.id			= ID_BACK;
ADDRGP4 removeBotsMenuInfo+1304+8
CNSTI4 13
ASGNI4
line 310
;310:	removeBotsMenuInfo.back.generic.callback	= UI_RemoveBotsMenu_BackEvent;
ADDRGP4 removeBotsMenuInfo+1304+48
ADDRGP4 UI_RemoveBotsMenu_BackEvent
ASGNP4
line 311
;311:	removeBotsMenuInfo.back.generic.x			= 320-128;
ADDRGP4 removeBotsMenuInfo+1304+12
CNSTI4 192
ASGNI4
line 312
;312:	removeBotsMenuInfo.back.generic.y			= 256+128-64;
ADDRGP4 removeBotsMenuInfo+1304+16
CNSTI4 320
ASGNI4
line 313
;313:	removeBotsMenuInfo.back.width				= 128;
ADDRGP4 removeBotsMenuInfo+1304+76
CNSTI4 128
ASGNI4
line 314
;314:	removeBotsMenuInfo.back.height				= 64;
ADDRGP4 removeBotsMenuInfo+1304+80
CNSTI4 64
ASGNI4
line 315
;315:	removeBotsMenuInfo.back.focuspic			= ART_BACK1;
ADDRGP4 removeBotsMenuInfo+1304+60
ADDRGP4 $133
ASGNP4
line 317
;316:
;317:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.background );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 318
;318:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.banner );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 319
;319:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.arrows );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 320
;320:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.up );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 321
;321:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.down );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+624
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 322
;322:	for( n = 0; n < count; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $287
JUMPV
LABELV $284
line 323
;323:		Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.bots[n] );
ADDRGP4 removeBotsMenuInfo
ARGP4
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 removeBotsMenuInfo+712
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 324
;324:	}
LABELV $285
line 322
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $287
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $284
line 325
;325:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.delete );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+1216
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 326
;326:	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.back );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 removeBotsMenuInfo+1304
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 328
;327:
;328:	removeBotsMenuInfo.baseBotNum = 0;
ADDRGP4 removeBotsMenuInfo+1396
CNSTI4 0
ASGNI4
line 329
;329:	removeBotsMenuInfo.selectedBotNum = 0;
ADDRGP4 removeBotsMenuInfo+1400
CNSTI4 0
ASGNI4
line 330
;330:	removeBotsMenuInfo.bots[0].color = color_white;
ADDRGP4 removeBotsMenuInfo+712+68
ADDRGP4 color_white
ASGNP4
line 331
;331:}
LABELV $136
endproc UI_RemoveBotsMenu_Init 24 12
export UI_RemoveBotsMenu
proc UI_RemoveBotsMenu 0 4
line 339
;332:
;333:
;334:/*
;335:=================
;336:UI_RemoveBotsMenu
;337:=================
;338:*/
;339:void UI_RemoveBotsMenu( void ) {
line 340
;340:	UI_RemoveBotsMenu_Init();
ADDRGP4 UI_RemoveBotsMenu_Init
CALLV
pop
line 341
;341:	UI_PushMenu( &removeBotsMenuInfo.menu );
ADDRGP4 removeBotsMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 342
;342:}
LABELV $295
endproc UI_RemoveBotsMenu 0 4
bss
align 4
LABELV removeBotsMenuInfo
skip 5724
import UI_RankStatusMenu
import RankStatus_Cache
import UI_SignupMenu
import Signup_Cache
import UI_LoginMenu
import Login_Cache
import UI_RankingsMenu
import Rankings_Cache
import Rankings_DrawPassword
import Rankings_DrawName
import Rankings_DrawText
import UI_InitGameinfo
import UI_SPUnlockMedals_f
import UI_SPUnlock_f
import UI_GetAwardLevel
import UI_LogAwardData
import UI_NewGame
import UI_GetCurrentGame
import UI_CanShowTierVideo
import UI_ShowTierVideo
import UI_TierCompleted
import UI_SetBestScore
import UI_GetBestScore
import UI_GetNumBots
import UI_GetBotInfoByName
import UI_GetBotInfoByNumber
import UI_GetNumSPTiers
import UI_GetNumSPArenas
import UI_GetNumArenas
import UI_GetSpecialArenaInfo
import UI_GetArenaInfoByMap
import UI_GetArenaInfoByNumber
import UI_NetworkOptionsMenu
import UI_NetworkOptionsMenu_Cache
import UI_SoundOptionsMenu
import UI_SoundOptionsMenu_Cache
import UI_DisplayOptionsMenu
import UI_DisplayOptionsMenu_Cache
import UI_SaveConfigMenu
import UI_SaveConfigMenu_Cache
import UI_LoadConfigMenu
import UI_LoadConfig_Cache
import UI_TeamOrdersMenu_Cache
import UI_TeamOrdersMenu_f
import UI_TeamOrdersMenu
import UI_AddBotsMenu
import UI_AddBots_Cache
import trap_SetPbClStatus
import trap_VerifyCDKey
import trap_SetCDKey
import trap_GetCDKey
import trap_MemoryRemaining
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_ServerStatus
import trap_LAN_GetPingQueueCount
import trap_LAN_GetServerInfo
import trap_LAN_GetServerAddressString
import trap_LAN_GetServerCount
import trap_GetConfigString
import trap_GetGlconfig
import trap_GetClientState
import trap_GetClipboardData
import trap_Key_SetCatcher
import trap_Key_GetCatcher
import trap_Key_ClearStates
import trap_Key_SetOverstrikeMode
import trap_Key_GetOverstrikeMode
import trap_Key_IsDown
import trap_Key_SetBinding
import trap_Key_GetBindingBuf
import trap_Key_KeynumToStringBuf
import trap_S_RegisterSound
import trap_S_StartLocalSound
import trap_CM_LerpTag
import trap_UpdateScreen
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_AddLightToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_R_RegisterShaderNoMip
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_FS_Seek
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Cmd_ExecuteText
import trap_Argv
import trap_Argc
import trap_Cvar_InfoStringBuffer
import trap_Cvar_Create
import trap_Cvar_Reset
import trap_Cvar_SetValue
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_Milliseconds
import trap_Error
import trap_Print
import UI_SPSkillMenu_Cache
import UI_SPSkillMenu
import UI_SPPostgameMenu_f
import UI_SPPostgameMenu_Cache
import UI_SPArena_Start
import UI_SPLevelMenu_ReInit
import UI_SPLevelMenu_f
import UI_SPLevelMenu
import UI_SPLevelMenu_Cache
import uis
import m_entersound
import UI_StartDemoLoop
import UI_Cvar_VariableString
import UI_Argv
import UI_ForceMenuOff
import UI_PopMenu
import UI_PushMenu
import UI_SetActiveMenu
import UI_IsFullscreen
import UI_DrawTextBox
import UI_AdjustFrom640
import UI_CursorInRect
import UI_DrawChar
import UI_DrawString
import UI_ProportionalStringWidth
import UI_DrawProportionalString_AutoWrapped
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_SetColor
import UI_UpdateScreen
import UI_DrawRect
import UI_FillRect
import UI_DrawHandlePic
import UI_DrawNamedPic
import UI_ClampCvar
import UI_ConsoleCommand
import UI_Refresh
import UI_MouseEvent
import UI_KeyEvent
import UI_Shutdown
import UI_Init
import UI_RegisterClientModelname
import UI_PlayerInfo_SetInfo
import UI_PlayerInfo_SetModel
import UI_DrawPlayer
import DriverInfo_Cache
import GraphicsOptions_Cache
import UI_GraphicsOptionsMenu
import ServerInfo_Cache
import UI_ServerInfoMenu
import UI_BotSelectMenu_Cache
import UI_BotSelectMenu
import ServerOptions_Cache
import StartServer_Cache
import UI_StartServerMenu
import ArenaServers_Cache
import UI_ArenaServersMenu
import SpecifyServer_Cache
import UI_SpecifyServerMenu
import SpecifyLeague_Cache
import UI_SpecifyLeagueMenu
import Preferences_Cache
import UI_PreferencesMenu
import PlayerSettings_Cache
import UI_PlayerSettingsMenu
import PlayerModel_Cache
import UI_PlayerModelMenu
import UI_CDKeyMenu_f
import UI_CDKeyMenu_Cache
import UI_CDKeyMenu
import UI_ModsMenu_Cache
import UI_ModsMenu
import UI_CinematicsMenu_Cache
import UI_CinematicsMenu_f
import UI_CinematicsMenu
import Demos_Cache
import UI_DemosMenu
import Controls_Cache
import UI_ControlsMenu
import UI_DrawConnectScreen
import TeamMain_Cache
import UI_TeamMainMenu
import UI_SetupMenu
import UI_SetupMenu_Cache
import UI_Message
import UI_ConfirmMenu_Style
import UI_ConfirmMenu
import ConfirmMenu_Cache
import UI_InGameMenu
import InGame_Cache
import UI_CreditMenu
import UI_UpdateCvars
import UI_RegisterCvars
import UI_MainMenu
import MainMenu_Cache
import MenuField_Key
import MenuField_Draw
import MenuField_Init
import MField_Draw
import MField_CharEvent
import MField_KeyDownEvent
import MField_Clear
import ui_medalSounds
import ui_medalPicNames
import ui_medalNames
import text_color_highlight
import text_color_normal
import text_color_disabled
import listbar_color
import list_color
import name_color
import color_dim
import color_red
import color_orange
import color_blue
import color_yellow
import color_white
import color_black
import menu_dim_color
import menu_black_color
import menu_red_color
import menu_highlight_color
import menu_dark_color
import menu_grayed_color
import menu_text_color
import weaponChangeSound
import menu_null_sound
import menu_buzz_sound
import menu_out_sound
import menu_move_sound
import menu_in_sound
import ScrollList_Key
import ScrollList_Draw
import Bitmap_Draw
import Bitmap_Init
import Menu_DefaultKey
import Menu_SetCursorToItem
import Menu_SetCursor
import Menu_ActivateItem
import Menu_ItemAtCursor
import Menu_Draw
import Menu_AdjustCursor
import Menu_AddItem
import Menu_Focus
import Menu_Cache
import ui_cdkeychecked
import ui_cdkey
import ui_server16
import ui_server15
import ui_server14
import ui_server13
import ui_server12
import ui_server11
import ui_server10
import ui_server9
import ui_server8
import ui_server7
import ui_server6
import ui_server5
import ui_server4
import ui_server3
import ui_server2
import ui_server1
import ui_marks
import ui_drawCrosshairNames
import ui_drawCrosshair
import ui_brassTime
import ui_browserShowEmpty
import ui_browserShowFull
import ui_browserSortKey
import ui_browserGameType
import ui_browserMaster
import ui_spSelection
import ui_spSkill
import ui_spVideos
import ui_spAwards
import ui_spScores5
import ui_spScores4
import ui_spScores3
import ui_spScores2
import ui_spScores1
import ui_botsFile
import ui_arenasFile
import ui_ctf_friendly
import ui_ctf_timelimit
import ui_ctf_capturelimit
import ui_team_friendly
import ui_team_timelimit
import ui_team_fraglimit
import ui_tourney_timelimit
import ui_tourney_fraglimit
import ui_ffa_timelimit
import ui_ffa_fraglimit
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
LABELV $218
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $200
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $172
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $151
byte 1 82
byte 1 69
byte 1 77
byte 1 79
byte 1 86
byte 1 69
byte 1 32
byte 1 66
byte 1 79
byte 1 84
byte 1 83
byte 1 0
align 1
LABELV $135
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $134
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $133
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $132
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $131
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 102
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $118
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
LABELV $85
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 107
byte 1 105
byte 1 99
byte 1 107
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $78
byte 1 110
byte 1 0
