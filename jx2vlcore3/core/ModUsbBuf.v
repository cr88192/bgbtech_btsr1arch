/*
Deal with two USB ports using buffers.

IO Pins:
  0: A D+
  1: A D-
  2: B D+
  3: B D-

 */

`include "CoreDefs.v"


module ModUsbBuf(
	/* verilator lint_off UNUSED */
	clock,			reset,
	usb_clkdat_i,	usb_clkdat_o,
	usb_clkdat_d,	usb_clkref,
	usbLinkState3,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;
input			reset;

input[3:0]			usb_clkdat_i;
output[3:0]			usb_clkdat_o;
output[3:0]			usb_clkdat_d;
output[1:0]			usb_clkref;

output[1:0]			usbLinkState3;

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

reg[63:0]		tMmioOutData;
reg[31:0]		tMmioOutAddr;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[31:0]		tMmioOutAddr2;
reg[1:0]		tMmioOK2;

// assign		usb_clkdat_o = 2'b00;
// assign		usb_clkdat_d = 2'b00;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioBufCSel;
assign		tMmioBufCSel = tMmioLowCSel && (tMmioAddr[15:13]==3'b100);

wire		tMmioCtlCSel;
assign		tMmioCtlCSel = tMmioLowCSel && (tMmioAddr[15:8]==8'hE8);

reg				mmioInBufOE;
reg				mmioInBufWR;

reg				mmioInCtlOE;
reg				mmioInCtlWR;

reg[3:0]		tUsbClkdatIn;
reg[3:0]		tUsbClkdatOut;
reg[3:0]		tUsbClkdatDir;
reg[3:0]		tNxtUsbClkdatOut;
reg[3:0]		tNxtUsbClkdatDir;
reg[1:0]		tUsbClkRef;

reg[3:0]		tUsbClkdatOut2;
reg[3:0]		tUsbClkdatDir2;
reg[1:0]		tUsbClkRef2;

assign		usb_clkdat_o	= tUsbClkdatOut2;
assign		usb_clkdat_d	= tUsbClkdatDir2;
assign		usb_clkref		= tUsbClkRef2;


reg[35:0]	arrUsbRxA[255:0];
reg[35:0]	arrUsbTxA[255:0];
reg[9:0]	idxUsbRxsA;
reg[9:0]	idxUsbRxeA;
reg[9:0]	idxUsbTxsA;
reg[9:0]	idxUsbTxeA;
reg[9:0]	nxtIdxUsbRxsA;
reg[9:0]	nxtIdxUsbRxeA;
reg[9:0]	nxtIdxUsbTxsA;
reg[9:0]	nxtIdxUsbTxeA;

reg[35:0]	valUsbLdRxA;
reg[35:0]	valUsbStRxA;
reg[35:0]	valUsbLdTxA;
reg[35:0]	valUsbStTxA;

reg[35:0]	valUsbLdSel;
reg[35:0]	valUsbStSel;

reg[7:0]	idxUsbLdRxA;
reg[7:0]	idxUsbStRxA;
reg[7:0]	idxUsbLdTxA;
reg[7:0]	idxUsbStTxA;
reg			doUsbStRxA;
reg			doUsbStTxA;

reg[7:0]	idxUsbLdRxAL;
reg[7:0]	idxUsbStRxAL;

reg[7:0]	idxUsbLdRxM;
reg[7:0]	idxUsbStRxM;

reg[35:0]	arrUsbRxB[255:0];
reg[35:0]	arrUsbTxB[255:0];
reg[9:0]	idxUsbRxsB;
reg[9:0]	idxUsbRxeB;
reg[9:0]	idxUsbTxsB;
reg[9:0]	idxUsbTxeB;
reg[9:0]	nxtIdxUsbRxsB;
reg[9:0]	nxtIdxUsbRxeB;
reg[9:0]	nxtIdxUsbTxsB;
reg[9:0]	nxtIdxUsbTxeB;

reg[35:0]	valUsbLdRxB;
reg[35:0]	valUsbStRxB;
reg[35:0]	valUsbLdTxB;
reg[35:0]	valUsbStTxB;

reg[7:0]	idxUsbLdRxB;
reg[7:0]	idxUsbStRxB;
reg[7:0]	idxUsbLdTxB;
reg[7:0]	idxUsbStTxB;
reg			doUsbStRxB;
reg			doUsbStTxB;

/* Clock Strobes, TX */
reg[12:0]	tClkAcc1p5Mhz;
reg[12:0]	tClkAcc12p0Mhz;
reg[12:0]	nxtClkAcc1p5Mhz;
reg[12:0]	nxtClkAcc12p0Mhz;
reg			tClkStrobe1p5Mhz;
reg			tClkStrobe12p0Mhz;

//reg[12:0]	tClkAccRxA;
//reg[12:0]	tClkAccRxB;
//reg[12:0]	nxtClkAccRxA;
//reg[12:0]	nxtClkAccRxB;

reg[15:0]	tClkAccRxA;
reg[15:0]	tClkAccRxB;
reg[15:0]	nxtClkAccRxA;
reg[15:0]	nxtClkAccRxB;

reg[15:0]	inhClkAccRxA;
reg[15:0]	inhClkAccRxB;

reg			tClkStrobeRxA;
reg			tClkStrobeRxB;
reg			tClkStrobeRxPhA;
reg			tClkStrobeRxPhB;
reg			tClkStrobeRxPhAL;
reg			tClkStrobeRxPhBL;

reg			tClkPulseRxPhA;
reg			tClkPulseRxPhB;
reg			tClkPulseRxPhAL;
reg			tClkPulseRxPhBL;

reg			tClkStrobeInhA;
reg			tClkStrobeInhB;

reg[1:0]	tLinkTypeA;
reg[1:0]	tLinkTypeB;
reg[1:0]	tNxtLinkTypeA;
reg[1:0]	tNxtLinkTypeB;

reg[1:0]	tLinkStateA;
reg[1:0]	tLinkStateB;
reg[1:0]	tNxtLinkStateA;
reg[1:0]	tNxtLinkStateB;

assign	usbLinkState3 = { tLinkStateB == 2'b11, tLinkStateA == 2'b11 };

reg[1:0]	tLinkStateTxA;
reg[1:0]	tLinkStateTxB;
reg[1:0]	tNxtLinkStateTxA;
reg[1:0]	tNxtLinkStateTxB;

reg[11:0]	tLinkStateCntA;
reg[11:0]	tLinkStateCntB;
reg[11:0]	tNxtLinkStateCntA;
reg[11:0]	tNxtLinkStateCntB;

reg[1:0]	tBitStateA;
reg[1:0]	tBitStateB;
reg[1:0]	tNxtBitStateA;
reg[1:0]	tNxtBitStateB;

reg[11:0]	tBitShiftA;
reg[11:0]	tBitShiftB;
reg[11:0]	tNxtBitShiftA;
reg[11:0]	tNxtBitShiftB;
reg[11:0]	tBitShiftNsA;
reg[11:0]	tBitShiftNsB;
reg[11:0]	tNxtBitShiftNsA;
reg[11:0]	tNxtBitShiftNsB;

reg[7:0]	tGetByteNsA;
reg[7:0]	tGetByteNsB;

reg[11:0]	tBitShiftTxA;
reg[11:0]	tBitShiftTxB;
reg[11:0]	tNxtBitShiftTxA;
reg[11:0]	tNxtBitShiftTxB;

reg[3:0]	tPacketStateA;
reg[3:0]	tPacketStateB;
reg[3:0]	tNxtPacketStateA;
reg[3:0]	tNxtPacketStateB;

reg[3:0]	tPacketStateTxA;
reg[3:0]	tPacketStateTxB;
reg[3:0]	tNxtPacketStateTxA;
reg[3:0]	tNxtPacketStateTxB;

reg[8:0]	tPacketTxByteA;
reg[8:0]	tPacketTxByteB;
reg[8:0]	tNxtPacketTxByteA;
reg[8:0]	tNxtPacketTxByteB;
reg[8:0]	tPacketTxByte2A;
reg[8:0]	tPacketTxByte2B;
reg[8:0]	tNxtPacketTxByte2A;
reg[8:0]	tNxtPacketTxByte2B;

reg			tPacketTxBitA;
reg			tPacketTxBitB;

reg[31:0]	tLinkStatusRegA;
reg[31:0]	tLinkStatusRegB;

reg[15:0]	tIdleTimerCntA;
reg[15:0]	tIdleTimerCntB;
reg[15:0]	tNxtIdleTimerCntA;
reg[15:0]	tNxtIdleTimerCntB;

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOutAddr	= UV32_00;
	tMmioOK			= UMEM_OK_READY;

//	mmioInBufOE			= (tMmioOpm[3]) && tMmioBufCSel;
//	mmioInBufWR			= (tMmioOpm[4]) && tMmioBufCSel;

//	mmioInCtlOE			= (tMmioOpm[3]) && tMmioCtlCSel;
//	mmioInCtlWR			= (tMmioOpm[4]) && tMmioCtlCSel;

	mmioInBufOE			= (tMmioOpm[4:3]==2'b01) && tMmioBufCSel && !reset;
	mmioInBufWR			= (tMmioOpm[4:3]==2'b10) && tMmioBufCSel && !reset;

	mmioInCtlOE			= (tMmioOpm[4:3]==2'b01) && tMmioCtlCSel && !reset;
	mmioInCtlWR			= (tMmioOpm[4:3]==2'b10) && tMmioCtlCSel && !reset;

	nxtIdxUsbRxsA		= idxUsbRxsA;
	nxtIdxUsbRxeA		= idxUsbRxeA;
	nxtIdxUsbTxsA		= idxUsbTxsA;
	nxtIdxUsbTxeA		= idxUsbTxeA;

	nxtIdxUsbRxsB		= idxUsbRxsB;
	nxtIdxUsbRxeB		= idxUsbRxeB;
	nxtIdxUsbTxsB		= idxUsbTxsB;
	nxtIdxUsbTxeB		= idxUsbTxeB;

	idxUsbLdRxA			= idxUsbRxeA[9:2];
	idxUsbStRxA			= idxUsbLdRxA;
	idxUsbLdTxA			= idxUsbTxsA[9:2];
	idxUsbStTxA			= idxUsbLdTxA;

	idxUsbLdRxB			= idxUsbRxeB[9:2];
	idxUsbStRxB			= idxUsbLdRxB;
	idxUsbLdTxB			= idxUsbTxsB[9:2];
	idxUsbStTxB			= idxUsbLdTxB;

	valUsbStRxA			= valUsbLdRxA;
	valUsbStTxA			= valUsbLdTxA;
	valUsbStRxB			= valUsbLdRxB;
	valUsbStTxB			= valUsbLdTxB;
	doUsbStRxA			= 0;
	doUsbStTxA			= 0;
	doUsbStRxB			= 0;
	doUsbStTxB			= 0;

	tNxtUsbClkdatOut	= tUsbClkdatOut;
	tNxtUsbClkdatDir	= tUsbClkdatDir;

	nxtClkAcc1p5Mhz		= tClkAcc1p5Mhz + 123;
	nxtClkAcc12p0Mhz	= tClkAcc12p0Mhz + 983;
	tClkStrobe1p5Mhz	= tClkAcc1p5Mhz[12];
	tClkStrobe12p0Mhz	= tClkAcc12p0Mhz[12];

	tNxtLinkTypeA		= tLinkTypeA;
	tNxtLinkTypeB		= tLinkTypeB;
	tNxtLinkStateA		= tUsbClkdatIn[1:0];
	tNxtLinkStateB		= tUsbClkdatIn[3:2];
	tNxtLinkStateCntA	= tLinkStateCntA + 1;
	tNxtLinkStateCntB	= tLinkStateCntB + 1;

	if(tUsbClkdatDir2[0])
		tNxtLinkStateA = tLinkTypeA;
	if(tUsbClkdatDir2[2])
		tNxtLinkStateB = tLinkTypeB;

//	nxtClkAccRxA = tClkAccRxA + 123;
//	nxtClkAccRxB = tClkAccRxB + 123;
//	if(tLinkTypeA == 2'b01)
//		nxtClkAccRxA = tClkAccRxA + 983;
//	if(tLinkTypeB == 2'b01)
//		nxtClkAccRxB = tClkAccRxB + 983;

`ifdef jx2_cpu_mmioclock_50
	nxtClkAccRxA = tClkAccRxA + 1966;
	nxtClkAccRxB = tClkAccRxB + 1966;
	if(tLinkTypeA == 2'b01)
		nxtClkAccRxA = tClkAccRxA + 15728;
	if(tLinkTypeB == 2'b01)
		nxtClkAccRxB = tClkAccRxB + 15728;
`endif

//	inhClkAccRxA	= tClkAccRxA + 7864;
//	inhClkAccRxB	= tClkAccRxB + 7864;

	inhClkAccRxA	= tClkAccRxA + 3932;
	inhClkAccRxB	= tClkAccRxB + 3932;

	if(tLinkTypeA == 2'b01)
		inhClkAccRxA	= tClkAccRxA + 15728;
	if(tLinkTypeB == 2'b01)
		inhClkAccRxB	= tClkAccRxB + 15728;

//	tClkStrobeRxA		= tClkAccRxA[12];
//	tClkStrobeRxB		= tClkAccRxB[12];
//	tClkStrobeRxA		= tClkAccRxA[11];
//	tClkStrobeRxB		= tClkAccRxB[11];
	tClkStrobeRxA		= tClkAccRxA[15];
	tClkStrobeRxB		= tClkAccRxB[15];

//	tClkStrobeRxPhA		= (tClkAccRxA[12:11] == 2'b01);
//	tClkStrobeRxPhB		= (tClkAccRxB[12:11] == 2'b01);
//	tClkStrobeRxPhA		= (tClkAccRxA[12] ^ tClkAccRxA[11]);
//	tClkStrobeRxPhB		= (tClkAccRxB[12] ^ tClkAccRxB[11]);
//	tClkStrobeRxPhA		= (tClkAccRxA[11] ^ tClkAccRxA[10]);
//	tClkStrobeRxPhB		= (tClkAccRxB[11] ^ tClkAccRxB[10]);

	tClkStrobeRxPhA		= (tClkAccRxA[15] ^ tClkAccRxA[14]);
	tClkStrobeRxPhB		= (tClkAccRxB[15] ^ tClkAccRxB[14]);

	tClkStrobeInhA		= tClkAccRxA[15:14] != inhClkAccRxA[15:14];
	tClkStrobeInhB		= tClkAccRxB[15:14] != inhClkAccRxA[15:14];

	tUsbClkRef			= { tClkStrobeRxPhB, tClkStrobeRxPhA };

	tClkPulseRxPhA	= 0;
	tClkPulseRxPhB	= 0;
	if(tClkStrobeRxPhA && !tClkStrobeRxPhAL)
		tClkPulseRxPhA	= 1;
	if(tClkStrobeRxPhB && !tClkStrobeRxPhBL)
		tClkPulseRxPhB	= 1;

	if(tClkPulseRxPhA)
		tClkStrobeInhA = 1;
	if(tClkPulseRxPhB)
		tClkStrobeInhB = 1;

	if((tPacketStateA==4'h0) && (tPacketStateTxA==4'h0))
		tClkStrobeInhA = 0;
	if((tPacketStateB==4'h0) && (tPacketStateTxB==4'h0))
		tClkStrobeInhB = 0;

	if(tNxtLinkStateA != tLinkStateA)
	begin
		tNxtLinkStateCntA	= 0;
		nxtClkAccRxA		= 0;
	end

	if(tNxtLinkStateB != tLinkStateB)
	begin
		tNxtLinkStateCntB	= 0;
		nxtClkAccRxB		= 0;
	end

	if(tLinkStateCntA[11:8] == 4'b1111)
		tNxtLinkTypeA = tLinkStateA;
	if(tLinkStateCntB[11:8] == 4'b1111)
		tNxtLinkTypeB = tLinkStateB;

	tNxtBitStateA		= tBitStateA;
	tNxtBitStateB		= tBitStateB;
	tNxtBitShiftA		= tBitShiftA;
	tNxtBitShiftB		= tBitShiftB;
	tNxtBitShiftNsA		= tBitShiftNsA;
	tNxtBitShiftNsB		= tBitShiftNsB;
	tNxtBitShiftTxA		= tBitShiftTxA;
	tNxtBitShiftTxB		= tBitShiftTxB;

	tNxtPacketStateA	= tPacketStateA;
	tNxtPacketStateB	= tPacketStateB;
	tNxtPacketStateTxA	= tPacketStateTxA;
	tNxtPacketStateTxB	= tPacketStateTxB;

	tNxtLinkStateTxA	= tLinkStateTxA;
	tNxtLinkStateTxB	= tLinkStateTxB;

	tNxtPacketTxByteA	= tPacketTxByteA;
	tNxtPacketTxByteB	= tPacketTxByteB;
	tNxtPacketTxByte2A	= tPacketTxByte2A;
	tNxtPacketTxByte2B	= tPacketTxByte2B;

	tNxtIdleTimerCntA	= tIdleTimerCntA;
	tNxtIdleTimerCntB	= tIdleTimerCntB;

	tLinkStatusRegA	= {
		22'h0,
		tNxtLinkTypeA,
		tPacketStateTxA,
		tPacketStateA };

	tLinkStatusRegB	= {
		22'h0,
		tNxtLinkTypeB,
		tPacketStateTxB,
		tPacketStateB };
	
	case(idxUsbTxsA[1:0])
		2'b00: tNxtPacketTxByte2A = valUsbLdTxA[ 8: 0];
		2'b01: tNxtPacketTxByte2A = valUsbLdTxA[17: 9];
		2'b10: tNxtPacketTxByte2A = valUsbLdTxA[26:18];
		2'b11: tNxtPacketTxByte2A = valUsbLdTxA[35:27];
	endcase

	case(idxUsbTxsB[1:0])
		2'b00: tNxtPacketTxByte2B = valUsbLdTxB[ 8: 0];
		2'b01: tNxtPacketTxByte2B = valUsbLdTxB[17: 9];
		2'b10: tNxtPacketTxByte2B = valUsbLdTxB[26:18];
		2'b11: tNxtPacketTxByte2B = valUsbLdTxB[35:27];
	endcase

//	$display("ModUsbBuf: %X %X", tPacketStateA, tPacketStateTxA);

	if((tPacketStateA==4'h0) && (tPacketStateTxA==4'h0) &&
		(idxUsbTxsA!=idxUsbTxeA))
	begin
		$display("ModUsbBuf: Begin TX A");

		tNxtPacketTxByteA	= 9'h07F;
		tNxtPacketStateTxA = 4'h6;
//		tNxtPacketStateTxA = 4'h8;
	end
	if((tPacketStateB==4'h0) && (tPacketStateTxB==4'h0) &&
		(idxUsbTxsB!=idxUsbTxeB))
	begin
		$display("ModUsbBuf: Begin TX B");

		tNxtPacketTxByteB	= 9'h07F;
		tNxtPacketStateTxB = 4'h6;
//		tNxtPacketStateTxB = 4'h8;
	end

	case(tPacketStateTxA[2:0])
		3'h0: tPacketTxBitA = tNxtPacketTxByteA[0];
		3'h1: tPacketTxBitA = tNxtPacketTxByteA[1];
		3'h2: tPacketTxBitA = tNxtPacketTxByteA[2];
		3'h3: tPacketTxBitA = tNxtPacketTxByteA[3];
		3'h4: tPacketTxBitA = tNxtPacketTxByteA[4];
		3'h5: tPacketTxBitA = tNxtPacketTxByteA[5];
		3'h6: tPacketTxBitA = tNxtPacketTxByteA[6];
		3'h7: tPacketTxBitA = tNxtPacketTxByteA[7];
	endcase

	case(tPacketStateTxB[2:0])
		3'h0: tPacketTxBitB = tNxtPacketTxByteB[0];
		3'h1: tPacketTxBitB = tNxtPacketTxByteB[1];
		3'h2: tPacketTxBitB = tNxtPacketTxByteB[2];
		3'h3: tPacketTxBitB = tNxtPacketTxByteB[3];
		3'h4: tPacketTxBitB = tNxtPacketTxByteB[4];
		3'h5: tPacketTxBitB = tNxtPacketTxByteB[5];
		3'h6: tPacketTxBitB = tNxtPacketTxByteB[6];
		3'h7: tPacketTxBitB = tNxtPacketTxByteB[7];
	endcase

	if(tClkPulseRxPhA)
	begin
	
//		$display("ModUsbBuf: tClkPulseRxPhA");
	
		if(tPacketStateTxA == 4'h0)
		begin
			tNxtUsbClkdatOut[1:0]	= 2'b00;
			tNxtUsbClkdatDir[1:0]	= 2'b00;

			tNxtLinkStateTxA		= tLinkTypeA;

			if(tIdleTimerCntA != 0)
			begin
				tNxtIdleTimerCntA = tIdleTimerCntA - 1;
			end
			else
			begin
				tNxtIdleTimerCntA = 1500;
				tNxtPacketStateTxA = 4'h4;
			end
		end

		if(tPacketStateTxA == 4'h1)
		begin
			tNxtLinkStateTxA		= 2'b00;
			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;
			tNxtUsbClkdatDir[1:0]	= 2'b11;

			if(tIdleTimerCntA != 0)
			begin
				tNxtIdleTimerCntA = tIdleTimerCntA - 1;
			end
			else
			begin
				tNxtIdleTimerCntA = 1500;
				if(tPacketStateA == 4'h0)
					tNxtPacketStateTxA = 4'h4;
			end
		end
	
		if(tPacketStateTxA[3:1]==3'b010)
		begin
//			$display("ModUsbBuf: tClkPulseRxPhA A0");

			tNxtLinkStateTxA		= 2'b00;
			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;
			tNxtUsbClkdatDir[1:0]	= 2'b11;
			tNxtPacketStateTxA = tPacketStateTxA + 1;
			if(tPacketStateTxA[0])
				tNxtPacketStateTxA	= 0;

//			$display("ModUsbBuf: tClkPulseRxPhA A1 %X", tNxtLinkStateTxA);
		end

		if(tPacketStateTxA[3:1]==3'b011)
		begin
//			$display("ModUsbBuf: tClkPulseRxPhA B0");

//			tNxtLinkStateTxA		= tPacketStateTxA[0] ? 2'b10 : 2'b01;
			tNxtLinkStateTxA		= tLinkTypeA;

			tNxtUsbClkdatDir[1:0]	= 2'b11;
			tNxtPacketStateTxA		= tPacketStateTxA + 1;

			if(tPacketTxByte2A[8] && tPacketTxByte2A[0])
			begin
				$display("ModUsbBuf: Reset Command A");
				tNxtLinkStateTxA	= 0;
//				tNxtIdleTimerCntA	= 3750;
				tNxtIdleTimerCntA	= 4095;
				tNxtPacketStateTxA	= 4'h1;
				nxtIdxUsbTxsA		= idxUsbTxsA + 1;
			end

			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;

//			$display("ModUsbBuf: tClkPulseRxPhA B1 %X", tNxtLinkStateTxA);
		end

		if(tPacketStateTxA[3])
		begin
//			$display("ModUsbBuf: tClkPulseRxPhA C0");

			if(tPacketTxBitA)
				tNxtLinkStateTxA	= tLinkStateTxA[0] ? 2'b10 : 2'b01;
			if(tNxtPacketTxByteA[8])
				tNxtLinkStateTxA	= 0;
			
			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;
			tNxtUsbClkdatDir[1:0]	= 2'b11;

			tNxtBitShiftTxA = { tLinkStateTxA != tNxtLinkStateTxA,
				tBitShiftTxA[11:1] };

			tNxtPacketStateTxA[2:0] = tPacketStateTxA[2:0] + 1;

//			if(tBitShiftTxA[11:6] == 6'b000000)
//			if(tNxtBitShiftTxA[11:6] == 6'b000000)
			if(tNxtBitShiftTxA[11:5] == 7'b0000000)
			begin
				/* If we sent 6 zero bits, hold prior state and bit-stuff. */
				tNxtBitShiftTxA = { 1'b1,
					tBitShiftTxA[11:1] };
				tNxtLinkStateTxA	= tLinkStateTxA[0] ? 2'b10 : 2'b01;
				tNxtPacketStateTxA	= tPacketStateTxA;
			end
			else
			if(tPacketStateTxA[2:0] == 3'b111)
			begin
				if(!tPacketTxByte2A[8] && (idxUsbTxsA!=idxUsbTxeA))
//				if(idxUsbTxsA!=idxUsbTxeA)
				begin
					$display("ModUsbBuf: Begin Send Byte %X S/E=%X/%X",
						tPacketTxByte2A, idxUsbTxsA, idxUsbTxeA);
//					tNxtPacketTxByteA	= tPacketTxByte2A;
//					tNxtPacketTxByteA	= ~tPacketTxByte2A;
					tNxtPacketTxByteA	= 	{
						tPacketTxByte2A[8],
						~tPacketTxByte2A[7:0] };
					nxtIdxUsbTxsA	= idxUsbTxsA + 1;
					tNxtPacketStateTxA = 4'h8;
				end
				else
				begin
					$display("ModUsbBuf: Send EOP");
					if(idxUsbTxsA!=idxUsbTxeA)
						nxtIdxUsbTxsA	= idxUsbTxsA + 1;
					tNxtPacketStateTxA = 4'h4;
				end
			end

			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;

//			$display("ModUsbBuf: tClkPulseRxPhA C1 %X", tNxtLinkStateTxA);
		end
	
		tNxtBitStateA	= tLinkStateA;
		if(tLinkStateA[1]^tLinkStateA[0])
		begin
			tNxtBitShiftA = { tBitStateA != tNxtBitStateA, tBitShiftA[11:1] };
//			if(tBitShiftA[11:6] != 6'b000000)
			if(tNxtBitShiftA[11:5] != 7'b1000000)
			begin
				tNxtBitShiftNsA = {
					tBitStateA != tNxtBitStateA,
					tBitShiftNsA[11:1] };
				if(tPacketStateA[3])
					tNxtPacketStateA[2:0] = tPacketStateA[2:0] + 1;
				
				if((tNxtBitShiftA[11:5] == 7'b0000000) && tPacketStateA[3])
				begin
					$display("ModUsbBuf: Missing expected stuff bit A");
				end
			end

			if(tPacketStateA!=0)
			begin
//				$display("ModUsbBuf: See-A bit=%X win=%X st=%X",
//					tNxtBitStateA, tNxtBitShiftA, tNxtPacketStateA);
			end
			else
			begin
				if(tBitStateA != tNxtBitStateA)
				begin
//					$display("ModUsbBuf: See-B bit=%X win=%X st=%X",
//						tNxtBitStateA, tNxtBitShiftA, tNxtPacketStateA);
				end
			end

//			if((tBitShiftA[11:3]==9'b011111111) && !tPacketStateA[3])
//			if((tNxtBitShiftA[11:3]==9'b011111111) && !tPacketStateA[3])
//			if((tNxtBitShiftA[11:4]==8'b01111111) && !tPacketStateA[3])
//			if((tNxtBitShiftA[11:5]==7'b0111111) && !tPacketStateA[3])
//			if((tNxtBitShiftA[11:6]==6'b011111) && !tPacketStateA[3])
			if((tNxtBitShiftA[11:7]==5'b01111) && !tPacketStateA[3])
			begin
				$display("ModUsbBuf: Seen Sync A");
				tNxtPacketStateA = 4'h8;
			end

			if(tPacketStateA==4'hF)
			begin
//				tGetByteNsA = tBitShiftNsA[11:4];
//				tGetByteNsA = ~tBitShiftNsA[11:4];
				tGetByteNsA = ~tNxtBitShiftNsA[11:4];
				$display("USB A: Get Byte %X %X..%X blk0=%X",
					tGetByteNsA, idxUsbRxsA, idxUsbRxeA, valUsbStRxA);
				case(idxUsbRxeA[1:0])
					2'b00: valUsbStRxA[ 8: 0] = {1'b0, tGetByteNsA};
					2'b01: valUsbStRxA[17: 9] = {1'b0, tGetByteNsA};
					2'b10: valUsbStRxA[26:18] = {1'b0, tGetByteNsA};
					2'b11: valUsbStRxA[35:27] = {1'b0, tGetByteNsA};
				endcase
				$display("  blk1=%X", valUsbStRxA);
				nxtIdxUsbRxeA	= idxUsbRxeA + 1;
				doUsbStRxA		= 1;
			end
		end
		else if(tLinkStateA==2'b00)
		begin
			if(tPacketStateA[3])
			begin
				$display("ModUsbBuf: See EOP");

				case(idxUsbRxeA[1:0])
					2'b00: valUsbStRxA[ 8: 0] = 9'h100;
					2'b01: valUsbStRxA[17: 9] = 9'h100;
					2'b10: valUsbStRxA[26:18] = 9'h100;
					2'b11: valUsbStRxA[35:27] = 9'h100;
				endcase
				nxtIdxUsbRxeA	= idxUsbRxeA + 1;
				doUsbStRxA		= 1;
			end

			tNxtPacketStateA = 0;
			tNxtBitStateA = 0;
			
			tNxtBitShiftA	= 12'h555;
			tNxtBitShiftNsA	= 12'h555;
		end
	end

	if(tClkPulseRxPhB)
	begin
		if(tPacketStateTxB == 4'h0)
		begin
			tNxtUsbClkdatOut[3:2]	= 2'b00;
			tNxtUsbClkdatDir[3:2]	= 2'b00;

			tNxtLinkStateTxB		= tLinkTypeB;

			if(tIdleTimerCntB != 0)
			begin
				tNxtIdleTimerCntB = tIdleTimerCntB - 1;
			end
			else
			begin
				tNxtIdleTimerCntB = 1500;
				if(tPacketStateB == 4'h0)
					tNxtPacketStateTxB = 4'h4;
			end
		end

		if(tPacketStateTxB == 4'h1)
		begin
			tNxtLinkStateTxB		= 2'b00;
			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
			tNxtUsbClkdatDir[3:2]	= 2'b11;

			if(tIdleTimerCntB != 0)
			begin
				tNxtIdleTimerCntB = tIdleTimerCntB - 1;
			end
			else
			begin
				tNxtIdleTimerCntB = 1500;
				tNxtPacketStateTxB = 4'h4;
			end
		end

		if(tPacketStateTxB[3:1]==3'b010)
		begin
			tNxtUsbClkdatOut[3:2]	= 2'b00;
			tNxtUsbClkdatDir[3:2]	= 2'b11;
			tNxtPacketStateTxB = tPacketStateTxB + 1;
			if(tPacketStateTxB[0])
				tNxtPacketStateTxB	= 0;
		end

		if(tPacketStateTxB[3:1]==3'b011)
		begin
//			tNxtLinkStateTxB		= tPacketStateTxB[0] ? 2'b10 : 2'b01;
			tNxtLinkStateTxB		= tLinkTypeB;

			tNxtUsbClkdatDir[3:2]	= 2'b11;
			tNxtPacketStateTxB		= tPacketStateTxB + 1;

			if(tPacketTxByte2B[8] && tPacketTxByte2B[0])
			begin
				$display("ModUsbBuf: Reset Command B");
				tNxtLinkStateTxB	= 0;
//				tNxtIdleTimerCntB	= 3750;
				tNxtIdleTimerCntB	= 4095;
				tNxtPacketStateTxB	= 4'h1;
				nxtIdxUsbTxsB		= idxUsbTxsB + 1;
			end

			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
		end

		if(tPacketStateTxB[3])
		begin
			if(tPacketTxBitB)
				tNxtLinkStateTxB	= tLinkStateTxB[0] ? 2'b10 : 2'b01;
			if(tNxtPacketTxByteB[8])
				tNxtLinkStateTxB	= 0;

			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
			tNxtUsbClkdatDir[3:2]	= 2'b11;

			tNxtBitShiftTxB = { tLinkStateTxB != tNxtLinkStateTxB,
				tBitShiftTxB[11:1] };

			tNxtPacketStateTxB[2:0] = tPacketStateTxB[2:0] + 1;

//			if(tBitShiftTxB[11:6] == 6'b000000)
//			if(tNxtBitShiftTxB[11:6] == 6'b000000)
			if(tNxtBitShiftTxB[11:5] == 7'b0000000)
			begin
				/* If we sent 6 zero bits, hold prior state and bit-stuff. */
				tNxtBitShiftTxB = { 1'b1,
					tBitShiftTxB[11:1] };
				tNxtLinkStateTxB	= tLinkStateTxB[0] ? 2'b10 : 2'b01;
				tNxtPacketStateTxB	= tPacketStateTxB;
			end
			else
			if(tPacketStateTxB[2:0] == 3'b111)
			begin
				if(!tPacketTxByte2B[8] && (idxUsbTxsB!=idxUsbTxeB))
//				if(idxUsbTxsB!=idxUsbTxeB)
				begin
//					tNxtPacketTxByteB	= tPacketTxByte2B;
//					tNxtPacketTxByteB	= ~tPacketTxByte2B;
					tNxtPacketTxByteB	= 	{
						tPacketTxByte2B[8],
						~tPacketTxByte2B[7:0] };
					nxtIdxUsbTxsB		= idxUsbTxsB + 1;
					tNxtPacketStateTxB	= 4'h8;
				end
				else
				begin
					if(idxUsbTxsB!=idxUsbTxeB)
						nxtIdxUsbTxsB	= idxUsbTxsB + 1;
					tNxtPacketStateTxB	= 4'h4;
				end
			end

			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
		end

		tNxtBitStateB	= tLinkStateB;
		if(tLinkStateB[1]^tLinkStateB[0])
		begin
			tNxtBitShiftB = { tBitStateB != tNxtBitStateB, tBitShiftB[11:1] };
//			if(tBitShiftB[11:6] != 6'b000000)
			if(tNxtBitShiftB[11:5] != 7'b1000000)
			begin
				tNxtBitShiftNsB = {
					tBitStateB != tNxtBitStateB,
					tBitShiftNsB[11:1] };
				if(tPacketStateB[3])
					tNxtPacketStateB[2:0] = tPacketStateB[2:0] + 1;
				
				if((tNxtBitShiftB[11:5] == 7'b0000000) && tPacketStateB[3])
				begin
					$display("ModUsbBuf: Missing expected stuff bit B");
				end
			end

//			if((tBitShiftB[11:3]==9'b011111111) && !tPacketStateB[3])
//			if((tNxtBitShiftB[11:3]==9'b011111111) && !tPacketStateB[3])
//			if((tNxtBitShiftB[11:4]==8'b01111111) && !tPacketStateB[3])
//			if((tNxtBitShiftB[11:5]==7'b0111111) && !tPacketStateB[3])
//			if((tNxtBitShiftB[11:6]==6'b011111) && !tPacketStateB[3])
			if((tNxtBitShiftB[11:7]==5'b01111) && !tPacketStateB[3])
			begin
				$display("ModUsbBuf: Seen Sync B");
				tNxtPacketStateB = 4'h8;
			end


			if(tPacketStateB==4'hF)
			begin
//				tGetByteNsB = tBitShiftNsB[11:4];
//				tGetByteNsB = ~tBitShiftNsB[11:4];
				tGetByteNsB = ~tNxtBitShiftNsB[11:4];
				$display("USB B: Get Byte %X  %X..%X",
					tGetByteNsB, idxUsbRxsB, idxUsbRxeB);
				case(idxUsbRxeB[1:0])
					2'b00: valUsbStRxB[ 8: 0] = {1'b0, tGetByteNsB};
					2'b01: valUsbStRxB[17: 9] = {1'b0, tGetByteNsB};
					2'b10: valUsbStRxB[26:18] = {1'b0, tGetByteNsB};
					2'b11: valUsbStRxB[35:27] = {1'b0, tGetByteNsB};
				endcase
				nxtIdxUsbRxeB	= idxUsbRxeB + 1;
				doUsbStRxB		= 1;
			end
			
		end
		else if(tLinkStateB==2'b00)
		begin
			if(tPacketStateB[3])
			begin
				case(idxUsbRxeB[1:0])
					2'b00: valUsbStRxB[ 8: 0] = 9'h100;
					2'b01: valUsbStRxB[17: 9] = 9'h100;
					2'b10: valUsbStRxB[26:18] = 9'h100;
					2'b11: valUsbStRxB[35:27] = 9'h100;
				endcase
				nxtIdxUsbRxeB	= idxUsbRxeB + 1;
				doUsbStRxB		= 1;
			end

			tNxtPacketStateB = 0;
			tNxtBitStateB = 0;
			
			tNxtBitShiftB	= 12'h555;
			tNxtBitShiftNsB	= 12'h555;
		end
	end

	case(tMmioAddr[12:11])
		2'b00: valUsbLdSel	= valUsbLdRxA;
		2'b01: valUsbLdSel	= valUsbLdTxA;
		2'b10: valUsbLdSel	= valUsbLdRxB;
		2'b11: valUsbLdSel	= valUsbLdTxB;
	endcase
	
	valUsbStSel = {
		tMmioInData[56:48],
		tMmioInData[40:32],
		tMmioInData[24:16],
		tMmioInData[ 8: 0]
		};
	
	if(mmioInBufOE)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
//		if((tMmioOK2 == UMEM_OK_OK) && (tMmioOutAddr2==tMmioAddr))
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			idxUsbLdRxM = tMmioAddr[10:3];
		
			if(!tClkStrobeInhA && !tClkStrobeInhB)
			begin
				idxUsbLdRxA = idxUsbLdRxM;
				idxUsbLdTxA	= idxUsbLdRxM;
				idxUsbLdRxB = idxUsbLdRxM;
				idxUsbLdTxB	= idxUsbLdRxM;
			end

			tMmioOutData = {
				{ 7'b0, valUsbLdSel[35:27] },
				{ 7'b0, valUsbLdSel[26:18] },
				{ 7'b0, valUsbLdSel[17: 9] },
				{ 7'b0, valUsbLdSel[ 8: 0] }
			};
			
			tMmioOK = UMEM_OK_HOLD;

			if(idxUsbLdRxAL	== idxUsbLdRxM)
			begin
				$display("ModUsbBuf: Read Data A=%X V=%X",
					tMmioAddr, tMmioOutData);
				tMmioOK = UMEM_OK_OK;
				tMmioOutAddr = tMmioAddr;
			end
		end
	end

	if(mmioInBufWR)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
//		if((tMmioOK2 == UMEM_OK_OK) && (tMmioOutAddr2==tMmioAddr))
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			idxUsbStRxM = tMmioAddr[10:3];

			if(!tClkStrobeInhA && !tClkStrobeInhB)
			begin
				idxUsbStRxA = idxUsbStRxM;
				idxUsbStTxA	= idxUsbStRxM;
				idxUsbStRxB = idxUsbStRxM;
				idxUsbStTxB	= idxUsbStRxM;

				valUsbStRxA	= valUsbStSel;
				valUsbStTxA	= valUsbStSel;
				valUsbStRxB	= valUsbStSel;
				valUsbStTxB	= valUsbStSel;

				case(tMmioAddr[12:11])
					2'b00: doUsbStRxA	= 1;
					2'b01: doUsbStTxA	= 1;
					2'b10: doUsbStRxB	= 1;
					2'b11: doUsbStTxB	= 1;
				endcase
			end

			tMmioOK = UMEM_OK_HOLD;

			if(idxUsbStRxAL	== idxUsbStRxM)
			begin
				tMmioOK = UMEM_OK_OK;
				tMmioOutAddr = tMmioAddr;
			end
		end
	end

	if(mmioInCtlOE)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
//		if((tMmioOK2 == UMEM_OK_OK) && (tMmioOutAddr2==tMmioAddr))
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			if(tMmioOpm[2:0] == 3'b011)
			begin
				case(tMmioAddr[7:2])
					6'h00:
						tMmioOutData = {
							22'h00, idxUsbRxeA,
							22'h00, idxUsbRxsA };
					6'h02:
						tMmioOutData = { 32'h00, tLinkStatusRegA };
					6'h04:
						tMmioOutData = {
							22'h00, idxUsbTxeA,
							22'h00, idxUsbTxsA };
					6'h06:
						tMmioOutData = { 32'h00, tLinkStatusRegA };
					6'h08:
						tMmioOutData = {
							22'h00, idxUsbRxeB,
							22'h00, idxUsbRxsB };
					6'h0A:
						tMmioOutData = { 32'h00, tLinkStatusRegB };
					6'h0C:
						tMmioOutData = {
							22'h00, idxUsbTxeB,
							22'h00, idxUsbTxsB };
					6'h0E:
						tMmioOutData = { 32'h00, tLinkStatusRegB };
					default: begin	end
				endcase
//				$display("ModUsbBuf: Read 64b %X %X", tMmioAddr, tMmioOutData);
			end
			else
			begin
				case(tMmioAddr[7:2])
					6'h00: tMmioOutData = { 54'h00, idxUsbRxsA };
					6'h01: tMmioOutData = { 54'h00, idxUsbRxeA };
					6'h02: tMmioOutData = { 32'h00, tLinkStatusRegA };
					6'h03: tMmioOutData = 0;

					6'h04: tMmioOutData = { 54'h00, idxUsbTxsA };
					6'h05: tMmioOutData = { 54'h00, idxUsbTxeA };
					6'h06: tMmioOutData = { 32'h00, tLinkStatusRegA };
					6'h07: tMmioOutData = 0;

					6'h08: tMmioOutData = { 54'h00, idxUsbRxsB };
					6'h09: tMmioOutData = { 54'h00, idxUsbRxeB };
					6'h0A: tMmioOutData = { 32'h00, tLinkStatusRegB };
					6'h0B: tMmioOutData = 0;

					6'h0C: tMmioOutData = { 54'h00, idxUsbTxsB };
					6'h0D: tMmioOutData = { 54'h00, idxUsbTxeB };
					6'h0E: tMmioOutData = { 32'h00, tLinkStatusRegB };
					6'h0F: tMmioOutData = 0;
					default: begin	end
				endcase
//				$display("ModUsbBuf: Read 32b %X %X", tMmioAddr, tMmioOutData);
			end
			tMmioOK = UMEM_OK_OK;
			tMmioOutAddr = tMmioAddr;
		end
	end

	if(mmioInCtlWR)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
//		if((tMmioOK2 == UMEM_OK_OK) && (tMmioOutAddr2==tMmioAddr))
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			if(tMmioOpm[2:0] == 3'b011)
			begin
				case(tMmioAddr[7:2])
					6'h00:	begin
						nxtIdxUsbRxsA = tMmioInData[ 9: 0];
						nxtIdxUsbRxeA = tMmioInData[41:32];
//						$display("ModUsbBuf: Update RX-A %X..%X",
//							nxtIdxUsbRxsA, nxtIdxUsbRxeA);
					end
					6'h04:	begin
						nxtIdxUsbTxsA = tMmioInData[ 9: 0];
						nxtIdxUsbTxeA = tMmioInData[41:32];
//						$display("ModUsbBuf: Update TX-A %X..%X",
//							nxtIdxUsbTxsA, nxtIdxUsbTxeA);
					end
					6'h08:	begin
						nxtIdxUsbRxsB = tMmioInData[ 9: 0];
						nxtIdxUsbRxeB = tMmioInData[41:32];
//						$display("ModUsbBuf: Update RX-B %X..%X",
//							nxtIdxUsbRxsB, nxtIdxUsbRxeB);
					end
					6'h0C:	begin
						nxtIdxUsbTxsB = tMmioInData[ 9: 0];
						nxtIdxUsbTxeB = tMmioInData[41:32];
//						$display("ModUsbBuf: Update TX-B %X..%X",
//							nxtIdxUsbTxsB, nxtIdxUsbTxeB);
					end
					default: begin	end
				endcase
			end
			else
			begin
//				$display("ModUsbBuf: Update 32b %X %X", tMmioAddr, tMmioInData);
				case(tMmioAddr[7:2])
					6'h00: nxtIdxUsbRxsA = tMmioInData[9:0];
					6'h01: nxtIdxUsbRxeA = tMmioInData[9:0];
					6'h04: nxtIdxUsbTxsA = tMmioInData[9:0];
					6'h05: nxtIdxUsbTxeA = tMmioInData[9:0];
					6'h08: nxtIdxUsbRxsB = tMmioInData[9:0];
					6'h09: nxtIdxUsbRxeB = tMmioInData[9:0];
					6'h0C: nxtIdxUsbTxsB = tMmioInData[9:0];
					6'h0D: nxtIdxUsbTxeB = tMmioInData[9:0];
					default: begin	end
				endcase
			end
			tMmioOK = UMEM_OK_OK;
			tMmioOutAddr = tMmioAddr;
		end
	end

	if(reset)
	begin
		tNxtPacketStateA	= 0;
		tNxtPacketStateB	= 0;
		tNxtLinkStateTxA	= 0;
		tNxtLinkStateTxB	= 0;

		nxtIdxUsbRxsA		= 0;
		nxtIdxUsbRxeA		= 0;
		nxtIdxUsbTxsA		= 0;
		nxtIdxUsbTxeA		= 0;

		nxtIdxUsbRxsB		= 0;
		nxtIdxUsbRxeB		= 0;
		nxtIdxUsbTxsB		= 0;
		nxtIdxUsbTxeB		= 0;

		tNxtUsbClkdatOut	= 0;
		tNxtUsbClkdatDir	= 0;

		tNxtIdleTimerCntA	= 4095;
		tNxtPacketStateTxA	= 4'h1;

		tNxtIdleTimerCntB	= 4095;
		tNxtPacketStateTxB	= 4'h1;

		tMmioOK				= UMEM_OK_READY;
	end
end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOutAddr2	<= tMmioOutAddr;
	tMmioOK2		<= tMmioOK;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;

	idxUsbRxsA		<= nxtIdxUsbRxsA;
	idxUsbRxeA		<= nxtIdxUsbRxeA;
	idxUsbTxsA		<= nxtIdxUsbTxsA;
	idxUsbTxeA		<= nxtIdxUsbTxeA;

	idxUsbRxsB		<= nxtIdxUsbRxsB;
	idxUsbRxeB		<= nxtIdxUsbRxeB;
	idxUsbTxsB		<= nxtIdxUsbTxsB;
	idxUsbTxeB		<= nxtIdxUsbTxeB;

	valUsbLdRxA		<= arrUsbRxA[idxUsbLdRxA];
	valUsbLdTxA		<= arrUsbTxA[idxUsbLdTxA];
	valUsbLdRxB		<= arrUsbRxB[idxUsbLdRxB];
	valUsbLdTxB		<= arrUsbTxB[idxUsbLdTxB];
	idxUsbLdRxAL	<= idxUsbLdRxA;
	idxUsbStRxAL	<= idxUsbStRxA;

	if(doUsbStRxA)
	begin
		arrUsbRxA[idxUsbStRxA]	<= valUsbStRxA;
	end
	if(doUsbStTxA)
	begin
		arrUsbTxA[idxUsbStTxA]	<= valUsbStTxA;
	end

	if(doUsbStRxB)
	begin
		arrUsbRxB[idxUsbStRxB]	<= valUsbStRxB;
	end
	if(doUsbStTxB)
	begin
		arrUsbTxB[idxUsbStTxB]	<= valUsbStTxB;
	end

	tClkAccRxA			<= nxtClkAccRxA;
	tClkAccRxB			<= nxtClkAccRxB;

	tClkAcc1p5Mhz		<= nxtClkAcc1p5Mhz;
	tClkAcc12p0Mhz		<= nxtClkAcc12p0Mhz;
	tLinkTypeA			<= tNxtLinkTypeA;
	tLinkTypeB			<= tNxtLinkTypeB;
	tLinkStateA			<= tNxtLinkStateA;
	tLinkStateB			<= tNxtLinkStateB;
	tLinkStateCntA		<= tNxtLinkStateCntA;
	tLinkStateCntB		<= tNxtLinkStateCntB;

	tLinkStateTxA		<= tNxtLinkStateTxA;
	tLinkStateTxB		<= tNxtLinkStateTxB;

	tUsbClkdatIn		<= usb_clkdat_i;
	tUsbClkdatOut		<= tNxtUsbClkdatOut;
	tUsbClkdatDir		<= tNxtUsbClkdatDir;
	tUsbClkdatOut2		<= tUsbClkdatOut;
	tUsbClkdatDir2		<= tUsbClkdatDir;
	tUsbClkRef2			<= tUsbClkRef;

	tClkStrobeRxPhAL	<= tClkStrobeRxPhA;
	tClkStrobeRxPhBL	<= tClkStrobeRxPhB;
	tClkPulseRxPhAL		<= tClkPulseRxPhA;
	tClkPulseRxPhBL		<= tClkPulseRxPhB;

	tBitStateA			<= tNxtBitStateA;
	tBitStateB			<= tNxtBitStateB;
	tBitShiftA			<= tNxtBitShiftA;
	tBitShiftB			<= tNxtBitShiftB;
	tBitShiftNsA		<= tNxtBitShiftNsA;
	tBitShiftNsB		<= tNxtBitShiftNsB;
	tBitShiftTxA		<= tNxtBitShiftTxA;
	tBitShiftTxB		<= tNxtBitShiftTxB;
	tPacketStateA		<= tNxtPacketStateA;
	tPacketStateB		<= tNxtPacketStateB;

	tPacketStateTxA		<= tNxtPacketStateTxA;
	tPacketStateTxB		<= tNxtPacketStateTxB;
	tPacketTxByteA		<= tNxtPacketTxByteA;
	tPacketTxByteB		<= tNxtPacketTxByteB;
	tPacketTxByte2A		<= tNxtPacketTxByte2A;
	tPacketTxByte2B		<= tNxtPacketTxByte2B;

	tIdleTimerCntA		<= tNxtIdleTimerCntA;
	tIdleTimerCntB		<= tNxtIdleTimerCntB;

end

endmodule
