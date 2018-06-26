#include <vx_kernel32.h>

WINBASEAPI void WINAPI FatalAppExitA(
	UINT a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI FatalAppExitW(
	UINT a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI FatalExit(
	int a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

/*
WINBASEAPI BOOL WINAPI FindActCtxSectionGuid(
	DWORD a0, const GUID *a1, ULONG a2, const GUID *a3, 
	PACTCTX_SECTION_KEYED_DATA a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindActCtxSectionStringA(
	DWORD a0, const GUID *a1, ULONG a2, LPCSTR a3,
	PACTCTX_SECTION_KEYED_DATA a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindActCtxSectionStringW(
	DWORD a0, const GUID *a1, ULONG a2, LPCWSTR a3, 
	PACTCTX_SECTION_KEYED_DATA a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/

WINBASEAPI ATOM WINAPI FindAtomA(
	LPCSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI ATOM WINAPI FindAtomW(
	LPCWSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindClose(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindCloseChangeNotification(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstChangeNotificationA(
	LPCSTR a0, BOOL a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstChangeNotificationW(
	LPCWSTR a0, BOOL a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindFirstFreeAce(
	PACL a0, PVOID *a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstVolumeA(
	LPCSTR a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstVolumeW(
	LPCWSTR a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstVolumeMountPointA(
	LPSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI FindFirstVolumeMountPointW(
	LPWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FindNextChangeNotification(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI FindNextVolumeA(
//	HANDLE a0, LPCSTR a1, DWORD a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI FindNextVolumeW(
//	HANDLE a0, LPWSTR a1, DWORD a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI FindNextVolumeMountPointA(
//	HANDLE a0, LPSTR a1, DWORD a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI FindNextVolumeMountPointW(
//	HANDLE a0, LPWSTR a1, DWORD a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI FindVolumeClose(
//	HANDLE a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI FindVolumeMountPointClose(
//	HANDLE a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI HRSRC WINAPI FindResourceA(
	HMODULE a0, LPCSTR a1, LPCSTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HRSRC WINAPI FindResourceW(
	HINSTANCE a0, LPCWSTR a1, LPCWSTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HRSRC WINAPI FindResourceExA(
	HINSTANCE a0, LPCSTR a1, LPCSTR a2, WORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HRSRC WINAPI FindResourceExW(
	HINSTANCE a0, LPCWSTR a1, LPCWSTR a2, WORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FlushInstructionCache(
	HANDLE a0, PCVOID a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI FormatMessageA(
	DWORD a0, PCVOID a1, DWORD a2, DWORD a3, LPSTR a4, DWORD a5, va_list *a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI FormatMessageW(
	DWORD a0, PCVOID a1, DWORD a2, DWORD a3, LPWSTR a4, DWORD a5, va_list *a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FreeEnvironmentStringsA(
	LPSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FreeEnvironmentStringsW(
	LPWSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FreeLibrary(
	HMODULE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

DECLSPEC_NORETURN WINBASEAPI void WINAPI FreeLibraryAndExitThread(
	HMODULE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
	while(1);
}

WINBASEAPI BOOL WINAPI FreeResource(
	HGLOBAL a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI FreeSid(
	PSID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
