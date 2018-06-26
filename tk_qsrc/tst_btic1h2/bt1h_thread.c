#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#endif

#ifdef linux
#include <unistd.h>
#include <pthread.h>
#endif

#ifndef UNDEFINED
#define UNDEFINED ((void *)(-1))
#endif

// #define BIPRO_GCEH_EXCEPTION	0xE015DEAD

#define GCEH_TRY_DEFAULT	0
#define GCEH_TRY_SYSTEM		1

typedef struct th_context_s thContext;

typedef struct BIPRO_RegisterState_s BIPRO_RegisterState;

struct th_context_s {
int (*fcn)(void *ptr);
void *ptr;
int id;

void *hdl;
void *stktop;

//void *tls[1024];
void **tls;
int sz_tls;

void **rootstack;
int rootstackpos, szrootstack;
int *rootframe;
int rootframepos, szrootframe;
};

struct BIPRO_RegisterState_s {
int dummy;
#ifdef X86
u32 eax, ecx, edx, ebx;
u32 esp, ebp, esi, edi;
u32 eip, eflags;
#endif
#ifdef X86_64
u64 rax, rcx, rdx, rbx;
u64 rsp, rbp, rsi, rdi;
u64 r8, r9, r10, r11;
u64 r12, r13, r14, r15;
u64 rip, rflags;
#endif
};


thContext *bipro_thread_inf[1024];

int bipro_thread_num=0;

int bipro_threadctx_tls=-1;
int bipro_threadtls_tls=-1;
int bipro_thread_tlsrov=1;

char *bipro_thread_tlsname[1024];


#ifdef _WIN32
HANDLE bipro_thread_ptrevent;
#endif

#if defined(linux) || defined(NATIVECLIENT)
pthread_key_t bipro_threadctx_tls2;

pthread_mutex_t bipro_threadptr_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bipro_threadptr_cond=PTHREAD_COND_INITIALIZER;
#endif

volatile long bipro_thread_ptrwait=0;
volatile long bipro_thread_ptrwait2=0;
volatile void *bipro_thread_sigptr=NULL;


#ifdef _WIN32
HMODULE bipro_hmodapp=NULL;

char *bipro_hmodname[256];
HMODULE bipro_hmoddll[256];
int bipro_numdll=0;

static char *bipro_os_envarr[256];
static char **bipro_os_env=NULL;

BOOL (__stdcall *SymFromAddr_f) (
  __in HANDLE hProcess, __in DWORD64 Address,
  __out PDWORD64 Displacement,
  __inout PSYMBOL_INFO Symbol );

DWORD (__stdcall *SymSetOptions_f) ( __in DWORD SymOptions );

BOOL (__stdcall *SymInitialize_f) (
	__in HANDLE hProcess, __in PCTSTR UserSearchPath,
	__in BOOL fInvadeProcess );
#endif

thContext *thGetContext();
void *BIPRO_LookupLabelOS(char *name);


void bipro_thread_markctx(thContext *inf)
{
	void *p, *q;
	int i, j;

	if(!inf)return;

	p=inf->stktop;
	if(!p)return;

	j=((1<<18)-6144)/sizeof(void *);

	q=(void *)(((void **)p)-j);
}

void bipro_thread_mark()
{
	void *p, *q;
	int i, j;

	for(i=0; i<bipro_thread_num; i++)
	{
		bipro_thread_markctx(bipro_thread_inf[i]);
	}
}

void bipro_thread_init()
{
	static int init=0;

	if(init)return;
	init=1;

#ifdef _WIN32
	bipro_threadctx_tls=TlsAlloc();
	bipro_threadtls_tls=TlsAlloc();
	bipro_thread_ptrevent=CreateEvent(
		NULL, TRUE, FALSE, TEXT("PointerEvent")); 
		
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
//	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#endif

#if defined(linux) || defined(NATIVECLIENT)
	pthread_key_create(&bipro_threadctx_tls2, NULL);
	pthread_mutex_init(&bipro_threadptr_mutex, NULL);
	pthread_cond_init(&bipro_threadptr_cond, NULL);
#endif

	thGetContext();	//make sure TLS set up
}

int thAllocTls()
{
	int i;

	i=bipro_thread_tlsrov++;
	bipro_thread_tlsname[i]="";
	return(i);
}

int thAllocTlsName(char *name, int sz)
{
	int i, j, n;

	for(i=0; i<bipro_thread_tlsrov; i++)
		if(bipro_thread_tlsname[i] &&
			!strcmp(bipro_thread_tlsname[i], name))
				return(i);

	n=(sz+(sizeof(void *)-1))/sizeof(void *);
	i=bipro_thread_tlsrov;
	bipro_thread_tlsrov+=n;
	bipro_thread_tlsname[i]=strdup(name);
	for(j=i+1; j<bipro_thread_tlsrov; j++)
		bipro_thread_tlsname[j]="~";

	return(i);
}

void thFreeTls(int idx)
{
}

void *thGetTlsValue(int idx)
{
	thContext *inf;
	inf=thGetContext();
	if(!inf || !inf->tls)
		return(NULL);
	return(inf->tls[idx]);
}

void thSetTlsValue(int idx, void *val)
{
	thContext *inf;
	inf=thGetContext();
	inf->tls[idx]=val;
}

void *thGetTlsPtr(int idx)
{
	thContext *inf;
	inf=thGetContext();
	return(&(inf->tls[idx]));
}

