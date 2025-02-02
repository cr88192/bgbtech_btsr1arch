#include "threads.h"
#include "stdint.h"

int TK_GetCurrentThreadId();
void TK_YieldCurrentThread();
void TK_ExitCurrentThread(int res);
void TK_DetachThread(int pid);
int64_t TK_JoinThread(int pid);
void TK_SleepCurrentThread(int usec);
int TK_AllocNewTlsA(void);

int TK_MutexTryLock(int *mtx, int flag);
int TK_MutexUnlock(int *mtx);
void TK_FastFlushCacheL1D();

tss_dtor_t	thr_dtor_fun[256];
int			thr_dtor_key[256];
int			thr_n_dtor;


int _mtx_sync_get(int *ptr);
int _mtx_sync_set(int *ptr, int val);
int _mtx_sync_getw(short *ptr);
int _mtx_sync_setw(short *ptr, int val);

int _mtx_sync_getcpu();

#ifdef __BJX2__
__asm {
_mtx_sync_get:
	SNIPE.DC	R4, R6
	MOV.L		(R6), R2
	MOV.L		(R4), R2
	RTS

_mtx_sync_set:
	SNIPE.DC	R4, R6
	MOV.L		(R6), R3
	MOV.L		(R4), R2
	MOV.L		R5, (R4)
	MOV.L		(R6), R3
	RTS

_mtx_sync_getw:
	SNIPE.DC	R4, R6
	MOV.W		(R6), R2
	MOV.W		(R4), R2
	RTS

_mtx_sync_setw:
	SNIPE.DC	R4, R6
	MOV.W		(R6), R3
	MOV.W		(R4), R2
	MOV.W		R5, (R4)
	MOV.W		(R6), R3
	RTS

_mtx_sync_getcpu:
	MOV			1, R2
	RTS
};
#else

int _mtx_sync_get(int *ptr)
{
	return(*ptr);
}

int _mtx_sync_set(int *ptr, int val)
{
	int lval;
	
	lval=*ptr;
	*ptr=val;
	return(lval);
}

int _mtx_sync_getw(short *ptr)
{
	return(*ptr);
}

int _mtx_sync_setw(short *ptr, int val)
{
	int lval;
	
	lval=*ptr;
	*ptr=val;
	return(lval);
}

int _mtx_sync_getcpu()
{
	return(1);
}

#endif

void call_once( once_flag* flag, void (*func)(void) )
{
	int i;
	i=_mtx_sync_set(flag, 1);
	if(!i)
		func();
}

int TK_SpawnNewThread(void *func, void *uptr);

int thrd_create( thrd_t *thr, thrd_start_t func, void *arg )
{
	int tid;

#ifndef __TK_CLIB_DLLSTUB__
	tid = TK_SpawnNewThread(func, arg);
	*thr = tid;
	return(thrd_success);
#endif
}

int thrd_equal( thrd_t lhs, thrd_t rhs )
{
	return(lhs==rhs);
}

thrd_t thrd_current(void)
{
#ifndef __TK_CLIB_DLLSTUB__
	return(TK_GetCurrentThreadId());
#endif
}

int thrd_sleep(
	const struct timespec* duration,
	struct timespec* remaining )
{
	int64_t usec;
	usec = (duration->tv_sec<<20) + (duration->tv_nsec>>10);
#ifndef __TK_CLIB_DLLSTUB__
	TK_SleepCurrentThread(usec);
#endif
	return(thrd_success);
}

void thrd_yield(void)
{
#ifndef __TK_CLIB_DLLSTUB__
	TK_YieldCurrentThread();
#endif
}

_Noreturn void thrd_exit( int res )
{
	int i;

	for(i=0; i<thr_n_dtor; i++)
		{ thr_dtor_fun[i](tss_get(thr_dtor_key[i])); }
#ifndef __TK_CLIB_DLLSTUB__
	TK_ExitCurrentThread(res);
#endif
	while(1);
}

int thrd_detach( thrd_t thr )
{
#ifndef __TK_CLIB_DLLSTUB__
	TK_DetachThread(thr);
#endif
	return(thrd_success);
}

int thrd_join( thrd_t thr, int *res )
{
#ifndef __TK_CLIB_DLLSTUB__
	*res=TK_JoinThread(thr);
#endif
	return(thrd_success);
}

int mtx_init( mtx_t* mutex, int type )
{
//	*mutex = 0;
	memset(mutex, 0, sizeof(mtx_t));
}

