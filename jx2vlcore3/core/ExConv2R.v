/*

Do Simple MOV+Conv style operation.

 */

// `include "ExOpClz.v"

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

`ifndef def_true
wire[7:0]		tClzVal;
ExOpClz	clz(0, 0,
	0, idUIxt,
	regValRs, tClzVal);
`endif

always @*
begin
	tRegOutVal	= regValRs;
	tRegOutSrT	= regInSrT;
	tRegOutVal	= UV64_XX;

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
		JX2_UCIX_CONV_NOT:
			tRegOutVal	= ~regValRs;

`ifndef def_true
		JX2_UCIX_CONV_CLZ:
			tRegOutVal	= { UV56_00, tClzVal };
		JX2_UCIX_CONV_CLZQ:
			tRegOutVal	= { UV56_00, tClzVal };
`endif

		JX2_UCIX_CONV_LDIHI: begin
			tRegOutVal	= { UV32_00, regValRs[9:0], UV22_00};
		end
		JX2_UCIX_CONV_LDIQHI: begin
			tRegOutVal	= { regValRs[9:0], UV54_00};
		end

// `ifdef def_true
`ifndef def_true
		JX2_UCIX_CONV_LDISIZ: begin
			case(regValRs[9:8])
				2'b00: tRegOutVal	= { UV56_00, regValRs[7:0]         };
				2'b01: tRegOutVal	= { UV48_00, regValRs[7:0], UV8_00 };
				2'b10: tRegOutVal	= { UV40_00, regValRs[7:0], UV16_00 };
				2'b11: tRegOutVal	= { UV32_00, regValRs[7:0], UV24_00 };
			endcase
		end
		JX2_UCIX_CONV_LDIQSIZ: begin
			case(regValRs[9:8])
				2'b00: tRegOutVal	= { UV24_00, regValRs[7:0], UV32_00 };
				2'b01: tRegOutVal	= { UV16_00, regValRs[7:0], UV40_00 };
				2'b10: tRegOutVal	= { UV8_00,  regValRs[7:0], UV48_00 };
				2'b11: tRegOutVal	= {          regValRs[7:0], UV56_00 };
			endcase
		end
		JX2_UCIX_CONV_LDISIN: begin
			case(regValRs[9:8])
				2'b00: tRegOutVal	= { UV56_FF, regValRs[7:0]          };
				2'b01: tRegOutVal	= { UV48_FF, regValRs[7:0], UV8_00  };
				2'b10: tRegOutVal	= { UV40_FF, regValRs[7:0], UV16_00 };
				2'b11: tRegOutVal	= { UV32_FF, regValRs[7:0], UV24_00 };
			endcase
		end
		JX2_UCIX_CONV_LDIQSIN: begin
			case(regValRs[9:8])
				2'b00: tRegOutVal	= { UV24_FF, regValRs[7:0], UV32_00 };
				2'b01: tRegOutVal	= { UV16_FF, regValRs[7:0], UV40_00 };
				2'b10: tRegOutVal	= { UV8_FF , regValRs[7:0], UV48_00 };
				2'b11: tRegOutVal	= {          regValRs[7:0], UV56_00 };
			endcase
		end
`endif
		default: begin
		end
	endcase
end

endmodule
