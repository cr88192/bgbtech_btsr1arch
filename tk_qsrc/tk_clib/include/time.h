/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  time.h - time header file.                                       */
/*                                                                   */
/*********************************************************************/

#ifndef __TIME_INCLUDED
#define __TIME_INCLUDED

#define CLOCKS_PER_SEC 1000
#define NULL ((void *)0)

#ifndef __SIZE_T_DEFINED
#define __SIZE_T_DEFINED
typedef unsigned long size_t;
#endif

#ifndef _CLOCK_T_DEFINED
typedef unsigned int clock_t;
#define	_CLOCK_T_DEFINED
#endif

#ifndef _TIME_T_DEFINED
typedef	unsigned long time_t;
#define	_TIME_T_DEFINED
#endif

#ifndef _TM_DEFINED /* Also in time.h */
struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};
#define _TM_DEFINED
#endif

#ifndef _TIMESPEC_DEFINED
struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};
#define _TIMESPEC_DEFINED
#endif

time_t time(time_t *timer);
clock_t clock(void);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *timeptr);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
size_t strftime(char *s, size_t maxsize,
                const char *format, const struct tm *timeptr);

#endif
