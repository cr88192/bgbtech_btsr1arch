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
// sys_null.h -- null system driver to aid porting efforts

#include <errno.h>
#include <stdio.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
// #include <sys/time.h>

#include <dlfcn.h>

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"

int			sys_curtime;


// #define	MAX_FOUND_FILES	0x1000
#define	MAX_FOUND_FILES	0x100

qboolean	Sys_GetPacket ( netadr_t *net_from, msg_t *net_message );

short BigShort(short l)
{
	return(__int16_bswap_s(l));
}

short LittleShort(short l)
{
	return(l);
}

int	BigLong (int l)
{
	return(__int32_bswap_s(l));
}

int	LittleLong (int l)
{
	return(l);
}

qint64  BigLong64 (qint64 l)
{
	return(__int64_bswap(l));
}

qint64  LittleLong64 (qint64 l)
{
	return(l);
}

float	BigFloat (float l)
{
	float f;
	*(int *)(&f)=BigLong(*(int *)(&l));
	return(f);
}

float	LittleFloat (float l)
{
	return(l);
}



//===================================================================

void Sys_BeginStreamedFile( fileHandle_t f, int readAhead )
{
}

void Sys_EndStreamedFile( fileHandle_t f )
{
}

int Sys_StreamedRead( void *buffer, int size, int count, fileHandle_t f )
{
//	return fread( buffer, size, count, f );
	return(FS_Read2(buffer, size*count, f));
}

void Sys_StreamSeek( fileHandle_t f, int offset, int origin ) {
//	fseek( f, offset, origin );
	return(FS_Seek(f, offset, origin));
}


//===================================================================


void Sys_mkdir ( const char *path )
{
}

void Sys_Error (char *error, ...)
{
	va_list		argptr;

	printf ("Sys_Error: ");	
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");

	exit (1);
}

void Sys_Print (char *error, ...)
{
	va_list		argptr;

//	printf ("Sys_Error: ");	
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");
}

void Sys_Quit (void)
{
	exit (0);
}

void	Sys_UnloadGame (void)
{
}

void	*Sys_GetGameAPI (void *parms)
{
	return NULL;
}

char *Sys_GetClipboardData( void )
{
	return NULL;
}

int TK_GetTimeMs();

int		Sys_Milliseconds (void)
{
	return(TK_GetTimeMs());
//	return 0;
}

void	Sys_Mkdir (char *path)
{
}

char	*Sys_FindFirst (char *path, unsigned musthave, unsigned canthave) {
	return NULL;
}

char	*Sys_FindNext (unsigned musthave, unsigned canthave) {
	return NULL;
}

void	Sys_FindClose (void) {
}

void	Sys_Init (void) {
}


void	Sys_EarlyOutput( char *string ) {
	printf( "%s", string );
}

void Sys_ListFilteredFiles(
	const char *basedir, char *subdirs,
	char *filter, char **list, int *numfiles )
{
	char		search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
	char		filename[MAX_OSPATH];
	DIR			*fdir;
	struct dirent *d;
//	struct stat st;

	if ( *numfiles >= MAX_FOUND_FILES - 1 ) {
		return;
	}

	if (strlen(subdirs)) {
		Com_sprintf( search, sizeof(search), "%s/%s", basedir, subdirs );
	}
	else {
		Com_sprintf( search, sizeof(search), "%s", basedir );
	}

	if ((fdir = opendir(search)) == NULL) {
		return;
	}

	while ((d = readdir(fdir)) != NULL)
	{
		Com_sprintf(filename, sizeof(filename), "%s/%s", search, d->d_name);

#if 0
		if (stat(filename, &st) == -1)
			continue;

		if (st.st_mode & S_IFDIR) {
			if (Q_stricmp(d->d_name, ".") && Q_stricmp(d->d_name, "..")) {
				if (strlen(subdirs)) {
					Com_sprintf( newsubdirs, sizeof(newsubdirs),
						"%s/%s", subdirs, d->d_name);
				}
				else {
					Com_sprintf( newsubdirs, sizeof(newsubdirs),
						"%s", d->d_name);
				}
				Sys_ListFilteredFiles( basedir, newsubdirs,
					filter, list, numfiles );
			}
		}
#endif

		if ( *numfiles >= MAX_FOUND_FILES - 1 ) {
			break;
		}
		Com_sprintf( filename, sizeof(filename), "%s/%s", subdirs, d->d_name );
		if (!Com_FilterPath( filter, filename, qfalse ))
			continue;
		list[ *numfiles ] = CopyString( filename );
		(*numfiles)++;
	}

	closedir(fdir);
}

