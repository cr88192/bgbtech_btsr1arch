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
#include "ui_local.h"

// this file is only included when building a dll
// syscalls.asm is included instead when building a qvm
#ifdef Q3_VM
#error "Do not use in VM build"
#endif

//static int (QDECL *syscall)( int arg, ... ) = (int (QDECL *)( int, ...))-1;
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

//void dllEntry( int (QDECL *syscallptr)( int arg,... ) ) {
VM_EXPORT void dllEntry( nlint (QDECL *syscallptr)( nlint *arg ) )
{
	if(!syscallptr)
		__debugbreak();
	syscall_i = syscallptr;
	if(!syscall_i)
		__debugbreak();
}

int PASSFLOAT( float x ) {
	float	floatTemp;
	floatTemp = x;
	return *(int *)&floatTemp;
}

void trap_Print( const char *string ) {
	syscall_2( UI_PRINT, (nlint)string );
}

void trap_Error( const char *string ) {
	syscall_2( UI_ERROR, (nlint)string );
}

int trap_Milliseconds( void ) {
	return syscall_1( UI_MILLISECONDS ); 
}

void trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags ) {
	syscall_5( UI_CVAR_REGISTER,
		(nlint)cvar, (nlint)var_name, (nlint)value, flags );
}

void trap_Cvar_Update( vmCvar_t *cvar ) {
	syscall_2( UI_CVAR_UPDATE, (nlint)cvar );
}

void trap_Cvar_Set( const char *var_name, const char *value ) {
	syscall_3( UI_CVAR_SET, (nlint)var_name, (nlint)value );
}

float trap_Cvar_VariableValue( const char *var_name ) {
	nlint temp;
	temp = syscall_2( UI_CVAR_VARIABLEVALUE, (nlint)var_name );
	return (*(float*)&temp);
}

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize ) {
	syscall_4( UI_CVAR_VARIABLESTRINGBUFFER,
		(nlint)var_name, (nlint)buffer, (nlint)bufsize );
}

void trap_Cvar_SetValue( const char *var_name, float value ) {
	syscall_3( UI_CVAR_SETVALUE, (nlint)var_name, PASSFLOAT( value ) );
}

void trap_Cvar_Reset( const char *name ) {
	syscall_2( UI_CVAR_RESET, (nlint)name ); 
}

void trap_Cvar_Create( const char *var_name, const char *var_value, int flags ) {
	syscall_4( UI_CVAR_CREATE,
		(nlint)var_name, (nlint)var_value, (nlint)flags );
}

void trap_Cvar_InfoStringBuffer( int bit, char *buffer, int bufsize ) {
	syscall_4( UI_CVAR_INFOSTRINGBUFFER,
		(nlint)bit, (nlint)buffer, (nlint)bufsize );
}

int trap_Argc( void ) {
	return syscall_1( UI_ARGC );
}

void trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall_4( UI_ARGV,
		(nlint)n, (nlint)buffer, (nlint)bufferLength );
}

void trap_Cmd_ExecuteText( int exec_when, const char *text ) {
	syscall_3( UI_CMD_EXECUTETEXT,
		(nlint)exec_when, (nlint)text );
}

int trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return syscall_4( UI_FS_FOPENFILE,
		(nlint)qpath, (nlint)f, (nlint)mode );
}

void trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	syscall_4( UI_FS_READ,
		(nlint)buffer, (nlint)len, (nlint)f );
}

void trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	syscall_4( UI_FS_WRITE, (nlint)buffer, (nlint)len, (nlint)f );
}

void trap_FS_FCloseFile( fileHandle_t f ) {
	syscall_2( UI_FS_FCLOSEFILE, (nlint)f );
}

int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) {
	return syscall_5( UI_FS_GETFILELIST,
		(nlint)path, (nlint)extension, (nlint)listbuf, (nlint)bufsize );
}

