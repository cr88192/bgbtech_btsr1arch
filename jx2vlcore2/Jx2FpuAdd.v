/*
FPU Add

Adds two input values, producing an output value.
ExOp: Is set to indicate that a floating-point op is requested.
ExOK: Set to indicate whether the operation has completed.

ExOp: 
	0: No Op
	1: Ro=Rn+Rm
	2: Ro=Rn-Rm
	3: Ro=I2F(Rn)
*/

`include "Jx2CoreDefs.v"

module Jx2FpuAdd(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regValRm,
	regValRn,
	regValRo,
	regExOp,
	regExOK
	);

input	clock;
input	reset;

input[63:0]		regValRm;
input[63:0]		regValRn;
output[63:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[63:0]		tRegValRo;
reg[1:0]		tRegExOK;

assign	regValRo	= tRegValRo;
assign	regExOK		= tRegExOK;

reg				tExEn1;
reg				tSgnA1;
reg				tSgnB1;
reg[10:0]		tExpA1;
reg[10:0]		tExpB1;
reg[63:0]		tFraA1;
reg[63:0]		tFraB1;

// reg				tSgnJ1;
// reg[10:0]		tExpJ1;
reg[63:0]		tFraJ1;

reg[11:0]		tExpA1D;
reg[11:0]		tExpB1D;
reg[63:0]		tFraA1D;
reg[63:0]		tFraB1D;


reg				tExEn2;
reg				tSgnA2;
reg				tSgnB2;
reg[10:0]		tExpA2;
reg[10:0]		tExpB2;
reg[63:0]		tFraA2;
reg[63:0]		tFraB2;
// reg				tSgnJ2;
// reg[10:0]		tExpJ2;
reg[63:0]		tFraJ2;

reg				tSgnC2;
reg[10:0]		tExpC2;
reg[63:0]		tFraC2;

reg				tExEn3;
reg				tSgnC3;
reg[10:0]		tExpC3;
reg[63:0]		tFraC3;
reg				tSgnC3B;
reg[11:0]		tExpC3B;
reg[63:0]		tFraC3B;

reg				tExEn4;
reg				tSgnC4;
reg[11:0]		tExpC4;
reg[63:0]		tFraC4;

reg				tSgnC4B;
reg[11:0]		tExpC4B;
reg[63:0]		tFraC4B;

reg[63:0]		tValC4;

reg				tExEn5;

always @*
begin
	tExEn1	= regExOp != 0;

	tRegExOK=UMEM_OK_READY;
	if(tExEn1)
	begin
		if(tExEn5)
			tRegExOK=UMEM_OK_OK;
		else
			tRegExOK=UMEM_OK_HOLD;
	end
	
	/* Stage 1 */
	tSgnA1	= regValRn[63];
//	tSgnB1	= regValRm[63];
	tSgnB1	= regValRm[63] ^ regExOp[1];
	tExpA1	= regValRn[62:52];
	tExpB1	= regValRm[62:52];
//	tFraA1	= {2'b01, regValRn[51:0], 10'h0};
//	tFraB1	= {2'b01, regValRm[51:0], 10'h0};
	tFraA1	= {1'b0, (tExpA1!=0), regValRn[51:0], 10'h0};
	tFraB1	= {1'b0, (tExpB1!=0), regValRm[51:0], 10'h0};
	tExpA1D	= {1'b0, tExpA1} - {1'b0, tExpB1};
	tExpB1D	= {1'b0, tExpB1} - {1'b0, tExpA1};
	tFraA1D	= (tExpB1D<=52) ? (tFraA1 >> tExpB1D[5:0]) : 0;
	tFraB1D	= (tExpA1D<=52) ? (tFraB1 >> tExpA1D[5:0]) : 0;

/*
	if(regValRn[63])
	begin
		tSgnJ1	= 1;
		tExpJ1	= 1086;
		tFraJ1	= -regValRn;
	end
	else
	begin
		tSgnJ1	= 0;
		tExpJ1	= 1086;
		tFraJ1	= regValRn;
	end
*/

	tFraJ1	= regValRn;

	/* Stage 2 */
	if(regExOp == 3)
	begin
		if(tFraJ2[63])
		begin
			tSgnC2	= 1;
			tExpC2	= 1086;
			tFraC2	= -regValRn;
		end
		else
		begin
			tSgnC2	= 0;
			tExpC2	= 1086;
			tFraC2	= regValRn;
		end

//		tSgnC2	= tSgnJ2;
//		tExpC2	= tExpJ2;
//		tFraC2	= tFraJ2;
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
	if(tFraC3[63])
	begin
		tExpC3B	= {1'b0, tExpC3} + 1;
		tFraC3B	= tFraC3 >> 1;
	end
	else
	if(tFraC3[62:54]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} ;
		tFraC3B	= tFraC3 ;
	end
	else if(tFraC3[53:46]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 8;
		tFraC3B	= tFraC3 << 8;
	end
	else if(tFraC3[45:38]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 16;
		tFraC3B	= tFraC3 << 16;
	end
	else if(tFraC3[37:30]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 24;
		tFraC3B	= tFraC3 << 24;
	end
	else if(tFraC3[29:22]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 32;
		tFraC3B	= tFraC3 << 32 ;
	end
	else if(tFraC3[21:14]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 40;
		tFraC3B	= tFraC3 << 40 ;
	end
	else if(tFraC3[13:6]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 48;
		tFraC3B	= tFraC3 << 48 ;
	end
	else if(tFraC3[5:0]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 56;
		tFraC3B	= tFraC3 << 56 ;
	end
	else
	begin
		tSgnC3B	= 0;
		tExpC3B	= 0;
		tFraC3B	= 0;
	end
	
	/* Stage 4 */
	casez(tFraC4[62:55])
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
	
	if(tExpC4B[11])
	begin
		tSgnC4B	= 0;
		tExpC4B	= 0;
		tFraC4B	= 0;
		tValC4 = UV64_00;
	end
	else
	begin
//		tValC4 = { tSgnC4B, tExpC4B[10:0],
//			tFraC4B[61:10]+(tFraC4B[9]?52'h1:52'h0) };
		tValC4 = { tSgnC4B, tExpC4B[10:0], tFraC4B[61:10] } +
			(tFraC4B[9]?64'h1:64'h0);
	end
	
end

always @(posedge clock)
begin
	if(tExEn1)
	begin
		if(tExpA1>=tExpB1)
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

//		tSgnJ2	<= tSgnJ1;
//		tExpJ2	<= tExpJ1;
		tFraJ2	<= tFraJ1;

		tSgnC3	<= tSgnC2;
		tExpC3	<= tExpC2;
		tFraC3	<= tFraC2;

		tSgnC4	<= tSgnC3B;
		tExpC4	<= tExpC3B;
		tFraC4	<= tFraC3B;
		
		tExEn2		<= tExEn1;
		tExEn3		<= tExEn2;
		tExEn4		<= tExEn3;
		tExEn5		<= tExEn4;

		tRegValRo	<= tValC4;
	end
	else
	begin
		tExEn2		<= 0;
		tExEn3		<= 0;
		tExEn4		<= 0;
		tExEn5		<= 0;
		tRegValRo	<= UV64_XX;
	end
end

endmodule
