data
export passwordNeeded
align 4
LABELV passwordNeeded
byte 4 1
code
proc UI_ReadableSize 16 16
file "../ui_connect.c"
line 40
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
;25:/*
;26:===============================================================================
;27:
;28:CONNECTION SCREEN
;29:
;30:===============================================================================
;31:*/
;32:
;33:qboolean	passwordNeeded = qtrue;
;34:menufield_s passwordField;
;35:
;36:static connstate_t	lastConnState;
;37:static char			lastLoadingText[MAX_INFO_VALUE];
;38:
;39:static void UI_ReadableSize ( char *buf, int bufsize, int value )
;40:{
line 41
;41:	if (value > 1024*1024*1024 ) { // gigs
ADDRFP4 8
INDIRI4
CNSTI4 1073741824
LEI4 $68
line 42
;42:		Com_sprintf( buf, bufsize, "%d", value / (1024*1024*1024) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $70
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1073741824
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 43
;43:		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d GB", 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ARGI4
ADDRGP4 $71
ARGP4
ADDRLP4 12
CNSTI4 1073741824
ASGNI4
CNSTI4 100
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
MODI4
MULI4
ADDRLP4 12
INDIRI4
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 45
;44:			(value % (1024*1024*1024))*100 / (1024*1024*1024) );
;45:	} else if (value > 1024*1024 ) { // megs
ADDRGP4 $69
JUMPV
LABELV $68
ADDRFP4 8
INDIRI4
CNSTI4 1048576
LEI4 $72
line 46
;46:		Com_sprintf( buf, bufsize, "%d", value / (1024*1024) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $70
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1048576
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 47
;47:		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d MB", 
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRP4
ADDP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ARGI4
ADDRGP4 $74
ARGP4
ADDRLP4 12
CNSTI4 1048576
ASGNI4
CNSTI4 100
ADDRFP4 8
INDIRI4
ADDRLP4 12
INDIRI4
MODI4
MULI4
ADDRLP4 12
INDIRI4
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 49
;48:			(value % (1024*1024))*100 / (1024*1024) );
;49:	} else if (value > 1024 ) { // kilos
ADDRGP4 $73
JUMPV
LABELV $72
ADDRFP4 8
INDIRI4
CNSTI4 1024
LEI4 $75
line 50
;50:		Com_sprintf( buf, bufsize, "%d KB", value / 1024 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $77
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1024
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 51
;51:	} else { // bytes
ADDRGP4 $76
JUMPV
LABELV $75
line 52
;52:		Com_sprintf( buf, bufsize, "%d bytes", value );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $78
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 53
;53:	}
LABELV $76
LABELV $73
LABELV $69
line 54
;54:}
LABELV $67
endproc UI_ReadableSize 16 16
proc UI_PrintTime 8 20
line 57
;55:
;56:// Assumes time is in msec
;57:static void UI_PrintTime ( char *buf, int bufsize, int time ) {
line 58
;58:	time /= 1000;  // change to seconds
ADDRFP4 8
ADDRFP4 8
INDIRI4
CNSTI4 1000
DIVI4
ASGNI4
line 60
;59:
;60:	if (time > 3600) { // in the hours range
ADDRFP4 8
INDIRI4
CNSTI4 3600
LEI4 $80
line 61
;61:		Com_sprintf( buf, bufsize, "%d hr %d min", time / 3600, (time % 3600) / 60 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $82
ARGP4
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 3600
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
DIVI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
CNSTI4 60
DIVI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 62
;62:	} else if (time > 60) { // mins
ADDRGP4 $81
JUMPV
LABELV $80
ADDRFP4 8
INDIRI4
CNSTI4 60
LEI4 $83
line 63
;63:		Com_sprintf( buf, bufsize, "%d min %d sec", time / 60, time % 60 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $85
ARGP4
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
CNSTI4 60
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
DIVI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 64
;64:	} else  { // secs
ADDRGP4 $84
JUMPV
LABELV $83
line 65
;65:		Com_sprintf( buf, bufsize, "%d sec", time );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $86
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 66
;66:	}
LABELV $84
LABELV $81
line 67
;67:}
LABELV $79
endproc UI_PrintTime 8 20
data
align 1
LABELV $88
byte 1 68
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 58
byte 1 0
align 1
LABELV $89
byte 1 69
byte 1 115
byte 1 116
byte 1 105
byte 1 109
byte 1 97
byte 1 116
byte 1 101
byte 1 100
byte 1 32
byte 1 116
byte 1 105
byte 1 109
byte 1 101
byte 1 32
byte 1 108
byte 1 101
byte 1 102
byte 1 116
byte 1 58
byte 1 0
align 1
LABELV $90
byte 1 84
byte 1 114
byte 1 97
byte 1 110
byte 1 115
byte 1 102
byte 1 101
byte 1 114
byte 1 32
byte 1 114
byte 1 97
byte 1 116
byte 1 101
byte 1 58
byte 1 0
code
proc UI_DisplayDownloadInfo 348 20
line 69
;68:
;69:static void UI_DisplayDownloadInfo( const char *downloadName ) {
line 78
;70:	static char dlText[]	= "Downloading:";
;71:	static char etaText[]	= "Estimated time left:";
;72:	static char xferText[]	= "Transfer rate:";
;73:
;74:	int downloadSize, downloadCount, downloadTime;
;75:	char dlSizeBuf[64], totalSizeBuf[64], xferRateBuf[64], dlTimeBuf[64];
;76:	int xferRate;
;77:	int width, leftWidth;
;78:	int style = UI_LEFT|UI_SMALLFONT|UI_DROPSHADOW;
ADDRLP4 0
CNSTI4 2064
ASGNI4
line 81
;79:	const char *s;
;80:
;81:	downloadSize = trap_Cvar_VariableValue( "cl_downloadSize" );
ADDRGP4 $91
ARGP4
ADDRLP4 288
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 288
INDIRF4
CVFI4 4
ASGNI4
line 82
;82:	downloadCount = trap_Cvar_VariableValue( "cl_downloadCount" );
ADDRGP4 $92
ARGP4
ADDRLP4 292
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 292
INDIRF4
CVFI4 4
ASGNI4
line 83
;83:	downloadTime = trap_Cvar_VariableValue( "cl_downloadTime" );
ADDRGP4 $93
ARGP4
ADDRLP4 296
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 148
ADDRLP4 296
INDIRF4
CVFI4 4
ASGNI4
line 94
;84:
;85:#if 0 // bk010104
;86:	fprintf( stderr, "\n\n-----------------------------------------------\n");
;87:	fprintf( stderr, "DB: downloadSize:  %16d\n", downloadSize );
;88:	fprintf( stderr, "DB: downloadCount: %16d\n", downloadCount );
;89:	fprintf( stderr, "DB: downloadTime:  %16d\n", downloadTime );  
;90:  	fprintf( stderr, "DB: UI realtime:   %16d\n", uis.realtime );	// bk
;91:	fprintf( stderr, "DB: UI frametime:  %16d\n", uis.frametime );	// bk
;92:#endif
;93:
;94:	leftWidth = width = UI_ProportionalStringWidth( dlText ) * UI_ProportionalSizeScale( style );
ADDRGP4 $88
ARGP4
ADDRLP4 300
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 304
ADDRGP4 UI_ProportionalSizeScale
CALLF4
ASGNF4
ADDRLP4 308
ADDRLP4 300
INDIRI4
CVIF4 4
ADDRLP4 304
INDIRF4
MULF4
CVFI4 4
ASGNI4
ADDRLP4 8
ADDRLP4 308
INDIRI4
ASGNI4
ADDRLP4 4
ADDRLP4 308
INDIRI4
ASGNI4
line 95
;95:	width = UI_ProportionalStringWidth( etaText ) * UI_ProportionalSizeScale( style );
ADDRGP4 $89
ARGP4
ADDRLP4 312
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 316
ADDRGP4 UI_ProportionalSizeScale
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 312
INDIRI4
CVIF4 4
ADDRLP4 316
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 96
;96:	if (width > leftWidth) leftWidth = width;
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $94
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $94
line 97
;97:	width = UI_ProportionalStringWidth( xferText ) * UI_ProportionalSizeScale( style );
ADDRGP4 $90
ARGP4
ADDRLP4 320
ADDRGP4 UI_ProportionalStringWidth
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 324
ADDRGP4 UI_ProportionalSizeScale
CALLF4
ASGNF4
ADDRLP4 8
ADDRLP4 320
INDIRI4
CVIF4 4
ADDRLP4 324
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 98
;98:	if (width > leftWidth) leftWidth = width;
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $96
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $96
line 99
;99:	leftWidth += 16;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 101
;100:
;101:	UI_DrawProportionalString( 8, 128, dlText, style, color_white );
CNSTI4 8
ARGI4
CNSTI4 128
ARGI4
ADDRGP4 $88
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 102
;102:	UI_DrawProportionalString( 8, 160, etaText, style, color_white );
CNSTI4 8
ARGI4
CNSTI4 160
ARGI4
ADDRGP4 $89
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 103
;103:	UI_DrawProportionalString( 8, 224, xferText, style, color_white );
CNSTI4 8
ARGI4
CNSTI4 224
ARGI4
ADDRGP4 $90
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 105
;104:
;105:	if (downloadSize > 0) {
ADDRLP4 12
INDIRI4
CNSTI4 0
LEI4 $98
line 106
;106:		s = va( "%s (%d%%)", downloadName, downloadCount * 100 / downloadSize );
ADDRGP4 $100
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 100
ADDRLP4 16
INDIRI4
MULI4
ADDRLP4 12
INDIRI4
DIVI4
ARGI4
ADDRLP4 328
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 156
ADDRLP4 328
INDIRP4
ASGNP4
line 107
;107:	} else {
ADDRGP4 $99
JUMPV
LABELV $98
line 108
;108:		s = downloadName;
ADDRLP4 156
ADDRFP4 0
INDIRP4
ASGNP4
line 109
;109:	}
LABELV $99
line 111
;110:
;111:	UI_DrawProportionalString( leftWidth, 128, s, style, color_white );
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 128
ARGI4
ADDRLP4 156
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 113
;112:
;113:	UI_ReadableSize( dlSizeBuf,		sizeof dlSizeBuf,		downloadCount );
ADDRLP4 20
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 16
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 114
;114:	UI_ReadableSize( totalSizeBuf,	sizeof totalSizeBuf,	downloadSize );
ADDRLP4 84
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 116
;115:
;116:	if (downloadCount < 4096 || !downloadTime) {
ADDRLP4 16
INDIRI4
CNSTI4 4096
LTI4 $103
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $101
LABELV $103
line 117
;117:		UI_DrawProportionalString( leftWidth, 160, "estimating", style, color_white );
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 160
ARGI4
ADDRGP4 $104
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 118
;118:		UI_DrawProportionalString( leftWidth, 192, 
ADDRGP4 $105
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 328
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 192
ARGI4
ADDRLP4 328
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 120
;119:			va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
;120:	} else {
ADDRGP4 $102
JUMPV
LABELV $101
line 125
;121:	  // bk010108
;122:	  //float elapsedTime = (float)(uis.realtime - downloadTime); // current - start (msecs)
;123:	  //elapsedTime = elapsedTime * 0.001f; // in seconds
;124:	  //if ( elapsedTime <= 0.0f ) elapsedTime == 0.0f;
;125:	  if ( (uis.realtime - downloadTime) / 1000) {
ADDRGP4 uis+4
INDIRI4
ADDRLP4 148
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
CNSTI4 0
EQI4 $106
line 126
;126:			xferRate = downloadCount / ((uis.realtime - downloadTime) / 1000);
ADDRLP4 152
ADDRLP4 16
INDIRI4
ADDRGP4 uis+4
INDIRI4
ADDRLP4 148
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
DIVI4
ASGNI4
line 128
;127:		  //xferRate = (int)( ((float)downloadCount) / elapsedTime);
;128:		} else {
ADDRGP4 $107
JUMPV
LABELV $106
line 129
;129:			xferRate = 0;
ADDRLP4 152
CNSTI4 0
ASGNI4
line 130
;130:		}
LABELV $107
line 135
;131:
;132:	  //fprintf( stderr, "DB: elapsedTime:  %16.8f\n", elapsedTime );	// bk
;133:	  //fprintf( stderr, "DB: xferRate:   %16d\n", xferRate );	// bk
;134:
;135:		UI_ReadableSize( xferRateBuf, sizeof xferRateBuf, xferRate );
ADDRLP4 160
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 152
INDIRI4
ARGI4
ADDRGP4 UI_ReadableSize
CALLV
pop
line 138
;136:
;137:		// Extrapolate estimated completion time
;138:		if (downloadSize && xferRate) {
ADDRLP4 328
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 328
INDIRI4
EQI4 $110
ADDRLP4 152
INDIRI4
ADDRLP4 328
INDIRI4
EQI4 $110
line 139
;139:			int n = downloadSize / xferRate; // estimated time for entire d/l in secs
ADDRLP4 332
ADDRLP4 12
INDIRI4
ADDRLP4 152
INDIRI4
DIVI4
ASGNI4
line 142
;140:
;141:			// We do it in K (/1024) because we'd overflow around 4MB
;142:			n = (n - (((downloadCount/1024) * n) / (downloadSize/1024))) * 1000;
ADDRLP4 336
ADDRLP4 332
INDIRI4
ASGNI4
ADDRLP4 340
CNSTI4 1024
ASGNI4
ADDRLP4 332
CNSTI4 1000
ADDRLP4 336
INDIRI4
ADDRLP4 16
INDIRI4
ADDRLP4 340
INDIRI4
DIVI4
ADDRLP4 336
INDIRI4
MULI4
ADDRLP4 12
INDIRI4
ADDRLP4 340
INDIRI4
DIVI4
DIVI4
SUBI4
MULI4
ASGNI4
line 144
;143:			
;144:			UI_PrintTime ( dlTimeBuf, sizeof dlTimeBuf, n ); // bk010104
ADDRLP4 224
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 332
INDIRI4
ARGI4
ADDRGP4 UI_PrintTime
CALLV
pop
line 147
;145:				//(n - (((downloadCount/1024) * n) / (downloadSize/1024))) * 1000);
;146:
;147:			UI_DrawProportionalString( leftWidth, 160, 
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 160
ARGI4
ADDRLP4 224
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 149
;148:				dlTimeBuf, style, color_white );
;149:			UI_DrawProportionalString( leftWidth, 192, 
ADDRGP4 $105
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 344
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 192
ARGI4
ADDRLP4 344
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 151
;150:				va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
;151:		} else {
ADDRGP4 $111
JUMPV
LABELV $110
line 152
;152:			UI_DrawProportionalString( leftWidth, 160, 
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 160
ARGI4
ADDRGP4 $104
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 154
;153:				"estimating", style, color_white );
;154:			if (downloadSize) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $112
line 155
;155:				UI_DrawProportionalString( leftWidth, 192, 
ADDRGP4 $105
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 332
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 192
ARGI4
ADDRLP4 332
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 157
;156:					va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
;157:			} else {
ADDRGP4 $113
JUMPV
LABELV $112
line 158
;158:				UI_DrawProportionalString( leftWidth, 192, 
ADDRGP4 $114
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 332
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 192
ARGI4
ADDRLP4 332
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 160
;159:					va("(%s copied)", dlSizeBuf), style, color_white );
;160:			}
LABELV $113
line 161
;161:		}
LABELV $111
line 163
;162:
;163:		if (xferRate) {
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $115
line 164
;164:			UI_DrawProportionalString( leftWidth, 224, 
ADDRGP4 $117
ARGP4
ADDRLP4 160
ARGP4
ADDRLP4 332
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 224
ARGI4
ADDRLP4 332
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 166
;165:				va("%s/Sec", xferRateBuf), style, color_white );
;166:		}
LABELV $115
line 167
;167:	}
LABELV $102
line 168
;168:}
LABELV $87
endproc UI_DisplayDownloadInfo 348 20
export UI_DrawConnectScreen
proc UI_DrawConnectScreen 5164 28
line 178
;169:
;170:/*
;171:========================
;172:UI_DrawConnectScreen
;173:
;174:This will also be overlaid on the cgame info screen during loading
;175:to prevent it from blinking away too rapidly on local or lan games.
;176:========================
;177:*/
;178:void UI_DrawConnectScreen( qboolean overlay ) {
line 183
;179:	char			*s;
;180:	uiClientState_t	cstate;
;181:	char			info[MAX_INFO_VALUE];
;182:
;183:	Menu_Cache();
ADDRGP4 Menu_Cache
CALLV
pop
line 185
;184:
;185:	if ( !overlay ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $119
line 187
;186:		// draw the dialog background
;187:		UI_SetColor( color_white );
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_SetColor
CALLV
pop
line 188
;188:		UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menuBackShader );
ADDRLP4 4112
CNSTF4 0
ASGNF4
ADDRLP4 4112
INDIRF4
ARGF4
ADDRLP4 4112
INDIRF4
ARGF4
CNSTF4 1142947840
ARGF4
CNSTF4 1139802112
ARGF4
ADDRGP4 uis+11396
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 189
;189:	}
LABELV $119
line 192
;190:
;191:	// see what information we should display
;192:	trap_GetClientState( &cstate );
ADDRLP4 0
ARGP4
ADDRGP4 trap_GetClientState
CALLV
pop
line 194
;193:
;194:	info[0] = '\0';
ADDRLP4 3084
CNSTI1 0
ASGNI1
line 195
;195:	if( trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) ) ) {
CNSTI4 0
ARGI4
ADDRLP4 3084
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4112
ADDRGP4 trap_GetConfigString
CALLI4
ASGNI4
ADDRLP4 4112
INDIRI4
CNSTI4 0
EQI4 $122
line 196
;196:		UI_DrawProportionalString( 320, 16, va( "Loading %s", Info_ValueForKey( info, "mapname" ) ), UI_BIGFONT|UI_CENTER|UI_DROPSHADOW, color_white );
ADDRLP4 3084
ARGP4
ADDRGP4 $125
ARGP4
ADDRLP4 4116
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $124
ARGP4
ADDRLP4 4116
INDIRP4
ARGP4
ADDRLP4 4120
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 320
ARGI4
CNSTI4 16
ARGI4
ADDRLP4 4120
INDIRP4
ARGP4
CNSTI4 2081
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 197
;197:	}
LABELV $122
line 199
;198:
;199:	UI_DrawProportionalString( 320, 64, va("Connecting to %s", cstate.servername), UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
ADDRGP4 $126
ARGP4
ADDRLP4 0+12
ARGP4
ADDRLP4 4116
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 320
ARGI4
CNSTI4 64
ARGI4
ADDRLP4 4116
INDIRP4
ARGP4
CNSTI4 2065
ARGI4
ADDRGP4 menu_text_color
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 203
;200:	//UI_DrawProportionalString( 320, 96, "Press Esc to abort", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
;201:
;202:	// display global MOTD at bottom
;203:	UI_DrawProportionalString( SCREEN_WIDTH/2, SCREEN_HEIGHT-32, 
ADDRLP4 0+1036
ARGP4
ADDRGP4 $129
ARGP4
ADDRLP4 4120
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
CNSTI4 320
ARGI4
CNSTI4 448
ARGI4
ADDRLP4 4120
INDIRP4
ARGP4
CNSTI4 2065
ARGI4
ADDRGP4 menu_text_color
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 207
;204:		Info_ValueForKey( cstate.updateInfoString, "motd" ), UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
;205:	
;206:	// print any server info (server full, bad version, etc)
;207:	if ( cstate.connState < CA_CONNECTED ) {
ADDRLP4 0
INDIRI4
CNSTI4 5
GEI4 $130
line 208
;208:		UI_DrawProportionalString_AutoWrapped( 320, 192, 630, 20, cstate.messageString, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
CNSTI4 320
ARGI4
CNSTI4 192
ARGI4
CNSTI4 630
ARGI4
CNSTI4 20
ARGI4
ADDRLP4 0+2060
ARGP4
CNSTI4 2065
ARGI4
ADDRGP4 menu_text_color
ARGP4
ADDRGP4 UI_DrawProportionalString_AutoWrapped
CALLV
pop
line 209
;209:	}
LABELV $130
line 235
;210:
;211:#if 0
;212:	// display password field
;213:	if ( passwordNeeded ) {
;214:		s_ingame_menu.x = SCREEN_WIDTH * 0.50 - 128;
;215:		s_ingame_menu.nitems = 0;
;216:		s_ingame_menu.wrapAround = qtrue;
;217:
;218:		passwordField.generic.type = MTYPE_FIELD;
;219:		passwordField.generic.name = "Password:";
;220:		passwordField.generic.callback = 0;
;221:		passwordField.generic.x		= 10;
;222:		passwordField.generic.y		= 180;
;223:		Field_Clear( &passwordField.field );
;224:		passwordField.width = 256;
;225:		passwordField.field.widthInChars = 16;
;226:		Q_strncpyz( passwordField.field.buffer, Cvar_VariableString("password"), 
;227:			sizeof(passwordField.field.buffer) );
;228:
;229:		Menu_AddItem( &s_ingame_menu, ( void * ) &s_customize_player_action );
;230:
;231:		MField_Draw( &passwordField );
;232:	}
;233:#endif
;234:
;235:	if ( lastConnState > cstate.connState ) {
ADDRGP4 lastConnState
INDIRI4
ADDRLP4 0
INDIRI4
LEI4 $133
line 236
;236:		lastLoadingText[0] = '\0';
ADDRGP4 lastLoadingText
CNSTI1 0
ASGNI1
line 237
;237:	}
LABELV $133
line 238
;238:	lastConnState = cstate.connState;
ADDRGP4 lastConnState
ADDRLP4 0
INDIRI4
ASGNI4
line 240
;239:
;240:	switch ( cstate.connState ) {
ADDRLP4 4124
ADDRLP4 0
INDIRI4
ASGNI4
ADDRLP4 4124
INDIRI4
CNSTI4 3
LTI4 $118
ADDRLP4 4124
INDIRI4
CNSTI4 7
GTI4 $118
ADDRLP4 4124
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $151-12
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $151
address $138
address $141
address $144
address $118
address $118
code
LABELV $138
line 242
;241:	case CA_CONNECTING:
;242:		s = va("Awaiting challenge...%i", cstate.connectPacketCount);
ADDRGP4 $139
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 4132
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4132
INDIRP4
ASGNP4
line 243
;243:		break;
ADDRGP4 $136
JUMPV
LABELV $141
line 245
;244:	case CA_CHALLENGING:
;245:		s = va("Awaiting connection...%i", cstate.connectPacketCount);
ADDRGP4 $142
ARGP4
ADDRLP4 0+4
INDIRI4
ARGI4
ADDRLP4 4136
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4136
INDIRP4
ASGNP4
line 246
;246:		break;
ADDRGP4 $136
JUMPV
LABELV $144
line 247
;247:	case CA_CONNECTED: {
line 250
;248:		char downloadName[MAX_INFO_VALUE];
;249:
;250:			trap_Cvar_VariableStringBuffer( "cl_downloadName", downloadName, sizeof(downloadName) );
ADDRGP4 $145
ARGP4
ADDRLP4 4140
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 251
;251:			if (*downloadName) {
ADDRLP4 4140
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $146
line 252
;252:				UI_DisplayDownloadInfo( downloadName );
ADDRLP4 4140
ARGP4
ADDRGP4 UI_DisplayDownloadInfo
CALLV
pop
line 253
;253:				return;
ADDRGP4 $118
JUMPV
LABELV $146
line 255
;254:			}
;255:		}
line 256
;256:		s = "Awaiting gamestate...";
ADDRLP4 4108
ADDRGP4 $148
ASGNP4
line 257
;257:		break;
line 259
;258:	case CA_LOADING:
;259:		return;
line 261
;260:	case CA_PRIMED:
;261:		return;
line 263
;262:	default:
;263:		return;
LABELV $136
line 266
;264:	}
;265:
;266:	UI_DrawProportionalString( 320, 128, s, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
CNSTI4 320
ARGI4
CNSTI4 128
ARGI4
ADDRLP4 4108
INDIRP4
ARGP4
CNSTI4 2065
ARGI4
ADDRGP4 color_white
ARGP4
ADDRGP4 UI_DrawProportionalString
CALLV
pop
line 269
;267:
;268:	// password required / connection rejected information goes here
;269:}
LABELV $118
endproc UI_DrawConnectScreen 5164 28
export UI_KeyConnect
proc UI_KeyConnect 0 8
line 277
;270:
;271:
;272:/*
;273:===================
;274:UI_KeyConnect
;275:===================
;276:*/
;277:void UI_KeyConnect( int key ) {
line 278
;278:	if ( key == K_ESCAPE ) {
ADDRFP4 0
INDIRI4
CNSTI4 27
NEI4 $154
line 279
;279:		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
CNSTI4 2
ARGI4
ADDRGP4 $156
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 280
;280:		return;
LABELV $154
line 282
;281:	}
;282:}
LABELV $153
endproc UI_KeyConnect 0 8
bss
align 1
LABELV lastLoadingText
skip 1024
align 4
LABELV lastConnState
skip 4
export passwordField
align 4
LABELV passwordField
skip 332
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
import UI_CinematicsMenu_Cache
import UI_CinematicsMenu_f
import UI_CinematicsMenu
import Demos_Cache
import UI_DemosMenu
import Controls_Cache
import UI_ControlsMenu
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
LABELV $156
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
byte 1 10
byte 1 0
align 1
LABELV $148
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 116
byte 1 97
byte 1 116
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 0
align 1
LABELV $145
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 78
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $142
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 111
byte 1 110
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $139
byte 1 65
byte 1 119
byte 1 97
byte 1 105
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 103
byte 1 101
byte 1 46
byte 1 46
byte 1 46
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $129
byte 1 109
byte 1 111
byte 1 116
byte 1 100
byte 1 0
align 1
LABELV $126
byte 1 67
byte 1 111
byte 1 110
byte 1 110
byte 1 101
byte 1 99
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $125
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $124
byte 1 76
byte 1 111
byte 1 97
byte 1 100
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $117
byte 1 37
byte 1 115
byte 1 47
byte 1 83
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $114
byte 1 40
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 111
byte 1 112
byte 1 105
byte 1 101
byte 1 100
byte 1 41
byte 1 0
align 1
LABELV $105
byte 1 40
byte 1 37
byte 1 115
byte 1 32
byte 1 111
byte 1 102
byte 1 32
byte 1 37
byte 1 115
byte 1 32
byte 1 99
byte 1 111
byte 1 112
byte 1 105
byte 1 101
byte 1 100
byte 1 41
byte 1 0
align 1
LABELV $104
byte 1 101
byte 1 115
byte 1 116
byte 1 105
byte 1 109
byte 1 97
byte 1 116
byte 1 105
byte 1 110
byte 1 103
byte 1 0
align 1
LABELV $100
byte 1 37
byte 1 115
byte 1 32
byte 1 40
byte 1 37
byte 1 100
byte 1 37
byte 1 37
byte 1 41
byte 1 0
align 1
LABELV $93
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $92
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 67
byte 1 111
byte 1 117
byte 1 110
byte 1 116
byte 1 0
align 1
LABELV $91
byte 1 99
byte 1 108
byte 1 95
byte 1 100
byte 1 111
byte 1 119
byte 1 110
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 83
byte 1 105
byte 1 122
byte 1 101
byte 1 0
align 1
LABELV $86
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $85
byte 1 37
byte 1 100
byte 1 32
byte 1 109
byte 1 105
byte 1 110
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 115
byte 1 101
byte 1 99
byte 1 0
align 1
LABELV $82
byte 1 37
byte 1 100
byte 1 32
byte 1 104
byte 1 114
byte 1 32
byte 1 37
byte 1 100
byte 1 32
byte 1 109
byte 1 105
byte 1 110
byte 1 0
align 1
LABELV $78
byte 1 37
byte 1 100
byte 1 32
byte 1 98
byte 1 121
byte 1 116
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $77
byte 1 37
byte 1 100
byte 1 32
byte 1 75
byte 1 66
byte 1 0
align 1
LABELV $74
byte 1 46
byte 1 37
byte 1 48
byte 1 50
byte 1 100
byte 1 32
byte 1 77
byte 1 66
byte 1 0
align 1
LABELV $71
byte 1 46
byte 1 37
byte 1 48
byte 1 50
byte 1 100
byte 1 32
byte 1 71
byte 1 66
byte 1 0
align 1
LABELV $70
byte 1 37
byte 1 100
byte 1 0
