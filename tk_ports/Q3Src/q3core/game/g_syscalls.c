/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
#include "g_local.h"

// this file is only included when building a dll
// g_syscalls.asm is included instead when building a qvm
#ifdef Q3_VM
#error "Do not use in VM build"
#endif

static nlint (QDECL *syscall_i)( nlint *args );

static nlint syscall_1( nlint a0 )
{
	nlint args[16];
	args[0]=a0;
	return(syscall_i(args));
}

static nlint syscall_2( nlint a0, nlint a1 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;
	return(syscall_i(args));
}

static nlint syscall_3( nlint a0, nlint a1, nlint a2 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;
	return(syscall_i(args));
}

static nlint syscall_4( nlint a0, nlint a1, nlint a2, nlint a3 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	return(syscall_i(args));
}


static nlint syscall_5( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;
	return(syscall_i(args));
}

static nlint syscall_6( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;
	return(syscall_i(args));
}

static nlint syscall_7( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;
	return(syscall_i(args));
}

static nlint syscall_8( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;	args[7]=a7;
	return(syscall_i(args));
}


static nlint syscall_9( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;	args[7]=a7;
	args[8]=a8;
	return(syscall_i(args));
}

static nlint syscall_10( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;	args[7]=a7;
	args[8]=a8;	args[9]=a9;
	return(syscall_i(args));
}

static nlint syscall_11( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;	args[7]=a7;
	args[8]=a8;	args[9]=a9;	args[10]=a10;
	return(syscall_i(args));
}

static nlint syscall_12( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10, nlint a11 )
{
	nlint args[16];
	args[0]=a0;	args[1]=a1;	args[2]=a2;	args[3]=a3;
	args[4]=a4;	args[5]=a5;	args[6]=a6;	args[7]=a7;
	args[8]=a8;	args[9]=a9;	args[10]=a10;	args[11]=a11;
	return(syscall_i(args));
}

static nlint syscall_13( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10, nlint a11,
	nlint a12 )
{
	nlint args[16];
	args[ 0]=a0;	args[ 1]=a1;	args[ 2]=a2;	args[3]=a3;
	args[ 4]=a4;	args[ 5]=a5;	args[ 6]=a6;	args[7]=a7;
	args[ 8]=a8;	args[ 9]=a9;	args[10]=a10;	args[11]=a11;
	args[12]=a12;
	return(syscall_i(args));
}

static nlint syscall_14( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10, nlint a11,
	nlint a12, nlint a13 )
{
	nlint args[16];
	args[ 0]=a0;	args[ 1]=a1;	args[ 2]=a2;	args[3]=a3;
	args[ 4]=a4;	args[ 5]=a5;	args[ 6]=a6;	args[7]=a7;
	args[ 8]=a8;	args[ 9]=a9;	args[10]=a10;	args[11]=a11;
	args[12]=a12;	args[13]=a13;
	return(syscall_i(args));
}

static nlint syscall_15( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10, nlint a11,
	nlint a12, nlint a13, nlint a14 )
{
	nlint args[16];
	args[ 0]=a0;	args[ 1]=a1;	args[ 2]=a2;	args[3]=a3;
	args[ 4]=a4;	args[ 5]=a5;	args[ 6]=a6;	args[7]=a7;
	args[ 8]=a8;	args[ 9]=a9;	args[10]=a10;	args[11]=a11;
	args[12]=a12;	args[13]=a13;	args[14]=a14;
	return(syscall_i(args));
}

static nlint syscall_16( nlint a0, nlint a1, nlint a2, nlint a3,
	nlint a4, nlint a5, nlint a6, nlint a7,
	nlint a8, nlint a9, nlint a10, nlint a11,
	nlint a12, nlint a13, nlint a14, nlint a15 )
{
	nlint args[16];
	args[ 0]=a0;	args[ 1]=a1;	args[ 2]=a2;	args[3]=a3;
	args[ 4]=a4;	args[ 5]=a5;	args[ 6]=a6;	args[7]=a7;
	args[ 8]=a8;	args[ 9]=a9;	args[10]=a10;	args[11]=a11;
	args[12]=a12;	args[13]=a13;	args[14]=a14;	args[15]=a15;
	return(syscall_i(args));
}

//void dllEntry( int (QDECL *syscallptr)( int arg,... ) ) {
VM_EXPORT void dllEntry( nlint (QDECL *syscallptr)( nlint *arg ) ) {
	syscall_i = syscallptr;
}


int PASSFLOAT( float x ) {
	float	floatTemp;
	floatTemp = x;
	return *(int *)&floatTemp;
}

void	trap_Printf( const char *fmt ) {
	syscall_2( G_PRINT, (nlint)fmt );
}

void	trap_Error( const char *fmt ) {
	syscall_2( G_ERROR, (nlint)fmt );
}

int		trap_Milliseconds( void ) {
	return syscall_1( G_MILLISECONDS ); 
}
int		trap_Argc( void ) {
	return syscall_1( G_ARGC );
}

