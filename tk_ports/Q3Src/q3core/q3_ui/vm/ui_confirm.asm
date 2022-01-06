code
proc ConfirmMenu_Event 4 4
file "../ui_confirm.c"
line 65
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
;26:CONFIRMATION MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ART_CONFIRM_FRAME	"menu/art/cut_frame"
;36:
;37:#define ID_CONFIRM_NO		10
;38:#define ID_CONFIRM_YES		11
;39:
;40:
;41:typedef struct {
;42:	menuframework_s menu;
;43:
;44:	menutext_s		no;
;45:	menutext_s		yes;
;46:
;47:	int				slashX;
;48:	const char *	question;
;49:	void			(*draw)( void );
;50:	void			(*action)( qboolean result );
;51:	
;52:	int style;
;53:	const char **lines;
;54:} confirmMenu_t;
;55:
;56:
;57:static confirmMenu_t	s_confirm;
;58:
;59:
;60:/*
;61:=================
;62:ConfirmMenu_Event
;63:=================
;64:*/
;65:static void ConfirmMenu_Event( void* ptr, int event ) {
line 68
;66:	qboolean	result;
;67:
;68:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $69
line 69
;69:		return;
ADDRGP4 $68
JUMPV
LABELV $69
line 72
;70:	}
;71:
;72:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 74
;73:
;74:	if( ((menucommon_s*)ptr)->id == ID_CONFIRM_NO ) {
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 10
NEI4 $71
line 75
;75:		result = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 76
;76:	}
ADDRGP4 $72
JUMPV
LABELV $71
line 77
;77:	else {
line 78
;78:		result = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 79
;79:	}
LABELV $72
line 81
;80:
;81:	if( s_confirm.action ) {
ADDRGP4 s_confirm+444
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $73
line 82
;82:		s_confirm.action( result );
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 s_confirm+444
INDIRP4
CALLV
pop
line 83
;83:	}
LABELV $73
line 84
;84:}
LABELV $68
endproc ConfirmMenu_Event 4 4
proc ConfirmMenu_Key 32 8
line 92
;85:
;86:
;87:/*
;88:=================
;89:ConfirmMenu_Key
;90:=================
;91:*/
;92:static sfxHandle_t ConfirmMenu_Key( int key ) {
line 93
;93:	switch ( key ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 110
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $81
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
GTI4 $86
LABELV $85
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 12
CNSTI4 78
ASGNI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $81
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LTI4 $78
LABELV $87
ADDRFP4 0
INDIRI4
CNSTI4 89
EQI4 $83
ADDRGP4 $78
JUMPV
LABELV $86
ADDRLP4 16
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 134
EQI4 $80
ADDRLP4 20
CNSTI4 135
ASGNI4
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $80
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
GTI4 $89
LABELV $88
ADDRFP4 0
INDIRI4
CNSTI4 121
EQI4 $83
ADDRGP4 $78
JUMPV
LABELV $89
ADDRLP4 24
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 163
EQI4 $80
ADDRLP4 24
INDIRI4
CNSTI4 165
EQI4 $80
ADDRGP4 $78
JUMPV
LABELV $80
line 98
;94:	case K_KP_LEFTARROW:
;95:	case K_LEFTARROW:
;96:	case K_KP_RIGHTARROW:
;97:	case K_RIGHTARROW:
;98:		key = K_TAB;
ADDRFP4 0
CNSTI4 9
ASGNI4
line 99
;99:		break;
ADDRGP4 $79
JUMPV
LABELV $81
line 103
;100:
;101:	case 'n':
;102:	case 'N':
;103:		ConfirmMenu_Event( &s_confirm.no, QM_ACTIVATED );
ADDRGP4 s_confirm+288
ARGP4
CNSTI4 3
ARGI4
ADDRGP4 ConfirmMenu_Event
CALLV
pop
line 104
;104:		break;
ADDRGP4 $79
JUMPV
LABELV $83
line 108
;105:
;106:	case 'y':
;107:	case 'Y':
;108:		ConfirmMenu_Event( &s_confirm.yes, QM_ACTIVATED );
ADDRGP4 s_confirm+360
ARGP4
CNSTI4 3
ARGI4
ADDRGP4 ConfirmMenu_Event
CALLV
pop
line 109
;109:		break;
LABELV $78
LABELV $79
line 112
;110:	}
;111:
;112:	return Menu_DefaultKey( &s_confirm.menu, key );
ADDRGP4 s_confirm
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
RETI4
LABELV $77
endproc ConfirmMenu_Key 32 8
proc MessageMenu_Draw 8 20
line 121
;113:}
;114:
;115:
;116:/*
;117:=================
;118:MessaheMenu_Draw
;119:=================
;120:*/
;121:static void MessageMenu_Draw( void ) {
line 124
;122:	int i,y;
;123:	
;124:	UI_DrawNamedPic( 142, 118, 359, 256, ART_CONFIRM_FRAME );
CNSTF4 1124990976
ARGF4
CNSTF4 1122762752
ARGF4
CNSTF4 1135837184
ARGF4
CNSTF4 1132462080
ARGF4
ADDRGP4 $91
ARGP4
ADDRGP4 UI_DrawNamedPic
CALLV
pop
line 126
;125:	
;126:	y = 188;
ADDRLP4 4
CNSTI4 188
ASGNI4
line 127
;127:	for(i=0; s_confirm.lines[i]; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $95
JUMPV
LABELV $92
line 128
;128:	{
line 129
;129:		UI_DrawProportionalString( 320, y, s_confirm.lines[i], s_confirm.style, color_red );
CNSTI4 320
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_confirm+452
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 s_confirm+448
INDIRI4
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 130
;130:		y += 18;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 131
;131:	}
LABELV $93
line 127
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $95
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_confirm+452
INDIRP4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $92
line 133
;132:
;133:	Menu_Draw( &s_confirm.menu );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 135
;134:
;135:	if( s_confirm.draw ) {
ADDRGP4 s_confirm+440
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $99
line 136
;136:		s_confirm.draw();
ADDRGP4 s_confirm+440
INDIRP4
CALLV
pop
line 137
;137:	}
LABELV $99
line 138
;138:}
LABELV $90
endproc MessageMenu_Draw 8 20
proc ConfirmMenu_Draw 0 20
line 145
;139:
;140:/*
;141:=================
;142:ConfirmMenu_Draw
;143:=================
;144:*/
;145:static void ConfirmMenu_Draw( void ) {
line 146
;146:	UI_DrawNamedPic( 142, 118, 359, 256, ART_CONFIRM_FRAME );
CNSTF4 1124990976
ARGF4
CNSTF4 1122762752
ARGF4
CNSTF4 1135837184
ARGF4
CNSTF4 1132462080
ARGF4
ADDRGP4 $91
ARGP4
ADDRGP4 UI_DrawNamedPic
CALLV
pop
line 147
;147:	UI_DrawProportionalString( 320, 204, s_confirm.question, s_confirm.style, color_red );
CNSTI4 320
ARGI4
CNSTI4 204
ARGI4
ADDRGP4 s_confirm+436
INDIRP4
ARGP4
ADDRGP4 s_confirm+448
INDIRI4
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 148
;148:	UI_DrawProportionalString( s_confirm.slashX, 265, "/", UI_LEFT|UI_INVERSE, color_red );
ADDRGP4 s_confirm+432
INDIRI4
ARGI4
CNSTI4 265
ARGI4
ADDRGP4 $107
ARGP4
CNSTI4 8192
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 150
;149:
;150:	Menu_Draw( &s_confirm.menu );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 152
;151:
;152:	if( s_confirm.draw ) {
ADDRGP4 s_confirm+440
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $108
line 153
;153:		s_confirm.draw();
ADDRGP4 s_confirm+440
INDIRP4
CALLV
pop
line 154
;154:	}
LABELV $108
line 155
;155:}
LABELV $103
endproc ConfirmMenu_Draw 0 20
export ConfirmMenu_Cache
proc ConfirmMenu_Cache 0 4
line 163
;156:
;157:
;158:/*
;159:=================
;160:ConfirmMenu_Cache
;161:=================
;162:*/
;163:void ConfirmMenu_Cache( void ) {
line 164
;164:	trap_R_RegisterShaderNoMip( ART_CONFIRM_FRAME );
ADDRGP4 $91
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 165
;165:}
LABELV $112
endproc ConfirmMenu_Cache 0 4
export UI_ConfirmMenu_Style
proc UI_ConfirmMenu_Style 3120 12
line 173
;166:
;167:
;168:/*
;169:=================
;170:UI_ConfirmMenu_Stlye
;171:=================
;172:*/
;173:void UI_ConfirmMenu_Style( const char *question, int style, void (*draw)( void ), void (*action)( qboolean result ) ) {
line 179
;174:	uiClientState_t	cstate;
;175:	int	n1, n2, n3;
;176:	int	l1, l2, l3;
;177:
;178:	// zero set all our globals
;179:	memset( &s_confirm, 0, sizeof(s_confirm) );
ADDRGP4 s_confirm
ARGP4
CNSTI4 0
ARGI4
CNSTI4 456
ARGI4
ADDRGP4 memset
CALLP4
pop
line 181
;180:
;181:	ConfirmMenu_Cache();
ADDRGP4 ConfirmMenu_Cache
CALLV
pop
line 183
;182:
;183:	n1 = UI_ProportionalStringWidth( "YES/NO" );
ADDRGP4 $114
ARGP4
ADDRLP4 3108
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 3092
ADDRLP4 3108
INDIRI4
ASGNI4
line 184
;184:	n2 = UI_ProportionalStringWidth( "YES" ) + PROP_GAP_WIDTH;
ADDRGP4 $115
ARGP4
ADDRLP4 3112
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 3096
ADDRLP4 3112
INDIRI4
CNSTI4 3
ADDI4
ASGNI4
line 185
;185:	n3 = UI_ProportionalStringWidth( "/" )  + PROP_GAP_WIDTH;
ADDRGP4 $107
ARGP4
ADDRLP4 3116
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 3100
ADDRLP4 3116
INDIRI4
CNSTI4 3
ADDI4
ASGNI4
line 186
;186:	l1 = 320 - ( n1 / 2 );
ADDRLP4 0
CNSTI4 320
ADDRLP4 3092
INDIRI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 187
;187:	l2 = l1 + n2;
ADDRLP4 4
ADDRLP4 0
INDIRI4
ADDRLP4 3096
INDIRI4
ADDI4
ASGNI4
line 188
;188:	l3 = l2 + n3;
ADDRLP4 3104
ADDRLP4 4
INDIRI4
ADDRLP4 3100
INDIRI4
ADDI4
ASGNI4
line 189
;189:	s_confirm.slashX = l2;
ADDRGP4 s_confirm+432
ADDRLP4 4
INDIRI4
ASGNI4
line 191
;190:
;191:	s_confirm.question = question;
ADDRGP4 s_confirm+436
ADDRFP4 0
INDIRP4
ASGNP4
line 192
;192:	s_confirm.draw = draw;
ADDRGP4 s_confirm+440
ADDRFP4 8
INDIRP4
ASGNP4
line 193
;193:	s_confirm.action = action;
ADDRGP4 s_confirm+444
ADDRFP4 12
INDIRP4
ASGNP4
line 194
;194:	s_confirm.style = style;
ADDRGP4 s_confirm+448
ADDRFP4 4
INDIRI4
ASGNI4
line 196
;195:
;196:	s_confirm.menu.draw       = ConfirmMenu_Draw;
ADDRGP4 s_confirm+268
ADDRGP4 ConfirmMenu_Draw
ASGNP4
line 197
;197:	s_confirm.menu.key        = ConfirmMenu_Key;
ADDRGP4 s_confirm+272
ADDRGP4 ConfirmMenu_Key
ASGNP4
line 198
;198:	s_confirm.menu.wrapAround = qtrue;
ADDRGP4 s_confirm+276
CNSTI4 1
ASGNI4
line 200
;199:
;200:	trap_GetClientState( &cstate );
ADDRLP4 8
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 201
;201:	if ( cstate.connState >= CA_CONNECTED ) {
ADDRLP4 8
INDIRI4
CNSTI4 5
LTI4 $124
line 202
;202:		s_confirm.menu.fullscreen = qfalse;
ADDRGP4 s_confirm+280
CNSTI4 0
ASGNI4
line 203
;203:	}
ADDRGP4 $125
JUMPV
LABELV $124
line 204
;204:	else {
line 205
;205:		s_confirm.menu.fullscreen = qtrue;
ADDRGP4 s_confirm+280
CNSTI4 1
ASGNI4
line 206
;206:	}
LABELV $125
line 208
;207:
;208:	s_confirm.yes.generic.type		= MTYPE_PTEXT;      
ADDRGP4 s_confirm+360
CNSTI4 9
ASGNI4
line 209
;209:	s_confirm.yes.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS; 
ADDRGP4 s_confirm+360+44
CNSTU4 260
ASGNU4
line 210
;210:	s_confirm.yes.generic.callback	= ConfirmMenu_Event;
ADDRGP4 s_confirm+360+48
ADDRGP4 ConfirmMenu_Event
ASGNP4
line 211
;211:	s_confirm.yes.generic.id		= ID_CONFIRM_YES;
ADDRGP4 s_confirm+360+8
CNSTI4 11
ASGNI4
line 212
;212:	s_confirm.yes.generic.x			= l1;
ADDRGP4 s_confirm+360+12
ADDRLP4 0
INDIRI4
ASGNI4
line 213
;213:	s_confirm.yes.generic.y			= 264;
ADDRGP4 s_confirm+360+16
CNSTI4 264
ASGNI4
line 214
;214:	s_confirm.yes.string			= "YES";
ADDRGP4 s_confirm+360+60
ADDRGP4 $115
ASGNP4
line 215
;215:	s_confirm.yes.color				= color_red;
ADDRGP4 s_confirm+360+68
ADDRGP4 color_red
ASGNP4
line 216
;216:	s_confirm.yes.style				= UI_LEFT;
ADDRGP4 s_confirm+360+64
CNSTI4 0
ASGNI4
line 218
;217:
;218:	s_confirm.no.generic.type		= MTYPE_PTEXT;      
ADDRGP4 s_confirm+288
CNSTI4 9
ASGNI4
line 219
;219:	s_confirm.no.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS; 
ADDRGP4 s_confirm+288+44
CNSTU4 260
ASGNU4
line 220
;220:	s_confirm.no.generic.callback	= ConfirmMenu_Event;
ADDRGP4 s_confirm+288+48
ADDRGP4 ConfirmMenu_Event
ASGNP4
line 221
;221:	s_confirm.no.generic.id			= ID_CONFIRM_NO;
ADDRGP4 s_confirm+288+8
CNSTI4 10
ASGNI4
line 222
;222:	s_confirm.no.generic.x		    = l3;
ADDRGP4 s_confirm+288+12
ADDRLP4 3104
INDIRI4
ASGNI4
line 223
;223:	s_confirm.no.generic.y		    = 264;
ADDRGP4 s_confirm+288+16
CNSTI4 264
ASGNI4
line 224
;224:	s_confirm.no.string				= "NO";
ADDRGP4 s_confirm+288+60
ADDRGP4 $158
ASGNP4
line 225
;225:	s_confirm.no.color			    = color_red;
ADDRGP4 s_confirm+288+68
ADDRGP4 color_red
ASGNP4
line 226
;226:	s_confirm.no.style			    = UI_LEFT;
ADDRGP4 s_confirm+288+64
CNSTI4 0
ASGNI4
line 228
;227:
;228:	Menu_AddItem( &s_confirm.menu,	&s_confirm.yes );             
ADDRGP4 s_confirm
ARGP4
ADDRGP4 s_confirm+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 229
;229:	Menu_AddItem( &s_confirm.menu,	&s_confirm.no );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 s_confirm+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 231
;230:
;231:	UI_PushMenu( &s_confirm.menu );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 233
;232:
;233:	Menu_SetCursorToItem( &s_confirm.menu, &s_confirm.no );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 s_confirm+288
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 234
;234:}
LABELV $113
endproc UI_ConfirmMenu_Style 3120 12
export UI_ConfirmMenu
proc UI_ConfirmMenu 0 16
line 241
;235:
;236:/*
;237:=================
;238:UI_ConfirmMenu
;239:=================
;240:*/
;241:void UI_ConfirmMenu( const char *question, void (*draw)( void ), void (*action)( qboolean result ) ) {
line 242
;242:	UI_ConfirmMenu_Style(question, UI_CENTER|UI_INVERSE, draw, action);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 8193
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 UI_ConfirmMenu_Style
CALLV
pop
line 243
;243:}
LABELV $166
endproc UI_ConfirmMenu 0 16
export UI_Message
proc UI_Message 3096 12
line 251
;244:
;245:/*
;246:=================
;247:UI_Message
;248:hacked over from Confirm stuff
;249:=================
;250:*/
;251:void UI_Message( const char **lines ) {
line 256
;252:	uiClientState_t	cstate;
;253:	int n1, l1;
;254:	
;255:	// zero set all our globals
;256:	memset( &s_confirm, 0, sizeof(s_confirm) );
ADDRGP4 s_confirm
ARGP4
CNSTI4 0
ARGI4
CNSTI4 456
ARGI4
ADDRGP4 memset
CALLP4
pop
line 258
;257:
;258:	ConfirmMenu_Cache();
ADDRGP4 ConfirmMenu_Cache
CALLV
pop
line 260
;259:
;260:	n1 = UI_ProportionalStringWidth( "OK" );
ADDRGP4 $168
ARGP4
ADDRLP4 3092
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 3084
ADDRLP4 3092
INDIRI4
ASGNI4
line 261
;261:	l1 = 320 - ( n1 / 2 );
ADDRLP4 3088
CNSTI4 320
ADDRLP4 3084
INDIRI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 263
;262:	
;263:	s_confirm.lines = lines;
ADDRGP4 s_confirm+452
ADDRFP4 0
INDIRP4
ASGNP4
line 264
;264:	s_confirm.style = UI_CENTER|UI_INVERSE|UI_SMALLFONT;
ADDRGP4 s_confirm+448
CNSTI4 8209
ASGNI4
line 266
;265:
;266:	s_confirm.menu.draw       = MessageMenu_Draw;
ADDRGP4 s_confirm+268
ADDRGP4 MessageMenu_Draw
ASGNP4
line 267
;267:	s_confirm.menu.key        = ConfirmMenu_Key;
ADDRGP4 s_confirm+272
ADDRGP4 ConfirmMenu_Key
ASGNP4
line 268
;268:	s_confirm.menu.wrapAround = qtrue;
ADDRGP4 s_confirm+276
CNSTI4 1
ASGNI4
line 270
;269:	
;270:	trap_GetClientState( &cstate );
ADDRLP4 0
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 271
;271:	if ( cstate.connState >= CA_CONNECTED ) {
ADDRLP4 0
INDIRI4
CNSTI4 5
LTI4 $174
line 272
;272:		s_confirm.menu.fullscreen = qfalse;
ADDRGP4 s_confirm+280
CNSTI4 0
ASGNI4
line 273
;273:	}
ADDRGP4 $175
JUMPV
LABELV $174
line 274
;274:	else {
line 275
;275:		s_confirm.menu.fullscreen = qtrue;
ADDRGP4 s_confirm+280
CNSTI4 1
ASGNI4
line 276
;276:	}
LABELV $175
line 278
;277:
;278:	s_confirm.yes.generic.type		= MTYPE_PTEXT;      
ADDRGP4 s_confirm+360
CNSTI4 9
ASGNI4
line 279
;279:	s_confirm.yes.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS; 
ADDRGP4 s_confirm+360+44
CNSTU4 260
ASGNU4
line 280
;280:	s_confirm.yes.generic.callback	= ConfirmMenu_Event;
ADDRGP4 s_confirm+360+48
ADDRGP4 ConfirmMenu_Event
ASGNP4
line 281
;281:	s_confirm.yes.generic.id		= ID_CONFIRM_YES;
ADDRGP4 s_confirm+360+8
CNSTI4 11
ASGNI4
line 282
;282:	s_confirm.yes.generic.x			= l1;
ADDRGP4 s_confirm+360+12
ADDRLP4 3088
INDIRI4
ASGNI4
line 283
;283:	s_confirm.yes.generic.y			= 280;
ADDRGP4 s_confirm+360+16
CNSTI4 280
ASGNI4
line 284
;284:	s_confirm.yes.string			= "OK";
ADDRGP4 s_confirm+360+60
ADDRGP4 $168
ASGNP4
line 285
;285:	s_confirm.yes.color				= color_red;
ADDRGP4 s_confirm+360+68
ADDRGP4 color_red
ASGNP4
line 286
;286:	s_confirm.yes.style				= UI_LEFT;
ADDRGP4 s_confirm+360+64
CNSTI4 0
ASGNI4
line 288
;287:
;288:	Menu_AddItem( &s_confirm.menu,	&s_confirm.yes );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 s_confirm+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 290
;289:	
;290:	UI_PushMenu( &s_confirm.menu );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 292
;291:
;292:	Menu_SetCursorToItem( &s_confirm.menu, &s_confirm.yes );
ADDRGP4 s_confirm
ARGP4
ADDRGP4 s_confirm+360
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 293
;293:}
LABELV $167
endproc UI_Message 3096 12
bss
align 4
LABELV s_confirm
skip 456
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
LABELV $168
byte 1 79
byte 1 75
byte 1 0
align 1
LABELV $158
byte 1 78
byte 1 79
byte 1 0
align 1
LABELV $115
byte 1 89
byte 1 69
byte 1 83
byte 1 0
align 1
LABELV $114
byte 1 89
byte 1 69
byte 1 83
byte 1 47
byte 1 78
byte 1 79
byte 1 0
align 1
LABELV $107
byte 1 47
byte 1 0
align 1
LABELV $91
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 99
byte 1 117
byte 1 116
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 0
