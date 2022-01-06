data
align 4
LABELV driverinfo_artlist
address $67
address $68
address $69
address $70
byte 4 0
code
proc DriverInfo_Event 4 0
file "../ui_video.c"
line 72
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
;25:void GraphicsOptions_MenuInit( void );
;26:
;27:/*
;28:=======================================================================
;29:
;30:DRIVER INFORMATION MENU
;31:
;32:=======================================================================
;33:*/
;34:
;35:
;36:#define DRIVERINFO_FRAMEL	"menu/art/frame2_l"
;37:#define DRIVERINFO_FRAMER	"menu/art/frame1_r"
;38:#define DRIVERINFO_BACK0	"menu/art/back_0"
;39:#define DRIVERINFO_BACK1	"menu/art/back_1"
;40:
;41:static char* driverinfo_artlist[] = 
;42:{
;43:	DRIVERINFO_FRAMEL,
;44:	DRIVERINFO_FRAMER,
;45:	DRIVERINFO_BACK0,
;46:	DRIVERINFO_BACK1,
;47:	NULL,
;48:};
;49:
;50:#define ID_DRIVERINFOBACK	100
;51:
;52:typedef struct
;53:{
;54:	menuframework_s	menu;
;55:	menutext_s		banner;
;56:	menubitmap_s	back;
;57:	menubitmap_s	framel;
;58:	menubitmap_s	framer;
;59:	char			stringbuff[1024];
;60:	char*			strings[64];
;61:	int				numstrings;
;62:} driverinfo_t;
;63:
;64:static driverinfo_t	s_driverinfo;
;65:
;66:/*
;67:=================
;68:DriverInfo_Event
;69:=================
;70:*/
;71:static void DriverInfo_Event( void* ptr, int event )
;72:{
line 73
;73:	if (event != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $73
line 74
;74:		return;
ADDRGP4 $72
JUMPV
LABELV $73
line 76
;75:
;76:	switch (((menucommon_s*)ptr)->id)
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 100
EQI4 $78
ADDRGP4 $75
JUMPV
line 77
;77:	{
LABELV $78
line 79
;78:		case ID_DRIVERINFOBACK:
;79:			UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 80
;80:			break;
LABELV $75
LABELV $76
line 82
;81:	}
;82:}
LABELV $72
endproc DriverInfo_Event 4 0
proc DriverInfo_MenuDraw 12 20
line 90
;83:
;84:/*
;85:=================
;86:DriverInfo_MenuDraw
;87:=================
;88:*/
;89:static void DriverInfo_MenuDraw( void )
;90:{
line 94
;91:	int	i;
;92:	int	y;
;93:
;94:	Menu_Draw( &s_driverinfo.menu );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 96
;95:
;96:	UI_DrawString( 320, 80, "VENDOR", UI_CENTER|UI_SMALLFONT, color_red );
CNSTI4 320
ARGI4
CNSTI4 80
ARGI4
ADDRGP4 $80
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 97
;97:	UI_DrawString( 320, 152, "PIXELFORMAT", UI_CENTER|UI_SMALLFONT, color_red );
CNSTI4 320
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 $81
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 98
;98:	UI_DrawString( 320, 192, "EXTENSIONS", UI_CENTER|UI_SMALLFONT, color_red );
CNSTI4 320
ARGI4
CNSTI4 192
ARGI4
ADDRGP4 $82
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 100
;99:
;100:	UI_DrawString( 320, 80+16, uis.glconfig.vendor_string, UI_CENTER|UI_SMALLFONT, text_color_normal );
CNSTI4 320
ARGI4
CNSTI4 96
ARGI4
ADDRGP4 uis+56+1024
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 101
;101:	UI_DrawString( 320, 96+16, uis.glconfig.version_string, UI_CENTER|UI_SMALLFONT, text_color_normal );
CNSTI4 320
ARGI4
CNSTI4 112
ARGI4
ADDRGP4 uis+56+2048
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 102
;102:	UI_DrawString( 320, 112+16, uis.glconfig.renderer_string, UI_CENTER|UI_SMALLFONT, text_color_normal );
CNSTI4 320
ARGI4
CNSTI4 128
ARGI4
ADDRGP4 uis+56
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 103
;103:	UI_DrawString( 320, 152+16, va ("color(%d-bits) Z(%d-bits) stencil(%d-bits)", uis.glconfig.colorBits, uis.glconfig.depthBits, uis.glconfig.stencilBits), UI_CENTER|UI_SMALLFONT, text_color_normal );
ADDRGP4 $88
ARGP4
ADDRGP4 uis+56+11272
INDIRI4
ARGI4
ADDRGP4 uis+56+11276
INDIRI4
ARGI4
ADDRGP4 uis+56+11280
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 320
ARGI4
CNSTI4 168
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 106
;104:
;105:	// double column
;106:	y = 192+16;
ADDRLP4 4
CNSTI4 208
ASGNI4
line 107
;107:	for (i=0; i<s_driverinfo.numstrings/2; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $98
JUMPV
LABELV $95
line 108
;108:		UI_DrawString( 320-4, y, s_driverinfo.strings[i*2], UI_RIGHT|UI_SMALLFONT, text_color_normal );
CNSTI4 316
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 1
LSHI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648
ADDP4
INDIRP4
ARGP4
CNSTI4 18
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 109
;109:		UI_DrawString( 320+4, y, s_driverinfo.strings[i*2+1], UI_LEFT|UI_SMALLFONT, text_color_normal );
CNSTI4 324
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 1
LSHI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648+4
ADDP4
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 110
;110:		y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 111
;111:	}
LABELV $96
line 107
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $98
ADDRLP4 0
INDIRI4
ADDRGP4 s_driverinfo+1904
INDIRI4
CNSTI4 2
DIVI4
LTI4 $95
line 113
;112:
;113:	if (s_driverinfo.numstrings & 1)
ADDRGP4 s_driverinfo+1904
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $103
line 114
;114:		UI_DrawString( 320, y, s_driverinfo.strings[s_driverinfo.numstrings-1], UI_CENTER|UI_SMALLFONT, text_color_normal );
CNSTI4 320
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 s_driverinfo+1904
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648-4
ADDP4
INDIRP4
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
LABELV $103
line 115
;115:}
LABELV $79
endproc DriverInfo_MenuDraw 12 20
export DriverInfo_Cache
proc DriverInfo_Cache 4 4
line 123
;116:
;117:/*
;118:=================
;119:DriverInfo_Cache
;120:=================
;121:*/
;122:void DriverInfo_Cache( void )
;123:{
line 127
;124:	int	i;
;125:
;126:	// touch all our pics
;127:	for (i=0; ;i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $110
line 128
;128:	{
line 129
;129:		if (!driverinfo_artlist[i])
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 driverinfo_artlist
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $114
line 130
;130:			break;
ADDRGP4 $112
JUMPV
LABELV $114
line 131
;131:		trap_R_RegisterShaderNoMip(driverinfo_artlist[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 driverinfo_artlist
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 132
;132:	}
LABELV $111
line 127
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $110
JUMPV
LABELV $112
line 133
;133:}
LABELV $109
endproc DriverInfo_Cache 4 4
proc UI_DriverInfo_Menu 36 12
line 141
;134:
;135:/*
;136:=================
;137:UI_DriverInfo_Menu
;138:=================
;139:*/
;140:static void UI_DriverInfo_Menu( void )
;141:{
line 147
;142:	char*	eptr;
;143:	int		i;
;144:	int		len;
;145:
;146:	// zero set all our globals
;147:	memset( &s_driverinfo, 0 ,sizeof(driverinfo_t) );
ADDRGP4 s_driverinfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1908
ARGI4
ADDRGP4 memset
CALLP4
pop
line 149
;148:
;149:	DriverInfo_Cache();
ADDRGP4 DriverInfo_Cache
CALLV
pop
line 151
;150:
;151:	s_driverinfo.menu.fullscreen = qtrue;
ADDRGP4 s_driverinfo+280
CNSTI4 1
ASGNI4
line 152
;152:	s_driverinfo.menu.draw       = DriverInfo_MenuDraw;
ADDRGP4 s_driverinfo+268
ADDRGP4 DriverInfo_MenuDraw
ASGNP4
line 154
;153:
;154:	s_driverinfo.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_driverinfo+288
CNSTI4 10
ASGNI4
line 155
;155:	s_driverinfo.banner.generic.x	  = 320;
ADDRGP4 s_driverinfo+288+12
CNSTI4 320
ASGNI4
line 156
;156:	s_driverinfo.banner.generic.y	  = 16;
ADDRGP4 s_driverinfo+288+16
CNSTI4 16
ASGNI4
line 157
;157:	s_driverinfo.banner.string		  = "DRIVER INFO";
ADDRGP4 s_driverinfo+288+60
ADDRGP4 $126
ASGNP4
line 158
;158:	s_driverinfo.banner.color	      = color_white;
ADDRGP4 s_driverinfo+288+68
ADDRGP4 color_white
ASGNP4
line 159
;159:	s_driverinfo.banner.style	      = UI_CENTER;
ADDRGP4 s_driverinfo+288+64
CNSTI4 1
ASGNI4
line 161
;160:
;161:	s_driverinfo.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_driverinfo+448
CNSTI4 6
ASGNI4
line 162
;162:	s_driverinfo.framel.generic.name  = DRIVERINFO_FRAMEL;
ADDRGP4 s_driverinfo+448+4
ADDRGP4 $67
ASGNP4
line 163
;163:	s_driverinfo.framel.generic.flags = QMF_INACTIVE;
ADDRGP4 s_driverinfo+448+44
CNSTU4 16384
ASGNU4
line 164
;164:	s_driverinfo.framel.generic.x	  = 0;
ADDRGP4 s_driverinfo+448+12
CNSTI4 0
ASGNI4
line 165
;165:	s_driverinfo.framel.generic.y	  = 78;
ADDRGP4 s_driverinfo+448+16
CNSTI4 78
ASGNI4
line 166
;166:	s_driverinfo.framel.width  	      = 256;
ADDRGP4 s_driverinfo+448+76
CNSTI4 256
ASGNI4
line 167
;167:	s_driverinfo.framel.height  	  = 329;
ADDRGP4 s_driverinfo+448+80
CNSTI4 329
ASGNI4
line 169
;168:
;169:	s_driverinfo.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_driverinfo+536
CNSTI4 6
ASGNI4
line 170
;170:	s_driverinfo.framer.generic.name  = DRIVERINFO_FRAMER;
ADDRGP4 s_driverinfo+536+4
ADDRGP4 $68
ASGNP4
line 171
;171:	s_driverinfo.framer.generic.flags = QMF_INACTIVE;
ADDRGP4 s_driverinfo+536+44
CNSTU4 16384
ASGNU4
line 172
;172:	s_driverinfo.framer.generic.x	  = 376;
ADDRGP4 s_driverinfo+536+12
CNSTI4 376
ASGNI4
line 173
;173:	s_driverinfo.framer.generic.y	  = 76;
ADDRGP4 s_driverinfo+536+16
CNSTI4 76
ASGNI4
line 174
;174:	s_driverinfo.framer.width  	      = 256;
ADDRGP4 s_driverinfo+536+76
CNSTI4 256
ASGNI4
line 175
;175:	s_driverinfo.framer.height  	  = 334;
ADDRGP4 s_driverinfo+536+80
CNSTI4 334
ASGNI4
line 177
;176:
;177:	s_driverinfo.back.generic.type	   = MTYPE_BITMAP;
ADDRGP4 s_driverinfo+360
CNSTI4 6
ASGNI4
line 178
;178:	s_driverinfo.back.generic.name     = DRIVERINFO_BACK0;
ADDRGP4 s_driverinfo+360+4
ADDRGP4 $69
ASGNP4
line 179
;179:	s_driverinfo.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_driverinfo+360+44
CNSTU4 260
ASGNU4
line 180
;180:	s_driverinfo.back.generic.callback = DriverInfo_Event;
ADDRGP4 s_driverinfo+360+48
ADDRGP4 DriverInfo_Event
ASGNP4
line 181
;181:	s_driverinfo.back.generic.id	   = ID_DRIVERINFOBACK;
ADDRGP4 s_driverinfo+360+8
CNSTI4 100
ASGNI4
line 182
;182:	s_driverinfo.back.generic.x		   = 0;
ADDRGP4 s_driverinfo+360+12
CNSTI4 0
ASGNI4
line 183
;183:	s_driverinfo.back.generic.y		   = 480-64;
ADDRGP4 s_driverinfo+360+16
CNSTI4 416
ASGNI4
line 184
;184:	s_driverinfo.back.width  		   = 128;
ADDRGP4 s_driverinfo+360+76
CNSTI4 128
ASGNI4
line 185
;185:	s_driverinfo.back.height  		   = 64;
ADDRGP4 s_driverinfo+360+80
CNSTI4 64
ASGNI4
line 186
;186:	s_driverinfo.back.focuspic         = DRIVERINFO_BACK1;
ADDRGP4 s_driverinfo+360+60
ADDRGP4 $70
ASGNP4
line 192
;187:
;188:  // TTimo: overflow with particularly long GL extensions (such as the gf3)
;189:  // https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=399
;190:  // NOTE: could have pushed the size of stringbuff, but the list is already out of the screen
;191:  // (no matter what your resolution)
;192:  Q_strncpyz(s_driverinfo.stringbuff, uis.glconfig.extensions_string, 1024);
ADDRGP4 s_driverinfo+624
ARGP4
ADDRGP4 uis+56+3072
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 195
;193:
;194:	// build null terminated extension strings
;195:	eptr = s_driverinfo.stringbuff;
ADDRLP4 0
ADDRGP4 s_driverinfo+624
ASGNP4
ADDRGP4 $181
JUMPV
line 197
;196:	while ( s_driverinfo.numstrings<40 && *eptr )
;197:	{
LABELV $184
line 199
;198:		while ( *eptr && *eptr == ' ' )
;199:			*eptr++ = '\0';
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI1 0
ASGNI1
LABELV $185
line 198
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $187
ADDRLP4 16
INDIRI4
CNSTI4 32
EQI4 $184
LABELV $187
line 202
;200:
;201:		// track start of valid string
;202:		if (*eptr && *eptr != ' ')
ADDRLP4 20
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $193
ADDRLP4 20
INDIRI4
CNSTI4 32
EQI4 $193
line 203
;203:			s_driverinfo.strings[s_driverinfo.numstrings++] = eptr;
ADDRLP4 28
ADDRGP4 s_driverinfo+1904
ASGNP4
ADDRLP4 24
ADDRLP4 28
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRP4
ADDRLP4 24
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $193
JUMPV
LABELV $192
line 206
;204:
;205:		while ( *eptr && *eptr != ' ' )
;206:			eptr++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $193
line 205
ADDRLP4 32
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $195
ADDRLP4 32
INDIRI4
CNSTI4 32
NEI4 $192
LABELV $195
line 207
;207:	}
LABELV $181
line 196
ADDRGP4 s_driverinfo+1904
INDIRI4
CNSTI4 40
GEI4 $196
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $185
LABELV $196
line 210
;208:
;209:	// safety length strings for display
;210:	for (i=0; i<s_driverinfo.numstrings; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $200
JUMPV
LABELV $197
line 211
;211:		len = strlen(s_driverinfo.strings[i]);
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 212
;212:		if (len > 32) {
ADDRLP4 8
INDIRI4
CNSTI4 32
LEI4 $203
line 213
;213:			s_driverinfo.strings[i][len-1] = '>';
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648
ADDP4
INDIRP4
ADDP4
CNSTI1 62
ASGNI1
line 214
;214:			s_driverinfo.strings[i][len]   = '\0';
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_driverinfo+1648
ADDP4
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 215
;215:		}
LABELV $203
line 216
;216:	}
LABELV $198
line 210
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $200
ADDRLP4 4
INDIRI4
ADDRGP4 s_driverinfo+1904
INDIRI4
LTI4 $197
line 218
;217:
;218:	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.banner );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 s_driverinfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 219
;219:	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.framel );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 s_driverinfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 220
;220:	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.framer );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 s_driverinfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 221
;221:	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.back );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 s_driverinfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 223
;222:
;223:	UI_PushMenu( &s_driverinfo.menu );
ADDRGP4 s_driverinfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 224
;224:}
LABELV $116
endproc UI_DriverInfo_Menu 36 12
data
align 4
LABELV s_drivers
address $211
address $212
byte 4 0
align 4
LABELV s_ivo_templates
byte 4 4
byte 4 1
byte 4 2
byte 4 0
byte 4 2
byte 4 2
byte 4 1
byte 4 1
byte 4 0
byte 4 1
byte 4 3
byte 4 1
byte 4 2
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 0
byte 4 0
byte 4 1
byte 4 2
byte 4 1
byte 4 1
byte 4 0
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 2
byte 4 1
byte 4 1
byte 4 1
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 3
byte 4 1
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 0
byte 4 0
byte 4 1
code
proc GraphicsOptions_GetInitialVideo 0 0
line 331
;225:
;226:/*
;227:=======================================================================
;228:
;229:GRAPHICS OPTIONS MENU
;230:
;231:=======================================================================
;232:*/
;233:
;234:#define GRAPHICSOPTIONS_FRAMEL	"menu/art/frame2_l"
;235:#define GRAPHICSOPTIONS_FRAMER	"menu/art/frame1_r"
;236:#define GRAPHICSOPTIONS_BACK0	"menu/art/back_0"
;237:#define GRAPHICSOPTIONS_BACK1	"menu/art/back_1"
;238:#define GRAPHICSOPTIONS_ACCEPT0	"menu/art/accept_0"
;239:#define GRAPHICSOPTIONS_ACCEPT1	"menu/art/accept_1"
;240:
;241:static const char *s_drivers[] =
;242:{
;243:	OPENGL_DRIVER_NAME,
;244:	_3DFX_DRIVER_NAME,
;245:	0
;246:};
;247:
;248:#define ID_BACK2		101
;249:#define ID_FULLSCREEN	102
;250:#define ID_LIST			103
;251:#define ID_MODE			104
;252:#define ID_DRIVERINFO	105
;253:#define ID_GRAPHICS		106
;254:#define ID_DISPLAY		107
;255:#define ID_SOUND		108
;256:#define ID_NETWORK		109
;257:
;258:typedef struct {
;259:	menuframework_s	menu;
;260:
;261:	menutext_s		banner;
;262:	menubitmap_s	framel;
;263:	menubitmap_s	framer;
;264:
;265:	menutext_s		graphics;
;266:	menutext_s		display;
;267:	menutext_s		sound;
;268:	menutext_s		network;
;269:
;270:	menulist_s		list;
;271:	menulist_s		mode;
;272:	menulist_s		driver;
;273:	menuslider_s	tq;
;274:	menulist_s  	fs;
;275:	menulist_s  	lighting;
;276:	menulist_s  	allow_extensions;
;277:	menulist_s  	texturebits;
;278:	menulist_s  	colordepth;
;279:	menulist_s  	geometry;
;280:	menulist_s  	filter;
;281:	menutext_s		driverinfo;
;282:
;283:	menubitmap_s	apply;
;284:	menubitmap_s	back;
;285:} graphicsoptions_t;
;286:
;287:typedef struct
;288:{
;289:	int mode;
;290:	qboolean fullscreen;
;291:	int tq;
;292:	int lighting;
;293:	int colordepth;
;294:	int texturebits;
;295:	int geometry;
;296:	int filter;
;297:	int driver;
;298:	qboolean extensions;
;299:} InitialVideoOptions_s;
;300:
;301:static InitialVideoOptions_s	s_ivo;
;302:static graphicsoptions_t		s_graphicsoptions;	
;303:
;304:static InitialVideoOptions_s s_ivo_templates[] =
;305:{
;306:	{
;307:		4, qtrue, 2, 0, 2, 2, 1, 1, 0, qtrue	// JDC: this was tq 3
;308:	},
;309:	{
;310:		3, qtrue, 2, 0, 0, 0, 1, 0, 0, qtrue
;311:	},
;312:	{
;313:		2, qtrue, 1, 0, 1, 0, 0, 0, 0, qtrue
;314:	},
;315:	{
;316:		2, qtrue, 1, 1, 1, 0, 0, 0, 0, qtrue
;317:	},
;318:	{
;319:		3, qtrue, 1, 0, 0, 0, 1, 0, 0, qtrue
;320:	}
;321:};
;322:
;323:#define NUM_IVO_TEMPLATES ( sizeof( s_ivo_templates ) / sizeof( s_ivo_templates[0] ) )
;324:
;325:/*
;326:=================
;327:GraphicsOptions_GetInitialVideo
;328:=================
;329:*/
;330:static void GraphicsOptions_GetInitialVideo( void )
;331:{
line 332
;332:	s_ivo.colordepth  = s_graphicsoptions.colordepth.curvalue;
ADDRGP4 s_ivo+16
ADDRGP4 s_graphicsoptions+1572+64
INDIRI4
ASGNI4
line 333
;333:	s_ivo.driver      = s_graphicsoptions.driver.curvalue;
ADDRGP4 s_ivo+32
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
ASGNI4
line 334
;334:	s_ivo.mode        = s_graphicsoptions.mode.curvalue;
ADDRGP4 s_ivo
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
ASGNI4
line 335
;335:	s_ivo.fullscreen  = s_graphicsoptions.fs.curvalue;
ADDRGP4 s_ivo+4
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
ASGNI4
line 336
;336:	s_ivo.extensions  = s_graphicsoptions.allow_extensions.curvalue;
ADDRGP4 s_ivo+36
ADDRGP4 s_graphicsoptions+1380+64
INDIRI4
ASGNI4
line 337
;337:	s_ivo.tq          = s_graphicsoptions.tq.curvalue;
ADDRGP4 s_ivo+8
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
CVFI4 4
ASGNI4
line 338
;338:	s_ivo.lighting    = s_graphicsoptions.lighting.curvalue;
ADDRGP4 s_ivo+12
ADDRGP4 s_graphicsoptions+1284+64
INDIRI4
ASGNI4
line 339
;339:	s_ivo.geometry    = s_graphicsoptions.geometry.curvalue;
ADDRGP4 s_ivo+24
ADDRGP4 s_graphicsoptions+1668+64
INDIRI4
ASGNI4
line 340
;340:	s_ivo.filter      = s_graphicsoptions.filter.curvalue;
ADDRGP4 s_ivo+28
ADDRGP4 s_graphicsoptions+1764+64
INDIRI4
ASGNI4
line 341
;341:	s_ivo.texturebits = s_graphicsoptions.texturebits.curvalue;
ADDRGP4 s_ivo+20
ADDRGP4 s_graphicsoptions+1476+64
INDIRI4
ASGNI4
line 342
;342:}
LABELV $215
endproc GraphicsOptions_GetInitialVideo 0 0
proc GraphicsOptions_CheckConfig 4 0
line 350
;343:
;344:/*
;345:=================
;346:GraphicsOptions_CheckConfig
;347:=================
;348:*/
;349:static void GraphicsOptions_CheckConfig( void )
;350:{
line 353
;351:	int i;
;352:
;353:	for ( i = 0; i < NUM_IVO_TEMPLATES; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $249
JUMPV
LABELV $246
line 354
;354:	{
line 355
;355:		if ( s_ivo_templates[i].colordepth != s_graphicsoptions.colordepth.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+16
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1572+64
INDIRI4
EQI4 $250
line 356
;356:			continue;
ADDRGP4 $247
JUMPV
LABELV $250
line 357
;357:		if ( s_ivo_templates[i].driver != s_graphicsoptions.driver.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+32
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
EQI4 $255
line 358
;358:			continue;
ADDRGP4 $247
JUMPV
LABELV $255
line 359
;359:		if ( s_ivo_templates[i].mode != s_graphicsoptions.mode.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
EQI4 $260
line 360
;360:			continue;
ADDRGP4 $247
JUMPV
LABELV $260
line 361
;361:		if ( s_ivo_templates[i].fullscreen != s_graphicsoptions.fs.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+4
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
EQI4 $264
line 362
;362:			continue;
ADDRGP4 $247
JUMPV
LABELV $264
line 363
;363:		if ( s_ivo_templates[i].tq != s_graphicsoptions.tq.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+8
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
EQF4 $269
line 364
;364:			continue;
ADDRGP4 $247
JUMPV
LABELV $269
line 365
;365:		if ( s_ivo_templates[i].lighting != s_graphicsoptions.lighting.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+12
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1284+64
INDIRI4
EQI4 $274
line 366
;366:			continue;
ADDRGP4 $247
JUMPV
LABELV $274
line 367
;367:		if ( s_ivo_templates[i].geometry != s_graphicsoptions.geometry.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+24
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1668+64
INDIRI4
EQI4 $279
line 368
;368:			continue;
ADDRGP4 $247
JUMPV
LABELV $279
line 369
;369:		if ( s_ivo_templates[i].filter != s_graphicsoptions.filter.curvalue )
CNSTI4 40
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_ivo_templates+28
ADDP4
INDIRI4
ADDRGP4 s_graphicsoptions+1764+64
INDIRI4
EQI4 $284
line 370
;370:			continue;
ADDRGP4 $247
JUMPV
LABELV $284
line 373
;371://		if ( s_ivo_templates[i].texturebits != s_graphicsoptions.texturebits.curvalue )
;372://			continue;
;373:		s_graphicsoptions.list.curvalue = i;
ADDRGP4 s_graphicsoptions+824+64
ADDRLP4 0
INDIRI4
ASGNI4
line 374
;374:		return;
ADDRGP4 $245
JUMPV
LABELV $247
line 353
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $249
ADDRLP4 0
INDIRI4
CVIU4 4
CNSTU4 5
LTU4 $246
line 376
;375:	}
;376:	s_graphicsoptions.list.curvalue = 4;
ADDRGP4 s_graphicsoptions+824+64
CNSTI4 4
ASGNI4
line 377
;377:}
LABELV $245
endproc GraphicsOptions_CheckConfig 4 0
proc GraphicsOptions_UpdateMenuItems 8 0
line 385
;378:
;379:/*
;380:=================
;381:GraphicsOptions_UpdateMenuItems
;382:=================
;383:*/
;384:static void GraphicsOptions_UpdateMenuItems( void )
;385:{
line 386
;386:	if ( s_graphicsoptions.driver.curvalue == 1 )
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
CNSTI4 1
NEI4 $294
line 387
;387:	{
line 388
;388:		s_graphicsoptions.fs.curvalue = 1;
ADDRGP4 s_graphicsoptions+1188+64
CNSTI4 1
ASGNI4
line 389
;389:		s_graphicsoptions.fs.generic.flags |= QMF_GRAYED;
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1188+44
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 390
;390:		s_graphicsoptions.colordepth.curvalue = 1;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 1
ASGNI4
line 391
;391:	}
ADDRGP4 $295
JUMPV
LABELV $294
line 393
;392:	else
;393:	{
line 394
;394:		s_graphicsoptions.fs.generic.flags &= ~QMF_GRAYED;
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1188+44
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 395
;395:	}
LABELV $295
line 397
;396:
;397:	if ( s_graphicsoptions.fs.curvalue == 0 || s_graphicsoptions.driver.curvalue == 1 )
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
CNSTI4 0
EQI4 $312
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
CNSTI4 1
NEI4 $306
LABELV $312
line 398
;398:	{
line 399
;399:		s_graphicsoptions.colordepth.curvalue = 0;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 0
ASGNI4
line 400
;400:		s_graphicsoptions.colordepth.generic.flags |= QMF_GRAYED;
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1572+44
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 401
;401:	}
ADDRGP4 $307
JUMPV
LABELV $306
line 403
;402:	else
;403:	{
line 404
;404:		s_graphicsoptions.colordepth.generic.flags &= ~QMF_GRAYED;
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1572+44
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 405
;405:	}
LABELV $307
line 407
;406:
;407:	if ( s_graphicsoptions.allow_extensions.curvalue == 0 )
ADDRGP4 s_graphicsoptions+1380+64
INDIRI4
CNSTI4 0
NEI4 $319
line 408
;408:	{
line 409
;409:		if ( s_graphicsoptions.texturebits.curvalue == 0 )
ADDRGP4 s_graphicsoptions+1476+64
INDIRI4
CNSTI4 0
NEI4 $323
line 410
;410:		{
line 411
;411:			s_graphicsoptions.texturebits.curvalue = 1;
ADDRGP4 s_graphicsoptions+1476+64
CNSTI4 1
ASGNI4
line 412
;412:		}
LABELV $323
line 413
;413:	}
LABELV $319
line 415
;414:
;415:	s_graphicsoptions.apply.generic.flags |= QMF_HIDDEN|QMF_INACTIVE;
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 417
;416:
;417:	if ( s_ivo.mode != s_graphicsoptions.mode.curvalue )
ADDRGP4 s_ivo
INDIRI4
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
EQI4 $331
line 418
;418:	{
line 419
;419:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 420
;420:	}
LABELV $331
line 421
;421:	if ( s_ivo.fullscreen != s_graphicsoptions.fs.curvalue )
ADDRGP4 s_ivo+4
INDIRI4
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
EQI4 $337
line 422
;422:	{
line 423
;423:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 424
;424:	}
LABELV $337
line 425
;425:	if ( s_ivo.extensions != s_graphicsoptions.allow_extensions.curvalue )
ADDRGP4 s_ivo+36
INDIRI4
ADDRGP4 s_graphicsoptions+1380+64
INDIRI4
EQI4 $344
line 426
;426:	{
line 427
;427:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 428
;428:	}
LABELV $344
line 429
;429:	if ( s_ivo.tq != s_graphicsoptions.tq.curvalue )
ADDRGP4 s_ivo+8
INDIRI4
CVIF4 4
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
EQF4 $351
line 430
;430:	{
line 431
;431:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 432
;432:	}
LABELV $351
line 433
;433:	if ( s_ivo.lighting != s_graphicsoptions.lighting.curvalue )
ADDRGP4 s_ivo+12
INDIRI4
ADDRGP4 s_graphicsoptions+1284+64
INDIRI4
EQI4 $358
line 434
;434:	{
line 435
;435:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 436
;436:	}
LABELV $358
line 437
;437:	if ( s_ivo.colordepth != s_graphicsoptions.colordepth.curvalue )
ADDRGP4 s_ivo+16
INDIRI4
ADDRGP4 s_graphicsoptions+1572+64
INDIRI4
EQI4 $365
line 438
;438:	{
line 439
;439:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 440
;440:	}
LABELV $365
line 441
;441:	if ( s_ivo.driver != s_graphicsoptions.driver.curvalue )
ADDRGP4 s_ivo+32
INDIRI4
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
EQI4 $372
line 442
;442:	{
line 443
;443:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 444
;444:	}
LABELV $372
line 445
;445:	if ( s_ivo.texturebits != s_graphicsoptions.texturebits.curvalue )
ADDRGP4 s_ivo+20
INDIRI4
ADDRGP4 s_graphicsoptions+1476+64
INDIRI4
EQI4 $379
line 446
;446:	{
line 447
;447:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 448
;448:	}
LABELV $379
line 449
;449:	if ( s_ivo.geometry != s_graphicsoptions.geometry.curvalue )
ADDRGP4 s_ivo+24
INDIRI4
ADDRGP4 s_graphicsoptions+1668+64
INDIRI4
EQI4 $386
line 450
;450:	{
line 451
;451:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 452
;452:	}
LABELV $386
line 453
;453:	if ( s_ivo.filter != s_graphicsoptions.filter.curvalue )
ADDRGP4 s_ivo+28
INDIRI4
ADDRGP4 s_graphicsoptions+1764+64
INDIRI4
EQI4 $393
line 454
;454:	{
line 455
;455:		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1932+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 456
;456:	}
LABELV $393
line 458
;457:
;458:	GraphicsOptions_CheckConfig();
ADDRGP4 GraphicsOptions_CheckConfig
CALLV
pop
line 459
;459:}	
LABELV $293
endproc GraphicsOptions_UpdateMenuItems 8 0
proc GraphicsOptions_ApplyChanges 8 8
line 467
;460:
;461:/*
;462:=================
;463:GraphicsOptions_ApplyChanges
;464:=================
;465:*/
;466:static void GraphicsOptions_ApplyChanges( void *unused, int notification )
;467:{
line 468
;468:	if (notification != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $401
line 469
;469:		return;
ADDRGP4 $400
JUMPV
LABELV $401
line 471
;470:
;471:	switch ( s_graphicsoptions.texturebits.curvalue  )
ADDRLP4 0
ADDRGP4 s_graphicsoptions+1476+64
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $407
ADDRLP4 0
INDIRI4
CNSTI4 1
EQI4 $409
ADDRLP4 0
INDIRI4
CNSTI4 2
EQI4 $410
ADDRGP4 $403
JUMPV
line 472
;472:	{
LABELV $407
line 474
;473:	case 0:
;474:		trap_Cvar_SetValue( "r_texturebits", 0 );
ADDRGP4 $408
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 475
;475:		break;
ADDRGP4 $404
JUMPV
LABELV $409
line 477
;476:	case 1:
;477:		trap_Cvar_SetValue( "r_texturebits", 16 );
ADDRGP4 $408
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 478
;478:		break;
ADDRGP4 $404
JUMPV
LABELV $410
line 480
;479:	case 2:
;480:		trap_Cvar_SetValue( "r_texturebits", 32 );
ADDRGP4 $408
ARGP4
CNSTF4 1107296256
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 481
;481:		break;
LABELV $403
LABELV $404
line 483
;482:	}
;483:	trap_Cvar_SetValue( "r_picmip", 3 - s_graphicsoptions.tq.curvalue );
ADDRGP4 $411
ARGP4
CNSTF4 1077936128
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
SUBF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 484
;484:	trap_Cvar_SetValue( "r_allowExtensions", s_graphicsoptions.allow_extensions.curvalue );
ADDRGP4 $414
ARGP4
ADDRGP4 s_graphicsoptions+1380+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 485
;485:	trap_Cvar_SetValue( "r_mode", s_graphicsoptions.mode.curvalue );
ADDRGP4 $417
ARGP4
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 486
;486:	trap_Cvar_SetValue( "r_fullscreen", s_graphicsoptions.fs.curvalue );
ADDRGP4 $420
ARGP4
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 487
;487:	trap_Cvar_Set( "r_glDriver", ( char * ) s_drivers[s_graphicsoptions.driver.curvalue] );
ADDRGP4 $423
ARGP4
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_drivers
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 488
;488:	switch ( s_graphicsoptions.colordepth.curvalue )
ADDRLP4 4
ADDRGP4 s_graphicsoptions+1572+64
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $430
ADDRLP4 4
INDIRI4
CNSTI4 1
EQI4 $434
ADDRLP4 4
INDIRI4
CNSTI4 2
EQI4 $435
ADDRGP4 $426
JUMPV
line 489
;489:	{
LABELV $430
line 491
;490:	case 0:
;491:		trap_Cvar_SetValue( "r_colorbits", 0 );
ADDRGP4 $431
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 492
;492:		trap_Cvar_SetValue( "r_depthbits", 0 );
ADDRGP4 $432
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 493
;493:		trap_Cvar_SetValue( "r_stencilbits", 0 );
ADDRGP4 $433
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 494
;494:		break;
ADDRGP4 $427
JUMPV
LABELV $434
line 496
;495:	case 1:
;496:		trap_Cvar_SetValue( "r_colorbits", 16 );
ADDRGP4 $431
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 497
;497:		trap_Cvar_SetValue( "r_depthbits", 16 );
ADDRGP4 $432
ARGP4
CNSTF4 1098907648
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 498
;498:		trap_Cvar_SetValue( "r_stencilbits", 0 );
ADDRGP4 $433
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 499
;499:		break;
ADDRGP4 $427
JUMPV
LABELV $435
line 501
;500:	case 2:
;501:		trap_Cvar_SetValue( "r_colorbits", 32 );
ADDRGP4 $431
ARGP4
CNSTF4 1107296256
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 502
;502:		trap_Cvar_SetValue( "r_depthbits", 24 );
ADDRGP4 $432
ARGP4
CNSTF4 1103101952
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 503
;503:		break;
LABELV $426
LABELV $427
line 505
;504:	}
;505:	trap_Cvar_SetValue( "r_vertexLight", s_graphicsoptions.lighting.curvalue );
ADDRGP4 $436
ARGP4
ADDRGP4 s_graphicsoptions+1284+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 507
;506:
;507:	if ( s_graphicsoptions.geometry.curvalue == 2 )
ADDRGP4 s_graphicsoptions+1668+64
INDIRI4
CNSTI4 2
NEI4 $439
line 508
;508:	{
line 509
;509:		trap_Cvar_SetValue( "r_lodBias", 0 );
ADDRGP4 $443
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 510
;510:		trap_Cvar_SetValue( "r_subdivisions", 4 );
ADDRGP4 $444
ARGP4
CNSTF4 1082130432
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 511
;511:	}
ADDRGP4 $440
JUMPV
LABELV $439
line 512
;512:	else if ( s_graphicsoptions.geometry.curvalue == 1 )
ADDRGP4 s_graphicsoptions+1668+64
INDIRI4
CNSTI4 1
NEI4 $445
line 513
;513:	{
line 514
;514:		trap_Cvar_SetValue( "r_lodBias", 1 );
ADDRGP4 $443
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 515
;515:		trap_Cvar_SetValue( "r_subdivisions", 12 );
ADDRGP4 $444
ARGP4
CNSTF4 1094713344
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 516
;516:	}
ADDRGP4 $446
JUMPV
LABELV $445
line 518
;517:	else
;518:	{
line 519
;519:		trap_Cvar_SetValue( "r_lodBias", 1 );
ADDRGP4 $443
ARGP4
CNSTF4 1065353216
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 520
;520:		trap_Cvar_SetValue( "r_subdivisions", 20 );
ADDRGP4 $444
ARGP4
CNSTF4 1101004800
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 521
;521:	}
LABELV $446
LABELV $440
line 523
;522:
;523:	if ( s_graphicsoptions.filter.curvalue )
ADDRGP4 s_graphicsoptions+1764+64
INDIRI4
CNSTI4 0
EQI4 $449
line 524
;524:	{
line 525
;525:		trap_Cvar_Set( "r_textureMode", "GL_LINEAR_MIPMAP_LINEAR" );
ADDRGP4 $453
ARGP4
ADDRGP4 $454
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 526
;526:	}
ADDRGP4 $450
JUMPV
LABELV $449
line 528
;527:	else
;528:	{
line 529
;529:		trap_Cvar_Set( "r_textureMode", "GL_LINEAR_MIPMAP_NEAREST" );
ADDRGP4 $453
ARGP4
ADDRGP4 $455
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 530
;530:	}
LABELV $450
line 532
;531:
;532:	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
CNSTI4 2
ARGI4
ADDRGP4 $456
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 533
;533:}
LABELV $400
endproc GraphicsOptions_ApplyChanges 8 8
proc GraphicsOptions_Event 12 0
line 540
;534:
;535:/*
;536:=================
;537:GraphicsOptions_Event
;538:=================
;539:*/
;540:static void GraphicsOptions_Event( void* ptr, int event ) {
line 543
;541:	InitialVideoOptions_s *ivo;
;542:
;543:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $458
line 544
;544:	 	return;
ADDRGP4 $457
JUMPV
LABELV $458
line 547
;545:	}
;546:
;547:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 101
LTI4 $460
ADDRLP4 4
INDIRI4
CNSTI4 109
GTI4 $460
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $505-404
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $505
address $500
address $460
address $480
address $463
address $499
address $461
address $502
address $503
address $504
code
LABELV $463
line 550
;548:	case ID_MODE:
;549:		// clamp 3dfx video modes
;550:		if ( s_graphicsoptions.driver.curvalue == 1 )
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
CNSTI4 1
NEI4 $461
line 551
;551:		{
line 552
;552:			if ( s_graphicsoptions.mode.curvalue < 2 )
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
CNSTI4 2
GEI4 $468
line 553
;553:				s_graphicsoptions.mode.curvalue = 2;
ADDRGP4 s_graphicsoptions+920+64
CNSTI4 2
ASGNI4
ADDRGP4 $461
JUMPV
LABELV $468
line 554
;554:			else if ( s_graphicsoptions.mode.curvalue > 6 )
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
CNSTI4 6
LEI4 $461
line 555
;555:				s_graphicsoptions.mode.curvalue = 6;
ADDRGP4 s_graphicsoptions+920+64
CNSTI4 6
ASGNI4
line 556
;556:		}
line 557
;557:		break;
ADDRGP4 $461
JUMPV
LABELV $480
line 560
;558:
;559:	case ID_LIST:
;560:		ivo = &s_ivo_templates[s_graphicsoptions.list.curvalue];
ADDRLP4 0
CNSTI4 40
ADDRGP4 s_graphicsoptions+824+64
INDIRI4
MULI4
ADDRGP4 s_ivo_templates
ADDP4
ASGNP4
line 562
;561:
;562:		s_graphicsoptions.mode.curvalue        = ivo->mode;
ADDRGP4 s_graphicsoptions+920+64
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 563
;563:		s_graphicsoptions.tq.curvalue          = ivo->tq;
ADDRGP4 s_graphicsoptions+1112+68
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 564
;564:		s_graphicsoptions.lighting.curvalue    = ivo->lighting;
ADDRGP4 s_graphicsoptions+1284+64
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 565
;565:		s_graphicsoptions.colordepth.curvalue  = ivo->colordepth;
ADDRGP4 s_graphicsoptions+1572+64
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 566
;566:		s_graphicsoptions.texturebits.curvalue = ivo->texturebits;
ADDRGP4 s_graphicsoptions+1476+64
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
line 567
;567:		s_graphicsoptions.geometry.curvalue    = ivo->geometry;
ADDRGP4 s_graphicsoptions+1668+64
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
line 568
;568:		s_graphicsoptions.filter.curvalue      = ivo->filter;
ADDRGP4 s_graphicsoptions+1764+64
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ASGNI4
line 569
;569:		s_graphicsoptions.fs.curvalue          = ivo->fullscreen;
ADDRGP4 s_graphicsoptions+1188+64
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 570
;570:		break;
ADDRGP4 $461
JUMPV
LABELV $499
line 573
;571:
;572:	case ID_DRIVERINFO:
;573:		UI_DriverInfo_Menu();
ADDRGP4 UI_DriverInfo_Menu
CALLV
pop
line 574
;574:		break;
ADDRGP4 $461
JUMPV
LABELV $500
line 577
;575:
;576:	case ID_BACK2:
;577:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 578
;578:		break;
ADDRGP4 $461
JUMPV
line 581
;579:
;580:	case ID_GRAPHICS:
;581:		break;
LABELV $502
line 584
;582:
;583:	case ID_DISPLAY:
;584:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 585
;585:		UI_DisplayOptionsMenu();
ADDRGP4 UI_DisplayOptionsMenu
CALLV
pop
line 586
;586:		break;
ADDRGP4 $461
JUMPV
LABELV $503
line 589
;587:
;588:	case ID_SOUND:
;589:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 590
;590:		UI_SoundOptionsMenu();
ADDRGP4 UI_SoundOptionsMenu
CALLV
pop
line 591
;591:		break;
ADDRGP4 $461
JUMPV
LABELV $504
line 594
;592:
;593:	case ID_NETWORK:
;594:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 595
;595:		UI_NetworkOptionsMenu();
ADDRGP4 UI_NetworkOptionsMenu
CALLV
pop
line 596
;596:		break;
LABELV $460
LABELV $461
line 598
;597:	}
;598:}
LABELV $457
endproc GraphicsOptions_Event 12 0
proc GraphicsOptions_TQEvent 0 0
line 606
;599:
;600:
;601:/*
;602:================
;603:GraphicsOptions_TQEvent
;604:================
;605:*/
;606:static void GraphicsOptions_TQEvent( void *ptr, int event ) {
line 607
;607:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $508
line 608
;608:	 	return;
ADDRGP4 $507
JUMPV
LABELV $508
line 610
;609:	}
;610:	s_graphicsoptions.tq.curvalue = (int)(s_graphicsoptions.tq.curvalue + 0.5);
ADDRGP4 s_graphicsoptions+1112+68
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
CNSTF4 1056964608
ADDF4
CVFI4 4
CVIF4 4
ASGNF4
line 611
;611:}
LABELV $507
endproc GraphicsOptions_TQEvent 0 0
export GraphicsOptions_MenuDraw
proc GraphicsOptions_MenuDraw 0 4
line 620
;612:
;613:
;614:/*
;615:================
;616:GraphicsOptions_MenuDraw
;617:================
;618:*/
;619:void GraphicsOptions_MenuDraw (void)
;620:{
line 622
;621://APSFIX - rework this
;622:	GraphicsOptions_UpdateMenuItems();
ADDRGP4 GraphicsOptions_UpdateMenuItems
CALLV
pop
line 624
;623:
;624:	Menu_Draw( &s_graphicsoptions.menu );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 625
;625:}
LABELV $514
endproc GraphicsOptions_MenuDraw 0 4
proc GraphicsOptions_SetMenuItems 68 8
line 633
;626:
;627:/*
;628:=================
;629:GraphicsOptions_SetMenuItems
;630:=================
;631:*/
;632:static void GraphicsOptions_SetMenuItems( void )
;633:{
line 634
;634:	s_graphicsoptions.mode.curvalue = trap_Cvar_VariableValue( "r_mode" );
ADDRGP4 $417
ARGP4
ADDRLP4 0
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 s_graphicsoptions+920+64
ADDRLP4 0
INDIRF4
CVFI4 4
ASGNI4
line 635
;635:	if ( s_graphicsoptions.mode.curvalue < 0 )
ADDRGP4 s_graphicsoptions+920+64
INDIRI4
CNSTI4 0
GEI4 $518
line 636
;636:	{
line 637
;637:		s_graphicsoptions.mode.curvalue = 3;
ADDRGP4 s_graphicsoptions+920+64
CNSTI4 3
ASGNI4
line 638
;638:	}
LABELV $518
line 639
;639:	s_graphicsoptions.fs.curvalue = trap_Cvar_VariableValue("r_fullscreen");
ADDRGP4 $420
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 s_graphicsoptions+1188+64
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 640
;640:	s_graphicsoptions.allow_extensions.curvalue = trap_Cvar_VariableValue("r_allowExtensions");
ADDRGP4 $414
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 s_graphicsoptions+1380+64
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 641
;641:	s_graphicsoptions.tq.curvalue = 3-trap_Cvar_VariableValue( "r_picmip");
ADDRGP4 $411
ARGP4
ADDRLP4 12
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 s_graphicsoptions+1112+68
CNSTF4 1077936128
ADDRLP4 12
INDIRF4
SUBF4
ASGNF4
line 642
;642:	if ( s_graphicsoptions.tq.curvalue < 0 )
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
CNSTF4 0
GEF4 $530
line 643
;643:	{
line 644
;644:		s_graphicsoptions.tq.curvalue = 0;
ADDRGP4 s_graphicsoptions+1112+68
CNSTF4 0
ASGNF4
line 645
;645:	}
ADDRGP4 $531
JUMPV
LABELV $530
line 646
;646:	else if ( s_graphicsoptions.tq.curvalue > 3 )
ADDRGP4 s_graphicsoptions+1112+68
INDIRF4
CNSTF4 1077936128
LEF4 $536
line 647
;647:	{
line 648
;648:		s_graphicsoptions.tq.curvalue = 3;
ADDRGP4 s_graphicsoptions+1112+68
CNSTF4 1077936128
ASGNF4
line 649
;649:	}
LABELV $536
LABELV $531
line 651
;650:
;651:	s_graphicsoptions.lighting.curvalue = trap_Cvar_VariableValue( "r_vertexLight" ) != 0;
ADDRGP4 $436
ARGP4
ADDRLP4 20
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 20
INDIRF4
CNSTF4 0
EQF4 $545
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRGP4 $546
JUMPV
LABELV $545
ADDRLP4 16
CNSTI4 0
ASGNI4
LABELV $546
ADDRGP4 s_graphicsoptions+1284+64
ADDRLP4 16
INDIRI4
ASGNI4
line 652
;652:	switch ( ( int ) trap_Cvar_VariableValue( "r_texturebits" ) )
ADDRGP4 $408
ARGP4
ADDRLP4 28
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 28
INDIRF4
CVFI4 4
ASGNI4
ADDRLP4 36
CNSTI4 16
ASGNI4
ADDRLP4 24
INDIRI4
ADDRLP4 36
INDIRI4
EQI4 $553
ADDRLP4 24
INDIRI4
ADDRLP4 36
INDIRI4
GTI4 $560
LABELV $559
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $550
ADDRGP4 $547
JUMPV
LABELV $560
ADDRLP4 24
INDIRI4
CNSTI4 32
EQI4 $556
ADDRGP4 $547
JUMPV
line 653
;653:	{
LABELV $547
LABELV $550
line 656
;654:	default:
;655:	case 0:
;656:		s_graphicsoptions.texturebits.curvalue = 0;
ADDRGP4 s_graphicsoptions+1476+64
CNSTI4 0
ASGNI4
line 657
;657:		break;
ADDRGP4 $548
JUMPV
LABELV $553
line 659
;658:	case 16:
;659:		s_graphicsoptions.texturebits.curvalue = 1;
ADDRGP4 s_graphicsoptions+1476+64
CNSTI4 1
ASGNI4
line 660
;660:		break;
ADDRGP4 $548
JUMPV
LABELV $556
line 662
;661:	case 32:
;662:		s_graphicsoptions.texturebits.curvalue = 2;
ADDRGP4 s_graphicsoptions+1476+64
CNSTI4 2
ASGNI4
line 663
;663:		break;
LABELV $548
line 666
;664:	}
;665:
;666:	if ( !Q_stricmp( UI_Cvar_VariableString( "r_textureMode" ), "GL_LINEAR_MIPMAP_NEAREST" ) )
ADDRGP4 $453
ARGP4
ADDRLP4 40
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRGP4 $455
ARGP4
ADDRLP4 44
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $561
line 667
;667:	{
line 668
;668:		s_graphicsoptions.filter.curvalue = 0;
ADDRGP4 s_graphicsoptions+1764+64
CNSTI4 0
ASGNI4
line 669
;669:	}
ADDRGP4 $562
JUMPV
LABELV $561
line 671
;670:	else
;671:	{
line 672
;672:		s_graphicsoptions.filter.curvalue = 1;
ADDRGP4 s_graphicsoptions+1764+64
CNSTI4 1
ASGNI4
line 673
;673:	}
LABELV $562
line 675
;674:
;675:	if ( trap_Cvar_VariableValue( "r_lodBias" ) > 0 )
ADDRGP4 $443
ARGP4
ADDRLP4 48
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 48
INDIRF4
CNSTF4 0
LEF4 $567
line 676
;676:	{
line 677
;677:		if ( trap_Cvar_VariableValue( "r_subdivisions" ) >= 20 )
ADDRGP4 $444
ARGP4
ADDRLP4 52
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 52
INDIRF4
CNSTF4 1101004800
LTF4 $569
line 678
;678:		{
line 679
;679:			s_graphicsoptions.geometry.curvalue = 0;
ADDRGP4 s_graphicsoptions+1668+64
CNSTI4 0
ASGNI4
line 680
;680:		}
ADDRGP4 $568
JUMPV
LABELV $569
line 682
;681:		else
;682:		{
line 683
;683:			s_graphicsoptions.geometry.curvalue = 1;
ADDRGP4 s_graphicsoptions+1668+64
CNSTI4 1
ASGNI4
line 684
;684:		}
line 685
;685:	}
ADDRGP4 $568
JUMPV
LABELV $567
line 687
;686:	else
;687:	{
line 688
;688:		s_graphicsoptions.geometry.curvalue = 2;
ADDRGP4 s_graphicsoptions+1668+64
CNSTI4 2
ASGNI4
line 689
;689:	}
LABELV $568
line 691
;690:
;691:	switch ( ( int ) trap_Cvar_VariableValue( "r_colorbits" ) )
ADDRGP4 $431
ARGP4
ADDRLP4 56
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 52
ADDRLP4 56
INDIRF4
CVFI4 4
ASGNI4
ADDRLP4 64
CNSTI4 16
ASGNI4
ADDRLP4 52
INDIRI4
ADDRLP4 64
INDIRI4
EQI4 $583
ADDRLP4 52
INDIRI4
ADDRLP4 64
INDIRI4
GTI4 $590
LABELV $589
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $580
ADDRGP4 $577
JUMPV
LABELV $590
ADDRLP4 52
INDIRI4
CNSTI4 32
EQI4 $586
ADDRGP4 $577
JUMPV
line 692
;692:	{
LABELV $577
LABELV $580
line 695
;693:	default:
;694:	case 0:
;695:		s_graphicsoptions.colordepth.curvalue = 0;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 0
ASGNI4
line 696
;696:		break;
ADDRGP4 $578
JUMPV
LABELV $583
line 698
;697:	case 16:
;698:		s_graphicsoptions.colordepth.curvalue = 1;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 1
ASGNI4
line 699
;699:		break;
ADDRGP4 $578
JUMPV
LABELV $586
line 701
;700:	case 32:
;701:		s_graphicsoptions.colordepth.curvalue = 2;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 2
ASGNI4
line 702
;702:		break;
LABELV $578
line 705
;703:	}
;704:
;705:	if ( s_graphicsoptions.fs.curvalue == 0 )
ADDRGP4 s_graphicsoptions+1188+64
INDIRI4
CNSTI4 0
NEI4 $591
line 706
;706:	{
line 707
;707:		s_graphicsoptions.colordepth.curvalue = 0;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 0
ASGNI4
line 708
;708:	}
LABELV $591
line 709
;709:	if ( s_graphicsoptions.driver.curvalue == 1 )
ADDRGP4 s_graphicsoptions+1016+64
INDIRI4
CNSTI4 1
NEI4 $597
line 710
;710:	{
line 711
;711:		s_graphicsoptions.colordepth.curvalue = 1;
ADDRGP4 s_graphicsoptions+1572+64
CNSTI4 1
ASGNI4
line 712
;712:	}
LABELV $597
line 713
;713:}
LABELV $515
endproc GraphicsOptions_SetMenuItems 68 8
data
align 4
LABELV $604
address $605
address $606
byte 4 0
align 4
LABELV $607
address $605
address $608
address $609
byte 4 0
align 4
LABELV $610
address $611
address $612
address $613
address $614
address $615
byte 4 0
align 4
LABELV $616
address $617
address $618
byte 4 0
align 4
LABELV $619
address $605
address $608
address $609
byte 4 0
align 4
LABELV $620
address $621
address $622
address $623
address $624
address $625
address $626
address $627
address $628
address $629
address $630
address $631
address $632
byte 4 0
align 4
LABELV $633
address $634
address $635
byte 4 0
align 4
LABELV $636
address $637
address $638
address $639
byte 4 0
align 4
LABELV $640
address $641
address $642
byte 4 0
export GraphicsOptions_MenuInit
code
proc GraphicsOptions_MenuInit 12 12
line 721
;714:
;715:/*
;716:================
;717:GraphicsOptions_MenuInit
;718:================
;719:*/
;720:void GraphicsOptions_MenuInit( void )
;721:{
line 801
;722:	static const char *s_driver_names[] =
;723:	{
;724:		"Default",
;725:		"Voodoo",
;726:		0
;727:	};
;728:
;729:	static const char *tq_names[] =
;730:	{
;731:		"Default",
;732:		"16 bit",
;733:		"32 bit",
;734:		0
;735:	};
;736:
;737:	static const char *s_graphics_options_names[] =
;738:	{
;739:		"High Quality",
;740:		"Normal",
;741:		"Fast",
;742:		"Fastest",
;743:		"Custom",
;744:		0
;745:	};
;746:
;747:	static const char *lighting_names[] =
;748:	{
;749:		"Lightmap",
;750:		"Vertex",
;751:		0
;752:	};
;753:
;754:	static const char *colordepth_names[] =
;755:	{
;756:		"Default",
;757:		"16 bit",
;758:		"32 bit",
;759:		0
;760:	};
;761:
;762:	static const char *resolutions[] = 
;763:	{
;764:		"320x240",
;765:		"400x300",
;766:		"512x384",
;767:		"640x480",
;768:		"800x600",
;769:		"960x720",
;770:		"1024x768",
;771:		"1152x864",
;772:		"1280x1024",
;773:		"1600x1200",
;774:		"2048x1536",
;775:		"856x480 wide screen",
;776:		0
;777:	};
;778:	static const char *filter_names[] =
;779:	{
;780:		"Bilinear",
;781:		"Trilinear",
;782:		0
;783:	};
;784:	static const char *quality_names[] =
;785:	{
;786:		"Low",
;787:		"Medium",
;788:		"High",
;789:		0
;790:	};
;791:	static const char *enabled_names[] =
;792:	{
;793:		"Off",
;794:		"On",
;795:		0
;796:	};
;797:
;798:	int y;
;799:
;800:	// zero set all our globals
;801:	memset( &s_graphicsoptions, 0 ,sizeof(graphicsoptions_t) );
ADDRGP4 s_graphicsoptions
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2108
ARGI4
ADDRGP4 memset
CALLP4
pop
line 803
;802:
;803:	GraphicsOptions_Cache();
ADDRGP4 GraphicsOptions_Cache
CALLV
pop
line 805
;804:
;805:	s_graphicsoptions.menu.wrapAround = qtrue;
ADDRGP4 s_graphicsoptions+276
CNSTI4 1
ASGNI4
line 806
;806:	s_graphicsoptions.menu.fullscreen = qtrue;
ADDRGP4 s_graphicsoptions+280
CNSTI4 1
ASGNI4
line 807
;807:	s_graphicsoptions.menu.draw       = GraphicsOptions_MenuDraw;
ADDRGP4 s_graphicsoptions+268
ADDRGP4 GraphicsOptions_MenuDraw
ASGNP4
line 809
;808:
;809:	s_graphicsoptions.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_graphicsoptions+288
CNSTI4 10
ASGNI4
line 810
;810:	s_graphicsoptions.banner.generic.x	   = 320;
ADDRGP4 s_graphicsoptions+288+12
CNSTI4 320
ASGNI4
line 811
;811:	s_graphicsoptions.banner.generic.y	   = 16;
ADDRGP4 s_graphicsoptions+288+16
CNSTI4 16
ASGNI4
line 812
;812:	s_graphicsoptions.banner.string  	   = "SYSTEM SETUP";
ADDRGP4 s_graphicsoptions+288+60
ADDRGP4 $653
ASGNP4
line 813
;813:	s_graphicsoptions.banner.color         = color_white;
ADDRGP4 s_graphicsoptions+288+68
ADDRGP4 color_white
ASGNP4
line 814
;814:	s_graphicsoptions.banner.style         = UI_CENTER;
ADDRGP4 s_graphicsoptions+288+64
CNSTI4 1
ASGNI4
line 816
;815:
;816:	s_graphicsoptions.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_graphicsoptions+360
CNSTI4 6
ASGNI4
line 817
;817:	s_graphicsoptions.framel.generic.name  = GRAPHICSOPTIONS_FRAMEL;
ADDRGP4 s_graphicsoptions+360+4
ADDRGP4 $67
ASGNP4
line 818
;818:	s_graphicsoptions.framel.generic.flags = QMF_INACTIVE;
ADDRGP4 s_graphicsoptions+360+44
CNSTU4 16384
ASGNU4
line 819
;819:	s_graphicsoptions.framel.generic.x	   = 0;
ADDRGP4 s_graphicsoptions+360+12
CNSTI4 0
ASGNI4
line 820
;820:	s_graphicsoptions.framel.generic.y	   = 78;
ADDRGP4 s_graphicsoptions+360+16
CNSTI4 78
ASGNI4
line 821
;821:	s_graphicsoptions.framel.width  	   = 256;
ADDRGP4 s_graphicsoptions+360+76
CNSTI4 256
ASGNI4
line 822
;822:	s_graphicsoptions.framel.height  	   = 329;
ADDRGP4 s_graphicsoptions+360+80
CNSTI4 329
ASGNI4
line 824
;823:
;824:	s_graphicsoptions.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_graphicsoptions+448
CNSTI4 6
ASGNI4
line 825
;825:	s_graphicsoptions.framer.generic.name  = GRAPHICSOPTIONS_FRAMER;
ADDRGP4 s_graphicsoptions+448+4
ADDRGP4 $68
ASGNP4
line 826
;826:	s_graphicsoptions.framer.generic.flags = QMF_INACTIVE;
ADDRGP4 s_graphicsoptions+448+44
CNSTU4 16384
ASGNU4
line 827
;827:	s_graphicsoptions.framer.generic.x	   = 376;
ADDRGP4 s_graphicsoptions+448+12
CNSTI4 376
ASGNI4
line 828
;828:	s_graphicsoptions.framer.generic.y	   = 76;
ADDRGP4 s_graphicsoptions+448+16
CNSTI4 76
ASGNI4
line 829
;829:	s_graphicsoptions.framer.width  	   = 256;
ADDRGP4 s_graphicsoptions+448+76
CNSTI4 256
ASGNI4
line 830
;830:	s_graphicsoptions.framer.height  	   = 334;
ADDRGP4 s_graphicsoptions+448+80
CNSTI4 334
ASGNI4
line 832
;831:
;832:	s_graphicsoptions.graphics.generic.type		= MTYPE_PTEXT;
ADDRGP4 s_graphicsoptions+536
CNSTI4 9
ASGNI4
line 833
;833:	s_graphicsoptions.graphics.generic.flags	= QMF_RIGHT_JUSTIFY;
ADDRGP4 s_graphicsoptions+536+44
CNSTU4 16
ASGNU4
line 834
;834:	s_graphicsoptions.graphics.generic.id		= ID_GRAPHICS;
ADDRGP4 s_graphicsoptions+536+8
CNSTI4 106
ASGNI4
line 835
;835:	s_graphicsoptions.graphics.generic.callback	= GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+536+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 836
;836:	s_graphicsoptions.graphics.generic.x		= 216;
ADDRGP4 s_graphicsoptions+536+12
CNSTI4 216
ASGNI4
line 837
;837:	s_graphicsoptions.graphics.generic.y		= 240 - 2 * PROP_HEIGHT;
ADDRGP4 s_graphicsoptions+536+16
CNSTI4 186
ASGNI4
line 838
;838:	s_graphicsoptions.graphics.string			= "GRAPHICS";
ADDRGP4 s_graphicsoptions+536+60
ADDRGP4 $697
ASGNP4
line 839
;839:	s_graphicsoptions.graphics.style			= UI_RIGHT;
ADDRGP4 s_graphicsoptions+536+64
CNSTI4 2
ASGNI4
line 840
;840:	s_graphicsoptions.graphics.color			= color_red;
ADDRGP4 s_graphicsoptions+536+68
ADDRGP4 color_red
ASGNP4
line 842
;841:
;842:	s_graphicsoptions.display.generic.type		= MTYPE_PTEXT;
ADDRGP4 s_graphicsoptions+608
CNSTI4 9
ASGNI4
line 843
;843:	s_graphicsoptions.display.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_graphicsoptions+608+44
CNSTU4 272
ASGNU4
line 844
;844:	s_graphicsoptions.display.generic.id		= ID_DISPLAY;
ADDRGP4 s_graphicsoptions+608+8
CNSTI4 107
ASGNI4
line 845
;845:	s_graphicsoptions.display.generic.callback	= GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+608+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 846
;846:	s_graphicsoptions.display.generic.x			= 216;
ADDRGP4 s_graphicsoptions+608+12
CNSTI4 216
ASGNI4
line 847
;847:	s_graphicsoptions.display.generic.y			= 240 - PROP_HEIGHT;
ADDRGP4 s_graphicsoptions+608+16
CNSTI4 213
ASGNI4
line 848
;848:	s_graphicsoptions.display.string			= "DISPLAY";
ADDRGP4 s_graphicsoptions+608+60
ADDRGP4 $715
ASGNP4
line 849
;849:	s_graphicsoptions.display.style				= UI_RIGHT;
ADDRGP4 s_graphicsoptions+608+64
CNSTI4 2
ASGNI4
line 850
;850:	s_graphicsoptions.display.color				= color_red;
ADDRGP4 s_graphicsoptions+608+68
ADDRGP4 color_red
ASGNP4
line 852
;851:
;852:	s_graphicsoptions.sound.generic.type		= MTYPE_PTEXT;
ADDRGP4 s_graphicsoptions+680
CNSTI4 9
ASGNI4
line 853
;853:	s_graphicsoptions.sound.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_graphicsoptions+680+44
CNSTU4 272
ASGNU4
line 854
;854:	s_graphicsoptions.sound.generic.id			= ID_SOUND;
ADDRGP4 s_graphicsoptions+680+8
CNSTI4 108
ASGNI4
line 855
;855:	s_graphicsoptions.sound.generic.callback	= GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+680+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 856
;856:	s_graphicsoptions.sound.generic.x			= 216;
ADDRGP4 s_graphicsoptions+680+12
CNSTI4 216
ASGNI4
line 857
;857:	s_graphicsoptions.sound.generic.y			= 240;
ADDRGP4 s_graphicsoptions+680+16
CNSTI4 240
ASGNI4
line 858
;858:	s_graphicsoptions.sound.string				= "SOUND";
ADDRGP4 s_graphicsoptions+680+60
ADDRGP4 $733
ASGNP4
line 859
;859:	s_graphicsoptions.sound.style				= UI_RIGHT;
ADDRGP4 s_graphicsoptions+680+64
CNSTI4 2
ASGNI4
line 860
;860:	s_graphicsoptions.sound.color				= color_red;
ADDRGP4 s_graphicsoptions+680+68
ADDRGP4 color_red
ASGNP4
line 862
;861:
;862:	s_graphicsoptions.network.generic.type		= MTYPE_PTEXT;
ADDRGP4 s_graphicsoptions+752
CNSTI4 9
ASGNI4
line 863
;863:	s_graphicsoptions.network.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_graphicsoptions+752+44
CNSTU4 272
ASGNU4
line 864
;864:	s_graphicsoptions.network.generic.id		= ID_NETWORK;
ADDRGP4 s_graphicsoptions+752+8
CNSTI4 109
ASGNI4
line 865
;865:	s_graphicsoptions.network.generic.callback	= GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+752+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 866
;866:	s_graphicsoptions.network.generic.x			= 216;
ADDRGP4 s_graphicsoptions+752+12
CNSTI4 216
ASGNI4
line 867
;867:	s_graphicsoptions.network.generic.y			= 240 + PROP_HEIGHT;
ADDRGP4 s_graphicsoptions+752+16
CNSTI4 267
ASGNI4
line 868
;868:	s_graphicsoptions.network.string			= "NETWORK";
ADDRGP4 s_graphicsoptions+752+60
ADDRGP4 $751
ASGNP4
line 869
;869:	s_graphicsoptions.network.style				= UI_RIGHT;
ADDRGP4 s_graphicsoptions+752+64
CNSTI4 2
ASGNI4
line 870
;870:	s_graphicsoptions.network.color				= color_red;
ADDRGP4 s_graphicsoptions+752+68
ADDRGP4 color_red
ASGNP4
line 872
;871:
;872:	y = 240 - 6 * (BIGCHAR_HEIGHT + 2);
ADDRLP4 0
CNSTI4 132
ASGNI4
line 873
;873:	s_graphicsoptions.list.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+824
CNSTI4 3
ASGNI4
line 874
;874:	s_graphicsoptions.list.generic.name     = "Graphics Settings:";
ADDRGP4 s_graphicsoptions+824+4
ADDRGP4 $759
ASGNP4
line 875
;875:	s_graphicsoptions.list.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+824+44
CNSTU4 258
ASGNU4
line 876
;876:	s_graphicsoptions.list.generic.x        = 400;
ADDRGP4 s_graphicsoptions+824+12
CNSTI4 400
ASGNI4
line 877
;877:	s_graphicsoptions.list.generic.y        = y;
ADDRGP4 s_graphicsoptions+824+16
ADDRLP4 0
INDIRI4
ASGNI4
line 878
;878:	s_graphicsoptions.list.generic.callback = GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+824+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 879
;879:	s_graphicsoptions.list.generic.id       = ID_LIST;
ADDRGP4 s_graphicsoptions+824+8
CNSTI4 103
ASGNI4
line 880
;880:	s_graphicsoptions.list.itemnames        = s_graphics_options_names;
ADDRGP4 s_graphicsoptions+824+76
ADDRGP4 $610
ASGNP4
line 881
;881:	y += 2 * ( BIGCHAR_HEIGHT + 2 );
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 36
ADDI4
ASGNI4
line 883
;882:
;883:	s_graphicsoptions.driver.generic.type  = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1016
CNSTI4 3
ASGNI4
line 884
;884:	s_graphicsoptions.driver.generic.name  = "GL Driver:";
ADDRGP4 s_graphicsoptions+1016+4
ADDRGP4 $775
ASGNP4
line 885
;885:	s_graphicsoptions.driver.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1016+44
CNSTU4 258
ASGNU4
line 886
;886:	s_graphicsoptions.driver.generic.x     = 400;
ADDRGP4 s_graphicsoptions+1016+12
CNSTI4 400
ASGNI4
line 887
;887:	s_graphicsoptions.driver.generic.y     = y;
ADDRGP4 s_graphicsoptions+1016+16
ADDRLP4 0
INDIRI4
ASGNI4
line 888
;888:	s_graphicsoptions.driver.itemnames     = s_driver_names;
ADDRGP4 s_graphicsoptions+1016+76
ADDRGP4 $604
ASGNP4
line 889
;889:	s_graphicsoptions.driver.curvalue      = (uis.glconfig.driverType == GLDRV_VOODOO);
ADDRGP4 uis+56+11284
INDIRI4
CNSTI4 2
NEI4 $789
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $790
JUMPV
LABELV $789
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $790
ADDRGP4 s_graphicsoptions+1016+64
ADDRLP4 4
INDIRI4
ASGNI4
line 890
;890:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 893
;891:
;892:	// references/modifies "r_allowExtensions"
;893:	s_graphicsoptions.allow_extensions.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1380
CNSTI4 3
ASGNI4
line 894
;894:	s_graphicsoptions.allow_extensions.generic.name	    = "GL Extensions:";
ADDRGP4 s_graphicsoptions+1380+4
ADDRGP4 $794
ASGNP4
line 895
;895:	s_graphicsoptions.allow_extensions.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1380+44
CNSTU4 258
ASGNU4
line 896
;896:	s_graphicsoptions.allow_extensions.generic.x	    = 400;
ADDRGP4 s_graphicsoptions+1380+12
CNSTI4 400
ASGNI4
line 897
;897:	s_graphicsoptions.allow_extensions.generic.y	    = y;
ADDRGP4 s_graphicsoptions+1380+16
ADDRLP4 0
INDIRI4
ASGNI4
line 898
;898:	s_graphicsoptions.allow_extensions.itemnames        = enabled_names;
ADDRGP4 s_graphicsoptions+1380+76
ADDRGP4 $640
ASGNP4
line 899
;899:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 902
;900:
;901:	// references/modifies "r_mode"
;902:	s_graphicsoptions.mode.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+920
CNSTI4 3
ASGNI4
line 903
;903:	s_graphicsoptions.mode.generic.name     = "Video Mode:";
ADDRGP4 s_graphicsoptions+920+4
ADDRGP4 $806
ASGNP4
line 904
;904:	s_graphicsoptions.mode.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+920+44
CNSTU4 258
ASGNU4
line 905
;905:	s_graphicsoptions.mode.generic.x        = 400;
ADDRGP4 s_graphicsoptions+920+12
CNSTI4 400
ASGNI4
line 906
;906:	s_graphicsoptions.mode.generic.y        = y;
ADDRGP4 s_graphicsoptions+920+16
ADDRLP4 0
INDIRI4
ASGNI4
line 907
;907:	s_graphicsoptions.mode.itemnames        = resolutions;
ADDRGP4 s_graphicsoptions+920+76
ADDRGP4 $620
ASGNP4
line 908
;908:	s_graphicsoptions.mode.generic.callback = GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+920+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 909
;909:	s_graphicsoptions.mode.generic.id       = ID_MODE;
ADDRGP4 s_graphicsoptions+920+8
CNSTI4 104
ASGNI4
line 910
;910:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 913
;911:
;912:	// references "r_colorbits"
;913:	s_graphicsoptions.colordepth.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1572
CNSTI4 3
ASGNI4
line 914
;914:	s_graphicsoptions.colordepth.generic.name     = "Color Depth:";
ADDRGP4 s_graphicsoptions+1572+4
ADDRGP4 $822
ASGNP4
line 915
;915:	s_graphicsoptions.colordepth.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1572+44
CNSTU4 258
ASGNU4
line 916
;916:	s_graphicsoptions.colordepth.generic.x        = 400;
ADDRGP4 s_graphicsoptions+1572+12
CNSTI4 400
ASGNI4
line 917
;917:	s_graphicsoptions.colordepth.generic.y        = y;
ADDRGP4 s_graphicsoptions+1572+16
ADDRLP4 0
INDIRI4
ASGNI4
line 918
;918:	s_graphicsoptions.colordepth.itemnames        = colordepth_names;
ADDRGP4 s_graphicsoptions+1572+76
ADDRGP4 $619
ASGNP4
line 919
;919:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 922
;920:
;921:	// references/modifies "r_fullscreen"
;922:	s_graphicsoptions.fs.generic.type     = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1188
CNSTI4 3
ASGNI4
line 923
;923:	s_graphicsoptions.fs.generic.name	  = "Fullscreen:";
ADDRGP4 s_graphicsoptions+1188+4
ADDRGP4 $834
ASGNP4
line 924
;924:	s_graphicsoptions.fs.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1188+44
CNSTU4 258
ASGNU4
line 925
;925:	s_graphicsoptions.fs.generic.x	      = 400;
ADDRGP4 s_graphicsoptions+1188+12
CNSTI4 400
ASGNI4
line 926
;926:	s_graphicsoptions.fs.generic.y	      = y;
ADDRGP4 s_graphicsoptions+1188+16
ADDRLP4 0
INDIRI4
ASGNI4
line 927
;927:	s_graphicsoptions.fs.itemnames	      = enabled_names;
ADDRGP4 s_graphicsoptions+1188+76
ADDRGP4 $640
ASGNP4
line 928
;928:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 931
;929:
;930:	// references/modifies "r_vertexLight"
;931:	s_graphicsoptions.lighting.generic.type  = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1284
CNSTI4 3
ASGNI4
line 932
;932:	s_graphicsoptions.lighting.generic.name	 = "Lighting:";
ADDRGP4 s_graphicsoptions+1284+4
ADDRGP4 $846
ASGNP4
line 933
;933:	s_graphicsoptions.lighting.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1284+44
CNSTU4 258
ASGNU4
line 934
;934:	s_graphicsoptions.lighting.generic.x	 = 400;
ADDRGP4 s_graphicsoptions+1284+12
CNSTI4 400
ASGNI4
line 935
;935:	s_graphicsoptions.lighting.generic.y	 = y;
ADDRGP4 s_graphicsoptions+1284+16
ADDRLP4 0
INDIRI4
ASGNI4
line 936
;936:	s_graphicsoptions.lighting.itemnames     = lighting_names;
ADDRGP4 s_graphicsoptions+1284+76
ADDRGP4 $616
ASGNP4
line 937
;937:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 940
;938:
;939:	// references/modifies "r_lodBias" & "subdivisions"
;940:	s_graphicsoptions.geometry.generic.type  = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1668
CNSTI4 3
ASGNI4
line 941
;941:	s_graphicsoptions.geometry.generic.name	 = "Geometric Detail:";
ADDRGP4 s_graphicsoptions+1668+4
ADDRGP4 $858
ASGNP4
line 942
;942:	s_graphicsoptions.geometry.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1668+44
CNSTU4 258
ASGNU4
line 943
;943:	s_graphicsoptions.geometry.generic.x	 = 400;
ADDRGP4 s_graphicsoptions+1668+12
CNSTI4 400
ASGNI4
line 944
;944:	s_graphicsoptions.geometry.generic.y	 = y;
ADDRGP4 s_graphicsoptions+1668+16
ADDRLP4 0
INDIRI4
ASGNI4
line 945
;945:	s_graphicsoptions.geometry.itemnames     = quality_names;
ADDRGP4 s_graphicsoptions+1668+76
ADDRGP4 $636
ASGNP4
line 946
;946:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 949
;947:
;948:	// references/modifies "r_picmip"
;949:	s_graphicsoptions.tq.generic.type	= MTYPE_SLIDER;
ADDRGP4 s_graphicsoptions+1112
CNSTI4 1
ASGNI4
line 950
;950:	s_graphicsoptions.tq.generic.name	= "Texture Detail:";
ADDRGP4 s_graphicsoptions+1112+4
ADDRGP4 $870
ASGNP4
line 951
;951:	s_graphicsoptions.tq.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1112+44
CNSTU4 258
ASGNU4
line 952
;952:	s_graphicsoptions.tq.generic.x		= 400;
ADDRGP4 s_graphicsoptions+1112+12
CNSTI4 400
ASGNI4
line 953
;953:	s_graphicsoptions.tq.generic.y		= y;
ADDRGP4 s_graphicsoptions+1112+16
ADDRLP4 0
INDIRI4
ASGNI4
line 954
;954:	s_graphicsoptions.tq.minvalue       = 0;
ADDRGP4 s_graphicsoptions+1112+60
CNSTF4 0
ASGNF4
line 955
;955:	s_graphicsoptions.tq.maxvalue       = 3;
ADDRGP4 s_graphicsoptions+1112+64
CNSTF4 1077936128
ASGNF4
line 956
;956:	s_graphicsoptions.tq.generic.callback = GraphicsOptions_TQEvent;
ADDRGP4 s_graphicsoptions+1112+48
ADDRGP4 GraphicsOptions_TQEvent
ASGNP4
line 957
;957:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 960
;958:
;959:	// references/modifies "r_textureBits"
;960:	s_graphicsoptions.texturebits.generic.type  = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1476
CNSTI4 3
ASGNI4
line 961
;961:	s_graphicsoptions.texturebits.generic.name	= "Texture Quality:";
ADDRGP4 s_graphicsoptions+1476+4
ADDRGP4 $886
ASGNP4
line 962
;962:	s_graphicsoptions.texturebits.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1476+44
CNSTU4 258
ASGNU4
line 963
;963:	s_graphicsoptions.texturebits.generic.x	    = 400;
ADDRGP4 s_graphicsoptions+1476+12
CNSTI4 400
ASGNI4
line 964
;964:	s_graphicsoptions.texturebits.generic.y	    = y;
ADDRGP4 s_graphicsoptions+1476+16
ADDRLP4 0
INDIRI4
ASGNI4
line 965
;965:	s_graphicsoptions.texturebits.itemnames     = tq_names;
ADDRGP4 s_graphicsoptions+1476+76
ADDRGP4 $607
ASGNP4
line 966
;966:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 969
;967:
;968:	// references/modifies "r_textureMode"
;969:	s_graphicsoptions.filter.generic.type   = MTYPE_SPINCONTROL;
ADDRGP4 s_graphicsoptions+1764
CNSTI4 3
ASGNI4
line 970
;970:	s_graphicsoptions.filter.generic.name	= "Texture Filter:";
ADDRGP4 s_graphicsoptions+1764+4
ADDRGP4 $898
ASGNP4
line 971
;971:	s_graphicsoptions.filter.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_graphicsoptions+1764+44
CNSTU4 258
ASGNU4
line 972
;972:	s_graphicsoptions.filter.generic.x	    = 400;
ADDRGP4 s_graphicsoptions+1764+12
CNSTI4 400
ASGNI4
line 973
;973:	s_graphicsoptions.filter.generic.y	    = y;
ADDRGP4 s_graphicsoptions+1764+16
ADDRLP4 0
INDIRI4
ASGNI4
line 974
;974:	s_graphicsoptions.filter.itemnames      = filter_names;
ADDRGP4 s_graphicsoptions+1764+76
ADDRGP4 $633
ASGNP4
line 975
;975:	y += 2*BIGCHAR_HEIGHT;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
ADDI4
ASGNI4
line 977
;976:
;977:	s_graphicsoptions.driverinfo.generic.type     = MTYPE_PTEXT;
ADDRGP4 s_graphicsoptions+1860
CNSTI4 9
ASGNI4
line 978
;978:	s_graphicsoptions.driverinfo.generic.flags    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_graphicsoptions+1860+44
CNSTU4 264
ASGNU4
line 979
;979:	s_graphicsoptions.driverinfo.generic.callback = GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+1860+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 980
;980:	s_graphicsoptions.driverinfo.generic.id       = ID_DRIVERINFO;
ADDRGP4 s_graphicsoptions+1860+8
CNSTI4 105
ASGNI4
line 981
;981:	s_graphicsoptions.driverinfo.generic.x        = 320;
ADDRGP4 s_graphicsoptions+1860+12
CNSTI4 320
ASGNI4
line 982
;982:	s_graphicsoptions.driverinfo.generic.y        = y;
ADDRGP4 s_graphicsoptions+1860+16
ADDRLP4 0
INDIRI4
ASGNI4
line 983
;983:	s_graphicsoptions.driverinfo.string           = "Driver Info";
ADDRGP4 s_graphicsoptions+1860+60
ADDRGP4 $920
ASGNP4
line 984
;984:	s_graphicsoptions.driverinfo.style            = UI_CENTER|UI_SMALLFONT;
ADDRGP4 s_graphicsoptions+1860+64
CNSTI4 17
ASGNI4
line 985
;985:	s_graphicsoptions.driverinfo.color            = color_red;
ADDRGP4 s_graphicsoptions+1860+68
ADDRGP4 color_red
ASGNP4
line 986
;986:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 988
;987:
;988:	s_graphicsoptions.back.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_graphicsoptions+2020
CNSTI4 6
ASGNI4
line 989
;989:	s_graphicsoptions.back.generic.name     = GRAPHICSOPTIONS_BACK0;
ADDRGP4 s_graphicsoptions+2020+4
ADDRGP4 $69
ASGNP4
line 990
;990:	s_graphicsoptions.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_graphicsoptions+2020+44
CNSTU4 260
ASGNU4
line 991
;991:	s_graphicsoptions.back.generic.callback = GraphicsOptions_Event;
ADDRGP4 s_graphicsoptions+2020+48
ADDRGP4 GraphicsOptions_Event
ASGNP4
line 992
;992:	s_graphicsoptions.back.generic.id	    = ID_BACK2;
ADDRGP4 s_graphicsoptions+2020+8
CNSTI4 101
ASGNI4
line 993
;993:	s_graphicsoptions.back.generic.x		= 0;
ADDRGP4 s_graphicsoptions+2020+12
CNSTI4 0
ASGNI4
line 994
;994:	s_graphicsoptions.back.generic.y		= 480-64;
ADDRGP4 s_graphicsoptions+2020+16
CNSTI4 416
ASGNI4
line 995
;995:	s_graphicsoptions.back.width  		    = 128;
ADDRGP4 s_graphicsoptions+2020+76
CNSTI4 128
ASGNI4
line 996
;996:	s_graphicsoptions.back.height  		    = 64;
ADDRGP4 s_graphicsoptions+2020+80
CNSTI4 64
ASGNI4
line 997
;997:	s_graphicsoptions.back.focuspic         = GRAPHICSOPTIONS_BACK1;
ADDRGP4 s_graphicsoptions+2020+60
ADDRGP4 $70
ASGNP4
line 999
;998:
;999:	s_graphicsoptions.apply.generic.type     = MTYPE_BITMAP;
ADDRGP4 s_graphicsoptions+1932
CNSTI4 6
ASGNI4
line 1000
;1000:	s_graphicsoptions.apply.generic.name     = GRAPHICSOPTIONS_ACCEPT0;
ADDRGP4 s_graphicsoptions+1932+4
ADDRGP4 $947
ASGNP4
line 1001
;1001:	s_graphicsoptions.apply.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_HIDDEN|QMF_INACTIVE;
ADDRGP4 s_graphicsoptions+1932+44
CNSTU4 20752
ASGNU4
line 1002
;1002:	s_graphicsoptions.apply.generic.callback = GraphicsOptions_ApplyChanges;
ADDRGP4 s_graphicsoptions+1932+48
ADDRGP4 GraphicsOptions_ApplyChanges
ASGNP4
line 1003
;1003:	s_graphicsoptions.apply.generic.x        = 640;
ADDRGP4 s_graphicsoptions+1932+12
CNSTI4 640
ASGNI4
line 1004
;1004:	s_graphicsoptions.apply.generic.y        = 480-64;
ADDRGP4 s_graphicsoptions+1932+16
CNSTI4 416
ASGNI4
line 1005
;1005:	s_graphicsoptions.apply.width  		     = 128;
ADDRGP4 s_graphicsoptions+1932+76
CNSTI4 128
ASGNI4
line 1006
;1006:	s_graphicsoptions.apply.height  		 = 64;
ADDRGP4 s_graphicsoptions+1932+80
CNSTI4 64
ASGNI4
line 1007
;1007:	s_graphicsoptions.apply.focuspic         = GRAPHICSOPTIONS_ACCEPT1;
ADDRGP4 s_graphicsoptions+1932+60
ADDRGP4 $962
ASGNP4
line 1009
;1008:
;1009:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.banner );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1010
;1010:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.framel );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1011
;1011:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.framer );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1013
;1012:
;1013:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.graphics );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1014
;1014:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.display );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+608
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1015
;1015:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.sound );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1016
;1016:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.network );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+752
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1018
;1017:
;1018:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.list );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+824
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1019
;1019:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.driver );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1016
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1020
;1020:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.allow_extensions );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1380
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1021
;1021:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.mode );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+920
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1022
;1022:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.colordepth );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1572
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1023
;1023:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.fs );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1188
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1024
;1024:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.lighting );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1284
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1025
;1025:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.geometry );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1668
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1026
;1026:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.tq );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1112
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1027
;1027:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.texturebits );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1476
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1028
;1028:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.filter );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1764
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1029
;1029:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.driverinfo );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1860
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1031
;1030:
;1031:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.back );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+2020
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1032
;1032:	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.apply );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+1932
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1034
;1033:
;1034:	GraphicsOptions_SetMenuItems();
ADDRGP4 GraphicsOptions_SetMenuItems
CALLV
pop
line 1035
;1035:	GraphicsOptions_GetInitialVideo();
ADDRGP4 GraphicsOptions_GetInitialVideo
CALLV
pop
line 1037
;1036:
;1037:	if ( uis.glconfig.driverType == GLDRV_ICD &&
ADDRGP4 uis+56+11284
INDIRI4
CNSTI4 0
NEI4 $984
ADDRGP4 uis+56+11288
INDIRI4
CNSTI4 1
NEI4 $984
line 1039
;1038:		 uis.glconfig.hardwareType == GLHW_3DFX_2D3D )
;1039:	{
line 1040
;1040:		s_graphicsoptions.driver.generic.flags |= QMF_HIDDEN|QMF_INACTIVE;
ADDRLP4 8
ADDRGP4 s_graphicsoptions+1016+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 1041
;1041:	}
LABELV $984
line 1042
;1042:}
LABELV $603
endproc GraphicsOptions_MenuInit 12 12
export GraphicsOptions_Cache
proc GraphicsOptions_Cache 0 4
line 1050
;1043:
;1044:
;1045:/*
;1046:=================
;1047:GraphicsOptions_Cache
;1048:=================
;1049:*/
;1050:void GraphicsOptions_Cache( void ) {
line 1051
;1051:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_FRAMEL );
ADDRGP4 $67
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1052
;1052:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_FRAMER );
ADDRGP4 $68
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1053
;1053:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_BACK0 );
ADDRGP4 $69
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1054
;1054:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_BACK1 );
ADDRGP4 $70
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1055
;1055:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_ACCEPT0 );
ADDRGP4 $947
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1056
;1056:	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_ACCEPT1 );
ADDRGP4 $962
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1057
;1057:}
LABELV $992
endproc GraphicsOptions_Cache 0 4
export UI_GraphicsOptionsMenu
proc UI_GraphicsOptionsMenu 0 8
line 1065
;1058:
;1059:
;1060:/*
;1061:=================
;1062:UI_GraphicsOptionsMenu
;1063:=================
;1064:*/
;1065:void UI_GraphicsOptionsMenu( void ) {
line 1066
;1066:	GraphicsOptions_MenuInit();
ADDRGP4 GraphicsOptions_MenuInit
CALLV
pop
line 1067
;1067:	UI_PushMenu( &s_graphicsoptions.menu );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 1068
;1068:	Menu_SetCursorToItem( &s_graphicsoptions.menu, &s_graphicsoptions.graphics );
ADDRGP4 s_graphicsoptions
ARGP4
ADDRGP4 s_graphicsoptions+536
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 1069
;1069:}
LABELV $993
endproc UI_GraphicsOptionsMenu 0 8
bss
align 4
LABELV s_graphicsoptions
skip 2108
align 4
LABELV s_ivo
skip 40
align 4
LABELV s_driverinfo
skip 1908
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
LABELV $962
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
LABELV $947
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
LABELV $920
byte 1 68
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 0
align 1
LABELV $898
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 70
byte 1 105
byte 1 108
byte 1 116
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $886
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 81
byte 1 117
byte 1 97
byte 1 108
byte 1 105
byte 1 116
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $870
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 68
byte 1 101
byte 1 116
byte 1 97
byte 1 105
byte 1 108
byte 1 58
byte 1 0
align 1
LABELV $858
byte 1 71
byte 1 101
byte 1 111
byte 1 109
byte 1 101
byte 1 116
byte 1 114
byte 1 105
byte 1 99
byte 1 32
byte 1 68
byte 1 101
byte 1 116
byte 1 97
byte 1 105
byte 1 108
byte 1 58
byte 1 0
align 1
LABELV $846
byte 1 76
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 0
align 1
LABELV $834
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 58
byte 1 0
align 1
LABELV $822
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 32
byte 1 68
byte 1 101
byte 1 112
byte 1 116
byte 1 104
byte 1 58
byte 1 0
align 1
LABELV $806
byte 1 86
byte 1 105
byte 1 100
byte 1 101
byte 1 111
byte 1 32
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $794
byte 1 71
byte 1 76
byte 1 32
byte 1 69
byte 1 120
byte 1 116
byte 1 101
byte 1 110
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $775
byte 1 71
byte 1 76
byte 1 32
byte 1 68
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $759
byte 1 71
byte 1 114
byte 1 97
byte 1 112
byte 1 104
byte 1 105
byte 1 99
byte 1 115
byte 1 32
byte 1 83
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $751
byte 1 78
byte 1 69
byte 1 84
byte 1 87
byte 1 79
byte 1 82
byte 1 75
byte 1 0
align 1
LABELV $733
byte 1 83
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 0
align 1
LABELV $715
byte 1 68
byte 1 73
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 0
align 1
LABELV $697
byte 1 71
byte 1 82
byte 1 65
byte 1 80
byte 1 72
byte 1 73
byte 1 67
byte 1 83
byte 1 0
align 1
LABELV $653
byte 1 83
byte 1 89
byte 1 83
byte 1 84
byte 1 69
byte 1 77
byte 1 32
byte 1 83
byte 1 69
byte 1 84
byte 1 85
byte 1 80
byte 1 0
align 1
LABELV $642
byte 1 79
byte 1 110
byte 1 0
align 1
LABELV $641
byte 1 79
byte 1 102
byte 1 102
byte 1 0
align 1
LABELV $639
byte 1 72
byte 1 105
byte 1 103
byte 1 104
byte 1 0
align 1
LABELV $638
byte 1 77
byte 1 101
byte 1 100
byte 1 105
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $637
byte 1 76
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $635
byte 1 84
byte 1 114
byte 1 105
byte 1 108
byte 1 105
byte 1 110
byte 1 101
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $634
byte 1 66
byte 1 105
byte 1 108
byte 1 105
byte 1 110
byte 1 101
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $632
byte 1 56
byte 1 53
byte 1 54
byte 1 120
byte 1 52
byte 1 56
byte 1 48
byte 1 32
byte 1 119
byte 1 105
byte 1 100
byte 1 101
byte 1 32
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $631
byte 1 50
byte 1 48
byte 1 52
byte 1 56
byte 1 120
byte 1 49
byte 1 53
byte 1 51
byte 1 54
byte 1 0
align 1
LABELV $630
byte 1 49
byte 1 54
byte 1 48
byte 1 48
byte 1 120
byte 1 49
byte 1 50
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $629
byte 1 49
byte 1 50
byte 1 56
byte 1 48
byte 1 120
byte 1 49
byte 1 48
byte 1 50
byte 1 52
byte 1 0
align 1
LABELV $628
byte 1 49
byte 1 49
byte 1 53
byte 1 50
byte 1 120
byte 1 56
byte 1 54
byte 1 52
byte 1 0
align 1
LABELV $627
byte 1 49
byte 1 48
byte 1 50
byte 1 52
byte 1 120
byte 1 55
byte 1 54
byte 1 56
byte 1 0
align 1
LABELV $626
byte 1 57
byte 1 54
byte 1 48
byte 1 120
byte 1 55
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $625
byte 1 56
byte 1 48
byte 1 48
byte 1 120
byte 1 54
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $624
byte 1 54
byte 1 52
byte 1 48
byte 1 120
byte 1 52
byte 1 56
byte 1 48
byte 1 0
align 1
LABELV $623
byte 1 53
byte 1 49
byte 1 50
byte 1 120
byte 1 51
byte 1 56
byte 1 52
byte 1 0
align 1
LABELV $622
byte 1 52
byte 1 48
byte 1 48
byte 1 120
byte 1 51
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $621
byte 1 51
byte 1 50
byte 1 48
byte 1 120
byte 1 50
byte 1 52
byte 1 48
byte 1 0
align 1
LABELV $618
byte 1 86
byte 1 101
byte 1 114
byte 1 116
byte 1 101
byte 1 120
byte 1 0
align 1
LABELV $617
byte 1 76
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $615
byte 1 67
byte 1 117
byte 1 115
byte 1 116
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $614
byte 1 70
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $613
byte 1 70
byte 1 97
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $612
byte 1 78
byte 1 111
byte 1 114
byte 1 109
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $611
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
byte 1 0
align 1
LABELV $609
byte 1 51
byte 1 50
byte 1 32
byte 1 98
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $608
byte 1 49
byte 1 54
byte 1 32
byte 1 98
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $606
byte 1 86
byte 1 111
byte 1 111
byte 1 100
byte 1 111
byte 1 111
byte 1 0
align 1
LABELV $605
byte 1 68
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $456
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
LABELV $455
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
LABELV $454
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
LABELV $453
byte 1 114
byte 1 95
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 77
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $444
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
LABELV $443
byte 1 114
byte 1 95
byte 1 108
byte 1 111
byte 1 100
byte 1 66
byte 1 105
byte 1 97
byte 1 115
byte 1 0
align 1
LABELV $436
byte 1 114
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 101
byte 1 120
byte 1 76
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $433
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
LABELV $432
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
LABELV $431
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
LABELV $423
byte 1 114
byte 1 95
byte 1 103
byte 1 108
byte 1 68
byte 1 114
byte 1 105
byte 1 118
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $420
byte 1 114
byte 1 95
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $417
byte 1 114
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $414
byte 1 114
byte 1 95
byte 1 97
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 69
byte 1 120
byte 1 116
byte 1 101
byte 1 110
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $411
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
LABELV $408
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
LABELV $212
byte 1 51
byte 1 100
byte 1 102
byte 1 120
byte 1 118
byte 1 103
byte 1 108
byte 1 0
align 1
LABELV $211
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 103
byte 1 108
byte 1 51
byte 1 50
byte 1 0
align 1
LABELV $126
byte 1 68
byte 1 82
byte 1 73
byte 1 86
byte 1 69
byte 1 82
byte 1 32
byte 1 73
byte 1 78
byte 1 70
byte 1 79
byte 1 0
align 1
LABELV $88
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
LABELV $82
byte 1 69
byte 1 88
byte 1 84
byte 1 69
byte 1 78
byte 1 83
byte 1 73
byte 1 79
byte 1 78
byte 1 83
byte 1 0
align 1
LABELV $81
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
byte 1 0
align 1
LABELV $80
byte 1 86
byte 1 69
byte 1 78
byte 1 68
byte 1 79
byte 1 82
byte 1 0
align 1
LABELV $70
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
LABELV $69
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
LABELV $68
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
LABELV $67
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
