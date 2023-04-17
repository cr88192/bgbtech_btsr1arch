/*
PCM Mixer

Audio samples will be assumed to be in 8-bit A-Law format.
 */

`include "CoreDefs.v"

`include "ModFmScVol6.v"
`include "ExConv_Al8Pcm12.v"

module ModAudMix(
	/* verilator lint_off UNUSED */
	clock,		reset,
	outPcmL,	outPcmR,
	busInData,	busOutData,
	busAddr,	busOpm,		busOK,
	cellIdx,	cellData,
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

output[15:0]	cellIdx;
input[127:0]	cellData;

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

reg[15:0]	tCellIdx;
reg[15:0]	tCellIdx2;
reg[127:0]	tCellData;
assign		cellIdx	= tCellIdx2;

wire		tDevCSel;
wire		tDevCSelChn;
assign		tDevCSel		= (tBusAddr[27:16] == 12'h008);
assign		tDevCSelChn		= (tBusAddr[15: 9] == 7'b1101_000);

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

// reg[7:0]	cbSinTab[63:0];
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

wire[11:0]	chTabPcmA1_Al8;
ExConv_Al8Pcm12	mixAl8(chTabPcmA1, chTabPcmA1_Al8);


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

always @*
begin
	tOutData	= UV32_00;
	tOutOK		= UMEM_OK_READY;
	tCellIdx	= 0;

	tNxtFlushRovAdv = 0;

	if(reset)
		tNxtFlushRovAdv = 1;
	if(tFlushRov == 0)
		tNxtFlushRovAdv = 1;

	tNxtFlushRov	= tFlushRov;
	if(tFlushRovAdv && !tFlushRovAdvL)
		tNxtFlushRov	= tFlushRov + 1;
	
	chNxtRov		= chRov;
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

	if(	(chRov == 0) &&
		(chRovL1 != chRovL2))
	begin
		chNxtClkStrobe	= chClkLatch;
		chNxtClkLatch	= 0;
	end
	
	if(tTimer64kHz)
	begin
		chNxtClkLatch	= 1;
	end
	
	/* First Clock */
	
	chNxtStsA	= chStsA;
	chNxtStsB	= chStsB;
	chDoStoreSts	= 0;
	
	if(chClkStrobe &&
		(chRovL4 == chRov) &&
		(chRovL5 != chRov) )
	begin
//		$display("ModeFm: Step %d", chRov);

//		chNxtStsA[19:0]	= chStsA[19:0] + chCtr0A[19:0];
//		chNxtStsB[19:0]	= chStsB[19:0] + chCtr0B[19:0];

		chNxtStsB[31:0]	= chStsB[31:0] + { 12'h00, chCtr0A[19:0] };

		if((chStsB[31:16] >= chCtr3A[31:16]) && !chCtr0A[21])
		begin
			/* End of sample and not loop, Hold */
			chNxtStsB[31:16] = chCtr3A[31:16];
		end

		if((chStsB[31:16] >= chCtr3B[31:16]) && chCtr0A[21])
		begin
			/* End of Loop and Loop, Return Loop Start */
			chNxtStsB[31:16] = chCtr3B[15:0];
		end

		chDoStoreSts	= 1;

	end


	/* Generate output PCM over several cycles. */

	chTabIxB	= chStsB[19:14];
	chTabIxD	= chStsB[18:13];
	chTabPcmB1		= chTabPcmB;
	chTabPcmBiB1	= 0;

	tCellIdx	= chCtr1A[15:0] + { 4'h0, chStsB[31:20] };

	/* Clock Edge */

	chTabPcmB3A = chTabPcmScB2;


	/* Clock Edge */

	chTabIxA	= chStsA[19:14];
	chTabIxC	= chStsA[18:13];

	if(chCtr0A[21])
	begin
		chTabIxA = chTabIxA + chTabPcmB3[9:4];
		chTabIxC = chTabIxC + chTabPcmB3[9:4];
	end

//	chTabPcmA1	= chTabPcmA;
	chTabPcmBiA1	= 0;

	case(chStsB[19:16])
		4'h0: chTabPcmA1	= tCellData[  7:  0];
		4'h1: chTabPcmA1	= tCellData[ 15:  8];
		4'h2: chTabPcmA1	= tCellData[ 23: 16];
		4'h3: chTabPcmA1	= tCellData[ 31: 24];
		4'h4: chTabPcmA1	= tCellData[ 39: 32];
		4'h5: chTabPcmA1	= tCellData[ 47: 40];
		4'h6: chTabPcmA1	= tCellData[ 55: 48];
		4'h7: chTabPcmA1	= tCellData[ 63: 56];
		4'h8: chTabPcmA1	= tCellData[ 71: 64];
		4'h9: chTabPcmA1	= tCellData[ 79: 72];
		4'hA: chTabPcmA1	= tCellData[ 87: 80];
		4'hB: chTabPcmA1	= tCellData[ 95: 88];
		4'hC: chTabPcmA1	= tCellData[103: 96];
		4'hD: chTabPcmA1	= tCellData[111:104];
		4'hE: chTabPcmA1	= tCellData[119:112];
		4'hF: chTabPcmA1	= tCellData[127:120];
	endcase

	/* Clock Edge */

	chTabPcmA3A = chTabPcmScA2;

	/* Clock Edge */

	chPcmA = 
		{ chTabPcmA3[11] ? 4'hF : 4'h0, chTabPcmA3[11:0] } + 
		( (chCtr0A[21]) ? 0 :
			{ chTabPcmB3[11] ? 4'hF : 4'h0, chTabPcmB3[11:0] } );

// `ifndef def_true
`ifdef def_true
	if(chCtr0A[35:32] != tFlushRov)
		chPcmA = 0;
	if(chCtr0A[19:8] == 0)
		chPcmA = 0;
`endif

	if(tTimer4MHzA && !tTimer4MHz)
	begin
		tNxtChAccumL = tChAccumL + chPcmA;		
		tChAccumOvfLb	= { tChAccumL[15], chPcmA[15], tNxtChAccumL[15] };
		tChAccumOvfLa	= (tChAccumOvfLb==3'b110) || (tChAccumOvfLb==3'b001);
		if(tChAccumOvfLa)
			tNxtChAccumL = tChAccumL;
	end

	/* Update output PCM. */

	if((chRov==15) && tTimer4MHz && !tTimer4MHzL)
	begin