#ifdef _WIN32
DWORD WINAPI BIPRO_ThreadProc(LPVOID lpParam) 
{
	thContext *inf;
	int i;

	inf=lpParam;
	TlsSetValue(bipro_threadctx_tls, (void *)inf);

	if(!inf->tls)
	{
		inf->tls=malloc(1024*sizeof(void *));
		memset(inf->tls, 0, 1024*sizeof(void *));
		inf->sz_tls=1024;

//		TlsSetValue(bipro_threadctx_tls, (void *)inf);
		TlsSetValue(bipro_threadtls_tls, (void *)(inf->tls));
	}

//	bipro_thread_stktop[inf->id]=&lpParam;
//	bipro_thread_inf[inf->id]=inf;

	bipro_thread_inf[inf->id]=inf;
	inf->stktop=&lpParam;

	i=inf->fcn(inf->ptr);

//	bipro_thread_hdl[inf->id]=NULL;
//	bipro_thread_stktop[inf->id]=NULL;

	inf->hdl=NULL;
	inf->stktop=NULL;
	bipro_thread_inf[inf->id]=NULL;
	return(i);
}

void *thThread(int (*fcn)(void *ptr), void *ptr)
{
	thContext *inf;
	HANDLE hth;
	DWORD thid;
	int i;

	bipro_thread_init();

//	for(i=0; i<bipro_thread_num; i++)
//		if(!bipro_thread_hdl[i])
//			break;

	for(i=0; i<bipro_thread_num; i++)
		if(!bipro_thread_inf[i])
			break;

	if(i>=bipro_thread_num)i=bipro_thread_num++;

	inf=malloc(sizeof(thContext));
	memset(inf, 0, sizeof(thContext));

	inf->id=i;

//	bipro_thread_hdl[i]=UNDEFINED;
//	bipro_thread_stktop[i]=UNDEFINED;

	inf->hdl=UNDEFINED;
	inf->stktop=UNDEFINED;

	inf->fcn=fcn;
	inf->ptr=ptr;

	hth=CreateThread(NULL, 1<<18,
		BIPRO_ThreadProc, (LPVOID)(inf),
		0, &thid);

//	bipro_thread_hdl[i]=hth;
	inf->hdl=hth;

//	printf("Th %p %d %p\n", hth, thid, fcn);

	return(hth);
}


void BIPRO_SuspendThreads()
{
	HANDLE hth;
	thContext *inf;
	int i;

	inf=thGetContext();

	for(i=0; i<bipro_thread_num; i++)
	{
		if(!bipro_thread_inf[i])
			continue;

		hth=bipro_thread_inf[i]->hdl;
		if(hth==inf->hdl)continue;
		SuspendThread(hth);
	}
}

void BIPRO_ResumeThreads()
{
	HANDLE hth;
	thContext *inf;
	int i;

	inf=thGetContext();

	for(i=0; i<bipro_thread_num; i++)
	{
		if(!bipro_thread_inf[i])
			continue;

		hth=bipro_thread_inf[i]->hdl;
		if(hth==inf->hdl)continue;
		ResumeThread(hth);
	}
}

void BIPRO_HandleThreadStates(
	void (*fcn)(thContext *inf))
{
	HANDLE hth;
	thContext *inf;
	int i;

	inf=thGetContext();

	for(i=0; i<bipro_thread_num; i++)
	{
		if(!bipro_thread_inf[i])
			continue;

		hth=bipro_thread_inf[i]->hdl;
		if(hth==inf->hdl)continue;
		SuspendThread(hth);
		fcn(bipro_thread_inf[i]);
		ResumeThread(hth);
	}
}

void BIPRO_GetThreadRegisterState(
	thContext *inf, BIPRO_RegisterState *regs)
{
	CONTEXT rctx;
	
	memset(&rctx, 0, sizeof(CONTEXT));
	
	rctx.ContextFlags=CONTEXT_INTEGER|CONTEXT_CONTROL;
	GetThreadContext(inf->hdl, &rctx);
	
#ifdef X86
	regs->eax=rctx.Eax;		regs->ecx=rctx.Ecx;
	regs->edx=rctx.Edx;		regs->ebx=rctx.Ebx;
	regs->esp=rctx.Esp;		regs->ebp=rctx.Ebp;
	regs->esi=rctx.Esi;		regs->edi=rctx.Edi;

	regs->eip=rctx.Eip;
	regs->eflags=rctx.EFlags;
#endif

#ifdef X86_64
	regs->rax=rctx.Rax;		regs->rcx=rctx.Rcx;
	regs->rdx=rctx.Rdx;		regs->rbx=rctx.Rbx;
	regs->rsp=rctx.Rsp;		regs->rbp=rctx.Rbp;
	regs->rsi=rctx.Rsi;		regs->rdi=rctx.Rdi;
	regs->r8=rctx.R8;		regs->r9=rctx.R9;
	regs->r10=rctx.R10;		regs->r11=rctx.R11;
	regs->r12=rctx.R12;		regs->r13=rctx.R13;
	regs->r14=rctx.R14;		regs->r15=rctx.R15;

	regs->rip=rctx.Rip;
	regs->rflags=rctx.EFlags;
#endif
}

#if 1
void *thMutex()
{
	CRITICAL_SECTION *pCS;
	pCS=malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSectionAndSpinCount(pCS, 0x00000400);
	return(pCS);
}

void thLockMutex(void *p)
{
	if(!p)return;
	EnterCriticalSection((CRITICAL_SECTION *)p);
}

int thTryLockMutex(void *p)
{
	int i;

	if(!p)return(-1);
	i=TryEnterCriticalSection((CRITICAL_SECTION *)p);
	if(!i)return(-1);
	return(0);
}

void thUnlockMutex(void *p)
{
	if(!p)return;
	LeaveCriticalSection((CRITICAL_SECTION *)p);
}

void thFreeMutex(void *p)
{
	if(!p)return;
	DeleteCriticalSection((CRITICAL_SECTION *)p);
	free(p);
}
#endif

