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
// cg_syscalls.c -- this file is only included when building a dll
// cg_syscalls.asm is included instead when building a qvm
#ifdef Q3_VM
#error "Do not use in VM build"
#endif

#include "cg_local.h"

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

void	trap_Print( const char *fmt ) {
	syscall_2( CG_PRINT, (nlint)fmt );
}

void	trap_Error( const char *fmt ) {
	syscall_2( CG_ERROR, (nlint)fmt );
}

int		trap_Milliseconds( void ) {
	return syscall_1( CG_MILLISECONDS ); 
}

void	trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags ) {
	syscall_5( CG_CVAR_REGISTER,
		(nlint)vmCvar, (nlint)varName, (nlint)defaultValue, flags );
}

void	trap_Cvar_Update( vmCvar_t *vmCvar ) {
	syscall_2( CG_CVAR_UPDATE, (nlint)vmCvar );
}

void	trap_Cvar_Set( const char *var_name, const char *value ) {
	syscall_3( CG_CVAR_SET, (nlint)var_name, (nlint)value );
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	syscall_4( CG_CVAR_VARIABLESTRINGBUFFER,
		(nlint)var_name, (nlint)buffer, bufsize );
}

int		trap_Argc( void ) {
	return syscall_1( CG_ARGC );
}

void	trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall_4( CG_ARGV, n, (nlint)buffer, bufferLength );
}

void	trap_Args( char *buffer, int bufferLength ) {
	syscall_3( CG_ARGS, (nlint)buffer, bufferLength );
}

int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return syscall_4( CG_FS_FOPENFILE, (nlint)qpath, (nlint)f, mode );
}

void	trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	syscall_4( CG_FS_READ, (nlint)buffer, len, f );
}

void	trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	syscall_4( CG_FS_WRITE, (nlint)buffer, len, f );
}

void	trap_FS_FCloseFile( fileHandle_t f ) {
	syscall_2( CG_FS_FCLOSEFILE, f );
}

int trap_FS_Seek( fileHandle_t f, long offset, int origin ) {
	return syscall_4( CG_FS_SEEK, f, offset, origin );
}

void	trap_SendConsoleCommand( const char *text ) {
	syscall_2( CG_SENDCONSOLECOMMAND, (nlint)text );
}

void	trap_AddCommand( const char *cmdName ) {
	syscall_2( CG_ADDCOMMAND, (nlint)cmdName );
}

void	trap_RemoveCommand( const char *cmdName ) {
	syscall_2( CG_REMOVECOMMAND, (nlint)cmdName );
}

void	trap_SendClientCommand( const char *s ) {
	syscall_2( CG_SENDCLIENTCOMMAND, (nlint)s );
}

void	trap_UpdateScreen( void ) {
	syscall_1( CG_UPDATESCREEN );
}

void	trap_CM_LoadMap( const char *mapname ) {
	syscall_2( CG_CM_LOADMAP, (nlint)mapname );
}

int		trap_CM_NumInlineModels( void ) {
	return syscall_1( CG_CM_NUMINLINEMODELS );
}

clipHandle_t trap_CM_InlineModel( int index ) {
	return syscall_2( CG_CM_INLINEMODEL, index );
}

clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs ) {
	return syscall_3( CG_CM_TEMPBOXMODEL, (nlint)mins, (nlint)maxs );
}

clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs ) {
	return syscall_3( CG_CM_TEMPCAPSULEMODEL, (nlint)mins, (nlint)maxs );
}

int		trap_CM_PointContents( const vec3_t p, clipHandle_t model ) {
	return syscall_3( CG_CM_POINTCONTENTS, (nlint)p, (nlint)model );
}

int		trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles ) {
	return syscall_5( CG_CM_TRANSFORMEDPOINTCONTENTS,
		(nlint)p, (nlint)model, (nlint)origin, (nlint)angles );
}

void	trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	syscall_8( CG_CM_BOXTRACE,
		(nlint)results, (nlint)start, (nlint)end,
		(nlint)mins, (nlint)maxs, (nlint)model, brushmask );
}

void	trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	syscall_8( CG_CM_CAPSULETRACE,
		(nlint)results, (nlint)start, (nlint)end,
		(nlint)mins, (nlint)maxs, (nlint)model, brushmask );
}

void	trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	syscall_10( CG_CM_TRANSFORMEDBOXTRACE,
		(nlint)results, (nlint)start, (nlint)end,
		(nlint)mins, (nlint)maxs, (nlint)model, brushmask,
		(nlint)origin, (nlint)angles );
}

void	trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	syscall_10( CG_CM_TRANSFORMEDCAPSULETRACE,
		(nlint)results, (nlint)start, (nlint)end,
		(nlint)mins, (nlint)maxs, (nlint)model,
		brushmask, (nlint)origin, (nlint)angles );
}

