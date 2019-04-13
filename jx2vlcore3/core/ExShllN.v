`include "CoreDefs.v"


`ifndef HAS_JX2EXOPSHLLN
`define HAS_JX2EXOPSHLLN

module ExShllN(
	regIdIxt,
	regValRm,	regOutVal,
	regInSrT,	regOutSrT);

input [ 7:0]	regIdIxt;
input [63:0]	regValRm;
output[63:0]	regOutVal;

input			regInSrT;
output			regOutSrT;

reg[63:0]		tRegOutVal;
assign		regOutVal = tRegOutVal;

reg			tRegOutSrT;
assign	regOutSrT = tRegOutSrT;

reg[63:0]		tRegSx;

always @*
begin
	tRegOutVal	= regValRm;
	tRegOutSrT	= regInSrT;

	tRegSx		= regValRm[63] ? UV64_FF : UV64_00;

	case(regIdIxt[4:0])
		5'h00: tRegOutVal = { regValRm[62:0], 1'b0 };
		5'h01: tRegOutVal = { regValRm[61:0], 2'b0 };
		5'h02: tRegOutVal = { regValRm[59:0], 4'b0 };
		5'h03: tRegOutVal = { regValRm[55:0], 8'b0 };
		5'h04: tRegOutVal = { 1'b0, regValRm[63:1] };
		5'h05: tRegOutVal = { 2'b0, regValRm[63:2] };
		5'h06: tRegOutVal = { 4'b0, regValRm[63:4] };
		5'h07: tRegOutVal = { 8'b0, regValRm[63:8] };
		5'h08: tRegOutVal = { tRegSx[  0], regValRm[63:1] };
		5'h09: tRegOutVal = { tRegSx[1:0], regValRm[63:2] };
		5'h0A: tRegOutVal = { tRegSx[3:0], regValRm[63:4] };
		5'h0B: tRegOutVal = { tRegSx[7:0], regValRm[63:8] };
		5'h0C: tRegOutVal = { regValRm[47:0], 16'b0 };
		5'h0D: tRegOutVal = { 16'b0, regValRm[63:16] };
		5'h0E: tRegOutVal = { tRegSx[15:0], regValRm[63:16] };
		5'h0F: tRegOutVal = tRegSx;
		5'h10: tRegOutVal = { regValRm[31:0], 32'b0 };
		5'h11: tRegOutVal = { 32'b0, regValRm[63:32] };
		5'h12: tRegOutVal = { tRegSx[31:0], regValRm[63:32] };

		5'h14: tRegOutVal	= { regValRm[62:0], regValRm[63] };
		5'h15: tRegOutVal = { regValRm[0], regValRm[63:1] };

		5'h16: begin
			tRegOutSrT	= regValRm[63];
			tRegOutVal	= { regValRm[62:0], regInSrT };
		end
		5'h17: begin
			tRegOutSrT	= regValRm[0];
			tRegOutVal = { regInSrT, regValRm[63:1] };
		end

		5'h18: begin
			tRegOutSrT	= regValRm[63];
			tRegOutVal	= { regValRm[62:0], 1'b0 };
		end
		5'h19: begin
			tRegOutSrT	= regValRm[0];
			tRegOutVal = { 1'b0, regValRm[63:1] };
		end
		5'h1A: begin
			tRegOutSrT	= regValRm[0];
			tRegOutVal = { regValRm[63], regValRm[63:1] };
		end

		default: tRegOutVal = UV64_XX;
	endcase
end

endmodule

`endif
