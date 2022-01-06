export MField_Draw
code
proc MField_Draw 1056 20
file "../ui_mfield.c"
line 33
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
;25:/*
;26:===================
;27:MField_Draw
;28:
;29:Handles horizontal scrolling and cursor blinking
;30:x, y, are in pixels
;31:===================
;32:*/
;33:void MField_Draw( mfield_t *edit, int x, int y, int style, vec4_t color ) {
line 41
;34:	int		len;
;35:	int		charw;
;36:	int		drawLen;
;37:	int		prestep;
;38:	int		cursorChar;
;39:	char	str[MAX_STRING_CHARS];
;40:
;41:	drawLen = edit->widthInChars;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 42
;42:	len     = strlen( edit->buffer ) + 1;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 1044
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 1044
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 45
;43:
;44:	// guarantee that cursor will be visible
;45:	if ( len <= drawLen ) {
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
GTI4 $68
line 46
;46:		prestep = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 47
;47:	} else {
ADDRGP4 $69
JUMPV
LABELV $68
line 48
;48:		if ( edit->scroll + drawLen > len ) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ADDRLP4 4
INDIRI4
LEI4 $70
line 49
;49:			edit->scroll = len - drawLen;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 50
;50:			if ( edit->scroll < 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 0
GEI4 $72
line 51
;51:				edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 52
;52:			}
LABELV $72
line 53
;53:		}
LABELV $70
line 54
;54:		prestep = edit->scroll;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 55
;55:	}
LABELV $69
line 57
;56:
;57:	if ( prestep + drawLen > len ) {
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ADDRLP4 4
INDIRI4
LEI4 $74
line 58
;58:		drawLen = len - prestep;
ADDRLP4 0
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ASGNI4
line 59
;59:	}
LABELV $74
line 62
;60:
;61:	// extract <drawLen> characters from the field at <prestep>
;62:	if ( drawLen >= MAX_STRING_CHARS ) {
ADDRLP4 0
INDIRI4
CNSTI4 1024
LTI4 $76
line 63
;63:		trap_Error( "drawLen >= MAX_STRING_CHARS" );
ADDRGP4 $78
ARGP4
ADDRGP4 trap_Error
CALLV
pop
line 64
;64:	}
LABELV $76
line 65
;65:	memcpy( str, edit->buffer + prestep, drawLen );
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 66
;66:	str[ drawLen ] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 12
ADDP4
CNSTI1 0
ASGNI1
line 68
;67:
;68:	UI_DrawString( x, y, str, style, color );
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 12
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
line 71
;69:
;70:	// draw the cursor
;71:	if (!(style & UI_PULSE)) {
ADDRFP4 12
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
NEI4 $79
line 72
;72:		return;
ADDRGP4 $67
JUMPV
LABELV $79
line 75
;73:	}
;74:
;75:	if ( trap_Key_GetOverstrikeMode() ) {
ADDRLP4 1048
ADDRGP4 trap_Key_GetOverstrikeMode
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
EQI4 $81
line 76
;76:		cursorChar = 11;
ADDRLP4 1040
CNSTI4 11
ASGNI4
line 77
;77:	} else {
ADDRGP4 $82
JUMPV
LABELV $81
line 78
;78:		cursorChar = 10;
ADDRLP4 1040
CNSTI4 10
ASGNI4
line 79
;79:	}
LABELV $82
line 81
;80:
;81:	style &= ~UI_PULSE;
ADDRFP4 12
ADDRFP4 12
INDIRI4
CNSTI4 -16385
BANDI4
ASGNI4
line 82
;82:	style |= UI_BLINK;
ADDRFP4 12
ADDRFP4 12
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 84
;83:
;84:	if (style & UI_SMALLFONT)
ADDRFP4 12
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $83
line 85
;85:	{
line 86
;86:		charw =	SMALLCHAR_WIDTH;
ADDRLP4 1036
CNSTI4 8
ASGNI4
line 87
;87:	}
ADDRGP4 $84
JUMPV
LABELV $83
line 88
;88:	else if (style & UI_GIANTFONT)
ADDRFP4 12
INDIRI4
CNSTI4 64
BANDI4
CNSTI4 0
EQI4 $85
line 89
;89:	{
line 90
;90:		charw =	GIANTCHAR_WIDTH;
ADDRLP4 1036
CNSTI4 32
ASGNI4
line 91
;91:	}
ADDRGP4 $86
JUMPV
LABELV $85
line 93
;92:	else
;93:	{
line 94
;94:		charw =	BIGCHAR_WIDTH;
ADDRLP4 1036
CNSTI4 16
ASGNI4
line 95
;95:	}
LABELV $86
LABELV $84
line 97
;96:
;97:	if (style & UI_CENTER)
ADDRFP4 12
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $87
line 98
;98:	{
line 99
;99:		len = strlen(str);
ADDRLP4 12
ARGP4
ADDRLP4 1052
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 1052
INDIRI4
ASGNI4
line 100
;100:		x = x - len*charw/2;
ADDRFP4 4
ADDRFP4 4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 1036
INDIRI4
MULI4
CNSTI4 2
DIVI4
SUBI4
ASGNI4
line 101
;101:	}
ADDRGP4 $88
JUMPV
LABELV $87
line 102
;102:	else if (style & UI_RIGHT)
ADDRFP4 12
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $89
line 103
;103:	{
line 104
;104:		len = strlen(str);
ADDRLP4 12
ARGP4
ADDRLP4 1052
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 1052
INDIRI4
ASGNI4
line 105
;105:		x = x - len*charw;
ADDRFP4 4
ADDRFP4 4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 1036
INDIRI4
MULI4
SUBI4
ASGNI4
line 106
;106:	}
LABELV $89
LABELV $88
line 108
;107:	
;108:	UI_DrawChar( x + ( edit->cursor - prestep ) * charw, y, cursorChar, style & ~(UI_CENTER|UI_RIGHT), color );
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ADDRLP4 1036
INDIRI4
MULI4
ADDI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 1040
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
CNSTI4 -4
BANDI4
ARGI4
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 109
;109:}
LABELV $67
endproc MField_Draw 1056 20
export MField_Paste
proc MField_Paste 76 8
line 116
;110:
;111:/*
;112:================
;113:MField_Paste
;114:================
;115:*/
;116:void MField_Paste( mfield_t *edit ) {
line 120
;117:	char	pasteBuffer[64];
;118:	int		pasteLen, i;
;119:
;120:	trap_GetClipboardData( pasteBuffer, 64 );
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_GetClipboardData
CALLV
pop
line 123
;121:
;122:	// send as if typed, so insert / overstrike works properly
;123:	pasteLen = strlen( pasteBuffer );
ADDRLP4 4
ARGP4
ADDRLP4 72
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 68
ADDRLP4 72
INDIRI4
ASGNI4
line 124
;124:	for ( i = 0 ; i < pasteLen ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $95
JUMPV
LABELV $92
line 125
;125:		MField_CharEvent( edit, pasteBuffer[i] );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ADDRLP4 4
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRGP4 MField_CharEvent
CALLV
pop
line 126
;126:	}
LABELV $93
line 124
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $95
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
LTI4 $92
line 127
;127:}
LABELV $91
endproc MField_Paste 76 8
export MField_KeyDownEvent
proc MField_KeyDownEvent 64 12
line 139
;128:
;129:/*
;130:=================
;131:MField_KeyDownEvent
;132:
;133:Performs the basic line editing functions for the console,
;134:in-game talk, and menu fields
;135:
;136:Key events are used for non-printable characters, others are gotten from char events.
;137:=================
;138:*/
;139:void MField_KeyDownEvent( mfield_t *edit, int key ) {
line 143
;140:	int		len;
;141:
;142:	// shift-insert is paste
;143:	if ( ( ( key == K_INS ) || ( key == K_KP_INS ) ) && trap_Key_IsDown( K_SHIFT ) ) {
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 139
EQI4 $99
ADDRLP4 4
INDIRI4
CNSTI4 170
NEI4 $97
LABELV $99
CNSTI4 138
ARGI4
ADDRLP4 8
ADDRGP4 trap_Key_IsDown
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $97
line 144
;144:		MField_Paste( edit );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MField_Paste
CALLV
pop
line 145
;145:		return;
ADDRGP4 $96
JUMPV
LABELV $97
line 148
;146:	}
;147:
;148:	len = strlen( edit->buffer );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
line 150
;149:
;150:	if ( key == K_DEL || key == K_KP_DEL ) {
ADDRLP4 16
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 140
EQI4 $102
ADDRLP4 16
INDIRI4
CNSTI4 171
NEI4 $100
LABELV $102
line 151
;151:		if ( edit->cursor < len ) {
ADDRFP4 0
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
GEI4 $96
line 152
;152:			memmove( edit->buffer + edit->cursor, 
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 20
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 28
ADDRLP4 24
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 154
;153:				edit->buffer + edit->cursor + 1, len - edit->cursor );
;154:		}
line 155
;155:		return;
ADDRGP4 $96
JUMPV
LABELV $100
line 158
;156:	}
;157:
;158:	if ( key == K_RIGHTARROW || key == K_KP_RIGHTARROW ) 
ADDRLP4 20
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 135
EQI4 $107
ADDRLP4 20
INDIRI4
CNSTI4 165
NEI4 $105
LABELV $107
line 159
;159:	{
line 160
;160:		if ( edit->cursor < len ) {
ADDRFP4 0
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
GEI4 $108
line 161
;161:			edit->cursor++;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 162
;162:		}
LABELV $108
line 163
;163:		if ( edit->cursor >= edit->scroll + edit->widthInChars && edit->cursor <= len )
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 24
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
LTI4 $96
ADDRLP4 28
INDIRI4
ADDRLP4 0
INDIRI4
GTI4 $96
line 164
;164:		{
line 165
;165:			edit->scroll++;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 166
;166:		}
line 167
;167:		return;
ADDRGP4 $96
JUMPV
LABELV $105
line 170
;168:	}
;169:
;170:	if ( key == K_LEFTARROW || key == K_KP_LEFTARROW ) 
ADDRLP4 24
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 134
EQI4 $114
ADDRLP4 24
INDIRI4
CNSTI4 163
NEI4 $112
LABELV $114
line 171
;171:	{
line 172
;172:		if ( edit->cursor > 0 ) {
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 0
LEI4 $115
line 173
;173:			edit->cursor--;
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 174
;174:		}
LABELV $115
line 175
;175:		if ( edit->cursor < edit->scroll )
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
GEI4 $96
line 176
;176:		{
line 177
;177:			edit->scroll--;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 178
;178:		}
line 179
;179:		return;
ADDRGP4 $96
JUMPV
LABELV $112
line 182
;180:	}
;181:
;182:	if ( key == K_HOME || key == K_KP_HOME || ( tolower(key) == 'a' && trap_Key_IsDown( K_CTRL ) ) ) {
ADDRLP4 28
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 143
EQI4 $122
ADDRLP4 28
INDIRI4
CNSTI4 160
EQI4 $122
ADDRLP4 28
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 tolower
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 97
NEI4 $119
CNSTI4 137
ARGI4
ADDRLP4 36
ADDRGP4 trap_Key_IsDown
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $119
LABELV $122
line 183
;183:		edit->cursor = 0;
ADDRFP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 184
;184:		edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 185
;185:		return;
ADDRGP4 $96
JUMPV
LABELV $119
line 188
;186:	}
;187:
;188:	if ( key == K_END || key == K_KP_END || ( tolower(key) == 'e' && trap_Key_IsDown( K_CTRL ) ) ) {
ADDRLP4 40
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 144
EQI4 $126
ADDRLP4 40
INDIRI4
CNSTI4 166
EQI4 $126
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 tolower
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 101
NEI4 $123
CNSTI4 137
ARGI4
ADDRLP4 48
ADDRGP4 trap_Key_IsDown
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $123
LABELV $126
line 189
;189:		edit->cursor = len;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ASGNI4
line 190
;190:		edit->scroll = len - edit->widthInChars + 1;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 191
;191:		if (edit->scroll < 0)
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 0
GEI4 $96
line 192
;192:			edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 193
;193:		return;
ADDRGP4 $96
JUMPV
LABELV $123
line 196
;194:	}
;195:
;196:	if ( key == K_INS || key == K_KP_INS ) {
ADDRLP4 52
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 139
EQI4 $131
ADDRLP4 52
INDIRI4
CNSTI4 170
NEI4 $129
LABELV $131
line 197
;197:		trap_Key_SetOverstrikeMode( !trap_Key_GetOverstrikeMode() );
ADDRLP4 60
ADDRGP4 trap_Key_GetOverstrikeMode
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
NEI4 $133
ADDRLP4 56
CNSTI4 1
ASGNI4
ADDRGP4 $134
JUMPV
LABELV $133
ADDRLP4 56
CNSTI4 0
ASGNI4
LABELV $134
ADDRLP4 56
INDIRI4
ARGI4
ADDRGP4 trap_Key_SetOverstrikeMode
CALLV
pop
line 198
;198:		return;
LABELV $129
line 200
;199:	}
;200:}
LABELV $96
endproc MField_KeyDownEvent 64 12
export MField_CharEvent
proc MField_CharEvent 36 12
line 207
;201:
;202:/*
;203:==================
;204:MField_CharEvent
;205:==================
;206:*/
;207:void MField_CharEvent( mfield_t *edit, int ch ) {
line 210
;208:	int		len;
;209:
;210:	if ( ch == 'v' - 'a' + 1 ) {	// ctrl-v is paste
ADDRFP4 4
INDIRI4
CNSTI4 22
NEI4 $136
line 211
;211:		MField_Paste( edit );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MField_Paste
CALLV
pop
line 212
;212:		return;
ADDRGP4 $135
JUMPV
LABELV $136
line 215
;213:	}
;214:
;215:	if ( ch == 'c' - 'a' + 1 ) {	// ctrl-c clears the field
ADDRFP4 4
INDIRI4
CNSTI4 3
NEI4 $138
line 216
;216:		MField_Clear( edit );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MField_Clear
CALLV
pop
line 217
;217:		return;
ADDRGP4 $135
JUMPV
LABELV $138
line 220
;218:	}
;219:
;220:	len = strlen( edit->buffer );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 222
;221:
;222:	if ( ch == 'h' - 'a' + 1 )	{	// ctrl-h is backspace
ADDRFP4 4
INDIRI4
CNSTI4 8
NEI4 $140
line 223
;223:		if ( edit->cursor > 0 ) {
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 0
LEI4 $135
line 224
;224:			memmove( edit->buffer + edit->cursor - 1, 
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 8
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 16
ADDRLP4 12
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 12
ADDP4
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 -1
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 12
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 226
;225:				edit->buffer + edit->cursor, len + 1 - edit->cursor );
;226:			edit->cursor--;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 227
;227:			if ( edit->cursor < edit->scroll )
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
GEI4 $135
line 228
;228:			{
line 229
;229:				edit->scroll--;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 230
;230:			}
line 231
;231:		}
line 232
;232:		return;
ADDRGP4 $135
JUMPV
LABELV $140
line 235
;233:	}
;234:
;235:	if ( ch == 'a' - 'a' + 1 ) {	// ctrl-a is home
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $146
line 236
;236:		edit->cursor = 0;
ADDRFP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 237
;237:		edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 238
;238:		return;
ADDRGP4 $135
JUMPV
LABELV $146
line 241
;239:	}
;240:
;241:	if ( ch == 'e' - 'a' + 1 ) {	// ctrl-e is end
ADDRFP4 4
INDIRI4
CNSTI4 5
NEI4 $148
line 242
;242:		edit->cursor = len;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ASGNI4
line 243
;243:		edit->scroll = edit->cursor - edit->widthInChars + 1;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 8
INDIRP4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 244
;244:		if (edit->scroll < 0)
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 0
GEI4 $135
line 245
;245:			edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 246
;246:		return;
ADDRGP4 $135
JUMPV
LABELV $148
line 252
;247:	}
;248:
;249:	//
;250:	// ignore any other non printable chars
;251:	//
;252:	if ( ch < 32 ) {
ADDRFP4 4
INDIRI4
CNSTI4 32
GEI4 $152
line 253
;253:		return;
ADDRGP4 $135
JUMPV
LABELV $152
line 256
;254:	}
;255:
;256:	if ( !trap_Key_GetOverstrikeMode() ) {	
ADDRLP4 8
ADDRGP4 trap_Key_GetOverstrikeMode
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $154
line 257
;257:		if ((edit->cursor == MAX_EDIT_LINE - 1) || (edit->maxchars && edit->cursor >= edit->maxchars))
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 255
EQI4 $158
ADDRLP4 20
ADDRLP4 12
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $155
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $155
LABELV $158
line 258
;258:			return;
ADDRGP4 $135
JUMPV
line 259
;259:	} else {
LABELV $154
line 261
;260:		// insert mode
;261:		if (( len == MAX_EDIT_LINE - 1 ) || (edit->maxchars && len >= edit->maxchars))
ADDRLP4 0
INDIRI4
CNSTI4 255
EQI4 $161
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $159
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $159
LABELV $161
line 262
;262:			return;
ADDRGP4 $135
JUMPV
LABELV $159
line 263
;263:		memmove( edit->buffer + edit->cursor + 1, edit->buffer + edit->cursor, len + 1 - edit->cursor );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 20
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 28
ADDRLP4 24
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 12
ADDP4
ADDP4
ASGNP4
ADDRLP4 32
CNSTI4 1
ASGNI4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ADDRLP4 32
INDIRI4
ADDI4
ADDRLP4 24
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 264
;264:	}
LABELV $155
line 266
;265:
;266:	edit->buffer[edit->cursor] = ch;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
ADDP4
ADDRFP4 4
INDIRI4
CVII1 4
ASGNI1
line 267
;267:	if (!edit->maxchars || edit->cursor < edit->maxchars-1)
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $164
ADDRLP4 16
INDIRP4
INDIRI4
ADDRLP4 20
INDIRI4
CNSTI4 1
SUBI4
GEI4 $162
LABELV $164
line 268
;268:		edit->cursor++;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $162
line 270
;269:
;270:	if ( edit->cursor >= edit->widthInChars )
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
LTI4 $165
line 271
;271:	{
line 272
;272:		edit->scroll++;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 273
;273:	}
LABELV $165
line 275
;274:
;275:	if ( edit->cursor == len + 1) {
ADDRFP4 0
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
NEI4 $167
line 276
;276:		edit->buffer[edit->cursor] = 0;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 12
ADDP4
ADDP4
CNSTI1 0
ASGNI1
line 277
;277:	}
LABELV $167
line 278
;278:}
LABELV $135
endproc MField_CharEvent 36 12
export MField_Clear
proc MField_Clear 0 0
line 285
;279:
;280:/*
;281:==================
;282:MField_Clear
;283:==================
;284:*/
;285:void MField_Clear( mfield_t *edit ) {
line 286
;286:	edit->buffer[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI1 0
ASGNI1
line 287
;287:	edit->cursor = 0;
ADDRFP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 288
;288:	edit->scroll = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 289
;289:}
LABELV $169
endproc MField_Clear 0 0
export MenuField_Init
proc MenuField_Init 32 4
line 296
;290:
;291:/*
;292:==================
;293:MenuField_Init
;294:==================
;295:*/
;296:void MenuField_Init( menufield_s* m ) {
line 301
;297:	int	l;
;298:	int	w;
;299:	int	h;
;300:
;301:	MField_Clear( &m->field );
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 MField_Clear
CALLV
pop
line 303
;302:
;303:	if (m->generic.flags & QMF_SMALLFONT)
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 2
BANDU4
CNSTU4 0
EQU4 $171
line 304
;304:	{
line 305
;305:		w = SMALLCHAR_WIDTH;
ADDRLP4 0
CNSTI4 8
ASGNI4
line 306
;306:		h = SMALLCHAR_HEIGHT;
ADDRLP4 8
CNSTI4 16
ASGNI4
line 307
;307:	}
ADDRGP4 $172
JUMPV
LABELV $171
line 309
;308:	else
;309:	{
line 310
;310:		w = BIGCHAR_WIDTH;
ADDRLP4 0
CNSTI4 16
ASGNI4
line 311
;311:		h = BIGCHAR_HEIGHT;
ADDRLP4 8
CNSTI4 16
ASGNI4
line 312
;312:	}	
LABELV $172
line 314
;313:
;314:	if (m->generic.name) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $173
line 315
;315:		l = (strlen( m->generic.name )+1) * w;		
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
line 316
;316:	}
ADDRGP4 $174
JUMPV
LABELV $173
line 317
;317:	else {
line 318
;318:		l = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 319
;319:	}
LABELV $174
line 321
;320:
;321:	m->generic.left   = m->generic.x - l;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 322
;322:	m->generic.top    = m->generic.y;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 323
;323:	m->generic.right  = m->generic.x + w + m->field.widthInChars*w;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ADDRLP4 20
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
MULI4
ADDI4
ASGNI4
line 324
;324:	m->generic.bottom = m->generic.y + h;
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 28
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 325
;325:}
LABELV $170
endproc MenuField_Init 32 4
export MenuField_Draw
proc MenuField_Draw 52 20
line 333
;326:
;327:/*
;328:==================
;329:MenuField_Draw
;330:==================
;331:*/
;332:void MenuField_Draw( menufield_s *f )
;333:{
line 342
;334:	int		x;
;335:	int		y;
;336:	int		w;
;337:	int		h;
;338:	int		style;
;339:	qboolean focus;
;340:	float	*color;
;341:
;342:	x =	f->generic.x;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 343
;343:	y =	f->generic.y;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 345
;344:
;345:	if (f->generic.flags & QMF_SMALLFONT)
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 2
BANDU4
CNSTU4 0
EQU4 $176
line 346
;346:	{
line 347
;347:		w = SMALLCHAR_WIDTH;
ADDRLP4 16
CNSTI4 8
ASGNI4
line 348
;348:		h = SMALLCHAR_HEIGHT;
ADDRLP4 24
CNSTI4 16
ASGNI4
line 349
;349:		style = UI_SMALLFONT;
ADDRLP4 0
CNSTI4 16
ASGNI4
line 350
;350:	}
ADDRGP4 $177
JUMPV
LABELV $176
line 352
;351:	else
;352:	{
line 353
;353:		w = BIGCHAR_WIDTH;
ADDRLP4 16
CNSTI4 16
ASGNI4
line 354
;354:		h = BIGCHAR_HEIGHT;
ADDRLP4 24
CNSTI4 16
ASGNI4
line 355
;355:		style = UI_BIGFONT;
ADDRLP4 0
CNSTI4 32
ASGNI4
line 356
;356:	}	
LABELV $177
line 358
;357:
;358:	if (Menu_ItemAtCursor( f->generic.parent ) == f) {
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 Menu_ItemAtCursor
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRP4
CVPU4 4
NEU4 $178
line 359
;359:		focus = qtrue;
ADDRLP4 20
CNSTI4 1
ASGNI4
line 360
;360:		style |= UI_PULSE;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 361
;361:	}
ADDRGP4 $179
JUMPV
LABELV $178
line 362
;362:	else {
line 363
;363:		focus = qfalse;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 364
;364:	}
LABELV $179
line 366
;365:
;366:	if (f->generic.flags & QMF_GRAYED)
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 8192
BANDU4
CNSTU4 0
EQU4 $180
line 367
;367:		color = text_color_disabled;
ADDRLP4 12
ADDRGP4 text_color_disabled
ASGNP4
ADDRGP4 $181
JUMPV
LABELV $180
line 368
;368:	else if (focus)
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $182
line 369
;369:		color = text_color_highlight;
ADDRLP4 12
ADDRGP4 text_color_highlight
ASGNP4
ADDRGP4 $183
JUMPV
LABELV $182
line 371
;370:	else
;371:		color = text_color_normal;
ADDRLP4 12
ADDRGP4 text_color_normal
ASGNP4
LABELV $183
LABELV $181
line 373
;372:
;373:	if ( focus )
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $184
line 374
;374:	{
line 376
;375:		// draw cursor
;376:		UI_FillRect( f->generic.left, f->generic.top, f->generic.right-f->generic.left+1, f->generic.bottom-f->generic.top+1, listbar_color ); 
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
ADDRLP4 36
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 44
ADDRLP4 36
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 48
CNSTI4 1
ASGNI4
ADDRLP4 36
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
SUBI4
ADDRLP4 48
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 36
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
SUBI4
ADDRLP4 48
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 listbar_color
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 377
;377:		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|style, color);
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
CNSTI4 13
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 4097
BORI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 378
;378:	}
LABELV $184
line 380
;379:
;380:	if ( f->generic.name ) {
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $186
line 381
;381:		UI_DrawString( x - w, y, f->generic.name, style|UI_RIGHT, color );
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
SUBI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
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
line 382
;382:	}
LABELV $186
line 384
;383:
;384:	MField_Draw( &f->field, x + w, y, style, color );
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
ADDI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 MField_Draw
CALLV
pop
line 385
;385:}
LABELV $175
endproc MenuField_Draw 52 20
export MenuField_Key
proc MenuField_Key 44 8
line 393
;386:
;387:/*
;388:==================
;389:MenuField_Key
;390:==================
;391:*/
;392:sfxHandle_t MenuField_Key( menufield_s* m, int* key )
;393:{
line 396
;394:	int keycode;
;395:
;396:	keycode = *key;
ADDRLP4 0
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
line 398
;397:
;398:	switch ( keycode )
ADDRLP4 0
INDIRI4
CNSTI4 132
EQI4 $190
ADDRLP4 8
CNSTI4 133
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $190
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
GTI4 $202
LABELV $201
ADDRLP4 16
CNSTI4 9
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $190
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $189
LABELV $203
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $191
ADDRGP4 $189
JUMPV
LABELV $202
ADDRLP4 0
INDIRI4
CNSTI4 167
EQI4 $190
ADDRLP4 24
CNSTI4 169
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $191
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRI4
GTI4 $205
LABELV $204
ADDRLP4 0
INDIRI4
CNSTI4 161
EQI4 $190
ADDRGP4 $189
JUMPV
LABELV $205
ADDRLP4 0
INDIRI4
CNSTI4 185
LTI4 $189
ADDRLP4 0
INDIRI4
CNSTI4 188
GTI4 $189
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $206-740
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $206
address $191
address $191
address $191
address $191
code
line 399
;399:	{
LABELV $191
line 407
;400:		case K_KP_ENTER:
;401:		case K_ENTER:
;402:		case K_JOY1:
;403:		case K_JOY2:
;404:		case K_JOY3:
;405:		case K_JOY4:
;406:			// have enter go to next cursor point
;407:			*key = K_TAB;
ADDRFP4 4
INDIRP4
CNSTI4 9
ASGNI4
line 408
;408:			break;
ADDRGP4 $190
JUMPV
line 415
;409:
;410:		case K_TAB:
;411:		case K_KP_DOWNARROW:
;412:		case K_DOWNARROW:
;413:		case K_KP_UPARROW:
;414:		case K_UPARROW:
;415:			break;
LABELV $189
line 418
;416:
;417:		default:
;418:			if ( keycode & K_CHAR_FLAG )
ADDRLP4 0
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $193
line 419
;419:			{
line 420
;420:				keycode &= ~K_CHAR_FLAG;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 -1025
BANDI4
ASGNI4
line 422
;421:
;422:				if ((m->generic.flags & QMF_UPPERCASE) && Q_islower( keycode ))
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 524288
BANDU4
CNSTU4 0
EQU4 $195
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 Q_islower
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $195
line 423
;423:					keycode -= 'a' - 'A';
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
ADDRGP4 $196
JUMPV
LABELV $195
line 424
;424:				else if ((m->generic.flags & QMF_LOWERCASE) && Q_isupper( keycode ))
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 262144
BANDU4
CNSTU4 0
EQU4 $197
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 36
ADDRGP4 Q_isupper
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $197
line 425
;425:					keycode -= 'A' - 'a';
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 -32
SUBI4
ASGNI4
ADDRGP4 $198
JUMPV
LABELV $197
line 426
;426:				else if ((m->generic.flags & QMF_NUMBERSONLY) && Q_isalpha( keycode ))
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 32
BANDU4
CNSTU4 0
EQU4 $199
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 Q_isalpha
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $199
line 427
;427:					return (menu_buzz_sound);
ADDRGP4 menu_buzz_sound
INDIRI4
RETI4
ADDRGP4 $188
JUMPV
LABELV $199
LABELV $198
LABELV $196
line 429
;428:
;429:				MField_CharEvent( &m->field, keycode);
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 MField_CharEvent
CALLV
pop
line 430
;430:			}
ADDRGP4 $190
JUMPV
LABELV $193
line 432
;431:			else
;432:				MField_KeyDownEvent( &m->field, keycode );
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 MField_KeyDownEvent
CALLV
pop
line 433
;433:			break;
LABELV $190
line 436
;434:	}
;435:
;436:	return (0);
CNSTI4 0
RETI4
LABELV $188
endproc MenuField_Key 44 8
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
import UI_MainMenu
import MainMenu_Cache
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
LABELV $78
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 76
byte 1 101
byte 1 110
byte 1 32
byte 1 62
byte 1 61
byte 1 32
byte 1 77
byte 1 65
byte 1 88
byte 1 95
byte 1 83
byte 1 84
byte 1 82
byte 1 73
byte 1 78
byte 1 71
byte 1 95
byte 1 67
byte 1 72
byte 1 65
byte 1 82
byte 1 83
byte 1 0
