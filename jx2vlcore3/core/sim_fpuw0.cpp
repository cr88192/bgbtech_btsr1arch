#include "VFpuExOpW.h"
#include "verilated.h"

VFpuExOpW *top = new VFpuExOpW;

vluint64_t main_time = 0;

#define JX2_UCMD_FPU3			0x22
#define JX2_UCMD_FLDCX			0x23
#define JX2_UCMD_FSTCX			0x24
#define JX2_UCMD_FIXS			0x25
#define JX2_UCMD_FCMP			0x26

#define JX2_UCIX_FPU_FADD		0x00		//FPU ADD
#define JX2_UCIX_FPU_FSUB		0x01		//FPU SUB
#define JX2_UCIX_FPU_FMUL		0x02		//FPU MUL
#define JX2_UCIX_FPU_FDIV		0x03		//FPU DIV
#define JX2_UCIX_FPU_FMOV		0x04		//FPU MOV

#define JX2_UCIX_FPU_FDIVA		0x13		//FPU DIV

#define JX2_UCIX_FPU_FADD_G		0x10		//FPU ADD (GFP)
#define JX2_UCIX_FPU_FSUB_G		0x11		//FPU SUB (GFP)
#define JX2_UCIX_FPU_FMUL_G		0x12		//FPU MUL (GFP)

#define JX2_UCIX_FPU_CMPNE		0x08		//ALU Command
#define JX2_UCIX_FPU_CMPGE		0x0A		//ALU Command
#define JX2_UCIX_FPU_CMPEQ		0x0C		//ALU Command
#define JX2_UCIX_FPU_CMPGT		0x0E		//ALU Command

#define JX2_UCIX_FPU_CMPNE_G	0x18		//ALU Command
#define JX2_UCIX_FPU_CMPGE_G	0x1A		//ALU Command
#define JX2_UCIX_FPU_CMPEQ_G	0x1C		//ALU Command
#define JX2_UCIX_FPU_CMPGT_G	0x1E		//ALU Command

#define JX2_UCIX_FPU_FSQRT		0x0F		//FPU SQRT
#define JX2_UCIX_FPU_FSQRTA		0x1F		//FPU SQRT

#define JX2_UCIX_FPIX_FNEG		0x00		//FPU ADD
#define JX2_UCIX_FPIX_FABS		0x01		//FPU SUB
#define JX2_UCIX_FPIX_FRCP		0x02		//FPU SUB
#define JX2_UCIX_FPIX_FSQRT		0x03		//FPU SUB
#define JX2_UCIX_FPIX_FRCPA		0x04		//FPU SUB
#define JX2_UCIX_FPIX_FSQRTA	0x05		//FPU SUB

#define JX2_UCIX_FPIX_FNEG_G	0x10		//FPU Negate
#define JX2_UCIX_FPIX_FABS_G	0x11		//FPU Absolute

#define JX2_UCIX_FPCX_S			0x00		//FPU Single
#define JX2_UCIX_FPCX_D			0x01		//FPU Double
#define JX2_UCIX_FPCX_I			0x02		//FPU Int
#define JX2_UCIX_FPCX_H			0x03		//FPU Half
#define JX2_UCIX_FPCX_S2		0x08		//FPU Single (High)

