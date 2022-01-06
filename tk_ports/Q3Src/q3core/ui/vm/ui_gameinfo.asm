export UI_ParseInfos
code
proc UI_ParseInfos 2084 12
file "../ui_gameinfo.c"
line 51
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
;23://
;24:// gameinfo.c
;25://
;26:
;27:#include "ui_local.h"
;28:
;29:
;30://
;31:// arena and bot info
;32://
;33:
;34:
;35:int				ui_numBots;
;36:static char		*ui_botInfos[MAX_BOTS];
;37:
;38:static int		ui_numArenas;
;39:static char		*ui_arenaInfos[MAX_ARENAS];
;40:
;41:#ifndef MISSIONPACK // bk001206
;42:static int		ui_numSinglePlayerArenas;
;43:static int		ui_numSpecialSinglePlayerArenas;
;44:#endif
;45:
;46:/*
;47:===============
;48:UI_ParseInfos
;49:===============
;50:*/
;51:int UI_ParseInfos( char *buf, int max, char *infos[] ) {
line 57
;52:	char	*token;
;53:	int		count;
;54:	char	key[MAX_TOKEN_CHARS];
;55:	char	info[MAX_INFO_STRING];
;56:
;57:	count = 0;
ADDRLP4 2052
CNSTI4 0
ASGNI4
ADDRGP4 $89
JUMPV
LABELV $88
line 59
;58:
;59:	while ( 1 ) {
line 60
;60:		token = COM_Parse( &buf );
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
line 61
;61:		if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $91
line 62
;62:			break;
ADDRGP4 $90
JUMPV
LABELV $91
line 64
;63:		}
;64:		if ( strcmp( token, "{" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $95
ARGP4
ADDRLP4 2060
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2060
INDIRI4
CNSTI4 0
EQI4 $93
line 65
;65:			Com_Printf( "Missing { in info file\n" );
ADDRGP4 $96
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 66
;66:			break;
ADDRGP4 $90
JUMPV
LABELV $93
line 69
;67:		}
;68:
;69:		if ( count == max ) {
ADDRLP4 2052
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $97
line 70
;70:			Com_Printf( "Max infos exceeded\n" );
ADDRGP4 $99
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 71
;71:			break;
ADDRGP4 $90
JUMPV
LABELV $97
line 74
;72:		}
;73:
;74:		info[0] = '\0';
ADDRLP4 1028
CNSTI1 0
ASGNI1
ADDRGP4 $101
JUMPV
LABELV $100
line 75
;75:		while ( 1 ) {
line 76
;76:			token = COM_ParseExt( &buf, qtrue );
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
line 77
;77:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $103
line 78
;78:				Com_Printf( "Unexpected end of info file\n" );
ADDRGP4 $105
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 79
;79:				break;
ADDRGP4 $102
JUMPV
LABELV $103
line 81
;80:			}
;81:			if ( !strcmp( token, "}" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $108
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $106
line 82
;82:				break;
ADDRGP4 $102
JUMPV
LABELV $106
line 84
;83:			}
;84:			Q_strncpyz( key, token, sizeof( key ) );
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
line 86
;85:
;86:			token = COM_ParseExt( &buf, qfalse );
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
line 87
;87:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $109
line 88
;88:				strcpy( token, "<NULL>" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $111
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 89
;89:			}
LABELV $109
line 90
;90:			Info_SetValueForKey( info, key, token );
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
line 91
;91:		}
LABELV $101
line 75
ADDRGP4 $100
JUMPV
LABELV $102
line 93
;92:		//NOTE: extra space for arena number
;93:		infos[count] = UI_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
ADDRLP4 1028
ARGP4
ADDRLP4 2064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $112
ARGP4
ADDRLP4 2068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $113
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
ADDRGP4 UI_Alloc
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
line 94
;94:		if (infos[count]) {
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
EQU4 $114
line 95
;95:			strcpy(infos[count], info);
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
line 96
;96:			count++;
ADDRLP4 2052
ADDRLP4 2052
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 97
;97:		}
LABELV $114
line 98
;98:	}
LABELV $89
line 59
ADDRGP4 $88
JUMPV
LABELV $90
line 99
;99:	return count;
ADDRLP4 2052
INDIRI4
RETI4
LABELV $87
endproc UI_ParseInfos 2084 12
proc UI_LoadArenasFromFile 8216 16
line 107
;100:}
;101:
;102:/*
;103:===============
;104:UI_LoadArenasFromFile
;105:===============
;106:*/
;107:static void UI_LoadArenasFromFile( char *filename ) {
line 112
;108:	int				len;
;109:	fileHandle_t	f;
;110:	char			buf[MAX_ARENAS_TEXT];
;111:
;112:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
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
line 113
;113:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $117
line 114
;114:		trap_Print( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $119
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
ADDRGP4 trap_Print
CALLV
pop
line 115
;115:		return;
ADDRGP4 $116
JUMPV
LABELV $117
line 117
;116:	}
;117:	if ( len >= MAX_ARENAS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $120
line 118
;118:		trap_Print( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT ) );
ADDRGP4 $122
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
ADDRGP4 trap_Print
CALLV
pop
line 119
;119:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 120
;120:		return;
ADDRGP4 $116
JUMPV
LABELV $120
line 123
;121:	}
;122:
;123:	trap_FS_Read( buf, len, f );
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
line 124
;124:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 125
;125:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 127
;126:
;127:	ui_numArenas += UI_ParseInfos( buf, MAX_ARENAS - ui_numArenas, &ui_arenaInfos[ui_numArenas] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 ui_numArenas
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
ADDRGP4 ui_arenaInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 UI_ParseInfos
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
line 128
;128:}
LABELV $116
endproc UI_LoadArenasFromFile 8216 16
export UI_LoadArenas
proc UI_LoadArenas 1524 16
line 135
;129:
;130:/*
;131:===============
;132:UI_LoadArenas
;133:===============
;134:*/
;135:void UI_LoadArenas( void ) {
line 145
;136:	int			numdirs;
;137:	vmCvar_t	arenasFile;
;138:	char		filename[128];
;139:	char		dirlist[1024];
;140:	char*		dirptr;
;141:	int			i, n;
;142:	int			dirlen;
;143:	char		*type;
;144:
;145:	ui_numArenas = 0;
ADDRGP4 ui_numArenas
CNSTI4 0
ASGNI4
line 146
;146:	uiInfo.mapCount = 0;
ADDRGP4 uiInfo+83224
CNSTI4 0
ASGNI4
line 148
;147:
;148:	trap_Cvar_Register( &arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 152
ARGP4
ADDRGP4 $125
ARGP4
ADDRGP4 $126
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 149
;149:	if( *arenasFile.string ) {
ADDRLP4 152+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $127
line 150
;150:		UI_LoadArenasFromFile(arenasFile.string);
ADDRLP4 152+16
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 151
;151:	}
ADDRGP4 $128
JUMPV
LABELV $127
line 152
;152:	else {
line 153
;153:		UI_LoadArenasFromFile("scripts/arenas.txt");
ADDRGP4 $131
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 154
;154:	}
LABELV $128
line 157
;155:
;156:	// get all arenas from .arena files
;157:	numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, 1024 );
ADDRGP4 $132
ARGP4
ADDRGP4 $133
ARGP4
ADDRLP4 424
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1448
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 148
ADDRLP4 1448
INDIRI4
ASGNI4
line 158
;158:	dirptr  = dirlist;
ADDRLP4 8
ADDRLP4 424
ASGNP4
line 159
;159:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRGP4 $137
JUMPV
LABELV $134
line 160
;160:		dirlen = strlen(dirptr);
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1452
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 144
ADDRLP4 1452
INDIRI4
ASGNI4
line 161
;161:		strcpy(filename, "scripts/");
ADDRLP4 12
ARGP4
ADDRGP4 $138
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 162
;162:		strcat(filename, dirptr);
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 163
;163:		UI_LoadArenasFromFile(filename);
ADDRLP4 12
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 164
;164:	}
LABELV $135
line 159
ADDRLP4 1452
CNSTI4 1
ASGNI4
ADDRLP4 140
ADDRLP4 140
INDIRI4
ADDRLP4 1452
INDIRI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 144
INDIRI4
ADDRLP4 1452
INDIRI4
ADDI4
ADDRLP4 8
INDIRP4
ADDP4
ASGNP4
LABELV $137
ADDRLP4 140
INDIRI4
ADDRLP4 148
INDIRI4
LTI4 $134
line 165
;165:	trap_Print( va( "%i arenas parsed\n", ui_numArenas ) );
ADDRGP4 $139
ARGP4
ADDRGP4 ui_numArenas
INDIRI4
ARGI4
ADDRLP4 1456
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1456
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 166
;166:	if (UI_OutOfMemory()) {
ADDRLP4 1460
ADDRGP4 UI_OutOfMemory
CALLI4
ASGNI4
ADDRLP4 1460
INDIRI4
CNSTI4 0
EQI4 $140
line 167
;167:		trap_Print(S_COLOR_YELLOW"WARNING: not anough memory in pool to load all arenas\n");
ADDRGP4 $142
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 168
;168:	}
LABELV $140
line 170
;169:
;170:	for( n = 0; n < ui_numArenas; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $146
JUMPV
LABELV $143
line 173
;171:		// determine type
;172:
;173:		uiInfo.mapList[uiInfo.mapCount].cinematic = -1;
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+24
ADDP4
CNSTI4 -1
ASGNI4
line 174
;174:		uiInfo.mapList[uiInfo.mapCount].mapLoadName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "map"));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $153
ARGP4
ADDRLP4 1464
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1464
INDIRP4
ARGP4
ADDRLP4 1468
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
ADDRLP4 1468
INDIRP4
ASGNP4
line 175
;175:		uiInfo.mapList[uiInfo.mapCount].mapName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "longname"));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $156
ARGP4
ADDRLP4 1472
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1472
INDIRP4
ARGP4
ADDRLP4 1476
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228
ADDP4
ADDRLP4 1476
INDIRP4
ASGNP4
line 176
;176:		uiInfo.mapList[uiInfo.mapCount].levelShot = -1;
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+92
ADDP4
CNSTI4 -1
ASGNI4
line 177
;177:		uiInfo.mapList[uiInfo.mapCount].imageName = String_Alloc(va("levelshots/%s", uiInfo.mapList[uiInfo.mapCount].mapLoadName));
ADDRGP4 $163
ARGP4
ADDRLP4 1480
CNSTI4 100
ASGNI4
ADDRLP4 1480
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1484
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1484
INDIRP4
ARGP4
ADDRLP4 1488
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 1480
INDIRI4
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+8
ADDP4
ADDRLP4 1488
INDIRP4
ASGNP4
line 178
;178:		uiInfo.mapList[uiInfo.mapCount].typeBits = 0;
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
CNSTI4 0
ASGNI4
line 180
;179:
;180:		type = Info_ValueForKey( ui_arenaInfos[n], "type" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $170
ARGP4
ADDRLP4 1492
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 1492
INDIRP4
ASGNP4
line 182
;181:		// if no type specified, it will be treated as "ffa"
;182:		if( *type ) {
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $171
line 183
;183:			if( strstr( type, "ffa" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $175
ARGP4
ADDRLP4 1496
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1496
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $173
line 184
;184:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_FFA);
ADDRLP4 1500
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1500
INDIRP4
ADDRLP4 1500
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 185
;185:			}
LABELV $173
line 186
;186:			if( strstr( type, "tourney" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $181
ARGP4
ADDRLP4 1500
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1500
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $179
line 187
;187:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_TOURNAMENT);
ADDRLP4 1504
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1504
INDIRP4
ADDRLP4 1504
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 188
;188:			}
LABELV $179
line 189
;189:			if( strstr( type, "ctf" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $187
ARGP4
ADDRLP4 1504
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1504
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $185
line 190
;190:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_CTF);
ADDRLP4 1508
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1508
INDIRP4
ADDRLP4 1508
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 191
;191:			}
LABELV $185
line 192
;192:			if( strstr( type, "oneflag" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $193
ARGP4
ADDRLP4 1508
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1508
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $191
line 193
;193:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_1FCTF);
ADDRLP4 1512
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1512
INDIRP4
ADDRLP4 1512
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 194
;194:			}
LABELV $191
line 195
;195:			if( strstr( type, "overload" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $199
ARGP4
ADDRLP4 1512
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1512
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $197
line 196
;196:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_OBELISK);
ADDRLP4 1516
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1516
INDIRP4
ADDRLP4 1516
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 197
;197:			}
LABELV $197
line 198
;198:			if( strstr( type, "harvester" ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $205
ARGP4
ADDRLP4 1516
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1516
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $172
line 199
;199:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_HARVESTER);
ADDRLP4 1520
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1520
INDIRP4
ADDRLP4 1520
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 200
;200:			}
line 201
;201:		} else {
ADDRGP4 $172
JUMPV
LABELV $171
line 202
;202:			uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_FFA);
ADDRLP4 1496
CNSTI4 100
ADDRGP4 uiInfo+83224
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+20
ADDP4
ASGNP4
ADDRLP4 1496
INDIRP4
ADDRLP4 1496
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 203
;203:		}
LABELV $172
line 205
;204:
;205:		uiInfo.mapCount++;
ADDRLP4 1496
ADDRGP4 uiInfo+83224
ASGNP4
ADDRLP4 1496
INDIRP4
ADDRLP4 1496
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 206
;206:		if (uiInfo.mapCount >= MAX_MAPS) {
ADDRGP4 uiInfo+83224
INDIRI4
CNSTI4 128
LTI4 $213
line 207
;207:			break;
ADDRGP4 $145
JUMPV
LABELV $213
line 209
;208:		}
;209:	}
LABELV $144
line 170
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $146
ADDRLP4 0
INDIRI4
ADDRGP4 ui_numArenas
INDIRI4
LTI4 $143
LABELV $145
line 210
;210:}
LABELV $123
endproc UI_LoadArenas 1524 16
proc UI_LoadBotsFromFile 8216 16
line 218
;211:
;212:
;213:/*
;214:===============
;215:UI_LoadBotsFromFile
;216:===============
;217:*/
;218:static void UI_LoadBotsFromFile( char *filename ) {
line 223
;219:	int				len;
;220:	fileHandle_t	f;
;221:	char			buf[MAX_BOTS_TEXT];
;222:
;223:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
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
line 224
;224:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $217
line 225
;225:		trap_Print( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $119
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
ADDRGP4 trap_Print
CALLV
pop
line 226
;226:		return;
ADDRGP4 $216
JUMPV
LABELV $217
line 228
;227:	}
;228:	if ( len >= MAX_BOTS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $219
line 229
;229:		trap_Print( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT ) );
ADDRGP4 $122
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
ADDRGP4 trap_Print
CALLV
pop
line 230
;230:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 231
;231:		return;
ADDRGP4 $216
JUMPV
LABELV $219
line 234
;232:	}
;233:
;234:	trap_FS_Read( buf, len, f );
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
line 235
;235:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 236
;236:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 238
;237:
;238:	COM_Compress(buf);
ADDRLP4 8
ARGP4
ADDRGP4 COM_Compress
CALLI4
pop
line 240
;239:
;240:	ui_numBots += UI_ParseInfos( buf, MAX_BOTS - ui_numBots, &ui_botInfos[ui_numBots] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 ui_numBots
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
ADDRGP4 ui_botInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 UI_ParseInfos
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
line 241
;241:}
LABELV $216
endproc UI_LoadBotsFromFile 8216 16
export UI_LoadBots
proc UI_LoadBots 1452 16
line 248
;242:
;243:/*
;244:===============
;245:UI_LoadBots
;246:===============
;247:*/
;248:void UI_LoadBots( void ) {
line 257
;249:	vmCvar_t	botsFile;
;250:	int			numdirs;
;251:	char		filename[128];
;252:	char		dirlist[1024];
;253:	char*		dirptr;
;254:	int			i;
;255:	int			dirlen;
;256:
;257:	ui_numBots = 0;
ADDRGP4 ui_numBots
CNSTI4 0
ASGNI4
line 259
;258:
;259:	trap_Cvar_Register( &botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 144
ARGP4
ADDRGP4 $222
ARGP4
ADDRGP4 $126
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 260
;260:	if( *botsFile.string ) {
ADDRLP4 144+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $223
line 261
;261:		UI_LoadBotsFromFile(botsFile.string);
ADDRLP4 144+16
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 262
;262:	}
ADDRGP4 $224
JUMPV
LABELV $223
line 263
;263:	else {
line 264
;264:		UI_LoadBotsFromFile("scripts/bots.txt");
ADDRGP4 $227
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 265
;265:	}
LABELV $224
line 268
;266:
;267:	// get all bots from .bot files
;268:	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024 );
ADDRGP4 $132
ARGP4
ADDRGP4 $228
ARGP4
ADDRLP4 416
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1440
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1440
INDIRI4
ASGNI4
line 269
;269:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 416
ASGNP4
line 270
;270:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $232
JUMPV
LABELV $229
line 271
;271:		dirlen = strlen(dirptr);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1444
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 136
ADDRLP4 1444
INDIRI4
ASGNI4
line 272
;272:		strcpy(filename, "scripts/");
ADDRLP4 4
ARGP4
ADDRGP4 $138
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 273
;273:		strcat(filename, dirptr);
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 274
;274:		UI_LoadBotsFromFile(filename);
ADDRLP4 4
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 275
;275:	}
LABELV $230
line 270
ADDRLP4 1444
CNSTI4 1
ASGNI4
ADDRLP4 132
ADDRLP4 132
INDIRI4
ADDRLP4 1444
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 136
INDIRI4
ADDRLP4 1444
INDIRI4
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
LABELV $232
ADDRLP4 132
INDIRI4
ADDRLP4 140
INDIRI4
LTI4 $229
line 276
;276:	trap_Print( va( "%i bots parsed\n", ui_numBots ) );
ADDRGP4 $233
ARGP4
ADDRGP4 ui_numBots
INDIRI4
ARGI4
ADDRLP4 1448
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1448
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 277
;277:}
LABELV $221
endproc UI_LoadBots 1452 16
export UI_GetBotInfoByNumber
proc UI_GetBotInfoByNumber 8 8
line 285
;278:
;279:
;280:/*
;281:===============
;282:UI_GetBotInfoByNumber
;283:===============
;284:*/
;285:char *UI_GetBotInfoByNumber( int num ) {
line 286
;286:	if( num < 0 || num >= ui_numBots ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $237
ADDRLP4 0
INDIRI4
ADDRGP4 ui_numBots
INDIRI4
LTI4 $235
LABELV $237
line 287
;287:		trap_Print( va( S_COLOR_RED "Invalid bot number: %i\n", num ) );
ADDRGP4 $238
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
ADDRGP4 trap_Print
CALLV
pop
line 288
;288:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $234
JUMPV
LABELV $235
line 290
;289:	}
;290:	return ui_botInfos[num];
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
RETP4
LABELV $234
endproc UI_GetBotInfoByNumber 8 8
export UI_GetBotInfoByName
proc UI_GetBotInfoByName 16 8
line 299
;291:}
;292:
;293:
;294:/*
;295:===============
;296:UI_GetBotInfoByName
;297:===============
;298:*/
;299:char *UI_GetBotInfoByName( const char *name ) {
line 303
;300:	int		n;
;301:	char	*value;
;302:
;303:	for ( n = 0; n < ui_numBots ; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $243
JUMPV
LABELV $240
line 304
;304:		value = Info_ValueForKey( ui_botInfos[n], "name" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $244
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 305
;305:		if ( !Q_stricmp( value, name ) ) {
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
NEI4 $245
line 306
;306:			return ui_botInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $239
JUMPV
LABELV $245
line 308
;307:		}
;308:	}
LABELV $241
line 303
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $243
ADDRLP4 0
INDIRI4
ADDRGP4 ui_numBots
INDIRI4
LTI4 $240
line 310
;309:
;310:	return NULL;
CNSTP4 0
RETP4
LABELV $239
endproc UI_GetBotInfoByName 16 8
export UI_GetNumBots
proc UI_GetNumBots 0 0
line 313
;311:}
;312:
;313:int UI_GetNumBots() {
line 314
;314:	return ui_numBots;
ADDRGP4 ui_numBots
INDIRI4
RETI4
LABELV $247
endproc UI_GetNumBots 0 0
export UI_GetBotNameByNumber
proc UI_GetBotNameByNumber 12 8
line 318
;315:}
;316:
;317:
;318:char *UI_GetBotNameByNumber( int num ) {
line 319
;319:	char *info = UI_GetBotInfoByNumber(num);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 320
;320:	if (info) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $249
line 321
;321:		return Info_ValueForKey( info, "name" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $244
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
RETP4
ADDRGP4 $248
JUMPV
LABELV $249
line 323
;322:	}
;323:	return "Sarge";
ADDRGP4 $251
RETP4
LABELV $248
endproc UI_GetBotNameByNumber 12 8
bss
align 4
LABELV ui_arenaInfos
skip 4096
align 4
LABELV ui_numArenas
skip 4
align 4
LABELV ui_botInfos
skip 4096
export ui_numBots
align 4
LABELV ui_numBots
skip 4
import UI_RankStatusMenu
import RankStatus_Cache
import UI_SignupMenu
import Signup_Cache
import UI_LoginMenu
import Login_Cache
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
import trap_R_RemapShader
import trap_RealTime
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_S_StartBackgroundTrack
import trap_S_StopBackgroundTrack
import trap_R_RegisterFont
import trap_SetCDKey
import trap_GetCDKey
import trap_MemoryRemaining
import trap_LAN_CompareServers
import trap_LAN_ServerStatus
import trap_LAN_ResetPings
import trap_LAN_RemoveServer
import trap_LAN_AddServer
import trap_LAN_UpdateVisiblePings
import trap_LAN_ServerIsVisible
import trap_LAN_MarkServerVisible
import trap_LAN_SaveCachedServers
import trap_LAN_LoadCachedServers
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_GetPingQueueCount
import trap_LAN_GetServerPing
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
import trap_R_ModelBounds
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
import UI_LoadBestScores
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
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_SetColor
import UI_UpdateScreen
import UI_DrawSides
import UI_DrawTopBottom
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
import uiInfo
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
import UI_ConfirmMenu
import ConfirmMenu_Cache
import UI_InGameMenu
import InGame_Cache
import UI_CreditMenu
import UI_UpdateCvars
import UI_RegisterCvars
import UI_MainMenu
import MainMenu_Cache
import UI_ClearScores
import UI_ShowPostGame
import UI_AdjustTimeByGame
import _UI_SetActiveMenu
import UI_LoadMenus
import UI_Load
import UI_Report
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
import ui_serverStatusTimeOut
import ui_bigFont
import ui_smallFont
import ui_scoreTime
import ui_scoreShutoutBonus
import ui_scoreSkillBonus
import ui_scoreTimeBonus
import ui_scoreBase
import ui_scoreTeam
import ui_scorePerfect
import ui_scoreScore
import ui_scoreGauntlets
import ui_scoreAssists
import ui_scoreDefends
import ui_scoreExcellents
import ui_scoreImpressives
import ui_scoreAccuracy
import ui_singlePlayerActive
import ui_lastServerRefresh_3
import ui_lastServerRefresh_2
import ui_lastServerRefresh_1
import ui_lastServerRefresh_0
import ui_selectedPlayerName
import ui_selectedPlayer
import ui_currentOpponent
import ui_mapIndex
import ui_currentNetMap
import ui_currentMap
import ui_currentTier
import ui_menuFiles
import ui_opponentName
import ui_dedicated
import ui_serverFilterType
import ui_netSource
import ui_joinGameType
import ui_actualNetGameType
import ui_netGameType
import ui_gameType
import ui_fragLimit
import ui_captureLimit
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
import trap_PC_SourceFileAndLine
import trap_PC_ReadToken
import trap_PC_FreeSource
import trap_PC_LoadSource
import trap_PC_AddGlobalDefine
import Controls_SetDefaults
import Controls_SetConfig
import Controls_GetConfig
import UI_OutOfMemory
import UI_InitMemory
import UI_Alloc
import Display_CacheAll
import Menu_SetFeederSelection
import Menu_Paint
import Menus_CloseAll
import LerpColor
import Display_HandleKey
import Menus_CloseByName
import Menus_ShowByName
import Menus_FindByName
import Menus_OpenByName
import Display_KeyBindPending
import Display_CursorType
import Display_MouseMove
import Display_CaptureItem
import Display_GetContext
import Menus_Activate
import Menus_AnyFullScreenVisible
import Menu_Reset
import Menus_ActivateByName
import Menu_PaintAll
import Menu_New
import Menu_Count
import PC_Script_Parse
import PC_String_Parse
import PC_Rect_Parse
import PC_Int_Parse
import PC_Color_Parse
import PC_Float_Parse
import Script_Parse
import String_Parse
import Rect_Parse
import Int_Parse
import Color_Parse
import Float_Parse
import Menu_ScrollFeeder
import Menu_HandleMouseMove
import Menu_HandleKey
import Menu_GetFocused
import Menu_PostParse
import Item_Init
import Menu_Init
import Display_ExpandMacros
import Init_Display
import String_Report
import String_Init
import String_Alloc
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
LABELV $251
byte 1 83
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $244
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $238
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
LABELV $233
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
LABELV $228
byte 1 46
byte 1 98
byte 1 111
byte 1 116
byte 1 0
align 1
LABELV $227
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
LABELV $222
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
LABELV $205
byte 1 104
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
LABELV $199
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $193
byte 1 111
byte 1 110
byte 1 101
byte 1 102
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $187
byte 1 99
byte 1 116
byte 1 102
byte 1 0
align 1
LABELV $181
byte 1 116
byte 1 111
byte 1 117
byte 1 114
byte 1 110
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $175
byte 1 102
byte 1 102
byte 1 97
byte 1 0
align 1
LABELV $170
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $163
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
LABELV $156
byte 1 108
byte 1 111
byte 1 110
byte 1 103
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $153
byte 1 109
byte 1 97
byte 1 112
byte 1 0
align 1
LABELV $142
byte 1 94
byte 1 51
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 116
byte 1 32
byte 1 97
byte 1 110
byte 1 111
byte 1 117
byte 1 103
byte 1 104
byte 1 32
byte 1 109
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 112
byte 1 111
byte 1 111
byte 1 108
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 32
byte 1 97
byte 1 108
byte 1 108
byte 1 32
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $139
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
LABELV $138
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
LABELV $133
byte 1 46
byte 1 97
byte 1 114
byte 1 101
byte 1 110
byte 1 97
byte 1 0
align 1
LABELV $132
byte 1 115
byte 1 99
byte 1 114
byte 1 105
byte 1 112
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $131
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
LABELV $126
byte 1 0
align 1
LABELV $125
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
LABELV $122
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
LABELV $119
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
LABELV $113
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $112
byte 1 92
byte 1 110
byte 1 117
byte 1 109
byte 1 92
byte 1 0
align 1
LABELV $111
byte 1 60
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 62
byte 1 0
align 1
LABELV $108
byte 1 125
byte 1 0
align 1
LABELV $105
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
LABELV $99
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
LABELV $96
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
LABELV $95
byte 1 123
byte 1 0
