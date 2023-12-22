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

#define JX2_UCIX_FPU_PADD		0x05		//FPU ADD (2x F32)
#define JX2_UCIX_FPU_PSUB		0x06		//FPU SUB (2x F32)
#define JX2_UCIX_FPU_PMUL		0x07		//FPU MUL (2x F32)

#define JX2_UCIX_FPU_FDIVA		0x13		//FPU DIV

#define JX2_UCIX_FPU_FADD_G		0x10		//FPU ADD (GFP)
#define JX2_UCIX_FPU_FSUB_G		0x11		//FPU SUB (GFP)
#define JX2_UCIX_FPU_FMUL_G		0x12		//FPU MUL (GFP)

#define JX2_UCIX_FPU_PADDH		0x15		//FPU ADD (4x F16)
#define JX2_UCIX_FPU_PSUBH		0x16		//FPU SUB (4x F16)
#define JX2_UCIX_FPU_PMULH		0x17		//FPU MUL (4x F16)

#define JX2_UCIX_FPU_PADDX		0x25		//FPU ADD (4x F32)
#define JX2_UCIX_FPU_PSUBX		0x26		//FPU SUB (4x F32)
#define JX2_UCIX_FPU_PMULX		0x27		//FPU MUL (4x F32)

#define JX2_UCIX_FPU_PADDXD		0x35		//FPU ADD (2x F64)
#define JX2_UCIX_FPU_PSUBXD		0x36		//FPU SUB (2x F64)
#define JX2_UCIX_FPU_PMULXD		0x37		//FPU MUL (2x F64)

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

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,  3.14,  2.17,  6.8138},

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



struct JX2_SimdTestCase_s {
int cmd;
int ixt;

vluint64_t x1;
vluint64_t y1;
vluint64_t z1;

vluint64_t x2;
vluint64_t y2;
vluint64_t z2;
}simdtest[]={
{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADDH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4300430043004300ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUBH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMULH,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4220422042204220ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x4220422042204220ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x4091C7004091C700ULL,
	0x40490FD040490FD0ULL,  0xBFB4FDF4BFB4FDF4ULL, 0x3FDD21003FDD2100ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x3FDD21003FDD2100ULL,
	0x40490FD040490FD0ULL,  0xBFB4FDF4BFB4FDF4ULL, 0x4091C7004091C700ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x408E2580408E2580ULL,
	0x40490FD040490FD0ULL,  0x3FB4FDF43FB4FDF4ULL, 0x408E2580408E2580ULL },



{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x3FDD2100BFDD2100ULL,
	0x40490FD0C0490FD0ULL,  0x3FB4FDF4BFB4FDF4ULL, 0x4091C700C091C700ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x4091C700C091C700ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0xC08E2580C08E2580ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x3E807E003E807E00ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x40490FD040490FD0ULL,  0x40490FD040490FD0ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x40490FD03FB4FDF4ULL,  0x8034FDF40034FDF4ULL, 0x0000000000000000ULL,
	0x3F003F003F003F00ULL,  0x8034FDF40034FDF4ULL, 0x0000000000000000ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADDX,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x3FDD2100BFDD2100ULL,
	0x3F803F003F803F00ULL,  0x3F803F003F803F00ULL, 0x40003F0040003F00ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUBX,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0x4091C700C091C700ULL,
	0x3F003F003F003F00ULL,  0x3F003F003F003F00ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMULX,
	0x40490FD0C0490FD0ULL,  0xBFB4FDF43FB4FDF4ULL, 0xC08E2580C08E2580ULL,
	0x3F803F003F803F00ULL,  0x3F803F003F803F00ULL, 0x3F807E003F807E00ULL },

#if 0
{JX2_UCMD_FPU3, JX2_UCIX_FPU_FADD,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x40153D6000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x40153D6000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FSUB,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x3FEF0A0000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x3FEF0A0000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_FMUL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x401B413000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x401B413000000000ULL },
#endif

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADDXD,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x40153D6000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x40153D6000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUBXD,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x3FEF0A0000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x3FEF0A0000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMULXD,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x401B413000000000ULL,
	0x40091EB851EB851FULL,  0x40015C28F5C28F5CULL, 0x401B413000000000ULL },



{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x4049990040499900ULL,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x4049990040499900ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x4048518040485180ULL,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x4048518040485180ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x3D009D003D009D00ULL,
	0x4048F5C34048F5C3ULL,  0x3C23D70A3C23D70AULL, 0x3D009D003D009D00ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0x447AC880447AC880ULL,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0x447AC880447AC880ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0xC4793700C4793700ULL,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0xC4793700C4793700ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0x45443F8045443F80ULL,
	0x4048F5C34048F5C3ULL,  0x447A0000447A0000ULL, 0x45443F8045443F80ULL },




