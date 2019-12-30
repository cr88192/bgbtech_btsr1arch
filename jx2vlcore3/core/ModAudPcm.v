/*
Simple PCM Sound Device

Sample formats:
  Linear PCM
  A-Law
  ...

S.E3.M4
	E=7: 16384..32767 (10)
	E=6:  8192..16383 ( 9)
	E=5:  4096.. 8191 ( 8)
	E=4:  2048.. 4095 ( 7)
	E=3:  1024.. 2047 ( 6)
	E=2:   512.. 1023 ( 5)
	E=1:   256..  511 ( 4)
	E=0:     0..  255 ( 4, Denorm)
 */

`include "CoreDefs.v"

// `define		JX2_AUD_16K
// `define		JX2_AUD_STEREO

module ModAudPcm(
	/* verilator lint_off UNUSED */
	clock,		reset,
	pwmOut,		auxPcmL,	auxPcmR,
	busInData,	busOutData,	busAddr,
	busOpm,		busOK,
	timer1MHz,	timer64kHz,
	timer1kHz,	timerNoise);

input			clock;
input			reset;
input[31:0]		busAddr;
input[31:0]		busInData;
input[4:0]		busOpm;

output[1:0]		pwmOut;
output[31:0]	busOutData;
output[1:0]		busOK;

input[7:0]		auxPcmL;
input[7:0]		auxPcmR;

input			timer1MHz;
input			timer64kHz;
input			timer1kHz;
input			timerNoise;

reg[31:0]		tBusAddr;
reg[31:0]		tBusInData;
reg[4:0]		tBusOpm;

reg[1:0]	tPwmOut;
reg[1:0]	tPwmOut2;
reg[31:0]	tOutData;
reg[31:0]	tOutData2;
reg[1:0]	tOutOK;
reg[1:0]	tOutOK2;

assign		pwmOut		= tPwmOut2;
assign		busOutData	= tOutData2;
assign		busOK		= tOutOK2;

`ifdef JX2_AUD_16K
reg[31:0]	pcmMemA[2047:0];
reg[31:0]	pcmMemB[2047:0];
`else
reg[31:0]	pcmMemA[1023:0];
reg[31:0]	pcmMemB[1023:0];
`endif

`ifdef JX2_AUD_STEREO
`ifdef JX2_AUD_16K
reg[31:0]	pcmMemC[2047:0];
reg[31:0]	pcmMemD[2047:0];
`else
reg[31:0]	pcmMemC[1023:0];
reg[31:0]	pcmMemD[1023:0];
`endif
`endif

`ifdef JX2_AUD_STEREO
reg[63:0]	tPcmBlkL;
reg[63:0]	tPcmBlkR;
reg[63:0]	tPcmBlk;
`else
reg[63:0]	tPcmBlk;
`endif

`ifdef JX2_AUD_16K
reg[10:0]	tPcmIdx;
`else
reg[9:0]	tPcmIdx;
`endif

`ifndef def_true
reg[11:0]	tPwmStL;
reg[11:0]	tPwmStR;
reg[11:0]	tPwmNextStL;
reg[11:0]	tPwmNextStR;
reg[11:0]	tPwmValL;
reg[11:0]	tPwmValR;
reg[11:0]	tPwmNextValL;
reg[11:0]	tPwmNextValR;
`endif

`ifdef def_true
reg[15:0]	tPwmStL;
reg[15:0]	tPwmStR;
reg[15:0]	tPwmNextStL;
reg[15:0]	tPwmNextStR;
reg[15:0]	tPwmValL;
reg[15:0]	tPwmValR;
reg[15:0]	tPwmNextValL;
reg[15:0]	tPwmNextValR;

reg[15:0]	tPwmAddValL;
reg[15:0]	tPwmAddValR;

reg[15:0]	tPcmValL;
reg[15:0]	tPcmValR;
reg[15:0]	tPcmNextValL;
reg[15:0]	tPcmNextValR;
`endif

reg[7:0]	tAuxPcmL;
reg[7:0]	tAuxPcmR;

