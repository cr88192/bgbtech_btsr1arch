/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

`ifndef HAS_EXMULC
`define HAS_EXMULC

`include "ExCsAdd64F.v"

`ifdef jx2_enable_btcuab1
`include "ExBtcUab1.v"
`endif

module ExMulC(
	clock,		reset,
	valRs,		valRt,		valRm,
	idUCmd,		idUIxt,
	exHold,		valRn
	);

input	clock;
input	reset;

input[63:0]		valRs;
input[63:0]		valRt;
input[63:0]		valRm;
output[63:0]	valRn;
input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			exHold;

reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[8:0]		tIdUCmd;
reg[8:0]		tIdUIxt;

reg[8:0]		tIdUCmd1;
reg[8:0]		tIdUIxt1;
reg[8:0]		tIdUCmd2;
reg[8:0]		tIdUIxt2;
reg[8:0]		tIdUCmd3;
reg[8:0]		tIdUIxt3;

reg[31:0]		tValRs;
reg[31:0]		tValRt;
reg[63:0]		tValRm;

`ifdef jx2_enable_btcuab1
wire[63:0]		tValUab1;
ExBtcUab1	uab1(
	clock, reset,
	valRs, valRt[3:0],
	idUIxt,
	tValUab1);
`endif

// reg[31:0]		tValRsSx;
// reg[31:0]		tValRtSx;
reg[15:0]		tValRsSx;
reg[15:0]		tValRtSx;

reg[63:0]	tMac1;
reg[31:0]	tMul1AA;
reg[31:0]	tMul1AB;
reg[31:0]	tMul1BA;
reg[31:0]	tMul1BB;
// reg[31:0]	tMul1AC;
// reg[31:0]	tMul1CA;
reg[31:0]	tMul1C;
reg[31:0]	ttMul1C;

reg[63:0]	tMac2;
reg[15:0]	tMul2A;
reg[15:0]	tMul2B;
reg[15:0]	tMul2C;
reg[15:0]	tMul2D;

reg[63:0]	tMul2WA;
reg[63:0]	tMul2WB;
// reg[63:0]	tMul2WC;
wire[63:0]	tMul2WC;
// ExCsAdd64F	mulAdd2(tMul2WA, tMul2WB, tMul2WC);

`ifdef jx2_alu_dmac
wire[63:0]	tMul2WCa;
ExCsAdd64F	mulAdd2a(tMul2WA, tMul2WB, tMul2WCa);
ExCsAdd64F	mulAdd2b(tMul2WCa, tMac2, tMul2WC);
`else
ExCsAdd64F	mulAdd2(tMul2WA, tMul2WB, tMul2WC);
`endif

reg[15:0]	tMul2E;
reg[15:0]	tMul2F;
reg[15:0]	tMul2G;
reg[15:0]	tMul2H;

reg[63:0]	tMac3;
reg[63:0]	tMul3A;
reg[63:0]	tMul3B;
reg[63:0]	tMul3C;
wire[63:0]	tMul3D;
wire[63:0]	tMul3C_A;

ExCsAdd64F	mulAdd3(tMul3A, tMul3B, tMul3C_A);

// ExCsAdd64F	mulAdd3a(tMul3A, tMul3B, tMul3D);
// ExCsAdd64F	mulAdd3b(tMul3D, tMac3, tMul3C_A);

reg[63:0]	tMul4;

always @*
begin
	tIdUCmd	= idUCmd;
	tIdUIxt	= idUIxt;
	tValRs	= valRs[31:0];
	tValRt	= valRt[31:0];

//	tValRm	= idUIxt[4] ? valRm[63:0] : UV64_00;
	tValRm	= (idUIxt[4] && (idUCmd[5:0]==JX2_UCMD_MUL3)) ?
		valRm[63:0] : UV64_00;

	if(idUIxt[0])
		ttMul1C = UV32_00;
	else
		ttMul1C =
			(tValRt[31]?(-tValRs[31:0]):UV32_00) +
			(tValRs[31]?(-tValRt[31:0]):UV32_00);

//	tValRsSx	= tValRs[31] ? UV32_FF : UV32_00;
//	tValRtSx	= tValRt[31] ? UV32_FF : UV32_00;

	tValRsSx	= (tValRs[31] && !tIdUIxt[0]) ? UV16_FF : UV16_00;
	tValRtSx	= (tValRt[31] && !tIdUIxt[0]) ? UV16_FF : UV16_00;

	tMul2WA = {
		(tMul1AB[31] && !tIdUIxt1[0]) ?
			UV16_FF : UV16_00, 
		tMul1AB[31:0], UV16_00 };
	tMul2WB = {
		(tMul1BA[31] && !tIdUIxt1[0]) ?
			UV16_FF : UV16_00, 
		tMul1BA[31:0], UV16_00 };

	tMul3A = { tMul2D, tMul2C, tMul2B, tMul2A  };
//	tMul3B = { tMul2G, tMul2F, tMul2E, UV16_00 };
	tMul3B = { tMul2G, tMul2F, tMul2E, tMul2H  };

	tMul3C = tMul3C_A;
	if(!tIdUIxt2[2])
		tMul3C[63:32] = (tMul3C_A[31] && !tIdUIxt2[0]) ?
			UV32_FF : UV32_00;

//	if(tIdUIxt2[4])
	if(tIdUIxt2[5])
	begin
`ifdef jx2_enable_btcuab1
		tMul3C = tValUab1;
