#macroed
function func1()
{
	print("""
	char *tk_teststr = "string1";
	""");
}
#endmacro

#define DO_FUNC1 #``func1()
DO_FUNC1

#define TK_CHKSANE_SIEVESZ	(1<<16)

static byte tk_chksane_sieve1_map[TK_CHKSANE_SIEVESZ];

int tk_chksane_sieve1a()
{
	s64 t0, t1, dt;
	int i, j, k, np;
	memset(tk_chksane_sieve1_map, 0, TK_CHKSANE_SIEVESZ);
	
	t0=TK_GetTimeUs();
	
	np=0;
	for(i=2; i<TK_CHKSANE_SIEVESZ; i++)
	{
		if(!tk_chksane_sieve1_map[i])
		{
			np++;
			for(k=i; k<TK_CHKSANE_SIEVESZ; k+=i)
				tk_chksane_sieve1_map[k]=1;
		}
	}

	t1=TK_GetTimeUs();
	dt=t1-t0;
	printf("Sieve 1A Time %dus, got %d\n", (int)dt, np);

	return(np);
}

int tk_chksane_sieve1v()
{
	s64 t0, t1, dt;
	__variant i, j, k, np;

	memset(tk_chksane_sieve1_map, 0, TK_CHKSANE_SIEVESZ);
	
	t0=TK_GetTimeUs();
	
	np=0;
	for(i=2; i<TK_CHKSANE_SIEVESZ; i++)
	{
		if(!tk_chksane_sieve1_map[i])
		{
			np++;
			for(k=i; k<TK_CHKSANE_SIEVESZ; k+=i)
				tk_chksane_sieve1_map[k]=1;
		}
	}

	t1=TK_GetTimeUs();
	dt=t1-t0;
	printf("Sieve 1V Time %dus, got %d\n", (int)dt, (int)np);

	return(np);
}

__class TkChkSane_Foo {
	__public int x, y;
};

__class TkChkSane_Bar __extends TkChkSane_Foo {
	__public int z;
	
	__public int sum()
	{
		return(x+y+z);
	}

	__public int sum2()
	{
		return(sum()*sum());
	}
};

struct TkChkSane_Foo_DlgTst1_s {
	int x;
};

struct TkChkSane_Foo_DlgTst2_s {
	int y;
	__delegate struct TkChkSane_Foo_DlgTst1_s *s1;
};

int tk_shell_chksane_var()
{
	__class TkChkSane_Foo fobj;
	__class TkChkSane_Bar bobj;
	struct TkChkSane_Foo_DlgTst1_s *ds1;
	struct TkChkSane_Foo_DlgTst2_s *ds2;

	__string str;
	__var v0, v1, v2;
	__var o0, o1, o2;
	int (*run1)(int x, int y);
	void *p1, *p2, *p3;
	int	i, j, k;
	long long li, lj, lk;

	tk_printf("CS-VA B0\n");
	
	str="Test";
	v0=1234;
	v1=5678;
	v2=v0+v1;
	j=(int)v2;
	if(j!=6912)
		__debugbreak();

	if(v2!=6912)
		__debugbreak();

	tk_printf("CS-VA B1\n");

	o0=__lvo_emptyobject();
	o0.x=v0;
	o0.y=v1;

//	v2=o0.x;

	v2=o0.x+o0.y;
	if(v2!=6912)
		__debugbreak();

//	j=(int)v2;
//	if(j!=6912)
//		__debugbreak();

	tk_printf("CS-VA B2\n");

	o1 = __var {x: 3, y: 4};

//	v2=o1.x;
//	__debugbreak();

//	v2=o1.x+o1.y;
//	__debugbreak();

	if((o1.x+o1.y)!=7)
		__debugbreak();

	o2 = __var [1, 2, 3, 4, 5];
	if((o2[2]+o2[3])!=7)
		__debugbreak();

	tk_printf("CS-VA B3\n");

//	fobj = __new __class TkChkSane_Foo();
	fobj = __new TkChkSane_Foo();

	fobj.x = 3;
	fobj.y = 4;
	if((fobj.x+fobj.y)!=7)
		__debugbreak();

	bobj = __new TkChkSane_Bar();
	bobj.x = 3;
	bobj.y = 4;
	bobj.z = 5;

	j=bobj.sum();
	if(j!=12)
		__debugbreak();

	k=bobj.sum2();
	if(k!=144)
		__debugbreak();

	tk_printf("CS-VA B4\n");

//	tk_chksane_sieve1a();
//	tk_chksane_sieve1v();

	p1 = __var(int x, int y):int { return(x+y); };
	p2 = __var(int x, int y):int { return(x+y+k); };

//	__debugbreak();

//	p3 = [=](int x, int y)->int { return(x+y); };
	p3 = [w=j*k](int x, int y)->int { return(x+y+k+w); };

	run1 = p1;
	j = run1(3, 4);
	if(j!=7)
		__debugbreak();

	run1 = p2;
	j = run1(3, 4);
	if(j!=151)
		__debugbreak();

//	__debugbreak();

	ds1=tk_malloc(sizeof(struct TkChkSane_Foo_DlgTst1_s));
	ds2=tk_malloc(sizeof(struct TkChkSane_Foo_DlgTst2_s));
	ds2->s1=ds1;
	ds1->x=3;
	ds2->y=4;
	j=ds2->x+ds2->y;
	if(j!=7)
		__debugbreak();


	li=TK_GetRandom();
	lj=TK_GetRandom();
	printf("RNG %X-%X %X-%X\n",
		(u32)(li>>32), (u32)(li>>0),
		(u32)(lj>>32), (u32)(lj>>0));

}

u64 TK_GetRandom();

int tk_shell_chksane_int128()
{
	__uint128 v0, v1, v2, v3;
	__int128 sv0, sv1, sv2, sv3;
	
	v0=0xDECAFB0F_DECAFB0F_DECAFB0F_DECAFB0FUI128;
	v1=0xBD95F61F_BD95F61F_BD95F61F_BD95F61EUI128;

	v2=v0;
	if(v2!=v0)
		__debugbreak();

	if(v1==v0)
		__debugbreak();

	if(v2>v0)
		__debugbreak();
	if(v2<v0)
		__debugbreak();

	if(v1>v0)
		__debugbreak();
	if(v1>=v0)
		__debugbreak();

	if(v0<v1)
		__debugbreak();
	if(v0<=v1)
		__debugbreak();


	v2=v0+v0;
	if(v2!=v1)
		__debugbreak();

	v2=v1-v0;

//	__debugbreak();

	if(v2!=v0)
		__debugbreak();
	
	v3=0xD95F61FB_D95F61FB_D95F61FB_D95F61E0UI128;
	v2=v0<<5;
	if(v2!=v3)
		__debugbreak();

	v3=0x006F657D_87EF657D_87EF657D_87EF657DUI128;
	v2=v0>>9;
	if(v2!=v3)
		__debugbreak();

	v2=-v0;
	v3=0x213504F0_213504F0_213504F0_213504F1UI128;
	if(v2!=v3)
		__debugbreak();

	sv0=0x00000000_00000000_00000000_DECAFB0FUI128;
	sv1=0x00000000_00000000_C1E4AC5F_2DE26AE1UI128;
	sv2=sv0*sv0;
	if(sv2!=sv1)
		__debugbreak();
	
//	__debugbreak();
}