`ifdef def_true
		tOutPcmL		= tChAccumL[12:1];
		if((tChAccumL[15:12] != 4'h0) && (tChAccumL[15:12] != 4'hF))
			tOutPcmL = tChAccumL[15] ? 12'h800 : 12'h7FF;
		tOutPcmR		= tChAccumR[12:1];
		if((tChAccumR[15:12] != 4'h0) && (tChAccumR[15:12] != 4'hF))
			tOutPcmR = tChAccumR[15] ? 12'h800 : 12'h7FF;
`endif

`ifdef jx2_fmsyn_dyndcbias
		tNxtChBiasL		= tChBiasL +
			{ tChAccumL[15] ? 16'hFFFF : 16'h0000, tChAccumL };
		tNxtChAccumL	= - tChBiasL[31:16];
		tNxtChBiasR		= tChBiasR +
			{ tChAccumR[15] ? 16'hFFFF : 16'h0000, tChAccumR };
		tNxtChAccumR	= - tChBiasR[31:16];
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

	tCellIdx2		<= tCellIdx;
	tCellData		<= cellData;

	tFlushRov		<= tNxtFlushRov;
	tFlushRovAdv	<= tNxtFlushRovAdv;
	tFlushRovAdvL	<= tFlushRovAdv;

	chCtr0A			<= regCtr0A[chRov];
	chCtr0B			<= regCtr0B[chRov];
	chStsA			<= regStsA[chRov];
	chStsB			<= regStsB[chRov];

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

//	chTabPcmA		<= cbSinTab[chTabIxA];
//	chTabPcmB		<= cbSinTab[chTabIxB];
//	chTabPcmC		<= cbSinTab[chTabIxC];
//	chTabPcmD		<= cbSinTab[chTabIxD];

	chClkStrobe		<= chNxtClkStrobe;
	chClkLatch		<= chNxtClkLatch;
	tChAccumL		<= tNxtChAccumL;
	tChAccumR		<= tNxtChAccumR;
	tChBiasL		<= tNxtChBiasL;
	tChBiasR		<= tNxtChBiasR;

`ifdef def_true
//	chTabPcmA1B		<=
//		{ chTabPcmA1[7], chTabPcmA1, 3'b0 } -
//		{ chTabPcmBiA1[7], chTabPcmBiA1, 3'b0 };
//	chTabPcmB1B		<=
//		{ chTabPcmB1[7], chTabPcmB1, 3'b0 } -
//		{ chTabPcmBiB1[7], chTabPcmBiB1, 3'b0 };
`endif

	chTabPcmA1B		<= chTabPcmA1_Al8;
	chTabPcmB1B		<= chTabPcmA1_Al8;

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

	tTimer4MHzL		<= tTimer4MHz;
	tTimer1MHz		<= timer1MHz;
	tTimer64kHz		<= timer64kHz;
	tTimer1kHz		<= timer1kHz;
	tTimerNoise		<= timerNoise;
end

endmodule
