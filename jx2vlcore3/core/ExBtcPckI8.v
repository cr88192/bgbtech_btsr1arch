`ifndef HAS_EXBTCPCKI8
`define HAS_EXBTCPCKI8

`include "ExBtcPckI8W.v"

module ExBtcPckI8(
	clock,	reset,
	regValRs,
	idUIxt,
	exHold,
	regOutVal
	);

input			clock;
input			reset;
input			exHold;

input[63:0]		regValRs;
input[8:0]		idUIxt;

output[31:0]	regOutVal;

reg[63:0]		tRegValRs;

// wire[63:0]		tRegValRs;
// assign		tRegValRs = regValRs;

reg[31:0]	tRegOutVal;
reg[31:0]	tRegOutVal2;
assign		regOutVal = tRegOutVal;
// assign		regOutVal = tRegOutVal2;


wire[7:0]		tRegPck1a;
wire[7:0]		tRegPck1b;
wire[7:0]		tRegPck1c;
wire[7:0]		tRegPck1d;
ExBtcPckI8W		pck1a(tRegValRs[15: 0], tRegPck1a);
ExBtcPckI8W		pck1b(tRegValRs[31:16], tRegPck1b);
ExBtcPckI8W		pck1c(tRegValRs[47:32], tRegPck1c);
ExBtcPckI8W		pck1d(tRegValRs[63:48], tRegPck1d);

reg[31:0]	tRegPck;

always @*
begin
	tRegPck = { tRegPck1d, tRegPck1c, tRegPck1b, tRegPck1a };
end

always @(posedge clock)
begin
	tRegValRs	<= regValRs;
	tRegOutVal	<= tRegPck;
	tRegOutVal2	<= tRegOutVal;
end

endmodule

`endif
