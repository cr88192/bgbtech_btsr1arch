module ExBnn_BitNeur_Mul(valV, valW, valO);
input[1:0]	valV;
input[2:0]	valW;
output[3:0]	valO;

reg[3:0]	tValO;
assign	valO = tValO;

always @*
begin
	casez({valV, valW})
		5'b00_zzz: tValO=4'b0000;
		5'b10_zzz: tValO=4'b0000;
		5'bzz_000: tValO=4'b0000;
		5'bzz_800: tValO=4'b0000;

		5'b01_001: tValO=4'b0001;
		5'b01_010: tValO=4'b0010;
		5'b01_011: tValO=4'b0011;

		5'b11_101: tValO=4'b0001;
		5'b11_110: tValO=4'b0010;
		5'b11_111: tValO=4'b0011;

		5'b01_101: tValO=4'b1111;
		5'b01_110: tValO=4'b1110;
		5'b01_111: tValO=4'b1101;

		5'b11_001: tValO=4'b1111;
		5'b11_010: tValO=4'b1110;
		5'b11_011: tValO=4'b1101;
	endcase
end

endmodule