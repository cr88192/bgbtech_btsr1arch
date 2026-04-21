#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int tst_i128()
{
	__int128 xi, xj, xk, xk1, xkl, xkh;
	__int64 li, lj, lk, lhi, lhj, lhk, lk0;
	int i;
	
	printf("tst_i128:\n");

	li=0x12345678;
	lj=0x23456789;
	lk0=li+lj;
	
	xi=0x12345678;
	xj=0x23456789;
	xk=xi+xj;
	
//	__debugbreak();
	
	printf("%016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj,
		(long long)(xk>>64), (long long)xk);

	li=xi;	lhi=xi>>64;
	lj=xj;	lhj=xj>>64;
	lk=xk;	lhk=xk>>64;
	printf("%016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		lhi, li, lhj, lj, lhk, lk);

	printf("%032LX %032LX %032LX\n", xi, xj, xk);

	if(xi>xj)
		{ __debugbreak(); }
	if(xj>xk)
		{ __debugbreak(); }

	if(xi>xi)
		{ __debugbreak(); }
	if(xi<xi)
		{ __debugbreak(); }
	if(xi!=xi)
		{ __debugbreak(); }

	if(xi>=xj)
		{ __debugbreak(); }
	if(xj>=xk)
		{ __debugbreak(); }

	if(!(xi>=xi))
		{ __debugbreak(); }
	if(!(xi<=xi))
		{ __debugbreak(); }

	printf("\n");

	xk=xi<<48;
//	__debugbreak();
	xk=xk>>8;

	printf("%016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj,
		(long long)(xk>>64), (long long)xk);

	printf("\n");

	for(i=0; i<8; i++)
	{
		xi=xi<<8;
		xj=xj<<8;
		xk=xi+xj;

		xk1=xk>>((i+1)*8);
		if(xk1!=lk0)
		{
			__debugbreak();
		}
		
//		printf("%016llX:%016llX %016llX:%016llX %016llX:%016llX\n",
//			(long long)(xi>>64), (long long)xi,
//			(long long)(xj>>64), (long long)xj,
//			(long long)(xk>>64), (long long)xk);
	}


	xi=0x12345678;
	xj=0x23456789;
	xk=xi-xj;
	
//	__debugbreak();
	
	printf("- %016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj,
		(long long)(xk>>64), (long long)xk);

	xk=xi*xj;
	
//	__debugbreak();
	
	printf("* %016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj,
		(long long)(xk>>64), (long long)xk);

	if(xi>>48)
		__debugbreak();
	if(!(xi>>16))
		__debugbreak();


	xi=0x23456789;
	xj=0x12345678;
	xk=xi-xj;
	
//	__debugbreak();
	
	printf("- %016llX:%016llX %016llX:%016llX\n\t%016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj,
		(long long)(xk>>64), (long long)xk);

	xi=0x12345678_9ABCDEF0_12345678_9ABCDEF0UI128;
	xj=0x9ABCDEF0_12345678_9ABCDEF0_12345678UI128;

	xk=xi*xj;
	__xli_dmul128u(xi, xj, &xkl, &xkh);
	
//	__debugbreak();
	
	printf("W* %016llX:%016llX %016llX:%016llX\n",
		(long long)(xi>>64), (long long)xi,
		(long long)(xj>>64), (long long)xj);

	printf("-> %016llX:%016llX:%016llX:%016llX\n",
		(long long)(xkh>>64), (long long)xkh,
		(long long)(xkl>>64), (long long)xkl);


	printf("tst_i128: OK\n");
}

int tst_f128()
{
	__float128 lfx, lfy, lfz;
	double dx, dy, dz;
	int i, j, k;
	
	printf("tst_f128:\n");

	dy=6.283185307179586476925286766559;
	lfy=dy;
	dz=lfy;
	printf("= %f %f\n", dy, dz);

	lfx=6.283185307179586476925286766559;
	dz=lfx;
	printf("B= %f\n", dz);
	
	k=6969;
	lfx=k;
	dz=lfx;
	printf("I= %f\n", dz);
	
	j=lfx;
	printf("I2= %d\n", j);
	
	dx=2;
	dy=3;
	lfx=dx;
	dz=lfx;

//	__debugbreak();

	lfy=dy;

	printf("= %f %f\n", dx, dz);

	lfz=lfx+lfy;
	dz=lfz;
	
	printf("+ %f %f %f\n", dx, dy, dz);

	lfz=lfx-lfy;
	dz=lfz;

	printf("- %f %f %f\n", dx, dy, dz);

	lfz=lfx-lfx;
	dz=lfz;
	printf("- %f %f %f\n", dx, dx, dz);

	lfz=lfy-lfx;
	dz=lfz;
	printf("- %f %f %f\n", dy, dx, dz);


	lfz=lfx*lfy;
	dz=lfz;

	printf("*1 %f %f %f\n", dx, dy, dz);

	lfz=lfx/lfy;
	dz=lfz;

	printf("/ %f %f %f\n", dx, dy, dz);

	dy=0.333333333;
	lfy=dy;
	lfz=lfx*lfy;
	dz=lfz;
	printf("* %f %f %f\n", dx, dy, dz);

	lfz=2.0;
	lfy=1.999;
	lfz=lfx-lfy;
	dx=lfx; dy=lfy; dz=lfz;
	printf("- %f %f %f\n", dx, dy, dz);

}

int main()
{
	tst_i128();
	tst_f128();
}

