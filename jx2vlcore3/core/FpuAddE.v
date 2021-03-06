/*
FPU Add (Extended Precision)

Uses S.15.80 as its format.
This format is a truncated version of the Binary128 format.


Adds two input values, producing an output value.
ExOp: Is set to indicate that a floating-point op is requested.
ExOK: Set to indicate whether the operation has completed.

ExOp: 
	0: No Op
	1: Ro=Rn+Rm
	2: Ro=Rn-Rm
	3: Ro=I2F(Rn)
*/

`include "CoreDefs.v"

module FpuAddE(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRm,
	regValRn,
	regValRo,
	regExOp,
	regExOK
	);

input	clock;
input	reset;
input	exHold;

input[95:0]		regValRm;
input[95:0]		regValRn;
output[95:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[95:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

assign	regValRo	= tRegValRo2;
assign	regExOK		= tRegExOK2;

reg[95:0]		tRegValRo;
reg[1:0]		tRegExOK;

reg[1:0]		tRegExOp1;
reg				tExEn1;
reg				tSgnA1;
reg				tSgnB1;
reg[14:0]		tExpA1;
reg[14:0]		tExpB1;
reg[89:0]		tFraA1;
reg[89:0]		tFraB1;
reg				tFraGe1;

reg[89:0]		tFraJ1;
reg[95:0]		tRegValRn1;

reg[15:0]		tExpA1D;
reg[15:0]		tExpB1D;
reg[89:0]		tFraA1D;
reg[89:0]		tFraB1D;


reg[1:0]		tRegExOp2;
reg				tExEn2;
reg				tSgnA2;
reg				tSgnB2;
reg[14:0]		tExpA2;
reg[14:0]		tExpB2;
reg[89:0]		tFraA2;
reg[89:0]		tFraB2;
reg[89:0]		tFraJ2;
reg[95:0]		tRegValRn2;

reg				tSgnC2;
reg[14:0]		tExpC2;
reg[89:0]		tFraC2;

reg				tExEn3;
reg				tSgnC3;
reg[14:0]		tExpC3;
reg[89:0]		tFraC3;
reg				tSgnC3B;
reg[15:0]		tExpC3B;
reg[89:0]		tFraC3B;

reg				tExEn4;
reg				tSgnC4;
reg[15:0]		tExpC4;
reg[89:0]		tFraC4;

reg				tSgnC4B;
reg[15:0]		tExpC4B;
reg[89:0]		tFraC4B;

reg[95:0]		tValC4;

reg				tExEn5;

always @*
begin
	tRegExOp1	= regExOp;
	tExEn1		= regExOp != 0;
	tRegValRn1	= regValRn;

	tRegExOK=UMEM_OK_READY;
	
	/* Stage 1 */
	tSgnA1	= regValRn[95];
	tSgnB1	= regValRm[95] ^ regExOp[1];
	tExpA1	= regValRn[94:80];
	tExpB1	= regValRm[94:80];
	tFraA1	= {1'b0, (tExpA1!=0), regValRn[79:0], 8'h0};
	tFraB1	= {1'b0, (tExpB1!=0), regValRm[79:0], 8'h0};
	tExpA1D	= {1'b0, tExpA1} - {1'b0, tExpB1};
	tExpB1D	= {1'b0, tExpB1} - {1'b0, tExpA1};
	tFraA1D	= (tExpB1D<=80) ? (tFraA1 >> tExpB1D[5:0]) : 0;
	tFraB1D	= (tExpA1D<=80) ? (tFraB1 >> tExpA1D[5:0]) : 0;

	tFraGe1 = (tFraA1 >= tFraB1);

//	tFraJ1	= regValRn;

	/* Stage 2 */
	if(tRegExOp2 == 3)
	begin
		if(tRegValRn2[95])
		begin
			tSgnC2	= 1;
//			tExpC2	= 1086;
			tExpC2	= 1085;
			tFraC2	= {2'b00, ~tRegValRn2[95:32], 24'hFFFFFF};
		end
		else
		begin
			tSgnC2	= 0;
//			tExpC2	= 1086;
			tExpC2	= 1085;
			tFraC2	= {2'b00, tRegValRn2[95:32], 24'h000000};
		end
	end
	else
	begin
		tSgnC2	= tSgnA2;
		tExpC2	= tExpA2;
		tFraC2	= (tSgnA2 == tSgnB2) ?
			(tFraA2 + tFraB2) :
			(tFraA2 - tFraB2) ;
	end
	
	/* Stage 3 */
	tSgnC3B	= tSgnC3;
	if(tFraC3[89])
	begin
		tExpC3B	= {1'b0, tExpC3} + 1;
		tFraC3B	= tFraC3 >> 1;
	end
	else
	if(tFraC3[88:80]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} ;
		tFraC3B	= tFraC3 ;
	end
	else if(tFraC3[79:72]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 8;
		tFraC3B	= tFraC3 << 8;
	end
	else if(tFraC3[71:64]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 16;
		tFraC3B	= tFraC3 << 16;
	end
	else if(tFraC3[63:56]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 24;
		tFraC3B	= tFraC3 << 24;
	end
	else if(tFraC3[55:48]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 32;
		tFraC3B	= tFraC3 << 32 ;
	end
	else if(tFraC3[47:40]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 40;
		tFraC3B	= tFraC3 << 40 ;
	end
	else if(tFraC3[39:32]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 48;
		tFraC3B	= tFraC3 << 48 ;
	end
	else if(tFraC3[31:24]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 56;
		tFraC3B	= tFraC3 << 56 ;
	end
	else if(tFraC3[23:16]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 64;
		tFraC3B	= tFraC3 << 64 ;
	end
	else if(tFraC3[15:8]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 72;
		tFraC3B	= tFraC3 << 72;
	end
	else if(tFraC3[7:0]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 80;
		tFraC3B	= tFraC3 << 80 ;
	end
	else
	begin
		tSgnC3B	= 0;
		tExpC3B	= 0;
		tFraC3B	= 0;
	end
	
	/* Stage 4 */
	casez(tFraC4[88:81])
		8'b1zzzzzzz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4;
			tFraC4B	= tFraC4;
		end
		8'b01zzzzzz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 1;
			tFraC4B	= tFraC4 << 1;
		end
		8'b001zzzzz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 2;
			tFraC4B	= tFraC4 << 2;
		end
		8'b0001zzzz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 3;
			tFraC4B	= tFraC4 << 3;
		end
		8'b00001zzz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 4;
			tFraC4B	= tFraC4 << 4;
		end
		8'b000001zz: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 5;
			tFraC4B	= tFraC4 << 5;
		end
		8'b0000001z: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 6;
			tFraC4B	= tFraC4 << 6;
		end
		8'b00000001: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 7;
			tFraC4B	= tFraC4 << 7;
		end
		8'b00000000: begin
			tSgnC4B	= tSgnC4;
			tExpC4B	= tExpC4 - 8;
			tFraC4B	= tFraC4 << 8;
		end
	endcase
	
	if(tExpC4B[15])
	begin
		tSgnC4B	= 0;
		tExpC4B	= 0;
		tFraC4B	= 0;
		tValC4 = UV96_00;
	end
	else
	begin
		tValC4 = { tSgnC4B, tExpC4B[14:0], tFraC4B[87:8] };
	end
	
end

always @(posedge clock)
begin
	tRegValRo2	<= tRegValRo;
	tRegExOK2	<= tRegExOK;

	if(tExEn1 && !exHold)
	begin
//		if(tExpA1>=tExpB1)
		if((tExpA1>tExpB1) || ((tExpA1==tExpB1) && tFraGe1))
		begin
			tSgnA2 <= tSgnA1;	tSgnB2 <= tSgnB1;
			tExpA2 <= tExpA1;	tExpB2 <= tExpB1;
			tFraA2 <= tFraA1;	tFraB2 <= tFraB1D;
		end
		else
		begin
			tSgnA2 <= tSgnB1;	tSgnB2 <= tSgnA1;
			tExpA2 <= tExpB1;	tExpB2 <= tExpA1;
			tFraA2 <= tFraB1;	tFraB2 <= tFraA1D;
		end
		
		tRegExOp2	<= tRegExOp1;
		tFraJ2		<= tFraJ1;
		tExEn2		<= tExEn1;
		tRegValRn2	<= tRegValRn1;
	end
	else if(!exHold)
	begin
		tExEn2		<= 0;
	end

	if(tExEn2)
	begin
		tSgnC3		<= tSgnC2;
		tExpC3		<= tExpC2;
		tFraC3		<= tFraC2;

		tSgnC4		<= tSgnC3B;
		tExpC4		<= tExpC3B;
		tFraC4		<= tFraC3B;
		
		tExEn3		<= tExEn2;
		tExEn4		<= tExEn3;
		tExEn5		<= tExEn4;

		tRegValRo	<= tValC4;
	end
	else
	begin
//		tExEn2		<= 0;
		tExEn3		<= 0;
		tExEn4		<= 0;
		tExEn5		<= 0;
		tRegValRo	<= UV96_00;
	end
end

endmodule
