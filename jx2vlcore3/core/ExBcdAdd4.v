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
	tCa	= { 1'b0, a } + { 1'b0, b } + { 4'b0, c_in };

	case(tCa)
		5'h00: tCb = 5'h00;
		5'h01: tCb = 5'h01;
		5'h02: tCb = 5'h02;
		5'h03: tCb = 5'h03;
		5'h04: tCb = 5'h04;
		5'h05: tCb = 5'h05;
		5'h06: tCb = 5'h06;
		5'h07: tCb = 5'h07;
		5'h08: tCb = 5'h08;
		5'h09: tCb = 5'h09;
		5'h0A: tCb = 5'h10;
		5'h0B: tCb = 5'h11;
		5'h0C: tCb = 5'h12;
		5'h0D: tCb = 5'h13;
		5'h0E: tCb = 5'h14;
		5'h0F: tCb = 5'h15;
		5'h10: tCb = 5'h16;
		5'h11: tCb = 5'h17;
		5'h12: tCb = 5'h18;
		5'h13: tCb = 5'h19;

		default:
		begin
			/* Out of range, but can't occur with in-range inputs. */
			tCb = 5'h00;
		end
	endcase

//	tCb	= tCa + ((tCa>9)?6:0);
	tC	= tCb[3:0];
	tCo	= tCb[4];
end

endmodule
