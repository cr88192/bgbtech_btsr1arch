/*
 * RISC-V Superscalar Check
 * Check Bundle for Superscalar Execution.
 * Synthesize WEX Bits
 */

`include "DecRvSscFlag.v"

module DecRvSscCheck(istrWord, istrWxFlag);
input[95:0]		istrWord;
output[3:0]		istrWxFlag;

reg[3:0]		tIstrWxFlag;
assign		istrWxFlag = 0;

wire[31:0]	op1istr;
wire[31:0]	op2istr;
wire[31:0]	op3istr;
assign	op1istr = istrWord[31: 0];
assign	op2istr = istrWord[63:32];
assign	op3istr = istrWord[95:64];

wire[3:0]	op1ssfl;
wire[3:0]	op2ssfl;
wire[3:0]	op3ssfl;

DecRvSscFlag opFl1(op1istr, op1ssfl);
DecRvSscFlag opFl2(op2istr, op2ssfl);
DecRvSscFlag opFl3(op3istr, op3ssfl);

wire[31:0]	op1RegRd;
wire[31:0]	op1RegRs;
wire[31:0]	op1RegRt;
assign	op1RegRd = op1istr[11: 7];
assign	op1RegRs = op1istr[19:15];
assign	op1RegRt = op1istr[24:20];

wire[31:0]	op2RegRd;
wire[31:0]	op2RegRs;
wire[31:0]	op2RegRt;
assign	op2RegRd = op2istr[11: 7];
assign	op2RegRs = op2istr[19:15];
assign	op2RegRt = op2istr[24:20];

wire[31:0]	op3RegRd;
wire[31:0]	op3RegRs;
wire[31:0]	op3RegRt;
assign	op3RegRd = op3istr[11: 7];
assign	op3RegRs = op3istr[19:15];
assign	op3RegRt = op3istr[24:20];

reg		tDepOp1d2;
reg		tDepOp1d3;
reg		tDepOp2d3;

always @*
begin
	tIstrWxFlag[1:0] = 0;

	tDepOp1d2 = (op1RegRd == op2RegRs) || (op1RegRd == op2RegRt);
	tDepOp1d3 = (op1RegRd == op3RegRs) || (op1RegRd == op3RegRt);
	tDepOp2d3 = (op2RegRd == op3RegRs) || (op2RegRd == op3RegRt);
	
	if(!tDepOp1d2 && !tDepOp1d3 && !tDepOp2d3)
	begin
		if((op3ssfl[1:0]==2'b11) && op2ssfl[2] && op1ssfl[3])
			tIstrWxFlag[1:0]=2'b11;
		else if(op2ssfl[0] && op12ssfl[2])
			tIstrWxFlag[1:0]=2'b01;
	end
	else if(!tDepOp1d2)
	begin
		else if(op2ssfl[0] && op12ssfl[2])
			tIstrWxFlag[1:0]=2'b01;
	end
end

endmodule
