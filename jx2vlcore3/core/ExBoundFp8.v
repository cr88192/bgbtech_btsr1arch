/*
Check Index against an 8-bit Bound.
The Bound is given as E5.F3.
 */

module ExBoundFp8(regRi, tag, opUIxt, boundFail);
input[63:0]		regRi;
input[15:0]		tag;
input[7:0]		opUIxt;
output			boundFail;

reg			tBoundFail;
assign	boundFail = tBoundFail;

wire		isBndCmp;
wire		isBndChk;
assign	isBndCmp = (opUIxt[5:2] == 4'b0111);
assign	isBndChk = (opUIxt[5:2] == 4'b1000);

reg[33:0]	tRegRiShr;

reg			tBoundFail0;
reg			tBoundFail1;
reg			tBoundFail2;
reg			tBoundFail3;

reg			tBoundNz0;
reg			tBoundNz1;
reg			tBoundNz2;
reg			tBoundNz3;

reg			tBoundFailL10;
reg			tBoundFailP8;

reg[11:0]	tBoundLo;
reg[11:0]	tBoundMi;

reg[11:0]	tBoundBiasLo;
reg[11:0]	tBoundBiasMi;

reg[11:0]	tBoundRef0;
reg[11:0]	tBoundRef1;
reg[11:0]	tBoundRef2;
reg[11:0]	tBoundRef3;

reg			tBoundRef0C;
reg			tBoundRef1C;
reg			tBoundRef2C;
reg			tBoundRef3C;

always @*
begin
	case(tag[5:0])
		6'h00: begin	tBoundLo = 12'h000;		tBoundMi = 12'h010;		end
		6'h01: begin	tBoundLo = 12'h001;		tBoundMi = 12'h012;		end
		6'h02: begin	tBoundLo = 12'h002;		tBoundMi = 12'h014;		end
		6'h03: begin	tBoundLo = 12'h003;		tBoundMi = 12'h016;		end
		6'h04: begin	tBoundLo = 12'h004;		tBoundMi = 12'h018;		end
		6'h05: begin	tBoundLo = 12'h005;		tBoundMi = 12'h01A;		end
		6'h06: begin	tBoundLo = 12'h006;		tBoundMi = 12'h01C;		end
		6'h07: begin	tBoundLo = 12'h007;		tBoundMi = 12'h01E;		end
		6'h08: begin	tBoundLo = 12'h008;		tBoundMi = 12'h020;		end
		6'h09: begin	tBoundLo = 12'h009;		tBoundMi = 12'h024;		end
		6'h0A: begin	tBoundLo = 12'h00A;		tBoundMi = 12'h028;		end
		6'h0B: begin	tBoundLo = 12'h00B;		tBoundMi = 12'h02C;		end
		6'h0C: begin	tBoundLo = 12'h00C;		tBoundMi = 12'h030;		end
		6'h0D: begin	tBoundLo = 12'h00D;		tBoundMi = 12'h034;		end
		6'h0E: begin	tBoundLo = 12'h00E;		tBoundMi = 12'h038;		end
		6'h0F: begin	tBoundLo = 12'h00F;		tBoundMi = 12'h03C;		end

		6'h10: begin	tBoundLo = 12'h010;		tBoundMi = 12'h040;		end
		6'h11: begin	tBoundLo = 12'h012;		tBoundMi = 12'h048;		end
		6'h12: begin	tBoundLo = 12'h014;		tBoundMi = 12'h050;		end
		6'h13: begin	tBoundLo = 12'h016;		tBoundMi = 12'h058;		end
		6'h14: begin	tBoundLo = 12'h018;		tBoundMi = 12'h060;		end
		6'h15: begin	tBoundLo = 12'h01A;		tBoundMi = 12'h068;		end
		6'h16: begin	tBoundLo = 12'h01C;		tBoundMi = 12'h070;		end
		6'h17: begin	tBoundLo = 12'h01E;		tBoundMi = 12'h078;		end
		6'h18: begin	tBoundLo = 12'h020;		tBoundMi = 12'h080;		end
		6'h19: begin	tBoundLo = 12'h024;		tBoundMi = 12'h090;		end
		6'h1A: begin	tBoundLo = 12'h028;		tBoundMi = 12'h0A0;		end
		6'h1B: begin	tBoundLo = 12'h02C;		tBoundMi = 12'h0B0;		end
		6'h1C: begin	tBoundLo = 12'h030;		tBoundMi = 12'h0C0;		end
		6'h1D: begin	tBoundLo = 12'h034;		tBoundMi = 12'h0D0;		end
		6'h1E: begin	tBoundLo = 12'h038;		tBoundMi = 12'h0E0;		end
		6'h1F: begin	tBoundLo = 12'h03C;		tBoundMi = 12'h0F0;		end

		6'h20: begin	tBoundLo = 12'h040;		tBoundMi = 12'h100;		end
		6'h21: begin	tBoundLo = 12'h048;		tBoundMi = 12'h120;		end
		6'h22: begin	tBoundLo = 12'h050;		tBoundMi = 12'h140;		end
		6'h23: begin	tBoundLo = 12'h058;		tBoundMi = 12'h160;		end
		6'h24: begin	tBoundLo = 12'h060;		tBoundMi = 12'h180;		end
		6'h25: begin	tBoundLo = 12'h068;		tBoundMi = 12'h1A0;		end
		6'h26: begin	tBoundLo = 12'h070;		tBoundMi = 12'h1C0;		end
		6'h27: begin	tBoundLo = 12'h078;		tBoundMi = 12'h1E0;		end
		6'h28: begin	tBoundLo = 12'h080;		tBoundMi = 12'h200;		end
		6'h29: begin	tBoundLo = 12'h090;		tBoundMi = 12'h240;		end
		6'h2A: begin	tBoundLo = 12'h0A0;		tBoundMi = 12'h280;		end
		6'h2B: begin	tBoundLo = 12'h0B0;		tBoundMi = 12'h2C0;		end
		6'h2C: begin	tBoundLo = 12'h0C0;		tBoundMi = 12'h300;		end
		6'h2D: begin	tBoundLo = 12'h0D0;		tBoundMi = 12'h340;		end
		6'h2E: begin	tBoundLo = 12'h0E0;		tBoundMi = 12'h380;		end
		6'h2F: begin	tBoundLo = 12'h0F0;		tBoundMi = 12'h3C0;		end

		6'h30: begin	tBoundLo = 12'h100;		tBoundMi = 12'h400;		end
		6'h31: begin	tBoundLo = 12'h120;		tBoundMi = 12'h480;		end
		6'h32: begin	tBoundLo = 12'h140;		tBoundMi = 12'h500;		end
		6'h33: begin	tBoundLo = 12'h160;		tBoundMi = 12'h580;		end
		6'h34: begin	tBoundLo = 12'h180;		tBoundMi = 12'h600;		end
		6'h35: begin	tBoundLo = 12'h1A0;		tBoundMi = 12'h680;		end
		6'h36: begin	tBoundLo = 12'h1C0;		tBoundMi = 12'h700;		end
		6'h37: begin	tBoundLo = 12'h1E0;		tBoundMi = 12'h780;		end
		6'h38: begin	tBoundLo = 12'h200;		tBoundMi = 12'h800;		end
		6'h39: begin	tBoundLo = 12'h240;		tBoundMi = 12'h900;		end
		6'h3A: begin	tBoundLo = 12'h280;		tBoundMi = 12'hA00;		end
		6'h3B: begin	tBoundLo = 12'h2C0;		tBoundMi = 12'hB00;		end
		6'h3C: begin	tBoundLo = 12'h300;		tBoundMi = 12'hC00;		end
		6'h3D: begin	tBoundLo = 12'h340;		tBoundMi = 12'hD00;		end
		6'h3E: begin	tBoundLo = 12'h380;		tBoundMi = 12'hE00;		end
		6'h3F: begin	tBoundLo = 12'h3C0;		tBoundMi = 12'hF00;		end
	endcase

	case({tag[5:3], tag[11:8]})
		7'h00: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h01: begin	tBoundBiasLo = 12'h001;	tBoundBiasMi = 12'h002;		end
		7'h02: begin	tBoundBiasLo = 12'h002;	tBoundBiasMi = 12'h004;		end
		7'h03: begin	tBoundBiasLo = 12'h003;	tBoundBiasMi = 12'h006;		end
		7'h04: begin	tBoundBiasLo = 12'h004;	tBoundBiasMi = 12'h008;		end
		7'h05: begin	tBoundBiasLo = 12'h005;	tBoundBiasMi = 12'h00A;		end
		7'h06: begin	tBoundBiasLo = 12'h006;	tBoundBiasMi = 12'h00C;		end
		7'h07: begin	tBoundBiasLo = 12'h007;	tBoundBiasMi = 12'h00E;		end
		7'h08: begin	tBoundBiasLo = 12'h008;	tBoundBiasMi = 12'h010;		end
		7'h09: begin	tBoundBiasLo = 12'h009;	tBoundBiasMi = 12'h012;		end
		7'h0A: begin	tBoundBiasLo = 12'h00A;	tBoundBiasMi = 12'h014;		end
		7'h0B: begin	tBoundBiasLo = 12'h00B;	tBoundBiasMi = 12'h016;		end
		7'h0C: begin	tBoundBiasLo = 12'h00C;	tBoundBiasMi = 12'h018;		end
		7'h0D: begin	tBoundBiasLo = 12'h00D;	tBoundBiasMi = 12'h01A;		end
		7'h0E: begin	tBoundBiasLo = 12'h00E;	tBoundBiasMi = 12'h01C;		end
		7'h0F: begin	tBoundBiasLo = 12'h00F;	tBoundBiasMi = 12'h01E;		end

		7'h10: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h11: begin	tBoundBiasLo = 12'h001;	tBoundBiasMi = 12'h004;		end
		7'h12: begin	tBoundBiasLo = 12'h002;	tBoundBiasMi = 12'h008;		end
		7'h13: begin	tBoundBiasLo = 12'h003;	tBoundBiasMi = 12'h00C;		end
		7'h14: begin	tBoundBiasLo = 12'h004;	tBoundBiasMi = 12'h010;		end
		7'h15: begin	tBoundBiasLo = 12'h005;	tBoundBiasMi = 12'h014;		end
		7'h16: begin	tBoundBiasLo = 12'h006;	tBoundBiasMi = 12'h018;		end
		7'h17: begin	tBoundBiasLo = 12'h007;	tBoundBiasMi = 12'h01C;		end
		7'h18: begin	tBoundBiasLo = 12'h008;	tBoundBiasMi = 12'h020;		end
		7'h19: begin	tBoundBiasLo = 12'h009;	tBoundBiasMi = 12'h024;		end
		7'h1A: begin	tBoundBiasLo = 12'h00A;	tBoundBiasMi = 12'h028;		end
		7'h1B: begin	tBoundBiasLo = 12'h00B;	tBoundBiasMi = 12'h02C;		end
		7'h1C: begin	tBoundBiasLo = 12'h00C;	tBoundBiasMi = 12'h030;		end
		7'h1D: begin	tBoundBiasLo = 12'h00D;	tBoundBiasMi = 12'h034;		end
		7'h1E: begin	tBoundBiasLo = 12'h00E;	tBoundBiasMi = 12'h038;		end
		7'h1F: begin	tBoundBiasLo = 12'h00F;	tBoundBiasMi = 12'h03C;		end

		7'h20: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h21: begin	tBoundBiasLo = 12'h002;	tBoundBiasMi = 12'h008;		end
		7'h22: begin	tBoundBiasLo = 12'h004;	tBoundBiasMi = 12'h010;		end
		7'h23: begin	tBoundBiasLo = 12'h006;	tBoundBiasMi = 12'h018;		end
		7'h24: begin	tBoundBiasLo = 12'h008;	tBoundBiasMi = 12'h020;		end
		7'h25: begin	tBoundBiasLo = 12'h00A;	tBoundBiasMi = 12'h028;		end
		7'h26: begin	tBoundBiasLo = 12'h00C;	tBoundBiasMi = 12'h030;		end
		7'h27: begin	tBoundBiasLo = 12'h00E;	tBoundBiasMi = 12'h038;		end
		7'h28: begin	tBoundBiasLo = 12'h010;	tBoundBiasMi = 12'h040;		end
		7'h29: begin	tBoundBiasLo = 12'h012;	tBoundBiasMi = 12'h048;		end
		7'h2A: begin	tBoundBiasLo = 12'h014;	tBoundBiasMi = 12'h050;		end
		7'h2B: begin	tBoundBiasLo = 12'h016;	tBoundBiasMi = 12'h058;		end
		7'h2C: begin	tBoundBiasLo = 12'h018;	tBoundBiasMi = 12'h060;		end
		7'h2D: begin	tBoundBiasLo = 12'h01A;	tBoundBiasMi = 12'h068;		end
		7'h2E: begin	tBoundBiasLo = 12'h01C;	tBoundBiasMi = 12'h070;		end
		7'h2F: begin	tBoundBiasLo = 12'h01E;	tBoundBiasMi = 12'h078;		end

		7'h30: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h31: begin	tBoundBiasLo = 12'h004;	tBoundBiasMi = 12'h010;		end
		7'h32: begin	tBoundBiasLo = 12'h008;	tBoundBiasMi = 12'h020;		end
		7'h33: begin	tBoundBiasLo = 12'h00C;	tBoundBiasMi = 12'h030;		end
		7'h34: begin	tBoundBiasLo = 12'h010;	tBoundBiasMi = 12'h040;		end
		7'h35: begin	tBoundBiasLo = 12'h014;	tBoundBiasMi = 12'h050;		end
		7'h36: begin	tBoundBiasLo = 12'h018;	tBoundBiasMi = 12'h060;		end
		7'h37: begin	tBoundBiasLo = 12'h01C;	tBoundBiasMi = 12'h070;		end
		7'h38: begin	tBoundBiasLo = 12'h020;	tBoundBiasMi = 12'h080;		end
		7'h39: begin	tBoundBiasLo = 12'h024;	tBoundBiasMi = 12'h090;		end
		7'h3A: begin	tBoundBiasLo = 12'h028;	tBoundBiasMi = 12'h0A0;		end
		7'h3B: begin	tBoundBiasLo = 12'h02C;	tBoundBiasMi = 12'h0B0;		end
		7'h3C: begin	tBoundBiasLo = 12'h030;	tBoundBiasMi = 12'h0C0;		end
		7'h3D: begin	tBoundBiasLo = 12'h034;	tBoundBiasMi = 12'h0D0;		end
		7'h3E: begin	tBoundBiasLo = 12'h038;	tBoundBiasMi = 12'h0E0;		end
		7'h3F: begin	tBoundBiasLo = 12'h03C;	tBoundBiasMi = 12'h0F0;		end

		7'h40: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h41: begin	tBoundBiasLo = 12'h008;	tBoundBiasMi = 12'h020;		end
		7'h42: begin	tBoundBiasLo = 12'h010;	tBoundBiasMi = 12'h040;		end
		7'h43: begin	tBoundBiasLo = 12'h018;	tBoundBiasMi = 12'h060;		end
		7'h44: begin	tBoundBiasLo = 12'h020;	tBoundBiasMi = 12'h080;		end
		7'h45: begin	tBoundBiasLo = 12'h028;	tBoundBiasMi = 12'h0A0;		end
		7'h46: begin	tBoundBiasLo = 12'h030;	tBoundBiasMi = 12'h0C0;		end
		7'h47: begin	tBoundBiasLo = 12'h038;	tBoundBiasMi = 12'h0E0;		end
		7'h48: begin	tBoundBiasLo = 12'h040;	tBoundBiasMi = 12'h100;		end
		7'h49: begin	tBoundBiasLo = 12'h048;	tBoundBiasMi = 12'h120;		end
		7'h4A: begin	tBoundBiasLo = 12'h050;	tBoundBiasMi = 12'h140;		end
		7'h4B: begin	tBoundBiasLo = 12'h058;	tBoundBiasMi = 12'h160;		end
		7'h4C: begin	tBoundBiasLo = 12'h060;	tBoundBiasMi = 12'h180;		end
		7'h4D: begin	tBoundBiasLo = 12'h068;	tBoundBiasMi = 12'h1A0;		end
		7'h4E: begin	tBoundBiasLo = 12'h070;	tBoundBiasMi = 12'h1C0;		end
		7'h4F: begin	tBoundBiasLo = 12'h078;	tBoundBiasMi = 12'h1E0;		end

		7'h50: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h51: begin	tBoundBiasLo = 12'h010;	tBoundBiasMi = 12'h040;		end
		7'h52: begin	tBoundBiasLo = 12'h020;	tBoundBiasMi = 12'h080;		end
		7'h53: begin	tBoundBiasLo = 12'h030;	tBoundBiasMi = 12'h0C0;		end
		7'h54: begin	tBoundBiasLo = 12'h040;	tBoundBiasMi = 12'h100;		end
		7'h55: begin	tBoundBiasLo = 12'h050;	tBoundBiasMi = 12'h140;		end
		7'h56: begin	tBoundBiasLo = 12'h060;	tBoundBiasMi = 12'h180;		end
		7'h57: begin	tBoundBiasLo = 12'h070;	tBoundBiasMi = 12'h1C0;		end
		7'h58: begin	tBoundBiasLo = 12'h080;	tBoundBiasMi = 12'h200;		end
		7'h59: begin	tBoundBiasLo = 12'h090;	tBoundBiasMi = 12'h240;		end
		7'h5A: begin	tBoundBiasLo = 12'h0A0;	tBoundBiasMi = 12'h280;		end
		7'h5B: begin	tBoundBiasLo = 12'h0B0;	tBoundBiasMi = 12'h2C0;		end
		7'h5C: begin	tBoundBiasLo = 12'h0C0;	tBoundBiasMi = 12'h300;		end
		7'h5D: begin	tBoundBiasLo = 12'h0D0;	tBoundBiasMi = 12'h340;		end
		7'h5E: begin	tBoundBiasLo = 12'h0E0;	tBoundBiasMi = 12'h380;		end
		7'h5F: begin	tBoundBiasLo = 12'h0F0;	tBoundBiasMi = 12'h3C0;		end

		7'h60: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h61: begin	tBoundBiasLo = 12'h020;	tBoundBiasMi = 12'h080;		end
		7'h62: begin	tBoundBiasLo = 12'h040;	tBoundBiasMi = 12'h100;		end
		7'h63: begin	tBoundBiasLo = 12'h060;	tBoundBiasMi = 12'h180;		end
		7'h64: begin	tBoundBiasLo = 12'h080;	tBoundBiasMi = 12'h200;		end
		7'h65: begin	tBoundBiasLo = 12'h0A0;	tBoundBiasMi = 12'h280;		end
		7'h66: begin	tBoundBiasLo = 12'h0C0;	tBoundBiasMi = 12'h300;		end
		7'h67: begin	tBoundBiasLo = 12'h0E0;	tBoundBiasMi = 12'h380;		end
		7'h68: begin	tBoundBiasLo = 12'h100;	tBoundBiasMi = 12'h400;		end
		7'h69: begin	tBoundBiasLo = 12'h120;	tBoundBiasMi = 12'h480;		end
		7'h6A: begin	tBoundBiasLo = 12'h140;	tBoundBiasMi = 12'h500;		end
		7'h6B: begin	tBoundBiasLo = 12'h160;	tBoundBiasMi = 12'h580;		end
		7'h6C: begin	tBoundBiasLo = 12'h180;	tBoundBiasMi = 12'h600;		end
		7'h6D: begin	tBoundBiasLo = 12'h1A0;	tBoundBiasMi = 12'h680;		end
		7'h6E: begin	tBoundBiasLo = 12'h1C0;	tBoundBiasMi = 12'h700;		end
		7'h6F: begin	tBoundBiasLo = 12'h1E0;	tBoundBiasMi = 12'h780;		end

		7'h70: begin	tBoundBiasLo = 12'h000;	tBoundBiasMi = 12'h000;		end
		7'h71: begin	tBoundBiasLo = 12'h040;	tBoundBiasMi = 12'h100;		end
		7'h72: begin	tBoundBiasLo = 12'h080;	tBoundBiasMi = 12'h200;		end
		7'h73: begin	tBoundBiasLo = 12'h0C0;	tBoundBiasMi = 12'h300;		end
		7'h74: begin	tBoundBiasLo = 12'h100;	tBoundBiasMi = 12'h400;		end
		7'h75: begin	tBoundBiasLo = 12'h140;	tBoundBiasMi = 12'h500;		end
		7'h76: begin	tBoundBiasLo = 12'h180;	tBoundBiasMi = 12'h600;		end
		7'h77: begin	tBoundBiasLo = 12'h1C0;	tBoundBiasMi = 12'h700;		end
		7'h78: begin	tBoundBiasLo = 12'h200;	tBoundBiasMi = 12'h800;		end
		7'h79: begin	tBoundBiasLo = 12'h240;	tBoundBiasMi = 12'h900;		end
		7'h7A: begin	tBoundBiasLo = 12'h280;	tBoundBiasMi = 12'hA00;		end
		7'h7B: begin	tBoundBiasLo = 12'h2C0;	tBoundBiasMi = 12'hB00;		end
		7'h7C: begin	tBoundBiasLo = 12'h300;	tBoundBiasMi = 12'hC00;		end
		7'h7D: begin	tBoundBiasLo = 12'h340;	tBoundBiasMi = 12'hD00;		end
		7'h7E: begin	tBoundBiasLo = 12'h380;	tBoundBiasMi = 12'hE00;		end
		7'h7F: begin	tBoundBiasLo = 12'h3C0;	tBoundBiasMi = 12'hF00;		end
	endcase
	
	case(opUIxt[1:0])
		2'b00: tRegRiShr = regRi[33:0];
		2'b01: tRegRiShr = regRi[34:1];
		2'b10: tRegRiShr = regRi[35:2];
		2'b11: tRegRiShr = regRi[36:3];
	endcase
	
	{ tBoundRef0C, tBoundRef0 }	=
		{1'b0, regRi[11: 0]} + {1'b0, tBoundBiasLo};
	{ tBoundRef1C, tBoundRef1 }	=
		{1'b0, regRi[17: 6]} + {1'b0, tBoundBiasMi};
	{ tBoundRef2C, tBoundRef2 }	=
		{1'b0, regRi[25:14]} + {1'b0, tBoundBiasMi};
	{ tBoundRef3C, tBoundRef3 }	=
		{1'b0, regRi[33:22]} + {1'b0, tBoundBiasMi};
	
	tBoundNz0	= (regRi[ 9: 0] != 0);
//	tBoundNz1	= (regRi[19:10] != 0);
//	tBoundNz2	= (regRi[29:20] != 0);
//	tBoundNz3	= (regRi[39:30] != 0);
	
//	tBoundFail0	= (regRi[ 9: 0] >= tBoundLo);
//	tBoundFail1	= (regRi[19:10] >= tBoundLo);
//	tBoundFail2	= (regRi[29:20] >= tBoundLo);
//	tBoundFail3	= (regRi[39:30] >= tBoundLo);


	tBoundNz1	= (regRi[17:10] != 0);
	tBoundNz2	= (regRi[25:18] != 0);
	tBoundNz3	= (regRi[33:26] != 0);
	
	if(tBoundRef0C)
		tBoundNz1	= (regRi[17:10] != 8'hFF);
	if(tBoundRef1C || tBoundRef0C)
		tBoundNz2	= (regRi[25:18] != 8'hFF);
	if(tBoundRef2C || tBoundRef1C || tBoundRef0C)
		tBoundNz3	= (regRi[33:26] != 8'hFF);

//	tBoundFail1	= (regRi[17: 6] >= tBoundMi);
//	tBoundFail2	= (regRi[25:14] >= tBoundMi);
//	tBoundFail3	= (regRi[33:22] >= tBoundMi);

	tBoundFail0	= (tBoundRef0 >= tBoundLo);
	tBoundFail1	= (tBoundRef1 >= tBoundMi);
	tBoundFail2	= (tBoundRef2 >= tBoundMi);
	tBoundFail3	= (tBoundRef3 >= tBoundMi);

	tBoundFailL10 = (regRi[9:0] >= tag[9:0]) ||
		tBoundNz1 || tBoundNz2 || tBoundNz3;

	case(tag[7:6])
		2'b00: tBoundFailP8 = tBoundFail0 || tBoundNz1 || tBoundNz2 || tBoundNz3;
		2'b01: tBoundFailP8 = tBoundFail1 || tBoundNz2 || tBoundNz3;
		2'b10: tBoundFailP8 = tBoundFail2 || tBoundNz3;
		2'b11: tBoundFailP8 = tBoundFail3;
	endcase

	case(tag[15:12])
//		4'h0: tBoundFail = 0;	/* Base Pointer */
		4'h0: tBoundFail = (tag[11:0] != 0) && isBndCmp;	/* Base Pointer */
		4'h1: tBoundFail = 1;
		4'h2: tBoundFail = 0;
//		4'h2: tBoundFail = tBoundFailL10;
		4'h3: tBoundFail = tBoundFailP8;

		4'h4: tBoundFail = 1;	/* Fixnum */
		4'h5: tBoundFail = 1;	/* Fixnum */
		4'h6: tBoundFail = 1;	/* Fixnum */
		4'h7: tBoundFail = 1;	/* Fixnum */

		4'h8: tBoundFail = 1;	/* Flonum */
		4'h9: tBoundFail = 1;	/* Flonum */
		4'hA: tBoundFail = 1;	/* Flonum */
		4'hB: tBoundFail = 1;	/* Flonum */

		4'hC: tBoundFail = 1;
		4'hD: tBoundFail = 1;
		4'hE: tBoundFail = 1;
		4'hF: tBoundFail = 0;	/* Raw Pointer 60 */
	endcase

	if(tBoundFail && isBndChk)
	begin
		$display("Bound Fail tag=%X ri=%X", tag, regRi);
	end

`ifndef def_true	
	case(tag[7:6])
		2'b00: tBoundRef = regRi[ 9: 0];
		2'b01: tBoundRef = regRi[19:10];
		2'b10: tBoundRef = regRi[29:20];
		2'b11: tBoundRef = regRi[39:30];
	endcase
	
	tBoundFail = (tBoundRef >= tBoundLo);
`endif

end

endmodule
