data
align 4
LABELV master_items
address $67
address $68
address $69
byte 4 0
align 4
LABELV servertype_items
address $70
address $71
address $72
address $73
address $74
byte 4 0
align 4
LABELV sortkey_items
address $75
address $76
address $77
address $78
address $79
byte 4 0
align 4
LABELV gamenames
address $80
address $81
address $82
address $83
address $84
address $85
address $86
address $87
address $88
address $89
address $90
address $91
address $92
byte 4 0
align 4
LABELV netnames
address $92
address $93
address $94
byte 4 0
align 1
LABELV quake3worldMessage
byte 1 86
byte 1 105
byte 1 115
byte 1 105
byte 1 116
byte 1 32
byte 1 119
byte 1 119
byte 1 119
byte 1 46
byte 1 113
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 51
byte 1 119
byte 1 111
byte 1 114
byte 1 108
byte 1 100
byte 1 46
byte 1 99
byte 1 111
byte 1 109
byte 1 32
byte 1 45
byte 1 32
byte 1 78
byte 1 101
byte 1 119
byte 1 115
byte 1 44
byte 1 32
byte 1 67
byte 1 111
byte 1 109
byte 1 109
byte 1 117
byte 1 110
byte 1 105
byte 1 116
byte 1 121
byte 1 44
byte 1 32
byte 1 69
byte 1 118
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 44
byte 1 32
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 115
byte 1 0
export punkbuster_items
align 4
LABELV punkbuster_items
address $95
address $96
byte 4 0
export punkbuster_msg
align 4
LABELV punkbuster_msg
address $97
address $98
address $99
address $100
byte 4 0
code
proc ArenaServers_MaxPing 8 4
file "../ui_servers2.c"
line 258
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
;26:MULTIPLAYER MENU (SERVER BROWSER)
;27:
;28:=======================================================================
;29:*/
;30:
;31:
;32:#include "ui_local.h"
;33:
;34:
;35:#define MAX_GLOBALSERVERS		128
;36:#define MAX_PINGREQUESTS		32
;37:#define MAX_ADDRESSLENGTH		64
;38:#define MAX_HOSTNAMELENGTH		22
;39:#define MAX_MAPNAMELENGTH		16
;40:#define MAX_LISTBOXITEMS		128
;41:#define MAX_LOCALSERVERS		128
;42:#define MAX_STATUSLENGTH		64
;43:#define MAX_LEAGUELENGTH		28
;44:#define MAX_LISTBOXWIDTH		68
;45:
;46:#define ART_BACK0				"menu/art/back_0"
;47:#define ART_BACK1				"menu/art/back_1"
;48:#define ART_CREATE0				"menu/art/create_0"
;49:#define ART_CREATE1				"menu/art/create_1"
;50:#define ART_SPECIFY0			"menu/art/specify_0"
;51:#define ART_SPECIFY1			"menu/art/specify_1"
;52:#define ART_REFRESH0			"menu/art/refresh_0"
;53:#define ART_REFRESH1			"menu/art/refresh_1"
;54:#define ART_CONNECT0			"menu/art/fight_0"
;55:#define ART_CONNECT1			"menu/art/fight_1"
;56:#define ART_ARROWS0				"menu/art/arrows_vert_0"
;57:#define ART_ARROWS_UP			"menu/art/arrows_vert_top"
;58:#define ART_ARROWS_DOWN			"menu/art/arrows_vert_bot"
;59:#define ART_UNKNOWNMAP			"menu/art/unknownmap"
;60:#define ART_REMOVE0				"menu/art/delete_0"
;61:#define ART_REMOVE1				"menu/art/delete_1"
;62:#define ART_PUNKBUSTER		"menu/art/pblogo"
;63:
;64:#define ID_MASTER			10
;65:#define ID_GAMETYPE			11
;66:#define ID_SORTKEY			12
;67:#define ID_SHOW_FULL		13
;68:#define ID_SHOW_EMPTY		14
;69:#define ID_LIST				15
;70:#define ID_SCROLL_UP		16
;71:#define ID_SCROLL_DOWN		17
;72:#define ID_BACK				18
;73:#define ID_REFRESH			19
;74:#define ID_SPECIFY			20
;75:#define ID_CREATE			21
;76:#define ID_CONNECT			22
;77:#define ID_REMOVE			23
;78:#define ID_PUNKBUSTER 24
;79:
;80:#define GR_LOGO				30
;81:#define GR_LETTERS			31
;82:
;83:#define AS_LOCAL			0
;84:#define AS_MPLAYER			1
;85:#define AS_GLOBAL			2
;86:#define AS_FAVORITES		3
;87:
;88:#define SORT_HOST			0
;89:#define SORT_MAP			1
;90:#define SORT_CLIENTS		2
;91:#define SORT_GAME			3
;92:#define SORT_PING			4
;93:
;94:#define GAMES_ALL			0
;95:#define GAMES_FFA			1
;96:#define GAMES_TEAMPLAY		2
;97:#define GAMES_TOURNEY		3
;98:#define GAMES_CTF			4
;99:
;100:static const char *master_items[] = {
;101:	"Local",
;102:	"Internet",
;103:	"Favorites",
;104:	0
;105:};
;106:
;107:static const char *servertype_items[] = {
;108:	"All",
;109:	"Free For All",
;110:	"Team Deathmatch",
;111:	"Tournament",
;112:	"Capture the Flag",
;113:	0
;114:};
;115:
;116:static const char *sortkey_items[] = {
;117:	"Server Name",
;118:	"Map Name",
;119:	"Open Player Spots",
;120:	"Game Type",
;121:	"Ping Time",
;122:	0
;123:};
;124:
;125:static char* gamenames[] = {
;126:	"DM ",	// deathmatch
;127:	"1v1",	// tournament
;128:	"SP ",	// single player
;129:	"Team DM",	// team deathmatch
;130:	"CTF",	// capture the flag
;131:	"One Flag CTF",		// one flag ctf
;132:	"OverLoad",				// Overload
;133:	"Harvester",			// Harvester
;134:	"Rocket Arena 3",	// Rocket Arena 3
;135:	"Q3F",						// Q3F
;136:	"Urban Terror",		// Urban Terror
;137:	"OSP",						// Orange Smoothie Productions
;138:	"???",			// unknown
;139:	0
;140:};
;141:
;142:static char* netnames[] = {
;143:	"???",
;144:	"UDP",
;145:	"IPX",
;146:	NULL
;147:};
;148:
;149:static char quake3worldMessage[] = "Visit www.quake3world.com - News, Community, Events, Files";
;150:
;151:const char* punkbuster_items[] = {
;152:	"Disabled",
;153:	"Enabled",
;154:	NULL
;155:};
;156:
;157:const char* punkbuster_msg[] = {
;158:	"PunkBuster will be",
;159:	"disabled the next time",
;160:	"Quake III Arena",
;161:	"is started.",
;162:	NULL
;163:};
;164:
;165:typedef struct {
;166:	char	adrstr[MAX_ADDRESSLENGTH];
;167:	int		start;
;168:} pinglist_t;
;169:
;170:typedef struct servernode_s {
;171:	char	adrstr[MAX_ADDRESSLENGTH];
;172:	char	hostname[MAX_HOSTNAMELENGTH+3];
;173:	char	mapname[MAX_MAPNAMELENGTH];
;174:	int		numclients;
;175:	int		maxclients;
;176:	int		pingtime;
;177:	int		gametype;
;178:	char	gamename[12];
;179:	int		nettype;
;180:	int		minPing;
;181:	int		maxPing;
;182:	qboolean bPB;
;183:
;184:} servernode_t; 
;185:
;186:typedef struct {
;187:	char			buff[MAX_LISTBOXWIDTH];
;188:	servernode_t*	servernode;
;189:} table_t;
;190:
;191:typedef struct {
;192:	menuframework_s		menu;
;193:
;194:	menutext_s			banner;
;195:
;196:	menulist_s			master;
;197:	menulist_s			gametype;
;198:	menulist_s			sortkey;
;199:	menuradiobutton_s	showfull;
;200:	menuradiobutton_s	showempty;
;201:
;202:	menulist_s			list;
;203:	menubitmap_s		mappic;
;204:	menubitmap_s		arrows;
;205:	menubitmap_s		up;
;206:	menubitmap_s		down;
;207:	menutext_s			status;
;208:	menutext_s			statusbar;
;209:
;210:	menubitmap_s		remove;
;211:	menubitmap_s		back;
;212:	menubitmap_s		refresh;
;213:	menubitmap_s		specify;
;214:	menubitmap_s		create;
;215:	menubitmap_s		go;
;216:
;217:	pinglist_t			pinglist[MAX_PINGREQUESTS];
;218:	table_t				table[MAX_LISTBOXITEMS];
;219:	char*				items[MAX_LISTBOXITEMS];
;220:	int					numqueriedservers;
;221:	int					*numservers;
;222:	servernode_t		*serverlist;	
;223:	int					currentping;
;224:	qboolean			refreshservers;
;225:	int					nextpingtime;
;226:	int					maxservers;
;227:	int					refreshtime;
;228:	char				favoriteaddresses[MAX_FAVORITESERVERS][MAX_ADDRESSLENGTH];
;229:	int					numfavoriteaddresses;
;230:
;231:	menulist_s		punkbuster;
;232:	menubitmap_s	pblogo;
;233:} arenaservers_t;
;234:
;235:static arenaservers_t	g_arenaservers;
;236:
;237:
;238:static servernode_t		g_globalserverlist[MAX_GLOBALSERVERS];
;239:static int				g_numglobalservers;
;240:static servernode_t		g_localserverlist[MAX_LOCALSERVERS];
;241:static int				g_numlocalservers;
;242:static servernode_t		g_favoriteserverlist[MAX_FAVORITESERVERS];
;243:static int				g_numfavoriteservers;
;244:static servernode_t		g_mplayerserverlist[MAX_GLOBALSERVERS];
;245:static int				g_nummplayerservers;
;246:static int				g_servertype;
;247:static int				g_gametype;
;248:static int				g_sortkey;
;249:static int				g_emptyservers;
;250:static int				g_fullservers;
;251:
;252:
;253:/*
;254:=================
;255:ArenaServers_MaxPing
;256:=================
;257:*/
;258:static int ArenaServers_MaxPing( void ) {
line 261
;259:	int		maxPing;
;260:
;261:	maxPing = (int)trap_Cvar_VariableValue( "cl_maxPing" );
ADDRGP4 $105
ARGP4
ADDRLP4 4
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 262
;262:	if( maxPing < 100 ) {
ADDRLP4 0
INDIRI4
CNSTI4 100
GEI4 $106
line 263
;263:		maxPing = 100;
ADDRLP4 0
CNSTI4 100
ASGNI4
line 264
;264:	}
LABELV $106
line 265
;265:	return maxPing;
ADDRLP4 0
INDIRI4
RETI4
LABELV $104
endproc ArenaServers_MaxPing 8 4
proc ArenaServers_Compare 68 8
line 274
;266:}
;267:
;268:
;269:/*
;270:=================
;271:ArenaServers_Compare
;272:=================
;273:*/
;274:static int QDECL ArenaServers_Compare( const void *arg1, const void *arg2 ) {
line 280
;275:	float			f1;
;276:	float			f2;
;277:	servernode_t*	t1;
;278:	servernode_t*	t2;
;279:
;280:	t1 = (servernode_t *)arg1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 281
;281:	t2 = (servernode_t *)arg2;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
line 283
;282:
;283:	switch( g_sortkey ) {
ADDRLP4 16
ADDRGP4 g_sortkey
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $109
ADDRLP4 16
INDIRI4
CNSTI4 4
GTI4 $109
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $132
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $132
address $111
address $112
address $113
address $122
address $127
code
LABELV $111
line 285
;284:	case SORT_HOST:
;285:		return Q_stricmp( t1->hostname, t2->hostname );
ADDRLP4 20
CNSTI4 64
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
RETI4
ADDRGP4 $108
JUMPV
LABELV $112
line 288
;286:
;287:	case SORT_MAP:
;288:		return Q_stricmp( t1->mapname, t2->mapname );
ADDRLP4 28
CNSTI4 89
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
RETI4
ADDRGP4 $108
JUMPV
LABELV $113
line 291
;289:
;290:	case SORT_CLIENTS:
;291:		f1 = t1->maxclients - t1->numclients;
ADDRLP4 36
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
ADDRLP4 36
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 292
;292:		if( f1 < 0 ) {
ADDRLP4 8
INDIRF4
CNSTF4 0
GEF4 $114
line 293
;293:			f1 = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 294
;294:		}
LABELV $114
line 296
;295:
;296:		f2 = t2->maxclients - t2->numclients;
ADDRLP4 40
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 40
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
ADDRLP4 40
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 297
;297:		if( f2 < 0 ) {
ADDRLP4 12
INDIRF4
CNSTF4 0
GEF4 $116
line 298
;298:			f2 = 0;
ADDRLP4 12
CNSTF4 0
ASGNF4
line 299
;299:		}
LABELV $116
line 301
;300:
;301:		if( f1 < f2 ) {
ADDRLP4 8
INDIRF4
ADDRLP4 12
INDIRF4
GEF4 $118
line 302
;302:			return 1;
CNSTI4 1
RETI4
ADDRGP4 $108
JUMPV
LABELV $118
line 304
;303:		}
;304:		if( f1 == f2 ) {
ADDRLP4 8
INDIRF4
ADDRLP4 12
INDIRF4
NEF4 $120
line 305
;305:			return 0;
CNSTI4 0
RETI4
ADDRGP4 $108
JUMPV
LABELV $120
line 307
;306:		}
;307:		return -1;
CNSTI4 -1
RETI4
ADDRGP4 $108
JUMPV
LABELV $122
line 310
;308:
;309:	case SORT_GAME:
;310:		if( t1->gametype < t2->gametype ) {
ADDRLP4 44
CNSTI4 120
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRI4
GEI4 $123
line 311
;311:			return -1;
CNSTI4 -1
RETI4
ADDRGP4 $108
JUMPV
LABELV $123
line 313
;312:		}
;313:		if( t1->gametype == t2->gametype ) {
ADDRLP4 48
CNSTI4 120
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRI4
NEI4 $125
line 314
;314:			return 0;
CNSTI4 0
RETI4
ADDRGP4 $108
JUMPV
LABELV $125
line 316
;315:		}
;316:		return 1;
CNSTI4 1
RETI4
ADDRGP4 $108
JUMPV
LABELV $127
line 319
;317:
;318:	case SORT_PING:
;319:		if( t1->pingtime < t2->pingtime ) {
ADDRLP4 52
CNSTI4 116
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRI4
GEI4 $128
line 320
;320:			return -1;
CNSTI4 -1
RETI4
ADDRGP4 $108
JUMPV
LABELV $128
line 322
;321:		}
;322:		if( t1->pingtime > t2->pingtime ) {
ADDRLP4 56
CNSTI4 116
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRI4
LEI4 $130
line 323
;323:			return 1;
CNSTI4 1
RETI4
ADDRGP4 $108
JUMPV
LABELV $130
line 325
;324:		}
;325:		return Q_stricmp( t1->hostname, t2->hostname );
ADDRLP4 60
CNSTI4 64
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
ARGP4
ADDRLP4 64
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
RETI4
ADDRGP4 $108
JUMPV
LABELV $109
line 328
;326:	}
;327:
;328:	return 0;
CNSTI4 0
RETI4
LABELV $108
endproc ArenaServers_Compare 68 8
proc ArenaServers_Go 8 8
line 337
;329:}
;330:
;331:
;332:/*
;333:=================
;334:ArenaServers_Go
;335:=================
;336:*/
;337:static void ArenaServers_Go( void ) {
line 340
;338:	servernode_t*	servernode;
;339:
;340:	servernode = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
ADDRLP4 0
CNSTI4 72
ADDRGP4 g_arenaservers+776+64
INDIRI4
MULI4
ADDRGP4 g_arenaservers+4072+68
ADDP4
INDIRP4
ASGNP4
line 341
;341:	if( servernode ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $138
line 342
;342:		trap_Cmd_ExecuteText( EXEC_APPEND, va( "connect %s\n", servernode->adrstr ) );
ADDRGP4 $140
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 343
;343:	}
LABELV $138
line 344
;344:}
LABELV $133
endproc ArenaServers_Go 8 8
bss
align 1
LABELV $142
skip 64
code
proc ArenaServers_UpdatePicture 4 16
line 352
;345:
;346:
;347:/*
;348:=================
;349:ArenaServers_UpdatePicture
;350:=================
;351:*/
;352:static void ArenaServers_UpdatePicture( void ) {
line 356
;353:	static char		picname[64];
;354:	servernode_t*	servernodeptr;
;355:
;356:	if( !g_arenaservers.list.numitems ) {
ADDRGP4 g_arenaservers+776+68
INDIRI4
CNSTI4 0
NEI4 $143
line 357
;357:		g_arenaservers.mappic.generic.name = NULL;
ADDRGP4 g_arenaservers+872+4
CNSTP4 0
ASGNP4
line 358
;358:	}
ADDRGP4 $144
JUMPV
LABELV $143
line 359
;359:	else {
line 360
;360:		servernodeptr = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
ADDRLP4 0
CNSTI4 72
ADDRGP4 g_arenaservers+776+64
INDIRI4
MULI4
ADDRGP4 g_arenaservers+4072+68
ADDP4
INDIRP4
ASGNP4
line 361
;361:		Com_sprintf( picname, sizeof(picname), "levelshots/%s.tga", servernodeptr->mapname );
ADDRGP4 $142
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $153
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 362
;362:		g_arenaservers.mappic.generic.name = picname;
ADDRGP4 g_arenaservers+872+4
ADDRGP4 $142
ASGNP4
line 364
;363:	
;364:	}
LABELV $144
line 367
;365:
;366:	// force shader update during draw
;367:	g_arenaservers.mappic.shader = 0;
ADDRGP4 g_arenaservers+872+68
CNSTI4 0
ASGNI4
line 368
;368:}
LABELV $141
endproc ArenaServers_UpdatePicture 4 16
proc ArenaServers_UpdateMenu 68 48
line 376
;369:
;370:
;371:/*
;372:=================
;373:ArenaServers_UpdateMenu
;374:=================
;375:*/
;376:static void ArenaServers_UpdateMenu( void ) {
line 385
;377:	int				i;
;378:	int				j;
;379:	int				count;
;380:	char*			buff;
;381:	servernode_t*	servernodeptr;
;382:	table_t*		tableptr;
;383:	char			*pingColor;
;384:
;385:	if( g_arenaservers.numqueriedservers > 0 ) {
ADDRGP4 g_arenaservers+13800
INDIRI4
CNSTI4 0
LEI4 $159
line 387
;386:		// servers found
;387:		if( g_arenaservers.refreshservers && ( g_arenaservers.currentping <= g_arenaservers.numqueriedservers ) ) {
ADDRGP4 g_arenaservers+13816
INDIRI4
CNSTI4 0
EQI4 $162
ADDRGP4 g_arenaservers+13812
INDIRI4
ADDRGP4 g_arenaservers+13800
INDIRI4
GTI4 $162
line 389
;388:			// show progress
;389:			Com_sprintf( g_arenaservers.status.string, MAX_STATUSLENGTH, "%d of %d Arena Servers.", g_arenaservers.currentping, g_arenaservers.numqueriedservers);
ADDRGP4 g_arenaservers+1224+60
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $169
ARGP4
ADDRGP4 g_arenaservers+13812
INDIRI4
ARGI4
ADDRGP4 g_arenaservers+13800
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 390
;390:			g_arenaservers.statusbar.string  = "Press SPACE to stop";
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 $174
ASGNP4
line 391
;391:			qsort( g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof( servernode_t ), ArenaServers_Compare);
ADDRGP4 g_arenaservers+13808
INDIRP4
ARGP4
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 ArenaServers_Compare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 392
;392:		}
ADDRGP4 $160
JUMPV
LABELV $162
line 393
;393:		else {
line 395
;394:			// all servers pinged - enable controls
;395:			g_arenaservers.master.generic.flags		&= ~QMF_GRAYED;
ADDRLP4 28
ADDRGP4 g_arenaservers+360+44
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 396
;396:			g_arenaservers.gametype.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 32
ADDRGP4 g_arenaservers+456+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 397
;397:			g_arenaservers.sortkey.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 36
ADDRGP4 g_arenaservers+552+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 398
;398:			g_arenaservers.showempty.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 40
ADDRGP4 g_arenaservers+712+44
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 399
;399:			g_arenaservers.showfull.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 44
ADDRGP4 g_arenaservers+648+44
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 400
;400:			g_arenaservers.list.generic.flags		&= ~QMF_GRAYED;
ADDRLP4 48
ADDRGP4 g_arenaservers+776+44
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 401
;401:			g_arenaservers.refresh.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 52
ADDRGP4 g_arenaservers+1544+44
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 402
;402:			g_arenaservers.go.generic.flags			&= ~QMF_GRAYED;
ADDRLP4 56
ADDRGP4 g_arenaservers+1808+44
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 403
;403:			g_arenaservers.punkbuster.generic.flags &= ~QMF_GRAYED;
ADDRLP4 60
ADDRGP4 g_arenaservers+14860+44
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 406
;404:
;405:			// update status bar
;406:			if( g_servertype == AS_GLOBAL || g_servertype == AS_MPLAYER ) {
ADDRLP4 64
ADDRGP4 g_servertype
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 2
EQI4 $197
ADDRLP4 64
INDIRI4
CNSTI4 1
NEI4 $195
LABELV $197
line 407
;407:				g_arenaservers.statusbar.string = quake3worldMessage;
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 quake3worldMessage
ASGNP4
line 408
;408:			}
ADDRGP4 $160
JUMPV
LABELV $195
line 409
;409:			else {
line 410
;410:				g_arenaservers.statusbar.string = "";
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 $202
ASGNP4
line 411
;411:			}
line 413
;412:
;413:		}
line 414
;414:	}
ADDRGP4 $160
JUMPV
LABELV $159
line 415
;415:	else {
line 417
;416:		// no servers found
;417:		if( g_arenaservers.refreshservers ) {
ADDRGP4 g_arenaservers+13816
INDIRI4
CNSTI4 0
EQI4 $203
line 418
;418:			strcpy( g_arenaservers.status.string,"Scanning For Servers." );
ADDRGP4 g_arenaservers+1224+60
INDIRP4
ARGP4
ADDRGP4 $208
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 419
;419:			g_arenaservers.statusbar.string = "Press SPACE to stop";
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 $174
ASGNP4
line 422
;420:
;421:			// disable controls during refresh
;422:			g_arenaservers.master.generic.flags		|= QMF_GRAYED;
ADDRLP4 28
ADDRGP4 g_arenaservers+360+44
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 423
;423:			g_arenaservers.gametype.generic.flags	|= QMF_GRAYED;
ADDRLP4 32
ADDRGP4 g_arenaservers+456+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 424
;424:			g_arenaservers.sortkey.generic.flags	|= QMF_GRAYED;
ADDRLP4 36
ADDRGP4 g_arenaservers+552+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 425
;425:			g_arenaservers.showempty.generic.flags	|= QMF_GRAYED;
ADDRLP4 40
ADDRGP4 g_arenaservers+712+44
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 426
;426:			g_arenaservers.showfull.generic.flags	|= QMF_GRAYED;
ADDRLP4 44
ADDRGP4 g_arenaservers+648+44
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 427
;427:			g_arenaservers.list.generic.flags		|= QMF_GRAYED;
ADDRLP4 48
ADDRGP4 g_arenaservers+776+44
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 428
;428:			g_arenaservers.refresh.generic.flags	|= QMF_GRAYED;
ADDRLP4 52
ADDRGP4 g_arenaservers+1544+44
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 429
;429:			g_arenaservers.go.generic.flags			|= QMF_GRAYED;
ADDRLP4 56
ADDRGP4 g_arenaservers+1808+44
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 430
;430:			g_arenaservers.punkbuster.generic.flags |= QMF_GRAYED;
ADDRLP4 60
ADDRGP4 g_arenaservers+14860+44
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 431
;431:		}
ADDRGP4 $204
JUMPV
LABELV $203
line 432
;432:		else {
line 433
;433:			if( g_arenaservers.numqueriedservers < 0 ) {
ADDRGP4 g_arenaservers+13800
INDIRI4
CNSTI4 0
GEI4 $229
line 434
;434:				strcpy(g_arenaservers.status.string,"No Response From Master Server." );
ADDRGP4 g_arenaservers+1224+60
INDIRP4
ARGP4
ADDRGP4 $234
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 435
;435:			}
ADDRGP4 $230
JUMPV
LABELV $229
line 436
;436:			else {
line 437
;437:				strcpy(g_arenaservers.status.string,"No Servers Found." );
ADDRGP4 g_arenaservers+1224+60
INDIRP4
ARGP4
ADDRGP4 $237
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 438
;438:			}
LABELV $230
line 441
;439:
;440:			// update status bar
;441:			if( g_servertype == AS_GLOBAL || g_servertype == AS_MPLAYER ) {
ADDRLP4 28
ADDRGP4 g_servertype
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 2
EQI4 $240
ADDRLP4 28
INDIRI4
CNSTI4 1
NEI4 $238
LABELV $240
line 442
;442:				g_arenaservers.statusbar.string = quake3worldMessage;
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 quake3worldMessage
ASGNP4
line 443
;443:			}
ADDRGP4 $239
JUMPV
LABELV $238
line 444
;444:			else {
line 445
;445:				g_arenaservers.statusbar.string = "";
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 $202
ASGNP4
line 446
;446:			}
LABELV $239
line 449
;447:
;448:			// end of refresh - set control state
;449:			g_arenaservers.master.generic.flags		&= ~QMF_GRAYED;
ADDRLP4 32
ADDRGP4 g_arenaservers+360+44
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 450
;450:			g_arenaservers.gametype.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 36
ADDRGP4 g_arenaservers+456+44
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 451
;451:			g_arenaservers.sortkey.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 40
ADDRGP4 g_arenaservers+552+44
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 452
;452:			g_arenaservers.showempty.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 44
ADDRGP4 g_arenaservers+712+44
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 453
;453:			g_arenaservers.showfull.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 48
ADDRGP4 g_arenaservers+648+44
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 454
;454:			g_arenaservers.list.generic.flags		|= QMF_GRAYED;
ADDRLP4 52
ADDRGP4 g_arenaservers+776+44
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 455
;455:			g_arenaservers.refresh.generic.flags	&= ~QMF_GRAYED;
ADDRLP4 56
ADDRGP4 g_arenaservers+1544+44
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 456
;456:			g_arenaservers.go.generic.flags			|= QMF_GRAYED;
ADDRLP4 60
ADDRGP4 g_arenaservers+1808+44
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRU4
CNSTU4 8192
BORU4
ASGNU4
line 457
;457:			g_arenaservers.punkbuster.generic.flags &= ~QMF_GRAYED;
ADDRLP4 64
ADDRGP4 g_arenaservers+14860+44
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRU4
CNSTU4 4294959103
BANDU4
ASGNU4
line 458
;458:		}
LABELV $204
line 461
;459:
;460:		// zero out list box
;461:		g_arenaservers.list.numitems = 0;
ADDRGP4 g_arenaservers+776+68
CNSTI4 0
ASGNI4
line 462
;462:		g_arenaservers.list.curvalue = 0;
ADDRGP4 g_arenaservers+776+64
CNSTI4 0
ASGNI4
line 463
;463:		g_arenaservers.list.top      = 0;
ADDRGP4 g_arenaservers+776+72
CNSTI4 0
ASGNI4
line 466
;464:
;465:		// update picture
;466:		ArenaServers_UpdatePicture();
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
line 467
;467:		return;
ADDRGP4 $158
JUMPV
LABELV $160
line 471
;468:	}
;469:
;470:	// build list box strings - apply culling filters
;471:	servernodeptr = g_arenaservers.serverlist;
ADDRLP4 0
ADDRGP4 g_arenaservers+13808
INDIRP4
ASGNP4
line 472
;472:	count         = *g_arenaservers.numservers;
ADDRLP4 24
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ASGNI4
line 473
;473:	for( i = 0, j = 0; i < count; i++, servernodeptr++ ) {
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 12
ADDRLP4 28
INDIRI4
ASGNI4
ADDRLP4 8
ADDRLP4 28
INDIRI4
ASGNI4
ADDRGP4 $274
JUMPV
LABELV $271
line 474
;474:		tableptr = &g_arenaservers.table[j];
ADDRLP4 4
CNSTI4 72
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 g_arenaservers+4072
ADDP4
ASGNP4
line 475
;475:		tableptr->servernode = servernodeptr;
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 476
;476:		buff = tableptr->buff;
ADDRLP4 16
ADDRLP4 4
INDIRP4
ASGNP4
line 479
;477:
;478:		// can only cull valid results
;479:		if( !g_emptyservers && !servernodeptr->numclients ) {
ADDRLP4 32
CNSTI4 0
ASGNI4
ADDRGP4 g_emptyservers
INDIRI4
ADDRLP4 32
INDIRI4
NEI4 $276
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 32
INDIRI4
NEI4 $276
line 480
;480:			continue;
ADDRGP4 $272
JUMPV
LABELV $276
line 483
;481:		}
;482:
;483:		if( !g_fullservers && ( servernodeptr->numclients == servernodeptr->maxclients ) ) {
ADDRGP4 g_fullservers
INDIRI4
CNSTI4 0
NEI4 $278
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
NEI4 $278
line 484
;484:			continue;
ADDRGP4 $272
JUMPV
LABELV $278
line 487
;485:		}
;486:
;487:		switch( g_gametype ) {
ADDRLP4 40
ADDRGP4 g_gametype
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
LTI4 $280
ADDRLP4 40
INDIRI4
CNSTI4 4
GTI4 $280
ADDRLP4 40
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $295
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $295
address $281
address $283
address $286
address $289
address $292
code
line 489
;488:		case GAMES_ALL:
;489:			break;
LABELV $283
line 492
;490:
;491:		case GAMES_FFA:
;492:			if( servernodeptr->gametype != GT_FFA ) {
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
CNSTI4 0
EQI4 $281
line 493
;493:				continue;
ADDRGP4 $272
JUMPV
line 495
;494:			}
;495:			break;
LABELV $286
line 498
;496:
;497:		case GAMES_TEAMPLAY:
;498:			if( servernodeptr->gametype != GT_TEAM ) {
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
CNSTI4 3
EQI4 $281
line 499
;499:				continue;
ADDRGP4 $272
JUMPV
line 501
;500:			}
;501:			break;
LABELV $289
line 504
;502:
;503:		case GAMES_TOURNEY:
;504:			if( servernodeptr->gametype != GT_TOURNAMENT ) {
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
CNSTI4 1
EQI4 $281
line 505
;505:				continue;
ADDRGP4 $272
JUMPV
line 507
;506:			}
;507:			break;
LABELV $292
line 510
;508:
;509:		case GAMES_CTF:
;510:			if( servernodeptr->gametype != GT_CTF ) {
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRI4
CNSTI4 4
EQI4 $281
line 511
;511:				continue;
ADDRGP4 $272
JUMPV
line 513
;512:			}
;513:			break;
LABELV $280
LABELV $281
line 516
;514:		}
;515:
;516:		if( servernodeptr->pingtime < servernodeptr->minPing ) {
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
GEI4 $296
line 517
;517:			pingColor = S_COLOR_BLUE;
ADDRLP4 20
ADDRGP4 $298
ASGNP4
line 518
;518:		}
ADDRGP4 $297
JUMPV
LABELV $296
line 519
;519:		else if( servernodeptr->maxPing && servernodeptr->pingtime > servernodeptr->maxPing ) {
ADDRLP4 52
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $299
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
LEI4 $299
line 520
;520:			pingColor = S_COLOR_BLUE;
ADDRLP4 20
ADDRGP4 $298
ASGNP4
line 521
;521:		}
ADDRGP4 $300
JUMPV
LABELV $299
line 522
;522:		else if( servernodeptr->pingtime < 200 ) {
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRI4
CNSTI4 200
GEI4 $301
line 523
;523:			pingColor = S_COLOR_GREEN;
ADDRLP4 20
ADDRGP4 $303
ASGNP4
line 524
;524:		}
ADDRGP4 $302
JUMPV
LABELV $301
line 525
;525:		else if( servernodeptr->pingtime < 400 ) {
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRI4
CNSTI4 400
GEI4 $304
line 526
;526:			pingColor = S_COLOR_YELLOW;
ADDRLP4 20
ADDRGP4 $306
ASGNP4
line 527
;527:		}
ADDRGP4 $305
JUMPV
LABELV $304
line 528
;528:		else {
line 529
;529:			pingColor = S_COLOR_RED;
ADDRLP4 20
ADDRGP4 $307
ASGNP4
line 530
;530:		}
LABELV $305
LABELV $302
LABELV $300
LABELV $297
line 532
;531:
;532:		Com_sprintf( buff, MAX_LISTBOXWIDTH, "%-20.20s %-12.12s %2d/%2d %-8.8s %3s %s%3d " S_COLOR_YELLOW "%s", 
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 68
ARGI4
ADDRGP4 $308
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 netnames
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 0
EQI4 $312
ADDRLP4 56
ADDRGP4 $309
ASGNP4
ADDRGP4 $313
JUMPV
LABELV $312
ADDRLP4 56
ADDRGP4 $310
ASGNP4
LABELV $313
ADDRLP4 56
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 536
;533:			servernodeptr->hostname, servernodeptr->mapname, servernodeptr->numclients,
;534: 			servernodeptr->maxclients, servernodeptr->gamename,
;535:			netnames[servernodeptr->nettype], pingColor, servernodeptr->pingtime, servernodeptr->bPB ? "Yes" : "No" );
;536:		j++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 537
;537:	}
LABELV $272
line 473
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ASGNP4
LABELV $274
ADDRLP4 12
INDIRI4
ADDRLP4 24
INDIRI4
LTI4 $271
line 539
;538:
;539:	g_arenaservers.list.numitems = j;
ADDRGP4 g_arenaservers+776+68
ADDRLP4 8
INDIRI4
ASGNI4
line 540
;540:	g_arenaservers.list.curvalue = 0;
ADDRGP4 g_arenaservers+776+64
CNSTI4 0
ASGNI4
line 541
;541:	g_arenaservers.list.top      = 0;
ADDRGP4 g_arenaservers+776+72
CNSTI4 0
ASGNI4
line 544
;542:
;543:	// update picture
;544:	ArenaServers_UpdatePicture();
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
line 545
;545:}
LABELV $158
endproc ArenaServers_UpdateMenu 68 48
proc ArenaServers_Remove 24 12
line 554
;546:
;547:
;548:/*
;549:=================
;550:ArenaServers_Remove
;551:=================
;552:*/
;553:static void ArenaServers_Remove( void )
;554:{
line 559
;555:	int				i;
;556:	servernode_t*	servernodeptr;
;557:	table_t*		tableptr;
;558:
;559:	if (!g_arenaservers.list.numitems)
ADDRGP4 g_arenaservers+776+68
INDIRI4
CNSTI4 0
NEI4 $321
line 560
;560:		return;
ADDRGP4 $320
JUMPV
LABELV $321
line 566
;561:
;562:	// remove selected item from display list
;563:	// items are in scattered order due to sort and cull
;564:	// perform delete on list box contents, resync all lists
;565:
;566:	tableptr      = &g_arenaservers.table[g_arenaservers.list.curvalue];
ADDRLP4 8
CNSTI4 72
ADDRGP4 g_arenaservers+776+64
INDIRI4
MULI4
ADDRGP4 g_arenaservers+4072
ADDP4
ASGNP4
line 567
;567:	servernodeptr = tableptr->servernode;
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 68
ADDP4
INDIRP4
ASGNP4
line 570
;568:
;569:	// find address in master list
;570:	for (i=0; i<g_arenaservers.numfavoriteaddresses; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $331
JUMPV
LABELV $328
line 571
;571:		if (!Q_stricmp(g_arenaservers.favoriteaddresses[i],servernodeptr->adrstr))
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $333
line 572
;572:				break;
ADDRGP4 $330
JUMPV
LABELV $333
LABELV $329
line 570
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $331
ADDRLP4 0
INDIRI4
ADDRGP4 g_arenaservers+14856
INDIRI4
LTI4 $328
LABELV $330
line 575
;573:
;574:	// delete address from master list
;575:	if (i <= g_arenaservers.numfavoriteaddresses-1)
ADDRLP4 0
INDIRI4
ADDRGP4 g_arenaservers+14856
INDIRI4
CNSTI4 1
SUBI4
GTI4 $336
line 576
;576:	{
line 577
;577:		if (i < g_arenaservers.numfavoriteaddresses-1)
ADDRLP4 0
INDIRI4
ADDRGP4 g_arenaservers+14856
INDIRI4
CNSTI4 1
SUBI4
GEI4 $339
line 578
;578:		{
line 580
;579:			// shift items up
;580:			memcpy( &g_arenaservers.favoriteaddresses[i], &g_arenaservers.favoriteaddresses[i+1], (g_arenaservers.numfavoriteaddresses - i - 1)*sizeof(MAX_ADDRESSLENGTH));
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRLP4 20
INDIRI4
ADDRGP4 g_arenaservers+13832+64
ADDP4
ARGP4
ADDRGP4 g_arenaservers+14856
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 1
SUBI4
CVIU4 4
CNSTI4 2
LSHU4
CVUI4 4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 581
;581:		}
LABELV $339
line 582
;582:		g_arenaservers.numfavoriteaddresses--;
ADDRLP4 16
ADDRGP4 g_arenaservers+14856
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 583
;583:	}	
LABELV $336
line 586
;584:
;585:	// find address in server list
;586:	for (i=0; i<g_numfavoriteservers; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $350
JUMPV
LABELV $347
line 587
;587:		if (&g_favoriteserverlist[i] == servernodeptr)
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_favoriteserverlist
ADDP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
NEU4 $351
line 588
;588:				break;
ADDRGP4 $349
JUMPV
LABELV $351
LABELV $348
line 586
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $350
ADDRLP4 0
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
LTI4 $347
LABELV $349
line 591
;589:
;590:	// delete address from server list
;591:	if (i <= g_numfavoriteservers-1)
ADDRLP4 0
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
CNSTI4 1
SUBI4
GTI4 $353
line 592
;592:	{
line 593
;593:		if (i < g_numfavoriteservers-1)
ADDRLP4 0
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
CNSTI4 1
SUBI4
GEI4 $355
line 594
;594:		{
line 596
;595:			// shift items up
;596:			memcpy( &g_favoriteserverlist[i], &g_favoriteserverlist[i+1], (g_numfavoriteservers - i - 1)*sizeof(servernode_t));
ADDRLP4 20
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 g_favoriteserverlist
ADDP4
ARGP4
ADDRLP4 20
INDIRI4
ADDRGP4 g_favoriteserverlist+152
ADDP4
ARGP4
CNSTU4 152
ADDRGP4 g_numfavoriteservers
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
CNSTI4 1
SUBI4
CVIU4 4
MULU4
CVUI4 4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 597
;597:		}
LABELV $355
line 598
;598:		g_numfavoriteservers--;
ADDRLP4 16
ADDRGP4 g_numfavoriteservers
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 599
;599:	}	
LABELV $353
line 601
;600:
;601:	g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
ADDRGP4 g_arenaservers+13800
ADDRGP4 g_arenaservers+14856
INDIRI4
ASGNI4
line 602
;602:	g_arenaservers.currentping       = g_arenaservers.numfavoriteaddresses;
ADDRGP4 g_arenaservers+13812
ADDRGP4 g_arenaservers+14856
INDIRI4
ASGNI4
line 603
;603:}
LABELV $320
endproc ArenaServers_Remove 24 12
proc ArenaServers_Insert 84 12
line 612
;604:
;605:
;606:/*
;607:=================
;608:ArenaServers_Insert
;609:=================
;610:*/
;611:static void ArenaServers_Insert( char* adrstr, char* info, int pingtime )
;612:{
line 618
;613:	servernode_t*	servernodeptr;
;614:	char*			s;
;615:	int				i;
;616:
;617:
;618:	if ((pingtime >= ArenaServers_MaxPing()) && (g_servertype != AS_FAVORITES))
ADDRLP4 12
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LTI4 $363
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
EQI4 $363
line 619
;619:	{
line 621
;620:		// slow global or local servers do not get entered
;621:		return;
ADDRGP4 $362
JUMPV
LABELV $363
line 624
;622:	}
;623:
;624:	if (*g_arenaservers.numservers >= g_arenaservers.maxservers) {
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ADDRGP4 g_arenaservers+13824
INDIRI4
LTI4 $365
line 626
;625:		// list full;
;626:		servernodeptr = g_arenaservers.serverlist+(*g_arenaservers.numservers)-1;
ADDRLP4 0
CNSTI4 152
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
MULI4
ADDRGP4 g_arenaservers+13808
INDIRP4
ADDP4
CNSTI4 -152
ADDP4
ASGNP4
line 627
;627:	} else {
ADDRGP4 $366
JUMPV
LABELV $365
line 629
;628:		// next slot
;629:		servernodeptr = g_arenaservers.serverlist+(*g_arenaservers.numservers);
ADDRLP4 0
CNSTI4 152
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
MULI4
ADDRGP4 g_arenaservers+13808
INDIRP4
ADDP4
ASGNP4
line 630
;630:		(*g_arenaservers.numservers)++;
ADDRLP4 16
ADDRGP4 g_arenaservers+13804
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 631
;631:	}
LABELV $366
line 633
;632:
;633:	Q_strncpyz( servernodeptr->adrstr, adrstr, MAX_ADDRESSLENGTH );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 635
;634:
;635:	Q_strncpyz( servernodeptr->hostname, Info_ValueForKey( info, "hostname"), MAX_HOSTNAMELENGTH );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $374
ARGP4
ADDRLP4 16
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 636
;636:	Q_CleanStr( servernodeptr->hostname );
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 637
;637:	Q_strupr( servernodeptr->hostname );
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 639
;638:
;639:	Q_strncpyz( servernodeptr->mapname, Info_ValueForKey( info, "mapname"), MAX_MAPNAMELENGTH );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $375
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 89
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
line 640
;640:	Q_CleanStr( servernodeptr->mapname );
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 641
;641:	Q_strupr( servernodeptr->mapname );
ADDRLP4 0
INDIRP4
CNSTI4 89
ADDP4
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 643
;642:
;643:	servernodeptr->numclients = atoi( Info_ValueForKey( info, "clients") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $376
ARGP4
ADDRLP4 24
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 644
;644:	servernodeptr->maxclients = atoi( Info_ValueForKey( info, "sv_maxclients") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $377
ARGP4
ADDRLP4 32
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 645
;645:	servernodeptr->pingtime   = pingtime;
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 646
;646:	servernodeptr->minPing    = atoi( Info_ValueForKey( info, "minPing") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $378
ARGP4
ADDRLP4 40
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 647
;647:	servernodeptr->maxPing    = atoi( Info_ValueForKey( info, "maxPing") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $379
ARGP4
ADDRLP4 48
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
line 648
;648:	servernodeptr->bPB = atoi( Info_ValueForKey( info, "punkbuster") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 56
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRLP4 60
INDIRI4
ASGNI4
line 666
;649:
;650:	/*
;651:	s = Info_ValueForKey( info, "nettype" );
;652:	for (i=0; ;i++)
;653:	{
;654:		if (!netnames[i])
;655:		{
;656:			servernodeptr->nettype = 0;
;657:			break;
;658:		}
;659:		else if (!Q_stricmp( netnames[i], s ))
;660:		{
;661:			servernodeptr->nettype = i;
;662:			break;
;663:		}
;664:	}
;665:	*/
;666:	servernodeptr->nettype = atoi(Info_ValueForKey(info, "nettype"));
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $381
ARGP4
ADDRLP4 64
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 136
ADDP4
ADDRLP4 68
INDIRI4
ASGNI4
line 668
;667:
;668:	s = Info_ValueForKey( info, "game");
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $382
ARGP4
ADDRLP4 72
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 72
INDIRP4
ASGNP4
line 669
;669:	i = atoi( Info_ValueForKey( info, "gametype") );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $383
ARGP4
ADDRLP4 76
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 80
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 80
INDIRI4
ASGNI4
line 670
;670:	if( i < 0 ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $384
line 671
;671:		i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 672
;672:	}
ADDRGP4 $385
JUMPV
LABELV $384
line 673
;673:	else if( i > 11 ) {
ADDRLP4 4
INDIRI4
CNSTI4 11
LEI4 $386
line 674
;674:		i = 12;
ADDRLP4 4
CNSTI4 12
ASGNI4
line 675
;675:	}
LABELV $386
LABELV $385
line 676
;676:	if( *s ) {
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $388
line 677
;677:		servernodeptr->gametype = i;//-1;
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 678
;678:		Q_strncpyz( servernodeptr->gamename, s, sizeof(servernodeptr->gamename) );
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 12
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 679
;679:	}
ADDRGP4 $389
JUMPV
LABELV $388
line 680
;680:	else {
line 681
;681:		servernodeptr->gametype = i;
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 682
;682:		Q_strncpyz( servernodeptr->gamename, gamenames[i], sizeof(servernodeptr->gamename) );
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gamenames
ADDP4
INDIRP4
ARGP4
CNSTI4 12
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 683
;683:	}
LABELV $389
line 684
;684:}
LABELV $362
endproc ArenaServers_Insert 84 12
export ArenaServers_InsertFavorites
proc ArenaServers_InsertFavorites 1040 12
line 695
;685:
;686:
;687:/*
;688:=================
;689:ArenaServers_InsertFavorites
;690:
;691:Insert nonresponsive address book entries into display lists.
;692:=================
;693:*/
;694:void ArenaServers_InsertFavorites( void )
;695:{
line 701
;696:	int		i;
;697:	int		j;
;698:	char	info[MAX_INFO_STRING];
;699:
;700:	// resync existing results with new or deleted cvars
;701:	info[0] = '\0';
ADDRLP4 8
CNSTI1 0
ASGNI1
line 702
;702:	Info_SetValueForKey( info, "hostname", "No Response" );
ADDRLP4 8
ARGP4
ADDRGP4 $374
ARGP4
ADDRGP4 $391
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 703
;703:	for (i=0; i<g_arenaservers.numfavoriteaddresses; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $395
JUMPV
LABELV $392
line 704
;704:	{
line 706
;705:		// find favorite address in refresh list
;706:		for (j=0; j<g_numfavoriteservers; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $400
JUMPV
LABELV $397
line 707
;707:			if (!Q_stricmp(g_arenaservers.favoriteaddresses[i],g_favoriteserverlist[j].adrstr))
ADDRLP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_favoriteserverlist
ADDP4
ARGP4
ADDRLP4 1032
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $401
line 708
;708:				break;
ADDRGP4 $399
JUMPV
LABELV $401
LABELV $398
line 706
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $400
ADDRLP4 0
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
LTI4 $397
LABELV $399
line 710
;709:
;710:		if ( j >= g_numfavoriteservers)
ADDRLP4 0
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
LTI4 $404
line 711
;711:		{
line 713
;712:			// not in list, add it
;713:			ArenaServers_Insert( g_arenaservers.favoriteaddresses[i], info, ArenaServers_MaxPing() );
ADDRLP4 1036
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 1036
INDIRI4
ARGI4
ADDRGP4 ArenaServers_Insert
CALLV
pop
line 714
;714:		}
LABELV $404
line 715
;715:	}
LABELV $393
line 703
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $395
ADDRLP4 4
INDIRI4
ADDRGP4 g_arenaservers+14856
INDIRI4
LTI4 $392
line 716
;716:}
LABELV $390
endproc ArenaServers_InsertFavorites 1040 12
export ArenaServers_LoadFavorites
proc ArenaServers_LoadFavorites 3552 12
line 727
;717:
;718:
;719:/*
;720:=================
;721:ArenaServers_LoadFavorites
;722:
;723:Load cvar address book entries into local lists.
;724:=================
;725:*/
;726:void ArenaServers_LoadFavorites( void )
;727:{
line 736
;728:	int				i;
;729:	int				j;
;730:	int				numtempitems;
;731:	char			emptyinfo[MAX_INFO_STRING];
;732:	char			adrstr[MAX_ADDRESSLENGTH];
;733:	servernode_t	templist[MAX_FAVORITESERVERS];
;734:	qboolean		found;
;735:
;736:	found        = qfalse;
ADDRLP4 2508
CNSTI4 0
ASGNI4
line 737
;737:	emptyinfo[0] = '\0';
ADDRLP4 2512
CNSTI1 0
ASGNI1
line 740
;738:
;739:	// copy the old
;740:	memcpy( templist, g_favoriteserverlist, sizeof(servernode_t)*MAX_FAVORITESERVERS );
ADDRLP4 72
ARGP4
ADDRGP4 g_favoriteserverlist
ARGP4
CNSTI4 2432
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 741
;741:	numtempitems = g_numfavoriteservers;
ADDRLP4 68
ADDRGP4 g_numfavoriteservers
INDIRI4
ASGNI4
line 744
;742:
;743:	// clear the current for sync
;744:	memset( g_favoriteserverlist, 0, sizeof(servernode_t)*MAX_FAVORITESERVERS );
ADDRGP4 g_favoriteserverlist
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2432
ARGI4
ADDRGP4 memset
CALLP4
pop
line 745
;745:	g_numfavoriteservers = 0;
ADDRGP4 g_numfavoriteservers
CNSTI4 0
ASGNI4
line 748
;746:
;747:	// resync existing results with new or deleted cvars
;748:	for (i=0; i<MAX_FAVORITESERVERS; i++)
ADDRLP4 2504
CNSTI4 0
ASGNI4
LABELV $408
line 749
;749:	{
line 750
;750:		trap_Cvar_VariableStringBuffer( va("server%d",i+1), adrstr, MAX_ADDRESSLENGTH );
ADDRGP4 $412
ARGP4
ADDRLP4 2504
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 3536
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 3536
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 751
;751:		if (!adrstr[0])
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $413
line 752
;752:			continue;
ADDRGP4 $409
JUMPV
LABELV $413
line 756
;753:
;754:		// quick sanity check to avoid slow domain name resolving
;755:		// first character must be numeric
;756:		if (adrstr[0] < '0' || adrstr[0] > '9')
ADDRLP4 3540
ADDRLP4 4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 3540
INDIRI4
CNSTI4 48
LTI4 $417
ADDRLP4 3540
INDIRI4
CNSTI4 57
LEI4 $415
LABELV $417
line 757
;757:			continue;
ADDRGP4 $409
JUMPV
LABELV $415
line 762
;758:
;759:		// favorite server addresses must be maintained outside refresh list
;760:		// this mimics local and global netadr's stored in client
;761:		// these can be fetched to fill ping list
;762:		strcpy( g_arenaservers.favoriteaddresses[g_numfavoriteservers], adrstr );
ADDRGP4 g_numfavoriteservers
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 765
;763:
;764:		// find this server in the old list
;765:		for (j=0; j<numtempitems; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $422
JUMPV
LABELV $419
line 766
;766:			if (!Q_stricmp( templist[j].adrstr, adrstr ))
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 72
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 3544
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 3544
INDIRI4
CNSTI4 0
NEI4 $423
line 767
;767:				break;
ADDRGP4 $421
JUMPV
LABELV $423
LABELV $420
line 765
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $422
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
LTI4 $419
LABELV $421
line 769
;768:
;769:		if (j < numtempitems)
ADDRLP4 0
INDIRI4
ADDRLP4 68
INDIRI4
GEI4 $425
line 770
;770:		{
line 772
;771:			// found server - add exisiting results
;772:			memcpy( &g_favoriteserverlist[g_numfavoriteservers], &templist[j], sizeof(servernode_t) );
ADDRLP4 3548
CNSTI4 152
ASGNI4
ADDRLP4 3548
INDIRI4
ADDRGP4 g_numfavoriteservers
INDIRI4
MULI4
ADDRGP4 g_favoriteserverlist
ADDP4
ARGP4
ADDRLP4 3548
INDIRI4
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 72
ADDP4
ARGP4
ADDRLP4 3548
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 773
;773:			found = qtrue;
ADDRLP4 2508
CNSTI4 1
ASGNI4
line 774
;774:		}
ADDRGP4 $426
JUMPV
LABELV $425
line 776
;775:		else
;776:		{
line 778
;777:			// add new server
;778:			Q_strncpyz( g_favoriteserverlist[g_numfavoriteservers].adrstr, adrstr, MAX_ADDRESSLENGTH );
CNSTI4 152
ADDRGP4 g_numfavoriteservers
INDIRI4
MULI4
ADDRGP4 g_favoriteserverlist
ADDP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 779
;779:			g_favoriteserverlist[g_numfavoriteservers].pingtime = ArenaServers_MaxPing();
ADDRLP4 3548
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
CNSTI4 152
ADDRGP4 g_numfavoriteservers
INDIRI4
MULI4
ADDRGP4 g_favoriteserverlist+116
ADDP4
ADDRLP4 3548
INDIRI4
ASGNI4
line 780
;780:		}
LABELV $426
line 782
;781:
;782:		g_numfavoriteservers++;
ADDRLP4 3548
ADDRGP4 g_numfavoriteservers
ASGNP4
ADDRLP4 3548
INDIRP4
ADDRLP4 3548
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 783
;783:	}
LABELV $409
line 748
ADDRLP4 2504
ADDRLP4 2504
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 2504
INDIRI4
CNSTI4 16
LTI4 $408
line 785
;784:
;785:	g_arenaservers.numfavoriteaddresses = g_numfavoriteservers;
ADDRGP4 g_arenaservers+14856
ADDRGP4 g_numfavoriteservers
INDIRI4
ASGNI4
line 787
;786:
;787:	if (!found)
ADDRLP4 2508
INDIRI4
CNSTI4 0
NEI4 $429
line 788
;788:	{
line 791
;789:		// no results were found, reset server list
;790:		// list will be automatically refreshed when selected
;791:		g_numfavoriteservers = 0;
ADDRGP4 g_numfavoriteservers
CNSTI4 0
ASGNI4
line 792
;792:	}
LABELV $429
line 793
;793:}
LABELV $407
endproc ArenaServers_LoadFavorites 3552 12
proc ArenaServers_StopRefresh 0 16
line 802
;794:
;795:
;796:/*
;797:=================
;798:ArenaServers_StopRefresh
;799:=================
;800:*/
;801:static void ArenaServers_StopRefresh( void )
;802:{
line 803
;803:	if (!g_arenaservers.refreshservers)
ADDRGP4 g_arenaservers+13816
INDIRI4
CNSTI4 0
NEI4 $432
line 805
;804:		// not currently refreshing
;805:		return;
ADDRGP4 $431
JUMPV
LABELV $432
line 807
;806:
;807:	g_arenaservers.refreshservers = qfalse;
ADDRGP4 g_arenaservers+13816
CNSTI4 0
ASGNI4
line 809
;808:
;809:	if (g_servertype == AS_FAVORITES)
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
NEI4 $436
line 810
;810:	{
line 812
;811:		// nonresponsive favorites must be shown
;812:		ArenaServers_InsertFavorites();
ADDRGP4 ArenaServers_InsertFavorites
CALLV
pop
line 813
;813:	}
LABELV $436
line 816
;814:
;815:	// final tally
;816:	if (g_arenaservers.numqueriedservers >= 0)
ADDRGP4 g_arenaservers+13800
INDIRI4
CNSTI4 0
LTI4 $438
line 817
;817:	{
line 818
;818:		g_arenaservers.currentping       = *g_arenaservers.numservers;
ADDRGP4 g_arenaservers+13812
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ASGNI4
line 819
;819:		g_arenaservers.numqueriedservers = *g_arenaservers.numservers; 
ADDRGP4 g_arenaservers+13800
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ASGNI4
line 820
;820:	}
LABELV $438
line 823
;821:	
;822:	// sort
;823:	qsort( g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof( servernode_t ), ArenaServers_Compare);
ADDRGP4 g_arenaservers+13808
INDIRP4
ARGP4
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 ArenaServers_Compare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 825
;824:
;825:	ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 826
;826:}
LABELV $431
endproc ArenaServers_StopRefresh 0 16
proc ArenaServers_DoRefresh 1120 16
line 835
;827:
;828:
;829:/*
;830:=================
;831:ArenaServers_DoRefresh
;832:=================
;833:*/
;834:static void ArenaServers_DoRefresh( void )
;835:{
line 843
;836:	int		i;
;837:	int		j;
;838:	int		time;
;839:	int		maxPing;
;840:	char	adrstr[MAX_ADDRESSLENGTH];
;841:	char	info[MAX_INFO_STRING];
;842:
;843:	if (uis.realtime < g_arenaservers.refreshtime)
ADDRGP4 uis+4
INDIRI4
ADDRGP4 g_arenaservers+13828
INDIRI4
GEI4 $448
line 844
;844:	{
line 845
;845:	  if (g_servertype != AS_FAVORITES) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
EQI4 $452
line 846
;846:			if (g_servertype == AS_LOCAL) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 0
NEI4 $454
line 847
;847:				if (!trap_LAN_GetServerCount(g_servertype)) {
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1104
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
NEI4 $456
line 848
;848:					return;
ADDRGP4 $447
JUMPV
LABELV $456
line 850
;849:				}
;850:			}
LABELV $454
line 851
;851:			if (trap_LAN_GetServerCount(g_servertype) < 0) {
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1104
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRLP4 1104
INDIRI4
CNSTI4 0
GEI4 $458
line 853
;852:			  // still waiting for response
;853:			  return;
ADDRGP4 $447
JUMPV
LABELV $458
line 855
;854:			}
;855:	  }
LABELV $452
line 856
;856:	}
LABELV $448
line 858
;857:
;858:	if (uis.realtime < g_arenaservers.nextpingtime)
ADDRGP4 uis+4
INDIRI4
ADDRGP4 g_arenaservers+13820
INDIRI4
GEI4 $460
line 859
;859:	{
line 861
;860:		// wait for time trigger
;861:		return;
ADDRGP4 $447
JUMPV
LABELV $460
line 865
;862:	}
;863:
;864:	// trigger at 10Hz intervals
;865:	g_arenaservers.nextpingtime = uis.realtime + 10;
ADDRGP4 g_arenaservers+13820
ADDRGP4 uis+4
INDIRI4
CNSTI4 10
ADDI4
ASGNI4
line 868
;866:
;867:	// process ping results
;868:	maxPing = ArenaServers_MaxPing();
ADDRLP4 1104
ADDRGP4 ArenaServers_MaxPing
CALLI4
ASGNI4
ADDRLP4 76
ADDRLP4 1104
INDIRI4
ASGNI4
line 869
;869:	for (i=0; i<MAX_PINGREQUESTS; i++)
ADDRLP4 68
CNSTI4 0
ASGNI4
LABELV $466
line 870
;870:	{
line 871
;871:		trap_LAN_GetPing( i, adrstr, MAX_ADDRESSLENGTH, &time );
ADDRLP4 68
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 72
ARGP4
ADDRGP4 trap_LAN_GetPing
CALLV
pop
line 872
;872:		if (!adrstr[0])
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $470
line 873
;873:		{
line 875
;874:			// ignore empty or pending pings
;875:			continue;
ADDRGP4 $467
JUMPV
LABELV $470
line 879
;876:		}
;877:
;878:		// find ping result in our local list
;879:		for (j=0; j<MAX_PINGREQUESTS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $472
line 880
;880:			if (!Q_stricmp( adrstr, g_arenaservers.pinglist[j].adrstr ))
ADDRLP4 4
ARGP4
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896
ADDP4
ARGP4
ADDRLP4 1108
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $476
line 881
;881:				break;
ADDRGP4 $474
JUMPV
LABELV $476
LABELV $473
line 879
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $472
LABELV $474
line 883
;882:
;883:		if (j < MAX_PINGREQUESTS)
ADDRLP4 0
INDIRI4
CNSTI4 32
GEI4 $479
line 884
;884:		{
line 886
;885:			// found it
;886:			if (!time)
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $481
line 887
;887:			{
line 888
;888:				time = uis.realtime - g_arenaservers.pinglist[j].start;
ADDRLP4 72
ADDRGP4 uis+4
INDIRI4
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896+64
ADDP4
INDIRI4
SUBI4
ASGNI4
line 889
;889:				if (time < maxPing)
ADDRLP4 72
INDIRI4
ADDRLP4 76
INDIRI4
GEI4 $486
line 890
;890:				{
line 892
;891:					// still waiting
;892:					continue;
ADDRGP4 $467
JUMPV
LABELV $486
line 894
;893:				}
;894:			}
LABELV $481
line 896
;895:
;896:			if (time > maxPing)
ADDRLP4 72
INDIRI4
ADDRLP4 76
INDIRI4
LEI4 $488
line 897
;897:			{
line 899
;898:				// stale it out
;899:				info[0] = '\0';
ADDRLP4 80
CNSTI1 0
ASGNI1
line 900
;900:				time    = maxPing;
ADDRLP4 72
ADDRLP4 76
INDIRI4
ASGNI4
line 901
;901:			}
ADDRGP4 $489
JUMPV
LABELV $488
line 903
;902:			else
;903:			{
line 904
;904:				trap_LAN_GetPingInfo( i, info, MAX_INFO_STRING );
ADDRLP4 68
INDIRI4
ARGI4
ADDRLP4 80
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_LAN_GetPingInfo
CALLV
pop
line 905
;905:			}
LABELV $489
line 908
;906:
;907:			// insert ping results
;908:			ArenaServers_Insert( adrstr, info, time );
ADDRLP4 4
ARGP4
ADDRLP4 80
ARGP4
ADDRLP4 72
INDIRI4
ARGI4
ADDRGP4 ArenaServers_Insert
CALLV
pop
line 911
;909:
;910:			// clear this query from internal list
;911:			g_arenaservers.pinglist[j].adrstr[0] = '\0';
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896
ADDP4
CNSTI1 0
ASGNI1
line 912
;912:   		}
LABELV $479
line 915
;913:
;914:		// clear this query from external list
;915:		trap_LAN_ClearPing( i );
ADDRLP4 68
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ClearPing
CALLV
pop
line 916
;916:	}
LABELV $467
line 869
ADDRLP4 68
ADDRLP4 68
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 32
LTI4 $466
line 920
;917:
;918:	// get results of servers query
;919:	// counts can increase as servers respond
;920:	if (g_servertype == AS_FAVORITES) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
NEI4 $491
line 921
;921:	  g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
ADDRGP4 g_arenaservers+13800
ADDRGP4 g_arenaservers+14856
INDIRI4
ASGNI4
line 922
;922:	} else {
ADDRGP4 $492
JUMPV
LABELV $491
line 923
;923:	  g_arenaservers.numqueriedservers = trap_LAN_GetServerCount(g_servertype);
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRLP4 1108
ADDRGP4 trap_LAN_GetServerCount
CALLI4
ASGNI4
ADDRGP4 g_arenaservers+13800
ADDRLP4 1108
INDIRI4
ASGNI4
line 924
;924:	}
LABELV $492
line 931
;925:
;926://	if (g_arenaservers.numqueriedservers > g_arenaservers.maxservers)
;927://		g_arenaservers.numqueriedservers = g_arenaservers.maxservers;
;928:
;929:	// send ping requests in reasonable bursts
;930:	// iterate ping through all found servers
;931:	for (i=0; i<MAX_PINGREQUESTS && g_arenaservers.currentping < g_arenaservers.numqueriedservers; i++)
ADDRLP4 68
CNSTI4 0
ASGNI4
ADDRGP4 $499
JUMPV
LABELV $496
line 932
;932:	{
line 933
;933:		if (trap_LAN_GetPingQueueCount() >= MAX_PINGREQUESTS)
ADDRLP4 1108
ADDRGP4 trap_LAN_GetPingQueueCount
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 32
LTI4 $502
line 934
;934:		{
line 936
;935:			// ping queue is full
;936:			break;
ADDRGP4 $498
JUMPV
LABELV $502
line 940
;937:		}
;938:
;939:		// find empty slot
;940:		for (j=0; j<MAX_PINGREQUESTS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $504
line 941
;941:			if (!g_arenaservers.pinglist[j].adrstr[0])
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $508
line 942
;942:				break;
ADDRGP4 $506
JUMPV
LABELV $508
LABELV $505
line 940
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $504
LABELV $506
line 944
;943:
;944:		if (j >= MAX_PINGREQUESTS)
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $511
line 946
;945:			// no empty slots available yet - wait for timeout
;946:			break;
ADDRGP4 $498
JUMPV
LABELV $511
line 950
;947:
;948:		// get an address to ping
;949:
;950:		if (g_servertype == AS_FAVORITES) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
NEI4 $513
line 951
;951:		  strcpy( adrstr, g_arenaservers.favoriteaddresses[g_arenaservers.currentping] ); 		
ADDRLP4 4
ARGP4
ADDRGP4 g_arenaservers+13812
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 952
;952:		} else {
ADDRGP4 $514
JUMPV
LABELV $513
line 953
;953:		  trap_LAN_GetServerAddressString(g_servertype, g_arenaservers.currentping, adrstr, MAX_ADDRESSLENGTH );
ADDRGP4 g_servertype
INDIRI4
ARGI4
ADDRGP4 g_arenaservers+13812
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 trap_LAN_GetServerAddressString
CALLV
pop
line 954
;954:		}
LABELV $514
line 956
;955:
;956:		strcpy( g_arenaservers.pinglist[j].adrstr, adrstr );
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 957
;957:		g_arenaservers.pinglist[j].start = uis.realtime;
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896+64
ADDP4
ADDRGP4 uis+4
INDIRI4
ASGNI4
line 959
;958:
;959:		trap_Cmd_ExecuteText( EXEC_NOW, va( "ping %s\n", adrstr )  );
ADDRGP4 $522
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1112
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 0
ARGI4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 962
;960:		
;961:		// advance to next server
;962:		g_arenaservers.currentping++;
ADDRLP4 1116
ADDRGP4 g_arenaservers+13812
ASGNP4
ADDRLP4 1116
INDIRP4
ADDRLP4 1116
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 963
;963:	}
LABELV $497
line 931
ADDRLP4 68
ADDRLP4 68
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $499
ADDRLP4 68
INDIRI4
CNSTI4 32
GEI4 $524
ADDRGP4 g_arenaservers+13812
INDIRI4
ADDRGP4 g_arenaservers+13800
INDIRI4
LTI4 $496
LABELV $524
LABELV $498
line 965
;964:
;965:	if (!trap_LAN_GetPingQueueCount())
ADDRLP4 1108
ADDRGP4 trap_LAN_GetPingQueueCount
CALLI4
ASGNI4
ADDRLP4 1108
INDIRI4
CNSTI4 0
NEI4 $525
line 966
;966:	{
line 968
;967:		// all pings completed
;968:		ArenaServers_StopRefresh();
ADDRGP4 ArenaServers_StopRefresh
CALLV
pop
line 969
;969:		return;
ADDRGP4 $447
JUMPV
LABELV $525
line 973
;970:	}
;971:
;972:	// update the user interface with ping status
;973:	ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 974
;974:}
LABELV $447
endproc ArenaServers_DoRefresh 1120 16
proc ArenaServers_StartRefresh 88 16
line 983
;975:
;976:
;977:/*
;978:=================
;979:ArenaServers_StartRefresh
;980:=================
;981:*/
;982:static void ArenaServers_StartRefresh( void )
;983:{
line 987
;984:	int		i;
;985:	char	myargs[32], protocol[32];
;986:
;987:	memset( g_arenaservers.serverlist, 0, g_arenaservers.maxservers*sizeof(table_t) );
ADDRGP4 g_arenaservers+13808
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTU4 72
ADDRGP4 g_arenaservers+13824
INDIRI4
CVIU4 4
MULU4
CVUI4 4
ARGI4
ADDRGP4 memset
CALLP4
pop
line 989
;988:
;989:	for (i=0; i<MAX_PINGREQUESTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $530
line 990
;990:	{
line 991
;991:		g_arenaservers.pinglist[i].adrstr[0] = '\0';
CNSTI4 68
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+1896
ADDP4
CNSTI1 0
ASGNI1
line 992
;992:		trap_LAN_ClearPing( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 trap_LAN_ClearPing
CALLV
pop
line 993
;993:	}
LABELV $531
line 989
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $530
line 995
;994:
;995:	g_arenaservers.refreshservers    = qtrue;
ADDRGP4 g_arenaservers+13816
CNSTI4 1
ASGNI4
line 996
;996:	g_arenaservers.currentping       = 0;
ADDRGP4 g_arenaservers+13812
CNSTI4 0
ASGNI4
line 997
;997:	g_arenaservers.nextpingtime      = 0;
ADDRGP4 g_arenaservers+13820
CNSTI4 0
ASGNI4
line 998
;998:	*g_arenaservers.numservers       = 0;
ADDRGP4 g_arenaservers+13804
INDIRP4
CNSTI4 0
ASGNI4
line 999
;999:	g_arenaservers.numqueriedservers = 0;
ADDRGP4 g_arenaservers+13800
CNSTI4 0
ASGNI4
line 1002
;1000:
;1001:	// allow max 5 seconds for responses
;1002:	g_arenaservers.refreshtime = uis.realtime + 5000;
ADDRGP4 g_arenaservers+13828
ADDRGP4 uis+4
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 1005
;1003:
;1004:	// place menu in zeroed state
;1005:	ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1007
;1006:
;1007:	if( g_servertype == AS_LOCAL ) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 0
NEI4 $542
line 1008
;1008:		trap_Cmd_ExecuteText( EXEC_APPEND, "localservers\n" );
CNSTI4 2
ARGI4
ADDRGP4 $544
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 1009
;1009:		return;
ADDRGP4 $527
JUMPV
LABELV $542
line 1012
;1010:	}
;1011:
;1012:	if( g_servertype == AS_GLOBAL || g_servertype == AS_MPLAYER ) {
ADDRLP4 68
ADDRGP4 g_servertype
INDIRI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 2
EQI4 $547
ADDRLP4 68
INDIRI4
CNSTI4 1
NEI4 $545
LABELV $547
line 1013
;1013:		if( g_servertype == AS_GLOBAL ) {
ADDRGP4 g_servertype
INDIRI4
CNSTI4 2
NEI4 $548
line 1014
;1014:			i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1015
;1015:		}
ADDRGP4 $549
JUMPV
LABELV $548
line 1016
;1016:		else {
line 1017
;1017:			i = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1018
;1018:		}
LABELV $549
line 1020
;1019:
;1020:		switch( g_arenaservers.gametype.curvalue ) {
ADDRLP4 72
ADDRGP4 g_arenaservers+456+64
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
LTI4 $550
ADDRLP4 72
INDIRI4
CNSTI4 4
GTI4 $550
ADDRLP4 72
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $563
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $563
address $554
address $555
address $557
address $559
address $561
code
LABELV $550
LABELV $554
line 1023
;1021:		default:
;1022:		case GAMES_ALL:
;1023:			myargs[0] = 0;
ADDRLP4 36
CNSTI1 0
ASGNI1
line 1024
;1024:			break;
ADDRGP4 $551
JUMPV
LABELV $555
line 1027
;1025:
;1026:		case GAMES_FFA:
;1027:			strcpy( myargs, " ffa" );
ADDRLP4 36
ARGP4
ADDRGP4 $556
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1028
;1028:			break;
ADDRGP4 $551
JUMPV
LABELV $557
line 1031
;1029:
;1030:		case GAMES_TEAMPLAY:
;1031:			strcpy( myargs, " team" );
ADDRLP4 36
ARGP4
ADDRGP4 $558
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1032
;1032:			break;
ADDRGP4 $551
JUMPV
LABELV $559
line 1035
;1033:
;1034:		case GAMES_TOURNEY:
;1035:			strcpy( myargs, " tourney" );
ADDRLP4 36
ARGP4
ADDRGP4 $560
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1036
;1036:			break;
ADDRGP4 $551
JUMPV
LABELV $561
line 1039
;1037:
;1038:		case GAMES_CTF:
;1039:			strcpy( myargs, " ctf" );
ADDRLP4 36
ARGP4
ADDRGP4 $562
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1040
;1040:			break;
LABELV $551
line 1044
;1041:		}
;1042:
;1043:
;1044:		if (g_emptyservers) {
ADDRGP4 g_emptyservers
INDIRI4
CNSTI4 0
EQI4 $564
line 1045
;1045:			strcat(myargs, " empty");
ADDRLP4 36
ARGP4
ADDRGP4 $566
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1046
;1046:		}
LABELV $564
line 1048
;1047:
;1048:		if (g_fullservers) {
ADDRGP4 g_fullservers
INDIRI4
CNSTI4 0
EQI4 $567
line 1049
;1049:			strcat(myargs, " full");
ADDRLP4 36
ARGP4
ADDRGP4 $569
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1050
;1050:		}
LABELV $567
line 1052
;1051:
;1052:		protocol[0] = '\0';
ADDRLP4 4
CNSTI1 0
ASGNI1
line 1053
;1053:		trap_Cvar_VariableStringBuffer( "debug_protocol", protocol, sizeof(protocol) );
ADDRGP4 $570
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 1054
;1054:		if (strlen(protocol)) {
ADDRLP4 4
ARGP4
ADDRLP4 76
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
EQI4 $571
line 1055
;1055:			trap_Cmd_ExecuteText( EXEC_APPEND, va( "globalservers %d %s%s\n", i, protocol, myargs ));
ADDRGP4 $573
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 80
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 80
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 1056
;1056:		}
ADDRGP4 $572
JUMPV
LABELV $571
line 1057
;1057:		else {
line 1058
;1058:			trap_Cmd_ExecuteText( EXEC_APPEND, va( "globalservers %d %d%s\n", i, (int)trap_Cvar_VariableValue( "protocol" ), myargs ) );
ADDRGP4 $575
ARGP4
ADDRLP4 80
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRGP4 $574
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 80
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 36
ARGP4
ADDRLP4 84
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 84
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 1059
;1059:		}
LABELV $572
line 1060
;1060:	}
LABELV $545
line 1061
;1061:}
LABELV $527
endproc ArenaServers_StartRefresh 88 16
export ArenaServers_SaveChanges
proc ArenaServers_SaveChanges 12 8
line 1070
;1062:
;1063:
;1064:/*
;1065:=================
;1066:ArenaServers_SaveChanges
;1067:=================
;1068:*/
;1069:void ArenaServers_SaveChanges( void )
;1070:{
line 1073
;1071:	int	i;
;1072:
;1073:	for (i=0; i<g_arenaservers.numfavoriteaddresses; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $580
JUMPV
LABELV $577
line 1074
;1074:		trap_Cvar_Set( va("server%d",i+1), g_arenaservers.favoriteaddresses[i] );
ADDRGP4 $412
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 g_arenaservers+13832
ADDP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
LABELV $578
line 1073
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $580
ADDRLP4 0
INDIRI4
ADDRGP4 g_arenaservers+14856
INDIRI4
LTI4 $577
line 1076
;1075:
;1076:	for (; i<MAX_FAVORITESERVERS; i++)
ADDRGP4 $586
JUMPV
LABELV $583
line 1077
;1077:		trap_Cvar_Set( va("server%d",i+1), "" );
ADDRGP4 $412
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $202
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
LABELV $584
line 1076
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $586
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $583
line 1078
;1078:}
LABELV $576
endproc ArenaServers_SaveChanges 12 8
export ArenaServers_Sort
proc ArenaServers_Sort 0 16
line 1086
;1079:
;1080:
;1081:/*
;1082:=================
;1083:ArenaServers_Sort
;1084:=================
;1085:*/
;1086:void ArenaServers_Sort( int type ) {
line 1087
;1087:	if( g_sortkey == type ) {
ADDRGP4 g_sortkey
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $588
line 1088
;1088:		return;
ADDRGP4 $587
JUMPV
LABELV $588
line 1091
;1089:	}
;1090:
;1091:	g_sortkey = type;
ADDRGP4 g_sortkey
ADDRFP4 0
INDIRI4
ASGNI4
line 1092
;1092:	qsort( g_arenaservers.serverlist, *g_arenaservers.numservers, sizeof( servernode_t ), ArenaServers_Compare);
ADDRGP4 g_arenaservers+13808
INDIRP4
ARGP4
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 ArenaServers_Compare
ARGP4
ADDRGP4 qsort
CALLV
pop
line 1093
;1093:}
LABELV $587
endproc ArenaServers_Sort 0 16
export ArenaServers_SetType
proc ArenaServers_SetType 20 8
line 1102
;1094:
;1095:
;1096:/*
;1097:=================
;1098:ArenaServers_SetType
;1099:=================
;1100:*/
;1101:void ArenaServers_SetType( int type )
;1102:{
line 1103
;1103:	if (g_servertype == type)
ADDRGP4 g_servertype
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $593
line 1104
;1104:		return;
ADDRGP4 $592
JUMPV
LABELV $593
line 1106
;1105:
;1106:	g_servertype = type;
ADDRGP4 g_servertype
ADDRFP4 0
INDIRI4
ASGNI4
line 1108
;1107:
;1108:	switch( type ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $595
ADDRLP4 0
INDIRI4
CNSTI4 3
GTI4 $595
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $621
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $621
address $597
address $615
address $603
address $609
code
LABELV $595
LABELV $597
line 1111
;1109:	default:
;1110:	case AS_LOCAL:
;1111:		g_arenaservers.remove.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 4
ADDRGP4 g_arenaservers+1368+44
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 1112
;1112:		g_arenaservers.serverlist = g_localserverlist;
ADDRGP4 g_arenaservers+13808
ADDRGP4 g_localserverlist
ASGNP4
line 1113
;1113:		g_arenaservers.numservers = &g_numlocalservers;
ADDRGP4 g_arenaservers+13804
ADDRGP4 g_numlocalservers
ASGNP4
line 1114
;1114:		g_arenaservers.maxservers = MAX_LOCALSERVERS;
ADDRGP4 g_arenaservers+13824
CNSTI4 128
ASGNI4
line 1115
;1115:		break;
ADDRGP4 $596
JUMPV
LABELV $603
line 1118
;1116:
;1117:	case AS_GLOBAL:
;1118:		g_arenaservers.remove.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 8
ADDRGP4 g_arenaservers+1368+44
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 1119
;1119:		g_arenaservers.serverlist = g_globalserverlist;
ADDRGP4 g_arenaservers+13808
ADDRGP4 g_globalserverlist
ASGNP4
line 1120
;1120:		g_arenaservers.numservers = &g_numglobalservers;
ADDRGP4 g_arenaservers+13804
ADDRGP4 g_numglobalservers
ASGNP4
line 1121
;1121:		g_arenaservers.maxservers = MAX_GLOBALSERVERS;
ADDRGP4 g_arenaservers+13824
CNSTI4 128
ASGNI4
line 1122
;1122:		break;
ADDRGP4 $596
JUMPV
LABELV $609
line 1125
;1123:
;1124:	case AS_FAVORITES:
;1125:		g_arenaservers.remove.generic.flags &= ~(QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 12
ADDRGP4 g_arenaservers+1368+44
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRU4
CNSTU4 4294946815
BANDU4
ASGNU4
line 1126
;1126:		g_arenaservers.serverlist = g_favoriteserverlist;
ADDRGP4 g_arenaservers+13808
ADDRGP4 g_favoriteserverlist
ASGNP4
line 1127
;1127:		g_arenaservers.numservers = &g_numfavoriteservers;
ADDRGP4 g_arenaservers+13804
ADDRGP4 g_numfavoriteservers
ASGNP4
line 1128
;1128:		g_arenaservers.maxservers = MAX_FAVORITESERVERS;
ADDRGP4 g_arenaservers+13824
CNSTI4 16
ASGNI4
line 1129
;1129:		break;
ADDRGP4 $596
JUMPV
LABELV $615
line 1132
;1130:
;1131:	case AS_MPLAYER:
;1132:		g_arenaservers.remove.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
ADDRLP4 16
ADDRGP4 g_arenaservers+1368+44
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRU4
CNSTU4 20480
BORU4
ASGNU4
line 1133
;1133:		g_arenaservers.serverlist = g_mplayerserverlist;
ADDRGP4 g_arenaservers+13808
ADDRGP4 g_mplayerserverlist
ASGNP4
line 1134
;1134:		g_arenaservers.numservers = &g_nummplayerservers;
ADDRGP4 g_arenaservers+13804
ADDRGP4 g_nummplayerservers
ASGNP4
line 1135
;1135:		g_arenaservers.maxservers = MAX_GLOBALSERVERS;
ADDRGP4 g_arenaservers+13824
CNSTI4 128
ASGNI4
line 1136
;1136:		break;
LABELV $596
line 1140
;1137:		
;1138:	}
;1139:
;1140:	if( !*g_arenaservers.numservers ) {
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $622
line 1141
;1141:		ArenaServers_StartRefresh();
ADDRGP4 ArenaServers_StartRefresh
CALLV
pop
line 1142
;1142:	}
ADDRGP4 $623
JUMPV
LABELV $622
line 1143
;1143:	else {
line 1145
;1144:		// avoid slow operation, use existing results
;1145:		g_arenaservers.currentping       = *g_arenaservers.numservers;
ADDRGP4 g_arenaservers+13812
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ASGNI4
line 1146
;1146:		g_arenaservers.numqueriedservers = *g_arenaservers.numservers; 
ADDRGP4 g_arenaservers+13800
ADDRGP4 g_arenaservers+13804
INDIRP4
INDIRI4
ASGNI4
line 1147
;1147:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1148
;1148:	}
LABELV $623
line 1149
;1149:	strcpy(g_arenaservers.status.string,"hit refresh to update");
ADDRGP4 g_arenaservers+1224+60
INDIRP4
ARGP4
ADDRGP4 $631
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1150
;1150:}
LABELV $592
endproc ArenaServers_SetType 20 8
proc Punkbuster_ConfirmEnable 8 12
line 1157
;1151:
;1152:/*
;1153:=================
;1154:PunkBuster_Confirm
;1155:=================
;1156:*/
;1157:static void Punkbuster_ConfirmEnable( qboolean result ) {
line 1158
;1158:	if (result)
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $633
line 1159
;1159:	{		
line 1160
;1160:		trap_SetPbClStatus(1);
CNSTI4 1
ARGI4
ADDRGP4 trap_SetPbClStatus
CALLV
pop
line 1161
;1161:	}
LABELV $633
line 1162
;1162:	g_arenaservers.punkbuster.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "cl_punkbuster" ) );
ADDRGP4 $637
ARGP4
ADDRLP4 0
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_arenaservers+14860+64
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 1163
;1163:}
LABELV $632
endproc Punkbuster_ConfirmEnable 8 12
proc Punkbuster_ConfirmDisable 8 12
line 1165
;1164:
;1165:static void Punkbuster_ConfirmDisable( qboolean result ) {
line 1166
;1166:	if (result)
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $639
line 1167
;1167:	{
line 1168
;1168:		trap_SetPbClStatus(0);
CNSTI4 0
ARGI4
ADDRGP4 trap_SetPbClStatus
CALLV
pop
line 1169
;1169:		UI_Message( punkbuster_msg );
ADDRGP4 punkbuster_msg
ARGP4
ADDRGP4 UI_Message
CALLV
pop
line 1170
;1170:	}
LABELV $639
line 1171
;1171:	g_arenaservers.punkbuster.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "cl_punkbuster" ) );
ADDRGP4 $637
ARGP4
ADDRLP4 0
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_arenaservers+14860+64
ADDRLP4 4
INDIRF4
CVFI4 4
ASGNI4
line 1172
;1172:}
LABELV $638
endproc Punkbuster_ConfirmDisable 8 12
proc ArenaServers_Event 12 16
line 1179
;1173:
;1174:/*
;1175:=================
;1176:ArenaServers_Event
;1177:=================
;1178:*/
;1179:static void ArenaServers_Event( void* ptr, int event ) {
line 1183
;1180:	int		id;
;1181:	int value;
;1182:
;1183:	id = ((menucommon_s*)ptr)->id;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ASGNI4
line 1185
;1184:
;1185:	if( event != QM_ACTIVATED && id != ID_LIST ) {
ADDRFP4 4
INDIRI4
CNSTI4 3
EQI4 $644
ADDRLP4 0
INDIRI4
CNSTI4 15
EQI4 $644
line 1186
;1186:		return;
ADDRGP4 $643
JUMPV
LABELV $644
line 1189
;1187:	}
;1188:
;1189:	switch( id ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
LTI4 $646
ADDRLP4 0
INDIRI4
CNSTI4 24
GTI4 $646
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $698-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $698
address $648
address $654
address $660
address $666
address $672
address $678
address $681
address $683
address $685
address $686
address $687
address $688
address $689
address $690
address $691
code
LABELV $648
line 1191
;1190:	case ID_MASTER:
;1191:		value = g_arenaservers.master.curvalue;
ADDRLP4 4
ADDRGP4 g_arenaservers+360+64
INDIRI4
ASGNI4
line 1192
;1192:		if (value >= 1)
ADDRLP4 4
INDIRI4
CNSTI4 1
LTI4 $651
line 1193
;1193:		{
line 1194
;1194:			value++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1195
;1195:		}
LABELV $651
line 1196
;1196:		trap_Cvar_SetValue( "ui_browserMaster", value );
ADDRGP4 $653
ARGP4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 1197
;1197:		ArenaServers_SetType( value );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 ArenaServers_SetType
CALLV
pop
line 1198
;1198:		break;
ADDRGP4 $647
JUMPV
LABELV $654
line 1201
;1199:
;1200:	case ID_GAMETYPE:
;1201:		trap_Cvar_SetValue( "ui_browserGameType", g_arenaservers.gametype.curvalue );
ADDRGP4 $655
ARGP4
ADDRGP4 g_arenaservers+456+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 1202
;1202:		g_gametype = g_arenaservers.gametype.curvalue;
ADDRGP4 g_gametype
ADDRGP4 g_arenaservers+456+64
INDIRI4
ASGNI4
line 1203
;1203:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1204
;1204:		break;
ADDRGP4 $647
JUMPV
LABELV $660
line 1207
;1205:
;1206:	case ID_SORTKEY:
;1207:		trap_Cvar_SetValue( "ui_browserSortKey", g_arenaservers.sortkey.curvalue );
ADDRGP4 $661
ARGP4
ADDRGP4 g_arenaservers+552+64
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 1208
;1208:		ArenaServers_Sort( g_arenaservers.sortkey.curvalue );
ADDRGP4 g_arenaservers+552+64
INDIRI4
ARGI4
ADDRGP4 ArenaServers_Sort
CALLV
pop
line 1209
;1209:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1210
;1210:		break;
ADDRGP4 $647
JUMPV
LABELV $666
line 1213
;1211:
;1212:	case ID_SHOW_FULL:
;1213:		trap_Cvar_SetValue( "ui_browserShowFull", g_arenaservers.showfull.curvalue );
ADDRGP4 $667
ARGP4
ADDRGP4 g_arenaservers+648+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 1214
;1214:		g_fullservers = g_arenaservers.showfull.curvalue;
ADDRGP4 g_fullservers
ADDRGP4 g_arenaservers+648+60
INDIRI4
ASGNI4
line 1215
;1215:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1216
;1216:		break;
ADDRGP4 $647
JUMPV
LABELV $672
line 1219
;1217:
;1218:	case ID_SHOW_EMPTY:
;1219:		trap_Cvar_SetValue( "ui_browserShowEmpty", g_arenaservers.showempty.curvalue );
ADDRGP4 $673
ARGP4
ADDRGP4 g_arenaservers+712+60
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 trap_Cvar_SetValue
CALLV
pop
line 1220
;1220:		g_emptyservers = g_arenaservers.showempty.curvalue;
ADDRGP4 g_emptyservers
ADDRGP4 g_arenaservers+712+60
INDIRI4
ASGNI4
line 1221
;1221:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1222
;1222:		break;
ADDRGP4 $647
JUMPV
LABELV $678
line 1225
;1223:
;1224:	case ID_LIST:
;1225:		if( event == QM_GOTFOCUS ) {
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $647
line 1226
;1226:			ArenaServers_UpdatePicture();
ADDRGP4 ArenaServers_UpdatePicture
CALLV
pop
line 1227
;1227:		}
line 1228
;1228:		break;
ADDRGP4 $647
JUMPV
LABELV $681
line 1231
;1229:
;1230:	case ID_SCROLL_UP:
;1231:		ScrollList_Key( &g_arenaservers.list, K_UPARROW );
ADDRGP4 g_arenaservers+776
ARGP4
CNSTI4 132
ARGI4
ADDRGP4 ScrollList_Key
CALLI4
pop
line 1232
;1232:		break;
ADDRGP4 $647
JUMPV
LABELV $683
line 1235
;1233:
;1234:	case ID_SCROLL_DOWN:
;1235:		ScrollList_Key( &g_arenaservers.list, K_DOWNARROW );
ADDRGP4 g_arenaservers+776
ARGP4
CNSTI4 133
ARGI4
ADDRGP4 ScrollList_Key
CALLI4
pop
line 1236
;1236:		break;
ADDRGP4 $647
JUMPV
LABELV $685
line 1239
;1237:
;1238:	case ID_BACK:
;1239:		ArenaServers_StopRefresh();
ADDRGP4 ArenaServers_StopRefresh
CALLV
pop
line 1240
;1240:		ArenaServers_SaveChanges();
ADDRGP4 ArenaServers_SaveChanges
CALLV
pop
line 1241
;1241:		UI_PopMenu();
ADDRGP4 UI_PopMenu
CALLV
pop
line 1242
;1242:		break;
ADDRGP4 $647
JUMPV
LABELV $686
line 1245
;1243:
;1244:	case ID_REFRESH:
;1245:		ArenaServers_StartRefresh();
ADDRGP4 ArenaServers_StartRefresh
CALLV
pop
line 1246
;1246:		break;
ADDRGP4 $647
JUMPV
LABELV $687
line 1249
;1247:
;1248:	case ID_SPECIFY:
;1249:		UI_SpecifyServerMenu();
ADDRGP4 UI_SpecifyServerMenu
CALLV
pop
line 1250
;1250:		break;
ADDRGP4 $647
JUMPV
LABELV $688
line 1253
;1251:
;1252:	case ID_CREATE:
;1253:		UI_StartServerMenu( qtrue );
CNSTI4 1
ARGI4
ADDRGP4 UI_StartServerMenu
CALLV
pop
line 1254
;1254:		break;
ADDRGP4 $647
JUMPV
LABELV $689
line 1257
;1255:
;1256:	case ID_CONNECT:
;1257:		ArenaServers_Go();
ADDRGP4 ArenaServers_Go
CALLV
pop
line 1258
;1258:		break;
ADDRGP4 $647
JUMPV
LABELV $690
line 1261
;1259:
;1260:	case ID_REMOVE:
;1261:		ArenaServers_Remove();
ADDRGP4 ArenaServers_Remove
CALLV
pop
line 1262
;1262:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1263
;1263:		break;
ADDRGP4 $647
JUMPV
LABELV $691
line 1266
;1264:	
;1265:	case ID_PUNKBUSTER:
;1266:		if (g_arenaservers.punkbuster.curvalue)			
ADDRGP4 g_arenaservers+14860+64
INDIRI4
CNSTI4 0
EQI4 $692
line 1267
;1267:		{
line 1268
;1268:			UI_ConfirmMenu_Style( "Enable Punkbuster?",  UI_CENTER|UI_INVERSE|UI_SMALLFONT, (voidfunc_f)NULL, Punkbuster_ConfirmEnable );
ADDRGP4 $696
ARGP4
CNSTI4 8209
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Punkbuster_ConfirmEnable
ARGP4
ADDRGP4 UI_ConfirmMenu_Style
CALLV
pop
line 1269
;1269:		}
ADDRGP4 $647
JUMPV
LABELV $692
line 1271
;1270:		else
;1271:		{
line 1272
;1272:			UI_ConfirmMenu_Style( "Disable Punkbuster?", UI_CENTER|UI_INVERSE|UI_SMALLFONT, (voidfunc_f)NULL, Punkbuster_ConfirmDisable );
ADDRGP4 $697
ARGP4
CNSTI4 8209
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Punkbuster_ConfirmDisable
ARGP4
ADDRGP4 UI_ConfirmMenu_Style
CALLV
pop
line 1273
;1273:		}
line 1274
;1274:		break;
LABELV $646
LABELV $647
line 1276
;1275:	}
;1276:}
LABELV $643
endproc ArenaServers_Event 12 16
proc ArenaServers_MenuDraw 0 4
line 1285
;1277:
;1278:
;1279:/*
;1280:=================
;1281:ArenaServers_MenuDraw
;1282:=================
;1283:*/
;1284:static void ArenaServers_MenuDraw( void )
;1285:{
line 1286
;1286:	if (g_arenaservers.refreshservers)
ADDRGP4 g_arenaservers+13816
INDIRI4
CNSTI4 0
EQI4 $701
line 1287
;1287:		ArenaServers_DoRefresh();
ADDRGP4 ArenaServers_DoRefresh
CALLV
pop
LABELV $701
line 1289
;1288:
;1289:	Menu_Draw( &g_arenaservers.menu );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 Menu_Draw
CALLV
pop
line 1290
;1290:}
LABELV $700
endproc ArenaServers_MenuDraw 0 4
proc ArenaServers_MenuKey 16 8
line 1298
;1291:
;1292:
;1293:/*
;1294:=================
;1295:ArenaServers_MenuKey
;1296:=================
;1297:*/
;1298:static sfxHandle_t ArenaServers_MenuKey( int key ) {
line 1299
;1299:	if( key == K_SPACE  && g_arenaservers.refreshservers ) {
ADDRFP4 0
INDIRI4
CNSTI4 32
NEI4 $705
ADDRGP4 g_arenaservers+13816
INDIRI4
CNSTI4 0
EQI4 $705
line 1300
;1300:		ArenaServers_StopRefresh();	
ADDRGP4 ArenaServers_StopRefresh
CALLV
pop
line 1301
;1301:		return menu_move_sound;
ADDRGP4 menu_move_sound
INDIRI4
RETI4
ADDRGP4 $704
JUMPV
LABELV $705
line 1304
;1302:	}
;1303:
;1304:	if( ( key == K_DEL || key == K_KP_DEL ) && ( g_servertype == AS_FAVORITES ) &&
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 140
EQI4 $711
ADDRLP4 0
INDIRI4
CNSTI4 171
NEI4 $708
LABELV $711
ADDRGP4 g_servertype
INDIRI4
CNSTI4 3
NEI4 $708
ADDRGP4 g_arenaservers
ARGP4
ADDRLP4 4
ADDRGP4 Menu_ItemAtCursor
CALLP4
ASGNP4
ADDRGP4 g_arenaservers+776
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
NEU4 $708
line 1305
;1305:		( Menu_ItemAtCursor( &g_arenaservers.menu) == &g_arenaservers.list ) ) {
line 1306
;1306:		ArenaServers_Remove();
ADDRGP4 ArenaServers_Remove
CALLV
pop
line 1307
;1307:		ArenaServers_UpdateMenu();
ADDRGP4 ArenaServers_UpdateMenu
CALLV
pop
line 1308
;1308:		return menu_move_sound;
ADDRGP4 menu_move_sound
INDIRI4
RETI4
ADDRGP4 $704
JUMPV
LABELV $708
line 1311
;1309:	}
;1310:
;1311:	if( key == K_MOUSE2 || key == K_ESCAPE ) {
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 179
EQI4 $714
ADDRLP4 8
INDIRI4
CNSTI4 27
NEI4 $712
LABELV $714
line 1312
;1312:		ArenaServers_StopRefresh();
ADDRGP4 ArenaServers_StopRefresh
CALLV
pop
line 1313
;1313:		ArenaServers_SaveChanges();
ADDRGP4 ArenaServers_SaveChanges
CALLV
pop
line 1314
;1314:	}
LABELV $712
line 1317
;1315:
;1316:
;1317:	return Menu_DefaultKey( &g_arenaservers.menu, key );
ADDRGP4 g_arenaservers
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 Menu_DefaultKey
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
LABELV $704
endproc ArenaServers_MenuKey 16 8
bss
align 1
LABELV $716
skip 64
code
proc ArenaServers_MenuInit 44 16
line 1326
;1318:}
;1319:
;1320:
;1321:/*
;1322:=================
;1323:ArenaServers_MenuInit
;1324:=================
;1325:*/
;1326:static void ArenaServers_MenuInit( void ) {
line 1334
;1327:	int			i;
;1328:	int			type;
;1329:	int			y;
;1330:	int			value;
;1331:	static char	statusbuffer[MAX_STATUSLENGTH];
;1332:
;1333:	// zero set all our globals
;1334:	memset( &g_arenaservers, 0 ,sizeof(arenaservers_t) );
ADDRGP4 g_arenaservers
ARGP4
CNSTI4 0
ARGI4
CNSTI4 15044
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1336
;1335:
;1336:	ArenaServers_Cache();
ADDRGP4 ArenaServers_Cache
CALLV
pop
line 1338
;1337:
;1338:	g_arenaservers.menu.fullscreen = qtrue;
ADDRGP4 g_arenaservers+280
CNSTI4 1
ASGNI4
line 1339
;1339:	g_arenaservers.menu.wrapAround = qtrue;
ADDRGP4 g_arenaservers+276
CNSTI4 1
ASGNI4
line 1340
;1340:	g_arenaservers.menu.draw       = ArenaServers_MenuDraw;
ADDRGP4 g_arenaservers+268
ADDRGP4 ArenaServers_MenuDraw
ASGNP4
line 1341
;1341:	g_arenaservers.menu.key        = ArenaServers_MenuKey;
ADDRGP4 g_arenaservers+272
ADDRGP4 ArenaServers_MenuKey
ASGNP4
line 1343
;1342:
;1343:	g_arenaservers.banner.generic.type  = MTYPE_BTEXT;
ADDRGP4 g_arenaservers+288
CNSTI4 10
ASGNI4
line 1344
;1344:	g_arenaservers.banner.generic.flags = QMF_CENTER_JUSTIFY;
ADDRGP4 g_arenaservers+288+44
CNSTU4 8
ASGNU4
line 1345
;1345:	g_arenaservers.banner.generic.x	    = 320;
ADDRGP4 g_arenaservers+288+12
CNSTI4 320
ASGNI4
line 1346
;1346:	g_arenaservers.banner.generic.y	    = 16;
ADDRGP4 g_arenaservers+288+16
CNSTI4 16
ASGNI4
line 1347
;1347:	g_arenaservers.banner.string  		= "ARENA SERVERS";
ADDRGP4 g_arenaservers+288+60
ADDRGP4 $730
ASGNP4
line 1348
;1348:	g_arenaservers.banner.style  	    = UI_CENTER;
ADDRGP4 g_arenaservers+288+64
CNSTI4 1
ASGNI4
line 1349
;1349:	g_arenaservers.banner.color  	    = color_white;
ADDRGP4 g_arenaservers+288+68
ADDRGP4 color_white
ASGNP4
line 1351
;1350:
;1351:	y = 80;
ADDRLP4 4
CNSTI4 80
ASGNI4
line 1352
;1352:	g_arenaservers.master.generic.type			= MTYPE_SPINCONTROL;
ADDRGP4 g_arenaservers+360
CNSTI4 3
ASGNI4
line 1353
;1353:	g_arenaservers.master.generic.name			= "Servers:";
ADDRGP4 g_arenaservers+360+4
ADDRGP4 $738
ASGNP4
line 1354
;1354:	g_arenaservers.master.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+360+44
CNSTU4 258
ASGNU4
line 1355
;1355:	g_arenaservers.master.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+360+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1356
;1356:	g_arenaservers.master.generic.id			= ID_MASTER;
ADDRGP4 g_arenaservers+360+8
CNSTI4 10
ASGNI4
line 1357
;1357:	g_arenaservers.master.generic.x				= 320;
ADDRGP4 g_arenaservers+360+12
CNSTI4 320
ASGNI4
line 1358
;1358:	g_arenaservers.master.generic.y				= y;
ADDRGP4 g_arenaservers+360+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1359
;1359:	g_arenaservers.master.itemnames				= master_items;
ADDRGP4 g_arenaservers+360+76
ADDRGP4 master_items
ASGNP4
line 1361
;1360:
;1361:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1362
;1362:	g_arenaservers.gametype.generic.type		= MTYPE_SPINCONTROL;
ADDRGP4 g_arenaservers+456
CNSTI4 3
ASGNI4
line 1363
;1363:	g_arenaservers.gametype.generic.name		= "Game Type:";
ADDRGP4 g_arenaservers+456+4
ADDRGP4 $754
ASGNP4
line 1364
;1364:	g_arenaservers.gametype.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+456+44
CNSTU4 258
ASGNU4
line 1365
;1365:	g_arenaservers.gametype.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+456+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1366
;1366:	g_arenaservers.gametype.generic.id			= ID_GAMETYPE;
ADDRGP4 g_arenaservers+456+8
CNSTI4 11
ASGNI4
line 1367
;1367:	g_arenaservers.gametype.generic.x			= 320;
ADDRGP4 g_arenaservers+456+12
CNSTI4 320
ASGNI4
line 1368
;1368:	g_arenaservers.gametype.generic.y			= y;
ADDRGP4 g_arenaservers+456+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1369
;1369:	g_arenaservers.gametype.itemnames			= servertype_items;
ADDRGP4 g_arenaservers+456+76
ADDRGP4 servertype_items
ASGNP4
line 1371
;1370:
;1371:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1372
;1372:	g_arenaservers.sortkey.generic.type			= MTYPE_SPINCONTROL;
ADDRGP4 g_arenaservers+552
CNSTI4 3
ASGNI4
line 1373
;1373:	g_arenaservers.sortkey.generic.name			= "Sort By:";
ADDRGP4 g_arenaservers+552+4
ADDRGP4 $770
ASGNP4
line 1374
;1374:	g_arenaservers.sortkey.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+552+44
CNSTU4 258
ASGNU4
line 1375
;1375:	g_arenaservers.sortkey.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+552+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1376
;1376:	g_arenaservers.sortkey.generic.id			= ID_SORTKEY;
ADDRGP4 g_arenaservers+552+8
CNSTI4 12
ASGNI4
line 1377
;1377:	g_arenaservers.sortkey.generic.x			= 320;
ADDRGP4 g_arenaservers+552+12
CNSTI4 320
ASGNI4
line 1378
;1378:	g_arenaservers.sortkey.generic.y			= y;
ADDRGP4 g_arenaservers+552+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1379
;1379:	g_arenaservers.sortkey.itemnames			= sortkey_items;
ADDRGP4 g_arenaservers+552+76
ADDRGP4 sortkey_items
ASGNP4
line 1381
;1380:
;1381:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1382
;1382:	g_arenaservers.showfull.generic.type		= MTYPE_RADIOBUTTON;
ADDRGP4 g_arenaservers+648
CNSTI4 5
ASGNI4
line 1383
;1383:	g_arenaservers.showfull.generic.name		= "Show Full:";
ADDRGP4 g_arenaservers+648+4
ADDRGP4 $786
ASGNP4
line 1384
;1384:	g_arenaservers.showfull.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+648+44
CNSTU4 258
ASGNU4
line 1385
;1385:	g_arenaservers.showfull.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+648+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1386
;1386:	g_arenaservers.showfull.generic.id			= ID_SHOW_FULL;
ADDRGP4 g_arenaservers+648+8
CNSTI4 13
ASGNI4
line 1387
;1387:	g_arenaservers.showfull.generic.x			= 320;
ADDRGP4 g_arenaservers+648+12
CNSTI4 320
ASGNI4
line 1388
;1388:	g_arenaservers.showfull.generic.y			= y;
ADDRGP4 g_arenaservers+648+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1390
;1389:
;1390:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1391
;1391:	g_arenaservers.showempty.generic.type		= MTYPE_RADIOBUTTON;
ADDRGP4 g_arenaservers+712
CNSTI4 5
ASGNI4
line 1392
;1392:	g_arenaservers.showempty.generic.name		= "Show Empty:";
ADDRGP4 g_arenaservers+712+4
ADDRGP4 $800
ASGNP4
line 1393
;1393:	g_arenaservers.showempty.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+712+44
CNSTU4 258
ASGNU4
line 1394
;1394:	g_arenaservers.showempty.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+712+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1395
;1395:	g_arenaservers.showempty.generic.id			= ID_SHOW_EMPTY;
ADDRGP4 g_arenaservers+712+8
CNSTI4 14
ASGNI4
line 1396
;1396:	g_arenaservers.showempty.generic.x			= 320;
ADDRGP4 g_arenaservers+712+12
CNSTI4 320
ASGNI4
line 1397
;1397:	g_arenaservers.showempty.generic.y			= y;
ADDRGP4 g_arenaservers+712+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1399
;1398:
;1399:	y += 3 * SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 48
ADDI4
ASGNI4
line 1400
;1400:	g_arenaservers.list.generic.type			= MTYPE_SCROLLLIST;
ADDRGP4 g_arenaservers+776
CNSTI4 8
ASGNI4
line 1401
;1401:	g_arenaservers.list.generic.flags			= QMF_HIGHLIGHT_IF_FOCUS;
ADDRGP4 g_arenaservers+776+44
CNSTU4 128
ASGNU4
line 1402
;1402:	g_arenaservers.list.generic.id				= ID_LIST;
ADDRGP4 g_arenaservers+776+8
CNSTI4 15
ASGNI4
line 1403
;1403:	g_arenaservers.list.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+776+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1404
;1404:	g_arenaservers.list.generic.x				= 72;
ADDRGP4 g_arenaservers+776+12
CNSTI4 72
ASGNI4
line 1405
;1405:	g_arenaservers.list.generic.y				= y;
ADDRGP4 g_arenaservers+776+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1406
;1406:	g_arenaservers.list.width					= MAX_LISTBOXWIDTH;
ADDRGP4 g_arenaservers+776+80
CNSTI4 68
ASGNI4
line 1407
;1407:	g_arenaservers.list.height					= 11;
ADDRGP4 g_arenaservers+776+84
CNSTI4 11
ASGNI4
line 1408
;1408:	g_arenaservers.list.itemnames				= (const char **)g_arenaservers.items;
ADDRGP4 g_arenaservers+776+76
ADDRGP4 g_arenaservers+13288
ASGNP4
line 1409
;1409:	for( i = 0; i < MAX_LISTBOXITEMS; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $829
line 1410
;1410:		g_arenaservers.items[i] = g_arenaservers.table[i].buff;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaservers+13288
ADDP4
CNSTI4 72
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_arenaservers+4072
ADDP4
ASGNP4
line 1411
;1411:	}
LABELV $830
line 1409
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 128
LTI4 $829
line 1413
;1412:
;1413:	g_arenaservers.mappic.generic.type			= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+872
CNSTI4 6
ASGNI4
line 1414
;1414:	g_arenaservers.mappic.generic.flags			= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 g_arenaservers+872+44
CNSTU4 16388
ASGNU4
line 1415
;1415:	g_arenaservers.mappic.generic.x				= 72;
ADDRGP4 g_arenaservers+872+12
CNSTI4 72
ASGNI4
line 1416
;1416:	g_arenaservers.mappic.generic.y				= 80;
ADDRGP4 g_arenaservers+872+16
CNSTI4 80
ASGNI4
line 1417
;1417:	g_arenaservers.mappic.width					= 128;
ADDRGP4 g_arenaservers+872+76
CNSTI4 128
ASGNI4
line 1418
;1418:	g_arenaservers.mappic.height				= 96;
ADDRGP4 g_arenaservers+872+80
CNSTI4 96
ASGNI4
line 1419
;1419:	g_arenaservers.mappic.errorpic				= ART_UNKNOWNMAP;
ADDRGP4 g_arenaservers+872+64
ADDRGP4 $848
ASGNP4
line 1421
;1420:
;1421:	g_arenaservers.arrows.generic.type			= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+960
CNSTI4 6
ASGNI4
line 1422
;1422:	g_arenaservers.arrows.generic.name			= ART_ARROWS0;
ADDRGP4 g_arenaservers+960+4
ADDRGP4 $852
ASGNP4
line 1423
;1423:	g_arenaservers.arrows.generic.flags			= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 g_arenaservers+960+44
CNSTU4 16388
ASGNU4
line 1424
;1424:	g_arenaservers.arrows.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+960+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1425
;1425:	g_arenaservers.arrows.generic.x				= 512+48;
ADDRGP4 g_arenaservers+960+12
CNSTI4 560
ASGNI4
line 1426
;1426:	g_arenaservers.arrows.generic.y				= 240-64+16;
ADDRGP4 g_arenaservers+960+16
CNSTI4 192
ASGNI4
line 1427
;1427:	g_arenaservers.arrows.width					= 64;
ADDRGP4 g_arenaservers+960+76
CNSTI4 64
ASGNI4
line 1428
;1428:	g_arenaservers.arrows.height				= 128;
ADDRGP4 g_arenaservers+960+80
CNSTI4 128
ASGNI4
line 1430
;1429:
;1430:	g_arenaservers.up.generic.type				= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1048
CNSTI4 6
ASGNI4
line 1431
;1431:	g_arenaservers.up.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
ADDRGP4 g_arenaservers+1048+44
CNSTU4 2308
ASGNU4
line 1432
;1432:	g_arenaservers.up.generic.callback			= ArenaServers_Event;
ADDRGP4 g_arenaservers+1048+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1433
;1433:	g_arenaservers.up.generic.id				= ID_SCROLL_UP;
ADDRGP4 g_arenaservers+1048+8
CNSTI4 16
ASGNI4
line 1434
;1434:	g_arenaservers.up.generic.x					= 512+48;
ADDRGP4 g_arenaservers+1048+12
CNSTI4 560
ASGNI4
line 1435
;1435:	g_arenaservers.up.generic.y					= 240-64+16;
ADDRGP4 g_arenaservers+1048+16
CNSTI4 192
ASGNI4
line 1436
;1436:	g_arenaservers.up.width						= 64;
ADDRGP4 g_arenaservers+1048+76
CNSTI4 64
ASGNI4
line 1437
;1437:	g_arenaservers.up.height					= 64;
ADDRGP4 g_arenaservers+1048+80
CNSTI4 64
ASGNI4
line 1438
;1438:	g_arenaservers.up.focuspic					= ART_ARROWS_UP;
ADDRGP4 g_arenaservers+1048+60
ADDRGP4 $882
ASGNP4
line 1440
;1439:
;1440:	g_arenaservers.down.generic.type			= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1136
CNSTI4 6
ASGNI4
line 1441
;1441:	g_arenaservers.down.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
ADDRGP4 g_arenaservers+1136+44
CNSTU4 2308
ASGNU4
line 1442
;1442:	g_arenaservers.down.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+1136+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1443
;1443:	g_arenaservers.down.generic.id				= ID_SCROLL_DOWN;
ADDRGP4 g_arenaservers+1136+8
CNSTI4 17
ASGNI4
line 1444
;1444:	g_arenaservers.down.generic.x				= 512+48;
ADDRGP4 g_arenaservers+1136+12
CNSTI4 560
ASGNI4
line 1445
;1445:	g_arenaservers.down.generic.y				= 240+16;
ADDRGP4 g_arenaservers+1136+16
CNSTI4 256
ASGNI4
line 1446
;1446:	g_arenaservers.down.width					= 64;
ADDRGP4 g_arenaservers+1136+76
CNSTI4 64
ASGNI4
line 1447
;1447:	g_arenaservers.down.height					= 64;
ADDRGP4 g_arenaservers+1136+80
CNSTI4 64
ASGNI4
line 1448
;1448:	g_arenaservers.down.focuspic				= ART_ARROWS_DOWN;
ADDRGP4 g_arenaservers+1136+60
ADDRGP4 $900
ASGNP4
line 1450
;1449:
;1450:	y = 376;
ADDRLP4 4
CNSTI4 376
ASGNI4
line 1451
;1451:	g_arenaservers.status.generic.type		= MTYPE_TEXT;
ADDRGP4 g_arenaservers+1224
CNSTI4 7
ASGNI4
line 1452
;1452:	g_arenaservers.status.generic.x			= 320;
ADDRGP4 g_arenaservers+1224+12
CNSTI4 320
ASGNI4
line 1453
;1453:	g_arenaservers.status.generic.y			= y;
ADDRGP4 g_arenaservers+1224+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1454
;1454:	g_arenaservers.status.string			= statusbuffer;
ADDRGP4 g_arenaservers+1224+60
ADDRGP4 $716
ASGNP4
line 1455
;1455:	g_arenaservers.status.style				= UI_CENTER|UI_SMALLFONT;
ADDRGP4 g_arenaservers+1224+64
CNSTI4 17
ASGNI4
line 1456
;1456:	g_arenaservers.status.color				= menu_text_color;
ADDRGP4 g_arenaservers+1224+68
ADDRGP4 menu_text_color
ASGNP4
line 1458
;1457:
;1458:	y += SMALLCHAR_HEIGHT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 1459
;1459:	g_arenaservers.statusbar.generic.type   = MTYPE_TEXT;
ADDRGP4 g_arenaservers+1296
CNSTI4 7
ASGNI4
line 1460
;1460:	g_arenaservers.statusbar.generic.x	    = 320;
ADDRGP4 g_arenaservers+1296+12
CNSTI4 320
ASGNI4
line 1461
;1461:	g_arenaservers.statusbar.generic.y	    = y;
ADDRGP4 g_arenaservers+1296+16
ADDRLP4 4
INDIRI4
ASGNI4
line 1462
;1462:	g_arenaservers.statusbar.string	        = "";
ADDRGP4 g_arenaservers+1296+60
ADDRGP4 $202
ASGNP4
line 1463
;1463:	g_arenaservers.statusbar.style	        = UI_CENTER|UI_SMALLFONT;
ADDRGP4 g_arenaservers+1296+64
CNSTI4 17
ASGNI4
line 1464
;1464:	g_arenaservers.statusbar.color	        = text_color_normal;
ADDRGP4 g_arenaservers+1296+68
ADDRGP4 text_color_normal
ASGNP4
line 1466
;1465:
;1466:	g_arenaservers.remove.generic.type		= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1368
CNSTI4 6
ASGNI4
line 1467
;1467:	g_arenaservers.remove.generic.name		= ART_REMOVE0;
ADDRGP4 g_arenaservers+1368+4
ADDRGP4 $926
ASGNP4
line 1468
;1468:	g_arenaservers.remove.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1368+44
CNSTU4 260
ASGNU4
line 1469
;1469:	g_arenaservers.remove.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+1368+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1470
;1470:	g_arenaservers.remove.generic.id		= ID_REMOVE;
ADDRGP4 g_arenaservers+1368+8
CNSTI4 23
ASGNI4
line 1471
;1471:	g_arenaservers.remove.generic.x			= 450;
ADDRGP4 g_arenaservers+1368+12
CNSTI4 450
ASGNI4
line 1472
;1472:	g_arenaservers.remove.generic.y			= 86;
ADDRGP4 g_arenaservers+1368+16
CNSTI4 86
ASGNI4
line 1473
;1473:	g_arenaservers.remove.width				= 96;
ADDRGP4 g_arenaservers+1368+76
CNSTI4 96
ASGNI4
line 1474
;1474:	g_arenaservers.remove.height			= 48;
ADDRGP4 g_arenaservers+1368+80
CNSTI4 48
ASGNI4
line 1475
;1475:	g_arenaservers.remove.focuspic			= ART_REMOVE1;
ADDRGP4 g_arenaservers+1368+60
ADDRGP4 $943
ASGNP4
line 1477
;1476:
;1477:	g_arenaservers.back.generic.type		= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1456
CNSTI4 6
ASGNI4
line 1478
;1478:	g_arenaservers.back.generic.name		= ART_BACK0;
ADDRGP4 g_arenaservers+1456+4
ADDRGP4 $947
ASGNP4
line 1479
;1479:	g_arenaservers.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1456+44
CNSTU4 260
ASGNU4
line 1480
;1480:	g_arenaservers.back.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+1456+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1481
;1481:	g_arenaservers.back.generic.id			= ID_BACK;
ADDRGP4 g_arenaservers+1456+8
CNSTI4 18
ASGNI4
line 1482
;1482:	g_arenaservers.back.generic.x			= 0;
ADDRGP4 g_arenaservers+1456+12
CNSTI4 0
ASGNI4
line 1483
;1483:	g_arenaservers.back.generic.y			= 480-64;
ADDRGP4 g_arenaservers+1456+16
CNSTI4 416
ASGNI4
line 1484
;1484:	g_arenaservers.back.width				= 128;
ADDRGP4 g_arenaservers+1456+76
CNSTI4 128
ASGNI4
line 1485
;1485:	g_arenaservers.back.height				= 64;
ADDRGP4 g_arenaservers+1456+80
CNSTI4 64
ASGNI4
line 1486
;1486:	g_arenaservers.back.focuspic			= ART_BACK1;
ADDRGP4 g_arenaservers+1456+60
ADDRGP4 $964
ASGNP4
line 1488
;1487:
;1488:	g_arenaservers.specify.generic.type	    = MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1632
CNSTI4 6
ASGNI4
line 1489
;1489:	g_arenaservers.specify.generic.name		= ART_SPECIFY0;
ADDRGP4 g_arenaservers+1632+4
ADDRGP4 $968
ASGNP4
line 1490
;1490:	g_arenaservers.specify.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1632+44
CNSTU4 260
ASGNU4
line 1491
;1491:	g_arenaservers.specify.generic.callback = ArenaServers_Event;
ADDRGP4 g_arenaservers+1632+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1492
;1492:	g_arenaservers.specify.generic.id	    = ID_SPECIFY;
ADDRGP4 g_arenaservers+1632+8
CNSTI4 20
ASGNI4
line 1493
;1493:	g_arenaservers.specify.generic.x		= 128;
ADDRGP4 g_arenaservers+1632+12
CNSTI4 128
ASGNI4
line 1494
;1494:	g_arenaservers.specify.generic.y		= 480-64;
ADDRGP4 g_arenaservers+1632+16
CNSTI4 416
ASGNI4
line 1495
;1495:	g_arenaservers.specify.width  		    = 128;
ADDRGP4 g_arenaservers+1632+76
CNSTI4 128
ASGNI4
line 1496
;1496:	g_arenaservers.specify.height  		    = 64;
ADDRGP4 g_arenaservers+1632+80
CNSTI4 64
ASGNI4
line 1497
;1497:	g_arenaservers.specify.focuspic         = ART_SPECIFY1;
ADDRGP4 g_arenaservers+1632+60
ADDRGP4 $985
ASGNP4
line 1499
;1498:
;1499:	g_arenaservers.refresh.generic.type		= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1544
CNSTI4 6
ASGNI4
line 1500
;1500:	g_arenaservers.refresh.generic.name		= ART_REFRESH0;
ADDRGP4 g_arenaservers+1544+4
ADDRGP4 $989
ASGNP4
line 1501
;1501:	g_arenaservers.refresh.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1544+44
CNSTU4 260
ASGNU4
line 1502
;1502:	g_arenaservers.refresh.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+1544+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1503
;1503:	g_arenaservers.refresh.generic.id		= ID_REFRESH;
ADDRGP4 g_arenaservers+1544+8
CNSTI4 19
ASGNI4
line 1504
;1504:	g_arenaservers.refresh.generic.x		= 256;
ADDRGP4 g_arenaservers+1544+12
CNSTI4 256
ASGNI4
line 1505
;1505:	g_arenaservers.refresh.generic.y		= 480-64;
ADDRGP4 g_arenaservers+1544+16
CNSTI4 416
ASGNI4
line 1506
;1506:	g_arenaservers.refresh.width			= 128;
ADDRGP4 g_arenaservers+1544+76
CNSTI4 128
ASGNI4
line 1507
;1507:	g_arenaservers.refresh.height			= 64;
ADDRGP4 g_arenaservers+1544+80
CNSTI4 64
ASGNI4
line 1508
;1508:	g_arenaservers.refresh.focuspic			= ART_REFRESH1;
ADDRGP4 g_arenaservers+1544+60
ADDRGP4 $1006
ASGNP4
line 1510
;1509:
;1510:	g_arenaservers.create.generic.type		= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1720
CNSTI4 6
ASGNI4
line 1511
;1511:	g_arenaservers.create.generic.name		= ART_CREATE0;
ADDRGP4 g_arenaservers+1720+4
ADDRGP4 $1010
ASGNP4
line 1512
;1512:	g_arenaservers.create.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1720+44
CNSTU4 260
ASGNU4
line 1513
;1513:	g_arenaservers.create.generic.callback	= ArenaServers_Event;
ADDRGP4 g_arenaservers+1720+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1514
;1514:	g_arenaservers.create.generic.id		= ID_CREATE;
ADDRGP4 g_arenaservers+1720+8
CNSTI4 21
ASGNI4
line 1515
;1515:	g_arenaservers.create.generic.x			= 384;
ADDRGP4 g_arenaservers+1720+12
CNSTI4 384
ASGNI4
line 1516
;1516:	g_arenaservers.create.generic.y			= 480-64;
ADDRGP4 g_arenaservers+1720+16
CNSTI4 416
ASGNI4
line 1517
;1517:	g_arenaservers.create.width				= 128;
ADDRGP4 g_arenaservers+1720+76
CNSTI4 128
ASGNI4
line 1518
;1518:	g_arenaservers.create.height			= 64;
ADDRGP4 g_arenaservers+1720+80
CNSTI4 64
ASGNI4
line 1519
;1519:	g_arenaservers.create.focuspic			= ART_CREATE1;
ADDRGP4 g_arenaservers+1720+60
ADDRGP4 $1027
ASGNP4
line 1521
;1520:
;1521:	g_arenaservers.go.generic.type			= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+1808
CNSTI4 6
ASGNI4
line 1522
;1522:	g_arenaservers.go.generic.name			= ART_CONNECT0;
ADDRGP4 g_arenaservers+1808+4
ADDRGP4 $1031
ASGNP4
line 1523
;1523:	g_arenaservers.go.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
ADDRGP4 g_arenaservers+1808+44
CNSTU4 272
ASGNU4
line 1524
;1524:	g_arenaservers.go.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+1808+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1525
;1525:	g_arenaservers.go.generic.id			= ID_CONNECT;
ADDRGP4 g_arenaservers+1808+8
CNSTI4 22
ASGNI4
line 1526
;1526:	g_arenaservers.go.generic.x				= 640;
ADDRGP4 g_arenaservers+1808+12
CNSTI4 640
ASGNI4
line 1527
;1527:	g_arenaservers.go.generic.y				= 480-64;
ADDRGP4 g_arenaservers+1808+16
CNSTI4 416
ASGNI4
line 1528
;1528:	g_arenaservers.go.width					= 128;
ADDRGP4 g_arenaservers+1808+76
CNSTI4 128
ASGNI4
line 1529
;1529:	g_arenaservers.go.height				= 64;
ADDRGP4 g_arenaservers+1808+80
CNSTI4 64
ASGNI4
line 1530
;1530:	g_arenaservers.go.focuspic				= ART_CONNECT1;
ADDRGP4 g_arenaservers+1808+60
ADDRGP4 $1048
ASGNP4
line 1532
;1531:
;1532:	g_arenaservers.punkbuster.generic.type			= MTYPE_SPINCONTROL;
ADDRGP4 g_arenaservers+14860
CNSTI4 3
ASGNI4
line 1533
;1533:	g_arenaservers.punkbuster.generic.name			= "Punkbuster:";
ADDRGP4 g_arenaservers+14860+4
ADDRGP4 $1052
ASGNP4
line 1534
;1534:	g_arenaservers.punkbuster.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
ADDRGP4 g_arenaservers+14860+44
CNSTU4 258
ASGNU4
line 1535
;1535:	g_arenaservers.punkbuster.generic.callback		= ArenaServers_Event;
ADDRGP4 g_arenaservers+14860+48
ADDRGP4 ArenaServers_Event
ASGNP4
line 1536
;1536:	g_arenaservers.punkbuster.generic.id			= ID_PUNKBUSTER;
ADDRGP4 g_arenaservers+14860+8
CNSTI4 24
ASGNI4
line 1537
;1537:	g_arenaservers.punkbuster.generic.x				= 480+32;
ADDRGP4 g_arenaservers+14860+12
CNSTI4 512
ASGNI4
line 1538
;1538:	g_arenaservers.punkbuster.generic.y				= 144;
ADDRGP4 g_arenaservers+14860+16
CNSTI4 144
ASGNI4
line 1539
;1539:	g_arenaservers.punkbuster.itemnames				= punkbuster_items;
ADDRGP4 g_arenaservers+14860+76
ADDRGP4 punkbuster_items
ASGNP4
line 1541
;1540:	
;1541:	g_arenaservers.pblogo.generic.type			= MTYPE_BITMAP;
ADDRGP4 g_arenaservers+14956
CNSTI4 6
ASGNI4
line 1542
;1542:	g_arenaservers.pblogo.generic.name			= ART_PUNKBUSTER;
ADDRGP4 g_arenaservers+14956+4
ADDRGP4 $1068
ASGNP4
line 1543
;1543:	g_arenaservers.pblogo.generic.flags			= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
ADDRGP4 g_arenaservers+14956+44
CNSTU4 16388
ASGNU4
line 1544
;1544:	g_arenaservers.pblogo.generic.x				= 526;
ADDRGP4 g_arenaservers+14956+12
CNSTI4 526
ASGNI4
line 1545
;1545:	g_arenaservers.pblogo.generic.y				= 176;
ADDRGP4 g_arenaservers+14956+16
CNSTI4 176
ASGNI4
line 1546
;1546:	g_arenaservers.pblogo.width					= 32;
ADDRGP4 g_arenaservers+14956+76
CNSTI4 32
ASGNI4
line 1547
;1547:	g_arenaservers.pblogo.height				= 16;
ADDRGP4 g_arenaservers+14956+80
CNSTI4 16
ASGNI4
line 1548
;1548:	g_arenaservers.pblogo.errorpic				= ART_UNKNOWNMAP;
ADDRGP4 g_arenaservers+14956+64
ADDRGP4 $848
ASGNP4
line 1550
;1549:	
;1550:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.banner );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+288
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1552
;1551:
;1552:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.master );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+360
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1553
;1553:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.gametype );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+456
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1554
;1554:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.sortkey );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+552
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1555
;1555:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.showfull);
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+648
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1556
;1556:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.showempty );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+712
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1558
;1557:
;1558:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.mappic );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+872
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1559
;1559:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.list );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+776
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1560
;1560:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.status );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1224
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1561
;1561:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.statusbar );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1296
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1562
;1562:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.arrows );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+960
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1563
;1563:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.up );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1048
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1564
;1564:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.down );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1136
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1566
;1565:
;1566:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.remove );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1368
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1567
;1567:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.back );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1456
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1568
;1568:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.specify );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1632
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1569
;1569:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.refresh );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1544
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1570
;1570:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.create );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1720
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1571
;1571:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.go );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+1808
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1573
;1572:
;1573:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.punkbuster );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+14860
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1574
;1574:	Menu_AddItem( &g_arenaservers.menu, (void*) &g_arenaservers.pblogo );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 g_arenaservers+14956
ARGP4
ADDRGP4 Menu_AddItem
CALLV
pop
line 1576
;1575:	
;1576:	ArenaServers_LoadFavorites();
ADDRGP4 ArenaServers_LoadFavorites
CALLV
pop
line 1578
;1577:
;1578:	g_servertype = Com_Clamp( 0, 3, ui_browserMaster.integer );
CNSTF4 0
ARGF4
CNSTF4 1077936128
ARGF4
ADDRGP4 ui_browserMaster+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 16
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_servertype
ADDRLP4 16
INDIRF4
CVFI4 4
ASGNI4
line 1580
;1579:	// hack to get rid of MPlayer stuff
;1580:	value = g_servertype;
ADDRLP4 8
ADDRGP4 g_servertype
INDIRI4
ASGNI4
line 1581
;1581:	if (value >= 1)
ADDRLP4 8
INDIRI4
CNSTI4 1
LTI4 $1103
line 1582
;1582:		value--;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $1103
line 1583
;1583:	g_arenaservers.master.curvalue = value;
ADDRGP4 g_arenaservers+360+64
ADDRLP4 8
INDIRI4
ASGNI4
line 1585
;1584:
;1585:	g_gametype = Com_Clamp( 0, 4, ui_browserGameType.integer );
CNSTF4 0
ARGF4
CNSTF4 1082130432
ARGF4
ADDRGP4 ui_browserGameType+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_gametype
ADDRLP4 20
INDIRF4
CVFI4 4
ASGNI4
line 1586
;1586:	g_arenaservers.gametype.curvalue = g_gametype;
ADDRGP4 g_arenaservers+456+64
ADDRGP4 g_gametype
INDIRI4
ASGNI4
line 1588
;1587:
;1588:	g_sortkey = Com_Clamp( 0, 4, ui_browserSortKey.integer );
CNSTF4 0
ARGF4
CNSTF4 1082130432
ARGF4
ADDRGP4 ui_browserSortKey+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_sortkey
ADDRLP4 24
INDIRF4
CVFI4 4
ASGNI4
line 1589
;1589:	g_arenaservers.sortkey.curvalue = g_sortkey;
ADDRGP4 g_arenaservers+552+64
ADDRGP4 g_sortkey
INDIRI4
ASGNI4
line 1591
;1590:
;1591:	g_fullservers = Com_Clamp( 0, 1, ui_browserShowFull.integer );
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRGP4 ui_browserShowFull+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 28
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_fullservers
ADDRLP4 28
INDIRF4
CVFI4 4
ASGNI4
line 1592
;1592:	g_arenaservers.showfull.curvalue = g_fullservers;
ADDRGP4 g_arenaservers+648+60
ADDRGP4 g_fullservers
INDIRI4
ASGNI4
line 1594
;1593:
;1594:	g_emptyservers = Com_Clamp( 0, 1, ui_browserShowEmpty.integer );
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRGP4 ui_browserShowEmpty+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 32
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_emptyservers
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 1595
;1595:	g_arenaservers.showempty.curvalue = g_emptyservers;
ADDRGP4 g_arenaservers+712+60
ADDRGP4 g_emptyservers
INDIRI4
ASGNI4
line 1597
;1596:	
;1597:	g_arenaservers.punkbuster.curvalue = Com_Clamp( 0, 1, trap_Cvar_VariableValue( "cl_punkbuster" ) );
ADDRGP4 $637
ARGP4
ADDRLP4 36
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
CNSTF4 0
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Com_Clamp
CALLF4
ASGNF4
ADDRGP4 g_arenaservers+14860+64
ADDRLP4 40
INDIRF4
CVFI4 4
ASGNI4
line 1600
;1598:
;1599:	// force to initial state and refresh
;1600:	type = g_servertype;
ADDRLP4 12
ADDRGP4 g_servertype
INDIRI4
ASGNI4
line 1601
;1601:	g_servertype = -1;
ADDRGP4 g_servertype
CNSTI4 -1
ASGNI4
line 1602
;1602:	ArenaServers_SetType( type );
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 ArenaServers_SetType
CALLV
pop
line 1604
;1603:
;1604:	trap_Cvar_Register(NULL, "debug_protocol", "", 0 );
CNSTP4 0
ARGP4
ADDRGP4 $570
ARGP4
ADDRGP4 $202
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1605
;1605:}
LABELV $715
endproc ArenaServers_MenuInit 44 16
export ArenaServers_Cache
proc ArenaServers_Cache 0 4
line 1613
;1606:
;1607:
;1608:/*
;1609:=================
;1610:ArenaServers_Cache
;1611:=================
;1612:*/
;1613:void ArenaServers_Cache( void ) {
line 1614
;1614:	trap_R_RegisterShaderNoMip( ART_BACK0 );
ADDRGP4 $947
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1615
;1615:	trap_R_RegisterShaderNoMip( ART_BACK1 );
ADDRGP4 $964
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1616
;1616:	trap_R_RegisterShaderNoMip( ART_CREATE0 );
ADDRGP4 $1010
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1617
;1617:	trap_R_RegisterShaderNoMip( ART_CREATE1 );
ADDRGP4 $1027
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1618
;1618:	trap_R_RegisterShaderNoMip( ART_SPECIFY0 );
ADDRGP4 $968
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1619
;1619:	trap_R_RegisterShaderNoMip( ART_SPECIFY1 );
ADDRGP4 $985
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1620
;1620:	trap_R_RegisterShaderNoMip( ART_REFRESH0 );
ADDRGP4 $989
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1621
;1621:	trap_R_RegisterShaderNoMip( ART_REFRESH1 );
ADDRGP4 $1006
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1622
;1622:	trap_R_RegisterShaderNoMip( ART_CONNECT0 );
ADDRGP4 $1031
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1623
;1623:	trap_R_RegisterShaderNoMip( ART_CONNECT1 );
ADDRGP4 $1048
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1624
;1624:	trap_R_RegisterShaderNoMip( ART_ARROWS0  );
ADDRGP4 $852
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1625
;1625:	trap_R_RegisterShaderNoMip( ART_ARROWS_UP );
ADDRGP4 $882
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1626
;1626:	trap_R_RegisterShaderNoMip( ART_ARROWS_DOWN );
ADDRGP4 $900
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1627
;1627:	trap_R_RegisterShaderNoMip( ART_UNKNOWNMAP );
ADDRGP4 $848
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1628
;1628:	trap_R_RegisterShaderNoMip( ART_PUNKBUSTER );
ADDRGP4 $1068
ARGP4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
pop
line 1629
;1629:}
LABELV $1121
endproc ArenaServers_Cache 0 4
export UI_ArenaServersMenu
proc UI_ArenaServersMenu 0 4
line 1637
;1630:
;1631:
;1632:/*
;1633:=================
;1634:UI_ArenaServersMenu
;1635:=================
;1636:*/
;1637:void UI_ArenaServersMenu( void ) {
line 1638
;1638:	ArenaServers_MenuInit();
ADDRGP4 ArenaServers_MenuInit
CALLV
pop
line 1639
;1639:	UI_PushMenu( &g_arenaservers.menu );
ADDRGP4 g_arenaservers
ARGP4
ADDRGP4 UI_PushMenu
CALLV
pop
line 1640
;1640:}						  
LABELV $1122
endproc UI_ArenaServersMenu 0 4
bss
align 4
LABELV g_fullservers
skip 4
align 4
LABELV g_emptyservers
skip 4
align 4
LABELV g_sortkey
skip 4
align 4
LABELV g_gametype
skip 4
align 4
LABELV g_servertype
skip 4
align 4
LABELV g_nummplayerservers
skip 4
align 4
LABELV g_mplayerserverlist
skip 19456
align 4
LABELV g_numfavoriteservers
skip 4
align 4
LABELV g_favoriteserverlist
skip 2432
align 4
LABELV g_numlocalservers
skip 4
align 4
LABELV g_localserverlist
skip 19456
align 4
LABELV g_numglobalservers
skip 4
align 4
LABELV g_globalserverlist
skip 19456
align 4
LABELV g_arenaservers
skip 15044
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
LABELV $1068
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 112
byte 1 98
byte 1 108
byte 1 111
byte 1 103
byte 1 111
byte 1 0
align 1
LABELV $1052
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
LABELV $1048
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
LABELV $1031
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
LABELV $1027
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
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $1010
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
byte 1 114
byte 1 101
byte 1 97
byte 1 116
byte 1 101
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $1006
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 114
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $989
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 114
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $985
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 105
byte 1 102
byte 1 121
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $968
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 105
byte 1 102
byte 1 121
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $964
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
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $943
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 95
byte 1 49
byte 1 0
align 1
LABELV $926
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 47
byte 1 97
byte 1 114
byte 1 116
byte 1 47
byte 1 100
byte 1 101
byte 1 108
byte 1 101
byte 1 116
byte 1 101
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $900
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
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $882
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
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $852
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
byte 1 114
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 95
byte 1 118
byte 1 101
byte 1 114
byte 1 116
byte 1 95
byte 1 48
byte 1 0
align 1
LABELV $848
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
LABELV $800
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 32
byte 1 69
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $786
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 32
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 58
byte 1 0
align 1
LABELV $770
byte 1 83
byte 1 111
byte 1 114
byte 1 116
byte 1 32
byte 1 66
byte 1 121
byte 1 58
byte 1 0
align 1
LABELV $754
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
LABELV $738
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 58
byte 1 0
align 1
LABELV $730
byte 1 65
byte 1 82
byte 1 69
byte 1 78
byte 1 65
byte 1 32
byte 1 83
byte 1 69
byte 1 82
byte 1 86
byte 1 69
byte 1 82
byte 1 83
byte 1 0
align 1
LABELV $697
byte 1 68
byte 1 105
byte 1 115
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
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
byte 1 63
byte 1 0
align 1
LABELV $696
byte 1 69
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
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
byte 1 63
byte 1 0
align 1
LABELV $673
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 69
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $667
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 104
byte 1 111
byte 1 119
byte 1 70
byte 1 117
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $661
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 83
byte 1 111
byte 1 114
byte 1 116
byte 1 75
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $655
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $653
byte 1 117
byte 1 105
byte 1 95
byte 1 98
byte 1 114
byte 1 111
byte 1 119
byte 1 115
byte 1 101
byte 1 114
byte 1 77
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $637
byte 1 99
byte 1 108
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
LABELV $631
byte 1 104
byte 1 105
byte 1 116
byte 1 32
byte 1 114
byte 1 101
byte 1 102
byte 1 114
byte 1 101
byte 1 115
byte 1 104
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 117
byte 1 112
byte 1 100
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $575
byte 1 112
byte 1 114
byte 1 111
byte 1 116
byte 1 111
byte 1 99
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $574
byte 1 103
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 37
byte 1 100
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $573
byte 1 103
byte 1 108
byte 1 111
byte 1 98
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $570
byte 1 100
byte 1 101
byte 1 98
byte 1 117
byte 1 103
byte 1 95
byte 1 112
byte 1 114
byte 1 111
byte 1 116
byte 1 111
byte 1 99
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $569
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $566
byte 1 32
byte 1 101
byte 1 109
byte 1 112
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $562
byte 1 32
byte 1 99
byte 1 116
byte 1 102
byte 1 0
align 1
LABELV $560
byte 1 32
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $558
byte 1 32
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $556
byte 1 32
byte 1 102
byte 1 102
byte 1 97
byte 1 0
align 1
LABELV $544
byte 1 108
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $522
byte 1 112
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $412
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
LABELV $391
byte 1 78
byte 1 111
byte 1 32
byte 1 82
byte 1 101
byte 1 115
byte 1 112
byte 1 111
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $383
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
LABELV $382
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $381
byte 1 110
byte 1 101
byte 1 116
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $380
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
LABELV $379
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $378
byte 1 109
byte 1 105
byte 1 110
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $377
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
LABELV $376
byte 1 99
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $375
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $374
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
LABELV $310
byte 1 78
byte 1 111
byte 1 0
align 1
LABELV $309
byte 1 89
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $308
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 46
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 49
byte 1 50
byte 1 46
byte 1 49
byte 1 50
byte 1 115
byte 1 32
byte 1 37
byte 1 50
byte 1 100
byte 1 47
byte 1 37
byte 1 50
byte 1 100
byte 1 32
byte 1 37
byte 1 45
byte 1 56
byte 1 46
byte 1 56
byte 1 115
byte 1 32
byte 1 37
byte 1 51
byte 1 115
byte 1 32
byte 1 37
byte 1 115
byte 1 37
byte 1 51
byte 1 100
byte 1 32
byte 1 94
byte 1 51
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $307
byte 1 94
byte 1 49
byte 1 0
align 1
LABELV $306
byte 1 94
byte 1 51
byte 1 0
align 1
LABELV $303
byte 1 94
byte 1 50
byte 1 0
align 1
LABELV $298
byte 1 94
byte 1 52
byte 1 0
align 1
LABELV $237
byte 1 78
byte 1 111
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 32
byte 1 70
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 46
byte 1 0
align 1
LABELV $234
byte 1 78
byte 1 111
byte 1 32
byte 1 82
byte 1 101
byte 1 115
byte 1 112
byte 1 111
byte 1 110
byte 1 115
byte 1 101
byte 1 32
byte 1 70
byte 1 114
byte 1 111
byte 1 109
byte 1 32
byte 1 77
byte 1 97
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 46
byte 1 0
align 1
LABELV $208
byte 1 83
byte 1 99
byte 1 97
byte 1 110
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 70
byte 1 111
byte 1 114
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 46
byte 1 0
align 1
LABELV $202
byte 1 0
align 1
LABELV $174
byte 1 80
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 32
byte 1 83
byte 1 80
byte 1 65
byte 1 67
byte 1 69
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 115
byte 1 116
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $169
byte 1 37
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 32
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 46
byte 1 0
align 1
LABELV $153
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
byte 1 46
byte 1 116
byte 1 103
byte 1 97
byte 1 0
align 1
LABELV $140
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $105
byte 1 99
byte 1 108
byte 1 95
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $100
byte 1 105
byte 1 115
byte 1 32
byte 1 115
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 101
byte 1 100
byte 1 46
byte 1 0
align 1
LABELV $99
byte 1 81
byte 1 117
byte 1 97
byte 1 107
byte 1 101
byte 1 32
byte 1 73
byte 1 73
byte 1 73
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $98
byte 1 100
byte 1 105
byte 1 115
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 104
byte 1 101
byte 1 32
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $97
byte 1 80
byte 1 117
byte 1 110
byte 1 107
byte 1 66
byte 1 117
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 32
byte 1 119
byte 1 105
byte 1 108
byte 1 108
byte 1 32
byte 1 98
byte 1 101
byte 1 0
align 1
LABELV $96
byte 1 69
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $95
byte 1 68
byte 1 105
byte 1 115
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $94
byte 1 73
byte 1 80
byte 1 88
byte 1 0
align 1
LABELV $93
byte 1 85
byte 1 68
byte 1 80
byte 1 0
align 1
LABELV $92
byte 1 63
byte 1 63
byte 1 63
byte 1 0
align 1
LABELV $91
byte 1 79
byte 1 83
byte 1 80
byte 1 0
align 1
LABELV $90
byte 1 85
byte 1 114
byte 1 98
byte 1 97
byte 1 110
byte 1 32
byte 1 84
byte 1 101
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $89
byte 1 81
byte 1 51
byte 1 70
byte 1 0
align 1
LABELV $88
byte 1 82
byte 1 111
byte 1 99
byte 1 107
byte 1 101
byte 1 116
byte 1 32
byte 1 65
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 32
byte 1 51
byte 1 0
align 1
LABELV $87
byte 1 72
byte 1 97
byte 1 114
byte 1 118
byte 1 101
byte 1 115
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $86
byte 1 79
byte 1 118
byte 1 101
byte 1 114
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $85
byte 1 79
byte 1 110
byte 1 101
byte 1 32
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 32
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $84
byte 1 67
byte 1 84
byte 1 70
byte 1 0
align 1
LABELV $83
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 32
byte 1 68
byte 1 77
byte 1 0
align 1
LABELV $82
byte 1 83
byte 1 80
byte 1 32
byte 1 0
align 1
LABELV $81
byte 1 49
byte 1 118
byte 1 49
byte 1 0
align 1
LABELV $80
byte 1 68
byte 1 77
byte 1 32
byte 1 0
align 1
LABELV $79
byte 1 80
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $78
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 84
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $77
byte 1 79
byte 1 112
byte 1 101
byte 1 110
byte 1 32
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 83
byte 1 112
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $76
byte 1 77
byte 1 97
byte 1 112
byte 1 32
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $75
byte 1 83
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $74
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
LABELV $73
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
LABELV $72
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
LABELV $71
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
align 1
LABELV $70
byte 1 65
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $69
byte 1 70
byte 1 97
byte 1 118
byte 1 111
byte 1 114
byte 1 105
byte 1 116
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $68
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
LABELV $67
byte 1 76
byte 1 111
byte 1 99
byte 1 97
byte 1 108
byte 1 0
