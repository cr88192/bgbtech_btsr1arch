#include "VJx2FpuExOp.h"
#include "verilated.h"

VJx2FpuExOp *top = new VJx2FpuExOp;

vluint64_t main_time = 0;

#define JX2_UCMD_FPU_FADD3 0x78
#define JX2_UCMD_FPU_FSUB3 0x79
#define JX2_UCMD_FPU_FMUL3 0x7A
#define JX2_UCMD_FPU_FDIV3 0x7B

#define JX2_NODIV

struct JX2_FpuTestCase_s {
int cmd;
int ixt;
double x;
double y;
double z;
}fputest[]={
{JX2_UCMD_FPU_FADD3, 0,  3.14,  2.73,  5.870000000},
{JX2_UCMD_FPU_FSUB3, 0,  3.14,  2.73,  0.410000000},
{JX2_UCMD_FPU_FMUL3, 0,  3.14,  2.73,  8.572200000},

{JX2_UCMD_FPU_FADD3, 0,  3.14, -2.73,  0.410000000},
{JX2_UCMD_FPU_FSUB3, 0,  3.14, -2.73,  5.870000000},
{JX2_UCMD_FPU_FMUL3, 0,  3.14, -2.73, -8.572200000},

{JX2_UCMD_FPU_FADD3, 0, -3.14,  2.73, -0.410000000},
{JX2_UCMD_FPU_FSUB3, 0, -3.14,  2.73, -5.870000000},
{JX2_UCMD_FPU_FMUL3, 0, -3.14,  2.73, -8.572200000},

#ifndef JX2_NODIV
{JX2_UCMD_FPU_FDIV3, 0,  3.14,  2.73,  1.1501831501831501831501831501832},
{JX2_UCMD_FPU_FDIV3, 0,  3.14, -2.73, -1.1501831501831501831501831501832},
{JX2_UCMD_FPU_FDIV3, 0, -3.14,  2.73, -1.1501831501831501831501831501832},
#endif

{0x57, 0,  3.14,  2.73, -3.140000000},
{0x57, 1,  3.14,  2.73,  3.140000000},

#ifndef JX2_NODIV
{0x57, 2,  3.14,  2.73,  0.318471338},
{0x57, 3,  3.14,  2.73,  1.772004515},
#endif

{0x57, 0, -3.14,  2.73,  3.140000000},
{0x57, 1, -3.14,  2.73,  3.140000000},

#ifndef JX2_NODIV
{0x57, 2, -3.14,  2.73, -0.318471338},
{0x57, 3, -3.14,  2.73, -1.772004515},
#endif

{JX2_UCMD_FPU_FADD3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  5.870000000},
{JX2_UCMD_FPU_FSUB3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  0.410000000},
{JX2_UCMD_FPU_FMUL3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  8.572200000},

{0x00, 0,  3.14,  2.73,  0.000000000},
};

int main(int argc, char **argv, char **env)
{
	double fx, fy, fz, fw;
	vluint64_t tx, ty, tz, tw;
	int tst, op, ixt;
	
	printf("FPU Test\n");
	
	Verilated::commandArgs(argc, argv);

	fx=3.14; fy=2.73;
	tx=*(vluint64_t *)(&fx);
	ty=*(vluint64_t *)(&fy);
	tst=0;

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
//		top->mode = 3;
		
		if(top->regOutOK==2)
		{
			top->eval();
			main_time++;
			continue;
		}

		if(top->regOutOK==1)
		{
			tz=top->regOutVal;
			fz=*(double *)(&tz);

			printf(
				"%02X-%02X Rn=%lX(%f) Rm=%lX(%f)\n"
				"Ro=%lX(%f) Expect=%lX(%f)\n",
				top->opCmd, top->regIdIxt,
				tx, fx, ty, fy, tz, fz, tw, fw);
			printf("\n");

			top->opCmd=0x00;
			top->regIdIxt=0;
			tst++;
		}


		if(top->regOutOK==0)
		{
			op=fputest[tst].cmd;
			ixt=fputest[tst].ixt;
			fx=fputest[tst].x;
			fy=fputest[tst].y;
			fw=fputest[tst].z;

			switch(op)
			{
			case JX2_UCMD_FPU_FADD3:	fw=fx+fy; break;
			case JX2_UCMD_FPU_FSUB3:	fw=fx-fy; break;
			case JX2_UCMD_FPU_FMUL3:	fw=fx*fy; break;
			case JX2_UCMD_FPU_FDIV3:	fw=fx/fy; break;
			}

			tx=*(vluint64_t *)(&fx);
			ty=*(vluint64_t *)(&fy);
			tw=*(vluint64_t *)(&fw);
		
//			top->opCmd=JX2_UCMD_FPU_FADD3;
			top->opCmd=op;
			top->regIdIxt=ixt;

			top->regIdRs=3;
			top->regValRs=tx;

			top->regIdRt=11;
			top->regValRt=ty;

			top->memDataLd=0;
			top->memDataOK=0;
			top->ctlInDlr=0;
		}

		top->eval();

		main_time++;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

//		if(main_time>256)
		if(main_time>4096)
		{
//			printf("%llX\n", (long long)(top->outAddr));
			break;
		}
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
