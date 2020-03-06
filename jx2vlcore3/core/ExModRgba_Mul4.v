module ExModRgba_Mul4(
	valRs,	valRt,	valRn
	);

input[3:0]	valRs;
input[3:0]	valRt;
output[3:0]	valRn;

reg[3:0]	tValRn;

assign	valRn = tValRn;

always @*
begin

`ifndef def_true
	casez( {valRs, valRt} )
		8'h0z: tValRn=4'h0;
		8'hz0: tValRn=4'h0;

		8'h11: tValRn=4'h0;
		8'h12: tValRn=4'h0;
		8'h13: tValRn=4'h0;
		8'h14: tValRn=4'h0;
		8'h15: tValRn=4'h0;
		8'h16: tValRn=4'h0;
		8'h17: tValRn=4'h0;
		8'h18: tValRn=4'h1;
		8'h19: tValRn=4'h1;
		8'h1A: tValRn=4'h1;
		8'h1B: tValRn=4'h1;
		8'h1C: tValRn=4'h1;
		8'h1D: tValRn=4'h1;
		8'h1E: tValRn=4'h1;
		8'h1F: tValRn=4'h1;
		8'h21: tValRn=4'h0;
		8'h22: tValRn=4'h0;
		8'h23: tValRn=4'h0;
		8'h24: tValRn=4'h1;
		8'h25: tValRn=4'h1;
		8'h26: tValRn=4'h1;
		8'h27: tValRn=4'h1;
		8'h28: tValRn=4'h1;
		8'h29: tValRn=4'h1;
		8'h2A: tValRn=4'h1;
		8'h2B: tValRn=4'h1;
		8'h2C: tValRn=4'h2;
		8'h2D: tValRn=4'h2;
		8'h2E: tValRn=4'h2;
		8'h2F: tValRn=4'h2;
		8'h31: tValRn=4'h0;
		8'h32: tValRn=4'h0;
		8'h33: tValRn=4'h1;
		8'h34: tValRn=4'h1;
		8'h35: tValRn=4'h1;
		8'h36: tValRn=4'h1;
		8'h37: tValRn=4'h1;
		8'h38: tValRn=4'h2;
		8'h39: tValRn=4'h2;
		8'h3A: tValRn=4'h2;
		8'h3B: tValRn=4'h2;
		8'h3C: tValRn=4'h2;
		8'h3D: tValRn=4'h2;
		8'h3E: tValRn=4'h3;
		8'h3F: tValRn=4'h3;
		8'h41: tValRn=4'h0;
		8'h42: tValRn=4'h1;
		8'h43: tValRn=4'h1;
		8'h44: tValRn=4'h1;
		8'h45: tValRn=4'h1;
		8'h46: tValRn=4'h2;
		8'h47: tValRn=4'h2;
		8'h48: tValRn=4'h2;
		8'h49: tValRn=4'h2;
		8'h4A: tValRn=4'h3;
		8'h4B: tValRn=4'h3;
		8'h4C: tValRn=4'h3;
		8'h4D: tValRn=4'h3;
		8'h4E: tValRn=4'h4;
		8'h4F: tValRn=4'h4;
		8'h51: tValRn=4'h0;
		8'h52: tValRn=4'h1;
		8'h53: tValRn=4'h1;
		8'h54: tValRn=4'h1;
		8'h55: tValRn=4'h2;
		8'h56: tValRn=4'h2;
		8'h57: tValRn=4'h2;
		8'h58: tValRn=4'h3;
		8'h59: tValRn=4'h3;
		8'h5A: tValRn=4'h3;
		8'h5B: tValRn=4'h3;
		8'h5C: tValRn=4'h4;
		8'h5D: tValRn=4'h4;
		8'h5E: tValRn=4'h4;
		8'h5F: tValRn=4'h5;
		8'h61: tValRn=4'h0;
		8'h62: tValRn=4'h1;
		8'h63: tValRn=4'h1;
		8'h64: tValRn=4'h2;
		8'h65: tValRn=4'h2;
		8'h66: tValRn=4'h2;
		8'h67: tValRn=4'h3;
		8'h68: tValRn=4'h3;
		8'h69: tValRn=4'h3;
		8'h6A: tValRn=4'h4;
		8'h6B: tValRn=4'h4;
		8'h6C: tValRn=4'h5;
		8'h6D: tValRn=4'h5;
		8'h6E: tValRn=4'h5;
		8'h6F: tValRn=4'h6;
		8'h71: tValRn=4'h0;
		8'h72: tValRn=4'h1;
		8'h73: tValRn=4'h1;
		8'h74: tValRn=4'h2;
		8'h75: tValRn=4'h2;
		8'h76: tValRn=4'h3;
		8'h77: tValRn=4'h3;
		8'h78: tValRn=4'h4;
		8'h79: tValRn=4'h4;
		8'h7A: tValRn=4'h4;
		8'h7B: tValRn=4'h5;
		8'h7C: tValRn=4'h5;
		8'h7D: tValRn=4'h6;
		8'h7E: tValRn=4'h6;
		8'h7F: tValRn=4'h7;
		8'h81: tValRn=4'h1;
		8'h82: tValRn=4'h1;
		8'h83: tValRn=4'h2;
		8'h84: tValRn=4'h2;
		8'h85: tValRn=4'h3;
		8'h86: tValRn=4'h3;
		8'h87: tValRn=4'h4;
		8'h88: tValRn=4'h4;
		8'h89: tValRn=4'h5;
		8'h8A: tValRn=4'h5;
		8'h8B: tValRn=4'h6;
		8'h8C: tValRn=4'h6;
		8'h8D: tValRn=4'h7;
		8'h8E: tValRn=4'h7;
		8'h8F: tValRn=4'h8;
		8'h91: tValRn=4'h1;
		8'h92: tValRn=4'h1;
		8'h93: tValRn=4'h2;
		8'h94: tValRn=4'h2;
		8'h95: tValRn=4'h3;
		8'h96: tValRn=4'h3;
		8'h97: tValRn=4'h4;
		8'h98: tValRn=4'h5;
		8'h99: tValRn=4'h5;
		8'h9A: tValRn=4'h6;
		8'h9B: tValRn=4'h6;
		8'h9C: tValRn=4'h7;
		8'h9D: tValRn=4'h7;
		8'h9E: tValRn=4'h8;
		8'h9F: tValRn=4'h9;
		8'hA1: tValRn=4'h1;
		8'hA2: tValRn=4'h1;
		8'hA3: tValRn=4'h2;
		8'hA4: tValRn=4'h3;
		8'hA5: tValRn=4'h3;
		8'hA6: tValRn=4'h4;
		8'hA7: tValRn=4'h4;
		8'hA8: tValRn=4'h5;
		8'hA9: tValRn=4'h6;
		8'hAA: tValRn=4'h6;
		8'hAB: tValRn=4'h7;
		8'hAC: tValRn=4'h8;
		8'hAD: tValRn=4'h8;
		8'hAE: tValRn=4'h9;
		8'hAF: tValRn=4'hA;
		8'hB1: tValRn=4'h1;
		8'hB2: tValRn=4'h1;
		8'hB3: tValRn=4'h2;
		8'hB4: tValRn=4'h3;
		8'hB5: tValRn=4'h3;
		8'hB6: tValRn=4'h4;
		8'hB7: tValRn=4'h5;
		8'hB8: tValRn=4'h6;
		8'hB9: tValRn=4'h6;
		8'hBA: tValRn=4'h7;
		8'hBB: tValRn=4'h8;
		8'hBC: tValRn=4'h8;
		8'hBD: tValRn=4'h9;
		8'hBE: tValRn=4'hA;
		8'hBF: tValRn=4'hB;
		8'hC1: tValRn=4'h1;
		8'hC2: tValRn=4'h2;
		8'hC3: tValRn=4'h2;
		8'hC4: tValRn=4'h3;
		8'hC5: tValRn=4'h4;
		8'hC6: tValRn=4'h5;
		8'hC7: tValRn=4'h5;
		8'hC8: tValRn=4'h6;
		8'hC9: tValRn=4'h7;
		8'hCA: tValRn=4'h8;
		8'hCB: tValRn=4'h8;
		8'hCC: tValRn=4'h9;
		8'hCD: tValRn=4'hA;
		8'hCE: tValRn=4'hB;
		8'hCF: tValRn=4'hC;
		8'hD1: tValRn=4'h1;
		8'hD2: tValRn=4'h2;
		8'hD3: tValRn=4'h2;
		8'hD4: tValRn=4'h3;
		8'hD5: tValRn=4'h4;
		8'hD6: tValRn=4'h5;
		8'hD7: tValRn=4'h6;
		8'hD8: tValRn=4'h7;
		8'hD9: tValRn=4'h7;
		8'hDA: tValRn=4'h8;
		8'hDB: tValRn=4'h9;
		8'hDC: tValRn=4'hA;
		8'hDD: tValRn=4'hB;
		8'hDE: tValRn=4'hB;
		8'hDF: tValRn=4'hD;
		8'hE1: tValRn=4'h1;
		8'hE2: tValRn=4'h2;
		8'hE3: tValRn=4'h3;
		8'hE4: tValRn=4'h4;
		8'hE5: tValRn=4'h4;
		8'hE6: tValRn=4'h5;
		8'hE7: tValRn=4'h6;
		8'hE8: tValRn=4'h7;
		8'hE9: tValRn=4'h8;
		8'hEA: tValRn=4'h9;
		8'hEB: tValRn=4'hA;
		8'hEC: tValRn=4'hB;
		8'hED: tValRn=4'hB;
		8'hEE: tValRn=4'hC;
		8'hEF: tValRn=4'hE;
		8'hF1: tValRn=4'h1;
		8'hF2: tValRn=4'h2;
		8'hF3: tValRn=4'h3;
		8'hF4: tValRn=4'h4;
		8'hF5: tValRn=4'h5;
		8'hF6: tValRn=4'h6;
		8'hF7: tValRn=4'h7;
		8'hF8: tValRn=4'h8;
		8'hF9: tValRn=4'h9;
		8'hFA: tValRn=4'hA;
		8'hFB: tValRn=4'hB;
		8'hFC: tValRn=4'hC;
		8'hFD: tValRn=4'hD;
		8'hFE: tValRn=4'hE;
		8'hFF: tValRn=4'hF;
	endcase
`endif

