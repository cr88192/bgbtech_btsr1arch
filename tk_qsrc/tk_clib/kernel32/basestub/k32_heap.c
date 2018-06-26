#include <vx_kernel32.h>

typedef struct VXK32_HeapInfo_s VXK32_HeapInfo;

struct VXK32_HeapInfo_s {
int nCells;
BYTE *cellMap;
};

WINBASEAPI PVOID WINAPI HeapAlloc(
	HANDLE hHeap, DWORD dwFlags, DWORD dwBytes)
{
	VXK32_HeapInfo *heap;
	void *ptr;
	int i, j, k, n;

	heap=(VXK32_HeapInfo *)hHeap;

	n=(dwBytes+15)/16;

	for(i=0; i<heap->nCells; i++)
	{
		if(heap->cellMap[i])
		{
			for(j=i+1; j<heap->nCells; i++)
				if(!(heap->cellMap[i]&3))
					break;
			i=j-1;
			continue;
		}
		k=i+n;
		for(j=i; (j<k) && (j<heap->nCells); i++)
			if(heap->cellMap[i]&3)
				break;
		k=j-i;
		if(k>=n)break;
		i=j-1;
	}

	if(i<heap->nCells)
	{
		ptr=((BYTE *)heap)+(i*16);
		
		k=i+n;
		for(j=i+1; j<k; j++)
			heap->cellMap[j]=2;
		heap->cellMap[i]=1;
		
		memset(ptr, 0, dwBytes);
		return(ptr);
	}

	return(NULL);

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

SIZE_T WINAPI HeapCompact(HANDLE a0, DWORD a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI HANDLE WINAPI HeapCreate(
	DWORD flOpts, DWORD dwInitSize, DWORD dwMaxSize)
{
	VXK32_HeapInfo *tmp;
	int i, j, k;

	tmp=vx_malloc(dwMaxSize);
	tmp->nCells=dwMaxSize/16;
	tmp->cellMap=(BYTE *)(tmp+1);
	
	k=(sizeof(VXK32_HeapInfo)+tmp->nCells+15)/16;
	for(i=0; i<k; i++)
		tmp->cellMap[i]=3;

	return(tmp);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapDestroy(HANDLE hHeap)
{
	vx_free(hHeap);
	return(TRUE);

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapFree(HANDLE hHeap, DWORD dwFlags, PVOID lpMem)
{
	VXK32_HeapInfo *heap;
	int i, j, k;

	heap=hHeap;
	i=(((BYTE *)lpMem)-((BYTE *)hHeap))/16;
	if(i<0)
		return(FALSE);
	if(i>=heap->nCells)
		return(FALSE);
	if((heap->cellMap[i]&3)!=1)
		return(FALSE);

	heap->cellMap[i]=0;
	for(j=i+1; j<heap->nCells; j++)
	{
		if((heap->cellMap[j]&3)!=2)
			break;
		heap->cellMap[j]=0;
	}
	return(TRUE);

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapLock(HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI PVOID WINAPI HeapReAlloc(
	HANDLE hHeap, DWORD dwFlags, PVOID lpMem, DWORD dwBytes)
{
	PVOID lpMem2;
	int sz, sz1;

	sz1=HeapSize(hHeap, 0, lpMem);
	sz=sz1;
	if(dwBytes<sz)sz=dwBytes;

	lpMem2=HeapAlloc(hHeap, 0, dwBytes);
	memcpy(lpMem2, lpMem, sz);
	HeapFree(hHeap, 0, lpMem);
	return(lpMem2);
//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI DWORD WINAPI HeapSize(HANDLE hHeap, DWORD dwFlags, PCVOID lpMem)
{
	VXK32_HeapInfo *heap;
	int i, j, k;

	heap=hHeap;
	i=(((BYTE *)lpMem)-((BYTE *)hHeap))/16;
	if(i<0)
		return(-1);
	if(i>=heap->nCells)
		return(-1);
	if((heap->cellMap[i]&3)!=1)
		return(-1);

	for(j=i+1; j<heap->nCells; j++)
	{
		if((heap->cellMap[j]&3)!=2)
			break;
	}
	return((j-i)*16);

//	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapUnlock(HANDLE a0)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapValidate(HANDLE a0, DWORD a1, PCVOID a2)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}

WINBASEAPI BOOL WINAPI HeapWalk(HANDLE a0, LPPROCESS_HEAP_ENTRY a1)
{
	VxStubFault(__FILE__, __LINE__, __FUNCTION__);
}