int trap_FS_Seek( fileHandle_t f, long offset, int origin ) {
	return syscall_4( UI_FS_SEEK, (nlint)f, (nlint)offset, (nlint)origin );
}

qhandle_t trap_R_RegisterModel( const char *name ) {
	return syscall_2( UI_R_REGISTERMODEL, (nlint)name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	return syscall_2( UI_R_REGISTERSKIN, (nlint)name );
}

void trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font) {
	syscall_4( UI_R_REGISTERFONT,
		(nlint)fontName, (nlint)pointSize, (nlint)font );
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	return syscall_2( UI_R_REGISTERSHADERNOMIP, (nlint)name );
}

void trap_R_ClearScene( void ) {
	syscall_1( UI_R_CLEARSCENE );
}

void trap_R_AddRefEntityToScene( const refEntity_t *re ) {
	syscall_2( UI_R_ADDREFENTITYTOSCENE, (nlint)re );
}

void trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) {
	syscall_4( UI_R_ADDPOLYTOSCENE,
		(nlint)hShader, (nlint)numVerts, (nlint)verts );
}

void trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {
	syscall_6( UI_R_ADDLIGHTTOSCENE,
		(nlint)org, PASSFLOAT(intensity), PASSFLOAT(r),
		PASSFLOAT(g), PASSFLOAT(b) );
}

void trap_R_RenderScene( const refdef_t *fd ) {
	syscall_2( UI_R_RENDERSCENE, (nlint)fd );
}

void trap_R_SetColor( const float *rgba ) {
	syscall_2( UI_R_SETCOLOR, (nlint)rgba );
}

void trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader ) {
	syscall_10( UI_R_DRAWSTRETCHPIC,
		PASSFLOAT(x), PASSFLOAT(y),
		PASSFLOAT(w), PASSFLOAT(h),
		PASSFLOAT(s1), PASSFLOAT(t1),
		PASSFLOAT(s2), PASSFLOAT(t2),
		(nlint)hShader );
}

void	trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs ) {
	syscall_4( UI_R_MODELBOUNDS, (nlint)model, (nlint)mins, (nlint)maxs );
}

void trap_UpdateScreen( void ) {
	syscall_1( UI_UPDATESCREEN );
}

int trap_CM_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName ) {
	return syscall_7( UI_CM_LERPTAG,
		(nlint)tag, (nlint)mod, (nlint)startFrame, (nlint)endFrame,
		PASSFLOAT(frac), (nlint)tagName );
}

void trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) {
	syscall_3( UI_S_STARTLOCALSOUND, (nlint)sfx, (nlint)channelNum );
}

sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed ) {
	return syscall_3( UI_S_REGISTERSOUND, (nlint)sample, (nlint)compressed );
}

void trap_Key_KeynumToStringBuf( int keynum, char *buf, int buflen ) {
	syscall_4( UI_KEY_KEYNUMTOSTRINGBUF,
		(nlint)keynum, (nlint)buf, (nlint)buflen );
}

void trap_Key_GetBindingBuf( int keynum, char *buf, int buflen ) {
	syscall_4( UI_KEY_GETBINDINGBUF,
		(nlint)keynum, (nlint)buf, (nlint)buflen );
}

void trap_Key_SetBinding( int keynum, const char *binding ) {
	syscall_3( UI_KEY_SETBINDING, (nlint)keynum, (nlint)binding );
}

qboolean trap_Key_IsDown( int keynum ) {
	return syscall_2( UI_KEY_ISDOWN, (nlint)keynum );
}

qboolean trap_Key_GetOverstrikeMode( void ) {
	return syscall_1( UI_KEY_GETOVERSTRIKEMODE );
}

void trap_Key_SetOverstrikeMode( qboolean state ) {
	syscall_2( UI_KEY_SETOVERSTRIKEMODE, (nlint)state );
}

void trap_Key_ClearStates( void ) {
	syscall_1( UI_KEY_CLEARSTATES );
}

