/*
64-bit SHAD/SHLD Unit
 */

`ifndef HAS_JX2EXSHADQ
`define HAS_JX2EXSHADQ

module ExShad64(
	/* verilator lint_off UNUSED */
	clock, reset,
	valRs, valRt,
	valRn, shOp
	);

input	clock;
input	reset;

// /* verilator lint_off UNOPTFLAT */

input[63:0]		valRs;
input[ 7:0]		valRt;
input			shOp;
output[63:0]	valRn;

reg[63:0]		tValRn;
assign			valRn = tValRn;
// /* verilator lint_on UNOPTFLAT */

reg[63:0]		tValRol;
reg[63:0]		tValRor;
reg[ 7:0]		tValSh;

always @*
begin

	tValRol=0;
	tValRor=0;
	tValRn = 0;
//	tValSh = 0;
	tValSh = valRt;

	if(shOp)
		tValRor=0;
	else
		tValRor=valRs[63] ? 64'hFFFFFFFF : 64'h00000000;

	casez(tValSh)
	8'b0z000000: tValRn = valRs;
	8'b0z000001: tValRn = { valRs[62:0], tValRol[63   ] };
	8'b0z000010: tValRn = { valRs[61:0], tValRol[63:62] };
	8'b0z000011: tValRn = { valRs[60:0], tValRol[63:61] };
	8'b0z000100: tValRn = { valRs[59:0], tValRol[63:60] };
	8'b0z000101: tValRn = { valRs[58:0], tValRol[63:59] };
	8'b0z000110: tValRn = { valRs[57:0], tValRol[63:58] };
	8'b0z000111: tValRn = { valRs[56:0], tValRol[63:57] };
	8'b0z001000: tValRn = { valRs[55:0], tValRol[63:56] };
	8'b0z001001: tValRn = { valRs[54:0], tValRol[63:55] };
	8'b0z001010: tValRn = { valRs[53:0], tValRol[63:54] };
	8'b0z001011: tValRn = { valRs[52:0], tValRol[63:53] };
	8'b0z001100: tValRn = { valRs[51:0], tValRol[63:52] };
	8'b0z001101: tValRn = { valRs[50:0], tValRol[63:51] };
	8'b0z001110: tValRn = { valRs[49:0], tValRol[63:50] };
	8'b0z001111: tValRn = { valRs[48:0], tValRol[63:49] };
	8'b0z010000: tValRn = { valRs[47:0], tValRol[63:48] };
	8'b0z010001: tValRn = { valRs[46:0], tValRol[63:47] };
	8'b0z010010: tValRn = { valRs[45:0], tValRol[63:46] };
	8'b0z010011: tValRn = { valRs[44:0], tValRol[63:45] };
	8'b0z010100: tValRn = { valRs[43:0], tValRol[63:44] };
	8'b0z010101: tValRn = { valRs[42:0], tValRol[63:43] };
	8'b0z010110: tValRn = { valRs[41:0], tValRol[63:42] };
	8'b0z010111: tValRn = { valRs[40:0], tValRol[63:41] };
	8'b0z011000: tValRn = { valRs[39:0], tValRol[63:40] };
	8'b0z011001: tValRn = { valRs[38:0], tValRol[63:39] };
	8'b0z011010: tValRn = { valRs[37:0], tValRol[63:38] };
	8'b0z011011: tValRn = { valRs[36:0], tValRol[63:37] };
	8'b0z011100: tValRn = { valRs[35:0], tValRol[63:36] };
	8'b0z011101: tValRn = { valRs[34:0], tValRol[63:35] };
	8'b0z011110: tValRn = { valRs[33:0], tValRol[63:34] };
	8'b0z011111: tValRn = { valRs[32:0], tValRol[63:33] };
	8'b0z100000: tValRn = { valRs[31:0], tValRol[63:32] };
	8'b0z100001: tValRn = { valRs[30:0], tValRol[63:31] };
	8'b0z100010: tValRn = { valRs[29:0], tValRol[63:30] };
	8'b0z100011: tValRn = { valRs[28:0], tValRol[63:29] };
	8'b0z100100: tValRn = { valRs[27:0], tValRol[63:28] };
	8'b0z100101: tValRn = { valRs[26:0], tValRol[63:27] };
	8'b0z100110: tValRn = { valRs[25:0], tValRol[63:26] };
	8'b0z100111: tValRn = { valRs[24:0], tValRol[63:25] };
	8'b0z101000: tValRn = { valRs[23:0], tValRol[63:24] };
	8'b0z101001: tValRn = { valRs[22:0], tValRol[63:23] };
	8'b0z101010: tValRn = { valRs[21:0], tValRol[63:22] };
	8'b0z101011: tValRn = { valRs[20:0], tValRol[63:21] };
	8'b0z101100: tValRn = { valRs[19:0], tValRol[63:20] };
	8'b0z101101: tValRn = { valRs[18:0], tValRol[63:19] };
	8'b0z101110: tValRn = { valRs[17:0], tValRol[63:18] };
	8'b0z101111: tValRn = { valRs[16:0], tValRol[63:17] };
	8'b0z110000: tValRn = { valRs[15:0], tValRol[63:16] };
	8'b0z110001: tValRn = { valRs[14:0], tValRol[63:15] };
	8'b0z110010: tValRn = { valRs[13:0], tValRol[63:14] };
	8'b0z110011: tValRn = { valRs[12:0], tValRol[63:13] };
	8'b0z110100: tValRn = { valRs[11:0], tValRol[63:12] };
	8'b0z110101: tValRn = { valRs[10:0], tValRol[63:11] };
	8'b0z110110: tValRn = { valRs[ 9:0], tValRol[63:10] };
	8'b0z110111: tValRn = { valRs[ 8:0], tValRol[63: 9] };
	8'b0z111000: tValRn = { valRs[ 7:0], tValRol[63: 8] };
	8'b0z111001: tValRn = { valRs[ 6:0], tValRol[63: 7] };
	8'b0z111010: tValRn = { valRs[ 5:0], tValRol[63: 6] };
	8'b0z111011: tValRn = { valRs[ 4:0], tValRol[63: 5] };
	8'b0z111100: tValRn = { valRs[ 3:0], tValRol[63: 4] };
	8'b0z111101: tValRn = { valRs[ 2:0], tValRol[63: 3] };
	8'b0z111110: tValRn = { valRs[ 1:0], tValRol[63: 2] };
	8'b0z111111: tValRn = { valRs[   0], tValRol[63: 1] };

	8'b1z111111: tValRn = { tValRor[ 0  ], valRs[63: 1] };
	8'b1z111110: tValRn = { tValRor[ 1:0], valRs[63: 2] };
	8'b1z111101: tValRn = { tValRor[ 2:0], valRs[63: 3] };
	8'b1z111100: tValRn = { tValRor[ 3:0], valRs[63: 4] };
	8'b1z111011: tValRn = { tValRor[ 4:0], valRs[63: 5] };
	8'b1z111010: tValRn = { tValRor[ 5:0], valRs[63: 6] };
	8'b1z111001: tValRn = { tValRor[ 6:0], valRs[63: 7] };
	8'b1z111000: tValRn = { tValRor[ 7:0], valRs[63: 8] };
	8'b1z110111: tValRn = { tValRor[ 8:0], valRs[63: 9] };
	8'b1z110110: tValRn = { tValRor[ 9:0], valRs[63:10] };
	8'b1z110101: tValRn = { tValRor[10:0], valRs[63:11] };
	8'b1z110100: tValRn = { tValRor[11:0], valRs[63:12] };
	8'b1z110011: tValRn = { tValRor[12:0], valRs[63:13] };
	8'b1z110010: tValRn = { tValRor[13:0], valRs[63:14] };
	8'b1z110001: tValRn = { tValRor[14:0], valRs[63:15] };
	8'b1z110000: tValRn = { tValRor[15:0], valRs[63:16] };
	8'b1z101111: tValRn = { tValRor[16:0], valRs[63:17] };
	8'b1z101110: tValRn = { tValRor[17:0], valRs[63:18] };
	8'b1z101101: tValRn = { tValRor[18:0], valRs[63:19] };
	8'b1z101100: tValRn = { tValRor[19:0], valRs[63:20] };
	8'b1z101011: tValRn = { tValRor[20:0], valRs[63:21] };
	8'b1z101010: tValRn = { tValRor[21:0], valRs[63:22] };
	8'b1z101001: tValRn = { tValRor[22:0], valRs[63:23] };
	8'b1z101000: tValRn = { tValRor[23:0], valRs[63:24] };
	8'b1z100111: tValRn = { tValRor[24:0], valRs[63:25] };
	8'b1z100110: tValRn = { tValRor[25:0], valRs[63:26] };
	8'b1z100101: tValRn = { tValRor[26:0], valRs[63:27] };
	8'b1z100100: tValRn = { tValRor[27:0], valRs[63:28] };
	8'b1z100011: tValRn = { tValRor[28:0], valRs[63:29] };
	8'b1z100010: tValRn = { tValRor[29:0], valRs[63:30] };
	8'b1z100001: tValRn = { tValRor[30:0], valRs[63:31] };
	8'b1z100000: tValRn = { tValRor[31:0], valRs[63:32] };
	8'b1z011111: tValRn = { tValRor[32:0], valRs[63:33] };
	8'b1z011110: tValRn = { tValRor[33:0], valRs[63:34] };
	8'b1z011101: tValRn = { tValRor[34:0], valRs[63:35] };
	8'b1z011100: tValRn = { tValRor[35:0], valRs[63:36] };
	8'b1z011011: tValRn = { tValRor[36:0], valRs[63:37] };
	8'b1z011010: tValRn = { tValRor[37:0], valRs[63:38] };
	8'b1z011001: tValRn = { tValRor[38:0], valRs[63:39] };
	8'b1z011000: tValRn = { tValRor[39:0], valRs[63:40] };
	8'b1z010111: tValRn = { tValRor[40:0], valRs[63:41] };
	8'b1z010110: tValRn = { tValRor[41:0], valRs[63:42] };
	8'b1z010101: tValRn = { tValRor[42:0], valRs[63:43] };
	8'b1z010100: tValRn = { tValRor[43:0], valRs[63:44] };
	8'b1z010011: tValRn = { tValRor[44:0], valRs[63:45] };
	8'b1z010010: tValRn = { tValRor[45:0], valRs[63:46] };
	8'b1z010001: tValRn = { tValRor[46:0], valRs[63:47] };
	8'b1z010000: tValRn = { tValRor[47:0], valRs[63:48] };
	8'b1z001111: tValRn = { tValRor[48:0], valRs[63:49] };
	8'b1z001110: tValRn = { tValRor[49:0], valRs[63:50] };
	8'b1z001101: tValRn = { tValRor[50:0], valRs[63:51] };
	8'b1z001100: tValRn = { tValRor[51:0], valRs[63:52] };
	8'b1z001011: tValRn = { tValRor[52:0], valRs[63:53] };
	8'b1z001010: tValRn = { tValRor[53:0], valRs[63:54] };
	8'b1z001001: tValRn = { tValRor[54:0], valRs[63:55] };
	8'b1z001000: tValRn = { tValRor[55:0], valRs[63:56] };
	8'b1z000111: tValRn = { tValRor[56:0], valRs[63:57] };
	8'b1z000110: tValRn = { tValRor[57:0], valRs[63:58] };
	8'b1z000101: tValRn = { tValRor[58:0], valRs[63:59] };
	8'b1z000100: tValRn = { tValRor[59:0], valRs[63:60] };
	8'b1z000011: tValRn = { tValRor[60:0], valRs[63:61] };
	8'b1z000010: tValRn = { tValRor[61:0], valRs[63:62] };
	8'b1z000001: tValRn = { tValRor[62:0], valRs[63   ] };
	8'b1z000000: tValRn = tValRor;

	endcase

end

endmodule

`endif
