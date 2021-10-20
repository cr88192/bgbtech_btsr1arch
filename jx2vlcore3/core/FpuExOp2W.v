/*
SMT FPU Module
 */

`include "FpuExOpW.v"

module FpuExOp2W(
	/* verilator lint_off UNUSED */
	clock,		reset,

	rg1OpCmdA,	rg1IdIxtA,
	rg1OpCmdB,	rg1IdIxtB,
	rg1ValImmA,	rg1ValImmB,

	rg2OpCmdA,	rg2IdIxtA,
	rg2OpCmdB,	rg2IdIxtB,
	rg2ValImmA,	rg2ValImmB,

	rg1IdRsA,	rg1ValRsA,
	rg1IdRtA,	rg1ValRtA,
	rg1IdRnA,	rg1ValRnA,

	rg2IdRsA,	rg2ValRsA,
	rg2IdRtA,	rg2ValRtA,
	rg2IdRnA,	rg2ValRnA,

	rg1IdRsB,	rg1ValRsB,
	rg1IdRtB,	rg1ValRtB,
	rg1IdRnB,	rg1ValRnB,

	rg2IdRsB,	rg2ValRsB,
	rg2IdRtB,	rg2ValRtB,
	rg2IdRnB,	rg2ValRnB,

	rg1OutOK,	rg1OutSrT,
	rg1InFpsr,	rg1OutFpsr,

	rg2OutOK,	rg2OutSrT,
	rg2InFpsr,	rg2OutFpsr,

	rg1InSr,	rg1BraFlush,
	rg2InSr,	rg2BraFlush,
	rg1ExHold,	rg2ExHold,

	rg1ValGRnA,	rg1ValGRnB,
	rg2ValGRnA,	rg2ValGRnB
	);

input			clock;
input			reset;

input[8:0]		rg1OpCmdA;
input[8:0]		rg1OpCmdB;
input[8:0]		rg1IdIxtA;
input[8:0]		rg1IdIxtB;
input[32:0]		rg1ValImmA;
input[32:0]		rg1ValImmB;

input[8:0]		rg2OpCmdA;
input[8:0]		rg2OpCmdB;
input[8:0]		rg2IdIxtA;
input[8:0]		rg2IdIxtB;
input[32:0]		rg2ValImmA;
input[32:0]		rg2ValImmB;

`input_gpr		rg1IdRsA;
`input_gpr		rg1IdRtA;
`input_gpr		rg1IdRnA;
`input_gpr		rg1IdRsB;
`input_gpr		rg1IdRtB;
`input_gpr		rg1IdRnB;

`input_gpr		rg2IdRsA;
`input_gpr		rg2IdRtA;
`input_gpr		rg2IdRnA;
`input_gpr		rg2IdRsB;
`input_gpr		rg2IdRtB;
`input_gpr		rg2IdRnB;

input[63:0]		rg1ValRsA;		//Rs input value (Lane 1)
input[63:0]		rg1ValRtA;		//Rt input value (Lane 1)
input[63:0]		rg1ValRnA;		//Rn input value (Lane 1)
input[63:0]		rg1ValRsB;		//Rs input value (Lane 2)
input[63:0]		rg1ValRtB;		//Rt input value (Lane 2)
input[63:0]		rg1ValRnB;		//Rn input value (Lane 2)

input[63:0]		rg2ValRsA;		//Rs input value (Lane 1)
input[63:0]		rg2ValRtA;		//Rt input value (Lane 1)
input[63:0]		rg2ValRnA;		//Rn input value (Lane 1)
input[63:0]		rg2ValRsB;		//Rs input value (Lane 2)
input[63:0]		rg2ValRtB;		//Rt input value (Lane 2)
input[63:0]		rg2ValRnB;		//Rn input value (Lane 2)

output[63:0]	rg1ValGRnA;		//Rn output value (Lane 1)
output[63:0]	rg1ValGRnB;		//Rn output value (Lane 2)
output			rg1OutSrT;
output[1:0]		rg1OutOK;		//execute status

output[63:0]	rg2ValGRnA;		//Rn output value (Lane 1)
output[63:0]	rg2ValGRnB;		//Rn output value (Lane 2)
output			rg2OutSrT;
output[1:0]		rg2OutOK;		//execute status

input[63:0]		rg1InSr;		//input SR
input			rg1BraFlush;
input			rg1ExHold;

input[63:0]		rg2InSr;		//input SR
input			rg2BraFlush;
input			rg2ExHold;

input[15:0]		rg1InFpsr;
output[15:0]	rg1OutFpsr;

input[15:0]		rg2InFpsr;
output[15:0]	rg2OutFpsr;


FpuExOpW	rg1Fpu(
	clock,			reset,

	rg1OpUCmdA,		rg1OpUIxtA,
	rg1OpUCmdB,		rg1OpUIxtB,
	rg1RegValImmA,	rg1RegValImmB,

	rg1RegIdRs,		rg1RegValRs,
	rg1RegIdRt,		rg1RegValRt,
	rg1RegIdRm,		rg1RegValRm,

	rg1RegIdRs,		rg1RegValRs,
	rg1RegIdRt,		rg1RegValRt,
	rg1RegIdRm,		rg1RegValRm,

	rg1FpuOK,		rg1FpuSrT,
	rg1OutFpsr,		rg1InFpsr,
	
	rg1InSr,
	rg1BraFlush,
	rg1ExHold,
	
	rg1FpuValGRnA,	rg1FpuValGRnB
	);


FpuExOpW	rg2Fpu(
	clock,			reset,

	rg2OpUCmdA,		rg2OpUIxtA,
	rg2OpUCmdB,		rg2OpUIxtB,
	rg2RegValImmA,	rg2RegValImmB,

	rg2RegIdRs,		rg2RegValRs,
	rg2RegIdRt,		rg2RegValRt,
	rg2RegIdRm,		rg2RegValRm,

	rg2RegIdRs,		rg2RegValRs,
	rg2RegIdRt,		rg2RegValRt,
	rg2RegIdRm,		rg2RegValRm,

	rg2FpuOK,		rg2FpuSrT,
	rg2OutFpsr,		rg2InFpsr,
	
	rg2InSr,
	rg2BraFlush,
	rg2ExHold,
	
	rg2FpuValGRnA,	rg2FpuValGRnB
	);

endmodule