int trap_Key_GetCatcher( void ) {
	return syscall_1( UI_KEY_GETCATCHER );
}

void trap_Key_SetCatcher( int catcher ) {
	syscall_2( UI_KEY_SETCATCHER, (nlint)catcher );
}

void trap_GetClipboardData( char *buf, int bufsize ) {
	syscall_3( UI_GETCLIPBOARDDATA, (nlint)buf, (nlint)bufsize );
}

void trap_GetClientState( uiClientState_t *state ) {
	syscall_2( UI_GETCLIENTSTATE, (nlint)state );
}

void trap_GetGlconfig( glconfig_t *glconfig ) {
	syscall_2( UI_GETGLCONFIG, (nlint)glconfig );
}

int trap_GetConfigString( int index, char* buff, int buffsize ) {
	return syscall_4( UI_GETCONFIGSTRING,
		(nlint)index, (nlint)buff, (nlint)buffsize );
}

int	trap_LAN_GetServerCount( int source ) {
	return syscall_2( UI_LAN_GETSERVERCOUNT, (nlint)source );
}

void trap_LAN_GetServerAddressString( int source, int n, char *buf, int buflen ) {
	syscall_5( UI_LAN_GETSERVERADDRESSSTRING,
		(nlint)source, (nlint)n, (nlint)buf, (nlint)buflen );
}

void trap_LAN_GetServerInfo( int source, int n, char *buf, int buflen ) {
	syscall_5( UI_LAN_GETSERVERINFO,
		(nlint)source, (nlint)n, (nlint)buf, (nlint)buflen );
}

int trap_LAN_GetServerPing( int source, int n ) {
	return syscall_3( UI_LAN_GETSERVERPING, (nlint)source, (nlint)n );
}

int trap_LAN_GetPingQueueCount( void ) {
	return syscall_1( UI_LAN_GETPINGQUEUECOUNT );
}

int trap_LAN_ServerStatus( const char *serverAddress, char *serverStatus, int maxLen ) {
	return syscall_4( UI_LAN_SERVERSTATUS,
		(nlint)serverAddress, (nlint)serverStatus, (nlint)maxLen );
}

void trap_LAN_SaveCachedServers() {
	syscall_1( UI_LAN_SAVECACHEDSERVERS );
}

void trap_LAN_LoadCachedServers() {
	syscall_1( UI_LAN_LOADCACHEDSERVERS );
}

void trap_LAN_ResetPings(int n) {
	syscall_2( UI_LAN_RESETPINGS, n );
}

void trap_LAN_ClearPing( int n ) {
	syscall_2( UI_LAN_CLEARPING, n );
}

void trap_LAN_GetPing( int n, char *buf, int buflen, int *pingtime ) {
	syscall_5( UI_LAN_GETPING,
		(nlint)n, (nlint)buf, (nlint)buflen, (nlint)pingtime );
}

void trap_LAN_GetPingInfo( int n, char *buf, int buflen ) {
	syscall_4( UI_LAN_GETPINGINFO,
		(nlint)n, (nlint)buf, (nlint)buflen );
}

void trap_LAN_MarkServerVisible( int source, int n, qboolean visible ) {
	syscall_4( UI_LAN_MARKSERVERVISIBLE,
		(nlint)source, (nlint)n, (nlint)visible );
}

int trap_LAN_ServerIsVisible( int source, int n) {
	return syscall_3( UI_LAN_SERVERISVISIBLE,
		(nlint)source, (nlint)n );
}

qboolean trap_LAN_UpdateVisiblePings( int source ) {
	return syscall_2( UI_LAN_UPDATEVISIBLEPINGS,
		(nlint)source );
}

int trap_LAN_AddServer(int source, const char *name, const char *addr) {
	return syscall_4( UI_LAN_ADDSERVER,
		(nlint)source, (nlint)name, (nlint)addr );
}