int		trap_CM_MarkFragments( int numPoints, const vec3_t *points, 
				const vec3_t projection,
				int maxPoints, vec3_t pointBuffer,
				int maxFragments, markFragment_t *fragmentBuffer ) {
	return syscall_8( CG_CM_MARKFRAGMENTS,
		numPoints, (nlint)points, (nlint)projection,
		(nlint)maxPoints, (nlint)pointBuffer,
		(nlint)maxFragments, (nlint)fragmentBuffer );
}

void	trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx ) {
	syscall_5( CG_S_STARTSOUND,
		(nlint)origin, entityNum, (nlint)entchannel, (nlint)sfx );
}

void	trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) {
	syscall_3( CG_S_STARTLOCALSOUND, (nlint)sfx, channelNum );
}

void	trap_S_ClearLoopingSounds( qboolean killall ) {
	syscall_2( CG_S_CLEARLOOPINGSOUNDS, killall );
}

void	trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	syscall_5( CG_S_ADDLOOPINGSOUND,
		entityNum, (nlint)origin, (nlint)velocity, (nlint)sfx );
}

void	trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	syscall_5( CG_S_ADDREALLOOPINGSOUND,
		entityNum, (nlint)origin, (nlint)velocity, (nlint)sfx );
}

void	trap_S_StopLoopingSound( int entityNum ) {
	syscall_2( CG_S_STOPLOOPINGSOUND, entityNum );
}

void	trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin ) {
	syscall_3( CG_S_UPDATEENTITYPOSITION, entityNum, (nlint)origin );
}

void	trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater ) {
	syscall_5( CG_S_RESPATIALIZE, entityNum,
		(nlint)origin, (nlint)axis, inwater );
}

sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed ) {
	return syscall_3( CG_S_REGISTERSOUND, (nlint)sample, compressed );
}

void	trap_S_StartBackgroundTrack( const char *intro, const char *loop ) {
	syscall_3( CG_S_STARTBACKGROUNDTRACK, (nlint)intro, (nlint)loop );
}

void	trap_R_LoadWorldMap( const char *mapname ) {
	syscall_2( CG_R_LOADWORLDMAP, (nlint)mapname );
}

qhandle_t trap_R_RegisterModel( const char *name ) {
	return syscall_2( CG_R_REGISTERMODEL, (nlint)name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	return syscall_2( CG_R_REGISTERSKIN, (nlint)name );
}

qhandle_t trap_R_RegisterShader( const char *name ) {
	return syscall_2( CG_R_REGISTERSHADER, (nlint)name );
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	return syscall_2( CG_R_REGISTERSHADERNOMIP, (nlint)name );
}

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font) {
	syscall_4(CG_R_REGISTERFONT,
		(nlint)fontName, (nlint)pointSize, (nlint)font );
}

void	trap_R_ClearScene( void ) {
	syscall_1( CG_R_CLEARSCENE );
}

void	trap_R_AddRefEntityToScene( const refEntity_t *re ) {
	syscall_2( CG_R_ADDREFENTITYTOSCENE, (nlint)re );
}

void	trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) {
	syscall_4( CG_R_ADDPOLYTOSCENE,
		hShader, numVerts, (nlint)verts );
}

void	trap_R_AddPolysToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts, int num ) {
	syscall_5( CG_R_ADDPOLYSTOSCENE, hShader, numVerts, (nlint)verts, num );
}

int		trap_R_LightForPoint( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir ) {
	return syscall_5( CG_R_LIGHTFORPOINT,
		(nlint)point, (nlint)ambientLight,
		(nlint)directedLight, (nlint)lightDir );
}

void	trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	syscall_6( CG_R_ADDLIGHTTOSCENE,
	(nlint)org, PASSFLOAT(intensity),
	PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b) );
}

void	trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	syscall_6( CG_R_ADDADDITIVELIGHTTOSCENE,
		(nlint)org, PASSFLOAT(intensity),
		PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b) );
}

void	trap_R_RenderScene( const refdef_t *fd ) {
	syscall_2( CG_R_RENDERSCENE, (nlint)fd );
}

void	trap_R_SetColor( const float *rgba ) {
	syscall_2( CG_R_SETCOLOR, (nlint)rgba );
}

void	trap_R_DrawStretchPic( float x, float y, float w, float h, 
							   float s1, float t1, float s2, float t2, qhandle_t hShader ) {
	syscall_10( CG_R_DRAWSTRETCHPIC,
		PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h),
		PASSFLOAT(s1), PASSFLOAT(t1),
		PASSFLOAT(s2), PASSFLOAT(t2), (nlint)hShader );
}

void	trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	syscall_4( CG_R_MODELBOUNDS, (nlint)model, (nlint)mins, (nlint)maxs );
}

int		trap_R_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, 
					   float frac, const char *tagName ) {
	return syscall_7( CG_R_LERPTAG,
		(nlint)tag, (nlint)mod,
		startFrame, endFrame, PASSFLOAT(frac), (nlint)tagName );
}

