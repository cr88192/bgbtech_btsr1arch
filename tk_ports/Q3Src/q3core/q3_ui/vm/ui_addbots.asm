code
proc UI_AddBotsMenu_FightEvent 16 20
file "../ui_addbots.c"
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
;26:ADD BOTS MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ART_BACK0			"menu/art/back_0"
;36:#define ART_BACK1			"menu/art/back_1"	
;37:#define ART_FIGHT0			"menu/art/accept_0"
;38:#define ART_FIGHT1			"menu/art/accept_1"
;39:#define ART_BACKGROUND		"menu/art/addbotframe"
;40:#define ART_ARROWS			"menu/art/arrows_vert_0"
;41:#define ART_ARROWUP			"menu/art/arrows_vert_top"
;42:#define ART_ARROWDOWN		"menu/art/arrows_vert_bot"
;43:
;44:#define ID_BACK				10
;45:#define ID_GO				11
;46:#define ID_LIST				12
;47:#define ID_UP				13
;48:#define ID_DOWN				14
;49:#define ID_SKILL			15
;50:#define ID_TEAM				16
;51:#define ID_BOTNAME0			20
;52:#define ID_BOTNAME1			21
;53:#define ID_BOTNAME2			22
;54:#define ID_BOTNAME3			23
;55:#define ID_BOTNAME4			24
;56:#define ID_BOTNAME5			25
;57:#define ID_BOTNAME6			26
;58:
;59:
;60:typedef struct {
;61:	menuframework_s	menu;
;62:	menubitmap_s	arrows;
;63:	menubitmap_s	up;
;64:	menubitmap_s	down;
;65:	menutext_s		bots[7];
;66:	menulist_s		skill;
;67:	menulist_s		team;
;68:	menubitmap_s	go;
;69:	menubitmap_s	back;
;70:
;71:	int				numBots;
;72:	int				delay;
;73:	int				baseBotNum;
;74:	int				selectedBotNum;
;75:	int				sortedBotNums[MAX_BOTS];
;76:	char			botnames[7][32];
;77:} addBotsMenuInfo_t;
;78:
;79:static addBotsMenuInfo_t	addBotsMenuInfo;
;80:
;81:
;82:/*
;83:=================
;84:UI_AddBotsMenu_FightEvent
;85:=================
;86:*/
;87:static void UI_AddBotsMenu_FightEvent( void* ptr, int event ) {
line 91
;88:	const char	*team;
;89:	int			skill;
;90:
;91:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $69
line 92
;92:		return;
ADDRGP4 $68
JUMPV
LABELV $69
line 95
;93:	}
;94:
;95:	team = addBotsMenuInfo.team.itemnames[addBotsMenuInfo.team.curvalue];
ADDRLP4 0
ADDRGP4 addBotsMenuInfo+1152+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 addBotsMenuInfo+1152+76
INDIRP4
ADDP4
INDIRP4
ASGNP4
line 96
;96:	skill = addBotsMenuInfo.skill.curvalue + 1;
ADDRLP4 4
ADDRGP4 addBotsMenuInfo+1056+64
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 98
;97:
;98:	trap_Cmd_ExecuteText( EXEC_APPEND, va("addbot %s %i %s %i\n",
ADDRGP4 $77
ARGP4
ADDRGP4 addBotsMenuInfo+1436
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 addBotsMenuInfo+5536
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 addBotsMenuInfo+1428
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 101
;99:		addBotsMenuInfo.botnames[addBotsMenuInfo.selectedBotNum], skill, team, addBotsMenuInfo.delay) );
;100:
;101:	addBotsMenuInfo.delay += 1500;
ADDRLP4 12
ADDRGP4 addBotsMenuInfo+1428
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 102
;102:}
LABELV $68
endproc UI_AddBotsMenu_FightEvent 16 20
proc UI_AddBotsMenu_BotEvent 0 0
line 110
;103:
;104:
;105:/*
;106:=================
;107:UI_AddBotsMenu_BotEvent
;108:=================
;109:*/
;110:static void UI_AddBotsMenu_BotEvent( void* ptr, int event ) {
line 111
;111:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $83
line 112
;112:		return;
ADDRGP4 $82
JUMPV
LABELV $83
line 115
;113:	}
;114:
;115:	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].color = color_orange;
CNSTI4 72
ADDRGP4 addBotsMenuInfo+1436
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 116
;116:	addBotsMenuInfo.selectedBotNum = ((menucommon_s*)ptr)->id - ID_BOTNAME0;
ADDRGP4 addBotsMenuInfo+1436
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 20
SUBI4
ASGNI4
line 117
;117:	addBotsMenuInfo.bots[addBotsMenuInfo.selectedBotNum].color = color_white;
CNSTI4 72
ADDRGP4 addBotsMenuInfo+1436
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+68
ADDP4
ADDRGP4 color_white
ASGNP4
line 118
;118:}
LABELV $82
endproc UI_AddBotsMenu_BotEvent 0 0
proc UI_AddBotsMenu_BackEvent 0 0
line 126
;119:
;120:
;121:/*
;122:=================
;123:UI_AddBotsMenu_BackEvent
;124:=================
;125:*/
;126:static void UI_AddBotsMenu_BackEvent( void* ptr, int event ) {
line 127
;127:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $93
line 128
;128:		return;
ADDRGP4 $92
JUMPV
LABELV $93
line 130
;129:	}
;130:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 131
;131:}
LABELV $92
endproc UI_AddBotsMenu_BackEvent 0 0
proc UI_AddBotsMenu_SetBotNames 16 12
line 139
;132:
;133:
;134:/*
;135:=================
;136:UI_AddBotsMenu_SetBotNames
;137:=================
;138:*/
;139:static void UI_AddBotsMenu_SetBotNames( void ) {
line 143
;140:	int			n;
;141:	const char	*info;
;142:
;143:	for ( n = 0; n < 7; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $96
line 144
;144:		info = UI_GetBotInfoByNumber( addBotsMenuInfo.sortedBotNums[addBotsMenuInfo.baseBotNum + n] );
ADDRGP4 addBotsMenuInfo+1432
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CNSTI4 2
LSHI4
ADDRGP4 addBotsMenuInfo+1440
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 145
;145:		Q_strncpyz( addBotsMenuInfo.botnames[n], Info_ValueForKey( info, "name" ), sizeof(addBotsMenuInfo.botnames[n]) );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $103
ARGP4
ADDRLP4 12
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 addBotsMenuInfo+5536
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 146
;146:	}
LABELV $97
line 143
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LTI4 $96
line 148
;147:
;148:}
LABELV $95
endproc UI_AddBotsMenu_SetBotNames 16 12
proc UI_AddBotsMenu_UpEvent 4 0
line 156
;149:
;150:
;151:/*
;152:=================
;153:UI_AddBotsMenu_UpEvent
;154:=================
;155:*/
;156:static void UI_AddBotsMenu_UpEvent( void* ptr, int event ) {
line 157
;157:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $106
line 158
;158:		return;
ADDRGP4 $105
JUMPV
LABELV $106
line 161
;159:	}
;160:
;161:	if( addBotsMenuInfo.baseBotNum > 0 ) {
ADDRGP4 addBotsMenuInfo+1432
INDIRI4
CNSTI4 0
LEI4 $108
line 162
;162:		addBotsMenuInfo.baseBotNum--;
ADDRLP4 0
ADDRGP4 addBotsMenuInfo+1432
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 163
;163:		UI_AddBotsMenu_SetBotNames();
ADDRGP4 UI_AddBotsMenu_SetBotNames
CALLV
pop
line 164
;164:	}
LABELV $108
line 165
;165:}
LABELV $105
endproc UI_AddBotsMenu_UpEvent 4 0
proc UI_AddBotsMenu_DownEvent 4 0
line 173
;166:
;167:
;168:/*
;169:=================
;170:UI_AddBotsMenu_DownEvent
;171:=================
;172:*/
;173:static void UI_AddBotsMenu_DownEvent( void* ptr, int event ) {
line 174
;174:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $113
line 175
;175:		return;
ADDRGP4 $112
JUMPV
LABELV $113
line 178
;176:	}
;177:
;178:	if( addBotsMenuInfo.baseBotNum + 7 < addBotsMenuInfo.numBots ) {
ADDRGP4 addBotsMenuInfo+1432
INDIRI4
CNSTI4 7
ADDI4
ADDRGP4 addBotsMenuInfo+1424
INDIRI4
GEI4 $115
line 179
;179:		addBotsMenuInfo.baseBotNum++;
ADDRLP4 0
ADDRGP4 addBotsMenuInfo+1432
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 180
;180:		UI_AddBotsMenu_SetBotNames();
ADDRGP4 UI_AddBotsMenu_SetBotNames
CALLV
pop
line 181
;181:	}
LABELV $115
line 182
;182:}
LABELV $112
endproc UI_AddBotsMenu_DownEvent 4 0
proc UI_AddBotsMenu_SortCompare 44 8
line 190
;183:
;184:
;185:/*
;186:=================
;187:UI_AddBotsMenu_GetSortedBotNums
;188:=================
;189:*/
;190:static int QDECL UI_AddBotsMenu_SortCompare( const void *arg1, const void *arg2 ) {
line 195
;191:	int			num1, num2;
;192:	const char	*info1, *info2;
;193:	const char	*name1, *name2;
;194:
;195:	num1 = *(int *)arg1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 196
;196:	num2 = *(int *)arg2;
ADDRLP4 4
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
line 198
;197:
;198:	info1 = UI_GetBotInfoByNumber( num1 );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 24
INDIRP4
ASGNP4
line 199
;199:	info2 = UI_GetBotInfoByNumber( num2 );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 28
INDIRP4
ASGNP4
line 201
;200:
;201:	name1 = Info_ValueForKey( info1, "name" );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $103
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 32
INDIRP4
ASGNP4
line 202
;202:	name2 = Info_ValueForKey( info2, "name" );
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $103
ARGP4
ADDRLP4 36
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 20
ADDRLP4 36
INDIRP4
ASGNP4
line 204
;203:
;204:	return Q_stricmp( name1, name2 );
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
RETI4
LABELV $120
endproc UI_AddBotsMenu_SortCompare 44 8
proc UI_AddBotsMenu_GetSortedBotNums 8 16
line 207
;205:}
;206:
;207:static void UI_AddBotsMenu_GetSortedBotNums( void ) {
line 211
;208:	int		n;
;209:
;210:	// initialize the array
;211:	for( n = 0; n < addBotsMenuInfo.numBots; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $125
JUMPV
LABELV $122
line 212
;212:		addBotsMenuInfo.sortedBotNums[n] = n;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 addBotsMenuInfo+1440
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 213
;213:	}
LABELV $123
line 211
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $125
ADDRLP4 0
INDIRI4
ADDRGP4 addBotsMenuInfo+1424
INDIRI4
LTI4 $122
line 215
;214:
;215:	qsort( addBotsMenuInfo.sortedBotNums, addBotsMenuInfo.numBots, sizeof(addBotsMenuInfo.sortedBotNums[0]), UI_AddBotsMenu_SortCompare );
ADDRGP4 addBotsMenuInfo+1440
ARGP4
ADDRGP4 addBotsMenuInfo+1424
INDIRI4
ARGI4
CNSTI4 4
ARGI4
ADDRGP4 UI_AddBotsMenu_SortCompare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 216
;216:}
LABELV $121
endproc UI_AddBotsMenu_GetSortedBotNums 8 16
proc UI_AddBotsMenu_Draw 0 20
line 224
;217:
;218:
;219:/*
;220:=================
;221:UI_AddBotsMenu_Draw
;222:=================
;223:*/
;224:static void UI_AddBotsMenu_Draw( void ) {
line 225
;225:	UI_DrawBannerString( 320, 16, "ADD BOTS", UI_CENTER, color_white );
CNSTI4 320
ARGI4
CNSTI4 16
ARGI4
ADDRGP4 $132
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawBannerString
CALLV
pop
line 226
;226:	UI_DrawNamedPic( 320-233, 240-166, 466, 332, ART_BACKGROUND );
CNSTF4 1118699520
ARGF4
CNSTF4 1116995584
ARGF4
CNSTF4 1139343360
ARGF4
CNSTF4 1134952448
ARGF4
ADDRGP4 $133
ARGP4
ADDRGP4 UI_DrawNamedPic
CALLV
pop
line 229
;227:
;228:	// standard menu drawing
;229:	Menu_Draw( &addBotsMenuInfo.menu );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 230
;230:}
LABELV $131
endproc UI_AddBotsMenu_Draw 0 20
data
align 4
LABELV skillNames
address $134
address $135
address $136
address $137
address $138
byte 4 0
align 4
LABELV teamNames1
address $139
byte 4 0
align 4
LABELV teamNames2
address $140
address $141
byte 4 0
code
proc UI_AddBotsMenu_Init 1064 12
line 258
;231:
;232:	
;233:/*
;234:=================
;235:UI_AddBotsMenu_Init
;236:=================
;237:*/
;238:static const char *skillNames[] = {
;239:	"I Can Win",
;240:	"Bring It On",
;241:	"Hurt Me Plenty",
;242:	"Hardcore",
;243:	"Nightmare!",
;244:	0
;245:};
;246:
;247:static const char *teamNames1[] = {
;248:	"Free",
;249:	0
;250:};
;251:
;252:static const char *teamNames2[] = {
;253:	"Red",
;254:	"Blue",
;255:	0
;256:};
;257:
;258:static void UI_AddBotsMenu_Init( void ) {
line 265
;259:	int		n;
;260:	int		y;
;261:	int		gametype;
;262:	int		count;
;263:	char	info[MAX_INFO_STRING];
;264:
;265:	trap_GetConfigString(CS_SERVERINFO, info, MAX_INFO_STRING);   
CNSTI4 0
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 266
;266:	gametype = atoi( Info_ValueForKey( info,"g_gametype" ) );
ADDRLP4 16
ARGP4
ADDRGP4 $143
ARGP4
ADDRLP4 1040
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
ARGP4
ADDRLP4 1044
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 1044
INDIRI4
ASGNI4
line 268
;267:
;268:	memset( &addBotsMenuInfo, 0 ,sizeof(addBotsMenuInfo) );
ADDRGP4 addBotsMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 5760
ARGI4
ADDRGP4 memset
CALLP4
pop
line 269
;269:	addBotsMenuInfo.menu.draw = UI_AddBotsMenu_Draw;
ADDRGP4 addBotsMenuInfo+268
ADDRGP4 UI_AddBotsMenu_Draw
ASGNP4
line 270
;270:	addBotsMenuInfo.menu.fullscreen = qfalse;
ADDRGP4 addBotsMenuInfo+280
CNSTI4 0
ASGNI4
line 271
;271:	addBotsMenuInfo.menu.wrapAround = qtrue;
ADDRGP4 addBotsMenuInfo+276
CNSTI4 1
ASGNI4
line 272
;272:	addBotsMenuInfo.delay = 1000;
ADDRGP4 addBotsMenuInfo+1428
CNSTI4 1000
ASGNI4
line 274
;273:
;274:	UI_AddBots_Cache();
ADDRGP4 UI_AddBots_Cache
CALLV
pop
line 276
;275:
;276:	addBotsMenuInfo.numBots = UI_GetNumBots();
ADDRLP4 1048
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRGP4 addBotsMenuInfo+1424
ADDRLP4 1048
INDIRI4
ASGNI4
line 277
;277:	count = addBotsMenuInfo.numBots < 7 ? addBotsMenuInfo.numBots : 7;
ADDRGP4 addBotsMenuInfo+1424
INDIRI4
CNSTI4 7
GEI4 $152
ADDRLP4 1052
ADDRGP4 addBotsMenuInfo+1424
INDIRI4
ASGNI4
ADDRGP4 $153
JUMPV
LABELV $152
ADDRLP4 1052
CNSTI4 7
ASGNI4
LABELV $153
ADDRLP4 8
ADDRLP4 1052
INDIRI4
ASGNI4
line 279
;278:
;279:	addBotsMenuInfo.arrows.generic.type  = MTYPE_BITMAP;
ADDRGP4 addBotsMenuInfo+288
CNSTI4 6
ASGNI4
line 280
;280:	addBotsMenuInfo.arrows.generic.name  = ART_ARROWS;
ADDRGP4 addBotsMenuInfo+288+4
ADDRGP4 $157
ASGNP4
line 281
;281:	addBotsMenuInfo.arrows.generic.flags = QMF_INACTIVE;
ADDRGP4 addBotsMenuInfo+288+44
CNSTU4 16384
ASGNU4
line 282
;282:	addBotsMenuInfo.arrows.generic.x	 = 200;
ADDRGP4 addBotsMenuInfo+288+12
CNSTI4 200
ASGNI4
line 283
;283:	addBotsMenuInfo.arrows.generic.y	 = 128;
ADDRGP4 addBotsMenuInfo+288+16
CNSTI4 128
ASGNI4
line 284
;284:	addBotsMenuInfo.arrows.width  	     = 64;
ADDRGP4 addBotsMenuInfo+288+76
CNSTI4 64
ASGNI4
line 285
;285:	addBotsMenuInfo.arrows.height  	     = 128;
ADDRGP4 addBotsMenuInfo+288+80
CNSTI4 128
ASGNI4
line 287
;286:
;287:	addBotsMenuInfo.up.generic.type	    = MTYPE_BITMAP;
ADDRGP4 addBotsMenuInfo+376
CNSTI4 6
ASGNI4
line 288
;288:	addBotsMenuInfo.up.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 addBotsMenuInfo+376+44
CNSTU4 260
ASGNU4
line 289
;289:	addBotsMenuInfo.up.generic.x		= 200;
ADDRGP4 addBotsMenuInfo+376+12
CNSTI4 200
ASGNI4
line 290
;290:	addBotsMenuInfo.up.generic.y		= 128;
ADDRGP4 addBotsMenuInfo+376+16
CNSTI4 128
ASGNI4
line 291
;291:	addBotsMenuInfo.up.generic.id	    = ID_UP;
ADDRGP4 addBotsMenuInfo+376+8
CNSTI4 13
ASGNI4
line 292
;292:	addBotsMenuInfo.up.generic.callback = UI_AddBotsMenu_UpEvent;
ADDRGP4 addBotsMenuInfo+376+48
ADDRGP4 UI_AddBotsMenu_UpEvent
ASGNP4
line 293
;293:	addBotsMenuInfo.up.width  		    = 64;
ADDRGP4 addBotsMenuInfo+376+76
CNSTI4 64
ASGNI4
line 294
;294:	addBotsMenuInfo.up.height  		    = 64;
ADDRGP4 addBotsMenuInfo+376+80
CNSTI4 64
ASGNI4
line 295
;295:	addBotsMenuInfo.up.focuspic         = ART_ARROWUP;
ADDRGP4 addBotsMenuInfo+376+60
ADDRGP4 $185
ASGNP4
line 297
;296:
;297:	addBotsMenuInfo.down.generic.type	  = MTYPE_BITMAP;
ADDRGP4 addBotsMenuInfo+464
CNSTI4 6
ASGNI4
line 298
;298:	addBotsMenuInfo.down.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 addBotsMenuInfo+464+44
CNSTU4 260
ASGNU4
line 299
;299:	addBotsMenuInfo.down.generic.x		  = 200;
ADDRGP4 addBotsMenuInfo+464+12
CNSTI4 200
ASGNI4
line 300
;300:	addBotsMenuInfo.down.generic.y		  = 128+64;
ADDRGP4 addBotsMenuInfo+464+16
CNSTI4 192
ASGNI4
line 301
;301:	addBotsMenuInfo.down.generic.id	      = ID_DOWN;
ADDRGP4 addBotsMenuInfo+464+8
CNSTI4 14
ASGNI4
line 302
;302:	addBotsMenuInfo.down.generic.callback = UI_AddBotsMenu_DownEvent;
ADDRGP4 addBotsMenuInfo+464+48
ADDRGP4 UI_AddBotsMenu_DownEvent
ASGNP4
line 303
;303:	addBotsMenuInfo.down.width  		  = 64;
ADDRGP4 addBotsMenuInfo+464+76
CNSTI4 64
ASGNI4
line 304
;304:	addBotsMenuInfo.down.height  		  = 64;
ADDRGP4 addBotsMenuInfo+464+80
CNSTI4 64
ASGNI4
line 305
;305:	addBotsMenuInfo.down.focuspic         = ART_ARROWDOWN;
ADDRGP4 addBotsMenuInfo+464+60
ADDRGP4 $203
ASGNP4
line 307
;306:
;307:	for( n = 0, y = 120; n < count; n++, y += 20 ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 120
ASGNI4
ADDRGP4 $207
JUMPV
LABELV $204
line 308
;308:		addBotsMenuInfo.bots[n].generic.type		= MTYPE_PTEXT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552
ADDP4
CNSTI4 9
ASGNI4
line 309
;309:		addBotsMenuInfo.bots[n].generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+44
ADDP4
CNSTU4 260
ASGNU4
line 310
;310:		addBotsMenuInfo.bots[n].generic.id			= ID_BOTNAME0 + n;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
line 311
;311:		addBotsMenuInfo.bots[n].generic.x			= 320 - 56;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+12
ADDP4
CNSTI4 264
ASGNI4
line 312
;312:		addBotsMenuInfo.bots[n].generic.y			= y;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 313
;313:		addBotsMenuInfo.bots[n].generic.callback	= UI_AddBotsMenu_BotEvent;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+48
ADDP4
ADDRGP4 UI_AddBotsMenu_BotEvent
ASGNP4
line 314
;314:		addBotsMenuInfo.bots[n].string				= addBotsMenuInfo.botnames[n];
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+60
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 addBotsMenuInfo+5536
ADDP4
ASGNP4
line 315
;315:		addBotsMenuInfo.bots[n].color				= color_orange;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 316
;316:		addBotsMenuInfo.bots[n].style				= UI_LEFT|UI_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552+64
ADDP4
CNSTI4 16
ASGNI4
line 317
;317:	}
LABELV $205
line 307
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
LABELV $207
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $204
line 319
;318:
;319:	y += 12;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
line 320
;320:	addBotsMenuInfo.skill.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 addBotsMenuInfo+1056
CNSTI4 3
ASGNI4
line 321
;321:	addBotsMenuInfo.skill.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 addBotsMenuInfo+1056+44
CNSTU4 258
ASGNU4
line 322
;322:	addBotsMenuInfo.skill.generic.x			= 320;
ADDRGP4 addBotsMenuInfo+1056+12
CNSTI4 320
ASGNI4
line 323
;323:	addBotsMenuInfo.skill.generic.y			= y;
ADDRGP4 addBotsMenuInfo+1056+16
ADDRLP4 4
INDIRI4
ASGNI4
line 324
;324:	addBotsMenuInfo.skill.generic.name		= "Skill:";
ADDRGP4 addBotsMenuInfo+1056+4
ADDRGP4 $235
ASGNP4
line 325
;325:	addBotsMenuInfo.skill.generic.id		= ID_SKILL;
ADDRGP4 addBotsMenuInfo+1056+8
CNSTI4 15
ASGNI4
line 326
;326:	addBotsMenuInfo.skill.itemnames			= skillNames;
ADDRGP4 addBotsMenuInfo+1056+76
ADDRGP4 skillNames
ASGNP4
line 327
;327:	addBotsMenuInfo.skill.curvalue			= Com_Clamp( 0, 4, (int)trap_Cvar_VariableValue( "g_spSkill" ) - 1 );
ADDRGP4 $242
ARGP4
ADDRLP4 1056
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1082130432
ARGF4
ADDRLP4 1056
INDIRF4
CVFI4 4
CNSTI4 1
SUBI4
CVIF4 4
ARGF4
ADDRLP4 1060
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 addBotsMenuInfo+1056+64
ADDRLP4 1060
INDIRF4
CVFI4 4
ASGNI4
line 329
;328:
;329:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 330
;330:	addBotsMenuInfo.team.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 addBotsMenuInfo+1152
CNSTI4 3
ASGNI4
line 331
;331:	addBotsMenuInfo.team.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 addBotsMenuInfo+1152+44
CNSTU4 258
ASGNU4
line 332
;332:	addBotsMenuInfo.team.generic.x			= 320;
ADDRGP4 addBotsMenuInfo+1152+12
CNSTI4 320
ASGNI4
line 333
;333:	addBotsMenuInfo.team.generic.y			= y;
ADDRGP4 addBotsMenuInfo+1152+16
ADDRLP4 4
INDIRI4
ASGNI4
line 334
;334:	addBotsMenuInfo.team.generic.name		= "Team: ";
ADDRGP4 addBotsMenuInfo+1152+4
ADDRGP4 $252
ASGNP4
line 335
;335:	addBotsMenuInfo.team.generic.id			= ID_TEAM;
ADDRGP4 addBotsMenuInfo+1152+8
CNSTI4 16
ASGNI4
line 336
;336:	if( gametype >= GT_TEAM ) {
ADDRLP4 12
INDIRI4
CNSTI4 3
LTI4 $255
line 337
;337:		addBotsMenuInfo.team.itemnames		= teamNames2;
ADDRGP4 addBotsMenuInfo+1152+76
ADDRGP4 teamNames2
ASGNP4
line 338
;338:	}
ADDRGP4 $256
JUMPV
LABELV $255
line 339
;339:	else {
line 340
;340:		addBotsMenuInfo.team.itemnames		= teamNames1;
ADDRGP4 addBotsMenuInfo+1152+76
ADDRGP4 teamNames1
ASGNP4
line 341
;341:		addBotsMenuInfo.team.generic.flags	= QMF_GRAYED;
ADDRGP4 addBotsMenuInfo+1152+44
CNSTU4 8192
ASGNU4
line 342
;342:	}
LABELV $256
line 344
;343:
;344:	addBotsMenuInfo.go.generic.type			= MTYPE_BITMAP;
ADDRGP4 addBotsMenuInfo+1248
CNSTI4 6
ASGNI4
line 345
;345:	addBotsMenuInfo.go.generic.name			= ART_FIGHT0;
ADDRGP4 addBotsMenuInfo+1248+4
ADDRGP4 $266
ASGNP4
line 346
;346:	addBotsMenuInfo.go.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 addBotsMenuInfo+1248+44
CNSTU4 260
ASGNU4
line 347
;347:	addBotsMenuInfo.go.generic.id			= ID_GO;
ADDRGP4 addBotsMenuInfo+1248+8
CNSTI4 11
ASGNI4
line 348
;348:	addBotsMenuInfo.go.generic.callback		= UI_AddBotsMenu_FightEvent;
ADDRGP4 addBotsMenuInfo+1248+48
ADDRGP4 UI_AddBotsMenu_FightEvent
ASGNP4
line 349
;349:	addBotsMenuInfo.go.generic.x			= 320+128-128;
ADDRGP4 addBotsMenuInfo+1248+12
CNSTI4 320
ASGNI4
line 350
;350:	addBotsMenuInfo.go.generic.y			= 256+128-64;
ADDRGP4 addBotsMenuInfo+1248+16
CNSTI4 320
ASGNI4
line 351
;351:	addBotsMenuInfo.go.width  				= 128;
ADDRGP4 addBotsMenuInfo+1248+76
CNSTI4 128
ASGNI4
line 352
;352:	addBotsMenuInfo.go.height  				= 64;
ADDRGP4 addBotsMenuInfo+1248+80
CNSTI4 64
ASGNI4
line 353
;353:	addBotsMenuInfo.go.focuspic				= ART_FIGHT1;
ADDRGP4 addBotsMenuInfo+1248+60
ADDRGP4 $283
ASGNP4
line 355
;354:
;355:	addBotsMenuInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 addBotsMenuInfo+1336
CNSTI4 6
ASGNI4
line 356
;356:	addBotsMenuInfo.back.generic.name		= ART_BACK0;
ADDRGP4 addBotsMenuInfo+1336+4
ADDRGP4 $287
ASGNP4
line 357
;357:	addBotsMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 addBotsMenuInfo+1336+44
CNSTU4 260
ASGNU4
line 358
;358:	addBotsMenuInfo.back.generic.id			= ID_BACK;
ADDRGP4 addBotsMenuInfo+1336+8
CNSTI4 10
ASGNI4
line 359
;359:	addBotsMenuInfo.back.generic.callback	= UI_AddBotsMenu_BackEvent;
ADDRGP4 addBotsMenuInfo+1336+48
ADDRGP4 UI_AddBotsMenu_BackEvent
ASGNP4
line 360
;360:	addBotsMenuInfo.back.generic.x			= 320-128;
ADDRGP4 addBotsMenuInfo+1336+12
CNSTI4 192
ASGNI4
line 361
;361:	addBotsMenuInfo.back.generic.y			= 256+128-64;
ADDRGP4 addBotsMenuInfo+1336+16
CNSTI4 320
ASGNI4
line 362
;362:	addBotsMenuInfo.back.width				= 128;
ADDRGP4 addBotsMenuInfo+1336+76
CNSTI4 128
ASGNI4
line 363
;363:	addBotsMenuInfo.back.height				= 64;
ADDRGP4 addBotsMenuInfo+1336+80
CNSTI4 64
ASGNI4
line 364
;364:	addBotsMenuInfo.back.focuspic			= ART_BACK1;
ADDRGP4 addBotsMenuInfo+1336+60
ADDRGP4 $304
ASGNP4
line 366
;365:
;366:	addBotsMenuInfo.baseBotNum = 0;
ADDRGP4 addBotsMenuInfo+1432
CNSTI4 0
ASGNI4
line 367
;367:	addBotsMenuInfo.selectedBotNum = 0;
ADDRGP4 addBotsMenuInfo+1436
CNSTI4 0
ASGNI4
line 368
;368:	addBotsMenuInfo.bots[0].color = color_white;
ADDRGP4 addBotsMenuInfo+552+68
ADDRGP4 color_white
ASGNP4
line 370
;369:
;370:	UI_AddBotsMenu_GetSortedBotNums();
ADDRGP4 UI_AddBotsMenu_GetSortedBotNums
CALLV
pop
line 371
;371:	UI_AddBotsMenu_SetBotNames();
ADDRGP4 UI_AddBotsMenu_SetBotNames
CALLV
pop
line 373
;372:
;373:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.arrows );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 375
;374:
;375:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.up );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+376
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 376
;376:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.down );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+464
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 377
;377:	for( n = 0; n < count; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $315
JUMPV
LABELV $312
line 378
;378:		Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.bots[n] );
ADDRGP4 addBotsMenuInfo
ARGP4
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 addBotsMenuInfo+552
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 379
;379:	}
LABELV $313
line 377
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $315
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $312
line 380
;380:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.skill );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+1056
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 381
;381:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.team );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+1152
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 382
;382:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.go );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+1248
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 383
;383:	Menu_AddItem( &addBotsMenuInfo.menu, &addBotsMenuInfo.back );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 addBotsMenuInfo+1336
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 384
;384:}
LABELV $142
endproc UI_AddBotsMenu_Init 1064 12
export UI_AddBots_Cache
proc UI_AddBots_Cache 0 4
line 392
;385:
;386:
;387:/*
;388:=================
;389:UI_AddBots_Cache
;390:=================
;391:*/
;392:void UI_AddBots_Cache( void ) {
line 393
;393:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $287
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 394
;394:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $304
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 395
;395:	trap_R_RegisterShaderNoMip( ART_FIGHT0 );
ADDRGP4 $266
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 396
;396:	trap_R_RegisterShaderNoMip( ART_FIGHT1 );
ADDRGP4 $283
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 397
;397:	trap_R_RegisterShaderNoMip( ART_BACKGROUND );
ADDRGP4 $133
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 398
;398:	trap_R_RegisterShaderNoMip( ART_ARROWS );
ADDRGP4 $157
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 399
;399:	trap_R_RegisterShaderNoMip( ART_ARROWUP );
ADDRGP4 $185
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 400
;400:	trap_R_RegisterShaderNoMip( ART_ARROWDOWN );
ADDRGP4 $203
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 401
;401:}
LABELV $321
endproc UI_AddBots_Cache 0 4
export UI_AddBotsMenu
proc UI_AddBotsMenu 0 4
line 409
;402:
;403:
;404:/*
;405:=================
;406:UI_AddBotsMenu
;407:=================
;408:*/
;409:void UI_AddBotsMenu( void ) {
line 410
;410:	UI_AddBotsMenu_Init();
ADDRGP4 UI_AddBotsMenu_Init
CALLV
pop
line 411
;411:	UI_PushMenu( &addBotsMenuInfo.menu );
ADDRGP4 addBotsMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 412
;412:}
LABELV $322
endproc UI_AddBotsMenu 0 4
bss
align 4
LABELV addBotsMenuInfo
skip 5760
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
import UI_RemoveBotsMenu
import UI_RemoveBots_Cache
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
LABELV $304
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
LABELV $287
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
LABELV $283
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
byte 1 99
byte 1 99
byte 1 101
byte 1 112
byte 1 116
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $266
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
byte 1 99
byte 1 99
byte 1 101
byte 1 112
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $252
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 58
byte 1 32
byte 1 0
align 1
LABELV $242
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
LABELV $235
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 58
byte 1 0
align 1
LABELV $203
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
LABELV $185
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
LABELV $157
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
LABELV $143
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
LABELV $141
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $140
byte 1 82
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $139
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 0
align 1
LABELV $138
byte 1 78
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 109
byte 1 97
byte 1 114
byte 1 101
byte 1 33
byte 1 0
align 1
LABELV $137
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
LABELV $136
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
LABELV $135
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
LABELV $134
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
LABELV $132
byte 1 65
byte 1 68
byte 1 68
byte 1 32
byte 1 66
byte 1 79
byte 1 84
byte 1 83
byte 1 0
align 1
LABELV $103
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $77
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
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
