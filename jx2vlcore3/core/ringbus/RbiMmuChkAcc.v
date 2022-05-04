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
Determine whether access can be allowed to a given page.

AccMode:
  [31:16]=VUGID
  [15: 4]=KR Access
  [ 3: 0]=Base Access
    3=NU (Not User)
    2=NX (No Execute)
    1=NW (No Write)
    0=NR (No Read)

NoRwx:
    5=Hold (Not-Ready)
    4=NU (Not User)
    3=NC (No Cache)
    2=NX (No Execute)
    1=NW (No Write)
    0=NR (No Read)

 */
 
`ifndef HAS_RBIMMUCHKACC
`define HAS_RBIMMUCHKACC

module RbiMmuChkAcc(
	clock, reset,
	regInHold,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbInAcc,
	aclEntryA,	aclEntryB,
	aclEntryC,	aclEntryD,
	accOutExc,
	regOutNoRwx
	);

/* verilator lint_off UNUSED */

input			clock;
input			reset;
input			regInHold;

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register

input[7:0]		regInOpm;		//Operation Size/Type
input[35:0]		tlbInAcc;		//TLB Access Mode

input[47:0]		aclEntryA;
input[47:0]		aclEntryB;
input[47:0]		aclEntryC;
input[47:0]		aclEntryD;

output[15:0]	accOutExc;		//Output Exception Code
output[5:0]		regOutNoRwx;	//No R/W/X

reg[15:0]		tAccOutExc2;	//Output Exception Code
assign			accOutExc = tAccOutExc2;
// assign			accOutExc = tAccOutExc;

reg[15:0]		tAccOutExc;		//Output Exception Code

reg[5:0]		tRegOutNoRwx;	//No R/W/X
reg[5:0]		tRegOutNoRwx2;	//No R/W/X
// assign			regOutNoRwx = tRegOutNoRwx2;
assign			regOutNoRwx = tRegOutNoRwx;

reg			tVugidEnA;
reg			tVugidEnB;
reg			tVugidEnC;
reg			tVugidEnD;
reg[15:0]	tKrrA;
reg[15:0]	tKrrB;
reg[15:0]	tKrrC;
reg[15:0]	tKrrD;
reg[15:0]	tVugid;

reg			tKrrGrpEqA;
reg			tKrrGrpEqB;
reg			tKrrGrpEqC;
reg			tKrrGrpEqD;
reg			tKrrGrpEq;
reg			tKrrGrpEqFw;		//Forward
reg			tKrrGrpEqRv;		//Reverse

reg			tKrrUsrEqA;
reg			tKrrUsrEqB;
reg			tKrrUsrEqC;
reg			tKrrUsrEqD;
reg			tKrrUsrEq;

reg			tAclTlbMatchA;
reg			tAclTlbMatchB;
reg			tAclTlbMatchC;
reg			tAclTlbMatchD;

reg			tAclKrrMatchA;
reg			tAclKrrMatchB;
reg			tAclKrrMatchC;
reg			tAclKrrMatchD;

reg			tAclUse;
reg			tAclUseA;
reg			tAclUseB;
reg			tAclUseC;
reg			tAclUseD;

reg[11:0]	tAccMode;

reg[2:0]	tKrrAccFl;
reg[2:0]	tNextKrrAccFl;

reg			tUsDeny;
reg			tlbMmuEnable;

reg[35:0]		tTlbInAcc;		//TLB Access Mode
reg[63:0]		tRegInKRR;		//Keyring Register
reg[7:0]		tRegInOpm;		//Operation Size/Type


/* verilator lint_on UNUSED */


always @*
begin
	tAccOutExc		= 0;
	tRegOutNoRwx	= 0;

	tlbMmuEnable	= regInMMCR[0];

	if(regInSR[29] && regInSR[28])
	begin
