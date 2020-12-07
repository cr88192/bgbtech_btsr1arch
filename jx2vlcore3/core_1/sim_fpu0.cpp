#include "VFpuExOp.h"
#include "verilated.h"

VFpuExOp *top = new VFpuExOp;

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

		if((top->regOutOK==1) && top->opCmd)
		{
			tz=top->regOutVal;
			fz=*(double *)(&tz);

			printf(
				"%02X-%02X Rn=%lX(%f) Rm=%lX(%f)\n"
				"Ro=%016lX(%f) Expect=%016lX(%f)\n",
				top->opCmd, top->regIdIxt,
				tx, fx, ty, fy, tz, fz, tw, fw);

			if(op==JX2_UCMD_FSTCX)
			{
				grn=top->regValGRn;
				printf("GRn=%llX (%lld), Expect=%llX(%lld)\n",
					grn, grn, egrn, egrn);
			}

			printf("\n");

			top->opCmd=0x00;
			top->regIdIxt=0;
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

#if 0
			switch(op)
			{
			case JX2_UCMD_FPU_FADD3:	fw=fx+fy; break;
			case JX2_UCMD_FPU_FSUB3:	fw=fx-fy; break;
			case JX2_UCMD_FPU_FMUL3:	fw=fx*fy; break;
			case JX2_UCMD_FPU_FDIV3:	fw=fx/fy; break;
			}
#endif

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
			top->opCmd=op;
			top->regIdIxt=ixt;

			top->regIdRs=3;
			top->regValRs=tx;

			top->regIdRt=11;
			top->regValRt=ty;

			top->memDataLd=0;
			top->memDataOK=0;
//			top->ctlInDlr=0;
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

	while (!Verilated::gotFinish())
	{
		if(top->regOutOK==0)
			break;

		top->clock = (main_time>>0)&1;
		main_time++;
		top->eval();
	}

#if 0
	n=4096; op=0;
	while (!Verilated::gotFinish())
	{
		printf("%d %d\r", n, op);
		fflush(stdout);
	
		if(top->regOutOK==1)
		{
			top->opCmd=0x00;
			top->regIdIxt=0;

			tz=top->regOutVal;
			fz=*(double *)(&tz);
			
			dst=tz-tw;
			if(dst<0)dst=-dst;
			
			isz_x = ((tz>>52)&2047)==0;
			isz_y = ((tw>>52)&2047)==0;

			isnan_x = ((tz>>52)&2047)==2047;
			isnan_y = ((tw>>52)&2047)==2047;
			
			if((dst>4) && (!isz_x || !isz_y) && (!isnan_x || !isnan_y))
			{
				printf("x=%016llX y=%016llX op=%d rn=%016llX exp=%016llX\n",
					tx, ty, op, tz, tw);
			}

			if(n>0)
			{
				n--;
			}else if(op<2)
			{
				op++;
				n=4096;
			}else
			{
				break;
			}
		}

		if(top->regOutOK==0)
		{
			tx=rand();
			tx=(tx*65521)+(rand());
			tx=(tx*65521)+(rand());
			tx=(tx*65521)+(rand());
			tx=(tx*65521)+(rand());

			ty=rand();
			ty=(ty*65521)+(rand());
			ty=(ty*65521)+(rand());
			ty=(ty*65521)+(rand());
			ty=(ty*65521)+(rand());

			fx=*(double *)(&tx);
			fy=*(double *)(&ty);
			
			switch(op)
			{
			case 0: fw=fx+fy; break;
			case 1: fw=fx-fy; break;
			case 2: fw=fx*fy; break;
			}
			tw=*(vluint64_t *)(&fw);

			top->opCmd=JX2_UCMD_FPU3;
			top->regIdIxt=op;

			top->regIdRs=3;
			top->regValRs=tx;

			top->regIdRt=11;
			top->regValRt=ty;
		}

		top->clock = (main_time>>0)&1;
		main_time++;
		top->eval();
	}
#endif

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
