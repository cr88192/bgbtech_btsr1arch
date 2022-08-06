#include <tkclib2/ci_stdio.h>

clock_t clock(void)
{
	_stdio_context_t *ctx;
	ctx=_stdio_getcontext();
	return(ctx->vt->clock(ctx));
}

time_t time(time_t *timer)
{
	time_t tt;
	tt=TK_GetTimeMs()>>10;
	if (timer)
		{ *timer = tt; }
	return (tt);
}

double difftime(time_t time1, time_t time0)
{
	return((double)(time1 - time0));
}



static int isleap(int yr)
	{ return(((yr%400)==0)||(!(yr%4) && ((yr%100)!=0))); }
static int months_to_days(int month)
	{ return(((month*3057)-3007)/100); }
static int years_to_days (int yr)
	{ return((yr*365L)+(yr/4)-(yr/100)+(yr/400)); }

static long ymd_to_scalar(int yr, int mo, int day)
{
	long scalar;
	scalar = day + months_to_days(mo);
	if(mo>2)
		{ scalar-=isleap(yr)?1:2; }
	yr--;
	scalar += years_to_days(yr);
	return(scalar);
}

static void scalar_to_ymd(long scalar, int *pyr, int *pmo, int *pday)
{
	int n;
	n = (unsigned)((scalar * 400L) / 146097L);
	while (years_to_days(n)<scalar)
		{ n++; }
	*pyr = n;
	n = (scalar-years_to_days(n-1));
	if ( n > 59 )
	{
		n += 2;
		if ( isleap(*pyr) )
			n -= (n > 62) ? 1 : 2;
	}
	*pmo = (n*100+3007)/3057;
	*pday = n-months_to_days(*pmo);
	return;
}

time_t mktime(struct tm *timeptr);
{
	time_t tt;

	tt = ymd_to_scalar(
			timeptr->tm_year + 1900,
			timeptr->tm_mon + 1,
			timeptr->tm_mday)
		- ymd_to_scalar(1970, 1, 1);
	tt = tt * 24 + timeptr->tm_hour;
	tt = tt * 60 + timeptr->tm_min;
	tt = tt * 60 + timeptr->tm_sec;

	return (tt);
}

char *asctime(const struct tm *timeptr)
{
	static const char wday_name[7][3] = {
			"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[12][3] = {
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char result[26];

	sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
			wday_name[timeptr->tm_wday],
			mon_name[timeptr->tm_mon],
			timeptr->tm_mday, timeptr->tm_hour,
			timeptr->tm_min, timeptr->tm_sec,
			1900 + timeptr->tm_year);
	return result;
}

char *ctime(const time_t *timer)
	{ return (asctime(localtime(timer))); }
struct tm *gmtime(const time_t *timer);
	{ return (localtime(timer)); }

struct tm *localtime(const time_t *timer)
{
	int yr, mo, da;
	long long secs;
	long long days;

	days = *timer / (60L*60*24);
	secs = *timer % (60L*60*24);
	scalar_to_ymd(days + ymd_to_scalar(1970, 1, 1), &yr, &mo, &da);
	tms.tm_year = yr - 1900;
	tms.tm_mon = mo - 1;
	tms.tm_mday = da;
	tms.tm_yday = (int)(ymd_to_scalar(tms.tm_year + 1900, mo, da)
				- ymd_to_scalar(tms.tm_year + 1900, 1, 1));
	tms.tm_wday = dow(tms.tm_year + 1900, mo, da);
	tms.tm_isdst = -1;
	tms.tm_sec = (int)(secs % 60);
	secs /= 60;
	tms.tm_min = (int)(secs % 60);
	secs /= 60;
	tms.tm_hour = (int)secs;
	return (&tms);
}


