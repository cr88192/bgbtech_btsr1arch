#include <vx_kernel32.h>

DWORD vxk32_errno;

WINBASEAPI DWORD WINAPI GetFirmwareEnvironmentVariableA(
	LPCSTR a0, LPCSTR a1, PVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetFirmwareEnvironmentVariableW(
	LPCWSTR a0, LPCWSTR a1, PVOID a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetAce(
	PACL a0, DWORD a1, LPVOID *a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetAclInformation(
	PACL a0, PVOID a1, DWORD a2, ACL_INFORMATION_CLASS a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetAtomNameA(
	ATOM a0, LPSTR a1, int a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetAtomNameW(
	ATOM a0, LPWSTR a1, int a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetBinaryTypeA(
	LPCSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetBinaryTypeW(
	LPCWSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPSTR WINAPI GetCommandLineA(VOID)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPWSTR WINAPI GetCommandLineW(VOID)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommConfig(
	HANDLE a0, LPCOMMCONFIG a1, PDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommMask(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommModemStatus(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommProperties(
	HANDLE a0, LPCOMMPROP a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommState(
	HANDLE a0, LPDCB a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCommTimeouts(
	HANDLE a0, LPCOMMTIMEOUTS a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetComputerNameA(
	LPSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetComputerNameW(
	LPWSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetComputerNameExA(
	COMPUTER_NAME_FORMAT a0, LPSTR a1, LPDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetComputerNameExW(
	COMPUTER_NAME_FORMAT a0, LPWSTR a1, LPDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCurrentActCtx(
	HANDLE *a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI DWORD WINAPI GetCurrentDirectoryA(
//	DWORD a0, LPSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI GetCurrentDirectoryW(
//	DWORD a0, LPWSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI GetCurrentHwProfileA(
	LPHW_PROFILE_INFOA a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetCurrentHwProfileW(
	LPHW_PROFILE_INFOW a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI GetCurrentProcess(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetCurrentProcessId(void)
{
    struct _TEB *teb;
	teb=NtCurrentTeb();
	return(teb->ProcessID);

//	return(VxGetCurrentPID());
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI GetCurrentThread(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetCurrentThreadId(void)
{
    struct _TEB *teb;
	teb=NtCurrentTeb();
	return(teb->ThreadID);
//	VxStubWarning(__FILE__, __LINE__, __FUNCTION__);
	return(0);
}

WINBASEAPI BOOL WINAPI GetDefaultCommConfigA(
	LPCSTR a0, LPCOMMCONFIG a1, PDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDefaultCommConfigW(
	LPCWSTR a0, LPCOMMCONFIG a1, PDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDevicePowerState(
	HANDLE a0,  BOOL *a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDiskFreeSpaceA(
	LPCSTR a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDiskFreeSpaceW(
	LPCWSTR a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDiskFreeSpaceExA(
	LPCSTR a0, PULARGE_INTEGER a1, PULARGE_INTEGER a2, PULARGE_INTEGER a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetDiskFreeSpaceExW(
	LPCWSTR a0, PULARGE_INTEGER a1, PULARGE_INTEGER a2, PULARGE_INTEGER a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI DWORD WINAPI GetDllDirectoryA(
//	DWORD a0, LPSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI GetDllDirectoryW(
//	DWORD a0, LPWSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI UINT WINAPI GetDriveTypeA(
	LPCSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetDriveTypeW(
	LPCWSTR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPCH WINAPI GetEnvironmentStrings(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPCH WINAPI GetEnvironmentStringsA(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LPWCH WINAPI GetEnvironmentStringsW(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetEnvironmentVariableA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetEnvironmentVariableW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetExitCodeProcess(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetExitCodeThread(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetFinalPathNameByHandleA(
	HANDLE a0, LPSTR a1, DWORD a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetFinalPathNameByHandleW(
	HANDLE a0, LPWSTR a1, DWORD a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetFullPathNameA(
	LPCSTR a0, DWORD a1, LPSTR a2, LPSTR *a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetFullPathNameW(
	LPCWSTR a0, DWORD a1, LPWSTR a2, LPWSTR *a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetHandleInformation(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetKernelObjectSecurity(
	HANDLE a0, SECURITY_INFORMATION a1, PSECURITY_DESCRIPTOR a2,
	DWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLastError(void)
{
    struct _TEB *teb;
	teb=NtCurrentTeb();
	return(teb->LastError);

//	return(vxk32_errno);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLengthSid(
	PSID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI void WINAPI GetLocalTime(
	LPSYSTEMTIME a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLogicalDrives(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLogicalDriveStringsA(
	DWORD a0, LPSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLogicalDriveStringsW(
	DWORD a0, LPWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLongPathNameA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetLongPathNameW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetMailslotInfo(
	HANDLE a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI HMODULE WINAPI GetModuleHandleA(
//	LPCSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI HMODULE WINAPI GetModuleHandleW(
//	LPCWSTR a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI GetModuleHandleExA(
//	DWORD a0, LPCSTR a1, HMODULE *a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI GetModuleHandleExW(
//	DWORD a0, LPCWSTR a1, HMODULE *a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI GetNamedPipeHandleStateA(
	HANDLE a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4,
	LPSTR a5, DWORD a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetNamedPipeHandleStateW(
	HANDLE a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4,
	LPWSTR a5, DWORD a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetNamedPipeInfo(
	HANDLE a0, PDWORD a1, PDWORD a2, PDWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI GetNativeSystemInfo(
	LPSYSTEM_INFO a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetNumberOfEventLogRecords(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetOldestEventLogRecord(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetOverlappedResult(
	HANDLE a0, LPOVERLAPPED a1, PDWORD a2, BOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPriorityClass(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetPrivateObjectSecurity(
	PSECURITY_DESCRIPTOR a0, SECURITY_INFORMATION a1,
	PSECURITY_DESCRIPTOR a2, DWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetPrivateProfileIntA(
	LPCSTR a0, LPCSTR a1, INT a2, LPCSTR a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetPrivateProfileIntW(
	LPCWSTR a0, LPCWSTR a1, INT a2, LPCWSTR a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileSectionA(
	LPCSTR a0, LPSTR a1, DWORD a2, LPCSTR a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileSectionW(
	LPCWSTR a0, LPWSTR a1, DWORD a2, LPCWSTR a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileSectionNamesA(
	LPSTR a0, DWORD a1, LPCSTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileSectionNamesW(
	LPWSTR a0, DWORD a1, LPCWSTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileStringA(
	LPCSTR a0, LPCSTR a1, LPCSTR a2, LPSTR a3, DWORD a4, LPCSTR a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetPrivateProfileStringW(
	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2, LPWSTR a3, DWORD a4, LPCWSTR a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetPrivateProfileStructA(
	LPCSTR a0, LPCSTR a1, LPVOID a2, UINT a3, LPCSTR a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetPrivateProfileStructW(
	LPCWSTR a0, LPCWSTR a1, LPVOID a2, UINT a3, LPCWSTR a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI FARPROC WINAPI GetProcAddress(
//	HINSTANCE a0, LPCSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI GetProcessAffinityMask(
	HANDLE a0, PDWORD a1, PDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessDEPPolicy (
	HANDLE a0, LPDWORD a1, PBOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessHandleCount(
	HANDLE a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI GetProcessHeap(VOID)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProcessHeaps(
	DWORD a0, PHANDLE a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProcessId(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessIoCounters(
	HANDLE a0, PIO_COUNTERS a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessPriorityBoost(
	HANDLE a0, PBOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessShutdownParameters(
	PDWORD a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessTimes(
	HANDLE a0, LPFILETIME a1, LPFILETIME a2, LPFILETIME a3, LPFILETIME a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProcessVersion(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HWINSTA WINAPI GetProcessWindowStation(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetProcessWorkingSetSize(
	HANDLE a0, PSIZE_T a1, PSIZE_T a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetProfileIntA(
	LPCSTR a0, LPCSTR a1, INT a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetProfileIntW(
	LPCWSTR a0, LPCWSTR a1, INT a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProfileSectionA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProfileSectionW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProfileStringA(
	LPCSTR a0, LPCSTR a1, LPCSTR a2, LPSTR a3, DWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetProfileStringW(
	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2, LPWSTR a3, DWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetQueuedCompletionStatus(
	HANDLE a0, PDWORD a1, PULONG_PTR a2, LPOVERLAPPED *a3, DWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSecurityDescriptorControl(
	PSECURITY_DESCRIPTOR a0, PSECURITY_DESCRIPTOR_CONTROL a1, PDWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSecurityDescriptorDacl(
	PSECURITY_DESCRIPTOR a0, LPBOOL a1, PACL *a2, LPBOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSecurityDescriptorGroup(
	PSECURITY_DESCRIPTOR a0, PSID *a1, LPBOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetSecurityDescriptorLength(
	PSECURITY_DESCRIPTOR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSecurityDescriptorOwner(
	PSECURITY_DESCRIPTOR a0, PSID *a1, LPBOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSecurityDescriptorSacl(
	PSECURITY_DESCRIPTOR a0, LPBOOL a1, PACL *a2, LPBOOL a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetShortPathNameA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetShortPathNameW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PSID_IDENTIFIER_AUTHORITY WINAPI GetSidIdentifierAuthority(
	PSID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetSidLengthRequired(
	UCHAR a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PDWORD WINAPI GetSidSubAuthority(
	PSID a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PUCHAR WINAPI GetSidSubAuthorityCount(
	PSID a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI GetStartupInfoA(
	LPSTARTUPINFOA a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI GetStartupInfoW(
	LPSTARTUPINFOW a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI GetStdHandle(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI DEP_SYSTEM_POLICY_TYPE WINAPI GetSystemDEPPolicy(void)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

/*
WINBASEAPI UINT WINAPI GetSystemDirectoryA(
	LPSTR a0, UINT a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetSystemDirectoryW(
	LPWSTR a0, UINT a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/

WINBASEAPI VOID WINAPI GetSystemInfo(
	LPSYSTEM_INFO a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSystemPowerStatus(
	LPSYSTEM_POWER_STATUS a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSystemRegistryQuota(
	PDWORD a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI VOID WINAPI GetSystemTime(
	LPSYSTEMTIME a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSystemTimes(
	LPFILETIME a0, LPFILETIME a1, LPFILETIME a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetSystemTimeAdjustment(
	PDWORD a0, PDWORD a1, PBOOL a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI UINT WINAPI GetSystemWindowsDirectoryA(
//	LPSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI UINT WINAPI GetSystemWindowsDirectoryW(
//	LPWSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI UINT WINAPI GetSystemWow64DirectoryA(
//	LPSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI UINT WINAPI GetSystemWow64DirectoryW(
//	LPWSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI DWORD WINAPI GetTapeParameters(
	HANDLE a0, DWORD a1, PDWORD a2, PVOID a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetTapePosition(
	HANDLE a0, DWORD a1, PDWORD a2, PDWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetTapeStatus(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetTempPathA(
	DWORD a0, LPSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetTempPathW(
	DWORD a0, LPWSTR a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetThreadContext(
	HANDLE a0, LPCONTEXT a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetThreadIOPendingFlag(
	HANDLE a0, PBOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI int WINAPI GetThreadPriority(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetThreadPriorityBoost(
	HANDLE a0, PBOOL a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetThreadSelectorEntry(
	HANDLE a0, DWORD a1, LPLDT_ENTRY a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetThreadTimes(
	HANDLE a0, LPFILETIME a1, LPFILETIME a2, LPFILETIME a3, LPFILETIME a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetTickCount(VOID)
{
	VxStubWarning(__FILE__, __LINE__, __FUNCTION__);
	return(0);
}

WINBASEAPI DWORD WINAPI GetTimeZoneInformation(
	LPTIME_ZONE_INFORMATION a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetTokenInformation(
	HANDLE a0, TOKEN_INFORMATION_CLASS a1, PVOID a2, DWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetUserNameA(
	LPSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetUserNameW(
	LPWSTR a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetVersion(void)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVersionExA(
	LPOSVERSIONINFOA a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVersionExW(
	LPOSVERSIONINFOW a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumeInformationA(
	LPCSTR a0, LPSTR a1, DWORD a2, PDWORD a3, PDWORD a4,
	PDWORD a5, LPSTR a6, DWORD a7)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumeInformationW(
	LPCWSTR a0, LPWSTR a1, DWORD a2, PDWORD a3, PDWORD a4,
	PDWORD a5, LPWSTR a6, DWORD a7)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumeNameForVolumeMountPointA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumeNameForVolumeMountPointW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumePathNameA(
	LPCSTR a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumePathNameW(
	LPCWSTR a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumePathNamesForVolumeNameA(
	LPCSTR a0, LPSTR a1, DWORD a2, PDWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetVolumePathNamesForVolumeNameW(
	LPCWSTR a0, LPWSTR a1, DWORD a2, PDWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI UINT WINAPI GetWindowsDirectoryA(
//	LPSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI UINT WINAPI GetWindowsDirectoryW(
//	LPWSTR a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI DWORD WINAPI GetWindowThreadProcessId(
	HWND a0, PDWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI UINT WINAPI GetWriteWatch(
	DWORD a0, PVOID a1, SIZE_T a2, PVOID *a3, PULONG_PTR a4, PULONG a5)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}


/*
WINBASEAPI ATOM WINAPI GlobalAddAtomA(
	LPCSTR a0)
WINBASEAPI ATOM WINAPI GlobalAddAtomW(
	LPCWSTR a0)
WINBASEAPI HGLOBAL WINAPI GlobalAlloc(
	UINT a0, DWORD a1)
WINBASEAPI SIZE_T WINAPI GlobalCompact(
	DWORD a0)
WINBASEAPI ATOM WINAPI GlobalDeleteAtom(
	ATOM a0)
WINBASEAPI ATOM WINAPI GlobalFindAtomA(
	LPCSTR a0)
WINBASEAPI ATOM WINAPI GlobalFindAtomW(
	LPCWSTR a0)
WINBASEAPI VOID WINAPI GlobalFix(
	HGLOBAL a0)
WINBASEAPI UINT WINAPI GlobalFlags(
	HGLOBAL a0)
WINBASEAPI HGLOBAL WINAPI GlobalFree(
	HGLOBAL a0)
WINBASEAPI UINT WINAPI GlobalGetAtomNameA(
	ATOM a0, LPSTR a1, int a2)
WINBASEAPI UINT WINAPI GlobalGetAtomNameW(
	ATOM a0, LPWSTR a1, int a2)
WINBASEAPI HGLOBAL WINAPI GlobalHandle(
	PCVOID a0)
WINBASEAPI LPVOID WINAPI GlobalLock(
	HGLOBAL a0)
WINBASEAPI VOID WINAPI GlobalMemoryStatus(
	LPMEMORYSTATUS a0)
WINBASEAPI BOOL WINAPI GlobalMemoryStatusEx(
	LPMEMORYSTATUSEX a0)
WINBASEAPI HGLOBAL WINAPI GlobalReAlloc(
	HGLOBAL a0, DWORD a1, UINT a2)
WINBASEAPI DWORD WINAPI GlobalSize(
	HGLOBAL a0)
WINBASEAPI VOID WINAPI GlobalUnfix(
	HGLOBAL a0)
WINBASEAPI BOOL WINAPI GlobalUnlock(
	HGLOBAL a0) 
WINBASEAPI BOOL WINAPI GlobalUnWire(
	HGLOBAL a0)
WINBASEAPI PVOID WINAPI GlobalWire(
	HGLOBAL a0)
*/