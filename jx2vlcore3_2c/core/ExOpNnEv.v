/*
Simple Perceptron

Rs: Input State
Rt: Weight Vector
 */

module ExOpNnEv(
	clock,	reset,
	idOpUCmd,	idOpUIxt,
	idValRs,	idValRt,
	idValRn);

input			clock;
input			reset;
input[7:0]		idOpUCmd;
input[7:0]		idOpUIxt;
input[63:0]		idValRs;
input[63:0]		idValRt;
output[63:0]	idValRn;

reg[11:0]		arrUf6V[63:0];

initial
begin

	arrUf6V[ 0]	= 12'h000;	arrUf6V[ 1]	= 12'h001;
	arrUf6V[ 2]	= 12'h002;	arrUf6V[ 3]	= 12'h003;
	arrUf6V[ 4]	= 12'h004;	arrUf6V[ 5]	= 12'h005;
	arrUf6V[ 6]	= 12'h006;	arrUf6V[ 7]	= 12'h007;
	arrUf6V[ 8]	= 12'h008;	arrUf6V[ 9]	= 12'h00A;
	arrUf6V[10]	= 12'h00C;	arrUf6V[11]	= 12'h00E;
	arrUf6V[12]	= 12'h010;	arrUf6V[13]	= 12'h014;
	arrUf6V[14]	= 12'h018;	arrUf6V[15]	= 12'h01C;
	arrUf6V[16]	= 12'h020;	arrUf6V[17]	= 12'h028;
	arrUf6V[18]	= 12'h030;	arrUf6V[19]	= 12'h038;
	arrUf6V[20]	= 12'h040;	arrUf6V[21]	= 12'h050;
	arrUf6V[22]	= 12'h060;	arrUf6V[23]	= 12'h070;
	arrUf6V[24]	= 12'h080;	arrUf6V[25]	= 12'h0A0;
	arrUf6V[26]	= 12'h0C0;	arrUf6V[27]	= 12'h0E0;
	arrUf6V[28]	= 12'h100;	arrUf6V[29]	= 12'h140;
	arrUf6V[30]	= 12'h180;	arrUf6V[31]	= 12'h1C0;

	arrUf6V[32]	= 12'h000;	arrUf6V[33]	= 12'hFFF;
	arrUf6V[34]	= 12'hFFE;	arrUf6V[35]	= 12'hFFD;
	arrUf6V[36]	= 12'hFFC;	arrUf6V[37]	= 12'hFFB;
	arrUf6V[38]	= 12'hFFA;	arrUf6V[39]	= 12'hFF9;
	arrUf6V[40]	= 12'hFF8;	arrUf6V[41]	= 12'hFF6;
	arrUf6V[42]	= 12'hFF4;	arrUf6V[43]	= 12'hFF2;
	arrUf6V[44]	= 12'hFF0;	arrUf6V[45]	= 12'hFEC;
	arrUf6V[46]	= 12'hFE8;	arrUf6V[47]	= 12'hFE4;
	arrUf6V[48]	= 12'hFE0;	arrUf6V[49]	= 12'hFD8;
	arrUf6V[50]	= 12'hFD0;	arrUf6V[51]	= 12'hFC8;
	arrUf6V[52]	= 12'hFC0;	arrUf6V[53]	= 12'hFB0;
	arrUf6V[54]	= 12'hFA0;	arrUf6V[55]	= 12'hF90;
	arrUf6V[56]	= 12'hF80;	arrUf6V[57]	= 12'hF60;
	arrUf6V[58]	= 12'hF40;	arrUf6V[59]	= 12'hF20;
	arrUf6V[60]	= 12'hF00;	arrUf6V[61]	= 12'hEC0;
	arrUf6V[62]	= 12'hE80;	arrUf6V[63]	= 12'hE40;

end


always @*
begin
end

endmodule
