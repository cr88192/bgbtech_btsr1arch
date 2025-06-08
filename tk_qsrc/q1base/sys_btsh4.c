/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// sys_null.h -- null system driver to aid porting efforts

#include "quakedef.h"
#include "errno.h"

qboolean isDedicated=0;

int tk_printf(char *str, ...);	//BGB

/*
===============================================================================

FILE IO

===============================================================================
*/

#define MAX_HANDLES             256
FILE    *sys_handles[MAX_HANDLES];

int             findhandle (void)
{
	int             i;
	
	for (i=1 ; i<MAX_HANDLES ; i++)
		if (!sys_handles[i])
			return i;
	Sys_Error ("out of handles");
	return -1;
}

/*
================
filelength
================
*/
int filelength (FILE *f)
{
	int             pos;
	int             end;

	printf("filelength: A\n");
	pos = ftell (f);
	printf("filelength: B\n");
	fseek (f, 0, SEEK_END);
	printf("filelength: C\n");
	end = ftell (f);
	printf("filelength: D\n");
//	fseek (f, pos, SEEK_SET);
	fseek (f, 0, SEEK_SET);
	printf("filelength: E\n");

	return end;
}

int Sys_FileOpenRead (char *path, int *hndl)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1;
	}
	sys_handles[i] = f;
	*hndl = i;
	
	return filelength(f);
}

int Sys_FileOpenWrite (char *path)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "wb");
	if (!f)
		Sys_Error ("Error opening %s: %s", path,strerror(errno));
	sys_handles[i] = f;
	
	return i;
}

void Sys_FileClose (int handle)
{
	fclose (sys_handles[handle]);
	sys_handles[handle] = NULL;
}

void Sys_FileSeek (int handle, int position)
{
	fseek (sys_handles[handle], position, SEEK_SET);
}

int Sys_FileRead (int handle, void *dest, int count)
{
	int rlen;
//	tk_printf("Sys_FileRead hdl=%d dst=%p sz=%d fd=%p\n",
//		handle, dest, count, sys_handles[handle]);
	rlen = fread (dest, 1, count, sys_handles[handle]);
//	tk_printf("Sys_FileRead rlen=%d\n", rlen);
	return(rlen);
}

int Sys_FileWrite (int handle, void *data, int count)
{
	return fwrite (data, 1, count, sys_handles[handle]);
}

int     Sys_FileTime (char *path)
{
	FILE    *f;
	
	f = fopen(path, "rb");
	if (f)
	{
		fclose(f);
		return 1;
	}
	
	return -1;
}

void Sys_mkdir (char *path)
{
}


/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable (nlint startaddr, nlint length)
{
}


void Sys_Error (char *error, ...)
{
//	char tb[1024];
	char tb[256];
	va_list         argptr;

	{ DBGBREAK }

	tk_printf("Sys_Error: ");   
	va_start(argptr, error);
//	vfprintf(stdout, error, argptr);
	vsprintf(tb, error, argptr);
//	tk_vprintf(error, argptr);
	va_end(argptr);
	printf("%s\n", tb);

	{ DBGBREAK }

//	exit(1);
//	*(int *)-1=-1;
}

void Sys_Printf (char *fmt, ...)
{
	char tb[1024];
	va_list         argptr;
	
	va_start(argptr, fmt);
//	vfprintf(stdout, fmt, argptr);
//	vsprintf(tb, fmt, argptr);
	tk_vprintf(fmt, argptr);
	va_end(argptr);
//	tk_puts(tb);
}

void Sys_Quit (void)
{
	exit (0);
}

u32 TK_GetTimeMs(void);

double Sys_FloatTime (void)
{
	static double t, t0;
//	static float t, t0;
	
//	t += 0.1;

	t0 = (int)TK_GetTimeMs();
	t = t0*0.001;
	
//	printf("\t\t\t\tT=%f\r", t);
	
//	*(int *)-1=-1;
	return t;
}

int I_GetTimeMs (void)
{
	int t;
	t = TK_GetTimeMs();
	return(t);
}

char *Sys_ConsoleInput (void)
{
	return NULL;
}

void Sys_Sleep (void)
{
}

void Sys_SendKeyEvents (void)
{
}

void Sys_HighFPPrecision (void)
{
}

void Sys_LowFPPrecision (void)
{
}

