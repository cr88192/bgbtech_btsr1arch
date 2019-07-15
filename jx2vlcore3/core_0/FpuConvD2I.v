module FpuConvD2I(
	clock,		 reset,
	regValFRm,
	regValFRn);

input			clock;
input			reset;

input[63:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRm;

reg[63:0]		tRegValFRn2;
reg[63:0]		tRegValFRn3;
reg[63:0]		tRegValFRnB;

assign	regValFRn = tRegValFRnB;

reg			tRegSgn1;
reg[10:0]	tRegExp1;
reg[51:0]	tRegFra1;
reg[63:0]	tRegFraExt1;

reg			tRegSgn2;
reg[10:0]	tRegExp2;
reg[51:0]	tRegFra2;
reg[63:0]	tRegFraExt2;

always @*
begin
	tRegValFRm	= regValFRm;

	tRegSgn1	= tRegValFRm[63];
	tRegExp1	= tRegValFRm[62:52];
//	if(tRegValFRm[63])
	if(0)
	begin
		tRegFra1 = -tRegValFRm[51:0];
		tRegFraExt1 = 64'hFFFFFFFF_FFFFFFFE;
	end
	else
	begin
		tRegFra1 = tRegValFRm[51:0];
		tRegFraExt1 = 64'h00000000_00000001;
	end

	case(tRegExp2)
		11'h3FF: tRegValFRn2 =   tRegFraExt2[63:0]                   ;
		11'h400: tRegValFRn2 = { tRegFraExt2[62:0], tRegFra2[51   ] };
		11'h401: tRegValFRn2 = { tRegFraExt2[61:0], tRegFra2[51:50] };
		11'h402: tRegValFRn2 = { tRegFraExt2[60:0], tRegFra2[51:49] };
		11'h403: tRegValFRn2 = { tRegFraExt2[59:0], tRegFra2[51:48] };
		11'h404: tRegValFRn2 = { tRegFraExt2[58:0], tRegFra2[51:47] };
		11'h405: tRegValFRn2 = { tRegFraExt2[57:0], tRegFra2[51:46] };
		11'h406: tRegValFRn2 = { tRegFraExt2[56:0], tRegFra2[51:45] };
		11'h407: tRegValFRn2 = { tRegFraExt2[55:0], tRegFra2[51:44] };
		11'h408: tRegValFRn2 = { tRegFraExt2[54:0], tRegFra2[51:43] };
		11'h409: tRegValFRn2 = { tRegFraExt2[53:0], tRegFra2[51:42] };
		11'h40A: tRegValFRn2 = { tRegFraExt2[52:0], tRegFra2[51:41] };
		11'h40B: tRegValFRn2 = { tRegFraExt2[51:0], tRegFra2[51:40] };
		11'h40C: tRegValFRn2 = { tRegFraExt2[50:0], tRegFra2[51:39] };
		11'h40D: tRegValFRn2 = { tRegFraExt2[49:0], tRegFra2[51:38] };
		11'h40E: tRegValFRn2 = { tRegFraExt2[48:0], tRegFra2[51:37] };
		11'h40F: tRegValFRn2 = { tRegFraExt2[47:0], tRegFra2[51:36] };
		11'h410: tRegValFRn2 = { tRegFraExt2[46:0], tRegFra2[51:35] };
		11'h411: tRegValFRn2 = { tRegFraExt2[45:0], tRegFra2[51:34] };
		11'h412: tRegValFRn2 = { tRegFraExt2[44:0], tRegFra2[51:33] };
		11'h413: tRegValFRn2 = { tRegFraExt2[43:0], tRegFra2[51:32] };
		11'h414: tRegValFRn2 = { tRegFraExt2[42:0], tRegFra2[51:31] };
		11'h415: tRegValFRn2 = { tRegFraExt2[41:0], tRegFra2[51:30] };
		11'h416: tRegValFRn2 = { tRegFraExt2[40:0], tRegFra2[51:29] };
		11'h417: tRegValFRn2 = { tRegFraExt2[39:0], tRegFra2[51:28] };
		11'h418: tRegValFRn2 = { tRegFraExt2[38:0], tRegFra2[51:27] };
		11'h419: tRegValFRn2 = { tRegFraExt2[37:0], tRegFra2[51:26] };
		11'h41A: tRegValFRn2 = { tRegFraExt2[36:0], tRegFra2[51:25] };
		11'h41B: tRegValFRn2 = { tRegFraExt2[35:0], tRegFra2[51:24] };
		11'h41C: tRegValFRn2 = { tRegFraExt2[34:0], tRegFra2[51:23] };
		11'h41D: tRegValFRn2 = { tRegFraExt2[33:0], tRegFra2[51:22] };
		11'h41E: tRegValFRn2 = { tRegFraExt2[32:0], tRegFra2[51:21] };
		11'h41F: tRegValFRn2 = { tRegFraExt2[31:0], tRegFra2[51:20] };
		11'h420: tRegValFRn2 = { tRegFraExt2[30:0], tRegFra2[51:19] };
		11'h421: tRegValFRn2 = { tRegFraExt2[29:0], tRegFra2[51:18] };
		11'h422: tRegValFRn2 = { tRegFraExt2[28:0], tRegFra2[51:17] };
		11'h423: tRegValFRn2 = { tRegFraExt2[27:0], tRegFra2[51:16] };
		11'h424: tRegValFRn2 = { tRegFraExt2[26:0], tRegFra2[51:15] };
		11'h425: tRegValFRn2 = { tRegFraExt2[25:0], tRegFra2[51:14] };
		11'h426: tRegValFRn2 = { tRegFraExt2[24:0], tRegFra2[51:13] };
		11'h427: tRegValFRn2 = { tRegFraExt2[23:0], tRegFra2[51:12] };
		11'h428: tRegValFRn2 = { tRegFraExt2[22:0], tRegFra2[51:11] };
		11'h429: tRegValFRn2 = { tRegFraExt2[21:0], tRegFra2[51:10] };
		11'h42A: tRegValFRn2 = { tRegFraExt2[20:0], tRegFra2[51: 9] };
		11'h42B: tRegValFRn2 = { tRegFraExt2[19:0], tRegFra2[51: 8] };
		11'h42C: tRegValFRn2 = { tRegFraExt2[18:0], tRegFra2[51: 7] };
		11'h42D: tRegValFRn2 = { tRegFraExt2[17:0], tRegFra2[51: 6] };
		11'h42E: tRegValFRn2 = { tRegFraExt2[16:0], tRegFra2[51: 5] };
		11'h42F: tRegValFRn2 = { tRegFraExt2[15:0], tRegFra2[51: 4] };
		11'h430: tRegValFRn2 = { tRegFraExt2[14:0], tRegFra2[51: 3] };
		11'h431: tRegValFRn2 = { tRegFraExt2[13:0], tRegFra2[51: 2] };
		11'h432: tRegValFRn2 = { tRegFraExt2[12:0], tRegFra2[51: 1] };
		11'h433: tRegValFRn2 = { tRegFraExt2[11:0], tRegFra2[51: 0] };
		11'h434: tRegValFRn2 = { tRegFraExt2[10:0], tRegFra2[51: 0],  1'b0 };
		11'h435: tRegValFRn2 = { tRegFraExt2[ 9:0], tRegFra2[51: 0],  2'b0 };
		11'h436: tRegValFRn2 = { tRegFraExt2[ 8:0], tRegFra2[51: 0],  3'b0 };
		11'h437: tRegValFRn2 = { tRegFraExt2[ 7:0], tRegFra2[51: 0],  4'b0 };
		11'h438: tRegValFRn2 = { tRegFraExt2[ 6:0], tRegFra2[51: 0],  5'b0 };
		11'h439: tRegValFRn2 = { tRegFraExt2[ 5:0], tRegFra2[51: 0],  6'b0 };
		11'h43A: tRegValFRn2 = { tRegFraExt2[ 4:0], tRegFra2[51: 0],  7'b0 };
		11'h43B: tRegValFRn2 = { tRegFraExt2[ 3:0], tRegFra2[51: 0],  8'b0 };
		11'h43C: tRegValFRn2 = { tRegFraExt2[ 2:0], tRegFra2[51: 0],  9'b0 };
		11'h43D: tRegValFRn2 = { tRegFraExt2[ 1:0], tRegFra2[51: 0], 10'b0 };
		11'h43E: tRegValFRn2 = { tRegFraExt2[   0], tRegFra2[51: 0], 11'b0 };
//		11'h43F: tRegValFRn2 = {                    tRegFra2[51: 0], 12'b0 };

		default: begin
			if(tRegValFRm[62])
				tRegValFRn2 = 64'h80000000_00000000;
			else
				tRegValFRn2 = UV64_00;
//				tRegValFRn2 = tRegValFRm[63] ? UV64_FF : UV64_00;
		end
	endcase
	
end

always @(posedge clock)
begin
//	tRegValFRm		<= regValFRm;
//	tRegValFRnB		<= tRegValFRn;

	tRegValFRn3		<= tRegValFRn2;
//	tRegValFRnB		<= tRegValFRn3;
	tRegValFRnB		<= tRegSgn2 ? (-tRegValFRn3) : tRegValFRn3;

	tRegSgn2		<= tRegSgn1;
	tRegExp2		<= tRegExp1;
	tRegFra2		<= tRegFra1;
	tRegFraExt2		<= tRegFraExt1;
end

endmodule
