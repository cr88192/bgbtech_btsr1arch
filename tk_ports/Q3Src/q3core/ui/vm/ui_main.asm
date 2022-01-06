data
align 4
LABELV MonthAbbrev
address $87
address $88
address $89
address $90
address $91
address $92
address $93
address $94
address $95
address $96
address $97
address $98
align 4
LABELV skillLevels
address $99
address $100
address $101
address $102
address $103
lit
align 4
LABELV numSkillLevels
byte 4 5
data
align 4
LABELV netSources
address $104
address $105
address $106
address $107
lit
align 4
LABELV numNetSources
byte 4 4
align 4
LABELV serverFilters
address $108
address $109
address $110
address $109
address $111
address $112
address $113
address $114
address $115
address $116
address $117
address $118
address $119
address $120
data
align 4
LABELV teamArenaGameTypes
address $121
address $122
address $123
address $124
address $125
address $126
address $127
address $128
address $129
lit
align 4
LABELV numTeamArenaGameTypes
byte 4 9
data
align 4
LABELV teamArenaGameNames
address $130
address $131
address $132
address $133
address $134
address $135
address $136
address $137
address $138
lit
align 4
LABELV numTeamArenaGameNames
byte 4 9
align 4
LABELV numServerFilters
byte 4 7
data
align 4
LABELV sortKeys
address $139
address $140
address $141
address $142
address $143
lit
align 4
LABELV numSortKeys
byte 4 5
data
align 4
LABELV netnames
address $144
address $145
address $146
byte 4 0
align 4
LABELV gamecodetoui
byte 4 4
byte 4 2
byte 4 3
byte 4 0
byte 4 5
byte 4 1
byte 4 6
align 4
LABELV uitogamecode
byte 4 4
byte 4 6
byte 4 2
byte 4 3
byte 4 1
byte 4 5
byte 4 7
export vmMain
code
proc vmMain 12 8
file "../ui_main.c"
line 168
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
;26:USER INTERFACE MAIN
;27:
;28:=======================================================================
;29:*/
;30:
;31:// use this to get a demo build without an explicit demo build, i.e. to get the demo ui files to build
;32://#define PRE_RELEASE_TADEMO
;33:
;34:#include "ui_local.h"
;35:
;36:uiInfo_t uiInfo;
;37:
;38:static const char *MonthAbbrev[] = {
;39:	"Jan","Feb","Mar",
;40:	"Apr","May","Jun",
;41:	"Jul","Aug","Sep",
;42:	"Oct","Nov","Dec"
;43:};
;44:
;45:
;46:static const char *skillLevels[] = {
;47:  "I Can Win",
;48:  "Bring It On",
;49:  "Hurt Me Plenty",
;50:  "Hardcore",
;51:  "Nightmare"
;52:};
;53:
;54:static const int numSkillLevels = sizeof(skillLevels) / sizeof(const char*);
;55:
;56:
;57:static const char *netSources[] = {
;58:	"Local",
;59:	"Mplayer",
;60:	"Internet",
;61:	"Favorites"
;62:};
;63:static const int numNetSources = sizeof(netSources) / sizeof(const char*);
;64:
;65:static const serverFilter_t serverFilters[] = {
;66:	{"All", "" },
;67:	{"Quake 3 Arena", "" },
;68:	{"Team Arena", "missionpack" },
;69:	{"Rocket Arena", "arena" },
;70:	{"Alliance", "alliance20" },
;71:	{"Weapons Factory Arena", "wfa" },
;72:	{"OSP", "osp" },
;73:};
;74:
;75:static const char *teamArenaGameTypes[] = {
;76:	"FFA",
;77:	"TOURNAMENT",
;78:	"SP",
;79:	"TEAM DM",
;80:	"CTF",
;81:	"1FCTF",
;82:	"OVERLOAD",
;83:	"HARVESTER",
;84:	"TEAMTOURNAMENT"
;85:};
;86:
;87:static int const numTeamArenaGameTypes = sizeof(teamArenaGameTypes) / sizeof(const char*);
;88:
;89:
;90:static const char *teamArenaGameNames[] = {
;91:	"Free For All",
;92:	"Tournament",
;93:	"Single Player",
;94:	"Team Deathmatch",
;95:	"Capture the Flag",
;96:	"One Flag CTF",
;97:	"Overload",
;98:	"Harvester",
;99:	"Team Tournament",
;100:};
;101:
;102:static int const numTeamArenaGameNames = sizeof(teamArenaGameNames) / sizeof(const char*);
;103:
;104:
;105:static const int numServerFilters = sizeof(serverFilters) / sizeof(serverFilter_t);
;106:
;107:static const char *sortKeys[] = {
;108:	"Server Name",
;109:	"Map Name",
;110:	"Open Player Spots",
;111:	"Game Type",
;112:	"Ping Time"
;113:};
;114:static const int numSortKeys = sizeof(sortKeys) / sizeof(const char*);
;115:
;116:static char* netnames[] = {
;117:	"???",
;118:	"UDP",
;119:	"IPX",
;120:	NULL
;121:};
;122:
;123:#ifndef MISSIONPACK // bk001206
;124:static char quake3worldMessage[] = "Visit www.quake3world.com - News, Community, Events, Files";
;125:#endif
;126:
;127:static int gamecodetoui[] = {4,2,3,0,5,1,6};
;128:static int uitogamecode[] = {4,6,2,3,1,5,7};
;129:
;130:
;131:static void UI_StartServerRefresh(qboolean full);
;132:static void UI_StopServerRefresh( void );
;133:static void UI_DoServerRefresh( void );
;134:static void UI_FeederSelection(float feederID, int index);
;135:static void UI_BuildServerDisplayList(qboolean force);
;136:static void UI_BuildServerStatus(qboolean force);
;137:static void UI_BuildFindPlayerList(qboolean force);
;138:static int QDECL UI_ServersQsortCompare( const void *arg1, const void *arg2 );
;139:static int UI_MapCountByGameType(qboolean singlePlayer);
;140:static int UI_HeadCountByTeam( void );
;141:static void UI_ParseGameInfo(const char *teamFile);
;142:static void UI_ParseTeamInfo(const char *teamFile);
;143:static const char *UI_SelectedMap(int index, int *actual);
;144:static const char *UI_SelectedHead(int index, int *actual);
;145:static int UI_GetIndexFromSelection(int actual);
;146:
;147:int ProcessNewUI( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6 );
;148:
;149:/*
;150:================
;151:vmMain
;152:
;153:This is the only way control passes into the module.
;154:This must be the very first function compiled into the .qvm file
;155:================
;156:*/
;157:vmCvar_t  ui_new;
;158:vmCvar_t  ui_debug;
;159:vmCvar_t  ui_initialized;
;160:vmCvar_t  ui_teamArenaFirstRun;
;161:
;162:void _UI_Init( qboolean );
;163:void _UI_Shutdown( void );
;164:void _UI_KeyEvent( int key, qboolean down );
;165:void _UI_MouseEvent( int dx, int dy );
;166:void _UI_Refresh( int realtime );
;167:qboolean _UI_IsFullscreen( void );
;168:int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
line 169
;169:  switch ( command ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $148
ADDRLP4 0
INDIRI4
CNSTI4 10
GTI4 $148
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $161
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $161
address $150
address $151
address $152
address $153
address $154
address $155
address $156
address $157
address $158
address $159
address $160
code
LABELV $150
line 171
;170:	  case UI_GETAPIVERSION:
;171:		  return UI_API_VERSION;
CNSTI4 6
RETI4
ADDRGP4 $147
JUMPV
LABELV $151
line 174
;172:
;173:	  case UI_INIT:
;174:		  _UI_Init(arg0);
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 _UI_Init
CALLV
pop
line 175
;175:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $152
line 178
;176:
;177:	  case UI_SHUTDOWN:
;178:		  _UI_Shutdown();
ADDRGP4 _UI_Shutdown
CALLV
pop
line 179
;179:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $153
line 182
;180:
;181:	  case UI_KEY_EVENT:
;182:		  _UI_KeyEvent( arg0, arg1 );
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 _UI_KeyEvent
CALLV
pop
line 183
;183:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $154
line 186
;184:
;185:	  case UI_MOUSE_EVENT:
;186:		  _UI_MouseEvent( arg0, arg1 );
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 _UI_MouseEvent
CALLV
pop
line 187
;187:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $155
line 190
;188:
;189:	  case UI_REFRESH:
;190:		  _UI_Refresh( arg0 );
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 _UI_Refresh
CALLV
pop
line 191
;191:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $156
line 194
;192:
;193:	  case UI_IS_FULLSCREEN:
;194:		  return _UI_IsFullscreen();
ADDRLP4 4
ADDRGP4 _UI_IsFullscreen
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
RETI4
ADDRGP4 $147
JUMPV
LABELV $157
line 197
;195:
;196:	  case UI_SET_ACTIVE_MENU:
;197:		  _UI_SetActiveMenu( arg0 );
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 _UI_SetActiveMenu
CALLV
pop
line 198
;198:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $158
line 201
;199:
;200:	  case UI_CONSOLE_COMMAND:
;201:		  return UI_ConsoleCommand(arg0);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 UI_ConsoleCommand
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
ADDRGP4 $147
JUMPV
LABELV $159
line 204
;202:
;203:	  case UI_DRAW_CONNECT_SCREEN:
;204:		  UI_DrawConnectScreen( arg0 );
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 UI_DrawConnectScreen
CALLV
pop
line 205
;205:		  return 0;
CNSTI4 0
RETI4
ADDRGP4 $147
JUMPV
LABELV $160
line 207
;206:	  case UI_HASUNIQUECDKEY: // mod authors need to observe this
;207:	    return qtrue; // bk010117 - change this to qfalse for mods!
CNSTI4 1
RETI4
ADDRGP4 $147
JUMPV
LABELV $148
line 211
;208:
;209:	}
;210:
;211:	return -1;
CNSTI4 -1
RETI4
LABELV $147
endproc vmMain 12 8
export AssetCache
proc AssetCache 84 8
line 216
;212:}
;213:
;214:
;215:
;216:void AssetCache() {
line 222
;217:	int n;
;218:	//if (Assets.textFont == NULL) {
;219:	//}
;220:	//Assets.background = trap_R_RegisterShaderNoMip( ASSET_BACKGROUND );
;221:	//Com_Printf("Menu Size: %i bytes\n", sizeof(Menus));
;222:	uiInfo.uiDC.Assets.gradientBar = trap_R_RegisterShaderNoMip( ASSET_GRADIENTBAR );
ADDRGP4 $165
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61660
ADDRLP4 4
INDIRI4
ASGNI4
line 223
;223:	uiInfo.uiDC.Assets.fxBasePic = trap_R_RegisterShaderNoMip( ART_FX_BASE );
ADDRGP4 $168
ARGP4
ADDRLP4 8
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61768
ADDRLP4 8
INDIRI4
ASGNI4
line 224
;224:	uiInfo.uiDC.Assets.fxPic[0] = trap_R_RegisterShaderNoMip( ART_FX_RED );
ADDRGP4 $171
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772
ADDRLP4 12
INDIRI4
ASGNI4
line 225
;225:	uiInfo.uiDC.Assets.fxPic[1] = trap_R_RegisterShaderNoMip( ART_FX_YELLOW );
ADDRGP4 $175
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+4
ADDRLP4 16
INDIRI4
ASGNI4
line 226
;226:	uiInfo.uiDC.Assets.fxPic[2] = trap_R_RegisterShaderNoMip( ART_FX_GREEN );
ADDRGP4 $179
ARGP4
ADDRLP4 20
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+8
ADDRLP4 20
INDIRI4
ASGNI4
line 227
;227:	uiInfo.uiDC.Assets.fxPic[3] = trap_R_RegisterShaderNoMip( ART_FX_TEAL );
ADDRGP4 $183
ARGP4
ADDRLP4 24
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+12
ADDRLP4 24
INDIRI4
ASGNI4
line 228
;228:	uiInfo.uiDC.Assets.fxPic[4] = trap_R_RegisterShaderNoMip( ART_FX_BLUE );
ADDRGP4 $187
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+16
ADDRLP4 28
INDIRI4
ASGNI4
line 229
;229:	uiInfo.uiDC.Assets.fxPic[5] = trap_R_RegisterShaderNoMip( ART_FX_CYAN );
ADDRGP4 $191
ARGP4
ADDRLP4 32
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+20
ADDRLP4 32
INDIRI4
ASGNI4
line 230
;230:	uiInfo.uiDC.Assets.fxPic[6] = trap_R_RegisterShaderNoMip( ART_FX_WHITE );
ADDRGP4 $195
ARGP4
ADDRLP4 36
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61772+24
ADDRLP4 36
INDIRI4
ASGNI4
line 231
;231:	uiInfo.uiDC.Assets.scrollBar = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR );
ADDRGP4 $198
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61680
ADDRLP4 40
INDIRI4
ASGNI4
line 232
;232:	uiInfo.uiDC.Assets.scrollBarArrowDown = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWDOWN );
ADDRGP4 $201
ARGP4
ADDRLP4 44
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61668
ADDRLP4 44
INDIRI4
ASGNI4
line 233
;233:	uiInfo.uiDC.Assets.scrollBarArrowUp = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWUP );
ADDRGP4 $204
ARGP4
ADDRLP4 48
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61664
ADDRLP4 48
INDIRI4
ASGNI4
line 234
;234:	uiInfo.uiDC.Assets.scrollBarArrowLeft = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWLEFT );
ADDRGP4 $207
ARGP4
ADDRLP4 52
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61672
ADDRLP4 52
INDIRI4
ASGNI4
line 235
;235:	uiInfo.uiDC.Assets.scrollBarArrowRight = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWRIGHT );
ADDRGP4 $210
ARGP4
ADDRLP4 56
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61676
ADDRLP4 56
INDIRI4
ASGNI4
line 236
;236:	uiInfo.uiDC.Assets.scrollBarThumb = trap_R_RegisterShaderNoMip( ASSET_SCROLL_THUMB );
ADDRGP4 $213
ARGP4
ADDRLP4 60
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61684
ADDRLP4 60
INDIRI4
ASGNI4
line 237
;237:	uiInfo.uiDC.Assets.sliderBar = trap_R_RegisterShaderNoMip( ASSET_SLIDER_BAR );
ADDRGP4 $216
ARGP4
ADDRLP4 64
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61700
ADDRLP4 64
INDIRI4
ASGNI4
line 238
;238:	uiInfo.uiDC.Assets.sliderThumb = trap_R_RegisterShaderNoMip( ASSET_SLIDER_THUMB );
ADDRGP4 $219
ARGP4
ADDRLP4 68
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61704
ADDRLP4 68
INDIRI4
ASGNI4
line 240
;239:
;240:	for( n = 0; n < NUM_CROSSHAIRS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $220
line 241
;241:		uiInfo.uiDC.Assets.crosshairShader[n] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a' + n ) );
ADDRGP4 $226
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 97
ADDI4
ARGI4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+228+61800
ADDP4
ADDRLP4 80
INDIRI4
ASGNI4
line 242
;242:	}
LABELV $221
line 240
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 10
LTI4 $220
line 244
;243:
;244:	uiInfo.newHighScoreSound = trap_S_RegisterSound("sound/feedback/voc_newhighscore.wav", qfalse);
ADDRGP4 $228
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 72
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRGP4 uiInfo+119484
ADDRLP4 72
INDIRI4
ASGNI4
line 245
;245:}
LABELV $162
endproc AssetCache 84 8
export _UI_DrawSides
proc _UI_DrawSides 12 36
line 247
;246:
;247:void _UI_DrawSides(float x, float y, float w, float h, float size) {
line 248
;248:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 249
;249:	size *= uiInfo.uiDC.xscale;
ADDRFP4 16
ADDRFP4 16
INDIRF4
ADDRGP4 uiInfo+200
INDIRF4
MULF4
ASGNF4
line 250
;250:	trap_R_DrawStretchPic( x, y, size, h, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 251
;251:	trap_R_DrawStretchPic( x + w - size, y, size, h, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRLP4 4
ADDRFP4 16
INDIRF4
ASGNF4
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRLP4 4
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 252
;252:}
LABELV $229
endproc _UI_DrawSides 12 36
export _UI_DrawTopBottom
proc _UI_DrawTopBottom 12 36
line 254
;253:
;254:void _UI_DrawTopBottom(float x, float y, float w, float h, float size) {
line 255
;255:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 256
;256:	size *= uiInfo.uiDC.yscale;
ADDRFP4 16
ADDRFP4 16
INDIRF4
ADDRGP4 uiInfo+196
INDIRF4
MULF4
ASGNF4
line 257
;257:	trap_R_DrawStretchPic( x, y, w, size, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 258
;258:	trap_R_DrawStretchPic( x, y + h - size, w, size, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRLP4 4
ADDRFP4 16
INDIRF4
ASGNF4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ADDRLP4 4
INDIRF4
SUBF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 259
;259:}
LABELV $233
endproc _UI_DrawTopBottom 12 36
export _UI_DrawRect
proc _UI_DrawRect 0 20
line 267
;260:/*
;261:================
;262:UI_DrawRect
;263:
;264:Coordinates are 640*480 virtual values
;265:=================
;266:*/
;267:void _UI_DrawRect( float x, float y, float width, float height, float size, const float *color ) {
line 268
;268:	trap_R_SetColor( color );
ADDRFP4 20
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 270
;269:
;270:  _UI_DrawTopBottom(x, y, width, height, size);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRGP4 _UI_DrawTopBottom
CALLV
pop
line 271
;271:  _UI_DrawSides(x, y, width, height, size);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRGP4 _UI_DrawSides
CALLV
pop
line 273
;272:
;273:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 274
;274:}
LABELV $237
endproc _UI_DrawRect 0 20
export Text_Width
proc Text_Width 48 4
line 276
;275:
;276:int Text_Width(const char *text, float scale, int limit) {
line 281
;277:  int count,len;
;278:	float out;
;279:	glyphInfo_t *glyph;
;280:	float useScale;
;281:	const char *s = text;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 282
;282:	fontInfo_t *font = &uiInfo.uiDC.Assets.textFont;
ADDRLP4 12
ADDRGP4 uiInfo+228+12
ASGNP4
line 283
;283:	if (scale <= ui_smallFont.value) {
ADDRFP4 4
INDIRF4
ADDRGP4 ui_smallFont+8
INDIRF4
GTF4 $241
line 284
;284:		font = &uiInfo.uiDC.Assets.smallFont;
ADDRLP4 12
ADDRGP4 uiInfo+228+20560
ASGNP4
line 285
;285:	} else if (scale >= ui_bigFont.value) {
ADDRGP4 $242
JUMPV
LABELV $241
ADDRFP4 4
INDIRF4
ADDRGP4 ui_bigFont+8
INDIRF4
LTF4 $246
line 286
;286:		font = &uiInfo.uiDC.Assets.bigFont;
ADDRLP4 12
ADDRGP4 uiInfo+228+41108
ASGNP4
line 287
;287:	}
LABELV $246
LABELV $242
line 288
;288:	useScale = scale * font->glyphScale;
ADDRLP4 24
ADDRFP4 4
INDIRF4
ADDRLP4 12
INDIRP4
CNSTI4 20480
ADDP4
INDIRF4
MULF4
ASGNF4
line 289
;289:  out = 0;
ADDRLP4 20
CNSTF4 0
ASGNF4
line 290
;290:  if (text) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $251
line 291
;291:    len = strlen(text);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 28
INDIRI4
ASGNI4
line 292
;292:		if (limit > 0 && len > limit) {
ADDRLP4 32
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
LEI4 $253
ADDRLP4 8
INDIRI4
ADDRLP4 32
INDIRI4
LEI4 $253
line 293
;293:			len = limit;
ADDRLP4 8
ADDRFP4 8
INDIRI4
ASGNI4
line 294
;294:		}
LABELV $253
line 295
;295:		count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $256
JUMPV
LABELV $255
line 296
;296:		while (s && *s && count < len) {
line 297
;297:			if ( Q_IsColorString(s) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $258
ADDRLP4 40
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 40
INDIRI4
NEI4 $258
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $258
ADDRLP4 44
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $258
line 298
;298:				s += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 299
;299:				continue;
ADDRGP4 $256
JUMPV
LABELV $258
line 300
;300:			} else {
line 301
;301:				glyph = &font->glyphs[(int)*s];
ADDRLP4 16
CNSTI4 80
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
MULI4
ADDRLP4 12
INDIRP4
ADDP4
ASGNP4
line 302
;302:				out += glyph->xSkip;
ADDRLP4 20
ADDRLP4 20
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 303
;303:				s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 304
;304:				count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 305
;305:			}
line 306
;306:    }
LABELV $256
line 296
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $261
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $261
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $255
LABELV $261
line 307
;307:  }
LABELV $251
line 308
;308:  return out * useScale;
ADDRLP4 20
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
CVFI4 4
RETI4
LABELV $238
endproc Text_Width 48 4
export Text_Height
proc Text_Height 48 4
line 311
;309:}
;310:
;311:int Text_Height(const char *text, float scale, int limit) {
line 316
;312:  int len, count;
;313:	float max;
;314:	glyphInfo_t *glyph;
;315:	float useScale;
;316:	const char *s = text; // bk001206 - unsigned
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 317
;317:	fontInfo_t *font = &uiInfo.uiDC.Assets.textFont;
ADDRLP4 20
ADDRGP4 uiInfo+228+12
ASGNP4
line 318
;318:	if (scale <= ui_smallFont.value) {
ADDRFP4 4
INDIRF4
ADDRGP4 ui_smallFont+8
INDIRF4
GTF4 $265
line 319
;319:		font = &uiInfo.uiDC.Assets.smallFont;
ADDRLP4 20
ADDRGP4 uiInfo+228+20560
ASGNP4
line 320
;320:	} else if (scale >= ui_bigFont.value) {
ADDRGP4 $266
JUMPV
LABELV $265
ADDRFP4 4
INDIRF4
ADDRGP4 ui_bigFont+8
INDIRF4
LTF4 $270
line 321
;321:		font = &uiInfo.uiDC.Assets.bigFont;
ADDRLP4 20
ADDRGP4 uiInfo+228+41108
ASGNP4
line 322
;322:	}
LABELV $270
LABELV $266
line 323
;323:	useScale = scale * font->glyphScale;
ADDRLP4 24
ADDRFP4 4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 20480
ADDP4
INDIRF4
MULF4
ASGNF4
line 324
;324:  max = 0;
ADDRLP4 16
CNSTF4 0
ASGNF4
line 325
;325:  if (text) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $275
line 326
;326:    len = strlen(text);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 28
INDIRI4
ASGNI4
line 327
;327:		if (limit > 0 && len > limit) {
ADDRLP4 32
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
LEI4 $277
ADDRLP4 8
INDIRI4
ADDRLP4 32
INDIRI4
LEI4 $277
line 328
;328:			len = limit;
ADDRLP4 8
ADDRFP4 8
INDIRI4
ASGNI4
line 329
;329:		}
LABELV $277
line 330
;330:		count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $280
JUMPV
LABELV $279
line 331
;331:		while (s && *s && count < len) {
line 332
;332:			if ( Q_IsColorString(s) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $282
ADDRLP4 40
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 40
INDIRI4
NEI4 $282
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $282
ADDRLP4 44
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $282
line 333
;333:				s += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 334
;334:				continue;
ADDRGP4 $280
JUMPV
LABELV $282
line 335
;335:			} else {
line 336
;336:				glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
ADDRLP4 12
CNSTI4 80
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
MULI4
ADDRLP4 20
INDIRP4
ADDP4
ASGNP4
line 337
;337:	      if (max < glyph->height) {
ADDRLP4 16
INDIRF4
ADDRLP4 12
INDIRP4
INDIRI4
CVIF4 4
GEF4 $284
line 338
;338:		      max = glyph->height;
ADDRLP4 16
ADDRLP4 12
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
line 339
;339:			  }
LABELV $284
line 340
;340:				s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 341
;341:				count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 342
;342:			}
line 343
;343:    }
LABELV $280
line 331
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $287
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $287
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $279
LABELV $287
line 344
;344:  }
LABELV $275
line 345
;345:  return max * useScale;
ADDRLP4 16
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
CVFI4 4
RETI4
LABELV $262
endproc Text_Height 48 4
export Text_PaintChar
proc Text_PaintChar 8 36
line 348
;346:}
;347:
;348:void Text_PaintChar(float x, float y, float width, float height, float scale, float s, float t, float s2, float t2, qhandle_t hShader) {
line 350
;349:  float w, h;
;350:  w = width * scale;
ADDRLP4 0
ADDRFP4 8
INDIRF4
ADDRFP4 16
INDIRF4
MULF4
ASGNF4
line 351
;351:  h = height * scale;
ADDRLP4 4
ADDRFP4 12
INDIRF4
ADDRFP4 16
INDIRF4
MULF4
ASGNF4
line 352
;352:  UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 353
;353:  trap_R_DrawStretchPic( x, y, w, h, s, t, s2, t2, hShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRFP4 20
INDIRF4
ARGF4
ADDRFP4 24
INDIRF4
ARGF4
ADDRFP4 28
INDIRF4
ARGF4
ADDRFP4 32
INDIRF4
ARGF4
ADDRFP4 36
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 354
;354:}
LABELV $288
endproc Text_PaintChar 8 36
export Text_Paint
proc Text_Paint 84 40
line 356
;355:
;356:void Text_Paint(float x, float y, float scale, vec4_t color, const char *text, float adjust, int limit, int style) {
line 361
;357:  int len, count;
;358:	vec4_t newColor;
;359:	glyphInfo_t *glyph;
;360:	float useScale;
;361:	fontInfo_t *font = &uiInfo.uiDC.Assets.textFont;
ADDRLP4 28
ADDRGP4 uiInfo+228+12
ASGNP4
line 362
;362:	if (scale <= ui_smallFont.value) {
ADDRFP4 8
INDIRF4
ADDRGP4 ui_smallFont+8
INDIRF4
GTF4 $292
line 363
;363:		font = &uiInfo.uiDC.Assets.smallFont;
ADDRLP4 28
ADDRGP4 uiInfo+228+20560
ASGNP4
line 364
;364:	} else if (scale >= ui_bigFont.value) {
ADDRGP4 $293
JUMPV
LABELV $292
ADDRFP4 8
INDIRF4
ADDRGP4 ui_bigFont+8
INDIRF4
LTF4 $297
line 365
;365:		font = &uiInfo.uiDC.Assets.bigFont;
ADDRLP4 28
ADDRGP4 uiInfo+228+41108
ASGNP4
line 366
;366:	}
LABELV $297
LABELV $293
line 367
;367:	useScale = scale * font->glyphScale;
ADDRLP4 20
ADDRFP4 8
INDIRF4
ADDRLP4 28
INDIRP4
CNSTI4 20480
ADDP4
INDIRF4
MULF4
ASGNF4
line 368
;368:  if (text) {
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $302
line 369
;369:    const char *s = text; // bk001206 - unsigned
ADDRLP4 36
ADDRFP4 16
INDIRP4
ASGNP4
line 370
;370:		trap_R_SetColor( color );
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 371
;371:		memcpy(&newColor[0], &color[0], sizeof(vec4_t));
ADDRLP4 4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 372
;372:    len = strlen(text);
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 32
ADDRLP4 40
INDIRI4
ASGNI4
line 373
;373:		if (limit > 0 && len > limit) {
ADDRLP4 44
ADDRFP4 24
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
LEI4 $304
ADDRLP4 32
INDIRI4
ADDRLP4 44
INDIRI4
LEI4 $304
line 374
;374:			len = limit;
ADDRLP4 32
ADDRFP4 24
INDIRI4
ASGNI4
line 375
;375:		}
LABELV $304
line 376
;376:		count = 0;
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRGP4 $307
JUMPV
LABELV $306
line 377
;377:		while (s && *s && count < len) {
line 378
;378:			glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
ADDRLP4 0
CNSTI4 80
ADDRLP4 36
INDIRP4
INDIRI1
CVII4 1
MULI4
ADDRLP4 28
INDIRP4
ADDP4
ASGNP4
line 381
;379:      //int yadj = Assets.textFont.glyphs[text[i]].bottom + Assets.textFont.glyphs[text[i]].top;
;380:      //float yadj = scale * (Assets.textFont.glyphs[text[i]].imageHeight - Assets.textFont.glyphs[text[i]].height);
;381:			if ( Q_IsColorString( s ) ) {
ADDRLP4 36
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $309
ADDRLP4 52
CNSTI4 94
ASGNI4
ADDRLP4 36
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 52
INDIRI4
NEI4 $309
ADDRLP4 56
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $309
ADDRLP4 56
INDIRI4
ADDRLP4 52
INDIRI4
EQI4 $309
line 382
;382:				memcpy( newColor, g_color_table[ColorIndex(*(s+1))], sizeof( newColor ) );
ADDRLP4 4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 4
LSHI4
ADDRGP4 g_color_table
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 383
;383:				newColor[3] = color[3];
ADDRLP4 4+12
ADDRFP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 384
;384:				trap_R_SetColor( newColor );
ADDRLP4 4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 385
;385:				s += 2;
ADDRLP4 36
ADDRLP4 36
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 386
;386:				continue;
ADDRGP4 $307
JUMPV
LABELV $309
line 387
;387:			} else {
line 388
;388:				float yadj = useScale * glyph->top;
ADDRLP4 60
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 389
;389:				if (style == ITEM_TEXTSTYLE_SHADOWED || style == ITEM_TEXTSTYLE_SHADOWEDMORE) {
ADDRLP4 64
ADDRFP4 28
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 3
EQI4 $314
ADDRLP4 64
INDIRI4
CNSTI4 6
NEI4 $312
LABELV $314
line 390
;390:					int ofs = style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;
ADDRFP4 28
INDIRI4
CNSTI4 3
NEI4 $316
ADDRLP4 72
CNSTI4 1
ASGNI4
ADDRGP4 $317
JUMPV
LABELV $316
ADDRLP4 72
CNSTI4 2
ASGNI4
LABELV $317
ADDRLP4 68
ADDRLP4 72
INDIRI4
ASGNI4
line 391
;391:					colorBlack[3] = newColor[3];
ADDRGP4 colorBlack+12
ADDRLP4 4+12
INDIRF4
ASGNF4
line 392
;392:					trap_R_SetColor( colorBlack );
ADDRGP4 colorBlack
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 393
;393:					Text_PaintChar(x + ofs, y - yadj + ofs, 
ADDRLP4 76
ADDRLP4 68
INDIRI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRF4
ADDRLP4 76
INDIRF4
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 60
INDIRF4
SUBF4
ADDRLP4 76
INDIRF4
ADDF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
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
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 402
;394:														glyph->imageWidth,
;395:														glyph->imageHeight,
;396:														useScale, 
;397:														glyph->s,
;398:														glyph->t,
;399:														glyph->s2,
;400:														glyph->t2,
;401:														glyph->glyph);
;402:					trap_R_SetColor( newColor );
ADDRLP4 4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 403
;403:					colorBlack[3] = 1.0;
ADDRGP4 colorBlack+12
CNSTF4 1065353216
ASGNF4
line 404
;404:				}
LABELV $312
line 405
;405:				Text_PaintChar(x, y - yadj, 
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 60
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
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
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 415
;406:													glyph->imageWidth,
;407:													glyph->imageHeight,
;408:													useScale, 
;409:													glyph->s,
;410:													glyph->t,
;411:													glyph->s2,
;412:													glyph->t2,
;413:													glyph->glyph);
;414:
;415:				x += (glyph->xSkip * useScale) + adjust;
ADDRFP4 0
ADDRFP4 0
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 20
INDIRF4
MULF4
ADDRFP4 20
INDIRF4
ADDF4
ADDF4
ASGNF4
line 416
;416:				s++;
ADDRLP4 36
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 417
;417:				count++;
ADDRLP4 24
ADDRLP4 24
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 418
;418:			}
line 419
;419:    }
LABELV $307
line 377
ADDRLP4 36
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $322
ADDRLP4 36
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $322
ADDRLP4 24
INDIRI4
ADDRLP4 32
INDIRI4
LTI4 $306
LABELV $322
line 420
;420:	  trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 421
;421:  }
LABELV $302
line 422
;422:}
LABELV $289
endproc Text_Paint 84 40
export Text_PaintWithCursor
proc Text_PaintWithCursor 88 40
ADDRFP4 24
ADDRFP4 24
INDIRI4
CVII1 4
ASGNI1
line 424
;423:
;424:void Text_PaintWithCursor(float x, float y, float scale, vec4_t color, const char *text, int cursorPos, char cursor, int limit, int style) {
line 430
;425:  int len, count;
;426:	vec4_t newColor;
;427:	glyphInfo_t *glyph, *glyph2;
;428:	float yadj;
;429:	float useScale;
;430:	fontInfo_t *font = &uiInfo.uiDC.Assets.textFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+12
ASGNP4
line 431
;431:	if (scale <= ui_smallFont.value) {
ADDRFP4 8
INDIRF4
ADDRGP4 ui_smallFont+8
INDIRF4
GTF4 $326
line 432
;432:		font = &uiInfo.uiDC.Assets.smallFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+20560
ASGNP4
line 433
;433:	} else if (scale >= ui_bigFont.value) {
ADDRGP4 $327
JUMPV
LABELV $326
ADDRFP4 8
INDIRF4
ADDRGP4 ui_bigFont+8
INDIRF4
LTF4 $331
line 434
;434:		font = &uiInfo.uiDC.Assets.bigFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+41108
ASGNP4
line 435
;435:	}
LABELV $331
LABELV $327
line 436
;436:	useScale = scale * font->glyphScale;
ADDRLP4 4
ADDRFP4 8
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 20480
ADDP4
INDIRF4
MULF4
ASGNF4
line 437
;437:  if (text) {
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $336
line 438
;438:    const char *s = text; // bk001206 - unsigned
ADDRLP4 44
ADDRFP4 16
INDIRP4
ASGNP4
line 439
;439:		trap_R_SetColor( color );
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 440
;440:		memcpy(&newColor[0], &color[0], sizeof(vec4_t));
ADDRLP4 12
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 441
;441:    len = strlen(text);
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 40
ADDRLP4 48
INDIRI4
ASGNI4
line 442
;442:		if (limit > 0 && len > limit) {
ADDRLP4 52
ADDRFP4 28
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
LEI4 $338
ADDRLP4 40
INDIRI4
ADDRLP4 52
INDIRI4
LEI4 $338
line 443
;443:			len = limit;
ADDRLP4 40
ADDRFP4 28
INDIRI4
ASGNI4
line 444
;444:		}
LABELV $338
line 445
;445:		count = 0;
ADDRLP4 28
CNSTI4 0
ASGNI4
line 446
;446:		glyph2 = &font->glyphs[ (int) cursor]; // bk001206 - possible signed char
ADDRLP4 8
CNSTI4 80
ADDRFP4 24
INDIRI1
CVII4 1
MULI4
ADDRLP4 36
INDIRP4
ADDP4
ASGNP4
ADDRGP4 $341
JUMPV
LABELV $340
line 447
;447:		while (s && *s && count < len) {
line 448
;448:			glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
ADDRLP4 0
CNSTI4 80
ADDRLP4 44
INDIRP4
INDIRI1
CVII4 1
MULI4
ADDRLP4 36
INDIRP4
ADDP4
ASGNP4
line 451
;449:      //int yadj = Assets.textFont.glyphs[text[i]].bottom + Assets.textFont.glyphs[text[i]].top;
;450:      //float yadj = scale * (Assets.textFont.glyphs[text[i]].imageHeight - Assets.textFont.glyphs[text[i]].height);
;451:			if ( Q_IsColorString( s ) ) {
ADDRLP4 44
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $343
ADDRLP4 60
CNSTI4 94
ASGNI4
ADDRLP4 44
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 60
INDIRI4
NEI4 $343
ADDRLP4 64
ADDRLP4 44
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
EQI4 $343
ADDRLP4 64
INDIRI4
ADDRLP4 60
INDIRI4
EQI4 $343
line 452
;452:				memcpy( newColor, g_color_table[ColorIndex(*(s+1))], sizeof( newColor ) );
ADDRLP4 12
ARGP4
ADDRLP4 44
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 4
LSHI4
ADDRGP4 g_color_table
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 453
;453:				newColor[3] = color[3];
ADDRLP4 12+12
ADDRFP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 454
;454:				trap_R_SetColor( newColor );
ADDRLP4 12
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 455
;455:				s += 2;
ADDRLP4 44
ADDRLP4 44
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 456
;456:				continue;
ADDRGP4 $341
JUMPV
LABELV $343
line 457
;457:			} else {
line 458
;458:				yadj = useScale * glyph->top;
ADDRLP4 32
ADDRLP4 4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 459
;459:				if (style == ITEM_TEXTSTYLE_SHADOWED || style == ITEM_TEXTSTYLE_SHADOWEDMORE) {
ADDRLP4 68
ADDRFP4 32
INDIRI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 3
EQI4 $348
ADDRLP4 68
INDIRI4
CNSTI4 6
NEI4 $346
LABELV $348
line 460
;460:					int ofs = style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;
ADDRFP4 32
INDIRI4
CNSTI4 3
NEI4 $350
ADDRLP4 76
CNSTI4 1
ASGNI4
ADDRGP4 $351
JUMPV
LABELV $350
ADDRLP4 76
CNSTI4 2
ASGNI4
LABELV $351
ADDRLP4 72
ADDRLP4 76
INDIRI4
ASGNI4
line 461
;461:					colorBlack[3] = newColor[3];
ADDRGP4 colorBlack+12
ADDRLP4 12+12
INDIRF4
ASGNF4
line 462
;462:					trap_R_SetColor( colorBlack );
ADDRGP4 colorBlack
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 463
;463:					Text_PaintChar(x + ofs, y - yadj + ofs, 
ADDRLP4 80
ADDRLP4 72
INDIRI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRF4
ADDRLP4 80
INDIRF4
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
ADDRLP4 80
INDIRF4
ADDF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
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
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 472
;464:														glyph->imageWidth,
;465:														glyph->imageHeight,
;466:														useScale, 
;467:														glyph->s,
;468:														glyph->t,
;469:														glyph->s2,
;470:														glyph->t2,
;471:														glyph->glyph);
;472:					colorBlack[3] = 1.0;
ADDRGP4 colorBlack+12
CNSTF4 1065353216
ASGNF4
line 473
;473:					trap_R_SetColor( newColor );
ADDRLP4 12
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 474
;474:				}
LABELV $346
line 475
;475:				Text_PaintChar(x, y - yadj, 
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
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
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 485
;476:													glyph->imageWidth,
;477:													glyph->imageHeight,
;478:													useScale, 
;479:													glyph->s,
;480:													glyph->t,
;481:													glyph->s2,
;482:													glyph->t2,
;483:													glyph->glyph);
;484:
;485:	      yadj = useScale * glyph2->top;
ADDRLP4 32
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 486
;486:		    if (count == cursorPos && !((uiInfo.uiDC.realTime/BLINK_DIVISOR) & 1)) {
ADDRLP4 28
INDIRI4
ADDRFP4 20
INDIRI4
NEI4 $355
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 200
DIVI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $355
line 487
;487:					Text_PaintChar(x, y - yadj, 
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 496
;488:														glyph2->imageWidth,
;489:														glyph2->imageHeight,
;490:														useScale, 
;491:														glyph2->s,
;492:														glyph2->t,
;493:														glyph2->s2,
;494:														glyph2->t2,
;495:														glyph2->glyph);
;496:				}
LABELV $355
line 498
;497:
;498:				x += (glyph->xSkip * useScale);
ADDRFP4 0
ADDRFP4 0
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
line 499
;499:				s++;
ADDRLP4 44
ADDRLP4 44
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 500
;500:				count++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 501
;501:			}
line 502
;502:    }
LABELV $341
line 447
ADDRLP4 44
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $359
ADDRLP4 44
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $359
ADDRLP4 28
INDIRI4
ADDRLP4 40
INDIRI4
LTI4 $340
LABELV $359
line 504
;503:    // need to paint cursor at end of text
;504:    if (cursorPos == len && !((uiInfo.uiDC.realTime/BLINK_DIVISOR) & 1)) {
ADDRFP4 20
INDIRI4
ADDRLP4 40
INDIRI4
NEI4 $360
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 200
DIVI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $360
line 505
;505:        yadj = useScale * glyph2->top;
ADDRLP4 32
ADDRLP4 4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 506
;506:        Text_PaintChar(x, y - yadj, 
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 516
;507:                          glyph2->imageWidth,
;508:                          glyph2->imageHeight,
;509:                          useScale, 
;510:                          glyph2->s,
;511:                          glyph2->t,
;512:                          glyph2->s2,
;513:                          glyph2->t2,
;514:                          glyph2->glyph);
;515:
;516:    }
LABELV $360
line 518
;517:
;518:	  trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 519
;519:  }
LABELV $336
line 520
;520:}
LABELV $323
endproc Text_PaintWithCursor 88 40
proc Text_Paint_Limit 76 40
line 523
;521:
;522:
;523:static void Text_Paint_Limit(float *maxX, float x, float y, float scale, vec4_t color, const char* text, float adjust, int limit) {
line 527
;524:  int len, count;
;525:	vec4_t newColor;
;526:	glyphInfo_t *glyph;
;527:  if (text) {
ADDRFP4 20
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $364
line 528
;528:    const char *s = text; // bk001206 - unsigned
ADDRLP4 28
ADDRFP4 20
INDIRP4
ASGNP4
line 529
;529:		float max = *maxX;
ADDRLP4 40
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 531
;530:		float useScale;
;531:		fontInfo_t *font = &uiInfo.uiDC.Assets.textFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+12
ASGNP4
line 532
;532:		if (scale <= ui_smallFont.value) {
ADDRFP4 12
INDIRF4
ADDRGP4 ui_smallFont+8
INDIRF4
GTF4 $368
line 533
;533:			font = &uiInfo.uiDC.Assets.smallFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+20560
ASGNP4
line 534
;534:		} else if (scale > ui_bigFont.value) {
ADDRGP4 $369
JUMPV
LABELV $368
ADDRFP4 12
INDIRF4
ADDRGP4 ui_bigFont+8
INDIRF4
LEF4 $373
line 535
;535:			font = &uiInfo.uiDC.Assets.bigFont;
ADDRLP4 36
ADDRGP4 uiInfo+228+41108
ASGNP4
line 536
;536:		}
LABELV $373
LABELV $369
line 537
;537:		useScale = scale * font->glyphScale;
ADDRLP4 32
ADDRFP4 12
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 20480
ADDP4
INDIRF4
MULF4
ASGNF4
line 538
;538:		trap_R_SetColor( color );
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 539
;539:    len = strlen(text);					 
ADDRFP4 20
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 44
INDIRI4
ASGNI4
line 540
;540:		if (limit > 0 && len > limit) {
ADDRLP4 48
ADDRFP4 28
INDIRI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
LEI4 $378
ADDRLP4 24
INDIRI4
ADDRLP4 48
INDIRI4
LEI4 $378
line 541
;541:			len = limit;
ADDRLP4 24
ADDRFP4 28
INDIRI4
ASGNI4
line 542
;542:		}
LABELV $378
line 543
;543:		count = 0;
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRGP4 $381
JUMPV
LABELV $380
line 544
;544:		while (s && *s && count < len) {
line 545
;545:			glyph = &font->glyphs[(int)*s]; // TTimo: FIXME: getting nasty warnings without the cast, hopefully this doesn't break the VM build
ADDRLP4 0
CNSTI4 80
ADDRLP4 28
INDIRP4
INDIRI1
CVII4 1
MULI4
ADDRLP4 36
INDIRP4
ADDP4
ASGNP4
line 546
;546:			if ( Q_IsColorString( s ) ) {
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $383
ADDRLP4 56
CNSTI4 94
ASGNI4
ADDRLP4 28
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 56
INDIRI4
NEI4 $383
ADDRLP4 60
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $383
ADDRLP4 60
INDIRI4
ADDRLP4 56
INDIRI4
EQI4 $383
line 547
;547:				memcpy( newColor, g_color_table[ColorIndex(*(s+1))], sizeof( newColor ) );
ADDRLP4 4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 4
LSHI4
ADDRGP4 g_color_table
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 548
;548:				newColor[3] = color[3];
ADDRLP4 4+12
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 549
;549:				trap_R_SetColor( newColor );
ADDRLP4 4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 550
;550:				s += 2;
ADDRLP4 28
ADDRLP4 28
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 551
;551:				continue;
ADDRGP4 $381
JUMPV
LABELV $383
line 552
;552:			} else {
line 553
;553:	      float yadj = useScale * glyph->top;
ADDRLP4 64
ADDRLP4 32
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 554
;554:				if (Text_Width(s, useScale, 1) + x > max) {
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
INDIRF4
ARGF4
CNSTI4 1
ARGI4
ADDRLP4 68
ADDRGP4 Text_Width
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CVIF4 4
ADDRFP4 4
INDIRF4
ADDF4
ADDRLP4 40
INDIRF4
LEF4 $386
line 555
;555:					*maxX = 0;
ADDRFP4 0
INDIRP4
CNSTF4 0
ASGNF4
line 556
;556:					break;
ADDRGP4 $382
JUMPV
LABELV $386
line 558
;557:				}
;558:		    Text_PaintChar(x, y - yadj, 
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ADDRLP4 64
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ARGF4
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
INDIRI4
ARGI4
ADDRGP4 Text_PaintChar
CALLV
pop
line 567
;559:			                 glyph->imageWidth,
;560:				               glyph->imageHeight,
;561:				               useScale, 
;562:						           glyph->s,
;563:								       glyph->t,
;564:								       glyph->s2,
;565:									     glyph->t2,
;566:										   glyph->glyph);
;567:	      x += (glyph->xSkip * useScale) + adjust;
ADDRFP4 4
ADDRFP4 4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 32
INDIRF4
MULF4
ADDRFP4 24
INDIRF4
ADDF4
ADDF4
ASGNF4
line 568
;568:				*maxX = x;
ADDRFP4 0
INDIRP4
ADDRFP4 4
INDIRF4
ASGNF4
line 569
;569:				count++;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 570
;570:				s++;
ADDRLP4 28
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 571
;571:	    }
line 572
;572:		}
LABELV $381
line 544
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $389
ADDRLP4 28
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $389
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
LTI4 $380
LABELV $389
LABELV $382
line 573
;573:	  trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 574
;574:  }
LABELV $364
line 576
;575:
;576:}
LABELV $363
endproc Text_Paint_Limit 76 40
export UI_ShowPostGame
proc UI_ShowPostGame 0 8
line 579
;577:
;578:
;579:void UI_ShowPostGame(qboolean newHigh) {
line 580
;580:	trap_Cvar_Set ("cg_cameraOrbit", "0");
ADDRGP4 $391
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 581
;581:	trap_Cvar_Set("cg_thirdPerson", "0");
ADDRGP4 $393
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 582
;582:	trap_Cvar_Set( "sv_killserver", "1" );
ADDRGP4 $394
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 583
;583:	uiInfo.soundHighScore = newHigh;
ADDRGP4 uiInfo+73436
ADDRFP4 0
INDIRI4
ASGNI4
line 584
;584:  _UI_SetActiveMenu(UIMENU_POSTGAME);
CNSTI4 6
ARGI4
ADDRGP4 _UI_SetActiveMenu
CALLV
pop
line 585
;585:}
LABELV $390
endproc UI_ShowPostGame 0 8
export UI_DrawCenteredPic
proc UI_DrawCenteredPic 8 20
line 592
;586:/*
;587:=================
;588:_UI_Refresh
;589:=================
;590:*/
;591:
;592:void UI_DrawCenteredPic(qhandle_t image, int w, int h) {
line 594
;593:  int x, y;
;594:  x = (SCREEN_WIDTH - w) / 2;
ADDRLP4 0
CNSTI4 640
ADDRFP4 4
INDIRI4
SUBI4
CNSTI4 2
DIVI4
ASGNI4
line 595
;595:  y = (SCREEN_HEIGHT - h) / 2;
ADDRLP4 4
CNSTI4 480
ADDRFP4 8
INDIRI4
SUBI4
CNSTI4 2
DIVI4
ASGNI4
line 596
;596:  UI_DrawHandlePic(x, y, w, h, image);
ADDRLP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 597
;597:}
LABELV $397
endproc UI_DrawCenteredPic 8 20
data
export frameCount
align 4
LABELV frameCount
byte 4 0
bss
align 4
LABELV $399
skip 4
align 4
LABELV $400
skip 16
export _UI_Refresh
code
proc _UI_Refresh 20 20
line 604
;598:
;599:int frameCount = 0;
;600:int startTime;
;601:
;602:#define	UI_FPS_FRAMES	4
;603:void _UI_Refresh( int realtime )
;604:{
line 612
;605:	static int index;
;606:	static int	previousTimes[UI_FPS_FRAMES];
;607:
;608:	//if ( !( trap_Key_GetCatcher() & KEYCATCH_UI ) ) {
;609:	//	return;
;610:	//}
;611:
;612:	uiInfo.uiDC.frameTime = realtime - uiInfo.uiDC.realTime;
ADDRGP4 uiInfo+212
ADDRFP4 0
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
SUBI4
ASGNI4
line 613
;613:	uiInfo.uiDC.realTime = realtime;
ADDRGP4 uiInfo+208
ADDRFP4 0
INDIRI4
ASGNI4
line 615
;614:
;615:	previousTimes[index % UI_FPS_FRAMES] = uiInfo.uiDC.frameTime;
ADDRGP4 $399
INDIRI4
CNSTI4 4
MODI4
CNSTI4 2
LSHI4
ADDRGP4 $400
ADDP4
ADDRGP4 uiInfo+212
INDIRI4
ASGNI4
line 616
;616:	index++;
ADDRLP4 0
ADDRGP4 $399
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 617
;617:	if ( index > UI_FPS_FRAMES ) {
ADDRGP4 $399
INDIRI4
CNSTI4 4
LEI4 $405
line 620
;618:		int i, total;
;619:		// average multiple frames together to smooth changes out a bit
;620:		total = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 621
;621:		for ( i = 0 ; i < UI_FPS_FRAMES ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $407
line 622
;622:			total += previousTimes[i];
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $400
ADDP4
INDIRI4
ADDI4
ASGNI4
line 623
;623:		}
LABELV $408
line 621
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 4
LTI4 $407
line 624
;624:		if ( !total ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $411
line 625
;625:			total = 1;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 626
;626:		}
LABELV $411
line 627
;627:		uiInfo.uiDC.FPS = 1000 * UI_FPS_FRAMES / total;
ADDRGP4 uiInfo+73412
CNSTI4 4000
ADDRLP4 8
INDIRI4
DIVI4
CVIF4 4
ASGNF4
line 628
;628:	}
LABELV $405
line 632
;629:
;630:
;631:
;632:	UI_UpdateCvars();
ADDRGP4 UI_UpdateCvars
CALLV
pop
line 634
;633:
;634:	if (Menu_Count() > 0) {
ADDRLP4 4
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LEI4 $414
line 636
;635:		// paint all the menus
;636:		Menu_PaintAll();
ADDRGP4 Menu_PaintAll
CALLV
pop
line 638
;637:		// refresh server browser list
;638:		UI_DoServerRefresh();
ADDRGP4 UI_DoServerRefresh
CALLV
pop
line 640
;639:		// refresh server status
;640:		UI_BuildServerStatus(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_BuildServerStatus
CALLV
pop
line 642
;641:		// refresh find player list
;642:		UI_BuildFindPlayerList(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_BuildFindPlayerList
CALLV
pop
line 643
;643:	} 
LABELV $414
line 646
;644:	
;645:	// draw cursor
;646:	UI_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 UI_SetColor
CALLV
pop
line 647
;647:	if (Menu_Count() > 0) {
ADDRLP4 8
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LEI4 $416
line 648
;648:		UI_DrawHandlePic( uiInfo.uiDC.cursorx-16, uiInfo.uiDC.cursory-16, 32, 32, uiInfo.uiDC.Assets.cursor);
ADDRLP4 12
CNSTI4 16
ASGNI4
ADDRGP4 uiInfo+216
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CVIF4 4
ARGF4
ADDRGP4 uiInfo+220
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CVIF4 4
ARGF4
ADDRLP4 16
CNSTF4 1107296256
ASGNF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uiInfo+228+61656
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 649
;649:	}
LABELV $416
line 652
;650:
;651:#ifndef NDEBUG
;652:	if (uiInfo.uiDC.debug)
ADDRGP4 uiInfo+224
INDIRI4
CNSTI4 0
EQI4 $422
line 653
;653:	{
line 657
;654:		// cursor coordinates
;655:		//FIXME
;656:		//UI_DrawString( 0, 0, va("(%d,%d)",uis.cursorx,uis.cursory), UI_LEFT|UI_SMALLFONT, colorRed );
;657:	}
LABELV $422
line 660
;658:#endif
;659:
;660:}
LABELV $398
endproc _UI_Refresh 20 20
export _UI_Shutdown
proc _UI_Shutdown 0 0
line 667
;661:
;662:/*
;663:=================
;664:_UI_Shutdown
;665:=================
;666:*/
;667:void _UI_Shutdown( void ) {
line 668
;668:	trap_LAN_SaveCachedServers();
ADDRGP4 trap_LAN_SaveCachedServers
CALLV
pop
line 669
;669:}
LABELV $425
endproc _UI_Shutdown 0 0
data
export defaultMenu
align 4
LABELV defaultMenu
byte 4 0
bss
align 1
LABELV $427
skip 32768
export GetMenuBuffer
code
proc GetMenuBuffer 16 16
line 673
;670:
;671:char *defaultMenu = NULL;
;672:
;673:char *GetMenuBuffer(const char *filename) {
line 678
;674:	int	len;
;675:	fileHandle_t	f;
;676:	static char buf[MAX_MENUFILE];
;677:
;678:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 679
;679:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $428
line 680
;680:		trap_Print( va( S_COLOR_RED "menu file not found: %s, using default\n", filename ) );
ADDRGP4 $430
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 681
;681:		return defaultMenu;
ADDRGP4 defaultMenu
INDIRP4
RETP4
ADDRGP4 $426
JUMPV
LABELV $428
line 683
;682:	}
;683:	if ( len >= MAX_MENUFILE ) {
ADDRLP4 0
INDIRI4
CNSTI4 32768
LTI4 $431
line 684
;684:		trap_Print( va( S_COLOR_RED "menu file too large: %s is %i, max allowed is %i", filename, len, MAX_MENUFILE ) );
ADDRGP4 $433
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 32768
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 685
;685:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 686
;686:		return defaultMenu;
ADDRGP4 defaultMenu
INDIRP4
RETP4
ADDRGP4 $426
JUMPV
LABELV $431
line 689
;687:	}
;688:
;689:	trap_FS_Read( buf, len, f );
ADDRGP4 $427
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 690
;690:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRGP4 $427
ADDP4
CNSTI1 0
ASGNI1
line 691
;691:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 693
;692:	//COM_Compress(buf);
;693:  return buf;
ADDRGP4 $427
RETP4
LABELV $426
endproc GetMenuBuffer 16 16
export Asset_Parse
proc Asset_Parse 1124 12
line 697
;694:
;695:}
;696:
;697:qboolean Asset_Parse(int handle) {
line 701
;698:	pc_token_t token;
;699:	const char *tempStr;
;700:
;701:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $435
line 702
;702:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $435
line 703
;703:	if (Q_stricmp(token.string, "{") != 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $440
ARGP4
ADDRLP4 1048
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
EQI4 $442
line 704
;704:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $441
line 707
;705:	}
;706:    
;707:	while ( 1 ) {
line 709
;708:
;709:		memset(&token, 0, sizeof(pc_token_t));
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1040
ARGI4
ADDRGP4 memset
CALLP4
pop
line 711
;710:
;711:		if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1052
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $444
line 712
;712:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $444
line 714
;713:
;714:		if (Q_stricmp(token.string, "}") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $446
line 715
;715:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $434
JUMPV
LABELV $446
line 719
;716:		}
;717:
;718:		// font
;719:		if (Q_stricmp(token.string, "font") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $453
ARGP4
ADDRLP4 1060
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $450
line 721
;720:			int pointSize;
;721:			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle,&pointSize)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1068
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1068
INDIRI4
CNSTI4 0
EQI4 $456
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1064
ARGP4
ADDRLP4 1072
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $454
LABELV $456
line 722
;722:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $454
line 724
;723:			}
;724:			trap_R_RegisterFont(tempStr, pointSize, &uiInfo.uiDC.Assets.textFont);
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1064
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+12
ARGP4
ADDRGP4 trap_R_RegisterFont
CALLV
pop
line 725
;725:			uiInfo.uiDC.Assets.fontRegistered = qtrue;
ADDRGP4 uiInfo+228+61764
CNSTI4 1
ASGNI4
line 726
;726:			continue;
ADDRGP4 $442
JUMPV
LABELV $450
line 729
;727:		}
;728:
;729:		if (Q_stricmp(token.string, "smallFont") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $464
ARGP4
ADDRLP4 1064
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $461
line 731
;730:			int pointSize;
;731:			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle,&pointSize)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1072
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
EQI4 $467
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1068
ARGP4
ADDRLP4 1076
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
NEI4 $465
LABELV $467
line 732
;732:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $465
line 734
;733:			}
;734:			trap_R_RegisterFont(tempStr, pointSize, &uiInfo.uiDC.Assets.smallFont);
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1068
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+20560
ARGP4
ADDRGP4 trap_R_RegisterFont
CALLV
pop
line 735
;735:			continue;
ADDRGP4 $442
JUMPV
LABELV $461
line 738
;736:		}
;737:
;738:		if (Q_stricmp(token.string, "bigFont") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $473
ARGP4
ADDRLP4 1068
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1068
INDIRI4
CNSTI4 0
NEI4 $470
line 740
;739:			int pointSize;
;740:			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle,&pointSize)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1076
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
EQI4 $476
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1072
ARGP4
ADDRLP4 1080
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 1080
INDIRI4
CNSTI4 0
NEI4 $474
LABELV $476
line 741
;741:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $474
line 743
;742:			}
;743:			trap_R_RegisterFont(tempStr, pointSize, &uiInfo.uiDC.Assets.bigFont);
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1072
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+41108
ARGP4
ADDRGP4 trap_R_RegisterFont
CALLV
pop
line 744
;744:			continue;
ADDRGP4 $442
JUMPV
LABELV $470
line 749
;745:		}
;746:
;747:
;748:		// gradientbar
;749:		if (Q_stricmp(token.string, "gradientbar") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $482
ARGP4
ADDRLP4 1072
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $479
line 750
;750:			if (!PC_String_Parse(handle, &tempStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1076
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
NEI4 $483
line 751
;751:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $483
line 753
;752:			}
;753:			uiInfo.uiDC.Assets.gradientBar = trap_R_RegisterShaderNoMip(tempStr);
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1080
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61660
ADDRLP4 1080
INDIRI4
ASGNI4
line 754
;754:			continue;
ADDRGP4 $442
JUMPV
LABELV $479
line 758
;755:		}
;756:
;757:		// enterMenuSound
;758:		if (Q_stricmp(token.string, "menuEnterSound") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $490
ARGP4
ADDRLP4 1076
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
NEI4 $487
line 759
;759:			if (!PC_String_Parse(handle, &tempStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1080
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1080
INDIRI4
CNSTI4 0
NEI4 $491
line 760
;760:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $491
line 762
;761:			}
;762:			uiInfo.uiDC.Assets.menuEnterSound = trap_S_RegisterSound( tempStr, qfalse );
ADDRLP4 1040
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1084
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61708
ADDRLP4 1084
INDIRI4
ASGNI4
line 763
;763:			continue;
ADDRGP4 $442
JUMPV
LABELV $487
line 767
;764:		}
;765:
;766:		// exitMenuSound
;767:		if (Q_stricmp(token.string, "menuExitSound") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $498
ARGP4
ADDRLP4 1080
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1080
INDIRI4
CNSTI4 0
NEI4 $495
line 768
;768:			if (!PC_String_Parse(handle, &tempStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1084
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1084
INDIRI4
CNSTI4 0
NEI4 $499
line 769
;769:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $499
line 771
;770:			}
;771:			uiInfo.uiDC.Assets.menuExitSound = trap_S_RegisterSound( tempStr, qfalse );
ADDRLP4 1040
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1088
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61712
ADDRLP4 1088
INDIRI4
ASGNI4
line 772
;772:			continue;
ADDRGP4 $442
JUMPV
LABELV $495
line 776
;773:		}
;774:
;775:		// itemFocusSound
;776:		if (Q_stricmp(token.string, "itemFocusSound") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $506
ARGP4
ADDRLP4 1084
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1084
INDIRI4
CNSTI4 0
NEI4 $503
line 777
;777:			if (!PC_String_Parse(handle, &tempStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1088
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1088
INDIRI4
CNSTI4 0
NEI4 $507
line 778
;778:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $507
line 780
;779:			}
;780:			uiInfo.uiDC.Assets.itemFocusSound = trap_S_RegisterSound( tempStr, qfalse );
ADDRLP4 1040
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1092
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61720
ADDRLP4 1092
INDIRI4
ASGNI4
line 781
;781:			continue;
ADDRGP4 $442
JUMPV
LABELV $503
line 785
;782:		}
;783:
;784:		// menuBuzzSound
;785:		if (Q_stricmp(token.string, "menuBuzzSound") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $514
ARGP4
ADDRLP4 1088
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1088
INDIRI4
CNSTI4 0
NEI4 $511
line 786
;786:			if (!PC_String_Parse(handle, &tempStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1040
ARGP4
ADDRLP4 1092
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1092
INDIRI4
CNSTI4 0
NEI4 $515
line 787
;787:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $515
line 789
;788:			}
;789:			uiInfo.uiDC.Assets.menuBuzzSound = trap_S_RegisterSound( tempStr, qfalse );
ADDRLP4 1040
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1096
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61716
ADDRLP4 1096
INDIRI4
ASGNI4
line 790
;790:			continue;
ADDRGP4 $442
JUMPV
LABELV $511
line 793
;791:		}
;792:
;793:		if (Q_stricmp(token.string, "cursor") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $522
ARGP4
ADDRLP4 1092
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1092
INDIRI4
CNSTI4 0
NEI4 $519
line 794
;794:			if (!PC_String_Parse(handle, &uiInfo.uiDC.Assets.cursorStr)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+4
ARGP4
ADDRLP4 1096
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 0
NEI4 $523
line 795
;795:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $523
line 797
;796:			}
;797:			uiInfo.uiDC.Assets.cursor = trap_R_RegisterShaderNoMip( uiInfo.uiDC.Assets.cursorStr);
ADDRGP4 uiInfo+228+4
INDIRP4
ARGP4
ADDRLP4 1100
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+228+61656
ADDRLP4 1100
INDIRI4
ASGNI4
line 798
;798:			continue;
ADDRGP4 $442
JUMPV
LABELV $519
line 801
;799:		}
;800:
;801:		if (Q_stricmp(token.string, "fadeClamp") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $534
ARGP4
ADDRLP4 1096
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 0
NEI4 $531
line 802
;802:			if (!PC_Float_Parse(handle, &uiInfo.uiDC.Assets.fadeClamp)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61724
ARGP4
ADDRLP4 1100
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 1100
INDIRI4
CNSTI4 0
NEI4 $442
line 803
;803:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
line 805
;804:			}
;805:			continue;
LABELV $531
line 808
;806:		}
;807:
;808:		if (Q_stricmp(token.string, "fadeCycle") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $542
ARGP4
ADDRLP4 1100
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1100
INDIRI4
CNSTI4 0
NEI4 $539
line 809
;809:			if (!PC_Int_Parse(handle, &uiInfo.uiDC.Assets.fadeCycle)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61728
ARGP4
ADDRLP4 1104
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
NEI4 $442
line 810
;810:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
line 812
;811:			}
;812:			continue;
LABELV $539
line 815
;813:		}
;814:
;815:		if (Q_stricmp(token.string, "fadeAmount") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $550
ARGP4
ADDRLP4 1104
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
NEI4 $547
line 816
;816:			if (!PC_Float_Parse(handle, &uiInfo.uiDC.Assets.fadeAmount)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61732
ARGP4
ADDRLP4 1108
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $442
line 817
;817:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
line 819
;818:			}
;819:			continue;
LABELV $547
line 822
;820:		}
;821:
;822:		if (Q_stricmp(token.string, "shadowX") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $558
ARGP4
ADDRLP4 1108
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $555
line 823
;823:			if (!PC_Float_Parse(handle, &uiInfo.uiDC.Assets.shadowX)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61736
ARGP4
ADDRLP4 1112
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
NEI4 $442
line 824
;824:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
line 826
;825:			}
;826:			continue;
LABELV $555
line 829
;827:		}
;828:
;829:		if (Q_stricmp(token.string, "shadowY") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $566
ARGP4
ADDRLP4 1112
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
NEI4 $563
line 830
;830:			if (!PC_Float_Parse(handle, &uiInfo.uiDC.Assets.shadowY)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61740
ARGP4
ADDRLP4 1116
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 0
NEI4 $442
line 831
;831:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
line 833
;832:			}
;833:			continue;
LABELV $563
line 836
;834:		}
;835:
;836:		if (Q_stricmp(token.string, "shadowColor") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $574
ARGP4
ADDRLP4 1116
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 0
NEI4 $571
line 837
;837:			if (!PC_Color_Parse(handle, &uiInfo.uiDC.Assets.shadowColor)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 uiInfo+228+61744
ARGP4
ADDRLP4 1120
ADDRGP4 PC_Color_Parse
CALLI4
ASGNI4
ADDRLP4 1120
INDIRI4
CNSTI4 0
NEI4 $575
line 838
;838:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $575
line 840
;839:			}
;840:			uiInfo.uiDC.Assets.shadowFadeClamp = uiInfo.uiDC.Assets.shadowColor[3];
ADDRGP4 uiInfo+228+61760
ADDRGP4 uiInfo+228+61744+12
INDIRF4
ASGNF4
line 841
;841:			continue;
LABELV $571
line 844
;842:		}
;843:
;844:	}
LABELV $442
line 707
ADDRGP4 $441
JUMPV
line 845
;845:	return qfalse;
CNSTI4 0
RETI4
LABELV $434
endproc Asset_Parse 1124 12
export Font_Report
proc Font_Report 8 12
line 848
;846:}
;847:
;848:void Font_Report() {
line 850
;849:  int i;
;850:  Com_Printf("Font Info\n");
ADDRGP4 $585
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 851
;851:  Com_Printf("=========\n");
ADDRGP4 $586
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 852
;852:  for ( i = 32; i < 96; i++) {
ADDRLP4 0
CNSTI4 32
ASGNI4
LABELV $587
line 853
;853:    Com_Printf("Glyph handle %i: %i\n", i, uiInfo.uiDC.Assets.textFont.glyphs[i].glyph);
ADDRGP4 $591
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 80
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+228+12+44
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 854
;854:  }
LABELV $588
line 852
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 96
LTI4 $587
line 855
;855:}
LABELV $584
endproc Font_Report 8 12
export UI_Report
proc UI_Report 0 0
line 857
;856:
;857:void UI_Report() {
line 858
;858:  String_Report();
ADDRGP4 String_Report
CALLV
pop
line 861
;859:  //Font_Report();
;860:
;861:}
LABELV $595
endproc UI_Report 0 0
export UI_ParseMenu
proc UI_ParseMenu 1060 12
line 863
;862:
;863:void UI_ParseMenu(const char *menuFile) {
line 867
;864:	int handle;
;865:	pc_token_t token;
;866:
;867:	Com_Printf("Parsing menu file:%s\n", menuFile);
ADDRGP4 $597
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 869
;868:
;869:	handle = trap_PC_LoadSource(menuFile);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_LoadSource
CALLI4
ASGNI4
ADDRLP4 1040
ADDRLP4 1044
INDIRI4
ASGNI4
line 870
;870:	if (!handle) {
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $601
line 871
;871:		return;
ADDRGP4 $596
JUMPV
LABELV $600
line 874
;872:	}
;873:
;874:	while ( 1 ) {
line 875
;875:		memset(&token, 0, sizeof(pc_token_t));
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1040
ARGI4
ADDRGP4 memset
CALLP4
pop
line 876
;876:		if (!trap_PC_ReadToken( handle, &token )) {
ADDRLP4 1040
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $603
line 877
;877:			break;
ADDRGP4 $602
JUMPV
LABELV $603
line 890
;878:		}
;879:
;880:		//if ( Q_stricmp( token, "{" ) ) {
;881:		//	Com_Printf( "Missing { in menu file\n" );
;882:		//	break;
;883:		//}
;884:
;885:		//if ( menuCount == MAX_MENUS ) {
;886:		//	Com_Printf( "Too many menus!\n" );
;887:		//	break;
;888:		//}
;889:
;890:		if ( token.string[0] == '}' ) {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $605
line 891
;891:			break;
ADDRGP4 $602
JUMPV
LABELV $605
line 894
;892:		}
;893:
;894:		if (Q_stricmp(token.string, "assetGlobalDef") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $611
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $608
line 895
;895:			if (Asset_Parse(handle)) {
ADDRLP4 1040
INDIRI4
ARGI4
ADDRLP4 1056
ADDRGP4 Asset_Parse
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $602
line 896
;896:				continue;
ADDRGP4 $601
JUMPV
line 897
;897:			} else {
line 898
;898:				break;
LABELV $608
line 902
;899:			}
;900:		}
;901:
;902:		if (Q_stricmp(token.string, "menudef") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $617
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $614
line 904
;903:			// start a new menu
;904:			Menu_New(handle);
ADDRLP4 1040
INDIRI4
ARGI4
ADDRGP4 Menu_New
CALLV
pop
line 905
;905:		}
LABELV $614
line 906
;906:	}
LABELV $601
line 874
ADDRGP4 $600
JUMPV
LABELV $602
line 907
;907:	trap_PC_FreeSource(handle);
ADDRLP4 1040
INDIRI4
ARGI4
ADDRGP4 trap_PC_FreeSource
CALLI4
pop
line 908
;908:}
LABELV $596
endproc UI_ParseMenu 1060 12
export Load_Menu
proc Load_Menu 1048 8
line 910
;909:
;910:qboolean Load_Menu(int handle) {
line 913
;911:	pc_token_t token;
;912:
;913:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1040
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $619
line 914
;914:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $618
JUMPV
LABELV $619
line 915
;915:	if (token.string[0] != '{') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $625
line 916
;916:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $618
JUMPV
LABELV $624
line 919
;917:	}
;918:
;919:	while ( 1 ) {
line 921
;920:
;921:		if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $627
line 922
;922:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $618
JUMPV
LABELV $627
line 924
;923:    
;924:		if ( token.string[0] == 0 ) {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $629
line 925
;925:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $618
JUMPV
LABELV $629
line 928
;926:		}
;927:
;928:		if ( token.string[0] == '}' ) {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $632
line 929
;929:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $618
JUMPV
LABELV $632
line 932
;930:		}
;931:
;932:		UI_ParseMenu(token.string); 
ADDRLP4 0+16
ARGP4
ADDRGP4 UI_ParseMenu
CALLV
pop
line 933
;933:	}
LABELV $625
line 919
ADDRGP4 $624
JUMPV
line 934
;934:	return qfalse;
CNSTI4 0
RETI4
LABELV $618
endproc Load_Menu 1048 8
export UI_LoadMenus
proc UI_LoadMenus 1068 8
line 937
;935:}
;936:
;937:void UI_LoadMenus(const char *menuFile, qboolean reset) {
line 942
;938:	pc_token_t token;
;939:	int handle;
;940:	int start;
;941:
;942:	start = trap_Milliseconds();
ADDRLP4 1048
ADDRGP4 trap_Milliseconds
CALLI4
ASGNI4
ADDRLP4 1044
ADDRLP4 1048
INDIRI4
ASGNI4
line 944
;943:
;944:	handle = trap_PC_LoadSource( menuFile );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1052
ADDRGP4 trap_PC_LoadSource
CALLI4
ASGNI4
ADDRLP4 1040
ADDRLP4 1052
INDIRI4
ASGNI4
line 945
;945:	if (!handle) {
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $637
line 946
;946:		trap_Error( va( S_COLOR_YELLOW "menu file not found: %s, using default\n", menuFile ) );
ADDRGP4 $639
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRGP4 trap_Error
CALLV
pop
line 947
;947:		handle = trap_PC_LoadSource( "ui/menus.txt" );
ADDRGP4 $640
ARGP4
ADDRLP4 1060
ADDRGP4 trap_PC_LoadSource
CALLI4
ASGNI4
ADDRLP4 1040
ADDRLP4 1060
INDIRI4
ASGNI4
line 948
;948:		if (!handle) {
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $641
line 949
;949:			trap_Error( va( S_COLOR_RED "default menu file not found: ui/menus.txt, unable to continue!\n", menuFile ) );
ADDRGP4 $643
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 trap_Error
CALLV
pop
line 950
;950:		}
LABELV $641
line 951
;951:	}
LABELV $637
line 953
;952:
;953:	ui_new.integer = 1;
ADDRGP4 ui_new+12
CNSTI4 1
ASGNI4
line 955
;954:
;955:	if (reset) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $648
line 956
;956:		Menu_Reset();
ADDRGP4 Menu_Reset
CALLV
pop
line 957
;957:	}
ADDRGP4 $648
JUMPV
LABELV $647
line 959
;958:
;959:	while ( 1 ) {
line 960
;960:		if (!trap_PC_ReadToken(handle, &token))
ADDRLP4 1040
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1056
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $650
line 961
;961:			break;
ADDRGP4 $649
JUMPV
LABELV $650
line 962
;962:		if( token.string[0] == 0 || token.string[0] == '}') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $656
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $652
LABELV $656
line 963
;963:			break;
ADDRGP4 $649
JUMPV
LABELV $652
line 966
;964:		}
;965:
;966:		if ( token.string[0] == '}' ) {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $657
line 967
;967:			break;
ADDRGP4 $649
JUMPV
LABELV $657
line 970
;968:		}
;969:
;970:		if (Q_stricmp(token.string, "loadmenu") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $663
ARGP4
ADDRLP4 1060
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $660
line 971
;971:			if (Load_Menu(handle)) {
ADDRLP4 1040
INDIRI4
ARGI4
ADDRLP4 1064
ADDRGP4 Load_Menu
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
EQI4 $649
line 972
;972:				continue;
line 973
;973:			} else {
line 974
;974:				break;
LABELV $660
line 977
;975:			}
;976:		}
;977:	}
LABELV $648
line 959
ADDRGP4 $647
JUMPV
LABELV $649
line 979
;978:
;979:	Com_Printf("UI menu load time = %d milli seconds\n", trap_Milliseconds() - start);
ADDRLP4 1056
ADDRGP4 trap_Milliseconds
CALLI4
ASGNI4
ADDRGP4 $666
ARGP4
ADDRLP4 1056
INDIRI4
ADDRLP4 1044
INDIRI4
SUBI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 981
;980:
;981:	trap_PC_FreeSource( handle );
ADDRLP4 1040
INDIRI4
ARGI4
ADDRGP4 trap_PC_FreeSource
CALLI4
pop
line 982
;982:}
LABELV $636
endproc UI_LoadMenus 1068 8
export UI_Load
proc UI_Load 1052 8
line 984
;983:
;984:void UI_Load() {
line 986
;985:	char lastName[1024];
;986:  menuDef_t *menu = Menu_GetFocused();
ADDRLP4 1032
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 1032
INDIRP4
ASGNP4
line 987
;987:	char *menuSet = UI_Cvar_VariableString("ui_menuFiles");
ADDRGP4 $668
ARGP4
ADDRLP4 1036
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1036
INDIRP4
ASGNP4
line 988
;988:	if (menu && menu->window.name) {
ADDRLP4 1044
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRLP4 1044
INDIRU4
EQU4 $669
ADDRLP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 1044
INDIRU4
EQU4 $669
line 989
;989:		strcpy(lastName, menu->window.name);
ADDRLP4 8
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 990
;990:	}
LABELV $669
line 991
;991:	if (menuSet == NULL || menuSet[0] == '\0') {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $673
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $671
LABELV $673
line 992
;992:		menuSet = "ui/menus.txt";
ADDRLP4 0
ADDRGP4 $640
ASGNP4
line 993
;993:	}
LABELV $671
line 995
;994:
;995:	String_Init();
ADDRGP4 String_Init
CALLV
pop
line 1000
;996:
;997:#ifdef PRE_RELEASE_TADEMO
;998:	UI_ParseGameInfo("demogameinfo.txt");
;999:#else
;1000:	UI_ParseGameInfo("gameinfo.txt");
ADDRGP4 $674
ARGP4
ADDRGP4 UI_ParseGameInfo
CALLV
pop
line 1001
;1001:	UI_LoadArenas();
ADDRGP4 UI_LoadArenas
CALLV
pop
line 1004
;1002:#endif
;1003:
;1004:	UI_LoadMenus(menuSet, qtrue);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_LoadMenus
CALLV
pop
line 1005
;1005:	Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 1006
;1006:	Menus_ActivateByName(lastName);
ADDRLP4 8
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 1008
;1007:
;1008:}
LABELV $667
endproc UI_Load 1052 8
data
align 4
LABELV handicapValues
address $675
address $676
address $677
address $678
address $679
address $680
address $681
address $682
address $683
address $684
address $685
address $686
address $687
address $688
address $689
address $690
address $691
address $692
address $693
address $694
byte 4 0
code
proc UI_DrawHandicap 20 32
line 1015
;1009:
;1010:static const char *handicapValues[] = {"None","95","90","85","80","75","70","65","60","55","50","45","40","35","30","25","20","15","10","5",NULL};
;1011:#ifndef MISSIONPACK // bk001206
;1012:static int numHandicaps = sizeof(handicapValues) / sizeof(const char*);
;1013:#endif
;1014:
;1015:static void UI_DrawHandicap(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1018
;1016:  int i, h;
;1017:
;1018:  h = Com_Clamp( 5, 100, trap_Cvar_VariableValue("handicap") );
ADDRGP4 $696
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 1084227584
ARGF4
CNSTF4 1120403456
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 1019
;1019:  i = 20 - h / 5;
ADDRLP4 0
CNSTI4 20
ADDRLP4 4
INDIRI4
CNSTI4 5
DIVI4
SUBI4
ASGNI4
line 1021
;1020:
;1021:  Text_Paint(rect->x, rect->y, scale, color, handicapValues[i], 0, 0, textStyle);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 handicapValues
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1022
;1022:}
LABELV $695
endproc UI_DrawHandicap 20 32
proc UI_DrawClanName 8 32
line 1024
;1023:
;1024:static void UI_DrawClanName(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1025
;1025:  Text_Paint(rect->x, rect->y, scale, color, UI_Cvar_VariableString("ui_teamName"), 0, 0, textStyle);
ADDRGP4 $698
ARGP4
ADDRLP4 0
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1026
;1026:}
LABELV $697
endproc UI_DrawClanName 8 32
proc UI_SetCapFragLimits 16 8
line 1029
;1027:
;1028:
;1029:static void UI_SetCapFragLimits(qboolean uiVars) {
line 1030
;1030:	int cap = 5;
ADDRLP4 0
CNSTI4 5
ASGNI4
line 1031
;1031:	int frag = 10;
ADDRLP4 4
CNSTI4 10
ASGNI4
line 1032
;1032:	if (uiInfo.gameTypes[ui_gameType.integer].gtEnum == GT_OBELISK) {
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
CNSTI4 6
NEI4 $700
line 1033
;1033:		cap = 4;
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1034
;1034:	} else if (uiInfo.gameTypes[ui_gameType.integer].gtEnum == GT_HARVESTER) {
ADDRGP4 $701
JUMPV
LABELV $700
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
CNSTI4 7
NEI4 $705
line 1035
;1035:		cap = 15;
ADDRLP4 0
CNSTI4 15
ASGNI4
line 1036
;1036:	}
LABELV $705
LABELV $701
line 1037
;1037:	if (uiVars) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $710
line 1038
;1038:		trap_Cvar_Set("ui_captureLimit", va("%d", cap));
ADDRGP4 $713
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $712
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1039
;1039:		trap_Cvar_Set("ui_fragLimit", va("%d", frag));
ADDRGP4 $713
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $714
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1040
;1040:	} else {
ADDRGP4 $711
JUMPV
LABELV $710
line 1041
;1041:		trap_Cvar_Set("capturelimit", va("%d", cap));
ADDRGP4 $713
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $715
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1042
;1042:		trap_Cvar_Set("fraglimit", va("%d", frag));
ADDRGP4 $713
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $716
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1043
;1043:	}
LABELV $711
line 1044
;1044:}
LABELV $699
endproc UI_SetCapFragLimits 16 8
proc UI_DrawGameType 4 32
line 1046
;1045:// ui_gameType assumes gametype 0 is -1 ALL and will not show
;1046:static void UI_DrawGameType(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1047
;1047:  Text_Paint(rect->x, rect->y, scale, color, uiInfo.gameTypes[ui_gameType.integer].gameType, 0, 0, textStyle);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1048
;1048:}
LABELV $717
endproc UI_DrawGameType 4 32
proc UI_DrawNetGameType 4 32
line 1050
;1049:
;1050:static void UI_DrawNetGameType(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1051
;1051:	if (ui_netGameType.integer < 0 || ui_netGameType.integer > uiInfo.numGameTypes) {
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 0
LTI4 $726
ADDRGP4 ui_netGameType+12
INDIRI4
ADDRGP4 uiInfo+78576
INDIRI4
LEI4 $721
LABELV $726
line 1052
;1052:		trap_Cvar_Set("ui_netGameType", "0");
ADDRGP4 $727
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1053
;1053:		trap_Cvar_Set("ui_actualNetGameType", "0");
ADDRGP4 $728
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1054
;1054:	}
LABELV $721
line 1055
;1055:  Text_Paint(rect->x, rect->y, scale, color, uiInfo.gameTypes[ui_netGameType.integer].gameType , 0, 0, textStyle);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1056
;1056:}
LABELV $720
endproc UI_DrawNetGameType 4 32
proc UI_DrawJoinGameType 4 32
line 1058
;1057:
;1058:static void UI_DrawJoinGameType(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1059
;1059:	if (ui_joinGameType.integer < 0 || ui_joinGameType.integer > uiInfo.numJoinGameTypes) {
ADDRGP4 ui_joinGameType+12
INDIRI4
CNSTI4 0
LTI4 $737
ADDRGP4 ui_joinGameType+12
INDIRI4
ADDRGP4 uiInfo+78708
INDIRI4
LEI4 $732
LABELV $737
line 1060
;1060:		trap_Cvar_Set("ui_joinGameType", "0");
ADDRGP4 $738
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1061
;1061:	}
LABELV $732
line 1062
;1062:  Text_Paint(rect->x, rect->y, scale, color, uiInfo.joinGameTypes[ui_joinGameType.integer].gameType , 0, 0, textStyle);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 ui_joinGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78712
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1063
;1063:}
LABELV $731
endproc UI_DrawJoinGameType 4 32
proc UI_TeamIndexFromName 12 8
line 1067
;1064:
;1065:
;1066:
;1067:static int UI_TeamIndexFromName(const char *name) {
line 1070
;1068:  int i;
;1069:
;1070:  if (name && *name) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $742
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $742
line 1071
;1071:    for (i = 0; i < uiInfo.teamCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $747
JUMPV
LABELV $744
line 1072
;1072:      if (Q_stricmp(name, uiInfo.teamList[i].teamName) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $749
line 1073
;1073:        return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $741
JUMPV
LABELV $749
line 1075
;1074:      }
;1075:    }
LABELV $745
line 1071
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $747
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $744
line 1076
;1076:  } 
LABELV $742
line 1078
;1077:
;1078:  return 0;
CNSTI4 0
RETI4
LABELV $741
endproc UI_TeamIndexFromName 12 8
proc UI_DrawClanLogo 48 20
line 1082
;1079:
;1080:}
;1081:
;1082:static void UI_DrawClanLogo(rectDef_t *rect, float scale, vec4_t color) {
line 1084
;1083:  int i;
;1084:  i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1085
;1085:  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $753
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $753
line 1086
;1086:  	trap_R_SetColor( color );
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1088
;1087:
;1088:		if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $756
line 1089
;1089:      uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 16
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 1090
;1090:      uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 24
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
line 1091
;1091:      uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 36
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 1092
;1092:		}
LABELV $756
line 1094
;1093:
;1094:  	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1095
;1095:    trap_R_SetColor(NULL);
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1096
;1096:  }
LABELV $753
line 1097
;1097:}
LABELV $752
endproc UI_DrawClanLogo 48 20
proc UI_DrawClanCinematic 32 24
line 1099
;1098:
;1099:static void UI_DrawClanCinematic(rectDef_t *rect, float scale, vec4_t color) {
line 1101
;1100:  int i;
;1101:  i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1102
;1102:  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $777
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $777
line 1104
;1103:
;1104:		if (uiInfo.teamList[i].cinematic >= -2) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
CNSTI4 -2
LTI4 $780
line 1105
;1105:			if (uiInfo.teamList[i].cinematic == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $784
line 1106
;1106:				uiInfo.teamList[i].cinematic = trap_CIN_PlayCinematic(va("%s.roq", uiInfo.teamList[i].imageName), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRLP4 16
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 28
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+75760+40
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1107
;1107:			}
LABELV $784
line 1108
;1108:			if (uiInfo.teamList[i].cinematic >= 0) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
CNSTI4 0
LTI4 $793
line 1109
;1109:			  trap_CIN_RunCinematic(uiInfo.teamList[i].cinematic);
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_RunCinematic
CALLI4
pop
line 1110
;1110:				trap_CIN_SetExtents(uiInfo.teamList[i].cinematic, rect->x, rect->y, rect->w, rect->h);
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 trap_CIN_SetExtents
CALLV
pop
line 1111
;1111:	 			trap_CIN_DrawCinematic(uiInfo.teamList[i].cinematic);
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_DrawCinematic
CALLV
pop
line 1112
;1112:			} else {
ADDRGP4 $781
JUMPV
LABELV $793
line 1113
;1113:			  	trap_R_SetColor( color );
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1114
;1114:				UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon_Metal);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+32
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1115
;1115:				trap_R_SetColor(NULL);
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1116
;1116:				uiInfo.teamList[i].cinematic = -2;
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
CNSTI4 -2
ASGNI4
line 1117
;1117:			}
line 1118
;1118:		} else {
ADDRGP4 $781
JUMPV
LABELV $780
line 1119
;1119:	  	trap_R_SetColor( color );
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1120
;1120:			UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1121
;1121:			trap_R_SetColor(NULL);
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1122
;1122:		}
LABELV $781
line 1123
;1123:	}
LABELV $777
line 1125
;1124:
;1125:}
LABELV $776
endproc UI_DrawClanCinematic 32 24
proc UI_DrawPreviewCinematic 16 24
line 1127
;1126:
;1127:static void UI_DrawPreviewCinematic(rectDef_t *rect, float scale, vec4_t color) {
line 1128
;1128:	if (uiInfo.previewMovie > -2) {
ADDRGP4 uiInfo+99260
INDIRI4
CNSTI4 -2
LEI4 $810
line 1129
;1129:		uiInfo.previewMovie = trap_CIN_PlayCinematic(va("%s.roq", uiInfo.movieList[uiInfo.movieIndex]), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRGP4 uiInfo+99256
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+98228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 8
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRGP4 uiInfo+99260
ADDRLP4 8
INDIRI4
ASGNI4
line 1130
;1130:		if (uiInfo.previewMovie >= 0) {
ADDRGP4 uiInfo+99260
INDIRI4
CNSTI4 0
LTI4 $816
line 1131
;1131:		  trap_CIN_RunCinematic(uiInfo.previewMovie);
ADDRGP4 uiInfo+99260
INDIRI4
ARGI4
ADDRGP4 trap_CIN_RunCinematic
CALLI4
pop
line 1132
;1132:			trap_CIN_SetExtents(uiInfo.previewMovie, rect->x, rect->y, rect->w, rect->h);
ADDRGP4 uiInfo+99260
INDIRI4
ARGI4
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 trap_CIN_SetExtents
CALLV
pop
line 1133
;1133: 			trap_CIN_DrawCinematic(uiInfo.previewMovie);
ADDRGP4 uiInfo+99260
INDIRI4
ARGI4
ADDRGP4 trap_CIN_DrawCinematic
CALLV
pop
line 1134
;1134:		} else {
ADDRGP4 $817
JUMPV
LABELV $816
line 1135
;1135:			uiInfo.previewMovie = -2;
ADDRGP4 uiInfo+99260
CNSTI4 -2
ASGNI4
line 1136
;1136:		}
LABELV $817
line 1137
;1137:	} 
LABELV $810
line 1139
;1138:
;1139:}
LABELV $809
endproc UI_DrawPreviewCinematic 16 24
proc UI_DrawSkill 16 32
line 1143
;1140:
;1141:
;1142:
;1143:static void UI_DrawSkill(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1145
;1144:  int i;
;1145:	i = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $824
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 1146
;1146:  if (i < 1 || i > numSkillLevels) {
ADDRLP4 0
INDIRI4
CNSTI4 1
LTI4 $827
ADDRLP4 0
INDIRI4
ADDRGP4 numSkillLevels
INDIRI4
LEI4 $825
LABELV $827
line 1147
;1147:    i = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1148
;1148:  }
LABELV $825
line 1149
;1149:  Text_Paint(rect->x, rect->y, scale, color, skillLevels[i-1],0, 0, textStyle);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 skillLevels-4
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1150
;1150:}
LABELV $823
endproc UI_DrawSkill 16 32
proc UI_DrawTeamName 32 32
line 1153
;1151:
;1152:
;1153:static void UI_DrawTeamName(rectDef_t *rect, float scale, vec4_t color, qboolean blue, int textStyle) {
line 1155
;1154:  int i;
;1155:  i = UI_TeamIndexFromName(UI_Cvar_VariableString((blue) ? "ui_blueTeam" : "ui_redTeam"));
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $833
ADDRLP4 4
ADDRGP4 $830
ASGNP4
ADDRGP4 $834
JUMPV
LABELV $833
ADDRLP4 4
ADDRGP4 $831
ASGNP4
LABELV $834
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 1156
;1156:  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $835
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $835
line 1157
;1157:    Text_Paint(rect->x, rect->y, scale, color, va("%s: %s", (blue) ? "Blue" : "Red", uiInfo.teamList[i].teamName),0, 0, textStyle);
ADDRGP4 $838
ARGP4
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $843
ADDRLP4 20
ADDRGP4 $839
ASGNP4
ADDRGP4 $844
JUMPV
LABELV $843
ADDRLP4 20
ADDRGP4 $840
ASGNP4
LABELV $844
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
INDIRF4
ARGF4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1158
;1158:  }
LABELV $835
line 1159
;1159:}
LABELV $829
endproc UI_DrawTeamName 32 32
proc UI_DrawTeamMember 28 32
line 1161
;1160:
;1161:static void UI_DrawTeamMember(rectDef_t *rect, float scale, vec4_t color, qboolean blue, int num, int textStyle) {
line 1165
;1162:	// 0 - None
;1163:	// 1 - Human
;1164:	// 2..NumCharacters - Bot
;1165:	int value = trap_Cvar_VariableValue(va(blue ? "ui_blueteam%i" : "ui_redteam%i", num));
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $849
ADDRLP4 8
ADDRGP4 $846
ASGNP4
ADDRGP4 $850
JUMPV
LABELV $849
ADDRLP4 8
ADDRGP4 $847
ASGNP4
LABELV $850
ADDRLP4 8
INDIRP4
ARGP4
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 16
INDIRF4
CVFI4 4
ASGNI4
line 1167
;1166:	const char *text;
;1167:	if (value <= 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $851
line 1168
;1168:		text = "Closed";
ADDRLP4 4
ADDRGP4 $853
ASGNP4
line 1169
;1169:	} else if (value == 1) {
ADDRGP4 $852
JUMPV
LABELV $851
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $854
line 1170
;1170:		text = "Human";
ADDRLP4 4
ADDRGP4 $856
ASGNP4
line 1171
;1171:	} else {
ADDRGP4 $855
JUMPV
LABELV $854
line 1172
;1172:		value -= 2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 1174
;1173:
;1174:		if (ui_actualNetGameType.integer >= GT_TEAM) {
ADDRGP4 ui_actualNetGameType+12
INDIRI4
CNSTI4 3
LTI4 $857
line 1175
;1175:			if (value >= uiInfo.characterCount) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $860
line 1176
;1176:				value = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1177
;1177:			}
LABELV $860
line 1178
;1178:			text = uiInfo.characterList[value].name;
ADDRLP4 4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ASGNP4
line 1179
;1179:		} else {
ADDRGP4 $858
JUMPV
LABELV $857
line 1180
;1180:			if (value >= UI_GetNumBots()) {
ADDRLP4 20
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $864
line 1181
;1181:				value = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1182
;1182:			}
LABELV $864
line 1183
;1183:			text = UI_GetBotNameByNumber(value);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_GetBotNameByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 24
INDIRP4
ASGNP4
line 1184
;1184:		}
LABELV $858
line 1185
;1185:	}
LABELV $855
LABELV $852
line 1186
;1186:  Text_Paint(rect->x, rect->y, scale, color, text, 0, 0, textStyle);
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
INDIRF4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1187
;1187:}
LABELV $845
endproc UI_DrawTeamMember 28 32
proc UI_DrawEffects 16 20
line 1189
;1188:
;1189:static void UI_DrawEffects(rectDef_t *rect, float scale, vec4_t color) {
line 1190
;1190:	UI_DrawHandlePic( rect->x, rect->y - 14, 128, 8, uiInfo.uiDC.Assets.fxBasePic );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1096810496
SUBF4
ARGF4
CNSTF4 1124073472
ARGF4
CNSTF4 1090519040
ARGF4
ADDRGP4 uiInfo+228+61768
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1191
;1191:	UI_DrawHandlePic( rect->x + uiInfo.effectsColor * 16 + 8, rect->y - 16, 16, 12, uiInfo.uiDC.Assets.fxPic[uiInfo.effectsColor] );
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTI4 4
ASGNI4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRGP4 uiInfo+136904
INDIRI4
ADDRLP4 8
INDIRI4
LSHI4
CVIF4 4
ADDF4
CNSTF4 1090519040
ADDF4
ARGF4
ADDRLP4 12
CNSTF4 1098907648
ASGNF4
ADDRLP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
CNSTF4 1094713344
ARGF4
ADDRGP4 uiInfo+136904
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+228+61772
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1192
;1192:}
LABELV $866
endproc UI_DrawEffects 16 20
proc UI_DrawMapPreview 20 20
line 1194
;1193:
;1194:static void UI_DrawMapPreview(rectDef_t *rect, float scale, vec4_t color, qboolean net) {
line 1195
;1195:	int map = (net) ? ui_currentNetMap.integer : ui_currentMap.integer;
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $877
ADDRLP4 4
ADDRGP4 ui_currentNetMap+12
INDIRI4
ASGNI4
ADDRGP4 $878
JUMPV
LABELV $877
ADDRLP4 4
ADDRGP4 ui_currentMap+12
INDIRI4
ASGNI4
LABELV $878
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1196
;1196:	if (map < 0 || map > uiInfo.mapCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $882
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LEI4 $879
LABELV $882
line 1197
;1197:		if (net) {
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $883
line 1198
;1198:			ui_currentNetMap.integer = 0;
ADDRGP4 ui_currentNetMap+12
CNSTI4 0
ASGNI4
line 1199
;1199:			trap_Cvar_Set("ui_currentNetMap", "0");
ADDRGP4 $886
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1200
;1200:		} else {
ADDRGP4 $884
JUMPV
LABELV $883
line 1201
;1201:			ui_currentMap.integer = 0;
ADDRGP4 ui_currentMap+12
CNSTI4 0
ASGNI4
line 1202
;1202:			trap_Cvar_Set("ui_currentMap", "0");
ADDRGP4 $888
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1203
;1203:		}
LABELV $884
line 1204
;1204:		map = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1205
;1205:	}
LABELV $879
line 1207
;1206:
;1207:	if (uiInfo.mapList[map].levelShot == -1) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $889
line 1208
;1208:		uiInfo.mapList[map].levelShot = trap_R_RegisterShaderNoMip(uiInfo.mapList[map].imageName);
ADDRLP4 12
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+8
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+92
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1209
;1209:	}
LABELV $889
line 1211
;1210:
;1211:	if (uiInfo.mapList[map].levelShot > 0) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
INDIRI4
CNSTI4 0
LEI4 $897
line 1212
;1212:		UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.mapList[map].levelShot);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1213
;1213:	} else {
ADDRGP4 $898
JUMPV
LABELV $897
line 1214
;1214:		UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, trap_R_RegisterShaderNoMip("menu/art/unknownmap"));
ADDRGP4 $903
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1215
;1215:	}
LABELV $898
line 1216
;1216:}						 
LABELV $873
endproc UI_DrawMapPreview 20 20
proc UI_DrawMapTimeToBeat 20 32
line 1219
;1217:
;1218:
;1219:static void UI_DrawMapTimeToBeat(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1221
;1220:	int minutes, seconds, time;
;1221:	if (ui_currentMap.integer < 0 || ui_currentMap.integer > uiInfo.mapCount) {
ADDRGP4 ui_currentMap+12
INDIRI4
CNSTI4 0
LTI4 $910
ADDRGP4 ui_currentMap+12
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LEI4 $905
LABELV $910
line 1222
;1222:		ui_currentMap.integer = 0;
ADDRGP4 ui_currentMap+12
CNSTI4 0
ASGNI4
line 1223
;1223:		trap_Cvar_Set("ui_currentMap", "0");
ADDRGP4 $888
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1224
;1224:	}
LABELV $905
line 1226
;1225:
;1226:	time = uiInfo.mapList[ui_currentMap.integer].timeToBeat[uiInfo.gameTypes[ui_gameType.integer].gtEnum];
ADDRLP4 0
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+28
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1228
;1227:
;1228:	minutes = time / 60;
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 60
DIVI4
ASGNI4
line 1229
;1229:	seconds = time % 60;
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 60
MODI4
ASGNI4
line 1231
;1230:
;1231:  Text_Paint(rect->x, rect->y, scale, color, va("%02i:%02i", minutes, seconds), 0, 0, textStyle);
ADDRGP4 $918
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1232
;1232:}
LABELV $904
endproc UI_DrawMapTimeToBeat 20 32
proc UI_DrawMapCinematic 28 24
line 1236
;1233:
;1234:
;1235:
;1236:static void UI_DrawMapCinematic(rectDef_t *rect, float scale, vec4_t color, qboolean net) {
line 1238
;1237:
;1238:	int map = (net) ? ui_currentNetMap.integer : ui_currentMap.integer; 
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $923
ADDRLP4 4
ADDRGP4 ui_currentNetMap+12
INDIRI4
ASGNI4
ADDRGP4 $924
JUMPV
LABELV $923
ADDRLP4 4
ADDRGP4 ui_currentMap+12
INDIRI4
ASGNI4
LABELV $924
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1239
;1239:	if (map < 0 || map > uiInfo.mapCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $928
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LEI4 $925
LABELV $928
line 1240
;1240:		if (net) {
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $929
line 1241
;1241:			ui_currentNetMap.integer = 0;
ADDRGP4 ui_currentNetMap+12
CNSTI4 0
ASGNI4
line 1242
;1242:			trap_Cvar_Set("ui_currentNetMap", "0");
ADDRGP4 $886
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1243
;1243:		} else {
ADDRGP4 $930
JUMPV
LABELV $929
line 1244
;1244:			ui_currentMap.integer = 0;
ADDRGP4 ui_currentMap+12
CNSTI4 0
ASGNI4
line 1245
;1245:			trap_Cvar_Set("ui_currentMap", "0");
ADDRGP4 $888
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1246
;1246:		}
LABELV $930
line 1247
;1247:		map = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1248
;1248:	}
LABELV $925
line 1250
;1249:
;1250:	if (uiInfo.mapList[map].cinematic >= -1) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
CNSTI4 -1
LTI4 $933
line 1251
;1251:		if (uiInfo.mapList[map].cinematic == -1) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $937
line 1252
;1252:			uiInfo.mapList[map].cinematic = trap_CIN_PlayCinematic(va("%s.roq", uiInfo.mapList[map].mapLoadName), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRLP4 12
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 24
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+24
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 1253
;1253:		}
LABELV $937
line 1254
;1254:		if (uiInfo.mapList[map].cinematic >= 0) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
CNSTI4 0
LTI4 $945
line 1255
;1255:		  trap_CIN_RunCinematic(uiInfo.mapList[map].cinematic);
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_RunCinematic
CALLI4
pop
line 1256
;1256:		  trap_CIN_SetExtents(uiInfo.mapList[map].cinematic, rect->x, rect->y, rect->w, rect->h);
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 trap_CIN_SetExtents
CALLV
pop
line 1257
;1257: 			trap_CIN_DrawCinematic(uiInfo.mapList[map].cinematic);
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_DrawCinematic
CALLV
pop
line 1258
;1258:		} else {
ADDRGP4 $934
JUMPV
LABELV $945
line 1259
;1259:			uiInfo.mapList[map].cinematic = -2;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
CNSTI4 -2
ASGNI4
line 1260
;1260:		}
line 1261
;1261:	} else {
ADDRGP4 $934
JUMPV
LABELV $933
line 1262
;1262:		UI_DrawMapPreview(rect, scale, color, net);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_DrawMapPreview
CALLV
pop
line 1263
;1263:	}
LABELV $934
line 1264
;1264:}
LABELV $919
endproc UI_DrawMapCinematic 28 24
data
align 4
LABELV updateModel
byte 4 1
align 4
LABELV q3Model
byte 4 0
bss
align 4
LABELV $958
skip 1276
code
proc UI_DrawPlayerModel 616 28
line 1271
;1265:
;1266:
;1267:
;1268:static qboolean updateModel = qtrue;
;1269:static qboolean q3Model = qfalse;
;1270:
;1271:static void UI_DrawPlayerModel(rectDef_t *rect) {
line 1279
;1272:  static playerInfo_t info;
;1273:  char model[MAX_QPATH];
;1274:  char team[256];
;1275:	char head[256];
;1276:	vec3_t	viewangles;
;1277:	vec3_t	moveangles;
;1278:
;1279:	  if (trap_Cvar_VariableValue("ui_Q3Model")) {
ADDRGP4 $961
ARGP4
ADDRLP4 600
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 600
INDIRF4
CNSTF4 0
EQF4 $959
line 1280
;1280:	  strcpy(model, UI_Cvar_VariableString("model"));
ADDRGP4 $962
ARGP4
ADDRLP4 604
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 12
ARGP4
ADDRLP4 604
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1281
;1281:		strcpy(head, UI_Cvar_VariableString("headmodel"));
ADDRGP4 $963
ARGP4
ADDRLP4 608
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 332
ARGP4
ADDRLP4 608
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1282
;1282:		if (!q3Model) {
ADDRGP4 q3Model
INDIRI4
CNSTI4 0
NEI4 $964
line 1283
;1283:			q3Model = qtrue;
ADDRGP4 q3Model
CNSTI4 1
ASGNI4
line 1284
;1284:			updateModel = qtrue;
ADDRGP4 updateModel
CNSTI4 1
ASGNI4
line 1285
;1285:		}
LABELV $964
line 1286
;1286:		team[0] = '\0';
ADDRLP4 76
CNSTI1 0
ASGNI1
line 1287
;1287:	} else {
ADDRGP4 $960
JUMPV
LABELV $959
line 1289
;1288:
;1289:		strcpy(team, UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 604
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 76
ARGP4
ADDRLP4 604
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1290
;1290:		strcpy(model, UI_Cvar_VariableString("team_model"));
ADDRGP4 $966
ARGP4
ADDRLP4 608
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 12
ARGP4
ADDRLP4 608
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1291
;1291:		strcpy(head, UI_Cvar_VariableString("team_headmodel"));
ADDRGP4 $967
ARGP4
ADDRLP4 612
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 332
ARGP4
ADDRLP4 612
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1292
;1292:		if (q3Model) {
ADDRGP4 q3Model
INDIRI4
CNSTI4 0
EQI4 $968
line 1293
;1293:			q3Model = qfalse;
ADDRGP4 q3Model
CNSTI4 0
ASGNI4
line 1294
;1294:			updateModel = qtrue;
ADDRGP4 updateModel
CNSTI4 1
ASGNI4
line 1295
;1295:		}
LABELV $968
line 1296
;1296:	}
LABELV $960
line 1297
;1297:  if (updateModel) {
ADDRGP4 updateModel
INDIRI4
CNSTI4 0
EQI4 $970
line 1298
;1298:  	memset( &info, 0, sizeof(playerInfo_t) );
ADDRGP4 $958
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1276
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1299
;1299:  	viewangles[YAW]   = 180 - 10;
ADDRLP4 0+4
CNSTF4 1126825984
ASGNF4
line 1300
;1300:  	viewangles[PITCH] = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1301
;1301:  	viewangles[ROLL]  = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 1302
;1302:  	VectorClear( moveangles );
ADDRLP4 604
CNSTF4 0
ASGNF4
ADDRLP4 588+8
ADDRLP4 604
INDIRF4
ASGNF4
ADDRLP4 588+4
ADDRLP4 604
INDIRF4
ASGNF4
ADDRLP4 588
ADDRLP4 604
INDIRF4
ASGNF4
line 1303
;1303:    UI_PlayerInfo_SetModel( &info, model, head, team);
ADDRGP4 $958
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 332
ARGP4
ADDRLP4 76
ARGP4
ADDRGP4 UI_PlayerInfo_SetModel
CALLV
pop
line 1304
;1304:    UI_PlayerInfo_SetInfo( &info, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_MACHINEGUN, qfalse );
ADDRGP4 $958
ARGP4
CNSTI4 22
ARGI4
CNSTI4 11
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 vec3_origin
ARGP4
CNSTI4 2
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 UI_PlayerInfo_SetInfo
CALLV
pop
line 1306
;1305://		UI_RegisterClientModelname( &info, model, head, team);
;1306:    updateModel = qfalse;
ADDRGP4 updateModel
CNSTI4 0
ASGNI4
line 1307
;1307:  }
LABELV $970
line 1309
;1308:
;1309:  UI_DrawPlayer( rect->x, rect->y, rect->w, rect->h, &info, uiInfo.uiDC.realTime / 2);
ADDRLP4 604
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 604
INDIRP4
INDIRF4
ARGF4
ADDRLP4 604
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 604
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 604
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRGP4 $958
ARGP4
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 2
DIVI4
ARGI4
ADDRGP4 UI_DrawPlayer
CALLV
pop
line 1311
;1310:
;1311:}
LABELV $957
endproc UI_DrawPlayerModel 616 28
proc UI_DrawNetSource 8 32
line 1313
;1312:
;1313:static void UI_DrawNetSource(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1314
;1314:	if (ui_netSource.integer < 0 || ui_netSource.integer > numNetSources) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
LTI4 $982
ADDRGP4 ui_netSource+12
INDIRI4
ADDRGP4 numNetSources
INDIRI4
LEI4 $978
LABELV $982
line 1315
;1315:		ui_netSource.integer = 0;
ADDRGP4 ui_netSource+12
CNSTI4 0
ASGNI4
line 1316
;1316:	}
LABELV $978
line 1317
;1317:  Text_Paint(rect->x, rect->y, scale, color, va("Source: %s", netSources[ui_netSource.integer]), 0, 0, textStyle);
ADDRGP4 $984
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 netSources
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1318
;1318:}
LABELV $977
endproc UI_DrawNetSource 8 32
proc UI_DrawNetMapPreview 8 20
line 1320
;1319:
;1320:static void UI_DrawNetMapPreview(rectDef_t *rect, float scale, vec4_t color) {
line 1322
;1321:
;1322:	if (uiInfo.serverStatus.currentServerPreview > 0) {
ADDRGP4 uiInfo+99264+10428
INDIRI4
CNSTI4 0
LEI4 $987
line 1323
;1323:		UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.serverStatus.currentServerPreview);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRGP4 uiInfo+99264+10428
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1324
;1324:	} else {
ADDRGP4 $988
JUMPV
LABELV $987
line 1325
;1325:		UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, trap_R_RegisterShaderNoMip("menu/art/unknownmap"));
ADDRGP4 $903
ARGP4
ADDRLP4 0
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1326
;1326:	}
LABELV $988
line 1327
;1327:}
LABELV $986
endproc UI_DrawNetMapPreview 8 20
proc UI_DrawNetMapCinematic 4 20
line 1329
;1328:
;1329:static void UI_DrawNetMapCinematic(rectDef_t *rect, float scale, vec4_t color) {
line 1330
;1330:	if (ui_currentNetMap.integer < 0 || ui_currentNetMap.integer > uiInfo.mapCount) {
ADDRGP4 ui_currentNetMap+12
INDIRI4
CNSTI4 0
LTI4 $999
ADDRGP4 ui_currentNetMap+12
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LEI4 $994
LABELV $999
line 1331
;1331:		ui_currentNetMap.integer = 0;
ADDRGP4 ui_currentNetMap+12
CNSTI4 0
ASGNI4
line 1332
;1332:		trap_Cvar_Set("ui_currentNetMap", "0");
ADDRGP4 $886
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1333
;1333:	}
LABELV $994
line 1335
;1334:
;1335:	if (uiInfo.serverStatus.currentServerCinematic >= 0) {
ADDRGP4 uiInfo+99264+10432
INDIRI4
CNSTI4 0
LTI4 $1001
line 1336
;1336:	  trap_CIN_RunCinematic(uiInfo.serverStatus.currentServerCinematic);
ADDRGP4 uiInfo+99264+10432
INDIRI4
ARGI4
ADDRGP4 trap_CIN_RunCinematic
CALLI4
pop
line 1337
;1337:	  trap_CIN_SetExtents(uiInfo.serverStatus.currentServerCinematic, rect->x, rect->y, rect->w, rect->h);
ADDRGP4 uiInfo+99264+10432
INDIRI4
ARGI4
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 trap_CIN_SetExtents
CALLV
pop
line 1338
;1338: 	  trap_CIN_DrawCinematic(uiInfo.serverStatus.currentServerCinematic);
ADDRGP4 uiInfo+99264+10432
INDIRI4
ARGI4
ADDRGP4 trap_CIN_DrawCinematic
CALLV
pop
line 1339
;1339:	} else {
ADDRGP4 $1002
JUMPV
LABELV $1001
line 1340
;1340:		UI_DrawNetMapPreview(rect, scale, color);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 UI_DrawNetMapPreview
CALLV
pop
line 1341
;1341:	}
LABELV $1002
line 1342
;1342:}
LABELV $993
endproc UI_DrawNetMapCinematic 4 20
proc UI_DrawNetFilter 8 32
line 1346
;1343:
;1344:
;1345:
;1346:static void UI_DrawNetFilter(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1347
;1347:	if (ui_serverFilterType.integer < 0 || ui_serverFilterType.integer > numServerFilters) {
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 0
LTI4 $1016
ADDRGP4 ui_serverFilterType+12
INDIRI4
ADDRGP4 numServerFilters
INDIRI4
LEI4 $1012
LABELV $1016
line 1348
;1348:		ui_serverFilterType.integer = 0;
ADDRGP4 ui_serverFilterType+12
CNSTI4 0
ASGNI4
line 1349
;1349:	}
LABELV $1012
line 1350
;1350:  Text_Paint(rect->x, rect->y, scale, color, va("Filter: %s", serverFilters[ui_serverFilterType.integer].description), 0, 0, textStyle);
ADDRGP4 $1018
ARGP4
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverFilters
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1351
;1351:}
LABELV $1011
endproc UI_DrawNetFilter 8 32
proc UI_DrawTier 20 32
line 1354
;1352:
;1353:
;1354:static void UI_DrawTier(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1356
;1355:  int i;
;1356:	i = trap_Cvar_VariableValue( "ui_currentTier" );
ADDRGP4 $1021
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 1357
;1357:  if (i < 0 || i >= uiInfo.tierCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1025
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+96028
INDIRI4
LTI4 $1022
LABELV $1025
line 1358
;1358:    i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1359
;1359:  }
LABELV $1022
line 1360
;1360:  Text_Paint(rect->x, rect->y, scale, color, va("Tier: %s", uiInfo.tierList[i].tierName),0, 0, textStyle);
ADDRGP4 $1026
ARGP4
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+96032
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1361
;1361:}
LABELV $1020
endproc UI_DrawTier 20 32
proc UI_DrawTierMap 28 20
line 1363
;1362:
;1363:static void UI_DrawTierMap(rectDef_t *rect, int index) {
line 1365
;1364:  int i;
;1365:	i = trap_Cvar_VariableValue( "ui_currentTier" );
ADDRGP4 $1021
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 1366
;1366:  if (i < 0 || i >= uiInfo.tierCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1032
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+96028
INDIRI4
LTI4 $1029
LABELV $1032
line 1367
;1367:    i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1368
;1368:  }
LABELV $1029
line 1370
;1369:
;1370:	if (uiInfo.tierList[i].mapHandles[index] == -1) {
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+96032+28
ADDP4
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1033
line 1371
;1371:		uiInfo.tierList[i].mapHandles[index] = trap_R_RegisterShaderNoMip(va("levelshots/%s", uiInfo.tierList[i].maps[index]));
ADDRGP4 $1039
ARGP4
ADDRLP4 12
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 16
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+96032+4
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+96032+28
ADDP4
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 1372
;1372:	}
LABELV $1033
line 1374
;1373:												 
;1374:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.tierList[i].mapHandles[index]);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+96032+28
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1375
;1375:}
LABELV $1028
endproc UI_DrawTierMap 28 20
proc UI_EnglishMapName 8 8
line 1377
;1376:
;1377:static const char *UI_EnglishMapName(const char *map) {
line 1379
;1378:	int i;
;1379:	for (i = 0; i < uiInfo.mapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1048
JUMPV
LABELV $1045
line 1380
;1380:		if (Q_stricmp(map, uiInfo.mapList[i].mapLoadName) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1050
line 1381
;1381:			return uiInfo.mapList[i].mapName;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
INDIRP4
RETP4
ADDRGP4 $1044
JUMPV
LABELV $1050
line 1383
;1382:		}
;1383:	}
LABELV $1046
line 1379
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1048
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LTI4 $1045
line 1384
;1384:	return "";
ADDRGP4 $109
RETP4
LABELV $1044
endproc UI_EnglishMapName 8 8
proc UI_DrawTierMapName 32 32
line 1387
;1385:}
;1386:
;1387:static void UI_DrawTierMapName(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1389
;1388:  int i, j;
;1389:	i = trap_Cvar_VariableValue( "ui_currentTier" );
ADDRGP4 $1021
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 1390
;1390:  if (i < 0 || i >= uiInfo.tierCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1059
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+96028
INDIRI4
LTI4 $1056
LABELV $1059
line 1391
;1391:    i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1392
;1392:  }
LABELV $1056
line 1393
;1393:	j = trap_Cvar_VariableValue("ui_currentMap");
ADDRGP4 $888
ARGP4
ADDRLP4 16
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 16
INDIRF4
CVFI4 4
ASGNI4
line 1394
;1394:	if (j < 0 || j > MAPS_PER_TIER) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $1062
ADDRLP4 4
INDIRI4
CNSTI4 3
LEI4 $1060
LABELV $1062
line 1395
;1395:		j = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1396
;1396:	}
LABELV $1060
line 1398
;1397:
;1398:  Text_Paint(rect->x, rect->y, scale, color, UI_EnglishMapName(uiInfo.tierList[i].maps[j]), 0, 0, textStyle);
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+96032+4
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 UI_EnglishMapName
CALLP4
ASGNP4
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
INDIRF4
ARGF4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1399
;1399:}
LABELV $1055
endproc UI_DrawTierMapName 32 32
proc UI_DrawTierGameType 28 32
line 1401
;1400:
;1401:static void UI_DrawTierGameType(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1403
;1402:  int i, j;
;1403:	i = trap_Cvar_VariableValue( "ui_currentTier" );
ADDRGP4 $1021
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 1404
;1404:  if (i < 0 || i >= uiInfo.tierCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1069
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+96028
INDIRI4
LTI4 $1066
LABELV $1069
line 1405
;1405:    i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1406
;1406:  }
LABELV $1066
line 1407
;1407:	j = trap_Cvar_VariableValue("ui_currentMap");
ADDRGP4 $888
ARGP4
ADDRLP4 16
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 16
INDIRF4
CVFI4 4
ASGNI4
line 1408
;1408:	if (j < 0 || j > MAPS_PER_TIER) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $1072
ADDRLP4 4
INDIRI4
CNSTI4 3
LEI4 $1070
LABELV $1072
line 1409
;1409:		j = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1410
;1410:	}
LABELV $1070
line 1412
;1411:
;1412:  Text_Paint(rect->x, rect->y, scale, color, uiInfo.gameTypes[uiInfo.tierList[i].gameTypes[j]].gameType , 0, 0, textStyle);
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
INDIRF4
ARGF4
ADDRLP4 24
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+96032+16
ADDP4
ADDP4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1413
;1413:}
LABELV $1065
endproc UI_DrawTierGameType 28 32
proc UI_AIFromName 8 8
line 1423
;1414:
;1415:
;1416:#ifndef MISSIONPACK // bk001206
;1417:static const char *UI_OpponentLeaderName() {
;1418:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
;1419:	return uiInfo.teamList[i].teamMembers[0];
;1420:}
;1421:#endif
;1422:
;1423:static const char *UI_AIFromName(const char *name) {
line 1425
;1424:	int j;
;1425:	for (j = 0; j < uiInfo.aliasCount; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1080
JUMPV
LABELV $1077
line 1426
;1426:		if (Q_stricmp(uiInfo.aliasList[j].name, name) == 0) {
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1082
line 1427
;1427:			return uiInfo.aliasList[j].ai;
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+74988+4
ADDP4
INDIRP4
RETP4
ADDRGP4 $1076
JUMPV
LABELV $1082
line 1429
;1428:		}
;1429:	}
LABELV $1078
line 1425
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1080
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
LTI4 $1077
line 1430
;1430:	return "James";
ADDRGP4 $1087
RETP4
LABELV $1076
endproc UI_AIFromName 8 8
data
align 4
LABELV updateOpponentModel
byte 4 1
bss
align 4
LABELV $1089
skip 1276
code
proc UI_DrawOpponent 420 28
line 1480
;1431:}
;1432:
;1433:#ifndef MISSIONPACK // bk001206
;1434:static const int UI_AIIndex(const char *name) {
;1435:	int j;
;1436:	for (j = 0; j < uiInfo.characterCount; j++) {
;1437:		if (Q_stricmp(name, uiInfo.characterList[j].name) == 0) {
;1438:			return j;
;1439:		}
;1440:	}
;1441:	return 0;
;1442:}
;1443:#endif
;1444:
;1445:#ifndef MISSIONPACK // bk001206
;1446:static const int UI_AIIndexFromName(const char *name) {
;1447:	int j;
;1448:	for (j = 0; j < uiInfo.aliasCount; j++) {
;1449:		if (Q_stricmp(uiInfo.aliasList[j].name, name) == 0) {
;1450:			return UI_AIIndex(uiInfo.aliasList[j].ai);
;1451:		}
;1452:	}
;1453:	return 0;
;1454:}
;1455:#endif
;1456:
;1457:
;1458:#ifndef MISSIONPACK // bk001206
;1459:static const char *UI_OpponentLeaderHead() {
;1460:	const char *leader = UI_OpponentLeaderName();
;1461:	return UI_AIFromName(leader);
;1462:}
;1463:#endif
;1464:
;1465:#ifndef MISSIONPACK // bk001206
;1466:static const char *UI_OpponentLeaderModel() {
;1467:	int i;
;1468:	const char *head = UI_OpponentLeaderHead();
;1469:	for (i = 0; i < uiInfo.characterCount; i++) {
;1470:		if (Q_stricmp(head, uiInfo.characterList[i].name) == 0) {
;1471:			return uiInfo.characterList[i].base;
;1472:		}
;1473:	}
;1474:	return "James";
;1475:}
;1476:#endif
;1477:
;1478:
;1479:static qboolean updateOpponentModel = qtrue;
;1480:static void UI_DrawOpponent(rectDef_t *rect) {
line 1488
;1481:  static playerInfo_t info2;
;1482:  char model[MAX_QPATH];
;1483:  char headmodel[MAX_QPATH];
;1484:  char team[256];
;1485:	vec3_t	viewangles;
;1486:	vec3_t	moveangles;
;1487:  
;1488:	if (updateOpponentModel) {
ADDRGP4 updateOpponentModel
INDIRI4
CNSTI4 0
EQI4 $1090
line 1490
;1489:		
;1490:		strcpy(model, UI_Cvar_VariableString("ui_opponentModel"));
ADDRGP4 $1092
ARGP4
ADDRLP4 408
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 12
ARGP4
ADDRLP4 408
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1491
;1491:	  strcpy(headmodel, UI_Cvar_VariableString("ui_opponentModel"));
ADDRGP4 $1092
ARGP4
ADDRLP4 412
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 76
ARGP4
ADDRLP4 412
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1492
;1492:		team[0] = '\0';
ADDRLP4 152
CNSTI1 0
ASGNI1
line 1494
;1493:
;1494:  	memset( &info2, 0, sizeof(playerInfo_t) );
ADDRGP4 $1089
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1276
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1495
;1495:  	viewangles[YAW]   = 180 - 10;
ADDRLP4 0+4
CNSTF4 1126825984
ASGNF4
line 1496
;1496:  	viewangles[PITCH] = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1497
;1497:  	viewangles[ROLL]  = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 1498
;1498:  	VectorClear( moveangles );
ADDRLP4 416
CNSTF4 0
ASGNF4
ADDRLP4 140+8
ADDRLP4 416
INDIRF4
ASGNF4
ADDRLP4 140+4
ADDRLP4 416
INDIRF4
ASGNF4
ADDRLP4 140
ADDRLP4 416
INDIRF4
ASGNF4
line 1499
;1499:    UI_PlayerInfo_SetModel( &info2, model, headmodel, "");
ADDRGP4 $1089
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 76
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 UI_PlayerInfo_SetModel
CALLV
pop
line 1500
;1500:    UI_PlayerInfo_SetInfo( &info2, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_MACHINEGUN, qfalse );
ADDRGP4 $1089
ARGP4
CNSTI4 22
ARGI4
CNSTI4 11
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 vec3_origin
ARGP4
CNSTI4 2
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 UI_PlayerInfo_SetInfo
CALLV
pop
line 1501
;1501:		UI_RegisterClientModelname( &info2, model, headmodel, team);
ADDRGP4 $1089
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 152
ARGP4
ADDRGP4 UI_RegisterClientModelname
CALLI4
pop
line 1502
;1502:    updateOpponentModel = qfalse;
ADDRGP4 updateOpponentModel
CNSTI4 0
ASGNI4
line 1503
;1503:  }
LABELV $1090
line 1505
;1504:
;1505:  UI_DrawPlayer( rect->x, rect->y, rect->w, rect->h, &info2, uiInfo.uiDC.realTime / 2);
ADDRLP4 408
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 408
INDIRP4
INDIRF4
ARGF4
ADDRLP4 408
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 408
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 408
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRGP4 $1089
ARGP4
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 2
DIVI4
ARGI4
ADDRGP4 UI_DrawPlayer
CALLV
pop
line 1507
;1506:
;1507:}
LABELV $1088
endproc UI_DrawOpponent 420 28
proc UI_NextOpponent 24 8
line 1509
;1508:
;1509:static void UI_NextOpponent() {
line 1510
;1510:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 1511
;1511:  int j = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 16
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
ASGNI4
line 1512
;1512:	i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1513
;1513:	if (i >= uiInfo.teamCount) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $1100
line 1514
;1514:		i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1515
;1515:	}
LABELV $1100
line 1516
;1516:	if (i == j) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $1103
line 1517
;1517:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1518
;1518:		if ( i >= uiInfo.teamCount) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $1105
line 1519
;1519:			i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1520
;1520:		}
LABELV $1105
line 1521
;1521:	}
LABELV $1103
line 1522
;1522: 	trap_Cvar_Set( "ui_opponentName", uiInfo.teamList[i].teamName );
ADDRGP4 $1099
ARGP4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1523
;1523:}
LABELV $1098
endproc UI_NextOpponent 24 8
proc UI_PriorOpponent 24 8
line 1525
;1524:
;1525:static void UI_PriorOpponent() {
line 1526
;1526:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 1527
;1527:  int j = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 16
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
ASGNI4
line 1528
;1528:	i--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1529
;1529:	if (i < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $1110
line 1530
;1530:		i = uiInfo.teamCount - 1;
ADDRLP4 0
ADDRGP4 uiInfo+75756
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1531
;1531:	}
LABELV $1110
line 1532
;1532:	if (i == j) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $1113
line 1533
;1533:		i--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1534
;1534:		if ( i < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $1115
line 1535
;1535:			i = uiInfo.teamCount - 1;
ADDRLP4 0
ADDRGP4 uiInfo+75756
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1536
;1536:		}
LABELV $1115
line 1537
;1537:	}
LABELV $1113
line 1538
;1538: 	trap_Cvar_Set( "ui_opponentName", uiInfo.teamList[i].teamName );
ADDRGP4 $1099
ARGP4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1539
;1539:}
LABELV $1109
endproc UI_PriorOpponent 24 8
proc UI_DrawPlayerLogo 44 20
line 1541
;1540:
;1541:static void	UI_DrawPlayerLogo(rectDef_t *rect, vec3_t color) {
line 1542
;1542:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1544
;1543:
;1544:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1120
line 1545
;1545:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1546
;1546:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1547
;1547:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1548
;1548:	}
LABELV $1120
line 1550
;1549:
;1550: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1551
;1551:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1552
;1552: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1553
;1553:}
LABELV $1119
endproc UI_DrawPlayerLogo 44 20
proc UI_DrawPlayerLogoMetal 44 20
line 1555
;1554:
;1555:static void	UI_DrawPlayerLogoMetal(rectDef_t *rect, vec3_t color) {
line 1556
;1556:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1557
;1557:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1139
line 1558
;1558:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1559
;1559:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1560
;1560:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1561
;1561:	}
LABELV $1139
line 1563
;1562:
;1563: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1564
;1564:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon_Metal );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+32
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1565
;1565: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1566
;1566:}
LABELV $1138
endproc UI_DrawPlayerLogoMetal 44 20
proc UI_DrawPlayerLogoName 44 20
line 1568
;1567:
;1568:static void	UI_DrawPlayerLogoName(rectDef_t *rect, vec3_t color) {
line 1569
;1569:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1570
;1570:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1158
line 1571
;1571:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1572
;1572:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1573
;1573:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1574
;1574:	}
LABELV $1158
line 1576
;1575:
;1576: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1577
;1577:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon_Name );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+36
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1578
;1578: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1579
;1579:}
LABELV $1157
endproc UI_DrawPlayerLogoName 44 20
proc UI_DrawOpponentLogo 44 20
line 1581
;1580:
;1581:static void	UI_DrawOpponentLogo(rectDef_t *rect, vec3_t color) {
line 1582
;1582:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1583
;1583:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1177
line 1584
;1584:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1585
;1585:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1586
;1586:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1587
;1587:	}
LABELV $1177
line 1589
;1588:
;1589: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1590
;1590:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1591
;1591: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1592
;1592:}
LABELV $1176
endproc UI_DrawOpponentLogo 44 20
proc UI_DrawOpponentLogoMetal 44 20
line 1594
;1593:
;1594:static void	UI_DrawOpponentLogoMetal(rectDef_t *rect, vec3_t color) {
line 1595
;1595:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1596
;1596:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1196
line 1597
;1597:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1598
;1598:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1599
;1599:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1600
;1600:	}
LABELV $1196
line 1602
;1601:
;1602: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1603
;1603:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon_Metal );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+32
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1604
;1604: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1605
;1605:}
LABELV $1195
endproc UI_DrawOpponentLogoMetal 44 20
proc UI_DrawOpponentLogoName 44 20
line 1607
;1606:
;1607:static void	UI_DrawOpponentLogoName(rectDef_t *rect, vec3_t color) {
line 1608
;1608:  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 1609
;1609:	if (uiInfo.teamList[i].teamIcon == -1) {
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1215
line 1610
;1610:    uiInfo.teamList[i].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[i].imageName);
ADDRLP4 12
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1611
;1611:    uiInfo.teamList[i].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[i].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 20
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 1612
;1612:    uiInfo.teamList[i].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[i].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 32
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1613
;1613:	}
LABELV $1215
line 1615
;1614:
;1615: 	trap_R_SetColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1616
;1616:	UI_DrawHandlePic( rect->x, rect->y, rect->w, rect->h, uiInfo.teamList[i].teamIcon_Name );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTI4 44
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+36
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1617
;1617: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1618
;1618:}
LABELV $1214
endproc UI_DrawOpponentLogoName 44 20
proc UI_DrawAllMapsSelection 16 32
line 1620
;1619:
;1620:static void UI_DrawAllMapsSelection(rectDef_t *rect, float scale, vec4_t color, int textStyle, qboolean net) {
line 1621
;1621:	int map = (net) ? ui_currentNetMap.integer : ui_currentMap.integer;
ADDRFP4 16
INDIRI4
CNSTI4 0
EQI4 $1237
ADDRLP4 4
ADDRGP4 ui_currentNetMap+12
INDIRI4
ASGNI4
ADDRGP4 $1238
JUMPV
LABELV $1237
ADDRLP4 4
ADDRGP4 ui_currentMap+12
INDIRI4
ASGNI4
LABELV $1238
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1622
;1622:	if (map >= 0 && map < uiInfo.mapCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1239
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
GEI4 $1239
line 1623
;1623:	  Text_Paint(rect->x, rect->y, scale, color, uiInfo.mapList[map].mapName, 0, 0, textStyle);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1624
;1624:	}
LABELV $1239
line 1625
;1625:}
LABELV $1233
endproc UI_DrawAllMapsSelection 16 32
proc UI_DrawOpponentName 8 32
line 1627
;1626:
;1627:static void UI_DrawOpponentName(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1628
;1628:  Text_Paint(rect->x, rect->y, scale, color, UI_Cvar_VariableString("ui_opponentName"), 0, 0, textStyle);
ADDRGP4 $1099
ARGP4
ADDRLP4 0
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1629
;1629:}
LABELV $1243
endproc UI_DrawOpponentName 8 32
proc UI_OwnerDrawWidth 112 12
line 1632
;1630:
;1631:
;1632:static int UI_OwnerDrawWidth(int ownerDraw, float scale) {
line 1635
;1633:	int i, h, value;
;1634:	const char *text;
;1635:	const char *s = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1637
;1636:
;1637:  switch (ownerDraw) {
ADDRLP4 20
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 200
LTI4 $1246
ADDRLP4 20
INDIRI4
CNSTI4 247
GTI4 $1316
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1317-800
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1317
address $1247
address $1246
address $1246
address $1248
address $1246
address $1249
address $1246
address $1252
address $1257
address $1262
address $1267
address $1267
address $1267
address $1267
address $1267
address $1277
address $1277
address $1277
address $1277
address $1277
address $1286
address $1246
address $1295
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1246
address $1313
code
LABELV $1316
ADDRFP4 0
INDIRI4
CNSTI4 250
EQI4 $1308
ADDRGP4 $1246
JUMPV
LABELV $1247
line 1639
;1638:    case UI_HANDICAP:
;1639:			  h = Com_Clamp( 5, 100, trap_Cvar_VariableValue("handicap") );
ADDRGP4 $696
ARGP4
ADDRLP4 24
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 1084227584
ARGF4
CNSTF4 1120403456
ARGF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 28
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 28
INDIRF4
CVFI4 4
ASGNI4
line 1640
;1640:				i = 20 - h / 5;
ADDRLP4 4
CNSTI4 20
ADDRLP4 16
INDIRI4
CNSTI4 5
DIVI4
SUBI4
ASGNI4
line 1641
;1641:				s = handicapValues[i];
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 handicapValues
ADDP4
INDIRP4
ASGNP4
line 1642
;1642:      break;
ADDRGP4 $1246
JUMPV
LABELV $1248
line 1644
;1643:    case UI_CLANNAME:
;1644:				s = UI_Cvar_VariableString("ui_teamName");
ADDRGP4 $698
ARGP4
ADDRLP4 32
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
ASGNP4
line 1645
;1645:      break;
ADDRGP4 $1246
JUMPV
LABELV $1249
line 1647
;1646:    case UI_GAMETYPE:
;1647:				s = uiInfo.gameTypes[ui_gameType.integer].gameType;
ADDRLP4 0
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580
ADDP4
INDIRP4
ASGNP4
line 1648
;1648:      break;
ADDRGP4 $1246
JUMPV
LABELV $1252
line 1650
;1649:    case UI_SKILL:
;1650:				i = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $824
ARGP4
ADDRLP4 36
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 36
INDIRF4
CVFI4 4
ASGNI4
line 1651
;1651:				if (i < 1 || i > numSkillLevels) {
ADDRLP4 40
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 1
LTI4 $1255
ADDRLP4 40
INDIRI4
ADDRGP4 numSkillLevels
INDIRI4
LEI4 $1253
LABELV $1255
line 1652
;1652:					i = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1653
;1653:				}
LABELV $1253
line 1654
;1654:			  s = skillLevels[i-1];
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 skillLevels-4
ADDP4
INDIRP4
ASGNP4
line 1655
;1655:      break;
ADDRGP4 $1246
JUMPV
LABELV $1257
line 1657
;1656:    case UI_BLUETEAMNAME:
;1657:			  i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_blueTeam"));
ADDRGP4 $830
ARGP4
ADDRLP4 44
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 48
INDIRI4
ASGNI4
line 1658
;1658:			  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 52
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
LTI4 $1246
ADDRLP4 52
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $1246
line 1659
;1659:			    s = va("%s: %s", "Blue", uiInfo.teamList[i].teamName);
ADDRGP4 $838
ARGP4
ADDRGP4 $839
ARGP4
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 56
INDIRP4
ASGNP4
line 1660
;1660:			  }
line 1661
;1661:      break;
ADDRGP4 $1246
JUMPV
LABELV $1262
line 1663
;1662:    case UI_REDTEAMNAME:
;1663:			  i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_redTeam"));
ADDRGP4 $831
ARGP4
ADDRLP4 56
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 60
INDIRI4
ASGNI4
line 1664
;1664:			  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 64
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
LTI4 $1246
ADDRLP4 64
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $1246
line 1665
;1665:			    s = va("%s: %s", "Red", uiInfo.teamList[i].teamName);
ADDRGP4 $838
ARGP4
ADDRGP4 $840
ARGP4
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 68
INDIRP4
ASGNP4
line 1666
;1666:			  }
line 1667
;1667:      break;
ADDRGP4 $1246
JUMPV
LABELV $1267
line 1673
;1668:    case UI_BLUETEAM1:
;1669:		case UI_BLUETEAM2:
;1670:		case UI_BLUETEAM3:
;1671:		case UI_BLUETEAM4:
;1672:		case UI_BLUETEAM5:
;1673:			value = trap_Cvar_VariableValue(va("ui_blueteam%i", ownerDraw-UI_BLUETEAM1 + 1));
ADDRGP4 $846
ARGP4
ADDRFP4 0
INDIRI4
CNSTI4 210
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 68
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 72
INDIRF4
CVFI4 4
ASGNI4
line 1674
;1674:			if (value <= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
GTI4 $1268
line 1675
;1675:				text = "Closed";
ADDRLP4 12
ADDRGP4 $853
ASGNP4
line 1676
;1676:			} else if (value == 1) {
ADDRGP4 $1269
JUMPV
LABELV $1268
ADDRLP4 8
INDIRI4
CNSTI4 1
NEI4 $1270
line 1677
;1677:				text = "Human";
ADDRLP4 12
ADDRGP4 $856
ASGNP4
line 1678
;1678:			} else {
ADDRGP4 $1271
JUMPV
LABELV $1270
line 1679
;1679:				value -= 2;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 1680
;1680:				if (value >= uiInfo.aliasCount) {
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
LTI4 $1272
line 1681
;1681:					value = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1682
;1682:				}
LABELV $1272
line 1683
;1683:				text = uiInfo.aliasList[value].name;
ADDRLP4 12
CNSTI4 12
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
ASGNP4
line 1684
;1684:			}
LABELV $1271
LABELV $1269
line 1685
;1685:			s = va("%i. %s", ownerDraw-UI_BLUETEAM1 + 1, text);
ADDRGP4 $1276
ARGP4
ADDRFP4 0
INDIRI4
CNSTI4 210
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 76
INDIRP4
ASGNP4
line 1686
;1686:      break;
ADDRGP4 $1246
JUMPV
LABELV $1277
line 1692
;1687:    case UI_REDTEAM1:
;1688:		case UI_REDTEAM2:
;1689:		case UI_REDTEAM3:
;1690:		case UI_REDTEAM4:
;1691:		case UI_REDTEAM5:
;1692:			value = trap_Cvar_VariableValue(va("ui_redteam%i", ownerDraw-UI_REDTEAM1 + 1));
ADDRGP4 $847
ARGP4
ADDRFP4 0
INDIRI4
CNSTI4 215
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 80
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 84
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 84
INDIRF4
CVFI4 4
ASGNI4
line 1693
;1693:			if (value <= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
GTI4 $1278
line 1694
;1694:				text = "Closed";
ADDRLP4 12
ADDRGP4 $853
ASGNP4
line 1695
;1695:			} else if (value == 1) {
ADDRGP4 $1279
JUMPV
LABELV $1278
ADDRLP4 8
INDIRI4
CNSTI4 1
NEI4 $1280
line 1696
;1696:				text = "Human";
ADDRLP4 12
ADDRGP4 $856
ASGNP4
line 1697
;1697:			} else {
ADDRGP4 $1281
JUMPV
LABELV $1280
line 1698
;1698:				value -= 2;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 1699
;1699:				if (value >= uiInfo.aliasCount) {
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
LTI4 $1282
line 1700
;1700:					value = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1701
;1701:				}
LABELV $1282
line 1702
;1702:				text = uiInfo.aliasList[value].name;
ADDRLP4 12
CNSTI4 12
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
ASGNP4
line 1703
;1703:			}
LABELV $1281
LABELV $1279
line 1704
;1704:			s = va("%i. %s", ownerDraw-UI_REDTEAM1 + 1, text);
ADDRGP4 $1276
ARGP4
ADDRFP4 0
INDIRI4
CNSTI4 215
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 88
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 88
INDIRP4
ASGNP4
line 1705
;1705:      break;
ADDRGP4 $1246
JUMPV
LABELV $1286
line 1707
;1706:		case UI_NETSOURCE:
;1707:			if (ui_netSource.integer < 0 || ui_netSource.integer > uiInfo.numJoinGameTypes) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
LTI4 $1292
ADDRGP4 ui_netSource+12
INDIRI4
ADDRGP4 uiInfo+78708
INDIRI4
LEI4 $1287
LABELV $1292
line 1708
;1708:				ui_netSource.integer = 0;
ADDRGP4 ui_netSource+12
CNSTI4 0
ASGNI4
line 1709
;1709:			}
LABELV $1287
line 1710
;1710:			s = va("Source: %s", netSources[ui_netSource.integer]);
ADDRGP4 $984
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 netSources
ADDP4
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 92
INDIRP4
ASGNP4
line 1711
;1711:			break;
ADDRGP4 $1246
JUMPV
LABELV $1295
line 1713
;1712:		case UI_NETFILTER:
;1713:			if (ui_serverFilterType.integer < 0 || ui_serverFilterType.integer > numServerFilters) {
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 0
LTI4 $1300
ADDRGP4 ui_serverFilterType+12
INDIRI4
ADDRGP4 numServerFilters
INDIRI4
LEI4 $1296
LABELV $1300
line 1714
;1714:				ui_serverFilterType.integer = 0;
ADDRGP4 ui_serverFilterType+12
CNSTI4 0
ASGNI4
line 1715
;1715:			}
LABELV $1296
line 1716
;1716:			s = va("Filter: %s", serverFilters[ui_serverFilterType.integer].description );
ADDRGP4 $1018
ARGP4
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverFilters
ADDP4
INDIRP4
ARGP4
ADDRLP4 96
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 96
INDIRP4
ASGNP4
line 1717
;1717:			break;
ADDRGP4 $1246
JUMPV
line 1719
;1718:		case UI_TIER:
;1719:			break;
line 1721
;1720:		case UI_TIER_MAPNAME:
;1721:			break;
line 1723
;1722:		case UI_TIER_GAMETYPE:
;1723:			break;
line 1725
;1724:		case UI_ALLMAPS_SELECTION:
;1725:			break;
line 1727
;1726:		case UI_OPPONENT_NAME:
;1727:			break;
LABELV $1308
line 1729
;1728:		case UI_KEYBINDSTATUS:
;1729:			if (Display_KeyBindPending()) {
ADDRLP4 100
ADDRGP4 Display_KeyBindPending
CALLI4
ASGNI4
ADDRLP4 100
INDIRI4
CNSTI4 0
EQI4 $1309
line 1730
;1730:				s = "Waiting for new key... Press ESCAPE to cancel";
ADDRLP4 0
ADDRGP4 $1311
ASGNP4
line 1731
;1731:			} else {
ADDRGP4 $1246
JUMPV
LABELV $1309
line 1732
;1732:				s = "Press ENTER or CLICK to change, Press BACKSPACE to clear";
ADDRLP4 0
ADDRGP4 $1312
ASGNP4
line 1733
;1733:			}
line 1734
;1734:			break;
ADDRGP4 $1246
JUMPV
LABELV $1313
line 1736
;1735:		case UI_SERVERREFRESHDATE:
;1736:			s = UI_Cvar_VariableString(va("ui_lastServerRefresh_%i", ui_netSource.integer));
ADDRGP4 $1314
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 104
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 108
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 108
INDIRP4
ASGNP4
line 1737
;1737:			break;
line 1739
;1738:    default:
;1739:      break;
LABELV $1246
line 1742
;1740:  }
;1741:
;1742:	if (s) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1319
line 1743
;1743:		return Text_Width(s, scale, 0);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 24
ADDRGP4 Text_Width
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
RETI4
ADDRGP4 $1244
JUMPV
LABELV $1319
line 1745
;1744:	}
;1745:	return 0;
CNSTI4 0
RETI4
LABELV $1244
endproc UI_OwnerDrawWidth 112 12
proc UI_DrawBotName 24 32
line 1748
;1746:}
;1747:
;1748:static void UI_DrawBotName(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1749
;1749:	int value = uiInfo.botIndex;
ADDRLP4 0
ADDRGP4 uiInfo+73444
INDIRI4
ASGNI4
line 1750
;1750:	int game = trap_Cvar_VariableValue("g_gametype");
ADDRGP4 $1323
ARGP4
ADDRLP4 12
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 1751
;1751:	const char *text = "";
ADDRLP4 4
ADDRGP4 $109
ASGNP4
line 1752
;1752:	if (game >= GT_TEAM) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LTI4 $1324
line 1753
;1753:		if (value >= uiInfo.characterCount) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $1326
line 1754
;1754:			value = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1755
;1755:		}
LABELV $1326
line 1756
;1756:		text = uiInfo.characterList[value].name;
ADDRLP4 4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ASGNP4
line 1757
;1757:	} else {
ADDRGP4 $1325
JUMPV
LABELV $1324
line 1758
;1758:		if (value >= UI_GetNumBots()) {
ADDRLP4 16
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $1330
line 1759
;1759:			value = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1760
;1760:		}
LABELV $1330
line 1761
;1761:		text = UI_GetBotNameByNumber(value);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 UI_GetBotNameByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 1762
;1762:	}
LABELV $1325
line 1763
;1763:  Text_Paint(rect->x, rect->y, scale, color, text, 0, 0, textStyle);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1764
;1764:}
LABELV $1321
endproc UI_DrawBotName 24 32
proc UI_DrawBotSkill 4 32
line 1766
;1765:
;1766:static void UI_DrawBotSkill(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1767
;1767:	if (uiInfo.skillIndex >= 0 && uiInfo.skillIndex < numSkillLevels) {
ADDRGP4 uiInfo+96672
INDIRI4
CNSTI4 0
LTI4 $1333
ADDRGP4 uiInfo+96672
INDIRI4
ADDRGP4 numSkillLevels
INDIRI4
GEI4 $1333
line 1768
;1768:	  Text_Paint(rect->x, rect->y, scale, color, skillLevels[uiInfo.skillIndex], 0, 0, textStyle);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 uiInfo+96672
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 skillLevels
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1769
;1769:	}
LABELV $1333
line 1770
;1770:}
LABELV $1332
endproc UI_DrawBotSkill 4 32
proc UI_DrawRedBlue 8 32
line 1772
;1771:
;1772:static void UI_DrawRedBlue(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1773
;1773:  Text_Paint(rect->x, rect->y, scale, color, (uiInfo.redBlue == 0) ? "Red" : "Blue", 0, 0, textStyle);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 uiInfo+78840
INDIRI4
CNSTI4 0
NEI4 $1341
ADDRLP4 0
ADDRGP4 $840
ASGNP4
ADDRGP4 $1342
JUMPV
LABELV $1341
ADDRLP4 0
ADDRGP4 $839
ASGNP4
LABELV $1342
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1774
;1774:}
LABELV $1338
endproc UI_DrawRedBlue 8 32
proc UI_DrawCrosshair 8 20
line 1776
;1775:
;1776:static void UI_DrawCrosshair(rectDef_t *rect, float scale, vec4_t color) {
line 1777
;1777: 	trap_R_SetColor( color );
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1778
;1778:	if (uiInfo.currentCrosshair < 0 || uiInfo.currentCrosshair >= NUM_CROSSHAIRS) {
ADDRGP4 uiInfo+119476
INDIRI4
CNSTI4 0
LTI4 $1348
ADDRGP4 uiInfo+119476
INDIRI4
CNSTI4 10
LTI4 $1344
LABELV $1348
line 1779
;1779:		uiInfo.currentCrosshair = 0;
ADDRGP4 uiInfo+119476
CNSTI4 0
ASGNI4
line 1780
;1780:	}
LABELV $1344
line 1781
;1781:	UI_DrawHandlePic( rect->x, rect->y - rect->h, rect->w, rect->h, uiInfo.uiDC.Assets.crosshairShader[uiInfo.currentCrosshair]);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRGP4 uiInfo+119476
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+228+61800
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1782
;1782: 	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1783
;1783:}
LABELV $1343
endproc UI_DrawCrosshair 8 20
proc UI_BuildPlayerList 4176 12
line 1790
;1784:
;1785:/*
;1786:===============
;1787:UI_BuildPlayerList
;1788:===============
;1789:*/
;1790:static void UI_BuildPlayerList() {
line 1795
;1791:	uiClientState_t	cs;
;1792:	int		n, count, team, team2, playerTeamNumber;
;1793:	char	info[MAX_INFO_STRING];
;1794:
;1795:	trap_GetClientState( &cs );
ADDRLP4 1040
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 1796
;1796:	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
ADDRLP4 1040+8
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
line 1797
;1797:	uiInfo.playerNumber = cs.clientNum;
ADDRGP4 uiInfo+78864
ADDRLP4 1040+8
INDIRI4
ASGNI4
line 1798
;1798:	uiInfo.teamLeader = atoi(Info_ValueForKey(info, "tl"));
ADDRLP4 4
ARGP4
ADDRGP4 $1358
ARGP4
ADDRLP4 4128
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4128
INDIRP4
ARGP4
ADDRLP4 4132
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 uiInfo+78868
ADDRLP4 4132
INDIRI4
ASGNI4
line 1799
;1799:	team = atoi(Info_ValueForKey(info, "t"));
ADDRLP4 4
ARGP4
ADDRGP4 $1359
ARGP4
ADDRLP4 4136
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4136
INDIRP4
ARGP4
ADDRLP4 4140
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1036
ADDRLP4 4140
INDIRI4
ASGNI4
line 1800
;1800:	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 1801
;1801:	count = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $1360
ARGP4
ADDRLP4 4144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4144
INDIRP4
ARGP4
ADDRLP4 4148
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1028
ADDRLP4 4148
INDIRI4
ASGNI4
line 1802
;1802:	uiInfo.playerCount = 0;
ADDRGP4 uiInfo+78844
CNSTI4 0
ASGNI4
line 1803
;1803:	uiInfo.myTeamCount = 0;
ADDRGP4 uiInfo+78848
CNSTI4 0
ASGNI4
line 1804
;1804:	playerTeamNumber = 0;
ADDRLP4 4124
CNSTI4 0
ASGNI4
line 1805
;1805:	for( n = 0; n < count; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1366
JUMPV
LABELV $1363
line 1806
;1806:		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );
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
line 1808
;1807:
;1808:		if (info[0]) {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $1367
line 1809
;1809:			Q_strncpyz( uiInfo.playerNames[uiInfo.playerCount], Info_ValueForKey( info, "n" ), MAX_NAME_LENGTH );
ADDRLP4 4
ARGP4
ADDRGP4 $1371
ARGP4
ADDRLP4 4152
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 uiInfo+78844
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+78872
ADDP4
ARGP4
ADDRLP4 4152
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1810
;1810:			Q_CleanStr( uiInfo.playerNames[uiInfo.playerCount] );
ADDRGP4 uiInfo+78844
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+78872
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1811
;1811:			uiInfo.playerCount++;
ADDRLP4 4156
ADDRGP4 uiInfo+78844
ASGNP4
ADDRLP4 4156
INDIRP4
ADDRLP4 4156
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1812
;1812:			team2 = atoi(Info_ValueForKey(info, "t"));
ADDRLP4 4
ARGP4
ADDRGP4 $1359
ARGP4
ADDRLP4 4160
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4160
INDIRP4
ARGP4
ADDRLP4 4164
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 4164
INDIRI4
ASGNI4
line 1813
;1813:			if (team2 == team) {
ADDRLP4 1032
INDIRI4
ADDRLP4 1036
INDIRI4
NEI4 $1375
line 1814
;1814:				Q_strncpyz( uiInfo.teamNames[uiInfo.myTeamCount], Info_ValueForKey( info, "n" ), MAX_NAME_LENGTH );
ADDRLP4 4
ARGP4
ADDRGP4 $1371
ARGP4
ADDRLP4 4168
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 uiInfo+78848
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRLP4 4168
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1815
;1815:				Q_CleanStr( uiInfo.teamNames[uiInfo.myTeamCount] );
ADDRGP4 uiInfo+78848
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1816
;1816:				uiInfo.teamClientNums[uiInfo.myTeamCount] = n;
ADDRGP4 uiInfo+78848
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+82968
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1817
;1817:				if (uiInfo.playerNumber == n) {
ADDRGP4 uiInfo+78864
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $1383
line 1818
;1818:					playerTeamNumber = uiInfo.myTeamCount;
ADDRLP4 4124
ADDRGP4 uiInfo+78848
INDIRI4
ASGNI4
line 1819
;1819:				}
LABELV $1383
line 1820
;1820:				uiInfo.myTeamCount++;
ADDRLP4 4172
ADDRGP4 uiInfo+78848
ASGNP4
ADDRLP4 4172
INDIRP4
ADDRLP4 4172
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1821
;1821:			}
LABELV $1375
line 1822
;1822:		}
LABELV $1367
line 1823
;1823:	}
LABELV $1364
line 1805
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1366
ADDRLP4 0
INDIRI4
ADDRLP4 1028
INDIRI4
LTI4 $1363
line 1825
;1824:
;1825:	if (!uiInfo.teamLeader) {
ADDRGP4 uiInfo+78868
INDIRI4
CNSTI4 0
NEI4 $1388
line 1826
;1826:		trap_Cvar_Set("cg_selectedPlayer", va("%d", playerTeamNumber));
ADDRGP4 $713
ARGP4
ADDRLP4 4124
INDIRI4
ARGI4
ADDRLP4 4152
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $1391
ARGP4
ADDRLP4 4152
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1827
;1827:	}
LABELV $1388
line 1829
;1828:
;1829:	n = trap_Cvar_VariableValue("cg_selectedPlayer");
ADDRGP4 $1391
ARGP4
ADDRLP4 4152
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4152
INDIRF4
CVFI4 4
ASGNI4
line 1830
;1830:	if (n < 0 || n > uiInfo.myTeamCount) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $1395
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
LEI4 $1392
LABELV $1395
line 1831
;1831:		n = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1832
;1832:	}
LABELV $1392
line 1833
;1833:	if (n < uiInfo.myTeamCount) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $1396
line 1834
;1834:		trap_Cvar_Set("cg_selectedPlayerName", uiInfo.teamNames[n]);
ADDRGP4 $1399
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1835
;1835:	}
LABELV $1396
line 1836
;1836:}
LABELV $1353
endproc UI_BuildPlayerList 4176 12
proc UI_DrawSelectedPlayer 16 32
line 1839
;1837:
;1838:
;1839:static void UI_DrawSelectedPlayer(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1840
;1840:	if (uiInfo.uiDC.realTime > uiInfo.playerRefresh) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+78856
INDIRI4
LEI4 $1402
line 1841
;1841:		uiInfo.playerRefresh = uiInfo.uiDC.realTime + 3000;
ADDRGP4 uiInfo+78856
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 1842
;1842:		UI_BuildPlayerList();
ADDRGP4 UI_BuildPlayerList
CALLV
pop
line 1843
;1843:	}
LABELV $1402
line 1844
;1844:  Text_Paint(rect->x, rect->y, scale, color, (uiInfo.teamLeader) ? UI_Cvar_VariableString("cg_selectedPlayerName") : UI_Cvar_VariableString("name") , 0, 0, textStyle);
ADDRGP4 uiInfo+78868
INDIRI4
CNSTI4 0
EQI4 $1411
ADDRGP4 $1399
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
ADDRGP4 $1412
JUMPV
LABELV $1411
ADDRGP4 $1409
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
LABELV $1412
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1845
;1845:}
LABELV $1401
endproc UI_DrawSelectedPlayer 16 32
proc UI_DrawServerRefreshDate 80 32
line 1847
;1846:
;1847:static void UI_DrawServerRefreshDate(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1848
;1848:	if (uiInfo.serverStatus.refreshActive) {
ADDRGP4 uiInfo+99264+2212
INDIRI4
CNSTI4 0
EQI4 $1414
line 1850
;1849:		vec4_t lowLight, newColor;
;1850:		lowLight[0] = 0.8 * color[0]; 
ADDRLP4 0
CNSTF4 1061997773
ADDRFP4 8
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1851
;1851:		lowLight[1] = 0.8 * color[1]; 
ADDRLP4 0+4
CNSTF4 1061997773
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
line 1852
;1852:		lowLight[2] = 0.8 * color[2]; 
ADDRLP4 0+8
CNSTF4 1061997773
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 1853
;1853:		lowLight[3] = 0.8 * color[3]; 
ADDRLP4 0+12
CNSTF4 1061997773
ADDRFP4 8
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
MULF4
ASGNF4
line 1854
;1854:		LerpColor(color,lowLight,newColor,0.5+0.5*sin(uiInfo.uiDC.realTime / PULSE_DIVISOR));
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 32
ADDRGP4 sin
CALLF4
ASGNF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
ARGP4
CNSTF4 1056964608
ADDRLP4 32
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 1855
;1855:	  Text_Paint(rect->x, rect->y, scale, newColor, va("Getting info for %d servers (ESC to cancel)", trap_LAN_GetServerCount(ui_netSource.integer)), 0, 0, textStyle);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 36
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRGP4 $1422
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 16
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1856
;1856:	} else {
ADDRGP4 $1415
JUMPV
LABELV $1414
line 1858
;1857:		char buff[64];
;1858:		Q_strncpyz(buff, UI_Cvar_VariableString(va("ui_lastServerRefresh_%i", ui_netSource.integer)), 64);
ADDRGP4 $1314
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 64
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1859
;1859:	  Text_Paint(rect->x, rect->y, scale, color, va("Refresh Time: %s", buff), 0, 0, textStyle);
ADDRGP4 $1425
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 72
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
INDIRF4
ARGF4
ADDRLP4 76
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1860
;1860:	}
LABELV $1415
line 1861
;1861:}
LABELV $1413
endproc UI_DrawServerRefreshDate 80 32
proc UI_DrawServerMOTD 24 32
line 1863
;1862:
;1863:static void UI_DrawServerMOTD(rectDef_t *rect, float scale, vec4_t color) {
line 1864
;1864:	if (uiInfo.serverStatus.motdLen) {
ADDRGP4 uiInfo+99264+10436
INDIRI4
CNSTI4 0
EQI4 $1427
line 1867
;1865:		float maxX;
;1866:	 
;1867:		if (uiInfo.serverStatus.motdWidth == -1) {
ADDRGP4 uiInfo+99264+10440
INDIRI4
CNSTI4 -1
NEI4 $1431
line 1868
;1868:			uiInfo.serverStatus.motdWidth = 0;
ADDRGP4 uiInfo+99264+10440
CNSTI4 0
ASGNI4
line 1869
;1869:			uiInfo.serverStatus.motdPaintX = rect->x + 1;
ADDRGP4 uiInfo+99264+10444
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1870
;1870:			uiInfo.serverStatus.motdPaintX2 = -1;
ADDRGP4 uiInfo+99264+10448
CNSTI4 -1
ASGNI4
line 1871
;1871:		}
LABELV $1431
line 1873
;1872:
;1873:		if (uiInfo.serverStatus.motdOffset > uiInfo.serverStatus.motdLen) {
ADDRGP4 uiInfo+99264+10452
INDIRI4
ADDRGP4 uiInfo+99264+10436
INDIRI4
LEI4 $1441
line 1874
;1874:			uiInfo.serverStatus.motdOffset = 0;
ADDRGP4 uiInfo+99264+10452
CNSTI4 0
ASGNI4
line 1875
;1875:			uiInfo.serverStatus.motdPaintX = rect->x + 1;
ADDRGP4 uiInfo+99264+10444
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1876
;1876:			uiInfo.serverStatus.motdPaintX2 = -1;
ADDRGP4 uiInfo+99264+10448
CNSTI4 -1
ASGNI4
line 1877
;1877:		}
LABELV $1441
line 1879
;1878:
;1879:		if (uiInfo.uiDC.realTime > uiInfo.serverStatus.motdTime) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+99264+10456
INDIRI4
LEI4 $1453
line 1880
;1880:			uiInfo.serverStatus.motdTime = uiInfo.uiDC.realTime + 10;
ADDRGP4 uiInfo+99264+10456
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 10
ADDI4
ASGNI4
line 1881
;1881:			if (uiInfo.serverStatus.motdPaintX <= rect->x + 2) {
ADDRGP4 uiInfo+99264+10444
INDIRI4
CVIF4 4
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1073741824
ADDF4
GTF4 $1461
line 1882
;1882:				if (uiInfo.serverStatus.motdOffset < uiInfo.serverStatus.motdLen) {
ADDRGP4 uiInfo+99264+10452
INDIRI4
ADDRGP4 uiInfo+99264+10436
INDIRI4
GEI4 $1465
line 1883
;1883:					uiInfo.serverStatus.motdPaintX += Text_Width(&uiInfo.serverStatus.motd[uiInfo.serverStatus.motdOffset], scale, 1) - 1;
ADDRGP4 uiInfo+99264+10452
INDIRI4
ADDRGP4 uiInfo+99264+10460
ADDP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 Text_Width
CALLI4
ASGNI4
ADDRLP4 8
ADDRGP4 uiInfo+99264+10444
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ADDI4
ASGNI4
line 1884
;1884:					uiInfo.serverStatus.motdOffset++;
ADDRLP4 12
ADDRGP4 uiInfo+99264+10452
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1885
;1885:				} else {
ADDRGP4 $1462
JUMPV
LABELV $1465
line 1886
;1886:					uiInfo.serverStatus.motdOffset = 0;
ADDRGP4 uiInfo+99264+10452
CNSTI4 0
ASGNI4
line 1887
;1887:					if (uiInfo.serverStatus.motdPaintX2 >= 0) {
ADDRGP4 uiInfo+99264+10448
INDIRI4
CNSTI4 0
LTI4 $1481
line 1888
;1888:						uiInfo.serverStatus.motdPaintX = uiInfo.serverStatus.motdPaintX2;
ADDRGP4 uiInfo+99264+10444
ADDRGP4 uiInfo+99264+10448
INDIRI4
ASGNI4
line 1889
;1889:					} else {
ADDRGP4 $1482
JUMPV
LABELV $1481
line 1890
;1890:						uiInfo.serverStatus.motdPaintX = rect->x + rect->w - 2;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 uiInfo+99264+10444
ADDRLP4 4
INDIRP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1073741824
SUBF4
CVFI4 4
ASGNI4
line 1891
;1891:					}
LABELV $1482
line 1892
;1892:					uiInfo.serverStatus.motdPaintX2 = -1;
ADDRGP4 uiInfo+99264+10448
CNSTI4 -1
ASGNI4
line 1893
;1893:				}
line 1894
;1894:			} else {
ADDRGP4 $1462
JUMPV
LABELV $1461
line 1896
;1895:				//serverStatus.motdPaintX--;
;1896:				uiInfo.serverStatus.motdPaintX -= 2;
ADDRLP4 4
ADDRGP4 uiInfo+99264+10444
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 1897
;1897:				if (uiInfo.serverStatus.motdPaintX2 >= 0) {
ADDRGP4 uiInfo+99264+10448
INDIRI4
CNSTI4 0
LTI4 $1495
line 1899
;1898:					//serverStatus.motdPaintX2--;
;1899:					uiInfo.serverStatus.motdPaintX2 -= 2;
ADDRLP4 8
ADDRGP4 uiInfo+99264+10448
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 1900
;1900:				}
LABELV $1495
line 1901
;1901:			}
LABELV $1462
line 1902
;1902:		}
LABELV $1453
line 1904
;1903:
;1904:		maxX = rect->x + rect->w - 2;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 1905
;1905:		Text_Paint_Limit(&maxX, uiInfo.serverStatus.motdPaintX, rect->y + rect->h - 3, scale, color, &uiInfo.serverStatus.motd[uiInfo.serverStatus.motdOffset], 0, 0); 
ADDRLP4 0
ARGP4
ADDRGP4 uiInfo+99264+10444
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1077936128
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 uiInfo+99264+10452
INDIRI4
ADDRGP4 uiInfo+99264+10460
ADDP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 Text_Paint_Limit
CALLV
pop
line 1906
;1906:		if (uiInfo.serverStatus.motdPaintX2 >= 0) {
ADDRGP4 uiInfo+99264+10448
INDIRI4
CNSTI4 0
LTI4 $1507
line 1907
;1907:			float maxX2 = rect->x + rect->w - 2;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 1908
;1908:			Text_Paint_Limit(&maxX2, uiInfo.serverStatus.motdPaintX2, rect->y + rect->h - 3, scale, color, uiInfo.serverStatus.motd, 0, uiInfo.serverStatus.motdOffset); 
ADDRLP4 12
ARGP4
ADDRGP4 uiInfo+99264+10448
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1077936128
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 uiInfo+99264+10460
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 uiInfo+99264+10452
INDIRI4
ARGI4
ADDRGP4 Text_Paint_Limit
CALLV
pop
line 1909
;1909:		}
LABELV $1507
line 1910
;1910:		if (uiInfo.serverStatus.motdOffset && maxX > 0) {
ADDRGP4 uiInfo+99264+10452
INDIRI4
CNSTI4 0
EQI4 $1517
ADDRLP4 0
INDIRF4
CNSTF4 0
LEF4 $1517
line 1912
;1911:			// if we have an offset ( we are skipping the first part of the string ) and we fit the string
;1912:			if (uiInfo.serverStatus.motdPaintX2 == -1) {
ADDRGP4 uiInfo+99264+10448
INDIRI4
CNSTI4 -1
NEI4 $1518
line 1913
;1913:						uiInfo.serverStatus.motdPaintX2 = rect->x + rect->w - 2;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 uiInfo+99264+10448
ADDRLP4 12
INDIRP4
INDIRF4
ADDRLP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1073741824
SUBF4
CVFI4 4
ASGNI4
line 1914
;1914:			}
line 1915
;1915:		} else {
ADDRGP4 $1518
JUMPV
LABELV $1517
line 1916
;1916:			uiInfo.serverStatus.motdPaintX2 = -1;
ADDRGP4 uiInfo+99264+10448
CNSTI4 -1
ASGNI4
line 1917
;1917:		}
LABELV $1518
line 1919
;1918:
;1919:	}
LABELV $1427
line 1920
;1920:}
LABELV $1426
endproc UI_DrawServerMOTD 24 32
proc UI_DrawKeyBindStatus 8 32
line 1922
;1921:
;1922:static void UI_DrawKeyBindStatus(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1924
;1923://	int ofs = 0; TTimo: unused
;1924:	if (Display_KeyBindPending()) {
ADDRLP4 0
ADDRGP4 Display_KeyBindPending
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $1530
line 1925
;1925:		Text_Paint(rect->x, rect->y, scale, color, "Waiting for new key... Press ESCAPE to cancel", 0, 0, textStyle);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 $1311
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1926
;1926:	} else {
ADDRGP4 $1531
JUMPV
LABELV $1530
line 1927
;1927:		Text_Paint(rect->x, rect->y, scale, color, "Press ENTER or CLICK to change, Press BACKSPACE to clear", 0, 0, textStyle);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 $1312
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1928
;1928:	}
LABELV $1531
line 1929
;1929:}
LABELV $1529
endproc UI_DrawKeyBindStatus 8 32
proc UI_DrawGLInfo 1336 32
line 1931
;1930:
;1931:static void UI_DrawGLInfo(rectDef_t *rect, float scale, vec4_t color, int textStyle) {
line 1937
;1932:	char * eptr;
;1933:	char buff[1024];
;1934:	const char *lines[64];
;1935:	int y, numLines, i;
;1936:
;1937:	Text_Paint(rect->x + 2, rect->y, scale, color, va("VENDOR: %s", uiInfo.uiDC.glconfig.vendor_string), 0, 30, textStyle);
ADDRGP4 $1533
ARGP4
ADDRGP4 uiInfo+62068+1024
ARGP4
ADDRLP4 1296
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1300
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1300
INDIRP4
INDIRF4
CNSTF4 1073741824
ADDF4
ARGF4
ADDRLP4 1300
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 1296
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 30
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1938
;1938:	Text_Paint(rect->x + 2, rect->y + 15, scale, color, va("VERSION: %s: %s", uiInfo.uiDC.glconfig.version_string,uiInfo.uiDC.glconfig.renderer_string), 0, 30, textStyle);
ADDRGP4 $1536
ARGP4
ADDRGP4 uiInfo+62068+2048
ARGP4
ADDRGP4 uiInfo+62068
ARGP4
ADDRLP4 1304
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1308
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1308
INDIRP4
INDIRF4
CNSTF4 1073741824
ADDF4
ARGF4
ADDRLP4 1308
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1097859072
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 1304
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 30
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1939
;1939:	Text_Paint(rect->x + 2, rect->y + 30, scale, color, va ("PIXELFORMAT: color(%d-bits) Z(%d-bits) stencil(%d-bits)", uiInfo.uiDC.glconfig.colorBits, uiInfo.uiDC.glconfig.depthBits, uiInfo.uiDC.glconfig.stencilBits), 0, 30, textStyle);
ADDRGP4 $1540
ARGP4
ADDRGP4 uiInfo+62068+11272
INDIRI4
ARGI4
ADDRGP4 uiInfo+62068+11276
INDIRI4
ARGI4
ADDRGP4 uiInfo+62068+11280
INDIRI4
ARGI4
ADDRLP4 1312
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1316
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1316
INDIRP4
INDIRF4
CNSTF4 1073741824
ADDF4
ARGF4
ADDRLP4 1316
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1106247680
ADDF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 1312
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 30
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1945
;1940:
;1941:	// build null terminated extension strings
;1942:  // TTimo: https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=399
;1943:  // in TA this was not directly crashing, but displaying a nasty broken shader right in the middle
;1944:  // brought down the string size to 1024, there's not much that can be shown on the screen anyway
;1945:	Q_strncpyz(buff, uiInfo.uiDC.glconfig.extensions_string, 1024);
ADDRLP4 272
ARGP4
ADDRGP4 uiInfo+62068+3072
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1946
;1946:	eptr = buff;
ADDRLP4 0
ADDRLP4 272
ASGNP4
line 1947
;1947:	y = rect->y + 45;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1110704128
ADDF4
CVFI4 4
ASGNI4
line 1948
;1948:	numLines = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $1550
JUMPV
line 1950
;1949:	while ( y < rect->y + rect->h && *eptr )
;1950:	{
LABELV $1552
line 1952
;1951:		while ( *eptr && *eptr == ' ' )
;1952:			*eptr++ = '\0';
ADDRLP4 1320
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1320
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1320
INDIRP4
CNSTI1 0
ASGNI1
LABELV $1553
line 1951
ADDRLP4 1324
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1324
INDIRI4
CNSTI4 0
EQI4 $1555
ADDRLP4 1324
INDIRI4
CNSTI4 32
EQI4 $1552
LABELV $1555
line 1955
;1953:
;1954:		// track start of valid string
;1955:		if (*eptr && *eptr != ' ') {
ADDRLP4 1328
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1328
INDIRI4
CNSTI4 0
EQI4 $1559
ADDRLP4 1328
INDIRI4
CNSTI4 32
EQI4 $1559
line 1956
;1956:			lines[numLines++] = eptr;
ADDRLP4 1332
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 12
ADDRLP4 1332
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1332
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1957
;1957:		}
ADDRGP4 $1559
JUMPV
LABELV $1558
line 1960
;1958:
;1959:		while ( *eptr && *eptr != ' ' )
;1960:			eptr++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $1559
line 1959
ADDRLP4 1332
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1332
INDIRI4
CNSTI4 0
EQI4 $1561
ADDRLP4 1332
INDIRI4
CNSTI4 32
NEI4 $1558
LABELV $1561
line 1961
;1961:	}
LABELV $1550
line 1949
ADDRLP4 1320
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRI4
CVIF4 4
ADDRLP4 1320
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 1320
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
GEF4 $1562
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1553
LABELV $1562
line 1963
;1962:
;1963:	i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $1564
JUMPV
LABELV $1563
line 1964
;1964:	while (i < numLines) {
line 1965
;1965:		Text_Paint(rect->x + 2, y, scale, color, lines[i++], 0, 20, textStyle);
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1073741824
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 1324
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 1324
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1324
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 20
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1966
;1966:		if (i < numLines) {
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
GEI4 $1566
line 1967
;1967:			Text_Paint(rect->x + rect->w / 2, y, scale, color, lines[i++], 0, 20, textStyle);
ADDRLP4 1332
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1332
INDIRP4
INDIRF4
ADDRLP4 1332
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1073741824
DIVF4
ADDF4
ARGF4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 1328
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 1328
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1328
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
ADDP4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 20
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 1968
;1968:		}
LABELV $1566
line 1969
;1969:		y += 10;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 10
ADDI4
ASGNI4
line 1970
;1970:		if (y > rect->y + rect->h - 11) {
ADDRLP4 1328
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRI4
CVIF4 4
ADDRLP4 1328
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 1328
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1093664768
SUBF4
LEF4 $1568
line 1971
;1971:			break;
ADDRGP4 $1565
JUMPV
LABELV $1568
line 1973
;1972:		}
;1973:	}
LABELV $1564
line 1964
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LTI4 $1563
LABELV $1565
line 1976
;1974:
;1975:
;1976:}
LABELV $1532
endproc UI_DrawGLInfo 1336 32
proc UI_OwnerDraw 24 24
line 1980
;1977:
;1978:// FIXME: table drive
;1979://
;1980:static void UI_OwnerDraw(float x, float y, float w, float h, float text_x, float text_y, int ownerDraw, int ownerDrawFlags, int align, float special, float scale, vec4_t color, qhandle_t shader, int textStyle) {
line 1983
;1981:	rectDef_t rect;
;1982:
;1983:  rect.x = x + text_x;
ADDRLP4 0
ADDRFP4 0
INDIRF4
ADDRFP4 16
INDIRF4
ADDF4
ASGNF4
line 1984
;1984:  rect.y = y + text_y;
ADDRLP4 0+4
ADDRFP4 4
INDIRF4
ADDRFP4 20
INDIRF4
ADDF4
ASGNF4
line 1985
;1985:  rect.w = w;
ADDRLP4 0+8
ADDRFP4 8
INDIRF4
ASGNF4
line 1986
;1986:  rect.h = h;
ADDRLP4 0+12
ADDRFP4 12
INDIRF4
ASGNF4
line 1988
;1987:
;1988:  switch (ownerDraw) {
ADDRLP4 16
ADDRFP4 24
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 200
LTI4 $1575
ADDRLP4 16
INDIRI4
CNSTI4 256
GTI4 $1575
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1624-800
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1624
address $1576
address $1577
address $1578
address $1579
address $1580
address $1583
address $1586
address $1590
address $1591
address $1592
address $1593
address $1593
address $1593
address $1593
address $1593
address $1594
address $1594
address $1594
address $1594
address $1594
address $1595
address $1596
address $1598
address $1599
address $1600
address $1601
address $1602
address $1603
address $1604
address $1607
address $1605
address $1608
address $1606
address $1609
address $1610
address $1611
address $1612
address $1614
address $1575
address $1615
address $1616
address $1617
address $1618
address $1619
address $1588
address $1584
address $1597
address $1620
address $1621
address $1622
address $1623
address $1581
address $1587
address $1585
address $1582
address $1589
address $1613
code
LABELV $1576
line 1990
;1989:    case UI_HANDICAP:
;1990:      UI_DrawHandicap(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandicap
CALLV
pop
line 1991
;1991:      break;
ADDRGP4 $1575
JUMPV
LABELV $1577
line 1993
;1992:    case UI_EFFECTS:
;1993:      UI_DrawEffects(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawEffects
CALLV
pop
line 1994
;1994:      break;
ADDRGP4 $1575
JUMPV
LABELV $1578
line 1996
;1995:    case UI_PLAYERMODEL:
;1996:      UI_DrawPlayerModel(&rect);
ADDRLP4 0
ARGP4
ADDRGP4 UI_DrawPlayerModel
CALLV
pop
line 1997
;1997:      break;
ADDRGP4 $1575
JUMPV
LABELV $1579
line 1999
;1998:    case UI_CLANNAME:
;1999:      UI_DrawClanName(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawClanName
CALLV
pop
line 2000
;2000:      break;
ADDRGP4 $1575
JUMPV
LABELV $1580
line 2002
;2001:    case UI_CLANLOGO:
;2002:      UI_DrawClanLogo(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawClanLogo
CALLV
pop
line 2003
;2003:      break;
ADDRGP4 $1575
JUMPV
LABELV $1581
line 2005
;2004:    case UI_CLANCINEMATIC:
;2005:      UI_DrawClanCinematic(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawClanCinematic
CALLV
pop
line 2006
;2006:      break;
ADDRGP4 $1575
JUMPV
LABELV $1582
line 2008
;2007:    case UI_PREVIEWCINEMATIC:
;2008:      UI_DrawPreviewCinematic(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawPreviewCinematic
CALLV
pop
line 2009
;2009:      break;
ADDRGP4 $1575
JUMPV
LABELV $1583
line 2011
;2010:    case UI_GAMETYPE:
;2011:      UI_DrawGameType(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawGameType
CALLV
pop
line 2012
;2012:      break;
ADDRGP4 $1575
JUMPV
LABELV $1584
line 2014
;2013:    case UI_NETGAMETYPE:
;2014:      UI_DrawNetGameType(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawNetGameType
CALLV
pop
line 2015
;2015:      break;
ADDRGP4 $1575
JUMPV
LABELV $1585
line 2017
;2016:    case UI_JOINGAMETYPE:
;2017:	  UI_DrawJoinGameType(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawJoinGameType
CALLV
pop
line 2018
;2018:	  break;
ADDRGP4 $1575
JUMPV
LABELV $1586
line 2020
;2019:    case UI_MAPPREVIEW:
;2020:      UI_DrawMapPreview(&rect, scale, color, qtrue);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_DrawMapPreview
CALLV
pop
line 2021
;2021:      break;
ADDRGP4 $1575
JUMPV
LABELV $1587
line 2023
;2022:    case UI_MAP_TIMETOBEAT:
;2023:      UI_DrawMapTimeToBeat(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawMapTimeToBeat
CALLV
pop
line 2024
;2024:      break;
ADDRGP4 $1575
JUMPV
LABELV $1588
line 2026
;2025:    case UI_MAPCINEMATIC:
;2026:      UI_DrawMapCinematic(&rect, scale, color, qfalse);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_DrawMapCinematic
CALLV
pop
line 2027
;2027:      break;
ADDRGP4 $1575
JUMPV
LABELV $1589
line 2029
;2028:    case UI_STARTMAPCINEMATIC:
;2029:      UI_DrawMapCinematic(&rect, scale, color, qtrue);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_DrawMapCinematic
CALLV
pop
line 2030
;2030:      break;
ADDRGP4 $1575
JUMPV
LABELV $1590
line 2032
;2031:    case UI_SKILL:
;2032:      UI_DrawSkill(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawSkill
CALLV
pop
line 2033
;2033:      break;
ADDRGP4 $1575
JUMPV
LABELV $1591
line 2035
;2034:    case UI_BLUETEAMNAME:
;2035:      UI_DrawTeamName(&rect, scale, color, qtrue, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTeamName
CALLV
pop
line 2036
;2036:      break;
ADDRGP4 $1575
JUMPV
LABELV $1592
line 2038
;2037:    case UI_REDTEAMNAME:
;2038:      UI_DrawTeamName(&rect, scale, color, qfalse, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTeamName
CALLV
pop
line 2039
;2039:      break;
ADDRGP4 $1575
JUMPV
LABELV $1593
line 2045
;2040:    case UI_BLUETEAM1:
;2041:		case UI_BLUETEAM2:
;2042:		case UI_BLUETEAM3:
;2043:		case UI_BLUETEAM4:
;2044:		case UI_BLUETEAM5:
;2045:      UI_DrawTeamMember(&rect, scale, color, qtrue, ownerDraw - UI_BLUETEAM1 + 1, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
CNSTI4 210
SUBI4
ADDRLP4 20
INDIRI4
ADDI4
ARGI4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTeamMember
CALLV
pop
line 2046
;2046:      break;
ADDRGP4 $1575
JUMPV
LABELV $1594
line 2052
;2047:    case UI_REDTEAM1:
;2048:		case UI_REDTEAM2:
;2049:		case UI_REDTEAM3:
;2050:		case UI_REDTEAM4:
;2051:		case UI_REDTEAM5:
;2052:      UI_DrawTeamMember(&rect, scale, color, qfalse, ownerDraw - UI_REDTEAM1 + 1, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 24
INDIRI4
CNSTI4 215
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTeamMember
CALLV
pop
line 2053
;2053:      break;
ADDRGP4 $1575
JUMPV
LABELV $1595
line 2055
;2054:		case UI_NETSOURCE:
;2055:      UI_DrawNetSource(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawNetSource
CALLV
pop
line 2056
;2056:			break;
ADDRGP4 $1575
JUMPV
LABELV $1596
line 2058
;2057:    case UI_NETMAPPREVIEW:
;2058:      UI_DrawNetMapPreview(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawNetMapPreview
CALLV
pop
line 2059
;2059:      break;
ADDRGP4 $1575
JUMPV
LABELV $1597
line 2061
;2060:    case UI_NETMAPCINEMATIC:
;2061:      UI_DrawNetMapCinematic(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawNetMapCinematic
CALLV
pop
line 2062
;2062:      break;
ADDRGP4 $1575
JUMPV
LABELV $1598
line 2064
;2063:		case UI_NETFILTER:
;2064:      UI_DrawNetFilter(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawNetFilter
CALLV
pop
line 2065
;2065:			break;
ADDRGP4 $1575
JUMPV
LABELV $1599
line 2067
;2066:		case UI_TIER:
;2067:			UI_DrawTier(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTier
CALLV
pop
line 2068
;2068:			break;
ADDRGP4 $1575
JUMPV
LABELV $1600
line 2070
;2069:		case UI_OPPONENTMODEL:
;2070:			UI_DrawOpponent(&rect);
ADDRLP4 0
ARGP4
ADDRGP4 UI_DrawOpponent
CALLV
pop
line 2071
;2071:			break;
ADDRGP4 $1575
JUMPV
LABELV $1601
line 2073
;2072:		case UI_TIERMAP1:
;2073:			UI_DrawTierMap(&rect, 0);
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_DrawTierMap
CALLV
pop
line 2074
;2074:			break;
ADDRGP4 $1575
JUMPV
LABELV $1602
line 2076
;2075:		case UI_TIERMAP2:
;2076:			UI_DrawTierMap(&rect, 1);
ADDRLP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_DrawTierMap
CALLV
pop
line 2077
;2077:			break;
ADDRGP4 $1575
JUMPV
LABELV $1603
line 2079
;2078:		case UI_TIERMAP3:
;2079:			UI_DrawTierMap(&rect, 2);
ADDRLP4 0
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 UI_DrawTierMap
CALLV
pop
line 2080
;2080:			break;
ADDRGP4 $1575
JUMPV
LABELV $1604
line 2082
;2081:		case UI_PLAYERLOGO:
;2082:			UI_DrawPlayerLogo(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawPlayerLogo
CALLV
pop
line 2083
;2083:			break;
ADDRGP4 $1575
JUMPV
LABELV $1605
line 2085
;2084:		case UI_PLAYERLOGO_METAL:
;2085:			UI_DrawPlayerLogoMetal(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawPlayerLogoMetal
CALLV
pop
line 2086
;2086:			break;
ADDRGP4 $1575
JUMPV
LABELV $1606
line 2088
;2087:		case UI_PLAYERLOGO_NAME:
;2088:			UI_DrawPlayerLogoName(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawPlayerLogoName
CALLV
pop
line 2089
;2089:			break;
ADDRGP4 $1575
JUMPV
LABELV $1607
line 2091
;2090:		case UI_OPPONENTLOGO:
;2091:			UI_DrawOpponentLogo(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawOpponentLogo
CALLV
pop
line 2092
;2092:			break;
ADDRGP4 $1575
JUMPV
LABELV $1608
line 2094
;2093:		case UI_OPPONENTLOGO_METAL:
;2094:			UI_DrawOpponentLogoMetal(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawOpponentLogoMetal
CALLV
pop
line 2095
;2095:			break;
ADDRGP4 $1575
JUMPV
LABELV $1609
line 2097
;2096:		case UI_OPPONENTLOGO_NAME:
;2097:			UI_DrawOpponentLogoName(&rect, color);
ADDRLP4 0
ARGP4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawOpponentLogoName
CALLV
pop
line 2098
;2098:			break;
ADDRGP4 $1575
JUMPV
LABELV $1610
line 2100
;2099:		case UI_TIER_MAPNAME:
;2100:			UI_DrawTierMapName(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTierMapName
CALLV
pop
line 2101
;2101:			break;
ADDRGP4 $1575
JUMPV
LABELV $1611
line 2103
;2102:		case UI_TIER_GAMETYPE:
;2103:			UI_DrawTierGameType(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawTierGameType
CALLV
pop
line 2104
;2104:			break;
ADDRGP4 $1575
JUMPV
LABELV $1612
line 2106
;2105:		case UI_ALLMAPS_SELECTION:
;2106:			UI_DrawAllMapsSelection(&rect, scale, color, textStyle, qtrue);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 UI_DrawAllMapsSelection
CALLV
pop
line 2107
;2107:			break;
ADDRGP4 $1575
JUMPV
LABELV $1613
line 2109
;2108:		case UI_MAPS_SELECTION:
;2109:			UI_DrawAllMapsSelection(&rect, scale, color, textStyle, qfalse);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 UI_DrawAllMapsSelection
CALLV
pop
line 2110
;2110:			break;
ADDRGP4 $1575
JUMPV
LABELV $1614
line 2112
;2111:		case UI_OPPONENT_NAME:
;2112:			UI_DrawOpponentName(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawOpponentName
CALLV
pop
line 2113
;2113:			break;
ADDRGP4 $1575
JUMPV
LABELV $1615
line 2115
;2114:		case UI_BOTNAME:
;2115:			UI_DrawBotName(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawBotName
CALLV
pop
line 2116
;2116:			break;
ADDRGP4 $1575
JUMPV
LABELV $1616
line 2118
;2117:		case UI_BOTSKILL:
;2118:			UI_DrawBotSkill(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawBotSkill
CALLV
pop
line 2119
;2119:			break;
ADDRGP4 $1575
JUMPV
LABELV $1617
line 2121
;2120:		case UI_REDBLUE:
;2121:			UI_DrawRedBlue(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawRedBlue
CALLV
pop
line 2122
;2122:			break;
ADDRGP4 $1575
JUMPV
LABELV $1618
line 2124
;2123:		case UI_CROSSHAIR:
;2124:			UI_DrawCrosshair(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawCrosshair
CALLV
pop
line 2125
;2125:			break;
ADDRGP4 $1575
JUMPV
LABELV $1619
line 2127
;2126:		case UI_SELECTEDPLAYER:
;2127:			UI_DrawSelectedPlayer(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawSelectedPlayer
CALLV
pop
line 2128
;2128:			break;
ADDRGP4 $1575
JUMPV
LABELV $1620
line 2130
;2129:		case UI_SERVERREFRESHDATE:
;2130:			UI_DrawServerRefreshDate(&rect, scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawServerRefreshDate
CALLV
pop
line 2131
;2131:			break;
ADDRGP4 $1575
JUMPV
LABELV $1621
line 2133
;2132:		case UI_SERVERMOTD:
;2133:			UI_DrawServerMOTD(&rect, scale, color);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRGP4 UI_DrawServerMOTD
CALLV
pop
line 2134
;2134:			break;
ADDRGP4 $1575
JUMPV
LABELV $1622
line 2136
;2135:		case UI_GLINFO:
;2136:			UI_DrawGLInfo(&rect,scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawGLInfo
CALLV
pop
line 2137
;2137:			break;
ADDRGP4 $1575
JUMPV
LABELV $1623
line 2139
;2138:		case UI_KEYBINDSTATUS:
;2139:			UI_DrawKeyBindStatus(&rect,scale, color, textStyle);
ADDRLP4 0
ARGP4
ADDRFP4 40
INDIRF4
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 52
INDIRI4
ARGI4
ADDRGP4 UI_DrawKeyBindStatus
CALLV
pop
line 2140
;2140:			break;
line 2142
;2141:    default:
;2142:      break;
LABELV $1575
line 2145
;2143:  }
;2144:
;2145:}
LABELV $1570
endproc UI_OwnerDraw 24 24
proc UI_OwnerDrawVisible 8 8
line 2147
;2146:
;2147:static qboolean UI_OwnerDrawVisible(int flags) {
line 2148
;2148:	qboolean vis = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $1628
JUMPV
LABELV $1627
line 2150
;2149:
;2150:	while (flags) {
line 2152
;2151:
;2152:		if (flags & UI_SHOW_FFA) {
ADDRFP4 0
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $1630
line 2153
;2153:			if (trap_Cvar_VariableValue("g_gametype") != GT_FFA) {
ADDRGP4 $1323
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
INDIRF4
CNSTF4 0
EQF4 $1632
line 2154
;2154:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2155
;2155:			}
LABELV $1632
line 2156
;2156:			flags &= ~UI_SHOW_FFA;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -257
BANDI4
ASGNI4
line 2157
;2157:		}
LABELV $1630
line 2159
;2158:
;2159:		if (flags & UI_SHOW_NOTFFA) {
ADDRFP4 0
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $1634
line 2160
;2160:			if (trap_Cvar_VariableValue("g_gametype") == GT_FFA) {
ADDRGP4 $1323
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $1636
line 2161
;2161:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2162
;2162:			}
LABELV $1636
line 2163
;2163:			flags &= ~UI_SHOW_NOTFFA;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -513
BANDI4
ASGNI4
line 2164
;2164:		}
LABELV $1634
line 2166
;2165:
;2166:		if (flags & UI_SHOW_LEADER) {
ADDRFP4 0
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $1638
line 2168
;2167:			// these need to show when this client can give orders to a player or a group
;2168:			if (!uiInfo.teamLeader) {
ADDRGP4 uiInfo+78868
INDIRI4
CNSTI4 0
NEI4 $1640
line 2169
;2169:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2170
;2170:			} else {
ADDRGP4 $1641
JUMPV
LABELV $1640
line 2172
;2171:				// if showing yourself
;2172:				if (ui_selectedPlayer.integer < uiInfo.myTeamCount && uiInfo.teamClientNums[ui_selectedPlayer.integer] == uiInfo.playerNumber) { 
ADDRGP4 ui_selectedPlayer+12
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $1643
ADDRGP4 ui_selectedPlayer+12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+82968
ADDP4
INDIRI4
ADDRGP4 uiInfo+78864
INDIRI4
NEI4 $1643
line 2173
;2173:					vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2174
;2174:				}
LABELV $1643
line 2175
;2175:			}
LABELV $1641
line 2176
;2176:			flags &= ~UI_SHOW_LEADER;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 2177
;2177:		} 
LABELV $1638
line 2178
;2178:		if (flags & UI_SHOW_NOTLEADER) {
ADDRFP4 0
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1650
line 2180
;2179:			// these need to show when this client is assigning their own status or they are NOT the leader
;2180:			if (uiInfo.teamLeader) {
ADDRGP4 uiInfo+78868
INDIRI4
CNSTI4 0
EQI4 $1652
line 2182
;2181:				// if not showing yourself
;2182:				if (!(ui_selectedPlayer.integer < uiInfo.myTeamCount && uiInfo.teamClientNums[ui_selectedPlayer.integer] == uiInfo.playerNumber)) { 
ADDRGP4 ui_selectedPlayer+12
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $1662
ADDRGP4 ui_selectedPlayer+12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+82968
ADDP4
INDIRI4
ADDRGP4 uiInfo+78864
INDIRI4
EQI4 $1655
LABELV $1662
line 2183
;2183:					vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2184
;2184:				}
LABELV $1655
line 2186
;2185:				// these need to show when this client can give orders to a player or a group
;2186:			}
LABELV $1652
line 2187
;2187:			flags &= ~UI_SHOW_NOTLEADER;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 2188
;2188:		} 
LABELV $1650
line 2189
;2189:		if (flags & UI_SHOW_FAVORITESERVERS) {
ADDRFP4 0
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $1663
line 2191
;2190:			// this assumes you only put this type of display flag on something showing in the proper context
;2191:			if (ui_netSource.integer != AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
EQI4 $1665
line 2192
;2192:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2193
;2193:			}
LABELV $1665
line 2194
;2194:			flags &= ~UI_SHOW_FAVORITESERVERS;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -5
BANDI4
ASGNI4
line 2195
;2195:		} 
LABELV $1663
line 2196
;2196:		if (flags & UI_SHOW_NOTFAVORITESERVERS) {
ADDRFP4 0
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $1668
line 2198
;2197:			// this assumes you only put this type of display flag on something showing in the proper context
;2198:			if (ui_netSource.integer == AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
NEI4 $1670
line 2199
;2199:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2200
;2200:			}
LABELV $1670
line 2201
;2201:			flags &= ~UI_SHOW_NOTFAVORITESERVERS;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -4097
BANDI4
ASGNI4
line 2202
;2202:		} 
LABELV $1668
line 2203
;2203:		if (flags & UI_SHOW_ANYTEAMGAME) {
ADDRFP4 0
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $1673
line 2204
;2204:			if (uiInfo.gameTypes[ui_gameType.integer].gtEnum <= GT_TEAM ) {
ADDRLP4 4
CNSTI4 3
ASGNI4
ADDRGP4 ui_gameType+12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
GTI4 $1675
line 2205
;2205:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2206
;2206:			}
LABELV $1675
line 2207
;2207:			flags &= ~UI_SHOW_ANYTEAMGAME;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -17
BANDI4
ASGNI4
line 2208
;2208:		} 
LABELV $1673
line 2209
;2209:		if (flags & UI_SHOW_ANYNONTEAMGAME) {
ADDRFP4 0
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $1680
line 2210
;2210:			if (uiInfo.gameTypes[ui_gameType.integer].gtEnum > GT_TEAM ) {
ADDRLP4 4
CNSTI4 3
ASGNI4
ADDRGP4 ui_gameType+12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $1682
line 2211
;2211:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2212
;2212:			}
LABELV $1682
line 2213
;2213:			flags &= ~UI_SHOW_ANYNONTEAMGAME;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -9
BANDI4
ASGNI4
line 2214
;2214:		} 
LABELV $1680
line 2215
;2215:		if (flags & UI_SHOW_NETANYTEAMGAME) {
ADDRFP4 0
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $1687
line 2216
;2216:			if (uiInfo.gameTypes[ui_netGameType.integer].gtEnum <= GT_TEAM ) {
ADDRLP4 4
CNSTI4 3
ASGNI4
ADDRGP4 ui_netGameType+12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
GTI4 $1689
line 2217
;2217:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2218
;2218:			}
LABELV $1689
line 2219
;2219:			flags &= ~UI_SHOW_NETANYTEAMGAME;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
line 2220
;2220:		} 
LABELV $1687
line 2221
;2221:		if (flags & UI_SHOW_NETANYNONTEAMGAME) {
ADDRFP4 0
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1694
line 2222
;2222:			if (uiInfo.gameTypes[ui_netGameType.integer].gtEnum > GT_TEAM ) {
ADDRLP4 4
CNSTI4 3
ASGNI4
ADDRGP4 ui_netGameType+12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $1696
line 2223
;2223:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2224
;2224:			}
LABELV $1696
line 2225
;2225:			flags &= ~UI_SHOW_NETANYNONTEAMGAME;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -1025
BANDI4
ASGNI4
line 2226
;2226:		} 
LABELV $1694
line 2227
;2227:		if (flags & UI_SHOW_NEWHIGHSCORE) {
ADDRFP4 0
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
EQI4 $1701
line 2228
;2228:			if (uiInfo.newHighScoreTime < uiInfo.uiDC.realTime) {
ADDRGP4 uiInfo+73416
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
GEI4 $1703
line 2229
;2229:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2230
;2230:			} else {
ADDRGP4 $1704
JUMPV
LABELV $1703
line 2231
;2231:				if (uiInfo.soundHighScore) {
ADDRGP4 uiInfo+73436
INDIRI4
CNSTI4 0
EQI4 $1707
line 2232
;2232:					if (trap_Cvar_VariableValue("sv_killserver") == 0) {
ADDRGP4 $394
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $1710
line 2234
;2233:						// wait on server to go down before playing sound
;2234:						trap_S_StartLocalSound(uiInfo.newHighScoreSound, CHAN_ANNOUNCER);
ADDRGP4 uiInfo+119484
INDIRI4
ARGI4
CNSTI4 7
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
line 2235
;2235:						uiInfo.soundHighScore = qfalse;
ADDRGP4 uiInfo+73436
CNSTI4 0
ASGNI4
line 2236
;2236:					}
LABELV $1710
line 2237
;2237:				}
LABELV $1707
line 2238
;2238:			}
LABELV $1704
line 2239
;2239:			flags &= ~UI_SHOW_NEWHIGHSCORE;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -33
BANDI4
ASGNI4
line 2240
;2240:		} 
LABELV $1701
line 2241
;2241:		if (flags & UI_SHOW_NEWBESTTIME) {
ADDRFP4 0
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $1714
line 2242
;2242:			if (uiInfo.newBestTime < uiInfo.uiDC.realTime) {
ADDRGP4 uiInfo+73420
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
GEI4 $1716
line 2243
;2243:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2244
;2244:			}
LABELV $1716
line 2245
;2245:			flags &= ~UI_SHOW_NEWBESTTIME;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 2246
;2246:		} 
LABELV $1714
line 2247
;2247:		if (flags & UI_SHOW_DEMOAVAILABLE) {
ADDRFP4 0
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
EQI4 $1720
line 2248
;2248:			if (!uiInfo.demoAvailable) {
ADDRGP4 uiInfo+73432
INDIRI4
CNSTI4 0
NEI4 $1722
line 2249
;2249:				vis = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2250
;2250:			}
LABELV $1722
line 2251
;2251:			flags &= ~UI_SHOW_DEMOAVAILABLE;
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 -65
BANDI4
ASGNI4
line 2252
;2252:		} else {
ADDRGP4 $1721
JUMPV
LABELV $1720
line 2253
;2253:			flags = 0;
ADDRFP4 0
CNSTI4 0
ASGNI4
line 2254
;2254:		}
LABELV $1721
line 2255
;2255:	}
LABELV $1628
line 2150
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $1627
line 2256
;2256:  return vis;
ADDRLP4 0
INDIRI4
RETI4
LABELV $1626
endproc UI_OwnerDrawVisible 8 8
proc UI_Handicap_HandleKey 20 12
line 2259
;2257:}
;2258:
;2259:static qboolean UI_Handicap_HandleKey(int flags, float *special, int key) {
line 2260
;2260:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1730
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1730
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1730
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1726
LABELV $1730
line 2262
;2261:    int h;
;2262:    h = Com_Clamp( 5, 100, trap_Cvar_VariableValue("handicap") );
ADDRGP4 $696
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 1084227584
ARGF4
CNSTF4 1120403456
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 2263
;2263:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1731
line 2264
;2264:	    h -= 5;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 5
SUBI4
ASGNI4
line 2265
;2265:		} else {
ADDRGP4 $1732
JUMPV
LABELV $1731
line 2266
;2266:	    h += 5;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 5
ADDI4
ASGNI4
line 2267
;2267:		}
LABELV $1732
line 2268
;2268:    if (h > 100) {
ADDRLP4 4
INDIRI4
CNSTI4 100
LEI4 $1733
line 2269
;2269:      h = 5;
ADDRLP4 4
CNSTI4 5
ASGNI4
line 2270
;2270:    } else if (h < 0) {
ADDRGP4 $1734
JUMPV
LABELV $1733
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1735
line 2271
;2271:			h = 100;
ADDRLP4 4
CNSTI4 100
ASGNI4
line 2272
;2272:		}
LABELV $1735
LABELV $1734
line 2273
;2273:  	trap_Cvar_Set( "handicap", va( "%i", h) );
ADDRGP4 $1737
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $696
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2274
;2274:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1725
JUMPV
LABELV $1726
line 2276
;2275:  }
;2276:  return qfalse;
CNSTI4 0
RETI4
LABELV $1725
endproc UI_Handicap_HandleKey 20 12
proc UI_Effects_HandleKey 8 8
line 2279
;2277:}
;2278:
;2279:static qboolean UI_Effects_HandleKey(int flags, float *special, int key) {
line 2280
;2280:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1743
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1743
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1743
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1739
LABELV $1743
line 2282
;2281:
;2282:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1744
line 2283
;2283:	    uiInfo.effectsColor--;
ADDRLP4 4
ADDRGP4 uiInfo+136904
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2284
;2284:		} else {
ADDRGP4 $1745
JUMPV
LABELV $1744
line 2285
;2285:	    uiInfo.effectsColor++;
ADDRLP4 4
ADDRGP4 uiInfo+136904
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2286
;2286:		}
LABELV $1745
line 2288
;2287:
;2288:    if( uiInfo.effectsColor > 6 ) {
ADDRGP4 uiInfo+136904
INDIRI4
CNSTI4 6
LEI4 $1748
line 2289
;2289:	  	uiInfo.effectsColor = 0;
ADDRGP4 uiInfo+136904
CNSTI4 0
ASGNI4
line 2290
;2290:		} else if (uiInfo.effectsColor < 0) {
ADDRGP4 $1749
JUMPV
LABELV $1748
ADDRGP4 uiInfo+136904
INDIRI4
CNSTI4 0
GEI4 $1752
line 2291
;2291:	  	uiInfo.effectsColor = 6;
ADDRGP4 uiInfo+136904
CNSTI4 6
ASGNI4
line 2292
;2292:		}
LABELV $1752
LABELV $1749
line 2294
;2293:
;2294:	  trap_Cvar_SetValue( "color1", uitogamecode[uiInfo.effectsColor] );
ADDRGP4 $1756
ARGP4
ADDRGP4 uiInfo+136904
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uitogamecode
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 2295
;2295:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1738
JUMPV
LABELV $1739
line 2297
;2296:  }
;2297:  return qfalse;
CNSTI4 0
RETI4
LABELV $1738
endproc UI_Effects_HandleKey 8 8
proc UI_ClanName_HandleKey 16 8
line 2300
;2298:}
;2299:
;2300:static qboolean UI_ClanName_HandleKey(int flags, float *special, int key) {
line 2301
;2301:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1763
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1763
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1763
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1759
LABELV $1763
line 2303
;2302:    int i;
;2303:    i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
line 2304
;2304:		if (uiInfo.teamList[i].cinematic >= 0) {
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
CNSTI4 0
LTI4 $1764
line 2305
;2305:		  trap_CIN_StopCinematic(uiInfo.teamList[i].cinematic);
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 2306
;2306:			uiInfo.teamList[i].cinematic = -1;
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
CNSTI4 -1
ASGNI4
line 2307
;2307:		}
LABELV $1764
line 2308
;2308:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1772
line 2309
;2309:	    i--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2310
;2310:		} else {
ADDRGP4 $1773
JUMPV
LABELV $1772
line 2311
;2311:	    i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2312
;2312:		}
LABELV $1773
line 2313
;2313:    if (i >= uiInfo.teamCount) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $1774
line 2314
;2314:      i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2315
;2315:    } else if (i < 0) {
ADDRGP4 $1775
JUMPV
LABELV $1774
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1777
line 2316
;2316:			i = uiInfo.teamCount - 1;
ADDRLP4 4
ADDRGP4 uiInfo+75756
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2317
;2317:		}
LABELV $1777
LABELV $1775
line 2318
;2318:  	trap_Cvar_Set( "ui_teamName", uiInfo.teamList[i].teamName);
ADDRGP4 $698
ARGP4
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2319
;2319:	UI_HeadCountByTeam();
ADDRGP4 UI_HeadCountByTeam
CALLI4
pop
line 2320
;2320:	UI_FeederSelection(FEEDER_HEADS, 0);
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 UI_FeederSelection
CALLV
pop
line 2321
;2321:	updateModel = qtrue;
ADDRGP4 updateModel
CNSTI4 1
ASGNI4
line 2322
;2322:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1758
JUMPV
LABELV $1759
line 2324
;2323:  }
;2324:  return qfalse;
CNSTI4 0
RETI4
LABELV $1758
endproc UI_ClanName_HandleKey 16 8
proc UI_GameType_HandleKey 20 16
line 2327
;2325:}
;2326:
;2327:static qboolean UI_GameType_HandleKey(int flags, float *special, int key, qboolean resetMap) {
line 2328
;2328:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1786
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1786
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1786
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1782
LABELV $1786
line 2329
;2329:		int oldCount = UI_MapCountByGameType(qtrue);
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 UI_MapCountByGameType
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 2332
;2330:
;2331:		// hard coded mess here
;2332:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1787
line 2333
;2333:			ui_gameType.integer--;
ADDRLP4 12
ADDRGP4 ui_gameType+12
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2334
;2334:			if (ui_gameType.integer == 2) {
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 2
NEI4 $1790
line 2335
;2335:				ui_gameType.integer = 1;
ADDRGP4 ui_gameType+12
CNSTI4 1
ASGNI4
line 2336
;2336:			} else if (ui_gameType.integer < 2) {
ADDRGP4 $1788
JUMPV
LABELV $1790
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 2
GEI4 $1788
line 2337
;2337:				ui_gameType.integer = uiInfo.numGameTypes - 1;
ADDRGP4 ui_gameType+12
ADDRGP4 uiInfo+78576
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2338
;2338:			}
line 2339
;2339:		} else {
ADDRGP4 $1788
JUMPV
LABELV $1787
line 2340
;2340:			ui_gameType.integer++;
ADDRLP4 12
ADDRGP4 ui_gameType+12
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2341
;2341:			if (ui_gameType.integer >= uiInfo.numGameTypes) {
ADDRGP4 ui_gameType+12
INDIRI4
ADDRGP4 uiInfo+78576
INDIRI4
LTI4 $1800
line 2342
;2342:				ui_gameType.integer = 1;
ADDRGP4 ui_gameType+12
CNSTI4 1
ASGNI4
line 2343
;2343:			} else if (ui_gameType.integer == 2) {
ADDRGP4 $1801
JUMPV
LABELV $1800
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 2
NEI4 $1805
line 2344
;2344:				ui_gameType.integer = 3;
ADDRGP4 ui_gameType+12
CNSTI4 3
ASGNI4
line 2345
;2345:			}
LABELV $1805
LABELV $1801
line 2346
;2346:		}
LABELV $1788
line 2348
;2347:    
;2348:		if (uiInfo.gameTypes[ui_gameType.integer].gtEnum == GT_TOURNAMENT) {
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1809
line 2349
;2349:			trap_Cvar_Set("ui_Q3Model", "1");
ADDRGP4 $961
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2350
;2350:		} else {
ADDRGP4 $1810
JUMPV
LABELV $1809
line 2351
;2351:			trap_Cvar_Set("ui_Q3Model", "0");
ADDRGP4 $961
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2352
;2352:		}
LABELV $1810
line 2354
;2353:
;2354:		trap_Cvar_Set("ui_gameType", va("%d", ui_gameType.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $1814
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2355
;2355:		UI_SetCapFragLimits(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_SetCapFragLimits
CALLV
pop
line 2356
;2356:		UI_LoadBestScores(uiInfo.mapList[ui_currentMap.integer].mapLoadName, uiInfo.gameTypes[ui_gameType.integer].gtEnum);
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_LoadBestScores
CALLV
pop
line 2357
;2357:		if (resetMap && oldCount != UI_MapCountByGameType(qtrue)) {
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $1822
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 UI_MapCountByGameType
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $1822
line 2358
;2358:	  	trap_Cvar_Set( "ui_currentMap", "0");
ADDRGP4 $888
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2359
;2359:			Menu_SetFeederSelection(NULL, FEEDER_MAPS, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 1
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 2360
;2360:		}
LABELV $1822
line 2361
;2361:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1781
JUMPV
LABELV $1782
line 2363
;2362:  }
;2363:  return qfalse;
CNSTI4 0
RETI4
LABELV $1781
endproc UI_GameType_HandleKey 20 16
proc UI_NetGameType_HandleKey 12 16
line 2366
;2364:}
;2365:
;2366:static qboolean UI_NetGameType_HandleKey(int flags, float *special, int key) {
line 2367
;2367:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1829
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1829
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1829
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1825
LABELV $1829
line 2369
;2368:
;2369:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1830
line 2370
;2370:			ui_netGameType.integer--;
ADDRLP4 4
ADDRGP4 ui_netGameType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2371
;2371:		} else {
ADDRGP4 $1831
JUMPV
LABELV $1830
line 2372
;2372:			ui_netGameType.integer++;
ADDRLP4 4
ADDRGP4 ui_netGameType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2373
;2373:		}
LABELV $1831
line 2375
;2374:
;2375:    if (ui_netGameType.integer < 0) {
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 0
GEI4 $1834
line 2376
;2376:      ui_netGameType.integer = uiInfo.numGameTypes - 1;
ADDRGP4 ui_netGameType+12
ADDRGP4 uiInfo+78576
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2377
;2377:		} else if (ui_netGameType.integer >= uiInfo.numGameTypes) {
ADDRGP4 $1835
JUMPV
LABELV $1834
ADDRGP4 ui_netGameType+12
INDIRI4
ADDRGP4 uiInfo+78576
INDIRI4
LTI4 $1839
line 2378
;2378:      ui_netGameType.integer = 0;
ADDRGP4 ui_netGameType+12
CNSTI4 0
ASGNI4
line 2379
;2379:    } 
LABELV $1839
LABELV $1835
line 2381
;2380:
;2381:  	trap_Cvar_Set( "ui_netGameType", va("%d", ui_netGameType.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_netGameType+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $727
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2382
;2382:  	trap_Cvar_Set( "ui_actualnetGameType", va("%d", uiInfo.gameTypes[ui_netGameType.integer].gtEnum));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $1845
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2383
;2383:  	trap_Cvar_Set( "ui_currentNetMap", "0");
ADDRGP4 $886
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2384
;2384:		UI_MapCountByGameType(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_MapCountByGameType
CALLI4
pop
line 2385
;2385:		Menu_SetFeederSelection(NULL, FEEDER_ALLMAPS, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 2386
;2386:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1824
JUMPV
LABELV $1825
line 2388
;2387:  }
;2388:  return qfalse;
CNSTI4 0
RETI4
LABELV $1824
endproc UI_NetGameType_HandleKey 12 16
proc UI_JoinGameType_HandleKey 8 8
line 2391
;2389:}
;2390:
;2391:static qboolean UI_JoinGameType_HandleKey(int flags, float *special, int key) {
line 2392
;2392:	if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1854
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1854
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1854
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1850
LABELV $1854
line 2394
;2393:
;2394:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1855
line 2395
;2395:			ui_joinGameType.integer--;
ADDRLP4 4
ADDRGP4 ui_joinGameType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2396
;2396:		} else {
ADDRGP4 $1856
JUMPV
LABELV $1855
line 2397
;2397:			ui_joinGameType.integer++;
ADDRLP4 4
ADDRGP4 ui_joinGameType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2398
;2398:		}
LABELV $1856
line 2400
;2399:
;2400:		if (ui_joinGameType.integer < 0) {
ADDRGP4 ui_joinGameType+12
INDIRI4
CNSTI4 0
GEI4 $1859
line 2401
;2401:			ui_joinGameType.integer = uiInfo.numJoinGameTypes - 1;
ADDRGP4 ui_joinGameType+12
ADDRGP4 uiInfo+78708
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2402
;2402:		} else if (ui_joinGameType.integer >= uiInfo.numJoinGameTypes) {
ADDRGP4 $1860
JUMPV
LABELV $1859
ADDRGP4 ui_joinGameType+12
INDIRI4
ADDRGP4 uiInfo+78708
INDIRI4
LTI4 $1864
line 2403
;2403:			ui_joinGameType.integer = 0;
ADDRGP4 ui_joinGameType+12
CNSTI4 0
ASGNI4
line 2404
;2404:		}
LABELV $1864
LABELV $1860
line 2406
;2405:
;2406:		trap_Cvar_Set( "ui_joinGameType", va("%d", ui_joinGameType.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_joinGameType+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $738
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2407
;2407:		UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 2408
;2408:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1849
JUMPV
LABELV $1850
line 2410
;2409:	}
;2410:	return qfalse;
CNSTI4 0
RETI4
LABELV $1849
endproc UI_JoinGameType_HandleKey 8 8
proc UI_Skill_HandleKey 16 8
line 2415
;2411:}
;2412:
;2413:
;2414:
;2415:static qboolean UI_Skill_HandleKey(int flags, float *special, int key) {
line 2416
;2416:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1875
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1875
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1875
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1871
LABELV $1875
line 2417
;2417:  	int i = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $824
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 2419
;2418:
;2419:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1876
line 2420
;2420:	    i--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2421
;2421:		} else {
ADDRGP4 $1877
JUMPV
LABELV $1876
line 2422
;2422:	    i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2423
;2423:		}
LABELV $1877
line 2425
;2424:
;2425:    if (i < 1) {
ADDRLP4 4
INDIRI4
CNSTI4 1
GEI4 $1878
line 2426
;2426:			i = numSkillLevels;
ADDRLP4 4
ADDRGP4 numSkillLevels
INDIRI4
ASGNI4
line 2427
;2427:		} else if (i > numSkillLevels) {
ADDRGP4 $1879
JUMPV
LABELV $1878
ADDRLP4 4
INDIRI4
ADDRGP4 numSkillLevels
INDIRI4
LEI4 $1880
line 2428
;2428:      i = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 2429
;2429:    }
LABELV $1880
LABELV $1879
line 2431
;2430:
;2431:    trap_Cvar_Set("g_spSkill", va("%i", i));
ADDRGP4 $1737
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $824
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2432
;2432:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1870
JUMPV
LABELV $1871
line 2434
;2433:  }
;2434:  return qfalse;
CNSTI4 0
RETI4
LABELV $1870
endproc UI_Skill_HandleKey 16 8
proc UI_TeamName_HandleKey 24 8
line 2437
;2435:}
;2436:
;2437:static qboolean UI_TeamName_HandleKey(int flags, float *special, int key, qboolean blue) {
line 2438
;2438:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1887
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1887
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1887
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1883
LABELV $1887
line 2440
;2439:    int i;
;2440:    i = UI_TeamIndexFromName(UI_Cvar_VariableString((blue) ? "ui_blueTeam" : "ui_redTeam"));
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $1889
ADDRLP4 8
ADDRGP4 $830
ASGNP4
ADDRGP4 $1890
JUMPV
LABELV $1889
ADDRLP4 8
ADDRGP4 $831
ASGNP4
LABELV $1890
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 2442
;2441:
;2442:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1891
line 2443
;2443:	    i--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2444
;2444:		} else {
ADDRGP4 $1892
JUMPV
LABELV $1891
line 2445
;2445:	    i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2446
;2446:		}
LABELV $1892
line 2448
;2447:
;2448:    if (i >= uiInfo.teamCount) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $1893
line 2449
;2449:      i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2450
;2450:    } else if (i < 0) {
ADDRGP4 $1894
JUMPV
LABELV $1893
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1896
line 2451
;2451:			i = uiInfo.teamCount - 1;
ADDRLP4 4
ADDRGP4 uiInfo+75756
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2452
;2452:		}
LABELV $1896
LABELV $1894
line 2454
;2453:
;2454:    trap_Cvar_Set( (blue) ? "ui_blueTeam" : "ui_redTeam", uiInfo.teamList[i].teamName);
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $1901
ADDRLP4 20
ADDRGP4 $830
ASGNP4
ADDRGP4 $1902
JUMPV
LABELV $1901
ADDRLP4 20
ADDRGP4 $831
ASGNP4
LABELV $1902
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2456
;2455:
;2456:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1882
JUMPV
LABELV $1883
line 2458
;2457:  }
;2458:  return qfalse;
CNSTI4 0
RETI4
LABELV $1882
endproc UI_TeamName_HandleKey 24 8
proc UI_TeamMember_HandleKey 32 8
line 2461
;2459:}
;2460:
;2461:static qboolean UI_TeamMember_HandleKey(int flags, float *special, int key, qboolean blue, int num) {
line 2462
;2462:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1908
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1908
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1908
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1904
LABELV $1908
line 2466
;2463:		// 0 - None
;2464:		// 1 - Human
;2465:		// 2..NumCharacters - Bot
;2466:		char *cvar = va(blue ? "ui_blueteam%i" : "ui_redteam%i", num);
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $1910
ADDRLP4 12
ADDRGP4 $846
ASGNP4
ADDRGP4 $1911
JUMPV
LABELV $1910
ADDRLP4 12
ADDRGP4 $847
ASGNP4
LABELV $1911
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 2467
;2467:		int value = trap_Cvar_VariableValue(cvar);
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 20
INDIRF4
CVFI4 4
ASGNI4
line 2469
;2468:
;2469:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1912
line 2470
;2470:			value--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2471
;2471:		} else {
ADDRGP4 $1913
JUMPV
LABELV $1912
line 2472
;2472:			value++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2473
;2473:		}
LABELV $1913
line 2475
;2474:
;2475:		if (ui_actualNetGameType.integer >= GT_TEAM) {
ADDRGP4 ui_actualNetGameType+12
INDIRI4
CNSTI4 3
LTI4 $1914
line 2476
;2476:			if (value >= uiInfo.characterCount + 2) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
CNSTI4 2
ADDI4
LTI4 $1917
line 2477
;2477:				value = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2478
;2478:			} else if (value < 0) {
ADDRGP4 $1915
JUMPV
LABELV $1917
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1915
line 2479
;2479:				value = uiInfo.characterCount + 2 - 1;
ADDRLP4 4
ADDRGP4 uiInfo+73440
INDIRI4
CNSTI4 2
ADDI4
CNSTI4 1
SUBI4
ASGNI4
line 2480
;2480:			}
line 2481
;2481:		} else {
ADDRGP4 $1915
JUMPV
LABELV $1914
line 2482
;2482:			if (value >= UI_GetNumBots() + 2) {
ADDRLP4 24
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
CNSTI4 2
ADDI4
LTI4 $1923
line 2483
;2483:				value = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2484
;2484:			} else if (value < 0) {
ADDRGP4 $1924
JUMPV
LABELV $1923
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1925
line 2485
;2485:				value = UI_GetNumBots() + 2 - 1;
ADDRLP4 28
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 28
INDIRI4
CNSTI4 2
ADDI4
CNSTI4 1
SUBI4
ASGNI4
line 2486
;2486:			}
LABELV $1925
LABELV $1924
line 2487
;2487:		}
LABELV $1915
line 2489
;2488:
;2489:		trap_Cvar_Set(cvar, va("%i", value));
ADDRGP4 $1737
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2490
;2490:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1903
JUMPV
LABELV $1904
line 2492
;2491:  }
;2492:  return qfalse;
CNSTI4 0
RETI4
LABELV $1903
endproc UI_TeamMember_HandleKey 32 8
proc UI_NetSource_HandleKey 12 8
line 2495
;2493:}
;2494:
;2495:static qboolean UI_NetSource_HandleKey(int flags, float *special, int key) {
line 2496
;2496:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1932
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1932
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1932
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1928
LABELV $1932
line 2498
;2497:		
;2498:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1933
line 2499
;2499:			ui_netSource.integer--;
ADDRLP4 4
ADDRGP4 ui_netSource+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2500
;2500:			if (ui_netSource.integer == AS_MPLAYER)
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 1
NEI4 $1934
line 2501
;2501:				ui_netSource.integer--;
ADDRLP4 8
ADDRGP4 ui_netSource+12
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2502
;2502:		} else {
ADDRGP4 $1934
JUMPV
LABELV $1933
line 2503
;2503:			ui_netSource.integer++;
ADDRLP4 4
ADDRGP4 ui_netSource+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2504
;2504:			if (ui_netSource.integer == AS_MPLAYER)
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 1
NEI4 $1941
line 2505
;2505:				ui_netSource.integer++;
ADDRLP4 8
ADDRGP4 ui_netSource+12
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1941
line 2506
;2506:		}
LABELV $1934
line 2508
;2507:    
;2508:		if (ui_netSource.integer >= numNetSources) {
ADDRGP4 ui_netSource+12
INDIRI4
ADDRGP4 numNetSources
INDIRI4
LTI4 $1945
line 2509
;2509:      ui_netSource.integer = 0;
ADDRGP4 ui_netSource+12
CNSTI4 0
ASGNI4
line 2510
;2510:    } else if (ui_netSource.integer < 0) {
ADDRGP4 $1946
JUMPV
LABELV $1945
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
GEI4 $1949
line 2511
;2511:      ui_netSource.integer = numNetSources - 1;
ADDRGP4 ui_netSource+12
ADDRGP4 numNetSources
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2512
;2512:		}
LABELV $1949
LABELV $1946
line 2514
;2513:
;2514:		UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 2515
;2515:		if (ui_netSource.integer != AS_GLOBAL) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 2
EQI4 $1953
line 2516
;2516:			UI_StartServerRefresh(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_StartServerRefresh
CALLV
pop
line 2517
;2517:		}
LABELV $1953
line 2518
;2518:  	trap_Cvar_Set( "ui_netSource", va("%d", ui_netSource.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $1956
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2519
;2519:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1927
JUMPV
LABELV $1928
line 2521
;2520:  }
;2521:  return qfalse;
CNSTI4 0
RETI4
LABELV $1927
endproc UI_NetSource_HandleKey 12 8
proc UI_NetFilter_HandleKey 8 4
line 2524
;2522:}
;2523:
;2524:static qboolean UI_NetFilter_HandleKey(int flags, float *special, int key) {
line 2525
;2525:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1963
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1963
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1963
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1959
LABELV $1963
line 2527
;2526:
;2527:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1964
line 2528
;2528:			ui_serverFilterType.integer--;
ADDRLP4 4
ADDRGP4 ui_serverFilterType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2529
;2529:		} else {
ADDRGP4 $1965
JUMPV
LABELV $1964
line 2530
;2530:			ui_serverFilterType.integer++;
ADDRLP4 4
ADDRGP4 ui_serverFilterType+12
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2531
;2531:		}
LABELV $1965
line 2533
;2532:
;2533:    if (ui_serverFilterType.integer >= numServerFilters) {
ADDRGP4 ui_serverFilterType+12
INDIRI4
ADDRGP4 numServerFilters
INDIRI4
LTI4 $1968
line 2534
;2534:      ui_serverFilterType.integer = 0;
ADDRGP4 ui_serverFilterType+12
CNSTI4 0
ASGNI4
line 2535
;2535:    } else if (ui_serverFilterType.integer < 0) {
ADDRGP4 $1969
JUMPV
LABELV $1968
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 0
GEI4 $1972
line 2536
;2536:      ui_serverFilterType.integer = numServerFilters - 1;
ADDRGP4 ui_serverFilterType+12
ADDRGP4 numServerFilters
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2537
;2537:		}
LABELV $1972
LABELV $1969
line 2538
;2538:		UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 2539
;2539:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1958
JUMPV
LABELV $1959
line 2541
;2540:  }
;2541:  return qfalse;
CNSTI4 0
RETI4
LABELV $1958
endproc UI_NetFilter_HandleKey 8 4
proc UI_OpponentName_HandleKey 4 0
line 2544
;2542:}
;2543:
;2544:static qboolean UI_OpponentName_HandleKey(int flags, float *special, int key) {
line 2545
;2545:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1981
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1981
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1981
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1977
LABELV $1981
line 2546
;2546:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1982
line 2547
;2547:			UI_PriorOpponent();
ADDRGP4 UI_PriorOpponent
CALLV
pop
line 2548
;2548:		} else {
ADDRGP4 $1983
JUMPV
LABELV $1982
line 2549
;2549:			UI_NextOpponent();
ADDRGP4 UI_NextOpponent
CALLV
pop
line 2550
;2550:		}
LABELV $1983
line 2551
;2551:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1976
JUMPV
LABELV $1977
line 2553
;2552:  }
;2553:  return qfalse;
CNSTI4 0
RETI4
LABELV $1976
endproc UI_OpponentName_HandleKey 4 0
proc UI_BotName_HandleKey 24 4
line 2556
;2554:}
;2555:
;2556:static qboolean UI_BotName_HandleKey(int flags, float *special, int key) {
line 2557
;2557:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1989
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1989
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $1989
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $1985
LABELV $1989
line 2558
;2558:		int game = trap_Cvar_VariableValue("g_gametype");
ADDRGP4 $1323
ARGP4
ADDRLP4 12
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 2559
;2559:		int value = uiInfo.botIndex;
ADDRLP4 4
ADDRGP4 uiInfo+73444
INDIRI4
ASGNI4
line 2561
;2560:
;2561:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $1991
line 2562
;2562:			value--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2563
;2563:		} else {
ADDRGP4 $1992
JUMPV
LABELV $1991
line 2564
;2564:			value++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2565
;2565:		}
LABELV $1992
line 2567
;2566:
;2567:		if (game >= GT_TEAM) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LTI4 $1993
line 2568
;2568:			if (value >= uiInfo.characterCount + 2) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
CNSTI4 2
ADDI4
LTI4 $1995
line 2569
;2569:				value = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2570
;2570:			} else if (value < 0) {
ADDRGP4 $1994
JUMPV
LABELV $1995
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $1994
line 2571
;2571:				value = uiInfo.characterCount + 2 - 1;
ADDRLP4 4
ADDRGP4 uiInfo+73440
INDIRI4
CNSTI4 2
ADDI4
CNSTI4 1
SUBI4
ASGNI4
line 2572
;2572:			}
line 2573
;2573:		} else {
ADDRGP4 $1994
JUMPV
LABELV $1993
line 2574
;2574:			if (value >= UI_GetNumBots() + 2) {
ADDRLP4 16
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
CNSTI4 2
ADDI4
LTI4 $2001
line 2575
;2575:				value = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2576
;2576:			} else if (value < 0) {
ADDRGP4 $2002
JUMPV
LABELV $2001
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $2003
line 2577
;2577:				value = UI_GetNumBots() + 2 - 1;
ADDRLP4 20
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
CNSTI4 2
ADDI4
CNSTI4 1
SUBI4
ASGNI4
line 2578
;2578:			}
LABELV $2003
LABELV $2002
line 2579
;2579:		}
LABELV $1994
line 2580
;2580:		uiInfo.botIndex = value;
ADDRGP4 uiInfo+73444
ADDRLP4 4
INDIRI4
ASGNI4
line 2581
;2581:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1984
JUMPV
LABELV $1985
line 2583
;2582:  }
;2583:  return qfalse;
CNSTI4 0
RETI4
LABELV $1984
endproc UI_BotName_HandleKey 24 4
proc UI_BotSkill_HandleKey 8 0
line 2586
;2584:}
;2585:
;2586:static qboolean UI_BotSkill_HandleKey(int flags, float *special, int key) {
line 2587
;2587:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $2011
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $2011
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $2011
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $2007
LABELV $2011
line 2588
;2588:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $2012
line 2589
;2589:			uiInfo.skillIndex--;
ADDRLP4 4
ADDRGP4 uiInfo+96672
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2590
;2590:		} else {
ADDRGP4 $2013
JUMPV
LABELV $2012
line 2591
;2591:			uiInfo.skillIndex++;
ADDRLP4 4
ADDRGP4 uiInfo+96672
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2592
;2592:		}
LABELV $2013
line 2593
;2593:		if (uiInfo.skillIndex >= numSkillLevels) {
ADDRGP4 uiInfo+96672
INDIRI4
ADDRGP4 numSkillLevels
INDIRI4
LTI4 $2016
line 2594
;2594:			uiInfo.skillIndex = 0;
ADDRGP4 uiInfo+96672
CNSTI4 0
ASGNI4
line 2595
;2595:		} else if (uiInfo.skillIndex < 0) {
ADDRGP4 $2017
JUMPV
LABELV $2016
ADDRGP4 uiInfo+96672
INDIRI4
CNSTI4 0
GEI4 $2020
line 2596
;2596:			uiInfo.skillIndex = numSkillLevels-1;
ADDRGP4 uiInfo+96672
ADDRGP4 numSkillLevels
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2597
;2597:		}
LABELV $2020
LABELV $2017
line 2598
;2598:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2006
JUMPV
LABELV $2007
line 2600
;2599:  }
;2600:	return qfalse;
CNSTI4 0
RETI4
LABELV $2006
endproc UI_BotSkill_HandleKey 8 0
proc UI_RedBlue_HandleKey 8 0
line 2603
;2601:}
;2602:
;2603:static qboolean UI_RedBlue_HandleKey(int flags, float *special, int key) {
line 2604
;2604:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $2029
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $2029
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $2029
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $2025
LABELV $2029
line 2605
;2605:		uiInfo.redBlue ^= 1;
ADDRLP4 4
ADDRGP4 uiInfo+78840
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 2606
;2606:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2024
JUMPV
LABELV $2025
line 2608
;2607:	}
;2608:	return qfalse;
CNSTI4 0
RETI4
LABELV $2024
endproc UI_RedBlue_HandleKey 8 0
proc UI_Crosshair_HandleKey 8 8
line 2611
;2609:}
;2610:
;2611:static qboolean UI_Crosshair_HandleKey(int flags, float *special, int key) {
line 2612
;2612:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $2036
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $2036
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $2036
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $2032
LABELV $2036
line 2613
;2613:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $2037
line 2614
;2614:			uiInfo.currentCrosshair--;
ADDRLP4 4
ADDRGP4 uiInfo+119476
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2615
;2615:		} else {
ADDRGP4 $2038
JUMPV
LABELV $2037
line 2616
;2616:			uiInfo.currentCrosshair++;
ADDRLP4 4
ADDRGP4 uiInfo+119476
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2617
;2617:		}
LABELV $2038
line 2619
;2618:
;2619:		if (uiInfo.currentCrosshair >= NUM_CROSSHAIRS) {
ADDRGP4 uiInfo+119476
INDIRI4
CNSTI4 10
LTI4 $2041
line 2620
;2620:			uiInfo.currentCrosshair = 0;
ADDRGP4 uiInfo+119476
CNSTI4 0
ASGNI4
line 2621
;2621:		} else if (uiInfo.currentCrosshair < 0) {
ADDRGP4 $2042
JUMPV
LABELV $2041
ADDRGP4 uiInfo+119476
INDIRI4
CNSTI4 0
GEI4 $2045
line 2622
;2622:			uiInfo.currentCrosshair = NUM_CROSSHAIRS - 1;
ADDRGP4 uiInfo+119476
CNSTI4 9
ASGNI4
line 2623
;2623:		}
LABELV $2045
LABELV $2042
line 2624
;2624:		trap_Cvar_Set("cg_drawCrosshair", va("%d", uiInfo.currentCrosshair)); 
ADDRGP4 $713
ARGP4
ADDRGP4 uiInfo+119476
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2049
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2625
;2625:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2031
JUMPV
LABELV $2032
line 2627
;2626:	}
;2627:	return qfalse;
CNSTI4 0
RETI4
LABELV $2031
endproc UI_Crosshair_HandleKey 8 8
proc UI_SelectedPlayer_HandleKey 16 8
line 2632
;2628:}
;2629:
;2630:
;2631:
;2632:static qboolean UI_SelectedPlayer_HandleKey(int flags, float *special, int key) {
line 2633
;2633:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) {
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $2056
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $2056
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $2056
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $2052
LABELV $2056
line 2636
;2634:		int selected;
;2635:
;2636:		UI_BuildPlayerList();
ADDRGP4 UI_BuildPlayerList
CALLV
pop
line 2637
;2637:		if (!uiInfo.teamLeader) {
ADDRGP4 uiInfo+78868
INDIRI4
CNSTI4 0
NEI4 $2057
line 2638
;2638:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2051
JUMPV
LABELV $2057
line 2640
;2639:		}
;2640:		selected = trap_Cvar_VariableValue("cg_selectedPlayer");
ADDRGP4 $1391
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 2642
;2641:		
;2642:		if (key == K_MOUSE2) {
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $2060
line 2643
;2643:			selected--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2644
;2644:		} else {
ADDRGP4 $2061
JUMPV
LABELV $2060
line 2645
;2645:			selected++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2646
;2646:		}
LABELV $2061
line 2648
;2647:
;2648:		if (selected > uiInfo.myTeamCount) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
LEI4 $2062
line 2649
;2649:			selected = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2650
;2650:		} else if (selected < 0) {
ADDRGP4 $2063
JUMPV
LABELV $2062
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $2065
line 2651
;2651:			selected = uiInfo.myTeamCount;
ADDRLP4 4
ADDRGP4 uiInfo+78848
INDIRI4
ASGNI4
line 2652
;2652:		}
LABELV $2065
LABELV $2063
line 2654
;2653:
;2654:		if (selected == uiInfo.myTeamCount) {
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
NEI4 $2068
line 2655
;2655:		 	trap_Cvar_Set( "cg_selectedPlayerName", "Everyone");
ADDRGP4 $1399
ARGP4
ADDRGP4 $2071
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2656
;2656:		} else {
ADDRGP4 $2069
JUMPV
LABELV $2068
line 2657
;2657:		 	trap_Cvar_Set( "cg_selectedPlayerName", uiInfo.teamNames[selected]);
ADDRGP4 $1399
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2658
;2658:		}
LABELV $2069
line 2659
;2659:	 	trap_Cvar_Set( "cg_selectedPlayer", va("%d", selected));
ADDRGP4 $713
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $1391
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2660
;2660:	}
LABELV $2052
line 2661
;2661:	return qfalse;
CNSTI4 0
RETI4
LABELV $2051
endproc UI_SelectedPlayer_HandleKey 16 8
proc UI_OwnerDrawHandleKey 52 20
line 2665
;2662:}
;2663:
;2664:
;2665:static qboolean UI_OwnerDrawHandleKey(int ownerDraw, int flags, float *special, int key) {
line 2666
;2666:  switch (ownerDraw) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 200
LTI4 $2075
ADDRLP4 0
INDIRI4
CNSTI4 253
GTI4 $2075
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $2095-800
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $2095
address $2076
address $2077
address $2075
address $2078
address $2075
address $2079
address $2075
address $2082
address $2083
address $2084
address $2085
address $2085
address $2085
address $2085
address $2085
address $2086
address $2086
address $2086
address $2086
address $2086
address $2087
address $2075
address $2088
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2089
address $2075
address $2090
address $2091
address $2092
address $2093
address $2094
address $2075
address $2080
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2075
address $2081
code
LABELV $2076
line 2668
;2667:    case UI_HANDICAP:
;2668:      return UI_Handicap_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 UI_Handicap_HandleKey
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2669
;2669:      break;
LABELV $2077
line 2671
;2670:    case UI_EFFECTS:
;2671:      return UI_Effects_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 UI_Effects_HandleKey
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2672
;2672:      break;
LABELV $2078
line 2674
;2673:    case UI_CLANNAME:
;2674:      return UI_ClanName_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 UI_ClanName_HandleKey
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2675
;2675:      break;
LABELV $2079
line 2677
;2676:    case UI_GAMETYPE:
;2677:      return UI_GameType_HandleKey(flags, special, key, qtrue);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 UI_GameType_HandleKey
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2678
;2678:      break;
LABELV $2080
line 2680
;2679:    case UI_NETGAMETYPE:
;2680:      return UI_NetGameType_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 UI_NetGameType_HandleKey
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2681
;2681:      break;
LABELV $2081
line 2683
;2682:    case UI_JOINGAMETYPE:
;2683:      return UI_JoinGameType_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_JoinGameType_HandleKey
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2684
;2684:      break;
LABELV $2082
line 2686
;2685:    case UI_SKILL:
;2686:      return UI_Skill_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 UI_Skill_HandleKey
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2687
;2687:      break;
LABELV $2083
line 2689
;2688:    case UI_BLUETEAMNAME:
;2689:      return UI_TeamName_HandleKey(flags, special, key, qtrue);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 32
ADDRGP4 UI_TeamName_HandleKey
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2690
;2690:      break;
LABELV $2084
line 2692
;2691:    case UI_REDTEAMNAME:
;2692:      return UI_TeamName_HandleKey(flags, special, key, qfalse);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 36
ADDRGP4 UI_TeamName_HandleKey
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2693
;2693:      break;
LABELV $2085
line 2699
;2694:    case UI_BLUETEAM1:
;2695:		case UI_BLUETEAM2:
;2696:		case UI_BLUETEAM3:
;2697:		case UI_BLUETEAM4:
;2698:		case UI_BLUETEAM5:
;2699:      UI_TeamMember_HandleKey(flags, special, key, qtrue, ownerDraw - UI_BLUETEAM1 + 1);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 40
CNSTI4 1
ASGNI4
ADDRLP4 40
INDIRI4
ARGI4
ADDRFP4 0
INDIRI4
CNSTI4 210
SUBI4
ADDRLP4 40
INDIRI4
ADDI4
ARGI4
ADDRGP4 UI_TeamMember_HandleKey
CALLI4
pop
line 2700
;2700:      break;
ADDRGP4 $2075
JUMPV
LABELV $2086
line 2706
;2701:    case UI_REDTEAM1:
;2702:		case UI_REDTEAM2:
;2703:		case UI_REDTEAM3:
;2704:		case UI_REDTEAM4:
;2705:		case UI_REDTEAM5:
;2706:      UI_TeamMember_HandleKey(flags, special, key, qfalse, ownerDraw - UI_REDTEAM1 + 1);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRI4
CNSTI4 215
SUBI4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 UI_TeamMember_HandleKey
CALLI4
pop
line 2707
;2707:      break;
ADDRGP4 $2075
JUMPV
LABELV $2087
line 2709
;2708:		case UI_NETSOURCE:
;2709:      UI_NetSource_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_NetSource_HandleKey
CALLI4
pop
line 2710
;2710:			break;
ADDRGP4 $2075
JUMPV
LABELV $2088
line 2712
;2711:		case UI_NETFILTER:
;2712:      UI_NetFilter_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_NetFilter_HandleKey
CALLI4
pop
line 2713
;2713:			break;
ADDRGP4 $2075
JUMPV
LABELV $2089
line 2715
;2714:		case UI_OPPONENT_NAME:
;2715:			UI_OpponentName_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_OpponentName_HandleKey
CALLI4
pop
line 2716
;2716:			break;
ADDRGP4 $2075
JUMPV
LABELV $2090
line 2718
;2717:		case UI_BOTNAME:
;2718:			return UI_BotName_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 UI_BotName_HandleKey
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2719
;2719:			break;
LABELV $2091
line 2721
;2720:		case UI_BOTSKILL:
;2721:			return UI_BotSkill_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 48
ADDRGP4 UI_BotSkill_HandleKey
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
RETI4
ADDRGP4 $2073
JUMPV
line 2722
;2722:			break;
LABELV $2092
line 2724
;2723:		case UI_REDBLUE:
;2724:			UI_RedBlue_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_RedBlue_HandleKey
CALLI4
pop
line 2725
;2725:			break;
ADDRGP4 $2075
JUMPV
LABELV $2093
line 2727
;2726:		case UI_CROSSHAIR:
;2727:			UI_Crosshair_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_Crosshair_HandleKey
CALLI4
pop
line 2728
;2728:			break;
ADDRGP4 $2075
JUMPV
LABELV $2094
line 2730
;2729:		case UI_SELECTEDPLAYER:
;2730:			UI_SelectedPlayer_HandleKey(flags, special, key);
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 UI_SelectedPlayer_HandleKey
CALLI4
pop
line 2731
;2731:			break;
line 2733
;2732:    default:
;2733:      break;
LABELV $2075
line 2736
;2734:  }
;2735:
;2736:  return qfalse;
CNSTI4 0
RETI4
LABELV $2073
endproc UI_OwnerDrawHandleKey 52 20
proc UI_GetValue 0 0
line 2740
;2737:}
;2738:
;2739:
;2740:static float UI_GetValue(int ownerDraw) {
line 2741
;2741:  return 0;
CNSTF4 0
RETF4
LABELV $2097
endproc UI_GetValue 0 0
proc UI_ServersQsortCompare 4 20
line 2749
;2742:}
;2743:
;2744:/*
;2745:=================
;2746:UI_ServersQsortCompare
;2747:=================
;2748:*/
;2749:static int QDECL UI_ServersQsortCompare( const void *arg1, const void *arg2 ) {
line 2750
;2750:	return trap_LAN_CompareServers( ui_netSource.integer, uiInfo.serverStatus.sortKey, uiInfo.serverStatus.sortDir, *(int*)arg1, *(int*)arg2);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2200
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2204
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 trap_LAN_CompareServers
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $2098
endproc UI_ServersQsortCompare 4 20
export UI_ServersSort
proc UI_ServersSort 0 16
line 2759
;2751:}
;2752:
;2753:
;2754:/*
;2755:=================
;2756:UI_ServersSort
;2757:=================
;2758:*/
;2759:void UI_ServersSort(int column, qboolean force) {
line 2761
;2760:
;2761:	if ( !force ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $2105
line 2762
;2762:		if ( uiInfo.serverStatus.sortKey == column ) {
ADDRGP4 uiInfo+99264+2200
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $2107
line 2763
;2763:			return;
ADDRGP4 $2104
JUMPV
LABELV $2107
line 2765
;2764:		}
;2765:	}
LABELV $2105
line 2767
;2766:
;2767:	uiInfo.serverStatus.sortKey = column;
ADDRGP4 uiInfo+99264+2200
ADDRFP4 0
INDIRI4
ASGNI4
line 2768
;2768:	qsort( &uiInfo.serverStatus.displayServers[0], uiInfo.serverStatus.numDisplayServers, sizeof(int), UI_ServersQsortCompare);
ADDRGP4 uiInfo+99264+2220
ARGP4
ADDRGP4 uiInfo+99264+10412
INDIRI4
ARGI4
CNSTI4 4
ARGI4
ADDRGP4 UI_ServersQsortCompare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 2769
;2769:}
LABELV $2104
endproc UI_ServersSort 0 16
proc UI_LoadMods 2092 16
line 2816
;2770:
;2771:/*
;2772:static void UI_StartSinglePlayer() {
;2773:	int i,j, k, skill;
;2774:	char buff[1024];
;2775:	i = trap_Cvar_VariableValue( "ui_currentTier" );
;2776:  if (i < 0 || i >= tierCount) {
;2777:    i = 0;
;2778:  }
;2779:	j = trap_Cvar_VariableValue("ui_currentMap");
;2780:	if (j < 0 || j > MAPS_PER_TIER) {
;2781:		j = 0;
;2782:	}
;2783:
;2784: 	trap_Cvar_SetValue( "singleplayer", 1 );
;2785: 	trap_Cvar_SetValue( "g_gametype", Com_Clamp( 0, 7, tierList[i].gameTypes[j] ) );
;2786:	trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", tierList[i].maps[j] ) );
;2787:	skill = trap_Cvar_VariableValue( "g_spSkill" );
;2788:
;2789:	if (j == MAPS_PER_TIER-1) {
;2790:		k = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
;2791:		Com_sprintf( buff, sizeof(buff), "wait ; addbot %s %i %s 250 %s\n", UI_AIFromName(teamList[k].teamMembers[0]), skill, "", teamList[k].teamMembers[0]);
;2792:	} else {
;2793:		k = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
;2794:		for (i = 0; i < PLAYERS_PER_TEAM; i++) {
;2795:			Com_sprintf( buff, sizeof(buff), "wait ; addbot %s %i %s 250 %s\n", UI_AIFromName(teamList[k].teamMembers[i]), skill, "Blue", teamList[k].teamMembers[i]);
;2796:			trap_Cmd_ExecuteText( EXEC_APPEND, buff );
;2797:		}
;2798:
;2799:		k = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
;2800:		for (i = 1; i < PLAYERS_PER_TEAM; i++) {
;2801:			Com_sprintf( buff, sizeof(buff), "wait ; addbot %s %i %s 250 %s\n", UI_AIFromName(teamList[k].teamMembers[i]), skill, "Red", teamList[k].teamMembers[i]);
;2802:			trap_Cmd_ExecuteText( EXEC_APPEND, buff );
;2803:		}
;2804:		trap_Cmd_ExecuteText( EXEC_APPEND, "wait 5; team Red\n" );
;2805:	}
;2806:	
;2807:
;2808:}
;2809:*/
;2810:
;2811:/*
;2812:===============
;2813:UI_LoadMods
;2814:===============
;2815:*/
;2816:static void UI_LoadMods() {
line 2824
;2817:	int		numdirs;
;2818:	char	dirlist[2048];
;2819:	char	*dirptr;
;2820:  char  *descptr;
;2821:	int		i;
;2822:	int		dirlen;
;2823:
;2824:	uiInfo.modCount = 0;
ADDRGP4 uiInfo+97188
CNSTI4 0
ASGNI4
line 2825
;2825:	numdirs = trap_FS_GetFileList( "$modlist", "", dirlist, sizeof(dirlist) );
ADDRGP4 $2119
ARGP4
ADDRGP4 $109
ARGP4
ADDRLP4 20
ARGP4
CNSTI4 2048
ARGI4
ADDRLP4 2068
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 2068
INDIRI4
ASGNI4
line 2826
;2826:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 20
ASGNP4
line 2827
;2827:	for( i = 0; i < numdirs; i++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $2123
JUMPV
LABELV $2120
line 2828
;2828:		dirlen = strlen( dirptr ) + 1;
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 2072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 2072
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2829
;2829:    descptr = dirptr + dirlen;
ADDRLP4 4
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 2830
;2830:		uiInfo.modList[uiInfo.modCount].modName = String_Alloc(dirptr);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 2076
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRGP4 uiInfo+97188
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+96676
ADDP4
ADDRLP4 2076
INDIRP4
ASGNP4
line 2831
;2831:		uiInfo.modList[uiInfo.modCount].modDescr = String_Alloc(descptr);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 2080
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRGP4 uiInfo+97188
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+96676+4
ADDP4
ADDRLP4 2080
INDIRP4
ASGNP4
line 2832
;2832:    dirptr += dirlen + strlen(descptr) + 1;
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 2084
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ADDRLP4 2084
INDIRI4
ADDI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 2833
;2833:		uiInfo.modCount++;
ADDRLP4 2088
ADDRGP4 uiInfo+97188
ASGNP4
ADDRLP4 2088
INDIRP4
ADDRLP4 2088
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2834
;2834:		if (uiInfo.modCount >= MAX_MODS) {
ADDRGP4 uiInfo+97188
INDIRI4
CNSTI4 64
LTI4 $2130
line 2835
;2835:			break;
ADDRGP4 $2122
JUMPV
LABELV $2130
line 2837
;2836:		}
;2837:	}
LABELV $2121
line 2827
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2123
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $2120
LABELV $2122
line 2839
;2838:
;2839:}
LABELV $2117
endproc UI_LoadMods 2092 16
proc UI_LoadTeams 4120 16
line 2847
;2840:
;2841:
;2842:/*
;2843:===============
;2844:UI_LoadTeams
;2845:===============
;2846:*/
;2847:static void UI_LoadTeams() {
line 2852
;2848:	char	teamList[4096];
;2849:	char	*teamName;
;2850:	int		i, len, count;
;2851:
;2852:	count = trap_FS_GetFileList( "", "team", teamList, 4096 );
ADDRGP4 $109
ARGP4
ADDRGP4 $2134
ARGP4
ADDRLP4 16
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4112
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 4112
INDIRI4
ASGNI4
line 2854
;2853:
;2854:	if (count) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2135
line 2855
;2855:		teamName = teamList;
ADDRLP4 0
ADDRLP4 16
ASGNP4
line 2856
;2856:		for ( i = 0; i < count; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $2140
JUMPV
LABELV $2137
line 2857
;2857:			len = strlen( teamName );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4116
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4116
INDIRI4
ASGNI4
line 2858
;2858:			UI_ParseTeamInfo(teamName);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 UI_ParseTeamInfo
CALLV
pop
line 2859
;2859:			teamName += len + 1;
ADDRLP4 0
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 2860
;2860:		}
LABELV $2138
line 2856
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2140
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LTI4 $2137
line 2861
;2861:	}
LABELV $2135
line 2863
;2862:
;2863:}
LABELV $2133
endproc UI_LoadTeams 4120 16
proc UI_LoadMovies 4124 16
line 2871
;2864:
;2865:
;2866:/*
;2867:===============
;2868:UI_LoadMovies
;2869:===============
;2870:*/
;2871:static void UI_LoadMovies() {
line 2876
;2872:	char	movielist[4096];
;2873:	char	*moviename;
;2874:	int		i, len;
;2875:
;2876:	uiInfo.movieCount = trap_FS_GetFileList( "video", "roq", movielist, 4096 );
ADDRGP4 $2143
ARGP4
ADDRGP4 $2144
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4108
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRGP4 uiInfo+99252
ADDRLP4 4108
INDIRI4
ASGNI4
line 2878
;2877:
;2878:	if (uiInfo.movieCount) {
ADDRGP4 uiInfo+99252
INDIRI4
CNSTI4 0
EQI4 $2145
line 2879
;2879:		if (uiInfo.movieCount > MAX_MOVIES) {
ADDRGP4 uiInfo+99252
INDIRI4
CNSTI4 256
LEI4 $2148
line 2880
;2880:			uiInfo.movieCount = MAX_MOVIES;
ADDRGP4 uiInfo+99252
CNSTI4 256
ASGNI4
line 2881
;2881:		}
LABELV $2148
line 2882
;2882:		moviename = movielist;
ADDRLP4 0
ADDRLP4 12
ASGNP4
line 2883
;2883:		for ( i = 0; i < uiInfo.movieCount; i++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $2155
JUMPV
LABELV $2152
line 2884
;2884:			len = strlen( moviename );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4112
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 4112
INDIRI4
ASGNI4
line 2885
;2885:			if (!Q_stricmp(moviename +  len - 4,".roq")) {
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
ADDP4
CNSTI4 -4
ADDP4
ARGP4
ADDRGP4 $2159
ARGP4
ADDRLP4 4116
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4116
INDIRI4
CNSTI4 0
NEI4 $2157
line 2886
;2886:				moviename[len-4] = '\0';
ADDRLP4 4
INDIRI4
CNSTI4 4
SUBI4
ADDRLP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 2887
;2887:			}
LABELV $2157
line 2888
;2888:			Q_strupr(moviename);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 2889
;2889:			uiInfo.movieList[i] = String_Alloc(moviename);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4120
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+98228
ADDP4
ADDRLP4 4120
INDIRP4
ASGNP4
line 2890
;2890:			moviename += len + 1;
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 2891
;2891:		}
LABELV $2153
line 2883
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2155
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+99252
INDIRI4
LTI4 $2152
line 2892
;2892:	}
LABELV $2145
line 2894
;2893:
;2894:}
LABELV $2141
endproc UI_LoadMovies 4124 16
proc UI_LoadDemos 4168 16
line 2903
;2895:
;2896:
;2897:
;2898:/*
;2899:===============
;2900:UI_LoadDemos
;2901:===============
;2902:*/
;2903:static void UI_LoadDemos() {
line 2909
;2904:	char	demolist[4096];
;2905:	char demoExt[32];
;2906:	char	*demoname;
;2907:	int		i, len;
;2908:
;2909:	Com_sprintf(demoExt, sizeof(demoExt), "dm_%d", (int)trap_Cvar_VariableValue("protocol"));
ADDRGP4 $2163
ARGP4
ADDRLP4 4140
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $2162
ARGP4
ADDRLP4 4140
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 2911
;2910:
;2911:	uiInfo.demoCount = trap_FS_GetFileList( "demos", demoExt, demolist, 4096 );
ADDRGP4 $2165
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 44
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4144
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRGP4 uiInfo+98220
ADDRLP4 4144
INDIRI4
ASGNI4
line 2913
;2912:
;2913:	Com_sprintf(demoExt, sizeof(demoExt), ".dm_%d", (int)trap_Cvar_VariableValue("protocol"));
ADDRGP4 $2163
ARGP4
ADDRLP4 4148
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $2166
ARGP4
ADDRLP4 4148
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 2915
;2914:
;2915:	if (uiInfo.demoCount) {
ADDRGP4 uiInfo+98220
INDIRI4
CNSTI4 0
EQI4 $2167
line 2916
;2916:		if (uiInfo.demoCount > MAX_DEMOS) {
ADDRGP4 uiInfo+98220
INDIRI4
CNSTI4 256
LEI4 $2170
line 2917
;2917:			uiInfo.demoCount = MAX_DEMOS;
ADDRGP4 uiInfo+98220
CNSTI4 256
ASGNI4
line 2918
;2918:		}
LABELV $2170
line 2919
;2919:		demoname = demolist;
ADDRLP4 0
ADDRLP4 44
ASGNP4
line 2920
;2920:		for ( i = 0; i < uiInfo.demoCount; i++ ) {
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRGP4 $2177
JUMPV
LABELV $2174
line 2921
;2921:			len = strlen( demoname );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4152
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 36
ADDRLP4 4152
INDIRI4
ASGNI4
line 2922
;2922:			if (!Q_stricmp(demoname +  len - strlen(demoExt), demoExt)) {
ADDRLP4 4
ARGP4
ADDRLP4 4156
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRLP4 0
INDIRP4
ADDP4
ADDRLP4 4156
INDIRI4
SUBP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 4160
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4160
INDIRI4
CNSTI4 0
NEI4 $2179
line 2923
;2923:				demoname[len-strlen(demoExt)] = '\0';
ADDRLP4 4
ARGP4
ADDRLP4 4164
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRLP4 4164
INDIRI4
SUBI4
ADDRLP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 2924
;2924:			}
LABELV $2179
line 2925
;2925:			Q_strupr(demoname);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 2926
;2926:			uiInfo.demoList[i] = String_Alloc(demoname);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4164
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 40
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+97196
ADDP4
ADDRLP4 4164
INDIRP4
ASGNP4
line 2927
;2927:			demoname += len + 1;
ADDRLP4 0
ADDRLP4 36
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 2928
;2928:		}
LABELV $2175
line 2920
ADDRLP4 40
ADDRLP4 40
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2177
ADDRLP4 40
INDIRI4
ADDRGP4 uiInfo+98220
INDIRI4
LTI4 $2174
line 2929
;2929:	}
LABELV $2167
line 2931
;2930:
;2931:}
LABELV $2161
endproc UI_LoadDemos 4168 16
proc UI_SetNextMap 8 16
line 2934
;2932:
;2933:
;2934:static qboolean UI_SetNextMap(int actual, int index) {
line 2936
;2935:	int i;
;2936:	for (i = actual + 1; i < uiInfo.mapCount; i++) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $2186
JUMPV
LABELV $2183
line 2937
;2937:		if (uiInfo.mapList[i].active) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+96
ADDP4
INDIRI4
CNSTI4 0
EQI4 $2188
line 2938
;2938:			Menu_SetFeederSelection(NULL, FEEDER_MAPS, index + 1, "skirmish");
CNSTP4 0
ARGP4
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ARGI4
ADDRGP4 $2192
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 2939
;2939:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2182
JUMPV
LABELV $2188
line 2941
;2940:		}
;2941:	}
LABELV $2184
line 2936
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2186
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LTI4 $2183
line 2942
;2942:	return qfalse;
CNSTI4 0
RETI4
LABELV $2182
endproc UI_SetNextMap 8 16
proc UI_StartSkirmish 1160 32
line 2946
;2943:}
;2944:
;2945:
;2946:static void UI_StartSkirmish(qboolean next) {
line 2951
;2947:	int i, k, g, delay, temp;
;2948:	float skill;
;2949:	char buff[MAX_STRING_CHARS];
;2950:
;2951:	if (next) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $2194
line 2953
;2952:		int actual;
;2953:		int index = trap_Cvar_VariableValue("ui_mapIndex");
ADDRGP4 $2196
ARGP4
ADDRLP4 1056
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1048
ADDRLP4 1056
INDIRF4
CVFI4 4
ASGNI4
line 2954
;2954:	 	UI_MapCountByGameType(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_MapCountByGameType
CALLI4
pop
line 2955
;2955:		UI_SelectedMap(index, &actual);
ADDRLP4 1048
INDIRI4
ARGI4
ADDRLP4 1052
ARGP4
ADDRGP4 UI_SelectedMap
CALLP4
pop
line 2956
;2956:		if (UI_SetNextMap(actual, index)) {
ADDRLP4 1052
INDIRI4
ARGI4
ADDRLP4 1048
INDIRI4
ARGI4
ADDRLP4 1060
ADDRGP4 UI_SetNextMap
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
EQI4 $2197
line 2957
;2957:		} else {
ADDRGP4 $2198
JUMPV
LABELV $2197
line 2958
;2958:			UI_GameType_HandleKey(0, 0, K_MOUSE1, qfalse);
ADDRLP4 1064
CNSTI4 0
ASGNI4
ADDRLP4 1064
INDIRI4
ARGI4
CNSTP4 0
ARGP4
CNSTI4 178
ARGI4
ADDRLP4 1064
INDIRI4
ARGI4
ADDRGP4 UI_GameType_HandleKey
CALLI4
pop
line 2959
;2959:			UI_MapCountByGameType(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_MapCountByGameType
CALLI4
pop
line 2960
;2960:			Menu_SetFeederSelection(NULL, FEEDER_MAPS, 0, "skirmish");
CNSTP4 0
ARGP4
CNSTI4 1
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 $2192
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 2961
;2961:		}
LABELV $2198
line 2962
;2962:	}
LABELV $2194
line 2964
;2963:
;2964:	g = uiInfo.gameTypes[ui_gameType.integer].gtEnum;
ADDRLP4 1040
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ASGNI4
line 2965
;2965:	trap_Cvar_SetValue( "g_gametype", g );
ADDRGP4 $1323
ARGP4
ADDRLP4 1040
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 2966
;2966:	trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", uiInfo.mapList[ui_currentMap.integer].mapLoadName) );
ADDRGP4 $2202
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1048
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 2967
;2967:	skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $824
ARGP4
ADDRLP4 1052
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1044
ADDRLP4 1052
INDIRF4
ASGNF4
line 2968
;2968:	trap_Cvar_Set("ui_scoreMap", uiInfo.mapList[ui_currentMap.integer].mapName);
ADDRGP4 $2206
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2970
;2969:
;2970:	k = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 1056
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRLP4 1060
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 1028
ADDRLP4 1060
INDIRI4
ASGNI4
line 2972
;2971:
;2972:	trap_Cvar_Set("ui_singlePlayerActive", "1");
ADDRGP4 $2209
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2975
;2973:
;2974:	// set up sp overrides, will be replaced on postgame
;2975:	temp = trap_Cvar_VariableValue( "capturelimit" );
ADDRGP4 $715
ARGP4
ADDRLP4 1064
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1064
INDIRF4
CVFI4 4
ASGNI4
line 2976
;2976:	trap_Cvar_Set("ui_saveCaptureLimit", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1068
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2210
ARGP4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2977
;2977:	temp = trap_Cvar_VariableValue( "fraglimit" );
ADDRGP4 $716
ARGP4
ADDRLP4 1072
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1072
INDIRF4
CVFI4 4
ASGNI4
line 2978
;2978:	trap_Cvar_Set("ui_saveFragLimit", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1076
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2211
ARGP4
ADDRLP4 1076
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2980
;2979:
;2980:	UI_SetCapFragLimits(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_SetCapFragLimits
CALLV
pop
line 2982
;2981:
;2982:	temp = trap_Cvar_VariableValue( "cg_drawTimer" );
ADDRGP4 $2212
ARGP4
ADDRLP4 1080
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1080
INDIRF4
CVFI4 4
ASGNI4
line 2983
;2983:	trap_Cvar_Set("ui_drawTimer", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1084
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2213
ARGP4
ADDRLP4 1084
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2984
;2984:	temp = trap_Cvar_VariableValue( "g_doWarmup" );
ADDRGP4 $2214
ARGP4
ADDRLP4 1088
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1088
INDIRF4
CVFI4 4
ASGNI4
line 2985
;2985:	trap_Cvar_Set("ui_doWarmup", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1092
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2215
ARGP4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2986
;2986:	temp = trap_Cvar_VariableValue( "g_friendlyFire" );
ADDRGP4 $2216
ARGP4
ADDRLP4 1096
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1096
INDIRF4
CVFI4 4
ASGNI4
line 2987
;2987:	trap_Cvar_Set("ui_friendlyFire", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1100
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2217
ARGP4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2988
;2988:	temp = trap_Cvar_VariableValue( "sv_maxClients" );
ADDRGP4 $2218
ARGP4
ADDRLP4 1104
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1104
INDIRF4
CVFI4 4
ASGNI4
line 2989
;2989:	trap_Cvar_Set("ui_maxClients", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1108
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2219
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2990
;2990:	temp = trap_Cvar_VariableValue( "g_warmup" );
ADDRGP4 $2220
ARGP4
ADDRLP4 1112
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1112
INDIRF4
CVFI4 4
ASGNI4
line 2991
;2991:	trap_Cvar_Set("ui_Warmup", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1116
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2221
ARGP4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2992
;2992:	temp = trap_Cvar_VariableValue( "sv_pure" );
ADDRGP4 $2222
ARGP4
ADDRLP4 1120
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1036
ADDRLP4 1120
INDIRF4
CVFI4 4
ASGNI4
line 2993
;2993:	trap_Cvar_Set("ui_pure", va("%i", temp));
ADDRGP4 $1737
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1124
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2223
ARGP4
ADDRLP4 1124
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2995
;2994:
;2995:	trap_Cvar_Set("cg_cameraOrbit", "0");
ADDRGP4 $391
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2996
;2996:	trap_Cvar_Set("cg_thirdPerson", "0");
ADDRGP4 $393
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2997
;2997:	trap_Cvar_Set("cg_drawTimer", "1");
ADDRGP4 $2212
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2998
;2998:	trap_Cvar_Set("g_doWarmup", "1");
ADDRGP4 $2214
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 2999
;2999:	trap_Cvar_Set("g_warmup", "15");
ADDRGP4 $2220
ARGP4
ADDRGP4 $692
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3000
;3000:	trap_Cvar_Set("sv_pure", "0");
ADDRGP4 $2222
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3001
;3001:	trap_Cvar_Set("g_friendlyFire", "0");
ADDRGP4 $2216
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3002
;3002:	trap_Cvar_Set("g_redTeam", UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 1128
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $2224
ARGP4
ADDRLP4 1128
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3003
;3003:	trap_Cvar_Set("g_blueTeam", UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 1132
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $2225
ARGP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3005
;3004:
;3005:	if (trap_Cvar_VariableValue("ui_recordSPDemo")) {
ADDRGP4 $2228
ARGP4
ADDRLP4 1136
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1136
INDIRF4
CNSTF4 0
EQF4 $2226
line 3006
;3006:		Com_sprintf(buff, MAX_STRING_CHARS, "%s_%i", uiInfo.mapList[ui_currentMap.integer].mapLoadName, g);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2229
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1040
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 3007
;3007:		trap_Cvar_Set("ui_recordSPDemoName", buff);
ADDRGP4 $2233
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3008
;3008:	}
LABELV $2226
line 3010
;3009:
;3010:	delay = 500;
ADDRLP4 1032
CNSTI4 500
ASGNI4
line 3012
;3011:
;3012:	if (g == GT_TOURNAMENT) {
ADDRLP4 1040
INDIRI4
CNSTI4 1
NEI4 $2234
line 3013
;3013:		trap_Cvar_Set("sv_maxClients", "2");
ADDRGP4 $2218
ARGP4
ADDRGP4 $2236
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3014
;3014:		Com_sprintf( buff, sizeof(buff), "wait ; addbot %s %f "", %i \n", uiInfo.mapList[ui_currentMap.integer].opponentName, skill, delay);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2237
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+12
ADDP4
INDIRP4
ARGP4
ADDRLP4 1044
INDIRF4
ARGF4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 3015
;3015:		trap_Cmd_ExecuteText( EXEC_APPEND, buff );
CNSTI4 2
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3016
;3016:	} else {
ADDRGP4 $2235
JUMPV
LABELV $2234
line 3017
;3017:		temp = uiInfo.mapList[ui_currentMap.integer].teamMembers * 2;
ADDRLP4 1036
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+16
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 3018
;3018:		trap_Cvar_Set("sv_maxClients", va("%d", temp));
ADDRGP4 $713
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1140
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2218
ARGP4
ADDRLP4 1140
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3019
;3019:		for (i =0; i < uiInfo.mapList[ui_currentMap.integer].teamMembers; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2247
JUMPV
LABELV $2244
line 3020
;3020:			Com_sprintf( buff, sizeof(buff), "addbot %s %f %s %i %s\n", UI_AIFromName(uiInfo.teamList[k].teamMembers[i]), skill, (g == GT_FFA) ? "" : "Blue", delay, uiInfo.teamList[k].teamMembers[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1148
ADDRGP4 UI_AIFromName
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2251
ARGP4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRLP4 1044
INDIRF4
ARGF4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $2257
ADDRLP4 1144
ADDRGP4 $109
ASGNP4
ADDRGP4 $2258
JUMPV
LABELV $2257
ADDRLP4 1144
ADDRGP4 $839
ASGNP4
LABELV $2258
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3021
;3021:			trap_Cmd_ExecuteText( EXEC_APPEND, buff );
CNSTI4 2
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3022
;3022:			delay += 500;
ADDRLP4 1032
ADDRLP4 1032
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 3023
;3023:		}
LABELV $2245
line 3019
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2247
ADDRLP4 0
INDIRI4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+16
ADDP4
INDIRI4
LTI4 $2244
line 3024
;3024:		k = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 1144
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1148
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 1028
ADDRLP4 1148
INDIRI4
ASGNI4
line 3025
;3025:		for (i =0; i < uiInfo.mapList[ui_currentMap.integer].teamMembers-1; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2262
JUMPV
LABELV $2259
line 3026
;3026:			Com_sprintf( buff, sizeof(buff), "addbot %s %f %s %i %s\n", UI_AIFromName(uiInfo.teamList[k].teamMembers[i]), skill, (g == GT_FFA) ? "" : "Red", delay, uiInfo.teamList[k].teamMembers[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1156
ADDRGP4 UI_AIFromName
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2251
ARGP4
ADDRLP4 1156
INDIRP4
ARGP4
ADDRLP4 1044
INDIRF4
ARGF4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $2271
ADDRLP4 1152
ADDRGP4 $109
ASGNP4
ADDRGP4 $2272
JUMPV
LABELV $2271
ADDRLP4 1152
ADDRGP4 $840
ASGNP4
LABELV $2272
ADDRLP4 1152
INDIRP4
ARGP4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3027
;3027:			trap_Cmd_ExecuteText( EXEC_APPEND, buff );
CNSTI4 2
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3028
;3028:			delay += 500;
ADDRLP4 1032
ADDRLP4 1032
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 3029
;3029:		}
LABELV $2260
line 3025
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2262
ADDRLP4 0
INDIRI4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+16
ADDP4
INDIRI4
CNSTI4 1
SUBI4
LTI4 $2259
line 3030
;3030:	}
LABELV $2235
line 3031
;3031:	if (g >= GT_TEAM ) {
ADDRLP4 1040
INDIRI4
CNSTI4 3
LTI4 $2273
line 3032
;3032:		trap_Cmd_ExecuteText( EXEC_APPEND, "wait 5; team Red\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2275
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3033
;3033:	}
LABELV $2273
line 3034
;3034:}
LABELV $2193
endproc UI_StartSkirmish 1160 32
proc UI_Update 36 8
line 3036
;3035:
;3036:static void UI_Update(const char *name) {
line 3037
;3037:	int	val = trap_Cvar_VariableValue(name);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 3039
;3038:
;3039: 	if (Q_stricmp(name, "ui_SetName") == 0) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2279
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2277
line 3040
;3040:		trap_Cvar_Set( "name", UI_Cvar_VariableString("ui_Name"));
ADDRGP4 $2280
ARGP4
ADDRLP4 12
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $1409
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3041
;3041: 	} else if (Q_stricmp(name, "ui_setRate") == 0) {
ADDRGP4 $2278
JUMPV
LABELV $2277
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2283
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2281
line 3042
;3042:		float rate = trap_Cvar_VariableValue("rate");
ADDRGP4 $2284
ARGP4
ADDRLP4 20
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 20
INDIRF4
ASGNF4
line 3043
;3043:		if (rate >= 5000) {
ADDRLP4 16
INDIRF4
CNSTF4 1167867904
LTF4 $2285
line 3044
;3044:			trap_Cvar_Set("cl_maxpackets", "30");
ADDRGP4 $2287
ARGP4
ADDRGP4 $689
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3045
;3045:			trap_Cvar_Set("cl_packetdup", "1");
ADDRGP4 $2288
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3046
;3046:		} else if (rate >= 4000) {
ADDRGP4 $2282
JUMPV
LABELV $2285
ADDRLP4 16
INDIRF4
CNSTF4 1165623296
LTF4 $2289
line 3047
;3047:			trap_Cvar_Set("cl_maxpackets", "15");
ADDRGP4 $2287
ARGP4
ADDRGP4 $692
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3048
;3048:			trap_Cvar_Set("cl_packetdup", "2");		// favor less prediction errors when there's packet loss
ADDRGP4 $2288
ARGP4
ADDRGP4 $2236
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3049
;3049:		} else {
ADDRGP4 $2282
JUMPV
LABELV $2289
line 3050
;3050:			trap_Cvar_Set("cl_maxpackets", "15");
ADDRGP4 $2287
ARGP4
ADDRGP4 $692
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3051
;3051:			trap_Cvar_Set("cl_packetdup", "1");		// favor lower bandwidth
ADDRGP4 $2288
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3052
;3052:		}
line 3053
;3053: 	} else if (Q_stricmp(name, "ui_GetName") == 0) {
ADDRGP4 $2282
JUMPV
LABELV $2281
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2293
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $2291
line 3054
;3054:		trap_Cvar_Set( "ui_Name", UI_Cvar_VariableString("name"));
ADDRGP4 $1409
ARGP4
ADDRLP4 20
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $2280
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3055
;3055: 	} else if (Q_stricmp(name, "r_colorbits") == 0) {
ADDRGP4 $2292
JUMPV
LABELV $2291
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2296
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $2294
line 3056
;3056:		switch (val) {
ADDRLP4 24
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 28
CNSTI4 16
ASGNI4
ADDRLP4 24
INDIRI4
ADDRLP4 28
INDIRI4
EQI4 $2302
ADDRLP4 24
INDIRI4
ADDRLP4 28
INDIRI4
GTI4 $2305
LABELV $2304
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2299
ADDRGP4 $2295
JUMPV
LABELV $2305
ADDRLP4 0
INDIRI4
CNSTI4 32
EQI4 $2303
ADDRGP4 $2295
JUMPV
LABELV $2299
line 3058
;3057:			case 0:
;3058:				trap_Cvar_SetValue( "r_depthbits", 0 );
ADDRGP4 $2300
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3059
;3059:				trap_Cvar_SetValue( "r_stencilbits", 0 );
ADDRGP4 $2301
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3060
;3060:			break;
ADDRGP4 $2295
JUMPV
LABELV $2302
line 3062
;3061:			case 16:
;3062:				trap_Cvar_SetValue( "r_depthbits", 16 );
ADDRGP4 $2300
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3063
;3063:				trap_Cvar_SetValue( "r_stencilbits", 0 );
ADDRGP4 $2301
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3064
;3064:			break;
ADDRGP4 $2295
JUMPV
LABELV $2303
line 3066
;3065:			case 32:
;3066:				trap_Cvar_SetValue( "r_depthbits", 24 );
ADDRGP4 $2300
ARGP4
CNSTF4 1103101952
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3067
;3067:			break;
line 3069
;3068:		}
;3069:	} else if (Q_stricmp(name, "r_lodbias") == 0) {
ADDRGP4 $2295
JUMPV
LABELV $2294
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2308
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $2306
line 3070
;3070:		switch (val) {
ADDRLP4 28
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $2311
ADDRLP4 28
INDIRI4
CNSTI4 1
EQI4 $2313
ADDRLP4 28
INDIRI4
CNSTI4 2
EQI4 $2314
ADDRGP4 $2307
JUMPV
LABELV $2311
line 3072
;3071:			case 0:
;3072:				trap_Cvar_SetValue( "r_subdivisions", 4 );
ADDRGP4 $2312
ARGP4
CNSTF4 1082130432
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3073
;3073:			break;
ADDRGP4 $2307
JUMPV
LABELV $2313
line 3075
;3074:			case 1:
;3075:				trap_Cvar_SetValue( "r_subdivisions", 12 );
ADDRGP4 $2312
ARGP4
CNSTF4 1094713344
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3076
;3076:			break;
ADDRGP4 $2307
JUMPV
LABELV $2314
line 3078
;3077:			case 2:
;3078:				trap_Cvar_SetValue( "r_subdivisions", 20 );
ADDRGP4 $2312
ARGP4
CNSTF4 1101004800
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3079
;3079:			break;
line 3081
;3080:		}
;3081:	} else if (Q_stricmp(name, "ui_glCustom") == 0) {
ADDRGP4 $2307
JUMPV
LABELV $2306
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2317
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $2315
line 3082
;3082:		switch (val) {
ADDRLP4 32
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
LTI4 $2316
ADDRLP4 32
INDIRI4
CNSTI4 3
GTI4 $2316
ADDRLP4 32
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $2336
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $2336
address $2320
address $2332
address $2333
address $2335
code
LABELV $2320
line 3084
;3083:			case 0:	// high quality
;3084:				trap_Cvar_SetValue( "r_fullScreen", 1 );
ADDRGP4 $2321
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3085
;3085:				trap_Cvar_SetValue( "r_subdivisions", 4 );
ADDRGP4 $2312
ARGP4
CNSTF4 1082130432
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3086
;3086:				trap_Cvar_SetValue( "r_vertexlight", 0 );
ADDRGP4 $2322
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3087
;3087:				trap_Cvar_SetValue( "r_lodbias", 0 );
ADDRGP4 $2308
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3088
;3088:				trap_Cvar_SetValue( "r_colorbits", 32 );
ADDRGP4 $2296
ARGP4
CNSTF4 1107296256
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3089
;3089:				trap_Cvar_SetValue( "r_depthbits", 24 );
ADDRGP4 $2300
ARGP4
CNSTF4 1103101952
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3090
;3090:				trap_Cvar_SetValue( "r_picmip", 0 );
ADDRGP4 $2323
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3091
;3091:				trap_Cvar_SetValue( "r_mode", 4 );
ADDRGP4 $2324
ARGP4
CNSTF4 1082130432
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3092
;3092:				trap_Cvar_SetValue( "r_texturebits", 32 );
ADDRGP4 $2325
ARGP4
CNSTF4 1107296256
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3093
;3093:				trap_Cvar_SetValue( "r_fastSky", 0 );
ADDRGP4 $2326
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3094
;3094:				trap_Cvar_SetValue( "r_inGameVideo", 1 );
ADDRGP4 $2327
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3095
;3095:				trap_Cvar_SetValue( "cg_shadows", 1 );
ADDRGP4 $2328
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3096
;3096:				trap_Cvar_SetValue( "cg_brassTime", 2500 );
ADDRGP4 $2329
ARGP4
CNSTF4 1159479296
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3097
;3097:				trap_Cvar_Set( "r_texturemode", "GL_LINEAR_MIPMAP_LINEAR" );
ADDRGP4 $2330
ARGP4
ADDRGP4 $2331
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3098
;3098:			break;
ADDRGP4 $2316
JUMPV
LABELV $2332
line 3100
;3099:			case 1: // normal 
;3100:				trap_Cvar_SetValue( "r_fullScreen", 1 );
ADDRGP4 $2321
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3101
;3101:				trap_Cvar_SetValue( "r_subdivisions", 12 );
ADDRGP4 $2312
ARGP4
CNSTF4 1094713344
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3102
;3102:				trap_Cvar_SetValue( "r_vertexlight", 0 );
ADDRGP4 $2322
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3103
;3103:				trap_Cvar_SetValue( "r_lodbias", 0 );
ADDRGP4 $2308
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3104
;3104:				trap_Cvar_SetValue( "r_colorbits", 0 );
ADDRGP4 $2296
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3105
;3105:				trap_Cvar_SetValue( "r_depthbits", 24 );
ADDRGP4 $2300
ARGP4
CNSTF4 1103101952
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3106
;3106:				trap_Cvar_SetValue( "r_picmip", 1 );
ADDRGP4 $2323
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3107
;3107:				trap_Cvar_SetValue( "r_mode", 3 );
ADDRGP4 $2324
ARGP4
CNSTF4 1077936128
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3108
;3108:				trap_Cvar_SetValue( "r_texturebits", 0 );
ADDRGP4 $2325
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3109
;3109:				trap_Cvar_SetValue( "r_fastSky", 0 );
ADDRGP4 $2326
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3110
;3110:				trap_Cvar_SetValue( "r_inGameVideo", 1 );
ADDRGP4 $2327
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3111
;3111:				trap_Cvar_SetValue( "cg_brassTime", 2500 );
ADDRGP4 $2329
ARGP4
CNSTF4 1159479296
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3112
;3112:				trap_Cvar_Set( "r_texturemode", "GL_LINEAR_MIPMAP_LINEAR" );
ADDRGP4 $2330
ARGP4
ADDRGP4 $2331
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3113
;3113:				trap_Cvar_SetValue( "cg_shadows", 0 );
ADDRGP4 $2328
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3114
;3114:			break;
ADDRGP4 $2316
JUMPV
LABELV $2333
line 3116
;3115:			case 2: // fast
;3116:				trap_Cvar_SetValue( "r_fullScreen", 1 );
ADDRGP4 $2321
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3117
;3117:				trap_Cvar_SetValue( "r_subdivisions", 8 );
ADDRGP4 $2312
ARGP4
CNSTF4 1090519040
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3118
;3118:				trap_Cvar_SetValue( "r_vertexlight", 0 );
ADDRGP4 $2322
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3119
;3119:				trap_Cvar_SetValue( "r_lodbias", 1 );
ADDRGP4 $2308
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3120
;3120:				trap_Cvar_SetValue( "r_colorbits", 0 );
ADDRGP4 $2296
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3121
;3121:				trap_Cvar_SetValue( "r_depthbits", 0 );
ADDRGP4 $2300
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3122
;3122:				trap_Cvar_SetValue( "r_picmip", 1 );
ADDRGP4 $2323
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3123
;3123:				trap_Cvar_SetValue( "r_mode", 3 );
ADDRGP4 $2324
ARGP4
CNSTF4 1077936128
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3124
;3124:				trap_Cvar_SetValue( "r_texturebits", 0 );
ADDRGP4 $2325
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3125
;3125:				trap_Cvar_SetValue( "cg_shadows", 0 );
ADDRGP4 $2328
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3126
;3126:				trap_Cvar_SetValue( "r_fastSky", 1 );
ADDRGP4 $2326
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3127
;3127:				trap_Cvar_SetValue( "r_inGameVideo", 0 );
ADDRGP4 $2327
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3128
;3128:				trap_Cvar_SetValue( "cg_brassTime", 0 );
ADDRGP4 $2329
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3129
;3129:				trap_Cvar_Set( "r_texturemode", "GL_LINEAR_MIPMAP_NEAREST" );
ADDRGP4 $2330
ARGP4
ADDRGP4 $2334
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3130
;3130:			break;
ADDRGP4 $2316
JUMPV
LABELV $2335
line 3132
;3131:			case 3: // fastest
;3132:				trap_Cvar_SetValue( "r_fullScreen", 1 );
ADDRGP4 $2321
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3133
;3133:				trap_Cvar_SetValue( "r_subdivisions", 20 );
ADDRGP4 $2312
ARGP4
CNSTF4 1101004800
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3134
;3134:				trap_Cvar_SetValue( "r_vertexlight", 1 );
ADDRGP4 $2322
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3135
;3135:				trap_Cvar_SetValue( "r_lodbias", 2 );
ADDRGP4 $2308
ARGP4
CNSTF4 1073741824
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3136
;3136:				trap_Cvar_SetValue( "r_colorbits", 16 );
ADDRGP4 $2296
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3137
;3137:				trap_Cvar_SetValue( "r_depthbits", 16 );
ADDRGP4 $2300
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3138
;3138:				trap_Cvar_SetValue( "r_mode", 3 );
ADDRGP4 $2324
ARGP4
CNSTF4 1077936128
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3139
;3139:				trap_Cvar_SetValue( "r_picmip", 2 );
ADDRGP4 $2323
ARGP4
CNSTF4 1073741824
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3140
;3140:				trap_Cvar_SetValue( "r_texturebits", 16 );
ADDRGP4 $2325
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3141
;3141:				trap_Cvar_SetValue( "cg_shadows", 0 );
ADDRGP4 $2328
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3142
;3142:				trap_Cvar_SetValue( "cg_brassTime", 0 );
ADDRGP4 $2329
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3143
;3143:				trap_Cvar_SetValue( "r_fastSky", 1 );
ADDRGP4 $2326
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3144
;3144:				trap_Cvar_SetValue( "r_inGameVideo", 0 );
ADDRGP4 $2327
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3145
;3145:				trap_Cvar_Set( "r_texturemode", "GL_LINEAR_MIPMAP_NEAREST" );
ADDRGP4 $2330
ARGP4
ADDRGP4 $2334
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3146
;3146:			break;
line 3148
;3147:		}
;3148:	} else if (Q_stricmp(name, "ui_mousePitch") == 0) {
ADDRGP4 $2316
JUMPV
LABELV $2315
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $2339
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $2337
line 3149
;3149:		if (val == 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2340
line 3150
;3150:			trap_Cvar_SetValue( "m_pitch", 0.022f );
ADDRGP4 $2342
ARGP4
CNSTF4 1018444120
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3151
;3151:		} else {
ADDRGP4 $2341
JUMPV
LABELV $2340
line 3152
;3152:			trap_Cvar_SetValue( "m_pitch", -0.022f );
ADDRGP4 $2342
ARGP4
CNSTF4 3165927768
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3153
;3153:		}
LABELV $2341
line 3154
;3154:	}
LABELV $2337
LABELV $2316
LABELV $2307
LABELV $2295
LABELV $2292
LABELV $2282
LABELV $2278
line 3155
;3155:}
LABELV $2276
endproc UI_Update 36 8
proc UI_RunMenuScript 1304 24
line 3157
;3156:
;3157:static void UI_RunMenuScript(char **args) {
line 3161
;3158:	const char *name, *name2;
;3159:	char buff[1024];
;3160:
;3161:	if (String_Parse(args, &name)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1024
ARGP4
ADDRLP4 1032
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
EQI4 $2344
line 3162
;3162:		if (Q_stricmp(name, "StartServer") == 0) {
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2348
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $2346
line 3165
;3163:			int i, clients, oldclients;
;3164:			float skill;
;3165:			trap_Cvar_Set("cg_thirdPerson", "0");
ADDRGP4 $393
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3166
;3166:			trap_Cvar_Set("cg_cameraOrbit", "0");
ADDRGP4 $391
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3167
;3167:			trap_Cvar_Set("ui_singlePlayerActive", "0");
ADDRGP4 $2209
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3168
;3168:			trap_Cvar_SetValue( "dedicated", Com_Clamp( 0, 2, ui_dedicated.integer ) );
CNSTF4 0
ARGF4
CNSTF4 1073741824
ARGF4
ADDRGP4 ui_dedicated+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 1056
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $2349
ARGP4
ADDRLP4 1056
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3169
;3169:			trap_Cvar_SetValue( "g_gametype", Com_Clamp( 0, 8, uiInfo.gameTypes[ui_netGameType.integer].gtEnum ) );
CNSTF4 0
ARGF4
CNSTF4 1090519040
ARGF4
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 1060
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $1323
ARGP4
ADDRLP4 1060
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 3170
;3170:			trap_Cvar_Set("g_redTeam", UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 1064
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $2224
ARGP4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3171
;3171:			trap_Cvar_Set("g_blueTeam", UI_Cvar_VariableString("ui_opponentName"));
ADDRGP4 $1099
ARGP4
ADDRLP4 1068
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $2225
ARGP4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3172
;3172:			trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", uiInfo.mapList[ui_currentNetMap.integer].mapLoadName ) );
ADDRGP4 $2202
ARGP4
CNSTI4 100
ADDRGP4 ui_currentNetMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1072
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1072
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3173
;3173:			skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $824
ARGP4
ADDRLP4 1076
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1048
ADDRLP4 1076
INDIRF4
ASGNF4
line 3175
;3174:			// set max clients based on spots
;3175:			oldclients = trap_Cvar_VariableValue( "sv_maxClients" );
ADDRGP4 $2218
ARGP4
ADDRLP4 1080
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1052
ADDRLP4 1080
INDIRF4
CVFI4 4
ASGNI4
line 3176
;3176:			clients = 0;
ADDRLP4 1044
CNSTI4 0
ASGNI4
line 3177
;3177:			for (i = 0; i < PLAYERS_PER_TEAM; i++) {
ADDRLP4 1040
CNSTI4 0
ASGNI4
LABELV $2357
line 3178
;3178:				int bot = trap_Cvar_VariableValue( va("ui_blueteam%i", i+1));
ADDRGP4 $846
ARGP4
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1088
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1088
INDIRP4
ARGP4
ADDRLP4 1092
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1084
ADDRLP4 1092
INDIRF4
CVFI4 4
ASGNI4
line 3179
;3179:				if (bot >= 0) {
ADDRLP4 1084
INDIRI4
CNSTI4 0
LTI4 $2361
line 3180
;3180:					clients++;
ADDRLP4 1044
ADDRLP4 1044
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3181
;3181:				}
LABELV $2361
line 3182
;3182:				bot = trap_Cvar_VariableValue( va("ui_redteam%i", i+1));
ADDRGP4 $847
ARGP4
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1096
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1096
INDIRP4
ARGP4
ADDRLP4 1100
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1084
ADDRLP4 1100
INDIRF4
CVFI4 4
ASGNI4
line 3183
;3183:				if (bot >= 0) {
ADDRLP4 1084
INDIRI4
CNSTI4 0
LTI4 $2363
line 3184
;3184:					clients++;
ADDRLP4 1044
ADDRLP4 1044
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3185
;3185:				}
LABELV $2363
line 3186
;3186:			}
LABELV $2358
line 3177
ADDRLP4 1040
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 5
LTI4 $2357
line 3187
;3187:			if (clients == 0) {
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2365
line 3188
;3188:				clients = 8;
ADDRLP4 1044
CNSTI4 8
ASGNI4
line 3189
;3189:			}
LABELV $2365
line 3191
;3190:			
;3191:			if (oldclients > clients) {
ADDRLP4 1052
INDIRI4
ADDRLP4 1044
INDIRI4
LEI4 $2367
line 3192
;3192:				clients = oldclients;
ADDRLP4 1044
ADDRLP4 1052
INDIRI4
ASGNI4
line 3193
;3193:			}
LABELV $2367
line 3195
;3194:
;3195:			trap_Cvar_Set("sv_maxClients", va("%d",clients));
ADDRGP4 $713
ARGP4
ADDRLP4 1044
INDIRI4
ARGI4
ADDRLP4 1084
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2218
ARGP4
ADDRLP4 1084
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3197
;3196:
;3197:			for (i = 0; i < PLAYERS_PER_TEAM; i++) {
ADDRLP4 1040
CNSTI4 0
ASGNI4
LABELV $2369
line 3198
;3198:				int bot = trap_Cvar_VariableValue( va("ui_blueteam%i", i+1));
ADDRGP4 $846
ARGP4
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1092
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1088
ADDRLP4 1096
INDIRF4
CVFI4 4
ASGNI4
line 3199
;3199:				if (bot > 1) {
ADDRLP4 1088
INDIRI4
CNSTI4 1
LEI4 $2373
line 3200
;3200:					if (ui_actualNetGameType.integer >= GT_TEAM) {
ADDRGP4 ui_actualNetGameType+12
INDIRI4
CNSTI4 3
LTI4 $2375
line 3201
;3201:						Com_sprintf( buff, sizeof(buff), "addbot %s %f %s\n", uiInfo.characterList[bot-2].name, skill, "Blue");
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2378
ARGP4
CNSTI4 24
ADDRLP4 1088
INDIRI4
MULI4
ADDRGP4 uiInfo+73448-48
ADDP4
INDIRP4
ARGP4
ADDRLP4 1048
INDIRF4
ARGF4
ADDRGP4 $839
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3202
;3202:					} else {
ADDRGP4 $2376
JUMPV
LABELV $2375
line 3203
;3203:						Com_sprintf( buff, sizeof(buff), "addbot %s %f \n", UI_GetBotNameByNumber(bot-2), skill);
ADDRLP4 1088
INDIRI4
CNSTI4 2
SUBI4
ARGI4
ADDRLP4 1100
ADDRGP4 UI_GetBotNameByNumber
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2381
ARGP4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 1048
INDIRF4
ARGF4
ADDRGP4 Com_sprintf
CALLV
pop
line 3204
;3204:					}
LABELV $2376
line 3205
;3205:					trap_Cmd_ExecuteText( EXEC_APPEND, buff );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3206
;3206:				}
LABELV $2373
line 3207
;3207:				bot = trap_Cvar_VariableValue( va("ui_redteam%i", i+1));
ADDRGP4 $847
ARGP4
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1100
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 1104
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1088
ADDRLP4 1104
INDIRF4
CVFI4 4
ASGNI4
line 3208
;3208:				if (bot > 1) {
ADDRLP4 1088
INDIRI4
CNSTI4 1
LEI4 $2382
line 3209
;3209:					if (ui_actualNetGameType.integer >= GT_TEAM) {
ADDRGP4 ui_actualNetGameType+12
INDIRI4
CNSTI4 3
LTI4 $2384
line 3210
;3210:						Com_sprintf( buff, sizeof(buff), "addbot %s %f %s\n", uiInfo.characterList[bot-2].name, skill, "Red");
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2378
ARGP4
CNSTI4 24
ADDRLP4 1088
INDIRI4
MULI4
ADDRGP4 uiInfo+73448-48
ADDP4
INDIRP4
ARGP4
ADDRLP4 1048
INDIRF4
ARGF4
ADDRGP4 $840
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3211
;3211:					} else {
ADDRGP4 $2385
JUMPV
LABELV $2384
line 3212
;3212:						Com_sprintf( buff, sizeof(buff), "addbot %s %f \n", UI_GetBotNameByNumber(bot-2), skill);
ADDRLP4 1088
INDIRI4
CNSTI4 2
SUBI4
ARGI4
ADDRLP4 1108
ADDRGP4 UI_GetBotNameByNumber
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2381
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRLP4 1048
INDIRF4
ARGF4
ADDRGP4 Com_sprintf
CALLV
pop
line 3213
;3213:					}
LABELV $2385
line 3214
;3214:					trap_Cmd_ExecuteText( EXEC_APPEND, buff );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3215
;3215:				}
LABELV $2382
line 3216
;3216:			}
LABELV $2370
line 3197
ADDRLP4 1040
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 5
LTI4 $2369
line 3217
;3217:		} else if (Q_stricmp(name, "updateSPMenu") == 0) {
ADDRGP4 $2347
JUMPV
LABELV $2346
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2391
ARGP4
ADDRLP4 1040
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $2389
line 3218
;3218:			UI_SetCapFragLimits(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_SetCapFragLimits
CALLV
pop
line 3219
;3219:			UI_MapCountByGameType(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_MapCountByGameType
CALLI4
pop
line 3220
;3220:			ui_mapIndex.integer = UI_GetIndexFromSelection(ui_currentMap.integer);
ADDRGP4 ui_currentMap+12
INDIRI4
ARGI4
ADDRLP4 1044
ADDRGP4 UI_GetIndexFromSelection
CALLI4
ASGNI4
ADDRGP4 ui_mapIndex+12
ADDRLP4 1044
INDIRI4
ASGNI4
line 3221
;3221:			trap_Cvar_Set("ui_mapIndex", va("%d", ui_mapIndex.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_mapIndex+12
INDIRI4
ARGI4
ADDRLP4 1048
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2196
ARGP4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3222
;3222:			Menu_SetFeederSelection(NULL, FEEDER_MAPS, ui_mapIndex.integer, "skirmish");
CNSTP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 ui_mapIndex+12
INDIRI4
ARGI4
ADDRGP4 $2192
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 3223
;3223:			UI_GameType_HandleKey(0, 0, K_MOUSE1, qfalse);
ADDRLP4 1052
CNSTI4 0
ASGNI4
ADDRLP4 1052
INDIRI4
ARGI4
CNSTP4 0
ARGP4
CNSTI4 178
ARGI4
ADDRLP4 1052
INDIRI4
ARGI4
ADDRGP4 UI_GameType_HandleKey
CALLI4
pop
line 3224
;3224:			UI_GameType_HandleKey(0, 0, K_MOUSE2, qfalse);
ADDRLP4 1056
CNSTI4 0
ASGNI4
ADDRLP4 1056
INDIRI4
ARGI4
CNSTP4 0
ARGP4
CNSTI4 179
ARGI4
ADDRLP4 1056
INDIRI4
ARGI4
ADDRGP4 UI_GameType_HandleKey
CALLI4
pop
line 3225
;3225:		} else if (Q_stricmp(name, "resetDefaults") == 0) {
ADDRGP4 $2390
JUMPV
LABELV $2389
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2398
ARGP4
ADDRLP4 1044
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2396
line 3226
;3226:			trap_Cmd_ExecuteText( EXEC_APPEND, "exec default.cfg\n");
CNSTI4 2
ARGI4
ADDRGP4 $2399
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3227
;3227:			trap_Cmd_ExecuteText( EXEC_APPEND, "cvar_restart\n");
CNSTI4 2
ARGI4
ADDRGP4 $2400
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3228
;3228:			Controls_SetDefaults();
ADDRGP4 Controls_SetDefaults
CALLV
pop
line 3229
;3229:			trap_Cvar_Set("com_introPlayed", "1" );
ADDRGP4 $2401
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3230
;3230:			trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2402
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3231
;3231:		} else if (Q_stricmp(name, "getCDKey") == 0) {
ADDRGP4 $2397
JUMPV
LABELV $2396
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2405
ARGP4
ADDRLP4 1048
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $2403
line 3233
;3232:			char out[17];
;3233:			trap_GetCDKey(buff, 17);
ADDRLP4 0
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 trap_GetCDKey
CALLV
pop
line 3234
;3234:			trap_Cvar_Set("cdkey1", "");
ADDRGP4 $2406
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3235
;3235:			trap_Cvar_Set("cdkey2", "");
ADDRGP4 $2407
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3236
;3236:			trap_Cvar_Set("cdkey3", "");
ADDRGP4 $2408
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3237
;3237:			trap_Cvar_Set("cdkey4", "");
ADDRGP4 $2409
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3238
;3238:			if (strlen(buff) == CDKEY_LEN) {
ADDRLP4 0
ARGP4
ADDRLP4 1072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 16
NEI4 $2404
line 3239
;3239:				Q_strncpyz(out, buff, 5);
ADDRLP4 1052
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 5
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3240
;3240:				trap_Cvar_Set("cdkey1", out);
ADDRGP4 $2406
ARGP4
ADDRLP4 1052
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3241
;3241:				Q_strncpyz(out, buff + 4, 5);
ADDRLP4 1052
ARGP4
ADDRLP4 0+4
ARGP4
CNSTI4 5
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3242
;3242:				trap_Cvar_Set("cdkey2", out);
ADDRGP4 $2407
ARGP4
ADDRLP4 1052
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3243
;3243:				Q_strncpyz(out, buff + 8, 5);
ADDRLP4 1052
ARGP4
ADDRLP4 0+8
ARGP4
CNSTI4 5
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3244
;3244:				trap_Cvar_Set("cdkey3", out);
ADDRGP4 $2408
ARGP4
ADDRLP4 1052
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3245
;3245:				Q_strncpyz(out, buff + 12, 5);
ADDRLP4 1052
ARGP4
ADDRLP4 0+12
ARGP4
CNSTI4 5
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3246
;3246:				trap_Cvar_Set("cdkey4", out);
ADDRGP4 $2409
ARGP4
ADDRLP4 1052
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3247
;3247:			}
line 3249
;3248:
;3249:		} else if (Q_stricmp(name, "verifyCDKey") == 0) {
ADDRGP4 $2404
JUMPV
LABELV $2403
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2417
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $2415
line 3250
;3250:			buff[0] = '\0';
ADDRLP4 0
CNSTI1 0
ASGNI1
line 3251
;3251:			Q_strcat(buff, 1024, UI_Cvar_VariableString("cdkey1")); 
ADDRGP4 $2406
ARGP4
ADDRLP4 1056
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 3252
;3252:			Q_strcat(buff, 1024, UI_Cvar_VariableString("cdkey2")); 
ADDRGP4 $2407
ARGP4
ADDRLP4 1060
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 3253
;3253:			Q_strcat(buff, 1024, UI_Cvar_VariableString("cdkey3")); 
ADDRGP4 $2408
ARGP4
ADDRLP4 1064
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 3254
;3254:			Q_strcat(buff, 1024, UI_Cvar_VariableString("cdkey4")); 
ADDRGP4 $2409
ARGP4
ADDRLP4 1068
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 3255
;3255:			trap_Cvar_Set("cdkey", buff);
ADDRGP4 $2418
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3256
;3256:			if (trap_VerifyCDKey(buff, UI_Cvar_VariableString("cdkeychecksum"))) {
ADDRGP4 $2421
ARGP4
ADDRLP4 1072
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 1072
INDIRP4
ARGP4
ADDRLP4 1076
ADDRGP4 trap_VerifyCDKey
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
EQI4 $2419
line 3257
;3257:				trap_Cvar_Set("ui_cdkeyvalid", "CD Key Appears to be valid.");
ADDRGP4 $2422
ARGP4
ADDRGP4 $2423
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3258
;3258:				trap_SetCDKey(buff);
ADDRLP4 0
ARGP4
ADDRGP4 trap_SetCDKey
CALLV
pop
line 3259
;3259:			} else {
ADDRGP4 $2416
JUMPV
LABELV $2419
line 3260
;3260:				trap_Cvar_Set("ui_cdkeyvalid", "CD Key does not appear to be valid.");
ADDRGP4 $2422
ARGP4
ADDRGP4 $2424
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3261
;3261:			}
line 3262
;3262:		} else if (Q_stricmp(name, "loadArenas") == 0) {
ADDRGP4 $2416
JUMPV
LABELV $2415
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2427
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $2425
line 3263
;3263:			UI_LoadArenas();
ADDRGP4 UI_LoadArenas
CALLV
pop
line 3264
;3264:			UI_MapCountByGameType(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_MapCountByGameType
CALLI4
pop
line 3265
;3265:			Menu_SetFeederSelection(NULL, FEEDER_ALLMAPS, 0, "createserver");
CNSTP4 0
ARGP4
CNSTI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 $2428
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 3266
;3266:		} else if (Q_stricmp(name, "saveControls") == 0) {
ADDRGP4 $2426
JUMPV
LABELV $2425
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2431
ARGP4
ADDRLP4 1060
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
NEI4 $2429
line 3267
;3267:			Controls_SetConfig(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 Controls_SetConfig
CALLV
pop
line 3268
;3268:		} else if (Q_stricmp(name, "loadControls") == 0) {
ADDRGP4 $2430
JUMPV
LABELV $2429
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2434
ARGP4
ADDRLP4 1064
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $2432
line 3269
;3269:			Controls_GetConfig();
ADDRGP4 Controls_GetConfig
CALLV
pop
line 3270
;3270:		} else if (Q_stricmp(name, "clearError") == 0) {
ADDRGP4 $2433
JUMPV
LABELV $2432
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2437
ARGP4
ADDRLP4 1068
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1068
INDIRI4
CNSTI4 0
NEI4 $2435
line 3271
;3271:			trap_Cvar_Set("com_errorMessage", "");
ADDRGP4 $2438
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3272
;3272:		} else if (Q_stricmp(name, "loadGameInfo") == 0) {
ADDRGP4 $2436
JUMPV
LABELV $2435
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2441
ARGP4
ADDRLP4 1072
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $2439
line 3276
;3273:#ifdef PRE_RELEASE_TADEMO
;3274:			UI_ParseGameInfo("demogameinfo.txt");
;3275:#else
;3276:			UI_ParseGameInfo("gameinfo.txt");
ADDRGP4 $674
ARGP4
ADDRGP4 UI_ParseGameInfo
CALLV
pop
line 3278
;3277:#endif
;3278:			UI_LoadBestScores(uiInfo.mapList[ui_currentMap.integer].mapLoadName, uiInfo.gameTypes[ui_gameType.integer].gtEnum);
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_LoadBestScores
CALLV
pop
line 3279
;3279:		} else if (Q_stricmp(name, "resetScores") == 0) {
ADDRGP4 $2440
JUMPV
LABELV $2439
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2450
ARGP4
ADDRLP4 1076
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1076
INDIRI4
CNSTI4 0
NEI4 $2448
line 3280
;3280:			UI_ClearScores();
ADDRGP4 UI_ClearScores
CALLV
pop
line 3281
;3281:		} else if (Q_stricmp(name, "RefreshServers") == 0) {
ADDRGP4 $2449
JUMPV
LABELV $2448
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2453
ARGP4
ADDRLP4 1080
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1080
INDIRI4
CNSTI4 0
NEI4 $2451
line 3282
;3282:			UI_StartServerRefresh(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_StartServerRefresh
CALLV
pop
line 3283
;3283:			UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 3284
;3284:		} else if (Q_stricmp(name, "RefreshFilter") == 0) {
ADDRGP4 $2452
JUMPV
LABELV $2451
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2456
ARGP4
ADDRLP4 1084
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1084
INDIRI4
CNSTI4 0
NEI4 $2454
line 3285
;3285:			UI_StartServerRefresh(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_StartServerRefresh
CALLV
pop
line 3286
;3286:			UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 3287
;3287:		} else if (Q_stricmp(name, "RunSPDemo") == 0) {
ADDRGP4 $2455
JUMPV
LABELV $2454
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2459
ARGP4
ADDRLP4 1088
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1088
INDIRI4
CNSTI4 0
NEI4 $2457
line 3288
;3288:			if (uiInfo.demoAvailable) {
ADDRGP4 uiInfo+73432
INDIRI4
CNSTI4 0
EQI4 $2458
line 3289
;3289:			  trap_Cmd_ExecuteText( EXEC_APPEND, va("demo %s_%i\n", uiInfo.mapList[ui_currentMap.integer].mapLoadName, uiInfo.gameTypes[ui_gameType.integer].gtEnum));
ADDRGP4 $2463
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRLP4 1092
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3290
;3290:			}
line 3291
;3291:		} else if (Q_stricmp(name, "LoadDemos") == 0) {
ADDRGP4 $2458
JUMPV
LABELV $2457
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2472
ARGP4
ADDRLP4 1092
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1092
INDIRI4
CNSTI4 0
NEI4 $2470
line 3292
;3292:			UI_LoadDemos();
ADDRGP4 UI_LoadDemos
CALLV
pop
line 3293
;3293:		} else if (Q_stricmp(name, "LoadMovies") == 0) {
ADDRGP4 $2471
JUMPV
LABELV $2470
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2475
ARGP4
ADDRLP4 1096
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 0
NEI4 $2473
line 3294
;3294:			UI_LoadMovies();
ADDRGP4 UI_LoadMovies
CALLV
pop
line 3295
;3295:		} else if (Q_stricmp(name, "LoadMods") == 0) {
ADDRGP4 $2474
JUMPV
LABELV $2473
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2478
ARGP4
ADDRLP4 1100
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1100
INDIRI4
CNSTI4 0
NEI4 $2476
line 3296
;3296:			UI_LoadMods();
ADDRGP4 UI_LoadMods
CALLV
pop
line 3297
;3297:		} else if (Q_stricmp(name, "playMovie") == 0) {
ADDRGP4 $2477
JUMPV
LABELV $2476
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2481
ARGP4
ADDRLP4 1104
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
NEI4 $2479
line 3298
;3298:			if (uiInfo.previewMovie >= 0) {
ADDRGP4 uiInfo+99260
INDIRI4
CNSTI4 0
LTI4 $2482
line 3299
;3299:			  trap_CIN_StopCinematic(uiInfo.previewMovie);
ADDRGP4 uiInfo+99260
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 3300
;3300:			}
LABELV $2482
line 3301
;3301:			trap_Cmd_ExecuteText( EXEC_APPEND, va("cinematic %s.roq 2\n", uiInfo.movieList[uiInfo.movieIndex]));
ADDRGP4 $2486
ARGP4
ADDRGP4 uiInfo+99256
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+98228
ADDP4
INDIRP4
ARGP4
ADDRLP4 1108
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3302
;3302:		} else if (Q_stricmp(name, "RunMod") == 0) {
ADDRGP4 $2480
JUMPV
LABELV $2479
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2491
ARGP4
ADDRLP4 1108
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $2489
line 3303
;3303:			trap_Cvar_Set( "fs_game", uiInfo.modList[uiInfo.modIndex].modName);
ADDRGP4 $2492
ARGP4
ADDRGP4 uiInfo+97192
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+96676
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3304
;3304:			trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart;" );
CNSTI4 2
ARGI4
ADDRGP4 $2495
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3305
;3305:		} else if (Q_stricmp(name, "RunDemo") == 0) {
ADDRGP4 $2490
JUMPV
LABELV $2489
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2498
ARGP4
ADDRLP4 1112
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
NEI4 $2496
line 3306
;3306:			trap_Cmd_ExecuteText( EXEC_APPEND, va("demo %s\n", uiInfo.demoList[uiInfo.demoIndex]));
ADDRGP4 $2499
ARGP4
ADDRGP4 uiInfo+98224
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+97196
ADDP4
INDIRP4
ARGP4
ADDRLP4 1116
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3307
;3307:		} else if (Q_stricmp(name, "Quake3") == 0) {
ADDRGP4 $2497
JUMPV
LABELV $2496
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2504
ARGP4
ADDRLP4 1116
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 0
NEI4 $2502
line 3308
;3308:			trap_Cvar_Set( "fs_game", "");
ADDRGP4 $2492
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3309
;3309:			trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart;" );
CNSTI4 2
ARGI4
ADDRGP4 $2495
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3310
;3310:		} else if (Q_stricmp(name, "closeJoin") == 0) {
ADDRGP4 $2503
JUMPV
LABELV $2502
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2507
ARGP4
ADDRLP4 1120
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1120
INDIRI4
CNSTI4 0
NEI4 $2505
line 3311
;3311:			if (uiInfo.serverStatus.refreshActive) {
ADDRGP4 uiInfo+99264+2212
INDIRI4
CNSTI4 0
EQI4 $2508
line 3312
;3312:				UI_StopServerRefresh();
ADDRGP4 UI_StopServerRefresh
CALLV
pop
line 3313
;3313:				uiInfo.serverStatus.nextDisplayRefresh = 0;
ADDRGP4 uiInfo+99264+10420
CNSTI4 0
ASGNI4
line 3314
;3314:				uiInfo.nextServerStatusRefresh = 0;
ADDRGP4 uiInfo+114144
CNSTI4 0
ASGNI4
line 3315
;3315:				uiInfo.nextFindPlayerRefresh = 0;
ADDRGP4 uiInfo+119472
CNSTI4 0
ASGNI4
line 3316
;3316:				UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 3317
;3317:			} else {
ADDRGP4 $2506
JUMPV
LABELV $2508
line 3318
;3318:				Menus_CloseByName("joinserver");
ADDRGP4 $2516
ARGP4
ADDRGP4 Menus_CloseByName
CALLV
pop
line 3319
;3319:				Menus_OpenByName("main");
ADDRGP4 $2517
ARGP4
ADDRGP4 Menus_OpenByName
CALLV
pop
line 3320
;3320:			}
line 3321
;3321:		} else if (Q_stricmp(name, "StopRefresh") == 0) {
ADDRGP4 $2506
JUMPV
LABELV $2505
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2520
ARGP4
ADDRLP4 1124
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1124
INDIRI4
CNSTI4 0
NEI4 $2518
line 3322
;3322:			UI_StopServerRefresh();
ADDRGP4 UI_StopServerRefresh
CALLV
pop
line 3323
;3323:			uiInfo.serverStatus.nextDisplayRefresh = 0;
ADDRGP4 uiInfo+99264+10420
CNSTI4 0
ASGNI4
line 3324
;3324:			uiInfo.nextServerStatusRefresh = 0;
ADDRGP4 uiInfo+114144
CNSTI4 0
ASGNI4
line 3325
;3325:			uiInfo.nextFindPlayerRefresh = 0;
ADDRGP4 uiInfo+119472
CNSTI4 0
ASGNI4
line 3326
;3326:		} else if (Q_stricmp(name, "UpdateFilter") == 0) {
ADDRGP4 $2519
JUMPV
LABELV $2518
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2527
ARGP4
ADDRLP4 1128
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1128
INDIRI4
CNSTI4 0
NEI4 $2525
line 3327
;3327:			if (ui_netSource.integer == AS_LOCAL) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
NEI4 $2528
line 3328
;3328:				UI_StartServerRefresh(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_StartServerRefresh
CALLV
pop
line 3329
;3329:			}
LABELV $2528
line 3330
;3330:			UI_BuildServerDisplayList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 3331
;3331:			UI_FeederSelection(FEEDER_SERVERS, 0);
CNSTF4 1073741824
ARGF4
CNSTI4 0
ARGI4
ADDRGP4 UI_FeederSelection
CALLV
pop
line 3332
;3332:		} else if (Q_stricmp(name, "ServerStatus") == 0) {
ADDRGP4 $2526
JUMPV
LABELV $2525
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2533
ARGP4
ADDRLP4 1132
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1132
INDIRI4
CNSTI4 0
NEI4 $2531
line 3333
;3333:			trap_LAN_GetServerAddressString(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.serverStatus.currentServer], uiInfo.serverStatusAddress, sizeof(uiInfo.serverStatusAddress));
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2216
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRGP4 uiInfo+110748
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_LAN_GetServerAddressString
CALLV
pop
line 3334
;3334:			UI_BuildServerStatus(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerStatus
CALLV
pop
line 3335
;3335:		} else if (Q_stricmp(name, "FoundPlayerServerStatus") == 0) {
ADDRGP4 $2532
JUMPV
LABELV $2531
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2543
ARGP4
ADDRLP4 1136
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1136
INDIRI4
CNSTI4 0
NEI4 $2541
line 3336
;3336:			Q_strncpyz(uiInfo.serverStatusAddress, uiInfo.foundPlayerServerAddresses[uiInfo.currentFoundPlayerServer], sizeof(uiInfo.serverStatusAddress));
ADDRGP4 uiInfo+110748
ARGP4
ADDRGP4 uiInfo+119464
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+117416
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3337
;3337:			UI_BuildServerStatus(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerStatus
CALLV
pop
line 3338
;3338:			Menu_SetFeederSelection(NULL, FEEDER_FINDPLAYER, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 14
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 3339
;3339:		} else if (Q_stricmp(name, "FindPlayer") == 0) {
ADDRGP4 $2542
JUMPV
LABELV $2541
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2550
ARGP4
ADDRLP4 1140
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1140
INDIRI4
CNSTI4 0
NEI4 $2548
line 3340
;3340:			UI_BuildFindPlayerList(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildFindPlayerList
CALLV
pop
line 3342
;3341:			// clear the displayed server status info
;3342:			uiInfo.serverStatusInfo.numLines = 0;
ADDRGP4 uiInfo+110812+3328
CNSTI4 0
ASGNI4
line 3343
;3343:			Menu_SetFeederSelection(NULL, FEEDER_FINDPLAYER, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 14
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 3344
;3344:		} else if (Q_stricmp(name, "JoinServer") == 0) {
ADDRGP4 $2549
JUMPV
LABELV $2548
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2555
ARGP4
ADDRLP4 1144
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1144
INDIRI4
CNSTI4 0
NEI4 $2553
line 3345
;3345:			trap_Cvar_Set("cg_thirdPerson", "0");
ADDRGP4 $393
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3346
;3346:			trap_Cvar_Set("cg_cameraOrbit", "0");
ADDRGP4 $391
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3347
;3347:			trap_Cvar_Set("ui_singlePlayerActive", "0");
ADDRGP4 $2209
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3348
;3348:			if (uiInfo.serverStatus.currentServer >= 0 && uiInfo.serverStatus.currentServer < uiInfo.serverStatus.numDisplayServers) {
ADDRGP4 uiInfo+99264+2216
INDIRI4
CNSTI4 0
LTI4 $2554
ADDRGP4 uiInfo+99264+2216
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
GEI4 $2554
line 3349
;3349:				trap_LAN_GetServerAddressString(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.serverStatus.currentServer], buff, 1024);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2216
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerAddressString
CALLV
pop
line 3350
;3350:				trap_Cmd_ExecuteText( EXEC_APPEND, va( "connect %s\n", buff ) );
ADDRGP4 $2569
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1148
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3351
;3351:			}
line 3352
;3352:		} else if (Q_stricmp(name, "FoundPlayerJoinServer") == 0) {
ADDRGP4 $2554
JUMPV
LABELV $2553
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2572
ARGP4
ADDRLP4 1148
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1148
INDIRI4
CNSTI4 0
NEI4 $2570
line 3353
;3353:			trap_Cvar_Set("ui_singlePlayerActive", "0");
ADDRGP4 $2209
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3354
;3354:			if (uiInfo.currentFoundPlayerServer >= 0 && uiInfo.currentFoundPlayerServer < uiInfo.numFoundPlayerServers) {
ADDRGP4 uiInfo+119464
INDIRI4
CNSTI4 0
LTI4 $2571
ADDRGP4 uiInfo+119464
INDIRI4
ADDRGP4 uiInfo+119468
INDIRI4
GEI4 $2571
line 3355
;3355:				trap_Cmd_ExecuteText( EXEC_APPEND, va( "connect %s\n", uiInfo.foundPlayerServerAddresses[uiInfo.currentFoundPlayerServer] ) );
ADDRGP4 $2569
ARGP4
ADDRGP4 uiInfo+119464
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+117416
ADDP4
ARGP4
ADDRLP4 1152
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1152
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3356
;3356:			}
line 3357
;3357:		} else if (Q_stricmp(name, "Quit") == 0) {
ADDRGP4 $2571
JUMPV
LABELV $2570
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2582
ARGP4
ADDRLP4 1152
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1152
INDIRI4
CNSTI4 0
NEI4 $2580
line 3358
;3358:			trap_Cvar_Set("ui_singlePlayerActive", "0");
ADDRGP4 $2209
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3359
;3359:			trap_Cmd_ExecuteText( EXEC_NOW, "quit");
CNSTI4 0
ARGI4
ADDRGP4 $2583
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3360
;3360:		} else if (Q_stricmp(name, "Controls") == 0) {
ADDRGP4 $2581
JUMPV
LABELV $2580
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2586
ARGP4
ADDRLP4 1156
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1156
INDIRI4
CNSTI4 0
NEI4 $2584
line 3361
;3361:		  trap_Cvar_Set( "cl_paused", "1" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3362
;3362:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3363
;3363:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3364
;3364:			Menus_ActivateByName("setup_menu2");
ADDRGP4 $2588
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 3365
;3365:		} else if (Q_stricmp(name, "Leave") == 0) {
ADDRGP4 $2585
JUMPV
LABELV $2584
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2591
ARGP4
ADDRLP4 1160
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1160
INDIRI4
CNSTI4 0
NEI4 $2589
line 3366
;3366:			trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2592
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3367
;3367:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3368
;3368:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3369
;3369:			Menus_ActivateByName("main");
ADDRGP4 $2517
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 3370
;3370:		} else if (Q_stricmp(name, "ServerSort") == 0) {
ADDRGP4 $2590
JUMPV
LABELV $2589
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2595
ARGP4
ADDRLP4 1164
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1164
INDIRI4
CNSTI4 0
NEI4 $2593
line 3372
;3371:			int sortColumn;
;3372:			if (Int_Parse(args, &sortColumn)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1168
ARGP4
ADDRLP4 1172
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 1172
INDIRI4
CNSTI4 0
EQI4 $2594
line 3374
;3373:				// if same column we're already sorting on then flip the direction
;3374:				if (sortColumn == uiInfo.serverStatus.sortKey) {
ADDRLP4 1168
INDIRI4
ADDRGP4 uiInfo+99264+2200
INDIRI4
NEI4 $2598
line 3375
;3375:					uiInfo.serverStatus.sortDir = !uiInfo.serverStatus.sortDir;
ADDRGP4 uiInfo+99264+2204
INDIRI4
CNSTI4 0
NEI4 $2607
ADDRLP4 1176
CNSTI4 1
ASGNI4
ADDRGP4 $2608
JUMPV
LABELV $2607
ADDRLP4 1176
CNSTI4 0
ASGNI4
LABELV $2608
ADDRGP4 uiInfo+99264+2204
ADDRLP4 1176
INDIRI4
ASGNI4
line 3376
;3376:				}
LABELV $2598
line 3378
;3377:				// make sure we sort again
;3378:				UI_ServersSort(sortColumn, qtrue);
ADDRLP4 1168
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 UI_ServersSort
CALLV
pop
line 3379
;3379:			}
line 3380
;3380:		} else if (Q_stricmp(name, "nextSkirmish") == 0) {
ADDRGP4 $2594
JUMPV
LABELV $2593
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2611
ARGP4
ADDRLP4 1168
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1168
INDIRI4
CNSTI4 0
NEI4 $2609
line 3381
;3381:			UI_StartSkirmish(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_StartSkirmish
CALLV
pop
line 3382
;3382:		} else if (Q_stricmp(name, "SkirmishStart") == 0) {
ADDRGP4 $2610
JUMPV
LABELV $2609
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2614
ARGP4
ADDRLP4 1172
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1172
INDIRI4
CNSTI4 0
NEI4 $2612
line 3383
;3383:			UI_StartSkirmish(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_StartSkirmish
CALLV
pop
line 3384
;3384:		} else if (Q_stricmp(name, "closeingame") == 0) {
ADDRGP4 $2613
JUMPV
LABELV $2612
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2617
ARGP4
ADDRLP4 1176
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1176
INDIRI4
CNSTI4 0
NEI4 $2615
line 3385
;3385:			trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 1180
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 1180
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3386
;3386:			trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 3387
;3387:			trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3388
;3388:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3389
;3389:		} else if (Q_stricmp(name, "voteMap") == 0) {
ADDRGP4 $2616
JUMPV
LABELV $2615
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2620
ARGP4
ADDRLP4 1180
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1180
INDIRI4
CNSTI4 0
NEI4 $2618
line 3390
;3390:			if (ui_currentNetMap.integer >=0 && ui_currentNetMap.integer < uiInfo.mapCount) {
ADDRGP4 ui_currentNetMap+12
INDIRI4
CNSTI4 0
LTI4 $2619
ADDRGP4 ui_currentNetMap+12
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
GEI4 $2619
line 3391
;3391:				trap_Cmd_ExecuteText( EXEC_APPEND, va("callvote map %s\n",uiInfo.mapList[ui_currentNetMap.integer].mapLoadName) );
ADDRGP4 $2626
ARGP4
CNSTI4 100
ADDRGP4 ui_currentNetMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1184
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1184
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3392
;3392:			}
line 3393
;3393:		} else if (Q_stricmp(name, "voteKick") == 0) {
ADDRGP4 $2619
JUMPV
LABELV $2618
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2632
ARGP4
ADDRLP4 1184
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1184
INDIRI4
CNSTI4 0
NEI4 $2630
line 3394
;3394:			if (uiInfo.playerIndex >= 0 && uiInfo.playerIndex < uiInfo.playerCount) {
ADDRGP4 uiInfo+78860
INDIRI4
CNSTI4 0
LTI4 $2631
ADDRGP4 uiInfo+78860
INDIRI4
ADDRGP4 uiInfo+78844
INDIRI4
GEI4 $2631
line 3395
;3395:				trap_Cmd_ExecuteText( EXEC_APPEND, va("callvote kick %s\n",uiInfo.playerNames[uiInfo.playerIndex]) );
ADDRGP4 $2638
ARGP4
ADDRGP4 uiInfo+78860
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+78872
ADDP4
ARGP4
ADDRLP4 1188
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1188
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3396
;3396:			}
line 3397
;3397:		} else if (Q_stricmp(name, "voteGame") == 0) {
ADDRGP4 $2631
JUMPV
LABELV $2630
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2643
ARGP4
ADDRLP4 1188
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1188
INDIRI4
CNSTI4 0
NEI4 $2641
line 3398
;3398:			if (ui_netGameType.integer >= 0 && ui_netGameType.integer < uiInfo.numGameTypes) {
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 0
LTI4 $2642
ADDRGP4 ui_netGameType+12
INDIRI4
ADDRGP4 uiInfo+78576
INDIRI4
GEI4 $2642
line 3399
;3399:				trap_Cmd_ExecuteText( EXEC_APPEND, va("callvote g_gametype %i\n",uiInfo.gameTypes[ui_netGameType.integer].gtEnum) );
ADDRGP4 $2649
ARGP4
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRLP4 1192
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1192
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3400
;3400:			}
line 3401
;3401:		} else if (Q_stricmp(name, "voteLeader") == 0) {
ADDRGP4 $2642
JUMPV
LABELV $2641
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2655
ARGP4
ADDRLP4 1192
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1192
INDIRI4
CNSTI4 0
NEI4 $2653
line 3402
;3402:			if (uiInfo.teamIndex >= 0 && uiInfo.teamIndex < uiInfo.myTeamCount) {
ADDRGP4 uiInfo+78852
INDIRI4
CNSTI4 0
LTI4 $2654
ADDRGP4 uiInfo+78852
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $2654
line 3403
;3403:				trap_Cmd_ExecuteText( EXEC_APPEND, va("callteamvote leader %s\n",uiInfo.teamNames[uiInfo.teamIndex]) );
ADDRGP4 $2661
ARGP4
ADDRGP4 uiInfo+78852
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRLP4 1196
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1196
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3404
;3404:			}
line 3405
;3405:		} else if (Q_stricmp(name, "addBot") == 0) {
ADDRGP4 $2654
JUMPV
LABELV $2653
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2666
ARGP4
ADDRLP4 1196
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1196
INDIRI4
CNSTI4 0
NEI4 $2664
line 3406
;3406:			if (trap_Cvar_VariableValue("g_gametype") >= GT_TEAM) {
ADDRGP4 $1323
ARGP4
ADDRLP4 1200
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1200
INDIRF4
CNSTF4 1077936128
LTF4 $2667
line 3407
;3407:				trap_Cmd_ExecuteText( EXEC_APPEND, va("addbot %s %i %s\n", uiInfo.characterList[uiInfo.botIndex].name, uiInfo.skillIndex+1, (uiInfo.redBlue == 0) ? "Red" : "Blue") );
ADDRGP4 $2669
ARGP4
CNSTI4 24
ADDRGP4 uiInfo+73444
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ARGP4
ADDRGP4 uiInfo+96672
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 uiInfo+78840
INDIRI4
CNSTI4 0
NEI4 $2675
ADDRLP4 1204
ADDRGP4 $840
ASGNP4
ADDRGP4 $2676
JUMPV
LABELV $2675
ADDRLP4 1204
ADDRGP4 $839
ASGNP4
LABELV $2676
ADDRLP4 1204
INDIRP4
ARGP4
ADDRLP4 1208
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1208
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3408
;3408:			} else {
ADDRGP4 $2665
JUMPV
LABELV $2667
line 3409
;3409:				trap_Cmd_ExecuteText( EXEC_APPEND, va("addbot %s %i %s\n", UI_GetBotNameByNumber(uiInfo.botIndex), uiInfo.skillIndex+1, (uiInfo.redBlue == 0) ? "Red" : "Blue") );
ADDRGP4 uiInfo+73444
INDIRI4
ARGI4
ADDRLP4 1208
ADDRGP4 UI_GetBotNameByNumber
CALLP4
ASGNP4
ADDRGP4 $2669
ARGP4
ADDRLP4 1208
INDIRP4
ARGP4
ADDRGP4 uiInfo+96672
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 uiInfo+78840
INDIRI4
CNSTI4 0
NEI4 $2681
ADDRLP4 1204
ADDRGP4 $840
ASGNP4
ADDRGP4 $2682
JUMPV
LABELV $2681
ADDRLP4 1204
ADDRGP4 $839
ASGNP4
LABELV $2682
ADDRLP4 1204
INDIRP4
ARGP4
ADDRLP4 1212
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1212
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3410
;3410:			}
line 3411
;3411:		} else if (Q_stricmp(name, "addFavorite") == 0) {
ADDRGP4 $2665
JUMPV
LABELV $2664
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2685
ARGP4
ADDRLP4 1200
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1200
INDIRI4
CNSTI4 0
NEI4 $2683
line 3412
;3412:			if (ui_netSource.integer != AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
EQI4 $2684
line 3417
;3413:				char name[MAX_NAME_LENGTH];
;3414:				char addr[MAX_NAME_LENGTH];
;3415:				int res;
;3416:
;3417:				trap_LAN_GetServerInfo(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.serverStatus.currentServer], buff, MAX_STRING_CHARS);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2216
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 3418
;3418:				name[0] = addr[0] = '\0';
ADDRLP4 1272
CNSTI1 0
ASGNI1
ADDRLP4 1204
ADDRLP4 1272
INDIRI1
ASGNI1
ADDRLP4 1236
ADDRLP4 1272
INDIRI1
ASGNI1
line 3419
;3419:				Q_strncpyz(name, 	Info_ValueForKey(buff, "hostname"), MAX_NAME_LENGTH);
ADDRLP4 0
ARGP4
ADDRGP4 $2694
ARGP4
ADDRLP4 1276
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1236
ARGP4
ADDRLP4 1276
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3420
;3420:				Q_strncpyz(addr, 	Info_ValueForKey(buff, "addr"), MAX_NAME_LENGTH);
ADDRLP4 0
ARGP4
ADDRGP4 $2695
ARGP4
ADDRLP4 1280
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1204
ARGP4
ADDRLP4 1280
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3421
;3421:				if (strlen(name) > 0 && strlen(addr) > 0) {
ADDRLP4 1236
ARGP4
ADDRLP4 1284
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1284
INDIRI4
CNSTI4 0
LEI4 $2684
ADDRLP4 1204
ARGP4
ADDRLP4 1288
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1288
INDIRI4
CNSTI4 0
LEI4 $2684
line 3422
;3422:					res = trap_LAN_AddServer(AS_FAVORITES, name, addr);
CNSTI4 3
ARGI4
ADDRLP4 1236
ARGP4
ADDRLP4 1204
ARGP4
ADDRLP4 1292
ADDRGP4 trap_LAN_AddServer
CALLI4
ASGNI4
ADDRLP4 1268
ADDRLP4 1292
INDIRI4
ASGNI4
line 3423
;3423:					if (res == 0) {
ADDRLP4 1268
INDIRI4
CNSTI4 0
NEI4 $2698
line 3425
;3424:						// server already in the list
;3425:						Com_Printf("Favorite already in list\n");
ADDRGP4 $2700
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3426
;3426:					}
ADDRGP4 $2684
JUMPV
LABELV $2698
line 3427
;3427:					else if (res == -1) {
ADDRLP4 1268
INDIRI4
CNSTI4 -1
NEI4 $2701
line 3429
;3428:						// list full
;3429:						Com_Printf("Favorite list full\n");
ADDRGP4 $2703
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3430
;3430:					}
ADDRGP4 $2684
JUMPV
LABELV $2701
line 3431
;3431:					else {
line 3433
;3432:						// successfully added
;3433:						Com_Printf("Added favorite server %s\n", addr);
ADDRGP4 $2704
ARGP4
ADDRLP4 1204
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3434
;3434:					}
line 3435
;3435:				}
line 3436
;3436:			}
line 3437
;3437:		} else if (Q_stricmp(name, "deleteFavorite") == 0) {
ADDRGP4 $2684
JUMPV
LABELV $2683
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2707
ARGP4
ADDRLP4 1204
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1204
INDIRI4
CNSTI4 0
NEI4 $2705
line 3438
;3438:			if (ui_netSource.integer == AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
NEI4 $2706
line 3440
;3439:				char addr[MAX_NAME_LENGTH];
;3440:				trap_LAN_GetServerInfo(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.serverStatus.currentServer], buff, MAX_STRING_CHARS);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2216
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 3441
;3441:				addr[0] = '\0';
ADDRLP4 1208
CNSTI1 0
ASGNI1
line 3442
;3442:				Q_strncpyz(addr, 	Info_ValueForKey(buff, "addr"), MAX_NAME_LENGTH);
ADDRLP4 0
ARGP4
ADDRGP4 $2695
ARGP4
ADDRLP4 1240
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1208
ARGP4
ADDRLP4 1240
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3443
;3443:				if (strlen(addr) > 0) {
ADDRLP4 1208
ARGP4
ADDRLP4 1244
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1244
INDIRI4
CNSTI4 0
LEI4 $2706
line 3444
;3444:					trap_LAN_RemoveServer(AS_FAVORITES, addr);
CNSTI4 3
ARGI4
ADDRLP4 1208
ARGP4
ADDRGP4 trap_LAN_RemoveServer
CALLV
pop
line 3445
;3445:				}
line 3446
;3446:			}
line 3447
;3447:		} else if (Q_stricmp(name, "createFavorite") == 0) {
ADDRGP4 $2706
JUMPV
LABELV $2705
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2720
ARGP4
ADDRLP4 1208
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1208
INDIRI4
CNSTI4 0
NEI4 $2718
line 3448
;3448:			if (ui_netSource.integer == AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
NEI4 $2719
line 3453
;3449:				char name[MAX_NAME_LENGTH];
;3450:				char addr[MAX_NAME_LENGTH];
;3451:				int res;
;3452:
;3453:				name[0] = addr[0] = '\0';
ADDRLP4 1280
CNSTI1 0
ASGNI1
ADDRLP4 1212
ADDRLP4 1280
INDIRI1
ASGNI1
ADDRLP4 1244
ADDRLP4 1280
INDIRI1
ASGNI1
line 3454
;3454:				Q_strncpyz(name, 	UI_Cvar_VariableString("ui_favoriteName"), MAX_NAME_LENGTH);
ADDRGP4 $2724
ARGP4
ADDRLP4 1284
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 1244
ARGP4
ADDRLP4 1284
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3455
;3455:				Q_strncpyz(addr, 	UI_Cvar_VariableString("ui_favoriteAddress"), MAX_NAME_LENGTH);
ADDRGP4 $2725
ARGP4
ADDRLP4 1288
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 1212
ARGP4
ADDRLP4 1288
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3456
;3456:				if (strlen(name) > 0 && strlen(addr) > 0) {
ADDRLP4 1244
ARGP4
ADDRLP4 1292
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1292
INDIRI4
CNSTI4 0
LEI4 $2719
ADDRLP4 1212
ARGP4
ADDRLP4 1296
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1296
INDIRI4
CNSTI4 0
LEI4 $2719
line 3457
;3457:					res = trap_LAN_AddServer(AS_FAVORITES, name, addr);
CNSTI4 3
ARGI4
ADDRLP4 1244
ARGP4
ADDRLP4 1212
ARGP4
ADDRLP4 1300
ADDRGP4 trap_LAN_AddServer
CALLI4
ASGNI4
ADDRLP4 1276
ADDRLP4 1300
INDIRI4
ASGNI4
line 3458
;3458:					if (res == 0) {
ADDRLP4 1276
INDIRI4
CNSTI4 0
NEI4 $2728
line 3460
;3459:						// server already in the list
;3460:						Com_Printf("Favorite already in list\n");
ADDRGP4 $2700
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3461
;3461:					}
ADDRGP4 $2719
JUMPV
LABELV $2728
line 3462
;3462:					else if (res == -1) {
ADDRLP4 1276
INDIRI4
CNSTI4 -1
NEI4 $2730
line 3464
;3463:						// list full
;3464:						Com_Printf("Favorite list full\n");
ADDRGP4 $2703
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3465
;3465:					}
ADDRGP4 $2719
JUMPV
LABELV $2730
line 3466
;3466:					else {
line 3468
;3467:						// successfully added
;3468:						Com_Printf("Added favorite server %s\n", addr);
ADDRGP4 $2704
ARGP4
ADDRLP4 1212
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3469
;3469:					}
line 3470
;3470:				}
line 3471
;3471:			}
line 3472
;3472:		} else if (Q_stricmp(name, "orders") == 0) {
ADDRGP4 $2719
JUMPV
LABELV $2718
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2734
ARGP4
ADDRLP4 1212
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1212
INDIRI4
CNSTI4 0
NEI4 $2732
line 3474
;3473:			const char *orders;
;3474:			if (String_Parse(args, &orders)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1216
ARGP4
ADDRLP4 1220
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1220
INDIRI4
CNSTI4 0
EQI4 $2733
line 3475
;3475:				int selectedPlayer = trap_Cvar_VariableValue("cg_selectedPlayer");
ADDRGP4 $1391
ARGP4
ADDRLP4 1228
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1224
ADDRLP4 1228
INDIRF4
CVFI4 4
ASGNI4
line 3476
;3476:				if (selectedPlayer < uiInfo.myTeamCount) {
ADDRLP4 1224
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $2737
line 3477
;3477:					strcpy(buff, orders);
ADDRLP4 0
ARGP4
ADDRLP4 1216
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 3478
;3478:					trap_Cmd_ExecuteText( EXEC_APPEND, va(buff, uiInfo.teamClientNums[selectedPlayer]) );
ADDRLP4 0
ARGP4
ADDRLP4 1224
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+82968
ADDP4
INDIRI4
ARGI4
ADDRLP4 1232
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1232
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3479
;3479:					trap_Cmd_ExecuteText( EXEC_APPEND, "\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2741
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3480
;3480:				} else {
ADDRGP4 $2738
JUMPV
LABELV $2737
line 3482
;3481:					int i;
;3482:					for (i = 0; i < uiInfo.myTeamCount; i++) {
ADDRLP4 1232
CNSTI4 0
ASGNI4
ADDRGP4 $2745
JUMPV
LABELV $2742
line 3483
;3483:						if (Q_stricmp(UI_Cvar_VariableString("name"), uiInfo.teamNames[i]) == 0) {
ADDRGP4 $1409
ARGP4
ADDRLP4 1236
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 1236
INDIRP4
ARGP4
ADDRLP4 1232
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRLP4 1240
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1240
INDIRI4
CNSTI4 0
NEI4 $2747
line 3484
;3484:							continue;
ADDRGP4 $2743
JUMPV
LABELV $2747
line 3486
;3485:						}
;3486:						strcpy(buff, orders);
ADDRLP4 0
ARGP4
ADDRLP4 1216
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 3487
;3487:						trap_Cmd_ExecuteText( EXEC_APPEND, va(buff, uiInfo.teamNames[i]) );
ADDRLP4 0
ARGP4
ADDRLP4 1232
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
ARGP4
ADDRLP4 1244
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1244
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3488
;3488:						trap_Cmd_ExecuteText( EXEC_APPEND, "\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2741
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3489
;3489:					}
LABELV $2743
line 3482
ADDRLP4 1232
ADDRLP4 1232
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2745
ADDRLP4 1232
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
LTI4 $2742
line 3490
;3490:				}
LABELV $2738
line 3491
;3491:				trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 1232
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3492
;3492:				trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 3493
;3493:				trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3494
;3494:				Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3495
;3495:			}
line 3496
;3496:		} else if (Q_stricmp(name, "voiceOrdersTeam") == 0) {
ADDRGP4 $2733
JUMPV
LABELV $2732
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2753
ARGP4
ADDRLP4 1216
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1216
INDIRI4
CNSTI4 0
NEI4 $2751
line 3498
;3497:			const char *orders;
;3498:			if (String_Parse(args, &orders)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1220
ARGP4
ADDRLP4 1224
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1224
INDIRI4
CNSTI4 0
EQI4 $2752
line 3499
;3499:				int selectedPlayer = trap_Cvar_VariableValue("cg_selectedPlayer");
ADDRGP4 $1391
ARGP4
ADDRLP4 1232
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1228
ADDRLP4 1232
INDIRF4
CVFI4 4
ASGNI4
line 3500
;3500:				if (selectedPlayer == uiInfo.myTeamCount) {
ADDRLP4 1228
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
NEI4 $2756
line 3501
;3501:					trap_Cmd_ExecuteText( EXEC_APPEND, orders );
CNSTI4 2
ARGI4
ADDRLP4 1220
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3502
;3502:					trap_Cmd_ExecuteText( EXEC_APPEND, "\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2741
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3503
;3503:				}
LABELV $2756
line 3504
;3504:				trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 1236
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 1236
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3505
;3505:				trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 3506
;3506:				trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3507
;3507:				Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3508
;3508:			}
line 3509
;3509:		} else if (Q_stricmp(name, "voiceOrders") == 0) {
ADDRGP4 $2752
JUMPV
LABELV $2751
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2761
ARGP4
ADDRLP4 1220
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1220
INDIRI4
CNSTI4 0
NEI4 $2759
line 3511
;3510:			const char *orders;
;3511:			if (String_Parse(args, &orders)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1224
ARGP4
ADDRLP4 1228
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1228
INDIRI4
CNSTI4 0
EQI4 $2760
line 3512
;3512:				int selectedPlayer = trap_Cvar_VariableValue("cg_selectedPlayer");
ADDRGP4 $1391
ARGP4
ADDRLP4 1236
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1232
ADDRLP4 1236
INDIRF4
CVFI4 4
ASGNI4
line 3513
;3513:				if (selectedPlayer < uiInfo.myTeamCount) {
ADDRLP4 1232
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $2764
line 3514
;3514:					strcpy(buff, orders);
ADDRLP4 0
ARGP4
ADDRLP4 1224
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 3515
;3515:					trap_Cmd_ExecuteText( EXEC_APPEND, va(buff, uiInfo.teamClientNums[selectedPlayer]) );
ADDRLP4 0
ARGP4
ADDRLP4 1232
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+82968
ADDP4
INDIRI4
ARGI4
ADDRLP4 1240
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 1240
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3516
;3516:					trap_Cmd_ExecuteText( EXEC_APPEND, "\n" );
CNSTI4 2
ARGI4
ADDRGP4 $2741
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 3517
;3517:				}
LABELV $2764
line 3518
;3518:				trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 1240
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 1240
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 3519
;3519:				trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 3520
;3520:				trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3521
;3521:				Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 3522
;3522:			}
line 3523
;3523:		} else if (Q_stricmp(name, "glCustom") == 0) {
ADDRGP4 $2760
JUMPV
LABELV $2759
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2770
ARGP4
ADDRLP4 1224
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1224
INDIRI4
CNSTI4 0
NEI4 $2768
line 3524
;3524:			trap_Cvar_Set("ui_glCustom", "4");
ADDRGP4 $2317
ARGP4
ADDRGP4 $2771
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 3525
;3525:		} else if (Q_stricmp(name, "update") == 0) {
ADDRGP4 $2769
JUMPV
LABELV $2768
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2774
ARGP4
ADDRLP4 1228
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1228
INDIRI4
CNSTI4 0
NEI4 $2772
line 3526
;3526:			if (String_Parse(args, &name2)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1028
ARGP4
ADDRLP4 1232
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 0
EQI4 $2773
line 3527
;3527:				UI_Update(name2);
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 UI_Update
CALLV
pop
line 3528
;3528:			}
line 3529
;3529:		} else if (Q_stricmp(name, "setPbClStatus") == 0) {
ADDRGP4 $2773
JUMPV
LABELV $2772
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 $2779
ARGP4
ADDRLP4 1232
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1232
INDIRI4
CNSTI4 0
NEI4 $2777
line 3531
;3530:			int stat;
;3531:			if ( Int_Parse( args, &stat ) )
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1236
ARGP4
ADDRLP4 1240
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 1240
INDIRI4
CNSTI4 0
EQI4 $2778
line 3532
;3532:				trap_SetPbClStatus( stat );
ADDRLP4 1236
INDIRI4
ARGI4
ADDRGP4 trap_SetPbClStatus
CALLV
pop
line 3533
;3533:		}
ADDRGP4 $2778
JUMPV
LABELV $2777
line 3534
;3534:		else {
line 3535
;3535:			Com_Printf("unknown UI script %s\n", name);
ADDRGP4 $2782
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 3536
;3536:		}
LABELV $2778
LABELV $2773
LABELV $2769
LABELV $2760
LABELV $2752
LABELV $2733
LABELV $2719
LABELV $2706
LABELV $2684
LABELV $2665
LABELV $2654
LABELV $2642
LABELV $2631
LABELV $2619
LABELV $2616
LABELV $2613
LABELV $2610
LABELV $2594
LABELV $2590
LABELV $2585
LABELV $2581
LABELV $2571
LABELV $2554
LABELV $2549
LABELV $2542
LABELV $2532
LABELV $2526
LABELV $2519
LABELV $2506
LABELV $2503
LABELV $2497
LABELV $2490
LABELV $2480
LABELV $2477
LABELV $2474
LABELV $2471
LABELV $2458
LABELV $2455
LABELV $2452
LABELV $2449
LABELV $2440
LABELV $2436
LABELV $2433
LABELV $2430
LABELV $2426
LABELV $2416
LABELV $2404
LABELV $2397
LABELV $2390
LABELV $2347
line 3537
;3537:	}
LABELV $2344
line 3538
;3538:}
LABELV $2343
endproc UI_RunMenuScript 1304 24
proc UI_GetTeamColor 0 0
line 3540
;3539:
;3540:static void UI_GetTeamColor(vec4_t *color) {
line 3541
;3541:}
LABELV $2783
endproc UI_GetTeamColor 0 0
proc UI_MapCountByGameType 16 0
line 3548
;3542:
;3543:/*
;3544:==================
;3545:UI_MapCountByGameType
;3546:==================
;3547:*/
;3548:static int UI_MapCountByGameType(qboolean singlePlayer) {
line 3550
;3549:	int i, c, game;
;3550:	c = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 3551
;3551:	game = singlePlayer ? uiInfo.gameTypes[ui_gameType.integer].gtEnum : uiInfo.gameTypes[ui_netGameType.integer].gtEnum;
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $2792
ADDRLP4 12
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $2793
JUMPV
LABELV $2792
ADDRLP4 12
ADDRGP4 ui_netGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ASGNI4
LABELV $2793
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
line 3552
;3552:	if (game == GT_SINGLE_PLAYER) {
ADDRLP4 4
INDIRI4
CNSTI4 2
NEI4 $2794
line 3553
;3553:		game++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3554
;3554:	} 
LABELV $2794
line 3555
;3555:	if (game == GT_TEAM) {
ADDRLP4 4
INDIRI4
CNSTI4 3
NEI4 $2796
line 3556
;3556:		game = GT_FFA;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 3557
;3557:	}
LABELV $2796
line 3559
;3558:
;3559:	for (i = 0; i < uiInfo.mapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2801
JUMPV
LABELV $2798
line 3560
;3560:		uiInfo.mapList[i].active = qfalse;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+96
ADDP4
CNSTI4 0
ASGNI4
line 3561
;3561:		if ( uiInfo.mapList[i].typeBits & (1 << game)) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $2805
line 3562
;3562:			if (singlePlayer) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $2809
line 3563
;3563:				if (!(uiInfo.mapList[i].typeBits & (1 << GT_SINGLE_PLAYER))) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $2811
line 3564
;3564:					continue;
ADDRGP4 $2799
JUMPV
LABELV $2811
line 3566
;3565:				}
;3566:			}
LABELV $2809
line 3567
;3567:			c++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3568
;3568:			uiInfo.mapList[i].active = qtrue;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+96
ADDP4
CNSTI4 1
ASGNI4
line 3569
;3569:		}
LABELV $2805
line 3570
;3570:	}
LABELV $2799
line 3559
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2801
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LTI4 $2798
line 3571
;3571:	return c;
ADDRLP4 8
INDIRI4
RETI4
LABELV $2784
endproc UI_MapCountByGameType 16 0
export UI_hasSkinForBase
proc UI_hasSkinForBase 1032 20
line 3574
;3572:}
;3573:
;3574:qboolean UI_hasSkinForBase(const char *base, const char *team) {
line 3577
;3575:	char	test[1024];
;3576:	
;3577:	Com_sprintf( test, sizeof( test ), "models/players/%s/%s/lower_default.skin", base, team );
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2818
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3579
;3578:
;3579:	if (trap_FS_FOpenFile(test, 0, FS_READ)) {
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1024
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
EQI4 $2819
line 3580
;3580:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2817
JUMPV
LABELV $2819
line 3582
;3581:	}
;3582:	Com_sprintf( test, sizeof( test ), "models/players/characters/%s/%s/lower_default.skin", base, team );
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $2821
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 3584
;3583:
;3584:	if (trap_FS_FOpenFile(test, 0, FS_READ)) {
ADDRLP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1028
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 0
EQI4 $2822
line 3585
;3585:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2817
JUMPV
LABELV $2822
line 3587
;3586:	}
;3587:	return qfalse;
CNSTI4 0
RETI4
LABELV $2817
endproc UI_hasSkinForBase 1032 20
data
align 4
LABELV $2825
byte 4 0
code
proc UI_HeadCountByTeam 40 8
line 3595
;3588:}
;3589:
;3590:/*
;3591:==================
;3592:UI_MapCountByTeam
;3593:==================
;3594:*/
;3595:static int UI_HeadCountByTeam() {
line 3599
;3596:	static int init = 0;
;3597:	int i, j, k, c, tIndex;
;3598:	
;3599:	c = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 3600
;3600:	if (!init) {
ADDRGP4 $2825
INDIRI4
CNSTI4 0
NEI4 $2826
line 3601
;3601:		for (i = 0; i < uiInfo.characterCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2831
JUMPV
LABELV $2828
line 3602
;3602:			uiInfo.characterList[i].reference = 0;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+20
ADDP4
CNSTI4 0
ASGNI4
line 3603
;3603:			for (j = 0; j < uiInfo.teamCount; j++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $2838
JUMPV
LABELV $2835
line 3604
;3604:			  if (UI_hasSkinForBase(uiInfo.characterList[i].base, uiInfo.teamList[j].teamName)) {
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
INDIRP4
ARGP4
CNSTI4 44
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 UI_hasSkinForBase
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $2840
line 3605
;3605:					uiInfo.characterList[i].reference |= (1<<j);
ADDRLP4 24
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+20
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 8
INDIRI4
LSHI4
BORI4
ASGNI4
line 3606
;3606:			  }
LABELV $2840
line 3607
;3607:			}
LABELV $2836
line 3603
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2838
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
LTI4 $2835
line 3608
;3608:		}
LABELV $2829
line 3601
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2831
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $2828
line 3609
;3609:		init = 1;
ADDRGP4 $2825
CNSTI4 1
ASGNI4
line 3610
;3610:	}
LABELV $2826
line 3612
;3611:
;3612:	tIndex = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 20
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 24
INDIRI4
ASGNI4
line 3615
;3613:
;3614:	// do names
;3615:	for (i = 0; i < uiInfo.characterCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2850
JUMPV
LABELV $2847
line 3616
;3616:		uiInfo.characterList[i].active = qfalse;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+16
ADDP4
CNSTI4 0
ASGNI4
line 3617
;3617:		for(j = 0; j < TEAM_MEMBERS; j++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $2854
line 3618
;3618:			if (uiInfo.teamList[tIndex].teamMembers[j] != NULL) {
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 12
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2858
line 3619
;3619:				if (uiInfo.characterList[i].reference&(1<<tIndex)) {// && Q_stricmp(uiInfo.teamList[tIndex].teamMembers[j], uiInfo.characterList[i].name)==0) {
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+20
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 12
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $2862
line 3620
;3620:					uiInfo.characterList[i].active = qtrue;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+16
ADDP4
CNSTI4 1
ASGNI4
line 3621
;3621:					c++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3622
;3622:					break;
ADDRGP4 $2856
JUMPV
LABELV $2862
line 3624
;3623:				}
;3624:			}
LABELV $2858
line 3625
;3625:		}
LABELV $2855
line 3617
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 5
LTI4 $2854
LABELV $2856
line 3626
;3626:	}
LABELV $2848
line 3615
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2850
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $2847
line 3629
;3627:
;3628:	// and then aliases
;3629:	for(j = 0; j < TEAM_MEMBERS; j++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $2868
line 3630
;3630:		for(k = 0; k < uiInfo.aliasCount; k++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $2875
JUMPV
LABELV $2872
line 3631
;3631:			if (uiInfo.aliasList[k].name != NULL) {
CNSTI4 12
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2877
line 3632
;3632:				if (Q_stricmp(uiInfo.teamList[tIndex].teamMembers[j], uiInfo.aliasList[k].name)==0) {
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRLP4 12
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
INDIRP4
ARGP4
CNSTI4 12
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $2880
line 3633
;3633:					for (i = 0; i < uiInfo.characterCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2888
JUMPV
LABELV $2885
line 3634
;3634:						if (uiInfo.characterList[i].headImage != -1 && uiInfo.characterList[i].reference&(1<<tIndex) && Q_stricmp(uiInfo.aliasList[k].ai, uiInfo.characterList[i].name)==0) {
ADDRLP4 32
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+73448+8
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $2890
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+73448+20
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 12
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $2890
CNSTI4 12
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+74988+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $2890
line 3635
;3635:							if (uiInfo.characterList[i].active == qfalse) {
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+16
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2887
line 3636
;3636:								uiInfo.characterList[i].active = qtrue;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+16
ADDP4
CNSTI4 1
ASGNI4
line 3637
;3637:								c++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3638
;3638:							}
line 3639
;3639:							break;
ADDRGP4 $2887
JUMPV
LABELV $2890
line 3641
;3640:						}
;3641:					}
LABELV $2886
line 3633
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2888
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $2885
LABELV $2887
line 3642
;3642:				}
LABELV $2880
line 3643
;3643:			}
LABELV $2877
line 3644
;3644:		}
LABELV $2873
line 3630
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2875
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
LTI4 $2872
line 3645
;3645:	}
LABELV $2869
line 3629
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 5
LTI4 $2868
line 3646
;3646:	return c;
ADDRLP4 16
INDIRI4
RETI4
LABELV $2824
endproc UI_HeadCountByTeam 40 8
proc UI_InsertServerIntoDisplayList 16 0
line 3654
;3647:}
;3648:
;3649:/*
;3650:==================
;3651:UI_InsertServerIntoDisplayList
;3652:==================
;3653:*/
;3654:static void UI_InsertServerIntoDisplayList(int num, int position) {
line 3657
;3655:	int i;
;3656:
;3657:	if (position < 0 || position > uiInfo.serverStatus.numDisplayServers ) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $2910
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
LEI4 $2906
LABELV $2910
line 3658
;3658:		return;
ADDRGP4 $2905
JUMPV
LABELV $2906
line 3661
;3659:	}
;3660:	//
;3661:	uiInfo.serverStatus.numDisplayServers++;
ADDRLP4 8
ADDRGP4 uiInfo+99264+10412
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3662
;3662:	for (i = uiInfo.serverStatus.numDisplayServers; i > position; i--) {
ADDRLP4 0
ADDRGP4 uiInfo+99264+10412
INDIRI4
ASGNI4
ADDRGP4 $2916
JUMPV
LABELV $2913
line 3663
;3663:		uiInfo.serverStatus.displayServers[i] = uiInfo.serverStatus.displayServers[i-1];
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+99264+2220
ADDP4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+99264+2220-4
ADDP4
INDIRI4
ASGNI4
line 3664
;3664:	}
LABELV $2914
line 3662
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $2916
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
GTI4 $2913
line 3665
;3665:	uiInfo.serverStatus.displayServers[position] = num;
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 3666
;3666:}
LABELV $2905
endproc UI_InsertServerIntoDisplayList 16 0
proc UI_RemoveServerFromDisplayList 16 0
line 3673
;3667:
;3668:/*
;3669:==================
;3670:UI_RemoveServerFromDisplayList
;3671:==================
;3672:*/
;3673:static void UI_RemoveServerFromDisplayList(int num) {
line 3676
;3674:	int i, j;
;3675:
;3676:	for (i = 0; i < uiInfo.serverStatus.numDisplayServers; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $2930
JUMPV
LABELV $2927
line 3677
;3677:		if (uiInfo.serverStatus.displayServers[i] == num) {
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $2933
line 3678
;3678:			uiInfo.serverStatus.numDisplayServers--;
ADDRLP4 8
ADDRGP4 uiInfo+99264+10412
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 3679
;3679:			for (j = i; j < uiInfo.serverStatus.numDisplayServers; j++) {
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $2942
JUMPV
LABELV $2939
line 3680
;3680:				uiInfo.serverStatus.displayServers[j] = uiInfo.serverStatus.displayServers[j+1];
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+99264+2220
ADDP4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+99264+2220+4
ADDP4
INDIRI4
ASGNI4
line 3681
;3681:			}
LABELV $2940
line 3679
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2942
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
LTI4 $2939
line 3682
;3682:			return;
ADDRGP4 $2926
JUMPV
LABELV $2933
line 3684
;3683:		}
;3684:	}
LABELV $2928
line 3676
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2930
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
LTI4 $2927
line 3685
;3685:}
LABELV $2926
endproc UI_RemoveServerFromDisplayList 16 0
proc UI_BinaryServerInsertion 20 20
line 3692
;3686:
;3687:/*
;3688:==================
;3689:UI_BinaryServerInsertion
;3690:==================
;3691:*/
;3692:static void UI_BinaryServerInsertion(int num) {
line 3696
;3693:	int mid, offset, res, len;
;3694:
;3695:	// use binary search to insert server
;3696:	len = uiInfo.serverStatus.numDisplayServers;
ADDRLP4 12
ADDRGP4 uiInfo+99264+10412
INDIRI4
ASGNI4
line 3697
;3697:	mid = len;
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 3698
;3698:	offset = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 3699
;3699:	res = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $2954
JUMPV
LABELV $2953
line 3700
;3700:	while(mid > 0) {
line 3701
;3701:		mid = len >> 1;
ADDRLP4 0
ADDRLP4 12
INDIRI4
CNSTI4 1
RSHI4
ASGNI4
line 3703
;3702:		//
;3703:		res = trap_LAN_CompareServers( ui_netSource.integer, uiInfo.serverStatus.sortKey,
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2200
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+2204
INDIRI4
ARGI4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 trap_LAN_CompareServers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 3706
;3704:					uiInfo.serverStatus.sortDir, num, uiInfo.serverStatus.displayServers[offset+mid]);
;3705:		// if equal
;3706:		if (res == 0) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2963
line 3707
;3707:			UI_InsertServerIntoDisplayList(num, offset+mid);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ARGI4
ADDRGP4 UI_InsertServerIntoDisplayList
CALLV
pop
line 3708
;3708:			return;
ADDRGP4 $2950
JUMPV
LABELV $2963
line 3711
;3709:		}
;3710:		// if larger
;3711:		else if (res == 1) {
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $2965
line 3712
;3712:			offset += mid;
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 3713
;3713:			len -= mid;
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 3714
;3714:		}
ADDRGP4 $2966
JUMPV
LABELV $2965
line 3716
;3715:		// if smaller
;3716:		else {
line 3717
;3717:			len -= mid;
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 3718
;3718:		}
LABELV $2966
line 3719
;3719:	}
LABELV $2954
line 3700
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $2953
line 3720
;3720:	if (res == 1) {
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $2967
line 3721
;3721:		offset++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3722
;3722:	}
LABELV $2967
line 3723
;3723:	UI_InsertServerIntoDisplayList(num, offset);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 UI_InsertServerIntoDisplayList
CALLV
pop
line 3724
;3724:}
LABELV $2950
endproc UI_BinaryServerInsertion 20 20
bss
align 4
LABELV $2970
skip 4
code
proc UI_BuildServerDisplayList 1100 16
line 3731
;3725:
;3726:/*
;3727:==================
;3728:UI_BuildServerDisplayList
;3729:==================
;3730:*/
;3731:static void UI_BuildServerDisplayList(qboolean force) {
line 3737
;3732:	int i, count, clients, maxClients, ping, game, len, visible;
;3733:	char info[MAX_STRING_CHARS];
;3734://	qboolean startRefresh = qtrue; TTimo: unused
;3735:	static int numinvisible;
;3736:
;3737:	if (!(force || uiInfo.uiDC.realTime > uiInfo.serverStatus.nextDisplayRefresh)) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $2971
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+99264+10420
INDIRI4
GTI4 $2971
line 3738
;3738:		return;
ADDRGP4 $2969
JUMPV
LABELV $2971
line 3741
;3739:	}
;3740:	// if we shouldn't reset
;3741:	if ( force == 2 ) {
ADDRFP4 0
INDIRI4
CNSTI4 2
NEI4 $2976
line 3742
;3742:		force = 0;
ADDRFP4 0
CNSTI4 0
ASGNI4
line 3743
;3743:	}
LABELV $2976
line 3746
;3744:
;3745:	// do motd updates here too
;3746:	trap_Cvar_VariableStringBuffer( "cl_motdString", uiInfo.serverStatus.motd, sizeof(uiInfo.serverStatus.motd) );
ADDRGP4 $2978
ARGP4
ADDRGP4 uiInfo+99264+10460
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 3747
;3747:	len = strlen(uiInfo.serverStatus.motd);
ADDRGP4 uiInfo+99264+10460
ARGP4
ADDRLP4 1056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1052
ADDRLP4 1056
INDIRI4
ASGNI4
line 3748
;3748:	if (len == 0) {
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $2985
line 3749
;3749:		strcpy(uiInfo.serverStatus.motd, "Welcome to Team Arena!");
ADDRGP4 uiInfo+99264+10460
ARGP4
ADDRGP4 $2989
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 3750
;3750:		len = strlen(uiInfo.serverStatus.motd);
ADDRGP4 uiInfo+99264+10460
ARGP4
ADDRLP4 1060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1052
ADDRLP4 1060
INDIRI4
ASGNI4
line 3751
;3751:	} 
LABELV $2985
line 3752
;3752:	if (len != uiInfo.serverStatus.motdLen) {
ADDRLP4 1052
INDIRI4
ADDRGP4 uiInfo+99264+10436
INDIRI4
EQI4 $2992
line 3753
;3753:		uiInfo.serverStatus.motdLen = len;
ADDRGP4 uiInfo+99264+10436
ADDRLP4 1052
INDIRI4
ASGNI4
line 3754
;3754:		uiInfo.serverStatus.motdWidth = -1;
ADDRGP4 uiInfo+99264+10440
CNSTI4 -1
ASGNI4
line 3755
;3755:	} 
LABELV $2992
line 3757
;3756:
;3757:	if (force) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $3000
line 3758
;3758:		numinvisible = 0;
ADDRGP4 $2970
CNSTI4 0
ASGNI4
line 3760
;3759:		// clear number of displayed servers
;3760:		uiInfo.serverStatus.numDisplayServers = 0;
ADDRGP4 uiInfo+99264+10412
CNSTI4 0
ASGNI4
line 3761
;3761:		uiInfo.serverStatus.numPlayersOnServers = 0;
ADDRGP4 uiInfo+99264+10416
CNSTI4 0
ASGNI4
line 3763
;3762:		// set list box index to zero
;3763:		Menu_SetFeederSelection(NULL, FEEDER_SERVERS, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 2
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 3765
;3764:		// mark all servers as visible so we store ping updates for them
;3765:		trap_LAN_MarkServerVisible(ui_netSource.integer, -1, qtrue);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
CNSTI4 -1
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3766
;3766:	}
LABELV $3000
line 3769
;3767:
;3768:	// get the server count (comes from the master)
;3769:	count = trap_LAN_GetServerCount(ui_netSource.integer);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 1060
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 1036
ADDRLP4 1060
INDIRI4
ASGNI4
line 3770
;3770:	if (count == -1 || (ui_netSource.integer == AS_LOCAL && count == 0) ) {
ADDRLP4 1036
INDIRI4
CNSTI4 -1
EQI4 $3011
ADDRLP4 1068
CNSTI4 0
ASGNI4
ADDRGP4 ui_netSource+12
INDIRI4
ADDRLP4 1068
INDIRI4
NEI4 $3008
ADDRLP4 1036
INDIRI4
ADDRLP4 1068
INDIRI4
NEI4 $3008
LABELV $3011
line 3772
;3771:		// still waiting on a response from the master
;3772:		uiInfo.serverStatus.numDisplayServers = 0;
ADDRGP4 uiInfo+99264+10412
CNSTI4 0
ASGNI4
line 3773
;3773:		uiInfo.serverStatus.numPlayersOnServers = 0;
ADDRGP4 uiInfo+99264+10416
CNSTI4 0
ASGNI4
line 3774
;3774:		uiInfo.serverStatus.nextDisplayRefresh = uiInfo.uiDC.realTime + 500;
ADDRGP4 uiInfo+99264+10420
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 3775
;3775:		return;
ADDRGP4 $2969
JUMPV
LABELV $3008
line 3778
;3776:	}
;3777:
;3778:	visible = qfalse;
ADDRLP4 1040
CNSTI4 0
ASGNI4
line 3779
;3779:	for (i = 0; i < count; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3022
JUMPV
LABELV $3019
line 3781
;3780:		// if we already got info for this server
;3781:		if (!trap_LAN_ServerIsVisible(ui_netSource.integer, i)) {
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1072
ADDRGP4 trap_LAN_ServerIsVisible
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $3023
line 3782
;3782:			continue;
ADDRGP4 $3020
JUMPV
LABELV $3023
line 3784
;3783:		}
;3784:		visible = qtrue;
ADDRLP4 1040
CNSTI4 1
ASGNI4
line 3786
;3785:		// get the ping for this server
;3786:		ping = trap_LAN_GetServerPing(ui_netSource.integer, i);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1076
ADDRGP4 trap_LAN_GetServerPing
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 1076
INDIRI4
ASGNI4
line 3787
;3787:		if (ping > 0 || ui_netSource.integer == AS_FAVORITES) {
ADDRLP4 4
INDIRI4
CNSTI4 0
GTI4 $3030
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
NEI4 $3027
LABELV $3030
line 3789
;3788:
;3789:			trap_LAN_GetServerInfo(ui_netSource.integer, i, info, MAX_STRING_CHARS);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 3791
;3790:
;3791:			clients = atoi(Info_ValueForKey(info, "clients"));
ADDRLP4 8
ARGP4
ADDRGP4 $3032
ARGP4
ADDRLP4 1080
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1080
INDIRP4
ARGP4
ADDRLP4 1084
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 1084
INDIRI4
ASGNI4
line 3792
;3792:			uiInfo.serverStatus.numPlayersOnServers += clients;
ADDRLP4 1088
ADDRGP4 uiInfo+99264+10416
ASGNP4
ADDRLP4 1088
INDIRP4
ADDRLP4 1088
INDIRP4
INDIRI4
ADDRLP4 1032
INDIRI4
ADDI4
ASGNI4
line 3794
;3793:
;3794:			if (ui_browserShowEmpty.integer == 0) {
ADDRGP4 ui_browserShowEmpty+12
INDIRI4
CNSTI4 0
NEI4 $3035
line 3795
;3795:				if (clients == 0) {
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $3038
line 3796
;3796:					trap_LAN_MarkServerVisible(ui_netSource.integer, i, qfalse);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3797
;3797:					continue;
ADDRGP4 $3020
JUMPV
LABELV $3038
line 3799
;3798:				}
;3799:			}
LABELV $3035
line 3801
;3800:
;3801:			if (ui_browserShowFull.integer == 0) {
ADDRGP4 ui_browserShowFull+12
INDIRI4
CNSTI4 0
NEI4 $3041
line 3802
;3802:				maxClients = atoi(Info_ValueForKey(info, "sv_maxclients"));
ADDRLP4 8
ARGP4
ADDRGP4 $1360
ARGP4
ADDRLP4 1092
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1044
ADDRLP4 1096
INDIRI4
ASGNI4
line 3803
;3803:				if (clients == maxClients) {
ADDRLP4 1032
INDIRI4
ADDRLP4 1044
INDIRI4
NEI4 $3044
line 3804
;3804:					trap_LAN_MarkServerVisible(ui_netSource.integer, i, qfalse);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3805
;3805:					continue;
ADDRGP4 $3020
JUMPV
LABELV $3044
line 3807
;3806:				}
;3807:			}
LABELV $3041
line 3809
;3808:
;3809:			if (uiInfo.joinGameTypes[ui_joinGameType.integer].gtEnum != -1) {
ADDRGP4 ui_joinGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78712+4
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $3047
line 3810
;3810:				game = atoi(Info_ValueForKey(info, "gametype"));
ADDRLP4 8
ARGP4
ADDRGP4 $3052
ARGP4
ADDRLP4 1092
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1048
ADDRLP4 1096
INDIRI4
ASGNI4
line 3811
;3811:				if (game != uiInfo.joinGameTypes[ui_joinGameType.integer].gtEnum) {
ADDRLP4 1048
INDIRI4
ADDRGP4 ui_joinGameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78712+4
ADDP4
INDIRI4
EQI4 $3053
line 3812
;3812:					trap_LAN_MarkServerVisible(ui_netSource.integer, i, qfalse);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3813
;3813:					continue;
ADDRGP4 $3020
JUMPV
LABELV $3053
line 3815
;3814:				}
;3815:			}
LABELV $3047
line 3817
;3816:				
;3817:			if (ui_serverFilterType.integer > 0) {
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 0
LEI4 $3059
line 3818
;3818:				if (Q_stricmp(Info_ValueForKey(info, "game"), serverFilters[ui_serverFilterType.integer].basedir) != 0) {
ADDRLP4 8
ARGP4
ADDRGP4 $3064
ARGP4
ADDRLP4 1092
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1092
INDIRP4
ARGP4
ADDRGP4 ui_serverFilterType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverFilters+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 0
EQI4 $3062
line 3819
;3819:					trap_LAN_MarkServerVisible(ui_netSource.integer, i, qfalse);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3820
;3820:					continue;
ADDRGP4 $3020
JUMPV
LABELV $3062
line 3822
;3821:				}
;3822:			}
LABELV $3059
line 3824
;3823:			// make sure we never add a favorite server twice
;3824:			if (ui_netSource.integer == AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
NEI4 $3068
line 3825
;3825:				UI_RemoveServerFromDisplayList(i);
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 UI_RemoveServerFromDisplayList
CALLV
pop
line 3826
;3826:			}
LABELV $3068
line 3828
;3827:			// insert the server into the list
;3828:			UI_BinaryServerInsertion(i);
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 UI_BinaryServerInsertion
CALLV
pop
line 3830
;3829:			// done with this server
;3830:			if (ping > 0) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LEI4 $3071
line 3831
;3831:				trap_LAN_MarkServerVisible(ui_netSource.integer, i, qfalse);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 3832
;3832:				numinvisible++;
ADDRLP4 1092
ADDRGP4 $2970
ASGNP4
ADDRLP4 1092
INDIRP4
ADDRLP4 1092
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3833
;3833:			}
LABELV $3071
line 3834
;3834:		}
LABELV $3027
line 3835
;3835:	}
LABELV $3020
line 3779
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3022
ADDRLP4 0
INDIRI4
ADDRLP4 1036
INDIRI4
LTI4 $3019
line 3837
;3836:
;3837:	uiInfo.serverStatus.refreshtime = uiInfo.uiDC.realTime;
ADDRGP4 uiInfo+99264+2192
ADDRGP4 uiInfo+208
INDIRI4
ASGNI4
line 3840
;3838:
;3839:	// if there were no servers visible for ping updates
;3840:	if (!visible) {
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $3077
line 3843
;3841://		UI_StopServerRefresh();
;3842://		uiInfo.serverStatus.nextDisplayRefresh = 0;
;3843:	}
LABELV $3077
line 3844
;3844:}
LABELV $2969
endproc UI_BuildServerDisplayList 1100 16
data
export serverStatusCvars
align 4
LABELV serverStatusCvars
address $3080
address $3081
address $3082
address $109
address $3083
address $3084
address $1323
address $3085
address $3086
address $3087
address $3088
address $109
address $2163
address $109
address $3089
address $109
address $716
address $109
byte 4 0
byte 4 0
code
proc UI_SortServerStatusInfo 56 8
line 3869
;3845:
;3846:typedef struct
;3847:{
;3848:	char *name, *altName;
;3849:} serverStatusCvar_t;
;3850:
;3851:serverStatusCvar_t serverStatusCvars[] = {
;3852:	{"sv_hostname", "Name"},
;3853:	{"Address", ""},
;3854:	{"gamename", "Game name"},
;3855:	{"g_gametype", "Game type"},
;3856:	{"mapname", "Map"},
;3857:	{"version", ""},
;3858:	{"protocol", ""},
;3859:	{"timelimit", ""},
;3860:	{"fraglimit", ""},
;3861:	{NULL, NULL}
;3862:};
;3863:
;3864:/*
;3865:==================
;3866:UI_SortServerStatusInfo
;3867:==================
;3868:*/
;3869:static void UI_SortServerStatusInfo( serverStatusInfo_t *info ) {
line 3876
;3870:	int i, j, index;
;3871:	char *tmp1, *tmp2;
;3872:
;3873:	// FIXME: if "gamename" == "baseq3" or "missionpack" then
;3874:	// replace the gametype number by FFA, CTF etc.
;3875:	//
;3876:	index = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 3877
;3877:	for (i = 0; serverStatusCvars[i].name; i++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $3094
JUMPV
LABELV $3091
line 3878
;3878:		for (j = 0; j < info->numLines; j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3098
JUMPV
LABELV $3095
line 3879
;3879:			if ( !info->lines[j][1] || info->lines[j][1][0] ) {
ADDRLP4 20
CNSTI4 4
ASGNI4
ADDRLP4 24
ADDRLP4 0
INDIRI4
ADDRLP4 20
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3101
ADDRLP4 24
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $3099
LABELV $3101
line 3880
;3880:				continue;
ADDRGP4 $3096
JUMPV
LABELV $3099
line 3882
;3881:			}
;3882:			if ( !Q_stricmp(serverStatusCvars[i].name, info->lines[j][0]) ) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverStatusCvars
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $3102
line 3884
;3883:				// swap lines
;3884:				tmp1 = info->lines[index][0];
ADDRLP4 12
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
INDIRP4
ASGNP4
line 3885
;3885:				tmp2 = info->lines[index][3];
ADDRLP4 16
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
INDIRP4
ASGNP4
line 3886
;3886:				info->lines[index][0] = info->lines[j][0];
ADDRLP4 32
CNSTI4 4
ASGNI4
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 4
INDIRI4
ADDRLP4 32
INDIRI4
LSHI4
ADDRLP4 36
INDIRP4
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 32
INDIRI4
LSHI4
ADDRLP4 36
INDIRP4
ADDP4
INDIRP4
ASGNP4
line 3887
;3887:				info->lines[index][3] = info->lines[j][3];
ADDRLP4 40
CNSTI4 4
ASGNI4
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 48
CNSTI4 12
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 40
INDIRI4
LSHI4
ADDRLP4 44
INDIRP4
ADDP4
ADDRLP4 48
INDIRI4
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 40
INDIRI4
LSHI4
ADDRLP4 44
INDIRP4
ADDP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRP4
ASGNP4
line 3888
;3888:				info->lines[j][0] = tmp1;
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 12
INDIRP4
ASGNP4
line 3889
;3889:				info->lines[j][3] = tmp2;
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRLP4 16
INDIRP4
ASGNP4
line 3891
;3890:				//
;3891:				if ( strlen(serverStatusCvars[i].altName) ) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverStatusCvars+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $3104
line 3892
;3892:					info->lines[index][0] = serverStatusCvars[i].altName;
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverStatusCvars+4
ADDP4
INDIRP4
ASGNP4
line 3893
;3893:				}
LABELV $3104
line 3894
;3894:				index++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3895
;3895:			}
LABELV $3102
line 3896
;3896:		}
LABELV $3096
line 3878
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3098
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
LTI4 $3095
line 3897
;3897:	}
LABELV $3092
line 3877
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3094
ADDRLP4 8
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 serverStatusCvars
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3091
line 3898
;3898:}
LABELV $3090
endproc UI_SortServerStatusInfo 56 8
proc UI_GetServerStatusInfo 168 16
line 3905
;3899:
;3900:/*
;3901:==================
;3902:UI_GetServerStatusInfo
;3903:==================
;3904:*/
;3905:static int UI_GetServerStatusInfo( const char *serverAddress, serverStatusInfo_t *info ) {
line 3909
;3906:	char *p, *score, *ping, *name;
;3907:	int i, len;
;3908:
;3909:	if (!info) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3109
line 3910
;3910:		trap_LAN_ServerStatus( serverAddress, NULL, 0);
ADDRFP4 0
INDIRP4
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_ServerStatus
CALLI4
pop
line 3911
;3911:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3108
JUMPV
LABELV $3109
line 3913
;3912:	}
;3913:	memset(info, 0, sizeof(*info));
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 3332
ARGI4
ADDRGP4 memset
CALLP4
pop
line 3914
;3914:	if ( trap_LAN_ServerStatus( serverAddress, info->text, sizeof(info->text)) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 2112
ADDP4
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 24
ADDRGP4 trap_LAN_ServerStatus
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $3111
line 3915
;3915:		Q_strncpyz(info->address, serverAddress, sizeof(info->address));
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 3916
;3916:		p = info->text;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 2112
ADDP4
ASGNP4
line 3917
;3917:		info->numLines = 0;
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
CNSTI4 0
ASGNI4
line 3918
;3918:		info->lines[info->numLines][0] = "Address";
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 28
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRGP4 $3082
ASGNP4
line 3919
;3919:		info->lines[info->numLines][1] = "";
ADDRLP4 32
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 4
ASGNI4
ADDRLP4 32
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
LSHI4
ADDRLP4 32
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 36
INDIRI4
ADDP4
ADDRGP4 $109
ASGNP4
line 3920
;3920:		info->lines[info->numLines][2] = "";
ADDRLP4 40
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRGP4 $109
ASGNP4
line 3921
;3921:		info->lines[info->numLines][3] = info->address;
ADDRLP4 44
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 44
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRLP4 44
INDIRP4
ASGNP4
line 3922
;3922:		info->numLines++;
ADDRLP4 48
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $3114
JUMPV
LABELV $3113
line 3924
;3923:		// get the cvars
;3924:		while (p && *p) {
line 3925
;3925:			p = strchr(p, '\\');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 52
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 52
INDIRP4
ASGNP4
line 3926
;3926:			if (!p) break;
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3116
ADDRGP4 $3115
JUMPV
LABELV $3116
line 3927
;3927:			*p++ = '\0';
ADDRLP4 56
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 56
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI1 0
ASGNI1
line 3928
;3928:			if (*p == '\\')
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $3118
line 3929
;3929:				break;
ADDRGP4 $3115
JUMPV
LABELV $3118
line 3930
;3930:			info->lines[info->numLines][0] = p;
ADDRLP4 60
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 60
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 3931
;3931:			info->lines[info->numLines][1] = "";
ADDRLP4 64
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 68
CNSTI4 4
ASGNI4
ADDRLP4 64
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
ADDRLP4 68
INDIRI4
LSHI4
ADDRLP4 64
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 68
INDIRI4
ADDP4
ADDRGP4 $109
ASGNP4
line 3932
;3932:			info->lines[info->numLines][2] = "";
ADDRLP4 72
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 72
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRGP4 $109
ASGNP4
line 3933
;3933:			p = strchr(p, '\\');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 76
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 76
INDIRP4
ASGNP4
line 3934
;3934:			if (!p) break;
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3120
ADDRGP4 $3115
JUMPV
LABELV $3120
line 3935
;3935:			*p++ = '\0';
ADDRLP4 80
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 80
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
CNSTI1 0
ASGNI1
line 3936
;3936:			info->lines[info->numLines][3] = p;
ADDRLP4 84
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 84
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 3938
;3937:
;3938:			info->numLines++;
ADDRLP4 88
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3939
;3939:			if (info->numLines >= MAX_SERVERSTATUS_LINES)
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 128
LTI4 $3122
line 3940
;3940:				break;
ADDRGP4 $3115
JUMPV
LABELV $3122
line 3941
;3941:		}
LABELV $3114
line 3924
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3124
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3113
LABELV $3124
LABELV $3115
line 3943
;3942:		// get the player list
;3943:		if (info->numLines < MAX_SERVERSTATUS_LINES-3) {
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 125
GEI4 $3125
line 3945
;3944:			// empty line
;3945:			info->lines[info->numLines][0] = "";
ADDRLP4 56
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 56
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRGP4 $109
ASGNP4
line 3946
;3946:			info->lines[info->numLines][1] = "";
ADDRLP4 60
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 64
CNSTI4 4
ASGNI4
ADDRLP4 60
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
ADDRLP4 64
INDIRI4
LSHI4
ADDRLP4 60
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 64
INDIRI4
ADDP4
ADDRGP4 $109
ASGNP4
line 3947
;3947:			info->lines[info->numLines][2] = "";
ADDRLP4 68
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 68
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRGP4 $109
ASGNP4
line 3948
;3948:			info->lines[info->numLines][3] = "";
ADDRLP4 72
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 72
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRGP4 $109
ASGNP4
line 3949
;3949:			info->numLines++;
ADDRLP4 76
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3951
;3950:			// header
;3951:			info->lines[info->numLines][0] = "num";
ADDRLP4 80
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 80
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 80
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRGP4 $3127
ASGNP4
line 3952
;3952:			info->lines[info->numLines][1] = "score";
ADDRLP4 84
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 88
CNSTI4 4
ASGNI4
ADDRLP4 84
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
ADDRLP4 88
INDIRI4
LSHI4
ADDRLP4 84
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 88
INDIRI4
ADDP4
ADDRGP4 $3128
ASGNP4
line 3953
;3953:			info->lines[info->numLines][2] = "ping";
ADDRLP4 92
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 92
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRGP4 $3129
ASGNP4
line 3954
;3954:			info->lines[info->numLines][3] = "name";
ADDRLP4 96
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 96
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 96
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRGP4 $1409
ASGNP4
line 3955
;3955:			info->numLines++;
ADDRLP4 100
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
ASGNP4
ADDRLP4 100
INDIRP4
ADDRLP4 100
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3957
;3956:			// parse players
;3957:			i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 3958
;3958:			len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $3131
JUMPV
LABELV $3130
line 3959
;3959:			while (p && *p) {
line 3960
;3960:				if (*p == '\\')
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $3133
line 3961
;3961:					*p++ = '\0';
ADDRLP4 104
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 104
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 104
INDIRP4
CNSTI1 0
ASGNI1
LABELV $3133
line 3962
;3962:				if (!p)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3135
line 3963
;3963:					break;
ADDRGP4 $3132
JUMPV
LABELV $3135
line 3964
;3964:				score = p;
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
line 3965
;3965:				p = strchr(p, ' ');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 108
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 108
INDIRP4
ASGNP4
line 3966
;3966:				if (!p)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3137
line 3967
;3967:					break;
ADDRGP4 $3132
JUMPV
LABELV $3137
line 3968
;3968:				*p++ = '\0';
ADDRLP4 112
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 112
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 112
INDIRP4
CNSTI1 0
ASGNI1
line 3969
;3969:				ping = p;
ADDRLP4 16
ADDRLP4 0
INDIRP4
ASGNP4
line 3970
;3970:				p = strchr(p, ' ');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 116
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 116
INDIRP4
ASGNP4
line 3971
;3971:				if (!p)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3139
line 3972
;3972:					break;
ADDRGP4 $3132
JUMPV
LABELV $3139
line 3973
;3973:				*p++ = '\0';
ADDRLP4 120
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 120
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 120
INDIRP4
CNSTI1 0
ASGNI1
line 3974
;3974:				name = p;
ADDRLP4 20
ADDRLP4 0
INDIRP4
ASGNP4
line 3975
;3975:				Com_sprintf(&info->pings[len], sizeof(info->pings)-len, "%d", i);
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 3136
ADDP4
ADDP4
ARGP4
CNSTU4 192
ADDRLP4 4
INDIRI4
CVIU4 4
SUBU4
CVUI4 4
ARGI4
ADDRGP4 $713
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 3976
;3976:				info->lines[info->numLines][0] = &info->pings[len];
ADDRLP4 128
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 128
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 128
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 4
INDIRI4
ADDRLP4 128
INDIRP4
CNSTI4 3136
ADDP4
ADDP4
ASGNP4
line 3977
;3977:				len += strlen(&info->pings[len]) + 1;
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
CNSTI4 3136
ADDP4
ADDP4
ARGP4
ADDRLP4 136
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 136
INDIRI4
CNSTI4 1
ADDI4
ADDI4
ASGNI4
line 3978
;3978:				info->lines[info->numLines][1] = score;
ADDRLP4 140
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 144
CNSTI4 4
ASGNI4
ADDRLP4 140
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
ADDRLP4 144
INDIRI4
LSHI4
ADDRLP4 140
INDIRP4
CNSTI4 64
ADDP4
ADDP4
ADDRLP4 144
INDIRI4
ADDP4
ADDRLP4 12
INDIRP4
ASGNP4
line 3979
;3979:				info->lines[info->numLines][2] = ping;
ADDRLP4 148
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 148
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 148
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRLP4 16
INDIRP4
ASGNP4
line 3980
;3980:				info->lines[info->numLines][3] = name;
ADDRLP4 152
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 152
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 152
INDIRP4
CNSTI4 64
ADDP4
ADDP4
CNSTI4 12
ADDP4
ADDRLP4 20
INDIRP4
ASGNP4
line 3981
;3981:				info->numLines++;
ADDRLP4 156
ADDRFP4 4
INDIRP4
CNSTI4 3328
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
line 3982
;3982:				if (info->numLines >= MAX_SERVERSTATUS_LINES)
ADDRFP4 4
INDIRP4
CNSTI4 3328
ADDP4
INDIRI4
CNSTI4 128
LTI4 $3141
line 3983
;3983:					break;
ADDRGP4 $3132
JUMPV
LABELV $3141
line 3984
;3984:				p = strchr(p, '\\');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 160
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 160
INDIRP4
ASGNP4
line 3985
;3985:				if (!p)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3143
line 3986
;3986:					break;
ADDRGP4 $3132
JUMPV
LABELV $3143
line 3987
;3987:				*p++ = '\0';
ADDRLP4 164
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 164
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 164
INDIRP4
CNSTI1 0
ASGNI1
line 3989
;3988:				//
;3989:				i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3990
;3990:			}
LABELV $3131
line 3959
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3145
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3130
LABELV $3145
LABELV $3132
line 3991
;3991:		}
LABELV $3125
line 3992
;3992:		UI_SortServerStatusInfo( info );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 UI_SortServerStatusInfo
CALLV
pop
line 3993
;3993:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3108
JUMPV
LABELV $3111
line 3995
;3994:	}
;3995:	return qfalse;
CNSTI4 0
RETI4
LABELV $3108
endproc UI_GetServerStatusInfo 168 16
proc stristr 12 4
line 4003
;3996:}
;3997:
;3998:/*
;3999:==================
;4000:stristr
;4001:==================
;4002:*/
;4003:static char *stristr(char *str, char *charset) {
ADDRGP4 $3148
JUMPV
LABELV $3147
line 4006
;4004:	int i;
;4005:
;4006:	while(*str) {
line 4007
;4007:		for (i = 0; charset[i] && str[i]; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3153
JUMPV
LABELV $3150
line 4008
;4008:			if (toupper(charset[i]) != toupper(str[i])) break;
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
EQI4 $3154
ADDRGP4 $3152
JUMPV
LABELV $3154
line 4009
;4009:		}
LABELV $3151
line 4007
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3153
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
EQI4 $3156
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ADDRLP4 8
INDIRI4
NEI4 $3150
LABELV $3156
LABELV $3152
line 4010
;4010:		if (!charset[i]) return str;
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3157
ADDRFP4 0
INDIRP4
RETP4
ADDRGP4 $3146
JUMPV
LABELV $3157
line 4011
;4011:		str++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 4012
;4012:	}
LABELV $3148
line 4006
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3147
line 4013
;4013:	return NULL;
CNSTP4 0
RETP4
LABELV $3146
endproc stristr 12 4
bss
align 4
LABELV $3160
skip 4
align 4
LABELV $3161
skip 4
code
proc UI_BuildFindPlayerList 4424 24
line 4021
;4014:}
;4015:
;4016:/*
;4017:==================
;4018:UI_BuildFindPlayerList
;4019:==================
;4020:*/
;4021:static void UI_BuildFindPlayerList(qboolean force) {
line 4028
;4022:	static int numFound, numTimeOuts;
;4023:	int i, j, resend;
;4024:	serverStatusInfo_t info;
;4025:	char name[MAX_NAME_LENGTH+2];
;4026:	char infoString[MAX_STRING_CHARS];
;4027:
;4028:	if (!force) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $3162
line 4029
;4029:		if (!uiInfo.nextFindPlayerRefresh || uiInfo.nextFindPlayerRefresh > uiInfo.uiDC.realTime) {
ADDRGP4 uiInfo+119472
INDIRI4
CNSTI4 0
EQI4 $3169
ADDRGP4 uiInfo+119472
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
LEI4 $3163
LABELV $3169
line 4030
;4030:			return;
ADDRGP4 $3159
JUMPV
line 4032
;4031:		}
;4032:	}
LABELV $3162
line 4033
;4033:	else {
line 4034
;4034:		memset(&uiInfo.pendingServerStatus, 0, sizeof(uiInfo.pendingServerStatus));
ADDRGP4 uiInfo+114148
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2244
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4035
;4035:		uiInfo.numFoundPlayerServers = 0;
ADDRGP4 uiInfo+119468
CNSTI4 0
ASGNI4
line 4036
;4036:		uiInfo.currentFoundPlayerServer = 0;
ADDRGP4 uiInfo+119464
CNSTI4 0
ASGNI4
line 4037
;4037:		trap_Cvar_VariableStringBuffer( "ui_findPlayer", uiInfo.findPlayerName, sizeof(uiInfo.findPlayerName));
ADDRGP4 $3174
ARGP4
ADDRGP4 uiInfo+116392
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 4038
;4038:		Q_CleanStr(uiInfo.findPlayerName);
ADDRGP4 uiInfo+116392
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 4040
;4039:		// should have a string of some length
;4040:		if (!strlen(uiInfo.findPlayerName)) {
ADDRGP4 uiInfo+116392
ARGP4
ADDRLP4 4404
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4404
INDIRI4
CNSTI4 0
NEI4 $3178
line 4041
;4041:			uiInfo.nextFindPlayerRefresh = 0;
ADDRGP4 uiInfo+119472
CNSTI4 0
ASGNI4
line 4042
;4042:			return;
ADDRGP4 $3159
JUMPV
LABELV $3178
line 4045
;4043:		}
;4044:		// set resend time
;4045:		resend = ui_serverStatusTimeOut.integer / 2 - 10;
ADDRLP4 4400
ADDRGP4 ui_serverStatusTimeOut+12
INDIRI4
CNSTI4 2
DIVI4
CNSTI4 10
SUBI4
ASGNI4
line 4046
;4046:		if (resend < 50) {
ADDRLP4 4400
INDIRI4
CNSTI4 50
GEI4 $3183
line 4047
;4047:			resend = 50;
ADDRLP4 4400
CNSTI4 50
ASGNI4
line 4048
;4048:		}
LABELV $3183
line 4049
;4049:		trap_Cvar_Set("cl_serverStatusResendTime", va("%d", resend));
ADDRGP4 $713
ARGP4
ADDRLP4 4400
INDIRI4
ARGI4
ADDRLP4 4408
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $3185
ARGP4
ADDRLP4 4408
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4051
;4050:		// reset all server status requests
;4051:		trap_LAN_ServerStatus( NULL, NULL, 0);
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_ServerStatus
CALLI4
pop
line 4053
;4052:		//
;4053:		uiInfo.numFoundPlayerServers = 1;
ADDRGP4 uiInfo+119468
CNSTI4 1
ASGNI4
line 4054
;4054:		Com_sprintf(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1],
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $3193
ARGP4
ADDRGP4 uiInfo+114148
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4057
;4055:						sizeof(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1]),
;4056:							"searching %d...", uiInfo.pendingServerStatus.num);
;4057:		numFound = 0;
ADDRGP4 $3160
CNSTI4 0
ASGNI4
line 4058
;4058:		numTimeOuts++;
ADDRLP4 4412
ADDRGP4 $3161
ASGNP4
ADDRLP4 4412
INDIRP4
ADDRLP4 4412
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4059
;4059:	}
LABELV $3163
line 4060
;4060:	for (i = 0; i < MAX_SERVERSTATUSREQUESTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $3195
line 4062
;4061:		// if this pending server is valid
;4062:		if (uiInfo.pendingServerStatus.server[i].valid) {
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3199
line 4064
;4063:			// try to get the server status for this server
;4064:			if (UI_GetServerStatusInfo( uiInfo.pendingServerStatus.server[i].adrstr, &info ) ) {
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 4404
ADDRGP4 UI_GetServerStatusInfo
CALLI4
ASGNI4
ADDRLP4 4404
INDIRI4
CNSTI4 0
EQI4 $3204
line 4066
;4065:				//
;4066:				numFound++;
ADDRLP4 4408
ADDRGP4 $3160
ASGNP4
ADDRLP4 4408
INDIRP4
ADDRLP4 4408
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4068
;4067:				// parse through the server status lines
;4068:				for (j = 0; j < info.numLines; j++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $3211
JUMPV
LABELV $3208
line 4070
;4069:					// should have ping info
;4070:					if ( !info.lines[j][2] || !info.lines[j][2][0] ) {
ADDRLP4 4412
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ASGNI4
ADDRLP4 4412
INDIRI4
ADDRLP4 8+64+8
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3219
ADDRLP4 4412
INDIRI4
ADDRLP4 8+64+8
ADDP4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3213
LABELV $3219
line 4071
;4071:						continue;
ADDRGP4 $3209
JUMPV
LABELV $3213
line 4074
;4072:					}
;4073:					// clean string first
;4074:					Q_strncpyz(name, info.lines[j][3], sizeof(name));
ADDRLP4 3340
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 8+64+12
ADDP4
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 4075
;4075:					Q_CleanStr(name);
ADDRLP4 3340
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 4077
;4076:					// if the player name is a substring
;4077:					if (stristr(name, uiInfo.findPlayerName)) {
ADDRLP4 3340
ARGP4
ADDRGP4 uiInfo+116392
ARGP4
ADDRLP4 4416
ADDRGP4 stristr
CALLP4
ASGNP4
ADDRLP4 4416
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3222
line 4079
;4078:						// add to found server list if we have space (always leave space for a line with the number found)
;4079:						if (uiInfo.numFoundPlayerServers < MAX_FOUNDPLAYER_SERVERS-1) {
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 15
GEI4 $3225
line 4081
;4080:							//
;4081:							Q_strncpyz(uiInfo.foundPlayerServerAddresses[uiInfo.numFoundPlayerServers-1],
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+117416-64
ADDP4
ARGP4
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 4084
;4082:										uiInfo.pendingServerStatus.server[i].adrstr,
;4083:											sizeof(uiInfo.foundPlayerServerAddresses[0]));
;4084:							Q_strncpyz(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1],
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 4087
;4085:										uiInfo.pendingServerStatus.server[i].name,
;4086:											sizeof(uiInfo.foundPlayerServerNames[0]));
;4087:							uiInfo.numFoundPlayerServers++;
ADDRLP4 4420
ADDRGP4 uiInfo+119468
ASGNP4
ADDRLP4 4420
INDIRP4
ADDRLP4 4420
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4088
;4088:						}
ADDRGP4 $3226
JUMPV
LABELV $3225
line 4089
;4089:						else {
line 4091
;4090:							// can't add any more so we're done
;4091:							uiInfo.pendingServerStatus.num = uiInfo.serverStatus.numDisplayServers;
ADDRGP4 uiInfo+114148
ADDRGP4 uiInfo+99264+10412
INDIRI4
ASGNI4
line 4092
;4092:						}
LABELV $3226
line 4093
;4093:					}
LABELV $3222
line 4094
;4094:				}
LABELV $3209
line 4068
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3211
ADDRLP4 4
INDIRI4
ADDRLP4 8+3328
INDIRI4
LTI4 $3208
line 4095
;4095:				Com_sprintf(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1],
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $3251
ARGP4
ADDRGP4 uiInfo+114148
INDIRI4
ARGI4
ADDRGP4 $3160
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4099
;4096:								sizeof(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1]),
;4097:									"searching %d/%d...", uiInfo.pendingServerStatus.num, numFound);
;4098:				// retrieved the server status so reuse this spot
;4099:				uiInfo.pendingServerStatus.server[i].valid = qfalse;
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
CNSTI4 0
ASGNI4
line 4100
;4100:			}
LABELV $3204
line 4101
;4101:		}
LABELV $3199
line 4103
;4102:		// if empty pending slot or timed out
;4103:		if (!uiInfo.pendingServerStatus.server[i].valid ||
ADDRLP4 4404
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4404
INDIRI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3266
ADDRLP4 4404
INDIRI4
ADDRGP4 uiInfo+114148+4+128
ADDP4
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 ui_serverStatusTimeOut+12
INDIRI4
SUBI4
GEI4 $3256
LABELV $3266
line 4104
;4104:			uiInfo.pendingServerStatus.server[i].startTime < uiInfo.uiDC.realTime - ui_serverStatusTimeOut.integer) {
line 4105
;4105:			if (uiInfo.pendingServerStatus.server[i].valid) {
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3267
line 4106
;4106:				numTimeOuts++;
ADDRLP4 4408
ADDRGP4 $3161
ASGNP4
ADDRLP4 4408
INDIRP4
ADDRLP4 4408
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4107
;4107:			}
LABELV $3267
line 4109
;4108:			// reset server status request for this address
;4109:			UI_GetServerStatusInfo( uiInfo.pendingServerStatus.server[i].adrstr, NULL );
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4
ADDP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 UI_GetServerStatusInfo
CALLI4
pop
line 4111
;4110:			// reuse pending slot
;4111:			uiInfo.pendingServerStatus.server[i].valid = qfalse;
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
CNSTI4 0
ASGNI4
line 4113
;4112:			// if we didn't try to get the status of all servers in the main browser yet
;4113:			if (uiInfo.pendingServerStatus.num < uiInfo.serverStatus.numDisplayServers) {
ADDRGP4 uiInfo+114148
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
GEI4 $3277
line 4114
;4114:				uiInfo.pendingServerStatus.server[i].startTime = uiInfo.uiDC.realTime;
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+128
ADDP4
ADDRGP4 uiInfo+208
INDIRI4
ASGNI4
line 4115
;4115:				trap_LAN_GetServerAddressString(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.pendingServerStatus.num],
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+114148
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_LAN_GetServerAddressString
CALLV
pop
line 4117
;4116:							uiInfo.pendingServerStatus.server[i].adrstr, sizeof(uiInfo.pendingServerStatus.server[i].adrstr));
;4117:				trap_LAN_GetServerInfo(ui_netSource.integer, uiInfo.serverStatus.displayServers[uiInfo.pendingServerStatus.num], infoString, sizeof(infoString));
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 uiInfo+114148
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRLP4 3374
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 4118
;4118:				Q_strncpyz(uiInfo.pendingServerStatus.server[i].name, Info_ValueForKey(infoString, "hostname"), sizeof(uiInfo.pendingServerStatus.server[0].name));
ADDRLP4 3374
ARGP4
ADDRGP4 $2694
ARGP4
ADDRLP4 4408
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+64
ADDP4
ARGP4
ADDRLP4 4408
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 4119
;4119:				uiInfo.pendingServerStatus.server[i].valid = qtrue;
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
CNSTI4 1
ASGNI4
line 4120
;4120:				uiInfo.pendingServerStatus.num++;
ADDRLP4 4412
ADDRGP4 uiInfo+114148
ASGNP4
ADDRLP4 4412
INDIRP4
ADDRLP4 4412
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4121
;4121:				Com_sprintf(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1],
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $3251
ARGP4
ADDRGP4 uiInfo+114148
INDIRI4
ARGI4
ADDRGP4 $3160
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 4124
;4122:								sizeof(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1]),
;4123:									"searching %d/%d...", uiInfo.pendingServerStatus.num, numFound);
;4124:			}
LABELV $3277
line 4125
;4125:		}
LABELV $3256
line 4126
;4126:	}
LABELV $3196
line 4060
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $3195
line 4127
;4127:	for (i = 0; i < MAX_SERVERSTATUSREQUESTS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $3315
line 4128
;4128:		if (uiInfo.pendingServerStatus.server[i].valid) {
CNSTI4 140
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+114148+4+136
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3319
line 4129
;4129:			break;
ADDRGP4 $3317
JUMPV
LABELV $3319
line 4131
;4130:		}
;4131:	}
LABELV $3316
line 4127
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $3315
LABELV $3317
line 4133
;4132:	// if still trying to retrieve server status info
;4133:	if (i < MAX_SERVERSTATUSREQUESTS) {
ADDRLP4 0
INDIRI4
CNSTI4 16
GEI4 $3324
line 4134
;4134:		uiInfo.nextFindPlayerRefresh = uiInfo.uiDC.realTime + 25;
ADDRGP4 uiInfo+119472
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 25
ADDI4
ASGNI4
line 4135
;4135:	}
ADDRGP4 $3325
JUMPV
LABELV $3324
line 4136
;4136:	else {
line 4138
;4137:		// add a line that shows the number of servers found
;4138:		if (!uiInfo.numFoundPlayerServers) {
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 0
NEI4 $3328
line 4139
;4139:			Com_sprintf(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1], sizeof(uiInfo.foundPlayerServerAddresses[0]), "no servers found");
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $3335
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4140
;4140:		}
ADDRGP4 $3329
JUMPV
LABELV $3328
line 4141
;4141:		else {
line 4142
;4142:			Com_sprintf(uiInfo.foundPlayerServerNames[uiInfo.numFoundPlayerServers-1], sizeof(uiInfo.foundPlayerServerAddresses[0]),
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440-64
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $3340
ARGP4
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 2
NEI4 $3346
ADDRLP4 4404
ADDRGP4 $109
ASGNP4
ADDRGP4 $3347
JUMPV
LABELV $3346
ADDRLP4 4404
ADDRGP4 $3343
ASGNP4
LABELV $3347
ADDRLP4 4404
INDIRP4
ARGP4
ADDRGP4 uiInfo+116392
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4145
;4143:						"%d server%s found with player %s", uiInfo.numFoundPlayerServers-1,
;4144:						uiInfo.numFoundPlayerServers == 2 ? "":"s", uiInfo.findPlayerName);
;4145:		}
LABELV $3329
line 4146
;4146:		uiInfo.nextFindPlayerRefresh = 0;
ADDRGP4 uiInfo+119472
CNSTI4 0
ASGNI4
line 4148
;4147:		// show the server status info for the selected server
;4148:		UI_FeederSelection(FEEDER_FINDPLAYER, uiInfo.currentFoundPlayerServer);
CNSTF4 1096810496
ARGF4
ADDRGP4 uiInfo+119464
INDIRI4
ARGI4
ADDRGP4 UI_FeederSelection
CALLV
pop
line 4149
;4149:	}
LABELV $3325
line 4150
;4150:}
LABELV $3159
endproc UI_BuildFindPlayerList 4424 24
proc UI_BuildServerStatus 8 16
line 4157
;4151:
;4152:/*
;4153:==================
;4154:UI_BuildServerStatus
;4155:==================
;4156:*/
;4157:static void UI_BuildServerStatus(qboolean force) {
line 4159
;4158:
;4159:	if (uiInfo.nextFindPlayerRefresh) {
ADDRGP4 uiInfo+119472
INDIRI4
CNSTI4 0
EQI4 $3351
line 4160
;4160:		return;
ADDRGP4 $3350
JUMPV
LABELV $3351
line 4162
;4161:	}
;4162:	if (!force) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $3354
line 4163
;4163:		if (!uiInfo.nextServerStatusRefresh || uiInfo.nextServerStatusRefresh > uiInfo.uiDC.realTime) {
ADDRGP4 uiInfo+114144
INDIRI4
CNSTI4 0
EQI4 $3361
ADDRGP4 uiInfo+114144
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
LEI4 $3355
LABELV $3361
line 4164
;4164:			return;
ADDRGP4 $3350
JUMPV
line 4166
;4165:		}
;4166:	}
LABELV $3354
line 4167
;4167:	else {
line 4168
;4168:		Menu_SetFeederSelection(NULL, FEEDER_SERVERSTATUS, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 13
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 4169
;4169:		uiInfo.serverStatusInfo.numLines = 0;
ADDRGP4 uiInfo+110812+3328
CNSTI4 0
ASGNI4
line 4171
;4170:		// reset all server status requests
;4171:		trap_LAN_ServerStatus( NULL, NULL, 0);
CNSTP4 0
ARGP4
CNSTP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_LAN_ServerStatus
CALLI4
pop
line 4172
;4172:	}
LABELV $3355
line 4173
;4173:	if (uiInfo.serverStatus.currentServer < 0 || uiInfo.serverStatus.currentServer > uiInfo.serverStatus.numDisplayServers || uiInfo.serverStatus.numDisplayServers == 0) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 uiInfo+99264+2216
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $3375
ADDRGP4 uiInfo+99264+2216
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
GTI4 $3375
ADDRGP4 uiInfo+99264+10412
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $3364
LABELV $3375
line 4174
;4174:		return;
ADDRGP4 $3350
JUMPV
LABELV $3364
line 4176
;4175:	}
;4176:	if (UI_GetServerStatusInfo( uiInfo.serverStatusAddress, &uiInfo.serverStatusInfo ) ) {
ADDRGP4 uiInfo+110748
ARGP4
ADDRGP4 uiInfo+110812
ARGP4
ADDRLP4 4
ADDRGP4 UI_GetServerStatusInfo
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $3376
line 4177
;4177:		uiInfo.nextServerStatusRefresh = 0;
ADDRGP4 uiInfo+114144
CNSTI4 0
ASGNI4
line 4178
;4178:		UI_GetServerStatusInfo( uiInfo.serverStatusAddress, NULL );
ADDRGP4 uiInfo+110748
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 UI_GetServerStatusInfo
CALLI4
pop
line 4179
;4179:	}
ADDRGP4 $3377
JUMPV
LABELV $3376
line 4180
;4180:	else {
line 4181
;4181:		uiInfo.nextServerStatusRefresh = uiInfo.uiDC.realTime + 500;
ADDRGP4 uiInfo+114144
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 4182
;4182:	}
LABELV $3377
line 4183
;4183:}
LABELV $3350
endproc UI_BuildServerStatus 8 16
proc UI_FeederCount 12 4
line 4190
;4184:
;4185:/*
;4186:==================
;4187:UI_FeederCount
;4188:==================
;4189:*/
;4190:static int UI_FeederCount(float feederID) {
line 4191
;4191:	if (feederID == FEEDER_HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 0
NEF4 $3385
line 4192
;4192:		return UI_HeadCountByTeam();
ADDRLP4 0
ADDRGP4 UI_HeadCountByTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3385
line 4193
;4193:	} else if (feederID == FEEDER_Q3HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 1094713344
NEF4 $3387
line 4194
;4194:		return uiInfo.q3HeadCount;
ADDRGP4 uiInfo+119488
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3387
line 4195
;4195:	} else if (feederID == FEEDER_CINEMATICS) {
ADDRFP4 0
INDIRF4
CNSTF4 1097859072
NEF4 $3390
line 4196
;4196:		return uiInfo.movieCount;
ADDRGP4 uiInfo+99252
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3390
line 4197
;4197:	} else if (feederID == FEEDER_MAPS || feederID == FEEDER_ALLMAPS) {
ADDRLP4 0
ADDRFP4 0
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
EQF4 $3395
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
NEF4 $3393
LABELV $3395
line 4198
;4198:		return UI_MapCountByGameType(feederID == FEEDER_MAPS ? qtrue : qfalse);
ADDRFP4 0
INDIRF4
CNSTF4 1065353216
NEF4 $3397
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $3398
JUMPV
LABELV $3397
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $3398
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 UI_MapCountByGameType
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3393
line 4199
;4199:	} else if (feederID == FEEDER_SERVERS) {
ADDRFP4 0
INDIRF4
CNSTF4 1073741824
NEF4 $3399
line 4200
;4200:		return uiInfo.serverStatus.numDisplayServers;
ADDRGP4 uiInfo+99264+10412
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3399
line 4201
;4201:	} else if (feederID == FEEDER_SERVERSTATUS) {
ADDRFP4 0
INDIRF4
CNSTF4 1095761920
NEF4 $3403
line 4202
;4202:		return uiInfo.serverStatusInfo.numLines;
ADDRGP4 uiInfo+110812+3328
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3403
line 4203
;4203:	} else if (feederID == FEEDER_FINDPLAYER) {
ADDRFP4 0
INDIRF4
CNSTF4 1096810496
NEF4 $3407
line 4204
;4204:		return uiInfo.numFoundPlayerServers;
ADDRGP4 uiInfo+119468
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3407
line 4205
;4205:	} else if (feederID == FEEDER_PLAYER_LIST) {
ADDRFP4 0
INDIRF4
CNSTF4 1088421888
NEF4 $3410
line 4206
;4206:		if (uiInfo.uiDC.realTime > uiInfo.playerRefresh) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+78856
INDIRI4
LEI4 $3412
line 4207
;4207:			uiInfo.playerRefresh = uiInfo.uiDC.realTime + 3000;
ADDRGP4 uiInfo+78856
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 4208
;4208:			UI_BuildPlayerList();
ADDRGP4 UI_BuildPlayerList
CALLV
pop
line 4209
;4209:		}
LABELV $3412
line 4210
;4210:		return uiInfo.playerCount;
ADDRGP4 uiInfo+78844
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3410
line 4211
;4211:	} else if (feederID == FEEDER_TEAM_LIST) {
ADDRFP4 0
INDIRF4
CNSTF4 1090519040
NEF4 $3419
line 4212
;4212:		if (uiInfo.uiDC.realTime > uiInfo.playerRefresh) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+78856
INDIRI4
LEI4 $3421
line 4213
;4213:			uiInfo.playerRefresh = uiInfo.uiDC.realTime + 3000;
ADDRGP4 uiInfo+78856
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 4214
;4214:			UI_BuildPlayerList();
ADDRGP4 UI_BuildPlayerList
CALLV
pop
line 4215
;4215:		}
LABELV $3421
line 4216
;4216:		return uiInfo.myTeamCount;
ADDRGP4 uiInfo+78848
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3419
line 4217
;4217:	} else if (feederID == FEEDER_MODS) {
ADDRFP4 0
INDIRF4
CNSTF4 1091567616
NEF4 $3428
line 4218
;4218:		return uiInfo.modCount;
ADDRGP4 uiInfo+97188
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3428
line 4219
;4219:	} else if (feederID == FEEDER_DEMOS) {
ADDRFP4 0
INDIRF4
CNSTF4 1092616192
NEF4 $3431
line 4220
;4220:		return uiInfo.demoCount;
ADDRGP4 uiInfo+98220
INDIRI4
RETI4
ADDRGP4 $3384
JUMPV
LABELV $3431
line 4222
;4221:	}
;4222:	return 0;
CNSTI4 0
RETI4
LABELV $3384
endproc UI_FeederCount 12 4
proc UI_SelectedMap 8 0
line 4225
;4223:}
;4224:
;4225:static const char *UI_SelectedMap(int index, int *actual) {
line 4227
;4226:	int i, c;
;4227:	c = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 4228
;4228:	*actual = 0;
ADDRFP4 4
INDIRP4
CNSTI4 0
ASGNI4
line 4229
;4229:	for (i = 0; i < uiInfo.mapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3438
JUMPV
LABELV $3435
line 4230
;4230:		if (uiInfo.mapList[i].active) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+96
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3440
line 4231
;4231:			if (c == index) {
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $3444
line 4232
;4232:				*actual = i;
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ASGNI4
line 4233
;4233:				return uiInfo.mapList[i].mapName;
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
INDIRP4
RETP4
ADDRGP4 $3434
JUMPV
LABELV $3444
line 4234
;4234:			} else {
line 4235
;4235:				c++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4236
;4236:			}
line 4237
;4237:		}
LABELV $3440
line 4238
;4238:	}
LABELV $3436
line 4229
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3438
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LTI4 $3435
line 4239
;4239:	return "";
ADDRGP4 $109
RETP4
LABELV $3434
endproc UI_SelectedMap 8 0
proc UI_SelectedHead 8 0
line 4242
;4240:}
;4241:
;4242:static const char *UI_SelectedHead(int index, int *actual) {
line 4244
;4243:	int i, c;
;4244:	c = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 4245
;4245:	*actual = 0;
ADDRFP4 4
INDIRP4
CNSTI4 0
ASGNI4
line 4246
;4246:	for (i = 0; i < uiInfo.characterCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3451
JUMPV
LABELV $3448
line 4247
;4247:		if (uiInfo.characterList[i].active) {
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+16
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3453
line 4248
;4248:			if (c == index) {
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $3457
line 4249
;4249:				*actual = i;
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ASGNI4
line 4250
;4250:				return uiInfo.characterList[i].name;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
RETP4
ADDRGP4 $3447
JUMPV
LABELV $3457
line 4251
;4251:			} else {
line 4252
;4252:				c++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4253
;4253:			}
line 4254
;4254:		}
LABELV $3453
line 4255
;4255:	}
LABELV $3449
line 4246
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3451
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
LTI4 $3448
line 4256
;4256:	return "";
ADDRGP4 $109
RETP4
LABELV $3447
endproc UI_SelectedHead 8 0
proc UI_GetIndexFromSelection 8 0
line 4259
;4257:}
;4258:
;4259:static int UI_GetIndexFromSelection(int actual) {
line 4261
;4260:	int i, c;
;4261:	c = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 4262
;4262:	for (i = 0; i < uiInfo.mapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $3464
JUMPV
LABELV $3461
line 4263
;4263:		if (uiInfo.mapList[i].active) {
CNSTI4 100
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+96
ADDP4
INDIRI4
CNSTI4 0
EQI4 $3466
line 4264
;4264:			if (i == actual) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $3470
line 4265
;4265:				return c;
ADDRLP4 4
INDIRI4
RETI4
ADDRGP4 $3460
JUMPV
LABELV $3470
line 4267
;4266:			}
;4267:				c++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4268
;4268:		}
LABELV $3466
line 4269
;4269:	}
LABELV $3462
line 4262
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $3464
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
LTI4 $3461
line 4270
;4270:  return 0;
CNSTI4 0
RETI4
LABELV $3460
endproc UI_GetIndexFromSelection 8 0
proc UI_UpdatePendingPings 0 4
line 4273
;4271:}
;4272:
;4273:static void UI_UpdatePendingPings() { 
line 4274
;4274:	trap_LAN_ResetPings(ui_netSource.integer);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ResetPings
CALLV
pop
line 4275
;4275:	uiInfo.serverStatus.refreshActive = qtrue;
ADDRGP4 uiInfo+99264+2212
CNSTI4 1
ASGNI4
line 4276
;4276:	uiInfo.serverStatus.refreshtime = uiInfo.uiDC.realTime + 1000;
ADDRGP4 uiInfo+99264+2192
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 4278
;4277:
;4278:}
LABELV $3472
endproc UI_UpdatePendingPings 0 4
bss
align 1
LABELV $3480
skip 1024
align 1
LABELV $3481
skip 1024
align 1
LABELV $3482
skip 32
data
align 4
LABELV $3483
byte 4 -1
align 4
LABELV $3484
byte 4 0
code
proc UI_FeederItemText 64 20
line 4280
;4279:
;4280:static const char *UI_FeederItemText(float feederID, int index, int column, qhandle_t *handle) {
line 4286
;4281:	static char info[MAX_STRING_CHARS];
;4282:	static char hostname[1024];
;4283:	static char clientBuff[32];
;4284:	static int lastColumn = -1;
;4285:	static int lastTime = 0;
;4286:	*handle = -1;
ADDRFP4 12
INDIRP4
CNSTI4 -1
ASGNI4
line 4287
;4287:	if (feederID == FEEDER_HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 0
NEF4 $3485
line 4289
;4288:		int actual;
;4289:		return UI_SelectedHead(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 UI_SelectedHead
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3485
line 4290
;4290:	} else if (feederID == FEEDER_Q3HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 1094713344
NEF4 $3487
line 4291
;4291:		if (index >= 0 && index < uiInfo.q3HeadCount) {
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $3488
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+119488
INDIRI4
GEI4 $3488
line 4292
;4292:			return uiInfo.q3HeadNames[index];
ADDRFP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+119492
ADDP4
RETP4
ADDRGP4 $3479
JUMPV
line 4294
;4293:		}
;4294:	} else if (feederID == FEEDER_MAPS || feederID == FEEDER_ALLMAPS) {
LABELV $3487
ADDRLP4 0
ADDRFP4 0
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
EQF4 $3495
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
NEF4 $3493
LABELV $3495
line 4296
;4295:		int actual;
;4296:		return UI_SelectedMap(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 UI_SelectedMap
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3493
line 4297
;4297:	} else if (feederID == FEEDER_SERVERS) {
ADDRFP4 0
INDIRF4
CNSTF4 1073741824
NEF4 $3496
line 4298
;4298:		if (index >= 0 && index < uiInfo.serverStatus.numDisplayServers) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3497
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
GEI4 $3497
line 4300
;4299:			int ping, game, punkbuster;
;4300:			if (lastColumn != column || lastTime > uiInfo.uiDC.realTime + 5000) {
ADDRGP4 $3483
INDIRI4
ADDRFP4 8
INDIRI4
NEI4 $3505
ADDRGP4 $3484
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 5000
ADDI4
LEI4 $3502
LABELV $3505
line 4301
;4301:				trap_LAN_GetServerInfo(ui_netSource.integer, uiInfo.serverStatus.displayServers[index], info, MAX_STRING_CHARS);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRGP4 $3480
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 4302
;4302:				lastColumn = column;
ADDRGP4 $3483
ADDRFP4 8
INDIRI4
ASGNI4
line 4303
;4303:				lastTime = uiInfo.uiDC.realTime;
ADDRGP4 $3484
ADDRGP4 uiInfo+208
INDIRI4
ASGNI4
line 4304
;4304:			}
LABELV $3502
line 4305
;4305:			ping = atoi(Info_ValueForKey(info, "ping"));
ADDRGP4 $3480
ARGP4
ADDRGP4 $3129
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 24
INDIRI4
ASGNI4
line 4306
;4306:			if (ping == -1) {
ADDRLP4 8
INDIRI4
CNSTI4 -1
NEI4 $3510
line 4309
;4307:				// if we ever see a ping that is out of date, do a server refresh
;4308:				// UI_UpdatePendingPings();
;4309:			}
LABELV $3510
line 4310
;4310:			switch (column) {
ADDRLP4 28
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
LTI4 $3497
ADDRLP4 28
INDIRI4
CNSTI4 5
GTI4 $3497
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $3540
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $3540
address $3514
address $3523
address $3524
address $3526
address $3530
address $3534
code
LABELV $3514
line 4312
;4311:				case SORT_HOST : 
;4312:					if (ping <= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
GTI4 $3515
line 4313
;4313:						return Info_ValueForKey(info, "addr");
ADDRGP4 $3480
ARGP4
ADDRGP4 $2695
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 32
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3515
line 4314
;4314:					} else {
line 4315
;4315:						if ( ui_netSource.integer == AS_LOCAL ) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
NEI4 $3517
line 4316
;4316:							Com_sprintf( hostname, sizeof(hostname), "%s [%s]",
ADDRGP4 $3480
ARGP4
ADDRGP4 $2694
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $3480
ARGP4
ADDRGP4 $3521
ARGP4
ADDRLP4 36
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 $3481
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $3520
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 40
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 netnames
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4319
;4317:											Info_ValueForKey(info, "hostname"),
;4318:											netnames[atoi(Info_ValueForKey(info, "nettype"))] );
;4319:							return hostname;
ADDRGP4 $3481
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3517
line 4321
;4320:						}
;4321:						else {
line 4322
;4322:							Com_sprintf( hostname, sizeof(hostname), "%s", Info_ValueForKey(info, "hostname"));
ADDRGP4 $3480
ARGP4
ADDRGP4 $2694
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $3481
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $3522
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4323
;4323:							return hostname;
ADDRGP4 $3481
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3523
line 4326
;4324:						}
;4325:					}
;4326:				case SORT_MAP : return Info_ValueForKey(info, "mapname");
ADDRGP4 $3480
ARGP4
ADDRGP4 $3086
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 32
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3524
line 4328
;4327:				case SORT_CLIENTS : 
;4328:					Com_sprintf( clientBuff, sizeof(clientBuff), "%s (%s)", Info_ValueForKey(info, "clients"), Info_ValueForKey(info, "sv_maxclients"));
ADDRGP4 $3480
ARGP4
ADDRGP4 $3032
ARGP4
ADDRLP4 36
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $3480
ARGP4
ADDRGP4 $1360
ARGP4
ADDRLP4 40
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $3482
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $3525
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4329
;4329:					return clientBuff;
ADDRGP4 $3482
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3526
line 4331
;4330:				case SORT_GAME : 
;4331:					game = atoi(Info_ValueForKey(info, "gametype"));
ADDRGP4 $3480
ARGP4
ADDRGP4 $3052
ARGP4
ADDRLP4 44
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 48
INDIRI4
ASGNI4
line 4332
;4332:					if (game >= 0 && game < numTeamArenaGameTypes) {
ADDRLP4 52
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
LTI4 $3527
ADDRLP4 52
INDIRI4
ADDRGP4 numTeamArenaGameTypes
INDIRI4
GEI4 $3527
line 4333
;4333:						return teamArenaGameTypes[game];
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 teamArenaGameTypes
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3527
line 4334
;4334:					} else {
line 4335
;4335:						return "Unknown";
ADDRGP4 $3529
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3530
line 4338
;4336:					}
;4337:				case SORT_PING : 
;4338:					if (ping <= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
GTI4 $3531
line 4339
;4339:						return "...";
ADDRGP4 $3533
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3531
line 4340
;4340:					} else {
line 4341
;4341:						return Info_ValueForKey(info, "ping");
ADDRGP4 $3480
ARGP4
ADDRGP4 $3129
ARGP4
ADDRLP4 56
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 56
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3534
line 4344
;4342:					}
;4343:				case SORT_PUNKBUSTER:
;4344:					punkbuster = atoi(Info_ValueForKey(info, "punkbuster"));
ADDRGP4 $3480
ARGP4
ADDRGP4 $3535
ARGP4
ADDRLP4 56
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 60
INDIRI4
ASGNI4
line 4345
;4345:					if ( punkbuster ) {
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $3536
line 4346
;4346:						return "Yes";
ADDRGP4 $3538
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3536
line 4347
;4347:					} else {
line 4348
;4348:						return "No";
ADDRGP4 $3539
RETP4
ADDRGP4 $3479
JUMPV
line 4351
;4349:					}
;4350:			}
;4351:		}
line 4352
;4352:	} else if (feederID == FEEDER_SERVERSTATUS) {
LABELV $3496
ADDRFP4 0
INDIRF4
CNSTF4 1095761920
NEF4 $3541
line 4353
;4353:		if ( index >= 0 && index < uiInfo.serverStatusInfo.numLines ) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3542
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+110812+3328
INDIRI4
GEI4 $3542
line 4354
;4354:			if ( column >= 0 && column < 4 ) {
ADDRLP4 8
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $3542
ADDRLP4 8
INDIRI4
CNSTI4 4
GEI4 $3542
line 4355
;4355:				return uiInfo.serverStatusInfo.lines[index][column];
ADDRFP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 uiInfo+110812+64
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
line 4357
;4356:			}
;4357:		}
line 4358
;4358:	} else if (feederID == FEEDER_FINDPLAYER) {
LABELV $3541
ADDRFP4 0
INDIRF4
CNSTF4 1096810496
NEF4 $3551
line 4359
;4359:		if ( index >= 0 && index < uiInfo.numFoundPlayerServers ) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3552
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+119468
INDIRI4
GEI4 $3552
line 4361
;4360:			//return uiInfo.foundPlayerServerAddresses[index];
;4361:			return uiInfo.foundPlayerServerNames[index];
ADDRFP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+118440
ADDP4
RETP4
ADDRGP4 $3479
JUMPV
line 4363
;4362:		}
;4363:	} else if (feederID == FEEDER_PLAYER_LIST) {
LABELV $3551
ADDRFP4 0
INDIRF4
CNSTF4 1088421888
NEF4 $3557
line 4364
;4364:		if (index >= 0 && index < uiInfo.playerCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3558
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+78844
INDIRI4
GEI4 $3558
line 4365
;4365:			return uiInfo.playerNames[index];
ADDRFP4 4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+78872
ADDP4
RETP4
ADDRGP4 $3479
JUMPV
line 4367
;4366:		}
;4367:	} else if (feederID == FEEDER_TEAM_LIST) {
LABELV $3557
ADDRFP4 0
INDIRF4
CNSTF4 1090519040
NEF4 $3563
line 4368
;4368:		if (index >= 0 && index < uiInfo.myTeamCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3564
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+78848
INDIRI4
GEI4 $3564
line 4369
;4369:			return uiInfo.teamNames[index];
ADDRFP4 4
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 uiInfo+80920
ADDP4
RETP4
ADDRGP4 $3479
JUMPV
line 4371
;4370:		}
;4371:	} else if (feederID == FEEDER_MODS) {
LABELV $3563
ADDRFP4 0
INDIRF4
CNSTF4 1091567616
NEF4 $3569
line 4372
;4372:		if (index >= 0 && index < uiInfo.modCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3570
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+97188
INDIRI4
GEI4 $3570
line 4373
;4373:			if (uiInfo.modList[index].modDescr && *uiInfo.modList[index].modDescr) {
ADDRLP4 8
ADDRFP4 4
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+96676+4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3574
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+96676+4
ADDP4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $3574
line 4374
;4374:				return uiInfo.modList[index].modDescr;
ADDRFP4 4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+96676+4
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3574
line 4375
;4375:			} else {
line 4376
;4376:				return uiInfo.modList[index].modName;
ADDRFP4 4
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+96676
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
line 4379
;4377:			}
;4378:		}
;4379:	} else if (feederID == FEEDER_CINEMATICS) {
LABELV $3569
ADDRFP4 0
INDIRF4
CNSTF4 1097859072
NEF4 $3583
line 4380
;4380:		if (index >= 0 && index < uiInfo.movieCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3584
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+99252
INDIRI4
GEI4 $3584
line 4381
;4381:			return uiInfo.movieList[index];
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+98228
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
line 4383
;4382:		}
;4383:	} else if (feederID == FEEDER_DEMOS) {
LABELV $3583
ADDRFP4 0
INDIRF4
CNSTF4 1092616192
NEF4 $3589
line 4384
;4384:		if (index >= 0 && index < uiInfo.demoCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3591
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+98220
INDIRI4
GEI4 $3591
line 4385
;4385:			return uiInfo.demoList[index];
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+97196
ADDP4
INDIRP4
RETP4
ADDRGP4 $3479
JUMPV
LABELV $3591
line 4387
;4386:		}
;4387:	}
LABELV $3589
LABELV $3584
LABELV $3570
LABELV $3564
LABELV $3558
LABELV $3552
LABELV $3542
LABELV $3497
LABELV $3488
line 4388
;4388:	return "";
ADDRGP4 $109
RETP4
LABELV $3479
endproc UI_FeederItemText 64 20
proc UI_FeederItemImage 20 8
line 4392
;4389:}
;4390:
;4391:
;4392:static qhandle_t UI_FeederItemImage(float feederID, int index) {
line 4393
;4393:  if (feederID == FEEDER_HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 0
NEF4 $3596
line 4395
;4394:	int actual;
;4395:	UI_SelectedHead(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 UI_SelectedHead
CALLP4
pop
line 4396
;4396:	index = actual;
ADDRFP4 4
ADDRLP4 0
INDIRI4
ASGNI4
line 4397
;4397:	if (index >= 0 && index < uiInfo.characterCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3597
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
GEI4 $3597
line 4398
;4398:		if (uiInfo.characterList[index].headImage == -1) {
CNSTI4 24
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+8
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $3601
line 4399
;4399:			uiInfo.characterList[index].headImage = trap_R_RegisterShaderNoMip(uiInfo.characterList[index].imageName);
ADDRLP4 8
CNSTI4 24
ADDRFP4 4
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+73448+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+73448+8
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 4400
;4400:		}
LABELV $3601
line 4401
;4401:		return uiInfo.characterList[index].headImage;
CNSTI4 24
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+8
ADDP4
INDIRI4
RETI4
ADDRGP4 $3595
JUMPV
line 4403
;4402:	}
;4403:  } else if (feederID == FEEDER_Q3HEADS) {
LABELV $3596
ADDRFP4 0
INDIRF4
CNSTF4 1094713344
NEF4 $3611
line 4404
;4404:    if (index >= 0 && index < uiInfo.q3HeadCount) {
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $3612
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+119488
INDIRI4
GEI4 $3612
line 4405
;4405:      return uiInfo.q3HeadIcons[index];
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+135876
ADDP4
INDIRI4
RETI4
ADDRGP4 $3595
JUMPV
line 4407
;4406:    }
;4407:	} else if (feederID == FEEDER_ALLMAPS || feederID == FEEDER_MAPS) {
LABELV $3611
ADDRLP4 0
ADDRFP4 0
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
EQF4 $3619
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
NEF4 $3617
LABELV $3619
line 4409
;4408:		int actual;
;4409:		UI_SelectedMap(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 UI_SelectedMap
CALLP4
pop
line 4410
;4410:		index = actual;
ADDRFP4 4
ADDRLP4 4
INDIRI4
ASGNI4
line 4411
;4411:		if (index >= 0 && index < uiInfo.mapCount) {
ADDRLP4 8
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $3620
ADDRLP4 8
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
GEI4 $3620
line 4412
;4412:			if (uiInfo.mapList[index].levelShot == -1) {
CNSTI4 100
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $3623
line 4413
;4413:				uiInfo.mapList[index].levelShot = trap_R_RegisterShaderNoMip(uiInfo.mapList[index].imageName);
ADDRLP4 12
CNSTI4 100
ADDRFP4 4
INDIRI4
MULI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+8
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 uiInfo+83228+92
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 4414
;4414:			}
LABELV $3623
line 4415
;4415:			return uiInfo.mapList[index].levelShot;
CNSTI4 100
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
INDIRI4
RETI4
ADDRGP4 $3595
JUMPV
LABELV $3620
line 4417
;4416:		}
;4417:	}
LABELV $3617
LABELV $3612
LABELV $3597
line 4418
;4418:  return 0;
CNSTI4 0
RETI4
LABELV $3595
endproc UI_FeederItemImage 20 8
bss
align 1
LABELV $3634
skip 1024
code
proc UI_FeederSelection 40 24
line 4421
;4419:}
;4420:
;4421:static void UI_FeederSelection(float feederID, int index) {
line 4423
;4422:	static char info[MAX_STRING_CHARS];
;4423:  if (feederID == FEEDER_HEADS) {
ADDRFP4 0
INDIRF4
CNSTF4 0
NEF4 $3635
line 4425
;4424:	int actual;
;4425:	UI_SelectedHead(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 UI_SelectedHead
CALLP4
pop
line 4426
;4426:	index = actual;
ADDRFP4 4
ADDRLP4 0
INDIRI4
ASGNI4
line 4427
;4427:    if (index >= 0 && index < uiInfo.characterCount) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $3636
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
GEI4 $3636
line 4428
;4428:		trap_Cvar_Set( "team_model", va("%s", uiInfo.characterList[index].base));
ADDRGP4 $3522
ARGP4
CNSTI4 24
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $966
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4429
;4429:		trap_Cvar_Set( "team_headmodel", va("*%s", uiInfo.characterList[index].name)); 
ADDRGP4 $3642
ARGP4
CNSTI4 24
ADDRFP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $967
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4430
;4430:		updateModel = qtrue;
ADDRGP4 updateModel
CNSTI4 1
ASGNI4
line 4431
;4431:    }
line 4432
;4432:  } else if (feederID == FEEDER_Q3HEADS) {
ADDRGP4 $3636
JUMPV
LABELV $3635
ADDRFP4 0
INDIRF4
CNSTF4 1094713344
NEF4 $3644
line 4433
;4433:    if (index >= 0 && index < uiInfo.q3HeadCount) {
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $3645
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+119488
INDIRI4
GEI4 $3645
line 4434
;4434:      trap_Cvar_Set( "model", uiInfo.q3HeadNames[index]);
ADDRGP4 $962
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+119492
ADDP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4435
;4435:      trap_Cvar_Set( "headmodel", uiInfo.q3HeadNames[index]);
ADDRGP4 $963
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+119492
ADDP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4436
;4436:			updateModel = qtrue;
ADDRGP4 updateModel
CNSTI4 1
ASGNI4
line 4437
;4437:		}
line 4438
;4438:  } else if (feederID == FEEDER_MAPS || feederID == FEEDER_ALLMAPS) {
ADDRGP4 $3645
JUMPV
LABELV $3644
ADDRLP4 0
ADDRFP4 0
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
EQF4 $3653
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
NEF4 $3651
LABELV $3653
line 4440
;4439:		int actual, map;
;4440:		map = (feederID == FEEDER_ALLMAPS) ? ui_currentNetMap.integer : ui_currentMap.integer;
ADDRFP4 0
INDIRF4
CNSTF4 1082130432
NEF4 $3657
ADDRLP4 12
ADDRGP4 ui_currentNetMap+12
INDIRI4
ASGNI4
ADDRGP4 $3658
JUMPV
LABELV $3657
ADDRLP4 12
ADDRGP4 ui_currentMap+12
INDIRI4
ASGNI4
LABELV $3658
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 4441
;4441:		if (uiInfo.mapList[map].cinematic >= 0) {
CNSTI4 100
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
CNSTI4 0
LTI4 $3659
line 4442
;4442:		  trap_CIN_StopCinematic(uiInfo.mapList[map].cinematic);
CNSTI4 100
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4443
;4443:		  uiInfo.mapList[map].cinematic = -1;
CNSTI4 100
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
CNSTI4 -1
ASGNI4
line 4444
;4444:		}
LABELV $3659
line 4445
;4445:		UI_SelectedMap(index, &actual);
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 UI_SelectedMap
CALLP4
pop
line 4446
;4446:		trap_Cvar_Set("ui_mapIndex", va("%d", index));
ADDRGP4 $713
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $2196
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4447
;4447:		ui_mapIndex.integer = index;
ADDRGP4 ui_mapIndex+12
ADDRFP4 4
INDIRI4
ASGNI4
line 4449
;4448:
;4449:		if (feederID == FEEDER_MAPS) {
ADDRFP4 0
INDIRF4
CNSTF4 1065353216
NEF4 $3668
line 4450
;4450:			ui_currentMap.integer = actual;
ADDRGP4 ui_currentMap+12
ADDRLP4 4
INDIRI4
ASGNI4
line 4451
;4451:			trap_Cvar_Set("ui_currentMap", va("%d", actual));
ADDRGP4 $713
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $888
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4452
;4452:	  	uiInfo.mapList[ui_currentMap.integer].cinematic = trap_CIN_PlayCinematic(va("%s.roq", uiInfo.mapList[ui_currentMap.integer].mapLoadName), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRLP4 24
CNSTI4 100
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 36
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 4453
;4453:			UI_LoadBestScores(uiInfo.mapList[ui_currentMap.integer].mapLoadName, uiInfo.gameTypes[ui_gameType.integer].gtEnum);
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_LoadBestScores
CALLV
pop
line 4454
;4454:			trap_Cvar_Set("ui_opponentModel", uiInfo.mapList[ui_currentMap.integer].opponentName);
ADDRGP4 $1092
ARGP4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+12
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4455
;4455:			updateOpponentModel = qtrue;
ADDRGP4 updateOpponentModel
CNSTI4 1
ASGNI4
line 4456
;4456:		} else {
ADDRGP4 $3652
JUMPV
LABELV $3668
line 4457
;4457:			ui_currentNetMap.integer = actual;
ADDRGP4 ui_currentNetMap+12
ADDRLP4 4
INDIRI4
ASGNI4
line 4458
;4458:			trap_Cvar_Set("ui_currentNetMap", va("%d", actual));
ADDRGP4 $713
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $886
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4459
;4459:	  	uiInfo.mapList[ui_currentNetMap.integer].cinematic = trap_CIN_PlayCinematic(va("%s.roq", uiInfo.mapList[ui_currentNetMap.integer].mapLoadName), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRLP4 24
CNSTI4 100
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 ui_currentNetMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 36
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 ui_currentNetMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 4460
;4460:		}
line 4462
;4461:
;4462:  } else if (feederID == FEEDER_SERVERS) {
ADDRGP4 $3652
JUMPV
LABELV $3651
ADDRFP4 0
INDIRF4
CNSTF4 1073741824
NEF4 $3693
line 4463
;4463:		const char *mapName = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 4464
;4464:		uiInfo.serverStatus.currentServer = index;
ADDRGP4 uiInfo+99264+2216
ADDRFP4 4
INDIRI4
ASGNI4
line 4465
;4465:		trap_LAN_GetServerInfo(ui_netSource.integer, uiInfo.serverStatus.displayServers[index], info, MAX_STRING_CHARS);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+99264+2220
ADDP4
INDIRI4
ARGI4
ADDRGP4 $3634
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetServerInfo
CALLV
pop
line 4466
;4466:		uiInfo.serverStatus.currentServerPreview = trap_R_RegisterShaderNoMip(va("levelshots/%s", Info_ValueForKey(info, "mapname")));
ADDRGP4 $3634
ARGP4
ADDRGP4 $3086
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $1039
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+99264+10428
ADDRLP4 16
INDIRI4
ASGNI4
line 4467
;4467:		if (uiInfo.serverStatus.currentServerCinematic >= 0) {
ADDRGP4 uiInfo+99264+10432
INDIRI4
CNSTI4 0
LTI4 $3702
line 4468
;4468:		  trap_CIN_StopCinematic(uiInfo.serverStatus.currentServerCinematic);
ADDRGP4 uiInfo+99264+10432
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4469
;4469:			uiInfo.serverStatus.currentServerCinematic = -1;
ADDRGP4 uiInfo+99264+10432
CNSTI4 -1
ASGNI4
line 4470
;4470:		}
LABELV $3702
line 4471
;4471:		mapName = Info_ValueForKey(info, "mapname");
ADDRGP4 $3634
ARGP4
ADDRGP4 $3086
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 4472
;4472:		if (mapName && *mapName) {
ADDRLP4 24
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3694
ADDRLP4 24
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $3694
line 4473
;4473:			uiInfo.serverStatus.currentServerCinematic = trap_CIN_PlayCinematic(va("%s.roq", mapName), 0, 0, 0, 0, (CIN_loop | CIN_silent) );
ADDRGP4 $790
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
ADDRLP4 32
INDIRI4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 36
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRGP4 uiInfo+99264+10432
ADDRLP4 36
INDIRI4
ASGNI4
line 4474
;4474:		}
line 4475
;4475:  } else if (feederID == FEEDER_SERVERSTATUS) {
ADDRGP4 $3694
JUMPV
LABELV $3693
ADDRFP4 0
INDIRF4
CNSTF4 1095761920
NEF4 $3714
line 4477
;4476:		//
;4477:  } else if (feederID == FEEDER_FINDPLAYER) {
ADDRGP4 $3715
JUMPV
LABELV $3714
ADDRFP4 0
INDIRF4
CNSTF4 1096810496
NEF4 $3716
line 4478
;4478:	  uiInfo.currentFoundPlayerServer = index;
ADDRGP4 uiInfo+119464
ADDRFP4 4
INDIRI4
ASGNI4
line 4480
;4479:	  //
;4480:	  if ( index < uiInfo.numFoundPlayerServers-1) {
ADDRFP4 4
INDIRI4
ADDRGP4 uiInfo+119468
INDIRI4
CNSTI4 1
SUBI4
GEI4 $3717
line 4482
;4481:			// build a new server status for this server
;4482:			Q_strncpyz(uiInfo.serverStatusAddress, uiInfo.foundPlayerServerAddresses[uiInfo.currentFoundPlayerServer], sizeof(uiInfo.serverStatusAddress));
ADDRGP4 uiInfo+110748
ARGP4
ADDRGP4 uiInfo+119464
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+117416
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 4483
;4483:			Menu_SetFeederSelection(NULL, FEEDER_SERVERSTATUS, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 13
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 4484
;4484:			UI_BuildServerStatus(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_BuildServerStatus
CALLV
pop
line 4485
;4485:	  }
line 4486
;4486:  } else if (feederID == FEEDER_PLAYER_LIST) {
ADDRGP4 $3717
JUMPV
LABELV $3716
ADDRFP4 0
INDIRF4
CNSTF4 1088421888
NEF4 $3726
line 4487
;4487:		uiInfo.playerIndex = index;
ADDRGP4 uiInfo+78860
ADDRFP4 4
INDIRI4
ASGNI4
line 4488
;4488:  } else if (feederID == FEEDER_TEAM_LIST) {
ADDRGP4 $3727
JUMPV
LABELV $3726
ADDRFP4 0
INDIRF4
CNSTF4 1090519040
NEF4 $3729
line 4489
;4489:		uiInfo.teamIndex = index;
ADDRGP4 uiInfo+78852
ADDRFP4 4
INDIRI4
ASGNI4
line 4490
;4490:  } else if (feederID == FEEDER_MODS) {
ADDRGP4 $3730
JUMPV
LABELV $3729
ADDRFP4 0
INDIRF4
CNSTF4 1091567616
NEF4 $3732
line 4491
;4491:		uiInfo.modIndex = index;
ADDRGP4 uiInfo+97192
ADDRFP4 4
INDIRI4
ASGNI4
line 4492
;4492:  } else if (feederID == FEEDER_CINEMATICS) {
ADDRGP4 $3733
JUMPV
LABELV $3732
ADDRFP4 0
INDIRF4
CNSTF4 1097859072
NEF4 $3735
line 4493
;4493:		uiInfo.movieIndex = index;
ADDRGP4 uiInfo+99256
ADDRFP4 4
INDIRI4
ASGNI4
line 4494
;4494:		if (uiInfo.previewMovie >= 0) {
ADDRGP4 uiInfo+99260
INDIRI4
CNSTI4 0
LTI4 $3738
line 4495
;4495:		  trap_CIN_StopCinematic(uiInfo.previewMovie);
ADDRGP4 uiInfo+99260
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4496
;4496:		}
LABELV $3738
line 4497
;4497:		uiInfo.previewMovie = -1;
ADDRGP4 uiInfo+99260
CNSTI4 -1
ASGNI4
line 4498
;4498:  } else if (feederID == FEEDER_DEMOS) {
ADDRGP4 $3736
JUMPV
LABELV $3735
ADDRFP4 0
INDIRF4
CNSTF4 1092616192
NEF4 $3743
line 4499
;4499:		uiInfo.demoIndex = index;
ADDRGP4 uiInfo+98224
ADDRFP4 4
INDIRI4
ASGNI4
line 4500
;4500:	}
LABELV $3743
LABELV $3736
LABELV $3733
LABELV $3730
LABELV $3727
LABELV $3717
LABELV $3715
LABELV $3694
LABELV $3652
LABELV $3645
LABELV $3636
line 4501
;4501:}
LABELV $3633
endproc UI_FeederSelection 40 24
proc Team_Parse 72 12
line 4503
;4502:
;4503:static qboolean Team_Parse(char **p) {
line 4508
;4504:  char *token;
;4505:  const char *tempStr;
;4506:	int i;
;4507:
;4508:  token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 12
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
ASGNP4
line 4510
;4509:
;4510:  if (token[0] != '{') {
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $3750
line 4511
;4511:    return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3749
line 4514
;4512:  }
;4513:
;4514:  while ( 1 ) {
line 4516
;4515:
;4516:    token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ASGNP4
line 4518
;4517:    
;4518:    if (Q_stricmp(token, "}") == 0) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $3752
line 4519
;4519:      return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3752
line 4522
;4520:    }
;4521:
;4522:    if ( !token || token[0] == 0 ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3756
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3754
LABELV $3756
line 4523
;4523:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3754
line 4526
;4524:    }
;4525:
;4526:    if (token[0] == '{') {
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $3757
line 4528
;4527:      // seven tokens per line, team name and icon, and 5 team member names
;4528:      if (!String_Parse(p, &uiInfo.teamList[uiInfo.teamCount].teamName) || !String_Parse(p, &tempStr)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $3763
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 32
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $3759
LABELV $3763
line 4529
;4529:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3759
line 4533
;4530:      }
;4531:    
;4532:
;4533:			uiInfo.teamList[uiInfo.teamCount].imageName = tempStr;
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+4
ADDP4
ADDRLP4 8
INDIRP4
ASGNP4
line 4534
;4534:	    uiInfo.teamList[uiInfo.teamCount].teamIcon = trap_R_RegisterShaderNoMip(uiInfo.teamList[uiInfo.teamCount].imageName);
ADDRLP4 36
CNSTI4 44
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+28
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 4535
;4535:		  uiInfo.teamList[uiInfo.teamCount].teamIcon_Metal = trap_R_RegisterShaderNoMip(va("%s_metal",uiInfo.teamList[uiInfo.teamCount].imageName));
ADDRGP4 $766
ARGP4
ADDRLP4 44
CNSTI4 44
ASGNI4
ADDRLP4 44
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+32
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
line 4536
;4536:			uiInfo.teamList[uiInfo.teamCount].teamIcon_Name = trap_R_RegisterShaderNoMip(va("%s_name", uiInfo.teamList[uiInfo.teamCount].imageName));
ADDRGP4 $771
ARGP4
ADDRLP4 56
CNSTI4 44
ASGNI4
ADDRLP4 56
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+36
ADDP4
ADDRLP4 64
INDIRI4
ASGNI4
line 4538
;4537:
;4538:			uiInfo.teamList[uiInfo.teamCount].cinematic = -1;
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
CNSTI4 -1
ASGNI4
line 4540
;4539:
;4540:			for (i = 0; i < TEAM_MEMBERS; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $3788
line 4541
;4541:				uiInfo.teamList[uiInfo.teamCount].teamMembers[i] = NULL;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
CNSTP4 0
ASGNP4
line 4542
;4542:				if (!String_Parse(p, &uiInfo.teamList[uiInfo.teamCount].teamMembers[i])) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+8
ADDP4
ADDP4
ARGP4
ADDRLP4 68
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
NEI4 $3795
line 4543
;4543:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3795
line 4545
;4544:				}
;4545:			}
LABELV $3789
line 4540
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 5
LTI4 $3788
line 4547
;4546:
;4547:      Com_Printf("Loaded team %s with team icon %s.\n", uiInfo.teamList[uiInfo.teamCount].teamName, tempStr);
ADDRGP4 $3800
ARGP4
CNSTI4 44
ADDRGP4 uiInfo+75756
INDIRI4
MULI4
ADDRGP4 uiInfo+75760
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4548
;4548:      if (uiInfo.teamCount < MAX_TEAMS) {
ADDRGP4 uiInfo+75756
INDIRI4
CNSTI4 64
GEI4 $3803
line 4549
;4549:        uiInfo.teamCount++;
ADDRLP4 68
ADDRGP4 uiInfo+75756
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4550
;4550:      } else {
ADDRGP4 $3804
JUMPV
LABELV $3803
line 4551
;4551:        Com_Printf("Too many teams, last team replaced!\n");
ADDRGP4 $3807
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4552
;4552:      }
LABELV $3804
line 4553
;4553:      token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 68
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 68
INDIRP4
ASGNP4
line 4554
;4554:      if (token[0] != '}') {
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
EQI4 $3808
line 4555
;4555:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3746
JUMPV
LABELV $3808
line 4557
;4556:      }
;4557:    }
LABELV $3757
line 4558
;4558:  }
LABELV $3750
line 4514
ADDRGP4 $3749
JUMPV
line 4560
;4559:
;4560:  return qfalse;
CNSTI4 0
RETI4
LABELV $3746
endproc Team_Parse 72 12
proc Character_Parse 68 12
line 4563
;4561:}
;4562:
;4563:static qboolean Character_Parse(char **p) {
line 4567
;4564:  char *token;
;4565:  const char *tempStr;
;4566:
;4567:  token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 4569
;4568:
;4569:  if (token[0] != '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $3814
line 4570
;4570:    return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3810
JUMPV
LABELV $3813
line 4574
;4571:  }
;4572:
;4573:
;4574:  while ( 1 ) {
line 4575
;4575:    token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 12
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 4577
;4576:
;4577:    if (Q_stricmp(token, "}") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $3816
line 4578
;4578:      return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3810
JUMPV
LABELV $3816
line 4581
;4579:    }
;4580:
;4581:    if ( !token || token[0] == 0 ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3820
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3818
LABELV $3820
line 4582
;4582:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3810
JUMPV
LABELV $3818
line 4585
;4583:    }
;4584:
;4585:    if (token[0] == '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $3821
line 4587
;4586:      // two tokens per line, character name and sex
;4587:      if (!String_Parse(p, &uiInfo.characterList[uiInfo.characterCount].name) || !String_Parse(p, &tempStr)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 24
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $3827
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 28
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $3823
LABELV $3827
line 4588
;4588:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3810
JUMPV
LABELV $3823
line 4591
;4589:      }
;4590:    
;4591:      uiInfo.characterList[uiInfo.characterCount].headImage = -1;
CNSTI4 24
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+8
ADDP4
CNSTI4 -1
ASGNI4
line 4592
;4592:			uiInfo.characterList[uiInfo.characterCount].imageName = String_Alloc(va("models/players/heads/%s/icon_default.tga", uiInfo.characterList[uiInfo.characterCount].name));
ADDRGP4 $3834
ARGP4
ADDRLP4 32
CNSTI4 24
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+4
ADDP4
ADDRLP4 40
INDIRP4
ASGNP4
line 4594
;4593:
;4594:	  if (tempStr && (!Q_stricmp(tempStr, "female"))) {
ADDRLP4 44
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3837
ADDRLP4 44
INDIRP4
ARGP4
ADDRGP4 $3839
ARGP4
ADDRLP4 48
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $3837
line 4595
;4595:        uiInfo.characterList[uiInfo.characterCount].base = String_Alloc(va("Janet"));
ADDRGP4 $3843
ARGP4
ADDRLP4 52
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 24
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
ADDRLP4 56
INDIRP4
ASGNP4
line 4596
;4596:      } else if (tempStr && (!Q_stricmp(tempStr, "male"))) {
ADDRGP4 $3838
JUMPV
LABELV $3837
ADDRLP4 52
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3844
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 $3846
ARGP4
ADDRLP4 56
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $3844
line 4597
;4597:        uiInfo.characterList[uiInfo.characterCount].base = String_Alloc(va("James"));
ADDRGP4 $1087
ARGP4
ADDRLP4 60
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 24
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
ADDRLP4 64
INDIRP4
ASGNP4
line 4598
;4598:	  } else {
ADDRGP4 $3845
JUMPV
LABELV $3844
line 4599
;4599:        uiInfo.characterList[uiInfo.characterCount].base = String_Alloc(va("%s",tempStr));
ADDRGP4 $3522
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 24
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
ADDRLP4 64
INDIRP4
ASGNP4
line 4600
;4600:	  }
LABELV $3845
LABELV $3838
line 4602
;4601:
;4602:      Com_Printf("Loaded %s character %s.\n", uiInfo.characterList[uiInfo.characterCount].base, uiInfo.characterList[uiInfo.characterCount].name);
ADDRGP4 $3853
ARGP4
ADDRLP4 60
CNSTI4 24
ASGNI4
ADDRLP4 60
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448+12
ADDP4
INDIRP4
ARGP4
ADDRLP4 60
INDIRI4
ADDRGP4 uiInfo+73440
INDIRI4
MULI4
ADDRGP4 uiInfo+73448
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4603
;4603:      if (uiInfo.characterCount < MAX_HEADS) {
ADDRGP4 uiInfo+73440
INDIRI4
CNSTI4 64
GEI4 $3859
line 4604
;4604:        uiInfo.characterCount++;
ADDRLP4 64
ADDRGP4 uiInfo+73440
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4605
;4605:      } else {
ADDRGP4 $3860
JUMPV
LABELV $3859
line 4606
;4606:        Com_Printf("Too many characters, last character replaced!\n");
ADDRGP4 $3863
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4607
;4607:      }
LABELV $3860
line 4609
;4608:     
;4609:      token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 64
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 64
INDIRP4
ASGNP4
line 4610
;4610:      if (token[0] != '}') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
EQI4 $3864
line 4611
;4611:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3810
JUMPV
LABELV $3864
line 4613
;4612:      }
;4613:    }
LABELV $3821
line 4614
;4614:  }
LABELV $3814
line 4574
ADDRGP4 $3813
JUMPV
line 4616
;4615:
;4616:  return qfalse;
CNSTI4 0
RETI4
LABELV $3810
endproc Character_Parse 68 12
proc Alias_Parse 40 12
line 4620
;4617:}
;4618:
;4619:
;4620:static qboolean Alias_Parse(char **p) {
line 4623
;4621:  char *token;
;4622:
;4623:  token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 4625
;4624:
;4625:  if (token[0] != '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $3870
line 4626
;4626:    return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3866
JUMPV
LABELV $3869
line 4629
;4627:  }
;4628:
;4629:  while ( 1 ) {
line 4630
;4630:    token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 4632
;4631:
;4632:    if (Q_stricmp(token, "}") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $3872
line 4633
;4633:      return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3866
JUMPV
LABELV $3872
line 4636
;4634:    }
;4635:
;4636:    if ( !token || token[0] == 0 ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3876
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3874
LABELV $3876
line 4637
;4637:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3866
JUMPV
LABELV $3874
line 4640
;4638:    }
;4639:
;4640:    if (token[0] == '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $3877
line 4642
;4641:      // three tokens per line, character name, bot alias, and preferred action a - all purpose, d - defense, o - offense
;4642:      if (!String_Parse(p, &uiInfo.aliasList[uiInfo.aliasCount].name) || !String_Parse(p, &uiInfo.aliasList[uiInfo.aliasCount].ai) || !String_Parse(p, &uiInfo.aliasList[uiInfo.aliasCount].action)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 12
ADDRGP4 uiInfo+74984
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $3890
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 12
ADDRGP4 uiInfo+74984
INDIRI4
MULI4
ADDRGP4 uiInfo+74988+4
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $3890
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 12
ADDRGP4 uiInfo+74984
INDIRI4
MULI4
ADDRGP4 uiInfo+74988+8
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $3879
LABELV $3890
line 4643
;4643:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3866
JUMPV
LABELV $3879
line 4646
;4644:      }
;4645:    
;4646:      Com_Printf("Loaded character alias %s using character ai %s.\n", uiInfo.aliasList[uiInfo.aliasCount].name, uiInfo.aliasList[uiInfo.aliasCount].ai);
ADDRGP4 $3891
ARGP4
ADDRLP4 32
CNSTI4 12
ASGNI4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
MULI4
ADDRGP4 uiInfo+74988
ADDP4
INDIRP4
ARGP4
ADDRLP4 32
INDIRI4
ADDRGP4 uiInfo+74984
INDIRI4
MULI4
ADDRGP4 uiInfo+74988+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4647
;4647:      if (uiInfo.aliasCount < MAX_ALIASES) {
ADDRGP4 uiInfo+74984
INDIRI4
CNSTI4 64
GEI4 $3897
line 4648
;4648:        uiInfo.aliasCount++;
ADDRLP4 36
ADDRGP4 uiInfo+74984
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4649
;4649:      } else {
ADDRGP4 $3898
JUMPV
LABELV $3897
line 4650
;4650:        Com_Printf("Too many aliases, last alias replaced!\n");
ADDRGP4 $3901
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4651
;4651:      }
LABELV $3898
line 4653
;4652:     
;4653:      token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 36
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 36
INDIRP4
ASGNP4
line 4654
;4654:      if (token[0] != '}') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
EQI4 $3902
line 4655
;4655:        return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3866
JUMPV
LABELV $3902
line 4657
;4656:      }
;4657:    }
LABELV $3877
line 4658
;4658:  }
LABELV $3870
line 4629
ADDRGP4 $3869
JUMPV
line 4660
;4659:
;4660:  return qfalse;
CNSTI4 0
RETI4
LABELV $3866
endproc Alias_Parse 40 12
proc UI_ParseTeamInfo 44 8
line 4669
;4661:}
;4662:
;4663:
;4664:
;4665:// mode 
;4666:// 0 - high level parsing
;4667:// 1 - team parsing
;4668:// 2 - character parsing
;4669:static void UI_ParseTeamInfo(const char *teamFile) {
line 4672
;4670:	char	*token;
;4671:  char *p;
;4672:  char *buff = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 4675
;4673:  //static int mode = 0; TTimo: unused
;4674:
;4675:  buff = GetMenuBuffer(teamFile);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 GetMenuBuffer
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 4676
;4676:  if (!buff) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $3905
line 4677
;4677:    return;
ADDRGP4 $3904
JUMPV
LABELV $3905
line 4680
;4678:  }
;4679:
;4680:  p = buff;
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
ADDRGP4 $3908
JUMPV
LABELV $3907
line 4682
;4681:
;4682:	while ( 1 ) {
line 4683
;4683:		token = COM_ParseExt( &p, qtrue );
ADDRLP4 4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 4684
;4684:		if( !token || token[0] == 0 || token[0] == '}') {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3913
ADDRLP4 24
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $3913
ADDRLP4 24
INDIRI4
CNSTI4 125
NEI4 $3910
LABELV $3913
line 4685
;4685:			break;
ADDRGP4 $3909
JUMPV
LABELV $3910
line 4688
;4686:		}
;4687:
;4688:		if ( Q_stricmp( token, "}" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $3914
line 4689
;4689:      break;
ADDRGP4 $3909
JUMPV
LABELV $3914
line 4692
;4690:    }
;4691:
;4692:    if (Q_stricmp(token, "teams") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $3918
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $3916
line 4694
;4693:
;4694:      if (Team_Parse(&p)) {
ADDRLP4 4
ARGP4
ADDRLP4 36
ADDRGP4 Team_Parse
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $3909
line 4695
;4695:        continue;
ADDRGP4 $3908
JUMPV
line 4696
;4696:      } else {
line 4697
;4697:        break;
LABELV $3916
line 4701
;4698:      }
;4699:    }
;4700:
;4701:    if (Q_stricmp(token, "characters") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $3923
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $3921
line 4702
;4702:      Character_Parse(&p);
ADDRLP4 4
ARGP4
ADDRGP4 Character_Parse
CALLI4
pop
line 4703
;4703:    }
LABELV $3921
line 4705
;4704:
;4705:    if (Q_stricmp(token, "aliases") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $3926
ARGP4
ADDRLP4 40
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
NEI4 $3924
line 4706
;4706:      Alias_Parse(&p);
ADDRLP4 4
ARGP4
ADDRGP4 Alias_Parse
CALLI4
pop
line 4707
;4707:    }
LABELV $3924
line 4709
;4708:
;4709:  }
LABELV $3908
line 4682
ADDRGP4 $3907
JUMPV
LABELV $3909
line 4711
;4710:
;4711:}
LABELV $3904
endproc UI_ParseTeamInfo 44 8
proc GameType_Parse 28 8
line 4714
;4712:
;4713:
;4714:static qboolean GameType_Parse(char **p, qboolean join) {
line 4717
;4715:	char *token;
;4716:
;4717:	token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 4719
;4718:
;4719:	if (token[0] != '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $3928
line 4720
;4720:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3927
JUMPV
LABELV $3928
line 4723
;4721:	}
;4722:
;4723:	if (join) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $3930
line 4724
;4724:		uiInfo.numJoinGameTypes = 0;
ADDRGP4 uiInfo+78708
CNSTI4 0
ASGNI4
line 4725
;4725:	} else {
ADDRGP4 $3935
JUMPV
LABELV $3930
line 4726
;4726:		uiInfo.numGameTypes = 0;
ADDRGP4 uiInfo+78576
CNSTI4 0
ASGNI4
line 4727
;4727:	}
ADDRGP4 $3935
JUMPV
LABELV $3934
line 4729
;4728:
;4729:	while ( 1 ) {
line 4730
;4730:		token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 4732
;4731:
;4732:		if (Q_stricmp(token, "}") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $3937
line 4733
;4733:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3927
JUMPV
LABELV $3937
line 4736
;4734:		}
;4735:
;4736:		if ( !token || token[0] == 0 ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3941
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3939
LABELV $3941
line 4737
;4737:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3927
JUMPV
LABELV $3939
line 4740
;4738:		}
;4739:
;4740:		if (token[0] == '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $3942
line 4742
;4741:			// two tokens per line, character name and sex
;4742:			if (join) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $3944
line 4743
;4743:				if (!String_Parse(p, &uiInfo.joinGameTypes[uiInfo.numJoinGameTypes].gameType) || !Int_Parse(p, &uiInfo.joinGameTypes[uiInfo.numJoinGameTypes].gtEnum)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 uiInfo+78708
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78712
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $3953
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 uiInfo+78708
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78712+4
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $3945
LABELV $3953
line 4744
;4744:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3927
JUMPV
line 4746
;4745:				}
;4746:			} else {
LABELV $3944
line 4747
;4747:				if (!String_Parse(p, &uiInfo.gameTypes[uiInfo.numGameTypes].gameType) || !Int_Parse(p, &uiInfo.gameTypes[uiInfo.numGameTypes].gtEnum)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 uiInfo+78576
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $3961
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 uiInfo+78576
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $3954
LABELV $3961
line 4748
;4748:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3927
JUMPV
LABELV $3954
line 4750
;4749:				}
;4750:			}
LABELV $3945
line 4752
;4751:    
;4752:			if (join) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $3962
line 4753
;4753:				if (uiInfo.numJoinGameTypes < MAX_GAMETYPES) {
ADDRGP4 uiInfo+78708
INDIRI4
CNSTI4 16
GEI4 $3964
line 4754
;4754:					uiInfo.numJoinGameTypes++;
ADDRLP4 20
ADDRGP4 uiInfo+78708
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4755
;4755:				} else {
ADDRGP4 $3963
JUMPV
LABELV $3964
line 4756
;4756:					Com_Printf("Too many net game types, last one replace!\n");
ADDRGP4 $3968
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4757
;4757:				}		
line 4758
;4758:			} else {
ADDRGP4 $3963
JUMPV
LABELV $3962
line 4759
;4759:				if (uiInfo.numGameTypes < MAX_GAMETYPES) {
ADDRGP4 uiInfo+78576
INDIRI4
CNSTI4 16
GEI4 $3969
line 4760
;4760:					uiInfo.numGameTypes++;
ADDRLP4 20
ADDRGP4 uiInfo+78576
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4761
;4761:				} else {
ADDRGP4 $3970
JUMPV
LABELV $3969
line 4762
;4762:					Com_Printf("Too many game types, last one replace!\n");
ADDRGP4 $3973
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4763
;4763:				}		
LABELV $3970
line 4764
;4764:			}
LABELV $3963
line 4766
;4765:     
;4766:			token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 20
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 4767
;4767:			if (token[0] != '}') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
EQI4 $3974
line 4768
;4768:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3927
JUMPV
LABELV $3974
line 4770
;4769:			}
;4770:		}
LABELV $3942
line 4771
;4771:	}
LABELV $3935
line 4729
ADDRGP4 $3934
JUMPV
line 4772
;4772:	return qfalse;
CNSTI4 0
RETI4
LABELV $3927
endproc GameType_Parse 28 8
proc MapList_Parse 52 8
line 4775
;4773:}
;4774:
;4775:static qboolean MapList_Parse(char **p) {
line 4778
;4776:	char *token;
;4777:
;4778:	token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 4780
;4779:
;4780:	if (token[0] != '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $3977
line 4781
;4781:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3976
JUMPV
LABELV $3977
line 4784
;4782:	}
;4783:
;4784:	uiInfo.mapCount = 0;
ADDRGP4 uiInfo+83224
CNSTI4 0
ASGNI4
ADDRGP4 $3981
JUMPV
LABELV $3980
line 4786
;4785:
;4786:	while ( 1 ) {
line 4787
;4787:		token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 4789
;4788:
;4789:		if (Q_stricmp(token, "}") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $3983
line 4790
;4790:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $3976
JUMPV
LABELV $3983
line 4793
;4791:		}
;4792:
;4793:		if ( !token || token[0] == 0 ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $3987
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $3985
LABELV $3987
line 4794
;4794:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3976
JUMPV
LABELV $3985
line 4797
;4795:		}
;4796:
;4797:		if (token[0] == '{') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $3988
line 4798
;4798:			if (!String_Parse(p, &uiInfo.mapList[uiInfo.mapCount].mapName) || !String_Parse(p, &uiInfo.mapList[uiInfo.mapCount].mapLoadName) 
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $4001
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $4001
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+16
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $3990
LABELV $4001
line 4799
;4799:				||!Int_Parse(p, &uiInfo.mapList[uiInfo.mapCount].teamMembers) ) {
line 4800
;4800:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3976
JUMPV
LABELV $3990
line 4803
;4801:			}
;4802:
;4803:			if (!String_Parse(p, &uiInfo.mapList[uiInfo.mapCount].opponentName)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+12
ADDP4
ARGP4
ADDRLP4 32
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $4002
line 4804
;4804:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3976
JUMPV
LABELV $4002
line 4807
;4805:			}
;4806:
;4807:			uiInfo.mapList[uiInfo.mapCount].typeBits = 0;
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
CNSTI4 0
ASGNI4
ADDRGP4 $4011
JUMPV
LABELV $4010
line 4809
;4808:
;4809:			while (1) {
line 4810
;4810:				token = COM_ParseExt(p, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 36
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 36
INDIRP4
ASGNP4
line 4811
;4811:				if (token[0] >= '0' && token[0] <= '9') {
ADDRLP4 40
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 48
LTI4 $4012
ADDRLP4 40
INDIRI4
CNSTI4 57
GTI4 $4012
line 4812
;4812:					uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << (token[0] - 0x030));
ADDRLP4 44
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
LSHI4
BORI4
ASGNI4
line 4813
;4813:					if (!Int_Parse(p, &uiInfo.mapList[uiInfo.mapCount].timeToBeat[token[0] - 0x30])) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 2
LSHI4
CNSTI4 192
SUBI4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+28
ADDP4
ADDP4
ARGP4
ADDRLP4 48
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $4014
line 4814
;4814:						return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $3976
JUMPV
line 4816
;4815:					}
;4816:				} else {
line 4817
;4817:					break;
LABELV $4014
line 4819
;4818:				} 
;4819:			}
LABELV $4011
line 4809
ADDRGP4 $4010
JUMPV
LABELV $4012
line 4826
;4820:
;4821:			//mapList[mapCount].imageName = String_Alloc(va("levelshots/%s", mapList[mapCount].mapLoadName));
;4822:			//if (uiInfo.mapCount == 0) {
;4823:			  // only load the first cinematic, selection loads the others
;4824:  			//  uiInfo.mapList[uiInfo.mapCount].cinematic = trap_CIN_PlayCinematic(va("%s.roq",uiInfo.mapList[uiInfo.mapCount].mapLoadName), qfalse, qfalse, qtrue, 0, 0, 0, 0);
;4825:			//}
;4826:  		uiInfo.mapList[uiInfo.mapCount].cinematic = -1;
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
CNSTI4 -1
ASGNI4
line 4827
;4827:			uiInfo.mapList[uiInfo.mapCount].levelShot = trap_R_RegisterShaderNoMip(va("levelshots/%s_small", uiInfo.mapList[uiInfo.mapCount].mapLoadName));
ADDRGP4 $4029
ARGP4
ADDRLP4 36
CNSTI4 100
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 4829
;4828:
;4829:			if (uiInfo.mapCount < MAX_MAPS) {
ADDRGP4 uiInfo+83224
INDIRI4
CNSTI4 128
GEI4 $4033
line 4830
;4830:				uiInfo.mapCount++;
ADDRLP4 48
ADDRGP4 uiInfo+83224
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4831
;4831:			} else {
ADDRGP4 $4034
JUMPV
LABELV $4033
line 4832
;4832:				Com_Printf("Too many maps, last one replaced!\n");
ADDRGP4 $4037
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 4833
;4833:			}
LABELV $4034
line 4834
;4834:		}
LABELV $3988
line 4835
;4835:	}
LABELV $3981
line 4786
ADDRGP4 $3980
JUMPV
line 4836
;4836:	return qfalse;
CNSTI4 0
RETI4
LABELV $3976
endproc MapList_Parse 52 8
proc UI_ParseGameInfo 44 8
line 4839
;4837:}
;4838:
;4839:static void UI_ParseGameInfo(const char *teamFile) {
line 4842
;4840:	char	*token;
;4841:	char *p;
;4842:	char *buff = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 4845
;4843:	//int mode = 0; TTimo: unused
;4844:
;4845:	buff = GetMenuBuffer(teamFile);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 GetMenuBuffer
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 4846
;4846:	if (!buff) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $4039
line 4847
;4847:		return;
ADDRGP4 $4038
JUMPV
LABELV $4039
line 4850
;4848:	}
;4849:
;4850:	p = buff;
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
ADDRGP4 $4042
JUMPV
LABELV $4041
line 4852
;4851:
;4852:	while ( 1 ) {
line 4853
;4853:		token = COM_ParseExt( &p, qtrue );
ADDRLP4 4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 4854
;4854:		if( !token || token[0] == 0 || token[0] == '}') {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4047
ADDRLP4 24
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $4047
ADDRLP4 24
INDIRI4
CNSTI4 125
NEI4 $4044
LABELV $4047
line 4855
;4855:			break;
ADDRGP4 $4043
JUMPV
LABELV $4044
line 4858
;4856:		}
;4857:
;4858:		if ( Q_stricmp( token, "}" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $4048
line 4859
;4859:			break;
ADDRGP4 $4043
JUMPV
LABELV $4048
line 4862
;4860:		}
;4861:
;4862:		if (Q_stricmp(token, "gametypes") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $4052
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $4050
line 4864
;4863:
;4864:			if (GameType_Parse(&p, qfalse)) {
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 36
ADDRGP4 GameType_Parse
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $4043
line 4865
;4865:				continue;
ADDRGP4 $4042
JUMPV
line 4866
;4866:			} else {
line 4867
;4867:				break;
LABELV $4050
line 4871
;4868:			}
;4869:		}
;4870:
;4871:		if (Q_stricmp(token, "joingametypes") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $4057
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $4055
line 4873
;4872:
;4873:			if (GameType_Parse(&p, qtrue)) {
ADDRLP4 4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 40
ADDRGP4 GameType_Parse
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $4043
line 4874
;4874:				continue;
ADDRGP4 $4042
JUMPV
line 4875
;4875:			} else {
line 4876
;4876:				break;
LABELV $4055
line 4880
;4877:			}
;4878:		}
;4879:
;4880:		if (Q_stricmp(token, "maps") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $4062
ARGP4
ADDRLP4 40
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
NEI4 $4060
line 4882
;4881:			// start a new menu
;4882:			MapList_Parse(&p);
ADDRLP4 4
ARGP4
ADDRGP4 MapList_Parse
CALLI4
pop
line 4883
;4883:		}
LABELV $4060
line 4885
;4884:
;4885:	}
LABELV $4042
line 4852
ADDRGP4 $4041
JUMPV
LABELV $4043
line 4886
;4886:}
LABELV $4038
endproc UI_ParseGameInfo 44 8
proc UI_Pause 4 8
line 4888
;4887:
;4888:static void UI_Pause(qboolean b) {
line 4889
;4889:	if (b) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $4064
line 4891
;4890:		// pause the game and set the ui keycatcher
;4891:	  trap_Cvar_Set( "cl_paused", "1" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4892
;4892:		trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 4893
;4893:	} else {
ADDRGP4 $4065
JUMPV
LABELV $4064
line 4895
;4894:		// unpause the game and clear the ui keycatcher
;4895:		trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 0
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 4896
;4896:		trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 4897
;4897:		trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 4898
;4898:	}
LABELV $4065
line 4899
;4899:}
LABELV $4063
endproc UI_Pause 4 8
proc UI_PlayCinematic 4 24
line 4908
;4900:
;4901:#ifndef MISSIONPACK // bk001206
;4902:static int UI_OwnerDraw_Width(int ownerDraw) {
;4903:  // bk001205 - LCC missing return value
;4904:  return 0;
;4905:}
;4906:#endif
;4907:
;4908:static int UI_PlayCinematic(const char *name, float x, float y, float w, float h) {
line 4909
;4909:  return trap_CIN_PlayCinematic(name, x, y, w, h, (CIN_loop | CIN_silent));
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 8
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 12
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 16
INDIRF4
CVFI4 4
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 0
ADDRGP4 trap_CIN_PlayCinematic
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $4066
endproc UI_PlayCinematic 4 24
proc UI_StopCinematic 20 4
line 4912
;4910:}
;4911:
;4912:static void UI_StopCinematic(int handle) {
line 4913
;4913:	if (handle >= 0) {
ADDRFP4 0
INDIRI4
CNSTI4 0
LTI4 $4068
line 4914
;4914:	  trap_CIN_StopCinematic(handle);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4915
;4915:	} else {
ADDRGP4 $4069
JUMPV
LABELV $4068
line 4916
;4916:		handle = abs(handle);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 abs
CALLI4
ASGNI4
ADDRFP4 0
ADDRLP4 0
INDIRI4
ASGNI4
line 4917
;4917:		if (handle == UI_MAPCINEMATIC) {
ADDRFP4 0
INDIRI4
CNSTI4 244
NEI4 $4070
line 4918
;4918:			if (uiInfo.mapList[ui_currentMap.integer].cinematic >= 0) {
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
CNSTI4 0
LTI4 $4071
line 4919
;4919:			  trap_CIN_StopCinematic(uiInfo.mapList[ui_currentMap.integer].cinematic);
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4920
;4920:			  uiInfo.mapList[ui_currentMap.integer].cinematic = -1;
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
CNSTI4 -1
ASGNI4
line 4921
;4921:			}
line 4922
;4922:		} else if (handle == UI_NETMAPCINEMATIC) {
ADDRGP4 $4071
JUMPV
LABELV $4070
ADDRFP4 0
INDIRI4
CNSTI4 246
NEI4 $4083
line 4923
;4923:			if (uiInfo.serverStatus.currentServerCinematic >= 0) {
ADDRGP4 uiInfo+99264+10432
INDIRI4
CNSTI4 0
LTI4 $4084
line 4924
;4924:			  trap_CIN_StopCinematic(uiInfo.serverStatus.currentServerCinematic);
ADDRGP4 uiInfo+99264+10432
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4925
;4925:				uiInfo.serverStatus.currentServerCinematic = -1;
ADDRGP4 uiInfo+99264+10432
CNSTI4 -1
ASGNI4
line 4926
;4926:			}
line 4927
;4927:		} else if (handle == UI_CLANCINEMATIC) {
ADDRGP4 $4084
JUMPV
LABELV $4083
ADDRFP4 0
INDIRI4
CNSTI4 251
NEI4 $4093
line 4928
;4928:		  int i = UI_TeamIndexFromName(UI_Cvar_VariableString("ui_teamName"));
ADDRGP4 $698
ARGP4
ADDRLP4 8
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 UI_TeamIndexFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
line 4929
;4929:		  if (i >= 0 && i < uiInfo.teamCount) {
ADDRLP4 16
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $4095
ADDRLP4 16
INDIRI4
ADDRGP4 uiInfo+75756
INDIRI4
GEI4 $4095
line 4930
;4930:				if (uiInfo.teamList[i].cinematic >= 0) {
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
CNSTI4 0
LTI4 $4098
line 4931
;4931:				  trap_CIN_StopCinematic(uiInfo.teamList[i].cinematic);
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_CIN_StopCinematic
CALLI4
pop
line 4932
;4932:					uiInfo.teamList[i].cinematic = -1;
CNSTI4 44
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 uiInfo+75760+40
ADDP4
CNSTI4 -1
ASGNI4
line 4933
;4933:				}
LABELV $4098
line 4934
;4934:			}
LABELV $4095
line 4935
;4935:		}
LABELV $4093
LABELV $4084
LABELV $4071
line 4936
;4936:	}
LABELV $4069
line 4937
;4937:}
LABELV $4067
endproc UI_StopCinematic 20 4
proc UI_DrawCinematic 0 20
line 4939
;4938:
;4939:static void UI_DrawCinematic(int handle, float x, float y, float w, float h) {
line 4940
;4940:	trap_CIN_SetExtents(handle, x, y, w, h);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 8
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 12
INDIRF4
CVFI4 4
ARGI4
ADDRFP4 16
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 trap_CIN_SetExtents
CALLV
pop
line 4941
;4941:  trap_CIN_DrawCinematic(handle);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 trap_CIN_DrawCinematic
CALLV
pop
line 4942
;4942:}
LABELV $4106
endproc UI_DrawCinematic 0 20
proc UI_RunCinematicFrame 0 4
line 4944
;4943:
;4944:static void UI_RunCinematicFrame(int handle) {
line 4945
;4945:  trap_CIN_RunCinematic(handle);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 trap_CIN_RunCinematic
CALLI4
pop
line 4946
;4946:}
LABELV $4107
endproc UI_RunCinematicFrame 0 4
proc UI_BuildQ3Model_List 4520 20
line 4956
;4947:
;4948:
;4949:
;4950:/*
;4951:=================
;4952:PlayerModel_BuildList
;4953:=================
;4954:*/
;4955:static void UI_BuildQ3Model_List( void )
;4956:{
line 4970
;4957:	int		numdirs;
;4958:	int		numfiles;
;4959:	char	dirlist[2048];
;4960:	char	filelist[2048];
;4961:	char	skinname[64];
;4962:	char	scratch[256];
;4963:	char*	dirptr;
;4964:	char*	fileptr;
;4965:	int		i;
;4966:	int		j, k, dirty;
;4967:	int		dirlen;
;4968:	int		filelen;
;4969:
;4970:	uiInfo.q3HeadCount = 0;
ADDRGP4 uiInfo+119488
CNSTI4 0
ASGNI4
line 4973
;4971:
;4972:	// iterate directory of all player models
;4973:	numdirs = trap_FS_GetFileList("models/players", "/", dirlist, 2048 );
ADDRGP4 $4110
ARGP4
ADDRGP4 $4111
ARGP4
ADDRLP4 2408
ARGP4
CNSTI4 2048
ARGI4
ADDRLP4 4456
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 2404
ADDRLP4 4456
INDIRI4
ASGNI4
line 4974
;4974:	dirptr  = dirlist;
ADDRLP4 340
ADDRLP4 2408
ASGNP4
line 4975
;4975:	for (i=0; i<numdirs && uiInfo.q3HeadCount < MAX_PLAYERMODELS; i++,dirptr+=dirlen+1)
ADDRLP4 352
CNSTI4 0
ASGNI4
ADDRGP4 $4115
JUMPV
LABELV $4112
line 4976
;4976:	{
line 4977
;4977:		dirlen = strlen(dirptr);
ADDRLP4 340
INDIRP4
ARGP4
ADDRLP4 4460
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 348
ADDRLP4 4460
INDIRI4
ASGNI4
line 4979
;4978:		
;4979:		if (dirlen && dirptr[dirlen-1]=='/') dirptr[dirlen-1]='\0';
ADDRLP4 348
INDIRI4
CNSTI4 0
EQI4 $4117
ADDRLP4 348
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 340
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $4117
ADDRLP4 348
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 340
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
LABELV $4117
line 4981
;4980:
;4981:		if (!strcmp(dirptr,".") || !strcmp(dirptr,".."))
ADDRLP4 340
INDIRP4
ARGP4
ADDRGP4 $4121
ARGP4
ADDRLP4 4468
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 4468
INDIRI4
CNSTI4 0
EQI4 $4123
ADDRLP4 340
INDIRP4
ARGP4
ADDRGP4 $4122
ARGP4
ADDRLP4 4472
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 4472
INDIRI4
CNSTI4 0
NEI4 $4119
LABELV $4123
line 4982
;4982:			continue;
ADDRGP4 $4113
JUMPV
LABELV $4119
line 4985
;4983:			
;4984:		// iterate all skin files in directory
;4985:		numfiles = trap_FS_GetFileList( va("models/players/%s",dirptr), "tga", filelist, 2048 );
ADDRGP4 $4124
ARGP4
ADDRLP4 340
INDIRP4
ARGP4
ADDRLP4 4476
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4476
INDIRP4
ARGP4
ADDRGP4 $4125
ARGP4
ADDRLP4 356
ARGP4
CNSTI4 2048
ARGI4
ADDRLP4 4480
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 344
ADDRLP4 4480
INDIRI4
ASGNI4
line 4986
;4986:		fileptr  = filelist;
ADDRLP4 328
ADDRLP4 356
ASGNP4
line 4987
;4987:		for (j=0; j<numfiles && uiInfo.q3HeadCount < MAX_PLAYERMODELS;j++,fileptr+=filelen+1)
ADDRLP4 332
CNSTI4 0
ASGNI4
ADDRGP4 $4129
JUMPV
LABELV $4126
line 4988
;4988:		{
line 4989
;4989:			filelen = strlen(fileptr);
ADDRLP4 328
INDIRP4
ARGP4
ADDRLP4 4484
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 336
ADDRLP4 4484
INDIRI4
ASGNI4
line 4991
;4990:
;4991:			COM_StripExtension(fileptr,skinname);
ADDRLP4 328
INDIRP4
ARGP4
ADDRLP4 260
ARGP4
ADDRGP4 COM_StripExtension
CALLV
pop
line 4994
;4992:
;4993:			// look for icon_????
;4994:			if (Q_stricmpn(skinname, "icon_", 5) == 0 && !(Q_stricmp(skinname,"icon_blue") == 0 || Q_stricmp(skinname,"icon_red") == 0))
ADDRLP4 260
ARGP4
ADDRGP4 $4133
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 4488
ADDRGP4 Q_stricmpn
CALLI4
ASGNI4
ADDRLP4 4488
INDIRI4
CNSTI4 0
NEI4 $4131
ADDRLP4 260
ARGP4
ADDRGP4 $4134
ARGP4
ADDRLP4 4492
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4492
INDIRI4
CNSTI4 0
EQI4 $4131
ADDRLP4 260
ARGP4
ADDRGP4 $4135
ARGP4
ADDRLP4 4496
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4496
INDIRI4
CNSTI4 0
EQI4 $4131
line 4995
;4995:			{
line 4996
;4996:				if (Q_stricmp(skinname, "icon_default") == 0) {
ADDRLP4 260
ARGP4
ADDRGP4 $4138
ARGP4
ADDRLP4 4500
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4500
INDIRI4
CNSTI4 0
NEI4 $4136
line 4997
;4997:					Com_sprintf( scratch, sizeof(scratch), dirptr);
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 340
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 4998
;4998:				} else {
ADDRGP4 $4137
JUMPV
LABELV $4136
line 4999
;4999:					Com_sprintf( scratch, sizeof(scratch), "%s/%s",dirptr, skinname + 5);
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $4139
ARGP4
ADDRLP4 340
INDIRP4
ARGP4
ADDRLP4 260+5
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 5000
;5000:				}
LABELV $4137
line 5001
;5001:				dirty = 0;
ADDRLP4 324
CNSTI4 0
ASGNI4
line 5002
;5002:				for(k=0;k<uiInfo.q3HeadCount;k++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $4144
JUMPV
LABELV $4141
line 5003
;5003:					if (!Q_stricmp(scratch, uiInfo.q3HeadNames[uiInfo.q3HeadCount])) {
ADDRLP4 4
ARGP4
ADDRGP4 uiInfo+119488
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+119492
ADDP4
ARGP4
ADDRLP4 4504
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4504
INDIRI4
CNSTI4 0
NEI4 $4146
line 5004
;5004:						dirty = 1;
ADDRLP4 324
CNSTI4 1
ASGNI4
line 5005
;5005:						break;
ADDRGP4 $4143
JUMPV
LABELV $4146
line 5007
;5006:					}
;5007:				}
LABELV $4142
line 5002
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $4144
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+119488
INDIRI4
LTI4 $4141
LABELV $4143
line 5008
;5008:				if (!dirty) {
ADDRLP4 324
INDIRI4
CNSTI4 0
NEI4 $4150
line 5009
;5009:					Com_sprintf( uiInfo.q3HeadNames[uiInfo.q3HeadCount], sizeof(uiInfo.q3HeadNames[uiInfo.q3HeadCount]), scratch);
ADDRGP4 uiInfo+119488
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 uiInfo+119492
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 5010
;5010:					uiInfo.q3HeadIcons[uiInfo.q3HeadCount++] = trap_R_RegisterShaderNoMip(va("models/players/%s/%s",dirptr,skinname));
ADDRLP4 4508
ADDRGP4 uiInfo+119488
ASGNP4
ADDRLP4 4504
ADDRLP4 4508
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 4508
INDIRP4
ADDRLP4 4504
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $4158
ARGP4
ADDRLP4 340
INDIRP4
ARGP4
ADDRLP4 260
ARGP4
ADDRLP4 4512
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4512
INDIRP4
ARGP4
ADDRLP4 4516
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 4504
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+135876
ADDP4
ADDRLP4 4516
INDIRI4
ASGNI4
line 5011
;5011:				}
LABELV $4150
line 5012
;5012:			}
LABELV $4131
line 5014
;5013:
;5014:		}
LABELV $4127
line 4987
ADDRLP4 4484
CNSTI4 1
ASGNI4
ADDRLP4 332
ADDRLP4 332
INDIRI4
ADDRLP4 4484
INDIRI4
ADDI4
ASGNI4
ADDRLP4 328
ADDRLP4 336
INDIRI4
ADDRLP4 4484
INDIRI4
ADDI4
ADDRLP4 328
INDIRP4
ADDP4
ASGNP4
LABELV $4129
ADDRLP4 332
INDIRI4
ADDRLP4 344
INDIRI4
GEI4 $4159
ADDRGP4 uiInfo+119488
INDIRI4
CNSTI4 256
LTI4 $4126
LABELV $4159
line 5015
;5015:	}	
LABELV $4113
line 4975
ADDRLP4 4460
CNSTI4 1
ASGNI4
ADDRLP4 352
ADDRLP4 352
INDIRI4
ADDRLP4 4460
INDIRI4
ADDI4
ASGNI4
ADDRLP4 340
ADDRLP4 348
INDIRI4
ADDRLP4 4460
INDIRI4
ADDI4
ADDRLP4 340
INDIRP4
ADDP4
ASGNP4
LABELV $4115
ADDRLP4 352
INDIRI4
ADDRLP4 2404
INDIRI4
GEI4 $4160
ADDRGP4 uiInfo+119488
INDIRI4
CNSTI4 256
LTI4 $4112
LABELV $4160
line 5017
;5016:
;5017:}
LABELV $4108
endproc UI_BuildQ3Model_List 4520 20
export _UI_Init
proc _UI_Init 52 16
line 5026
;5018:
;5019:
;5020:
;5021:/*
;5022:=================
;5023:UI_Init
;5024:=================
;5025:*/
;5026:void _UI_Init( qboolean inGameLoad ) {
line 5032
;5027:	const char *menuSet;
;5028:	int start;
;5029:
;5030:	//uiInfo.inGameLoad = inGameLoad;
;5031:
;5032:	UI_RegisterCvars();
ADDRGP4 UI_RegisterCvars
CALLV
pop
line 5033
;5033:	UI_InitMemory();
ADDRGP4 UI_InitMemory
CALLV
pop
line 5036
;5034:
;5035:	// cache redundant calulations
;5036:	trap_GetGlconfig( &uiInfo.uiDC.glconfig );
ADDRGP4 uiInfo+62068
ARGP4
ADDRGP4 trap_GetGlconfig
CALLV
pop
line 5039
;5037:
;5038:	// for 640x480 virtualized screen
;5039:	uiInfo.uiDC.yscale = uiInfo.uiDC.glconfig.vidHeight * (1.0/480.0);
ADDRGP4 uiInfo+196
CNSTF4 990414985
ADDRGP4 uiInfo+62068+11308
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 5040
;5040:	uiInfo.uiDC.xscale = uiInfo.uiDC.glconfig.vidWidth * (1.0/640.0);
ADDRGP4 uiInfo+200
CNSTF4 986500301
ADDRGP4 uiInfo+62068+11304
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 5041
;5041:	if ( uiInfo.uiDC.glconfig.vidWidth * 480 > uiInfo.uiDC.glconfig.vidHeight * 640 ) {
CNSTI4 480
ADDRGP4 uiInfo+62068+11304
INDIRI4
MULI4
CNSTI4 640
ADDRGP4 uiInfo+62068+11308
INDIRI4
MULI4
LEI4 $4169
line 5043
;5042:		// wide screen
;5043:		uiInfo.uiDC.bias = 0.5 * ( uiInfo.uiDC.glconfig.vidWidth - ( uiInfo.uiDC.glconfig.vidHeight * (640.0/480.0) ) );
ADDRGP4 uiInfo+204
CNSTF4 1056964608
ADDRGP4 uiInfo+62068+11304
INDIRI4
CVIF4 4
CNSTF4 1068149419
ADDRGP4 uiInfo+62068+11308
INDIRI4
CVIF4 4
MULF4
SUBF4
MULF4
ASGNF4
line 5044
;5044:	}
ADDRGP4 $4170
JUMPV
LABELV $4169
line 5045
;5045:	else {
line 5047
;5046:		// no wide screen
;5047:		uiInfo.uiDC.bias = 0;
ADDRGP4 uiInfo+204
CNSTF4 0
ASGNF4
line 5048
;5048:	}
LABELV $4170
line 5052
;5049:
;5050:
;5051:  //UI_Load();
;5052:	uiInfo.uiDC.registerShaderNoMip = &trap_R_RegisterShaderNoMip;
ADDRGP4 uiInfo
ADDRGP4 trap_R_RegisterShaderNoMip
ASGNP4
line 5053
;5053:	uiInfo.uiDC.setColor = &UI_SetColor;
ADDRGP4 uiInfo+4
ADDRGP4 UI_SetColor
ASGNP4
line 5054
;5054:	uiInfo.uiDC.drawHandlePic = &UI_DrawHandlePic;
ADDRGP4 uiInfo+8
ADDRGP4 UI_DrawHandlePic
ASGNP4
line 5055
;5055:	uiInfo.uiDC.drawStretchPic = &trap_R_DrawStretchPic;
ADDRGP4 uiInfo+12
ADDRGP4 trap_R_DrawStretchPic
ASGNP4
line 5056
;5056:	uiInfo.uiDC.drawText = &Text_Paint;
ADDRGP4 uiInfo+16
ADDRGP4 Text_Paint
ASGNP4
line 5057
;5057:	uiInfo.uiDC.textWidth = &Text_Width;
ADDRGP4 uiInfo+20
ADDRGP4 Text_Width
ASGNP4
line 5058
;5058:	uiInfo.uiDC.textHeight = &Text_Height;
ADDRGP4 uiInfo+24
ADDRGP4 Text_Height
ASGNP4
line 5059
;5059:	uiInfo.uiDC.registerModel = &trap_R_RegisterModel;
ADDRGP4 uiInfo+28
ADDRGP4 trap_R_RegisterModel
ASGNP4
line 5060
;5060:	uiInfo.uiDC.modelBounds = &trap_R_ModelBounds;
ADDRGP4 uiInfo+32
ADDRGP4 trap_R_ModelBounds
ASGNP4
line 5061
;5061:	uiInfo.uiDC.fillRect = &UI_FillRect;
ADDRGP4 uiInfo+36
ADDRGP4 UI_FillRect
ASGNP4
line 5062
;5062:	uiInfo.uiDC.drawRect = &_UI_DrawRect;
ADDRGP4 uiInfo+40
ADDRGP4 _UI_DrawRect
ASGNP4
line 5063
;5063:	uiInfo.uiDC.drawSides = &_UI_DrawSides;
ADDRGP4 uiInfo+44
ADDRGP4 _UI_DrawSides
ASGNP4
line 5064
;5064:	uiInfo.uiDC.drawTopBottom = &_UI_DrawTopBottom;
ADDRGP4 uiInfo+48
ADDRGP4 _UI_DrawTopBottom
ASGNP4
line 5065
;5065:	uiInfo.uiDC.clearScene = &trap_R_ClearScene;
ADDRGP4 uiInfo+52
ADDRGP4 trap_R_ClearScene
ASGNP4
line 5066
;5066:	uiInfo.uiDC.drawSides = &_UI_DrawSides;
ADDRGP4 uiInfo+44
ADDRGP4 _UI_DrawSides
ASGNP4
line 5067
;5067:	uiInfo.uiDC.addRefEntityToScene = &trap_R_AddRefEntityToScene;
ADDRGP4 uiInfo+56
ADDRGP4 trap_R_AddRefEntityToScene
ASGNP4
line 5068
;5068:	uiInfo.uiDC.renderScene = &trap_R_RenderScene;
ADDRGP4 uiInfo+60
ADDRGP4 trap_R_RenderScene
ASGNP4
line 5069
;5069:	uiInfo.uiDC.registerFont = &trap_R_RegisterFont;
ADDRGP4 uiInfo+64
ADDRGP4 trap_R_RegisterFont
ASGNP4
line 5070
;5070:	uiInfo.uiDC.ownerDrawItem = &UI_OwnerDraw;
ADDRGP4 uiInfo+68
ADDRGP4 UI_OwnerDraw
ASGNP4
line 5071
;5071:	uiInfo.uiDC.getValue = &UI_GetValue;
ADDRGP4 uiInfo+72
ADDRGP4 UI_GetValue
ASGNP4
line 5072
;5072:	uiInfo.uiDC.ownerDrawVisible = &UI_OwnerDrawVisible;
ADDRGP4 uiInfo+76
ADDRGP4 UI_OwnerDrawVisible
ASGNP4
line 5073
;5073:	uiInfo.uiDC.runScript = &UI_RunMenuScript;
ADDRGP4 uiInfo+80
ADDRGP4 UI_RunMenuScript
ASGNP4
line 5074
;5074:	uiInfo.uiDC.getTeamColor = &UI_GetTeamColor;
ADDRGP4 uiInfo+84
ADDRGP4 UI_GetTeamColor
ASGNP4
line 5075
;5075:	uiInfo.uiDC.setCVar = trap_Cvar_Set;
ADDRGP4 uiInfo+96
ADDRGP4 trap_Cvar_Set
ASGNP4
line 5076
;5076:	uiInfo.uiDC.getCVarString = trap_Cvar_VariableStringBuffer;
ADDRGP4 uiInfo+88
ADDRGP4 trap_Cvar_VariableStringBuffer
ASGNP4
line 5077
;5077:	uiInfo.uiDC.getCVarValue = trap_Cvar_VariableValue;
ADDRGP4 uiInfo+92
ADDRGP4 trap_Cvar_VariableValue
ASGNP4
line 5078
;5078:	uiInfo.uiDC.drawTextWithCursor = &Text_PaintWithCursor;
ADDRGP4 uiInfo+100
ADDRGP4 Text_PaintWithCursor
ASGNP4
line 5079
;5079:	uiInfo.uiDC.setOverstrikeMode = &trap_Key_SetOverstrikeMode;
ADDRGP4 uiInfo+104
ADDRGP4 trap_Key_SetOverstrikeMode
ASGNP4
line 5080
;5080:	uiInfo.uiDC.getOverstrikeMode = &trap_Key_GetOverstrikeMode;
ADDRGP4 uiInfo+108
ADDRGP4 trap_Key_GetOverstrikeMode
ASGNP4
line 5081
;5081:	uiInfo.uiDC.startLocalSound = &trap_S_StartLocalSound;
ADDRGP4 uiInfo+112
ADDRGP4 trap_S_StartLocalSound
ASGNP4
line 5082
;5082:	uiInfo.uiDC.ownerDrawHandleKey = &UI_OwnerDrawHandleKey;
ADDRGP4 uiInfo+116
ADDRGP4 UI_OwnerDrawHandleKey
ASGNP4
line 5083
;5083:	uiInfo.uiDC.feederCount = &UI_FeederCount;
ADDRGP4 uiInfo+120
ADDRGP4 UI_FeederCount
ASGNP4
line 5084
;5084:	uiInfo.uiDC.feederItemImage = &UI_FeederItemImage;
ADDRGP4 uiInfo+128
ADDRGP4 UI_FeederItemImage
ASGNP4
line 5085
;5085:	uiInfo.uiDC.feederItemText = &UI_FeederItemText;
ADDRGP4 uiInfo+124
ADDRGP4 UI_FeederItemText
ASGNP4
line 5086
;5086:	uiInfo.uiDC.feederSelection = &UI_FeederSelection;
ADDRGP4 uiInfo+132
ADDRGP4 UI_FeederSelection
ASGNP4
line 5087
;5087:	uiInfo.uiDC.setBinding = &trap_Key_SetBinding;
ADDRGP4 uiInfo+144
ADDRGP4 trap_Key_SetBinding
ASGNP4
line 5088
;5088:	uiInfo.uiDC.getBindingBuf = &trap_Key_GetBindingBuf;
ADDRGP4 uiInfo+140
ADDRGP4 trap_Key_GetBindingBuf
ASGNP4
line 5089
;5089:	uiInfo.uiDC.keynumToStringBuf = &trap_Key_KeynumToStringBuf;
ADDRGP4 uiInfo+136
ADDRGP4 trap_Key_KeynumToStringBuf
ASGNP4
line 5090
;5090:	uiInfo.uiDC.executeText = &trap_Cmd_ExecuteText;
ADDRGP4 uiInfo+148
ADDRGP4 trap_Cmd_ExecuteText
ASGNP4
line 5091
;5091:	uiInfo.uiDC.Error = &Com_Error; 
ADDRGP4 uiInfo+152
ADDRGP4 Com_Error
ASGNP4
line 5092
;5092:	uiInfo.uiDC.Print = &Com_Printf; 
ADDRGP4 uiInfo+156
ADDRGP4 Com_Printf
ASGNP4
line 5093
;5093:	uiInfo.uiDC.Pause = &UI_Pause;
ADDRGP4 uiInfo+160
ADDRGP4 UI_Pause
ASGNP4
line 5094
;5094:	uiInfo.uiDC.ownerDrawWidth = &UI_OwnerDrawWidth;
ADDRGP4 uiInfo+164
ADDRGP4 UI_OwnerDrawWidth
ASGNP4
line 5095
;5095:	uiInfo.uiDC.registerSound = &trap_S_RegisterSound;
ADDRGP4 uiInfo+168
ADDRGP4 trap_S_RegisterSound
ASGNP4
line 5096
;5096:	uiInfo.uiDC.startBackgroundTrack = &trap_S_StartBackgroundTrack;
ADDRGP4 uiInfo+172
ADDRGP4 trap_S_StartBackgroundTrack
ASGNP4
line 5097
;5097:	uiInfo.uiDC.stopBackgroundTrack = &trap_S_StopBackgroundTrack;
ADDRGP4 uiInfo+176
ADDRGP4 trap_S_StopBackgroundTrack
ASGNP4
line 5098
;5098:	uiInfo.uiDC.playCinematic = &UI_PlayCinematic;
ADDRGP4 uiInfo+180
ADDRGP4 UI_PlayCinematic
ASGNP4
line 5099
;5099:	uiInfo.uiDC.stopCinematic = &UI_StopCinematic;
ADDRGP4 uiInfo+184
ADDRGP4 UI_StopCinematic
ASGNP4
line 5100
;5100:	uiInfo.uiDC.drawCinematic = &UI_DrawCinematic;
ADDRGP4 uiInfo+188
ADDRGP4 UI_DrawCinematic
ASGNP4
line 5101
;5101:	uiInfo.uiDC.runCinematicFrame = &UI_RunCinematicFrame;
ADDRGP4 uiInfo+192
ADDRGP4 UI_RunCinematicFrame
ASGNP4
line 5103
;5102:
;5103:	Init_Display(&uiInfo.uiDC);
ADDRGP4 uiInfo
ARGP4
ADDRGP4 Init_Display
CALLV
pop
line 5105
;5104:
;5105:	String_Init();
ADDRGP4 String_Init
CALLV
pop
line 5107
;5106:  
;5107:	uiInfo.uiDC.cursor	= trap_R_RegisterShaderNoMip( "menu/art/3_cursor2" );
ADDRGP4 $4231
ARGP4
ADDRLP4 8
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+73408
ADDRLP4 8
INDIRI4
ASGNI4
line 5108
;5108:	uiInfo.uiDC.whiteShader = trap_R_RegisterShaderNoMip( "white" );
ADDRGP4 $4233
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiInfo+73400
ADDRLP4 12
INDIRI4
ASGNI4
line 5110
;5109:
;5110:	AssetCache();
ADDRGP4 AssetCache
CALLV
pop
line 5112
;5111:
;5112:	start = trap_Milliseconds();
ADDRLP4 16
ADDRGP4 trap_Milliseconds
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 5114
;5113:
;5114:  uiInfo.teamCount = 0;
ADDRGP4 uiInfo+75756
CNSTI4 0
ASGNI4
line 5115
;5115:  uiInfo.characterCount = 0;
ADDRGP4 uiInfo+73440
CNSTI4 0
ASGNI4
line 5116
;5116:  uiInfo.aliasCount = 0;
ADDRGP4 uiInfo+74984
CNSTI4 0
ASGNI4
line 5122
;5117:
;5118:#ifdef PRE_RELEASE_TADEMO
;5119:	UI_ParseTeamInfo("demoteaminfo.txt");
;5120:	UI_ParseGameInfo("demogameinfo.txt");
;5121:#else
;5122:	UI_ParseTeamInfo("teaminfo.txt");
ADDRGP4 $4237
ARGP4
ADDRGP4 UI_ParseTeamInfo
CALLV
pop
line 5123
;5123:	UI_LoadTeams();
ADDRGP4 UI_LoadTeams
CALLV
pop
line 5124
;5124:	UI_ParseGameInfo("gameinfo.txt");
ADDRGP4 $674
ARGP4
ADDRGP4 UI_ParseGameInfo
CALLV
pop
line 5127
;5125:#endif
;5126:
;5127:	menuSet = UI_Cvar_VariableString("ui_menuFiles");
ADDRGP4 $668
ARGP4
ADDRLP4 20
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 5128
;5128:	if (menuSet == NULL || menuSet[0] == '\0') {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4240
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $4238
LABELV $4240
line 5129
;5129:		menuSet = "ui/menus.txt";
ADDRLP4 0
ADDRGP4 $640
ASGNP4
line 5130
;5130:	}
LABELV $4238
line 5138
;5131:
;5132:#if 0
;5133:	if (uiInfo.inGameLoad) {
;5134:		UI_LoadMenus("ui/ingame.txt", qtrue);
;5135:	} else { // bk010222: left this: UI_LoadMenus(menuSet, qtrue);
;5136:	}
;5137:#else 
;5138:	UI_LoadMenus(menuSet, qtrue);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_LoadMenus
CALLV
pop
line 5139
;5139:	UI_LoadMenus("ui/ingame.txt", qfalse);
ADDRGP4 $4241
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_LoadMenus
CALLV
pop
line 5142
;5140:#endif
;5141:	
;5142:	Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5144
;5143:
;5144:	trap_LAN_LoadCachedServers();
ADDRGP4 trap_LAN_LoadCachedServers
CALLV
pop
line 5145
;5145:	UI_LoadBestScores(uiInfo.mapList[ui_currentMap.integer].mapLoadName, uiInfo.gameTypes[ui_gameType.integer].gtEnum);
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRGP4 ui_gameType+12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiInfo+78580+4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_LoadBestScores
CALLV
pop
line 5147
;5146:
;5147:	UI_BuildQ3Model_List();
ADDRGP4 UI_BuildQ3Model_List
CALLV
pop
line 5148
;5148:	UI_LoadBots();
ADDRGP4 UI_LoadBots
CALLV
pop
line 5151
;5149:
;5150:	// sets defaults for ui temp cvars
;5151:	uiInfo.effectsColor = gamecodetoui[(int)trap_Cvar_VariableValue("color1")-1];
ADDRGP4 $1756
ARGP4
ADDRLP4 28
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 uiInfo+136904
ADDRLP4 28
INDIRF4
CVFI4 4
CNSTI4 2
LSHI4
ADDRGP4 gamecodetoui-4
ADDP4
INDIRI4
ASGNI4
line 5152
;5152:	uiInfo.currentCrosshair = (int)trap_Cvar_VariableValue("cg_drawCrosshair");
ADDRGP4 $2049
ARGP4
ADDRLP4 32
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 uiInfo+119476
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 5153
;5153:	trap_Cvar_Set("ui_mousePitch", (trap_Cvar_VariableValue("m_pitch") >= 0) ? "0" : "1");
ADDRGP4 $2342
ARGP4
ADDRLP4 40
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 40
INDIRF4
CNSTF4 0
LTF4 $4252
ADDRLP4 36
ADDRGP4 $392
ASGNP4
ADDRGP4 $4253
JUMPV
LABELV $4252
ADDRLP4 36
ADDRGP4 $395
ASGNP4
LABELV $4253
ADDRGP4 $2339
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5155
;5154:
;5155:	uiInfo.serverStatus.currentServerCinematic = -1;
ADDRGP4 uiInfo+99264+10432
CNSTI4 -1
ASGNI4
line 5156
;5156:	uiInfo.previewMovie = -1;
ADDRGP4 uiInfo+99260
CNSTI4 -1
ASGNI4
line 5158
;5157:
;5158:	if (trap_Cvar_VariableValue("ui_TeamArenaFirstRun") == 0) {
ADDRGP4 $4259
ARGP4
ADDRLP4 44
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 44
INDIRF4
CNSTF4 0
NEF4 $4257
line 5159
;5159:		trap_Cvar_Set("s_volume", "0.8");
ADDRGP4 $4260
ARGP4
ADDRGP4 $4261
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5160
;5160:		trap_Cvar_Set("s_musicvolume", "0.5");
ADDRGP4 $4262
ARGP4
ADDRGP4 $4263
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5161
;5161:		trap_Cvar_Set("ui_TeamArenaFirstRun", "1");
ADDRGP4 $4259
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5162
;5162:	}
LABELV $4257
line 5164
;5163:
;5164:	trap_Cvar_Register(NULL, "debug_protocol", "", 0 );
CNSTP4 0
ARGP4
ADDRGP4 $4264
ARGP4
ADDRGP4 $109
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5166
;5165:
;5166:	trap_Cvar_Set("ui_actualNetGameType", va("%d", ui_netGameType.integer));
ADDRGP4 $713
ARGP4
ADDRGP4 ui_netGameType+12
INDIRI4
ARGI4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $728
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5167
;5167:}
LABELV $4161
endproc _UI_Init 52 16
export _UI_KeyEvent
proc _UI_KeyEvent 16 12
line 5175
;5168:
;5169:
;5170:/*
;5171:=================
;5172:UI_KeyEvent
;5173:=================
;5174:*/
;5175:void _UI_KeyEvent( int key, qboolean down ) {
line 5177
;5176:
;5177:  if (Menu_Count() > 0) {
ADDRLP4 0
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LEI4 $4267
line 5178
;5178:    menuDef_t *menu = Menu_GetFocused();
ADDRLP4 8
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 5179
;5179:		if (menu) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4269
line 5180
;5180:			if (key == K_ESCAPE && down && !Menus_AnyFullScreenVisible()) {
ADDRFP4 0
INDIRI4
CNSTI4 27
NEI4 $4271
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $4271
ADDRLP4 12
ADDRGP4 Menus_AnyFullScreenVisible
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $4271
line 5181
;5181:				Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5182
;5182:			} else {
ADDRGP4 $4270
JUMPV
LABELV $4271
line 5183
;5183:				Menu_HandleKey(menu, key, down );
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Menu_HandleKey
CALLV
pop
line 5184
;5184:			}
line 5185
;5185:		} else {
ADDRGP4 $4270
JUMPV
LABELV $4269
line 5186
;5186:			trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 12
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5187
;5187:			trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 5188
;5188:			trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5189
;5189:		}
LABELV $4270
line 5190
;5190:  }
LABELV $4267
line 5195
;5191:
;5192:  //if ((s > 0) && (s != menu_null_sound)) {
;5193:	//  trap_S_StartLocalSound( s, CHAN_LOCAL_SOUND );
;5194:  //}
;5195:}
LABELV $4266
endproc _UI_KeyEvent 16 12
export _UI_MouseEvent
proc _UI_MouseEvent 12 12
line 5203
;5196:
;5197:/*
;5198:=================
;5199:UI_MouseEvent
;5200:=================
;5201:*/
;5202:void _UI_MouseEvent( int dx, int dy )
;5203:{
line 5205
;5204:	// update mouse screen position
;5205:	uiInfo.uiDC.cursorx += dx;
ADDRLP4 0
ADDRGP4 uiInfo+216
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 5206
;5206:	if (uiInfo.uiDC.cursorx < 0)
ADDRGP4 uiInfo+216
INDIRI4
CNSTI4 0
GEI4 $4275
line 5207
;5207:		uiInfo.uiDC.cursorx = 0;
ADDRGP4 uiInfo+216
CNSTI4 0
ASGNI4
ADDRGP4 $4276
JUMPV
LABELV $4275
line 5208
;5208:	else if (uiInfo.uiDC.cursorx > SCREEN_WIDTH)
ADDRGP4 uiInfo+216
INDIRI4
CNSTI4 640
LEI4 $4279
line 5209
;5209:		uiInfo.uiDC.cursorx = SCREEN_WIDTH;
ADDRGP4 uiInfo+216
CNSTI4 640
ASGNI4
LABELV $4279
LABELV $4276
line 5211
;5210:
;5211:	uiInfo.uiDC.cursory += dy;
ADDRLP4 4
ADDRGP4 uiInfo+220
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
line 5212
;5212:	if (uiInfo.uiDC.cursory < 0)
ADDRGP4 uiInfo+220
INDIRI4
CNSTI4 0
GEI4 $4284
line 5213
;5213:		uiInfo.uiDC.cursory = 0;
ADDRGP4 uiInfo+220
CNSTI4 0
ASGNI4
ADDRGP4 $4285
JUMPV
LABELV $4284
line 5214
;5214:	else if (uiInfo.uiDC.cursory > SCREEN_HEIGHT)
ADDRGP4 uiInfo+220
INDIRI4
CNSTI4 480
LEI4 $4288
line 5215
;5215:		uiInfo.uiDC.cursory = SCREEN_HEIGHT;
ADDRGP4 uiInfo+220
CNSTI4 480
ASGNI4
LABELV $4288
LABELV $4285
line 5217
;5216:
;5217:  if (Menu_Count() > 0) {
ADDRLP4 8
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LEI4 $4292
line 5220
;5218:    //menuDef_t *menu = Menu_GetFocused();
;5219:    //Menu_HandleMouseMove(menu, uiInfo.uiDC.cursorx, uiInfo.uiDC.cursory);
;5220:		Display_MouseMove(NULL, uiInfo.uiDC.cursorx, uiInfo.uiDC.cursory);
CNSTP4 0
ARGP4
ADDRGP4 uiInfo+216
INDIRI4
ARGI4
ADDRGP4 uiInfo+220
INDIRI4
ARGI4
ADDRGP4 Display_MouseMove
CALLI4
pop
line 5221
;5221:  }
LABELV $4292
line 5223
;5222:
;5223:}
LABELV $4273
endproc _UI_MouseEvent 12 12
export UI_LoadNonIngame
proc UI_LoadNonIngame 12 8
line 5225
;5224:
;5225:void UI_LoadNonIngame() {
line 5226
;5226:	const char *menuSet = UI_Cvar_VariableString("ui_menuFiles");
ADDRGP4 $668
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 5227
;5227:	if (menuSet == NULL || menuSet[0] == '\0') {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4299
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $4297
LABELV $4299
line 5228
;5228:		menuSet = "ui/menus.txt";
ADDRLP4 0
ADDRGP4 $640
ASGNP4
line 5229
;5229:	}
LABELV $4297
line 5230
;5230:	UI_LoadMenus(menuSet, qfalse);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_LoadMenus
CALLV
pop
line 5231
;5231:	uiInfo.inGameLoad = qfalse;
ADDRGP4 uiInfo+136908
CNSTI4 0
ASGNI4
line 5232
;5232:}
LABELV $4296
endproc UI_LoadNonIngame 12 8
export _UI_SetActiveMenu
proc _UI_SetActiveMenu 292 12
line 5234
;5233:
;5234:void _UI_SetActiveMenu( uiMenuCommand_t menu ) {
line 5239
;5235:	char buf[256];
;5236:
;5237:	// this should be the ONLY way the menu system is brought up
;5238:	// enusure minumum menu data is cached
;5239:  if (Menu_Count() > 0) {
ADDRLP4 256
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 256
INDIRI4
CNSTI4 0
LEI4 $4302
line 5241
;5240:		vec3_t v;
;5241:		v[0] = v[1] = v[2] = 0;
ADDRLP4 272
CNSTF4 0
ASGNF4
ADDRLP4 260+8
ADDRLP4 272
INDIRF4
ASGNF4
ADDRLP4 260+4
ADDRLP4 272
INDIRF4
ASGNF4
ADDRLP4 260
ADDRLP4 272
INDIRF4
ASGNF4
line 5242
;5242:	  switch ( menu ) {
ADDRLP4 276
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 276
INDIRI4
CNSTI4 0
LTI4 $4306
ADDRLP4 276
INDIRI4
CNSTI4 6
GTI4 $4306
ADDRLP4 276
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $4330
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $4330
address $4309
address $4310
address $4328
address $4301
address $4301
address $4320
address $4323
code
LABELV $4309
line 5244
;5243:	  case UIMENU_NONE:
;5244:			trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
ADDRLP4 284
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 284
INDIRI4
CNSTI4 -3
BANDI4
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5245
;5245:			trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 5246
;5246:			trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5247
;5247:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5249
;5248:
;5249:		  return;
ADDRGP4 $4301
JUMPV
LABELV $4310
line 5252
;5250:	  case UIMENU_MAIN:
;5251:			//trap_Cvar_Set( "sv_killserver", "1" );
;5252:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5255
;5253:			//trap_S_StartLocalSound( trap_S_RegisterSound("sound/misc/menu_background.wav", qfalse) , CHAN_LOCAL_SOUND );
;5254:			//trap_S_StartBackgroundTrack("sound/misc/menu_background.wav", NULL);
;5255:			if (uiInfo.inGameLoad) {
ADDRGP4 uiInfo+136908
INDIRI4
CNSTI4 0
EQI4 $4311
line 5256
;5256:				UI_LoadNonIngame();
ADDRGP4 UI_LoadNonIngame
CALLV
pop
line 5257
;5257:			}
LABELV $4311
line 5258
;5258:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5259
;5259:			Menus_ActivateByName("main");
ADDRGP4 $2517
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 5260
;5260:			trap_Cvar_VariableStringBuffer("com_errorMessage", buf, sizeof(buf));
ADDRGP4 $2438
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 5261
;5261:			if (strlen(buf)) {
ADDRLP4 0
ARGP4
ADDRLP4 288
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 288
INDIRI4
CNSTI4 0
EQI4 $4301
line 5262
;5262:				if (!ui_singlePlayerActive.integer) {
ADDRGP4 ui_singlePlayerActive+12
INDIRI4
CNSTI4 0
NEI4 $4316
line 5263
;5263:					Menus_ActivateByName("error_popmenu");
ADDRGP4 $4319
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 5264
;5264:				} else {
ADDRGP4 $4301
JUMPV
LABELV $4316
line 5265
;5265:					trap_Cvar_Set("com_errorMessage", "");
ADDRGP4 $2438
ARGP4
ADDRGP4 $109
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5266
;5266:				}
line 5267
;5267:			}
line 5268
;5268:		  return;
ADDRGP4 $4301
JUMPV
LABELV $4320
line 5270
;5269:	  case UIMENU_TEAM:
;5270:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5271
;5271:      Menus_ActivateByName("team");
ADDRGP4 $2134
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 5272
;5272:		  return;
ADDRGP4 $4301
JUMPV
line 5278
;5273:	  case UIMENU_NEED_CD:
;5274:			// no cd check in TA
;5275:			//trap_Key_SetCatcher( KEYCATCH_UI );
;5276:      //Menus_ActivateByName("needcd");
;5277:		  //UI_ConfirmMenu( "Insert the CD", NULL, NeedCDAction );
;5278:		  return;
line 5284
;5279:	  case UIMENU_BAD_CD_KEY:
;5280:			// no cd check in TA
;5281:			//trap_Key_SetCatcher( KEYCATCH_UI );
;5282:      //Menus_ActivateByName("badcd");
;5283:		  //UI_ConfirmMenu( "Bad CD Key", NULL, NeedCDKeyAction );
;5284:		  return;
LABELV $4323
line 5287
;5285:	  case UIMENU_POSTGAME:
;5286:			//trap_Cvar_Set( "sv_killserver", "1" );
;5287:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5288
;5288:			if (uiInfo.inGameLoad) {
ADDRGP4 uiInfo+136908
INDIRI4
CNSTI4 0
EQI4 $4324
line 5289
;5289:				UI_LoadNonIngame();
ADDRGP4 UI_LoadNonIngame
CALLV
pop
line 5290
;5290:			}
LABELV $4324
line 5291
;5291:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5292
;5292:			Menus_ActivateByName("endofgame");
ADDRGP4 $4327
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 5294
;5293:		  //UI_ConfirmMenu( "Bad CD Key", NULL, NeedCDKeyAction );
;5294:		  return;
ADDRGP4 $4301
JUMPV
LABELV $4328
line 5296
;5295:	  case UIMENU_INGAME:
;5296:		  trap_Cvar_Set( "cl_paused", "1" );
ADDRGP4 $2587
ARGP4
ADDRGP4 $395
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5297
;5297:			trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 5298
;5298:			UI_BuildPlayerList();
ADDRGP4 UI_BuildPlayerList
CALLV
pop
line 5299
;5299:			Menus_CloseAll();
ADDRGP4 Menus_CloseAll
CALLV
pop
line 5300
;5300:			Menus_ActivateByName("ingame");
ADDRGP4 $4329
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 5301
;5301:		  return;
LABELV $4306
line 5303
;5302:	  }
;5303:  }
LABELV $4302
line 5304
;5304:}
LABELV $4301
endproc _UI_SetActiveMenu 292 12
export _UI_IsFullscreen
proc _UI_IsFullscreen 4 0
line 5306
;5305:
;5306:qboolean _UI_IsFullscreen( void ) {
line 5307
;5307:	return Menus_AnyFullScreenVisible();
ADDRLP4 0
ADDRGP4 Menus_AnyFullScreenVisible
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $4331
endproc _UI_IsFullscreen 4 0
proc UI_ReadableSize 16 16
line 5316
;5308:}
;5309:
;5310:
;5311:
;5312:static connstate_t	lastConnState;
;5313:static char			lastLoadingText[MAX_INFO_VALUE];
;5314:
;5315:static void UI_ReadableSize ( char *buf, int bufsize, int value )
;5316:{
line 5317
;5317:	if (value > 1024*1024*1024 ) { // gigs
ADDRFP4 8
INDIRI4
CNSTI4 1073741824
LEI4 $4333
line 5318
;5318:		Com_sprintf( buf, bufsize, "%d", value / (1024*1024*1024) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $713
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1073741824
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5319
;5319:		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d GB", 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ARGI4
ADDRGP4 $4335
ARGP4
ADDRLP4 12
CNSTI4 1073741824
ASGNI4
CNSTI4 100
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
MODI4
MULI4
ADDRLP4 12
INDIRI4
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5321
;5320:			(value % (1024*1024*1024))*100 / (1024*1024*1024) );
;5321:	} else if (value > 1024*1024 ) { // megs
ADDRGP4 $4334
JUMPV
LABELV $4333
ADDRFP4 8
INDIRI4
CNSTI4 1048576
LEI4 $4336
line 5322
;5322:		Com_sprintf( buf, bufsize, "%d", value / (1024*1024) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $713
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1048576
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5323
;5323:		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d MB", 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ARGI4
ADDRGP4 $4338
ARGP4
ADDRLP4 12
CNSTI4 1048576
ASGNI4
CNSTI4 100
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
MODI4
MULI4
ADDRLP4 12
INDIRI4
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5325
;5324:			(value % (1024*1024))*100 / (1024*1024) );
;5325:	} else if (value > 1024 ) { // kilos
ADDRGP4 $4337
JUMPV
LABELV $4336
ADDRFP4 8
INDIRI4
CNSTI4 1024
LEI4 $4339
line 5326
;5326:		Com_sprintf( buf, bufsize, "%d KB", value / 1024 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $4341
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1024
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5327
;5327:	} else { // bytes
ADDRGP4 $4340
JUMPV
LABELV $4339
line 5328
;5328:		Com_sprintf( buf, bufsize, "%d bytes", value );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $4342
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5329
;5329:	}
LABELV $4340
LABELV $4337
LABELV $4334
line 5330
;5330:}
LABELV $4332
endproc UI_ReadableSize 16 16
proc UI_PrintTime 8 20
line 5333
;5331:
;5332:// Assumes time is in msec
;5333:static void UI_PrintTime ( char *buf, int bufsize, int time ) {
line 5334
;5334:	time /= 1000;  // change to seconds
ADDRFP4 8
ADDRFP4 8
INDIRI4
CNSTI4 1000
DIVI4
ASGNI4
line 5336
;5335:
;5336:	if (time > 3600) { // in the hours range
ADDRFP4 8
INDIRI4
CNSTI4 3600
LEI4 $4344
line 5337
;5337:		Com_sprintf( buf, bufsize, "%d hr %d min", time / 3600, (time % 3600) / 60 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $4346
ARGP4
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 3600
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
DIVI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
CNSTI4 60
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5338
;5338:	} else if (time > 60) { // mins
ADDRGP4 $4345
JUMPV
LABELV $4344
ADDRFP4 8
INDIRI4
CNSTI4 60
LEI4 $4347
line 5339
;5339:		Com_sprintf( buf, bufsize, "%d min %d sec", time / 60, time % 60 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $4349
ARGP4
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 60
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
DIVI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5340
;5340:	} else  { // secs
ADDRGP4 $4348
JUMPV
LABELV $4347
line 5341
;5341:		Com_sprintf( buf, bufsize, "%d sec", time );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $4350
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 5342
;5342:	}
LABELV $4348
LABELV $4345
line 5343
;5343:}
LABELV $4343
endproc UI_PrintTime 8 20
export Text_PaintCenter
proc Text_PaintCenter 8 32
line 5345
;5344:
;5345:void Text_PaintCenter(float x, float y, float scale, vec4_t color, const char *text, float adjust) {
line 5346
;5346:	int len = Text_Width(text, scale, 0);
ADDRFP4 16
INDIRP4
ARGP4
ADDRFP4 8
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 Text_Width
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 5347
;5347:	Text_Paint(x - len / 2, y, scale, color, text, 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE);
ADDRFP4 0
INDIRF4
ADDRLP4 0
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 Text_Paint
CALLV
pop
line 5348
;5348:}
LABELV $4351
endproc Text_PaintCenter 8 32
export Text_PaintCenter_AutoWrapped
proc Text_PaintCenter_AutoWrapped 1056 24
line 5350
;5349:
;5350:void Text_PaintCenter_AutoWrapped(float x, float y, float xmax, float ystep, float scale, vec4_t color, const char *str, float adjust) {
line 5356
;5351:	int width;
;5352:	char *s1,*s2,*s3;
;5353:	char c_bcp;
;5354:	char buf[1024];
;5355:
;5356:	if (!str || str[0]=='\0')
ADDRLP4 1044
ADDRFP4 24
INDIRP4
ASGNP4
ADDRLP4 1044
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4355
ADDRLP4 1044
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $4353
LABELV $4355
line 5357
;5357:		return;
ADDRGP4 $4352
JUMPV
LABELV $4353
line 5359
;5358:
;5359:	Q_strncpyz(buf, str, sizeof(buf));
ADDRLP4 20
ARGP4
ADDRFP4 24
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 5360
;5360:	s1 = s2 = s3 = buf;
ADDRLP4 0
ADDRLP4 20
ASGNP4
ADDRLP4 4
ADDRLP4 20
ASGNP4
ADDRLP4 12
ADDRLP4 20
ASGNP4
ADDRGP4 $4357
JUMPV
LABELV $4356
line 5362
;5361:
;5362:	while (1) {
LABELV $4359
line 5363
;5363:		do {
line 5364
;5364:			s3++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 5365
;5365:		} while (*s3!=' ' && *s3!='\0');
LABELV $4360
ADDRLP4 1048
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 32
EQI4 $4362
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $4359
LABELV $4362
line 5366
;5366:		c_bcp = *s3;
ADDRLP4 8
ADDRLP4 0
INDIRP4
INDIRI1
ASGNI1
line 5367
;5367:		*s3 = '\0';
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 5368
;5368:		width = Text_Width(s1, scale, 0);
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 1052
ADDRGP4 Text_Width
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 1052
INDIRI4
ASGNI4
line 5369
;5369:		*s3 = c_bcp;
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI1
ASGNI1
line 5370
;5370:		if (width > xmax) {
ADDRLP4 16
INDIRI4
CVIF4 4
ADDRFP4 8
INDIRF4
LEF4 $4363
line 5371
;5371:			if (s1==s2)
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
NEU4 $4365
line 5372
;5372:			{
line 5374
;5373:				// fuck, don't have a clean cut, we'll overflow
;5374:				s2 = s3;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 5375
;5375:			}
LABELV $4365
line 5376
;5376:			*s2 = '\0';
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 5377
;5377:			Text_PaintCenter(x, y, scale, color, s1, adjust);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 20
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 28
INDIRF4
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5378
;5378:			y += ystep;
ADDRFP4 4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ASGNF4
line 5379
;5379:			if (c_bcp == '\0')
ADDRLP4 8
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $4367
line 5380
;5380:      {
line 5385
;5381:				// that was the last word
;5382:        // we could start a new loop, but that wouldn't be much use
;5383:        // even if the word is too long, we would overflow it (see above)
;5384:        // so just print it now if needed
;5385:        s2++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 5386
;5386:        if (*s2 != '\0') // if we are printing an overflowing line we have s2 == s3
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $4358
line 5387
;5387:          Text_PaintCenter(x, y, scale, color, s2, adjust);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 20
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 28
INDIRF4
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5388
;5388:        break;
ADDRGP4 $4358
JUMPV
LABELV $4367
line 5390
;5389:      }
;5390:			s2++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 5391
;5391:			s1 = s2;
ADDRLP4 12
ADDRLP4 4
INDIRP4
ASGNP4
line 5392
;5392:			s3 = s2;
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 5393
;5393:		}
ADDRGP4 $4364
JUMPV
LABELV $4363
line 5395
;5394:		else
;5395:		{
line 5396
;5396:			s2 = s3;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 5397
;5397:			if (c_bcp == '\0') // we reached the end
ADDRLP4 8
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $4371
line 5398
;5398:			{
line 5399
;5399:				Text_PaintCenter(x, y, scale, color, s1, adjust);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 16
INDIRF4
ARGF4
ADDRFP4 20
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 28
INDIRF4
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5400
;5400:				break;
ADDRGP4 $4358
JUMPV
LABELV $4371
line 5402
;5401:			}
;5402:		}
LABELV $4364
line 5403
;5403:	}
LABELV $4357
line 5362
ADDRGP4 $4356
JUMPV
LABELV $4358
line 5404
;5404:}
LABELV $4352
endproc Text_PaintCenter_AutoWrapped 1056 24
data
align 1
LABELV $4374
byte 1 68
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 0
align 1
LABELV $4375
byte 1 69
byte 1 115
byte 1 116
byte 1 105
byte 1 109
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $4376
byte 1 84
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 102
byte 1 101
byte 1 114
byte 1 32
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 58
byte 1 0
code
proc UI_DisplayDownloadInfo 312 24
line 5406
;5405:
;5406:static void UI_DisplayDownloadInfo( const char *downloadName, float centerPoint, float yStart, float scale ) {
line 5417
;5407:	static char dlText[]	= "Downloading:";
;5408:	static char etaText[]	= "Estimated time left:";
;5409:	static char xferText[]	= "Transfer rate:";
;5410:
;5411:	int downloadSize, downloadCount, downloadTime;
;5412:	char dlSizeBuf[64], totalSizeBuf[64], xferRateBuf[64], dlTimeBuf[64];
;5413:	int xferRate;
;5414:	int leftWidth;
;5415:	const char *s;
;5416:
;5417:	downloadSize = trap_Cvar_VariableValue( "cl_downloadSize" );
ADDRGP4 $4377
ARGP4
ADDRLP4 280
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 280
INDIRF4
CVFI4 4
ASGNI4
line 5418
;5418:	downloadCount = trap_Cvar_VariableValue( "cl_downloadCount" );
ADDRGP4 $4378
ARGP4
ADDRLP4 284
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 284
INDIRF4
CVFI4 4
ASGNI4
line 5419
;5419:	downloadTime = trap_Cvar_VariableValue( "cl_downloadTime" );
ADDRGP4 $4379
ARGP4
ADDRLP4 288
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 140
ADDRLP4 288
INDIRF4
CVFI4 4
ASGNI4
line 5421
;5420:
;5421:	leftWidth = 320;
ADDRLP4 8
CNSTI4 320
ASGNI4
line 5423
;5422:
;5423:	UI_SetColor(colorWhite);
ADDRGP4 colorWhite
ARGP4
ADDRGP4 UI_SetColor
CALLV
pop
line 5424
;5424:	Text_PaintCenter(centerPoint, yStart + 112, scale, colorWhite, dlText, 0);
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1121976320
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 $4374
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5425
;5425:	Text_PaintCenter(centerPoint, yStart + 192, scale, colorWhite, etaText, 0);
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1128267776
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 $4375
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5426
;5426:	Text_PaintCenter(centerPoint, yStart + 248, scale, colorWhite, xferText, 0);
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1131937792
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 $4376
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5428
;5427:
;5428:	if (downloadSize > 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LEI4 $4380
line 5429
;5429:		s = va( "%s (%d%%)", downloadName, downloadCount * 100 / downloadSize );
ADDRGP4 $4382
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRLP4 4
INDIRI4
MULI4
ADDRLP4 0
INDIRI4
DIVI4
ARGI4
ADDRLP4 292
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 148
ADDRLP4 292
INDIRP4
ASGNP4
line 5430
;5430:	} else {
ADDRGP4 $4381
JUMPV
LABELV $4380
line 5431
;5431:		s = downloadName;
ADDRLP4 148
ADDRFP4 0
INDIRP4
ASGNP4
line 5432
;5432:	}
LABELV $4381
line 5434
;5433:
;5434:	Text_PaintCenter(centerPoint, yStart+136, scale, colorWhite, s, 0);
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1124597760
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 148
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5436
;5435:
;5436:	UI_ReadableSize( dlSizeBuf,		sizeof dlSizeBuf,		downloadCount );
ADDRLP4 12
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 5437
;5437:	UI_ReadableSize( totalSizeBuf,	sizeof totalSizeBuf,	downloadSize );
ADDRLP4 76
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 5439
;5438:
;5439:	if (downloadCount < 4096 || !downloadTime) {
ADDRLP4 4
INDIRI4
CNSTI4 4096
LTI4 $4385
ADDRLP4 140
INDIRI4
CNSTI4 0
NEI4 $4383
LABELV $4385
line 5440
;5440:		Text_PaintCenter(leftWidth, yStart+216, scale, colorWhite, "estimating", 0);
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1129840640
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 $4386
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5441
;5441:		Text_PaintCenter(leftWidth, yStart+160, scale, colorWhite, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), 0);
ADDRGP4 $4387
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 292
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1126170624
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 292
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5442
;5442:	} else {
ADDRGP4 $4384
JUMPV
LABELV $4383
line 5443
;5443:		if ((uiInfo.uiDC.realTime - downloadTime) / 1000) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRLP4 140
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
CNSTI4 0
EQI4 $4388
line 5444
;5444:			xferRate = downloadCount / ((uiInfo.uiDC.realTime - downloadTime) / 1000);
ADDRLP4 144
ADDRLP4 4
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
ADDRLP4 140
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
DIVI4
ASGNI4
line 5445
;5445:		} else {
ADDRGP4 $4389
JUMPV
LABELV $4388
line 5446
;5446:			xferRate = 0;
ADDRLP4 144
CNSTI4 0
ASGNI4
line 5447
;5447:		}
LABELV $4389
line 5448
;5448:		UI_ReadableSize( xferRateBuf, sizeof xferRateBuf, xferRate );
ADDRLP4 152
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 144
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 5451
;5449:
;5450:		// Extrapolate estimated completion time
;5451:		if (downloadSize && xferRate) {
ADDRLP4 292
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 292
INDIRI4
EQI4 $4392
ADDRLP4 144
INDIRI4
ADDRLP4 292
INDIRI4
EQI4 $4392
line 5452
;5452:			int n = downloadSize / xferRate; // estimated time for entire d/l in secs
ADDRLP4 296
ADDRLP4 0
INDIRI4
ADDRLP4 144
INDIRI4
DIVI4
ASGNI4
line 5455
;5453:
;5454:			// We do it in K (/1024) because we'd overflow around 4MB
;5455:			UI_PrintTime ( dlTimeBuf, sizeof dlTimeBuf, 
ADDRLP4 216
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 300
ADDRLP4 296
INDIRI4
ASGNI4
ADDRLP4 304
CNSTI4 1024
ASGNI4
CNSTI4 1000
ADDRLP4 300
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 304
INDIRI4
DIVI4
ADDRLP4 300
INDIRI4
MULI4
ADDRLP4 0
INDIRI4
ADDRLP4 304
INDIRI4
DIVI4
DIVI4
SUBI4
MULI4
ARGI4
ADDRGP4 UI_PrintTime
CALLV
pop
line 5458
;5456:				(n - (((downloadCount/1024) * n) / (downloadSize/1024))) * 1000);
;5457:
;5458:			Text_PaintCenter(leftWidth, yStart+216, scale, colorWhite, dlTimeBuf, 0);
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1129840640
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 216
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5459
;5459:			Text_PaintCenter(leftWidth, yStart+160, scale, colorWhite, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), 0);
ADDRGP4 $4387
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 308
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1126170624
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 308
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5460
;5460:		} else {
ADDRGP4 $4393
JUMPV
LABELV $4392
line 5461
;5461:			Text_PaintCenter(leftWidth, yStart+216, scale, colorWhite, "estimating", 0);
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1129840640
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 $4386
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5462
;5462:			if (downloadSize) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $4394
line 5463
;5463:				Text_PaintCenter(leftWidth, yStart+160, scale, colorWhite, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), 0);
ADDRGP4 $4387
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 296
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1126170624
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 296
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5464
;5464:			} else {
ADDRGP4 $4395
JUMPV
LABELV $4394
line 5465
;5465:				Text_PaintCenter(leftWidth, yStart+160, scale, colorWhite, va("(%s copied)", dlSizeBuf), 0);
ADDRGP4 $4396
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 296
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1126170624
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 296
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5466
;5466:			}
LABELV $4395
line 5467
;5467:		}
LABELV $4393
line 5469
;5468:
;5469:		if (xferRate) {
ADDRLP4 144
INDIRI4
CNSTI4 0
EQI4 $4397
line 5470
;5470:			Text_PaintCenter(leftWidth, yStart+272, scale, colorWhite, va("%s/Sec", xferRateBuf), 0);
ADDRGP4 $4399
ARGP4
ADDRLP4 152
ARGP4
ADDRLP4 296
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRF4
CNSTF4 1132986368
ADDF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 296
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5471
;5471:		}
LABELV $4397
line 5472
;5472:	}
LABELV $4384
line 5473
;5473:}
LABELV $4373
endproc UI_DisplayDownloadInfo 312 24
export UI_DrawConnectScreen
proc UI_DrawConnectScreen 5440 32
line 5483
;5474:
;5475:/*
;5476:========================
;5477:UI_DrawConnectScreen
;5478:
;5479:This will also be overlaid on the cgame info screen during loading
;5480:to prevent it from blinking away too rapidly on local or lan games.
;5481:========================
;5482:*/
;5483:void UI_DrawConnectScreen( qboolean overlay ) {
line 5490
;5484:	char			*s;
;5485:	uiClientState_t	cstate;
;5486:	char			info[MAX_INFO_VALUE];
;5487:	char text[256];
;5488:	float centerPoint, yStart, scale;
;5489:	
;5490:	menuDef_t *menu = Menus_FindByName("Connect");
ADDRGP4 $4401
ARGP4
ADDRLP4 4384
ADDRGP4 Menus_FindByName
CALLP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4384
INDIRP4
ASGNP4
line 5493
;5491:
;5492:
;5493:	if ( !overlay && menu ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $4402
ADDRLP4 4120
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $4402
line 5494
;5494:		Menu_Paint(menu, qtrue);
ADDRLP4 4120
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_Paint
CALLV
pop
line 5495
;5495:	}
LABELV $4402
line 5497
;5496:
;5497:	if (!overlay) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $4404
line 5498
;5498:		centerPoint = 320;
ADDRLP4 3084
CNSTF4 1134559232
ASGNF4
line 5499
;5499:		yStart = 130;
ADDRLP4 3092
CNSTF4 1124204544
ASGNF4
line 5500
;5500:		scale = 0.5f;
ADDRLP4 3088
CNSTF4 1056964608
ASGNF4
line 5501
;5501:	} else {
ADDRGP4 $4405
JUMPV
LABELV $4404
line 5502
;5502:		centerPoint = 320;
ADDRLP4 3084
CNSTF4 1134559232
ASGNF4
line 5503
;5503:		yStart = 32;
ADDRLP4 3092
CNSTF4 1107296256
ASGNF4
line 5504
;5504:		scale = 0.6f;
ADDRLP4 3088
CNSTF4 1058642330
ASGNF4
line 5505
;5505:		return;
ADDRGP4 $4400
JUMPV
LABELV $4405
line 5509
;5506:	}
;5507:
;5508:	// see what information we should display
;5509:	trap_GetClientState( &cstate );
ADDRLP4 0
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 5511
;5510:
;5511:	info[0] = '\0';
ADDRLP4 3096
CNSTI1 0
ASGNI1
line 5512
;5512:	if( trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) ) ) {
CNSTI4 0
ARGI4
ADDRLP4 3096
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4388
ADDRGP4 trap_GetConfigString
CALLI4
ASGNI4
ADDRLP4 4388
INDIRI4
CNSTI4 0
EQI4 $4406
line 5513
;5513:		Text_PaintCenter(centerPoint, yStart, scale, colorWhite, va( "Loading %s", Info_ValueForKey( info, "mapname" )), 0);
ADDRLP4 3096
ARGP4
ADDRGP4 $3086
ARGP4
ADDRLP4 4392
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $4408
ARGP4
ADDRLP4 4392
INDIRP4
ARGP4
ADDRLP4 4396
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 4396
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5514
;5514:	}
LABELV $4406
line 5516
;5515:
;5516:	if (!Q_stricmp(cstate.servername,"localhost")) {
ADDRLP4 0+12
ARGP4
ADDRGP4 $4412
ARGP4
ADDRLP4 4392
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4392
INDIRI4
CNSTI4 0
NEI4 $4409
line 5517
;5517:		Text_PaintCenter(centerPoint, yStart + 48, scale, colorWhite, va("Starting up..."), ITEM_TEXTSTYLE_SHADOWEDMORE);
ADDRGP4 $4413
ARGP4
ADDRLP4 4396
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
CNSTF4 1111490560
ADDF4
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 4396
INDIRP4
ARGP4
CNSTF4 1086324736
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5518
;5518:	} else {
ADDRGP4 $4410
JUMPV
LABELV $4409
line 5519
;5519:		strcpy(text, va("Connecting to %s", cstate.servername));
ADDRGP4 $4414
ARGP4
ADDRLP4 0+12
ARGP4
ADDRLP4 4396
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4124
ARGP4
ADDRLP4 4396
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 5520
;5520:		Text_PaintCenter(centerPoint, yStart + 48, scale, colorWhite,text , ITEM_TEXTSTYLE_SHADOWEDMORE);
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
CNSTF4 1111490560
ADDF4
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 4124
ARGP4
CNSTF4 1086324736
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5521
;5521:	}
LABELV $4410
line 5524
;5522:
;5523:	// display global MOTD at bottom
;5524:	Text_PaintCenter(centerPoint, 600, scale, colorWhite, Info_ValueForKey( cstate.updateInfoString, "motd" ), 0);
ADDRLP4 0+1036
ARGP4
ADDRGP4 $4417
ARGP4
ADDRLP4 4396
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3084
INDIRF4
ARGF4
CNSTF4 1142292480
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 4396
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5526
;5525:	// print any server info (server full, bad version, etc)
;5526:	if ( cstate.connState < CA_CONNECTED ) {
ADDRLP4 0
INDIRI4
CNSTI4 5
GEI4 $4418
line 5527
;5527:		Text_PaintCenter_AutoWrapped(centerPoint, yStart + 176, 630, 20, scale, colorWhite, cstate.messageString, 0);
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
CNSTF4 1127219200
ADDF4
ARGF4
CNSTF4 1142784000
ARGF4
CNSTF4 1101004800
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 0+2060
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter_AutoWrapped
CALLV
pop
line 5528
;5528:	}
LABELV $4418
line 5530
;5529:
;5530:	if ( lastConnState > cstate.connState ) {
ADDRGP4 lastConnState
INDIRI4
ADDRLP4 0
INDIRI4
LEI4 $4421
line 5531
;5531:		lastLoadingText[0] = '\0';
ADDRGP4 lastLoadingText
CNSTI1 0
ASGNI1
line 5532
;5532:	}
LABELV $4421
line 5533
;5533:	lastConnState = cstate.connState;
ADDRGP4 lastConnState
ADDRLP4 0
INDIRI4
ASGNI4
line 5535
;5534:
;5535:	switch ( cstate.connState ) {
ADDRLP4 4400
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 4400
INDIRI4
CNSTI4 3
LTI4 $4400
ADDRLP4 4400
INDIRI4
CNSTI4 7
GTI4 $4400
ADDRLP4 4400
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $4439-12
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $4439
address $4426
address $4429
address $4432
address $4400
address $4400
code
LABELV $4426
line 5537
;5536:	case CA_CONNECTING:
;5537:		s = va("Awaiting connection...%i", cstate.connectPacketCount);
ADDRGP4 $4427
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 4408
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4380
ADDRLP4 4408
INDIRP4
ASGNP4
line 5538
;5538:		break;
ADDRGP4 $4424
JUMPV
LABELV $4429
line 5540
;5539:	case CA_CHALLENGING:
;5540:		s = va("Awaiting challenge...%i", cstate.connectPacketCount);
ADDRGP4 $4430
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 4412
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4380
ADDRLP4 4412
INDIRP4
ASGNP4
line 5541
;5541:		break;
ADDRGP4 $4424
JUMPV
LABELV $4432
line 5542
;5542:	case CA_CONNECTED: {
line 5545
;5543:		char downloadName[MAX_INFO_VALUE];
;5544:
;5545:			trap_Cvar_VariableStringBuffer( "cl_downloadName", downloadName, sizeof(downloadName) );
ADDRGP4 $4433
ARGP4
ADDRLP4 4416
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 5546
;5546:			if (*downloadName) {
ADDRLP4 4416
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $4434
line 5547
;5547:				UI_DisplayDownloadInfo( downloadName, centerPoint, yStart, scale );
ADDRLP4 4416
ARGP4
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 UI_DisplayDownloadInfo
CALLV
pop
line 5548
;5548:				return;
ADDRGP4 $4400
JUMPV
LABELV $4434
line 5550
;5549:			}
;5550:		}
line 5551
;5551:		s = "Awaiting gamestate...";
ADDRLP4 4380
ADDRGP4 $4436
ASGNP4
line 5552
;5552:		break;
line 5554
;5553:	case CA_LOADING:
;5554:		return;
line 5556
;5555:	case CA_PRIMED:
;5556:		return;
line 5558
;5557:	default:
;5558:		return;
LABELV $4424
line 5562
;5559:	}
;5560:
;5561:
;5562:	if (Q_stricmp(cstate.servername,"localhost")) {
ADDRLP4 0+12
ARGP4
ADDRGP4 $4412
ARGP4
ADDRLP4 4408
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4408
INDIRI4
CNSTI4 0
EQI4 $4441
line 5563
;5563:		Text_PaintCenter(centerPoint, yStart + 80, scale, colorWhite, s, 0);
ADDRLP4 3084
INDIRF4
ARGF4
ADDRLP4 3092
INDIRF4
CNSTF4 1117782016
ADDF4
ARGF4
ADDRLP4 3088
INDIRF4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRLP4 4380
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Text_PaintCenter
CALLV
pop
line 5564
;5564:	}
LABELV $4441
line 5567
;5565:
;5566:	// password required / connection rejected information goes here
;5567:}
LABELV $4400
endproc UI_DrawConnectScreen 5440 32
data
align 4
LABELV cvarTable
address ui_ffa_fraglimit
address $4445
address $691
byte 4 1
address ui_ffa_timelimit
address $4446
address $392
byte 4 1
address ui_tourney_fraglimit
address $4447
address $392
byte 4 1
address ui_tourney_timelimit
address $4448
address $692
byte 4 1
address ui_team_fraglimit
address $4449
address $392
byte 4 1
address ui_team_timelimit
address $4450
address $691
byte 4 1
address ui_team_friendly
address $4451
address $395
byte 4 1
address ui_ctf_capturelimit
address $4452
address $4453
byte 4 1
address ui_ctf_timelimit
address $4454
address $689
byte 4 1
address ui_ctf_friendly
address $4455
address $392
byte 4 1
address ui_arenasFile
address $4456
address $109
byte 4 80
address ui_botsFile
address $4457
address $109
byte 4 80
address ui_spScores1
address $4458
address $109
byte 4 65
address ui_spScores2
address $4459
address $109
byte 4 65
address ui_spScores3
address $4460
address $109
byte 4 65
address ui_spScores4
address $4461
address $109
byte 4 65
address ui_spScores5
address $4462
address $109
byte 4 65
address ui_spAwards
address $4463
address $109
byte 4 65
address ui_spVideos
address $4464
address $109
byte 4 65
address ui_spSkill
address $824
address $2236
byte 4 1
address ui_spSelection
address $4465
address $109
byte 4 64
address ui_browserMaster
address $4466
address $392
byte 4 1
address ui_browserGameType
address $4467
address $392
byte 4 1
address ui_browserSortKey
address $4468
address $2771
byte 4 1
address ui_browserShowFull
address $4469
address $395
byte 4 1
address ui_browserShowEmpty
address $4470
address $395
byte 4 1
address ui_brassTime
address $2329
address $4471
byte 4 1
address ui_drawCrosshair
address $2049
address $2771
byte 4 1
address ui_drawCrosshairNames
address $4472
address $395
byte 4 1
address ui_marks
address $4473
address $395
byte 4 1
address ui_server1
address $4474
address $109
byte 4 1
address ui_server2
address $4475
address $109
byte 4 1
address ui_server3
address $4476
address $109
byte 4 1
address ui_server4
address $4477
address $109
byte 4 1
address ui_server5
address $4478
address $109
byte 4 1
address ui_server6
address $4479
address $109
byte 4 1
address ui_server7
address $4480
address $109
byte 4 1
address ui_server8
address $4481
address $109
byte 4 1
address ui_server9
address $4482
address $109
byte 4 1
address ui_server10
address $4483
address $109
byte 4 1
address ui_server11
address $4484
address $109
byte 4 1
address ui_server12
address $4485
address $109
byte 4 1
address ui_server13
address $4486
address $109
byte 4 1
address ui_server14
address $4487
address $109
byte 4 1
address ui_server15
address $4488
address $109
byte 4 1
address ui_server16
address $4489
address $109
byte 4 1
address ui_cdkeychecked
address $4490
address $392
byte 4 64
address ui_new
address $4491
address $392
byte 4 256
address ui_debug
address $4492
address $392
byte 4 256
address ui_initialized
address $4493
address $392
byte 4 256
address ui_teamName
address $698
address $4494
byte 4 1
address ui_opponentName
address $1099
address $4495
byte 4 1
address ui_redteam
address $4496
address $4494
byte 4 1
address ui_blueteam
address $4497
address $4495
byte 4 1
address ui_dedicated
address $4498
address $392
byte 4 1
address ui_gameType
address $4499
address $4500
byte 4 1
address ui_joinGameType
address $4501
address $392
byte 4 1
address ui_netGameType
address $4502
address $4500
byte 4 1
address ui_actualNetGameType
address $4503
address $4500
byte 4 1
address ui_redteam1
address $4504
address $392
byte 4 1
address ui_redteam2
address $4505
address $392
byte 4 1
address ui_redteam3
address $4506
address $392
byte 4 1
address ui_redteam4
address $4507
address $392
byte 4 1
address ui_redteam5
address $4508
address $392
byte 4 1
address ui_blueteam1
address $4509
address $392
byte 4 1
address ui_blueteam2
address $4510
address $392
byte 4 1
address ui_blueteam3
address $4511
address $392
byte 4 1
address ui_blueteam4
address $4512
address $392
byte 4 1
address ui_blueteam5
address $4513
address $392
byte 4 1
address ui_netSource
address $1956
address $392
byte 4 1
address ui_menuFiles
address $668
address $640
byte 4 1
address ui_currentTier
address $1021
address $392
byte 4 1
address ui_currentMap
address $888
address $392
byte 4 1
address ui_currentNetMap
address $886
address $392
byte 4 1
address ui_mapIndex
address $2196
address $392
byte 4 1
address ui_currentOpponent
address $4514
address $392
byte 4 1
address ui_selectedPlayer
address $1391
address $392
byte 4 1
address ui_selectedPlayerName
address $1399
address $109
byte 4 1
address ui_lastServerRefresh_0
address $4515
address $109
byte 4 1
address ui_lastServerRefresh_1
address $4516
address $109
byte 4 1
address ui_lastServerRefresh_2
address $4517
address $109
byte 4 1
address ui_lastServerRefresh_3
address $4518
address $109
byte 4 1
address ui_singlePlayerActive
address $2209
address $392
byte 4 0
address ui_scoreAccuracy
address $4519
address $392
byte 4 1
address ui_scoreImpressives
address $4520
address $392
byte 4 1
address ui_scoreExcellents
address $4521
address $392
byte 4 1
address ui_scoreCaptures
address $4522
address $392
byte 4 1
address ui_scoreDefends
address $4523
address $392
byte 4 1
address ui_scoreAssists
address $4524
address $392
byte 4 1
address ui_scoreGauntlets
address $4525
address $392
byte 4 1
address ui_scoreScore
address $4526
address $392
byte 4 1
address ui_scorePerfect
address $4527
address $392
byte 4 1
address ui_scoreTeam
address $4528
address $4529
byte 4 1
address ui_scoreBase
address $4530
address $392
byte 4 1
address ui_scoreTime
address $4531
address $4532
byte 4 1
address ui_scoreTimeBonus
address $4533
address $392
byte 4 1
address ui_scoreSkillBonus
address $4534
address $392
byte 4 1
address ui_scoreShutoutBonus
address $4535
address $392
byte 4 1
address ui_fragLimit
address $714
address $693
byte 4 0
address ui_captureLimit
address $712
address $694
byte 4 0
address ui_smallFont
address $4536
address $4537
byte 4 1
address ui_bigFont
address $4538
address $4539
byte 4 1
address ui_findPlayer
address $3174
address $4540
byte 4 1
address ui_Q3Model
address $4541
address $392
byte 4 1
address ui_hudFiles
address $4542
address $4543
byte 4 1
address ui_recordSPDemo
address $2228
address $392
byte 4 1
address ui_teamArenaFirstRun
address $4544
address $392
byte 4 1
address ui_realWarmUp
address $2220
address $691
byte 4 1
address ui_realCaptureLimit
address $715
address $4453
byte 4 1029
address ui_serverStatusTimeOut
address $4545
address $4546
byte 4 1
align 4
LABELV cvarTableSize
byte 4 110
export UI_RegisterCvars
code
proc UI_RegisterCvars 12 16
line 5834
;5568:
;5569:
;5570:/*
;5571:================
;5572:cvars
;5573:================
;5574:*/
;5575:
;5576:typedef struct {
;5577:	vmCvar_t	*vmCvar;
;5578:	char		*cvarName;
;5579:	char		*defaultString;
;5580:	int			cvarFlags;
;5581:} cvarTable_t;
;5582:
;5583:vmCvar_t	ui_ffa_fraglimit;
;5584:vmCvar_t	ui_ffa_timelimit;
;5585:
;5586:vmCvar_t	ui_tourney_fraglimit;
;5587:vmCvar_t	ui_tourney_timelimit;
;5588:
;5589:vmCvar_t	ui_team_fraglimit;
;5590:vmCvar_t	ui_team_timelimit;
;5591:vmCvar_t	ui_team_friendly;
;5592:
;5593:vmCvar_t	ui_ctf_capturelimit;
;5594:vmCvar_t	ui_ctf_timelimit;
;5595:vmCvar_t	ui_ctf_friendly;
;5596:
;5597:vmCvar_t	ui_arenasFile;
;5598:vmCvar_t	ui_botsFile;
;5599:vmCvar_t	ui_spScores1;
;5600:vmCvar_t	ui_spScores2;
;5601:vmCvar_t	ui_spScores3;
;5602:vmCvar_t	ui_spScores4;
;5603:vmCvar_t	ui_spScores5;
;5604:vmCvar_t	ui_spAwards;
;5605:vmCvar_t	ui_spVideos;
;5606:vmCvar_t	ui_spSkill;
;5607:
;5608:vmCvar_t	ui_spSelection;
;5609:
;5610:vmCvar_t	ui_browserMaster;
;5611:vmCvar_t	ui_browserGameType;
;5612:vmCvar_t	ui_browserSortKey;
;5613:vmCvar_t	ui_browserShowFull;
;5614:vmCvar_t	ui_browserShowEmpty;
;5615:
;5616:vmCvar_t	ui_brassTime;
;5617:vmCvar_t	ui_drawCrosshair;
;5618:vmCvar_t	ui_drawCrosshairNames;
;5619:vmCvar_t	ui_marks;
;5620:
;5621:vmCvar_t	ui_server1;
;5622:vmCvar_t	ui_server2;
;5623:vmCvar_t	ui_server3;
;5624:vmCvar_t	ui_server4;
;5625:vmCvar_t	ui_server5;
;5626:vmCvar_t	ui_server6;
;5627:vmCvar_t	ui_server7;
;5628:vmCvar_t	ui_server8;
;5629:vmCvar_t	ui_server9;
;5630:vmCvar_t	ui_server10;
;5631:vmCvar_t	ui_server11;
;5632:vmCvar_t	ui_server12;
;5633:vmCvar_t	ui_server13;
;5634:vmCvar_t	ui_server14;
;5635:vmCvar_t	ui_server15;
;5636:vmCvar_t	ui_server16;
;5637:
;5638:vmCvar_t	ui_cdkeychecked;
;5639:
;5640:vmCvar_t	ui_redteam;
;5641:vmCvar_t	ui_redteam1;
;5642:vmCvar_t	ui_redteam2;
;5643:vmCvar_t	ui_redteam3;
;5644:vmCvar_t	ui_redteam4;
;5645:vmCvar_t	ui_redteam5;
;5646:vmCvar_t	ui_blueteam;
;5647:vmCvar_t	ui_blueteam1;
;5648:vmCvar_t	ui_blueteam2;
;5649:vmCvar_t	ui_blueteam3;
;5650:vmCvar_t	ui_blueteam4;
;5651:vmCvar_t	ui_blueteam5;
;5652:vmCvar_t	ui_teamName;
;5653:vmCvar_t	ui_dedicated;
;5654:vmCvar_t	ui_gameType;
;5655:vmCvar_t	ui_netGameType;
;5656:vmCvar_t	ui_actualNetGameType;
;5657:vmCvar_t	ui_joinGameType;
;5658:vmCvar_t	ui_netSource;
;5659:vmCvar_t	ui_serverFilterType;
;5660:vmCvar_t	ui_opponentName;
;5661:vmCvar_t	ui_menuFiles;
;5662:vmCvar_t	ui_currentTier;
;5663:vmCvar_t	ui_currentMap;
;5664:vmCvar_t	ui_currentNetMap;
;5665:vmCvar_t	ui_mapIndex;
;5666:vmCvar_t	ui_currentOpponent;
;5667:vmCvar_t	ui_selectedPlayer;
;5668:vmCvar_t	ui_selectedPlayerName;
;5669:vmCvar_t	ui_lastServerRefresh_0;
;5670:vmCvar_t	ui_lastServerRefresh_1;
;5671:vmCvar_t	ui_lastServerRefresh_2;
;5672:vmCvar_t	ui_lastServerRefresh_3;
;5673:vmCvar_t	ui_singlePlayerActive;
;5674:vmCvar_t	ui_scoreAccuracy;
;5675:vmCvar_t	ui_scoreImpressives;
;5676:vmCvar_t	ui_scoreExcellents;
;5677:vmCvar_t	ui_scoreCaptures;
;5678:vmCvar_t	ui_scoreDefends;
;5679:vmCvar_t	ui_scoreAssists;
;5680:vmCvar_t	ui_scoreGauntlets;
;5681:vmCvar_t	ui_scoreScore;
;5682:vmCvar_t	ui_scorePerfect;
;5683:vmCvar_t	ui_scoreTeam;
;5684:vmCvar_t	ui_scoreBase;
;5685:vmCvar_t	ui_scoreTimeBonus;
;5686:vmCvar_t	ui_scoreSkillBonus;
;5687:vmCvar_t	ui_scoreShutoutBonus;
;5688:vmCvar_t	ui_scoreTime;
;5689:vmCvar_t	ui_captureLimit;
;5690:vmCvar_t	ui_fragLimit;
;5691:vmCvar_t	ui_smallFont;
;5692:vmCvar_t	ui_bigFont;
;5693:vmCvar_t	ui_findPlayer;
;5694:vmCvar_t	ui_Q3Model;
;5695:vmCvar_t	ui_hudFiles;
;5696:vmCvar_t	ui_recordSPDemo;
;5697:vmCvar_t	ui_realCaptureLimit;
;5698:vmCvar_t	ui_realWarmUp;
;5699:vmCvar_t	ui_serverStatusTimeOut;
;5700:
;5701:
;5702:// bk001129 - made static to avoid aliasing
;5703:static cvarTable_t		cvarTable[] = {
;5704:	{ &ui_ffa_fraglimit, "ui_ffa_fraglimit", "20", CVAR_ARCHIVE },
;5705:	{ &ui_ffa_timelimit, "ui_ffa_timelimit", "0", CVAR_ARCHIVE },
;5706:
;5707:	{ &ui_tourney_fraglimit, "ui_tourney_fraglimit", "0", CVAR_ARCHIVE },
;5708:	{ &ui_tourney_timelimit, "ui_tourney_timelimit", "15", CVAR_ARCHIVE },
;5709:
;5710:	{ &ui_team_fraglimit, "ui_team_fraglimit", "0", CVAR_ARCHIVE },
;5711:	{ &ui_team_timelimit, "ui_team_timelimit", "20", CVAR_ARCHIVE },
;5712:	{ &ui_team_friendly, "ui_team_friendly",  "1", CVAR_ARCHIVE },
;5713:
;5714:	{ &ui_ctf_capturelimit, "ui_ctf_capturelimit", "8", CVAR_ARCHIVE },
;5715:	{ &ui_ctf_timelimit, "ui_ctf_timelimit", "30", CVAR_ARCHIVE },
;5716:	{ &ui_ctf_friendly, "ui_ctf_friendly",  "0", CVAR_ARCHIVE },
;5717:
;5718:	{ &ui_arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM },
;5719:	{ &ui_botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM },
;5720:	{ &ui_spScores1, "g_spScores1", "", CVAR_ARCHIVE | CVAR_ROM },
;5721:	{ &ui_spScores2, "g_spScores2", "", CVAR_ARCHIVE | CVAR_ROM },
;5722:	{ &ui_spScores3, "g_spScores3", "", CVAR_ARCHIVE | CVAR_ROM },
;5723:	{ &ui_spScores4, "g_spScores4", "", CVAR_ARCHIVE | CVAR_ROM },
;5724:	{ &ui_spScores5, "g_spScores5", "", CVAR_ARCHIVE | CVAR_ROM },
;5725:	{ &ui_spAwards, "g_spAwards", "", CVAR_ARCHIVE | CVAR_ROM },
;5726:	{ &ui_spVideos, "g_spVideos", "", CVAR_ARCHIVE | CVAR_ROM },
;5727:	{ &ui_spSkill, "g_spSkill", "2", CVAR_ARCHIVE },
;5728:
;5729:	{ &ui_spSelection, "ui_spSelection", "", CVAR_ROM },
;5730:
;5731:	{ &ui_browserMaster, "ui_browserMaster", "0", CVAR_ARCHIVE },
;5732:	{ &ui_browserGameType, "ui_browserGameType", "0", CVAR_ARCHIVE },
;5733:	{ &ui_browserSortKey, "ui_browserSortKey", "4", CVAR_ARCHIVE },
;5734:	{ &ui_browserShowFull, "ui_browserShowFull", "1", CVAR_ARCHIVE },
;5735:	{ &ui_browserShowEmpty, "ui_browserShowEmpty", "1", CVAR_ARCHIVE },
;5736:
;5737:	{ &ui_brassTime, "cg_brassTime", "2500", CVAR_ARCHIVE },
;5738:	{ &ui_drawCrosshair, "cg_drawCrosshair", "4", CVAR_ARCHIVE },
;5739:	{ &ui_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
;5740:	{ &ui_marks, "cg_marks", "1", CVAR_ARCHIVE },
;5741:
;5742:	{ &ui_server1, "server1", "", CVAR_ARCHIVE },
;5743:	{ &ui_server2, "server2", "", CVAR_ARCHIVE },
;5744:	{ &ui_server3, "server3", "", CVAR_ARCHIVE },
;5745:	{ &ui_server4, "server4", "", CVAR_ARCHIVE },
;5746:	{ &ui_server5, "server5", "", CVAR_ARCHIVE },
;5747:	{ &ui_server6, "server6", "", CVAR_ARCHIVE },
;5748:	{ &ui_server7, "server7", "", CVAR_ARCHIVE },
;5749:	{ &ui_server8, "server8", "", CVAR_ARCHIVE },
;5750:	{ &ui_server9, "server9", "", CVAR_ARCHIVE },
;5751:	{ &ui_server10, "server10", "", CVAR_ARCHIVE },
;5752:	{ &ui_server11, "server11", "", CVAR_ARCHIVE },
;5753:	{ &ui_server12, "server12", "", CVAR_ARCHIVE },
;5754:	{ &ui_server13, "server13", "", CVAR_ARCHIVE },
;5755:	{ &ui_server14, "server14", "", CVAR_ARCHIVE },
;5756:	{ &ui_server15, "server15", "", CVAR_ARCHIVE },
;5757:	{ &ui_server16, "server16", "", CVAR_ARCHIVE },
;5758:	{ &ui_cdkeychecked, "ui_cdkeychecked", "0", CVAR_ROM },
;5759:	{ &ui_new, "ui_new", "0", CVAR_TEMP },
;5760:	{ &ui_debug, "ui_debug", "0", CVAR_TEMP },
;5761:	{ &ui_initialized, "ui_initialized", "0", CVAR_TEMP },
;5762:	{ &ui_teamName, "ui_teamName", "Pagans", CVAR_ARCHIVE },
;5763:	{ &ui_opponentName, "ui_opponentName", "Stroggs", CVAR_ARCHIVE },
;5764:	{ &ui_redteam, "ui_redteam", "Pagans", CVAR_ARCHIVE },
;5765:	{ &ui_blueteam, "ui_blueteam", "Stroggs", CVAR_ARCHIVE },
;5766:	{ &ui_dedicated, "ui_dedicated", "0", CVAR_ARCHIVE },
;5767:	{ &ui_gameType, "ui_gametype", "3", CVAR_ARCHIVE },
;5768:	{ &ui_joinGameType, "ui_joinGametype", "0", CVAR_ARCHIVE },
;5769:	{ &ui_netGameType, "ui_netGametype", "3", CVAR_ARCHIVE },
;5770:	{ &ui_actualNetGameType, "ui_actualNetGametype", "3", CVAR_ARCHIVE },
;5771:	{ &ui_redteam1, "ui_redteam1", "0", CVAR_ARCHIVE },
;5772:	{ &ui_redteam2, "ui_redteam2", "0", CVAR_ARCHIVE },
;5773:	{ &ui_redteam3, "ui_redteam3", "0", CVAR_ARCHIVE },
;5774:	{ &ui_redteam4, "ui_redteam4", "0", CVAR_ARCHIVE },
;5775:	{ &ui_redteam5, "ui_redteam5", "0", CVAR_ARCHIVE },
;5776:	{ &ui_blueteam1, "ui_blueteam1", "0", CVAR_ARCHIVE },
;5777:	{ &ui_blueteam2, "ui_blueteam2", "0", CVAR_ARCHIVE },
;5778:	{ &ui_blueteam3, "ui_blueteam3", "0", CVAR_ARCHIVE },
;5779:	{ &ui_blueteam4, "ui_blueteam4", "0", CVAR_ARCHIVE },
;5780:	{ &ui_blueteam5, "ui_blueteam5", "0", CVAR_ARCHIVE },
;5781:	{ &ui_netSource, "ui_netSource", "0", CVAR_ARCHIVE },
;5782:	{ &ui_menuFiles, "ui_menuFiles", "ui/menus.txt", CVAR_ARCHIVE },
;5783:	{ &ui_currentTier, "ui_currentTier", "0", CVAR_ARCHIVE },
;5784:	{ &ui_currentMap, "ui_currentMap", "0", CVAR_ARCHIVE },
;5785:	{ &ui_currentNetMap, "ui_currentNetMap", "0", CVAR_ARCHIVE },
;5786:	{ &ui_mapIndex, "ui_mapIndex", "0", CVAR_ARCHIVE },
;5787:	{ &ui_currentOpponent, "ui_currentOpponent", "0", CVAR_ARCHIVE },
;5788:	{ &ui_selectedPlayer, "cg_selectedPlayer", "0", CVAR_ARCHIVE},
;5789:	{ &ui_selectedPlayerName, "cg_selectedPlayerName", "", CVAR_ARCHIVE},
;5790:	{ &ui_lastServerRefresh_0, "ui_lastServerRefresh_0", "", CVAR_ARCHIVE},
;5791:	{ &ui_lastServerRefresh_1, "ui_lastServerRefresh_1", "", CVAR_ARCHIVE},
;5792:	{ &ui_lastServerRefresh_2, "ui_lastServerRefresh_2", "", CVAR_ARCHIVE},
;5793:	{ &ui_lastServerRefresh_3, "ui_lastServerRefresh_3", "", CVAR_ARCHIVE},
;5794:	{ &ui_singlePlayerActive, "ui_singlePlayerActive", "0", 0},
;5795:	{ &ui_scoreAccuracy, "ui_scoreAccuracy", "0", CVAR_ARCHIVE},
;5796:	{ &ui_scoreImpressives, "ui_scoreImpressives", "0", CVAR_ARCHIVE},
;5797:	{ &ui_scoreExcellents, "ui_scoreExcellents", "0", CVAR_ARCHIVE},
;5798:	{ &ui_scoreCaptures, "ui_scoreCaptures", "0", CVAR_ARCHIVE},
;5799:	{ &ui_scoreDefends, "ui_scoreDefends", "0", CVAR_ARCHIVE},
;5800:	{ &ui_scoreAssists, "ui_scoreAssists", "0", CVAR_ARCHIVE},
;5801:	{ &ui_scoreGauntlets, "ui_scoreGauntlets", "0",CVAR_ARCHIVE},
;5802:	{ &ui_scoreScore, "ui_scoreScore", "0", CVAR_ARCHIVE},
;5803:	{ &ui_scorePerfect, "ui_scorePerfect", "0", CVAR_ARCHIVE},
;5804:	{ &ui_scoreTeam, "ui_scoreTeam", "0 to 0", CVAR_ARCHIVE},
;5805:	{ &ui_scoreBase, "ui_scoreBase", "0", CVAR_ARCHIVE},
;5806:	{ &ui_scoreTime, "ui_scoreTime", "00:00", CVAR_ARCHIVE},
;5807:	{ &ui_scoreTimeBonus, "ui_scoreTimeBonus", "0", CVAR_ARCHIVE},
;5808:	{ &ui_scoreSkillBonus, "ui_scoreSkillBonus", "0", CVAR_ARCHIVE},
;5809:	{ &ui_scoreShutoutBonus, "ui_scoreShutoutBonus", "0", CVAR_ARCHIVE},
;5810:	{ &ui_fragLimit, "ui_fragLimit", "10", 0},
;5811:	{ &ui_captureLimit, "ui_captureLimit", "5", 0},
;5812:	{ &ui_smallFont, "ui_smallFont", "0.25", CVAR_ARCHIVE},
;5813:	{ &ui_bigFont, "ui_bigFont", "0.4", CVAR_ARCHIVE},
;5814:	{ &ui_findPlayer, "ui_findPlayer", "Sarge", CVAR_ARCHIVE},
;5815:	{ &ui_Q3Model, "ui_q3model", "0", CVAR_ARCHIVE},
;5816:	{ &ui_hudFiles, "cg_hudFiles", "ui/hud.txt", CVAR_ARCHIVE},
;5817:	{ &ui_recordSPDemo, "ui_recordSPDemo", "0", CVAR_ARCHIVE},
;5818:	{ &ui_teamArenaFirstRun, "ui_teamArenaFirstRun", "0", CVAR_ARCHIVE},
;5819:	{ &ui_realWarmUp, "g_warmup", "20", CVAR_ARCHIVE},
;5820:	{ &ui_realCaptureLimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART},
;5821:	{ &ui_serverStatusTimeOut, "ui_serverStatusTimeOut", "7000", CVAR_ARCHIVE},
;5822:
;5823:};
;5824:
;5825:// bk001129 - made static to avoid aliasing
;5826:static int		cvarTableSize = sizeof(cvarTable) / sizeof(cvarTable[0]);
;5827:
;5828:
;5829:/*
;5830:=================
;5831:UI_RegisterCvars
;5832:=================
;5833:*/
;5834:void UI_RegisterCvars( void ) {
line 5838
;5835:	int			i;
;5836:	cvarTable_t	*cv;
;5837:
;5838:	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
ADDRGP4 cvarTable
ASGNP4
ADDRGP4 $4551
JUMPV
LABELV $4548
line 5839
;5839:		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
ADDRLP4 0
INDIRP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 5840
;5840:	}
LABELV $4549
line 5838
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
LABELV $4551
ADDRLP4 4
INDIRI4
ADDRGP4 cvarTableSize
INDIRI4
LTI4 $4548
line 5841
;5841:}
LABELV $4547
endproc UI_RegisterCvars 12 16
export UI_UpdateCvars
proc UI_UpdateCvars 8 4
line 5848
;5842:
;5843:/*
;5844:=================
;5845:UI_UpdateCvars
;5846:=================
;5847:*/
;5848:void UI_UpdateCvars( void ) {
line 5852
;5849:	int			i;
;5850:	cvarTable_t	*cv;
;5851:
;5852:	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
ADDRGP4 cvarTable
ASGNP4
ADDRGP4 $4556
JUMPV
LABELV $4553
line 5853
;5853:		trap_Cvar_Update( cv->vmCvar );
ADDRLP4 4
INDIRP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 5854
;5854:	}
LABELV $4554
line 5852
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
LABELV $4556
ADDRLP4 0
INDIRI4
ADDRGP4 cvarTableSize
INDIRI4
LTI4 $4553
line 5855
;5855:}
LABELV $4552
endproc UI_UpdateCvars 8 4
proc UI_StopServerRefresh 12 12
line 5864
;5856:
;5857:
;5858:/*
;5859:=================
;5860:ArenaServers_StopRefresh
;5861:=================
;5862:*/
;5863:static void UI_StopServerRefresh( void )
;5864:{
line 5867
;5865:	int count;
;5866:
;5867:	if (!uiInfo.serverStatus.refreshActive) {
ADDRGP4 uiInfo+99264+2212
INDIRI4
CNSTI4 0
NEI4 $4558
line 5869
;5868:		// not currently refreshing
;5869:		return;
ADDRGP4 $4557
JUMPV
LABELV $4558
line 5871
;5870:	}
;5871:	uiInfo.serverStatus.refreshActive = qfalse;
ADDRGP4 uiInfo+99264+2212
CNSTI4 0
ASGNI4
line 5872
;5872:	Com_Printf("%d servers listed in browser with %d players.\n",
ADDRGP4 $4564
ARGP4
ADDRGP4 uiInfo+99264+10412
INDIRI4
ARGI4
ADDRGP4 uiInfo+99264+10416
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 5875
;5873:					uiInfo.serverStatus.numDisplayServers,
;5874:					uiInfo.serverStatus.numPlayersOnServers);
;5875:	count = trap_LAN_GetServerCount(ui_netSource.integer);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 5876
;5876:	if (count - uiInfo.serverStatus.numDisplayServers > 0) {
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
SUBI4
CNSTI4 0
LEI4 $4570
line 5877
;5877:		Com_Printf("%d servers not listed due to packet loss or pings higher than %d\n",
ADDRGP4 $4577
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 $4574
ARGP4
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+99264+10412
INDIRI4
SUBI4
ARGI4
ADDRLP4 8
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 5880
;5878:						count - uiInfo.serverStatus.numDisplayServers,
;5879:						(int) trap_Cvar_VariableValue("cl_maxPing"));
;5880:	}
LABELV $4570
line 5882
;5881:
;5882:}
LABELV $4557
endproc UI_StopServerRefresh 12 12
proc UI_DoServerRefresh 8 4
line 5907
;5883:
;5884:/*
;5885:=================
;5886:ArenaServers_MaxPing
;5887:=================
;5888:*/
;5889:#ifndef MISSIONPACK // bk001206
;5890:static int ArenaServers_MaxPing( void ) {
;5891:	int		maxPing;
;5892:
;5893:	maxPing = (int)trap_Cvar_VariableValue( "cl_maxPing" );
;5894:	if( maxPing < 100 ) {
;5895:		maxPing = 100;
;5896:	}
;5897:	return maxPing;
;5898:}
;5899:#endif
;5900:
;5901:/*
;5902:=================
;5903:UI_DoServerRefresh
;5904:=================
;5905:*/
;5906:static void UI_DoServerRefresh( void )
;5907:{
line 5908
;5908:	qboolean wait = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 5910
;5909:
;5910:	if (!uiInfo.serverStatus.refreshActive) {
ADDRGP4 uiInfo+99264+2212
INDIRI4
CNSTI4 0
NEI4 $4579
line 5911
;5911:		return;
ADDRGP4 $4578
JUMPV
LABELV $4579
line 5913
;5912:	}
;5913:	if (ui_netSource.integer != AS_FAVORITES) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 3
EQI4 $4583
line 5914
;5914:		if (ui_netSource.integer == AS_LOCAL) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
NEI4 $4586
line 5915
;5915:			if (!trap_LAN_GetServerCount(ui_netSource.integer)) {
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $4587
line 5916
;5916:				wait = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 5917
;5917:			}
line 5918
;5918:		} else {
ADDRGP4 $4587
JUMPV
LABELV $4586
line 5919
;5919:			if (trap_LAN_GetServerCount(ui_netSource.integer) < 0) {
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $4592
line 5920
;5920:				wait = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 5921
;5921:			}
LABELV $4592
line 5922
;5922:		}
LABELV $4587
line 5923
;5923:	}
LABELV $4583
line 5925
;5924:
;5925:	if (uiInfo.uiDC.realTime < uiInfo.serverStatus.refreshtime) {
ADDRGP4 uiInfo+208
INDIRI4
ADDRGP4 uiInfo+99264+2192
INDIRI4
GEI4 $4595
line 5926
;5926:		if (wait) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $4600
line 5927
;5927:			return;
ADDRGP4 $4578
JUMPV
LABELV $4600
line 5929
;5928:		}
;5929:	}
LABELV $4595
line 5932
;5930:
;5931:	// if still trying to retrieve pings
;5932:	if (trap_LAN_UpdateVisiblePings(ui_netSource.integer)) {
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 trap_LAN_UpdateVisiblePings
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $4602
line 5933
;5933:		uiInfo.serverStatus.refreshtime = uiInfo.uiDC.realTime + 1000;
ADDRGP4 uiInfo+99264+2192
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 5934
;5934:	} else if (!wait) {
ADDRGP4 $4603
JUMPV
LABELV $4602
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $4608
line 5936
;5935:		// get the last servers in the list
;5936:		UI_BuildServerDisplayList(2);
CNSTI4 2
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 5938
;5937:		// stop the refresh
;5938:		UI_StopServerRefresh();
ADDRGP4 UI_StopServerRefresh
CALLV
pop
line 5939
;5939:	}
LABELV $4608
LABELV $4603
line 5941
;5940:	//
;5941:	UI_BuildServerDisplayList(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 UI_BuildServerDisplayList
CALLV
pop
line 5942
;5942:}
LABELV $4578
endproc UI_DoServerRefresh 8 4
proc UI_StartServerRefresh 68 24
line 5950
;5943:
;5944:/*
;5945:=================
;5946:UI_StartServerRefresh
;5947:=================
;5948:*/
;5949:static void UI_StartServerRefresh(qboolean full)
;5950:{
line 5955
;5951:	int		i;
;5952:	char	*ptr;
;5953:
;5954:	qtime_t q;
;5955:	trap_RealTime(&q);
ADDRLP4 0
ARGP4
ADDRGP4 trap_RealTime
CALLI4
pop
line 5956
;5956: 	trap_Cvar_Set( va("ui_lastServerRefresh_%i", ui_netSource.integer), va("%s-%i, %i at %i:%i", MonthAbbrev[q.tm_mon],q.tm_mday, 1900+q.tm_year,q.tm_hour,q.tm_min));
ADDRGP4 $1314
ARGP4
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $4612
ARGP4
ADDRLP4 0+16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 MonthAbbrev
ADDP4
INDIRP4
ARGP4
ADDRLP4 0+12
INDIRI4
ARGI4
ADDRLP4 0+20
INDIRI4
CNSTI4 1900
ADDI4
ARGI4
ADDRLP4 0+8
INDIRI4
ARGI4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 5958
;5957:
;5958:	if (!full) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $4618
line 5959
;5959:		UI_UpdatePendingPings();
ADDRGP4 UI_UpdatePendingPings
CALLV
pop
line 5960
;5960:		return;
ADDRGP4 $4610
JUMPV
LABELV $4618
line 5963
;5961:	}
;5962:
;5963:	uiInfo.serverStatus.refreshActive = qtrue;
ADDRGP4 uiInfo+99264+2212
CNSTI4 1
ASGNI4
line 5964
;5964:	uiInfo.serverStatus.nextDisplayRefresh = uiInfo.uiDC.realTime + 1000;
ADDRGP4 uiInfo+99264+10420
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 5966
;5965:	// clear number of displayed servers
;5966:	uiInfo.serverStatus.numDisplayServers = 0;
ADDRGP4 uiInfo+99264+10412
CNSTI4 0
ASGNI4
line 5967
;5967:	uiInfo.serverStatus.numPlayersOnServers = 0;
ADDRGP4 uiInfo+99264+10416
CNSTI4 0
ASGNI4
line 5969
;5968:	// mark all servers as visible so we store ping updates for them
;5969:	trap_LAN_MarkServerVisible(ui_netSource.integer, -1, qtrue);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
CNSTI4 -1
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_LAN_MarkServerVisible
CALLV
pop
line 5971
;5970:	// reset all the pings
;5971:	trap_LAN_ResetPings(ui_netSource.integer);
ADDRGP4 ui_netSource+12
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ResetPings
CALLV
pop
line 5973
;5972:	//
;5973:	if( ui_netSource.integer == AS_LOCAL ) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 0
NEI4 $4631
line 5974
;5974:		trap_Cmd_ExecuteText( EXEC_NOW, "localservers\n" );
CNSTI4 0
ARGI4
ADDRGP4 $4634
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 5975
;5975:		uiInfo.serverStatus.refreshtime = uiInfo.uiDC.realTime + 1000;
ADDRGP4 uiInfo+99264+2192
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 5976
;5976:		return;
ADDRGP4 $4610
JUMPV
LABELV $4631
line 5979
;5977:	}
;5978:
;5979:	uiInfo.serverStatus.refreshtime = uiInfo.uiDC.realTime + 5000;
ADDRGP4 uiInfo+99264+2192
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 5980
;5980:	if( ui_netSource.integer == AS_GLOBAL || ui_netSource.integer == AS_MPLAYER ) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 2
EQI4 $4645
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 1
NEI4 $4641
LABELV $4645
line 5981
;5981:		if( ui_netSource.integer == AS_GLOBAL ) {
ADDRGP4 ui_netSource+12
INDIRI4
CNSTI4 2
NEI4 $4646
line 5982
;5982:			i = 0;
ADDRLP4 40
CNSTI4 0
ASGNI4
line 5983
;5983:		}
ADDRGP4 $4647
JUMPV
LABELV $4646
line 5984
;5984:		else {
line 5985
;5985:			i = 1;
ADDRLP4 40
CNSTI4 1
ASGNI4
line 5986
;5986:		}
LABELV $4647
line 5988
;5987:
;5988:		ptr = UI_Cvar_VariableString("debug_protocol");
ADDRGP4 $4264
ARGP4
ADDRLP4 52
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 36
ADDRLP4 52
INDIRP4
ASGNP4
line 5989
;5989:		if (strlen(ptr)) {
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $4649
line 5990
;5990:			trap_Cmd_ExecuteText( EXEC_NOW, va( "globalservers %d %s full empty\n", i, ptr));
ADDRGP4 $4651
ARGP4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 0
ARGI4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 5991
;5991:		}
ADDRGP4 $4650
JUMPV
LABELV $4649
line 5992
;5992:		else {
line 5993
;5993:			trap_Cmd_ExecuteText( EXEC_NOW, va( "globalservers %d %d full empty\n", i, (int)trap_Cvar_VariableValue( "protocol" ) ) );
ADDRGP4 $2163
ARGP4
ADDRLP4 60
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 $4652
ARGP4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 60
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 64
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 0
ARGI4
ADDRLP4 64
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 5994
;5994:		}
LABELV $4650
line 5995
;5995:	}
LABELV $4641
line 5996
;5996:}
LABELV $4610
endproc UI_StartServerRefresh 68 24
bss
export ui_realWarmUp
align 4
LABELV ui_realWarmUp
skip 272
export ui_realCaptureLimit
align 4
LABELV ui_realCaptureLimit
skip 272
export ui_recordSPDemo
align 4
LABELV ui_recordSPDemo
skip 272
export ui_hudFiles
align 4
LABELV ui_hudFiles
skip 272
export ui_Q3Model
align 4
LABELV ui_Q3Model
skip 272
export ui_findPlayer
align 4
LABELV ui_findPlayer
skip 272
export ui_scoreCaptures
align 4
LABELV ui_scoreCaptures
skip 272
export ui_teamName
align 4
LABELV ui_teamName
skip 272
export ui_blueteam5
align 4
LABELV ui_blueteam5
skip 272
export ui_blueteam4
align 4
LABELV ui_blueteam4
skip 272
export ui_blueteam3
align 4
LABELV ui_blueteam3
skip 272
export ui_blueteam2
align 4
LABELV ui_blueteam2
skip 272
export ui_blueteam1
align 4
LABELV ui_blueteam1
skip 272
export ui_blueteam
align 4
LABELV ui_blueteam
skip 272
export ui_redteam5
align 4
LABELV ui_redteam5
skip 272
export ui_redteam4
align 4
LABELV ui_redteam4
skip 272
export ui_redteam3
align 4
LABELV ui_redteam3
skip 272
export ui_redteam2
align 4
LABELV ui_redteam2
skip 272
export ui_redteam1
align 4
LABELV ui_redteam1
skip 272
export ui_redteam
align 4
LABELV ui_redteam
skip 272
align 1
LABELV lastLoadingText
skip 1024
align 4
LABELV lastConnState
skip 4
export startTime
align 4
LABELV startTime
skip 4
export ui_teamArenaFirstRun
align 4
LABELV ui_teamArenaFirstRun
skip 272
export ui_initialized
align 4
LABELV ui_initialized
skip 272
export ui_debug
align 4
LABELV ui_debug
skip 272
export ui_new
align 4
LABELV ui_new
skip 272
import ProcessNewUI
import UI_RankStatusMenu
import RankStatus_Cache
import UI_SignupMenu
import Signup_Cache
import UI_LoginMenu
import Login_Cache
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
import UI_GetBotNameByNumber
import UI_LoadBots
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
import UI_RemoveBotsMenu
import UI_RemoveBots_Cache
import UI_AddBotsMenu
import UI_AddBots_Cache
import trap_SetPbClStatus
import trap_VerifyCDKey
import trap_R_RemapShader
import trap_RealTime
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_S_StartBackgroundTrack
import trap_S_StopBackgroundTrack
import trap_R_RegisterFont
import trap_SetCDKey
import trap_GetCDKey
import trap_MemoryRemaining
import trap_LAN_CompareServers
import trap_LAN_ServerStatus
import trap_LAN_ResetPings
import trap_LAN_RemoveServer
import trap_LAN_AddServer
import trap_LAN_UpdateVisiblePings
import trap_LAN_ServerIsVisible
import trap_LAN_MarkServerVisible
import trap_LAN_SaveCachedServers
import trap_LAN_LoadCachedServers
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_GetPingQueueCount
import trap_LAN_GetServerPing
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
import trap_R_ModelBounds
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
import UI_LoadBestScores
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
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_SetColor
import UI_UpdateScreen
import UI_DrawSides
import UI_DrawTopBottom
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
export uiInfo
align 4
LABELV uiInfo
skip 136912
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
import TeamMain_Cache
import UI_TeamMainMenu
import UI_SetupMenu
import UI_SetupMenu_Cache
import UI_ConfirmMenu
import ConfirmMenu_Cache
import UI_InGameMenu
import InGame_Cache
import UI_CreditMenu
import UI_MainMenu
import MainMenu_Cache
import UI_LoadArenas
import UI_ClearScores
import UI_AdjustTimeByGame
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
export ui_serverStatusTimeOut
align 4
LABELV ui_serverStatusTimeOut
skip 272
export ui_bigFont
align 4
LABELV ui_bigFont
skip 272
export ui_smallFont
align 4
LABELV ui_smallFont
skip 272
export ui_scoreTime
align 4
LABELV ui_scoreTime
skip 272
export ui_scoreShutoutBonus
align 4
LABELV ui_scoreShutoutBonus
skip 272
export ui_scoreSkillBonus
align 4
LABELV ui_scoreSkillBonus
skip 272
export ui_scoreTimeBonus
align 4
LABELV ui_scoreTimeBonus
skip 272
export ui_scoreBase
align 4
LABELV ui_scoreBase
skip 272
export ui_scoreTeam
align 4
LABELV ui_scoreTeam
skip 272
export ui_scorePerfect
align 4
LABELV ui_scorePerfect
skip 272
export ui_scoreScore
align 4
LABELV ui_scoreScore
skip 272
export ui_scoreGauntlets
align 4
LABELV ui_scoreGauntlets
skip 272
export ui_scoreAssists
align 4
LABELV ui_scoreAssists
skip 272
export ui_scoreDefends
align 4
LABELV ui_scoreDefends
skip 272
export ui_scoreExcellents
align 4
LABELV ui_scoreExcellents
skip 272
export ui_scoreImpressives
align 4
LABELV ui_scoreImpressives
skip 272
export ui_scoreAccuracy
align 4
LABELV ui_scoreAccuracy
skip 272
export ui_singlePlayerActive
align 4
LABELV ui_singlePlayerActive
skip 272
export ui_lastServerRefresh_3
align 4
LABELV ui_lastServerRefresh_3
skip 272
export ui_lastServerRefresh_2
align 4
LABELV ui_lastServerRefresh_2
skip 272
export ui_lastServerRefresh_1
align 4
LABELV ui_lastServerRefresh_1
skip 272
export ui_lastServerRefresh_0
align 4
LABELV ui_lastServerRefresh_0
skip 272
export ui_selectedPlayerName
align 4
LABELV ui_selectedPlayerName
skip 272
export ui_selectedPlayer
align 4
LABELV ui_selectedPlayer
skip 272
export ui_currentOpponent
align 4
LABELV ui_currentOpponent
skip 272
export ui_mapIndex
align 4
LABELV ui_mapIndex
skip 272
export ui_currentNetMap
align 4
LABELV ui_currentNetMap
skip 272
export ui_currentMap
align 4
LABELV ui_currentMap
skip 272
export ui_currentTier
align 4
LABELV ui_currentTier
skip 272
export ui_menuFiles
align 4
LABELV ui_menuFiles
skip 272
export ui_opponentName
align 4
LABELV ui_opponentName
skip 272
export ui_dedicated
align 4
LABELV ui_dedicated
skip 272
export ui_serverFilterType
align 4
LABELV ui_serverFilterType
skip 272
export ui_netSource
align 4
LABELV ui_netSource
skip 272
export ui_joinGameType
align 4
LABELV ui_joinGameType
skip 272
export ui_actualNetGameType
align 4
LABELV ui_actualNetGameType
skip 272
export ui_netGameType
align 4
LABELV ui_netGameType
skip 272
export ui_gameType
align 4
LABELV ui_gameType
skip 272
export ui_fragLimit
align 4
LABELV ui_fragLimit
skip 272
export ui_captureLimit
align 4
LABELV ui_captureLimit
skip 272
export ui_cdkeychecked
align 4
LABELV ui_cdkeychecked
skip 272
import ui_cdkey
export ui_server16
align 4
LABELV ui_server16
skip 272
export ui_server15
align 4
LABELV ui_server15
skip 272
export ui_server14
align 4
LABELV ui_server14
skip 272
export ui_server13
align 4
LABELV ui_server13
skip 272
export ui_server12
align 4
LABELV ui_server12
skip 272
export ui_server11
align 4
LABELV ui_server11
skip 272
export ui_server10
align 4
LABELV ui_server10
skip 272
export ui_server9
align 4
LABELV ui_server9
skip 272
export ui_server8
align 4
LABELV ui_server8
skip 272
export ui_server7
align 4
LABELV ui_server7
skip 272
export ui_server6
align 4
LABELV ui_server6
skip 272
export ui_server5
align 4
LABELV ui_server5
skip 272
export ui_server4
align 4
LABELV ui_server4
skip 272
export ui_server3
align 4
LABELV ui_server3
skip 272
export ui_server2
align 4
LABELV ui_server2
skip 272
export ui_server1
align 4
LABELV ui_server1
skip 272
export ui_marks
align 4
LABELV ui_marks
skip 272
export ui_drawCrosshairNames
align 4
LABELV ui_drawCrosshairNames
skip 272
export ui_drawCrosshair
align 4
LABELV ui_drawCrosshair
skip 272
export ui_brassTime
align 4
LABELV ui_brassTime
skip 272
export ui_browserShowEmpty
align 4
LABELV ui_browserShowEmpty
skip 272
export ui_browserShowFull
align 4
LABELV ui_browserShowFull
skip 272
export ui_browserSortKey
align 4
LABELV ui_browserSortKey
skip 272
export ui_browserGameType
align 4
LABELV ui_browserGameType
skip 272
export ui_browserMaster
align 4
LABELV ui_browserMaster
skip 272
export ui_spSelection
align 4
LABELV ui_spSelection
skip 272
export ui_spSkill
align 4
LABELV ui_spSkill
skip 272
export ui_spVideos
align 4
LABELV ui_spVideos
skip 272
export ui_spAwards
align 4
LABELV ui_spAwards
skip 272
export ui_spScores5
align 4
LABELV ui_spScores5
skip 272
export ui_spScores4
align 4
LABELV ui_spScores4
skip 272
export ui_spScores3
align 4
LABELV ui_spScores3
skip 272
export ui_spScores2
align 4
LABELV ui_spScores2
skip 272
export ui_spScores1
align 4
LABELV ui_spScores1
skip 272
export ui_botsFile
align 4
LABELV ui_botsFile
skip 272
export ui_arenasFile
align 4
LABELV ui_arenasFile
skip 272
export ui_ctf_friendly
align 4
LABELV ui_ctf_friendly
skip 272
export ui_ctf_timelimit
align 4
LABELV ui_ctf_timelimit
skip 272
export ui_ctf_capturelimit
align 4
LABELV ui_ctf_capturelimit
skip 272
export ui_team_friendly
align 4
LABELV ui_team_friendly
skip 272
export ui_team_timelimit
align 4
LABELV ui_team_timelimit
skip 272
export ui_team_fraglimit
align 4
LABELV ui_team_fraglimit
skip 272
export ui_tourney_timelimit
align 4
LABELV ui_tourney_timelimit
skip 272
export ui_tourney_fraglimit
align 4
LABELV ui_tourney_fraglimit
skip 272
export ui_ffa_timelimit
align 4
LABELV ui_ffa_timelimit
skip 272
export ui_ffa_fraglimit
align 4
LABELV ui_ffa_fraglimit
skip 272
import trap_PC_SourceFileAndLine
import trap_PC_ReadToken
import trap_PC_FreeSource
import trap_PC_LoadSource
import trap_PC_AddGlobalDefine
import Controls_SetDefaults
import Controls_SetConfig
import Controls_GetConfig
import UI_OutOfMemory
import UI_InitMemory
import UI_Alloc
import Display_CacheAll
import Menu_SetFeederSelection
import Menu_Paint
import Menus_CloseAll
import LerpColor
import Display_HandleKey
import Menus_CloseByName
import Menus_ShowByName
import Menus_FindByName
import Menus_OpenByName
import Display_KeyBindPending
import Display_CursorType
import Display_MouseMove
import Display_CaptureItem
import Display_GetContext
import Menus_Activate
import Menus_AnyFullScreenVisible
import Menu_Reset
import Menus_ActivateByName
import Menu_PaintAll
import Menu_New
import Menu_Count
import PC_Script_Parse
import PC_String_Parse
import PC_Rect_Parse
import PC_Int_Parse
import PC_Color_Parse
import PC_Float_Parse
import Script_Parse
import String_Parse
import Rect_Parse
import Int_Parse
import Color_Parse
import Float_Parse
import Menu_ScrollFeeder
import Menu_HandleMouseMove
import Menu_HandleKey
import Menu_GetFocused
import Menu_PostParse
import Item_Init
import Menu_Init
import Display_ExpandMacros
import Init_Display
import String_Report
import String_Init
import String_Alloc
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
LABELV $4652
byte 1 103
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 32
byte 1 101
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 10
byte 1 0
align 1
LABELV $4651
byte 1 103
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 32
byte 1 101
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 10
byte 1 0
align 1
LABELV $4634
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $4612
byte 1 37
byte 1 115
byte 1 45
byte 1 37
byte 1 105
byte 1 44
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 97
byte 1 116
byte 1 32
byte 1 37
byte 1 105
byte 1 58
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $4577
byte 1 99
byte 1 108
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $4574
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 100
byte 1 117
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 112
byte 1 97
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 32
byte 1 108
byte 1 111
byte 1 115
byte 1 115
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 115
byte 1 32
byte 1 104
byte 1 105
byte 1 103
byte 1 104
byte 1 101
byte 1 114
byte 1 32
byte 1 116
byte 1 104
byte 1 97
byte 1 110
byte 1 32
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $4564
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $4546
byte 1 55
byte 1 48
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $4545
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 83
byte 1 116
byte 1 97
byte 1 116
byte 1 117
byte 1 115
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 79
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $4544
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 70
byte 1 105
byte 1 114
byte 1 115
byte 1 116
byte 1 82
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $4543
byte 1 117
byte 1 105
byte 1 47
byte 1 104
byte 1 117
byte 1 100
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $4542
byte 1 99
byte 1 103
byte 1 95
byte 1 104
byte 1 117
byte 1 100
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4541
byte 1 117
byte 1 105
byte 1 95
byte 1 113
byte 1 51
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $4540
byte 1 83
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $4539
byte 1 48
byte 1 46
byte 1 52
byte 1 0
align 1
LABELV $4538
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 105
byte 1 103
byte 1 70
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $4537
byte 1 48
byte 1 46
byte 1 50
byte 1 53
byte 1 0
align 1
LABELV $4536
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 109
byte 1 97
byte 1 108
byte 1 108
byte 1 70
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $4535
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 104
byte 1 117
byte 1 116
byte 1 111
byte 1 117
byte 1 116
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $4534
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $4533
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $4532
byte 1 48
byte 1 48
byte 1 58
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $4531
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4530
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 66
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $4529
byte 1 48
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 48
byte 1 0
align 1
LABELV $4528
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $4527
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 80
byte 1 101
byte 1 114
byte 1 102
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $4526
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $4525
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 71
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $4524
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 115
byte 1 115
byte 1 105
byte 1 115
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $4523
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 68
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $4522
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4521
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 69
byte 1 120
byte 1 99
byte 1 101
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $4520
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 73
byte 1 109
byte 1 112
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 118
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4519
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 99
byte 1 99
byte 1 117
byte 1 114
byte 1 97
byte 1 99
byte 1 121
byte 1 0
align 1
LABELV $4518
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 51
byte 1 0
align 1
LABELV $4517
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 50
byte 1 0
align 1
LABELV $4516
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $4515
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $4514
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 114
byte 1 101
byte 1 110
byte 1 116
byte 1 79
byte 1 112
byte 1 112
byte 1 111
byte 1 110
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $4513
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 53
byte 1 0
align 1
LABELV $4512
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 52
byte 1 0
align 1
LABELV $4511
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 51
byte 1 0
align 1
LABELV $4510
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 50
byte 1 0
align 1
LABELV $4509
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 49
byte 1 0
align 1
LABELV $4508
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 53
byte 1 0
align 1
LABELV $4507
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 52
byte 1 0
align 1
LABELV $4506
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 51
byte 1 0
align 1
LABELV $4505
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 50
byte 1 0
align 1
LABELV $4504
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 49
byte 1 0
align 1
LABELV $4503
byte 1 117
byte 1 105
byte 1 95
byte 1 97
byte 1 99
byte 1 116
byte 1 117
byte 1 97
byte 1 108
byte 1 78
byte 1 101
byte 1 116
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $4502
byte 1 117
byte 1 105
byte 1 95
byte 1 110
byte 1 101
byte 1 116
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $4501
byte 1 117
byte 1 105
byte 1 95
byte 1 106
byte 1 111
byte 1 105
byte 1 110
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $4500
byte 1 51
byte 1 0
align 1
LABELV $4499
byte 1 117
byte 1 105
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
LABELV $4498
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $4497
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $4496
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $4495
byte 1 83
byte 1 116
byte 1 114
byte 1 111
byte 1 103
byte 1 103
byte 1 115
byte 1 0
align 1
LABELV $4494
byte 1 80
byte 1 97
byte 1 103
byte 1 97
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $4493
byte 1 117
byte 1 105
byte 1 95
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
byte 1 0
align 1
LABELV $4492
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 101
byte 1 98
byte 1 117
byte 1 103
byte 1 0
align 1
LABELV $4491
byte 1 117
byte 1 105
byte 1 95
byte 1 110
byte 1 101
byte 1 119
byte 1 0
align 1
LABELV $4490
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 99
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $4489
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 54
byte 1 0
align 1
LABELV $4488
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 53
byte 1 0
align 1
LABELV $4487
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 52
byte 1 0
align 1
LABELV $4486
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 51
byte 1 0
align 1
LABELV $4485
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 50
byte 1 0
align 1
LABELV $4484
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 49
byte 1 0
align 1
LABELV $4483
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $4482
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 57
byte 1 0
align 1
LABELV $4481
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 56
byte 1 0
align 1
LABELV $4480
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 55
byte 1 0
align 1
LABELV $4479
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 54
byte 1 0
align 1
LABELV $4478
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 53
byte 1 0
align 1
LABELV $4477
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 52
byte 1 0
align 1
LABELV $4476
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 51
byte 1 0
align 1
LABELV $4475
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $4474
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $4473
byte 1 99
byte 1 103
byte 1 95
byte 1 109
byte 1 97
byte 1 114
byte 1 107
byte 1 115
byte 1 0
align 1
LABELV $4472
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4471
byte 1 50
byte 1 53
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $4470
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 69
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $4469
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $4468
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 111
byte 1 114
byte 1 116
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $4467
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $4466
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 77
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $4465
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $4464
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 86
byte 1 105
byte 1 100
byte 1 101
byte 1 111
byte 1 115
byte 1 0
align 1
LABELV $4463
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 65
byte 1 119
byte 1 97
byte 1 114
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $4462
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 53
byte 1 0
align 1
LABELV $4461
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 52
byte 1 0
align 1
LABELV $4460
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 51
byte 1 0
align 1
LABELV $4459
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $4458
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 49
byte 1 0
align 1
LABELV $4457
byte 1 103
byte 1 95
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $4456
byte 1 103
byte 1 95
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $4455
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 0
align 1
LABELV $4454
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4453
byte 1 56
byte 1 0
align 1
LABELV $4452
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4451
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 0
align 1
LABELV $4450
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4449
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4448
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4447
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4446
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 102
byte 1 97
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4445
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 102
byte 1 97
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $4436
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 116
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $4433
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4430
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $4427
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $4417
byte 1 109
byte 1 111
byte 1 116
byte 1 100
byte 1 0
align 1
LABELV $4414
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $4413
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 117
byte 1 112
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $4412
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $4408
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $4401
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $4399
byte 1 37
byte 1 115
byte 1 47
byte 1 83
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $4396
byte 1 40
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 111
byte 1 112
byte 1 105
byte 1 101
byte 1 100
byte 1 41
byte 1 0
align 1
LABELV $4387
byte 1 40
byte 1 37
byte 1 115
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 111
byte 1 112
byte 1 105
byte 1 101
byte 1 100
byte 1 41
byte 1 0
align 1
LABELV $4386
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $4382
byte 1 37
byte 1 115
byte 1 32
byte 1 40
byte 1 37
byte 1 100
byte 1 37
byte 1 37
byte 1 41
byte 1 0
align 1
LABELV $4379
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4378
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 67
byte 1 111
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $4377
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 83
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $4350
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $4349
byte 1 37
byte 1 100
byte 1 32
byte 1 109
byte 1 105
byte 1 110
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $4346
byte 1 37
byte 1 100
byte 1 32
byte 1 104
byte 1 114
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 109
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $4342
byte 1 37
byte 1 100
byte 1 32
byte 1 98
byte 1 121
byte 1 116
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4341
byte 1 37
byte 1 100
byte 1 32
byte 1 75
byte 1 66
byte 1 0
align 1
LABELV $4338
byte 1 46
byte 1 37
byte 1 48
byte 1 50
byte 1 100
byte 1 32
byte 1 77
byte 1 66
byte 1 0
align 1
LABELV $4335
byte 1 46
byte 1 37
byte 1 48
byte 1 50
byte 1 100
byte 1 32
byte 1 71
byte 1 66
byte 1 0
align 1
LABELV $4329
byte 1 105
byte 1 110
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4327
byte 1 101
byte 1 110
byte 1 100
byte 1 111
byte 1 102
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4319
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 95
byte 1 112
byte 1 111
byte 1 112
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 0
align 1
LABELV $4264
byte 1 100
byte 1 101
byte 1 98
byte 1 117
byte 1 103
byte 1 95
byte 1 112
byte 1 114
byte 1 111
byte 1 116
byte 1 111
byte 1 99
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $4263
byte 1 48
byte 1 46
byte 1 53
byte 1 0
align 1
LABELV $4262
byte 1 115
byte 1 95
byte 1 109
byte 1 117
byte 1 115
byte 1 105
byte 1 99
byte 1 118
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4261
byte 1 48
byte 1 46
byte 1 56
byte 1 0
align 1
LABELV $4260
byte 1 115
byte 1 95
byte 1 118
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $4259
byte 1 117
byte 1 105
byte 1 95
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 70
byte 1 105
byte 1 114
byte 1 115
byte 1 116
byte 1 82
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $4241
byte 1 117
byte 1 105
byte 1 47
byte 1 105
byte 1 110
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $4237
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $4233
byte 1 119
byte 1 104
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $4231
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 51
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 115
byte 1 111
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $4158
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $4139
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $4138
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $4135
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $4134
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $4133
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 0
align 1
LABELV $4125
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $4124
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $4122
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $4121
byte 1 46
byte 1 0
align 1
LABELV $4111
byte 1 47
byte 1 0
align 1
LABELV $4110
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $4062
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 0
align 1
LABELV $4057
byte 1 106
byte 1 111
byte 1 105
byte 1 110
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4052
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $4037
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 111
byte 1 110
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 100
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $4029
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 95
byte 1 115
byte 1 109
byte 1 97
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $3973
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 111
byte 1 110
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $3968
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
byte 1 110
byte 1 101
byte 1 116
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 111
byte 1 110
byte 1 101
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $3926
byte 1 97
byte 1 108
byte 1 105
byte 1 97
byte 1 115
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $3923
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
LABELV $3918
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 115
byte 1 0
align 1
LABELV $3901
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
byte 1 97
byte 1 108
byte 1 105
byte 1 97
byte 1 115
byte 1 101
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 97
byte 1 108
byte 1 105
byte 1 97
byte 1 115
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 100
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $3891
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 101
byte 1 100
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 97
byte 1 108
byte 1 105
byte 1 97
byte 1 115
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 117
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 97
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $3863
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
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
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 100
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $3853
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 101
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $3846
byte 1 109
byte 1 97
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $3843
byte 1 74
byte 1 97
byte 1 110
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $3839
byte 1 102
byte 1 101
byte 1 109
byte 1 97
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $3834
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $3807
byte 1 84
byte 1 111
byte 1 111
byte 1 32
byte 1 109
byte 1 97
byte 1 110
byte 1 121
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 108
byte 1 97
byte 1 99
byte 1 101
byte 1 100
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $3800
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $3642
byte 1 42
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $3539
byte 1 78
byte 1 111
byte 1 0
align 1
LABELV $3538
byte 1 89
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $3535
byte 1 112
byte 1 117
byte 1 110
byte 1 107
byte 1 98
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $3533
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $3529
byte 1 85
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $3525
byte 1 37
byte 1 115
byte 1 32
byte 1 40
byte 1 37
byte 1 115
byte 1 41
byte 1 0
align 1
LABELV $3522
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $3521
byte 1 110
byte 1 101
byte 1 116
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $3520
byte 1 37
byte 1 115
byte 1 32
byte 1 91
byte 1 37
byte 1 115
byte 1 93
byte 1 0
align 1
LABELV $3343
byte 1 115
byte 1 0
align 1
LABELV $3340
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 37
byte 1 115
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
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $3335
byte 1 110
byte 1 111
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $3251
byte 1 115
byte 1 101
byte 1 97
byte 1 114
byte 1 99
byte 1 104
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 100
byte 1 47
byte 1 37
byte 1 100
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $3193
byte 1 115
byte 1 101
byte 1 97
byte 1 114
byte 1 99
byte 1 104
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 100
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $3185
byte 1 99
byte 1 108
byte 1 95
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 83
byte 1 116
byte 1 97
byte 1 116
byte 1 117
byte 1 115
byte 1 82
byte 1 101
byte 1 115
byte 1 101
byte 1 110
byte 1 100
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3174
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 105
byte 1 110
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $3129
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $3128
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $3127
byte 1 110
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $3089
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $3088
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $3087
byte 1 77
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $3086
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3085
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $3084
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3083
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3082
byte 1 65
byte 1 100
byte 1 100
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 0
align 1
LABELV $3081
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3080
byte 1 115
byte 1 118
byte 1 95
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3064
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $3052
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
LABELV $3032
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2989
byte 1 87
byte 1 101
byte 1 108
byte 1 99
byte 1 111
byte 1 109
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 33
byte 1 0
align 1
LABELV $2978
byte 1 99
byte 1 108
byte 1 95
byte 1 109
byte 1 111
byte 1 116
byte 1 100
byte 1 83
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $2821
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
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
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 46
byte 1 115
byte 1 107
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2818
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 46
byte 1 115
byte 1 107
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2782
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 85
byte 1 73
byte 1 32
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2779
byte 1 115
byte 1 101
byte 1 116
byte 1 80
byte 1 98
byte 1 67
byte 1 108
byte 1 83
byte 1 116
byte 1 97
byte 1 116
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $2774
byte 1 117
byte 1 112
byte 1 100
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2771
byte 1 52
byte 1 0
align 1
LABELV $2770
byte 1 103
byte 1 108
byte 1 67
byte 1 117
byte 1 115
byte 1 116
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $2761
byte 1 118
byte 1 111
byte 1 105
byte 1 99
byte 1 101
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $2753
byte 1 118
byte 1 111
byte 1 105
byte 1 99
byte 1 101
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $2741
byte 1 10
byte 1 0
align 1
LABELV $2734
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $2725
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 65
byte 1 100
byte 1 100
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 0
align 1
LABELV $2724
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2720
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2707
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2704
byte 1 65
byte 1 100
byte 1 100
byte 1 101
byte 1 100
byte 1 32
byte 1 102
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2703
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 32
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 10
byte 1 0
align 1
LABELV $2700
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 32
byte 1 97
byte 1 108
byte 1 114
byte 1 101
byte 1 97
byte 1 100
byte 1 121
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $2695
byte 1 97
byte 1 100
byte 1 100
byte 1 114
byte 1 0
align 1
LABELV $2694
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2685
byte 1 97
byte 1 100
byte 1 100
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2669
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
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2666
byte 1 97
byte 1 100
byte 1 100
byte 1 66
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $2661
byte 1 99
byte 1 97
byte 1 108
byte 1 108
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 32
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2655
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 76
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2649
byte 1 99
byte 1 97
byte 1 108
byte 1 108
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 32
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
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $2643
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2638
byte 1 99
byte 1 97
byte 1 108
byte 1 108
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 32
byte 1 107
byte 1 105
byte 1 99
byte 1 107
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2632
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 75
byte 1 105
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $2626
byte 1 99
byte 1 97
byte 1 108
byte 1 108
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2620
byte 1 118
byte 1 111
byte 1 116
byte 1 101
byte 1 77
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $2617
byte 1 99
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 105
byte 1 110
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2614
byte 1 83
byte 1 107
byte 1 105
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 104
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $2611
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 83
byte 1 107
byte 1 105
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $2595
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 83
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $2592
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $2591
byte 1 76
byte 1 101
byte 1 97
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $2588
byte 1 115
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 95
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 50
byte 1 0
align 1
LABELV $2587
byte 1 99
byte 1 108
byte 1 95
byte 1 112
byte 1 97
byte 1 117
byte 1 115
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $2586
byte 1 67
byte 1 111
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 115
byte 1 0
align 1
LABELV $2583
byte 1 113
byte 1 117
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $2582
byte 1 81
byte 1 117
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $2572
byte 1 70
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 74
byte 1 111
byte 1 105
byte 1 110
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2569
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2555
byte 1 74
byte 1 111
byte 1 105
byte 1 110
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2550
byte 1 70
byte 1 105
byte 1 110
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2543
byte 1 70
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 83
byte 1 116
byte 1 97
byte 1 116
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $2533
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 83
byte 1 116
byte 1 97
byte 1 116
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $2527
byte 1 85
byte 1 112
byte 1 100
byte 1 97
byte 1 116
byte 1 101
byte 1 70
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2520
byte 1 83
byte 1 116
byte 1 111
byte 1 112
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $2517
byte 1 109
byte 1 97
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2516
byte 1 106
byte 1 111
byte 1 105
byte 1 110
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2507
byte 1 99
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 74
byte 1 111
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2504
byte 1 81
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 51
byte 1 0
align 1
LABELV $2499
byte 1 100
byte 1 101
byte 1 109
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2498
byte 1 82
byte 1 117
byte 1 110
byte 1 68
byte 1 101
byte 1 109
byte 1 111
byte 1 0
align 1
LABELV $2495
byte 1 118
byte 1 105
byte 1 100
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 59
byte 1 0
align 1
LABELV $2492
byte 1 102
byte 1 115
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2491
byte 1 82
byte 1 117
byte 1 110
byte 1 77
byte 1 111
byte 1 100
byte 1 0
align 1
LABELV $2486
byte 1 99
byte 1 105
byte 1 110
byte 1 101
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 114
byte 1 111
byte 1 113
byte 1 32
byte 1 50
byte 1 10
byte 1 0
align 1
LABELV $2481
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 77
byte 1 111
byte 1 118
byte 1 105
byte 1 101
byte 1 0
align 1
LABELV $2478
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 77
byte 1 111
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $2475
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 77
byte 1 111
byte 1 118
byte 1 105
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $2472
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 68
byte 1 101
byte 1 109
byte 1 111
byte 1 115
byte 1 0
align 1
LABELV $2463
byte 1 100
byte 1 101
byte 1 109
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 95
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $2459
byte 1 82
byte 1 117
byte 1 110
byte 1 83
byte 1 80
byte 1 68
byte 1 101
byte 1 109
byte 1 111
byte 1 0
align 1
LABELV $2456
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 70
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2453
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $2450
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $2441
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 0
align 1
LABELV $2438
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 77
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $2437
byte 1 99
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 69
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $2434
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 67
byte 1 111
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 115
byte 1 0
align 1
LABELV $2431
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 67
byte 1 111
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 115
byte 1 0
align 1
LABELV $2428
byte 1 99
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2427
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 0
align 1
LABELV $2424
byte 1 67
byte 1 68
byte 1 32
byte 1 75
byte 1 101
byte 1 121
byte 1 32
byte 1 100
byte 1 111
byte 1 101
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 97
byte 1 112
byte 1 112
byte 1 101
byte 1 97
byte 1 114
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 98
byte 1 101
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 46
byte 1 0
align 1
LABELV $2423
byte 1 67
byte 1 68
byte 1 32
byte 1 75
byte 1 101
byte 1 121
byte 1 32
byte 1 65
byte 1 112
byte 1 112
byte 1 101
byte 1 97
byte 1 114
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 98
byte 1 101
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 46
byte 1 0
align 1
LABELV $2422
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 0
align 1
LABELV $2421
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 99
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 115
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $2418
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $2417
byte 1 118
byte 1 101
byte 1 114
byte 1 105
byte 1 102
byte 1 121
byte 1 67
byte 1 68
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $2409
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 52
byte 1 0
align 1
LABELV $2408
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 51
byte 1 0
align 1
LABELV $2407
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 50
byte 1 0
align 1
LABELV $2406
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 49
byte 1 0
align 1
LABELV $2405
byte 1 103
byte 1 101
byte 1 116
byte 1 67
byte 1 68
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $2402
byte 1 118
byte 1 105
byte 1 100
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $2401
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 105
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $2400
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 95
byte 1 114
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $2399
byte 1 101
byte 1 120
byte 1 101
byte 1 99
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 46
byte 1 99
byte 1 102
byte 1 103
byte 1 10
byte 1 0
align 1
LABELV $2398
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 68
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2391
byte 1 117
byte 1 112
byte 1 100
byte 1 97
byte 1 116
byte 1 101
byte 1 83
byte 1 80
byte 1 77
byte 1 101
byte 1 110
byte 1 117
byte 1 0
align 1
LABELV $2381
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
byte 1 37
byte 1 102
byte 1 32
byte 1 10
byte 1 0
align 1
LABELV $2378
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
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2349
byte 1 100
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $2348
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2342
byte 1 109
byte 1 95
byte 1 112
byte 1 105
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $2339
byte 1 117
byte 1 105
byte 1 95
byte 1 109
byte 1 111
byte 1 117
byte 1 115
byte 1 101
byte 1 80
byte 1 105
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $2334
byte 1 71
byte 1 76
byte 1 95
byte 1 76
byte 1 73
byte 1 78
byte 1 69
byte 1 65
byte 1 82
byte 1 95
byte 1 77
byte 1 73
byte 1 80
byte 1 77
byte 1 65
byte 1 80
byte 1 95
byte 1 78
byte 1 69
byte 1 65
byte 1 82
byte 1 69
byte 1 83
byte 1 84
byte 1 0
align 1
LABELV $2331
byte 1 71
byte 1 76
byte 1 95
byte 1 76
byte 1 73
byte 1 78
byte 1 69
byte 1 65
byte 1 82
byte 1 95
byte 1 77
byte 1 73
byte 1 80
byte 1 77
byte 1 65
byte 1 80
byte 1 95
byte 1 76
byte 1 73
byte 1 78
byte 1 69
byte 1 65
byte 1 82
byte 1 0
align 1
LABELV $2330
byte 1 114
byte 1 95
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $2329
byte 1 99
byte 1 103
byte 1 95
byte 1 98
byte 1 114
byte 1 97
byte 1 115
byte 1 115
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2328
byte 1 99
byte 1 103
byte 1 95
byte 1 115
byte 1 104
byte 1 97
byte 1 100
byte 1 111
byte 1 119
byte 1 115
byte 1 0
align 1
LABELV $2327
byte 1 114
byte 1 95
byte 1 105
byte 1 110
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 86
byte 1 105
byte 1 100
byte 1 101
byte 1 111
byte 1 0
align 1
LABELV $2326
byte 1 114
byte 1 95
byte 1 102
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 107
byte 1 121
byte 1 0
align 1
LABELV $2325
byte 1 114
byte 1 95
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2324
byte 1 114
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $2323
byte 1 114
byte 1 95
byte 1 112
byte 1 105
byte 1 99
byte 1 109
byte 1 105
byte 1 112
byte 1 0
align 1
LABELV $2322
byte 1 114
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 101
byte 1 120
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $2321
byte 1 114
byte 1 95
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 83
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $2317
byte 1 117
byte 1 105
byte 1 95
byte 1 103
byte 1 108
byte 1 67
byte 1 117
byte 1 115
byte 1 116
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $2312
byte 1 114
byte 1 95
byte 1 115
byte 1 117
byte 1 98
byte 1 100
byte 1 105
byte 1 118
byte 1 105
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $2308
byte 1 114
byte 1 95
byte 1 108
byte 1 111
byte 1 100
byte 1 98
byte 1 105
byte 1 97
byte 1 115
byte 1 0
align 1
LABELV $2301
byte 1 114
byte 1 95
byte 1 115
byte 1 116
byte 1 101
byte 1 110
byte 1 99
byte 1 105
byte 1 108
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2300
byte 1 114
byte 1 95
byte 1 100
byte 1 101
byte 1 112
byte 1 116
byte 1 104
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2296
byte 1 114
byte 1 95
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2293
byte 1 117
byte 1 105
byte 1 95
byte 1 71
byte 1 101
byte 1 116
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2288
byte 1 99
byte 1 108
byte 1 95
byte 1 112
byte 1 97
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 100
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2287
byte 1 99
byte 1 108
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 112
byte 1 97
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2284
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2283
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 101
byte 1 116
byte 1 82
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $2280
byte 1 117
byte 1 105
byte 1 95
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2279
byte 1 117
byte 1 105
byte 1 95
byte 1 83
byte 1 101
byte 1 116
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2275
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 53
byte 1 59
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 82
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $2251
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
byte 1 37
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2237
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 59
byte 1 32
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
byte 1 37
byte 1 102
byte 1 32
byte 1 44
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 10
byte 1 0
align 1
LABELV $2236
byte 1 50
byte 1 0
align 1
LABELV $2233
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 99
byte 1 111
byte 1 114
byte 1 100
byte 1 83
byte 1 80
byte 1 68
byte 1 101
byte 1 109
byte 1 111
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2229
byte 1 37
byte 1 115
byte 1 95
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $2228
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 99
byte 1 111
byte 1 114
byte 1 100
byte 1 83
byte 1 80
byte 1 68
byte 1 101
byte 1 109
byte 1 111
byte 1 0
align 1
LABELV $2225
byte 1 103
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $2224
byte 1 103
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $2223
byte 1 117
byte 1 105
byte 1 95
byte 1 112
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $2222
byte 1 115
byte 1 118
byte 1 95
byte 1 112
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $2221
byte 1 117
byte 1 105
byte 1 95
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2220
byte 1 103
byte 1 95
byte 1 119
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2219
byte 1 117
byte 1 105
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2218
byte 1 115
byte 1 118
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $2217
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 70
byte 1 105
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $2216
byte 1 103
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 70
byte 1 105
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $2215
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 111
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2214
byte 1 103
byte 1 95
byte 1 100
byte 1 111
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2213
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2212
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2211
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 70
byte 1 114
byte 1 97
byte 1 103
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $2210
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $2209
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $2206
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 77
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $2202
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 59
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 59
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $2196
byte 1 117
byte 1 105
byte 1 95
byte 1 109
byte 1 97
byte 1 112
byte 1 73
byte 1 110
byte 1 100
byte 1 101
byte 1 120
byte 1 0
align 1
LABELV $2192
byte 1 115
byte 1 107
byte 1 105
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $2166
byte 1 46
byte 1 100
byte 1 109
byte 1 95
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $2165
byte 1 100
byte 1 101
byte 1 109
byte 1 111
byte 1 115
byte 1 0
align 1
LABELV $2163
byte 1 112
byte 1 114
byte 1 111
byte 1 116
byte 1 111
byte 1 99
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $2162
byte 1 100
byte 1 109
byte 1 95
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $2159
byte 1 46
byte 1 114
byte 1 111
byte 1 113
byte 1 0
align 1
LABELV $2144
byte 1 114
byte 1 111
byte 1 113
byte 1 0
align 1
LABELV $2143
byte 1 118
byte 1 105
byte 1 100
byte 1 101
byte 1 111
byte 1 0
align 1
LABELV $2134
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $2119
byte 1 36
byte 1 109
byte 1 111
byte 1 100
byte 1 108
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $2071
byte 1 69
byte 1 118
byte 1 101
byte 1 114
byte 1 121
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $2049
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 0
align 1
LABELV $1956
byte 1 117
byte 1 105
byte 1 95
byte 1 110
byte 1 101
byte 1 116
byte 1 83
byte 1 111
byte 1 117
byte 1 114
byte 1 99
byte 1 101
byte 1 0
align 1
LABELV $1845
byte 1 117
byte 1 105
byte 1 95
byte 1 97
byte 1 99
byte 1 116
byte 1 117
byte 1 97
byte 1 108
byte 1 110
byte 1 101
byte 1 116
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $1814
byte 1 117
byte 1 105
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $1756
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $1737
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $1540
byte 1 80
byte 1 73
byte 1 88
byte 1 69
byte 1 76
byte 1 70
byte 1 79
byte 1 82
byte 1 77
byte 1 65
byte 1 84
byte 1 58
byte 1 32
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 40
byte 1 37
byte 1 100
byte 1 45
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 41
byte 1 32
byte 1 90
byte 1 40
byte 1 37
byte 1 100
byte 1 45
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 41
byte 1 32
byte 1 115
byte 1 116
byte 1 101
byte 1 110
byte 1 99
byte 1 105
byte 1 108
byte 1 40
byte 1 37
byte 1 100
byte 1 45
byte 1 98
byte 1 105
byte 1 116
byte 1 115
byte 1 41
byte 1 0
align 1
LABELV $1536
byte 1 86
byte 1 69
byte 1 82
byte 1 83
byte 1 73
byte 1 79
byte 1 78
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1533
byte 1 86
byte 1 69
byte 1 78
byte 1 68
byte 1 79
byte 1 82
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1425
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 32
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1422
byte 1 71
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 32
byte 1 102
byte 1 111
byte 1 114
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 40
byte 1 69
byte 1 83
byte 1 67
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 97
byte 1 110
byte 1 99
byte 1 101
byte 1 108
byte 1 41
byte 1 0
align 1
LABELV $1409
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $1399
byte 1 99
byte 1 103
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $1391
byte 1 99
byte 1 103
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1371
byte 1 110
byte 1 0
align 1
LABELV $1360
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
LABELV $1359
byte 1 116
byte 1 0
align 1
LABELV $1358
byte 1 116
byte 1 108
byte 1 0
align 1
LABELV $1323
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
LABELV $1314
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 97
byte 1 115
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 82
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $1312
byte 1 80
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 69
byte 1 78
byte 1 84
byte 1 69
byte 1 82
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 67
byte 1 76
byte 1 73
byte 1 67
byte 1 75
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 44
byte 1 32
byte 1 80
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 66
byte 1 65
byte 1 67
byte 1 75
byte 1 83
byte 1 80
byte 1 65
byte 1 67
byte 1 69
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $1311
byte 1 87
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 102
byte 1 111
byte 1 114
byte 1 32
byte 1 110
byte 1 101
byte 1 119
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 46
byte 1 46
byte 1 46
byte 1 32
byte 1 80
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 69
byte 1 83
byte 1 67
byte 1 65
byte 1 80
byte 1 69
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 97
byte 1 110
byte 1 99
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $1276
byte 1 37
byte 1 105
byte 1 46
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1099
byte 1 117
byte 1 105
byte 1 95
byte 1 111
byte 1 112
byte 1 112
byte 1 111
byte 1 110
byte 1 101
byte 1 110
byte 1 116
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $1092
byte 1 117
byte 1 105
byte 1 95
byte 1 111
byte 1 112
byte 1 112
byte 1 111
byte 1 110
byte 1 101
byte 1 110
byte 1 116
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $1087
byte 1 74
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $1039
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1026
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $1021
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 114
byte 1 101
byte 1 110
byte 1 116
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1018
byte 1 70
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $984
byte 1 83
byte 1 111
byte 1 117
byte 1 114
byte 1 99
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $967
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $966
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $963
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $962
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $961
byte 1 117
byte 1 105
byte 1 95
byte 1 81
byte 1 51
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $918
byte 1 37
byte 1 48
byte 1 50
byte 1 105
byte 1 58
byte 1 37
byte 1 48
byte 1 50
byte 1 105
byte 1 0
align 1
LABELV $903
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $888
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 114
byte 1 101
byte 1 110
byte 1 116
byte 1 77
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $886
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 114
byte 1 101
byte 1 110
byte 1 116
byte 1 78
byte 1 101
byte 1 116
byte 1 77
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $856
byte 1 72
byte 1 117
byte 1 109
byte 1 97
byte 1 110
byte 1 0
align 1
LABELV $853
byte 1 67
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $847
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $846
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $840
byte 1 82
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $839
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $838
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $831
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $830
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $824
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
LABELV $790
byte 1 37
byte 1 115
byte 1 46
byte 1 114
byte 1 111
byte 1 113
byte 1 0
align 1
LABELV $771
byte 1 37
byte 1 115
byte 1 95
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $766
byte 1 37
byte 1 115
byte 1 95
byte 1 109
byte 1 101
byte 1 116
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $738
byte 1 117
byte 1 105
byte 1 95
byte 1 106
byte 1 111
byte 1 105
byte 1 110
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $728
byte 1 117
byte 1 105
byte 1 95
byte 1 97
byte 1 99
byte 1 116
byte 1 117
byte 1 97
byte 1 108
byte 1 78
byte 1 101
byte 1 116
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $727
byte 1 117
byte 1 105
byte 1 95
byte 1 110
byte 1 101
byte 1 116
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $716
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $715
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $714
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $713
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $712
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $698
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $696
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $694
byte 1 53
byte 1 0
align 1
LABELV $693
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $692
byte 1 49
byte 1 53
byte 1 0
align 1
LABELV $691
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $690
byte 1 50
byte 1 53
byte 1 0
align 1
LABELV $689
byte 1 51
byte 1 48
byte 1 0
align 1
LABELV $688
byte 1 51
byte 1 53
byte 1 0
align 1
LABELV $687
byte 1 52
byte 1 48
byte 1 0
align 1
LABELV $686
byte 1 52
byte 1 53
byte 1 0
align 1
LABELV $685
byte 1 53
byte 1 48
byte 1 0
align 1
LABELV $684
byte 1 53
byte 1 53
byte 1 0
align 1
LABELV $683
byte 1 54
byte 1 48
byte 1 0
align 1
LABELV $682
byte 1 54
byte 1 53
byte 1 0
align 1
LABELV $681
byte 1 55
byte 1 48
byte 1 0
align 1
LABELV $680
byte 1 55
byte 1 53
byte 1 0
align 1
LABELV $679
byte 1 56
byte 1 48
byte 1 0
align 1
LABELV $678
byte 1 56
byte 1 53
byte 1 0
align 1
LABELV $677
byte 1 57
byte 1 48
byte 1 0
align 1
LABELV $676
byte 1 57
byte 1 53
byte 1 0
align 1
LABELV $675
byte 1 78
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $674
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $668
byte 1 117
byte 1 105
byte 1 95
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $666
byte 1 85
byte 1 73
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 61
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 109
byte 1 105
byte 1 108
byte 1 108
byte 1 105
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 111
byte 1 110
byte 1 100
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $663
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 0
align 1
LABELV $643
byte 1 94
byte 1 49
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 117
byte 1 105
byte 1 47
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 115
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 44
byte 1 32
byte 1 117
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 116
byte 1 105
byte 1 110
byte 1 117
byte 1 101
byte 1 33
byte 1 10
byte 1 0
align 1
LABELV $640
byte 1 117
byte 1 105
byte 1 47
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 115
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $639
byte 1 94
byte 1 51
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 44
byte 1 32
byte 1 117
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $617
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 100
byte 1 101
byte 1 102
byte 1 0
align 1
LABELV $611
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 71
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 68
byte 1 101
byte 1 102
byte 1 0
align 1
LABELV $597
byte 1 80
byte 1 97
byte 1 114
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 58
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $591
byte 1 71
byte 1 108
byte 1 121
byte 1 112
byte 1 104
byte 1 32
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 108
byte 1 101
byte 1 32
byte 1 37
byte 1 105
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $586
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 61
byte 1 10
byte 1 0
align 1
LABELV $585
byte 1 70
byte 1 111
byte 1 110
byte 1 116
byte 1 32
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 10
byte 1 0
align 1
LABELV $574
byte 1 115
byte 1 104
byte 1 97
byte 1 100
byte 1 111
byte 1 119
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $566
byte 1 115
byte 1 104
byte 1 97
byte 1 100
byte 1 111
byte 1 119
byte 1 89
byte 1 0
align 1
LABELV $558
byte 1 115
byte 1 104
byte 1 97
byte 1 100
byte 1 111
byte 1 119
byte 1 88
byte 1 0
align 1
LABELV $550
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 65
byte 1 109
byte 1 111
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $542
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 67
byte 1 121
byte 1 99
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $534
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 67
byte 1 108
byte 1 97
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $522
byte 1 99
byte 1 117
byte 1 114
byte 1 115
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $514
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 66
byte 1 117
byte 1 122
byte 1 122
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $506
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 70
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $498
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 69
byte 1 120
byte 1 105
byte 1 116
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $490
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 69
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $482
byte 1 103
byte 1 114
byte 1 97
byte 1 100
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 98
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $473
byte 1 98
byte 1 105
byte 1 103
byte 1 70
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $464
byte 1 115
byte 1 109
byte 1 97
byte 1 108
byte 1 108
byte 1 70
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $453
byte 1 102
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $449
byte 1 125
byte 1 0
align 1
LABELV $440
byte 1 123
byte 1 0
align 1
LABELV $433
byte 1 94
byte 1 49
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 111
byte 1 32
byte 1 108
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 44
byte 1 32
byte 1 109
byte 1 97
byte 1 120
byte 1 32
byte 1 97
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $430
byte 1 94
byte 1 49
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 44
byte 1 32
byte 1 117
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $395
byte 1 49
byte 1 0
align 1
LABELV $394
byte 1 115
byte 1 118
byte 1 95
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $393
byte 1 99
byte 1 103
byte 1 95
byte 1 116
byte 1 104
byte 1 105
byte 1 114
byte 1 100
byte 1 80
byte 1 101
byte 1 114
byte 1 115
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $392
byte 1 48
byte 1 0
align 1
LABELV $391
byte 1 99
byte 1 103
byte 1 95
byte 1 99
byte 1 97
byte 1 109
byte 1 101
byte 1 114
byte 1 97
byte 1 79
byte 1 114
byte 1 98
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $228
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 102
byte 1 101
byte 1 101
byte 1 100
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 47
byte 1 118
byte 1 111
byte 1 99
byte 1 95
byte 1 110
byte 1 101
byte 1 119
byte 1 104
byte 1 105
byte 1 103
byte 1 104
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 0
align 1
LABELV $226
byte 1 103
byte 1 102
byte 1 120
byte 1 47
byte 1 50
byte 1 100
byte 1 47
byte 1 99
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 37
byte 1 99
byte 1 0
align 1
LABELV $219
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 108
byte 1 105
byte 1 100
byte 1 101
byte 1 114
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 95
byte 1 49
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $216
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 108
byte 1 105
byte 1 100
byte 1 101
byte 1 114
byte 1 50
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $213
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 95
byte 1 116
byte 1 104
byte 1 117
byte 1 109
byte 1 98
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $210
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $207
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $204
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 117
byte 1 112
byte 1 95
byte 1 97
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $201
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 100
byte 1 119
byte 1 110
byte 1 95
byte 1 97
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $198
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 98
byte 1 97
byte 1 114
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $195
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 119
byte 1 104
byte 1 105
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $191
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 99
byte 1 121
byte 1 97
byte 1 110
byte 1 0
align 1
LABELV $187
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $183
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $179
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 103
byte 1 114
byte 1 110
byte 1 0
align 1
LABELV $175
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 121
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $171
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $168
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 102
byte 1 120
byte 1 95
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $165
byte 1 117
byte 1 105
byte 1 47
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 47
byte 1 103
byte 1 114
byte 1 97
byte 1 100
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 98
byte 1 97
byte 1 114
byte 1 50
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $146
byte 1 73
byte 1 80
byte 1 88
byte 1 0
align 1
LABELV $145
byte 1 85
byte 1 68
byte 1 80
byte 1 0
align 1
LABELV $144
byte 1 63
byte 1 63
byte 1 63
byte 1 0
align 1
LABELV $143
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $142
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $141
byte 1 79
byte 1 112
byte 1 101
byte 1 110
byte 1 32
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 83
byte 1 112
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $140
byte 1 77
byte 1 97
byte 1 112
byte 1 32
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $139
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $138
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 84
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $137
byte 1 72
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $136
byte 1 79
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $135
byte 1 79
byte 1 110
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 32
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $134
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $133
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
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
byte 1 0
align 1
LABELV $132
byte 1 83
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 32
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $131
byte 1 84
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $130
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 70
byte 1 111
byte 1 114
byte 1 32
byte 1 65
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $129
byte 1 84
byte 1 69
byte 1 65
byte 1 77
byte 1 84
byte 1 79
byte 1 85
byte 1 82
byte 1 78
byte 1 65
byte 1 77
byte 1 69
byte 1 78
byte 1 84
byte 1 0
align 1
LABELV $128
byte 1 72
byte 1 65
byte 1 82
byte 1 86
byte 1 69
byte 1 83
byte 1 84
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $127
byte 1 79
byte 1 86
byte 1 69
byte 1 82
byte 1 76
byte 1 79
byte 1 65
byte 1 68
byte 1 0
align 1
LABELV $126
byte 1 49
byte 1 70
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $125
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $124
byte 1 84
byte 1 69
byte 1 65
byte 1 77
byte 1 32
byte 1 68
byte 1 77
byte 1 0
align 1
LABELV $123
byte 1 83
byte 1 80
byte 1 0
align 1
LABELV $122
byte 1 84
byte 1 79
byte 1 85
byte 1 82
byte 1 78
byte 1 65
byte 1 77
byte 1 69
byte 1 78
byte 1 84
byte 1 0
align 1
LABELV $121
byte 1 70
byte 1 70
byte 1 65
byte 1 0
align 1
LABELV $120
byte 1 111
byte 1 115
byte 1 112
byte 1 0
align 1
LABELV $119
byte 1 79
byte 1 83
byte 1 80
byte 1 0
align 1
LABELV $118
byte 1 119
byte 1 102
byte 1 97
byte 1 0
align 1
LABELV $117
byte 1 87
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 115
byte 1 32
byte 1 70
byte 1 97
byte 1 99
byte 1 116
byte 1 111
byte 1 114
byte 1 121
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $116
byte 1 97
byte 1 108
byte 1 108
byte 1 105
byte 1 97
byte 1 110
byte 1 99
byte 1 101
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $115
byte 1 65
byte 1 108
byte 1 108
byte 1 105
byte 1 97
byte 1 110
byte 1 99
byte 1 101
byte 1 0
align 1
LABELV $114
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $113
byte 1 82
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $112
byte 1 109
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 112
byte 1 97
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $111
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $110
byte 1 81
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 32
byte 1 51
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $109
byte 1 0
align 1
LABELV $108
byte 1 65
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $107
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $106
byte 1 73
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 110
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $105
byte 1 77
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $104
byte 1 76
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $103
byte 1 78
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 109
byte 1 97
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $102
byte 1 72
byte 1 97
byte 1 114
byte 1 100
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $101
byte 1 72
byte 1 117
byte 1 114
byte 1 116
byte 1 32
byte 1 77
byte 1 101
byte 1 32
byte 1 80
byte 1 108
byte 1 101
byte 1 110
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $100
byte 1 66
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 73
byte 1 116
byte 1 32
byte 1 79
byte 1 110
byte 1 0
align 1
LABELV $99
byte 1 73
byte 1 32
byte 1 67
byte 1 97
byte 1 110
byte 1 32
byte 1 87
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $98
byte 1 68
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $97
byte 1 78
byte 1 111
byte 1 118
byte 1 0
align 1
LABELV $96
byte 1 79
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $95
byte 1 83
byte 1 101
byte 1 112
byte 1 0
align 1
LABELV $94
byte 1 65
byte 1 117
byte 1 103
byte 1 0
align 1
LABELV $93
byte 1 74
byte 1 117
byte 1 108
byte 1 0
align 1
LABELV $92
byte 1 74
byte 1 117
byte 1 110
byte 1 0
align 1
LABELV $91
byte 1 77
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $90
byte 1 65
byte 1 112
byte 1 114
byte 1 0
align 1
LABELV $89
byte 1 77
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $88
byte 1 70
byte 1 101
byte 1 98
byte 1 0
align 1
LABELV $87
byte 1 74
byte 1 97
byte 1 110
byte 1 0
