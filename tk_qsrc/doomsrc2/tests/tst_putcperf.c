int main()
{
	signed char a, b, c;

	int n;
	int t0, t1, t2, dt, tt;
	int i, j, k;
	
	a=110;
	b=75;
	c=a+b;
	printf("%d\n", c);
	
	t0=TK_GetTimeMs();
	tt=0; n=0;
	while(1)
	{
		for(i=0; i<4096; i++)
		{
			tk_putc(' ');
			tk_putc('\r');
		}
		t1=TK_GetTimeMs();
		t2=t1-t0;
		if(t2<0)
		{
			t0=t1;
			continue;
		}
		
		t0=t1;
		tt+=t2;
		n+=8192;
		tk_printf("  %d %dms %d us/c\r", n, tt, (tt*1000)/n);
	}
}
