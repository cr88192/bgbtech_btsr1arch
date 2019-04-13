/*

Do Simple MOV+Conv style operation.

 */

module ExConv2R(
	/* verilator lint_off UNUSED */
	regValRs,
	idUIxt,
	regInSrT,
	regOutVal,
	regOutSrT
	);

input[63:0]		regValRs;
input[7:0]		idUIxt;
input			regInSrT;

output[63:0]	regOutVal;
output			regOutSrT;

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
assign	regOutVal = tRegOutVal;
assign	regOutSrT = tRegOutSrT;

always @*
begin
	tRegOutVal	= regValRs;
	tRegOutSrT	= regInSrT;

	case(idUIxt[5:0])
		JX2_UCIX_CONV_EXTSB: tRegOutVal	=
			{ regValRs[ 7]?UV56_FF:UV56_00, regValRs[ 7:0] };
		JX2_UCIX_CONV_EXTSW: tRegOutVal	=
			{ regValRs[15]?UV48_FF:UV48_00, regValRs[15:0] };
		JX2_UCIX_CONV_EXTSL: tRegOutVal	=
			{ regValRs[31]?UV32_FF:UV32_00, regValRs[31:0] };
		JX2_UCIX_CONV_MOV:
			tRegOutVal	= regValRs;
		JX2_UCIX_CONV_EXTUB: tRegOutVal	=
			{ UV56_00, regValRs[ 7:0] };
		JX2_UCIX_CONV_EXTUW: tRegOutVal	=
			{ UV48_00, regValRs[15:0] };
		JX2_UCIX_CONV_EXTUL: tRegOutVal	=
			{ UV32_00, regValRs[31:0] };

		default: begin
		end
	endcase
end

endmodule