`endif
	end

	tValRn	= tMul3C;
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		/* Stage 1 */
		tIdUCmd1	<= tIdUCmd;
		tIdUIxt1	<= tIdUIxt;
		tMul1AA <= { UV16_00, tValRs[15: 0] } * { UV16_00, tValRt[15: 0] };
		tMac1		<= tValRm;

`ifdef def_true
		tMul1AB <=
			{ UV16_00 , tValRs[15: 0] } *
			{ tValRtSx, tValRt[31:16] };
		tMul1BA <=
			{ tValRsSx, tValRs[31:16] } *
			{ UV16_00 , tValRt[15: 0] };
		tMul1BB <=
			{ tValRsSx, tValRs[31:16] } *
			{ tValRtSx, tValRt[31:16] };
`endif

		tMul1C	<= ttMul1C;
		
		/* Stage 2 */
		tIdUCmd2	<= tIdUCmd1;
		tIdUIxt2	<= tIdUIxt1;
		tMac2		<= tMac1;
//		tMac2		<= tIdUIxt1[4] ? tMac1 : UV64_00;

`ifndef def_true
		tMul2A	<= tMul1AA[15:0];
		tMul2B	<= tMul1AA[31:16] + tMul1AB[15:0];
		tMul2C	<= tMul1AB[31:16] + tMul1BB[15:0];
		tMul2D	<= tMul1BB[31:16];

		tMul2E	<= tMul1BA[15: 0];
		tMul2F	<= tMul1BA[31:16] + tMul1C[15:0];
		tMul2G	<= tMul1C [31:16];
`endif

`ifdef def_true
		tMul2A	<= tMul1AA[15: 0];
		tMul2B	<= tMul1AA[31:16];
		tMul2C	<= tMul1BB[15: 0];
		tMul2D	<= tMul1BB[31:16];

		tMul2E	<= tMul2WC[31:16];
		tMul2F	<= tMul2WC[47:32];
		tMul2G	<= tMul2WC[63:48];
		tMul2H	<= tMul2WC[15: 0];
`endif

		/* Stage 3 */
		tIdUCmd3	<= tIdUCmd2;
		tIdUIxt3	<= tIdUIxt2;
//		tValRn		<= tMul3C;
		tMac3		<= tMac2;

`ifndef def_true
		if(tIdUCmd[5:0]==JX2_UCMD_MUL3)
		begin
			$display("MUL: Rs=%X Rt=%x", tValRs, tValRt);
			$display("MUL: Mul1AA=%X Mul1AB=%x Mul1BA=%x Mul1BB=%x",
				tMul1AA, tMul1AB, tMul1BA, tMul1BB);
		end
`endif
	end


end

endmodule

`endif
