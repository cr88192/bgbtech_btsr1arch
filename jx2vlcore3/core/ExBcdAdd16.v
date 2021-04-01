`include "ExBcdAdd4.v"

module ExBcdAdd16(a, b, c, c_in, c_out);
input [15:0]		a;
input [15:0]		b;
output[15:0]		c;
input			c_in;
output			c_out;

wire[16:0]	tCa;

reg[15:0]	tC;
reg			tCo;
assign		c = tC;
assign		c_out = tCo;

wire	tCarry1;
wire	tCarry2;
wire	tCarry3;
wire	tCarry4;

ExBcdAdd4	add1(a[ 3: 0], b[ 3: 0], tCa[ 3: 0], c_in, tCarry1);
ExBcdAdd4	add2(a[ 7: 4], b[ 7: 4], tCa[ 7: 4], tCarry1, tCarry2);
ExBcdAdd4	add3(a[11: 8], b[11: 8], tCa[11: 8], tCarry2, tCarry3);
ExBcdAdd4	add4(a[15:12], b[15:12], tCa[15:12], tCarry3, tCarry4);

always @*
begin
	tC	= tCa[15:0];
	tCo = tCarry4;
end

endmodule