`ifndef def_true
	casez( {valRs, valRt} )
		8'b0000_zzzz: tValRn=4'b0000;
		8'b0001_zzzz: tValRn=4'b0000;

		8'b0010_0zzz: tValRn=4'b0000;
		8'b0010_1zzz: tValRn=4'b0001;

		8'b0011_00zz: tValRn=4'b0000;
		8'b0011_01zz: tValRn=4'b0001;
		8'b0011_10zz: tValRn=4'b0001;
		8'b0011_11zz: tValRn=4'b0010;

		8'b0100_00zz: tValRn=4'b0000;
		8'b0100_01zz: tValRn=4'b0001;
		8'b0100_10zz: tValRn=4'b0010;
		8'b0100_11zz: tValRn=4'b0011;

		8'b0101_000z: tValRn=4'b0000;
		8'b0101_001z: tValRn=4'b0000;
		8'b0101_010z: tValRn=4'b0001;
		8'b0101_011z: tValRn=4'b0001;
		8'b0101_100z: tValRn=4'b0010;
		8'b0101_101z: tValRn=4'b0010;
		8'b0101_110z: tValRn=4'b0011;
		8'b0101_111z: tValRn=4'b0011;
	endcase
`endif

`ifndef def_true
	case(valRs)
		4'h0: tValRn=4'b0000;
		4'h1: tValRn=4'b0000;
		4'h2: tValRn={ 3'b000, valRt[3] };
		4'h3:
			casez(valRt)
				4'b00zz: tValRn=4'b0000;
				4'b010z: tValRn=4'b0000;
				4'b011z: tValRn=4'b0001;
				4'b100z: tValRn=4'b0001;
				4'b101z: tValRn=4'b0010;
				4'b110z: tValRn=4'b0010;
				4'b1110: tValRn=4'b0010;
				4'b1111: tValRn=4'b0011;
			endcase
		4'h4: tValRn={ 2'b00, valRt[3:2] };

		4'h5:
			casez(valRt)
				4'b000z: tValRn=4'b0000;
				4'b001z: tValRn=4'b0000;
				4'b010z: tValRn=4'b0001;
				4'b011z: tValRn=4'b0010;
				4'b100z: tValRn=4'b0011;
				4'b101z: tValRn=4'b0011;
				4'b110z: tValRn=4'b0100;
				4'b1110: tValRn=4'b0100;
				4'b1111: tValRn=4'b0101;
			endcase
		4'h6:
			casez(valRt)
				4'b000z: tValRn=4'b0000;
				4'b001z: tValRn=4'b0000;
				4'b010z: tValRn=4'b0001;
				4'b011z: tValRn=4'b0010;
				4'b100z: tValRn=4'b0011;
				4'b101z: tValRn=4'b0011;
				4'b110z: tValRn=4'b0100;
				4'b1110: tValRn=4'b0101;
				4'b1111: tValRn=4'b0110;
			endcase

		4'h8: tValRn={ 1'b0, valRt[3:1] };

		4'hF: tValRn=valRt;
	endcase
`endif

end

endmodule
