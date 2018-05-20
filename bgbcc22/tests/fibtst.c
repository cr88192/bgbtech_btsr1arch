//#ifndef __BSCC
#if 0

#include <stdio.h>
#include <time.h>

#endif

#ifndef CLOCKS_PER_SEC
// #define CLOCKS_PER_SEC	60
#define CLOCKS_PER_SEC	1000
#endif

void printf(char *str, ...);
int clock(void);
int puts(char *str);

#define	EXPR(a, b)	((a)*(a))+((b)*(b))

int fib(int x)
{
	if(x>2)return(fib(x-1)+fib(x-2));
	return(1);
}

#if 0
long long lfib(long long x)
{
	if(x>2)return(lfib(x-1)+lfib(x-2));
	return(1);
}

double dfib(double x)
{
	if(x>2)return(dfib(x-1)+dfib(x-2));
	return(1);
}

int ltst(int x)
{
	int i, j;

	j=0;
	for(i=0; i<x; i++)j++;

	return(j);
}

typedef struct foo_s {
int x, y;
}foo;

int stst(foo *st)
{
	return(st->x+st->y);
}

#endif

//int arrtst[]={1, 2, 3, 4};

int hash_str(char *s)
{
	int i;

	i=0;
	while(*s)i=i*251+(*s++);
	return(i&0xFFF);
}

#if 1

typedef struct test0_s test0_t;

struct test0_s {
int xi;
int yi;
// double xd;
// double yd;
};

test0_t test0_init={ 3, 4 };

int arrtst[16]={1,2,3,4,5};

#if 1
int arrtst2[16][16]={
{1,2,3,4,5},
{2,3,4,5,6},
{3,4,5,6,7},
{4,5,6,7,8}
};
#endif

void *ptrtst=hash_str;

int test_struct1(test0_t t2)
{
	test0_t t3;
	int z1, z2;
	
	t3=t2;

	z1=t2.xi+t2.yi;
	z2=t3.xi+t3.yi;
	printf("test_struct1: A %d %d\n", z1, z2);
}

int test_struct0()
{
	test0_t t0, t1;
	test0_t *pt0, *pt1;
	int z, z0;
	
	pt0=&t0;
	pt0->xi=3;
	pt0->yi=4;
	z=pt0->xi+pt0->yi;
	printf("test_struct0: A %d\n", z);

	z0=pt0->xi+pt0->yi;
	printf("test_struct0: B-0 %d\n", z0);

	t1=t0;

	z0=pt0->xi+pt0->yi;
	printf("test_struct0: B-1 %d\n", z0);

	pt1=&t1;
//	z=t1.xi+t1.yi;
	z=pt1->xi+pt1->yi;
	z0=pt0->xi+pt0->yi;
	
//	*(int *)-1=-1;
	printf("test_struct0: B-2 %d %d\n", z, z0);
	
	test_struct1(t1);

	return(0);
}

int test_ar2d_0()
{
	int i, j, k;
	
	for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{
			k=arrtst2[i][j];
			printf("%d ", k);
		}
		printf("\n");
	}
}

#endif

#if 1
int test_ll_0()
{
	long long li, lj, lk;
	unsigned char *cs;
	__int128 lxi, lxj, lxk;
	int p0, p1, p2, p3;
	int i, j, k;
	
//	li=0x12_3456_789A_BCDELL;
	li=0x123456789ABCDEFLL;
	lj=li+li;

//	*(int *)-1=-1;

//	i=0x314159;

	j=(int)(li>>32);
	i=(int)li;
	
	lk=li*128;

//	j=0x6969;
	
//	*(int *)-1=-1;

//	printf("LL0: %8X %8X\n", i, j);

//	printf("LL1: %8X %8X\n", (int)lj, (int)(lj>>32));

	printf("LL0: %8X_%8X\n", (int)(li>>32), (int)li);
	printf("LL1: %8X_%8X\n", (int)(lj>>32), (int)lj);
	printf("LL2: %8X_%8X\n", (int)(lk>>32), (int)lk);

	lk=li/10;
	printf("LL3: %8X_%8X\n", (int)(lk>>32), (int)lk);

	lk=li>>2;
	printf("LL4: %8X_%8X\n", (int)(lk>>32), (int)lk);
	
	lxi=0x01234567_89ABCDEF_FEDCBA98_76543210XL;
	
#if 0
	cs=&lxi;
	for(i=0; i<16; i++)
	{
		j=*cs;
		cs++;
		printf("%2X ", j);
	}
	printf("\n");
#endif

//	lxj=lxi+lxi;

//	*(int *)-1=-1;

	p0=(int)(lxi>>96);
	p1=(int)(lxi>>64);
	p2=(int)(lxi>>32);
	p3=(int)lxi;
//	p3=(int)(lxi>>0);

//	*(int *)-1=-1;

//	printf("LX0: %8X_%8X_", p0, p1);
//	printf("%8X_%8X\n", p2, p3);
	printf("LX0: %8X_%8X_%8X_%8X\n", p0, p1, p2, p3);

//	printf("LX0: %8X_%8X_%8X_%8X\n",
//		(int)(lxi>>96), (int)(lxi>>64),
//		(int)(lxi>>32), (int)lxi);
}
#endif

#if 1
test_old1(a, b)
int a, b;
{
	printf("Old Style %d %d\n", a, b);
}
#endif

#if 0
int bscc_main()
{
	long long li;
	int i, t, dt;

	printf("test\n");

	t=clock();
	i=fib(36);
	dt=clock()-t;

	printf("fibtst %d %d\n", i, dt);

	printf("hashtst %d\n", hash_str("foobarbaz"));

	li=0;
//	li=i;
	if(!li)printf("LI-Z\n");
//	t=i;
//	li=0;

	return(0);
}
#endif

void print_hex_n(unsigned int v, int n);
void print_decimal(int val);

void print_float(double val);
void print_float_ss(float val);

#if 1
int main()
{
	int i, j, k, t, dt;

	t=clock();

//	EXPR(3, 4);

	puts("main: A\n");

	print_hex_n(0x12345678, 8);
	puts("\n");

	k=0x12345678;
	for(i=0; i<10; i++)
	{
		j=k/10;
		print_hex_n(j, 8);
		puts("(");
		print_hex_n(k%10, 2);
		puts(") ");
		k=j;
	}
	puts("\n");

//	print_hex_n(0x12345678/10, 8);
//	puts("\n");

//	print_hex_n(0x12345678%10, 8);
//	puts("\n");

	print_decimal(0x12345678);
	puts("\n");


//	i=fib(36);
	i=fib(24);

//	i=ltst(100000000);

	puts("main: B\n");

	print_decimal(i);
	puts("\n");

	dt=clock()-t;
//	printf("%d %fms\n", i, (dt*1000.0)/CLOCKS_PER_SEC);
	printf("V=%d Dt=%dms\n", i, dt);
//	printf("V=%X Dt=%Xms\n", i, dt);

	puts("main: C\n");

	printf("hashtst %d\n", hash_str("foobarbaz"));
	puts("main: D\n");
	
	print_float(3.14159);
//	print_float_ss(3.14159f);
	puts("\n");

	puts("main: E\n");
	
	test_struct0();

	puts("main: F\n");
	
	test_ar2d_0();

	puts("main: G\n");
	
	test_ll_0();
	
	test_old1(2, 3);

	printf("pp %f\n", 3.14159);
//	*(int *)-1=-1;

	puts("main: H\n");

}
#endif
