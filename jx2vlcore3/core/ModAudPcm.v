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
	clock, reset, pwmOut,
	busAddr, busInData, busOutData, busOpm, busOK);

input			clock;
input			reset;
input[31:0]		busAddr;
input[31:0]		busInData;
input[4:0]		busOpm;

output[1:0]		pwmOut;
output[31:0]	busOutData;
output[1:0]		busOK;

reg[1:0]	tPwmOut;
reg[1:0]	tPwmOut2;
reg[31:0]	tOutData;
reg[1:0]	tOutOK;

assign		pwmOut		= tPwmOut2;
assign		busOutData	= tOutData;
assign		busOK		= tOutOK;

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

reg[11:0]	tPwmStL;
reg[11:0]	tPwmStR;
reg[11:0]	tPwmNextStL;
reg[11:0]	tPwmNextStR;
reg[11:0]	tPwmValL;
reg[11:0]	tPwmValR;
reg[11:0]	tPwmNextValL;
reg[11:0]	tPwmNextValR;
reg			tPwmStCfL;
reg			tPwmStCfR;

reg[13:0]	tClkDivCnt;
reg[13:0]	tNxtClkDivCnt;
reg[13:0]	tClkDivRst;
reg[13:0]	tNxtClkDivRst;


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
assign		tDevCSel = (busAddr[27:16] == 12'h009);
assign		tDevCSelAuL = (busAddr[15:14] == 2'h0);
assign		tDevCSelAuR = (busAddr[15:14] == 2'h1);
assign		tDevCSelCtr = (busAddr[15:12] == 4'hF);

always @*
begin
	tOutData		= UV32_XX;
	tOutOK			= UMEM_OK_READY;
	tPwmNextValL	= tPwmValL;
	tPwmNextValR	= tPwmValR;
//	tNxtClkDivCnt	= tClkDivCnt + 1;
	tNxtClkDivCnt	= tClkDivCnt - 1;
	tNxtSampPos		= tSampPos;
//	tUseCompand		= 1;

	tUseCompand		= tRegCtrl0[0];
	tUse16b			= tRegCtrl0[1];
	tIsStereo		= tRegCtrl0[2];
	tOutEnable		= tRegCtrl0[3];

	tIsStereoR		= tClkDivCnt[6];

	{tPwmStCfL, tPwmNextStL} = {1'b0, tPwmStL} + {1'b0, tPwmValL};
	{tPwmStCfR, tPwmNextStR} = {1'b0, tPwmStR} + {1'b0, tPwmValR};
	tPwmOut = tOutEnable ? { tPwmStCfR, tPwmStCfL } : 2'b11;

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
	
	tSamp12b = (tSamp8b[7]) ?
		{ 1'b1,  tSamp11a } :
		{ 1'b0, ~tSamp11a };
	
	tSamp12c = tUse16b ?
		{ tSamp16b[15]^tUseCompand, tSamp16b[14:4] } :
		(tUseCompand ? tSamp12b : { tSamp8b, 4'h0 });
	
	if(tIsStereo)
	begin
		if(tIsStereoR)
			tPwmNextValR	= tSamp12c;
		else
			tPwmNextValL	= tSamp12c;
	end
	else
	begin
		tPwmNextValL	= tSamp12c;
		tPwmNextValR	= tSamp12c;
	end

	if(tDevCSel && (busOpm[4:3]!=0))
		tOutOK	= UMEM_OK_OK;
		
end

always @(posedge clock)
begin
	tPwmOut2	<= tPwmOut;
	tPwmStL		<= tPwmNextStL;
	tPwmStR		<= tPwmNextStR;
	tPwmValL	<= tPwmNextValL;
	tPwmValR	<= tPwmNextValR;
	tClkDivCnt	<= tNxtClkDivCnt;
	tClkDivRst	<= tNxtClkDivRst;
	tSampPos	<= tNxtSampPos;
	tSamp8b		<= tNxtSamp8b;
	tSamp16b	<= tNxtSamp16b;

`ifdef JX2_AUD_STEREO
	tPcmBlkL	<= { pcmMemB[tPcmIdx], pcmMemA[tPcmIdx] };
	tPcmBlkR	<= { pcmMemD[tPcmIdx], pcmMemC[tPcmIdx] };
`else
	tPcmBlk		<= { pcmMemB[tPcmIdx], pcmMemA[tPcmIdx] };
`endif
	
	if(tDevCSel && busOpm[4])
	begin
		if(tDevCSelCtr)
		begin
			case(busAddr[7:2])
				6'h00: tRegCtrl0	<= busInData;

				default: begin
				end
			endcase
		end
	
		if(tDevCSelAuL)
		begin
`ifdef JX2_AUD_16K
			if(busAddr[2])
				pcmMemB[busAddr[13:3]]	<= busInData;
			else
				pcmMemA[busAddr[13:3]]	<= busInData;
`else
			if(busAddr[2])
				pcmMemB[busAddr[12:3]]	<= busInData;
			else
				pcmMemA[busAddr[12:3]]	<= busInData;
`endif
		end

`ifdef JX2_AUD_STEREO
		if(tDevCSelAuR)
		begin
`ifdef JX2_AUD_16K
			if(busAddr[2])
				pcmMemD[busAddr[13:3]]	<= busInData;
			else
				pcmMemC[busAddr[13:3]]	<= busInData;
`else
			if(busAddr[2])
				pcmMemD[busAddr[12:3]]	<= busInData;
			else
				pcmMemC[busAddr[12:3]]	<= busInData;
`endif
		end
`endif

	end
end

endmodule
