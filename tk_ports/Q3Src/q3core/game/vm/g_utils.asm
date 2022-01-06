data
export remapCount
align 4
LABELV remapCount
byte 4 0
export AddRemap
code
proc AddRemap 8 8
file "../g_utils.c"
line 38
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
;23:// g_utils.c -- misc utility functions for game module
;24:
;25:#include "g_local.h"
;26:
;27:typedef struct {
;28:  char oldShader[MAX_QPATH];
;29:  char newShader[MAX_QPATH];
;30:  float timeOffset;
;31:} shaderRemap_t;
;32:
;33:#define MAX_SHADER_REMAPS 128
;34:
;35:int remapCount = 0;
;36:shaderRemap_t remappedShaders[MAX_SHADER_REMAPS];
;37:
;38:void AddRemap(const char *oldShader, const char *newShader, float timeOffset) {
line 41
;39:	int i;
;40:
;41:	for (i = 0; i < remapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $57
JUMPV
LABELV $54
line 42
;42:		if (Q_stricmp(oldShader, remappedShaders[i].oldShader) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $58
line 44
;43:			// found it, just update this one
;44:			strcpy(remappedShaders[i].newShader,newShader);
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 45
;45:			remappedShaders[i].timeOffset = timeOffset;
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders+128
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 46
;46:			return;
ADDRGP4 $53
JUMPV
LABELV $58
line 48
;47:		}
;48:	}
LABELV $55
line 41
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $57
ADDRLP4 0
INDIRI4
ADDRGP4 remapCount
INDIRI4
LTI4 $54
line 49
;49:	if (remapCount < MAX_SHADER_REMAPS) {
ADDRGP4 remapCount
INDIRI4
CNSTI4 128
GEI4 $62
line 50
;50:		strcpy(remappedShaders[remapCount].newShader,newShader);
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 51
;51:		strcpy(remappedShaders[remapCount].oldShader,oldShader);
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 52
;52:		remappedShaders[remapCount].timeOffset = timeOffset;
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders+128
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 53
;53:		remapCount++;
ADDRLP4 4
ADDRGP4 remapCount
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 54
;54:	}
LABELV $62
line 55
;55:}
LABELV $53
endproc AddRemap 8 8
bss
align 1
LABELV $67
skip 4096
export BuildShaderStateConfig
code
proc BuildShaderStateConfig 144 24
line 57
;56:
;57:const char *BuildShaderStateConfig() {
line 62
;58:	static char	buff[MAX_STRING_CHARS*4];
;59:	char out[(MAX_QPATH * 2) + 5];
;60:	int i;
;61:  
;62:	memset(buff, 0, MAX_STRING_CHARS);
ADDRGP4 $67
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 63
;63:	for (i = 0; i < remapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $71
JUMPV
LABELV $68
line 64
;64:		Com_sprintf(out, (MAX_QPATH * 2) + 5, "%s=%s:%5.2f@", remappedShaders[i].oldShader, remappedShaders[i].newShader, remappedShaders[i].timeOffset);
ADDRLP4 4
ARGP4
CNSTI4 133
ARGI4
ADDRGP4 $72
ARGP4
ADDRLP4 140
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders+128
ADDP4
INDIRF4
ARGF4
ADDRGP4 Com_sprintf
CALLV
pop
line 65
;65:		Q_strcat( buff, sizeof( buff ), out);
ADDRGP4 $67
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 66
;66:	}
LABELV $69
line 63
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $71
ADDRLP4 0
INDIRI4
ADDRGP4 remapCount
INDIRI4
LTI4 $68
line 67
;67:	return buff;
ADDRGP4 $67
RETP4
LABELV $66
endproc BuildShaderStateConfig 144 24
export G_FindConfigstringIndex
proc G_FindConfigstringIndex 1036 12
line 84
;68:}
;69:
;70:/*
;71:=========================================================================
;72:
;73:model / sound configstring indexes
;74:
;75:=========================================================================
;76:*/
;77:
;78:/*
;79:================
;80:G_FindConfigstringIndex
;81:
;82:================
;83:*/
;84:int G_FindConfigstringIndex( char *name, int start, int max, qboolean create ) {
line 88
;85:	int		i;
;86:	char	s[MAX_STRING_CHARS];
;87:
;88:	if ( !name || !name[0] ) {
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $78
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $76
LABELV $78
line 89
;89:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $75
JUMPV
LABELV $76
line 92
;90:	}
;91:
;92:	for ( i=1 ; i<max ; i++ ) {
ADDRLP4 1024
CNSTI4 1
ASGNI4
ADDRGP4 $82
JUMPV
LABELV $79
line 93
;93:		trap_GetConfigstring( start + i, s, sizeof( s ) );
ADDRFP4 4
INDIRI4
ADDRLP4 1024
INDIRI4
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 94
;94:		if ( !s[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $83
line 95
;95:			break;
ADDRGP4 $81
JUMPV
LABELV $83
line 97
;96:		}
;97:		if ( !strcmp( s, name ) ) {
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $85
line 98
;98:			return i;
ADDRLP4 1024
INDIRI4
RETI4
ADDRGP4 $75
JUMPV
LABELV $85
line 100
;99:		}
;100:	}
LABELV $80
line 92
ADDRLP4 1024
ADDRLP4 1024
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $82
ADDRLP4 1024
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $79
LABELV $81
line 102
;101:
;102:	if ( !create ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
NEI4 $87
line 103
;103:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $75
JUMPV
LABELV $87
line 106
;104:	}
;105:
;106:	if ( i == max ) {
ADDRLP4 1024
INDIRI4
ADDRFP4 8
INDIRI4
NEI4 $89
line 107
;107:		G_Error( "G_FindConfigstringIndex: overflow" );
ADDRGP4 $91
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 108
;108:	}
LABELV $89
line 110
;109:
;110:	trap_SetConfigstring( start + i, name );
ADDRFP4 4
INDIRI4
ADDRLP4 1024
INDIRI4
ADDI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 112
;111:
;112:	return i;
ADDRLP4 1024
INDIRI4
RETI4
LABELV $75
endproc G_FindConfigstringIndex 1036 12
export G_ModelIndex
proc G_ModelIndex 4 16
line 116
;113:}
;114:
;115:
;116:int G_ModelIndex( char *name ) {
line 117
;117:	return G_FindConfigstringIndex (name, CS_MODELS, MAX_MODELS, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
CNSTI4 256
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $92
endproc G_ModelIndex 4 16
export G_SoundIndex
proc G_SoundIndex 4 16
line 120
;118:}
;119:
;120:int G_SoundIndex( char *name ) {
line 121
;121:	return G_FindConfigstringIndex (name, CS_SOUNDS, MAX_SOUNDS, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 288
ARGI4
CNSTI4 256
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $93
endproc G_SoundIndex 4 16
export G_TeamCommand
proc G_TeamCommand 8 8
line 134
;122:}
;123:
;124://=====================================================================
;125:
;126:
;127:/*
;128:================
;129:G_TeamCommand
;130:
;131:Broadcasts a command to only a specific team
;132:================
;133:*/
;134:void G_TeamCommand( team_t team, char *cmd ) {
line 137
;135:	int		i;
;136:
;137:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $98
JUMPV
LABELV $95
line 138
;138:		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 2
NEI4 $100
line 139
;139:			if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 616
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $102
line 140
;140:				trap_SendServerCommand( i, va("%s", cmd ));
ADDRGP4 $104
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 141
;141:			}
LABELV $102
line 142
;142:		}
LABELV $100
line 143
;143:	}
LABELV $96
line 137
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $98
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $95
line 144
;144:}
LABELV $94
endproc G_TeamCommand 8 8
export G_Find
proc G_Find 8 8
line 160
;145:
;146:
;147:/*
;148:=============
;149:G_Find
;150:
;151:Searches all active entities for the next one that holds
;152:the matching string at fieldofs (use the FOFS() macro) in the structure.
;153:
;154:Searches beginning at the entity after from, or the beginning if NULL
;155:NULL will be returned if the end of the list is reached.
;156:
;157:=============
;158:*/
;159:gentity_t *G_Find (gentity_t *from, int fieldofs, const char *match)
;160:{
line 163
;161:	char	*s;
;162:
;163:	if (!from)
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $106
line 164
;164:		from = g_entities;
ADDRFP4 0
ADDRGP4 g_entities
ASGNP4
ADDRGP4 $111
JUMPV
LABELV $106
line 166
;165:	else
;166:		from++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
line 168
;167:
;168:	for ( ; from < &g_entities[level.num_entities] ; from++)
ADDRGP4 $111
JUMPV
LABELV $108
line 169
;169:	{
line 170
;170:		if (!from->inuse)
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $113
line 171
;171:			continue;
ADDRGP4 $109
JUMPV
LABELV $113
line 172
;172:		s = *(char **) ((byte *)from + fieldofs);
ADDRLP4 0
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
line 173
;173:		if (!s)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $115
line 174
;174:			continue;
ADDRGP4 $109
JUMPV
LABELV $115
line 175
;175:		if (!Q_stricmp (s, match))
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $117
line 176
;176:			return from;
ADDRFP4 0
INDIRP4
RETP4
ADDRGP4 $105
JUMPV
LABELV $117
line 177
;177:	}
LABELV $109
line 168
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $111
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTI4 808
ADDRGP4 level+12
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
CVPU4 4
LTU4 $108
line 179
;178:
;179:	return NULL;
CNSTP4 0
RETP4
LABELV $105
endproc G_Find 8 8
export G_PickTarget
proc G_PickTarget 144 12
line 193
;180:}
;181:
;182:
;183:/*
;184:=============
;185:G_PickTarget
;186:
;187:Selects a random entity from among the targets
;188:=============
;189:*/
;190:#define MAXCHOICES	32
;191:
;192:gentity_t *G_PickTarget (char *targetname)
;193:{
line 194
;194:	gentity_t	*ent = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 195
;195:	int		num_choices = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 198
;196:	gentity_t	*choice[MAXCHOICES];
;197:
;198:	if (!targetname)
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $124
line 199
;199:	{
line 200
;200:		G_Printf("G_PickTarget called with NULL targetname\n");
ADDRGP4 $122
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 201
;201:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $119
JUMPV
LABELV $123
line 205
;202:	}
;203:
;204:	while(1)
;205:	{
line 206
;206:		ent = G_Find (ent, FOFS(targetname), targetname);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 652
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 136
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 136
INDIRP4
ASGNP4
line 207
;207:		if (!ent)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $126
line 208
;208:			break;
ADDRGP4 $125
JUMPV
LABELV $126
line 209
;209:		choice[num_choices++] = ent;
ADDRLP4 140
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
ADDRLP4 140
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 210
;210:		if (num_choices == MAXCHOICES)
ADDRLP4 4
INDIRI4
CNSTI4 32
NEI4 $128
line 211
;211:			break;
ADDRGP4 $125
JUMPV
LABELV $128
line 212
;212:	}
LABELV $124
line 204
ADDRGP4 $123
JUMPV
LABELV $125
line 214
;213:
;214:	if (!num_choices)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $130
line 215
;215:	{
line 216
;216:		G_Printf("G_PickTarget: target %s not found\n", targetname);
ADDRGP4 $132
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 217
;217:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $119
JUMPV
LABELV $130
line 220
;218:	}
;219:
;220:	return choice[rand() % num_choices];
ADDRLP4 136
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
RETP4
LABELV $119
endproc G_PickTarget 144 12
export G_UseTargets
proc G_UseTargets 24 12
line 235
;221:}
;222:
;223:
;224:/*
;225:==============================
;226:G_UseTargets
;227:
;228:"activator" should be set to the entity that initiated the firing.
;229:
;230:Search for (string)targetname in all entities that
;231:match (string)self.target and call their .use function
;232:
;233:==============================
;234:*/
;235:void G_UseTargets( gentity_t *ent, gentity_t *activator ) {
line 238
;236:	gentity_t		*t;
;237:	
;238:	if ( !ent ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $134
line 239
;239:		return;
ADDRGP4 $133
JUMPV
LABELV $134
line 242
;240:	}
;241:
;242:	if (ent->targetShaderName && ent->targetShaderNewName) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CNSTI4 660
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $136
ADDRLP4 4
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $136
line 243
;243:		float f = level.time * 0.001;
ADDRLP4 12
CNSTF4 981668463
ADDRGP4 level+32
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 244
;244:		AddRemap(ent->targetShaderName, ent->targetShaderNewName, f);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 660
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 664
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRF4
ARGF4
ADDRGP4 AddRemap
CALLV
pop
line 245
;245:		trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
ADDRLP4 20
ADDRGP4 BuildShaderStateConfig
CALLP4
ASGNP4
CNSTI4 24
ARGI4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 246
;246:	}
LABELV $136
line 248
;247:
;248:	if ( !ent->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $139
line 249
;249:		return;
ADDRGP4 $133
JUMPV
LABELV $139
line 252
;250:	}
;251:
;252:	t = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $142
JUMPV
LABELV $141
line 253
;253:	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL ) {
line 254
;254:		if ( t == ent ) {
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $144
line 255
;255:			G_Printf ("WARNING: Entity used itself.\n");
ADDRGP4 $146
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 256
;256:		} else {
ADDRGP4 $145
JUMPV
LABELV $144
line 257
;257:			if ( t->use ) {
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $147
line 258
;258:				t->use (t, ent, activator);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
INDIRP4
CALLV
pop
line 259
;259:			}
LABELV $147
line 260
;260:		}
LABELV $145
line 261
;261:		if ( !ent->inuse ) {
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
NEI4 $149
line 262
;262:			G_Printf("entity was removed while using targets\n");
ADDRGP4 $151
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 263
;263:			return;
ADDRGP4 $133
JUMPV
LABELV $149
line 265
;264:		}
;265:	}
LABELV $142
line 253
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 652
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $141
line 266
;266:}
LABELV $133
endproc G_UseTargets 24 12
bss
align 4
LABELV $153
skip 4
align 4
LABELV $154
skip 96
export tv
code
proc tv 8 0
line 277
;267:
;268:
;269:/*
;270:=============
;271:TempVector
;272:
;273:This is just a convenience function
;274:for making temporary vectors for function calls
;275:=============
;276:*/
;277:float	*tv( float x, float y, float z ) {
line 284
;278:	static	int		index;
;279:	static	vec3_t	vecs[8];
;280:	float	*v;
;281:
;282:	// use an array so that multiple tempvectors won't collide
;283:	// for a while
;284:	v = vecs[index];
ADDRLP4 0
CNSTI4 12
ADDRGP4 $153
INDIRI4
MULI4
ADDRGP4 $154
ADDP4
ASGNP4
line 285
;285:	index = (index + 1)&7;
ADDRLP4 4
ADDRGP4 $153
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 7
BANDI4
ASGNI4
line 287
;286:
;287:	v[0] = x;
ADDRLP4 0
INDIRP4
ADDRFP4 0
INDIRF4
ASGNF4
line 288
;288:	v[1] = y;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 4
INDIRF4
ASGNF4
line 289
;289:	v[2] = z;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 291
;290:
;291:	return v;
ADDRLP4 0
INDIRP4
RETP4
LABELV $152
endproc tv 8 0
bss
align 4
LABELV $156
skip 4
align 1
LABELV $157
skip 256
export vtos
code
proc vtos 12 24
line 303
;292:}
;293:
;294:
;295:/*
;296:=============
;297:VectorToString
;298:
;299:This is just a convenience function
;300:for printing vectors
;301:=============
;302:*/
;303:char	*vtos( const vec3_t v ) {
line 309
;304:	static	int		index;
;305:	static	char	str[8][32];
;306:	char	*s;
;307:
;308:	// use an array so that multiple vtos won't collide
;309:	s = str[index];
ADDRLP4 0
ADDRGP4 $156
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 $157
ADDP4
ASGNP4
line 310
;310:	index = (index + 1)&7;
ADDRLP4 4
ADDRGP4 $156
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 7
BANDI4
ASGNI4
line 312
;311:
;312:	Com_sprintf (s, 32, "(%i %i %i)", (int)v[0], (int)v[1], (int)v[2]);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $158
ARGP4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 314
;313:
;314:	return s;
ADDRLP4 0
INDIRP4
RETP4
LABELV $155
endproc vtos 12 24
data
align 4
LABELV $160
byte 4 0
byte 4 3212836864
byte 4 0
align 4
LABELV $161
byte 4 0
byte 4 0
byte 4 1065353216
align 4
LABELV $162
byte 4 0
byte 4 3221225472
byte 4 0
align 4
LABELV $163
byte 4 0
byte 4 0
byte 4 3212836864
export G_SetMovedir
code
proc G_SetMovedir 16 16
line 328
;315:}
;316:
;317:
;318:/*
;319:===============
;320:G_SetMovedir
;321:
;322:The editor only specifies a single value for angles (yaw),
;323:but we have special constants to generate an up or down direction.
;324:Angles will be cleared, because it is being used to represent a direction
;325:instead of an orientation.
;326:===============
;327:*/
;328:void G_SetMovedir( vec3_t angles, vec3_t movedir ) {
line 334
;329:	static vec3_t VEC_UP		= {0, -1, 0};
;330:	static vec3_t MOVEDIR_UP	= {0, 0, 1};
;331:	static vec3_t VEC_DOWN		= {0, -2, 0};
;332:	static vec3_t MOVEDIR_DOWN	= {0, 0, -1};
;333:
;334:	if ( VectorCompare (angles, VEC_UP) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $160
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $164
line 335
;335:		VectorCopy (MOVEDIR_UP, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 $161
INDIRB
ASGNB 12
line 336
;336:	} else if ( VectorCompare (angles, VEC_DOWN) ) {
ADDRGP4 $165
JUMPV
LABELV $164
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $162
ARGP4
ADDRLP4 4
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $166
line 337
;337:		VectorCopy (MOVEDIR_DOWN, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 $163
INDIRB
ASGNB 12
line 338
;338:	} else {
ADDRGP4 $167
JUMPV
LABELV $166
line 339
;339:		AngleVectors (angles, movedir, NULL, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 340
;340:	}
LABELV $167
LABELV $165
line 341
;341:	VectorClear( angles );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
line 342
;342:}
LABELV $159
endproc G_SetMovedir 16 16
export vectoyaw
proc vectoyaw 20 8
line 345
;343:
;344:
;345:float vectoyaw( const vec3_t vec ) {
line 348
;346:	float	yaw;
;347:	
;348:	if (vec[YAW] == 0 && vec[PITCH] == 0) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $169
ADDRLP4 4
INDIRP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $169
line 349
;349:		yaw = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 350
;350:	} else {
ADDRGP4 $170
JUMPV
LABELV $169
line 351
;351:		if (vec[PITCH]) {
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 0
EQF4 $171
line 352
;352:			yaw = ( atan2( vec[YAW], vec[PITCH]) * 180 / M_PI );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 atan2
CALLF4
ASGNF4
ADDRLP4 0
CNSTF4 1127481344
ADDRLP4 16
INDIRF4
MULF4
CNSTF4 1078530011
DIVF4
ASGNF4
line 353
;353:		} else if (vec[YAW] > 0) {
ADDRGP4 $172
JUMPV
LABELV $171
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 0
LEF4 $173
line 354
;354:			yaw = 90;
ADDRLP4 0
CNSTF4 1119092736
ASGNF4
line 355
;355:		} else {
ADDRGP4 $174
JUMPV
LABELV $173
line 356
;356:			yaw = 270;
ADDRLP4 0
CNSTF4 1132920832
ASGNF4
line 357
;357:		}
LABELV $174
LABELV $172
line 358
;358:		if (yaw < 0) {
ADDRLP4 0
INDIRF4
CNSTF4 0
GEF4 $175
line 359
;359:			yaw += 360;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
ADDF4
ASGNF4
line 360
;360:		}
LABELV $175
line 361
;361:	}
LABELV $170
line 363
;362:
;363:	return yaw;
ADDRLP4 0
INDIRF4
RETF4
LABELV $168
endproc vectoyaw 20 8
export G_InitGentity
proc G_InitGentity 4 0
line 367
;364:}
;365:
;366:
;367:void G_InitGentity( gentity_t *e ) {
line 368
;368:	e->inuse = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 1
ASGNI4
line 369
;369:	e->classname = "noclass";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $178
ASGNP4
line 370
;370:	e->s.number = e - g_entities;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 808
DIVI4
ASGNI4
line 371
;371:	e->r.ownerNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
CNSTI4 1023
ASGNI4
line 372
;372:}
LABELV $177
endproc G_InitGentity 4 0
export G_Spawn
proc G_Spawn 16 20
line 389
;373:
;374:/*
;375:=================
;376:G_Spawn
;377:
;378:Either finds a free entity, or allocates a new one.
;379:
;380:  The slots from 0 to MAX_CLIENTS-1 are always reserved for clients, and will
;381:never be used by anything else.
;382:
;383:Try to avoid reusing an entity that was recently freed, because it
;384:can cause the client to think the entity morphed into something else
;385:instead of being removed and recreated, which can cause interpolated
;386:angles and bad trails.
;387:=================
;388:*/
;389:gentity_t *G_Spawn( void ) {
line 393
;390:	int			i, force;
;391:	gentity_t	*e;
;392:
;393:	e = NULL;	// shut up warning
ADDRLP4 0
CNSTP4 0
ASGNP4
line 394
;394:	i = 0;		// shut up warning
ADDRLP4 4
CNSTI4 0
ASGNI4
line 395
;395:	for ( force = 0 ; force < 2 ; force++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $180
line 398
;396:		// if we go through all entities and can't find one to free,
;397:		// override the normal minimum times before use
;398:		e = &g_entities[MAX_CLIENTS];
ADDRLP4 0
ADDRGP4 g_entities+51712
ASGNP4
line 399
;399:		for ( i = MAX_CLIENTS ; i<level.num_entities ; i++, e++) {
ADDRLP4 4
CNSTI4 64
ASGNI4
ADDRGP4 $188
JUMPV
LABELV $185
line 400
;400:			if ( e->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
EQI4 $190
line 401
;401:				continue;
ADDRGP4 $186
JUMPV
LABELV $190
line 406
;402:			}
;403:
;404:			// the first couple seconds of server time can involve a lot of
;405:			// freeing and allocating, so relax the replacement policy
;406:			if ( !force && e->freetime > level.startTime + 2000 && level.time - e->freetime < 1000 ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $192
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 level+40
INDIRI4
CNSTI4 2000
ADDI4
LEI4 $192
ADDRGP4 level+32
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CNSTI4 1000
GEI4 $192
line 407
;407:				continue;
ADDRGP4 $186
JUMPV
LABELV $192
line 411
;408:			}
;409:
;410:			// reuse this slot
;411:			G_InitGentity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 412
;412:			return e;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $179
JUMPV
LABELV $186
line 399
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $188
ADDRLP4 4
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $185
line 414
;413:		}
;414:		if ( i != MAX_GENTITIES ) {
ADDRLP4 4
INDIRI4
CNSTI4 1024
EQI4 $196
line 415
;415:			break;
ADDRGP4 $182
JUMPV
LABELV $196
line 417
;416:		}
;417:	}
LABELV $181
line 395
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 2
LTI4 $180
LABELV $182
line 418
;418:	if ( i == ENTITYNUM_MAX_NORMAL ) {
ADDRLP4 4
INDIRI4
CNSTI4 1022
NEI4 $198
line 419
;419:		for (i = 0; i < MAX_GENTITIES; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $200
line 420
;420:			G_Printf("%4i: %s\n", i, g_entities[i].classname);
ADDRGP4 $204
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 808
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities+524
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 421
;421:		}
LABELV $201
line 419
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $200
line 422
;422:		G_Error( "G_Spawn: no free entities" );
ADDRGP4 $206
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 423
;423:	}
LABELV $198
line 426
;424:	
;425:	// open up a new slot
;426:	level.num_entities++;
ADDRLP4 12
ADDRGP4 level+12
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 429
;427:
;428:	// let the server system know that there are more entities
;429:	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
ADDRGP4 level+4
INDIRP4
ARGP4
ADDRGP4 level+12
INDIRI4
ARGI4
CNSTI4 808
ARGI4
ADDRGP4 level
INDIRP4
ARGP4
CNSTI4 776
ARGI4
ADDRGP4 trap_LocateGameData
CALLV
pop
line 432
;430:		&level.clients[0].ps, sizeof( level.clients[0] ) );
;431:
;432:	G_InitGentity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 433
;433:	return e;
ADDRLP4 0
INDIRP4
RETP4
LABELV $179
endproc G_Spawn 16 20
export G_EntitiesFree
proc G_EntitiesFree 8 0
line 441
;434:}
;435:
;436:/*
;437:=================
;438:G_EntitiesFree
;439:=================
;440:*/
;441:qboolean G_EntitiesFree( void ) {
line 445
;442:	int			i;
;443:	gentity_t	*e;
;444:
;445:	e = &g_entities[MAX_CLIENTS];
ADDRLP4 4
ADDRGP4 g_entities+51712
ASGNP4
line 446
;446:	for ( i = MAX_CLIENTS; i < level.num_entities; i++, e++) {
ADDRLP4 0
CNSTI4 64
ASGNI4
ADDRGP4 $215
JUMPV
LABELV $212
line 447
;447:		if ( e->inuse ) {
ADDRLP4 4
INDIRP4
CNSTI4 520
ADDP4
INDIRI4
CNSTI4 0
EQI4 $217
line 448
;448:			continue;
ADDRGP4 $213
JUMPV
LABELV $217
line 451
;449:		}
;450:		// slot available
;451:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $210
JUMPV
LABELV $213
line 446
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 808
ADDP4
ASGNP4
LABELV $215
ADDRLP4 0
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $212
line 453
;452:	}
;453:	return qfalse;
CNSTI4 0
RETI4
LABELV $210
endproc G_EntitiesFree 8 0
export G_FreeEntity
proc G_FreeEntity 0 12
line 464
;454:}
;455:
;456:
;457:/*
;458:=================
;459:G_FreeEntity
;460:
;461:Marks the entity as free
;462:=================
;463:*/
;464:void G_FreeEntity( gentity_t *ed ) {
line 465
;465:	trap_UnlinkEntity (ed);		// unlink from world
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 467
;466:
;467:	if ( ed->neverFree ) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
EQI4 $220
line 468
;468:		return;
ADDRGP4 $219
JUMPV
LABELV $220
line 471
;469:	}
;470:
;471:	memset (ed, 0, sizeof(*ed));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 808
ARGI4
ADDRGP4 memset
CALLP4
pop
line 472
;472:	ed->classname = "freed";
ADDRFP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $222
ASGNP4
line 473
;473:	ed->freetime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 474
;474:	ed->inuse = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 520
ADDP4
CNSTI4 0
ASGNI4
line 475
;475:}
LABELV $219
endproc G_FreeEntity 0 12
export G_TempEntity
proc G_TempEntity 20 8
line 486
;476:
;477:/*
;478:=================
;479:G_TempEntity
;480:
;481:Spawns an event entity that will be auto-removed
;482:The origin will be snapped to save net bandwidth, so care
;483:must be taken if the origin is right on a surface (snap towards start vector first)
;484:=================
;485:*/
;486:gentity_t *G_TempEntity( vec3_t origin, int event ) {
line 490
;487:	gentity_t		*e;
;488:	vec3_t		snapped;
;489:
;490:	e = G_Spawn();
ADDRLP4 16
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 491
;491:	e->s.eType = ET_EVENTS + event;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 4
INDIRI4
CNSTI4 13
ADDI4
ASGNI4
line 493
;492:
;493:	e->classname = "tempEntity";
ADDRLP4 0
INDIRP4
CNSTI4 524
ADDP4
ADDRGP4 $225
ASGNP4
line 494
;494:	e->eventTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 495
;495:	e->freeAfterEvent = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 556
ADDP4
CNSTI4 1
ASGNI4
line 497
;496:
;497:	VectorCopy( origin, snapped );
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 498
;498:	SnapVector( snapped );		// save network bandwidth
ADDRLP4 4
ADDRLP4 4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 499
;499:	G_SetOrigin( e, snapped );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 502
;500:
;501:	// find cluster for PVS
;502:	trap_LinkEntity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 504
;503:
;504:	return e;
ADDRLP4 0
INDIRP4
RETP4
LABELV $224
endproc G_TempEntity 20 8
export G_KillBox
proc G_KillBox 4168 32
line 525
;505:}
;506:
;507:
;508:
;509:/*
;510:==============================================================================
;511:
;512:Kill box
;513:
;514:==============================================================================
;515:*/
;516:
;517:/*
;518:=================
;519:G_KillBox
;520:
;521:Kills all entities that would touch the proposed new positioning
;522:of ent.  Ent should be unlinked before calling this!
;523:=================
;524:*/
;525:void G_KillBox (gentity_t *ent) {
line 531
;526:	int			i, num;
;527:	int			touch[MAX_GENTITIES];
;528:	gentity_t	*hit;
;529:	vec3_t		mins, maxs;
;530:
;531:	VectorAdd( ent->client->ps.origin, ent->r.mins, mins );
ADDRLP4 4132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4136
ADDRLP4 4132
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4136
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4132
INDIRP4
CNSTI4 436
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+4
ADDRLP4 4136
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4132
INDIRP4
CNSTI4 440
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4108+8
ADDRLP4 4140
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4140
INDIRP4
CNSTI4 444
ADDP4
INDIRF4
ADDF4
ASGNF4
line 532
;532:	VectorAdd( ent->client->ps.origin, ent->r.maxs, maxs );
ADDRLP4 4144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4148
ADDRLP4 4144
INDIRP4
CNSTI4 516
ADDP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4148
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4144
INDIRP4
CNSTI4 448
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+4
ADDRLP4 4148
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4144
INDIRP4
CNSTI4 452
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4120+8
ADDRLP4 4152
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4152
INDIRP4
CNSTI4 456
ADDP4
INDIRF4
ADDF4
ASGNF4
line 533
;533:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 4108
ARGP4
ADDRLP4 4120
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4156
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4156
INDIRI4
ASGNI4
line 535
;534:
;535:	for (i=0 ; i<num ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $239
JUMPV
LABELV $236
line 536
;536:		hit = &g_entities[touch[i]];
ADDRLP4 4
CNSTI4 808
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 537
;537:		if ( !hit->client ) {
ADDRLP4 4
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $240
line 538
;538:			continue;
ADDRGP4 $237
JUMPV
LABELV $240
line 542
;539:		}
;540:
;541:		// nail it
;542:		G_Damage ( hit, ent, ent, NULL, NULL,
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4160
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4160
INDIRP4
ARGP4
ADDRLP4 4160
INDIRP4
ARGP4
ADDRLP4 4164
CNSTP4 0
ASGNP4
ADDRLP4 4164
INDIRP4
ARGP4
ADDRLP4 4164
INDIRP4
ARGP4
CNSTI4 100000
ARGI4
CNSTI4 8
ARGI4
CNSTI4 18
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 544
;543:			100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
;544:	}
LABELV $237
line 535
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $239
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $236
line 546
;545:
;546:}
LABELV $231
endproc G_KillBox 4168 32
export G_AddPredictableEvent
proc G_AddPredictableEvent 0 12
line 559
;547:
;548://==============================================================================
;549:
;550:/*
;551:===============
;552:G_AddPredictableEvent
;553:
;554:Use for non-pmove events that would also be predicted on the
;555:client side: jumppads and item pickups
;556:Adds an event+parm and twiddles the event counter
;557:===============
;558:*/
;559:void G_AddPredictableEvent( gentity_t *ent, int event, int eventParm ) {
line 560
;560:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $243
line 561
;561:		return;
ADDRGP4 $242
JUMPV
LABELV $243
line 563
;562:	}
;563:	BG_AddPredictableEventToPlayerstate( event, eventParm, &ent->client->ps );
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 564
;564:}
LABELV $242
endproc G_AddPredictableEvent 0 12
export G_AddEvent
proc G_AddEvent 4 8
line 574
;565:
;566:
;567:/*
;568:===============
;569:G_AddEvent
;570:
;571:Adds an event+parm and twiddles the event counter
;572:===============
;573:*/
;574:void G_AddEvent( gentity_t *ent, int event, int eventParm ) {
line 577
;575:	int		bits;
;576:
;577:	if ( !event ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $246
line 578
;578:		G_Printf( "G_AddEvent: zero event added for entity %i\n", ent->s.number );
ADDRGP4 $248
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 579
;579:		return;
ADDRGP4 $245
JUMPV
LABELV $246
line 583
;580:	}
;581:
;582:	// clients need to add the event in playerState_t instead of entityState_t
;583:	if ( ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $249
line 584
;584:		bits = ent->client->ps.externalEvent & EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 128
ADDP4
INDIRI4
CNSTI4 768
BANDI4
ASGNI4
line 585
;585:		bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 256
ADDI4
CNSTI4 768
BANDI4
ASGNI4
line 586
;586:		ent->client->ps.externalEvent = event | bits;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 128
ADDP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 587
;587:		ent->client->ps.externalEventParm = eventParm;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 588
;588:		ent->client->ps.externalEventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 516
ADDP4
INDIRP4
CNSTI4 136
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 589
;589:	} else {
ADDRGP4 $250
JUMPV
LABELV $249
line 590
;590:		bits = ent->s.event & EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRI4
CNSTI4 768
BANDI4
ASGNI4
line 591
;591:		bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 256
ADDI4
CNSTI4 768
BANDI4
ASGNI4
line 592
;592:		ent->s.event = event | bits;
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 593
;593:		ent->s.eventParm = eventParm;
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 594
;594:	}
LABELV $250
line 595
;595:	ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 596
;596:}
LABELV $245
endproc G_AddEvent 4 8
export G_Sound
proc G_Sound 8 8
line 604
;597:
;598:
;599:/*
;600:=============
;601:G_Sound
;602:=============
;603:*/
;604:void G_Sound( gentity_t *ent, int channel, int soundIndex ) {
line 607
;605:	gentity_t	*te;
;606:
;607:	te = G_TempEntity( ent->r.currentOrigin, EV_GENERAL_SOUND );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ARGP4
CNSTI4 45
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 608
;608:	te->s.eventParm = soundIndex;
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 609
;609:}
LABELV $253
endproc G_Sound 8 8
export G_SetOrigin
proc G_SetOrigin 8 0
line 622
;610:
;611:
;612://==============================================================================
;613:
;614:
;615:/*
;616:================
;617:G_SetOrigin
;618:
;619:Sets the pos trajectory for a fixed position
;620:================
;621:*/
;622:void G_SetOrigin( gentity_t *ent, vec3_t origin ) {
line 623
;623:	VectorCopy( origin, ent->s.pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 624
;624:	ent->s.pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 625
;625:	ent->s.pos.trTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 0
ASGNI4
line 626
;626:	ent->s.pos.trDuration = 0;
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
CNSTI4 0
ASGNI4
line 627
;627:	VectorClear( ent->s.pos.trDelta );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 629
;628:
;629:	VectorCopy( origin, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 630
;630:}
LABELV $254
endproc G_SetOrigin 8 0
lit
align 4
LABELV $256
byte 4 0
byte 4 0
byte 4 1065353216
export DebugLine
code
proc DebugLine 116 12
line 640
;631:
;632:/*
;633:================
;634:DebugLine
;635:
;636:  debug polygons only work when running a local game
;637:  with r_debugSurface set to 2
;638:================
;639:*/
;640:int DebugLine(vec3_t start, vec3_t end, int color) {
line 641
;641:	vec3_t points[4], dir, cross, up = {0, 0, 1};
ADDRLP4 72
ADDRGP4 $256
INDIRB
ASGNB 12
line 644
;642:	float dot;
;643:
;644:	VectorCopy(start, points[0]);
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 645
;645:	VectorCopy(start, points[1]);
ADDRLP4 0+12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 647
;646:	//points[1][2] -= 2;
;647:	VectorCopy(end, points[2]);
ADDRLP4 0+24
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 649
;648:	//points[2][2] -= 2;
;649:	VectorCopy(end, points[3]);
ADDRLP4 0+36
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 652
;650:
;651:
;652:	VectorSubtract(end, start, dir);
ADDRLP4 88
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 92
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
ADDRLP4 88
INDIRP4
INDIRF4
ADDRLP4 92
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 96
CNSTI4 4
ASGNI4
ADDRLP4 60+4
ADDRLP4 88
INDIRP4
ADDRLP4 96
INDIRI4
ADDP4
INDIRF4
ADDRLP4 92
INDIRP4
ADDRLP4 96
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 100
CNSTI4 8
ASGNI4
ADDRLP4 60+8
ADDRFP4 4
INDIRP4
ADDRLP4 100
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 100
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 653
;653:	VectorNormalize(dir);
ADDRLP4 60
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 654
;654:	dot = DotProduct(dir, up);
ADDRLP4 84
ADDRLP4 60
INDIRF4
ADDRLP4 72
INDIRF4
MULF4
ADDRLP4 60+4
INDIRF4
ADDRLP4 72+4
INDIRF4
MULF4
ADDF4
ADDRLP4 60+8
INDIRF4
ADDRLP4 72+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 655
;655:	if (dot > 0.99 || dot < -0.99) VectorSet(cross, 1, 0, 0);
ADDRLP4 84
INDIRF4
CNSTF4 1065185444
GTF4 $268
ADDRLP4 84
INDIRF4
CNSTF4 3212669092
GEF4 $266
LABELV $268
ADDRLP4 48
CNSTF4 1065353216
ASGNF4
ADDRLP4 48+4
CNSTF4 0
ASGNF4
ADDRLP4 48+8
CNSTF4 0
ASGNF4
ADDRGP4 $267
JUMPV
LABELV $266
line 656
;656:	else CrossProduct(dir, up, cross);
ADDRLP4 60
ARGP4
ADDRLP4 72
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
LABELV $267
line 658
;657:
;658:	VectorNormalize(cross);
ADDRLP4 48
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 660
;659:
;660:	VectorMA(points[0], 2, cross, points[0]);
ADDRLP4 108
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 661
;661:	VectorMA(points[1], -2, cross, points[1]);
ADDRLP4 0+12
ADDRLP4 0+12
INDIRF4
CNSTF4 3221225472
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+12+4
ADDRLP4 0+12+4
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+12+8
ADDRLP4 0+12+8
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 662
;662:	VectorMA(points[2], -2, cross, points[2]);
ADDRLP4 0+24
ADDRLP4 0+24
INDIRF4
CNSTF4 3221225472
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+24+4
ADDRLP4 0+24+4
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+24+8
ADDRLP4 0+24+8
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 663
;663:	VectorMA(points[3], 2, cross, points[3]);
ADDRLP4 0+36
ADDRLP4 0+36
INDIRF4
CNSTF4 1073741824
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+36+4
ADDRLP4 0+36+4
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+36+8
ADDRLP4 0+36+8
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 665
;664:
;665:	return trap_DebugPolygonCreate(color, 4, points);
ADDRFP4 8
INDIRI4
ARGI4
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 112
ADDRGP4 trap_DebugPolygonCreate
CALLI4
ASGNI4
ADDRLP4 112
INDIRI4
RETI4
LABELV $255
endproc DebugLine 116 12
bss
export remappedShaders
align 4
LABELV remappedShaders
skip 16896
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
import trap_Cvar_VariableValue
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
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
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
import G_TouchSolids
import G_TouchTriggers
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
LABELV $248
byte 1 71
byte 1 95
byte 1 65
byte 1 100
byte 1 100
byte 1 69
byte 1 118
byte 1 101
byte 1 110
byte 1 116
byte 1 58
byte 1 32
byte 1 122
byte 1 101
byte 1 114
byte 1 111
byte 1 32
byte 1 101
byte 1 118
byte 1 101
byte 1 110
byte 1 116
byte 1 32
byte 1 97
byte 1 100
byte 1 100
byte 1 101
byte 1 100
byte 1 32
byte 1 102
byte 1 111
byte 1 114
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $225
byte 1 116
byte 1 101
byte 1 109
byte 1 112
byte 1 69
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 0
align 1
LABELV $222
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $206
byte 1 71
byte 1 95
byte 1 83
byte 1 112
byte 1 97
byte 1 119
byte 1 110
byte 1 58
byte 1 32
byte 1 110
byte 1 111
byte 1 32
byte 1 102
byte 1 114
byte 1 101
byte 1 101
byte 1 32
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 105
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $204
byte 1 37
byte 1 52
byte 1 105
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $178
byte 1 110
byte 1 111
byte 1 99
byte 1 108
byte 1 97
byte 1 115
byte 1 115
byte 1 0
align 1
LABELV $158
byte 1 40
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 41
byte 1 0
align 1
LABELV $151
byte 1 101
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 119
byte 1 97
byte 1 115
byte 1 32
byte 1 114
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 100
byte 1 32
byte 1 119
byte 1 104
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 117
byte 1 115
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $146
byte 1 87
byte 1 65
byte 1 82
byte 1 78
byte 1 73
byte 1 78
byte 1 71
byte 1 58
byte 1 32
byte 1 69
byte 1 110
byte 1 116
byte 1 105
byte 1 116
byte 1 121
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 116
byte 1 115
byte 1 101
byte 1 108
byte 1 102
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $132
byte 1 71
byte 1 95
byte 1 80
byte 1 105
byte 1 99
byte 1 107
byte 1 84
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 58
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 37
byte 1 115
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
byte 1 10
byte 1 0
align 1
LABELV $122
byte 1 71
byte 1 95
byte 1 80
byte 1 105
byte 1 99
byte 1 107
byte 1 84
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 32
byte 1 99
byte 1 97
byte 1 108
byte 1 108
byte 1 101
byte 1 100
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 116
byte 1 97
byte 1 114
byte 1 103
byte 1 101
byte 1 116
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 10
byte 1 0
align 1
LABELV $104
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $91
byte 1 71
byte 1 95
byte 1 70
byte 1 105
byte 1 110
byte 1 100
byte 1 67
byte 1 111
byte 1 110
byte 1 102
byte 1 105
byte 1 103
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 73
byte 1 110
byte 1 100
byte 1 101
byte 1 120
byte 1 58
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 102
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $72
byte 1 37
byte 1 115
byte 1 61
byte 1 37
byte 1 115
byte 1 58
byte 1 37
byte 1 53
byte 1 46
byte 1 50
byte 1 102
byte 1 64
byte 1 0
