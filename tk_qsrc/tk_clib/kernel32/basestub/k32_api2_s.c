#include <vx_kernel32.h>

WINBASEAPI DWORD WINAPI SearchPathA(
	LPCSTR a0, LPCSTR a1, LPCSTR a2, DWORD a3, LPSTR a4, LPSTR *a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SearchPathW(
	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2, DWORD a3, LPWSTR a4, LPWSTR *a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetAclInformation(
	PACL a0, PVOID a1, DWORD a2, ACL_INFORMATION_CLASS a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetCommBreak(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetCommConfig(
	HANDLE a0, LPCOMMCONFIG a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetCommMask(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetCommState(
	HANDLE a0, LPDCB a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetCommTimeouts(
	HANDLE a0, LPCOMMTIMEOUTS a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetComputerNameA(
	LPCSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetComputerNameW(
	LPCWSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetComputerNameExA(
	COMPUTER_NAME_FORMAT a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetComputerNameExW(
	COMPUTER_NAME_FORMAT a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI SetCurrentDirectoryA(
//	LPCSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI SetCurrentDirectoryW(
//	LPCWSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI SetDefaultCommConfigA(
	LPCSTR a0, LPCOMMCONFIG a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetDefaultCommConfigW(
	LPCWSTR a0, LPCOMMCONFIG a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI SetDllDirectoryA(
//	LPCSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI SetDllDirectoryW(
//	LPCWSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI SetEnvironmentVariableA(
	LPCSTR a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetEnvironmentVariableW(
	LPCWSTR a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI SetErrorMode(
	UINT a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetEvent(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetFirmwareEnvironmentVariableA(
	LPCSTR a0, LPCSTR a1, PVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetFirmwareEnvironmentVariableW(
	LPCWSTR a0, LPCWSTR a1, PVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI SetHandleCount(
	UINT a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetHandleInformation(
	HANDLE a0, DWORD a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetKernelObjectSecurity(
	HANDLE a0, SECURITY_INFORMATION a1, PSECURITY_DESCRIPTOR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI SetLastError(
	DWORD a0)
{
    struct _TEB *teb;
	teb=NtCurrentTeb();
	teb->LastError=a0;

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI SetLastErrorEx(
	DWORD a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetLocalTime(
	const SYSTEMTIME *a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetMailslotInfo(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetNamedPipeHandleState(
	HANDLE a0, PDWORD a1, PDWORD a2, PDWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetPriorityClass(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetPrivateObjectSecurity(
	SECURITY_INFORMATION a0, PSECURITY_DESCRIPTOR a1,
	PSECURITY_DESCRIPTOR  *a2, PGENERIC_MAPPING a3, HANDLE a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetProcessAffinityMask(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetProcessDEPPolicy(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetProcessPriorityBoost(
	HANDLE a0, BOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetProcessShutdownParameters(
	DWORD a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetProcessWorkingSetSize(
	HANDLE a0, SIZE_T a1, SIZE_T a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSecurityDescriptorControl(
	PSECURITY_DESCRIPTOR a0, SECURITY_DESCRIPTOR_CONTROL a1, 
	SECURITY_DESCRIPTOR_CONTROL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSecurityDescriptorDacl(
	PSECURITY_DESCRIPTOR a0, BOOL a1, PACL a2, BOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSecurityDescriptorGroup(
	PSECURITY_DESCRIPTOR a0, PSID a1, BOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSecurityDescriptorOwner(
	PSECURITY_DESCRIPTOR a0, PSID a1, BOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSecurityDescriptorSacl(
	PSECURITY_DESCRIPTOR a0, BOOL a1, PACL a2, BOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetStdHandle(
	DWORD a0, HANDLE a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSystemPowerState(
	BOOL a0, BOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSystemTime(
	const SYSTEMTIME *a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetSystemTimeAdjustment(
	DWORD a0, BOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SetTapeParameters(
	HANDLE a0, DWORD a1, PVOID a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SetTapePosition(
	HANDLE a0, DWORD a1, DWORD a2, DWORD a3, DWORD a4, BOOL a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SetThreadAffinityMask(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetThreadContext(
	HANDLE a0, const CONTEXT *a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI EXECUTION_STATE WINAPI SetThreadExecutionState(
	EXECUTION_STATE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SetThreadIdealProcessor(
	HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetThreadPriority(
	HANDLE a0, int a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetThreadPriorityBoost(
	HANDLE a0, BOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetThreadToken(
	PHANDLE a0, HANDLE a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetTimeZoneInformation(
	const TIME_ZONE_INFORMATION  *a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetTokenInformation(
	HANDLE a0, TOKEN_INFORMATION_CLASS a1, PVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter(
	LPTOP_LEVEL_EXCEPTION_FILTER a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetupComm(
	HANDLE a0, DWORD a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetVolumeLabelA(
	LPCSTR a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetVolumeLabelW(
	LPCWSTR a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetVolumeMountPointA(
	LPCSTR a0, LPCSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetVolumeMountPointW(
	LPCWSTR a0, LPCWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI SetWaitableTimer(
	HANDLE a0, const LARGE_INTEGER *a1, LONG a2, PTIMERAPCROUTINE a3,
	PVOID a4, BOOL a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SignalObjectAndWait(
	HANDLE a0, HANDLE a1, DWORD a2, BOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI SizeofResource(
	HINSTANCE a0, HRSRC a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI void WINAPI Sleep(
//	DWORD a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI SleepEx(
//	DWORD a0, BOOL a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI SuspendThread(
//	HANDLE a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI void WINAPI SwitchToFiber(
//	PVOID a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI SwitchToThread(void)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI SystemTimeToTzSpecificLocalTime(
	LPTIME_ZONE_INFORMATION a0, LPSYSTEMTIME a1, LPSYSTEMTIME a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