reg			tPwmStCfL;
reg			tPwmStCfR;

reg[13:0]	tClkDivCnt;
reg[13:0]	tNxtClkDivCnt;
reg[13:0]	tClkDivRst;
reg[13:0]	tNxtClkDivRst;

reg			tClk1kHz;
reg			tNxtClk1kHz;

`ifdef JX2_AUD_16K
reg[13:0]	tSampPos;
reg[13:0]	tNxtSampPos;
`else
reg[12:0]	tSampPos;
reg[12:0]	tNxtSampPos;
`endif

reg[7:0]	tSamp8b;
reg[7:0]	tNxtSamp8b;
reg[10:0]	tSamp11a;
reg[11:0]	tSamp12b;
reg[11:0]	tSamp12c;

reg[15:0]	tSamp16b;
reg[15:0]	tNxtSamp16b;

reg			tPcmCarryL;
reg			tPcmCarryR;

reg			tUseCompand;
reg			tUse16b;
reg			tOutEnable;
reg			tIsStereo;
reg			tIsStereoR;

reg[31:0]	tRegCtrl0;

wire		tDevCSel;
wire		tDevCSelAuL;
wire		tDevCSelAuR;
wire		tDevCSelCtr;
// assign		tDevCSel = (busAddr[27:16] == 12'h009);
// assign		tDevCSelAuL = (busAddr[15:14] == 2'h0);
// assign		tDevCSelAuR = (busAddr[15:14] == 2'h1);
// assign		tDevCSelCtr = (busAddr[15:12] == 4'hF);

assign		tDevCSel = (tBusAddr[27:16] == 12'h009);
assign		tDevCSelAuL = (tBusAddr[15:14] == 2'h0);
assign		tDevCSelAuR = (tBusAddr[15:14] == 2'h1);
assign		tDevCSelCtr = (tBusAddr[15:12] == 4'hF);

initial begin
	pcmMemA[0] = UV32_FF;
	pcmMemB[0] = UV32_00;
	pcmMemA[1] = UV32_FF;
	pcmMemB[1] = UV32_00;
end

always @*
begin
//	tOutData		= UV32_XX;
	tOutData		= UV32_00;
	tOutOK			= UMEM_OK_READY;
	tPwmNextValL	= tPwmValL;
	tPwmNextValR	= tPwmValR;
	tPcmNextValL	= tPcmValL;
	tPcmNextValR	= tPcmValR;

//	tNxtClkDivCnt	= tClkDivCnt + 1;
	tNxtClkDivCnt	= tClkDivCnt - 1;
	tNxtSampPos		= tSampPos;
//	tUseCompand		= 1;

	tUseCompand		= tRegCtrl0[0];
	tUse16b			= tRegCtrl0[1];
	tIsStereo		= tRegCtrl0[2];
	tOutEnable		= tRegCtrl0[3];

	tIsStereoR		= tClkDivCnt[6];

//	tOutEnable = 1;

//	tNxtClk1kHz		= timer1kHz ? !tClk1kHz : tClk1kHz;

	{tPwmStCfL, tPwmNextStL} = {1'b0, tPwmStL} + {1'b0, tPwmValL};
	{tPwmStCfR, tPwmNextStR} = {1'b0, tPwmStR} + {1'b0, tPwmValR};
	tPwmOut = tOutEnable ? { tPwmStCfR, tPwmStCfL } : 2'b11;
//	tPwmOut = tOutEnable ? { tPwmStCfR, tPwmStCfL } : { tClk1kHz, tClk1kHz };

//	if(tOutEnable)
//		$display("Ctrl=%X, Vl=%X, pwm=%d", tRegCtrl0, tPwmValL, tPwmStCfL);

	case(tRegCtrl0[7:4])
		4'h0: tNxtClkDivRst=12500;
		4'h1: tNxtClkDivRst= 9070;
		4'h2: tNxtClkDivRst= 6250;
		4'h3: tNxtClkDivRst= 4535;
		4'h4: tNxtClkDivRst= 3125;
		4'h5: tNxtClkDivRst= 2268;
		4'h6: tNxtClkDivRst= 1562;
		4'h7: tNxtClkDivRst= 1134;
		4'h8: tNxtClkDivRst=  781;
		4'h9: tNxtClkDivRst= 2083;
		4'hA: tNxtClkDivRst=  781;
		4'hB: tNxtClkDivRst= 1042;
		4'hC: tNxtClkDivRst=  781;
		4'hD: tNxtClkDivRst=  781;
		4'hE: tNxtClkDivRst=  781;
		4'hF: tNxtClkDivRst=  781;
	endcase

//	if(tClkDivCnt==6250)
//	if(tClkDivCnt==tClkDivRst)
	if(tClkDivCnt==0)
	begin
//		tNxtClkDivCnt	= 0;
		tNxtClkDivCnt	= tClkDivRst;
		tNxtSampPos		= tSampPos + 1;
	end
	
`ifdef JX2_AUD_16K
	tPcmIdx = tUse16b ? tSampPos[12:2] : tSampPos[13:3];
