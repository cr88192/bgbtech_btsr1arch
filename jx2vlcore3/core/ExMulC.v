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

`ifdef jx2_enable_rgb5btcenccc
`include "ExBtcEncCc.v"
`endif

module ExMulC(
	clock,		reset,
	valRs,		valRt,		valRm,
	idUCmd,		idUIxt,
	exHold,		valRn,
	ex1MulFaz,	ex3MulFaz
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

output			ex1MulFaz;
output			ex3MulFaz;


(* max_fanout = 200 *)
	wire			exHoldN;

assign	exHoldN = !exHold;


reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[2:0]		tEx1MulFaz;
reg[2:0]		tEx2MulFaz;
reg[2:0]		tEx3MulFaz;
assign		ex1MulFaz = (tEx1MulFaz!=0);
assign		ex3MulFaz = (tEx3MulFaz!=0);

reg[8:0]		tIdUCmd;
reg[8:0]		tIdUIxt;

reg[8:0]		tIdUCmd1;
reg[8:0]		tIdUIxt1;
reg[8:0]		tIdUCmd2;
reg[8:0]		tIdUIxt2;
reg[8:0]		tIdUCmd3;
reg[8:0]		tIdUIxt3;

(* max_fanout = 200 *)
	reg[31:0]		tValRs;
(* max_fanout = 200 *)
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

`ifdef jx2_enable_rgb5btcenccc
wire[31:0]		tValEncCc1;
ExBtcEncCc		encCc1(
	clock, reset,
	valRs,
	valRt[31:0],
	valRm[31:0],
	idUIxt,
	exHold,
	tValEncCc1);
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
reg[4:0]	tFazRcpIx;
reg[31:0]	tFazRcp;

always @*
begin
	tIdUCmd	= idUCmd;
	tIdUIxt	= idUIxt;
	tValRs	= valRs[31:0];
	tValRt	= valRt[31:0];
	tEx1MulFaz	= 0;
//	tEx3MulFaz	= 0;

`ifdef jx2_alu_dmac
//	tValRm	= idUIxt[4] ? valRm[63:0] : UV64_00;
	tValRm	= (idUIxt[4] && (idUCmd[5:0]==JX2_UCMD_MUL3)) ?
		valRm[63:0] : UV64_00;
`else
	tValRm	= 0;
