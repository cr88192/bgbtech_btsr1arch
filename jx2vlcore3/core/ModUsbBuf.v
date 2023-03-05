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
	usb_clkdat_i,	usb_clkdat_o,	usb_clkdat_d,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;
input			reset;

input[3:0]			usb_clkdat_i;
output[3:0]			usb_clkdat_o;
output[3:0]			usb_clkdat_d;


input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
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

reg[3:0]		tUsbClkdatOut2;
reg[3:0]		tUsbClkdatDir2;

assign		usb_clkdat_o = tUsbClkdatOut2;
assign		usb_clkdat_d = tUsbClkdatDir2;


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

reg[12:0]	tClkAccRxA;
reg[12:0]	tClkAccRxB;
reg[12:0]	nxtClkAccRxA;
reg[12:0]	nxtClkAccRxB;
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

reg[1:0]	tLinkTypeA;
reg[1:0]	tLinkTypeB;
reg[1:0]	tNxtLinkTypeA;
reg[1:0]	tNxtLinkTypeB;

reg[1:0]	tLinkStateA;
reg[1:0]	tLinkStateB;
reg[1:0]	tNxtLinkStateA;
reg[1:0]	tNxtLinkStateB;

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

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOK			= UMEM_OK_READY;

	mmioInBufOE			= (tMmioOpm[3]) && tMmioBufCSel;
	mmioInBufWR			= (tMmioOpm[4]) && tMmioBufCSel;

	mmioInCtlOE			= (tMmioOpm[3]) && tMmioCtlCSel;
	mmioInCtlWR			= (tMmioOpm[4]) && tMmioCtlCSel;

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

	nxtClkAccRxA = tClkAccRxA + 123;
	nxtClkAccRxB = tClkAccRxB + 123;
	if(tLinkTypeA == 2'b01)
		nxtClkAccRxA = tClkAccRxA + 983;
	if(tLinkTypeB == 2'b01)
		nxtClkAccRxB = tClkAccRxB + 983;

	tClkStrobeRxA		= tClkAccRxA[12];
	tClkStrobeRxB		= tClkAccRxB[12];
//	tClkStrobeRxPhA		= (tClkAccRxA[12:11] == 2'b01);
//	tClkStrobeRxPhB		= (tClkAccRxB[12:11] == 2'b01);
	tClkStrobeRxPhA		= (tClkAccRxA[12] ^ tClkAccRxA[11]);
	tClkStrobeRxPhB		= (tClkAccRxB[12] ^ tClkAccRxB[11]);

	tClkPulseRxPhA	= 0;
	tClkPulseRxPhB	= 0;
	if(tClkStrobeRxPhA && !tClkStrobeRxPhAL)
		tClkPulseRxPhA	= 1;
	if(tClkStrobeRxPhB && !tClkStrobeRxPhBL)
		tClkPulseRxPhB	= 1;

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
	
	case(idxUsbRxeA[1:0])
		2'b00: tNxtPacketTxByte2A = valUsbLdTxA[ 8: 0];
		2'b01: tNxtPacketTxByte2A = valUsbLdTxA[17: 9];
		2'b10: tNxtPacketTxByte2A = valUsbLdTxA[26:18];
		2'b11: tNxtPacketTxByte2A = valUsbLdTxA[35:27];
	endcase

	case(idxUsbRxeB[1:0])
		2'b00: tNxtPacketTxByte2B = valUsbLdTxB[ 8: 0];
		2'b01: tNxtPacketTxByte2B = valUsbLdTxB[17: 9];
		2'b10: tNxtPacketTxByte2B = valUsbLdTxB[26:18];
		2'b11: tNxtPacketTxByte2B = valUsbLdTxB[35:27];
	endcase

	if((tPacketStateA==4'h0) && (tPacketStateTxA==4'h0) &&
		(idxUsbTxsA!=idxUsbTxeA))
	begin
		tNxtPacketTxByteA	= 9'h07F;
		tNxtPacketStateTxA = 4'h6;
	end
	if((tPacketStateB==4'h0) && (tPacketStateTxB==4'h0) &&
		(idxUsbTxsB!=idxUsbTxeB))
	begin
		tNxtPacketTxByteB	= 9'h07F;
		tNxtPacketStateTxB = 4'h6;
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
		if(tPacketStateTxA == 4'h0)
		begin
			tNxtUsbClkdatOut[1:0]	= 2'b00;
			tNxtUsbClkdatDir[1:0]	= 2'b00;
		end
	
		if(tPacketStateTxA[3:1]==3'b010)
		begin
			tNxtUsbClkdatOut[1:0]	= 2'b00;
			tNxtUsbClkdatDir[1:0]	= 2'b11;
			tNxtPacketStateTxA = tPacketStateTxA + 1;
			if(tPacketStateTxA[0])
				tNxtPacketStateTxA	= 0;
		end

		if(tPacketStateTxA[3:1]==3'b011)
		begin
			tNxtLinkStateTxA		= tPacketStateTxA[0] ? 2'b10 : 2'b01;
			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;
			tNxtUsbClkdatDir[1:0]	= 2'b11;
			tNxtPacketStateTxA		= tPacketStateTxA + 1;
		end

		if(tPacketStateTxA[3])
		begin
			if(tPacketTxBitA)
				tNxtLinkStateTxA	= tLinkStateTxA[0] ? 2'b10 : 2'b01;
			tNxtUsbClkdatOut[1:0]	= tNxtLinkStateTxA;
			tNxtUsbClkdatDir[1:0]	= 2'b11;

			tNxtBitShiftTxA = { tLinkStateTxA != tNxtLinkStateTxA,
				tBitShiftTxA[11:1] };

			tNxtPacketStateTxA[2:0] = tPacketStateTxA[2:0] + 1;

			if(tBitShiftTxA[11:6] == 6'b000000)
			begin
				/* If we sent 6 zero bits, hold prior state and bit-stuff. */
				tNxtLinkStateTxA	= tLinkStateTxA[0] ? 2'b10 : 2'b01;
				tNxtPacketStateTxA	= tPacketStateTxA;
			end
			else
			if(tPacketStateTxA[2:0] == 3'b111)
			begin
				if(!tPacketTxByte2A[8] && (idxUsbTxsA!=idxUsbTxeA))
				begin
					tNxtPacketTxByteA	= tPacketTxByte2A;
					nxtIdxUsbTxsA	= idxUsbTxsA + 1;
					tNxtPacketStateTxA = 4'h8;
				end
				else
				begin
					tNxtPacketStateTxA = 4'h4;
				end
			end
		end
	
		tNxtBitStateA	= tLinkStateA;
		if(tLinkStateA[1]^tLinkStateA[0])
		begin
			tNxtBitShiftA = { tBitStateA != tNxtBitStateA, tBitShiftA[11:1] };
			if(tBitShiftA[11:6] != 6'b000000)
				tNxtBitShiftNsA = {
					tBitStateA != tNxtBitStateA,
					tBitShiftNsA[11:1] };
			if(tPacketStateA[3])
				tNxtPacketStateA[2:0] = tPacketStateA[2:0] + 1;
			if((tBitShiftA[11:3]==9'b011111111) && !tPacketStateA[3])
				tNxtPacketStateA = 4'h8;

			if(tPacketStateA==4'hF)
			begin
				$display("USB A: Get Byte %X", tBitShiftNsA[11:4]);
				case(idxUsbRxeA[1:0])
					2'b00: valUsbStRxA[ 8: 0] = {1'b0, tBitShiftNsA[11:4]};
					2'b01: valUsbStRxA[17: 9] = {1'b0, tBitShiftNsA[11:4]};
					2'b10: valUsbStRxA[26:18] = {1'b0, tBitShiftNsA[11:4]};
					2'b11: valUsbStRxA[35:27] = {1'b0, tBitShiftNsA[11:4]};
				endcase
				nxtIdxUsbRxeA	= idxUsbRxeA + 1;
				doUsbStRxA		= 1;
			end
		end
		else if(tLinkStateA==2'b00)
		begin
			tNxtPacketStateA = 0;
			tNxtBitStateA = 0;
		end
	end

	if(tClkPulseRxPhB)
	begin
		if(tPacketStateTxB == 4'h0)
		begin
			tNxtUsbClkdatOut[3:2]	= 2'b00;
			tNxtUsbClkdatDir[3:2]	= 2'b00;
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
			tNxtLinkStateTxB		= tPacketStateTxB[0] ? 2'b10 : 2'b01;
			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
			tNxtUsbClkdatDir[3:2]	= 2'b11;
			tNxtPacketStateTxB		= tPacketStateTxB + 1;
		end

		if(tPacketStateTxB[3])
		begin
			if(tPacketTxBitB)
				tNxtLinkStateTxB	= tLinkStateTxB[0] ? 2'b10 : 2'b01;
			tNxtUsbClkdatOut[3:2]	= tNxtLinkStateTxB;
			tNxtUsbClkdatDir[3:2]	= 2'b11;

			tNxtBitShiftTxB = { tLinkStateTxB != tNxtLinkStateTxB,
				tBitShiftTxB[11:1] };

			tNxtPacketStateTxB[2:0] = tPacketStateTxB[2:0] + 1;

			if(tBitShiftTxB[11:6] == 6'b000000)
			begin
				/* If we sent 6 zero bits, hold prior state and bit-stuff. */
				tNxtLinkStateTxB	= tLinkStateTxB[0] ? 2'b10 : 2'b01;
				tNxtPacketStateTxB	= tPacketStateTxB;
			end
			else
			if(tPacketStateTxB[2:0] == 3'b111)
			begin
				if(!tPacketTxByte2B[8] && (idxUsbTxsB!=idxUsbTxeB))
				begin
					tNxtPacketTxByteB	= tPacketTxByte2B;
					nxtIdxUsbTxsB		= idxUsbTxsB + 1;
					tNxtPacketStateTxB	= 4'h8;
				end
				else
				begin
					tNxtPacketStateTxB	= 4'h4;
				end
			end
		end

		tNxtBitStateB	= tLinkStateB;
		if(tLinkStateB[1]^tLinkStateB[0])
		begin
			tNxtBitShiftB = { tBitStateB != tNxtBitStateB, tBitShiftB[11:1] };
			if(tBitShiftB[11:6] != 6'b000000)
				tNxtBitShiftNsB = {
					tBitStateA != tNxtBitStateA,
					tBitShiftNsB[11:1] };
			if(tPacketStateB[3])
				tNxtPacketStateB[2:0] = tPacketStateB[2:0] + 1;
			if((tBitShiftB[11:3]==9'b011111111) && !tPacketStateB[3])
				tNxtPacketStateB = 4'h8;

			if(tPacketStateB==4'hF)
			begin
				$display("USB B: Get Byte %X", tBitShiftNsB[11:4]);
				case(idxUsbRxeB[1:0])
					2'b00: valUsbStRxB[ 8: 0] = {1'b0, tBitShiftNsB[11:4]};
					2'b01: valUsbStRxB[17: 9] = {1'b0, tBitShiftNsB[11:4]};
					2'b10: valUsbStRxB[26:18] = {1'b0, tBitShiftNsB[11:4]};
					2'b11: valUsbStRxB[35:27] = {1'b0, tBitShiftNsB[11:4]};
				endcase
				nxtIdxUsbRxeB	= idxUsbRxeB + 1;
				doUsbStRxB		= 1;
			end
			
		end
		else if(tLinkStateB==2'b00)
		begin
			tNxtPacketStateB = 0;
			tNxtBitStateB = 0;
		end
	end

	case(tMmioAddr[12:11])
		2'b00: valUsbLdSel	= valUsbLdRxA;
		2'b01: valUsbLdSel	= valUsbLdTxA;
		2'b10: valUsbLdSel	= valUsbLdRxB;
		2'b11: valUsbLdSel	= valUsbLdTxB;
	endcase
	
	if(mmioInBufOE)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			idxUsbLdRxA = tMmioAddr[10:3];
			idxUsbLdTxA	= idxUsbLdRxA;
			idxUsbLdRxB = tMmioAddr[10:3];
			idxUsbLdTxB	= idxUsbLdRxB;

			tMmioOutData = {
				{ 7'b0, valUsbLdSel[35:27] },
				{ 7'b0, valUsbLdSel[26:18] },
				{ 7'b0, valUsbLdSel[17: 9] },
				{ 7'b0, valUsbLdSel[ 8: 0] }
			};
			
			if(idxUsbLdRxAL	== idxUsbLdRxA)
			begin
				tMmioOK = UMEM_OK_OK;
			end
		end
	end

	if(mmioInBufWR)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			idxUsbStRxA = tMmioAddr[10:3];
			idxUsbStTxA	= idxUsbStRxA;
			idxUsbStRxB = tMmioAddr[10:3];
			idxUsbStTxB	= idxUsbStRxB;

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

			if(idxUsbStRxAL	== idxUsbStRxA)
			begin
				tMmioOK = UMEM_OK_OK;
			end
		end
	end

	if(mmioInCtlOE)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
			case(tMmioAddr[7:2])
				6'h00: tMmioOutData = { 54'h00, idxUsbRxsA };
				6'h01: tMmioOutData = { 54'h00, idxUsbRxeA };
				6'h04: tMmioOutData = { 54'h00, idxUsbTxsA };
				6'h05: tMmioOutData = { 54'h00, idxUsbTxeA };
				6'h08: tMmioOutData = { 54'h00, idxUsbRxsB };
				6'h09: tMmioOutData = { 54'h00, idxUsbRxeB };
				6'h0C: tMmioOutData = { 54'h00, idxUsbTxsB };
				6'h0D: tMmioOutData = { 54'h00, idxUsbTxeB };
				default: begin	end
			endcase
			tMmioOK = UMEM_OK_OK;
		end
	end

	if(mmioInCtlWR)
	begin
		if(tMmioOK2 == UMEM_OK_OK)
		begin
			tMmioOutData = tMmioOutData2;
			tMmioOK = UMEM_OK_OK;
		end
		else
		begin
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
			tMmioOK = UMEM_OK_OK;
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
	end
end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
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

end

endmodule
