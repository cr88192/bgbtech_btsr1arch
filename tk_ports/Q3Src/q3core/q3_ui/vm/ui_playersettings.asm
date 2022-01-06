data
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
align 4
LABELV handicap_items
address $68
address $69
address $70
address $71
address $72
address $73
address $74
address $75
address $76
address $77
address $78
address $79
address $80
address $81
address $82
address $83
address $84
address $85
address $86
address $87
byte 4 0
code
proc PlayerSettings_DrawName 96 20
file "../ui_playersettings.c"
line 107
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
;23:#include "ui_local.h"
;24:
;25:#define ART_FRAMEL			"menu/art/frame2_l"
;26:#define ART_FRAMER			"menu/art/frame1_r"
;27:#define ART_MODEL0			"menu/art/model_0"
;28:#define ART_MODEL1			"menu/art/model_1"
;29:#define ART_BACK0			"menu/art/back_0"
;30:#define ART_BACK1			"menu/art/back_1"
;31:#define ART_FX_BASE			"menu/art/fx_base"
;32:#define ART_FX_BLUE			"menu/art/fx_blue"
;33:#define ART_FX_CYAN			"menu/art/fx_cyan"
;34:#define ART_FX_GREEN		"menu/art/fx_grn"
;35:#define ART_FX_RED			"menu/art/fx_red"
;36:#define ART_FX_TEAL			"menu/art/fx_teal"
;37:#define ART_FX_WHITE		"menu/art/fx_white"
;38:#define ART_FX_YELLOW		"menu/art/fx_yel"
;39:
;40:#define ID_NAME			10
;41:#define ID_HANDICAP		11
;42:#define ID_EFFECTS		12
;43:#define ID_BACK			13
;44:#define ID_MODEL		14
;45:
;46:#define MAX_NAMELENGTH	20
;47:
;48:
;49:typedef struct {
;50:	menuframework_s		menu;
;51:
;52:	menutext_s			banner;
;53:	menubitmap_s		framel;
;54:	menubitmap_s		framer;
;55:	menubitmap_s		player;
;56:
;57:	menufield_s			name;
;58:	menulist_s			handicap;
;59:	menulist_s			effects;
;60:
;61:	menubitmap_s		back;
;62:	menubitmap_s		model;
;63:	menubitmap_s		item_null;
;64:
;65:	qhandle_t			fxBasePic;
;66:	qhandle_t			fxPic[7];
;67:	playerInfo_t		playerinfo;
;68:	int					current_fx;
;69:	char				playerModel[MAX_QPATH];
;70:} playersettings_t;
;71:
;72:static playersettings_t	s_playersettings;
;73:
;74:static int gamecodetoui[] = {4,2,3,0,5,1,6};
;75:static int uitogamecode[] = {4,6,2,3,1,5,7};
;76:
;77:static const char *handicap_items[] = {
;78:	"None",
;79:	"95",
;80:	"90",
;81:	"85",
;82:	"80",
;83:	"75",
;84:	"70",
;85:	"65",
;86:	"60",
;87:	"55",
;88:	"50",
;89:	"45",
;90:	"40",
;91:	"35",
;92:	"30",
;93:	"25",
;94:	"20",
;95:	"15",
;96:	"10",
;97:	"5",
;98:	0
;99:};
;100:
;101:
;102:/*
;103:=================
;104:PlayerSettings_DrawName
;105:=================
;106:*/
;107:static void PlayerSettings_DrawName( void *self ) {
line 118
;108:	menufield_s		*f;
;109:	qboolean		focus;
;110:	int				style;
;111:	char			*txt;
;112:	char			c;
;113:	float			*color;
;114:	int				n;
;115:	int				basex, x, y;
;116:	char			name[32];
;117:
;118:	f = (menufield_s*)self;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
line 119
;119:	basex = f->generic.x;
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 120
;120:	y = f->generic.y;
ADDRLP4 24
ADDRLP4 32
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 121
;121:	focus = (f->generic.parent->cursor == f->generic.menuPosition);
ADDRLP4 32
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
NEI4 $90
ADDRLP4 72
CNSTI4 1
ASGNI4
ADDRGP4 $91
JUMPV
LABELV $90
ADDRLP4 72
CNSTI4 0
ASGNI4
LABELV $91
ADDRLP4 28
ADDRLP4 72
INDIRI4
ASGNI4
line 123
;122:
;123:	style = UI_LEFT|UI_SMALLFONT;
ADDRLP4 20
CNSTI4 16
ASGNI4
line 124
;124:	color = text_color_normal;
ADDRLP4 12
ADDRGP4 text_color_normal
ASGNP4
line 125
;125:	if( focus ) {
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $92
line 126
;126:		style |= UI_PULSE;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 127
;127:		color = text_color_highlight;
ADDRLP4 12
ADDRGP4 text_color_highlight
ASGNP4
line 128
;128:	}
LABELV $92
line 130
;129:
;130:	UI_DrawProportionalString( basex, y, "Name", style, color );
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 $94
ARGP4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 133
;131:
;132:	// draw the actual name
;133:	basex += 64;
ADDRLP4 36
ADDRLP4 36
INDIRI4
CNSTI4 64
ADDI4
ASGNI4
line 134
;134:	y += PROP_HEIGHT;
ADDRLP4 24
ADDRLP4 24
INDIRI4
CNSTI4 27
ADDI4
ASGNI4
line 135
;135:	txt = f->field.buffer;
ADDRLP4 0
ADDRLP4 32
INDIRP4
CNSTI4 72
ADDP4
ASGNP4
line 136
;136:	color = g_color_table[ColorIndex(COLOR_WHITE)];
ADDRLP4 12
ADDRGP4 g_color_table+112
ASGNP4
line 137
;137:	x = basex;
ADDRLP4 8
ADDRLP4 36
INDIRI4
ASGNI4
ADDRGP4 $97
JUMPV
LABELV $96
line 138
;138:	while ( (c = *txt) != 0 ) {
line 139
;139:		if ( !focus && Q_IsColorString( txt ) ) {
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRLP4 28
INDIRI4
ADDRLP4 80
INDIRI4
NEI4 $99
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $99
ADDRLP4 88
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 88
INDIRI4
NEI4 $99
ADDRLP4 92
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 92
INDIRI4
ADDRLP4 80
INDIRI4
EQI4 $99
ADDRLP4 92
INDIRI4
ADDRLP4 88
INDIRI4
EQI4 $99
line 140
;140:			n = ColorIndex( *(txt+1) );
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
ASGNI4
line 141
;141:			if( n == 0 ) {
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $101
line 142
;142:				n = 7;
ADDRLP4 16
CNSTI4 7
ASGNI4
line 143
;143:			}
LABELV $101
line 144
;144:			color = g_color_table[n];
ADDRLP4 12
ADDRLP4 16
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 g_color_table
ADDP4
ASGNP4
line 145
;145:			txt += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 146
;146:			continue;
ADDRGP4 $97
JUMPV
LABELV $99
line 148
;147:		}
;148:		UI_DrawChar( x, y, c, style, color );
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRLP4 4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 149
;149:		txt++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 150
;150:		x += SMALLCHAR_WIDTH;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 8
ADDI4
ASGNI4
line 151
;151:	}
LABELV $97
line 138
ADDRLP4 80
ADDRLP4 0
INDIRP4
INDIRI1
ASGNI1
ADDRLP4 4
ADDRLP4 80
INDIRI1
ASGNI1
ADDRLP4 80
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $96
line 154
;152:
;153:	// draw cursor if we have focus
;154:	if( focus ) {
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $103
line 155
;155:		if ( trap_Key_GetOverstrikeMode() ) {
ADDRLP4 84
ADDRGP4 trap_Key_GetOverstrikeMode
CALLI4
ASGNI4
ADDRLP4 84
INDIRI4
CNSTI4 0
EQI4 $105
line 156
;156:			c = 11;
ADDRLP4 4
CNSTI1 11
ASGNI1
line 157
;157:		} else {
ADDRGP4 $106
JUMPV
LABELV $105
line 158
;158:			c = 10;
ADDRLP4 4
CNSTI1 10
ASGNI1
line 159
;159:		}
LABELV $106
line 161
;160:
;161:		style &= ~UI_PULSE;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 -16385
BANDI4
ASGNI4
line 162
;162:		style |= UI_BLINK;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 164
;163:
;164:		UI_DrawChar( basex + f->field.cursor * SMALLCHAR_WIDTH, y, c, style, color_white );
ADDRLP4 36
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
CNSTI4 3
LSHI4
ADDI4
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRLP4 4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 165
;165:	}
LABELV $103
line 168
;166:
;167:	// draw at bottom also using proportional font
;168:	Q_strncpyz( name, f->field.buffer, sizeof(name) );
ADDRLP4 40
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 72
ADDP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 169
;169:	Q_CleanStr( name );
ADDRLP4 40
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 170
;170:	UI_DrawProportionalString( 320, 440, name, UI_CENTER|UI_BIGFONT, text_color_normal );
CNSTI4 320
ARGI4
CNSTI4 440
ARGI4
ADDRLP4 40
ARGP4
CNSTI4 33
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 171
;171:}
LABELV $88
endproc PlayerSettings_DrawName 96 20
proc PlayerSettings_DrawHandicap 36 20
line 179
;172:
;173:
;174:/*
;175:=================
;176:PlayerSettings_DrawHandicap
;177:=================
;178:*/
;179:static void PlayerSettings_DrawHandicap( void *self ) {
line 185
;180:	menulist_s		*item;
;181:	qboolean		focus;
;182:	int				style;
;183:	float			*color;
;184:
;185:	item = (menulist_s *)self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 186
;186:	focus = (item->generic.parent->cursor == item->generic.menuPosition);
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
NEI4 $109
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRGP4 $110
JUMPV
LABELV $109
ADDRLP4 16
CNSTI4 0
ASGNI4
LABELV $110
ADDRLP4 12
ADDRLP4 16
INDIRI4
ASGNI4
line 188
;187:
;188:	style = UI_LEFT|UI_SMALLFONT;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 189
;189:	color = text_color_normal;
ADDRLP4 8
ADDRGP4 text_color_normal
ASGNP4
line 190
;190:	if( focus ) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $111
line 191
;191:		style |= UI_PULSE;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 192
;192:		color = text_color_highlight;
ADDRLP4 8
ADDRGP4 text_color_highlight
ASGNP4
line 193
;193:	}
LABELV $111
line 195
;194:
;195:	UI_DrawProportionalString( item->generic.x, item->generic.y, "Handicap", style, color );
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRGP4 $113
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 196
;196:	UI_DrawProportionalString( item->generic.x + 64, item->generic.y + PROP_HEIGHT, handicap_items[item->curvalue], style, color );
ADDRLP4 32
CNSTI4 64
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 32
INDIRI4
ADDI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 27
ADDI4
ARGI4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 handicap_items
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 197
;197:}
LABELV $107
endproc PlayerSettings_DrawHandicap 36 20
proc PlayerSettings_DrawEffects 44 20
line 205
;198:
;199:
;200:/*
;201:=================
;202:PlayerSettings_DrawEffects
;203:=================
;204:*/
;205:static void PlayerSettings_DrawEffects( void *self ) {
line 211
;206:	menulist_s		*item;
;207:	qboolean		focus;
;208:	int				style;
;209:	float			*color;
;210:
;211:	item = (menulist_s *)self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 212
;212:	focus = (item->generic.parent->cursor == item->generic.menuPosition);
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
NEI4 $116
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRGP4 $117
JUMPV
LABELV $116
ADDRLP4 16
CNSTI4 0
ASGNI4
LABELV $117
ADDRLP4 12
ADDRLP4 16
INDIRI4
ASGNI4
line 214
;213:
;214:	style = UI_LEFT|UI_SMALLFONT;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 215
;215:	color = text_color_normal;
ADDRLP4 8
ADDRGP4 text_color_normal
ASGNP4
line 216
;216:	if( focus ) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $118
line 217
;217:		style |= UI_PULSE;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 218
;218:		color = text_color_highlight;
ADDRLP4 8
ADDRGP4 text_color_highlight
ASGNP4
line 219
;219:	}
LABELV $118
line 221
;220:
;221:	UI_DrawProportionalString( item->generic.x, item->generic.y, "Effects", style, color );
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRGP4 $120
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 223
;222:
;223:	UI_DrawHandlePic( item->generic.x + 64, item->generic.y + PROP_HEIGHT + 8, 128, 8, s_playersettings.fxBasePic );
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 64
ADDI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 27
ADDI4
CNSTI4 8
ADDI4
CVIF4 4
ARGF4
CNSTF4 1124073472
ARGF4
CNSTF4 1090519040
ARGF4
ADDRGP4 s_playersettings+1412
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 224
;224:	UI_DrawHandlePic( item->generic.x + 64 + item->curvalue * 16 + 8, item->generic.y + PROP_HEIGHT + 6, 16, 12, s_playersettings.fxPic[item->curvalue] );
ADDRLP4 36
CNSTI4 64
ASGNI4
ADDRLP4 40
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
ADDI4
ADDRLP4 40
INDIRI4
CNSTI4 4
LSHI4
ADDI4
CNSTI4 8
ADDI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 27
ADDI4
CNSTI4 6
ADDI4
CVIF4 4
ARGF4
CNSTF4 1098907648
ARGF4
CNSTF4 1094713344
ARGF4
ADDRLP4 40
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_playersettings+1416
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 225
;225:}
LABELV $114
endproc PlayerSettings_DrawEffects 44 20
proc PlayerSettings_DrawPlayer 88 28
line 233
;226:
;227:
;228:/*
;229:=================
;230:PlayerSettings_DrawPlayer
;231:=================
;232:*/
;233:static void PlayerSettings_DrawPlayer( void *self ) {
line 238
;234:	menubitmap_s	*b;
;235:	vec3_t			viewangles;
;236:	char			buf[MAX_QPATH];
;237:
;238:	trap_Cvar_VariableStringBuffer( "model", buf, sizeof( buf ) );
ADDRGP4 $124
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 239
;239:	if ( strcmp( buf, s_playersettings.playerModel ) != 0 ) {
ADDRLP4 4
ARGP4
ADDRGP4 s_playersettings+2556
ARGP4
ADDRLP4 80
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 80
INDIRI4
CNSTI4 0
EQI4 $125
line 240
;240:		UI_PlayerInfo_SetModel( &s_playersettings.playerinfo, buf );
ADDRGP4 s_playersettings+1444
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 UI_PlayerInfo_SetModel
CALLV
pop
line 241
;241:		strcpy( s_playersettings.playerModel, buf );
ADDRGP4 s_playersettings+2556
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 243
;242:
;243:		viewangles[YAW]   = 180 - 30;
ADDRLP4 68+4
CNSTF4 1125515264
ASGNF4
line 244
;244:		viewangles[PITCH] = 0;
ADDRLP4 68
CNSTF4 0
ASGNF4
line 245
;245:		viewangles[ROLL]  = 0;
ADDRLP4 68+8
CNSTF4 0
ASGNF4
line 246
;246:		UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_MACHINEGUN, qfalse );
ADDRGP4 s_playersettings+1444
ARGP4
CNSTI4 22
ARGI4
CNSTI4 11
ARGI4
ADDRLP4 68
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
line 247
;247:	}
LABELV $125
line 249
;248:
;249:	b = (menubitmap_s*) self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 250
;250:	UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, &s_playersettings.playerinfo, uis.realtime/2 );
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 s_playersettings+1444
ARGP4
ADDRGP4 uis+4
INDIRI4
CNSTI4 2
DIVI4
ARGI4
ADDRGP4 UI_DrawPlayer
CALLV
pop
line 251
;251:}
LABELV $123
endproc PlayerSettings_DrawPlayer 88 28
proc PlayerSettings_SaveChanges 0 8
line 259
;252:
;253:
;254:/*
;255:=================
;256:PlayerSettings_SaveChanges
;257:=================
;258:*/
;259:static void PlayerSettings_SaveChanges( void ) {
line 261
;260:	// name
;261:	trap_Cvar_Set( "name", s_playersettings.name.field.buffer );
ADDRGP4 $136
ARGP4
ADDRGP4 s_playersettings+624+60+12
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 264
;262:
;263:	// handicap
;264:	trap_Cvar_SetValue( "handicap", 100 - s_playersettings.handicap.curvalue * 5 );
ADDRGP4 $140
ARGP4
CNSTI4 100
CNSTI4 5
ADDRGP4 s_playersettings+956+64
INDIRI4
MULI4
SUBI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 267
;265:
;266:	// effects color
;267:	trap_Cvar_SetValue( "color1", uitogamecode[s_playersettings.effects.curvalue] );
ADDRGP4 $143
ARGP4
ADDRGP4 s_playersettings+1052+64
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
line 268
;268:}
LABELV $135
endproc PlayerSettings_SaveChanges 0 8
proc PlayerSettings_MenuKey 8 8
line 276
;269:
;270:
;271:/*
;272:=================
;273:PlayerSettings_MenuKey
;274:=================
;275:*/
;276:static sfxHandle_t PlayerSettings_MenuKey( int key ) {
line 277
;277:	if( key == K_MOUSE2 || key == K_ESCAPE ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $149
ADDRLP4 0
INDIRI4
CNSTI4 27
NEI4 $147
LABELV $149
line 278
;278:		PlayerSettings_SaveChanges();
ADDRGP4 PlayerSettings_SaveChanges
CALLV
pop
line 279
;279:	}
LABELV $147
line 280
;280:	return Menu_DefaultKey( &s_playersettings.menu, key );
ADDRGP4 s_playersettings
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
RETI4
LABELV $146
endproc PlayerSettings_MenuKey 8 8
proc PlayerSettings_SetMenuItems 44 28
line 289
;281:}
;282:
;283:
;284:/*
;285:=================
;286:PlayerSettings_SetMenuItems
;287:=================
;288:*/
;289:static void PlayerSettings_SetMenuItems( void ) {
line 295
;290:	vec3_t	viewangles;
;291:	int		c;
;292:	int		h;
;293:
;294:	// name
;295:	Q_strncpyz( s_playersettings.name.field.buffer, UI_Cvar_VariableString("name"), sizeof(s_playersettings.name.field.buffer) );
ADDRGP4 $136
ARGP4
ADDRLP4 20
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 s_playersettings+624+60+12
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 298
;296:
;297:	// effects color
;298:	c = trap_Cvar_VariableValue( "color1" ) - 1;
ADDRGP4 $143
ARGP4
ADDRLP4 24
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 24
INDIRF4
CNSTF4 1065353216
SUBF4
CVFI4 4
ASGNI4
line 299
;299:	if( c < 0 || c > 6 ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $159
ADDRLP4 0
INDIRI4
CNSTI4 6
LEI4 $157
LABELV $159
line 300
;300:		c = 6;
ADDRLP4 0
CNSTI4 6
ASGNI4
line 301
;301:	}
LABELV $157
line 302
;302:	s_playersettings.effects.curvalue = gamecodetoui[c];
ADDRGP4 s_playersettings+1052+64
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gamecodetoui
ADDP4
INDIRI4
ASGNI4
line 305
;303:
;304:	// model/skin
;305:	memset( &s_playersettings.playerinfo, 0, sizeof(playerInfo_t) );
ADDRGP4 s_playersettings+1444
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1108
ARGI4
ADDRGP4 memset
CALLP4
pop
line 307
;306:	
;307:	viewangles[YAW]   = 180 - 30;
ADDRLP4 4+4
CNSTF4 1125515264
ASGNF4
line 308
;308:	viewangles[PITCH] = 0;
ADDRLP4 4
CNSTF4 0
ASGNF4
line 309
;309:	viewangles[ROLL]  = 0;
ADDRLP4 4+8
CNSTF4 0
ASGNF4
line 311
;310:
;311:	UI_PlayerInfo_SetModel( &s_playersettings.playerinfo, UI_Cvar_VariableString( "model" ) );
ADDRGP4 $124
ARGP4
ADDRLP4 32
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 s_playersettings+1444
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRGP4 UI_PlayerInfo_SetModel
CALLV
pop
line 312
;312:	UI_PlayerInfo_SetInfo( &s_playersettings.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, WP_MACHINEGUN, qfalse );
ADDRGP4 s_playersettings+1444
ARGP4
CNSTI4 22
ARGI4
CNSTI4 11
ARGI4
ADDRLP4 4
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
line 315
;313:
;314:	// handicap
;315:	h = Com_Clamp( 5, 100, trap_Cvar_VariableValue("handicap") );
ADDRGP4 $140
ARGP4
ADDRLP4 36
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 1084227584
ARGF4
CNSTF4 1120403456
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 40
INDIRF4
CVFI4 4
ASGNI4
line 316
;316:	s_playersettings.handicap.curvalue = 20 - h / 5;
ADDRGP4 s_playersettings+956+64
CNSTI4 20
ADDRLP4 16
INDIRI4
CNSTI4 5
DIVI4
SUBI4
ASGNI4
line 317
;317:}
LABELV $150
endproc PlayerSettings_SetMenuItems 44 28
proc PlayerSettings_MenuEvent 12 8
line 325
;318:
;319:
;320:/*
;321:=================
;322:PlayerSettings_MenuEvent
;323:=================
;324:*/
;325:static void PlayerSettings_MenuEvent( void* ptr, int event ) {
line 326
;326:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $170
line 327
;327:		return;
ADDRGP4 $169
JUMPV
LABELV $170
line 330
;328:	}
;329:
;330:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 11
EQI4 $175
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $180
ADDRLP4 0
INDIRI4
CNSTI4 14
EQI4 $179
ADDRGP4 $172
JUMPV
LABELV $175
line 332
;331:	case ID_HANDICAP:
;332:		trap_Cvar_Set( "handicap", va( "%i", 100 - 25 * s_playersettings.handicap.curvalue ) );
ADDRGP4 $176
ARGP4
CNSTI4 100
CNSTI4 25
ADDRGP4 s_playersettings+956+64
INDIRI4
MULI4
SUBI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $140
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 333
;333:		break;
ADDRGP4 $173
JUMPV
LABELV $179
line 336
;334:
;335:	case ID_MODEL:
;336:		PlayerSettings_SaveChanges();
ADDRGP4 PlayerSettings_SaveChanges
CALLV
pop
line 337
;337:		UI_PlayerModelMenu();
ADDRGP4 UI_PlayerModelMenu
CALLV
pop
line 338
;338:		break;
ADDRGP4 $173
JUMPV
LABELV $180
line 341
;339:
;340:	case ID_BACK:
;341:		PlayerSettings_SaveChanges();
ADDRGP4 PlayerSettings_SaveChanges
CALLV
pop
line 342
;342:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 343
;343:		break;
LABELV $172
LABELV $173
line 345
;344:	}
;345:}
LABELV $169
endproc PlayerSettings_MenuEvent 12 8
proc PlayerSettings_MenuInit 4 12
line 353
;346:
;347:
;348:/*
;349:=================
;350:PlayerSettings_MenuInit
;351:=================
;352:*/
;353:static void PlayerSettings_MenuInit( void ) {
line 356
;354:	int		y;
;355:
;356:	memset(&s_playersettings,0,sizeof(playersettings_t));
ADDRGP4 s_playersettings
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2620
ARGI4
ADDRGP4 memset
CALLP4
pop
line 358
;357:
;358:	PlayerSettings_Cache();
ADDRGP4 PlayerSettings_Cache
CALLV
pop
line 360
;359:
;360:	s_playersettings.menu.key        = PlayerSettings_MenuKey;
ADDRGP4 s_playersettings+272
ADDRGP4 PlayerSettings_MenuKey
ASGNP4
line 361
;361:	s_playersettings.menu.wrapAround = qtrue;
ADDRGP4 s_playersettings+276
CNSTI4 1
ASGNI4
line 362
;362:	s_playersettings.menu.fullscreen = qtrue;
ADDRGP4 s_playersettings+280
CNSTI4 1
ASGNI4
line 364
;363:
;364:	s_playersettings.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_playersettings+288
CNSTI4 10
ASGNI4
line 365
;365:	s_playersettings.banner.generic.x     = 320;
ADDRGP4 s_playersettings+288+12
CNSTI4 320
ASGNI4
line 366
;366:	s_playersettings.banner.generic.y     = 16;
ADDRGP4 s_playersettings+288+16
CNSTI4 16
ASGNI4
line 367
;367:	s_playersettings.banner.string        = "PLAYER SETTINGS";
ADDRGP4 s_playersettings+288+60
ADDRGP4 $192
ASGNP4
line 368
;368:	s_playersettings.banner.color         = color_white;
ADDRGP4 s_playersettings+288+68
ADDRGP4 color_white
ASGNP4
line 369
;369:	s_playersettings.banner.style         = UI_CENTER;
ADDRGP4 s_playersettings+288+64
CNSTI4 1
ASGNI4
line 371
;370:
;371:	s_playersettings.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_playersettings+360
CNSTI4 6
ASGNI4
line 372
;372:	s_playersettings.framel.generic.name  = ART_FRAMEL;
ADDRGP4 s_playersettings+360+4
ADDRGP4 $200
ASGNP4
line 373
;373:	s_playersettings.framel.generic.flags = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 s_playersettings+360+44
CNSTU4 16388
ASGNU4
line 374
;374:	s_playersettings.framel.generic.x     = 0;
ADDRGP4 s_playersettings+360+12
CNSTI4 0
ASGNI4
line 375
;375:	s_playersettings.framel.generic.y     = 78;
ADDRGP4 s_playersettings+360+16
CNSTI4 78
ASGNI4
line 376
;376:	s_playersettings.framel.width         = 256;
ADDRGP4 s_playersettings+360+76
CNSTI4 256
ASGNI4
line 377
;377:	s_playersettings.framel.height        = 329;
ADDRGP4 s_playersettings+360+80
CNSTI4 329
ASGNI4
line 379
;378:
;379:	s_playersettings.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_playersettings+448
CNSTI4 6
ASGNI4
line 380
;380:	s_playersettings.framer.generic.name  = ART_FRAMER;
ADDRGP4 s_playersettings+448+4
ADDRGP4 $214
ASGNP4
line 381
;381:	s_playersettings.framer.generic.flags = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 s_playersettings+448+44
CNSTU4 16388
ASGNU4
line 382
;382:	s_playersettings.framer.generic.x     = 376;
ADDRGP4 s_playersettings+448+12
CNSTI4 376
ASGNI4
line 383
;383:	s_playersettings.framer.generic.y     = 76;
ADDRGP4 s_playersettings+448+16
CNSTI4 76
ASGNI4
line 384
;384:	s_playersettings.framer.width         = 256;
ADDRGP4 s_playersettings+448+76
CNSTI4 256
ASGNI4
line 385
;385:	s_playersettings.framer.height        = 334;
ADDRGP4 s_playersettings+448+80
CNSTI4 334
ASGNI4
line 387
;386:
;387:	y = 144;
ADDRLP4 0
CNSTI4 144
ASGNI4
line 388
;388:	s_playersettings.name.generic.type			= MTYPE_FIELD;
ADDRGP4 s_playersettings+624
CNSTI4 4
ASGNI4
line 389
;389:	s_playersettings.name.generic.flags			= QMF_NODEFAULTINIT;
ADDRGP4 s_playersettings+624+44
CNSTU4 32768
ASGNU4
line 390
;390:	s_playersettings.name.generic.ownerdraw		= PlayerSettings_DrawName;
ADDRGP4 s_playersettings+624+56
ADDRGP4 PlayerSettings_DrawName
ASGNP4
line 391
;391:	s_playersettings.name.field.widthInChars	= MAX_NAMELENGTH;
ADDRGP4 s_playersettings+624+60+8
CNSTI4 20
ASGNI4
line 392
;392:	s_playersettings.name.field.maxchars		= MAX_NAMELENGTH;
ADDRGP4 s_playersettings+624+60+268
CNSTI4 20
ASGNI4
line 393
;393:	s_playersettings.name.generic.x				= 192;
ADDRGP4 s_playersettings+624+12
CNSTI4 192
ASGNI4
line 394
;394:	s_playersettings.name.generic.y				= y;
ADDRGP4 s_playersettings+624+16
ADDRLP4 0
INDIRI4
ASGNI4
line 395
;395:	s_playersettings.name.generic.left			= 192 - 8;
ADDRGP4 s_playersettings+624+20
CNSTI4 184
ASGNI4
line 396
;396:	s_playersettings.name.generic.top			= y - 8;
ADDRGP4 s_playersettings+624+24
ADDRLP4 0
INDIRI4
CNSTI4 8
SUBI4
ASGNI4
line 397
;397:	s_playersettings.name.generic.right			= 192 + 200;
ADDRGP4 s_playersettings+624+28
CNSTI4 392
ASGNI4
line 398
;398:	s_playersettings.name.generic.bottom		= y + 2 * PROP_HEIGHT;
ADDRGP4 s_playersettings+624+32
ADDRLP4 0
INDIRI4
CNSTI4 54
ADDI4
ASGNI4
line 400
;399:
;400:	y += 3 * PROP_HEIGHT;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 81
ADDI4
ASGNI4
line 401
;401:	s_playersettings.handicap.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 s_playersettings+956
CNSTI4 3
ASGNI4
line 402
;402:	s_playersettings.handicap.generic.flags		= QMF_NODEFAULTINIT;
ADDRGP4 s_playersettings+956+44
CNSTU4 32768
ASGNU4
line 403
;403:	s_playersettings.handicap.generic.id		= ID_HANDICAP;
ADDRGP4 s_playersettings+956+8
CNSTI4 11
ASGNI4
line 404
;404:	s_playersettings.handicap.generic.ownerdraw	= PlayerSettings_DrawHandicap;
ADDRGP4 s_playersettings+956+56
ADDRGP4 PlayerSettings_DrawHandicap
ASGNP4
line 405
;405:	s_playersettings.handicap.generic.x			= 192;
ADDRGP4 s_playersettings+956+12
CNSTI4 192
ASGNI4
line 406
;406:	s_playersettings.handicap.generic.y			= y;
ADDRGP4 s_playersettings+956+16
ADDRLP4 0
INDIRI4
ASGNI4
line 407
;407:	s_playersettings.handicap.generic.left		= 192 - 8;
ADDRGP4 s_playersettings+956+20
CNSTI4 184
ASGNI4
line 408
;408:	s_playersettings.handicap.generic.top		= y - 8;
ADDRGP4 s_playersettings+956+24
ADDRLP4 0
INDIRI4
CNSTI4 8
SUBI4
ASGNI4
line 409
;409:	s_playersettings.handicap.generic.right		= 192 + 200;
ADDRGP4 s_playersettings+956+28
CNSTI4 392
ASGNI4
line 410
;410:	s_playersettings.handicap.generic.bottom	= y + 2 * PROP_HEIGHT;
ADDRGP4 s_playersettings+956+32
ADDRLP4 0
INDIRI4
CNSTI4 54
ADDI4
ASGNI4
line 411
;411:	s_playersettings.handicap.numitems			= 20;
ADDRGP4 s_playersettings+956+68
CNSTI4 20
ASGNI4
line 413
;412:
;413:	y += 3 * PROP_HEIGHT;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 81
ADDI4
ASGNI4
line 414
;414:	s_playersettings.effects.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 s_playersettings+1052
CNSTI4 3
ASGNI4
line 415
;415:	s_playersettings.effects.generic.flags		= QMF_NODEFAULTINIT;
ADDRGP4 s_playersettings+1052+44
CNSTU4 32768
ASGNU4
line 416
;416:	s_playersettings.effects.generic.id			= ID_EFFECTS;
ADDRGP4 s_playersettings+1052+8
CNSTI4 12
ASGNI4
line 417
;417:	s_playersettings.effects.generic.ownerdraw	= PlayerSettings_DrawEffects;
ADDRGP4 s_playersettings+1052+56
ADDRGP4 PlayerSettings_DrawEffects
ASGNP4
line 418
;418:	s_playersettings.effects.generic.x			= 192;
ADDRGP4 s_playersettings+1052+12
CNSTI4 192
ASGNI4
line 419
;419:	s_playersettings.effects.generic.y			= y;
ADDRGP4 s_playersettings+1052+16
ADDRLP4 0
INDIRI4
ASGNI4
line 420
;420:	s_playersettings.effects.generic.left		= 192 - 8;
ADDRGP4 s_playersettings+1052+20
CNSTI4 184
ASGNI4
line 421
;421:	s_playersettings.effects.generic.top		= y - 8;
ADDRGP4 s_playersettings+1052+24
ADDRLP4 0
INDIRI4
CNSTI4 8
SUBI4
ASGNI4
line 422
;422:	s_playersettings.effects.generic.right		= 192 + 200;
ADDRGP4 s_playersettings+1052+28
CNSTI4 392
ASGNI4
line 423
;423:	s_playersettings.effects.generic.bottom		= y + 2* PROP_HEIGHT;
ADDRGP4 s_playersettings+1052+32
ADDRLP4 0
INDIRI4
CNSTI4 54
ADDI4
ASGNI4
line 424
;424:	s_playersettings.effects.numitems			= 7;
ADDRGP4 s_playersettings+1052+68
CNSTI4 7
ASGNI4
line 426
;425:
;426:	s_playersettings.model.generic.type			= MTYPE_BITMAP;
ADDRGP4 s_playersettings+1236
CNSTI4 6
ASGNI4
line 427
;427:	s_playersettings.model.generic.name			= ART_MODEL0;
ADDRGP4 s_playersettings+1236+4
ADDRGP4 $293
ASGNP4
line 428
;428:	s_playersettings.model.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_playersettings+1236+44
CNSTU4 272
ASGNU4
line 429
;429:	s_playersettings.model.generic.id			= ID_MODEL;
ADDRGP4 s_playersettings+1236+8
CNSTI4 14
ASGNI4
line 430
;430:	s_playersettings.model.generic.callback		= PlayerSettings_MenuEvent;
ADDRGP4 s_playersettings+1236+48
ADDRGP4 PlayerSettings_MenuEvent
ASGNP4
line 431
;431:	s_playersettings.model.generic.x			= 640;
ADDRGP4 s_playersettings+1236+12
CNSTI4 640
ASGNI4
line 432
;432:	s_playersettings.model.generic.y			= 480-64;
ADDRGP4 s_playersettings+1236+16
CNSTI4 416
ASGNI4
line 433
;433:	s_playersettings.model.width				= 128;
ADDRGP4 s_playersettings+1236+76
CNSTI4 128
ASGNI4
line 434
;434:	s_playersettings.model.height				= 64;
ADDRGP4 s_playersettings+1236+80
CNSTI4 64
ASGNI4
line 435
;435:	s_playersettings.model.focuspic				= ART_MODEL1;
ADDRGP4 s_playersettings+1236+60
ADDRGP4 $310
ASGNP4
line 437
;436:
;437:	s_playersettings.player.generic.type		= MTYPE_BITMAP;
ADDRGP4 s_playersettings+536
CNSTI4 6
ASGNI4
line 438
;438:	s_playersettings.player.generic.flags		= QMF_INACTIVE;
ADDRGP4 s_playersettings+536+44
CNSTU4 16384
ASGNU4
line 439
;439:	s_playersettings.player.generic.ownerdraw	= PlayerSettings_DrawPlayer;
ADDRGP4 s_playersettings+536+56
ADDRGP4 PlayerSettings_DrawPlayer
ASGNP4
line 440
;440:	s_playersettings.player.generic.x			= 400;
ADDRGP4 s_playersettings+536+12
CNSTI4 400
ASGNI4
line 441
;441:	s_playersettings.player.generic.y			= -40;
ADDRGP4 s_playersettings+536+16
CNSTI4 -40
ASGNI4
line 442
;442:	s_playersettings.player.width				= 32*10;
ADDRGP4 s_playersettings+536+76
CNSTI4 320
ASGNI4
line 443
;443:	s_playersettings.player.height				= 56*10;
ADDRGP4 s_playersettings+536+80
CNSTI4 560
ASGNI4
line 445
;444:
;445:	s_playersettings.back.generic.type			= MTYPE_BITMAP;
ADDRGP4 s_playersettings+1148
CNSTI4 6
ASGNI4
line 446
;446:	s_playersettings.back.generic.name			= ART_BACK0;
ADDRGP4 s_playersettings+1148+4
ADDRGP4 $327
ASGNP4
line 447
;447:	s_playersettings.back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_playersettings+1148+44
CNSTU4 260
ASGNU4
line 448
;448:	s_playersettings.back.generic.id			= ID_BACK;
ADDRGP4 s_playersettings+1148+8
CNSTI4 13
ASGNI4
line 449
;449:	s_playersettings.back.generic.callback		= PlayerSettings_MenuEvent;
ADDRGP4 s_playersettings+1148+48
ADDRGP4 PlayerSettings_MenuEvent
ASGNP4
line 450
;450:	s_playersettings.back.generic.x				= 0;
ADDRGP4 s_playersettings+1148+12
CNSTI4 0
ASGNI4
line 451
;451:	s_playersettings.back.generic.y				= 480-64;
ADDRGP4 s_playersettings+1148+16
CNSTI4 416
ASGNI4
line 452
;452:	s_playersettings.back.width					= 128;
ADDRGP4 s_playersettings+1148+76
CNSTI4 128
ASGNI4
line 453
;453:	s_playersettings.back.height				= 64;
ADDRGP4 s_playersettings+1148+80
CNSTI4 64
ASGNI4
line 454
;454:	s_playersettings.back.focuspic				= ART_BACK1;
ADDRGP4 s_playersettings+1148+60
ADDRGP4 $344
ASGNP4
line 456
;455:
;456:	s_playersettings.item_null.generic.type		= MTYPE_BITMAP;
ADDRGP4 s_playersettings+1324
CNSTI4 6
ASGNI4
line 457
;457:	s_playersettings.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
ADDRGP4 s_playersettings+1324+44
CNSTU4 1050628
ASGNU4
line 458
;458:	s_playersettings.item_null.generic.x		= 0;
ADDRGP4 s_playersettings+1324+12
CNSTI4 0
ASGNI4
line 459
;459:	s_playersettings.item_null.generic.y		= 0;
ADDRGP4 s_playersettings+1324+16
CNSTI4 0
ASGNI4
line 460
;460:	s_playersettings.item_null.width			= 640;
ADDRGP4 s_playersettings+1324+76
CNSTI4 640
ASGNI4
line 461
;461:	s_playersettings.item_null.height			= 480;
ADDRGP4 s_playersettings+1324+80
CNSTI4 480
ASGNI4
line 463
;462:
;463:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.banner );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 464
;464:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.framel );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 465
;465:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.framer );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 467
;466:
;467:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.name );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+624
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 468
;468:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.handicap );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+956
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 469
;469:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.effects );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+1052
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 470
;470:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.model );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+1236
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 471
;471:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.back );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+1148
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 473
;472:
;473:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.player );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 475
;474:
;475:	Menu_AddItem( &s_playersettings.menu, &s_playersettings.item_null );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 s_playersettings+1324
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 477
;476:
;477:	PlayerSettings_SetMenuItems();
ADDRGP4 PlayerSettings_SetMenuItems
CALLV
pop
line 478
;478:}
LABELV $181
endproc PlayerSettings_MenuInit 4 12
export PlayerSettings_Cache
proc PlayerSettings_Cache 32 4
line 486
;479:
;480:
;481:/*
;482:=================
;483:PlayerSettings_Cache
;484:=================
;485:*/
;486:void PlayerSettings_Cache( void ) {
line 487
;487:	trap_R_RegisterShaderNoMip( ART_FRAMEL );
ADDRGP4 $200
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 488
;488:	trap_R_RegisterShaderNoMip( ART_FRAMER );
ADDRGP4 $214
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 489
;489:	trap_R_RegisterShaderNoMip( ART_MODEL0 );
ADDRGP4 $293
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 490
;490:	trap_R_RegisterShaderNoMip( ART_MODEL1 );
ADDRGP4 $310
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 491
;491:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $327
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 492
;492:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $344
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 494
;493:
;494:	s_playersettings.fxBasePic = trap_R_RegisterShaderNoMip( ART_FX_BASE );
ADDRGP4 $368
ARGP4
ADDRLP4 0
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1412
ADDRLP4 0
INDIRI4
ASGNI4
line 495
;495:	s_playersettings.fxPic[0] = trap_R_RegisterShaderNoMip( ART_FX_RED );
ADDRGP4 $370
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416
ADDRLP4 4
INDIRI4
ASGNI4
line 496
;496:	s_playersettings.fxPic[1] = trap_R_RegisterShaderNoMip( ART_FX_YELLOW );
ADDRGP4 $373
ARGP4
ADDRLP4 8
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+4
ADDRLP4 8
INDIRI4
ASGNI4
line 497
;497:	s_playersettings.fxPic[2] = trap_R_RegisterShaderNoMip( ART_FX_GREEN );
ADDRGP4 $376
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+8
ADDRLP4 12
INDIRI4
ASGNI4
line 498
;498:	s_playersettings.fxPic[3] = trap_R_RegisterShaderNoMip( ART_FX_TEAL );
ADDRGP4 $379
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+12
ADDRLP4 16
INDIRI4
ASGNI4
line 499
;499:	s_playersettings.fxPic[4] = trap_R_RegisterShaderNoMip( ART_FX_BLUE );
ADDRGP4 $382
ARGP4
ADDRLP4 20
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+16
ADDRLP4 20
INDIRI4
ASGNI4
line 500
;500:	s_playersettings.fxPic[5] = trap_R_RegisterShaderNoMip( ART_FX_CYAN );
ADDRGP4 $385
ARGP4
ADDRLP4 24
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+20
ADDRLP4 24
INDIRI4
ASGNI4
line 501
;501:	s_playersettings.fxPic[6] = trap_R_RegisterShaderNoMip( ART_FX_WHITE );
ADDRGP4 $388
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 s_playersettings+1416+24
ADDRLP4 28
INDIRI4
ASGNI4
line 502
;502:}
LABELV $366
endproc PlayerSettings_Cache 32 4
export UI_PlayerSettingsMenu
proc UI_PlayerSettingsMenu 0 4
line 510
;503:
;504:
;505:/*
;506:=================
;507:UI_PlayerSettingsMenu
;508:=================
;509:*/
;510:void UI_PlayerSettingsMenu( void ) {
line 511
;511:	PlayerSettings_MenuInit();
ADDRGP4 PlayerSettings_MenuInit
CALLV
pop
line 512
;512:	UI_PushMenu( &s_playersettings.menu );
ADDRGP4 s_playersettings
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 513
;513:}
LABELV $389
endproc UI_PlayerSettingsMenu 0 4
bss
align 4
LABELV s_playersettings
skip 2620
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
LABELV $388
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
LABELV $385
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
LABELV $382
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
LABELV $379
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
LABELV $376
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
LABELV $373
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
LABELV $370
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
LABELV $368
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
LABELV $344
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
LABELV $327
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
LABELV $310
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
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $293
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
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $214
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
LABELV $192
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 69
byte 1 82
byte 1 32
byte 1 83
byte 1 69
byte 1 84
byte 1 84
byte 1 73
byte 1 78
byte 1 71
byte 1 83
byte 1 0
align 1
LABELV $176
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $143
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $140
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
LABELV $136
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $120
byte 1 69
byte 1 102
byte 1 102
byte 1 101
byte 1 99
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $113
byte 1 72
byte 1 97
byte 1 110
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $94
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $87
byte 1 53
byte 1 0
align 1
LABELV $86
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $85
byte 1 49
byte 1 53
byte 1 0
align 1
LABELV $84
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $83
byte 1 50
byte 1 53
byte 1 0
align 1
LABELV $82
byte 1 51
byte 1 48
byte 1 0
align 1
LABELV $81
byte 1 51
byte 1 53
byte 1 0
align 1
LABELV $80
byte 1 52
byte 1 48
byte 1 0
align 1
LABELV $79
byte 1 52
byte 1 53
byte 1 0
align 1
LABELV $78
byte 1 53
byte 1 48
byte 1 0
align 1
LABELV $77
byte 1 53
byte 1 53
byte 1 0
align 1
LABELV $76
byte 1 54
byte 1 48
byte 1 0
align 1
LABELV $75
byte 1 54
byte 1 53
byte 1 0
align 1
LABELV $74
byte 1 55
byte 1 48
byte 1 0
align 1
LABELV $73
byte 1 55
byte 1 53
byte 1 0
align 1
LABELV $72
byte 1 56
byte 1 48
byte 1 0
align 1
LABELV $71
byte 1 56
byte 1 53
byte 1 0
align 1
LABELV $70
byte 1 57
byte 1 48
byte 1 0
align 1
LABELV $69
byte 1 57
byte 1 53
byte 1 0
align 1
LABELV $68
byte 1 78
byte 1 111
byte 1 110
byte 1 101
byte 1 0
