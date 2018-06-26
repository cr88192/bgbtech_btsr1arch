#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "tkcl_softdiv.c"
#include "tkcl_softfpu.c"

int main(int argc, char *argv[])
{
	float fx, fy, fz;
	int a, b, c, d, e, f;
	int i;
	
	printf("SoftDiv\n");
	
//	a=2235;	b=69;
	a=0x31415927U;
	b=69;
	c=a/b;
	d=tkcl_div_si(a, b);
	
	printf("%d %d (%d %d)\n", a, b, c, d);
	
	for(i=0; i<16; i++)
	{
		a=rand();
		b=rand();
		if(b>a)
			{ c=a; a=b; b=c; }
		if(!b)b=1;
		c=a/b;
		d=tkcl_div_si(a, b);
		e=a%b;
		f=tkcl_mod_si(a, b);
		printf("%d %d (%d %d) (%d %d)\n", a, b, c, d, e, f);
	}

	printf("SoftFPU\n");
	
	fx=3;	fy=4;
	a=*(uint32_t *)(&fx);
	b=*(uint32_t *)(&fy);
//	c=*(uint32_t *)(&fz);
//	d=__sfp_fadd_f32(a, b);
	c=__sfp_float_f32(3);
	d=__sfp_float_f32(4);
	e=b-d; if(e<0)e=-e;
	printf("i->f x,y: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);
	
	fx=3.14159; fy=2.7182818285;
	fz=fx+fy;
	
	a=*(uint32_t *)(&fx);
	b=*(uint32_t *)(&fy);
	c=*(uint32_t *)(&fz);
	d=__sfp_fadd_f32(a, b);
	e=c-d; if(e<0)e=-e;
	printf("x+y: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);

	fz=fx-fy;
	a=*(uint32_t *)(&fx);
	b=*(uint32_t *)(&fy);
	c=*(uint32_t *)(&fz);
	d=__sfp_fsub_f32(a, b);
	e=c-d; if(e<0)e=-e;
	printf("x-y: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);

	fz=fx*fy;
	a=*(uint32_t *)(&fx);
	b=*(uint32_t *)(&fy);
	c=*(uint32_t *)(&fz);
	d=__sfp_fmul_f32(a, b);
	e=c-d; if(e<0)e=-e;
	printf("x*y: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);

	fz=fx/fy;
	a=*(uint32_t *)(&fx);
	b=*(uint32_t *)(&fy);
	c=*(uint32_t *)(&fz);
	d=__sfp_fdiv_f32(a, b);
	e=c-d; if(e<0)e=-e;
	printf("x/y: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);

	fz=1.0/fx;
	c=*(uint32_t *)(&fz);
	d=tkcl_sfp_rcp_f32(a);
	e=c-d; if(e<0)e=-e;
	printf("1/x: %08X %08X %08X %08X  %08X\n", a, b, c, d, e);

}
