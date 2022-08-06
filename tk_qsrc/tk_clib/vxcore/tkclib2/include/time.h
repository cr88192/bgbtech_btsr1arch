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

#ifndef STD_TIME_H
#define STD_TIME_H

#define CLOCKS_PER_SEC 1000
#define NULL ((void *)0)

#ifndef STD_SIZE_T
#define STD_SIZE_T
typedef unsigned long size_t;
#endif

#ifndef STD_CLOCK_T
#define	STD_CLOCK_T
typedef unsigned int clock_t;
#endif

#ifndef STD_TIME_T
#define	STD_TIME_T
typedef	long long time_t;
#endif

#ifndef STD_TM_S
#define STD_TM_S
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
#endif

#ifndef STD_TIMESPEC_S
#define STD_TIMESPEC_S
struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};
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
