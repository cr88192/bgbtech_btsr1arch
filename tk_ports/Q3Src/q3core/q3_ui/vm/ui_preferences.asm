data
align 4
LABELV teamoverlay_names
address $68
address $69
address $70
address $71
byte 4 0
code
proc Preferences_SetMenuItems 84 12
file "../ui_preferences.c"
line 92
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
;26:GAME OPTIONS MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ART_FRAMEL				"menu/art/frame2_l"
;36:#define ART_FRAMER				"menu/art/frame1_r"
;37:#define ART_BACK0				"menu/art/back_0"
;38:#define ART_BACK1				"menu/art/back_1"
;39:
;40:#define PREFERENCES_X_POS		360
;41:
;42:#define ID_CROSSHAIR			127
;43:#define ID_SIMPLEITEMS			128
;44:#define ID_HIGHQUALITYSKY		129
;45:#define ID_EJECTINGBRASS		130
;46:#define ID_WALLMARKS			131
;47:#define ID_DYNAMICLIGHTS		132
;48:#define ID_IDENTIFYTARGET		133
;49:#define ID_SYNCEVERYFRAME		134
;50:#define ID_FORCEMODEL			135
;51:#define ID_DRAWTEAMOVERLAY		136
;52:#define ID_ALLOWDOWNLOAD			137
;53:#define ID_BACK					138
;54:
;55:#define	NUM_CROSSHAIRS			10
;56:
;57:
;58:typedef struct {
;59:	menuframework_s		menu;
;60:
;61:	menutext_s			banner;
;62:	menubitmap_s		framel;
;63:	menubitmap_s		framer;
;64:
;65:	menulist_s			crosshair;
;66:	menuradiobutton_s	simpleitems;
;67:	menuradiobutton_s	brass;
;68:	menuradiobutton_s	wallmarks;
;69:	menuradiobutton_s	dynamiclights;
;70:	menuradiobutton_s	identifytarget;
;71:	menuradiobutton_s	highqualitysky;
;72:	menuradiobutton_s	synceveryframe;
;73:	menuradiobutton_s	forcemodel;
;74:	menulist_s			drawteamoverlay;
;75:	menuradiobutton_s	allowdownload;
;76:	menubitmap_s		back;
;77:
;78:	qhandle_t			crosshairShader[NUM_CROSSHAIRS];
;79:} preferences_t;
;80:
;81:static preferences_t s_preferences;
;82:
;83:static const char *teamoverlay_names[] =
;84:{
;85:	"off",
;86:	"upper right",
;87:	"lower right",
;88:	"lower left",
;89:	0
;90:};
;91:
;92:static void Preferences_SetMenuItems( void ) {
line 93
;93:	s_preferences.crosshair.curvalue		= (int)trap_Cvar_VariableValue( "cg_drawCrosshair" ) % NUM_CROSSHAIRS;
ADDRGP4 $75
ARGP4
ADDRLP4 0
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 s_preferences+536+64
ADDRLP4 0
INDIRF4
CVFI4 4
CNSTI4 10
MODI4
ASGNI4
line 94
;94:	s_preferences.simpleitems.curvalue		= trap_Cvar_VariableValue( "cg_simpleItems" ) != 0;
ADDRGP4 $78
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 8
INDIRF4
CNSTF4 0
EQF4 $80
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $81
JUMPV
LABELV $80
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $81
ADDRGP4 s_preferences+632+60
ADDRLP4 4
INDIRI4
ASGNI4
line 95
;95:	s_preferences.brass.curvalue			= trap_Cvar_VariableValue( "cg_brassTime" ) != 0;
ADDRGP4 $84
ARGP4
ADDRLP4 16
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 16
INDIRF4
CNSTF4 0
EQF4 $86
ADDRLP4 12
CNSTI4 1
ASGNI4
ADDRGP4 $87
JUMPV
LABELV $86
ADDRLP4 12
CNSTI4 0
ASGNI4
LABELV $87
ADDRGP4 s_preferences+696+60
ADDRLP4 12
INDIRI4
ASGNI4
line 96
;96:	s_preferences.wallmarks.curvalue		= trap_Cvar_VariableValue( "cg_marks" ) != 0;
ADDRGP4 $90
ARGP4
ADDRLP4 24
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 24
INDIRF4
CNSTF4 0
EQF4 $92
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRGP4 $93
JUMPV
LABELV $92
ADDRLP4 20
CNSTI4 0
ASGNI4
LABELV $93
ADDRGP4 s_preferences+760+60
ADDRLP4 20
INDIRI4
ASGNI4
line 97
;97:	s_preferences.identifytarget.curvalue	= trap_Cvar_VariableValue( "cg_drawCrosshairNames" ) != 0;
ADDRGP4 $96
ARGP4
ADDRLP4 32
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 32
INDIRF4
CNSTF4 0
EQF4 $98
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRGP4 $99
JUMPV
LABELV $98
ADDRLP4 28
CNSTI4 0
ASGNI4
LABELV $99
ADDRGP4 s_preferences+888+60
ADDRLP4 28
INDIRI4
ASGNI4
line 98
;98:	s_preferences.dynamiclights.curvalue	= trap_Cvar_VariableValue( "r_dynamiclight" ) != 0;
ADDRGP4 $102
ARGP4
ADDRLP4 40
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 40
INDIRF4
CNSTF4 0
EQF4 $104
ADDRLP4 36
CNSTI4 1
ASGNI4
ADDRGP4 $105
JUMPV
LABELV $104
ADDRLP4 36
CNSTI4 0
ASGNI4
LABELV $105
ADDRGP4 s_preferences+824+60
ADDRLP4 36
INDIRI4
ASGNI4
line 99
;99:	s_preferences.highqualitysky.curvalue	= trap_Cvar_VariableValue ( "r_fastsky" ) == 0;
ADDRGP4 $108
ARGP4
ADDRLP4 48
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 48
INDIRF4
CNSTF4 0
NEF4 $110
ADDRLP4 44
CNSTI4 1
ASGNI4
ADDRGP4 $111
JUMPV
LABELV $110
ADDRLP4 44
CNSTI4 0
ASGNI4
LABELV $111
ADDRGP4 s_preferences+952+60
ADDRLP4 44
INDIRI4
ASGNI4
line 100
;100:	s_preferences.synceveryframe.curvalue	= trap_Cvar_VariableValue( "r_finish" ) != 0;
ADDRGP4 $114
ARGP4
ADDRLP4 56
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 56
INDIRF4
CNSTF4 0
EQF4 $116
ADDRLP4 52
CNSTI4 1
ASGNI4
ADDRGP4 $117
JUMPV
LABELV $116
ADDRLP4 52
CNSTI4 0
ASGNI4
LABELV $117
ADDRGP4 s_preferences+1016+60
ADDRLP4 52
INDIRI4
ASGNI4
line 101
;101:	s_preferences.forcemodel.curvalue		= trap_Cvar_VariableValue( "cg_forcemodel" ) != 0;
ADDRGP4 $120
ARGP4
ADDRLP4 64
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 64
INDIRF4
CNSTF4 0
EQF4 $122
ADDRLP4 60
CNSTI4 1
ASGNI4
ADDRGP4 $123
JUMPV
LABELV $122
ADDRLP4 60
CNSTI4 0
ASGNI4
LABELV $123
ADDRGP4 s_preferences+1080+60
ADDRLP4 60
INDIRI4
ASGNI4
line 102
;102:	s_preferences.drawteamoverlay.curvalue	= Com_Clamp( 0, 3, trap_Cvar_VariableValue( "cg_drawTeamOverlay" ) );
ADDRGP4 $126
ARGP4
ADDRLP4 68
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1077936128
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_preferences+1144+64
ADDRLP4 72
INDIRF4
CVFI4 4
ASGNI4
line 103
;103:	s_preferences.allowdownload.curvalue	= trap_Cvar_VariableValue( "cl_allowDownload" ) != 0;
ADDRGP4 $129
ARGP4
ADDRLP4 80
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 80
INDIRF4
CNSTF4 0
EQF4 $131
ADDRLP4 76
CNSTI4 1
ASGNI4
ADDRGP4 $132
JUMPV
LABELV $131
ADDRLP4 76
CNSTI4 0
ASGNI4
LABELV $132
ADDRGP4 s_preferences+1240+60
ADDRLP4 76
INDIRI4
ASGNI4
line 104
;104:}
LABELV $72
endproc Preferences_SetMenuItems 84 12
proc Preferences_Event 16 8
line 107
;105:
;106:
;107:static void Preferences_Event( void* ptr, int notification ) {
line 108
;108:	if( notification != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $134
line 109
;109:		return;
ADDRGP4 $133
JUMPV
LABELV $134
line 112
;110:	}
;111:
;112:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 127
LTI4 $136
ADDRLP4 0
INDIRI4
CNSTI4 138
GTI4 $136
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $189-508
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $189
address $139
address $150
address $153
address $159
address $164
address $167
address $170
address $173
address $176
address $179
address $182
address $188
code
LABELV $139
line 114
;113:	case ID_CROSSHAIR:
;114:		s_preferences.crosshair.curvalue++;
ADDRLP4 8
ADDRGP4 s_preferences+536+64
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 115
;115:		if( s_preferences.crosshair.curvalue == NUM_CROSSHAIRS ) {
ADDRGP4 s_preferences+536+64
INDIRI4
CNSTI4 10
NEI4 $142
line 116
;116:			s_preferences.crosshair.curvalue = 0;
ADDRGP4 s_preferences+536+64
CNSTI4 0
ASGNI4
line 117
;117:		}
LABELV $142
line 118
;118:		trap_Cvar_SetValue( "cg_drawCrosshair", s_preferences.crosshair.curvalue );
ADDRGP4 $75
ARGP4
ADDRGP4 s_preferences+536+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 119
;119:		break;
ADDRGP4 $137
JUMPV
LABELV $150
line 122
;120:
;121:	case ID_SIMPLEITEMS:
;122:		trap_Cvar_SetValue( "cg_simpleItems", s_preferences.simpleitems.curvalue );
ADDRGP4 $78
ARGP4
ADDRGP4 s_preferences+632+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 123
;123:		break;
ADDRGP4 $137
JUMPV
LABELV $153
line 126
;124:
;125:	case ID_HIGHQUALITYSKY:
;126:		trap_Cvar_SetValue( "r_fastsky", !s_preferences.highqualitysky.curvalue );
ADDRGP4 $108
ARGP4
ADDRGP4 s_preferences+952+60
INDIRI4
CNSTI4 0
NEI4 $157
ADDRLP4 12
CNSTI4 1
ASGNI4
ADDRGP4 $158
JUMPV
LABELV $157
ADDRLP4 12
CNSTI4 0
ASGNI4
LABELV $158
ADDRLP4 12
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 127
;127:		break;
ADDRGP4 $137
JUMPV
LABELV $159
line 130
;128:
;129:	case ID_EJECTINGBRASS:
;130:		if ( s_preferences.brass.curvalue )
ADDRGP4 s_preferences+696+60
INDIRI4
CNSTI4 0
EQI4 $160
line 131
;131:			trap_Cvar_Reset( "cg_brassTime" );
ADDRGP4 $84
ARGP4
ADDRGP4 trap_Cvar_Reset
CALLV
pop
ADDRGP4 $137
JUMPV
LABELV $160
line 133
;132:		else
;133:			trap_Cvar_SetValue( "cg_brassTime", 0 );
ADDRGP4 $84
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 134
;134:		break;
ADDRGP4 $137
JUMPV
LABELV $164
line 137
;135:
;136:	case ID_WALLMARKS:
;137:		trap_Cvar_SetValue( "cg_marks", s_preferences.wallmarks.curvalue );
ADDRGP4 $90
ARGP4
ADDRGP4 s_preferences+760+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 138
;138:		break;
ADDRGP4 $137
JUMPV
LABELV $167
line 141
;139:
;140:	case ID_DYNAMICLIGHTS:
;141:		trap_Cvar_SetValue( "r_dynamiclight", s_preferences.dynamiclights.curvalue );
ADDRGP4 $102
ARGP4
ADDRGP4 s_preferences+824+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 142
;142:		break;		
ADDRGP4 $137
JUMPV
LABELV $170
line 145
;143:
;144:	case ID_IDENTIFYTARGET:
;145:		trap_Cvar_SetValue( "cg_drawCrosshairNames", s_preferences.identifytarget.curvalue );
ADDRGP4 $96
ARGP4
ADDRGP4 s_preferences+888+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 146
;146:		break;
ADDRGP4 $137
JUMPV
LABELV $173
line 149
;147:
;148:	case ID_SYNCEVERYFRAME:
;149:		trap_Cvar_SetValue( "r_finish", s_preferences.synceveryframe.curvalue );
ADDRGP4 $114
ARGP4
ADDRGP4 s_preferences+1016+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 150
;150:		break;
ADDRGP4 $137
JUMPV
LABELV $176
line 153
;151:
;152:	case ID_FORCEMODEL:
;153:		trap_Cvar_SetValue( "cg_forcemodel", s_preferences.forcemodel.curvalue );
ADDRGP4 $120
ARGP4
ADDRGP4 s_preferences+1080+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 154
;154:		break;
ADDRGP4 $137
JUMPV
LABELV $179
line 157
;155:
;156:	case ID_DRAWTEAMOVERLAY:
;157:		trap_Cvar_SetValue( "cg_drawTeamOverlay", s_preferences.drawteamoverlay.curvalue );
ADDRGP4 $126
ARGP4
ADDRGP4 s_preferences+1144+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 158
;158:		break;
ADDRGP4 $137
JUMPV
LABELV $182
line 161
;159:
;160:	case ID_ALLOWDOWNLOAD:
;161:		trap_Cvar_SetValue( "cl_allowDownload", s_preferences.allowdownload.curvalue );
ADDRGP4 $129
ARGP4
ADDRGP4 s_preferences+1240+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 162
;162:		trap_Cvar_SetValue( "sv_allowDownload", s_preferences.allowdownload.curvalue );
ADDRGP4 $185
ARGP4
ADDRGP4 s_preferences+1240+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 163
;163:		break;
ADDRGP4 $137
JUMPV
LABELV $188
line 166
;164:
;165:	case ID_BACK:
;166:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 167
;167:		break;
LABELV $136
LABELV $137
line 169
;168:	}
;169:}
LABELV $133
endproc Preferences_Event 16 8
proc Crosshair_Draw 48 20
line 177
;170:
;171:
;172:/*
;173:=================
;174:Crosshair_Draw
;175:=================
;176:*/
;177:static void Crosshair_Draw( void *self ) {
line 184
;178:	menulist_s	*s;
;179:	float		*color;
;180:	int			x, y;
;181:	int			style;
;182:	qboolean	focus;
;183:
;184:	s = (menulist_s *)self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 185
;185:	x = s->generic.x;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 186
;186:	y =	s->generic.y;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 188
;187:
;188:	style = UI_SMALLFONT;
ADDRLP4 20
CNSTI4 16
ASGNI4
line 189
;189:	focus = (s->generic.parent->cursor == s->generic.menuPosition);
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
NEI4 $193
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRGP4 $194
JUMPV
LABELV $193
ADDRLP4 24
CNSTI4 0
ASGNI4
LABELV $194
ADDRLP4 16
ADDRLP4 24
INDIRI4
ASGNI4
line 191
;190:
;191:	if ( s->generic.flags & QMF_GRAYED )
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 8192
BANDU4
CNSTU4 0
EQU4 $195
line 192
;192:		color = text_color_disabled;
ADDRLP4 12
ADDRGP4 text_color_disabled
ASGNP4
ADDRGP4 $196
JUMPV
LABELV $195
line 193
;193:	else if ( focus )
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $197
line 194
;194:	{
line 195
;195:		color = text_color_highlight;
ADDRLP4 12
ADDRGP4 text_color_highlight
ASGNP4
line 196
;196:		style |= UI_PULSE;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 197
;197:	}
ADDRGP4 $198
JUMPV
LABELV $197
line 198
;198:	else if ( s->generic.flags & QMF_BLINK )
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 1
BANDU4
CNSTU4 0
EQU4 $199
line 199
;199:	{
line 200
;200:		color = text_color_highlight;
ADDRLP4 12
ADDRGP4 text_color_highlight
ASGNP4
line 201
;201:		style |= UI_BLINK;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 202
;202:	}
ADDRGP4 $200
JUMPV
LABELV $199
line 204
;203:	else
;204:		color = text_color_normal;
ADDRLP4 12
ADDRGP4 text_color_normal
ASGNP4
LABELV $200
LABELV $198
LABELV $196
line 206
;205:
;206:	if ( focus )
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $201
line 207
;207:	{
line 209
;208:		// draw cursor
;209:		UI_FillRect( s->generic.left, s->generic.top, s->generic.right-s->generic.left+1, s->generic.bottom-s->generic.top+1, listbar_color ); 
ADDRLP4 36
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 44
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
SUBI4
ADDRLP4 44
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
SUBI4
ADDRLP4 44
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 listbar_color
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 210
;210:		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color);
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
CNSTI4 13
ARGI4
CNSTI4 4113
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 211
;211:	}
LABELV $201
line 213
;212:
;213:	UI_DrawString( x - SMALLCHAR_WIDTH, y, s->generic.name, style|UI_RIGHT, color );
ADDRLP4 4
INDIRI4
CNSTI4 8
SUBI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
INDIRI4
CNSTI4 2
BORI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 214
;214:	if( !s->curvalue ) {
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
CNSTI4 0
NEI4 $203
line 215
;215:		return;
ADDRGP4 $191
JUMPV
LABELV $203
line 217
;216:	}
;217:	UI_DrawHandlePic( x + SMALLCHAR_WIDTH, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue] );
ADDRLP4 4
INDIRI4
CNSTI4 8
ADDI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CNSTI4 4
SUBI4
CVIF4 4
ARGF4
ADDRLP4 32
CNSTF4 1103101952
ASGNF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_preferences+1392
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 218
;218:}
LABELV $191
endproc Crosshair_Draw 48 20
proc Preferences_MenuInit 8 12
line 221
;219:
;220:
;221:static void Preferences_MenuInit( void ) {
line 224
;222:	int				y;
;223:
;224:	memset( &s_preferences, 0 ,sizeof(preferences_t) );
ADDRGP4 s_preferences
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1432
ARGI4
ADDRGP4 memset
CALLP4
pop
line 226
;225:
;226:	Preferences_Cache();
ADDRGP4 Preferences_Cache
CALLV
pop
line 228
;227:
;228:	s_preferences.menu.wrapAround = qtrue;
ADDRGP4 s_preferences+276
CNSTI4 1
ASGNI4
line 229
;229:	s_preferences.menu.fullscreen = qtrue;
ADDRGP4 s_preferences+280
CNSTI4 1
ASGNI4
line 231
;230:
;231:	s_preferences.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_preferences+288
CNSTI4 10
ASGNI4
line 232
;232:	s_preferences.banner.generic.x	   = 320;
ADDRGP4 s_preferences+288+12
CNSTI4 320
ASGNI4
line 233
;233:	s_preferences.banner.generic.y	   = 16;
ADDRGP4 s_preferences+288+16
CNSTI4 16
ASGNI4
line 234
;234:	s_preferences.banner.string		   = "GAME OPTIONS";
ADDRGP4 s_preferences+288+60
ADDRGP4 $216
ASGNP4
line 235
;235:	s_preferences.banner.color         = color_white;
ADDRGP4 s_preferences+288+68
ADDRGP4 color_white
ASGNP4
line 236
;236:	s_preferences.banner.style         = UI_CENTER;
ADDRGP4 s_preferences+288+64
CNSTI4 1
ASGNI4
line 238
;237:
;238:	s_preferences.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_preferences+360
CNSTI4 6
ASGNI4
line 239
;239:	s_preferences.framel.generic.name  = ART_FRAMEL;
ADDRGP4 s_preferences+360+4
ADDRGP4 $224
ASGNP4
line 240
;240:	s_preferences.framel.generic.flags = QMF_INACTIVE;
ADDRGP4 s_preferences+360+44
CNSTU4 16384
ASGNU4
line 241
;241:	s_preferences.framel.generic.x	   = 0;
ADDRGP4 s_preferences+360+12
CNSTI4 0
ASGNI4
line 242
;242:	s_preferences.framel.generic.y	   = 78;
ADDRGP4 s_preferences+360+16
CNSTI4 78
ASGNI4
line 243
;243:	s_preferences.framel.width  	   = 256;
ADDRGP4 s_preferences+360+76
CNSTI4 256
ASGNI4
line 244
;244:	s_preferences.framel.height  	   = 329;
ADDRGP4 s_preferences+360+80
CNSTI4 329
ASGNI4
line 246
;245:
;246:	s_preferences.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_preferences+448
CNSTI4 6
ASGNI4
line 247
;247:	s_preferences.framer.generic.name  = ART_FRAMER;
ADDRGP4 s_preferences+448+4
ADDRGP4 $238
ASGNP4
line 248
;248:	s_preferences.framer.generic.flags = QMF_INACTIVE;
ADDRGP4 s_preferences+448+44
CNSTU4 16384
ASGNU4
line 249
;249:	s_preferences.framer.generic.x	   = 376;
ADDRGP4 s_preferences+448+12
CNSTI4 376
ASGNI4
line 250
;250:	s_preferences.framer.generic.y	   = 76;
ADDRGP4 s_preferences+448+16
CNSTI4 76
ASGNI4
line 251
;251:	s_preferences.framer.width  	   = 256;
ADDRGP4 s_preferences+448+76
CNSTI4 256
ASGNI4
line 252
;252:	s_preferences.framer.height  	   = 334;
ADDRGP4 s_preferences+448+80
CNSTI4 334
ASGNI4
line 254
;253:
;254:	y = 144;
ADDRLP4 0
CNSTI4 144
ASGNI4
line 255
;255:	s_preferences.crosshair.generic.type		= MTYPE_TEXT;
ADDRGP4 s_preferences+536
CNSTI4 7
ASGNI4
line 256
;256:	s_preferences.crosshair.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
ADDRGP4 s_preferences+536+44
CNSTU4 98562
ASGNU4
line 257
;257:	s_preferences.crosshair.generic.x			= PREFERENCES_X_POS;
ADDRGP4 s_preferences+536+12
CNSTI4 360
ASGNI4
line 258
;258:	s_preferences.crosshair.generic.y			= y;
ADDRGP4 s_preferences+536+16
ADDRLP4 0
INDIRI4
ASGNI4
line 259
;259:	s_preferences.crosshair.generic.name		= "Crosshair:";
ADDRGP4 s_preferences+536+4
ADDRGP4 $258
ASGNP4
line 260
;260:	s_preferences.crosshair.generic.callback	= Preferences_Event;
ADDRGP4 s_preferences+536+48
ADDRGP4 Preferences_Event
ASGNP4
line 261
;261:	s_preferences.crosshair.generic.ownerdraw	= Crosshair_Draw;
ADDRGP4 s_preferences+536+56
ADDRGP4 Crosshair_Draw
ASGNP4
line 262
;262:	s_preferences.crosshair.generic.id			= ID_CROSSHAIR;
ADDRGP4 s_preferences+536+8
CNSTI4 127
ASGNI4
line 263
;263:	s_preferences.crosshair.generic.top			= y - 4;
ADDRGP4 s_preferences+536+24
ADDRLP4 0
INDIRI4
CNSTI4 4
SUBI4
ASGNI4
line 264
;264:	s_preferences.crosshair.generic.bottom		= y + 20;
ADDRGP4 s_preferences+536+32
ADDRLP4 0
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
line 265
;265:	s_preferences.crosshair.generic.left		= PREFERENCES_X_POS - ( ( strlen(s_preferences.crosshair.generic.name) + 1 ) * SMALLCHAR_WIDTH );
ADDRGP4 s_preferences+536+4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 s_preferences+536+20
CNSTI4 360
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
CNSTI4 8
ADDI4
SUBI4
ASGNI4
line 266
;266:	s_preferences.crosshair.generic.right		= PREFERENCES_X_POS + 48;
ADDRGP4 s_preferences+536+28
CNSTI4 408
ASGNI4
line 268
;267:
;268:	y += BIGCHAR_HEIGHT+2+4;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 22
ADDI4
ASGNI4
line 269
;269:	s_preferences.simpleitems.generic.type        = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+632
CNSTI4 5
ASGNI4
line 270
;270:	s_preferences.simpleitems.generic.name	      = "Simple Items:";
ADDRGP4 s_preferences+632+4
ADDRGP4 $278
ASGNP4
line 271
;271:	s_preferences.simpleitems.generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+632+44
CNSTU4 258
ASGNU4
line 272
;272:	s_preferences.simpleitems.generic.callback    = Preferences_Event;
ADDRGP4 s_preferences+632+48
ADDRGP4 Preferences_Event
ASGNP4
line 273
;273:	s_preferences.simpleitems.generic.id          = ID_SIMPLEITEMS;
ADDRGP4 s_preferences+632+8
CNSTI4 128
ASGNI4
line 274
;274:	s_preferences.simpleitems.generic.x	          = PREFERENCES_X_POS;
ADDRGP4 s_preferences+632+12
CNSTI4 360
ASGNI4
line 275
;275:	s_preferences.simpleitems.generic.y	          = y;
ADDRGP4 s_preferences+632+16
ADDRLP4 0
INDIRI4
ASGNI4
line 277
;276:
;277:	y += BIGCHAR_HEIGHT;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 278
;278:	s_preferences.wallmarks.generic.type          = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+760
CNSTI4 5
ASGNI4
line 279
;279:	s_preferences.wallmarks.generic.name	      = "Marks on Walls:";
ADDRGP4 s_preferences+760+4
ADDRGP4 $292
ASGNP4
line 280
;280:	s_preferences.wallmarks.generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+760+44
CNSTU4 258
ASGNU4
line 281
;281:	s_preferences.wallmarks.generic.callback      = Preferences_Event;
ADDRGP4 s_preferences+760+48
ADDRGP4 Preferences_Event
ASGNP4
line 282
;282:	s_preferences.wallmarks.generic.id            = ID_WALLMARKS;
ADDRGP4 s_preferences+760+8
CNSTI4 131
ASGNI4
line 283
;283:	s_preferences.wallmarks.generic.x	          = PREFERENCES_X_POS;
ADDRGP4 s_preferences+760+12
CNSTI4 360
ASGNI4
line 284
;284:	s_preferences.wallmarks.generic.y	          = y;
ADDRGP4 s_preferences+760+16
ADDRLP4 0
INDIRI4
ASGNI4
line 286
;285:
;286:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 287
;287:	s_preferences.brass.generic.type              = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+696
CNSTI4 5
ASGNI4
line 288
;288:	s_preferences.brass.generic.name	          = "Ejecting Brass:";
ADDRGP4 s_preferences+696+4
ADDRGP4 $306
ASGNP4
line 289
;289:	s_preferences.brass.generic.flags	          = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+696+44
CNSTU4 258
ASGNU4
line 290
;290:	s_preferences.brass.generic.callback          = Preferences_Event;
ADDRGP4 s_preferences+696+48
ADDRGP4 Preferences_Event
ASGNP4
line 291
;291:	s_preferences.brass.generic.id                = ID_EJECTINGBRASS;
ADDRGP4 s_preferences+696+8
CNSTI4 130
ASGNI4
line 292
;292:	s_preferences.brass.generic.x	              = PREFERENCES_X_POS;
ADDRGP4 s_preferences+696+12
CNSTI4 360
ASGNI4
line 293
;293:	s_preferences.brass.generic.y	              = y;
ADDRGP4 s_preferences+696+16
ADDRLP4 0
INDIRI4
ASGNI4
line 295
;294:
;295:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 296
;296:	s_preferences.dynamiclights.generic.type      = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+824
CNSTI4 5
ASGNI4
line 297
;297:	s_preferences.dynamiclights.generic.name	  = "Dynamic Lights:";
ADDRGP4 s_preferences+824+4
ADDRGP4 $320
ASGNP4
line 298
;298:	s_preferences.dynamiclights.generic.flags     = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+824+44
CNSTU4 258
ASGNU4
line 299
;299:	s_preferences.dynamiclights.generic.callback  = Preferences_Event;
ADDRGP4 s_preferences+824+48
ADDRGP4 Preferences_Event
ASGNP4
line 300
;300:	s_preferences.dynamiclights.generic.id        = ID_DYNAMICLIGHTS;
ADDRGP4 s_preferences+824+8
CNSTI4 132
ASGNI4
line 301
;301:	s_preferences.dynamiclights.generic.x	      = PREFERENCES_X_POS;
ADDRGP4 s_preferences+824+12
CNSTI4 360
ASGNI4
line 302
;302:	s_preferences.dynamiclights.generic.y	      = y;
ADDRGP4 s_preferences+824+16
ADDRLP4 0
INDIRI4
ASGNI4
line 304
;303:
;304:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 305
;305:	s_preferences.identifytarget.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+888
CNSTI4 5
ASGNI4
line 306
;306:	s_preferences.identifytarget.generic.name	  = "Identify Target:";
ADDRGP4 s_preferences+888+4
ADDRGP4 $334
ASGNP4
line 307
;307:	s_preferences.identifytarget.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+888+44
CNSTU4 258
ASGNU4
line 308
;308:	s_preferences.identifytarget.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+888+48
ADDRGP4 Preferences_Event
ASGNP4
line 309
;309:	s_preferences.identifytarget.generic.id       = ID_IDENTIFYTARGET;
ADDRGP4 s_preferences+888+8
CNSTI4 133
ASGNI4
line 310
;310:	s_preferences.identifytarget.generic.x	      = PREFERENCES_X_POS;
ADDRGP4 s_preferences+888+12
CNSTI4 360
ASGNI4
line 311
;311:	s_preferences.identifytarget.generic.y	      = y;
ADDRGP4 s_preferences+888+16
ADDRLP4 0
INDIRI4
ASGNI4
line 313
;312:
;313:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 314
;314:	s_preferences.highqualitysky.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+952
CNSTI4 5
ASGNI4
line 315
;315:	s_preferences.highqualitysky.generic.name	  = "High Quality Sky:";
ADDRGP4 s_preferences+952+4
ADDRGP4 $348
ASGNP4
line 316
;316:	s_preferences.highqualitysky.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+952+44
CNSTU4 258
ASGNU4
line 317
;317:	s_preferences.highqualitysky.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+952+48
ADDRGP4 Preferences_Event
ASGNP4
line 318
;318:	s_preferences.highqualitysky.generic.id       = ID_HIGHQUALITYSKY;
ADDRGP4 s_preferences+952+8
CNSTI4 129
ASGNI4
line 319
;319:	s_preferences.highqualitysky.generic.x	      = PREFERENCES_X_POS;
ADDRGP4 s_preferences+952+12
CNSTI4 360
ASGNI4
line 320
;320:	s_preferences.highqualitysky.generic.y	      = y;
ADDRGP4 s_preferences+952+16
ADDRLP4 0
INDIRI4
ASGNI4
line 322
;321:
;322:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 323
;323:	s_preferences.synceveryframe.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+1016
CNSTI4 5
ASGNI4
line 324
;324:	s_preferences.synceveryframe.generic.name	  = "Sync Every Frame:";
ADDRGP4 s_preferences+1016+4
ADDRGP4 $362
ASGNP4
line 325
;325:	s_preferences.synceveryframe.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+1016+44
CNSTU4 258
ASGNU4
line 326
;326:	s_preferences.synceveryframe.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+1016+48
ADDRGP4 Preferences_Event
ASGNP4
line 327
;327:	s_preferences.synceveryframe.generic.id       = ID_SYNCEVERYFRAME;
ADDRGP4 s_preferences+1016+8
CNSTI4 134
ASGNI4
line 328
;328:	s_preferences.synceveryframe.generic.x	      = PREFERENCES_X_POS;
ADDRGP4 s_preferences+1016+12
CNSTI4 360
ASGNI4
line 329
;329:	s_preferences.synceveryframe.generic.y	      = y;
ADDRGP4 s_preferences+1016+16
ADDRLP4 0
INDIRI4
ASGNI4
line 331
;330:
;331:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 332
;332:	s_preferences.forcemodel.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+1080
CNSTI4 5
ASGNI4
line 333
;333:	s_preferences.forcemodel.generic.name	  = "Force Player Models:";
ADDRGP4 s_preferences+1080+4
ADDRGP4 $376
ASGNP4
line 334
;334:	s_preferences.forcemodel.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+1080+44
CNSTU4 258
ASGNU4
line 335
;335:	s_preferences.forcemodel.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+1080+48
ADDRGP4 Preferences_Event
ASGNP4
line 336
;336:	s_preferences.forcemodel.generic.id       = ID_FORCEMODEL;
ADDRGP4 s_preferences+1080+8
CNSTI4 135
ASGNI4
line 337
;337:	s_preferences.forcemodel.generic.x	      = PREFERENCES_X_POS;
ADDRGP4 s_preferences+1080+12
CNSTI4 360
ASGNI4
line 338
;338:	s_preferences.forcemodel.generic.y	      = y;
ADDRGP4 s_preferences+1080+16
ADDRLP4 0
INDIRI4
ASGNI4
line 340
;339:
;340:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 341
;341:	s_preferences.drawteamoverlay.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_preferences+1144
CNSTI4 3
ASGNI4
line 342
;342:	s_preferences.drawteamoverlay.generic.name	   = "Draw Team Overlay:";
ADDRGP4 s_preferences+1144+4
ADDRGP4 $390
ASGNP4
line 343
;343:	s_preferences.drawteamoverlay.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+1144+44
CNSTU4 258
ASGNU4
line 344
;344:	s_preferences.drawteamoverlay.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+1144+48
ADDRGP4 Preferences_Event
ASGNP4
line 345
;345:	s_preferences.drawteamoverlay.generic.id       = ID_DRAWTEAMOVERLAY;
ADDRGP4 s_preferences+1144+8
CNSTI4 136
ASGNI4
line 346
;346:	s_preferences.drawteamoverlay.generic.x	       = PREFERENCES_X_POS;
ADDRGP4 s_preferences+1144+12
CNSTI4 360
ASGNI4
line 347
;347:	s_preferences.drawteamoverlay.generic.y	       = y;
ADDRGP4 s_preferences+1144+16
ADDRLP4 0
INDIRI4
ASGNI4
line 348
;348:	s_preferences.drawteamoverlay.itemnames			= teamoverlay_names;
ADDRGP4 s_preferences+1144+76
ADDRGP4 teamoverlay_names
ASGNP4
line 350
;349:
;350:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 351
;351:	s_preferences.allowdownload.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_preferences+1240
CNSTI4 5
ASGNI4
line 352
;352:	s_preferences.allowdownload.generic.name	   = "Automatic Downloading:";
ADDRGP4 s_preferences+1240+4
ADDRGP4 $406
ASGNP4
line 353
;353:	s_preferences.allowdownload.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_preferences+1240+44
CNSTU4 258
ASGNU4
line 354
;354:	s_preferences.allowdownload.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+1240+48
ADDRGP4 Preferences_Event
ASGNP4
line 355
;355:	s_preferences.allowdownload.generic.id       = ID_ALLOWDOWNLOAD;
ADDRGP4 s_preferences+1240+8
CNSTI4 137
ASGNI4
line 356
;356:	s_preferences.allowdownload.generic.x	       = PREFERENCES_X_POS;
ADDRGP4 s_preferences+1240+12
CNSTI4 360
ASGNI4
line 357
;357:	s_preferences.allowdownload.generic.y	       = y;
ADDRGP4 s_preferences+1240+16
ADDRLP4 0
INDIRI4
ASGNI4
line 359
;358:
;359:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 360
;360:	s_preferences.back.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_preferences+1304
CNSTI4 6
ASGNI4
line 361
;361:	s_preferences.back.generic.name     = ART_BACK0;
ADDRGP4 s_preferences+1304+4
ADDRGP4 $420
ASGNP4
line 362
;362:	s_preferences.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_preferences+1304+44
CNSTU4 260
ASGNU4
line 363
;363:	s_preferences.back.generic.callback = Preferences_Event;
ADDRGP4 s_preferences+1304+48
ADDRGP4 Preferences_Event
ASGNP4
line 364
;364:	s_preferences.back.generic.id	    = ID_BACK;
ADDRGP4 s_preferences+1304+8
CNSTI4 138
ASGNI4
line 365
;365:	s_preferences.back.generic.x		= 0;
ADDRGP4 s_preferences+1304+12
CNSTI4 0
ASGNI4
line 366
;366:	s_preferences.back.generic.y		= 480-64;
ADDRGP4 s_preferences+1304+16
CNSTI4 416
ASGNI4
line 367
;367:	s_preferences.back.width  		    = 128;
ADDRGP4 s_preferences+1304+76
CNSTI4 128
ASGNI4
line 368
;368:	s_preferences.back.height  		    = 64;
ADDRGP4 s_preferences+1304+80
CNSTI4 64
ASGNI4
line 369
;369:	s_preferences.back.focuspic         = ART_BACK1;
ADDRGP4 s_preferences+1304+60
ADDRGP4 $437
ASGNP4
line 371
;370:
;371:	Menu_AddItem( &s_preferences.menu, &s_preferences.banner );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 372
;372:	Menu_AddItem( &s_preferences.menu, &s_preferences.framel );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 373
;373:	Menu_AddItem( &s_preferences.menu, &s_preferences.framer );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 375
;374:
;375:	Menu_AddItem( &s_preferences.menu, &s_preferences.crosshair );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 376
;376:	Menu_AddItem( &s_preferences.menu, &s_preferences.simpleitems );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+632
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 377
;377:	Menu_AddItem( &s_preferences.menu, &s_preferences.wallmarks );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+760
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 378
;378:	Menu_AddItem( &s_preferences.menu, &s_preferences.brass );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+696
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 379
;379:	Menu_AddItem( &s_preferences.menu, &s_preferences.dynamiclights );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+824
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 380
;380:	Menu_AddItem( &s_preferences.menu, &s_preferences.identifytarget );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+888
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 381
;381:	Menu_AddItem( &s_preferences.menu, &s_preferences.highqualitysky );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+952
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 382
;382:	Menu_AddItem( &s_preferences.menu, &s_preferences.synceveryframe );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+1016
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 383
;383:	Menu_AddItem( &s_preferences.menu, &s_preferences.forcemodel );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+1080
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 384
;384:	Menu_AddItem( &s_preferences.menu, &s_preferences.drawteamoverlay );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+1144
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 385
;385:	Menu_AddItem( &s_preferences.menu, &s_preferences.allowdownload );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+1240
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 387
;386:
;387:	Menu_AddItem( &s_preferences.menu, &s_preferences.back );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 s_preferences+1304
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 389
;388:
;389:	Preferences_SetMenuItems();
ADDRGP4 Preferences_SetMenuItems
CALLV
pop
line 390
;390:}
LABELV $206
endproc Preferences_MenuInit 8 12
export Preferences_Cache
proc Preferences_Cache 16 8
line 398
;391:
;392:
;393:/*
;394:===============
;395:Preferences_Cache
;396:===============
;397:*/
;398:void Preferences_Cache( void ) {
line 401
;399:	int		n;
;400:
;401:	trap_R_RegisterShaderNoMip( ART_FRAMEL );
ADDRGP4 $224
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 402
;402:	trap_R_RegisterShaderNoMip( ART_FRAMER );
ADDRGP4 $238
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 403
;403:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $420
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 404
;404:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $437
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 405
;405:	for( n = 0; n < NUM_CROSSHAIRS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $454
line 406
;406:		s_preferences.crosshairShader[n] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a' + n ) );
ADDRGP4 $459
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 97
ADDI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_preferences+1392
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 407
;407:	}
LABELV $455
line 405
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 10
LTI4 $454
line 408
;408:}
LABELV $453
endproc Preferences_Cache 16 8
export UI_PreferencesMenu
proc UI_PreferencesMenu 0 4
line 416
;409:
;410:
;411:/*
;412:===============
;413:UI_PreferencesMenu
;414:===============
;415:*/
;416:void UI_PreferencesMenu( void ) {
line 417
;417:	Preferences_MenuInit();
ADDRGP4 Preferences_MenuInit
CALLV
pop
line 418
;418:	UI_PushMenu( &s_preferences.menu );
ADDRGP4 s_preferences
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 419
;419:}
LABELV $460
endproc UI_PreferencesMenu 0 4
bss
align 4
LABELV s_preferences
skip 1432
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
LABELV $459
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
LABELV $437
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
LABELV $420
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
LABELV $406
byte 1 65
byte 1 117
byte 1 116
byte 1 111
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 32
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
LABELV $390
byte 1 68
byte 1 114
byte 1 97
byte 1 119
byte 1 32
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 79
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 97
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $376
byte 1 70
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 32
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $362
byte 1 83
byte 1 121
byte 1 110
byte 1 99
byte 1 32
byte 1 69
byte 1 118
byte 1 101
byte 1 114
byte 1 121
byte 1 32
byte 1 70
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $348
byte 1 72
byte 1 105
byte 1 103
byte 1 104
byte 1 32
byte 1 81
byte 1 117
byte 1 97
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 83
byte 1 107
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $334
byte 1 73
byte 1 100
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 102
byte 1 121
byte 1 32
byte 1 84
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $320
byte 1 68
byte 1 121
byte 1 110
byte 1 97
byte 1 109
byte 1 105
byte 1 99
byte 1 32
byte 1 76
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $306
byte 1 69
byte 1 106
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 66
byte 1 114
byte 1 97
byte 1 115
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $292
byte 1 77
byte 1 97
byte 1 114
byte 1 107
byte 1 115
byte 1 32
byte 1 111
byte 1 110
byte 1 32
byte 1 87
byte 1 97
byte 1 108
byte 1 108
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $278
byte 1 83
byte 1 105
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 32
byte 1 73
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $258
byte 1 67
byte 1 114
byte 1 111
byte 1 115
byte 1 115
byte 1 104
byte 1 97
byte 1 105
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $238
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
LABELV $224
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
LABELV $216
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
byte 1 115
byte 1 118
byte 1 95
byte 1 97
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 68
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $129
byte 1 99
byte 1 108
byte 1 95
byte 1 97
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 68
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $126
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 79
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $120
byte 1 99
byte 1 103
byte 1 95
byte 1 102
byte 1 111
byte 1 114
byte 1 99
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $114
byte 1 114
byte 1 95
byte 1 102
byte 1 105
byte 1 110
byte 1 105
byte 1 115
byte 1 104
byte 1 0
align 1
LABELV $108
byte 1 114
byte 1 95
byte 1 102
byte 1 97
byte 1 115
byte 1 116
byte 1 115
byte 1 107
byte 1 121
byte 1 0
align 1
LABELV $102
byte 1 114
byte 1 95
byte 1 100
byte 1 121
byte 1 110
byte 1 97
byte 1 109
byte 1 105
byte 1 99
byte 1 108
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $96
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
LABELV $90
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
LABELV $84
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
LABELV $78
byte 1 99
byte 1 103
byte 1 95
byte 1 115
byte 1 105
byte 1 109
byte 1 112
byte 1 108
byte 1 101
byte 1 73
byte 1 116
byte 1 101
byte 1 109
byte 1 115
byte 1 0
align 1
LABELV $75
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
LABELV $71
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 32
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 0
align 1
LABELV $70
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 114
byte 1 32
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $69
byte 1 117
byte 1 112
byte 1 112
byte 1 101
byte 1 114
byte 1 32
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $68
byte 1 111
byte 1 102
byte 1 102
byte 1 0
