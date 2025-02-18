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

module RegCR(
	clock,
	reset,
	hold,

	regIdCm,		//Source ID
	regValCm,		//Source Value

	regIdCn1,		//Destination ID (EX1)
	regValCn1,		//Destination Value (EX1)
	regIdCn2,		//Destination ID (EX2)
	regValCn2,		//Destination Value (EX2)
	regIdCn3,		//Destination ID (EX3)
	regValCn3,		//Destination Value (EX3)

	regEx1Flush,
	regEx2Flush,
	regEx3Flush,

	gprValPc,		//PC Value (Synthesized)

	regOutPc,	regInPc,
	regOutLr,	regInLr,
	regOutSr,	regInSr,

	regOutExsr,		regInExsr,
	regOutSpc,		regInSpc,
	regOutSsp,		regInSsp,
	regOutTea,		regInTea,
	regOutTeaHi,	regInTeaHi,
	regOutFpsr,		regInFpsr,

`ifdef jx2_enable_vaddr96
	regOutPcHi,		regInPcHi,
	regOutLrHi,		regInLrHi,
	regOutSpcHi,	regInSpcHi,
`endif

	regOutVbr,
	regOutGbr,
	regOutTbr,

`ifdef jx2_enable_vaddr96
	regOutVbrHi,
	regOutGbrHi,
//	regOutTbrHi,
`endif

	regOutMmcr,
	regOutKrr,
	regOutVipt,
	regOutVbrCm
	);

input			clock;
input			reset;
input			hold;

`input_gpr		regIdCm;		//Source C, MemStore
`output_gprval	regValCm;

`input_gpr		regIdCn1;		//Destination ID
`input_gprval	regValCn1;		//Destination Value
`input_gpr		regIdCn2;		//Destination ID
`input_gprval	regValCn2;		//Destination Value

input			regEx1Flush;
input			regEx2Flush;

`input_gpr		regIdCn3;		//Destination ID
`input_gprval	regValCn3;		//Destination Value
input			regEx3Flush;

input [47:0]	gprValPc;		//PC Value (Synthesized)

output[47:0]	regOutPc;
input [47:0]	regInPc;
output[63:0]	regOutLr;
input [63:0]	regInLr;

output[63:0]	regOutSr;
input [63:0]	regInSr;

output[63:0]	regOutExsr;
input [63:0]	regInExsr;
output[47:0]	regOutSpc;
input [47:0]	regInSpc;
output[47:0]	regOutSsp;
input [47:0]	regInSsp;
output[63:0]	regOutTea;
input [63:0]	regInTea;
output[63:0]	regOutTeaHi;
input [63:0]	regInTeaHi;

output[15:0]	regOutFpsr;
input [15:0]	regInFpsr;

output[47:0]	regOutVbr;
// output[47:0]	regOutGbr;
output[63:0]	regOutGbr;
output[47:0]	regOutTbr;
output[63:0]	regOutMmcr;
output[63:0]	regOutKrr;
output[47:0]	regOutVipt;

output[15:0]	regOutVbrCm;

`ifdef jx2_enable_vaddr96

output[47:0]	regOutPcHi;
input [47:0]	regInPcHi;
output[47:0]	regOutLrHi;
input [47:0]	regInLrHi;
output[47:0]	regOutSpcHi;
input [47:0]	regInSpcHi;

output[47:0]	regOutVbrHi;
output[47:0]	regOutGbrHi;
// output[47:0]	regOutTbrHi;

`endif

parameter		disableTlb = 0;

(* max_fanout = 200 *)
	wire		holdN = !hold;

(* max_fanout = 200 *)
	reg[63:0]	crRegSr;
(* max_fanout = 200 *)
	reg[63:0]	crRegExsr;

reg[47:0]	crRegPc;

(* max_fanout = 200 *)
	reg[63:0]	crRegLr;