`else
	tPcmIdx = tUse16b ? tSampPos[11:2] : tSampPos[12:3];
`endif

`ifdef JX2_AUD_STEREO
	tPcmBlk = tIsStereoR ? tPcmBlkR : tPcmBlkL;
`endif

	case(tSampPos[2:0])
		3'b000: tNxtSamp8b	= tPcmBlk[ 7: 0];
		3'b001: tNxtSamp8b	= tPcmBlk[15: 8];
		3'b010: tNxtSamp8b	= tPcmBlk[23:16];
		3'b011: tNxtSamp8b	= tPcmBlk[31:24];
		3'b100: tNxtSamp8b	= tPcmBlk[39:32];
		3'b101: tNxtSamp8b	= tPcmBlk[47:40];
		3'b110: tNxtSamp8b	= tPcmBlk[55:48];
		3'b111: tNxtSamp8b	= tPcmBlk[63:56];
	endcase

	case(tSampPos[1:0])
		2'b00: tNxtSamp16b	= tPcmBlk[15: 0];
		2'b01: tNxtSamp16b	= tPcmBlk[31:16];
		2'b10: tNxtSamp16b	= tPcmBlk[47:32];
		2'b11: tNxtSamp16b	= tPcmBlk[63:48];
	endcase
	
	case(tSamp8b[6:4])
		3'b000: tSamp11a={7'h0, tSamp8b[3:0]                    };
		3'b001: tSamp11a={7'h1, tSamp8b[3:0]                    };
		3'b010: tSamp11a={6'h1, tSamp8b[3:0], tSamp8b[  3]      };
		3'b011: tSamp11a={5'h1, tSamp8b[3:0], tSamp8b[3:2]      };
		3'b100: tSamp11a={4'h1, tSamp8b[3:0], tSamp8b[3:1]      };
		3'b101: tSamp11a={3'h1, tSamp8b[3:0], tSamp8b[3:0]      };
		3'b110: tSamp11a={2'h1, tSamp8b[3:0], tSamp8b[3:0], 1'h0};
		3'b111: tSamp11a={1'h1, tSamp8b[3:0], tSamp8b[3:0], 2'h0};
	endcase
	
