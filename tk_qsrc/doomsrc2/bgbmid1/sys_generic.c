#include <bgbmid.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

BMID_API int BGBMID_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#else
#ifndef linux
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;

	if(!init)init=t;

	return((unsigned int)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif
#endif
}

BMID_API s64 BGBMID_TimeUS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((t-init)*1000LL);
#else
#ifndef linux
	static s64 init;
	s64 t;

	t=clock();
//	t*=CLOCKS_PER_SEC/1000.0;
	t*=1000000.0/CLOCKS_PER_SEC;

	if(!init)init=t;

	return((s64)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000000LL)+tp.tv_usec);
#endif
#endif
}