reg[47:0]	crRegVbr;
reg[47:0]	crRegSpc;
reg[47:0]	crRegSsp;
reg[47:0]	crRegGbr;
reg[47:0]	crRegTbr;
reg[63:0]	crRegTea;
reg[63:0]	crRegTeaHi;
reg[15:0]	crRegFpsr;
reg[15:0]	crRegVbrCm;

`ifdef jx2_enable_mmu
// reg[47:0]	crRegTtb;
reg[63:0]	crRegTtb;
reg[63:0]	crRegMmcr;
reg[47:0]	crRegSttb;
reg[63:0]	crRegKrr;
reg[47:0]	crRegVipt;
`endif

`ifdef jx2_enable_vaddr96
reg[47:0]	crRegPcHi;
reg[47:0]	crRegLrHi;
reg[47:0]	crRegSpcHi;

reg[47:0]	crRegVbrHi;
reg[47:0]	crRegGbrHi;
// reg[47:0]	crRegTbrHi;
`endif

reg[63:0]	tRegValCm;

`ifdef jx2_enable_memcap
assign	regValCm = { 2'b00, tRegValCm };
`else
assign	regValCm = tRegValCm;
`endif

(* max_fanout = 200 *)
	reg[63:0]	crRegLr2;

assign	regOutPc	= crRegPc;
assign	regOutLr	= crRegLr;
// assign	regOutLr	= crRegLr2;
assign	regOutSr	= crRegSr;
assign	regOutExsr	= crRegExsr;
assign	regOutSpc	= crRegSpc;
assign	regOutSsp	= crRegSsp;

assign	regOutVbr	= crRegVbr;
// assign	regOutGbr	= crRegGbr;
assign	regOutGbr	= { crRegFpsr, crRegGbr };
assign	regOutTbr	= crRegTbr;
assign	regOutTea	= crRegTea;
assign	regOutTeaHi	= crRegTeaHi;
assign	regOutFpsr	= crRegFpsr;

assign	regOutVbrCm	= crRegVbrCm;

`ifdef jx2_enable_mmu
// assign	regOutMmcr	= crRegMmcr;
assign	regOutMmcr	= { crRegTtb[63:48], crRegMmcr[47:0] };
assign	regOutKrr	= crRegKrr;
`ifdef jx2_enable_mmu_vipt
assign	regOutVipt	= crRegVipt;
`else
assign	regOutVipt	= UV48_00;
`endif
`else
assign	regOutMmcr	= UV64_00;
assign	regOutKrr	= UV64_00;
assign	regOutVipt	= UV48_00;
`endif

`ifdef jx2_enable_vaddr96

assign	regOutPcHi	= crRegPcHi;
assign	regOutLrHi	= crRegLrHi;

assign	regOutSpcHi	= crRegSpcHi;

assign	regOutVbrHi	= crRegVbrHi;
assign	regOutGbrHi	= crRegGbrHi;
// assign	regOutTbrHi	= crRegTbrHi;

`endif

reg[63:0]	tValCmA;
reg			tValCmZz;

reg			tIsIsrEdge;

`reg_gpr		regIdCn2B;			//Destination ID
reg[63:0]		regValCn2B;			//Destination Value
reg[47:0]		regValCn2B_48b;		//Destination Value
reg[15:0]		regValCn2B_16h;		//Destination Value

(* max_fanout = 200 *)
	reg[63:0]		tRegInLr;
(* max_fanout = 200 *)
	reg				tResetL;

always @*
begin

	crRegLr2 = crRegLr;

//	if(({1'b1, regIdCn3}==JX2_CR_LR) && !regEx3Flush)
//		crRegLr2=regValCn3[47:0];
//	if(({1'b1, regIdCn2}==JX2_CR_LR) && !regEx2Flush)
//		crRegLr2=regValCn2[47:0];
//	if(({1'b1, regIdCn1}==JX2_CR_LR) && !regEx1Flush)
//		crRegLr2=regValCn1[47:0];

end


always @*
begin

	tValCmZz=0;

	tRegInLr	= regInLr;

