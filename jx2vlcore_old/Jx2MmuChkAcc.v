/*
Determine whether access can be allowed to a given page.

AccMode:
  [31:16]=VUGID
  [15: 4]=KR Access
  [ 3: 0]=Base Access
    3=NU
    2=NX
    1=NW
    0=NR
  
 */
module Jx2MmuChkAcc(
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbInAcc,
	accOutExc);

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register

input[4:0]		regInOpm;		//Operation Size/Type
input[31:0]		tlbInAcc;		//TLB Access Mode

output[15:0]	accOutExc;		//Output Exception Code

reg[15:0]		tAccOutExc;		//Output Exception Code
assign			accOutExc = tAccOutExc;

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

reg			tKrrUsrEqA;
reg			tKrrUsrEqB;
reg			tKrrUsrEqC;
reg			tKrrUsrEqD;
reg			tKrrUsrEq;

reg[2:0]	tKrrAccFl;

reg			tUsDeny;

always @*
begin
	tAccOutExc	= 0;

	tKrrA		= regInKRR[15: 0];
	tKrrB		= regInKRR[31:16];
	tKrrC		= regInKRR[47:32];
	tKrrD		= regInKRR[63:48];
	tVugidEnA	= (tKrrA != 16'h0000);
	tVugidEnB	= (tKrrB != 16'h0000);
	tVugidEnC	= (tKrrC != 16'h0000);
	tVugidEnD	= (tKrrD != 16'h0000);
	tVugid		= tlbInAcc[31:16];
	
	tKrrGrpEqA	= (tKrrA[15:10] == tVugid[15:10]) && tVugidEnA;
	tKrrGrpEqB	= (tKrrB[15:10] == tVugid[15:10]) && tVugidEnB;
	tKrrGrpEqC	= (tKrrC[15:10] == tVugid[15:10]) && tVugidEnC;
	tKrrGrpEqD	= (tKrrD[15:10] == tVugid[15:10]) && tVugidEnD;
	tKrrUsrEqA	= (tKrrA[ 9: 0] == tVugid[ 9: 0]) && tVugidEnA;
	tKrrUsrEqB	= (tKrrB[ 9: 0] == tVugid[ 9: 0]) && tVugidEnB;
	tKrrUsrEqC	= (tKrrC[ 9: 0] == tVugid[ 9: 0]) && tVugidEnC;
	tKrrUsrEqD	= (tKrrD[ 9: 0] == tVugid[ 9: 0]) && tVugidEnD;
	
	tKrrGrpEq	= tKrrGrpEqA || tKrrGrpEqB || tKrrGrpEqC || tKrrGrpEqD;
	tKrrUsrEq	=
		(tKrrGrpEqA && tKrrUsrEqA) ||
		(tKrrGrpEqB && tKrrUsrEqB) ||
		(tKrrGrpEqC && tKrrUsrEqC) ||
		(tKrrGrpEqD && tKrrUsrEqD) ;
	tKrrAccFl = 
		tKrrUsrEq ? tlbInAcc[ 9: 7] :
		tKrrGrpEq ? tlbInAcc[12:10] :
		tlbInAcc[15:13];
	
	if(tVugidEnA)
	begin
		case(tlbInAcc[6:4])
			3'b000: begin
				if(regInOpm[4])
					tAccOutExc	= 16'h8002;
				if(regInOpm[3])
					tAccOutExc	= 16'h8001;
			end
			3'b001: begin
				if(regInOpm[4] && !tKrrAccFl[1])
					tAccOutExc	= 16'h8002;
				if(regInOpm[3] && !tKrrAccFl[0])
					tAccOutExc	= 16'h8001;
			end
			3'b010: begin
				tAccOutExc	= 16'hA002;
			end
			3'b011: begin
				if(regInOpm[4] && !tKrrAccFl[1])
					tAccOutExc	= 16'hA002;
				if(regInOpm[3] && !tKrrAccFl[0])
					tAccOutExc	= 16'hA002;
			end
			default: begin
				tAccOutExc	= 16'hA002;
			end
		endcase
	end

	tUsDeny = (tlbInAcc[3] && !regInSR[30]);

	if(regInOpm[4] && (tlbInAcc[1] || tUsDeny))
		tAccOutExc	= 16'h8002;
	if(regInOpm[3] && (tlbInAcc[0] || tUsDeny))
		tAccOutExc	= 16'h8001;

end

endmodule