void sleep_0();

#ifdef __arm__
int tk_puts(char *str)
{
}

int tk_printf(char *str, ...)
{
}

int tk_vprintf(char *str, va_list lst)
{
}

// void __debugbreak()
//{
//}

int *__get_errno()
{
	static int i;
	i=0;
	return(&i);
}

FILE **__get_stderr()
{
	static int i;
	i=0;
	return(&i);
}

int __setj(jmp_buf env)
{
}

u32 TK_GetTimeMs(void)
{
}

int tk_print_float(double v)
{
}

int tk_kbhit()
{
}

int tk_getch()
{
}

int tk_putc(int v)
{
}

void sleep_0()
{
}

#endif

#ifndef _BGBCC
// void __debugbreak()
// {
//	*(int *)-1=-1;
// }
#endif

void Sys_CheckSanity(void)
{
	static byte pat_tst0[16]={
		0x78, 0x56, 0x34, 0x12,
		0xEF, 0xCD, 0xAB, 0x89,
		};
	static int rec=0;
	char tb[256];
	int *pi, *pj, *pk;
	long long li, lj;
	double		time, oldtime, newtime;
	double f, g, h;
	float ff, gf, hf;
	int i, j, k, l;

	if(!rec)
	{
#if 0
		tk_puts("Q Flt 0: ");
		tk_print_float(3.14159);
		tk_puts("\n");
		time=3.14159*2;
		tk_puts("Q Flt 1: ");
		tk_print_float(time);
		tk_puts("\n");
#endif

		tk_printf("Q Flt 0: %f\n", 3.14159);

		tk_printf("Q Flt 2: %f\n", time);
		printf("Q Flt 3: %f\n", time);
		
		*(double *)(&oldtime)=time;
		newtime=*(double *)(&time);
		tk_printf("Q Flt 4: %f\n", oldtime);
		tk_printf("Q Flt 5: %f\n", newtime);
		
		oldtime=time*10.0;
		newtime=time*0.1;
		tk_printf("Q Flt 4-1: %f\n", oldtime);
		tk_printf("Q Flt 5-1: %f\n", newtime);

		oldtime=time;
		newtime=time+oldtime;
		tk_printf("Q Flt 5-2: %f\n", newtime);
		newtime=time-oldtime;
		tk_printf("Q Flt 5-3: %f\n", newtime);

		newtime=time*oldtime;
		tk_printf("Q Flt 5-4: %f\n", newtime);
		newtime=newtime/oldtime;
		tk_printf("Q Flt 5-5: %f\n", newtime);
		
		time=Q_atof("3.14159");
	//	*(int *)-1=-1;
		tk_printf("Q Flt 6: %f\n", time);
		
//		if(time!=3.14159)
//			{ DBGBREAK }

		for(i=-1; i<=1; i++)
			for(j=-1; j<=1; j++)
		{
			f=i; g=j;

			tk_printf("%d==%d -> %d\n", i, j, i==j);
			tk_printf("%d!=%d -> %d\n", i, j, i!=j);
			tk_printf("%d< %d -> %d\n", i, j, i< j);
			tk_printf("%d> %d -> %d\n", i, j, i> j);
			tk_printf("%d<=%d -> %d\n", i, j, i<=j);
			tk_printf("%d>=%d -> %d\n", i, j, i>=j);

			if(i==j)	tk_printf("%d==%d -> T\n", i, j);
			else		tk_printf("%d==%d -> F\n", i, j);
			if(i!=j)	tk_printf("%d!=%d -> T\n", i, j);
			else		tk_printf("%d!=%d -> F\n", i, j);
			if(i<j)		tk_printf("%d< %d -> T\n", i, j);
			else		tk_printf("%d< %d -> F\n", i, j);
			if(i>j)		tk_printf("%d> %d -> T\n", i, j);
			else		tk_printf("%d> %d -> F\n", i, j);
			if(i<=j)	tk_printf("%d<=%d -> T\n", i, j);
			else		tk_printf("%d<=%d -> F\n", i, j);
			if(i>=j)	tk_printf("%d>=%d -> T\n", i, j);
			else		tk_printf("%d>=%d -> F\n", i, j);

			printf("%f==%f -> %d\n", f, g, f==g);
			printf("%f!=%f -> %d\n", f, g, f!=g);
			printf("%f< %f -> %d\n", f, g, f< g);
			printf("%f> %f -> %d\n", f, g, f> g);
			printf("%f<=%f -> %d\n", f, g, f<=g);
			printf("%f>=%f -> %d\n", f, g, f>=g);
			
			if(f==g)	printf("%f==%f -> T\n", f, g);
			else		printf("%f==%f -> F\n", f, g);
			if(f!=g)	printf("%f!=%f -> T\n", f, g);
			else		printf("%f!=%f -> F\n", f, g);
			if(f<g)		printf("%f<%f -> T\n", f, g);
			else		printf("%f<%f -> F\n", f, g);
			if(f>g)		printf("%f>%f -> T\n", f, g);
			else		printf("%f>%f -> F\n", f, g);
			if(f<=g)	printf("%f<=%f -> T\n", f, g);
			else		printf("%f<=%f -> F\n", f, g);
			if(f>=g)	printf("%f>=%f -> T\n", f, g);
			else		printf("%f>=%f -> F\n", f, g);
		}

		pi=&i;
		pj=&j;

//		*pj=1;

//		j=i-j;
		j=!tk_iskernel();
		hf=0;

		for(i=0; i<32; i++)
		{
			f=((M_PI*2.0)/16.0)*((double)i);
			g=sqrt(i);
			h=g*g;
#if 1
			printf("i=%d: ", i);
	//		tk_printf("a=%f sin=%f cos=%f sqrt(i)=%f(^2=%f)\n",
	//			f, sin(f), cos(f), g, h);
			printf("a=%f sin=%f cos=%f ", f, sin(f), cos(f));
			printf("sqrt(i)=%f(^2=%f)\n", g, h);
#endif

			hf+=_sin_fast(f);
		}

//		printf("%f\n", h);

		if(fabs(hf)>0.01)
			{ __debugbreak(); }

		printf("%f\n", hf);

#if 0
		f=0;
		g=_sin_fast(f);
		if(fabs(g-0.0)>0.0001)
			{ DBGBREAK }
		g=_cos_fast(f);
		if(fabs(g-1.0)>0.0001)
			{ DBGBREAK }
#endif
	}

#if 1
	if(!rec)	tk_puts("Sanity A\n");

	if(LittleShort(0x1234)!=0x1234)
		{ DBGBREAK }
	if(LittleLong(0x12345678)!=0x12345678)
		{ DBGBREAK }
	if(LittleShort(0xABCD)!=(short)0xABCD)
		{ DBGBREAK }
	if(LittleLong(0x89ABCDEF)!=0x89ABCDEF)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity B\n");

	if(LittleShort(0x12345678)!=0x5678)
		{ DBGBREAK }
	if(LittleShort(0x89ABCDEF)!=(short)0xCDEF)
		{ DBGBREAK }

	if(LittleFloat(M_PI)!=((float)M_PI))
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity C\n");

	if(LittleShort(*(short *)(pat_tst0+2))!=0x1234)
		{ DBGBREAK }
	if(LittleLong(*(int *)(pat_tst0+0))!=0x12345678)
		{ DBGBREAK }
	if(LittleShort(*(short *)(pat_tst0+5))!=(short)0xABCD)
		{ DBGBREAK }
	if(LittleLong(*(int *)(pat_tst0+4))!=0x89ABCDEF)
		{ DBGBREAK }

	if(LittleShort(*(int *)(pat_tst0+0))!=0x5678)
		{ DBGBREAK }
	if(LittleShort(*(int *)(pat_tst0+4))!=(short)0xCDEF)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity C1\n");

	i=*(int *)(pat_tst0+4);
	if((i<<16)!=0xCDEF0000)
		{ DBGBREAK }
	if((i>>16)!=0xFFFF89AB)
		{ DBGBREAK }

	if((((u32)i)<<16)!=0xCDEF0000)
		{ DBGBREAK }
	if((((u32)i)>>16)!=0x000089AB)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity D\n");


	i=4095;
	if(&pr_functions[i] != pr_functions+i)
		{ DBGBREAK }
	if((byte *)(&pr_functions[i]) !=
		(byte *)pr_functions+i*sizeof(*pr_functions))
			{ DBGBREAK }
	if(sizeof(*pr_functions)!=sizeof(dfunction_t))
		{ DBGBREAK }
	if(sizeof(dfunction_t)!=36)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity E\n");

	pj=(int *)pat_tst0+3;
	pk=(int *)pat_tst0;
	if((pj-pk)!=3)
		{ DBGBREAK }
	if(((byte *)pj-(byte *)pk)!=12)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity F\n");
#endif

	i=3; j=4; k=5;
	if((i+j)!=7)
		{ DBGBREAK }
	if((i-k)!=-2)
		{ DBGBREAK }
	if((i*j)!=12)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity G\n");

	if((i&j)!=0)
		{ DBGBREAK }
	if((i|k)!=7)
		{ DBGBREAK }
	if((i^k)!=6)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity H\n");

	i=-6972; j=1;

	if((i>>1)!=(-3486))
		{ DBGBREAK }
	if((((u32)i)>>1)!=(0x7FFFF262))
		{ DBGBREAK }

	if((i>>j)!=(-3486))
		{ DBGBREAK }
	if((((u32)i)>>j)!=(0x7FFFF262))
		{ DBGBREAK }

	k=0;
	if((i<<k)!=i)
		{ DBGBREAK }
	if((i>>k)!=i)
		{ DBGBREAK }

	j=1; k=9;
	if((j<<k)!=512)
		{ DBGBREAK }
	if((j<<8)!=256)
		{ DBGBREAK }
	if((2<<k)!=1024)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity I\n");

	i>>=3;
	if(i!=(-872))
		{ DBGBREAK }
	i<<=19;
	if(i!=(-457179136))
		{ DBGBREAK }
	i=-i;
	if(i!=(457179136))
		{ DBGBREAK }

	i=-6972; j=1;

	if(!rec)	tk_puts("Sanity I1\n");

	g=-6972.0;
	gf=-6972.0;

	f=i; ff=i;
//	if(f!=(-6972.0))
	if(f!=g)
	{
//		g=-6972.0;
		li=*(long long *)(&f);
		lj=*(long long *)(&g);
		printf("I1-0: i=%d f=%08X-%08X g=%08X-%08X\n", i,
			(int)(li>>32), (int)li,
			(int)(lj>>32), (int)lj);
		{ DBGBREAK }
	}
//	if(ff!=(-6972.0))
	if(ff!=gf)
	{
//		gf=-6972.0;
		printf("I1-1: i=%d ff=%08X exp=%08X\n", i,
			*(int *)(&ff), *(int *)(&gf));
		{ DBGBREAK }
	}

	if(!rec)	tk_puts("Sanity J\n");

	f=3.14159; ff=f;
	i=f; j=ff;
	if(i!=3)
		{ DBGBREAK }
	if(j!=3)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity J1\n");

	i=0; k=-1;

	f=7.0; g=8.0; h=9.0;
	if((f+g)!=15.0)
		{ DBGBREAK }
	if((f-h)!=-2.0)
		{ DBGBREAK }
	if((f*h)!=63.0)
		{ DBGBREAK }
//	if((h/g)!=1.125)
	if(fabs((h/g)-1.125)>0.000001)
		{ DBGBREAK }

	if(ceil(3.14)!=4.0)
		{ DBGBREAK }
	if(floor(3.14)!=3.0)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity K\n");

	if(i)
		{ DBGBREAK }
	if(!j)
		{ DBGBREAK }
	if(j<=0)
		{ DBGBREAK }
	if(k>=0)
		{ DBGBREAK }
	if(i<0)
		{ DBGBREAK }
	if(i>0)
		{ DBGBREAK }

	i=0; j=1; k=!j; l=!i;
	if(i && j)
		{ DBGBREAK }
	if(i || k)
		{ DBGBREAK }
	if(j && l)	{}
	else	{ DBGBREAK }
	if(j || k) {}
	else	{ DBGBREAK }
	if(k || l) {}
	else	{ DBGBREAK }

	if(!rec)	tk_puts("Sanity L\n");

	if((i && j) || !(j && l))
		{ DBGBREAK }
	if((i || j) && (j && l)) {}
	else	{ DBGBREAK }

	if((i || j) && (i || k))
		{ DBGBREAK }
	if((i || k) && (i || j))
		{ DBGBREAK }

	if((i && j) || (j && l)) {}
	else	{ DBGBREAK }
	if((j && l) || (i && j)) {}
	else	{ DBGBREAK }

	if(!rec)	tk_puts("Sanity M\n");

	i='5';
	if (i >= '0' && i <= '9') {}
	else	{ DBGBREAK }
	if (i >= 'a' && i <= 'z')
		{ { DBGBREAK } }
	else	{ }

	i='H';
	if (i >= 'A' && i <= 'Z') {}
	else	{ DBGBREAK }

	i=999; j=10;
	if((i%j)!=9)
		{ DBGBREAK }
	i=486; j=7;
	if((i%j)!=3)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity N\n");

	if(!rec)
	{
		i=3; j=4; k=5;
		f=7.0; g=8.0; h=9.0;
		ff=1.1f;	gf=2.2f;	hf=3.3f;
	}else
	{
		i=3; j=4; k=5;
		f=7.0; g=8.0; h=9.0;
	}

	if(rec<3)
	{
		i=rec;
		rec++;
		Sys_CheckSanity();
		rec--;
		
		if(i!=rec)
			{ DBGBREAK }
	}
	
	if(rec<0)
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity O\n");

	if(!rec)
	{
		tk_puts("Check for: Int Stomp\n");
	
//		if(i!=3)
//			{ DBGBREAK }
		if(j!=4)
			{ DBGBREAK }
		if(k!=5)
			{ DBGBREAK }

		tk_puts("Check for: Double Stomp\n");

		if(f==g)
			{ DBGBREAK }

		if(f!=7.0)
			{ DBGBREAK }
		if(g!=8.0)
			{ DBGBREAK }
		if(h!=9.0)
			{ DBGBREAK }

		tk_puts("Check for: Float Stomp\n");

		if(ff!=1.1f)
			{ DBGBREAK }
		if(gf!=2.2f)
			{ DBGBREAK }
		if(hf!=3.3f)
			{ DBGBREAK }
	}else
	{
		f=rand()*0.01;
		ff=sin(f);
		gf=cos(f);
		g=ff*f;
		h=gf*f;
	}

	if(!rec)	tk_puts("Sanity P\n");

#ifdef _BGBCC
	struct {
		short float sfa;
		short float sfb;
		short float sfc;
	}sfas;
	short float sfa[8];
	short float sf1, sf2, sf3;
	u16 *sfxa;
	sf1=2.0;
	sf2=3.0;
	sf3=sf1+sf2;
	tk_printf("F16 sizeof=%d\n", sizeof(short float));
	tk_printf("F16 A sf1=%f sf2=%f sf3=%f\n", sf1, sf2, sf3);
	
	sfa[0]=sf1;
	sfa[1]=sf2;
	sfa[2]=sfa[0]+sfa[1];
	sf1=sfa[2];
	sf2=sfa[1];
	sf3=sfa[0];
	tk_printf("F16 B sf1=%f sf2=%f sf3=%f\n", sf1, sf2, sf3);

	sfxa=(u16 *)sfa;
	tk_printf("F16 C sf1=%X sf2=%X sf3=%X\n", sfxa[0], sfxa[1], sfxa[2]);
	
	sfas.sfa=sf1;
	sfas.sfb=sf2;
	sfas.sfc=sf3;

	tk_printf("F16 D sf1=%f sf2=%f sf3=%f\n", sfas.sfa, sfas.sfb, sfas.sfc);

	__bfloat16 bf1, bf2, bf3;
	bf1=2.0;
	bf2=3.0;
	bf3=bf1+bf2;
	tk_printf("BF16 sizeof=%d\n", sizeof(__bfloat16));
	tk_printf("BF16 A sf1=%f sf2=%f sf3=%f\n", bf1, bf2, bf3);
	

#if 1
	tk_printf("VAR sizeof=%d\n", sizeof(__variant));

	__variant vf0, vf1, vf2;
	vf0=2;
//	*((int *)-1)=-1;
	vf1=3.0;
//	*((int *)-1)=-1;
	vf2=vf0+vf1;
	tk_printf("VAR A vf1=%f vf2=%f vf3=%f\n",
		(float)vf0, (float)vf1, (float)vf2);
	tk_printf("VAR B vf1=%d vf2=%d vf3=%d\n",
		(int)vf0, (int)vf1, (int)vf2);
	tk_printf("VAR C-1 vf1=%8X_%8X\n", ((int *)(&vf0))[1], ((int *)(&vf0))[0]);
	tk_printf("VAR C-2 vf2=%8X_%8X\n", ((int *)(&vf1))[1], ((int *)(&vf1))[0]);
	tk_printf("VAR C-3 vf3=%8X_%8X\n", ((int *)(&vf2))[1], ((int *)(&vf2))[0]);
//	*((int *)-1)=-1;
#endif

#endif

//	sprintf(tb, "AB%iCD%iEF", 1, 4);
//	if(strcmp(tb, "AB1CD4EF"))
//		{ DBGBREAK }
//	tk_printf("P %s\n", tb);

	sprintf(tb, "AB%iCD%iEF", 1234, 4567);
	tk_printf("P %s\n", tb);
	if(strcmp(tb, "AB1234CD4567EF"))
		{ DBGBREAK }

	if(!rec)	tk_puts("Sanity Q\n");
}