void	trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall_4( G_ARGV, n, (nlint)buffer, bufferLength );
}

int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return syscall_4( G_FS_FOPEN_FILE, (nlint)qpath, (nlint)f, mode );
}

void	trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	syscall_4( G_FS_READ, (nlint)buffer, len, f );
}

void	trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	syscall_4( G_FS_WRITE, (nlint)buffer, len, f );
}

void	trap_FS_FCloseFile( fileHandle_t f ) {
	syscall_2( G_FS_FCLOSE_FILE, f );
}

int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) {
	return syscall_5( G_FS_GETFILELIST,
		(nlint)path, (nlint)extension, (nlint)listbuf, bufsize );
}

int trap_FS_Seek( fileHandle_t f, long offset, int origin ) {
	return syscall_4( G_FS_SEEK, f, offset, origin );
}

void	trap_SendConsoleCommand( int exec_when, const char *text ) {
	syscall_3( G_SEND_CONSOLE_COMMAND, exec_when, (nlint)text );
}

void	trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags ) {
	syscall_5( G_CVAR_REGISTER, (nlint)cvar, (nlint)var_name, (nlint)value, flags );
}

void	trap_Cvar_Update( vmCvar_t *cvar ) {
	syscall_2( G_CVAR_UPDATE, (nlint)cvar );
}

void trap_Cvar_Set( const char *var_name, const char *value ) {
	syscall_3( G_CVAR_SET, (nlint)var_name, (nlint)value );
}

int trap_Cvar_VariableIntegerValue( const char *var_name ) {
	return syscall_2( G_CVAR_VARIABLE_INTEGER_VALUE, (nlint)var_name );
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	syscall_4( G_CVAR_VARIABLE_STRING_BUFFER,
		(nlint)var_name, (nlint)buffer, bufsize );
}


void trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t,
						 playerState_t *clients, int sizeofGClient ) {
	syscall_6( G_LOCATE_GAME_DATA,
		(nlint)gEnts, numGEntities, sizeofGEntity_t,
		(nlint)clients, sizeofGClient );
}

void trap_DropClient( int clientNum, const char *reason ) {
	syscall_3( G_DROP_CLIENT, clientNum, (nlint)reason );
}

void trap_SendServerCommand( int clientNum, const char *text ) {
	syscall_3( G_SEND_SERVER_COMMAND, clientNum, (nlint)text );
}

void trap_SetConfigstring( int num, const char *string ) {
	syscall_3( G_SET_CONFIGSTRING, num, (nlint)string );
}

void trap_GetConfigstring( int num, char *buffer, int bufferSize ) {
	syscall_4( G_GET_CONFIGSTRING, num, (nlint)buffer, bufferSize );
}

void trap_GetUserinfo( int num, char *buffer, int bufferSize ) {
	syscall_4( G_GET_USERINFO, num, (nlint)buffer, bufferSize );
}

void trap_SetUserinfo( int num, const char *buffer ) {
	syscall_3( G_SET_USERINFO, num, (nlint)buffer );
}

void trap_GetServerinfo( char *buffer, int bufferSize ) {
	syscall_3( G_GET_SERVERINFO, (nlint)buffer, bufferSize );
}

void trap_SetBrushModel( gentity_t *ent, const char *name ) {
	syscall_3( G_SET_BRUSH_MODEL, (nlint)ent, (nlint)name );
}

void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask ) {
	syscall_8( G_TRACE,
		(nlint)results, (nlint)start, (nlint)mins, (nlint)maxs, (nlint)end, passEntityNum, contentmask );
}

void trap_TraceCapsule( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask ) {
	syscall_8( G_TRACECAPSULE,
		(nlint)results, (nlint)start, (nlint)mins, (nlint)maxs, (nlint)end, passEntityNum, contentmask );
}

int trap_PointContents( const vec3_t point, int passEntityNum ) {
	return syscall_3( G_POINT_CONTENTS, (nlint)point, passEntityNum );
}


qboolean trap_InPVS( const vec3_t p1, const vec3_t p2 ) {
	return syscall_3( G_IN_PVS, (nlint)p1, (nlint)p2 );
}

qboolean trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 ) {
	return syscall_3( G_IN_PVS_IGNORE_PORTALS, (nlint)p1, (nlint)p2 );
}

void trap_AdjustAreaPortalState( gentity_t *ent, qboolean open ) {
	syscall_3( G_ADJUST_AREA_PORTAL_STATE, (nlint)ent, open );
}

qboolean trap_AreasConnected( int area1, int area2 ) {
	return syscall_3( G_AREAS_CONNECTED, area1, area2 );
}

void trap_LinkEntity( gentity_t *ent ) {
	syscall_2( G_LINKENTITY, (nlint)ent );
}

void trap_UnlinkEntity( gentity_t *ent ) {
	syscall_2( G_UNLINKENTITY, (nlint)ent );
}

int trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *list, int maxcount ) {
	return syscall_5( G_ENTITIES_IN_BOX,
		(nlint)mins, (nlint)maxs, (nlint)list, maxcount );
}

qboolean trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent ) {
	return syscall_4( G_ENTITY_CONTACT, (nlint)mins, (nlint)maxs, (nlint)ent );
}

qboolean trap_EntityContactCapsule( const vec3_t mins, const vec3_t maxs, const gentity_t *ent ) {
	return syscall_4( G_ENTITY_CONTACTCAPSULE,
		(nlint)mins, (nlint)maxs, (nlint)ent );
}

int trap_BotAllocateClient( void ) {
	return syscall_1( G_BOT_ALLOCATE_CLIENT );
}

void trap_BotFreeClient( int clientNum ) {
	syscall_2( G_BOT_FREE_CLIENT, clientNum );
}

void trap_GetUsercmd( int clientNum, usercmd_t *cmd ) {
	syscall_3( G_GET_USERCMD, clientNum, (nlint)cmd );
}

qboolean trap_GetEntityToken( char *buffer, int bufferSize ) {
	return syscall_3( G_GET_ENTITY_TOKEN, (nlint)buffer, bufferSize );
}

int trap_DebugPolygonCreate(int color, int numPoints, vec3_t *points) {
	return syscall_4( G_DEBUG_POLYGON_CREATE,
		color, numPoints, (nlint)points );
}

void trap_DebugPolygonDelete(int id) {
	syscall_2( G_DEBUG_POLYGON_DELETE, id );
}

int trap_RealTime( qtime_t *qtime ) {
	return syscall_2( G_REAL_TIME, (nlint)qtime );
}

void trap_SnapVector( float *v ) {
	syscall_2( G_SNAPVECTOR, (nlint)v );
	return;
}

// BotLib traps start here
int trap_BotLibSetup( void ) {
	return syscall_1( BOTLIB_SETUP );
}

int trap_BotLibShutdown( void ) {
	return syscall_1( BOTLIB_SHUTDOWN );
}

int trap_BotLibVarSet(char *var_name, char *value) {
	return syscall_3( BOTLIB_LIBVAR_SET, (nlint)var_name, (nlint)value );
}

int trap_BotLibVarGet(char *var_name, char *value, int size) {
	return syscall_4( BOTLIB_LIBVAR_GET,
		(nlint)var_name, (nlint)value, size );
}

int trap_BotLibDefine(char *string) {
	return syscall_2( BOTLIB_PC_ADD_GLOBAL_DEFINE, (nlint)string );
}

int trap_BotLibStartFrame(float time) {
	return syscall_2( BOTLIB_START_FRAME, PASSFLOAT( time ) );
}

int trap_BotLibLoadMap(const char *mapname) {
	return syscall_2( BOTLIB_LOAD_MAP, (nlint)mapname );
}

int trap_BotLibUpdateEntity(int ent, void /* struct bot_updateentity_s */ *bue) {
	return syscall_3( BOTLIB_UPDATENTITY, ent, (nlint)bue );
}

int trap_BotLibTest(int parm0, char *parm1, vec3_t parm2, vec3_t parm3) {
	return syscall_5( BOTLIB_TEST,
		(nlint)parm0, (nlint)parm1, (nlint)parm2, (nlint)parm3 );
}

int trap_BotGetSnapshotEntity( int clientNum, int sequence ) {
	return syscall_3( BOTLIB_GET_SNAPSHOT_ENTITY, clientNum, sequence );
}

int trap_BotGetServerCommand(int clientNum, char *message, int size) {
	return syscall_4( BOTLIB_GET_CONSOLE_MESSAGE,
		clientNum, (nlint)message, size );
}

void trap_BotUserCommand(int clientNum, usercmd_t *ucmd) {
	syscall_3( BOTLIB_USER_COMMAND, clientNum, (nlint)ucmd );
}

void trap_AAS_EntityInfo(int entnum, void /* struct aas_entityinfo_s */ *info) {
	syscall_3( BOTLIB_AAS_ENTITY_INFO, entnum, (nlint)info );
}

int trap_AAS_Initialized(void) {
	return syscall_1( BOTLIB_AAS_INITIALIZED );
}

void trap_AAS_PresenceTypeBoundingBox(int presencetype, vec3_t mins, vec3_t maxs) {
	syscall_4( BOTLIB_AAS_PRESENCE_TYPE_BOUNDING_BOX,
		presencetype, (nlint)mins, (nlint)maxs );
}

float trap_AAS_Time(void) {
	int temp;
	temp = syscall_1( BOTLIB_AAS_TIME );
	return (*(float*)&temp);
}

int trap_AAS_PointAreaNum(vec3_t point) {
	return syscall_2( BOTLIB_AAS_POINT_AREA_NUM, (nlint)point );
}

int trap_AAS_PointReachabilityAreaIndex(vec3_t point) {
	return syscall_2( BOTLIB_AAS_POINT_REACHABILITY_AREA_INDEX, (nlint)point );
}

