module RegSpr_3W(
	clock,	// reset,
	regId,	regVal,
	wrIdA,	wrValA,
	wrIdB,	wrValB,
	wrIdC,	wrValC,
	regInVal, regHold,
	regFlush
	);

input			clock;
// input			reset;
`input_gpr		regId;
`output_gprval	regVal;
`input_gpr		wrIdA;
`input_gprval	wrValA;
`input_gpr		wrIdB;
`input_gprval	wrValB;
`input_gpr		wrIdC;
`input_gprval	wrValC;
`input_gprval	regInVal;

(* max_fanout = 200 *)
	input			regHold;
(* max_fanout = 200 *)
	input			regFlush;

(* max_fanout = 200 *)
	`reg_gprval		tRegVal;
(* max_fanout = 200 *)
	`reg_gprval		tNxtRegVal;

(* max_fanout = 200 *)
	wire		regHoldN = !regHold;


assign 		regVal = tRegVal;

reg				tIsIdA;
reg				tIsIdB;
reg				tIsIdC;
reg[1:0]		tSel;

always @*
begin
`ifndef def_true
//	tIsIdA	= (wrIdA == regId);
//	tIsIdB	= (wrIdB == regId);
//	tIsIdC	= (wrIdC == regId);
	
`ifdef jx2_riscv_usezsp
	if(regId==JX2_GR_SP)
	begin
		tIsIdA	= (wrIdA == JX2_GR_SP) || (wrIdA == JX2_GR_ZSP);
		tIsIdB	= (wrIdB == JX2_GR_SP) || (wrIdB == JX2_GR_ZSP);
		tIsIdC	= (wrIdC == JX2_GR_SP) || (wrIdC == JX2_GR_ZSP);
	end
	else
	begin
		tIsIdA	= (wrIdA == regId);
		tIsIdB	= (wrIdB == regId);
		tIsIdC	= (wrIdC == regId);
	end
`else
	tIsIdA	= (wrIdA == regId);
	tIsIdB	= (wrIdB == regId);
	tIsIdC	= (wrIdC == regId);
`endif

	casez( {tIsIdC, tIsIdB, tIsIdA} )
		3'b000:		tNxtRegVal	= regInVal;
//		3'b001:		tNxtRegVal	= wrValA;
//		3'b01z:		tNxtRegVal	= wrValB;
//		3'b1zz:		tNxtRegVal	= wrValC;
		3'bzz1:		tNxtRegVal	= wrValA;
		3'bz10:		tNxtRegVal	= wrValB;
		3'b100:		tNxtRegVal	= wrValC;
	endcase
`endif

`ifdef def_true
	tIsIdA	= (wrIdA == regId);
	tIsIdB	= (wrIdB == regId);
	tIsIdC	= (wrIdC == regId);
	casez( {regFlush, tIsIdC, tIsIdB, tIsIdA} )
		4'b0000:	tSel = 0;
		4'b0zz1:	tSel = 1;
		4'b0z10:	tSel = 2;
		4'b0100:	tSel = 3;
		4'b1zzz:	tSel = 0;
	endcase
	case(tSel)
		2'b00:	tNxtRegVal	= regInVal;
		2'b01:	tNxtRegVal	= wrValA;
		2'b10:	tNxtRegVal	= wrValB;
		2'b11:	tNxtRegVal	= wrValC;
	endcase
`endif

`ifndef def_true
	tNxtRegVal	= regInVal;
	if(!regFlush)
	begin
		if(wrIdC == regId)
			tNxtRegVal	= wrValC;
		if(wrIdB == regId)
			tNxtRegVal	= wrValB;
		if(wrIdA == regId)
			tNxtRegVal	= wrValA;
	end
`endif
end

always @(posedge clock)
begin
	if(regHoldN)
	begin
		tRegVal		<= tNxtRegVal;
	end
end

endmodule
