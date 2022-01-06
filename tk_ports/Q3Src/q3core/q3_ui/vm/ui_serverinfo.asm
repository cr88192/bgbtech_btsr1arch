data
align 4
LABELV serverinfo_artlist
address $67
address $68
address $69
address $70
byte 4 0
export Favorites_Add
code
proc Favorites_Add 276 12
file "../ui_serverinfo.c"
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
;23:#include "ui_local.h"
;24:
;25:#define SERVERINFO_FRAMEL	"menu/art/frame2_l"
;26:#define SERVERINFO_FRAMER	"menu/art/frame1_r"
;27:#define SERVERINFO_BACK0	"menu/art/back_0"
;28:#define SERVERINFO_BACK1	"menu/art/back_1"
;29:
;30:static char* serverinfo_artlist[] =
;31:{
;32:	SERVERINFO_FRAMEL,	
;33:	SERVERINFO_FRAMER,
;34:	SERVERINFO_BACK0,
;35:	SERVERINFO_BACK1,
;36:	NULL
;37:};
;38:
;39:#define ID_ADD	 100
;40:#define ID_BACK	 101
;41:
;42:typedef struct
;43:{
;44:	menuframework_s	menu;
;45:	menutext_s		banner;
;46:	menubitmap_s	framel;
;47:	menubitmap_s	framer;
;48:	menubitmap_s	back;
;49:	menutext_s		add;
;50:	char			info[MAX_INFO_STRING];
;51:	int				numlines;
;52:} serverinfo_t;
;53:
;54:static serverinfo_t	s_serverinfo;
;55:
;56:
;57:/*
;58:=================
;59:Favorites_Add
;60:
;61:Add current server to favorites
;62:=================
;63:*/
;64:void Favorites_Add( void )
;65:{
line 71
;66:	char	adrstr[128];
;67:	char	serverbuff[128];
;68:	int		i;
;69:	int		best;
;70:
;71:	trap_Cvar_VariableStringBuffer( "cl_currentServerAddress", serverbuff, sizeof(serverbuff) );
ADDRGP4 $73
ARGP4
ADDRLP4 136
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 72
;72:	if (!serverbuff[0])
ADDRLP4 136
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $74
line 73
;73:		return;
ADDRGP4 $72
JUMPV
LABELV $74
line 75
;74:
;75:	best = 0;
ADDRLP4 132
CNSTI4 0
ASGNI4
line 76
;76:	for (i=0; i<MAX_FAVORITESERVERS; i++)
ADDRLP4 128
CNSTI4 0
ASGNI4
LABELV $76
line 77
;77:	{
line 78
;78:		trap_Cvar_VariableStringBuffer( va("server%d",i+1), adrstr, sizeof(adrstr) );
ADDRGP4 $80
ARGP4
ADDRLP4 128
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 264
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 264
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 79
;79:		if (!Q_stricmp(serverbuff,adrstr))
ADDRLP4 136
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 268
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 0
NEI4 $81
line 80
;80:		{
line 82
;81:			// already in list
;82:			return;
ADDRGP4 $72
JUMPV
LABELV $81
line 86
;83:		}
;84:		
;85:		// use first empty or non-numeric available slot
;86:		if ((adrstr[0]  < '0' || adrstr[0] > '9' ) && !best)
ADDRLP4 272
ADDRLP4 0
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 272
INDIRI4
CNSTI4 48
LTI4 $85
ADDRLP4 272
INDIRI4
CNSTI4 57
LEI4 $83
LABELV $85
ADDRLP4 132
INDIRI4
CNSTI4 0
NEI4 $83
line 87
;87:			best = i+1;
ADDRLP4 132
ADDRLP4 128
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $83
line 88
;88:	}
LABELV $77
line 76
ADDRLP4 128
ADDRLP4 128
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 128
INDIRI4
CNSTI4 16
LTI4 $76
line 90
;89:
;90:	if (best)
ADDRLP4 132
INDIRI4
CNSTI4 0
EQI4 $86
line 91
;91:		trap_Cvar_Set( va("server%d",best), serverbuff);
ADDRGP4 $80
ARGP4
ADDRLP4 132
INDIRI4
ARGI4
ADDRLP4 264
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 264
INDIRP4
ARGP4
ADDRLP4 136
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
LABELV $86
line 92
;92:}
LABELV $72
endproc Favorites_Add 276 12
proc ServerInfo_Event 8 0
line 101
;93:
;94:
;95:/*
;96:=================
;97:ServerInfo_Event
;98:=================
;99:*/
;100:static void ServerInfo_Event( void* ptr, int event )
;101:{
line 102
;102:	switch (((menucommon_s*)ptr)->id)
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
EQI4 $92
ADDRLP4 0
INDIRI4
CNSTI4 101
EQI4 $95
ADDRGP4 $89
JUMPV
line 103
;103:	{
LABELV $92
line 105
;104:		case ID_ADD:
;105:			if (event != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $93
line 106
;106:				break;
ADDRGP4 $90
JUMPV
LABELV $93
line 108
;107:		
;108:			Favorites_Add();
ADDRGP4 Favorites_Add
CALLV
pop
line 109
;109:			UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 110
;110:			break;
ADDRGP4 $90
JUMPV
LABELV $95
line 113
;111:
;112:		case ID_BACK:
;113:			if (event != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $96
line 114
;114:				break;
ADDRGP4 $90
JUMPV
LABELV $96
line 116
;115:
;116:			UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 117
;117:			break;
LABELV $89
LABELV $90
line 119
;118:	}
;119:}
LABELV $88
endproc ServerInfo_Event 8 0
proc ServerInfo_MenuDraw 2056 20
line 127
;120:
;121:/*
;122:=================
;123:ServerInfo_MenuDraw
;124:=================
;125:*/
;126:static void ServerInfo_MenuDraw( void )
;127:{
line 133
;128:	const char		*s;
;129:	char			key[MAX_INFO_KEY];
;130:	char			value[MAX_INFO_VALUE];
;131:	int				y;
;132:
;133:	y = SCREEN_HEIGHT/2 - s_serverinfo.numlines*(SMALLCHAR_HEIGHT)/2 - 20;
ADDRLP4 1024
CNSTI4 240
ADDRGP4 s_serverinfo+1720
INDIRI4
CNSTI4 4
LSHI4
CNSTI4 2
DIVI4
SUBI4
CNSTI4 20
SUBI4
ASGNI4
line 134
;134:	s = s_serverinfo.info;
ADDRLP4 1028
ADDRGP4 s_serverinfo+696
ASGNP4
ADDRGP4 $102
JUMPV
LABELV $101
line 135
;135:	while ( s ) {
line 136
;136:		Info_NextPair( &s, key, value );
ADDRLP4 1028
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1032
ARGP4
ADDRGP4 Info_NextPair
CALLV
pop
line 137
;137:		if ( !key[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $104
line 138
;138:			break;
ADDRGP4 $103
JUMPV
LABELV $104
line 141
;139:		}
;140:
;141:		Q_strcat( key, MAX_INFO_KEY, ":" ); 
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $106
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 143
;142:
;143:		UI_DrawString(SCREEN_WIDTH*0.50 - 8,y,key,UI_RIGHT|UI_SMALLFONT,color_red);
CNSTI4 312
ARGI4
ADDRLP4 1024
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 18
ARGI4
ADDRGP4 color_red
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 144
;144:		UI_DrawString(SCREEN_WIDTH*0.50 + 8,y,value,UI_LEFT|UI_SMALLFONT,text_color_normal);
CNSTI4 328
ARGI4
ADDRLP4 1024
INDIRI4
ARGI4
ADDRLP4 1032
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 text_color_normal
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 146
;145:
;146:		y += SMALLCHAR_HEIGHT;
ADDRLP4 1024
ADDRLP4 1024
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 147
;147:	}
LABELV $102
line 135
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $101
LABELV $103
line 149
;148:
;149:	Menu_Draw( &s_serverinfo.menu );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 150
;150:}
LABELV $98
endproc ServerInfo_MenuDraw 2056 20
proc ServerInfo_MenuKey 4 8
line 158
;151:
;152:/*
;153:=================
;154:ServerInfo_MenuKey
;155:=================
;156:*/
;157:static sfxHandle_t ServerInfo_MenuKey( int key )
;158:{
line 159
;159:	return ( Menu_DefaultKey( &s_serverinfo.menu, key ) );
ADDRGP4 s_serverinfo
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $107
endproc ServerInfo_MenuKey 4 8
export ServerInfo_Cache
proc ServerInfo_Cache 4 4
line 168
;160:}
;161:
;162:/*
;163:=================
;164:ServerInfo_Cache
;165:=================
;166:*/
;167:void ServerInfo_Cache( void )
;168:{
line 172
;169:	int	i;
;170:
;171:	// touch all our pics
;172:	for (i=0; ;i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $109
line 173
;173:	{
line 174
;174:		if (!serverinfo_artlist[i])
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 serverinfo_artlist
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $113
line 175
;175:			break;
ADDRGP4 $111
JUMPV
LABELV $113
line 176
;176:		trap_R_RegisterShaderNoMip(serverinfo_artlist[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 serverinfo_artlist
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 177
;177:	}
LABELV $110
line 172
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $109
JUMPV
LABELV $111
line 178
;178:}
LABELV $108
endproc ServerInfo_Cache 4 4
export UI_ServerInfoMenu
proc UI_ServerInfoMenu 2060 12
line 186
;179:
;180:/*
;181:=================
;182:UI_ServerInfoMenu
;183:=================
;184:*/
;185:void UI_ServerInfoMenu( void )
;186:{
line 192
;187:	const char		*s;
;188:	char			key[MAX_INFO_KEY];
;189:	char			value[MAX_INFO_VALUE];
;190:
;191:	// zero set all our globals
;192:	memset( &s_serverinfo, 0 ,sizeof(serverinfo_t) );
ADDRGP4 s_serverinfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1724
ARGI4
ADDRGP4 memset
CALLP4
pop
line 194
;193:
;194:	ServerInfo_Cache();
ADDRGP4 ServerInfo_Cache
CALLV
pop
line 196
;195:
;196:	s_serverinfo.menu.draw       = ServerInfo_MenuDraw;
ADDRGP4 s_serverinfo+268
ADDRGP4 ServerInfo_MenuDraw
ASGNP4
line 197
;197:	s_serverinfo.menu.key        = ServerInfo_MenuKey;
ADDRGP4 s_serverinfo+272
ADDRGP4 ServerInfo_MenuKey
ASGNP4
line 198
;198:	s_serverinfo.menu.wrapAround = qtrue;
ADDRGP4 s_serverinfo+276
CNSTI4 1
ASGNI4
line 199
;199:	s_serverinfo.menu.fullscreen = qtrue;
ADDRGP4 s_serverinfo+280
CNSTI4 1
ASGNI4
line 201
;200:
;201:	s_serverinfo.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_serverinfo+288
CNSTI4 10
ASGNI4
line 202
;202:	s_serverinfo.banner.generic.x	  = 320;
ADDRGP4 s_serverinfo+288+12
CNSTI4 320
ASGNI4
line 203
;203:	s_serverinfo.banner.generic.y	  = 16;
ADDRGP4 s_serverinfo+288+16
CNSTI4 16
ASGNI4
line 204
;204:	s_serverinfo.banner.string		  = "SERVER INFO";
ADDRGP4 s_serverinfo+288+60
ADDRGP4 $127
ASGNP4
line 205
;205:	s_serverinfo.banner.color	      = color_white;
ADDRGP4 s_serverinfo+288+68
ADDRGP4 color_white
ASGNP4
line 206
;206:	s_serverinfo.banner.style	      = UI_CENTER;
ADDRGP4 s_serverinfo+288+64
CNSTI4 1
ASGNI4
line 208
;207:
;208:	s_serverinfo.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_serverinfo+360
CNSTI4 6
ASGNI4
line 209
;209:	s_serverinfo.framel.generic.name  = SERVERINFO_FRAMEL;
ADDRGP4 s_serverinfo+360+4
ADDRGP4 $67
ASGNP4
line 210
;210:	s_serverinfo.framel.generic.flags = QMF_INACTIVE;
ADDRGP4 s_serverinfo+360+44
CNSTU4 16384
ASGNU4
line 211
;211:	s_serverinfo.framel.generic.x	  = 0;  
ADDRGP4 s_serverinfo+360+12
CNSTI4 0
ASGNI4
line 212
;212:	s_serverinfo.framel.generic.y	  = 78;
ADDRGP4 s_serverinfo+360+16
CNSTI4 78
ASGNI4
line 213
;213:	s_serverinfo.framel.width  	      = 256;
ADDRGP4 s_serverinfo+360+76
CNSTI4 256
ASGNI4
line 214
;214:	s_serverinfo.framel.height  	  = 329;
ADDRGP4 s_serverinfo+360+80
CNSTI4 329
ASGNI4
line 216
;215:
;216:	s_serverinfo.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_serverinfo+448
CNSTI4 6
ASGNI4
line 217
;217:	s_serverinfo.framer.generic.name  = SERVERINFO_FRAMER;
ADDRGP4 s_serverinfo+448+4
ADDRGP4 $68
ASGNP4
line 218
;218:	s_serverinfo.framer.generic.flags = QMF_INACTIVE;
ADDRGP4 s_serverinfo+448+44
CNSTU4 16384
ASGNU4
line 219
;219:	s_serverinfo.framer.generic.x	  = 376;
ADDRGP4 s_serverinfo+448+12
CNSTI4 376
ASGNI4
line 220
;220:	s_serverinfo.framer.generic.y	  = 76;
ADDRGP4 s_serverinfo+448+16
CNSTI4 76
ASGNI4
line 221
;221:	s_serverinfo.framer.width  	      = 256;
ADDRGP4 s_serverinfo+448+76
CNSTI4 256
ASGNI4
line 222
;222:	s_serverinfo.framer.height  	  = 334;
ADDRGP4 s_serverinfo+448+80
CNSTI4 334
ASGNI4
line 224
;223:
;224:	s_serverinfo.add.generic.type	  = MTYPE_PTEXT;
ADDRGP4 s_serverinfo+624
CNSTI4 9
ASGNI4
line 225
;225:	s_serverinfo.add.generic.flags    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_serverinfo+624+44
CNSTU4 264
ASGNU4
line 226
;226:	s_serverinfo.add.generic.callback = ServerInfo_Event;
ADDRGP4 s_serverinfo+624+48
ADDRGP4 ServerInfo_Event
ASGNP4
line 227
;227:	s_serverinfo.add.generic.id	      = ID_ADD;
ADDRGP4 s_serverinfo+624+8
CNSTI4 100
ASGNI4
line 228
;228:	s_serverinfo.add.generic.x		  = 320;
ADDRGP4 s_serverinfo+624+12
CNSTI4 320
ASGNI4
line 229
;229:	s_serverinfo.add.generic.y		  = 371;
ADDRGP4 s_serverinfo+624+16
CNSTI4 371
ASGNI4
line 230
;230:	s_serverinfo.add.string  		  = "ADD TO FAVORITES";
ADDRGP4 s_serverinfo+624+60
ADDRGP4 $171
ASGNP4
line 231
;231:	s_serverinfo.add.style  		  = UI_CENTER|UI_SMALLFONT;
ADDRGP4 s_serverinfo+624+64
CNSTI4 17
ASGNI4
line 232
;232:	s_serverinfo.add.color			  =	color_red;
ADDRGP4 s_serverinfo+624+68
ADDRGP4 color_red
ASGNP4
line 233
;233:	if( trap_Cvar_VariableValue( "sv_running" ) ) {
ADDRGP4 $178
ARGP4
ADDRLP4 2052
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 2052
INDIRF4
CNSTF4 0
EQF4 $176
line 234
;234:		s_serverinfo.add.generic.flags |= QMF_GRAYED;
ADDRLP4 2056
ADDRGP4 s_serverinfo+624+44
ASGNP4
ADDRLP4 2056
INDIRP4
ADDRLP4 2056
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 235
;235:	}
LABELV $176
line 237
;236:
;237:	s_serverinfo.back.generic.type	   = MTYPE_BITMAP;
ADDRGP4 s_serverinfo+536
CNSTI4 6
ASGNI4
line 238
;238:	s_serverinfo.back.generic.name     = SERVERINFO_BACK0;
ADDRGP4 s_serverinfo+536+4
ADDRGP4 $69
ASGNP4
line 239
;239:	s_serverinfo.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_serverinfo+536+44
CNSTU4 260
ASGNU4
line 240
;240:	s_serverinfo.back.generic.callback = ServerInfo_Event;
ADDRGP4 s_serverinfo+536+48
ADDRGP4 ServerInfo_Event
ASGNP4
line 241
;241:	s_serverinfo.back.generic.id	   = ID_BACK;
ADDRGP4 s_serverinfo+536+8
CNSTI4 101
ASGNI4
line 242
;242:	s_serverinfo.back.generic.x		   = 0;
ADDRGP4 s_serverinfo+536+12
CNSTI4 0
ASGNI4
line 243
;243:	s_serverinfo.back.generic.y		   = 480-64;
ADDRGP4 s_serverinfo+536+16
CNSTI4 416
ASGNI4
line 244
;244:	s_serverinfo.back.width  		   = 128;
ADDRGP4 s_serverinfo+536+76
CNSTI4 128
ASGNI4
line 245
;245:	s_serverinfo.back.height  		   = 64;
ADDRGP4 s_serverinfo+536+80
CNSTI4 64
ASGNI4
line 246
;246:	s_serverinfo.back.focuspic         = SERVERINFO_BACK1;
ADDRGP4 s_serverinfo+536+60
ADDRGP4 $70
ASGNP4
line 248
;247:
;248:	trap_GetConfigString( CS_SERVERINFO, s_serverinfo.info, MAX_INFO_STRING );
CNSTI4 0
ARGI4
ADDRGP4 s_serverinfo+696
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 250
;249:
;250:	s_serverinfo.numlines = 0;
ADDRGP4 s_serverinfo+1720
CNSTI4 0
ASGNI4
line 251
;251:	s = s_serverinfo.info;
ADDRLP4 0
ADDRGP4 s_serverinfo+696
ASGNP4
ADDRGP4 $204
JUMPV
LABELV $203
line 252
;252:	while ( s ) {
line 253
;253:		Info_NextPair( &s, key, value );
ADDRLP4 0
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1028
ARGP4
ADDRGP4 Info_NextPair
CALLV
pop
line 254
;254:		if ( !key[0] ) {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $206
line 255
;255:			break;
ADDRGP4 $205
JUMPV
LABELV $206
line 257
;256:		}
;257:		s_serverinfo.numlines++;
ADDRLP4 2056
ADDRGP4 s_serverinfo+1720
ASGNP4
ADDRLP4 2056
INDIRP4
ADDRLP4 2056
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 258
;258:	}
LABELV $204
line 252
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $203
LABELV $205
line 260
;259:
;260:	if (s_serverinfo.numlines > 16)
ADDRGP4 s_serverinfo+1720
INDIRI4
CNSTI4 16
LEI4 $209
line 261
;261:		s_serverinfo.numlines = 16;
ADDRGP4 s_serverinfo+1720
CNSTI4 16
ASGNI4
LABELV $209
line 263
;262:
;263:	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.banner );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 s_serverinfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 264
;264:	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.framel );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 s_serverinfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 265
;265:	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.framer );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 s_serverinfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 266
;266:	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.add );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 s_serverinfo+624
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 267
;267:	Menu_AddItem( &s_serverinfo.menu, (void*) &s_serverinfo.back );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 s_serverinfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 269
;268:
;269:	UI_PushMenu( &s_serverinfo.menu );
ADDRGP4 s_serverinfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 270
;270:}
LABELV $115
endproc UI_ServerInfoMenu 2060 12
bss
align 4
LABELV s_serverinfo
skip 1724
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
LABELV $178
byte 1 115
byte 1 118
byte 1 95
byte 1 114
byte 1 117
byte 1 110
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $171
byte 1 65
byte 1 68
byte 1 68
byte 1 32
byte 1 84
byte 1 79
byte 1 32
byte 1 70
byte 1 65
byte 1 86
byte 1 79
byte 1 82
byte 1 73
byte 1 84
byte 1 69
byte 1 83
byte 1 0
align 1
LABELV $127
byte 1 83
byte 1 69
byte 1 82
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
LABELV $106
byte 1 58
byte 1 0
align 1
LABELV $80
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $73
byte 1 99
byte 1 108
byte 1 95
byte 1 99
byte 1 117
byte 1 114
byte 1 114
byte 1 101
byte 1 110
byte 1 116
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 65
byte 1 100
byte 1 100
byte 1 114
byte 1 101
byte 1 115
byte 1 115
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
