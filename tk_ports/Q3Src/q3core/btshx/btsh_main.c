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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#include <dlfcn.h>

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"
#include "../renderer/tr_public.h"

// Structure containing functions exported from refresh DLL
refexport_t re;

unsigned  sys_frame_time;

uid_t saved_euid;
qboolean stdin_active = qtrue;

static cvar_t *ttycon = NULL;
static qboolean ttycon_on = qfalse;
static int ttycon_hide = 0;
static int tty_erase;
static int tty_eof;

static field_t tty_con;

// =======================================================================
// General routines
// =======================================================================

// bk001207 
#define MEM_THRESHOLD 64*1024*1024

/*
==================
Sys_LowPhysicalMemory()
==================
*/
qboolean Sys_LowPhysicalMemory()
{
  return qfalse; // bk001207 - FIXME
}

/*
==================
Sys_FunctionCmp
==================
*/
int Sys_FunctionCmp(void *f1, void *f2) {
  return qtrue;
}

/*
==================
Sys_FunctionCheckSum
==================
*/
int Sys_FunctionCheckSum(void *f1) {
  return 0;
}

/*
==================
Sys_MonkeyShouldBeSpanked
==================
*/
int Sys_MonkeyShouldBeSpanked( void ) {
  return 0;
}

void Sys_BeginProfiling( void ) {
}

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( void ) 
{
  IN_Shutdown();
  IN_Init();
}

// never exit without calling this, or your terminal will be left in a pretty bad state
void Sys_ConsoleInputShutdown()
{
  if (ttycon_on)
  {
    Com_Printf("Shutdown tty console\n");
  }
}

// single exit point (regular exit or in case of signal fault)
void Sys_Exit( int ex )
{
  Sys_ConsoleInputShutdown();

  exit(ex);
}


void Sys_Quit (void) {
  CL_Shutdown ();
  Sys_Exit(0);
}

void Sys_Init(void)
{
  Cmd_AddCommand ("in_restart", Sys_In_Restart_f);

  Cvar_Set( "arch", "BJX2 TestKern" );
  Cvar_Set( "username", Sys_GetCurrentUser() );

  IN_Init();

}

void  Sys_Error( const char *error, ...)
{ 
  va_list     argptr;
  char        string[1024];

  CL_Shutdown ();

  va_start (argptr,error);
  vsprintf (string,error,argptr);
  va_end (argptr);
  fprintf(stderr, "Sys_Error: %s\n", string);

  Sys_Exit( 1 ); // bk010104 - use single exit point.
} 

void Sys_Warn (char *warning, ...)
{ 
  va_list     argptr;
  char        string[1024];

  va_start (argptr,warning);
  vsprintf (string,warning,argptr);
  va_end (argptr);

  fprintf(stderr, "Warning: %s", string);
} 

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime (char *path)
{
  struct  stat  buf;

  if (stat (path,&buf) == -1)
    return -1;

  return buf.st_mtime;
}

void Sys_ConsoleInputInit()
{
}

char *Sys_ConsoleInput(void)
{
}

/*****************************************************************************/

/*
=================
Sys_UnloadDll

=================
*/
void Sys_UnloadDll( void *dllHandle )
{
  const char* err; // rb010123 - now const
  if ( !dllHandle )
  {
    Com_Printf("Sys_UnloadDll(NULL)\n");
    return;
  }

  dlclose( dllHandle );

  err = dlerror();

  if ( err != NULL )
    Com_Printf ( "Sys_UnloadGame failed on dlclose: \"%s\"!\n", err );
}


extern char   *FS_BuildOSPath(
	const char *base, const char *game, const char *qpath );

