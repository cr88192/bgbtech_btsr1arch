#ifndef __THREADS_H
#define __THREADS_H

#include <time.h>

#define ONCE_FLAG_INIT			0
#define TSS_DTOR_ITERATIONS		256

#define thread_local			_Thread_local


typedef enum thrd_status_s thrd_status_t;
typedef enum mtx_type_s mtx_type_t;

enum thrd_status_s {

    thrd_success = 0,
    thrd_nomem = 1,
    thrd_timedout = 2,
    thrd_busy = 3,
    thrd_error = 4
};

enum mtx_type_s {
    mtx_plain = 0,
    mtx_recursive = 1,
    mtx_timed = 2
};

typedef long thrd_t;
typedef int (*thrd_start_t)(void *ptr);
typedef void (*tss_dtor_t)(void *ptr);

// typedef int mtx_t;
typedef struct thrd_mtx_s mtx_t;
typedef int cnd_t;
typedef int tss_t;

typedef int once_flag;

struct thrd_mtx_s {
int vtid;			//locked thread ID
short vcor;			//core ID
short vcflag;		//core flags
long long vpad;	//aligning pad for now
};

void call_once( once_flag* flag, void (*func)(void) );

int thrd_create( thrd_t *thr, thrd_start_t func, void *arg );
int thrd_equal( thrd_t lhs, thrd_t rhs );
thrd_t thrd_current(void);

int thrd_sleep(
	const struct timespec* duration,
	struct timespec* remaining );
void thrd_yield(void);
_Noreturn void thrd_exit( int res );
int thrd_detach( thrd_t thr );
int thrd_join( thrd_t thr, int *res );


int mtx_init( mtx_t* mutex, int type );
int mtx_lock( mtx_t* mutex );
int mtx_timedlock(
	mtx_t *restrict mutex,
	const struct timespec *restrict time_point );
int mtx_trylock( mtx_t *mutex );
int mtx_unlock( mtx_t *mutex );
void mtx_destroy( mtx_t *mutex );


int cnd_init( cnd_t* cond );
int cnd_signal( cnd_t *cond );
int cnd_broadcast( cnd_t *cond );
int cnd_wait( cnd_t* cond, mtx_t* mutex );
int cnd_timedwait(
	cnd_t* restrict cond, mtx_t* restrict mutex,
	const struct timespec* restrict time_point );
void cnd_destroy( cnd_t* cond );

int tss_create( tss_t* tss_key, tss_dtor_t destructor );
void *tss_get( tss_t tss_key );
int tss_set( tss_t tss_id, void *val );
void tss_delete( tss_t tss_id );

#endif
