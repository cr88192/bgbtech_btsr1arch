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
    5=Resv
    4=Hold (Not-Ready)
    3=NU (Not User)
    2=NX (No Execute)
    1=NW (No Write)
    0=NR (No Read)

 */
module RbiMmuChkAcc(
	clock, reset,
	regInHold,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbInAcc,
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
input[31:0]		tlbInAcc;		//TLB Access Mode

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

reg			tKrrUsrEqA;
reg			tKrrUsrEqB;
reg			tKrrUsrEqC;
reg			tKrrUsrEqD;
reg			tKrrUsrEq;

reg[2:0]	tKrrAccFl;
reg[2:0]	tNextKrrAccFl;

reg			tUsDeny;
reg			tlbMmuEnable;

reg[31:0]		tTlbInAcc;		//TLB Access Mode
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
	
//	tVugidEnA	= 0;
	
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
//	tNextKrrAccFl = 
		tKrrUsrEq ? tTlbInAcc[ 9: 7] :
		tKrrGrpEq ? tTlbInAcc[12:10] :
		tlbInAcc[15:13];
	
	if(tVugidEnA)
	begin
		case(tTlbInAcc[6:4])
			3'b000: begin
				tRegOutNoRwx[2] = 1;
				tRegOutNoRwx[1] = 1;
				tRegOutNoRwx[0] = 1;
			end
			3'b001: begin
				if(!tKrrAccFl[2])
					tRegOutNoRwx[2] = 1;
				if(!tKrrAccFl[1])
					tRegOutNoRwx[1] = 1;
				if(!tKrrAccFl[0])
					tRegOutNoRwx[0] = 1;
			end
			3'b010: begin
				tAccOutExc	= 16'hA002;
			end
			3'b011: begin
				if(tRegInOpm[5] && !tKrrAccFl[1])
					tAccOutExc	= 16'hA002;
				if(tRegInOpm[4] && !tKrrAccFl[0])
					tAccOutExc	= 16'hA002;
			end
			default: begin
				tAccOutExc	= 16'hA002;
			end
		endcase
	end

	tUsDeny = (tlbInAcc[3] && !regInSR[30]);

	if(tUsDeny)
		tRegOutNoRwx[3] = 1;
	if(tlbInAcc[2] || tUsDeny)
		tRegOutNoRwx[2] = 1;
	if(tlbInAcc[1] || tUsDeny)
		tRegOutNoRwx[1] = 1;
	if(tlbInAcc[0] || tUsDeny)
		tRegOutNoRwx[0] = 1;

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
