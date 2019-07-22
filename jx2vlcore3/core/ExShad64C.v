/*
64-bit SHAD/SHLD Unit
Extend to also support 32-bit SHAD/SHLD.
 */

`ifndef HAS_JX2EXSHADQ_C
`define HAS_JX2EXSHADQ_C

module ExShad64C(
	/* verilator lint_off UNUSED */
	clock, reset,
	valRs, valRt,
	valRn, shOpA
	);

input	clock;
input	reset;

// /* verilator lint_off UNOPTFLAT */

input[63:0]		valRs;
input[ 7:0]		valRt;
input[ 1:0]		shOpA;
output[63:0]	valRn;

wire			shOp;
wire			shOpQ;
assign		shOp = shOpA[0];
assign		shOpQ = shOpA[1];

reg[63:0]		tValRn;
assign			valRn = tValRn;
// /* verilator lint_on UNOPTFLAT */

reg[63:0]		tValRs;

reg[63:0]		tValRol;
reg[63:0]		tValRor;
reg[ 7:0]		tValSh;
reg[ 7:0]		tValShN;

reg[63:0]		tValSht32;
reg[63:0]		tValSht16;
reg[63:0]		tValSht8;
reg[63:0]		tValSht4;
reg[63:0]		tValSht2;
reg[63:0]		tValSht1;

always @*
begin

	tValRol=0;
	tValRor=0;
	tValRn = 0;
//	tValSh = 0;
	tValSh = valRt;
	tValShN	= ~valRt;

	if(shOpQ)
	begin
		if(shOp)
			tValRor=0;
		else
			tValRor=valRs[63] ? UV64_FF : UV64_00;
		tValRs = valRs;
	end
	else
	begin
		if(shOp)
			tValRor=0;
		else
			tValRor=valRs[31] ? UV64_FF : UV64_00;
		tValRs = { tValRor[31:0], valRs[31:0] };
		tValSh[5]=0;
		tValShN[5]=0;
	end

	if(tValSh[7])
	begin
		tValSht32	= tValShN[5] ?
			{ tValRor[31:0], tValRs   [63:32] } : tValRs   ;
		tValSht16	= tValShN[4] ?
			{ tValRor[15:0], tValSht32[63:16] } : tValSht32;
		tValSht8	= tValShN[3] ?
			{ tValRor[ 7:0], tValSht16[63: 8] } : tValSht16;
		tValSht4	= tValShN[2] ?
			{ tValRor[ 3:0], tValSht8 [63: 4] } : tValSht8 ;
		tValSht2	= tValShN[1] ?
			{ tValRor[ 1:0], tValSht4 [63: 2] } : tValSht4 ;
		tValSht1	= tValShN[0] ?
			{ tValRor[   0], tValSht2 [63: 1] } : tValSht2 ;
		tValRn		= { tValRor[0], tValSht2 [63: 1] };
	end
	else
	begin
		tValSht32	= tValSh[5] ? { tValRs   [31:0], UV32_00 } : tValRs   ;
		tValSht16	= tValSh[4] ? { tValSht32[47:0], UV16_00 } : tValSht32;
		tValSht8	= tValSh[3] ? { tValSht16[55:0], UV8_00  } : tValSht16;
		tValSht4	= tValSh[2] ? { tValSht8 [59:0], UV4_00  } : tValSht8 ;
		tValSht2	= tValSh[1] ? { tValSht4 [61:0], UV2_00  } : tValSht4 ;
		tValSht1	= tValSh[0] ? { tValSht2 [62:0], UV1_00  } : tValSht2 ;
		tValRn		= tValSht1;
	end

end

endmodule

`endif