#if 1
struct th_fastmutexinfo_s {
int state;
char *fn;
int ln;
};

void *thFastMutex()
{
	int *pi;
//	pi=malloc(sizeof(int));
	pi=malloc(sizeof(struct th_fastmutexinfo_s));
	memset(pi, 0, sizeof(struct th_fastmutexinfo_s));
	*pi=0;
	return((void *)pi);
}

void thLockFastMutex(void *p)
{
	int i;

	if(!p)return;

	i=65536;
	while((i>0) && InterlockedExchange((PLONG)p, 2))i--;
	if(i>0)
	{
		*(volatile int *)p=1;
		return;
	}

	i=65536;
	while((i>0) && InterlockedExchange((PLONG)p, 2))
		{ Sleep(0); i--; }
	if(i>0)
	{
		*(volatile int *)p=1;
		return;
	}

	i=16384;
	while((i>0) && InterlockedExchange((PLONG)p, 2))
		{ Sleep(1); i--; }
	if(i>0)
	{
		*(volatile int *)p=1;
		return;
	}

	printf("thLockFastMutex: deadlock\n");
//	*(volatile int *)p=1;
}

int thTryLockFastMutex(void *p)
{
	int i;

	if(!p)return(-1);
	while(1)
	{
		if(*(volatile int *)p)return(-1);
		if(!InterlockedExchange((PLONG)p, 1))break;
	}
	return(0);
}

void thLockFastMutexLLn(void *p, char *fn, int ln)
{
	int i, n;

	if(!p)return;
	
	n=1<<20;
	while(InterlockedExchange((PLONG)p, 2) && n--)
		Sleep(0);
	*(volatile int *)p=1;

	if(n<=0)
	{
		printf("thLockFastMutexLLn: Deadlock %s:%d <-> %s:%d\n",
			((struct th_fastmutexinfo_s *)p)->fn,
			((struct th_fastmutexinfo_s *)p)->ln,
			fn, ln);
	}

//	thLockFastMutex(p);

	((struct th_fastmutexinfo_s *)p)->fn=fn;
	((struct th_fastmutexinfo_s *)p)->ln=ln;
}

int thTryLockFastMutexLLn(void *p, char *fn, int ln)
{
	if(thTryLockFastMutex(p)>=0)
	{
		((struct th_fastmutexinfo_s *)p)->fn=fn;
		((struct th_fastmutexinfo_s *)p)->ln=ln;
		return(0);
	}
	return(-1);
}

void thUnlockFastMutex(void *p)
{
	if(!p)return;
	if(InterlockedExchange((PLONG)p, 0)>1)
		Sleep(0);
}

void thFreeFastMutex(void *p)
{
	if(!p)return;
	free(p);
}
#endif

void thSleep(int ms)
{
	Sleep(ms);
}


thContext *thGetContext()
{
	thContext *inf;
	HANDLE hcth;
	int i;

	bipro_thread_init();

	inf=(thContext *)TlsGetValue(bipro_threadctx_tls);
	if(!inf)
	{
		for(i=0; i<bipro_thread_num; i++)
			if(!bipro_thread_inf[i])
				break;

		if(i>=bipro_thread_num)i=bipro_thread_num++;

		inf=malloc(sizeof(thContext));
		memset(inf, 0, sizeof(thContext));
		inf->id=i;
//		inf->hdl=GetCurrentThread();

		DuplicateHandle(GetCurrentProcess(), 
			GetCurrentThread(), 
			GetCurrentProcess(),
			&hcth, 
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);

		inf->hdl=hcth;

		inf->tls=malloc(1024*sizeof(void *));
		memset(inf->tls, 0, 1024*sizeof(void *));
		inf->sz_tls=1024;

		TlsSetValue(bipro_threadctx_tls, (void *)inf);
		TlsSetValue(bipro_threadtls_tls, (void *)(inf->tls));

		bipro_thread_inf[inf->id]=inf;
	}
	return(inf);
}

int thGetTlsTlsW32()
{
	bipro_thread_init();
	return(bipro_threadtls_tls);
}

void thWaitPtr(void *ptr)
{
	if(!ptr)return;

	while(1)
	{
		WaitForSingleObject(bipro_thread_ptrevent, 10);
		InterlockedDecrement((long *)(&bipro_thread_ptrwait));
		if(bipro_thread_sigptr==ptr)break;
		InterlockedIncrement((long *)(&bipro_thread_ptrwait2));
		while(bipro_thread_sigptr!=UNDEFINED)SwitchToThread();
		InterlockedIncrement((long *)(&bipro_thread_ptrwait));
		InterlockedDecrement((long *)(&bipro_thread_ptrwait2));
	}
}

void thSignalPtr(void *ptr)
{
	if(!ptr)return;
	if(!bipro_thread_ptrwait)return;

	while(bipro_thread_sigptr)SwitchToThread();
	bipro_thread_sigptr=ptr;
	SetEvent(bipro_thread_ptrevent);
	while(bipro_thread_ptrwait)SwitchToThread();
	ResetEvent(bipro_thread_ptrevent);

	bipro_thread_sigptr=UNDEFINED;
	while(bipro_thread_ptrwait2)SwitchToThread();
	bipro_thread_sigptr=NULL;
}

#endif

#if defined(linux)

void BIPRO_SuspendThreads()
{
}

void BIPRO_ResumeThreads()
{
}