// `ifdef def_true
`ifndef def_true
//	if((regValCn3[31:0]==32'h55BAADAA) && !regEx3Flush)
	if(regValCn3[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX3 Bad, Not Flushed");
	end
//	if((regValCn2[31:0]==32'h55BAADAA) && !regEx2Flush)
	if(regValCn2[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX2 Bad, Not Flushed");
	end
//	if((regValCn1[31:0]==32'h55BAADAA) && !regEx1Flush)
	if(regValCn1[31:0]==32'h55BAADAA)
	begin
		$display("RegCR: EX1 Bad, Not Flushed");
	end
`endif

	tIsIsrEdge	= regInSr[28] ^ crRegSr[28];


//	regIdCn2B	= { 1'b1, regIdCn3 };
	regIdCn2B	= regIdCn3;
	regValCn2B	= regValCn3[63:0];

	if(regEx3Flush)
	begin
		regIdCn2B	= JX2_CR_ZZR;
	end

`ifdef jx2_enable_vaddr48
//	regValCn2B_48b	= regValCn2[47:0];
	regValCn2B_48b	= regValCn2B[47:0];
`else
//	regValCn2B_48b	= { UV16_00, regValCn2[31:0] };
	regValCn2B_48b	= { UV16_00, regValCn2B[31:0] };
`endif

	regValCn2B_16h	= regValCn2B[63:48];

//	case({1'b1, regIdCm})
	case(regIdCm)
		JX2_CR_SR:		tValCmA=crRegSr;
		JX2_CR_EXSR:	tValCmA=crRegExsr;

`ifdef jx2_enable_vaddr48
//		JX2_CR_PC:		tValCmA={UV16_00, crRegPc};
		JX2_CR_PC:		tValCmA={UV16_00, gprValPc};
//		JX2_CR_LR:		tValCmA={UV16_00, crRegLr};
		JX2_CR_LR:		tValCmA=crRegLr;
//		JX2_CR_VBR:		tValCmA={UV16_00, crRegVbr};
		JX2_CR_VBR:		tValCmA={crRegVbrCm, crRegVbr};
		JX2_CR_SPC:		tValCmA={UV16_00, crRegSpc};
		JX2_CR_SSP:		tValCmA={UV16_00, crRegSsp};
//		JX2_CR_GBR:		tValCmA={UV16_00, crRegGbr};
		JX2_CR_GBR:		tValCmA={crRegFpsr, crRegGbr};
		JX2_CR_TBR:		tValCmA={UV16_00, crRegTbr};
//		JX2_CR_TEA:		tValCmA={UV16_00, crRegTea};
		JX2_CR_TEA:		tValCmA=crRegTea;

`ifdef jx2_enable_vaddr96
		JX2_CR_TEAH:	tValCmA=crRegTeaHi;
`endif

`else
//		JX2_CR_PC:		tValCmA={UV32_00, crRegPc[31:0]};
		JX2_CR_PC:		tValCmA={UV32_00, gprValPc[31:0]};
		JX2_CR_LR:		tValCmA={UV32_00, crRegLr[31:0]};
//		JX2_CR_VBR:		tValCmA={UV32_00, crRegVbr[31:0]};
		JX2_CR_VBR:		tValCmA={crRegVbrCm, UV16_00, crRegVbr[31:0]};
		JX2_CR_SPC:		tValCmA={UV32_00, crRegSpc[31:0]};
		JX2_CR_SSP:		tValCmA={UV32_00, crRegSsp[31:0]};
//		JX2_CR_GBR:		tValCmA={UV32_00, crRegGbr[31:0]};
		JX2_CR_GBR:		tValCmA={crRegFpsr, UV16_00, crRegGbr[31:0]};
		JX2_CR_TBR:		tValCmA={UV32_00, crRegTbr[31:0]};
//		JX2_CR_TEA:		tValCmA={UV32_00, crRegTea[31:0]};
		JX2_CR_TEA:		tValCmA=crRegTea;
//		JX2_CR_TEAH:	tValCmA=crRegTeaHi;
`endif

