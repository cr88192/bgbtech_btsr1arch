/*

Do Simple MOV+Conv style operation.

 */

// `include "ExOpClz.v"

`ifndef jx2_do_convfp16_alu

`ifdef jx2_enable_convrgb32f
`include "ExConv_Fp8Exp12.v"
`include "ExConv_Fp12Pck8.v"
`endif

`ifdef jx2_enable_convrgb30a
`include "ExConv_Rgb30aExp.v"
// `include "ExConv_Rgb30aPck.v"
`endif

`ifdef jx2_enable_convfp16
`include "ExConv_Fp16Exp32.v"
`include "ExConv_Fp32Pck16.v"
`endif

`endif

module ExConv2R(
	/* verilator lint_off UNUSED */
	regValRs,
	idUIxt,
	regInSrT,
	regOutVal,
	regOutSrT
	);

input[63:0]		regValRs;
input[8:0]		idUIxt;
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

reg[31:0]	tRegRgb5Upck32;


`ifndef jx2_do_convfp16_alu

wire[1:0]		tFp8ExpIsSign;
assign		tFp8ExpIsSign = { 1'b0, idUIxt[0] };

`ifdef jx2_enable_convrgb32f
wire[63:0]	tRegRgb32Upck64F;
ExConv_Fp8Exp12		conv_exp32a(
	regValRs[ 7: 0], tRegRgb32Upck64F[15: 4], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32b(
	regValRs[15: 8], tRegRgb32Upck64F[31:20], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32c(
	regValRs[23:16], tRegRgb32Upck64F[47:36], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32d(
	regValRs[31:24], tRegRgb32Upck64F[63:52], tFp8ExpIsSign);
assign	tRegRgb32Upck64F[ 3: 0]=0;
assign	tRegRgb32Upck64F[19:16]=0;
assign	tRegRgb32Upck64F[35:32]=0;
assign	tRegRgb32Upck64F[51:48]=0;

wire[31:0]	tRegRgb32Pck64F;
ExConv_Fp12Pck8		conv_pck32a(
	regValRs[15: 4], tRegRgb32Pck64F[ 7: 0], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32b(
	regValRs[31:20], tRegRgb32Pck64F[15: 8], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32c(
	regValRs[47:36], tRegRgb32Pck64F[23:16], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32d(
	regValRs[63:52], tRegRgb32Pck64F[31:24], tFp8ExpIsSign);

`endif

`ifdef jx2_enable_convrgb30a
wire[63:0]	tRegRgb30aUpck64F;
ExConv_Rgb30aExp	conv_upck30a(
	regValRs[31:0], regValRs[31:30],
	tRegRgb30aUpck64F);

//wire[31:0]	tRegRgb30aPck64F;
//ExConv_Rgb30aPck	conv_pck30a(
//	regValRs[63:0], tRegRgb30aPck64F);
`endif

`ifdef jx2_enable_convfp16
//wire[63:0]	tRegFp16Upck32L;
//wire[63:0]	tRegFp16Upck32H;
wire[63:0]	tRegFp16Upck32;
wire[31:0]	tRegFp32Pck16;

wire[31:0]	tRegFp16UPckT = idUIxt[0] ? regValRs[63:32] : regValRs[31: 0];
ExConv_Fp16Exp32	conv_fp16upcka(tRegFp16UPckT[15: 0], tRegFp16Upck32[31: 0]);
ExConv_Fp16Exp32	conv_fp16upckb(tRegFp16UPckT[31:16], tRegFp16Upck32[63:32]);

ExConv_Fp32Pck16	conv_fp16pcka(regValRs[31: 0], tRegFp32Pck16[15: 0]);
ExConv_Fp32Pck16	conv_fp16pckb(regValRs[63:32], tRegFp32Pck16[31:16]);
`endif

`endif


always @*
begin
	tRegOutVal	= regValRs;
	tRegOutSrT	= regInSrT;
	tRegOutVal	= UV64_XX;

	tRegRgb5Upck32	=
		{ (regValRs[15]) ?
				{ regValRs[10], regValRs[5], regValRs[0], UV5_00 } :
				UV8_FF,
			regValRs[14:10], regValRs[14:12],
			regValRs[ 9: 5], regValRs[ 9: 7],
			regValRs[ 4: 0], regValRs[ 4: 2] };

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
`ifndef def_true
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
`endif

			tRegOutVal	= {
				UV48_00,
				{ 1'b0,
					1'b0, regValRs[14:11],
					1'b0, regValRs[ 9: 6],
					1'b0, regValRs[ 4: 1] }
				};

		JX2_UCIX_CONV_RGB5PCK32:
		begin
			tRegOutVal	= {
//				UV32_00,
//				{ 1'b0,
//					regValRs[55:51],
//					regValRs[47:43],
//					regValRs[39:35] },
				UV48_00,
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
//				tRegOutVal[63:32] = tRegOutVal[31:0];
		end
		JX2_UCIX_CONV_RGB5UPCK32:
		begin
`ifndef def_true
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
`endif

			tRegOutVal	= { UV32_00, tRegRgb5Upck32 };
		end

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
//			tRegOutVal[31:16] = tRegOutVal[15:0];
//			tRegOutVal[63:32] = tRegOutVal[31:0];
		end
		JX2_UCIX_CONV_RGB5UPCK64: begin
`ifndef def_true
			tRegOutVal	= {
				(regValRs[15]) ?
						{ regValRs[10], regValRs[5], regValRs[0], UV13_00 } :
						UV16_FF,
					regValRs[14:10], regValRs[14:10], UV6_00,
					regValRs[ 9: 5], regValRs[ 9: 5], UV6_00,
					regValRs[ 4: 0], regValRs[ 4: 0], UV6_00
				};
`endif

`ifdef def_true
			tRegOutVal	= {
				tRegRgb5Upck32[31:24],
				tRegRgb5Upck32[31:24],
				tRegRgb5Upck32[23:16],
				tRegRgb5Upck32[23:16],
				tRegRgb5Upck32[15: 8],
				tRegRgb5Upck32[15: 8],
				tRegRgb5Upck32[ 7: 0],
				tRegRgb5Upck32[ 7: 0]
			};
`endif
		end

		JX2_UCIX_CONV_RGB32PCK64:
		begin
`ifndef def_true
			tRegOutVal	= {
				regValRs[63:56],
				regValRs[47:40],
				regValRs[31:24],
				regValRs[15: 8],
				regValRs[63:56],
				regValRs[47:40],
				regValRs[31:24],
				regValRs[15: 8]	};
`endif
`ifdef def_true
			tRegOutVal	= {
				UV32_00,
				regValRs[63:56],
				regValRs[47:40],
				regValRs[31:24],
				regValRs[15: 8]	};
`endif
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

`ifndef jx2_do_convfp16_alu

`ifdef jx2_enable_convrgb32f
		JX2_UCIX_CONV_RGB32PCK64FU: begin
			tRegOutVal = { UV32_00, tRegRgb32Pck64F };
		end
		JX2_UCIX_CONV_RGB32PCK64FS: begin
			tRegOutVal = { UV32_00, tRegRgb32Pck64F };
		end
		JX2_UCIX_CONV_RGB32UPCK64FU: begin
			tRegOutVal = tRegRgb32Upck64F;
		end
		JX2_UCIX_CONV_RGB32UPCK64FS: begin
			tRegOutVal = tRegRgb32Upck64F;
		end
`endif

`ifdef jx2_enable_convrgb30a
		JX2_UCIX_CONV_RGB30APCK64F: begin
//			tRegOutVal = { UV32_00, tRegRgb30aPck64F };
		end
		JX2_UCIX_CONV_RGB30AUPCK64F: begin
			tRegOutVal = tRegRgb30aUpck64F;
		end
`endif

`ifdef jx2_enable_convfp16
		JX2_UCIX_CONV_FP16UPCK32L: begin
//			tRegOutVal = tRegFp16Upck32L;
			tRegOutVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16UPCK32H: begin
//			tRegOutVal = tRegFp16Upck32H;
			tRegOutVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16PCK32: begin
			tRegOutVal = { UV32_00, tRegFp32Pck16 };
		end
`endif

`endif

		default: begin
		end
	endcase
end

endmodule
