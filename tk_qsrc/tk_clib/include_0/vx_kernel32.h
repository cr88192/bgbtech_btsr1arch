// #define WINAPI __stdcall
#define WINAPI __cdecl
#define DECLSPEC_EXPORT __declspec(dllexport)

#define WINBASEAPI DECLSPEC_EXPORT

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0700
#endif

#include <stdarg.h>

#include <vxcore.h>

#include <windef.h>
#include <wincon.h>
#include <winbase.h>
#include <tlhelp32.h>
#include <winnls.h>
#include <winver.h>
#include <winnt.h>

typedef struct _TEB TEB, *PTEB;

struct _TEB {
	PVOID SehFrame;
	PVOID StackTop;
	PVOID StackBottom;
	PVOID TibSubsystem;
	PVOID FiberData;
	PVOID ArbitraryData;
	PVOID AddressOfTEB;
	/* End Of Subsys Independent */
	PVOID EnvPointer;
	DWORD ProcessID;
	DWORD ThreadID;
	//
	DWORD ActiveRPCHandle;
	PVOID TlsArray;
	PVOID AddressOfPEB;
	DWORD LastError;
	DWORD CountOwnerCriticalSections;
	DWORD AddressOfCSRClient;
	DWORD ThreadInformation;
	BYTE Reserved1[1884];
//	BYTE Reserved1[1912];
//	BYTE Reserved1[1952];
	PVOID Reserved2[412];
	PVOID TlsSlots[64];		//0xE10
	BYTE Reserved3[8];		//0xF10
	PVOID Reserved4[26];	
	PVOID ReservedForOle;
	PVOID Reserved5[4];
	PVOID TlsExpansionSlots;
};
