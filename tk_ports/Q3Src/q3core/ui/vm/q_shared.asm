export Com_Clamp
code
proc Com_Clamp 0 0
file "../../game/q_shared.c"
line 26
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
;23:// q_shared.c -- stateless support routines that are included in each code dll
;24:#include "q_shared.h"
;25:
;26:float Com_Clamp( float min, float max, float value ) {
line 27
;27:	if ( value < min ) {
ADDRFP4 8
INDIRF4
ADDRFP4 0
INDIRF4
GEF4 $23
line 28
;28:		return min;
ADDRFP4 0
INDIRF4
RETF4
ADDRGP4 $22
JUMPV
LABELV $23
line 30
;29:	}
;30:	if ( value > max ) {
ADDRFP4 8
INDIRF4
ADDRFP4 4
INDIRF4
LEF4 $25
line 31
;31:		return max;
ADDRFP4 4
INDIRF4
RETF4
ADDRGP4 $22
JUMPV
LABELV $25
line 33
;32:	}
;33:	return value;
ADDRFP4 8
INDIRF4
RETF4
LABELV $22
endproc Com_Clamp 0 0
export COM_SkipPath
proc COM_SkipPath 4 0
line 43
;34:}
;35:
;36:
;37:/*
;38:============
;39:COM_SkipPath
;40:============
;41:*/
;42:char *COM_SkipPath (char *pathname)
;43:{
line 46
;44:	char	*last;
;45:	
;46:	last = pathname;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $29
JUMPV
LABELV $28
line 48
;47:	while (*pathname)
;48:	{
line 49
;49:		if (*pathname=='/')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $31
line 50
;50:			last = pathname+1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $31
line 51
;51:		pathname++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 52
;52:	}
LABELV $29
line 47
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $28
line 53
;53:	return last;
ADDRLP4 0
INDIRP4
RETP4
LABELV $27
endproc COM_SkipPath 4 0
export COM_StripExtension
proc COM_StripExtension 12 0
line 61
;54:}
;55:
;56:/*
;57:============
;58:COM_StripExtension
;59:============
;60:*/
;61:void COM_StripExtension( const char *in, char *out ) {
ADDRGP4 $35
JUMPV
LABELV $34
line 62
;62:	while ( *in && *in != '.' ) {
line 63
;63:		*out++ = *in++;
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRFP4 4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI1
ASGNI1
line 64
;64:	}
LABELV $35
line 62
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $37
ADDRLP4 0
INDIRI4
CNSTI4 46
NEI4 $34
LABELV $37
line 65
;65:	*out = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 66
;66:}
LABELV $33
endproc COM_StripExtension 12 0
export COM_DefaultExtension
proc COM_DefaultExtension 76 20
line 74
;67:
;68:
;69:/*
;70:==================
;71:COM_DefaultExtension
;72:==================
;73:*/
;74:void COM_DefaultExtension (char *path, int maxSize, const char *extension ) {
line 82
;75:	char	oldPath[MAX_QPATH];
;76:	char    *src;
;77:
;78://
;79:// if path doesn't have a .EXT, append extension
;80:// (extension should include the .)
;81://
;82:	src = path + strlen(path) - 1;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 68
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI4 -1
ADDP4
ASGNP4
ADDRGP4 $40
JUMPV
LABELV $39
line 84
;83:
;84:	while (*src != '/' && src != path) {
line 85
;85:		if ( *src == '.' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 46
NEI4 $42
line 86
;86:			return;                 // it has an extension
ADDRGP4 $38
JUMPV
LABELV $42
line 88
;87:		}
;88:		src--;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 -1
ADDP4
ASGNP4
line 89
;89:	}
LABELV $40
line 84
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 47
EQI4 $44
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $39
LABELV $44
line 91
;90:
;91:	Q_strncpyz( oldPath, path, sizeof( oldPath ) );
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
line 92
;92:	Com_sprintf( path, maxSize, "%s%s", oldPath, extension );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $45
ARGP4
ADDRLP4 4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 93
;93:}
LABELV $38
endproc COM_DefaultExtension 76 20
export ShortSwap
proc ShortSwap 2 0
ADDRFP4 0
ADDRFP4 0
INDIRI4
CVII2 4
ASGNI2
line 125
;94:
;95:/*
;96:============================================================================
;97:
;98:					BYTE ORDER FUNCTIONS
;99:
;100:============================================================================
;101:*/
;102:/*
;103:// can't just use function pointers, or dll linkage can
;104:// mess up when qcommon is included in multiple places
;105:static short	(*_BigShort) (short l);
;106:static short	(*_LittleShort) (short l);
;107:static int		(*_BigLong) (int l);
;108:static int		(*_LittleLong) (int l);
;109:static qint64	(*_BigLong64) (qint64 l);
;110:static qint64	(*_LittleLong64) (qint64 l);
;111:static float	(*_BigFloat) (const float *l);
;112:static float	(*_LittleFloat) (const float *l);
;113:
;114:short	BigShort(short l){return _BigShort(l);}
;115:short	LittleShort(short l) {return _LittleShort(l);}
;116:int		BigLong (int l) {return _BigLong(l);}
;117:int		LittleLong (int l) {return _LittleLong(l);}
;118:qint64 	BigLong64 (qint64 l) {return _BigLong64(l);}
;119:qint64 	LittleLong64 (qint64 l) {return _LittleLong64(l);}
;120:float	BigFloat (const float *l) {return _BigFloat(l);}
;121:float	LittleFloat (const float *l) {return _LittleFloat(l);}
;122:*/
;123:
;124:short   ShortSwap (short l)
;125:{
line 128
;126:	byte    b1,b2;
;127:
;128:	b1 = l&255;
ADDRLP4 0
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 129
;129:	b2 = (l>>8)&255;
ADDRLP4 1
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 8
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 131
;130:
;131:	return (b1<<8) + b2;
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 8
LSHI4
ADDRLP4 1
INDIRU1
CVUI4 1
ADDI4
CVII2 4
CVII4 2
RETI4
LABELV $46
endproc ShortSwap 2 0
export ShortNoSwap
proc ShortNoSwap 0 0
ADDRFP4 0
ADDRFP4 0
INDIRI4
CVII2 4
ASGNI2
line 135
;132:}
;133:
;134:short	ShortNoSwap (short l)
;135:{
line 136
;136:	return l;
ADDRFP4 0
INDIRI2
CVII4 2
RETI4
LABELV $47
endproc ShortNoSwap 0 0
export LongSwap
proc LongSwap 4 0
line 140
;137:}
;138:
;139:int    LongSwap (int l)
;140:{
line 143
;141:	byte    b1,b2,b3,b4;
;142:
;143:	b1 = l&255;
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 144
;144:	b2 = (l>>8)&255;
ADDRLP4 1
ADDRFP4 0
INDIRI4
CNSTI4 8
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 145
;145:	b3 = (l>>16)&255;
ADDRLP4 2
ADDRFP4 0
INDIRI4
CNSTI4 16
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 146
;146:	b4 = (l>>24)&255;
ADDRLP4 3
ADDRFP4 0
INDIRI4
CNSTI4 24
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 148
;147:
;148:	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 24
LSHI4
ADDRLP4 1
INDIRU1
CVUI4 1
CNSTI4 16
LSHI4
ADDI4
ADDRLP4 2
INDIRU1
CVUI4 1
CNSTI4 8
LSHI4
ADDI4
ADDRLP4 3
INDIRU1
CVUI4 1
ADDI4
RETI4
LABELV $48
endproc LongSwap 4 0
export LongNoSwap
proc LongNoSwap 0 0
line 152
;149:}
;150:
;151:int	LongNoSwap (int l)
;152:{
line 153
;153:	return l;
ADDRFP4 0
INDIRI4
RETI4
LABELV $49
endproc LongNoSwap 0 0
export Long64Swap
proc Long64Swap 8 0
line 157
;154:}
;155:
;156:qint64 Long64Swap (qint64 ll)
;157:{
line 160
;158:	qint64	result;
;159:
;160:	result.b0 = ll.b7;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 7
ADDP4
INDIRU1
ASGNU1
line 161
;161:	result.b1 = ll.b6;
ADDRLP4 0+1
ADDRFP4 4
INDIRP4
CNSTI4 6
ADDP4
INDIRU1
ASGNU1
line 162
;162:	result.b2 = ll.b5;
ADDRLP4 0+2
ADDRFP4 4
INDIRP4
CNSTI4 5
ADDP4
INDIRU1
ASGNU1
line 163
;163:	result.b3 = ll.b4;
ADDRLP4 0+3
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRU1
ASGNU1
line 164
;164:	result.b4 = ll.b3;
ADDRLP4 0+4
ADDRFP4 4
INDIRP4
CNSTI4 3
ADDP4
INDIRU1
ASGNU1
line 165
;165:	result.b5 = ll.b2;
ADDRLP4 0+5
ADDRFP4 4
INDIRP4
CNSTI4 2
ADDP4
INDIRU1
ASGNU1
line 166
;166:	result.b6 = ll.b1;
ADDRLP4 0+6
ADDRFP4 4
INDIRP4
CNSTI4 1
ADDP4
INDIRU1
ASGNU1
line 167
;167:	result.b7 = ll.b0;
ADDRLP4 0+7
ADDRFP4 4
INDIRP4
INDIRU1
ASGNU1
line 169
;168:
;169:	return result;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRB
ASGNB 8
LABELV $50
endproc Long64Swap 8 0
export Long64NoSwap
proc Long64NoSwap 0 0
line 173
;170:}
;171:
;172:qint64 Long64NoSwap (qint64 ll)
;173:{
line 174
;174:	return ll;
ADDRFP4 0
INDIRP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 8
LABELV $59
endproc Long64NoSwap 0 0
export FloatSwap
proc FloatSwap 12 4
line 182
;175:}
;176:
;177:typedef union {
;178:    float	f;
;179:    unsigned int i;
;180:} _FloatByteUnion;
;181:
;182:float FloatSwap (const float *f) {
line 186
;183:	const _FloatByteUnion *in;
;184:	_FloatByteUnion out;
;185:
;186:	in = (_FloatByteUnion *)f;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 187
;187:	out.i = LongSwap(in->i);
ADDRLP4 0
INDIRP4
INDIRU4
CVUI4 4
ARGI4
ADDRLP4 8
ADDRGP4 LongSwap
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
CVIU4 4
ASGNU4
line 189
;188:
;189:	return out.f;
ADDRLP4 4
INDIRF4
RETF4
LABELV $62
endproc FloatSwap 12 4
export FloatNoSwap
proc FloatNoSwap 0 0
line 193
;190:}
;191:
;192:float FloatNoSwap (const float *f)
;193:{
line 194
;194:	return *f;
ADDRFP4 0
INDIRP4
INDIRF4
RETF4
LABELV $63
endproc FloatNoSwap 0 0
export COM_BeginParseSession
proc COM_BeginParseSession 0 16
line 247
;195:}
;196:
;197:/*
;198:================
;199:Swap_Init
;200:================
;201:*/
;202:/*
;203:void Swap_Init (void)
;204:{
;205:	byte	swaptest[2] = {1,0};
;206:
;207:// set the byte swapping variables in a portable manner	
;208:	if ( *(short *)swaptest == 1)
;209:	{
;210:		_BigShort = ShortSwap;
;211:		_LittleShort = ShortNoSwap;
;212:		_BigLong = LongSwap;
;213:		_LittleLong = LongNoSwap;
;214:		_BigLong64 = Long64Swap;
;215:		_LittleLong64 = Long64NoSwap;
;216:		_BigFloat = FloatSwap;
;217:		_LittleFloat = FloatNoSwap;
;218:	}
;219:	else
;220:	{
;221:		_BigShort = ShortNoSwap;
;222:		_LittleShort = ShortSwap;
;223:		_BigLong = LongNoSwap;
;224:		_LittleLong = LongSwap;
;225:		_BigLong64 = Long64NoSwap;
;226:		_LittleLong64 = Long64Swap;
;227:		_BigFloat = FloatNoSwap;
;228:		_LittleFloat = FloatSwap;
;229:	}
;230:
;231:}
;232:*/
;233:
;234:/*
;235:============================================================================
;236:
;237:PARSING
;238:
;239:============================================================================
;240:*/
;241:
;242:static	char	com_token[MAX_TOKEN_CHARS];
;243:static	char	com_parsename[MAX_TOKEN_CHARS];
;244:static	int		com_lines;
;245:
;246:void COM_BeginParseSession( const char *name )
;247:{
line 248
;248:	com_lines = 0;
ADDRGP4 com_lines
CNSTI4 0
ASGNI4
line 249
;249:	Com_sprintf(com_parsename, sizeof(com_parsename), "%s", name);
ADDRGP4 com_parsename
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $65
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 250
;250:}
LABELV $64
endproc COM_BeginParseSession 0 16
export COM_GetCurrentParseLine
proc COM_GetCurrentParseLine 0 0
line 253
;251:
;252:int COM_GetCurrentParseLine( void )
;253:{
line 254
;254:	return com_lines;
ADDRGP4 com_lines
INDIRI4
RETI4
LABELV $66
endproc COM_GetCurrentParseLine 0 0
export COM_Parse
proc COM_Parse 4 8
line 258
;255:}
;256:
;257:char *COM_Parse( char **data_p )
;258:{
line 259
;259:	return COM_ParseExt( data_p, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $67
endproc COM_Parse 4 8
bss
align 1
LABELV $69
skip 4096
export COM_ParseError
code
proc COM_ParseError 4 16
line 263
;260:}
;261:
;262:void COM_ParseError( char *format, ... )
;263:{
line 267
;264:	va_list argptr;
;265:	static char string[4096];
;266:
;267:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 268
;268:	vsprintf (string, format, argptr);
ADDRGP4 $69
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 269
;269:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 271
;270:
;271:	Com_Printf("ERROR: %s, line %d: %s\n", com_parsename, com_lines, string);
ADDRGP4 $71
ARGP4
ADDRGP4 com_parsename
ARGP4
ADDRGP4 com_lines
INDIRI4
ARGI4
ADDRGP4 $69
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 272
;272:}
LABELV $68
endproc COM_ParseError 4 16
bss
align 1
LABELV $73
skip 4096
export COM_ParseWarning
code
proc COM_ParseWarning 4 16
line 275
;273:
;274:void COM_ParseWarning( char *format, ... )
;275:{
line 279
;276:	va_list argptr;
;277:	static char string[4096];
;278:
;279:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 280
;280:	vsprintf (string, format, argptr);
ADDRGP4 $73
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 281
;281:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 283
;282:
;283:	Com_Printf("WARNING: %s, line %d: %s\n", com_parsename, com_lines, string);
ADDRGP4 $75
ARGP4
ADDRGP4 com_parsename
ARGP4
ADDRGP4 com_lines
INDIRI4
ARGI4
ADDRGP4 $73
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 284
;284:}
LABELV $72
endproc COM_ParseWarning 4 16
proc SkipWhitespace 8 0
line 298
;285:
;286:/*
;287:==============
;288:COM_Parse
;289:
;290:Parse a token out of a string
;291:Will never return NULL, just empty strings
;292:
;293:If "allowLineBreaks" is qtrue then an empty
;294:string will be returned if the next token is
;295:a newline.
;296:==============
;297:*/
;298:static char *SkipWhitespace( char *data, qboolean *hasNewLines ) {
ADDRGP4 $78
JUMPV
LABELV $77
line 301
;299:	int c;
;300:
;301:	while( (c = *data) <= ' ') {
line 302
;302:		if( !c ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $80
line 303
;303:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $76
JUMPV
LABELV $80
line 305
;304:		}
;305:		if( c == '\n' ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
NEI4 $82
line 306
;306:			com_lines++;
ADDRLP4 4
ADDRGP4 com_lines
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 307
;307:			*hasNewLines = qtrue;
ADDRFP4 4
INDIRP4
CNSTI4 1
ASGNI4
line 308
;308:		}
LABELV $82
line 309
;309:		data++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 310
;310:	}
LABELV $78
line 301
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 32
LEI4 $77
line 312
;311:
;312:	return data;
ADDRFP4 0
INDIRP4
RETP4
LABELV $76
endproc SkipWhitespace 8 0
export COM_Compress
proc COM_Compress 48 0
line 315
;313:}
;314:
;315:int COM_Compress( char *data_p ) {
line 318
;316:	char *in, *out;
;317:	int c;
;318:	qboolean newline = qfalse, whitespace = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 16
CNSTI4 0
ASGNI4
line 320
;319:
;320:	in = out = data_p;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 20
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 321
;321:	if (in) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $85
ADDRGP4 $88
JUMPV
LABELV $87
line 322
;322:		while ((c = *in) != 0) {
line 324
;323:			// skip double slash comments
;324:			if ( c == '/' && in[1] == '/' ) {
ADDRLP4 24
CNSTI4 47
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $90
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ADDRLP4 24
INDIRI4
NEI4 $90
ADDRGP4 $93
JUMPV
LABELV $92
line 325
;325:				while (*in && *in != '\n') {
line 326
;326:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 327
;327:				}
LABELV $93
line 325
ADDRLP4 28
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $95
ADDRLP4 28
INDIRI4
CNSTI4 10
NEI4 $92
LABELV $95
line 329
;328:			// skip /* */ comments
;329:			} else if ( c == '/' && in[1] == '*' ) {
ADDRGP4 $91
JUMPV
LABELV $90
ADDRLP4 4
INDIRI4
CNSTI4 47
NEI4 $96
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $96
ADDRGP4 $99
JUMPV
LABELV $98
line 331
;330:				while ( *in && ( *in != '*' || in[1] != '/' ) ) 
;331:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $99
line 330
ADDRLP4 32
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $101
ADDRLP4 32
INDIRI4
CNSTI4 42
NEI4 $98
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $98
LABELV $101
line 332
;332:				if ( *in ) 
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $97
line 333
;333:					in += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 335
;334:                        // record when we hit a newline
;335:                        } else if ( c == '\n' || c == '\r' ) {
ADDRGP4 $97
JUMPV
LABELV $96
ADDRLP4 4
INDIRI4
CNSTI4 10
EQI4 $106
ADDRLP4 4
INDIRI4
CNSTI4 13
NEI4 $104
LABELV $106
line 336
;336:                            newline = qtrue;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 337
;337:                            in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 339
;338:                        // record when we hit whitespace
;339:                        } else if ( c == ' ' || c == '\t') {
ADDRGP4 $105
JUMPV
LABELV $104
ADDRLP4 4
INDIRI4
CNSTI4 32
EQI4 $109
ADDRLP4 4
INDIRI4
CNSTI4 9
NEI4 $107
LABELV $109
line 340
;340:                            whitespace = qtrue;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 341
;341:                            in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 343
;342:                        // an actual token
;343:			} else {
ADDRGP4 $108
JUMPV
LABELV $107
line 345
;344:                            // if we have a pending newline, emit it (and it counts as whitespace)
;345:                            if (newline) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $110
line 346
;346:                                *out++ = '\n';
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI1 10
ASGNI1
line 347
;347:                                newline = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 348
;348:                                whitespace = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 349
;349:                            } if (whitespace) {
LABELV $110
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $112
line 350
;350:                                *out++ = ' ';
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI1 32
ASGNI1
line 351
;351:                                whitespace = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 352
;352:                            }
LABELV $112
line 355
;353:                            
;354:                            // copy quoted strings unmolested
;355:                            if (c == '"') {
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $114
line 356
;356:                                    *out++ = c;
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 357
;357:                                    in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $117
JUMPV
LABELV $116
line 358
;358:                                    while (1) {
line 359
;359:                                        c = *in;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 360
;360:                                        if (c && c != '"') {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $118
ADDRLP4 4
INDIRI4
CNSTI4 34
EQI4 $118
line 361
;361:                                            *out++ = c;
ADDRLP4 44
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 44
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 362
;362:                                            in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 363
;363:                                        } else {
line 364
;364:                                            break;
LABELV $120
line 366
;365:                                        }
;366:                                    }
LABELV $117
line 358
ADDRGP4 $116
JUMPV
LABELV $118
line 367
;367:                                    if (c == '"') {
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $115
line 368
;368:                                        *out++ = c;
ADDRLP4 40
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 40
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 369
;369:                                        in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 370
;370:                                    }
line 371
;371:                            } else {
ADDRGP4 $115
JUMPV
LABELV $114
line 372
;372:                                *out = c;
ADDRLP4 8
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 373
;373:                                out++;
ADDRLP4 8
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 374
;374:                                in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 375
;375:                            }
LABELV $115
line 376
;376:			}
LABELV $108
LABELV $105
LABELV $97
LABELV $91
line 377
;377:		}
LABELV $88
line 322
ADDRLP4 24
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 4
ADDRLP4 24
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $87
line 378
;378:	}
LABELV $85
line 379
;379:	*out = 0;
ADDRLP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 380
;380:	return out - data_p;
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
RETI4
LABELV $84
endproc COM_Compress 48 0
export COM_ParseExt
proc COM_ParseExt 36 8
line 384
;381:}
;382:
;383:char *COM_ParseExt( char **data_p, qboolean allowLineBreaks )
;384:{
line 385
;385:	int c = 0, len;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 386
;386:	qboolean hasNewLines = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 389
;387:	char *data;
;388:
;389:	data = *data_p;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
line 390
;390:	len = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 391
;391:	com_token[0] = 0;
ADDRGP4 com_token
CNSTI1 0
ASGNI1
line 394
;392:
;393:	// make sure incoming data is valid
;394:	if ( !data )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $127
line 395
;395:	{
line 396
;396:		*data_p = NULL;
ADDRFP4 0
INDIRP4
CNSTP4 0
ASGNP4
line 397
;397:		return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $123
JUMPV
LABELV $126
line 401
;398:	}
;399:
;400:	while ( 1 )
;401:	{
line 403
;402:		// skip whitespace
;403:		data = SkipWhitespace( data, &hasNewLines );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 16
ADDRGP4 SkipWhitespace
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 404
;404:		if ( !data )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $129
line 405
;405:		{
line 406
;406:			*data_p = NULL;
ADDRFP4 0
INDIRP4
CNSTP4 0
ASGNP4
line 407
;407:			return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $123
JUMPV
LABELV $129
line 409
;408:		}
;409:		if ( hasNewLines && !allowLineBreaks )
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $131
ADDRFP4 4
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $131
line 410
;410:		{
line 411
;411:			*data_p = data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 412
;412:			return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $123
JUMPV
LABELV $131
line 415
;413:		}
;414:
;415:		c = *data;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 418
;416:
;417:		// skip double slash comments
;418:		if ( c == '/' && data[1] == '/' )
ADDRLP4 24
CNSTI4 47
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $133
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ADDRLP4 24
INDIRI4
NEI4 $133
line 419
;419:		{
line 420
;420:			data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
ADDRGP4 $136
JUMPV
LABELV $135
line 421
;421:			while (*data && *data != '\n') {
line 422
;422:				data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 423
;423:			}
LABELV $136
line 421
ADDRLP4 28
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $138
ADDRLP4 28
INDIRI4
CNSTI4 10
NEI4 $135
LABELV $138
line 424
;424:		}
ADDRGP4 $134
JUMPV
LABELV $133
line 426
;425:		// skip /* */ comments
;426:		else if ( c=='/' && data[1] == '*' ) 
ADDRLP4 4
INDIRI4
CNSTI4 47
NEI4 $128
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $128
line 427
;427:		{
line 428
;428:			data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
ADDRGP4 $142
JUMPV
LABELV $141
line 430
;429:			while ( *data && ( *data != '*' || data[1] != '/' ) ) 
;430:			{
line 431
;431:				data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 432
;432:			}
LABELV $142
line 429
ADDRLP4 32
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $144
ADDRLP4 32
INDIRI4
CNSTI4 42
NEI4 $141
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $141
LABELV $144
line 433
;433:			if ( *data ) 
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $140
line 434
;434:			{
line 435
;435:				data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 436
;436:			}
line 437
;437:		}
line 439
;438:		else
;439:		{
line 440
;440:			break;
LABELV $140
LABELV $134
line 442
;441:		}
;442:	}
LABELV $127
line 400
ADDRGP4 $126
JUMPV
LABELV $128
line 445
;443:
;444:	// handle quoted strings
;445:	if (c == '\"')
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $147
line 446
;446:	{
line 447
;447:		data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $150
JUMPV
LABELV $149
line 449
;448:		while (1)
;449:		{
line 450
;450:			c = *data++;
ADDRLP4 16
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 451
;451:			if (c=='\"' || !c)
ADDRLP4 4
INDIRI4
CNSTI4 34
EQI4 $154
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $152
LABELV $154
line 452
;452:			{
line 453
;453:				com_token[len] = 0;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
CNSTI1 0
ASGNI1
line 454
;454:				*data_p = ( char * ) data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 455
;455:				return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $123
JUMPV
LABELV $152
line 457
;456:			}
;457:			if (len < MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
GEI4 $155
line 458
;458:			{
line 459
;459:				com_token[len] = c;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 460
;460:				len++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 461
;461:			}
LABELV $155
line 462
;462:		}
LABELV $150
line 448
ADDRGP4 $149
JUMPV
line 463
;463:	}
LABELV $147
LABELV $157
line 467
;464:
;465:	// parse a regular word
;466:	do
;467:	{
line 468
;468:		if (len < MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
GEI4 $160
line 469
;469:		{
line 470
;470:			com_token[len] = c;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 471
;471:			len++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 472
;472:		}
LABELV $160
line 473
;473:		data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 474
;474:		c = *data;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 475
;475:		if ( c == '\n' )
ADDRLP4 4
INDIRI4
CNSTI4 10
NEI4 $162
line 476
;476:			com_lines++;
ADDRLP4 16
ADDRGP4 com_lines
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $162
line 477
;477:	} while (c>32);
LABELV $158
ADDRLP4 4
INDIRI4
CNSTI4 32
GTI4 $157
line 479
;478:
;479:	if (len == MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
NEI4 $164
line 480
;480:	{
line 482
;481://		Com_Printf ("Token exceeded %i chars, discarded.\n", MAX_TOKEN_CHARS);
;482:		len = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 483
;483:	}
LABELV $164
line 484
;484:	com_token[len] = 0;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
CNSTI1 0
ASGNI1
line 486
;485:
;486:	*data_p = ( char * ) data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 487
;487:	return com_token;
ADDRGP4 com_token
RETP4
LABELV $123
endproc COM_ParseExt 36 8
export COM_MatchToken
proc COM_MatchToken 12 16
line 551
;488:}
;489:
;490:
;491:#if 0
;492:// no longer used
;493:/*
;494:===============
;495:COM_ParseInfos
;496:===============
;497:*/
;498:int COM_ParseInfos( char *buf, int max, char infos[][MAX_INFO_STRING] ) {
;499:	char	*token;
;500:	int		count;
;501:	char	key[MAX_TOKEN_CHARS];
;502:
;503:	count = 0;
;504:
;505:	while ( 1 ) {
;506:		token = COM_Parse( &buf );
;507:		if ( !token[0] ) {
;508:			break;
;509:		}
;510:		if ( strcmp( token, "{" ) ) {
;511:			Com_Printf( "Missing { in info file\n" );
;512:			break;
;513:		}
;514:
;515:		if ( count == max ) {
;516:			Com_Printf( "Max infos exceeded\n" );
;517:			break;
;518:		}
;519:
;520:		infos[count][0] = 0;
;521:		while ( 1 ) {
;522:			token = COM_ParseExt( &buf, qtrue );
;523:			if ( !token[0] ) {
;524:				Com_Printf( "Unexpected end of info file\n" );
;525:				break;
;526:			}
;527:			if ( !strcmp( token, "}" ) ) {
;528:				break;
;529:			}
;530:			Q_strncpyz( key, token, sizeof( key ) );
;531:
;532:			token = COM_ParseExt( &buf, qfalse );
;533:			if ( !token[0] ) {
;534:				strcpy( token, "<NULL>" );
;535:			}
;536:			Info_SetValueForKey( infos[count], key, token );
;537:		}
;538:		count++;
;539:	}
;540:
;541:	return count;
;542:}
;543:#endif
;544:
;545:
;546:/*
;547:==================
;548:COM_MatchToken
;549:==================
;550:*/
;551:void COM_MatchToken( char **buf_p, char *match ) {
line 554
;552:	char	*token;
;553:
;554:	token = COM_Parse( buf_p );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 555
;555:	if ( strcmp( token, match ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $167
line 556
;556:		Com_Error( ERR_DROP, "MatchToken: %s != %s", token, match );
CNSTI4 1
ARGI4
ADDRGP4 $169
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 557
;557:	}
LABELV $167
line 558
;558:}
LABELV $166
endproc COM_MatchToken 12 16
export SkipBracedSection
proc SkipBracedSection 12 8
line 570
;559:
;560:
;561:/*
;562:=================
;563:SkipBracedSection
;564:
;565:The next token should be an open brace.
;566:Skips until a matching close brace is found.
;567:Internal brace depths are properly skipped.
;568:=================
;569:*/
;570:void SkipBracedSection (char **program) {
line 574
;571:	char			*token;
;572:	int				depth;
;573:
;574:	depth = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $171
line 575
;575:	do {
line 576
;576:		token = COM_ParseExt( program, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 577
;577:		if( token[1] == 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $174
line 578
;578:			if( token[0] == '{' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $176
line 579
;579:				depth++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 580
;580:			}
ADDRGP4 $177
JUMPV
LABELV $176
line 581
;581:			else if( token[0] == '}' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $178
line 582
;582:				depth--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 583
;583:			}
LABELV $178
LABELV $177
line 584
;584:		}
LABELV $174
line 585
;585:	} while( depth && *program );
LABELV $172
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $180
ADDRFP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $171
LABELV $180
line 586
;586:}
LABELV $170
endproc SkipBracedSection 12 8
export SkipRestOfLine
proc SkipRestOfLine 16 0
line 593
;587:
;588:/*
;589:=================
;590:SkipRestOfLine
;591:=================
;592:*/
;593:void SkipRestOfLine ( char **data ) {
line 597
;594:	char	*p;
;595:	int		c;
;596:
;597:	p = *data;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
ADDRGP4 $183
JUMPV
LABELV $182
line 598
;598:	while ( (c = *p++) != 0 ) {
line 599
;599:		if ( c == '\n' ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
NEI4 $185
line 600
;600:			com_lines++;
ADDRLP4 8
ADDRGP4 com_lines
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 601
;601:			break;
ADDRGP4 $184
JUMPV
LABELV $185
line 603
;602:		}
;603:	}
LABELV $183
line 598
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $182
LABELV $184
line 605
;604:
;605:	*data = p;
ADDRFP4 0
INDIRP4
ADDRLP4 4
INDIRP4
ASGNP4
line 606
;606:}
LABELV $181
endproc SkipRestOfLine 16 0
export Parse1DMatrix
proc Parse1DMatrix 16 8
line 609
;607:
;608:
;609:void Parse1DMatrix (char **buf_p, int x, float *m) {
line 613
;610:	char	*token;
;611:	int		i;
;612:
;613:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $188
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 615
;614:
;615:	for (i = 0 ; i < x ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $192
JUMPV
LABELV $189
line 616
;616:		token = COM_Parse(buf_p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 617
;617:		m[i] = atof(token);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 618
;618:	}
LABELV $190
line 615
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $192
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $189
line 620
;619:
;620:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $193
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 621
;621:}
LABELV $187
endproc Parse1DMatrix 16 8
export Parse2DMatrix
proc Parse2DMatrix 8 12
line 623
;622:
;623:void Parse2DMatrix (char **buf_p, int y, int x, float *m) {
line 626
;624:	int		i;
;625:
;626:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $188
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 628
;627:
;628:	for (i = 0 ; i < y ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $198
JUMPV
LABELV $195
line 629
;629:		Parse1DMatrix (buf_p, x, m + i * x);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 2
LSHI4
ADDRFP4 12
INDIRP4
ADDP4
ARGP4
ADDRGP4 Parse1DMatrix
CALLV
pop
line 630
;630:	}
LABELV $196
line 628
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $198
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $195
line 632
;631:
;632:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $193
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 633
;633:}
LABELV $194
endproc Parse2DMatrix 8 12
export Parse3DMatrix
proc Parse3DMatrix 12 16
line 635
;634:
;635:void Parse3DMatrix (char **buf_p, int z, int y, int x, float *m) {
line 638
;636:	int		i;
;637:
;638:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $188
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 640
;639:
;640:	for (i = 0 ; i < z ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $203
JUMPV
LABELV $200
line 641
;641:		Parse2DMatrix (buf_p, y, x, m + i * x*y);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
ADDRFP4 12
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 2
LSHI4
ADDRFP4 16
INDIRP4
ADDP4
ARGP4
ADDRGP4 Parse2DMatrix
CALLV
pop
line 642
;642:	}
LABELV $201
line 640
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $203
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $200
line 644
;643:
;644:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $193
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 645
;645:}
LABELV $199
endproc Parse3DMatrix 12 16
export Q_isprint
proc Q_isprint 4 0
line 657
;646:
;647:
;648:/*
;649:============================================================================
;650:
;651:					LIBRARY REPLACEMENT FUNCTIONS
;652:
;653:============================================================================
;654:*/
;655:
;656:int Q_isprint( int c )
;657:{
line 658
;658:	if ( c >= 0x20 && c <= 0x7E )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $205
ADDRLP4 0
INDIRI4
CNSTI4 126
GTI4 $205
line 659
;659:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $204
JUMPV
LABELV $205
line 660
;660:	return ( 0 );
CNSTI4 0
RETI4
LABELV $204
endproc Q_isprint 4 0
export Q_islower
proc Q_islower 4 0
line 664
;661:}
;662:
;663:int Q_islower( int c )
;664:{
line 665
;665:	if (c >= 'a' && c <= 'z')
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $208
ADDRLP4 0
INDIRI4
CNSTI4 122
GTI4 $208
line 666
;666:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $207
JUMPV
LABELV $208
line 667
;667:	return ( 0 );
CNSTI4 0
RETI4
LABELV $207
endproc Q_islower 4 0
export Q_isupper
proc Q_isupper 4 0
line 671
;668:}
;669:
;670:int Q_isupper( int c )
;671:{
line 672
;672:	if (c >= 'A' && c <= 'Z')
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 65
LTI4 $211
ADDRLP4 0
INDIRI4
CNSTI4 90
GTI4 $211
line 673
;673:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $210
JUMPV
LABELV $211
line 674
;674:	return ( 0 );
CNSTI4 0
RETI4
LABELV $210
endproc Q_isupper 4 0
export Q_isalpha
proc Q_isalpha 8 0
line 678
;675:}
;676:
;677:int Q_isalpha( int c )
;678:{
line 679
;679:	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $217
ADDRLP4 0
INDIRI4
CNSTI4 122
LEI4 $216
LABELV $217
ADDRLP4 4
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 65
LTI4 $214
ADDRLP4 4
INDIRI4
CNSTI4 90
GTI4 $214
LABELV $216
line 680
;680:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $213
JUMPV
LABELV $214
line 681
;681:	return ( 0 );
CNSTI4 0
RETI4
LABELV $213
endproc Q_isalpha 8 0
export Q_strrchr
proc Q_strrchr 12 0
line 685
;682:}
;683:
;684:char* Q_strrchr( const char* string, int c )
;685:{
line 686
;686:	char cc = c;
ADDRLP4 4
ADDRFP4 4
INDIRI4
CVII1 4
ASGNI1
line 688
;687:	char *s;
;688:	char *sp=(char *)0;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 690
;689:
;690:	s = (char*)string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $220
JUMPV
LABELV $219
line 693
;691:
;692:	while (*s)
;693:	{
line 694
;694:		if (*s == cc)
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI1
CVII4 1
NEI4 $222
line 695
;695:			sp = s;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $222
line 696
;696:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 697
;697:	}
LABELV $220
line 692
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $219
line 698
;698:	if (cc == 0)
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $224
line 699
;699:		sp = s;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $224
line 701
;700:
;701:	return sp;
ADDRLP4 8
INDIRP4
RETP4
LABELV $218
endproc Q_strrchr 12 0
export Q_strncpyz
proc Q_strncpyz 0 12
line 711
;702:}
;703:
;704:/*
;705:=============
;706:Q_strncpyz
;707: 
;708:Safe strncpy that ensures a trailing zero
;709:=============
;710:*/
;711:void Q_strncpyz( char *dest, const char *src, int destsize ) {
line 713
;712:  // bk001129 - also NULL dest
;713:  if ( !dest ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $227
line 714
;714:    Com_Error( ERR_FATAL, "Q_strncpyz: NULL dest" );
CNSTI4 0
ARGI4
ADDRGP4 $229
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 715
;715:  }
LABELV $227
line 716
;716:	if ( !src ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $230
line 717
;717:		Com_Error( ERR_FATAL, "Q_strncpyz: NULL src" );
CNSTI4 0
ARGI4
ADDRGP4 $232
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 718
;718:	}
LABELV $230
line 719
;719:	if ( destsize < 1 ) {
ADDRFP4 8
INDIRI4
CNSTI4 1
GEI4 $233
line 720
;720:		Com_Error(ERR_FATAL,"Q_strncpyz: destsize < 1" ); 
CNSTI4 0
ARGI4
ADDRGP4 $235
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 721
;721:	}
LABELV $233
line 723
;722:
;723:	strncpy( dest, src, destsize-1 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 724
;724:  dest[destsize-1] = 0;
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 725
;725:}
LABELV $226
endproc Q_strncpyz 0 12
export Q_stricmpn
proc Q_stricmpn 32 0
line 727
;726:                 
;727:int Q_stricmpn (const char *s1, const char *s2, int n) {
line 731
;728:	int		c1, c2;
;729:
;730:	// bk001129 - moved in 1.17 fix not in id codebase
;731:        if ( s1 == NULL ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $237
line 732
;732:           if ( s2 == NULL )
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $239
line 733
;733:             return 0;
CNSTI4 0
RETI4
ADDRGP4 $236
JUMPV
LABELV $239
line 735
;734:           else
;735:             return -1;
CNSTI4 -1
RETI4
ADDRGP4 $236
JUMPV
LABELV $237
line 737
;736:        }
;737:        else if ( s2==NULL )
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $241
line 738
;738:          return 1;
CNSTI4 1
RETI4
ADDRGP4 $236
JUMPV
LABELV $241
LABELV $243
line 742
;739:
;740:
;741:	
;742:	do {
line 743
;743:		c1 = *s1++;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 744
;744:		c2 = *s2++;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 12
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 746
;745:
;746:		if (!n--) {
ADDRLP4 16
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 8
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $246
line 747
;747:			return 0;		// strings are equal until end point
CNSTI4 0
RETI4
ADDRGP4 $236
JUMPV
LABELV $246
line 750
;748:		}
;749:		
;750:		if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $248
line 751
;751:			if (c1 >= 'a' && c1 <= 'z') {
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $250
ADDRLP4 0
INDIRI4
CNSTI4 122
GTI4 $250
line 752
;752:				c1 -= ('a' - 'A');
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
line 753
;753:			}
LABELV $250
line 754
;754:			if (c2 >= 'a' && c2 <= 'z') {
ADDRLP4 4
INDIRI4
CNSTI4 97
LTI4 $252
ADDRLP4 4
INDIRI4
CNSTI4 122
GTI4 $252
line 755
;755:				c2 -= ('a' - 'A');
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
line 756
;756:			}
LABELV $252
line 757
;757:			if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $254
line 758
;758:				return c1 < c2 ? -1 : 1;
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
GEI4 $257
ADDRLP4 28
CNSTI4 -1
ASGNI4
ADDRGP4 $258
JUMPV
LABELV $257
ADDRLP4 28
CNSTI4 1
ASGNI4
LABELV $258
ADDRLP4 28
INDIRI4
RETI4
ADDRGP4 $236
JUMPV
LABELV $254
line 760
;759:			}
;760:		}
LABELV $248
line 761
;761:	} while (c1);
LABELV $244
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $243
line 763
;762:	
;763:	return 0;		// strings are equal
CNSTI4 0
RETI4
LABELV $236
endproc Q_stricmpn 32 0
export Q_strncmp
proc Q_strncmp 24 0
line 766
;764:}
;765:
;766:int Q_strncmp (const char *s1, const char *s2, int n) {
LABELV $260
line 769
;767:	int		c1, c2;
;768:	
;769:	do {
line 770
;770:		c1 = *s1++;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 771
;771:		c2 = *s2++;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 12
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 773
;772:
;773:		if (!n--) {
ADDRLP4 16
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 8
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $263
line 774
;774:			return 0;		// strings are equal until end point
CNSTI4 0
RETI4
ADDRGP4 $259
JUMPV
LABELV $263
line 777
;775:		}
;776:		
;777:		if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $265
line 778
;778:			return c1 < c2 ? -1 : 1;
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
GEI4 $268
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRGP4 $269
JUMPV
LABELV $268
ADDRLP4 20
CNSTI4 1
ASGNI4
LABELV $269
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $259
JUMPV
LABELV $265
line 780
;779:		}
;780:	} while (c1);
LABELV $261
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $260
line 782
;781:	
;782:	return 0;		// strings are equal
CNSTI4 0
RETI4
LABELV $259
endproc Q_strncmp 24 0
export Q_stricmp
proc Q_stricmp 12 12
line 785
;783:}
;784:
;785:int Q_stricmp (const char *s1, const char *s2) {
line 786
;786:	return (s1 && s2) ? Q_stricmpn (s1, s2, 99999) : -1;
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $272
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $272
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 99999
ARGI4
ADDRLP4 8
ADDRGP4 Q_stricmpn
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
ADDRGP4 $273
JUMPV
LABELV $272
ADDRLP4 0
CNSTI4 -1
ASGNI4
LABELV $273
ADDRLP4 0
INDIRI4
RETI4
LABELV $270
endproc Q_stricmp 12 12
export Q_strlwr
proc Q_strlwr 12 4
line 790
;787:}
;788:
;789:
;790:char *Q_strlwr( char *s1 ) {
line 793
;791:    char	*s;
;792:
;793:    s = s1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $276
JUMPV
LABELV $275
line 794
;794:	while ( *s ) {
line 795
;795:		*s = tolower(*s);
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 tolower
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
CVII1 4
ASGNI1
line 796
;796:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 797
;797:	}
LABELV $276
line 794
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $275
line 798
;798:    return s1;
ADDRFP4 0
INDIRP4
RETP4
LABELV $274
endproc Q_strlwr 12 4
export Q_strupr
proc Q_strupr 12 4
line 801
;799:}
;800:
;801:char *Q_strupr( char *s1 ) {
line 804
;802:    char	*s;
;803:
;804:    s = s1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $280
JUMPV
LABELV $279
line 805
;805:	while ( *s ) {
line 806
;806:		*s = toupper(*s);
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 toupper
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
CVII1 4
ASGNI1
line 807
;807:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 808
;808:	}
LABELV $280
line 805
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $279
line 809
;809:    return s1;
ADDRFP4 0
INDIRP4
RETP4
LABELV $278
endproc Q_strupr 12 4
export Q_strcat
proc Q_strcat 12 12
line 814
;810:}
;811:
;812:
;813:// never goes past bounds or leaves without a terminating 0
;814:void Q_strcat( char *dest, int size, const char *src ) {
line 817
;815:	int		l1;
;816:
;817:	l1 = strlen( dest );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 818
;818:	if ( l1 >= size ) {
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $283
line 819
;819:		Com_Error( ERR_FATAL, "Q_strcat: already overflowed" );
CNSTI4 0
ARGI4
ADDRGP4 $285
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 820
;820:	}
LABELV $283
line 821
;821:	Q_strncpyz( dest + l1, src, size - l1 );
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 822
;822:}
LABELV $282
endproc Q_strcat 12 12
export Q_PrintStrlen
proc Q_PrintStrlen 20 0
line 825
;823:
;824:
;825:int Q_PrintStrlen( const char *string ) {
line 829
;826:	int			len;
;827:	const char	*p;
;828:
;829:	if( !string ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $287
line 830
;830:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $286
JUMPV
LABELV $287
line 833
;831:	}
;832:
;833:	len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 834
;834:	p = string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $290
JUMPV
LABELV $289
line 835
;835:	while( *p ) {
line 836
;836:		if( Q_IsColorString( p ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $292
ADDRLP4 12
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
NEI4 $292
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $292
ADDRLP4 16
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $292
line 837
;837:			p += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 838
;838:			continue;
ADDRGP4 $290
JUMPV
LABELV $292
line 840
;839:		}
;840:		p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 841
;841:		len++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 842
;842:	}
LABELV $290
line 835
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $289
line 844
;843:
;844:	return len;
ADDRLP4 4
INDIRI4
RETI4
LABELV $286
endproc Q_PrintStrlen 20 0
export Q_CleanStr
proc Q_CleanStr 32 0
line 848
;845:}
;846:
;847:
;848:char *Q_CleanStr( char *string ) {
line 853
;849:	char*	d;
;850:	char*	s;
;851:	int		c;
;852:
;853:	s = string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 854
;854:	d = string;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $296
JUMPV
LABELV $295
line 855
;855:	while ((c = *s) != 0 ) {
line 856
;856:		if ( Q_IsColorString( s ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $298
ADDRLP4 16
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 16
INDIRI4
NEI4 $298
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $298
ADDRLP4 20
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $298
line 857
;857:			s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 858
;858:		}		
ADDRGP4 $299
JUMPV
LABELV $298
line 859
;859:		else if ( c >= 0x20 && c <= 0x7E ) {
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $300
ADDRLP4 4
INDIRI4
CNSTI4 126
GTI4 $300
line 860
;860:			*d++ = c;
ADDRLP4 28
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 861
;861:		}
LABELV $300
LABELV $299
line 862
;862:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 863
;863:	}
LABELV $296
line 855
ADDRLP4 12
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $295
line 864
;864:	*d = '\0';
ADDRLP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 866
;865:
;866:	return string;
ADDRFP4 0
INDIRP4
RETP4
LABELV $294
endproc Q_CleanStr 32 0
export Com_sprintf
proc Com_sprintf 32012 12
line 870
;867:}
;868:
;869:
;870:void QDECL Com_sprintf( char *dest, int size, const char *fmt, ...) {
line 875
;871:	int		len;
;872:	va_list		argptr;
;873:	char	bigbuffer[32000];	// big, but small enough to fit in PPC stack
;874:
;875:	va_start (argptr,fmt);
ADDRLP4 4
ADDRFP4 8+4
ASGNP4
line 876
;876:	len = vsprintf (bigbuffer,fmt,argptr);
ADDRLP4 8
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 32008
ADDRGP4 vsprintf
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 32008
INDIRI4
ASGNI4
line 877
;877:	va_end (argptr);
ADDRLP4 4
CNSTP4 0
ASGNP4
line 878
;878:	if ( len >= sizeof( bigbuffer ) ) {
ADDRLP4 0
INDIRI4
CVIU4 4
CNSTU4 32000
LTU4 $304
line 879
;879:		Com_Error( ERR_FATAL, "Com_sprintf: overflowed bigbuffer" );
CNSTI4 0
ARGI4
ADDRGP4 $306
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 880
;880:	}
LABELV $304
line 881
;881:	if (len >= size) {
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $307
line 882
;882:		Com_Printf ("Com_sprintf: overflow of %i in %i\n", len, size);
ADDRGP4 $309
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 888
;883:#ifdef	_DEBUG
;884:		__asm {
;885:			int 3;
;886:		}
;887:#endif
;888:	}
LABELV $307
line 889
;889:	Q_strncpyz (dest, bigbuffer, size );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 890
;890:}
LABELV $302
endproc Com_sprintf 32012 12
bss
align 1
LABELV $311
skip 64000
data
align 4
LABELV $312
byte 4 0
export va
code
proc va 12 12
line 902
;891:
;892:
;893:/*
;894:============
;895:va
;896:
;897:does a varargs printf into a temp buffer, so I don't need to have
;898:varargs versions of all text functions.
;899:FIXME: make this buffer size safe someday
;900:============
;901:*/
;902:char	* QDECL va( char *format, ... ) {
line 908
;903:	va_list		argptr;
;904:	static char		string[2][32000];	// in case va is called by nested functions
;905:	static int		index = 0;
;906:	char	*buf;
;907:
;908:	buf = string[index & 1];
ADDRLP4 4
CNSTI4 32000
ADDRGP4 $312
INDIRI4
CNSTI4 1
BANDI4
MULI4
ADDRGP4 $311
ADDP4
ASGNP4
line 909
;909:	index++;
ADDRLP4 8
ADDRGP4 $312
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 911
;910:
;911:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 912
;912:	vsprintf (buf, format,argptr);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 913
;913:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 915
;914:
;915:	return buf;
ADDRLP4 4
INDIRP4
RETP4
LABELV $310
endproc va 12 12
bss
align 1
LABELV $315
skip 16384
data
align 4
LABELV $316
byte 4 0
export Info_ValueForKey
code
proc Info_ValueForKey 8220 8
line 936
;916:}
;917:
;918:
;919:/*
;920:=====================================================================
;921:
;922:  INFO STRINGS
;923:
;924:=====================================================================
;925:*/
;926:
;927:/*
;928:===============
;929:Info_ValueForKey
;930:
;931:Searches the string for the given
;932:key and returns the associated value, or an empty string.
;933:FIXME: overflow check?
;934:===============
;935:*/
;936:char *Info_ValueForKey( const char *s, const char *key ) {
line 943
;937:	char	pkey[BIG_INFO_KEY];
;938:	static	char value[2][BIG_INFO_VALUE];	// use two buffers so compares
;939:											// work without stomping on each other
;940:	static	int	valueindex = 0;
;941:	char	*o;
;942:	
;943:	if ( !s || !key ) {
ADDRLP4 8196
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 8196
INDIRU4
EQU4 $319
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 8196
INDIRU4
NEU4 $317
LABELV $319
line 944
;944:		return "";
ADDRGP4 $320
RETP4
ADDRGP4 $314
JUMPV
LABELV $317
line 947
;945:	}
;946:
;947:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8200
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8200
INDIRI4
CNSTI4 8192
LTI4 $321
line 948
;948:		Com_Error( ERR_DROP, "Info_ValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $323
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 949
;949:	}
LABELV $321
line 951
;950:
;951:	valueindex ^= 1;
ADDRLP4 8204
ADDRGP4 $316
ASGNP4
ADDRLP4 8204
INDIRP4
ADDRLP4 8204
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 952
;952:	if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $327
line 953
;953:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $327
JUMPV
LABELV $326
line 955
;954:	while (1)
;955:	{
line 956
;956:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $330
JUMPV
LABELV $329
line 958
;957:		while (*s != '\\')
;958:		{
line 959
;959:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $332
line 960
;960:				return "";
ADDRGP4 $320
RETP4
ADDRGP4 $314
JUMPV
LABELV $332
line 961
;961:			*o++ = *s++;
ADDRLP4 8208
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8216
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 8208
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8212
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8208
INDIRP4
ADDRLP4 8212
INDIRP4
INDIRI1
ASGNI1
line 962
;962:		}
LABELV $330
line 957
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $329
line 963
;963:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 964
;964:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 966
;965:
;966:		o = value[valueindex];
ADDRLP4 0
ADDRGP4 $316
INDIRI4
CNSTI4 13
LSHI4
ADDRGP4 $315
ADDP4
ASGNP4
ADDRGP4 $335
JUMPV
LABELV $334
line 969
;967:
;968:		while (*s != '\\' && *s)
;969:		{
line 970
;970:			*o++ = *s++;
ADDRLP4 8208
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8216
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 8208
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8212
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8208
INDIRP4
ADDRLP4 8212
INDIRP4
INDIRI1
ASGNI1
line 971
;971:		}
LABELV $335
line 968
ADDRLP4 8208
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 8208
INDIRI4
CNSTI4 92
EQI4 $337
ADDRLP4 8208
INDIRI4
CNSTI4 0
NEI4 $334
LABELV $337
line 972
;972:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 974
;973:
;974:		if (!Q_stricmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 8212
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8212
INDIRI4
CNSTI4 0
NEI4 $338
line 975
;975:			return value[valueindex];
ADDRGP4 $316
INDIRI4
CNSTI4 13
LSHI4
ADDRGP4 $315
ADDP4
RETP4
ADDRGP4 $314
JUMPV
LABELV $338
line 977
;976:
;977:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $340
line 978
;978:			break;
ADDRGP4 $328
JUMPV
LABELV $340
line 979
;979:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 980
;980:	}
LABELV $327
line 954
ADDRGP4 $326
JUMPV
LABELV $328
line 982
;981:
;982:	return "";
ADDRGP4 $320
RETP4
LABELV $314
endproc Info_ValueForKey 8220 8
export Info_NextPair
proc Info_NextPair 20 0
line 993
;983:}
;984:
;985:
;986:/*
;987:===================
;988:Info_NextPair
;989:
;990:Used to itterate through all the key/value pairs in an info string
;991:===================
;992:*/
;993:void Info_NextPair( const char **head, char *key, char *value ) {
line 997
;994:	char	*o;
;995:	const char	*s;
;996:
;997:	s = *head;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
line 999
;998:
;999:	if ( *s == '\\' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $343
line 1000
;1000:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1001
;1001:	}
LABELV $343
line 1002
;1002:	key[0] = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1003
;1003:	value[0] = 0;
ADDRFP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 1005
;1004:
;1005:	o = key;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRGP4 $346
JUMPV
LABELV $345
line 1006
;1006:	while ( *s != '\\' ) {
line 1007
;1007:		if ( !*s ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $348
line 1008
;1008:			*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1009
;1009:			*head = s;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1010
;1010:			return;
ADDRGP4 $342
JUMPV
LABELV $348
line 1012
;1011:		}
;1012:		*o++ = *s++;
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI1
ASGNI1
line 1013
;1013:	}
LABELV $346
line 1006
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $345
line 1014
;1014:	*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1015
;1015:	s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1017
;1016:
;1017:	o = value;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRGP4 $351
JUMPV
LABELV $350
line 1018
;1018:	while ( *s != '\\' && *s ) {
line 1019
;1019:		*o++ = *s++;
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI1
ASGNI1
line 1020
;1020:	}
LABELV $351
line 1018
ADDRLP4 8
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 92
EQI4 $353
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $350
LABELV $353
line 1021
;1021:	*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1023
;1022:
;1023:	*head = s;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1024
;1024:}
LABELV $342
endproc Info_NextPair 20 0
export Info_RemoveKey
proc Info_RemoveKey 2076 8
line 1032
;1025:
;1026:
;1027:/*
;1028:===================
;1029:Info_RemoveKey
;1030:===================
;1031:*/
;1032:void Info_RemoveKey( char *s, const char *key ) {
line 1038
;1033:	char	*start;
;1034:	char	pkey[MAX_INFO_KEY];
;1035:	char	value[MAX_INFO_VALUE];
;1036:	char	*o;
;1037:
;1038:	if ( strlen( s ) >= MAX_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 2056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 2056
INDIRI4
CNSTI4 1024
LTI4 $355
line 1039
;1039:		Com_Error( ERR_DROP, "Info_RemoveKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $357
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1040
;1040:	}
LABELV $355
line 1042
;1041:
;1042:	if (strchr (key, '\\')) {
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 2060
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 2060
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $361
line 1043
;1043:		return;
ADDRGP4 $354
JUMPV
LABELV $360
line 1047
;1044:	}
;1045:
;1046:	while (1)
;1047:	{
line 1048
;1048:		start = s;
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
line 1049
;1049:		if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $363
line 1050
;1050:			s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $363
line 1051
;1051:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $366
JUMPV
LABELV $365
line 1053
;1052:		while (*s != '\\')
;1053:		{
line 1054
;1054:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $368
line 1055
;1055:				return;
ADDRGP4 $354
JUMPV
LABELV $368
line 1056
;1056:			*o++ = *s++;
ADDRLP4 2064
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 2072
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2068
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 2068
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2064
INDIRP4
ADDRLP4 2068
INDIRP4
INDIRI1
ASGNI1
line 1057
;1057:		}
LABELV $366
line 1052
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $365
line 1058
;1058:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1059
;1059:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1061
;1060:
;1061:		o = value;
ADDRLP4 0
ADDRLP4 1032
ASGNP4
ADDRGP4 $371
JUMPV
LABELV $370
line 1063
;1062:		while (*s != '\\' && *s)
;1063:		{
line 1064
;1064:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $373
line 1065
;1065:				return;
ADDRGP4 $354
JUMPV
LABELV $373
line 1066
;1066:			*o++ = *s++;
ADDRLP4 2064
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 2072
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2068
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 2068
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2064
INDIRP4
ADDRLP4 2068
INDIRP4
INDIRI1
ASGNI1
line 1067
;1067:		}
LABELV $371
line 1062
ADDRLP4 2064
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 2064
INDIRI4
CNSTI4 92
EQI4 $375
ADDRLP4 2064
INDIRI4
CNSTI4 0
NEI4 $370
LABELV $375
line 1068
;1068:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1070
;1069:
;1070:		if (!strcmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $376
line 1071
;1071:		{
line 1072
;1072:			strcpy (start, s);	// remove this part
ADDRLP4 1028
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1073
;1073:			return;
ADDRGP4 $354
JUMPV
LABELV $376
line 1076
;1074:		}
;1075:
;1076:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $378
line 1077
;1077:			return;
ADDRGP4 $354
JUMPV
LABELV $378
line 1078
;1078:	}
LABELV $361
line 1046
ADDRGP4 $360
JUMPV
line 1080
;1079:
;1080:}
LABELV $354
endproc Info_RemoveKey 2076 8
export Info_RemoveKey_Big
proc Info_RemoveKey_Big 16412 8
line 1087
;1081:
;1082:/*
;1083:===================
;1084:Info_RemoveKey_Big
;1085:===================
;1086:*/
;1087:void Info_RemoveKey_Big( char *s, const char *key ) {
line 1093
;1088:	char	*start;
;1089:	char	pkey[BIG_INFO_KEY];
;1090:	char	value[BIG_INFO_VALUE];
;1091:	char	*o;
;1092:
;1093:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16392
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 16392
INDIRI4
CNSTI4 8192
LTI4 $381
line 1094
;1094:		Com_Error( ERR_DROP, "Info_RemoveKey_Big: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $383
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1095
;1095:	}
LABELV $381
line 1097
;1096:
;1097:	if (strchr (key, '\\')) {
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 16396
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 16396
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $387
line 1098
;1098:		return;
ADDRGP4 $380
JUMPV
LABELV $386
line 1102
;1099:	}
;1100:
;1101:	while (1)
;1102:	{
line 1103
;1103:		start = s;
ADDRLP4 8196
ADDRFP4 0
INDIRP4
ASGNP4
line 1104
;1104:		if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $389
line 1105
;1105:			s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $389
line 1106
;1106:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $392
JUMPV
LABELV $391
line 1108
;1107:		while (*s != '\\')
;1108:		{
line 1109
;1109:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $394
line 1110
;1110:				return;
ADDRGP4 $380
JUMPV
LABELV $394
line 1111
;1111:			*o++ = *s++;
ADDRLP4 16400
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16408
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 16400
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16404
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 16404
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16400
INDIRP4
ADDRLP4 16404
INDIRP4
INDIRI1
ASGNI1
line 1112
;1112:		}
LABELV $392
line 1107
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $391
line 1113
;1113:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1114
;1114:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1116
;1115:
;1116:		o = value;
ADDRLP4 0
ADDRLP4 8200
ASGNP4
ADDRGP4 $397
JUMPV
LABELV $396
line 1118
;1117:		while (*s != '\\' && *s)
;1118:		{
line 1119
;1119:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $399
line 1120
;1120:				return;
ADDRGP4 $380
JUMPV
LABELV $399
line 1121
;1121:			*o++ = *s++;
ADDRLP4 16400
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16408
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 16400
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16404
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 16404
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16400
INDIRP4
ADDRLP4 16404
INDIRP4
INDIRI1
ASGNI1
line 1122
;1122:		}
LABELV $397
line 1117
ADDRLP4 16400
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16400
INDIRI4
CNSTI4 92
EQI4 $401
ADDRLP4 16400
INDIRI4
CNSTI4 0
NEI4 $396
LABELV $401
line 1123
;1123:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1125
;1124:
;1125:		if (!strcmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 16404
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 16404
INDIRI4
CNSTI4 0
NEI4 $402
line 1126
;1126:		{
line 1127
;1127:			strcpy (start, s);	// remove this part
ADDRLP4 8196
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1128
;1128:			return;
ADDRGP4 $380
JUMPV
LABELV $402
line 1131
;1129:		}
;1130:
;1131:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $404
line 1132
;1132:			return;
ADDRGP4 $380
JUMPV
LABELV $404
line 1133
;1133:	}
LABELV $387
line 1101
ADDRGP4 $386
JUMPV
line 1135
;1134:
;1135:}
LABELV $380
endproc Info_RemoveKey_Big 16412 8
export Info_Validate
proc Info_Validate 8 8
line 1148
;1136:
;1137:
;1138:
;1139:
;1140:/*
;1141:==================
;1142:Info_Validate
;1143:
;1144:Some characters are illegal in info strings because they
;1145:can mess up the server's parsing
;1146:==================
;1147:*/
;1148:qboolean Info_Validate( const char *s ) {
line 1149
;1149:	if ( strchr( s, '\"' ) ) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 0
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $407
line 1150
;1150:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $406
JUMPV
LABELV $407
line 1152
;1151:	}
;1152:	if ( strchr( s, ';' ) ) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 4
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $409
line 1153
;1153:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $406
JUMPV
LABELV $409
line 1155
;1154:	}
;1155:	return qtrue;
CNSTI4 1
RETI4
LABELV $406
endproc Info_Validate 8 8
export Info_SetValueForKey
proc Info_SetValueForKey 1068 20
line 1165
;1156:}
;1157:
;1158:/*
;1159:==================
;1160:Info_SetValueForKey
;1161:
;1162:Changes or adds a key/value pair
;1163:==================
;1164:*/
;1165:void Info_SetValueForKey( char *s, const char *key, const char *value ) {
line 1168
;1166:	char	newi[MAX_INFO_STRING];
;1167:
;1168:	if ( strlen( s ) >= MAX_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1024
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 1024
LTI4 $412
line 1169
;1169:		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $414
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1170
;1170:	}
LABELV $412
line 1172
;1171:
;1172:	if (strchr (key, '\\') || strchr (value, '\\'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 1028
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $417
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 1032
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1032
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $415
LABELV $417
line 1173
;1173:	{
line 1174
;1174:		Com_Printf ("Can't use keys or values with a \\\n");
ADDRGP4 $418
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1175
;1175:		return;
ADDRGP4 $411
JUMPV
LABELV $415
line 1178
;1176:	}
;1177:
;1178:	if (strchr (key, ';') || strchr (value, ';'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 1036
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $421
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 1040
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $419
LABELV $421
line 1179
;1179:	{
line 1180
;1180:		Com_Printf ("Can't use keys or values with a semicolon\n");
ADDRGP4 $422
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1181
;1181:		return;
ADDRGP4 $411
JUMPV
LABELV $419
line 1184
;1182:	}
;1183:
;1184:	if (strchr (key, '\"') || strchr (value, '\"'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1044
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1044
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $425
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1048
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1048
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $423
LABELV $425
line 1185
;1185:	{
line 1186
;1186:		Com_Printf ("Can't use keys or values with a \"\n");
ADDRGP4 $426
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1187
;1187:		return;
ADDRGP4 $411
JUMPV
LABELV $423
line 1190
;1188:	}
;1189:
;1190:	Info_RemoveKey (s, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Info_RemoveKey
CALLV
pop
line 1191
;1191:	if (!value || !strlen(value))
ADDRLP4 1052
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1052
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $429
ADDRLP4 1052
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $427
LABELV $429
line 1192
;1192:		return;
ADDRGP4 $411
JUMPV
LABELV $427
line 1194
;1193:
;1194:	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $430
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1196
;1195:
;1196:	if (strlen(newi) + strlen(s) > MAX_INFO_STRING)
ADDRLP4 0
ARGP4
ADDRLP4 1060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
ADDRLP4 1064
INDIRI4
ADDI4
CNSTI4 1024
LEI4 $431
line 1197
;1197:	{
line 1198
;1198:		Com_Printf ("Info string length exceeded\n");
ADDRGP4 $433
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1199
;1199:		return;
ADDRGP4 $411
JUMPV
LABELV $431
line 1202
;1200:	}
;1201:
;1202:	strcat (newi, s);
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1203
;1203:	strcpy (s, newi);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1204
;1204:}
LABELV $411
endproc Info_SetValueForKey 1068 20
export Info_SetValueForKey_Big
proc Info_SetValueForKey_Big 8236 20
line 1213
;1205:
;1206:/*
;1207:==================
;1208:Info_SetValueForKey_Big
;1209:
;1210:Changes or adds a key/value pair
;1211:==================
;1212:*/
;1213:void Info_SetValueForKey_Big( char *s, const char *key, const char *value ) {
line 1216
;1214:	char	newi[BIG_INFO_STRING];
;1215:
;1216:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8192
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8192
INDIRI4
CNSTI4 8192
LTI4 $435
line 1217
;1217:		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $414
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1218
;1218:	}
LABELV $435
line 1220
;1219:
;1220:	if (strchr (key, '\\') || strchr (value, '\\'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 8196
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8196
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $439
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 8200
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8200
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $437
LABELV $439
line 1221
;1221:	{
line 1222
;1222:		Com_Printf ("Can't use keys or values with a \\\n");
ADDRGP4 $418
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1223
;1223:		return;
ADDRGP4 $434
JUMPV
LABELV $437
line 1226
;1224:	}
;1225:
;1226:	if (strchr (key, ';') || strchr (value, ';'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 8204
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $442
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 8208
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8208
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $440
LABELV $442
line 1227
;1227:	{
line 1228
;1228:		Com_Printf ("Can't use keys or values with a semicolon\n");
ADDRGP4 $422
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1229
;1229:		return;
ADDRGP4 $434
JUMPV
LABELV $440
line 1232
;1230:	}
;1231:
;1232:	if (strchr (key, '\"') || strchr (value, '\"'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 8212
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8212
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $445
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 8216
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8216
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $443
LABELV $445
line 1233
;1233:	{
line 1234
;1234:		Com_Printf ("Can't use keys or values with a \"\n");
ADDRGP4 $426
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1235
;1235:		return;
ADDRGP4 $434
JUMPV
LABELV $443
line 1238
;1236:	}
;1237:
;1238:	Info_RemoveKey_Big (s, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Info_RemoveKey_Big
CALLV
pop
line 1239
;1239:	if (!value || !strlen(value))
ADDRLP4 8220
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8220
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $448
ADDRLP4 8220
INDIRP4
ARGP4
ADDRLP4 8224
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8224
INDIRI4
CNSTI4 0
NEI4 $446
LABELV $448
line 1240
;1240:		return;
ADDRGP4 $434
JUMPV
LABELV $446
line 1242
;1241:
;1242:	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);
ADDRLP4 0
ARGP4
CNSTI4 8192
ARGI4
ADDRGP4 $430
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1244
;1243:
;1244:	if (strlen(newi) + strlen(s) > BIG_INFO_STRING)
ADDRLP4 0
ARGP4
ADDRLP4 8228
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8232
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8228
INDIRI4
ADDRLP4 8232
INDIRI4
ADDI4
CNSTI4 8192
LEI4 $449
line 1245
;1245:	{
line 1246
;1246:		Com_Printf ("BIG Info string length exceeded\n");
ADDRGP4 $451
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1247
;1247:		return;
ADDRGP4 $434
JUMPV
LABELV $449
line 1250
;1248:	}
;1249:
;1250:	strcat (s, newi);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1251
;1251:}
LABELV $434
endproc Info_SetValueForKey_Big 8236 20
bss
align 4
LABELV com_lines
skip 4
align 1
LABELV com_parsename
skip 1024
align 1
LABELV com_token
skip 1024
import Com_Printf
import Com_Error
import Info_RemoveKey_big
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
LABELV $451
byte 1 66
byte 1 73
byte 1 71
byte 1 32
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 32
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 116
byte 1 104
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
LABELV $433
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 32
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 116
byte 1 104
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
LABELV $430
byte 1 92
byte 1 37
byte 1 115
byte 1 92
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $426
byte 1 67
byte 1 97
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 115
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 115
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 97
byte 1 32
byte 1 34
byte 1 10
byte 1 0
align 1
LABELV $422
byte 1 67
byte 1 97
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 115
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 115
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 97
byte 1 32
byte 1 115
byte 1 101
byte 1 109
byte 1 105
byte 1 99
byte 1 111
byte 1 108
byte 1 111
byte 1 110
byte 1 10
byte 1 0
align 1
LABELV $418
byte 1 67
byte 1 97
byte 1 110
byte 1 39
byte 1 116
byte 1 32
byte 1 117
byte 1 115
byte 1 101
byte 1 32
byte 1 107
byte 1 101
byte 1 121
byte 1 115
byte 1 32
byte 1 111
byte 1 114
byte 1 32
byte 1 118
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 115
byte 1 32
byte 1 119
byte 1 105
byte 1 116
byte 1 104
byte 1 32
byte 1 97
byte 1 32
byte 1 92
byte 1 10
byte 1 0
align 1
LABELV $414
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 83
byte 1 101
byte 1 116
byte 1 86
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 70
byte 1 111
byte 1 114
byte 1 75
byte 1 101
byte 1 121
byte 1 58
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $383
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 82
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 75
byte 1 101
byte 1 121
byte 1 95
byte 1 66
byte 1 105
byte 1 103
byte 1 58
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $357
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 82
byte 1 101
byte 1 109
byte 1 111
byte 1 118
byte 1 101
byte 1 75
byte 1 101
byte 1 121
byte 1 58
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $323
byte 1 73
byte 1 110
byte 1 102
byte 1 111
byte 1 95
byte 1 86
byte 1 97
byte 1 108
byte 1 117
byte 1 101
byte 1 70
byte 1 111
byte 1 114
byte 1 75
byte 1 101
byte 1 121
byte 1 58
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 32
byte 1 105
byte 1 110
byte 1 102
byte 1 111
byte 1 115
byte 1 116
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $320
byte 1 0
align 1
LABELV $309
byte 1 67
byte 1 111
byte 1 109
byte 1 95
byte 1 115
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 102
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
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 105
byte 1 110
byte 1 32
byte 1 37
byte 1 105
byte 1 10
byte 1 0
align 1
LABELV $306
byte 1 67
byte 1 111
byte 1 109
byte 1 95
byte 1 115
byte 1 112
byte 1 114
byte 1 105
byte 1 110
byte 1 116
byte 1 102
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
byte 1 101
byte 1 100
byte 1 32
byte 1 98
byte 1 105
byte 1 103
byte 1 98
byte 1 117
byte 1 102
byte 1 102
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $285
byte 1 81
byte 1 95
byte 1 115
byte 1 116
byte 1 114
byte 1 99
byte 1 97
byte 1 116
byte 1 58
byte 1 32
byte 1 97
byte 1 108
byte 1 114
byte 1 101
byte 1 97
byte 1 100
byte 1 121
byte 1 32
byte 1 111
byte 1 118
byte 1 101
byte 1 114
byte 1 102
byte 1 108
byte 1 111
byte 1 119
byte 1 101
byte 1 100
byte 1 0
align 1
LABELV $235
byte 1 81
byte 1 95
byte 1 115
byte 1 116
byte 1 114
byte 1 110
byte 1 99
byte 1 112
byte 1 121
byte 1 122
byte 1 58
byte 1 32
byte 1 100
byte 1 101
byte 1 115
byte 1 116
byte 1 115
byte 1 105
byte 1 122
byte 1 101
byte 1 32
byte 1 60
byte 1 32
byte 1 49
byte 1 0
align 1
LABELV $232
byte 1 81
byte 1 95
byte 1 115
byte 1 116
byte 1 114
byte 1 110
byte 1 99
byte 1 112
byte 1 121
byte 1 122
byte 1 58
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 115
byte 1 114
byte 1 99
byte 1 0
align 1
LABELV $229
byte 1 81
byte 1 95
byte 1 115
byte 1 116
byte 1 114
byte 1 110
byte 1 99
byte 1 112
byte 1 121
byte 1 122
byte 1 58
byte 1 32
byte 1 78
byte 1 85
byte 1 76
byte 1 76
byte 1 32
byte 1 100
byte 1 101
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $193
byte 1 41
byte 1 0
align 1
LABELV $188
byte 1 40
byte 1 0
align 1
LABELV $169
byte 1 77
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 84
byte 1 111
byte 1 107
byte 1 101
byte 1 110
byte 1 58
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 33
byte 1 61
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $75
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
LABELV $71
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
LABELV $65
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $45
byte 1 37
byte 1 115
byte 1 37
byte 1 115
byte 1 0
