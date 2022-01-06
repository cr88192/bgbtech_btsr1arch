data
align 4
LABELV cinematics
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
code
proc UI_CinematicsMenu_BackEvent 0 0
file "../ui_cinematics.c"
line 84
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
;25:
;26:#define ART_BACK0		"menu/art/back_0"
;27:#define ART_BACK1		"menu/art/back_1"	
;28:#define ART_FRAMEL		"menu/art/frame2_l"
;29:#define ART_FRAMER		"menu/art/frame1_r"
;30:
;31:#define VERTICAL_SPACING	30
;32:
;33:#define ID_BACK			10
;34:#define ID_CIN_IDLOGO	11
;35:#define ID_CIN_INTRO	12
;36:#define ID_CIN_TIER1	13
;37:#define ID_CIN_TIER2	14
;38:#define ID_CIN_TIER3	15
;39:#define ID_CIN_TIER4	16
;40:#define ID_CIN_TIER5	17
;41:#define ID_CIN_TIER6	18
;42:#define ID_CIN_TIER7	19
;43:#define ID_CIN_END		20
;44:
;45:
;46:typedef struct {
;47:	menuframework_s	menu;
;48:	menutext_s		banner;
;49:	menubitmap_s	framel;
;50:	menubitmap_s	framer;
;51:	menutext_s		cin_idlogo;
;52:	menutext_s		cin_intro;
;53:	menutext_s		cin_tier1;
;54:	menutext_s		cin_tier2;
;55:	menutext_s		cin_tier3;
;56:	menutext_s		cin_tier4;
;57:	menutext_s		cin_tier5;
;58:	menutext_s		cin_tier6;
;59:	menutext_s		cin_tier7;
;60:	menutext_s		cin_end;
;61:	menubitmap_s	back;
;62:} cinematicsMenuInfo_t;
;63:
;64:static cinematicsMenuInfo_t	cinematicsMenuInfo;
;65:
;66:static char *cinematics[] = {
;67:	"idlogo",
;68:	"intro",
;69:	"tier1",
;70:	"tier2",
;71:	"tier3",
;72:	"tier4",
;73:	"tier5",
;74:	"tier6",
;75:	"tier7",
;76:	"end"
;77:};
;78:
;79:/*
;80:===============
;81:UI_CinematicsMenu_BackEvent
;82:===============
;83:*/
;84:static void UI_CinematicsMenu_BackEvent( void *ptr, int event ) {
line 85
;85:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $79
line 86
;86:		return;
ADDRGP4 $78
JUMPV
LABELV $79
line 88
;87:	}
;88:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 89
;89:}
LABELV $78
endproc UI_CinematicsMenu_BackEvent 0 0
proc UI_CinematicsMenu_Event 12 8
line 97
;90:
;91:
;92:/*
;93:===============
;94:UI_CinematicsMenu_Event
;95:===============
;96:*/
;97:static void UI_CinematicsMenu_Event( void *ptr, int event ) {
line 100
;98:	int		n;
;99:
;100:	if (event != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $82
line 101
;101:		return;
ADDRGP4 $81
JUMPV
LABELV $82
line 103
;102:
;103:	n = ((menucommon_s*)ptr)->id - ID_CIN_IDLOGO;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 11
SUBI4
ASGNI4
line 104
;104:	trap_Cvar_Set( "nextmap", va( "ui_cinematics %i", n ) );
ADDRGP4 $85
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $84
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 105
;105:	if( uis.demoversion && ((menucommon_s*)ptr)->id == ID_CIN_END ) {
ADDRGP4 uis+11440
INDIRI4
CNSTI4 0
EQI4 $86
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 20
NEI4 $86
line 106
;106:		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect; cinematic demoEnd.RoQ 1\n" );
CNSTI4 2
ARGI4
ADDRGP4 $89
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 107
;107:	}
ADDRGP4 $87
JUMPV
LABELV $86
line 108
;108:	else {
line 109
;109:		trap_Cmd_ExecuteText( EXEC_APPEND, va( "disconnect; cinematic %s.RoQ\n", cinematics[n] ) );
ADDRGP4 $90
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 cinematics
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 110
;110:	}
LABELV $87
line 111
;111:}
LABELV $81
endproc UI_CinematicsMenu_Event 12 8
proc UI_CinematicsMenu_Init 40 12
line 119
;112:
;113:
;114:/*
;115:===============
;116:UI_CinematicsMenu_Init
;117:===============
;118:*/
;119:static void UI_CinematicsMenu_Init( void ) {
line 122
;120:	int		y;
;121:
;122:	UI_CinematicsMenu_Cache();
ADDRGP4 UI_CinematicsMenu_Cache
CALLV
pop
line 124
;123:
;124:	memset( &cinematicsMenuInfo, 0, sizeof(cinematicsMenuInfo) );
ADDRGP4 cinematicsMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1344
ARGI4
ADDRGP4 memset
CALLP4
pop
line 125
;125:	cinematicsMenuInfo.menu.fullscreen = qtrue;
ADDRGP4 cinematicsMenuInfo+280
CNSTI4 1
ASGNI4
line 127
;126:
;127:	cinematicsMenuInfo.banner.generic.type		= MTYPE_BTEXT;
ADDRGP4 cinematicsMenuInfo+288
CNSTI4 10
ASGNI4
line 128
;128:	cinematicsMenuInfo.banner.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+288+12
CNSTI4 320
ASGNI4
line 129
;129:	cinematicsMenuInfo.banner.generic.y			= 16;
ADDRGP4 cinematicsMenuInfo+288+16
CNSTI4 16
ASGNI4
line 130
;130:	cinematicsMenuInfo.banner.string			= "CINEMATICS";
ADDRGP4 cinematicsMenuInfo+288+60
ADDRGP4 $100
ASGNP4
line 131
;131:	cinematicsMenuInfo.banner.color				= color_white;
ADDRGP4 cinematicsMenuInfo+288+68
ADDRGP4 color_white
ASGNP4
line 132
;132:	cinematicsMenuInfo.banner.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+288+64
CNSTI4 1
ASGNI4
line 134
;133:
;134:	cinematicsMenuInfo.framel.generic.type		= MTYPE_BITMAP;
ADDRGP4 cinematicsMenuInfo+360
CNSTI4 6
ASGNI4
line 135
;135:	cinematicsMenuInfo.framel.generic.name		= ART_FRAMEL;
ADDRGP4 cinematicsMenuInfo+360+4
ADDRGP4 $108
ASGNP4
line 136
;136:	cinematicsMenuInfo.framel.generic.flags		= QMF_INACTIVE;
ADDRGP4 cinematicsMenuInfo+360+44
CNSTU4 16384
ASGNU4
line 137
;137:	cinematicsMenuInfo.framel.generic.x			= 0;  
ADDRGP4 cinematicsMenuInfo+360+12
CNSTI4 0
ASGNI4
line 138
;138:	cinematicsMenuInfo.framel.generic.y			= 78;
ADDRGP4 cinematicsMenuInfo+360+16
CNSTI4 78
ASGNI4
line 139
;139:	cinematicsMenuInfo.framel.width  			= 256;
ADDRGP4 cinematicsMenuInfo+360+76
CNSTI4 256
ASGNI4
line 140
;140:	cinematicsMenuInfo.framel.height  			= 329;
ADDRGP4 cinematicsMenuInfo+360+80
CNSTI4 329
ASGNI4
line 142
;141:
;142:	cinematicsMenuInfo.framer.generic.type		= MTYPE_BITMAP;
ADDRGP4 cinematicsMenuInfo+448
CNSTI4 6
ASGNI4
line 143
;143:	cinematicsMenuInfo.framer.generic.name		= ART_FRAMER;
ADDRGP4 cinematicsMenuInfo+448+4
ADDRGP4 $122
ASGNP4
line 144
;144:	cinematicsMenuInfo.framer.generic.flags		= QMF_INACTIVE;
ADDRGP4 cinematicsMenuInfo+448+44
CNSTU4 16384
ASGNU4
line 145
;145:	cinematicsMenuInfo.framer.generic.x			= 376;
ADDRGP4 cinematicsMenuInfo+448+12
CNSTI4 376
ASGNI4
line 146
;146:	cinematicsMenuInfo.framer.generic.y			= 76;
ADDRGP4 cinematicsMenuInfo+448+16
CNSTI4 76
ASGNI4
line 147
;147:	cinematicsMenuInfo.framer.width  			= 256;
ADDRGP4 cinematicsMenuInfo+448+76
CNSTI4 256
ASGNI4
line 148
;148:	cinematicsMenuInfo.framer.height  			= 334;
ADDRGP4 cinematicsMenuInfo+448+80
CNSTI4 334
ASGNI4
line 150
;149:
;150:	y = 100;
ADDRLP4 0
CNSTI4 100
ASGNI4
line 151
;151:	cinematicsMenuInfo.cin_idlogo.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+536
CNSTI4 9
ASGNI4
line 152
;152:	cinematicsMenuInfo.cin_idlogo.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+536+44
CNSTU4 264
ASGNU4
line 153
;153:	cinematicsMenuInfo.cin_idlogo.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+536+12
CNSTI4 320
ASGNI4
line 154
;154:	cinematicsMenuInfo.cin_idlogo.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+536+16
ADDRLP4 0
INDIRI4
ASGNI4
line 155
;155:	cinematicsMenuInfo.cin_idlogo.generic.id		= ID_CIN_IDLOGO;
ADDRGP4 cinematicsMenuInfo+536+8
CNSTI4 11
ASGNI4
line 156
;156:	cinematicsMenuInfo.cin_idlogo.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+536+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 157
;157:	cinematicsMenuInfo.cin_idlogo.string			= "ID LOGO";
ADDRGP4 cinematicsMenuInfo+536+60
ADDRGP4 $146
ASGNP4
line 158
;158:	cinematicsMenuInfo.cin_idlogo.color				= color_red;
ADDRGP4 cinematicsMenuInfo+536+68
ADDRGP4 color_red
ASGNP4
line 159
;159:	cinematicsMenuInfo.cin_idlogo.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+536+64
CNSTI4 1
ASGNI4
line 161
;160:
;161:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 162
;162:	cinematicsMenuInfo.cin_intro.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+608
CNSTI4 9
ASGNI4
line 163
;163:	cinematicsMenuInfo.cin_intro.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+608+44
CNSTU4 264
ASGNU4
line 164
;164:	cinematicsMenuInfo.cin_intro.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+608+12
CNSTI4 320
ASGNI4
line 165
;165:	cinematicsMenuInfo.cin_intro.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+608+16
ADDRLP4 0
INDIRI4
ASGNI4
line 166
;166:	cinematicsMenuInfo.cin_intro.generic.id			= ID_CIN_INTRO;
ADDRGP4 cinematicsMenuInfo+608+8
CNSTI4 12
ASGNI4
line 167
;167:	cinematicsMenuInfo.cin_intro.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+608+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 168
;168:	cinematicsMenuInfo.cin_intro.string				= "INTRO";
ADDRGP4 cinematicsMenuInfo+608+60
ADDRGP4 $164
ASGNP4
line 169
;169:	cinematicsMenuInfo.cin_intro.color				= color_red;
ADDRGP4 cinematicsMenuInfo+608+68
ADDRGP4 color_red
ASGNP4
line 170
;170:	cinematicsMenuInfo.cin_intro.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+608+64
CNSTI4 1
ASGNI4
line 171
;171:	if( uis.demoversion ) {
ADDRGP4 uis+11440
INDIRI4
CNSTI4 0
EQI4 $169
line 172
;172:		cinematicsMenuInfo.cin_intro.generic.flags |= QMF_GRAYED;
ADDRLP4 4
ADDRGP4 cinematicsMenuInfo+608+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 173
;173:	}
LABELV $169
line 175
;174:
;175:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 176
;176:	cinematicsMenuInfo.cin_tier1.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+680
CNSTI4 9
ASGNI4
line 177
;177:	cinematicsMenuInfo.cin_tier1.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+680+44
CNSTU4 264
ASGNU4
line 178
;178:	cinematicsMenuInfo.cin_tier1.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+680+12
CNSTI4 320
ASGNI4
line 179
;179:	cinematicsMenuInfo.cin_tier1.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+680+16
ADDRLP4 0
INDIRI4
ASGNI4
line 180
;180:	cinematicsMenuInfo.cin_tier1.generic.id			= ID_CIN_TIER1;
ADDRGP4 cinematicsMenuInfo+680+8
CNSTI4 13
ASGNI4
line 181
;181:	cinematicsMenuInfo.cin_tier1.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+680+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 182
;182:	cinematicsMenuInfo.cin_tier1.string				= "Tier 1";
ADDRGP4 cinematicsMenuInfo+680+60
ADDRGP4 $187
ASGNP4
line 183
;183:	cinematicsMenuInfo.cin_tier1.color				= color_red;
ADDRGP4 cinematicsMenuInfo+680+68
ADDRGP4 color_red
ASGNP4
line 184
;184:	cinematicsMenuInfo.cin_tier1.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+680+64
CNSTI4 1
ASGNI4
line 185
;185:	if( !UI_CanShowTierVideo( 1 ) ) {
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $192
line 186
;186:		cinematicsMenuInfo.cin_tier1.generic.flags |= QMF_GRAYED;
ADDRLP4 8
ADDRGP4 cinematicsMenuInfo+680+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 187
;187:	}
LABELV $192
line 189
;188:
;189:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 190
;190:	cinematicsMenuInfo.cin_tier2.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+752
CNSTI4 9
ASGNI4
line 191
;191:	cinematicsMenuInfo.cin_tier2.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+752+44
CNSTU4 264
ASGNU4
line 192
;192:	cinematicsMenuInfo.cin_tier2.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+752+12
CNSTI4 320
ASGNI4
line 193
;193:	cinematicsMenuInfo.cin_tier2.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+752+16
ADDRLP4 0
INDIRI4
ASGNI4
line 194
;194:	cinematicsMenuInfo.cin_tier2.generic.id			= ID_CIN_TIER2;
ADDRGP4 cinematicsMenuInfo+752+8
CNSTI4 14
ASGNI4
line 195
;195:	cinematicsMenuInfo.cin_tier2.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+752+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 196
;196:	cinematicsMenuInfo.cin_tier2.string				= "Tier 2";
ADDRGP4 cinematicsMenuInfo+752+60
ADDRGP4 $209
ASGNP4
line 197
;197:	cinematicsMenuInfo.cin_tier2.color				= color_red;
ADDRGP4 cinematicsMenuInfo+752+68
ADDRGP4 color_red
ASGNP4
line 198
;198:	cinematicsMenuInfo.cin_tier2.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+752+64
CNSTI4 1
ASGNI4
line 199
;199:	if( !UI_CanShowTierVideo( 2 ) ) {
CNSTI4 2
ARGI4
ADDRLP4 8
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $214
line 200
;200:		cinematicsMenuInfo.cin_tier2.generic.flags |= QMF_GRAYED;
ADDRLP4 12
ADDRGP4 cinematicsMenuInfo+752+44
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 201
;201:	}
LABELV $214
line 203
;202:
;203:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 204
;204:	cinematicsMenuInfo.cin_tier3.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+824
CNSTI4 9
ASGNI4
line 205
;205:	cinematicsMenuInfo.cin_tier3.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+824+44
CNSTU4 264
ASGNU4
line 206
;206:	cinematicsMenuInfo.cin_tier3.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+824+12
CNSTI4 320
ASGNI4
line 207
;207:	cinematicsMenuInfo.cin_tier3.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+824+16
ADDRLP4 0
INDIRI4
ASGNI4
line 208
;208:	cinematicsMenuInfo.cin_tier3.generic.id			= ID_CIN_TIER3;
ADDRGP4 cinematicsMenuInfo+824+8
CNSTI4 15
ASGNI4
line 209
;209:	cinematicsMenuInfo.cin_tier3.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+824+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 210
;210:	cinematicsMenuInfo.cin_tier3.string				= "Tier 3";
ADDRGP4 cinematicsMenuInfo+824+60
ADDRGP4 $231
ASGNP4
line 211
;211:	cinematicsMenuInfo.cin_tier3.color				= color_red;
ADDRGP4 cinematicsMenuInfo+824+68
ADDRGP4 color_red
ASGNP4
line 212
;212:	cinematicsMenuInfo.cin_tier3.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+824+64
CNSTI4 1
ASGNI4
line 213
;213:	if( !UI_CanShowTierVideo( 3 ) ) {
CNSTI4 3
ARGI4
ADDRLP4 12
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $236
line 214
;214:		cinematicsMenuInfo.cin_tier3.generic.flags |= QMF_GRAYED;
ADDRLP4 16
ADDRGP4 cinematicsMenuInfo+824+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 215
;215:	}
LABELV $236
line 217
;216:
;217:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 218
;218:	cinematicsMenuInfo.cin_tier4.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+896
CNSTI4 9
ASGNI4
line 219
;219:	cinematicsMenuInfo.cin_tier4.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+896+44
CNSTU4 264
ASGNU4
line 220
;220:	cinematicsMenuInfo.cin_tier4.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+896+12
CNSTI4 320
ASGNI4
line 221
;221:	cinematicsMenuInfo.cin_tier4.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+896+16
ADDRLP4 0
INDIRI4
ASGNI4
line 222
;222:	cinematicsMenuInfo.cin_tier4.generic.id			= ID_CIN_TIER4;
ADDRGP4 cinematicsMenuInfo+896+8
CNSTI4 16
ASGNI4
line 223
;223:	cinematicsMenuInfo.cin_tier4.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+896+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 224
;224:	cinematicsMenuInfo.cin_tier4.string				= "Tier 4";
ADDRGP4 cinematicsMenuInfo+896+60
ADDRGP4 $253
ASGNP4
line 225
;225:	cinematicsMenuInfo.cin_tier4.color				= color_red;
ADDRGP4 cinematicsMenuInfo+896+68
ADDRGP4 color_red
ASGNP4
line 226
;226:	cinematicsMenuInfo.cin_tier4.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+896+64
CNSTI4 1
ASGNI4
line 227
;227:	if( !UI_CanShowTierVideo( 4 ) ) {
CNSTI4 4
ARGI4
ADDRLP4 16
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $258
line 228
;228:		cinematicsMenuInfo.cin_tier4.generic.flags |= QMF_GRAYED;
ADDRLP4 20
ADDRGP4 cinematicsMenuInfo+896+44
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 229
;229:	}
LABELV $258
line 231
;230:
;231:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 232
;232:	cinematicsMenuInfo.cin_tier5.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+968
CNSTI4 9
ASGNI4
line 233
;233:	cinematicsMenuInfo.cin_tier5.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+968+44
CNSTU4 264
ASGNU4
line 234
;234:	cinematicsMenuInfo.cin_tier5.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+968+12
CNSTI4 320
ASGNI4
line 235
;235:	cinematicsMenuInfo.cin_tier5.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+968+16
ADDRLP4 0
INDIRI4
ASGNI4
line 236
;236:	cinematicsMenuInfo.cin_tier5.generic.id			= ID_CIN_TIER5;
ADDRGP4 cinematicsMenuInfo+968+8
CNSTI4 17
ASGNI4
line 237
;237:	cinematicsMenuInfo.cin_tier5.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+968+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 238
;238:	cinematicsMenuInfo.cin_tier5.string				= "Tier 5";
ADDRGP4 cinematicsMenuInfo+968+60
ADDRGP4 $275
ASGNP4
line 239
;239:	cinematicsMenuInfo.cin_tier5.color				= color_red;
ADDRGP4 cinematicsMenuInfo+968+68
ADDRGP4 color_red
ASGNP4
line 240
;240:	cinematicsMenuInfo.cin_tier5.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+968+64
CNSTI4 1
ASGNI4
line 241
;241:	if( !UI_CanShowTierVideo( 5 ) ) {
CNSTI4 5
ARGI4
ADDRLP4 20
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $280
line 242
;242:		cinematicsMenuInfo.cin_tier5.generic.flags |= QMF_GRAYED;
ADDRLP4 24
ADDRGP4 cinematicsMenuInfo+968+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 243
;243:	}
LABELV $280
line 245
;244:
;245:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 246
;246:	cinematicsMenuInfo.cin_tier6.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+1040
CNSTI4 9
ASGNI4
line 247
;247:	cinematicsMenuInfo.cin_tier6.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+1040+44
CNSTU4 264
ASGNU4
line 248
;248:	cinematicsMenuInfo.cin_tier6.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+1040+12
CNSTI4 320
ASGNI4
line 249
;249:	cinematicsMenuInfo.cin_tier6.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+1040+16
ADDRLP4 0
INDIRI4
ASGNI4
line 250
;250:	cinematicsMenuInfo.cin_tier6.generic.id			= ID_CIN_TIER6;
ADDRGP4 cinematicsMenuInfo+1040+8
CNSTI4 18
ASGNI4
line 251
;251:	cinematicsMenuInfo.cin_tier6.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+1040+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 252
;252:	cinematicsMenuInfo.cin_tier6.string				= "Tier 6";
ADDRGP4 cinematicsMenuInfo+1040+60
ADDRGP4 $297
ASGNP4
line 253
;253:	cinematicsMenuInfo.cin_tier6.color				= color_red;
ADDRGP4 cinematicsMenuInfo+1040+68
ADDRGP4 color_red
ASGNP4
line 254
;254:	cinematicsMenuInfo.cin_tier6.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+1040+64
CNSTI4 1
ASGNI4
line 255
;255:	if( !UI_CanShowTierVideo( 6 ) ) {
CNSTI4 6
ARGI4
ADDRLP4 24
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $302
line 256
;256:		cinematicsMenuInfo.cin_tier6.generic.flags |= QMF_GRAYED;
ADDRLP4 28
ADDRGP4 cinematicsMenuInfo+1040+44
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 257
;257:	}
LABELV $302
line 259
;258:
;259:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 260
;260:	cinematicsMenuInfo.cin_tier7.generic.type		= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+1112
CNSTI4 9
ASGNI4
line 261
;261:	cinematicsMenuInfo.cin_tier7.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+1112+44
CNSTU4 264
ASGNU4
line 262
;262:	cinematicsMenuInfo.cin_tier7.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+1112+12
CNSTI4 320
ASGNI4
line 263
;263:	cinematicsMenuInfo.cin_tier7.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+1112+16
ADDRLP4 0
INDIRI4
ASGNI4
line 264
;264:	cinematicsMenuInfo.cin_tier7.generic.id			= ID_CIN_TIER7;
ADDRGP4 cinematicsMenuInfo+1112+8
CNSTI4 19
ASGNI4
line 265
;265:	cinematicsMenuInfo.cin_tier7.generic.callback	= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+1112+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 266
;266:	cinematicsMenuInfo.cin_tier7.string				= "Tier 7";
ADDRGP4 cinematicsMenuInfo+1112+60
ADDRGP4 $319
ASGNP4
line 267
;267:	cinematicsMenuInfo.cin_tier7.color				= color_red;
ADDRGP4 cinematicsMenuInfo+1112+68
ADDRGP4 color_red
ASGNP4
line 268
;268:	cinematicsMenuInfo.cin_tier7.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+1112+64
CNSTI4 1
ASGNI4
line 269
;269:	if( !UI_CanShowTierVideo( 7 ) ) {
CNSTI4 7
ARGI4
ADDRLP4 28
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $324
line 270
;270:		cinematicsMenuInfo.cin_tier7.generic.flags |= QMF_GRAYED;
ADDRLP4 32
ADDRGP4 cinematicsMenuInfo+1112+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 271
;271:	}
LABELV $324
line 273
;272:
;273:	y += VERTICAL_SPACING;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 30
ADDI4
ASGNI4
line 274
;274:	cinematicsMenuInfo.cin_end.generic.type			= MTYPE_PTEXT;
ADDRGP4 cinematicsMenuInfo+1184
CNSTI4 9
ASGNI4
line 275
;275:	cinematicsMenuInfo.cin_end.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+1184+44
CNSTU4 264
ASGNU4
line 276
;276:	cinematicsMenuInfo.cin_end.generic.x			= 320;
ADDRGP4 cinematicsMenuInfo+1184+12
CNSTI4 320
ASGNI4
line 277
;277:	cinematicsMenuInfo.cin_end.generic.y			= y;
ADDRGP4 cinematicsMenuInfo+1184+16
ADDRLP4 0
INDIRI4
ASGNI4
line 278
;278:	cinematicsMenuInfo.cin_end.generic.id			= ID_CIN_END;
ADDRGP4 cinematicsMenuInfo+1184+8
CNSTI4 20
ASGNI4
line 279
;279:	cinematicsMenuInfo.cin_end.generic.callback		= UI_CinematicsMenu_Event; 
ADDRGP4 cinematicsMenuInfo+1184+48
ADDRGP4 UI_CinematicsMenu_Event
ASGNP4
line 280
;280:	cinematicsMenuInfo.cin_end.string				= "END";
ADDRGP4 cinematicsMenuInfo+1184+60
ADDRGP4 $341
ASGNP4
line 281
;281:	cinematicsMenuInfo.cin_end.color				= color_red;
ADDRGP4 cinematicsMenuInfo+1184+68
ADDRGP4 color_red
ASGNP4
line 282
;282:	cinematicsMenuInfo.cin_end.style				= UI_CENTER;
ADDRGP4 cinematicsMenuInfo+1184+64
CNSTI4 1
ASGNI4
line 283
;283:	if( !UI_CanShowTierVideo( 8 ) ) {
CNSTI4 8
ARGI4
ADDRLP4 32
ADDRGP4 UI_CanShowTierVideo
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $346
line 284
;284:		cinematicsMenuInfo.cin_end.generic.flags |= QMF_GRAYED;
ADDRLP4 36
ADDRGP4 cinematicsMenuInfo+1184+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 285
;285:	}
LABELV $346
line 287
;286:
;287:	cinematicsMenuInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 cinematicsMenuInfo+1256
CNSTI4 6
ASGNI4
line 288
;288:	cinematicsMenuInfo.back.generic.name		= ART_BACK0;
ADDRGP4 cinematicsMenuInfo+1256+4
ADDRGP4 $353
ASGNP4
line 289
;289:	cinematicsMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 cinematicsMenuInfo+1256+44
CNSTU4 260
ASGNU4
line 290
;290:	cinematicsMenuInfo.back.generic.id			= ID_BACK;
ADDRGP4 cinematicsMenuInfo+1256+8
CNSTI4 10
ASGNI4
line 291
;291:	cinematicsMenuInfo.back.generic.callback	= UI_CinematicsMenu_BackEvent;
ADDRGP4 cinematicsMenuInfo+1256+48
ADDRGP4 UI_CinematicsMenu_BackEvent
ASGNP4
line 292
;292:	cinematicsMenuInfo.back.generic.x			= 0;
ADDRGP4 cinematicsMenuInfo+1256+12
CNSTI4 0
ASGNI4
line 293
;293:	cinematicsMenuInfo.back.generic.y			= 480-64;
ADDRGP4 cinematicsMenuInfo+1256+16
CNSTI4 416
ASGNI4
line 294
;294:	cinematicsMenuInfo.back.width				= 128;
ADDRGP4 cinematicsMenuInfo+1256+76
CNSTI4 128
ASGNI4
line 295
;295:	cinematicsMenuInfo.back.height				= 64;
ADDRGP4 cinematicsMenuInfo+1256+80
CNSTI4 64
ASGNI4
line 296
;296:	cinematicsMenuInfo.back.focuspic			= ART_BACK1;
ADDRGP4 cinematicsMenuInfo+1256+60
ADDRGP4 $370
ASGNP4
line 298
;297:
;298:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.banner );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 299
;299:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.framel );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 300
;300:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.framer );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 301
;301:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_idlogo );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 302
;302:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_intro );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+608
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 303
;303:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier1 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 304
;304:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier2 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+752
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 305
;305:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier3 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+824
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 306
;306:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier4 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+896
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 307
;307:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier5 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+968
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 308
;308:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier6 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+1040
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 309
;309:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier7 );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+1112
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 310
;310:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_end );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+1184
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 311
;311:	Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.back );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 cinematicsMenuInfo+1256
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 312
;312:}
LABELV $91
endproc UI_CinematicsMenu_Init 40 12
export UI_CinematicsMenu_Cache
proc UI_CinematicsMenu_Cache 0 4
line 320
;313:
;314:
;315:/*
;316:=================
;317:UI_CinematicsMenu_Cache
;318:=================
;319:*/
;320:void UI_CinematicsMenu_Cache( void ) {
line 321
;321:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $353
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 322
;322:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $370
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 323
;323:	trap_R_RegisterShaderNoMip( ART_FRAMEL );
ADDRGP4 $108
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 324
;324:	trap_R_RegisterShaderNoMip( ART_FRAMER );
ADDRGP4 $122
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 325
;325:}
LABELV $385
endproc UI_CinematicsMenu_Cache 0 4
export UI_CinematicsMenu
proc UI_CinematicsMenu 0 4
line 333
;326:
;327:
;328:/*
;329:===============
;330:UI_CinematicsMenu
;331:===============
;332:*/
;333:void UI_CinematicsMenu( void ) {
line 334
;334:	UI_CinematicsMenu_Init();
ADDRGP4 UI_CinematicsMenu_Init
CALLV
pop
line 335
;335:	UI_PushMenu( &cinematicsMenuInfo.menu );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 336
;336:}
LABELV $386
endproc UI_CinematicsMenu 0 4
export UI_CinematicsMenu_f
proc UI_CinematicsMenu_f 12 8
line 344
;337:
;338:
;339:/*
;340:===============
;341:UI_CinematicsMenu_f
;342:===============
;343:*/
;344:void UI_CinematicsMenu_f( void ) {
line 347
;345:	int		n;
;346:
;347:	n = atoi( UI_Argv( 1 ) );
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 348
;348:	UI_CinematicsMenu();
ADDRGP4 UI_CinematicsMenu
CALLV
pop
line 349
;349:	Menu_SetCursorToItem( &cinematicsMenuInfo.menu, cinematicsMenuInfo.menu.items[n + 3] );
ADDRGP4 cinematicsMenuInfo
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 cinematicsMenuInfo+12+12
ADDP4
INDIRP4
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 350
;350:}
LABELV $387
endproc UI_CinematicsMenu_f 12 8
bss
align 4
LABELV cinematicsMenuInfo
skip 1344
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
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $353
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
LABELV $341
byte 1 69
byte 1 78
byte 1 68
byte 1 0
align 1
LABELV $319
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 55
byte 1 0
align 1
LABELV $297
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 54
byte 1 0
align 1
LABELV $275
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 53
byte 1 0
align 1
LABELV $253
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 52
byte 1 0
align 1
LABELV $231
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 51
byte 1 0
align 1
LABELV $209
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 50
byte 1 0
align 1
LABELV $187
byte 1 84
byte 1 105
byte 1 101
byte 1 114
byte 1 32
byte 1 49
byte 1 0
align 1
LABELV $164
byte 1 73
byte 1 78
byte 1 84
byte 1 82
byte 1 79
byte 1 0
align 1
LABELV $146
byte 1 73
byte 1 68
byte 1 32
byte 1 76
byte 1 79
byte 1 71
byte 1 79
byte 1 0
align 1
LABELV $122
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
LABELV $108
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
LABELV $100
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
LABELV $90
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 59
byte 1 32
byte 1 99
byte 1 105
byte 1 110
byte 1 101
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 32
byte 1 37
byte 1 115
byte 1 46
byte 1 82
byte 1 111
byte 1 81
byte 1 10
byte 1 0
align 1
LABELV $89
byte 1 100
byte 1 105
byte 1 115
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 59
byte 1 32
byte 1 99
byte 1 105
byte 1 110
byte 1 101
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 32
byte 1 100
byte 1 101
byte 1 109
byte 1 111
byte 1 69
byte 1 110
byte 1 100
byte 1 46
byte 1 82
byte 1 111
byte 1 81
byte 1 32
byte 1 49
byte 1 10
byte 1 0
align 1
LABELV $85
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
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $84
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $77
byte 1 101
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $76
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 55
byte 1 0
align 1
LABELV $75
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 54
byte 1 0
align 1
LABELV $74
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 53
byte 1 0
align 1
LABELV $73
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 52
byte 1 0
align 1
LABELV $72
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 51
byte 1 0
align 1
LABELV $71
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $70
byte 1 116
byte 1 105
byte 1 101
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $69
byte 1 105
byte 1 110
byte 1 116
byte 1 114
byte 1 111
byte 1 0
align 1
LABELV $68
byte 1 105
byte 1 100
byte 1 108
byte 1 111
byte 1 103
byte 1 111
byte 1 0
