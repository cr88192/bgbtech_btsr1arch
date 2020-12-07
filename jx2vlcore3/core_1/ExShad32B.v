/*
32-bit SHAD/SHLD Unit
 */

`ifndef HAS_JX2EXSHAD_B
`define HAS_JX2EXSHAD_B

module ExShad32B(
	/* verilator lint_off UNUSED */
	clock, reset,
	valRs, valRt,
	valRn, shOp
	);

input	clock;
input	reset;

// /* verilator lint_off UNOPTFLAT */

input[31:0]		valRs;
input[ 7:0]		valRt;
input			shOp;
output[31:0]	valRn;

reg[31:0]		tValRn;
assign			valRn = tValRn;
// /* verilator lint_on UNOPTFLAT */

reg[31:0]		tValRol;
reg[31:0]		tValRor;
reg[ 7:0]		tValSh;
reg[ 7:0]		tValShN;

reg[31:0]		tValSht16;
reg[31:0]		tValSht8;
reg[31:0]		tValSht4;
reg[31:0]		tValSht2;
reg[31:0]		tValSht1;

always @*
begin

	tValRol=0;
	tValRor=0;
	tValRn = 0;
//	tValSh = 0;
	tValSh = valRt;
	tValShN	= ~valRt;

	if(shOp)
		tValRor=0;
	else
		tValRor=valRs[31] ? UV32_FF : UV32_00;

	if(tValSh[7])
	begin
		tValSht16	= tValShN[4] ?
			{ tValRor[15:0], valRs    [31:16] } : valRs    ;
		tValSht8	= tValShN[3] ?
			{ tValRor[ 7:0], tValSht16[31: 8] } : tValSht16;
		tValSht4	= tValShN[2] ?
			{ tValRor[ 3:0], tValSht8 [31: 4] } : tValSht8 ;
		tValSht2	= tValShN[1] ?
			{ tValRor[ 1:0], tValSht4 [31: 2] } : tValSht4 ;
		tValSht1	= tValShN[0] ?
			{ tValRor[   0], tValSht2 [31: 1] } : tValSht2 ;
		tValRn		= { tValRor[0], tValSht1 [31: 1] };
	end
	else
	begin
		tValSht16	= tValSh[4] ? { valRs    [15:0], UV16_00 } : valRs    ;
		tValSht8	= tValSh[3] ? { tValSht16[23:0], UV8_00  } : tValSht16;
		tValSht4	= tValSh[2] ? { tValSht8 [27:0], UV4_00  } : tValSht8 ;
		tValSht2	= tValSh[1] ? { tValSht4 [29:0], UV2_00  } : tValSht4 ;
		tValSht1	= tValSh[0] ? { tValSht2 [30:0], UV1_00  } : tValSht2 ;
		tValRn		= tValSht1;
	end

end

endmodule

`endif
