/*
Single Precision / SIMD-Vector FPU
Aim for 3 cycle latency.

Will support several formats:
  Binary64: Internally Truncated to Single Precision
  2x Binary32
  4x Binary16

 */

module ExFpuSv(
	/* verilator lint_off UNUSED */
	clock,		reset,		exHold,
	opCmd,		regIdIxt,
	regValRs,
	regValRt,
	regValRn
	);

input	clock;
input	reset;
input	exHold;

input[8:0]		opCmd;			//command opcode
input[8:0]		regIdIxt;		//ALU Index / Opcode Extension

input[63:0]		regValRs;
input[63:0]		regValRt;
output[63:0]	regValRn;

endmodule
