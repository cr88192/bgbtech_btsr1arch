#include <vx_kernel32.h>

WINBASEAPI HINSTANCE WINAPI LoadLibraryA(LPCSTR a0)
{
	return(VxLoadLibrary(a0));
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HINSTANCE WINAPI LoadLibraryExA(
	LPCSTR a0, HANDLE a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HINSTANCE WINAPI LoadLibraryExW(
	LPCWSTR a0, HANDLE a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HINSTANCE WINAPI LoadLibraryW(
	LPCWSTR a0)
{
	char tb[256];
	LPCWSTR *s;
	char *t;
	
	if(a0)
	{
		s=a0; t=tb;
		while(*s)
			*t++=*s++;
		*t++=0;
		return(VxLoadLibrary(tb));
	}

	return(VxLoadLibrary(NULL));
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI LoadModule(
	LPCSTR a0, PVOID a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetModuleFileNameA(
	HINSTANCE a0, LPSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI GetModuleFileNameW(
	HINSTANCE a0, LPWSTR a1, DWORD a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HMODULE WINAPI GetModuleHandleA(
	LPCSTR a0)
{
	return(VxGetModuleHandle(a0));
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HMODULE WINAPI GetModuleHandleW(
	LPCWSTR a0)
{
	char tb[256];
	LPCWSTR *s;
	char *t, *s1;
	
	if(a0)
	{
		if(*a0>255)
//		if(1)
		{
			s1=(char *)a0; t=tb;
			while(*s1)
				*t++=*s1++;
			*t++=0;
		}else
		{
			s=a0; t=tb;
			while(*s)
				*t++=*s++;
			*t++=0;
		}
		
		vx_printstr("GetModuleHandleW: ");
		vx_printstr(tb);
		vx_printstr("\n");
		
		return(VxGetModuleHandle(tb));
	}

	return(VxGetModuleHandle(NULL));

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetModuleHandleExA(
	DWORD a0, LPCSTR a1, HMODULE *a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI GetModuleHandleExW(
	DWORD a0, LPCWSTR a1, HMODULE *a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI FARPROC WINAPI GetProcAddress(
	HINSTANCE a0, LPCSTR a1)
{
	return(VxGetProcAddress(a0, a1));
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
