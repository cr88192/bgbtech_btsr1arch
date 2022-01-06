data
align 4
LABELV gametype_items
address $68
address $69
address $70
address $71
byte 4 0
align 4
LABELV gametype_remap
byte 4 0
byte 4 3
byte 4 1
byte 4 4
align 4
LABELV gametype_remap2
byte 4 0
byte 4 2
byte 4 0
byte 4 1
byte 4 3
code
proc GametypeBits 36 8
file "../ui_startserver.c"
line 118
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
;24:=============================================================================
;25:
;26:START SERVER MENU *****
;27:
;28:=============================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define GAMESERVER_BACK0		"menu/art/back_0"
;36:#define GAMESERVER_BACK1		"menu/art/back_1"
;37:#define GAMESERVER_NEXT0		"menu/art/next_0"
;38:#define GAMESERVER_NEXT1		"menu/art/next_1"
;39:#define GAMESERVER_FRAMEL		"menu/art/frame2_l"
;40:#define GAMESERVER_FRAMER		"menu/art/frame1_r"
;41:#define GAMESERVER_SELECT		"menu/art/maps_select"
;42:#define GAMESERVER_SELECTED		"menu/art/maps_selected"
;43:#define GAMESERVER_FIGHT0		"menu/art/fight_0"
;44:#define GAMESERVER_FIGHT1		"menu/art/fight_1"
;45:#define GAMESERVER_UNKNOWNMAP	"menu/art/unknownmap"
;46:#define GAMESERVER_ARROWS		"menu/art/gs_arrows_0"
;47:#define GAMESERVER_ARROWSL		"menu/art/gs_arrows_l"
;48:#define GAMESERVER_ARROWSR		"menu/art/gs_arrows_r"
;49:
;50:#define MAX_MAPROWS		2
;51:#define MAX_MAPCOLS		2
;52:#define MAX_MAPSPERPAGE	4
;53:
;54:#define	MAX_SERVERSTEXT	8192
;55:
;56:#define MAX_SERVERMAPS	64
;57:#define MAX_NAMELENGTH	16
;58:
;59:#define ID_GAMETYPE				10
;60:#define ID_PICTURES				11	// 12, 13, 14
;61:#define ID_PREVPAGE				15
;62:#define ID_NEXTPAGE				16
;63:#define ID_STARTSERVERBACK		17
;64:#define ID_STARTSERVERNEXT		18
;65:
;66:typedef struct {
;67:	menuframework_s	menu;
;68:
;69:	menutext_s		banner;
;70:	menubitmap_s	framel;
;71:	menubitmap_s	framer;
;72:
;73:	menulist_s		gametype;
;74:	menubitmap_s	mappics[MAX_MAPSPERPAGE];
;75:	menubitmap_s	mapbuttons[MAX_MAPSPERPAGE];
;76:	menubitmap_s	arrows;
;77:	menubitmap_s	prevpage;
;78:	menubitmap_s	nextpage;
;79:	menubitmap_s	back;
;80:	menubitmap_s	next;
;81:
;82:	menutext_s		mapname;
;83:	menubitmap_s	item_null;
;84:
;85:	qboolean		multiplayer;
;86:	int				currentmap;
;87:	int				nummaps;
;88:	int				page;
;89:	int				maxpages;
;90:	char			maplist[MAX_SERVERMAPS][MAX_NAMELENGTH];
;91:	int				mapGamebits[MAX_SERVERMAPS];
;92:} startserver_t;
;93:
;94:static startserver_t s_startserver;
;95:
;96:static const char *gametype_items[] = {
;97:	"Free For All",
;98:	"Team Deathmatch",
;99:	"Tournament",
;100:	"Capture the Flag",
;101:	0
;102:};
;103:
;104:static int gametype_remap[] = {GT_FFA, GT_TEAM, GT_TOURNAMENT, GT_CTF};
;105:static int gametype_remap2[] = {0, 2, 0, 1, 3};
;106:
;107:// use ui_servers2.c definition
;108:extern const char* punkbuster_items[];
;109:
;110:static void UI_ServerOptionsMenu( qboolean multiplayer );
;111:
;112:
;113:/*
;114:=================
;115:GametypeBits
;116:=================
;117:*/
;118:static int GametypeBits( char *string ) {
line 123
;119:	int		bits;
;120:	char	*p;
;121:	char	*token;
;122:
;123:	bits = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 124
;124:	p = string;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $74
JUMPV
LABELV $73
line 125
;125:	while( 1 ) {
line 126
;126:		token = COM_ParseExt( &p, qfalse );
ADDRLP4 8
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 12
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 127
;127:		if( token[0] == 0 ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $76
line 128
;128:			break;
ADDRGP4 $75
JUMPV
LABELV $76
line 131
;129:		}
;130:
;131:		if( Q_stricmp( token, "ffa" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $80
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $78
line 132
;132:			bits |= 1 << GT_FFA;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 133
;133:			continue;
ADDRGP4 $74
JUMPV
LABELV $78
line 136
;134:		}
;135:
;136:		if( Q_stricmp( token, "tourney" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $83
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $81
line 137
;137:			bits |= 1 << GT_TOURNAMENT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 138
;138:			continue;
ADDRGP4 $74
JUMPV
LABELV $81
line 141
;139:		}
;140:
;141:		if( Q_stricmp( token, "single" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $86
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $84
line 142
;142:			bits |= 1 << GT_SINGLE_PLAYER;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 143
;143:			continue;
ADDRGP4 $74
JUMPV
LABELV $84
line 146
;144:		}
;145:
;146:		if( Q_stricmp( token, "team" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $89
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $87
line 147
;147:			bits |= 1 << GT_TEAM;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 148
;148:			continue;
ADDRGP4 $74
JUMPV
LABELV $87
line 151
;149:		}
;150:
;151:		if( Q_stricmp( token, "ctf" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $92
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $90
line 152
;152:			bits |= 1 << GT_CTF;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 153
;153:			continue;
LABELV $90
line 155
;154:		}
;155:	}
LABELV $74
line 125
ADDRGP4 $73
JUMPV
LABELV $75
line 157
;156:
;157:	return bits;
ADDRLP4 4
INDIRI4
RETI4
LABELV $72
endproc GametypeBits 36 8
bss
align 1
LABELV $94
skip 256
code
proc StartServer_Update 28 16
line 166
;158:}
;159:
;160:
;161:/*
;162:=================
;163:StartServer_Update
;164:=================
;165:*/
;166:static void StartServer_Update( void ) {
line 171
;167:	int				i;
;168:	int				top;
;169:	static	char	picname[MAX_MAPSPERPAGE][64];
;170:
;171:	top = s_startserver.page*MAX_MAPSPERPAGE;
ADDRLP4 4
ADDRGP4 s_startserver+1948
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
line 173
;172:
;173:	for (i=0; i<MAX_MAPSPERPAGE; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $96
line 174
;174:	{
line 175
;175:		if (top+i >= s_startserver.nummaps)
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ADDRGP4 s_startserver+1944
INDIRI4
LTI4 $100
line 176
;176:			break;
ADDRGP4 $118
JUMPV
LABELV $100
line 178
;177:
;178:		Com_sprintf( picname[i], sizeof(picname[i]), "levelshots/%s", s_startserver.maplist[top+i] );
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 $94
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $103
ARGP4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 180
;179:
;180:		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
ADDRLP4 12
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+44
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 4294967231
BANDU4
ASGNU4
line 181
;181:		s_startserver.mappics[i].generic.name   = picname[i];
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+4
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 $94
ADDP4
ASGNP4
line 182
;182:		s_startserver.mappics[i].shader         = 0;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+68
ADDP4
CNSTI4 0
ASGNI4
line 185
;183:
;184:		// reset
;185:		s_startserver.mapbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
ADDRLP4 20
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
line 186
;186:		s_startserver.mapbuttons[i].generic.flags &= ~QMF_INACTIVE;
ADDRLP4 24
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 187
;187:	}
LABELV $97
line 173
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $96
line 189
;188:
;189:	for (; i<MAX_MAPSPERPAGE; i++)
ADDRGP4 $118
JUMPV
LABELV $115
line 190
;190:	{
line 191
;191:		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
ADDRLP4 8
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294967231
BANDU4
ASGNU4
line 192
;192:		s_startserver.mappics[i].generic.name   = NULL;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+4
ADDP4
CNSTP4 0
ASGNP4
line 193
;193:		s_startserver.mappics[i].shader         = 0;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+68
ADDP4
CNSTI4 0
ASGNI4
line 196
;194:
;195:		// disable
;196:		s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
ADDRLP4 12
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
line 197
;197:		s_startserver.mapbuttons[i].generic.flags |= QMF_INACTIVE;
ADDRLP4 16
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 198
;198:	}
LABELV $116
line 189
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $118
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $115
line 202
;199:
;200:
;201:	// no servers to start
;202:	if( !s_startserver.nummaps ) {
ADDRGP4 s_startserver+1944
INDIRI4
CNSTI4 0
NEI4 $129
line 203
;203:		s_startserver.next.generic.flags |= QMF_INACTIVE;
ADDRLP4 8
ADDRGP4 s_startserver+1688+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 206
;204:
;205:		// set the map name
;206:		strcpy( s_startserver.mapname.string, "NO MAPS FOUND" );
ADDRGP4 s_startserver+1776+60
INDIRP4
ARGP4
ADDRGP4 $136
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 207
;207:	}
ADDRGP4 $130
JUMPV
LABELV $129
line 208
;208:	else {
line 210
;209:		// set the highlight
;210:		s_startserver.next.generic.flags &= ~QMF_INACTIVE;
ADDRLP4 8
ADDRGP4 s_startserver+1688+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 211
;211:		i = s_startserver.currentmap - top;
ADDRLP4 0
ADDRGP4 s_startserver+1940
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 212
;212:		if ( i >=0 && i < MAX_MAPSPERPAGE ) 
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $140
ADDRLP4 0
INDIRI4
CNSTI4 4
GEI4 $140
line 213
;213:		{
line 214
;214:			s_startserver.mappics[i].generic.flags    |= QMF_HIGHLIGHT;
ADDRLP4 16
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+44
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 64
BORU4
ASGNU4
line 215
;215:			s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
ADDRLP4 20
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
line 216
;216:		}
LABELV $140
line 219
;217:
;218:		// set the map name
;219:		strcpy( s_startserver.mapname.string, s_startserver.maplist[s_startserver.currentmap] );
ADDRGP4 s_startserver+1776+60
INDIRP4
ARGP4
ADDRGP4 s_startserver+1940
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 220
;220:	}
LABELV $130
line 222
;221:	
;222:	Q_strupr( s_startserver.mapname.string );
ADDRGP4 s_startserver+1776+60
INDIRP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 223
;223:}
LABELV $93
endproc StartServer_Update 28 16
proc StartServer_MapEvent 0 0
line 231
;224:
;225:
;226:/*
;227:=================
;228:StartServer_MapEvent
;229:=================
;230:*/
;231:static void StartServer_MapEvent( void* ptr, int event ) {
line 232
;232:	if( event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $153
line 233
;233:		return;
ADDRGP4 $152
JUMPV
LABELV $153
line 236
;234:	}
;235:
;236:	s_startserver.currentmap = (s_startserver.page*MAX_MAPSPERPAGE) + (((menucommon_s*)ptr)->id - ID_PICTURES);
ADDRGP4 s_startserver+1940
ADDRGP4 s_startserver+1948
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 11
SUBI4
ADDI4
ASGNI4
line 237
;237:	StartServer_Update();
ADDRGP4 StartServer_Update
CALLV
pop
line 238
;238:}
LABELV $152
endproc StartServer_MapEvent 0 0
proc StartServer_GametypeEvent 44 12
line 246
;239:
;240:
;241:/*
;242:=================
;243:StartServer_GametypeEvent
;244:=================
;245:*/
;246:static void StartServer_GametypeEvent( void* ptr, int event ) {
line 253
;247:	int			i;
;248:	int			count;
;249:	int			gamebits;
;250:	int			matchbits;
;251:	const char	*info;
;252:
;253:	if( event != QM_ACTIVATED) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $158
line 254
;254:		return;
ADDRGP4 $157
JUMPV
LABELV $158
line 257
;255:	}
;256:
;257:	count = UI_GetNumArenas();
ADDRLP4 20
ADDRGP4 UI_GetNumArenas
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 20
INDIRI4
ASGNI4
line 258
;258:	s_startserver.nummaps = 0;
ADDRGP4 s_startserver+1944
CNSTI4 0
ASGNI4
line 259
;259:	matchbits = 1 << gametype_remap[s_startserver.gametype.curvalue];
ADDRLP4 12
CNSTI4 1
ADDRGP4 s_startserver+536+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gametype_remap
ADDP4
INDIRI4
LSHI4
ASGNI4
line 260
;260:	if( gametype_remap[s_startserver.gametype.curvalue] == GT_FFA ) {
ADDRGP4 s_startserver+536+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gametype_remap
ADDP4
INDIRI4
CNSTI4 0
NEI4 $163
line 261
;261:		matchbits |= ( 1 << GT_SINGLE_PLAYER );
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 262
;262:	}
LABELV $163
line 263
;263:	for( i = 0; i < count; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $170
JUMPV
LABELV $167
line 264
;264:		info = UI_GetArenaInfoByNumber( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_GetArenaInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 24
INDIRP4
ASGNP4
line 266
;265:
;266:		gamebits = GametypeBits( Info_ValueForKey( info, "type") );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $171
ARGP4
ADDRLP4 28
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 GametypeBits
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 32
INDIRI4
ASGNI4
line 267
;267:		if( !( gamebits & matchbits ) ) {
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
BANDI4
CNSTI4 0
NEI4 $172
line 268
;268:			continue;
ADDRGP4 $168
JUMPV
LABELV $172
line 271
;269:		}
;270:
;271:		Q_strncpyz( s_startserver.maplist[s_startserver.nummaps], Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $176
ARGP4
ADDRLP4 36
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 s_startserver+1944
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 272
;272:		Q_strupr( s_startserver.maplist[s_startserver.nummaps] );
ADDRGP4 s_startserver+1944
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 273
;273:		s_startserver.mapGamebits[s_startserver.nummaps] = gamebits;
ADDRGP4 s_startserver+1944
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_startserver+2980
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 274
;274:		s_startserver.nummaps++;
ADDRLP4 40
ADDRGP4 s_startserver+1944
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 275
;275:	}
LABELV $168
line 263
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $170
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $167
line 276
;276:	s_startserver.maxpages = (s_startserver.nummaps + MAX_MAPSPERPAGE-1)/MAX_MAPSPERPAGE;
ADDRLP4 24
CNSTI4 4
ASGNI4
ADDRGP4 s_startserver+1952
ADDRGP4 s_startserver+1944
INDIRI4
ADDRLP4 24
INDIRI4
ADDI4
CNSTI4 1
SUBI4
ADDRLP4 24
INDIRI4
DIVI4
ASGNI4
line 277
;277:	s_startserver.page = 0;
ADDRGP4 s_startserver+1948
CNSTI4 0
ASGNI4
line 278
;278:	s_startserver.currentmap = 0;
ADDRGP4 s_startserver+1940
CNSTI4 0
ASGNI4
line 280
;279:
;280:	StartServer_Update();
ADDRGP4 StartServer_Update
CALLV
pop
line 281
;281:}
LABELV $157
endproc StartServer_GametypeEvent 44 12
proc StartServer_MenuEvent 12 8
line 289
;282:
;283:
;284:/*
;285:=================
;286:StartServer_MenuEvent
;287:=================
;288:*/
;289:static void StartServer_MenuEvent( void* ptr, int event ) {
line 290
;290:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $187
line 291
;291:		return;
ADDRGP4 $186
JUMPV
LABELV $187
line 294
;292:	}
;293:
;294:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 15
LTI4 $189
ADDRLP4 0
INDIRI4
CNSTI4 18
GTI4 $189
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $209-60
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $209
address $192
address $197
address $208
address $203
code
LABELV $192
line 296
;295:	case ID_PREVPAGE:
;296:		if( s_startserver.page > 0 ) {
ADDRGP4 s_startserver+1948
INDIRI4
CNSTI4 0
LEI4 $190
line 297
;297:			s_startserver.page--;
ADDRLP4 8
ADDRGP4 s_startserver+1948
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 298
;298:			StartServer_Update();
ADDRGP4 StartServer_Update
CALLV
pop
line 299
;299:		}
line 300
;300:		break;
ADDRGP4 $190
JUMPV
LABELV $197
line 303
;301:
;302:	case ID_NEXTPAGE:
;303:		if( s_startserver.page < s_startserver.maxpages - 1 ) {
ADDRGP4 s_startserver+1948
INDIRI4
ADDRGP4 s_startserver+1952
INDIRI4
CNSTI4 1
SUBI4
GEI4 $190
line 304
;304:			s_startserver.page++;
ADDRLP4 8
ADDRGP4 s_startserver+1948
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 305
;305:			StartServer_Update();
ADDRGP4 StartServer_Update
CALLV
pop
line 306
;306:		}
line 307
;307:		break;
ADDRGP4 $190
JUMPV
LABELV $203
line 310
;308:
;309:	case ID_STARTSERVERNEXT:
;310:		trap_Cvar_SetValue( "g_gameType", gametype_remap[s_startserver.gametype.curvalue] );
ADDRGP4 $204
ARGP4
ADDRGP4 s_startserver+536+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gametype_remap
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 311
;311:		UI_ServerOptionsMenu( s_startserver.multiplayer );
ADDRGP4 s_startserver+1936
INDIRI4
ARGI4
ADDRGP4 UI_ServerOptionsMenu
CALLV
pop
line 312
;312:		break;
ADDRGP4 $190
JUMPV
LABELV $208
line 315
;313:
;314:	case ID_STARTSERVERBACK:
;315:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 316
;316:		break;
LABELV $189
LABELV $190
line 318
;317:	}
;318:}
LABELV $186
endproc StartServer_MenuEvent 12 8
proc StartServer_LevelshotDraw 48 20
line 326
;319:
;320:
;321:/*
;322:===============
;323:StartServer_LevelshotDraw
;324:===============
;325:*/
;326:static void StartServer_LevelshotDraw( void *self ) {
line 334
;327:	menubitmap_s	*b;
;328:	int				x;
;329:	int				y;
;330:	int				w;
;331:	int				h;
;332:	int				n;
;333:
;334:	b = (menubitmap_s *)self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 336
;335:
;336:	if( !b->generic.name ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $212
line 337
;337:		return;
ADDRGP4 $211
JUMPV
LABELV $212
line 340
;338:	}
;339:
;340:	if( b->generic.name && !b->shader ) {
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $214
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 0
NEI4 $214
line 341
;341:		b->shader = trap_R_RegisterShaderNoMip( b->generic.name );
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
line 342
;342:		if( !b->shader && b->errorpic ) {
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 0
NEI4 $216
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $216
line 343
;343:			b->shader = trap_R_RegisterShaderNoMip( b->errorpic );
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 344
;344:		}
LABELV $216
line 345
;345:	}
LABELV $214
line 347
;346:
;347:	if( b->focuspic && !b->focusshader ) {
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $218
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
CNSTI4 0
NEI4 $218
line 348
;348:		b->focusshader = trap_R_RegisterShaderNoMip( b->focuspic );
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 349
;349:	}
LABELV $218
line 351
;350:
;351:	x = b->generic.x;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 352
;352:	y = b->generic.y;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 353
;353:	w = b->width;
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
ASGNI4
line 354
;354:	h =	b->height;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
ASGNI4
line 355
;355:	if( b->shader ) {
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 0
EQI4 $220
line 356
;356:		UI_DrawHandlePic( x, y, w, h, b->shader );
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 16
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 357
;357:	}
LABELV $220
line 359
;358:
;359:	x = b->generic.x;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 360
;360:	y = b->generic.y + b->height;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
ADDI4
ASGNI4
line 361
;361:	UI_FillRect( x, y, b->width, 28, colorBlack );
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
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
CNSTF4 1105199104
ARGF4
ADDRGP4 colorBlack
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 363
;362:
;363:	x += b->width / 2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
CNSTI4 2
DIVI4
ADDI4
ASGNI4
line 364
;364:	y += 4;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 4
ADDI4
ASGNI4
line 365
;365:	n = s_startserver.page * MAX_MAPSPERPAGE + b->generic.id - ID_PICTURES;
ADDRLP4 20
ADDRGP4 s_startserver+1948
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
CNSTI4 11
SUBI4
ASGNI4
line 366
;366:	UI_DrawString( x, y, s_startserver.maplist[n], UI_CENTER|UI_SMALLFONT, color_orange );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 20
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 368
;367:
;368:	x = b->generic.x;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 369
;369:	y = b->generic.y;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 370
;370:	w = b->width;
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
ASGNI4
line 371
;371:	h =	b->height + 28;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
CNSTI4 28
ADDI4
ASGNI4
line 372
;372:	if( b->generic.flags & QMF_HIGHLIGHT ) {	
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 64
BANDU4
CNSTU4 0
EQU4 $224
line 373
;373:		UI_DrawHandlePic( x, y, w, h, b->focusshader );
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 16
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 374
;374:	}
LABELV $224
line 375
;375:}
LABELV $211
endproc StartServer_LevelshotDraw 48 20
bss
align 1
LABELV $227
skip 64
code
proc StartServer_MenuInit 20 12
line 383
;376:
;377:
;378:/*
;379:=================
;380:StartServer_MenuInit
;381:=================
;382:*/
;383:static void StartServer_MenuInit( void ) {
line 390
;384:	int	i;
;385:	int	x;
;386:	int	y;
;387:	static char mapnamebuffer[64];
;388:
;389:	// zero set all our globals
;390:	memset( &s_startserver, 0 ,sizeof(startserver_t) );
ADDRGP4 s_startserver
ARGP4
CNSTI4 0
ARGI4
CNSTI4 3236
ARGI4
ADDRGP4 memset
CALLP4
pop
line 392
;391:
;392:	StartServer_Cache();
ADDRGP4 StartServer_Cache
CALLV
pop
line 394
;393:
;394:	s_startserver.menu.wrapAround = qtrue;
ADDRGP4 s_startserver+276
CNSTI4 1
ASGNI4
line 395
;395:	s_startserver.menu.fullscreen = qtrue;
ADDRGP4 s_startserver+280
CNSTI4 1
ASGNI4
line 397
;396:
;397:	s_startserver.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 s_startserver+288
CNSTI4 10
ASGNI4
line 398
;398:	s_startserver.banner.generic.x	   = 320;
ADDRGP4 s_startserver+288+12
CNSTI4 320
ASGNI4
line 399
;399:	s_startserver.banner.generic.y	   = 16;
ADDRGP4 s_startserver+288+16
CNSTI4 16
ASGNI4
line 400
;400:	s_startserver.banner.string        = "GAME SERVER";
ADDRGP4 s_startserver+288+60
ADDRGP4 $237
ASGNP4
line 401
;401:	s_startserver.banner.color         = color_white;
ADDRGP4 s_startserver+288+68
ADDRGP4 color_white
ASGNP4
line 402
;402:	s_startserver.banner.style         = UI_CENTER;
ADDRGP4 s_startserver+288+64
CNSTI4 1
ASGNI4
line 404
;403:
;404:	s_startserver.framel.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_startserver+360
CNSTI4 6
ASGNI4
line 405
;405:	s_startserver.framel.generic.name  = GAMESERVER_FRAMEL;
ADDRGP4 s_startserver+360+4
ADDRGP4 $245
ASGNP4
line 406
;406:	s_startserver.framel.generic.flags = QMF_INACTIVE;
ADDRGP4 s_startserver+360+44
CNSTU4 16384
ASGNU4
line 407
;407:	s_startserver.framel.generic.x	   = 0;  
ADDRGP4 s_startserver+360+12
CNSTI4 0
ASGNI4
line 408
;408:	s_startserver.framel.generic.y	   = 78;
ADDRGP4 s_startserver+360+16
CNSTI4 78
ASGNI4
line 409
;409:	s_startserver.framel.width  	   = 256;
ADDRGP4 s_startserver+360+76
CNSTI4 256
ASGNI4
line 410
;410:	s_startserver.framel.height  	   = 329;
ADDRGP4 s_startserver+360+80
CNSTI4 329
ASGNI4
line 412
;411:
;412:	s_startserver.framer.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_startserver+448
CNSTI4 6
ASGNI4
line 413
;413:	s_startserver.framer.generic.name  = GAMESERVER_FRAMER;
ADDRGP4 s_startserver+448+4
ADDRGP4 $259
ASGNP4
line 414
;414:	s_startserver.framer.generic.flags = QMF_INACTIVE;
ADDRGP4 s_startserver+448+44
CNSTU4 16384
ASGNU4
line 415
;415:	s_startserver.framer.generic.x	   = 376;
ADDRGP4 s_startserver+448+12
CNSTI4 376
ASGNI4
line 416
;416:	s_startserver.framer.generic.y	   = 76;
ADDRGP4 s_startserver+448+16
CNSTI4 76
ASGNI4
line 417
;417:	s_startserver.framer.width  	   = 256;
ADDRGP4 s_startserver+448+76
CNSTI4 256
ASGNI4
line 418
;418:	s_startserver.framer.height  	   = 334;
ADDRGP4 s_startserver+448+80
CNSTI4 334
ASGNI4
line 420
;419:
;420:	s_startserver.gametype.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 s_startserver+536
CNSTI4 3
ASGNI4
line 421
;421:	s_startserver.gametype.generic.name		= "Game Type:";
ADDRGP4 s_startserver+536+4
ADDRGP4 $273
ASGNP4
line 422
;422:	s_startserver.gametype.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_startserver+536+44
CNSTU4 258
ASGNU4
line 423
;423:	s_startserver.gametype.generic.callback	= StartServer_GametypeEvent;
ADDRGP4 s_startserver+536+48
ADDRGP4 StartServer_GametypeEvent
ASGNP4
line 424
;424:	s_startserver.gametype.generic.id		= ID_GAMETYPE;
ADDRGP4 s_startserver+536+8
CNSTI4 10
ASGNI4
line 425
;425:	s_startserver.gametype.generic.x		= 320 - 24;
ADDRGP4 s_startserver+536+12
CNSTI4 296
ASGNI4
line 426
;426:	s_startserver.gametype.generic.y		= 368;
ADDRGP4 s_startserver+536+16
CNSTI4 368
ASGNI4
line 427
;427:	s_startserver.gametype.itemnames		= gametype_items;
ADDRGP4 s_startserver+536+76
ADDRGP4 gametype_items
ASGNP4
line 429
;428:
;429:	for (i=0; i<MAX_MAPSPERPAGE; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $286
line 430
;430:	{
line 431
;431:		x =	(i % MAX_MAPCOLS) * (128+8) + 188;
ADDRLP4 4
CNSTI4 136
ADDRLP4 0
INDIRI4
CNSTI4 2
MODI4
MULI4
CNSTI4 188
ADDI4
ASGNI4
line 432
;432:		y = (i / MAX_MAPROWS) * (128+8) + 96;
ADDRLP4 8
CNSTI4 136
ADDRLP4 0
INDIRI4
CNSTI4 2
DIVI4
MULI4
CNSTI4 96
ADDI4
ASGNI4
line 434
;433:
;434:		s_startserver.mappics[i].generic.type   = MTYPE_BITMAP;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632
ADDP4
CNSTI4 6
ASGNI4
line 435
;435:		s_startserver.mappics[i].generic.flags  = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+44
ADDP4
CNSTU4 16388
ASGNU4
line 436
;436:		s_startserver.mappics[i].generic.x	    = x;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+12
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 437
;437:		s_startserver.mappics[i].generic.y	    = y;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+16
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 438
;438:		s_startserver.mappics[i].generic.id		= ID_PICTURES+i;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 11
ADDI4
ASGNI4
line 439
;439:		s_startserver.mappics[i].width  		= 128;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+76
ADDP4
CNSTI4 128
ASGNI4
line 440
;440:		s_startserver.mappics[i].height  	    = 96;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+80
ADDP4
CNSTI4 96
ASGNI4
line 441
;441:		s_startserver.mappics[i].focuspic       = GAMESERVER_SELECTED;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+60
ADDP4
ADDRGP4 $305
ASGNP4
line 442
;442:		s_startserver.mappics[i].errorpic       = GAMESERVER_UNKNOWNMAP;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+64
ADDP4
ADDRGP4 $308
ASGNP4
line 443
;443:		s_startserver.mappics[i].generic.ownerdraw = StartServer_LevelshotDraw;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632+56
ADDP4
ADDRGP4 StartServer_LevelshotDraw
ASGNP4
line 445
;444:
;445:		s_startserver.mapbuttons[i].generic.type     = MTYPE_BITMAP;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984
ADDP4
CNSTI4 6
ASGNI4
line 446
;446:		s_startserver.mapbuttons[i].generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_NODEFAULTINIT;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+44
ADDP4
CNSTU4 33028
ASGNU4
line 447
;447:		s_startserver.mapbuttons[i].generic.id       = ID_PICTURES+i;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 11
ADDI4
ASGNI4
line 448
;448:		s_startserver.mapbuttons[i].generic.callback = StartServer_MapEvent;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+48
ADDP4
ADDRGP4 StartServer_MapEvent
ASGNP4
line 449
;449:		s_startserver.mapbuttons[i].generic.x	     = x - 30;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+12
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 30
SUBI4
ASGNI4
line 450
;450:		s_startserver.mapbuttons[i].generic.y	     = y - 32;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+16
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
line 451
;451:		s_startserver.mapbuttons[i].width  		     = 256;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+76
ADDP4
CNSTI4 256
ASGNI4
line 452
;452:		s_startserver.mapbuttons[i].height  	     = 248;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+80
ADDP4
CNSTI4 248
ASGNI4
line 453
;453:		s_startserver.mapbuttons[i].generic.left     = x;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+20
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 454
;454:		s_startserver.mapbuttons[i].generic.top  	 = y;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+24
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 455
;455:		s_startserver.mapbuttons[i].generic.right    = x + 128;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+28
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 128
ADDI4
ASGNI4
line 456
;456:		s_startserver.mapbuttons[i].generic.bottom   = y + 128;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+32
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 128
ADDI4
ASGNI4
line 457
;457:		s_startserver.mapbuttons[i].focuspic         = GAMESERVER_SELECT;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984+60
ADDP4
ADDRGP4 $336
ASGNP4
line 458
;458:	}
LABELV $287
line 429
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $286
line 460
;459:
;460:	s_startserver.arrows.generic.type  = MTYPE_BITMAP;
ADDRGP4 s_startserver+1336
CNSTI4 6
ASGNI4
line 461
;461:	s_startserver.arrows.generic.name  = GAMESERVER_ARROWS;
ADDRGP4 s_startserver+1336+4
ADDRGP4 $340
ASGNP4
line 462
;462:	s_startserver.arrows.generic.flags = QMF_INACTIVE;
ADDRGP4 s_startserver+1336+44
CNSTU4 16384
ASGNU4
line 463
;463:	s_startserver.arrows.generic.x	   = 260;
ADDRGP4 s_startserver+1336+12
CNSTI4 260
ASGNI4
line 464
;464:	s_startserver.arrows.generic.y	   = 400;
ADDRGP4 s_startserver+1336+16
CNSTI4 400
ASGNI4
line 465
;465:	s_startserver.arrows.width  	   = 128;
ADDRGP4 s_startserver+1336+76
CNSTI4 128
ASGNI4
line 466
;466:	s_startserver.arrows.height  	   = 32;
ADDRGP4 s_startserver+1336+80
CNSTI4 32
ASGNI4
line 468
;467:
;468:	s_startserver.prevpage.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_startserver+1424
CNSTI4 6
ASGNI4
line 469
;469:	s_startserver.prevpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_startserver+1424+44
CNSTU4 260
ASGNU4
line 470
;470:	s_startserver.prevpage.generic.callback = StartServer_MenuEvent;
ADDRGP4 s_startserver+1424+48
ADDRGP4 StartServer_MenuEvent
ASGNP4
line 471
;471:	s_startserver.prevpage.generic.id	    = ID_PREVPAGE;
ADDRGP4 s_startserver+1424+8
CNSTI4 15
ASGNI4
line 472
;472:	s_startserver.prevpage.generic.x		= 260;
ADDRGP4 s_startserver+1424+12
CNSTI4 260
ASGNI4
line 473
;473:	s_startserver.prevpage.generic.y		= 400;
ADDRGP4 s_startserver+1424+16
CNSTI4 400
ASGNI4
line 474
;474:	s_startserver.prevpage.width  		    = 64;
ADDRGP4 s_startserver+1424+76
CNSTI4 64
ASGNI4
line 475
;475:	s_startserver.prevpage.height  		    = 32;
ADDRGP4 s_startserver+1424+80
CNSTI4 32
ASGNI4
line 476
;476:	s_startserver.prevpage.focuspic         = GAMESERVER_ARROWSL;
ADDRGP4 s_startserver+1424+60
ADDRGP4 $368
ASGNP4
line 478
;477:
;478:	s_startserver.nextpage.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_startserver+1512
CNSTI4 6
ASGNI4
line 479
;479:	s_startserver.nextpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_startserver+1512+44
CNSTU4 260
ASGNU4
line 480
;480:	s_startserver.nextpage.generic.callback = StartServer_MenuEvent;
ADDRGP4 s_startserver+1512+48
ADDRGP4 StartServer_MenuEvent
ASGNP4
line 481
;481:	s_startserver.nextpage.generic.id	    = ID_NEXTPAGE;
ADDRGP4 s_startserver+1512+8
CNSTI4 16
ASGNI4
line 482
;482:	s_startserver.nextpage.generic.x		= 321;
ADDRGP4 s_startserver+1512+12
CNSTI4 321
ASGNI4
line 483
;483:	s_startserver.nextpage.generic.y		= 400;
ADDRGP4 s_startserver+1512+16
CNSTI4 400
ASGNI4
line 484
;484:	s_startserver.nextpage.width  		    = 64;
ADDRGP4 s_startserver+1512+76
CNSTI4 64
ASGNI4
line 485
;485:	s_startserver.nextpage.height  		    = 32;
ADDRGP4 s_startserver+1512+80
CNSTI4 32
ASGNI4
line 486
;486:	s_startserver.nextpage.focuspic         = GAMESERVER_ARROWSR;
ADDRGP4 s_startserver+1512+60
ADDRGP4 $386
ASGNP4
line 488
;487:
;488:	s_startserver.mapname.generic.type  = MTYPE_PTEXT;
ADDRGP4 s_startserver+1776
CNSTI4 9
ASGNI4
line 489
;489:	s_startserver.mapname.generic.flags = QMF_CENTER_JUSTIFY|QMF_INACTIVE;
ADDRGP4 s_startserver+1776+44
CNSTU4 16392
ASGNU4
line 490
;490:	s_startserver.mapname.generic.x	    = 320;
ADDRGP4 s_startserver+1776+12
CNSTI4 320
ASGNI4
line 491
;491:	s_startserver.mapname.generic.y	    = 440;
ADDRGP4 s_startserver+1776+16
CNSTI4 440
ASGNI4
line 492
;492:	s_startserver.mapname.string        = mapnamebuffer;
ADDRGP4 s_startserver+1776+60
ADDRGP4 $227
ASGNP4
line 493
;493:	s_startserver.mapname.style         = UI_CENTER|UI_BIGFONT;
ADDRGP4 s_startserver+1776+64
CNSTI4 33
ASGNI4
line 494
;494:	s_startserver.mapname.color         = text_color_normal;
ADDRGP4 s_startserver+1776+68
ADDRGP4 text_color_normal
ASGNP4
line 496
;495:
;496:	s_startserver.back.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_startserver+1600
CNSTI4 6
ASGNI4
line 497
;497:	s_startserver.back.generic.name     = GAMESERVER_BACK0;
ADDRGP4 s_startserver+1600+4
ADDRGP4 $403
ASGNP4
line 498
;498:	s_startserver.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_startserver+1600+44
CNSTU4 260
ASGNU4
line 499
;499:	s_startserver.back.generic.callback = StartServer_MenuEvent;
ADDRGP4 s_startserver+1600+48
ADDRGP4 StartServer_MenuEvent
ASGNP4
line 500
;500:	s_startserver.back.generic.id	    = ID_STARTSERVERBACK;
ADDRGP4 s_startserver+1600+8
CNSTI4 17
ASGNI4
line 501
;501:	s_startserver.back.generic.x		= 0;
ADDRGP4 s_startserver+1600+12
CNSTI4 0
ASGNI4
line 502
;502:	s_startserver.back.generic.y		= 480-64;
ADDRGP4 s_startserver+1600+16
CNSTI4 416
ASGNI4
line 503
;503:	s_startserver.back.width  		    = 128;
ADDRGP4 s_startserver+1600+76
CNSTI4 128
ASGNI4
line 504
;504:	s_startserver.back.height  		    = 64;
ADDRGP4 s_startserver+1600+80
CNSTI4 64
ASGNI4
line 505
;505:	s_startserver.back.focuspic         = GAMESERVER_BACK1;
ADDRGP4 s_startserver+1600+60
ADDRGP4 $420
ASGNP4
line 507
;506:
;507:	s_startserver.next.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_startserver+1688
CNSTI4 6
ASGNI4
line 508
;508:	s_startserver.next.generic.name     = GAMESERVER_NEXT0;
ADDRGP4 s_startserver+1688+4
ADDRGP4 $424
ASGNP4
line 509
;509:	s_startserver.next.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_startserver+1688+44
CNSTU4 272
ASGNU4
line 510
;510:	s_startserver.next.generic.callback = StartServer_MenuEvent;
ADDRGP4 s_startserver+1688+48
ADDRGP4 StartServer_MenuEvent
ASGNP4
line 511
;511:	s_startserver.next.generic.id	    = ID_STARTSERVERNEXT;
ADDRGP4 s_startserver+1688+8
CNSTI4 18
ASGNI4
line 512
;512:	s_startserver.next.generic.x		= 640;
ADDRGP4 s_startserver+1688+12
CNSTI4 640
ASGNI4
line 513
;513:	s_startserver.next.generic.y		= 480-64;
ADDRGP4 s_startserver+1688+16
CNSTI4 416
ASGNI4
line 514
;514:	s_startserver.next.width  		    = 128;
ADDRGP4 s_startserver+1688+76
CNSTI4 128
ASGNI4
line 515
;515:	s_startserver.next.height  		    = 64;
ADDRGP4 s_startserver+1688+80
CNSTI4 64
ASGNI4
line 516
;516:	s_startserver.next.focuspic         = GAMESERVER_NEXT1;
ADDRGP4 s_startserver+1688+60
ADDRGP4 $441
ASGNP4
line 518
;517:
;518:	s_startserver.item_null.generic.type	= MTYPE_BITMAP;
ADDRGP4 s_startserver+1848
CNSTI4 6
ASGNI4
line 519
;519:	s_startserver.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
ADDRGP4 s_startserver+1848+44
CNSTU4 1050628
ASGNU4
line 520
;520:	s_startserver.item_null.generic.x		= 0;
ADDRGP4 s_startserver+1848+12
CNSTI4 0
ASGNI4
line 521
;521:	s_startserver.item_null.generic.y		= 0;
ADDRGP4 s_startserver+1848+16
CNSTI4 0
ASGNI4
line 522
;522:	s_startserver.item_null.width			= 640;
ADDRGP4 s_startserver+1848+76
CNSTI4 640
ASGNI4
line 523
;523:	s_startserver.item_null.height			= 480;
ADDRGP4 s_startserver+1848+80
CNSTI4 480
ASGNI4
line 525
;524:
;525:	Menu_AddItem( &s_startserver.menu, &s_startserver.banner );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 526
;526:	Menu_AddItem( &s_startserver.menu, &s_startserver.framel );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 527
;527:	Menu_AddItem( &s_startserver.menu, &s_startserver.framer );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 529
;528:
;529:	Menu_AddItem( &s_startserver.menu, &s_startserver.gametype );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 530
;530:	for (i=0; i<MAX_MAPSPERPAGE; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $457
line 531
;531:	{
line 532
;532:		Menu_AddItem( &s_startserver.menu, &s_startserver.mappics[i] );
ADDRGP4 s_startserver
ARGP4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+632
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 533
;533:		Menu_AddItem( &s_startserver.menu, &s_startserver.mapbuttons[i] );
ADDRGP4 s_startserver
ARGP4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_startserver+984
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 534
;534:	}
LABELV $458
line 530
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $457
line 536
;535:
;536:	Menu_AddItem( &s_startserver.menu, &s_startserver.arrows );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1336
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 537
;537:	Menu_AddItem( &s_startserver.menu, &s_startserver.prevpage );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1424
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 538
;538:	Menu_AddItem( &s_startserver.menu, &s_startserver.nextpage );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1512
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 539
;539:	Menu_AddItem( &s_startserver.menu, &s_startserver.back );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1600
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 540
;540:	Menu_AddItem( &s_startserver.menu, &s_startserver.next );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1688
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 541
;541:	Menu_AddItem( &s_startserver.menu, &s_startserver.mapname );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1776
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 542
;542:	Menu_AddItem( &s_startserver.menu, &s_startserver.item_null );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 s_startserver+1848
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 544
;543:
;544:	StartServer_GametypeEvent( NULL, QM_ACTIVATED );
CNSTP4 0
ARGP4
CNSTI4 3
ARGI4
ADDRGP4 StartServer_GametypeEvent
CALLV
pop
line 545
;545:}
LABELV $226
endproc StartServer_MenuInit 20 12
export StartServer_Cache
proc StartServer_Cache 100 16
line 554
;546:
;547:
;548:/*
;549:=================
;550:StartServer_Cache
;551:=================
;552:*/
;553:void StartServer_Cache( void )
;554:{
line 560
;555:	int				i;
;556:	const char		*info;
;557:	qboolean		precache;
;558:	char			picname[64];
;559:
;560:	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );	
ADDRGP4 $403
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 561
;561:	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );	
ADDRGP4 $420
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 562
;562:	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT0 );	
ADDRGP4 $424
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 563
;563:	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT1 );	
ADDRGP4 $441
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 564
;564:	trap_R_RegisterShaderNoMip( GAMESERVER_FRAMEL );	
ADDRGP4 $245
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 565
;565:	trap_R_RegisterShaderNoMip( GAMESERVER_FRAMER );	
ADDRGP4 $259
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 566
;566:	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );	
ADDRGP4 $336
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 567
;567:	trap_R_RegisterShaderNoMip( GAMESERVER_SELECTED );	
ADDRGP4 $305
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 568
;568:	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
ADDRGP4 $308
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 569
;569:	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWS );
ADDRGP4 $340
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 570
;570:	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSL );
ADDRGP4 $368
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 571
;571:	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSR );
ADDRGP4 $386
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 573
;572:
;573:	precache = trap_Cvar_VariableValue("com_buildscript");
ADDRGP4 $471
ARGP4
ADDRLP4 76
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 72
ADDRLP4 76
INDIRF4
CVFI4 4
ASGNI4
line 575
;574:
;575:	s_startserver.nummaps = UI_GetNumArenas();
ADDRLP4 80
ADDRGP4 UI_GetNumArenas
CALLI4
ASGNI4
ADDRGP4 s_startserver+1944
ADDRLP4 80
INDIRI4
ASGNI4
line 577
;576:
;577:	for( i = 0; i < s_startserver.nummaps; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $476
JUMPV
LABELV $473
line 578
;578:		info = UI_GetArenaInfoByNumber( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 84
ADDRGP4 UI_GetArenaInfoByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 84
INDIRP4
ASGNP4
line 580
;579:
;580:		Q_strncpyz( s_startserver.maplist[i], Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $176
ARGP4
ADDRLP4 88
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 581
;581:		Q_strupr( s_startserver.maplist[i] );
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 582
;582:		s_startserver.mapGamebits[i] = GametypeBits( Info_ValueForKey( info, "type") );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $171
ARGP4
ADDRLP4 92
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRLP4 96
ADDRGP4 GametypeBits
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 s_startserver+2980
ADDP4
ADDRLP4 96
INDIRI4
ASGNI4
line 584
;583:
;584:		if( precache ) {
ADDRLP4 72
INDIRI4
CNSTI4 0
EQI4 $481
line 585
;585:			Com_sprintf( picname, sizeof(picname), "levelshots/%s", s_startserver.maplist[i] );
ADDRLP4 8
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $103
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 586
;586:			trap_R_RegisterShaderNoMip(picname);
ADDRLP4 8
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 587
;587:		}
LABELV $481
line 588
;588:	}
LABELV $474
line 577
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $476
ADDRLP4 0
INDIRI4
ADDRGP4 s_startserver+1944
INDIRI4
LTI4 $473
line 590
;589:
;590:	s_startserver.maxpages = (s_startserver.nummaps + MAX_MAPSPERPAGE-1)/MAX_MAPSPERPAGE;
ADDRLP4 84
CNSTI4 4
ASGNI4
ADDRGP4 s_startserver+1952
ADDRGP4 s_startserver+1944
INDIRI4
ADDRLP4 84
INDIRI4
ADDI4
CNSTI4 1
SUBI4
ADDRLP4 84
INDIRI4
DIVI4
ASGNI4
line 591
;591:}
LABELV $470
endproc StartServer_Cache 100 16
export UI_StartServerMenu
proc UI_StartServerMenu 0 4
line 599
;592:
;593:
;594:/*
;595:=================
;596:UI_StartServerMenu
;597:=================
;598:*/
;599:void UI_StartServerMenu( qboolean multiplayer ) {
line 600
;600:	StartServer_MenuInit();
ADDRGP4 StartServer_MenuInit
CALLV
pop
line 601
;601:	s_startserver.multiplayer = multiplayer;
ADDRGP4 s_startserver+1936
ADDRFP4 0
INDIRI4
ASGNI4
line 602
;602:	UI_PushMenu( &s_startserver.menu );
ADDRGP4 s_startserver
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 603
;603:}
LABELV $486
endproc UI_StartServerMenu 0 4
data
align 4
LABELV dedicated_list
address $489
address $490
address $491
byte 4 0
align 4
LABELV playerType_list
address $492
address $493
address $494
byte 4 0
align 4
LABELV playerTeam_list
address $495
address $496
byte 4 0
align 4
LABELV botSkill_list
address $497
address $498
address $499
address $500
address $501
byte 4 0
code
proc BotAlreadySelected 12 8
line 699
;604:
;605:
;606:
;607:/*
;608:=============================================================================
;609:
;610:SERVER OPTIONS MENU *****
;611:
;612:=============================================================================
;613:*/
;614:
;615:#define ID_PLAYER_TYPE			20
;616:#define ID_MAXCLIENTS			21
;617:#define ID_DEDICATED			22
;618:#define ID_GO					23
;619:#define ID_BACK					24
;620:
;621:#define PLAYER_SLOTS			12
;622:
;623:
;624:typedef struct {
;625:	menuframework_s		menu;
;626:
;627:	menutext_s			banner;
;628:
;629:	menubitmap_s		mappic;
;630:	menubitmap_s		picframe;
;631:
;632:	menulist_s			dedicated;
;633:	menufield_s			timelimit;
;634:	menufield_s			fraglimit;
;635:	menufield_s			flaglimit;
;636:	menuradiobutton_s	friendlyfire;
;637:	menufield_s			hostname;
;638:	menuradiobutton_s	pure;
;639:	menulist_s			botSkill;
;640:
;641:	menutext_s			player0;
;642:	menulist_s			playerType[PLAYER_SLOTS];
;643:	menutext_s			playerName[PLAYER_SLOTS];
;644:	menulist_s			playerTeam[PLAYER_SLOTS];
;645:
;646:	menubitmap_s		go;
;647:	menubitmap_s		next;
;648:	menubitmap_s		back;
;649:
;650:	qboolean			multiplayer;
;651:	int					gametype;
;652:	char				mapnamebuffer[32];
;653:	char				playerNameBuffers[PLAYER_SLOTS][16];
;654:
;655:	qboolean			newBot;
;656:	int					newBotIndex;
;657:	char				newBotName[16];
;658:	
;659:	menulist_s		punkbuster;
;660:} serveroptions_t;
;661:
;662:static serveroptions_t s_serveroptions;
;663:
;664:static const char *dedicated_list[] = {
;665:	"No",
;666:	"LAN",
;667:	"Internet",
;668:	0
;669:};
;670:
;671:static const char *playerType_list[] = {
;672:	"Open",
;673:	"Bot",
;674:	"----",
;675:	0
;676:};
;677:
;678:static const char *playerTeam_list[] = {
;679:	"Blue",
;680:	"Red",
;681:	0
;682:};
;683:
;684:static const char *botSkill_list[] = {
;685:	"I Can Win",
;686:	"Bring It On",
;687:	"Hurt Me Plenty",
;688:	"Hardcore",
;689:	"Nightmare!",
;690:	0
;691:};
;692:
;693:
;694:/*
;695:=================
;696:BotAlreadySelected
;697:=================
;698:*/
;699:static qboolean BotAlreadySelected( const char *checkName ) {
line 702
;700:	int		n;
;701:
;702:	for( n = 1; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $503
line 703
;703:		if( s_serveroptions.playerType[n].curvalue != 1 ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 1
EQI4 $507
line 704
;704:			continue;
ADDRGP4 $504
JUMPV
LABELV $507
line 706
;705:		}
;706:		if( (s_serveroptions.gametype >= GT_TEAM) &&
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $511
ADDRLP4 4
CNSTI4 96
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+64
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRGP4 s_serveroptions+5924
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+64
ADDP4
INDIRI4
EQI4 $511
line 707
;707:			(s_serveroptions.playerTeam[n].curvalue != s_serveroptions.playerTeam[s_serveroptions.newBotIndex].curvalue ) ) {
line 708
;708:			continue;
ADDRGP4 $504
JUMPV
LABELV $511
line 710
;709:		}
;710:		if( Q_stricmp( checkName, s_serveroptions.playerNameBuffers[n] ) == 0 ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $519
line 711
;711:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $502
JUMPV
LABELV $519
line 713
;712:		}
;713:	}
LABELV $504
line 702
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $503
line 715
;714:
;715:	return qfalse;
CNSTI4 0
RETI4
LABELV $502
endproc BotAlreadySelected 12 8
proc ServerOptions_Start 160 24
line 724
;716:}
;717:
;718:
;719:/*
;720:=================
;721:ServerOptions_Start
;722:=================
;723:*/
;724:static void ServerOptions_Start( void ) {
line 737
;725:	int		timelimit;
;726:	int		fraglimit;
;727:	int		maxclients;
;728:	int		dedicated;
;729:	int		friendlyfire;
;730:	int		flaglimit;
;731:	int		pure;
;732:	int		skill;
;733:	int		n;
;734:	char	buf[64];
;735:
;736:
;737:	timelimit	 = atoi( s_serveroptions.timelimit.field.buffer );
ADDRGP4 s_serveroptions+632+60+12
ARGP4
ADDRLP4 100
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 76
ADDRLP4 100
INDIRI4
ASGNI4
line 738
;738:	fraglimit	 = atoi( s_serveroptions.fraglimit.field.buffer );
ADDRGP4 s_serveroptions+964+60+12
ARGP4
ADDRLP4 104
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 80
ADDRLP4 104
INDIRI4
ASGNI4
line 739
;739:	flaglimit	 = atoi( s_serveroptions.flaglimit.field.buffer );
ADDRGP4 s_serveroptions+1296+60+12
ARGP4
ADDRLP4 108
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 88
ADDRLP4 108
INDIRI4
ASGNI4
line 740
;740:	dedicated	 = s_serveroptions.dedicated.curvalue;
ADDRLP4 84
ADDRGP4 s_serveroptions+536+64
INDIRI4
ASGNI4
line 741
;741:	friendlyfire = s_serveroptions.friendlyfire.curvalue;
ADDRLP4 92
ADDRGP4 s_serveroptions+1628+60
INDIRI4
ASGNI4
line 742
;742:	pure		 = s_serveroptions.pure.curvalue;
ADDRLP4 96
ADDRGP4 s_serveroptions+2024+60
INDIRI4
ASGNI4
line 743
;743:	skill		 = s_serveroptions.botSkill.curvalue + 1;
ADDRLP4 72
ADDRGP4 s_serveroptions+2088+64
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 746
;744:
;745:	//set maxclients
;746:	for( n = 0, maxclients = 0; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 112
CNSTI4 0
ASGNI4
ADDRLP4 0
ADDRLP4 112
INDIRI4
ASGNI4
ADDRLP4 68
ADDRLP4 112
INDIRI4
ASGNI4
ADDRGP4 $543
JUMPV
LABELV $540
line 747
;747:		if( s_serveroptions.playerType[n].curvalue == 2 ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 2
NEI4 $544
line 748
;748:			continue;
ADDRGP4 $541
JUMPV
LABELV $544
line 750
;749:		}
;750:		if( (s_serveroptions.playerType[n].curvalue == 1) && (s_serveroptions.playerNameBuffers[n][0] == 0) ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 1
NEI4 $548
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $548
line 751
;751:			continue;
ADDRGP4 $541
JUMPV
LABELV $548
line 753
;752:		}
;753:		maxclients++;
ADDRLP4 68
ADDRLP4 68
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 754
;754:	}
LABELV $541
line 746
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $543
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $540
line 756
;755:
;756:	switch( s_serveroptions.gametype ) {
ADDRLP4 116
ADDRGP4 s_serveroptions+5692
INDIRI4
ASGNI4
ADDRLP4 116
INDIRI4
CNSTI4 0
LTI4 $553
ADDRLP4 116
INDIRI4
CNSTI4 4
GTI4 $553
ADDRLP4 116
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $570
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $570
address $556
address $559
address $553
address $562
address $566
code
LABELV $556
LABELV $553
line 759
;757:	case GT_FFA:
;758:	default:
;759:		trap_Cvar_SetValue( "ui_ffa_fraglimit", fraglimit );
ADDRGP4 $557
ARGP4
ADDRLP4 80
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 760
;760:		trap_Cvar_SetValue( "ui_ffa_timelimit", timelimit );
ADDRGP4 $558
ARGP4
ADDRLP4 76
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 761
;761:		break;
ADDRGP4 $554
JUMPV
LABELV $559
line 764
;762:
;763:	case GT_TOURNAMENT:
;764:		trap_Cvar_SetValue( "ui_tourney_fraglimit", fraglimit );
ADDRGP4 $560
ARGP4
ADDRLP4 80
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 765
;765:		trap_Cvar_SetValue( "ui_tourney_timelimit", timelimit );
ADDRGP4 $561
ARGP4
ADDRLP4 76
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 766
;766:		break;
ADDRGP4 $554
JUMPV
LABELV $562
line 769
;767:
;768:	case GT_TEAM:
;769:		trap_Cvar_SetValue( "ui_team_fraglimit", fraglimit );
ADDRGP4 $563
ARGP4
ADDRLP4 80
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 770
;770:		trap_Cvar_SetValue( "ui_team_timelimit", timelimit );
ADDRGP4 $564
ARGP4
ADDRLP4 76
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 771
;771:		trap_Cvar_SetValue( "ui_team_friendlt", friendlyfire );
ADDRGP4 $565
ARGP4
ADDRLP4 92
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 772
;772:		break;
ADDRGP4 $554
JUMPV
LABELV $566
line 775
;773:
;774:	case GT_CTF:
;775:		trap_Cvar_SetValue( "ui_ctf_fraglimit", fraglimit );
ADDRGP4 $567
ARGP4
ADDRLP4 80
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 776
;776:		trap_Cvar_SetValue( "ui_ctf_timelimit", timelimit );
ADDRGP4 $568
ARGP4
ADDRLP4 76
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 777
;777:		trap_Cvar_SetValue( "ui_ctf_friendlt", friendlyfire );
ADDRGP4 $569
ARGP4
ADDRLP4 92
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 778
;778:		break;
LABELV $554
line 781
;779:	}
;780:
;781:	trap_Cvar_SetValue( "sv_maxclients", Com_Clamp( 0, 12, maxclients ) );
CNSTF4 0
ARGF4
CNSTF4 1094713344
ARGF4
ADDRLP4 68
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 120
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $571
ARGP4
ADDRLP4 120
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 782
;782:	trap_Cvar_SetValue( "dedicated", Com_Clamp( 0, 2, dedicated ) );
CNSTF4 0
ARGF4
CNSTF4 1073741824
ARGF4
ADDRLP4 84
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 124
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $572
ARGP4
ADDRLP4 124
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 783
;783:	trap_Cvar_SetValue ("timelimit", Com_Clamp( 0, timelimit, timelimit ) );
CNSTF4 0
ARGF4
ADDRLP4 128
ADDRLP4 76
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 128
INDIRF4
ARGF4
ADDRLP4 128
INDIRF4
ARGF4
ADDRLP4 132
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $573
ARGP4
ADDRLP4 132
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 784
;784:	trap_Cvar_SetValue ("fraglimit", Com_Clamp( 0, fraglimit, fraglimit ) );
CNSTF4 0
ARGF4
ADDRLP4 136
ADDRLP4 80
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 136
INDIRF4
ARGF4
ADDRLP4 136
INDIRF4
ARGF4
ADDRLP4 140
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $574
ARGP4
ADDRLP4 140
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 785
;785:	trap_Cvar_SetValue ("capturelimit", Com_Clamp( 0, flaglimit, flaglimit ) );
CNSTF4 0
ARGF4
ADDRLP4 144
ADDRLP4 88
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 144
INDIRF4
ARGF4
ADDRLP4 144
INDIRF4
ARGF4
ADDRLP4 148
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 $575
ARGP4
ADDRLP4 148
INDIRF4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 786
;786:	trap_Cvar_SetValue( "g_friendlyfire", friendlyfire );
ADDRGP4 $576
ARGP4
ADDRLP4 92
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 787
;787:	trap_Cvar_SetValue( "sv_pure", pure );
ADDRGP4 $577
ARGP4
ADDRLP4 96
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 788
;788:	trap_Cvar_Set("sv_hostname", s_serveroptions.hostname.field.buffer );
ADDRGP4 $578
ARGP4
ADDRGP4 s_serveroptions+1692+60+12
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 790
;789:	
;790:	trap_Cvar_SetValue( "sv_punkbuster", s_serveroptions.punkbuster.curvalue );
ADDRGP4 $582
ARGP4
ADDRGP4 s_serveroptions+5944+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 793
;791:
;792:	// the wait commands will allow the dedicated to take effect
;793:	trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", s_startserver.maplist[s_startserver.currentmap] ) );
ADDRGP4 $585
ARGP4
ADDRGP4 s_startserver+1940
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRLP4 152
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 152
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 796
;794:
;795:	// add bots
;796:	trap_Cmd_ExecuteText( EXEC_APPEND, "wait 3\n" );
CNSTI4 2
ARGI4
ADDRGP4 $588
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 797
;797:	for( n = 1; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $589
line 798
;798:		if( s_serveroptions.playerType[n].curvalue != 1 ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 1
EQI4 $593
line 799
;799:			continue;
ADDRGP4 $590
JUMPV
LABELV $593
line 801
;800:		}
;801:		if( s_serveroptions.playerNameBuffers[n][0] == 0 ) {
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $597
line 802
;802:			continue;
ADDRGP4 $590
JUMPV
LABELV $597
line 804
;803:		}
;804:		if( s_serveroptions.playerNameBuffers[n][0] == '-' ) {
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
INDIRI1
CVII4 1
CNSTI4 45
NEI4 $600
line 805
;805:			continue;
ADDRGP4 $590
JUMPV
LABELV $600
line 807
;806:		}
;807:		if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $603
line 808
;808:			Com_sprintf( buf, sizeof(buf), "addbot %s %i %s\n", s_serveroptions.playerNameBuffers[n], skill,
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $606
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRLP4 72
INDIRI4
ARGI4
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+64
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 playerTeam_list
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 810
;809:				playerTeam_list[s_serveroptions.playerTeam[n].curvalue] );
;810:		}
ADDRGP4 $604
JUMPV
LABELV $603
line 811
;811:		else {
line 812
;812:			Com_sprintf( buf, sizeof(buf), "addbot %s %i\n", s_serveroptions.playerNameBuffers[n], skill );
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $610
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 813
;813:		}
LABELV $604
line 814
;814:		trap_Cmd_ExecuteText( EXEC_APPEND, buf );
CNSTI4 2
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 815
;815:	}
LABELV $590
line 797
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $589
line 818
;816:
;817:	// set player's team
;818:	if( dedicated == 0 && s_serveroptions.gametype >= GT_TEAM ) {
ADDRLP4 84
INDIRI4
CNSTI4 0
NEI4 $612
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $612
line 819
;819:		trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait 5; team %s\n", playerTeam_list[s_serveroptions.playerTeam[0].curvalue] ) );
ADDRGP4 $615
ARGP4
ADDRGP4 s_serveroptions+4272+64
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 playerTeam_list
ADDP4
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 156
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 820
;820:	}
LABELV $612
line 821
;821:}
LABELV $522
endproc ServerOptions_Start 160 24
proc ServerOptions_InitPlayerItems 12 12
line 829
;822:
;823:
;824:/*
;825:=================
;826:ServerOptions_InitPlayerItems
;827:=================
;828:*/
;829:static void ServerOptions_InitPlayerItems( void ) {
line 834
;830:	int		n;
;831:	int		v;
;832:
;833:	// init types
;834:	if( s_serveroptions.multiplayer ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $619
line 835
;835:		v = 0;	// open
ADDRLP4 4
CNSTI4 0
ASGNI4
line 836
;836:	}
ADDRGP4 $620
JUMPV
LABELV $619
line 837
;837:	else {
line 838
;838:		v = 1;	// bot
ADDRLP4 4
CNSTI4 1
ASGNI4
line 839
;839:	}
LABELV $620
line 841
;840:	
;841:	for( n = 0; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $622
line 842
;842:		s_serveroptions.playerType[n].curvalue = v;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 843
;843:	}
LABELV $623
line 841
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $622
line 845
;844:
;845:	if( s_serveroptions.multiplayer && (s_serveroptions.gametype < GT_TEAM) ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $628
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
GEI4 $628
line 846
;846:		for( n = 8; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 8
ASGNI4
LABELV $632
line 847
;847:			s_serveroptions.playerType[n].curvalue = 2;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
CNSTI4 2
ASGNI4
line 848
;848:		}
LABELV $633
line 846
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $632
line 849
;849:	}
LABELV $628
line 852
;850:
;851:	// if not a dedicated server, first slot is reserved for the human on the server
;852:	if( s_serveroptions.dedicated.curvalue == 0 ) {
ADDRGP4 s_serveroptions+536+64
INDIRI4
CNSTI4 0
NEI4 $638
line 854
;853:		// human
;854:		s_serveroptions.playerType[0].generic.flags |= QMF_INACTIVE;
ADDRLP4 8
ADDRGP4 s_serveroptions+2256+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 855
;855:		s_serveroptions.playerType[0].curvalue = 0;
ADDRGP4 s_serveroptions+2256+64
CNSTI4 0
ASGNI4
line 856
;856:		trap_Cvar_VariableStringBuffer( "name", s_serveroptions.playerNameBuffers[0], sizeof(s_serveroptions.playerNameBuffers[0]) );
ADDRGP4 $646
ARGP4
ADDRGP4 s_serveroptions+5728
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 857
;857:		Q_CleanStr( s_serveroptions.playerNameBuffers[0] );
ADDRGP4 s_serveroptions+5728
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 858
;858:	}
LABELV $638
line 861
;859:
;860:	// init teams
;861:	if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $650
line 862
;862:		for( n = 0; n < (PLAYER_SLOTS / 2); n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $653
line 863
;863:			s_serveroptions.playerTeam[n].curvalue = 0;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+64
ADDP4
CNSTI4 0
ASGNI4
line 864
;864:		}
LABELV $654
line 862
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 6
LTI4 $653
line 865
;865:		for( ; n < PLAYER_SLOTS; n++ ) {
ADDRGP4 $662
JUMPV
LABELV $659
line 866
;866:			s_serveroptions.playerTeam[n].curvalue = 1;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+64
ADDP4
CNSTI4 1
ASGNI4
line 867
;867:		}
LABELV $660
line 865
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $662
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $659
line 868
;868:	}
ADDRGP4 $651
JUMPV
LABELV $650
line 869
;869:	else {
line 870
;870:		for( n = 0; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $665
line 871
;871:			s_serveroptions.playerTeam[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 872
;872:		}
LABELV $666
line 870
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $665
line 873
;873:	}
LABELV $651
line 874
;874:}
LABELV $618
endproc ServerOptions_InitPlayerItems 12 12
proc ServerOptions_SetPlayerItems 12 0
line 882
;875:
;876:
;877:/*
;878:=================
;879:ServerOptions_SetPlayerItems
;880:=================
;881:*/
;882:static void ServerOptions_SetPlayerItems( void ) {
line 894
;883:	int		start;
;884:	int		n;
;885:
;886:	// types
;887://	for( n = 0; n < PLAYER_SLOTS; n++ ) {
;888://		if( (!s_serveroptions.multiplayer) && (n > 0) && (s_serveroptions.playerType[n].curvalue == 0) ) {
;889://			s_serveroptions.playerType[n].curvalue = 1;
;890://		}
;891://	}
;892:
;893:	// names
;894:	if( s_serveroptions.dedicated.curvalue == 0 ) {
ADDRGP4 s_serveroptions+536+64
INDIRI4
CNSTI4 0
NEI4 $672
line 895
;895:		s_serveroptions.player0.string = "Human";
ADDRGP4 s_serveroptions+2184+60
ADDRGP4 $678
ASGNP4
line 896
;896:		s_serveroptions.playerName[0].generic.flags &= ~QMF_HIDDEN;
ADDRLP4 8
ADDRGP4 s_serveroptions+3408+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294963199
BANDU4
ASGNU4
line 898
;897:
;898:		start = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 899
;899:	}
ADDRGP4 $673
JUMPV
LABELV $672
line 900
;900:	else {
line 901
;901:		s_serveroptions.player0.string = "Open";
ADDRGP4 s_serveroptions+2184+60
ADDRGP4 $492
ASGNP4
line 902
;902:		start = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 903
;903:	}
LABELV $673
line 904
;904:	for( n = start; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $686
JUMPV
LABELV $683
line 905
;905:		if( s_serveroptions.playerType[n].curvalue == 1 ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 1
NEI4 $687
line 906
;906:			s_serveroptions.playerName[n].generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 907
;907:		}
ADDRGP4 $688
JUMPV
LABELV $687
line 908
;908:		else {
line 909
;909:			s_serveroptions.playerName[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 910
;910:		}
LABELV $688
line 911
;911:	}
LABELV $684
line 904
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $686
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $683
line 914
;912:
;913:	// teams
;914:	if( s_serveroptions.gametype < GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
GEI4 $695
line 915
;915:		return;
ADDRGP4 $671
JUMPV
LABELV $695
line 917
;916:	}
;917:	for( n = start; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $701
JUMPV
LABELV $698
line 918
;918:		if( s_serveroptions.playerType[n].curvalue == 2 ) {
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 2
NEI4 $702
line 919
;919:			s_serveroptions.playerTeam[n].generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 920
;920:		}
ADDRGP4 $703
JUMPV
LABELV $702
line 921
;921:		else {
line 922
;922:			s_serveroptions.playerTeam[n].generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 923
;923:		}
LABELV $703
line 924
;924:	}
LABELV $699
line 917
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $701
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $698
line 925
;925:}
LABELV $671
endproc ServerOptions_SetPlayerItems 12 0
proc ServerOptions_Event 8 0
line 933
;926:
;927:
;928:/*
;929:=================
;930:ServerOptions_Event
;931:=================
;932:*/
;933:static void ServerOptions_Event( void* ptr, int event ) {
line 934
;934:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $711
ADDRLP4 0
INDIRI4
CNSTI4 24
GTI4 $711
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $727-72
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $727
address $721
address $711
address $714
address $717
address $717
address $718
address $724
code
LABELV $714
line 940
;935:	
;936:	//if( event != QM_ACTIVATED && event != QM_LOSTFOCUS) {
;937:	//	return;
;938:	//}
;939:	case ID_PLAYER_TYPE:
;940:		if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $715
line 941
;941:			break;
ADDRGP4 $712
JUMPV
LABELV $715
line 943
;942:		}
;943:		ServerOptions_SetPlayerItems();
ADDRGP4 ServerOptions_SetPlayerItems
CALLV
pop
line 944
;944:		break;
ADDRGP4 $712
JUMPV
LABELV $717
line 948
;945:
;946:	case ID_MAXCLIENTS:
;947:	case ID_DEDICATED:
;948:		ServerOptions_SetPlayerItems();
ADDRGP4 ServerOptions_SetPlayerItems
CALLV
pop
line 949
;949:		break;
ADDRGP4 $712
JUMPV
LABELV $718
line 951
;950:	case ID_GO:
;951:		if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $719
line 952
;952:			break;
ADDRGP4 $712
JUMPV
LABELV $719
line 954
;953:		}
;954:		ServerOptions_Start();
ADDRGP4 ServerOptions_Start
CALLV
pop
line 955
;955:		break;
ADDRGP4 $712
JUMPV
LABELV $721
line 958
;956:
;957:	case ID_STARTSERVERNEXT:
;958:		if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $712
line 959
;959:			break;
ADDRGP4 $712
JUMPV
line 961
;960:		}
;961:		break;
LABELV $724
line 963
;962:	case ID_BACK:
;963:		if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $725
line 964
;964:			break;
ADDRGP4 $712
JUMPV
LABELV $725
line 966
;965:		}
;966:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 967
;967:		break;
LABELV $711
LABELV $712
line 969
;968:	}
;969:}
LABELV $710
endproc ServerOptions_Event 8 0
proc ServerOptions_PlayerNameEvent 4 4
line 972
;970:
;971:
;972:static void ServerOptions_PlayerNameEvent( void* ptr, int event ) {
line 975
;973:	int		n;
;974:
;975:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $730
line 976
;976:		return;
ADDRGP4 $729
JUMPV
LABELV $730
line 978
;977:	}
;978:	n = ((menutext_s*)ptr)->generic.id;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 979
;979:	s_serveroptions.newBotIndex = n;
ADDRGP4 s_serveroptions+5924
ADDRLP4 0
INDIRI4
ASGNI4
line 980
;980:	UI_BotSelectMenu( s_serveroptions.playerNameBuffers[n] );
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRGP4 UI_BotSelectMenu
CALLV
pop
line 981
;981:}
LABELV $729
endproc ServerOptions_PlayerNameEvent 4 4
proc ServerOptions_StatusBar 4 20
line 989
;982:
;983:
;984:/*
;985:=================
;986:ServerOptions_StatusBar
;987:=================
;988:*/
;989:static void ServerOptions_StatusBar( void* ptr ) {
line 990
;990:	switch( ((menucommon_s*)ptr)->id ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $735
JUMPV
LABELV $735
line 992
;991:	default:
;992:		UI_DrawString( 320, 440, "0 = NO LIMIT", UI_CENTER|UI_SMALLFONT, colorWhite );
CNSTI4 320
ARGI4
CNSTI4 440
ARGI4
ADDRGP4 $738
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 993
;993:		break;
LABELV $736
line 995
;994:	}
;995:}
LABELV $734
endproc ServerOptions_StatusBar 4 20
proc ServerOptions_LevelshotDraw 20 20
line 1003
;996:
;997:
;998:/*
;999:===============
;1000:ServerOptions_LevelshotDraw
;1001:===============
;1002:*/
;1003:static void ServerOptions_LevelshotDraw( void *self ) {
line 1009
;1004:	menubitmap_s	*b;
;1005:	int				x;
;1006:	int				y;
;1007:
;1008:	// strange place for this, but it works
;1009:	if( s_serveroptions.newBot ) {
ADDRGP4 s_serveroptions+5920
INDIRI4
CNSTI4 0
EQI4 $740
line 1010
;1010:		Q_strncpyz( s_serveroptions.playerNameBuffers[s_serveroptions.newBotIndex], s_serveroptions.newBotName, 16 );
ADDRGP4 s_serveroptions+5924
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRGP4 s_serveroptions+5928
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1011
;1011:		s_serveroptions.newBot = qfalse;
ADDRGP4 s_serveroptions+5920
CNSTI4 0
ASGNI4
line 1012
;1012:	}
LABELV $740
line 1014
;1013:
;1014:	b = (menubitmap_s *)self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 1016
;1015:
;1016:	Bitmap_Draw( b );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Bitmap_Draw
CALLV
pop
line 1018
;1017:
;1018:	x = b->generic.x;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1019
;1019:	y = b->generic.y + b->height;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1020
;1020:	UI_FillRect( x, y, b->width, 40, colorBlack );
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
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
CNSTF4 1109393408
ARGF4
ADDRGP4 colorBlack
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 1022
;1021:
;1022:	x += b->width / 2;
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
INDIRI4
CNSTI4 2
DIVI4
ADDI4
ASGNI4
line 1023
;1023:	y += 4;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 4
ADDI4
ASGNI4
line 1024
;1024:	UI_DrawString( x, y, s_serveroptions.mapnamebuffer, UI_CENTER|UI_SMALLFONT, color_orange );
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 s_serveroptions+5696
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 1026
;1025:
;1026:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1027
;1027:	UI_DrawString( x, y, gametype_items[gametype_remap2[s_serveroptions.gametype]], UI_CENTER|UI_SMALLFONT, color_orange );
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 16
CNSTI4 2
ASGNI4
ADDRGP4 s_serveroptions+5692
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRGP4 gametype_remap2
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRGP4 gametype_items
ADDP4
INDIRP4
ARGP4
CNSTI4 17
ARGI4
ADDRGP4 color_orange
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 1028
;1028:}
LABELV $739
endproc ServerOptions_LevelshotDraw 20 20
proc ServerOptions_InitBotNames 1072 12
line 1031
;1029:
;1030:
;1031:static void ServerOptions_InitBotNames( void ) {
line 1040
;1032:	int			count;
;1033:	int			n;
;1034:	const char	*arenaInfo;
;1035:	const char	*botInfo;
;1036:	char		*p;
;1037:	char		*bot;
;1038:	char		bots[MAX_INFO_STRING];
;1039:
;1040:	if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $750
line 1041
;1041:		Q_strncpyz( s_serveroptions.playerNameBuffers[1], "grunt", 16 );
ADDRGP4 s_serveroptions+5728+16
ARGP4
ADDRGP4 $755
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1042
;1042:		Q_strncpyz( s_serveroptions.playerNameBuffers[2], "major", 16 );
ADDRGP4 s_serveroptions+5728+32
ARGP4
ADDRGP4 $758
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1043
;1043:		if( s_serveroptions.gametype == GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
NEI4 $759
line 1044
;1044:			Q_strncpyz( s_serveroptions.playerNameBuffers[3], "visor", 16 );
ADDRGP4 s_serveroptions+5728+48
ARGP4
ADDRGP4 $764
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1045
;1045:		}
ADDRGP4 $760
JUMPV
LABELV $759
line 1046
;1046:		else {
line 1047
;1047:			s_serveroptions.playerType[3].curvalue = 2;
ADDRGP4 s_serveroptions+2256+288+64
CNSTI4 2
ASGNI4
line 1048
;1048:		}
LABELV $760
line 1049
;1049:		s_serveroptions.playerType[4].curvalue = 2;
ADDRGP4 s_serveroptions+2256+384+64
CNSTI4 2
ASGNI4
line 1050
;1050:		s_serveroptions.playerType[5].curvalue = 2;
ADDRGP4 s_serveroptions+2256+480+64
CNSTI4 2
ASGNI4
line 1052
;1051:
;1052:		Q_strncpyz( s_serveroptions.playerNameBuffers[6], "sarge", 16 );
ADDRGP4 s_serveroptions+5728+96
ARGP4
ADDRGP4 $776
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1053
;1053:		Q_strncpyz( s_serveroptions.playerNameBuffers[7], "grunt", 16 );
ADDRGP4 s_serveroptions+5728+112
ARGP4
ADDRGP4 $755
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1054
;1054:		Q_strncpyz( s_serveroptions.playerNameBuffers[8], "major", 16 );
ADDRGP4 s_serveroptions+5728+128
ARGP4
ADDRGP4 $758
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1055
;1055:		if( s_serveroptions.gametype == GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
NEI4 $781
line 1056
;1056:			Q_strncpyz( s_serveroptions.playerNameBuffers[9], "visor", 16 );
ADDRGP4 s_serveroptions+5728+144
ARGP4
ADDRGP4 $764
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1057
;1057:		}
ADDRGP4 $782
JUMPV
LABELV $781
line 1058
;1058:		else {
line 1059
;1059:			s_serveroptions.playerType[9].curvalue = 2;
ADDRGP4 s_serveroptions+2256+864+64
CNSTI4 2
ASGNI4
line 1060
;1060:		}
LABELV $782
line 1061
;1061:		s_serveroptions.playerType[10].curvalue = 2;
ADDRGP4 s_serveroptions+2256+960+64
CNSTI4 2
ASGNI4
line 1062
;1062:		s_serveroptions.playerType[11].curvalue = 2;
ADDRGP4 s_serveroptions+2256+1056+64
CNSTI4 2
ASGNI4
line 1064
;1063:
;1064:		return;
ADDRGP4 $749
JUMPV
LABELV $750
line 1067
;1065:	}
;1066:
;1067:	count = 1;	// skip the first slot, reserved for a human
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1070
;1068:
;1069:	// get info for this map
;1070:	arenaInfo = UI_GetArenaInfoByMap( s_serveroptions.mapnamebuffer );
ADDRGP4 s_serveroptions+5696
ARGP4
ADDRLP4 1048
ADDRGP4 UI_GetArenaInfoByMap
CALLP4
ASGNP4
ADDRLP4 1044
ADDRLP4 1048
INDIRP4
ASGNP4
line 1073
;1071:
;1072:	// get the bot info - we'll seed with them if any are listed
;1073:	Q_strncpyz( bots, Info_ValueForKey( arenaInfo, "bots" ), sizeof(bots) );
ADDRLP4 1044
INDIRP4
ARGP4
ADDRGP4 $796
ARGP4
ADDRLP4 1052
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 20
ARGP4
ADDRLP4 1052
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1074
;1074:	p = &bots[0];
ADDRLP4 0
ADDRLP4 20
ASGNP4
ADDRGP4 $798
JUMPV
line 1075
;1075:	while( *p && count < PLAYER_SLOTS ) {
LABELV $800
line 1077
;1076:		//skip spaces
;1077:		while( *p && *p == ' ' ) {
line 1078
;1078:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1079
;1079:		}
LABELV $801
line 1077
ADDRLP4 1056
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $803
ADDRLP4 1056
INDIRI4
CNSTI4 32
EQI4 $800
LABELV $803
line 1080
;1080:		if( !p ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $804
line 1081
;1081:			break;
ADDRGP4 $799
JUMPV
LABELV $804
line 1085
;1082:		}
;1083:
;1084:		// mark start of bot name
;1085:		bot = p;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $807
JUMPV
LABELV $806
line 1088
;1086:
;1087:		// skip until space of null
;1088:		while( *p && *p != ' ' ) {
line 1089
;1089:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1090
;1090:		}
LABELV $807
line 1088
ADDRLP4 1060
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
EQI4 $809
ADDRLP4 1060
INDIRI4
CNSTI4 32
NEI4 $806
LABELV $809
line 1091
;1091:		if( *p ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $810
line 1092
;1092:			*p++ = 0;
ADDRLP4 1064
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1064
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1064
INDIRP4
CNSTI1 0
ASGNI1
line 1093
;1093:		}
LABELV $810
line 1095
;1094:
;1095:		botInfo = UI_GetBotInfoByName( bot );
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 UI_GetBotInfoByName
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 1064
INDIRP4
ASGNP4
line 1096
;1096:		bot = Info_ValueForKey( botInfo, "name" );
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 $646
ARGP4
ADDRLP4 1068
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 1068
INDIRP4
ASGNP4
line 1098
;1097:
;1098:		Q_strncpyz( s_serveroptions.playerNameBuffers[count], bot, sizeof(s_serveroptions.playerNameBuffers[count]) );
ADDRLP4 4
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1099
;1099:		count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1100
;1100:	}
LABELV $798
line 1075
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $814
ADDRLP4 4
INDIRI4
CNSTI4 12
LTI4 $801
LABELV $814
LABELV $799
line 1103
;1101:
;1102:	// set the rest of the bot slots to "---"
;1103:	for( n = count; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
ADDRGP4 $818
JUMPV
LABELV $815
line 1104
;1104:		strcpy( s_serveroptions.playerNameBuffers[n], "--------" );
ADDRLP4 12
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ARGP4
ADDRGP4 $820
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1105
;1105:	}
LABELV $816
line 1103
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $818
ADDRLP4 12
INDIRI4
CNSTI4 12
LTI4 $815
line 1108
;1106:
;1107:	// pad up to #8 as open slots
;1108:	for( ;count < 8; count++ ) {
ADDRGP4 $824
JUMPV
LABELV $821
line 1109
;1109:		s_serveroptions.playerType[count].curvalue = 0;
CNSTI4 96
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
CNSTI4 0
ASGNI4
line 1110
;1110:	}
LABELV $822
line 1108
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $824
ADDRLP4 4
INDIRI4
CNSTI4 8
LTI4 $821
line 1113
;1111:
;1112:	// close off the rest by default
;1113:	for( ;count < PLAYER_SLOTS; count++ ) {
ADDRGP4 $830
JUMPV
LABELV $827
line 1114
;1114:		if( s_serveroptions.playerType[count].curvalue == 1 ) {
CNSTI4 96
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
INDIRI4
CNSTI4 1
NEI4 $831
line 1115
;1115:			s_serveroptions.playerType[count].curvalue = 2;
CNSTI4 96
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+64
ADDP4
CNSTI4 2
ASGNI4
line 1116
;1116:		}
LABELV $831
line 1117
;1117:	}
LABELV $828
line 1113
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $830
ADDRLP4 4
INDIRI4
CNSTI4 12
LTI4 $827
line 1118
;1118:}
LABELV $749
endproc ServerOptions_InitBotNames 1072 12
bss
align 1
LABELV $838
skip 64
code
proc ServerOptions_SetMenuItems 84 16
line 1126
;1119:
;1120:
;1121:/*
;1122:=================
;1123:ServerOptions_SetMenuItems
;1124:=================
;1125:*/
;1126:static void ServerOptions_SetMenuItems( void ) {
line 1129
;1127:	static char picname[64];
;1128:
;1129:	switch( s_serveroptions.gametype ) {
ADDRLP4 0
ADDRGP4 s_serveroptions+5692
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $839
ADDRLP4 0
INDIRI4
CNSTI4 4
GTI4 $839
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $878
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $878
address $842
address $850
address $839
address $857
address $867
code
LABELV $842
LABELV $839
line 1132
;1130:	case GT_FFA:
;1131:	default:
;1132:		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ffa_fraglimit" ) ) );
ADDRGP4 $557
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+964+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 8
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1133
;1133:		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ffa_timelimit" ) ) );
ADDRGP4 $558
ARGP4
ADDRLP4 12
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+632+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 16
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1134
;1134:		break;
ADDRGP4 $840
JUMPV
LABELV $850
line 1137
;1135:
;1136:	case GT_TOURNAMENT:
;1137:		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_tourney_fraglimit" ) ) );
ADDRGP4 $560
ARGP4
ADDRLP4 20
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 24
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+964+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 24
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1138
;1138:		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_tourney_timelimit" ) ) );
ADDRGP4 $561
ARGP4
ADDRLP4 28
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 28
INDIRF4
ARGF4
ADDRLP4 32
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+632+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 32
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1139
;1139:		break;
ADDRGP4 $840
JUMPV
LABELV $857
line 1142
;1140:
;1141:	case GT_TEAM:
;1142:		Com_sprintf( s_serveroptions.fraglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_team_fraglimit" ) ) );
ADDRGP4 $563
ARGP4
ADDRLP4 36
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+964+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 40
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1143
;1143:		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_team_timelimit" ) ) );
ADDRGP4 $564
ARGP4
ADDRLP4 44
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+632+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 48
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1144
;1144:		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_team_friendly" ) );
ADDRGP4 $866
ARGP4
ADDRLP4 52
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 52
INDIRF4
ARGF4
ADDRLP4 56
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+1628+60
ADDRLP4 56
INDIRF4
CVFI4 4
ASGNI4
line 1145
;1145:		break;
ADDRGP4 $840
JUMPV
LABELV $867
line 1148
;1146:
;1147:	case GT_CTF:
;1148:		Com_sprintf( s_serveroptions.flaglimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "ui_ctf_capturelimit" ) ) );
ADDRGP4 $871
ARGP4
ADDRLP4 60
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1120403456
ARGF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 64
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+1296+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 64
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1149
;1149:		Com_sprintf( s_serveroptions.timelimit.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "ui_ctf_timelimit" ) ) );
ADDRGP4 $568
ARGP4
ADDRLP4 68
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1148829696
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+632+60+12
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $846
ARGP4
ADDRLP4 72
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1150
;1150:		s_serveroptions.friendlyfire.curvalue = (int)Com_Clamp( 0, 1, trap_Cvar_VariableValue( "ui_ctf_friendly" ) );
ADDRGP4 $877
ARGP4
ADDRLP4 76
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 76
INDIRF4
ARGF4
ADDRLP4 80
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+1628+60
ADDRLP4 80
INDIRF4
CVFI4 4
ASGNI4
line 1151
;1151:		break;
LABELV $840
line 1154
;1152:	}
;1153:
;1154:	Q_strncpyz( s_serveroptions.hostname.field.buffer, UI_Cvar_VariableString( "sv_hostname" ), sizeof( s_serveroptions.hostname.field.buffer ) );
ADDRGP4 $578
ARGP4
ADDRLP4 4
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 s_serveroptions+1692+60+12
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1155
;1155:	s_serveroptions.pure.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "sv_pure" ) );
ADDRGP4 $577
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+2024+60
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 1158
;1156:
;1157:	// set the map pic
;1158:	Com_sprintf( picname, 64, "levelshots/%s", s_startserver.maplist[s_startserver.currentmap] );
ADDRGP4 $838
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $103
ARGP4
ADDRGP4 s_startserver+1940
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_startserver+1956
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1159
;1159:	s_serveroptions.mappic.generic.name = picname;
ADDRGP4 s_serveroptions+360+4
ADDRGP4 $838
ASGNP4
line 1162
;1160:
;1161:	// set the map name
;1162:	strcpy( s_serveroptions.mapnamebuffer, s_startserver.mapname.string );
ADDRGP4 s_serveroptions+5696
ARGP4
ADDRGP4 s_startserver+1776+60
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1163
;1163:	Q_strupr( s_serveroptions.mapnamebuffer );
ADDRGP4 s_serveroptions+5696
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 1166
;1164:
;1165:	// get the player selections initialized
;1166:	ServerOptions_InitPlayerItems();
ADDRGP4 ServerOptions_InitPlayerItems
CALLV
pop
line 1167
;1167:	ServerOptions_SetPlayerItems();
ADDRGP4 ServerOptions_SetPlayerItems
CALLV
pop
line 1170
;1168:
;1169:	// seed bot names
;1170:	ServerOptions_InitBotNames();
ADDRGP4 ServerOptions_InitBotNames
CALLV
pop
line 1171
;1171:	ServerOptions_SetPlayerItems();
ADDRGP4 ServerOptions_SetPlayerItems
CALLV
pop
line 1172
;1172:}
LABELV $837
endproc ServerOptions_SetMenuItems 84 16
proc PlayerName_Draw 48 20
line 1179
;1173:
;1174:/*
;1175:=================
;1176:PlayerName_Draw
;1177:=================
;1178:*/
;1179:static void PlayerName_Draw( void *item ) {
line 1186
;1180:	menutext_s	*s;
;1181:	float		*color;
;1182:	int			x, y;
;1183:	int			style;
;1184:	qboolean	focus;
;1185:
;1186:	s = (menutext_s *)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 1188
;1187:
;1188:	x = s->generic.x;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1189
;1189:	y =	s->generic.y;
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 1191
;1190:
;1191:	style = UI_SMALLFONT;
ADDRLP4 16
CNSTI4 16
ASGNI4
line 1192
;1192:	focus = (s->generic.parent->cursor == s->generic.menuPosition);
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
NEI4 $897
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRGP4 $898
JUMPV
LABELV $897
ADDRLP4 24
CNSTI4 0
ASGNI4
LABELV $898
ADDRLP4 20
ADDRLP4 24
INDIRI4
ASGNI4
line 1194
;1193:
;1194:	if ( s->generic.flags & QMF_GRAYED )
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 8192
BANDU4
CNSTU4 0
EQU4 $899
line 1195
;1195:		color = text_color_disabled;
ADDRLP4 4
ADDRGP4 text_color_disabled
ASGNP4
ADDRGP4 $900
JUMPV
LABELV $899
line 1196
;1196:	else if ( focus )
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $901
line 1197
;1197:	{
line 1198
;1198:		color = text_color_highlight;
ADDRLP4 4
ADDRGP4 text_color_highlight
ASGNP4
line 1199
;1199:		style |= UI_PULSE;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 1200
;1200:	}
ADDRGP4 $902
JUMPV
LABELV $901
line 1201
;1201:	else if ( s->generic.flags & QMF_BLINK )
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRU4
CNSTU4 1
BANDU4
CNSTU4 0
EQU4 $903
line 1202
;1202:	{
line 1203
;1203:		color = text_color_highlight;
ADDRLP4 4
ADDRGP4 text_color_highlight
ASGNP4
line 1204
;1204:		style |= UI_BLINK;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 1205
;1205:	}
ADDRGP4 $904
JUMPV
LABELV $903
line 1207
;1206:	else
;1207:		color = text_color_normal;
ADDRLP4 4
ADDRGP4 text_color_normal
ASGNP4
LABELV $904
LABELV $902
LABELV $900
line 1209
;1208:
;1209:	if ( focus )
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $905
line 1210
;1210:	{
line 1212
;1211:		// draw cursor
;1212:		UI_FillRect( s->generic.left, s->generic.top, s->generic.right-s->generic.left+1, s->generic.bottom-s->generic.top+1, listbar_color ); 
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
line 1213
;1213:		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color);
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
CNSTI4 13
ARGI4
CNSTI4 4113
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 UI_DrawChar
CALLV
pop
line 1214
;1214:	}
LABELV $905
line 1216
;1215:
;1216:	UI_DrawString( x - SMALLCHAR_WIDTH, y, s->generic.name, style|UI_RIGHT, color );
ADDRLP4 8
INDIRI4
CNSTI4 8
SUBI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRI4
CNSTI4 2
BORI4
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 1217
;1217:	UI_DrawString( x + SMALLCHAR_WIDTH, y, s->string, style|UI_LEFT, color );
ADDRLP4 8
INDIRI4
CNSTI4 8
ADDI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 UI_DrawString
CALLV
pop
line 1218
;1218:}
LABELV $895
endproc PlayerName_Draw 48 20
proc ServerOptions_MenuInit 52 12
line 1228
;1219:
;1220:
;1221:/*
;1222:=================
;1223:ServerOptions_MenuInit
;1224:=================
;1225:*/
;1226:#define OPTIONS_X	456
;1227:
;1228:static void ServerOptions_MenuInit( qboolean multiplayer ) {
line 1232
;1229:	int		y;
;1230:	int		n;
;1231:
;1232:	memset( &s_serveroptions, 0 ,sizeof(serveroptions_t) );
ADDRGP4 s_serveroptions
ARGP4
CNSTI4 0
ARGI4
CNSTI4 6040
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1233
;1233:	s_serveroptions.multiplayer = multiplayer;
ADDRGP4 s_serveroptions+5688
ADDRFP4 0
INDIRI4
ASGNI4
line 1234
;1234:	s_serveroptions.gametype = (int)Com_Clamp( 0, 5, trap_Cvar_VariableValue( "g_gameType" ) );
ADDRGP4 $204
ARGP4
ADDRLP4 8
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1084227584
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+5692
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 1235
;1235:	s_serveroptions.punkbuster.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "sv_punkbuster" ) );
ADDRGP4 $582
ARGP4
ADDRLP4 16
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 20
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 s_serveroptions+5944+64
ADDRLP4 20
INDIRF4
CVFI4 4
ASGNI4
line 1237
;1236:
;1237:	ServerOptions_Cache();
ADDRGP4 ServerOptions_Cache
CALLV
pop
line 1239
;1238:
;1239:	s_serveroptions.menu.wrapAround = qtrue;
ADDRGP4 s_serveroptions+276
CNSTI4 1
ASGNI4
line 1240
;1240:	s_serveroptions.menu.fullscreen = qtrue;
ADDRGP4 s_serveroptions+280
CNSTI4 1
ASGNI4
line 1242
;1241:
;1242:	s_serveroptions.banner.generic.type			= MTYPE_BTEXT;
ADDRGP4 s_serveroptions+288
CNSTI4 10
ASGNI4
line 1243
;1243:	s_serveroptions.banner.generic.x			= 320;
ADDRGP4 s_serveroptions+288+12
CNSTI4 320
ASGNI4
line 1244
;1244:	s_serveroptions.banner.generic.y			= 16;
ADDRGP4 s_serveroptions+288+16
CNSTI4 16
ASGNI4
line 1245
;1245:	s_serveroptions.banner.string  				= "GAME SERVER";
ADDRGP4 s_serveroptions+288+60
ADDRGP4 $237
ASGNP4
line 1246
;1246:	s_serveroptions.banner.color  				= color_white;
ADDRGP4 s_serveroptions+288+68
ADDRGP4 color_white
ASGNP4
line 1247
;1247:	s_serveroptions.banner.style  				= UI_CENTER;
ADDRGP4 s_serveroptions+288+64
CNSTI4 1
ASGNI4
line 1249
;1248:
;1249:	s_serveroptions.mappic.generic.type			= MTYPE_BITMAP;
ADDRGP4 s_serveroptions+360
CNSTI4 6
ASGNI4
line 1250
;1250:	s_serveroptions.mappic.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 s_serveroptions+360+44
CNSTU4 16388
ASGNU4
line 1251
;1251:	s_serveroptions.mappic.generic.x			= 352;
ADDRGP4 s_serveroptions+360+12
CNSTI4 352
ASGNI4
line 1252
;1252:	s_serveroptions.mappic.generic.y			= 80;
ADDRGP4 s_serveroptions+360+16
CNSTI4 80
ASGNI4
line 1253
;1253:	s_serveroptions.mappic.width				= 160;
ADDRGP4 s_serveroptions+360+76
CNSTI4 160
ASGNI4
line 1254
;1254:	s_serveroptions.mappic.height				= 120;
ADDRGP4 s_serveroptions+360+80
CNSTI4 120
ASGNI4
line 1255
;1255:	s_serveroptions.mappic.errorpic				= GAMESERVER_UNKNOWNMAP;
ADDRGP4 s_serveroptions+360+64
ADDRGP4 $308
ASGNP4
line 1256
;1256:	s_serveroptions.mappic.generic.ownerdraw	= ServerOptions_LevelshotDraw;
ADDRGP4 s_serveroptions+360+56
ADDRGP4 ServerOptions_LevelshotDraw
ASGNP4
line 1258
;1257:
;1258:	s_serveroptions.picframe.generic.type		= MTYPE_BITMAP;
ADDRGP4 s_serveroptions+448
CNSTI4 6
ASGNI4
line 1259
;1259:	s_serveroptions.picframe.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE|QMF_HIGHLIGHT;
ADDRGP4 s_serveroptions+448+44
CNSTU4 16452
ASGNU4
line 1260
;1260:	s_serveroptions.picframe.generic.x			= 352 - 38;
ADDRGP4 s_serveroptions+448+12
CNSTI4 314
ASGNI4
line 1261
;1261:	s_serveroptions.picframe.generic.y			= 80 - 40;
ADDRGP4 s_serveroptions+448+16
CNSTI4 40
ASGNI4
line 1262
;1262:	s_serveroptions.picframe.width  			= 320;
ADDRGP4 s_serveroptions+448+76
CNSTI4 320
ASGNI4
line 1263
;1263:	s_serveroptions.picframe.height  			= 320;
ADDRGP4 s_serveroptions+448+80
CNSTI4 320
ASGNI4
line 1264
;1264:	s_serveroptions.picframe.focuspic			= GAMESERVER_SELECT;
ADDRGP4 s_serveroptions+448+60
ADDRGP4 $336
ASGNP4
line 1266
;1265:
;1266:	y = 272;
ADDRLP4 4
CNSTI4 272
ASGNI4
line 1267
;1267:	if( s_serveroptions.gametype != GT_CTF ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 4
EQI4 $953
line 1268
;1268:		s_serveroptions.fraglimit.generic.type       = MTYPE_FIELD;
ADDRGP4 s_serveroptions+964
CNSTI4 4
ASGNI4
line 1269
;1269:		s_serveroptions.fraglimit.generic.name       = "Frag Limit:";
ADDRGP4 s_serveroptions+964+4
ADDRGP4 $959
ASGNP4
line 1270
;1270:		s_serveroptions.fraglimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+964+44
CNSTU4 290
ASGNU4
line 1271
;1271:		s_serveroptions.fraglimit.generic.x	         = OPTIONS_X;
ADDRGP4 s_serveroptions+964+12
CNSTI4 456
ASGNI4
line 1272
;1272:		s_serveroptions.fraglimit.generic.y	         = y;
ADDRGP4 s_serveroptions+964+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1273
;1273:		s_serveroptions.fraglimit.generic.statusbar  = ServerOptions_StatusBar;
ADDRGP4 s_serveroptions+964+52
ADDRGP4 ServerOptions_StatusBar
ASGNP4
line 1274
;1274:		s_serveroptions.fraglimit.field.widthInChars = 3;
ADDRGP4 s_serveroptions+964+60+8
CNSTI4 3
ASGNI4
line 1275
;1275:		s_serveroptions.fraglimit.field.maxchars     = 3;
ADDRGP4 s_serveroptions+964+60+268
CNSTI4 3
ASGNI4
line 1276
;1276:	}
ADDRGP4 $954
JUMPV
LABELV $953
line 1277
;1277:	else {
line 1278
;1278:		s_serveroptions.flaglimit.generic.type       = MTYPE_FIELD;
ADDRGP4 s_serveroptions+1296
CNSTI4 4
ASGNI4
line 1279
;1279:		s_serveroptions.flaglimit.generic.name       = "Capture Limit:";
ADDRGP4 s_serveroptions+1296+4
ADDRGP4 $977
ASGNP4
line 1280
;1280:		s_serveroptions.flaglimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+1296+44
CNSTU4 290
ASGNU4
line 1281
;1281:		s_serveroptions.flaglimit.generic.x	         = OPTIONS_X;
ADDRGP4 s_serveroptions+1296+12
CNSTI4 456
ASGNI4
line 1282
;1282:		s_serveroptions.flaglimit.generic.y	         = y;
ADDRGP4 s_serveroptions+1296+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1283
;1283:		s_serveroptions.flaglimit.generic.statusbar  = ServerOptions_StatusBar;
ADDRGP4 s_serveroptions+1296+52
ADDRGP4 ServerOptions_StatusBar
ASGNP4
line 1284
;1284:		s_serveroptions.flaglimit.field.widthInChars = 3;
ADDRGP4 s_serveroptions+1296+60+8
CNSTI4 3
ASGNI4
line 1285
;1285:		s_serveroptions.flaglimit.field.maxchars     = 3;
ADDRGP4 s_serveroptions+1296+60+268
CNSTI4 3
ASGNI4
line 1286
;1286:	}
LABELV $954
line 1288
;1287:
;1288:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1289
;1289:	s_serveroptions.timelimit.generic.type       = MTYPE_FIELD;
ADDRGP4 s_serveroptions+632
CNSTI4 4
ASGNI4
line 1290
;1290:	s_serveroptions.timelimit.generic.name       = "Time Limit:";
ADDRGP4 s_serveroptions+632+4
ADDRGP4 $995
ASGNP4
line 1291
;1291:	s_serveroptions.timelimit.generic.flags      = QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+632+44
CNSTU4 290
ASGNU4
line 1292
;1292:	s_serveroptions.timelimit.generic.x	         = OPTIONS_X;
ADDRGP4 s_serveroptions+632+12
CNSTI4 456
ASGNI4
line 1293
;1293:	s_serveroptions.timelimit.generic.y	         = y;
ADDRGP4 s_serveroptions+632+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1294
;1294:	s_serveroptions.timelimit.generic.statusbar  = ServerOptions_StatusBar;
ADDRGP4 s_serveroptions+632+52
ADDRGP4 ServerOptions_StatusBar
ASGNP4
line 1295
;1295:	s_serveroptions.timelimit.field.widthInChars = 3;
ADDRGP4 s_serveroptions+632+60+8
CNSTI4 3
ASGNI4
line 1296
;1296:	s_serveroptions.timelimit.field.maxchars     = 3;
ADDRGP4 s_serveroptions+632+60+268
CNSTI4 3
ASGNI4
line 1298
;1297:
;1298:	if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $1010
line 1299
;1299:		y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1300
;1300:		s_serveroptions.friendlyfire.generic.type     = MTYPE_RADIOBUTTON;
ADDRGP4 s_serveroptions+1628
CNSTI4 5
ASGNI4
line 1301
;1301:		s_serveroptions.friendlyfire.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+1628+44
CNSTU4 258
ASGNU4
line 1302
;1302:		s_serveroptions.friendlyfire.generic.x	      = OPTIONS_X;
ADDRGP4 s_serveroptions+1628+12
CNSTI4 456
ASGNI4
line 1303
;1303:		s_serveroptions.friendlyfire.generic.y	      = y;
ADDRGP4 s_serveroptions+1628+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1304
;1304:		s_serveroptions.friendlyfire.generic.name	  = "Friendly Fire:";
ADDRGP4 s_serveroptions+1628+4
ADDRGP4 $1022
ASGNP4
line 1305
;1305:	}
LABELV $1010
line 1307
;1306:
;1307:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1308
;1308:	s_serveroptions.pure.generic.type			= MTYPE_RADIOBUTTON;
ADDRGP4 s_serveroptions+2024
CNSTI4 5
ASGNI4
line 1309
;1309:	s_serveroptions.pure.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+2024+44
CNSTU4 258
ASGNU4
line 1310
;1310:	s_serveroptions.pure.generic.x				= OPTIONS_X;
ADDRGP4 s_serveroptions+2024+12
CNSTI4 456
ASGNI4
line 1311
;1311:	s_serveroptions.pure.generic.y				= y;
ADDRGP4 s_serveroptions+2024+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1312
;1312:	s_serveroptions.pure.generic.name			= "Pure Server:";
ADDRGP4 s_serveroptions+2024+4
ADDRGP4 $1032
ASGNP4
line 1314
;1313:
;1314:	if( s_serveroptions.multiplayer ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $1033
line 1315
;1315:		y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1316
;1316:		s_serveroptions.dedicated.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 s_serveroptions+536
CNSTI4 3
ASGNI4
line 1317
;1317:		s_serveroptions.dedicated.generic.id		= ID_DEDICATED;
ADDRGP4 s_serveroptions+536+8
CNSTI4 22
ASGNI4
line 1318
;1318:		s_serveroptions.dedicated.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+536+44
CNSTU4 258
ASGNU4
line 1319
;1319:		s_serveroptions.dedicated.generic.callback	= ServerOptions_Event;
ADDRGP4 s_serveroptions+536+48
ADDRGP4 ServerOptions_Event
ASGNP4
line 1320
;1320:		s_serveroptions.dedicated.generic.x			= OPTIONS_X;
ADDRGP4 s_serveroptions+536+12
CNSTI4 456
ASGNI4
line 1321
;1321:		s_serveroptions.dedicated.generic.y			= y;
ADDRGP4 s_serveroptions+536+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1322
;1322:		s_serveroptions.dedicated.generic.name		= "Dedicated:";
ADDRGP4 s_serveroptions+536+4
ADDRGP4 $1049
ASGNP4
line 1323
;1323:		s_serveroptions.dedicated.itemnames			= dedicated_list;
ADDRGP4 s_serveroptions+536+76
ADDRGP4 dedicated_list
ASGNP4
line 1324
;1324:	}
LABELV $1033
line 1326
;1325:
;1326:	if( s_serveroptions.multiplayer ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $1052
line 1327
;1327:		y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1328
;1328:		s_serveroptions.hostname.generic.type       = MTYPE_FIELD;
ADDRGP4 s_serveroptions+1692
CNSTI4 4
ASGNI4
line 1329
;1329:		s_serveroptions.hostname.generic.name       = "Hostname:";
ADDRGP4 s_serveroptions+1692+4
ADDRGP4 $1058
ASGNP4
line 1330
;1330:		s_serveroptions.hostname.generic.flags      = QMF_SMALLFONT;
ADDRGP4 s_serveroptions+1692+44
CNSTU4 2
ASGNU4
line 1331
;1331:		s_serveroptions.hostname.generic.x          = OPTIONS_X;
ADDRGP4 s_serveroptions+1692+12
CNSTI4 456
ASGNI4
line 1332
;1332:		s_serveroptions.hostname.generic.y	        = y;
ADDRGP4 s_serveroptions+1692+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1333
;1333:		s_serveroptions.hostname.field.widthInChars = 18;
ADDRGP4 s_serveroptions+1692+60+8
CNSTI4 18
ASGNI4
line 1334
;1334:		s_serveroptions.hostname.field.maxchars     = 64;
ADDRGP4 s_serveroptions+1692+60+268
CNSTI4 64
ASGNI4
line 1335
;1335:	}
LABELV $1052
line 1337
;1336:
;1337:	y += BIGCHAR_HEIGHT+2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 18
ADDI4
ASGNI4
line 1338
;1338:	s_serveroptions.punkbuster.generic.type			= MTYPE_SPINCONTROL;
ADDRGP4 s_serveroptions+5944
CNSTI4 3
ASGNI4
line 1339
;1339:	s_serveroptions.punkbuster.generic.name			= "Punkbuster:";
ADDRGP4 s_serveroptions+5944+4
ADDRGP4 $1074
ASGNP4
line 1340
;1340:	s_serveroptions.punkbuster.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+5944+44
CNSTU4 258
ASGNU4
line 1341
;1341:	s_serveroptions.punkbuster.generic.id			= 0;
ADDRGP4 s_serveroptions+5944+8
CNSTI4 0
ASGNI4
line 1342
;1342:	s_serveroptions.punkbuster.generic.x				= OPTIONS_X;
ADDRGP4 s_serveroptions+5944+12
CNSTI4 456
ASGNI4
line 1343
;1343:	s_serveroptions.punkbuster.generic.y				= y;
ADDRGP4 s_serveroptions+5944+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1344
;1344:	s_serveroptions.punkbuster.itemnames				= punkbuster_items;
ADDRGP4 s_serveroptions+5944+76
ADDRGP4 punkbuster_items
ASGNP4
line 1346
;1345:	
;1346:	y = 80;
ADDRLP4 4
CNSTI4 80
ASGNI4
line 1347
;1347:	s_serveroptions.botSkill.generic.type			= MTYPE_SPINCONTROL;
ADDRGP4 s_serveroptions+2088
CNSTI4 3
ASGNI4
line 1348
;1348:	s_serveroptions.botSkill.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 s_serveroptions+2088+44
CNSTU4 258
ASGNU4
line 1349
;1349:	s_serveroptions.botSkill.generic.name			= "Bot Skill:  ";
ADDRGP4 s_serveroptions+2088+4
ADDRGP4 $1090
ASGNP4
line 1350
;1350:	s_serveroptions.botSkill.generic.x				= 32 + (strlen(s_serveroptions.botSkill.generic.name) + 2 ) * SMALLCHAR_WIDTH;
ADDRGP4 s_serveroptions+2088+4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 s_serveroptions+2088+12
ADDRLP4 24
INDIRI4
CNSTI4 3
LSHI4
CNSTI4 16
ADDI4
CNSTI4 32
ADDI4
ASGNI4
line 1351
;1351:	s_serveroptions.botSkill.generic.y				= y;
ADDRGP4 s_serveroptions+2088+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1352
;1352:	s_serveroptions.botSkill.itemnames				= botSkill_list;
ADDRGP4 s_serveroptions+2088+76
ADDRGP4 botSkill_list
ASGNP4
line 1353
;1353:	s_serveroptions.botSkill.curvalue				= 1;
ADDRGP4 s_serveroptions+2088+64
CNSTI4 1
ASGNI4
line 1355
;1354:
;1355:	y += ( 2 * SMALLCHAR_HEIGHT );
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 32
ADDI4
ASGNI4
line 1356
;1356:	s_serveroptions.player0.generic.type			= MTYPE_TEXT;
ADDRGP4 s_serveroptions+2184
CNSTI4 7
ASGNI4
line 1357
;1357:	s_serveroptions.player0.generic.flags			= QMF_SMALLFONT;
ADDRGP4 s_serveroptions+2184+44
CNSTU4 2
ASGNU4
line 1358
;1358:	s_serveroptions.player0.generic.x				= 32 + SMALLCHAR_WIDTH;
ADDRGP4 s_serveroptions+2184+12
CNSTI4 40
ASGNI4
line 1359
;1359:	s_serveroptions.player0.generic.y				= y;
ADDRGP4 s_serveroptions+2184+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1360
;1360:	s_serveroptions.player0.color					= color_orange;
ADDRGP4 s_serveroptions+2184+68
ADDRGP4 color_orange
ASGNP4
line 1361
;1361:	s_serveroptions.player0.style					= UI_LEFT|UI_SMALLFONT;
ADDRGP4 s_serveroptions+2184+64
CNSTI4 16
ASGNI4
line 1363
;1362:
;1363:	for( n = 0; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1112
line 1364
;1364:		s_serveroptions.playerType[n].generic.type		= MTYPE_SPINCONTROL;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256
ADDP4
CNSTI4 3
ASGNI4
line 1365
;1365:		s_serveroptions.playerType[n].generic.flags		= QMF_SMALLFONT;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+44
ADDP4
CNSTU4 2
ASGNU4
line 1366
;1366:		s_serveroptions.playerType[n].generic.id		= ID_PLAYER_TYPE;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+8
ADDP4
CNSTI4 20
ASGNI4
line 1367
;1367:		s_serveroptions.playerType[n].generic.callback	= ServerOptions_Event;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+48
ADDP4
ADDRGP4 ServerOptions_Event
ASGNP4
line 1368
;1368:		s_serveroptions.playerType[n].generic.x			= 32;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+12
ADDP4
CNSTI4 32
ASGNI4
line 1369
;1369:		s_serveroptions.playerType[n].generic.y			= y;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1370
;1370:		s_serveroptions.playerType[n].itemnames			= playerType_list;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256+76
ADDP4
ADDRGP4 playerType_list
ASGNP4
line 1372
;1371:
;1372:		s_serveroptions.playerName[n].generic.type		= MTYPE_TEXT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408
ADDP4
CNSTI4 7
ASGNI4
line 1373
;1373:		s_serveroptions.playerName[n].generic.flags		= QMF_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+44
ADDP4
CNSTU4 2
ASGNU4
line 1374
;1374:		s_serveroptions.playerName[n].generic.x			= 96;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+12
ADDP4
CNSTI4 96
ASGNI4
line 1375
;1375:		s_serveroptions.playerName[n].generic.y			= y;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1376
;1376:		s_serveroptions.playerName[n].generic.callback	= ServerOptions_PlayerNameEvent;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+48
ADDP4
ADDRGP4 ServerOptions_PlayerNameEvent
ASGNP4
line 1377
;1377:		s_serveroptions.playerName[n].generic.id		= n;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+8
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1378
;1378:		s_serveroptions.playerName[n].generic.ownerdraw	= PlayerName_Draw;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+56
ADDP4
ADDRGP4 PlayerName_Draw
ASGNP4
line 1379
;1379:		s_serveroptions.playerName[n].color				= color_orange;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 1380
;1380:		s_serveroptions.playerName[n].style				= UI_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+64
ADDP4
CNSTI4 16
ASGNI4
line 1381
;1381:		s_serveroptions.playerName[n].string			= s_serveroptions.playerNameBuffers[n];
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408+60
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 s_serveroptions+5728
ADDP4
ASGNP4
line 1382
;1382:		s_serveroptions.playerName[n].generic.top		= s_serveroptions.playerName[n].generic.y;
ADDRLP4 36
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 36
INDIRI4
ADDRGP4 s_serveroptions+3408+24
ADDP4
ADDRLP4 36
INDIRI4
ADDRGP4 s_serveroptions+3408+16
ADDP4
INDIRI4
ASGNI4
line 1383
;1383:		s_serveroptions.playerName[n].generic.bottom	= s_serveroptions.playerName[n].generic.y + SMALLCHAR_HEIGHT;
ADDRLP4 40
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 40
INDIRI4
ADDRGP4 s_serveroptions+3408+32
ADDP4
ADDRLP4 40
INDIRI4
ADDRGP4 s_serveroptions+3408+16
ADDP4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1384
;1384:		s_serveroptions.playerName[n].generic.left		= s_serveroptions.playerName[n].generic.x - SMALLCHAR_HEIGHT/ 2;
ADDRLP4 44
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 44
INDIRI4
ADDRGP4 s_serveroptions+3408+20
ADDP4
ADDRLP4 44
INDIRI4
ADDRGP4 s_serveroptions+3408+12
ADDP4
INDIRI4
CNSTI4 8
SUBI4
ASGNI4
line 1385
;1385:		s_serveroptions.playerName[n].generic.right		= s_serveroptions.playerName[n].generic.x + 16 * SMALLCHAR_WIDTH;
ADDRLP4 48
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 48
INDIRI4
ADDRGP4 s_serveroptions+3408+28
ADDP4
ADDRLP4 48
INDIRI4
ADDRGP4 s_serveroptions+3408+12
ADDP4
INDIRI4
CNSTI4 128
ADDI4
ASGNI4
line 1387
;1386:
;1387:		s_serveroptions.playerTeam[n].generic.type		= MTYPE_SPINCONTROL;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272
ADDP4
CNSTI4 3
ASGNI4
line 1388
;1388:		s_serveroptions.playerTeam[n].generic.flags		= QMF_SMALLFONT;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+44
ADDP4
CNSTU4 2
ASGNU4
line 1389
;1389:		s_serveroptions.playerTeam[n].generic.x			= 240;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+12
ADDP4
CNSTI4 240
ASGNI4
line 1390
;1390:		s_serveroptions.playerTeam[n].generic.y			= y;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1391
;1391:		s_serveroptions.playerTeam[n].itemnames			= playerTeam_list;
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272+76
ADDP4
ADDRGP4 playerTeam_list
ASGNP4
line 1393
;1392:
;1393:		y += ( SMALLCHAR_HEIGHT + 4 );
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 20
ADDI4
ASGNI4
line 1394
;1394:	}
LABELV $1113
line 1363
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $1112
line 1396
;1395:
;1396:	s_serveroptions.back.generic.type	  = MTYPE_BITMAP;
ADDRGP4 s_serveroptions+5600
CNSTI4 6
ASGNI4
line 1397
;1397:	s_serveroptions.back.generic.name     = GAMESERVER_BACK0;
ADDRGP4 s_serveroptions+5600+4
ADDRGP4 $403
ASGNP4
line 1398
;1398:	s_serveroptions.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_serveroptions+5600+44
CNSTU4 260
ASGNU4
line 1399
;1399:	s_serveroptions.back.generic.callback = ServerOptions_Event;
ADDRGP4 s_serveroptions+5600+48
ADDRGP4 ServerOptions_Event
ASGNP4
line 1400
;1400:	s_serveroptions.back.generic.id	      = ID_BACK;
ADDRGP4 s_serveroptions+5600+8
CNSTI4 24
ASGNI4
line 1401
;1401:	s_serveroptions.back.generic.x		  = 0;
ADDRGP4 s_serveroptions+5600+12
CNSTI4 0
ASGNI4
line 1402
;1402:	s_serveroptions.back.generic.y		  = 480-64;
ADDRGP4 s_serveroptions+5600+16
CNSTI4 416
ASGNI4
line 1403
;1403:	s_serveroptions.back.width  		  = 128;
ADDRGP4 s_serveroptions+5600+76
CNSTI4 128
ASGNI4
line 1404
;1404:	s_serveroptions.back.height  		  = 64;
ADDRGP4 s_serveroptions+5600+80
CNSTI4 64
ASGNI4
line 1405
;1405:	s_serveroptions.back.focuspic         = GAMESERVER_BACK1;
ADDRGP4 s_serveroptions+5600+60
ADDRGP4 $420
ASGNP4
line 1407
;1406:
;1407:	s_serveroptions.next.generic.type	  = MTYPE_BITMAP;
ADDRGP4 s_serveroptions+5512
CNSTI4 6
ASGNI4
line 1408
;1408:	s_serveroptions.next.generic.name     = GAMESERVER_NEXT0;
ADDRGP4 s_serveroptions+5512+4
ADDRGP4 $424
ASGNP4
line 1409
;1409:	s_serveroptions.next.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_INACTIVE|QMF_GRAYED|QMF_HIDDEN;
ADDRGP4 s_serveroptions+5512+44
CNSTU4 28944
ASGNU4
line 1410
;1410:	s_serveroptions.next.generic.callback = ServerOptions_Event;
ADDRGP4 s_serveroptions+5512+48
ADDRGP4 ServerOptions_Event
ASGNP4
line 1411
;1411:	s_serveroptions.next.generic.id	      = ID_STARTSERVERNEXT;
ADDRGP4 s_serveroptions+5512+8
CNSTI4 18
ASGNI4
line 1412
;1412:	s_serveroptions.next.generic.x		  = 640;
ADDRGP4 s_serveroptions+5512+12
CNSTI4 640
ASGNI4
line 1413
;1413:	s_serveroptions.next.generic.y		  = 480-64-72;
ADDRGP4 s_serveroptions+5512+16
CNSTI4 344
ASGNI4
line 1414
;1414:	s_serveroptions.next.generic.statusbar  = ServerOptions_StatusBar;
ADDRGP4 s_serveroptions+5512+52
ADDRGP4 ServerOptions_StatusBar
ASGNP4
line 1415
;1415:	s_serveroptions.next.width  		  = 128;
ADDRGP4 s_serveroptions+5512+76
CNSTI4 128
ASGNI4
line 1416
;1416:	s_serveroptions.next.height  		  = 64;
ADDRGP4 s_serveroptions+5512+80
CNSTI4 64
ASGNI4
line 1417
;1417:	s_serveroptions.next.focuspic         = GAMESERVER_NEXT1;
ADDRGP4 s_serveroptions+5512+60
ADDRGP4 $441
ASGNP4
line 1419
;1418:
;1419:	s_serveroptions.go.generic.type	    = MTYPE_BITMAP;
ADDRGP4 s_serveroptions+5424
CNSTI4 6
ASGNI4
line 1420
;1420:	s_serveroptions.go.generic.name     = GAMESERVER_FIGHT0;
ADDRGP4 s_serveroptions+5424+4
ADDRGP4 $1217
ASGNP4
line 1421
;1421:	s_serveroptions.go.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 s_serveroptions+5424+44
CNSTU4 272
ASGNU4
line 1422
;1422:	s_serveroptions.go.generic.callback = ServerOptions_Event;
ADDRGP4 s_serveroptions+5424+48
ADDRGP4 ServerOptions_Event
ASGNP4
line 1423
;1423:	s_serveroptions.go.generic.id	    = ID_GO;
ADDRGP4 s_serveroptions+5424+8
CNSTI4 23
ASGNI4
line 1424
;1424:	s_serveroptions.go.generic.x		= 640;
ADDRGP4 s_serveroptions+5424+12
CNSTI4 640
ASGNI4
line 1425
;1425:	s_serveroptions.go.generic.y		= 480-64;
ADDRGP4 s_serveroptions+5424+16
CNSTI4 416
ASGNI4
line 1426
;1426:	s_serveroptions.go.width  		    = 128;
ADDRGP4 s_serveroptions+5424+76
CNSTI4 128
ASGNI4
line 1427
;1427:	s_serveroptions.go.height  		    = 64;
ADDRGP4 s_serveroptions+5424+80
CNSTI4 64
ASGNI4
line 1428
;1428:	s_serveroptions.go.focuspic         = GAMESERVER_FIGHT1;
ADDRGP4 s_serveroptions+5424+60
ADDRGP4 $1234
ASGNP4
line 1430
;1429:
;1430:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.banner );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1432
;1431:
;1432:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.mappic );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1433
;1433:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.picframe );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+448
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1435
;1434:
;1435:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.botSkill );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+2088
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1436
;1436:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.player0 );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+2184
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1437
;1437:	for( n = 0; n < PLAYER_SLOTS; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1240
line 1438
;1438:		if( n != 0 ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $1244
line 1439
;1439:			Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerType[n] );
ADDRGP4 s_serveroptions
ARGP4
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+2256
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1440
;1440:		}
LABELV $1244
line 1441
;1441:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerName[n] );
ADDRGP4 s_serveroptions
ARGP4
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+3408
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1442
;1442:		if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $1248
line 1443
;1443:			Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.playerTeam[n] );
ADDRGP4 s_serveroptions
ARGP4
CNSTI4 96
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 s_serveroptions+4272
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1444
;1444:		}
LABELV $1248
line 1445
;1445:	}
LABELV $1241
line 1437
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 12
LTI4 $1240
line 1447
;1446:
;1447:	if( s_serveroptions.gametype != GT_CTF ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 4
EQI4 $1252
line 1448
;1448:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.fraglimit );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+964
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1449
;1449:	}
ADDRGP4 $1253
JUMPV
LABELV $1252
line 1450
;1450:	else {
line 1451
;1451:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.flaglimit );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+1296
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1452
;1452:	}
LABELV $1253
line 1453
;1453:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.timelimit );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+632
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1454
;1454:	if( s_serveroptions.gametype >= GT_TEAM ) {
ADDRGP4 s_serveroptions+5692
INDIRI4
CNSTI4 3
LTI4 $1258
line 1455
;1455:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.friendlyfire );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+1628
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1456
;1456:	}
LABELV $1258
line 1457
;1457:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.pure );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+2024
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1458
;1458:	if( s_serveroptions.multiplayer ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $1263
line 1459
;1459:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.dedicated );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+536
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1460
;1460:	}
LABELV $1263
line 1461
;1461:	if( s_serveroptions.multiplayer ) {
ADDRGP4 s_serveroptions+5688
INDIRI4
CNSTI4 0
EQI4 $1267
line 1462
;1462:		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.hostname );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+1692
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1463
;1463:	}
LABELV $1267
line 1465
;1464:
;1465:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.back );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+5600
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1466
;1466:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.next );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+5512
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1467
;1467:	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.go );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+5424
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1469
;1468:
;1469:	Menu_AddItem( &s_serveroptions.menu, (void*) &s_serveroptions.punkbuster );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 s_serveroptions+5944
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1471
;1470:	
;1471:	ServerOptions_SetMenuItems();
ADDRGP4 ServerOptions_SetMenuItems
CALLV
pop
line 1472
;1472:}
LABELV $907
endproc ServerOptions_MenuInit 52 12
export ServerOptions_Cache
proc ServerOptions_Cache 0 4
line 1479
;1473:
;1474:/*
;1475:=================
;1476:ServerOptions_Cache
;1477:=================
;1478:*/
;1479:void ServerOptions_Cache( void ) {
line 1480
;1480:	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );
ADDRGP4 $403
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1481
;1481:	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );
ADDRGP4 $420
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1482
;1482:	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT0 );
ADDRGP4 $1217
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1483
;1483:	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT1 );
ADDRGP4 $1234
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1484
;1484:	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );
ADDRGP4 $336
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1485
;1485:	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
ADDRGP4 $308
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1486
;1486:}
LABELV $1275
endproc ServerOptions_Cache 0 4
proc UI_ServerOptionsMenu 0 4
line 1494
;1487:
;1488:
;1489:/*
;1490:=================
;1491:UI_ServerOptionsMenu
;1492:=================
;1493:*/
;1494:static void UI_ServerOptionsMenu( qboolean multiplayer ) {
line 1495
;1495:	ServerOptions_MenuInit( multiplayer );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ServerOptions_MenuInit
CALLV
pop
line 1496
;1496:	UI_PushMenu( &s_serveroptions.menu );
ADDRGP4 s_serveroptions
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 1497
;1497:}
LABELV $1276
endproc UI_ServerOptionsMenu 0 4
proc UI_BotSelectMenu_SortCompare 44 8
line 1558
;1498:
;1499:
;1500:
;1501:/*
;1502:=============================================================================
;1503:
;1504:BOT SELECT MENU *****
;1505:
;1506:=============================================================================
;1507:*/
;1508:
;1509:
;1510:#define BOTSELECT_BACK0			"menu/art/back_0"
;1511:#define BOTSELECT_BACK1			"menu/art/back_1"
;1512:#define BOTSELECT_ACCEPT0		"menu/art/accept_0"
;1513:#define BOTSELECT_ACCEPT1		"menu/art/accept_1"
;1514:#define BOTSELECT_SELECT		"menu/art/opponents_select"
;1515:#define BOTSELECT_SELECTED		"menu/art/opponents_selected"
;1516:#define BOTSELECT_ARROWS		"menu/art/gs_arrows_0"
;1517:#define BOTSELECT_ARROWSL		"menu/art/gs_arrows_l"
;1518:#define BOTSELECT_ARROWSR		"menu/art/gs_arrows_r"
;1519:
;1520:#define PLAYERGRID_COLS			4
;1521:#define PLAYERGRID_ROWS			4
;1522:#define MAX_MODELSPERPAGE		(PLAYERGRID_ROWS * PLAYERGRID_COLS)
;1523:
;1524:
;1525:typedef struct {
;1526:	menuframework_s	menu;
;1527:
;1528:	menutext_s		banner;
;1529:
;1530:	menubitmap_s	pics[MAX_MODELSPERPAGE];
;1531:	menubitmap_s	picbuttons[MAX_MODELSPERPAGE];
;1532:	menutext_s		picnames[MAX_MODELSPERPAGE];
;1533:
;1534:	menubitmap_s	arrows;
;1535:	menubitmap_s	left;
;1536:	menubitmap_s	right;
;1537:
;1538:	menubitmap_s	go;
;1539:	menubitmap_s	back;
;1540:
;1541:	int				numBots;
;1542:	int				modelpage;
;1543:	int				numpages;
;1544:	int				selectedmodel;
;1545:	int				sortedBotNums[MAX_BOTS];
;1546:	char			boticons[MAX_MODELSPERPAGE][MAX_QPATH];
;1547:	char			botnames[MAX_MODELSPERPAGE][16];
;1548:} botSelectInfo_t;
;1549:
;1550:static botSelectInfo_t	botSelectInfo;
;1551:
;1552:
;1553:/*
;1554:=================
;1555:UI_BotSelectMenu_SortCompare
;1556:=================
;1557:*/
;1558:static int QDECL UI_BotSelectMenu_SortCompare( const void *arg1, const void *arg2 ) {
line 1563
;1559:	int			num1, num2;
;1560:	const char	*info1, *info2;
;1561:	const char	*name1, *name2;
;1562:
;1563:	num1 = *(int *)arg1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1564
;1564:	num2 = *(int *)arg2;
ADDRLP4 4
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
line 1566
;1565:
;1566:	info1 = UI_GetBotInfoByNumber( num1 );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 24
INDIRP4
ASGNP4
line 1567
;1567:	info2 = UI_GetBotInfoByNumber( num2 );
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 28
INDIRP4
ASGNP4
line 1569
;1568:
;1569:	name1 = Info_ValueForKey( info1, "name" );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $646
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 32
INDIRP4
ASGNP4
line 1570
;1570:	name2 = Info_ValueForKey( info2, "name" );
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $646
ARGP4
ADDRLP4 36
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 20
ADDRLP4 36
INDIRP4
ASGNP4
line 1572
;1571:
;1572:	return Q_stricmp( name1, name2 );
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
RETI4
LABELV $1278
endproc UI_BotSelectMenu_SortCompare 44 8
proc UI_BotSelectMenu_BuildList 12 16
line 1581
;1573:}
;1574:
;1575:
;1576:/*
;1577:=================
;1578:UI_BotSelectMenu_BuildList
;1579:=================
;1580:*/
;1581:static void UI_BotSelectMenu_BuildList( void ) {
line 1584
;1582:	int		n;
;1583:
;1584:	botSelectInfo.modelpage = 0;
ADDRGP4 botSelectInfo+4772
CNSTI4 0
ASGNI4
line 1585
;1585:	botSelectInfo.numBots = UI_GetNumBots();
ADDRLP4 4
ADDRGP4 UI_GetNumBots
CALLI4
ASGNI4
ADDRGP4 botSelectInfo+4768
ADDRLP4 4
INDIRI4
ASGNI4
line 1586
;1586:	botSelectInfo.numpages = botSelectInfo.numBots / MAX_MODELSPERPAGE;
ADDRGP4 botSelectInfo+4776
ADDRGP4 botSelectInfo+4768
INDIRI4
CNSTI4 16
DIVI4
ASGNI4
line 1587
;1587:	if( botSelectInfo.numBots % MAX_MODELSPERPAGE ) {
ADDRGP4 botSelectInfo+4768
INDIRI4
CNSTI4 16
MODI4
CNSTI4 0
EQI4 $1284
line 1588
;1588:		botSelectInfo.numpages++;
ADDRLP4 8
ADDRGP4 botSelectInfo+4776
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1589
;1589:	}
LABELV $1284
line 1592
;1590:
;1591:	// initialize the array
;1592:	for( n = 0; n < botSelectInfo.numBots; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1291
JUMPV
LABELV $1288
line 1593
;1593:		botSelectInfo.sortedBotNums[n] = n;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botSelectInfo+4784
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1594
;1594:	}
LABELV $1289
line 1592
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1291
ADDRLP4 0
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
LTI4 $1288
line 1597
;1595:
;1596:	// now sort it
;1597:	qsort( botSelectInfo.sortedBotNums, botSelectInfo.numBots, sizeof(botSelectInfo.sortedBotNums[0]), UI_BotSelectMenu_SortCompare );
ADDRGP4 botSelectInfo+4784
ARGP4
ADDRGP4 botSelectInfo+4768
INDIRI4
ARGI4
CNSTI4 4
ARGI4
ADDRGP4 UI_BotSelectMenu_SortCompare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 1598
;1598:}
LABELV $1279
endproc UI_BotSelectMenu_BuildList 12 16
proc ServerPlayerIcon 80 20
line 1606
;1599:
;1600:
;1601:/*
;1602:=================
;1603:ServerPlayerIcon
;1604:=================
;1605:*/
;1606:static void ServerPlayerIcon( const char *modelAndSkin, char *iconName, int iconNameMaxSize ) {
line 1610
;1607:	char	*skin;
;1608:	char	model[MAX_QPATH];
;1609:
;1610:	Q_strncpyz( model, modelAndSkin, sizeof(model));
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1611
;1611:	skin = Q_strrchr( model, '/' );
ADDRLP4 4
ARGP4
CNSTI4 47
ARGI4
ADDRLP4 68
ADDRGP4 Q_strrchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 68
INDIRP4
ASGNP4
line 1612
;1612:	if ( skin ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1298
line 1613
;1613:		*skin++ = '\0';
ADDRLP4 72
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 72
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI1 0
ASGNI1
line 1614
;1614:	}
ADDRGP4 $1299
JUMPV
LABELV $1298
line 1615
;1615:	else {
line 1616
;1616:		skin = "default";
ADDRLP4 0
ADDRGP4 $1300
ASGNP4
line 1617
;1617:	}
LABELV $1299
line 1619
;1618:
;1619:	Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_%s.tga", model, skin );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 $1301
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1621
;1620:
;1621:	if( !trap_R_RegisterShaderNoMip( iconName ) && Q_stricmp( skin, "default" ) != 0 ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 72
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $1302
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $1300
ARGP4
ADDRLP4 76
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
EQI4 $1302
line 1622
;1622:		Com_sprintf(iconName, iconNameMaxSize, "models/players/%s/icon_default.tga", model );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 $1304
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1623
;1623:	}
LABELV $1302
line 1624
;1624:}
LABELV $1297
endproc ServerPlayerIcon 80 20
proc UI_BotSelectMenu_UpdateGrid 36 12
line 1632
;1625:
;1626:
;1627:/*
;1628:=================
;1629:UI_BotSelectMenu_UpdateGrid
;1630:=================
;1631:*/
;1632:static void UI_BotSelectMenu_UpdateGrid( void ) {
line 1637
;1633:	const char	*info;
;1634:	int			i;
;1635:    int			j;
;1636:
;1637:	j = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
ADDRLP4 4
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 4
LSHI4
ASGNI4
line 1638
;1638:	for( i = 0; i < (PLAYERGRID_ROWS * PLAYERGRID_COLS); i++, j++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1307
line 1639
;1639:		if( j < botSelectInfo.numBots ) { 
ADDRLP4 4
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
GEI4 $1311
line 1640
;1640:			info = UI_GetBotInfoByNumber( botSelectInfo.sortedBotNums[j] );
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botSelectInfo+4784
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 1641
;1641:			ServerPlayerIcon( Info_ValueForKey( info, "model" ), botSelectInfo.boticons[i], MAX_QPATH );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $1315
ARGP4
ADDRLP4 16
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 botSelectInfo+8880
ADDP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 ServerPlayerIcon
CALLV
pop
line 1642
;1642:			Q_strncpyz( botSelectInfo.botnames[i], Info_ValueForKey( info, "name" ), 16 );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $646
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1643
;1643:			Q_CleanStr( botSelectInfo.botnames[i] );
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 1644
;1644: 			botSelectInfo.pics[i].generic.name = botSelectInfo.boticons[i];
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+4
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 botSelectInfo+8880
ADDP4
ASGNP4
line 1645
;1645:			if( BotAlreadySelected( botSelectInfo.botnames[i] ) ) {
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
ARGP4
ADDRLP4 28
ADDRGP4 BotAlreadySelected
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $1322
line 1646
;1646:				botSelectInfo.picnames[i].color = color_red;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+68
ADDP4
ADDRGP4 color_red
ASGNP4
line 1647
;1647:			}
ADDRGP4 $1323
JUMPV
LABELV $1322
line 1648
;1648:			else {
line 1649
;1649:				botSelectInfo.picnames[i].color = color_orange;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 1650
;1650:			}
LABELV $1323
line 1651
;1651:			botSelectInfo.picbuttons[i].generic.flags &= ~QMF_INACTIVE;
ADDRLP4 32
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 1652
;1652:		}
ADDRGP4 $1312
JUMPV
LABELV $1311
line 1653
;1653:		else {
line 1655
;1654:			// dead slot
;1655: 			botSelectInfo.pics[i].generic.name         = NULL;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+4
ADDP4
CNSTP4 0
ASGNP4
line 1656
;1656:			botSelectInfo.picbuttons[i].generic.flags |= QMF_INACTIVE;
ADDRLP4 12
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 1657
;1657:			botSelectInfo.botnames[i][0] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
CNSTI1 0
ASGNI1
line 1658
;1658:		}
LABELV $1312
line 1660
;1659:
;1660: 		botSelectInfo.pics[i].generic.flags       &= ~QMF_HIGHLIGHT;
ADDRLP4 12
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+44
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 4294967231
BANDU4
ASGNU4
line 1661
;1661: 		botSelectInfo.pics[i].shader               = 0;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+68
ADDP4
CNSTI4 0
ASGNI4
line 1662
;1662: 		botSelectInfo.picbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
ADDRLP4 16
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
line 1663
;1663:	}
LABELV $1308
line 1638
ADDRLP4 12
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $1307
line 1666
;1664:
;1665:	// set selected model
;1666:	i = botSelectInfo.selectedmodel % MAX_MODELSPERPAGE;
ADDRLP4 0
ADDRGP4 botSelectInfo+4780
INDIRI4
CNSTI4 16
MODI4
ASGNI4
line 1667
;1667:	botSelectInfo.pics[i].generic.flags |= QMF_HIGHLIGHT;
ADDRLP4 16
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+44
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 64
BORU4
ASGNU4
line 1668
;1668:	botSelectInfo.picbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
ADDRLP4 20
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
line 1670
;1669:
;1670:	if( botSelectInfo.numpages > 1 ) {
ADDRGP4 botSelectInfo+4776
INDIRI4
CNSTI4 1
LEI4 $1347
line 1671
;1671:		if( botSelectInfo.modelpage > 0 ) {
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 0
LEI4 $1350
line 1672
;1672:			botSelectInfo.left.generic.flags &= ~QMF_INACTIVE;
ADDRLP4 24
ADDRGP4 botSelectInfo+4416+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 1673
;1673:		}
ADDRGP4 $1351
JUMPV
LABELV $1350
line 1674
;1674:		else {
line 1675
;1675:			botSelectInfo.left.generic.flags |= QMF_INACTIVE;
ADDRLP4 24
ADDRGP4 botSelectInfo+4416+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 1676
;1676:		}
LABELV $1351
line 1678
;1677:
;1678:		if( botSelectInfo.modelpage < (botSelectInfo.numpages - 1) ) {
ADDRGP4 botSelectInfo+4772
INDIRI4
ADDRGP4 botSelectInfo+4776
INDIRI4
CNSTI4 1
SUBI4
GEI4 $1357
line 1679
;1679:			botSelectInfo.right.generic.flags &= ~QMF_INACTIVE;
ADDRLP4 24
ADDRGP4 botSelectInfo+4504+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 4294950911
BANDU4
ASGNU4
line 1680
;1680:		}
ADDRGP4 $1348
JUMPV
LABELV $1357
line 1681
;1681:		else {
line 1682
;1682:			botSelectInfo.right.generic.flags |= QMF_INACTIVE;
ADDRLP4 24
ADDRGP4 botSelectInfo+4504+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 1683
;1683:		}
line 1684
;1684:	}
ADDRGP4 $1348
JUMPV
LABELV $1347
line 1685
;1685:	else {
line 1687
;1686:		// hide left/right markers
;1687:		botSelectInfo.left.generic.flags |= QMF_INACTIVE;
ADDRLP4 24
ADDRGP4 botSelectInfo+4416+44
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 1688
;1688:		botSelectInfo.right.generic.flags |= QMF_INACTIVE;
ADDRLP4 28
ADDRGP4 botSelectInfo+4504+44
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 16384
BORU4
ASGNU4
line 1689
;1689:	}
LABELV $1348
line 1690
;1690:}
LABELV $1305
endproc UI_BotSelectMenu_UpdateGrid 36 12
proc UI_BotSelectMenu_Default 28 8
line 1698
;1691:
;1692:
;1693:/*
;1694:=================
;1695:UI_BotSelectMenu_Default
;1696:=================
;1697:*/
;1698:static void UI_BotSelectMenu_Default( char *bot ) {
line 1704
;1699:	const char	*botInfo;
;1700:	const char	*test;
;1701:	int			n;
;1702:	int			i;
;1703:
;1704:	for( n = 0; n < botSelectInfo.numBots; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1373
JUMPV
LABELV $1370
line 1705
;1705:		botInfo = UI_GetBotInfoByNumber( n );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 1706
;1706:		test = Info_ValueForKey( botInfo, "name" );
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $646
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 20
INDIRP4
ASGNP4
line 1707
;1707:		if( Q_stricmp( bot, test ) == 0 ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $1375
line 1708
;1708:			break;
ADDRGP4 $1372
JUMPV
LABELV $1375
line 1710
;1709:		}
;1710:	}
LABELV $1371
line 1704
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1373
ADDRLP4 0
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
LTI4 $1370
LABELV $1372
line 1711
;1711:	if( n == botSelectInfo.numBots ) {
ADDRLP4 0
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
NEI4 $1377
line 1712
;1712:		botSelectInfo.selectedmodel = 0;
ADDRGP4 botSelectInfo+4780
CNSTI4 0
ASGNI4
line 1713
;1713:		return;
ADDRGP4 $1369
JUMPV
LABELV $1377
line 1716
;1714:	}
;1715:
;1716:	for( i = 0; i < botSelectInfo.numBots; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $1384
JUMPV
LABELV $1381
line 1717
;1717:		if( botSelectInfo.sortedBotNums[i] == n ) {
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 botSelectInfo+4784
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $1386
line 1718
;1718:			break;
ADDRGP4 $1383
JUMPV
LABELV $1386
line 1720
;1719:		}
;1720:	}
LABELV $1382
line 1716
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1384
ADDRLP4 4
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
LTI4 $1381
LABELV $1383
line 1721
;1721:	if( i == botSelectInfo.numBots ) {
ADDRLP4 4
INDIRI4
ADDRGP4 botSelectInfo+4768
INDIRI4
NEI4 $1389
line 1722
;1722:		botSelectInfo.selectedmodel = 0;
ADDRGP4 botSelectInfo+4780
CNSTI4 0
ASGNI4
line 1723
;1723:		return;
ADDRGP4 $1369
JUMPV
LABELV $1389
line 1726
;1724:	}
;1725:
;1726:	botSelectInfo.selectedmodel = i;
ADDRGP4 botSelectInfo+4780
ADDRLP4 4
INDIRI4
ASGNI4
line 1727
;1727:}
LABELV $1369
endproc UI_BotSelectMenu_Default 28 8
proc UI_BotSelectMenu_LeftEvent 4 0
line 1735
;1728:
;1729:
;1730:/*
;1731:=================
;1732:UI_BotSelectMenu_LeftEvent
;1733:=================
;1734:*/
;1735:static void UI_BotSelectMenu_LeftEvent( void* ptr, int event ) {
line 1736
;1736:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $1395
line 1737
;1737:		return;
ADDRGP4 $1394
JUMPV
LABELV $1395
line 1739
;1738:	}
;1739:	if( botSelectInfo.modelpage > 0 ) {
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 0
LEI4 $1397
line 1740
;1740:		botSelectInfo.modelpage--;
ADDRLP4 0
ADDRGP4 botSelectInfo+4772
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1741
;1741:		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
ADDRGP4 botSelectInfo+4780
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 4
LSHI4
ASGNI4
line 1742
;1742:		UI_BotSelectMenu_UpdateGrid();
ADDRGP4 UI_BotSelectMenu_UpdateGrid
CALLV
pop
line 1743
;1743:	}
LABELV $1397
line 1744
;1744:}
LABELV $1394
endproc UI_BotSelectMenu_LeftEvent 4 0
proc UI_BotSelectMenu_RightEvent 4 0
line 1752
;1745:
;1746:
;1747:/*
;1748:=================
;1749:UI_BotSelectMenu_RightEvent
;1750:=================
;1751:*/
;1752:static void UI_BotSelectMenu_RightEvent( void* ptr, int event ) {
line 1753
;1753:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $1404
line 1754
;1754:		return;
ADDRGP4 $1403
JUMPV
LABELV $1404
line 1756
;1755:	}
;1756:	if( botSelectInfo.modelpage < botSelectInfo.numpages - 1 ) {
ADDRGP4 botSelectInfo+4772
INDIRI4
ADDRGP4 botSelectInfo+4776
INDIRI4
CNSTI4 1
SUBI4
GEI4 $1406
line 1757
;1757:		botSelectInfo.modelpage++;
ADDRLP4 0
ADDRGP4 botSelectInfo+4772
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1758
;1758:		botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE;
ADDRGP4 botSelectInfo+4780
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 4
LSHI4
ASGNI4
line 1759
;1759:		UI_BotSelectMenu_UpdateGrid();
ADDRGP4 UI_BotSelectMenu_UpdateGrid
CALLV
pop
line 1760
;1760:	}
LABELV $1406
line 1761
;1761:}
LABELV $1403
endproc UI_BotSelectMenu_RightEvent 4 0
proc UI_BotSelectMenu_BotEvent 12 0
line 1769
;1762:
;1763:
;1764:/*
;1765:=================
;1766:UI_BotSelectMenu_BotEvent
;1767:=================
;1768:*/
;1769:static void UI_BotSelectMenu_BotEvent( void* ptr, int event ) {
line 1772
;1770:	int		i;
;1771:
;1772:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $1414
line 1773
;1773:		return;
ADDRGP4 $1413
JUMPV
LABELV $1414
line 1776
;1774:	}
;1775:
;1776:	for( i = 0; i < (PLAYERGRID_ROWS * PLAYERGRID_COLS); i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1416
line 1777
;1777: 		botSelectInfo.pics[i].generic.flags &= ~QMF_HIGHLIGHT;
ADDRLP4 4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+44
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 4294967231
BANDU4
ASGNU4
line 1778
;1778: 		botSelectInfo.picbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
ADDRLP4 8
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 256
BORU4
ASGNU4
line 1779
;1779:	}
LABELV $1417
line 1776
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $1416
line 1782
;1780:
;1781:	// set selected
;1782:	i = ((menucommon_s*)ptr)->id;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1783
;1783:	botSelectInfo.pics[i].generic.flags |= QMF_HIGHLIGHT;
ADDRLP4 4
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+44
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 64
BORU4
ASGNU4
line 1784
;1784:	botSelectInfo.picbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
ADDRLP4 8
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 4294967039
BANDU4
ASGNU4
line 1785
;1785:	botSelectInfo.selectedmodel = botSelectInfo.modelpage * MAX_MODELSPERPAGE + i;
ADDRGP4 botSelectInfo+4780
ADDRGP4 botSelectInfo+4772
INDIRI4
CNSTI4 4
LSHI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 1786
;1786:}
LABELV $1413
endproc UI_BotSelectMenu_BotEvent 12 0
proc UI_BotSelectMenu_BackEvent 0 0
line 1794
;1787:
;1788:
;1789:/*
;1790:=================
;1791:UI_BotSelectMenu_BackEvent
;1792:=================
;1793:*/
;1794:static void UI_BotSelectMenu_BackEvent( void* ptr, int event ) {
line 1795
;1795:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $1431
line 1796
;1796:		return;
ADDRGP4 $1430
JUMPV
LABELV $1431
line 1798
;1797:	}
;1798:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 1799
;1799:}
LABELV $1430
endproc UI_BotSelectMenu_BackEvent 0 0
proc UI_BotSelectMenu_SelectEvent 4 12
line 1807
;1800:
;1801:
;1802:/*
;1803:=================
;1804:UI_BotSelectMenu_SelectEvent
;1805:=================
;1806:*/
;1807:static void UI_BotSelectMenu_SelectEvent( void* ptr, int event ) {
line 1808
;1808:	if( event != QM_ACTIVATED ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $1434
line 1809
;1809:		return;
ADDRGP4 $1433
JUMPV
LABELV $1434
line 1811
;1810:	}
;1811:	UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 1813
;1812:
;1813:	s_serveroptions.newBot = qtrue;
ADDRGP4 s_serveroptions+5920
CNSTI4 1
ASGNI4
line 1814
;1814:	Q_strncpyz( s_serveroptions.newBotName, botSelectInfo.botnames[botSelectInfo.selectedmodel % MAX_MODELSPERPAGE], 16 );
ADDRGP4 s_serveroptions+5928
ARGP4
ADDRLP4 0
CNSTI4 16
ASGNI4
ADDRGP4 botSelectInfo+4780
INDIRI4
ADDRLP4 0
INDIRI4
MODI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1815
;1815:}
LABELV $1433
endproc UI_BotSelectMenu_SelectEvent 4 12
export UI_BotSelectMenu_Cache
proc UI_BotSelectMenu_Cache 0 4
line 1823
;1816:
;1817:
;1818:/*
;1819:=================
;1820:UI_BotSelectMenu_Cache
;1821:=================
;1822:*/
;1823:void UI_BotSelectMenu_Cache( void ) {
line 1824
;1824:	trap_R_RegisterShaderNoMip( BOTSELECT_BACK0 );
ADDRGP4 $403
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1825
;1825:	trap_R_RegisterShaderNoMip( BOTSELECT_BACK1 );
ADDRGP4 $420
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1826
;1826:	trap_R_RegisterShaderNoMip( BOTSELECT_ACCEPT0 );
ADDRGP4 $1441
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1827
;1827:	trap_R_RegisterShaderNoMip( BOTSELECT_ACCEPT1 );
ADDRGP4 $1442
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1828
;1828:	trap_R_RegisterShaderNoMip( BOTSELECT_SELECT );
ADDRGP4 $1443
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1829
;1829:	trap_R_RegisterShaderNoMip( BOTSELECT_SELECTED );
ADDRGP4 $1444
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1830
;1830:	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWS );
ADDRGP4 $340
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1831
;1831:	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWSL );
ADDRGP4 $368
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1832
;1832:	trap_R_RegisterShaderNoMip( BOTSELECT_ARROWSR );
ADDRGP4 $386
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1833
;1833:}
LABELV $1440
endproc UI_BotSelectMenu_Cache 0 4
proc UI_BotSelectMenu_Init 36 12
line 1836
;1834:
;1835:
;1836:static void UI_BotSelectMenu_Init( char *bot ) {
line 1840
;1837:	int		i, j, k;
;1838:	int		x, y;
;1839:
;1840:	memset( &botSelectInfo, 0 ,sizeof(botSelectInfo) );
ADDRGP4 botSelectInfo
ARGP4
CNSTI4 0
ARGI4
CNSTI4 10160
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1841
;1841:	botSelectInfo.menu.wrapAround = qtrue;
ADDRGP4 botSelectInfo+276
CNSTI4 1
ASGNI4
line 1842
;1842:	botSelectInfo.menu.fullscreen = qtrue;
ADDRGP4 botSelectInfo+280
CNSTI4 1
ASGNI4
line 1844
;1843:
;1844:	UI_BotSelectMenu_Cache();
ADDRGP4 UI_BotSelectMenu_Cache
CALLV
pop
line 1846
;1845:
;1846:	botSelectInfo.banner.generic.type	= MTYPE_BTEXT;
ADDRGP4 botSelectInfo+288
CNSTI4 10
ASGNI4
line 1847
;1847:	botSelectInfo.banner.generic.x		= 320;
ADDRGP4 botSelectInfo+288+12
CNSTI4 320
ASGNI4
line 1848
;1848:	botSelectInfo.banner.generic.y		= 16;
ADDRGP4 botSelectInfo+288+16
CNSTI4 16
ASGNI4
line 1849
;1849:	botSelectInfo.banner.string			= "SELECT BOT";
ADDRGP4 botSelectInfo+288+60
ADDRGP4 $1455
ASGNP4
line 1850
;1850:	botSelectInfo.banner.color			= color_white;
ADDRGP4 botSelectInfo+288+68
ADDRGP4 color_white
ASGNP4
line 1851
;1851:	botSelectInfo.banner.style			= UI_CENTER;
ADDRGP4 botSelectInfo+288+64
CNSTI4 1
ASGNI4
line 1853
;1852:
;1853:	y =	80;
ADDRLP4 8
CNSTI4 80
ASGNI4
line 1854
;1854:	for( i = 0, k = 0; i < PLAYERGRID_ROWS; i++) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 16
ADDRLP4 20
INDIRI4
ASGNI4
ADDRLP4 0
ADDRLP4 20
INDIRI4
ASGNI4
ADDRGP4 $1463
JUMPV
LABELV $1460
line 1855
;1855:		x =	180;
ADDRLP4 4
CNSTI4 180
ASGNI4
line 1856
;1856:		for( j = 0; j < PLAYERGRID_COLS; j++, k++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
LABELV $1464
line 1857
;1857:			botSelectInfo.pics[k].generic.type				= MTYPE_BITMAP;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360
ADDP4
CNSTI4 6
ASGNI4
line 1858
;1858:			botSelectInfo.pics[k].generic.flags				= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+44
ADDP4
CNSTU4 16388
ASGNU4
line 1859
;1859:			botSelectInfo.pics[k].generic.x					= x;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+12
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1860
;1860:			botSelectInfo.pics[k].generic.y					= y;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+16
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1861
;1861: 			botSelectInfo.pics[k].generic.name				= botSelectInfo.boticons[k];
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+4
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 botSelectInfo+8880
ADDP4
ASGNP4
line 1862
;1862:			botSelectInfo.pics[k].width						= 64;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+76
ADDP4
CNSTI4 64
ASGNI4
line 1863
;1863:			botSelectInfo.pics[k].height					= 64;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+80
ADDP4
CNSTI4 64
ASGNI4
line 1864
;1864:			botSelectInfo.pics[k].focuspic					= BOTSELECT_SELECTED;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+60
ADDP4
ADDRGP4 $1444
ASGNP4
line 1865
;1865:			botSelectInfo.pics[k].focuscolor				= colorRed;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360+84
ADDP4
ADDRGP4 colorRed
ASGNP4
line 1867
;1866:
;1867:			botSelectInfo.picbuttons[k].generic.type		= MTYPE_BITMAP;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768
ADDP4
CNSTI4 6
ASGNI4
line 1868
;1868:			botSelectInfo.picbuttons[k].generic.flags		= QMF_LEFT_JUSTIFY|QMF_NODEFAULTINIT|QMF_PULSEIFFOCUS;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+44
ADDP4
CNSTU4 33028
ASGNU4
line 1869
;1869:			botSelectInfo.picbuttons[k].generic.callback	= UI_BotSelectMenu_BotEvent;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+48
ADDP4
ADDRGP4 UI_BotSelectMenu_BotEvent
ASGNP4
line 1870
;1870:			botSelectInfo.picbuttons[k].generic.id			= k;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+8
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1871
;1871:			botSelectInfo.picbuttons[k].generic.x			= x - 16;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+12
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 16
SUBI4
ASGNI4
line 1872
;1872:			botSelectInfo.picbuttons[k].generic.y			= y - 16;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+16
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 16
SUBI4
ASGNI4
line 1873
;1873:			botSelectInfo.picbuttons[k].generic.left		= x;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+20
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1874
;1874:			botSelectInfo.picbuttons[k].generic.top			= y;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+24
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1875
;1875:			botSelectInfo.picbuttons[k].generic.right		= x + 64;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+28
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 64
ADDI4
ASGNI4
line 1876
;1876:			botSelectInfo.picbuttons[k].generic.bottom		= y + 64;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+32
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 64
ADDI4
ASGNI4
line 1877
;1877:			botSelectInfo.picbuttons[k].width				= 128;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+76
ADDP4
CNSTI4 128
ASGNI4
line 1878
;1878:			botSelectInfo.picbuttons[k].height				= 128;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+80
ADDP4
CNSTI4 128
ASGNI4
line 1879
;1879:			botSelectInfo.picbuttons[k].focuspic			= BOTSELECT_SELECT;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+60
ADDP4
ADDRGP4 $1443
ASGNP4
line 1880
;1880:			botSelectInfo.picbuttons[k].focuscolor			= colorRed;
CNSTI4 88
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768+84
ADDP4
ADDRGP4 colorRed
ASGNP4
line 1882
;1881:
;1882:			botSelectInfo.picnames[k].generic.type			= MTYPE_TEXT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176
ADDP4
CNSTI4 7
ASGNI4
line 1883
;1883:			botSelectInfo.picnames[k].generic.flags			= QMF_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+44
ADDP4
CNSTU4 2
ASGNU4
line 1884
;1884:			botSelectInfo.picnames[k].generic.x				= x + 32;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+12
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 32
ADDI4
ASGNI4
line 1885
;1885:			botSelectInfo.picnames[k].generic.y				= y + 64;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+16
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 64
ADDI4
ASGNI4
line 1886
;1886:			botSelectInfo.picnames[k].string				= botSelectInfo.botnames[k];
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+60
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 botSelectInfo+9904
ADDP4
ASGNP4
line 1887
;1887:			botSelectInfo.picnames[k].color					= color_orange;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+68
ADDP4
ADDRGP4 color_orange
ASGNP4
line 1888
;1888:			botSelectInfo.picnames[k].style					= UI_CENTER|UI_SMALLFONT;
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176+64
ADDP4
CNSTI4 17
ASGNI4
line 1890
;1889:
;1890:			x += (64 + 6);
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 70
ADDI4
ASGNI4
line 1891
;1891:		}
LABELV $1465
line 1856
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 24
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 24
INDIRI4
ADDI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 4
LTI4 $1464
line 1892
;1892:		y += (64 + SMALLCHAR_HEIGHT + 6);
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 86
ADDI4
ASGNI4
line 1893
;1893:	}
LABELV $1461
line 1854
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1463
ADDRLP4 16
INDIRI4
CNSTI4 4
LTI4 $1460
line 1895
;1894:
;1895:	botSelectInfo.arrows.generic.type		= MTYPE_BITMAP;
ADDRGP4 botSelectInfo+4328
CNSTI4 6
ASGNI4
line 1896
;1896:	botSelectInfo.arrows.generic.name		= BOTSELECT_ARROWS;
ADDRGP4 botSelectInfo+4328+4
ADDRGP4 $340
ASGNP4
line 1897
;1897:	botSelectInfo.arrows.generic.flags		= QMF_INACTIVE;
ADDRGP4 botSelectInfo+4328+44
CNSTU4 16384
ASGNU4
line 1898
;1898:	botSelectInfo.arrows.generic.x			= 260;
ADDRGP4 botSelectInfo+4328+12
CNSTI4 260
ASGNI4
line 1899
;1899:	botSelectInfo.arrows.generic.y			= 440;
ADDRGP4 botSelectInfo+4328+16
CNSTI4 440
ASGNI4
line 1900
;1900:	botSelectInfo.arrows.width				= 128;
ADDRGP4 botSelectInfo+4328+76
CNSTI4 128
ASGNI4
line 1901
;1901:	botSelectInfo.arrows.height				= 32;
ADDRGP4 botSelectInfo+4328+80
CNSTI4 32
ASGNI4
line 1903
;1902:
;1903:	botSelectInfo.left.generic.type			= MTYPE_BITMAP;
ADDRGP4 botSelectInfo+4416
CNSTI4 6
ASGNI4
line 1904
;1904:	botSelectInfo.left.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 botSelectInfo+4416+44
CNSTU4 260
ASGNU4
line 1905
;1905:	botSelectInfo.left.generic.callback		= UI_BotSelectMenu_LeftEvent;
ADDRGP4 botSelectInfo+4416+48
ADDRGP4 UI_BotSelectMenu_LeftEvent
ASGNP4
line 1906
;1906:	botSelectInfo.left.generic.x			= 260;
ADDRGP4 botSelectInfo+4416+12
CNSTI4 260
ASGNI4
line 1907
;1907:	botSelectInfo.left.generic.y			= 440;
ADDRGP4 botSelectInfo+4416+16
CNSTI4 440
ASGNI4
line 1908
;1908:	botSelectInfo.left.width  				= 64;
ADDRGP4 botSelectInfo+4416+76
CNSTI4 64
ASGNI4
line 1909
;1909:	botSelectInfo.left.height  				= 32;
ADDRGP4 botSelectInfo+4416+80
CNSTI4 32
ASGNI4
line 1910
;1910:	botSelectInfo.left.focuspic				= BOTSELECT_ARROWSL;
ADDRGP4 botSelectInfo+4416+60
ADDRGP4 $368
ASGNP4
line 1912
;1911:
;1912:	botSelectInfo.right.generic.type	    = MTYPE_BITMAP;
ADDRGP4 botSelectInfo+4504
CNSTI4 6
ASGNI4
line 1913
;1913:	botSelectInfo.right.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 botSelectInfo+4504+44
CNSTU4 260
ASGNU4
line 1914
;1914:	botSelectInfo.right.generic.callback	= UI_BotSelectMenu_RightEvent;
ADDRGP4 botSelectInfo+4504+48
ADDRGP4 UI_BotSelectMenu_RightEvent
ASGNP4
line 1915
;1915:	botSelectInfo.right.generic.x			= 321;
ADDRGP4 botSelectInfo+4504+12
CNSTI4 321
ASGNI4
line 1916
;1916:	botSelectInfo.right.generic.y			= 440;
ADDRGP4 botSelectInfo+4504+16
CNSTI4 440
ASGNI4
line 1917
;1917:	botSelectInfo.right.width  				= 64;
ADDRGP4 botSelectInfo+4504+76
CNSTI4 64
ASGNI4
line 1918
;1918:	botSelectInfo.right.height  		    = 32;
ADDRGP4 botSelectInfo+4504+80
CNSTI4 32
ASGNI4
line 1919
;1919:	botSelectInfo.right.focuspic			= BOTSELECT_ARROWSR;
ADDRGP4 botSelectInfo+4504+60
ADDRGP4 $386
ASGNP4
line 1921
;1920:
;1921:	botSelectInfo.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 botSelectInfo+4680
CNSTI4 6
ASGNI4
line 1922
;1922:	botSelectInfo.back.generic.name		= BOTSELECT_BACK0;
ADDRGP4 botSelectInfo+4680+4
ADDRGP4 $403
ASGNP4
line 1923
;1923:	botSelectInfo.back.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 botSelectInfo+4680+44
CNSTU4 260
ASGNU4
line 1924
;1924:	botSelectInfo.back.generic.callback	= UI_BotSelectMenu_BackEvent;
ADDRGP4 botSelectInfo+4680+48
ADDRGP4 UI_BotSelectMenu_BackEvent
ASGNP4
line 1925
;1925:	botSelectInfo.back.generic.x		= 0;
ADDRGP4 botSelectInfo+4680+12
CNSTI4 0
ASGNI4
line 1926
;1926:	botSelectInfo.back.generic.y		= 480-64;
ADDRGP4 botSelectInfo+4680+16
CNSTI4 416
ASGNI4
line 1927
;1927:	botSelectInfo.back.width			= 128;
ADDRGP4 botSelectInfo+4680+76
CNSTI4 128
ASGNI4
line 1928
;1928:	botSelectInfo.back.height			= 64;
ADDRGP4 botSelectInfo+4680+80
CNSTI4 64
ASGNI4
line 1929
;1929:	botSelectInfo.back.focuspic			= BOTSELECT_BACK1;
ADDRGP4 botSelectInfo+4680+60
ADDRGP4 $420
ASGNP4
line 1931
;1930:
;1931:	botSelectInfo.go.generic.type		= MTYPE_BITMAP;
ADDRGP4 botSelectInfo+4592
CNSTI4 6
ASGNI4
line 1932
;1932:	botSelectInfo.go.generic.name		= BOTSELECT_ACCEPT0;
ADDRGP4 botSelectInfo+4592+4
ADDRGP4 $1441
ASGNP4
line 1933
;1933:	botSelectInfo.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 botSelectInfo+4592+44
CNSTU4 272
ASGNU4
line 1934
;1934:	botSelectInfo.go.generic.callback	= UI_BotSelectMenu_SelectEvent;
ADDRGP4 botSelectInfo+4592+48
ADDRGP4 UI_BotSelectMenu_SelectEvent
ASGNP4
line 1935
;1935:	botSelectInfo.go.generic.x			= 640;
ADDRGP4 botSelectInfo+4592+12
CNSTI4 640
ASGNI4
line 1936
;1936:	botSelectInfo.go.generic.y			= 480-64;
ADDRGP4 botSelectInfo+4592+16
CNSTI4 416
ASGNI4
line 1937
;1937:	botSelectInfo.go.width				= 128;
ADDRGP4 botSelectInfo+4592+76
CNSTI4 128
ASGNI4
line 1938
;1938:	botSelectInfo.go.height				= 64;
ADDRGP4 botSelectInfo+4592+80
CNSTI4 64
ASGNI4
line 1939
;1939:	botSelectInfo.go.focuspic			= BOTSELECT_ACCEPT1;
ADDRGP4 botSelectInfo+4592+60
ADDRGP4 $1442
ASGNP4
line 1941
;1940:
;1941:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.banner );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1942
;1942:	for( i = 0; i < MAX_MODELSPERPAGE; i++ ) {
ADDRLP4 16
CNSTI4 0
ASGNI4
LABELV $1605
line 1943
;1943:		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.pics[i] );
ADDRGP4 botSelectInfo
ARGP4
CNSTI4 88
ADDRLP4 16
INDIRI4
MULI4
ADDRGP4 botSelectInfo+360
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1944
;1944:		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.picbuttons[i] );
ADDRGP4 botSelectInfo
ARGP4
CNSTI4 88
ADDRLP4 16
INDIRI4
MULI4
ADDRGP4 botSelectInfo+1768
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1945
;1945:		Menu_AddItem( &botSelectInfo.menu,	&botSelectInfo.picnames[i] );
ADDRGP4 botSelectInfo
ARGP4
CNSTI4 72
ADDRLP4 16
INDIRI4
MULI4
ADDRGP4 botSelectInfo+3176
ADDP4
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1946
;1946:	}
LABELV $1606
line 1942
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 16
LTI4 $1605
line 1947
;1947:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.arrows );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+4328
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1948
;1948:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.left );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+4416
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1949
;1949:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.right );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+4504
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1950
;1950:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.back );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+4680
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1951
;1951:	Menu_AddItem( &botSelectInfo.menu, &botSelectInfo.go );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 botSelectInfo+4592
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1953
;1952:
;1953:	UI_BotSelectMenu_BuildList();
ADDRGP4 UI_BotSelectMenu_BuildList
CALLV
pop
line 1954
;1954:	UI_BotSelectMenu_Default( bot );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 UI_BotSelectMenu_Default
CALLV
pop
line 1955
;1955:	botSelectInfo.modelpage = botSelectInfo.selectedmodel / MAX_MODELSPERPAGE;
ADDRGP4 botSelectInfo+4772
ADDRGP4 botSelectInfo+4780
INDIRI4
CNSTI4 16
DIVI4
ASGNI4
line 1956
;1956:	UI_BotSelectMenu_UpdateGrid();
ADDRGP4 UI_BotSelectMenu_UpdateGrid
CALLV
pop
line 1957
;1957:}
LABELV $1445
endproc UI_BotSelectMenu_Init 36 12
export UI_BotSelectMenu
proc UI_BotSelectMenu 0 4
line 1965
;1958:
;1959:
;1960:/*
;1961:=================
;1962:UI_BotSelectMenu
;1963:=================
;1964:*/
;1965:void UI_BotSelectMenu( char *bot ) {
line 1966
;1966:	UI_BotSelectMenu_Init( bot );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 UI_BotSelectMenu_Init
CALLV
pop
line 1967
;1967:	UI_PushMenu( &botSelectInfo.menu );
ADDRGP4 botSelectInfo
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 1968
;1968:}
LABELV $1619
endproc UI_BotSelectMenu 0 4
bss
align 4
LABELV botSelectInfo
skip 10160
align 4
LABELV s_serveroptions
skip 6040
import punkbuster_items
align 4
LABELV s_startserver
skip 3236
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
LABELV $1455
byte 1 83
byte 1 69
byte 1 76
byte 1 69
byte 1 67
byte 1 84
byte 1 32
byte 1 66
byte 1 79
byte 1 84
byte 1 0
align 1
LABELV $1444
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 111
byte 1 112
byte 1 112
byte 1 111
byte 1 110
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $1443
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 111
byte 1 112
byte 1 112
byte 1 111
byte 1 110
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $1442
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
LABELV $1441
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
LABELV $1315
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $1304
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $1301
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 115
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 47
byte 1 105
byte 1 99
byte 1 111
byte 1 110
byte 1 95
byte 1 37
byte 1 115
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $1300
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $1234
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
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $1217
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
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $1090
byte 1 66
byte 1 111
byte 1 116
byte 1 32
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 58
byte 1 32
byte 1 32
byte 1 0
align 1
LABELV $1074
byte 1 80
byte 1 117
byte 1 110
byte 1 107
byte 1 98
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $1058
byte 1 72
byte 1 111
byte 1 115
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $1049
byte 1 68
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 58
byte 1 0
align 1
LABELV $1032
byte 1 80
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 58
byte 1 0
align 1
LABELV $1022
byte 1 70
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 32
byte 1 70
byte 1 105
byte 1 114
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $995
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $977
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $959
byte 1 70
byte 1 114
byte 1 97
byte 1 103
byte 1 32
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $877
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 0
align 1
LABELV $871
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $866
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 0
align 1
LABELV $846
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $820
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 0
align 1
LABELV $796
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $776
byte 1 115
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $764
byte 1 118
byte 1 105
byte 1 115
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $758
byte 1 109
byte 1 97
byte 1 106
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $755
byte 1 103
byte 1 114
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $738
byte 1 48
byte 1 32
byte 1 61
byte 1 32
byte 1 78
byte 1 79
byte 1 32
byte 1 76
byte 1 73
byte 1 77
byte 1 73
byte 1 84
byte 1 0
align 1
LABELV $678
byte 1 72
byte 1 117
byte 1 109
byte 1 97
byte 1 110
byte 1 0
align 1
LABELV $646
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $615
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 53
byte 1 59
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $610
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $606
byte 1 97
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $588
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 51
byte 1 10
byte 1 0
align 1
LABELV $585
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 59
byte 1 32
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 32
byte 1 59
byte 1 32
byte 1 109
byte 1 97
byte 1 112
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $582
byte 1 115
byte 1 118
byte 1 95
byte 1 112
byte 1 117
byte 1 110
byte 1 107
byte 1 98
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $578
byte 1 115
byte 1 118
byte 1 95
byte 1 104
byte 1 111
byte 1 115
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $577
byte 1 115
byte 1 118
byte 1 95
byte 1 112
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $576
byte 1 103
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 102
byte 1 105
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $575
byte 1 99
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $574
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $573
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $572
byte 1 100
byte 1 101
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $571
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
LABELV $569
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $568
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $567
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 116
byte 1 102
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $565
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $564
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $563
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $561
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $560
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $558
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 102
byte 1 97
byte 1 95
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $557
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 102
byte 1 97
byte 1 95
byte 1 102
byte 1 114
byte 1 97
byte 1 103
byte 1 108
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $501
byte 1 78
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 109
byte 1 97
byte 1 114
byte 1 101
byte 1 33
byte 1 0
align 1
LABELV $500
byte 1 72
byte 1 97
byte 1 114
byte 1 100
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $499
byte 1 72
byte 1 117
byte 1 114
byte 1 116
byte 1 32
byte 1 77
byte 1 101
byte 1 32
byte 1 80
byte 1 108
byte 1 101
byte 1 110
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $498
byte 1 66
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 73
byte 1 116
byte 1 32
byte 1 79
byte 1 110
byte 1 0
align 1
LABELV $497
byte 1 73
byte 1 32
byte 1 67
byte 1 97
byte 1 110
byte 1 32
byte 1 87
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $496
byte 1 82
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $495
byte 1 66
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $494
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 0
align 1
LABELV $493
byte 1 66
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $492
byte 1 79
byte 1 112
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $491
byte 1 73
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 110
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $490
byte 1 76
byte 1 65
byte 1 78
byte 1 0
align 1
LABELV $489
byte 1 78
byte 1 111
byte 1 0
align 1
LABELV $471
byte 1 99
byte 1 111
byte 1 109
byte 1 95
byte 1 98
byte 1 117
byte 1 105
byte 1 108
byte 1 100
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 0
align 1
LABELV $441
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $424
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 95
byte 1 48
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
byte 1 49
byte 1 0
align 1
LABELV $403
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
LABELV $386
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 103
byte 1 115
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 114
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
byte 1 103
byte 1 115
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 108
byte 1 0
align 1
LABELV $340
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 103
byte 1 115
byte 1 95
byte 1 97
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $336
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
byte 1 97
byte 1 112
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $308
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $305
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
byte 1 97
byte 1 112
byte 1 115
byte 1 95
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $273
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 58
byte 1 0
align 1
LABELV $259
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
LABELV $237
byte 1 71
byte 1 65
byte 1 77
byte 1 69
byte 1 32
byte 1 83
byte 1 69
byte 1 82
byte 1 86
byte 1 69
byte 1 82
byte 1 0
align 1
LABELV $204
byte 1 103
byte 1 95
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $176
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $171
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $136
byte 1 78
byte 1 79
byte 1 32
byte 1 77
byte 1 65
byte 1 80
byte 1 83
byte 1 32
byte 1 70
byte 1 79
byte 1 85
byte 1 78
byte 1 68
byte 1 0
align 1
LABELV $103
byte 1 108
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $92
byte 1 99
byte 1 116
byte 1 102
byte 1 0
align 1
LABELV $89
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $86
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $83
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $80
byte 1 102
byte 1 102
byte 1 97
byte 1 0
align 1
LABELV $71
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $70
byte 1 84
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $69
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 68
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 0
align 1
LABELV $68
byte 1 70
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 70
byte 1 111
byte 1 114
byte 1 32
byte 1 65
byte 1 108
byte 1 108
byte 1 0