void Sys_CheckSanityB(void)
{
	u32 t_arr[4]={0x12345678, 0xAB89EFCD, 0x00001234, 0x89ABCDEF};
	u64 t_arrl[4]={
		0x0000123412345678ULL, 0xAB89EFCD12345678ULL,
		0x00001234, 0x89ABCDEF};
	double fg;
	float ff;
	byte b_arr0[16];
	u32 ui, uj, uk;
	u64 uli, ulj, ulk;
	int i, j, k;

	tk_printf("VA Tst: (%X %X %X %X) (%X %X %X %X)\n",
		0x12345678, 0x89ABCDEF,	0x56781234, 0xCDEF89AB,
		0x34127856, 0xAB89EFCD,	0x78563412, 0xEFCDAB89);

#if 1
	if((t_arr[0]>>4)!=0x01234567)
		{ DBGBREAK }
	if((((s32)t_arr[1])>>4)!=((s32)0xFAB89EFC))
		{ DBGBREAK }
	if((((u32)t_arr[1])>>4)!=((u32)0x0AB89EFC))
		{ DBGBREAK }

	if((t_arr[0]>>11)!=0x0002468A)
		{ DBGBREAK }
	if((((s32)t_arr[1])>>13)!=((s32)0xFFFD5C4F))
		{ DBGBREAK }
	if((((u32)t_arr[1])>>13)!=((u32)0x00055C4F))
		{ DBGBREAK }
	if((t_arr[2]<<17)!=0x24680000)
		{ DBGBREAK }

	if(((t_arr[0]>>4)&15)!=7)
		{ DBGBREAK }

//	if((((s64)(t_arr[0])>>4)&15)!=7)
//		{ DBGBREAK }
//	if(((0x0123456789ABCDEFLL>>44)&15)!=5)
//		{ DBGBREAK }

	tk_puts("Q Pt0 OK\n");
	
	((short *)(b_arr0))[0]=0x1234;
	((short *)(b_arr0))[1]=0xABCD;
	if(((short *)(b_arr0))[0]!=0x1234)
		{ DBGBREAK }
	if(((short *)(b_arr0))[1]!=((int)0xFFFFABCD))
		{ DBGBREAK }
	if(((unsigned short *)(b_arr0))[0]!=0x1234)
		{ DBGBREAK }
	if(((unsigned short *)(b_arr0))[1]!=0xABCD)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 1\n");

	((char *)(b_arr0))[0]=0x1234;
	((char *)(b_arr0))[1]=0xABCD;
	if(((signed char *)(b_arr0))[0]!=0x34)
		{ DBGBREAK }
	if(((signed char *)(b_arr0))[1]!=((int)0xFFFFFFCD))
		{ DBGBREAK }
	if(((unsigned char *)(b_arr0))[0]!=0x34)
		{ DBGBREAK }
	if(((unsigned char *)(b_arr0))[1]!=0xCD)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 2\n");

	i=t_arr[0]; j=i<<16; k=j>>16;
	if(k!=0x5678)
		{ DBGBREAK }
	i=t_arr[3]; j=i<<16; k=j>>16;
	if(k!=0xFFFFCDEF)
		{ DBGBREAK }
	i=t_arr[3]; j=i<<16; k=((unsigned int)j)>>16;
	if(k!=0xCDEF)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 3\n");
	
	ui=t_arr[3];	uli=ui;
	if(uli!=0x0000000089ABCDEFULL)
		{ DBGBREAK }
	i=t_arr[3];		uli=i;
	if(uli!=0xFFFFFFFF89ABCDEFULL)
		{ DBGBREAK }

	uli=t_arr[3];	ulj=t_arr[0];	ulk=uli*ulj;
	if(ulk!=0x09CA39E0E242D208ULL)
		{ DBGBREAK }
	uli=(s32)(t_arr[3]);	ulj=(s32)(t_arr[0]);	ulk=uli*ulj;
	if(ulk!=0xF795E368E242D208ULL)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 4\n");

	uli=t_arrl[0];	ulj=t_arrl[1];
	ulk=t_arrl[0];
	if(uli==ulj)	{ DBGBREAK }
	if(uli>ulj)		{ DBGBREAK }
	if(uli>=ulj)	{ DBGBREAK }
	if(ulj<uli)		{ DBGBREAK }
	if(ulj<=uli)	{ DBGBREAK }

	if(uli!=ulk)	{ DBGBREAK }
	if(uli>ulk)		{ DBGBREAK }
	if(uli<ulk)		{ DBGBREAK }

	tk_puts("Q Pt0 OK 5\n");

	if((t_arr[0]/10)!=0x1D208A5)
		{ DBGBREAK }
	if((t_arr[0]%10)!=6)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 5-1\n");

#if 1
	*((int *)b_arr0)=10;
	i=*((int *)b_arr0);
	if((t_arr[0]/i)!=0x1D208A5)
		{ DBGBREAK }

	tk_puts("Q Pt0 OK 5-2\n");

	if((t_arr[0]%i)!=6)
		{ DBGBREAK }
#endif

#endif

	tk_puts("Q Pt0 OK 6\n");

	i=123456;
	ff=i;
	j=ff;
	if(i!=j)
		{ DBGBREAK }
		
	i=123456789;
	fg=i;
	j=fg;
	if(i!=j)
		{ DBGBREAK }


	i=-123456;
	ff=i;
	j=ff;
	if(i!=j)
		{ DBGBREAK }
		
	i=-123456789;
	fg=i;
	j=fg;
	if(i!=j)
		{ DBGBREAK }
	
	ff=(i==i);
	ff=!ff;
	j=ff;
	if(j)
		{ DBGBREAK }
	if(ff)
		{ DBGBREAK }
	ff=!ff;
	if(ff!=1.0)
		{ DBGBREAK }
	if(!ff)
		{ DBGBREAK }
		
	ff=123456;
	if(ff!=123456)
		{ DBGBREAK }
}

