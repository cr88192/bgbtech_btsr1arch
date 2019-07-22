/*
Compare Rs and Rt
*/

`include "CoreDefs.v"

module FpuCmp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmd,		regIdIxt,
	regValRs,	regValRt,
	regOutOK,	regOutSrT);

input			clock;
input			reset;

input[7:0]		opCmd;			//command opcode
input[7:0]		regIdIxt;		//ALU Index / Opcode Extension

input[63:0]		regValRs;		//Rs input value (FPR)
input[63:0]		regValRt;		//Rt input value (FPR)

output[1:0]		regOutOK;		//execute status
output			regOutSrT;

reg[1:0]		tRegOutOK;			//execute status
reg				tRegOutSrT;
reg				tRegOutSrT2;

assign	regOutOK	= tRegOutOK;
assign	regOutSrT	= tRegOutSrT2;

reg	tCmpEq;
reg	tCmpGt;

reg	tCmpSgEq;
reg	tCmpSgEqP;
reg	tCmpSgEqN;
reg	tCmpSgGt;
reg	tCmpSgLt;

reg	tCmpExEq;
reg	tCmpExGt;
reg	tCmpExLt;

reg	tCmpFraEq;
reg	tCmpFraGt;
reg	tCmpFraLt;

reg	tCmpFraEq0;
reg	tCmpFraEq1;
reg	tCmpFraEq2;
reg	tCmpFraGt0;
reg	tCmpFraGt1;
reg	tCmpFraGt2;

reg		tRsIsNaN;
reg		tRtIsNaN;

reg[18:0]	tCmpFraSub0;
reg[18:0]	tCmpFraSub1;
reg[16:0]	tCmpFraSub2;

always @*
begin
	tRegOutOK	= UMEM_OK_READY;
	tRegOutSrT	= 0;

	tRsIsNaN	= (regValRs[62:52] == 11'h7FF) &&
		(regValRs[51:48] != 0);
	tRtIsNaN	= (regValRt[62:52] == 11'h7FF) &&
		(regValRt[51:48] != 0);

	case({regValRs[63], regValRt[63]})
		2'b00: begin
			tCmpSgEq = 1;	tCmpSgGt = 0;	tCmpSgLt = 0;
			tCmpSgEqP = 1;	tCmpSgEqN = 0;
		end
		2'b01: begin
			tCmpSgEq = 0;	tCmpSgGt = 1;	tCmpSgLt = 0;
			tCmpSgEqP = 0;	tCmpSgEqN = 0;
		end
		2'b10: begin
			tCmpSgEq = 0;	tCmpSgGt = 0;	tCmpSgLt = 1;
			tCmpSgEqP = 0;	tCmpSgEqN = 0;
		end
		2'b11: begin
			tCmpSgEq = 1;	tCmpSgGt = 0;	tCmpSgLt = 0;
			tCmpSgEqP = 0;	tCmpSgEqN = 1;
		end
	endcase
	
	tCmpExEq	= (regValRs[62:52] == regValRt[62:52]);
	tCmpExGt	= (regValRs[62:52] >  regValRt[62:52]);
	tCmpExLt	= !tCmpExGt && !tCmpExEq;

//	tCmpFraEq = (regValRs[51:32] == regValRt[51:32]);
//	tCmpFraGt = (regValRs[51:32] >  regValRt[51:32]);

	tCmpFraSub0	= {1'b0, regValRt[51:34]} - {1'b0, regValRs[51:34]};
	tCmpFraSub1	= {1'b0, regValRt[33:16]} - {1'b0, regValRs[33:16]};
	tCmpFraSub2	= {1'b0, regValRt[15: 0]} - {1'b0, regValRs[15: 0]};
	tCmpFraEq0	= (tCmpFraSub0 == 0);
	tCmpFraEq1	= (tCmpFraSub1 == 0);
	tCmpFraEq2	= (tCmpFraSub2 == 0);
	tCmpFraGt0	= tCmpFraSub0[18];
	tCmpFraGt1	= tCmpFraSub1[18];
	tCmpFraGt2	= tCmpFraSub2[16];
	
	tCmpFraEq	= tCmpFraEq0 && tCmpFraEq1 && tCmpFraEq2;
	tCmpFraGt	= tCmpFraGt0 ||
		(tCmpFraGt1 && tCmpFraEq0) ||
		(tCmpFraGt2 && tCmpFraEq0 && tCmpFraEq1) ;
	tCmpFraLt	= !tCmpFraGt && !tCmpFraEq;
	
	tCmpEq	= tCmpSgEq && tCmpExEq && tCmpFraEq && !tRsIsNaN && !tRtIsNaN;
	
	tCmpGt	= tCmpSgGt ||
		(tCmpExGt && tCmpSgEqP) ||
		(tCmpFraGt && tCmpSgEqP && tCmpExEq) ||
		(tCmpExLt && tCmpSgEqN) ||
		(tCmpFraLt && tCmpSgEqN && tCmpExEq) ;
		
	case(regIdIxt[5:0])
		JX2_UCIX_FPU_CMPEQ: begin
			tRegOutSrT = tCmpEq;
		end
		JX2_UCIX_FPU_CMPGT: begin
			tRegOutSrT = tCmpGt;
		end
		JX2_UCIX_FPU_CMPNE: begin
			tRegOutSrT = !tCmpEq;
		end
		JX2_UCIX_FPU_CMPGE: begin
			tRegOutSrT = tCmpGt || tCmpEq;
		end
		
		default: begin
		end
	endcase
	
end

always @(posedge clock)
begin
	tRegOutSrT2		<= tRegOutSrT;
end

endmodule
