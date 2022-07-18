#include "VFpuVec4SF.h"
#include "verilated.h"

VFpuVec4SF *top = new VFpuVec4SF;

vluint64_t main_time = 0;

#define JX2_UCMD_FPUV4SF		0x0D

#define JX2_UCIX_FPU_PADD		0x05		//FPU ADD (2x F32)
#define JX2_UCIX_FPU_PSUB		0x06		//FPU SUB (2x F32)
#define JX2_UCIX_FPU_PMUL		0x07		//FPU MUL (2x F32)

#define JX2_UCIX_FPU_PADDH		0x15		//FPU ADD (4x F16)
#define JX2_UCIX_FPU_PSUBH		0x16		//FPU SUB (4x F16)
#define JX2_UCIX_FPU_PMULH		0x17		//FPU MUL (4x F16)

#define JX2_UCIX_FPU_PADDX		0x25		//FPU ADD (4x F32)
#define JX2_UCIX_FPU_PSUBX		0x26		//FPU SUB (4x F32)
#define JX2_UCIX_FPU_PMULX		0x27		//FPU MUL (4x F32)


struct JX2_FpuTestCase_s {
int cmd;
int ixt;

vluint64_t x1;
vluint64_t y1;
vluint64_t z1;

vluint64_t x2;
vluint64_t y2;
vluint64_t z2;
}fputest[]={
{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PADDH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PSUBH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PMULH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4220422042204220ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4220422042204220ULL },


{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PADD,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x4091C7004091C700ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PSUB,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x3FDD21003FDD2100ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PMUL,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x408E2580408E2580ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },



{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PADD,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x3FDD2100BFDD2100ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PSUB,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x4091C700C091C700ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PMUL,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0xC08E2580C08E2580ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },


{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PSUB,
	0x40490FD040490FD0ULL,  0x40490FD040490FD0ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PMUL,
	0x40490FD03FB4FDF4ULL,  0x8034FDF40034FDF4ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

// {JX2_UCMD_FPUV4SF, JX2_UCIX_FPU_PADDH,  12345678,    8191,	1507},

{0x00, 0,  0,  0},
};

int main(int argc, char **argv, char **env)
{
	double fx, fy, fz, fw;
	float sfa, sfb, sfc, sfd, sfe, sff;
	vluint64_t tx, ty, tz, tw;
	vluint64_t grn, egrn;
	int64_t dst;
	
	int tst, op, ixt, held;
	int n, isz_x, isz_y, isnan_x, isnan_y;
	
	printf("FPU Test\n");
	
	Verilated::commandArgs(argc, argv);

	fx=3.14; fy=2.73;
	tx=*(vluint64_t *)(&fx);
	ty=*(vluint64_t *)(&fy);
	tst=0;
	held=0;

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;
//		top->mode = 3;

		top->exHold=0;
		
//		if(top->exOutHold)
//		{
//			top->exInHold=1;
//			top->eval();
//			main_time++;
//			continue;
//		}

		if(top->clock)
		{
			top->eval();
			main_time++;
			continue;
		}
		
		if(held)
		{
			held--;
			top->eval();
			main_time++;
			continue;
		}

		if(top->opCmdA)
		{
			tz=top->regValRnA;

			*(vluint32_t *)(&sfa)=tx;
			*(vluint32_t *)(&sfb)=tx>>32;
			*(vluint32_t *)(&sfc)=ty;
			*(vluint32_t *)(&sfd)=ty>>32;
			*(vluint32_t *)(&sfe)=tz;
			*(vluint32_t *)(&sff)=tz>>32;

			printf(
				"%02X-%02X Rn=%lX Rm=%lX\n"
				"Ro=%016lX Expect=%016lX %s\n",
				top->opCmdA, top->regIdIxtA,
				tx, ty, tz, tw, (tz==tw)?"Pass":"Fail");

			if(ixt&16)
			{
			}else
			{
				printf("S=(%f %f) T=(%f %f) O=(%f %f)\n",
					sfa, sfb, sfc, sfd, sfe, sff);
			}

			printf("\n");

			top->opCmdA=0x00;
			top->regIdIxtA=0;
			tst++;

			op=fputest[tst].cmd;
			if(!op)
				break;
		}else
		{
			op=fputest[tst].cmd;
			ixt=fputest[tst].ixt;
			tx=fputest[tst].x1;
			ty=fputest[tst].y1;
			tw=fputest[tst].z1;
//			egrn=fputest[tst].grn;
			
			if(!op)
				break;
		
			top->opCmdA=op;
			top->regIdIxtA=ixt;

			top->regValRsA=tx;
			top->regValRtA=ty;

			held=1;
		}

		top->eval();

		main_time++;
		
//		if(main_time>256)
		if(main_time>8192)
		{
			break;
		}
	}

	delete top;
	exit(0);
}

// verilator -Wall --cc our.v --exe sim_main.cpp