#define JX2_UCIX_FPCX_SG		0x10		//FPU Single
#define JX2_UCIX_FPCX_DG		0x11		//FPU Double
#define JX2_UCIX_FPCX_IG		0x12		//FPU Int
#define JX2_UCIX_FPCX_HG		0x13		//FPU Half
#define JX2_UCIX_FPCX_S2G		0x18		//FPU Single (High)


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
vluint64_t grn;
}fputest[]={
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.14,  2.73,  5.870000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSUB,  3.14,  2.73,  0.410000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,  3.14,  2.73,  8.572200000},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.14, -2.73,  0.410000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSUB,  3.14, -2.73,  5.870000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,  3.14, -2.73, -8.572200000},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD, -3.14,  2.73, -0.410000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSUB, -3.14,  2.73, -5.870000000},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL, -3.14,  2.73, -8.572200000},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.14,  0,  3.14},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSUB,  3.14,  0,  3.14},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,  3.14,  0,  0},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.14      ,  0.001592654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.141     ,  0.000592654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.1415    ,  0.000092654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.14159   ,  0.00002654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.141592  ,  0.00000654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.1415926 ,  0.00000054},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  3.141592654, -3.14159265,  0.00000004},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.14      ,  -0.001592654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.141     ,  -0.000592654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.1415    ,  -0.000092654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.14159   ,  -0.00002654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.141592  ,  -0.00000654},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.1415926 ,  -0.00000054},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  -3.141592654, 3.14159265,  -0.00000004},


{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,  2.634884, -3.048831,  -0.413947},


{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,  0.00000004, 0.00000004, 0.0000000000000016},

{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  6972, 0,  6972.0},
{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  -6972, 0,  -6972.0},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  6972.0, 0,  6972.0, 6972},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -6972.0, 0,  -6972.0, -6972},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  6972.005, 0,  6972.0, 6972},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  6972.375, 0,  6972.0, 6972},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  6972.995, 0,  6972.0, 6972},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -6972.005, 0,  6972.0, -6972},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -6972.375, 0,  6972.0, -6972},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -6972.995, 0,  6972.0, -6972},

{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  123, 0,  123.0},
{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  -123, 0,  -123.0},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  123.0, 0,  6972.0, 123},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -123.0, 0,  -6972.0, -123},

{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  13, 0,  13.0},
{JX2_UCMD_FLDCX, JX2_UCIX_FPCX_I,  -13, 0,  -13.0},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  13.0, 0,  6972.0, 13},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -13.0, 0,  -6972.0, -13},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  3.14159, 0,  6972.0, 3},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -3.14159, 0,  -6972.0, -3},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  2.0, 0,  6972.0, 2},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -2.0, 0,  -6972.0, -2},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  1.0, 0,  6972.0, 1},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -1.0, 0,  -6972.0, -1},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  0.0, 0,  6972.0, 0},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -0.0, 0,  -6972.0, 0},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  0.75, 0,  6972.0, 0},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -0.75, 0,  -6972.0, 0},

{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  0.00000001, 0,  6972.0, 0},
{JX2_UCMD_FSTCX, JX2_UCIX_FPCX_I,  -0.00000001, 0,  -6972.0, 0},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV,  2.634884,  3.048831,  0.86422763347656856},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV,  2.634884, -3.048831, -0.86422763347656856},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV, -2.634884,  3.048831, -0.86422763347656856},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV, -2.634884, -3.048831,  0.86422763347656856},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSQRT,  3.14159,  3.048831,  1.77245310234149},

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSQRT,  9.8695877281,  3.048831,  3.14159 },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV,   3.048831, 2.634884,  
			1.1571025517631895749490300142245},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV,   3.14159, 1.414214,  
			2.2214389052859043963643408989021},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIV,   1.414214,  3.14159, 
			0.45015867761229186494736741586267},


{JX2_UCMD_FPU3, JX2_UCIX_FPU_FDIVA,   1.414214,  3.14159, 
			0.45015867761229186494736741586267},
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSQRTA,  9.8695877281,  3.048831,  3.14159 },

#if 0
{0x57, 0,  3.14,  2.73, -3.140000000},
{0x57, 1,  3.14,  2.73,  3.140000000},

{0x57, 0, -3.14,  2.73,  3.140000000},
{0x57, 1, -3.14,  2.73,  3.140000000},

{JX2_UCMD_FPU_FADD3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  5.870000000},
{JX2_UCMD_FPU_FSUB3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  0.410000000},
{JX2_UCMD_FPU_FMUL3, 0,  3.6055512754639892931192212674705,  4.1231056256176605498214098559741,  8.572200000},
#endif

