module MemIcOplen(istr, opLen);
input[15:0]		istr;
output[2:0]		opLen;

reg[2:0]		tOpLen;
assign opLen	= tOpLen;

always @*
begin
	casez( {istr[15:13], istr[11:10]} )
		5'b0zz_zz:	tOpLen[1:0] = 2'b01;
		5'b10z_zz:	tOpLen[1:0] = 2'b01;
		5'b110_zz:	tOpLen[1:0] = 2'b01;
		5'b111_0z:	tOpLen[1:0] = 2'b10;
		5'b111_10:	tOpLen[1:0] = 2'b10;
		5'b111_11:	tOpLen[1:0] = 2'b11;
	endcase
	
	casez( {tOpLen[1], istr[12], istr[11:8]} )
		6'b0z_zzzz:	tOpLen[2]	= 1'b0;

		6'b10_00zz:	tOpLen[2]	= 1'b0;
		6'b10_01zz:	tOpLen[2]	= 1'b0;
		6'b10_100z:	tOpLen[2]	= 1'b0;
		6'b10_101z:	tOpLen[2]	= 1'b1;
		6'b10_11zz:	tOpLen[2]	= 1'b0;

		6'b11_00zz:	tOpLen[2]	= 1'b0;
		6'b11_01zz:	tOpLen[2]	= 1'b1;
		6'b11_1000:	tOpLen[2]	= 1'b0;
		6'b11_1001:	tOpLen[2]	= 1'b1;
		6'b11_101z:	tOpLen[2]	= 1'b0;
		6'b11_110z:	tOpLen[2]	= 1'b0;
		6'b11_111z:	tOpLen[2]	= 1'b1;
	endcase
end

endmodule
