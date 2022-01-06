code
proc MainMenu_ExitAction 0 0
file "../ui_menu.c"
line 78
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
;26:MAIN MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ID_SINGLEPLAYER			10
;36:#define ID_MULTIPLAYER			11
;37:#define ID_SETUP				12
;38:#define ID_DEMOS				13
;39:#define ID_CINEMATICS			14
;40:#define ID_TEAMARENA		15
;41:#define ID_MODS					16
;42:#define ID_EXIT					17
;43:
;44:#define MAIN_BANNER_MODEL				"models/mapobjects/banner/banner5.md3"
;45:#define MAIN_MENU_VERTICAL_SPACING		34
;46:
;47:
;48:typedef struct {
;49:	menuframework_s	menu;
;50:
;51:	menutext_s		singleplayer;
;52:	menutext_s		multiplayer;
;53:	menutext_s		setup;
;54:	menutext_s		demos;
;55:	menutext_s		cinematics;
;56:	menutext_s		teamArena;
;57:	menutext_s		mods;
;58:	menutext_s		exit;
;59:
;60:	qhandle_t		bannerModel;
;61:} mainmenu_t;
;62:
;63:
;64:static mainmenu_t s_main;
;65:
;66:typedef struct {
;67:	menuframework_s menu;	
;68:	char errorMessage[4096];
;69:} errorMessage_t;
;70:
;71:static errorMessage_t s_errorMessage;
;72:
;73:/*
;74:=================
;75:MainMenu_ExitAction
;76:=================
;77:*/
;78:static void MainMenu_ExitAction( qboolean result ) {
line 79
;79:	if( !result ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $70
line 80
;80:		return;
ADDRGP4 $69
JUMPV
LABELV $70
line 82
;81:	}
;82:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 83
;83:	UI_CreditMenu();
ADDRGP4 UI_CreditMenu
CALLV
pop
line 84
;84:}
LABELV $69
endproc MainMenu_ExitAction 0 0
export Main_MenuEvent
proc Main_MenuEvent 8 12
line 93
;85:
;86:
;87:
;88:/*
;89:=================
;90:Main_MenuEvent
;91:=================
;92:*/
;93:void Main_MenuEvent (void* ptr, int event) {
line 94
;94:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $73
line 95
;95:		return;
ADDRGP4 $72
JUMPV
LABELV $73
line 98
;96:	}
;97:
;98:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 10
LTI4 $75
ADDRLP4 0
INDIRI4
CNSTI4 17
GTI4 $75
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $90-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $90
address $78
address $79
address $80
address $81
address $82
address $84
address $83
address $88
code
LABELV $78
line 100
;99:	case ID_SINGLEPLAYER:
;100:		UI_SPLevelMenu();
ADDRGP4 UI_SPLevelMenu
CALLV
pop
line 101
;101:		break;
ADDRGP4 $76
JUMPV
LABELV $79
line 104
;102:
;103:	case ID_MULTIPLAYER:
;104:		UI_ArenaServersMenu();
ADDRGP4 UI_ArenaServersMenu
CALLV
pop
line 105
;105:		break;
ADDRGP4 $76
JUMPV
LABELV $80
line 108
;106:
;107:	case ID_SETUP:
;108:		UI_SetupMenu();
ADDRGP4 UI_SetupMenu
CALLV
pop
line 109
;109:		break;
ADDRGP4 $76
JUMPV
LABELV $81
line 112
;110:
;111:	case ID_DEMOS:
;112:		UI_DemosMenu();
ADDRGP4 UI_DemosMenu
CALLV
pop
line 113
;113:		break;
ADDRGP4 $76
JUMPV
LABELV $82
line 116
;114:
;115:	case ID_CINEMATICS:
;116:		UI_CinematicsMenu();
ADDRGP4 UI_CinematicsMenu
CALLV
pop
line 117
;117:		break;
ADDRGP4 $76
JUMPV
LABELV $83
line 120
;118:
;119:	case ID_MODS:
;120:		UI_ModsMenu();
ADDRGP4 UI_ModsMenu
CALLV
pop
line 121
;121:		break;
ADDRGP4 $76
JUMPV
LABELV $84
line 124
;122:
;123:	case ID_TEAMARENA:
;124:		trap_Cvar_Set( "fs_game", "missionpack");
ADDRGP4 $85
ARGP4
ADDRGP4 $86
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 125
;125:		trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart;" );
CNSTI4 2
ARGI4
ADDRGP4 $87
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 126
;126:		break;
ADDRGP4 $76
JUMPV
LABELV $88
line 129
;127:
;128:	case ID_EXIT:
;129:		UI_ConfirmMenu( "EXIT GAME?", NULL, MainMenu_ExitAction );
ADDRGP4 $89
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 MainMenu_ExitAction
ARGP4
ADDRGP4 UI_ConfirmMenu
CALLV
pop
line 130
;130:		break;
LABELV $75
LABELV $76
line 132
;131:	}
;132:}
LABELV $72
endproc Main_MenuEvent 8 12
export MainMenu_Cache
proc MainMenu_Cache 4 4
line 140
;133:
;134:
;135:/*
;136:===============
;137:MainMenu_Cache
;138:===============
;139:*/
;140:void MainMenu_Cache( void ) {
line 141
;141:	s_main.bannerModel = trap_R_RegisterModel( MAIN_BANNER_MODEL );
ADDRGP4 $94
ARGP4
ADDRLP4 0
ADDRGP4 trap_R_RegisterModel
CALLI4
ASGNI4
ADDRGP4 s_main+864
ADDRLP4 0
INDIRI4
ASGNI4
line 142
;142:}
LABELV $92
endproc MainMenu_Cache 4 4
export ErrorMessage_Key
proc ErrorMessage_Key 0 8
line 145
;143:
;144:sfxHandle_t ErrorMessage_Key(int key)
;145:{
line 146
;146:	trap_Cvar_Set( "com_errorMessage", "" );
ADDRGP4 $96
ARGP4
ADDRGP4 $97
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 147
;147:	UI_MainMenu();
ADDRGP4 UI_MainMenu
CALLV
pop
line 148
;148:	return (menu_null_sound);
ADDRGP4 menu_null_sound
INDIRI4
RETI4
LABELV $95
endproc ErrorMessage_Key 0 8
lit
align 4
LABELV $99
byte 4 1056964608
byte 4 0
byte 4 0
byte 4 1065353216
code
proc Main_MenuDraw 576 28
line 158
;149:}
;150:
;151:/*
;152:===============
;153:Main_MenuDraw
;154:TTimo: this function is common to the main menu and errorMessage menu
;155:===============
;156:*/
;157:
;158:static void Main_MenuDraw( void ) {
line 165
;159:	refdef_t		refdef;
;160:	refEntity_t		ent;
;161:	vec3_t			origin;
;162:	vec3_t			angles;
;163:	float			adjust;
;164:	float			x, y, w, h;
;165:	vec4_t			color = {0.5, 0, 0, 1};
ADDRLP4 552
ADDRGP4 $99
INDIRB
ASGNB 16
line 169
;166:
;167:	// setup the refdef
;168:
;169:	memset( &refdef, 0, sizeof( refdef ) );
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 368
ARGI4
ADDRGP4 memset
CALLP4
pop
line 171
;170:
;171:	refdef.rdflags = RDF_NOWORLDMODEL;
ADDRLP4 0+76
CNSTI4 1
ASGNI4
line 173
;172:
;173:	AxisClear( refdef.viewaxis );
ADDRLP4 0+36
ARGP4
ADDRGP4 AxisClear
CALLV
pop
line 175
;174:
;175:	x = 0;
ADDRLP4 536
CNSTF4 0
ASGNF4
line 176
;176:	y = 0;
ADDRLP4 540
CNSTF4 0
ASGNF4
line 177
;177:	w = 640;
ADDRLP4 544
CNSTF4 1142947840
ASGNF4
line 178
;178:	h = 120;
ADDRLP4 548
CNSTF4 1123024896
ASGNF4
line 179
;179:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRLP4 536
ARGP4
ADDRLP4 540
ARGP4
ADDRLP4 544
ARGP4
ADDRLP4 548
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 180
;180:	refdef.x = x;
ADDRLP4 0
ADDRLP4 536
INDIRF4
CVFI4 4
ASGNI4
line 181
;181:	refdef.y = y;
ADDRLP4 0+4
ADDRLP4 540
INDIRF4
CVFI4 4
ASGNI4
line 182
;182:	refdef.width = w;
ADDRLP4 0+8
ADDRLP4 544
INDIRF4
CVFI4 4
ASGNI4
line 183
;183:	refdef.height = h;
ADDRLP4 0+12
ADDRLP4 548
INDIRF4
CVFI4 4
ASGNI4
line 185
;184:
;185:	adjust = 0; // JDC: Kenneth asked me to stop this 1.0 * sin( (float)uis.realtime / 1000 );
ADDRLP4 520
CNSTF4 0
ASGNF4
line 186
;186:	refdef.fov_x = 60 + adjust;
ADDRLP4 0+16
ADDRLP4 520
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 187
;187:	refdef.fov_y = 19.6875 + adjust;
ADDRLP4 0+20
ADDRLP4 520
INDIRF4
CNSTF4 1100840960
ADDF4
ASGNF4
line 189
;188:
;189:	refdef.time = uis.realtime;
ADDRLP4 0+72
ADDRGP4 uis+4
INDIRI4
ASGNI4
line 191
;190:
;191:	origin[0] = 300;
ADDRLP4 508
CNSTF4 1133903872
ASGNF4
line 192
;192:	origin[1] = 0;
ADDRLP4 508+4
CNSTF4 0
ASGNF4
line 193
;193:	origin[2] = -32;
ADDRLP4 508+8
CNSTF4 3254779904
ASGNF4
line 195
;194:
;195:	trap_R_ClearScene();
ADDRGP4 trap_R_ClearScene
CALLV
pop
line 199
;196:
;197:	// add the model
;198:
;199:	memset( &ent, 0, sizeof(ent) );
ADDRLP4 368
ARGP4
CNSTI4 0
ARGI4
CNSTI4 140
ARGI4
ADDRGP4 memset
CALLP4
pop
line 201
;200:
;201:	adjust = 5.0 * sin( (float)uis.realtime / 5000 );
ADDRGP4 uis+4
INDIRI4
CVIF4 4
CNSTF4 1167867904
DIVF4
ARGF4
ADDRLP4 568
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 520
CNSTF4 1084227584
ADDRLP4 568
INDIRF4
MULF4
ASGNF4
line 202
;202:	VectorSet( angles, 0, 180 + adjust, 0 );
ADDRLP4 524
CNSTF4 0
ASGNF4
ADDRLP4 524+4
ADDRLP4 520
INDIRF4
CNSTF4 1127481344
ADDF4
ASGNF4
ADDRLP4 524+8
CNSTF4 0
ASGNF4
line 203
;203:	AnglesToAxis( angles, ent.axis );
ADDRLP4 524
ARGP4
ADDRLP4 368+28
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 204
;204:	ent.hModel = s_main.bannerModel;
ADDRLP4 368+8
ADDRGP4 s_main+864
INDIRI4
ASGNI4
line 205
;205:	VectorCopy( origin, ent.origin );
ADDRLP4 368+68
ADDRLP4 508
INDIRB
ASGNB 12
line 206
;206:	VectorCopy( origin, ent.lightingOrigin );
ADDRLP4 368+12
ADDRLP4 508
INDIRB
ASGNB 12
line 207
;207:	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;
ADDRLP4 368+4
CNSTI4 192
ASGNI4
line 208
;208:	VectorCopy( ent.origin, ent.oldorigin );
ADDRLP4 368+84
ADDRLP4 368+68
INDIRB
ASGNB 12
line 210
;209:
;210:	trap_R_AddRefEntityToScene( &ent );
ADDRLP4 368
ARGP4
ADDRGP4 trap_R_AddRefEntityToScene
CALLV
pop
line 212
;211:
;212:	trap_R_RenderScene( &refdef );
ADDRLP4 0
ARGP4
ADDRGP4 trap_R_RenderScene
CALLV
pop
line 214
;213:	
;214:	if (strlen(s_errorMessage.errorMessage))
ADDRGP4 s_errorMessage+288
ARGP4
ADDRLP4 572
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 572
INDIRI4
CNSTI4 0
EQI4 $122
line 215
;215:	{
line 216
;216:		UI_DrawProportionalString_AutoWrapped( 320, 192, 600, 20, s_errorMessage.errorMessage, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
CNSTI4 320
ARGI4
CNSTI4 192
ARGI4
CNSTI4 600
ARGI4
CNSTI4 20
ARGI4
ADDRGP4 s_errorMessage+288
ARGP4
CNSTI4 2065
ARGI4
ADDRGP4 menu_text_color
ARGP4
ADDRGP4 UI_DrawProportionalString_AutoWrapped
CALLV
pop
line 217
;217:	}
ADDRGP4 $123
JUMPV
LABELV $122
line 219
;218:	else
;219:	{
line 221
;220:		// standard menu drawing
;221:		Menu_Draw( &s_main.menu );		
ADDRGP4 s_main
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 222
;222:	}
LABELV $123
line 224
;223:
;224:	if (uis.demoversion) {
ADDRGP4 uis+11440
INDIRI4
CNSTI4 0
EQI4 $126
line 225
;225:		UI_DrawProportionalString( 320, 372, "DEMO      FOR MATURE AUDIENCES      DEMO", UI_CENTER|UI_SMALLFONT, color );
CNSTI4 320
ARGI4
CNSTI4 372
ARGI4
ADDRGP4 $129
ARGP4
CNSTI4 17
ARGI4
ADDRLP4 552
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 226
;226:		UI_DrawString( 320, 400, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color );
CNSTI4 320
ARGI4
CNSTI4 400
ARGI4
ADDRGP4 $130
ARGP4
CNSTI4 17
ARGI4
ADDRLP4 552
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 227
;227:	} else {
ADDRGP4 $127
JUMPV
LABELV $126
line 228
;228:		UI_DrawString( 320, 450, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color );
CNSTI4 320
ARGI4
CNSTI4 450
ARGI4
ADDRGP4 $130
ARGP4
CNSTI4 17
ARGI4
ADDRLP4 552
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 229
;229:	}
LABELV $127
line 230
;230:}
LABELV $98
endproc Main_MenuDraw 576 28
proc UI_TeamArenaExists 2084 16
line 238
;231:
;232:
;233:/*
;234:===============
;235:UI_TeamArenaExists
;236:===============
;237:*/
;238:static qboolean UI_TeamArenaExists( void ) {
line 246
;239:	int		numdirs;
;240:	char	dirlist[2048];
;241:	char	*dirptr;
;242:  char  *descptr;
;243:	int		i;
;244:	int		dirlen;
;245:
;246:	numdirs = trap_FS_GetFileList( "$modlist", "", dirlist, sizeof(dirlist) );
ADDRGP4 $132
ARGP4
ADDRGP4 $97
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
line 247
;247:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 20
ASGNP4
line 248
;248:	for( i = 0; i < numdirs; i++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $136
JUMPV
LABELV $133
line 249
;249:		dirlen = strlen( dirptr ) + 1;
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 2072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 2072
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 250
;250:    descptr = dirptr + dirlen;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 251
;251:		if (Q_stricmp(dirptr, "missionpack") == 0) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $86
ARGP4
ADDRLP4 2076
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 2076
INDIRI4
CNSTI4 0
NEI4 $137
line 252
;252:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $131
JUMPV
LABELV $137
line 254
;253:		}
;254:    dirptr += dirlen + strlen(descptr) + 1;
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 2080
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ADDRLP4 2080
INDIRI4
ADDI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 255
;255:	}
LABELV $134
line 248
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $136
ADDRLP4 8
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $133
line 256
;256:	return qfalse;
CNSTI4 0
RETI4
LABELV $131
endproc UI_TeamArenaExists 2084 16
export UI_MainMenu
proc UI_MainMenu 40 12
line 269
;257:}
;258:
;259:
;260:/*
;261:===============
;262:UI_MainMenu
;263:
;264:The main menu only comes up when not in a game,
;265:so make sure that the attract loop server is down
;266:and that local cinematics are killed
;267:===============
;268:*/
;269:void UI_MainMenu( void ) {
line 271
;270:	int		y;
;271:	qboolean teamArena = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 272
;272:	int		style = UI_CENTER | UI_DROPSHADOW;
ADDRLP4 4
CNSTI4 2049
ASGNI4
line 274
;273:
;274:	trap_Cvar_Set( "sv_killserver", "1" );
ADDRGP4 $140
ARGP4
ADDRGP4 $141
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 276
;275:
;276:	if( !uis.demoversion && !ui_cdkeychecked.integer ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 uis+11440
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $142
ADDRGP4 ui_cdkeychecked+12
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $142
line 279
;277:		char	key[17];
;278:
;279:		trap_GetCDKey( key, sizeof(key) );
ADDRLP4 16
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 trap_GetCDKey
CALLV
pop
line 280
;280:		if( trap_VerifyCDKey( key, NULL ) == qfalse ) {
ADDRLP4 16
ARGP4
CNSTP4 0
ARGP4
ADDRLP4 36
ADDRGP4 trap_VerifyCDKey
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $146
line 281
;281:			UI_CDKeyMenu();
ADDRGP4 UI_CDKeyMenu
CALLV
pop
line 282
;282:			return;
ADDRGP4 $139
JUMPV
LABELV $146
line 284
;283:		}
;284:	}
LABELV $142
line 286
;285:	
;286:	memset( &s_main, 0 ,sizeof(mainmenu_t) );
ADDRGP4 s_main
ARGP4
CNSTI4 0
ARGI4
CNSTI4 868
ARGI4
ADDRGP4 memset
CALLP4
pop
line 287
;287:	memset( &s_errorMessage, 0 ,sizeof(errorMessage_t) );
ADDRGP4 s_errorMessage
ARGP4
CNSTI4 0
ARGI4
CNSTI4 4384
ARGI4
ADDRGP4 memset
CALLP4
pop
line 290
;288:
;289:	// com_errorMessage would need that too
;290:	MainMenu_Cache();
ADDRGP4 MainMenu_Cache
CALLV
pop
line 292
;291:	
;292:	trap_Cvar_VariableStringBuffer( "com_errorMessage", s_errorMessage.errorMessage, sizeof(s_errorMessage.errorMessage) );
ADDRGP4 $96
ARGP4
ADDRGP4 s_errorMessage+288
ARGP4
CNSTI4 4096
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 293
;293:	if (strlen(s_errorMessage.errorMessage))
ADDRGP4 s_errorMessage+288
ARGP4
ADDRLP4 16
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $150
line 294
;294:	{	
line 295
;295:		s_errorMessage.menu.draw = Main_MenuDraw;
ADDRGP4 s_errorMessage+268
ADDRGP4 Main_MenuDraw
ASGNP4
line 296
;296:		s_errorMessage.menu.key = ErrorMessage_Key;
ADDRGP4 s_errorMessage+272
ADDRGP4 ErrorMessage_Key
ASGNP4
line 297
;297:		s_errorMessage.menu.fullscreen = qtrue;
ADDRGP4 s_errorMessage+280
CNSTI4 1
ASGNI4
line 298
;298:		s_errorMessage.menu.wrapAround = qtrue;
ADDRGP4 s_errorMessage+276
CNSTI4 1
ASGNI4
line 299
;299:		s_errorMessage.menu.showlogo = qtrue;		
ADDRGP4 s_errorMessage+284
CNSTI4 1
ASGNI4
line 301
;300:
;301:		trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 302
;302:		uis.menusp = 0;
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
line 303
;303:		UI_PushMenu ( &s_errorMessage.menu );
ADDRGP4 s_errorMessage
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 305
;304:		
;305:		return;
ADDRGP4 $139
JUMPV
LABELV $150
line 308
;306:	}
;307:
;308:	s_main.menu.draw = Main_MenuDraw;
ADDRGP4 s_main+268
ADDRGP4 Main_MenuDraw
ASGNP4
line 309
;309:	s_main.menu.fullscreen = qtrue;
ADDRGP4 s_main+280
CNSTI4 1
ASGNI4
line 310
;310:	s_main.menu.wrapAround = qtrue;
ADDRGP4 s_main+276
CNSTI4 1
ASGNI4
line 311
;311:	s_main.menu.showlogo = qtrue;
ADDRGP4 s_main+284
CNSTI4 1
ASGNI4
line 313
;312:
;313:	y = 134;
ADDRLP4 0
CNSTI4 134
ASGNI4
line 314
;314:	s_main.singleplayer.generic.type		= MTYPE_PTEXT;
ADDRGP4 s_main+288
CNSTI4 9
ASGNI4
line 315
;315:	s_main.singleplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+288+44
CNSTU4 264
ASGNU4
line 316
;316:	s_main.singleplayer.generic.x			= 320;
ADDRGP4 s_main+288+12
CNSTI4 320
ASGNI4
line 317
;317:	s_main.singleplayer.generic.y			= y;
ADDRGP4 s_main+288+16
ADDRLP4 0
INDIRI4
ASGNI4
line 318
;318:	s_main.singleplayer.generic.id			= ID_SINGLEPLAYER;
ADDRGP4 s_main+288+8
CNSTI4 10
ASGNI4
line 319
;319:	s_main.singleplayer.generic.callback	= Main_MenuEvent; 
ADDRGP4 s_main+288+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 320
;320:	s_main.singleplayer.string				= "SINGLE PLAYER";
ADDRGP4 s_main+288+60
ADDRGP4 $176
ASGNP4
line 321
;321:	s_main.singleplayer.color				= color_red;
ADDRGP4 s_main+288+68
ADDRGP4 color_red
ASGNP4
line 322
;322:	s_main.singleplayer.style				= style;
ADDRGP4 s_main+288+64
ADDRLP4 4
INDIRI4
ASGNI4
line 324
;323:
;324:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 325
;325:	s_main.multiplayer.generic.type			= MTYPE_PTEXT;
ADDRGP4 s_main+360
CNSTI4 9
ASGNI4
line 326
;326:	s_main.multiplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+360+44
CNSTU4 264
ASGNU4
line 327
;327:	s_main.multiplayer.generic.x			= 320;
ADDRGP4 s_main+360+12
CNSTI4 320
ASGNI4
line 328
;328:	s_main.multiplayer.generic.y			= y;
ADDRGP4 s_main+360+16
ADDRLP4 0
INDIRI4
ASGNI4
line 329
;329:	s_main.multiplayer.generic.id			= ID_MULTIPLAYER;
ADDRGP4 s_main+360+8
CNSTI4 11
ASGNI4
line 330
;330:	s_main.multiplayer.generic.callback		= Main_MenuEvent; 
ADDRGP4 s_main+360+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 331
;331:	s_main.multiplayer.string				= "MULTIPLAYER";
ADDRGP4 s_main+360+60
ADDRGP4 $194
ASGNP4
line 332
;332:	s_main.multiplayer.color				= color_red;
ADDRGP4 s_main+360+68
ADDRGP4 color_red
ASGNP4
line 333
;333:	s_main.multiplayer.style				= style;
ADDRGP4 s_main+360+64
ADDRLP4 4
INDIRI4
ASGNI4
line 335
;334:
;335:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 336
;336:	s_main.setup.generic.type				= MTYPE_PTEXT;
ADDRGP4 s_main+432
CNSTI4 9
ASGNI4
line 337
;337:	s_main.setup.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+432+44
CNSTU4 264
ASGNU4
line 338
;338:	s_main.setup.generic.x					= 320;
ADDRGP4 s_main+432+12
CNSTI4 320
ASGNI4
line 339
;339:	s_main.setup.generic.y					= y;
ADDRGP4 s_main+432+16
ADDRLP4 0
INDIRI4
ASGNI4
line 340
;340:	s_main.setup.generic.id					= ID_SETUP;
ADDRGP4 s_main+432+8
CNSTI4 12
ASGNI4
line 341
;341:	s_main.setup.generic.callback			= Main_MenuEvent; 
ADDRGP4 s_main+432+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 342
;342:	s_main.setup.string						= "SETUP";
ADDRGP4 s_main+432+60
ADDRGP4 $212
ASGNP4
line 343
;343:	s_main.setup.color						= color_red;
ADDRGP4 s_main+432+68
ADDRGP4 color_red
ASGNP4
line 344
;344:	s_main.setup.style						= style;
ADDRGP4 s_main+432+64
ADDRLP4 4
INDIRI4
ASGNI4
line 346
;345:
;346:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 347
;347:	s_main.demos.generic.type				= MTYPE_PTEXT;
ADDRGP4 s_main+504
CNSTI4 9
ASGNI4
line 348
;348:	s_main.demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+504+44
CNSTU4 264
ASGNU4
line 349
;349:	s_main.demos.generic.x					= 320;
ADDRGP4 s_main+504+12
CNSTI4 320
ASGNI4
line 350
;350:	s_main.demos.generic.y					= y;
ADDRGP4 s_main+504+16
ADDRLP4 0
INDIRI4
ASGNI4
line 351
;351:	s_main.demos.generic.id					= ID_DEMOS;
ADDRGP4 s_main+504+8
CNSTI4 13
ASGNI4
line 352
;352:	s_main.demos.generic.callback			= Main_MenuEvent; 
ADDRGP4 s_main+504+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 353
;353:	s_main.demos.string						= "DEMOS";
ADDRGP4 s_main+504+60
ADDRGP4 $230
ASGNP4
line 354
;354:	s_main.demos.color						= color_red;
ADDRGP4 s_main+504+68
ADDRGP4 color_red
ASGNP4
line 355
;355:	s_main.demos.style						= style;
ADDRGP4 s_main+504+64
ADDRLP4 4
INDIRI4
ASGNI4
line 357
;356:
;357:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 358
;358:	s_main.cinematics.generic.type			= MTYPE_PTEXT;
ADDRGP4 s_main+576
CNSTI4 9
ASGNI4
line 359
;359:	s_main.cinematics.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+576+44
CNSTU4 264
ASGNU4
line 360
;360:	s_main.cinematics.generic.x				= 320;
ADDRGP4 s_main+576+12
CNSTI4 320
ASGNI4
line 361
;361:	s_main.cinematics.generic.y				= y;
ADDRGP4 s_main+576+16
ADDRLP4 0
INDIRI4
ASGNI4
line 362
;362:	s_main.cinematics.generic.id			= ID_CINEMATICS;
ADDRGP4 s_main+576+8
CNSTI4 14
ASGNI4
line 363
;363:	s_main.cinematics.generic.callback		= Main_MenuEvent; 
ADDRGP4 s_main+576+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 364
;364:	s_main.cinematics.string				= "CINEMATICS";
ADDRGP4 s_main+576+60
ADDRGP4 $248
ASGNP4
line 365
;365:	s_main.cinematics.color					= color_red;
ADDRGP4 s_main+576+68
ADDRGP4 color_red
ASGNP4
line 366
;366:	s_main.cinematics.style					= style;
ADDRGP4 s_main+576+64
ADDRLP4 4
INDIRI4
ASGNI4
line 368
;367:
;368:	if (UI_TeamArenaExists()) {
ADDRLP4 20
ADDRGP4 UI_TeamArenaExists
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $253
line 369
;369:		teamArena = qtrue;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 370
;370:		y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 371
;371:		s_main.teamArena.generic.type			= MTYPE_PTEXT;
ADDRGP4 s_main+648
CNSTI4 9
ASGNI4
line 372
;372:		s_main.teamArena.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+648+44
CNSTU4 264
ASGNU4
line 373
;373:		s_main.teamArena.generic.x				= 320;
ADDRGP4 s_main+648+12
CNSTI4 320
ASGNI4
line 374
;374:		s_main.teamArena.generic.y				= y;
ADDRGP4 s_main+648+16
ADDRLP4 0
INDIRI4
ASGNI4
line 375
;375:		s_main.teamArena.generic.id				= ID_TEAMARENA;
ADDRGP4 s_main+648+8
CNSTI4 15
ASGNI4
line 376
;376:		s_main.teamArena.generic.callback		= Main_MenuEvent; 
ADDRGP4 s_main+648+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 377
;377:		s_main.teamArena.string					= "TEAM ARENA";
ADDRGP4 s_main+648+60
ADDRGP4 $268
ASGNP4
line 378
;378:		s_main.teamArena.color					= color_red;
ADDRGP4 s_main+648+68
ADDRGP4 color_red
ASGNP4
line 379
;379:		s_main.teamArena.style					= style;
ADDRGP4 s_main+648+64
ADDRLP4 4
INDIRI4
ASGNI4
line 380
;380:	}
LABELV $253
line 382
;381:
;382:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 383
;383:	s_main.mods.generic.type			= MTYPE_PTEXT;
ADDRGP4 s_main+720
CNSTI4 9
ASGNI4
line 384
;384:	s_main.mods.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+720+44
CNSTU4 264
ASGNU4
line 385
;385:	s_main.mods.generic.x				= 320;
ADDRGP4 s_main+720+12
CNSTI4 320
ASGNI4
line 386
;386:	s_main.mods.generic.y				= y;
ADDRGP4 s_main+720+16
ADDRLP4 0
INDIRI4
ASGNI4
line 387
;387:	s_main.mods.generic.id				= ID_MODS;
ADDRGP4 s_main+720+8
CNSTI4 16
ASGNI4
line 388
;388:	s_main.mods.generic.callback		= Main_MenuEvent; 
ADDRGP4 s_main+720+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 389
;389:	s_main.mods.string					= "MODS";
ADDRGP4 s_main+720+60
ADDRGP4 $286
ASGNP4
line 390
;390:	s_main.mods.color					= color_red;
ADDRGP4 s_main+720+68
ADDRGP4 color_red
ASGNP4
line 391
;391:	s_main.mods.style					= style;
ADDRGP4 s_main+720+64
ADDRLP4 4
INDIRI4
ASGNI4
line 393
;392:
;393:	y += MAIN_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 394
;394:	s_main.exit.generic.type				= MTYPE_PTEXT;
ADDRGP4 s_main+792
CNSTI4 9
ASGNI4
line 395
;395:	s_main.exit.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_main+792+44
CNSTU4 264
ASGNU4
line 396
;396:	s_main.exit.generic.x					= 320;
ADDRGP4 s_main+792+12
CNSTI4 320
ASGNI4
line 397
;397:	s_main.exit.generic.y					= y;
ADDRGP4 s_main+792+16
ADDRLP4 0
INDIRI4
ASGNI4
line 398
;398:	s_main.exit.generic.id					= ID_EXIT;
ADDRGP4 s_main+792+8
CNSTI4 17
ASGNI4
line 399
;399:	s_main.exit.generic.callback			= Main_MenuEvent; 
ADDRGP4 s_main+792+48
ADDRGP4 Main_MenuEvent
ASGNP4
line 400
;400:	s_main.exit.string						= "EXIT";
ADDRGP4 s_main+792+60
ADDRGP4 $304
ASGNP4
line 401
;401:	s_main.exit.color						= color_red;
ADDRGP4 s_main+792+68
ADDRGP4 color_red
ASGNP4
line 402
;402:	s_main.exit.style						= style;
ADDRGP4 s_main+792+64
ADDRLP4 4
INDIRI4
ASGNI4
line 404
;403:
;404:	Menu_AddItem( &s_main.menu,	&s_main.singleplayer );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 405
;405:	Menu_AddItem( &s_main.menu,	&s_main.multiplayer );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 406
;406:	Menu_AddItem( &s_main.menu,	&s_main.setup );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+432
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 407
;407:	Menu_AddItem( &s_main.menu,	&s_main.demos );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+504
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 408
;408:	Menu_AddItem( &s_main.menu,	&s_main.cinematics );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+576
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 409
;409:	if (teamArena) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $314
line 410
;410:		Menu_AddItem( &s_main.menu,	&s_main.teamArena );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+648
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 411
;411:	}
LABELV $314
line 412
;412:	Menu_AddItem( &s_main.menu,	&s_main.mods );
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+720
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 413
;413:	Menu_AddItem( &s_main.menu,	&s_main.exit );             
ADDRGP4 s_main
ARGP4
ADDRGP4 s_main+792
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 415
;414:
;415:	trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 416
;416:	uis.menusp = 0;
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
line 417
;417:	UI_PushMenu ( &s_main.menu );
ADDRGP4 s_main
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 419
;418:		
;419:}
LABELV $139
endproc UI_MainMenu 40 12
bss
align 4
LABELV s_errorMessage
skip 4384
align 4
LABELV s_main
skip 868
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
byte 1 69
byte 1 88
byte 1 73
byte 1 84
byte 1 0
align 1
LABELV $286
byte 1 77
byte 1 79
byte 1 68
byte 1 83
byte 1 0
align 1
LABELV $268
byte 1 84
byte 1 69
byte 1 65
byte 1 77
byte 1 32
byte 1 65
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 0
align 1
LABELV $248
byte 1 67
byte 1 73
byte 1 78
byte 1 69
byte 1 77
byte 1 65
byte 1 84
byte 1 73
byte 1 67
byte 1 83
byte 1 0
align 1
LABELV $230
byte 1 68
byte 1 69
byte 1 77
byte 1 79
byte 1 83
byte 1 0
align 1
LABELV $212
byte 1 83
byte 1 69
byte 1 84
byte 1 85
byte 1 80
byte 1 0
align 1
LABELV $194
byte 1 77
byte 1 85
byte 1 76
byte 1 84
byte 1 73
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $176
byte 1 83
byte 1 73
byte 1 78
byte 1 71
byte 1 76
byte 1 69
byte 1 32
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $141
byte 1 49
byte 1 0
align 1
LABELV $140
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
LABELV $132
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
LABELV $130
byte 1 81
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 32
byte 1 73
byte 1 73
byte 1 73
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 40
byte 1 99
byte 1 41
byte 1 32
byte 1 49
byte 1 57
byte 1 57
byte 1 57
byte 1 45
byte 1 50
byte 1 48
byte 1 48
byte 1 48
byte 1 44
byte 1 32
byte 1 73
byte 1 100
byte 1 32
byte 1 83
byte 1 111
byte 1 102
byte 1 116
byte 1 119
byte 1 97
byte 1 114
byte 1 101
byte 1 44
byte 1 32
byte 1 73
byte 1 110
byte 1 99
byte 1 46
byte 1 32
byte 1 32
byte 1 65
byte 1 108
byte 1 108
byte 1 32
byte 1 82
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 115
byte 1 32
byte 1 82
byte 1 101
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $129
byte 1 68
byte 1 69
byte 1 77
byte 1 79
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 70
byte 1 79
byte 1 82
byte 1 32
byte 1 77
byte 1 65
byte 1 84
byte 1 85
byte 1 82
byte 1 69
byte 1 32
byte 1 65
byte 1 85
byte 1 68
byte 1 73
byte 1 69
byte 1 78
byte 1 67
byte 1 69
byte 1 83
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 68
byte 1 69
byte 1 77
byte 1 79
byte 1 0
align 1
LABELV $97
byte 1 0
align 1
LABELV $96
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
LABELV $94
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 109
byte 1 97
byte 1 112
byte 1 111
byte 1 98
byte 1 106
byte 1 101
byte 1 99
byte 1 116
byte 1 115
byte 1 47
byte 1 98
byte 1 97
byte 1 110
byte 1 110
byte 1 101
byte 1 114
byte 1 47
byte 1 98
byte 1 97
byte 1 110
byte 1 110
byte 1 101
byte 1 114
byte 1 53
byte 1 46
byte 1 109
byte 1 100
byte 1 51
byte 1 0
align 1
LABELV $89
byte 1 69
byte 1 88
byte 1 73
byte 1 84
byte 1 32
byte 1 71
byte 1 65
byte 1 77
byte 1 69
byte 1 63
byte 1 0
align 1
LABELV $87
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
LABELV $86
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
LABELV $85
byte 1 102
byte 1 115
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
