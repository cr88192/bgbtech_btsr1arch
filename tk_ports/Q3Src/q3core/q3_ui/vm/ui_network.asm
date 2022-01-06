data
align 4
LABELV rate_items
address $67
address $68
address $69
address $70
address $71
byte 4 0
code
proc UI_NetworkOptionsMenu_Event 8 8
file "../ui_network.c"
line 81
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
;26:NETWORK OPTIONS MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:#include "ui_local.h"
;32:
;33:
;34:#define ART_FRAMEL			"menu/art/frame2_l"
;35:#define ART_FRAMER			"menu/art/frame1_r"
;36:#define ART_BACK0			"menu/art/back_0"
;37:#define ART_BACK1			"menu/art/back_1"
;38:
;39:#define ID_GRAPHICS			10
;40:#define ID_DISPLAY			11
;41:#define ID_SOUND			12
;42:#define ID_NETWORK			13
;43:#define ID_RATE				14
;44:#define ID_BACK				15
;45:
;46:
;47:static const char *rate_items[] = {
;48:	"<= 28.8K",
;49:	"33.6K",
;50:	"56K",
;51:	"ISDN",
;52:	"LAN/Cable/xDSL",
;53:	0
;54:};
;55:
;56:typedef struct {
;57:	menuframework_s	menu;
;58:
;59:	menutext_s		banner;
;60:	menubitmap_s	framel;
;61:	menubitmap_s	framer;
;62:
;63:	menutext_s		graphics;
;64:	menutext_s		display;
;65:	menutext_s		sound;
;66:	menutext_s		network;
;67:
;68:	menulist_s		rate;
;69:
;70:	menubitmap_s	back;
;71:} networkOptionsInfo_t;
;72:
;73:static networkOptionsInfo_t	networkOptionsInfo;
;74:
;75:
;76:/*
;77:=================
;78:UI_NetworkOptionsMenu_Event
;79:=================
;80:*/
;81:static void UI_NetworkOptionsMenu_Event( void* ptr, int event ) {
line 82
;82:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $74
line 83
;83:		return;
ADDRGP4 $73
JUMPV
LABELV $74
line 86
;84:	}
;85:
;86:	switch( ((menucommon_s*)ptr)->id ) {
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
LTI4 $76
ADDRLP4 0
INDIRI4
CNSTI4 15
GTI4 $76
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $106-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $106
address $79
address $80
address $81
address $77
address $83
address $105
code
LABELV $79
line 88
;87:	case ID_GRAPHICS:
;88:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 89
;89:		UI_GraphicsOptionsMenu();
ADDRGP4 UI_GraphicsOptionsMenu
CALLV
pop
line 90
;90:		break;
ADDRGP4 $77
JUMPV
LABELV $80
line 93
;91:
;92:	case ID_DISPLAY:
;93:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 94
;94:		UI_DisplayOptionsMenu();
ADDRGP4 UI_DisplayOptionsMenu
CALLV
pop
line 95
;95:		break;
ADDRGP4 $77
JUMPV
LABELV $81
line 98
;96:
;97:	case ID_SOUND:
;98:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 99
;99:		UI_SoundOptionsMenu();
ADDRGP4 UI_SoundOptionsMenu
CALLV
pop
line 100
;100:		break;
ADDRGP4 $77
JUMPV
line 103
;101:
;102:	case ID_NETWORK:
;103:		break;
LABELV $83
line 106
;104:
;105:	case ID_RATE:
;106:		if( networkOptionsInfo.rate.curvalue == 0 ) {
ADDRGP4 networkOptionsInfo+824+64
INDIRI4
CNSTI4 0
NEI4 $84
line 107
;107:			trap_Cvar_SetValue( "rate", 2500 );
ADDRGP4 $88
ARGP4
CNSTF4 1159479296
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 108
;108:		}
ADDRGP4 $77
JUMPV
LABELV $84
line 109
;109:		else if( networkOptionsInfo.rate.curvalue == 1 ) {
ADDRGP4 networkOptionsInfo+824+64
INDIRI4
CNSTI4 1
NEI4 $89
line 110
;110:			trap_Cvar_SetValue( "rate", 3000 );
ADDRGP4 $88
ARGP4
CNSTF4 1161527296
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 111
;111:		}
ADDRGP4 $77
JUMPV
LABELV $89
line 112
;112:		else if( networkOptionsInfo.rate.curvalue == 2 ) {
ADDRGP4 networkOptionsInfo+824+64
INDIRI4
CNSTI4 2
NEI4 $93
line 113
;113:			trap_Cvar_SetValue( "rate", 4000 );
ADDRGP4 $88
ARGP4
CNSTF4 1165623296
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 114
;114:		}
ADDRGP4 $77
JUMPV
LABELV $93
line 115
;115:		else if( networkOptionsInfo.rate.curvalue == 3 ) {
ADDRGP4 networkOptionsInfo+824+64
INDIRI4
CNSTI4 3
NEI4 $97
line 116
;116:			trap_Cvar_SetValue( "rate", 5000 );
ADDRGP4 $88
ARGP4
CNSTF4 1167867904
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 117
;117:		}
ADDRGP4 $77
JUMPV
LABELV $97
line 118
;118:		else if( networkOptionsInfo.rate.curvalue == 4 ) {
ADDRGP4 networkOptionsInfo+824+64
INDIRI4
CNSTI4 4
NEI4 $77
line 119
;119:			trap_Cvar_SetValue( "rate", 25000 );
ADDRGP4 $88
ARGP4
CNSTF4 1187205120
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 120
;120:		}
line 121
;121:		break;
ADDRGP4 $77
JUMPV
LABELV $105
line 124
;122:
;123:	case ID_BACK:
;124:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 125
;125:		break;
LABELV $76
LABELV $77
line 127
;126:	}
;127:}
LABELV $73
endproc UI_NetworkOptionsMenu_Event 8 8
proc UI_NetworkOptionsMenu_Init 12 12
line 135
;128:
;129:
;130:/*
;131:===============
;132:UI_NetworkOptionsMenu_Init
;133:===============
;134:*/
;135:static void UI_NetworkOptionsMenu_Init( void ) {
line 139
;136:	int		y;
;137:	int		rate;
;138:
;139:	memset( &networkOptionsInfo, 0, sizeof(networkOptionsInfo) );
ADDRGP4 networkOptionsInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1008
ARGI4
ADDRGP4 memset
CALLP4
pop
line 141
;140:
;141:	UI_NetworkOptionsMenu_Cache();
ADDRGP4 UI_NetworkOptionsMenu_Cache
CALLV
pop
line 142
;142:	networkOptionsInfo.menu.wrapAround = qtrue;
ADDRGP4 networkOptionsInfo+276
CNSTI4 1
ASGNI4
line 143
;143:	networkOptionsInfo.menu.fullscreen = qtrue;
ADDRGP4 networkOptionsInfo+280
CNSTI4 1
ASGNI4
line 145
;144:
;145:	networkOptionsInfo.banner.generic.type		= MTYPE_BTEXT;
ADDRGP4 networkOptionsInfo+288
CNSTI4 10
ASGNI4
line 146
;146:	networkOptionsInfo.banner.generic.flags		= QMF_CENTER_JUSTIFY;
ADDRGP4 networkOptionsInfo+288+44
CNSTU4 8
ASGNU4
line 147
;147:	networkOptionsInfo.banner.generic.x			= 320;
ADDRGP4 networkOptionsInfo+288+12
CNSTI4 320
ASGNI4
line 148
;148:	networkOptionsInfo.banner.generic.y			= 16;
ADDRGP4 networkOptionsInfo+288+16
CNSTI4 16
ASGNI4
line 149
;149:	networkOptionsInfo.banner.string			= "SYSTEM SETUP";
ADDRGP4 networkOptionsInfo+288+60
ADDRGP4 $120
ASGNP4
line 150
;150:	networkOptionsInfo.banner.color				= color_white;
ADDRGP4 networkOptionsInfo+288+68
ADDRGP4 color_white
ASGNP4
line 151
;151:	networkOptionsInfo.banner.style				= UI_CENTER;
ADDRGP4 networkOptionsInfo+288+64
CNSTI4 1
ASGNI4
line 153
;152:
;153:	networkOptionsInfo.framel.generic.type		= MTYPE_BITMAP;
ADDRGP4 networkOptionsInfo+360
CNSTI4 6
ASGNI4
line 154
;154:	networkOptionsInfo.framel.generic.name		= ART_FRAMEL;
ADDRGP4 networkOptionsInfo+360+4
ADDRGP4 $128
ASGNP4
line 155
;155:	networkOptionsInfo.framel.generic.flags		= QMF_INACTIVE;
ADDRGP4 networkOptionsInfo+360+44
CNSTU4 16384
ASGNU4
line 156
;156:	networkOptionsInfo.framel.generic.x			= 0;  
ADDRGP4 networkOptionsInfo+360+12
CNSTI4 0
ASGNI4
line 157
;157:	networkOptionsInfo.framel.generic.y			= 78;
ADDRGP4 networkOptionsInfo+360+16
CNSTI4 78
ASGNI4
line 158
;158:	networkOptionsInfo.framel.width				= 256;
ADDRGP4 networkOptionsInfo+360+76
CNSTI4 256
ASGNI4
line 159
;159:	networkOptionsInfo.framel.height			= 329;
ADDRGP4 networkOptionsInfo+360+80
CNSTI4 329
ASGNI4
line 161
;160:
;161:	networkOptionsInfo.framer.generic.type		= MTYPE_BITMAP;
ADDRGP4 networkOptionsInfo+448
CNSTI4 6
ASGNI4
line 162
;162:	networkOptionsInfo.framer.generic.name		= ART_FRAMER;
ADDRGP4 networkOptionsInfo+448+4
ADDRGP4 $142
ASGNP4
line 163
;163:	networkOptionsInfo.framer.generic.flags		= QMF_INACTIVE;
ADDRGP4 networkOptionsInfo+448+44
CNSTU4 16384
ASGNU4
line 164
;164:	networkOptionsInfo.framer.generic.x			= 376;
ADDRGP4 networkOptionsInfo+448+12
CNSTI4 376
ASGNI4
line 165
;165:	networkOptionsInfo.framer.generic.y			= 76;
ADDRGP4 networkOptionsInfo+448+16
CNSTI4 76
ASGNI4
line 166
;166:	networkOptionsInfo.framer.width				= 256;
ADDRGP4 networkOptionsInfo+448+76
CNSTI4 256
ASGNI4
line 167
;167:	networkOptionsInfo.framer.height			= 334;
ADDRGP4 networkOptionsInfo+448+80
CNSTI4 334
ASGNI4
line 169
;168:
;169:	networkOptionsInfo.graphics.generic.type		= MTYPE_PTEXT;
ADDRGP4 networkOptionsInfo+536
CNSTI4 9
ASGNI4
line 170
;170:	networkOptionsInfo.graphics.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 networkOptionsInfo+536+44
CNSTU4 272
ASGNU4
line 171
;171:	networkOptionsInfo.graphics.generic.id			= ID_GRAPHICS;
ADDRGP4 networkOptionsInfo+536+8
CNSTI4 10
ASGNI4
line 172
;172:	networkOptionsInfo.graphics.generic.callback	= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+536+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 173
;173:	networkOptionsInfo.graphics.generic.x			= 216;
ADDRGP4 networkOptionsInfo+536+12
CNSTI4 216
ASGNI4
line 174
;174:	networkOptionsInfo.graphics.generic.y			= 240 - 2 * PROP_HEIGHT;
ADDRGP4 networkOptionsInfo+536+16
CNSTI4 186
ASGNI4
line 175
;175:	networkOptionsInfo.graphics.string				= "GRAPHICS";
ADDRGP4 networkOptionsInfo+536+60
ADDRGP4 $166
ASGNP4
line 176
;176:	networkOptionsInfo.graphics.style				= UI_RIGHT;
ADDRGP4 networkOptionsInfo+536+64
CNSTI4 2
ASGNI4
line 177
;177:	networkOptionsInfo.graphics.color				= color_red;
ADDRGP4 networkOptionsInfo+536+68
ADDRGP4 color_red
ASGNP4
line 179
;178:
;179:	networkOptionsInfo.display.generic.type			= MTYPE_PTEXT;
ADDRGP4 networkOptionsInfo+608
CNSTI4 9
ASGNI4
line 180
;180:	networkOptionsInfo.display.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 networkOptionsInfo+608+44
CNSTU4 272
ASGNU4
line 181
;181:	networkOptionsInfo.display.generic.id			= ID_DISPLAY;
ADDRGP4 networkOptionsInfo+608+8
CNSTI4 11
ASGNI4
line 182
;182:	networkOptionsInfo.display.generic.callback		= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+608+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 183
;183:	networkOptionsInfo.display.generic.x			= 216;
ADDRGP4 networkOptionsInfo+608+12
CNSTI4 216
ASGNI4
line 184
;184:	networkOptionsInfo.display.generic.y			= 240 - PROP_HEIGHT;
ADDRGP4 networkOptionsInfo+608+16
CNSTI4 213
ASGNI4
line 185
;185:	networkOptionsInfo.display.string				= "DISPLAY";
ADDRGP4 networkOptionsInfo+608+60
ADDRGP4 $184
ASGNP4
line 186
;186:	networkOptionsInfo.display.style				= UI_RIGHT;
ADDRGP4 networkOptionsInfo+608+64
CNSTI4 2
ASGNI4
line 187
;187:	networkOptionsInfo.display.color				= color_red;
ADDRGP4 networkOptionsInfo+608+68
ADDRGP4 color_red
ASGNP4
line 189
;188:
;189:	networkOptionsInfo.sound.generic.type			= MTYPE_PTEXT;
ADDRGP4 networkOptionsInfo+680
CNSTI4 9
ASGNI4
line 190
;190:	networkOptionsInfo.sound.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 networkOptionsInfo+680+44
CNSTU4 272
ASGNU4
line 191
;191:	networkOptionsInfo.sound.generic.id				= ID_SOUND;
ADDRGP4 networkOptionsInfo+680+8
CNSTI4 12
ASGNI4
line 192
;192:	networkOptionsInfo.sound.generic.callback		= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+680+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 193
;193:	networkOptionsInfo.sound.generic.x				= 216;
ADDRGP4 networkOptionsInfo+680+12
CNSTI4 216
ASGNI4
line 194
;194:	networkOptionsInfo.sound.generic.y				= 240;
ADDRGP4 networkOptionsInfo+680+16
CNSTI4 240
ASGNI4
line 195
;195:	networkOptionsInfo.sound.string					= "SOUND";
ADDRGP4 networkOptionsInfo+680+60
ADDRGP4 $202
ASGNP4
line 196
;196:	networkOptionsInfo.sound.style					= UI_RIGHT;
ADDRGP4 networkOptionsInfo+680+64
CNSTI4 2
ASGNI4
line 197
;197:	networkOptionsInfo.sound.color					= color_red;
ADDRGP4 networkOptionsInfo+680+68
ADDRGP4 color_red
ASGNP4
line 199
;198:
;199:	networkOptionsInfo.network.generic.type			= MTYPE_PTEXT;
ADDRGP4 networkOptionsInfo+752
CNSTI4 9
ASGNI4
line 200
;200:	networkOptionsInfo.network.generic.flags		= QMF_RIGHT_JUSTIFY;
ADDRGP4 networkOptionsInfo+752+44
CNSTU4 16
ASGNU4
line 201
;201:	networkOptionsInfo.network.generic.id			= ID_NETWORK;
ADDRGP4 networkOptionsInfo+752+8
CNSTI4 13
ASGNI4
line 202
;202:	networkOptionsInfo.network.generic.callback		= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+752+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 203
;203:	networkOptionsInfo.network.generic.x			= 216;
ADDRGP4 networkOptionsInfo+752+12
CNSTI4 216
ASGNI4
line 204
;204:	networkOptionsInfo.network.generic.y			= 240 + PROP_HEIGHT;
ADDRGP4 networkOptionsInfo+752+16
CNSTI4 267
ASGNI4
line 205
;205:	networkOptionsInfo.network.string				= "NETWORK";
ADDRGP4 networkOptionsInfo+752+60
ADDRGP4 $220
ASGNP4
line 206
;206:	networkOptionsInfo.network.style				= UI_RIGHT;
ADDRGP4 networkOptionsInfo+752+64
CNSTI4 2
ASGNI4
line 207
;207:	networkOptionsInfo.network.color				= color_red;
ADDRGP4 networkOptionsInfo+752+68
ADDRGP4 color_red
ASGNP4
line 209
;208:
;209:	y = 240 - 1 * (BIGCHAR_HEIGHT+2);
ADDRLP4 4
CNSTI4 222
ASGNI4
line 210
;210:	networkOptionsInfo.rate.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 networkOptionsInfo+824
CNSTI4 3
ASGNI4
line 211
;211:	networkOptionsInfo.rate.generic.name		= "Data Rate:";
ADDRGP4 networkOptionsInfo+824+4
ADDRGP4 $228
ASGNP4
line 212
;212:	networkOptionsInfo.rate.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 networkOptionsInfo+824+44
CNSTU4 258
ASGNU4
line 213
;213:	networkOptionsInfo.rate.generic.callback	= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+824+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 214
;214:	networkOptionsInfo.rate.generic.id			= ID_RATE;
ADDRGP4 networkOptionsInfo+824+8
CNSTI4 14
ASGNI4
line 215
;215:	networkOptionsInfo.rate.generic.x			= 400;
ADDRGP4 networkOptionsInfo+824+12
CNSTI4 400
ASGNI4
line 216
;216:	networkOptionsInfo.rate.generic.y			= y;
ADDRGP4 networkOptionsInfo+824+16
ADDRLP4 4
INDIRI4
ASGNI4
line 217
;217:	networkOptionsInfo.rate.itemnames			= rate_items;
ADDRGP4 networkOptionsInfo+824+76
ADDRGP4 rate_items
ASGNP4
line 219
;218:
;219:	networkOptionsInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 networkOptionsInfo+920
CNSTI4 6
ASGNI4
line 220
;220:	networkOptionsInfo.back.generic.name		= ART_BACK0;
ADDRGP4 networkOptionsInfo+920+4
ADDRGP4 $244
ASGNP4
line 221
;221:	networkOptionsInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 networkOptionsInfo+920+44
CNSTU4 260
ASGNU4
line 222
;222:	networkOptionsInfo.back.generic.callback	= UI_NetworkOptionsMenu_Event;
ADDRGP4 networkOptionsInfo+920+48
ADDRGP4 UI_NetworkOptionsMenu_Event
ASGNP4
line 223
;223:	networkOptionsInfo.back.generic.id			= ID_BACK;
ADDRGP4 networkOptionsInfo+920+8
CNSTI4 15
ASGNI4
line 224
;224:	networkOptionsInfo.back.generic.x			= 0;
ADDRGP4 networkOptionsInfo+920+12
CNSTI4 0
ASGNI4
line 225
;225:	networkOptionsInfo.back.generic.y			= 480-64;
ADDRGP4 networkOptionsInfo+920+16
CNSTI4 416
ASGNI4
line 226
;226:	networkOptionsInfo.back.width				= 128;
ADDRGP4 networkOptionsInfo+920+76
CNSTI4 128
ASGNI4
line 227
;227:	networkOptionsInfo.back.height				= 64;
ADDRGP4 networkOptionsInfo+920+80
CNSTI4 64
ASGNI4
line 228
;228:	networkOptionsInfo.back.focuspic			= ART_BACK1;
ADDRGP4 networkOptionsInfo+920+60
ADDRGP4 $261
ASGNP4
line 230
;229:
;230:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.banner );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 231
;231:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.framel );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 232
;232:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.framer );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 233
;233:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.graphics );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 234
;234:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.display );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+608
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 235
;235:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.sound );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 236
;236:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.network );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+752
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 237
;237:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.rate );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+824
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 238
;238:	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.back );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+920
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 240
;239:
;240:	rate = trap_Cvar_VariableValue( "rate" );
ADDRGP4 $88
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 8
INDIRF4
CVFI4 4
ASGNI4
line 241
;241:	if( rate <= 2500 ) {
ADDRLP4 0
INDIRI4
CNSTI4 2500
GTI4 $271
line 242
;242:		networkOptionsInfo.rate.curvalue = 0;
ADDRGP4 networkOptionsInfo+824+64
CNSTI4 0
ASGNI4
line 243
;243:	}
ADDRGP4 $272
JUMPV
LABELV $271
line 244
;244:	else if( rate <= 3000 ) {
ADDRLP4 0
INDIRI4
CNSTI4 3000
GTI4 $275
line 245
;245:		networkOptionsInfo.rate.curvalue = 1;
ADDRGP4 networkOptionsInfo+824+64
CNSTI4 1
ASGNI4
line 246
;246:	}
ADDRGP4 $276
JUMPV
LABELV $275
line 247
;247:	else if( rate <= 4000 ) {
ADDRLP4 0
INDIRI4
CNSTI4 4000
GTI4 $279
line 248
;248:		networkOptionsInfo.rate.curvalue = 2;
ADDRGP4 networkOptionsInfo+824+64
CNSTI4 2
ASGNI4
line 249
;249:	}
ADDRGP4 $280
JUMPV
LABELV $279
line 250
;250:	else if( rate <= 5000 ) {
ADDRLP4 0
INDIRI4
CNSTI4 5000
GTI4 $283
line 251
;251:		networkOptionsInfo.rate.curvalue = 3;
ADDRGP4 networkOptionsInfo+824+64
CNSTI4 3
ASGNI4
line 252
;252:	}
ADDRGP4 $284
JUMPV
LABELV $283
line 253
;253:	else {
line 254
;254:		networkOptionsInfo.rate.curvalue = 4;
ADDRGP4 networkOptionsInfo+824+64
CNSTI4 4
ASGNI4
line 255
;255:	}
LABELV $284
LABELV $280
LABELV $276
LABELV $272
line 256
;256:}
LABELV $108
endproc UI_NetworkOptionsMenu_Init 12 12
export UI_NetworkOptionsMenu_Cache
proc UI_NetworkOptionsMenu_Cache 0 4
line 264
;257:
;258:
;259:/*
;260:===============
;261:UI_NetworkOptionsMenu_Cache
;262:===============
;263:*/
;264:void UI_NetworkOptionsMenu_Cache( void ) {
line 265
;265:	trap_R_RegisterShaderNoMip( ART_FRAMEL );
ADDRGP4 $128
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 266
;266:	trap_R_RegisterShaderNoMip( ART_FRAMER );
ADDRGP4 $142
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 267
;267:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $244
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 268
;268:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $261
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 269
;269:}
LABELV $289
endproc UI_NetworkOptionsMenu_Cache 0 4
export UI_NetworkOptionsMenu
proc UI_NetworkOptionsMenu 0 8
line 277
;270:
;271:
;272:/*
;273:===============
;274:UI_NetworkOptionsMenu
;275:===============
;276:*/
;277:void UI_NetworkOptionsMenu( void ) {
line 278
;278:	UI_NetworkOptionsMenu_Init();
ADDRGP4 UI_NetworkOptionsMenu_Init
CALLV
pop
line 279
;279:	UI_PushMenu( &networkOptionsInfo.menu );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 280
;280:	Menu_SetCursorToItem( &networkOptionsInfo.menu, &networkOptionsInfo.network );
ADDRGP4 networkOptionsInfo
ARGP4
ADDRGP4 networkOptionsInfo+752
ARGP4
ADDRGP4 Menu_SetCursorToItem
CALLV
pop
line 281
;281:}
LABELV $290
endproc UI_NetworkOptionsMenu 0 8
bss
align 4
LABELV networkOptionsInfo
skip 1008
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
LABELV $261
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
LABELV $244
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
LABELV $228
byte 1 68
byte 1 97
byte 1 116
byte 1 97
byte 1 32
byte 1 82
byte 1 97
byte 1 116
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $220
byte 1 78
byte 1 69
byte 1 84
byte 1 87
byte 1 79
byte 1 82
byte 1 75
byte 1 0
align 1
LABELV $202
byte 1 83
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 0
align 1
LABELV $184
byte 1 68
byte 1 73
byte 1 83
byte 1 80
byte 1 76
byte 1 65
byte 1 89
byte 1 0
align 1
LABELV $166
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
LABELV $142
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
LABELV $128
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
LABELV $120
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
LABELV $88
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $71
byte 1 76
byte 1 65
byte 1 78
byte 1 47
byte 1 67
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 47
byte 1 120
byte 1 68
byte 1 83
byte 1 76
byte 1 0
align 1
LABELV $70
byte 1 73
byte 1 83
byte 1 68
byte 1 78
byte 1 0
align 1
LABELV $69
byte 1 53
byte 1 54
byte 1 75
byte 1 0
align 1
LABELV $68
byte 1 51
byte 1 51
byte 1 46
byte 1 54
byte 1 75
byte 1 0
align 1
LABELV $67
byte 1 60
byte 1 61
byte 1 32
byte 1 50
byte 1 56
byte 1 46
byte 1 56
byte 1 75
byte 1 0
