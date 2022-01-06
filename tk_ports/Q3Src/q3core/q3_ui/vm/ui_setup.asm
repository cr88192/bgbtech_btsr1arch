code
proc Setup_ResetDefaults_Action 0 8
file "../ui_setup.c"
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
;26:SETUP MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define SETUP_MENU_VERTICAL_SPACING		34
;36:
;37:#define ART_BACK0		"menu/art/back_0"
;38:#define ART_BACK1		"menu/art/back_1"	
;39:#define ART_FRAMEL		"menu/art/frame2_l"
;40:#define ART_FRAMER		"menu/art/frame1_r"
;41:
;42:#define ID_CUSTOMIZEPLAYER		10
;43:#define ID_CUSTOMIZECONTROLS	11
;44:#define ID_SYSTEMCONFIG			12
;45:#define ID_GAME					13
;46:#define ID_CDKEY				14
;47:#define ID_LOAD					15
;48:#define ID_SAVE					16
;49:#define ID_DEFAULTS				17
;50:#define ID_BACK					18
;51:
;52:
;53:typedef struct {
;54:	menuframework_s	menu;
;55:
;56:	menutext_s		banner;
;57:	menubitmap_s	framel;
;58:	menubitmap_s	framer;
;59:	menutext_s		setupplayer;
;60:	menutext_s		setupcontrols;
;61:	menutext_s		setupsystem;
;62:	menutext_s		game;
;63:	menutext_s		cdkey;
;64://	menutext_s		load;
;65://	menutext_s		save;
;66:	menutext_s		defaults;
;67:	menubitmap_s	back;
;68:} setupMenuInfo_t;
;69:
;70:static setupMenuInfo_t	setupMenuInfo;
;71:
;72:
;73:/*
;74:=================
;75:Setup_ResetDefaults_Action
;76:=================
;77:*/
;78:static void Setup_ResetDefaults_Action( qboolean result ) {
line 79
;79:	if( !result ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $69
line 80
;80:		return;
ADDRGP4 $68
JUMPV
LABELV $69
line 82
;81:	}
;82:	trap_Cmd_ExecuteText( EXEC_APPEND, "exec default.cfg\n");
CNSTI4 2
ARGI4
ADDRGP4 $71
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 83
;83:	trap_Cmd_ExecuteText( EXEC_APPEND, "cvar_restart\n");
CNSTI4 2
ARGI4
ADDRGP4 $72
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 84
;84:	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
CNSTI4 2
ARGI4
ADDRGP4 $73
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 85
;85:}
LABELV $68
endproc Setup_ResetDefaults_Action 0 8
proc Setup_ResetDefaults_Draw 0 20
line 93
;86:
;87:
;88:/*
;89:=================
;90:Setup_ResetDefaults_Draw
;91:=================
;92:*/
;93:static void Setup_ResetDefaults_Draw( void ) {
line 94
;94:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 0, "WARNING: This will reset *ALL*", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 356
ARGI4
ADDRGP4 $75
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 95
;95:	UI_DrawProportionalString( SCREEN_WIDTH/2, 356 + PROP_HEIGHT * 1, "options to their default values.", UI_CENTER|UI_SMALLFONT, color_yellow );
CNSTI4 320
ARGI4
CNSTI4 383
ARGI4
ADDRGP4 $76
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_yellow
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 96
;96:}
LABELV $74
endproc Setup_ResetDefaults_Draw 0 20
proc UI_SetupMenu_Event 8 12
line 104
;97:
;98:
;99:/*
;100:===============
;101:UI_SetupMenu_Event
;102:===============
;103:*/
;104:static void UI_SetupMenu_Event( void *ptr, int event ) {
line 105
;105:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $78
line 106
;106:		return;
ADDRGP4 $77
JUMPV
LABELV $78
line 109
;107:	}
;108:
;109:	switch( ((menucommon_s*)ptr)->id ) {
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
LTI4 $80
ADDRLP4 0
INDIRI4
CNSTI4 18
GTI4 $80
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $91-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $91
address $83
address $84
address $85
address $86
address $87
address $80
address $80
address $88
address $90
code
LABELV $83
line 111
;110:	case ID_CUSTOMIZEPLAYER:
;111:		UI_PlayerSettingsMenu();
ADDRGP4 UI_PlayerSettingsMenu
CALLV
pop
line 112
;112:		break;
ADDRGP4 $81
JUMPV
LABELV $84
line 115
;113:
;114:	case ID_CUSTOMIZECONTROLS:
;115:		UI_ControlsMenu();
ADDRGP4 UI_ControlsMenu
CALLV
pop
line 116
;116:		break;
ADDRGP4 $81
JUMPV
LABELV $85
line 119
;117:
;118:	case ID_SYSTEMCONFIG:
;119:		UI_GraphicsOptionsMenu();
ADDRGP4 UI_GraphicsOptionsMenu
CALLV
pop
line 120
;120:		break;
ADDRGP4 $81
JUMPV
LABELV $86
line 123
;121:
;122:	case ID_GAME:
;123:		UI_PreferencesMenu();
ADDRGP4 UI_PreferencesMenu
CALLV
pop
line 124
;124:		break;
ADDRGP4 $81
JUMPV
LABELV $87
line 127
;125:
;126:	case ID_CDKEY:
;127:		UI_CDKeyMenu();
ADDRGP4 UI_CDKeyMenu
CALLV
pop
line 128
;128:		break;
ADDRGP4 $81
JUMPV
LABELV $88
line 139
;129:
;130://	case ID_LOAD:
;131://		UI_LoadConfigMenu();
;132://		break;
;133:
;134://	case ID_SAVE:
;135://		UI_SaveConfigMenu();
;136://		break;
;137:
;138:	case ID_DEFAULTS:
;139:		UI_ConfirmMenu( "SET TO DEFAULTS?", Setup_ResetDefaults_Draw, Setup_ResetDefaults_Action );
ADDRGP4 $89
ARGP4
ADDRGP4 Setup_ResetDefaults_Draw
ARGP4
ADDRGP4 Setup_ResetDefaults_Action
ARGP4
ADDRGP4 UI_ConfirmMenu
CALLV
pop
line 140
;140:		break;
ADDRGP4 $81
JUMPV
LABELV $90
line 143
;141:
;142:	case ID_BACK:
;143:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 144
;144:		break;
LABELV $80
LABELV $81
line 146
;145:	}
;146:}
LABELV $77
endproc UI_SetupMenu_Event 8 12
proc UI_SetupMenu_Init 12 12
line 154
;147:
;148:
;149:/*
;150:===============
;151:UI_SetupMenu_Init
;152:===============
;153:*/
;154:static void UI_SetupMenu_Init( void ) {
line 157
;155:	int				y;
;156:
;157:	UI_SetupMenu_Cache();
ADDRGP4 UI_SetupMenu_Cache
CALLV
pop
line 159
;158:
;159:	memset( &setupMenuInfo, 0, sizeof(setupMenuInfo) );
ADDRGP4 setupMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1056
ARGI4
ADDRGP4 memset
CALLP4
pop
line 160
;160:	setupMenuInfo.menu.wrapAround = qtrue;
ADDRGP4 setupMenuInfo+276
CNSTI4 1
ASGNI4
line 161
;161:	setupMenuInfo.menu.fullscreen = qtrue;
ADDRGP4 setupMenuInfo+280
CNSTI4 1
ASGNI4
line 163
;162:
;163:	setupMenuInfo.banner.generic.type				= MTYPE_BTEXT;
ADDRGP4 setupMenuInfo+288
CNSTI4 10
ASGNI4
line 164
;164:	setupMenuInfo.banner.generic.x					= 320;
ADDRGP4 setupMenuInfo+288+12
CNSTI4 320
ASGNI4
line 165
;165:	setupMenuInfo.banner.generic.y					= 16;
ADDRGP4 setupMenuInfo+288+16
CNSTI4 16
ASGNI4
line 166
;166:	setupMenuInfo.banner.string						= "SETUP";
ADDRGP4 setupMenuInfo+288+60
ADDRGP4 $103
ASGNP4
line 167
;167:	setupMenuInfo.banner.color						= color_white;
ADDRGP4 setupMenuInfo+288+68
ADDRGP4 color_white
ASGNP4
line 168
;168:	setupMenuInfo.banner.style						= UI_CENTER;
ADDRGP4 setupMenuInfo+288+64
CNSTI4 1
ASGNI4
line 170
;169:
;170:	setupMenuInfo.framel.generic.type				= MTYPE_BITMAP;
ADDRGP4 setupMenuInfo+360
CNSTI4 6
ASGNI4
line 171
;171:	setupMenuInfo.framel.generic.name				= ART_FRAMEL;
ADDRGP4 setupMenuInfo+360+4
ADDRGP4 $111
ASGNP4
line 172
;172:	setupMenuInfo.framel.generic.flags				= QMF_INACTIVE;
ADDRGP4 setupMenuInfo+360+44
CNSTU4 16384
ASGNU4
line 173
;173:	setupMenuInfo.framel.generic.x					= 0;  
ADDRGP4 setupMenuInfo+360+12
CNSTI4 0
ASGNI4
line 174
;174:	setupMenuInfo.framel.generic.y					= 78;
ADDRGP4 setupMenuInfo+360+16
CNSTI4 78
ASGNI4
line 175
;175:	setupMenuInfo.framel.width  					= 256;
ADDRGP4 setupMenuInfo+360+76
CNSTI4 256
ASGNI4
line 176
;176:	setupMenuInfo.framel.height  					= 329;
ADDRGP4 setupMenuInfo+360+80
CNSTI4 329
ASGNI4
line 178
;177:
;178:	setupMenuInfo.framer.generic.type				= MTYPE_BITMAP;
ADDRGP4 setupMenuInfo+448
CNSTI4 6
ASGNI4
line 179
;179:	setupMenuInfo.framer.generic.name				= ART_FRAMER;
ADDRGP4 setupMenuInfo+448+4
ADDRGP4 $125
ASGNP4
line 180
;180:	setupMenuInfo.framer.generic.flags				= QMF_INACTIVE;
ADDRGP4 setupMenuInfo+448+44
CNSTU4 16384
ASGNU4
line 181
;181:	setupMenuInfo.framer.generic.x					= 376;
ADDRGP4 setupMenuInfo+448+12
CNSTI4 376
ASGNI4
line 182
;182:	setupMenuInfo.framer.generic.y					= 76;
ADDRGP4 setupMenuInfo+448+16
CNSTI4 76
ASGNI4
line 183
;183:	setupMenuInfo.framer.width  					= 256;
ADDRGP4 setupMenuInfo+448+76
CNSTI4 256
ASGNI4
line 184
;184:	setupMenuInfo.framer.height  					= 334;
ADDRGP4 setupMenuInfo+448+80
CNSTI4 334
ASGNI4
line 186
;185:
;186:	y = 134;
ADDRLP4 0
CNSTI4 134
ASGNI4
line 187
;187:	setupMenuInfo.setupplayer.generic.type			= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+536
CNSTI4 9
ASGNI4
line 188
;188:	setupMenuInfo.setupplayer.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+536+44
CNSTU4 264
ASGNU4
line 189
;189:	setupMenuInfo.setupplayer.generic.x				= 320;
ADDRGP4 setupMenuInfo+536+12
CNSTI4 320
ASGNI4
line 190
;190:	setupMenuInfo.setupplayer.generic.y				= y;
ADDRGP4 setupMenuInfo+536+16
ADDRLP4 0
INDIRI4
ASGNI4
line 191
;191:	setupMenuInfo.setupplayer.generic.id			= ID_CUSTOMIZEPLAYER;
ADDRGP4 setupMenuInfo+536+8
CNSTI4 10
ASGNI4
line 192
;192:	setupMenuInfo.setupplayer.generic.callback		= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+536+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 193
;193:	setupMenuInfo.setupplayer.string				= "PLAYER";
ADDRGP4 setupMenuInfo+536+60
ADDRGP4 $149
ASGNP4
line 194
;194:	setupMenuInfo.setupplayer.color					= color_red;
ADDRGP4 setupMenuInfo+536+68
ADDRGP4 color_red
ASGNP4
line 195
;195:	setupMenuInfo.setupplayer.style					= UI_CENTER;
ADDRGP4 setupMenuInfo+536+64
CNSTI4 1
ASGNI4
line 197
;196:
;197:	y += SETUP_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 198
;198:	setupMenuInfo.setupcontrols.generic.type		= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+608
CNSTI4 9
ASGNI4
line 199
;199:	setupMenuInfo.setupcontrols.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+608+44
CNSTU4 264
ASGNU4
line 200
;200:	setupMenuInfo.setupcontrols.generic.x			= 320;
ADDRGP4 setupMenuInfo+608+12
CNSTI4 320
ASGNI4
line 201
;201:	setupMenuInfo.setupcontrols.generic.y			= y;
ADDRGP4 setupMenuInfo+608+16
ADDRLP4 0
INDIRI4
ASGNI4
line 202
;202:	setupMenuInfo.setupcontrols.generic.id			= ID_CUSTOMIZECONTROLS;
ADDRGP4 setupMenuInfo+608+8
CNSTI4 11
ASGNI4
line 203
;203:	setupMenuInfo.setupcontrols.generic.callback	= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+608+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 204
;204:	setupMenuInfo.setupcontrols.string				= "CONTROLS";
ADDRGP4 setupMenuInfo+608+60
ADDRGP4 $167
ASGNP4
line 205
;205:	setupMenuInfo.setupcontrols.color				= color_red;
ADDRGP4 setupMenuInfo+608+68
ADDRGP4 color_red
ASGNP4
line 206
;206:	setupMenuInfo.setupcontrols.style				= UI_CENTER;
ADDRGP4 setupMenuInfo+608+64
CNSTI4 1
ASGNI4
line 208
;207:
;208:	y += SETUP_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 209
;209:	setupMenuInfo.setupsystem.generic.type			= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+680
CNSTI4 9
ASGNI4
line 210
;210:	setupMenuInfo.setupsystem.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+680+44
CNSTU4 264
ASGNU4
line 211
;211:	setupMenuInfo.setupsystem.generic.x				= 320;
ADDRGP4 setupMenuInfo+680+12
CNSTI4 320
ASGNI4
line 212
;212:	setupMenuInfo.setupsystem.generic.y				= y;
ADDRGP4 setupMenuInfo+680+16
ADDRLP4 0
INDIRI4
ASGNI4
line 213
;213:	setupMenuInfo.setupsystem.generic.id			= ID_SYSTEMCONFIG;
ADDRGP4 setupMenuInfo+680+8
CNSTI4 12
ASGNI4
line 214
;214:	setupMenuInfo.setupsystem.generic.callback		= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+680+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 215
;215:	setupMenuInfo.setupsystem.string				= "SYSTEM";
ADDRGP4 setupMenuInfo+680+60
ADDRGP4 $185
ASGNP4
line 216
;216:	setupMenuInfo.setupsystem.color					= color_red;
ADDRGP4 setupMenuInfo+680+68
ADDRGP4 color_red
ASGNP4
line 217
;217:	setupMenuInfo.setupsystem.style					= UI_CENTER;
ADDRGP4 setupMenuInfo+680+64
CNSTI4 1
ASGNI4
line 219
;218:
;219:	y += SETUP_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 220
;220:	setupMenuInfo.game.generic.type					= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+752
CNSTI4 9
ASGNI4
line 221
;221:	setupMenuInfo.game.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+752+44
CNSTU4 264
ASGNU4
line 222
;222:	setupMenuInfo.game.generic.x					= 320;
ADDRGP4 setupMenuInfo+752+12
CNSTI4 320
ASGNI4
line 223
;223:	setupMenuInfo.game.generic.y					= y;
ADDRGP4 setupMenuInfo+752+16
ADDRLP4 0
INDIRI4
ASGNI4
line 224
;224:	setupMenuInfo.game.generic.id					= ID_GAME;
ADDRGP4 setupMenuInfo+752+8
CNSTI4 13
ASGNI4
line 225
;225:	setupMenuInfo.game.generic.callback				= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+752+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 226
;226:	setupMenuInfo.game.string						= "GAME OPTIONS";
ADDRGP4 setupMenuInfo+752+60
ADDRGP4 $203
ASGNP4
line 227
;227:	setupMenuInfo.game.color						= color_red;
ADDRGP4 setupMenuInfo+752+68
ADDRGP4 color_red
ASGNP4
line 228
;228:	setupMenuInfo.game.style						= UI_CENTER;
ADDRGP4 setupMenuInfo+752+64
CNSTI4 1
ASGNI4
line 230
;229:
;230:	y += SETUP_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 231
;231:	setupMenuInfo.cdkey.generic.type				= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+824
CNSTI4 9
ASGNI4
line 232
;232:	setupMenuInfo.cdkey.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+824+44
CNSTU4 264
ASGNU4
line 233
;233:	setupMenuInfo.cdkey.generic.x					= 320;
ADDRGP4 setupMenuInfo+824+12
CNSTI4 320
ASGNI4
line 234
;234:	setupMenuInfo.cdkey.generic.y					= y;
ADDRGP4 setupMenuInfo+824+16
ADDRLP4 0
INDIRI4
ASGNI4
line 235
;235:	setupMenuInfo.cdkey.generic.id					= ID_CDKEY;
ADDRGP4 setupMenuInfo+824+8
CNSTI4 14
ASGNI4
line 236
;236:	setupMenuInfo.cdkey.generic.callback			= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+824+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 237
;237:	setupMenuInfo.cdkey.string						= "CD Key";
ADDRGP4 setupMenuInfo+824+60
ADDRGP4 $221
ASGNP4
line 238
;238:	setupMenuInfo.cdkey.color						= color_red;
ADDRGP4 setupMenuInfo+824+68
ADDRGP4 color_red
ASGNP4
line 239
;239:	setupMenuInfo.cdkey.style						= UI_CENTER;
ADDRGP4 setupMenuInfo+824+64
CNSTI4 1
ASGNI4
line 241
;240:
;241:	if( !trap_Cvar_VariableValue( "cl_paused" ) ) {
ADDRGP4 $228
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 4
INDIRF4
CNSTF4 0
NEF4 $226
line 266
;242:#if 0
;243:		y += SETUP_MENU_VERTICAL_SPACING;
;244:		setupMenuInfo.load.generic.type					= MTYPE_PTEXT;
;245:		setupMenuInfo.load.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
;246:		setupMenuInfo.load.generic.x					= 320;
;247:		setupMenuInfo.load.generic.y					= y;
;248:		setupMenuInfo.load.generic.id					= ID_LOAD;
;249:		setupMenuInfo.load.generic.callback				= UI_SetupMenu_Event; 
;250:		setupMenuInfo.load.string						= "LOAD";
;251:		setupMenuInfo.load.color						= color_red;
;252:		setupMenuInfo.load.style						= UI_CENTER;
;253:
;254:		y += SETUP_MENU_VERTICAL_SPACING;
;255:		setupMenuInfo.save.generic.type					= MTYPE_PTEXT;
;256:		setupMenuInfo.save.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
;257:		setupMenuInfo.save.generic.x					= 320;
;258:		setupMenuInfo.save.generic.y					= y;
;259:		setupMenuInfo.save.generic.id					= ID_SAVE;
;260:		setupMenuInfo.save.generic.callback				= UI_SetupMenu_Event; 
;261:		setupMenuInfo.save.string						= "SAVE";
;262:		setupMenuInfo.save.color						= color_red;
;263:		setupMenuInfo.save.style						= UI_CENTER;
;264:#endif
;265:
;266:		y += SETUP_MENU_VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 34
ADDI4
ASGNI4
line 267
;267:		setupMenuInfo.defaults.generic.type				= MTYPE_PTEXT;
ADDRGP4 setupMenuInfo+896
CNSTI4 9
ASGNI4
line 268
;268:		setupMenuInfo.defaults.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+896+44
CNSTU4 264
ASGNU4
line 269
;269:		setupMenuInfo.defaults.generic.x				= 320;
ADDRGP4 setupMenuInfo+896+12
CNSTI4 320
ASGNI4
line 270
;270:		setupMenuInfo.defaults.generic.y				= y;
ADDRGP4 setupMenuInfo+896+16
ADDRLP4 0
INDIRI4
ASGNI4
line 271
;271:		setupMenuInfo.defaults.generic.id				= ID_DEFAULTS;
ADDRGP4 setupMenuInfo+896+8
CNSTI4 17
ASGNI4
line 272
;272:		setupMenuInfo.defaults.generic.callback			= UI_SetupMenu_Event; 
ADDRGP4 setupMenuInfo+896+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 273
;273:		setupMenuInfo.defaults.string					= "DEFAULTS";
ADDRGP4 setupMenuInfo+896+60
ADDRGP4 $242
ASGNP4
line 274
;274:		setupMenuInfo.defaults.color					= color_red;
ADDRGP4 setupMenuInfo+896+68
ADDRGP4 color_red
ASGNP4
line 275
;275:		setupMenuInfo.defaults.style					= UI_CENTER;
ADDRGP4 setupMenuInfo+896+64
CNSTI4 1
ASGNI4
line 276
;276:	}
LABELV $226
line 278
;277:
;278:	setupMenuInfo.back.generic.type					= MTYPE_BITMAP;
ADDRGP4 setupMenuInfo+968
CNSTI4 6
ASGNI4
line 279
;279:	setupMenuInfo.back.generic.name					= ART_BACK0;
ADDRGP4 setupMenuInfo+968+4
ADDRGP4 $250
ASGNP4
line 280
;280:	setupMenuInfo.back.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 setupMenuInfo+968+44
CNSTU4 260
ASGNU4
line 281
;281:	setupMenuInfo.back.generic.id					= ID_BACK;
ADDRGP4 setupMenuInfo+968+8
CNSTI4 18
ASGNI4
line 282
;282:	setupMenuInfo.back.generic.callback				= UI_SetupMenu_Event;
ADDRGP4 setupMenuInfo+968+48
ADDRGP4 UI_SetupMenu_Event
ASGNP4
line 283
;283:	setupMenuInfo.back.generic.x					= 0;
ADDRGP4 setupMenuInfo+968+12
CNSTI4 0
ASGNI4
line 284
;284:	setupMenuInfo.back.generic.y					= 480-64;
ADDRGP4 setupMenuInfo+968+16
CNSTI4 416
ASGNI4
line 285
;285:	setupMenuInfo.back.width						= 128;
ADDRGP4 setupMenuInfo+968+76
CNSTI4 128
ASGNI4
line 286
;286:	setupMenuInfo.back.height						= 64;
ADDRGP4 setupMenuInfo+968+80
CNSTI4 64
ASGNI4
line 287
;287:	setupMenuInfo.back.focuspic						= ART_BACK1;
ADDRGP4 setupMenuInfo+968+60
ADDRGP4 $267
ASGNP4
line 289
;288:
;289:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.banner );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 290
;290:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.framel );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 291
;291:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.framer );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 292
;292:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupplayer );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 293
;293:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupcontrols );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+608
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 294
;294:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupsystem );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 295
;295:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.game );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+752
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 296
;296:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.cdkey );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+824
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 299
;297://	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.load );
;298://	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.save );
;299:	if( !trap_Cvar_VariableValue( "cl_paused" ) ) {
ADDRGP4 $228
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
INDIRF4
CNSTF4 0
NEF4 $276
line 300
;300:		Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.defaults );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+896
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 301
;301:	}
LABELV $276
line 302
;302:	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.back );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 setupMenuInfo+968
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 303
;303:}
LABELV $93
endproc UI_SetupMenu_Init 12 12
export UI_SetupMenu_Cache
proc UI_SetupMenu_Cache 0 4
line 311
;304:
;305:
;306:/*
;307:=================
;308:UI_SetupMenu_Cache
;309:=================
;310:*/
;311:void UI_SetupMenu_Cache( void ) {
line 312
;312:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $250
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 313
;313:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $267
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 314
;314:	trap_R_RegisterShaderNoMip( ART_FRAMEL );
ADDRGP4 $111
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 315
;315:	trap_R_RegisterShaderNoMip( ART_FRAMER );
ADDRGP4 $125
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 316
;316:}
LABELV $280
endproc UI_SetupMenu_Cache 0 4
export UI_SetupMenu
proc UI_SetupMenu 0 4
line 324
;317:
;318:
;319:/*
;320:===============
;321:UI_SetupMenu
;322:===============
;323:*/
;324:void UI_SetupMenu( void ) {
line 325
;325:	UI_SetupMenu_Init();
ADDRGP4 UI_SetupMenu_Init
CALLV
pop
line 326
;326:	UI_PushMenu( &setupMenuInfo.menu );
ADDRGP4 setupMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 327
;327:}
LABELV $281
endproc UI_SetupMenu 0 4
bss
align 4
LABELV setupMenuInfo
skip 1056
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
LABELV $267
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
LABELV $250
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
LABELV $242
byte 1 68
byte 1 69
byte 1 70
byte 1 65
byte 1 85
byte 1 76
byte 1 84
byte 1 83
byte 1 0
align 1
LABELV $228
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
LABELV $221
byte 1 67
byte 1 68
byte 1 32
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $203
byte 1 71
byte 1 65
byte 1 77
byte 1 69
byte 1 32
byte 1 79
byte 1 80
byte 1 84
byte 1 73
byte 1 79
byte 1 78
byte 1 83
byte 1 0
align 1
LABELV $185
byte 1 83
byte 1 89
byte 1 83
byte 1 84
byte 1 69
byte 1 77
byte 1 0
align 1
LABELV $167
byte 1 67
byte 1 79
byte 1 78
byte 1 84
byte 1 82
byte 1 79
byte 1 76
byte 1 83
byte 1 0
align 1
LABELV $149
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $125
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
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 49
byte 1 95
byte 1 114
byte 1 0
align 1
LABELV $111
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
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 50
byte 1 95
byte 1 108
byte 1 0
align 1
LABELV $103
byte 1 83
byte 1 69
byte 1 84
byte 1 85
byte 1 80
byte 1 0
align 1
LABELV $89
byte 1 83
byte 1 69
byte 1 84
byte 1 32
byte 1 84
byte 1 79
byte 1 32
byte 1 68
byte 1 69
byte 1 70
byte 1 65
byte 1 85
byte 1 76
byte 1 84
byte 1 83
byte 1 63
byte 1 0
align 1
LABELV $76
byte 1 111
byte 1 112
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 105
byte 1 114
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 115
byte 1 46
byte 1 0
align 1
LABELV $75
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
byte 1 119
byte 1 105
byte 1 108
byte 1 108
byte 1 32
byte 1 114
byte 1 101
byte 1 115
byte 1 101
byte 1 116
byte 1 32
byte 1 42
byte 1 65
byte 1 76
byte 1 76
byte 1 42
byte 1 0
align 1
LABELV $73
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
LABELV $72
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
LABELV $71
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