`endif

`ifdef jx2_alu_slomuldiv
`ifdef jx2_mul_fazdiv
	if(idUCmd[5:0]==JX2_UCMD_QMULDIV)
	begin
		/* Check if integer divide can be turned into multiply by recip. */
		if(	(idUIxt[5:0]==JX2_UCIX_QMUL_DIVSL) ||
			(idUIxt[5:0]==JX2_UCIX_QMUL_DIVUL))
		begin
//			if(valRt[31:5]==UV27_00)
			if(valRt[31:6]==UV26_00)
			begin
				tEx1MulFaz	= 1;

`ifdef def_true
				case(valRt[5:0])
					6'h00: begin tFazRcpIx = 5'h00; tEx1MulFaz = 0; end
					6'h01: begin tFazRcpIx = 5'h00; tEx1MulFaz = 0; end
					6'h02: begin tFazRcpIx = 5'h00; tEx1MulFaz = 1; end
					6'h03: begin tFazRcpIx = 5'h01; tEx1MulFaz = 2; end
					6'h04: begin tFazRcpIx = 5'h00; tEx1MulFaz = 2; end
					6'h05: begin tFazRcpIx = 5'h02; tEx1MulFaz = 3; end
					6'h06: begin tFazRcpIx = 5'h01; tEx1MulFaz = 3; end
					6'h07: begin tFazRcpIx = 5'h03; tEx1MulFaz = 3; end
					6'h08: begin tFazRcpIx = 5'h00; tEx1MulFaz = 3; end
					6'h09: begin tFazRcpIx = 5'h04; tEx1MulFaz = 4; end
					6'h0A: begin tFazRcpIx = 5'h02; tEx1MulFaz = 4; end
					6'h0B: begin tFazRcpIx = 5'h05; tEx1MulFaz = 4; end
					6'h0C: begin tFazRcpIx = 5'h01; tEx1MulFaz = 4; end
					6'h0D: begin tFazRcpIx = 5'h06; tEx1MulFaz = 4; end
					6'h0E: begin tFazRcpIx = 5'h03; tEx1MulFaz = 4; end
					6'h0F: begin tFazRcpIx = 5'h07; tEx1MulFaz = 4; end
					6'h10: begin tFazRcpIx = 5'h00; tEx1MulFaz = 4; end
					6'h11: begin tFazRcpIx = 5'h08; tEx1MulFaz = 5; end
					6'h12: begin tFazRcpIx = 5'h04; tEx1MulFaz = 5; end
					6'h13: begin tFazRcpIx = 5'h09; tEx1MulFaz = 5; end
					6'h14: begin tFazRcpIx = 5'h02; tEx1MulFaz = 5; end
					6'h15: begin tFazRcpIx = 5'h0A; tEx1MulFaz = 5; end
					6'h16: begin tFazRcpIx = 5'h05; tEx1MulFaz = 5; end
					6'h17: begin tFazRcpIx = 5'h0B; tEx1MulFaz = 5; end
					6'h18: begin tFazRcpIx = 5'h01; tEx1MulFaz = 5; end
					6'h19: begin tFazRcpIx = 5'h0C; tEx1MulFaz = 5; end
					6'h1A: begin tFazRcpIx = 5'h06; tEx1MulFaz = 5; end
					6'h1B: begin tFazRcpIx = 5'h0D; tEx1MulFaz = 5; end
					6'h1C: begin tFazRcpIx = 5'h03; tEx1MulFaz = 5; end
					6'h1D: begin tFazRcpIx = 5'h0E; tEx1MulFaz = 5; end
					6'h1E: begin tFazRcpIx = 5'h07; tEx1MulFaz = 5; end
					6'h1F: begin tFazRcpIx = 5'h0F; tEx1MulFaz = 5; end
					6'h20: begin tFazRcpIx = 5'h00; tEx1MulFaz = 5; end
					6'h21: begin tFazRcpIx = 5'h10; tEx1MulFaz = 6; end
					6'h22: begin tFazRcpIx = 5'h08; tEx1MulFaz = 6; end
					6'h23: begin tFazRcpIx = 5'h11; tEx1MulFaz = 6; end
					6'h24: begin tFazRcpIx = 5'h04; tEx1MulFaz = 6; end
					6'h25: begin tFazRcpIx = 5'h12; tEx1MulFaz = 6; end
					6'h26: begin tFazRcpIx = 5'h09; tEx1MulFaz = 6; end
					6'h27: begin tFazRcpIx = 5'h13; tEx1MulFaz = 6; end
					6'h28: begin tFazRcpIx = 5'h02; tEx1MulFaz = 6; end
					6'h29: begin tFazRcpIx = 5'h14; tEx1MulFaz = 6; end
					6'h2A: begin tFazRcpIx = 5'h0A; tEx1MulFaz = 6; end
					6'h2B: begin tFazRcpIx = 5'h15; tEx1MulFaz = 6; end
					6'h2C: begin tFazRcpIx = 5'h05; tEx1MulFaz = 6; end
					6'h2D: begin tFazRcpIx = 5'h16; tEx1MulFaz = 6; end
					6'h2E: begin tFazRcpIx = 5'h0B; tEx1MulFaz = 6; end
					6'h2F: begin tFazRcpIx = 5'h17; tEx1MulFaz = 6; end
					6'h30: begin tFazRcpIx = 5'h01; tEx1MulFaz = 6; end
					6'h31: begin tFazRcpIx = 5'h18; tEx1MulFaz = 6; end
					6'h32: begin tFazRcpIx = 5'h0C; tEx1MulFaz = 6; end
					6'h33: begin tFazRcpIx = 5'h19; tEx1MulFaz = 6; end
					6'h34: begin tFazRcpIx = 5'h06; tEx1MulFaz = 6; end
					6'h35: begin tFazRcpIx = 5'h1A; tEx1MulFaz = 6; end
					6'h36: begin tFazRcpIx = 5'h0D; tEx1MulFaz = 6; end
					6'h37: begin tFazRcpIx = 5'h1B; tEx1MulFaz = 6; end
					6'h38: begin tFazRcpIx = 5'h03; tEx1MulFaz = 6; end
					6'h39: begin tFazRcpIx = 5'h1C; tEx1MulFaz = 6; end
					6'h3A: begin tFazRcpIx = 5'h0E; tEx1MulFaz = 6; end
					6'h3B: begin tFazRcpIx = 5'h1D; tEx1MulFaz = 6; end
					6'h3C: begin tFazRcpIx = 5'h07; tEx1MulFaz = 6; end
					6'h3D: begin tFazRcpIx = 5'h1E; tEx1MulFaz = 6; end
					6'h3E: begin tFazRcpIx = 5'h0F; tEx1MulFaz = 6; end
					6'h3F: begin tFazRcpIx = 5'h1F; tEx1MulFaz = 6; end
					default: 	tEx1MulFaz	= 0;
				endcase

				case(tFazRcpIx)
					5'h00: tFazRcp = 32'h40000000;
					5'h01: tFazRcp = 32'h55555556;
					5'h02: tFazRcp = 32'h66666667;
					5'h03: tFazRcp = 32'h4924924A;
					5'h04: tFazRcp = 32'h71C71C72;
					5'h05: tFazRcp = 32'h5D1745D2;
					5'h06: tFazRcp = 32'h4EC4EC4F;
					5'h07: tFazRcp = 32'h44444445;
					5'h08: tFazRcp = 32'h78787879;
					5'h09: tFazRcp = 32'h6BCA1AF3;
					5'h0A: tFazRcp = 32'h61861862;
					5'h0B: tFazRcp = 32'h590B2165;
					5'h0C: tFazRcp = 32'h51EB851F;
					5'h0D: tFazRcp = 32'h4BDA12F7;
					5'h0E: tFazRcp = 32'h469EE585;
					5'h0F: tFazRcp = 32'h42108422;
					5'h10: tFazRcp = 32'h7C1F07C2;
					5'h11: tFazRcp = 32'h75075076;
					5'h12: tFazRcp = 32'h6EB3E454;
					5'h13: tFazRcp = 32'h6906906A;
					5'h14: tFazRcp = 32'h63E7063F;
					5'h15: tFazRcp = 32'h5F417D06;
					5'h16: tFazRcp = 32'h5B05B05C;
					5'h17: tFazRcp = 32'h572620AF;
					5'h18: tFazRcp = 32'h5397829D;
					5'h19: tFazRcp = 32'h50505051;
					5'h1A: tFazRcp = 32'h4D4873ED;
					5'h1B: tFazRcp = 32'h4A7904A8;
					5'h1C: tFazRcp = 32'h47DC11F8;
					5'h1D: tFazRcp = 32'h456C797E;
					5'h1E: tFazRcp = 32'h4325C53F;
					5'h1F: tFazRcp = 32'h41041042;
				endcase