void *Sys_LoadDll( const char *name, char *fqpath ,
                   nlint (**entryPoint)(nlint, ...),
                   nlint (*systemcalls)(nlint, ...) ) 
{
  void *libHandle;
  void  (*dllEntry)( int (*syscallptr)(int, ...) );
  char  curpath[MAX_OSPATH];
  char  fname[MAX_OSPATH];
  char  *basepath;
  char  *homepath;
  char  *pwdpath;
  char  *gamedir;
  char  *fn;
  const char*  err = NULL;
	
	*fqpath = 0;

  // bk001206 - let's have some paranoia
  assert( name );

#ifdef __BJX2__
  snprintf (fname, sizeof(fname), "%sbjx2.dll", name);
#else
#endif

// bk001129 - was RTLD_LAZY 
#define Q_RTLD    RTLD_NOW

  pwdpath = Sys_Cwd();
  basepath = Cvar_VariableString( "fs_basepath" );
  homepath = Cvar_VariableString( "fs_homepath" );
  gamedir = Cvar_VariableString( "fs_game" );

  // pwdpath
  fn = FS_BuildOSPath( pwdpath, gamedir, fname );
  Com_Printf( "Sys_LoadDll(%s)... \n", fn );
  libHandle = dlopen( fn, Q_RTLD );

  if ( !libHandle )
  {
    Com_Printf( "Sys_LoadDll(%s) failed:\n\"%s\"\n", fn, dlerror() );
    // fs_homepath
    fn = FS_BuildOSPath( homepath, gamedir, fname );
    Com_Printf( "Sys_LoadDll(%s)... \n", fn );
    libHandle = dlopen( fn, Q_RTLD );

    if ( !libHandle )
    {
      Com_Printf( "Sys_LoadDll(%s) failed:\n\"%s\"\n", fn, dlerror() );
      // fs_basepath
      fn = FS_BuildOSPath( basepath, gamedir, fname );
      Com_Printf( "Sys_LoadDll(%s)... \n", fn );
      libHandle = dlopen( fn, Q_RTLD );

      if ( !libHandle )
      {
        Com_Printf ( "Sys_LoadDll(%s) failed dlopen() completely!\n", name );
        return NULL;
      } else
        Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn );
    } else
      Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn );
  } else
    Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn ); 

  dllEntry = dlsym( libHandle, "dllEntry" ); 
  *entryPoint = dlsym( libHandle, "vmMain" );
  if ( !*entryPoint || !dllEntry )
  {
    err = dlerror();
    Com_Printf ( "Sys_LoadDll(%s) failed dlsym(vmMain):\n\"%s\" !\n",
		name, err );
    dlclose( libHandle );
    err = dlerror();
    if ( err != NULL )
      Com_Printf ( "Sys_LoadDll(%s) failed dlcose:\n\"%s\"\n", name, err );
    return NULL;
  }
  Com_Printf ( "Sys_LoadDll(%s) found **vmMain** at  %p  \n",
	name, *entryPoint ); // bk001212
  dllEntry( systemcalls );
  Com_Printf ( "Sys_LoadDll(%s) succeeded!\n", name );
  if ( libHandle ) Q_strncpyz ( fqpath , fn , MAX_QPATH ) ;
  return libHandle;
}

/*
========================================================================

BACKGROUND FILE STREAMING

========================================================================
*/

void Sys_InitStreamThread( void ) {
}

void Sys_ShutdownStreamThread( void ) {
}

void Sys_BeginStreamedFile( fileHandle_t f, int readAhead ) {
}

void Sys_EndStreamedFile( fileHandle_t f ) {
}

int Sys_StreamedRead( void *buffer, int size, int count, fileHandle_t f ) {
  return FS_Read( buffer, size * count, f );
}

void Sys_StreamSeek( fileHandle_t f, int offset, int origin ) {
  FS_Seek( f, offset, origin );
}


#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t  eventQue[MAX_QUED_EVENTS];
int   eventHead = 0;
int             eventTail = 0;
byte    sys_packetReceived[MAX_MSGLEN];


void Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr ) {
  sysEvent_t  *ev;

  ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

  if ( eventHead - eventTail >= MAX_QUED_EVENTS )
  {
    Com_Printf("Sys_QueEvent: overflow\n");
    if ( ev->evPtr )
    {
      Z_Free( ev->evPtr );
    }
    eventTail++;
  }

  eventHead++;

  if ( time == 0 )
  {
    time = Sys_Milliseconds();
  }

  ev->evTime = time;
  ev->evType = type;
  ev->evValue = value;
  ev->evValue2 = value2;
  ev->evPtrLength = ptrLength;
  ev->evPtr = ptr;
}

