data
align 4
LABELV ctfOrders
address $68
address $69
address $70
address $71
address $72
address $73
address $74
byte 4 0
align 4
LABELV ctfMessages
address $75
address $76
address $77
address $78
address $79
address $80
address $81
byte 4 0
align 4
LABELV teamOrders
address $68
address $70
address $82
address $72
address $73
address $74
byte 4 0
align 4
LABELV teamMessages
address $75
address $77
address $83
address $79
address $80
address $81
byte 4 0
code
proc UI_TeamOrdersMenu_BackEvent 0 0
file "../ui_teamorders.c"
line 111
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
;26:TEAM ORDERS MENU
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define ART_FRAME		"menu/art/addbotframe"
;36:#define ART_BACK0		"menu/art/back_0"
;37:#define ART_BACK1		"menu/art/back_1"	
;38:
;39:#define ID_LIST_BOTS		10
;40:#define ID_LIST_CTF_ORDERS	11
;41:#define ID_LIST_TEAM_ORDERS	12
;42:
;43:
;44:typedef struct {
;45:	menuframework_s	menu;
;46:
;47:	menutext_s		banner;
;48:	menubitmap_s	frame;
;49:
;50:	menulist_s		list;
;51:
;52:	menubitmap_s	back;
;53:
;54:	int				gametype;
;55:	int				numBots;
;56:	int				selectedBot;
;57:	char			*bots[9];
;58:	char			botNames[9][16];
;59:} teamOrdersMenuInfo_t;
;60:
;61:static teamOrdersMenuInfo_t	teamOrdersMenuInfo;
;62:
;63:#define NUM_CTF_ORDERS		7
;64:static const char *ctfOrders[] = {
;65:	"I Am the Leader",
;66:	"Defend the Base",
;67:	"Follow Me",
;68:	"Get Enemy Flag",
;69:	"Camp Here",
;70:	"Report",
;71:	"I Relinquish Command",
;72:	NULL
;73:};
;74:static const char *ctfMessages[] = {
;75:	"i am the leader",
;76:	"%s defend the base",
;77:	"%s follow me",
;78:	"%s get enemy flag",
;79:	"%s camp here",
;80:	"%s report",
;81:	"i stop being the leader",
;82:	NULL
;83:};
;84:
;85:#define NUM_TEAM_ORDERS		6
;86:static const char *teamOrders[] = {
;87:	"I Am the Leader",
;88:	"Follow Me",
;89:	"Roam",
;90:	"Camp Here",
;91:	"Report",
;92:	"I Relinquish Command",
;93:	NULL
;94:};
;95:static const char *teamMessages[] = {
;96:	"i am the leader",
;97:	"%s follow me",
;98:	"%s roam",
;99:	"%s camp here",
;100:	"%s report",
;101:	"i stop being the leader",
;102:	NULL
;103:};
;104:
;105:
;106:/*
;107:===============
;108:UI_TeamOrdersMenu_BackEvent
;109:===============
;110:*/
;111:static void UI_TeamOrdersMenu_BackEvent( void *ptr, int event ) {
line 112
;112:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $85
line 113
;113:		return;
ADDRGP4 $84
JUMPV
LABELV $85
line 115
;114:	}
;115:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 116
;116:}
LABELV $84
endproc UI_TeamOrdersMenu_BackEvent 0 0
proc UI_TeamOrdersMenu_SetList 4 0
line 124
;117:
;118:
;119:/*
;120:===============
;121:UI_TeamOrdersMenu_SetList
;122:===============
;123:*/
;124:static void UI_TeamOrdersMenu_SetList( int id ) {
line 125
;125:	switch( id ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 10
EQI4 $90
ADDRLP4 0
INDIRI4
CNSTI4 11
EQI4 $99
ADDRLP4 0
INDIRI4
CNSTI4 12
EQI4 $106
ADDRGP4 $88
JUMPV
LABELV $88
LABELV $90
line 128
;126:	default:
;127:	case ID_LIST_BOTS:
;128:		teamOrdersMenuInfo.list.generic.id = id;
ADDRGP4 teamOrdersMenuInfo+448+8
ADDRFP4 0
INDIRI4
ASGNI4
line 129
;129:		teamOrdersMenuInfo.list.numitems = teamOrdersMenuInfo.numBots;
ADDRGP4 teamOrdersMenuInfo+448+68
ADDRGP4 teamOrdersMenuInfo+636
INDIRI4
ASGNI4
line 130
;130:		teamOrdersMenuInfo.list.itemnames = (const char **)teamOrdersMenuInfo.bots;
ADDRGP4 teamOrdersMenuInfo+448+76
ADDRGP4 teamOrdersMenuInfo+644
ASGNP4
line 131
;131:		 break;
ADDRGP4 $89
JUMPV
LABELV $99
line 134
;132:
;133:	case ID_LIST_CTF_ORDERS:
;134:		teamOrdersMenuInfo.list.generic.id = id;
ADDRGP4 teamOrdersMenuInfo+448+8
ADDRFP4 0
INDIRI4
ASGNI4
line 135
;135:		teamOrdersMenuInfo.list.numitems = NUM_CTF_ORDERS;
ADDRGP4 teamOrdersMenuInfo+448+68
CNSTI4 7
ASGNI4
line 136
;136:		teamOrdersMenuInfo.list.itemnames = ctfOrders;
ADDRGP4 teamOrdersMenuInfo+448+76
ADDRGP4 ctfOrders
ASGNP4
line 137
;137:		break;
ADDRGP4 $89
JUMPV
LABELV $106
line 140
;138:
;139:	case ID_LIST_TEAM_ORDERS:
;140:		teamOrdersMenuInfo.list.generic.id = id;
ADDRGP4 teamOrdersMenuInfo+448+8
ADDRFP4 0
INDIRI4
ASGNI4
line 141
;141:		teamOrdersMenuInfo.list.numitems = NUM_TEAM_ORDERS;
ADDRGP4 teamOrdersMenuInfo+448+68
CNSTI4 6
ASGNI4
line 142
;142:		teamOrdersMenuInfo.list.itemnames = teamOrders;
ADDRGP4 teamOrdersMenuInfo+448+76
ADDRGP4 teamOrders
ASGNP4
line 143
;143:		break;
LABELV $89
line 146
;144:	}
;145:
;146:	teamOrdersMenuInfo.list.generic.bottom = teamOrdersMenuInfo.list.generic.top + teamOrdersMenuInfo.list.numitems * PROP_HEIGHT;
ADDRGP4 teamOrdersMenuInfo+448+32
ADDRGP4 teamOrdersMenuInfo+448+24
INDIRI4
CNSTI4 27
ADDRGP4 teamOrdersMenuInfo+448+68
INDIRI4
MULI4
ADDI4
ASGNI4
line 147
;147:}
LABELV $87
endproc UI_TeamOrdersMenu_SetList 4 0
export UI_TeamOrdersMenu_Key
proc UI_TeamOrdersMenu_Key 72 16
line 155
;148:
;149:
;150:/*
;151:=================
;152:UI_TeamOrdersMenu_Key
;153:=================
;154:*/
;155:sfxHandle_t UI_TeamOrdersMenu_Key( int key ) {
line 161
;156:	menulist_s	*l;
;157:	int	x;
;158:	int	y;
;159:	int	index;
;160:
;161:	l = (menulist_s	*)Menu_ItemAtCursor( &teamOrdersMenuInfo.menu );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRLP4 16
ADDRGP4 Menu_ItemAtCursor
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 162
;162:	if( l != &teamOrdersMenuInfo.list ) {
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 teamOrdersMenuInfo+448
CVPU4 4
EQU4 $120
line 163
;163:		return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $119
JUMPV
LABELV $120
line 166
;164:	}
;165:
;166:	switch( key ) {
ADDRLP4 20
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 24
CNSTI4 161
ASGNI4
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $131
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
GTI4 $138
LABELV $137
ADDRLP4 28
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 132
EQI4 $131
ADDRLP4 28
INDIRI4
CNSTI4 133
EQI4 $134
ADDRGP4 $123
JUMPV
LABELV $138
ADDRLP4 32
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 36
CNSTI4 167
ASGNI4
ADDRLP4 32
INDIRI4
ADDRLP4 36
INDIRI4
EQI4 $134
ADDRLP4 32
INDIRI4
ADDRLP4 36
INDIRI4
LTI4 $123
LABELV $139
ADDRFP4 0
INDIRI4
CNSTI4 178
EQI4 $125
ADDRGP4 $123
JUMPV
LABELV $125
line 168
;167:		case K_MOUSE1:
;168:			x = l->generic.left;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
line 169
;169:			y = l->generic.top;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
line 170
;170:			if( UI_CursorInRect( x, y, l->generic.right - x, l->generic.bottom - y ) ) {
ADDRLP4 40
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 44
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
SUBI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
SUBI4
ARGI4
ADDRLP4 52
ADDRGP4 UI_CursorInRect
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $126
line 171
;171:				index = (uis.cursory - y) / PROP_HEIGHT;
ADDRLP4 12
ADDRGP4 uis+12
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 27
DIVI4
ASGNI4
line 172
;172:				l->oldvalue = l->curvalue;
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
ASGNI4
line 173
;173:				l->curvalue = index;
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 175
;174:
;175:				if( l->generic.callback ) {
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $129
line 176
;176:					l->generic.callback( l, QM_ACTIVATED );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRP4
CALLV
pop
line 177
;177:					return menu_move_sound;
ADDRGP4 menu_move_sound
INDIRI4
RETI4
ADDRGP4 $119
JUMPV
LABELV $129
line 179
;178:				}
;179:			}
LABELV $126
line 180
;180:			return menu_null_sound;
ADDRGP4 menu_null_sound
INDIRI4
RETI4
ADDRGP4 $119
JUMPV
LABELV $131
line 184
;181:
;182:		case K_KP_UPARROW:
;183:		case K_UPARROW:
;184:			l->oldvalue = l->curvalue;
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
ASGNI4
line 186
;185:
;186:			if( l->curvalue == 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
CNSTI4 0
NEI4 $132
line 187
;187:				l->curvalue = l->numitems - 1;
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 188
;188:			}
ADDRGP4 $133
JUMPV
LABELV $132
line 189
;189:			else {
line 190
;190:				l->curvalue--;
ADDRLP4 60
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 191
;191:			}
LABELV $133
line 192
;192:			return menu_move_sound;
ADDRGP4 menu_move_sound
INDIRI4
RETI4
ADDRGP4 $119
JUMPV
LABELV $134
line 196
;193:
;194:		case K_KP_DOWNARROW:
;195:		case K_DOWNARROW:
;196:			l->oldvalue = l->curvalue;
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
ASGNI4
line 198
;197:
;198:			if( l->curvalue == l->numitems - 1 ) {
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1
SUBI4
NEI4 $135
line 199
;199:				l->curvalue = 0;;
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
CNSTI4 0
ASGNI4
line 200
;200:			}
ADDRGP4 $136
JUMPV
LABELV $135
line 201
;201:			else {
line 202
;202:				l->curvalue++;
ADDRLP4 68
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 203
;203:			}
LABELV $136
line 204
;204:			return menu_move_sound;
ADDRGP4 menu_move_sound
INDIRI4
RETI4
ADDRGP4 $119
JUMPV
LABELV $123
line 207
;205:	}
;206:
;207:	return Menu_DefaultKey( &teamOrdersMenuInfo.menu, key );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
RETI4
LABELV $119
endproc UI_TeamOrdersMenu_Key 72 16
proc UI_TeamOrdersMenu_ListDraw 36 20
line 216
;208:}
;209:
;210:
;211:/*
;212:=================
;213:UI_TeamOrdersMenu_ListDraw
;214:=================
;215:*/
;216:static void UI_TeamOrdersMenu_ListDraw( void *self ) {
line 225
;217:	menulist_s	*l;
;218:	int			x;
;219:	int			y;
;220:	int			i;
;221:	float		*color;
;222:	qboolean	hasfocus;
;223:	int			style;
;224:
;225:	l = (menulist_s *)self;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 227
;226:
;227:	hasfocus = (l->generic.parent->cursor == l->generic.menuPosition);
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
NEI4 $142
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRGP4 $143
JUMPV
LABELV $142
ADDRLP4 28
CNSTI4 0
ASGNI4
LABELV $143
ADDRLP4 24
ADDRLP4 28
INDIRI4
ASGNI4
line 229
;228:
;229:	x =	320;//l->generic.x;
ADDRLP4 20
CNSTI4 320
ASGNI4
line 230
;230:	y =	l->generic.y;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 231
;231:	for( i = 0; i < l->numitems; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $147
JUMPV
LABELV $144
line 232
;232:		style = UI_LEFT|UI_SMALLFONT|UI_CENTER;
ADDRLP4 8
CNSTI4 17
ASGNI4
line 233
;233:		if( i == l->curvalue ) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
NEI4 $148
line 234
;234:			color = color_yellow;
ADDRLP4 16
ADDRGP4 color_yellow
ASGNP4
line 235
;235:			if( hasfocus ) {
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $149
line 236
;236:				style |= UI_PULSE;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 237
;237:			}
line 238
;238:		}
ADDRGP4 $149
JUMPV
LABELV $148
line 239
;239:		else {
line 240
;240:			color = color_orange;
ADDRLP4 16
ADDRGP4 color_orange
ASGNP4
line 241
;241:		}
LABELV $149
line 243
;242:
;243:		UI_DrawProportionalString( x, y, l->itemnames[i], style, color );
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 76
ADDP4
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 244
;244:		y += PROP_HEIGHT;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 27
ADDI4
ASGNI4
line 245
;245:	}
LABELV $145
line 231
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $147
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
LTI4 $144
line 246
;246:}
LABELV $140
endproc UI_TeamOrdersMenu_ListDraw 36 20
proc UI_TeamOrdersMenu_ListEvent 268 16
line 254
;247:
;248:
;249:/*
;250:===============
;251:UI_TeamOrdersMenu_ListEvent
;252:===============
;253:*/
;254:static void UI_TeamOrdersMenu_ListEvent( void *ptr, int event ) {
line 259
;255:	int		id;
;256:	int		selection;
;257:	char	message[256];
;258:
;259:	if (event != QM_ACTIVATED)
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $153
line 260
;260:		return;
ADDRGP4 $152
JUMPV
LABELV $153
line 262
;261:
;262:	id = ((menulist_s *)ptr)->generic.id;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 263
;263:	selection = ((menulist_s *)ptr)->curvalue;
ADDRLP4 260
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRI4
ASGNI4
line 265
;264:
;265:	if( id == ID_LIST_BOTS ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
NEI4 $155
line 266
;266:		teamOrdersMenuInfo.selectedBot = selection;
ADDRGP4 teamOrdersMenuInfo+640
ADDRLP4 260
INDIRI4
ASGNI4
line 267
;267:		if( teamOrdersMenuInfo.gametype == GT_CTF ) {
ADDRGP4 teamOrdersMenuInfo+632
INDIRI4
CNSTI4 4
NEI4 $158
line 268
;268:			UI_TeamOrdersMenu_SetList( ID_LIST_CTF_ORDERS );
CNSTI4 11
ARGI4
ADDRGP4 UI_TeamOrdersMenu_SetList
CALLV
pop
line 269
;269:		}
ADDRGP4 $152
JUMPV
LABELV $158
line 270
;270:		else {
line 271
;271:			UI_TeamOrdersMenu_SetList( ID_LIST_TEAM_ORDERS );
CNSTI4 12
ARGI4
ADDRGP4 UI_TeamOrdersMenu_SetList
CALLV
pop
line 272
;272:		}
line 273
;273:		return;
ADDRGP4 $152
JUMPV
LABELV $155
line 276
;274:	}
;275:
;276:	if( id == ID_LIST_CTF_ORDERS ) {
ADDRLP4 0
INDIRI4
CNSTI4 11
NEI4 $161
line 277
;277:		Com_sprintf( message, sizeof(message), ctfMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ctfMessages
ADDP4
INDIRP4
ARGP4
ADDRGP4 teamOrdersMenuInfo+640
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 teamOrdersMenuInfo+680
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 278
;278:	}
ADDRGP4 $162
JUMPV
LABELV $161
line 279
;279:	else {
line 280
;280:		Com_sprintf( message, sizeof(message), teamMessages[selection], teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.selectedBot] );
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 teamMessages
ADDP4
INDIRP4
ARGP4
ADDRGP4 teamOrdersMenuInfo+640
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 teamOrdersMenuInfo+680
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 281
;281:	}
LABELV $162
line 283
;282:
;283:	trap_Cmd_ExecuteText( EXEC_APPEND, va( "say_team \"%s\"\n", message ) );
ADDRGP4 $167
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 264
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 264
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 284
;284:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 285
;285:}
LABELV $152
endproc UI_TeamOrdersMenu_ListEvent 268 16
proc UI_TeamOrdersMenu_BuildBotList 4164 12
line 293
;286:
;287:
;288:/*
;289:===============
;290:UI_TeamOrdersMenu_BuildBotList
;291:===============
;292:*/
;293:static void UI_TeamOrdersMenu_BuildBotList( void ) {
line 302
;294:	uiClientState_t	cs;
;295:	int		numPlayers;
;296:	int		isBot;
;297:	int		n;
;298:	char	playerTeam;
;299:	char	botTeam;
;300:	char	info[MAX_INFO_STRING];
;301:
;302:	for( n = 0; n < 9; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $169
line 303
;303:		teamOrdersMenuInfo.bots[n] = teamOrdersMenuInfo.botNames[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 teamOrdersMenuInfo+644
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 teamOrdersMenuInfo+680
ADDP4
ASGNP4
line 304
;304:	}
LABELV $170
line 302
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 9
LTI4 $169
line 306
;305:
;306:	trap_GetClientState( &cs );
ADDRLP4 1040
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 308
;307:
;308:	Q_strncpyz( teamOrdersMenuInfo.botNames[0], "Everyone", 16 );
ADDRGP4 teamOrdersMenuInfo+680
ARGP4
ADDRGP4 $176
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 309
;309:	teamOrdersMenuInfo.numBots = 1;
ADDRGP4 teamOrdersMenuInfo+636
CNSTI4 1
ASGNI4
line 311
;310:
;311:	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
CNSTI4 0
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 312
;312:	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $178
ARGP4
ADDRLP4 4128
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4128
INDIRP4
ARGP4
ADDRLP4 4132
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4124
ADDRLP4 4132
INDIRI4
ASGNI4
line 313
;313:	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $180
ARGP4
ADDRLP4 4136
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4136
INDIRP4
ARGP4
ADDRLP4 4140
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 teamOrdersMenuInfo+632
ADDRLP4 4140
INDIRI4
ASGNI4
line 315
;314:
;315:	for( n = 0; n < numPlayers && teamOrdersMenuInfo.numBots < 9; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $184
JUMPV
LABELV $181
line 316
;316:		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );
ADDRLP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 318
;317:
;318:		playerTeam = TEAM_SPECTATOR; // bk001204 = possible uninit use
ADDRLP4 1028
CNSTI1 3
ASGNI1
line 320
;319:
;320:		if( n == cs.clientNum ) {
ADDRLP4 0
INDIRI4
ADDRLP4 1040+8
INDIRI4
NEI4 $186
line 321
;321:			playerTeam = *Info_ValueForKey( info, "t" );
ADDRLP4 4
ARGP4
ADDRGP4 $189
ARGP4
ADDRLP4 4144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 4144
INDIRP4
INDIRI1
ASGNI1
line 322
;322:			continue;
ADDRGP4 $182
JUMPV
LABELV $186
line 325
;323:		}
;324:
;325:		isBot = atoi( Info_ValueForKey( info, "skill" ) );
ADDRLP4 4
ARGP4
ADDRGP4 $190
ARGP4
ADDRLP4 4144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4144
INDIRP4
ARGP4
ADDRLP4 4148
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 4148
INDIRI4
ASGNI4
line 326
;326:		if( !isBot ) {
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $191
line 327
;327:			continue;
ADDRGP4 $182
JUMPV
LABELV $191
line 330
;328:		}
;329:
;330:		botTeam = *Info_ValueForKey( info, "t" );
ADDRLP4 4
ARGP4
ADDRGP4 $189
ARGP4
ADDRLP4 4152
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ADDRLP4 4152
INDIRP4
INDIRI1
ASGNI1
line 331
;331:		if( botTeam != playerTeam ) {
ADDRLP4 1036
INDIRI1
CVII4 1
ADDRLP4 1028
INDIRI1
CVII4 1
EQI4 $193
line 332
;332:			continue;
ADDRGP4 $182
JUMPV
LABELV $193
line 335
;333:		}
;334:
;335:		Q_strncpyz( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots], Info_ValueForKey( info, "n" ), 16 );
ADDRLP4 4
ARGP4
ADDRGP4 $197
ARGP4
ADDRLP4 4156
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 teamOrdersMenuInfo+636
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 teamOrdersMenuInfo+680
ADDP4
ARGP4
ADDRLP4 4156
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 336
;336:		Q_CleanStr( teamOrdersMenuInfo.botNames[teamOrdersMenuInfo.numBots] );
ADDRGP4 teamOrdersMenuInfo+636
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 teamOrdersMenuInfo+680
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 337
;337:		teamOrdersMenuInfo.numBots++;
ADDRLP4 4160
ADDRGP4 teamOrdersMenuInfo+636
ASGNP4
ADDRLP4 4160
INDIRP4
ADDRLP4 4160
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 338
;338:	}
LABELV $182
line 315
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $184
ADDRLP4 0
INDIRI4
ADDRLP4 4124
INDIRI4
GEI4 $201
ADDRGP4 teamOrdersMenuInfo+636
INDIRI4
CNSTI4 9
LTI4 $181
LABELV $201
line 339
;339:}
LABELV $168
endproc UI_TeamOrdersMenu_BuildBotList 4164 12
proc UI_TeamOrdersMenu_Init 0 12
line 347
;340:
;341:
;342:/*
;343:===============
;344:UI_TeamOrdersMenu_Init
;345:===============
;346:*/
;347:static void UI_TeamOrdersMenu_Init( void ) {
line 348
;348:	UI_TeamOrdersMenu_Cache();
ADDRGP4 UI_TeamOrdersMenu_Cache
CALLV
pop
line 350
;349:
;350:	memset( &teamOrdersMenuInfo, 0, sizeof(teamOrdersMenuInfo) );
ADDRGP4 teamOrdersMenuInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 824
ARGI4
ADDRGP4 memset
CALLP4
pop
line 351
;351:	teamOrdersMenuInfo.menu.fullscreen = qfalse;
ADDRGP4 teamOrdersMenuInfo+280
CNSTI4 0
ASGNI4
line 352
;352:	teamOrdersMenuInfo.menu.key = UI_TeamOrdersMenu_Key;
ADDRGP4 teamOrdersMenuInfo+272
ADDRGP4 UI_TeamOrdersMenu_Key
ASGNP4
line 354
;353:
;354:	UI_TeamOrdersMenu_BuildBotList();
ADDRGP4 UI_TeamOrdersMenu_BuildBotList
CALLV
pop
line 356
;355:
;356:	teamOrdersMenuInfo.banner.generic.type		= MTYPE_BTEXT;
ADDRGP4 teamOrdersMenuInfo+288
CNSTI4 10
ASGNI4
line 357
;357:	teamOrdersMenuInfo.banner.generic.x			= 320;
ADDRGP4 teamOrdersMenuInfo+288+12
CNSTI4 320
ASGNI4
line 358
;358:	teamOrdersMenuInfo.banner.generic.y			= 16;
ADDRGP4 teamOrdersMenuInfo+288+16
CNSTI4 16
ASGNI4
line 359
;359:	teamOrdersMenuInfo.banner.string			= "TEAM ORDERS";
ADDRGP4 teamOrdersMenuInfo+288+60
ADDRGP4 $212
ASGNP4
line 360
;360:	teamOrdersMenuInfo.banner.color				= color_white;
ADDRGP4 teamOrdersMenuInfo+288+68
ADDRGP4 color_white
ASGNP4
line 361
;361:	teamOrdersMenuInfo.banner.style				= UI_CENTER;
ADDRGP4 teamOrdersMenuInfo+288+64
CNSTI4 1
ASGNI4
line 363
;362:
;363:	teamOrdersMenuInfo.frame.generic.type		= MTYPE_BITMAP;
ADDRGP4 teamOrdersMenuInfo+360
CNSTI4 6
ASGNI4
line 364
;364:	teamOrdersMenuInfo.frame.generic.flags		= QMF_INACTIVE;
ADDRGP4 teamOrdersMenuInfo+360+44
CNSTU4 16384
ASGNU4
line 365
;365:	teamOrdersMenuInfo.frame.generic.name		= ART_FRAME;
ADDRGP4 teamOrdersMenuInfo+360+4
ADDRGP4 $222
ASGNP4
line 366
;366:	teamOrdersMenuInfo.frame.generic.x			= 320-233;
ADDRGP4 teamOrdersMenuInfo+360+12
CNSTI4 87
ASGNI4
line 367
;367:	teamOrdersMenuInfo.frame.generic.y			= 240-166;
ADDRGP4 teamOrdersMenuInfo+360+16
CNSTI4 74
ASGNI4
line 368
;368:	teamOrdersMenuInfo.frame.width				= 466;
ADDRGP4 teamOrdersMenuInfo+360+76
CNSTI4 466
ASGNI4
line 369
;369:	teamOrdersMenuInfo.frame.height				= 332;
ADDRGP4 teamOrdersMenuInfo+360+80
CNSTI4 332
ASGNI4
line 371
;370:
;371:	teamOrdersMenuInfo.list.generic.type		= MTYPE_SCROLLLIST;
ADDRGP4 teamOrdersMenuInfo+448
CNSTI4 8
ASGNI4
line 372
;372:	teamOrdersMenuInfo.list.generic.flags		= QMF_PULSEIFFOCUS;
ADDRGP4 teamOrdersMenuInfo+448+44
CNSTU4 256
ASGNU4
line 373
;373:	teamOrdersMenuInfo.list.generic.ownerdraw	= UI_TeamOrdersMenu_ListDraw;
ADDRGP4 teamOrdersMenuInfo+448+56
ADDRGP4 UI_TeamOrdersMenu_ListDraw
ASGNP4
line 374
;374:	teamOrdersMenuInfo.list.generic.callback	= UI_TeamOrdersMenu_ListEvent;
ADDRGP4 teamOrdersMenuInfo+448+48
ADDRGP4 UI_TeamOrdersMenu_ListEvent
ASGNP4
line 375
;375:	teamOrdersMenuInfo.list.generic.x			= 320-64;
ADDRGP4 teamOrdersMenuInfo+448+12
CNSTI4 256
ASGNI4
line 376
;376:	teamOrdersMenuInfo.list.generic.y			= 120;
ADDRGP4 teamOrdersMenuInfo+448+16
CNSTI4 120
ASGNI4
line 378
;377:
;378:	teamOrdersMenuInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 teamOrdersMenuInfo+544
CNSTI4 6
ASGNI4
line 379
;379:	teamOrdersMenuInfo.back.generic.name		= ART_BACK0;
ADDRGP4 teamOrdersMenuInfo+544+4
ADDRGP4 $245
ASGNP4
line 380
;380:	teamOrdersMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 teamOrdersMenuInfo+544+44
CNSTU4 260
ASGNU4
line 381
;381:	teamOrdersMenuInfo.back.generic.callback	= UI_TeamOrdersMenu_BackEvent;
ADDRGP4 teamOrdersMenuInfo+544+48
ADDRGP4 UI_TeamOrdersMenu_BackEvent
ASGNP4
line 382
;382:	teamOrdersMenuInfo.back.generic.x			= 0;
ADDRGP4 teamOrdersMenuInfo+544+12
CNSTI4 0
ASGNI4
line 383
;383:	teamOrdersMenuInfo.back.generic.y			= 480-64;
ADDRGP4 teamOrdersMenuInfo+544+16
CNSTI4 416
ASGNI4
line 384
;384:	teamOrdersMenuInfo.back.width				= 128;
ADDRGP4 teamOrdersMenuInfo+544+76
CNSTI4 128
ASGNI4
line 385
;385:	teamOrdersMenuInfo.back.height				= 64;
ADDRGP4 teamOrdersMenuInfo+544+80
CNSTI4 64
ASGNI4
line 386
;386:	teamOrdersMenuInfo.back.focuspic			= ART_BACK1;
ADDRGP4 teamOrdersMenuInfo+544+60
ADDRGP4 $260
ASGNP4
line 388
;387:
;388:	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.banner );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRGP4 teamOrdersMenuInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 389
;389:	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.frame );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRGP4 teamOrdersMenuInfo+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 390
;390:	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.list );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRGP4 teamOrdersMenuInfo+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 391
;391:	Menu_AddItem( &teamOrdersMenuInfo.menu, &teamOrdersMenuInfo.back );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRGP4 teamOrdersMenuInfo+544
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 393
;392:
;393:	teamOrdersMenuInfo.list.generic.left = 220;
ADDRGP4 teamOrdersMenuInfo+448+20
CNSTI4 220
ASGNI4
line 394
;394:	teamOrdersMenuInfo.list.generic.top = teamOrdersMenuInfo.list.generic.y;
ADDRGP4 teamOrdersMenuInfo+448+24
ADDRGP4 teamOrdersMenuInfo+448+16
INDIRI4
ASGNI4
line 395
;395:	teamOrdersMenuInfo.list.generic.right = 420;
ADDRGP4 teamOrdersMenuInfo+448+28
CNSTI4 420
ASGNI4
line 396
;396:	UI_TeamOrdersMenu_SetList( ID_LIST_BOTS );
CNSTI4 10
ARGI4
ADDRGP4 UI_TeamOrdersMenu_SetList
CALLV
pop
line 397
;397:}
LABELV $202
endproc UI_TeamOrdersMenu_Init 0 12
export UI_TeamOrdersMenu_Cache
proc UI_TeamOrdersMenu_Cache 0 4
line 405
;398:
;399:
;400:/*
;401:=================
;402:UI_TeamOrdersMenu_Cache
;403:=================
;404:*/
;405:void UI_TeamOrdersMenu_Cache( void ) {
line 406
;406:	trap_R_RegisterShaderNoMip( ART_FRAME );
ADDRGP4 $222
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 407
;407:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $245
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 408
;408:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $260
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 409
;409:}
LABELV $273
endproc UI_TeamOrdersMenu_Cache 0 4
export UI_TeamOrdersMenu
proc UI_TeamOrdersMenu 0 4
line 417
;410:
;411:
;412:/*
;413:===============
;414:UI_TeamOrdersMenu
;415:===============
;416:*/
;417:void UI_TeamOrdersMenu( void ) {
line 418
;418:	UI_TeamOrdersMenu_Init();
ADDRGP4 UI_TeamOrdersMenu_Init
CALLV
pop
line 419
;419:	UI_PushMenu( &teamOrdersMenuInfo.menu );
ADDRGP4 teamOrdersMenuInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 420
;420:}
LABELV $274
endproc UI_TeamOrdersMenu 0 4
export UI_TeamOrdersMenu_f
proc UI_TeamOrdersMenu_f 4128 12
line 428
;421:
;422:
;423:/*
;424:===============
;425:UI_TeamOrdersMenu_f
;426:===============
;427:*/
;428:void UI_TeamOrdersMenu_f( void ) {
line 434
;429:	uiClientState_t	cs;
;430:	char	info[MAX_INFO_STRING];
;431:	int		team;
;432:
;433:	// make sure it's a team game
;434:	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 435
;435:	teamOrdersMenuInfo.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
ADDRLP4 0
ARGP4
ADDRGP4 $180
ARGP4
ADDRLP4 4112
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4112
INDIRP4
ARGP4
ADDRLP4 4116
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 teamOrdersMenuInfo+632
ADDRLP4 4116
INDIRI4
ASGNI4
line 436
;436:	if( teamOrdersMenuInfo.gametype < GT_TEAM ) {
ADDRGP4 teamOrdersMenuInfo+632
INDIRI4
CNSTI4 3
GEI4 $277
line 437
;437:		return;
ADDRGP4 $275
JUMPV
LABELV $277
line 441
;438:	}
;439:
;440:	// not available to spectators
;441:	trap_GetClientState( &cs );
ADDRLP4 1024
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 442
;442:	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
ADDRLP4 1024+8
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 443
;443:	team = atoi( Info_ValueForKey( info, "t" ) );
ADDRLP4 0
ARGP4
ADDRGP4 $189
ARGP4
ADDRLP4 4120
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4120
INDIRP4
ARGP4
ADDRLP4 4124
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4108
ADDRLP4 4124
INDIRI4
ASGNI4
line 444
;444:	if( team == TEAM_SPECTATOR ) {
ADDRLP4 4108
INDIRI4
CNSTI4 3
NEI4 $281
line 445
;445:		return;
ADDRGP4 $275
JUMPV
LABELV $281
line 448
;446:	}
;447:
;448:	UI_TeamOrdersMenu();
ADDRGP4 UI_TeamOrdersMenu
CALLV
pop
line 449
;449:}
LABELV $275
endproc UI_TeamOrdersMenu_f 4128 12
bss
align 4
LABELV teamOrdersMenuInfo
skip 824
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
LABELV $260
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
LABELV $245
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
LABELV $222
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
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 102
byte 1 114
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $212
byte 1 84
byte 1 69
byte 1 65
byte 1 77
byte 1 32
byte 1 79
byte 1 82
byte 1 68
byte 1 69
byte 1 82
byte 1 83
byte 1 0
align 1
LABELV $197
byte 1 110
byte 1 0
align 1
LABELV $190
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $189
byte 1 116
byte 1 0
align 1
LABELV $180
byte 1 103
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $178
byte 1 115
byte 1 118
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $176
byte 1 69
byte 1 118
byte 1 101
byte 1 114
byte 1 121
byte 1 111
byte 1 110
byte 1 101
byte 1 0
align 1
LABELV $167
byte 1 115
byte 1 97
byte 1 121
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 10
byte 1 0
align 1
LABELV $83
byte 1 37
byte 1 115
byte 1 32
byte 1 114
byte 1 111
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $82
byte 1 82
byte 1 111
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $81
byte 1 105
byte 1 32
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 32
byte 1 98
byte 1 101
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $80
byte 1 37
byte 1 115
byte 1 32
byte 1 114
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $79
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 97
byte 1 109
byte 1 112
byte 1 32
byte 1 104
byte 1 101
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $78
byte 1 37
byte 1 115
byte 1 32
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 101
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $77
byte 1 37
byte 1 115
byte 1 32
byte 1 102
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 32
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $76
byte 1 37
byte 1 115
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 98
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $75
byte 1 105
byte 1 32
byte 1 97
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 108
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $74
byte 1 73
byte 1 32
byte 1 82
byte 1 101
byte 1 108
byte 1 105
byte 1 110
byte 1 113
byte 1 117
byte 1 105
byte 1 115
byte 1 104
byte 1 32
byte 1 67
byte 1 111
byte 1 109
byte 1 109
byte 1 97
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $73
byte 1 82
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $72
byte 1 67
byte 1 97
byte 1 109
byte 1 112
byte 1 32
byte 1 72
byte 1 101
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $71
byte 1 71
byte 1 101
byte 1 116
byte 1 32
byte 1 69
byte 1 110
byte 1 101
byte 1 109
byte 1 121
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $70
byte 1 70
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 32
byte 1 77
byte 1 101
byte 1 0
align 1
LABELV $69
byte 1 68
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 66
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $68
byte 1 73
byte 1 32
byte 1 65
byte 1 109
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 76
byte 1 101
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
