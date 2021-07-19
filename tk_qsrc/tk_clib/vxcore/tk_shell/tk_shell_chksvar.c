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

int tk_shell_chksane_var()
{
	__class TkChkSane_Foo fobj;
	__class TkChkSane_Bar bobj;

	__string str;
	__var v0, v1, v2;
	__var o0, o1, o2;
	int	i, j, k;
	
	str="Test";
	v0=1234;
	v1=5678;
	v2=v0+v1;
	j=(int)v2;
	if(j!=6912)
		__debugbreak();

	if(v2!=6912)
		__debugbreak();

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

//	tk_chksane_sieve1a();
//	tk_chksane_sieve1v();

//	__debugbreak();
}
