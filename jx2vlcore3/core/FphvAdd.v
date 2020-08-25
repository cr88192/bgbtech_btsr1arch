/*
Half Precision Add/Sub

Add/Sub two 16-bit half-float Numbers.
Intended for pipelined operation.
*/

module FphvAdd(
	clock,		reset,
	regValRs,
	regValRt,
	regValRn,
	regExOp
	);

/* verilator lint_off UNUSED */

input	clock;
input	reset;

/* verilator lint_on UNUSED */

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRn;
input[1:0]		regExOp;

reg[31:0]		tRegValRn;

assign	regValRn	= tRegValRn;


reg[1:0]	tRegExOp0;
reg[1:0]	tRegExOp1;
reg[1:0]	tRegExOp2;

reg[8:0]	tExa0;
reg[8:0]	tExb0;

reg[8:0]	tExa1;
reg[8:0]	tExb1;

reg[8:0]	tExc2;

reg[8:0]	tExDifA0;
reg[8:0]	tExDifB0;

reg			tSga0;
reg			tSgb0;
reg			tSga1;
reg			tSgb1;

reg			tSgc2;

reg			tSgd0;
reg			tSgd1;

reg			tExGtAB0;
reg			tExGtBA0;
reg			tFrGtAB0;

reg			tExGtAB1;
reg			tExGtBA1;
reg			tFrGtAB1;

reg[17:0]	tFra0;
reg[17:0]	tFrb0;
reg[17:0]	tFra1;
reg[17:0]	tFrb1;

reg[17:0]	tFrShrA0;
reg[17:0]	tFrShrB0;
reg[17:0]	tFrShrA1;
reg[17:0]	tFrShrB1;

reg[17:0]	tFrDifA0;
reg[17:0]	tFrDifB0;
reg[17:0]	tFrSum0;

reg[17:0]	tFrDifA1;
reg[17:0]	tFrDifB1;
reg[17:0]	tFrSum1;


reg			tSgc1;
reg[8:0]	tExc1;
reg[17:0]	tFrc1;

reg[17:0]	tFrc2;
reg			tSgc2B;
reg[8:0]	tExc2B;
reg[15:0]	tFrc2B;

// /* verilator lint_on UNUSED */