char **Sys_ListFiles(
	const char *directory, const char *extension,
	char *filter, int *numfiles, qboolean wantsubs )
{
	struct dirent *d;
	// char *p; // bk001204 - unused
	DIR		*fdir;
	qboolean dironly = wantsubs;
	char		search[MAX_OSPATH];
	int			nfiles;
	char		**listCopy;
	char		*list[MAX_FOUND_FILES];
	//int			flag; // bk001204 - unused
	int			i;
//	struct stat st;

	int			extLen;

	if (filter)
	{
		nfiles = 0;
		Sys_ListFilteredFiles( directory, "", filter, list, &nfiles );

		list[ nfiles ] = 0;
		*numfiles = nfiles;

		if (!nfiles)
			return NULL;

		listCopy = Z_Malloc( ( nfiles + 1 ) * sizeof( *listCopy ) );
		for ( i = 0 ; i < nfiles ; i++ ) {
			listCopy[i] = list[i];
		}
		listCopy[i] = NULL;

		return listCopy;
	}

	if ( !extension)
		extension = "";

	if ( extension[0] == '/' && extension[1] == 0 )
	{
		extension = "";
		dironly = qtrue;
	}

	extLen = strlen( extension );
	
	// search
	nfiles = 0;

	if ((fdir = opendir(directory)) == NULL)
	{
		tk_printf("Sys_ListFiles: Opendir fail %s\n", directory);
		*numfiles = 0;
		return NULL;
	}

	while ((d = readdir(fdir)) != NULL)
	{
		Com_sprintf(search, sizeof(search), "%s/%s", directory, d->d_name);

		tk_printf("Sys_ListFiles: Check %s\n", search);

#if 0
		if (stat(search, &st) == -1)
			continue;

		if ((dironly && !(st.st_mode & S_IFDIR)) ||
			(!dironly && (st.st_mode & S_IFDIR)))
			continue;
#endif

		if (*extension) {
			if ( strlen( d->d_name ) < strlen( extension ) ||
				Q_stricmp( 
					d->d_name + strlen( d->d_name ) - strlen( extension ),
					extension ) )
			{
				continue; // didn't match
			}
		}

		if ( nfiles == MAX_FOUND_FILES - 1 )
			break;
		list[ nfiles ] = CopyString( d->d_name );
		nfiles++;
	}

	list[ nfiles ] = 0;

	closedir(fdir);

	// return a copy of the list
	*numfiles = nfiles;

	if ( !nfiles ) {
		return NULL;
	}

	listCopy = Z_Malloc( ( nfiles + 1 ) * sizeof( *listCopy ) );
	for ( i = 0 ; i < nfiles ; i++ ) {
		listCopy[i] = list[i];
	}
	listCopy[i] = NULL;

	return listCopy;
}

void	Sys_FreeFileList( char **list )
{
	int i;

	if ( !list ) {
		return;
	}

	for ( i = 0 ; list[i] ; i++ ) {
		Z_Free( list[i] );
	}

	Z_Free( list );
}

void Sys_ShowConsole( int visLevel, qboolean quitOnClose )
{
}

char *Sys_Cwd( void ) 
{
	static char tcwd[MAX_OSPATH];

	memset(tcwd, 0, MAX_OSPATH);
//	getcwd( tcwd, sizeof( cwd ) - 1 );
	getcwd( tcwd, MAX_OSPATH - 1 );
//	tcwd[MAX_OSPATH-1] = 0;

	return tcwd;
}

char *Sys_DefaultHomePath(void)
{
	return(NULL);
//	return(Sys_Cwd());
}

char *Sys_DefaultInstallPath(void)
{
	return(Sys_Cwd());
}

char *Sys_DefaultCDPath(void)
{
	return(Sys_Cwd());
}

qboolean Sys_CheckCD( void )
{
	return qtrue;
}