int trap_AAS_TraceAreas(vec3_t start, vec3_t end, int *areas, vec3_t *points, int maxareas) {
	return syscall_6( BOTLIB_AAS_TRACE_AREAS,
		(nlint)start, (nlint)end, (nlint)areas, (nlint)points, maxareas );
}

int trap_AAS_BBoxAreas(vec3_t absmins, vec3_t absmaxs, int *areas, int maxareas) {
	return syscall_5( BOTLIB_AAS_BBOX_AREAS,
		(nlint)absmins, (nlint)absmaxs, (nlint)areas, (nlint)maxareas );
}

int trap_AAS_AreaInfo( int areanum, void /* struct aas_areainfo_s */ *info ) {
	return syscall_3( BOTLIB_AAS_AREA_INFO, areanum, (nlint)info );
}

int trap_AAS_PointContents(vec3_t point) {
	return syscall_2( BOTLIB_AAS_POINT_CONTENTS, (nlint)point );
}

int trap_AAS_NextBSPEntity(int ent) {
	return syscall_2( BOTLIB_AAS_NEXT_BSP_ENTITY, ent );
}

int trap_AAS_ValueForBSPEpairKey(int ent, char *key, char *value, int size) {
	return syscall_5( BOTLIB_AAS_VALUE_FOR_BSP_EPAIR_KEY,
		ent, (nlint)key, (nlint)value, size );
}

int trap_AAS_VectorForBSPEpairKey(int ent, char *key, vec3_t v) {
	return syscall_4( BOTLIB_AAS_VECTOR_FOR_BSP_EPAIR_KEY,
		ent, (nlint)key, (nlint)v );
}

int trap_AAS_FloatForBSPEpairKey(int ent, char *key, float *value) {
	return syscall_4( BOTLIB_AAS_FLOAT_FOR_BSP_EPAIR_KEY,
		ent, (nlint)key, (nlint)value );
}

int trap_AAS_IntForBSPEpairKey(int ent, char *key, int *value) {
	return syscall_4( BOTLIB_AAS_INT_FOR_BSP_EPAIR_KEY,
		ent, (nlint)key, (nlint)value );
}

int trap_AAS_AreaReachability(int areanum) {
	return syscall_2( BOTLIB_AAS_AREA_REACHABILITY, areanum );
}

int trap_AAS_AreaTravelTimeToGoalArea(int areanum, vec3_t origin, int goalareanum, int travelflags) {
	return syscall_5( BOTLIB_AAS_AREA_TRAVEL_TIME_TO_GOAL_AREA,
		areanum, (nlint)origin, goalareanum, travelflags );
}

int trap_AAS_EnableRoutingArea( int areanum, int enable )
{
	return syscall_3( BOTLIB_AAS_ENABLE_ROUTING_AREA, areanum, enable );
}

int trap_AAS_PredictRoute(void /*struct aas_predictroute_s*/ *route,
	int areanum, vec3_t origin,
	int goalareanum, int travelflags, int maxareas, int maxtime,
	int stopevent, int stopcontents, int stoptfl, int stopareanum)
{
	return syscall_12( BOTLIB_AAS_PREDICT_ROUTE,
		(nlint)route, areanum, (nlint)origin, goalareanum,
		travelflags, maxareas, maxtime, stopevent,
		stopcontents, stoptfl, stopareanum );
}

int trap_AAS_AlternativeRouteGoals(vec3_t start, int startareanum,
	vec3_t goal, int goalareanum, int travelflags,
	void /*struct aas_altroutegoal_s*/ *altroutegoals,
	int maxaltroutegoals,
	int type)
{
	return syscall_9( BOTLIB_AAS_ALTERNATIVE_ROUTE_GOAL,
		(nlint)start, startareanum, (nlint)goal,
		goalareanum, travelflags, (nlint)altroutegoals,
		maxaltroutegoals, type );
}

int trap_AAS_Swimming(vec3_t origin) {
	return syscall_2( BOTLIB_AAS_SWIMMING, (nlint)origin );
}

int trap_AAS_PredictClientMovement(
	void /* struct aas_clientmove_s */ *move, int entnum,
	vec3_t origin, int presencetype, int onground,
	vec3_t velocity, vec3_t cmdmove,
	int cmdframes, int maxframes,
	float frametime, int stopevent,
	int stopareanum, int visualize)
{
	return syscall_14( BOTLIB_AAS_PREDICT_CLIENT_MOVEMENT,
		(nlint)move, entnum, (nlint)origin, presencetype,
		onground, (nlint)velocity, (nlint)cmdmove, cmdframes,
		maxframes, PASSFLOAT(frametime),
		stopevent, stopareanum, visualize );
}

void trap_EA_Say(int client, char *str) {
	syscall_3( BOTLIB_EA_SAY, client, (nlint)str );
}

