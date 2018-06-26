#include <vx_kernel32.h>

WINBASEAPI void WINAPI RaiseException(
	DWORD a0, DWORD a1, DWORD a2, const DWORD *a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI ReadDirectoryChangesW(
//	HANDLE a0, PVOID a1, DWORD a2, BOOL a3, DWORD a4,
//	PDWORD a5, LPOVERLAPPED a6, LPOVERLAPPED_COMPLETION_ROUTINE a7)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI ReadEventLogA(
	HANDLE a0, DWORD a1, DWORD a2, PVOID a3, DWORD a4, DWORD *a5, DWORD *a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReadEventLogW(
	HANDLE a0, DWORD a1, DWORD a2, PVOID a3, DWORD a4, DWORD *a5, DWORD *a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReadProcessMemory(
	HANDLE a0, PCVOID a1, PVOID a2, DWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI RegisterEventSourceA(
	LPCSTR a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI RegisterEventSourceW(
	LPCWSTR a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI RegisterWaitForSingleObject(
	PHANDLE a0, HANDLE a1, WAITORTIMERCALLBACK a2, PVOID a3, ULONG a4,
	ULONG a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI RegisterWaitForSingleObjectEx(
	HANDLE a0, WAITORTIMERCALLBACK a1, PVOID a2, ULONG a3, ULONG a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI ReleaseActCtx(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReleaseMutex(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReleaseSemaphore(
	HANDLE a0, LONG a1, LPLONG a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

/*
WINBASEAPI BOOL WINAPI RemoveDirectoryA(
	LPCSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI RemoveDirectoryW(
	LPCWSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/

WINBASEAPI ULONG WINAPI RemoveVectoredExceptionHandler(
	PVOID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReportEventA(
	HANDLE a0, WORD a1, WORD a2, DWORD a3, PSID a4,
	WORD a5, DWORD a6, LPCSTR *a7, PVOID a8)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ReportEventW(
	HANDLE a0, WORD a1, WORD a2, DWORD a3, PSID a4,
	WORD a5, DWORD a6, LPCWSTR *a7, PVOID a8)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ResetEvent(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI ResetWriteWatch(
	LPVOID a0, SIZE_T a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI RestoreLastError(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI ResumeThread(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI RevertToSelf(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

