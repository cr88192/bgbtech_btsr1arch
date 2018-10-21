class tst_clz {

	public:

	int fib0(int x);

	int add();
	
	int t_v0;
	int t_v1;
};

int operator*(tst_clz a, tst_clz b)
{
	return(a.t_v0*b.t_v0+a.t_v1*b.t_v1);
}

int tst_clz::fib0(int x)
{
	if(x>2)
		return(fib(x-1)+fib(x-2));
	return(1);
}

int tst_clz::add()
{
	return(t_v0+t_v1);
}