`ifdef jx2_enable_mmu
//		JX2_CR_TTB:		tValCmA={UV32_00, crRegTtb[31:0]};
//		JX2_CR_TTB:		tValCmA={UV16_00, crRegTtb[47:0]};
		JX2_CR_TTB:		tValCmA=crRegTtb;
		JX2_CR_MMCR:	tValCmA=crRegMmcr;
//		JX2_CR_STTB:	tValCmA={UV32_00, crRegSttb[31:0]};
		JX2_CR_STTB:	tValCmA={UV16_00, crRegSttb[47:0]};
		JX2_CR_KRR:		tValCmA=crRegKrr;

`ifdef jx2_enable_xgpr
`ifdef jx2_enable_mmu_vipt
		JX2_CR_VIPT:	tValCmA={UV16_00, crRegVipt};
`endif
`endif

`else
		JX2_CR_TTB:		tValCmA=UV64_00;
		JX2_CR_MMCR:	tValCmA=UV64_00;
		JX2_CR_STTB:	tValCmA=UV64_00;
		JX2_CR_KRR:		tValCmA=UV64_00;
`endif

//		JX2_CR_IMM:	begin
//			tValCmA=UV64_00;
//			tValCmZz=1;
//		end

		JX2_CR_ZZR, JX2_CR_IMM:	begin
			tValCmA=UV64_00;
			tValCmZz=1;
		end
		
		default: begin
//			$display("Reg CR: Unhandled %X", regIdCm);
			tValCmA=UV64_00;
		end
	endcase
	
	tRegValCm = tValCmA;

`ifndef def_true
// `ifdef def_true
	if(!tValCmZz)
	begin
		if(regIdCm==regIdCn3)
			tRegValCm=regValCn3;
		if(regIdCm==regIdCn2)
			tRegValCm=regValCn2;
		if(regIdCm==regIdCn1)
			tRegValCm=regValCn1;
	end
`endif
end

always @(posedge clock)
begin
	tResetL		<= reset;

	if(tResetL)
	begin
//		crRegPc		<= UV48_00;
//		crRegLr		<= UV48_00;
//		crRegVbr	<= UV48_00;
//		crRegGbr	<= UV48_00;
//		crRegTbr	<= UV48_00;

		crRegSr		<= UV64_00;
		crRegSr[30]	<= 1;

`ifdef jx2_enable_mmu
		crRegMmcr	<= UV64_00;
		crRegKrr	<= UV64_00;
//		crRegVipt	<= UV48_00;

//		crRegTtb	<= UV48_00;
//		crRegSttb	<= UV48_00;
`endif

`ifdef jx2_enable_vaddr96
		crRegPcHi		<= UV48_00;
		crRegLrHi		<= UV48_00;
		crRegGbrHi		<= UV48_00;
		crRegVbrHi		<= UV48_00;
`endif

	end
	else
//		if(!hold)
		if(holdN)
//		if(!hold || tIsIsrEdge)
	begin
		crRegSr		<= (regIdCn2B==JX2_CR_SR  ) ? regValCn2B[63:0] : regInSr;
		crRegExsr	<= (regIdCn2B==JX2_CR_EXSR) ? regValCn2B[63:0] : regInExsr;

`ifdef jx2_enable_vaddr48
		crRegPc		<= regInPc;
`else
		crRegPc		<= { UV16_00, regInPc[31:0] };
`endif

`ifndef def_true
		crRegPc		<= regInPc;
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B[47:0] : regInLr;
		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B[63:0] : regInLr;
		crRegSpc	<= (regIdCn2B==JX2_CR_SPC ) ? regValCn2B[47:0] : regInSpc;
		crRegSsp	<= (regIdCn2B==JX2_CR_SSP ) ? regValCn2B[47:0] : regInSsp;
//		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B[47:0] : regInTea;
		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B[63:0] : regInTea;
		crRegTeaHi	<= (regIdCn2B==JX2_CR_TEAH) ? regValCn2B[63:0] : regInTeaHi;
		crRegVbr	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2B[47:0] : crRegVbr;
		crRegGbr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2B[47:0] : crRegGbr;
		crRegTbr	<= (regIdCn2B==JX2_CR_TBR ) ? regValCn2B[47:0] : crRegTbr;
`endif