sysEvent_t Sys_GetEvent( void ) {
  sysEvent_t  ev;
  char    *s;
  msg_t   netmsg;
  netadr_t  adr;

  if ( eventHead > eventTail )
  {
    eventTail++;
    return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
  }

  Sys_SendKeyEvents ();

  s = Sys_ConsoleInput();
  if ( s )
  {
    char  *b;
    int   len;

    len = strlen( s ) + 1;
    b = Z_Malloc( len );
    strcpy( b, s );
    Sys_QueEvent( 0, SE_CONSOLE, 0, 0, len, b );
  }

  IN_Frame();

  MSG_Init( &netmsg, sys_packetReceived, sizeof( sys_packetReceived ) );
  if ( Sys_GetPacket ( &adr, &netmsg ) )
  {
    netadr_t    *buf;
    int       len;

    len = sizeof( netadr_t ) + netmsg.cursize;
    buf = Z_Malloc( len );
    *buf = adr;
    memcpy( buf+1, netmsg.data, netmsg.cursize );
    Sys_QueEvent( 0, SE_PACKET, 0, 0, len, buf );
  }

  if ( eventHead > eventTail )
  {
    eventTail++;
    return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
  }

  memset( &ev, 0, sizeof( ev ) );
  ev.evTime = Sys_Milliseconds();

  return ev;
}


qboolean Sys_CheckCD( void ) {
  return qtrue;
}

void Sys_AppActivate (void)
{
}

char *Sys_GetClipboardData(void)
{
  return NULL;
}

void  Sys_Print( const char *msg )
{
  if (ttycon_on)
  {
    tty_Hide();
  }
  fputs(msg, stderr);
  if (ttycon_on)
  {
    tty_Show();
  }
}

void Sys_PrintBinVersion( const char* name )
{
  char* date = __DATE__;
  char* time = __TIME__;
  char* sep = "==============================================================";
  fprintf( stdout, "\n\n%s\n", sep );
  fprintf( stdout, "Linux Quake3 Full Executable  [%s %s]\n", date, time );  
  fprintf( stdout, " local install: %s\n", name );
  fprintf( stdout, "%s\n\n", sep );
}

void Sys_ParseArgs( int argc, char* argv[] )
{

  if ( argc==2 )
  {
    if ( (!strcmp( argv[1], "--version" ))
         || ( !strcmp( argv[1], "-v" )) )
    {
      Sys_PrintBinVersion( argv[0] );
      Sys_Exit(0);
    }
  }
}

#include "../client/client.h"
extern clientStatic_t cls;

int main ( int argc, char* argv[] )
{
  // int 	oldtime, newtime; // bk001204 - unused
  int   len, i;
  char  *cmdline;
  void Sys_SetDefaultCDPath(const char *path);

  // go back to real user for config loads
  saved_euid = geteuid();
  seteuid(getuid());

  Sys_ParseArgs( argc, argv );  // bk010104 - added this for support

  Sys_SetDefaultCDPath(argv[0]);

  // merge the command line, this is kinda silly
  for (len = 1, i = 1; i < argc; i++)
    len += strlen(argv[i]) + 1;
  cmdline = malloc(len);
  *cmdline = 0;
  for (i = 1; i < argc; i++)
  {
    if (i > 1)
      strcat(cmdline, " ");
    strcat(cmdline, argv[i]);
  }

  // bk000306 - clear queues
  memset( &eventQue[0], 0, MAX_QUED_EVENTS*sizeof(sysEvent_t) ); 
  memset( &sys_packetReceived[0], 0, MAX_MSGLEN*sizeof(byte) );

  Com_Init(cmdline);
  NET_Init();

  Sys_ConsoleInputInit();

  while (1)
  {
    Com_Frame ();
  }
}
