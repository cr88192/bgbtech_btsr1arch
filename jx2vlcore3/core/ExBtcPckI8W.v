module ExBtcPckI8W(
	regValRs,
	regOutVal
	);

input[15:0]		regValRs;
output[7:0]		regOutVal;

reg[3:0]	tRegValCr;
reg[3:0]	tRegValCg;
reg[3:0]	tRegValCb;
reg[3:0]	tRegValCy;
reg[4:0]	tRegValCi;
reg[4:0]	tRegValCj;
reg[1:0]	tRegValSy;

reg[3:0]	tRegValCrn;
reg[3:0]	tRegValCgn;
reg[3:0]	tRegValCbn;

reg[3:0]	tRegValCrs;
reg[3:0]	tRegValCgs;
reg[3:0]	tRegValCbs;
reg[3:0]	tRegValCby;

reg[4:0]	tRegValCi_Yr;
reg[4:0]	tRegValCi_Yg;
reg[4:0]	tRegValCi_Yb;

reg[2:0]	tRegValCsh;

reg[7:0]		tRegOutVal;
assign		regOutVal = tRegOutVal;

always @*
begin
	tRegValCr	= regValRs[14:11];
	tRegValCg	= regValRs[ 9: 6];
	tRegValCb	= regValRs[ 4: 1];
	
	tRegValCy	= tRegValCg;
	tRegValSy	= 2;

	if(tRegValCr > tRegValCy)
	begin
		tRegValCy	= tRegValCr;
		tRegValSy	= 1;
	end

	if(tRegValCb > tRegValCy)
	begin
		tRegValCy	= tRegValCb;
		tRegValSy	= 3;
	end

	case(tRegValCy)
		4'hF: tRegValCsh=0;
		4'hE: tRegValCsh=0;
		4'hD: tRegValCsh=1;
		4'hC: tRegValCsh=2;
		4'hB: tRegValCsh=2;
		4'hA: tRegValCsh=3;
		4'h9: tRegValCsh=3;
		4'h8: tRegValCsh=3;
		4'h7: tRegValCsh=4;
		4'h6: tRegValCsh=4;
		4'h5: tRegValCsh=5;
		4'h4: tRegValCsh=5;
		4'h3: tRegValCsh=0;
		4'h2: tRegValCsh=0;
		4'h1: tRegValCsh=0;
		4'h0: tRegValCsh=0;
	endcase

	case(tRegValCy)
		4'hF: tRegValCby=0;
		4'hE: tRegValCby=1;
		4'hD: tRegValCby=1;
		4'hC: tRegValCby=0;
		4'hB: tRegValCby=2;
		4'hA: tRegValCby=0;
		4'h9: tRegValCby=2;
		4'h8: tRegValCby=3;
		4'h7: tRegValCby=1;
		4'h6: tRegValCby=3;
		4'h5: tRegValCby=0;
		4'h4: tRegValCby=3;
		4'h3: tRegValCby=0;
		4'h2: tRegValCby=0;
		4'h1: tRegValCby=0;
		4'h0: tRegValCby=0;
	endcase

	case(tRegValCsh)
		0: begin
			tRegValCrs = 0;
			tRegValCgs = 0;
			tRegValCbs = 0;
		end
		1: begin
			tRegValCrs = {3'b0, tRegValCr[3]};
			tRegValCgs = {3'b0, tRegValCg[3]};
			tRegValCbs = {3'b0, tRegValCb[3]};
		end
		2: begin
			tRegValCrs = {2'b0, tRegValCr[3:2]};
			tRegValCgs = {2'b0, tRegValCg[3:2]};
			tRegValCbs = {2'b0, tRegValCb[3:2]};
		end
		3: begin
			tRegValCrs = {1'b0, tRegValCr[3:1]};
			tRegValCgs = {1'b0, tRegValCg[3:1]};
			tRegValCbs = {1'b0, tRegValCb[3:1]};
		end
		4: begin
			tRegValCrs = tRegValCr;
			tRegValCgs = tRegValCg;
			tRegValCbs = tRegValCb;
		end
		5: begin
			tRegValCrs = {tRegValCr[2:0], 1'b0};
			tRegValCgs = {tRegValCg[2:0], 1'b0};
			tRegValCbs = {tRegValCb[2:0], 1'b0};
		end
		6: begin
//			tRegValCrs = {tRegValCr[1:0], 2'b0};
//			tRegValCgs = {tRegValCg[1:0], 2'b0};
//			tRegValCbs = {tRegValCb[1:0], 2'b0};
			tRegValCrs = 0;
			tRegValCgs = 0;
			tRegValCbs = 0;
		end
		7: begin
//			tRegValCrs = {tRegValCr[0], 3'b0};
//			tRegValCgs = {tRegValCg[0], 3'b0};
//			tRegValCbs = {tRegValCb[0], 3'b0};
			tRegValCrs = 0;
			tRegValCgs = 0;
			tRegValCbs = 0;
		end
	endcase

	tRegValCrn	= tRegValCr + tRegValCrs;
	tRegValCgn	= tRegValCg + tRegValCgs;
	tRegValCbn	= tRegValCb + tRegValCbs;

	case( {tRegValCrn[3:2], tRegValCgn[3:2], tRegValCbn[3:2]} )
		6'h00: tRegValCi=5'h00;
		6'h01: tRegValCi=5'h01;
		6'h02: tRegValCi=5'h01;
		6'h03: tRegValCi=5'h01;
		6'h04: tRegValCi=5'h02;
		6'h05: tRegValCi=5'h03;
		6'h06: tRegValCi=5'h01;
		6'h07: tRegValCi=5'h01;
		6'h08: tRegValCi=5'h02;
		6'h09: tRegValCi=5'h02;
		6'h0A: tRegValCi=5'h03;
		6'h0B: tRegValCi=5'h13;
		6'h0C: tRegValCi=5'h02;
		6'h0D: tRegValCi=5'h02;
		6'h0E: tRegValCi=5'h02;
		6'h0F: tRegValCi=5'h03;
		6'h10: tRegValCi=5'h04;
		6'h11: tRegValCi=5'h05;
		6'h12: tRegValCi=5'h01;
		6'h13: tRegValCi=5'h01;
		6'h14: tRegValCi=5'h06;
		6'h15: tRegValCi=5'h1F;
		6'h16: tRegValCi=5'h01;
		6'h17: tRegValCi=5'h01;
		6'h18: tRegValCi=5'h02;
		6'h19: tRegValCi=5'h02;
		6'h1A: tRegValCi=5'h03;
		6'h1B: tRegValCi=5'h13;
		6'h1C: tRegValCi=5'h02;
		6'h1D: tRegValCi=5'h02;
		6'h1E: tRegValCi=5'h0A;
		6'h1F: tRegValCi=5'h03;
		6'h20: tRegValCi=5'h04;
		6'h21: tRegValCi=5'h04;
		6'h22: tRegValCi=5'h05;
		6'h23: tRegValCi=5'h01;
		6'h24: tRegValCi=5'h04;
		6'h25: tRegValCi=5'h04;
		6'h26: tRegValCi=5'h05;
		6'h27: tRegValCi=5'h09;
		6'h28: tRegValCi=5'h06;
		6'h29: tRegValCi=5'h06;
		6'h2A: tRegValCi=5'h1F;
		6'h2B: tRegValCi=5'h09;
		6'h2C: tRegValCi=5'h12;
		6'h2D: tRegValCi=5'h12;
		6'h2E: tRegValCi=5'h0A;
		6'h2F: tRegValCi=5'h0B;
		6'h30: tRegValCi=5'h04;
		6'h31: tRegValCi=5'h04;
		6'h32: tRegValCi=5'h04;
		6'h33: tRegValCi=5'h05;
		6'h34: tRegValCi=5'h04;
		6'h35: tRegValCi=5'h04;
		6'h36: tRegValCi=5'h0C;
		6'h37: tRegValCi=5'h05;
		6'h38: tRegValCi=5'h11;
		6'h39: tRegValCi=5'h11;
		6'h3A: tRegValCi=5'h0C;
		6'h3B: tRegValCi=5'h0D;
		6'h3C: tRegValCi=5'h06;
		6'h3D: tRegValCi=5'h06;
		6'h3E: tRegValCi=5'h0E;
		6'h3F: tRegValCi=5'h1F;
	endcase

	case( {tRegValCrn[1:0], tRegValCgn[1:0], tRegValCbn[1:0]} )
		6'h00: tRegValCj=5'h00;
		6'h01: tRegValCj=5'h00;
		6'h02: tRegValCj=5'h07;
		6'h03: tRegValCj=5'h07;
		6'h04: tRegValCj=5'h00;
		6'h05: tRegValCj=5'h00;
		6'h06: tRegValCj=5'h0F;
		6'h07: tRegValCj=5'h0F;
		6'h08: tRegValCj=5'h08;
		6'h09: tRegValCj=5'h0F;
		6'h0A: tRegValCj=5'h0F;
		6'h0B: tRegValCj=5'h0F;
		6'h0C: tRegValCj=5'h08;
		6'h0D: tRegValCj=5'h0F;
		6'h0E: tRegValCj=5'h0F;
		6'h0F: tRegValCj=5'h0F;
		6'h10: tRegValCj=5'h00;
		6'h11: tRegValCj=5'h00;
		6'h12: tRegValCj=5'h07;
		6'h13: tRegValCj=5'h07;
		6'h14: tRegValCj=5'h00;
		6'h15: tRegValCj=5'h00;
		6'h16: tRegValCj=5'h00;
		6'h17: tRegValCj=5'h07;
		6'h18: tRegValCj=5'h08;
		6'h19: tRegValCj=5'h00;
		6'h1A: tRegValCj=5'h00;
		6'h1B: tRegValCj=5'h0F;
		6'h1C: tRegValCj=5'h08;
		6'h1D: tRegValCj=5'h08;
		6'h1E: tRegValCj=5'h0F;
		6'h1F: tRegValCj=5'h0F;
		6'h20: tRegValCj=5'h07;
		6'h21: tRegValCj=5'h07;
		6'h22: tRegValCj=5'h07;
		6'h23: tRegValCj=5'h07;
		6'h24: tRegValCj=5'h08;
		6'h25: tRegValCj=5'h00;
		6'h26: tRegValCj=5'h00;
		6'h27: tRegValCj=5'h07;
		6'h28: tRegValCj=5'h08;
		6'h29: tRegValCj=5'h00;
		6'h2A: tRegValCj=5'h00;
		6'h2B: tRegValCj=5'h00;
		6'h2C: tRegValCj=5'h08;
		6'h2D: tRegValCj=5'h08;
		6'h2E: tRegValCj=5'h00;
		6'h2F: tRegValCj=5'h00;
		6'h30: tRegValCj=5'h07;
		6'h31: tRegValCj=5'h07;
		6'h32: tRegValCj=5'h07;
		6'h33: tRegValCj=5'h07;
		6'h34: tRegValCj=5'h08;
		6'h35: tRegValCj=5'h07;
		6'h36: tRegValCj=5'h07;
		6'h37: tRegValCj=5'h07;
		6'h38: tRegValCj=5'h08;
		6'h39: tRegValCj=5'h08;
		6'h3A: tRegValCj=5'h00;
		6'h3B: tRegValCj=5'h00;
		6'h3C: tRegValCj=5'h08;
		6'h3D: tRegValCj=5'h08;
		6'h3E: tRegValCj=5'h00;
		6'h3F: tRegValCj=5'h00;
	endcase
	

	case( {tRegValCgn[3:1], tRegValCbn[3:1]} )
		6'h00: tRegValCi_Yr=5'h04;
		6'h01: tRegValCi_Yr=5'h04;
		6'h02: tRegValCi_Yr=5'h04;
		6'h03: tRegValCi_Yr=5'h04;
		6'h04: tRegValCi_Yr=5'h05;
		6'h05: tRegValCi_Yr=5'h05;
		6'h06: tRegValCi_Yr=5'h05;
		6'h07: tRegValCi_Yr=5'h05;
		6'h08: tRegValCi_Yr=5'h04;
		6'h09: tRegValCi_Yr=5'h04;
		6'h0A: tRegValCi_Yr=5'h04;
		6'h0B: tRegValCi_Yr=5'h04;
		6'h0C: tRegValCi_Yr=5'h05;
		6'h0D: tRegValCi_Yr=5'h05;
		6'h0E: tRegValCi_Yr=5'h05;
		6'h0F: tRegValCi_Yr=5'h05;
		6'h10: tRegValCi_Yr=5'h04;
		6'h11: tRegValCi_Yr=5'h04;
		6'h12: tRegValCi_Yr=5'h04;
		6'h13: tRegValCi_Yr=5'h04;
		6'h14: tRegValCi_Yr=5'h05;
		6'h15: tRegValCi_Yr=5'h05;
		6'h16: tRegValCi_Yr=5'h05;
		6'h17: tRegValCi_Yr=5'h05;
		6'h18: tRegValCi_Yr=5'h04;
		6'h19: tRegValCi_Yr=5'h04;
		6'h1A: tRegValCi_Yr=5'h04;
		6'h1B: tRegValCi_Yr=5'h04;
		6'h1C: tRegValCi_Yr=5'h05;
		6'h1D: tRegValCi_Yr=5'h05;
		6'h1E: tRegValCi_Yr=5'h05;
		6'h1F: tRegValCi_Yr=5'h05;
		6'h20: tRegValCi_Yr=5'h11;
		6'h21: tRegValCi_Yr=5'h11;
		6'h22: tRegValCi_Yr=5'h11;
		6'h23: tRegValCi_Yr=5'h11;
		6'h24: tRegValCi_Yr=5'h0C;
		6'h25: tRegValCi_Yr=5'h0C;
		6'h26: tRegValCi_Yr=5'h0D;
		6'h27: tRegValCi_Yr=5'h0D;
		6'h28: tRegValCi_Yr=5'h11;
		6'h29: tRegValCi_Yr=5'h11;
		6'h2A: tRegValCi_Yr=5'h11;
		6'h2B: tRegValCi_Yr=5'h11;
		6'h2C: tRegValCi_Yr=5'h0C;
		6'h2D: tRegValCi_Yr=5'h0C;
		6'h2E: tRegValCi_Yr=5'h0D;
		6'h2F: tRegValCi_Yr=5'h0D;
		6'h30: tRegValCi_Yr=5'h06;
		6'h31: tRegValCi_Yr=5'h06;
		6'h32: tRegValCi_Yr=5'h06;
		6'h33: tRegValCi_Yr=5'h06;
		6'h34: tRegValCi_Yr=5'h0E;
		6'h35: tRegValCi_Yr=5'h0E;
		6'h36: tRegValCi_Yr=5'h00;
		6'h37: tRegValCi_Yr=5'h07;
		6'h38: tRegValCi_Yr=5'h06;
		6'h39: tRegValCi_Yr=5'h06;
		6'h3A: tRegValCi_Yr=5'h06;
		6'h3B: tRegValCi_Yr=5'h06;
		6'h3C: tRegValCi_Yr=5'h0E;
		6'h3D: tRegValCi_Yr=5'h0E;
		6'h3E: tRegValCi_Yr=5'h08;
		6'h3F: tRegValCi_Yr=5'h00;
	endcase


	case( {tRegValCrn[3:1], tRegValCbn[3:1]} )
		6'h00: tRegValCi_Yg=5'h02;
		6'h01: tRegValCi_Yg=5'h02;
		6'h02: tRegValCi_Yg=5'h02;
		6'h03: tRegValCi_Yg=5'h02;
		6'h04: tRegValCi_Yg=5'h03;
		6'h05: tRegValCi_Yg=5'h03;
		6'h06: tRegValCi_Yg=5'h03;
		6'h07: tRegValCi_Yg=5'h03;
		6'h08: tRegValCi_Yg=5'h02;
		6'h09: tRegValCi_Yg=5'h02;
		6'h0A: tRegValCi_Yg=5'h02;
		6'h0B: tRegValCi_Yg=5'h02;
		6'h0C: tRegValCi_Yg=5'h03;
		6'h0D: tRegValCi_Yg=5'h03;
		6'h0E: tRegValCi_Yg=5'h03;
		6'h0F: tRegValCi_Yg=5'h03;
		6'h10: tRegValCi_Yg=5'h02;
		6'h11: tRegValCi_Yg=5'h02;
		6'h12: tRegValCi_Yg=5'h02;
		6'h13: tRegValCi_Yg=5'h02;
		6'h14: tRegValCi_Yg=5'h03;
		6'h15: tRegValCi_Yg=5'h03;
		6'h16: tRegValCi_Yg=5'h03;
		6'h17: tRegValCi_Yg=5'h03;
		6'h18: tRegValCi_Yg=5'h02;
		6'h19: tRegValCi_Yg=5'h02;
		6'h1A: tRegValCi_Yg=5'h02;
		6'h1B: tRegValCi_Yg=5'h02;
		6'h1C: tRegValCi_Yg=5'h03;
		6'h1D: tRegValCi_Yg=5'h03;
		6'h1E: tRegValCi_Yg=5'h03;
		6'h1F: tRegValCi_Yg=5'h03;
		6'h20: tRegValCi_Yg=5'h12;
		6'h21: tRegValCi_Yg=5'h12;
		6'h22: tRegValCi_Yg=5'h12;
		6'h23: tRegValCi_Yg=5'h12;
		6'h24: tRegValCi_Yg=5'h0A;
		6'h25: tRegValCi_Yg=5'h0A;
		6'h26: tRegValCi_Yg=5'h0B;
		6'h27: tRegValCi_Yg=5'h0B;
		6'h28: tRegValCi_Yg=5'h12;
		6'h29: tRegValCi_Yg=5'h12;
		6'h2A: tRegValCi_Yg=5'h12;
		6'h2B: tRegValCi_Yg=5'h12;
		6'h2C: tRegValCi_Yg=5'h0A;
		6'h2D: tRegValCi_Yg=5'h0A;
		6'h2E: tRegValCi_Yg=5'h0B;
		6'h2F: tRegValCi_Yg=5'h0B;
		6'h30: tRegValCi_Yg=5'h06;
		6'h31: tRegValCi_Yg=5'h06;
		6'h32: tRegValCi_Yg=5'h06;
		6'h33: tRegValCi_Yg=5'h06;
		6'h34: tRegValCi_Yg=5'h0E;
		6'h35: tRegValCi_Yg=5'h0E;
		6'h36: tRegValCi_Yg=5'h00;
		6'h37: tRegValCi_Yg=5'h0F;
		6'h38: tRegValCi_Yg=5'h06;
		6'h39: tRegValCi_Yg=5'h06;
		6'h3A: tRegValCi_Yg=5'h06;
		6'h3B: tRegValCi_Yg=5'h06;
		6'h3C: tRegValCi_Yg=5'h0E;
		6'h3D: tRegValCi_Yg=5'h0E;
		6'h3E: tRegValCi_Yg=5'h08;
		6'h3F: tRegValCi_Yg=5'h00;
	endcase

	case( {tRegValCrn[3:1], tRegValCgn[3:1]} )
		6'h00: tRegValCi_Yb=5'h01;
		6'h01: tRegValCi_Yb=5'h01;
		6'h02: tRegValCi_Yb=5'h01;
		6'h03: tRegValCi_Yb=5'h01;
		6'h04: tRegValCi_Yb=5'h13;
		6'h05: tRegValCi_Yb=5'h13;
		6'h06: tRegValCi_Yb=5'h03;
		6'h07: tRegValCi_Yb=5'h03;
		6'h08: tRegValCi_Yb=5'h01;
		6'h09: tRegValCi_Yb=5'h01;
		6'h0A: tRegValCi_Yb=5'h01;
		6'h0B: tRegValCi_Yb=5'h01;
		6'h0C: tRegValCi_Yb=5'h13;
		6'h0D: tRegValCi_Yb=5'h13;
		6'h0E: tRegValCi_Yb=5'h03;
		6'h0F: tRegValCi_Yb=5'h03;
		6'h10: tRegValCi_Yb=5'h01;
		6'h11: tRegValCi_Yb=5'h01;
		6'h12: tRegValCi_Yb=5'h01;
		6'h13: tRegValCi_Yb=5'h01;
		6'h14: tRegValCi_Yb=5'h13;
		6'h15: tRegValCi_Yb=5'h13;
		6'h16: tRegValCi_Yb=5'h03;
		6'h17: tRegValCi_Yb=5'h03;
		6'h18: tRegValCi_Yb=5'h01;
		6'h19: tRegValCi_Yb=5'h01;
		6'h1A: tRegValCi_Yb=5'h01;
		6'h1B: tRegValCi_Yb=5'h01;
		6'h1C: tRegValCi_Yb=5'h13;
		6'h1D: tRegValCi_Yb=5'h13;
		6'h1E: tRegValCi_Yb=5'h03;
		6'h1F: tRegValCi_Yb=5'h03;
		6'h20: tRegValCi_Yb=5'h05;
		6'h21: tRegValCi_Yb=5'h05;
		6'h22: tRegValCi_Yb=5'h05;
		6'h23: tRegValCi_Yb=5'h05;
		6'h24: tRegValCi_Yb=5'h09;
		6'h25: tRegValCi_Yb=5'h09;
		6'h26: tRegValCi_Yb=5'h0B;
		6'h27: tRegValCi_Yb=5'h0B;
		6'h28: tRegValCi_Yb=5'h05;
		6'h29: tRegValCi_Yb=5'h05;
		6'h2A: tRegValCi_Yb=5'h05;
		6'h2B: tRegValCi_Yb=5'h05;
		6'h2C: tRegValCi_Yb=5'h09;
		6'h2D: tRegValCi_Yb=5'h09;
		6'h2E: tRegValCi_Yb=5'h0B;
		6'h2F: tRegValCi_Yb=5'h0B;
		6'h30: tRegValCi_Yb=5'h05;
		6'h31: tRegValCi_Yb=5'h05;
		6'h32: tRegValCi_Yb=5'h05;
		6'h33: tRegValCi_Yb=5'h05;
		6'h34: tRegValCi_Yb=5'h0D;
		6'h35: tRegValCi_Yb=5'h0D;
		6'h36: tRegValCi_Yb=5'h00;
		6'h37: tRegValCi_Yb=5'h0F;
		6'h38: tRegValCi_Yb=5'h05;
		6'h39: tRegValCi_Yb=5'h05;
		6'h3A: tRegValCi_Yb=5'h05;
		6'h3B: tRegValCi_Yb=5'h05;
		6'h3C: tRegValCi_Yb=5'h0D;
		6'h3D: tRegValCi_Yb=5'h0D;
		6'h3E: tRegValCi_Yb=5'h07;
		6'h3F: tRegValCi_Yb=5'h00;
	endcase

	if((tRegValCrn[3:1]!=3'b111) && (tRegValSy == 1))
		tRegValSy = 0;
	if((tRegValCgn[3:1]!=3'b111) && (tRegValSy == 2))
		tRegValSy = 0;
	if((tRegValCbn[3:1]!=3'b111) && (tRegValSy == 3))
		tRegValSy = 0;

	if(tRegValSy == 1)
		tRegValCi = tRegValCi_Yr;
	if(tRegValSy == 2)
		tRegValCi = tRegValCi_Yg;
	if(tRegValSy == 3)
		tRegValCi = tRegValCi_Yb;

	if(tRegValCi==5'h1F)
		tRegValCi=tRegValCj;

//	if(tRegValCy[3:2]==0)
//		tRegValCi=0;

	if(tRegValCy[3:2]==0)
	begin
//		if((tRegValCy[1:0]!=0) && (tRegValCi!=0) && !tRegValCi[3])
		if((tRegValCy[1:0]!=0) &&
			(tRegValCi!=0) &&
			(tRegValCi!=7) &&
			(tRegValCi[4:3]==2'b00))
		begin
			if(tRegValSy == 1)
				tRegValCi = 5'h11;
			if(tRegValSy == 2)
				tRegValCi = 5'h12;
			if(tRegValSy == 3)
				tRegValCi = 5'h13;
		end
		else
		begin
			tRegValCi=0;
		end
	end

	tRegOutVal	= { tRegValCi[3:0], tRegValCy };
	if(tRegValCi[4])
	begin
		tRegOutVal	= { tRegValCy, tRegValCi[3:0] };
	end
end

endmodule
