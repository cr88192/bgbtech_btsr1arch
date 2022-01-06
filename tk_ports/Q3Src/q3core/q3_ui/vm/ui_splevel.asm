code
proc PlayerIcon 80 20
file "../ui_splevel.c"
line 128
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
;24:=============================================================================
;25:
;26:SINGLE PLAYER LEVEL SELECT MENU
;27:
;28:=============================================================================
;29:*/
;30:
;31:#include "ui_local.h"
;32:
;33:
;34:#define ART_LEVELFRAME_FOCUS		"menu/art/maps_select"
;35:#define ART_LEVELFRAME_SELECTED		"menu/art/maps_selected"
;36:#define ART_ARROW					"menu/art/narrow_0"
;37:#define ART_ARROW_FOCUS				"menu/art/narrow_1"
;38:#define ART_MAP_UNKNOWN				"menu/art/unknownmap"
;39:#define ART_MAP_COMPLETE1			"menu/art/level_complete1"
;40:#define ART_MAP_COMPLETE2			"menu/art/level_complete2"
;41:#define ART_MAP_COMPLETE3			"menu/art/level_complete3"
;42:#define ART_MAP_COMPLETE4			"menu/art/level_complete4"
;43:#define ART_MAP_COMPLETE5			"menu/art/level_complete5"
;44:#define ART_BACK0					"menu/art/back_0"
;45:#define ART_BACK1					"menu/art/back_1"	
;46:#define ART_FIGHT0					"menu/art/fight_0"
;47:#define ART_FIGHT1					"menu/art/fight_1"
;48:#define ART_RESET0					"menu/art/reset_0"
;49:#define ART_RESET1					"menu/art/reset_1"	
;50:#define ART_CUSTOM0					"menu/art/skirmish_0"
;51:#define ART_CUSTOM1					"menu/art/skirmish_1"
;52:
;53:#define ID_LEFTARROW		10
;54:#define ID_PICTURE0			11
;55:#define ID_PICTURE1			12
;56:#define ID_PICTURE2			13
;57:#define ID_PICTURE3			14
;58:#define ID_RIGHTARROW		15
;59:#define ID_PLAYERPIC		16
;60:#define ID_AWARD1			17
;61:#define ID_AWARD2			18
;62:#define ID_AWARD3			19
;63:#define ID_AWARD4			20
;64:#define ID_AWARD5			21
;65:#define ID_AWARD6			22
;66:#define ID_BACK				23
;67:#define ID_RESET			24
;68:#define ID_CUSTOM			25
;69:#define ID_NEXT				26
;70:
;71:#define PLAYER_Y			314
;72:#define AWARDS_Y			(PLAYER_Y + 26)
;73:
;74:
;75:typedef struct {
;76:	menuframework_s	menu;
;77:	menutext_s		item_banner;
;78:	menubitmap_s	item_leftarrow;
;79:	menubitmap_s	item_maps[4];
;80:	menubitmap_s	item_rightarrow;
;81:	menubitmap_s	item_player;
;82:	menubitmap_s	item_awards[6];
;83:	menubitmap_s	item_back;
;84:	menubitmap_s	item_reset;
;85:	menubitmap_s	item_custom;
;86:	menubitmap_s	item_next;
;87:	menubitmap_s	item_null;
;88:
;89:	qboolean		reinit;
;90:
;91:	const char *	selectedArenaInfo;
;92:	int				numMaps;
;93:	char			levelPicNames[4][MAX_QPATH];
;94:	char			levelNames[4][16];
;95:	int				levelScores[4];
;96:	int				levelScoresSkill[4];
;97:	qhandle_t		levelSelectedPic;
;98:	qhandle_t		levelFocusPic;
;99:	qhandle_t		levelCompletePic[5];
;100:
;101:	char			playerModel[MAX_QPATH];
;102:	char			playerPicName[MAX_QPATH];
;103:	int				awardLevels[6];
;104:	sfxHandle_t		awardSounds[6];
;105:
;106:	int				numBots;
;107:	qhandle_t		botPics[7];
;108:	char			botNames[7][10];
;109:} levelMenuInfo_t;
;110:
;111:static levelMenuInfo_t	levelMenuInfo;
;112:
;113:static int	selectedArenaSet;
;114:static int	selectedArena;
;115:static int	currentSet;
;116:static int	currentGame;
;117:static int	trainingTier;
;118:static int	finalTier;
;119:static int	minTier;
;120:static int	maxTier;
;121:
;122:
;123:/*
;124:=================
;125:PlayerIcon
;126:=================
;127:*/
;128:static void PlayerIcon( const char *modelAndSkin, char *iconName, int iconNameMaxSize ) {
line 132
;129:	char	*skin;
;130:	char	model[MAX_QPATH];
;131:
;132:	Q_strncpyz( model, modelAndSkin, sizeof(model));
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 133
;133:	skin = Q_strrchr( model, '/' );
ADDRLP4 4
ARGP4
CNSTI4 47
ARGI4
ADDRLP4 68
ADDRGP4 Q_strrchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 68
INDIRP4
ASGNP4
line 134
;134:	if ( skin ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $69
line 135
;135:		*skin++ = '\0';
ADDRLP4 72
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 72
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI1 0
ASGNI1
line 136
;136:	}
ADDRGP4 $70
JUMPV
LABELV $69
line 137
;137:	else {
line 138
;138:		skin = "default";
ADDRLP4 0
ADDRGP4 $71
ASGNP4
line 139
;139:	}
LABELV $70
line 141
;140:
;141:	Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_%s.tga", model, skin );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 $72
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 143
;142:
;143:	if( !trap_R_RegisterShaderNoMip( iconName ) && Q_stricmp( skin, "default" ) != 0 ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $73
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $71
ARGP4
ADDRLP4 76
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
EQI4 $73
line 144
;144:		Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_default.tga", model );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 $75
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 145
;145:	}
LABELV $73
line 146
;146:}
LABELV $68
endproc PlayerIcon 80 20
proc PlayerIconHandle 68 12
line 154
;147:
;148:
;149:/*
;150:=================
;151:PlayerIconhandle
;152:=================
;153:*/
;154:static qhandle_t PlayerIconHandle( const char *modelAndSkin ) {
line 157
;155:	char	iconName[MAX_QPATH];
;156:
;157:	PlayerIcon( modelAndSkin, iconName, sizeof(iconName) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 PlayerIcon
CALLV
pop
line 158
;158:	return trap_R_RegisterShaderNoMip( iconName );
ADDRLP4 0
ARGP4
ADDRLP4 64
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
RETI4
LABELV $76
endproc PlayerIconHandle 68 12
proc UI_SPLevelMenu_SetBots 1068 12
line 167
;159:}
;160:
;161:
;162:/*
;163:=================
;164:UI_SPLevelMenu_SetBots
;165:=================
;166:*/
;167:static void UI_SPLevelMenu_SetBots( void ) {
line 173
;168:	char	*p;
;169:	char	*bot;
;170:	char	*botInfo;
;171:	char	bots[MAX_INFO_STRING];
;172:
;173:	levelMenuInfo.numBots = 0;
ADDRGP4 levelMenuInfo+2512
CNSTI4 0
ASGNI4
line 174
;174:	if ( selectedArenaSet > currentSet ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 currentSet
INDIRI4
LEI4 $79
line 175
;175:		return;
ADDRGP4 $77
JUMPV
LABELV $79
line 178
;176:	}
;177:
;178:	Q_strncpyz( bots, Info_ValueForKey( levelMenuInfo.selectedArenaInfo, "bots" ), sizeof(bots) );
ADDRGP4 levelMenuInfo+1948
INDIRP4
ARGP4
ADDRGP4 $82
ARGP4
ADDRLP4 1036
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 12
ARGP4
ADDRLP4 1036
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 180
;179:
;180:	p = &bots[0];
ADDRLP4 0
ADDRLP4 12
ASGNP4
ADDRGP4 $84
JUMPV
line 181
;181:	while( *p && levelMenuInfo.numBots < 7 ) {
LABELV $87
line 183
;182:		//skip spaces
;183:		while( *p && *p == ' ' ) {
line 184
;184:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 185
;185:		}
LABELV $88
line 183
ADDRLP4 1040
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $90
ADDRLP4 1040
INDIRI4
CNSTI4 32
EQI4 $87
LABELV $90
line 186
;186:		if( !p ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $91
line 187
;187:			break;
ADDRGP4 $85
JUMPV
LABELV $91
line 191
;188:		}
;189:
;190:		// mark start of bot name
;191:		bot = p;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $94
JUMPV
LABELV $93
line 194
;192:
;193:		// skip until space of null
;194:		while( *p && *p != ' ' ) {
line 195
;195:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 196
;196:		}
LABELV $94
line 194
ADDRLP4 1044
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $96
ADDRLP4 1044
INDIRI4
CNSTI4 32
NEI4 $93
LABELV $96
line 197
;197:		if( *p ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $97
line 198
;198:			*p++ = 0;
ADDRLP4 1048
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1048
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1048
INDIRP4
CNSTI1 0
ASGNI1
line 199
;199:		}
LABELV $97
line 201
;200:
;201:		botInfo = UI_GetBotInfoByName( bot );
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1048
ADDRGP4 UI_GetBotInfoByName
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 1048
INDIRP4
ASGNP4
line 202
;202:		if( botInfo ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $99
line 203
;203:			levelMenuInfo.botPics[levelMenuInfo.numBots] = PlayerIconHandle( Info_ValueForKey( botInfo, "model" ) );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $103
ARGP4
ADDRLP4 1052
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 PlayerIconHandle
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2512
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2516
ADDP4
ADDRLP4 1056
INDIRI4
ASGNI4
line 204
;204:			Q_strncpyz( levelMenuInfo.botNames[levelMenuInfo.numBots], Info_ValueForKey( botInfo, "name" ), 10 );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $106
ARGP4
ADDRLP4 1060
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1064
CNSTI4 10
ASGNI4
ADDRLP4 1064
INDIRI4
ADDRGP4 levelMenuInfo+2512
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+2544
ADDP4
ARGP4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRLP4 1064
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 205
;205:		}
ADDRGP4 $100
JUMPV
LABELV $99
line 206
;206:		else {
line 207
;207:			levelMenuInfo.botPics[levelMenuInfo.numBots] = 0;
ADDRGP4 levelMenuInfo+2512
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2516
ADDP4
CNSTI4 0
ASGNI4
line 208
;208:			Q_strncpyz( levelMenuInfo.botNames[levelMenuInfo.numBots], bot, 10 );
ADDRLP4 1052
CNSTI4 10
ASGNI4
ADDRLP4 1052
INDIRI4
ADDRGP4 levelMenuInfo+2512
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+2544
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1052
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 209
;209:		}
LABELV $100
line 210
;210:		Q_CleanStr( levelMenuInfo.botNames[levelMenuInfo.numBots] );
CNSTI4 10
ADDRGP4 levelMenuInfo+2512
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+2544
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 211
;211:		levelMenuInfo.numBots++;
ADDRLP4 1052
ADDRGP4 levelMenuInfo+2512
ASGNP4
ADDRLP4 1052
INDIRP4
ADDRLP4 1052
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 212
;212:	}
LABELV $84
line 181
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $114
ADDRGP4 levelMenuInfo+2512
INDIRI4
CNSTI4 7
LTI4 $88
LABELV $114
LABELV $85
line 213
;213:}
LABELV $77
endproc UI_SPLevelMenu_SetBots 1068 12
proc UI_SPLevelMenu_SetMenuArena 84 12
line 221
;214:
;215:
;216:/*
;217:=================
;218:UI_SPLevelMenu_SetMenuItems
;219:=================
;220:*/
;221:static void UI_SPLevelMenu_SetMenuArena( int n, int level, const char *arenaInfo ) {
line 224
;222:	char		map[MAX_QPATH];
;223:
;224:	Q_strncpyz( map, Info_ValueForKey( arenaInfo, "map" ), sizeof(map) );
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 $116
ARGP4
ADDRLP4 64
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 226
;225:
;226:	Q_strncpyz( levelMenuInfo.levelNames[n], map, sizeof(levelMenuInfo.levelNames[n]) );
ADDRFP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 levelMenuInfo+2212
ADDP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 227
;227:	Q_strupr( levelMenuInfo.levelNames[n] );
ADDRFP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 levelMenuInfo+2212
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 229
;228:
;229:	UI_GetBestScore( level, &levelMenuInfo.levelScores[n], &levelMenuInfo.levelScoresSkill[n] );
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 68
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 68
INDIRI4
ADDRGP4 levelMenuInfo+2276
ADDP4
ARGP4
ADDRLP4 68
INDIRI4
ADDRGP4 levelMenuInfo+2292
ADDP4
ARGP4
ADDRGP4 UI_GetBestScore
CALLV
pop
line 230
;230:	if( levelMenuInfo.levelScores[n] > 8 ) {
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2276
ADDP4
INDIRI4
CNSTI4 8
LEI4 $122
line 231
;231:		levelMenuInfo.levelScores[n] = 8;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2276
ADDP4
CNSTI4 8
ASGNI4
line 232
;232:	}
LABELV $122
line 234
;233:
;234:	strcpy( levelMenuInfo.levelPicNames[n], va( "levelshots/%s.tga", map ) );
ADDRGP4 $127
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 72
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 levelMenuInfo+1956
ADDP4
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 235
;235:	if( !trap_R_RegisterShaderNoMip( levelMenuInfo.levelPicNames[n] ) ) {
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 levelMenuInfo+1956
ADDP4
ARGP4
ADDRLP4 76
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
NEI4 $128
line 236
;236:		strcpy( levelMenuInfo.levelPicNames[n], ART_MAP_UNKNOWN );
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 levelMenuInfo+1956
ADDP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 237
;237:	}
LABELV $128
line 238
;238:	levelMenuInfo.item_maps[n].shader = 0;
CNSTI4 88
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+68
ADDP4
CNSTI4 0
ASGNI4
line 239
;239:	if ( selectedArenaSet > currentSet ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 currentSet
INDIRI4
LEI4 $135
line 240
;240:		levelMenuInfo.item_maps[n].generic.flags |= QMF_GRAYED;
ADDRLP4 80
CNSTI4 88
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+44
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 241
;241:	}
ADDRGP4 $136
JUMPV
LABELV $135
line 242
;242:	else {
line 243
;243:		levelMenuInfo.item_maps[n].generic.flags &= ~QMF_GRAYED;
ADDRLP4 80
CNSTI4 88
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+44
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 244
;244:	}
LABELV $136
line 246
;245:
;246:	levelMenuInfo.item_maps[n].generic.flags &= ~QMF_INACTIVE;
ADDRLP4 80
CNSTI4 88
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+44
ADDP4
ASGNP4
ADDRLP4 80
INDIRP4
ADDRLP4 80
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 247
;247:}
LABELV $115
endproc UI_SPLevelMenu_SetMenuArena 84 12
proc UI_SPLevelMenu_SetMenuItems 44 12
line 249
;248:
;249:static void UI_SPLevelMenu_SetMenuItems( void ) {
line 254
;250:	int			n;
;251:	int			level;
;252:	const char	*arenaInfo;
;253:
;254:	if ( selectedArenaSet > currentSet ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 currentSet
INDIRI4
LEI4 $144
line 255
;255:		selectedArena = -1;
ADDRGP4 selectedArena
CNSTI4 -1
ASGNI4
line 256
;256:	}
ADDRGP4 $145
JUMPV
LABELV $144
line 257
;257:	else if ( selectedArena == -1 ) {
ADDRGP4 selectedArena
INDIRI4
CNSTI4 -1
NEI4 $146
line 258
;258:		selectedArena = 0;
ADDRGP4 selectedArena
CNSTI4 0
ASGNI4
line 259
;259:	}
LABELV $146
LABELV $145
line 261
;260:
;261:	if( selectedArenaSet == trainingTier || selectedArenaSet == finalTier ) {
ADDRLP4 12
ADDRGP4 selectedArenaSet
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 trainingTier
INDIRI4
EQI4 $150
ADDRLP4 12
INDIRI4
ADDRGP4 finalTier
INDIRI4
NEI4 $148
LABELV $150
line 262
;262:		selectedArena = 0;
ADDRGP4 selectedArena
CNSTI4 0
ASGNI4
line 263
;263:	}
LABELV $148
line 265
;264:
;265:	if( selectedArena != -1 ) {
ADDRGP4 selectedArena
INDIRI4
CNSTI4 -1
EQI4 $151
line 266
;266:		trap_Cvar_SetValue( "ui_spSelection", selectedArenaSet * ARENAS_PER_TIER + selectedArena );
ADDRGP4 $153
ARGP4
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 selectedArena
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 267
;267:	}
LABELV $151
line 269
;268:
;269:	if( selectedArenaSet == trainingTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 trainingTier
INDIRI4
NEI4 $154
line 270
;270:		arenaInfo = UI_GetSpecialArenaInfo( "training" );
ADDRGP4 $156
ARGP4
ADDRLP4 16
ADDRGP4 UI_GetSpecialArenaInfo
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 271
;271:		level = atoi( Info_ValueForKey( arenaInfo, "num" ) );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $157
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
ADDRLP4 4
ADDRLP4 24
INDIRI4
ASGNI4
line 272
;272:		UI_SPLevelMenu_SetMenuArena( 0, level, arenaInfo );
CNSTI4 0
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_SPLevelMenu_SetMenuArena
CALLV
pop
line 273
;273:		levelMenuInfo.selectedArenaInfo = arenaInfo;
ADDRGP4 levelMenuInfo+1948
ADDRLP4 8
INDIRP4
ASGNP4
line 275
;274:
;275:		levelMenuInfo.item_maps[0].generic.x = 256;
ADDRGP4 levelMenuInfo+448+12
CNSTI4 256
ASGNI4
line 276
;276:		Bitmap_Init( &levelMenuInfo.item_maps[0] );
ADDRGP4 levelMenuInfo+448
ARGP4
ADDRGP4 Bitmap_Init
CALLV
pop
line 277
;277:		levelMenuInfo.item_maps[0].generic.bottom += 32;
ADDRLP4 28
ADDRGP4 levelMenuInfo+448+32
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 32
ADDI4
ASGNI4
line 278
;278:		levelMenuInfo.numMaps = 1;
ADDRGP4 levelMenuInfo+1952
CNSTI4 1
ASGNI4
line 280
;279:
;280:		levelMenuInfo.item_maps[1].generic.flags |= QMF_INACTIVE;
ADDRLP4 32
ADDRGP4 levelMenuInfo+448+88+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 281
;281:		levelMenuInfo.item_maps[2].generic.flags |= QMF_INACTIVE;
ADDRLP4 36
ADDRGP4 levelMenuInfo+448+176+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 282
;282:		levelMenuInfo.item_maps[3].generic.flags |= QMF_INACTIVE;
ADDRLP4 40
ADDRGP4 levelMenuInfo+448+264+44
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 283
;283:		levelMenuInfo.levelPicNames[1][0] = 0;
ADDRGP4 levelMenuInfo+1956+64
CNSTI1 0
ASGNI1
line 284
;284:		levelMenuInfo.levelPicNames[2][0] = 0;
ADDRGP4 levelMenuInfo+1956+128
CNSTI1 0
ASGNI1
line 285
;285:		levelMenuInfo.levelPicNames[3][0] = 0;
ADDRGP4 levelMenuInfo+1956+192
CNSTI1 0
ASGNI1
line 286
;286:		levelMenuInfo.item_maps[1].shader = 0;
ADDRGP4 levelMenuInfo+448+88+68
CNSTI4 0
ASGNI4
line 287
;287:		levelMenuInfo.item_maps[2].shader = 0;
ADDRGP4 levelMenuInfo+448+176+68
CNSTI4 0
ASGNI4
line 288
;288:		levelMenuInfo.item_maps[3].shader = 0;
ADDRGP4 levelMenuInfo+448+264+68
CNSTI4 0
ASGNI4
line 289
;289:	}
ADDRGP4 $155
JUMPV
LABELV $154
line 290
;290:	else if( selectedArenaSet == finalTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 finalTier
INDIRI4
NEI4 $189
line 291
;291:		arenaInfo = UI_GetSpecialArenaInfo( "final" );
ADDRGP4 $191
ARGP4
ADDRLP4 16
ADDRGP4 UI_GetSpecialArenaInfo
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 292
;292:		level = atoi( Info_ValueForKey( arenaInfo, "num" ) );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $157
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
ADDRLP4 4
ADDRLP4 24
INDIRI4
ASGNI4
line 293
;293:		UI_SPLevelMenu_SetMenuArena( 0, level, arenaInfo );
CNSTI4 0
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_SPLevelMenu_SetMenuArena
CALLV
pop
line 294
;294:		levelMenuInfo.selectedArenaInfo = arenaInfo;
ADDRGP4 levelMenuInfo+1948
ADDRLP4 8
INDIRP4
ASGNP4
line 296
;295:
;296:		levelMenuInfo.item_maps[0].generic.x = 256;
ADDRGP4 levelMenuInfo+448+12
CNSTI4 256
ASGNI4
line 297
;297:		Bitmap_Init( &levelMenuInfo.item_maps[0] );
ADDRGP4 levelMenuInfo+448
ARGP4
ADDRGP4 Bitmap_Init
CALLV
pop
line 298
;298:		levelMenuInfo.item_maps[0].generic.bottom += 32;
ADDRLP4 28
ADDRGP4 levelMenuInfo+448+32
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 32
ADDI4
ASGNI4
line 299
;299:		levelMenuInfo.numMaps = 1;
ADDRGP4 levelMenuInfo+1952
CNSTI4 1
ASGNI4
line 301
;300:
;301:		levelMenuInfo.item_maps[1].generic.flags |= QMF_INACTIVE;
ADDRLP4 32
ADDRGP4 levelMenuInfo+448+88+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 302
;302:		levelMenuInfo.item_maps[2].generic.flags |= QMF_INACTIVE;
ADDRLP4 36
ADDRGP4 levelMenuInfo+448+176+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 303
;303:		levelMenuInfo.item_maps[3].generic.flags |= QMF_INACTIVE;
ADDRLP4 40
ADDRGP4 levelMenuInfo+448+264+44
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 304
;304:		levelMenuInfo.levelPicNames[1][0] = 0;
ADDRGP4 levelMenuInfo+1956+64
CNSTI1 0
ASGNI1
line 305
;305:		levelMenuInfo.levelPicNames[2][0] = 0;
ADDRGP4 levelMenuInfo+1956+128
CNSTI1 0
ASGNI1
line 306
;306:		levelMenuInfo.levelPicNames[3][0] = 0;
ADDRGP4 levelMenuInfo+1956+192
CNSTI1 0
ASGNI1
line 307
;307:		levelMenuInfo.item_maps[1].shader = 0;
ADDRGP4 levelMenuInfo+448+88+68
CNSTI4 0
ASGNI4
line 308
;308:		levelMenuInfo.item_maps[2].shader = 0;
ADDRGP4 levelMenuInfo+448+176+68
CNSTI4 0
ASGNI4
line 309
;309:		levelMenuInfo.item_maps[3].shader = 0;
ADDRGP4 levelMenuInfo+448+264+68
CNSTI4 0
ASGNI4
line 310
;310:	}
ADDRGP4 $190
JUMPV
LABELV $189
line 311
;311:	else {
line 312
;312:		levelMenuInfo.item_maps[0].generic.x = 46;
ADDRGP4 levelMenuInfo+448+12
CNSTI4 46
ASGNI4
line 313
;313:		Bitmap_Init( &levelMenuInfo.item_maps[0] );
ADDRGP4 levelMenuInfo+448
ARGP4
ADDRGP4 Bitmap_Init
CALLV
pop
line 314
;314:		levelMenuInfo.item_maps[0].generic.bottom += 18;
ADDRLP4 16
ADDRGP4 levelMenuInfo+448+32
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 315
;315:		levelMenuInfo.numMaps = 4;
ADDRGP4 levelMenuInfo+1952
CNSTI4 4
ASGNI4
line 317
;316:
;317:		for ( n = 0; n < 4; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $229
line 318
;318:			level = selectedArenaSet * ARENAS_PER_TIER + n;
ADDRLP4 4
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 319
;319:			arenaInfo = UI_GetArenaInfoByNumber( level );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 UI_GetArenaInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 20
INDIRP4
ASGNP4
line 320
;320:			UI_SPLevelMenu_SetMenuArena( n, level, arenaInfo );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_SPLevelMenu_SetMenuArena
CALLV
pop
line 321
;321:		}
LABELV $230
line 317
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $229
line 323
;322:
;323:		if( selectedArena != -1 ) {
ADDRGP4 selectedArena
INDIRI4
CNSTI4 -1
EQI4 $233
line 324
;324:			levelMenuInfo.selectedArenaInfo = UI_GetArenaInfoByNumber( selectedArenaSet * ARENAS_PER_TIER + selectedArena );
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 selectedArena
INDIRI4
ADDI4
ARGI4
ADDRLP4 20
ADDRGP4 UI_GetArenaInfoByNumber
CALLP4
ASGNP4
ADDRGP4 levelMenuInfo+1948
ADDRLP4 20
INDIRP4
ASGNP4
line 325
;325:		}
LABELV $233
line 326
;326:	}
LABELV $190
LABELV $155
line 329
;327:
;328:	// enable/disable arrows when they are valid/invalid
;329:	if ( selectedArenaSet == minTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 minTier
INDIRI4
NEI4 $236
line 330
;330:		levelMenuInfo.item_leftarrow.generic.flags |= ( QMF_INACTIVE | QMF_HIDDEN );
ADDRLP4 16
ADDRGP4 levelMenuInfo+360+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 331
;331:	}
ADDRGP4 $237
JUMPV
LABELV $236
line 332
;332:	else {
line 333
;333:		levelMenuInfo.item_leftarrow.generic.flags &= ~( QMF_INACTIVE | QMF_HIDDEN );
ADDRLP4 16
ADDRGP4 levelMenuInfo+360+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 334
;334:	}
LABELV $237
line 336
;335:
;336:	if ( selectedArenaSet == maxTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 maxTier
INDIRI4
NEI4 $242
line 337
;337:		levelMenuInfo.item_rightarrow.generic.flags |= ( QMF_INACTIVE | QMF_HIDDEN );
ADDRLP4 16
ADDRGP4 levelMenuInfo+800+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 338
;338:	}
ADDRGP4 $243
JUMPV
LABELV $242
line 339
;339:	else {
line 340
;340:		levelMenuInfo.item_rightarrow.generic.flags &= ~( QMF_INACTIVE | QMF_HIDDEN );
ADDRLP4 16
ADDRGP4 levelMenuInfo+800+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 341
;341:	}
LABELV $243
line 343
;342:
;343:	UI_SPLevelMenu_SetBots();
ADDRGP4 UI_SPLevelMenu_SetBots
CALLV
pop
line 344
;344:}
LABELV $143
endproc UI_SPLevelMenu_SetMenuItems 44 12
proc UI_SPLevelMenu_ResetDraw 0 20
line 352
;345:
;346:
;347:/*
;348:=================
;349:UI_SPLevelMenu_ResetEvent
;350:=================
;351:*/
;352:static void UI_SPLevelMenu_ResetDraw( void ) {
line 353
;353:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 0, "WARNING: This resets all of the", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 356
ARGI4
ADDRGP4 $249
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 354
;354:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 1, "single player game variables.", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 383
ARGI4
ADDRGP4 $250
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 355
;355:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 2, "Do this only if you want to", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 410
ARGI4
ADDRGP4 $251
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 356
;356:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 3, "start over from the beginning.", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 437
ARGI4
ADDRGP4 $252
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 357
;357:}
LABELV $248
endproc UI_SPLevelMenu_ResetDraw 0 20
proc UI_SPLevelMenu_ResetAction 0 8
line 359
;358:
;359:static void UI_SPLevelMenu_ResetAction( qboolean result ) {
line 360
;360:	if( !result ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $254
line 361
;361:		return;
ADDRGP4 $253
JUMPV
LABELV $254
line 365
;362:	}
;363:
;364:	// clear game variables
;365:	UI_NewGame();
ADDRGP4 UI_NewGame
CALLV
pop
line 366
;366:	trap_Cvar_SetValue( "ui_spSelection", -4 );
ADDRGP4 $153
ARGP4
CNSTF4 3229614080
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 369
;367:
;368:	// make the level select menu re-initialize
;369:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 370
;370:	UI_SPLevelMenu();
ADDRGP4 UI_SPLevelMenu
CALLV
pop
line 371
;371:}
LABELV $253
endproc UI_SPLevelMenu_ResetAction 0 8
proc UI_SPLevelMenu_ResetEvent 0 12
line 374
;372:
;373:static void UI_SPLevelMenu_ResetEvent( void* ptr, int event )
;374:{
line 375
;375:	if (event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $257
line 376
;376:		return;
ADDRGP4 $256
JUMPV
LABELV $257
line 379
;377:	}
;378:
;379:	UI_ConfirmMenu( "RESET GAME?", UI_SPLevelMenu_ResetDraw, UI_SPLevelMenu_ResetAction );
ADDRGP4 $259
ARGP4
ADDRGP4 UI_SPLevelMenu_ResetDraw
ARGP4
ADDRGP4 UI_SPLevelMenu_ResetAction
ARGP4
ADDRGP4 UI_ConfirmMenu
CALLV
pop
line 380
;380:}
LABELV $256
endproc UI_SPLevelMenu_ResetEvent 0 12
proc UI_SPLevelMenu_LevelEvent 8 8
line 388
;381:
;382:
;383:/*
;384:=================
;385:UI_SPLevelMenu_LevelEvent
;386:=================
;387:*/
;388:static void UI_SPLevelMenu_LevelEvent( void* ptr, int notification ) {
line 389
;389:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $261
line 390
;390:		return;
ADDRGP4 $260
JUMPV
LABELV $261
line 393
;391:	}
;392:
;393:	if ( selectedArenaSet == trainingTier || selectedArenaSet == finalTier ) {
ADDRLP4 0
ADDRGP4 selectedArenaSet
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRGP4 trainingTier
INDIRI4
EQI4 $265
ADDRLP4 0
INDIRI4
ADDRGP4 finalTier
INDIRI4
NEI4 $263
LABELV $265
line 394
;394:		return;
ADDRGP4 $260
JUMPV
LABELV $263
line 397
;395:	}
;396:
;397:	selectedArena = ((menucommon_s*)ptr)->id - ID_PICTURE0;
ADDRGP4 selectedArena
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 11
SUBI4
ASGNI4
line 398
;398:	levelMenuInfo.selectedArenaInfo = UI_GetArenaInfoByNumber( selectedArenaSet * ARENAS_PER_TIER + selectedArena );
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 selectedArena
INDIRI4
ADDI4
ARGI4
ADDRLP4 4
ADDRGP4 UI_GetArenaInfoByNumber
CALLP4
ASGNP4
ADDRGP4 levelMenuInfo+1948
ADDRLP4 4
INDIRP4
ASGNP4
line 399
;399:	UI_SPLevelMenu_SetBots();
ADDRGP4 UI_SPLevelMenu_SetBots
CALLV
pop
line 401
;400:
;401:	trap_Cvar_SetValue( "ui_spSelection", selectedArenaSet * ARENAS_PER_TIER + selectedArena );
ADDRGP4 $153
ARGP4
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 selectedArena
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 402
;402:}
LABELV $260
endproc UI_SPLevelMenu_LevelEvent 8 8
proc UI_SPLevelMenu_LeftArrowEvent 4 0
line 410
;403:
;404:
;405:/*
;406:=================
;407:UI_SPLevelMenu_LeftArrowEvent
;408:=================
;409:*/
;410:static void UI_SPLevelMenu_LeftArrowEvent( void* ptr, int notification ) {
line 411
;411:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $268
line 412
;412:		return;
ADDRGP4 $267
JUMPV
LABELV $268
line 415
;413:	}
;414:
;415:	if ( selectedArenaSet == minTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 minTier
INDIRI4
NEI4 $270
line 416
;416:		return;
ADDRGP4 $267
JUMPV
LABELV $270
line 419
;417:	}
;418:
;419:	selectedArenaSet--;
ADDRLP4 0
ADDRGP4 selectedArenaSet
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 420
;420:	UI_SPLevelMenu_SetMenuItems();
ADDRGP4 UI_SPLevelMenu_SetMenuItems
CALLV
pop
line 421
;421:}
LABELV $267
endproc UI_SPLevelMenu_LeftArrowEvent 4 0
proc UI_SPLevelMenu_RightArrowEvent 4 0
line 429
;422:
;423:
;424:/*
;425:=================
;426:UI_SPLevelMenu_RightArrowEvent
;427:=================
;428:*/
;429:static void UI_SPLevelMenu_RightArrowEvent( void* ptr, int notification ) {
line 430
;430:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $273
line 431
;431:		return;
ADDRGP4 $272
JUMPV
LABELV $273
line 434
;432:	}
;433:
;434:	if ( selectedArenaSet == maxTier ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 maxTier
INDIRI4
NEI4 $275
line 435
;435:		return;
ADDRGP4 $272
JUMPV
LABELV $275
line 438
;436:	}
;437:
;438:	selectedArenaSet++;
ADDRLP4 0
ADDRGP4 selectedArenaSet
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 439
;439:	UI_SPLevelMenu_SetMenuItems();
ADDRGP4 UI_SPLevelMenu_SetMenuItems
CALLV
pop
line 440
;440:}
LABELV $272
endproc UI_SPLevelMenu_RightArrowEvent 4 0
proc UI_SPLevelMenu_PlayerEvent 0 0
line 448
;441:
;442:
;443:/*
;444:=================
;445:UI_SPLevelMenu_PlayerEvent
;446:=================
;447:*/
;448:static void UI_SPLevelMenu_PlayerEvent( void* ptr, int notification ) {
line 449
;449:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $278
line 450
;450:		return;
ADDRGP4 $277
JUMPV
LABELV $278
line 453
;451:	}
;452:
;453:	UI_PlayerSettingsMenu();
ADDRGP4 UI_PlayerSettingsMenu
CALLV
pop
line 454
;454:}
LABELV $277
endproc UI_SPLevelMenu_PlayerEvent 0 0
proc UI_SPLevelMenu_AwardEvent 4 8
line 462
;455:
;456:
;457:/*
;458:=================
;459:UI_SPLevelMenu_AwardEvent
;460:=================
;461:*/
;462:static void UI_SPLevelMenu_AwardEvent( void* ptr, int notification ) {
line 465
;463:	int		n;
;464:
;465:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $281
line 466
;466:		return;
ADDRGP4 $280
JUMPV
LABELV $281
line 469
;467:	}
;468:
;469:	n = ((menucommon_s*)ptr)->id - ID_AWARD1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 17
SUBI4
ASGNI4
line 470
;470:	trap_S_StartLocalSound( levelMenuInfo.awardSounds[n], CHAN_ANNOUNCER );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2488
ADDP4
INDIRI4
ARGI4
CNSTI4 7
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
line 471
;471:}
LABELV $280
endproc UI_SPLevelMenu_AwardEvent 4 8
proc UI_SPLevelMenu_NextEvent 0 4
line 479
;472:
;473:
;474:/*
;475:=================
;476:UI_SPLevelMenu_NextEvent
;477:=================
;478:*/
;479:static void UI_SPLevelMenu_NextEvent( void* ptr, int notification ) {
line 480
;480:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $285
line 481
;481:		return;
ADDRGP4 $284
JUMPV
LABELV $285
line 484
;482:	}
;483:
;484:	if ( selectedArenaSet > currentSet ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 currentSet
INDIRI4
LEI4 $287
line 485
;485:		return;
ADDRGP4 $284
JUMPV
LABELV $287
line 488
;486:	}
;487:
;488:	if ( selectedArena == -1 ) {
ADDRGP4 selectedArena
INDIRI4
CNSTI4 -1
NEI4 $289
line 489
;489:		selectedArena = 0;
ADDRGP4 selectedArena
CNSTI4 0
ASGNI4
line 490
;490:	}
LABELV $289
line 492
;491:
;492:	UI_SPSkillMenu( levelMenuInfo.selectedArenaInfo );
ADDRGP4 levelMenuInfo+1948
INDIRP4
ARGP4
ADDRGP4 UI_SPSkillMenu
CALLV
pop
line 493
;493:}
LABELV $284
endproc UI_SPLevelMenu_NextEvent 0 4
proc UI_SPLevelMenu_BackEvent 0 0
line 501
;494:
;495:
;496:/*
;497:=================
;498:UI_SPLevelMenu_BackEvent
;499:=================
;500:*/
;501:static void UI_SPLevelMenu_BackEvent( void* ptr, int notification ) {
line 502
;502:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $293
line 503
;503:		return;
ADDRGP4 $292
JUMPV
LABELV $293
line 506
;504:	}
;505:
;506:	if ( selectedArena == -1 ) {
ADDRGP4 selectedArena
INDIRI4
CNSTI4 -1
NEI4 $295
line 507
;507:		selectedArena = 0;
ADDRGP4 selectedArena
CNSTI4 0
ASGNI4
line 508
;508:	}
LABELV $295
line 510
;509:
;510:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 511
;511:}
LABELV $292
endproc UI_SPLevelMenu_BackEvent 0 0
proc UI_SPLevelMenu_CustomEvent 0 4
line 519
;512:
;513:
;514:/*
;515:=================
;516:UI_SPLevelMenu_CustomEvent
;517:=================
;518:*/
;519:static void UI_SPLevelMenu_CustomEvent( void* ptr, int notification ) {
line 520
;520:	if (notification != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $298
line 521
;521:		return;
ADDRGP4 $297
JUMPV
LABELV $298
line 524
;522:	}
;523:
;524:	UI_StartServerMenu( qfalse );
CNSTI4 0
ARGI4
ADDRGP4 UI_StartServerMenu
CALLV
pop
line 525
;525:}
LABELV $297
endproc UI_SPLevelMenu_CustomEvent 0 4
proc UI_SPLevelMenu_MenuDraw 1152 20
line 535
;526:
;527:
;528:/*
;529:=================
;530:UI_SPLevelMenu_MenuDraw
;531:=================
;532:*/
;533:#define LEVEL_DESC_LEFT_MARGIN		332
;534:
;535:static void UI_SPLevelMenu_MenuDraw( void ) {
line 545
;536:	int				n, i;
;537:	int				x, y;
;538:	vec4_t			color;
;539:	int				level;
;540://	int				fraglimit;
;541:	int				pad;
;542:	char			buf[MAX_INFO_VALUE];
;543:	char			string[64];
;544:
;545:	if(	levelMenuInfo.reinit ) {
ADDRGP4 levelMenuInfo+1944
INDIRI4
CNSTI4 0
EQI4 $301
line 546
;546:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 547
;547:		UI_SPLevelMenu();
ADDRGP4 UI_SPLevelMenu
CALLV
pop
line 548
;548:		return;
ADDRGP4 $300
JUMPV
LABELV $301
line 552
;549:	}
;550:
;551:	// draw player name
;552:	trap_Cvar_VariableStringBuffer( "name", string, 32 );
ADDRGP4 $106
ARGP4
ADDRLP4 16
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 553
;553:	Q_CleanStr( string );
ADDRLP4 16
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 554
;554:	UI_DrawProportionalString( 320, PLAYER_Y, string, UI_CENTER|UI_SMALLFONT, color_orange );
CNSTI4 320
ARGI4
CNSTI4 314
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 557
;555:
;556:	// check for model changes
;557:	trap_Cvar_VariableStringBuffer( "model", buf, sizeof(buf) );
ADDRGP4 $103
ARGP4
ADDRLP4 104
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 558
;558:	if( Q_stricmp( buf, levelMenuInfo.playerModel ) != 0 ) {
ADDRLP4 104
ARGP4
ADDRGP4 levelMenuInfo+2336
ARGP4
ADDRLP4 1128
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1128
INDIRI4
CNSTI4 0
EQI4 $304
line 559
;559:		Q_strncpyz( levelMenuInfo.playerModel, buf, sizeof(levelMenuInfo.playerModel) );
ADDRGP4 levelMenuInfo+2336
ARGP4
ADDRLP4 104
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 560
;560:		PlayerIcon( levelMenuInfo.playerModel, levelMenuInfo.playerPicName, sizeof(levelMenuInfo.playerPicName) );
ADDRGP4 levelMenuInfo+2336
ARGP4
ADDRGP4 levelMenuInfo+2400
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 PlayerIcon
CALLV
pop
line 561
;561:		levelMenuInfo.item_player.shader = 0;
ADDRGP4 levelMenuInfo+888+68
CNSTI4 0
ASGNI4
line 562
;562:	}
LABELV $304
line 565
;563:
;564:	// standard menu drawing
;565:	Menu_Draw( &levelMenuInfo.menu );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 568
;566:
;567:	// draw player award levels
;568:	y = AWARDS_Y;
ADDRLP4 8
CNSTI4 340
ASGNI4
line 569
;569:	i = 0;
ADDRLP4 80
CNSTI4 0
ASGNI4
line 570
;570:	for( n = 0; n < 6; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $314
line 571
;571:		level = levelMenuInfo.awardLevels[n];
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2464
ADDP4
INDIRI4
ASGNI4
line 572
;572:		if( level > 0 ) {
ADDRLP4 12
INDIRI4
CNSTI4 0
LEI4 $319
line 573
;573:			if( i & 1 ) {
ADDRLP4 80
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $321
line 574
;574:				x = 224 - (i - 1 ) / 2 * (48 + 16);
ADDRLP4 4
CNSTI4 224
ADDRLP4 80
INDIRI4
CNSTI4 1
SUBI4
CNSTI4 2
DIVI4
CNSTI4 6
LSHI4
SUBI4
ASGNI4
line 575
;575:			}
ADDRGP4 $322
JUMPV
LABELV $321
line 576
;576:			else {
line 577
;577:				x = 368 + i / 2 * (48 + 16);
ADDRLP4 4
ADDRLP4 80
INDIRI4
CNSTI4 2
DIVI4
CNSTI4 6
LSHI4
CNSTI4 368
ADDI4
ASGNI4
line 578
;578:			}
LABELV $322
line 579
;579:			i++;
ADDRLP4 80
ADDRLP4 80
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 581
;580:
;581:			if( level == 1 ) {
ADDRLP4 12
INDIRI4
CNSTI4 1
NEI4 $323
line 582
;582:				continue;
ADDRGP4 $315
JUMPV
LABELV $323
line 585
;583:			}
;584:
;585:			if( level >= 1000000 ) {
ADDRLP4 12
INDIRI4
CNSTI4 1000000
LTI4 $325
line 586
;586:				Com_sprintf( string, sizeof(string), "%im", level / 1000000 );
ADDRLP4 16
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $327
ARGP4
ADDRLP4 12
INDIRI4
CNSTI4 1000000
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 587
;587:			}
ADDRGP4 $326
JUMPV
LABELV $325
line 588
;588:			else if( level >= 1000 ) {
ADDRLP4 12
INDIRI4
CNSTI4 1000
LTI4 $328
line 589
;589:				Com_sprintf( string, sizeof(string), "%ik", level / 1000 );
ADDRLP4 16
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $330
ARGP4
ADDRLP4 12
INDIRI4
CNSTI4 1000
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 590
;590:			}
ADDRGP4 $329
JUMPV
LABELV $328
line 591
;591:			else {
line 592
;592:				Com_sprintf( string, sizeof(string), "%i", level );
ADDRLP4 16
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $331
ARGP4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 593
;593:			}
LABELV $329
LABELV $326
line 595
;594:
;595:			UI_DrawString( x + 24, y + 48, string, UI_CENTER, color_yellow );
ADDRLP4 4
INDIRI4
CNSTI4 24
ADDI4
ARGI4
ADDRLP4 8
INDIRI4
CNSTI4 48
ADDI4
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 596
;596:		}
LABELV $319
line 597
;597:	}
LABELV $315
line 570
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LTI4 $314
line 599
;598:
;599:	UI_DrawProportionalString( 18, 38, va( "Tier %i", selectedArenaSet + 1 ), UI_LEFT|UI_SMALLFONT, color_orange );
ADDRGP4 $332
ARGP4
ADDRGP4 selectedArenaSet
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 1132
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 18
ARGI4
CNSTI4 38
ARGI4
ADDRLP4 1132
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 601
;600:
;601:	for ( n = 0; n < levelMenuInfo.numMaps; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $336
JUMPV
LABELV $333
line 602
;602:		x = levelMenuInfo.item_maps[n].generic.x;
ADDRLP4 4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+12
ADDP4
INDIRI4
ASGNI4
line 603
;603:		y = levelMenuInfo.item_maps[n].generic.y;
ADDRLP4 8
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+16
ADDP4
INDIRI4
ASGNI4
line 604
;604:		UI_FillRect( x, y + 96, 128, 18, color_black );
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CNSTI4 96
ADDI4
CVIF4 4
ARGF4
CNSTF4 1124073472
ARGF4
CNSTF4 1099956224
ARGF4
ADDRGP4 color_black
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 605
;605:	}
LABELV $334
line 601
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $336
ADDRLP4 0
INDIRI4
ADDRGP4 levelMenuInfo+1952
INDIRI4
LTI4 $333
line 607
;606:
;607:	if ( selectedArenaSet > currentSet ) {
ADDRGP4 selectedArenaSet
INDIRI4
ADDRGP4 currentSet
INDIRI4
LEI4 $342
line 608
;608:		UI_DrawProportionalString( 320, 216, "ACCESS DENIED", UI_CENTER|UI_BIGFONT, color_red );
CNSTI4 320
ARGI4
CNSTI4 216
ARGI4
ADDRGP4 $344
ARGP4
CNSTI4 33
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 609
;609:		return;
ADDRGP4 $300
JUMPV
LABELV $342
line 613
;610:	}
;611:
;612:	// show levelshots for levels of current tier
;613:	Vector4Copy( color_white, color );
ADDRLP4 88
ADDRGP4 color_white
INDIRF4
ASGNF4
ADDRLP4 88+4
ADDRGP4 color_white+4
INDIRF4
ASGNF4
ADDRLP4 88+8
ADDRGP4 color_white+8
INDIRF4
ASGNF4
ADDRLP4 88+12
ADDRGP4 color_white+12
INDIRF4
ASGNF4
line 614
;614:	color[3] = 0.5+0.5*sin(uis.realtime/PULSE_DIVISOR);
ADDRGP4 uis+4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 1136
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 88+12
CNSTF4 1056964608
ADDRLP4 1136
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ASGNF4
line 615
;615:	for ( n = 0; n < levelMenuInfo.numMaps; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $356
JUMPV
LABELV $353
line 616
;616:		x = levelMenuInfo.item_maps[n].generic.x;
ADDRLP4 4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+12
ADDP4
INDIRI4
ASGNI4
line 617
;617:		y = levelMenuInfo.item_maps[n].generic.y;
ADDRLP4 8
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448+16
ADDP4
INDIRI4
ASGNI4
line 619
;618:
;619:		UI_DrawString( x + 64, y + 96, levelMenuInfo.levelNames[n], UI_CENTER|UI_SMALLFONT, color_orange );
ADDRLP4 4
INDIRI4
CNSTI4 64
ADDI4
ARGI4
ADDRLP4 8
INDIRI4
CNSTI4 96
ADDI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 levelMenuInfo+2212
ADDP4
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 621
;620:
;621:		if( levelMenuInfo.levelScores[n] == 1 ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2276
ADDP4
INDIRI4
CNSTI4 1
NEI4 $363
line 622
;622:			UI_DrawHandlePic( x, y, 128, 96, levelMenuInfo.levelCompletePic[levelMenuInfo.levelScoresSkill[n] - 1] ); 
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
CNSTF4 1124073472
ARGF4
CNSTF4 1119879168
ARGF4
ADDRLP4 1140
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 1140
INDIRI4
LSHI4
ADDRGP4 levelMenuInfo+2292
ADDP4
INDIRI4
ADDRLP4 1140
INDIRI4
LSHI4
ADDRGP4 levelMenuInfo+2316-4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 623
;623:		}
LABELV $363
line 625
;624:
;625:		if ( n == selectedArena ) {
ADDRLP4 0
INDIRI4
ADDRGP4 selectedArena
INDIRI4
NEI4 $369
line 626
;626:			if( Menu_ItemAtCursor( &levelMenuInfo.menu ) == &levelMenuInfo.item_maps[n] ) {
ADDRGP4 levelMenuInfo
ARGP4
ADDRLP4 1140
ADDRGP4 Menu_ItemAtCursor
CALLP4
ASGNP4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448
ADDP4
CVPU4 4
ADDRLP4 1140
INDIRP4
CVPU4 4
NEU4 $371
line 627
;627:				trap_R_SetColor( color );
ADDRLP4 88
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 628
;628:			}
LABELV $371
line 629
;629:			UI_DrawHandlePic( x-1, y-1, 130, 130 - 14, levelMenuInfo.levelSelectedPic ); 
ADDRLP4 1144
CNSTI4 1
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 1144
INDIRI4
SUBI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
ADDRLP4 1144
INDIRI4
SUBI4
CVIF4 4
ARGF4
CNSTF4 1124204544
ARGF4
CNSTF4 1122500608
ARGF4
ADDRGP4 levelMenuInfo+2308
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 630
;630:			trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 631
;631:		}
ADDRGP4 $370
JUMPV
LABELV $369
line 632
;632:		else if( Menu_ItemAtCursor( &levelMenuInfo.menu ) == &levelMenuInfo.item_maps[n] ) {
ADDRGP4 levelMenuInfo
ARGP4
ADDRLP4 1140
ADDRGP4 Menu_ItemAtCursor
CALLP4
ASGNP4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+448
ADDP4
CVPU4 4
ADDRLP4 1140
INDIRP4
CVPU4 4
NEU4 $375
line 633
;633:			trap_R_SetColor( color );
ADDRLP4 88
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 634
;634:			UI_DrawHandlePic( x-31, y-30, 256, 256-27, levelMenuInfo.levelFocusPic); 
ADDRLP4 4
INDIRI4
CNSTI4 31
SUBI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CNSTI4 30
SUBI4
CVIF4 4
ARGF4
CNSTF4 1132462080
ARGF4
CNSTF4 1130692608
ARGF4
ADDRGP4 levelMenuInfo+2312
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 635
;635:			trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 636
;636:		}
LABELV $375
LABELV $370
line 637
;637:	}
LABELV $354
line 615
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $356
ADDRLP4 0
INDIRI4
ADDRGP4 levelMenuInfo+1952
INDIRI4
LTI4 $353
line 640
;638:
;639:	// show map name and long name of selected level
;640:	y = 192;
ADDRLP4 8
CNSTI4 192
ASGNI4
line 641
;641:	Q_strncpyz( buf, Info_ValueForKey( levelMenuInfo.selectedArenaInfo, "map" ), 20 );
ADDRGP4 levelMenuInfo+1948
INDIRP4
ARGP4
ADDRGP4 $116
ARGP4
ADDRLP4 1140
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 104
ARGP4
ADDRLP4 1140
INDIRP4
ARGP4
CNSTI4 20
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 642
;642:	Q_strupr( buf );
ADDRLP4 104
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 643
;643:	Com_sprintf( string, sizeof(string), "%s: %s", buf, Info_ValueForKey( levelMenuInfo.selectedArenaInfo, "longname" ) );
ADDRGP4 levelMenuInfo+1948
INDIRP4
ARGP4
ADDRGP4 $382
ARGP4
ADDRLP4 1144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $380
ARGP4
ADDRLP4 104
ARGP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 644
;644:	UI_DrawProportionalString( 320, y, string, UI_CENTER|UI_SMALLFONT, color_orange );
CNSTI4 320
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 16
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 650
;645:
;646://	fraglimit = atoi( Info_ValueForKey( levelMenuInfo.selectedArenaInfo, "fraglimit" ) );
;647://	UI_DrawString( 18, 212, va("Frags %i", fraglimit) , UI_LEFT|UI_SMALLFONT, color_orange );
;648:
;649:	// draw bot opponents
;650:	y += 24;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 24
ADDI4
ASGNI4
line 651
;651:	pad = (7 - levelMenuInfo.numBots) * (64 + 26) / 2;
ADDRLP4 84
CNSTI4 90
CNSTI4 7
ADDRGP4 levelMenuInfo+2512
INDIRI4
SUBI4
MULI4
CNSTI4 2
DIVI4
ASGNI4
line 652
;652:	for( n = 0; n < levelMenuInfo.numBots; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $387
JUMPV
LABELV $384
line 653
;653:		x = 18 + pad + (64 + 26) * n;
ADDRLP4 4
ADDRLP4 84
INDIRI4
CNSTI4 18
ADDI4
CNSTI4 90
ADDRLP4 0
INDIRI4
MULI4
ADDI4
ASGNI4
line 654
;654:		if( levelMenuInfo.botPics[n] ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2516
ADDP4
INDIRI4
CNSTI4 0
EQI4 $389
line 655
;655:			UI_DrawHandlePic( x, y, 64, 64, levelMenuInfo.botPics[n]);
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 1148
CNSTF4 1115684864
ASGNF4
ADDRLP4 1148
INDIRF4
ARGF4
ADDRLP4 1148
INDIRF4
ARGF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2516
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 656
;656:		}
ADDRGP4 $390
JUMPV
LABELV $389
line 657
;657:		else {
line 658
;658:			UI_FillRect( x, y, 64, 64, color_black );
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 1148
CNSTF4 1115684864
ASGNF4
ADDRLP4 1148
INDIRF4
ARGF4
ADDRLP4 1148
INDIRF4
ARGF4
ADDRGP4 color_black
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 659
;659:			UI_DrawProportionalString( x+22, y+18, "?", UI_BIGFONT, color_orange );
ADDRLP4 4
INDIRI4
CNSTI4 22
ADDI4
ARGI4
ADDRLP4 8
INDIRI4
CNSTI4 18
ADDI4
ARGI4
ADDRGP4 $393
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 660
;660:		}
LABELV $390
line 661
;661:		UI_DrawString( x, y + 64, levelMenuInfo.botNames[n], UI_SMALLFONT|UI_LEFT, color_orange );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
CNSTI4 64
ADDI4
ARGI4
CNSTI4 10
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+2544
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 662
;662:	}
LABELV $385
line 652
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $387
ADDRLP4 0
INDIRI4
ADDRGP4 levelMenuInfo+2512
INDIRI4
LTI4 $384
line 663
;663:}
LABELV $300
endproc UI_SPLevelMenu_MenuDraw 1152 20
export UI_SPLevelMenu_Cache
proc UI_SPLevelMenu_Cache 32 8
line 671
;664:
;665:
;666:/*
;667:=================
;668:UI_SPLevelMenu_Cache
;669:=================
;670:*/
;671:void UI_SPLevelMenu_Cache( void ) {
line 674
;672:	int				n;
;673:
;674:	trap_R_RegisterShaderNoMip( ART_LEVELFRAME_FOCUS );
ADDRGP4 $396
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 675
;675:	trap_R_RegisterShaderNoMip( ART_LEVELFRAME_SELECTED );
ADDRGP4 $397
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 676
;676:	trap_R_RegisterShaderNoMip( ART_ARROW );
ADDRGP4 $398
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 677
;677:	trap_R_RegisterShaderNoMip( ART_ARROW_FOCUS );
ADDRGP4 $399
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 678
;678:	trap_R_RegisterShaderNoMip( ART_MAP_UNKNOWN );
ADDRGP4 $132
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 679
;679:	trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE1 );
ADDRGP4 $400
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 680
;680:	trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE2 );
ADDRGP4 $401
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 681
;681:	trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE3 );
ADDRGP4 $402
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 682
;682:	trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE4 );
ADDRGP4 $403
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 683
;683:	trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE5 );
ADDRGP4 $404
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 684
;684:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $405
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 685
;685:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $406
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 686
;686:	trap_R_RegisterShaderNoMip( ART_FIGHT0 );
ADDRGP4 $407
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 687
;687:	trap_R_RegisterShaderNoMip( ART_FIGHT1 );
ADDRGP4 $408
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 688
;688:	trap_R_RegisterShaderNoMip( ART_RESET0 );
ADDRGP4 $409
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 689
;689:	trap_R_RegisterShaderNoMip( ART_RESET1 );
ADDRGP4 $410
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 690
;690:	trap_R_RegisterShaderNoMip( ART_CUSTOM0 );
ADDRGP4 $411
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 691
;691:	trap_R_RegisterShaderNoMip( ART_CUSTOM1 );
ADDRGP4 $412
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 693
;692:
;693:	for( n = 0; n < 6; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $413
line 694
;694:		trap_R_RegisterShaderNoMip( ui_medalPicNames[n] );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_medalPicNames
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 695
;695:		levelMenuInfo.awardSounds[n] = trap_S_RegisterSound( ui_medalSounds[n], qfalse );
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 ui_medalSounds
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 trap_S_RegisterSound
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 levelMenuInfo+2488
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 696
;696:	}
LABELV $414
line 693
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LTI4 $413
line 698
;697:
;698:	levelMenuInfo.levelSelectedPic = trap_R_RegisterShaderNoMip( ART_LEVELFRAME_SELECTED );
ADDRGP4 $397
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2308
ADDRLP4 4
INDIRI4
ASGNI4
line 699
;699:	levelMenuInfo.levelFocusPic = trap_R_RegisterShaderNoMip( ART_LEVELFRAME_FOCUS );
ADDRGP4 $396
ARGP4
ADDRLP4 8
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2312
ADDRLP4 8
INDIRI4
ASGNI4
line 700
;700:	levelMenuInfo.levelCompletePic[0] = trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE1 );
ADDRGP4 $400
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2316
ADDRLP4 12
INDIRI4
ASGNI4
line 701
;701:	levelMenuInfo.levelCompletePic[1] = trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE2 );
ADDRGP4 $401
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2316+4
ADDRLP4 16
INDIRI4
ASGNI4
line 702
;702:	levelMenuInfo.levelCompletePic[2] = trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE3 );
ADDRGP4 $402
ARGP4
ADDRLP4 20
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2316+8
ADDRLP4 20
INDIRI4
ASGNI4
line 703
;703:	levelMenuInfo.levelCompletePic[3] = trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE4 );
ADDRGP4 $403
ARGP4
ADDRLP4 24
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2316+12
ADDRLP4 24
INDIRI4
ASGNI4
line 704
;704:	levelMenuInfo.levelCompletePic[4] = trap_R_RegisterShaderNoMip( ART_MAP_COMPLETE5 );
ADDRGP4 $404
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 levelMenuInfo+2316+16
ADDRLP4 28
INDIRI4
ASGNI4
line 705
;705:}
LABELV $395
endproc UI_SPLevelMenu_Cache 32 8
proc UI_SPLevelMenu_Init 116 12
line 713
;706:
;707:
;708:/*
;709:=================
;710:UI_SPLevelMenu_Init
;711:=================
;712:*/
;713:static void UI_SPLevelMenu_Init( void ) {
line 720
;714:	int		skill;
;715:	int		n;
;716:	int		x, y;
;717:	int		count;
;718:	char	buf[MAX_QPATH];
;719:
;720:	skill = (int)trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $430
ARGP4
ADDRLP4 84
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 84
INDIRF4
CVFI4 4
ASGNI4
line 721
;721:	if( skill < 1 || skill > 5 ) {
ADDRLP4 16
INDIRI4
CNSTI4 1
LTI4 $433
ADDRLP4 16
INDIRI4
CNSTI4 5
LEI4 $431
LABELV $433
line 722
;722:		trap_Cvar_Set( "g_spSkill", "2" );
ADDRGP4 $430
ARGP4
ADDRGP4 $434
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 723
;723:		skill = 2;
ADDRLP4 16
CNSTI4 2
ASGNI4
line 724
;724:	}
LABELV $431
line 726
;725:
;726:	memset( &levelMenuInfo, 0, sizeof(levelMenuInfo) );
ADDRGP4 levelMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2616
ARGI4
ADDRGP4 memset
CALLP4
pop
line 727
;727:	levelMenuInfo.menu.fullscreen = qtrue;
ADDRGP4 levelMenuInfo+280
CNSTI4 1
ASGNI4
line 728
;728:	levelMenuInfo.menu.wrapAround = qtrue;
ADDRGP4 levelMenuInfo+276
CNSTI4 1
ASGNI4
line 729
;729:	levelMenuInfo.menu.draw = UI_SPLevelMenu_MenuDraw;
ADDRGP4 levelMenuInfo+268
ADDRGP4 UI_SPLevelMenu_MenuDraw
ASGNP4
line 731
;730:
;731:	UI_SPLevelMenu_Cache();
ADDRGP4 UI_SPLevelMenu_Cache
CALLV
pop
line 733
;732:
;733:	levelMenuInfo.item_banner.generic.type			= MTYPE_BTEXT;
ADDRGP4 levelMenuInfo+288
CNSTI4 10
ASGNI4
line 734
;734:	levelMenuInfo.item_banner.generic.x				= 320;
ADDRGP4 levelMenuInfo+288+12
CNSTI4 320
ASGNI4
line 735
;735:	levelMenuInfo.item_banner.generic.y				= 16;
ADDRGP4 levelMenuInfo+288+16
CNSTI4 16
ASGNI4
line 736
;736:	levelMenuInfo.item_banner.string				= "CHOOSE LEVEL";
ADDRGP4 levelMenuInfo+288+60
ADDRGP4 $445
ASGNP4
line 737
;737:	levelMenuInfo.item_banner.color					= color_red;
ADDRGP4 levelMenuInfo+288+68
ADDRGP4 color_red
ASGNP4
line 738
;738:	levelMenuInfo.item_banner.style					= UI_CENTER;
ADDRGP4 levelMenuInfo+288+64
CNSTI4 1
ASGNI4
line 740
;739:
;740:	levelMenuInfo.item_leftarrow.generic.type		= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+360
CNSTI4 6
ASGNI4
line 741
;741:	levelMenuInfo.item_leftarrow.generic.name		= ART_ARROW;
ADDRGP4 levelMenuInfo+360+4
ADDRGP4 $398
ASGNP4
line 742
;742:	levelMenuInfo.item_leftarrow.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+360+44
CNSTU4 260
ASGNU4
line 743
;743:	levelMenuInfo.item_leftarrow.generic.x			= 18;
ADDRGP4 levelMenuInfo+360+12
CNSTI4 18
ASGNI4
line 744
;744:	levelMenuInfo.item_leftarrow.generic.y			= 64;
ADDRGP4 levelMenuInfo+360+16
CNSTI4 64
ASGNI4
line 745
;745:	levelMenuInfo.item_leftarrow.generic.callback	= UI_SPLevelMenu_LeftArrowEvent;
ADDRGP4 levelMenuInfo+360+48
ADDRGP4 UI_SPLevelMenu_LeftArrowEvent
ASGNP4
line 746
;746:	levelMenuInfo.item_leftarrow.generic.id			= ID_LEFTARROW;
ADDRGP4 levelMenuInfo+360+8
CNSTI4 10
ASGNI4
line 747
;747:	levelMenuInfo.item_leftarrow.width				= 16;
ADDRGP4 levelMenuInfo+360+76
CNSTI4 16
ASGNI4
line 748
;748:	levelMenuInfo.item_leftarrow.height				= 114;
ADDRGP4 levelMenuInfo+360+80
CNSTI4 114
ASGNI4
line 749
;749:	levelMenuInfo.item_leftarrow.focuspic			= ART_ARROW_FOCUS;
ADDRGP4 levelMenuInfo+360+60
ADDRGP4 $399
ASGNP4
line 751
;750:
;751:	levelMenuInfo.item_maps[0].generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+448
CNSTI4 6
ASGNI4
line 752
;752:	levelMenuInfo.item_maps[0].generic.name			= levelMenuInfo.levelPicNames[0];
ADDRGP4 levelMenuInfo+448+4
ADDRGP4 levelMenuInfo+1956
ASGNP4
line 753
;753:	levelMenuInfo.item_maps[0].generic.flags		= QMF_LEFT_JUSTIFY;
ADDRGP4 levelMenuInfo+448+44
CNSTU4 4
ASGNU4
line 754
;754:	levelMenuInfo.item_maps[0].generic.x			= 46;
ADDRGP4 levelMenuInfo+448+12
CNSTI4 46
ASGNI4
line 755
;755:	levelMenuInfo.item_maps[0].generic.y			= 64;
ADDRGP4 levelMenuInfo+448+16
CNSTI4 64
ASGNI4
line 756
;756:	levelMenuInfo.item_maps[0].generic.id			= ID_PICTURE0;
ADDRGP4 levelMenuInfo+448+8
CNSTI4 11
ASGNI4
line 757
;757:	levelMenuInfo.item_maps[0].generic.callback		= UI_SPLevelMenu_LevelEvent;
ADDRGP4 levelMenuInfo+448+48
ADDRGP4 UI_SPLevelMenu_LevelEvent
ASGNP4
line 758
;758:	levelMenuInfo.item_maps[0].width				= 128;
ADDRGP4 levelMenuInfo+448+76
CNSTI4 128
ASGNI4
line 759
;759:	levelMenuInfo.item_maps[0].height				= 96;
ADDRGP4 levelMenuInfo+448+80
CNSTI4 96
ASGNI4
line 761
;760:
;761:	levelMenuInfo.item_maps[1].generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+448+88
CNSTI4 6
ASGNI4
line 762
;762:	levelMenuInfo.item_maps[1].generic.name			= levelMenuInfo.levelPicNames[1];
ADDRGP4 levelMenuInfo+448+88+4
ADDRGP4 levelMenuInfo+1956+64
ASGNP4
line 763
;763:	levelMenuInfo.item_maps[1].generic.flags		= QMF_LEFT_JUSTIFY;
ADDRGP4 levelMenuInfo+448+88+44
CNSTU4 4
ASGNU4
line 764
;764:	levelMenuInfo.item_maps[1].generic.x			= 186;
ADDRGP4 levelMenuInfo+448+88+12
CNSTI4 186
ASGNI4
line 765
;765:	levelMenuInfo.item_maps[1].generic.y			= 64;
ADDRGP4 levelMenuInfo+448+88+16
CNSTI4 64
ASGNI4
line 766
;766:	levelMenuInfo.item_maps[1].generic.id			= ID_PICTURE1;
ADDRGP4 levelMenuInfo+448+88+8
CNSTI4 12
ASGNI4
line 767
;767:	levelMenuInfo.item_maps[1].generic.callback		= UI_SPLevelMenu_LevelEvent;
ADDRGP4 levelMenuInfo+448+88+48
ADDRGP4 UI_SPLevelMenu_LevelEvent
ASGNP4
line 768
;768:	levelMenuInfo.item_maps[1].width				= 128;
ADDRGP4 levelMenuInfo+448+88+76
CNSTI4 128
ASGNI4
line 769
;769:	levelMenuInfo.item_maps[1].height				= 96;
ADDRGP4 levelMenuInfo+448+88+80
CNSTI4 96
ASGNI4
line 771
;770:
;771:	levelMenuInfo.item_maps[2].generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+448+176
CNSTI4 6
ASGNI4
line 772
;772:	levelMenuInfo.item_maps[2].generic.name			= levelMenuInfo.levelPicNames[2];
ADDRGP4 levelMenuInfo+448+176+4
ADDRGP4 levelMenuInfo+1956+128
ASGNP4
line 773
;773:	levelMenuInfo.item_maps[2].generic.flags		= QMF_LEFT_JUSTIFY;
ADDRGP4 levelMenuInfo+448+176+44
CNSTU4 4
ASGNU4
line 774
;774:	levelMenuInfo.item_maps[2].generic.x			= 326;
ADDRGP4 levelMenuInfo+448+176+12
CNSTI4 326
ASGNI4
line 775
;775:	levelMenuInfo.item_maps[2].generic.y			= 64;
ADDRGP4 levelMenuInfo+448+176+16
CNSTI4 64
ASGNI4
line 776
;776:	levelMenuInfo.item_maps[2].generic.id			= ID_PICTURE2;
ADDRGP4 levelMenuInfo+448+176+8
CNSTI4 13
ASGNI4
line 777
;777:	levelMenuInfo.item_maps[2].generic.callback		= UI_SPLevelMenu_LevelEvent;
ADDRGP4 levelMenuInfo+448+176+48
ADDRGP4 UI_SPLevelMenu_LevelEvent
ASGNP4
line 778
;778:	levelMenuInfo.item_maps[2].width				= 128;
ADDRGP4 levelMenuInfo+448+176+76
CNSTI4 128
ASGNI4
line 779
;779:	levelMenuInfo.item_maps[2].height				= 96;
ADDRGP4 levelMenuInfo+448+176+80
CNSTI4 96
ASGNI4
line 781
;780:
;781:	levelMenuInfo.item_maps[3].generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+448+264
CNSTI4 6
ASGNI4
line 782
;782:	levelMenuInfo.item_maps[3].generic.name			= levelMenuInfo.levelPicNames[3];
ADDRGP4 levelMenuInfo+448+264+4
ADDRGP4 levelMenuInfo+1956+192
ASGNP4
line 783
;783:	levelMenuInfo.item_maps[3].generic.flags		= QMF_LEFT_JUSTIFY;
ADDRGP4 levelMenuInfo+448+264+44
CNSTU4 4
ASGNU4
line 784
;784:	levelMenuInfo.item_maps[3].generic.x			= 466;
ADDRGP4 levelMenuInfo+448+264+12
CNSTI4 466
ASGNI4
line 785
;785:	levelMenuInfo.item_maps[3].generic.y			= 64;
ADDRGP4 levelMenuInfo+448+264+16
CNSTI4 64
ASGNI4
line 786
;786:	levelMenuInfo.item_maps[3].generic.id			= ID_PICTURE3;
ADDRGP4 levelMenuInfo+448+264+8
CNSTI4 14
ASGNI4
line 787
;787:	levelMenuInfo.item_maps[3].generic.callback		= UI_SPLevelMenu_LevelEvent;
ADDRGP4 levelMenuInfo+448+264+48
ADDRGP4 UI_SPLevelMenu_LevelEvent
ASGNP4
line 788
;788:	levelMenuInfo.item_maps[3].width				= 128;
ADDRGP4 levelMenuInfo+448+264+76
CNSTI4 128
ASGNI4
line 789
;789:	levelMenuInfo.item_maps[3].height				= 96;
ADDRGP4 levelMenuInfo+448+264+80
CNSTI4 96
ASGNI4
line 791
;790:
;791:	levelMenuInfo.item_rightarrow.generic.type		= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+800
CNSTI4 6
ASGNI4
line 792
;792:	levelMenuInfo.item_rightarrow.generic.name		= ART_ARROW;
ADDRGP4 levelMenuInfo+800+4
ADDRGP4 $398
ASGNP4
line 793
;793:	levelMenuInfo.item_rightarrow.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+800+44
CNSTU4 260
ASGNU4
line 794
;794:	levelMenuInfo.item_rightarrow.generic.x			= 606;
ADDRGP4 levelMenuInfo+800+12
CNSTI4 606
ASGNI4
line 795
;795:	levelMenuInfo.item_rightarrow.generic.y			= 64;
ADDRGP4 levelMenuInfo+800+16
CNSTI4 64
ASGNI4
line 796
;796:	levelMenuInfo.item_rightarrow.generic.callback	= UI_SPLevelMenu_RightArrowEvent;
ADDRGP4 levelMenuInfo+800+48
ADDRGP4 UI_SPLevelMenu_RightArrowEvent
ASGNP4
line 797
;797:	levelMenuInfo.item_rightarrow.generic.id		= ID_RIGHTARROW;
ADDRGP4 levelMenuInfo+800+8
CNSTI4 15
ASGNI4
line 798
;798:	levelMenuInfo.item_rightarrow.width				= -16;
ADDRGP4 levelMenuInfo+800+76
CNSTI4 -16
ASGNI4
line 799
;799:	levelMenuInfo.item_rightarrow.height			= 114;
ADDRGP4 levelMenuInfo+800+80
CNSTI4 114
ASGNI4
line 800
;800:	levelMenuInfo.item_rightarrow.focuspic			= ART_ARROW_FOCUS;
ADDRGP4 levelMenuInfo+800+60
ADDRGP4 $399
ASGNP4
line 802
;801:
;802:	trap_Cvar_VariableStringBuffer( "model", levelMenuInfo.playerModel, sizeof(levelMenuInfo.playerModel) );
ADDRGP4 $103
ARGP4
ADDRGP4 levelMenuInfo+2336
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 803
;803:	PlayerIcon( levelMenuInfo.playerModel, levelMenuInfo.playerPicName, sizeof(levelMenuInfo.playerPicName) );
ADDRGP4 levelMenuInfo+2336
ARGP4
ADDRGP4 levelMenuInfo+2400
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 PlayerIcon
CALLV
pop
line 804
;804:	levelMenuInfo.item_player.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+888
CNSTI4 6
ASGNI4
line 805
;805:	levelMenuInfo.item_player.generic.name			= levelMenuInfo.playerPicName;
ADDRGP4 levelMenuInfo+888+4
ADDRGP4 levelMenuInfo+2400
ASGNP4
line 806
;806:	levelMenuInfo.item_player.generic.flags			= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY;
ADDRGP4 levelMenuInfo+888+44
CNSTU4 2052
ASGNU4
line 807
;807:	levelMenuInfo.item_player.generic.x				= 288;
ADDRGP4 levelMenuInfo+888+12
CNSTI4 288
ASGNI4
line 808
;808:	levelMenuInfo.item_player.generic.y				= AWARDS_Y;
ADDRGP4 levelMenuInfo+888+16
CNSTI4 340
ASGNI4
line 809
;809:	levelMenuInfo.item_player.generic.id			= ID_PLAYERPIC;
ADDRGP4 levelMenuInfo+888+8
CNSTI4 16
ASGNI4
line 810
;810:	levelMenuInfo.item_player.generic.callback		= UI_SPLevelMenu_PlayerEvent;
ADDRGP4 levelMenuInfo+888+48
ADDRGP4 UI_SPLevelMenu_PlayerEvent
ASGNP4
line 811
;811:	levelMenuInfo.item_player.width					= 64;
ADDRGP4 levelMenuInfo+888+76
CNSTI4 64
ASGNI4
line 812
;812:	levelMenuInfo.item_player.height				= 64;
ADDRGP4 levelMenuInfo+888+80
CNSTI4 64
ASGNI4
line 814
;813:
;814:	for( n = 0; n < 6; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $613
line 815
;815:		levelMenuInfo.awardLevels[n] = UI_GetAwardLevel( n );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 96
ADDRGP4 UI_GetAwardLevel
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2464
ADDP4
ADDRLP4 96
INDIRI4
ASGNI4
line 816
;816:	}
LABELV $614
line 814
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LTI4 $613
line 817
;817:	levelMenuInfo.awardLevels[AWARD_FRAGS] = 100 * (levelMenuInfo.awardLevels[AWARD_FRAGS] / 100);
ADDRLP4 92
CNSTI4 100
ASGNI4
ADDRGP4 levelMenuInfo+2464+16
ADDRLP4 92
INDIRI4
ADDRGP4 levelMenuInfo+2464+16
INDIRI4
ADDRLP4 92
INDIRI4
DIVI4
MULI4
ASGNI4
line 819
;818:
;819:	y = AWARDS_Y;
ADDRLP4 12
CNSTI4 340
ASGNI4
line 820
;820:	count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 821
;821:	for( n = 0; n < 6; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $622
line 822
;822:		if( levelMenuInfo.awardLevels[n] ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 levelMenuInfo+2464
ADDP4
INDIRI4
CNSTI4 0
EQI4 $626
line 823
;823:			if( count & 1 ) {
ADDRLP4 4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $629
line 824
;824:				x = 224 - (count - 1 ) / 2 * (48 + 16);
ADDRLP4 8
CNSTI4 224
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
CNSTI4 2
DIVI4
CNSTI4 6
LSHI4
SUBI4
ASGNI4
line 825
;825:			}
ADDRGP4 $630
JUMPV
LABELV $629
line 826
;826:			else {
line 827
;827:				x = 368 + count / 2 * (48 + 16);
ADDRLP4 8
ADDRLP4 4
INDIRI4
CNSTI4 2
DIVI4
CNSTI4 6
LSHI4
CNSTI4 368
ADDI4
ASGNI4
line 828
;828:			}
LABELV $630
line 830
;829:
;830:			levelMenuInfo.item_awards[count].generic.type		= MTYPE_BITMAP;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976
ADDP4
CNSTI4 6
ASGNI4
line 831
;831:			levelMenuInfo.item_awards[count].generic.name		= ui_medalPicNames[n];
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+4
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_medalPicNames
ADDP4
INDIRP4
ASGNP4
line 832
;832:			levelMenuInfo.item_awards[count].generic.flags		= QMF_LEFT_JUSTIFY|QMF_SILENT|QMF_MOUSEONLY;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+44
ADDP4
CNSTU4 1050628
ASGNU4
line 833
;833:			levelMenuInfo.item_awards[count].generic.x			= x;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+12
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 834
;834:			levelMenuInfo.item_awards[count].generic.y			= y;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+16
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 835
;835:			levelMenuInfo.item_awards[count].generic.id			= ID_AWARD1 + n;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 17
ADDI4
ASGNI4
line 836
;836:			levelMenuInfo.item_awards[count].generic.callback	= UI_SPLevelMenu_AwardEvent;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+48
ADDP4
ADDRGP4 UI_SPLevelMenu_AwardEvent
ASGNP4
line 837
;837:			levelMenuInfo.item_awards[count].width				= 48;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+76
ADDP4
CNSTI4 48
ASGNI4
line 838
;838:			levelMenuInfo.item_awards[count].height				= 48;
CNSTI4 88
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976+80
ADDP4
CNSTI4 48
ASGNI4
line 839
;839:			count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 840
;840:		}
LABELV $626
line 841
;841:	}
LABELV $623
line 821
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LTI4 $622
line 843
;842:
;843:	levelMenuInfo.item_back.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+1504
CNSTI4 6
ASGNI4
line 844
;844:	levelMenuInfo.item_back.generic.name			= ART_BACK0;
ADDRGP4 levelMenuInfo+1504+4
ADDRGP4 $405
ASGNP4
line 845
;845:	levelMenuInfo.item_back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+1504+44
CNSTU4 260
ASGNU4
line 846
;846:	levelMenuInfo.item_back.generic.x				= 0;
ADDRGP4 levelMenuInfo+1504+12
CNSTI4 0
ASGNI4
line 847
;847:	levelMenuInfo.item_back.generic.y				= 480-64;
ADDRGP4 levelMenuInfo+1504+16
CNSTI4 416
ASGNI4
line 848
;848:	levelMenuInfo.item_back.generic.callback		= UI_SPLevelMenu_BackEvent;
ADDRGP4 levelMenuInfo+1504+48
ADDRGP4 UI_SPLevelMenu_BackEvent
ASGNP4
line 849
;849:	levelMenuInfo.item_back.generic.id				= ID_BACK;
ADDRGP4 levelMenuInfo+1504+8
CNSTI4 23
ASGNI4
line 850
;850:	levelMenuInfo.item_back.width					= 128;
ADDRGP4 levelMenuInfo+1504+76
CNSTI4 128
ASGNI4
line 851
;851:	levelMenuInfo.item_back.height					= 64;
ADDRGP4 levelMenuInfo+1504+80
CNSTI4 64
ASGNI4
line 852
;852:	levelMenuInfo.item_back.focuspic				= ART_BACK1;
ADDRGP4 levelMenuInfo+1504+60
ADDRGP4 $406
ASGNP4
line 854
;853:
;854:	levelMenuInfo.item_reset.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+1592
CNSTI4 6
ASGNI4
line 855
;855:	levelMenuInfo.item_reset.generic.name			= ART_RESET0;
ADDRGP4 levelMenuInfo+1592+4
ADDRGP4 $409
ASGNP4
line 856
;856:	levelMenuInfo.item_reset.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+1592+44
CNSTU4 260
ASGNU4
line 857
;857:	levelMenuInfo.item_reset.generic.x				= 170;
ADDRGP4 levelMenuInfo+1592+12
CNSTI4 170
ASGNI4
line 858
;858:	levelMenuInfo.item_reset.generic.y				= 480-64;
ADDRGP4 levelMenuInfo+1592+16
CNSTI4 416
ASGNI4
line 859
;859:	levelMenuInfo.item_reset.generic.callback		= UI_SPLevelMenu_ResetEvent;
ADDRGP4 levelMenuInfo+1592+48
ADDRGP4 UI_SPLevelMenu_ResetEvent
ASGNP4
line 860
;860:	levelMenuInfo.item_reset.generic.id				= ID_RESET;
ADDRGP4 levelMenuInfo+1592+8
CNSTI4 24
ASGNI4
line 861
;861:	levelMenuInfo.item_reset.width					= 128;
ADDRGP4 levelMenuInfo+1592+76
CNSTI4 128
ASGNI4
line 862
;862:	levelMenuInfo.item_reset.height					= 64;
ADDRGP4 levelMenuInfo+1592+80
CNSTI4 64
ASGNI4
line 863
;863:	levelMenuInfo.item_reset.focuspic				= ART_RESET1;
ADDRGP4 levelMenuInfo+1592+60
ADDRGP4 $410
ASGNP4
line 865
;864:
;865:	levelMenuInfo.item_custom.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+1680
CNSTI4 6
ASGNI4
line 866
;866:	levelMenuInfo.item_custom.generic.name			= ART_CUSTOM0;
ADDRGP4 levelMenuInfo+1680+4
ADDRGP4 $411
ASGNP4
line 867
;867:	levelMenuInfo.item_custom.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+1680+44
CNSTU4 260
ASGNU4
line 868
;868:	levelMenuInfo.item_custom.generic.x				= 342;
ADDRGP4 levelMenuInfo+1680+12
CNSTI4 342
ASGNI4
line 869
;869:	levelMenuInfo.item_custom.generic.y				= 480-64;
ADDRGP4 levelMenuInfo+1680+16
CNSTI4 416
ASGNI4
line 870
;870:	levelMenuInfo.item_custom.generic.callback		= UI_SPLevelMenu_CustomEvent;
ADDRGP4 levelMenuInfo+1680+48
ADDRGP4 UI_SPLevelMenu_CustomEvent
ASGNP4
line 871
;871:	levelMenuInfo.item_custom.generic.id			= ID_CUSTOM;
ADDRGP4 levelMenuInfo+1680+8
CNSTI4 25
ASGNI4
line 872
;872:	levelMenuInfo.item_custom.width					= 128;
ADDRGP4 levelMenuInfo+1680+76
CNSTI4 128
ASGNI4
line 873
;873:	levelMenuInfo.item_custom.height				= 64;
ADDRGP4 levelMenuInfo+1680+80
CNSTI4 64
ASGNI4
line 874
;874:	levelMenuInfo.item_custom.focuspic				= ART_CUSTOM1;
ADDRGP4 levelMenuInfo+1680+60
ADDRGP4 $412
ASGNP4
line 876
;875:
;876:	levelMenuInfo.item_next.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+1768
CNSTI4 6
ASGNI4
line 877
;877:	levelMenuInfo.item_next.generic.name			= ART_FIGHT0;
ADDRGP4 levelMenuInfo+1768+4
ADDRGP4 $407
ASGNP4
line 878
;878:	levelMenuInfo.item_next.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 levelMenuInfo+1768+44
CNSTU4 272
ASGNU4
line 879
;879:	levelMenuInfo.item_next.generic.x				= 640;
ADDRGP4 levelMenuInfo+1768+12
CNSTI4 640
ASGNI4
line 880
;880:	levelMenuInfo.item_next.generic.y				= 480-64;
ADDRGP4 levelMenuInfo+1768+16
CNSTI4 416
ASGNI4
line 881
;881:	levelMenuInfo.item_next.generic.callback		= UI_SPLevelMenu_NextEvent;
ADDRGP4 levelMenuInfo+1768+48
ADDRGP4 UI_SPLevelMenu_NextEvent
ASGNP4
line 882
;882:	levelMenuInfo.item_next.generic.id				= ID_NEXT;
ADDRGP4 levelMenuInfo+1768+8
CNSTI4 26
ASGNI4
line 883
;883:	levelMenuInfo.item_next.width					= 128;
ADDRGP4 levelMenuInfo+1768+76
CNSTI4 128
ASGNI4
line 884
;884:	levelMenuInfo.item_next.height					= 64;
ADDRGP4 levelMenuInfo+1768+80
CNSTI4 64
ASGNI4
line 885
;885:	levelMenuInfo.item_next.focuspic				= ART_FIGHT1;
ADDRGP4 levelMenuInfo+1768+60
ADDRGP4 $408
ASGNP4
line 887
;886:
;887:	levelMenuInfo.item_null.generic.type			= MTYPE_BITMAP;
ADDRGP4 levelMenuInfo+1856
CNSTI4 6
ASGNI4
line 888
;888:	levelMenuInfo.item_null.generic.flags			= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
ADDRGP4 levelMenuInfo+1856+44
CNSTU4 1050628
ASGNU4
line 889
;889:	levelMenuInfo.item_null.generic.x				= 0;
ADDRGP4 levelMenuInfo+1856+12
CNSTI4 0
ASGNI4
line 890
;890:	levelMenuInfo.item_null.generic.y				= 0;
ADDRGP4 levelMenuInfo+1856+16
CNSTI4 0
ASGNI4
line 891
;891:	levelMenuInfo.item_null.width					= 640;
ADDRGP4 levelMenuInfo+1856+76
CNSTI4 640
ASGNI4
line 892
;892:	levelMenuInfo.item_null.height					= 480;
ADDRGP4 levelMenuInfo+1856+80
CNSTI4 480
ASGNI4
line 894
;893:
;894:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_banner );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 896
;895:
;896:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_leftarrow );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 897
;897:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_maps[0] );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 898
;898:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_maps[1] );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+448+88
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 899
;899:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_maps[2] );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+448+176
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 900
;900:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_maps[3] );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+448+264
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 901
;901:	levelMenuInfo.item_maps[0].generic.bottom += 18;
ADDRLP4 96
ADDRGP4 levelMenuInfo+448+32
ASGNP4
ADDRLP4 96
INDIRP4
ADDRLP4 96
INDIRP4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 902
;902:	levelMenuInfo.item_maps[1].generic.bottom += 18;
ADDRLP4 100
ADDRGP4 levelMenuInfo+448+88+32
ASGNP4
ADDRLP4 100
INDIRP4
ADDRLP4 100
INDIRP4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 903
;903:	levelMenuInfo.item_maps[2].generic.bottom += 18;
ADDRLP4 104
ADDRGP4 levelMenuInfo+448+176+32
ASGNP4
ADDRLP4 104
INDIRP4
ADDRLP4 104
INDIRP4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 904
;904:	levelMenuInfo.item_maps[3].generic.bottom += 18;
ADDRLP4 108
ADDRGP4 levelMenuInfo+448+264+32
ASGNP4
ADDRLP4 108
INDIRP4
ADDRLP4 108
INDIRP4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 905
;905:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_rightarrow );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+800
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 907
;906:
;907:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_player );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+888
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 909
;908:
;909:	for( n = 0; n < count; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $760
JUMPV
LABELV $757
line 910
;910:		Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_awards[n] );
ADDRGP4 levelMenuInfo
ARGP4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 levelMenuInfo+976
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 911
;911:	}
LABELV $758
line 909
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $760
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $757
line 912
;912:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_back );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1504
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 913
;913:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_reset );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1592
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 914
;914:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_custom );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 915
;915:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_next );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1768
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 916
;916:	Menu_AddItem( &levelMenuInfo.menu, &levelMenuInfo.item_null );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1856
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 918
;917:
;918:	trap_Cvar_VariableStringBuffer( "ui_spSelection", buf, sizeof(buf) );
ADDRGP4 $153
ARGP4
ADDRLP4 20
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 919
;919:	if( *buf ) {
ADDRLP4 20
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $767
line 920
;920:		n = atoi( buf );
ADDRLP4 20
ARGP4
ADDRLP4 112
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 112
INDIRI4
ASGNI4
line 921
;921:		selectedArenaSet = n / ARENAS_PER_TIER;
ADDRGP4 selectedArenaSet
ADDRLP4 0
INDIRI4
CNSTI4 4
DIVI4
ASGNI4
line 922
;922:		selectedArena = n % ARENAS_PER_TIER;
ADDRGP4 selectedArena
ADDRLP4 0
INDIRI4
CNSTI4 4
MODI4
ASGNI4
line 923
;923:	}
ADDRGP4 $768
JUMPV
LABELV $767
line 924
;924:	else {
line 925
;925:		selectedArenaSet = currentSet;
ADDRGP4 selectedArenaSet
ADDRGP4 currentSet
INDIRI4
ASGNI4
line 926
;926:		selectedArena = currentGame;
ADDRGP4 selectedArena
ADDRGP4 currentGame
INDIRI4
ASGNI4
line 927
;927:	}
LABELV $768
line 929
;928:
;929:	UI_SPLevelMenu_SetMenuItems();
ADDRGP4 UI_SPLevelMenu_SetMenuItems
CALLV
pop
line 930
;930:}
LABELV $429
endproc UI_SPLevelMenu_Init 116 12
export UI_SPLevelMenu
proc UI_SPLevelMenu 32 8
line 938
;931:
;932:
;933:/*
;934:=================
;935:UI_SPLevelMenu
;936:=================
;937:*/
;938:void UI_SPLevelMenu( void ) {
line 943
;939:	int			level;
;940:	int			trainingLevel;
;941:	const char	*arenaInfo;
;942:
;943:	trainingTier = -1;
ADDRGP4 trainingTier
CNSTI4 -1
ASGNI4
line 944
;944:	arenaInfo = UI_GetSpecialArenaInfo( "training" );
ADDRGP4 $156
ARGP4
ADDRLP4 12
ADDRGP4 UI_GetSpecialArenaInfo
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
ASGNP4
line 945
;945:	if( arenaInfo ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $770
line 946
;946:		minTier = trainingTier;
ADDRGP4 minTier
ADDRGP4 trainingTier
INDIRI4
ASGNI4
line 947
;947:		trainingLevel = atoi( Info_ValueForKey( arenaInfo, "num" ) );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $157
ARGP4
ADDRLP4 16
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 20
INDIRI4
ASGNI4
line 948
;948:	}
ADDRGP4 $771
JUMPV
LABELV $770
line 949
;949:	else {
line 950
;950:		minTier = 0;
ADDRGP4 minTier
CNSTI4 0
ASGNI4
line 951
;951:		trainingLevel = -2;
ADDRLP4 8
CNSTI4 -2
ASGNI4
line 952
;952:	}
LABELV $771
line 954
;953:
;954:	finalTier = UI_GetNumSPTiers();
ADDRLP4 16
ADDRGP4 UI_GetNumSPTiers
CALLI4
ASGNI4
ADDRGP4 finalTier
ADDRLP4 16
INDIRI4
ASGNI4
line 955
;955:	arenaInfo = UI_GetSpecialArenaInfo( "final" );
ADDRGP4 $191
ARGP4
ADDRLP4 20
ADDRGP4 UI_GetSpecialArenaInfo
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 956
;956:	if( arenaInfo ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $772
line 957
;957:		maxTier = finalTier;
ADDRGP4 maxTier
ADDRGP4 finalTier
INDIRI4
ASGNI4
line 958
;958:	}
ADDRGP4 $773
JUMPV
LABELV $772
line 959
;959:	else {
line 960
;960:		maxTier = finalTier - 1;
ADDRGP4 maxTier
ADDRGP4 finalTier
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 961
;961:		if( maxTier < minTier ) {
ADDRGP4 maxTier
INDIRI4
ADDRGP4 minTier
INDIRI4
GEI4 $774
line 962
;962:			maxTier = minTier;
ADDRGP4 maxTier
ADDRGP4 minTier
INDIRI4
ASGNI4
line 963
;963:		}
LABELV $774
line 964
;964:	}
LABELV $773
line 966
;965:
;966:	level = UI_GetCurrentGame();
ADDRLP4 24
ADDRGP4 UI_GetCurrentGame
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 24
INDIRI4
ASGNI4
line 967
;967:	if ( level == -1 ) {
ADDRLP4 0
INDIRI4
CNSTI4 -1
NEI4 $776
line 968
;968:		level = UI_GetNumSPArenas() - 1;
ADDRLP4 28
ADDRGP4 UI_GetNumSPArenas
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 28
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 969
;969:		if( maxTier == finalTier ) {
ADDRGP4 maxTier
INDIRI4
ADDRGP4 finalTier
INDIRI4
NEI4 $778
line 970
;970:			level++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 971
;971:		}
LABELV $778
line 972
;972:	}
LABELV $776
line 974
;973:
;974:	if( level == trainingLevel ) {
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
NEI4 $780
line 975
;975:		currentSet = -1;
ADDRGP4 currentSet
CNSTI4 -1
ASGNI4
line 976
;976:		currentGame = 0;
ADDRGP4 currentGame
CNSTI4 0
ASGNI4
line 977
;977:	}
ADDRGP4 $781
JUMPV
LABELV $780
line 978
;978:	else {
line 979
;979:		currentSet = level / ARENAS_PER_TIER;
ADDRGP4 currentSet
ADDRLP4 0
INDIRI4
CNSTI4 4
DIVI4
ASGNI4
line 980
;980:		currentGame = level % ARENAS_PER_TIER;
ADDRGP4 currentGame
ADDRLP4 0
INDIRI4
CNSTI4 4
MODI4
ASGNI4
line 981
;981:	}
LABELV $781
line 983
;982:
;983:	UI_SPLevelMenu_Init();
ADDRGP4 UI_SPLevelMenu_Init
CALLV
pop
line 984
;984:	UI_PushMenu( &levelMenuInfo.menu );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 985
;985:	Menu_SetCursorToItem( &levelMenuInfo.menu, &levelMenuInfo.item_next );
ADDRGP4 levelMenuInfo
ARGP4
ADDRGP4 levelMenuInfo+1768
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 986
;986:}
LABELV $769
endproc UI_SPLevelMenu 32 8
export UI_SPLevelMenu_f
proc UI_SPLevelMenu_f 0 4
line 994
;987:
;988:
;989:/*
;990:=================
;991:UI_SPLevelMenu_f
;992:=================
;993:*/
;994:void UI_SPLevelMenu_f( void ) {
line 995
;995:	trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 996
;996:	uis.menusp = 0;
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
line 997
;997:	UI_SPLevelMenu();
ADDRGP4 UI_SPLevelMenu
CALLV
pop
line 998
;998:}
LABELV $783
endproc UI_SPLevelMenu_f 0 4
export UI_SPLevelMenu_ReInit
proc UI_SPLevelMenu_ReInit 0 0
line 1006
;999:
;1000:
;1001:/*
;1002:=================
;1003:UI_SPLevelMenu_ReInit
;1004:=================
;1005:*/
;1006:void UI_SPLevelMenu_ReInit( void ) {
line 1007
;1007:	levelMenuInfo.reinit = qtrue;
ADDRGP4 levelMenuInfo+1944
CNSTI4 1
ASGNI4
line 1008
;1008:}
LABELV $785
endproc UI_SPLevelMenu_ReInit 0 0
bss
align 4
LABELV maxTier
skip 4
align 4
LABELV minTier
skip 4
align 4
LABELV finalTier
skip 4
align 4
LABELV trainingTier
skip 4
align 4
LABELV currentGame
skip 4
align 4
LABELV currentSet
skip 4
align 4
LABELV selectedArena
skip 4
align 4
LABELV selectedArenaSet
skip 4
align 4
LABELV levelMenuInfo
skip 2616
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
LABELV $445
byte 1 67
byte 1 72
byte 1 79
byte 1 79
byte 1 83
byte 1 69
byte 1 32
byte 1 76
byte 1 69
byte 1 86
byte 1 69
byte 1 76
byte 1 0
align 1
LABELV $434
byte 1 50
byte 1 0
align 1
LABELV $430
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
LABELV $412
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 115
byte 1 107
byte 1 105
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 104
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $411
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 115
byte 1 107
byte 1 105
byte 1 114
byte 1 109
byte 1 105
byte 1 115
byte 1 104
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $410
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $409
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $408
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
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $407
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
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $406
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
LABELV $405
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
LABELV $404
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 53
byte 1 0
align 1
LABELV $403
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 52
byte 1 0
align 1
LABELV $402
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 51
byte 1 0
align 1
LABELV $401
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 50
byte 1 0
align 1
LABELV $400
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 95
byte 1 99
byte 1 111
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 49
byte 1 0
align 1
LABELV $399
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 110
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $398
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 110
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $397
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $396
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 109
byte 1 97
byte 1 112
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $393
byte 1 63
byte 1 0
align 1
LABELV $382
byte 1 108
byte 1 111
byte 1 110
byte 1 103
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $380
byte 1 37
byte 1 115
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $344
byte 1 65
byte 1 67
byte 1 67
byte 1 69
byte 1 83
byte 1 83
byte 1 32
byte 1 68
byte 1 69
byte 1 78
byte 1 73
byte 1 69
byte 1 68
byte 1 0
align 1
LABELV $332
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $331
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $330
byte 1 37
byte 1 105
byte 1 107
byte 1 0
align 1
LABELV $327
byte 1 37
byte 1 105
byte 1 109
byte 1 0
align 1
LABELV $259
byte 1 82
byte 1 69
byte 1 83
byte 1 69
byte 1 84
byte 1 32
byte 1 71
byte 1 65
byte 1 77
byte 1 69
byte 1 63
byte 1 0
align 1
LABELV $252
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 102
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 98
byte 1 101
byte 1 103
byte 1 105
byte 1 110
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 46
byte 1 0
align 1
LABELV $251
byte 1 68
byte 1 111
byte 1 32
byte 1 116
byte 1 104
byte 1 105
byte 1 115
byte 1 32
byte 1 111
byte 1 110
byte 1 108
byte 1 121
byte 1 32
byte 1 105
byte 1 102
byte 1 32
byte 1 121
byte 1 111
byte 1 117
byte 1 32
byte 1 119
byte 1 97
byte 1 110
byte 1 116
byte 1 32
byte 1 116
byte 1 111
byte 1 0
align 1
LABELV $250
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 32
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 118
byte 1 97
byte 1 114
byte 1 105
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 115
byte 1 46
byte 1 0
align 1
LABELV $249
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 84
byte 1 104
byte 1 105
byte 1 115
byte 1 32
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 115
byte 1 32
byte 1 97
byte 1 108
byte 1 108
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 0
align 1
LABELV $191
byte 1 102
byte 1 105
byte 1 110
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $157
byte 1 110
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $156
byte 1 116
byte 1 114
byte 1 97
byte 1 105
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $153
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
LABELV $127
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
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $116
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $106
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $103
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $82
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $75
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
LABELV $72
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
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 37
byte 1 115
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $71
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