void trap_LAN_RemoveServer(int source, const char *addr) {
	syscall_3( UI_LAN_REMOVESERVER,
		(nlint)source, (nlint)addr );
}

int trap_LAN_CompareServers( int source, int sortKey, int sortDir, int s1, int s2 ) {
	return syscall_6( UI_LAN_COMPARESERVERS,
		(nlint)source, (nlint)sortKey, (nlint)sortDir, (nlint)s1, (nlint)s2 );
}

int trap_MemoryRemaining( void ) {
	return syscall_1( UI_MEMORY_REMAINING );
}

void trap_GetCDKey( char *buf, int buflen ) {
	syscall_3( UI_GET_CDKEY, (nlint)buf, (nlint)buflen );
}

void trap_SetCDKey( char *buf ) {
	syscall_2( UI_SET_CDKEY, (nlint)buf );
}

int trap_PC_AddGlobalDefine( char *define ) {
	return syscall_2( UI_PC_ADD_GLOBAL_DEFINE, (nlint)define );
}

int trap_PC_LoadSource( const char *filename ) {
	return syscall_2( UI_PC_LOAD_SOURCE, (nlint)filename );
}

int trap_PC_FreeSource( int handle ) {
	return syscall_2( UI_PC_FREE_SOURCE, (nlint)handle );
}

int trap_PC_ReadToken( int handle, pc_token_t *pc_token ) {
	return syscall_3( UI_PC_READ_TOKEN, (nlint)handle, (nlint)pc_token );
}

int trap_PC_SourceFileAndLine( int handle, char *filename, int *line ) {
	return syscall_4( UI_PC_SOURCE_FILE_AND_LINE,
		(nlint)handle, (nlint)filename, (nlint)line );
}

void trap_S_StopBackgroundTrack( void ) {
	syscall_1( UI_S_STOPBACKGROUNDTRACK );
}

void trap_S_StartBackgroundTrack( const char *intro, const char *loop) {
	syscall_3( UI_S_STARTBACKGROUNDTRACK, (nlint)intro, (nlint)loop );
}

int trap_RealTime(qtime_t *qtime) {
	return syscall_2( UI_REAL_TIME, (nlint)qtime );
}

// this returns a handle.  arg0 is the name in the format "idlogo.roq", set arg1 to NULL, alteredstates to qfalse (do not alter gamestate)
int trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits) {
  return syscall_7(UI_CIN_PLAYCINEMATIC,
	(nlint)arg0, (nlint)xpos, (nlint)ypos,
	(nlint)width, (nlint)height, (nlint)bits);
}
 
// stops playing the cinematic and ends it.  should always return FMV_EOF
// cinematics must be stopped in reverse order of when they are started
e_status trap_CIN_StopCinematic(int handle) {
  return syscall_2(UI_CIN_STOPCINEMATIC, (nlint)handle);
}


// will run a frame of the cinematic but will not draw it.  Will return FMV_EOF if the end of the cinematic has been reached.
e_status trap_CIN_RunCinematic (int handle) {
  return syscall_2(UI_CIN_RUNCINEMATIC, (nlint)handle);
}
 

// draws the current frame
void trap_CIN_DrawCinematic (int handle) {
  syscall_2(UI_CIN_DRAWCINEMATIC, (nlint)handle);
}
 

// allows you to resize the animation dynamically
void trap_CIN_SetExtents (int handle, int x, int y, int w, int h) {
  syscall_6(UI_CIN_SETEXTENTS, handle, x, y, w, h);
}


void	trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset ) {
	syscall_4( UI_R_REMAP_SHADER,
		(nlint)oldShader, (nlint)newShader, (nlint)timeOffset );
}

qboolean trap_VerifyCDKey( const char *key, const char *chksum) {
	return syscall_3( UI_VERIFY_CDKEY, (nlint)key, (nlint)chksum);
}

void trap_SetPbClStatus( int status ) {
	syscall_2( UI_SET_PBCLSTATUS, status );
}
