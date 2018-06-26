#include <vx_kernel32.h>

int vxk32_nexttls=16;

WINBASEAPI void WINAPI Sleep(DWORD a0)
{
	vx_nsleep(a0*1000000LL);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SleepEx(DWORD a0, BOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SuspendThread(HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI SwitchToFiber(PVOID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SwitchToThread(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TerminateProcess(HANDLE a0, UINT a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TerminateThread(HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI TlsAlloc(VOID)
{
	return(vxk32_nexttls++);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TlsFree(DWORD a0)
{
	return(FALSE);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI TlsGetValue(DWORD a0)
{
    struct _TEB *teb;

	teb=NtCurrentTeb();
	if((a0>=0) && (a0<64))
	{
		SetLastError(ERROR_SUCCESS);
		return(teb->TlsSlots[a0]);
	}
	if((a0>=64) && (a0<1088))
	{
		if(!teb->TlsExpansionSlots)
		{
			teb->TlsExpansionSlots=((BYTE *)teb)+sizeof(struct _TEB);
		}
		SetLastError(ERROR_SUCCESS);
		return(((PVOID *)teb->TlsExpansionSlots)[a0-64]);
	}
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
	return(NULL);
}

WINBASEAPI BOOL WINAPI TlsSetValue(DWORD a0, PVOID a1)
{
    struct _TEB *teb;

	teb=NtCurrentTeb();
	if((a0>=0) && (a0<64))
	{
		teb->TlsSlots[a0]=a1;
		return(TRUE);
	}
	if((a0>=64) && (a0<1088))
	{
		if(!teb->TlsExpansionSlots)
		{
			teb->TlsExpansionSlots=((BYTE *)teb)+sizeof(struct _TEB);
		}
		((PVOID *)teb->TlsExpansionSlots)[a0-64]=a1;
		return(TRUE);
	}
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
	return(FALSE);
}



// #if (_WIN32_WINNT >= 0x0500)
#if 1
WINBASEAPI HANDLE WINAPI CreateJobObjectA(
	LPSECURITY_ATTRIBUTES a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI CreateJobObjectW(
	LPSECURITY_ATTRIBUTES a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TerminateJobObject(
	HANDLE a0, UINT a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI AssignProcessToJobObject(
	HANDLE a0, HANDLE a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetInformationJobObject(
	HANDLE a0, JOBOBJECTINFOCLASS a1, LPVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI QueryInformationJobObject(
	HANDLE a0, JOBOBJECTINFOCLASS a1, LPVOID a2, DWORD a3, LPDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
#endif

WINBASEAPI BOOL WINAPI ConvertFiberToThread(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI ConvertThreadToFiber(PVOID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPVOID WINAPI CreateFiber(
	SIZE_T a0, LPFIBER_START_ROUTINE a1, LPVOID a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

// #if (_WIN32_WINNT >= 0x0400)
WINBASEAPI LPVOID WINAPI CreateFiberEx(
	SIZE_T a0, SIZE_T a1, DWORD a2, LPFIBER_START_ROUTINE a3, LPVOID a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
// #endif

void WINAPI RtlUnwind(
	PVOID TargetFrame,
	PVOID TargetIp,
	PEXCEPTION_RECORD ExceptionRecord,
	PVOID ReturnValue)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

