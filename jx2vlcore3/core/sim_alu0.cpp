#include "VExALU.h"
#include "verilated.h"

VExALU *top = new VExALU;

vluint64_t main_time = 0;

#define JX2_UCMD_ALU3			0x10		//ALU Command (3R)
#define JX2_UCMD_ALUCMP			0x11		//ALU Compare
#define JX2_UCMD_UNARY			0x12		//ALU Unary Operator

#define JX2_UCIX_ALU_ADD		0x20		//ALU ADD
#define JX2_UCIX_ALU_SUB		0x21		//ALU SUB
#define JX2_UCIX_ALU_ADC		0x22		//ALU ADC
#define JX2_UCIX_ALU_SBB		0x23		//ALU SBB
#define JX2_UCIX_ALU_TSTQ		0x24		//ALU TST
#define JX2_UCIX_ALU_AND		0x25		//ALU AND
#define JX2_UCIX_ALU_OR			0x26		//ALU OR
#define JX2_UCIX_ALU_XOR		0x27		//ALU XOR
#define JX2_UCIX_ALU_CMPQNE		0x28		//ALU CMPQNE
#define JX2_UCIX_ALU_CMPQHS		0x29		//ALU CMPQHS
#define JX2_UCIX_ALU_CMPQGE		0x2A		//ALU CMPQGE
#define JX2_UCIX_ALU_NOR		0x2B		//ALU NOR
#define JX2_UCIX_ALU_CMPQEQ		0x2C		//ALU Command
#define JX2_UCIX_ALU_CMPQHI		0x2D		//ALU Command
#define JX2_UCIX_ALU_CMPQGT		0x2E		//ALU Command
#define JX2_UCIX_ALU_CSELT		0x2F		//ALU CSELT

#define JX2_UCIX_ALU_ADDSL		0x00		//ALU ADDSL
#define JX2_UCIX_ALU_SUBSL		0x01		//ALU SUBSL

#define JX2_UCIX_ALU_ADDUL		0x10		//ALU ADDUL
#define JX2_UCIX_ALU_SUBUL		0x11		//ALU SUBUL

#define JX2_UCIX_ALU_TST		0x04		//ALU Command
#define JX2_UCIX_ALU_CMPNE		0x08		//ALU Command
#define JX2_UCIX_ALU_CMPHS		0x09		//ALU Command
#define JX2_UCIX_ALU_CMPGE		0x0A		//ALU Command
#define JX2_UCIX_ALU_CMPEQ		0x0C		//ALU Command
#define JX2_UCIX_ALU_CMPHI		0x0D		//ALU Command
#define JX2_UCIX_ALU_CMPGT		0x0E		//ALU Command

#define JX2_UCIX_ALU_PADDL		0x30		//Packed ALU ADD
#define JX2_UCIX_ALU_PSUBL		0x31		//Packed ALU SUB
#define JX2_UCIX_ALU_PADCL		0x32		//Packed ALU ADC
#define JX2_UCIX_ALU_PSBBL		0x33		//Packed ALU SBB

#define JX2_UCIX_ALU_PCSELT		0x3F		//Packed CSELT

struct JX2_FpuTestCase_s {
int cmd;
int ixt;
vluint64_t x;
vluint64_t y;
vluint64_t z;
}fputest[]={
{JX2_UCMD_ALU3, JX2_UCIX_ALU_ADD,	0x12345678,	0x1234CDEF,	0x24692467},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_SUB,	0x12345678,	0x1234CDEF,
	0xFFFFFFFFFFFF8889LL},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_ADC,	0x12345678,	0x1234CDEF,	0x24692467},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_SBB,	0x12345678,	0x1234CDEF,
	0xFFFFFFFFFFFF8889LL},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_AND,	0x12345678,	0x1234CDEF,	0x12344468},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_OR,	0x12345678,	0x1234CDEF,	0x1234DFFF},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_XOR,	0x12345678,	0x1234CDEF,	0x00009B97},

{JX2_UCMD_ALU3, JX2_UCIX_ALU_ADDSL,	0x12345678,	0x1234CDEF,	0x24692467},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_SUBSL,	0x12345678,	0x1234CDEF,
	0xFFFFFFFFFFFF8889LL},

{JX2_UCMD_ALU3, JX2_UCIX_ALU_ADDUL,	0x12345678,	0x1234CDEF,	0x24692467},
{JX2_UCMD_ALU3, JX2_UCIX_ALU_SUBUL,	0x12345678,	0x1234CDEF,	0x0FFFF8889LL},

{0x00, 0,  3.14,  2.73,  0.000000000},
};

int main(int argc, char **argv, char **env)
{
	vluint64_t tx, ty, tz, tw;
	int tst, op, ixt, tsr;
	
	printf("ALU Test\n");
	
	Verilated::commandArgs(argc, argv);

	tst=0; op=0; tsr=0;

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
//		top->mode = 3;
		
		if(!top->clock)
		{
			top->eval();
			main_time++;
			continue;
		}
		
//		if(top->regOutOK==2)
		if((main_time>>1)&1)
		{
			top->eval();
			main_time++;
			continue;
		}

//		if(top->regOutOK==1)
		if(op)
		{
			tz=top->regOutVal;
			tsr=top->regOutSrST;

			printf(
				"%02X-%02X Rs=%lX Rt=%lX\n"
				"Rn=%lX Expect=%lX SR=%02X\n",
				op, top->idUIxt,
				tx, ty, tz, tw, tsr&63);
			printf("\n");

//			top->opCmd=0x00;
			top->idUIxt=0;
			tst++;

			op=fputest[tst].cmd;
			if(!op)
				break;
		}


//		if(top->regOutOK==0)
		if(1)
		{
			op=fputest[tst].cmd;
			ixt=fputest[tst].ixt;
			tx=fputest[tst].x;
			ty=fputest[tst].y;
			tw=fputest[tst].z;
			
			if(!op)
				break;
		
//			top->opCmd=op;
			top->idUIxt=ixt;

//			top->regIdRs=3;
			top->regValRs=tx;

//			top->regIdRt=11;
			top->regValRt=ty;

			top->regInSrST=tsr;

//			top->memDataLd=0;
//			top->memDataOK=0;
//			top->ctlInDlr=0;
		}

		top->eval();

		main_time++;
		
		if(main_time>4096)
		{
			break;
		}
	}
	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