#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t  eventQue[MAX_QUED_EVENTS];
int   eventHead = 0;
int             eventTail = 0;
byte    sys_packetReceived[MAX_MSGLEN];

void Sys_QueEvent(
	int time, sysEventType_t type, int value, int value2,
	int ptrLength, void *ptr )
{
	sysEvent_t  *ev;

	ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

	// bk000305 - was missing
	if ( eventHead - eventTail >= MAX_QUED_EVENTS )
	{
		Com_Printf("Sys_QueEvent: overflow\n");
		// we are discarding an event, but don't leak memory
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

sysEvent_t Sys_GetEvent( void )
{
	sysEvent_t  ev;
	char    *s;
	msg_t   netmsg;
	netadr_t  adr;

#if 1
	// return if we have data
	if ( eventHead > eventTail )
	{
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// pump the message loop
	// in vga this calls KBD_Update, under X, it calls GetEvent
	Sys_SendKeyEvents ();
#endif

#if 0
	// check for console commands
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
#endif

#if 1
	// check for other input devices
	IN_Frame();

	// check for network packets
	MSG_Init( &netmsg, sys_packetReceived, sizeof( sys_packetReceived ) );
	if ( Sys_GetPacket ( &adr, &netmsg ) )
	{
		netadr_t    *buf;
		int       len;

		// copy out to a seperate buffer for qeueing
		len = sizeof( netadr_t ) + netmsg.cursize;
		buf = Z_Malloc( len );
		*buf = adr;
		memcpy( buf+1, netmsg.data, netmsg.cursize );
		Sys_QueEvent( 0, SE_PACKET, 0, 0, len, buf );
	}

	// return if we have data
	if ( eventHead > eventTail )
	{
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}
#endif

	// create an empty event to return

	memset( &ev, 0, sizeof( ev ) );
	ev.evTime = Sys_Milliseconds();

	return ev;
}

void Sys_UnloadDll( void *dllHandle ) {
	// bk001206 - verbose error reporting
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


extern char   *FS_BuildOSPath( const char *base, const char *game, const char *qpath );

void *Sys_LoadDll( const char *name, char *fqpath ,
									 int (**entryPoint)(int, ...),
									 int (*systemcalls)(int, ...) ) 
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
	
	if(!systemcalls)
		__debugbreak();

	*fqpath = 0;

	// bk001206 - let's have some paranoia
	assert( name );

	getcwd(curpath, sizeof(curpath));
	snprintf (fname, sizeof(fname), "%sbjx2.dll", name);

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
#ifndef NDEBUG // bk001206 - in debug abort on failure
				Com_Error ( ERR_FATAL, "Sys_LoadDll(%s) failed dlopen() completely!\n", name  );
#else
				Com_Printf ( "Sys_LoadDll(%s) failed dlopen() completely!\n", name );
#endif
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
	Com_Printf ( "Sys_LoadDll(%s) found **vmMain** at  %p  \n", name, *entryPoint ); // bk001212
	dllEntry( systemcalls );
	
//  __debugbreak();
	
	Com_Printf ( "Sys_LoadDll(%s) succeeded!\n", name );
	if ( libHandle ) Q_strncpyz ( fqpath , fn , MAX_QPATH ) ;		// added 7/20/02 by T.Ray
	return libHandle;
}

void Sys_SnapVector( float *v )
{
	v[0] = (int)(v[0]);
	v[1] = (int)(v[1]);
	v[2] = (int)(v[2]);
}

void Sys_BeginProfiling( void ) {
}

qboolean Sys_LowPhysicalMemory()
{
	return qfalse;
}



void main (int argc, char **argv)
{
	int   len, i;
	char  *cmdline;

	// merge the command line, this is kinda silly
	for (len = 1, i = 1; i < argc; i++)
		len += strlen(argv[i]) + 3;
	cmdline = malloc(len);
	*cmdline = 0;
	for (i = 1; i < argc; i++)
	{
		if (i > 1)
			strcat(cmdline, " ");
		strcat(cmdline, argv[i]);
	}

	Com_Init(cmdline);
//	NET_Init();

	Cvar_Set("r_vertexLight", "1");

//	Com_Init (argc, argv);

	while (1) {
		Com_Frame( );
	}
}