{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x4048F5804048F580ULL,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x4048F5804048F580ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x4048F5004048F500ULL,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x4048F5004048F500ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x3652B8003652B800ULL,
	0x4048F5C34048F5C3ULL,  0x358637BD358637BDULL, 0x3652B8003652B800ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0x4974240049742400ULL,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0x4974240049742400ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0xC9742380C9742380ULL,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0xC9742380C9742380ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0x4A3FA6004A3FA600ULL,
	0x4048F5C34048F5C3ULL,  0x4974240049742400ULL, 0x4A3FA6004A3FA600ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x4048F5804048F580ULL,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x4048F5804048F580ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x4048F5804048F580ULL,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x4048F5804048F580ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x0000000000000000ULL,
	0x4048F5C34048F5C3ULL,  0x0000000000000000ULL, 0x0000000000000000ULL },


{JX2_UCMD_FPU3, JX2_UCIX_FPU_PADD,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PSUB,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL },

{JX2_UCMD_FPU3, JX2_UCIX_FPU_PMUL,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL,
	0x0000000000000000ULL,  0x0000000000000000ULL, 0x0000000000000000ULL },


// {JX2_UCMD_FPU3, JX2_UCIX_FPU_PADDH,  12345678,    8191,	1507},

{0x00, 0,  0,  0},
};

char *FpuCheckRestultStr(int ucmd, int uidx,
	vluint64_t val, vluint64_t ref)
{
	int clz;

	if(val==ref)
		return("Exact");

	if((uidx>=0x00) && (uidx<=0x02))
	{
		if((val>>32)==(ref>>32))
			return("InexactEq");
		if(((val>>32)+1)==(ref>>32))
			return("InexactGt");
		if(((val>>32)-1)==(ref>>32))
			return("InexactLt");
		return("Fail");
	}

	if((uidx>=0x35) && (uidx<=0x37))
	{
		if((val>>32)==(ref>>32))
			return("InexactEq");
		if(((val>>32)+1)==(ref>>32))
			return("InexactGt");
		if(((val>>32)-1)==(ref>>32))
			return("InexactLt");
		return("Fail");
	}

	if((uidx>=0x05) && (uidx<=0x07))
	{
		if(	((((val>>8)+0)&0xFFFFFF)!=((ref>>8)&0xFFFFFF)) &&
			((((val>>8)+1)&0xFFFFFF)!=((ref>>8)&0xFFFFFF)) &&
			((((val>>8)-1)&0xFFFFFF)!=((ref>>8)&0xFFFFFF)))
		{
			return("Fail");
		}
		if(	((((val>>40)+0)&0xFFFFFF)!=((ref>>40)&0xFFFFFF)) &&
			((((val>>40)+1)&0xFFFFFF)!=((ref>>40)&0xFFFFFF)) &&
			((((val>>40)-1)&0xFFFFFF)!=((ref>>40)&0xFFFFFF)))
		{
			return("Fail");
		}
		return("Inexact");
	}

	return("Inexact");
}