{0x00, 0,  3.14,  2.73,  0.000000000},
};

int main(int argc, char **argv, char **env)
{
	double fx, fy, fz, fw;
	vluint64_t tx, ty, tz, tw;
	vluint64_t grn, egrn;
	int64_t dst;
	
	int tst, op, ixt;
	int n, isz_x, isz_y, isnan_x, isnan_y;
	
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

		top->exHold=0;
		
		if(top->regOutOK==2)
		{
			top->exHold=1;
			top->eval();
			main_time++;
			continue;
		}

		if((top->regOutOK==1) && top->opCmdA)
		{
//			tz=top->regOutValA;
			tz=top->regValGRnA;
			fz=*(double *)(&tz);

			grn=top->regValGRnA;
			if((tz==tw) || ((op==JX2_UCMD_FSTCX) && (grn==egrn)))
			{
				printf(
					"%02X-%02X Rn=%016llX(% f) Rm=%016llX(% f)  Exact\n",
					top->opCmdA, top->regIdIxtA,
					tx, fx, ty, fy);
			}else
			{
				printf(
					"%02X-%02X Rn=%lX(%f) Rm=%lX(%f)\n"
					"Ro=%016lX(%f) Expect=%016lX(%f) E=%ld\n",
					top->opCmdA, top->regIdIxtA,
					tx, fx, ty, fy, tz, fz, tw, fw, tz-tw);

				if(op==JX2_UCMD_FSTCX)
				{
					grn=top->regValGRnA;
					printf("GRn=%llX (%lld), Expect=%llX(%lld)\n",
						grn, grn, egrn, egrn);
				}
				printf("\n");
			}


			top->opCmdA=0x00;
			top->opCmdB=0x00;
			top->regIdIxtA=0;
			top->regIdIxtB=0;
			tst++;

			op=fputest[tst].cmd;
			if(!op)
				break;
		}


		if(top->regOutOK==0)
		{
			op=fputest[tst].cmd;
			ixt=fputest[tst].ixt;
			fx=fputest[tst].x;
			fy=fputest[tst].y;
			fw=fputest[tst].z;
			egrn=fputest[tst].grn;
			
			if(!op)
				break;

			switch(op)
			{
			case JX2_UCMD_FPU3:
				switch(ixt&15)
				{
				case JX2_UCIX_FPU_FADD:
					fw=fx+fy; break;
				case JX2_UCIX_FPU_FSUB:
					fw=fx-fy; break;
				case JX2_UCIX_FPU_FMUL:
					fw=fx*fy; break;
				}
				break;
			}

			tx=*(vluint64_t *)(&fx);
			ty=*(vluint64_t *)(&fy);
			tw=*(vluint64_t *)(&fw);
			
			if(op==JX2_UCMD_FLDCX)
			{
				switch(ixt&15)
				{
				case JX2_UCIX_FPCX_I:
					tx=fx;
					break;
				}
			}
		
//			top->opCmd=JX2_UCMD_FPU_FADD3;
			top->opCmdA=op;
			top->opCmdB=op;
			top->regIdIxtA=ixt;
			top->regIdIxtB=ixt;

			top->regIdRsA=3;
			top->regValRsA=tx;

			top->regIdRtA=11;
			top->regValRtA=ty;

//			top->memDataLd=0;
//			top->memDataOK=0;
//			top->ctlInDlr=0;
		}

		top->eval();

		main_time++;
		
//		top->baseAddr=0xDECAB00;
//		top->idxAddr=0x100;
//		top->idxDisp=3;

//		if(main_time>256)
//		if(main_time>4096)
//		if(main_time>16384)
		if(main_time>65536)
		{
//			printf("%llX\n", (long long)(top->outAddr));
			break;
		}
	}

	while (!Verilated::gotFinish())
	{
		if(top->regOutOK==0)
			break;

		top->clock = (main_time>>0)&1;
		main_time++;
		top->eval();
	}

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
