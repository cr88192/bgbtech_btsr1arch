/*
FM Synth
 */

`include "CoreDefs.v"

module ModAudFm(
	/* verilator lint_off UNUSED */
	clock,		reset,
	outPcmL,	outPcmR,
	busInData,	busOutData,
	busAddr,	busOpm,		busOK
	);

input			clock;
input			reset;
input[31:0]		busAddr;
input[31:0]		busInData;
input[4:0]		busOpm;

output[7:0]		outPcmL;
output[7:0]		outPcmR;
output[31:0]	busOutData;
output[1:0]		busOK;

reg[31:0]		tOutData;
reg[31:0]		tOutData2;
reg[1:0]		tOutOK;
reg[1:0]		tOutOK2;
assign		busOutData	= tOutData2;
assign		busOK		= tOutOK2;

reg[31:0]		tBusAddr;
reg[31:0]		tBusInData;
reg[4:0]		tBusOpm;

reg[7:0]	tOutPcmL;
reg[7:0]	tOutPcmR;
reg[7:0]	tOutPcmL2;
reg[7:0]	tOutPcmR2;
assign		outPcmL		= tOutPcmL2;
assign		outPcmR		= tOutPcmR2;

wire		tDevCSel;
// wire		tDevCSelCtr;
wire		tDevCSelChn;
// assign		tDevCSel = (busAddr[27:16] == 12'h008);
assign		tDevCSel = (tBusAddr[27:16] == 12'h008);
// assign		tDevCSelCtr = (busAddr[15:12] == 4'hC);
// assign		tDevCSelChn = (busAddr[15:10] == 6'b1100_00);
assign		tDevCSelChn = (tBusAddr[15:10] == 6'b1100_00);

reg[31:0]	regCtr0A[15:0];
reg[31:0]	regCtr1A[15:0];
reg[31:0]	regCtr2A[15:0];
reg[31:0]	regStsA[15:0];
reg[31:0]	regCtr0B[15:0];
reg[31:0]	regCtr1B[15:0];
reg[31:0]	regCtr2B[15:0];
reg[31:0]	regStsB[15:0];

reg[3:0]	chRov;
reg[3:0]	chNxtRov;
reg[3:0]	chRovL1;
reg[3:0]	chRovL2;
reg[3:0]	chRovL3;
reg[3:0]	chRovL4;
reg[3:0]	chRovL5;
reg[3:0]	chRovL6;
reg[3:0]	chRovL7;
reg[3:0]	chRovL8;
reg[3:0]	chRovL9;

reg			chClkStrobe;
reg			chNxtClkStrobe;
reg			chClkLatch;
reg			chNxtClkLatch;

reg[31:0]	chCtr0A;
reg[31:0]	chCtr0B;

reg[31:0]	chStsA;
reg[31:0]	chStsB;
reg[31:0]	chNxtStsA;
reg[31:0]	chNxtStsB;

reg[15:0]	fracTimer1MHz;
reg[15:0]	nextFracTimer1MHz;
reg			stepTimer1MHz;

reg[15:0]	fracTimer4MHz;
reg[15:0]	nextFracTimer4MHz;
reg			stepTimer4MHz;

// reg[15:0]	fracTimer32MHz;
// reg[15:0]	nextFracTimer32MHz;
// reg			stepTimer32MHz;

reg[19:0]	fracTimer64kHz;
reg[19:0]	nextFracTimer64kHz;
reg			stepTimer64kHz;

reg[11:0]	tChAccumL;
reg[11:0]	tChAccumR;
reg[11:0]	tNxtChAccumL;
reg[11:0]	tNxtChAccumR;

reg[7:0]	cbSinTab[63:0];
reg[7:0]	chTabPcmA;
reg[7:0]	chTabPcmB;
reg[7:0]	chTabPcmC;
reg[7:0]	chTabPcmD;

reg[7:0]	chTabPcmA1;
reg[7:0]	chTabPcmB1;
reg[7:0]	chTabPcmA1B;
reg[7:0]	chTabPcmB1B;

reg[7:0]	chTabPcmA2;
reg[7:0]	chTabPcmB2;
reg[7:0]	chTabPcmA2A;
reg[7:0]	chTabPcmB2A;

reg[7:0]	chTabPcmA3;
reg[7:0]	chTabPcmB3;
reg[7:0]	chTabPcmA3A;
reg[7:0]	chTabPcmB3A;

reg[7:0]	chPcmA;

reg[5:0]	chTabIxA;
reg[5:0]	chTabIxB;
reg[5:0]	chTabIxC;
reg[5:0]	chTabIxD;

initial
begin
	cbSinTab[ 0]=8'h00;	cbSinTab[ 1]=8'h0C;
	cbSinTab[ 2]=8'h18;	cbSinTab[ 3]=8'h24;
	cbSinTab[ 4]=8'h30;	cbSinTab[ 5]=8'h3B;
	cbSinTab[ 6]=8'h46;	cbSinTab[ 7]=8'h50;
	cbSinTab[ 8]=8'h59;	cbSinTab[ 9]=8'h62;
	cbSinTab[10]=8'h69;	cbSinTab[11]=8'h70;
	cbSinTab[12]=8'h75;	cbSinTab[13]=8'h79;
	cbSinTab[14]=8'h7C;	cbSinTab[15]=8'h7E;
	cbSinTab[16]=8'h7F;	cbSinTab[17]=8'h7E;
	cbSinTab[18]=8'h7C;	cbSinTab[19]=8'h79;
	cbSinTab[20]=8'h75;	cbSinTab[21]=8'h70;
	cbSinTab[22]=8'h69;	cbSinTab[23]=8'h62;
	cbSinTab[24]=8'h59;	cbSinTab[25]=8'h50;
	cbSinTab[26]=8'h46;	cbSinTab[27]=8'h3B;
	cbSinTab[28]=8'h30;	cbSinTab[29]=8'h24;
	cbSinTab[30]=8'h18;	cbSinTab[31]=8'h0C;
	cbSinTab[32]=8'h00;	cbSinTab[33]=8'hF4;
	cbSinTab[34]=8'hE8;	cbSinTab[35]=8'hDC;
	cbSinTab[36]=8'hD0;	cbSinTab[37]=8'hC5;
	cbSinTab[38]=8'hBA;	cbSinTab[39]=8'hB0;
	cbSinTab[40]=8'hA7;	cbSinTab[41]=8'h9E;
	cbSinTab[42]=8'h97;	cbSinTab[43]=8'h90;
	cbSinTab[44]=8'h8B;	cbSinTab[45]=8'h87;
	cbSinTab[46]=8'h84;	cbSinTab[47]=8'h82;
	cbSinTab[48]=8'h81;	cbSinTab[49]=8'h82;
	cbSinTab[50]=8'h84;	cbSinTab[51]=8'h87;
	cbSinTab[52]=8'h8B;	cbSinTab[53]=8'h90;
	cbSinTab[54]=8'h97;	cbSinTab[55]=8'h9E;
	cbSinTab[56]=8'hA7;	cbSinTab[57]=8'hB0;
	cbSinTab[58]=8'hBA;	cbSinTab[59]=8'hC5;
	cbSinTab[60]=8'hD0;	cbSinTab[61]=8'hDC;
	cbSinTab[62]=8'hE8;	cbSinTab[63]=8'hF4;
end

always @*
begin
//	tOutData	= UV32_XX;
	tOutData	= UV32_00;
	tOutOK		= UMEM_OK_READY;

	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h0290;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h0A40;

//	{ stepTimer32MHz, nextFracTimer32MHz }	=
//		{ 1'b0, fracTimer32MHz } + 17'h5200;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h02AC;
	
//	chNxtRov		= chRov + 1;
	chNxtRov		= chRov;
//	if(stepTimer32MHz)
//	if(stepTimer1MHz)
	if(stepTimer4MHz)
		chNxtRov		= chRov + 1;

	chNxtClkStrobe	= chClkStrobe;
	chNxtClkLatch	= chClkLatch;
	tNxtChAccumL	= tChAccumL;
	tNxtChAccumR	= tChAccumR;
	
	tOutPcmL		= tOutPcmL2;
	tOutPcmR		= tOutPcmR2;

	if(chRov==15)
	begin
		chNxtClkStrobe	= chClkLatch;
		chNxtClkLatch	= 0;
//		tNxtChAccum		= 0;
	end
	
//	if(stepTimer1MHz)
	if(stepTimer64kHz)
	begin
		chNxtClkLatch	= 1;
	end
	
	/* First Clock */
//	chCtr0A		= regCtr0A[chRov];
//	chCtr0B		= regCtr0B[chRov];
//	chStsA		= regStsA[chRov];
//	chStsB		= regStsB[chRov];
	
	chNxtStsA	= chStsA;
	chNxtStsB	= chStsB;
	
	if(chClkStrobe)
	begin
		chNxtStsA[19:0]	= chStsA[19:0] + chCtr0A[19:0];
		chNxtStsB[19:0]	= chStsB[19:0] + chCtr0B[19:0];
	end


	/* Generate output PCM over several cycles. */

	chTabIxB	= chStsB[19:14];
	chTabIxD	= chStsB[18:13];
	chTabPcmB	= cbSinTab[chTabIxB];
	chTabPcmD	= cbSinTab[chTabIxD];
	chTabPcmB1	= chTabPcmB;

	case(chCtr0B[31:28])
		4'h0: chTabPcmB1 = chTabPcmB;
		4'h1: chTabPcmB1 = chTabPcmB[7] ? 8'h00 : chTabPcmB;
		4'h2: chTabPcmB1 = chTabPcmB[7] ? ~chTabPcmB : chTabPcmB;
		4'h3: chTabPcmB1 = chStsB[18] ? 8'h00 : chTabPcmB;

		4'h4: chTabPcmB1 = chStsB[18] ? 8'h00 : chTabPcmD;
		4'h5: chTabPcmB1 = chStsB[18] ? 8'h00 :
			(chTabPcmD[7] ? ~chTabPcmD : chTabPcmD);
		4'h6: chTabPcmB1 = chTabPcmB[7] ? 8'h80 : 8'h7F;
		4'h7: chTabPcmB1 = { !chTabIxB[5], chTabIxB[4:0], chTabIxB[4:3] };
		default: chTabPcmB1	= chTabPcmB;
	endcase

	/* Clock Edge */

	chTabPcmB2A = chTabPcmB1B;
	if(chCtr0B[22])
		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1>>3);
	if(chCtr0B[23])
		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1>>2);
	if(chCtr0B[24])
		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1>>1);
		
	/* Clock Edge */

	case(chCtr0B[27:25])
		3'b000: chTabPcmB3A = chTabPcmB2;
		3'b001: chTabPcmB3A = chTabPcmB2>>1;
		3'b010: chTabPcmB3A = chTabPcmB2>>2;
		3'b011: chTabPcmB3A = chTabPcmB2>>3;
		3'b100: chTabPcmB3A = chTabPcmB2>>4;
		3'b101: chTabPcmB3A = chTabPcmB2>>5;
		3'b110: chTabPcmB3A = chTabPcmB2>>6;
		3'b111: chTabPcmB3A = chTabPcmB2>>7;
	endcase

	/* Clock Edge */

	chTabIxA	= chStsA[19:14];
	chTabIxC	= chStsA[18:13];

	if(chCtr0A[21])
	begin
		chTabIxA = chTabIxA + chTabPcmB3[7:2];
		chTabIxC = chTabIxC + chTabPcmB3[7:2];
	end

	chTabPcmA	= cbSinTab[chTabIxA];
	chTabPcmC	= cbSinTab[chTabIxC];
	chTabPcmA1	= chTabPcmA;
	
	case(chCtr0A[31:28])
		4'h0: chTabPcmA1 = chTabPcmA;
		4'h1: chTabPcmA1 = chTabPcmA[7] ? 8'h00 : chTabPcmA;
		4'h2: chTabPcmA1 = chTabPcmA[7] ? ~chTabPcmA : chTabPcmA;
		4'h3: chTabPcmA1 = chStsA[18] ? 8'h00 : chTabPcmA;
		4'h4: chTabPcmA1 = chStsA[18] ? 8'h00 : chTabPcmC;
		4'h5: chTabPcmA1 = chStsA[18] ? 8'h00 :
			(chTabPcmC[7] ? ~chTabPcmC : chTabPcmC);
		4'h6: chTabPcmA1 = chTabPcmA[7] ? 8'h80 : 8'h7F;
		4'h7: chTabPcmA1 = { !chTabIxA[5], chTabIxA[4:0], chTabIxA[4:3] };
		default: chTabPcmA1	= chTabPcmA;
	endcase

	/* Clock Edge */

	chTabPcmA2A = chTabPcmA1B;
	if(chCtr0A[22])
		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1>>3);
	if(chCtr0A[23])
		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1>>2);
	if(chCtr0A[24])
		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1>>1);
		
	/* Clock Edge */

	case(chCtr0A[27:25])
		3'b000: chTabPcmA3A = chTabPcmA2;
		3'b001: chTabPcmA3A = chTabPcmA2>>1;
		3'b010: chTabPcmA3A = chTabPcmA2>>2;
		3'b011: chTabPcmA3A = chTabPcmA2>>3;
		3'b100: chTabPcmA3A = chTabPcmA2>>4;
		3'b101: chTabPcmA3A = chTabPcmA2>>5;
		3'b110: chTabPcmA3A = chTabPcmA2>>6;
		3'b111: chTabPcmA3A = chTabPcmA2>>7;
	endcase

	/* Clock Edge */

	chPcmA = (chCtr0A[21]) ? chTabPcmA3 : (chTabPcmA3 + chTabPcmB3);
	
//	if(chRovL3==chRov)
//	if((chRovL6==chRov) && (chRovL7!=chRov))
	if((chRovL8==chRov) && (chRovL9!=chRov))
	begin
		tNxtChAccumL = tChAccumL + { (chPcmA[7] ? 4'hF : 4'h0), chPcmA };
		tNxtChAccumR = tChAccumR + { (chPcmA[7] ? 4'hF : 4'h0), chPcmA };
	end

	/* Update output PCM. */

//	if(chRov==15)
//	if((chRov==15) && stepTimer1MHz)
	if((chRov==15) && stepTimer4MHz)
	begin
		tOutPcmL		= tChAccumL[7:0];
		tOutPcmR		= tChAccumR[7:0];
		if(tChAccumL[11:8] != (tChAccumL[7] ? 4'hF : 4'h0))
			tOutPcmL = tChAccumL[11] ? 8'h80 : 8'h7F;
		if(tChAccumR[11:8] != (tChAccumR[7] ? 4'hF : 4'h0))
			tOutPcmR = tChAccumR[11] ? 8'h80 : 8'h7F;
		tNxtChAccumL	= 0;
		tNxtChAccumR	= 0;
	end

	if(tDevCSel && tBusOpm[3])
	begin
		tOutOK		= UMEM_OK_OK;

		if(tDevCSelChn)
		begin
			case(tBusAddr[4:2])
				3'b000: tOutData = regCtr0A[tBusAddr[8:5]];
				3'b001: tOutData = regCtr1A[tBusAddr[8:5]];
				3'b010: tOutData = regCtr2A[tBusAddr[8:5]];
				3'b011: tOutData = regStsA[tBusAddr[8:5]];
				3'b100: tOutData = regCtr0B[tBusAddr[8:5]];
				3'b101: tOutData = regCtr1B[tBusAddr[8:5]];
				3'b110: tOutData = regCtr2B[tBusAddr[8:5]];
				3'b111: tOutData = regStsB[tBusAddr[8:5]];
			endcase
		end
	end

	if(tDevCSel && tBusOpm[4])
	begin
		tOutOK		= UMEM_OK_OK;
	end

end

always @(posedge clock)
begin
	tOutData2		<= tOutData;
	tOutOK2			<= tOutOK;
	tOutPcmL2		<= tOutPcmL;
	tOutPcmR2		<= tOutPcmR;

	tBusAddr		<= busAddr;
	tBusInData		<= busInData;
	tBusOpm			<= busOpm;

	fracTimer1MHz	<= nextFracTimer1MHz;
	fracTimer4MHz	<= nextFracTimer4MHz;
//	fracTimer32MHz	<= nextFracTimer32MHz;
	fracTimer64kHz	<= nextFracTimer64kHz;

	chCtr0A			<= regCtr0A[chRov];
	chCtr0B			<= regCtr0B[chRov];
	chStsA			<= regStsA[chRov];
	chStsB			<= regStsB[chRov];

	regStsA[chRov]	<= chNxtStsA;
	regStsB[chRov]	<= chNxtStsB;

	chRov			<= chNxtRov;
	chRovL1			<= chRov;
	chRovL2			<= chRovL1;
	chRovL3			<= chRovL2;
	chRovL4			<= chRovL3;
	chRovL5			<= chRovL4;
	chRovL6			<= chRovL5;
	chRovL7			<= chRovL6;
	chRovL8			<= chRovL7;
	chRovL9			<= chRovL8;

	chClkStrobe		<= chNxtClkStrobe;
	chClkLatch		<= chNxtClkLatch;
	tChAccumL		<= tNxtChAccumL;
	tChAccumR		<= tNxtChAccumR;

	chTabPcmA1B		<= chTabPcmA1;
	chTabPcmB1B		<= chTabPcmB1;
	chTabPcmA2		<= chTabPcmA2A;
	chTabPcmB2		<= chTabPcmB2A;
	chTabPcmA3		<= chTabPcmA3A;
	chTabPcmB3		<= chTabPcmB3A;

	if(tDevCSel && tBusOpm[4])
	begin
		if(tDevCSelChn)
		begin
			case(tBusAddr[4:2])
				3'b000: regCtr0A[busAddr[8:5]]	<= tBusInData;
				3'b001: regCtr1A[busAddr[8:5]]	<= tBusInData;
				3'b010: regCtr2A[busAddr[8:5]]	<= tBusInData;
				3'b100: regCtr0B[busAddr[8:5]]	<= tBusInData;
				3'b101: regCtr1B[busAddr[8:5]]	<= tBusInData;
				3'b110: regCtr2B[busAddr[8:5]]	<= tBusInData;
				default: begin
				end
			endcase
		end
	end
end

endmodule
