/*
FM Synth
 */

`include "CoreDefs.v"

`include "ModFmScVol6.v"

module ModAudFm(
	/* verilator lint_off UNUSED */
	clock,		reset,
	outPcmL,	outPcmR,
	busInData,	busOutData,
	busAddr,	busOpm,		busOK,
	timer4MHz,	timer1MHz,	timer64kHz,
	timer1kHz,	timerNoise
	);

input			clock;
input			reset;
input[31:0]		busAddr;
input[31:0]		busInData;
input[4:0]		busOpm;

output[11:0]	outPcmL;
output[11:0]	outPcmR;
output[31:0]	busOutData;
output[1:0]		busOK;

input			timer4MHz;
input			timer1MHz;
input			timer64kHz;
input			timer1kHz;
input			timerNoise;

reg				tTimer4MHzA;
reg				tTimer4MHzB;
reg				tTimer4MHz;
reg				tTimer4MHzL;
reg				tTimer1MHz;
reg				tTimer64kHz;
reg				tTimer1kHz;
reg				tTimerNoise;


reg[31:0]		tOutData;
reg[31:0]		tOutData2;
reg[1:0]		tOutOK;
reg[1:0]		tOutOK2;
assign		busOutData	= tOutData2;
assign		busOK		= tOutOK2;

reg[31:0]		tBusAddr;
reg[31:0]		tBusInData;
reg[4:0]		tBusOpm;

reg[11:0]	tOutPcmL;
reg[11:0]	tOutPcmR;
reg[11:0]	tOutPcmL2;
reg[11:0]	tOutPcmR2;
assign		outPcmL		= tOutPcmL2;
assign		outPcmR		= tOutPcmR2;