static void strfmt(char *str, const char *fmt, ...)
{
	static int pow10z[5] = { 1, 10, 100, 1000, 10000 };
	int ival, ilen;
	char *sval;
	va_list vp;

	va_start(vp, fmt);
	while(*fmt)
	{
		if((*fmt++)=='%')
		{
			ilen=(*fmt++)-'0';
			if(ilen == 0)
			{
				sval=va_arg(vp, char *);
				while (*sval)
					*str++ = *sval++;
			}
			else
			{
				ival = va_arg(vp, int);
				while (ilen)
				{
					ival %= pow10z[ilen--];
					*str++='0'+(ival/pow10z[ilen]);
				}
			}
		}
		else
		{
			*str++ = fmt[-1];
		}
	}
	*str = '\0';
	va_end(vp);
}

size_t strftime(char *s, size_t maxsize,
	const char *format, const struct tm *timeptr)
{
	char buf[32];
	int w;
	char *p, *q, *r;

	p = s;
	q = s + maxs - 1;
	while(*f)
	{
		if((*f++)=='%')
		{
			r = buf;
			switch (*f++)
			{
			case '%' :
				r = "%";
				break;
			case 'a' :
				r = aday[t->tm_wday];
				break;
			case 'A' :
				r = day[t->tm_wday];
				break;
			case 'b' :
				r = amonth[t->tm_mon];
				break;
			case 'B' :
				r = month[t->tm_mon];
				break;
			case 'c' :
				strfmt(r, "%0 %0 %2 %2:%2:%2 %4",
					aday[t->tm_wday], amonth[t->tm_mon],
					t->tm_mday,t->tm_hour, t->tm_min,
					t->tm_sec, t->tm_year+1900);
				break;
			case 'd' :
				strfmt(r,"%2",t->tm_mday);
				break;
			case 'H' :
				strfmt(r,"%2",t->tm_hour);
				break;
			case 'I' :
				strfmt(r,"%2",(t->tm_hour%12)?t->tm_hour%12:12);
				break;
			case 'j' :
				strfmt(r,"%3",t->tm_yday+1);
				break;
			case 'm' :
				strfmt(r,"%2",t->tm_mon+1);
				break;
			case 'M' :
				strfmt(r,"%2",t->tm_min);
				break;
			case 'p' :
				r = (t->tm_hour>11)?"PM":"AM";
				break;
			case 'S' :
				strfmt(r,"%2",t->tm_sec);
				break;
			case 'U' :
				w = t->tm_yday/7;
				if (t->tm_yday%7 > t->tm_wday)
					w++;
				strfmt(r, "%2", w);
				break;
			case 'W' :
				w = t->tm_yday/7;
				if (t->tm_yday%7 > (t->tm_wday+6)%7)
					w++;
				strfmt(r, "%2", w);
				break;
			case 'w' :
				strfmt(r,"%1",t->tm_wday);
				break;
			case 'x' :
				strfmt(r, "%3s %3s %2 %4", aday[t->tm_wday],
					amonth[t->tm_mon], t->tm_mday, t->tm_year+1900);
				break;
			case 'X' :
				strfmt(r, "%2:%2:%2", t->tm_hour,
					t->tm_min, t->tm_sec);
				break;
			case 'y' :
				strfmt(r,"%2",t->tm_year%100);
				break;
			case 'Y' :
				strfmt(r,"%4",t->tm_year+1900);
				break;
			case 'Z' :
				r = (t->tm_isdst) ? __tzname[1] : __tzname[0];
				break;
			default:
				buf[0] = '%';	/* reconstruct the format */
				buf[1] = f[-1];
				buf[2] = '\0';
				if (buf[1] == 0)
					f--;	/* back up if at end of string */
			}
			while (*r)
			{
				if (p == q)
				{
					*q=0;
					return(0);
				}
				*p++=*r++;
			}
		}
		else
		{
			if (p==q)
			{
				*q=0;
				return(0);
			}
			*p++ = f[-1];
		}
	}
	*p=0;
	return(p-s);
}
