/*
32-bit SHAD/SHLD Unit
 */

module Jx2ExShad32(
	clock, reset,
	valRs, valRt,
	valRn, shOp
	);

input	clock;
input	reset;

/* verilator lint_off UNOPTFLAT */

input[31:0]		valRs;
input[ 7:0]		valRt;
input[ 2:0]		shOp;
output[31:0]	valRn;

reg[31:0]		tValRn;
assign			valRn = tValRn;
/* verilator lint_on UNOPTFLAT */

reg[31:0]		tValRol;
reg[31:0]		tValRor;
reg[ 7:0]		tValSh;

always @*
begin

	tValRol=0;
	tValRor=0;
	tValRn = 0;
	tValSh = 0;

	case(shOp)
	3'h0: begin
	end

	3'h1: begin		//SHLD
		tValRol=0;
		tValRor=0;
		tValSh = valRt;
	end

	3'h2: begin		//SHAD
		tValRol=0;
		tValRor=valRs[31] ? 32'hFFFFFFFF : 32'h00000000;
		tValSh = valRt;
	end

/*
	3'h3: begin		//SHLDR
		tValRol=0;
		tValRor=0;
		tValSh = -valRt;
	end

	3'h4: begin		//SHADR
		tValRol=0;
		tValRor=valRs[31] ? 32'hFFFFFFFF : 32'h00000000;
		tValSh = -valRt;
	end
*/

	default:
	begin
		tValRol=0;
		tValRor=valRs[31] ? 32'hFFFFFFFF : 32'h00000000;
		tValSh = valRt;
	end

	endcase

	casez(tValSh)
	8'b0zz00000: tValRn = valRs;
	8'b0zz00001: tValRn = { valRs[30:0], tValRol[31   ] };
	8'b0zz00010: tValRn = { valRs[29:0], tValRol[31:30] };
	8'b0zz00011: tValRn = { valRs[28:0], tValRol[31:29] };
	8'b0zz00100: tValRn = { valRs[27:0], tValRol[31:28] };
	8'b0zz00101: tValRn = { valRs[26:0], tValRol[31:27] };
	8'b0zz00110: tValRn = { valRs[25:0], tValRol[31:26] };
	8'b0zz00111: tValRn = { valRs[24:0], tValRol[31:25] };
	8'b0zz01000: tValRn = { valRs[23:0], tValRol[31:24] };
	8'b0zz01001: tValRn = { valRs[22:0], tValRol[31:23] };
	8'b0zz01010: tValRn = { valRs[21:0], tValRol[31:22] };
	8'b0zz01011: tValRn = { valRs[20:0], tValRol[31:21] };
	8'b0zz01100: tValRn = { valRs[19:0], tValRol[31:20] };
	8'b0zz01101: tValRn = { valRs[18:0], tValRol[31:19] };
	8'b0zz01110: tValRn = { valRs[17:0], tValRol[31:18] };
	8'b0zz01111: tValRn = { valRs[16:0], tValRol[31:17] };
	8'b0zz10000: tValRn = { valRs[15:0], tValRol[31:16] };
	8'b0zz10001: tValRn = { valRs[14:0], tValRol[31:15] };
	8'b0zz10010: tValRn = { valRs[13:0], tValRol[31:14] };
	8'b0zz10011: tValRn = { valRs[12:0], tValRol[31:13] };
	8'b0zz10100: tValRn = { valRs[11:0], tValRol[31:12] };
	8'b0zz10101: tValRn = { valRs[10:0], tValRol[31:11] };
	8'b0zz10110: tValRn = { valRs[ 9:0], tValRol[31:10] };
	8'b0zz10111: tValRn = { valRs[ 8:0], tValRol[31: 9] };
	8'b0zz11000: tValRn = { valRs[ 7:0], tValRol[31: 8] };
	8'b0zz11001: tValRn = { valRs[ 6:0], tValRol[31: 7] };
	8'b0zz11010: tValRn = { valRs[ 5:0], tValRol[31: 6] };
	8'b0zz11011: tValRn = { valRs[ 4:0], tValRol[31: 5] };
	8'b0zz11100: tValRn = { valRs[ 3:0], tValRol[31: 4] };
	8'b0zz11101: tValRn = { valRs[ 2:0], tValRol[31: 3] };
	8'b0zz11110: tValRn = { valRs[ 1:0], tValRol[31: 2] };
	8'b0zz11111: tValRn = { valRs[   0], tValRol[31: 1] };

	8'b1zz11111: tValRn = { tValRor[ 0  ], valRs[31: 1] };
	8'b1zz11110: tValRn = { tValRor[ 1:0], valRs[31: 2] };
	8'b1zz11101: tValRn = { tValRor[ 2:0], valRs[31: 3] };
	8'b1zz11100: tValRn = { tValRor[ 3:0], valRs[31: 4] };
	8'b1zz11011: tValRn = { tValRor[ 4:0], valRs[31: 5] };
	8'b1zz11010: tValRn = { tValRor[ 5:0], valRs[31: 6] };
	8'b1zz11001: tValRn = { tValRor[ 6:0], valRs[31: 7] };
	8'b1zz11000: tValRn = { tValRor[ 7:0], valRs[31: 8] };
	8'b1zz10111: tValRn = { tValRor[ 8:0], valRs[31: 9] };
	8'b1zz10110: tValRn = { tValRor[ 9:0], valRs[31:10] };
	8'b1zz10101: tValRn = { tValRor[10:0], valRs[31:11] };
	8'b1zz10100: tValRn = { tValRor[11:0], valRs[31:12] };
	8'b1zz10011: tValRn = { tValRor[12:0], valRs[31:13] };
	8'b1zz10010: tValRn = { tValRor[13:0], valRs[31:14] };
	8'b1zz10001: tValRn = { tValRor[14:0], valRs[31:15] };
	8'b1zz10000: tValRn = { tValRor[15:0], valRs[31:16] };
	8'b1zz01111: tValRn = { tValRor[16:0], valRs[31:17] };
	8'b1zz01110: tValRn = { tValRor[17:0], valRs[31:18] };
	8'b1zz01101: tValRn = { tValRor[18:0], valRs[31:19] };
	8'b1zz01100: tValRn = { tValRor[19:0], valRs[31:20] };
	8'b1zz01011: tValRn = { tValRor[20:0], valRs[31:21] };
	8'b1zz01010: tValRn = { tValRor[21:0], valRs[31:22] };
	8'b1zz01001: tValRn = { tValRor[22:0], valRs[31:23] };
	8'b1zz01000: tValRn = { tValRor[23:0], valRs[31:24] };
	8'b1zz00111: tValRn = { tValRor[24:0], valRs[31:25] };
	8'b1zz00110: tValRn = { tValRor[25:0], valRs[31:26] };
	8'b1zz00101: tValRn = { tValRor[26:0], valRs[31:27] };
	8'b1zz00100: tValRn = { tValRor[27:0], valRs[31:28] };
	8'b1zz00011: tValRn = { tValRor[28:0], valRs[31:29] };
	8'b1zz00010: tValRn = { tValRor[29:0], valRs[31:30] };
	8'b1zz00001: tValRn = { tValRor[30:0], valRs[31   ] };
	8'b1zz00000: tValRn = tValRor;

	endcase

end

endmodule