int mtx_lock( mtx_t* mutex )
{
	int tid, ntid, tid1, cpu;
	int i, j;
	
	if(!mutex)
		return(thrd_error);

	tid=thrd_current();
	ntid=-tid;
	cpu=_mtx_sync_getcpu();

	/* fast case. */
	i=_mtx_sync_set((int *)mutex, ntid);
	if(!i || (i==tid))
//	if(!i || ((i&0xFFFFF)==tid))
	{
		if(i)
		{
			j=_mtx_sync_set((int *)mutex, i);
			if(i==j)
				return(thrd_success);
		}else
		{
			j=mutex->vcor;
			if(!j || (j==cpu))
			{
				i=_mtx_sync_set((int *)mutex, tid);
				_mtx_sync_setw(&(mutex->vcor), cpu);
				if(i==ntid)
					return(thrd_success);
			}
		}
	}else
	{
		/* restore prior value */
		j=_mtx_sync_set((int *)mutex, i);
	}

#if 0
	while(1)
	{
		while(i)
		{
			thrd_yield();
			i=_mtx_sync_set((int *)mutex, ntid);
		}
		i=_mtx_sync_set((int *)mutex, tid);
		if(i==ntid)
			break;
	}
#endif

	while(1)
	{
		i=TK_MutexTryLock((int *)mutex, 1);
		if(i>0)
			break;
	}

	return(thrd_success);
}

int mtx_timedlock(
	mtx_t *restrict mutex,
	const struct timespec *restrict time_point );

int mtx_trylock( mtx_t *mutex )
{
	int tid, ntid, cpu;
	int i, j;
	
	if(!mutex)
		return(thrd_error);

	tid=thrd_current();
	ntid=-tid;
	cpu=_mtx_sync_getcpu();
	
#if 0
	i=_mtx_sync_set(mutex, ntid);
//	if(!i || (i==tid))
	if(!i || (i==tid))
	{
		i=_mtx_sync_set(mutex, tid);
		if(i==ntid)
			return(thrd_success);
	}
#endif

#if 1
	i=_mtx_sync_set((int *)mutex, ntid);
	if(!i || (i==tid))
//	if(!i || ((i&0xFFFFF)==tid))
	{
		if(i)
		{
			j=_mtx_sync_set((int *)mutex, i);
			if(i==j)
				return(thrd_success);
		}else
		{
			j=mutex->vcor;
			if(!j || (j==cpu))
			{
				i=_mtx_sync_set((int *)mutex, tid);
				_mtx_sync_setw(&(mutex->vcor), cpu);
				if(i==ntid)
					return(thrd_success);
			}
		}
	}else
	{
		/* restore prior value */
		j=_mtx_sync_set((int *)mutex, i);
	}
#endif

	i=TK_MutexTryLock((int *)mutex, 0);
	if(i>0)
		return(thrd_success);

	return(thrd_busy);
}

int mtx_unlock( mtx_t *mutex )
{
	if(!mutex->vcflag)
	{
		/* fast case, simply mark as unlocked. */
		_mtx_sync_set((int *)mutex, 0);
		return(1);
	}

	TK_MutexUnlock((int *)mutex);
	return(1);
}

void mtx_destroy( mtx_t *mutex )
{
}


int cnd_init( cnd_t* cond )
{
//	*cond=0;
	_mtx_sync_set(cond, 0);
	return(thrd_success);
}

int cnd_signal( cnd_t *cond )
{
	int i;
	i=_mtx_sync_get(cond);
	_mtx_sync_set(cond, i-1);
	return(thrd_success);
}

int cnd_broadcast( cnd_t *cond )
{
	_mtx_sync_set(cond, 0);
	return(thrd_success);
}

int cnd_wait( cnd_t* cond, mtx_t* mutex )
{
	int i, id;
	
	mtx_unlock(mutex);
	
	id=_mtx_sync_get(cond);
	_mtx_sync_set(cond, id+1);

	i=_mtx_sync_get(cond);
	while(i>id)
	{
		thrd_yield();
		i=_mtx_sync_get(cond);
	}
	mtx_lock(mutex);
	return(thrd_success);
}

int cnd_timedwait(
	cnd_t* restrict cond, mtx_t* restrict mutex,
	const struct timespec* restrict time_point )
{
}

void cnd_destroy( cnd_t* cond )
{
}

int tss_create( tss_t* tss_key, tss_dtor_t destructor )
{
	int tid, ix;
	
#ifndef __TK_CLIB_DLLSTUB__
	tid=TK_AllocNewTlsA();
#endif

	*tss_key=tid;

	if(destructor)
	{
		ix=thr_n_dtor++;
		thr_dtor_fun[ix]=destructor;
		thr_dtor_key[ix]=tid;
	}
	return(thrd_success);
}

int64_t TK_TlsGet(int key);
int64_t TK_TlsSet(int key, int64_t val);

void *tss_get( tss_t tss_key )
{
#ifndef __TK_CLIB_DLLSTUB__
	return((void *)TK_TlsGet(tss_key));
#endif
}

int tss_set( tss_t tss_id, void *val )
{
#ifndef __TK_CLIB_DLLSTUB__
	TK_TlsSet(tss_id, (int64_t)val);
	return(thrd_success);
#endif
}

void tss_delete( tss_t tss_id )
{
}
