export Com_Error
code
proc Com_Error 1032 12
file "../ui_atoms.c"
line 36
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
;23:/**********************************************************************
;24:	UI_ATOMS.C
;25:
;26:	User interface building blocks and support functions.
;27:**********************************************************************/
;28:#include "ui_local.h"
;29:
;30:uiStatic_t		uis;
;31:qboolean		m_entersound;		// after a frame, so caching won't disrupt the sound
;32:
;33:// these are here so the functions in q_shared.c can link
;34:#ifndef UI_HARD_LINKED
;35:
;36:void QDECL Com_Error( int level, const char *error, ... ) {
line 40
;37:	va_list		argptr;
;38:	char		text[1024];
;39:
;40:	va_start (argptr, error);
ADDRLP4 0
ADDRFP4 4+4
ASGNP4
line 41
;41:	vsprintf (text, error, argptr);
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 42
;42:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 44
;43:
;44:	trap_Error( va("%s", text) );
ADDRGP4 $69
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_Error
CALLV
pop
line 45
;45:}
LABELV $67
endproc Com_Error 1032 12
export Com_Printf
proc Com_Printf 1032 12
line 47
;46:
;47:void QDECL Com_Printf( const char *msg, ... ) {
line 51
;48:	va_list		argptr;
;49:	char		text[1024];
;50:
;51:	va_start (argptr, msg);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 52
;52:	vsprintf (text, msg, argptr);
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 53
;53:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 55
;54:
;55:	trap_Print( va("%s", text) );
ADDRGP4 $69
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 56
;56:}
LABELV $70
endproc Com_Printf 1032 12
export UI_ClampCvar
proc UI_ClampCvar 0 0
line 66
;57:
;58:#endif
;59:
;60:/*
;61:=================
;62:UI_ClampCvar
;63:=================
;64:*/
;65:float UI_ClampCvar( float min, float max, float value )
;66:{
line 67
;67:	if ( value < min ) return min;
ADDRFP4 8
INDIRF4
ADDRFP4 0
INDIRF4
GEF4 $73
ADDRFP4 0
INDIRF4
RETF4
ADDRGP4 $72
JUMPV
LABELV $73
line 68
;68:	if ( value > max ) return max;
ADDRFP4 8
INDIRF4
ADDRFP4 4
INDIRF4
LEF4 $75
ADDRFP4 4
INDIRF4
RETF4
ADDRGP4 $72
JUMPV
LABELV $75
line 69
;69:	return value;
ADDRFP4 8
INDIRF4
RETF4
LABELV $72
endproc UI_ClampCvar 0 0
export UI_StartDemoLoop
proc UI_StartDemoLoop 0 8
line 77
;70:}
;71:
;72:/*
;73:=================
;74:UI_StartDemoLoop
;75:=================
;76:*/
;77:void UI_StartDemoLoop( void ) {
line 78
;78:	trap_Cmd_ExecuteText( EXEC_APPEND, "d1\n" );
CNSTI4 2
ARGI4
ADDRGP4 $78
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 79
;79:}
LABELV $77
endproc UI_StartDemoLoop 0 8
export UI_PushMenu
proc UI_PushMenu 16 8
line 87
;80:
;81:/*
;82:=================
;83:UI_PushMenu
;84:=================
;85:*/
;86:void UI_PushMenu( menuframework_s *menu )
;87:{
line 92
;88:	int				i;
;89:	menucommon_s*	item;
;90:
;91:	// avoid stacking menus invoked by hotkeys
;92:	for (i=0 ; i<uis.menusp ; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $83
JUMPV
LABELV $80
line 93
;93:	{
line 94
;94:		if (uis.stack[i] == menu)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+24
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $85
line 95
;95:		{
line 96
;96:			uis.menusp = i;
ADDRGP4 uis+16
ADDRLP4 0
INDIRI4
ASGNI4
line 97
;97:			break;
ADDRGP4 $82
JUMPV
LABELV $85
line 99
;98:		}
;99:	}
LABELV $81
line 92
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $83
ADDRLP4 0
INDIRI4
ADDRGP4 uis+16
INDIRI4
LTI4 $80
LABELV $82
line 101
;100:
;101:	if (i == uis.menusp)
ADDRLP4 0
INDIRI4
ADDRGP4 uis+16
INDIRI4
NEI4 $89
line 102
;102:	{
line 103
;103:		if (uis.menusp >= MAX_MENUDEPTH)
ADDRGP4 uis+16
INDIRI4
CNSTI4 8
LTI4 $92
line 104
;104:			trap_Error("UI_PushMenu: menu stack overflow");
ADDRGP4 $95
ARGP4
ADDRGP4 trap_Error
CALLV
pop
LABELV $92
line 106
;105:
;106:		uis.stack[uis.menusp++] = menu;
ADDRLP4 12
ADDRGP4 uis+16
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRP4
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+24
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 107
;107:	}
LABELV $89
line 109
;108:
;109:	uis.activemenu = menu;
ADDRGP4 uis+20
ADDRFP4 0
INDIRP4
ASGNP4
line 112
;110:
;111:	// default cursor position
;112:	menu->cursor      = 0;
ADDRFP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 113
;113:	menu->cursor_prev = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 115
;114:
;115:	m_entersound = qtrue;
ADDRGP4 m_entersound
CNSTI4 1
ASGNI4
line 117
;116:
;117:	trap_Key_SetCatcher( KEYCATCH_UI );
CNSTI4 2
ARGI4
ADDRGP4 trap_Key_SetCatcher
CALLV
pop
line 120
;118:
;119:	// force first available item to have focus
;120:	for (i=0; i<menu->nitems; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $102
JUMPV
LABELV $99
line 121
;121:	{
line 122
;122:		item = (menucommon_s *)menu->items[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
ASGNP4
line 123
;123:		if (!(item->flags & (QMF_GRAYED|QMF_MOUSEONLY|QMF_INACTIVE)))
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 26624
BANDU4
CNSTU4 0
NEU4 $103
line 124
;124:		{
line 125
;125:			menu->cursor_prev = -1;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 -1
ASGNI4
line 126
;126:			Menu_SetCursor( menu, i );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 Menu_SetCursor
CALLV
pop
line 127
;127:			break;
ADDRGP4 $101
JUMPV
LABELV $103
line 129
;128:		}
;129:	}
LABELV $100
line 120
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $102
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
LTI4 $99
LABELV $101
line 131
;130:
;131:	uis.firstdraw = qtrue;
ADDRGP4 uis+11444
CNSTI4 1
ASGNI4
line 132
;132:}
LABELV $79
endproc UI_PushMenu 16 8
export UI_PopMenu
proc UI_PopMenu 4 8
line 140
;133:
;134:/*
;135:=================
;136:UI_PopMenu
;137:=================
;138:*/
;139:void UI_PopMenu (void)
;140:{
line 141
;141:	trap_S_StartLocalSound( menu_out_sound, CHAN_LOCAL_SOUND );
ADDRGP4 menu_out_sound
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
line 143
;142:
;143:	uis.menusp--;
ADDRLP4 0
ADDRGP4 uis+16
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 145
;144:
;145:	if (uis.menusp < 0)
ADDRGP4 uis+16
INDIRI4
CNSTI4 0
GEI4 $108
line 146
;146:		trap_Error ("UI_PopMenu: menu stack underflow");
ADDRGP4 $111
ARGP4
ADDRGP4 trap_Error
CALLV
pop
LABELV $108
line 148
;147:
;148:	if (uis.menusp) {
ADDRGP4 uis+16
INDIRI4
CNSTI4 0
EQI4 $112
line 149
;149:		uis.activemenu = uis.stack[uis.menusp-1];
ADDRGP4 uis+20
ADDRGP4 uis+16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+24-4
ADDP4
INDIRP4
ASGNP4
line 150
;150:		uis.firstdraw = qtrue;
ADDRGP4 uis+11444
CNSTI4 1
ASGNI4
line 151
;151:	}
ADDRGP4 $113
JUMPV
LABELV $112
line 152
;152:	else {
line 153
;153:		UI_ForceMenuOff ();
ADDRGP4 UI_ForceMenuOff
CALLV
pop
line 154
;154:	}
LABELV $113
line 155
;155:}
LABELV $106
endproc UI_PopMenu 4 8
export UI_ForceMenuOff
proc UI_ForceMenuOff 4 8
line 158
;156:
;157:void UI_ForceMenuOff (void)
;158:{
line 159
;159:	uis.menusp     = 0;
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
line 160
;160:	uis.activemenu = NULL;
ADDRGP4 uis+20
CNSTP4 0
ASGNP4
line 162
;161:
;162:	trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
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
line 163
;163:	trap_Key_ClearStates();
ADDRGP4 trap_Key_ClearStates
CALLV
pop
line 164
;164:	trap_Cvar_Set( "cl_paused", "0" );
ADDRGP4 $123
ARGP4
ADDRGP4 $124
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 165
;165:}
LABELV $120
endproc UI_ForceMenuOff 4 8
export UI_LerpColor
proc UI_LerpColor 12 0
line 173
;166:
;167:/*
;168:=================
;169:UI_LerpColor
;170:=================
;171:*/
;172:void UI_LerpColor(vec4_t a, vec4_t b, vec4_t c, float t)
;173:{
line 177
;174:	int i;
;175:
;176:	// lerp and clamp each component
;177:	for (i=0; i<4; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $126
line 178
;178:	{
line 179
;179:		c[i] = a[i] + t*(b[i]-a[i]);
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ASGNF4
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 8
INDIRF4
ADDRFP4 12
INDIRF4
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 180
;180:		if (c[i] < 0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $130
line 181
;181:			c[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
CNSTF4 0
ASGNF4
ADDRGP4 $131
JUMPV
LABELV $130
line 182
;182:		else if (c[i] > 1.0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
CNSTF4 1065353216
LEF4 $132
line 183
;183:			c[i] = 1.0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
CNSTF4 1065353216
ASGNF4
LABELV $132
LABELV $131
line 184
;184:	}
LABELV $127
line 177
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $126
line 185
;185:}
LABELV $125
endproc UI_LerpColor 12 0
data
align 4
LABELV propMap
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 -1
byte 4 0
byte 4 0
byte 4 8
byte 4 11
byte 4 122
byte 4 7
byte 4 154
byte 4 181
byte 4 14
byte 4 55
byte 4 122
byte 4 17
byte 4 79
byte 4 122
byte 4 18
byte 4 101
byte 4 122
byte 4 23
byte 4 153
byte 4 122
byte 4 18
byte 4 9
byte 4 93
byte 4 7
byte 4 207
byte 4 122
byte 4 8
byte 4 230
byte 4 122
byte 4 9
byte 4 177
byte 4 122
byte 4 18
byte 4 30
byte 4 152
byte 4 18
byte 4 85
byte 4 181
byte 4 7
byte 4 34
byte 4 93
byte 4 11
byte 4 110
byte 4 181
byte 4 6
byte 4 130
byte 4 152
byte 4 14
byte 4 22
byte 4 64
byte 4 17
byte 4 41
byte 4 64
byte 4 12
byte 4 58
byte 4 64
byte 4 17
byte 4 78
byte 4 64
byte 4 18
byte 4 98
byte 4 64
byte 4 19
byte 4 120
byte 4 64
byte 4 18
byte 4 141
byte 4 64
byte 4 18
byte 4 204
byte 4 64
byte 4 16
byte 4 162
byte 4 64
byte 4 17
byte 4 182
byte 4 64
byte 4 18
byte 4 59
byte 4 181
byte 4 7
byte 4 35
byte 4 181
byte 4 7
byte 4 203
byte 4 152
byte 4 14
byte 4 56
byte 4 93
byte 4 14
byte 4 228
byte 4 152
byte 4 14
byte 4 177
byte 4 181
byte 4 18
byte 4 28
byte 4 122
byte 4 22
byte 4 5
byte 4 4
byte 4 18
byte 4 27
byte 4 4
byte 4 18
byte 4 48
byte 4 4
byte 4 18
byte 4 69
byte 4 4
byte 4 17
byte 4 90
byte 4 4
byte 4 13
byte 4 106
byte 4 4
byte 4 13
byte 4 121
byte 4 4
byte 4 18
byte 4 143
byte 4 4
byte 4 17
byte 4 164
byte 4 4
byte 4 8
byte 4 175
byte 4 4
byte 4 16
byte 4 195
byte 4 4
byte 4 18
byte 4 216
byte 4 4
byte 4 12
byte 4 230
byte 4 4
byte 4 23
byte 4 6
byte 4 34
byte 4 18
byte 4 27
byte 4 34
byte 4 18
byte 4 48
byte 4 34
byte 4 18
byte 4 68
byte 4 34
byte 4 18
byte 4 90
byte 4 34
byte 4 17
byte 4 110
byte 4 34
byte 4 18
byte 4 130
byte 4 34
byte 4 14
byte 4 146
byte 4 34
byte 4 18
byte 4 166
byte 4 34
byte 4 19
byte 4 185
byte 4 34
byte 4 29
byte 4 215
byte 4 34
byte 4 18
byte 4 234
byte 4 34
byte 4 18
byte 4 5
byte 4 64
byte 4 14
byte 4 60
byte 4 152
byte 4 7
byte 4 106
byte 4 151
byte 4 13
byte 4 83
byte 4 152
byte 4 7
byte 4 128
byte 4 122
byte 4 17
byte 4 4
byte 4 152
byte 4 21
byte 4 134
byte 4 181
byte 4 5
byte 4 5
byte 4 4
byte 4 18
byte 4 27
byte 4 4
byte 4 18
byte 4 48
byte 4 4
byte 4 18
byte 4 69
byte 4 4
byte 4 17
byte 4 90
byte 4 4
byte 4 13
byte 4 106
byte 4 4
byte 4 13
byte 4 121
byte 4 4
byte 4 18
byte 4 143
byte 4 4
byte 4 17
byte 4 164
byte 4 4
byte 4 8
byte 4 175
byte 4 4
byte 4 16
byte 4 195
byte 4 4
byte 4 18
byte 4 216
byte 4 4
byte 4 12
byte 4 230
byte 4 4
byte 4 23
byte 4 6
byte 4 34
byte 4 18
byte 4 27
byte 4 34
byte 4 18
byte 4 48
byte 4 34
byte 4 18
byte 4 68
byte 4 34
byte 4 18
byte 4 90
byte 4 34
byte 4 17
byte 4 110
byte 4 34
byte 4 18
byte 4 130
byte 4 34
byte 4 14
byte 4 146
byte 4 34
byte 4 18
byte 4 166
byte 4 34
byte 4 19
byte 4 185
byte 4 34
byte 4 29
byte 4 215
byte 4 34
byte 4 18
byte 4 234
byte 4 34
byte 4 18
byte 4 5
byte 4 64
byte 4 14
byte 4 153
byte 4 152
byte 4 13
byte 4 11
byte 4 181
byte 4 5
byte 4 180
byte 4 152
byte 4 13
byte 4 79
byte 4 93
byte 4 17
byte 4 0
byte 4 0
byte 4 -1
align 4
LABELV propMapB
byte 4 11
byte 4 12
byte 4 33
byte 4 49
byte 4 12
byte 4 31
byte 4 85
byte 4 12
byte 4 31
byte 4 120
byte 4 12
byte 4 30
byte 4 156
byte 4 12
byte 4 21
byte 4 183
byte 4 12
byte 4 21
byte 4 207
byte 4 12
byte 4 32
byte 4 13
byte 4 55
byte 4 30
byte 4 49
byte 4 55
byte 4 13
byte 4 66
byte 4 55
byte 4 29
byte 4 101
byte 4 55
byte 4 31
byte 4 135
byte 4 55
byte 4 21
byte 4 158
byte 4 55
byte 4 40
byte 4 204
byte 4 55
byte 4 32
byte 4 12
byte 4 97
byte 4 31
byte 4 48
byte 4 97
byte 4 31
byte 4 82
byte 4 97
byte 4 30
byte 4 118
byte 4 97
byte 4 30
byte 4 153
byte 4 97
byte 4 30
byte 4 185
byte 4 97
byte 4 25
byte 4 213
byte 4 97
byte 4 30
byte 4 11
byte 4 139
byte 4 32
byte 4 42
byte 4 139
byte 4 51
byte 4 93
byte 4 139
byte 4 32
byte 4 126
byte 4 139
byte 4 31
byte 4 158
byte 4 139
byte 4 25
code
proc UI_DrawBannerString2 52 36
line 346
;186:
;187:/*
;188:=================
;189:UI_DrawProportionalString2
;190:=================
;191:*/
;192:static int	propMap[128][3] = {
;193:{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
;194:{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
;195:
;196:{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
;197:{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
;198:
;199:{0, 0, PROP_SPACE_WIDTH},		// SPACE
;200:{11, 122, 7},	// !
;201:{154, 181, 14},	// "
;202:{55, 122, 17},	// #
;203:{79, 122, 18},	// $
;204:{101, 122, 23},	// %
;205:{153, 122, 18},	// &
;206:{9, 93, 7},		// '
;207:{207, 122, 8},	// (
;208:{230, 122, 9},	// )
;209:{177, 122, 18},	// *
;210:{30, 152, 18},	// +
;211:{85, 181, 7},	// ,
;212:{34, 93, 11},	// -
;213:{110, 181, 6},	// .
;214:{130, 152, 14},	// /
;215:
;216:{22, 64, 17},	// 0
;217:{41, 64, 12},	// 1
;218:{58, 64, 17},	// 2
;219:{78, 64, 18},	// 3
;220:{98, 64, 19},	// 4
;221:{120, 64, 18},	// 5
;222:{141, 64, 18},	// 6
;223:{204, 64, 16},	// 7
;224:{162, 64, 17},	// 8
;225:{182, 64, 18},	// 9
;226:{59, 181, 7},	// :
;227:{35,181, 7},	// ;
;228:{203, 152, 14},	// <
;229:{56, 93, 14},	// =
;230:{228, 152, 14},	// >
;231:{177, 181, 18},	// ?
;232:
;233:{28, 122, 22},	// @
;234:{5, 4, 18},		// A
;235:{27, 4, 18},	// B
;236:{48, 4, 18},	// C
;237:{69, 4, 17},	// D
;238:{90, 4, 13},	// E
;239:{106, 4, 13},	// F
;240:{121, 4, 18},	// G
;241:{143, 4, 17},	// H
;242:{164, 4, 8},	// I
;243:{175, 4, 16},	// J
;244:{195, 4, 18},	// K
;245:{216, 4, 12},	// L
;246:{230, 4, 23},	// M
;247:{6, 34, 18},	// N
;248:{27, 34, 18},	// O
;249:
;250:{48, 34, 18},	// P
;251:{68, 34, 18},	// Q
;252:{90, 34, 17},	// R
;253:{110, 34, 18},	// S
;254:{130, 34, 14},	// T
;255:{146, 34, 18},	// U
;256:{166, 34, 19},	// V
;257:{185, 34, 29},	// W
;258:{215, 34, 18},	// X
;259:{234, 34, 18},	// Y
;260:{5, 64, 14},	// Z
;261:{60, 152, 7},	// [
;262:{106, 151, 13},	// '\'
;263:{83, 152, 7},	// ]
;264:{128, 122, 17},	// ^
;265:{4, 152, 21},	// _
;266:
;267:{134, 181, 5},	// '
;268:{5, 4, 18},		// A
;269:{27, 4, 18},	// B
;270:{48, 4, 18},	// C
;271:{69, 4, 17},	// D
;272:{90, 4, 13},	// E
;273:{106, 4, 13},	// F
;274:{121, 4, 18},	// G
;275:{143, 4, 17},	// H
;276:{164, 4, 8},	// I
;277:{175, 4, 16},	// J
;278:{195, 4, 18},	// K
;279:{216, 4, 12},	// L
;280:{230, 4, 23},	// M
;281:{6, 34, 18},	// N
;282:{27, 34, 18},	// O
;283:
;284:{48, 34, 18},	// P
;285:{68, 34, 18},	// Q
;286:{90, 34, 17},	// R
;287:{110, 34, 18},	// S
;288:{130, 34, 14},	// T
;289:{146, 34, 18},	// U
;290:{166, 34, 19},	// V
;291:{185, 34, 29},	// W
;292:{215, 34, 18},	// X
;293:{234, 34, 18},	// Y
;294:{5, 64, 14},	// Z
;295:{153, 152, 13},	// {
;296:{11, 181, 5},	// |
;297:{180, 152, 13},	// }
;298:{79, 93, 17},	// ~
;299:{0, 0, -1}		// DEL
;300:};
;301:
;302:static int propMapB[26][3] = {
;303:{11, 12, 33},
;304:{49, 12, 31},
;305:{85, 12, 31},
;306:{120, 12, 30},
;307:{156, 12, 21},
;308:{183, 12, 21},
;309:{207, 12, 32},
;310:
;311:{13, 55, 30},
;312:{49, 55, 13},
;313:{66, 55, 29},
;314:{101, 55, 31},
;315:{135, 55, 21},
;316:{158, 55, 40},
;317:{204, 55, 32},
;318:
;319:{12, 97, 31},
;320:{48, 97, 31},
;321:{82, 97, 30},
;322:{118, 97, 30},
;323:{153, 97, 30},
;324:{185, 97, 25},
;325:{213, 97, 30},
;326:
;327:{11, 139, 32},
;328:{42, 139, 51},
;329:{93, 139, 32},
;330:{126, 139, 31},
;331:{158, 139, 25},
;332:};
;333:
;334:#define PROPB_GAP_WIDTH		4
;335:#define PROPB_SPACE_WIDTH	12
;336:#define PROPB_HEIGHT		36
;337:
;338:// bk001205 - code below duplicated in cgame/cg_drawtools.c
;339:// bk001205 - FIXME: does this belong in ui_shared.c?
;340:/*
;341:=================
;342:UI_DrawBannerString
;343:=================
;344:*/
;345:static void UI_DrawBannerString2( int x, int y, const char* str, vec4_t color )
;346:{
line 359
;347:	const char* s;
;348:	unsigned char	ch; // bk001204 - unsigned
;349:	float	ax;
;350:	float	ay;
;351:	float	aw;
;352:	float	ah;
;353:	float	frow;
;354:	float	fcol;
;355:	float	fwidth;
;356:	float	fheight;
;357:
;358:	// draw the colored text
;359:	trap_R_SetColor( color );
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 361
;360:	
;361:	ax = x * uis.scale + uis.bias;
ADDRLP4 8
ADDRFP4 0
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRGP4 uis+11436
INDIRF4
ADDF4
ASGNF4
line 362
;362:	ay = y * uis.scale;
ADDRLP4 36
ADDRFP4 4
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 364
;363:
;364:	s = str;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRGP4 $139
JUMPV
LABELV $138
line 366
;365:	while ( *s )
;366:	{
line 367
;367:		ch = *s & 127;
ADDRLP4 0
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 368
;368:		if ( ch == ' ' ) {
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 32
NEI4 $141
line 369
;369:			ax += ((float)PROPB_SPACE_WIDTH + (float)PROPB_GAP_WIDTH)* uis.scale;
ADDRLP4 8
ADDRLP4 8
INDIRF4
CNSTF4 1098907648
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDF4
ASGNF4
line 370
;370:		}
ADDRGP4 $142
JUMPV
LABELV $141
line 371
;371:		else if ( ch >= 'A' && ch <= 'Z' ) {
ADDRLP4 40
ADDRLP4 0
INDIRU1
CVUI4 1
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 65
LTI4 $144
ADDRLP4 40
INDIRI4
CNSTI4 90
GTI4 $144
line 372
;372:			ch -= 'A';
ADDRLP4 0
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 65
SUBI4
CVIU4 4
CVUU1 4
ASGNU1
line 373
;373:			fcol = (float)propMapB[ch][0] / 256.0f;
ADDRLP4 20
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMapB
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 374
;374:			frow = (float)propMapB[ch][1] / 256.0f;
ADDRLP4 16
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMapB+4
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 375
;375:			fwidth = (float)propMapB[ch][2] / 256.0f;
ADDRLP4 28
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMapB+8
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 376
;376:			fheight = (float)PROPB_HEIGHT / 256.0f;
ADDRLP4 32
CNSTF4 1041235968
ASGNF4
line 377
;377:			aw = (float)propMapB[ch][2] * uis.scale;
ADDRLP4 12
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMapB+8
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 378
;378:			ah = (float)PROPB_HEIGHT * uis.scale;
ADDRLP4 24
CNSTF4 1108344832
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 379
;379:			trap_R_DrawStretchPic( ax, ay, aw, ah, fcol, frow, fcol+fwidth, frow+fheight, uis.charsetPropB );
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ADDRLP4 28
INDIRF4
ADDF4
ARGF4
ADDRLP4 16
INDIRF4
ADDRLP4 32
INDIRF4
ADDF4
ARGF4
ADDRGP4 uis+11416
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 380
;380:			ax += (aw + (float)PROPB_GAP_WIDTH * uis.scale);
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRLP4 12
INDIRF4
CNSTF4 1082130432
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDF4
ADDF4
ASGNF4
line 381
;381:		}
LABELV $144
LABELV $142
line 382
;382:		s++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 383
;383:	}
LABELV $139
line 365
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $138
line 385
;384:
;385:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 386
;386:}
LABELV $134
endproc UI_DrawBannerString2 52 36
export UI_DrawBannerString
proc UI_DrawBannerString 44 16
line 388
;387:
;388:void UI_DrawBannerString( int x, int y, const char* str, int style, vec4_t color ) {
line 395
;389:	const char *	s;
;390:	int				ch;
;391:	int				width;
;392:	vec4_t			drawcolor;
;393:
;394:	// find the width of the drawn text
;395:	s = str;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
line 396
;396:	width = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $155
JUMPV
LABELV $154
line 397
;397:	while ( *s ) {
line 398
;398:		ch = *s;
ADDRLP4 0
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 399
;399:		if ( ch == ' ' ) {
ADDRLP4 0
INDIRI4
CNSTI4 32
NEI4 $157
line 400
;400:			width += PROPB_SPACE_WIDTH;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 12
ADDI4
ASGNI4
line 401
;401:		}
ADDRGP4 $158
JUMPV
LABELV $157
line 402
;402:		else if ( ch >= 'A' && ch <= 'Z' ) {
ADDRLP4 0
INDIRI4
CNSTI4 65
LTI4 $159
ADDRLP4 0
INDIRI4
CNSTI4 90
GTI4 $159
line 403
;403:			width += propMapB[ch - 'A'][2] + PROPB_GAP_WIDTH;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 propMapB-780+8
ADDP4
INDIRI4
CNSTI4 4
ADDI4
ADDI4
ASGNI4
line 404
;404:		}
LABELV $159
LABELV $158
line 405
;405:		s++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 406
;406:	}
LABELV $155
line 397
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $154
line 407
;407:	width -= PROPB_GAP_WIDTH;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 4
SUBI4
ASGNI4
line 409
;408:
;409:	switch( style & UI_FORMATMASK ) {
ADDRLP4 28
ADDRFP4 12
INDIRI4
CNSTI4 7
BANDI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $164
ADDRLP4 28
INDIRI4
CNSTI4 1
EQI4 $166
ADDRLP4 28
INDIRI4
CNSTI4 2
EQI4 $167
ADDRGP4 $164
JUMPV
LABELV $166
line 411
;410:		case UI_CENTER:
;411:			x -= width / 2;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 8
INDIRI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 412
;412:			break;
ADDRGP4 $164
JUMPV
LABELV $167
line 415
;413:
;414:		case UI_RIGHT:
;415:			x -= width;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ASGNI4
line 416
;416:			break;
line 420
;417:
;418:		case UI_LEFT:
;419:		default:
;420:			break;
LABELV $164
line 423
;421:	}
;422:
;423:	if ( style & UI_DROPSHADOW ) {
ADDRFP4 12
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $169
line 424
;424:		drawcolor[0] = drawcolor[1] = drawcolor[2] = 0;
ADDRLP4 36
CNSTF4 0
ASGNF4
ADDRLP4 12+8
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 12
ADDRLP4 36
INDIRF4
ASGNF4
line 425
;425:		drawcolor[3] = color[3];
ADDRLP4 12+12
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 426
;426:		UI_DrawBannerString2( x+2, y+2, str, drawcolor );
ADDRLP4 40
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
ARGI4
ADDRFP4 4
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 UI_DrawBannerString2
CALLV
pop
line 427
;427:	}
LABELV $169
line 429
;428:
;429:	UI_DrawBannerString2( x, y, str, color );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 UI_DrawBannerString2
CALLV
pop
line 430
;430:}
LABELV $153
endproc UI_DrawBannerString 44 16
export UI_ProportionalStringWidth
proc UI_ProportionalStringWidth 16 0
line 433
;431:
;432:
;433:int UI_ProportionalStringWidth( const char* str ) {
line 439
;434:	const char *	s;
;435:	int				ch;
;436:	int				charWidth;
;437:	int				width;
;438:
;439:	s = str;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 440
;440:	width = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $176
JUMPV
LABELV $175
line 441
;441:	while ( *s ) {
line 442
;442:		ch = *s & 127;
ADDRLP4 8
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
BANDI4
ASGNI4
line 443
;443:		charWidth = propMap[ch][2];
ADDRLP4 4
CNSTI4 12
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 propMap+8
ADDP4
INDIRI4
ASGNI4
line 444
;444:		if ( charWidth != -1 ) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
EQI4 $179
line 445
;445:			width += charWidth;
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 446
;446:			width += PROP_GAP_WIDTH;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 3
ADDI4
ASGNI4
line 447
;447:		}
LABELV $179
line 448
;448:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 449
;449:	}
LABELV $176
line 441
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $175
line 451
;450:
;451:	width -= PROP_GAP_WIDTH;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 3
SUBI4
ASGNI4
line 452
;452:	return width;
ADDRLP4 12
INDIRI4
RETI4
LABELV $174
endproc UI_ProportionalStringWidth 16 0
proc UI_DrawProportionalString2 48 36
line 456
;453:}
;454:
;455:static void UI_DrawProportionalString2( int x, int y, const char* str, vec4_t color, float sizeScale, qhandle_t charset )
;456:{
line 461
;457:	const char* s;
;458:	unsigned char	ch; // bk001204 - unsigned
;459:	float	ax;
;460:	float	ay;
;461:	float	aw = 0; // bk001204 - init
ADDRLP4 8
CNSTF4 0
ASGNF4
line 469
;462:	float	ah;
;463:	float	frow;
;464:	float	fcol;
;465:	float	fwidth;
;466:	float	fheight;
;467:
;468:	// draw the colored text
;469:	trap_R_SetColor( color );
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 471
;470:	
;471:	ax = x * uis.scale + uis.bias;
ADDRLP4 12
ADDRFP4 0
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRGP4 uis+11436
INDIRF4
ADDF4
ASGNF4
line 472
;472:	ay = y * uis.scale;
ADDRLP4 36
ADDRFP4 4
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 474
;473:
;474:	s = str;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRGP4 $186
JUMPV
LABELV $185
line 476
;475:	while ( *s )
;476:	{
line 477
;477:		ch = *s & 127;
ADDRLP4 0
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 127
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 478
;478:		if ( ch == ' ' ) {
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 32
NEI4 $188
line 479
;479:			aw = (float)PROP_SPACE_WIDTH * uis.scale * sizeScale;
ADDRLP4 8
CNSTF4 1090519040
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRFP4 16
INDIRF4
MULF4
ASGNF4
line 480
;480:		}
ADDRGP4 $189
JUMPV
LABELV $188
line 481
;481:		else if ( propMap[ch][2] != -1 ) {
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMap+8
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $191
line 482
;482:			fcol = (float)propMap[ch][0] / 256.0f;
ADDRLP4 20
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMap
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 483
;483:			frow = (float)propMap[ch][1] / 256.0f;
ADDRLP4 16
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMap+4
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 484
;484:			fwidth = (float)propMap[ch][2] / 256.0f;
ADDRLP4 28
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMap+8
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1132462080
DIVF4
ASGNF4
line 485
;485:			fheight = (float)PROP_HEIGHT / 256.0f;
ADDRLP4 32
CNSTF4 1037565952
ASGNF4
line 486
;486:			aw = (float)propMap[ch][2] * uis.scale * sizeScale;
ADDRLP4 8
CNSTI4 12
ADDRLP4 0
INDIRU1
CVUI4 1
MULI4
ADDRGP4 propMap+8
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRFP4 16
INDIRF4
MULF4
ASGNF4
line 487
;487:			ah = (float)PROP_HEIGHT * uis.scale * sizeScale;
ADDRLP4 24
CNSTF4 1104674816
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRFP4 16
INDIRF4
MULF4
ASGNF4
line 488
;488:			trap_R_DrawStretchPic( ax, ay, aw, ah, fcol, frow, fcol+fwidth, frow+fheight, charset );
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ADDRLP4 28
INDIRF4
ADDF4
ARGF4
ADDRLP4 16
INDIRF4
ADDRLP4 32
INDIRF4
ADDF4
ARGF4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 489
;489:		}
LABELV $191
LABELV $189
line 491
;490:
;491:		ax += (aw + (float)PROP_GAP_WIDTH * uis.scale * sizeScale);
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
CNSTF4 1077936128
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRFP4 16
INDIRF4
MULF4
ADDF4
ADDF4
ASGNF4
line 492
;492:		s++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 493
;493:	}
LABELV $186
line 475
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $185
line 495
;494:
;495:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 496
;496:}
LABELV $181
endproc UI_DrawProportionalString2 48 36
export UI_ProportionalSizeScale
proc UI_ProportionalSizeScale 0 0
line 503
;497:
;498:/*
;499:=================
;500:UI_ProportionalSizeScale
;501:=================
;502:*/
;503:float UI_ProportionalSizeScale( int style ) {
line 504
;504:	if(  style & UI_SMALLFONT ) {
ADDRFP4 0
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $201
line 505
;505:		return PROP_SMALL_SIZE_SCALE;
CNSTF4 1061158912
RETF4
ADDRGP4 $200
JUMPV
LABELV $201
line 508
;506:	}
;507:
;508:	return 1.00;
CNSTF4 1065353216
RETF4
LABELV $200
endproc UI_ProportionalSizeScale 0 0
export UI_DrawProportionalString
proc UI_DrawProportionalString 44 24
line 517
;509:}
;510:
;511:
;512:/*
;513:=================
;514:UI_DrawProportionalString
;515:=================
;516:*/
;517:void UI_DrawProportionalString( int x, int y, const char* str, int style, vec4_t color ) {
line 522
;518:	vec4_t	drawcolor;
;519:	int		width;
;520:	float	sizeScale;
;521:
;522:	sizeScale = UI_ProportionalSizeScale( style );
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_ProportionalSizeScale
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 24
INDIRF4
ASGNF4
line 524
;523:
;524:	switch( style & UI_FORMATMASK ) {
ADDRLP4 28
ADDRFP4 12
INDIRI4
CNSTI4 7
BANDI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $205
ADDRLP4 28
INDIRI4
CNSTI4 1
EQI4 $207
ADDRLP4 28
INDIRI4
CNSTI4 2
EQI4 $208
ADDRGP4 $205
JUMPV
LABELV $207
line 526
;525:		case UI_CENTER:
;526:			width = UI_ProportionalStringWidth( str ) * sizeScale;
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 20
ADDRLP4 36
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 527
;527:			x -= width / 2;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 20
INDIRI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 528
;528:			break;
ADDRGP4 $205
JUMPV
LABELV $208
line 531
;529:
;530:		case UI_RIGHT:
;531:			width = UI_ProportionalStringWidth( str ) * sizeScale;
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 20
ADDRLP4 40
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 532
;532:			x -= width;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 20
INDIRI4
SUBI4
ASGNI4
line 533
;533:			break;
line 537
;534:
;535:		case UI_LEFT:
;536:		default:
;537:			break;
LABELV $205
line 540
;538:	}
;539:
;540:	if ( style & UI_DROPSHADOW ) {
ADDRFP4 12
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $210
line 541
;541:		drawcolor[0] = drawcolor[1] = drawcolor[2] = 0;
ADDRLP4 36
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 36
INDIRF4
ASGNF4
line 542
;542:		drawcolor[3] = color[3];
ADDRLP4 0+12
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 543
;543:		UI_DrawProportionalString2( x+2, y+2, str, drawcolor, sizeScale, uis.charsetProp );
ADDRLP4 40
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
ARGI4
ADDRFP4 4
INDIRI4
ADDRLP4 40
INDIRI4
ADDI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uis+11408
INDIRI4
ARGI4
ADDRGP4 UI_DrawProportionalString2
CALLV
pop
line 544
;544:	}
LABELV $210
line 546
;545:
;546:	if ( style & UI_INVERSE ) {
ADDRFP4 12
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $216
line 547
;547:		drawcolor[0] = color[0] * 0.7;
ADDRLP4 0
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
INDIRF4
MULF4
ASGNF4
line 548
;548:		drawcolor[1] = color[1] * 0.7;
ADDRLP4 0+4
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
line 549
;549:		drawcolor[2] = color[2] * 0.7;
ADDRLP4 0+8
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 550
;550:		drawcolor[3] = color[3];
ADDRLP4 0+12
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 551
;551:		UI_DrawProportionalString2( x, y, str, drawcolor, sizeScale, uis.charsetProp );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uis+11408
INDIRI4
ARGI4
ADDRGP4 UI_DrawProportionalString2
CALLV
pop
line 552
;552:		return;
ADDRGP4 $203
JUMPV
LABELV $216
line 555
;553:	}
;554:
;555:	if ( style & UI_PULSE ) {
ADDRFP4 12
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $222
line 556
;556:		drawcolor[0] = color[0] * 0.7;
ADDRLP4 0
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
INDIRF4
MULF4
ASGNF4
line 557
;557:		drawcolor[1] = color[1] * 0.7;
ADDRLP4 0+4
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
line 558
;558:		drawcolor[2] = color[2] * 0.7;
ADDRLP4 0+8
CNSTF4 1060320051
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 559
;559:		drawcolor[3] = color[3];
ADDRLP4 0+12
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 560
;560:		UI_DrawProportionalString2( x, y, str, color, sizeScale, uis.charsetProp );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uis+11408
INDIRI4
ARGI4
ADDRGP4 UI_DrawProportionalString2
CALLV
pop
line 562
;561:
;562:		drawcolor[0] = color[0];
ADDRLP4 0
ADDRFP4 16
INDIRP4
INDIRF4
ASGNF4
line 563
;563:		drawcolor[1] = color[1];
ADDRLP4 0+4
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 564
;564:		drawcolor[2] = color[2];
ADDRLP4 0+8
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ASGNF4
line 565
;565:		drawcolor[3] = 0.5 + 0.5 * sin( uis.realtime / PULSE_DIVISOR );
ADDRGP4 uis+4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 36
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 0+12
CNSTF4 1056964608
ADDRLP4 36
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ASGNF4
line 566
;566:		UI_DrawProportionalString2( x, y, str, drawcolor, sizeScale, uis.charsetPropGlow );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uis+11412
INDIRI4
ARGI4
ADDRGP4 UI_DrawProportionalString2
CALLV
pop
line 567
;567:		return;
ADDRGP4 $203
JUMPV
LABELV $222
line 570
;568:	}
;569:
;570:	UI_DrawProportionalString2( x, y, str, color, sizeScale, uis.charsetProp );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRGP4 uis+11408
INDIRI4
ARGI4
ADDRGP4 UI_DrawProportionalString2
CALLV
pop
line 571
;571:}
LABELV $203
endproc UI_DrawProportionalString 44 24
export UI_DrawProportionalString_AutoWrapped
proc UI_DrawProportionalString_AutoWrapped 1064 20
line 578
;572:
;573:/*
;574:=================
;575:UI_DrawProportionalString_Wrapped
;576:=================
;577:*/
;578:void UI_DrawProportionalString_AutoWrapped( int x, int y, int xmax, int ystep, const char* str, int style, vec4_t color ) {
line 585
;579:	int width;
;580:	char *s1,*s2,*s3;
;581:	char c_bcp;
;582:	char buf[1024];
;583:	float   sizeScale;
;584:
;585:	if (!str || str[0]=='\0')
ADDRLP4 1048
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 1048
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $237
ADDRLP4 1048
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $235
LABELV $237
line 586
;586:		return;
ADDRGP4 $234
JUMPV
LABELV $235
line 588
;587:	
;588:	sizeScale = UI_ProportionalSizeScale( style );
ADDRFP4 20
INDIRI4
ARGI4
ADDRLP4 1052
ADDRGP4 UI_ProportionalSizeScale
CALLF4
ASGNF4
ADDRLP4 20
ADDRLP4 1052
INDIRF4
ASGNF4
line 590
;589:	
;590:	Q_strncpyz(buf, str, sizeof(buf));
ADDRLP4 24
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 591
;591:	s1 = s2 = s3 = buf;
ADDRLP4 0
ADDRLP4 24
ASGNP4
ADDRLP4 4
ADDRLP4 24
ASGNP4
ADDRLP4 12
ADDRLP4 24
ASGNP4
ADDRGP4 $239
JUMPV
LABELV $238
line 593
;592:
;593:	while (1) {
LABELV $241
line 594
;594:		do {
line 595
;595:			s3++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 596
;596:		} while (*s3!=' ' && *s3!='\0');
LABELV $242
ADDRLP4 1056
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 32
EQI4 $244
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $241
LABELV $244
line 597
;597:		c_bcp = *s3;
ADDRLP4 8
ADDRLP4 0
INDIRP4
INDIRI1
ASGNI1
line 598
;598:		*s3 = '\0';
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 599
;599:		width = UI_ProportionalStringWidth(s1) * sizeScale;
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 1060
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 1060
INDIRI4
CVIF4 4
ADDRLP4 20
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 600
;600:		*s3 = c_bcp;
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI1
ASGNI1
line 601
;601:		if (width > xmax) {
ADDRLP4 16
INDIRI4
ADDRFP4 8
INDIRI4
LEI4 $245
line 602
;602:			if (s1==s2)
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
NEU4 $247
line 603
;603:			{
line 605
;604:				// fuck, don't have a clean cut, we'll overflow
;605:				s2 = s3;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 606
;606:			}
LABELV $247
line 607
;607:			*s2 = '\0';
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 608
;608:			UI_DrawProportionalString(x, y, s1, style, color);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 609
;609:			y += ystep;
ADDRFP4 4
ADDRFP4 4
INDIRI4
ADDRFP4 12
INDIRI4
ADDI4
ASGNI4
line 610
;610:			if (c_bcp == '\0')
ADDRLP4 8
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $249
line 611
;611:      {
line 616
;612:        // that was the last word
;613:        // we could start a new loop, but that wouldn't be much use
;614:        // even if the word is too long, we would overflow it (see above)
;615:        // so just print it now if needed
;616:        s2++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 617
;617:        if (*s2 != '\0') // if we are printing an overflowing line we have s2 == s3
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $240
line 618
;618:          UI_DrawProportionalString(x, y, s2, style, color);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 619
;619:				break; 
ADDRGP4 $240
JUMPV
LABELV $249
line 621
;620:      }
;621:			s2++;
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 622
;622:			s1 = s2;
ADDRLP4 12
ADDRLP4 4
INDIRP4
ASGNP4
line 623
;623:			s3 = s2;
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 624
;624:		}
ADDRGP4 $246
JUMPV
LABELV $245
line 626
;625:		else
;626:		{
line 627
;627:			s2 = s3;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 628
;628:			if (c_bcp == '\0') // we reached the end
ADDRLP4 8
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $253
line 629
;629:			{
line 630
;630:				UI_DrawProportionalString(x, y, s1, style, color);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 631
;631:				break;
ADDRGP4 $240
JUMPV
LABELV $253
line 633
;632:			}
;633:		}
LABELV $246
line 634
;634:	}
LABELV $239
line 593
ADDRGP4 $238
JUMPV
LABELV $240
line 635
;635:}
LABELV $234
endproc UI_DrawProportionalString_AutoWrapped 1064 20
proc UI_DrawString2 76 36
line 643
;636:
;637:/*
;638:=================
;639:UI_DrawString2
;640:=================
;641:*/
;642:static void UI_DrawString2( int x, int y, const char* str, vec4_t color, int charw, int charh )
;643:{
line 646
;644:	const char* s;
;645:	char	ch;
;646:	int forceColor = qfalse; //APSFIXME;
ADDRLP4 40
CNSTI4 0
ASGNI4
line 655
;647:	vec4_t	tempcolor;
;648:	float	ax;
;649:	float	ay;
;650:	float	aw;
;651:	float	ah;
;652:	float	frow;
;653:	float	fcol;
;654:
;655:	if (y < -charh)
ADDRFP4 4
INDIRI4
ADDRFP4 20
INDIRI4
NEGI4
GEI4 $256
line 657
;656:		// offscreen
;657:		return;
ADDRGP4 $255
JUMPV
LABELV $256
line 660
;658:
;659:	// draw the colored text
;660:	trap_R_SetColor( color );
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 662
;661:	
;662:	ax = x * uis.scale + uis.bias;
ADDRLP4 8
ADDRFP4 0
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRGP4 uis+11436
INDIRF4
ADDF4
ASGNF4
line 663
;663:	ay = y * uis.scale;
ADDRLP4 44
ADDRFP4 4
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 664
;664:	aw = charw * uis.scale;
ADDRLP4 12
ADDRFP4 16
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 665
;665:	ah = charh * uis.scale;
ADDRLP4 48
ADDRFP4 20
INDIRI4
CVIF4 4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 667
;666:
;667:	s = str;
ADDRLP4 0
ADDRFP4 8
INDIRP4
ASGNP4
ADDRGP4 $264
JUMPV
LABELV $263
line 669
;668:	while ( *s )
;669:	{
line 670
;670:		if ( Q_IsColorString( s ) )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $266
ADDRLP4 56
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 56
INDIRI4
NEI4 $266
ADDRLP4 60
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $266
ADDRLP4 60
INDIRI4
ADDRLP4 56
INDIRI4
EQI4 $266
line 671
;671:		{
line 672
;672:			if ( !forceColor )
ADDRLP4 40
INDIRI4
CNSTI4 0
NEI4 $268
line 673
;673:			{
line 674
;674:				memcpy( tempcolor, g_color_table[ColorIndex(s[1])], sizeof( tempcolor ) );
ADDRLP4 24
ARGP4
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
line 675
;675:				tempcolor[3] = color[3];
ADDRLP4 24+12
ADDRFP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 676
;676:				trap_R_SetColor( tempcolor );
ADDRLP4 24
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 677
;677:			}
LABELV $268
line 678
;678:			s += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 679
;679:			continue;
ADDRGP4 $264
JUMPV
LABELV $266
line 682
;680:		}
;681:
;682:		ch = *s & 255;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 255
BANDI4
CVII1 4
ASGNI1
line 683
;683:		if (ch != ' ')
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 32
EQI4 $271
line 684
;684:		{
line 685
;685:			frow = (ch>>4)*0.0625;
ADDRLP4 16
CNSTF4 1031798784
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 4
RSHI4
CVIF4 4
MULF4
ASGNF4
line 686
;686:			fcol = (ch&15)*0.0625;
ADDRLP4 20
CNSTF4 1031798784
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 15
BANDI4
CVIF4 4
MULF4
ASGNF4
line 687
;687:			trap_R_DrawStretchPic( ax, ay, aw, ah, fcol, frow, fcol + 0.0625, frow + 0.0625, uis.charset );
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 48
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 72
CNSTF4 1031798784
ASGNF4
ADDRLP4 20
INDIRF4
ADDRLP4 72
INDIRF4
ADDF4
ARGF4
ADDRLP4 16
INDIRF4
ADDRLP4 72
INDIRF4
ADDF4
ARGF4
ADDRGP4 uis+11404
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 688
;688:		}
LABELV $271
line 690
;689:
;690:		ax += aw;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRLP4 12
INDIRF4
ADDF4
ASGNF4
line 691
;691:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 692
;692:	}
LABELV $264
line 668
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $263
line 694
;693:
;694:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 695
;695:}
LABELV $255
endproc UI_DrawString2 76 36
export UI_DrawString
proc UI_DrawString 84 24
line 703
;696:
;697:/*
;698:=================
;699:UI_DrawString
;700:=================
;701:*/
;702:void UI_DrawString( int x, int y, const char* str, int style, vec4_t color )
;703:{
line 712
;704:	int		len;
;705:	int		charw;
;706:	int		charh;
;707:	vec4_t	newcolor;
;708:	vec4_t	lowlight;
;709:	float	*drawcolor;
;710:	vec4_t	dropcolor;
;711:
;712:	if( !str ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $275
line 713
;713:		return;
ADDRGP4 $274
JUMPV
LABELV $275
line 716
;714:	}
;715:
;716:	if ((style & UI_BLINK) && ((uis.realtime/BLINK_DIVISOR) & 1))
ADDRLP4 64
CNSTI4 0
ASGNI4
ADDRFP4 12
INDIRI4
CNSTI4 4096
BANDI4
ADDRLP4 64
INDIRI4
EQI4 $277
ADDRGP4 uis+4
INDIRI4
CNSTI4 200
DIVI4
CNSTI4 1
BANDI4
ADDRLP4 64
INDIRI4
EQI4 $277
line 717
;717:		return;
ADDRGP4 $274
JUMPV
LABELV $277
line 719
;718:
;719:	if (style & UI_SMALLFONT)
ADDRFP4 12
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $280
line 720
;720:	{
line 721
;721:		charw =	SMALLCHAR_WIDTH;
ADDRLP4 0
CNSTI4 8
ASGNI4
line 722
;722:		charh =	SMALLCHAR_HEIGHT;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 723
;723:	}
ADDRGP4 $281
JUMPV
LABELV $280
line 724
;724:	else if (style & UI_GIANTFONT)
ADDRFP4 12
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
EQI4 $282
line 725
;725:	{
line 726
;726:		charw =	GIANTCHAR_WIDTH;
ADDRLP4 0
CNSTI4 32
ASGNI4
line 727
;727:		charh =	GIANTCHAR_HEIGHT;
ADDRLP4 4
CNSTI4 48
ASGNI4
line 728
;728:	}
ADDRGP4 $283
JUMPV
LABELV $282
line 730
;729:	else
;730:	{
line 731
;731:		charw =	BIGCHAR_WIDTH;
ADDRLP4 0
CNSTI4 16
ASGNI4
line 732
;732:		charh =	BIGCHAR_HEIGHT;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 733
;733:	}
LABELV $283
LABELV $281
line 735
;734:
;735:	if (style & UI_PULSE)
ADDRFP4 12
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $284
line 736
;736:	{
line 737
;737:		lowlight[0] = 0.8*color[0]; 
ADDRLP4 8
CNSTF4 1061997773
ADDRFP4 16
INDIRP4
INDIRF4
MULF4
ASGNF4
line 738
;738:		lowlight[1] = 0.8*color[1];
ADDRLP4 8+4
CNSTF4 1061997773
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
line 739
;739:		lowlight[2] = 0.8*color[2];
ADDRLP4 8+8
CNSTF4 1061997773
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 740
;740:		lowlight[3] = 0.8*color[3];
ADDRLP4 8+12
CNSTF4 1061997773
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
MULF4
ASGNF4
line 741
;741:		UI_LerpColor(color,lowlight,newcolor,0.5+0.5*sin(uis.realtime/PULSE_DIVISOR));
ADDRGP4 uis+4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 68
ADDRGP4 sin
CALLF4
ASGNF4
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 44
ARGP4
CNSTF4 1056964608
ADDRLP4 68
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 UI_LerpColor
CALLV
pop
line 742
;742:		drawcolor = newcolor;
ADDRLP4 24
ADDRLP4 44
ASGNP4
line 743
;743:	}	
ADDRGP4 $285
JUMPV
LABELV $284
line 745
;744:	else
;745:		drawcolor = color;
ADDRLP4 24
ADDRFP4 16
INDIRP4
ASGNP4
LABELV $285
line 747
;746:
;747:	switch (style & UI_FORMATMASK)
ADDRLP4 68
ADDRFP4 12
INDIRI4
CNSTI4 7
BANDI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 1
EQI4 $293
ADDRLP4 68
INDIRI4
CNSTI4 2
EQI4 $294
ADDRGP4 $291
JUMPV
line 748
;748:	{
LABELV $293
line 751
;749:		case UI_CENTER:
;750:			// center justify at x
;751:			len = strlen(str);
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 76
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 60
ADDRLP4 76
INDIRI4
ASGNI4
line 752
;752:			x   = x - len*charw/2;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 60
INDIRI4
ADDRLP4 0
INDIRI4
MULI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 753
;753:			break;
ADDRGP4 $291
JUMPV
LABELV $294
line 757
;754:
;755:		case UI_RIGHT:
;756:			// right justify at x
;757:			len = strlen(str);
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 60
ADDRLP4 80
INDIRI4
ASGNI4
line 758
;758:			x   = x - len*charw;
ADDRFP4 0
ADDRFP4 0
INDIRI4
ADDRLP4 60
INDIRI4
ADDRLP4 0
INDIRI4
MULI4
SUBI4
ASGNI4
line 759
;759:			break;
line 763
;760:
;761:		default:
;762:			// left justify at x
;763:			break;
LABELV $291
line 766
;764:	}
;765:
;766:	if ( style & UI_DROPSHADOW )
ADDRFP4 12
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $295
line 767
;767:	{
line 768
;768:		dropcolor[0] = dropcolor[1] = dropcolor[2] = 0;
ADDRLP4 76
CNSTF4 0
ASGNF4
ADDRLP4 28+8
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 28
ADDRLP4 76
INDIRF4
ASGNF4
line 769
;769:		dropcolor[3] = drawcolor[3];
ADDRLP4 28+12
ADDRLP4 24
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ASGNF4
line 770
;770:		UI_DrawString2(x+2,y+2,str,dropcolor,charw,charh);
ADDRLP4 80
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 80
INDIRI4
ADDI4
ARGI4
ADDRFP4 4
INDIRI4
ADDRLP4 80
INDIRI4
ADDI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 UI_DrawString2
CALLV
pop
line 771
;771:	}
LABELV $295
line 773
;772:
;773:	UI_DrawString2(x,y,str,drawcolor,charw,charh);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 UI_DrawString2
CALLV
pop
line 774
;774:}
LABELV $274
endproc UI_DrawString 84 24
export UI_DrawChar
proc UI_DrawChar 2 20
line 782
;775:
;776:/*
;777:=================
;778:UI_DrawChar
;779:=================
;780:*/
;781:void UI_DrawChar( int x, int y, int ch, int style, vec4_t color )
;782:{
line 785
;783:	char	buff[2];
;784:
;785:	buff[0] = ch;
ADDRLP4 0
ADDRFP4 8
INDIRI4
CVII1 4
ASGNI1
line 786
;786:	buff[1] = '\0';
ADDRLP4 0+1
CNSTI1 0
ASGNI1
line 788
;787:
;788:	UI_DrawString( x, y, buff, style, color );
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 789
;789:}
LABELV $300
endproc UI_DrawChar 2 20
export UI_IsFullscreen
proc UI_IsFullscreen 4 0
line 791
;790:
;791:qboolean UI_IsFullscreen( void ) {
line 792
;792:	if ( uis.activemenu && ( trap_Key_GetCatcher() & KEYCATCH_UI ) ) {
ADDRGP4 uis+20
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $303
ADDRLP4 0
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $303
line 793
;793:		return uis.activemenu->fullscreen;
ADDRGP4 uis+20
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
RETI4
ADDRGP4 $302
JUMPV
LABELV $303
line 796
;794:	}
;795:
;796:	return qfalse;
CNSTI4 0
RETI4
LABELV $302
endproc UI_IsFullscreen 4 0
proc NeedCDAction 0 8
line 799
;797:}
;798:
;799:static void NeedCDAction( qboolean result ) {
line 800
;800:	if ( !result ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $308
line 801
;801:		trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
CNSTI4 2
ARGI4
ADDRGP4 $310
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 802
;802:	}
LABELV $308
line 803
;803:}
LABELV $307
endproc NeedCDAction 0 8
proc NeedCDKeyAction 0 8
line 805
;804:
;805:static void NeedCDKeyAction( qboolean result ) {
line 806
;806:	if ( !result ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $312
line 807
;807:		trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
CNSTI4 2
ARGI4
ADDRGP4 $310
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 808
;808:	}
LABELV $312
line 809
;809:}
LABELV $311
endproc NeedCDKeyAction 0 8
export UI_SetActiveMenu
proc UI_SetActiveMenu 8 12
line 811
;810:
;811:void UI_SetActiveMenu( uiMenuCommand_t menu ) {
line 814
;812:	// this should be the ONLY way the menu system is brought up
;813:	// enusure minumum menu data is cached
;814:	Menu_Cache();
ADDRGP4 Menu_Cache
CALLV
pop
line 816
;815:
;816:	switch ( menu ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $315
ADDRLP4 0
INDIRI4
CNSTI4 6
GTI4 $315
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $328
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $328
address $318
address $319
address $324
address $320
address $322
address $326
address $326
code
LABELV $318
line 818
;817:	case UIMENU_NONE:
;818:		UI_ForceMenuOff();
ADDRGP4 UI_ForceMenuOff
CALLV
pop
line 819
;819:		return;
ADDRGP4 $314
JUMPV
LABELV $319
line 821
;820:	case UIMENU_MAIN:
;821:		UI_MainMenu();
ADDRGP4 UI_MainMenu
CALLV
pop
line 822
;822:		return;
ADDRGP4 $314
JUMPV
LABELV $320
line 824
;823:	case UIMENU_NEED_CD:
;824:		UI_ConfirmMenu( "Insert the CD", (voidfunc_f)NULL, NeedCDAction );
ADDRGP4 $321
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 NeedCDAction
ARGP4
ADDRGP4 UI_ConfirmMenu
CALLV
pop
line 825
;825:		return;
ADDRGP4 $314
JUMPV
LABELV $322
line 827
;826:	case UIMENU_BAD_CD_KEY:
;827:		UI_ConfirmMenu( "Bad CD Key", (voidfunc_f)NULL, NeedCDKeyAction );
ADDRGP4 $323
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 NeedCDKeyAction
ARGP4
ADDRGP4 UI_ConfirmMenu
CALLV
pop
line 828
;828:		return;
ADDRGP4 $314
JUMPV
LABELV $324
line 835
;829:	case UIMENU_INGAME:
;830:		/*
;831:		//GRank
;832:		UI_RankingsMenu();
;833:		return;
;834:		*/
;835:		trap_Cvar_Set( "cl_paused", "1" );
ADDRGP4 $123
ARGP4
ADDRGP4 $325
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 836
;836:		UI_InGameMenu();
ADDRGP4 UI_InGameMenu
CALLV
pop
line 837
;837:		return;
ADDRGP4 $314
JUMPV
LABELV $326
LABELV $315
line 844
;838:		
;839:	// bk001204
;840:	case UIMENU_TEAM:
;841:	case UIMENU_POSTGAME:
;842:	default:
;843:#ifndef NDEBUG
;844:	  Com_Printf("UI_SetActiveMenu: bad enum %d\n", menu );
ADDRGP4 $327
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 846
;845:#endif
;846:	  break;
LABELV $316
line 848
;847:	}
;848:}
LABELV $314
endproc UI_SetActiveMenu 8 12
export UI_KeyEvent
proc UI_KeyEvent 16 8
line 855
;849:
;850:/*
;851:=================
;852:UI_KeyEvent
;853:=================
;854:*/
;855:void UI_KeyEvent( int key, int down ) {
line 858
;856:	sfxHandle_t		s;
;857:
;858:	if (!uis.activemenu) {
ADDRGP4 uis+20
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $330
line 859
;859:		return;
ADDRGP4 $329
JUMPV
LABELV $330
line 862
;860:	}
;861:
;862:	if (!down) {
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $333
line 863
;863:		return;
ADDRGP4 $329
JUMPV
LABELV $333
line 866
;864:	}
;865:
;866:	if (uis.activemenu->key)
ADDRGP4 uis+20
INDIRP4
CNSTI4 272
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $335
line 867
;867:		s = uis.activemenu->key( key );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 uis+20
INDIRP4
CNSTI4 272
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $336
JUMPV
LABELV $335
line 869
;868:	else
;869:		s = Menu_DefaultKey( uis.activemenu, key );
ADDRGP4 uis+20
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $336
line 871
;870:
;871:	if ((s > 0) && (s != menu_null_sound))
ADDRLP4 0
INDIRI4
CNSTI4 0
LEI4 $340
ADDRLP4 0
INDIRI4
ADDRGP4 menu_null_sound
INDIRI4
EQI4 $340
line 872
;872:		trap_S_StartLocalSound( s, CHAN_LOCAL_SOUND );
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
LABELV $340
line 873
;873:}
LABELV $329
endproc UI_KeyEvent 16 8
export UI_MouseEvent
proc UI_MouseEvent 24 8
line 881
;874:
;875:/*
;876:=================
;877:UI_MouseEvent
;878:=================
;879:*/
;880:void UI_MouseEvent( int dx, int dy )
;881:{
line 885
;882:	int				i;
;883:	menucommon_s*	m;
;884:
;885:	if (!uis.activemenu)
ADDRGP4 uis+20
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $343
line 886
;886:		return;
ADDRGP4 $342
JUMPV
LABELV $343
line 889
;887:
;888:	// update mouse screen position
;889:	uis.cursorx += dx;
ADDRLP4 8
ADDRGP4 uis+8
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 890
;890:	if (uis.cursorx < 0)
ADDRGP4 uis+8
INDIRI4
CNSTI4 0
GEI4 $347
line 891
;891:		uis.cursorx = 0;
ADDRGP4 uis+8
CNSTI4 0
ASGNI4
ADDRGP4 $348
JUMPV
LABELV $347
line 892
;892:	else if (uis.cursorx > SCREEN_WIDTH)
ADDRGP4 uis+8
INDIRI4
CNSTI4 640
LEI4 $351
line 893
;893:		uis.cursorx = SCREEN_WIDTH;
ADDRGP4 uis+8
CNSTI4 640
ASGNI4
LABELV $351
LABELV $348
line 895
;894:
;895:	uis.cursory += dy;
ADDRLP4 12
ADDRGP4 uis+12
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
line 896
;896:	if (uis.cursory < 0)
ADDRGP4 uis+12
INDIRI4
CNSTI4 0
GEI4 $356
line 897
;897:		uis.cursory = 0;
ADDRGP4 uis+12
CNSTI4 0
ASGNI4
ADDRGP4 $357
JUMPV
LABELV $356
line 898
;898:	else if (uis.cursory > SCREEN_HEIGHT)
ADDRGP4 uis+12
INDIRI4
CNSTI4 480
LEI4 $360
line 899
;899:		uis.cursory = SCREEN_HEIGHT;
ADDRGP4 uis+12
CNSTI4 480
ASGNI4
LABELV $360
LABELV $357
line 902
;900:
;901:	// region test the active menu items
;902:	for (i=0; i<uis.activemenu->nitems; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $367
JUMPV
LABELV $364
line 903
;903:	{
line 904
;904:		m = (menucommon_s*)uis.activemenu->items[i];
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
ASGNP4
line 906
;905:
;906:		if (m->flags & (QMF_GRAYED|QMF_INACTIVE))
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 24576
BANDU4
CNSTU4 0
EQU4 $370
line 907
;907:			continue;
ADDRGP4 $365
JUMPV
LABELV $370
line 909
;908:
;909:		if ((uis.cursorx < m->left) ||
ADDRGP4 uis+8
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
LTI4 $380
ADDRGP4 uis+8
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
GTI4 $380
ADDRGP4 uis+12
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
LTI4 $380
ADDRGP4 uis+12
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
LEI4 $372
LABELV $380
line 913
;910:			(uis.cursorx > m->right) ||
;911:			(uis.cursory < m->top) ||
;912:			(uis.cursory > m->bottom))
;913:		{
line 915
;914:			// cursor out of item bounds
;915:			continue;
ADDRGP4 $365
JUMPV
LABELV $372
line 919
;916:		}
;917:
;918:		// set focus to item at cursor
;919:		if (uis.activemenu->cursor != i)
ADDRGP4 uis+20
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $381
line 920
;920:		{
line 921
;921:			Menu_SetCursor( uis.activemenu, i );
ADDRGP4 uis+20
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 Menu_SetCursor
CALLV
pop
line 922
;922:			((menucommon_s*)(uis.activemenu->items[uis.activemenu->cursor_prev]))->flags &= ~QMF_HASMOUSEFOCUS;
ADDRLP4 20
ADDRGP4 uis+20
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 4294966783
BANDU4
ASGNU4
line 924
;923:
;924:			if ( !(((menucommon_s*)(uis.activemenu->items[uis.activemenu->cursor]))->flags & QMF_SILENT ) ) {
ADDRGP4 uis+20
INDIRP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 1048576
BANDU4
CNSTU4 0
NEU4 $387
line 925
;925:				trap_S_StartLocalSound( menu_move_sound, CHAN_LOCAL_SOUND );
ADDRGP4 menu_move_sound
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
line 926
;926:			}
LABELV $387
line 927
;927:		}
LABELV $381
line 929
;928:
;929:		((menucommon_s*)(uis.activemenu->items[uis.activemenu->cursor]))->flags |= QMF_HASMOUSEFOCUS;
ADDRLP4 20
ADDRGP4 uis+20
INDIRP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 512
BORU4
ASGNU4
line 930
;930:		return;
ADDRGP4 $342
JUMPV
LABELV $365
line 902
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $367
ADDRLP4 4
INDIRI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
LTI4 $364
line 933
;931:	}  
;932:
;933:	if (uis.activemenu->nitems > 0) {
ADDRGP4 uis+20
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 0
LEI4 $393
line 935
;934:		// out of any region
;935:		((menucommon_s*)(uis.activemenu->items[uis.activemenu->cursor]))->flags &= ~QMF_HASMOUSEFOCUS;
ADDRLP4 16
ADDRGP4 uis+20
INDIRP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uis+20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 4294966783
BANDU4
ASGNU4
line 936
;936:	}
LABELV $393
line 937
;937:}
LABELV $342
endproc UI_MouseEvent 24 8
bss
align 1
LABELV $399
skip 1024
export UI_Argv
code
proc UI_Argv 0 12
line 939
;938:
;939:char *UI_Argv( int arg ) {
line 942
;940:	static char	buffer[MAX_STRING_CHARS];
;941:
;942:	trap_Argv( arg, buffer, sizeof( buffer ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $399
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 944
;943:
;944:	return buffer;
ADDRGP4 $399
RETP4
LABELV $398
endproc UI_Argv 0 12
bss
align 1
LABELV $401
skip 1024
export UI_Cvar_VariableString
code
proc UI_Cvar_VariableString 0 12
line 948
;945:}
;946:
;947:
;948:char *UI_Cvar_VariableString( const char *var_name ) {
line 951
;949:	static char	buffer[MAX_STRING_CHARS];
;950:
;951:	trap_Cvar_VariableStringBuffer( var_name, buffer, sizeof( buffer ) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $401
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 953
;952:
;953:	return buffer;
ADDRGP4 $401
RETP4
LABELV $400
endproc UI_Cvar_VariableString 0 12
export UI_Cache_f
proc UI_Cache_f 0 0
line 962
;954:}
;955:
;956:
;957:/*
;958:=================
;959:UI_Cache
;960:=================
;961:*/
;962:void UI_Cache_f( void ) {
line 963
;963:	MainMenu_Cache();
ADDRGP4 MainMenu_Cache
CALLV
pop
line 964
;964:	InGame_Cache();
ADDRGP4 InGame_Cache
CALLV
pop
line 965
;965:	ConfirmMenu_Cache();
ADDRGP4 ConfirmMenu_Cache
CALLV
pop
line 966
;966:	PlayerModel_Cache();
ADDRGP4 PlayerModel_Cache
CALLV
pop
line 967
;967:	PlayerSettings_Cache();
ADDRGP4 PlayerSettings_Cache
CALLV
pop
line 968
;968:	Controls_Cache();
ADDRGP4 Controls_Cache
CALLV
pop
line 969
;969:	Demos_Cache();
ADDRGP4 Demos_Cache
CALLV
pop
line 970
;970:	UI_CinematicsMenu_Cache();
ADDRGP4 UI_CinematicsMenu_Cache
CALLV
pop
line 971
;971:	Preferences_Cache();
ADDRGP4 Preferences_Cache
CALLV
pop
line 972
;972:	ServerInfo_Cache();
ADDRGP4 ServerInfo_Cache
CALLV
pop
line 973
;973:	SpecifyServer_Cache();
ADDRGP4 SpecifyServer_Cache
CALLV
pop
line 974
;974:	ArenaServers_Cache();
ADDRGP4 ArenaServers_Cache
CALLV
pop
line 975
;975:	StartServer_Cache();
ADDRGP4 StartServer_Cache
CALLV
pop
line 976
;976:	ServerOptions_Cache();
ADDRGP4 ServerOptions_Cache
CALLV
pop
line 977
;977:	DriverInfo_Cache();
ADDRGP4 DriverInfo_Cache
CALLV
pop
line 978
;978:	GraphicsOptions_Cache();
ADDRGP4 GraphicsOptions_Cache
CALLV
pop
line 979
;979:	UI_DisplayOptionsMenu_Cache();
ADDRGP4 UI_DisplayOptionsMenu_Cache
CALLV
pop
line 980
;980:	UI_SoundOptionsMenu_Cache();
ADDRGP4 UI_SoundOptionsMenu_Cache
CALLV
pop
line 981
;981:	UI_NetworkOptionsMenu_Cache();
ADDRGP4 UI_NetworkOptionsMenu_Cache
CALLV
pop
line 982
;982:	UI_SPLevelMenu_Cache();
ADDRGP4 UI_SPLevelMenu_Cache
CALLV
pop
line 983
;983:	UI_SPSkillMenu_Cache();
ADDRGP4 UI_SPSkillMenu_Cache
CALLV
pop
line 984
;984:	UI_SPPostgameMenu_Cache();
ADDRGP4 UI_SPPostgameMenu_Cache
CALLV
pop
line 985
;985:	TeamMain_Cache();
ADDRGP4 TeamMain_Cache
CALLV
pop
line 986
;986:	UI_AddBots_Cache();
ADDRGP4 UI_AddBots_Cache
CALLV
pop
line 987
;987:	UI_RemoveBots_Cache();
ADDRGP4 UI_RemoveBots_Cache
CALLV
pop
line 988
;988:	UI_SetupMenu_Cache();
ADDRGP4 UI_SetupMenu_Cache
CALLV
pop
line 991
;989://	UI_LoadConfig_Cache();
;990://	UI_SaveConfigMenu_Cache();
;991:	UI_BotSelectMenu_Cache();
ADDRGP4 UI_BotSelectMenu_Cache
CALLV
pop
line 992
;992:	UI_CDKeyMenu_Cache();
ADDRGP4 UI_CDKeyMenu_Cache
CALLV
pop
line 993
;993:	UI_ModsMenu_Cache();
ADDRGP4 UI_ModsMenu_Cache
CALLV
pop
line 995
;994:
;995:}
LABELV $402
endproc UI_Cache_f 0 0
export UI_ConsoleCommand
proc UI_ConsoleCommand 40 8
line 1003
;996:
;997:
;998:/*
;999:=================
;1000:UI_ConsoleCommand
;1001:=================
;1002:*/
;1003:qboolean UI_ConsoleCommand( int realTime ) {
line 1006
;1004:	char	*cmd;
;1005:
;1006:	cmd = UI_Argv( 0 );
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1009
;1007:
;1008:	// ensure minimum menu data is available
;1009:	Menu_Cache();
ADDRGP4 Menu_Cache
CALLV
pop
line 1011
;1010:
;1011:	if ( Q_stricmp (cmd, "levelselect") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $406
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $404
line 1012
;1012:		UI_SPLevelMenu_f();
ADDRGP4 UI_SPLevelMenu_f
CALLV
pop
line 1013
;1013:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $404
line 1016
;1014:	}
;1015:
;1016:	if ( Q_stricmp (cmd, "postgame") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $409
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $407
line 1017
;1017:		UI_SPPostgameMenu_f();
ADDRGP4 UI_SPPostgameMenu_f
CALLV
pop
line 1018
;1018:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $407
line 1021
;1019:	}
;1020:
;1021:	if ( Q_stricmp (cmd, "ui_cache") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $412
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $410
line 1022
;1022:		UI_Cache_f();
ADDRGP4 UI_Cache_f
CALLV
pop
line 1023
;1023:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $410
line 1026
;1024:	}
;1025:
;1026:	if ( Q_stricmp (cmd, "ui_cinematics") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $415
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $413
line 1027
;1027:		UI_CinematicsMenu_f();
ADDRGP4 UI_CinematicsMenu_f
CALLV
pop
line 1028
;1028:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $413
line 1031
;1029:	}
;1030:
;1031:	if ( Q_stricmp (cmd, "ui_teamOrders") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $418
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $416
line 1032
;1032:		UI_TeamOrdersMenu_f();
ADDRGP4 UI_TeamOrdersMenu_f
CALLV
pop
line 1033
;1033:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $416
line 1036
;1034:	}
;1035:
;1036:	if ( Q_stricmp (cmd, "iamacheater") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $421
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $419
line 1037
;1037:		UI_SPUnlock_f();
ADDRGP4 UI_SPUnlock_f
CALLV
pop
line 1038
;1038:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $419
line 1041
;1039:	}
;1040:
;1041:	if ( Q_stricmp (cmd, "iamamonkey") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $424
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $422
line 1042
;1042:		UI_SPUnlockMedals_f();
ADDRGP4 UI_SPUnlockMedals_f
CALLV
pop
line 1043
;1043:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $422
line 1046
;1044:	}
;1045:
;1046:	if ( Q_stricmp (cmd, "ui_cdkey") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $427
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $425
line 1047
;1047:		UI_CDKeyMenu_f();
ADDRGP4 UI_CDKeyMenu_f
CALLV
pop
line 1048
;1048:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $403
JUMPV
LABELV $425
line 1051
;1049:	}
;1050:
;1051:	return qfalse;
CNSTI4 0
RETI4
LABELV $403
endproc UI_ConsoleCommand 40 8
export UI_Shutdown
proc UI_Shutdown 0 0
line 1059
;1052:}
;1053:
;1054:/*
;1055:=================
;1056:UI_Shutdown
;1057:=================
;1058:*/
;1059:void UI_Shutdown( void ) {
line 1060
;1060:}
LABELV $428
endproc UI_Shutdown 0 0
export UI_Init
proc UI_Init 0 4
line 1067
;1061:
;1062:/*
;1063:=================
;1064:UI_Init
;1065:=================
;1066:*/
;1067:void UI_Init( void ) {
line 1068
;1068:	UI_RegisterCvars();
ADDRGP4 UI_RegisterCvars
CALLV
pop
line 1070
;1069:
;1070:	UI_InitGameinfo();
ADDRGP4 UI_InitGameinfo
CALLV
pop
line 1073
;1071:
;1072:	// cache redundant calulations
;1073:	trap_GetGlconfig( &uis.glconfig );
ADDRGP4 uis+56
ARGP4
ADDRGP4 trap_GetGlconfig
CALLV
pop
line 1076
;1074:
;1075:	// for 640x480 virtualized screen
;1076:	uis.scale = uis.glconfig.vidHeight * (1.0/480.0);
ADDRGP4 uis+11432
CNSTF4 990414985
ADDRGP4 uis+56+11308
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1077
;1077:	if ( uis.glconfig.vidWidth * 480 > uis.glconfig.vidHeight * 640 ) {
CNSTI4 480
ADDRGP4 uis+56+11304
INDIRI4
MULI4
CNSTI4 640
ADDRGP4 uis+56+11308
INDIRI4
MULI4
LEI4 $434
line 1079
;1078:		// wide screen
;1079:		uis.bias = 0.5 * ( uis.glconfig.vidWidth - ( uis.glconfig.vidHeight * (640.0/480.0) ) );
ADDRGP4 uis+11436
CNSTF4 1056964608
ADDRGP4 uis+56+11304
INDIRI4
CVIF4 4
CNSTF4 1068149419
ADDRGP4 uis+56+11308
INDIRI4
CVIF4 4
MULF4
SUBF4
MULF4
ASGNF4
line 1080
;1080:	}
ADDRGP4 $435
JUMPV
LABELV $434
line 1081
;1081:	else {
line 1083
;1082:		// no wide screen
;1083:		uis.bias = 0;
ADDRGP4 uis+11436
CNSTF4 0
ASGNF4
line 1084
;1084:	}
LABELV $435
line 1087
;1085:
;1086:	// initialize the menu system
;1087:	Menu_Cache();
ADDRGP4 Menu_Cache
CALLV
pop
line 1089
;1088:
;1089:	uis.activemenu = NULL;
ADDRGP4 uis+20
CNSTP4 0
ASGNP4
line 1090
;1090:	uis.menusp     = 0;
ADDRGP4 uis+16
CNSTI4 0
ASGNI4
line 1091
;1091:}
LABELV $429
endproc UI_Init 0 4
export UI_AdjustFrom640
proc UI_AdjustFrom640 16 0
line 1100
;1092:
;1093:/*
;1094:================
;1095:UI_AdjustFrom640
;1096:
;1097:Adjusted for resolution and screen aspect ratio
;1098:================
;1099:*/
;1100:void UI_AdjustFrom640( float *x, float *y, float *w, float *h ) {
line 1102
;1101:	// expect valid pointers
;1102:	*x = *x * uis.scale + uis.bias;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRGP4 uis+11432
INDIRF4
MULF4
ADDRGP4 uis+11436
INDIRF4
ADDF4
ASGNF4
line 1103
;1103:	*y *= uis.scale;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 1104
;1104:	*w *= uis.scale;
ADDRLP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 1105
;1105:	*h *= uis.scale;
ADDRLP4 12
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRGP4 uis+11432
INDIRF4
MULF4
ASGNF4
line 1106
;1106:}
LABELV $448
endproc UI_AdjustFrom640 16 0
export UI_DrawNamedPic
proc UI_DrawNamedPic 16 36
line 1108
;1107:
;1108:void UI_DrawNamedPic( float x, float y, float width, float height, const char *picname ) {
line 1111
;1109:	qhandle_t	hShader;
;1110:
;1111:	hShader = trap_R_RegisterShaderNoMip( picname );
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1112
;1112:	UI_AdjustFrom640( &x, &y, &width, &height );
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
line 1113
;1113:	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
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
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1114
;1114:}
LABELV $454
endproc UI_DrawNamedPic 16 36
export UI_DrawHandlePic
proc UI_DrawHandlePic 16 36
line 1116
;1115:
;1116:void UI_DrawHandlePic( float x, float y, float w, float h, qhandle_t hShader ) {
line 1122
;1117:	float	s0;
;1118:	float	s1;
;1119:	float	t0;
;1120:	float	t1;
;1121:
;1122:	if( w < 0 ) {	// flip about vertical
ADDRFP4 8
INDIRF4
CNSTF4 0
GEF4 $456
line 1123
;1123:		w  = -w;
ADDRFP4 8
ADDRFP4 8
INDIRF4
NEGF4
ASGNF4
line 1124
;1124:		s0 = 1;
ADDRLP4 0
CNSTF4 1065353216
ASGNF4
line 1125
;1125:		s1 = 0;
ADDRLP4 4
CNSTF4 0
ASGNF4
line 1126
;1126:	}
ADDRGP4 $457
JUMPV
LABELV $456
line 1127
;1127:	else {
line 1128
;1128:		s0 = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1129
;1129:		s1 = 1;
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 1130
;1130:	}
LABELV $457
line 1132
;1131:
;1132:	if( h < 0 ) {	// flip about horizontal
ADDRFP4 12
INDIRF4
CNSTF4 0
GEF4 $458
line 1133
;1133:		h  = -h;
ADDRFP4 12
ADDRFP4 12
INDIRF4
NEGF4
ASGNF4
line 1134
;1134:		t0 = 1;
ADDRLP4 8
CNSTF4 1065353216
ASGNF4
line 1135
;1135:		t1 = 0;
ADDRLP4 12
CNSTF4 0
ASGNF4
line 1136
;1136:	}
ADDRGP4 $459
JUMPV
LABELV $458
line 1137
;1137:	else {
line 1138
;1138:		t0 = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 1139
;1139:		t1 = 1;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 1140
;1140:	}
LABELV $459
line 1142
;1141:	
;1142:	UI_AdjustFrom640( &x, &y, &w, &h );
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
line 1143
;1143:	trap_R_DrawStretchPic( x, y, w, h, s0, t0, s1, t1, hShader );
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
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1144
;1144:}
LABELV $455
endproc UI_DrawHandlePic 16 36
export UI_FillRect
proc UI_FillRect 4 36
line 1153
;1145:
;1146:/*
;1147:================
;1148:UI_FillRect
;1149:
;1150:Coordinates are 640*480 virtual values
;1151:=================
;1152:*/
;1153:void UI_FillRect( float x, float y, float width, float height, const float *color ) {
line 1154
;1154:	trap_R_SetColor( color );
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1156
;1155:
;1156:	UI_AdjustFrom640( &x, &y, &width, &height );
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
line 1157
;1157:	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 0, 0, uis.whiteShader );
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
ADDRGP4 uis+11392
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1159
;1158:
;1159:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1160
;1160:}
LABELV $460
endproc UI_FillRect 4 36
export UI_DrawRect
proc UI_DrawRect 24 36
line 1169
;1161:
;1162:/*
;1163:================
;1164:UI_DrawRect
;1165:
;1166:Coordinates are 640*480 virtual values
;1167:=================
;1168:*/
;1169:void UI_DrawRect( float x, float y, float width, float height, const float *color ) {
line 1170
;1170:	trap_R_SetColor( color );
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1172
;1171:
;1172:	UI_AdjustFrom640( &x, &y, &width, &height );
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
line 1174
;1173:
;1174:	trap_R_DrawStretchPic( x, y, width, 1, 0, 0, 0, 0, uis.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
CNSTF4 1065353216
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
ADDRGP4 uis+11392
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1175
;1175:	trap_R_DrawStretchPic( x, y, 1, height, 0, 0, 0, 0, uis.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRGP4 uis+11392
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1176
;1176:	trap_R_DrawStretchPic( x, y + height - 1, width, 1, 0, 0, 0, 0, uis.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 1065353216
ASGNF4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ADDRLP4 8
INDIRF4
SUBF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRGP4 uis+11392
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1177
;1177:	trap_R_DrawStretchPic( x + width - 1, y, 1, height, 0, 0, 0, 0, uis.whiteShader );
ADDRLP4 16
CNSTF4 1065353216
ASGNF4
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRLP4 16
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRGP4 uis+11392
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 1179
;1178:
;1179:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1180
;1180:}
LABELV $462
endproc UI_DrawRect 24 36
export UI_SetColor
proc UI_SetColor 0 4
line 1182
;1181:
;1182:void UI_SetColor( const float *rgba ) {
line 1183
;1183:	trap_R_SetColor( rgba );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 1184
;1184:}
LABELV $467
endproc UI_SetColor 0 4
export UI_UpdateScreen
proc UI_UpdateScreen 0 0
line 1186
;1185:
;1186:void UI_UpdateScreen( void ) {
line 1187
;1187:	trap_UpdateScreen();
ADDRGP4 trap_UpdateScreen
CALLV
pop
line 1188
;1188:}
LABELV $468
endproc UI_UpdateScreen 0 0
export UI_Refresh
proc UI_Refresh 20 20
line 1196
;1189:
;1190:/*
;1191:=================
;1192:UI_Refresh
;1193:=================
;1194:*/
;1195:void UI_Refresh( int realtime )
;1196:{
line 1197
;1197:	uis.frametime = realtime - uis.realtime;
ADDRGP4 uis
ADDRFP4 0
INDIRI4
ADDRGP4 uis+4
INDIRI4
SUBI4
ASGNI4
line 1198
;1198:	uis.realtime  = realtime;
ADDRGP4 uis+4
ADDRFP4 0
INDIRI4
ASGNI4
line 1200
;1199:
;1200:	if ( !( trap_Key_GetCatcher() & KEYCATCH_UI ) ) {
ADDRLP4 0
ADDRGP4 trap_Key_GetCatcher
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
NEI4 $472
line 1201
;1201:		return;
ADDRGP4 $469
JUMPV
LABELV $472
line 1204
;1202:	}
;1203:
;1204:	UI_UpdateCvars();
ADDRGP4 UI_UpdateCvars
CALLV
pop
line 1206
;1205:
;1206:	if ( uis.activemenu )
ADDRGP4 uis+20
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $474
line 1207
;1207:	{
line 1208
;1208:		if (uis.activemenu->fullscreen)
ADDRGP4 uis+20
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
CNSTI4 0
EQI4 $477
line 1209
;1209:		{
line 1211
;1210:			// draw the background
;1211:			if( uis.activemenu->showlogo ) {
ADDRGP4 uis+20
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
CNSTI4 0
EQI4 $480
line 1212
;1212:				UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menuBackShader );
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
CNSTF4 1142947840
ARGF4
CNSTF4 1139802112
ARGF4
ADDRGP4 uis+11396
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1213
;1213:			}
ADDRGP4 $481
JUMPV
LABELV $480
line 1214
;1214:			else {
line 1215
;1215:				UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menuBackNoLogoShader );
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
CNSTF4 1142947840
ARGF4
CNSTF4 1139802112
ARGF4
ADDRGP4 uis+11400
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1216
;1216:			}
LABELV $481
line 1217
;1217:		}
LABELV $477
line 1219
;1218:
;1219:		if (uis.activemenu->draw)
ADDRGP4 uis+20
INDIRP4
CNSTI4 268
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $485
line 1220
;1220:			uis.activemenu->draw();
ADDRGP4 uis+20
INDIRP4
CNSTI4 268
ADDP4
INDIRP4
CALLV
pop
ADDRGP4 $486
JUMPV
LABELV $485
line 1222
;1221:		else
;1222:			Menu_Draw( uis.activemenu );
ADDRGP4 uis+20
INDIRP4
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
LABELV $486
line 1224
;1223:
;1224:		if( uis.firstdraw ) {
ADDRGP4 uis+11444
INDIRI4
CNSTI4 0
EQI4 $490
line 1225
;1225:			UI_MouseEvent( 0, 0 );
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 UI_MouseEvent
CALLV
pop
line 1226
;1226:			uis.firstdraw = qfalse;
ADDRGP4 uis+11444
CNSTI4 0
ASGNI4
line 1227
;1227:		}
LABELV $490
line 1228
;1228:	}
LABELV $474
line 1231
;1229:
;1230:	// draw cursor
;1231:	UI_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 UI_SetColor
CALLV
pop
line 1232
;1232:	UI_DrawHandlePic( uis.cursorx-16, uis.cursory-16, 32, 32, uis.cursor);
ADDRLP4 4
CNSTI4 16
ASGNI4
ADDRGP4 uis+8
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CVIF4 4
ARGF4
ADDRGP4 uis+12
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CVIF4 4
ARGF4
ADDRLP4 8
CNSTF4 1107296256
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 uis+11420
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 1235
;1233:
;1234:#ifndef NDEBUG
;1235:	if (uis.debug)
ADDRGP4 uis+11388
INDIRI4
CNSTI4 0
EQI4 $497
line 1236
;1236:	{
line 1238
;1237:		// cursor coordinates
;1238:		UI_DrawString( 0, 0, va("(%d,%d)",uis.cursorx,uis.cursory), UI_LEFT|UI_SMALLFONT, colorRed );
ADDRGP4 $500
ARGP4
ADDRGP4 uis+8
INDIRI4
ARGI4
ADDRGP4 uis+12
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 colorRed
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 1239
;1239:	}
LABELV $497
line 1245
;1240:#endif
;1241:
;1242:	// delay playing the enter sound until after the
;1243:	// menu has been drawn, to avoid delay while
;1244:	// caching images
;1245:	if (m_entersound)
ADDRGP4 m_entersound
INDIRI4
CNSTI4 0
EQI4 $503
line 1246
;1246:	{
line 1247
;1247:		trap_S_StartLocalSound( menu_in_sound, CHAN_LOCAL_SOUND );
ADDRGP4 menu_in_sound
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 trap_S_StartLocalSound
CALLV
pop
line 1248
;1248:		m_entersound = qfalse;
ADDRGP4 m_entersound
CNSTI4 0
ASGNI4
line 1249
;1249:	}
LABELV $503
line 1250
;1250:}
LABELV $469
endproc UI_Refresh 20 20
export UI_DrawTextBox
proc UI_DrawTextBox 24 20
line 1253
;1251:
;1252:void UI_DrawTextBox (int x, int y, int width, int lines)
;1253:{
line 1254
;1254:	UI_FillRect( x + BIGCHAR_WIDTH/2, y + BIGCHAR_HEIGHT/2, ( width + 1 ) * BIGCHAR_WIDTH, ( lines + 1 ) * BIGCHAR_HEIGHT, colorBlack );
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 4
CNSTI4 4
ASGNI4
ADDRLP4 8
CNSTI4 16
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRLP4 8
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRLP4 8
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 colorBlack
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 1255
;1255:	UI_DrawRect( x + BIGCHAR_WIDTH/2, y + BIGCHAR_HEIGHT/2, ( width + 1 ) * BIGCHAR_WIDTH, ( lines + 1 ) * BIGCHAR_HEIGHT, colorWhite );
ADDRLP4 12
CNSTI4 8
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 16
CNSTI4 4
ASGNI4
ADDRLP4 20
CNSTI4 16
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRLP4 20
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 12
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRLP4 20
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 UI_DrawRect
CALLV
pop
line 1256
;1256:}
LABELV $505
endproc UI_DrawTextBox 24 20
export UI_CursorInRect
proc UI_CursorInRect 8 0
line 1259
;1257:
;1258:qboolean UI_CursorInRect (int x, int y, int width, int height)
;1259:{
line 1260
;1260:	if (uis.cursorx < x ||
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRGP4 uis+8
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $515
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 uis+12
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $515
ADDRGP4 uis+8
INDIRI4
ADDRLP4 0
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
GTI4 $515
ADDRGP4 uis+12
INDIRI4
ADDRLP4 4
INDIRI4
ADDRFP4 12
INDIRI4
ADDI4
LEI4 $507
LABELV $515
line 1264
;1261:		uis.cursory < y ||
;1262:		uis.cursorx > x+width ||
;1263:		uis.cursory > y+height)
;1264:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $506
JUMPV
LABELV $507
line 1266
;1265:
;1266:	return qtrue;
CNSTI4 1
RETI4
LABELV $506
endproc UI_CursorInRect 8 0
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
bss
export uis
align 4
LABELV uis
skip 11448
export m_entersound
align 4
LABELV m_entersound
skip 4
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
LABELV $500
byte 1 40
byte 1 37
byte 1 100
byte 1 44
byte 1 37
byte 1 100
byte 1 41
byte 1 0
align 1
LABELV $427
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $424
byte 1 105
byte 1 97
byte 1 109
byte 1 97
byte 1 109
byte 1 111
byte 1 110
byte 1 107
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $421
byte 1 105
byte 1 97
byte 1 109
byte 1 97
byte 1 99
byte 1 104
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $418
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $415
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 105
byte 1 110
byte 1 101
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 115
byte 1 0
align 1
LABELV $412
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 97
byte 1 99
byte 1 104
byte 1 101
byte 1 0
align 1
LABELV $409
byte 1 112
byte 1 111
byte 1 115
byte 1 116
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $406
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $327
byte 1 85
byte 1 73
byte 1 95
byte 1 83
byte 1 101
byte 1 116
byte 1 65
byte 1 99
byte 1 116
byte 1 105
byte 1 118
byte 1 101
byte 1 77
byte 1 101
byte 1 110
byte 1 117
byte 1 58
byte 1 32
byte 1 98
byte 1 97
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 117
byte 1 109
byte 1 32
byte 1 37
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $325
byte 1 49
byte 1 0
align 1
LABELV $323
byte 1 66
byte 1 97
byte 1 100
byte 1 32
byte 1 67
byte 1 68
byte 1 32
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $321
byte 1 73
byte 1 110
byte 1 115
byte 1 101
byte 1 114
byte 1 116
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 67
byte 1 68
byte 1 0
align 1
LABELV $310
byte 1 113
byte 1 117
byte 1 105
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $124
byte 1 48
byte 1 0
align 1
LABELV $123
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
LABELV $111
byte 1 85
byte 1 73
byte 1 95
byte 1 80
byte 1 111
byte 1 112
byte 1 77
byte 1 101
byte 1 110
byte 1 117
byte 1 58
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 115
byte 1 116
byte 1 97
byte 1 99
byte 1 107
byte 1 32
byte 1 117
byte 1 110
byte 1 100
byte 1 101
byte 1 114
byte 1 102
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $95
byte 1 85
byte 1 73
byte 1 95
byte 1 80
byte 1 117
byte 1 115
byte 1 104
byte 1 77
byte 1 101
byte 1 110
byte 1 117
byte 1 58
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 115
byte 1 116
byte 1 97
byte 1 99
byte 1 107
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 102
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $78
byte 1 100
byte 1 49
byte 1 10
byte 1 0
align 1
LABELV $69
byte 1 37
byte 1 115
byte 1 0