void trap_EA_SayTeam(int client, char *str) {
	syscall_3( BOTLIB_EA_SAY_TEAM, client,(nlint) str );
}

void trap_EA_Command(int client, char *command) {
	syscall_3( BOTLIB_EA_COMMAND, client, (nlint)command );
}

void trap_EA_Action(int client, int action) {
	syscall_3( BOTLIB_EA_ACTION, client, action );
}

void trap_EA_Gesture(int client) {
	syscall_2( BOTLIB_EA_GESTURE, client );
}

void trap_EA_Talk(int client) {
	syscall_2( BOTLIB_EA_TALK, client );
}

void trap_EA_Attack(int client) {
	syscall_2( BOTLIB_EA_ATTACK, client );
}

void trap_EA_Use(int client) {
	syscall_2( BOTLIB_EA_USE, client );
}

void trap_EA_Respawn(int client) {
	syscall_2( BOTLIB_EA_RESPAWN, client );
}

void trap_EA_Crouch(int client) {
	syscall_2( BOTLIB_EA_CROUCH, client );
}

void trap_EA_MoveUp(int client) {
	syscall_2( BOTLIB_EA_MOVE_UP, client );
}

void trap_EA_MoveDown(int client) {
	syscall_2( BOTLIB_EA_MOVE_DOWN, client );
}

void trap_EA_MoveForward(int client) {
	syscall_2( BOTLIB_EA_MOVE_FORWARD, client );
}

void trap_EA_MoveBack(int client) {
	syscall_2( BOTLIB_EA_MOVE_BACK, client );
}

void trap_EA_MoveLeft(int client) {
	syscall_2( BOTLIB_EA_MOVE_LEFT, client );
}

void trap_EA_MoveRight(int client) {
	syscall_2( BOTLIB_EA_MOVE_RIGHT, client );
}

void trap_EA_SelectWeapon(int client, int weapon) {
	syscall_3( BOTLIB_EA_SELECT_WEAPON, client, weapon );
}

void trap_EA_Jump(int client) {
	syscall_2( BOTLIB_EA_JUMP, client );
}

void trap_EA_DelayedJump(int client) {
	syscall_2( BOTLIB_EA_DELAYED_JUMP, client );
}

void trap_EA_Move(int client, vec3_t dir, float speed) {
	syscall_4( BOTLIB_EA_MOVE, client, (nlint)dir, PASSFLOAT(speed) );
}

void trap_EA_View(int client, vec3_t viewangles) {
	syscall_3( BOTLIB_EA_VIEW, client, (nlint)viewangles );
}

void trap_EA_EndRegular(int client, float thinktime) {
	syscall_3( BOTLIB_EA_END_REGULAR, client, PASSFLOAT(thinktime) );
}

void trap_EA_GetInput(int client, float thinktime, void /* struct bot_input_s */ *input) {
	syscall_4( BOTLIB_EA_GET_INPUT,
		client, PASSFLOAT(thinktime), (nlint)input );
}

void trap_EA_ResetInput(int client) {
	syscall_2( BOTLIB_EA_RESET_INPUT, client );
}

int trap_BotLoadCharacter(char *charfile, float skill) {
	return syscall_3( BOTLIB_AI_LOAD_CHARACTER,
		(nlint)charfile, PASSFLOAT(skill));
}

void trap_BotFreeCharacter(int character) {
	syscall_2( BOTLIB_AI_FREE_CHARACTER, character );
}

float trap_Characteristic_Float(int character, int index) {
	int temp;
	temp = syscall_3( BOTLIB_AI_CHARACTERISTIC_FLOAT, character, index );
	return (*(float*)&temp);
}

float trap_Characteristic_BFloat(int character, int index, float min, float max) {
	int temp;
	temp = syscall_5( BOTLIB_AI_CHARACTERISTIC_BFLOAT,
		character, index, PASSFLOAT(min), PASSFLOAT(max) );
	return (*(float*)&temp);
}

int trap_Characteristic_Integer(int character, int index) {
	return syscall_3( BOTLIB_AI_CHARACTERISTIC_INTEGER, character, index );
}

int trap_Characteristic_BInteger(int character, int index, int min, int max) {
	return syscall_5( BOTLIB_AI_CHARACTERISTIC_BINTEGER,
		character, index, min, max );
}

void trap_Characteristic_String(int character, int index, char *buf, int size) {
	syscall_5( BOTLIB_AI_CHARACTERISTIC_STRING,
		character, index, (nlint)buf, size );
}

int trap_BotAllocChatState(void) {
	return syscall_1( BOTLIB_AI_ALLOC_CHAT_STATE );
}

void trap_BotFreeChatState(int handle) {
	syscall_2( BOTLIB_AI_FREE_CHAT_STATE, handle );
}

void trap_BotQueueConsoleMessage(int chatstate, int type, char *message) {
	syscall_4( BOTLIB_AI_QUEUE_CONSOLE_MESSAGE,
		chatstate, type, (nlint)message );
}

