#include <vx_kernel32.h>

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
	HANDLE a0, LPCVOID a1,
	PMEMORY_BASIC_INFORMATION a2, DWORD a3)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI VirtualUnlock(PVOID a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

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
	PVOID *a0,ULONG_PTR a1, PULONG_PTR a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