always @*
begin
	/* Stage 1 */

	tRegExOp0	= regExOp;
	tExa0	= { 1'b0, regValRs[30:23] };
	tExb0	= { 1'b0, regValRt[30:23] };
	tFra0	= { 2'b01, regValRs[22:7] };
	tFrb0	= { 2'b01, regValRt[22:7] };
	tSga0	= regValRs[15];
	tSgb0	= regValRt[15] ^ (!regExOp[0]);

	tExDifA0	= tExa0-tExb0;
	tExDifB0	= tExb0-tExa0;
	
	tExGtAB0	= tExDifB0[8];
	tExGtBA0	= tExDifA0[8];

	tFrDifA0	= tFra0-tFrb0;
	tFrDifB0	= tFrb0-tFra0;
	tFrSum0	= tFra0+tFrb0;

	tFrGtAB0	= tFrDifB0[17];
	
	tFrShrA0	= (tExDifB0[8:3]==0) ? (tFra0>>tExDifB0[3:0]) : 0;
	tFrShrB0	= (tExDifA0[8:3]==0) ? (tFrb0>>tExDifA0[3:0]) : 0;
	
	tSgd0	= tSga0^tSgb0;


	/* Stage 2 */
	
	if(tExGtAB1)
	begin
		tSgc1=tSga1;
		tExc1=tExa1;
		if(tSgd1)
			tFrc1=tFra1-tFrShrB1;
		else
			tFrc1=tFra1+tFrShrB1;
	end
	else if(tExGtBA1)
	begin
		tSgc1=tSgb1;
		tExc1=tExb1;
		if(tSgd1)
			tFrc1=tFrb1-tFrShrA1;
		else
			tFrc1=tFrb1+tFrShrA1;
	end
	else
	begin
		if(tFrGtAB1)
		begin
			tSgc1=tSga1;
			tExc1=tExa1;
			if(tSgd1)
				tFrc1=tFrDifA1;
			else
				tFrc1=tFrSum1;
		end
		else
		begin
			tSgc1=tSgb1;
			tExc1=tExb1;
			if(tSgd1)
				tFrc1=tFrDifB1;
			else
				tFrc1=tFrSum1;
		end
	end


	/* Stage 3 */

	tSgc2B = tSgc2;

	casez(tFrc2)
		18'b1zzzzzzzzzzzzzzzzz: begin
			tExc2B	= tExc2+1;	tFrc2B	= { tFrc2[16:1]      };		end
		18'b01zzzzzzzzzzzzzzzz: begin
			tExc2B	= tExc2;	tFrc2B	= { tFrc2[15:0]      };		end
		18'b001zzzzzzzzzzzzzzz: begin
			tExc2B	= tExc2-1;	tFrc2B	= { tFrc2[14:0], 1'b0 };		end
		18'b0001zzzzzzzzzzzzzz: begin
			tExc2B	= tExc2-2;	tFrc2B	= { tFrc2[13:0], 2'b0 };		end
		18'b00001zzzzzzzzzzzzz: begin
			tExc2B	= tExc2-3;	tFrc2B	= { tFrc2[12:0], 3'b0 };		end
		18'b000001zzzzzzzzzzzz: begin
			tExc2B	= tExc2-4;	tFrc2B	= { tFrc2[11:0], 4'b0 };		end
		18'b0000001zzzzzzzzzzz: begin
			tExc2B	= tExc2-5;	tFrc2B	= { tFrc2[10:0], 5'b0 };		end
		18'b00000001zzzzzzzzzz: begin
			tExc2B	= tExc2-6;	tFrc2B	= { tFrc2[9:0], 6'b0 };		end
		18'b000000001zzzzzzzzz: begin
			tExc2B	= tExc2-7;	tFrc2B	= { tFrc2[8:0], 7'b0 };		end
		18'b0000000001zzzzzzzz: begin
			tExc2B	= tExc2-8;	tFrc2B	= { tFrc2[7:0], 8'b0 };		end
		18'b00000000001zzzzzzz: begin
			tExc2B	= tExc2-9;	tFrc2B	= { tFrc2[6:0], 9'b0 };		end
		18'b000000000001zzzzzz: begin
			tExc2B	= tExc2-10;	tFrc2B	= { tFrc2[5:0], 10'b0 };	end
		18'b0000000000001zzzzz: begin
			tExc2B	= tExc2-11;	tFrc2B	= { tFrc2[4:0], 11'b0 };	end
		18'b00000000000001zzzz: begin
			tExc2B	= tExc2-12;	tFrc2B	= { tFrc2[3:0], 12'b0 };	end
		18'b000000000000001zzz: begin
			tExc2B	= tExc2-13;	tFrc2B	= { tFrc2[2:0], 13'b0 };	end
		18'b0000000000000001zz: begin
			tExc2B	= tExc2-14;	tFrc2B	= { tFrc2[1:0], 14'b0 };	end
		18'b00000000000000001z: begin
			tExc2B	= tExc2-15;	tFrc2B	= { tFrc2[  0], 15'b0 };	end
		18'b000000000000000001: begin
			tExc2B	= tExc2-16;	tFrc2B	= {            16'b0 };		end
		18'b000000000000000000: begin
			tExc2B	= 0;	tFrc2B	= 0;
			tSgc2B	= 0;
		end
	endcase

	if(tExc2B[8])
	begin
		tRegValRn = tExc2B[7] ? 32'h00000000 : {tSgc2B, 31'h7F800000};
	end
	else
	begin
		tRegValRn = { tSgc2B, tExc2B[7:0], tFrc2B[15:0], UV7_00 };
	end

end

always @(posedge clock)
begin
	tRegExOp1	<= tRegExOp0;
	tRegExOp2	<= tRegExOp1;

	tExa1		<= tExa0;
	tExb1		<= tExb0;

	tSga1		<= tSga0;
	tSgb1		<= tSgb0;

	tSgd1		<= tSgd0;

	tFra1		<= tFra0;
	tFrb1		<= tFrb0;
	tFrShrA1	<= tFrShrA0;
	tFrShrB1	<= tFrShrB0;
	tFrDifA1	<= tFrDifA0;
	tFrDifB1	<= tFrDifB0;
	tFrSum1		<= tFrSum0;

	tExGtAB1	<= tExGtAB0;
	tExGtBA1	<= tExGtBA0;
	tFrGtAB1	<= tFrGtAB0;


	tSgc2		<= tSgc1;
	tExc2		<= tExc1;
	tFrc2		<= tFrc1;
end

endmodule
