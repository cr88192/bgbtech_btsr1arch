export G_WriteClientSessionData
code
proc G_WriteClientSessionData 20 32
file "../g_session.c"
line 43
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
;23:#include "g_local.h"
;24:
;25:
;26:/*
;27:=======================================================================
;28:
;29:  SESSION DATA
;30:
;31:Session data is the only data that stays persistant across level loads
;32:and tournament restarts.
;33:=======================================================================
;34:*/
;35:
;36:/*
;37:================
;38:G_WriteClientSessionData
;39:
;40:Called on game shutdown
;41:================
;42:*/
;43:void G_WriteClientSessionData( gclient_t *client ) {
line 47
;44:	const char	*s;
;45:	const char	*var;
;46:
;47:	s = va("%i %i %i %i %i %i %i", 
ADDRGP4 $53
ARGP4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 616
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 620
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 636
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 640
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 57
;48:		client->sess.sessionTeam,
;49:		client->sess.spectatorTime,
;50:		client->sess.spectatorState,
;51:		client->sess.spectatorClient,
;52:		client->sess.wins,
;53:		client->sess.losses,
;54:		client->sess.teamLeader
;55:		);
;56:
;57:	var = va( "session%i", client - level.clients );
ADDRGP4 $54
ARGP4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 776
DIVI4
ARGI4
ADDRLP4 16
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ASGNP4
line 59
;58:
;59:	trap_Cvar_Set( var, s );
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 60
;60:}
LABELV $52
endproc G_WriteClientSessionData 20 32
export G_ReadSessionData
proc G_ReadSessionData 1048 36
line 69
;61:
;62:/*
;63:================
;64:G_ReadSessionData
;65:
;66:Called on a reconnect
;67:================
;68:*/
;69:void G_ReadSessionData( gclient_t *client ) {
line 78
;70:	char	s[MAX_STRING_CHARS];
;71:	const char	*var;
;72:
;73:	// bk001205 - format
;74:	int teamLeader;
;75:	int spectatorState;
;76:	int sessionTeam;
;77:
;78:	var = va( "session%i", client - level.clients );
ADDRGP4 $54
ARGP4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 776
DIVI4
ARGI4
ADDRLP4 1040
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1024
ADDRLP4 1040
INDIRP4
ASGNP4
line 79
;79:	trap_Cvar_VariableStringBuffer( var, s, sizeof(s) );
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 81
;80:
;81:	sscanf( s, "%i %i %i %i %i %i %i",
ADDRLP4 0
ARGP4
ADDRGP4 $53
ARGP4
ADDRLP4 1036
ARGP4
ADDRLP4 1044
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1044
INDIRP4
CNSTI4 620
ADDP4
ARGP4
ADDRLP4 1032
ARGP4
ADDRLP4 1044
INDIRP4
CNSTI4 628
ADDP4
ARGP4
ADDRLP4 1044
INDIRP4
CNSTI4 632
ADDP4
ARGP4
ADDRLP4 1044
INDIRP4
CNSTI4 636
ADDP4
ARGP4
ADDRLP4 1028
ARGP4
ADDRGP4 sscanf
CALLI4
pop
line 92
;82:		&sessionTeam,                 // bk010221 - format
;83:		&client->sess.spectatorTime,
;84:		&spectatorState,              // bk010221 - format
;85:		&client->sess.spectatorClient,
;86:		&client->sess.wins,
;87:		&client->sess.losses,
;88:		&teamLeader                   // bk010221 - format
;89:		);
;90:
;91:	// bk001205 - format issues
;92:	client->sess.sessionTeam = (team_t)sessionTeam;
ADDRFP4 0
INDIRP4
CNSTI4 616
ADDP4
ADDRLP4 1036
INDIRI4
ASGNI4
line 93
;93:	client->sess.spectatorState = (spectatorState_t)spectatorState;
ADDRFP4 0
INDIRP4
CNSTI4 624
ADDP4
ADDRLP4 1032
INDIRI4
ASGNI4
line 94
;94:	client->sess.teamLeader = (qboolean)teamLeader;
ADDRFP4 0
INDIRP4
CNSTI4 640
ADDP4
ADDRLP4 1028
INDIRI4
ASGNI4
line 95
;95:}
LABELV $55
endproc G_ReadSessionData 1048 36
export G_InitSessionData
proc G_InitSessionData 16 8
line 105
;96:
;97:
;98:/*
;99:================
;100:G_InitSessionData
;101:
;102:Called on a first-time connect
;103:================
;104:*/
;105:void G_InitSessionData( gclient_t *client, char *userinfo ) {
line 109
;106:	clientSession_t	*sess;
;107:	const char		*value;
;108:
;109:	sess = &client->sess;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 616
ADDP4
ASGNP4
line 112
;110:
;111:	// initial team determination
;112:	if ( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
LTI4 $57
line 113
;113:		if ( g_teamAutoJoin.integer ) {
ADDRGP4 g_teamAutoJoin+12
INDIRI4
CNSTI4 0
EQI4 $60
line 114
;114:			sess->sessionTeam = PickTeam( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 8
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ASGNI4
line 115
;115:			BroadcastTeamChange( client, -1 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 BroadcastTeamChange
CALLV
pop
line 116
;116:		} else {
ADDRGP4 $58
JUMPV
LABELV $60
line 118
;117:			// always spawn as spectator in team games
;118:			sess->sessionTeam = TEAM_SPECTATOR;	
ADDRLP4 0
INDIRP4
CNSTI4 3
ASGNI4
line 119
;119:		}
line 120
;120:	} else {
ADDRGP4 $58
JUMPV
LABELV $57
line 121
;121:		value = Info_ValueForKey( userinfo, "team" );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 $63
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 122
;122:		if ( value[0] == 's' ) {
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 115
NEI4 $64
line 124
;123:			// a willing spectator, not a waiting-in-line
;124:			sess->sessionTeam = TEAM_SPECTATOR;
ADDRLP4 0
INDIRP4
CNSTI4 3
ASGNI4
line 125
;125:		} else {
ADDRGP4 $65
JUMPV
LABELV $64
line 126
;126:			switch ( g_gametype.integer ) {
ADDRLP4 12
ADDRGP4 g_gametype+12
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $69
ADDRLP4 12
INDIRI4
CNSTI4 1
EQI4 $75
ADDRLP4 12
INDIRI4
CNSTI4 2
EQI4 $69
ADDRGP4 $66
JUMPV
LABELV $66
LABELV $69
line 130
;127:			default:
;128:			case GT_FFA:
;129:			case GT_SINGLE_PLAYER:
;130:				if ( g_maxGameClients.integer > 0 && 
ADDRGP4 g_maxGameClients+12
INDIRI4
CNSTI4 0
LEI4 $70
ADDRGP4 level+76
INDIRI4
ADDRGP4 g_maxGameClients+12
INDIRI4
LTI4 $70
line 131
;131:					level.numNonSpectatorClients >= g_maxGameClients.integer ) {
line 132
;132:					sess->sessionTeam = TEAM_SPECTATOR;
ADDRLP4 0
INDIRP4
CNSTI4 3
ASGNI4
line 133
;133:				} else {
ADDRGP4 $67
JUMPV
LABELV $70
line 134
;134:					sess->sessionTeam = TEAM_FREE;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 135
;135:				}
line 136
;136:				break;
ADDRGP4 $67
JUMPV
LABELV $75
line 139
;137:			case GT_TOURNAMENT:
;138:				// if the game is full, go into a waiting mode
;139:				if ( level.numNonSpectatorClients >= 2 ) {
ADDRGP4 level+76
INDIRI4
CNSTI4 2
LTI4 $76
line 140
;140:					sess->sessionTeam = TEAM_SPECTATOR;
ADDRLP4 0
INDIRP4
CNSTI4 3
ASGNI4
line 141
;141:				} else {
ADDRGP4 $67
JUMPV
LABELV $76
line 142
;142:					sess->sessionTeam = TEAM_FREE;
ADDRLP4 0
INDIRP4
CNSTI4 0
ASGNI4
line 143
;143:				}
line 144
;144:				break;
LABELV $67
line 146
;145:			}
;146:		}
LABELV $65
line 147
;147:	}
LABELV $58
line 149
;148:
;149:	sess->spectatorState = SPECTATOR_FREE;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 150
;150:	sess->spectatorTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 152
;151:
;152:	G_WriteClientSessionData( client );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_WriteClientSessionData
CALLV
pop
line 153
;153:}
LABELV $56
endproc G_InitSessionData 16 8
export G_InitWorldSession
proc G_InitWorldSession 1032 12
line 162
;154:
;155:
;156:/*
;157:==================
;158:G_InitWorldSession
;159:
;160:==================
;161:*/
;162:void G_InitWorldSession( void ) {
line 166
;163:	char	s[MAX_STRING_CHARS];
;164:	int			gt;
;165:
;166:	trap_Cvar_VariableStringBuffer( "session", s, sizeof(s) );
ADDRGP4 $81
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 167
;167:	gt = atoi( s );
ADDRLP4 0
ARGP4
ADDRLP4 1028
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1024
ADDRLP4 1028
INDIRI4
ASGNI4
line 171
;168:	
;169:	// if the gametype changed since the last session, don't use any
;170:	// client sessions
;171:	if ( g_gametype.integer != gt ) {
ADDRGP4 g_gametype+12
INDIRI4
ADDRLP4 1024
INDIRI4
EQI4 $82
line 172
;172:		level.newSession = qtrue;
ADDRGP4 level+64
CNSTI4 1
ASGNI4
line 173
;173:		G_Printf( "Gametype changed, clearing session data.\n" );
ADDRGP4 $86
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 174
;174:	}
LABELV $82
line 175
;175:}
LABELV $80
endproc G_InitWorldSession 1032 12
export G_WriteSessionData
proc G_WriteSessionData 8 8
line 183
;176:
;177:/*
;178:==================
;179:G_WriteSessionData
;180:
;181:==================
;182:*/
;183:void G_WriteSessionData( void ) {
line 186
;184:	int		i;
;185:
;186:	trap_Cvar_Set( "session", va("%i", g_gametype.integer) );
ADDRGP4 $88
ARGP4
ADDRGP4 g_gametype+12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $81
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 188
;187:
;188:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $93
JUMPV
LABELV $90
line 189
;189:		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
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
NEI4 $95
line 190
;190:			G_WriteClientSessionData( &level.clients[i] );
CNSTI4 776
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ARGP4
ADDRGP4 G_WriteClientSessionData
CALLV
pop
line 191
;191:		}
LABELV $95
line 192
;192:	}
LABELV $91
line 188
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $93
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $90
line 193
;193:}
LABELV $87
endproc G_WriteSessionData 8 8
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
LABELV $88
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $86
byte 1 71
byte 1 97
byte 1 109
byte 1 101
byte 1 116
byte 1 121
byte 1 112
byte 1 101
byte 1 32
byte 1 99
byte 1 104
byte 1 97
byte 1 110
byte 1 103
byte 1 101
byte 1 100
byte 1 44
byte 1 32
byte 1 99
byte 1 108
byte 1 101
byte 1 97
byte 1 114
byte 1 105
byte 1 110
byte 1 103
byte 1 32
byte 1 115
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 32
byte 1 100
byte 1 97
byte 1 116
byte 1 97
byte 1 46
byte 1 10
byte 1 0
align 1
LABELV $81
byte 1 115
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 0
align 1
LABELV $63
byte 1 116
byte 1 101
byte 1 97
byte 1 109
byte 1 0
align 1
LABELV $54
byte 1 115
byte 1 101
byte 1 115
byte 1 115
byte 1 105
byte 1 111
byte 1 110
byte 1 37
byte 1 105
byte 1 0
align 1
LABELV $53
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 32
byte 1 37
byte 1 105
byte 1 0
