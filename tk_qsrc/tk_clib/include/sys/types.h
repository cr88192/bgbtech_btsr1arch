#ifndef __TK_SYS_TYPES_H
#define __TK_SYS_TYPES_H

#include <stdint.h>

typedef int blkcnt_t;
typedef int blksize_t;
typedef int clockid_t;
typedef int dev_t;

typedef int pid_t;
typedef int uid_t;
typedef int gid_t;
typedef int id_t;

typedef unsigned int fsblkcnt_t;
typedef unsigned int fsfilcnt_t;
typedef unsigned int ino_t;

#ifndef __MODE_T_DEFINED
#define __MODE_T_DEFINED
typedef unsigned int mode_t;
#endif

#ifndef __OFF_T_DEFINED
#define __OFF_T_DEFINED
typedef long long off_t;
#endif

typedef int nlink_t;

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned long size_t;
#endif

#ifndef __SSIZE_T_DEFINED
#define __SSIZE_T_DEFINED
typedef signed long ssize_t;
#endif

typedef int useconds_t;
typedef int suseconds_t;

#ifndef _TIME_T_DEFINED
typedef unsigned long time_t;
#define	_TIME_T_DEFINED
#endif

#ifndef _CLOCK_T_DEFINED
typedef unsigned int clock_t;
#define	_CLOCK_T_DEFINED
#endif

typedef int timer_t;

typedef int key_t;

typedef int pthread_attr_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_t;

typedef int trace_attr_t;
typedef int trace_event_id_t;
typedef int trace_event_set_t;
typedef int trace_id_t;
    
#endif
