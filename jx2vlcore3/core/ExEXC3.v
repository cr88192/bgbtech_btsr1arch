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

/*
EX3 Stage (Top Level, Lane 2+)

Third Execute Stage.
Continues on the work from the first 2 stages.

 */

`include "CoreDefs.v"

module ExEXC3(
	clock,		reset,
	opUCmd,		opUIxt,
	exHold,

	regIdRs,		//Source A, ALU / Base
	regIdRt,		//Source B, ALU / Index
	regIdRm,		//Source C, MemStore / Dest
	regValRs,		//Source A Value
	regValRt,		//Source B Value
	regValRm,		//Source C Value / Dest

	regIdRn1,		//Destination ID (EX1)
	regValRn1,		//Destination Value (EX1)

	regIdRn2,		//Destination ID (EX2)
	regValRn2,		//Destination Value (EX2)
	
	regIdRn4,		//Destination ID (EX2)
	regValRn4,		//Destination Value (EX2)
	
	regValPc,		//PC Value (Synthesized)
	regValImm,		//Immediate (Decode)
	regValAluRes,	//ALU Result
	regValMulwRes,	//ALU Result
	regValKrreRes,	//Keyring Result

	regFpuGRn,		//FPU GPR Result
	opBraFlush,

	regInLastSr,	
	memDataIn,
	memDataInB
	);

input			clock;
input			reset;
input[8:0]		opUCmd;
input[8:0]		opUIxt;
output[1:0]		exHold;

`input_gpr		regIdRs;		//Source A, ALU / Base
`input_gpr		regIdRt;		//Source B, ALU / Index
`input_gpr		regIdRm;		//Source C, MemStore
`input_gprval		regValRs;		//Source A Value
`input_gprval		regValRt;		//Source B Value
`input_gprval		regValRm;		//Source C Value

`input_gpr		regIdRn1;		//Destination ID (EX1)
`input_gprval		regValRn1;		//Destination Value (EX1)

`output_gpr		regIdRn2;		//Destination ID (EX1)
`output_gprval	regValRn2;		//Destination Value (EX1)

`output_gpr		regIdRn4;		//Destination ID (EX1)
`output_gprval	regValRn4;		//Destination Value (EX1)

input[47:0]		regValPc;		//PC Value (Synthesized)
input[32:0]		regValImm;		//Immediate (Decode)

input[65:0]		regValAluRes;	//ALU Result
input[63:0]		regValMulwRes;	//MUL.W Result
input[65:0]		regValKrreRes;	//Keyring Result

input[63:0]		regFpuGRn;		//FPU GPR Result

input			opBraFlush;

input[ 7:0]		regInLastSr;

input[65:0]		memDataIn;
input[65:0]		memDataInB;

reg				tExHold;
reg				tRegHeld;
assign	exHold		= { tRegHeld, tExHold };

`reg_gpr		tRegIdRn2;
`reg_gprval		tRegValRn2;
`reg_gpr		tRegIdCn2;
`reg_gprval		tRegValCn2;

assign	regIdRn2	= tRegIdRn2;
assign	regValRn2	= tRegValRn2;

assign	regIdRn4	= tRegIdRn2;
assign	regValRn4	= tRegValRn2;


`reg_gprval	tValOutDfl;
reg			tDoOutDfl;

(* max_fanout = 50 *)
	reg[5:0]	tOpUCmd1;

// reg[3:0]	tHoldCyc;
reg			tOpEnable;

reg		tMsgLatch;
reg		tNextMsgLatch;


always @*
begin
	tRegIdRn2	= regIdRn1;		//Forward by default
	tRegValRn2	= regValRn1;	//Forward by default

	tExHold			= 0;
	tRegHeld		= 0;
	tNextMsgLatch	= 0;

	tValOutDfl		= UVGPRV_00;
	tDoOutDfl		= 0;

	tOpEnable	= !opBraFlush;
	tOpUCmd1	= tOpEnable ? opUCmd[5:0] : JX2_UCMD_NOP;

	case(tOpUCmd1)
		JX2_UCMD_NOP: begin
		end
		
		JX2_UCMD_OP_IXS: begin
		end
		
		JX2_UCMD_OP_IXT: begin
			case(opUIxt[5:0])
				JX2_UCIX_IXT_NOP: begin
				end

// `ifdef def_true
`ifndef def_true
				JX2_UCIX_IXT_LDEENC: begin
					tRegIdRn2	= JX2_GR_DHR;
					tRegValRn2	= regValKrreRes[63:0];
				end

				JX2_UCIX_IXT_SVEKRR: begin
					tRegIdRn2	= JX2_GR_DHR;
					tRegValRn2	= regValKrreRes[63:0];
				end
`endif

				default: begin
				end
			endcase
		end
		
		JX2_UCMD_MOV_IR: begin
		end
	
		JX2_UCMD_LEA_MR: begin
		end

		JX2_UCMD_MOV_RM: begin
		end
		JX2_UCMD_MOV_MR: begin
//			tRegIdRn2	= regIdRm;
//			tRegValRn2	= memDataInB;
		end

//		JX2_UCMD_ADDSP: begin
//		end

		JX2_UCMD_ALU3, JX2_UCMD_UNARY, JX2_UCMD_ALUW3: begin
		end

		JX2_UCMD_ALUCMP: begin
		end
		
		JX2_UCMD_MUL3: begin
		end

		JX2_UCMD_MULW3: begin
//			tRegIdRn2	= regIdRm;			//
//			tRegValRn2	= regValMulwRes;		//
		end

		JX2_UCMD_SHAD3: begin
		end
`ifndef jx2_merge_shadq
		JX2_UCMD_SHLD3: begin
		end
		JX2_UCMD_SHADQ3: begin
		end
		JX2_UCMD_SHLDQ3: begin
		end
`endif
		
		JX2_UCMD_CONV_RR: begin
		end
		
		JX2_UCMD_MOV_RC: begin
		end
		JX2_UCMD_MOV_CR: begin
		end

		JX2_UCMD_FPU3: begin
//			tRegIdRn2		= regIdRm;
//			tRegValRn2		= regFpuGRn;
		end

		JX2_UCMD_BLINT: begin
		end

		default: begin
			if(!tMsgLatch)
				$display("EX3C: Unhandled UCmd %X", opUCmd);
			tNextMsgLatch	= 1;
		end
	
	endcase

	if(tDoOutDfl)
	begin
		tRegIdRn2		= regIdRm;
		tRegValRn2		= tValOutDfl;
	end
	
	if(opBraFlush)
	begin
		tRegIdRn2	= JX2_GR_ZZR;
//		tRegIdRn4	= JX2_CR_ZZR;
	end

end

always @(posedge clock)
begin
	tMsgLatch	<= tNextMsgLatch;

//	if(tExHold)
//		tHoldCyc <= tHoldCyc + 1;
//	else
//		tHoldCyc <= 0;
end

endmodule
