`include "CoreDefs.v"

module ExALW(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	idUCmd,
	idUIxt,
	exHold,
	regInSrST,
	regOutVal,
	regOutSrST
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
input			exHold;
input[1:0]		regInSrST;

output[63:0]	regOutVal;
output[1:0]		regOutSrST;

wire			regInSrT;
wire			regInSrS;
assign		regInSrT = regInSrST[0];
assign		regInSrS = regInSrST[1];

reg[7:0]		idUIxt2;


reg[63:0]	tRegOutVal2;
reg			tRegOutSrT2;
reg			tRegOutSrS2;
assign	regOutVal = tRegOutVal2;
assign	regOutSrST = { tRegOutSrS2, tRegOutSrT2 };

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
reg			tRegOutSrS;

always @*
begin
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		idUIxt2			<= idUIxt;
		tRegOutVal2		<= tRegOutVal;
		tRegOutSrT2		<= tRegOutSrT;
		tRegOutSrS2		<= tRegOutSrS;
	end
end

endmodule