`endif
				
				if(valRs[31:30]!=0)
					tEx1MulFaz = 0;
				
				if(tEx1MulFaz!=0)
				begin
					$display("ExMulC: FAZ %X/%X R=%X",
						valRs, valRt, tFazRcp);
					tValRt = tFazRcp;
				end
			end
		end
	end
`endif
`endif

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

`ifdef jx2_mul_fazdiv
	if(tEx3MulFaz!=0)
	begin
		tMul3C[63:32] = (tMul3C_A[63] && !tIdUIxt2[0]) ?
			UV32_FF : UV32_00;

		case(tEx3MulFaz)
			3'b000: tMul3C[31:0] = tMul3C_A[62:31];
			3'b001: tMul3C[31:0] = tMul3C_A[62:31];
			3'b010: tMul3C[31:0] = tMul3C_A[63:32];
			3'b011: tMul3C[31:0] = { tMul3C[   63], tMul3C_A[63:33] };
			3'b100: tMul3C[31:0] = { tMul3C[63:62], tMul3C_A[63:34] };
			3'b101: tMul3C[31:0] = { tMul3C[63:61], tMul3C_A[63:35] };
			3'b110: tMul3C[31:0] = { tMul3C[63:60], tMul3C_A[63:36] };
			3'b111: tMul3C[31:0] = { tMul3C[63:59], tMul3C_A[63:37] };
		endcase

		$display("ExMulC: Res=%X",
			tMul3C);
	end
`endif

//	if(tIdUIxt2[4])
	if(tIdUIxt2[5])
	begin
		case(tIdUIxt2[5:0])
`ifdef jx2_enable_btcuab1
			JX2_UCIX_MUL3_BLKUAB1, JX2_UCIX_MUL3_BLKUAB2: begin
				tMul3C = tValUab1;
			end
`endif

`ifdef jx2_enable_rgb5btcenccc
			JX2_UCIX_MUL3_ENCCC1,
			JX2_UCIX_MUL3_ENCCC2,
			JX2_UCIX_MUL3_RGB5MINMAX:
			begin
				tMul3C = { UV32_00, tValEncCc1 };
			end
`endif

			default:
			begin
				tMul3C = UV64_XX;
			end
		endcase
	end

	tValRn	= tMul3C;
end

always @(posedge clock)
begin
	if(exHoldN)
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
		tEx2MulFaz	<= tEx1MulFaz;

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
		tEx3MulFaz	<= tEx2MulFaz;

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
