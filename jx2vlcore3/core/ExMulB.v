`include "ExCsAdd64F.v"

module ExMulB(
	clock,		reset,
	valRs,		valRt,
	idUCmd,		idUIxt,
	exHold,		valRn
	);

input	clock;
input	reset;

input[31:0]		valRs;
input[31:0]		valRt;
output[63:0]	valRn;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
input			exHold;

reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[7:0]		tIdUCmd;
reg[7:0]		tIdUIxt;

reg[31:0]		tValRs;
reg[31:0]		tValRt;

reg[31:0]		tValRsSx;
reg[31:0]		tValRtSx;

reg[31:0]	tMul1AA;
reg[31:0]	tMul1AB;
reg[31:0]	tMul1BA;
reg[31:0]	tMul1BB;
// reg[31:0]	tMul1AC;
// reg[31:0]	tMul1CA;
reg[31:0]	tMul1C;
// reg[31:0]	ttMul1C;

reg[15:0]	tMul2A;
reg[15:0]	tMul2B;
reg[15:0]	tMul2C;
reg[15:0]	tMul2D;

reg[63:0]	tMul2WA;
reg[63:0]	tMul2WB;
wire[63:0]	tMul2WC;
ExCsAdd64F	mulAdd2(tMul2WA, tMul2WB, tMul2WC);


reg[15:0]	tMul2E;
reg[15:0]	tMul2F;
reg[15:0]	tMul2G;

reg[63:0]	tMul3A;
reg[63:0]	tMul3B;
// reg[63:0]	tMul3C;
wire[63:0]	tMul3C;
ExCsAdd64F	mulAdd3(tMul3A, tMul3B, tMul3C);

reg[63:0]	tMul4;

always @*
begin
//	ttMul1C = UV32_00;

//	tValRsSx	= tValRs[31] ? UV32_FF : UV32_00;
//	tValRtSx	= tValRt[31] ? UV32_FF : UV32_00;

	tValRsSx	= (tValRs[31] && !tIdUIxt[0]) ? UV32_FF : UV32_00;
	tValRtSx	= (tValRt[31] && !tIdUIxt[0]) ? UV32_FF : UV32_00;

//	tMul2WA = { tMul1BB[31:0], tMul1AA[31:0] };
//	tMul2WB = { UV16_00, tMul1AB[31:0], UV16_00 };
	tMul2WA = { UV16_00, tMul1AB[31:0], UV16_00 };
	tMul2WB = { UV16_00, tMul1BA[31:0], UV16_00 };

	tMul3A = { tMul2D, tMul2C, tMul2B, tMul2A  };
	tMul3B = { tMul2G, tMul2F, tMul2E, UV16_00 };
//	tMul3C = tMul3A + tMul3B;
end

always @(posedge clock)
begin
//	tValRs	<= valRs;
//	tValRt	<= valRt;

	if(!exHold)
	begin
		tIdUCmd	<= idUCmd;
		tIdUIxt	<= idUIxt;
		tValRs	<= valRs;
		tValRt	<= valRt;
	end

	/* Stage 1 */
	tMul1AA <= { UV16_00, tValRs[15: 0] } * { UV16_00, tValRt[15: 0] };
//	tMul1AB <= { UV16_00, tValRs[15: 0] } * { UV16_00, tValRt[31:16] };
//	tMul1BA <= { UV16_00, tValRs[31:16] } * { UV16_00, tValRt[15: 0] };
//	tMul1BB <= { UV16_00, tValRs[31:16] } * { UV16_00, tValRt[31:16] };

	tMul1AB <=
		{ UV16_00       , tValRs[15: 0] } *
		{ tValRtSx[15:0], tValRt[31:16] };
	tMul1BA <=
		{ tValRsSx[15:0], tValRs[31:16] } *
		{ UV16_00       , tValRt[15: 0] };
	tMul1BB <=
		{ tValRsSx[15:0], tValRs[31:16] } *
		{ tValRtSx[15:0], tValRt[31:16] };

//	tMul1AC <= valRt[31] ? (-valRs[31:0]) : UV32_00;
//	tMul1CA <= valRs[31] ? (-valRt[31:0]) : UV32_00;
//	tMul1C	<= ttMul1C;
	
	/* Stage 2 */
//	tMul2A	<= tMul1AA[15:0];
//	tMul2B	<= tMul1AA[31:16] + tMul1AB[15:0];
//	tMul2C	<= tMul1AB[31:16] + tMul1BB[15:0];
//	tMul2D	<= tMul1BB[31:16];

//	tMul2A	<= tMul2WC[15: 0];
//	tMul2B	<= tMul2WC[31:16];
//	tMul2C	<= tMul2WC[47:32];
//	tMul2D	<= tMul2WC[63:48];

	tMul2A	<= tMul1AA[15: 0];
	tMul2B	<= tMul1AA[31:16];
	tMul2C	<= tMul1BB[15: 0];
	tMul2D	<= tMul1BB[31:16];

//	tMul2E	<= tMul1BA[15: 0];
//	tMul2F	<= tMul1BA[31:16];
//	tMul2G	<= UV16_00;

	tMul2E	<= tMul2WC[31:16];
	tMul2F	<= tMul2WC[47:32];
//	tMul2G	<= tMul2WC[63:48];
	tMul2G	<= UV16_00;
	
	/* Stage 3 */
	tValRn	<= tMul3C;
//	tMul4	<= tMul3C;
//	tValRn	<= tMul4;

`ifndef def_true
	if(tIdUCmd[5:0]==JX2_UCMD_MUL3)
	begin
		$display("MUL: Rs=%X Rt=%x", tValRs, tValRt);
		$display("MUL: Mul1AA=%X Mul1AB=%x Mul1BA=%x Mul1BB=%x",
			tMul1AA, tMul1AB, tMul1BA, tMul1BB);
	end
`endif

end

endmodule