//=============================================================================

int main (int argc, char **argv)
{
	static quakeparms_t    parms;
	double		time, oldtime, newtime;
	double f, g, h;
	int i, j, k;

	tk_puts("Q Main\n");

	Sys_CheckSanityB();

	COM_InitEndianSwap();

	Sys_CheckSanity();

//	fgetc(stdin);
//	while(1);

	tk_puts("Q A0-0\n");

//	{ DBGBREAK }

//	parms.memsize = 8*1024*1024;
	parms.memsize = 24*1024*1024;
	parms.membase = malloc (parms.memsize);
	parms.basedir = ".";

	tk_puts("Q A0\n");

	fprintf(stderr, "malloc'd: %d @ %p..%p\n",
		parms.memsize,
		parms.membase, parms.membase+parms.memsize);

//	tk_con_disable();

	tk_puts("Q A1\n");

	COM_InitArgv (argc, argv);

	tk_puts("Q A2\n");

	parms.argc = com_argc;
	parms.argv = com_argv;

	printf ("Host_Init\n");
	Host_Init (&parms);

	printf ("Host_Init: Done\n");

	tk_con_disable();

//	Sys_CheckSanity();

	printf ("Enter Main Loop\n");
    oldtime = Sys_FloatTime () - 0.1;
	while (1)
	{
        newtime = Sys_FloatTime ();
        time = newtime - oldtime;
        
        if(time<0)
        {
			newtime = Sys_FloatTime ();
			oldtime = newtime;
			time=0;
		}

//		printf("\t\t\t\t\tT=%f %f dt=%f\r", newtime, oldtime, time);

		oldtime+=time;

		Host_Frame (time);
//		Host_Frame (0.1);
#ifdef __BJX2__
		sleep_0();
#endif
	}
}