`ifdef def_true
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B_48b : regInLr;
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B_48b : tRegInLr;
//		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B     : tRegInLr;
		crRegLr		<= (regIdCn2B==JX2_CR_LR  ) ? regValCn2B     : crRegLr;
		crRegSpc	<= (regIdCn2B==JX2_CR_SPC ) ? regValCn2B_48b : regInSpc;
		crRegSsp	<= (regIdCn2B==JX2_CR_SSP ) ? regValCn2B_48b : regInSsp;
//		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B_48b : regInTea;
		crRegTea	<= (regIdCn2B==JX2_CR_TEA ) ? regValCn2B     : regInTea;

`ifdef jx2_enable_vaddr96
		crRegTeaHi	<= (regIdCn2B==JX2_CR_TEAH) ? regValCn2B     : regInTeaHi;
`else
		crRegTeaHi	<= 0;
`endif

		crRegVbr	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2B_48b : crRegVbr;
		crRegGbr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2B_48b : crRegGbr;
		crRegTbr	<= (regIdCn2B==JX2_CR_TBR ) ? regValCn2B_48b : crRegTbr;
		crRegFpsr	<= (regIdCn2B==JX2_CR_GBR ) ? regValCn2B_16h : regInFpsr;
		crRegVbrCm	<= (regIdCn2B==JX2_CR_VBR ) ? regValCn2B_16h : crRegVbrCm;
`endif

		if(!disableTlb)
		begin
`ifdef jx2_enable_mmu
			crRegTtb	<= (regIdCn2B==JX2_CR_TTB ) ?
				regValCn2B[63:0] : crRegTtb;
			crRegMmcr	<= (regIdCn2B==JX2_CR_MMCR) ?
				regValCn2B[63:0] : crRegMmcr;
			crRegSttb	<= (regIdCn2B==JX2_CR_STTB) ?
				regValCn2B[47:0] : crRegSttb;
			crRegKrr	<= (regIdCn2B==JX2_CR_KRR ) ?
				regValCn2B[63:0] : crRegKrr;

`ifdef jx2_enable_xgpr
`ifdef jx2_enable_mmu_vipt
			crRegVipt	<= (regIdCn2B==JX2_CR_VIPT) ?
				regValCn2B[47:0] : crRegVipt;
`else
			crRegVipt	<= 0;
`endif
`else
			crRegVipt	<= 0;
`endif

`endif

`ifdef jx2_enable_vaddr96
			crRegPcHi	<= (regIdCn2B==JX2_GR_PC_HI ) ?
				regValCn2B_48b : crRegPcHi;
//			crRegLrHi	<= (regIdCn2B==JX2_GR_LR_HI ) ?
//				regValCn2B_48b : crRegLrHi;
			crRegLrHi	<= 0;
			crRegGbrHi	<= (regIdCn2B==JX2_GR_GBR_HI) ?
				regValCn2B_48b : crRegGbrHi;
			crRegVbrHi	<= (regIdCn2B==JX2_GR_VBR_HI) ?
					regValCn2B_48b : crRegVbrHi;
			crRegSpcHi	<= (regIdCn2B==JX2_GR_SPC_HI) ?
				regValCn2B_48b : crRegSpcHi;
`endif
		end
		else
		begin
`ifdef jx2_enable_mmu
			crRegTtb	<= 0;
			crRegMmcr	<= 0;
			crRegSttb	<= 0;
			crRegKrr	<= 0;
			crRegVipt	<= 0;
`endif

`ifdef jx2_enable_vaddr96
			crRegPcHi	<= 0;
			crRegLrHi	<= 0;
			crRegGbrHi	<= 0;
			crRegVbrHi	<= 0;
			crRegSpcHi	<= 0;
`endif
		end

	end
end

endmodule