wire		tDevCSel;
// wire		tDevCSelCtr;
wire		tDevCSelChn;
assign		tDevCSel		= (tBusAddr[27:16] == 12'h008);
// assign		tDevCSelChn		= (tBusAddr[15:10] == 6'b1100_00);
assign		tDevCSelChn		= (tBusAddr[15: 9] == 7'b1100_000);

reg[35:0]	regCtr0A[15:0];
reg[31:0]	regCtr1A[15:0];
reg[31:0]	regCtr2A[15:0];
reg[31:0]	regStsA [15:0];
reg[35:0]	regCtr0B[15:0];
reg[31:0]	regCtr1B[15:0];
reg[31:0]	regCtr2B[15:0];
reg[31:0]	regStsB [15:0];

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
reg[3:0]	chRovL10;
reg[3:0]	chRovL11;

reg			chClkStrobe;
reg			chNxtClkStrobe;
reg			chClkLatch;
reg			chNxtClkLatch;

reg[35:0]	chCtr0A;
reg[35:0]	chCtr0B;

reg[31:0]	chStsA;
reg[31:0]	chStsB;
reg[31:0]	chNxtStsA;
reg[31:0]	chNxtStsB;
reg			chDoStoreSts;

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

reg[15:0]	tChAccumL;
reg[15:0]	tChAccumR;
reg[15:0]	tNxtChAccumL;
reg[15:0]	tNxtChAccumR;
reg[2:0]	tChAccumOvfLb;
reg[2:0]	tChAccumOvfRb;
reg			tChAccumOvfLa;
reg			tChAccumOvfRa;

reg[31:0]	tChBiasL;
reg[31:0]	tChBiasR;
reg[31:0]	tNxtChBiasL;
reg[31:0]	tNxtChBiasR;

reg[7:0]	cbSinTab[63:0];
reg[7:0]	chTabPcmA;
reg[7:0]	chTabPcmB;
reg[7:0]	chTabPcmC;
reg[7:0]	chTabPcmD;

reg[7:0]	chTabPcmA1;
reg[7:0]	chTabPcmB1;
reg[7:0]	chTabPcmBiA1;
reg[7:0]	chTabPcmBiB1;

reg[11:0]	chTabPcmA1B;
reg[11:0]	chTabPcmB1B;

reg[11:0]	chTabPcmA2;
reg[11:0]	chTabPcmB2;
reg[11:0]	chTabPcmA2A;
reg[11:0]	chTabPcmB2A;

reg[11:0]	chTabPcmA3;
reg[11:0]	chTabPcmB3;
reg[11:0]	chTabPcmA3A;
reg[11:0]	chTabPcmB3A;

wire[5:0]		chTabScA;
wire[5:0]		chTabScB;

wire[11:0]		chTabPcmScA2;
wire[11:0]		chTabPcmScB2;
ModFmScVol6		fmScA(
	clock,	reset,
	chTabPcmA1B, chTabPcmScA2,
	chTabScA);
ModFmScVol6		fmScB(
	clock,	reset,
	chTabPcmB1B, chTabPcmScB2,
	chTabScB);

assign		chTabScA = chCtr0A[27:22];
assign		chTabScB = chCtr0B[27:22];


// reg[7:0]	chPcmA;
// reg[11:0]	chPcmA;
reg[15:0]	chPcmA;

reg[5:0]	chTabIxA;
reg[5:0]	chTabIxB;
reg[5:0]	chTabIxC;
reg[5:0]	chTabIxD;

reg[3:0]	tFlushRov;
reg[3:0]	tNxtFlushRov;
reg			tNxtFlushRovAdv;
reg			tFlushRovAdv;
reg			tFlushRovAdvL;

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

	tNxtFlushRovAdv = 0;

	if(reset)
		tNxtFlushRovAdv = 1;
	if(tFlushRov == 0)
		tNxtFlushRovAdv = 1;

	tNxtFlushRov	= tFlushRov;
	if(tFlushRovAdv && !tFlushRovAdvL)
		tNxtFlushRov	= tFlushRov + 1;

//	{ stepTimer1MHz, nextFracTimer1MHz }	=
//		{ 1'b0, fracTimer1MHz } + 17'h0290;

//	{ stepTimer4MHz, nextFracTimer4MHz }	=
//		{ 1'b0, fracTimer4MHz } + 17'h0A40;

//	{ stepTimer32MHz, nextFracTimer32MHz }	=
//		{ 1'b0, fracTimer32MHz } + 17'h5200;

//	{ stepTimer64kHz, nextFracTimer64kHz }	=
//		{ 1'b0, fracTimer64kHz } + 21'h02AC;
	
//	chNxtRov		= chRov + 1;
	chNxtRov		= chRov;
//	if(stepTimer32MHz)
//	if(stepTimer1MHz)
//	if(stepTimer4MHz)
//	if(tTimer4MHz)
	if(tTimer4MHz && !tTimer4MHzL)
		chNxtRov		= chRov + 1;

	chNxtClkStrobe	= chClkStrobe;
	chNxtClkLatch	= chClkLatch;
	tNxtChAccumL	= tChAccumL;
	tNxtChAccumR	= tChAccumR;
	tNxtChBiasL		= tChBiasL;
	tNxtChBiasR		= tChBiasR;
	
	tOutPcmL		= tOutPcmL2;
	tOutPcmR		= tOutPcmR2;

//	if(chRov==15)
//	if(	(chRov == 15) &&
	if(	(chRov == 0) &&
		(chRovL1 != chRovL2))
	begin
		chNxtClkStrobe	= chClkLatch;
		chNxtClkLatch	= 0;
//		tNxtChAccum		= 0;
	end
	
//	if(stepTimer1MHz)
//	if(stepTimer64kHz)
	if(tTimer64kHz)
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
	chDoStoreSts	= 0;
	
//	if(chClkStrobe)
	if(chClkStrobe &&
		(chRovL4 == chRov) &&
		(chRovL5 != chRov) )
	begin
		$display("ModeFm: Step %d", chRov);

		chNxtStsA[19:0]	= chStsA[19:0] + chCtr0A[19:0];
		chNxtStsB[19:0]	= chStsB[19:0] + chCtr0B[19:0];

//		chNxtStsA[19:0]	= chStsA[19:0] + 20'h003FF;
//		chNxtStsB[19:0]	= chStsB[19:0] + 20'h003FF;

//		if(chRov==12)
//			chNxtStsA[19:0]	= chStsA[19:0] + 20'h003FF;

		chDoStoreSts	= 1;

	end


	/* Generate output PCM over several cycles. */

	chTabIxB	= chStsB[19:14];
	chTabIxD	= chStsB[18:13];
//	chTabPcmB	= cbSinTab[chTabIxB];
//	chTabPcmD	= cbSinTab[chTabIxD];
	chTabPcmB1		= chTabPcmB;
	chTabPcmBiB1	= 0;

	case(chCtr0B[31:28])
		4'h0: 		chTabPcmBiB1	= 8'h00;
		4'h1: 		chTabPcmBiB1	= 8'h28;
		4'h2: 		chTabPcmBiB1	= 8'h50;
		4'h3: 		chTabPcmBiB1	= 8'h28;
		4'h4: 		chTabPcmBiB1	= 8'h00;
		4'h5: 		chTabPcmBiB1	= 8'h28;
		4'h6: 		chTabPcmBiB1	= 8'h00;
		4'h7: 		chTabPcmBiB1	= 8'h00;
		default: 	chTabPcmBiB1	= 8'h00;
	endcase

	case(chCtr0B[31:28])
		4'h0: chTabPcmB1 = chTabPcmB;
		4'h1: chTabPcmB1 = chTabPcmB[7] ? 8'h00 : chTabPcmB;
		4'h2: chTabPcmB1 = chTabPcmB[7] ? ~chTabPcmB : chTabPcmB;
		4'h3: chTabPcmB1 = chStsB[18] ? 8'h00 : chTabPcmB;

		4'h4: chTabPcmB1 = chStsB[18] ? 8'h00 : chTabPcmD;
		4'h5: chTabPcmB1 = chStsB[18] ? 8'h00 :
			(chTabPcmD[7] ? ~chTabPcmD : chTabPcmD);
		4'h6: chTabPcmB1 = chTabPcmB[7] ? 8'h80 : 8'h7F;
//		4'h7: chTabPcmB1 = { !chTabIxB[5], chTabIxB[4:0], chTabIxB[4:3] };
		4'h7: chTabPcmB1 = { chTabIxB[5], ~chTabIxB[4:0], ~chTabIxB[5:4] };
		default: chTabPcmB1	= chTabPcmB;
	endcase

	/* Clock Edge */
`ifndef def_true
	chTabPcmB2A = chTabPcmB1B;
//	if(chRov[0])
//		chTabPcmB2A = ~chTabPcmB1B;

//	if(chCtr0B[22])
//		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1B >>> 3);
//	if(chCtr0B[23])
//		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1B >>> 2);
//	if(chCtr0B[24])
//		chTabPcmB2A = chTabPcmB2A - (chTabPcmB1B >>> 1);
		
	if(chCtr0B[22])
		chTabPcmB2A = chTabPcmB2A -
			{ chTabPcmB1B[11]?3'b111:3'b000, chTabPcmB1B[11:3] };
	if(chCtr0B[23])
		chTabPcmB2A = chTabPcmB2A -
			{ chTabPcmB1B[11]?2'b11:2'b00, chTabPcmB1B[11:2] };
	if(chCtr0B[24])
		chTabPcmB2A = chTabPcmB2A -
			{ chTabPcmB1B[11], chTabPcmB1B[11:1] };

	/* Clock Edge */

	case(chCtr0B[27:25])
//		3'b000: chTabPcmB3A = chTabPcmB2;
//		3'b001: chTabPcmB3A = chTabPcmB2 >>> 1;
//		3'b010: chTabPcmB3A = chTabPcmB2 >>> 2;
//		3'b011: chTabPcmB3A = chTabPcmB2 >>> 3;
//		3'b100: chTabPcmB3A = chTabPcmB2 >>> 4;
//		3'b101: chTabPcmB3A = chTabPcmB2 >>> 5;
//		3'b110: chTabPcmB3A = chTabPcmB2 >>> 6;
//		3'b111: chTabPcmB3A = chTabPcmB2 >>> 7;

		3'b000: chTabPcmB3A =
			chTabPcmB2;
		3'b001: chTabPcmB3A =
			{ chTabPcmB2[11], chTabPcmB2[11:1] };
		3'b010: chTabPcmB3A =
			{ chTabPcmB2[11]?2'b11:2'b00, chTabPcmB2[11:2] };
		3'b011: chTabPcmB3A =
			{ chTabPcmB2[11]?3'b111:3'b100, chTabPcmB2[11:3] };
		3'b100: chTabPcmB3A =
			{ chTabPcmB2[11]?4'b1111:4'b0000, chTabPcmB2[11:4] };
		3'b101: chTabPcmB3A = 
			{ chTabPcmB2[11]?5'b11111:5'b00000, chTabPcmB2[11:5] };
		3'b110: chTabPcmB3A = 
			{ chTabPcmB2[11]?6'b111111:6'b000000, chTabPcmB2[11:6] };
		3'b111: chTabPcmB3A = 
			{ chTabPcmB2[11]?7'b1111111:7'b0000000, chTabPcmB2[11:7] };
	endcase
`endif

	chTabPcmB3A = chTabPcmScB2;


	/* Clock Edge */

	chTabIxA	= chStsA[19:14];
	chTabIxC	= chStsA[18:13];

//	chTabIxA=5;
//	chTabIxA={ 2'b00, chRov };

	if(chCtr0A[21])
	begin
		chTabIxA = chTabIxA + chTabPcmB3[7:2];
		chTabIxC = chTabIxC + chTabPcmB3[7:2];
	end

//	chTabPcmA	= cbSinTab[chTabIxA];
//	chTabPcmC	= cbSinTab[chTabIxC];
	chTabPcmA1	= chTabPcmA;
	chTabPcmBiA1	= 0;
	
	case(chCtr0A[31:28])
		4'h0: 		chTabPcmBiA1	= 8'h00;
		4'h1: 		chTabPcmBiA1	= 8'h28;
		4'h2: 		chTabPcmBiA1	= 8'h50;
		4'h3: 		chTabPcmBiA1	= 8'h28;
		4'h4: 		chTabPcmBiA1	= 8'h00;
		4'h5: 		chTabPcmBiA1	= 8'h28;
		4'h6: 		chTabPcmBiA1	= 8'h00;
		4'h7: 		chTabPcmBiA1	= 8'h00;
		default: 	chTabPcmBiA1	= 8'h00;
	endcase

	case(chCtr0A[31:28])
		4'h0: chTabPcmA1 = chTabPcmA;
		4'h1: chTabPcmA1 = chTabPcmA[7] ? 8'h00 : chTabPcmA;
		4'h2: chTabPcmA1 = chTabPcmA[7] ? ~chTabPcmA : chTabPcmA;
		4'h3: chTabPcmA1 = chStsA[18] ? 8'h00 : chTabPcmA;
		4'h4: chTabPcmA1 = chStsA[18] ? 8'h00 : chTabPcmC;
		4'h5: chTabPcmA1 = chStsA[18] ? 8'h00 :
			(chTabPcmC[7] ? ~chTabPcmC : chTabPcmC);
		4'h6: chTabPcmA1 = chTabPcmA[7] ? 8'h80 : 8'h7F;
//		4'h7: chTabPcmA1 = { !chTabIxA[5], chTabIxA[4:0], chTabIxA[4:3] };
		4'h7: chTabPcmA1 = { chTabIxA[5], ~chTabIxA[4:0], ~chTabIxA[5:4] };
		default: chTabPcmA1	= chTabPcmA;
	endcase

	/* Clock Edge */

`ifndef def_true
	chTabPcmA2A = chTabPcmA1B;
//	if(chRov[0])
//		chTabPcmA2A = ~chTabPcmA1B;

	if(chCtr0A[22])
//		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1B >>> 3);
		chTabPcmA2A = chTabPcmA2A -
			{ chTabPcmA1B[11]?3'b111:3'b000, chTabPcmA1B[11:3] };
	if(chCtr0A[23])
//		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1B >>> 2);
		chTabPcmA2A = chTabPcmA2A -
			{ chTabPcmA1B[11]?2'b11:2'b00, chTabPcmA1B[11:2] };
	if(chCtr0A[24])
//		chTabPcmA2A = chTabPcmA2A - (chTabPcmA1B >>> 1);
		chTabPcmA2A = chTabPcmA2A -
			{ chTabPcmA1B[11], chTabPcmA1B[11:1] };
		
	/* Clock Edge */

	case(chCtr0A[27:25])

//		3'b000: chTabPcmA3A = chTabPcmA2;
//		3'b001: chTabPcmA3A = chTabPcmA2 >>> 1;
//		3'b010: chTabPcmA3A = chTabPcmA2 >>> 2;
//		3'b011: chTabPcmA3A = chTabPcmA2 >>> 3;
//		3'b100: chTabPcmA3A = chTabPcmA2 >>> 4;
//		3'b101: chTabPcmA3A = chTabPcmA2 >>> 5;
//		3'b110: chTabPcmA3A = chTabPcmA2 >>> 6;
//		3'b111: chTabPcmA3A = chTabPcmA2 >>> 7;

		3'b000: chTabPcmA3A =
			chTabPcmA2;
		3'b001: chTabPcmA3A =
			{ chTabPcmA2[11], chTabPcmA2[11:1] };
		3'b010: chTabPcmA3A =
			{ chTabPcmA2[11]?2'b11:2'b00, chTabPcmA2[11:2] };
		3'b011: chTabPcmA3A =
			{ chTabPcmA2[11]?3'b111:3'b100, chTabPcmA2[11:3] };
		3'b100: chTabPcmA3A =
			{ chTabPcmA2[11]?4'b1111:4'b0000, chTabPcmA2[11:4] };
		3'b101: chTabPcmA3A = 
			{ chTabPcmA2[11]?5'b11111:5'b00000, chTabPcmA2[11:5] };
		3'b110: chTabPcmA3A = 
			{ chTabPcmA2[11]?6'b111111:6'b000000, chTabPcmA2[11:6] };
		3'b111: chTabPcmA3A = 
			{ chTabPcmA2[11]?7'b1111111:7'b0000000, chTabPcmA2[11:7] };

//		3'b000: chTabPcmA3A = chTabPcmA2 >>> 2;
//		3'b001: chTabPcmA3A = chTabPcmA2 >>> 3;
//		3'b010: chTabPcmA3A = chTabPcmA2 >>> 4;
//		3'b011: chTabPcmA3A = chTabPcmA2 >>> 5;
//		3'b100: chTabPcmA3A = chTabPcmA2 >>> 6;
//		3'b101: chTabPcmA3A = chTabPcmA2 >>> 7;
//		3'b110: chTabPcmA3A = chTabPcmA2 >>> 8;
//		3'b111: chTabPcmA3A = chTabPcmA2 >>> 9;

	endcase
`endif

	chTabPcmA3A = chTabPcmScA2;

	/* Clock Edge */

//	chPcmA = (chCtr0A[21]) ? chTabPcmA3 : (chTabPcmA3 + chTabPcmB3);
	chPcmA = 
		{ chTabPcmA3[11] ? 4'hF : 4'h0, chTabPcmA3[11:0] } + 
		( (chCtr0A[21]) ? 0 :
			{ chTabPcmB3[11] ? 4'hF : 4'h0, chTabPcmB3[11:0] } );

//	if(chRov[0])
//		chPcmA = ~chPcmA;

// `ifndef def_true
`ifdef def_true
	if(chCtr0A[35:32] != tFlushRov)
		chPcmA = 0;
//	if(chCtr0A[19:0] == 0)
	if(chCtr0A[19:8] == 0)
		chPcmA = 0;
`endif

//	if(chRovL3==chRov)
//	if((chRovL6==chRov) && (chRovL7!=chRov))
//	if((chRovL8==chRov) && (chRovL9!=chRov))
//	if((chRovL9==chRov) && (chRovL10!=chRov))
//	if((chRovL10==chRov) && (chRovL11!=chRov))
//	if(tTimer4MHzA)
//	if(tTimer4MHzB && !tTimer4MHzA)
	if(tTimer4MHzA && !tTimer4MHz)
	begin
		$display("ModFm: Sample Pulse %d, V=%X Ph=%X",
			chRov, chPcmA, chStsA[19:0]);
//		$display("%X %X", chTabPcmA, chTabPcmC);
//		tNxtChAccumL = tChAccumL + { (chPcmA[9] ? 2'h3 : 2'h0), chPcmA };
//		tNxtChAccumR = tChAccumR + { (chPcmA[9] ? 2'h3 : 2'h0), chPcmA };

//		tNxtChAccumL = tChAccumL + { (chPcmA[11] ? 4'hF : 4'h0), chPcmA };
//		tNxtChAccumR = tChAccumR + { (chPcmA[11] ? 4'hF : 4'h0), chPcmA };

		tNxtChAccumL = tChAccumL + chPcmA;		
		tChAccumOvfLb	= { tChAccumL[15], chPcmA[15], tNxtChAccumL[15] };
		tChAccumOvfLa	= (tChAccumOvfLb==3'b110) || (tChAccumOvfLb==3'b001);
		if(tChAccumOvfLa)
			tNxtChAccumL = tChAccumL;

`ifndef jx2_audio_leftonly
		tNxtChAccumR = tChAccumR + chPcmA;
		tChAccumOvfRb	= { tChAccumR[15], chPcmA[15], tNxtChAccumR[15] };
		tChAccumOvfRa	= (tChAccumOvfRb==3'b110) || (tChAccumOvfRb==3'b001);
		if(tChAccumOvfRa)
			tNxtChAccumR = tChAccumR;
`endif
	end

	/* Update output PCM. */

//	if(chRov==15)
//	if((chRov==15) && stepTimer1MHz)
//	if((chRov==15) && stepTimer4MHz)
//	if((chRov==15) && tTimer4MHz)
	if((chRov==15) && tTimer4MHz && !tTimer4MHzL)
	begin
		$display("ModFm: Ch15 Pulse %d", chRov);

//		tOutPcmL		= tChAccumL[7:0];
//		tOutPcmR		= tChAccumR[7:0];
//		if(tChAccumL[11:8] != (tChAccumL[7] ? 4'hF : 4'h0))
//			tOutPcmL = tChAccumL[11] ? 8'h80 : 8'h7F;
//		if(tChAccumR[11:8] != (tChAccumR[7] ? 4'hF : 4'h0))
//			tOutPcmR = tChAccumR[11] ? 8'h80 : 8'h7F;

//		if((tChAccumL[11:8] != 4'h0) && (tChAccumL[11:8] != 4'hF))
//			tOutPcmL = tChAccumL[11] ? 8'h81 : 8'h7E;
//		if((tChAccumR[11:8] != 4'h0) && (tChAccumR[11:8] != 4'hF))
//			tOutPcmR = tChAccumR[11] ? 8'h81 : 8'h7E;

//		tOutPcmL		= tChAccumL[9:2];
//		tOutPcmR		= tChAccumR[9:2];
//		if((tChAccumL[15:12] != 4'b00) && (tChAccumL[11:10] != 2'b11))
//			tOutPcmL = tChAccumL[11] ? 8'h81 : 8'h7E;
//		if((tChAccumR[15:12] != 4'b00) && (tChAccumR[11:10] != 2'b11))
//			tOutPcmR = tChAccumR[11] ? 8'h81 : 8'h7E;

// `ifdef def_true
`ifndef def_true
		tOutPcmL		= tChAccumL[13:2];
//		tOutPcmL		= tChAccumL[14:3];
//		if((tChAccumL[15:14] != 2'h0) && (tChAccumL[15:14] != 2'h3))
		if((tChAccumL[15:13] != 3'h0) && (tChAccumL[15:13] != 3'h7))
			tOutPcmL = tChAccumL[15] ? 12'h800 : 12'h7FF;

`ifndef jx2_audio_leftonly
		tOutPcmR		= tChAccumR[13:2];
//		tOutPcmR		= tChAccumR[14:3];
//		if((tChAccumR[15:14] != 2'h0) && (tChAccumR[15:14] != 2'h3))
		if((tChAccumR[15:13] != 3'h0) && (tChAccumR[15:13] != 3'h7))
			tOutPcmR = tChAccumR[15] ? 12'h800 : 12'h7FF;
`endif
`endif

`ifdef def_true
// `ifndef def_true
		tOutPcmL		= tChAccumL[12:1];
		if((tChAccumL[15:12] != 4'h0) && (tChAccumL[15:12] != 4'hF))
			tOutPcmL = tChAccumL[15] ? 12'h800 : 12'h7FF;

`ifndef jx2_audio_leftonly
		tOutPcmR		= tChAccumR[12:1];
		if((tChAccumR[15:12] != 4'h0) && (tChAccumR[15:12] != 4'hF))
			tOutPcmR = tChAccumR[15] ? 12'h800 : 12'h7FF;
`endif
`endif

// `ifdef def_true
`ifndef def_true
		tOutPcmL		= tChAccumL[12:5];
		if((tChAccumL[15:13] != 3'h0) && (tChAccumL[15:13] != 3'h7))
			tOutPcmL = tChAccumL[15] ? 8'h80 : 8'h7F;

`ifndef jx2_audio_leftonly
		tOutPcmR		= tChAccumR[12:5];
		if((tChAccumR[15:13] != 3'h0) && (tChAccumR[15:13] != 3'h7))
			tOutPcmR = tChAccumR[15] ? 8'h80 : 8'h7F;
`endif
`endif

// `ifdef def_true
`ifndef def_true
		tOutPcmL		= tChAccumL[11:4];
		if((tChAccumL[15:12] != 4'h0) && (tChAccumL[15:12] != 4'hF))
			tOutPcmL = tChAccumL[15] ? 8'h80 : 8'h7F;

`ifndef jx2_audio_leftonly
		tOutPcmR		= tChAccumR[11:4];
		if((tChAccumR[15:12] != 4'h0) && (tChAccumR[15:12] != 4'hF))
			tOutPcmR = tChAccumR[15] ? 8'h80 : 8'h7F;
`endif
`endif

// `ifdef def_true
`ifndef def_true
		tOutPcmL		= tChAccumL[10:3];
		if((tChAccumL[15:11] != 5'h0) && (tChAccumL[15:11] != 5'h1F))
			tOutPcmL = tChAccumL[15] ? 8'h80 : 8'h7F;

`ifndef jx2_audio_leftonly
		tOutPcmR		= tChAccumR[10:3];
		if((tChAccumR[15:11] != 5'h0) && (tChAccumR[15:11] != 5'h1F))
			tOutPcmR = tChAccumR[15] ? 8'h80 : 8'h7F;
`endif
`endif

		$display("ModFm: Ch15 Out %X / %X", tOutPcmL, tOutPcmR);

`ifdef jx2_fmsyn_dyndcbias
		tNxtChBiasL		= tChBiasL +
			{ tChAccumL[15] ? 16'hFFFF : 16'h0000, tChAccumL };
		tNxtChAccumL	= - tChBiasL[31:16];

`ifndef jx2_audio_leftonly
		tNxtChBiasR		= tChBiasR +
			{ tChAccumR[15] ? 16'hFFFF : 16'h0000, tChAccumR };
		tNxtChAccumR	= - tChBiasR[31:16];
`endif

//		tNxtChAccumL	= - { tChBiasL[15]?8'hFF:8'h00, tChBiasL[15:8] };
//		tNxtChAccumR	= - { tChBiasR[15]?8'hFF:8'h00, tChBiasR[15:8] };
`endif

`ifndef jx2_fmsyn_dyndcbias
		tNxtChAccumL	= 0;
		tNxtChAccumR	= 0;
`endif
	end

	if(tDevCSel && tBusOpm[3])
	begin
		tOutOK		= UMEM_OK_OK;

		if(tDevCSelChn)
		begin
			case(tBusAddr[4:2])
				3'b000: tOutData = regCtr0A[tBusAddr[8:5]][31:0];
				3'b001: tOutData = regCtr1A[tBusAddr[8:5]];
				3'b010: tOutData = regCtr2A[tBusAddr[8:5]];
				3'b011: tOutData = regStsA [tBusAddr[8:5]];
				3'b100: tOutData = regCtr0B[tBusAddr[8:5]][31:0];
				3'b101: tOutData = regCtr1B[tBusAddr[8:5]];
				3'b110: tOutData = regCtr2B[tBusAddr[8:5]];
				3'b111: tOutData = regStsB [tBusAddr[8:5]];
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

//	fracTimer1MHz	<= nextFracTimer1MHz;
//	fracTimer4MHz	<= nextFracTimer4MHz;
//	fracTimer32MHz	<= nextFracTimer32MHz;
//	fracTimer64kHz	<= nextFracTimer64kHz;

	tFlushRov		<= tNxtFlushRov;
	tFlushRovAdv	<= tNxtFlushRovAdv;
	tFlushRovAdvL	<= tFlushRovAdv;

	chCtr0A			<= regCtr0A[chRov];
	chCtr0B			<= regCtr0B[chRov];
	chStsA			<= regStsA[chRov];
	chStsB			<= regStsB[chRov];

//	if(chRov == chRovL3)
//	if(chRov == chRovL8)
//	if(chRov == chRovL4)
	if(chDoStoreSts)
	begin
		regStsA[chRov]	<= chNxtStsA;
		regStsB[chRov]	<= chNxtStsB;
	end

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
	chRovL10		<= chRovL9;
	chRovL11		<= chRovL10;

	chTabPcmA		<= cbSinTab[chTabIxA];
	chTabPcmB		<= cbSinTab[chTabIxB];
	chTabPcmC		<= cbSinTab[chTabIxC];
	chTabPcmD		<= cbSinTab[chTabIxD];

	chClkStrobe		<= chNxtClkStrobe;
	chClkLatch		<= chNxtClkLatch;
	tChAccumL		<= tNxtChAccumL;
	tChAccumR		<= tNxtChAccumR;
	tChBiasL		<= tNxtChBiasL;
	tChBiasR		<= tNxtChBiasR;

//	chTabPcmA1B		<= chTabPcmA1;
//	chTabPcmB1B		<= chTabPcmB1;

//	chTabPcmA1B		<= { chTabPcmA1[7] ? 4'hF : 4'h0, chTabPcmA1 };
//	chTabPcmB1B		<= { chTabPcmB1[7] ? 4'hF : 4'h0, chTabPcmB1 };

//	chTabPcmA1B		<= { chTabPcmA1[7] ? 2'h3 : 2'h0, chTabPcmA1, 2'b0 };
//	chTabPcmB1B		<= { chTabPcmB1[7] ? 2'h3 : 2'h0, chTabPcmB1, 2'b0 };

`ifndef def_true
	chTabPcmA1B		<=
		{ chTabPcmA1[7] ? 2'h3 : 2'h0, chTabPcmA1, 2'b0 } -
		{ chTabPcmBiA1[7] ? 2'h3 : 2'h0, chTabPcmBiA1, 2'b0 };
	chTabPcmB1B		<=
		{ chTabPcmB1[7] ? 2'h3 : 2'h0, chTabPcmB1, 2'b0 } -
		{ chTabPcmBiB1[7] ? 2'h3 : 2'h0, chTabPcmBiB1, 2'b0 };
`endif

`ifdef def_true
	chTabPcmA1B		<=
		{ chTabPcmA1[7], chTabPcmA1, 3'b0 } -
		{ chTabPcmBiA1[7], chTabPcmBiA1, 3'b0 };
	chTabPcmB1B		<=
		{ chTabPcmB1[7], chTabPcmB1, 3'b0 } -
		{ chTabPcmBiB1[7], chTabPcmBiB1, 3'b0 };
`endif

//	chTabPcmA1B		<= { chTabPcmA1[7], chTabPcmA1, 3'b0 };
//	chTabPcmB1B		<= { chTabPcmB1[7], chTabPcmB1, 3'b0 };

//	chTabPcmA2		<= chTabPcmA2A;
//	chTabPcmB2		<= chTabPcmB2A;
	chTabPcmA3		<= chTabPcmA3A;
	chTabPcmB3		<= chTabPcmB3A;

	if(tDevCSel && tBusOpm[4])
	begin
		if(tDevCSelChn)
		begin
			case(tBusAddr[4:2])
				3'b000: regCtr0A[tBusAddr[8:5]]	<= { tFlushRov, tBusInData };
				3'b001: regCtr1A[tBusAddr[8:5]]	<= tBusInData;
				3'b010: regCtr2A[tBusAddr[8:5]]	<= tBusInData;
				3'b100: regCtr0B[tBusAddr[8:5]]	<= { tFlushRov, tBusInData };
				3'b101: regCtr1B[tBusAddr[8:5]]	<= tBusInData;
				3'b110: regCtr2B[tBusAddr[8:5]]	<= tBusInData;
				default: begin
				end
			endcase
		end
	end

	tTimer4MHzB		<= timer4MHz;
	tTimer4MHzA		<= tTimer4MHzB;
	tTimer4MHz		<= tTimer4MHzA;

//	tTimer4MHz		<= timer4MHz;
	tTimer4MHzL		<= tTimer4MHz;
	tTimer1MHz		<= timer1MHz;
	tTimer64kHz		<= timer64kHz;
	tTimer1kHz		<= timer1kHz;
	tTimerNoise		<= timerNoise;
end

endmodule
