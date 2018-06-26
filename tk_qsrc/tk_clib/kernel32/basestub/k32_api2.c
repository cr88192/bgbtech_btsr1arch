#include <vx_kernel32.h>

//WINBASEAPI BOOL WINAPI TerminateJobObject(
//	HANDLE a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI SetInformationJobObject(
//	HANDLE a0, JOBOBJECTINFOCLASS a1, LPVOID a2, DWORD a3)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI QueryInformationJobObject(
//	HANDLE a0, JOBOBJECTINFOCLASS a1, LPVOID a2, DWORD a3, LPDWORD a4)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI DWORD WINAPI SetCriticalSectionSpinCount(
	LPCRITICAL_SECTION a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}


WINBASEAPI BOOL WINAPI TzSpecificLocalTimeToSystemTime(
	LPTIME_ZONE_INFORMATION a0, LPSYSTEMTIME a1, LPSYSTEMTIME a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI TerminateProcess(
//	HANDLE a0, UINT a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI TerminateThread(
//	HANDLE a0, DWORD a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

/* 
WINBASEAPI DWORD WINAPI TlsAlloc(VOID)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TlsFree(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI TlsGetValue(
	DWORD a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TlsSetValue(
	DWORD a0, PVOID a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/

WINBASEAPI BOOL WINAPI TransactNamedPipe(
	HANDLE a0, PVOID a1, DWORD a2, PVOID a3, DWORD a4,
	PDWORD a5, LPOVERLAPPED a6)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TransmitCommChar(
	HANDLE a0, char a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI TryEnterCriticalSection(
	LPCRITICAL_SECTION a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI LONG WINAPI UnhandledExceptionFilter(
	LPEXCEPTION_POINTERS a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI UnregisterWait(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI UnregisterWaitEx(
	HANDLE a0, HANDLE a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

//WINBASEAPI BOOL WINAPI UpdateResourceA(
//	HANDLE a0, LPCSTR a1, LPCSTR a2, WORD a3, PVOID a4, DWORD a5)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI UpdateResourceW(
//	HANDLE a0, LPCWSTR a1, LPCWSTR a2, WORD a3, PVOID a4, DWORD a5)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI VerifyVersionInfoA(
//	LPOSVERSIONINFOEXA a0, DWORD a1, DWORDLONG a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI VerifyVersionInfoW(
//	LPOSVERSIONINFOEXW a0, DWORD a1, DWORDLONG a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

/*
WINBASEAPI PVOID WINAPI VirtualAlloc(
	PVOID a0, DWORD a1, DWORD a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI VirtualAllocEx(
	HANDLE a0, PVOID a1, DWORD a2, DWORD a3, DWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualFree(
	PVOID a0, DWORD a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualFreeEx(
	HANDLE a0, PVOID a1, DWORD a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualLock(
	PVOID a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualProtect(
	PVOID a0, DWORD a1, DWORD a2, PDWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualProtectEx(
	HANDLE a0, PVOID a1, DWORD a2, DWORD a3, PDWORD a4)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI VirtualQuery(
	LPCVOID a0, PMEMORY_BASIC_INFORMATION a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI VirtualQueryEx(
	HANDLE a0, LPCVOID a1, PMEMORY_BASIC_INFORMATION a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualUnlock(
	PVOID a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/

//WINBASEAPI BOOL WINAPI WaitCommEvent(
//	HANDLE a0, PDWORD a1, LPOVERLAPPED a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WaitForDebugEvent(
//	LPDEBUG_EVENT a0, DWORD a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI WaitForMultipleObjects(
//	DWORD a0, const HANDLE *a1, BOOL a2, DWORD a3)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI WaitForMultipleObjectsEx(
//	DWORD a0, const HANDLE *a1, BOOL a2, DWORD a3, BOOL a4)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI WaitForSingleObject(
//	HANDLE a0, DWORD a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI WaitForSingleObjectEx(
//	HANDLE a0, DWORD a1, BOOL a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WaitNamedPipeA(
//	LPCSTR a0, DWORD a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WaitNamedPipeW(
//	LPCWSTR a0, DWORD a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WinLoadTrustProvider(
//	GUID *a0)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileSectionA(
//	LPCSTR a0, LPCSTR a1, LPCSTR a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileSectionW(
//	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileStringA(
//	LPCSTR a0, LPCSTR a1, LPCSTR a2, LPCSTR a3)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileStringW(
//	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2, LPCWSTR a3)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileStructA(
//	LPCSTR a0, LPCSTR a1, LPVOID a2, UINT a3, LPCSTR a4)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WritePrivateProfileStructW(
//	LPCWSTR a0, LPCWSTR a1, LPVOID a2, UINT a3, LPCWSTR a4)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WriteProcessMemory(
//	HANDLE a0, LPVOID a1, LPCVOID a2, SIZE_T a3, SIZE_T *a4)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WriteProfileSectionA(
//	LPCSTR a0, LPCSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WriteProfileSectionW(
//	LPCWSTR a0, LPCWSTR a1)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WriteProfileStringA(
//	LPCSTR a0, LPCSTR a1, LPCSTR a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI BOOL WINAPI WriteProfileStringW(
//	LPCWSTR a0, LPCWSTR a1, LPCWSTR a2)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

//WINBASEAPI DWORD WINAPI WriteTapemark(
//	HANDLE a0, DWORD a1, DWORD a2, BOOL a3)
//{
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
//}

WINBASEAPI BOOL WINAPI ZombifyActCtx(
	HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

/*
WINBASEAPI BOOL WINAPI AllocateUserPhysicalPages(
	HANDLE a0, PULONG_PTR a1, PULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI FreeUserPhysicalPages(
	HANDLE a0, PULONG_PTR a1, PULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI MapUserPhysicalPages(
	PVOID a0, ULONG_PTR a1, PULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI MapUserPhysicalPagesScatter(
	PVOID *a0, ULONG_PTR a1, PULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
*/
