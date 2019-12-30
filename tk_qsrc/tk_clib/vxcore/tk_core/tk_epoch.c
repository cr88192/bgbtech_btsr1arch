static const int tk_mdtab_cy[12]={
	  0,  31,  59,  90,
	120, 151, 181, 212,
	243, 273, 304, 331 };
static const int tk_mdtab_ly[12]={
	  0,  31,  60,  91,
	121, 152, 182, 213,
	244, 274, 305, 332 };

s64 TK_DateToEpochUsec(TK_DATETIME *date)
{
	/*
	Seconds per day: 86400
	Days in common year: 365, 31536000 seconds
	Days in leap year: 366, 31622400‬ seconds
	Seconds in 4 year period: 126230400
	
	Length of Months (Seconds/Days):
		Jan: 2678400(31)
		Feb: 2419200(28) / 2505600(29)
		Mar: 2678400(31)
		Apr: 2592000(30)
		May: 2678400(31)
		Jun: 2592000(30)
		Jul: 2678400(31)
		Aug: 2678400(31)
		Sep: 2592000(30)
		Oct: 2678400(31)
		Nov: 2592000(30)
		Dec: 2678400(31)
	*/
	
//	static const u32 mstab_cy[12] = {
//		       0,	 2678400,	 5097600‬,	 7776000‬,‭  /* Jan/Feb/Mar/Apr */
//		10368000‬,	13046400‬,	15638400‬,	18316800‬,  /* May/Jun/Jul/Aug */
//		20995200‬,	23587200‬,	26265600‬,	28857600‬}; /* Sep/Oct/Nov/Dec */
//	static const u32 mstab_ly[12] = {
//		       0,	 2678400,	 5184000,	 7862400‬,‭  /* Jan/Feb/Mar/Apr */
//		10454400‬,	13132800‬,	15724800‬,	18403200‬,  /* May/Jun/Jul/Aug */
//		21081600‬,	23673600‬,	26352000‬,	28944000‬}; /* Sep/Oct/Nov/Dec */

	u16 yr, y4, y1;
	byte mo, dy, hr, mm, sc;
	int us;
	s64 tt, ys, ms;

//	yr=((cdate>>9)&127)+1980;
//	mo=((cdate>>5)&15)-1;
//	dy=(cdate&31)-1;
//	hr=(ctime>>11)&31;
//	mm=(ctime>>5)&63;
//	sc=(ctime<<0)&63;

	yr=date->year;
	mo=date->mon;
	dy=date->day;
	hr=date->hour;
	mm=date->min;
	sc=date->sec;
	us=((date->msc4)<<12) | date->usc4;
	
	y4=(yr-1968)>>2;					//leap year was 1968
	y1=yr-((y4<<2)+1968);				//years following leap year
	ys=(y4*126230400U)+(y1*31536000U);
	ys-=(2*31536000U);					//sub 2 years for 1970 epoch
	
	if(!y1)
//		{ ms=mstab_cy[mo]; }
		{ ms=tk_mdtab_cy[mo]*86400; }
	else
//		{ ms=mstab_ly[mo]; }
		{ ms=tk_mdtab_ly[mo]*86400; }
	
//	tt=((yr-1970)*31558150U)+(mo*2628000U)+(d*86400U)+(h*3600U)+(m*60)+s;
//	tt=ys+(mo*2628000U)+(dy*86400U)+(hr*3600U)+(mm*60)+sc;
	tt=ys+ms+(dy*86400U)+(hr*3600U)+(mm*60)+sc;
	tt=(tt<<20) | us;
	return(tt);
}

void TK_DateFromEpochUsec(TK_DATETIME *date, s64 epoch)
{	
	s64 epsec;
	int aday, asec, usec;
	int yr4, yr1, yday4, yday;
	int mo, day;
	int hr, min, sec, amin;
	
	usec=epoch&1048575;
	epsec=epoch>>20;
	epsec+=(2*31536000U);	//add 2 years (epoch 1970->1968)
	
	aday=epsec/86400;
	asec=epsec-(aday*86400);
	yr4=aday/1461;
	yday4=aday-(yr4*1461);

	if(yday4>=366)
	{
		yr1=1+((yday4-366)/365);
		yday=yday4-((yr1*365)+1);
		mo=yday/31;
		if(tk_mdtab_cy[mo]>yday)mo--;
		day=yday-tk_mdtab_cy[mo];
	}else
	{
		yr1=0;
		yday=yday4;
		mo=yday/31;
		if(tk_mdtab_ly[mo]>yday)mo--;
		day=yday-tk_mdtab_ly[mo];
	}
	
	amin=asec/60;
	sec=asec-(amin*60);
	hr=amin/60;
	min=amin-(hr*60);

	date->year=1968+(yr4*4)+yr1;
	date->mon=mo;
	date->day=day;
	date->hour=hr;
	date->min=min;
	date->sec=sec;
	date->msc4=usec>>12;
	date->usc4=usec&4095;
}
