#include <vx_kernel32.h>

WINBASEAPI BOOL WINAPI PeekNamedPipe(
	HANDLE a0, PVOID a1, DWORD a2, PDWORD a3, PDWORD a4, PDWORD a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PostQueuedCompletionStatus(
	HANDLE a0, DWORD a1, ULONG_PTR a2, LPOVERLAPPED a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI PrepareTape(
	HANDLE a0, DWORD a1, BOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PrivilegeCheck (
	HANDLE a0, PPRIVILEGE_SET a1, PBOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PrivilegedServiceAuditAlarmA(
	LPCSTR a0, LPCSTR a1, HANDLE a2, PPRIVILEGE_SET a3, BOOL a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PrivilegedServiceAuditAlarmW(
	LPCWSTR a0, LPCWSTR a1, HANDLE a2, PPRIVILEGE_SET a3, BOOL a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI ProcessIdToSessionId(
	DWORD a0, DWORD *a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PulseEvent(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI PurgeComm(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueryActCtxW(
	DWORD a0, HANDLE a1, PVOID a2, ULONG a3, PVOID a4,
	SIZE_T a5, SIZE_T *a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI QueryDosDeviceA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI QueryDosDeviceW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueryMemoryResourceNotification(
	HANDLE a0, PBOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(
	PLARGE_INTEGER a0)
{
	VxStubWarning(__FILE__, __LINE__, __FUNCTION__);
	return(0);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(
	PLARGE_INTEGER a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI QueueUserAPC(
	PAPCFUNC a0, HANDLE a1, ULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueueUserWorkItem(
	LPTHREAD_START_ROUTINE a0, PVOID a1, ULONG a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