void *BIPRO_ThreadProcLnx(void *parm) 
{
	thContext *inf;
	int i;

	inf=parm;
//	TlsSetValue(bipro_threadctx_tls, (void *)inf);
	pthread_setspecific(bipro_threadctx_tls2, (void *)inf);

//	bipro_thread_stktop[inf->id]=&parm;
	bipro_thread_inf[inf->id]=inf;
	inf->stktop=&parm;

	i=inf->fcn(inf->ptr);

//	bipro_thread_hdl[inf->id]=NULL;
//	bipro_thread_stktop[inf->id]=NULL;
	bipro_thread_inf[inf->id]=NULL;
	return(NULL);
}

void *thThread(int (*fcn)(void *ptr), void *ptr)
{
	thContext *inf;
	pthread_t *th;
	int i;

	bipro_thread_init();

//	for(i=0; i<bipro_thread_num; i++)
//		if(!bipro_thread_hdl[i])
//			break;

	for(i=0; i<bipro_thread_num; i++)
		if(!bipro_thread_inf[i])
			break;

	if(i>=bipro_thread_num)i=bipro_thread_num++;

	inf=malloc(sizeof(thContext));
	memset(inf, 0, sizeof(thContext));

	inf->id=i;
//	bipro_thread_hdl[i]=UNDEFINED;
//	bipro_thread_stktop[i]=UNDEFINED;

	inf->tls=malloc(1024*sizeof(void *));
	memset(inf->tls, 0, 1024*sizeof(void *));

	inf->hdl=UNDEFINED;
	inf->stktop=UNDEFINED;

	inf->fcn=fcn;
	inf->ptr=ptr;

	th=malloc(sizeof(pthread_t));
	i = pthread_create(th, NULL, BIPRO_ThreadProcLnx, (void *)inf);

//	bipro_thread_hdl[i]=th;
	inf->hdl=th;

	return(th);
}

void *thMutexLL()
{
	pthread_mutex_t *mutex;
	mutex=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	return((void *)mutex);
}

void *thMutex()
{
	pthread_mutex_t *mutex;
	mutex=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	return((void *)mutex);
}

void thLockMutex(void *p)
{
	if(!p)return;
	pthread_mutex_lock((pthread_mutex_t *)p);
}

int thTryLockMutex(void *p)
{
	if(!p)return;
	pthread_mutex_trylock((pthread_mutex_t *)p);
}

void thUnlockMutex(void *p)
{
	if(!p)return;
	pthread_mutex_unlock((pthread_mutex_t *)p);
}

void thFreeMutex(void *p)
{
	if(!p)return;
	free(p);
}

//need faster implementation...
void *thFastMutex()
	{ return(thMutex()); }
void thLockFastMutex(void *p)
	{ thLockMutex(p); }
int thTryLockFastMutex(void *p)
	{ thTryLockMutex(p); }
void thUnlockFastMutex(void *p)
	{ thUnlockMutex(p); }
void thFreeFastMutex(void *p)
	{ thFreeMutex(p); }


void thSleep(int ms)
{
	usleep(ms*1000);
}

thContext *thGetContext()
{
	thContext *inf;
	inf=pthread_getspecific(bipro_threadctx_tls2);
	if(!inf)
	{
		inf=malloc(sizeof(thContext));
		memset(inf, 0, sizeof(thContext));

		inf->tls=malloc(1024*sizeof(void *));
		memset(inf->tls, 0, 1024*sizeof(void *));

		pthread_setspecific(bipro_threadctx_tls2, (void *)inf);
	}
	return(inf);
}

void thWaitPtr(void *ptr)
{
	if(!ptr)return;

	while(1)
	{
		pthread_mutex_lock(&bipro_threadptr_mutex);
		pthread_cond_wait(&bipro_threadptr_cond,
			&bipro_threadptr_mutex);
		bipro_thread_ptrwait--;
		if(bipro_thread_sigptr==ptr)break;

		bipro_thread_ptrwait2++;
		pthread_mutex_unlock(&bipro_threadptr_mutex);

		while(bipro_thread_sigptr!=UNDEFINED)usleep(0);

		pthread_mutex_lock(&bipro_threadptr_mutex);
		bipro_thread_ptrwait++;
		bipro_thread_ptrwait2--;
		pthread_mutex_unlock(&bipro_threadptr_mutex);
	}

	pthread_mutex_unlock(&bipro_threadptr_mutex);
}

void thSignalPtr(void *ptr)
{
	if(!ptr)return;
	if(!bipro_thread_ptrwait)return;

	while(bipro_thread_sigptr)usleep(0);
	bipro_thread_sigptr=ptr;
	pthread_cond_broadcast(&bipro_threadptr_cond);
	while(bipro_thread_ptrwait)usleep(0);
//	ResetEvent(bipro_threadptr_cond);

	bipro_thread_sigptr=UNDEFINED;
	while(bipro_thread_ptrwait2)usleep(0);
	bipro_thread_sigptr=NULL;
}

#endif

char *bipro_exesec_n[256];
s64 bipro_exesec_va[256];
s64 bipro_exesec_sz[256];
int bipro_n_exesec;

char *bipro_strdup(char *str)
{
	return(strdup(str));
}

void BIPRO_AddExeSection(char *name, s64 va, s64 sz, int fl)
{
	int i;

	if(bipro_n_exesec>=256)
		return;

	i=bipro_n_exesec++;
	bipro_exesec_n[i]=bipro_strdup(name);
	bipro_exesec_va[i]=va;
	bipro_exesec_sz[i]=sz;
}

char *BIPRO_LookupExeSectionPtrName(void *ptr)
{
	s64 va, vae;
	int i, j, k;
	
	for(i=0; i<bipro_n_exesec; i++)
	{
		va=bipro_exesec_va[i];
		vae=va+bipro_exesec_sz[i];
		if(((byte *)ptr)<((byte *)va))
			continue;
		if(((byte *)ptr)>=((byte *)vae))
			continue;
		return(bipro_exesec_n[i]);
	}
	return(NULL);
}

