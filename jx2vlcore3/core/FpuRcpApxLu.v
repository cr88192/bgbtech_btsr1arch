/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

module FpuRcpApxLu(valRm, valRn);
input[63:0]		valRm;
output[63:0]	valRn;

reg[63:0]	tValRn;
assign	valRn = tValRn;

reg			tSga;
reg[10:0]	tExa;
reg[51:0]	tFra;
reg			tFraZ;

reg[11:0]	tBitsA;
reg[11:0]	tBitsB;
reg[11:0]	tBitsC;

reg[17:0]	tBitsB2;
reg[23:0]	tBitsA2;

reg[10:0]	tExc;
reg[51:0]	tFrc;

always @*
begin
	tSga = valRm[63];
	tExa = valRm[62:52];
	tFra = valRm[51: 0];

	tFraZ =
		(tFra[51:46] == 0) &&
		(tFra[45:40] == 0) &&
		(tFra[39:34] == 0) ;

	case(tFra[51:46])
		6'h00: tBitsA=12'hFDF;
		6'h01: tBitsA=12'hFA1;
		6'h02: tBitsA=12'hF65;
		6'h03: tBitsA=12'hF2B;
		6'h04: tBitsA=12'hEF2;
		6'h05: tBitsA=12'hEBB;
		6'h06: tBitsA=12'hE85;
		6'h07: tBitsA=12'hE51;
		6'h08: tBitsA=12'hE1F;
		6'h09: tBitsA=12'hDEE;
		6'h0A: tBitsA=12'hDBE;
		6'h0B: tBitsA=12'hD8F;
		6'h0C: tBitsA=12'hD62;
		6'h0D: tBitsA=12'hD36;
		6'h0E: tBitsA=12'hD0A;
		6'h0F: tBitsA=12'hCE0;
		6'h10: tBitsA=12'hCB7;
		6'h11: tBitsA=12'hC90;
		6'h12: tBitsA=12'hC69;
		6'h13: tBitsA=12'hC42;
		6'h14: tBitsA=12'hC1D;
		6'h15: tBitsA=12'hBF9;
		6'h16: tBitsA=12'hBD6;
		6'h17: tBitsA=12'hBB3;
		6'h18: tBitsA=12'hB91;
		6'h19: tBitsA=12'hB70;
		6'h1A: tBitsA=12'hB50;
		6'h1B: tBitsA=12'hB30;
		6'h1C: tBitsA=12'hB11;
		6'h1D: tBitsA=12'hAF3;
		6'h1E: tBitsA=12'hAD5;
		6'h1F: tBitsA=12'hAB8;
		6'h20: tBitsA=12'hA9C;
		6'h21: tBitsA=12'hA80;
		6'h22: tBitsA=12'hA64;
		6'h23: tBitsA=12'hA4A;
		6'h24: tBitsA=12'hA2F;
		6'h25: tBitsA=12'hA16;
		6'h26: tBitsA=12'h9FD;
		6'h27: tBitsA=12'h9E4;
		6'h28: tBitsA=12'h9CC;
		6'h29: tBitsA=12'h9B4;
		6'h2A: tBitsA=12'h99C;
		6'h2B: tBitsA=12'h986;
		6'h2C: tBitsA=12'h96F;
		6'h2D: tBitsA=12'h959;
		6'h2E: tBitsA=12'h943;
		6'h2F: tBitsA=12'h92E;
		6'h30: tBitsA=12'h919;
		6'h31: tBitsA=12'h905;
		6'h32: tBitsA=12'h8F0;
		6'h33: tBitsA=12'h8DD;
		6'h34: tBitsA=12'h8C9;
		6'h35: tBitsA=12'h8B6;
		6'h36: tBitsA=12'h8A3;
		6'h37: tBitsA=12'h891;
		6'h38: tBitsA=12'h87E;
		6'h39: tBitsA=12'h86D;
		6'h3A: tBitsA=12'h85B;
		6'h3B: tBitsA=12'h84A;
		6'h3C: tBitsA=12'h839;
		6'h3D: tBitsA=12'h828;
		6'h3E: tBitsA=12'h817;
		6'h3F: tBitsA=12'h807;
	endcase
	case(tFra[45:40])
		6'h00: tBitsB=12'h432;
		6'h01: tBitsB=12'h411;
		6'h02: tBitsB=12'h3F1;
		6'h03: tBitsB=12'h3D1;
		6'h04: tBitsB=12'h3B0;
		6'h05: tBitsB=12'h390;
		6'h06: tBitsB=12'h370;
		6'h07: tBitsB=12'h34F;
		6'h08: tBitsB=12'h32F;
		6'h09: tBitsB=12'h30F;
		6'h0A: tBitsB=12'h2EF;
		6'h0B: tBitsB=12'h2CE;
		6'h0C: tBitsB=12'h2AE;
		6'h0D: tBitsB=12'h28E;
		6'h0E: tBitsB=12'h26E;
		6'h0F: tBitsB=12'h24D;
		6'h10: tBitsB=12'h22D;
		6'h11: tBitsB=12'h20D;
		6'h12: tBitsB=12'h1ED;
		6'h13: tBitsB=12'h1CD;
		6'h14: tBitsB=12'h1AD;
		6'h15: tBitsB=12'h18C;
		6'h16: tBitsB=12'h16C;
		6'h17: tBitsB=12'h14C;
		6'h18: tBitsB=12'h12C;
		6'h19: tBitsB=12'h10C;
		6'h1A: tBitsB=12'h0EC;
		6'h1B: tBitsB=12'h0CC;
		6'h1C: tBitsB=12'h0AC;
		6'h1D: tBitsB=12'h08C;
		6'h1E: tBitsB=12'h06C;
		6'h1F: tBitsB=12'h04C;
		6'h20: tBitsB=12'h02C;
		6'h21: tBitsB=12'h00C;
		6'h22: tBitsB=12'hFEC;
		6'h23: tBitsB=12'hFCC;
		6'h24: tBitsB=12'hFAC;
		6'h25: tBitsB=12'hF8C;
		6'h26: tBitsB=12'hF6C;
		6'h27: tBitsB=12'hF4C;
		6'h28: tBitsB=12'hF2C;
		6'h29: tBitsB=12'hF0D;
		6'h2A: tBitsB=12'hEED;
		6'h2B: tBitsB=12'hECD;
		6'h2C: tBitsB=12'hEAD;
		6'h2D: tBitsB=12'hE8D;
		6'h2E: tBitsB=12'hE6D;
		6'h2F: tBitsB=12'hE4D;
		6'h30: tBitsB=12'hE2E;
		6'h31: tBitsB=12'hE0E;
		6'h32: tBitsB=12'hDEE;
		6'h33: tBitsB=12'hDCE;
		6'h34: tBitsB=12'hDAF;
		6'h35: tBitsB=12'hD8F;
		6'h36: tBitsB=12'hD6F;
		6'h37: tBitsB=12'hD4F;
		6'h38: tBitsB=12'hD30;
		6'h39: tBitsB=12'hD10;
		6'h3A: tBitsB=12'hCF0;
		6'h3B: tBitsB=12'hCD1;
		6'h3C: tBitsB=12'hCB1;
		6'h3D: tBitsB=12'hC91;
		6'h3E: tBitsB=12'hC72;
		6'h3F: tBitsB=12'hC52;
	endcase
	case(tFra[39:34])
		6'h00: tBitsC=12'h40A;
		6'h01: tBitsC=12'h3EA;
		6'h02: tBitsC=12'h3CA;
		6'h03: tBitsC=12'h3AA;
		6'h04: tBitsC=12'h38A;
		6'h05: tBitsC=12'h36A;
		6'h06: tBitsC=12'h34A;
		6'h07: tBitsC=12'h32A;
		6'h08: tBitsC=12'h30A;
		6'h09: tBitsC=12'h2EA;
		6'h0A: tBitsC=12'h2CA;
		6'h0B: tBitsC=12'h2AA;
		6'h0C: tBitsC=12'h28A;
		6'h0D: tBitsC=12'h26A;
		6'h0E: tBitsC=12'h24A;
		6'h0F: tBitsC=12'h22A;
		6'h10: tBitsC=12'h20A;
		6'h11: tBitsC=12'h1EA;
		6'h12: tBitsC=12'h1CA;
		6'h13: tBitsC=12'h1AA;
		6'h14: tBitsC=12'h18A;
		6'h15: tBitsC=12'h16A;
		6'h16: tBitsC=12'h14A;
		6'h17: tBitsC=12'h12A;
		6'h18: tBitsC=12'h10A;
		6'h19: tBitsC=12'h0EA;
		6'h1A: tBitsC=12'h0CA;
		6'h1B: tBitsC=12'h0AA;
		6'h1C: tBitsC=12'h08A;
		6'h1D: tBitsC=12'h06A;
		6'h1E: tBitsC=12'h04A;
		6'h1F: tBitsC=12'h02A;
		6'h20: tBitsC=12'h00A;
		6'h21: tBitsC=12'hFEB;
		6'h22: tBitsC=12'hFCB;
		6'h23: tBitsC=12'hFAB;
		6'h24: tBitsC=12'hF8B;
		6'h25: tBitsC=12'hF6B;
		6'h26: tBitsC=12'hF4B;
		6'h27: tBitsC=12'hF2B;
		6'h28: tBitsC=12'hF0B;
		6'h29: tBitsC=12'hEEB;
		6'h2A: tBitsC=12'hECB;
		6'h2B: tBitsC=12'hEAB;
		6'h2C: tBitsC=12'hE8B;
		6'h2D: tBitsC=12'hE6B;
		6'h2E: tBitsC=12'hE4B;
		6'h2F: tBitsC=12'hE2B;
		6'h30: tBitsC=12'hE0B;
		6'h31: tBitsC=12'hDEB;
		6'h32: tBitsC=12'hDCB;
		6'h33: tBitsC=12'hDAB;
		6'h34: tBitsC=12'hD8B;
		6'h35: tBitsC=12'hD6B;
		6'h36: tBitsC=12'hD4B;
		6'h37: tBitsC=12'hD2B;
		6'h38: tBitsC=12'hD0B;
		6'h39: tBitsC=12'hCEB;
		6'h3A: tBitsC=12'hCCB;
		6'h3B: tBitsC=12'hCAB;
		6'h3C: tBitsC=12'hC8B;
		6'h3D: tBitsC=12'hC6B;
		6'h3E: tBitsC=12'hC4B;
		6'h3F: tBitsC=12'hC2B;
	endcase

	tBitsB2 = { tBitsB,  6'h00  } + { tBitsC[11] ? 6'h3F : 6'h00, tBitsC };
	tBitsA2 = { tBitsA, 12'h000 } + { tBitsB2[17] ? 6'h3F : 6'h00, tBitsB2 };
	
//	tExc = 11'h7FE - tExa;
	tExc = 11'h7FD - tExa;
//	tFrc = { tBitsA[4:0], tBitsB, tBitsC, 35'h0 };
	tFrc = { tBitsA2[22:0], 29'h0 };

	if(tFraZ)
	begin
		tExc = 11'h7FE - tExa;
		tFrc = 0;
	end

	tValRn = { tSga, tExc, tFrc };

end

endmodule
