#include <vx_kernel32.h>

LONG WINAPI InterlockedCompareExchange(
	LONG volatile  *a0, LONG a1, LONG a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

LONG WINAPI InterlockedDecrement(
	LONG volatile  *a0)
{
	return(*a0--);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

LONG WINAPI InterlockedExchange(
	LONG volatile  *a0, LONG a1)
{
	LONG t0;
	
	t0=*a0;
	*a0=a1;
	return(t0);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

LONG WINAPI InterlockedExchangeAdd(
	LONG volatile  *a0, LONG a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

PSLIST_ENTRY WINAPI InterlockedFlushSList(
	PSLIST_HEADER a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

LONG WINAPI InterlockedIncrement(
	LONG volatile  *a0)
{
	return(*a0++);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

PSLIST_ENTRY WINAPI InterlockedPopEntrySList(
	PSLIST_HEADER a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

PSLIST_ENTRY WINAPI InterlockedPushEntrySList(
	PSLIST_HEADER a0, PSLIST_ENTRY a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI InitializeCriticalSection(
	LPCRITICAL_SECTION a0)
{
	a0->DebugInfo=NULL;
	a0->LockCount=0;
	a0->RecursionCount=0;
	a0->OwningThread=NULL;
	a0->LockSemaphore=NULL;
	a0->SpinCount=0;
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI InitializeCriticalSectionAndSpinCount(
	LPCRITICAL_SECTION a0, DWORD a1)
{
	a0->DebugInfo=NULL;
	a0->LockCount=0;
	a0->RecursionCount=0;
	a0->OwningThread=NULL;
	a0->LockSemaphore=NULL;
	a0->SpinCount=a1;
	return(TRUE);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI EnterCriticalSection(LPCRITICAL_SECTION a0)
{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI LeaveCriticalSection(LPCRITICAL_SECTION a0)
{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI WaitCommEvent(HANDLE a0, PDWORD a1, LPOVERLAPPED a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI WaitForDebugEvent(LPDEBUG_EVENT a0,DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI WaitForMultipleObjects(
	DWORD a0,const HANDLE *a1,BOOL a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI WaitForMultipleObjectsEx(
	DWORD a0, const HANDLE *a1, BOOL a2, DWORD a3, BOOL a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI WaitForSingleObject(HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI WaitForSingleObjectEx(HANDLE a0, DWORD a1, BOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