//	tSamp12b = (tSamp8b[7]) ?
//		{ 1'b1,  tSamp11a } :
//		{ 1'b0, ~tSamp11a };

	tSamp12b = (tSamp8b[7]) ?
		{ 1'b0,  tSamp11a } :
		{ 1'b1, ~tSamp11a };
	
//	tSamp12c = tUse16b ?
//		{ tSamp16b[15]^tUseCompand, tSamp16b[14:4] } :
//		(tUseCompand ? tSamp12b : { tSamp8b, 4'h0 });

	tSamp12c = tUse16b ?
		{ tSamp16b[15]^(~tUseCompand), tSamp16b[14:4] } :
		(tUseCompand ? tSamp12b : { ~tSamp8b[7], tSamp8b[6:0], 4'h0 });
	
	if(tIsStereo)
	begin
		if(tIsStereoR)
//			tPcmNextValR	= tSamp12c;
			tPcmNextValR	= { tSamp12c, tSamp12c[11:8] };
		else
//			tPcmNextValL	= tSamp12c;
			tPcmNextValL	= { tSamp12c, tSamp12c[11:8] };
	end
	else
	begin
//		tPcmNextValL	= tSamp12c;
//		tPcmNextValR	= tSamp12c;

		tPcmNextValL	= { tSamp12c, tSamp12c[11:8] };
		tPcmNextValR	= { tSamp12c, tSamp12c[11:8] };
	end
	
//	tPwmNextValL	= tPcmNextValL + { auxPcmL, 3'h0, timerNoise };
//	tPwmNextValR	= tPcmNextValR + { auxPcmR, 3'h0, timerNoise };

//	tPwmNextValL	= tPcmNextValL + { auxPcmL, 7'h0, timerNoise };
//	tPwmNextValR	= tPcmNextValR + { auxPcmR, 7'h0, timerNoise };

`ifndef def_true
	tPwmAddValL = 
		{ tPcmValL[15] ? 2'b11 : 2'b00, tPcmValL[15:2] } +
		{ tAuxPcmL[ 7] ? 2'b11 : 2'b00, tAuxPcmL, 5'h0, timerNoise };
	tPwmAddValR =
		{ tPcmValR[15] ? 2'b11 : 2'b00, tPcmValR[15:2] } +
		{ tAuxPcmR[ 7] ? 2'b11 : 2'b00, tAuxPcmR, 5'h0, timerNoise };
	tPwmNextValL = tPwmAddValL;
	tPwmNextValR = tPwmAddValR;
`endif

`ifdef def_true
	tPwmAddValL = 
		{ tPcmValL[15], tPcmValL[15:1] } +
		{ tAuxPcmL[ 7], tAuxPcmL[7:0], 6'h0, timerNoise };
	tPwmAddValR =
		{ tPcmValR[15], tPcmValR[15:1] } +
		{ tAuxPcmR[ 7], tAuxPcmR[7:0], 6'h0, timerNoise };
	tPwmNextValL = tPwmAddValL;
	tPwmNextValR = tPwmAddValR;
`endif

`ifndef def_true
	{ tPcmCarryL, tPwmAddValL }	= 
		{ 1'b0, tPcmValL } + { 1'b0, tAuxPcmL, 7'h0, timerNoise };
	{ tPcmCarryR, tPwmAddValR }	=
		{ 1'b0, tPcmValR } + { 1'b0, tAuxPcmR, 7'h0, timerNoise };

	case({tPwmAddValL[15], tPcmValL[15], tAuxPcmL[7]})
		3'b000: tPwmNextValL = tPwmAddValL;
		3'b001: tPwmNextValL = tPwmAddValL;
		3'b010: tPwmNextValL = tPwmAddValL;
		3'b011: tPwmNextValL = 16'h8000;
		3'b100: tPwmNextValL = 16'h7FFF;
		3'b101: tPwmNextValL = tPwmAddValL;
		3'b110: tPwmNextValL = tPwmAddValL;
		3'b111: tPwmNextValL = tPwmAddValL;
	endcase

	case({tPwmAddValR[15], tPcmValR[15], tAuxPcmR[7]})
		3'b000: tPwmNextValR = tPwmAddValR;
		3'b001: tPwmNextValR = tPwmAddValR;
		3'b010: tPwmNextValR = tPwmAddValR;
		3'b011: tPwmNextValR = 16'h8000;
		3'b100: tPwmNextValR = 16'h7FFF;
		3'b101: tPwmNextValR = tPwmAddValR;
		3'b110: tPwmNextValR = tPwmAddValR;
		3'b111: tPwmNextValR = tPwmAddValR;
	endcase
`endif
	
//	if(tPcmCarryL != tPwmNextValL[15])
//		tPwmNextValL = tPcmValL[15] ? 16'h8000 : 16'h7FFF;

//	if(tPcmCarryR != tPwmNextValR[15])
//		tPwmNextValR = tPcmValR[15] ? 16'h8000 : 16'h7FFF;

	if(tDevCSel && (tBusOpm[4:3]!=0))
		tOutOK	= UMEM_OK_OK;
		
end

always @(posedge clock)
begin
	tPwmOut2	<= tPwmOut;
//	tClk1kHz	<= tNxtClk1kHz;

//	if(timer1MHz)
//	begin
		tPwmStL		<= tPwmNextStL;
		tPwmStR		<= tPwmNextStR;
//	end

	tPcmValL	<= tPcmNextValL;
	tPcmValR	<= tPcmNextValR;

	tAuxPcmL	<= auxPcmL;
	tAuxPcmR	<= auxPcmR;
//	tAuxPcmL	<= { ~auxPcmL[7], auxPcmL[6:0] };
//	tAuxPcmR	<= { ~auxPcmR[7], auxPcmR[6:0] };

//	tPwmValL	<= tPwmNextValL;
//	tPwmValR	<= tPwmNextValR;
	tPwmValL	<= { ~tPwmNextValL[15], tPwmNextValL[14:0] };
	tPwmValR	<= { ~tPwmNextValR[15], tPwmNextValR[14:0] };

	tClkDivCnt	<= tNxtClkDivCnt;
	tClkDivRst	<= tNxtClkDivRst;
	tSampPos	<= tNxtSampPos;
	tSamp8b		<= tNxtSamp8b;
	tSamp16b	<= tNxtSamp16b;

	tOutData2	<= tOutData;
	tOutOK2		<= tOutOK;

	tBusAddr	<= busAddr;
	tBusInData	<= busInData;
	tBusOpm		<= busOpm;

//	$display("A=%X D=%X Opm=%X, Cs=%d CsAuL=%d CsAuR=%d CsCtl=%d",
//		tBusAddr, tBusInData, tBusOpm,
//		tDevCSel, tDevCSelAuL, tDevCSelAuR, tDevCSelCtr);

`ifdef JX2_AUD_STEREO
	tPcmBlkL	<= { pcmMemB[tPcmIdx], pcmMemA[tPcmIdx] };
	tPcmBlkR	<= { pcmMemD[tPcmIdx], pcmMemC[tPcmIdx] };
`else
	tPcmBlk		<= { pcmMemB[tPcmIdx], pcmMemA[tPcmIdx] };
`endif
	
	if(tDevCSel && tBusOpm[4])
	begin
		if(tDevCSelCtr)
		begin
			case(tBusAddr[7:2])
				6'h00:
				begin
//					$display("ModAudPcm: Ctrl0=%X", tRegCtrl0);
					tRegCtrl0	<= tBusInData;
				end

				default: begin
				end
			endcase
		end
	
		if(tDevCSelAuL)
		begin
`ifdef JX2_AUD_16K
			if(tBusAddr[2])
				pcmMemB[tBusAddr[13:3]]	<= tBusInData;
			else
				pcmMemA[tBusAddr[13:3]]	<= tBusInData;
`else
			if(busAddr[2])
				pcmMemB[tBusAddr[12:3]]	<= tBusInData;
			else
				pcmMemA[tBusAddr[12:3]]	<= tBusInData;
`endif
		end

`ifdef JX2_AUD_STEREO
		if(tDevCSelAuR)
		begin
`ifdef JX2_AUD_16K
			if(tBusAddr[2])
				pcmMemD[tBusAddr[13:3]]	<= tBusInData;
			else
				pcmMemC[tBusAddr[13:3]]	<= tBusInData;
`else
			if(busAddr[2])
				pcmMemD[tBusAddr[12:3]]	<= tBusInData;
			else
				pcmMemC[tBusAddr[12:3]]	<= tBusInData;
`endif
		end
`endif

	end
end

endmodule