char *bipro_sym_name[1<<14];
void *bipro_sym_addr[1<<14];
int bipro_sym_nsym=0;

void BIPRO_AddSym(char *name, void *ptr)
{
	int i;
	
	i=bipro_sym_nsym++;
	bipro_sym_name[i]=bipro_strdup(name);
	bipro_sym_addr[i]=ptr;
}

void *BIPRO_LookupLabel(char *name)
{
	void *ptr;
	int i;
	
	for(i=0; i<bipro_sym_nsym; i++)
		if(!strcmp(bipro_sym_name[i], name))
			return(bipro_sym_addr[i]);
	
	ptr=BIPRO_LookupLabelOS(name);
	return(ptr);
}

#ifdef _WIN32

#define HAS_LOOKUPLABELOS

int BIPRO_GetModuleListOS(char **buf, int max)
{
	int i, n;

	n=bipro_numdll;
	if(n>max)n=max;
	for(i=0; i<n; i++)
		buf[i]=bipro_hmodname[i];
	return(n);
}

char **BIPRO_GetEnvironOS()
{
	static char *ta[256];
	char *s, *buf;
	int n;

	if(bipro_os_env)
		return(bipro_os_env);

	buf=GetEnvironmentStrings();

	bipro_os_env=bipro_os_envarr;
	s=buf; n=0;
	while(*s)
	{
		bipro_os_env[n++]=bipro_strdup(s);
		s+=strlen(s)+1;
	}
	bipro_os_env[n]=NULL;
	
	FreeEnvironmentStrings(buf);
	return(bipro_os_env);
}

void BIPRO_ProbeTst(byte *buf, int sz)
{
	int i, j;

	for(i=0; i<(sz/4096); i++)
		j=buf[i*4096+2048];
}

void BIPRO_UpdateDllsOS(void *hmod)
{
	HMODULE hdl;
	DWORD dwPid;
	HANDLE hSnap;
	MODULEENTRY32 me32;

	hdl=(HMODULE)hmod;

	dwPid=GetCurrentProcessId();
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

	if(hSnap!=INVALID_HANDLE_VALUE)
	{
		memset(&me32, 0, sizeof(MODULEENTRY32));
		me32.dwSize=sizeof(MODULEENTRY32);
		if(Module32First(hSnap, &me32))
		{
			while(1)
			{
				printf("(MOD) %s %p %d\n",
					me32.szModule,
					me32.modBaseAddr, me32.modBaseSize);

//				BIPRO_ProbeTst(me32.modBaseAddr, me32.modBaseSize);

				if(!hdl || (me32.hModule==hdl))
				{
					BIPRO_AddExeSection(me32.szModule,
						(s64)me32.modBaseAddr,
						me32.modBaseSize, 0);
				}

				BIPRO_LoadDynamicOS(me32.szModule);
				if(!Module32Next(hSnap, &me32))
					break;
			}
		}
		CloseHandle(hSnap);
	}
}

int BIPRO_InitDllsOS()
{
	static int init=0;

	if(init)return(0);
	init=1;

	if(!bipro_hmodapp)
		bipro_hmodapp=GetModuleHandle(NULL);

	BIPRO_UpdateDllsOS(NULL);

	return(1);
}

int BIPRO_LoadDynamicOS(char *name)
{
	char buf[256];
	HMODULE hdl;
	char *s;
	int i;

	BIPRO_InitDllsOS();

	for(i=0; i<bipro_numdll; i++)
		if(!strcmp(bipro_hmodname[i], name))
			return(0);

//	if(bipro_lookup_blacklist(name)>0)
//		return(-1);

	//LoadLibrary does not like '/'...
	strcpy(buf, name);
	s=buf; while(*s) { if(*s=='/')*s='\\'; s++; }

	hdl=GetModuleHandle(buf);
	if(!hdl)hdl=LoadLibrary(buf);

	if(hdl)
	{
		printf("BIPRO_LoadDynamicOS: Pass Load lib %s\n", name);

		i=bipro_numdll++;
		bipro_hmodname[i]=bipro_strdup(name);
		bipro_hmoddll[i]=hdl;

//		BIPRO_UpdateDllsOS(hdl);
		return(0);
	}

	printf("BIPRO_LoadDynamicOS: Fail Load lib %s\n", name);

	return(-1);
}

void *BIPRO_LoadModuleFileDataOS(char *name, int *rsz)
{
	char nbuf[1024];
	HMODULE hdl;
	HANDLE hFile;
	LARGE_INTEGER szFile;
	DWORD dwBytesRead;
	void *buf;
	char *s;
	int i, sz;

	BIPRO_InitDllsOS();

//	if(bipro_lookup_blacklist(name)>0)
//		return(-1);

	hdl=GetModuleHandle(name);
	i=GetModuleFileName(hdl, nbuf, 1024);
	if(!i)return(NULL);

	hFile=CreateFile(nbuf,		// module name
		GENERIC_READ,			// open for reading
		FILE_SHARE_READ,		// share for reading
		NULL,					// default security
		OPEN_EXISTING,			// existing file only
		FILE_ATTRIBUTE_NORMAL,	// normal file
		NULL);					// no attr. template

	if (hFile==INVALID_HANDLE_VALUE) 
		return(NULL);

	if(!GetFileSizeEx(hFile, &szFile))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	sz=szFile.QuadPart;

	//fail file is overly large
	if(sz>(1<<26))
	{
		CloseHandle(hFile);
		return(NULL);
	}

	buf=malloc(sz);

	if(!ReadFile(hFile, buf, sz, &dwBytesRead, NULL))
	{
//	printf("Could not read from file (error %d)\n", GetLastError());
		CloseHandle(hFile);
		return(NULL);
	}

	CloseHandle(hFile);
	if(rsz)*rsz=sz;
	return(buf);
}