//		$display("TLB Disable ISR");
		tlbMmuEnable = 0;
	end

	tKrrA		= tRegInKRR[15: 0];
	tKrrB		= tRegInKRR[31:16];
	tKrrC		= tRegInKRR[47:32];
	tKrrD		= tRegInKRR[63:48];
	tVugidEnA	= (tKrrA != 16'h0000);
	tVugidEnB	= (tKrrB != 16'h0000);
	tVugidEnC	= (tKrrC != 16'h0000);
	tVugidEnD	= (tKrrD != 16'h0000);
	tVugid		= tTlbInAcc[31:16];
	tAccMode	= tTlbInAcc[15: 4];

`ifdef jx2_enable_mmu_acl

	tAclKrrMatchA	=
		(aclEntryA[31:16] == tKrrA) ||
		(aclEntryA[31:16] == tKrrB) ||
		(aclEntryA[31:16] == tKrrC) ||
		(aclEntryA[31:16] == tKrrD)	;
	tAclKrrMatchB	=
		(aclEntryB[31:16] == tKrrA) ||
		(aclEntryB[31:16] == tKrrB) ||
		(aclEntryB[31:16] == tKrrC) ||
		(aclEntryB[31:16] == tKrrD)	;
	tAclKrrMatchC	=
		(aclEntryC[31:16] == tKrrA) ||
		(aclEntryC[31:16] == tKrrB) ||
		(aclEntryC[31:16] == tKrrC) ||
		(aclEntryC[31:16] == tKrrD)	;
	tAclKrrMatchD	=
		(aclEntryD[31:16] == tKrrA) ||
		(aclEntryD[31:16] == tKrrB) ||
		(aclEntryD[31:16] == tKrrC) ||
		(aclEntryD[31:16] == tKrrD)	;

	tAclTlbMatchA = (aclEntryA[15:0] == tTlbInAcc[31:16]);
	tAclTlbMatchB = (aclEntryB[15:0] == tTlbInAcc[31:16]);
	tAclTlbMatchC = (aclEntryC[15:0] == tTlbInAcc[31:16]);
	tAclTlbMatchD = (aclEntryD[15:0] == tTlbInAcc[31:16]);

	tAclUse = 0;
	tAclUseA = 0;
	tAclUseB = 0;
	tAclUseC = 0;
	tAclUseD = 0;

	if(tTlbInAcc[5])
	begin
		if(tAclTlbMatchA && tAclKrrMatchA)
		begin
			tAclUseA	= 1;
		end
		else if(tAclTlbMatchB && tAclKrrMatchB)
		begin
			tAclUseB	= 1;
		end
		else if(tAclTlbMatchC && tAclKrrMatchC)
		begin
			tAclUseC	= 1;
		end
		else if(tAclTlbMatchD && tAclKrrMatchD)
		begin
			tAclUseD	= 1;
		end

		if(tAclUseA && (aclEntryA[34:32]!=0))
		begin
			tVugid		= aclEntryA[31:16];
			tAccMode	= aclEntryA[43:32];
			tAclUse		= 1;
		end
		else if(tAclUseB && (aclEntryB[34:32]!=0))
		begin
			tVugid		= aclEntryB[31:16];
			tAccMode	= aclEntryB[43:32];
			tAclUse		= 1;
		end
		else if(tAclUseC && (aclEntryC[34:32]!=0))
		begin
			tVugid		= aclEntryC[31:16];
			tAccMode	= aclEntryC[43:32];
			tAclUse		= 1;
		end
		else if(tAclUseD && (aclEntryD[34:32]!=0))
		begin
			tVugid		= aclEntryD[31:16];
			tAccMode	= aclEntryD[43:32];
			tAclUse		= 1;
		end
	end

`endif


//	tVugidEnA	= 0;
	
	tKrrGrpEqA	= (tKrrA[15:10] == tVugid[15:10]) && tVugidEnA;
	tKrrGrpEqB	= (tKrrB[15:10] == tVugid[15:10]) && tVugidEnB;
	tKrrGrpEqC	= (tKrrC[15:10] == tVugid[15:10]) && tVugidEnC;
	tKrrGrpEqD	= (tKrrD[15:10] == tVugid[15:10]) && tVugidEnD;
	tKrrUsrEqA	= (tKrrA[ 9: 0] == tVugid[ 9: 0]) && tVugidEnA;
	tKrrUsrEqB	= (tKrrB[ 9: 0] == tVugid[ 9: 0]) && tVugidEnB;
	tKrrUsrEqC	= (tKrrC[ 9: 0] == tVugid[ 9: 0]) && tVugidEnC;
	tKrrUsrEqD	= (tKrrD[ 9: 0] == tVugid[ 9: 0]) && tVugidEnD;
	
	tKrrGrpEqFw	= tKrrGrpEqA || tKrrGrpEqB || tKrrGrpEqC || tKrrGrpEqD;
	tKrrGrpEqRv	= tKrrUsrEqA || tKrrUsrEqB || tKrrUsrEqC || tKrrUsrEqD;
	tKrrGrpEq	= tKrrGrpEqFw;
//	if(tTlbInAcc[6])
	if(tAccMode[2])
		tKrrGrpEq	= tKrrGrpEqRv;

	tKrrUsrEq	=
		(tKrrGrpEqA && tKrrUsrEqA) ||
		(tKrrGrpEqB && tKrrUsrEqB) ||
		(tKrrGrpEqC && tKrrUsrEqC) ||
		(tKrrGrpEqD && tKrrUsrEqD) ;

//	tKrrAccFl = 
//		tKrrUsrEq ? tTlbInAcc[ 9: 7] :
//		tKrrGrpEq ? tTlbInAcc[12:10] :
//		tlbInAcc[15:13];

	tKrrAccFl = 
		tKrrUsrEq ? tAccMode[ 5: 3] :
		tKrrGrpEq ? tAccMode[8:6] :
		tAccMode[11:9];
	
	if(tVugidEnA)
	begin
//		casez(tTlbInAcc[6:4])
//		casez(tTlbInAcc[5:4])
		casez(tAccMode[1:0])
//			3'b000: begin
			2'b00: begin
				tRegOutNoRwx[2] = 1;
				tRegOutNoRwx[1] = 1;
				tRegOutNoRwx[0] = 1;
			end
//			3'b001: begin
			2'b01: begin
				if(!tKrrAccFl[2])
					tRegOutNoRwx[2] = 1;
				if(!tKrrAccFl[1])
					tRegOutNoRwx[1] = 1;
				if(!tKrrAccFl[0])
					tRegOutNoRwx[0] = 1;
			end
//			3'b010: begin
			2'b10: begin
				if(tAclUse)
				begin
					if(!tKrrAccFl[2])
						tRegOutNoRwx[2] = 1;
					if(!tKrrAccFl[1])
						tRegOutNoRwx[1] = 1;
					if(!tKrrAccFl[0])
						tRegOutNoRwx[0] = 1;
				end
				else
				begin
					tAccOutExc	= 16'hA002;
				end
			end
//			3'b011: begin
			2'b11: begin
				if(tAclUse || tKrrGrpEq)
				begin
					if(!tKrrAccFl[2])
						tRegOutNoRwx[2] = 1;
					if(!tKrrAccFl[1])
						tRegOutNoRwx[1] = 1;
					if(!tKrrAccFl[0])
						tRegOutNoRwx[0] = 1;
				end
				else
				begin
					tAccOutExc	= 16'hA002;
				end

//				if(!tKrrGrpEq)
//					tAccOutExc	= 16'hA002;

//				if(tRegInOpm[5] && !tKrrAccFl[1])
//					tAccOutExc	= 16'hA002;
//				if(tRegInOpm[4] && !tKrrAccFl[0])
//					tAccOutExc	= 16'hA002;
			end

//			default: begin
//				tAccOutExc	= 16'hA002;
//			end

		endcase
	end

	tUsDeny = (tlbInAcc[3] && !regInSR[30]);

	tRegOutNoRwx[3] = tlbInAcc[35];

	if(tUsDeny)
//		tRegOutNoRwx[3] = 1;
		tRegOutNoRwx[4] = 1;
	if(tlbInAcc[2] || tUsDeny)
		tRegOutNoRwx[2] = 1;
	if(tlbInAcc[1] || tUsDeny)
		tRegOutNoRwx[1] = 1;
	if(tlbInAcc[0] || tUsDeny)
		tRegOutNoRwx[0] = 1;

//	if(tlbInAcc[35] && !tlbInAcc[2])
	if(tlbInAcc[35] && !tlbInAcc[3])
	begin
		if(tlbInAcc[1:0]!=2'b00 && tlbInAcc[2])
			tRegOutNoRwx[3] = 0;

		if(regInSR[30])
		begin
			tRegOutNoRwx[1:0] = 0;
		end
	end

	if(!tlbMmuEnable)
	begin
		tAccOutExc	= 0;
		tRegOutNoRwx = 0;
	end

end

always @(posedge clock)
begin
	if(!regInHold)
	begin
		tTlbInAcc	<= tlbInAcc;		//TLB Access Mode
		tRegInKRR	<= regInKRR;		//Keyring Register
		tRegInOpm	<= regInOpm;

		tAccOutExc2		<= tAccOutExc;
		tRegOutNoRwx2	<= tRegOutNoRwx;
	end
end

endmodule

`endif