void	trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset ) {
	syscall_4( CG_R_REMAP_SHADER,
		(nlint)oldShader, (nlint)newShader, (nlint)timeOffset );
}

void		trap_GetGlconfig( glconfig_t *glconfig ) {
	syscall_2( CG_GETGLCONFIG, (nlint)glconfig );
}

void		trap_GetGameState( gameState_t *gamestate ) {
	syscall_2( CG_GETGAMESTATE, (nlint)gamestate );
}

void		trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime ) {
	syscall_3( CG_GETCURRENTSNAPSHOTNUMBER,
		(nlint)snapshotNumber, (nlint)serverTime );
}

qboolean	trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	return syscall_3( CG_GETSNAPSHOT, snapshotNumber, (nlint)snapshot );
}

qboolean	trap_GetServerCommand( int serverCommandNumber ) {
	return syscall_2( CG_GETSERVERCOMMAND, serverCommandNumber );
}

int			trap_GetCurrentCmdNumber( void ) {
	return syscall_1( CG_GETCURRENTCMDNUMBER );
}

qboolean	trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd ) {
	return syscall_3( CG_GETUSERCMD, cmdNumber, (nlint)ucmd );
}

void		trap_SetUserCmdValue( int stateValue, float sensitivityScale ) {
	syscall_3( CG_SETUSERCMDVALUE, stateValue, PASSFLOAT(sensitivityScale) );
}

void		testPrintInt( char *string, int i ) {
	syscall_3( CG_TESTPRINTINT, (nlint)string, i );
}

void		testPrintFloat( char *string, float f ) {
	syscall_3( CG_TESTPRINTFLOAT, (nlint)string, PASSFLOAT(f) );
}

int trap_MemoryRemaining( void ) {
	return syscall_1( CG_MEMORY_REMAINING );
}

qboolean trap_Key_IsDown( int keynum ) {
	return syscall_2( CG_KEY_ISDOWN, keynum );
}

int trap_Key_GetCatcher( void ) {
	return syscall_1( CG_KEY_GETCATCHER );
}

void trap_Key_SetCatcher( int catcher ) {
	syscall_2( CG_KEY_SETCATCHER, catcher );
}

int trap_Key_GetKey( const char *binding ) {
	return syscall_2( CG_KEY_GETKEY, (nlint)binding );
}

int trap_PC_AddGlobalDefine( char *define ) {
	return syscall_2( CG_PC_ADD_GLOBAL_DEFINE, (nlint)define );
}

int trap_PC_LoadSource( const char *filename ) {
	return syscall_2( CG_PC_LOAD_SOURCE, (nlint)filename );
}

int trap_PC_FreeSource( int handle ) {
	return syscall_2( CG_PC_FREE_SOURCE, handle );
}

int trap_PC_ReadToken( int handle, pc_token_t *pc_token ) {
	return syscall_3( CG_PC_READ_TOKEN, handle, (nlint)pc_token );
}

int trap_PC_SourceFileAndLine( int handle, char *filename, int *line ) {
	return syscall_4( CG_PC_SOURCE_FILE_AND_LINE,
		handle, (nlint)filename, (nlint)line );
}

void	trap_S_StopBackgroundTrack( void ) {
	syscall_1( CG_S_STOPBACKGROUNDTRACK );
}

int trap_RealTime(qtime_t *qtime) {
	return syscall_2( CG_REAL_TIME, (nlint)qtime );
}

void trap_SnapVector( float *v ) {
	syscall_2( CG_SNAPVECTOR, (nlint)v );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return syscall_7(CG_CIN_PLAYCINEMATIC,
	(nlint)arg0, xpos, ypos, width, height, bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic(int handle) {
  return syscall_2(CG_CIN_STOPCINEMATIC, handle);
}


// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic (int handle) {
  return syscall_2(CG_CIN_RUNCINEMATIC, handle);
}
 

// draws the current frame
void trap_CIN_DrawCinematic (int handle) {
  syscall_2(CG_CIN_DRAWCINEMATIC, handle);
}
 

// allows you to resize the animation dynamically
void trap_CIN_SetExtents (int handle, int x, int y, int w, int h) {
  syscall_6(CG_CIN_SETEXTENTS, handle, x, y, w, h);
}

/*
qboolean trap_loadCamera( const char *name ) {
	return syscall( CG_LOADCAMERA, name );
}

void trap_startCamera(int time) {
	syscall(CG_STARTCAMERA, time);
}

qboolean trap_getCameraInfo( int time, vec3_t *origin, vec3_t *angles) {
	return syscall( CG_GETCAMERAINFO, time, origin, angles );
}
*/

qboolean trap_GetEntityToken( char *buffer, int bufferSize ) {
	return syscall_3( CG_GET_ENTITY_TOKEN, (nlint)buffer, bufferSize );
}

qboolean trap_R_inPVS( const vec3_t p1, const vec3_t p2 ) {
	return syscall_3( CG_R_INPVS, (nlint)p1, (nlint)p2 );
}
