module ExBcdAdd4(a, b, c, c_in, c_out);
input [3:0]		a;
input [3:0]		b;
output[3:0]		c;
input			c_in;
output			c_out;

reg[4:0]	tCa;
reg[4:0]	tCb;

reg[3:0]	tC;
reg			tCo;
assign		c = tC;
assign		c_out = tCo;

always @*
begin
	tCa	= { 1'b0, a } + { 1'b0, b } + {3'b0, c_in};
	tCb	= tCa + ((tCa>9)?6:0);
	tC	= tCb[3:0];
	tCo	= tCb[4];
end

endmodule