void BIPRO_FreeModuleDataOS(void *buf)
{
	free(buf);
}

void BIPRO_UpdateProxyOS(char *name, void *ptr)
{
	char ntmp[256];
	void *p;
	int i;

	BIPRO_InitDllsOS();

#ifdef X86_32
	sprintf(ntmp, "__iproxy_%s", name);
#else
	sprintf(ntmp, "_iproxy_%s", name);
#endif

	if(bipro_hmodapp)
	{
		p=(void *)GetProcAddress(bipro_hmodapp, TEXT(ntmp));
		if(p)*(void **)p=ptr;
	}

	for(i=0; i<bipro_numdll; i++)
	{
		p=(void *)GetProcAddress(bipro_hmoddll[i], TEXT(ntmp));
		if(p)*(void **)p=ptr;
	}
}

void *BIPRO_LookupLabelOS_1(char *name)
{
	char ntmp[64];
	void *p, *q;
	int i;

	p=NULL;

	if(bipro_hmodapp)
	{
		p=(void *)GetProcAddress(bipro_hmodapp, TEXT(name));
		if(p)return(p);
	}

	for(i=0; i<bipro_numdll; i++)
	{
		p=(void *)GetProcAddress(bipro_hmoddll[i], TEXT(name));
		if(p)break;
	}

	return(p);
}

void *BIPRO_LookupLabelOS_2(char *name, int *rfl)
{
	char ntmp[256];
	void *p;
	int i;

	*rfl=0;

	if(*name!='_')
	{
		*rfl|=1;

		for(i=9; i>0; i--)
		{
			sprintf(ntmp, "BSRT%d__%s", i, name);
			p=BIPRO_LookupLabelOS_1(ntmp);
			if(p)break;
		}

		if(p)return(p);

		sprintf(ntmp, "BSRT__%s", name);
		p=BIPRO_LookupLabelOS_1(ntmp);
		if(p)return(p);

		*rfl&=~1;
	}

	p=BIPRO_LookupLabelOS_1(name);
	return(p);
}

void *BIPRO_LookupLabelOS(char *name)
{
	void *p, *q;
	int fl;

	BIPRO_InitDllsOS();

	p=BIPRO_LookupLabelOS_2(name, &fl);
	return(p);
}

