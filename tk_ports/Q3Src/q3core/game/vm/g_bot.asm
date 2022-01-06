export trap_Cvar_VariableValue
code
proc trap_Cvar_VariableValue 132 12
file "../g_bot.c"
line 55
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
;23:// g_bot.c
;24:
;25:#include "g_local.h"
;26:
;27:
;28:static int		g_numBots;
;29:static char		*g_botInfos[MAX_BOTS];
;30:
;31:
;32:int				g_numArenas;
;33:static char		*g_arenaInfos[MAX_ARENAS];
;34:
;35:
;36:#define BOT_BEGIN_DELAY_BASE		2000
;37:#define BOT_BEGIN_DELAY_INCREMENT	1500
;38:
;39:#define BOT_SPAWN_QUEUE_DEPTH	16
;40:
;41:typedef struct {
;42:	int		clientNum;
;43:	int		spawnTime;
;44:} botSpawnQueue_t;
;45:
;46://static int			botBeginDelay = 0;  // bk001206 - unused, init
;47:static botSpawnQueue_t	botSpawnQueue[BOT_SPAWN_QUEUE_DEPTH];
;48:
;49:vmCvar_t bot_minplayers;
;50:
;51:extern gentity_t	*podium1;
;52:extern gentity_t	*podium2;
;53:extern gentity_t	*podium3;
;54:
;55:float trap_Cvar_VariableValue( const char *var_name ) {
line 58
;56:	char buf[128];
;57:
;58:	trap_Cvar_VariableStringBuffer(var_name, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 59
;59:	return atof(buf);
ADDRLP4 0
ARGP4
ADDRLP4 128
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 128
INDIRF4
RETF4
LABELV $53
endproc trap_Cvar_VariableValue 132 12
export G_ParseInfos
proc G_ParseInfos 2084 12
line 69
;60:}
;61:
;62:
;63:
;64:/*
;65:===============
;66:G_ParseInfos
;67:===============
;68:*/
;69:int G_ParseInfos( char *buf, int max, char *infos[] ) {
line 75
;70:	char	*token;
;71:	int		count;
;72:	char	key[MAX_TOKEN_CHARS];
;73:	char	info[MAX_INFO_STRING];
;74:
;75:	count = 0;
ADDRLP4 2052
CNSTI4 0
ASGNI4
ADDRGP4 $56
JUMPV
LABELV $55
line 77
;76:
;77:	while ( 1 ) {
line 78
;78:		token = COM_Parse( &buf );
ADDRFP4 0
ARGP4
ADDRLP4 2056
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2056
INDIRP4
ASGNP4
line 79
;79:		if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $58
line 80
;80:			break;
ADDRGP4 $57
JUMPV
LABELV $58
line 82
;81:		}
;82:		if ( strcmp( token, "{" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $62
ARGP4
ADDRLP4 2060
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2060
INDIRI4
CNSTI4 0
EQI4 $60
line 83
;83:			Com_Printf( "Missing { in info file\n" );
ADDRGP4 $63
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 84
;84:			break;
ADDRGP4 $57
JUMPV
LABELV $60
line 87
;85:		}
;86:
;87:		if ( count == max ) {
ADDRLP4 2052
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $64
line 88
;88:			Com_Printf( "Max infos exceeded\n" );
ADDRGP4 $66
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 89
;89:			break;
ADDRGP4 $57
JUMPV
LABELV $64
line 92
;90:		}
;91:
;92:		info[0] = '\0';
ADDRLP4 1028
CNSTI1 0
ASGNI1
ADDRGP4 $68
JUMPV
LABELV $67
line 93
;93:		while ( 1 ) {
line 94
;94:			token = COM_ParseExt( &buf, qtrue );
ADDRFP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 2064
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ASGNP4
line 95
;95:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $70
line 96
;96:				Com_Printf( "Unexpected end of info file\n" );
ADDRGP4 $72
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 97
;97:				break;
ADDRGP4 $69
JUMPV
LABELV $70
line 99
;98:			}
;99:			if ( !strcmp( token, "}" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $75
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $73
line 100
;100:				break;
ADDRGP4 $69
JUMPV
LABELV $73
line 102
;101:			}
;102:			Q_strncpyz( key, token, sizeof( key ) );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 104
;103:
;104:			token = COM_ParseExt( &buf, qfalse );
ADDRFP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 2072
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2072
INDIRP4
ASGNP4
line 105
;105:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $76
line 106
;106:				strcpy( token, "<NULL>" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $78
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 107
;107:			}
LABELV $76
line 108
;108:			Info_SetValueForKey( info, key, token );
ADDRLP4 1028
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 109
;109:		}
LABELV $68
line 93
ADDRGP4 $67
JUMPV
LABELV $69
line 111
;110:		//NOTE: extra space for arena number
;111:		infos[count] = G_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
ADDRLP4 1028
ARGP4
ADDRLP4 2064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $79
ARGP4
ADDRLP4 2068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $80
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2072
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 2072
INDIRP4
ARGP4
ADDRLP4 2076
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 2064
INDIRI4
ADDRLP4 2068
INDIRI4
ADDI4
ADDRLP4 2076
INDIRI4
ADDI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 2080
ADDRGP4 G_Alloc
CALLP4
ASGNP4
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 2080
INDIRP4
ASGNP4
line 112
;112:		if (infos[count]) {
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $81
line 113
;113:			strcpy(infos[count], info);
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1028
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 114
;114:			count++;
ADDRLP4 2052
ADDRLP4 2052
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 115
;115:		}
LABELV $81
line 116
;116:	}
LABELV $56
line 77
ADDRGP4 $55
JUMPV
LABELV $57
line 117
;117:	return count;
ADDRLP4 2052
INDIRI4
RETI4
LABELV $54
endproc G_ParseInfos 2084 12
proc G_LoadArenasFromFile 8216 16
line 125
;118:}
;119:
;120:/*
;121:===============
;122:G_LoadArenasFromFile
;123:===============
;124:*/
;125:static void G_LoadArenasFromFile( char *filename ) {
line 130
;126:	int				len;
;127:	fileHandle_t	f;
;128:	char			buf[MAX_ARENAS_TEXT];
;129:
;130:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8200
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8200
INDIRI4
ASGNI4
line 131
;131:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $84
line 132
;132:		trap_Printf( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $86
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 133
;133:		return;
ADDRGP4 $83
JUMPV
LABELV $84
line 135
;134:	}
;135:	if ( len >= MAX_ARENAS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $87
line 136
;136:		trap_Printf( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT ) );
ADDRGP4 $89
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 8192
ARGI4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 137
;137:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 138
;138:		return;
ADDRGP4 $83
JUMPV
LABELV $87
line 141
;139:	}
;140:
;141:	trap_FS_Read( buf, len, f );
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 142
;142:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 143
;143:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 145
;144:
;145:	g_numArenas += G_ParseInfos( buf, MAX_ARENAS - g_numArenas, &g_arenaInfos[g_numArenas] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 g_numArenas
ASGNP4
ADDRLP4 8208
ADDRLP4 8204
INDIRP4
INDIRI4
ASGNI4
CNSTI4 1024
ADDRLP4 8208
INDIRI4
SUBI4
ARGI4
ADDRLP4 8208
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 G_ParseInfos
CALLI4
ASGNI4
ADDRLP4 8204
INDIRP4
ADDRLP4 8208
INDIRI4
ADDRLP4 8212
INDIRI4
ADDI4
ASGNI4
line 146
;146:}
LABELV $83
endproc G_LoadArenasFromFile 8216 16
proc G_LoadArenas 1460 16
line 153
;147:
;148:/*
;149:===============
;150:G_LoadArenas
;151:===============
;152:*/
;153:static void G_LoadArenas( void ) {
line 162
;154:	int			numdirs;
;155:	vmCvar_t	arenasFile;
;156:	char		filename[128];
;157:	char		dirlist[1024];
;158:	char*		dirptr;
;159:	int			i, n;
;160:	int			dirlen;
;161:
;162:	g_numArenas = 0;
ADDRGP4 g_numArenas
CNSTI4 0
ASGNI4
line 164
;163:
;164:	trap_Cvar_Register( &arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 148
ARGP4
ADDRGP4 $91
ARGP4
ADDRGP4 $92
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 165
;165:	if( *arenasFile.string ) {
ADDRLP4 148+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $93
line 166
;166:		G_LoadArenasFromFile(arenasFile.string);
ADDRLP4 148+16
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 167
;167:	}
ADDRGP4 $94
JUMPV
LABELV $93
line 168
;168:	else {
line 169
;169:		G_LoadArenasFromFile("scripts/arenas.txt");
ADDRGP4 $97
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 170
;170:	}
LABELV $94
line 173
;171:
;172:	// get all arenas from .arena files
;173:	numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, 1024 );
ADDRGP4 $98
ARGP4
ADDRGP4 $99
ARGP4
ADDRLP4 420
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1444
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 144
ADDRLP4 1444
INDIRI4
ASGNI4
line 174
;174:	dirptr  = dirlist;
ADDRLP4 4
ADDRLP4 420
ASGNP4
line 175
;175:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 136
CNSTI4 0
ASGNI4
ADDRGP4 $103
JUMPV
LABELV $100
line 176
;176:		dirlen = strlen(dirptr);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 1448
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1448
INDIRI4
ASGNI4
line 177
;177:		strcpy(filename, "scripts/");
ADDRLP4 8
ARGP4
ADDRGP4 $104
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 178
;178:		strcat(filename, dirptr);
ADDRLP4 8
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 179
;179:		G_LoadArenasFromFile(filename);
ADDRLP4 8
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 180
;180:	}
LABELV $101
line 175
ADDRLP4 1448
CNSTI4 1
ASGNI4
ADDRLP4 136
ADDRLP4 136
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 140
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ADDRLP4 4
INDIRP4
ADDP4
ASGNP4
LABELV $103
ADDRLP4 136
INDIRI4
ADDRLP4 144
INDIRI4
LTI4 $100
line 181
;181:	trap_Printf( va( "%i arenas parsed\n", g_numArenas ) );
ADDRGP4 $105
ARGP4
ADDRGP4 g_numArenas
INDIRI4
ARGI4
ADDRLP4 1452
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1452
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 183
;182:	
;183:	for( n = 0; n < g_numArenas; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $109
JUMPV
LABELV $106
line 184
;184:		Info_SetValueForKey( g_arenaInfos[n], "num", va( "%i", n ) );
ADDRGP4 $111
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1456
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $110
ARGP4
ADDRLP4 1456
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 185
;185:	}
LABELV $107
line 183
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $109
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $106
line 186
;186:}
LABELV $90
endproc G_LoadArenas 1460 16
export G_GetArenaInfoByMap
proc G_GetArenaInfoByMap 12 8
line 194
;187:
;188:
;189:/*
;190:===============
;191:G_GetArenaInfoByNumber
;192:===============
;193:*/
;194:const char *G_GetArenaInfoByMap( const char *map ) {
line 197
;195:	int			n;
;196:
;197:	for( n = 0; n < g_numArenas; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $116
JUMPV
LABELV $113
line 198
;198:		if( Q_stricmp( Info_ValueForKey( g_arenaInfos[n], "map" ), map ) == 0 ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $119
ARGP4
ADDRLP4 4
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $117
line 199
;199:			return g_arenaInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $112
JUMPV
LABELV $117
line 201
;200:		}
;201:	}
LABELV $114
line 197
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $116
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $113
line 203
;202:
;203:	return NULL;
CNSTP4 0
RETP4
LABELV $112
endproc G_GetArenaInfoByMap 12 8
proc PlayerIntroSound 80 12
line 212
;204:}
;205:
;206:
;207:/*
;208:=================
;209:PlayerIntroSound
;210:=================
;211:*/
;212:static void PlayerIntroSound( const char *modelAndSkin ) {
line 216
;213:	char	model[MAX_QPATH];
;214:	char	*skin;
;215:
;216:	Q_strncpyz( model, modelAndSkin, sizeof(model) );
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
line 217
;217:	skin = Q_strrchr( model, '/' );
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
line 218
;218:	if ( skin ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $121
line 219
;219:		*skin++ = '\0';
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
line 220
;220:	}
ADDRGP4 $122
JUMPV
LABELV $121
line 221
;221:	else {
line 222
;222:		skin = model;
ADDRLP4 0
ADDRLP4 4
ASGNP4
line 223
;223:	}
LABELV $122
line 225
;224:
;225:	if( Q_stricmp( skin, "default" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $125
ARGP4
ADDRLP4 72
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $123
line 226
;226:		skin = model;
ADDRLP4 0
ADDRLP4 4
ASGNP4
line 227
;227:	}
LABELV $123
line 229
;228:
;229:	trap_SendConsoleCommand( EXEC_APPEND, va( "play sound/player/announce/%s.wav\n", skin ) );
ADDRGP4 $126
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 230
;230:}
LABELV $120
endproc PlayerIntroSound 80 12
export G_AddRandomBot
proc G_AddRandomBot 80 20
line 237
;231:
;232:/*
;233:===============
;234:G_AddRandomBot
;235:===============
;236:*/
;237:void G_AddRandomBot( int team ) {
line 243
;238:	int		i, n, num;
;239:	float	skill;
;240:	char	*value, netname[36], *teamstr;
;241:	gclient_t	*cl;
;242:
;243:	num = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 244
;244:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $131
JUMPV
LABELV $128
line 245
;245:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 64
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 64
INDIRP4
ASGNP4
line 247
;246:		//
;247:		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $136
JUMPV
LABELV $133
line 248
;248:			cl = level.clients + i;
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 249
;249:			if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $138
line 250
;250:				continue;
ADDRGP4 $134
JUMPV
LABELV $138
line 252
;251:			}
;252:			if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 808
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $140
line 253
;253:				continue;
ADDRGP4 $134
JUMPV
LABELV $140
line 255
;254:			}
;255:			if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 68
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
LTI4 $144
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 68
INDIRI4
EQI4 $144
line 256
;256:				continue;
ADDRGP4 $134
JUMPV
LABELV $144
line 258
;257:			}
;258:			if ( !Q_stricmp( value, cl->pers.netname ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 72
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $146
line 259
;259:				break;
ADDRGP4 $135
JUMPV
LABELV $146
line 261
;260:			}
;261:		}
LABELV $134
line 247
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $136
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $133
LABELV $135
line 262
;262:		if (i >= g_maxclients.integer) {
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $148
line 263
;263:			num++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 264
;264:		}
LABELV $148
line 265
;265:	}
LABELV $129
line 244
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $131
ADDRLP4 12
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $128
line 266
;266:	num = random() * num;
ADDRLP4 64
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 64
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 16
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 267
;267:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $154
JUMPV
LABELV $151
line 268
;268:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 68
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 68
INDIRP4
ASGNP4
line 270
;269:		//
;270:		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $158
JUMPV
LABELV $155
line 271
;271:			cl = level.clients + i;
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 272
;272:			if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $160
line 273
;273:				continue;
ADDRGP4 $156
JUMPV
LABELV $160
line 275
;274:			}
;275:			if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 808
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $162
line 276
;276:				continue;
ADDRGP4 $156
JUMPV
LABELV $162
line 278
;277:			}
;278:			if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 72
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
LTI4 $166
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
EQI4 $166
line 279
;279:				continue;
ADDRGP4 $156
JUMPV
LABELV $166
line 281
;280:			}
;281:			if ( !Q_stricmp( value, cl->pers.netname ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRLP4 76
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
NEI4 $168
line 282
;282:				break;
ADDRGP4 $157
JUMPV
LABELV $168
line 284
;283:			}
;284:		}
LABELV $156
line 270
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $158
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $155
LABELV $157
line 285
;285:		if (i >= g_maxclients.integer) {
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $170
line 286
;286:			num--;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 287
;287:			if (num <= 0) {
ADDRLP4 16
INDIRI4
CNSTI4 0
GTI4 $173
line 288
;288:				skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $175
ARGP4
ADDRLP4 72
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 56
ADDRLP4 72
INDIRF4
ASGNF4
line 289
;289:				if (team == TEAM_RED) teamstr = "red";
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $176
ADDRLP4 60
ADDRGP4 $178
ASGNP4
ADDRGP4 $177
JUMPV
LABELV $176
line 290
;290:				else if (team == TEAM_BLUE) teamstr = "blue";
ADDRFP4 0
INDIRI4
CNSTI4 2
NEI4 $179
ADDRLP4 60
ADDRGP4 $181
ASGNP4
ADDRGP4 $180
JUMPV
LABELV $179
line 291
;291:				else teamstr = "";
ADDRLP4 60
ADDRGP4 $92
ASGNP4
LABELV $180
LABELV $177
line 292
;292:				strncpy(netname, value, sizeof(netname)-1);
ADDRLP4 20
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 35
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 293
;293:				netname[sizeof(netname)-1] = '\0';
ADDRLP4 20+35
CNSTI1 0
ASGNI1
line 294
;294:				Q_CleanStr(netname);
ADDRLP4 20
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 295
;295:				trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s %f %s %i\n", netname, skill, teamstr, 0) );
ADDRGP4 $183
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 296
;296:				return;
ADDRGP4 $127
JUMPV
LABELV $173
line 298
;297:			}
;298:		}
LABELV $170
line 299
;299:	}
LABELV $152
line 267
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $154
ADDRLP4 12
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $151
line 300
;300:}
LABELV $127
endproc G_AddRandomBot 80 20
export G_RemoveRandomBot
proc G_RemoveRandomBot 52 8
line 307
;301:
;302:/*
;303:===============
;304:G_RemoveRandomBot
;305:===============
;306:*/
;307:int G_RemoveRandomBot( int team ) {
line 312
;308:	int i;
;309:	char netname[36];
;310:	gclient_t	*cl;
;311:
;312:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $188
JUMPV
LABELV $185
line 313
;313:		cl = level.clients + i;
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 314
;314:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $190
line 315
;315:			continue;
ADDRGP4 $186
JUMPV
LABELV $190
line 317
;316:		}
;317:		if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 808
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $192
line 318
;318:			continue;
ADDRGP4 $186
JUMPV
LABELV $192
line 320
;319:		}
;320:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 44
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
LTI4 $196
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
EQI4 $196
line 321
;321:			continue;
ADDRGP4 $186
JUMPV
LABELV $196
line 323
;322:		}
;323:		strcpy(netname, cl->pers.netname);
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 512
ADDP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 324
;324:		Q_CleanStr(netname);
ADDRLP4 8
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 325
;325:		trap_SendConsoleCommand( EXEC_INSERT, va("kick %s\n", netname) );
ADDRGP4 $198
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 48
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 326
;326:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $184
JUMPV
LABELV $186
line 312
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $188
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $185
line 328
;327:	}
;328:	return qfalse;
CNSTI4 0
RETI4
LABELV $184
endproc G_RemoveRandomBot 52 8
export G_CountHumanPlayers
proc G_CountHumanPlayers 16 0
line 336
;329:}
;330:
;331:/*
;332:===============
;333:G_CountHumanPlayers
;334:===============
;335:*/
;336:int G_CountHumanPlayers( int team ) {
line 340
;337:	int i, num;
;338:	gclient_t	*cl;
;339:
;340:	num = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 341
;341:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $203
JUMPV
LABELV $200
line 342
;342:		cl = level.clients + i;
ADDRLP4 0
CNSTI4 776
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 343
;343:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $205
line 344
;344:			continue;
ADDRGP4 $201
JUMPV
LABELV $205
line 346
;345:		}
;346:		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
CNSTI4 808
ADDRLP4 0
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $207
line 347
;347:			continue;
ADDRGP4 $201
JUMPV
LABELV $207
line 349
;348:		}
;349:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 12
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
LTI4 $211
ADDRLP4 0
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $211
line 350
;350:			continue;
ADDRGP4 $201
JUMPV
LABELV $211
line 352
;351:		}
;352:		num++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 353
;353:	}
LABELV $201
line 341
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $203
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $200
line 354
;354:	return num;
ADDRLP4 8
INDIRI4
RETI4
LABELV $199
endproc G_CountHumanPlayers 16 0
export G_CountBotPlayers
proc G_CountBotPlayers 20 0
line 362
;355:}
;356:
;357:/*
;358:===============
;359:G_CountBotPlayers
;360:===============
;361:*/
;362:int G_CountBotPlayers( int team ) {
line 366
;363:	int i, n, num;
;364:	gclient_t	*cl;
;365:
;366:	num = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 367
;367:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $217
JUMPV
LABELV $214
line 368
;368:		cl = level.clients + i;
ADDRLP4 4
CNSTI4 776
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 369
;369:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 4
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
EQI4 $219
line 370
;370:			continue;
ADDRGP4 $215
JUMPV
LABELV $219
line 372
;371:		}
;372:		if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 808
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+208+216
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $221
line 373
;373:			continue;
ADDRGP4 $215
JUMPV
LABELV $221
line 375
;374:		}
;375:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 16
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $225
ADDRLP4 4
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $225
line 376
;376:			continue;
ADDRGP4 $215
JUMPV
LABELV $225
line 378
;377:		}
;378:		num++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 379
;379:	}
LABELV $215
line 367
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $217
ADDRLP4 8
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $214
line 380
;380:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $227
line 381
;381:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $231
line 382
;382:			continue;
ADDRGP4 $228
JUMPV
LABELV $231
line 384
;383:		}
;384:		if ( botSpawnQueue[n].spawnTime > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $234
line 385
;385:			continue;
ADDRGP4 $228
JUMPV
LABELV $234
line 387
;386:		}
;387:		num++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 388
;388:	}
LABELV $228
line 380
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $227
line 389
;389:	return num;
ADDRLP4 12
INDIRI4
RETI4
LABELV $213
endproc G_CountBotPlayers 20 0
bss
align 4
LABELV $239
skip 4
export G_CheckMinimumPlayers
code
proc G_CheckMinimumPlayers 36 4
line 397
;390:}
;391:
;392:/*
;393:===============
;394:G_CheckMinimumPlayers
;395:===============
;396:*/
;397:void G_CheckMinimumPlayers( void ) {
line 402
;398:	int minplayers;
;399:	int humanplayers, botplayers;
;400:	static int checkminimumplayers_time;
;401:
;402:	if (level.intermissiontime) return;
ADDRGP4 level+9128
INDIRI4
CNSTI4 0
EQI4 $240
ADDRGP4 $238
JUMPV
LABELV $240
line 404
;403:	//only check once each 10 seconds
;404:	if (checkminimumplayers_time > level.time - 10000) {
ADDRGP4 $239
INDIRI4
ADDRGP4 level+32
INDIRI4
CNSTI4 10000
SUBI4
LEI4 $243
line 405
;405:		return;
ADDRGP4 $238
JUMPV
LABELV $243
line 407
;406:	}
;407:	checkminimumplayers_time = level.time;
ADDRGP4 $239
ADDRGP4 level+32
INDIRI4
ASGNI4
line 408
;408:	trap_Cvar_Update(&bot_minplayers);
ADDRGP4 bot_minplayers
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 409
;409:	minplayers = bot_minplayers.integer;
ADDRLP4 0
ADDRGP4 bot_minplayers+12
INDIRI4
ASGNI4
line 410
;410:	if (minplayers <= 0) return;
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $248
ADDRGP4 $238
JUMPV
LABELV $248
line 412
;411:
;412:	if (g_gametype.integer >= GT_TEAM) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $250
line 413
;413:		if (minplayers >= g_maxclients.integer / 2) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 2
DIVI4
LTI4 $253
line 414
;414:			minplayers = (g_maxclients.integer / 2) -1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 2
DIVI4
CNSTI4 1
SUBI4
ASGNI4
line 415
;415:		}
LABELV $253
line 417
;416:
;417:		humanplayers = G_CountHumanPlayers( TEAM_RED );
CNSTI4 1
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 418
;418:		botplayers = G_CountBotPlayers(	TEAM_RED );
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 420
;419:		//
;420:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $257
line 421
;421:			G_AddRandomBot( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 422
;422:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $258
JUMPV
LABELV $257
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $259
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $259
line 423
;423:			G_RemoveRandomBot( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 424
;424:		}
LABELV $259
LABELV $258
line 426
;425:		//
;426:		humanplayers = G_CountHumanPlayers( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRLP4 24
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 24
INDIRI4
ASGNI4
line 427
;427:		botplayers = G_CountBotPlayers( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRLP4 28
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 28
INDIRI4
ASGNI4
line 429
;428:		//
;429:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $261
line 430
;430:			G_AddRandomBot( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 431
;431:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $251
JUMPV
LABELV $261
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $251
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $251
line 432
;432:			G_RemoveRandomBot( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 433
;433:		}
line 434
;434:	}
ADDRGP4 $251
JUMPV
LABELV $250
line 435
;435:	else if (g_gametype.integer == GT_TOURNAMENT ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
NEI4 $265
line 436
;436:		if (minplayers >= g_maxclients.integer) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $268
line 437
;437:			minplayers = g_maxclients.integer-1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 438
;438:		}
LABELV $268
line 439
;439:		humanplayers = G_CountHumanPlayers( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 440
;440:		botplayers = G_CountBotPlayers( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 442
;441:		//
;442:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $272
line 443
;443:			G_AddRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 444
;444:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $266
JUMPV
LABELV $272
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $266
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $266
line 446
;445:			// try to remove spectators first
;446:			if (!G_RemoveRandomBot( TEAM_SPECTATOR )) {
CNSTI4 3
ARGI4
ADDRLP4 24
ADDRGP4 G_RemoveRandomBot
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $266
line 448
;447:				// just remove the bot that is playing
;448:				G_RemoveRandomBot( -1 );
CNSTI4 -1
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 449
;449:			}
line 450
;450:		}
line 451
;451:	}
ADDRGP4 $266
JUMPV
LABELV $265
line 452
;452:	else if (g_gametype.integer == GT_FFA) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 0
NEI4 $278
line 453
;453:		if (minplayers >= g_maxclients.integer) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $281
line 454
;454:			minplayers = g_maxclients.integer-1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 455
;455:		}
LABELV $281
line 456
;456:		humanplayers = G_CountHumanPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 457
;457:		botplayers = G_CountBotPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 459
;458:		//
;459:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $285
line 460
;460:			G_AddRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 461
;461:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $286
JUMPV
LABELV $285
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $287
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $287
line 462
;462:			G_RemoveRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 463
;463:		}
LABELV $287
LABELV $286
line 464
;464:	}
LABELV $278
LABELV $266
LABELV $251
line 465
;465:}
LABELV $238
endproc G_CheckMinimumPlayers 36 4
export G_CheckBotSpawn
proc G_CheckBotSpawn 1032 12
line 472
;466:
;467:/*
;468:===============
;469:G_CheckBotSpawn
;470:===============
;471:*/
;472:void G_CheckBotSpawn( void ) {
line 476
;473:	int		n;
;474:	char	userinfo[MAX_INFO_VALUE];
;475:
;476:	G_CheckMinimumPlayers();
ADDRGP4 G_CheckMinimumPlayers
CALLV
pop
line 478
;477:
;478:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $290
line 479
;479:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $294
line 480
;480:			continue;
ADDRGP4 $291
JUMPV
LABELV $294
line 482
;481:		}
;482:		if ( botSpawnQueue[n].spawnTime > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $297
line 483
;483:			continue;
ADDRGP4 $291
JUMPV
LABELV $297
line 485
;484:		}
;485:		ClientBegin( botSpawnQueue[n].clientNum );
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 486
;486:		botSpawnQueue[n].spawnTime = 0;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
CNSTI4 0
ASGNI4
line 488
;487:
;488:		if( g_gametype.integer == GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $302
line 489
;489:			trap_GetUserinfo( botSpawnQueue[n].clientNum, userinfo, sizeof(userinfo) );
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 490
;490:			PlayerIntroSound( Info_ValueForKey (userinfo, "model") );
ADDRLP4 4
ARGP4
ADDRGP4 $305
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 PlayerIntroSound
CALLV
pop
line 491
;491:		}
LABELV $302
line 492
;492:	}
LABELV $291
line 478
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $290
line 493
;493:}
LABELV $289
endproc G_CheckBotSpawn 1032 12
proc AddBotToSpawnQueue 4 4
line 501
;494:
;495:
;496:/*
;497:===============
;498:AddBotToSpawnQueue
;499:===============
;500:*/
;501:static void AddBotToSpawnQueue( int clientNum, int delay ) {
line 504
;502:	int		n;
;503:
;504:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $307
line 505
;505:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $311
line 506
;506:			botSpawnQueue[n].spawnTime = level.time + delay;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
line 507
;507:			botSpawnQueue[n].clientNum = clientNum;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 508
;508:			return;
ADDRGP4 $306
JUMPV
LABELV $311
line 510
;509:		}
;510:	}
LABELV $308
line 504
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $307
line 512
;511:
;512:	G_Printf( S_COLOR_YELLOW "Unable to delay spawn\n" );
ADDRGP4 $316
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 513
;513:	ClientBegin( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 514
;514:}
LABELV $306
endproc AddBotToSpawnQueue 4 4
export G_RemoveQueuedBotBegin
proc G_RemoveQueuedBotBegin 4 0
line 525
;515:
;516:
;517:/*
;518:===============
;519:G_RemoveQueuedBotBegin
;520:
;521:Called on client disconnect to make sure the delayed spawn
;522:doesn't happen on a freed index
;523:===============
;524:*/
;525:void G_RemoveQueuedBotBegin( int clientNum ) {
line 528
;526:	int		n;
;527:
;528:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $318
line 529
;529:		if( botSpawnQueue[n].clientNum == clientNum ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $322
line 530
;530:			botSpawnQueue[n].spawnTime = 0;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
CNSTI4 0
ASGNI4
line 531
;531:			return;
ADDRGP4 $317
JUMPV
LABELV $322
line 533
;532:		}
;533:	}
LABELV $319
line 528
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $318
line 534
;534:}
LABELV $317
endproc G_RemoveQueuedBotBegin 4 0
export G_BotConnect
proc G_BotConnect 1336 12
line 542
;535:
;536:
;537:/*
;538:===============
;539:G_BotConnect
;540:===============
;541:*/
;542:qboolean G_BotConnect( int clientNum, qboolean restart ) {
line 546
;543:	bot_settings_t	settings;
;544:	char			userinfo[MAX_INFO_STRING];
;545:
;546:	trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 548
;547:
;548:	Q_strncpyz( settings.characterfile, Info_ValueForKey( userinfo, "characterfile" ), sizeof(settings.characterfile) );
ADDRLP4 292
ARGP4
ADDRGP4 $326
ARGP4
ADDRLP4 1316
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 1316
INDIRP4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 549
;549:	settings.skill = atof( Info_ValueForKey( userinfo, "skill" ) );
ADDRLP4 292
ARGP4
ADDRGP4 $328
ARGP4
ADDRLP4 1320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1320
INDIRP4
ARGP4
ADDRLP4 1324
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0+144
ADDRLP4 1324
INDIRF4
ASGNF4
line 550
;550:	Q_strncpyz( settings.team, Info_ValueForKey( userinfo, "team" ), sizeof(settings.team) );
ADDRLP4 292
ARGP4
ADDRGP4 $330
ARGP4
ADDRLP4 1328
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0+148
ARGP4
ADDRLP4 1328
INDIRP4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 552
;551:
;552:	if (!BotAISetupClient( clientNum, &settings, restart )) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 1332
ADDRGP4 BotAISetupClient
CALLI4
ASGNI4
ADDRLP4 1332
INDIRI4
CNSTI4 0
NEI4 $332
line 553
;553:		trap_DropClient( clientNum, "BotAISetupClient failed" );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $334
ARGP4
ADDRGP4 trap_DropClient
CALLV
pop
line 554
;554:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $325
JUMPV
LABELV $332
line 557
;555:	}
;556:
;557:	return qtrue;
CNSTI4 1
RETI4
LABELV $325
endproc G_BotConnect 1336 12
proc G_AddBot 1136 12
line 566
;558:}
;559:
;560:
;561:/*
;562:===============
;563:G_AddBot
;564:===============
;565:*/
;566:static void G_AddBot( const char *name, float skill, const char *team, int delay, char *altname) {
line 578
;567:	int				clientNum;
;568:	char			*botinfo;
;569:	gentity_t		*bot;
;570:	char			*key;
;571:	char			*s;
;572:	char			*botname;
;573:	char			*model;
;574:	char			*headmodel;
;575:	char			userinfo[MAX_INFO_STRING];
;576:
;577:	// get the botinfo from bots.txt
;578:	botinfo = G_GetBotInfoByName( name );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 G_GetBotInfoByName
CALLP4
ASGNP4
ADDRLP4 1032
ADDRLP4 1056
INDIRP4
ASGNP4
line 579
;579:	if ( !botinfo ) {
ADDRLP4 1032
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $336
line 580
;580:		G_Printf( S_COLOR_RED "Error: Bot '%s' not defined\n", name );
ADDRGP4 $338
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 581
;581:		return;
ADDRGP4 $335
JUMPV
LABELV $336
line 585
;582:	}
;583:
;584:	// create the bot's userinfo
;585:	userinfo[0] = '\0';
ADDRLP4 4
CNSTI1 0
ASGNI1
line 587
;586:
;587:	botname = Info_ValueForKey( botinfo, "funname" );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $339
ARGP4
ADDRLP4 1060
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1048
ADDRLP4 1060
INDIRP4
ASGNP4
line 588
;588:	if( !botname[0] ) {
ADDRLP4 1048
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $340
line 589
;589:		botname = Info_ValueForKey( botinfo, "name" );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 1064
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1048
ADDRLP4 1064
INDIRP4
ASGNP4
line 590
;590:	}
LABELV $340
line 592
;591:	// check for an alternative name
;592:	if (altname && altname[0]) {
ADDRLP4 1064
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 1064
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $342
ADDRLP4 1064
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $342
line 593
;593:		botname = altname;
ADDRLP4 1048
ADDRFP4 16
INDIRP4
ASGNP4
line 594
;594:	}
LABELV $342
line 595
;595:	Info_SetValueForKey( userinfo, "name", botname );
ADDRLP4 4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 596
;596:	Info_SetValueForKey( userinfo, "rate", "25000" );
ADDRLP4 4
ARGP4
ADDRGP4 $344
ARGP4
ADDRGP4 $345
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 597
;597:	Info_SetValueForKey( userinfo, "snaps", "20" );
ADDRLP4 4
ARGP4
ADDRGP4 $346
ARGP4
ADDRGP4 $347
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 598
;598:	Info_SetValueForKey( userinfo, "skill", va("%1.2f", skill) );
ADDRGP4 $348
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 1068
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRGP4 $328
ARGP4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 600
;599:
;600:	if ( skill >= 1 && skill < 2 ) {
ADDRLP4 1072
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1072
INDIRF4
CNSTF4 1065353216
LTF4 $349
ADDRLP4 1072
INDIRF4
CNSTF4 1073741824
GEF4 $349
line 601
;601:		Info_SetValueForKey( userinfo, "handicap", "50" );
ADDRLP4 4
ARGP4
ADDRGP4 $351
ARGP4
ADDRGP4 $352
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 602
;602:	}
ADDRGP4 $350
JUMPV
LABELV $349
line 603
;603:	else if ( skill >= 2 && skill < 3 ) {
ADDRLP4 1076
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1076
INDIRF4
CNSTF4 1073741824
LTF4 $353
ADDRLP4 1076
INDIRF4
CNSTF4 1077936128
GEF4 $353
line 604
;604:		Info_SetValueForKey( userinfo, "handicap", "70" );
ADDRLP4 4
ARGP4
ADDRGP4 $351
ARGP4
ADDRGP4 $355
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 605
;605:	}
ADDRGP4 $354
JUMPV
LABELV $353
line 606
;606:	else if ( skill >= 3 && skill < 4 ) {
ADDRLP4 1080
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1080
INDIRF4
CNSTF4 1077936128
LTF4 $356
ADDRLP4 1080
INDIRF4
CNSTF4 1082130432
GEF4 $356
line 607
;607:		Info_SetValueForKey( userinfo, "handicap", "90" );
ADDRLP4 4
ARGP4
ADDRGP4 $351
ARGP4
ADDRGP4 $358
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 608
;608:	}
LABELV $356
LABELV $354
LABELV $350
line 610
;609:
;610:	key = "model";
ADDRLP4 0
ADDRGP4 $305
ASGNP4
line 611
;611:	model = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1084
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1040
ADDRLP4 1084
INDIRP4
ASGNP4
line 612
;612:	if ( !*model ) {
ADDRLP4 1040
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $359
line 613
;613:		model = "visor/default";
ADDRLP4 1040
ADDRGP4 $361
ASGNP4
line 614
;614:	}
LABELV $359
line 615
;615:	Info_SetValueForKey( userinfo, key, model );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1040
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 616
;616:	key = "team_model";
ADDRLP4 0
ADDRGP4 $362
ASGNP4
line 617
;617:	Info_SetValueForKey( userinfo, key, model );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1040
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 619
;618:
;619:	key = "headmodel";
ADDRLP4 0
ADDRGP4 $363
ASGNP4
line 620
;620:	headmodel = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1088
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1044
ADDRLP4 1088
INDIRP4
ASGNP4
line 621
;621:	if ( !*headmodel ) {
ADDRLP4 1044
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $364
line 622
;622:		headmodel = model;
ADDRLP4 1044
ADDRLP4 1040
INDIRP4
ASGNP4
line 623
;623:	}
LABELV $364
line 624
;624:	Info_SetValueForKey( userinfo, key, headmodel );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1044
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 625
;625:	key = "team_headmodel";
ADDRLP4 0
ADDRGP4 $366
ASGNP4
line 626
;626:	Info_SetValueForKey( userinfo, key, headmodel );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1044
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 628
;627:
;628:	key = "gender";
ADDRLP4 0
ADDRGP4 $367
ASGNP4
line 629
;629:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1092
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1092
INDIRP4
ASGNP4
line 630
;630:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $368
line 631
;631:		s = "male";
ADDRLP4 1028
ADDRGP4 $370
ASGNP4
line 632
;632:	}
LABELV $368
line 633
;633:	Info_SetValueForKey( userinfo, "sex", s );
ADDRLP4 4
ARGP4
ADDRGP4 $371
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 635
;634:
;635:	key = "color1";
ADDRLP4 0
ADDRGP4 $372
ASGNP4
line 636
;636:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1096
INDIRP4
ASGNP4
line 637
;637:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $373
line 638
;638:		s = "4";
ADDRLP4 1028
ADDRGP4 $375
ASGNP4
line 639
;639:	}
LABELV $373
line 640
;640:	Info_SetValueForKey( userinfo, key, s );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 642
;641:
;642:	key = "color2";
ADDRLP4 0
ADDRGP4 $376
ASGNP4
line 643
;643:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1100
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1100
INDIRP4
ASGNP4
line 644
;644:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $377
line 645
;645:		s = "5";
ADDRLP4 1028
ADDRGP4 $379
ASGNP4
line 646
;646:	}
LABELV $377
line 647
;647:	Info_SetValueForKey( userinfo, key, s );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 649
;648:
;649:	s = Info_ValueForKey(botinfo, "aifile");
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 1104
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1104
INDIRP4
ASGNP4
line 650
;650:	if (!*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $381
line 651
;651:		trap_Printf( S_COLOR_RED "Error: bot has no aifile specified\n" );
ADDRGP4 $383
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 652
;652:		return;
ADDRGP4 $335
JUMPV
LABELV $381
line 656
;653:	}
;654:
;655:	// have the server allocate a client slot
;656:	clientNum = trap_BotAllocateClient();
ADDRLP4 1108
ADDRGP4 trap_BotAllocateClient
CALLI4
ASGNI4
ADDRLP4 1036
ADDRLP4 1108
INDIRI4
ASGNI4
line 657
;657:	if ( clientNum == -1 ) {
ADDRLP4 1036
INDIRI4
CNSTI4 -1
NEI4 $384
line 658
;658:		G_Printf( S_COLOR_RED "Unable to add bot.  All player slots are in use.\n" );
ADDRGP4 $386
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 659
;659:		G_Printf( S_COLOR_RED "Start server with more 'open' slots (or check setting of sv_maxclients cvar).\n" );
ADDRGP4 $387
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 660
;660:		return;
ADDRGP4 $335
JUMPV
LABELV $384
line 664
;661:	}
;662:
;663:	// initialize the bot settings
;664:	if( !team || !*team ) {
ADDRLP4 1112
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1112
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $390
ADDRLP4 1112
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $388
LABELV $390
line 665
;665:		if( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $391
line 666
;666:			if( PickTeam(clientNum) == TEAM_RED) {
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1116
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 1
NEI4 $394
line 667
;667:				team = "red";
ADDRFP4 8
ADDRGP4 $178
ASGNP4
line 668
;668:			}
ADDRGP4 $392
JUMPV
LABELV $394
line 669
;669:			else {
line 670
;670:				team = "blue";
ADDRFP4 8
ADDRGP4 $181
ASGNP4
line 671
;671:			}
line 672
;672:		}
ADDRGP4 $392
JUMPV
LABELV $391
line 673
;673:		else {
line 674
;674:			team = "red";
ADDRFP4 8
ADDRGP4 $178
ASGNP4
line 675
;675:		}
LABELV $392
line 676
;676:	}
LABELV $388
line 677
;677:	Info_SetValueForKey( userinfo, "characterfile", Info_ValueForKey( botinfo, "aifile" ) );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 1116
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRGP4 $326
ARGP4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 678
;678:	Info_SetValueForKey( userinfo, "skill", va( "%5.2f", skill ) );
ADDRGP4 $396
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 1120
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRGP4 $328
ARGP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 679
;679:	Info_SetValueForKey( userinfo, "team", team );
ADDRLP4 4
ARGP4
ADDRGP4 $330
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 681
;680:
;681:	bot = &g_entities[ clientNum ];
ADDRLP4 1052
CNSTI4 808
ADDRLP4 1036
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 682
;682:	bot->r.svFlags |= SVF_BOT;
ADDRLP4 1124
ADDRLP4 1052
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 1124
INDIRP4
ADDRLP4 1124
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 683
;683:	bot->inuse = qtrue;
ADDRLP4 1052
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 686
;684:
;685:	// register the userinfo
;686:	trap_SetUserinfo( clientNum, userinfo );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 689
;687:
;688:	// have it connect to the game as a normal client
;689:	if ( ClientConnect( clientNum, qtrue, qtrue ) ) {
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1128
CNSTI4 1
ASGNI4
ADDRLP4 1128
INDIRI4
ARGI4
ADDRLP4 1128
INDIRI4
ARGI4
ADDRLP4 1132
ADDRGP4 ClientConnect
CALLP4
ASGNP4
ADDRLP4 1132
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $397
line 690
;690:		return;
ADDRGP4 $335
JUMPV
LABELV $397
line 693
;691:	}
;692:
;693:	if( delay == 0 ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
NEI4 $399
line 694
;694:		ClientBegin( clientNum );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 695
;695:		return;
ADDRGP4 $335
JUMPV
LABELV $399
line 698
;696:	}
;697:
;698:	AddBotToSpawnQueue( clientNum, delay );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 AddBotToSpawnQueue
CALLV
pop
line 699
;699:}
LABELV $335
endproc G_AddBot 1136 12
export Svcmd_AddBot_f
proc Svcmd_AddBot_f 4112 20
line 707
;700:
;701:
;702:/*
;703:===============
;704:Svcmd_AddBot_f
;705:===============
;706:*/
;707:void Svcmd_AddBot_f( void ) {
line 716
;708:	float			skill;
;709:	int				delay;
;710:	char			name[MAX_TOKEN_CHARS];
;711:	char			altname[MAX_TOKEN_CHARS];
;712:	char			string[MAX_TOKEN_CHARS];
;713:	char			team[MAX_TOKEN_CHARS];
;714:
;715:	// are bots enabled?
;716:	if ( !trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
ADDRGP4 $404
ARGP4
ADDRLP4 4104
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 4104
INDIRI4
CNSTI4 0
NEI4 $402
line 717
;717:		return;
ADDRGP4 $401
JUMPV
LABELV $402
line 721
;718:	}
;719:
;720:	// name
;721:	trap_Argv( 1, name, sizeof( name ) );
CNSTI4 1
ARGI4
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 722
;722:	if ( !name[0] ) {
ADDRLP4 1024
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $405
line 723
;723:		trap_Printf( "Usage: Addbot <botname> [skill 1-5] [team] [msec delay] [altname]\n" );
ADDRGP4 $407
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 724
;724:		return;
ADDRGP4 $401
JUMPV
LABELV $405
line 728
;725:	}
;726:
;727:	// skill
;728:	trap_Argv( 2, string, sizeof( string ) );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 729
;729:	if ( !string[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $408
line 730
;730:		skill = 4;
ADDRLP4 4096
CNSTF4 1082130432
ASGNF4
line 731
;731:	}
ADDRGP4 $409
JUMPV
LABELV $408
line 732
;732:	else {
line 733
;733:		skill = atof( string );
ADDRLP4 0
ARGP4
ADDRLP4 4108
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 4096
ADDRLP4 4108
INDIRF4
ASGNF4
line 734
;734:	}
LABELV $409
line 737
;735:
;736:	// team
;737:	trap_Argv( 3, team, sizeof( team ) );
CNSTI4 3
ARGI4
ADDRLP4 3072
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 740
;738:
;739:	// delay
;740:	trap_Argv( 4, string, sizeof( string ) );
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 741
;741:	if ( !string[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $410
line 742
;742:		delay = 0;
ADDRLP4 4100
CNSTI4 0
ASGNI4
line 743
;743:	}
ADDRGP4 $411
JUMPV
LABELV $410
line 744
;744:	else {
line 745
;745:		delay = atoi( string );
ADDRLP4 0
ARGP4
ADDRLP4 4108
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4100
ADDRLP4 4108
INDIRI4
ASGNI4
line 746
;746:	}
LABELV $411
line 749
;747:
;748:	// alternative name
;749:	trap_Argv( 5, altname, sizeof( altname ) );
CNSTI4 5
ARGI4
ADDRLP4 2048
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 751
;750:
;751:	G_AddBot( name, skill, team, delay, altname );
ADDRLP4 1024
ARGP4
ADDRLP4 4096
INDIRF4
ARGF4
ADDRLP4 3072
ARGP4
ADDRLP4 4100
INDIRI4
ARGI4
ADDRLP4 2048
ARGP4
ADDRGP4 G_AddBot
CALLV
pop
line 755
;752:
;753:	// if this was issued during gameplay and we are playing locally,
;754:	// go ahead and load the bot's media immediately
;755:	if ( level.time - level.startTime > 1000 &&
ADDRGP4 level+32
INDIRI4
ADDRGP4 level+40
INDIRI4
SUBI4
CNSTI4 1000
LEI4 $412
ADDRGP4 $416
ARGP4
ADDRLP4 4108
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 4108
INDIRI4
CNSTI4 0
EQI4 $412
line 756
;756:		trap_Cvar_VariableIntegerValue( "cl_running" ) ) {
line 757
;757:		trap_SendServerCommand( -1, "loaddefered\n" );	// FIXME: spelled wrong, but not changing for demo
CNSTI4 -1
ARGI4
ADDRGP4 $417
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 758
;758:	}
LABELV $412
line 759
;759:}
LABELV $401
endproc Svcmd_AddBot_f 4112 20
export Svcmd_BotList_f
proc Svcmd_BotList_f 4120 20
line 766
;760:
;761:/*
;762:===============
;763:Svcmd_BotList_f
;764:===============
;765:*/
;766:void Svcmd_BotList_f( void ) {
line 773
;767:	int i;
;768:	char name[MAX_TOKEN_CHARS];
;769:	char funname[MAX_TOKEN_CHARS];
;770:	char model[MAX_TOKEN_CHARS];
;771:	char aifile[MAX_TOKEN_CHARS];
;772:
;773:	trap_Printf("^1name             model            aifile              funname\n");
ADDRGP4 $419
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 774
;774:	for (i = 0; i < g_numBots; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $423
JUMPV
LABELV $420
line 775
;775:		strcpy(name, Info_ValueForKey( g_botInfos[i], "name" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 4100
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRLP4 4100
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 776
;776:		if ( !*name ) {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $424
line 777
;777:			strcpy(name, "UnnamedPlayer");
ADDRLP4 4
ARGP4
ADDRGP4 $426
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 778
;778:		}
LABELV $424
line 779
;779:		strcpy(funname, Info_ValueForKey( g_botInfos[i], "funname" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $339
ARGP4
ADDRLP4 4104
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ARGP4
ADDRLP4 4104
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 780
;780:		if ( !*funname ) {
ADDRLP4 1028
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $427
line 781
;781:			strcpy(funname, "");
ADDRLP4 1028
ARGP4
ADDRGP4 $92
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 782
;782:		}
LABELV $427
line 783
;783:		strcpy(model, Info_ValueForKey( g_botInfos[i], "model" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $305
ARGP4
ADDRLP4 4108
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2052
ARGP4
ADDRLP4 4108
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 784
;784:		if ( !*model ) {
ADDRLP4 2052
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $429
line 785
;785:			strcpy(model, "visor/default");
ADDRLP4 2052
ARGP4
ADDRGP4 $361
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 786
;786:		}
LABELV $429
line 787
;787:		strcpy(aifile, Info_ValueForKey( g_botInfos[i], "aifile"));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $380
ARGP4
ADDRLP4 4112
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3076
ARGP4
ADDRLP4 4112
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 788
;788:		if (!*aifile ) {
ADDRLP4 3076
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $431
line 789
;789:			strcpy(aifile, "bots/default_c.c");
ADDRLP4 3076
ARGP4
ADDRGP4 $433
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 790
;790:		}
LABELV $431
line 791
;791:		trap_Printf(va("%-16s %-16s %-20s %-20s\n", name, model, aifile, funname));
ADDRGP4 $434
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 2052
ARGP4
ADDRLP4 3076
ARGP4
ADDRLP4 1028
ARGP4
ADDRLP4 4116
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4116
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 792
;792:	}
LABELV $421
line 774
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $423
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $420
line 793
;793:}
LABELV $418
endproc Svcmd_BotList_f 4120 20
proc G_SpawnBots 1056 16
line 801
;794:
;795:
;796:/*
;797:===============
;798:G_SpawnBots
;799:===============
;800:*/
;801:static void G_SpawnBots( char *botList, int baseDelay ) {
line 808
;802:	char		*bot;
;803:	char		*p;
;804:	float		skill;
;805:	int			delay;
;806:	char		bots[MAX_INFO_VALUE];
;807:
;808:	podium1 = NULL;
ADDRGP4 podium1
CNSTP4 0
ASGNP4
line 809
;809:	podium2 = NULL;
ADDRGP4 podium2
CNSTP4 0
ASGNP4
line 810
;810:	podium3 = NULL;
ADDRGP4 podium3
CNSTP4 0
ASGNP4
line 812
;811:
;812:	skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $175
ARGP4
ADDRLP4 1040
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 1040
INDIRF4
ASGNF4
line 813
;813:	if( skill < 1 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1065353216
GEF4 $436
line 814
;814:		trap_Cvar_Set( "g_spSkill", "1" );
ADDRGP4 $175
ARGP4
ADDRGP4 $438
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 815
;815:		skill = 1;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 816
;816:	}
ADDRGP4 $437
JUMPV
LABELV $436
line 817
;817:	else if ( skill > 5 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1084227584
LEF4 $439
line 818
;818:		trap_Cvar_Set( "g_spSkill", "5" );
ADDRGP4 $175
ARGP4
ADDRGP4 $379
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 819
;819:		skill = 5;
ADDRLP4 12
CNSTF4 1084227584
ASGNF4
line 820
;820:	}
LABELV $439
LABELV $437
line 822
;821:
;822:	Q_strncpyz( bots, botList, sizeof(bots) );
ADDRLP4 16
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 823
;823:	p = &bots[0];
ADDRLP4 0
ADDRLP4 16
ASGNP4
line 824
;824:	delay = baseDelay;
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 $442
JUMPV
line 825
;825:	while( *p ) {
LABELV $444
line 827
;826:		//skip spaces
;827:		while( *p && *p == ' ' ) {
line 828
;828:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 829
;829:		}
LABELV $445
line 827
ADDRLP4 1044
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $447
ADDRLP4 1044
INDIRI4
CNSTI4 32
EQI4 $444
LABELV $447
line 830
;830:		if( !p ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $448
line 831
;831:			break;
ADDRGP4 $443
JUMPV
LABELV $448
line 835
;832:		}
;833:
;834:		// mark start of bot name
;835:		bot = p;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $451
JUMPV
LABELV $450
line 838
;836:
;837:		// skip until space of null
;838:		while( *p && *p != ' ' ) {
line 839
;839:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 840
;840:		}
LABELV $451
line 838
ADDRLP4 1048
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
EQI4 $453
ADDRLP4 1048
INDIRI4
CNSTI4 32
NEI4 $450
LABELV $453
line 841
;841:		if( *p ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $454
line 842
;842:			*p++ = 0;
ADDRLP4 1052
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1052
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
CNSTI1 0
ASGNI1
line 843
;843:		}
LABELV $454
line 847
;844:
;845:		// we must add the bot this way, calling G_AddBot directly at this stage
;846:		// does "Bad Things"
;847:		trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s %f free %i\n", bot, skill, delay) );
ADDRGP4 $456
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 1052
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 849
;848:
;849:		delay += BOT_BEGIN_DELAY_INCREMENT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 850
;850:	}
LABELV $442
line 825
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $445
LABELV $443
line 851
;851:}
LABELV $435
endproc G_SpawnBots 1056 16
proc G_LoadBotsFromFile 8216 16
line 859
;852:
;853:
;854:/*
;855:===============
;856:G_LoadBotsFromFile
;857:===============
;858:*/
;859:static void G_LoadBotsFromFile( char *filename ) {
line 864
;860:	int				len;
;861:	fileHandle_t	f;
;862:	char			buf[MAX_BOTS_TEXT];
;863:
;864:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8200
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8200
INDIRI4
ASGNI4
line 865
;865:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $458
line 866
;866:		trap_Printf( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $86
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 867
;867:		return;
ADDRGP4 $457
JUMPV
LABELV $458
line 869
;868:	}
;869:	if ( len >= MAX_BOTS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $460
line 870
;870:		trap_Printf( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT ) );
ADDRGP4 $89
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 8192
ARGI4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 871
;871:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 872
;872:		return;
ADDRGP4 $457
JUMPV
LABELV $460
line 875
;873:	}
;874:
;875:	trap_FS_Read( buf, len, f );
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 876
;876:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 877
;877:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 879
;878:
;879:	g_numBots += G_ParseInfos( buf, MAX_BOTS - g_numBots, &g_botInfos[g_numBots] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 g_numBots
ASGNP4
ADDRLP4 8208
ADDRLP4 8204
INDIRP4
INDIRI4
ASGNI4
CNSTI4 1024
ADDRLP4 8208
INDIRI4
SUBI4
ARGI4
ADDRLP4 8208
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 G_ParseInfos
CALLI4
ASGNI4
ADDRLP4 8204
INDIRP4
ADDRLP4 8208
INDIRI4
ADDRLP4 8212
INDIRI4
ADDI4
ASGNI4
line 880
;880:}
LABELV $457
endproc G_LoadBotsFromFile 8216 16
proc G_LoadBots 1456 16
line 887
;881:
;882:/*
;883:===============
;884:G_LoadBots
;885:===============
;886:*/
;887:static void G_LoadBots( void ) {
line 896
;888:	vmCvar_t	botsFile;
;889:	int			numdirs;
;890:	char		filename[128];
;891:	char		dirlist[1024];
;892:	char*		dirptr;
;893:	int			i;
;894:	int			dirlen;
;895:
;896:	if ( !trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
ADDRGP4 $404
ARGP4
ADDRLP4 1440
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 1440
INDIRI4
CNSTI4 0
NEI4 $463
line 897
;897:		return;
ADDRGP4 $462
JUMPV
LABELV $463
line 900
;898:	}
;899:
;900:	g_numBots = 0;
ADDRGP4 g_numBots
CNSTI4 0
ASGNI4
line 902
;901:
;902:	trap_Cvar_Register( &botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 144
ARGP4
ADDRGP4 $465
ARGP4
ADDRGP4 $92
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 903
;903:	if( *botsFile.string ) {
ADDRLP4 144+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $466
line 904
;904:		G_LoadBotsFromFile(botsFile.string);
ADDRLP4 144+16
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 905
;905:	}
ADDRGP4 $467
JUMPV
LABELV $466
line 906
;906:	else {
line 907
;907:		G_LoadBotsFromFile("scripts/bots.txt");
ADDRGP4 $470
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 908
;908:	}
LABELV $467
line 911
;909:
;910:	// get all bots from .bot files
;911:	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024 );
ADDRGP4 $98
ARGP4
ADDRGP4 $471
ARGP4
ADDRLP4 416
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1444
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1444
INDIRI4
ASGNI4
line 912
;912:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 416
ASGNP4
line 913
;913:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $475
JUMPV
LABELV $472
line 914
;914:		dirlen = strlen(dirptr);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1448
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 136
ADDRLP4 1448
INDIRI4
ASGNI4
line 915
;915:		strcpy(filename, "scripts/");
ADDRLP4 4
ARGP4
ADDRGP4 $104
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 916
;916:		strcat(filename, dirptr);
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 917
;917:		G_LoadBotsFromFile(filename);
ADDRLP4 4
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 918
;918:	}
LABELV $473
line 913
ADDRLP4 1448
CNSTI4 1
ASGNI4
ADDRLP4 132
ADDRLP4 132
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 136
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
LABELV $475
ADDRLP4 132
INDIRI4
ADDRLP4 140
INDIRI4
LTI4 $472
line 919
;919:	trap_Printf( va( "%i bots parsed\n", g_numBots ) );
ADDRGP4 $476
ARGP4
ADDRGP4 g_numBots
INDIRI4
ARGI4
ADDRLP4 1452
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1452
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 920
;920:}
LABELV $462
endproc G_LoadBots 1456 16
export G_GetBotInfoByNumber
proc G_GetBotInfoByNumber 8 8
line 929
;921:
;922:
;923:
;924:/*
;925:===============
;926:G_GetBotInfoByNumber
;927:===============
;928:*/
;929:char *G_GetBotInfoByNumber( int num ) {
line 930
;930:	if( num < 0 || num >= g_numBots ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $480
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $478
LABELV $480
line 931
;931:		trap_Printf( va( S_COLOR_RED "Invalid bot number: %i\n", num ) );
ADDRGP4 $481
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 932
;932:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $477
JUMPV
LABELV $478
line 934
;933:	}
;934:	return g_botInfos[num];
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
RETP4
LABELV $477
endproc G_GetBotInfoByNumber 8 8
export G_GetBotInfoByName
proc G_GetBotInfoByName 16 8
line 943
;935:}
;936:
;937:
;938:/*
;939:===============
;940:G_GetBotInfoByName
;941:===============
;942:*/
;943:char *G_GetBotInfoByName( const char *name ) {
line 947
;944:	int		n;
;945:	char	*value;
;946:
;947:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $486
JUMPV
LABELV $483
line 948
;948:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $132
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 949
;949:		if ( !Q_stricmp( value, name ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $487
line 950
;950:			return g_botInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $482
JUMPV
LABELV $487
line 952
;951:		}
;952:	}
LABELV $484
line 947
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $486
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $483
line 954
;953:
;954:	return NULL;
CNSTP4 0
RETP4
LABELV $482
endproc G_GetBotInfoByName 16 8
export G_InitBots
proc G_InitBots 1148 16
line 962
;955:}
;956:
;957:/*
;958:===============
;959:G_InitBots
;960:===============
;961:*/
;962:void G_InitBots( qboolean restart ) {
line 971
;963:	int			fragLimit;
;964:	int			timeLimit;
;965:	const char	*arenainfo;
;966:	char		*strValue;
;967:	int			basedelay;
;968:	char		map[MAX_QPATH];
;969:	char		serverinfo[MAX_INFO_STRING];
;970:
;971:	G_LoadBots();
ADDRGP4 G_LoadBots
CALLV
pop
line 972
;972:	G_LoadArenas();
ADDRGP4 G_LoadArenas
CALLV
pop
line 974
;973:
;974:	trap_Cvar_Register( &bot_minplayers, "bot_minplayers", "0", CVAR_SERVERINFO );
ADDRGP4 bot_minplayers
ARGP4
ADDRGP4 $490
ARGP4
ADDRGP4 $491
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 976
;975:
;976:	if( g_gametype.integer == GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $492
line 977
;977:		trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
ADDRLP4 80
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetServerinfo
CALLV
pop
line 978
;978:		Q_strncpyz( map, Info_ValueForKey( serverinfo, "mapname" ), sizeof(map) );
ADDRLP4 80
ARGP4
ADDRGP4 $495
ARGP4
ADDRLP4 1108
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 979
;979:		arenainfo = G_GetArenaInfoByMap( map );
ADDRLP4 16
ARGP4
ADDRLP4 1112
ADDRGP4 G_GetArenaInfoByMap
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 1112
INDIRP4
ASGNP4
line 980
;980:		if ( !arenainfo ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $496
line 981
;981:			return;
ADDRGP4 $489
JUMPV
LABELV $496
line 984
;982:		}
;983:
;984:		strValue = Info_ValueForKey( arenainfo, "fraglimit" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $498
ARGP4
ADDRLP4 1116
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1116
INDIRP4
ASGNP4
line 985
;985:		fragLimit = atoi( strValue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1120
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 1120
INDIRI4
ASGNI4
line 986
;986:		if ( fragLimit ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $499
line 987
;987:			trap_Cvar_Set( "fraglimit", strValue );
ADDRGP4 $498
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 988
;988:		}
ADDRGP4 $500
JUMPV
LABELV $499
line 989
;989:		else {
line 990
;990:			trap_Cvar_Set( "fraglimit", "0" );
ADDRGP4 $498
ARGP4
ADDRGP4 $491
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 991
;991:		}
LABELV $500
line 993
;992:
;993:		strValue = Info_ValueForKey( arenainfo, "timelimit" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $501
ARGP4
ADDRLP4 1124
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1124
INDIRP4
ASGNP4
line 994
;994:		timeLimit = atoi( strValue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1128
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 1128
INDIRI4
ASGNI4
line 995
;995:		if ( timeLimit ) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $502
line 996
;996:			trap_Cvar_Set( "timelimit", strValue );
ADDRGP4 $501
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 997
;997:		}
ADDRGP4 $503
JUMPV
LABELV $502
line 998
;998:		else {
line 999
;999:			trap_Cvar_Set( "timelimit", "0" );
ADDRGP4 $501
ARGP4
ADDRGP4 $491
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1000
;1000:		}
LABELV $503
line 1002
;1001:
;1002:		if ( !fragLimit && !timeLimit ) {
ADDRLP4 1132
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRI4
ADDRLP4 1132
INDIRI4
NEI4 $504
ADDRLP4 12
INDIRI4
ADDRLP4 1132
INDIRI4
NEI4 $504
line 1003
;1003:			trap_Cvar_Set( "fraglimit", "10" );
ADDRGP4 $498
ARGP4
ADDRGP4 $506
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1004
;1004:			trap_Cvar_Set( "timelimit", "0" );
ADDRGP4 $501
ARGP4
ADDRGP4 $491
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1005
;1005:		}
LABELV $504
line 1007
;1006:
;1007:		basedelay = BOT_BEGIN_DELAY_BASE;
ADDRLP4 1104
CNSTI4 2000
ASGNI4
line 1008
;1008:		strValue = Info_ValueForKey( arenainfo, "special" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $507
ARGP4
ADDRLP4 1136
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1136
INDIRP4
ASGNP4
line 1009
;1009:		if( Q_stricmp( strValue, "training" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $510
ARGP4
ADDRLP4 1140
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1140
INDIRI4
CNSTI4 0
NEI4 $508
line 1010
;1010:			basedelay += 10000;
ADDRLP4 1104
ADDRLP4 1104
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 1011
;1011:		}
LABELV $508
line 1013
;1012:
;1013:		if( !restart ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $511
line 1014
;1014:			G_SpawnBots( Info_ValueForKey( arenainfo, "bots" ), basedelay );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $513
ARGP4
ADDRLP4 1144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1104
INDIRI4
ARGI4
ADDRGP4 G_SpawnBots
CALLV
pop
line 1015
;1015:		}
LABELV $511
line 1016
;1016:	}
LABELV $492
line 1017
;1017:}
LABELV $489
endproc G_InitBots 1148 16
import podium3
import podium2
import podium1
bss
export bot_minplayers
align 4
LABELV bot_minplayers
skip 272
align 4
LABELV botSpawnQueue
skip 128
align 4
LABELV g_arenaInfos
skip 4096
export g_numArenas
align 4
LABELV g_numArenas
skip 4
align 4
LABELV g_botInfos
skip 4096
align 4
LABELV g_numBots
skip 4
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_Seek
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_proxMineTimeout
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_cubeTimeout
import g_obeliskRespawnDelay
import g_obeliskRegenAmount
import g_obeliskRegenPeriod
import g_obeliskHealth
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlyFire
import g_capturelimit
import g_timelimit
import g_fraglimit
import g_dmflags
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotTestAAS
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import BotInterbreedEndMatch
import Svcmd_AbortPodium_f
import SpawnModelsOnVictoryPads
import UpdateTournamentInfo
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import CheckObeliskAttack
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import ClientCommand
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import FireWeapon
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
import SpotWouldTelefrag
import CalculateRanks
import AddScore
import player_die
import ClientSpawn
import InitBodyQue
import InitClientResp
import InitClientPersistant
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
import Weapon_HookThink
import Weapon_HookFree
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import fire_grapple
import fire_bfg
import fire_rocket
import fire_grenade
import fire_plasma
import fire_blaster
import G_RunMissile
import TossClientCubes
import TossClientItems
import body_die
import G_InvulnerabilityEffect
import G_RadiusDamage
import G_Damage
import CanDamage
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vectoyaw
import vtos
import tv
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_Sound
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_Find
import G_KillBox
import G_TeamCommand
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
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
LABELV $513
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $510
byte 1 116
byte 1 114
byte 1 97
byte 1 105
byte 1 110
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $507
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 105
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $506
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $501
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
LABELV $498
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
LABELV $495
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $491
byte 1 48
byte 1 0
align 1
LABELV $490
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 109
byte 1 105
byte 1 110
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $481
byte 1 94
byte 1 49
byte 1 73
byte 1 110
byte 1 118
byte 1 97
byte 1 108
byte 1 105
byte 1 100
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 110
byte 1 117
byte 1 109
byte 1 98
byte 1 101
byte 1 114
byte 1 58
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $476
byte 1 37
byte 1 105
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 32
byte 1 112
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $471
byte 1 46
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $470
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 115
byte 1 47
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $465
byte 1 103
byte 1 95
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $456
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
byte 1 102
byte 1 32
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $438
byte 1 49
byte 1 0
align 1
LABELV $434
byte 1 37
byte 1 45
byte 1 49
byte 1 54
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 49
byte 1 54
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 32
byte 1 37
byte 1 45
byte 1 50
byte 1 48
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $433
byte 1 98
byte 1 111
byte 1 116
byte 1 115
byte 1 47
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 95
byte 1 99
byte 1 46
byte 1 99
byte 1 0
align 1
LABELV $426
byte 1 85
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 100
byte 1 80
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $419
byte 1 94
byte 1 49
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 97
byte 1 105
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 32
byte 1 102
byte 1 117
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $417
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 100
byte 1 101
byte 1 102
byte 1 101
byte 1 114
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $416
byte 1 99
byte 1 108
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
LABELV $407
byte 1 85
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 65
byte 1 100
byte 1 100
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 60
byte 1 98
byte 1 111
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 62
byte 1 32
byte 1 91
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 32
byte 1 49
byte 1 45
byte 1 53
byte 1 93
byte 1 32
byte 1 91
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 93
byte 1 32
byte 1 91
byte 1 109
byte 1 115
byte 1 101
byte 1 99
byte 1 32
byte 1 100
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 93
byte 1 32
byte 1 91
byte 1 97
byte 1 108
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 93
byte 1 10
byte 1 0
align 1
LABELV $404
byte 1 98
byte 1 111
byte 1 116
byte 1 95
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $396
byte 1 37
byte 1 53
byte 1 46
byte 1 50
byte 1 102
byte 1 0
align 1
LABELV $387
byte 1 94
byte 1 49
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 32
byte 1 115
byte 1 101
byte 1 114
byte 1 118
byte 1 101
byte 1 114
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 109
byte 1 111
byte 1 114
byte 1 101
byte 1 32
byte 1 39
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 39
byte 1 32
byte 1 115
byte 1 108
byte 1 111
byte 1 116
byte 1 115
byte 1 32
byte 1 40
byte 1 111
byte 1 114
byte 1 32
byte 1 99
byte 1 104
byte 1 101
byte 1 99
byte 1 107
byte 1 32
byte 1 115
byte 1 101
byte 1 116
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 111
byte 1 102
byte 1 32
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
byte 1 32
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 41
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $386
byte 1 94
byte 1 49
byte 1 85
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 97
byte 1 100
byte 1 100
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 46
byte 1 32
byte 1 32
byte 1 65
byte 1 108
byte 1 108
byte 1 32
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 32
byte 1 115
byte 1 108
byte 1 111
byte 1 116
byte 1 115
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $383
byte 1 94
byte 1 49
byte 1 69
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 58
byte 1 32
byte 1 98
byte 1 111
byte 1 116
byte 1 32
byte 1 104
byte 1 97
byte 1 115
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 97
byte 1 105
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 105
byte 1 102
byte 1 105
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $380
byte 1 97
byte 1 105
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $379
byte 1 53
byte 1 0
align 1
LABELV $376
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 50
byte 1 0
align 1
LABELV $375
byte 1 52
byte 1 0
align 1
LABELV $372
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 49
byte 1 0
align 1
LABELV $371
byte 1 115
byte 1 101
byte 1 120
byte 1 0
align 1
LABELV $370
byte 1 109
byte 1 97
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $367
byte 1 103
byte 1 101
byte 1 110
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $366
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $363
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $362
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $361
byte 1 118
byte 1 105
byte 1 115
byte 1 111
byte 1 114
byte 1 47
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $358
byte 1 57
byte 1 48
byte 1 0
align 1
LABELV $355
byte 1 55
byte 1 48
byte 1 0
align 1
LABELV $352
byte 1 53
byte 1 48
byte 1 0
align 1
LABELV $351
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 105
byte 1 99
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $348
byte 1 37
byte 1 49
byte 1 46
byte 1 50
byte 1 102
byte 1 0
align 1
LABELV $347
byte 1 50
byte 1 48
byte 1 0
align 1
LABELV $346
byte 1 115
byte 1 110
byte 1 97
byte 1 112
byte 1 115
byte 1 0
align 1
LABELV $345
byte 1 50
byte 1 53
byte 1 48
byte 1 48
byte 1 48
byte 1 0
align 1
LABELV $344
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 0
align 1
LABELV $339
byte 1 102
byte 1 117
byte 1 110
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $338
byte 1 94
byte 1 49
byte 1 69
byte 1 114
byte 1 114
byte 1 111
byte 1 114
byte 1 58
byte 1 32
byte 1 66
byte 1 111
byte 1 116
byte 1 32
byte 1 39
byte 1 37
byte 1 115
byte 1 39
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 100
byte 1 101
byte 1 102
byte 1 105
byte 1 110
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $334
byte 1 66
byte 1 111
byte 1 116
byte 1 65
byte 1 73
byte 1 83
byte 1 101
byte 1 116
byte 1 117
byte 1 112
byte 1 67
byte 1 108
byte 1 105
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 102
byte 1 97
byte 1 105
byte 1 108
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $330
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $328
byte 1 115
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $326
byte 1 99
byte 1 104
byte 1 97
byte 1 114
byte 1 97
byte 1 99
byte 1 116
byte 1 101
byte 1 114
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $316
byte 1 94
byte 1 51
byte 1 85
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 100
byte 1 101
byte 1 108
byte 1 97
byte 1 121
byte 1 32
byte 1 115
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 10
byte 1 0
align 1
LABELV $305
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $198
byte 1 107
byte 1 105
byte 1 99
byte 1 107
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $183
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
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $181
byte 1 98
byte 1 108
byte 1 117
byte 1 101
byte 1 0
align 1
LABELV $178
byte 1 114
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $175
byte 1 103
byte 1 95
byte 1 115
byte 1 112
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $132
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $126
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 32
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 47
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 47
byte 1 97
byte 1 110
byte 1 110
byte 1 111
byte 1 117
byte 1 110
byte 1 99
byte 1 101
byte 1 47
byte 1 37
byte 1 115
byte 1 46
byte 1 119
byte 1 97
byte 1 118
byte 1 10
byte 1 0
align 1
LABELV $125
byte 1 100
byte 1 101
byte 1 102
byte 1 97
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $119
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $111
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $110
byte 1 110
byte 1 117
byte 1 109
byte 1 0
align 1
LABELV $105
byte 1 37
byte 1 105
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 32
byte 1 112
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $104
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 115
byte 1 47
byte 1 0
align 1
LABELV $99
byte 1 46
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $98
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $97
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 115
byte 1 47
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 46
byte 1 116
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $92
byte 1 0
align 1
LABELV $91
byte 1 103
byte 1 95
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 70
byte 1 105
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $89
byte 1 94
byte 1 49
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 116
byte 1 111
byte 1 111
byte 1 32
byte 1 108
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 44
byte 1 32
byte 1 109
byte 1 97
byte 1 120
byte 1 32
byte 1 97
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $86
byte 1 94
byte 1 49
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $80
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $79
byte 1 92
byte 1 110
byte 1 117
byte 1 109
byte 1 92
byte 1 0
align 1
LABELV $78
byte 1 60
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 62
byte 1 0
align 1
LABELV $75
byte 1 125
byte 1 0
align 1
LABELV $72
byte 1 85
byte 1 110
byte 1 101
byte 1 120
byte 1 112
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 101
byte 1 110
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $66
byte 1 77
byte 1 97
byte 1 120
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 115
byte 1 32
byte 1 101
byte 1 120
byte 1 99
byte 1 101
byte 1 101
byte 1 100
byte 1 101
byte 1 100
byte 1 10
byte 1 0
align 1
LABELV $63
byte 1 77
byte 1 105
byte 1 115
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 123
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $62
byte 1 123
byte 1 0
