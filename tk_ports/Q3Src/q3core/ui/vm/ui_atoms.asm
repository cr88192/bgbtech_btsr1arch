export Com_Error
code
proc Com_Error 1032 12
file "../ui_atoms.c"
line 35
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
;23:/**********************************************************************
;24:	UI_ATOMS.C
;25:
;26:	User interface building blocks and support functions.
;27:**********************************************************************/
;28:#include "ui_local.h"
;29:
;30:qboolean		m_entersound;		// after a frame, so caching won't disrupt the sound
;31:
;32:// these are here so the functions in q_shared.c can link
;33:#ifndef UI_HARD_LINKED
;34:
;35:void QDECL Com_Error( int level, const char *error, ... ) {
line 39
;36:	va_list		argptr;
;37:	char		text[1024];
;38:
;39:	va_start (argptr, error);
ADDRLP4 0
ADDRFP4 4+4
ASGNP4
line 40
;40:	vsprintf (text, error, argptr);
ADDRLP4 4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 41
;41:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 43
;42:
;43:	trap_Error( va("%s", text) );
ADDRGP4 $89
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_Error
CALLV
pop
line 44
;44:}
LABELV $87
endproc Com_Error 1032 12
export Com_Printf
proc Com_Printf 1032 12
line 46
;45:
;46:void QDECL Com_Printf( const char *msg, ... ) {
line 50
;47:	va_list		argptr;
;48:	char		text[1024];
;49:
;50:	va_start (argptr, msg);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 51
;51:	vsprintf (text, msg, argptr);
ADDRLP4 4
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
line 52
;52:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 54
;53:
;54:	trap_Print( va("%s", text) );
ADDRGP4 $89
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 55
;55:}
LABELV $90
endproc Com_Printf 1032 12
data
export newUI
align 4
LABELV newUI
byte 4 0
export UI_ClampCvar
code
proc UI_ClampCvar 0 0
line 68
;56:
;57:#endif
;58:
;59:qboolean newUI = qfalse;
;60:
;61:
;62:/*
;63:=================
;64:UI_ClampCvar
;65:=================
;66:*/
;67:float UI_ClampCvar( float min, float max, float value )
;68:{
line 69
;69:	if ( value < min ) return min;
ADDRFP4 8
INDIRF4
ADDRFP4 0
INDIRF4
GEF4 $93
ADDRFP4 0
INDIRF4
RETF4
ADDRGP4 $92
JUMPV
LABELV $93
line 70
;70:	if ( value > max ) return max;
ADDRFP4 8
INDIRF4
ADDRFP4 4
INDIRF4
LEF4 $95
ADDRFP4 4
INDIRF4
RETF4
ADDRGP4 $92
JUMPV
LABELV $95
line 71
;71:	return value;
ADDRFP4 8
INDIRF4
RETF4
LABELV $92
endproc UI_ClampCvar 0 0
export UI_StartDemoLoop
proc UI_StartDemoLoop 0 8
line 79
;72:}
;73:
;74:/*
;75:=================
;76:UI_StartDemoLoop
;77:=================
;78:*/
;79:void UI_StartDemoLoop( void ) {
line 80
;80:	trap_Cmd_ExecuteText( EXEC_APPEND, "d1\n" );
CNSTI4 2
ARGI4
ADDRGP4 $98
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 81
;81:}
LABELV $97
endproc UI_StartDemoLoop 0 8
bss
align 1
LABELV $100
skip 1024
export UI_Argv
code
proc UI_Argv 0 12
line 100
;82:
;83:
;84:#ifndef MISSIONPACK // bk001206
;85:static void NeedCDAction( qboolean result ) {
;86:	if ( !result ) {
;87:		trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
;88:	}
;89:}
;90:#endif // MISSIONPACK
;91:
;92:#ifndef MISSIONPACK // bk001206
;93:static void NeedCDKeyAction( qboolean result ) {
;94:	if ( !result ) {
;95:		trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
;96:	}
;97:}
;98:#endif // MISSIONPACK
;99:
;100:char *UI_Argv( int arg ) {
line 103
;101:	static char	buffer[MAX_STRING_CHARS];
;102:
;103:	trap_Argv( arg, buffer, sizeof( buffer ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $100
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 105
;104:
;105:	return buffer;
ADDRGP4 $100
RETP4
LABELV $99
endproc UI_Argv 0 12
bss
align 1
LABELV $102
skip 1024
export UI_Cvar_VariableString
code
proc UI_Cvar_VariableString 0 12
line 109
;106:}
;107:
;108:
;109:char *UI_Cvar_VariableString( const char *var_name ) {
line 112
;110:	static char	buffer[MAX_STRING_CHARS];
;111:
;112:	trap_Cvar_VariableStringBuffer( var_name, buffer, sizeof( buffer ) );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $102
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 114
;113:
;114:	return buffer;
ADDRGP4 $102
RETP4
LABELV $101
endproc UI_Cvar_VariableString 0 12
export UI_SetBestScores
proc UI_SetBestScores 144 12
line 119
;115:}
;116:
;117:
;118:
;119:void UI_SetBestScores(postGameInfo_t *newInfo, qboolean postGame) {
line 120
;120:	trap_Cvar_Set("ui_scoreAccuracy",     va("%i%%", newInfo->accuracy));
ADDRGP4 $105
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $104
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 121
;121:	trap_Cvar_Set("ui_scoreImpressives",	va("%i", newInfo->impressives));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $106
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 122
;122:	trap_Cvar_Set("ui_scoreExcellents", 	va("%i", newInfo->excellents));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $108
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 123
;123:	trap_Cvar_Set("ui_scoreDefends", 			va("%i", newInfo->defends));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $109
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 124
;124:	trap_Cvar_Set("ui_scoreAssists", 			va("%i", newInfo->assists));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $110
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 125
;125:	trap_Cvar_Set("ui_scoreGauntlets", 		va("%i", newInfo->gauntlets));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $111
ARGP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 126
;126:	trap_Cvar_Set("ui_scoreScore", 				va("%i", newInfo->score));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $112
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 127
;127:	trap_Cvar_Set("ui_scorePerfect",	 		va("%i", newInfo->perfects));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $113
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 128
;128:	trap_Cvar_Set("ui_scoreTeam",					va("%i to %i", newInfo->redScore, newInfo->blueScore));
ADDRGP4 $115
ARGP4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 36
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $114
ARGP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 129
;129:	trap_Cvar_Set("ui_scoreBase",					va("%i", newInfo->baseScore));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 40
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $116
ARGP4
ADDRLP4 40
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 130
;130:	trap_Cvar_Set("ui_scoreTimeBonus",		va("%i", newInfo->timeBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
ARGI4
ADDRLP4 44
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $117
ARGP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 131
;131:	trap_Cvar_Set("ui_scoreSkillBonus",		va("%i", newInfo->skillBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $118
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 132
;132:	trap_Cvar_Set("ui_scoreShutoutBonus",	va("%i", newInfo->shutoutBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ARGI4
ADDRLP4 52
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $119
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 133
;133:	trap_Cvar_Set("ui_scoreTime",					va("%02i:%02i", newInfo->time / 60, newInfo->time % 60));
ADDRGP4 $121
ARGP4
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ASGNI4
ADDRLP4 60
CNSTI4 60
ASGNI4
ADDRLP4 56
INDIRI4
ADDRLP4 60
INDIRI4
DIVI4
ARGI4
ADDRLP4 56
INDIRI4
ADDRLP4 60
INDIRI4
MODI4
ARGI4
ADDRLP4 64
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $120
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 134
;134:	trap_Cvar_Set("ui_scoreCaptures",		va("%i", newInfo->captures));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 68
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $122
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 135
;135:  if (postGame) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $123
line 136
;136:		trap_Cvar_Set("ui_scoreAccuracy2",     va("%i%%", newInfo->accuracy));
ADDRGP4 $105
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
ARGI4
ADDRLP4 72
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $125
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 137
;137:		trap_Cvar_Set("ui_scoreImpressives2",	va("%i", newInfo->impressives));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $126
ARGP4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 138
;138:		trap_Cvar_Set("ui_scoreExcellents2", 	va("%i", newInfo->excellents));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRI4
ARGI4
ADDRLP4 80
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $127
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 139
;139:		trap_Cvar_Set("ui_scoreDefends2", 			va("%i", newInfo->defends));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRI4
ARGI4
ADDRLP4 84
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $128
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 140
;140:		trap_Cvar_Set("ui_scoreAssists2", 			va("%i", newInfo->assists));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 88
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $129
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 141
;141:		trap_Cvar_Set("ui_scoreGauntlets2", 		va("%i", newInfo->gauntlets));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 92
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $130
ARGP4
ADDRLP4 92
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 142
;142:		trap_Cvar_Set("ui_scoreScore2", 				va("%i", newInfo->score));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRLP4 96
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $131
ARGP4
ADDRLP4 96
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 143
;143:		trap_Cvar_Set("ui_scorePerfect2",	 		va("%i", newInfo->perfects));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ARGI4
ADDRLP4 100
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $132
ARGP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 144
;144:		trap_Cvar_Set("ui_scoreTeam2",					va("%i to %i", newInfo->redScore, newInfo->blueScore));
ADDRGP4 $115
ARGP4
ADDRLP4 104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 104
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRLP4 104
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ARGI4
ADDRLP4 108
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $133
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 145
;145:		trap_Cvar_Set("ui_scoreBase2",					va("%i", newInfo->baseScore));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
ARGI4
ADDRLP4 112
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $134
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 146
;146:		trap_Cvar_Set("ui_scoreTimeBonus2",		va("%i", newInfo->timeBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
ARGI4
ADDRLP4 116
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $135
ARGP4
ADDRLP4 116
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 147
;147:		trap_Cvar_Set("ui_scoreSkillBonus2",		va("%i", newInfo->skillBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
ARGI4
ADDRLP4 120
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $136
ARGP4
ADDRLP4 120
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 148
;148:		trap_Cvar_Set("ui_scoreShutoutBonus2",	va("%i", newInfo->shutoutBonus));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ARGI4
ADDRLP4 124
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $137
ARGP4
ADDRLP4 124
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 149
;149:		trap_Cvar_Set("ui_scoreTime2",					va("%02i:%02i", newInfo->time / 60, newInfo->time % 60));
ADDRGP4 $121
ARGP4
ADDRLP4 128
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
ASGNI4
ADDRLP4 132
CNSTI4 60
ASGNI4
ADDRLP4 128
INDIRI4
ADDRLP4 132
INDIRI4
DIVI4
ARGI4
ADDRLP4 128
INDIRI4
ADDRLP4 132
INDIRI4
MODI4
ARGI4
ADDRLP4 136
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $138
ARGP4
ADDRLP4 136
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 150
;150:		trap_Cvar_Set("ui_scoreCaptures2",		va("%i", newInfo->captures));
ADDRGP4 $107
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 140
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $139
ARGP4
ADDRLP4 140
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 151
;151:	}
LABELV $123
line 152
;152:}
LABELV $103
endproc UI_SetBestScores 144 12
export UI_LoadBestScores
proc UI_LoadBestScores 144 24
line 154
;153:
;154:void UI_LoadBestScores(const char *map, int game) {
line 158
;155:	char		fileName[MAX_QPATH];
;156:	fileHandle_t f;
;157:	postGameInfo_t newInfo;
;158:	memset(&newInfo, 0, sizeof(postGameInfo_t));
ADDRLP4 68
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 159
;159:	Com_sprintf(fileName, MAX_QPATH, "games/%s_%i.game", map, game);
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $141
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 160
;160:	if (trap_FS_FOpenFile(fileName, &f, FS_READ) >= 0) {
ADDRLP4 0
ARGP4
ADDRLP4 64
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 132
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 0
LTI4 $142
line 161
;161:		int size = 0;
ADDRLP4 136
CNSTI4 0
ASGNI4
line 162
;162:		trap_FS_Read(&size, sizeof(int), f);
ADDRLP4 136
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 163
;163:		if (size == sizeof(postGameInfo_t)) {
ADDRLP4 136
INDIRI4
CVIU4 4
CNSTU4 64
NEU4 $144
line 164
;164:			trap_FS_Read(&newInfo, sizeof(postGameInfo_t), f);
ADDRLP4 68
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 165
;165:		}
LABELV $144
line 166
;166:		trap_FS_FCloseFile(f);
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 167
;167:	}
LABELV $142
line 168
;168:	UI_SetBestScores(&newInfo, qfalse);
ADDRLP4 68
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_SetBestScores
CALLV
pop
line 170
;169:
;170:	Com_sprintf(fileName, MAX_QPATH, "demos/%s_%d.dm_%d", map, game, (int)trap_Cvar_VariableValue("protocol"));
ADDRGP4 $147
ARGP4
ADDRLP4 136
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $146
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 136
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 171
;171:	uiInfo.demoAvailable = qfalse;
ADDRGP4 uiInfo+73432
CNSTI4 0
ASGNI4
line 172
;172:	if (trap_FS_FOpenFile(fileName, &f, FS_READ) >= 0) {
ADDRLP4 0
ARGP4
ADDRLP4 64
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 140
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 0
LTI4 $149
line 173
;173:		uiInfo.demoAvailable = qtrue;
ADDRGP4 uiInfo+73432
CNSTI4 1
ASGNI4
line 174
;174:		trap_FS_FCloseFile(f);
ADDRLP4 64
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 175
;175:	} 
LABELV $149
line 176
;176:}
LABELV $140
endproc UI_LoadBestScores 144 24
export UI_ClearScores
proc UI_ClearScores 4200 16
line 183
;177:
;178:/*
;179:===============
;180:UI_ClearScores
;181:===============
;182:*/
;183:void UI_ClearScores() {
line 190
;184:	char	gameList[4096];
;185:	char *gameFile;
;186:	int		i, len, count, size;
;187:	fileHandle_t f;
;188:	postGameInfo_t newInfo;
;189:
;190:	count = trap_FS_GetFileList( "games", "game", gameList, sizeof(gameList) );
ADDRGP4 $153
ARGP4
ADDRGP4 $154
ARGP4
ADDRLP4 88
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4184
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 4184
INDIRI4
ASGNI4
line 192
;191:
;192:	size = sizeof(postGameInfo_t);
ADDRLP4 20
CNSTI4 64
ASGNI4
line 193
;193:	memset(&newInfo, 0, size);
ADDRLP4 24
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRGP4 memset
CALLP4
pop
line 195
;194:
;195:	if (count > 0) {
ADDRLP4 16
INDIRI4
CNSTI4 0
LEI4 $155
line 196
;196:		gameFile = gameList;
ADDRLP4 0
ADDRLP4 88
ASGNP4
line 197
;197:		for ( i = 0; i < count; i++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $160
JUMPV
LABELV $157
line 198
;198:			len = strlen(gameFile);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4188
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 4188
INDIRI4
ASGNI4
line 199
;199:			if (trap_FS_FOpenFile(va("games/%s",gameFile), &f, FS_WRITE) >= 0) {
ADDRGP4 $163
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4192
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4192
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 4196
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 4196
INDIRI4
CNSTI4 0
LTI4 $161
line 200
;200:				trap_FS_Write(&size, sizeof(int), f);
ADDRLP4 20
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 201
;201:				trap_FS_Write(&newInfo, size, f);
ADDRLP4 24
ARGP4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 202
;202:				trap_FS_FCloseFile(f);
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 203
;203:			}
LABELV $161
line 204
;204:			gameFile += len + 1;
ADDRLP4 0
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
line 205
;205:		}
LABELV $158
line 197
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $160
ADDRLP4 8
INDIRI4
ADDRLP4 16
INDIRI4
LTI4 $157
line 206
;206:	}
LABELV $155
line 208
;207:	
;208:	UI_SetBestScores(&newInfo, qfalse);
ADDRLP4 24
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 UI_SetBestScores
CALLV
pop
line 210
;209:
;210:}
LABELV $152
endproc UI_ClearScores 4200 16
proc UI_Cache_f 0 0
line 214
;211:
;212:
;213:
;214:static void	UI_Cache_f() {
line 215
;215:	Display_CacheAll();
ADDRGP4 Display_CacheAll
CALLV
pop
line 216
;216:}
LABELV $164
endproc UI_Cache_f 0 0
proc UI_CalcPostGameStats 1456 20
line 223
;217:
;218:/*
;219:=======================
;220:UI_CalcPostGameStats
;221:=======================
;222:*/
;223:static void UI_CalcPostGameStats() {
line 231
;224:	char		map[MAX_QPATH];
;225:	char		fileName[MAX_QPATH];
;226:	char		info[MAX_INFO_STRING];
;227:	fileHandle_t f;
;228:	int size, game, time, adjustedTime;
;229:	postGameInfo_t oldInfo;
;230:	postGameInfo_t newInfo;
;231:	qboolean newHigh = qfalse;
ADDRLP4 1088
CNSTI4 0
ASGNI4
line 233
;232:
;233:	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
CNSTI4 0
ARGI4
ADDRLP4 64
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 234
;234:	Q_strncpyz( map, Info_ValueForKey( info, "mapname" ), sizeof(map) );
ADDRLP4 64
ARGP4
ADDRGP4 $166
ARGP4
ADDRLP4 1304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1160
ARGP4
ADDRLP4 1304
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 235
;235:	game = atoi(Info_ValueForKey(info, "g_gametype"));
ADDRLP4 64
ARGP4
ADDRGP4 $167
ARGP4
ADDRLP4 1308
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1308
INDIRP4
ARGP4
ADDRLP4 1312
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1224
ADDRLP4 1312
INDIRI4
ASGNI4
line 238
;236:
;237:	// compose file name
;238:	Com_sprintf(fileName, MAX_QPATH, "games/%s_%i.game", map, game);
ADDRLP4 1228
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $141
ARGP4
ADDRLP4 1160
ARGP4
ADDRLP4 1224
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 240
;239:	// see if we have one already
;240:	memset(&oldInfo, 0, sizeof(postGameInfo_t));
ADDRLP4 1096
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 241
;241:	if (trap_FS_FOpenFile(fileName, &f, FS_READ) >= 0) {
ADDRLP4 1228
ARGP4
ADDRLP4 1092
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 1316
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 1316
INDIRI4
CNSTI4 0
LTI4 $168
line 243
;242:	// if so load it
;243:		size = 0;
ADDRLP4 1296
CNSTI4 0
ASGNI4
line 244
;244:		trap_FS_Read(&size, sizeof(int), f);
ADDRLP4 1296
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 245
;245:		if (size == sizeof(postGameInfo_t)) {
ADDRLP4 1296
INDIRI4
CVIU4 4
CNSTU4 64
NEU4 $170
line 246
;246:			trap_FS_Read(&oldInfo, sizeof(postGameInfo_t), f);
ADDRLP4 1096
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 247
;247:		}
LABELV $170
line 248
;248:		trap_FS_FCloseFile(f);
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 249
;249:	}					 
LABELV $168
line 251
;250:
;251:	newInfo.accuracy = atoi(UI_Argv(3));
CNSTI4 3
ARGI4
ADDRLP4 1320
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1320
INDIRP4
ARGP4
ADDRLP4 1324
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+16
ADDRLP4 1324
INDIRI4
ASGNI4
line 252
;252:	newInfo.impressives = atoi(UI_Argv(4));
CNSTI4 4
ARGI4
ADDRLP4 1328
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1328
INDIRP4
ARGP4
ADDRLP4 1332
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+20
ADDRLP4 1332
INDIRI4
ASGNI4
line 253
;253:	newInfo.excellents = atoi(UI_Argv(5));
CNSTI4 5
ARGI4
ADDRLP4 1336
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1336
INDIRP4
ARGP4
ADDRLP4 1340
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+24
ADDRLP4 1340
INDIRI4
ASGNI4
line 254
;254:	newInfo.defends = atoi(UI_Argv(6));
CNSTI4 6
ARGI4
ADDRLP4 1344
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1344
INDIRP4
ARGP4
ADDRLP4 1348
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+28
ADDRLP4 1348
INDIRI4
ASGNI4
line 255
;255:	newInfo.assists = atoi(UI_Argv(7));
CNSTI4 7
ARGI4
ADDRLP4 1352
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1352
INDIRP4
ARGP4
ADDRLP4 1356
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+32
ADDRLP4 1356
INDIRI4
ASGNI4
line 256
;256:	newInfo.gauntlets = atoi(UI_Argv(8));
CNSTI4 8
ARGI4
ADDRLP4 1360
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1360
INDIRP4
ARGP4
ADDRLP4 1364
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+36
ADDRLP4 1364
INDIRI4
ASGNI4
line 257
;257:	newInfo.baseScore = atoi(UI_Argv(9));
CNSTI4 9
ARGI4
ADDRLP4 1368
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1368
INDIRP4
ARGP4
ADDRLP4 1372
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+60
ADDRLP4 1372
INDIRI4
ASGNI4
line 258
;258:	newInfo.perfects = atoi(UI_Argv(10));
CNSTI4 10
ARGI4
ADDRLP4 1376
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1376
INDIRP4
ARGP4
ADDRLP4 1380
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+12
ADDRLP4 1380
INDIRI4
ASGNI4
line 259
;259:	newInfo.redScore = atoi(UI_Argv(11));
CNSTI4 11
ARGI4
ADDRLP4 1384
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1384
INDIRP4
ARGP4
ADDRLP4 1388
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 1388
INDIRI4
ASGNI4
line 260
;260:	newInfo.blueScore = atoi(UI_Argv(12));
CNSTI4 12
ARGI4
ADDRLP4 1392
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1392
INDIRP4
ARGP4
ADDRLP4 1396
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+8
ADDRLP4 1396
INDIRI4
ASGNI4
line 261
;261:	time = atoi(UI_Argv(13));
CNSTI4 13
ARGI4
ADDRLP4 1400
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1400
INDIRP4
ARGP4
ADDRLP4 1404
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1300
ADDRLP4 1404
INDIRI4
ASGNI4
line 262
;262:	newInfo.captures = atoi(UI_Argv(14));
CNSTI4 14
ARGI4
ADDRLP4 1408
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 1408
INDIRP4
ARGP4
ADDRLP4 1412
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0+40
ADDRLP4 1412
INDIRI4
ASGNI4
line 264
;263:
;264:	newInfo.time = (time - trap_Cvar_VariableValue("ui_matchStartTime")) / 1000;
ADDRGP4 $184
ARGP4
ADDRLP4 1416
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0+44
ADDRLP4 1300
INDIRI4
CVIF4 4
ADDRLP4 1416
INDIRF4
SUBF4
CNSTF4 1148846080
DIVF4
CVFI4 4
ASGNI4
line 265
;265:	adjustedTime = uiInfo.mapList[ui_currentMap.integer].timeToBeat[game];
ADDRLP4 1292
ADDRLP4 1224
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 100
ADDRGP4 ui_currentMap+12
INDIRI4
MULI4
ADDRGP4 uiInfo+83228+28
ADDP4
ADDP4
INDIRI4
ASGNI4
line 266
;266:	if (newInfo.time < adjustedTime) { 
ADDRLP4 0+44
INDIRI4
ADDRLP4 1292
INDIRI4
GEI4 $188
line 267
;267:		newInfo.timeBonus = (adjustedTime - newInfo.time) * 10;
ADDRLP4 0+48
CNSTI4 10
ADDRLP4 1292
INDIRI4
ADDRLP4 0+44
INDIRI4
SUBI4
MULI4
ASGNI4
line 268
;268:	} else {
ADDRGP4 $189
JUMPV
LABELV $188
line 269
;269:		newInfo.timeBonus = 0;
ADDRLP4 0+48
CNSTI4 0
ASGNI4
line 270
;270:	}
LABELV $189
line 272
;271:
;272:	if (newInfo.redScore > newInfo.blueScore && newInfo.blueScore <= 0) {
ADDRLP4 0+4
INDIRI4
ADDRLP4 0+8
INDIRI4
LEI4 $194
ADDRLP4 0+8
INDIRI4
CNSTI4 0
GTI4 $194
line 273
;273:		newInfo.shutoutBonus = 100;
ADDRLP4 0+52
CNSTI4 100
ASGNI4
line 274
;274:	} else {
ADDRGP4 $195
JUMPV
LABELV $194
line 275
;275:		newInfo.shutoutBonus = 0;
ADDRLP4 0+52
CNSTI4 0
ASGNI4
line 276
;276:	}
LABELV $195
line 278
;277:
;278:	newInfo.skillBonus = trap_Cvar_VariableValue("g_spSkill");
ADDRGP4 $202
ARGP4
ADDRLP4 1420
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 0+56
ADDRLP4 1420
INDIRF4
CVFI4 4
ASGNI4
line 279
;279:	if (newInfo.skillBonus <= 0) {
ADDRLP4 0+56
INDIRI4
CNSTI4 0
GTI4 $203
line 280
;280:		newInfo.skillBonus = 1;
ADDRLP4 0+56
CNSTI4 1
ASGNI4
line 281
;281:	}
LABELV $203
line 282
;282:	newInfo.score = newInfo.baseScore + newInfo.shutoutBonus + newInfo.timeBonus;
ADDRLP4 0
ADDRLP4 0+60
INDIRI4
ADDRLP4 0+52
INDIRI4
ADDI4
ADDRLP4 0+48
INDIRI4
ADDI4
ASGNI4
line 283
;283:	newInfo.score *= newInfo.skillBonus;
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 0+56
INDIRI4
MULI4
ASGNI4
line 286
;284:
;285:	// see if the score is higher for this one
;286:	newHigh = (newInfo.redScore > newInfo.blueScore && newInfo.score > oldInfo.score);
ADDRLP4 0+4
INDIRI4
ADDRLP4 0+8
INDIRI4
LEI4 $214
ADDRLP4 0
INDIRI4
ADDRLP4 1096
INDIRI4
LEI4 $214
ADDRLP4 1424
CNSTI4 1
ASGNI4
ADDRGP4 $215
JUMPV
LABELV $214
ADDRLP4 1424
CNSTI4 0
ASGNI4
LABELV $215
ADDRLP4 1088
ADDRLP4 1424
INDIRI4
ASGNI4
line 288
;287:
;288:	if  (newHigh) {
ADDRLP4 1088
INDIRI4
CNSTI4 0
EQI4 $216
line 290
;289:		// if so write out the new one
;290:		uiInfo.newHighScoreTime = uiInfo.uiDC.realTime + 20000;
ADDRGP4 uiInfo+73416
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 20000
ADDI4
ASGNI4
line 291
;291:		if (trap_FS_FOpenFile(fileName, &f, FS_WRITE) >= 0) {
ADDRLP4 1228
ARGP4
ADDRLP4 1092
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 1428
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 1428
INDIRI4
CNSTI4 0
LTI4 $220
line 292
;292:			size = sizeof(postGameInfo_t);
ADDRLP4 1296
CNSTI4 64
ASGNI4
line 293
;293:			trap_FS_Write(&size, sizeof(int), f);
ADDRLP4 1296
ARGP4
CNSTI4 4
ARGI4
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 294
;294:			trap_FS_Write(&newInfo, sizeof(postGameInfo_t), f);
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 295
;295:			trap_FS_FCloseFile(f);
ADDRLP4 1092
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 296
;296:		}
LABELV $220
line 297
;297:	}
LABELV $216
line 299
;298:
;299:	if (newInfo.time < oldInfo.time) {
ADDRLP4 0+44
INDIRI4
ADDRLP4 1096+44
INDIRI4
GEI4 $222
line 300
;300:		uiInfo.newBestTime = uiInfo.uiDC.realTime + 20000;
ADDRGP4 uiInfo+73420
ADDRGP4 uiInfo+208
INDIRI4
CNSTI4 20000
ADDI4
ASGNI4
line 301
;301:	}
LABELV $222
line 304
;302: 
;303:	// put back all the ui overrides
;304:	trap_Cvar_Set("capturelimit", UI_Cvar_VariableString("ui_saveCaptureLimit"));
ADDRGP4 $229
ARGP4
ADDRLP4 1428
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $228
ARGP4
ADDRLP4 1428
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 305
;305:	trap_Cvar_Set("fraglimit", UI_Cvar_VariableString("ui_saveFragLimit"));
ADDRGP4 $231
ARGP4
ADDRLP4 1432
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $230
ARGP4
ADDRLP4 1432
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 306
;306:	trap_Cvar_Set("cg_drawTimer", UI_Cvar_VariableString("ui_drawTimer"));
ADDRGP4 $233
ARGP4
ADDRLP4 1436
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $232
ARGP4
ADDRLP4 1436
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 307
;307:	trap_Cvar_Set("g_doWarmup", UI_Cvar_VariableString("ui_doWarmup"));
ADDRGP4 $235
ARGP4
ADDRLP4 1440
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $234
ARGP4
ADDRLP4 1440
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 308
;308:	trap_Cvar_Set("g_Warmup", UI_Cvar_VariableString("ui_Warmup"));
ADDRGP4 $237
ARGP4
ADDRLP4 1444
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $236
ARGP4
ADDRLP4 1444
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 309
;309:	trap_Cvar_Set("sv_pure", UI_Cvar_VariableString("ui_pure"));
ADDRGP4 $239
ARGP4
ADDRLP4 1448
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $238
ARGP4
ADDRLP4 1448
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 310
;310:	trap_Cvar_Set("g_friendlyFire", UI_Cvar_VariableString("ui_friendlyFire"));
ADDRGP4 $241
ARGP4
ADDRLP4 1452
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRGP4 $240
ARGP4
ADDRLP4 1452
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 312
;311:
;312:	UI_SetBestScores(&newInfo, qtrue);
ADDRLP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 UI_SetBestScores
CALLV
pop
line 313
;313:	UI_ShowPostGame(newHigh);
ADDRLP4 1088
INDIRI4
ARGI4
ADDRGP4 UI_ShowPostGame
CALLV
pop
line 316
;314:
;315:
;316:}
LABELV $165
endproc UI_CalcPostGameStats 1456 20
export UI_ConsoleCommand
proc UI_ConsoleCommand 168 12
line 324
;317:
;318:
;319:/*
;320:=================
;321:UI_ConsoleCommand
;322:=================
;323:*/
;324:qboolean UI_ConsoleCommand( int realTime ) {
line 327
;325:	char	*cmd;
;326:
;327:	uiInfo.uiDC.frameTime = realTime - uiInfo.uiDC.realTime;
ADDRGP4 uiInfo+212
ADDRFP4 0
INDIRI4
ADDRGP4 uiInfo+208
INDIRI4
SUBI4
ASGNI4
line 328
;328:	uiInfo.uiDC.realTime = realTime;
ADDRGP4 uiInfo+208
ADDRFP4 0
INDIRI4
ASGNI4
line 330
;329:
;330:	cmd = UI_Argv( 0 );
CNSTI4 0
ARGI4
ADDRLP4 4
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 335
;331:
;332:	// ensure minimum menu data is available
;333:	//Menu_Cache();
;334:
;335:	if ( Q_stricmp (cmd, "ui_test") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $248
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $246
line 336
;336:		UI_ShowPostGame(qtrue);
CNSTI4 1
ARGI4
ADDRGP4 UI_ShowPostGame
CALLV
pop
line 337
;337:	}
LABELV $246
line 339
;338:
;339:	if ( Q_stricmp (cmd, "ui_report") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $251
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $249
line 340
;340:		UI_Report();
ADDRGP4 UI_Report
CALLV
pop
line 341
;341:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $249
line 344
;342:	}
;343:	
;344:	if ( Q_stricmp (cmd, "ui_load") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $254
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $252
line 345
;345:		UI_Load();
ADDRGP4 UI_Load
CALLV
pop
line 346
;346:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $252
line 349
;347:	}
;348:
;349:	if ( Q_stricmp (cmd, "remapShader") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $257
ARGP4
ADDRLP4 20
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
NEI4 $255
line 350
;350:		if (trap_Argc() == 4) {
ADDRLP4 24
ADDRGP4 trap_Argc
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 4
NEI4 $258
line 353
;351:			char shader1[MAX_QPATH];
;352:			char shader2[MAX_QPATH];
;353:			Q_strncpyz(shader1, UI_Argv(1), sizeof(shader1));
CNSTI4 1
ARGI4
ADDRLP4 156
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 28
ARGP4
ADDRLP4 156
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 354
;354:			Q_strncpyz(shader2, UI_Argv(2), sizeof(shader2));
CNSTI4 2
ARGI4
ADDRLP4 160
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 92
ARGP4
ADDRLP4 160
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 355
;355:			trap_R_RemapShader(shader1, shader2, UI_Argv(3));
CNSTI4 3
ARGI4
ADDRLP4 164
ADDRGP4 UI_Argv
CALLP4
ASGNP4
ADDRLP4 28
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 164
INDIRP4
ARGP4
ADDRGP4 trap_R_RemapShader
CALLV
pop
line 356
;356:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $258
line 358
;357:		}
;358:	}
LABELV $255
line 360
;359:
;360:	if ( Q_stricmp (cmd, "postgame") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $262
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $260
line 361
;361:		UI_CalcPostGameStats();
ADDRGP4 UI_CalcPostGameStats
CALLV
pop
line 362
;362:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $260
line 365
;363:	}
;364:
;365:	if ( Q_stricmp (cmd, "ui_cache") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $265
ARGP4
ADDRLP4 28
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $263
line 366
;366:		UI_Cache_f();
ADDRGP4 UI_Cache_f
CALLV
pop
line 367
;367:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $263
line 370
;368:	}
;369:
;370:	if ( Q_stricmp (cmd, "ui_teamOrders") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 32
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $266
line 372
;371:		//UI_TeamOrdersMenu_f();
;372:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $266
line 376
;373:	}
;374:
;375:
;376:	if ( Q_stricmp (cmd, "ui_cdkey") == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $271
ARGP4
ADDRLP4 36
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $269
line 378
;377:		//UI_CDKeyMenu_f();
;378:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $242
JUMPV
LABELV $269
line 381
;379:	}
;380:
;381:	return qfalse;
CNSTI4 0
RETI4
LABELV $242
endproc UI_ConsoleCommand 168 12
export UI_Shutdown
proc UI_Shutdown 0 0
line 389
;382:}
;383:
;384:/*
;385:=================
;386:UI_Shutdown
;387:=================
;388:*/
;389:void UI_Shutdown( void ) {
line 390
;390:}
LABELV $272
endproc UI_Shutdown 0 0
export UI_AdjustFrom640
proc UI_AdjustFrom640 16 0
line 399
;391:
;392:/*
;393:================
;394:UI_AdjustFrom640
;395:
;396:Adjusted for resolution and screen aspect ratio
;397:================
;398:*/
;399:void UI_AdjustFrom640( float *x, float *y, float *w, float *h ) {
line 408
;400:	// expect valid pointers
;401:#if 0
;402:	*x = *x * uiInfo.uiDC.scale + uiInfo.uiDC.bias;
;403:	*y *= uiInfo.uiDC.scale;
;404:	*w *= uiInfo.uiDC.scale;
;405:	*h *= uiInfo.uiDC.scale;
;406:#endif
;407:
;408:	*x *= uiInfo.uiDC.xscale;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRGP4 uiInfo+200
INDIRF4
MULF4
ASGNF4
line 409
;409:	*y *= uiInfo.uiDC.yscale;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
ADDRGP4 uiInfo+196
INDIRF4
MULF4
ASGNF4
line 410
;410:	*w *= uiInfo.uiDC.xscale;
ADDRLP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRGP4 uiInfo+200
INDIRF4
MULF4
ASGNF4
line 411
;411:	*h *= uiInfo.uiDC.yscale;
ADDRLP4 12
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
ADDRGP4 uiInfo+196
INDIRF4
MULF4
ASGNF4
line 413
;412:
;413:}
LABELV $273
endproc UI_AdjustFrom640 16 0
export UI_DrawNamedPic
proc UI_DrawNamedPic 16 36
line 415
;414:
;415:void UI_DrawNamedPic( float x, float y, float width, float height, const char *picname ) {
line 418
;416:	qhandle_t	hShader;
;417:
;418:	hShader = trap_R_RegisterShaderNoMip( picname );
ADDRFP4 16
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 419
;419:	UI_AdjustFrom640( &x, &y, &width, &height );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 420
;420:	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 421
;421:}
LABELV $278
endproc UI_DrawNamedPic 16 36
export UI_DrawHandlePic
proc UI_DrawHandlePic 16 36
line 423
;422:
;423:void UI_DrawHandlePic( float x, float y, float w, float h, qhandle_t hShader ) {
line 429
;424:	float	s0;
;425:	float	s1;
;426:	float	t0;
;427:	float	t1;
;428:
;429:	if( w < 0 ) {	// flip about vertical
ADDRFP4 8
INDIRF4
CNSTF4 0
GEF4 $280
line 430
;430:		w  = -w;
ADDRFP4 8
ADDRFP4 8
INDIRF4
NEGF4
ASGNF4
line 431
;431:		s0 = 1;
ADDRLP4 0
CNSTF4 1065353216
ASGNF4
line 432
;432:		s1 = 0;
ADDRLP4 4
CNSTF4 0
ASGNF4
line 433
;433:	}
ADDRGP4 $281
JUMPV
LABELV $280
line 434
;434:	else {
line 435
;435:		s0 = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 436
;436:		s1 = 1;
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 437
;437:	}
LABELV $281
line 439
;438:
;439:	if( h < 0 ) {	// flip about horizontal
ADDRFP4 12
INDIRF4
CNSTF4 0
GEF4 $282
line 440
;440:		h  = -h;
ADDRFP4 12
ADDRFP4 12
INDIRF4
NEGF4
ASGNF4
line 441
;441:		t0 = 1;
ADDRLP4 8
CNSTF4 1065353216
ASGNF4
line 442
;442:		t1 = 0;
ADDRLP4 12
CNSTF4 0
ASGNF4
line 443
;443:	}
ADDRGP4 $283
JUMPV
LABELV $282
line 444
;444:	else {
line 445
;445:		t0 = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 446
;446:		t1 = 1;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 447
;447:	}
LABELV $283
line 449
;448:	
;449:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 450
;450:	trap_R_DrawStretchPic( x, y, w, h, s0, t0, s1, t1, hShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 12
INDIRF4
ARGF4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 451
;451:}
LABELV $279
endproc UI_DrawHandlePic 16 36
export UI_FillRect
proc UI_FillRect 4 36
line 460
;452:
;453:/*
;454:================
;455:UI_FillRect
;456:
;457:Coordinates are 640*480 virtual values
;458:=================
;459:*/
;460:void UI_FillRect( float x, float y, float width, float height, const float *color ) {
line 461
;461:	trap_R_SetColor( color );
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 463
;462:
;463:	UI_AdjustFrom640( &x, &y, &width, &height );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 464
;464:	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 466
;465:
;466:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 467
;467:}
LABELV $284
endproc UI_FillRect 4 36
export UI_DrawSides
proc UI_DrawSides 12 36
line 469
;468:
;469:void UI_DrawSides(float x, float y, float w, float h) {
line 470
;470:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 471
;471:	trap_R_DrawStretchPic( x, y, 1, h, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 472
;472:	trap_R_DrawStretchPic( x + w - 1, y, 1, h, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ADDRLP4 4
INDIRF4
SUBF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 473
;473:}
LABELV $286
endproc UI_DrawSides 12 36
export UI_DrawTopBottom
proc UI_DrawTopBottom 12 36
line 475
;474:
;475:void UI_DrawTopBottom(float x, float y, float w, float h) {
line 476
;476:	UI_AdjustFrom640( &x, &y, &w, &h );
ADDRFP4 0
ARGP4
ADDRFP4 4
ARGP4
ADDRFP4 8
ARGP4
ADDRFP4 12
ARGP4
ADDRGP4 UI_AdjustFrom640
CALLV
pop
line 477
;477:	trap_R_DrawStretchPic( x, y, w, 1, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
CNSTF4 1065353216
ARGF4
ADDRLP4 0
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 0
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 478
;478:	trap_R_DrawStretchPic( x, y + h - 1, w, 1, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );
ADDRFP4 0
INDIRF4
ARGF4
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
ADDRFP4 4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ADDRLP4 4
INDIRF4
SUBF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRLP4 4
INDIRF4
ARGF4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 8
INDIRF4
ARGF4
ADDRGP4 uiInfo+73400
INDIRI4
ARGI4
ADDRGP4 trap_R_DrawStretchPic
CALLV
pop
line 479
;479:}
LABELV $289
endproc UI_DrawTopBottom 12 36
export UI_DrawRect
proc UI_DrawRect 0 16
line 487
;480:/*
;481:================
;482:UI_DrawRect
;483:
;484:Coordinates are 640*480 virtual values
;485:=================
;486:*/
;487:void UI_DrawRect( float x, float y, float width, float height, const float *color ) {
line 488
;488:	trap_R_SetColor( color );
ADDRFP4 16
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 490
;489:
;490:  UI_DrawTopBottom(x, y, width, height);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 UI_DrawTopBottom
CALLV
pop
line 491
;491:  UI_DrawSides(x, y, width, height);
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 4
INDIRF4
ARGF4
ADDRFP4 8
INDIRF4
ARGF4
ADDRFP4 12
INDIRF4
ARGF4
ADDRGP4 UI_DrawSides
CALLV
pop
line 493
;492:
;493:	trap_R_SetColor( NULL );
CNSTP4 0
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 494
;494:}
LABELV $292
endproc UI_DrawRect 0 16
export UI_SetColor
proc UI_SetColor 0 4
line 496
;495:
;496:void UI_SetColor( const float *rgba ) {
line 497
;497:	trap_R_SetColor( rgba );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_R_SetColor
CALLV
pop
line 498
;498:}
LABELV $293
endproc UI_SetColor 0 4
export UI_UpdateScreen
proc UI_UpdateScreen 0 0
line 500
;499:
;500:void UI_UpdateScreen( void ) {
line 501
;501:	trap_UpdateScreen();
ADDRGP4 trap_UpdateScreen
CALLV
pop
line 502
;502:}
LABELV $294
endproc UI_UpdateScreen 0 0
export UI_DrawTextBox
proc UI_DrawTextBox 24 20
line 506
;503:
;504:
;505:void UI_DrawTextBox (int x, int y, int width, int lines)
;506:{
line 507
;507:	UI_FillRect( x + BIGCHAR_WIDTH/2, y + BIGCHAR_HEIGHT/2, ( width + 1 ) * BIGCHAR_WIDTH, ( lines + 1 ) * BIGCHAR_HEIGHT, colorBlack );
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 4
CNSTI4 4
ASGNI4
ADDRLP4 8
CNSTI4 16
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRLP4 8
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 12
INDIRI4
ADDRLP4 4
INDIRI4
LSHI4
ADDRLP4 8
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 colorBlack
ARGP4
ADDRGP4 UI_FillRect
CALLV
pop
line 508
;508:	UI_DrawRect( x + BIGCHAR_WIDTH/2, y + BIGCHAR_HEIGHT/2, ( width + 1 ) * BIGCHAR_WIDTH, ( lines + 1 ) * BIGCHAR_HEIGHT, colorWhite );
ADDRLP4 12
CNSTI4 8
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRLP4 16
CNSTI4 4
ASGNI4
ADDRLP4 20
CNSTI4 16
ASGNI4
ADDRFP4 8
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRLP4 20
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRFP4 12
INDIRI4
ADDRLP4 16
INDIRI4
LSHI4
ADDRLP4 20
INDIRI4
ADDI4
CVIF4 4
ARGF4
ADDRGP4 colorWhite
ARGP4
ADDRGP4 UI_DrawRect
CALLV
pop
line 509
;509:}
LABELV $295
endproc UI_DrawTextBox 24 20
export UI_CursorInRect
proc UI_CursorInRect 8 0
line 512
;510:
;511:qboolean UI_CursorInRect (int x, int y, int width, int height)
;512:{
line 513
;513:	if (uiInfo.uiDC.cursorx < x ||
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRGP4 uiInfo+216
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $305
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 uiInfo+220
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $305
ADDRGP4 uiInfo+216
INDIRI4
ADDRLP4 0
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
GTI4 $305
ADDRGP4 uiInfo+220
INDIRI4
ADDRLP4 4
INDIRI4
ADDRFP4 12
INDIRI4
ADDI4
LEI4 $297
LABELV $305
line 517
;514:		uiInfo.uiDC.cursory < y ||
;515:		uiInfo.uiDC.cursorx > x+width ||
;516:		uiInfo.uiDC.cursory > y+height)
;517:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $296
JUMPV
LABELV $297
line 519
;518:
;519:	return qtrue;
CNSTI4 1
RETI4
LABELV $296
endproc UI_CursorInRect 8 0
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
import UI_GetBotNameByNumber
import UI_LoadBots
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
bss
export m_entersound
align 4
LABELV m_entersound
skip 4
import UI_ForceMenuOff
import UI_PopMenu
import UI_PushMenu
import UI_SetActiveMenu
import UI_IsFullscreen
import UI_DrawChar
import UI_DrawString
import UI_ProportionalStringWidth
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_Refresh
import UI_MouseEvent
import UI_KeyEvent
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
import UI_LoadArenas
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
LABELV $271
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 100
byte 1 107
byte 1 101
byte 1 121
byte 1 0
align 1
LABELV $268
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 79
byte 1 114
byte 1 100
byte 1 101
byte 1 114
byte 1 115
byte 1 0
align 1
LABELV $265
byte 1 117
byte 1 105
byte 1 95
byte 1 99
byte 1 97
byte 1 99
byte 1 104
byte 1 101
byte 1 0
align 1
LABELV $262
byte 1 112
byte 1 111
byte 1 115
byte 1 116
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $257
byte 1 114
byte 1 101
byte 1 109
byte 1 97
byte 1 112
byte 1 83
byte 1 104
byte 1 97
byte 1 100
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $254
byte 1 117
byte 1 105
byte 1 95
byte 1 108
byte 1 111
byte 1 97
byte 1 100
byte 1 0
align 1
LABELV $251
byte 1 117
byte 1 105
byte 1 95
byte 1 114
byte 1 101
byte 1 112
byte 1 111
byte 1 114
byte 1 116
byte 1 0
align 1
LABELV $248
byte 1 117
byte 1 105
byte 1 95
byte 1 116
byte 1 101
byte 1 115
byte 1 116
byte 1 0
align 1
LABELV $241
byte 1 117
byte 1 105
byte 1 95
byte 1 102
byte 1 114
byte 1 105
byte 1 101
byte 1 110
byte 1 100
byte 1 108
byte 1 121
byte 1 70
byte 1 105
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $240
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
byte 1 70
byte 1 105
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $239
byte 1 117
byte 1 105
byte 1 95
byte 1 112
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $238
byte 1 115
byte 1 118
byte 1 95
byte 1 112
byte 1 117
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $237
byte 1 117
byte 1 105
byte 1 95
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $236
byte 1 103
byte 1 95
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $235
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 111
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $234
byte 1 103
byte 1 95
byte 1 100
byte 1 111
byte 1 87
byte 1 97
byte 1 114
byte 1 109
byte 1 117
byte 1 112
byte 1 0
align 1
LABELV $233
byte 1 117
byte 1 105
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $232
byte 1 99
byte 1 103
byte 1 95
byte 1 100
byte 1 114
byte 1 97
byte 1 119
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 114
byte 1 0
align 1
LABELV $231
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 70
byte 1 114
byte 1 97
byte 1 103
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $230
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
LABELV $229
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 97
byte 1 118
byte 1 101
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 76
byte 1 105
byte 1 109
byte 1 105
byte 1 116
byte 1 0
align 1
LABELV $228
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
LABELV $202
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
LABELV $184
byte 1 117
byte 1 105
byte 1 95
byte 1 109
byte 1 97
byte 1 116
byte 1 99
byte 1 104
byte 1 83
byte 1 116
byte 1 97
byte 1 114
byte 1 116
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $167
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
LABELV $166
byte 1 109
byte 1 97
byte 1 112
byte 1 110
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $163
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 0
align 1
LABELV $154
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $153
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $147
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
LABELV $146
byte 1 100
byte 1 101
byte 1 109
byte 1 111
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 95
byte 1 37
byte 1 100
byte 1 46
byte 1 100
byte 1 109
byte 1 95
byte 1 37
byte 1 100
byte 1 0
align 1
LABELV $141
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 115
byte 1 47
byte 1 37
byte 1 115
byte 1 95
byte 1 37
byte 1 105
byte 1 46
byte 1 103
byte 1 97
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $139
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $138
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 50
byte 1 0
align 1
LABELV $137
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 104
byte 1 117
byte 1 116
byte 1 111
byte 1 117
byte 1 116
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $136
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $135
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $134
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 66
byte 1 97
byte 1 115
byte 1 101
byte 1 50
byte 1 0
align 1
LABELV $133
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 50
byte 1 0
align 1
LABELV $132
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 80
byte 1 101
byte 1 114
byte 1 102
byte 1 101
byte 1 99
byte 1 116
byte 1 50
byte 1 0
align 1
LABELV $131
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 50
byte 1 0
align 1
LABELV $130
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 71
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $129
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 115
byte 1 115
byte 1 105
byte 1 115
byte 1 116
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $128
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 68
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $127
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 69
byte 1 120
byte 1 99
byte 1 101
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $126
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 73
byte 1 109
byte 1 112
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 118
byte 1 101
byte 1 115
byte 1 50
byte 1 0
align 1
LABELV $125
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 99
byte 1 99
byte 1 117
byte 1 114
byte 1 97
byte 1 99
byte 1 121
byte 1 50
byte 1 0
align 1
LABELV $122
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 67
byte 1 97
byte 1 112
byte 1 116
byte 1 117
byte 1 114
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $121
byte 1 37
byte 1 48
byte 1 50
byte 1 105
byte 1 58
byte 1 37
byte 1 48
byte 1 50
byte 1 105
byte 1 0
align 1
LABELV $120
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 0
align 1
LABELV $119
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 104
byte 1 117
byte 1 116
byte 1 111
byte 1 117
byte 1 116
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $118
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 107
byte 1 105
byte 1 108
byte 1 108
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $117
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 105
byte 1 109
byte 1 101
byte 1 66
byte 1 111
byte 1 110
byte 1 117
byte 1 115
byte 1 0
align 1
LABELV $116
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 66
byte 1 97
byte 1 115
byte 1 101
byte 1 0
align 1
LABELV $115
byte 1 37
byte 1 105
byte 1 32
byte 1 116
byte 1 111
byte 1 32
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $114
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 84
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $113
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 80
byte 1 101
byte 1 114
byte 1 102
byte 1 101
byte 1 99
byte 1 116
byte 1 0
align 1
LABELV $112
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 83
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 0
align 1
LABELV $111
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 71
byte 1 97
byte 1 117
byte 1 110
byte 1 116
byte 1 108
byte 1 101
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $110
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 115
byte 1 115
byte 1 105
byte 1 115
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $109
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 68
byte 1 101
byte 1 102
byte 1 101
byte 1 110
byte 1 100
byte 1 115
byte 1 0
align 1
LABELV $108
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 69
byte 1 120
byte 1 99
byte 1 101
byte 1 108
byte 1 108
byte 1 101
byte 1 110
byte 1 116
byte 1 115
byte 1 0
align 1
LABELV $107
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $106
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 73
byte 1 109
byte 1 112
byte 1 114
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 118
byte 1 101
byte 1 115
byte 1 0
align 1
LABELV $105
byte 1 37
byte 1 105
byte 1 37
byte 1 37
byte 1 0
align 1
LABELV $104
byte 1 117
byte 1 105
byte 1 95
byte 1 115
byte 1 99
byte 1 111
byte 1 114
byte 1 101
byte 1 65
byte 1 99
byte 1 99
byte 1 117
byte 1 114
byte 1 97
byte 1 99
byte 1 121
byte 1 0
align 1
LABELV $98
byte 1 100
byte 1 49
byte 1 10
byte 1 0
align 1
LABELV $89
byte 1 37
byte 1 115
byte 1 0