void trap_BotRemoveConsoleMessage(int chatstate, int handle) {
	syscall_3( BOTLIB_AI_REMOVE_CONSOLE_MESSAGE, chatstate, handle );
}

int trap_BotNextConsoleMessage(int chatstate, void /* struct bot_consolemessage_s */ *cm) {
	return syscall_3( BOTLIB_AI_NEXT_CONSOLE_MESSAGE, chatstate, (nlint)cm );
}

int trap_BotNumConsoleMessages(int chatstate) {
	return syscall_2( BOTLIB_AI_NUM_CONSOLE_MESSAGE, chatstate );
}

void trap_BotInitialChat(int chatstate, char *type, int mcontext, char *var0, char *var1, char *var2, char *var3, char *var4, char *var5, char *var6, char *var7 ) {
	syscall_12( BOTLIB_AI_INITIAL_CHAT,
		chatstate, (nlint)type, mcontext,
		(nlint)var0, (nlint)var1, (nlint)var2, (nlint)var3,
		(nlint)var4, (nlint)var5, (nlint)var6, (nlint)var7 );
}

int	trap_BotNumInitialChats(int chatstate, char *type) {
	return syscall_3( BOTLIB_AI_NUM_INITIAL_CHATS, chatstate, (nlint)type );
}

int trap_BotReplyChat(int chatstate, char *message, int mcontext, int vcontext, char *var0, char *var1, char *var2, char *var3, char *var4, char *var5, char *var6, char *var7 ) {
	return syscall_13( BOTLIB_AI_REPLY_CHAT,
		(nlint)chatstate, (nlint)message, mcontext, vcontext,
		(nlint)var0, (nlint)var1, (nlint)var2, (nlint)var3,
		(nlint)var4, (nlint)var5, (nlint)var6, (nlint)var7 );
}

int trap_BotChatLength(int chatstate) {
	return syscall_2( BOTLIB_AI_CHAT_LENGTH, chatstate );
}

void trap_BotEnterChat(int chatstate, int client, int sendto) {
	syscall_4( BOTLIB_AI_ENTER_CHAT, chatstate, client, sendto );
}

void trap_BotGetChatMessage(int chatstate, char *buf, int size) {
	syscall_4( BOTLIB_AI_GET_CHAT_MESSAGE, chatstate, (nlint)buf, size);
}

int trap_StringContains(char *str1, char *str2, int casesensitive) {
	return syscall_4( BOTLIB_AI_STRING_CONTAINS,
		(nlint)str1, (nlint)str2, casesensitive );
}

int trap_BotFindMatch(char *str, void /* struct bot_match_s */ *match, unsigned long int context) {
	return syscall_4( BOTLIB_AI_FIND_MATCH,
		(nlint)str, (nlint)match, context );
}

void trap_BotMatchVariable(void /* struct bot_match_s */ *match, int variable, char *buf, int size) {
	syscall_5( BOTLIB_AI_MATCH_VARIABLE,
		(nlint)match, variable, (nlint)buf, size );
}

void trap_UnifyWhiteSpaces(char *string) {
	syscall_2( BOTLIB_AI_UNIFY_WHITE_SPACES, (nlint)string );
}

void trap_BotReplaceSynonyms(char *string, unsigned long int context) {
	syscall_3( BOTLIB_AI_REPLACE_SYNONYMS, (nlint)string, context );
}

int trap_BotLoadChatFile(int chatstate, char *chatfile, char *chatname) {
	return syscall_4( BOTLIB_AI_LOAD_CHAT_FILE,
		(nlint)chatstate, (nlint)chatfile, (nlint)chatname );
}

void trap_BotSetChatGender(int chatstate, int gender) {
	syscall_3( BOTLIB_AI_SET_CHAT_GENDER, chatstate, gender );
}

void trap_BotSetChatName(int chatstate, char *name, int client) {
	syscall_4( BOTLIB_AI_SET_CHAT_NAME, chatstate, (nlint)name, client );
}

void trap_BotResetGoalState(int goalstate) {
	syscall_2( BOTLIB_AI_RESET_GOAL_STATE, goalstate );
}

void trap_BotResetAvoidGoals(int goalstate) {
	syscall_2( BOTLIB_AI_RESET_AVOID_GOALS, goalstate );
}

void trap_BotRemoveFromAvoidGoals(int goalstate, int number) {
	syscall_3( BOTLIB_AI_REMOVE_FROM_AVOID_GOALS, goalstate, number);
}

void trap_BotPushGoal(int goalstate, void /* struct bot_goal_s */ *goal) {
	syscall_3( BOTLIB_AI_PUSH_GOAL, goalstate, (nlint)goal );
}

void trap_BotPopGoal(int goalstate) {
	syscall_2( BOTLIB_AI_POP_GOAL, goalstate );
}

void trap_BotEmptyGoalStack(int goalstate) {
	syscall_2( BOTLIB_AI_EMPTY_GOAL_STACK, goalstate );
}