int main(int argc, char **argv, char **env)
{
	double fx, fy, fz, fw;
	float sfxa, sfxb, sfxc, sfxd;
	float sfya, sfyb, sfyc, sfyd;
	float sfza, sfzb, sfzc, sfzd;
	float sfwa, sfwb, sfwc, sfwd;

	vluint64_t tx, ty, tz, tw, txb, tyb, tzb, twb;
	vluint64_t grn, egrn, tta, ttb;
	int64_t dst;
	int srng;
	
	int tst, op, ixt, held;
	int n, isz_x, isz_y, isnan_x, isnan_y;
	
	printf("FPU Test\n");
	
	Verilated::commandArgs(argc, argv);

	fx=3.14; fy=2.73;
	tx=*(vluint64_t *)(&fx);
	ty=*(vluint64_t *)(&fy);
	tst=0;

	srng=13;
	srng=srng*65521+1;
	srng=srng*65521+1;

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

	fx=3.14; fy=2.73;
	tx=*(vluint64_t *)(&fx);
	ty=*(vluint64_t *)(&fy);
	tst=0;
	held=0;
	top->opCmdA=0;
	top->opCmdB=0;
	
	printf("FPU SIMD Tests\n");

	while (!Verilated::gotFinish())
	{
		top->clock = (main_time>>0)&1;

		if(srng)
		{
			srng=srng*65521+1;
			if(!srng)srng++;
		}

		if(top->clock)
		{
			top->eval();
			main_time++;
			continue;
		}
		
		if(top->regOutOK==2)
		{
			top->exHold=1;
			top->eval();
			main_time++;
			continue;
		}

//		if((srng>>16)&1)
		if(0)
		{
			top->exHold=1;
			top->eval();
			main_time++;
			continue;
		}else
		{
			top->exHold=0;
		}

		if(held)
		{
			held--;
			top->eval();
			main_time++;
			continue;
		}

		if((top->regOutOK==1) && top->opCmdA)
		{
//			tz=top->regValRnA;
//			tzb=top->regValRnB;
			tz=top->regValGRnA;
			tzb=top->regValGRnB;

//			tta=tx &0xFFFFFF80FFFFFF80ULL;
//			ttb=txb&0xFFFFFF80FFFFFF80ULL;
			tta=tx;	ttb=txb;
			*(vluint32_t *)(&sfxa)=tta;
			*(vluint32_t *)(&sfxb)=tta>>32;
			*(vluint32_t *)(&sfxc)=ttb;
			*(vluint32_t *)(&sfxd)=ttb>>32;

//			tta=ty &0xFFFFFF80FFFFFF80ULL;
//			ttb=tyb&0xFFFFFF80FFFFFF80ULL;
			tta=ty;	ttb=tyb;
			*(vluint32_t *)(&sfya)=tta;
			*(vluint32_t *)(&sfyb)=tta>>32;
			*(vluint32_t *)(&sfyc)=ttb;
			*(vluint32_t *)(&sfyd)=ttb>>32;

//			tta=tz &0xFFFFFF80FFFFFF80ULL;
//			ttb=tzb&0xFFFFFF80FFFFFF80ULL;
			tta=tz;	ttb=tzb;
			*(vluint32_t *)(&sfza)=tta;
			*(vluint32_t *)(&sfzb)=tta>>32;
			*(vluint32_t *)(&sfzc)=ttb;
			*(vluint32_t *)(&sfzd)=ttb>>32;

			*(vluint64_t *)(&fx)=tx;
			*(vluint64_t *)(&fy)=ty;
			*(vluint64_t *)(&fz)=tz;
			*(vluint64_t *)(&fw)=tw;

			printf(
				"%02X-%02X\n"
				"Rs    =%016llX-%016llX\n"
				"Rt    =%016llX-%016llX\n"
				"Ro    =%016llX-%016llX\n"
				"Expect=%016llX-%016llX %s/%s\n",
				top->opCmdA, top->regIdIxtA,
				tx, txb,
				ty, tyb,
				tz, tzb,
				tw, twb,
//				(tz==tw)?"Pass":"Fail",
//				(tzb==twb)?"Pass":"Fail");
				FpuCheckRestultStr(top->opCmdA, top->regIdIxtA, tz, tw),
				FpuCheckRestultStr(top->opCmdA, top->regIdIxtA, tzb, twb)
				);

			if((ixt&0x30)==0x10)
			{
			}else
				if(((ixt&0x30)==0x30) || ((ixt&0x3C)==0x00))
			{
				printf("S=%f T=%f O=%f E=%f\n",
					fx, fy, fz, fw);
			}else
			{
				printf(	"S=(%f %f %f %f)\n"
						"T=(%f %f %f %f)\n"
						"O=(%f %f %f %f)\n",
					sfxa, sfxb, sfxc, sfxd,
					sfya, sfyb, sfyc, sfyd,
					sfza, sfzb, sfzc, sfzd);
				
				if((ixt&0x0F)==0x05)
				{
					sfwa=sfxa+sfya;
					sfwb=sfxb+sfyb;
					sfwc=sfxc+sfyc;
					sfwd=sfxd+sfyd;
				}

				if((ixt&0x0F)==0x06)
				{
					sfwa=sfxa-sfya;
					sfwb=sfxb-sfyb;
					sfwc=sfxc-sfyc;
					sfwd=sfxd-sfyd;
				}

				if((ixt&0x0F)==0x07)
				{
					sfwa=sfxa*sfya;
					sfwb=sfxb*sfyb;
					sfwc=sfxc*sfyc;
					sfwd=sfxd*sfyd;
				}

//				*(vluint32_t *)(&sfwa)=*(vluint32_t *)(&sfwa)&0xFFFFFF80U;
//				*(vluint32_t *)(&sfwb)=*(vluint32_t *)(&sfwb)&0xFFFFFF80U;
//				*(vluint32_t *)(&sfwc)=*(vluint32_t *)(&sfwc)&0xFFFFFF80U;
//				*(vluint32_t *)(&sfwd)=*(vluint32_t *)(&sfwd)&0xFFFFFF80U;

				*(vluint32_t *)(&sfwa)=*(vluint32_t *)(&sfwa)&0xFFFFFFFFU;
				*(vluint32_t *)(&sfwb)=*(vluint32_t *)(&sfwb)&0xFFFFFFFFU;
				*(vluint32_t *)(&sfwc)=*(vluint32_t *)(&sfwc)&0xFFFFFFFFU;
				*(vluint32_t *)(&sfwd)=*(vluint32_t *)(&sfwd)&0xFFFFFFFFU;

				printf("p=(%f %f %f %f)\n", sfwa, sfwb, sfwc, sfwd);
			}

			printf("\n");

			top->opCmdA=0x00;
			top->regIdIxtA=0;
			tst++;
		}
		
		if(top->regOutOK==0)
		{
			op=simdtest[tst].cmd;
			ixt=simdtest[tst].ixt;

			tx=simdtest[tst].x1;
			ty=simdtest[tst].y1;
			tw=simdtest[tst].z1;

			txb=simdtest[tst].x2;
			tyb=simdtest[tst].y2;
			twb=simdtest[tst].z2;

//			twb=tw;
//			egrn=simdtest[tst].grn;
			
			if(!op)
				break;
		
			top->opCmdA=op;
			top->regIdIxtA=ixt;

			top->regValRsA=tx;
			top->regValRtA=ty;

			top->regValRsB=txb;
			top->regValRtB=tyb;

			held=1;
		}

		top->eval();

		main_time++;
		
//		if(main_time>256)
		if(main_time>65536)
		{
			printf("Cycle Timeout\n");
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
