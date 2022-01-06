data
align 4
LABELV captureFunc
byte 4 0
align 4
LABELV captureData
byte 4 0
align 4
LABELV itemCapture
byte 4 0
export DC
align 4
LABELV DC
byte 4 0
align 4
LABELV g_waitingForKey
byte 4 0
align 4
LABELV g_editingField
byte 4 0
align 4
LABELV g_bindItem
byte 4 0
align 4
LABELV g_editItem
byte 4 0
export menuCount
align 4
LABELV menuCount
byte 4 0
export openMenuCount
align 4
LABELV openMenuCount
byte 4 0
align 4
LABELV debugMode
byte 4 0
align 4
LABELV lastListBoxClickTime
byte 4 0
export UI_Alloc
code
proc UI_Alloc 8 4
file "../ui_shared.c"
line 92
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
;23:// string allocation/managment
;24:
;25:#include "ui_shared.h"
;26:
;27:#define SCROLL_TIME_START					500
;28:#define SCROLL_TIME_ADJUST				150
;29:#define SCROLL_TIME_ADJUSTOFFSET	40
;30:#define SCROLL_TIME_FLOOR					20
;31:
;32:typedef struct scrollInfo_s {
;33:	int nextScrollTime;
;34:	int nextAdjustTime;
;35:	int adjustValue;
;36:	int scrollKey;
;37:	float xStart;
;38:	float yStart;
;39:	itemDef_t *item;
;40:	qboolean scrollDir;
;41:} scrollInfo_t;
;42:
;43:static scrollInfo_t scrollInfo;
;44:
;45:static void (*captureFunc) (void *p) = NULL;
;46:static void *captureData = NULL;
;47:static itemDef_t *itemCapture = NULL;   // item that has the mouse captured ( if any )
;48:
;49:displayContextDef_t *DC = NULL;
;50:
;51:static qboolean g_waitingForKey = qfalse;
;52:static qboolean g_editingField = qfalse;
;53:
;54:static itemDef_t *g_bindItem = NULL;
;55:static itemDef_t *g_editItem = NULL;
;56:
;57:menuDef_t Menus[MAX_MENUS];      // defined menus
;58:int menuCount = 0;               // how many
;59:
;60:menuDef_t *menuStack[MAX_OPEN_MENUS];
;61:int openMenuCount = 0;
;62:
;63:static qboolean debugMode = qfalse;
;64:
;65:#define DOUBLE_CLICK_DELAY 300
;66:static int lastListBoxClickTime = 0;
;67:
;68:void Item_RunScript(itemDef_t *item, const char *s);
;69:void Item_SetupKeywordHash(void);
;70:void Menu_SetupKeywordHash(void);
;71:int BindingIDFromName(const char *name);
;72:qboolean Item_Bind_HandleKey(itemDef_t *item, int key, qboolean down);
;73:itemDef_t *Menu_SetPrevCursorItem(menuDef_t *menu);
;74:itemDef_t *Menu_SetNextCursorItem(menuDef_t *menu);
;75:static qboolean Menu_OverActiveItem(menuDef_t *menu, float x, float y);
;76:
;77:#ifdef CGAME
;78:#define MEM_POOL_SIZE  128 * 1024
;79:#else
;80:#define MEM_POOL_SIZE  1024 * 1024
;81:#endif
;82:
;83:static char		memoryPool[MEM_POOL_SIZE];
;84:static int		allocPoint, outOfMemory;
;85:
;86:
;87:/*
;88:===============
;89:UI_Alloc
;90:===============
;91:*/				  
;92:void *UI_Alloc( int size ) {
line 95
;93:	char	*p; 
;94:
;95:	if ( allocPoint + size > MEM_POOL_SIZE ) {
ADDRGP4 allocPoint
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
CNSTI4 1048576
LEI4 $41
line 96
;96:		outOfMemory = qtrue;
ADDRGP4 outOfMemory
CNSTI4 1
ASGNI4
line 97
;97:		if (DC->Print) {
ADDRGP4 DC
INDIRP4
CNSTI4 156
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $43
line 98
;98:			DC->Print("UI_Alloc: Failure. Out of memory!\n");
ADDRGP4 $45
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 156
ADDP4
INDIRP4
CALLV
pop
line 99
;99:		}
LABELV $43
line 101
;100:    //DC->trap_Print(S_COLOR_YELLOW"WARNING: UI Out of Memory!\n");
;101:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $40
JUMPV
LABELV $41
line 104
;102:	}
;103:
;104:	p = &memoryPool[allocPoint];
ADDRLP4 0
ADDRGP4 allocPoint
INDIRI4
ADDRGP4 memoryPool
ADDP4
ASGNP4
line 106
;105:
;106:	allocPoint += ( size + 15 ) & ~15;
ADDRLP4 4
ADDRGP4 allocPoint
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
CNSTI4 15
ADDI4
CNSTI4 -16
BANDI4
ADDI4
ASGNI4
line 108
;107:
;108:	return p;
ADDRLP4 0
INDIRP4
RETP4
LABELV $40
endproc UI_Alloc 8 4
export UI_InitMemory
proc UI_InitMemory 0 0
line 116
;109:}
;110:
;111:/*
;112:===============
;113:UI_InitMemory
;114:===============
;115:*/
;116:void UI_InitMemory( void ) {
line 117
;117:	allocPoint = 0;
ADDRGP4 allocPoint
CNSTI4 0
ASGNI4
line 118
;118:	outOfMemory = qfalse;
ADDRGP4 outOfMemory
CNSTI4 0
ASGNI4
line 119
;119:}
LABELV $46
endproc UI_InitMemory 0 0
export UI_OutOfMemory
proc UI_OutOfMemory 0 0
line 121
;120:
;121:qboolean UI_OutOfMemory() {
line 122
;122:	return outOfMemory;
ADDRGP4 outOfMemory
INDIRI4
RETI4
LABELV $47
endproc UI_OutOfMemory 0 0
proc hashForString 16 4
line 135
;123:}
;124:
;125:
;126:
;127:
;128:
;129:#define HASH_TABLE_SIZE 2048
;130:/*
;131:================
;132:return a hash value for the string
;133:================
;134:*/
;135:static long hashForString(const char *str) {
line 140
;136:	int		i;
;137:	long	hash;
;138:	char	letter;
;139:
;140:	hash = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 141
;141:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $50
JUMPV
LABELV $49
line 142
;142:	while (str[i] != '\0') {
line 143
;143:		letter = tolower(str[i]);
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 12
ADDRGP4 tolower
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
CVII1 4
ASGNI1
line 144
;144:		hash+=(long)(letter)*(i+119);
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI1
CVII4 1
ADDRLP4 0
INDIRI4
CNSTI4 119
ADDI4
MULI4
ADDI4
ASGNI4
line 145
;145:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 146
;146:	}
LABELV $50
line 142
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $49
line 147
;147:	hash &= (HASH_TABLE_SIZE-1);
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 2047
BANDI4
ASGNI4
line 148
;148:	return hash;
ADDRLP4 8
INDIRI4
RETI4
LABELV $48
endproc hashForString 16 4
data
align 4
LABELV strPoolIndex
byte 4 0
align 4
LABELV strHandleCount
byte 4 0
align 4
LABELV $53
address $54
export String_Alloc
code
proc String_Alloc 44 8
line 163
;149:}
;150:
;151:typedef struct stringDef_s {
;152:	struct stringDef_s *next;
;153:	const char *str;
;154:} stringDef_t;
;155:
;156:static int strPoolIndex = 0;
;157:static char strPool[STRING_POOL_SIZE];
;158:
;159:static int strHandleCount = 0;
;160:static stringDef_t *strHandle[HASH_TABLE_SIZE];
;161:
;162:
;163:const char *String_Alloc(const char *p) {
line 169
;164:	int len;
;165:	long hash;
;166:	stringDef_t *str, *last;
;167:	static const char *staticNULL = "";
;168:
;169:	if (p == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $55
line 170
;170:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $52
JUMPV
LABELV $55
line 173
;171:	}
;172:
;173:	if (*p == 0) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $57
line 174
;174:		return staticNULL;
ADDRGP4 $53
INDIRP4
RETP4
ADDRGP4 $52
JUMPV
LABELV $57
line 177
;175:	}
;176:
;177:	hash = hashForString(p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 hashForString
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
ASGNI4
line 179
;178:
;179:	str = strHandle[hash];
ADDRLP4 0
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 strHandle
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $60
JUMPV
LABELV $59
line 180
;180:	while (str) {
line 181
;181:		if (strcmp(p, str->str) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $62
line 182
;182:			return str->str;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
RETP4
ADDRGP4 $52
JUMPV
LABELV $62
line 184
;183:		}
;184:		str = str->next;
ADDRLP4 0
ADDRLP4 0
INDIRP4
INDIRP4
ASGNP4
line 185
;185:	}
LABELV $60
line 180
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $59
line 187
;186:
;187:	len = strlen(p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
line 188
;188:	if (len + strPoolIndex + 1 < STRING_POOL_SIZE) {
ADDRLP4 12
INDIRI4
ADDRGP4 strPoolIndex
INDIRI4
ADDI4
CNSTI4 1
ADDI4
CNSTI4 393216
GEI4 $64
line 189
;189:		int ph = strPoolIndex;
ADDRLP4 24
ADDRGP4 strPoolIndex
INDIRI4
ASGNI4
line 190
;190:		strcpy(&strPool[strPoolIndex], p);
ADDRGP4 strPoolIndex
INDIRI4
ADDRGP4 strPool
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 191
;191:		strPoolIndex += len + 1;
ADDRLP4 28
ADDRGP4 strPoolIndex
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ADDI4
ASGNI4
line 193
;192:
;193:		str = strHandle[hash];
ADDRLP4 0
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 strHandle
ADDP4
INDIRP4
ASGNP4
line 194
;194:		last = str;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $67
JUMPV
LABELV $66
line 195
;195:		while (str && str->next) {
line 196
;196:			last = str;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 197
;197:			str = str->next;
ADDRLP4 0
ADDRLP4 0
INDIRP4
INDIRP4
ASGNP4
line 198
;198:		}
LABELV $67
line 195
ADDRLP4 36
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 36
INDIRU4
EQU4 $69
ADDRLP4 0
INDIRP4
INDIRP4
CVPU4 4
ADDRLP4 36
INDIRU4
NEU4 $66
LABELV $69
line 200
;199:
;200:		str  = UI_Alloc(sizeof(stringDef_t));
CNSTI4 8
ARGI4
ADDRLP4 40
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 40
INDIRP4
ASGNP4
line 201
;201:		str->next = NULL;
ADDRLP4 0
INDIRP4
CNSTP4 0
ASGNP4
line 202
;202:		str->str = &strPool[ph];
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 24
INDIRI4
ADDRGP4 strPool
ADDP4
ASGNP4
line 203
;203:		if (last) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $70
line 204
;204:			last->next = str;
ADDRLP4 4
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 205
;205:		} else {
ADDRGP4 $71
JUMPV
LABELV $70
line 206
;206:			strHandle[hash] = str;
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 strHandle
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 207
;207:		}
LABELV $71
line 208
;208:		return &strPool[ph];
ADDRLP4 24
INDIRI4
ADDRGP4 strPool
ADDP4
RETP4
ADDRGP4 $52
JUMPV
LABELV $64
line 210
;209:	}
;210:	return NULL;
CNSTP4 0
RETP4
LABELV $52
endproc String_Alloc 44 8
export String_Report
proc String_Report 4 16
line 213
;211:}
;212:
;213:void String_Report() {
line 215
;214:	float f;
;215:	Com_Printf("Memory/String Pool Info\n");
ADDRGP4 $73
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 216
;216:	Com_Printf("----------------\n");
ADDRGP4 $74
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 217
;217:	f = strPoolIndex;
ADDRLP4 0
ADDRGP4 strPoolIndex
INDIRI4
CVIF4 4
ASGNF4
line 218
;218:	f /= STRING_POOL_SIZE;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1220542464
DIVF4
ASGNF4
line 219
;219:	f *= 100;
ADDRLP4 0
CNSTF4 1120403456
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 220
;220:	Com_Printf("String Pool is %.1f%% full, %i bytes out of %i used.\n", f, strPoolIndex, STRING_POOL_SIZE);
ADDRGP4 $75
ARGP4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 strPoolIndex
INDIRI4
ARGI4
CNSTI4 393216
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 221
;221:	f = allocPoint;
ADDRLP4 0
ADDRGP4 allocPoint
INDIRI4
CVIF4 4
ASGNF4
line 222
;222:	f /= MEM_POOL_SIZE;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1233125376
DIVF4
ASGNF4
line 223
;223:	f *= 100;
ADDRLP4 0
CNSTF4 1120403456
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 224
;224:	Com_Printf("Memory Pool is %.1f%% full, %i bytes out of %i used.\n", f, allocPoint, MEM_POOL_SIZE);
ADDRGP4 $76
ARGP4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 allocPoint
INDIRI4
ARGI4
CNSTI4 1048576
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 225
;225:}
LABELV $72
endproc String_Report 4 16
export String_Init
proc String_Init 12 0
line 232
;226:
;227:/*
;228:=================
;229:String_Init
;230:=================
;231:*/
;232:void String_Init() {
line 234
;233:	int i;
;234:	for (i = 0; i < HASH_TABLE_SIZE; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $78
line 235
;235:		strHandle[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 strHandle
ADDP4
CNSTP4 0
ASGNP4
line 236
;236:	}
LABELV $79
line 234
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2048
LTI4 $78
line 237
;237:	strHandleCount = 0;
ADDRGP4 strHandleCount
CNSTI4 0
ASGNI4
line 238
;238:	strPoolIndex = 0;
ADDRGP4 strPoolIndex
CNSTI4 0
ASGNI4
line 239
;239:	menuCount = 0;
ADDRGP4 menuCount
CNSTI4 0
ASGNI4
line 240
;240:	openMenuCount = 0;
ADDRGP4 openMenuCount
CNSTI4 0
ASGNI4
line 241
;241:	UI_InitMemory();
ADDRGP4 UI_InitMemory
CALLV
pop
line 242
;242:	Item_SetupKeywordHash();
ADDRGP4 Item_SetupKeywordHash
CALLV
pop
line 243
;243:	Menu_SetupKeywordHash();
ADDRGP4 Menu_SetupKeywordHash
CALLV
pop
line 244
;244:	if (DC && DC->getBindingBuf) {
ADDRLP4 4
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $82
ADDRLP4 4
INDIRP4
CNSTI4 140
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $82
line 245
;245:		Controls_GetConfig();
ADDRGP4 Controls_GetConfig
CALLV
pop
line 246
;246:	}
LABELV $82
line 247
;247:}
LABELV $77
endproc String_Init 12 0
bss
align 1
LABELV $85
skip 4096
export PC_SourceWarning
code
proc PC_SourceWarning 136 16
line 254
;248:
;249:/*
;250:=================
;251:PC_SourceWarning
;252:=================
;253:*/
;254:void PC_SourceWarning(int handle, char *format, ...) {
line 260
;255:	int line;
;256:	char filename[128];
;257:	va_list argptr;
;258:	static char string[4096];
;259:
;260:	va_start (argptr, format);
ADDRLP4 132
ADDRFP4 4+4
ASGNP4
line 261
;261:	vsprintf (string, format, argptr);
ADDRGP4 $85
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 132
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 262
;262:	va_end (argptr);
ADDRLP4 132
CNSTP4 0
ASGNP4
line 264
;263:
;264:	filename[0] = '\0';
ADDRLP4 4
CNSTI1 0
ASGNI1
line 265
;265:	line = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 266
;266:	trap_PC_SourceFileAndLine(handle, filename, &line);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_PC_SourceFileAndLine
CALLI4
pop
line 268
;267:
;268:	Com_Printf(S_COLOR_YELLOW "WARNING: %s, line %d: %s\n", filename, line, string);
ADDRGP4 $87
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $85
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 269
;269:}
LABELV $84
endproc PC_SourceWarning 136 16
bss
align 1
LABELV $89
skip 4096
export PC_SourceError
code
proc PC_SourceError 136 16
line 276
;270:
;271:/*
;272:=================
;273:PC_SourceError
;274:=================
;275:*/
;276:void PC_SourceError(int handle, char *format, ...) {
line 282
;277:	int line;
;278:	char filename[128];
;279:	va_list argptr;
;280:	static char string[4096];
;281:
;282:	va_start (argptr, format);
ADDRLP4 132
ADDRFP4 4+4
ASGNP4
line 283
;283:	vsprintf (string, format, argptr);
ADDRGP4 $89
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 132
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 284
;284:	va_end (argptr);
ADDRLP4 132
CNSTP4 0
ASGNP4
line 286
;285:
;286:	filename[0] = '\0';
ADDRLP4 4
CNSTI1 0
ASGNI1
line 287
;287:	line = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 288
;288:	trap_PC_SourceFileAndLine(handle, filename, &line);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 trap_PC_SourceFileAndLine
CALLI4
pop
line 290
;289:
;290:	Com_Printf(S_COLOR_RED "ERROR: %s, line %d: %s\n", filename, line, string);
ADDRGP4 $91
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 $89
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 291
;291:}
LABELV $88
endproc PC_SourceError 136 16
export LerpColor
proc LerpColor 12 0
line 299
;292:
;293:/*
;294:=================
;295:LerpColor
;296:=================
;297:*/
;298:void LerpColor(vec4_t a, vec4_t b, vec4_t c, float t)
;299:{
line 303
;300:	int i;
;301:
;302:	// lerp and clamp each component
;303:	for (i=0; i<4; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $93
line 304
;304:	{
line 305
;305:		c[i] = a[i] + t*(b[i]-a[i]);
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ASGNF4
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 8
INDIRF4
ADDRFP4 12
INDIRF4
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
SUBF4
MULF4
ADDF4
ASGNF4
line 306
;306:		if (c[i] < 0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $97
line 307
;307:			c[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
CNSTF4 0
ASGNF4
ADDRGP4 $98
JUMPV
LABELV $97
line 308
;308:		else if (c[i] > 1.0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
CNSTF4 1065353216
LEF4 $99
line 309
;309:			c[i] = 1.0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
CNSTF4 1065353216
ASGNF4
LABELV $99
LABELV $98
line 310
;310:	}
LABELV $94
line 303
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $93
line 311
;311:}
LABELV $92
endproc LerpColor 12 0
export Float_Parse
proc Float_Parse 16 8
line 318
;312:
;313:/*
;314:=================
;315:Float_Parse
;316:=================
;317:*/
;318:qboolean Float_Parse(char **p, float *f) {
line 320
;319:	char	*token;
;320:	token = COM_ParseExt(p, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 321
;321:	if (token && token[0] != 0) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $102
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $102
line 322
;322:		*f = atof(token);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atof
CALLF4
ASGNF4
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
line 323
;323:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $101
JUMPV
LABELV $102
line 324
;324:	} else {
line 325
;325:		return qfalse;
CNSTI4 0
RETI4
LABELV $101
endproc Float_Parse 16 8
export PC_Float_Parse
proc PC_Float_Parse 1052 12
line 334
;326:	}
;327:}
;328:
;329:/*
;330:=================
;331:PC_Float_Parse
;332:=================
;333:*/
;334:qboolean PC_Float_Parse(int handle, float *f) {
line 336
;335:	pc_token_t token;
;336:	int negative = qfalse;
ADDRLP4 1040
CNSTI4 0
ASGNI4
line 338
;337:
;338:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $105
line 339
;339:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $104
JUMPV
LABELV $105
line 340
;340:	if (token.string[0] == '-') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 45
NEI4 $107
line 341
;341:		if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $110
line 342
;342:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $104
JUMPV
LABELV $110
line 343
;343:		negative = qtrue;
ADDRLP4 1040
CNSTI4 1
ASGNI4
line 344
;344:	}
LABELV $107
line 345
;345:	if (token.type != TT_NUMBER) {
ADDRLP4 0
INDIRI4
CNSTI4 3
EQI4 $112
line 346
;346:		PC_SourceError(handle, "expected float but found %s\n", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $114
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 347
;347:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $104
JUMPV
LABELV $112
line 349
;348:	}
;349:	if (negative)
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $116
line 350
;350:		*f = -token.floatvalue;
ADDRFP4 4
INDIRP4
ADDRLP4 0+12
INDIRF4
NEGF4
ASGNF4
ADDRGP4 $117
JUMPV
LABELV $116
line 352
;351:	else
;352:		*f = token.floatvalue;
ADDRFP4 4
INDIRP4
ADDRLP4 0+12
INDIRF4
ASGNF4
LABELV $117
line 353
;353:	return qtrue;
CNSTI4 1
RETI4
LABELV $104
endproc PC_Float_Parse 1052 12
export Color_Parse
proc Color_Parse 12 8
line 361
;354:}
;355:
;356:/*
;357:=================
;358:Color_Parse
;359:=================
;360:*/
;361:qboolean Color_Parse(char **p, vec4_t *c) {
line 365
;362:	int i;
;363:	float f;
;364:
;365:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $121
line 366
;366:		if (!Float_Parse(p, &f)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $125
line 367
;367:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $120
JUMPV
LABELV $125
line 369
;368:		}
;369:		(*c)[i] = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 370
;370:	}
LABELV $122
line 365
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $121
line 371
;371:	return qtrue;
CNSTI4 1
RETI4
LABELV $120
endproc Color_Parse 12 8
export PC_Color_Parse
proc PC_Color_Parse 12 8
line 379
;372:}
;373:
;374:/*
;375:=================
;376:PC_Color_Parse
;377:=================
;378:*/
;379:qboolean PC_Color_Parse(int handle, vec4_t *c) {
line 383
;380:	int i;
;381:	float f;
;382:
;383:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $128
line 384
;384:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $132
line 385
;385:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $127
JUMPV
LABELV $132
line 387
;386:		}
;387:		(*c)[i] = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 388
;388:	}
LABELV $129
line 383
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $128
line 389
;389:	return qtrue;
CNSTI4 1
RETI4
LABELV $127
endproc PC_Color_Parse 12 8
export Int_Parse
proc Int_Parse 16 8
line 397
;390:}
;391:
;392:/*
;393:=================
;394:Int_Parse
;395:=================
;396:*/
;397:qboolean Int_Parse(char **p, int *i) {
line 399
;398:	char	*token;
;399:	token = COM_ParseExt(p, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 401
;400:
;401:	if (token && token[0] != 0) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $135
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $135
line 402
;402:		*i = atoi(token);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 403
;403:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $134
JUMPV
LABELV $135
line 404
;404:	} else {
line 405
;405:		return qfalse;
CNSTI4 0
RETI4
LABELV $134
endproc Int_Parse 16 8
export PC_Int_Parse
proc PC_Int_Parse 1052 12
line 414
;406:	}
;407:}
;408:
;409:/*
;410:=================
;411:PC_Int_Parse
;412:=================
;413:*/
;414:qboolean PC_Int_Parse(int handle, int *i) {
line 416
;415:	pc_token_t token;
;416:	int negative = qfalse;
ADDRLP4 1040
CNSTI4 0
ASGNI4
line 418
;417:
;418:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $138
line 419
;419:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $137
JUMPV
LABELV $138
line 420
;420:	if (token.string[0] == '-') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 45
NEI4 $140
line 421
;421:		if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $143
line 422
;422:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $137
JUMPV
LABELV $143
line 423
;423:		negative = qtrue;
ADDRLP4 1040
CNSTI4 1
ASGNI4
line 424
;424:	}
LABELV $140
line 425
;425:	if (token.type != TT_NUMBER) {
ADDRLP4 0
INDIRI4
CNSTI4 3
EQI4 $145
line 426
;426:		PC_SourceError(handle, "expected integer but found %s\n", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $147
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 427
;427:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $137
JUMPV
LABELV $145
line 429
;428:	}
;429:	*i = token.intvalue;
ADDRFP4 4
INDIRP4
ADDRLP4 0+8
INDIRI4
ASGNI4
line 430
;430:	if (negative)
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $150
line 431
;431:		*i = - *i;
ADDRLP4 1048
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 1048
INDIRP4
ADDRLP4 1048
INDIRP4
INDIRI4
NEGI4
ASGNI4
LABELV $150
line 432
;432:	return qtrue;
CNSTI4 1
RETI4
LABELV $137
endproc PC_Int_Parse 1052 12
export Rect_Parse
proc Rect_Parse 16 8
line 440
;433:}
;434:
;435:/*
;436:=================
;437:Rect_Parse
;438:=================
;439:*/
;440:qboolean Rect_Parse(char **p, rectDef_t *r) {
line 441
;441:	if (Float_Parse(p, &r->x)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $153
line 442
;442:		if (Float_Parse(p, &r->y)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $155
line 443
;443:			if (Float_Parse(p, &r->w)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $157
line 444
;444:				if (Float_Parse(p, &r->h)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $159
line 445
;445:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $152
JUMPV
LABELV $159
line 447
;446:				}
;447:			}
LABELV $157
line 448
;448:		}
LABELV $155
line 449
;449:	}
LABELV $153
line 450
;450:	return qfalse;
CNSTI4 0
RETI4
LABELV $152
endproc Rect_Parse 16 8
export PC_Rect_Parse
proc PC_Rect_Parse 16 8
line 458
;451:}
;452:
;453:/*
;454:=================
;455:PC_Rect_Parse
;456:=================
;457:*/
;458:qboolean PC_Rect_Parse(int handle, rectDef_t *r) {
line 459
;459:	if (PC_Float_Parse(handle, &r->x)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $162
line 460
;460:		if (PC_Float_Parse(handle, &r->y)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $164
line 461
;461:			if (PC_Float_Parse(handle, &r->w)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $166
line 462
;462:				if (PC_Float_Parse(handle, &r->h)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $168
line 463
;463:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $161
JUMPV
LABELV $168
line 465
;464:				}
;465:			}
LABELV $166
line 466
;466:		}
LABELV $164
line 467
;467:	}
LABELV $162
line 468
;468:	return qfalse;
CNSTI4 0
RETI4
LABELV $161
endproc PC_Rect_Parse 16 8
export String_Parse
proc String_Parse 16 8
line 476
;469:}
;470:
;471:/*
;472:=================
;473:String_Parse
;474:=================
;475:*/
;476:qboolean String_Parse(char **p, const char **out) {
line 479
;477:	char *token;
;478:
;479:	token = COM_ParseExt(p, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 480
;480:	if (token && token[0] != 0) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $171
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $171
line 481
;481:		*(out) = String_Alloc(token);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRP4
ASGNP4
line 482
;482:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $170
JUMPV
LABELV $171
line 484
;483:	}
;484:	return qfalse;
CNSTI4 0
RETI4
LABELV $170
endproc String_Parse 16 8
export PC_String_Parse
proc PC_String_Parse 1048 8
line 492
;485:}
;486:
;487:/*
;488:=================
;489:PC_String_Parse
;490:=================
;491:*/
;492:qboolean PC_String_Parse(int handle, const char **out) {
line 495
;493:	pc_token_t token;
;494:
;495:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1040
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $174
line 496
;496:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $173
JUMPV
LABELV $174
line 498
;497:	
;498:	*(out) = String_Alloc(token.string);
ADDRLP4 0+16
ARGP4
ADDRLP4 1044
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRFP4 4
INDIRP4
ADDRLP4 1044
INDIRP4
ASGNP4
line 499
;499:    return qtrue;
CNSTI4 1
RETI4
LABELV $173
endproc PC_String_Parse 1048 8
export PC_Script_Parse
proc PC_Script_Parse 2084 12
line 507
;500:}
;501:
;502:/*
;503:=================
;504:PC_Script_Parse
;505:=================
;506:*/
;507:qboolean PC_Script_Parse(int handle, const char **out) {
line 511
;508:	char script[1024];
;509:	pc_token_t token;
;510:
;511:	memset(script, 0, sizeof(script));
ADDRLP4 1040
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 515
;512:	// scripts start with { and have ; separated command lists.. commands are command, arg.. 
;513:	// basically we want everything between the { } as it will be interpreted at run time
;514:  
;515:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 2064
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 2064
INDIRI4
CNSTI4 0
NEI4 $178
line 516
;516:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $177
JUMPV
LABELV $178
line 517
;517:	if (Q_stricmp(token.string, "{") != 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $183
ARGP4
ADDRLP4 2068
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
EQI4 $185
line 518
;518:	    return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $177
JUMPV
LABELV $184
line 521
;519:	}
;520:
;521:	while ( 1 ) {
line 522
;522:		if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 2072
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 2072
INDIRI4
CNSTI4 0
NEI4 $187
line 523
;523:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $177
JUMPV
LABELV $187
line 525
;524:
;525:		if (Q_stricmp(token.string, "}") == 0) {
ADDRLP4 0+16
ARGP4
ADDRGP4 $192
ARGP4
ADDRLP4 2076
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 2076
INDIRI4
CNSTI4 0
NEI4 $189
line 526
;526:			*out = String_Alloc(script);
ADDRLP4 1040
ARGP4
ADDRLP4 2080
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRFP4 4
INDIRP4
ADDRLP4 2080
INDIRP4
ASGNP4
line 527
;527:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $177
JUMPV
LABELV $189
line 530
;528:		}
;529:
;530:		if (token.string[1] != '\0') {
ADDRLP4 0+16+1
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $193
line 531
;531:			Q_strcat(script, 1024, va("\"%s\"", token.string));
ADDRGP4 $197
ARGP4
ADDRLP4 0+16
ARGP4
ADDRLP4 2080
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1040
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2080
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 532
;532:		} else {
ADDRGP4 $194
JUMPV
LABELV $193
line 533
;533:			Q_strcat(script, 1024, token.string);
ADDRLP4 1040
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 0+16
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 534
;534:		}
LABELV $194
line 535
;535:		Q_strcat(script, 1024, " ");
ADDRLP4 1040
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 536
;536:	}
LABELV $185
line 521
ADDRGP4 $184
JUMPV
line 537
;537:	return qfalse; 	// bk001105 - LCC   missing return value
CNSTI4 0
RETI4
LABELV $177
endproc PC_Script_Parse 2084 12
export Init_Display
proc Init_Display 0 0
line 550
;538:}
;539:
;540:// display, window, menu, item code
;541:// 
;542:
;543:/*
;544:==================
;545:Init_Display
;546:
;547:Initializes the display with a structure to all the drawing routines
;548: ==================
;549:*/
;550:void Init_Display(displayContextDef_t *dc) {
line 551
;551:	DC = dc;
ADDRGP4 DC
ADDRFP4 0
INDIRP4
ASGNP4
line 552
;552:}
LABELV $201
endproc Init_Display 0 0
export GradientBar_Paint
proc GradientBar_Paint 12 20
line 558
;553:
;554:
;555:
;556:// type and style painting 
;557:
;558:void GradientBar_Paint(rectDef_t *rect, vec4_t color) {
line 560
;559:	// gradient bar takes two paints
;560:	DC->setColor( color );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 561
;561:	DC->drawHandlePic(rect->x, rect->y, rect->w, rect->h, DC->Assets.gradientBar);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
CNSTI4 8
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 61888
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRP4
CALLV
pop
line 562
;562:	DC->setColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 563
;563:}
LABELV $202
endproc GradientBar_Paint 12 20
export Window_Init
proc Window_Init 8 12
line 574
;564:
;565:
;566:/*
;567:==================
;568:Window_Init
;569:
;570:Initializes a window structure ( windowDef_t ) with defaults
;571: 
;572:==================
;573:*/
;574:void Window_Init(Window *w) {
line 575
;575:	memset(w, 0, sizeof(windowDef_t));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 180
ARGI4
ADDRGP4 memset
CALLP4
pop
line 576
;576:	w->borderSize = 1;
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
CNSTF4 1065353216
ASGNF4
line 577
;577:	w->foreColor[0] = w->foreColor[1] = w->foreColor[2] = w->foreColor[3] = 1.0;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 578
;578:	w->cinematic = -1;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 -1
ASGNI4
line 579
;579:}
LABELV $203
endproc Window_Init 8 12
export Fade
proc Fade 8 0
line 581
;580:
;581:void Fade(int *flags, float *f, float clamp, int *nextTime, int offsetTime, qboolean bFlags, float fadeAmount) {
line 582
;582:  if (*flags & (WINDOW_FADINGOUT | WINDOW_FADINGIN)) {
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 96
BANDI4
CNSTI4 0
EQI4 $205
line 583
;583:    if (DC->realTime > *nextTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRFP4 12
INDIRP4
INDIRI4
LEI4 $207
line 584
;584:      *nextTime = DC->realTime + offsetTime;
ADDRFP4 12
INDIRP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRFP4 16
INDIRI4
ADDI4
ASGNI4
line 585
;585:      if (*flags & WINDOW_FADINGOUT) {
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
EQI4 $209
line 586
;586:        *f -= fadeAmount;
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRFP4 24
INDIRF4
SUBF4
ASGNF4
line 587
;587:        if (bFlags && *f <= 0.0) {
ADDRFP4 20
INDIRI4
CNSTI4 0
EQI4 $210
ADDRFP4 4
INDIRP4
INDIRF4
CNSTF4 0
GTF4 $210
line 588
;588:          *flags &= ~(WINDOW_FADINGOUT | WINDOW_VISIBLE);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -37
BANDI4
ASGNI4
line 589
;589:        }
line 590
;590:      } else {
ADDRGP4 $210
JUMPV
LABELV $209
line 591
;591:        *f += fadeAmount;
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRFP4 24
INDIRF4
ADDF4
ASGNF4
line 592
;592:        if (*f >= clamp) {
ADDRFP4 4
INDIRP4
INDIRF4
ADDRFP4 8
INDIRF4
LTF4 $213
line 593
;593:          *f = clamp;
ADDRFP4 4
INDIRP4
ADDRFP4 8
INDIRF4
ASGNF4
line 594
;594:          if (bFlags) {
ADDRFP4 20
INDIRI4
CNSTI4 0
EQI4 $215
line 595
;595:            *flags &= ~WINDOW_FADINGIN;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -65
BANDI4
ASGNI4
line 596
;596:          }
LABELV $215
line 597
;597:        }
LABELV $213
line 598
;598:      }
LABELV $210
line 599
;599:    }
LABELV $207
line 600
;600:  }
LABELV $205
line 601
;601:}
LABELV $204
endproc Fade 8 0
export Window_Paint
proc Window_Paint 60 28
line 605
;602:
;603:
;604:
;605:void Window_Paint(Window *w, float fadeAmount, float fadeClamp, float fadeCycle) {
line 608
;606:  //float bordersize = 0;
;607:  vec4_t color;
;608:  rectDef_t fillRect = w->rect;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 16
line 611
;609:
;610:
;611:  if (debugMode) {
ADDRGP4 debugMode
INDIRI4
CNSTI4 0
EQI4 $218
line 612
;612:    color[0] = color[1] = color[2] = color[3] = 1;
ADDRLP4 32
CNSTF4 1065353216
ASGNF4
ADDRLP4 16+12
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 16+8
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 16
ADDRLP4 32
INDIRF4
ASGNF4
line 613
;613:    DC->drawRect(w->rect.x, w->rect.y, w->rect.w, w->rect.h, 1, color);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
INDIRF4
ARGF4
ADDRLP4 36
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 36
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 36
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 16
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 614
;614:  }
LABELV $218
line 616
;615:
;616:  if (w == NULL || (w->style == 0 && w->border == 0)) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $225
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRLP4 32
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
NEI4 $223
ADDRLP4 32
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
NEI4 $223
LABELV $225
line 617
;617:    return;
ADDRGP4 $217
JUMPV
LABELV $223
line 620
;618:  }
;619:
;620:  if (w->border != 0) {
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $226
line 621
;621:    fillRect.x += w->borderSize;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 622
;622:    fillRect.y += w->borderSize;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 623
;623:    fillRect.w -= w->borderSize + 1;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
SUBF4
ASGNF4
line 624
;624:    fillRect.h -= w->borderSize + 1;
ADDRLP4 0+12
ADDRLP4 0+12
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
SUBF4
ASGNF4
line 625
;625:  }
LABELV $226
line 627
;626:
;627:  if (w->style == WINDOW_STYLE_FILLED) {
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 1
NEI4 $231
line 629
;628:    // box, but possible a shader that needs filled
;629:		if (w->background) {
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
CNSTI4 0
EQI4 $233
line 630
;630:		  Fade(&w->flags, &w->backColor[3], fadeClamp, &w->nextTime, fadeCycle, qtrue, fadeAmount);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRLP4 40
INDIRP4
CNSTI4 140
ADDP4
ARGP4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 108
ADDP4
ARGP4
ADDRFP4 12
INDIRF4
CVFI4 4
ARGI4
CNSTI4 1
ARGI4
ADDRFP4 4
INDIRF4
ARGF4
ADDRGP4 Fade
CALLV
pop
line 631
;631:      DC->setColor(w->backColor);
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 632
;632:	    DC->drawHandlePic(fillRect.x, fillRect.y, fillRect.w, fillRect.h, w->background);
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 633
;633:		  DC->setColor(NULL);
CNSTP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 634
;634:		} else {
ADDRGP4 $232
JUMPV
LABELV $233
line 635
;635:	    DC->fillRect(fillRect.x, fillRect.y, fillRect.w, fillRect.h, w->backColor);
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
CALLV
pop
line 636
;636:		}
line 637
;637:  } else if (w->style == WINDOW_STYLE_GRADIENT) {
ADDRGP4 $232
JUMPV
LABELV $231
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 2
NEI4 $241
line 638
;638:    GradientBar_Paint(&fillRect, w->backColor);
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ARGP4
ADDRGP4 GradientBar_Paint
CALLV
pop
line 640
;639:    // gradient bar
;640:  } else if (w->style == WINDOW_STYLE_SHADER) {
ADDRGP4 $242
JUMPV
LABELV $241
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 3
NEI4 $243
line 641
;641:    if (w->flags & WINDOW_FORECOLORSET) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 512
BANDI4
CNSTI4 0
EQI4 $245
line 642
;642:      DC->setColor(w->foreColor);
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 643
;643:    }
LABELV $245
line 644
;644:    DC->drawHandlePic(fillRect.x, fillRect.y, fillRect.w, fillRect.h, w->background);
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 645
;645:    DC->setColor(NULL);
CNSTP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 646
;646:  } else if (w->style == WINDOW_STYLE_TEAMCOLOR) {
ADDRGP4 $244
JUMPV
LABELV $243
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 4
NEI4 $250
line 647
;647:    if (DC->getTeamColor) {
ADDRGP4 DC
INDIRP4
CNSTI4 84
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $251
line 648
;648:      DC->getTeamColor(&color);
ADDRLP4 16
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 84
ADDP4
INDIRP4
CALLV
pop
line 649
;649:      DC->fillRect(fillRect.x, fillRect.y, fillRect.w, fillRect.h, color);
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRLP4 16
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
CALLV
pop
line 650
;650:    }
line 651
;651:  } else if (w->style == WINDOW_STYLE_CINEMATIC) {
ADDRGP4 $251
JUMPV
LABELV $250
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 5
NEI4 $257
line 652
;652:		if (w->cinematic == -1) {
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $259
line 653
;653:			w->cinematic = DC->playCinematic(w->cinematicName, fillRect.x, fillRect.y, fillRect.w, fillRect.h);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRLP4 44
ADDRGP4 DC
INDIRP4
CNSTI4 180
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 40
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 44
INDIRI4
ASGNI4
line 654
;654:			if (w->cinematic == -1) {
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $264
line 655
;655:				w->cinematic = -2;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 -2
ASGNI4
line 656
;656:			}
LABELV $264
line 657
;657:		} 
LABELV $259
line 658
;658:		if (w->cinematic >= 0) {
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LTI4 $266
line 659
;659:	    DC->runCinematicFrame(w->cinematic);
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 192
ADDP4
INDIRP4
CALLV
pop
line 660
;660:			DC->drawCinematic(w->cinematic, fillRect.x, fillRect.y, fillRect.w, fillRect.h);
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0+4
INDIRF4
ARGF4
ADDRLP4 0+8
INDIRF4
ARGF4
ADDRLP4 0+12
INDIRF4
ARGF4
ADDRGP4 DC
INDIRP4
CNSTI4 188
ADDP4
INDIRP4
CALLV
pop
line 661
;661:		}
LABELV $266
line 662
;662:  }
LABELV $257
LABELV $251
LABELV $244
LABELV $242
LABELV $232
line 664
;663:
;664:  if (w->border == WINDOW_BORDER_FULL) {
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 1
NEI4 $271
line 667
;665:    // full
;666:    // HACK HACK HACK
;667:    if (w->style == WINDOW_STYLE_TEAMCOLOR) {
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 4
NEI4 $273
line 668
;668:      if (color[0] > 0) { 
ADDRLP4 16
INDIRF4
CNSTF4 0
LEF4 $275
line 670
;669:        // red
;670:        color[0] = 1;
ADDRLP4 16
CNSTF4 1065353216
ASGNF4
line 671
;671:        color[1] = color[2] = .5;
ADDRLP4 40
CNSTF4 1056964608
ASGNF4
ADDRLP4 16+8
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 40
INDIRF4
ASGNF4
line 673
;672:
;673:      } else {
ADDRGP4 $276
JUMPV
LABELV $275
line 674
;674:        color[2] = 1;
ADDRLP4 16+8
CNSTF4 1065353216
ASGNF4
line 675
;675:        color[0] = color[1] = .5;
ADDRLP4 40
CNSTF4 1056964608
ASGNF4
ADDRLP4 16+4
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 16
ADDRLP4 40
INDIRF4
ASGNF4
line 676
;676:      }
LABELV $276
line 677
;677:      color[3] = 1;
ADDRLP4 16+12
CNSTF4 1065353216
ASGNF4
line 678
;678:      DC->drawRect(w->rect.x, w->rect.y, w->rect.w, w->rect.h, w->borderSize, color);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 679
;679:    } else {
ADDRGP4 $272
JUMPV
LABELV $273
line 680
;680:      DC->drawRect(w->rect.x, w->rect.y, w->rect.w, w->rect.h, w->borderSize, w->borderColor);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 681
;681:    }
line 682
;682:  } else if (w->border == WINDOW_BORDER_HORZ) {
ADDRGP4 $272
JUMPV
LABELV $271
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 2
NEI4 $282
line 684
;683:    // top/bottom
;684:    DC->setColor(w->borderColor);
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 685
;685:    DC->drawTopBottom(w->rect.x, w->rect.y, w->rect.w, w->rect.h, w->borderSize);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRGP4 DC
INDIRP4
CNSTI4 48
ADDP4
INDIRP4
CALLV
pop
line 686
;686:  	DC->setColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 687
;687:  } else if (w->border == WINDOW_BORDER_VERT) {
ADDRGP4 $283
JUMPV
LABELV $282
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 3
NEI4 $284
line 689
;688:    // left right
;689:    DC->setColor(w->borderColor);
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 690
;690:    DC->drawSides(w->rect.x, w->rect.y, w->rect.w, w->rect.h, w->borderSize);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRGP4 DC
INDIRP4
CNSTI4 44
ADDP4
INDIRP4
CALLV
pop
line 691
;691:  	DC->setColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 692
;692:  } else if (w->border == WINDOW_BORDER_KCGRADIENT) {
ADDRGP4 $285
JUMPV
LABELV $284
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 4
NEI4 $286
line 694
;693:    // this is just two gradient bars along each horz edge
;694:    rectDef_t r = w->rect;
ADDRLP4 40
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 16
line 695
;695:    r.h = w->borderSize;
ADDRLP4 40+12
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ASGNF4
line 696
;696:    GradientBar_Paint(&r, w->borderColor);
ADDRLP4 40
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 GradientBar_Paint
CALLV
pop
line 697
;697:    r.y = w->rect.y + w->rect.h - 1;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40+4
ADDRLP4 56
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 698
;698:    GradientBar_Paint(&r, w->borderColor);
ADDRLP4 40
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 GradientBar_Paint
CALLV
pop
line 699
;699:  }
LABELV $286
LABELV $285
LABELV $283
LABELV $272
line 701
;700:
;701:}
LABELV $217
endproc Window_Paint 60 28
export Item_SetScreenCoords
proc Item_SetScreenCoords 16 0
line 704
;702:
;703:
;704:void Item_SetScreenCoords(itemDef_t *item, float x, float y) {
line 706
;705:  
;706:  if (item == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $291
line 707
;707:    return;
ADDRGP4 $290
JUMPV
LABELV $291
line 710
;708:  }
;709:
;710:  if (item->window.border != 0) {
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $293
line 711
;711:    x += item->window.borderSize;
ADDRFP4 4
ADDRFP4 4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 712
;712:    y += item->window.borderSize;
ADDRFP4 8
ADDRFP4 8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 713
;713:  }
LABELV $293
line 715
;714:
;715:  item->window.rect.x = x + item->window.rectClient.x;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRFP4 4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDF4
ASGNF4
line 716
;716:  item->window.rect.y = y + item->window.rectClient.y;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 8
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
ASGNF4
line 717
;717:  item->window.rect.w = item->window.rectClient.w;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
line 718
;718:  item->window.rect.h = item->window.rectClient.h;
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
line 721
;719:
;720:  // force the text rects to recompute
;721:  item->textRect.w = 0;
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
CNSTF4 0
ASGNF4
line 722
;722:  item->textRect.h = 0;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
CNSTF4 0
ASGNF4
line 723
;723:}
LABELV $290
endproc Item_SetScreenCoords 16 0
export Item_UpdatePosition
proc Item_UpdatePosition 20 12
line 726
;724:
;725:// FIXME: consolidate this with nearby stuff
;726:void Item_UpdatePosition(itemDef_t *item) {
line 730
;727:  float x, y;
;728:  menuDef_t *menu;
;729:  
;730:  if (item == NULL || item->parent == NULL) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
CNSTU4 0
ASGNU4
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $298
ADDRLP4 12
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
NEU4 $296
LABELV $298
line 731
;731:    return;
ADDRGP4 $295
JUMPV
LABELV $296
line 734
;732:  }
;733:
;734:  menu = item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 736
;735:
;736:  x = menu->window.rect.x;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRF4
ASGNF4
line 737
;737:  y = menu->window.rect.y;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 739
;738:  
;739:  if (menu->window.border != 0) {
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $299
line 740
;740:    x += menu->window.borderSize;
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 741
;741:    y += menu->window.borderSize;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 742
;742:  }
LABELV $299
line 744
;743:
;744:  Item_SetScreenCoords(item, x, y);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 Item_SetScreenCoords
CALLV
pop
line 746
;745:
;746:}
LABELV $295
endproc Item_UpdatePosition 20 12
export Menu_UpdatePosition
proc Menu_UpdatePosition 12 12
line 749
;747:
;748:// menus
;749:void Menu_UpdatePosition(menuDef_t *menu) {
line 753
;750:  int i;
;751:  float x, y;
;752:
;753:  if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $302
line 754
;754:    return;
ADDRGP4 $301
JUMPV
LABELV $302
line 757
;755:  }
;756:  
;757:  x = menu->window.rect.x;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 758
;758:  y = menu->window.rect.y;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 759
;759:  if (menu->window.border != 0) {
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $304
line 760
;760:    x += menu->window.borderSize;
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 761
;761:    y += menu->window.borderSize;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 762
;762:  }
LABELV $304
line 764
;763:
;764:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $309
JUMPV
LABELV $306
line 765
;765:    Item_SetScreenCoords(menu->items[i], x, y);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 Item_SetScreenCoords
CALLV
pop
line 766
;766:  }
LABELV $307
line 764
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $309
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $306
line 767
;767:}
LABELV $301
endproc Menu_UpdatePosition 12 12
export Menu_PostParse
proc Menu_PostParse 0 4
line 769
;768:
;769:void Menu_PostParse(menuDef_t *menu) {
line 770
;770:	if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $311
line 771
;771:		return;
ADDRGP4 $310
JUMPV
LABELV $311
line 773
;772:	}
;773:	if (menu->fullScreen) {
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
EQI4 $313
line 774
;774:		menu->window.rect.x = 0;
ADDRFP4 0
INDIRP4
CNSTF4 0
ASGNF4
line 775
;775:		menu->window.rect.y = 0;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTF4 0
ASGNF4
line 776
;776:		menu->window.rect.w = 640;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTF4 1142947840
ASGNF4
line 777
;777:		menu->window.rect.h = 480;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTF4 1139802112
ASGNF4
line 778
;778:	}
LABELV $313
line 779
;779:	Menu_UpdatePosition(menu);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_UpdatePosition
CALLV
pop
line 780
;780:}
LABELV $310
endproc Menu_PostParse 0 4
export Menu_ClearFocus
proc Menu_ClearFocus 28 8
line 782
;781:
;782:itemDef_t *Menu_ClearFocus(menuDef_t *menu) {
line 784
;783:  int i;
;784:  itemDef_t *ret = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 786
;785:
;786:  if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $316
line 787
;787:    return NULL;
CNSTP4 0
RETP4
ADDRGP4 $315
JUMPV
LABELV $316
line 790
;788:  }
;789:
;790:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $321
JUMPV
LABELV $318
line 791
;791:    if (menu->items[i]->window.flags & WINDOW_HASFOCUS) {
ADDRLP4 8
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
BANDI4
CNSTI4 0
EQI4 $322
line 792
;792:      ret = menu->items[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
line 793
;793:    } 
LABELV $322
line 794
;794:    menu->items[i]->window.flags &= ~WINDOW_HASFOCUS;
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 795
;795:    if (menu->items[i]->leaveFocus) {
ADDRLP4 16
CNSTI4 260
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ADDP4
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $324
line 796
;796:      Item_RunScript(menu->items[i], menu->items[i]->leaveFocus);
ADDRLP4 20
CNSTI4 260
ASGNI4
ADDRLP4 24
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 797
;797:    }
LABELV $324
line 798
;798:  }
LABELV $319
line 790
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $321
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $318
line 800
;799: 
;800:  return ret;
ADDRLP4 4
INDIRP4
RETP4
LABELV $315
endproc Menu_ClearFocus 28 8
export IsVisible
proc IsVisible 12 0
line 803
;801:}
;802:
;803:qboolean IsVisible(int flags) {
line 804
;804:  return (flags & WINDOW_VISIBLE && !(flags & WINDOW_FADINGOUT));
ADDRLP4 4
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 4
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $328
ADDRLP4 4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 8
INDIRI4
NEI4 $328
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $329
JUMPV
LABELV $328
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $329
ADDRLP4 0
INDIRI4
RETI4
LABELV $326
endproc IsVisible 12 0
export Rect_ContainsPoint
proc Rect_ContainsPoint 20 0
line 807
;805:}
;806:
;807:qboolean Rect_ContainsPoint(rectDef_t *rect, float x, float y) {
line 808
;808:  if (rect) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $331
line 809
;809:    if (x > rect->x && x < rect->x + rect->w && y > rect->y && y < rect->y + rect->h) {
ADDRLP4 0
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 4
INDIRP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
ADDRLP4 8
INDIRF4
LEF4 $333
ADDRLP4 0
INDIRF4
ADDRLP4 8
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
GEF4 $333
ADDRLP4 12
ADDRFP4 8
INDIRF4
ASGNF4
ADDRLP4 16
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
ADDRLP4 12
INDIRF4
ADDRLP4 16
INDIRF4
LEF4 $333
ADDRLP4 12
INDIRF4
ADDRLP4 16
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
GEF4 $333
line 810
;810:      return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $330
JUMPV
LABELV $333
line 812
;811:    }
;812:  }
LABELV $331
line 813
;813:  return qfalse;
CNSTI4 0
RETI4
LABELV $330
endproc Rect_ContainsPoint 20 0
export Menu_ItemsMatchingGroup
proc Menu_ItemsMatchingGroup 20 8
line 816
;814:}
;815:
;816:int Menu_ItemsMatchingGroup(menuDef_t *menu, const char *name) {
line 818
;817:  int i;
;818:  int count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 819
;819:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $339
JUMPV
LABELV $336
line 820
;820:    if (Q_stricmp(menu->items[i]->window.name, name) == 0 || (menu->items[i]->window.group && Q_stricmp(menu->items[i]->window.group, name) == 0)) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $342
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $340
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $340
LABELV $342
line 821
;821:      count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 822
;822:    } 
LABELV $340
line 823
;823:  }
LABELV $337
line 819
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $339
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $336
line 824
;824:  return count;
ADDRLP4 4
INDIRI4
RETI4
LABELV $335
endproc Menu_ItemsMatchingGroup 20 8
export Menu_GetMatchingItemByNumber
proc Menu_GetMatchingItemByNumber 20 8
line 827
;825:}
;826:
;827:itemDef_t *Menu_GetMatchingItemByNumber(menuDef_t *menu, int index, const char *name) {
line 829
;828:  int i;
;829:  int count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 830
;830:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $347
JUMPV
LABELV $344
line 831
;831:    if (Q_stricmp(menu->items[i]->window.name, name) == 0 || (menu->items[i]->window.group && Q_stricmp(menu->items[i]->window.group, name) == 0)) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $350
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $348
ADDRLP4 12
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $348
LABELV $350
line 832
;832:      if (count == index) {
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $351
line 833
;833:        return menu->items[i];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $343
JUMPV
LABELV $351
line 835
;834:      }
;835:      count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 836
;836:    } 
LABELV $348
line 837
;837:  }
LABELV $345
line 830
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $347
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $344
line 838
;838:  return NULL;
CNSTP4 0
RETP4
LABELV $343
endproc Menu_GetMatchingItemByNumber 20 8
export Script_SetColor
proc Script_SetColor 36 8
line 843
;839:}
;840:
;841:
;842:
;843:void Script_SetColor(itemDef_t *item, char **args) {
line 849
;844:  const char *name;
;845:  int i;
;846:  float f;
;847:  vec4_t *out;
;848:  // expecting type of color to set and 4 args for the color
;849:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 16
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $354
line 850
;850:      out = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 851
;851:      if (Q_stricmp(name, "backcolor") == 0) {
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $358
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $356
line 852
;852:        out = &item->window.backColor;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ASGNP4
line 853
;853:        item->window.flags |= WINDOW_BACKCOLORSET;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 4194304
BORI4
ASGNI4
line 854
;854:      } else if (Q_stricmp(name, "forecolor") == 0) {
ADDRGP4 $357
JUMPV
LABELV $356
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $361
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $359
line 855
;855:        out = &item->window.foreColor;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
line 856
;856:        item->window.flags |= WINDOW_FORECOLORSET;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 857
;857:      } else if (Q_stricmp(name, "bordercolor") == 0) {
ADDRGP4 $360
JUMPV
LABELV $359
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $364
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $362
line 858
;858:        out = &item->window.borderColor;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ASGNP4
line 859
;859:      }
LABELV $362
LABELV $360
LABELV $357
line 861
;860:
;861:      if (out) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $365
line 862
;862:        for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $367
line 863
;863:          if (!Float_Parse(args, &f)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 32
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $371
line 864
;864:            return;
ADDRGP4 $353
JUMPV
LABELV $371
line 866
;865:          }
;866:          (*out)[i] = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 867
;867:        }
LABELV $368
line 862
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $367
line 868
;868:      }
LABELV $365
line 869
;869:  }
LABELV $354
line 870
;870:}
LABELV $353
endproc Script_SetColor 36 8
export Script_SetAsset
proc Script_SetAsset 8 8
line 872
;871:
;872:void Script_SetAsset(itemDef_t *item, char **args) {
line 875
;873:  const char *name;
;874:  // expecting name to set asset to
;875:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $374
line 877
;876:    // check for a model 
;877:    if (item->type == ITEM_TYPE_MODEL) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 7
NEI4 $376
line 878
;878:    }
LABELV $376
line 879
;879:  }
LABELV $374
line 880
;880:}
LABELV $373
endproc Script_SetAsset 8 8
export Script_SetBackground
proc Script_SetBackground 12 8
line 882
;881:
;882:void Script_SetBackground(itemDef_t *item, char **args) {
line 885
;883:  const char *name;
;884:  // expecting name to set asset to
;885:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $379
line 886
;886:    item->window.background = DC->registerShaderNoMip(name);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 887
;887:  }
LABELV $379
line 888
;888:}
LABELV $378
endproc Script_SetBackground 12 8
export Menu_FindItemByName
proc Menu_FindItemByName 12 8
line 893
;889:
;890:
;891:
;892:
;893:itemDef_t *Menu_FindItemByName(menuDef_t *menu, const char *p) {
line 895
;894:  int i;
;895:  if (menu == NULL || p == NULL) {
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $384
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
NEU4 $382
LABELV $384
line 896
;896:    return NULL;
CNSTP4 0
RETP4
ADDRGP4 $381
JUMPV
LABELV $382
line 899
;897:  }
;898:
;899:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $388
JUMPV
LABELV $385
line 900
;900:    if (Q_stricmp(p, menu->items[i]->window.name) == 0) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $389
line 901
;901:      return menu->items[i];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $381
JUMPV
LABELV $389
line 903
;902:    }
;903:  }
LABELV $386
line 899
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $388
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $385
line 905
;904:
;905:  return NULL;
CNSTP4 0
RETP4
LABELV $381
endproc Menu_FindItemByName 12 8
export Script_SetTeamColor
proc Script_SetTeamColor 24 4
line 908
;906:}
;907:
;908:void Script_SetTeamColor(itemDef_t *item, char **args) {
line 909
;909:  if (DC->getTeamColor) {
ADDRGP4 DC
INDIRP4
CNSTI4 84
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $392
line 912
;910:    int i;
;911:    vec4_t color;
;912:    DC->getTeamColor(&color);
ADDRLP4 4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 84
ADDP4
INDIRP4
CALLV
pop
line 913
;913:    for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $394
line 914
;914:      item->window.backColor[i] = color[i];
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDP4
ADDRLP4 20
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
ASGNF4
line 915
;915:    }
LABELV $395
line 913
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $394
line 916
;916:  }
LABELV $392
line 917
;917:}
LABELV $391
endproc Script_SetTeamColor 24 4
export Script_SetItemColor
proc Script_SetItemColor 80 12
line 919
;918:
;919:void Script_SetItemColor(itemDef_t *item, char **args) {
line 926
;920:  const char *itemname;
;921:  const char *name;
;922:  vec4_t color;
;923:  int i;
;924:  vec4_t *out;
;925:  // expecting type of color to set and 4 args for the color
;926:  if (String_Parse(args, &itemname) && String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 32
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $399
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 36
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $399
line 929
;927:    itemDef_t *item2;
;928:    int j;
;929:    int count = Menu_ItemsMatchingGroup(item->parent, itemname);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 Menu_ItemsMatchingGroup
CALLI4
ASGNI4
ADDRLP4 48
ADDRLP4 52
INDIRI4
ASGNI4
line 931
;930:
;931:    if (!Color_Parse(args, &color)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 56
ADDRGP4 Color_Parse
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $401
line 932
;932:      return;
ADDRGP4 $398
JUMPV
LABELV $401
line 935
;933:    }
;934:
;935:    for (j = 0; j < count; j++) {
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRGP4 $406
JUMPV
LABELV $403
line 936
;936:      item2 = Menu_GetMatchingItemByNumber(item->parent, j, itemname);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 40
INDIRI4
ARGI4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 Menu_GetMatchingItemByNumber
CALLP4
ASGNP4
ADDRLP4 44
ADDRLP4 60
INDIRP4
ASGNP4
line 937
;937:      if (item2 != NULL) {
ADDRLP4 44
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $407
line 938
;938:        out = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 939
;939:        if (Q_stricmp(name, "backcolor") == 0) {
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 $358
ARGP4
ADDRLP4 64
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
NEI4 $409
line 940
;940:          out = &item2->window.backColor;
ADDRLP4 4
ADDRLP4 44
INDIRP4
CNSTI4 128
ADDP4
ASGNP4
line 941
;941:        } else if (Q_stricmp(name, "forecolor") == 0) {
ADDRGP4 $410
JUMPV
LABELV $409
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 $361
ARGP4
ADDRLP4 68
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
NEI4 $411
line 942
;942:          out = &item2->window.foreColor;
ADDRLP4 4
ADDRLP4 44
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
line 943
;943:          item2->window.flags |= WINDOW_FORECOLORSET;
ADDRLP4 72
ADDRLP4 44
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 944
;944:        } else if (Q_stricmp(name, "bordercolor") == 0) {
ADDRGP4 $412
JUMPV
LABELV $411
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 $364
ARGP4
ADDRLP4 72
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $413
line 945
;945:          out = &item2->window.borderColor;
ADDRLP4 4
ADDRLP4 44
INDIRP4
CNSTI4 144
ADDP4
ASGNP4
line 946
;946:        }
LABELV $413
LABELV $412
LABELV $410
line 948
;947:
;948:        if (out) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $415
line 949
;949:          for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $417
line 950
;950:            (*out)[i] = color[i];
ADDRLP4 76
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 76
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ADDRLP4 76
INDIRI4
ADDRLP4 8
ADDP4
INDIRF4
ASGNF4
line 951
;951:          }
LABELV $418
line 949
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $417
line 952
;952:        }
LABELV $415
line 953
;953:      }
LABELV $407
line 954
;954:    }
LABELV $404
line 935
ADDRLP4 40
ADDRLP4 40
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $406
ADDRLP4 40
INDIRI4
ADDRLP4 48
INDIRI4
LTI4 $403
line 955
;955:  }
LABELV $399
line 956
;956:}
LABELV $398
endproc Script_SetItemColor 80 12
export Menu_ShowItemByName
proc Menu_ShowItemByName 24 12
line 959
;957:
;958:
;959:void Menu_ShowItemByName(menuDef_t *menu, const char *p, qboolean bShow) {
line 962
;960:	itemDef_t *item;
;961:	int i;
;962:	int count = Menu_ItemsMatchingGroup(menu, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Menu_ItemsMatchingGroup
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 963
;963:	for (i = 0; i < count; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $425
JUMPV
LABELV $422
line 964
;964:		item = Menu_GetMatchingItemByNumber(menu, i, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Menu_GetMatchingItemByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ASGNP4
line 965
;965:		if (item != NULL) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $426
line 966
;966:			if (bShow) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $428
line 967
;967:				item->window.flags |= WINDOW_VISIBLE;
ADDRLP4 20
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 968
;968:			} else {
ADDRGP4 $429
JUMPV
LABELV $428
line 969
;969:				item->window.flags &= ~WINDOW_VISIBLE;
ADDRLP4 20
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 -5
BANDI4
ASGNI4
line 971
;970:				// stop cinematics playing in the window
;971:				if (item->window.cinematic >= 0) {
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LTI4 $430
line 972
;972:					DC->stopCinematic(item->window.cinematic);
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 184
ADDP4
INDIRP4
CALLV
pop
line 973
;973:					item->window.cinematic = -1;
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 -1
ASGNI4
line 974
;974:				}
LABELV $430
line 975
;975:			}
LABELV $429
line 976
;976:		}
LABELV $426
line 977
;977:	}
LABELV $423
line 963
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $425
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $422
line 978
;978:}
LABELV $421
endproc Menu_ShowItemByName 24 12
export Menu_FadeItemByName
proc Menu_FadeItemByName 32 12
line 980
;979:
;980:void Menu_FadeItemByName(menuDef_t *menu, const char *p, qboolean fadeOut) {
line 983
;981:  itemDef_t *item;
;982:  int i;
;983:  int count = Menu_ItemsMatchingGroup(menu, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Menu_ItemsMatchingGroup
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 984
;984:  for (i = 0; i < count; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $436
JUMPV
LABELV $433
line 985
;985:    item = Menu_GetMatchingItemByNumber(menu, i, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Menu_GetMatchingItemByNumber
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ASGNP4
line 986
;986:    if (item != NULL) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $437
line 987
;987:      if (fadeOut) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $439
line 988
;988:        item->window.flags |= (WINDOW_FADINGOUT | WINDOW_VISIBLE);
ADDRLP4 20
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 36
BORI4
ASGNI4
line 989
;989:        item->window.flags &= ~WINDOW_FADINGIN;
ADDRLP4 24
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 -65
BANDI4
ASGNI4
line 990
;990:      } else {
ADDRGP4 $440
JUMPV
LABELV $439
line 991
;991:        item->window.flags |= (WINDOW_VISIBLE | WINDOW_FADINGIN);
ADDRLP4 20
CNSTI4 68
ASGNI4
ADDRLP4 24
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 20
INDIRI4
BORI4
ASGNI4
line 992
;992:        item->window.flags &= ~WINDOW_FADINGOUT;
ADDRLP4 28
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 -33
BANDI4
ASGNI4
line 993
;993:      }
LABELV $440
line 994
;994:    }
LABELV $437
line 995
;995:  }
LABELV $434
line 984
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $436
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $433
line 996
;996:}
LABELV $432
endproc Menu_FadeItemByName 32 12
export Menus_FindByName
proc Menus_FindByName 8 8
line 998
;997:
;998:menuDef_t *Menus_FindByName(const char *p) {
line 1000
;999:  int i;
;1000:  for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $445
JUMPV
LABELV $442
line 1001
;1001:    if (Q_stricmp(Menus[i].window.name, p) == 0) {
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+32
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $446
line 1002
;1002:      return &Menus[i];
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
RETP4
ADDRGP4 $441
JUMPV
LABELV $446
line 1004
;1003:    } 
;1004:  }
LABELV $443
line 1000
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $445
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $442
line 1005
;1005:  return NULL;
CNSTP4 0
RETP4
LABELV $441
endproc Menus_FindByName 8 8
export Menus_ShowByName
proc Menus_ShowByName 8 4
line 1008
;1006:}
;1007:
;1008:void Menus_ShowByName(const char *p) {
line 1009
;1009:	menuDef_t *menu = Menus_FindByName(p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Menus_FindByName
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1010
;1010:	if (menu) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $450
line 1011
;1011:		Menus_Activate(menu);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menus_Activate
CALLV
pop
line 1012
;1012:	}
LABELV $450
line 1013
;1013:}
LABELV $449
endproc Menus_ShowByName 8 4
export Menus_OpenByName
proc Menus_OpenByName 0 4
line 1015
;1014:
;1015:void Menus_OpenByName(const char *p) {
line 1016
;1016:  Menus_ActivateByName(p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menus_ActivateByName
CALLP4
pop
line 1017
;1017:}
LABELV $452
endproc Menus_OpenByName 0 4
proc Menu_RunCloseScript 548 8
line 1019
;1018:
;1019:static void Menu_RunCloseScript(menuDef_t *menu) {
line 1020
;1020:	if (menu && menu->window.flags & WINDOW_VISIBLE && menu->onClose) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $454
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $454
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $454
line 1022
;1021:		itemDef_t item;
;1022:    item.parent = menu;
ADDRLP4 8+228
ADDRFP4 0
INDIRP4
ASGNP4
line 1023
;1023:    Item_RunScript(&item, menu->onClose);
ADDRLP4 8
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1024
;1024:	}
LABELV $454
line 1025
;1025:}
LABELV $453
endproc Menu_RunCloseScript 548 8
export Menus_CloseByName
proc Menus_CloseByName 12 4
line 1027
;1026:
;1027:void Menus_CloseByName(const char *p) {
line 1028
;1028:  menuDef_t *menu = Menus_FindByName(p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Menus_FindByName
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1029
;1029:  if (menu != NULL) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $458
line 1030
;1030:		Menu_RunCloseScript(menu);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_RunCloseScript
CALLV
pop
line 1031
;1031:		menu->window.flags &= ~(WINDOW_VISIBLE | WINDOW_HASFOCUS);
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -7
BANDI4
ASGNI4
line 1032
;1032:  }
LABELV $458
line 1033
;1033:}
LABELV $457
endproc Menus_CloseByName 12 4
export Menus_CloseAll
proc Menus_CloseAll 8 4
line 1035
;1034:
;1035:void Menus_CloseAll() {
line 1037
;1036:  int i;
;1037:  for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $464
JUMPV
LABELV $461
line 1038
;1038:		Menu_RunCloseScript(&Menus[i]);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRGP4 Menu_RunCloseScript
CALLV
pop
line 1039
;1039:		Menus[i].window.flags &= ~(WINDOW_HASFOCUS | WINDOW_VISIBLE);
ADDRLP4 4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+68
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -7
BANDI4
ASGNI4
line 1040
;1040:  }
LABELV $462
line 1037
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $464
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $461
line 1041
;1041:}
LABELV $460
endproc Menus_CloseAll 8 4
export Script_Show
proc Script_Show 8 12
line 1044
;1042:
;1043:
;1044:void Script_Show(itemDef_t *item, char **args) {
line 1046
;1045:  const char *name;
;1046:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $467
line 1047
;1047:    Menu_ShowItemByName(item->parent, name, qtrue);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 1048
;1048:  }
LABELV $467
line 1049
;1049:}
LABELV $466
endproc Script_Show 8 12
export Script_Hide
proc Script_Hide 8 12
line 1051
;1050:
;1051:void Script_Hide(itemDef_t *item, char **args) {
line 1053
;1052:  const char *name;
;1053:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $470
line 1054
;1054:    Menu_ShowItemByName(item->parent, name, qfalse);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 1055
;1055:  }
LABELV $470
line 1056
;1056:}
LABELV $469
endproc Script_Hide 8 12
export Script_FadeIn
proc Script_FadeIn 8 12
line 1058
;1057:
;1058:void Script_FadeIn(itemDef_t *item, char **args) {
line 1060
;1059:  const char *name;
;1060:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $473
line 1061
;1061:    Menu_FadeItemByName(item->parent, name, qfalse);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_FadeItemByName
CALLV
pop
line 1062
;1062:  }
LABELV $473
line 1063
;1063:}
LABELV $472
endproc Script_FadeIn 8 12
export Script_FadeOut
proc Script_FadeOut 8 12
line 1065
;1064:
;1065:void Script_FadeOut(itemDef_t *item, char **args) {
line 1067
;1066:  const char *name;
;1067:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $476
line 1068
;1068:    Menu_FadeItemByName(item->parent, name, qtrue);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_FadeItemByName
CALLV
pop
line 1069
;1069:  }
LABELV $476
line 1070
;1070:}
LABELV $475
endproc Script_FadeOut 8 12
export Script_Open
proc Script_Open 8 8
line 1074
;1071:
;1072:
;1073:
;1074:void Script_Open(itemDef_t *item, char **args) {
line 1076
;1075:  const char *name;
;1076:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $479
line 1077
;1077:    Menus_OpenByName(name);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menus_OpenByName
CALLV
pop
line 1078
;1078:  }
LABELV $479
line 1079
;1079:}
LABELV $478
endproc Script_Open 8 8
export Script_ConditionalOpen
proc Script_ConditionalOpen 32 8
line 1081
;1080:
;1081:void Script_ConditionalOpen(itemDef_t *item, char **args) {
line 1087
;1082:	const char *cvar;
;1083:	const char *name1;
;1084:	const char *name2;
;1085:	float           val;
;1086:
;1087:	if ( String_Parse(args, &cvar) && String_Parse(args, &name1) && String_Parse(args, &name2) ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $482
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 20
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $482
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 24
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $482
line 1088
;1088:		val = DC->getCVarValue( cvar );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 28
INDIRF4
ASGNF4
line 1089
;1089:		if ( val == 0.f ) {
ADDRLP4 12
INDIRF4
CNSTF4 0
NEF4 $484
line 1090
;1090:			Menus_OpenByName(name2);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 Menus_OpenByName
CALLV
pop
line 1091
;1091:		} else {
ADDRGP4 $485
JUMPV
LABELV $484
line 1092
;1092:			Menus_OpenByName(name1);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Menus_OpenByName
CALLV
pop
line 1093
;1093:		}
LABELV $485
line 1094
;1094:	}
LABELV $482
line 1095
;1095:}
LABELV $481
endproc Script_ConditionalOpen 32 8
export Script_Close
proc Script_Close 8 8
line 1097
;1096:
;1097:void Script_Close(itemDef_t *item, char **args) {
line 1099
;1098:  const char *name;
;1099:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $487
line 1100
;1100:    Menus_CloseByName(name);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menus_CloseByName
CALLV
pop
line 1101
;1101:  }
LABELV $487
line 1102
;1102:}
LABELV $486
endproc Script_Close 8 8
export Menu_TransitionItemByName
proc Menu_TransitionItemByName 56 12
line 1104
;1103:
;1104:void Menu_TransitionItemByName(menuDef_t *menu, const char *p, rectDef_t rectFrom, rectDef_t rectTo, int time, float amt) {
line 1107
;1105:  itemDef_t *item;
;1106:  int i;
;1107:  int count = Menu_ItemsMatchingGroup(menu, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Menu_ItemsMatchingGroup
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 1108
;1108:  for (i = 0; i < count; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $493
JUMPV
LABELV $490
line 1109
;1109:    item = Menu_GetMatchingItemByNumber(menu, i, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Menu_GetMatchingItemByNumber
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 1110
;1110:    if (item != NULL) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $494
line 1111
;1111:      item->window.flags |= (WINDOW_INTRANSITION | WINDOW_VISIBLE);
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 260
BORI4
ASGNI4
line 1112
;1112:      item->window.offsetTime = time;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 1113
;1113:			memcpy(&item->window.rectClient, &rectFrom, sizeof(rectDef_t));
ADDRLP4 24
CNSTI4 16
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1114
;1114:			memcpy(&item->window.rectEffects, &rectTo, sizeof(rectDef_t));
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 1115
;1115:			item->window.rectEffects2.x = abs(rectTo.x - rectFrom.x) / amt;
ADDRFP4 12
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
INDIRF4
SUBF4
CVFI4 4
ARGI4
ADDRLP4 28
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 88
ADDP4
ADDRLP4 28
INDIRI4
CVIF4 4
ADDRFP4 20
INDIRF4
DIVF4
ASGNF4
line 1116
;1116:			item->window.rectEffects2.y = abs(rectTo.y - rectFrom.y) / amt;
ADDRLP4 32
CNSTI4 4
ASGNI4
ADDRFP4 12
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
CVFI4 4
ARGI4
ADDRLP4 36
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 36
INDIRI4
CVIF4 4
ADDRFP4 20
INDIRF4
DIVF4
ASGNF4
line 1117
;1117:			item->window.rectEffects2.w = abs(rectTo.w - rectFrom.w) / amt;
ADDRLP4 40
CNSTI4 8
ASGNI4
ADDRFP4 12
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
SUBF4
CVFI4 4
ARGI4
ADDRLP4 44
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
ADDRLP4 44
INDIRI4
CVIF4 4
ADDRFP4 20
INDIRF4
DIVF4
ASGNF4
line 1118
;1118:			item->window.rectEffects2.h = abs(rectTo.h - rectFrom.h) / amt;
ADDRLP4 48
CNSTI4 12
ASGNI4
ADDRFP4 12
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
CVFI4 4
ARGI4
ADDRLP4 52
ADDRGP4 abs
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
ADDRLP4 52
INDIRI4
CVIF4 4
ADDRFP4 20
INDIRF4
DIVF4
ASGNF4
line 1119
;1119:      Item_UpdatePosition(item);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Item_UpdatePosition
CALLV
pop
line 1120
;1120:    }
LABELV $494
line 1121
;1121:  }
LABELV $491
line 1108
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $493
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $490
line 1122
;1122:}
LABELV $489
endproc Menu_TransitionItemByName 56 12
export Script_Transition
proc Script_Transition 96 24
line 1125
;1123:
;1124:
;1125:void Script_Transition(itemDef_t *item, char **args) {
line 1131
;1126:  const char *name;
;1127:	rectDef_t rectFrom, rectTo;
;1128:  int time;
;1129:	float amt;
;1130:
;1131:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 44
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $497
line 1132
;1132:    if ( Rect_Parse(args, &rectFrom) && Rect_Parse(args, &rectTo) && Int_Parse(args, &time) && Float_Parse(args, &amt)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 48
ADDRGP4 Rect_Parse
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $499
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 52
ADDRGP4 Rect_Parse
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
EQI4 $499
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 36
ARGP4
ADDRLP4 56
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $499
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 40
ARGP4
ADDRLP4 60
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $499
line 1133
;1133:      Menu_TransitionItemByName(item->parent, name, rectFrom, rectTo, time, amt);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRLP4 4
INDIRB
ASGNB 16
ADDRLP4 64
ARGP4
ADDRLP4 80
ADDRLP4 20
INDIRB
ASGNB 16
ADDRLP4 80
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 40
INDIRF4
ARGF4
ADDRGP4 Menu_TransitionItemByName
CALLV
pop
line 1134
;1134:    }
LABELV $499
line 1135
;1135:  }
LABELV $497
line 1136
;1136:}
LABELV $496
endproc Script_Transition 96 24
export Menu_OrbitItemByName
proc Menu_OrbitItemByName 24 12
line 1139
;1137:
;1138:
;1139:void Menu_OrbitItemByName(menuDef_t *menu, const char *p, float x, float y, float cx, float cy, int time) {
line 1142
;1140:  itemDef_t *item;
;1141:  int i;
;1142:  int count = Menu_ItemsMatchingGroup(menu, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Menu_ItemsMatchingGroup
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 1143
;1143:  for (i = 0; i < count; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $505
JUMPV
LABELV $502
line 1144
;1144:    item = Menu_GetMatchingItemByNumber(menu, i, p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Menu_GetMatchingItemByNumber
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 1145
;1145:    if (item != NULL) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $506
line 1146
;1146:      item->window.flags |= (WINDOW_ORBITING | WINDOW_VISIBLE);
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 65540
BORI4
ASGNI4
line 1147
;1147:      item->window.offsetTime = time;
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRFP4 24
INDIRI4
ASGNI4
line 1148
;1148:      item->window.rectEffects.x = cx;
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRFP4 16
INDIRF4
ASGNF4
line 1149
;1149:      item->window.rectEffects.y = cy;
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRFP4 20
INDIRF4
ASGNF4
line 1150
;1150:      item->window.rectClient.x = x;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 1151
;1151:      item->window.rectClient.y = y;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 12
INDIRF4
ASGNF4
line 1152
;1152:      Item_UpdatePosition(item);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Item_UpdatePosition
CALLV
pop
line 1153
;1153:    }
LABELV $506
line 1154
;1154:  }
LABELV $503
line 1143
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $505
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $502
line 1155
;1155:}
LABELV $501
endproc Menu_OrbitItemByName 24 12
export Script_Orbit
proc Script_Orbit 48 28
line 1158
;1156:
;1157:
;1158:void Script_Orbit(itemDef_t *item, char **args) {
line 1163
;1159:  const char *name;
;1160:  float cx, cy, x, y;
;1161:  int time;
;1162:
;1163:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 24
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $509
line 1164
;1164:    if ( Float_Parse(args, &x) && Float_Parse(args, &y) && Float_Parse(args, &cx) && Float_Parse(args, &cy) && Int_Parse(args, &time) ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $511
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 32
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $511
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 36
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $511
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 40
ADDRGP4 Float_Parse
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $511
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 44
ADDRGP4 Int_Parse
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $511
line 1165
;1165:      Menu_OrbitItemByName(item->parent, name, x, y, cx, cy, time);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 20
INDIRI4
ARGI4
ADDRGP4 Menu_OrbitItemByName
CALLV
pop
line 1166
;1166:    }
LABELV $511
line 1167
;1167:  }
LABELV $509
line 1168
;1168:}
LABELV $508
endproc Script_Orbit 48 28
export Script_SetFocus
proc Script_SetFocus 36 8
line 1172
;1169:
;1170:
;1171:
;1172:void Script_SetFocus(itemDef_t *item, char **args) {
line 1176
;1173:  const char *name;
;1174:  itemDef_t *focusItem;
;1175:
;1176:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $514
line 1177
;1177:    focusItem = Menu_FindItemByName(item->parent, name);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Menu_FindItemByName
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 1178
;1178:    if (focusItem && !(focusItem->window.flags & WINDOW_DECORATION) && !(focusItem->window.flags & WINDOW_HASFOCUS)) {
ADDRLP4 16
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $516
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 16
BANDI4
ADDRLP4 24
INDIRI4
NEI4 $516
ADDRLP4 20
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 24
INDIRI4
NEI4 $516
line 1179
;1179:      Menu_ClearFocus(item->parent);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRGP4 Menu_ClearFocus
CALLP4
pop
line 1180
;1180:      focusItem->window.flags |= WINDOW_HASFOCUS;
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1181
;1181:      if (focusItem->onFocus) {
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $518
line 1182
;1182:        Item_RunScript(focusItem, focusItem->onFocus);
ADDRLP4 32
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1183
;1183:      }
LABELV $518
line 1184
;1184:      if (DC->Assets.itemFocusSound) {
ADDRGP4 DC
INDIRP4
CNSTI4 61948
ADDP4
INDIRI4
CNSTI4 0
EQI4 $520
line 1185
;1185:        DC->startLocalSound( DC->Assets.itemFocusSound, CHAN_LOCAL_SOUND );
ADDRLP4 32
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 61948
ADDP4
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRLP4 32
INDIRP4
CNSTI4 112
ADDP4
INDIRP4
CALLV
pop
line 1186
;1186:      }
LABELV $520
line 1187
;1187:    }
LABELV $516
line 1188
;1188:  }
LABELV $514
line 1189
;1189:}
LABELV $513
endproc Script_SetFocus 36 8
export Script_SetPlayerModel
proc Script_SetPlayerModel 8 8
line 1191
;1190:
;1191:void Script_SetPlayerModel(itemDef_t *item, char **args) {
line 1193
;1192:  const char *name;
;1193:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $523
line 1194
;1194:    DC->setCVar("team_model", name);
ADDRGP4 $525
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 1195
;1195:  }
LABELV $523
line 1196
;1196:}
LABELV $522
endproc Script_SetPlayerModel 8 8
export Script_SetPlayerHead
proc Script_SetPlayerHead 8 8
line 1198
;1197:
;1198:void Script_SetPlayerHead(itemDef_t *item, char **args) {
line 1200
;1199:  const char *name;
;1200:  if (String_Parse(args, &name)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $527
line 1201
;1201:    DC->setCVar("team_headmodel", name);
ADDRGP4 $529
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 1202
;1202:  }
LABELV $527
line 1203
;1203:}
LABELV $526
endproc Script_SetPlayerHead 8 8
export Script_SetCvar
proc Script_SetCvar 16 8
line 1205
;1204:
;1205:void Script_SetCvar(itemDef_t *item, char **args) {
line 1207
;1206:	const char *cvar, *val;
;1207:	if (String_Parse(args, &cvar) && String_Parse(args, &val)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $531
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 12
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $531
line 1208
;1208:		DC->setCVar(cvar, val);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 1209
;1209:	}
LABELV $531
line 1211
;1210:	
;1211:}
LABELV $530
endproc Script_SetCvar 16 8
export Script_Exec
proc Script_Exec 12 8
line 1213
;1212:
;1213:void Script_Exec(itemDef_t *item, char **args) {
line 1215
;1214:	const char *val;
;1215:	if (String_Parse(args, &val)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $534
line 1216
;1216:		DC->executeText(EXEC_APPEND, va("%s ; ", val));
ADDRGP4 $536
ARGP4
ADDRLP4 0
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
ADDRGP4 DC
INDIRP4
CNSTI4 148
ADDP4
INDIRP4
CALLV
pop
line 1217
;1217:	}
LABELV $534
line 1218
;1218:}
LABELV $533
endproc Script_Exec 12 8
export Script_Play
proc Script_Play 12 8
line 1220
;1219:
;1220:void Script_Play(itemDef_t *item, char **args) {
line 1222
;1221:	const char *val;
;1222:	if (String_Parse(args, &val)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $538
line 1223
;1223:		DC->startLocalSound(DC->registerSound(val, qfalse), CHAN_LOCAL_SOUND);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
CNSTI4 168
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 112
ADDP4
INDIRP4
CALLV
pop
line 1224
;1224:	}
LABELV $538
line 1225
;1225:}
LABELV $537
endproc Script_Play 12 8
export Script_playLooped
proc Script_playLooped 12 8
line 1227
;1226:
;1227:void Script_playLooped(itemDef_t *item, char **args) {
line 1229
;1228:	const char *val;
;1229:	if (String_Parse(args, &val)) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $541
line 1230
;1230:		DC->stopBackgroundTrack();
ADDRGP4 DC
INDIRP4
CNSTI4 176
ADDP4
INDIRP4
CALLV
pop
line 1231
;1231:		DC->startBackgroundTrack(val, val);
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 172
ADDP4
INDIRP4
CALLV
pop
line 1232
;1232:	}
LABELV $541
line 1233
;1233:}
LABELV $540
endproc Script_playLooped 12 8
data
export commandList
align 4
LABELV commandList
address $543
address Script_FadeIn
address $544
address Script_FadeOut
address $545
address Script_Show
address $546
address Script_Hide
address $547
address Script_SetColor
address $548
address Script_Open
address $549
address Script_ConditionalOpen
address $550
address Script_Close
address $551
address Script_SetAsset
address $552
address Script_SetBackground
address $553
address Script_SetItemColor
address $554
address Script_SetTeamColor
address $555
address Script_SetFocus
address $556
address Script_SetPlayerModel
address $557
address Script_SetPlayerHead
address $558
address Script_Transition
address $559
address Script_SetCvar
address $560
address Script_Exec
address $561
address Script_Play
address $562
address Script_playLooped
address $563
address Script_Orbit
export scriptCommandCount
align 4
LABELV scriptCommandCount
byte 4 21
export Item_RunScript
code
proc Item_RunScript 1060 12
line 1264
;1234:
;1235:
;1236:commandDef_t commandList[] =
;1237:{
;1238:  {"fadein", &Script_FadeIn},                   // group/name
;1239:  {"fadeout", &Script_FadeOut},                 // group/name
;1240:  {"show", &Script_Show},                       // group/name
;1241:  {"hide", &Script_Hide},                       // group/name
;1242:  {"setcolor", &Script_SetColor},               // works on this
;1243:  {"open", &Script_Open},                       // menu
;1244:	{"conditionalopen", &Script_ConditionalOpen},	// menu
;1245:  {"close", &Script_Close},                     // menu
;1246:  {"setasset", &Script_SetAsset},               // works on this
;1247:  {"setbackground", &Script_SetBackground},     // works on this
;1248:  {"setitemcolor", &Script_SetItemColor},       // group/name
;1249:  {"setteamcolor", &Script_SetTeamColor},       // sets this background color to team color
;1250:  {"setfocus", &Script_SetFocus},               // sets this background color to team color
;1251:  {"setplayermodel", &Script_SetPlayerModel},   // sets this background color to team color
;1252:  {"setplayerhead", &Script_SetPlayerHead},     // sets this background color to team color
;1253:  {"transition", &Script_Transition},           // group/name
;1254:  {"setcvar", &Script_SetCvar},           // group/name
;1255:  {"exec", &Script_Exec},           // group/name
;1256:  {"play", &Script_Play},           // group/name
;1257:  {"playlooped", &Script_playLooped},           // group/name
;1258:  {"orbit", &Script_Orbit}                      // group/name
;1259:};
;1260:
;1261:int scriptCommandCount = sizeof(commandList) / sizeof(commandDef_t);
;1262:
;1263:
;1264:void Item_RunScript(itemDef_t *item, const char *s) {
line 1268
;1265:  char script[1024], *p;
;1266:  int i;
;1267:  qboolean bRan;
;1268:  memset(script, 0, sizeof(script));
ADDRLP4 12
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1269
;1269:  if (item && s && s[0]) {
ADDRLP4 1036
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 1036
INDIRU4
EQU4 $565
ADDRLP4 1040
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 1040
INDIRP4
CVPU4 4
ADDRLP4 1036
INDIRU4
EQU4 $565
ADDRLP4 1040
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $565
line 1270
;1270:    Q_strcat(script, 1024, s);
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 1271
;1271:    p = script;
ADDRLP4 8
ADDRLP4 12
ASGNP4
ADDRGP4 $568
JUMPV
LABELV $567
line 1272
;1272:    while (1) {
line 1275
;1273:      const char *command;
;1274:      // expect command then arguments, ; ends command, NULL ends script
;1275:      if (!String_Parse(&p, &command)) {
ADDRLP4 8
ARGP4
ADDRLP4 1044
ARGP4
ADDRLP4 1048
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $570
line 1276
;1276:        return;
ADDRGP4 $564
JUMPV
LABELV $570
line 1279
;1277:      }
;1278:
;1279:      if (command[0] == ';' && command[1] == '\0') {
ADDRLP4 1052
ADDRLP4 1044
INDIRP4
ASGNP4
ADDRLP4 1052
INDIRP4
INDIRI1
CVII4 1
CNSTI4 59
NEI4 $572
ADDRLP4 1052
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $572
line 1280
;1280:        continue;
ADDRGP4 $568
JUMPV
LABELV $572
line 1283
;1281:      }
;1282:
;1283:      bRan = qfalse;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1284
;1284:      for (i = 0; i < scriptCommandCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $577
JUMPV
LABELV $574
line 1285
;1285:        if (Q_stricmp(command, commandList[i].name) == 0) {
ADDRLP4 1044
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 commandList
ADDP4
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $578
line 1286
;1286:          (commandList[i].handler(item, &p));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 commandList+4
ADDP4
INDIRP4
CALLV
pop
line 1287
;1287:          bRan = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1288
;1288:          break;
ADDRGP4 $576
JUMPV
LABELV $578
line 1290
;1289:        }
;1290:      }
LABELV $575
line 1284
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $577
ADDRLP4 0
INDIRI4
ADDRGP4 scriptCommandCount
INDIRI4
LTI4 $574
LABELV $576
line 1292
;1291:      // not in our auto list, pass to handler
;1292:      if (!bRan) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $581
line 1293
;1293:        DC->runScript(&p);
ADDRLP4 8
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 80
ADDP4
INDIRP4
CALLV
pop
line 1294
;1294:      }
LABELV $581
line 1295
;1295:    }
LABELV $568
line 1272
ADDRGP4 $567
JUMPV
line 1296
;1296:  }
LABELV $565
line 1297
;1297:}
LABELV $564
endproc Item_RunScript 1060 12
export Item_EnableShowViaCvar
proc Item_EnableShowViaCvar 2088 12
line 1300
;1298:
;1299:
;1300:qboolean Item_EnableShowViaCvar(itemDef_t *item, int flag) {
line 1302
;1301:  char script[1024], *p;
;1302:  memset(script, 0, sizeof(script));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1303
;1303:  if (item && item->enableCvar && *item->enableCvar && item->cvarTest && *item->cvarTest) {
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1032
CNSTU4 0
ASGNU4
ADDRLP4 1028
INDIRP4
CVPU4 4
ADDRLP4 1032
INDIRU4
EQU4 $584
ADDRLP4 1036
ADDRLP4 1028
INDIRP4
CNSTI4 272
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1036
INDIRP4
CVPU4 4
ADDRLP4 1032
INDIRU4
EQU4 $584
ADDRLP4 1040
CNSTI4 0
ASGNI4
ADDRLP4 1036
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 1040
INDIRI4
EQI4 $584
ADDRLP4 1044
ADDRLP4 1028
INDIRP4
CNSTI4 268
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1044
INDIRP4
CVPU4 4
ADDRLP4 1032
INDIRU4
EQU4 $584
ADDRLP4 1044
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 1040
INDIRI4
EQI4 $584
line 1305
;1304:		char buff[1024];
;1305:	  DC->getCVarString(item->cvarTest, buff, sizeof(buff));
ADDRFP4 0
INDIRP4
CNSTI4 268
ADDP4
INDIRP4
ARGP4
ADDRLP4 1048
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 1307
;1306:
;1307:    Q_strcat(script, 1024, item->enableCvar);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 1308
;1308:    p = script;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $587
JUMPV
LABELV $586
line 1309
;1309:    while (1) {
line 1312
;1310:      const char *val;
;1311:      // expect value then ; or NULL, NULL ends list
;1312:      if (!String_Parse(&p, &val)) {
ADDRLP4 0
ARGP4
ADDRLP4 2072
ARGP4
ADDRLP4 2076
ADDRGP4 String_Parse
CALLI4
ASGNI4
ADDRLP4 2076
INDIRI4
CNSTI4 0
NEI4 $589
line 1313
;1313:				return (item->cvarFlags & flag) ? qfalse : qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $592
ADDRLP4 2080
CNSTI4 0
ASGNI4
ADDRGP4 $593
JUMPV
LABELV $592
ADDRLP4 2080
CNSTI4 1
ASGNI4
LABELV $593
ADDRLP4 2080
INDIRI4
RETI4
ADDRGP4 $583
JUMPV
LABELV $589
line 1316
;1314:      }
;1315:
;1316:      if (val[0] == ';' && val[1] == '\0') {
ADDRLP4 2080
ADDRLP4 2072
INDIRP4
ASGNP4
ADDRLP4 2080
INDIRP4
INDIRI1
CVII4 1
CNSTI4 59
NEI4 $594
ADDRLP4 2080
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $594
line 1317
;1317:        continue;
ADDRGP4 $587
JUMPV
LABELV $594
line 1321
;1318:      }
;1319:
;1320:			// enable it if any of the values are true
;1321:			if (item->cvarFlags & flag) {
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $596
line 1322
;1322:        if (Q_stricmp(buff, val) == 0) {
ADDRLP4 1048
ARGP4
ADDRLP4 2072
INDIRP4
ARGP4
ADDRLP4 2084
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 2084
INDIRI4
CNSTI4 0
NEI4 $597
line 1323
;1323:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $583
JUMPV
line 1325
;1324:				}
;1325:			} else {
LABELV $596
line 1327
;1326:				// disable it if any of the values are true
;1327:        if (Q_stricmp(buff, val) == 0) {
ADDRLP4 1048
ARGP4
ADDRLP4 2072
INDIRP4
ARGP4
ADDRLP4 2084
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 2084
INDIRI4
CNSTI4 0
NEI4 $600
line 1328
;1328:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $583
JUMPV
LABELV $600
line 1330
;1329:				}
;1330:			}
LABELV $597
line 1332
;1331:
;1332:    }
LABELV $587
line 1309
ADDRGP4 $586
JUMPV
line 1333
;1333:		return (item->cvarFlags & flag) ? qfalse : qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $603
ADDRLP4 2072
CNSTI4 0
ASGNI4
ADDRGP4 $604
JUMPV
LABELV $603
ADDRLP4 2072
CNSTI4 1
ASGNI4
LABELV $604
ADDRLP4 2072
INDIRI4
RETI4
ADDRGP4 $583
JUMPV
LABELV $584
line 1335
;1334:  }
;1335:	return qtrue;
CNSTI4 1
RETI4
LABELV $583
endproc Item_EnableShowViaCvar 2088 12
export Item_SetFocus
proc Item_SetFocus 80 12
line 1340
;1336:}
;1337:
;1338:
;1339:// will optionaly set focus to this item 
;1340:qboolean Item_SetFocus(itemDef_t *item, float x, float y) {
line 1343
;1341:	int i;
;1342:	itemDef_t *oldFocus;
;1343:	sfxHandle_t *sfx = &DC->Assets.itemFocusSound;
ADDRLP4 8
ADDRGP4 DC
INDIRP4
CNSTI4 61948
ADDP4
ASGNP4
line 1344
;1344:	qboolean playSound = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1347
;1345:	menuDef_t *parent; // bk001206: = (menuDef_t*)item->parent;
;1346:	// sanity check, non-null, not a decoration and does not already have the focus
;1347:	if (item == NULL || item->window.flags & WINDOW_DECORATION || item->window.flags & WINDOW_HASFOCUS || !(item->window.flags & WINDOW_VISIBLE)) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $610
ADDRLP4 24
ADDRLP4 20
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 16
BANDI4
ADDRLP4 28
INDIRI4
NEI4 $610
ADDRLP4 24
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 28
INDIRI4
NEI4 $610
ADDRLP4 24
INDIRI4
CNSTI4 4
BANDI4
ADDRLP4 28
INDIRI4
NEI4 $606
LABELV $610
line 1348
;1348:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $605
JUMPV
LABELV $606
line 1352
;1349:	}
;1350:
;1351:	// bk001206 - this can be NULL.
;1352:	parent = (menuDef_t*)item->parent; 
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 1355
;1353:      
;1354:	// items can be enabled and disabled based on cvars
;1355:	if (item->cvarFlags & (CVAR_ENABLE | CVAR_DISABLE) && !Item_EnableShowViaCvar(item, CVAR_ENABLE)) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $611
ADDRLP4 32
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 36
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $611
line 1356
;1356:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $605
JUMPV
LABELV $611
line 1359
;1357:	}
;1358:
;1359:	if (item->cvarFlags & (CVAR_SHOW | CVAR_HIDE) && !Item_EnableShowViaCvar(item, CVAR_SHOW)) {
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 12
BANDI4
CNSTI4 0
EQI4 $613
ADDRLP4 40
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 44
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $613
line 1360
;1360:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $605
JUMPV
LABELV $613
line 1363
;1361:	}
;1362:
;1363:	oldFocus = Menu_ClearFocus(item->parent);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 Menu_ClearFocus
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 48
INDIRP4
ASGNP4
line 1365
;1364:
;1365:	if (item->type == ITEM_TYPE_TEXT) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 0
NEI4 $615
line 1367
;1366:		rectDef_t r;
;1367:		r = item->textRect;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRB
ASGNB 16
line 1368
;1368:		r.y -= r.h;
ADDRLP4 52+4
ADDRLP4 52+4
INDIRF4
ADDRLP4 52+12
INDIRF4
SUBF4
ASGNF4
line 1369
;1369:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 52
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $619
line 1370
;1370:			item->window.flags |= WINDOW_HASFOCUS;
ADDRLP4 72
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1371
;1371:			if (item->focusSound) {
ADDRFP4 0
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
CNSTI4 0
EQI4 $621
line 1372
;1372:				sfx = &item->focusSound;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 280
ADDP4
ASGNP4
line 1373
;1373:			}
LABELV $621
line 1374
;1374:			playSound = qtrue;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 1375
;1375:		} else {
ADDRGP4 $616
JUMPV
LABELV $619
line 1376
;1376:			if (oldFocus) {
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $616
line 1377
;1377:				oldFocus->window.flags |= WINDOW_HASFOCUS;
ADDRLP4 72
ADDRLP4 16
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1378
;1378:				if (oldFocus->onFocus) {
ADDRLP4 16
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $616
line 1379
;1379:					Item_RunScript(oldFocus, oldFocus->onFocus);
ADDRLP4 76
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1380
;1380:				}
line 1381
;1381:			}
line 1382
;1382:		}
line 1383
;1383:	} else {
ADDRGP4 $616
JUMPV
LABELV $615
line 1384
;1384:	    item->window.flags |= WINDOW_HASFOCUS;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1385
;1385:		if (item->onFocus) {
ADDRFP4 0
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $627
line 1386
;1386:			Item_RunScript(item, item->onFocus);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
CNSTI4 256
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1387
;1387:		}
LABELV $627
line 1388
;1388:		if (item->focusSound) {
ADDRFP4 0
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
CNSTI4 0
EQI4 $629
line 1389
;1389:			sfx = &item->focusSound;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 280
ADDP4
ASGNP4
line 1390
;1390:		}
LABELV $629
line 1391
;1391:		playSound = qtrue;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 1392
;1392:	}
LABELV $616
line 1394
;1393:
;1394:	if (playSound && sfx) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $631
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $631
line 1395
;1395:		DC->startLocalSound( *sfx, CHAN_LOCAL_SOUND );
ADDRLP4 8
INDIRP4
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 112
ADDP4
INDIRP4
CALLV
pop
line 1396
;1396:	}
LABELV $631
line 1398
;1397:
;1398:	for (i = 0; i < parent->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $636
JUMPV
LABELV $633
line 1399
;1399:		if (parent->items[i] == item) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $637
line 1400
;1400:			parent->cursorItem = i;
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1401
;1401:			break;
ADDRGP4 $635
JUMPV
LABELV $637
line 1403
;1402:		}
;1403:	}
LABELV $634
line 1398
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $636
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $633
LABELV $635
line 1405
;1404:
;1405:	return qtrue;
CNSTI4 1
RETI4
LABELV $605
endproc Item_SetFocus 80 12
export Item_ListBox_MaxScroll
proc Item_ListBox_MaxScroll 16 4
line 1408
;1406:}
;1407:
;1408:int Item_ListBox_MaxScroll(itemDef_t *item) {
line 1409
;1409:	listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1410
;1410:	int count = DC->feederCount(item->special);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 120
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 1413
;1411:	int max;
;1412:
;1413:	if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $640
line 1414
;1414:		max = count - (item->window.rect.w / listPtr->elementWidth) + 1;
ADDRLP4 0
ADDRLP4 8
INDIRI4
CVIF4 4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
DIVF4
SUBF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1415
;1415:	}
ADDRGP4 $641
JUMPV
LABELV $640
line 1416
;1416:	else {
line 1417
;1417:		max = count - (item->window.rect.h / listPtr->elementHeight) + 1;
ADDRLP4 0
ADDRLP4 8
INDIRI4
CVIF4 4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
DIVF4
SUBF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1418
;1418:	}
LABELV $641
line 1419
;1419:	if (max < 0) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $642
line 1420
;1420:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $639
JUMPV
LABELV $642
line 1422
;1421:	}
;1422:	return max;
ADDRLP4 0
INDIRI4
RETI4
LABELV $639
endproc Item_ListBox_MaxScroll 16 4
export Item_ListBox_ThumbPosition
proc Item_ListBox_ThumbPosition 20 4
line 1425
;1423:}
;1424:
;1425:int Item_ListBox_ThumbPosition(itemDef_t *item) {
line 1427
;1426:	float max, pos, size;
;1427:	listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1429
;1428:
;1429:	max = Item_ListBox_MaxScroll(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Item_ListBox_MaxScroll
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
CVIF4 4
ASGNF4
line 1430
;1430:	if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $645
line 1431
;1431:		size = item->window.rect.w - (SCROLLBAR_SIZE * 2) - 2;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 1432
;1432:		if (max > 0) {
ADDRLP4 4
INDIRF4
CNSTF4 0
LEF4 $647
line 1433
;1433:			pos = (size-SCROLLBAR_SIZE) / (float) max;
ADDRLP4 0
ADDRLP4 12
INDIRF4
CNSTF4 1098907648
SUBF4
ADDRLP4 4
INDIRF4
DIVF4
ASGNF4
line 1434
;1434:		} else {
ADDRGP4 $648
JUMPV
LABELV $647
line 1435
;1435:			pos = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1436
;1436:		}
LABELV $648
line 1437
;1437:		pos *= listPtr->startPos;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 8
INDIRP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1438
;1438:		return item->window.rect.x + 1 + SCROLLBAR_SIZE + pos;
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
CNSTF4 1098907648
ADDF4
ADDRLP4 0
INDIRF4
ADDF4
CVFI4 4
RETI4
ADDRGP4 $644
JUMPV
LABELV $645
line 1440
;1439:	}
;1440:	else {
line 1441
;1441:		size = item->window.rect.h - (SCROLLBAR_SIZE * 2) - 2;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 1442
;1442:		if (max > 0) {
ADDRLP4 4
INDIRF4
CNSTF4 0
LEF4 $649
line 1443
;1443:			pos = (size-SCROLLBAR_SIZE) / (float) max;
ADDRLP4 0
ADDRLP4 12
INDIRF4
CNSTF4 1098907648
SUBF4
ADDRLP4 4
INDIRF4
DIVF4
ASGNF4
line 1444
;1444:		} else {
ADDRGP4 $650
JUMPV
LABELV $649
line 1445
;1445:			pos = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1446
;1446:		}
LABELV $650
line 1447
;1447:		pos *= listPtr->startPos;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 8
INDIRP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1448
;1448:		return item->window.rect.y + 1 + SCROLLBAR_SIZE + pos;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
CNSTF4 1098907648
ADDF4
ADDRLP4 0
INDIRF4
ADDF4
CVFI4 4
RETI4
LABELV $644
endproc Item_ListBox_ThumbPosition 20 4
export Item_ListBox_ThumbDrawPosition
proc Item_ListBox_ThumbDrawPosition 24 4
line 1452
;1449:	}
;1450:}
;1451:
;1452:int Item_ListBox_ThumbDrawPosition(itemDef_t *item) {
line 1455
;1453:	int min, max;
;1454:
;1455:	if (itemCapture == item) {
ADDRGP4 itemCapture
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $652
line 1456
;1456:		if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $654
line 1457
;1457:			min = item->window.rect.x + SCROLLBAR_SIZE + 1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1098907648
ADDF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1458
;1458:			max = item->window.rect.x + item->window.rect.w - 2*SCROLLBAR_SIZE - 1;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1107296256
SUBF4
CNSTF4 1065353216
SUBF4
CVFI4 4
ASGNI4
line 1459
;1459:			if (DC->cursorx >= min + SCROLLBAR_SIZE/2 && DC->cursorx <= max + SCROLLBAR_SIZE/2) {
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 16
CNSTF4 1090519040
ASGNF4
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
ADDF4
LTF4 $656
ADDRLP4 12
INDIRF4
ADDRLP4 4
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
ADDF4
GTF4 $656
line 1460
;1460:				return DC->cursorx - SCROLLBAR_SIZE/2;
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1090519040
SUBF4
CVFI4 4
RETI4
ADDRGP4 $651
JUMPV
LABELV $656
line 1462
;1461:			}
;1462:			else {
line 1463
;1463:				return Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 Item_ListBox_ThumbPosition
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $651
JUMPV
LABELV $654
line 1466
;1464:			}
;1465:		}
;1466:		else {
line 1467
;1467:			min = item->window.rect.y + SCROLLBAR_SIZE + 1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1098907648
ADDF4
CNSTF4 1065353216
ADDF4
CVFI4 4
ASGNI4
line 1468
;1468:			max = item->window.rect.y + item->window.rect.h - 2*SCROLLBAR_SIZE - 1;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1107296256
SUBF4
CNSTF4 1065353216
SUBF4
CVFI4 4
ASGNI4
line 1469
;1469:			if (DC->cursory >= min + SCROLLBAR_SIZE/2 && DC->cursory <= max + SCROLLBAR_SIZE/2) {
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 16
CNSTF4 1090519040
ASGNF4
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
ADDF4
LTF4 $658
ADDRLP4 12
INDIRF4
ADDRLP4 4
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRF4
ADDF4
GTF4 $658
line 1470
;1470:				return DC->cursory - SCROLLBAR_SIZE/2;
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1090519040
SUBF4
CVFI4 4
RETI4
ADDRGP4 $651
JUMPV
LABELV $658
line 1472
;1471:			}
;1472:			else {
line 1473
;1473:				return Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 Item_ListBox_ThumbPosition
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $651
JUMPV
LABELV $652
line 1477
;1474:			}
;1475:		}
;1476:	}
;1477:	else {
line 1478
;1478:		return Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Item_ListBox_ThumbPosition
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
LABELV $651
endproc Item_ListBox_ThumbDrawPosition 24 4
export Item_Slider_ThumbPosition
proc Item_Slider_ThumbPosition 28 4
line 1482
;1479:	}
;1480:}
;1481:
;1482:float Item_Slider_ThumbPosition(itemDef_t *item) {
line 1484
;1483:	float value, range, x;
;1484:	editFieldDef_t *editDef = item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1486
;1485:
;1486:	if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $661
line 1487
;1487:		x = item->textRect.x + item->textRect.w + 8;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 1488
;1488:	} else {
ADDRGP4 $662
JUMPV
LABELV $661
line 1489
;1489:		x = item->window.rect.x;
ADDRLP4 8
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 1490
;1490:	}
LABELV $662
line 1492
;1491:
;1492:	if (editDef == NULL && item->cvar) {
ADDRLP4 16
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
NEU4 $663
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $663
line 1493
;1493:		return x;
ADDRLP4 8
INDIRF4
RETF4
ADDRGP4 $660
JUMPV
LABELV $663
line 1496
;1494:	}
;1495:
;1496:	value = DC->getCVarValue(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
line 1498
;1497:
;1498:	if (value < editDef->minVal) {
ADDRLP4 0
INDIRF4
ADDRLP4 4
INDIRP4
INDIRF4
GEF4 $665
line 1499
;1499:		value = editDef->minVal;
ADDRLP4 0
ADDRLP4 4
INDIRP4
INDIRF4
ASGNF4
line 1500
;1500:	} else if (value > editDef->maxVal) {
ADDRGP4 $666
JUMPV
LABELV $665
ADDRLP4 0
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
LEF4 $667
line 1501
;1501:		value = editDef->maxVal;
ADDRLP4 0
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 1502
;1502:	}
LABELV $667
LABELV $666
line 1504
;1503:
;1504:	range = editDef->maxVal - editDef->minVal;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 4
INDIRP4
INDIRF4
SUBF4
ASGNF4
line 1505
;1505:	value -= editDef->minVal;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 4
INDIRP4
INDIRF4
SUBF4
ASGNF4
line 1506
;1506:	value /= range;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 12
INDIRF4
DIVF4
ASGNF4
line 1508
;1507:	//value /= (editDef->maxVal - editDef->minVal);
;1508:	value *= SLIDER_WIDTH;
ADDRLP4 0
CNSTF4 1119879168
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 1509
;1509:	x += value;
ADDRLP4 8
ADDRLP4 8
INDIRF4
ADDRLP4 0
INDIRF4
ADDF4
ASGNF4
line 1512
;1510:	// vm fuckage
;1511:	//x = x + (((float)value / editDef->maxVal) * SLIDER_WIDTH);
;1512:	return x;
ADDRLP4 8
INDIRF4
RETF4
LABELV $660
endproc Item_Slider_ThumbPosition 28 4
export Item_Slider_OverSlider
proc Item_Slider_OverSlider 24 12
line 1515
;1513:}
;1514:
;1515:int Item_Slider_OverSlider(itemDef_t *item, float x, float y) {
line 1518
;1516:	rectDef_t r;
;1517:
;1518:	r.x = Item_Slider_ThumbPosition(item) - (SLIDER_THUMB_WIDTH / 2);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Item_Slider_ThumbPosition
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 16
INDIRF4
CNSTF4 1086324736
SUBF4
ASGNF4
line 1519
;1519:	r.y = item->window.rect.y - 2;
ADDRLP4 0+4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 1520
;1520:	r.w = SLIDER_THUMB_WIDTH;
ADDRLP4 0+8
CNSTF4 1094713344
ASGNF4
line 1521
;1521:	r.h = SLIDER_THUMB_HEIGHT;
ADDRLP4 0+12
CNSTF4 1101004800
ASGNF4
line 1523
;1522:
;1523:	if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 20
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $673
line 1524
;1524:		return WINDOW_LB_THUMB;
CNSTI4 8192
RETI4
ADDRGP4 $669
JUMPV
LABELV $673
line 1526
;1525:	}
;1526:	return 0;
CNSTI4 0
RETI4
LABELV $669
endproc Item_Slider_OverSlider 24 12
export Item_ListBox_OverLB
proc Item_ListBox_OverLB 72 12
line 1529
;1527:}
;1528:
;1529:int Item_ListBox_OverLB(itemDef_t *item, float x, float y) {
line 1535
;1530:	rectDef_t r;
;1531:	listBoxDef_t *listPtr;
;1532:	int thumbstart;
;1533:	int count;
;1534:
;1535:	count = DC->feederCount(item->special);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 28
ADDRGP4 DC
INDIRP4
CNSTI4 120
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 28
INDIRI4
ASGNI4
line 1536
;1536:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1537
;1537:	if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $676
line 1539
;1538:		// check if on left arrow
;1539:		r.x = item->window.rect.x;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 1540
;1540:		r.y = item->window.rect.y + item->window.rect.h - SCROLLBAR_SIZE;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1541
;1541:		r.h = r.w = SCROLLBAR_SIZE;
ADDRLP4 36
CNSTF4 1098907648
ASGNF4
ADDRLP4 0+8
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0+12
ADDRLP4 36
INDIRF4
ASGNF4
line 1542
;1542:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $681
line 1543
;1543:			return WINDOW_LB_LEFTARROW;
CNSTI4 2048
RETI4
ADDRGP4 $675
JUMPV
LABELV $681
line 1546
;1544:		}
;1545:		// check if on right arrow
;1546:		r.x = item->window.rect.x + item->window.rect.w - SCROLLBAR_SIZE;
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 44
INDIRP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1547
;1547:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $683
line 1548
;1548:			return WINDOW_LB_RIGHTARROW;
CNSTI4 4096
RETI4
ADDRGP4 $675
JUMPV
LABELV $683
line 1551
;1549:		}
;1550:		// check if on thumb
;1551:		thumbstart = Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 Item_ListBox_ThumbPosition
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 52
INDIRI4
ASGNI4
line 1552
;1552:		r.x = thumbstart;
ADDRLP4 0
ADDRLP4 16
INDIRI4
CVIF4 4
ASGNF4
line 1553
;1553:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 56
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $685
line 1554
;1554:			return WINDOW_LB_THUMB;
CNSTI4 8192
RETI4
ADDRGP4 $675
JUMPV
LABELV $685
line 1556
;1555:		}
;1556:		r.x = item->window.rect.x + SCROLLBAR_SIZE;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 1557
;1557:		r.w = thumbstart - r.x;
ADDRLP4 0+8
ADDRLP4 16
INDIRI4
CVIF4 4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
line 1558
;1558:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 60
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $688
line 1559
;1559:			return WINDOW_LB_PGUP;
CNSTI4 16384
RETI4
ADDRGP4 $675
JUMPV
LABELV $688
line 1561
;1560:		}
;1561:		r.x = thumbstart + SCROLLBAR_SIZE;
ADDRLP4 0
ADDRLP4 16
INDIRI4
CVIF4 4
CNSTF4 1098907648
ADDF4
ASGNF4
line 1562
;1562:		r.w = item->window.rect.x + item->window.rect.w - SCROLLBAR_SIZE;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1563
;1563:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $677
line 1564
;1564:			return WINDOW_LB_PGDN;
CNSTI4 32768
RETI4
ADDRGP4 $675
JUMPV
line 1566
;1565:		}
;1566:	} else {
LABELV $676
line 1567
;1567:		r.x = item->window.rect.x + item->window.rect.w - SCROLLBAR_SIZE;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1568
;1568:		r.y = item->window.rect.y;
ADDRLP4 0+4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 1569
;1569:		r.h = r.w = SCROLLBAR_SIZE;
ADDRLP4 36
CNSTF4 1098907648
ASGNF4
ADDRLP4 0+8
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 0+12
ADDRLP4 36
INDIRF4
ASGNF4
line 1570
;1570:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $696
line 1571
;1571:			return WINDOW_LB_LEFTARROW;
CNSTI4 2048
RETI4
ADDRGP4 $675
JUMPV
LABELV $696
line 1573
;1572:		}
;1573:		r.y = item->window.rect.y + item->window.rect.h - SCROLLBAR_SIZE;
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1574
;1574:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
EQI4 $699
line 1575
;1575:			return WINDOW_LB_RIGHTARROW;
CNSTI4 4096
RETI4
ADDRGP4 $675
JUMPV
LABELV $699
line 1577
;1576:		}
;1577:		thumbstart = Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 Item_ListBox_ThumbPosition
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 52
INDIRI4
ASGNI4
line 1578
;1578:		r.y = thumbstart;
ADDRLP4 0+4
ADDRLP4 16
INDIRI4
CVIF4 4
ASGNF4
line 1579
;1579:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 56
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $702
line 1580
;1580:			return WINDOW_LB_THUMB;
CNSTI4 8192
RETI4
ADDRGP4 $675
JUMPV
LABELV $702
line 1582
;1581:		}
;1582:		r.y = item->window.rect.y + SCROLLBAR_SIZE;
ADDRLP4 0+4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 1583
;1583:		r.h = thumbstart - r.y;
ADDRLP4 0+12
ADDRLP4 16
INDIRI4
CVIF4 4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
line 1584
;1584:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 60
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $707
line 1585
;1585:			return WINDOW_LB_PGUP;
CNSTI4 16384
RETI4
ADDRGP4 $675
JUMPV
LABELV $707
line 1587
;1586:		}
;1587:		r.y = thumbstart + SCROLLBAR_SIZE;
ADDRLP4 0+4
ADDRLP4 16
INDIRI4
CVIF4 4
CNSTF4 1098907648
ADDF4
ASGNF4
line 1588
;1588:		r.h = item->window.rect.y + item->window.rect.h - SCROLLBAR_SIZE;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+12
ADDRLP4 64
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1589
;1589:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
EQI4 $711
line 1590
;1590:			return WINDOW_LB_PGDN;
CNSTI4 32768
RETI4
ADDRGP4 $675
JUMPV
LABELV $711
line 1592
;1591:		}
;1592:	}
LABELV $677
line 1593
;1593:	return 0;
CNSTI4 0
RETI4
LABELV $675
endproc Item_ListBox_OverLB 72 12
export Item_ListBox_MouseEnter
proc Item_ListBox_MouseEnter 56 12
line 1598
;1594:}
;1595:
;1596:
;1597:void Item_ListBox_MouseEnter(itemDef_t *item, float x, float y) 
;1598:{
line 1600
;1599:	rectDef_t r;
;1600:	listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1602
;1601:        
;1602:	item->window.flags &= ~(WINDOW_LB_LEFTARROW | WINDOW_LB_RIGHTARROW | WINDOW_LB_THUMB | WINDOW_LB_PGUP | WINDOW_LB_PGDN);
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 -63489
BANDI4
ASGNI4
line 1603
;1603:	item->window.flags |= Item_ListBox_OverLB(item, x, y);
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 28
ADDRGP4 Item_ListBox_OverLB
CALLI4
ASGNI4
ADDRLP4 32
ADDRLP4 24
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
ADDRLP4 28
INDIRI4
BORI4
ASGNI4
line 1605
;1604:
;1605:	if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $714
line 1606
;1606:		if (!(item->window.flags & (WINDOW_LB_LEFTARROW | WINDOW_LB_RIGHTARROW | WINDOW_LB_THUMB | WINDOW_LB_PGUP | WINDOW_LB_PGDN))) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 63488
BANDI4
CNSTI4 0
NEI4 $715
line 1608
;1607:			// check for selection hit as we have exausted buttons and thumb
;1608:			if (listPtr->elementStyle == LISTBOX_IMAGE) {
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 1
NEI4 $715
line 1609
;1609:				r.x = item->window.rect.x;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 1610
;1610:				r.y = item->window.rect.y;
ADDRLP4 4+4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 1611
;1611:				r.h = item->window.rect.h - SCROLLBAR_SIZE;
ADDRLP4 4+12
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1612
;1612:				r.w = item->window.rect.w - listPtr->drawPadding;
ADDRLP4 36
CNSTI4 8
ASGNI4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 1613
;1613:				if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 40
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
EQI4 $715
line 1614
;1614:					listPtr->cursorPos =  (int)((x - r.x) / listPtr->elementWidth)  + listPtr->startPos;
ADDRLP4 44
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 4
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ADDRLP4 44
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
DIVF4
CVFI4 4
ADDRLP4 44
INDIRP4
INDIRI4
ADDI4
ASGNI4
line 1615
;1615:					if (listPtr->cursorPos >= listPtr->endPos) {
ADDRLP4 48
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
LTI4 $715
line 1616
;1616:						listPtr->cursorPos = listPtr->endPos;
ADDRLP4 52
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 1617
;1617:					}
line 1618
;1618:				}
line 1619
;1619:			} else {
line 1621
;1620:				// text hit.. 
;1621:			}
line 1622
;1622:		}
line 1623
;1623:	} else if (!(item->window.flags & (WINDOW_LB_LEFTARROW | WINDOW_LB_RIGHTARROW | WINDOW_LB_THUMB | WINDOW_LB_PGUP | WINDOW_LB_PGDN))) {
ADDRGP4 $715
JUMPV
LABELV $714
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 63488
BANDI4
CNSTI4 0
NEI4 $727
line 1624
;1624:		r.x = item->window.rect.x;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 1625
;1625:		r.y = item->window.rect.y;
ADDRLP4 4+4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 1626
;1626:		r.w = item->window.rect.w - SCROLLBAR_SIZE;
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1098907648
SUBF4
ASGNF4
line 1627
;1627:		r.h = item->window.rect.h - listPtr->drawPadding;
ADDRLP4 4+12
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 1628
;1628:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $732
line 1629
;1629:			listPtr->cursorPos =  (int)((y - 2 - r.y) / listPtr->elementHeight)  + listPtr->startPos;
ADDRLP4 40
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 8
INDIRF4
CNSTF4 1073741824
SUBF4
ADDRLP4 4+4
INDIRF4
SUBF4
ADDRLP4 40
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
DIVF4
CVFI4 4
ADDRLP4 40
INDIRP4
INDIRI4
ADDI4
ASGNI4
line 1630
;1630:			if (listPtr->cursorPos > listPtr->endPos) {
ADDRLP4 44
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
LEI4 $735
line 1631
;1631:				listPtr->cursorPos = listPtr->endPos;
ADDRLP4 48
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
line 1632
;1632:			}
LABELV $735
line 1633
;1633:		}
LABELV $732
line 1634
;1634:	}
LABELV $727
LABELV $715
line 1635
;1635:}
LABELV $713
endproc Item_ListBox_MouseEnter 56 12
export Item_MouseEnter
proc Item_MouseEnter 44 12
line 1637
;1636:
;1637:void Item_MouseEnter(itemDef_t *item, float x, float y) {
line 1639
;1638:	rectDef_t r;
;1639:	if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $738
line 1640
;1640:		r = item->textRect;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRB
ASGNB 16
line 1641
;1641:		r.y -= r.h;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 0+12
INDIRF4
SUBF4
ASGNF4
line 1645
;1642:		// in the text rect?
;1643:
;1644:		// items can be enabled and disabled based on cvars
;1645:		if (item->cvarFlags & (CVAR_ENABLE | CVAR_DISABLE) && !Item_EnableShowViaCvar(item, CVAR_ENABLE)) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $742
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 20
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $742
line 1646
;1646:			return;
ADDRGP4 $737
JUMPV
LABELV $742
line 1649
;1647:		}
;1648:
;1649:		if (item->cvarFlags & (CVAR_SHOW | CVAR_HIDE) && !Item_EnableShowViaCvar(item, CVAR_SHOW)) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 12
BANDI4
CNSTI4 0
EQI4 $744
ADDRLP4 24
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 28
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $744
line 1650
;1650:			return;
ADDRGP4 $737
JUMPV
LABELV $744
line 1653
;1651:		}
;1652:
;1653:		if (Rect_ContainsPoint(&r, x, y)) {
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 32
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $746
line 1654
;1654:			if (!(item->window.flags & WINDOW_MOUSEOVERTEXT)) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
NEI4 $748
line 1655
;1655:				Item_RunScript(item, item->mouseEnterText);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 236
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1656
;1656:				item->window.flags |= WINDOW_MOUSEOVERTEXT;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1657
;1657:			}
LABELV $748
line 1658
;1658:			if (!(item->window.flags & WINDOW_MOUSEOVER)) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $747
line 1659
;1659:				Item_RunScript(item, item->mouseEnter);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 244
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1660
;1660:				item->window.flags |= WINDOW_MOUSEOVER;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1661
;1661:			}
line 1663
;1662:
;1663:		} else {
ADDRGP4 $747
JUMPV
LABELV $746
line 1665
;1664:			// not in the text rect
;1665:			if (item->window.flags & WINDOW_MOUSEOVERTEXT) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $752
line 1667
;1666:				// if we were
;1667:				Item_RunScript(item, item->mouseExitText);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1668
;1668:				item->window.flags &= ~WINDOW_MOUSEOVERTEXT;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 1669
;1669:			}
LABELV $752
line 1670
;1670:			if (!(item->window.flags & WINDOW_MOUSEOVER)) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $754
line 1671
;1671:				Item_RunScript(item, item->mouseEnter);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 244
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1672
;1672:				item->window.flags |= WINDOW_MOUSEOVER;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1673
;1673:			}
LABELV $754
line 1675
;1674:
;1675:			if (item->type == ITEM_TYPE_LISTBOX) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 6
NEI4 $756
line 1676
;1676:				Item_ListBox_MouseEnter(item, x, y);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRGP4 Item_ListBox_MouseEnter
CALLV
pop
line 1677
;1677:			}
LABELV $756
line 1678
;1678:		}
LABELV $747
line 1679
;1679:	}
LABELV $738
line 1680
;1680:}
LABELV $737
endproc Item_MouseEnter 44 12
export Item_MouseLeave
proc Item_MouseLeave 8 8
line 1682
;1681:
;1682:void Item_MouseLeave(itemDef_t *item) {
line 1683
;1683:  if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $759
line 1684
;1684:    if (item->window.flags & WINDOW_MOUSEOVERTEXT) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $761
line 1685
;1685:      Item_RunScript(item, item->mouseExitText);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1686
;1686:      item->window.flags &= ~WINDOW_MOUSEOVERTEXT;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 1687
;1687:    }
LABELV $761
line 1688
;1688:    Item_RunScript(item, item->mouseExit);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1689
;1689:    item->window.flags &= ~(WINDOW_LB_RIGHTARROW | WINDOW_LB_LEFTARROW);
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -6145
BANDI4
ASGNI4
line 1690
;1690:  }
LABELV $759
line 1691
;1691:}
LABELV $758
endproc Item_MouseLeave 8 8
export Menu_HitTest
proc Menu_HitTest 8 12
line 1693
;1692:
;1693:itemDef_t *Menu_HitTest(menuDef_t *menu, float x, float y) {
line 1695
;1694:  int i;
;1695:  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $767
JUMPV
LABELV $764
line 1696
;1696:    if (Rect_ContainsPoint(&menu->items[i]->window.rect, x, y)) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $768
line 1697
;1697:      return menu->items[i];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $763
JUMPV
LABELV $768
line 1699
;1698:    }
;1699:  }
LABELV $765
line 1695
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $767
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $764
line 1700
;1700:  return NULL;
CNSTP4 0
RETP4
LABELV $763
endproc Menu_HitTest 8 12
export Item_SetMouseOver
proc Item_SetMouseOver 4 0
line 1703
;1701:}
;1702:
;1703:void Item_SetMouseOver(itemDef_t *item, qboolean focus) {
line 1704
;1704:  if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $771
line 1705
;1705:    if (focus) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $773
line 1706
;1706:      item->window.flags |= WINDOW_MOUSEOVER;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1707
;1707:    } else {
ADDRGP4 $774
JUMPV
LABELV $773
line 1708
;1708:      item->window.flags &= ~WINDOW_MOUSEOVER;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 1709
;1709:    }
LABELV $774
line 1710
;1710:  }
LABELV $771
line 1711
;1711:}
LABELV $770
endproc Item_SetMouseOver 4 0
export Item_OwnerDraw_HandleKey
proc Item_OwnerDraw_HandleKey 12 16
line 1714
;1712:
;1713:
;1714:qboolean Item_OwnerDraw_HandleKey(itemDef_t *item, int key) {
line 1715
;1715:  if (item && DC->ownerDrawHandleKey) {
ADDRLP4 0
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 0
INDIRU4
EQU4 $776
ADDRGP4 DC
INDIRP4
CNSTI4 116
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 0
INDIRU4
EQU4 $776
line 1716
;1716:    return DC->ownerDrawHandleKey(item->window.ownerDraw, item->window.ownerDrawFlags, &item->special, key);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 528
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
CNSTI4 116
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
ADDRGP4 $775
JUMPV
LABELV $776
line 1718
;1717:  }
;1718:  return qfalse;
CNSTI4 0
RETI4
LABELV $775
endproc Item_OwnerDraw_HandleKey 12 16
export Item_ListBox_HandleKey
proc Item_ListBox_HandleKey 72 12
line 1721
;1719:}
;1720:
;1721:qboolean Item_ListBox_HandleKey(itemDef_t *item, int key, qboolean down, qboolean force) {
line 1722
;1722:	listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1723
;1723:	int count = DC->feederCount(item->special);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 DC
INDIRP4
CNSTI4 120
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
ASGNI4
line 1726
;1724:	int max, viewmax;
;1725:
;1726:	if (force || (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory) && item->window.flags & WINDOW_HASFOCUS)) {
ADDRFP4 12
INDIRI4
CNSTI4 0
NEI4 $781
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRI4
ADDRLP4 28
INDIRI4
EQI4 $779
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 28
INDIRI4
EQI4 $779
LABELV $781
line 1727
;1727:		max = Item_ListBox_MaxScroll(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 Item_ListBox_MaxScroll
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 32
INDIRI4
ASGNI4
line 1728
;1728:		if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $782
line 1729
;1729:			viewmax = (item->window.rect.w / listPtr->elementWidth);
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
DIVF4
CVFI4 4
ASGNI4
line 1730
;1730:			if ( key == K_LEFTARROW || key == K_KP_LEFTARROW ) 
ADDRLP4 36
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 134
EQI4 $786
ADDRLP4 36
INDIRI4
CNSTI4 163
NEI4 $784
LABELV $786
line 1731
;1731:			{
line 1732
;1732:				if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $787
line 1733
;1733:					listPtr->cursorPos--;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1734
;1734:					if (listPtr->cursorPos < 0) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 0
GEI4 $789
line 1735
;1735:						listPtr->cursorPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 1736
;1736:					}
LABELV $789
line 1737
;1737:					if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $791
line 1738
;1738:						listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1739
;1739:					}
LABELV $791
line 1740
;1740:					if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $793
line 1741
;1741:						listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1742
;1742:					}
LABELV $793
line 1743
;1743:					item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1744
;1744:					DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1745
;1745:				}
ADDRGP4 $788
JUMPV
LABELV $787
line 1746
;1746:				else {
line 1747
;1747:					listPtr->startPos--;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1748
;1748:					if (listPtr->startPos < 0)
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
GEI4 $795
line 1749
;1749:						listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
LABELV $795
line 1750
;1750:				}
LABELV $788
line 1751
;1751:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $784
line 1753
;1752:			}
;1753:			if ( key == K_RIGHTARROW || key == K_KP_RIGHTARROW ) 
ADDRLP4 40
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 135
EQI4 $799
ADDRLP4 40
INDIRI4
CNSTI4 165
NEI4 $783
LABELV $799
line 1754
;1754:			{
line 1755
;1755:				if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $800
line 1756
;1756:					listPtr->cursorPos++;
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1757
;1757:					if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $802
line 1758
;1758:						listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1759
;1759:					}
LABELV $802
line 1760
;1760:					if (listPtr->cursorPos >= count) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $804
line 1761
;1761:						listPtr->cursorPos = count-1;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1762
;1762:					}
LABELV $804
line 1763
;1763:					if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $806
line 1764
;1764:						listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1765
;1765:					}
LABELV $806
line 1766
;1766:					item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1767
;1767:					DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 56
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1768
;1768:				}
ADDRGP4 $801
JUMPV
LABELV $800
line 1769
;1769:				else {
line 1770
;1770:					listPtr->startPos++;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1771
;1771:					if (listPtr->startPos >= count)
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $808
line 1772
;1772:						listPtr->startPos = count-1;
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $808
line 1773
;1773:				}
LABELV $801
line 1774
;1774:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
line 1776
;1775:			}
;1776:		}
LABELV $782
line 1777
;1777:		else {
line 1778
;1778:			viewmax = (item->window.rect.h / listPtr->elementHeight);
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
DIVF4
CVFI4 4
ASGNI4
line 1779
;1779:			if ( key == K_UPARROW || key == K_KP_UPARROW ) 
ADDRLP4 36
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 132
EQI4 $812
ADDRLP4 36
INDIRI4
CNSTI4 161
NEI4 $810
LABELV $812
line 1780
;1780:			{
line 1781
;1781:				if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $813
line 1782
;1782:					listPtr->cursorPos--;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1783
;1783:					if (listPtr->cursorPos < 0) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 0
GEI4 $815
line 1784
;1784:						listPtr->cursorPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 1785
;1785:					}
LABELV $815
line 1786
;1786:					if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $817
line 1787
;1787:						listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1788
;1788:					}
LABELV $817
line 1789
;1789:					if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $819
line 1790
;1790:						listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1791
;1791:					}
LABELV $819
line 1792
;1792:					item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1793
;1793:					DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1794
;1794:				}
ADDRGP4 $814
JUMPV
LABELV $813
line 1795
;1795:				else {
line 1796
;1796:					listPtr->startPos--;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1797
;1797:					if (listPtr->startPos < 0)
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
GEI4 $821
line 1798
;1798:						listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
LABELV $821
line 1799
;1799:				}
LABELV $814
line 1800
;1800:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $810
line 1802
;1801:			}
;1802:			if ( key == K_DOWNARROW || key == K_KP_DOWNARROW ) 
ADDRLP4 40
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 133
EQI4 $825
ADDRLP4 40
INDIRI4
CNSTI4 167
NEI4 $823
LABELV $825
line 1803
;1803:			{
line 1804
;1804:				if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $826
line 1805
;1805:					listPtr->cursorPos++;
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1806
;1806:					if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $828
line 1807
;1807:						listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1808
;1808:					}
LABELV $828
line 1809
;1809:					if (listPtr->cursorPos >= count) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $830
line 1810
;1810:						listPtr->cursorPos = count-1;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1811
;1811:					}
LABELV $830
line 1812
;1812:					if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $832
line 1813
;1813:						listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1814
;1814:					}
LABELV $832
line 1815
;1815:					item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1816
;1816:					DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 56
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1817
;1817:				}
ADDRGP4 $827
JUMPV
LABELV $826
line 1818
;1818:				else {
line 1819
;1819:					listPtr->startPos++;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1820
;1820:					if (listPtr->startPos > max)
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $834
line 1821
;1821:						listPtr->startPos = max;
ADDRLP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
LABELV $834
line 1822
;1822:				}
LABELV $827
line 1823
;1823:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $823
line 1825
;1824:			}
;1825:		}
LABELV $783
line 1827
;1826:		// mouse hit
;1827:		if (key == K_MOUSE1 || key == K_MOUSE2) {
ADDRLP4 36
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 178
EQI4 $838
ADDRLP4 36
INDIRI4
CNSTI4 179
NEI4 $836
LABELV $838
line 1828
;1828:			if (item->window.flags & WINDOW_LB_LEFTARROW) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $839
line 1829
;1829:				listPtr->startPos--;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1830
;1830:				if (listPtr->startPos < 0) {
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
GEI4 $840
line 1831
;1831:					listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1832
;1832:				}
line 1833
;1833:			} else if (item->window.flags & WINDOW_LB_RIGHTARROW) {
ADDRGP4 $840
JUMPV
LABELV $839
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $843
line 1835
;1834:				// one down
;1835:				listPtr->startPos++;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1836
;1836:				if (listPtr->startPos > max) {
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $844
line 1837
;1837:					listPtr->startPos = max;
ADDRLP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1838
;1838:				}
line 1839
;1839:			} else if (item->window.flags & WINDOW_LB_PGUP) {
ADDRGP4 $844
JUMPV
LABELV $843
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $847
line 1841
;1840:				// page up
;1841:				listPtr->startPos -= viewmax;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 1842
;1842:				if (listPtr->startPos < 0) {
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
GEI4 $848
line 1843
;1843:					listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1844
;1844:				}
line 1845
;1845:			} else if (item->window.flags & WINDOW_LB_PGDN) {
ADDRGP4 $848
JUMPV
LABELV $847
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 32768
BANDI4
CNSTI4 0
EQI4 $851
line 1847
;1846:				// page down
;1847:				listPtr->startPos += viewmax;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 1848
;1848:				if (listPtr->startPos > max) {
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $852
line 1849
;1849:					listPtr->startPos = max;
ADDRLP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1850
;1850:				}
line 1851
;1851:			} else if (item->window.flags & WINDOW_LB_THUMB) {
ADDRGP4 $852
JUMPV
LABELV $851
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $855
line 1853
;1852:				// Display_SetCaptureItem(item);
;1853:			} else {
ADDRGP4 $856
JUMPV
LABELV $855
line 1855
;1854:				// select an item
;1855:				if (DC->realTime < lastListBoxClickTime && listPtr->doubleClick) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRGP4 lastListBoxClickTime
INDIRI4
GEI4 $857
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $857
line 1856
;1856:					Item_RunScript(item, listPtr->doubleClick);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 1857
;1857:				}
LABELV $857
line 1858
;1858:				lastListBoxClickTime = DC->realTime + DOUBLE_CLICK_DELAY;
ADDRGP4 lastListBoxClickTime
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 300
ADDI4
ASGNI4
line 1859
;1859:				if (item->cursorPos != listPtr->cursorPos) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
EQI4 $859
line 1860
;1860:					item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1861
;1861:					DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1862
;1862:				}
LABELV $859
line 1863
;1863:			}
LABELV $856
LABELV $852
LABELV $848
LABELV $844
LABELV $840
line 1864
;1864:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $836
line 1866
;1865:		}
;1866:		if ( key == K_HOME || key == K_KP_HOME) {
ADDRLP4 40
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 143
EQI4 $863
ADDRLP4 40
INDIRI4
CNSTI4 160
NEI4 $861
LABELV $863
line 1868
;1867:			// home
;1868:			listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1869
;1869:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $861
line 1871
;1870:		}
;1871:		if ( key == K_END || key == K_KP_END) {
ADDRLP4 44
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 144
EQI4 $866
ADDRLP4 44
INDIRI4
CNSTI4 166
NEI4 $864
LABELV $866
line 1873
;1872:			// end
;1873:			listPtr->startPos = max;
ADDRLP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1874
;1874:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $864
line 1876
;1875:		}
;1876:		if (key == K_PGUP || key == K_KP_PGUP ) {
ADDRLP4 48
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 142
EQI4 $869
ADDRLP4 48
INDIRI4
CNSTI4 162
NEI4 $867
LABELV $869
line 1878
;1877:			// page up
;1878:			if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $870
line 1879
;1879:				listPtr->cursorPos -= viewmax;
ADDRLP4 52
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 1880
;1880:				if (listPtr->cursorPos < 0) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 0
GEI4 $872
line 1881
;1881:					listPtr->cursorPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 1882
;1882:				}
LABELV $872
line 1883
;1883:				if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $874
line 1884
;1884:					listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1885
;1885:				}
LABELV $874
line 1886
;1886:				if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $876
line 1887
;1887:					listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1888
;1888:				}
LABELV $876
line 1889
;1889:				item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1890
;1890:				DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 64
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1891
;1891:			}
ADDRGP4 $871
JUMPV
LABELV $870
line 1892
;1892:			else {
line 1893
;1893:				listPtr->startPos -= viewmax;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 1894
;1894:				if (listPtr->startPos < 0) {
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
GEI4 $878
line 1895
;1895:					listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 1896
;1896:				}
LABELV $878
line 1897
;1897:			}
LABELV $871
line 1898
;1898:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $867
line 1900
;1899:		}
;1900:		if ( key == K_PGDN || key == K_KP_PGDN ) {
ADDRLP4 52
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 141
EQI4 $882
ADDRLP4 52
INDIRI4
CNSTI4 168
NEI4 $880
LABELV $882
line 1902
;1901:			// page down
;1902:			if (!listPtr->notselectable) {
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRI4
CNSTI4 0
NEI4 $883
line 1903
;1903:				listPtr->cursorPos += viewmax;
ADDRLP4 56
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 1904
;1904:				if (listPtr->cursorPos < listPtr->startPos) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
GEI4 $885
line 1905
;1905:					listPtr->startPos = listPtr->cursorPos;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1906
;1906:				}
LABELV $885
line 1907
;1907:				if (listPtr->cursorPos >= count) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $887
line 1908
;1908:					listPtr->cursorPos = count-1;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1909
;1909:				}
LABELV $887
line 1910
;1910:				if (listPtr->cursorPos >= listPtr->startPos + viewmax) {
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
LTI4 $889
line 1911
;1911:					listPtr->startPos = listPtr->cursorPos - viewmax + 1;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
CNSTI4 1
ADDI4
ASGNI4
line 1912
;1912:				}
LABELV $889
line 1913
;1913:				item->cursorPos = listPtr->cursorPos;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
line 1914
;1914:				DC->feederSelection(item->special, item->cursorPos);
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 68
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 1915
;1915:			}
ADDRGP4 $884
JUMPV
LABELV $883
line 1916
;1916:			else {
line 1917
;1917:				listPtr->startPos += viewmax;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 1918
;1918:				if (listPtr->startPos > max) {
ADDRLP4 0
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $891
line 1919
;1919:					listPtr->startPos = max;
ADDRLP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1920
;1920:				}
LABELV $891
line 1921
;1921:			}
LABELV $884
line 1922
;1922:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $778
JUMPV
LABELV $880
line 1924
;1923:		}
;1924:	}
LABELV $779
line 1925
;1925:	return qfalse;
CNSTI4 0
RETI4
LABELV $778
endproc Item_ListBox_HandleKey 72 12
export Item_YesNo_HandleKey
proc Item_YesNo_HandleKey 32 12
line 1928
;1926:}
;1927:
;1928:qboolean Item_YesNo_HandleKey(itemDef_t *item, int key) {
line 1930
;1929:
;1930:  if (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory) && item->window.flags & WINDOW_HASFOCUS && item->cvar) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $894
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $894
ADDRLP4 12
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $894
line 1931
;1931:		if (key == K_MOUSE1 || key == K_ENTER || key == K_MOUSE2 || key == K_MOUSE3) {
ADDRLP4 16
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 178
EQI4 $900
ADDRLP4 16
INDIRI4
CNSTI4 13
EQI4 $900
ADDRLP4 16
INDIRI4
CNSTI4 179
EQI4 $900
ADDRLP4 16
INDIRI4
CNSTI4 180
NEI4 $896
LABELV $900
line 1932
;1932:	    DC->setCVar(item->cvar, va("%i", !DC->getCVarValue(item->cvar)));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 24
INDIRF4
CNSTF4 0
NEF4 $903
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRGP4 $904
JUMPV
LABELV $903
ADDRLP4 20
CNSTI4 0
ASGNI4
LABELV $904
ADDRGP4 $901
ARGP4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 1933
;1933:		  return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $893
JUMPV
LABELV $896
line 1935
;1934:		}
;1935:  }
LABELV $894
line 1937
;1936:
;1937:  return qfalse;
CNSTI4 0
RETI4
LABELV $893
endproc Item_YesNo_HandleKey 32 12
export Item_Multi_CountSettings
proc Item_Multi_CountSettings 4 0
line 1941
;1938:
;1939:}
;1940:
;1941:int Item_Multi_CountSettings(itemDef_t *item) {
line 1942
;1942:	multiDef_t *multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1943
;1943:	if (multiPtr == NULL) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $906
line 1944
;1944:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $905
JUMPV
LABELV $906
line 1946
;1945:	}
;1946:	return multiPtr->count;
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
RETI4
LABELV $905
endproc Item_Multi_CountSettings 4 0
export Item_Multi_FindCvarByValue
proc Item_Multi_FindCvarByValue 1040 12
line 1949
;1947:}
;1948:
;1949:int Item_Multi_FindCvarByValue(itemDef_t *item) {
line 1951
;1950:	char buff[1024];
;1951:	float value = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 1953
;1952:	int i;
;1953:	multiDef_t *multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1954
;1954:	if (multiPtr) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $909
line 1955
;1955:		if (multiPtr->strDef) {
ADDRLP4 4
INDIRP4
CNSTI4 388
ADDP4
INDIRI4
CNSTI4 0
EQI4 $911
line 1956
;1956:	    DC->getCVarString(item->cvar, buff, sizeof(buff));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 1957
;1957:		} else {
ADDRGP4 $912
JUMPV
LABELV $911
line 1958
;1958:			value = DC->getCVarValue(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 1036
INDIRF4
ASGNF4
line 1959
;1959:		}
LABELV $912
line 1960
;1960:		for (i = 0; i < multiPtr->count; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $916
JUMPV
LABELV $913
line 1961
;1961:			if (multiPtr->strDef) {
ADDRLP4 4
INDIRP4
CNSTI4 388
ADDP4
INDIRI4
CNSTI4 0
EQI4 $917
line 1962
;1962:				if (Q_stricmp(buff, multiPtr->cvarStr[i]) == 0) {
ADDRLP4 12
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 128
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $918
line 1963
;1963:					return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $908
JUMPV
line 1965
;1964:				}
;1965:			} else {
LABELV $917
line 1966
;1966: 				if (multiPtr->cvarValue[i] == value) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 256
ADDP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $921
line 1967
;1967: 					return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $908
JUMPV
LABELV $921
line 1969
;1968: 				}
;1969: 			}
LABELV $918
line 1970
;1970: 		}
LABELV $914
line 1960
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $916
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
LTI4 $913
line 1971
;1971:	}
LABELV $909
line 1972
;1972:	return 0;
CNSTI4 0
RETI4
LABELV $908
endproc Item_Multi_FindCvarByValue 1040 12
export Item_Multi_Setting
proc Item_Multi_Setting 1040 12
line 1975
;1973:}
;1974:
;1975:const char *Item_Multi_Setting(itemDef_t *item) {
line 1977
;1976:	char buff[1024];
;1977:	float value = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 1979
;1978:	int i;
;1979:	multiDef_t *multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 1980
;1980:	if (multiPtr) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $924
line 1981
;1981:		if (multiPtr->strDef) {
ADDRLP4 0
INDIRP4
CNSTI4 388
ADDP4
INDIRI4
CNSTI4 0
EQI4 $926
line 1982
;1982:	    DC->getCVarString(item->cvar, buff, sizeof(buff));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 1983
;1983:		} else {
ADDRGP4 $927
JUMPV
LABELV $926
line 1984
;1984:			value = DC->getCVarValue(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 1036
INDIRF4
ASGNF4
line 1985
;1985:		}
LABELV $927
line 1986
;1986:		for (i = 0; i < multiPtr->count; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $931
JUMPV
LABELV $928
line 1987
;1987:			if (multiPtr->strDef) {
ADDRLP4 0
INDIRP4
CNSTI4 388
ADDP4
INDIRI4
CNSTI4 0
EQI4 $932
line 1988
;1988:				if (Q_stricmp(buff, multiPtr->cvarStr[i]) == 0) {
ADDRLP4 12
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $933
line 1989
;1989:					return multiPtr->cvarList[i];
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $923
JUMPV
line 1991
;1990:				}
;1991:			} else {
LABELV $932
line 1992
;1992: 				if (multiPtr->cvarValue[i] == value) {
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $936
line 1993
;1993:					return multiPtr->cvarList[i];
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $923
JUMPV
LABELV $936
line 1995
;1994: 				}
;1995: 			}
LABELV $933
line 1996
;1996: 		}
LABELV $929
line 1986
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $931
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
LTI4 $928
line 1997
;1997:	}
LABELV $924
line 1998
;1998:	return "";
ADDRGP4 $54
RETP4
LABELV $923
endproc Item_Multi_Setting 1040 12
export Item_Multi_HandleKey
proc Item_Multi_HandleKey 56 12
line 2001
;1999:}
;2000:
;2001:qboolean Item_Multi_HandleKey(itemDef_t *item, int key) {
line 2002
;2002:	multiDef_t *multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 2003
;2003:	if (multiPtr) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $939
line 2004
;2004:	  if (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory) && item->window.flags & WINDOW_HASFOCUS && item->cvar) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $941
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 12
INDIRI4
EQI4 $941
ADDRLP4 16
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $941
line 2005
;2005:			if (key == K_MOUSE1 || key == K_ENTER || key == K_MOUSE2 || key == K_MOUSE3) {
ADDRLP4 20
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 178
EQI4 $947
ADDRLP4 20
INDIRI4
CNSTI4 13
EQI4 $947
ADDRLP4 20
INDIRI4
CNSTI4 179
EQI4 $947
ADDRLP4 20
INDIRI4
CNSTI4 180
NEI4 $943
LABELV $947
line 2006
;2006:				int current = Item_Multi_FindCvarByValue(item) + 1;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 32
ADDRGP4 Item_Multi_FindCvarByValue
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 32
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2007
;2007:				int max = Item_Multi_CountSettings(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 Item_Multi_CountSettings
CALLI4
ASGNI4
ADDRLP4 28
ADDRLP4 36
INDIRI4
ASGNI4
line 2008
;2008:				if ( current < 0 || current >= max ) {
ADDRLP4 40
ADDRLP4 24
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
LTI4 $950
ADDRLP4 40
INDIRI4
ADDRLP4 28
INDIRI4
LTI4 $948
LABELV $950
line 2009
;2009:					current = 0;
ADDRLP4 24
CNSTI4 0
ASGNI4
line 2010
;2010:				}
LABELV $948
line 2011
;2011:				if (multiPtr->strDef) {
ADDRLP4 0
INDIRP4
CNSTI4 388
ADDP4
INDIRI4
CNSTI4 0
EQI4 $951
line 2012
;2012:					DC->setCVar(item->cvar, multiPtr->cvarStr[current]);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2013
;2013:				} else {
ADDRGP4 $952
JUMPV
LABELV $951
line 2014
;2014:					float value = multiPtr->cvarValue[current];
ADDRLP4 44
ADDRLP4 24
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDP4
INDIRF4
ASGNF4
line 2015
;2015:					if (((float)((int) value)) == value) {
ADDRLP4 48
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 48
INDIRF4
CVFI4 4
CVIF4 4
ADDRLP4 48
INDIRF4
NEF4 $953
line 2016
;2016:						DC->setCVar(item->cvar, va("%i", (int) value ));
ADDRGP4 $901
ARGP4
ADDRLP4 44
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 52
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2017
;2017:					}
ADDRGP4 $954
JUMPV
LABELV $953
line 2018
;2018:					else {
line 2019
;2019:						DC->setCVar(item->cvar, va("%f", value ));
ADDRGP4 $955
ARGP4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 52
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2020
;2020:					}
LABELV $954
line 2021
;2021:				}
LABELV $952
line 2022
;2022:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $938
JUMPV
LABELV $943
line 2024
;2023:			}
;2024:		}
LABELV $941
line 2025
;2025:	}
LABELV $939
line 2026
;2026:  return qfalse;
CNSTI4 0
RETI4
LABELV $938
endproc Item_Multi_HandleKey 56 12
export Item_TextField_HandleKey
proc Item_TextField_HandleKey 1076 12
line 2029
;2027:}
;2028:
;2029:qboolean Item_TextField_HandleKey(itemDef_t *item, int key) {
line 2032
;2030:	char buff[1024];
;2031:	int len;
;2032:	itemDef_t *newItem = NULL;
ADDRLP4 1028
CNSTP4 0
ASGNP4
line 2033
;2033:	editFieldDef_t *editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 2035
;2034:
;2035:	if (item->cvar) {
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $957
line 2037
;2036:
;2037:		memset(buff, 0, sizeof(buff));
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 2038
;2038:		DC->getCVarString(item->cvar, buff, sizeof(buff));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 2039
;2039:		len = strlen(buff);
ADDRLP4 4
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 1036
INDIRI4
ASGNI4
line 2040
;2040:		if (editPtr->maxChars && len > editPtr->maxChars) {
ADDRLP4 1040
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
EQI4 $959
ADDRLP4 1032
INDIRI4
ADDRLP4 1040
INDIRI4
LEI4 $959
line 2041
;2041:			len = editPtr->maxChars;
ADDRLP4 1032
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
line 2042
;2042:		}
LABELV $959
line 2043
;2043:		if ( key & K_CHAR_FLAG ) {
ADDRFP4 4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $961
line 2044
;2044:			key &= ~K_CHAR_FLAG;
ADDRFP4 4
ADDRFP4 4
INDIRI4
CNSTI4 -1025
BANDI4
ASGNI4
line 2047
;2045:
;2046:
;2047:			if (key == 'h' - 'a' + 1 )	{	// ctrl-h is backspace
ADDRFP4 4
INDIRI4
CNSTI4 8
NEI4 $963
line 2048
;2048:				if ( item->cursorPos > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
LEI4 $965
line 2049
;2049:					memmove( &buff[item->cursorPos - 1], &buff[item->cursorPos], len + 1 - item->cursorPos);
ADDRLP4 1044
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1044
INDIRI4
ADDRLP4 4-1
ADDP4
ARGP4
ADDRLP4 1044
INDIRI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 1032
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 1044
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 2050
;2050:					item->cursorPos--;
ADDRLP4 1048
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ASGNP4
ADDRLP4 1048
INDIRP4
ADDRLP4 1048
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2051
;2051:					if (item->cursorPos < editPtr->paintOffset) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
GEI4 $968
line 2052
;2052:						editPtr->paintOffset--;
ADDRLP4 1052
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
ADDRLP4 1052
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2053
;2053:					}
LABELV $968
line 2054
;2054:				}
LABELV $965
line 2055
;2055:				DC->setCVar(item->cvar, buff);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2056
;2056:	    		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $963
line 2063
;2057:			}
;2058:
;2059:
;2060:			//
;2061:			// ignore any non printable chars
;2062:			//
;2063:			if ( key < 32 || !item->cvar) {
ADDRFP4 4
INDIRI4
CNSTI4 32
LTI4 $972
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $970
LABELV $972
line 2064
;2064:			    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $970
line 2067
;2065:		    }
;2066:
;2067:			if (item->type == ITEM_TYPE_NUMERICFIELD) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 9
NEI4 $973
line 2068
;2068:				if (key < '0' || key > '9') {
ADDRLP4 1044
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 48
LTI4 $977
ADDRLP4 1044
INDIRI4
CNSTI4 57
LEI4 $975
LABELV $977
line 2069
;2069:					return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $956
JUMPV
LABELV $975
line 2071
;2070:				}
;2071:			}
LABELV $973
line 2073
;2072:
;2073:			if (!DC->getOverstrikeMode()) {
ADDRLP4 1044
ADDRGP4 DC
INDIRP4
CNSTI4 108
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $978
line 2074
;2074:				if (( len == MAX_EDITFIELD - 1 ) || (editPtr->maxChars && len >= editPtr->maxChars)) {
ADDRLP4 1048
ADDRLP4 1032
INDIRI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 255
EQI4 $982
ADDRLP4 1052
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
EQI4 $980
ADDRLP4 1048
INDIRI4
ADDRLP4 1052
INDIRI4
LTI4 $980
LABELV $982
line 2075
;2075:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $980
line 2077
;2076:				}
;2077:				memmove( &buff[item->cursorPos + 1], &buff[item->cursorPos], len + 1 - item->cursorPos );
ADDRLP4 1056
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1056
INDIRI4
ADDRLP4 4+1
ADDP4
ARGP4
ADDRLP4 1056
INDIRI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 1032
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 1056
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 2078
;2078:			} else {
ADDRGP4 $979
JUMPV
LABELV $978
line 2079
;2079:				if (editPtr->maxChars && item->cursorPos >= editPtr->maxChars) {
ADDRLP4 1048
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
EQI4 $984
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1048
INDIRI4
LTI4 $984
line 2080
;2080:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $984
line 2082
;2081:				}
;2082:			}
LABELV $979
line 2084
;2083:
;2084:			buff[item->cursorPos] = key;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 4
ADDP4
ADDRFP4 4
INDIRI4
CVII1 4
ASGNI1
line 2086
;2085:
;2086:			DC->setCVar(item->cvar, buff);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2088
;2087:
;2088:			if (item->cursorPos < len + 1) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1032
INDIRI4
CNSTI4 1
ADDI4
GEI4 $962
line 2089
;2089:				item->cursorPos++;
ADDRLP4 1048
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ASGNP4
ADDRLP4 1048
INDIRP4
ADDRLP4 1048
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2090
;2090:				if (editPtr->maxPaintChars && item->cursorPos > editPtr->maxPaintChars) {
ADDRLP4 1052
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
EQI4 $962
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1052
INDIRI4
LEI4 $962
line 2091
;2091:					editPtr->paintOffset++;
ADDRLP4 1056
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 1056
INDIRP4
ADDRLP4 1056
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2092
;2092:				}
line 2093
;2093:			}
line 2095
;2094:
;2095:		} else {
ADDRGP4 $962
JUMPV
LABELV $961
line 2097
;2096:
;2097:			if ( key == K_DEL || key == K_KP_DEL ) {
ADDRLP4 1044
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 140
EQI4 $992
ADDRLP4 1044
INDIRI4
CNSTI4 171
NEI4 $990
LABELV $992
line 2098
;2098:				if ( item->cursorPos < len ) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1032
INDIRI4
GEI4 $993
line 2099
;2099:					memmove( buff + item->cursorPos, buff + item->cursorPos + 1, len - item->cursorPos);
ADDRLP4 1048
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1048
INDIRI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 1048
INDIRI4
ADDRLP4 4+1
ADDP4
ARGP4
ADDRLP4 1032
INDIRI4
ADDRLP4 1048
INDIRI4
SUBI4
ARGI4
ADDRGP4 memmove
CALLP4
pop
line 2100
;2100:					DC->setCVar(item->cvar, buff);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2101
;2101:				}
LABELV $993
line 2102
;2102:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $990
line 2105
;2103:			}
;2104:
;2105:			if ( key == K_RIGHTARROW || key == K_KP_RIGHTARROW ) 
ADDRLP4 1048
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 135
EQI4 $998
ADDRLP4 1048
INDIRI4
CNSTI4 165
NEI4 $996
LABELV $998
line 2106
;2106:			{
line 2107
;2107:				if (editPtr->maxPaintChars && item->cursorPos >= editPtr->maxPaintChars && item->cursorPos < len) {
ADDRLP4 1052
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
EQI4 $999
ADDRLP4 1056
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1056
INDIRI4
ADDRLP4 1052
INDIRI4
LTI4 $999
ADDRLP4 1056
INDIRI4
ADDRLP4 1032
INDIRI4
GEI4 $999
line 2108
;2108:					item->cursorPos++;
ADDRLP4 1060
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ASGNP4
ADDRLP4 1060
INDIRP4
ADDRLP4 1060
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2109
;2109:					editPtr->paintOffset++;
ADDRLP4 1064
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 1064
INDIRP4
ADDRLP4 1064
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2110
;2110:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $999
line 2112
;2111:				}
;2112:				if (item->cursorPos < len) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1032
INDIRI4
GEI4 $1001
line 2113
;2113:					item->cursorPos++;
ADDRLP4 1060
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ASGNP4
ADDRLP4 1060
INDIRP4
ADDRLP4 1060
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2114
;2114:				} 
LABELV $1001
line 2115
;2115:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $996
line 2118
;2116:			}
;2117:
;2118:			if ( key == K_LEFTARROW || key == K_KP_LEFTARROW ) 
ADDRLP4 1052
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 134
EQI4 $1005
ADDRLP4 1052
INDIRI4
CNSTI4 163
NEI4 $1003
LABELV $1005
line 2119
;2119:			{
line 2120
;2120:				if ( item->cursorPos > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
LEI4 $1006
line 2121
;2121:					item->cursorPos--;
ADDRLP4 1056
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ASGNP4
ADDRLP4 1056
INDIRP4
ADDRLP4 1056
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2122
;2122:				}
LABELV $1006
line 2123
;2123:				if (item->cursorPos < editPtr->paintOffset) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
GEI4 $1008
line 2124
;2124:					editPtr->paintOffset--;
ADDRLP4 1056
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 1056
INDIRP4
ADDRLP4 1056
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2125
;2125:				}
LABELV $1008
line 2126
;2126:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $1003
line 2129
;2127:			}
;2128:
;2129:			if ( key == K_HOME || key == K_KP_HOME) {// || ( tolower(key) == 'a' && trap_Key_IsDown( K_CTRL ) ) ) {
ADDRLP4 1056
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 143
EQI4 $1012
ADDRLP4 1056
INDIRI4
CNSTI4 160
NEI4 $1010
LABELV $1012
line 2130
;2130:				item->cursorPos = 0;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 0
ASGNI4
line 2131
;2131:				editPtr->paintOffset = 0;
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
CNSTI4 0
ASGNI4
line 2132
;2132:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $1010
line 2135
;2133:			}
;2134:
;2135:			if ( key == K_END || key == K_KP_END)  {// ( tolower(key) == 'e' && trap_Key_IsDown( K_CTRL ) ) ) {
ADDRLP4 1060
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 144
EQI4 $1015
ADDRLP4 1060
INDIRI4
CNSTI4 166
NEI4 $1013
LABELV $1015
line 2136
;2136:				item->cursorPos = len;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
ADDRLP4 1032
INDIRI4
ASGNI4
line 2137
;2137:				if(item->cursorPos > editPtr->maxPaintChars) {
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
LEI4 $1016
line 2138
;2138:					editPtr->paintOffset = len - editPtr->maxPaintChars;
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 1032
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
SUBI4
ASGNI4
line 2139
;2139:				}
LABELV $1016
line 2140
;2140:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $1013
line 2143
;2141:			}
;2142:
;2143:			if ( key == K_INS || key == K_KP_INS ) {
ADDRLP4 1064
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 139
EQI4 $1020
ADDRLP4 1064
INDIRI4
CNSTI4 170
NEI4 $1018
LABELV $1020
line 2144
;2144:				DC->setOverstrikeMode(!DC->getOverstrikeMode());
ADDRLP4 1072
ADDRGP4 DC
INDIRP4
CNSTI4 108
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1072
INDIRI4
CNSTI4 0
NEI4 $1022
ADDRLP4 1068
CNSTI4 1
ASGNI4
ADDRGP4 $1023
JUMPV
LABELV $1022
ADDRLP4 1068
CNSTI4 0
ASGNI4
LABELV $1023
ADDRLP4 1068
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 104
ADDP4
INDIRP4
CALLV
pop
line 2145
;2145:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $1018
line 2147
;2146:			}
;2147:		}
LABELV $962
line 2149
;2148:
;2149:		if (key == K_TAB || key == K_DOWNARROW || key == K_KP_DOWNARROW) {
ADDRLP4 1044
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 9
EQI4 $1027
ADDRLP4 1044
INDIRI4
CNSTI4 133
EQI4 $1027
ADDRLP4 1044
INDIRI4
CNSTI4 167
NEI4 $1024
LABELV $1027
line 2150
;2150:			newItem = Menu_SetNextCursorItem(item->parent);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 1048
ADDRGP4 Menu_SetNextCursorItem
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1048
INDIRP4
ASGNP4
line 2151
;2151:			if (newItem && (newItem->type == ITEM_TYPE_EDITFIELD || newItem->type == ITEM_TYPE_NUMERICFIELD)) {
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1028
ADDRLP4 1056
ADDRLP4 1028
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 4
EQI4 $1030
ADDRLP4 1056
INDIRI4
CNSTI4 9
NEI4 $1028
LABELV $1030
line 2152
;2152:				g_editItem = newItem;
ADDRGP4 g_editItem
ADDRLP4 1028
INDIRP4
ASGNP4
line 2153
;2153:			}
LABELV $1028
line 2154
;2154:		}
LABELV $1024
line 2156
;2155:
;2156:		if (key == K_UPARROW || key == K_KP_UPARROW) {
ADDRLP4 1048
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 132
EQI4 $1033
ADDRLP4 1048
INDIRI4
CNSTI4 161
NEI4 $1031
LABELV $1033
line 2157
;2157:			newItem = Menu_SetPrevCursorItem(item->parent);
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ARGP4
ADDRLP4 1052
ADDRGP4 Menu_SetPrevCursorItem
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1052
INDIRP4
ASGNP4
line 2158
;2158:			if (newItem && (newItem->type == ITEM_TYPE_EDITFIELD || newItem->type == ITEM_TYPE_NUMERICFIELD)) {
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1034
ADDRLP4 1060
ADDRLP4 1028
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 4
EQI4 $1036
ADDRLP4 1060
INDIRI4
CNSTI4 9
NEI4 $1034
LABELV $1036
line 2159
;2159:				g_editItem = newItem;
ADDRGP4 g_editItem
ADDRLP4 1028
INDIRP4
ASGNP4
line 2160
;2160:			}
LABELV $1034
line 2161
;2161:		}
LABELV $1031
line 2163
;2162:
;2163:		if ( key == K_ENTER || key == K_KP_ENTER || key == K_ESCAPE)  {
ADDRLP4 1052
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 13
EQI4 $1040
ADDRLP4 1052
INDIRI4
CNSTI4 169
EQI4 $1040
ADDRLP4 1052
INDIRI4
CNSTI4 27
NEI4 $1037
LABELV $1040
line 2164
;2164:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $956
JUMPV
LABELV $1037
line 2167
;2165:		}
;2166:
;2167:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $956
JUMPV
LABELV $957
line 2169
;2168:	}
;2169:	return qfalse;
CNSTI4 0
RETI4
LABELV $956
endproc Item_TextField_HandleKey 1076 12
proc Scroll_ListBox_AutoFunc 12 16
line 2173
;2170:
;2171:}
;2172:
;2173:static void Scroll_ListBox_AutoFunc(void *p) {
line 2174
;2174:	scrollInfo_t *si = (scrollInfo_t*)p;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 2175
;2175:	if (DC->realTime > si->nextScrollTime) { 
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
LEI4 $1042
line 2179
;2176:		// need to scroll which is done by simulating a click to the item
;2177:		// this is done a bit sideways as the autoscroll "knows" that the item is a listbox
;2178:		// so it calls it directly
;2179:		Item_ListBox_HandleKey(si->item, si->scrollKey, qtrue, qfalse);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 Item_ListBox_HandleKey
CALLI4
pop
line 2180
;2180:		si->nextScrollTime = DC->realTime + si->adjustValue; 
ADDRLP4 0
INDIRP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
ASGNI4
line 2181
;2181:	}
LABELV $1042
line 2183
;2182:
;2183:	if (DC->realTime > si->nextAdjustTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
LEI4 $1044
line 2184
;2184:		si->nextAdjustTime = DC->realTime + SCROLL_TIME_ADJUST;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 150
ADDI4
ASGNI4
line 2185
;2185:		if (si->adjustValue > SCROLL_TIME_FLOOR) {
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 20
LEI4 $1046
line 2186
;2186:			si->adjustValue -= SCROLL_TIME_ADJUSTOFFSET;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 40
SUBI4
ASGNI4
line 2187
;2187:		}
LABELV $1046
line 2188
;2188:	}
LABELV $1044
line 2189
;2189:}
LABELV $1041
endproc Scroll_ListBox_AutoFunc 12 16
proc Scroll_ListBox_ThumbFunc 40 16
line 2191
;2190:
;2191:static void Scroll_ListBox_ThumbFunc(void *p) {
line 2192
;2192:	scrollInfo_t *si = (scrollInfo_t*)p;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 2196
;2193:	rectDef_t r;
;2194:	int pos, max;
;2195:
;2196:	listBoxDef_t *listPtr = (listBoxDef_t*)si->item->typeData;
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 2197
;2197:	if (si->item->window.flags & WINDOW_HORIZONTAL) {
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1049
line 2198
;2198:		if (DC->cursorx == si->xStart) {
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
NEF4 $1051
line 2199
;2199:			return;
ADDRGP4 $1048
JUMPV
LABELV $1051
line 2201
;2200:		}
;2201:		r.x = si->item->window.rect.x + SCROLLBAR_SIZE + 1;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
INDIRF4
CNSTF4 1098907648
ADDF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2202
;2202:		r.y = si->item->window.rect.y + si->item->window.rect.h - SCROLLBAR_SIZE - 1;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4+4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 2203
;2203:		r.h = SCROLLBAR_SIZE;
ADDRLP4 4+12
CNSTF4 1098907648
ASGNF4
line 2204
;2204:		r.w = si->item->window.rect.w - (SCROLLBAR_SIZE*2) - 2;
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 2205
;2205:		max = Item_ListBox_MaxScroll(si->item);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 Item_ListBox_MaxScroll
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 36
INDIRI4
ASGNI4
line 2207
;2206:		//
;2207:		pos = (DC->cursorx - r.x - SCROLLBAR_SIZE/2) * max / (r.w - SCROLLBAR_SIZE);
ADDRLP4 20
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
SUBF4
CNSTF4 1090519040
SUBF4
ADDRLP4 24
INDIRI4
CVIF4 4
MULF4
ADDRLP4 4+8
INDIRF4
CNSTF4 1098907648
SUBF4
DIVF4
CVFI4 4
ASGNI4
line 2208
;2208:		if (pos < 0) {
ADDRLP4 20
INDIRI4
CNSTI4 0
GEI4 $1057
line 2209
;2209:			pos = 0;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 2210
;2210:		}
ADDRGP4 $1058
JUMPV
LABELV $1057
line 2211
;2211:		else if (pos > max) {
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $1059
line 2212
;2212:			pos = max;
ADDRLP4 20
ADDRLP4 24
INDIRI4
ASGNI4
line 2213
;2213:		}
LABELV $1059
LABELV $1058
line 2214
;2214:		listPtr->startPos = pos;
ADDRLP4 28
INDIRP4
ADDRLP4 20
INDIRI4
ASGNI4
line 2215
;2215:		si->xStart = DC->cursorx;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2216
;2216:	}
ADDRGP4 $1050
JUMPV
LABELV $1049
line 2217
;2217:	else if (DC->cursory != si->yStart) {
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
EQF4 $1061
line 2219
;2218:
;2219:		r.x = si->item->window.rect.x + si->item->window.rect.w - SCROLLBAR_SIZE - 1;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 2220
;2220:		r.y = si->item->window.rect.y + SCROLLBAR_SIZE + 1;
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1098907648
ADDF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2221
;2221:		r.h = si->item->window.rect.h - (SCROLLBAR_SIZE*2) - 2;
ADDRLP4 4+12
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 2222
;2222:		r.w = SCROLLBAR_SIZE;
ADDRLP4 4+8
CNSTF4 1098907648
ASGNF4
line 2223
;2223:		max = Item_ListBox_MaxScroll(si->item);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 Item_ListBox_MaxScroll
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 36
INDIRI4
ASGNI4
line 2225
;2224:		//
;2225:		pos = (DC->cursory - r.y - SCROLLBAR_SIZE/2) * max / (r.h - SCROLLBAR_SIZE);
ADDRLP4 20
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 4+4
INDIRF4
SUBF4
CNSTF4 1090519040
SUBF4
ADDRLP4 24
INDIRI4
CVIF4 4
MULF4
ADDRLP4 4+12
INDIRF4
CNSTF4 1098907648
SUBF4
DIVF4
CVFI4 4
ASGNI4
line 2226
;2226:		if (pos < 0) {
ADDRLP4 20
INDIRI4
CNSTI4 0
GEI4 $1068
line 2227
;2227:			pos = 0;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 2228
;2228:		}
ADDRGP4 $1069
JUMPV
LABELV $1068
line 2229
;2229:		else if (pos > max) {
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $1070
line 2230
;2230:			pos = max;
ADDRLP4 20
ADDRLP4 24
INDIRI4
ASGNI4
line 2231
;2231:		}
LABELV $1070
LABELV $1069
line 2232
;2232:		listPtr->startPos = pos;
ADDRLP4 28
INDIRP4
ADDRLP4 20
INDIRI4
ASGNI4
line 2233
;2233:		si->yStart = DC->cursory;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2234
;2234:	}
LABELV $1061
LABELV $1050
line 2236
;2235:
;2236:	if (DC->realTime > si->nextScrollTime) { 
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
LEI4 $1072
line 2240
;2237:		// need to scroll which is done by simulating a click to the item
;2238:		// this is done a bit sideways as the autoscroll "knows" that the item is a listbox
;2239:		// so it calls it directly
;2240:		Item_ListBox_HandleKey(si->item, si->scrollKey, qtrue, qfalse);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 Item_ListBox_HandleKey
CALLI4
pop
line 2241
;2241:		si->nextScrollTime = DC->realTime + si->adjustValue; 
ADDRLP4 0
INDIRP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
ASGNI4
line 2242
;2242:	}
LABELV $1072
line 2244
;2243:
;2244:	if (DC->realTime > si->nextAdjustTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
LEI4 $1074
line 2245
;2245:		si->nextAdjustTime = DC->realTime + SCROLL_TIME_ADJUST;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 150
ADDI4
ASGNI4
line 2246
;2246:		if (si->adjustValue > SCROLL_TIME_FLOOR) {
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 20
LEI4 $1076
line 2247
;2247:			si->adjustValue -= SCROLL_TIME_ADJUSTOFFSET;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 40
SUBI4
ASGNI4
line 2248
;2248:		}
LABELV $1076
line 2249
;2249:	}
LABELV $1074
line 2250
;2250:}
LABELV $1048
endproc Scroll_ListBox_ThumbFunc 40 16
proc Scroll_Slider_ThumbFunc 28 8
line 2252
;2251:
;2252:static void Scroll_Slider_ThumbFunc(void *p) {
line 2254
;2253:	float x, value, cursorx;
;2254:	scrollInfo_t *si = (scrollInfo_t*)p;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 2255
;2255:	editFieldDef_t *editDef = si->item->typeData;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 2257
;2256:
;2257:	if (si->item->text) {
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1079
line 2258
;2258:		x = si->item->textRect.x + si->item->textRect.w + 8;
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 20
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 2259
;2259:	} else {
ADDRGP4 $1080
JUMPV
LABELV $1079
line 2260
;2260:		x = si->item->window.rect.x;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
INDIRF4
ASGNF4
line 2261
;2261:	}
LABELV $1080
line 2263
;2262:
;2263:	cursorx = DC->cursorx;
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2265
;2264:
;2265:	if (cursorx < x) {
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
GEF4 $1081
line 2266
;2266:		cursorx = x;
ADDRLP4 12
ADDRLP4 8
INDIRF4
ASGNF4
line 2267
;2267:	} else if (cursorx > x + SLIDER_WIDTH) {
ADDRGP4 $1082
JUMPV
LABELV $1081
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
CNSTF4 1119879168
ADDF4
LEF4 $1083
line 2268
;2268:		cursorx = x + SLIDER_WIDTH;
ADDRLP4 12
ADDRLP4 8
INDIRF4
CNSTF4 1119879168
ADDF4
ASGNF4
line 2269
;2269:	}
LABELV $1083
LABELV $1082
line 2270
;2270:	value = cursorx - x;
ADDRLP4 4
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
SUBF4
ASGNF4
line 2271
;2271:	value /= SLIDER_WIDTH;
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1119879168
DIVF4
ASGNF4
line 2272
;2272:	value *= (editDef->maxVal - editDef->minVal);
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 16
INDIRP4
INDIRF4
SUBF4
MULF4
ASGNF4
line 2273
;2273:	value += editDef->minVal;
ADDRLP4 4
ADDRLP4 4
INDIRF4
ADDRLP4 16
INDIRP4
INDIRF4
ADDF4
ASGNF4
line 2274
;2274:	DC->setCVar(si->item->cvar, va("%f", value));
ADDRGP4 $955
ARGP4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2275
;2275:}
LABELV $1078
endproc Scroll_Slider_ThumbFunc 28 8
export Item_StartCapture
proc Item_StartCapture 24 12
line 2277
;2276:
;2277:void Item_StartCapture(itemDef_t *item, int key) {
line 2279
;2278:	int flags;
;2279:	switch (item->type) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 4
LTI4 $1086
ADDRLP4 4
INDIRI4
CNSTI4 10
GTI4 $1086
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1113-16
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1113
address $1089
address $1086
address $1089
address $1086
address $1086
address $1089
address $1106
code
LABELV $1089
line 2284
;2280:    case ITEM_TYPE_EDITFIELD:
;2281:    case ITEM_TYPE_NUMERICFIELD:
;2282:
;2283:		case ITEM_TYPE_LISTBOX:
;2284:		{
line 2285
;2285:			flags = Item_ListBox_OverLB(item, DC->cursorx, DC->cursory);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 16
ADDRGP4 Item_ListBox_OverLB
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 16
INDIRI4
ASGNI4
line 2286
;2286:			if (flags & (WINDOW_LB_LEFTARROW | WINDOW_LB_RIGHTARROW)) {
ADDRLP4 0
INDIRI4
CNSTI4 6144
BANDI4
CNSTI4 0
EQI4 $1090
line 2287
;2287:				scrollInfo.nextScrollTime = DC->realTime + SCROLL_TIME_START;
ADDRGP4 scrollInfo
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 2288
;2288:				scrollInfo.nextAdjustTime = DC->realTime + SCROLL_TIME_ADJUST;
ADDRGP4 scrollInfo+4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 150
ADDI4
ASGNI4
line 2289
;2289:				scrollInfo.adjustValue = SCROLL_TIME_START;
ADDRGP4 scrollInfo+8
CNSTI4 500
ASGNI4
line 2290
;2290:				scrollInfo.scrollKey = key;
ADDRGP4 scrollInfo+12
ADDRFP4 4
INDIRI4
ASGNI4
line 2291
;2291:				scrollInfo.scrollDir = (flags & WINDOW_LB_LEFTARROW) ? qtrue : qfalse;
ADDRLP4 0
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $1097
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRGP4 $1098
JUMPV
LABELV $1097
ADDRLP4 20
CNSTI4 0
ASGNI4
LABELV $1098
ADDRGP4 scrollInfo+28
ADDRLP4 20
INDIRI4
ASGNI4
line 2292
;2292:				scrollInfo.item = item;
ADDRGP4 scrollInfo+24
ADDRFP4 0
INDIRP4
ASGNP4
line 2293
;2293:				captureData = &scrollInfo;
ADDRGP4 captureData
ADDRGP4 scrollInfo
ASGNP4
line 2294
;2294:				captureFunc = &Scroll_ListBox_AutoFunc;
ADDRGP4 captureFunc
ADDRGP4 Scroll_ListBox_AutoFunc
ASGNP4
line 2295
;2295:				itemCapture = item;
ADDRGP4 itemCapture
ADDRFP4 0
INDIRP4
ASGNP4
line 2296
;2296:			} else if (flags & WINDOW_LB_THUMB) {
ADDRGP4 $1087
JUMPV
LABELV $1090
ADDRLP4 0
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $1087
line 2297
;2297:				scrollInfo.scrollKey = key;
ADDRGP4 scrollInfo+12
ADDRFP4 4
INDIRI4
ASGNI4
line 2298
;2298:				scrollInfo.item = item;
ADDRGP4 scrollInfo+24
ADDRFP4 0
INDIRP4
ASGNP4
line 2299
;2299:				scrollInfo.xStart = DC->cursorx;
ADDRGP4 scrollInfo+16
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2300
;2300:				scrollInfo.yStart = DC->cursory;
ADDRGP4 scrollInfo+20
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2301
;2301:				captureData = &scrollInfo;
ADDRGP4 captureData
ADDRGP4 scrollInfo
ASGNP4
line 2302
;2302:				captureFunc = &Scroll_ListBox_ThumbFunc;
ADDRGP4 captureFunc
ADDRGP4 Scroll_ListBox_ThumbFunc
ASGNP4
line 2303
;2303:				itemCapture = item;
ADDRGP4 itemCapture
ADDRFP4 0
INDIRP4
ASGNP4
line 2304
;2304:			}
line 2305
;2305:			break;
ADDRGP4 $1087
JUMPV
LABELV $1106
line 2308
;2306:		}
;2307:		case ITEM_TYPE_SLIDER:
;2308:		{
line 2309
;2309:			flags = Item_Slider_OverSlider(item, DC->cursorx, DC->cursory);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 16
ADDRGP4 Item_Slider_OverSlider
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 16
INDIRI4
ASGNI4
line 2310
;2310:			if (flags & WINDOW_LB_THUMB) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
BANDI4
CNSTI4 0
EQI4 $1087
line 2311
;2311:				scrollInfo.scrollKey = key;
ADDRGP4 scrollInfo+12
ADDRFP4 4
INDIRI4
ASGNI4
line 2312
;2312:				scrollInfo.item = item;
ADDRGP4 scrollInfo+24
ADDRFP4 0
INDIRP4
ASGNP4
line 2313
;2313:				scrollInfo.xStart = DC->cursorx;
ADDRGP4 scrollInfo+16
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2314
;2314:				scrollInfo.yStart = DC->cursory;
ADDRGP4 scrollInfo+20
ADDRGP4 DC
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2315
;2315:				captureData = &scrollInfo;
ADDRGP4 captureData
ADDRGP4 scrollInfo
ASGNP4
line 2316
;2316:				captureFunc = &Scroll_Slider_ThumbFunc;
ADDRGP4 captureFunc
ADDRGP4 Scroll_Slider_ThumbFunc
ASGNP4
line 2317
;2317:				itemCapture = item;
ADDRGP4 itemCapture
ADDRFP4 0
INDIRP4
ASGNP4
line 2318
;2318:			}
line 2319
;2319:			break;
LABELV $1086
LABELV $1087
line 2322
;2320:		}
;2321:	}
;2322:}
LABELV $1085
endproc Item_StartCapture 24 12
export Item_StopCapture
proc Item_StopCapture 0 0
line 2324
;2323:
;2324:void Item_StopCapture(itemDef_t *item) {
line 2326
;2325:
;2326:}
LABELV $1115
endproc Item_StopCapture 0 0
export Item_Slider_HandleKey
proc Item_Slider_HandleKey 68 12
line 2328
;2327:
;2328:qboolean Item_Slider_HandleKey(itemDef_t *item, int key, qboolean down) {
line 2332
;2329:	float x, value, width, work;
;2330:
;2331:	//DC->Print("slider handle key\n");
;2332:	if (item->window.flags & WINDOW_HASFOCUS && item->cvar && Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory)) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1117
ADDRLP4 16
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1117
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $1117
line 2333
;2333:		if (key == K_MOUSE1 || key == K_ENTER || key == K_MOUSE2 || key == K_MOUSE3) {
ADDRLP4 28
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 178
EQI4 $1123
ADDRLP4 28
INDIRI4
CNSTI4 13
EQI4 $1123
ADDRLP4 28
INDIRI4
CNSTI4 179
EQI4 $1123
ADDRLP4 28
INDIRI4
CNSTI4 180
NEI4 $1119
LABELV $1123
line 2334
;2334:			editFieldDef_t *editDef = item->typeData;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 2335
;2335:			if (editDef) {
ADDRLP4 32
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1124
line 2337
;2336:				rectDef_t testRect;
;2337:				width = SLIDER_WIDTH;
ADDRLP4 8
CNSTF4 1119879168
ASGNF4
line 2338
;2338:				if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1126
line 2339
;2339:					x = item->textRect.x + item->textRect.w + 8;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 2340
;2340:				} else {
ADDRGP4 $1127
JUMPV
LABELV $1126
line 2341
;2341:					x = item->window.rect.x;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 2342
;2342:				}
LABELV $1127
line 2344
;2343:
;2344:				testRect = item->window.rect;
ADDRLP4 36
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 16
line 2345
;2345:				testRect.x = x;
ADDRLP4 36
ADDRLP4 4
INDIRF4
ASGNF4
line 2346
;2346:				value = (float)SLIDER_THUMB_WIDTH / 2;
ADDRLP4 0
CNSTF4 1086324736
ASGNF4
line 2347
;2347:				testRect.x -= value;
ADDRLP4 36
ADDRLP4 36
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
line 2349
;2348:				//DC->Print("slider x: %f\n", testRect.x);
;2349:				testRect.w = (SLIDER_WIDTH + (float)SLIDER_THUMB_WIDTH / 2);
ADDRLP4 36+8
CNSTF4 1120665600
ASGNF4
line 2351
;2350:				//DC->Print("slider w: %f\n", testRect.w);
;2351:				if (Rect_ContainsPoint(&testRect, DC->cursorx, DC->cursory)) {
ADDRLP4 36
ARGP4
ADDRLP4 52
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 56
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
EQI4 $1129
line 2352
;2352:					work = DC->cursorx - x;
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
SUBF4
ASGNF4
line 2353
;2353:					value = work / width;
ADDRLP4 0
ADDRLP4 12
INDIRF4
ADDRLP4 8
INDIRF4
DIVF4
ASGNF4
line 2354
;2354:					value *= (editDef->maxVal - editDef->minVal);
ADDRLP4 60
ADDRLP4 32
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
INDIRF4
SUBF4
MULF4
ASGNF4
line 2357
;2355:					// vm fuckage
;2356:					// value = (((float)(DC->cursorx - x)/ SLIDER_WIDTH) * (editDef->maxVal - editDef->minVal));
;2357:					value += editDef->minVal;
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 32
INDIRP4
INDIRF4
ADDF4
ASGNF4
line 2358
;2358:					DC->setCVar(item->cvar, va("%f", value));
ADDRGP4 $955
ARGP4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 64
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 96
ADDP4
INDIRP4
CALLV
pop
line 2359
;2359:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1116
JUMPV
LABELV $1129
line 2361
;2360:				}
;2361:			}
LABELV $1124
line 2362
;2362:		}
LABELV $1119
line 2363
;2363:	}
LABELV $1117
line 2364
;2364:	DC->Print("slider handle key exit\n");
ADDRGP4 $1131
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 156
ADDP4
INDIRP4
CALLV
pop
line 2365
;2365:	return qfalse;
CNSTI4 0
RETI4
LABELV $1116
endproc Item_Slider_HandleKey 68 12
export Item_HandleKey
proc Item_HandleKey 32 16
line 2369
;2366:}
;2367:
;2368:
;2369:qboolean Item_HandleKey(itemDef_t *item, int key, qboolean down) {
line 2371
;2370:
;2371:	if (itemCapture) {
ADDRGP4 itemCapture
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1133
line 2372
;2372:		Item_StopCapture(itemCapture);
ADDRGP4 itemCapture
INDIRP4
ARGP4
ADDRGP4 Item_StopCapture
CALLV
pop
line 2373
;2373:		itemCapture = NULL;
ADDRGP4 itemCapture
CNSTP4 0
ASGNP4
line 2374
;2374:		captureFunc = NULL;
ADDRGP4 captureFunc
CNSTP4 0
ASGNP4
line 2375
;2375:		captureData = NULL;
ADDRGP4 captureData
CNSTP4 0
ASGNP4
line 2376
;2376:	} else {
ADDRGP4 $1134
JUMPV
LABELV $1133
line 2378
;2377:	  // bk001206 - parentheses
;2378:		if ( down && ( key == K_MOUSE1 || key == K_MOUSE2 || key == K_MOUSE3 ) ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $1135
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $1138
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $1138
ADDRLP4 0
INDIRI4
CNSTI4 180
NEI4 $1135
LABELV $1138
line 2379
;2379:			Item_StartCapture(item, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Item_StartCapture
CALLV
pop
line 2380
;2380:		}
LABELV $1135
line 2381
;2381:	}
LABELV $1134
line 2383
;2382:
;2383:	if (!down) {
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $1139
line 2384
;2384:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
LABELV $1139
line 2387
;2385:	}
;2386:
;2387:  switch (item->type) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1
LTI4 $1141
ADDRLP4 0
INDIRI4
CNSTI4 13
GTI4 $1141
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1155-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1155
address $1144
address $1145
address $1146
address $1147
address $1148
address $1149
address $1141
address $1152
address $1147
address $1154
address $1150
address $1151
address $1153
code
LABELV $1144
line 2389
;2388:    case ITEM_TYPE_BUTTON:
;2389:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
line 2390
;2390:      break;
LABELV $1145
line 2392
;2391:    case ITEM_TYPE_RADIOBUTTON:
;2392:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
line 2393
;2393:      break;
LABELV $1146
line 2395
;2394:    case ITEM_TYPE_CHECKBOX:
;2395:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
line 2396
;2396:      break;
LABELV $1147
line 2400
;2397:    case ITEM_TYPE_EDITFIELD:
;2398:    case ITEM_TYPE_NUMERICFIELD:
;2399:      //return Item_TextField_HandleKey(item, key);
;2400:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
line 2401
;2401:      break;
LABELV $1148
line 2403
;2402:    case ITEM_TYPE_COMBO:
;2403:      return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $1132
JUMPV
line 2404
;2404:      break;
LABELV $1149
line 2406
;2405:    case ITEM_TYPE_LISTBOX:
;2406:      return Item_ListBox_HandleKey(item, key, down, qfalse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 Item_ListBox_HandleKey
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2407
;2407:      break;
LABELV $1150
line 2409
;2408:    case ITEM_TYPE_YESNO:
;2409:      return Item_YesNo_HandleKey(item, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 Item_YesNo_HandleKey
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2410
;2410:      break;
LABELV $1151
line 2412
;2411:    case ITEM_TYPE_MULTI:
;2412:      return Item_Multi_HandleKey(item, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 Item_Multi_HandleKey
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2413
;2413:      break;
LABELV $1152
line 2415
;2414:    case ITEM_TYPE_OWNERDRAW:
;2415:      return Item_OwnerDraw_HandleKey(item, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 Item_OwnerDraw_HandleKey
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2416
;2416:      break;
LABELV $1153
line 2418
;2417:    case ITEM_TYPE_BIND:
;2418:			return Item_Bind_HandleKey(item, key, down);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 Item_Bind_HandleKey
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2419
;2419:      break;
LABELV $1154
line 2421
;2420:    case ITEM_TYPE_SLIDER:
;2421:      return Item_Slider_HandleKey(item, key, down);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 Item_Slider_HandleKey
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
RETI4
ADDRGP4 $1132
JUMPV
line 2422
;2422:      break;
LABELV $1141
line 2427
;2423:    //case ITEM_TYPE_IMAGE:
;2424:    //  Item_Image_Paint(item);
;2425:    //  break;
;2426:    default:
;2427:      return qfalse;
CNSTI4 0
RETI4
line 2428
;2428:      break;
LABELV $1132
endproc Item_HandleKey 32 16
export Item_Action
proc Item_Action 4 8
line 2434
;2429:  }
;2430:
;2431:  //return qfalse;
;2432:}
;2433:
;2434:void Item_Action(itemDef_t *item) {
line 2435
;2435:  if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1158
line 2436
;2436:    Item_RunScript(item, item->action);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 252
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 2437
;2437:  }
LABELV $1158
line 2438
;2438:}
LABELV $1157
endproc Item_Action 4 8
export Menu_SetPrevCursorItem
proc Menu_SetPrevCursorItem 44 12
line 2440
;2439:
;2440:itemDef_t *Menu_SetPrevCursorItem(menuDef_t *menu) {
line 2441
;2441:  qboolean wrapped = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2442
;2442:	int oldCursor = menu->cursorItem;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
line 2444
;2443:  
;2444:  if (menu->cursorItem < 0) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 0
GEI4 $1164
line 2445
;2445:    menu->cursorItem = menu->itemCount-1;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2446
;2446:    wrapped = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 2447
;2447:  } 
ADDRGP4 $1164
JUMPV
LABELV $1163
line 2449
;2448:
;2449:  while (menu->cursorItem > -1) {
line 2451
;2450:    
;2451:    menu->cursorItem--;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2452
;2452:    if (menu->cursorItem < 0 && !wrapped) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
GEI4 $1166
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $1166
line 2453
;2453:      wrapped = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 2454
;2454:      menu->cursorItem = menu->itemCount -1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2455
;2455:    }
LABELV $1166
line 2457
;2456:
;2457:		if (Item_SetFocus(menu->items[menu->cursorItem], DC->cursorx, DC->cursory)) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Item_SetFocus
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $1168
line 2458
;2458:			Menu_HandleMouseMove(menu, menu->items[menu->cursorItem]->window.rect.x + 1, menu->items[menu->cursorItem]->window.rect.y + 1);
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
ADDRLP4 28
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 28
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTF4 1065353216
ASGNF4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ARGF4
ADDRGP4 Menu_HandleMouseMove
CALLV
pop
line 2459
;2459:      return menu->items[menu->cursorItem];
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 40
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $1160
JUMPV
LABELV $1168
line 2461
;2460:    }
;2461:  }
LABELV $1164
line 2449
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 -1
GTI4 $1163
line 2462
;2462:	menu->cursorItem = oldCursor;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 2463
;2463:	return NULL;
CNSTP4 0
RETP4
LABELV $1160
endproc Menu_SetPrevCursorItem 44 12
export Menu_SetNextCursorItem
proc Menu_SetNextCursorItem 44 12
line 2467
;2464:
;2465:}
;2466:
;2467:itemDef_t *Menu_SetNextCursorItem(menuDef_t *menu) {
line 2469
;2468:
;2469:  qboolean wrapped = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 2470
;2470:	int oldCursor = menu->cursorItem;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
line 2473
;2471:
;2472:
;2473:  if (menu->cursorItem == -1) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1174
line 2474
;2474:    menu->cursorItem = 0;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
CNSTI4 0
ASGNI4
line 2475
;2475:    wrapped = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 2476
;2476:  }
ADDRGP4 $1174
JUMPV
LABELV $1173
line 2478
;2477:
;2478:  while (menu->cursorItem < menu->itemCount) {
line 2480
;2479:
;2480:    menu->cursorItem++;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2481
;2481:    if (menu->cursorItem >= menu->itemCount && !wrapped) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1176
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $1176
line 2482
;2482:      wrapped = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 2483
;2483:      menu->cursorItem = 0;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
CNSTI4 0
ASGNI4
line 2484
;2484:    }
LABELV $1176
line 2485
;2485:		if (Item_SetFocus(menu->items[menu->cursorItem], DC->cursorx, DC->cursory)) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 16
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Item_SetFocus
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $1178
line 2486
;2486:			Menu_HandleMouseMove(menu, menu->items[menu->cursorItem]->window.rect.x + 1, menu->items[menu->cursorItem]->window.rect.y + 1);
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 32
ADDRLP4 28
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 28
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTF4 1065353216
ASGNF4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 36
INDIRF4
ADDF4
ARGF4
ADDRGP4 Menu_HandleMouseMove
CALLV
pop
line 2487
;2487:      return menu->items[menu->cursorItem];
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 40
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $1170
JUMPV
LABELV $1178
line 2490
;2488:    }
;2489:    
;2490:  }
LABELV $1174
line 2478
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1173
line 2492
;2491:
;2492:	menu->cursorItem = oldCursor;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 2493
;2493:	return NULL;
CNSTP4 0
RETP4
LABELV $1170
endproc Menu_SetNextCursorItem 44 12
proc Window_CloseCinematic 4 4
line 2496
;2494:}
;2495:
;2496:static void Window_CloseCinematic(windowDef_t *window) {
line 2497
;2497:	if (window->style == WINDOW_STYLE_CINEMATIC && window->cinematic >= 0) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 5
NEI4 $1181
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LTI4 $1181
line 2498
;2498:		DC->stopCinematic(window->cinematic);
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 184
ADDP4
INDIRP4
CALLV
pop
line 2499
;2499:		window->cinematic = -1;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 -1
ASGNI4
line 2500
;2500:	}
LABELV $1181
line 2501
;2501:}
LABELV $1180
endproc Window_CloseCinematic 4 4
proc Menu_CloseCinematics 4 4
line 2503
;2502:
;2503:static void Menu_CloseCinematics(menuDef_t *menu) {
line 2504
;2504:	if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1184
line 2506
;2505:		int i;
;2506:		Window_CloseCinematic(&menu->window);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Window_CloseCinematic
CALLV
pop
line 2507
;2507:	  for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1189
JUMPV
LABELV $1186
line 2508
;2508:		  Window_CloseCinematic(&menu->items[i]->window);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Window_CloseCinematic
CALLV
pop
line 2509
;2509:			if (menu->items[i]->type == ITEM_TYPE_OWNERDRAW) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 8
NEI4 $1190
line 2510
;2510:				DC->stopCinematic(0-menu->items[i]->window.ownerDraw);
CNSTI4 0
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
SUBI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 184
ADDP4
INDIRP4
CALLV
pop
line 2511
;2511:			}
LABELV $1190
line 2512
;2512:	  }
LABELV $1187
line 2507
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1189
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1186
line 2513
;2513:	}
LABELV $1184
line 2514
;2514:}
LABELV $1183
endproc Menu_CloseCinematics 4 4
proc Display_CloseCinematics 4 4
line 2516
;2515:
;2516:static void Display_CloseCinematics() {
line 2518
;2517:	int i;
;2518:	for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1196
JUMPV
LABELV $1193
line 2519
;2519:		Menu_CloseCinematics(&Menus[i]);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRGP4 Menu_CloseCinematics
CALLV
pop
line 2520
;2520:	}
LABELV $1194
line 2518
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1196
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1193
line 2521
;2521:}
LABELV $1192
endproc Display_CloseCinematics 4 4
export Menus_Activate
proc Menus_Activate 544 8
line 2523
;2522:
;2523:void  Menus_Activate(menuDef_t *menu) {
line 2524
;2524:	menu->window.flags |= (WINDOW_HASFOCUS | WINDOW_VISIBLE);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 6
BORI4
ASGNI4
line 2525
;2525:	if (menu->onOpen) {
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1198
line 2527
;2526:		itemDef_t item;
;2527:		item.parent = menu;
ADDRLP4 4+228
ADDRFP4 0
INDIRP4
ASGNP4
line 2528
;2528:		Item_RunScript(&item, menu->onOpen);
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 2529
;2529:	}
LABELV $1198
line 2531
;2530:
;2531:	if (menu->soundName && *menu->soundName) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1201
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $1201
line 2533
;2532://		DC->stopBackgroundTrack();					// you don't want to do this since it will reset s_rawend
;2533:		DC->startBackgroundTrack(menu->soundName, menu->soundName);
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 172
ADDP4
INDIRP4
CALLV
pop
line 2534
;2534:	}
LABELV $1201
line 2536
;2535:
;2536:	Display_CloseCinematics();
ADDRGP4 Display_CloseCinematics
CALLV
pop
line 2538
;2537:
;2538:}
LABELV $1197
endproc Menus_Activate 544 8
export Display_VisibleMenuCount
proc Display_VisibleMenuCount 8 0
line 2540
;2539:
;2540:int Display_VisibleMenuCount() {
line 2542
;2541:	int i, count;
;2542:	count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2543
;2543:	for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1207
JUMPV
LABELV $1204
line 2544
;2544:		if (Menus[i].window.flags & (WINDOW_FORCED | WINDOW_VISIBLE)) {
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+68
ADDP4
INDIRI4
CNSTI4 1048580
BANDI4
CNSTI4 0
EQI4 $1208
line 2545
;2545:			count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2546
;2546:		}
LABELV $1208
line 2547
;2547:	}
LABELV $1205
line 2543
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1207
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1204
line 2548
;2548:	return count;
ADDRLP4 4
INDIRI4
RETI4
LABELV $1203
endproc Display_VisibleMenuCount 8 0
export Menus_HandleOOBClick
proc Menus_HandleOOBClick 24 12
line 2551
;2549:}
;2550:
;2551:void Menus_HandleOOBClick(menuDef_t *menu, int key, qboolean down) {
line 2552
;2552:	if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1212
line 2557
;2553:		int i;
;2554:		// basically the behaviour we are looking for is if there are windows in the stack.. see if 
;2555:		// the cursor is within any of them.. if not close them otherwise activate them and pass the 
;2556:		// key on.. force a mouse move to activate focus and script stuff 
;2557:		if (down && menu->window.flags & WINDOW_OOB_CLICK) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $1214
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 131072
BANDI4
ADDRLP4 4
INDIRI4
EQI4 $1214
line 2558
;2558:			Menu_RunCloseScript(menu);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_RunCloseScript
CALLV
pop
line 2559
;2559:			menu->window.flags &= ~(WINDOW_HASFOCUS | WINDOW_VISIBLE);
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -7
BANDI4
ASGNI4
line 2560
;2560:		}
LABELV $1214
line 2562
;2561:
;2562:		for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1219
JUMPV
LABELV $1216
line 2563
;2563:			if (Menu_OverActiveItem(&Menus[i], DC->cursorx, DC->cursory)) {
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
ADDRGP4 Menu_OverActiveItem
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $1220
line 2564
;2564:				Menu_RunCloseScript(menu);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_RunCloseScript
CALLV
pop
line 2565
;2565:				menu->window.flags &= ~(WINDOW_HASFOCUS | WINDOW_VISIBLE);
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 -7
BANDI4
ASGNI4
line 2566
;2566:				Menus_Activate(&Menus[i]);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRGP4 Menus_Activate
CALLV
pop
line 2567
;2567:				Menu_HandleMouseMove(&Menus[i], DC->cursorx, DC->cursory);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 Menu_HandleMouseMove
CALLV
pop
line 2568
;2568:				Menu_HandleKey(&Menus[i], key, down);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Menu_HandleKey
CALLV
pop
line 2569
;2569:			}
LABELV $1220
line 2570
;2570:		}
LABELV $1217
line 2562
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1219
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1216
line 2572
;2571:
;2572:		if (Display_VisibleMenuCount() == 0) {
ADDRLP4 8
ADDRGP4 Display_VisibleMenuCount
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $1222
line 2573
;2573:			if (DC->Pause) {
ADDRGP4 DC
INDIRP4
CNSTI4 160
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1224
line 2574
;2574:				DC->Pause(qfalse);
CNSTI4 0
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 160
ADDP4
INDIRP4
CALLV
pop
line 2575
;2575:			}
LABELV $1224
line 2576
;2576:		}
LABELV $1222
line 2577
;2577:		Display_CloseCinematics();
ADDRGP4 Display_CloseCinematics
CALLV
pop
line 2578
;2578:	}
LABELV $1212
line 2579
;2579:}
LABELV $1211
endproc Menus_HandleOOBClick 24 12
bss
align 4
LABELV $1227
skip 16
code
proc Item_CorrectedTextRect 4 12
line 2581
;2580:
;2581:static rectDef_t *Item_CorrectedTextRect(itemDef_t *item) {
line 2583
;2582:	static rectDef_t rect;
;2583:	memset(&rect, 0, sizeof(rectDef_t));
ADDRGP4 $1227
ARGP4
CNSTI4 0
ARGI4
CNSTI4 16
ARGI4
ADDRGP4 memset
CALLP4
pop
line 2584
;2584:	if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1228
line 2585
;2585:		rect = item->textRect;
ADDRGP4 $1227
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRB
ASGNB 16
line 2586
;2586:		if (rect.w) {
ADDRGP4 $1227+8
INDIRF4
CNSTF4 0
EQF4 $1230
line 2587
;2587:			rect.y -= rect.h;
ADDRLP4 0
ADDRGP4 $1227+4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRGP4 $1227+12
INDIRF4
SUBF4
ASGNF4
line 2588
;2588:		}
LABELV $1230
line 2589
;2589:	}
LABELV $1228
line 2590
;2590:	return &rect;
ADDRGP4 $1227
RETP4
LABELV $1226
endproc Item_CorrectedTextRect 4 12
data
align 4
LABELV $1256
byte 4 0
export Menu_HandleKey
code
proc Menu_HandleKey 628 12
line 2593
;2591:}
;2592:
;2593:void Menu_HandleKey(menuDef_t *menu, int key, qboolean down) {
line 2595
;2594:	int i;
;2595:	itemDef_t *item = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 2596
;2596:	qboolean inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2598
;2597:
;2598:	if (inHandler) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $1236
line 2599
;2599:		return;
ADDRGP4 $1235
JUMPV
LABELV $1236
line 2602
;2600:	}
;2601:
;2602:	inHandler = qtrue;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 2603
;2603:	if (g_waitingForKey && down) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 g_waitingForKey
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $1238
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $1238
line 2604
;2604:		Item_Bind_HandleKey(g_bindItem, key, down);
ADDRGP4 g_bindItem
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Item_Bind_HandleKey
CALLI4
pop
line 2605
;2605:		inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2606
;2606:		return;
ADDRGP4 $1235
JUMPV
LABELV $1238
line 2609
;2607:	}
;2608:
;2609:	if (g_editingField && down) {
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 g_editingField
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $1240
ADDRFP4 8
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $1240
line 2610
;2610:		if (!Item_TextField_HandleKey(g_editItem, key)) {
ADDRGP4 g_editItem
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 Item_TextField_HandleKey
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $1242
line 2611
;2611:			g_editingField = qfalse;
ADDRGP4 g_editingField
CNSTI4 0
ASGNI4
line 2612
;2612:			g_editItem = NULL;
ADDRGP4 g_editItem
CNSTP4 0
ASGNP4
line 2613
;2613:			inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2614
;2614:			return;
ADDRGP4 $1235
JUMPV
LABELV $1242
line 2615
;2615:		} else if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_MOUSE3) {
ADDRLP4 24
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 178
EQI4 $1247
ADDRLP4 24
INDIRI4
CNSTI4 179
EQI4 $1247
ADDRLP4 24
INDIRI4
CNSTI4 180
NEI4 $1244
LABELV $1247
line 2616
;2616:			g_editingField = qfalse;
ADDRGP4 g_editingField
CNSTI4 0
ASGNI4
line 2617
;2617:			g_editItem = NULL;
ADDRGP4 g_editItem
CNSTP4 0
ASGNP4
line 2618
;2618:			Display_MouseMove(NULL, DC->cursorx, DC->cursory);
CNSTP4 0
ARGP4
ADDRLP4 28
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 Display_MouseMove
CALLI4
pop
line 2619
;2619:		} else if (key == K_TAB || key == K_UPARROW || key == K_DOWNARROW) {
ADDRGP4 $1245
JUMPV
LABELV $1244
ADDRLP4 28
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 9
EQI4 $1251
ADDRLP4 28
INDIRI4
CNSTI4 132
EQI4 $1251
ADDRLP4 28
INDIRI4
CNSTI4 133
NEI4 $1248
LABELV $1251
line 2620
;2620:			return;
ADDRGP4 $1235
JUMPV
LABELV $1248
LABELV $1245
line 2622
;2621:		}
;2622:	}
LABELV $1240
line 2624
;2623:
;2624:	if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1252
line 2625
;2625:		inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2626
;2626:		return;
ADDRGP4 $1235
JUMPV
LABELV $1252
line 2630
;2627:	}
;2628:
;2629:		// see if the mouse is within the window bounds and if so is this a mouse click
;2630:	if (down && !(menu->window.flags & WINDOW_POPUP) && !Rect_ContainsPoint(&menu->window.rect, DC->cursorx, DC->cursory)) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $1254
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2097152
BANDI4
ADDRLP4 20
INDIRI4
NEI4 $1254
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 28
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 32
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $1254
line 2633
;2631:		static qboolean inHandleKey = qfalse;
;2632:		// bk001206 - parentheses
;2633:		if (!inHandleKey && ( key == K_MOUSE1 || key == K_MOUSE2 || key == K_MOUSE3 ) ) {
ADDRGP4 $1256
INDIRI4
CNSTI4 0
NEI4 $1257
ADDRLP4 36
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 178
EQI4 $1260
ADDRLP4 36
INDIRI4
CNSTI4 179
EQI4 $1260
ADDRLP4 36
INDIRI4
CNSTI4 180
NEI4 $1257
LABELV $1260
line 2634
;2634:			inHandleKey = qtrue;
ADDRGP4 $1256
CNSTI4 1
ASGNI4
line 2635
;2635:			Menus_HandleOOBClick(menu, key, down);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Menus_HandleOOBClick
CALLV
pop
line 2636
;2636:			inHandleKey = qfalse;
ADDRGP4 $1256
CNSTI4 0
ASGNI4
line 2637
;2637:			inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2638
;2638:			return;
ADDRGP4 $1235
JUMPV
LABELV $1257
line 2640
;2639:		}
;2640:	}
LABELV $1254
line 2643
;2641:
;2642:	// get the item with focus
;2643:	for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1264
JUMPV
LABELV $1261
line 2644
;2644:		if (menu->items[i]->window.flags & WINDOW_HASFOCUS) {
ADDRLP4 36
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 36
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ADDRLP4 36
INDIRI4
BANDI4
CNSTI4 0
EQI4 $1265
line 2645
;2645:			item = menu->items[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
line 2646
;2646:		}
LABELV $1265
line 2647
;2647:	}
LABELV $1262
line 2643
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1264
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1261
line 2649
;2648:
;2649:	if (item != NULL) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1267
line 2650
;2650:		if (Item_HandleKey(item, key, down)) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 36
ADDRGP4 Item_HandleKey
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $1269
line 2651
;2651:			Item_Action(item);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Item_Action
CALLV
pop
line 2652
;2652:			inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2653
;2653:			return;
ADDRGP4 $1235
JUMPV
LABELV $1269
line 2655
;2654:		}
;2655:	}
LABELV $1267
line 2657
;2656:
;2657:	if (!down) {
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $1271
line 2658
;2658:		inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2659
;2659:		return;
ADDRGP4 $1235
JUMPV
LABELV $1271
line 2663
;2660:	}
;2661:
;2662:	// default handling
;2663:	switch ( key ) {
ADDRLP4 36
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 155
EQI4 $1275
ADDRLP4 40
CNSTI4 156
ASGNI4
ADDRLP4 36
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $1279
ADDRLP4 36
INDIRI4
ADDRLP4 40
INDIRI4
GTI4 $1311
LABELV $1310
ADDRLP4 44
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 48
CNSTI4 13
ASGNI4
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRI4
EQI4 $1304
ADDRLP4 44
INDIRI4
ADDRLP4 48
INDIRI4
GTI4 $1313
LABELV $1312
ADDRFP4 4
INDIRI4
CNSTI4 9
EQI4 $1288
ADDRGP4 $1273
JUMPV
LABELV $1313
ADDRLP4 52
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 56
CNSTI4 27
ASGNI4
ADDRLP4 52
INDIRI4
ADDRLP4 56
INDIRI4
EQI4 $1284
ADDRLP4 52
INDIRI4
ADDRLP4 56
INDIRI4
LTI4 $1273
LABELV $1314
ADDRLP4 60
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 132
EQI4 $1283
ADDRLP4 60
INDIRI4
CNSTI4 133
EQI4 $1288
ADDRGP4 $1273
JUMPV
LABELV $1311
ADDRLP4 64
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 167
EQI4 $1288
ADDRLP4 68
CNSTI4 169
ASGNI4
ADDRLP4 64
INDIRI4
ADDRLP4 68
INDIRI4
EQI4 $1304
ADDRLP4 64
INDIRI4
ADDRLP4 68
INDIRI4
GTI4 $1316
LABELV $1315
ADDRFP4 4
INDIRI4
CNSTI4 161
EQI4 $1283
ADDRGP4 $1273
JUMPV
LABELV $1316
ADDRLP4 72
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 178
LTI4 $1273
ADDRLP4 72
INDIRI4
CNSTI4 188
GTI4 $1317
ADDRLP4 72
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1318-712
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1318
address $1289
address $1289
address $1273
address $1273
address $1273
address $1273
address $1273
address $1274
address $1274
address $1274
address $1274
code
LABELV $1317
ADDRLP4 76
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 217
LTI4 $1273
ADDRLP4 76
INDIRI4
CNSTI4 232
GTI4 $1273
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1320-868
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1320
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
address $1274
code
LABELV $1275
line 2666
;2664:
;2665:		case K_F11:
;2666:			if (DC->getCVarValue("developer")) {
ADDRGP4 $1278
ARGP4
ADDRLP4 80
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 80
INDIRF4
CNSTF4 0
EQF4 $1274
line 2667
;2667:				debugMode ^= 1;
ADDRLP4 84
ADDRGP4 debugMode
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 2668
;2668:			}
line 2669
;2669:			break;
ADDRGP4 $1274
JUMPV
LABELV $1279
line 2672
;2670:
;2671:		case K_F12:
;2672:			if (DC->getCVarValue("developer")) {
ADDRGP4 $1278
ARGP4
ADDRLP4 84
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 84
INDIRF4
CNSTF4 0
EQF4 $1274
line 2673
;2673:				DC->executeText(EXEC_APPEND, "screenshot\n");
CNSTI4 2
ARGI4
ADDRGP4 $1282
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 148
ADDP4
INDIRP4
CALLV
pop
line 2674
;2674:			}
line 2675
;2675:			break;
ADDRGP4 $1274
JUMPV
LABELV $1283
line 2678
;2676:		case K_KP_UPARROW:
;2677:		case K_UPARROW:
;2678:			Menu_SetPrevCursorItem(menu);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_SetPrevCursorItem
CALLP4
pop
line 2679
;2679:			break;
ADDRGP4 $1274
JUMPV
LABELV $1284
line 2682
;2680:
;2681:		case K_ESCAPE:
;2682:			if (!g_waitingForKey && menu->onESC) {
ADDRGP4 g_waitingForKey
INDIRI4
CNSTI4 0
NEI4 $1274
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1274
line 2684
;2683:				itemDef_t it;
;2684:		    it.parent = menu;
ADDRLP4 88+228
ADDRFP4 0
INDIRP4
ASGNP4
line 2685
;2685:		    Item_RunScript(&it, menu->onESC);
ADDRLP4 88
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_RunScript
CALLV
pop
line 2686
;2686:			}
line 2687
;2687:			break;
ADDRGP4 $1274
JUMPV
LABELV $1288
line 2691
;2688:		case K_TAB:
;2689:		case K_KP_DOWNARROW:
;2690:		case K_DOWNARROW:
;2691:			Menu_SetNextCursorItem(menu);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_SetNextCursorItem
CALLP4
pop
line 2692
;2692:			break;
ADDRGP4 $1274
JUMPV
LABELV $1289
line 2696
;2693:
;2694:		case K_MOUSE1:
;2695:		case K_MOUSE2:
;2696:			if (item) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1274
line 2697
;2697:				if (item->type == ITEM_TYPE_TEXT) {
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1292
line 2698
;2698:					if (Rect_ContainsPoint(Item_CorrectedTextRect(item), DC->cursorx, DC->cursory)) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 88
ADDRGP4 Item_CorrectedTextRect
CALLP4
ASGNP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 92
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 96
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 96
INDIRI4
CNSTI4 0
EQI4 $1274
line 2699
;2699:						Item_Action(item);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Item_Action
CALLV
pop
line 2700
;2700:					}
line 2701
;2701:				} else if (item->type == ITEM_TYPE_EDITFIELD || item->type == ITEM_TYPE_NUMERICFIELD) {
ADDRGP4 $1274
JUMPV
LABELV $1292
ADDRLP4 88
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 88
INDIRI4
CNSTI4 4
EQI4 $1298
ADDRLP4 88
INDIRI4
CNSTI4 9
NEI4 $1296
LABELV $1298
line 2702
;2702:					if (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory)) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 92
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 96
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 96
INDIRI4
CNSTI4 0
EQI4 $1274
line 2703
;2703:						item->cursorPos = 0;
ADDRLP4 4
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 0
ASGNI4
line 2704
;2704:						g_editingField = qtrue;
ADDRGP4 g_editingField
CNSTI4 1
ASGNI4
line 2705
;2705:						g_editItem = item;
ADDRGP4 g_editItem
ADDRLP4 4
INDIRP4
ASGNP4
line 2706
;2706:						DC->setOverstrikeMode(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 104
ADDP4
INDIRP4
CALLV
pop
line 2707
;2707:					}
line 2708
;2708:				} else {
ADDRGP4 $1274
JUMPV
LABELV $1296
line 2709
;2709:					if (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory)) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 92
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 92
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 96
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 96
INDIRI4
CNSTI4 0
EQI4 $1274
line 2710
;2710:						Item_Action(item);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Item_Action
CALLV
pop
line 2711
;2711:					}
line 2712
;2712:				}
line 2713
;2713:			}
line 2714
;2714:			break;
ADDRGP4 $1274
JUMPV
line 2736
;2715:
;2716:		case K_JOY1:
;2717:		case K_JOY2:
;2718:		case K_JOY3:
;2719:		case K_JOY4:
;2720:		case K_AUX1:
;2721:		case K_AUX2:
;2722:		case K_AUX3:
;2723:		case K_AUX4:
;2724:		case K_AUX5:
;2725:		case K_AUX6:
;2726:		case K_AUX7:
;2727:		case K_AUX8:
;2728:		case K_AUX9:
;2729:		case K_AUX10:
;2730:		case K_AUX11:
;2731:		case K_AUX12:
;2732:		case K_AUX13:
;2733:		case K_AUX14:
;2734:		case K_AUX15:
;2735:		case K_AUX16:
;2736:			break;
LABELV $1304
line 2739
;2737:		case K_KP_ENTER:
;2738:		case K_ENTER:
;2739:			if (item) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1274
line 2740
;2740:				if (item->type == ITEM_TYPE_EDITFIELD || item->type == ITEM_TYPE_NUMERICFIELD) {
ADDRLP4 88
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 88
INDIRI4
CNSTI4 4
EQI4 $1309
ADDRLP4 88
INDIRI4
CNSTI4 9
NEI4 $1307
LABELV $1309
line 2741
;2741:					item->cursorPos = 0;
ADDRLP4 4
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 0
ASGNI4
line 2742
;2742:					g_editingField = qtrue;
ADDRGP4 g_editingField
CNSTI4 1
ASGNI4
line 2743
;2743:					g_editItem = item;
ADDRGP4 g_editItem
ADDRLP4 4
INDIRP4
ASGNP4
line 2744
;2744:					DC->setOverstrikeMode(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 104
ADDP4
INDIRP4
CALLV
pop
line 2745
;2745:				} else {
ADDRGP4 $1274
JUMPV
LABELV $1307
line 2746
;2746:						Item_Action(item);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Item_Action
CALLV
pop
line 2747
;2747:				}
line 2748
;2748:			}
line 2749
;2749:			break;
LABELV $1273
LABELV $1274
line 2751
;2750:	}
;2751:	inHandler = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2752
;2752:}
LABELV $1235
endproc Menu_HandleKey 628 12
export ToWindowCoords
proc ToWindowCoords 8 0
line 2754
;2753:
;2754:void ToWindowCoords(float *x, float *y, windowDef_t *window) {
line 2755
;2755:	if (window->border != 0) {
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $1323
line 2756
;2756:		*x += window->borderSize;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2757
;2757:		*y += window->borderSize;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2758
;2758:	} 
LABELV $1323
line 2759
;2759:	*x += window->rect.x;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
INDIRF4
ADDF4
ASGNF4
line 2760
;2760:	*y += window->rect.y;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2761
;2761:}
LABELV $1322
endproc ToWindowCoords 8 0
export Rect_ToWindowCoords
proc Rect_ToWindowCoords 4 12
line 2763
;2762:
;2763:void Rect_ToWindowCoords(rectDef_t *rect, windowDef_t *window) {
line 2764
;2764:	ToWindowCoords(&rect->x, &rect->y, window);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 ToWindowCoords
CALLV
pop
line 2765
;2765:}
LABELV $1325
endproc Rect_ToWindowCoords 4 12
export Item_SetTextExtents
proc Item_SetTextExtents 296 12
line 2767
;2766:
;2767:void Item_SetTextExtents(itemDef_t *item, int *width, int *height, const char *text) {
line 2768
;2768:	const char *textPtr = (text) ? text : item->text;
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1328
ADDRLP4 4
ADDRFP4 12
INDIRP4
ASGNP4
ADDRGP4 $1329
JUMPV
LABELV $1328
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
LABELV $1329
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 2770
;2769:
;2770:	if (textPtr == NULL ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1330
line 2771
;2771:		return;
ADDRGP4 $1326
JUMPV
LABELV $1330
line 2774
;2772:	}
;2773:
;2774:	*width = item->textRect.w;
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 2775
;2775:	*height = item->textRect.h;
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 2778
;2776:
;2777:	// keeps us from computing the widths and heights more than once
;2778:	if (*width == 0 || (item->type == ITEM_TYPE_OWNERDRAW && item->textalignment == ITEM_ALIGN_CENTER)) {
ADDRFP4 4
INDIRP4
INDIRI4
CNSTI4 0
EQI4 $1334
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 8
NEI4 $1332
ADDRLP4 8
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1332
LABELV $1334
line 2779
;2779:		int originalWidth = DC->textWidth(item->text, item->textscale, 0);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 20
ADDRGP4 DC
INDIRP4
CNSTI4 20
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 20
INDIRI4
ASGNI4
line 2781
;2780:
;2781:		if (item->type == ITEM_TYPE_OWNERDRAW && (item->textalignment == ITEM_ALIGN_CENTER || item->textalignment == ITEM_ALIGN_RIGHT)) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 8
NEI4 $1335
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 1
EQI4 $1337
ADDRLP4 28
INDIRI4
CNSTI4 2
NEI4 $1335
LABELV $1337
line 2782
;2782:			originalWidth += DC->ownerDrawWidth(item->window.ownerDraw, item->textscale);
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 32
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 DC
INDIRP4
CNSTI4 164
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 36
INDIRI4
ADDI4
ASGNI4
line 2783
;2783:		} else if (item->type == ITEM_TYPE_EDITFIELD && item->textalignment == ITEM_ALIGN_CENTER && item->cvar) {
ADDRGP4 $1336
JUMPV
LABELV $1335
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 4
NEI4 $1338
ADDRLP4 32
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1338
ADDRLP4 32
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1338
line 2785
;2784:			char buff[256];
;2785:			DC->getCVarString(item->cvar, buff, 256);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 2786
;2786:			originalWidth += DC->textWidth(buff, item->textscale, 0);
ADDRLP4 36
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 292
ADDRGP4 DC
INDIRP4
CNSTI4 20
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 292
INDIRI4
ADDI4
ASGNI4
line 2787
;2787:		}
LABELV $1338
LABELV $1336
line 2789
;2788:
;2789:		*width = DC->textWidth(textPtr, item->textscale, 0);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 36
ADDRGP4 DC
INDIRP4
CNSTI4 20
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 36
INDIRI4
ASGNI4
line 2790
;2790:		*height = DC->textHeight(textPtr, item->textscale, 0);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 40
ADDRGP4 DC
INDIRP4
CNSTI4 24
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 8
INDIRP4
ADDRLP4 40
INDIRI4
ASGNI4
line 2791
;2791:		item->textRect.w = *width;
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
ADDRFP4 4
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
line 2792
;2792:		item->textRect.h = *height;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
line 2793
;2793:		item->textRect.x = item->textalignx;
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 44
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ASGNF4
line 2794
;2794:		item->textRect.y = item->textaligny;
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 48
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
ASGNF4
line 2795
;2795:		if (item->textalignment == ITEM_ALIGN_RIGHT) {
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 2
NEI4 $1340
line 2796
;2796:			item->textRect.x = item->textalignx - originalWidth;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 52
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ADDRLP4 12
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 2797
;2797:		} else if (item->textalignment == ITEM_ALIGN_CENTER) {
ADDRGP4 $1341
JUMPV
LABELV $1340
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1342
line 2798
;2798:			item->textRect.x = item->textalignx - originalWidth / 2;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 52
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ADDRLP4 12
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
SUBF4
ASGNF4
line 2799
;2799:		}
LABELV $1342
LABELV $1341
line 2801
;2800:
;2801:		ToWindowCoords(&item->textRect.x, &item->textRect.y, &item->window);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 52
INDIRP4
CNSTI4 184
ADDP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 ToWindowCoords
CALLV
pop
line 2802
;2802:	}
LABELV $1332
line 2803
;2803:}
LABELV $1326
endproc Item_SetTextExtents 296 12
export Item_TextColor
proc Item_TextColor 60 28
line 2805
;2804:
;2805:void Item_TextColor(itemDef_t *item, vec4_t *newColor) {
line 2807
;2806:	vec4_t lowLight;
;2807:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 2809
;2808:
;2809:	Fade(&item->window.flags, &item->window.foreColor[3], parent->fadeClamp, &item->window.nextTime, parent->fadeCycle, qtrue, parent->fadeAmount);
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 124
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRF4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 108
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ARGF4
ADDRGP4 Fade
CALLV
pop
line 2811
;2810:
;2811:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1345
line 2812
;2812:		lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 2813
;2813:		lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 4+4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 2814
;2814:		lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 4+8
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 2815
;2815:		lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 4+12
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 2816
;2816:		LerpColor(parent->focusColor,lowLight,*newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 28
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTF4 1056964608
ADDRLP4 28
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 2817
;2817:	} else if (item->textStyle == ITEM_TEXTSTYLE_BLINK && !((DC->realTime/BLINK_DIVISOR) & 1)) {
ADDRGP4 $1346
JUMPV
LABELV $1345
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ADDRLP4 28
INDIRI4
NEI4 $1350
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 200
DIVI4
ADDRLP4 28
INDIRI4
BANDI4
CNSTI4 0
NEI4 $1350
line 2818
;2818:		lowLight[0] = 0.8 * item->window.foreColor[0]; 
ADDRLP4 4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRF4
MULF4
ASGNF4
line 2819
;2819:		lowLight[1] = 0.8 * item->window.foreColor[1]; 
ADDRLP4 4+4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
MULF4
ASGNF4
line 2820
;2820:		lowLight[2] = 0.8 * item->window.foreColor[2]; 
ADDRLP4 4+8
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
MULF4
ASGNF4
line 2821
;2821:		lowLight[3] = 0.8 * item->window.foreColor[3]; 
ADDRLP4 4+12
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
MULF4
ASGNF4
line 2822
;2822:		LerpColor(item->window.foreColor,lowLight,*newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 32
ADDRGP4 sin
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTF4 1056964608
ADDRLP4 32
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 2823
;2823:	} else {
ADDRGP4 $1351
JUMPV
LABELV $1350
line 2824
;2824:		memcpy(newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 2826
;2825:		// items can be enabled and disabled based on cvars
;2826:	}
LABELV $1351
LABELV $1346
line 2828
;2827:
;2828:	if (item->enableCvar && *item->enableCvar && item->cvarTest && *item->cvarTest) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 272
ADDP4
INDIRP4
ASGNP4
ADDRLP4 40
CNSTU4 0
ASGNU4
ADDRLP4 36
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
EQU4 $1355
ADDRLP4 44
CNSTI4 0
ASGNI4
ADDRLP4 36
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 44
INDIRI4
EQI4 $1355
ADDRLP4 48
ADDRLP4 32
INDIRP4
CNSTI4 268
ADDP4
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
EQU4 $1355
ADDRLP4 48
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 44
INDIRI4
EQI4 $1355
line 2829
;2829:		if (item->cvarFlags & (CVAR_ENABLE | CVAR_DISABLE) && !Item_EnableShowViaCvar(item, CVAR_ENABLE)) {
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $1357
ADDRLP4 52
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 56
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $1357
line 2830
;2830:			memcpy(newColor, &parent->disableColor, sizeof(vec4_t));
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 244
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 2831
;2831:		}
LABELV $1357
line 2832
;2832:	}
LABELV $1355
line 2833
;2833:}
LABELV $1344
endproc Item_TextColor 60 28
export Item_Text_AutoWrapped_Paint
proc Item_Text_AutoWrapped_Paint 2128 32
line 2835
;2834:
;2835:void Item_Text_AutoWrapped_Paint(itemDef_t *item) {
line 2843
;2836:	char text[1024];
;2837:	const char *p, *textPtr, *newLinePtr;
;2838:	char buff[1024];
;2839:	int width, height, len, textWidth, newLine, newLineWidth;
;2840:	float y;
;2841:	vec4_t color;
;2842:
;2843:	textWidth = 0;
ADDRLP4 1032
CNSTI4 0
ASGNI4
line 2844
;2844:	newLinePtr = NULL;
ADDRLP4 1044
CNSTP4 0
ASGNP4
line 2846
;2845:
;2846:	if (item->text == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1360
line 2847
;2847:		if (item->cvar == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1362
line 2848
;2848:			return;
ADDRGP4 $1359
JUMPV
LABELV $1362
line 2850
;2849:		}
;2850:		else {
line 2851
;2851:			DC->getCVarString(item->cvar, text, sizeof(text));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 1080
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 2852
;2852:			textPtr = text;
ADDRLP4 1056
ADDRLP4 1080
ASGNP4
line 2853
;2853:		}
line 2854
;2854:	}
ADDRGP4 $1361
JUMPV
LABELV $1360
line 2855
;2855:	else {
line 2856
;2856:		textPtr = item->text;
ADDRLP4 1056
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
line 2857
;2857:	}
LABELV $1361
line 2858
;2858:	if (*textPtr == '\0') {
ADDRLP4 1056
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1364
line 2859
;2859:		return;
ADDRGP4 $1359
JUMPV
LABELV $1364
line 2861
;2860:	}
;2861:	Item_TextColor(item, &color);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1060
ARGP4
ADDRGP4 Item_TextColor
CALLV
pop
line 2862
;2862:	Item_SetTextExtents(item, &width, &height, textPtr);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1076
ARGP4
ADDRLP4 1052
ARGP4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRGP4 Item_SetTextExtents
CALLV
pop
line 2864
;2863:
;2864:	y = item->textaligny;
ADDRLP4 1048
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
ASGNF4
line 2865
;2865:	len = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 2866
;2866:	buff[0] = '\0';
ADDRLP4 4
CNSTI1 0
ASGNI1
line 2867
;2867:	newLine = 0;
ADDRLP4 1036
CNSTI4 0
ASGNI4
line 2868
;2868:	newLineWidth = 0;
ADDRLP4 1040
CNSTI4 0
ASGNI4
line 2869
;2869:	p = textPtr;
ADDRLP4 0
ADDRLP4 1056
INDIRP4
ASGNP4
ADDRGP4 $1367
JUMPV
LABELV $1366
line 2870
;2870:	while (p) {
line 2871
;2871:		if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
ADDRLP4 2104
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 2104
INDIRI4
CNSTI4 32
EQI4 $1373
ADDRLP4 2104
INDIRI4
CNSTI4 9
EQI4 $1373
ADDRLP4 2104
INDIRI4
CNSTI4 10
EQI4 $1373
ADDRLP4 2104
INDIRI4
CNSTI4 0
NEI4 $1369
LABELV $1373
line 2872
;2872:			newLine = len;
ADDRLP4 1036
ADDRLP4 1028
INDIRI4
ASGNI4
line 2873
;2873:			newLinePtr = p+1;
ADDRLP4 1044
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 2874
;2874:			newLineWidth = textWidth;
ADDRLP4 1040
ADDRLP4 1032
INDIRI4
ASGNI4
line 2875
;2875:		}
LABELV $1369
line 2876
;2876:		textWidth = DC->textWidth(buff, item->textscale, 0);
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 2108
ADDRGP4 DC
INDIRP4
CNSTI4 20
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1032
ADDRLP4 2108
INDIRI4
ASGNI4
line 2877
;2877:		if ( (newLine && textWidth > item->window.rect.w) || *p == '\n' || *p == '\0') {
ADDRLP4 1036
INDIRI4
CNSTI4 0
EQI4 $1377
ADDRLP4 1032
INDIRI4
CVIF4 4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
GTF4 $1378
LABELV $1377
ADDRLP4 2112
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 2112
INDIRI4
CNSTI4 10
EQI4 $1378
ADDRLP4 2112
INDIRI4
CNSTI4 0
NEI4 $1374
LABELV $1378
line 2878
;2878:			if (len) {
ADDRLP4 1028
INDIRI4
CNSTI4 0
EQI4 $1379
line 2879
;2879:				if (item->textalignment == ITEM_ALIGN_LEFT) {
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1381
line 2880
;2880:					item->textRect.x = item->textalignx;
ADDRLP4 2116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2116
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 2116
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ASGNF4
line 2881
;2881:				} else if (item->textalignment == ITEM_ALIGN_RIGHT) {
ADDRGP4 $1382
JUMPV
LABELV $1381
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 2
NEI4 $1383
line 2882
;2882:					item->textRect.x = item->textalignx - newLineWidth;
ADDRLP4 2116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2116
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 2116
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ADDRLP4 1040
INDIRI4
CVIF4 4
SUBF4
ASGNF4
line 2883
;2883:				} else if (item->textalignment == ITEM_ALIGN_CENTER) {
ADDRGP4 $1384
JUMPV
LABELV $1383
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1385
line 2884
;2884:					item->textRect.x = item->textalignx - newLineWidth / 2;
ADDRLP4 2116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2116
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 2116
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ADDRLP4 1040
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
SUBF4
ASGNF4
line 2885
;2885:				}
LABELV $1385
LABELV $1384
LABELV $1382
line 2886
;2886:				item->textRect.y = y;
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 1048
INDIRF4
ASGNF4
line 2887
;2887:				ToWindowCoords(&item->textRect.x, &item->textRect.y, &item->window);
ADDRLP4 2116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2116
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 2116
INDIRP4
CNSTI4 184
ADDP4
ARGP4
ADDRLP4 2116
INDIRP4
ARGP4
ADDRGP4 ToWindowCoords
CALLV
pop
line 2889
;2888:				//
;2889:				buff[newLine] = '\0';
ADDRLP4 1036
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 0
ASGNI1
line 2890
;2890:				DC->drawText(item->textRect.x, item->textRect.y, item->textscale, color, buff, 0, 0, item->textStyle);
ADDRLP4 2120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2120
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ARGF4
ADDRLP4 2120
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 2120
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 1060
ARGP4
ADDRLP4 4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 2120
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 2891
;2891:			}
LABELV $1379
line 2892
;2892:			if (*p == '\0') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1387
line 2893
;2893:				break;
ADDRGP4 $1368
JUMPV
LABELV $1387
line 2896
;2894:			}
;2895:			//
;2896:			y += height + 5;
ADDRLP4 1048
ADDRLP4 1048
INDIRF4
ADDRLP4 1052
INDIRI4
CNSTI4 5
ADDI4
CVIF4 4
ADDF4
ASGNF4
line 2897
;2897:			p = newLinePtr;
ADDRLP4 0
ADDRLP4 1044
INDIRP4
ASGNP4
line 2898
;2898:			len = 0;
ADDRLP4 1028
CNSTI4 0
ASGNI4
line 2899
;2899:			newLine = 0;
ADDRLP4 1036
CNSTI4 0
ASGNI4
line 2900
;2900:			newLineWidth = 0;
ADDRLP4 1040
CNSTI4 0
ASGNI4
line 2901
;2901:			continue;
ADDRGP4 $1367
JUMPV
LABELV $1374
line 2903
;2902:		}
;2903:		buff[len++] = *p++;
ADDRLP4 2116
ADDRLP4 1028
INDIRI4
ASGNI4
ADDRLP4 2124
CNSTI4 1
ASGNI4
ADDRLP4 1028
ADDRLP4 2116
INDIRI4
ADDRLP4 2124
INDIRI4
ADDI4
ASGNI4
ADDRLP4 2120
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 2120
INDIRP4
ADDRLP4 2124
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2116
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 2120
INDIRP4
INDIRI1
ASGNI1
line 2904
;2904:		buff[len] = '\0';
ADDRLP4 1028
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 0
ASGNI1
line 2905
;2905:	}
LABELV $1367
line 2870
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1366
LABELV $1368
line 2906
;2906:}
LABELV $1359
endproc Item_Text_AutoWrapped_Paint 2128 32
export Item_Text_Wrapped_Paint
proc Item_Text_Wrapped_Paint 2112 32
line 2908
;2907:
;2908:void Item_Text_Wrapped_Paint(itemDef_t *item) {
line 2919
;2909:	char text[1024];
;2910:	const char *p, *start, *textPtr;
;2911:	char buff[1024];
;2912:	int width, height;
;2913:	float x, y;
;2914:	vec4_t color;
;2915:
;2916:	// now paint the text and/or any optional images
;2917:	// default to left
;2918:
;2919:	if (item->text == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1390
line 2920
;2920:		if (item->cvar == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1392
line 2921
;2921:			return;
ADDRGP4 $1389
JUMPV
LABELV $1392
line 2923
;2922:		}
;2923:		else {
line 2924
;2924:			DC->getCVarString(item->cvar, text, sizeof(text));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 1068
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 2925
;2925:			textPtr = text;
ADDRLP4 1060
ADDRLP4 1068
ASGNP4
line 2926
;2926:		}
line 2927
;2927:	}
ADDRGP4 $1391
JUMPV
LABELV $1390
line 2928
;2928:	else {
line 2929
;2929:		textPtr = item->text;
ADDRLP4 1060
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
line 2930
;2930:	}
LABELV $1391
line 2931
;2931:	if (*textPtr == '\0') {
ADDRLP4 1060
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1394
line 2932
;2932:		return;
ADDRGP4 $1389
JUMPV
LABELV $1394
line 2935
;2933:	}
;2934:
;2935:	Item_TextColor(item, &color);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1040
ARGP4
ADDRGP4 Item_TextColor
CALLV
pop
line 2936
;2936:	Item_SetTextExtents(item, &width, &height, textPtr);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1064
ARGP4
ADDRLP4 1056
ARGP4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRGP4 Item_SetTextExtents
CALLV
pop
line 2938
;2937:
;2938:	x = item->textRect.x;
ADDRLP4 1036
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ASGNF4
line 2939
;2939:	y = item->textRect.y;
ADDRLP4 1032
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ASGNF4
line 2940
;2940:	start = textPtr;
ADDRLP4 4
ADDRLP4 1060
INDIRP4
ASGNP4
line 2941
;2941:	p = strchr(textPtr, '\r');
ADDRLP4 1060
INDIRP4
ARGP4
CNSTI4 13
ARGI4
ADDRLP4 2092
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2092
INDIRP4
ASGNP4
ADDRGP4 $1397
JUMPV
LABELV $1396
line 2942
;2942:	while (p && *p) {
line 2943
;2943:		strncpy(buff, start, p-start+1);
ADDRLP4 8
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1
ADDI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 2944
;2944:		buff[p-start] = '\0';
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 2945
;2945:		DC->drawText(x, y, item->textscale, color, buff, 0, 0, item->textStyle);
ADDRLP4 1036
INDIRF4
ARGF4
ADDRLP4 1032
INDIRF4
ARGF4
ADDRLP4 2100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2100
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 1040
ARGP4
ADDRLP4 8
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 2100
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 2946
;2946:		y += height + 5;
ADDRLP4 1032
ADDRLP4 1032
INDIRF4
ADDRLP4 1056
INDIRI4
CNSTI4 5
ADDI4
CVIF4 4
ADDF4
ASGNF4
line 2947
;2947:		start += p - start + 1;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1
ADDI4
ADDRLP4 4
INDIRP4
ADDP4
ASGNP4
line 2948
;2948:		p = strchr(p+1, '\r');
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ARGP4
CNSTI4 13
ARGI4
ADDRLP4 2108
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2108
INDIRP4
ASGNP4
line 2949
;2949:	}
LABELV $1397
line 2942
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1399
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1396
LABELV $1399
line 2950
;2950:	DC->drawText(x, y, item->textscale, color, start, 0, 0, item->textStyle);
ADDRLP4 1036
INDIRF4
ARGF4
ADDRLP4 1032
INDIRF4
ARGF4
ADDRLP4 2100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 2100
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 1040
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 2100
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 2951
;2951:}
LABELV $1389
endproc Item_Text_Wrapped_Paint 2112 32
export Item_Text_Paint
proc Item_Text_Paint 1056 32
line 2953
;2952:
;2953:void Item_Text_Paint(itemDef_t *item) {
line 2959
;2954:	char text[1024];
;2955:	const char *textPtr;
;2956:	int height, width;
;2957:	vec4_t color;
;2958:
;2959:	if (item->window.flags & WINDOW_WRAPPED) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 262144
BANDI4
CNSTI4 0
EQI4 $1401
line 2960
;2960:		Item_Text_Wrapped_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Wrapped_Paint
CALLV
pop
line 2961
;2961:		return;
ADDRGP4 $1400
JUMPV
LABELV $1401
line 2963
;2962:	}
;2963:	if (item->window.flags & WINDOW_AUTOWRAPPED) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 524288
BANDI4
CNSTI4 0
EQI4 $1403
line 2964
;2964:		Item_Text_AutoWrapped_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_AutoWrapped_Paint
CALLV
pop
line 2965
;2965:		return;
ADDRGP4 $1400
JUMPV
LABELV $1403
line 2968
;2966:	}
;2967:
;2968:	if (item->text == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1405
line 2969
;2969:		if (item->cvar == NULL) {
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1407
line 2970
;2970:			return;
ADDRGP4 $1400
JUMPV
LABELV $1407
line 2972
;2971:		}
;2972:		else {
line 2973
;2973:			DC->getCVarString(item->cvar, text, sizeof(text));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 2974
;2974:			textPtr = text;
ADDRLP4 0
ADDRLP4 28
ASGNP4
line 2975
;2975:		}
line 2976
;2976:	}
ADDRGP4 $1406
JUMPV
LABELV $1405
line 2977
;2977:	else {
line 2978
;2978:		textPtr = item->text;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
line 2979
;2979:	}
LABELV $1406
line 2982
;2980:
;2981:	// this needs to go here as it sets extents for cvar types as well
;2982:	Item_SetTextExtents(item, &width, &height, textPtr);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Item_SetTextExtents
CALLV
pop
line 2984
;2983:
;2984:	if (*textPtr == '\0') {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1409
line 2985
;2985:		return;
ADDRGP4 $1400
JUMPV
LABELV $1409
line 2989
;2986:	}
;2987:
;2988:
;2989:	Item_TextColor(item, &color);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Item_TextColor
CALLV
pop
line 3020
;2990:
;2991:	//FIXME: this is a fucking mess
;2992:/*
;2993:	adjust = 0;
;2994:	if (item->textStyle == ITEM_TEXTSTYLE_OUTLINED || item->textStyle == ITEM_TEXTSTYLE_OUTLINESHADOWED) {
;2995:		adjust = 0.5;
;2996:	}
;2997:
;2998:	if (item->textStyle == ITEM_TEXTSTYLE_SHADOWED || item->textStyle == ITEM_TEXTSTYLE_OUTLINESHADOWED) {
;2999:		Fade(&item->window.flags, &DC->Assets.shadowColor[3], DC->Assets.fadeClamp, &item->window.nextTime, DC->Assets.fadeCycle, qfalse);
;3000:		DC->drawText(item->textRect.x + DC->Assets.shadowX, item->textRect.y + DC->Assets.shadowY, item->textscale, DC->Assets.shadowColor, textPtr, adjust);
;3001:	}
;3002:*/
;3003:
;3004:
;3005://	if (item->textStyle == ITEM_TEXTSTYLE_OUTLINED || item->textStyle == ITEM_TEXTSTYLE_OUTLINESHADOWED) {
;3006://		Fade(&item->window.flags, &item->window.outlineColor[3], DC->Assets.fadeClamp, &item->window.nextTime, DC->Assets.fadeCycle, qfalse);
;3007://		/*
;3008://		Text_Paint(item->textRect.x-1, item->textRect.y-1, item->textscale, item->window.foreColor, textPtr, adjust);
;3009://		Text_Paint(item->textRect.x, item->textRect.y-1, item->textscale, item->window.foreColor, textPtr, adjust);
;3010://		Text_Paint(item->textRect.x+1, item->textRect.y-1, item->textscale, item->window.foreColor, textPtr, adjust);
;3011://		Text_Paint(item->textRect.x-1, item->textRect.y, item->textscale, item->window.foreColor, textPtr, adjust);
;3012://		Text_Paint(item->textRect.x+1, item->textRect.y, item->textscale, item->window.foreColor, textPtr, adjust);
;3013://		Text_Paint(item->textRect.x-1, item->textRect.y+1, item->textscale, item->window.foreColor, textPtr, adjust);
;3014://		Text_Paint(item->textRect.x, item->textRect.y+1, item->textscale, item->window.foreColor, textPtr, adjust);
;3015://		Text_Paint(item->textRect.x+1, item->textRect.y+1, item->textscale, item->window.foreColor, textPtr, adjust);
;3016://		*/
;3017://		DC->drawText(item->textRect.x - 1, item->textRect.y + 1, item->textscale * 1.02, item->window.outlineColor, textPtr, adjust);
;3018://	}
;3019:
;3020:	DC->drawText(item->textRect.x, item->textRect.y, item->textscale, color, textPtr, 0, 0, item->textStyle);
ADDRLP4 1052
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1052
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ARGF4
ADDRLP4 1052
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 1052
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 1052
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3021
;3021:}
LABELV $1400
endproc Item_Text_Paint 1056 32
export Item_TextField_Paint
proc Item_TextField_Paint 1104 36
line 3028
;3022:
;3023:
;3024:
;3025://float			trap_Cvar_VariableValue( const char *var_name );
;3026://void			trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
;3027:
;3028:void Item_TextField_Paint(itemDef_t *item) {
line 3032
;3029:	char buff[1024];
;3030:	vec4_t newColor, lowLight;
;3031:	int offset;
;3032:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3033
;3033:	editFieldDef_t *editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 3035
;3034:
;3035:	Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3037
;3036:
;3037:	buff[0] = '\0';
ADDRLP4 8
CNSTI1 0
ASGNI1
line 3039
;3038:
;3039:	if (item->cvar) {
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1412
line 3040
;3040:		DC->getCVarString(item->cvar, buff, sizeof(buff));
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 88
ADDP4
INDIRP4
CALLV
pop
line 3041
;3041:	} 
LABELV $1412
line 3043
;3042:
;3043:	parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3045
;3044:
;3045:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1414
line 3046
;3046:		lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 1032
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3047
;3047:		lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 1032+4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3048
;3048:		lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 1032+8
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3049
;3049:		lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 1032+12
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3050
;3050:		LerpColor(parent->focusColor,lowLight,newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 1068
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 1032
ARGP4
ADDRLP4 1048
ARGP4
CNSTF4 1056964608
ADDRLP4 1068
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3051
;3051:	} else {
ADDRGP4 $1415
JUMPV
LABELV $1414
line 3052
;3052:		memcpy(&newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRLP4 1048
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3053
;3053:	}
LABELV $1415
line 3055
;3054:
;3055:	offset = (item->text && *item->text) ? 8 : 0;
ADDRLP4 1072
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1072
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1420
ADDRLP4 1072
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $1420
ADDRLP4 1068
CNSTI4 8
ASGNI4
ADDRGP4 $1421
JUMPV
LABELV $1420
ADDRLP4 1068
CNSTI4 0
ASGNI4
LABELV $1421
ADDRLP4 1064
ADDRLP4 1068
INDIRI4
ASGNI4
line 3056
;3056:	if (item->window.flags & WINDOW_HASFOCUS && g_editingField) {
ADDRLP4 1076
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 1076
INDIRI4
EQI4 $1422
ADDRGP4 g_editingField
INDIRI4
ADDRLP4 1076
INDIRI4
EQI4 $1422
line 3057
;3057:		char cursor = DC->getOverstrikeMode() ? '_' : '|';
ADDRLP4 1088
ADDRGP4 DC
INDIRP4
CNSTI4 108
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1088
INDIRI4
CNSTI4 0
EQI4 $1425
ADDRLP4 1084
CNSTI4 95
ASGNI4
ADDRGP4 $1426
JUMPV
LABELV $1425
ADDRLP4 1084
CNSTI4 124
ASGNI4
LABELV $1426
ADDRLP4 1080
ADDRLP4 1084
INDIRI4
CVII1 4
ASGNI1
line 3058
;3058:		DC->drawTextWithCursor(item->textRect.x + item->textRect.w + offset, item->textRect.y, item->textscale, newColor, buff + editPtr->paintOffset, item->cursorPos - editPtr->paintOffset , cursor, editPtr->maxPaintChars, item->textStyle);
ADDRLP4 1092
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1092
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 1092
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
ADDRLP4 1064
INDIRI4
CVIF4 4
ADDF4
ARGF4
ADDRLP4 1092
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 1092
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 1048
ARGP4
ADDRLP4 1100
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1100
INDIRI4
ADDRLP4 8
ADDP4
ARGP4
ADDRLP4 1092
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ADDRLP4 1100
INDIRI4
SUBI4
ARGI4
ADDRLP4 1080
INDIRI1
CVII4 1
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 1092
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 100
ADDP4
INDIRP4
CALLV
pop
line 3059
;3059:	} else {
ADDRGP4 $1423
JUMPV
LABELV $1422
line 3060
;3060:		DC->drawText(item->textRect.x + item->textRect.w + offset, item->textRect.y, item->textscale, newColor, buff + editPtr->paintOffset, 0, editPtr->maxPaintChars, item->textStyle);
ADDRLP4 1080
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 1080
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
ADDRLP4 1064
INDIRI4
CVIF4 4
ADDF4
ARGF4
ADDRLP4 1080
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 1080
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 1048
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ADDRLP4 8
ADDP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 1080
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3061
;3061:	}
LABELV $1423
line 3063
;3062:
;3063:}
LABELV $1411
endproc Item_TextField_Paint 1104 36
export Item_YesNo_Paint
proc Item_YesNo_Paint 56 32
line 3065
;3064:
;3065:void Item_YesNo_Paint(itemDef_t *item) {
line 3068
;3066:	vec4_t newColor, lowLight;
;3067:	float value;
;3068:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3070
;3069:
;3070:	value = (item->cvar) ? DC->getCVarValue(item->cvar) : 0;
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1429
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 44
INDIRF4
ASGNF4
ADDRGP4 $1430
JUMPV
LABELV $1429
ADDRLP4 40
CNSTF4 0
ASGNF4
LABELV $1430
ADDRLP4 36
ADDRLP4 40
INDIRF4
ASGNF4
line 3072
;3071:
;3072:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1431
line 3073
;3073:		lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3074
;3074:		lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 4+4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3075
;3075:		lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 4+8
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3076
;3076:		lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 4+12
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3077
;3077:		LerpColor(parent->focusColor,lowLight,newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 48
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 20
ARGP4
CNSTF4 1056964608
ADDRLP4 48
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3078
;3078:	} else {
ADDRGP4 $1432
JUMPV
LABELV $1431
line 3079
;3079:		memcpy(&newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRLP4 20
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3080
;3080:	}
LABELV $1432
line 3082
;3081:
;3082:	if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1436
line 3083
;3083:		Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3084
;3084:		DC->drawText(item->textRect.x + item->textRect.w + 8, item->textRect.y, item->textscale, newColor, (value != 0) ? "Yes" : "No", 0, 0, item->textStyle);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 20
ARGP4
ADDRLP4 36
INDIRF4
CNSTF4 0
EQF4 $1441
ADDRLP4 48
ADDRGP4 $1438
ASGNP4
ADDRGP4 $1442
JUMPV
LABELV $1441
ADDRLP4 48
ADDRGP4 $1439
ASGNP4
LABELV $1442
ADDRLP4 48
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3085
;3085:	} else {
ADDRGP4 $1437
JUMPV
LABELV $1436
line 3086
;3086:		DC->drawText(item->textRect.x, item->textRect.y, item->textscale, newColor, (value != 0) ? "Yes" : "No", 0, 0, item->textStyle);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 52
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 20
ARGP4
ADDRLP4 36
INDIRF4
CNSTF4 0
EQF4 $1444
ADDRLP4 48
ADDRGP4 $1438
ASGNP4
ADDRGP4 $1445
JUMPV
LABELV $1444
ADDRLP4 48
ADDRGP4 $1439
ASGNP4
LABELV $1445
ADDRLP4 48
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3087
;3087:	}
LABELV $1437
line 3088
;3088:}
LABELV $1427
endproc Item_YesNo_Paint 56 32
export Item_Multi_Paint
proc Item_Multi_Paint 48 32
line 3090
;3089:
;3090:void Item_Multi_Paint(itemDef_t *item) {
line 3092
;3091:	vec4_t newColor, lowLight;
;3092:	const char *text = "";
ADDRLP4 4
ADDRGP4 $54
ASGNP4
line 3093
;3093:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3095
;3094:
;3095:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1447
line 3096
;3096:		lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 8
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3097
;3097:		lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 8+4
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3098
;3098:		lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 8+8
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3099
;3099:		lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 8+12
CNSTF4 1061997773
ADDRLP4 0
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3100
;3100:		LerpColor(parent->focusColor,lowLight,newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 40
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 24
ARGP4
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3101
;3101:	} else {
ADDRGP4 $1448
JUMPV
LABELV $1447
line 3102
;3102:		memcpy(&newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRLP4 24
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3103
;3103:	}
LABELV $1448
line 3105
;3104:
;3105:	text = Item_Multi_Setting(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 Item_Multi_Setting
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 40
INDIRP4
ASGNP4
line 3107
;3106:
;3107:	if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1452
line 3108
;3108:		Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3109
;3109:		DC->drawText(item->textRect.x + item->textRect.w + 8, item->textRect.y, item->textscale, newColor, text, 0, 0, item->textStyle);
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 24
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 44
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3110
;3110:	} else {
ADDRGP4 $1453
JUMPV
LABELV $1452
line 3111
;3111:		DC->drawText(item->textRect.x, item->textRect.y, item->textscale, newColor, text, 0, 0, item->textStyle);
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 24
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 44
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3112
;3112:	}
LABELV $1453
line 3113
;3113:}
LABELV $1446
endproc Item_Multi_Paint 48 32
data
align 4
LABELV g_bindings
address $1456
byte 4 9
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1457
byte 4 13
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1458
byte 4 138
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1459
byte 4 132
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1460
byte 4 133
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1461
byte 4 44
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1462
byte 4 46
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1463
byte 4 32
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1464
byte 4 99
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1465
byte 4 134
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1466
byte 4 135
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1467
byte 4 136
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1468
byte 4 141
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1469
byte 4 140
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1470
byte 4 47
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1471
byte 4 144
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1472
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1473
byte 4 49
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1474
byte 4 50
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1475
byte 4 51
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1476
byte 4 52
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1477
byte 4 53
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1478
byte 4 54
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1479
byte 4 55
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1480
byte 4 56
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1481
byte 4 57
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1482
byte 4 48
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1483
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1484
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1485
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1486
byte 4 137
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1487
byte 4 91
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1488
byte 4 93
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1489
byte 4 180
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1490
byte 4 181
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1491
byte 4 119
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1492
byte 4 114
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1493
byte 4 116
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1494
byte 4 121
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1495
byte 4 110
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1496
byte 4 111
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1497
byte 4 100
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1498
byte 4 112
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1499
byte 4 99
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1500
byte 4 102
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1501
byte 4 118
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1502
byte 4 101
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1503
byte 4 105
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1504
byte 4 107
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1505
byte 4 145
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1506
byte 4 146
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1507
byte 4 147
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1508
byte 4 148
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1509
byte 4 149
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1510
byte 4 162
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1511
byte 4 168
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1512
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1513
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1514
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
address $1515
byte 4 -1
byte 4 -1
byte 4 -1
byte 4 -1
skip 4
lit
align 4
LABELV g_bindCount
byte 4 60
code
proc Controls_GetKeyAssignment 276 12
line 3222
;3114:
;3115:
;3116:typedef struct {
;3117:	char	*command;
;3118:	int		id;
;3119:	int		defaultbind1;
;3120:	int		defaultbind2;
;3121:	int		bind1;
;3122:	int		bind2;
;3123:} bind_t;
;3124:
;3125:typedef struct
;3126:{
;3127:	char*	name;
;3128:	float	defaultvalue;
;3129:	float	value;	
;3130:} configcvar_t;
;3131:
;3132:
;3133:static bind_t g_bindings[] = 
;3134:{
;3135:	{"+scores",			 K_TAB,				-1,		-1, -1},
;3136:	{"+button2",		 K_ENTER,			-1,		-1, -1},
;3137:	{"+speed", 			 K_SHIFT,			-1,		-1,	-1},
;3138:	{"+forward", 		 K_UPARROW,		-1,		-1, -1},
;3139:	{"+back", 			 K_DOWNARROW,	-1,		-1, -1},
;3140:	{"+moveleft", 	 ',',					-1,		-1, -1},
;3141:	{"+moveright", 	 '.',					-1,		-1, -1},
;3142:	{"+moveup",			 K_SPACE,			-1,		-1, -1},
;3143:	{"+movedown",		 'c',					-1,		-1, -1},
;3144:	{"+left", 			 K_LEFTARROW,	-1,		-1, -1},
;3145:	{"+right", 			 K_RIGHTARROW,	-1,		-1, -1},
;3146:	{"+strafe", 		 K_ALT,				-1,		-1, -1},
;3147:	{"+lookup", 		 K_PGDN,				-1,		-1, -1},
;3148:	{"+lookdown", 	 K_DEL,				-1,		-1, -1},
;3149:	{"+mlook", 			 '/',					-1,		-1, -1},
;3150:	{"centerview", 	 K_END,				-1,		-1, -1},
;3151:	{"+zoom", 			 -1,						-1,		-1, -1},
;3152:	{"weapon 1",		 '1',					-1,		-1, -1},
;3153:	{"weapon 2",		 '2',					-1,		-1, -1},
;3154:	{"weapon 3",		 '3',					-1,		-1, -1},
;3155:	{"weapon 4",		 '4',					-1,		-1, -1},
;3156:	{"weapon 5",		 '5',					-1,		-1, -1},
;3157:	{"weapon 6",		 '6',					-1,		-1, -1},
;3158:	{"weapon 7",		 '7',					-1,		-1, -1},
;3159:	{"weapon 8",		 '8',					-1,		-1, -1},
;3160:	{"weapon 9",		 '9',					-1,		-1, -1},
;3161:	{"weapon 10",		 '0',					-1,		-1, -1},
;3162:	{"weapon 11",		 -1,					-1,		-1, -1},
;3163:	{"weapon 12",		 -1,					-1,		-1, -1},
;3164:	{"weapon 13",		 -1,					-1,		-1, -1},
;3165:	{"+attack", 		 K_CTRL,				-1,		-1, -1},
;3166:	{"weapprev",		 '[',					-1,		-1, -1},
;3167:	{"weapnext", 		 ']',					-1,		-1, -1},
;3168:	{"+button3", 		 K_MOUSE3,			-1,		-1, -1},
;3169:	{"+button4", 		 K_MOUSE4,			-1,		-1, -1},
;3170:	{"prevTeamMember", 'w',					-1,		-1, -1},
;3171:	{"nextTeamMember", 'r',					-1,		-1, -1},
;3172:	{"nextOrder", 't',					-1,		-1, -1},
;3173:	{"confirmOrder", 'y',					-1,		-1, -1},
;3174:	{"denyOrder", 'n',					-1,		-1, -1},
;3175:	{"taskOffense", 'o',					-1,		-1, -1},
;3176:	{"taskDefense", 'd',					-1,		-1, -1},
;3177:	{"taskPatrol", 'p',					-1,		-1, -1},
;3178:	{"taskCamp", 'c',					-1,		-1, -1},
;3179:	{"taskFollow", 'f',					-1,		-1, -1},
;3180:	{"taskRetrieve", 'v',					-1,		-1, -1},
;3181:	{"taskEscort", 'e',					-1,		-1, -1},
;3182:	{"taskOwnFlag", 'i',					-1,		-1, -1},
;3183:	{"taskSuicide", 'k',					-1,		-1, -1},
;3184:	{"tauntKillInsult", K_F1,			-1,		-1, -1},
;3185:	{"tauntPraise", K_F2,			-1,		-1, -1},
;3186:	{"tauntTaunt", K_F3,			-1,		-1, -1},
;3187:	{"tauntDeathInsult", K_F4,			-1,		-1, -1},
;3188:	{"tauntGauntlet", K_F5,			-1,		-1, -1},
;3189:	{"scoresUp", K_KP_PGUP,			-1,		-1, -1},
;3190:	{"scoresDown", K_KP_PGDN,			-1,		-1, -1},
;3191:	// bk001205 - this one below was:  '-1' 
;3192:	{"messagemode",  -1,					-1,		-1, -1},
;3193:	{"messagemode2", -1,						-1,		-1, -1},
;3194:	{"messagemode3", -1,						-1,		-1, -1},
;3195:	{"messagemode4", -1,						-1,		-1, -1}
;3196:};
;3197:
;3198:
;3199:static const int g_bindCount = sizeof(g_bindings) / sizeof(bind_t);
;3200:
;3201:#ifndef MISSIONPACK // bk001206
;3202:static configcvar_t g_configcvars[] =
;3203:{
;3204:	{"cl_run",			0,					0},
;3205:	{"m_pitch",			0,					0},
;3206:	{"cg_autoswitch",	0,					0},
;3207:	{"sensitivity",		0,					0},
;3208:	{"in_joystick",		0,					0},
;3209:	{"joy_threshold",	0,					0},
;3210:	{"m_filter",		0,					0},
;3211:	{"cl_freelook",		0,					0},
;3212:	{NULL,				0,					0}
;3213:};
;3214:#endif
;3215:
;3216:/*
;3217:=================
;3218:Controls_GetKeyAssignment
;3219:=================
;3220:*/
;3221:static void Controls_GetKeyAssignment (char *command, int *twokeys)
;3222:{
line 3227
;3223:	int		count;
;3224:	int		j;
;3225:	char	b[256];
;3226:
;3227:	twokeys[0] = twokeys[1] = -1;
ADDRLP4 264
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 268
CNSTI4 -1
ASGNI4
ADDRLP4 264
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 268
INDIRI4
ASGNI4
ADDRLP4 264
INDIRP4
ADDRLP4 268
INDIRI4
ASGNI4
line 3228
;3228:	count = 0;
ADDRLP4 260
CNSTI4 0
ASGNI4
line 3230
;3229:
;3230:	for ( j = 0; j < 256; j++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $1517
line 3231
;3231:	{
line 3232
;3232:		DC->getBindingBuf( j, b, 256 );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 140
ADDP4
INDIRP4
CALLV
pop
line 3233
;3233:		if ( *b == 0 ) {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $1521
line 3234
;3234:			continue;
ADDRGP4 $1518
JUMPV
LABELV $1521
line 3236
;3235:		}
;3236:		if ( !Q_stricmp( b, command ) ) {
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 272
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 272
INDIRI4
CNSTI4 0
NEI4 $1523
line 3237
;3237:			twokeys[count] = j;
ADDRLP4 260
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 3238
;3238:			count++;
ADDRLP4 260
ADDRLP4 260
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3239
;3239:			if (count == 2) {
ADDRLP4 260
INDIRI4
CNSTI4 2
NEI4 $1525
line 3240
;3240:				break;
ADDRGP4 $1519
JUMPV
LABELV $1525
line 3242
;3241:			}
;3242:		}
LABELV $1523
line 3243
;3243:	}
LABELV $1518
line 3230
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 256
LTI4 $1517
LABELV $1519
line 3244
;3244:}
LABELV $1516
endproc Controls_GetKeyAssignment 276 12
export Controls_GetConfig
proc Controls_GetConfig 12 8
line 3252
;3245:
;3246:/*
;3247:=================
;3248:Controls_GetConfig
;3249:=================
;3250:*/
;3251:void Controls_GetConfig( void )
;3252:{
line 3257
;3253:	int		i;
;3254:	int		twokeys[2];
;3255:
;3256:	// iterate each command, get its numeric binding
;3257:	for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1531
JUMPV
LABELV $1528
line 3258
;3258:	{
line 3260
;3259:
;3260:		Controls_GetKeyAssignment(g_bindings[i].command, twokeys);
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Controls_GetKeyAssignment
CALLV
pop
line 3262
;3261:
;3262:		g_bindings[i].bind1 = twokeys[0];
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 3263
;3263:		g_bindings[i].bind2 = twokeys[1];
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
ADDRLP4 4+4
INDIRI4
ASGNI4
line 3264
;3264:	}
LABELV $1529
line 3257
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1531
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1528
line 3274
;3265:
;3266:	//s_controls.invertmouse.curvalue  = DC->getCVarValue( "m_pitch" ) < 0;
;3267:	//s_controls.smoothmouse.curvalue  = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "m_filter" ) );
;3268:	//s_controls.alwaysrun.curvalue    = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "cl_run" ) );
;3269:	//s_controls.autoswitch.curvalue   = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "cg_autoswitch" ) );
;3270:	//s_controls.sensitivity.curvalue  = UI_ClampCvar( 2, 30, Controls_GetCvarValue( "sensitivity" ) );
;3271:	//s_controls.joyenable.curvalue    = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "in_joystick" ) );
;3272:	//s_controls.joythreshold.curvalue = UI_ClampCvar( 0.05, 0.75, Controls_GetCvarValue( "joy_threshold" ) );
;3273:	//s_controls.freelook.curvalue     = UI_ClampCvar( 0, 1, Controls_GetCvarValue( "cl_freelook" ) );
;3274:}
LABELV $1527
endproc Controls_GetConfig 12 8
export Controls_SetConfig
proc Controls_SetConfig 12 8
line 3282
;3275:
;3276:/*
;3277:=================
;3278:Controls_SetConfig
;3279:=================
;3280:*/
;3281:void Controls_SetConfig(qboolean restart)
;3282:{
line 3286
;3283:	int		i;
;3284:
;3285:	// iterate each command, get its numeric binding
;3286:	for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1539
JUMPV
LABELV $1536
line 3287
;3287:	{
line 3289
;3288:
;3289:		if (g_bindings[i].bind1 != -1)
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $1540
line 3290
;3290:		{	
line 3291
;3291:			DC->setBinding( g_bindings[i].bind1, g_bindings[i].command );
ADDRLP4 4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ADDRGP4 g_bindings
ADDP4
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
line 3293
;3292:
;3293:			if (g_bindings[i].bind2 != -1)
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $1544
line 3294
;3294:				DC->setBinding( g_bindings[i].bind2, g_bindings[i].command );
ADDRLP4 8
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ADDRGP4 g_bindings
ADDP4
INDIRP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
LABELV $1544
line 3295
;3295:		}
LABELV $1540
line 3296
;3296:	}
LABELV $1537
line 3286
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1539
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1536
line 3310
;3297:
;3298:	//if ( s_controls.invertmouse.curvalue )
;3299:	//	DC->setCVar("m_pitch", va("%f),-fabs( DC->getCVarValue( "m_pitch" ) ) );
;3300:	//else
;3301:	//	trap_Cvar_SetValue( "m_pitch", fabs( trap_Cvar_VariableValue( "m_pitch" ) ) );
;3302:
;3303:	//trap_Cvar_SetValue( "m_filter", s_controls.smoothmouse.curvalue );
;3304:	//trap_Cvar_SetValue( "cl_run", s_controls.alwaysrun.curvalue );
;3305:	//trap_Cvar_SetValue( "cg_autoswitch", s_controls.autoswitch.curvalue );
;3306:	//trap_Cvar_SetValue( "sensitivity", s_controls.sensitivity.curvalue );
;3307:	//trap_Cvar_SetValue( "in_joystick", s_controls.joyenable.curvalue );
;3308:	//trap_Cvar_SetValue( "joy_threshold", s_controls.joythreshold.curvalue );
;3309:	//trap_Cvar_SetValue( "cl_freelook", s_controls.freelook.curvalue );
;3310:	DC->executeText(EXEC_APPEND, "in_restart\n");
CNSTI4 2
ARGI4
ADDRGP4 $1548
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 148
ADDP4
INDIRP4
CALLV
pop
line 3312
;3311:	//trap_Cmd_ExecuteText( EXEC_APPEND, "in_restart\n" );
;3312:}
LABELV $1535
endproc Controls_SetConfig 12 8
export Controls_SetDefaults
proc Controls_SetDefaults 12 0
line 3320
;3313:
;3314:/*
;3315:=================
;3316:Controls_SetDefaults
;3317:=================
;3318:*/
;3319:void Controls_SetDefaults( void )
;3320:{
line 3324
;3321:	int	i;
;3322:
;3323:	// iterate each command, set its default binding
;3324:  for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1553
JUMPV
LABELV $1550
line 3325
;3325:	{
line 3326
;3326:		g_bindings[i].bind1 = g_bindings[i].defaultbind1;
ADDRLP4 4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 g_bindings+16
ADDP4
ADDRLP4 4
INDIRI4
ADDRGP4 g_bindings+8
ADDP4
INDIRI4
ASGNI4
line 3327
;3327:		g_bindings[i].bind2 = g_bindings[i].defaultbind2;
ADDRLP4 8
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 g_bindings+20
ADDP4
ADDRLP4 8
INDIRI4
ADDRGP4 g_bindings+12
ADDP4
INDIRI4
ASGNI4
line 3328
;3328:	}
LABELV $1551
line 3324
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1553
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1550
line 3338
;3329:
;3330:	//s_controls.invertmouse.curvalue  = Controls_GetCvarDefault( "m_pitch" ) < 0;
;3331:	//s_controls.smoothmouse.curvalue  = Controls_GetCvarDefault( "m_filter" );
;3332:	//s_controls.alwaysrun.curvalue    = Controls_GetCvarDefault( "cl_run" );
;3333:	//s_controls.autoswitch.curvalue   = Controls_GetCvarDefault( "cg_autoswitch" );
;3334:	//s_controls.sensitivity.curvalue  = Controls_GetCvarDefault( "sensitivity" );
;3335:	//s_controls.joyenable.curvalue    = Controls_GetCvarDefault( "in_joystick" );
;3336:	//s_controls.joythreshold.curvalue = Controls_GetCvarDefault( "joy_threshold" );
;3337:	//s_controls.freelook.curvalue     = Controls_GetCvarDefault( "cl_freelook" );
;3338:}
LABELV $1549
endproc Controls_SetDefaults 12 0
export BindingIDFromName
proc BindingIDFromName 8 8
line 3340
;3339:
;3340:int BindingIDFromName(const char *name) {
line 3342
;3341:	int i;
;3342:  for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1562
JUMPV
LABELV $1559
line 3343
;3343:	{
line 3344
;3344:		if (Q_stricmp(name, g_bindings[i].command) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $1563
line 3345
;3345:			return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $1558
JUMPV
LABELV $1563
line 3347
;3346:		}
;3347:	}
LABELV $1560
line 3342
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1562
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1559
line 3348
;3348:	return -1;
CNSTI4 -1
RETI4
LABELV $1558
endproc BindingIDFromName 8 8
export BindingFromName
proc BindingFromName 16 12
line 3354
;3349:}
;3350:
;3351:char g_nameBind1[32];
;3352:char g_nameBind2[32];
;3353:
;3354:void BindingFromName(const char *cvar) {
line 3358
;3355:	int	i, b1, b2;
;3356:
;3357:	// iterate each command, set its default binding
;3358:	for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1569
JUMPV
LABELV $1566
line 3359
;3359:	{
line 3360
;3360:		if (Q_stricmp(cvar, g_bindings[i].command) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $1570
line 3361
;3361:			b1 = g_bindings[i].bind1;
ADDRLP4 4
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ASGNI4
line 3362
;3362:			if (b1 == -1) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
NEI4 $1573
line 3363
;3363:				break;
ADDRGP4 $1568
JUMPV
LABELV $1573
line 3365
;3364:			}
;3365:				DC->keynumToStringBuf( b1, g_nameBind1, 32 );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 g_nameBind1
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 136
ADDP4
INDIRP4
CALLV
pop
line 3366
;3366:				Q_strupr(g_nameBind1);
ADDRGP4 g_nameBind1
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 3368
;3367:
;3368:				b2 = g_bindings[i].bind2;
ADDRLP4 8
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ASGNI4
line 3369
;3369:				if (b2 != -1)
ADDRLP4 8
INDIRI4
CNSTI4 -1
EQI4 $1565
line 3370
;3370:				{
line 3371
;3371:					DC->keynumToStringBuf( b2, g_nameBind2, 32 );
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 g_nameBind2
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 136
ADDP4
INDIRP4
CALLV
pop
line 3372
;3372:					Q_strupr(g_nameBind2);
ADDRGP4 g_nameBind2
ARGP4
ADDRGP4 Q_strupr
CALLP4
pop
line 3373
;3373:					strcat( g_nameBind1, " or " );
ADDRGP4 g_nameBind1
ARGP4
ADDRGP4 $1578
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 3374
;3374:					strcat( g_nameBind1, g_nameBind2 );
ADDRGP4 g_nameBind1
ARGP4
ADDRGP4 g_nameBind2
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 3375
;3375:				}
line 3376
;3376:			return;
ADDRGP4 $1565
JUMPV
LABELV $1570
line 3378
;3377:		}
;3378:	}
LABELV $1567
line 3358
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1569
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1566
LABELV $1568
line 3379
;3379:	strcpy(g_nameBind1, "???");
ADDRGP4 g_nameBind1
ARGP4
ADDRGP4 $1579
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 3380
;3380:}
LABELV $1565
endproc BindingFromName 16 12
export Item_Slider_Paint
proc Item_Slider_Paint 68 20
line 3382
;3381:
;3382:void Item_Slider_Paint(itemDef_t *item) {
line 3385
;3383:	vec4_t newColor, lowLight;
;3384:	float x, y, value;
;3385:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3387
;3386:
;3387:	value = (item->cvar) ? DC->getCVarValue(item->cvar) : 0;
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1582
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 48
ADDRLP4 52
INDIRF4
ASGNF4
ADDRGP4 $1583
JUMPV
LABELV $1582
ADDRLP4 48
CNSTF4 0
ASGNF4
LABELV $1583
ADDRLP4 44
ADDRLP4 48
INDIRF4
ASGNF4
line 3389
;3388:
;3389:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1584
line 3390
;3390:		lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 12
CNSTF4 1061997773
ADDRLP4 4
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3391
;3391:		lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 12+4
CNSTF4 1061997773
ADDRLP4 4
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3392
;3392:		lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 12+8
CNSTF4 1061997773
ADDRLP4 4
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3393
;3393:		lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 12+12
CNSTF4 1061997773
ADDRLP4 4
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3394
;3394:		LerpColor(parent->focusColor,lowLight,newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 56
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 28
ARGP4
CNSTF4 1056964608
ADDRLP4 56
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3395
;3395:	} else {
ADDRGP4 $1585
JUMPV
LABELV $1584
line 3396
;3396:		memcpy(&newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3397
;3397:	}
LABELV $1585
line 3399
;3398:
;3399:	y = item->window.rect.y;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ASGNF4
line 3400
;3400:	if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1589
line 3401
;3401:		Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3402
;3402:		x = item->textRect.x + item->textRect.w + 8;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 56
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 3403
;3403:	} else {
ADDRGP4 $1590
JUMPV
LABELV $1589
line 3404
;3404:		x = item->window.rect.x;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
line 3405
;3405:	}
LABELV $1590
line 3406
;3406:	DC->setColor(newColor);
ADDRLP4 28
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLV
pop
line 3407
;3407:	DC->drawHandlePic( x, y, SLIDER_WIDTH, SLIDER_HEIGHT, DC->Assets.sliderBar );
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
CNSTF4 1119879168
ARGF4
CNSTF4 1098907648
ARGF4
ADDRLP4 56
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 61928
ADDP4
INDIRI4
ARGI4
ADDRLP4 56
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3409
;3408:
;3409:	x = Item_Slider_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 Item_Slider_ThumbPosition
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 60
INDIRF4
ASGNF4
line 3410
;3410:	DC->drawHandlePic( x - (SLIDER_THUMB_WIDTH / 2), y - 2, SLIDER_THUMB_WIDTH, SLIDER_THUMB_HEIGHT, DC->Assets.sliderThumb );
ADDRLP4 0
INDIRF4
CNSTF4 1086324736
SUBF4
ARGF4
ADDRLP4 8
INDIRF4
CNSTF4 1073741824
SUBF4
ARGF4
CNSTF4 1094713344
ARGF4
CNSTF4 1101004800
ARGF4
ADDRLP4 64
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 61932
ADDP4
INDIRI4
ARGI4
ADDRLP4 64
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3412
;3411:
;3412:}
LABELV $1580
endproc Item_Slider_Paint 68 20
export Item_Bind_Paint
proc Item_Bind_Paint 64 32
line 3414
;3413:
;3414:void Item_Bind_Paint(itemDef_t *item) {
line 3417
;3415:	vec4_t newColor, lowLight;
;3416:	float value;
;3417:	int maxChars = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 3418
;3418:	menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3419
;3419:	editFieldDef_t *editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 3420
;3420:	if (editPtr) {
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1592
line 3421
;3421:		maxChars = editPtr->maxPaintChars;
ADDRLP4 16
ADDRLP4 24
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ASGNI4
line 3422
;3422:	}
LABELV $1592
line 3424
;3423:
;3424:	value = (item->cvar) ? DC->getCVarValue(item->cvar) : 0;
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1595
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 DC
INDIRP4
CNSTI4 92
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 48
ADDRLP4 52
INDIRF4
ASGNF4
ADDRGP4 $1596
JUMPV
LABELV $1595
ADDRLP4 48
CNSTF4 0
ASGNF4
LABELV $1596
ADDRLP4 44
ADDRLP4 48
INDIRF4
ASGNF4
line 3426
;3425:
;3426:	if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1597
line 3427
;3427:		if (g_bindItem == item) {
ADDRGP4 g_bindItem
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $1599
line 3428
;3428:			lowLight[0] = 0.8f * 1.0f;
ADDRLP4 0
CNSTF4 1061997773
ASGNF4
line 3429
;3429:			lowLight[1] = 0.8f * 0.0f;
ADDRLP4 0+4
CNSTF4 0
ASGNF4
line 3430
;3430:			lowLight[2] = 0.8f * 0.0f;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 3431
;3431:			lowLight[3] = 0.8f * 1.0f;
ADDRLP4 0+12
CNSTF4 1061997773
ASGNF4
line 3432
;3432:		} else {
ADDRGP4 $1600
JUMPV
LABELV $1599
line 3433
;3433:			lowLight[0] = 0.8f * parent->focusColor[0]; 
ADDRLP4 0
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3434
;3434:			lowLight[1] = 0.8f * parent->focusColor[1]; 
ADDRLP4 0+4
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3435
;3435:			lowLight[2] = 0.8f * parent->focusColor[2]; 
ADDRLP4 0+8
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3436
;3436:			lowLight[3] = 0.8f * parent->focusColor[3]; 
ADDRLP4 0+12
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3437
;3437:		}
LABELV $1600
line 3438
;3438:		LerpColor(parent->focusColor,lowLight,newColor,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 56
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 20
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 28
ARGP4
CNSTF4 1056964608
ADDRLP4 56
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3439
;3439:	} else {
ADDRGP4 $1598
JUMPV
LABELV $1597
line 3440
;3440:		memcpy(&newColor, &item->window.foreColor, sizeof(vec4_t));
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3441
;3441:	}
LABELV $1598
line 3443
;3442:
;3443:	if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1607
line 3444
;3444:		Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3445
;3445:		BindingFromName(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRGP4 BindingFromName
CALLV
pop
line 3446
;3446:		DC->drawText(item->textRect.x + item->textRect.w + 8, item->textRect.y, item->textscale, newColor, g_nameBind1, 0, maxChars, item->textStyle);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ARGF4
ADDRLP4 56
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 56
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 28
ARGP4
ADDRGP4 g_nameBind1
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 56
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3447
;3447:	} else {
ADDRGP4 $1608
JUMPV
LABELV $1607
line 3448
;3448:		DC->drawText(item->textRect.x, item->textRect.y, item->textscale, newColor, (value != 0) ? "FIXME" : "FIXME", 0, maxChars, item->textStyle);
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 184
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 28
ARGP4
ADDRLP4 44
INDIRF4
CNSTF4 0
EQF4 $1611
ADDRLP4 56
ADDRGP4 $1609
ASGNP4
ADDRGP4 $1612
JUMPV
LABELV $1611
ADDRLP4 56
ADDRGP4 $1609
ASGNP4
LABELV $1612
ADDRLP4 56
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 16
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3449
;3449:	}
LABELV $1608
line 3450
;3450:}
LABELV $1591
endproc Item_Bind_Paint 64 32
export Display_KeyBindPending
proc Display_KeyBindPending 0 0
line 3452
;3451:
;3452:qboolean Display_KeyBindPending() {
line 3453
;3453:	return g_waitingForKey;
ADDRGP4 g_waitingForKey
INDIRI4
RETI4
LABELV $1613
endproc Display_KeyBindPending 0 0
export Item_Bind_HandleKey
proc Item_Bind_HandleKey 32 12
line 3456
;3454:}
;3455:
;3456:qboolean Item_Bind_HandleKey(itemDef_t *item, int key, qboolean down) {
line 3460
;3457:	int			id;
;3458:	int			i;
;3459:
;3460:	if (Rect_ContainsPoint(&item->window.rect, DC->cursorx, DC->cursory) && !g_waitingForKey)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $1615
ADDRGP4 g_waitingForKey
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $1615
line 3461
;3461:	{
line 3462
;3462:		if (down && (key == K_MOUSE1 || key == K_ENTER)) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $1617
ADDRLP4 20
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 178
EQI4 $1619
ADDRLP4 20
INDIRI4
CNSTI4 13
NEI4 $1617
LABELV $1619
line 3463
;3463:			g_waitingForKey = qtrue;
ADDRGP4 g_waitingForKey
CNSTI4 1
ASGNI4
line 3464
;3464:			g_bindItem = item;
ADDRGP4 g_bindItem
ADDRFP4 0
INDIRP4
ASGNP4
line 3465
;3465:		}
LABELV $1617
line 3466
;3466:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1615
line 3469
;3467:	}
;3468:	else
;3469:	{
line 3470
;3470:		if (!g_waitingForKey || g_bindItem == NULL) {
ADDRGP4 g_waitingForKey
INDIRI4
CNSTI4 0
EQI4 $1622
ADDRGP4 g_bindItem
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1620
LABELV $1622
line 3471
;3471:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1620
line 3474
;3472:		}
;3473:
;3474:		if (key & K_CHAR_FLAG) {
ADDRFP4 4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1623
line 3475
;3475:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1623
line 3478
;3476:		}
;3477:
;3478:		switch (key)
ADDRLP4 20
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 24
CNSTI4 96
ASGNI4
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $1633
ADDRLP4 20
INDIRI4
ADDRLP4 24
INDIRI4
GTI4 $1635
LABELV $1634
ADDRFP4 4
INDIRI4
CNSTI4 27
EQI4 $1627
ADDRGP4 $1625
JUMPV
LABELV $1635
ADDRFP4 4
INDIRI4
CNSTI4 127
EQI4 $1628
ADDRGP4 $1625
JUMPV
line 3479
;3479:		{
LABELV $1627
line 3481
;3480:			case K_ESCAPE:
;3481:				g_waitingForKey = qfalse;
ADDRGP4 g_waitingForKey
CNSTI4 0
ASGNI4
line 3482
;3482:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1628
line 3485
;3483:	
;3484:			case K_BACKSPACE:
;3485:				id = BindingIDFromName(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 BindingIDFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 28
INDIRI4
ASGNI4
line 3486
;3486:				if (id != -1) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
EQI4 $1629
line 3487
;3487:					g_bindings[id].bind1 = -1;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
CNSTI4 -1
ASGNI4
line 3488
;3488:					g_bindings[id].bind2 = -1;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
CNSTI4 -1
ASGNI4
line 3489
;3489:				}
LABELV $1629
line 3490
;3490:				Controls_SetConfig(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 Controls_SetConfig
CALLV
pop
line 3491
;3491:				g_waitingForKey = qfalse;
ADDRGP4 g_waitingForKey
CNSTI4 0
ASGNI4
line 3492
;3492:				g_bindItem = NULL;
ADDRGP4 g_bindItem
CNSTP4 0
ASGNP4
line 3493
;3493:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1633
line 3496
;3494:
;3495:			case '`':
;3496:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1614
JUMPV
LABELV $1625
line 3498
;3497:		}
;3498:	}
line 3500
;3499:
;3500:	if (key != -1)
ADDRFP4 4
INDIRI4
CNSTI4 -1
EQI4 $1636
line 3501
;3501:	{
line 3503
;3502:
;3503:		for (i=0; i < g_bindCount; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1641
JUMPV
LABELV $1638
line 3504
;3504:		{
line 3506
;3505:
;3506:			if (g_bindings[i].bind2 == key) {
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $1642
line 3507
;3507:				g_bindings[i].bind2 = -1;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
CNSTI4 -1
ASGNI4
line 3508
;3508:			}
LABELV $1642
line 3510
;3509:
;3510:			if (g_bindings[i].bind1 == key)
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $1646
line 3511
;3511:			{
line 3512
;3512:				g_bindings[i].bind1 = g_bindings[i].bind2;
ADDRLP4 20
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRGP4 g_bindings+16
ADDP4
ADDRLP4 20
INDIRI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ASGNI4
line 3513
;3513:				g_bindings[i].bind2 = -1;
CNSTI4 24
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
CNSTI4 -1
ASGNI4
line 3514
;3514:			}
LABELV $1646
line 3515
;3515:		}
LABELV $1639
line 3503
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1641
ADDRLP4 0
INDIRI4
ADDRGP4 g_bindCount
INDIRI4
LTI4 $1638
line 3516
;3516:	}
LABELV $1636
line 3519
;3517:
;3518:
;3519:	id = BindingIDFromName(item->cvar);
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 BindingIDFromName
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 20
INDIRI4
ASGNI4
line 3521
;3520:
;3521:	if (id != -1) {
ADDRLP4 4
INDIRI4
CNSTI4 -1
EQI4 $1652
line 3522
;3522:		if (key == -1) {
ADDRFP4 4
INDIRI4
CNSTI4 -1
NEI4 $1654
line 3523
;3523:			if( g_bindings[id].bind1 != -1 ) {
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $1656
line 3524
;3524:				DC->setBinding( g_bindings[id].bind1, "" );
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ARGI4
ADDRGP4 $54
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
line 3525
;3525:				g_bindings[id].bind1 = -1;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
CNSTI4 -1
ASGNI4
line 3526
;3526:			}
LABELV $1656
line 3527
;3527:			if( g_bindings[id].bind2 != -1 ) {
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $1655
line 3528
;3528:				DC->setBinding( g_bindings[id].bind2, "" );
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ARGI4
ADDRGP4 $54
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
line 3529
;3529:				g_bindings[id].bind2 = -1;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
CNSTI4 -1
ASGNI4
line 3530
;3530:			}
line 3531
;3531:		}
ADDRGP4 $1655
JUMPV
LABELV $1654
line 3532
;3532:		else if (g_bindings[id].bind1 == -1) {
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1666
line 3533
;3533:			g_bindings[id].bind1 = key;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 3534
;3534:		}
ADDRGP4 $1667
JUMPV
LABELV $1666
line 3535
;3535:		else if (g_bindings[id].bind1 != key && g_bindings[id].bind2 == -1) {
ADDRLP4 24
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
EQI4 $1670
ADDRLP4 24
INDIRI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $1670
line 3536
;3536:			g_bindings[id].bind2 = key;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 3537
;3537:		}
ADDRGP4 $1671
JUMPV
LABELV $1670
line 3538
;3538:		else {
line 3539
;3539:			DC->setBinding( g_bindings[id].bind1, "" );
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
INDIRI4
ARGI4
ADDRGP4 $54
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
line 3540
;3540:			DC->setBinding( g_bindings[id].bind2, "" );
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
INDIRI4
ARGI4
ADDRGP4 $54
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 144
ADDP4
INDIRP4
CALLV
pop
line 3541
;3541:			g_bindings[id].bind1 = key;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+16
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 3542
;3542:			g_bindings[id].bind2 = -1;
CNSTI4 24
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_bindings+20
ADDP4
CNSTI4 -1
ASGNI4
line 3543
;3543:		}						
LABELV $1671
LABELV $1667
LABELV $1655
line 3544
;3544:	}
LABELV $1652
line 3546
;3545:
;3546:	Controls_SetConfig(qtrue);	
CNSTI4 1
ARGI4
ADDRGP4 Controls_SetConfig
CALLV
pop
line 3547
;3547:	g_waitingForKey = qfalse;
ADDRGP4 g_waitingForKey
CNSTI4 0
ASGNI4
line 3549
;3548:
;3549:	return qtrue;
CNSTI4 1
RETI4
LABELV $1614
endproc Item_Bind_HandleKey 32 12
export AdjustFrom640
proc AdjustFrom640 16 0
line 3554
;3550:}
;3551:
;3552:
;3553:
;3554:void AdjustFrom640(float *x, float *y, float *w, float *h) {
line 3556
;3555:	//*x = *x * DC->scale + DC->bias;
;3556:	*x *= DC->xscale;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRGP4 DC
INDIRP4
CNSTI4 200
ADDP4
INDIRF4
MULF4
ASGNF4
line 3557
;3557:	*y *= DC->yscale;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRGP4 DC
INDIRP4
CNSTI4 196
ADDP4
INDIRF4
MULF4
ASGNF4
line 3558
;3558:	*w *= DC->xscale;
ADDRLP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRGP4 DC
INDIRP4
CNSTI4 200
ADDP4
INDIRF4
MULF4
ASGNF4
line 3559
;3559:	*h *= DC->yscale;
ADDRLP4 12
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRGP4 DC
INDIRP4
CNSTI4 196
ADDP4
INDIRF4
MULF4
ASGNF4
line 3560
;3560:}
LABELV $1679
endproc AdjustFrom640 16 0
export Item_Model_Paint
proc Item_Model_Paint 588 16
line 3562
;3561:
;3562:void Item_Model_Paint(itemDef_t *item) {
line 3568
;3563:	float x, y, w, h;
;3564:	refdef_t refdef;
;3565:	refEntity_t		ent;
;3566:	vec3_t			mins, maxs, origin;
;3567:	vec3_t			angles;
;3568:	modelDef_t *modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 508
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 3570
;3569:
;3570:	if (modelPtr == NULL) {
ADDRLP4 508
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1681
line 3571
;3571:		return;
ADDRGP4 $1680
JUMPV
LABELV $1681
line 3575
;3572:	}
;3573:
;3574:	// setup the refdef
;3575:	memset( &refdef, 0, sizeof( refdef ) );
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 368
ARGI4
ADDRGP4 memset
CALLP4
pop
line 3576
;3576:	refdef.rdflags = RDF_NOWORLDMODEL;
ADDRLP4 0+76
CNSTI4 1
ASGNI4
line 3577
;3577:	AxisClear( refdef.viewaxis );
ADDRLP4 0+36
ARGP4
ADDRGP4 AxisClear
CALLV
pop
line 3578
;3578:	x = item->window.rect.x+1;
ADDRLP4 568
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3579
;3579:	y = item->window.rect.y+1;
ADDRLP4 572
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3580
;3580:	w = item->window.rect.w-2;
ADDRLP4 524
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 3581
;3581:	h = item->window.rect.h-2;
ADDRLP4 528
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 3583
;3582:
;3583:	AdjustFrom640( &x, &y, &w, &h );
ADDRLP4 568
ARGP4
ADDRLP4 572
ARGP4
ADDRLP4 524
ARGP4
ADDRLP4 528
ARGP4
ADDRGP4 AdjustFrom640
CALLV
pop
line 3585
;3584:
;3585:	refdef.x = x;
ADDRLP4 0
ADDRLP4 568
INDIRF4
CVFI4 4
ASGNI4
line 3586
;3586:	refdef.y = y;
ADDRLP4 0+4
ADDRLP4 572
INDIRF4
CVFI4 4
ASGNI4
line 3587
;3587:	refdef.width = w;
ADDRLP4 0+8
ADDRLP4 524
INDIRF4
CVFI4 4
ASGNI4
line 3588
;3588:	refdef.height = h;
ADDRLP4 0+12
ADDRLP4 528
INDIRF4
CVFI4 4
ASGNI4
line 3590
;3589:
;3590:	DC->modelBounds( item->asset, mins, maxs );
ADDRFP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
ARGI4
ADDRLP4 544
ARGP4
ADDRLP4 556
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
CALLV
pop
line 3592
;3591:
;3592:	origin[2] = -0.5 * ( mins[2] + maxs[2] );
ADDRLP4 512+8
CNSTF4 3204448256
ADDRLP4 544+8
INDIRF4
ADDRLP4 556+8
INDIRF4
ADDF4
MULF4
ASGNF4
line 3593
;3593:	origin[1] = 0.5 * ( mins[1] + maxs[1] );
ADDRLP4 512+4
CNSTF4 1056964608
ADDRLP4 544+4
INDIRF4
ADDRLP4 556+4
INDIRF4
ADDF4
MULF4
ASGNF4
line 3596
;3594:
;3595:	// calculate distance so the model nearly fills the box
;3596:	if (qtrue) {
line 3597
;3597:		float len = 0.5 * ( maxs[2] - mins[2] );		
ADDRLP4 576
CNSTF4 1056964608
ADDRLP4 556+8
INDIRF4
ADDRLP4 544+8
INDIRF4
SUBF4
MULF4
ASGNF4
line 3598
;3598:		origin[0] = len / 0.268;	// len / tan( fov/2 )
ADDRLP4 512
ADDRLP4 576
INDIRF4
CNSTF4 1049179980
DIVF4
ASGNF4
line 3600
;3599:		//origin[0] = len / tan(w/2);
;3600:	} else {
ADDRGP4 $1695
JUMPV
LABELV $1694
line 3601
;3601:		origin[0] = item->textscale;
ADDRLP4 512
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ASGNF4
line 3602
;3602:	}
LABELV $1695
line 3603
;3603:	refdef.fov_x = (modelPtr->fov_x) ? modelPtr->fov_x : w;
ADDRLP4 508
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
CNSTF4 0
EQF4 $1700
ADDRLP4 576
ADDRLP4 508
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ASGNF4
ADDRGP4 $1701
JUMPV
LABELV $1700
ADDRLP4 576
ADDRLP4 524
INDIRF4
ASGNF4
LABELV $1701
ADDRLP4 0+16
ADDRLP4 576
INDIRF4
ASGNF4
line 3604
;3604:	refdef.fov_y = (modelPtr->fov_y) ? modelPtr->fov_y : h;
ADDRLP4 508
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
CNSTF4 0
EQF4 $1704
ADDRLP4 580
ADDRLP4 508
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ASGNF4
ADDRGP4 $1705
JUMPV
LABELV $1704
ADDRLP4 580
ADDRLP4 528
INDIRF4
ASGNF4
LABELV $1705
ADDRLP4 0+20
ADDRLP4 580
INDIRF4
ASGNF4
line 3611
;3605:
;3606:	//refdef.fov_x = (int)((float)refdef.width / 640.0f * 90.0f);
;3607:	//xx = refdef.width / tan( refdef.fov_x / 360 * M_PI );
;3608:	//refdef.fov_y = atan2( refdef.height, xx );
;3609:	//refdef.fov_y *= ( 360 / M_PI );
;3610:
;3611:	DC->clearScene();
ADDRGP4 DC
INDIRP4
CNSTI4 52
ADDP4
INDIRP4
CALLV
pop
line 3613
;3612:
;3613:	refdef.time = DC->realTime;
ADDRLP4 0+72
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ASGNI4
line 3617
;3614:
;3615:	// add the model
;3616:
;3617:	memset( &ent, 0, sizeof(ent) );
ADDRLP4 368
ARGP4
CNSTI4 0
ARGI4
CNSTI4 140
ARGI4
ADDRGP4 memset
CALLP4
pop
line 3624
;3618:
;3619:	//adjust = 5.0 * sin( (float)uis.realtime / 500 );
;3620:	//adjust = 360 % (int)((float)uis.realtime / 1000);
;3621:	//VectorSet( angles, 0, 0, 1 );
;3622:
;3623:	// use item storage to track
;3624:	if (modelPtr->rotationSpeed) {
ADDRLP4 508
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 0
EQI4 $1707
line 3625
;3625:		if (DC->realTime > item->window.nextTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LEI4 $1709
line 3626
;3626:			item->window.nextTime = DC->realTime + modelPtr->rotationSpeed;
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 508
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ADDI4
ASGNI4
line 3627
;3627:			modelPtr->angle = (int)(modelPtr->angle + 1) % 360;
ADDRLP4 508
INDIRP4
ADDRLP4 508
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 360
MODI4
ASGNI4
line 3628
;3628:		}
LABELV $1709
line 3629
;3629:	}
LABELV $1707
line 3630
;3630:	VectorSet( angles, 0, modelPtr->angle, 0 );
ADDRLP4 532
CNSTF4 0
ASGNF4
ADDRLP4 532+4
ADDRLP4 508
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 532+8
CNSTF4 0
ASGNF4
line 3631
;3631:	AnglesToAxis( angles, ent.axis );
ADDRLP4 532
ARGP4
ADDRLP4 368+28
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 3633
;3632:
;3633:	ent.hModel = item->asset;
ADDRLP4 368+8
ADDRFP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
ASGNI4
line 3634
;3634:	VectorCopy( origin, ent.origin );
ADDRLP4 368+68
ADDRLP4 512
INDIRB
ASGNB 12
line 3635
;3635:	VectorCopy( origin, ent.lightingOrigin );
ADDRLP4 368+12
ADDRLP4 512
INDIRB
ASGNB 12
line 3636
;3636:	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;
ADDRLP4 368+4
CNSTI4 192
ASGNI4
line 3637
;3637:	VectorCopy( ent.origin, ent.oldorigin );
ADDRLP4 368+84
ADDRLP4 368+68
INDIRB
ASGNB 12
line 3639
;3638:
;3639:	DC->addRefEntityToScene( &ent );
ADDRLP4 368
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 56
ADDP4
INDIRP4
CALLV
pop
line 3640
;3640:	DC->renderScene( &refdef );
ADDRLP4 0
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 60
ADDP4
INDIRP4
CALLV
pop
line 3642
;3641:
;3642:}
LABELV $1680
endproc Item_Model_Paint 588 16
export Item_Image_Paint
proc Item_Image_Paint 16 20
line 3645
;3643:
;3644:
;3645:void Item_Image_Paint(itemDef_t *item) {
line 3646
;3646:	if (item == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1721
line 3647
;3647:		return;
ADDRGP4 $1720
JUMPV
LABELV $1721
line 3649
;3648:	}
;3649:	DC->drawHandlePic(item->window.rect.x+1, item->window.rect.y+1, item->window.rect.w-2, item->window.rect.h-2, item->asset);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRLP4 4
INDIRF4
ADDF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
ADDF4
ARGF4
ADDRLP4 8
CNSTI4 8
ASGNI4
ADDRLP4 12
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRP4
CALLV
pop
line 3650
;3650:}
LABELV $1720
endproc Item_Image_Paint 16 20
export Item_ListBox_Paint
proc Item_ListBox_Paint 104 32
line 3652
;3651:
;3652:void Item_ListBox_Paint(itemDef_t *item) {
line 3656
;3653:	float x, y, size, count, i, thumb;
;3654:	qhandle_t image;
;3655:	qhandle_t optionalImage;
;3656:	listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 3662
;3657:
;3658:	// the listbox is horizontal or vertical and has a fixed size scroll bar going either direction
;3659:	// elements are enumerated from the DC and either text or image handles are acquired from the DC as well
;3660:	// textscale is used to size the text, textalignx and textaligny are used to size image elements
;3661:	// there is no clipping available so only the last completely visible item is painted
;3662:	count = DC->feederCount(item->special);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 DC
INDIRP4
CNSTI4 120
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 28
ADDRLP4 36
INDIRI4
CVIF4 4
ASGNF4
line 3664
;3663:	// default is vertical if horizontal flag is not here
;3664:	if (item->window.flags & WINDOW_HORIZONTAL) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $1724
line 3667
;3665:		// draw scrollbar in bottom of the window
;3666:		// bar
;3667:		x = item->window.rect.x + 1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3668
;3668:		y = item->window.rect.y + item->window.rect.h - SCROLLBAR_SIZE - 1;
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 3669
;3669:		DC->drawHandlePic(x, y, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarArrowLeft);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 44
CNSTF4 1098907648
ASGNF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 61900
ADDP4
INDIRI4
ARGI4
ADDRLP4 48
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3670
;3670:		x += SCROLLBAR_SIZE - 1;
ADDRLP4 16
ADDRLP4 16
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 3671
;3671:		size = item->window.rect.w - (SCROLLBAR_SIZE * 2);
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
ASGNF4
line 3672
;3672:		DC->drawHandlePic(x, y, size+1, SCROLLBAR_SIZE, DC->Assets.scrollBar);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
CNSTF4 1065353216
ADDF4
ARGF4
CNSTF4 1098907648
ARGF4
ADDRLP4 52
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 61908
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3673
;3673:		x += size - 1;
ADDRLP4 16
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
CNSTF4 1065353216
SUBF4
ADDF4
ASGNF4
line 3674
;3674:		DC->drawHandlePic(x, y, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarArrowRight);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 56
CNSTF4 1098907648
ASGNF4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 60
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 61904
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3676
;3675:		// thumb
;3676:		thumb = Item_ListBox_ThumbDrawPosition(item);//Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 Item_ListBox_ThumbDrawPosition
CALLI4
ASGNI4
ADDRLP4 32
ADDRLP4 64
INDIRI4
CVIF4 4
ASGNF4
line 3677
;3677:		if (thumb > x - SCROLLBAR_SIZE - 1) {
ADDRLP4 32
INDIRF4
ADDRLP4 16
INDIRF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
LEF4 $1726
line 3678
;3678:			thumb = x - SCROLLBAR_SIZE - 1;
ADDRLP4 32
ADDRLP4 16
INDIRF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 3679
;3679:		}
LABELV $1726
line 3680
;3680:		DC->drawHandlePic(thumb, y, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarThumb);
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 68
CNSTF4 1098907648
ASGNF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 68
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI4 61912
ADDP4
INDIRI4
ARGI4
ADDRLP4 72
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3682
;3681:		//
;3682:		listPtr->endPos = listPtr->startPos;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 3683
;3683:		size = item->window.rect.w - 2;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 3686
;3684:		// items
;3685:		// size contains max available space
;3686:		if (listPtr->elementStyle == LISTBOX_IMAGE) {
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1725
line 3688
;3687:			// fit = 0;
;3688:			x = item->window.rect.x + 1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3689
;3689:			y = item->window.rect.y + 1;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3690
;3690:			for (i = listPtr->startPos; i < count; i++) {
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
ADDRGP4 $1733
JUMPV
LABELV $1730
line 3693
;3691:				// always draw at least one
;3692:				// which may overdraw the box if it is too small for the element
;3693:				image = DC->feederItemImage(item->special, i);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 80
ADDRGP4 DC
INDIRP4
CNSTI4 128
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 80
INDIRI4
ASGNI4
line 3694
;3694:				if (image) {
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $1734
line 3695
;3695:					DC->drawHandlePic(x+1, y+1, listPtr->elementWidth - 2, listPtr->elementHeight - 2, image);
ADDRLP4 84
CNSTF4 1065353216
ASGNF4
ADDRLP4 16
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 92
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 92
INDIRF4
SUBF4
ARGF4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3696
;3696:				}
LABELV $1734
line 3698
;3697:
;3698:				if (i == item->cursorPos) {
ADDRLP4 4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CVIF4 4
NEF4 $1736
line 3699
;3699:					DC->drawRect(x, y, listPtr->elementWidth-1, listPtr->elementHeight-1, item->window.borderSize, item->window.borderColor);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 88
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 88
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 88
INDIRF4
SUBF4
ARGF4
ADDRLP4 92
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRLP4 92
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 3700
;3700:				}
LABELV $1736
line 3702
;3701:
;3702:				size -= listPtr->elementWidth;
ADDRLP4 20
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3703
;3703:				if (size < listPtr->elementWidth) {
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
GEF4 $1738
line 3704
;3704:					listPtr->drawPadding = size; //listPtr->elementWidth - size;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 20
INDIRF4
CVFI4 4
ASGNI4
line 3705
;3705:					break;
ADDRGP4 $1725
JUMPV
LABELV $1738
line 3707
;3706:				}
;3707:				x += listPtr->elementWidth;
ADDRLP4 16
ADDRLP4 16
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3708
;3708:				listPtr->endPos++;
ADDRLP4 84
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3710
;3709:				// fit++;
;3710:			}
LABELV $1731
line 3690
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
LABELV $1733
ADDRLP4 4
INDIRF4
ADDRLP4 28
INDIRF4
LTF4 $1730
line 3711
;3711:		} else {
line 3713
;3712:			//
;3713:		}
line 3714
;3714:	} else {
ADDRGP4 $1725
JUMPV
LABELV $1724
line 3716
;3715:		// draw scrollbar to right side of the window
;3716:		x = item->window.rect.x + item->window.rect.w - SCROLLBAR_SIZE - 1;
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 40
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 3717
;3717:		y = item->window.rect.y + 1;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3718
;3718:		DC->drawHandlePic(x, y, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarArrowUp);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 44
CNSTF4 1098907648
ASGNF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 44
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 61892
ADDP4
INDIRI4
ARGI4
ADDRLP4 48
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3719
;3719:		y += SCROLLBAR_SIZE - 1;
ADDRLP4 12
ADDRLP4 12
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 3721
;3720:
;3721:		listPtr->endPos = listPtr->startPos;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 3722
;3722:		size = item->window.rect.h - (SCROLLBAR_SIZE * 2);
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1107296256
SUBF4
ASGNF4
line 3723
;3723:		DC->drawHandlePic(x, y, SCROLLBAR_SIZE, size+1, DC->Assets.scrollBar);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
CNSTF4 1098907648
ARGF4
ADDRLP4 20
INDIRF4
CNSTF4 1065353216
ADDF4
ARGF4
ADDRLP4 56
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 61908
ADDP4
INDIRI4
ARGI4
ADDRLP4 56
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3724
;3724:		y += size - 1;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 20
INDIRF4
CNSTF4 1065353216
SUBF4
ADDF4
ASGNF4
line 3725
;3725:		DC->drawHandlePic(x, y, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarArrowDown);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 60
CNSTF4 1098907648
ASGNF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 60
INDIRF4
ARGF4
ADDRLP4 64
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 61896
ADDP4
INDIRI4
ARGI4
ADDRLP4 64
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3727
;3726:		// thumb
;3727:		thumb = Item_ListBox_ThumbDrawPosition(item);//Item_ListBox_ThumbPosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 Item_ListBox_ThumbDrawPosition
CALLI4
ASGNI4
ADDRLP4 32
ADDRLP4 68
INDIRI4
CVIF4 4
ASGNF4
line 3728
;3728:		if (thumb > y - SCROLLBAR_SIZE - 1) {
ADDRLP4 32
INDIRF4
ADDRLP4 12
INDIRF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
LEF4 $1740
line 3729
;3729:			thumb = y - SCROLLBAR_SIZE - 1;
ADDRLP4 32
ADDRLP4 12
INDIRF4
CNSTF4 1098907648
SUBF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 3730
;3730:		}
LABELV $1740
line 3731
;3731:		DC->drawHandlePic(x, thumb, SCROLLBAR_SIZE, SCROLLBAR_SIZE, DC->Assets.scrollBarThumb);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 32
INDIRF4
ARGF4
ADDRLP4 72
CNSTF4 1098907648
ASGNF4
ADDRLP4 72
INDIRF4
ARGF4
ADDRLP4 72
INDIRF4
ARGF4
ADDRLP4 76
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CNSTI4 61912
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3734
;3732:
;3733:		// adjust size for item painting
;3734:		size = item->window.rect.h - 2;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
CNSTF4 1073741824
SUBF4
ASGNF4
line 3735
;3735:		if (listPtr->elementStyle == LISTBOX_IMAGE) {
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
CNSTI4 1
NEI4 $1742
line 3737
;3736:			// fit = 0;
;3737:			x = item->window.rect.x + 1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3738
;3738:			y = item->window.rect.y + 1;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3739
;3739:			for (i = listPtr->startPos; i < count; i++) {
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
ADDRGP4 $1747
JUMPV
LABELV $1744
line 3742
;3740:				// always draw at least one
;3741:				// which may overdraw the box if it is too small for the element
;3742:				image = DC->feederItemImage(item->special, i);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 80
ADDRGP4 DC
INDIRP4
CNSTI4 128
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 80
INDIRI4
ASGNI4
line 3743
;3743:				if (image) {
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $1748
line 3744
;3744:					DC->drawHandlePic(x+1, y+1, listPtr->elementWidth - 2, listPtr->elementHeight - 2, image);
ADDRLP4 84
CNSTF4 1065353216
ASGNF4
ADDRLP4 16
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 92
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 92
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 92
INDIRF4
SUBF4
ARGF4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3745
;3745:				}
LABELV $1748
line 3747
;3746:
;3747:				if (i == item->cursorPos) {
ADDRLP4 4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CVIF4 4
NEF4 $1750
line 3748
;3748:					DC->drawRect(x, y, listPtr->elementWidth - 1, listPtr->elementHeight - 1, item->window.borderSize, item->window.borderColor);
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 88
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 88
INDIRF4
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 88
INDIRF4
SUBF4
ARGF4
ADDRLP4 92
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 92
INDIRP4
CNSTI4 64
ADDP4
INDIRF4
ARGF4
ADDRLP4 92
INDIRP4
CNSTI4 144
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 3749
;3749:				}
LABELV $1750
line 3751
;3750:
;3751:				listPtr->endPos++;
ADDRLP4 84
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3752
;3752:				size -= listPtr->elementWidth;
ADDRLP4 20
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3753
;3753:				if (size < listPtr->elementHeight) {
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
GEF4 $1752
line 3754
;3754:					listPtr->drawPadding = listPtr->elementHeight - size;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 20
INDIRF4
SUBF4
CVFI4 4
ASGNI4
line 3755
;3755:					break;
ADDRGP4 $1743
JUMPV
LABELV $1752
line 3757
;3756:				}
;3757:				y += listPtr->elementHeight;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3759
;3758:				// fit++;
;3759:			}
LABELV $1745
line 3739
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
LABELV $1747
ADDRLP4 4
INDIRF4
ADDRLP4 28
INDIRF4
LTF4 $1744
line 3760
;3760:		} else {
ADDRGP4 $1743
JUMPV
LABELV $1742
line 3761
;3761:			x = item->window.rect.x + 1;
ADDRLP4 16
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3762
;3762:			y = item->window.rect.y + 1;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 3763
;3763:			for (i = listPtr->startPos; i < count; i++) {
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI4
CVIF4 4
ASGNF4
ADDRGP4 $1757
JUMPV
LABELV $1754
line 3768
;3764:				const char *text;
;3765:				// always draw at least one
;3766:				// which may overdraw the box if it is too small for the element
;3767:
;3768:				if (listPtr->numColumns > 0) {
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
CNSTI4 0
LEI4 $1758
line 3770
;3769:					int j;
;3770:					for (j = 0; j < listPtr->numColumns; j++) {
ADDRLP4 84
CNSTI4 0
ASGNI4
ADDRGP4 $1763
JUMPV
LABELV $1760
line 3771
;3771:						text = DC->feederItemText(item->special, i, j, &optionalImage);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 84
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 88
ADDRGP4 DC
INDIRP4
CNSTI4 124
ADDP4
INDIRP4
CALLP4
ASGNP4
ADDRLP4 80
ADDRLP4 88
INDIRP4
ASGNP4
line 3772
;3772:						if (optionalImage >= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $1764
line 3773
;3773:							DC->drawHandlePic(x + 4 + listPtr->columnInfo[j].pos, y - 1 + listPtr->elementHeight / 2, listPtr->columnInfo[j].width, listPtr->columnInfo[j].width, optionalImage);
ADDRLP4 96
CNSTI4 12
ADDRLP4 84
INDIRI4
MULI4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ASGNP4
ADDRLP4 16
INDIRF4
CNSTF4 1082130432
ADDF4
ADDRLP4 96
INDIRP4
INDIRI4
CVIF4 4
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
CNSTF4 1065353216
SUBF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
CNSTF4 1073741824
DIVF4
ADDF4
ARGF4
ADDRLP4 100
ADDRLP4 96
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 100
INDIRF4
ARGF4
ADDRLP4 100
INDIRF4
ARGF4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 3774
;3774:						} else if (text) {
ADDRGP4 $1765
JUMPV
LABELV $1764
ADDRLP4 80
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1766
line 3775
;3775:							DC->drawText(x + 4 + listPtr->columnInfo[j].pos, y + listPtr->elementHeight, item->textscale, item->window.foreColor, text, 0, listPtr->columnInfo[j].maxChars, item->textStyle);
ADDRLP4 96
CNSTI4 12
ADDRLP4 84
INDIRI4
MULI4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ASGNP4
ADDRLP4 16
INDIRF4
CNSTF4 1082130432
ADDF4
ADDRLP4 96
INDIRP4
INDIRI4
CVIF4 4
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
ARGF4
ADDRLP4 100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 100
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 100
INDIRP4
CNSTI4 112
ADDP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 96
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 100
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3776
;3776:						}
LABELV $1766
LABELV $1765
line 3777
;3777:					}
LABELV $1761
line 3770
ADDRLP4 84
ADDRLP4 84
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1763
ADDRLP4 84
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
LTI4 $1760
line 3778
;3778:				} else {
ADDRGP4 $1759
JUMPV
LABELV $1758
line 3779
;3779:					text = DC->feederItemText(item->special, i, 0, &optionalImage);
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
CVFI4 4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 84
ADDRGP4 DC
INDIRP4
CNSTI4 124
ADDP4
INDIRP4
CALLP4
ASGNP4
ADDRLP4 80
ADDRLP4 84
INDIRP4
ASGNP4
line 3780
;3780:					if (optionalImage >= 0) {
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $1768
line 3782
;3781:						//DC->drawHandlePic(x + 4 + listPtr->elementHeight, y, listPtr->columnInfo[j].width, listPtr->columnInfo[j].width, optionalImage);
;3782:					} else if (text) {
ADDRGP4 $1769
JUMPV
LABELV $1768
ADDRLP4 80
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1770
line 3783
;3783:						DC->drawText(x + 4, y + listPtr->elementHeight, item->textscale, item->window.foreColor, text, 0, 0, item->textStyle);
ADDRLP4 16
INDIRF4
CNSTF4 1082130432
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
ARGF4
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 88
INDIRP4
CNSTI4 112
ADDP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
CNSTF4 0
ARGF4
CNSTI4 0
ARGI4
ADDRLP4 88
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 3784
;3784:					}
LABELV $1770
LABELV $1769
line 3785
;3785:				}
LABELV $1759
line 3787
;3786:
;3787:				if (i == item->cursorPos) {
ADDRLP4 4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CVIF4 4
NEF4 $1772
line 3788
;3788:					DC->fillRect(x + 2, y + 2, item->window.rect.w - SCROLLBAR_SIZE - 4, listPtr->elementHeight, item->window.outlineColor);
ADDRLP4 84
CNSTF4 1073741824
ASGNF4
ADDRLP4 16
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 12
INDIRF4
ADDRLP4 84
INDIRF4
ADDF4
ARGF4
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1098907648
SUBF4
CNSTF4 1082130432
SUBF4
ARGF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ARGF4
ADDRLP4 88
INDIRP4
CNSTI4 160
ADDP4
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 36
ADDP4
INDIRP4
CALLV
pop
line 3789
;3789:				}
LABELV $1772
line 3791
;3790:
;3791:				size -= listPtr->elementHeight;
ADDRLP4 20
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3792
;3792:				if (size < listPtr->elementHeight) {
ADDRLP4 20
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
GEF4 $1774
line 3793
;3793:					listPtr->drawPadding = listPtr->elementHeight - size;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 20
INDIRF4
SUBF4
CVFI4 4
ASGNI4
line 3794
;3794:					break;
ADDRGP4 $1756
JUMPV
LABELV $1774
line 3796
;3795:				}
;3796:				listPtr->endPos++;
ADDRLP4 84
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3797
;3797:				y += listPtr->elementHeight;
ADDRLP4 12
ADDRLP4 12
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3799
;3798:				// fit++;
;3799:			}
LABELV $1755
line 3763
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
LABELV $1757
ADDRLP4 4
INDIRF4
ADDRLP4 28
INDIRF4
LTF4 $1754
LABELV $1756
line 3800
;3800:		}
LABELV $1743
line 3801
;3801:	}
LABELV $1725
line 3802
;3802:}
LABELV $1723
endproc Item_ListBox_Paint 104 32
export Item_OwnerDraw_Paint
proc Item_OwnerDraw_Paint 68 56
line 3805
;3803:
;3804:
;3805:void Item_OwnerDraw_Paint(itemDef_t *item) {
line 3808
;3806:  menuDef_t *parent;
;3807:
;3808:	if (item == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1777
line 3809
;3809:		return;
ADDRGP4 $1776
JUMPV
LABELV $1777
line 3811
;3810:	}
;3811:  parent = (menuDef_t*)item->parent;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3813
;3812:
;3813:	if (DC->ownerDrawItem) {
ADDRGP4 DC
INDIRP4
CNSTI4 68
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1779
line 3815
;3814:		vec4_t color, lowLight;
;3815:		menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3816
;3816:		Fade(&item->window.flags, &item->window.foreColor[3], parent->fadeClamp, &item->window.nextTime, parent->fadeCycle, qtrue, parent->fadeAmount);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 68
ADDP4
ARGP4
ADDRLP4 40
INDIRP4
CNSTI4 124
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 204
ADDP4
INDIRF4
ARGF4
ADDRLP4 40
INDIRP4
CNSTI4 108
ADDP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 20
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ARGF4
ADDRGP4 Fade
CALLV
pop
line 3817
;3817:		memcpy(&color, &item->window.foreColor, sizeof(color));
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3818
;3818:		if (item->numColors > 0 && DC->getValue) {
ADDRFP4 0
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
CNSTI4 0
LEI4 $1781
ADDRGP4 DC
INDIRP4
CNSTI4 72
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1781
line 3821
;3819:			// if the value is within one of the ranges then set color to that, otherwise leave at default
;3820:			int i;
;3821:			float f = DC->getValue(item->window.ownerDraw);
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 56
ADDRGP4 DC
INDIRP4
CNSTI4 72
ADDP4
INDIRP4
CALLF4
ASGNF4
ADDRLP4 52
ADDRLP4 56
INDIRF4
ASGNF4
line 3822
;3822:			for (i = 0; i < item->numColors; i++) {
ADDRLP4 48
CNSTI4 0
ASGNI4
ADDRGP4 $1786
JUMPV
LABELV $1783
line 3823
;3823:				if (f >= item->colorRanges[i].low && f <= item->colorRanges[i].high) {
ADDRLP4 64
CNSTI4 24
ADDRLP4 48
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 288
ADDP4
ADDP4
ASGNP4
ADDRLP4 52
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
LTF4 $1787
ADDRLP4 52
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
GTF4 $1787
line 3824
;3824:					memcpy(&color, &item->colorRanges[i].color, sizeof(color));
ADDRLP4 4
ARGP4
CNSTI4 24
ADDRLP4 48
INDIRI4
MULI4
ADDRFP4 0
INDIRP4
CNSTI4 288
ADDP4
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3825
;3825:					break;
ADDRGP4 $1785
JUMPV
LABELV $1787
line 3827
;3826:				}
;3827:			}
LABELV $1784
line 3822
ADDRLP4 48
ADDRLP4 48
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1786
ADDRLP4 48
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
LTI4 $1783
LABELV $1785
line 3828
;3828:		}
LABELV $1781
line 3830
;3829:
;3830:		if (item->window.flags & WINDOW_HASFOCUS) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $1789
line 3831
;3831:			lowLight[0] = 0.8 * parent->focusColor[0]; 
ADDRLP4 24
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 228
ADDP4
INDIRF4
MULF4
ASGNF4
line 3832
;3832:			lowLight[1] = 0.8 * parent->focusColor[1]; 
ADDRLP4 24+4
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 232
ADDP4
INDIRF4
MULF4
ASGNF4
line 3833
;3833:			lowLight[2] = 0.8 * parent->focusColor[2]; 
ADDRLP4 24+8
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 236
ADDP4
INDIRF4
MULF4
ASGNF4
line 3834
;3834:			lowLight[3] = 0.8 * parent->focusColor[3]; 
ADDRLP4 24+12
CNSTF4 1061997773
ADDRLP4 20
INDIRP4
CNSTI4 240
ADDP4
INDIRF4
MULF4
ASGNF4
line 3835
;3835:			LerpColor(parent->focusColor,lowLight,color,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 48
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 20
INDIRP4
CNSTI4 228
ADDP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 4
ARGP4
CNSTF4 1056964608
ADDRLP4 48
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3836
;3836:		} else if (item->textStyle == ITEM_TEXTSTYLE_BLINK && !((DC->realTime/BLINK_DIVISOR) & 1)) {
ADDRGP4 $1790
JUMPV
LABELV $1789
ADDRLP4 48
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ADDRLP4 48
INDIRI4
NEI4 $1794
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 200
DIVI4
ADDRLP4 48
INDIRI4
BANDI4
CNSTI4 0
NEI4 $1794
line 3837
;3837:			lowLight[0] = 0.8 * item->window.foreColor[0]; 
ADDRLP4 24
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRF4
MULF4
ASGNF4
line 3838
;3838:			lowLight[1] = 0.8 * item->window.foreColor[1]; 
ADDRLP4 24+4
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
MULF4
ASGNF4
line 3839
;3839:			lowLight[2] = 0.8 * item->window.foreColor[2]; 
ADDRLP4 24+8
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
MULF4
ASGNF4
line 3840
;3840:			lowLight[3] = 0.8 * item->window.foreColor[3]; 
ADDRLP4 24+12
CNSTF4 1061997773
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
MULF4
ASGNF4
line 3841
;3841:			LerpColor(item->window.foreColor,lowLight,color,0.5+0.5*sin(DC->realTime / PULSE_DIVISOR));
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
CNSTI4 75
DIVI4
CVIF4 4
ARGF4
ADDRLP4 52
ADDRGP4 sin
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 4
ARGP4
CNSTF4 1056964608
ADDRLP4 52
INDIRF4
MULF4
CNSTF4 1056964608
ADDF4
ARGF4
ADDRGP4 LerpColor
CALLV
pop
line 3842
;3842:		}
LABELV $1794
LABELV $1790
line 3844
;3843:
;3844:		if (item->cvarFlags & (CVAR_ENABLE | CVAR_DISABLE) && !Item_EnableShowViaCvar(item, CVAR_ENABLE)) {
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $1799
ADDRLP4 52
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 56
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $1799
line 3845
;3845:		  memcpy(color, parent->disableColor, sizeof(vec4_t)); // bk001207 - FIXME: Com_Memcpy
ADDRLP4 4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 244
ADDP4
ARGP4
CNSTI4 16
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 3846
;3846:		}
LABELV $1799
line 3848
;3847:	
;3848:		if (item->text) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1801
line 3849
;3849:			Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 3850
;3850:				if (item->text[0]) {
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $1803
line 3852
;3851:					// +8 is an offset kludge to properly align owner draw items that have text combined with them
;3852:					DC->ownerDrawItem(item->textRect.x + item->textRect.w + 8, item->window.rect.y, item->window.rect.w, item->window.rect.h, 0, item->textaligny, item->window.ownerDraw, item->window.ownerDrawFlags, item->alignment, item->special, item->textscale, color, item->window.background, item->textStyle );
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
CNSTF4 1090519040
ADDF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTF4 0
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 68
ADDP4
INDIRP4
CALLV
pop
line 3853
;3853:				} else {
ADDRGP4 $1802
JUMPV
LABELV $1803
line 3854
;3854:					DC->ownerDrawItem(item->textRect.x + item->textRect.w, item->window.rect.y, item->window.rect.w, item->window.rect.h, 0, item->textaligny, item->window.ownerDraw, item->window.ownerDrawFlags, item->alignment, item->special, item->textscale, color, item->window.background, item->textStyle );
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 180
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 188
ADDP4
INDIRF4
ADDF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTF4 0
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 68
ADDP4
INDIRP4
CALLV
pop
line 3855
;3855:				}
line 3856
;3856:			} else {
ADDRGP4 $1802
JUMPV
LABELV $1801
line 3857
;3857:			DC->ownerDrawItem(item->window.rect.x, item->window.rect.y, item->window.rect.w, item->window.rect.h, item->textalignx, item->textaligny, item->window.ownerDraw, item->window.ownerDrawFlags, item->alignment, item->special, item->textscale, color, item->window.background, item->textStyle );
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 212
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRLP4 60
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 68
ADDP4
INDIRP4
CALLV
pop
line 3858
;3858:		}
LABELV $1802
line 3859
;3859:	}
LABELV $1779
line 3860
;3860:}
LABELV $1776
endproc Item_OwnerDraw_Paint 68 56
export Item_Paint
proc Item_Paint 84 24
line 3863
;3861:
;3862:
;3863:void Item_Paint(itemDef_t *item) {
line 3865
;3864:  vec4_t red;
;3865:  menuDef_t *parent = (menuDef_t*)item->parent;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 228
ADDP4
INDIRP4
ASGNP4
line 3866
;3866:  red[0] = red[3] = 1;
ADDRLP4 20
CNSTF4 1065353216
ASGNF4
ADDRLP4 0+12
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
line 3867
;3867:  red[1] = red[2] = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 24
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 24
INDIRF4
ASGNF4
line 3869
;3868:
;3869:  if (item == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1809
line 3870
;3870:    return;
ADDRGP4 $1805
JUMPV
LABELV $1809
line 3873
;3871:  }
;3872:
;3873:  if (item->window.flags & WINDOW_ORBITING) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 65536
BANDI4
CNSTI4 0
EQI4 $1811
line 3874
;3874:    if (DC->realTime > item->window.nextTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LEI4 $1813
line 3877
;3875:      float rx, ry, a, c, s, w, h;
;3876:      
;3877:      item->window.nextTime = DC->realTime + item->window.offsetTime;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 108
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 56
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
ADDI4
ASGNI4
line 3879
;3878:      // translate
;3879:      w = item->window.rectClient.w / 2;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 3880
;3880:      h = item->window.rectClient.h / 2;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 3881
;3881:      rx = item->window.rectClient.x + w - item->window.rectEffects.x;
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 60
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 48
INDIRF4
ADDF4
ADDRLP4 60
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3882
;3882:      ry = item->window.rectClient.y + h - item->window.rectEffects.y;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
ADDRLP4 64
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 52
INDIRF4
ADDF4
ADDRLP4 64
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3883
;3883:      a = 3 * M_PI / 180;
ADDRLP4 36
CNSTF4 1029076816
ASGNF4
line 3884
;3884:  	  c = cos(a);
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 68
ADDRGP4 cos
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 68
INDIRF4
ASGNF4
line 3885
;3885:      s = sin(a);
ADDRLP4 36
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 44
ADDRLP4 72
INDIRF4
ASGNF4
line 3886
;3886:      item->window.rectClient.x = (rx * c - ry * s) + item->window.rectEffects.x - w;
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 28
INDIRF4
ADDRLP4 40
INDIRF4
MULF4
ADDRLP4 32
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
SUBF4
ADDRLP4 76
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ADDF4
ADDRLP4 48
INDIRF4
SUBF4
ASGNF4
line 3887
;3887:      item->window.rectClient.y = (rx * s + ry * c) + item->window.rectEffects.y - h;
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 80
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 28
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDRLP4 32
INDIRF4
ADDRLP4 40
INDIRF4
MULF4
ADDF4
ADDRLP4 80
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ADDF4
ADDRLP4 52
INDIRF4
SUBF4
ASGNF4
line 3888
;3888:      Item_UpdatePosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_UpdatePosition
CALLV
pop
line 3890
;3889:
;3890:    }
LABELV $1813
line 3891
;3891:  }
LABELV $1811
line 3894
;3892:
;3893:
;3894:  if (item->window.flags & WINDOW_INTRANSITION) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $1815
line 3895
;3895:    if (DC->realTime > item->window.nextTime) {
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
LEI4 $1817
line 3896
;3896:      int done = 0;
ADDRLP4 28
CNSTI4 0
ASGNI4
line 3897
;3897:      item->window.nextTime = DC->realTime + item->window.offsetTime;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 108
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 208
ADDP4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
ADDI4
ASGNI4
line 3899
;3898:			// transition the x,y
;3899:			if (item->window.rectClient.x == item->window.rectEffects.x) {
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
NEF4 $1819
line 3900
;3900:				done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3901
;3901:			} else {
ADDRGP4 $1820
JUMPV
LABELV $1819
line 3902
;3902:				if (item->window.rectClient.x < item->window.rectEffects.x) {
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
GEF4 $1821
line 3903
;3903:					item->window.rectClient.x += item->window.rectEffects2.x;
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
ADDRLP4 44
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 88
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3904
;3904:					if (item->window.rectClient.x > item->window.rectEffects.x) {
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
LEF4 $1822
line 3905
;3905:						item->window.rectClient.x = item->window.rectEffects.x;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 56
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ASGNF4
line 3906
;3906:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3907
;3907:					}
line 3908
;3908:				} else {
ADDRGP4 $1822
JUMPV
LABELV $1821
line 3909
;3909:					item->window.rectClient.x -= item->window.rectEffects2.x;
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
ADDRLP4 44
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 88
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3910
;3910:					if (item->window.rectClient.x < item->window.rectEffects.x) {
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
GEF4 $1825
line 3911
;3911:						item->window.rectClient.x = item->window.rectEffects.x;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 56
INDIRP4
CNSTI4 72
ADDP4
INDIRF4
ASGNF4
line 3912
;3912:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3913
;3913:					}
LABELV $1825
line 3914
;3914:				}
LABELV $1822
line 3915
;3915:			}
LABELV $1820
line 3916
;3916:			if (item->window.rectClient.y == item->window.rectEffects.y) {
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
NEF4 $1827
line 3917
;3917:				done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3918
;3918:			} else {
ADDRGP4 $1828
JUMPV
LABELV $1827
line 3919
;3919:				if (item->window.rectClient.y < item->window.rectEffects.y) {
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
GEF4 $1829
line 3920
;3920:					item->window.rectClient.y += item->window.rectEffects2.y;
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 48
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3921
;3921:					if (item->window.rectClient.y > item->window.rectEffects.y) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
LEF4 $1830
line 3922
;3922:						item->window.rectClient.y = item->window.rectEffects.y;
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 60
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ASGNF4
line 3923
;3923:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3924
;3924:					}
line 3925
;3925:				} else {
ADDRGP4 $1830
JUMPV
LABELV $1829
line 3926
;3926:					item->window.rectClient.y -= item->window.rectEffects2.y;
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
ADDRLP4 48
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3927
;3927:					if (item->window.rectClient.y < item->window.rectEffects.y) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
GEF4 $1833
line 3928
;3928:						item->window.rectClient.y = item->window.rectEffects.y;
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 60
INDIRP4
CNSTI4 76
ADDP4
INDIRF4
ASGNF4
line 3929
;3929:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3930
;3930:					}
LABELV $1833
line 3931
;3931:				}
LABELV $1830
line 3932
;3932:			}
LABELV $1828
line 3933
;3933:			if (item->window.rectClient.w == item->window.rectEffects.w) {
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
NEF4 $1835
line 3934
;3934:				done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3935
;3935:			} else {
ADDRGP4 $1836
JUMPV
LABELV $1835
line 3936
;3936:				if (item->window.rectClient.w < item->window.rectEffects.w) {
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
GEF4 $1837
line 3937
;3937:					item->window.rectClient.w += item->window.rectEffects2.w;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 52
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3938
;3938:					if (item->window.rectClient.w > item->window.rectEffects.w) {
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
LEF4 $1838
line 3939
;3939:						item->window.rectClient.w = item->window.rectEffects.w;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 64
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
ASGNF4
line 3940
;3940:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3941
;3941:					}
line 3942
;3942:				} else {
ADDRGP4 $1838
JUMPV
LABELV $1837
line 3943
;3943:					item->window.rectClient.w -= item->window.rectEffects2.w;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
ADDRLP4 52
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3944
;3944:					if (item->window.rectClient.w < item->window.rectEffects.w) {
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
GEF4 $1841
line 3945
;3945:						item->window.rectClient.w = item->window.rectEffects.w;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 64
INDIRP4
CNSTI4 80
ADDP4
INDIRF4
ASGNF4
line 3946
;3946:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3947
;3947:					}
LABELV $1841
line 3948
;3948:				}
LABELV $1838
line 3949
;3949:			}
LABELV $1836
line 3950
;3950:			if (item->window.rectClient.h == item->window.rectEffects.h) {
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
NEF4 $1843
line 3951
;3951:				done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3952
;3952:			} else {
ADDRGP4 $1844
JUMPV
LABELV $1843
line 3953
;3953:				if (item->window.rectClient.h < item->window.rectEffects.h) {
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
GEF4 $1845
line 3954
;3954:					item->window.rectClient.h += item->window.rectEffects2.h;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
ADDRLP4 56
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDF4
ASGNF4
line 3955
;3955:					if (item->window.rectClient.h > item->window.rectEffects.h) {
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
LEF4 $1846
line 3956
;3956:						item->window.rectClient.h = item->window.rectEffects.h;
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
ASGNF4
line 3957
;3957:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3958
;3958:					}
line 3959
;3959:				} else {
ADDRGP4 $1846
JUMPV
LABELV $1845
line 3960
;3960:					item->window.rectClient.h -= item->window.rectEffects2.h;
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
ADDRLP4 56
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3961
;3961:					if (item->window.rectClient.h < item->window.rectEffects.h) {
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
GEF4 $1849
line 3962
;3962:						item->window.rectClient.h = item->window.rectEffects.h;
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 84
ADDP4
INDIRF4
ASGNF4
line 3963
;3963:						done++;
ADDRLP4 28
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 3964
;3964:					}
LABELV $1849
line 3965
;3965:				}
LABELV $1846
line 3966
;3966:			}
LABELV $1844
line 3968
;3967:
;3968:      Item_UpdatePosition(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_UpdatePosition
CALLV
pop
line 3970
;3969:
;3970:      if (done == 4) {
ADDRLP4 28
INDIRI4
CNSTI4 4
NEI4 $1851
line 3971
;3971:        item->window.flags &= ~WINDOW_INTRANSITION;
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRI4
CNSTI4 -257
BANDI4
ASGNI4
line 3972
;3972:      }
LABELV $1851
line 3974
;3973:
;3974:    }
LABELV $1817
line 3975
;3975:  }
LABELV $1815
line 3977
;3976:
;3977:	if (item->window.ownerDrawFlags && DC->ownerDrawVisible) {
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
CNSTI4 0
EQI4 $1853
ADDRGP4 DC
INDIRP4
CNSTI4 76
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1853
line 3978
;3978:		if (!DC->ownerDrawVisible(item->window.ownerDrawFlags)) {
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 DC
INDIRP4
CNSTI4 76
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $1855
line 3979
;3979:			item->window.flags &= ~WINDOW_VISIBLE;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 -5
BANDI4
ASGNI4
line 3980
;3980:		} else {
ADDRGP4 $1856
JUMPV
LABELV $1855
line 3981
;3981:			item->window.flags |= WINDOW_VISIBLE;
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 3982
;3982:		}
LABELV $1856
line 3983
;3983:	}
LABELV $1853
line 3985
;3984:
;3985:	if (item->cvarFlags & (CVAR_SHOW | CVAR_HIDE)) {
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 12
BANDI4
CNSTI4 0
EQI4 $1857
line 3986
;3986:		if (!Item_EnableShowViaCvar(item, CVAR_SHOW)) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 28
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $1859
line 3987
;3987:			return;
ADDRGP4 $1805
JUMPV
LABELV $1859
line 3989
;3988:		}
;3989:	}
LABELV $1857
line 3991
;3990:
;3991:  if (item->window.flags & WINDOW_TIMEDVISIBLE) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 8388608
BANDI4
CNSTI4 0
EQI4 $1861
line 3993
;3992:
;3993:	}
LABELV $1861
line 3995
;3994:
;3995:  if (!(item->window.flags & WINDOW_VISIBLE)) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $1863
line 3996
;3996:    return;
ADDRGP4 $1805
JUMPV
LABELV $1863
line 4000
;3997:  }
;3998:
;3999:  // paint the rect first.. 
;4000:  Window_Paint(&item->window, parent->fadeAmount , parent->fadeClamp, parent->fadeCycle);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 204
ADDP4
INDIRF4
ARGF4
ADDRLP4 16
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 Window_Paint
CALLV
pop
line 4002
;4001:
;4002:  if (debugMode) {
ADDRGP4 debugMode
INDIRI4
CNSTI4 0
EQI4 $1865
line 4004
;4003:		vec4_t color;
;4004:		rectDef_t *r = Item_CorrectedTextRect(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 Item_CorrectedTextRect
CALLP4
ASGNP4
ADDRLP4 32
ADDRLP4 52
INDIRP4
ASGNP4
line 4005
;4005:    color[1] = color[3] = 1;
ADDRLP4 56
CNSTF4 1065353216
ASGNF4
ADDRLP4 36+12
ADDRLP4 56
INDIRF4
ASGNF4
ADDRLP4 36+4
ADDRLP4 56
INDIRF4
ASGNF4
line 4006
;4006:    color[0] = color[2] = 0;
ADDRLP4 60
CNSTF4 0
ASGNF4
ADDRLP4 36+8
ADDRLP4 60
INDIRF4
ASGNF4
ADDRLP4 36
ADDRLP4 60
INDIRF4
ASGNF4
line 4007
;4007:    DC->drawRect(r->x, r->y, r->w, r->h, 1, color);
ADDRLP4 32
INDIRP4
INDIRF4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 36
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 4008
;4008:  }
LABELV $1865
line 4012
;4009:
;4010:  //DC->drawRect(item->window.rect.x, item->window.rect.y, item->window.rect.w, item->window.rect.h, 1, red);
;4011:
;4012:  switch (item->type) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
LTI4 $1871
ADDRLP4 32
INDIRI4
CNSTI4 13
GTI4 $1871
ADDRLP4 32
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $1885
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $1885
address $1874
address $1874
address $1871
address $1871
address $1877
address $1871
address $1879
address $1880
address $1873
address $1877
address $1884
address $1881
address $1882
address $1883
code
LABELV $1873
line 4014
;4013:    case ITEM_TYPE_OWNERDRAW:
;4014:      Item_OwnerDraw_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_OwnerDraw_Paint
CALLV
pop
line 4015
;4015:      break;
ADDRGP4 $1871
JUMPV
LABELV $1874
line 4018
;4016:    case ITEM_TYPE_TEXT:
;4017:    case ITEM_TYPE_BUTTON:
;4018:      Item_Text_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Text_Paint
CALLV
pop
line 4019
;4019:      break;
ADDRGP4 $1871
JUMPV
line 4021
;4020:    case ITEM_TYPE_RADIOBUTTON:
;4021:      break;
line 4023
;4022:    case ITEM_TYPE_CHECKBOX:
;4023:      break;
LABELV $1877
line 4026
;4024:    case ITEM_TYPE_EDITFIELD:
;4025:    case ITEM_TYPE_NUMERICFIELD:
;4026:      Item_TextField_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_TextField_Paint
CALLV
pop
line 4027
;4027:      break;
ADDRGP4 $1871
JUMPV
line 4029
;4028:    case ITEM_TYPE_COMBO:
;4029:      break;
LABELV $1879
line 4031
;4030:    case ITEM_TYPE_LISTBOX:
;4031:      Item_ListBox_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ListBox_Paint
CALLV
pop
line 4032
;4032:      break;
ADDRGP4 $1871
JUMPV
LABELV $1880
line 4037
;4033:    //case ITEM_TYPE_IMAGE:
;4034:    //  Item_Image_Paint(item);
;4035:    //  break;
;4036:    case ITEM_TYPE_MODEL:
;4037:      Item_Model_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Model_Paint
CALLV
pop
line 4038
;4038:      break;
ADDRGP4 $1871
JUMPV
LABELV $1881
line 4040
;4039:    case ITEM_TYPE_YESNO:
;4040:      Item_YesNo_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_YesNo_Paint
CALLV
pop
line 4041
;4041:      break;
ADDRGP4 $1871
JUMPV
LABELV $1882
line 4043
;4042:    case ITEM_TYPE_MULTI:
;4043:      Item_Multi_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Multi_Paint
CALLV
pop
line 4044
;4044:      break;
ADDRGP4 $1871
JUMPV
LABELV $1883
line 4046
;4045:    case ITEM_TYPE_BIND:
;4046:      Item_Bind_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Bind_Paint
CALLV
pop
line 4047
;4047:      break;
ADDRGP4 $1871
JUMPV
LABELV $1884
line 4049
;4048:    case ITEM_TYPE_SLIDER:
;4049:      Item_Slider_Paint(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_Slider_Paint
CALLV
pop
line 4050
;4050:      break;
line 4052
;4051:    default:
;4052:      break;
LABELV $1871
line 4055
;4053:  }
;4054:
;4055:}
LABELV $1805
endproc Item_Paint 84 24
export Menu_Init
proc Menu_Init 0 12
line 4057
;4056:
;4057:void Menu_Init(menuDef_t *menu) {
line 4058
;4058:	memset(menu, 0, sizeof(menuDef_t));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 644
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4059
;4059:	menu->cursorItem = -1;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
CNSTI4 -1
ASGNI4
line 4060
;4060:	menu->fadeAmount = DC->Assets.fadeAmount;
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 61960
ADDP4
INDIRF4
ASGNF4
line 4061
;4061:	menu->fadeClamp = DC->Assets.fadeClamp;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 61952
ADDP4
INDIRF4
ASGNF4
line 4062
;4062:	menu->fadeCycle = DC->Assets.fadeCycle;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRGP4 DC
INDIRP4
CNSTI4 61956
ADDP4
INDIRI4
ASGNI4
line 4063
;4063:	Window_Init(&menu->window);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Window_Init
CALLV
pop
line 4064
;4064:}
LABELV $1886
endproc Menu_Init 0 12
export Menu_GetFocusedItem
proc Menu_GetFocusedItem 8 0
line 4066
;4065:
;4066:itemDef_t *Menu_GetFocusedItem(menuDef_t *menu) {
line 4068
;4067:  int i;
;4068:  if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1888
line 4069
;4069:    for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1893
JUMPV
LABELV $1890
line 4070
;4070:      if (menu->items[i]->window.flags & WINDOW_HASFOCUS) {
ADDRLP4 4
CNSTI4 2
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
BANDI4
CNSTI4 0
EQI4 $1894
line 4071
;4071:        return menu->items[i];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $1887
JUMPV
LABELV $1894
line 4073
;4072:      }
;4073:    }
LABELV $1891
line 4069
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1893
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1890
line 4074
;4074:  }
LABELV $1888
line 4075
;4075:  return NULL;
CNSTP4 0
RETP4
LABELV $1887
endproc Menu_GetFocusedItem 8 0
export Menu_GetFocused
proc Menu_GetFocused 12 0
line 4078
;4076:}
;4077:
;4078:menuDef_t *Menu_GetFocused() {
line 4080
;4079:  int i;
;4080:  for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1900
JUMPV
LABELV $1897
line 4081
;4081:    if (Menus[i].window.flags & WINDOW_HASFOCUS && Menus[i].window.flags & WINDOW_VISIBLE) {
ADDRLP4 4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 Menus+68
ADDP4
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $1901
ADDRLP4 4
INDIRI4
ADDRGP4 Menus+68
ADDP4
INDIRI4
CNSTI4 4
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $1901
line 4082
;4082:      return &Menus[i];
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
RETP4
ADDRGP4 $1896
JUMPV
LABELV $1901
line 4084
;4083:    }
;4084:  }
LABELV $1898
line 4080
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1900
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1897
line 4085
;4085:  return NULL;
CNSTP4 0
RETP4
LABELV $1896
endproc Menu_GetFocused 12 0
export Menu_ScrollFeeder
proc Menu_ScrollFeeder 12 16
line 4088
;4086:}
;4087:
;4088:void Menu_ScrollFeeder(menuDef_t *menu, int feeder, qboolean down) {
line 4089
;4089:	if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1906
line 4091
;4090:		int i;
;4091:    for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1911
JUMPV
LABELV $1908
line 4092
;4092:			if (menu->items[i]->special == feeder) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ADDRFP4 4
INDIRI4
CVIF4 4
NEF4 $1912
line 4093
;4093:				Item_ListBox_HandleKey(menu->items[i], (down) ? K_DOWNARROW : K_UPARROW, qtrue, qtrue);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $1915
ADDRLP4 4
CNSTI4 133
ASGNI4
ADDRGP4 $1916
JUMPV
LABELV $1915
ADDRLP4 4
CNSTI4 132
ASGNI4
LABELV $1916
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 Item_ListBox_HandleKey
CALLI4
pop
line 4094
;4094:				return;
ADDRGP4 $1905
JUMPV
LABELV $1912
line 4096
;4095:			}
;4096:		}
LABELV $1909
line 4091
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1911
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1908
line 4097
;4097:	}
LABELV $1906
line 4098
;4098:}
LABELV $1905
endproc Menu_ScrollFeeder 12 16
export Menu_SetFeederSelection
proc Menu_SetFeederSelection 8 8
line 4102
;4099:
;4100:
;4101:
;4102:void Menu_SetFeederSelection(menuDef_t *menu, int feeder, int index, const char *name) {
line 4103
;4103:	if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1918
line 4104
;4104:		if (name == NULL) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1920
line 4105
;4105:			menu = Menu_GetFocused();
ADDRLP4 0
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRFP4 0
ADDRLP4 0
INDIRP4
ASGNP4
line 4106
;4106:		} else {
ADDRGP4 $1921
JUMPV
LABELV $1920
line 4107
;4107:			menu = Menus_FindByName(name);
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 Menus_FindByName
CALLP4
ASGNP4
ADDRFP4 0
ADDRLP4 0
INDIRP4
ASGNP4
line 4108
;4108:		}
LABELV $1921
line 4109
;4109:	}
LABELV $1918
line 4111
;4110:
;4111:	if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1922
line 4113
;4112:		int i;
;4113:    for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1927
JUMPV
LABELV $1924
line 4114
;4114:			if (menu->items[i]->special == feeder) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ADDRFP4 4
INDIRI4
CVIF4 4
NEF4 $1928
line 4115
;4115:				if (index == 0) {
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $1930
line 4116
;4116:					listBoxDef_t *listPtr = (listBoxDef_t*)menu->items[i]->typeData;
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4117
;4117:					listPtr->cursorPos = 0;
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 4118
;4118:					listPtr->startPos = 0;
ADDRLP4 4
INDIRP4
CNSTI4 0
ASGNI4
line 4119
;4119:				}
LABELV $1930
line 4120
;4120:				menu->items[i]->cursorPos = index;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 532
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 4121
;4121:				DC->feederSelection(menu->items[i]->special, menu->items[i]->cursorPos);
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 528
ADDP4
INDIRF4
ARGF4
ADDRLP4 4
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 132
ADDP4
INDIRP4
CALLV
pop
line 4122
;4122:				return;
ADDRGP4 $1917
JUMPV
LABELV $1928
line 4124
;4123:			}
;4124:		}
LABELV $1925
line 4113
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1927
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1924
line 4125
;4125:	}
LABELV $1922
line 4126
;4126:}
LABELV $1917
endproc Menu_SetFeederSelection 8 8
export Menus_AnyFullScreenVisible
proc Menus_AnyFullScreenVisible 12 0
line 4128
;4127:
;4128:qboolean Menus_AnyFullScreenVisible() {
line 4130
;4129:  int i;
;4130:  for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1936
JUMPV
LABELV $1933
line 4131
;4131:    if (Menus[i].window.flags & WINDOW_VISIBLE && Menus[i].fullScreen) {
ADDRLP4 4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 Menus+68
ADDP4
INDIRI4
CNSTI4 4
BANDI4
ADDRLP4 8
INDIRI4
EQI4 $1937
ADDRLP4 4
INDIRI4
ADDRGP4 Menus+184
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $1937
line 4132
;4132:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $1932
JUMPV
LABELV $1937
line 4134
;4133:    }
;4134:  }
LABELV $1934
line 4130
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1936
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1933
line 4135
;4135:  return qfalse;
CNSTI4 0
RETI4
LABELV $1932
endproc Menus_AnyFullScreenVisible 12 0
export Menus_ActivateByName
proc Menus_ActivateByName 28 8
line 4138
;4136:}
;4137:
;4138:menuDef_t *Menus_ActivateByName(const char *p) {
line 4140
;4139:  int i;
;4140:  menuDef_t *m = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 4141
;4141:	menuDef_t *focus = Menu_GetFocused();
ADDRLP4 12
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 4142
;4142:  for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1945
JUMPV
LABELV $1942
line 4143
;4143:    if (Q_stricmp(Menus[i].window.name, p) == 0) {
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+32
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $1946
line 4144
;4144:	    m = &Menus[i];
ADDRLP4 4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ASGNP4
line 4145
;4145:			Menus_Activate(m);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Menus_Activate
CALLV
pop
line 4146
;4146:			if (openMenuCount < MAX_OPEN_MENUS && focus != NULL) {
ADDRGP4 openMenuCount
INDIRI4
CNSTI4 16
GEI4 $1947
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1947
line 4147
;4147:				menuStack[openMenuCount++] = focus;
ADDRLP4 24
ADDRGP4 openMenuCount
ASGNP4
ADDRLP4 20
ADDRLP4 24
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRP4
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 menuStack
ADDP4
ADDRLP4 8
INDIRP4
ASGNP4
line 4148
;4148:			}
line 4149
;4149:    } else {
ADDRGP4 $1947
JUMPV
LABELV $1946
line 4150
;4150:      Menus[i].window.flags &= ~WINDOW_HASFOCUS;
ADDRLP4 20
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 -3
BANDI4
ASGNI4
line 4151
;4151:    }
LABELV $1947
line 4152
;4152:  }
LABELV $1943
line 4142
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1945
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $1942
line 4153
;4153:	Display_CloseCinematics();
ADDRGP4 Display_CloseCinematics
CALLV
pop
line 4154
;4154:  return m;
ADDRLP4 4
INDIRP4
RETP4
LABELV $1941
endproc Menus_ActivateByName 28 8
export Item_Init
proc Item_Init 0 12
line 4158
;4155:}
;4156:
;4157:
;4158:void Item_Init(itemDef_t *item) {
line 4159
;4159:	memset(item, 0, sizeof(itemDef_t));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 540
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4160
;4160:	item->textscale = 0.55f;
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
CNSTF4 1057803469
ASGNF4
line 4161
;4161:	Window_Init(&item->window);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Window_Init
CALLV
pop
line 4162
;4162:}
LABELV $1952
endproc Item_Init 0 12
export Menu_HandleMouseMove
proc Menu_HandleMouseMove 52 12
line 4164
;4163:
;4164:void Menu_HandleMouseMove(menuDef_t *menu, float x, float y) {
line 4166
;4165:  int i, pass;
;4166:  qboolean focusSet = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 4169
;4167:
;4168:  itemDef_t *overItem;
;4169:  if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1954
line 4170
;4170:    return;
ADDRGP4 $1953
JUMPV
LABELV $1954
line 4173
;4171:  }
;4172:
;4173:  if (!(menu->window.flags & (WINDOW_VISIBLE | WINDOW_FORCED))) {
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1048580
BANDI4
CNSTI4 0
NEI4 $1956
line 4174
;4174:    return;
ADDRGP4 $1953
JUMPV
LABELV $1956
line 4177
;4175:  }
;4176:
;4177:	if (itemCapture) {
ADDRGP4 itemCapture
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1958
line 4179
;4178:		//Item_MouseMove(itemCapture, x, y);
;4179:		return;
ADDRGP4 $1953
JUMPV
LABELV $1958
line 4182
;4180:	}
;4181:
;4182:	if (g_waitingForKey || g_editingField) {
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 g_waitingForKey
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $1962
ADDRGP4 g_editingField
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $1960
LABELV $1962
line 4183
;4183:		return;
ADDRGP4 $1953
JUMPV
LABELV $1960
line 4188
;4184:	}
;4185:
;4186:  // FIXME: this is the whole issue of focus vs. mouse over.. 
;4187:  // need a better overall solution as i don't like going through everything twice
;4188:  for (pass = 0; pass < 2; pass++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $1963
line 4189
;4189:    for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $1970
JUMPV
LABELV $1967
line 4193
;4190:      // turn off focus each item
;4191:      // menu->items[i].window.flags &= ~WINDOW_HASFOCUS;
;4192:
;4193:      if (!(menu->items[i]->window.flags & (WINDOW_VISIBLE | WINDOW_FORCED))) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1048580
BANDI4
CNSTI4 0
NEI4 $1971
line 4194
;4194:        continue;
ADDRGP4 $1968
JUMPV
LABELV $1971
line 4198
;4195:      }
;4196:
;4197:			// items can be enabled and disabled based on cvars
;4198:			if (menu->items[i]->cvarFlags & (CVAR_ENABLE | CVAR_DISABLE) && !Item_EnableShowViaCvar(menu->items[i], CVAR_ENABLE)) {
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $1973
ADDRLP4 20
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 24
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $1973
line 4199
;4199:				continue;
ADDRGP4 $1968
JUMPV
LABELV $1973
line 4202
;4200:			}
;4201:
;4202:			if (menu->items[i]->cvarFlags & (CVAR_SHOW | CVAR_HIDE) && !Item_EnableShowViaCvar(menu->items[i], CVAR_SHOW)) {
ADDRLP4 28
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 12
BANDI4
CNSTI4 0
EQI4 $1975
ADDRLP4 28
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 32
ADDRGP4 Item_EnableShowViaCvar
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $1975
line 4203
;4203:				continue;
ADDRGP4 $1968
JUMPV
LABELV $1975
line 4208
;4204:			}
;4205:
;4206:
;4207:
;4208:      if (Rect_ContainsPoint(&menu->items[i]->window.rect, x, y)) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $1977
line 4209
;4209:				if (pass == 1) {
ADDRLP4 8
INDIRI4
CNSTI4 1
NEI4 $1978
line 4210
;4210:					overItem = menu->items[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
line 4211
;4211:					if (overItem->type == ITEM_TYPE_TEXT && overItem->text) {
ADDRLP4 4
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 0
NEI4 $1981
ADDRLP4 4
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1981
line 4212
;4212:						if (!Rect_ContainsPoint(Item_CorrectedTextRect(overItem), x, y)) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 44
ADDRGP4 Item_CorrectedTextRect
CALLP4
ASGNP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $1983
line 4213
;4213:							continue;
ADDRGP4 $1968
JUMPV
LABELV $1983
line 4215
;4214:						}
;4215:					}
LABELV $1981
line 4217
;4216:					// if we are over an item
;4217:					if (IsVisible(overItem->window.flags)) {
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 IsVisible
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $1978
line 4219
;4218:						// different one
;4219:						Item_MouseEnter(overItem, x, y);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRGP4 Item_MouseEnter
CALLV
pop
line 4223
;4220:						// Item_SetMouseOver(overItem, qtrue);
;4221:
;4222:						// if item is not a decoration see if it can take focus
;4223:						if (!focusSet) {
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $1978
line 4224
;4224:							focusSet = Item_SetFocus(overItem, x, y);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 48
ADDRGP4 Item_SetFocus
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 48
INDIRI4
ASGNI4
line 4225
;4225:						}
line 4226
;4226:					}
line 4227
;4227:				}
line 4228
;4228:      } else if (menu->items[i]->window.flags & WINDOW_MOUSEOVER) {
ADDRGP4 $1978
JUMPV
LABELV $1977
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $1989
line 4229
;4229:          Item_MouseLeave(menu->items[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_MouseLeave
CALLV
pop
line 4230
;4230:          Item_SetMouseOver(menu->items[i], qfalse);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Item_SetMouseOver
CALLV
pop
line 4231
;4231:      }
LABELV $1989
LABELV $1978
line 4232
;4232:    }
LABELV $1968
line 4189
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $1970
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $1967
line 4233
;4233:  }
LABELV $1964
line 4188
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 2
LTI4 $1963
line 4235
;4234:
;4235:}
LABELV $1953
endproc Menu_HandleMouseMove 52 12
export Menu_Paint
proc Menu_Paint 48 24
line 4237
;4236:
;4237:void Menu_Paint(menuDef_t *menu, qboolean forcePaint) {
line 4240
;4238:	int i;
;4239:
;4240:	if (menu == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $1992
line 4241
;4241:		return;
ADDRGP4 $1991
JUMPV
LABELV $1992
line 4244
;4242:	}
;4243:
;4244:	if (!(menu->window.flags & WINDOW_VISIBLE) &&  !forcePaint) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 4
BANDI4
ADDRLP4 4
INDIRI4
NEI4 $1994
ADDRFP4 4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $1994
line 4245
;4245:		return;
ADDRGP4 $1991
JUMPV
LABELV $1994
line 4248
;4246:	}
;4247:
;4248:	if (menu->window.ownerDrawFlags && DC->ownerDrawVisible && !DC->ownerDrawVisible(menu->window.ownerDrawFlags)) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $1996
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 76
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $1996
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $1996
line 4249
;4249:		return;
ADDRGP4 $1991
JUMPV
LABELV $1996
line 4252
;4250:	}
;4251:	
;4252:	if (forcePaint) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $1998
line 4253
;4253:		menu->window.flags |= WINDOW_FORCED;
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1048576
BORI4
ASGNI4
line 4254
;4254:	}
LABELV $1998
line 4257
;4255:
;4256:	// draw the background if necessary
;4257:	if (menu->fullScreen) {
ADDRFP4 0
INDIRP4
CNSTI4 184
ADDP4
INDIRI4
CNSTI4 0
EQI4 $2000
line 4260
;4258:		// implies a background shader
;4259:		// FIXME: make sure we have a default shader if fullscreen is set with no background
;4260:		DC->drawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menu->window.background );
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 20
INDIRF4
ARGF4
ADDRLP4 20
INDIRF4
ARGF4
CNSTF4 1142947840
ARGF4
CNSTF4 1139802112
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
CALLV
pop
line 4261
;4261:	} else if (menu->window.background) {
ADDRGP4 $2001
JUMPV
LABELV $2000
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
INDIRI4
CNSTI4 0
EQI4 $2002
line 4264
;4262:		// this allows a background shader without being full screen
;4263:		//UI_DrawHandlePic(menu->window.rect.x, menu->window.rect.y, menu->window.rect.w, menu->window.rect.h, menu->backgroundShader);
;4264:	}
LABELV $2002
LABELV $2001
line 4267
;4265:
;4266:	// paint the background and or border
;4267:	Window_Paint(&menu->window, menu->fadeAmount, menu->fadeClamp, menu->fadeCycle );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 208
ADDP4
INDIRF4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 204
ADDP4
INDIRF4
ARGF4
ADDRLP4 20
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 Window_Paint
CALLV
pop
line 4269
;4268:
;4269:	for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2007
JUMPV
LABELV $2004
line 4270
;4270:		Item_Paint(menu->items[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_Paint
CALLV
pop
line 4271
;4271:	}
LABELV $2005
line 4269
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2007
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $2004
line 4273
;4272:
;4273:	if (debugMode) {
ADDRGP4 debugMode
INDIRI4
CNSTI4 0
EQI4 $2008
line 4275
;4274:		vec4_t color;
;4275:		color[0] = color[2] = color[3] = 1;
ADDRLP4 40
CNSTF4 1065353216
ASGNF4
ADDRLP4 24+12
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 24+8
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 24
ADDRLP4 40
INDIRF4
ASGNF4
line 4276
;4276:		color[1] = 0;
ADDRLP4 24+4
CNSTF4 0
ASGNF4
line 4277
;4277:		DC->drawRect(menu->window.rect.x, menu->window.rect.y, menu->window.rect.w, menu->window.rect.h, 1, color);
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ARGF4
ADDRLP4 44
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 24
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CALLV
pop
line 4278
;4278:	}
LABELV $2008
line 4279
;4279:}
LABELV $1991
endproc Menu_Paint 48 24
export Item_ValidateTypeData
proc Item_ValidateTypeData 8 12
line 4286
;4280:
;4281:/*
;4282:===============
;4283:Item_ValidateTypeData
;4284:===============
;4285:*/
;4286:void Item_ValidateTypeData(itemDef_t *item) {
line 4287
;4287:	if (item->typeData) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2014
line 4288
;4288:		return;
ADDRGP4 $2013
JUMPV
LABELV $2014
line 4291
;4289:	}
;4290:
;4291:	if (item->type == ITEM_TYPE_LISTBOX) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 6
NEI4 $2016
line 4292
;4292:		item->typeData = UI_Alloc(sizeof(listBoxDef_t));
CNSTI4 232
ARGI4
ADDRLP4 0
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 4293
;4293:		memset(item->typeData, 0, sizeof(listBoxDef_t));
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 232
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4294
;4294:	} else if (item->type == ITEM_TYPE_EDITFIELD || item->type == ITEM_TYPE_NUMERICFIELD || item->type == ITEM_TYPE_YESNO || item->type == ITEM_TYPE_BIND || item->type == ITEM_TYPE_SLIDER || item->type == ITEM_TYPE_TEXT) {
ADDRGP4 $2017
JUMPV
LABELV $2016
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
EQI4 $2024
ADDRLP4 0
INDIRI4
CNSTI4 9
EQI4 $2024
ADDRLP4 0
INDIRI4
CNSTI4 11
EQI4 $2024
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $2024
ADDRLP4 0
INDIRI4
CNSTI4 10
EQI4 $2024
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2018
LABELV $2024
line 4295
;4295:		item->typeData = UI_Alloc(sizeof(editFieldDef_t));
CNSTI4 28
ARGI4
ADDRLP4 4
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 4296
;4296:		memset(item->typeData, 0, sizeof(editFieldDef_t));
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 28
ARGI4
ADDRGP4 memset
CALLP4
pop
line 4297
;4297:		if (item->type == ITEM_TYPE_EDITFIELD) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 4
NEI4 $2019
line 4298
;4298:			if (!((editFieldDef_t *) item->typeData)->maxPaintChars) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2019
line 4299
;4299:				((editFieldDef_t *) item->typeData)->maxPaintChars = MAX_EDITFIELD;
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CNSTI4 20
ADDP4
CNSTI4 256
ASGNI4
line 4300
;4300:			}
line 4301
;4301:		}
line 4302
;4302:	} else if (item->type == ITEM_TYPE_MULTI) {
ADDRGP4 $2019
JUMPV
LABELV $2018
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 12
NEI4 $2029
line 4303
;4303:		item->typeData = UI_Alloc(sizeof(multiDef_t));
CNSTI4 392
ARGI4
ADDRLP4 4
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 4304
;4304:	} else if (item->type == ITEM_TYPE_MODEL) {
ADDRGP4 $2030
JUMPV
LABELV $2029
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 7
NEI4 $2031
line 4305
;4305:		item->typeData = UI_Alloc(sizeof(modelDef_t));
CNSTI4 28
ARGI4
ADDRLP4 4
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 4306
;4306:	}
LABELV $2031
LABELV $2030
LABELV $2019
LABELV $2017
line 4307
;4307:}
LABELV $2013
endproc Item_ValidateTypeData 8 12
export KeywordHash_Key
proc KeywordHash_Key 20 0
line 4324
;4308:
;4309:/*
;4310:===============
;4311:Keyword Hash
;4312:===============
;4313:*/
;4314:
;4315:#define KEYWORDHASH_SIZE	512
;4316:
;4317:typedef struct keywordHash_s
;4318:{
;4319:	char *keyword;
;4320:	qboolean (*func)(itemDef_t *item, int handle);
;4321:	struct keywordHash_s *next;
;4322:} keywordHash_t;
;4323:
;4324:int KeywordHash_Key(char *keyword) {
line 4327
;4325:	int register hash, i;
;4326:
;4327:	hash = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 4328
;4328:	for (i = 0; keyword[i] != '\0'; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $2037
JUMPV
LABELV $2034
line 4329
;4329:		if (keyword[i] >= 'A' && keyword[i] <= 'Z')
ADDRLP4 8
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 65
LTI4 $2038
ADDRLP4 8
INDIRI4
CNSTI4 90
GTI4 $2038
line 4330
;4330:			hash += (keyword[i] + ('a' - 'A')) * (119 + i);
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 32
ADDI4
ADDRLP4 4
INDIRI4
CNSTI4 119
ADDI4
MULI4
ADDI4
ASGNI4
ADDRGP4 $2039
JUMPV
LABELV $2038
line 4332
;4331:		else
;4332:			hash += keyword[i] * (119 + i);
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI4
CNSTI4 119
ADDI4
MULI4
ADDI4
ASGNI4
LABELV $2039
line 4333
;4333:	}
LABELV $2035
line 4328
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2037
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $2034
line 4334
;4334:	hash = (hash ^ (hash >> 10) ^ (hash >> 20)) & (KEYWORDHASH_SIZE-1);
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 10
RSHI4
BXORI4
ADDRLP4 0
INDIRI4
CNSTI4 20
RSHI4
BXORI4
CNSTI4 511
BANDI4
ASGNI4
line 4335
;4335:	return hash;
ADDRLP4 0
INDIRI4
RETI4
LABELV $2033
endproc KeywordHash_Key 20 0
export KeywordHash_Add
proc KeywordHash_Add 8 4
line 4338
;4336:}
;4337:
;4338:void KeywordHash_Add(keywordHash_t *table[], keywordHash_t *key) {
line 4341
;4339:	int hash;
;4340:
;4341:	hash = KeywordHash_Key(key->keyword);
ADDRFP4 4
INDIRP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 KeywordHash_Key
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 4347
;4342:/*
;4343:	if (table[hash]) {
;4344:		int collision = qtrue;
;4345:	}
;4346:*/
;4347:	key->next = table[hash];
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
line 4348
;4348:	table[hash] = key;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 4349
;4349:}
LABELV $2040
endproc KeywordHash_Add 8 4
export KeywordHash_Find
proc KeywordHash_Find 16 8
line 4352
;4350:
;4351:keywordHash_t *KeywordHash_Find(keywordHash_t *table[], char *keyword)
;4352:{
line 4356
;4353:	keywordHash_t *key;
;4354:	int hash;
;4355:
;4356:	hash = KeywordHash_Key(keyword);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 KeywordHash_Key
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 4357
;4357:	for (key = table[hash]; key; key = key->next) {
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
ADDRGP4 $2045
JUMPV
LABELV $2042
line 4358
;4358:		if (!Q_stricmp(key->keyword, keyword))
ADDRLP4 0
INDIRP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2046
line 4359
;4359:			return key;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $2041
JUMPV
LABELV $2046
line 4360
;4360:	}
LABELV $2043
line 4357
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRP4
ASGNP4
LABELV $2045
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2042
line 4361
;4361:	return NULL;
CNSTP4 0
RETP4
LABELV $2041
endproc KeywordHash_Find 16 8
export ItemParse_name
proc ItemParse_name 4 8
line 4371
;4362:}
;4363:
;4364:/*
;4365:===============
;4366:Item Keyword Parse functions
;4367:===============
;4368:*/
;4369:
;4370:// name <string>
;4371:qboolean ItemParse_name( itemDef_t *item, int handle ) {
line 4372
;4372:	if (!PC_String_Parse(handle, &item->window.name)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2049
line 4373
;4373:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2048
JUMPV
LABELV $2049
line 4375
;4374:	}
;4375:	return qtrue;
CNSTI4 1
RETI4
LABELV $2048
endproc ItemParse_name 4 8
export ItemParse_focusSound
proc ItemParse_focusSound 12 8
line 4379
;4376:}
;4377:
;4378:// name <string>
;4379:qboolean ItemParse_focusSound( itemDef_t *item, int handle ) {
line 4381
;4380:	const char *temp;
;4381:	if (!PC_String_Parse(handle, &temp)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2052
line 4382
;4382:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2051
JUMPV
LABELV $2052
line 4384
;4383:	}
;4384:	item->focusSound = DC->registerSound(temp, qfalse);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
CNSTI4 168
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 280
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 4385
;4385:	return qtrue;
CNSTI4 1
RETI4
LABELV $2051
endproc ItemParse_focusSound 12 8
export ItemParse_text
proc ItemParse_text 4 8
line 4390
;4386:}
;4387:
;4388:
;4389:// text <string>
;4390:qboolean ItemParse_text( itemDef_t *item, int handle ) {
line 4391
;4391:	if (!PC_String_Parse(handle, &item->text)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2055
line 4392
;4392:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2054
JUMPV
LABELV $2055
line 4394
;4393:	}
;4394:	return qtrue;
CNSTI4 1
RETI4
LABELV $2054
endproc ItemParse_text 4 8
export ItemParse_group
proc ItemParse_group 4 8
line 4398
;4395:}
;4396:
;4397:// group <string>
;4398:qboolean ItemParse_group( itemDef_t *item, int handle ) {
line 4399
;4399:	if (!PC_String_Parse(handle, &item->window.group)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2058
line 4400
;4400:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2057
JUMPV
LABELV $2058
line 4402
;4401:	}
;4402:	return qtrue;
CNSTI4 1
RETI4
LABELV $2057
endproc ItemParse_group 4 8
export ItemParse_asset_model
proc ItemParse_asset_model 20 8
line 4406
;4403:}
;4404:
;4405:// asset_model <string>
;4406:qboolean ItemParse_asset_model( itemDef_t *item, int handle ) {
line 4409
;4407:	const char *temp;
;4408:	modelDef_t *modelPtr;
;4409:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4410
;4410:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4412
;4411:
;4412:	if (!PC_String_Parse(handle, &temp)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2061
line 4413
;4413:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2060
JUMPV
LABELV $2061
line 4415
;4414:	}
;4415:	item->asset = DC->registerModel(temp);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 DC
INDIRP4
CNSTI4 28
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 232
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 4416
;4416:	modelPtr->angle = rand() % 360;
ADDRLP4 16
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
ADDRLP4 16
INDIRI4
CNSTI4 360
MODI4
ASGNI4
line 4417
;4417:	return qtrue;
CNSTI4 1
RETI4
LABELV $2060
endproc ItemParse_asset_model 20 8
export ItemParse_asset_shader
proc ItemParse_asset_shader 12 8
line 4421
;4418:}
;4419:
;4420:// asset_shader <string>
;4421:qboolean ItemParse_asset_shader( itemDef_t *item, int handle ) {
line 4424
;4422:	const char *temp;
;4423:
;4424:	if (!PC_String_Parse(handle, &temp)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2064
line 4425
;4425:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2063
JUMPV
LABELV $2064
line 4427
;4426:	}
;4427:	item->asset = DC->registerShaderNoMip(temp);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 232
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 4428
;4428:	return qtrue;
CNSTI4 1
RETI4
LABELV $2063
endproc ItemParse_asset_shader 12 8
export ItemParse_model_origin
proc ItemParse_model_origin 16 8
line 4432
;4429:}
;4430:
;4431:// model_origin <number> <number> <number>
;4432:qboolean ItemParse_model_origin( itemDef_t *item, int handle ) {
line 4434
;4433:	modelDef_t *modelPtr;
;4434:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4435
;4435:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4437
;4436:
;4437:	if (PC_Float_Parse(handle, &modelPtr->origin[0])) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $2067
line 4438
;4438:		if (PC_Float_Parse(handle, &modelPtr->origin[1])) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $2069
line 4439
;4439:			if (PC_Float_Parse(handle, &modelPtr->origin[2])) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2071
line 4440
;4440:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2066
JUMPV
LABELV $2071
line 4442
;4441:			}
;4442:		}
LABELV $2069
line 4443
;4443:	}
LABELV $2067
line 4444
;4444:	return qfalse;
CNSTI4 0
RETI4
LABELV $2066
endproc ItemParse_model_origin 16 8
export ItemParse_model_fovx
proc ItemParse_model_fovx 8 8
line 4448
;4445:}
;4446:
;4447:// model_fovx <number>
;4448:qboolean ItemParse_model_fovx( itemDef_t *item, int handle ) {
line 4450
;4449:	modelDef_t *modelPtr;
;4450:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4451
;4451:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4453
;4452:
;4453:	if (!PC_Float_Parse(handle, &modelPtr->fov_x)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2074
line 4454
;4454:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2073
JUMPV
LABELV $2074
line 4456
;4455:	}
;4456:	return qtrue;
CNSTI4 1
RETI4
LABELV $2073
endproc ItemParse_model_fovx 8 8
export ItemParse_model_fovy
proc ItemParse_model_fovy 8 8
line 4460
;4457:}
;4458:
;4459:// model_fovy <number>
;4460:qboolean ItemParse_model_fovy( itemDef_t *item, int handle ) {
line 4462
;4461:	modelDef_t *modelPtr;
;4462:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4463
;4463:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4465
;4464:
;4465:	if (!PC_Float_Parse(handle, &modelPtr->fov_y)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2077
line 4466
;4466:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2076
JUMPV
LABELV $2077
line 4468
;4467:	}
;4468:	return qtrue;
CNSTI4 1
RETI4
LABELV $2076
endproc ItemParse_model_fovy 8 8
export ItemParse_model_rotation
proc ItemParse_model_rotation 8 8
line 4472
;4469:}
;4470:
;4471:// model_rotation <integer>
;4472:qboolean ItemParse_model_rotation( itemDef_t *item, int handle ) {
line 4474
;4473:	modelDef_t *modelPtr;
;4474:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4475
;4475:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4477
;4476:
;4477:	if (!PC_Int_Parse(handle, &modelPtr->rotationSpeed)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2080
line 4478
;4478:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2079
JUMPV
LABELV $2080
line 4480
;4479:	}
;4480:	return qtrue;
CNSTI4 1
RETI4
LABELV $2079
endproc ItemParse_model_rotation 8 8
export ItemParse_model_angle
proc ItemParse_model_angle 8 8
line 4484
;4481:}
;4482:
;4483:// model_angle <integer>
;4484:qboolean ItemParse_model_angle( itemDef_t *item, int handle ) {
line 4486
;4485:	modelDef_t *modelPtr;
;4486:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4487
;4487:	modelPtr = (modelDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4489
;4488:
;4489:	if (!PC_Int_Parse(handle, &modelPtr->angle)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2083
line 4490
;4490:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2082
JUMPV
LABELV $2083
line 4492
;4491:	}
;4492:	return qtrue;
CNSTI4 1
RETI4
LABELV $2082
endproc ItemParse_model_angle 8 8
export ItemParse_rect
proc ItemParse_rect 4 8
line 4496
;4493:}
;4494:
;4495:// rect <rectangle>
;4496:qboolean ItemParse_rect( itemDef_t *item, int handle ) {
line 4497
;4497:	if (!PC_Rect_Parse(handle, &item->window.rectClient)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Rect_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2086
line 4498
;4498:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2085
JUMPV
LABELV $2086
line 4500
;4499:	}
;4500:	return qtrue;
CNSTI4 1
RETI4
LABELV $2085
endproc ItemParse_rect 4 8
export ItemParse_style
proc ItemParse_style 4 8
line 4504
;4501:}
;4502:
;4503:// style <integer>
;4504:qboolean ItemParse_style( itemDef_t *item, int handle ) {
line 4505
;4505:	if (!PC_Int_Parse(handle, &item->window.style)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2089
line 4506
;4506:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2088
JUMPV
LABELV $2089
line 4508
;4507:	}
;4508:	return qtrue;
CNSTI4 1
RETI4
LABELV $2088
endproc ItemParse_style 4 8
export ItemParse_decoration
proc ItemParse_decoration 4 0
line 4512
;4509:}
;4510:
;4511:// decoration
;4512:qboolean ItemParse_decoration( itemDef_t *item, int handle ) {
line 4513
;4513:	item->window.flags |= WINDOW_DECORATION;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 4514
;4514:	return qtrue;
CNSTI4 1
RETI4
LABELV $2091
endproc ItemParse_decoration 4 0
export ItemParse_notselectable
proc ItemParse_notselectable 4 4
line 4518
;4515:}
;4516:
;4517:// notselectable
;4518:qboolean ItemParse_notselectable( itemDef_t *item, int handle ) {
line 4520
;4519:	listBoxDef_t *listPtr;
;4520:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4521
;4521:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4522
;4522:	if (item->type == ITEM_TYPE_LISTBOX && listPtr) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 6
NEI4 $2093
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2093
line 4523
;4523:		listPtr->notselectable = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 228
ADDP4
CNSTI4 1
ASGNI4
line 4524
;4524:	}
LABELV $2093
line 4525
;4525:	return qtrue;
CNSTI4 1
RETI4
LABELV $2092
endproc ItemParse_notselectable 4 4
export ItemParse_wrapped
proc ItemParse_wrapped 4 0
line 4529
;4526:}
;4527:
;4528:// manually wrapped
;4529:qboolean ItemParse_wrapped( itemDef_t *item, int handle ) {
line 4530
;4530:	item->window.flags |= WINDOW_WRAPPED;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 262144
BORI4
ASGNI4
line 4531
;4531:	return qtrue;
CNSTI4 1
RETI4
LABELV $2095
endproc ItemParse_wrapped 4 0
export ItemParse_autowrapped
proc ItemParse_autowrapped 4 0
line 4535
;4532:}
;4533:
;4534:// auto wrapped
;4535:qboolean ItemParse_autowrapped( itemDef_t *item, int handle ) {
line 4536
;4536:	item->window.flags |= WINDOW_AUTOWRAPPED;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 524288
BORI4
ASGNI4
line 4537
;4537:	return qtrue;
CNSTI4 1
RETI4
LABELV $2096
endproc ItemParse_autowrapped 4 0
export ItemParse_horizontalscroll
proc ItemParse_horizontalscroll 4 0
line 4542
;4538:}
;4539:
;4540:
;4541:// horizontalscroll
;4542:qboolean ItemParse_horizontalscroll( itemDef_t *item, int handle ) {
line 4543
;4543:	item->window.flags |= WINDOW_HORIZONTAL;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1024
BORI4
ASGNI4
line 4544
;4544:	return qtrue;
CNSTI4 1
RETI4
LABELV $2097
endproc ItemParse_horizontalscroll 4 0
export ItemParse_type
proc ItemParse_type 4 8
line 4548
;4545:}
;4546:
;4547:// type <integer>
;4548:qboolean ItemParse_type( itemDef_t *item, int handle ) {
line 4549
;4549:	if (!PC_Int_Parse(handle, &item->type)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2099
line 4550
;4550:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2098
JUMPV
LABELV $2099
line 4552
;4551:	}
;4552:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4553
;4553:	return qtrue;
CNSTI4 1
RETI4
LABELV $2098
endproc ItemParse_type 4 8
export ItemParse_elementwidth
proc ItemParse_elementwidth 8 8
line 4558
;4554:}
;4555:
;4556:// elementwidth, used for listbox image elements
;4557:// uses textalignx for storage
;4558:qboolean ItemParse_elementwidth( itemDef_t *item, int handle ) {
line 4561
;4559:	listBoxDef_t *listPtr;
;4560:
;4561:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4562
;4562:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4563
;4563:	if (!PC_Float_Parse(handle, &listPtr->elementWidth)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2102
line 4564
;4564:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2101
JUMPV
LABELV $2102
line 4566
;4565:	}
;4566:	return qtrue;
CNSTI4 1
RETI4
LABELV $2101
endproc ItemParse_elementwidth 8 8
export ItemParse_elementheight
proc ItemParse_elementheight 8 8
line 4571
;4567:}
;4568:
;4569:// elementheight, used for listbox image elements
;4570:// uses textaligny for storage
;4571:qboolean ItemParse_elementheight( itemDef_t *item, int handle ) {
line 4574
;4572:	listBoxDef_t *listPtr;
;4573:
;4574:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4575
;4575:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4576
;4576:	if (!PC_Float_Parse(handle, &listPtr->elementHeight)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2105
line 4577
;4577:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2104
JUMPV
LABELV $2105
line 4579
;4578:	}
;4579:	return qtrue;
CNSTI4 1
RETI4
LABELV $2104
endproc ItemParse_elementheight 8 8
export ItemParse_feeder
proc ItemParse_feeder 4 8
line 4583
;4580:}
;4581:
;4582:// feeder <float>
;4583:qboolean ItemParse_feeder( itemDef_t *item, int handle ) {
line 4584
;4584:	if (!PC_Float_Parse(handle, &item->special)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2108
line 4585
;4585:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2107
JUMPV
LABELV $2108
line 4587
;4586:	}
;4587:	return qtrue;
CNSTI4 1
RETI4
LABELV $2107
endproc ItemParse_feeder 4 8
export ItemParse_elementtype
proc ItemParse_elementtype 8 8
line 4592
;4588:}
;4589:
;4590:// elementtype, used to specify what type of elements a listbox contains
;4591:// uses textstyle for storage
;4592:qboolean ItemParse_elementtype( itemDef_t *item, int handle ) {
line 4595
;4593:	listBoxDef_t *listPtr;
;4594:
;4595:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4596
;4596:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2111
line 4597
;4597:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2110
JUMPV
LABELV $2111
line 4598
;4598:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4599
;4599:	if (!PC_Int_Parse(handle, &listPtr->elementStyle)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2113
line 4600
;4600:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2110
JUMPV
LABELV $2113
line 4602
;4601:	}
;4602:	return qtrue;
CNSTI4 1
RETI4
LABELV $2110
endproc ItemParse_elementtype 8 8
export ItemParse_columns
proc ItemParse_columns 40 8
line 4606
;4603:}
;4604:
;4605:// columns sets a number of columns and an x pos and width per.. 
;4606:qboolean ItemParse_columns( itemDef_t *item, int handle ) {
line 4610
;4607:	int num, i;
;4608:	listBoxDef_t *listPtr;
;4609:
;4610:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4611
;4611:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2116
line 4612
;4612:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2115
JUMPV
LABELV $2116
line 4613
;4613:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4614
;4614:	if (PC_Int_Parse(handle, &num)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 12
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2118
line 4615
;4615:		if (num > MAX_LB_COLUMNS) {
ADDRLP4 8
INDIRI4
CNSTI4 16
LEI4 $2120
line 4616
;4616:			num = MAX_LB_COLUMNS;
ADDRLP4 8
CNSTI4 16
ASGNI4
line 4617
;4617:		}
LABELV $2120
line 4618
;4618:		listPtr->numColumns = num;
ADDRLP4 4
INDIRP4
CNSTI4 28
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 4619
;4619:		for (i = 0; i < num; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2125
JUMPV
LABELV $2122
line 4622
;4620:			int pos, width, maxChars;
;4621:
;4622:			if (PC_Int_Parse(handle, &pos) && PC_Int_Parse(handle, &width) && PC_Int_Parse(handle, &maxChars)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 16
ARGP4
ADDRLP4 28
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $2126
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 20
ARGP4
ADDRLP4 32
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $2126
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 24
ARGP4
ADDRLP4 36
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
EQI4 $2126
line 4623
;4623:				listPtr->columnInfo[i].pos = pos;
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 4
INDIRP4
CNSTI4 32
ADDP4
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 4624
;4624:				listPtr->columnInfo[i].width = width;
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 4
INDIRP4
CNSTI4 32
ADDP4
ADDP4
CNSTI4 4
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 4625
;4625:				listPtr->columnInfo[i].maxChars = maxChars;
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRLP4 4
INDIRP4
CNSTI4 32
ADDP4
ADDP4
CNSTI4 8
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 4626
;4626:			} else {
ADDRGP4 $2127
JUMPV
LABELV $2126
line 4627
;4627:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2115
JUMPV
LABELV $2127
line 4629
;4628:			}
;4629:		}
LABELV $2123
line 4619
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2125
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $2122
line 4630
;4630:	} else {
ADDRGP4 $2119
JUMPV
LABELV $2118
line 4631
;4631:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2115
JUMPV
LABELV $2119
line 4633
;4632:	}
;4633:	return qtrue;
CNSTI4 1
RETI4
LABELV $2115
endproc ItemParse_columns 40 8
export ItemParse_border
proc ItemParse_border 4 8
line 4636
;4634:}
;4635:
;4636:qboolean ItemParse_border( itemDef_t *item, int handle ) {
line 4637
;4637:	if (!PC_Int_Parse(handle, &item->window.border)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2129
line 4638
;4638:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2128
JUMPV
LABELV $2129
line 4640
;4639:	}
;4640:	return qtrue;
CNSTI4 1
RETI4
LABELV $2128
endproc ItemParse_border 4 8
export ItemParse_bordersize
proc ItemParse_bordersize 4 8
line 4643
;4641:}
;4642:
;4643:qboolean ItemParse_bordersize( itemDef_t *item, int handle ) {
line 4644
;4644:	if (!PC_Float_Parse(handle, &item->window.borderSize)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2132
line 4645
;4645:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2131
JUMPV
LABELV $2132
line 4647
;4646:	}
;4647:	return qtrue;
CNSTI4 1
RETI4
LABELV $2131
endproc ItemParse_bordersize 4 8
export ItemParse_visible
proc ItemParse_visible 12 8
line 4650
;4648:}
;4649:
;4650:qboolean ItemParse_visible( itemDef_t *item, int handle ) {
line 4653
;4651:	int i;
;4652:
;4653:	if (!PC_Int_Parse(handle, &i)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2135
line 4654
;4654:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2134
JUMPV
LABELV $2135
line 4656
;4655:	}
;4656:	if (i) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2137
line 4657
;4657:		item->window.flags |= WINDOW_VISIBLE;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 4658
;4658:	}
LABELV $2137
line 4659
;4659:	return qtrue;
CNSTI4 1
RETI4
LABELV $2134
endproc ItemParse_visible 12 8
export ItemParse_ownerdraw
proc ItemParse_ownerdraw 4 8
line 4662
;4660:}
;4661:
;4662:qboolean ItemParse_ownerdraw( itemDef_t *item, int handle ) {
line 4663
;4663:	if (!PC_Int_Parse(handle, &item->window.ownerDraw)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2140
line 4664
;4664:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2139
JUMPV
LABELV $2140
line 4666
;4665:	}
;4666:	item->type = ITEM_TYPE_OWNERDRAW;
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
CNSTI4 8
ASGNI4
line 4667
;4667:	return qtrue;
CNSTI4 1
RETI4
LABELV $2139
endproc ItemParse_ownerdraw 4 8
export ItemParse_align
proc ItemParse_align 4 8
line 4670
;4668:}
;4669:
;4670:qboolean ItemParse_align( itemDef_t *item, int handle ) {
line 4671
;4671:	if (!PC_Int_Parse(handle, &item->alignment)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2143
line 4672
;4672:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2142
JUMPV
LABELV $2143
line 4674
;4673:	}
;4674:	return qtrue;
CNSTI4 1
RETI4
LABELV $2142
endproc ItemParse_align 4 8
export ItemParse_textalign
proc ItemParse_textalign 4 8
line 4677
;4675:}
;4676:
;4677:qboolean ItemParse_textalign( itemDef_t *item, int handle ) {
line 4678
;4678:	if (!PC_Int_Parse(handle, &item->textalignment)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2146
line 4679
;4679:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2145
JUMPV
LABELV $2146
line 4681
;4680:	}
;4681:	return qtrue;
CNSTI4 1
RETI4
LABELV $2145
endproc ItemParse_textalign 4 8
export ItemParse_textalignx
proc ItemParse_textalignx 4 8
line 4684
;4682:}
;4683:
;4684:qboolean ItemParse_textalignx( itemDef_t *item, int handle ) {
line 4685
;4685:	if (!PC_Float_Parse(handle, &item->textalignx)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2149
line 4686
;4686:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2148
JUMPV
LABELV $2149
line 4688
;4687:	}
;4688:	return qtrue;
CNSTI4 1
RETI4
LABELV $2148
endproc ItemParse_textalignx 4 8
export ItemParse_textaligny
proc ItemParse_textaligny 4 8
line 4691
;4689:}
;4690:
;4691:qboolean ItemParse_textaligny( itemDef_t *item, int handle ) {
line 4692
;4692:	if (!PC_Float_Parse(handle, &item->textaligny)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2152
line 4693
;4693:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2151
JUMPV
LABELV $2152
line 4695
;4694:	}
;4695:	return qtrue;
CNSTI4 1
RETI4
LABELV $2151
endproc ItemParse_textaligny 4 8
export ItemParse_textscale
proc ItemParse_textscale 4 8
line 4698
;4696:}
;4697:
;4698:qboolean ItemParse_textscale( itemDef_t *item, int handle ) {
line 4699
;4699:	if (!PC_Float_Parse(handle, &item->textscale)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2155
line 4700
;4700:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2154
JUMPV
LABELV $2155
line 4702
;4701:	}
;4702:	return qtrue;
CNSTI4 1
RETI4
LABELV $2154
endproc ItemParse_textscale 4 8
export ItemParse_textstyle
proc ItemParse_textstyle 4 8
line 4705
;4703:}
;4704:
;4705:qboolean ItemParse_textstyle( itemDef_t *item, int handle ) {
line 4706
;4706:	if (!PC_Int_Parse(handle, &item->textStyle)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2158
line 4707
;4707:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2157
JUMPV
LABELV $2158
line 4709
;4708:	}
;4709:	return qtrue;
CNSTI4 1
RETI4
LABELV $2157
endproc ItemParse_textstyle 4 8
export ItemParse_backcolor
proc ItemParse_backcolor 12 8
line 4712
;4710:}
;4711:
;4712:qboolean ItemParse_backcolor( itemDef_t *item, int handle ) {
line 4716
;4713:	int i;
;4714:	float f;
;4715:
;4716:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2161
line 4717
;4717:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2165
line 4718
;4718:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2160
JUMPV
LABELV $2165
line 4720
;4719:		}
;4720:		item->window.backColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 128
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 4721
;4721:	}
LABELV $2162
line 4716
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2161
line 4722
;4722:	return qtrue;
CNSTI4 1
RETI4
LABELV $2160
endproc ItemParse_backcolor 12 8
export ItemParse_forecolor
proc ItemParse_forecolor 16 8
line 4725
;4723:}
;4724:
;4725:qboolean ItemParse_forecolor( itemDef_t *item, int handle ) {
line 4729
;4726:	int i;
;4727:	float f;
;4728:
;4729:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2168
line 4730
;4730:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2172
line 4731
;4731:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2167
JUMPV
LABELV $2172
line 4733
;4732:		}
;4733:		item->window.foreColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 4734
;4734:		item->window.flags |= WINDOW_FORECOLORSET;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 4735
;4735:	}
LABELV $2169
line 4729
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2168
line 4736
;4736:	return qtrue;
CNSTI4 1
RETI4
LABELV $2167
endproc ItemParse_forecolor 16 8
export ItemParse_bordercolor
proc ItemParse_bordercolor 12 8
line 4739
;4737:}
;4738:
;4739:qboolean ItemParse_bordercolor( itemDef_t *item, int handle ) {
line 4743
;4740:	int i;
;4741:	float f;
;4742:
;4743:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2175
line 4744
;4744:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2179
line 4745
;4745:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2174
JUMPV
LABELV $2179
line 4747
;4746:		}
;4747:		item->window.borderColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 4748
;4748:	}
LABELV $2176
line 4743
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2175
line 4749
;4749:	return qtrue;
CNSTI4 1
RETI4
LABELV $2174
endproc ItemParse_bordercolor 12 8
export ItemParse_outlinecolor
proc ItemParse_outlinecolor 4 8
line 4752
;4750:}
;4751:
;4752:qboolean ItemParse_outlinecolor( itemDef_t *item, int handle ) {
line 4753
;4753:	if (!PC_Color_Parse(handle, &item->window.outlineColor)){
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 160
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Color_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2182
line 4754
;4754:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2181
JUMPV
LABELV $2182
line 4756
;4755:	}
;4756:	return qtrue;
CNSTI4 1
RETI4
LABELV $2181
endproc ItemParse_outlinecolor 4 8
export ItemParse_background
proc ItemParse_background 12 8
line 4759
;4757:}
;4758:
;4759:qboolean ItemParse_background( itemDef_t *item, int handle ) {
line 4762
;4760:	const char *temp;
;4761:
;4762:	if (!PC_String_Parse(handle, &temp)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2185
line 4763
;4763:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2184
JUMPV
LABELV $2185
line 4765
;4764:	}
;4765:	item->window.background = DC->registerShaderNoMip(temp);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
INDIRP4
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 4766
;4766:	return qtrue;
CNSTI4 1
RETI4
LABELV $2184
endproc ItemParse_background 12 8
export ItemParse_cinematic
proc ItemParse_cinematic 4 8
line 4769
;4767:}
;4768:
;4769:qboolean ItemParse_cinematic( itemDef_t *item, int handle ) {
line 4770
;4770:	if (!PC_String_Parse(handle, &item->window.cinematicName)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2188
line 4771
;4771:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2187
JUMPV
LABELV $2188
line 4773
;4772:	}
;4773:	return qtrue;
CNSTI4 1
RETI4
LABELV $2187
endproc ItemParse_cinematic 4 8
export ItemParse_doubleClick
proc ItemParse_doubleClick 8 8
line 4776
;4774:}
;4775:
;4776:qboolean ItemParse_doubleClick( itemDef_t *item, int handle ) {
line 4779
;4777:	listBoxDef_t *listPtr;
;4778:
;4779:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4780
;4780:	if (!item->typeData) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2191
line 4781
;4781:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2190
JUMPV
LABELV $2191
line 4784
;4782:	}
;4783:
;4784:	listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4786
;4785:
;4786:	if (!PC_Script_Parse(handle, &listPtr->doubleClick)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2193
line 4787
;4787:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2190
JUMPV
LABELV $2193
line 4789
;4788:	}
;4789:	return qtrue;
CNSTI4 1
RETI4
LABELV $2190
endproc ItemParse_doubleClick 8 8
export ItemParse_onFocus
proc ItemParse_onFocus 4 8
line 4792
;4790:}
;4791:
;4792:qboolean ItemParse_onFocus( itemDef_t *item, int handle ) {
line 4793
;4793:	if (!PC_Script_Parse(handle, &item->onFocus)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 256
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2196
line 4794
;4794:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2195
JUMPV
LABELV $2196
line 4796
;4795:	}
;4796:	return qtrue;
CNSTI4 1
RETI4
LABELV $2195
endproc ItemParse_onFocus 4 8
export ItemParse_leaveFocus
proc ItemParse_leaveFocus 4 8
line 4799
;4797:}
;4798:
;4799:qboolean ItemParse_leaveFocus( itemDef_t *item, int handle ) {
line 4800
;4800:	if (!PC_Script_Parse(handle, &item->leaveFocus)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2199
line 4801
;4801:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2198
JUMPV
LABELV $2199
line 4803
;4802:	}
;4803:	return qtrue;
CNSTI4 1
RETI4
LABELV $2198
endproc ItemParse_leaveFocus 4 8
export ItemParse_mouseEnter
proc ItemParse_mouseEnter 4 8
line 4806
;4804:}
;4805:
;4806:qboolean ItemParse_mouseEnter( itemDef_t *item, int handle ) {
line 4807
;4807:	if (!PC_Script_Parse(handle, &item->mouseEnter)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 244
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2202
line 4808
;4808:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2201
JUMPV
LABELV $2202
line 4810
;4809:	}
;4810:	return qtrue;
CNSTI4 1
RETI4
LABELV $2201
endproc ItemParse_mouseEnter 4 8
export ItemParse_mouseExit
proc ItemParse_mouseExit 4 8
line 4813
;4811:}
;4812:
;4813:qboolean ItemParse_mouseExit( itemDef_t *item, int handle ) {
line 4814
;4814:	if (!PC_Script_Parse(handle, &item->mouseExit)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 248
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2205
line 4815
;4815:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2204
JUMPV
LABELV $2205
line 4817
;4816:	}
;4817:	return qtrue;
CNSTI4 1
RETI4
LABELV $2204
endproc ItemParse_mouseExit 4 8
export ItemParse_mouseEnterText
proc ItemParse_mouseEnterText 4 8
line 4820
;4818:}
;4819:
;4820:qboolean ItemParse_mouseEnterText( itemDef_t *item, int handle ) {
line 4821
;4821:	if (!PC_Script_Parse(handle, &item->mouseEnterText)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 236
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2208
line 4822
;4822:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2207
JUMPV
LABELV $2208
line 4824
;4823:	}
;4824:	return qtrue;
CNSTI4 1
RETI4
LABELV $2207
endproc ItemParse_mouseEnterText 4 8
export ItemParse_mouseExitText
proc ItemParse_mouseExitText 4 8
line 4827
;4825:}
;4826:
;4827:qboolean ItemParse_mouseExitText( itemDef_t *item, int handle ) {
line 4828
;4828:	if (!PC_Script_Parse(handle, &item->mouseExitText)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 240
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2211
line 4829
;4829:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2210
JUMPV
LABELV $2211
line 4831
;4830:	}
;4831:	return qtrue;
CNSTI4 1
RETI4
LABELV $2210
endproc ItemParse_mouseExitText 4 8
export ItemParse_action
proc ItemParse_action 4 8
line 4834
;4832:}
;4833:
;4834:qboolean ItemParse_action( itemDef_t *item, int handle ) {
line 4835
;4835:	if (!PC_Script_Parse(handle, &item->action)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 252
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2214
line 4836
;4836:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2213
JUMPV
LABELV $2214
line 4838
;4837:	}
;4838:	return qtrue;
CNSTI4 1
RETI4
LABELV $2213
endproc ItemParse_action 4 8
export ItemParse_special
proc ItemParse_special 4 8
line 4841
;4839:}
;4840:
;4841:qboolean ItemParse_special( itemDef_t *item, int handle ) {
line 4842
;4842:	if (!PC_Float_Parse(handle, &item->special)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 528
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2217
line 4843
;4843:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2216
JUMPV
LABELV $2217
line 4845
;4844:	}
;4845:	return qtrue;
CNSTI4 1
RETI4
LABELV $2216
endproc ItemParse_special 4 8
export ItemParse_cvarTest
proc ItemParse_cvarTest 4 8
line 4848
;4846:}
;4847:
;4848:qboolean ItemParse_cvarTest( itemDef_t *item, int handle ) {
line 4849
;4849:	if (!PC_String_Parse(handle, &item->cvarTest)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 268
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $2220
line 4850
;4850:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2219
JUMPV
LABELV $2220
line 4852
;4851:	}
;4852:	return qtrue;
CNSTI4 1
RETI4
LABELV $2219
endproc ItemParse_cvarTest 4 8
export ItemParse_cvar
proc ItemParse_cvar 8 8
line 4855
;4853:}
;4854:
;4855:qboolean ItemParse_cvar( itemDef_t *item, int handle ) {
line 4858
;4856:	editFieldDef_t *editPtr;
;4857:
;4858:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4859
;4859:	if (!PC_String_Parse(handle, &item->cvar)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2223
line 4860
;4860:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2222
JUMPV
LABELV $2223
line 4862
;4861:	}
;4862:	if (item->typeData) {
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2225
line 4863
;4863:		editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4864
;4864:		editPtr->minVal = -1;
ADDRLP4 0
INDIRP4
CNSTF4 3212836864
ASGNF4
line 4865
;4865:		editPtr->maxVal = -1;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTF4 3212836864
ASGNF4
line 4866
;4866:		editPtr->defVal = -1;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTF4 3212836864
ASGNF4
line 4867
;4867:	}
LABELV $2225
line 4868
;4868:	return qtrue;
CNSTI4 1
RETI4
LABELV $2222
endproc ItemParse_cvar 8 8
export ItemParse_maxChars
proc ItemParse_maxChars 12 8
line 4871
;4869:}
;4870:
;4871:qboolean ItemParse_maxChars( itemDef_t *item, int handle ) {
line 4875
;4872:	editFieldDef_t *editPtr;
;4873:	int maxChars;
;4874:
;4875:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4876
;4876:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2228
line 4877
;4877:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2227
JUMPV
LABELV $2228
line 4879
;4878:
;4879:	if (!PC_Int_Parse(handle, &maxChars)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2230
line 4880
;4880:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2227
JUMPV
LABELV $2230
line 4882
;4881:	}
;4882:	editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4883
;4883:	editPtr->maxChars = maxChars;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 4884
;4884:	return qtrue;
CNSTI4 1
RETI4
LABELV $2227
endproc ItemParse_maxChars 12 8
export ItemParse_maxPaintChars
proc ItemParse_maxPaintChars 12 8
line 4887
;4885:}
;4886:
;4887:qboolean ItemParse_maxPaintChars( itemDef_t *item, int handle ) {
line 4891
;4888:	editFieldDef_t *editPtr;
;4889:	int maxChars;
;4890:
;4891:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4892
;4892:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2233
line 4893
;4893:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2232
JUMPV
LABELV $2233
line 4895
;4894:
;4895:	if (!PC_Int_Parse(handle, &maxChars)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2235
line 4896
;4896:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2232
JUMPV
LABELV $2235
line 4898
;4897:	}
;4898:	editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4899
;4899:	editPtr->maxPaintChars = maxChars;
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 4900
;4900:	return qtrue;
CNSTI4 1
RETI4
LABELV $2232
endproc ItemParse_maxPaintChars 12 8
export ItemParse_cvarFloat
proc ItemParse_cvarFloat 20 8
line 4905
;4901:}
;4902:
;4903:
;4904:
;4905:qboolean ItemParse_cvarFloat( itemDef_t *item, int handle ) {
line 4908
;4906:	editFieldDef_t *editPtr;
;4907:
;4908:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4909
;4909:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2238
line 4910
;4910:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2237
JUMPV
LABELV $2238
line 4911
;4911:	editPtr = (editFieldDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4912
;4912:	if (PC_String_Parse(handle, &item->cvar) &&
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $2240
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $2240
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2240
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ARGP4
ADDRLP4 16
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $2240
line 4915
;4913:		PC_Float_Parse(handle, &editPtr->defVal) &&
;4914:		PC_Float_Parse(handle, &editPtr->minVal) &&
;4915:		PC_Float_Parse(handle, &editPtr->maxVal)) {
line 4916
;4916:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2237
JUMPV
LABELV $2240
line 4918
;4917:	}
;4918:	return qfalse;
CNSTI4 0
RETI4
LABELV $2237
endproc ItemParse_cvarFloat 20 8
export ItemParse_cvarStrList
proc ItemParse_cvarStrList 1068 8
line 4921
;4919:}
;4920:
;4921:qboolean ItemParse_cvarStrList( itemDef_t *item, int handle ) {
line 4926
;4922:	pc_token_t token;
;4923:	multiDef_t *multiPtr;
;4924:	int pass;
;4925:	
;4926:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4927
;4927:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2243
line 4928
;4928:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2243
line 4929
;4929:	multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 1040
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4930
;4930:	multiPtr->count = 0;
ADDRLP4 1040
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 0
ASGNI4
line 4931
;4931:	multiPtr->strDef = qtrue;
ADDRLP4 1040
INDIRP4
CNSTI4 388
ADDP4
CNSTI4 1
ASGNI4
line 4933
;4932:
;4933:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $2245
line 4934
;4934:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2245
line 4935
;4935:	if (*token.string != '{') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $2247
line 4936
;4936:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2247
line 4939
;4937:	}
;4938:
;4939:	pass = 0;
ADDRLP4 1044
CNSTI4 0
ASGNI4
ADDRGP4 $2251
JUMPV
LABELV $2250
line 4940
;4940:	while ( 1 ) {
line 4941
;4941:		if (!trap_PC_ReadToken(handle, &token)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1052
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $2253
line 4942
;4942:			PC_SourceError(handle, "end of file inside menu item\n");
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $2255
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 4943
;4943:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2253
line 4946
;4944:		}
;4945:
;4946:		if (*token.string == '}') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $2256
line 4947
;4947:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2256
line 4950
;4948:		}
;4949:
;4950:		if (*token.string == ',' || *token.string == ';') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 44
EQI4 $2263
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 59
NEI4 $2259
LABELV $2263
line 4951
;4951:			continue;
ADDRGP4 $2251
JUMPV
LABELV $2259
line 4954
;4952:		}
;4953:
;4954:		if (pass == 0) {
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2264
line 4955
;4955:			multiPtr->cvarList[multiPtr->count] = String_Alloc(token.string);
ADDRLP4 0+16
ARGP4
ADDRLP4 1056
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1040
INDIRP4
ADDP4
ADDRLP4 1056
INDIRP4
ASGNP4
line 4956
;4956:			pass = 1;
ADDRLP4 1044
CNSTI4 1
ASGNI4
line 4957
;4957:		} else {
ADDRGP4 $2265
JUMPV
LABELV $2264
line 4958
;4958:			multiPtr->cvarStr[multiPtr->count] = String_Alloc(token.string);
ADDRLP4 0+16
ARGP4
ADDRLP4 1056
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1040
INDIRP4
CNSTI4 128
ADDP4
ADDP4
ADDRLP4 1056
INDIRP4
ASGNP4
line 4959
;4959:			pass = 0;
ADDRLP4 1044
CNSTI4 0
ASGNI4
line 4960
;4960:			multiPtr->count++;
ADDRLP4 1064
ADDRLP4 1040
INDIRP4
CNSTI4 384
ADDP4
ASGNP4
ADDRLP4 1064
INDIRP4
ADDRLP4 1064
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 4961
;4961:			if (multiPtr->count >= MAX_MULTI_CVARS) {
ADDRLP4 1040
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 32
LTI4 $2268
line 4962
;4962:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2242
JUMPV
LABELV $2268
line 4964
;4963:			}
;4964:		}
LABELV $2265
line 4966
;4965:
;4966:	}
LABELV $2251
line 4940
ADDRGP4 $2250
JUMPV
line 4967
;4967:	return qfalse; 	// bk001205 - LCC missing return value
CNSTI4 0
RETI4
LABELV $2242
endproc ItemParse_cvarStrList 1068 8
export ItemParse_cvarFloatList
proc ItemParse_cvarFloatList 1072 8
line 4970
;4968:}
;4969:
;4970:qboolean ItemParse_cvarFloatList( itemDef_t *item, int handle ) {
line 4974
;4971:	pc_token_t token;
;4972:	multiDef_t *multiPtr;
;4973:	
;4974:	Item_ValidateTypeData(item);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Item_ValidateTypeData
CALLV
pop
line 4975
;4975:	if (!item->typeData)
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2271
line 4976
;4976:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2271
line 4977
;4977:	multiPtr = (multiDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 4978
;4978:	multiPtr->count = 0;
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
CNSTI4 0
ASGNI4
line 4979
;4979:	multiPtr->strDef = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 388
ADDP4
CNSTI4 0
ASGNI4
line 4981
;4980:
;4981:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2273
line 4982
;4982:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2273
line 4983
;4983:	if (*token.string != '{') {
ADDRLP4 4+16
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $2279
line 4984
;4984:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2278
line 4987
;4985:	}
;4986:
;4987:	while ( 1 ) {
line 4988
;4988:		if (!trap_PC_ReadToken(handle, &token)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $2281
line 4989
;4989:			PC_SourceError(handle, "end of file inside menu item\n");
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $2255
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 4990
;4990:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2281
line 4993
;4991:		}
;4992:
;4993:		if (*token.string == '}') {
ADDRLP4 4+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $2283
line 4994
;4994:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2283
line 4997
;4995:		}
;4996:
;4997:		if (*token.string == ',' || *token.string == ';') {
ADDRLP4 4+16
INDIRI1
CVII4 1
CNSTI4 44
EQI4 $2290
ADDRLP4 4+16
INDIRI1
CVII4 1
CNSTI4 59
NEI4 $2286
LABELV $2290
line 4998
;4998:			continue;
ADDRGP4 $2279
JUMPV
LABELV $2286
line 5001
;4999:		}
;5000:
;5001:		multiPtr->cvarList[multiPtr->count] = String_Alloc(token.string);
ADDRLP4 4+16
ARGP4
ADDRLP4 1052
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
ADDP4
ADDRLP4 1052
INDIRP4
ASGNP4
line 5002
;5002:		if (!PC_Float_Parse(handle, &multiPtr->cvarValue[multiPtr->count])) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDP4
ARGP4
ADDRLP4 1064
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $2292
line 5003
;5003:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2292
line 5006
;5004:		}
;5005:
;5006:		multiPtr->count++;
ADDRLP4 1068
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
ASGNP4
ADDRLP4 1068
INDIRP4
ADDRLP4 1068
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 5007
;5007:		if (multiPtr->count >= MAX_MULTI_CVARS) {
ADDRLP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRI4
CNSTI4 32
LTI4 $2294
line 5008
;5008:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2270
JUMPV
LABELV $2294
line 5011
;5009:		}
;5010:
;5011:	}
LABELV $2279
line 4987
ADDRGP4 $2278
JUMPV
line 5012
;5012:	return qfalse; 	// bk001205 - LCC missing return value
CNSTI4 0
RETI4
LABELV $2270
endproc ItemParse_cvarFloatList 1072 8
export ItemParse_addColorRange
proc ItemParse_addColorRange 48 12
line 5017
;5013:}
;5014:
;5015:
;5016:
;5017:qboolean ItemParse_addColorRange( itemDef_t *item, int handle ) {
line 5020
;5018:	colorRangeDef_t color;
;5019:
;5020:	if (PC_Float_Parse(handle, &color.low) &&
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0+16
ARGP4
ADDRLP4 24
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $2297
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0+20
ARGP4
ADDRLP4 28
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $2297
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 32
ADDRGP4 PC_Color_Parse
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $2297
line 5022
;5021:		PC_Float_Parse(handle, &color.high) &&
;5022:		PC_Color_Parse(handle, &color.color) ) {
line 5023
;5023:		if (item->numColors < MAX_COLOR_RANGES) {
ADDRFP4 0
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
CNSTI4 10
GEI4 $2301
line 5024
;5024:			memcpy(&item->colorRanges[item->numColors], &color, sizeof(color));
ADDRLP4 36
CNSTI4 24
ASGNI4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRI4
ADDRLP4 40
INDIRP4
CNSTI4 284
ADDP4
INDIRI4
MULI4
ADDRLP4 40
INDIRP4
CNSTI4 288
ADDP4
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRGP4 memcpy
CALLP4
pop
line 5025
;5025:			item->numColors++;
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 284
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 5026
;5026:		}
LABELV $2301
line 5027
;5027:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2296
JUMPV
LABELV $2297
line 5029
;5028:	}
;5029:	return qfalse;
CNSTI4 0
RETI4
LABELV $2296
endproc ItemParse_addColorRange 48 12
export ItemParse_ownerdrawFlag
proc ItemParse_ownerdrawFlag 12 8
line 5032
;5030:}
;5031:
;5032:qboolean ItemParse_ownerdrawFlag( itemDef_t *item, int handle ) {
line 5034
;5033:	int i;
;5034:	if (!PC_Int_Parse(handle, &i)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2304
line 5035
;5035:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2303
JUMPV
LABELV $2304
line 5037
;5036:	}
;5037:	item->window.ownerDrawFlags |= i;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 5038
;5038:	return qtrue;
CNSTI4 1
RETI4
LABELV $2303
endproc ItemParse_ownerdrawFlag 12 8
export ItemParse_enableCvar
proc ItemParse_enableCvar 4 8
line 5041
;5039:}
;5040:
;5041:qboolean ItemParse_enableCvar( itemDef_t *item, int handle ) {
line 5042
;5042:	if (PC_Script_Parse(handle, &item->enableCvar)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2307
line 5043
;5043:		item->cvarFlags = CVAR_ENABLE;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 1
ASGNI4
line 5044
;5044:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2306
JUMPV
LABELV $2307
line 5046
;5045:	}
;5046:	return qfalse;
CNSTI4 0
RETI4
LABELV $2306
endproc ItemParse_enableCvar 4 8
export ItemParse_disableCvar
proc ItemParse_disableCvar 4 8
line 5049
;5047:}
;5048:
;5049:qboolean ItemParse_disableCvar( itemDef_t *item, int handle ) {
line 5050
;5050:	if (PC_Script_Parse(handle, &item->enableCvar)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2310
line 5051
;5051:		item->cvarFlags = CVAR_DISABLE;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 2
ASGNI4
line 5052
;5052:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2309
JUMPV
LABELV $2310
line 5054
;5053:	}
;5054:	return qfalse;
CNSTI4 0
RETI4
LABELV $2309
endproc ItemParse_disableCvar 4 8
export ItemParse_showCvar
proc ItemParse_showCvar 4 8
line 5057
;5055:}
;5056:
;5057:qboolean ItemParse_showCvar( itemDef_t *item, int handle ) {
line 5058
;5058:	if (PC_Script_Parse(handle, &item->enableCvar)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2313
line 5059
;5059:		item->cvarFlags = CVAR_SHOW;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 4
ASGNI4
line 5060
;5060:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2312
JUMPV
LABELV $2313
line 5062
;5061:	}
;5062:	return qfalse;
CNSTI4 0
RETI4
LABELV $2312
endproc ItemParse_showCvar 4 8
export ItemParse_hideCvar
proc ItemParse_hideCvar 4 8
line 5065
;5063:}
;5064:
;5065:qboolean ItemParse_hideCvar( itemDef_t *item, int handle ) {
line 5066
;5066:	if (PC_Script_Parse(handle, &item->enableCvar)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2316
line 5067
;5067:		item->cvarFlags = CVAR_HIDE;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 8
ASGNI4
line 5068
;5068:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2315
JUMPV
LABELV $2316
line 5070
;5069:	}
;5070:	return qfalse;
CNSTI4 0
RETI4
LABELV $2315
endproc ItemParse_hideCvar 4 8
data
export itemParseKeywords
align 4
LABELV itemParseKeywords
address $2318
address ItemParse_name
byte 4 0
address $2319
address ItemParse_text
byte 4 0
address $2320
address ItemParse_group
byte 4 0
address $2321
address ItemParse_asset_model
byte 4 0
address $2322
address ItemParse_asset_shader
byte 4 0
address $2323
address ItemParse_model_origin
byte 4 0
address $2324
address ItemParse_model_fovx
byte 4 0
address $2325
address ItemParse_model_fovy
byte 4 0
address $2326
address ItemParse_model_rotation
byte 4 0
address $2327
address ItemParse_model_angle
byte 4 0
address $2328
address ItemParse_rect
byte 4 0
address $2329
address ItemParse_style
byte 4 0
address $2330
address ItemParse_decoration
byte 4 0
address $2331
address ItemParse_notselectable
byte 4 0
address $2332
address ItemParse_wrapped
byte 4 0
address $2333
address ItemParse_autowrapped
byte 4 0
address $2334
address ItemParse_horizontalscroll
byte 4 0
address $2335
address ItemParse_type
byte 4 0
address $2336
address ItemParse_elementwidth
byte 4 0
address $2337
address ItemParse_elementheight
byte 4 0
address $2338
address ItemParse_feeder
byte 4 0
address $2339
address ItemParse_elementtype
byte 4 0
address $2340
address ItemParse_columns
byte 4 0
address $2341
address ItemParse_border
byte 4 0
address $2342
address ItemParse_bordersize
byte 4 0
address $2343
address ItemParse_visible
byte 4 0
address $2344
address ItemParse_ownerdraw
byte 4 0
address $2345
address ItemParse_align
byte 4 0
address $2346
address ItemParse_textalign
byte 4 0
address $2347
address ItemParse_textalignx
byte 4 0
address $2348
address ItemParse_textaligny
byte 4 0
address $2349
address ItemParse_textscale
byte 4 0
address $2350
address ItemParse_textstyle
byte 4 0
address $358
address ItemParse_backcolor
byte 4 0
address $361
address ItemParse_forecolor
byte 4 0
address $364
address ItemParse_bordercolor
byte 4 0
address $2351
address ItemParse_outlinecolor
byte 4 0
address $2352
address ItemParse_background
byte 4 0
address $2353
address ItemParse_onFocus
byte 4 0
address $2354
address ItemParse_leaveFocus
byte 4 0
address $2355
address ItemParse_mouseEnter
byte 4 0
address $2356
address ItemParse_mouseExit
byte 4 0
address $2357
address ItemParse_mouseEnterText
byte 4 0
address $2358
address ItemParse_mouseExitText
byte 4 0
address $2359
address ItemParse_action
byte 4 0
address $2360
address ItemParse_special
byte 4 0
address $2361
address ItemParse_cvar
byte 4 0
address $2362
address ItemParse_maxChars
byte 4 0
address $2363
address ItemParse_maxPaintChars
byte 4 0
address $2364
address ItemParse_focusSound
byte 4 0
address $2365
address ItemParse_cvarFloat
byte 4 0
address $2366
address ItemParse_cvarStrList
byte 4 0
address $2367
address ItemParse_cvarFloatList
byte 4 0
address $2368
address ItemParse_addColorRange
byte 4 0
address $2369
address ItemParse_ownerdrawFlag
byte 4 0
address $2370
address ItemParse_enableCvar
byte 4 0
address $2371
address ItemParse_cvarTest
byte 4 0
address $2372
address ItemParse_disableCvar
byte 4 0
address $2373
address ItemParse_showCvar
byte 4 0
address $2374
address ItemParse_hideCvar
byte 4 0
address $2375
address ItemParse_cinematic
byte 4 0
address $2376
address ItemParse_doubleClick
byte 4 0
byte 4 0
byte 4 0
byte 4 0
export Item_SetupKeywordHash
code
proc Item_SetupKeywordHash 4 12
line 5147
;5071:}
;5072:
;5073:
;5074:keywordHash_t itemParseKeywords[] = {
;5075:	{"name", ItemParse_name, NULL},
;5076:	{"text", ItemParse_text, NULL},
;5077:	{"group", ItemParse_group, NULL},
;5078:	{"asset_model", ItemParse_asset_model, NULL},
;5079:	{"asset_shader", ItemParse_asset_shader, NULL},
;5080:	{"model_origin", ItemParse_model_origin, NULL},
;5081:	{"model_fovx", ItemParse_model_fovx, NULL},
;5082:	{"model_fovy", ItemParse_model_fovy, NULL},
;5083:	{"model_rotation", ItemParse_model_rotation, NULL},
;5084:	{"model_angle", ItemParse_model_angle, NULL},
;5085:	{"rect", ItemParse_rect, NULL},
;5086:	{"style", ItemParse_style, NULL},
;5087:	{"decoration", ItemParse_decoration, NULL},
;5088:	{"notselectable", ItemParse_notselectable, NULL},
;5089:	{"wrapped", ItemParse_wrapped, NULL},
;5090:	{"autowrapped", ItemParse_autowrapped, NULL},
;5091:	{"horizontalscroll", ItemParse_horizontalscroll, NULL},
;5092:	{"type", ItemParse_type, NULL},
;5093:	{"elementwidth", ItemParse_elementwidth, NULL},
;5094:	{"elementheight", ItemParse_elementheight, NULL},
;5095:	{"feeder", ItemParse_feeder, NULL},
;5096:	{"elementtype", ItemParse_elementtype, NULL},
;5097:	{"columns", ItemParse_columns, NULL},
;5098:	{"border", ItemParse_border, NULL},
;5099:	{"bordersize", ItemParse_bordersize, NULL},
;5100:	{"visible", ItemParse_visible, NULL},
;5101:	{"ownerdraw", ItemParse_ownerdraw, NULL},
;5102:	{"align", ItemParse_align, NULL},
;5103:	{"textalign", ItemParse_textalign, NULL},
;5104:	{"textalignx", ItemParse_textalignx, NULL},
;5105:	{"textaligny", ItemParse_textaligny, NULL},
;5106:	{"textscale", ItemParse_textscale, NULL},
;5107:	{"textstyle", ItemParse_textstyle, NULL},
;5108:	{"backcolor", ItemParse_backcolor, NULL},
;5109:	{"forecolor", ItemParse_forecolor, NULL},
;5110:	{"bordercolor", ItemParse_bordercolor, NULL},
;5111:	{"outlinecolor", ItemParse_outlinecolor, NULL},
;5112:	{"background", ItemParse_background, NULL},
;5113:	{"onFocus", ItemParse_onFocus, NULL},
;5114:	{"leaveFocus", ItemParse_leaveFocus, NULL},
;5115:	{"mouseEnter", ItemParse_mouseEnter, NULL},
;5116:	{"mouseExit", ItemParse_mouseExit, NULL},
;5117:	{"mouseEnterText", ItemParse_mouseEnterText, NULL},
;5118:	{"mouseExitText", ItemParse_mouseExitText, NULL},
;5119:	{"action", ItemParse_action, NULL},
;5120:	{"special", ItemParse_special, NULL},
;5121:	{"cvar", ItemParse_cvar, NULL},
;5122:	{"maxChars", ItemParse_maxChars, NULL},
;5123:	{"maxPaintChars", ItemParse_maxPaintChars, NULL},
;5124:	{"focusSound", ItemParse_focusSound, NULL},
;5125:	{"cvarFloat", ItemParse_cvarFloat, NULL},
;5126:	{"cvarStrList", ItemParse_cvarStrList, NULL},
;5127:	{"cvarFloatList", ItemParse_cvarFloatList, NULL},
;5128:	{"addColorRange", ItemParse_addColorRange, NULL},
;5129:	{"ownerdrawFlag", ItemParse_ownerdrawFlag, NULL},
;5130:	{"enableCvar", ItemParse_enableCvar, NULL},
;5131:	{"cvarTest", ItemParse_cvarTest, NULL},
;5132:	{"disableCvar", ItemParse_disableCvar, NULL},
;5133:	{"showCvar", ItemParse_showCvar, NULL},
;5134:	{"hideCvar", ItemParse_hideCvar, NULL},
;5135:	{"cinematic", ItemParse_cinematic, NULL},
;5136:	{"doubleclick", ItemParse_doubleClick, NULL},
;5137:	{NULL, NULL, NULL}
;5138:};
;5139:
;5140:keywordHash_t *itemParseKeywordHash[KEYWORDHASH_SIZE];
;5141:
;5142:/*
;5143:===============
;5144:Item_SetupKeywordHash
;5145:===============
;5146:*/
;5147:void Item_SetupKeywordHash(void) {
line 5150
;5148:	int i;
;5149:
;5150:	memset(itemParseKeywordHash, 0, sizeof(itemParseKeywordHash));
ADDRGP4 itemParseKeywordHash
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 5151
;5151:	for (i = 0; itemParseKeywords[i].keyword; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2381
JUMPV
LABELV $2378
line 5152
;5152:		KeywordHash_Add(itemParseKeywordHash, &itemParseKeywords[i]);
ADDRGP4 itemParseKeywordHash
ARGP4
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 itemParseKeywords
ADDP4
ARGP4
ADDRGP4 KeywordHash_Add
CALLV
pop
line 5153
;5153:	}
LABELV $2379
line 5151
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2381
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 itemParseKeywords
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2378
line 5154
;5154:}
LABELV $2377
endproc Item_SetupKeywordHash 4 12
export Item_Parse
proc Item_Parse 1060 12
line 5161
;5155:
;5156:/*
;5157:===============
;5158:Item_Parse
;5159:===============
;5160:*/
;5161:qboolean Item_Parse(int handle, itemDef_t *item) {
line 5166
;5162:	pc_token_t token;
;5163:	keywordHash_t *key;
;5164:
;5165:
;5166:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2383
line 5167
;5167:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2382
JUMPV
LABELV $2383
line 5168
;5168:	if (*token.string != '{') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $2389
line 5169
;5169:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2382
JUMPV
LABELV $2388
line 5171
;5170:	}
;5171:	while ( 1 ) {
line 5172
;5172:		if (!trap_PC_ReadToken(handle, &token)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $2391
line 5173
;5173:			PC_SourceError(handle, "end of file inside menu item\n");
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2255
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5174
;5174:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2382
JUMPV
LABELV $2391
line 5177
;5175:		}
;5176:
;5177:		if (*token.string == '}') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $2393
line 5178
;5178:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2382
JUMPV
LABELV $2393
line 5181
;5179:		}
;5180:
;5181:		key = KeywordHash_Find(itemParseKeywordHash, token.string);
ADDRGP4 itemParseKeywordHash
ARGP4
ADDRLP4 0+16
ARGP4
ADDRLP4 1052
ADDRGP4 KeywordHash_Find
CALLP4
ASGNP4
ADDRLP4 1040
ADDRLP4 1052
INDIRP4
ASGNP4
line 5182
;5182:		if (!key) {
ADDRLP4 1040
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2397
line 5183
;5183:			PC_SourceError(handle, "unknown menu item keyword %s", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2399
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5184
;5184:			continue;
ADDRGP4 $2389
JUMPV
LABELV $2397
line 5186
;5185:		}
;5186:		if ( !key->func(item, handle) ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1056
ADDRLP4 1040
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $2401
line 5187
;5187:			PC_SourceError(handle, "couldn't parse menu item keyword %s", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2403
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5188
;5188:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2382
JUMPV
LABELV $2401
line 5190
;5189:		}
;5190:	}
LABELV $2389
line 5171
ADDRGP4 $2388
JUMPV
line 5191
;5191:	return qfalse; 	// bk001205 - LCC missing return value
CNSTI4 0
RETI4
LABELV $2382
endproc Item_Parse 1060 12
export Item_InitControls
proc Item_InitControls 4 0
line 5197
;5192:}
;5193:
;5194:
;5195:// Item_InitControls
;5196:// init's special control types
;5197:void Item_InitControls(itemDef_t *item) {
line 5198
;5198:	if (item == NULL) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2406
line 5199
;5199:		return;
ADDRGP4 $2405
JUMPV
LABELV $2406
line 5201
;5200:	}
;5201:	if (item->type == ITEM_TYPE_LISTBOX) {
ADDRFP4 0
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 6
NEI4 $2408
line 5202
;5202:		listBoxDef_t *listPtr = (listBoxDef_t*)item->typeData;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 5203
;5203:		item->cursorPos = 0;
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
CNSTI4 0
ASGNI4
line 5204
;5204:		if (listPtr) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2410
line 5205
;5205:			listPtr->cursorPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 5206
;5206:			listPtr->startPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 5207
;5207:			listPtr->endPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 5208
;5208:			listPtr->cursorPos = 0;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 5209
;5209:		}
LABELV $2410
line 5210
;5210:	}
LABELV $2408
line 5211
;5211:}
LABELV $2405
endproc Item_InitControls 4 0
export MenuParse_font
proc MenuParse_font 12 12
line 5219
;5212:
;5213:/*
;5214:===============
;5215:Menu Keyword Parse functions
;5216:===============
;5217:*/
;5218:
;5219:qboolean MenuParse_font( itemDef_t *item, int handle ) {
line 5220
;5220:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5221
;5221:	if (!PC_String_Parse(handle, &menu->font)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2413
line 5222
;5222:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2412
JUMPV
LABELV $2413
line 5224
;5223:	}
;5224:	if (!DC->Assets.fontRegistered) {
ADDRGP4 DC
INDIRP4
CNSTI4 61992
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2415
line 5225
;5225:		DC->registerFont(menu->font, 48, &DC->Assets.textFont);
ADDRLP4 0
INDIRP4
CNSTI4 180
ADDP4
INDIRP4
ARGP4
CNSTI4 48
ARGI4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 240
ADDP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 64
ADDP4
INDIRP4
CALLV
pop
line 5226
;5226:		DC->Assets.fontRegistered = qtrue;
ADDRGP4 DC
INDIRP4
CNSTI4 61992
ADDP4
CNSTI4 1
ASGNI4
line 5227
;5227:	}
LABELV $2415
line 5228
;5228:	return qtrue;
CNSTI4 1
RETI4
LABELV $2412
endproc MenuParse_font 12 12
export MenuParse_name
proc MenuParse_name 12 8
line 5231
;5229:}
;5230:
;5231:qboolean MenuParse_name( itemDef_t *item, int handle ) {
line 5232
;5232:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5233
;5233:	if (!PC_String_Parse(handle, &menu->window.name)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2418
line 5234
;5234:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2417
JUMPV
LABELV $2418
line 5236
;5235:	}
;5236:	if (Q_stricmp(menu->window.name, "main") == 0) {
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRP4
ARGP4
ADDRGP4 $2422
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2420
line 5239
;5237:		// default main as having focus
;5238:		//menu->window.flags |= WINDOW_HASFOCUS;
;5239:	}
LABELV $2420
line 5240
;5240:	return qtrue;
CNSTI4 1
RETI4
LABELV $2417
endproc MenuParse_name 12 8
export MenuParse_fullscreen
proc MenuParse_fullscreen 8 8
line 5243
;5241:}
;5242:
;5243:qboolean MenuParse_fullscreen( itemDef_t *item, int handle ) {
line 5244
;5244:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5245
;5245:	if (!PC_Int_Parse(handle, (int*) &menu->fullScreen)) { // bk001206 - cast qboolean
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2424
line 5246
;5246:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2423
JUMPV
LABELV $2424
line 5248
;5247:	}
;5248:	return qtrue;
CNSTI4 1
RETI4
LABELV $2423
endproc MenuParse_fullscreen 8 8
export MenuParse_rect
proc MenuParse_rect 8 8
line 5251
;5249:}
;5250:
;5251:qboolean MenuParse_rect( itemDef_t *item, int handle ) {
line 5252
;5252:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5253
;5253:	if (!PC_Rect_Parse(handle, &menu->window.rect)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Rect_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2427
line 5254
;5254:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2426
JUMPV
LABELV $2427
line 5256
;5255:	}
;5256:	return qtrue;
CNSTI4 1
RETI4
LABELV $2426
endproc MenuParse_rect 8 8
export MenuParse_style
proc MenuParse_style 8 8
line 5259
;5257:}
;5258:
;5259:qboolean MenuParse_style( itemDef_t *item, int handle ) {
line 5260
;5260:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5261
;5261:	if (!PC_Int_Parse(handle, &menu->window.style)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2430
line 5262
;5262:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2429
JUMPV
LABELV $2430
line 5264
;5263:	}
;5264:	return qtrue;
CNSTI4 1
RETI4
LABELV $2429
endproc MenuParse_style 8 8
export MenuParse_visible
proc MenuParse_visible 16 8
line 5267
;5265:}
;5266:
;5267:qboolean MenuParse_visible( itemDef_t *item, int handle ) {
line 5269
;5268:	int i;
;5269:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 5271
;5270:
;5271:	if (!PC_Int_Parse(handle, &i)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2433
line 5272
;5272:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2432
JUMPV
LABELV $2433
line 5274
;5273:	}
;5274:	if (i) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $2435
line 5275
;5275:		menu->window.flags |= WINDOW_VISIBLE;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 5276
;5276:	}
LABELV $2435
line 5277
;5277:	return qtrue;
CNSTI4 1
RETI4
LABELV $2432
endproc MenuParse_visible 16 8
export MenuParse_onOpen
proc MenuParse_onOpen 8 8
line 5280
;5278:}
;5279:
;5280:qboolean MenuParse_onOpen( itemDef_t *item, int handle ) {
line 5281
;5281:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5282
;5282:	if (!PC_Script_Parse(handle, &menu->onOpen)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2438
line 5283
;5283:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2437
JUMPV
LABELV $2438
line 5285
;5284:	}
;5285:	return qtrue;
CNSTI4 1
RETI4
LABELV $2437
endproc MenuParse_onOpen 8 8
export MenuParse_onClose
proc MenuParse_onClose 8 8
line 5288
;5286:}
;5287:
;5288:qboolean MenuParse_onClose( itemDef_t *item, int handle ) {
line 5289
;5289:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5290
;5290:	if (!PC_Script_Parse(handle, &menu->onClose)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2441
line 5291
;5291:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2440
JUMPV
LABELV $2441
line 5293
;5292:	}
;5293:	return qtrue;
CNSTI4 1
RETI4
LABELV $2440
endproc MenuParse_onClose 8 8
export MenuParse_onESC
proc MenuParse_onESC 8 8
line 5296
;5294:}
;5295:
;5296:qboolean MenuParse_onESC( itemDef_t *item, int handle ) {
line 5297
;5297:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5298
;5298:	if (!PC_Script_Parse(handle, &menu->onESC)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Script_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2444
line 5299
;5299:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2443
JUMPV
LABELV $2444
line 5301
;5300:	}
;5301:	return qtrue;
CNSTI4 1
RETI4
LABELV $2443
endproc MenuParse_onESC 8 8
export MenuParse_border
proc MenuParse_border 8 8
line 5306
;5302:}
;5303:
;5304:
;5305:
;5306:qboolean MenuParse_border( itemDef_t *item, int handle ) {
line 5307
;5307:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5308
;5308:	if (!PC_Int_Parse(handle, &menu->window.border)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2447
line 5309
;5309:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2446
JUMPV
LABELV $2447
line 5311
;5310:	}
;5311:	return qtrue;
CNSTI4 1
RETI4
LABELV $2446
endproc MenuParse_border 8 8
export MenuParse_borderSize
proc MenuParse_borderSize 8 8
line 5314
;5312:}
;5313:
;5314:qboolean MenuParse_borderSize( itemDef_t *item, int handle ) {
line 5315
;5315:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5316
;5316:	if (!PC_Float_Parse(handle, &menu->window.borderSize)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2450
line 5317
;5317:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2449
JUMPV
LABELV $2450
line 5319
;5318:	}
;5319:	return qtrue;
CNSTI4 1
RETI4
LABELV $2449
endproc MenuParse_borderSize 8 8
export MenuParse_backcolor
proc MenuParse_backcolor 16 8
line 5322
;5320:}
;5321:
;5322:qboolean MenuParse_backcolor( itemDef_t *item, int handle ) {
line 5325
;5323:	int i;
;5324:	float f;
;5325:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
line 5327
;5326:
;5327:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2453
line 5328
;5328:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2457
line 5329
;5329:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2452
JUMPV
LABELV $2457
line 5331
;5330:		}
;5331:		menu->window.backColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 128
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 5332
;5332:	}
LABELV $2454
line 5327
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2453
line 5333
;5333:	return qtrue;
CNSTI4 1
RETI4
LABELV $2452
endproc MenuParse_backcolor 16 8
export MenuParse_forecolor
proc MenuParse_forecolor 20 8
line 5336
;5334:}
;5335:
;5336:qboolean MenuParse_forecolor( itemDef_t *item, int handle ) {
line 5339
;5337:	int i;
;5338:	float f;
;5339:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 5341
;5340:
;5341:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2460
line 5342
;5342:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2464
line 5343
;5343:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2459
JUMPV
LABELV $2464
line 5345
;5344:		}
;5345:		menu->window.foreColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
line 5346
;5346:		menu->window.flags |= WINDOW_FORECOLORSET;
ADDRLP4 16
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 5347
;5347:	}
LABELV $2461
line 5341
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2460
line 5348
;5348:	return qtrue;
CNSTI4 1
RETI4
LABELV $2459
endproc MenuParse_forecolor 20 8
export MenuParse_bordercolor
proc MenuParse_bordercolor 16 8
line 5351
;5349:}
;5350:
;5351:qboolean MenuParse_bordercolor( itemDef_t *item, int handle ) {
line 5354
;5352:	int i;
;5353:	float f;
;5354:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
line 5356
;5355:
;5356:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2467
line 5357
;5357:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2471
line 5358
;5358:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2466
JUMPV
LABELV $2471
line 5360
;5359:		}
;5360:		menu->window.borderColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 144
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 5361
;5361:	}
LABELV $2468
line 5356
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2467
line 5362
;5362:	return qtrue;
CNSTI4 1
RETI4
LABELV $2466
endproc MenuParse_bordercolor 16 8
export MenuParse_focuscolor
proc MenuParse_focuscolor 16 8
line 5365
;5363:}
;5364:
;5365:qboolean MenuParse_focuscolor( itemDef_t *item, int handle ) {
line 5368
;5366:	int i;
;5367:	float f;
;5368:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
line 5370
;5369:
;5370:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2474
line 5371
;5371:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2478
line 5372
;5372:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2473
JUMPV
LABELV $2478
line 5374
;5373:		}
;5374:		menu->focusColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 228
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 5375
;5375:	}
LABELV $2475
line 5370
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2474
line 5376
;5376:	return qtrue;
CNSTI4 1
RETI4
LABELV $2473
endproc MenuParse_focuscolor 16 8
export MenuParse_disablecolor
proc MenuParse_disablecolor 16 8
line 5379
;5377:}
;5378:
;5379:qboolean MenuParse_disablecolor( itemDef_t *item, int handle ) {
line 5382
;5380:	int i;
;5381:	float f;
;5382:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
line 5383
;5383:	for (i = 0; i < 4; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $2481
line 5384
;5384:		if (!PC_Float_Parse(handle, &f)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
ADDRLP4 12
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $2485
line 5385
;5385:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2480
JUMPV
LABELV $2485
line 5387
;5386:		}
;5387:		menu->disableColor[i]  = f;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRP4
CNSTI4 244
ADDP4
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 5388
;5388:	}
LABELV $2482
line 5383
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LTI4 $2481
line 5389
;5389:	return qtrue;
CNSTI4 1
RETI4
LABELV $2480
endproc MenuParse_disablecolor 16 8
export MenuParse_outlinecolor
proc MenuParse_outlinecolor 8 8
line 5393
;5390:}
;5391:
;5392:
;5393:qboolean MenuParse_outlinecolor( itemDef_t *item, int handle ) {
line 5394
;5394:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5395
;5395:	if (!PC_Color_Parse(handle, &menu->window.outlineColor)){
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 160
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Color_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2488
line 5396
;5396:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2487
JUMPV
LABELV $2488
line 5398
;5397:	}
;5398:	return qtrue;
CNSTI4 1
RETI4
LABELV $2487
endproc MenuParse_outlinecolor 8 8
export MenuParse_background
proc MenuParse_background 16 8
line 5401
;5399:}
;5400:
;5401:qboolean MenuParse_background( itemDef_t *item, int handle ) {
line 5403
;5402:	const char *buff;
;5403:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 5405
;5404:
;5405:	if (!PC_String_Parse(handle, &buff)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2491
line 5406
;5406:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2490
JUMPV
LABELV $2491
line 5408
;5407:	}
;5408:	menu->window.background = DC->registerShaderNoMip(buff);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 DC
INDIRP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 176
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 5409
;5409:	return qtrue;
CNSTI4 1
RETI4
LABELV $2490
endproc MenuParse_background 16 8
export MenuParse_cinematic
proc MenuParse_cinematic 8 8
line 5412
;5410:}
;5411:
;5412:qboolean MenuParse_cinematic( itemDef_t *item, int handle ) {
line 5413
;5413:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5415
;5414:
;5415:	if (!PC_String_Parse(handle, &menu->window.cinematicName)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2494
line 5416
;5416:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2493
JUMPV
LABELV $2494
line 5418
;5417:	}
;5418:	return qtrue;
CNSTI4 1
RETI4
LABELV $2493
endproc MenuParse_cinematic 8 8
export MenuParse_ownerdrawFlag
proc MenuParse_ownerdrawFlag 16 8
line 5421
;5419:}
;5420:
;5421:qboolean MenuParse_ownerdrawFlag( itemDef_t *item, int handle ) {
line 5423
;5422:	int i;
;5423:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 5425
;5424:
;5425:	if (!PC_Int_Parse(handle, &i)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 8
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $2497
line 5426
;5426:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2496
JUMPV
LABELV $2497
line 5428
;5427:	}
;5428:	menu->window.ownerDrawFlags |= i;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 60
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 5429
;5429:	return qtrue;
CNSTI4 1
RETI4
LABELV $2496
endproc MenuParse_ownerdrawFlag 16 8
export MenuParse_ownerdraw
proc MenuParse_ownerdraw 8 8
line 5432
;5430:}
;5431:
;5432:qboolean MenuParse_ownerdraw( itemDef_t *item, int handle ) {
line 5433
;5433:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5435
;5434:
;5435:	if (!PC_Int_Parse(handle, &menu->window.ownerDraw)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 56
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2500
line 5436
;5436:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2499
JUMPV
LABELV $2500
line 5438
;5437:	}
;5438:	return qtrue;
CNSTI4 1
RETI4
LABELV $2499
endproc MenuParse_ownerdraw 8 8
export MenuParse_popup
proc MenuParse_popup 8 0
line 5443
;5439:}
;5440:
;5441:
;5442:// decoration
;5443:qboolean MenuParse_popup( itemDef_t *item, int handle ) {
line 5444
;5444:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5445
;5445:	menu->window.flags |= WINDOW_POPUP;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 2097152
BORI4
ASGNI4
line 5446
;5446:	return qtrue;
CNSTI4 1
RETI4
LABELV $2502
endproc MenuParse_popup 8 0
export MenuParse_outOfBounds
proc MenuParse_outOfBounds 8 0
line 5450
;5447:}
;5448:
;5449:
;5450:qboolean MenuParse_outOfBounds( itemDef_t *item, int handle ) {
line 5451
;5451:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5453
;5452:
;5453:	menu->window.flags |= WINDOW_OOB_CLICK;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 131072
BORI4
ASGNI4
line 5454
;5454:	return qtrue;
CNSTI4 1
RETI4
LABELV $2503
endproc MenuParse_outOfBounds 8 0
export MenuParse_soundLoop
proc MenuParse_soundLoop 8 8
line 5457
;5455:}
;5456:
;5457:qboolean MenuParse_soundLoop( itemDef_t *item, int handle ) {
line 5458
;5458:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5460
;5459:
;5460:	if (!PC_String_Parse(handle, &menu->soundName)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_String_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2505
line 5461
;5461:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2504
JUMPV
LABELV $2505
line 5463
;5462:	}
;5463:	return qtrue;
CNSTI4 1
RETI4
LABELV $2504
endproc MenuParse_soundLoop 8 8
export MenuParse_fadeClamp
proc MenuParse_fadeClamp 8 8
line 5466
;5464:}
;5465:
;5466:qboolean MenuParse_fadeClamp( itemDef_t *item, int handle ) {
line 5467
;5467:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5469
;5468:
;5469:	if (!PC_Float_Parse(handle, &menu->fadeClamp)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2508
line 5470
;5470:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2507
JUMPV
LABELV $2508
line 5472
;5471:	}
;5472:	return qtrue;
CNSTI4 1
RETI4
LABELV $2507
endproc MenuParse_fadeClamp 8 8
export MenuParse_fadeAmount
proc MenuParse_fadeAmount 8 8
line 5475
;5473:}
;5474:
;5475:qboolean MenuParse_fadeAmount( itemDef_t *item, int handle ) {
line 5476
;5476:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5478
;5477:
;5478:	if (!PC_Float_Parse(handle, &menu->fadeAmount)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Float_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2511
line 5479
;5479:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2510
JUMPV
LABELV $2511
line 5481
;5480:	}
;5481:	return qtrue;
CNSTI4 1
RETI4
LABELV $2510
endproc MenuParse_fadeAmount 8 8
export MenuParse_fadeCycle
proc MenuParse_fadeCycle 8 8
line 5485
;5482:}
;5483:
;5484:
;5485:qboolean MenuParse_fadeCycle( itemDef_t *item, int handle ) {
line 5486
;5486:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5488
;5487:
;5488:	if (!PC_Int_Parse(handle, &menu->fadeCycle)) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 PC_Int_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $2514
line 5489
;5489:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2513
JUMPV
LABELV $2514
line 5491
;5490:	}
;5491:	return qtrue;
CNSTI4 1
RETI4
LABELV $2513
endproc MenuParse_fadeCycle 8 8
export MenuParse_itemDef
proc MenuParse_itemDef 40 8
line 5495
;5492:}
;5493:
;5494:
;5495:qboolean MenuParse_itemDef( itemDef_t *item, int handle ) {
line 5496
;5496:	menuDef_t *menu = (menuDef_t*)item;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 5497
;5497:	if (menu->itemCount < MAX_MENUITEMS) {
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 96
GEI4 $2517
line 5498
;5498:		menu->items[menu->itemCount] = UI_Alloc(sizeof(itemDef_t));
CNSTI4 540
ARGI4
ADDRLP4 4
ADDRGP4 UI_Alloc
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 5499
;5499:		Item_Init(menu->items[menu->itemCount]);
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_Init
CALLV
pop
line 5500
;5500:		if (!Item_Parse(handle, menu->items[menu->itemCount])) {
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 Item_Parse
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $2519
line 5501
;5501:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2516
JUMPV
LABELV $2519
line 5503
;5502:		}
;5503:		Item_InitControls(menu->items[menu->itemCount]);
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_InitControls
CALLV
pop
line 5504
;5504:		menu->items[menu->itemCount++]->parent = menu;
ADDRLP4 32
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
ASGNP4
ADDRLP4 28
ADDRLP4 32
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 32
INDIRP4
ADDRLP4 28
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 228
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 5505
;5505:	}
LABELV $2517
line 5506
;5506:	return qtrue;
CNSTI4 1
RETI4
LABELV $2516
endproc MenuParse_itemDef 40 8
data
export menuParseKeywords
align 4
LABELV menuParseKeywords
address $2521
address MenuParse_font
byte 4 0
address $2318
address MenuParse_name
byte 4 0
address $2522
address MenuParse_fullscreen
byte 4 0
address $2328
address MenuParse_rect
byte 4 0
address $2329
address MenuParse_style
byte 4 0
address $2343
address MenuParse_visible
byte 4 0
address $2523
address MenuParse_onOpen
byte 4 0
address $2524
address MenuParse_onClose
byte 4 0
address $2525
address MenuParse_onESC
byte 4 0
address $2341
address MenuParse_border
byte 4 0
address $2526
address MenuParse_borderSize
byte 4 0
address $358
address MenuParse_backcolor
byte 4 0
address $361
address MenuParse_forecolor
byte 4 0
address $364
address MenuParse_bordercolor
byte 4 0
address $2527
address MenuParse_focuscolor
byte 4 0
address $2528
address MenuParse_disablecolor
byte 4 0
address $2351
address MenuParse_outlinecolor
byte 4 0
address $2352
address MenuParse_background
byte 4 0
address $2344
address MenuParse_ownerdraw
byte 4 0
address $2369
address MenuParse_ownerdrawFlag
byte 4 0
address $2529
address MenuParse_outOfBounds
byte 4 0
address $2530
address MenuParse_soundLoop
byte 4 0
address $2531
address MenuParse_itemDef
byte 4 0
address $2375
address MenuParse_cinematic
byte 4 0
address $2532
address MenuParse_popup
byte 4 0
address $2533
address MenuParse_fadeClamp
byte 4 0
address $2534
address MenuParse_fadeCycle
byte 4 0
address $2535
address MenuParse_fadeAmount
byte 4 0
byte 4 0
byte 4 0
byte 4 0
export Menu_SetupKeywordHash
code
proc Menu_SetupKeywordHash 4 12
line 5548
;5507:}
;5508:
;5509:keywordHash_t menuParseKeywords[] = {
;5510:	{"font", MenuParse_font, NULL},
;5511:	{"name", MenuParse_name, NULL},
;5512:	{"fullscreen", MenuParse_fullscreen, NULL},
;5513:	{"rect", MenuParse_rect, NULL},
;5514:	{"style", MenuParse_style, NULL},
;5515:	{"visible", MenuParse_visible, NULL},
;5516:	{"onOpen", MenuParse_onOpen, NULL},
;5517:	{"onClose", MenuParse_onClose, NULL},
;5518:	{"onESC", MenuParse_onESC, NULL},
;5519:	{"border", MenuParse_border, NULL},
;5520:	{"borderSize", MenuParse_borderSize, NULL},
;5521:	{"backcolor", MenuParse_backcolor, NULL},
;5522:	{"forecolor", MenuParse_forecolor, NULL},
;5523:	{"bordercolor", MenuParse_bordercolor, NULL},
;5524:	{"focuscolor", MenuParse_focuscolor, NULL},
;5525:	{"disablecolor", MenuParse_disablecolor, NULL},
;5526:	{"outlinecolor", MenuParse_outlinecolor, NULL},
;5527:	{"background", MenuParse_background, NULL},
;5528:	{"ownerdraw", MenuParse_ownerdraw, NULL},
;5529:	{"ownerdrawFlag", MenuParse_ownerdrawFlag, NULL},
;5530:	{"outOfBoundsClick", MenuParse_outOfBounds, NULL},
;5531:	{"soundLoop", MenuParse_soundLoop, NULL},
;5532:	{"itemDef", MenuParse_itemDef, NULL},
;5533:	{"cinematic", MenuParse_cinematic, NULL},
;5534:	{"popup", MenuParse_popup, NULL},
;5535:	{"fadeClamp", MenuParse_fadeClamp, NULL},
;5536:	{"fadeCycle", MenuParse_fadeCycle, NULL},
;5537:	{"fadeAmount", MenuParse_fadeAmount, NULL},
;5538:	{NULL, NULL, NULL}
;5539:};
;5540:
;5541:keywordHash_t *menuParseKeywordHash[KEYWORDHASH_SIZE];
;5542:
;5543:/*
;5544:===============
;5545:Menu_SetupKeywordHash
;5546:===============
;5547:*/
;5548:void Menu_SetupKeywordHash(void) {
line 5551
;5549:	int i;
;5550:
;5551:	memset(menuParseKeywordHash, 0, sizeof(menuParseKeywordHash));
ADDRGP4 menuParseKeywordHash
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 5552
;5552:	for (i = 0; menuParseKeywords[i].keyword; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2540
JUMPV
LABELV $2537
line 5553
;5553:		KeywordHash_Add(menuParseKeywordHash, &menuParseKeywords[i]);
ADDRGP4 menuParseKeywordHash
ARGP4
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 menuParseKeywords
ADDP4
ARGP4
ADDRGP4 KeywordHash_Add
CALLV
pop
line 5554
;5554:	}
LABELV $2538
line 5552
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2540
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 menuParseKeywords
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2537
line 5555
;5555:}
LABELV $2536
endproc Menu_SetupKeywordHash 4 12
export Menu_Parse
proc Menu_Parse 1060 12
line 5562
;5556:
;5557:/*
;5558:===============
;5559:Menu_Parse
;5560:===============
;5561:*/
;5562:qboolean Menu_Parse(int handle, menuDef_t *menu) {
line 5566
;5563:	pc_token_t token;
;5564:	keywordHash_t *key;
;5565:
;5566:	if (!trap_PC_ReadToken(handle, &token))
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1044
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $2542
line 5567
;5567:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2541
JUMPV
LABELV $2542
line 5568
;5568:	if (*token.string != '{') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 123
EQI4 $2548
line 5569
;5569:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2541
JUMPV
LABELV $2547
line 5572
;5570:	}
;5571:    
;5572:	while ( 1 ) {
line 5574
;5573:
;5574:		memset(&token, 0, sizeof(pc_token_t));
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1040
ARGI4
ADDRGP4 memset
CALLP4
pop
line 5575
;5575:		if (!trap_PC_ReadToken(handle, &token)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 1048
ADDRGP4 trap_PC_ReadToken
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $2550
line 5576
;5576:			PC_SourceError(handle, "end of file inside menu\n");
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2552
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5577
;5577:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2541
JUMPV
LABELV $2550
line 5580
;5578:		}
;5579:
;5580:		if (*token.string == '}') {
ADDRLP4 0+16
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $2553
line 5581
;5581:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2541
JUMPV
LABELV $2553
line 5584
;5582:		}
;5583:
;5584:		key = KeywordHash_Find(menuParseKeywordHash, token.string);
ADDRGP4 menuParseKeywordHash
ARGP4
ADDRLP4 0+16
ARGP4
ADDRLP4 1052
ADDRGP4 KeywordHash_Find
CALLP4
ASGNP4
ADDRLP4 1040
ADDRLP4 1052
INDIRP4
ASGNP4
line 5585
;5585:		if (!key) {
ADDRLP4 1040
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2557
line 5586
;5586:			PC_SourceError(handle, "unknown menu keyword %s", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2559
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5587
;5587:			continue;
ADDRGP4 $2548
JUMPV
LABELV $2557
line 5589
;5588:		}
;5589:		if ( !key->func((itemDef_t*)menu, handle) ) {
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1056
ADDRLP4 1040
INDIRP4
CNSTI4 4
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $2561
line 5590
;5590:			PC_SourceError(handle, "couldn't parse menu keyword %s", token.string);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $2563
ARGP4
ADDRLP4 0+16
ARGP4
ADDRGP4 PC_SourceError
CALLV
pop
line 5591
;5591:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $2541
JUMPV
LABELV $2561
line 5593
;5592:		}
;5593:	}
LABELV $2548
line 5572
ADDRGP4 $2547
JUMPV
line 5594
;5594:	return qfalse; 	// bk001205 - LCC missing return value
CNSTI4 0
RETI4
LABELV $2541
endproc Menu_Parse 1060 12
export Menu_New
proc Menu_New 12 8
line 5602
;5595:}
;5596:
;5597:/*
;5598:===============
;5599:Menu_New
;5600:===============
;5601:*/
;5602:void Menu_New(int handle) {
line 5603
;5603:	menuDef_t *menu = &Menus[menuCount];
ADDRLP4 0
CNSTI4 644
ADDRGP4 menuCount
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ASGNP4
line 5605
;5604:
;5605:	if (menuCount < MAX_MENUS) {
ADDRGP4 menuCount
INDIRI4
CNSTI4 64
GEI4 $2566
line 5606
;5606:		Menu_Init(menu);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_Init
CALLV
pop
line 5607
;5607:		if (Menu_Parse(handle, menu)) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Menu_Parse
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $2568
line 5608
;5608:			Menu_PostParse(menu);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Menu_PostParse
CALLV
pop
line 5609
;5609:			menuCount++;
ADDRLP4 8
ADDRGP4 menuCount
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 5610
;5610:		}
LABELV $2568
line 5611
;5611:	}
LABELV $2566
line 5612
;5612:}
LABELV $2565
endproc Menu_New 12 8
export Menu_Count
proc Menu_Count 0 0
line 5614
;5613:
;5614:int Menu_Count() {
line 5615
;5615:	return menuCount;
ADDRGP4 menuCount
INDIRI4
RETI4
LABELV $2570
endproc Menu_Count 0 0
lit
align 4
LABELV $2580
byte 4 1065353216
byte 4 1065353216
byte 4 1065353216
byte 4 1065353216
export Menu_PaintAll
code
proc Menu_PaintAll 32 32
line 5618
;5616:}
;5617:
;5618:void Menu_PaintAll() {
line 5620
;5619:	int i;
;5620:	if (captureFunc) {
ADDRGP4 captureFunc
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2572
line 5621
;5621:		captureFunc(captureData);
ADDRGP4 captureData
INDIRP4
ARGP4
ADDRGP4 captureFunc
INDIRP4
CALLV
pop
line 5622
;5622:	}
LABELV $2572
line 5624
;5623:
;5624:	for (i = 0; i < Menu_Count(); i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2577
JUMPV
LABELV $2574
line 5625
;5625:		Menu_Paint(&Menus[i], qfalse);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_Paint
CALLV
pop
line 5626
;5626:	}
LABELV $2575
line 5624
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2577
ADDRLP4 4
ADDRGP4 Menu_Count
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $2574
line 5628
;5627:
;5628:	if (debugMode) {
ADDRGP4 debugMode
INDIRI4
CNSTI4 0
EQI4 $2578
line 5629
;5629:		vec4_t v = {1, 1, 1, 1};
ADDRLP4 8
ADDRGP4 $2580
INDIRB
ASGNB 16
line 5630
;5630:		DC->drawText(5, 25, .5, v, va("fps: %f", DC->FPS), 0, 0, 0);
ADDRGP4 $2581
ARGP4
ADDRGP4 DC
INDIRP4
CNSTI4 73412
ADDP4
INDIRF4
ARGF4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
CNSTF4 1084227584
ARGF4
CNSTF4 1103626240
ARGF4
CNSTF4 1056964608
ARGF4
ADDRLP4 8
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRLP4 28
CNSTI4 0
ASGNI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 16
ADDP4
INDIRP4
CALLV
pop
line 5631
;5631:	}
LABELV $2578
line 5632
;5632:}
LABELV $2571
endproc Menu_PaintAll 32 32
export Menu_Reset
proc Menu_Reset 0 0
line 5634
;5633:
;5634:void Menu_Reset() {
line 5635
;5635:	menuCount = 0;
ADDRGP4 menuCount
CNSTI4 0
ASGNI4
line 5636
;5636:}
LABELV $2582
endproc Menu_Reset 0 0
export Display_GetContext
proc Display_GetContext 0 0
line 5638
;5637:
;5638:displayContextDef_t *Display_GetContext() {
line 5639
;5639:	return DC;
ADDRGP4 DC
INDIRP4
RETP4
LABELV $2583
endproc Display_GetContext 0 0
export Display_CaptureItem
proc Display_CaptureItem 8 12
line 5647
;5640:}
;5641: 
;5642:#ifndef MISSIONPACK // bk001206
;5643:static float captureX;
;5644:static float captureY;
;5645:#endif
;5646:
;5647:void *Display_CaptureItem(int x, int y) {
line 5650
;5648:	int i;
;5649:
;5650:	for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2588
JUMPV
LABELV $2585
line 5653
;5651:		// turn off focus each item
;5652:		// menu->items[i].window.flags &= ~WINDOW_HASFOCUS;
;5653:		if (Rect_ContainsPoint(&Menus[i].window.rect, x, y)) {
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRFP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 4
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $2589
line 5654
;5654:			return &Menus[i];
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
RETP4
ADDRGP4 $2584
JUMPV
LABELV $2589
line 5656
;5655:		}
;5656:	}
LABELV $2586
line 5650
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2588
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $2585
line 5657
;5657:	return NULL;
CNSTP4 0
RETP4
LABELV $2584
endproc Display_CaptureItem 8 12
export Display_MouseMove
proc Display_MouseMove 16 12
line 5662
;5658:}
;5659:
;5660:
;5661:// FIXME: 
;5662:qboolean Display_MouseMove(void *p, int x, int y) {
line 5664
;5663:	int i;
;5664:	menuDef_t *menu = p;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 5666
;5665:
;5666:	if (menu == NULL) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2592
line 5667
;5667:    menu = Menu_GetFocused();
ADDRLP4 8
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 5668
;5668:		if (menu) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2594
line 5669
;5669:			if (menu->window.flags & WINDOW_POPUP) {
ADDRLP4 4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 2097152
BANDI4
CNSTI4 0
EQI4 $2596
line 5670
;5670:				Menu_HandleMouseMove(menu, x, y);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 Menu_HandleMouseMove
CALLV
pop
line 5671
;5671:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2591
JUMPV
LABELV $2596
line 5673
;5672:			}
;5673:		}
LABELV $2594
line 5674
;5674:		for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2601
JUMPV
LABELV $2598
line 5675
;5675:			Menu_HandleMouseMove(&Menus[i], x, y);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRGP4 Menu_HandleMouseMove
CALLV
pop
line 5676
;5676:		}
LABELV $2599
line 5674
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2601
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $2598
line 5677
;5677:	} else {
ADDRGP4 $2593
JUMPV
LABELV $2592
line 5678
;5678:		menu->window.rect.x += x;
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRFP4 4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 5679
;5679:		menu->window.rect.y += y;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRFP4 8
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 5680
;5680:		Menu_UpdatePosition(menu);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 Menu_UpdatePosition
CALLV
pop
line 5681
;5681:	}
LABELV $2593
line 5682
;5682: 	return qtrue;
CNSTI4 1
RETI4
LABELV $2591
endproc Display_MouseMove 16 12
export Display_CursorType
proc Display_CursorType 28 12
line 5686
;5683:
;5684:}
;5685:
;5686:int Display_CursorType(int x, int y) {
line 5688
;5687:	int i;
;5688:	for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2606
JUMPV
LABELV $2603
line 5690
;5689:		rectDef_t r2;
;5690:		r2.x = Menus[i].window.rect.x - 3;
ADDRLP4 4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
INDIRF4
CNSTF4 1077936128
SUBF4
ASGNF4
line 5691
;5691:		r2.y = Menus[i].window.rect.y - 3;
ADDRLP4 4+4
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus+4
ADDP4
INDIRF4
CNSTF4 1077936128
SUBF4
ASGNF4
line 5692
;5692:		r2.w = r2.h = 7;
ADDRLP4 20
CNSTF4 1088421888
ASGNF4
ADDRLP4 4+12
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 20
INDIRF4
ASGNF4
line 5693
;5693:		if (Rect_ContainsPoint(&r2, x, y)) {
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 24
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
EQI4 $2611
line 5694
;5694:			return CURSOR_SIZER;
CNSTI4 4
RETI4
ADDRGP4 $2602
JUMPV
LABELV $2611
line 5696
;5695:		}
;5696:	}
LABELV $2604
line 5688
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2606
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $2603
line 5697
;5697:	return CURSOR_ARROW;
CNSTI4 2
RETI4
LABELV $2602
endproc Display_CursorType 28 12
export Display_HandleKey
proc Display_HandleKey 12 12
line 5701
;5698:}
;5699:
;5700:
;5701:void Display_HandleKey(int key, qboolean down, int x, int y) {
line 5702
;5702:	menuDef_t *menu = Display_CaptureItem(x, y);
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 Display_CaptureItem
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 5703
;5703:	if (menu == NULL) {  
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $2614
line 5704
;5704:		menu = Menu_GetFocused();
ADDRLP4 8
ADDRGP4 Menu_GetFocused
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 5705
;5705:	}
LABELV $2614
line 5706
;5706:	if (menu) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2616
line 5707
;5707:		Menu_HandleKey(menu, key, down );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Menu_HandleKey
CALLV
pop
line 5708
;5708:	}
LABELV $2616
line 5709
;5709:}
LABELV $2613
endproc Display_HandleKey 12 12
proc Window_CacheContents 12 20
line 5711
;5710:
;5711:static void Window_CacheContents(windowDef_t *window) {
line 5712
;5712:	if (window) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2619
line 5713
;5713:		if (window->cinematicName) {
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2621
line 5714
;5714:			int cin = DC->playCinematic(window->cinematicName, 0, 0, 0, 0);
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
ADDRGP4 DC
INDIRP4
CNSTI4 180
ADDP4
INDIRP4
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
line 5715
;5715:			DC->stopCinematic(cin);
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 184
ADDP4
INDIRP4
CALLV
pop
line 5716
;5716:		}
LABELV $2621
line 5717
;5717:	}
LABELV $2619
line 5718
;5718:}
LABELV $2618
endproc Window_CacheContents 12 20
proc Item_CacheContents 0 4
line 5721
;5719:
;5720:
;5721:static void Item_CacheContents(itemDef_t *item) {
line 5722
;5722:	if (item) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2624
line 5723
;5723:		Window_CacheContents(&item->window);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Window_CacheContents
CALLV
pop
line 5724
;5724:	}
LABELV $2624
line 5726
;5725:
;5726:}
LABELV $2623
endproc Item_CacheContents 0 4
proc Menu_CacheContents 8 8
line 5728
;5727:
;5728:static void Menu_CacheContents(menuDef_t *menu) {
line 5729
;5729:	if (menu) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2627
line 5731
;5730:		int i;
;5731:		Window_CacheContents(&menu->window);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Window_CacheContents
CALLV
pop
line 5732
;5732:		for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2632
JUMPV
LABELV $2629
line 5733
;5733:			Item_CacheContents(menu->items[i]);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRGP4 Item_CacheContents
CALLV
pop
line 5734
;5734:		}
LABELV $2630
line 5732
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2632
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $2629
line 5736
;5735:
;5736:		if (menu->soundName && *menu->soundName) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2633
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $2633
line 5737
;5737:			DC->registerSound(menu->soundName, qfalse);
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 DC
INDIRP4
CNSTI4 168
ADDP4
INDIRP4
CALLI4
pop
line 5738
;5738:		}
LABELV $2633
line 5739
;5739:	}
LABELV $2627
line 5741
;5740:
;5741:}
LABELV $2626
endproc Menu_CacheContents 8 8
export Display_CacheAll
proc Display_CacheAll 4 4
line 5743
;5742:
;5743:void Display_CacheAll() {
line 5745
;5744:	int i;
;5745:	for (i = 0; i < menuCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $2639
JUMPV
LABELV $2636
line 5746
;5746:		Menu_CacheContents(&Menus[i]);
CNSTI4 644
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 Menus
ADDP4
ARGP4
ADDRGP4 Menu_CacheContents
CALLV
pop
line 5747
;5747:	}
LABELV $2637
line 5745
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2639
ADDRLP4 0
INDIRI4
ADDRGP4 menuCount
INDIRI4
LTI4 $2636
line 5748
;5748:}
LABELV $2635
endproc Display_CacheAll 4 4
proc Menu_OverActiveItem 32 12
line 5751
;5749:
;5750:
;5751:static qboolean Menu_OverActiveItem(menuDef_t *menu, float x, float y) {
line 5752
;5752: 	if (menu && menu->window.flags & (WINDOW_VISIBLE | WINDOW_FORCED)) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2641
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1048580
BANDI4
CNSTI4 0
EQI4 $2641
line 5753
;5753:		if (Rect_ContainsPoint(&menu->window.rect, x, y)) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 4
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $2643
line 5755
;5754:			int i;
;5755:			for (i = 0; i < menu->itemCount; i++) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $2648
JUMPV
LABELV $2645
line 5759
;5756:				// turn off focus each item
;5757:				// menu->items[i].window.flags &= ~WINDOW_HASFOCUS;
;5758:
;5759:				if (!(menu->items[i]->window.flags & (WINDOW_VISIBLE | WINDOW_FORCED))) {
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 1048580
BANDI4
CNSTI4 0
NEI4 $2649
line 5760
;5760:					continue;
ADDRGP4 $2646
JUMPV
LABELV $2649
line 5763
;5761:				}
;5762:
;5763:				if (menu->items[i]->window.flags & WINDOW_DECORATION) {
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
CNSTI4 68
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $2651
line 5764
;5764:					continue;
ADDRGP4 $2646
JUMPV
LABELV $2651
line 5767
;5765:				}
;5766:
;5767:				if (Rect_ContainsPoint(&menu->items[i]->window.rect, x, y)) {
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $2653
line 5768
;5768:					itemDef_t *overItem = menu->items[i];
ADDRLP4 16
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDP4
INDIRP4
ASGNP4
line 5769
;5769:					if (overItem->type == ITEM_TYPE_TEXT && overItem->text) {
ADDRLP4 16
INDIRP4
CNSTI4 196
ADDP4
INDIRI4
CNSTI4 0
NEI4 $2655
ADDRLP4 16
INDIRP4
CNSTI4 224
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $2655
line 5770
;5770:						if (Rect_ContainsPoint(Item_CorrectedTextRect(overItem), x, y)) {
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 Item_CorrectedTextRect
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 28
ADDRGP4 Rect_ContainsPoint
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $2646
line 5771
;5771:							return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2640
JUMPV
line 5772
;5772:						} else {
line 5773
;5773:							continue;
LABELV $2655
line 5775
;5774:						}
;5775:					} else {
line 5776
;5776:						return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $2640
JUMPV
LABELV $2653
line 5779
;5777:					}
;5778:				}
;5779:			}
LABELV $2646
line 5755
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $2648
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 188
ADDP4
INDIRI4
LTI4 $2645
line 5781
;5780:
;5781:		}
LABELV $2643
line 5782
;5782:	}
LABELV $2641
line 5783
;5783:	return qfalse;
CNSTI4 0
RETI4
LABELV $2640
endproc Menu_OverActiveItem 32 12
bss
export menuParseKeywordHash
align 4
LABELV menuParseKeywordHash
skip 2048
export itemParseKeywordHash
align 4
LABELV itemParseKeywordHash
skip 2048
export g_nameBind2
align 1
LABELV g_nameBind2
skip 32
export g_nameBind1
align 1
LABELV g_nameBind1
skip 32
align 4
LABELV strHandle
skip 8192
align 1
LABELV strPool
skip 393216
align 4
LABELV outOfMemory
skip 4
align 4
LABELV allocPoint
skip 4
align 1
LABELV memoryPool
skip 1048576
export menuStack
align 4
LABELV menuStack
skip 64
export Menus
align 4
LABELV Menus
skip 41216
align 4
LABELV scrollInfo
skip 32
import trap_PC_SourceFileAndLine
import trap_PC_ReadToken
import trap_PC_FreeSource
import trap_PC_LoadSource
import trap_PC_AddGlobalDefine
import Script_Parse
import Display_ExpandMacros
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
LABELV $2581
byte 1 102
byte 1 112
byte 1 115
byte 1 58
byte 1 32
byte 1 37
byte 1 102
byte 1 0
align 1
LABELV $2563
byte 1 99
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 112
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $2559
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $2552
byte 1 101
byte 1 110
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 115
byte 1 105
byte 1 100
byte 1 101
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 10
byte 1 0
align 1
LABELV $2535
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 65
byte 1 109
byte 1 111
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $2534
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 67
byte 1 121
byte 1 99
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2533
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 67
byte 1 108
byte 1 97
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $2532
byte 1 112
byte 1 111
byte 1 112
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2531
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 68
byte 1 101
byte 1 102
byte 1 0
align 1
LABELV $2530
byte 1 115
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 76
byte 1 111
byte 1 111
byte 1 112
byte 1 0
align 1
LABELV $2529
byte 1 111
byte 1 117
byte 1 116
byte 1 79
byte 1 102
byte 1 66
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 115
byte 1 67
byte 1 108
byte 1 105
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $2528
byte 1 100
byte 1 105
byte 1 115
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $2527
byte 1 102
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $2526
byte 1 98
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 83
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $2525
byte 1 111
byte 1 110
byte 1 69
byte 1 83
byte 1 67
byte 1 0
align 1
LABELV $2524
byte 1 111
byte 1 110
byte 1 67
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $2523
byte 1 111
byte 1 110
byte 1 79
byte 1 112
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $2522
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
LABELV $2521
byte 1 102
byte 1 111
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $2422
byte 1 109
byte 1 97
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2403
byte 1 99
byte 1 111
byte 1 117
byte 1 108
byte 1 100
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 112
byte 1 97
byte 1 114
byte 1 115
byte 1 101
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $2399
byte 1 117
byte 1 110
byte 1 107
byte 1 110
byte 1 111
byte 1 119
byte 1 110
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 119
byte 1 111
byte 1 114
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $2376
byte 1 100
byte 1 111
byte 1 117
byte 1 98
byte 1 108
byte 1 101
byte 1 99
byte 1 108
byte 1 105
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $2375
byte 1 99
byte 1 105
byte 1 110
byte 1 101
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 99
byte 1 0
align 1
LABELV $2374
byte 1 104
byte 1 105
byte 1 100
byte 1 101
byte 1 67
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $2373
byte 1 115
byte 1 104
byte 1 111
byte 1 119
byte 1 67
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $2372
byte 1 100
byte 1 105
byte 1 115
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 67
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $2371
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 84
byte 1 101
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $2370
byte 1 101
byte 1 110
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 67
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $2369
byte 1 111
byte 1 119
byte 1 110
byte 1 101
byte 1 114
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $2368
byte 1 97
byte 1 100
byte 1 100
byte 1 67
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 82
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 0
align 1
LABELV $2367
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 70
byte 1 108
byte 1 111
byte 1 97
byte 1 116
byte 1 76
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $2366
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 83
byte 1 116
byte 1 114
byte 1 76
byte 1 105
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $2365
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 70
byte 1 108
byte 1 111
byte 1 97
byte 1 116
byte 1 0
align 1
LABELV $2364
byte 1 102
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 83
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $2363
byte 1 109
byte 1 97
byte 1 120
byte 1 80
byte 1 97
byte 1 105
byte 1 110
byte 1 116
byte 1 67
byte 1 104
byte 1 97
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $2362
byte 1 109
byte 1 97
byte 1 120
byte 1 67
byte 1 104
byte 1 97
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $2361
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $2360
byte 1 115
byte 1 112
byte 1 101
byte 1 99
byte 1 105
byte 1 97
byte 1 108
byte 1 0
align 1
LABELV $2359
byte 1 97
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $2358
byte 1 109
byte 1 111
byte 1 117
byte 1 115
byte 1 101
byte 1 69
byte 1 120
byte 1 105
byte 1 116
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $2357
byte 1 109
byte 1 111
byte 1 117
byte 1 115
byte 1 101
byte 1 69
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 84
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $2356
byte 1 109
byte 1 111
byte 1 117
byte 1 115
byte 1 101
byte 1 69
byte 1 120
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $2355
byte 1 109
byte 1 111
byte 1 117
byte 1 115
byte 1 101
byte 1 69
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2354
byte 1 108
byte 1 101
byte 1 97
byte 1 118
byte 1 101
byte 1 70
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $2353
byte 1 111
byte 1 110
byte 1 70
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $2352
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 103
byte 1 114
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $2351
byte 1 111
byte 1 117
byte 1 116
byte 1 108
byte 1 105
byte 1 110
byte 1 101
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $2350
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 115
byte 1 116
byte 1 121
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2349
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 115
byte 1 99
byte 1 97
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2348
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 97
byte 1 108
byte 1 105
byte 1 103
byte 1 110
byte 1 121
byte 1 0
align 1
LABELV $2347
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 97
byte 1 108
byte 1 105
byte 1 103
byte 1 110
byte 1 120
byte 1 0
align 1
LABELV $2346
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 97
byte 1 108
byte 1 105
byte 1 103
byte 1 110
byte 1 0
align 1
LABELV $2345
byte 1 97
byte 1 108
byte 1 105
byte 1 103
byte 1 110
byte 1 0
align 1
LABELV $2344
byte 1 111
byte 1 119
byte 1 110
byte 1 101
byte 1 114
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 0
align 1
LABELV $2343
byte 1 118
byte 1 105
byte 1 115
byte 1 105
byte 1 98
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2342
byte 1 98
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $2341
byte 1 98
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2340
byte 1 99
byte 1 111
byte 1 108
byte 1 117
byte 1 109
byte 1 110
byte 1 115
byte 1 0
align 1
LABELV $2339
byte 1 101
byte 1 108
byte 1 101
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $2338
byte 1 102
byte 1 101
byte 1 101
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2337
byte 1 101
byte 1 108
byte 1 101
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 104
byte 1 101
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $2336
byte 1 101
byte 1 108
byte 1 101
byte 1 109
byte 1 101
byte 1 110
byte 1 116
byte 1 119
byte 1 105
byte 1 100
byte 1 116
byte 1 104
byte 1 0
align 1
LABELV $2335
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 0
align 1
LABELV $2334
byte 1 104
byte 1 111
byte 1 114
byte 1 105
byte 1 122
byte 1 111
byte 1 110
byte 1 116
byte 1 97
byte 1 108
byte 1 115
byte 1 99
byte 1 114
byte 1 111
byte 1 108
byte 1 108
byte 1 0
align 1
LABELV $2333
byte 1 97
byte 1 117
byte 1 116
byte 1 111
byte 1 119
byte 1 114
byte 1 97
byte 1 112
byte 1 112
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $2332
byte 1 119
byte 1 114
byte 1 97
byte 1 112
byte 1 112
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $2331
byte 1 110
byte 1 111
byte 1 116
byte 1 115
byte 1 101
byte 1 108
byte 1 101
byte 1 99
byte 1 116
byte 1 97
byte 1 98
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2330
byte 1 100
byte 1 101
byte 1 99
byte 1 111
byte 1 114
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $2329
byte 1 115
byte 1 116
byte 1 121
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2328
byte 1 114
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $2327
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 97
byte 1 110
byte 1 103
byte 1 108
byte 1 101
byte 1 0
align 1
LABELV $2326
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 114
byte 1 111
byte 1 116
byte 1 97
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $2325
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 102
byte 1 111
byte 1 118
byte 1 121
byte 1 0
align 1
LABELV $2324
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 102
byte 1 111
byte 1 118
byte 1 120
byte 1 0
align 1
LABELV $2323
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 95
byte 1 111
byte 1 114
byte 1 105
byte 1 103
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $2322
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 95
byte 1 115
byte 1 104
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $2321
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 95
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $2320
byte 1 103
byte 1 114
byte 1 111
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $2319
byte 1 116
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $2318
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $2255
byte 1 101
byte 1 110
byte 1 100
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 102
byte 1 105
byte 1 108
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 115
byte 1 105
byte 1 100
byte 1 101
byte 1 32
byte 1 109
byte 1 101
byte 1 110
byte 1 117
byte 1 32
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 10
byte 1 0
align 1
LABELV $1609
byte 1 70
byte 1 73
byte 1 88
byte 1 77
byte 1 69
byte 1 0
align 1
LABELV $1579
byte 1 63
byte 1 63
byte 1 63
byte 1 0
align 1
LABELV $1578
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 0
align 1
LABELV $1548
byte 1 105
byte 1 110
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
LABELV $1515
byte 1 109
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 52
byte 1 0
align 1
LABELV $1514
byte 1 109
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 51
byte 1 0
align 1
LABELV $1513
byte 1 109
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 50
byte 1 0
align 1
LABELV $1512
byte 1 109
byte 1 101
byte 1 115
byte 1 115
byte 1 97
byte 1 103
byte 1 101
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $1511
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 68
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $1510
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 85
byte 1 112
byte 1 0
align 1
LABELV $1509
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 71
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $1508
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 68
byte 1 101
byte 1 97
byte 1 116
byte 1 104
byte 1 73
byte 1 110
byte 1 115
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $1507
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 84
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $1506
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 80
byte 1 114
byte 1 97
byte 1 105
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $1505
byte 1 116
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 75
byte 1 105
byte 1 108
byte 1 108
byte 1 73
byte 1 110
byte 1 115
byte 1 117
byte 1 108
byte 1 116
byte 1 0
align 1
LABELV $1504
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 83
byte 1 117
byte 1 105
byte 1 99
byte 1 105
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $1503
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 79
byte 1 119
byte 1 110
byte 1 70
byte 1 108
byte 1 97
byte 1 103
byte 1 0
align 1
LABELV $1502
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 69
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $1501
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 82
byte 1 101
byte 1 116
byte 1 114
byte 1 105
byte 1 101
byte 1 118
byte 1 101
byte 1 0
align 1
LABELV $1500
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 70
byte 1 111
byte 1 108
byte 1 108
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $1499
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 67
byte 1 97
byte 1 109
byte 1 112
byte 1 0
align 1
LABELV $1498
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 80
byte 1 97
byte 1 116
byte 1 114
byte 1 111
byte 1 108
byte 1 0
align 1
LABELV $1497
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 68
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $1496
byte 1 116
byte 1 97
byte 1 115
byte 1 107
byte 1 79
byte 1 102
byte 1 102
byte 1 101
byte 1 110
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $1495
byte 1 100
byte 1 101
byte 1 110
byte 1 121
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1494
byte 1 99
byte 1 111
byte 1 110
byte 1 102
byte 1 105
byte 1 114
byte 1 109
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1493
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1492
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 77
byte 1 101
byte 1 109
byte 1 98
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1491
byte 1 112
byte 1 114
byte 1 101
byte 1 118
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 77
byte 1 101
byte 1 109
byte 1 98
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1490
byte 1 43
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 111
byte 1 110
byte 1 52
byte 1 0
align 1
LABELV $1489
byte 1 43
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 111
byte 1 110
byte 1 51
byte 1 0
align 1
LABELV $1488
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 110
byte 1 101
byte 1 120
byte 1 116
byte 1 0
align 1
LABELV $1487
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 112
byte 1 114
byte 1 101
byte 1 118
byte 1 0
align 1
LABELV $1486
byte 1 43
byte 1 97
byte 1 116
byte 1 116
byte 1 97
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $1485
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 49
byte 1 51
byte 1 0
align 1
LABELV $1484
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 49
byte 1 50
byte 1 0
align 1
LABELV $1483
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 49
byte 1 49
byte 1 0
align 1
LABELV $1482
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 49
byte 1 48
byte 1 0
align 1
LABELV $1481
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 57
byte 1 0
align 1
LABELV $1480
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 56
byte 1 0
align 1
LABELV $1479
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 55
byte 1 0
align 1
LABELV $1478
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 54
byte 1 0
align 1
LABELV $1477
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 53
byte 1 0
align 1
LABELV $1476
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 52
byte 1 0
align 1
LABELV $1475
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 51
byte 1 0
align 1
LABELV $1474
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 50
byte 1 0
align 1
LABELV $1473
byte 1 119
byte 1 101
byte 1 97
byte 1 112
byte 1 111
byte 1 110
byte 1 32
byte 1 49
byte 1 0
align 1
LABELV $1472
byte 1 43
byte 1 122
byte 1 111
byte 1 111
byte 1 109
byte 1 0
align 1
LABELV $1471
byte 1 99
byte 1 101
byte 1 110
byte 1 116
byte 1 101
byte 1 114
byte 1 118
byte 1 105
byte 1 101
byte 1 119
byte 1 0
align 1
LABELV $1470
byte 1 43
byte 1 109
byte 1 108
byte 1 111
byte 1 111
byte 1 107
byte 1 0
align 1
LABELV $1469
byte 1 43
byte 1 108
byte 1 111
byte 1 111
byte 1 107
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $1468
byte 1 43
byte 1 108
byte 1 111
byte 1 111
byte 1 107
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $1467
byte 1 43
byte 1 115
byte 1 116
byte 1 114
byte 1 97
byte 1 102
byte 1 101
byte 1 0
align 1
LABELV $1466
byte 1 43
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $1465
byte 1 43
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 0
align 1
LABELV $1464
byte 1 43
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 0
align 1
LABELV $1463
byte 1 43
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $1462
byte 1 43
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 105
byte 1 103
byte 1 104
byte 1 116
byte 1 0
align 1
LABELV $1461
byte 1 43
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 0
align 1
LABELV $1460
byte 1 43
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 0
align 1
LABELV $1459
byte 1 43
byte 1 102
byte 1 111
byte 1 114
byte 1 119
byte 1 97
byte 1 114
byte 1 100
byte 1 0
align 1
LABELV $1458
byte 1 43
byte 1 115
byte 1 112
byte 1 101
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $1457
byte 1 43
byte 1 98
byte 1 117
byte 1 116
byte 1 116
byte 1 111
byte 1 110
byte 1 50
byte 1 0
align 1
LABELV $1456
byte 1 43
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $1439
byte 1 78
byte 1 111
byte 1 0
align 1
LABELV $1438
byte 1 89
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $1282
byte 1 115
byte 1 99
byte 1 114
byte 1 101
byte 1 101
byte 1 110
byte 1 115
byte 1 104
byte 1 111
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $1278
byte 1 100
byte 1 101
byte 1 118
byte 1 101
byte 1 108
byte 1 111
byte 1 112
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $1131
byte 1 115
byte 1 108
byte 1 105
byte 1 100
byte 1 101
byte 1 114
byte 1 32
byte 1 104
byte 1 97
byte 1 110
byte 1 100
byte 1 108
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 32
byte 1 101
byte 1 120
byte 1 105
byte 1 116
byte 1 10
byte 1 0
align 1
LABELV $955
byte 1 37
byte 1 102
byte 1 0
align 1
LABELV $901
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $563
byte 1 111
byte 1 114
byte 1 98
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $562
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 108
byte 1 111
byte 1 111
byte 1 112
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $561
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 0
align 1
LABELV $560
byte 1 101
byte 1 120
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $559
byte 1 115
byte 1 101
byte 1 116
byte 1 99
byte 1 118
byte 1 97
byte 1 114
byte 1 0
align 1
LABELV $558
byte 1 116
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $557
byte 1 115
byte 1 101
byte 1 116
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 104
byte 1 101
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $556
byte 1 115
byte 1 101
byte 1 116
byte 1 112
byte 1 108
byte 1 97
byte 1 121
byte 1 101
byte 1 114
byte 1 109
byte 1 111
byte 1 100
byte 1 101
byte 1 108
byte 1 0
align 1
LABELV $555
byte 1 115
byte 1 101
byte 1 116
byte 1 102
byte 1 111
byte 1 99
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $554
byte 1 115
byte 1 101
byte 1 116
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $553
byte 1 115
byte 1 101
byte 1 116
byte 1 105
byte 1 116
byte 1 101
byte 1 109
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $552
byte 1 115
byte 1 101
byte 1 116
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 103
byte 1 114
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 0
align 1
LABELV $551
byte 1 115
byte 1 101
byte 1 116
byte 1 97
byte 1 115
byte 1 115
byte 1 101
byte 1 116
byte 1 0
align 1
LABELV $550
byte 1 99
byte 1 108
byte 1 111
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $549
byte 1 99
byte 1 111
byte 1 110
byte 1 100
byte 1 105
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 97
byte 1 108
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $548
byte 1 111
byte 1 112
byte 1 101
byte 1 110
byte 1 0
align 1
LABELV $547
byte 1 115
byte 1 101
byte 1 116
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $546
byte 1 104
byte 1 105
byte 1 100
byte 1 101
byte 1 0
align 1
LABELV $545
byte 1 115
byte 1 104
byte 1 111
byte 1 119
byte 1 0
align 1
LABELV $544
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 111
byte 1 117
byte 1 116
byte 1 0
align 1
LABELV $543
byte 1 102
byte 1 97
byte 1 100
byte 1 101
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $536
byte 1 37
byte 1 115
byte 1 32
byte 1 59
byte 1 32
byte 1 0
align 1
LABELV $529
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
LABELV $525
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
LABELV $364
byte 1 98
byte 1 111
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $361
byte 1 102
byte 1 111
byte 1 114
byte 1 101
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $358
byte 1 98
byte 1 97
byte 1 99
byte 1 107
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 114
byte 1 0
align 1
LABELV $200
byte 1 32
byte 1 0
align 1
LABELV $197
byte 1 34
byte 1 37
byte 1 115
byte 1 34
byte 1 0
align 1
LABELV $192
byte 1 125
byte 1 0
align 1
LABELV $183
byte 1 123
byte 1 0
align 1
LABELV $147
byte 1 101
byte 1 120
byte 1 112
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 105
byte 1 110
byte 1 116
byte 1 101
byte 1 103
byte 1 101
byte 1 114
byte 1 32
byte 1 98
byte 1 117
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $114
byte 1 101
byte 1 120
byte 1 112
byte 1 101
byte 1 99
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 102
byte 1 108
byte 1 111
byte 1 97
byte 1 116
byte 1 32
byte 1 98
byte 1 117
byte 1 116
byte 1 32
byte 1 102
byte 1 111
byte 1 117
byte 1 110
byte 1 100
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $91
byte 1 94
byte 1 49
byte 1 69
byte 1 82
byte 1 82
byte 1 79
byte 1 82
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 105
byte 1 110
byte 1 101
byte 1 32
byte 1 37
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $87
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
byte 1 37
byte 1 115
byte 1 44
byte 1 32
byte 1 108
byte 1 105
byte 1 110
byte 1 101
byte 1 32
byte 1 37
byte 1 100
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 10
byte 1 0
align 1
LABELV $76
byte 1 77
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 32
byte 1 80
byte 1 111
byte 1 111
byte 1 108
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 46
byte 1 49
byte 1 102
byte 1 37
byte 1 37
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 44
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 98
byte 1 121
byte 1 116
byte 1 101
byte 1 115
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 100
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $75
byte 1 83
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 80
byte 1 111
byte 1 111
byte 1 108
byte 1 32
byte 1 105
byte 1 115
byte 1 32
byte 1 37
byte 1 46
byte 1 49
byte 1 102
byte 1 37
byte 1 37
byte 1 32
byte 1 102
byte 1 117
byte 1 108
byte 1 108
byte 1 44
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 98
byte 1 121
byte 1 116
byte 1 101
byte 1 115
byte 1 32
byte 1 111
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 100
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $74
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 45
byte 1 10
byte 1 0
align 1
LABELV $73
byte 1 77
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 47
byte 1 83
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 80
byte 1 111
byte 1 111
byte 1 108
byte 1 32
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 10
byte 1 0
align 1
LABELV $54
byte 1 0
align 1
LABELV $45
byte 1 85
byte 1 73
byte 1 95
byte 1 65
byte 1 108
byte 1 108
byte 1 111
byte 1 99
byte 1 58
byte 1 32
byte 1 70
byte 1 97
byte 1 105
byte 1 108
byte 1 117
byte 1 114
byte 1 101
byte 1 46
byte 1 32
byte 1 79
byte 1 117
byte 1 116
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 109
byte 1 101
byte 1 109
byte 1 111
byte 1 114
byte 1 121
byte 1 33
byte 1 10
byte 1 0