void trap_BotDumpAvoidGoals(int goalstate) {
	syscall_2( BOTLIB_AI_DUMP_AVOID_GOALS, goalstate );
}

void trap_BotDumpGoalStack(int goalstate) {
	syscall_2( BOTLIB_AI_DUMP_GOAL_STACK, goalstate );
}

void trap_BotGoalName(int number, char *name, int size) {
	syscall_4( BOTLIB_AI_GOAL_NAME, number, (nlint)name, size );
}

int trap_BotGetTopGoal(int goalstate, void /* struct bot_goal_s */ *goal) {
	return syscall_3( BOTLIB_AI_GET_TOP_GOAL,
		goalstate, (nlint)goal );
}

int trap_BotGetSecondGoal(int goalstate, void /* struct bot_goal_s */ *goal) {
	return syscall_3( BOTLIB_AI_GET_SECOND_GOAL,
		goalstate, (nlint)goal );
}

int trap_BotChooseLTGItem(int goalstate, vec3_t origin, int *inventory, int travelflags) {
	return syscall_5( BOTLIB_AI_CHOOSE_LTG_ITEM,
		goalstate, (nlint)origin, (nlint)inventory, travelflags );
}

int trap_BotChooseNBGItem(int goalstate, vec3_t origin, int *inventory, int travelflags, void /* struct bot_goal_s */ *ltg, float maxtime) {
	return syscall_7( BOTLIB_AI_CHOOSE_NBG_ITEM,
		goalstate, (nlint)origin, (nlint)inventory,
		travelflags, (nlint)ltg, PASSFLOAT(maxtime) );
}

int trap_BotTouchingGoal(vec3_t origin, void /* struct bot_goal_s */ *goal) {
	return syscall_3( BOTLIB_AI_TOUCHING_GOAL, (nlint)origin, (nlint)goal );
}

int trap_BotItemGoalInVisButNotVisible(int viewer, vec3_t eye, vec3_t viewangles, void /* struct bot_goal_s */ *goal) {
	return syscall_5( BOTLIB_AI_ITEM_GOAL_IN_VIS_BUT_NOT_VISIBLE,
		viewer, (nlint)eye, (nlint)viewangles, (nlint)goal );
}

int trap_BotGetLevelItemGoal(int index, char *classname, void /* struct bot_goal_s */ *goal) {
	return syscall_4( BOTLIB_AI_GET_LEVEL_ITEM_GOAL,
		index, (nlint)classname, (nlint)goal );
}

int trap_BotGetNextCampSpotGoal(int num, void /* struct bot_goal_s */ *goal) {
	return syscall_3( BOTLIB_AI_GET_NEXT_CAMP_SPOT_GOAL, num, (nlint)goal );
}

int trap_BotGetMapLocationGoal(char *name, void /* struct bot_goal_s */ *goal) {
	return syscall_3( BOTLIB_AI_GET_MAP_LOCATION_GOAL,
		(nlint)name, (nlint)goal );
}

float trap_BotAvoidGoalTime(int goalstate, int number) {
	int temp;
	temp = syscall_3( BOTLIB_AI_AVOID_GOAL_TIME, goalstate, number );
	return (*(float*)&temp);
}

void trap_BotSetAvoidGoalTime(int goalstate, int number, float avoidtime) {
	syscall_4( BOTLIB_AI_SET_AVOID_GOAL_TIME,
		goalstate, number, PASSFLOAT(avoidtime));
}

void trap_BotInitLevelItems(void) {
	syscall_1( BOTLIB_AI_INIT_LEVEL_ITEMS );
}

void trap_BotUpdateEntityItems(void) {
	syscall_1( BOTLIB_AI_UPDATE_ENTITY_ITEMS );
}

int trap_BotLoadItemWeights(int goalstate, char *filename) {
	return syscall_3( BOTLIB_AI_LOAD_ITEM_WEIGHTS,
		goalstate, (nlint)filename );
}

void trap_BotFreeItemWeights(int goalstate) {
	syscall_2( BOTLIB_AI_FREE_ITEM_WEIGHTS, goalstate );
}

void trap_BotInterbreedGoalFuzzyLogic(int parent1, int parent2, int child) {
	syscall_4( BOTLIB_AI_INTERBREED_GOAL_FUZZY_LOGIC,
		parent1, parent2, child );
}

void trap_BotSaveGoalFuzzyLogic(int goalstate, char *filename) {
	syscall_3( BOTLIB_AI_SAVE_GOAL_FUZZY_LOGIC,
		goalstate, (nlint)filename );
}

void trap_BotMutateGoalFuzzyLogic(int goalstate, float range) {
	syscall_3( BOTLIB_AI_MUTATE_GOAL_FUZZY_LOGIC, goalstate, range );
}

int trap_BotAllocGoalState(int state) {
	return syscall_2( BOTLIB_AI_ALLOC_GOAL_STATE, state );
}

