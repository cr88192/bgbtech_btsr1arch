#include "VJx2FphfVecOp.h"
#include "verilated.h"

typedef vluint16_t u16;
typedef vluint32_t u32;
typedef vluint64_t u64;

VJx2FphfVecOp *top = new VJx2FphfVecOp;

vluint64_t main_time = 0;

u16 jx2_f2h(float f)
{
	u32 v0;
	u16 v1;
	
	v0=*(u32 *)(&f);
	v0+=4095;
	v1=((v0>>16)&0xC000) | ((v0>>13)&0x3FFF);
	return(v1);
}

float jx2_h2f(u16 v0)
{
	u32 v1;
	
	v1=((v0&0x8000)<<16) |
		(((v0&0x7FFF)+0x1C000)<<13);
	return(*(float *)(&v1));
}

u64 jx2_mkvec_hf(float f0, float f1, float f2, float f3)
{
	u16 hv0, hv1, hv2, hv3;
	u32 v0, v1;
	u64 v;
	
	hv0=jx2_f2h(f0);
	hv1=jx2_f2h(f1);
	hv2=jx2_f2h(f2);
	hv3=jx2_f2h(f3);
	
	v0=hv0 | (hv1<<16);
	v1=hv2 | (hv3<<16);
	v=v0 | (((u64)v1)<<32);
	return(v);
}

void jx2_upvec_hf(float *fv, u64 v0)
{
	fv[0]=jx2_h2f(v0>> 0);
	fv[1]=jx2_h2f(v0>>16);
	fv[2]=jx2_h2f(v0>>32);
	fv[3]=jx2_h2f(v0>>48);
}

u64 jx2_dovec_hf(u64 v0, u64 v1, int op)
{
	float tv0[4], tv1[4], tv2[4];
	u64 v2;

	jx2_upvec_hf(tv0, v0);
	jx2_upvec_hf(tv1, v1);
	
	switch(op)
	{
	case 0:
		tv2[0]=0;
		tv2[1]=0;
		tv2[2]=0;
		tv2[3]=0;
		break;
	case 1:
		tv2[0]=tv0[0]+tv1[0];
		tv2[1]=tv0[1]+tv1[1];
		tv2[2]=tv0[2]+tv1[2];
		tv2[3]=tv0[3]+tv1[3];
		break;
	case 2:
		tv2[0]=tv0[0]-tv1[0];
		tv2[1]=tv0[1]-tv1[1];
		tv2[2]=tv0[2]-tv1[2];
		tv2[3]=tv0[3]-tv1[3];
		break;
	case 3:
		tv2[0]=tv0[0]*tv1[0];
		tv2[1]=tv0[1]*tv1[1];
		tv2[2]=tv0[2]*tv1[2];
		tv2[3]=tv0[3]*tv1[3];
		break;
	default:
		break;
	}
	
	v2=jx2_mkvec_hf(tv2[0], tv2[1], tv2[2], tv2[3]);
	return(v2);
}

int main(int argc, char **argv, char **env)
{
	float tv0[4], tv1[4], tv2[4];
	float f, g;
	u64 v0, v1, v2, v3;
	u16 hv0;
	int op;

	f=3.14;
	hv0=jx2_f2h(f);
	g=jx2_h2f(hv0);

	printf("RTT0 %f %04X %f\n", f, hv0, g);

	Verilated::commandArgs(argc, argv);

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
		main_time++;

		top->eval();
		
		op=1;

		v0=jx2_mkvec_hf(3.140, 2.780, 1.414, 0.707);
		v1=jx2_mkvec_hf(1.414, 2.780, 3.140, 7.389);
		v3=jx2_dovec_hf(v0, v1, op);

		top->regValRs=v0;
		top->regValRt=v1;
		top->regExOp=op;
		v2=top->regValRo;
		
		if(top->regExOK==1)
		{
			jx2_upvec_hf(tv0, v2);
			jx2_upvec_hf(tv1, v3);

			printf("v0=%016lX v1=%016lX got=%016lX exp=%016lX\n",
				v0, v1, v2, v3);
			
			printf("Got: %.3f %.3f %.3f %.3f\n",
				tv0[0], tv0[1], tv0[2], tv0[3]);
			printf("Exp: %.3f %.3f %.3f %.3f\n",
				tv1[0], tv1[1], tv1[2], tv1[3]);
			break;
		}
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
