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

//		JX2_UCIX_CONV_MOVX:
//			tRegOutVal	= regValRs;

`ifndef def_true
		JX2_UCIX_CONV_CLZ:
			tRegOutVal	= { UV56_00, tClzVal };
		JX2_UCIX_CONV_CLZQ:
			tRegOutVal	= { UV56_00, tClzVal };
`endif

		JX2_UCIX_CONV_LDIHI: begin
//			tRegOutVal	= { UV32_00, regValRs[9:0], UV22_00};
			tRegOutVal	= { regValRs[41:0], UV22_00};
		end
		JX2_UCIX_CONV_LDIQHI: begin
			tRegOutVal	= { regValRs[9:0], UV54_00};
		end
		JX2_UCIX_CONV_LDIQHI16: begin
			tRegOutVal	= { regValRs[47:0], UV16_00};
		end
		JX2_UCIX_CONV_LDIQHI32: begin
			tRegOutVal	= { regValRs[31:0], UV32_00};
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

		JX2_UCIX_CONV_FNEG:
			tRegOutVal	= { ~regValRs[63], regValRs[62:0] };
		JX2_UCIX_CONV_FABS:
			tRegOutVal	= { 1'b0, regValRs[62:0] };

`ifdef def_true
		JX2_UCIX_CONV_RGBSHR1:
			tRegOutVal	= {
				{ 1'b0,
					1'b0, regValRs[62:59],
					1'b0, regValRs[57:54],
					1'b0, regValRs[52:49] },
				{ 1'b0,
					1'b0, regValRs[46:43],
					1'b0, regValRs[41:38],
					1'b0, regValRs[36:33] },
				{ 1'b0,
					1'b0, regValRs[30:27],
					1'b0, regValRs[25:22],
					1'b0, regValRs[20:17] },
				{ 1'b0,
					1'b0, regValRs[14:11],
					1'b0, regValRs[ 9: 6],
					1'b0, regValRs[ 4: 1] }
				};

		JX2_UCIX_CONV_RGB5PCK32:
		begin
			tRegOutVal	= {
				UV32_00,
				{ 1'b0,
					regValRs[55:51],
					regValRs[47:43],
					regValRs[39:35] },
				{ 1'b0,
					regValRs[23:19],
					regValRs[15:11],
					regValRs[ 7: 3] }
				};
				if(regValRs[31:28]!=4'hF)
				begin
					tRegOutVal[15]=1;
					tRegOutVal[10]=regValRs[31];
					tRegOutVal[ 5]=regValRs[30];
					tRegOutVal[ 0]=regValRs[29];
				end
				tRegOutVal[63:32] = tRegOutVal[31:0];
		end
		JX2_UCIX_CONV_RGB5UPCK32:
			tRegOutVal	= {
				{ (regValRs[31]) ?
						{ regValRs[26], regValRs[21], regValRs[16], UV5_00 } :
						UV8_FF,
					regValRs[30:26], regValRs[30:28],
					regValRs[25:21], regValRs[25:23],
					regValRs[20:16], regValRs[20:18] },
				{ (regValRs[15]) ?
						{ regValRs[10], regValRs[5], regValRs[0], UV5_00 } :
						UV8_FF,
					regValRs[14:10], regValRs[14:12],
					regValRs[ 9: 5], regValRs[ 9: 7],
					regValRs[ 4: 0], regValRs[ 4: 2] }
				};

		JX2_UCIX_CONV_RGB5PCK64:
		begin
			tRegOutVal	= {
				UV48_00,
				{ 1'b0,
					regValRs[47:43],
					regValRs[31:27],
					regValRs[15:11] }
				};
			if(regValRs[63:60]!=4'hF)
			begin
				tRegOutVal[15]=1;
				tRegOutVal[10]=regValRs[63];
				tRegOutVal[ 5]=regValRs[62];
				tRegOutVal[ 0]=regValRs[61];
			end
			tRegOutVal[31:16] = tRegOutVal[15:0];
			tRegOutVal[63:32] = tRegOutVal[31:0];
		end
		JX2_UCIX_CONV_RGB5UPCK64:
			tRegOutVal	= {
				(regValRs[15]) ?
						{ regValRs[10], regValRs[5], regValRs[0], UV13_00 } :
						UV16_FF,
					regValRs[14:10], regValRs[14:10], UV6_00,
					regValRs[ 9: 5], regValRs[ 9: 5], UV6_00,
					regValRs[ 4: 0], regValRs[ 4: 0], UV6_00
				};

		JX2_UCIX_CONV_RGB32PCK64:
		begin
			tRegOutVal	= {
				regValRs[63:56],
				regValRs[47:40],
				regValRs[31:24],
				regValRs[15: 8],
				regValRs[63:56],
				regValRs[47:40],
				regValRs[31:24],
				regValRs[15: 8]	};
		end
		JX2_UCIX_CONV_RGB32UPCK64:
		begin
			tRegOutVal	= {
				regValRs[31:24],
				regValRs[31:24],
				regValRs[23:16],
				regValRs[23:16],
				regValRs[15: 8],
				regValRs[15: 8],
				regValRs[ 7: 0],
				regValRs[ 7: 0]
			};
			
		end
`endif

		default: begin
		end
	endcase
end

endmodule