void trap_BotFreeGoalState(int handle) {
	syscall_2( BOTLIB_AI_FREE_GOAL_STATE, handle );
}

void trap_BotResetMoveState(int movestate) {
	syscall_2( BOTLIB_AI_RESET_MOVE_STATE, movestate );
}

void trap_BotAddAvoidSpot(int movestate, vec3_t origin, float radius, int type) {
	syscall_5( BOTLIB_AI_ADD_AVOID_SPOT,
		movestate, (nlint)origin, PASSFLOAT(radius), type);
}

void trap_BotMoveToGoal(void /* struct bot_moveresult_s */ *result, int movestate, void /* struct bot_goal_s */ *goal, int travelflags) {
	syscall_5( BOTLIB_AI_MOVE_TO_GOAL,
		(nlint)result, movestate, (nlint)goal, travelflags );
}

int trap_BotMoveInDirection(int movestate, vec3_t dir, float speed, int type) {
	return syscall_5( BOTLIB_AI_MOVE_IN_DIRECTION,
		movestate, (nlint)dir, PASSFLOAT(speed), type );
}

void trap_BotResetAvoidReach(int movestate) {
	syscall_2( BOTLIB_AI_RESET_AVOID_REACH, movestate );
}

void trap_BotResetLastAvoidReach(int movestate) {
	syscall_2( BOTLIB_AI_RESET_LAST_AVOID_REACH,movestate  );
}

int trap_BotReachabilityArea(vec3_t origin, int testground) {
	return syscall_3( BOTLIB_AI_REACHABILITY_AREA, (nlint)origin, testground );
}

int trap_BotMovementViewTarget(int movestate, void /* struct bot_goal_s */ *goal, int travelflags, float lookahead, vec3_t target) {
	return syscall_6( BOTLIB_AI_MOVEMENT_VIEW_TARGET,
		movestate, (nlint)goal, travelflags,
		PASSFLOAT(lookahead), (nlint)target );
}

int trap_BotPredictVisiblePosition(vec3_t origin, int areanum, void /* struct bot_goal_s */ *goal, int travelflags, vec3_t target) {
	return syscall_6( BOTLIB_AI_PREDICT_VISIBLE_POSITION,
		(nlint)origin, areanum, (nlint)goal, travelflags, (nlint)target );
}

int trap_BotAllocMoveState(void) {
	return syscall_1( BOTLIB_AI_ALLOC_MOVE_STATE );
}

void trap_BotFreeMoveState(int handle) {
	syscall_2( BOTLIB_AI_FREE_MOVE_STATE, handle );
}

void trap_BotInitMoveState(int handle, void /* struct bot_initmove_s */ *initmove) {
	syscall_3( BOTLIB_AI_INIT_MOVE_STATE, handle, (nlint)initmove );
}

int trap_BotChooseBestFightWeapon(int weaponstate, int *inventory) {
	return syscall_3( BOTLIB_AI_CHOOSE_BEST_FIGHT_WEAPON,
		weaponstate, (nlint)inventory );
}

void trap_BotGetWeaponInfo(int weaponstate, int weapon, void /* struct weaponinfo_s */ *weaponinfo) {
	syscall_4( BOTLIB_AI_GET_WEAPON_INFO,
		weaponstate, weapon, (nlint)weaponinfo );
}

int trap_BotLoadWeaponWeights(int weaponstate, char *filename) {
	return syscall_3( BOTLIB_AI_LOAD_WEAPON_WEIGHTS,
		weaponstate, (nlint)filename );
}

int trap_BotAllocWeaponState(void) {
	return syscall_1( BOTLIB_AI_ALLOC_WEAPON_STATE );
}

void trap_BotFreeWeaponState(int weaponstate) {
	syscall_2( BOTLIB_AI_FREE_WEAPON_STATE, weaponstate );
}

void trap_BotResetWeaponState(int weaponstate) {
	syscall_2( BOTLIB_AI_RESET_WEAPON_STATE, weaponstate );
}

int trap_GeneticParentsAndChildSelection(int numranks, float *ranks, int *parent1, int *parent2, int *child) {
	return syscall_6( BOTLIB_AI_GENETIC_PARENTS_AND_CHILD_SELECTION,
		numranks, (nlint)ranks, (nlint)parent1, (nlint)parent2, (nlint)child );
}

int trap_PC_LoadSource( const char *filename ) {
	return syscall_2( BOTLIB_PC_LOAD_SOURCE, (nlint)filename );
}

int trap_PC_FreeSource( int handle ) {
	return syscall_2( BOTLIB_PC_FREE_SOURCE, handle );
}

int trap_PC_ReadToken( int handle, pc_token_t *pc_token ) {
	return syscall_3( BOTLIB_PC_READ_TOKEN, handle, (nlint)pc_token );
}

int trap_PC_SourceFileAndLine( int handle, char *filename, int *line ) {
	return syscall_4( BOTLIB_PC_SOURCE_FILE_AND_LINE,
		handle, (nlint)filename, (nlint)line );
}
