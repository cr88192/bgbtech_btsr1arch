template<class T> class TestVec
{
public:
	T x, y;
};

TestVec<float> tst_tv0;


template<class T> T tst_add(T x, T y)
	{ return(x+y); }

int tst_add2()
{
	tst_add<int>(3, 4);
}

class TestClass0 {
public:
	int x, y;
	int TestMethod();
};

int TestClass0::TestMethod()
{
	return(x+y);
}

TestClass0 operator+(TestClass0 a, TestClass0 b)
{
	TestClass0 c;
	
	c.x=a.x+b.x;
	c.y=a.y+b.y;
	return(c);
}