int BIPRO_InitDbgHelpOS()
{
	static int init=0;
	DWORD dwPid;
	HANDLE hSnap;
	MODULEENTRY32 me32;

	if(init)return(0);
	init=1;

	BIPRO_LoadDynamicOS("dbghelp");

	SymFromAddr_f=BIPRO_LookupLabel("SymFromAddr");
	SymSetOptions_f=BIPRO_LookupLabel("SymSetOptions");
	SymInitialize_f=BIPRO_LookupLabel("SymInitialize");

	if(!SymFromAddr_f || !SymSetOptions_f || !SymInitialize_f)
	{
		printf("Failed load dbgHelp\n");
		return(-1);
	}

	SymSetOptions_f(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize_f(GetCurrentProcess(), NULL, TRUE);

	return(1);
}

char *BIPRO_NameOS(void *addr, void **addr2)
{
	char buf[1024];
	HANDLE hProcess;
	DWORD64 offs;
	SYMBOL_INFO *pSym;
	int i;

	BIPRO_InitDbgHelpOS();

	if(!SymFromAddr_f)
		return(NULL);

	memset(buf, 0, 1024);
	pSym=(SYMBOL_INFO *)buf;
	pSym->SizeOfStruct=sizeof(SYMBOL_INFO);
	pSym->MaxNameLen=1024-sizeof(SYMBOL_INFO);

	hProcess=GetCurrentProcess();
	i=SymFromAddr_f(hProcess, (DWORD64)addr, &offs, pSym);

	if(i)
	{
		if(addr2)*addr2=(void *)(pSym->Address);
		return(bipro_strdup(pSym->Name));
	}

	if(addr2)*addr2=NULL;
	return(NULL);
}

int BIPRO_ProcessMAP(char *name)
{
	char nbuf[256], tb[256], tb1[32];
	FILE *fd;
	char *s, *t;
	s64 l;
	int i;

	fd=fopen(name, "rb");

	if(!fd)
	{
		sprintf(nbuf, "%s.map", name);
		fd=fopen(nbuf, "rb");
	}

	if(!fd)
	{
		printf("BIPRO_ProcessMAP: fail open\n");
		return(-1);
	}

	printf("BIPRO_ProcessMAP: Load Map '%s'\n", name);

	while(!feof(fd))
	{
		memset(nbuf, 0, 256);
		fgets(nbuf, 255, fd);

		s=nbuf;
		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		i=4;
		while(i--)
		{
			if((*s>='0') && (*s<='9')) { s++; continue; }
			if((*s>='A') && (*s<='F')) { s++; continue; }
			if((*s>='a') && (*s<='f')) { s++; continue; }
			break;
		}

		if(!i)continue;
		if(*s!=':')continue;

		s++;
		i=8;
		while(i--)
		{
			if((*s>='0') && (*s<='9')) { s++; continue; }
			if((*s>='A') && (*s<='F')) { s++; continue; }
			if((*s>='a') && (*s<='f')) { s++; continue; }
			break;
		}

		if(!i)continue;
		if(!*s)continue;
		if(*s>' ')continue;

		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		if((*s=='_') ||
			((*s>='a') && (*s<='z')) ||
			((*s>='A') && (*s<='Z')))

		{
			t=tb;
			while(*s)
			{
				if(*s=='_') { *t++=*s++; continue; }
				if((*s>='0') && (*s<='9'))
					{ *t++=*s++; continue; }
				if((*s>='a') && (*s<='z'))
					{ *t++=*s++; continue; }
				if((*s>='A') && (*s<='Z'))
					{ *t++=*s++; continue; }
				break;
			}
			*t++=0;

			if(!*s)continue;
			if(*s>' ')continue;
		}

		while(*s && (*s<=' '))s++;
		if(!*s)continue;

		if(	((*s>='0') && (*s<='9')) ||
			((*s>='a') && (*s<='f')) ||
			((*s>='A') && (*s<='F')))

		{
			t=tb1;
			while(*s)
			{
				if((*s>='0') && (*s<='9'))
					{ *t++=*s++; continue; }
				if((*s>='a') && (*s<='f'))
					{ *t++=*s++; continue; }
				if((*s>='A') && (*s<='F'))
					{ *t++=*s++; continue; }
				break;
			}
			*t++=0;

			if(!*s)continue;
			if(*s>' ')continue;
		}


		s=tb1; l=0;
		while(*s)
		{
			i=0;
			if((*s>='0') && (*s<='9'))i=*s-'0';
			if((*s>='a') && (*s<='f'))i=*s-'a'+10;
			if((*s>='A') && (*s<='F'))i=*s-'A'+10;

			s++;
			l=l*16+i;
		}

//		printf("%s %lX\n", tb, l);

		BIPRO_AddSym(tb, (void *)(l));
	}

	fclose(fd);
	return(0);
}


int BIPRO_ProcessEXE(char *name)
{
	static int init=0;

	s64 secva[8];
	int secvs[8];
	char nbuf[256];

	void *p, *q;
	byte *buf;
	FILE *fd;
	char *s;
	int sz, peoffs, coff_hdr, optoffs, symoffs, nsyms;
	int secoffs, nsecs, stroffs, mach;
	s64 disp, addr;
//	int syo;
	int i, j, k, l;

//	BIPRO_InitLink();

	if(init)return(-1);

	return(-1);	//strategy no longer used

	printf("BIPRO_ProcessEXE '%s'\n", name);

//	printf("open %s\n", name);
	fd=fopen(name, "rb");

	if(!fd)
	{
		sprintf(nbuf, "%s.exe", name);
		fd=fopen(nbuf, "rb");
	}

	if(!fd)
	{
		printf("BIPRO_ProcessEXE: fail open\n");
		return(-1);
	}

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=(byte *)malloc(sz);
	fread(buf, 1, sz, fd);
	fclose(fd);

	printf("EXE %d (0x%X) bytes\n", sz, sz);

	if((buf[0]!='M') || (buf[1]!='Z'))
	{
		printf("BIPRO_ProcessEXE: EXE Sig Fail\n");
		return(-1);
	}

	peoffs=buf[0x3C]|(buf[0x3D]<<8)|(buf[0x3E]<<16)|(buf[0x3F]<<24);

	if((buf[peoffs+0]!='P') || (buf[peoffs+1]!='E'))
	{
		printf("BIPRO_ProcessEXE: PE Sig Fail\n");
		return(-1);
	}

//	printf("PE Sig At 0x%X\n", peoffs);
	coff_hdr=peoffs+4;

	i=buf[coff_hdr+0]|(buf[coff_hdr+1]<<8);
	mach=i;
//	printf("\tMachine 0x%04X\n", i);

	symoffs=buf[coff_hdr+8]|(buf[coff_hdr+9]<<8)|
		(buf[coff_hdr+10]<<16)|(buf[coff_hdr+11]<<24);
	nsyms=buf[coff_hdr+12]|(buf[coff_hdr+13]<<8)|
		(buf[coff_hdr+14]<<16)|(buf[coff_hdr+15]<<24);

//	printf("\tSyms 0x%X %d\n", symoffs, nsyms);

	stroffs=symoffs+(nsyms*18);

	i=buf[coff_hdr+16]|(buf[coff_hdr+17]<<8);
	optoffs=coff_hdr+20;
	secoffs=coff_hdr+20+i;

	nsecs=buf[coff_hdr+2]|(buf[coff_hdr+3]<<8);

	if(mach==0x8664)
	{
		disp=	( (s64)buf[coff_hdr+20+24])|
			(((s64)buf[coff_hdr+20+25])<<8)|
			(((s64)buf[coff_hdr+20+26])<<16)|
			(((s64)buf[coff_hdr+20+27])<<24)|
			(((s64)buf[coff_hdr+20+28])<<32)|
			(((s64)buf[coff_hdr+20+29])<<40)|
			(((s64)buf[coff_hdr+20+30])<<48)|
			(((s64)buf[coff_hdr+20+31])<<56);
	}else
	{
		disp=buf[coff_hdr+20+28]|(buf[coff_hdr+20+29]<<8)|
			(buf[coff_hdr+20+30]<<16)|(buf[coff_hdr+20+31]<<24);
	}

	printf("\tDisplacement 0x%08X%08X\n", (u32)(disp>>32), (u32)disp);

//	printf("\tSecs 0x%X %d\n", secoffs, nsecs);

//	printf("\nSections:\n");
	for(i=0; i<nsecs; i++)
	{
		j=secoffs+(i*40);

		s=(char *)(buf+j);
		l=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		k=buf[j+12]|(buf[j+13]<<8)|(buf[j+14]<<16)|(buf[j+15]<<24);

		secva[i]=k+disp;
		secvs[i]=l;

		printf("\t%d %s 0x%08X%08X %d\n", i+1, s,
			(u32)(secva[i]>>32), (u32)secva[i], l);

		BIPRO_AddExeSection(s, secva[i], secvs[i], 0);
	}

	for(i=0; i<nsyms; i++)
	{
		j=symoffs+(i*18);

		if(buf[j])
		{
			memset(nbuf, 0, 16);
			memcpy(nbuf, buf+j, 8);
			s=nbuf;
		}else
		{
			k=buf[j+4]|(buf[j+5]<<8)|(buf[j+6]<<16)|(buf[j+7]<<24);
			s=(char *)(buf+stroffs+k);
		}

		k=buf[j+8]|(buf[j+9]<<8)|(buf[j+10]<<16)|(buf[j+11]<<24);
		l=buf[j+12]|(buf[j+13]<<8);

		if((l>0) && (l<=nsecs))addr=secva[l-1]+k;

//		if((*s=='.') || ((s[0]=='_') && (s[1]=='_')))
//		if((*s=='.') || ((s[0]=='_') && (s[1]=='_')) || (*s!='_'))
		if(*s!='_')
		{
			i+=buf[j+17];
			continue;
		}

//		printf("\t%s 0x%08X %d\n", s, k, l);

		BIPRO_AddSym(s+1, (void *)addr);

		i+=buf[j+17];
	}


	if(!nsyms)
	{
		printf("BIPRO_ProcessEXE: Warning: "
			"EXE has stripped symbols \n");

		strcpy(nbuf, name);
		s=nbuf+strlen(nbuf);
		while((s>nbuf) && (*s!='.'))s--;

		if(!strcmp(s, ".exe") || !strcmp(s, ".EXE"))
		{
			strcpy(s, ".map");
		}else
		{
			strcat(nbuf, ".map");
		}

		BIPRO_ProcessMAP(nbuf);
	}

//	p=BIPRO_FetchSym("BIPRO_ProcessEXE");
	p=BIPRO_LookupLabel("BIPRO_ProcessEXE");
	q=(void *)(&BIPRO_ProcessEXE);
	if(p!=q)
	{
		printf("BIPRO_ProcessEXE: Alignment Failed %p->%p\n", p, q);
//		bipro_sym_nsym=0;
//		return(-1);
	}

	init=1;
	return(0);
}

#endif

#ifndef HAS_LOOKUPLABELOS
#define HAS_LOOKUPLABELOS
void *BIPRO_LookupLabelOS(char *name)
{
	return(NULL);
}
#endif

#ifndef HAS_LOOKUPADDRNAMEOS
#define HAS_LOOKUPADDRNAMEOS
char *BIPRO_LookupAddrNameOS(void *ptr, void **rbase)
{
	return(NULL);
}
#endif

#if 1
char *BIPRO_GetNamePtr(void *ptr)
{
	void *q;
	char *s;
	int i;

	for(i=0; i<bipro_sym_nsym; i++)
		if(bipro_sym_addr[i]==ptr)
			return(bipro_sym_name[i]);

	s=BIPRO_LookupAddrNameOS(ptr, &q);
	if(s && (q==ptr))return(s);

	return(NULL);
}

char *BIPRO_GetLastNamePtr(void *ptr, void **rbp)
{
	void *p, *bp;
	int i, j, bd;
	char *s, *bn;

	bp=NULL; bn=NULL;
	for(i=0; i<bipro_sym_nsym; i++)
	{
		p=bipro_sym_addr[i];
		if(p>ptr)continue;
		if(p==ptr)
		{
			if(rbp)*rbp=p;
			return(bipro_sym_name[i]);
		}

		j=((byte *)ptr)-((byte *)p);
		if(j<0)j=999999999;
		if(!bp || (j<bd))
			{ bp=p; bd=j; bn=bipro_sym_name[i]; }

	}

	s=BIPRO_LookupAddrNameOS(ptr, &p);
	j=((byte *)ptr)-((byte *)p);
	if(j<0)j=999999999;
	if(!bp || (j<bd))
		{ bp=p; bd=j; bn=s; }

	if(rbp)*rbp=bp;
	return(bn);
}


char *BIPRO_GetAddrName(void *ptr)
{
	return(BIPRO_GetNamePtr(ptr));
}

char *BIPRO_GetAddrLastName(void *ptr, void **rbp)
{
	return(BIPRO_GetLastNamePtr(ptr, rbp));
}

char *BIPRO_LookupSectionAddrName(void *ptr)
{
	char *s;

	s=BIPRO_LookupExeSectionPtrName(ptr);
	if(s)return(s);

	return(NULL);
}
#endif

#ifdef _BGBCC

void *thMutex()
{
//	pthread_mutex_t *mutex;
//	mutex=malloc(sizeof(pthread_mutex_t));
//	pthread_mutex_init(mutex, NULL);
//	return((void *)mutex);
}

void thLockMutex(void *p)
{
	if(!p)return;
//	pthread_mutex_lock((pthread_mutex_t *)p);
}

int thTryLockMutex(void *p)
{
	if(!p)return;
//	pthread_mutex_trylock((pthread_mutex_t *)p);
}

void thUnlockMutex(void *p)
{
	if(!p)return;
//	pthread_mutex_unlock((pthread_mutex_t *)p);
}

void thFreeMutex(void *p)
{
	if(!p)return;
	free(p);
}

//need faster implementation...
void *thFastMutex()
	{ return(thMutex()); }
void thLockFastMutex(void *p)
	{ thLockMutex(p); }
int thTryLockFastMutex(void *p)
	{ thTryLockMutex(p); }
void thUnlockFastMutex(void *p)
	{ thUnlockMutex(p); }
void thFreeFastMutex(void *p)
	{ thFreeMutex(p); }


void thSleep(int ms)
{
//	usleep(ms*1000);
}

void *thThread(int (*fcn)(void *ptr), void *ptr)
{
	return(NULL);
}

#endif

