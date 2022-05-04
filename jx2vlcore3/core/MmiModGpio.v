/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*

Fixed Pins
	1(I): UART RX
	0(O): UART TX

 */

`include "CoreDefs.v"

module MmiModGpio(
	/* verilator lint_off UNUSED */
	clock,		reset,

	gpioPinsOut,	gpioPinsIn,		gpioPinsDir,
	fixedPinsOut,	fixedPinsIn,
	outTimer1MHz,	outTimer100MHz,
	outBounceIrq,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;
input			reset;

output[31:0]	gpioPinsOut;
input[31:0]		gpioPinsIn;
output[31:0]	gpioPinsDir;

output[15:0]	fixedPinsOut;
input[15:0]		fixedPinsIn;

output[63:0]	outTimer1MHz;
output[63:0]	outTimer100MHz;
output[63:0]	outBounceIrq;

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire[15:0]		tFixedPinsOut;
reg[15:0]		tFixedPinsOutB;
assign		fixedPinsOut = tFixedPinsOutB;

reg[15:0]		tFixedPinsIn;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

reg[31:0]		gpioOut;
reg[31:0]		gpioDir;

reg[31:0]		gpioNextOut;
reg[31:0]		gpioNextDir;

assign			gpioPinsOut = gpioOut;
assign			gpioPinsDir = gpioDir;

reg		uartTx;
reg		uartRx;

assign	tFixedPinsOut = {
	12'hzzz,
	2'bz,
	1'bz,		//UART RX
	uartTx		//UART TX
	};

reg[63:0]		timer1MHz;
reg[63:0]		nextTimer1MHz;

reg[63:0]		timer100MHz;
reg[63:0]		nextTimer100MHz;

assign		outTimer1MHz	= timer1MHz;
assign		outTimer100MHz	= timer100MHz;

reg[63:0]		tOutBounceIrq;
reg[63:0]		tOutBounceIrqB;
assign		outBounceIrq	= tOutBounceIrqB;

reg[15:0]		fracTimer1MHz;
reg[15:0]		nextFracTimer1MHz;
reg				stepTimer1MHz;

reg[3:0]		uartBitPos;
reg[3:0]		uartNextBitPos;
reg[15:0]		uartFracTimer;
reg[15:0]		uartNextFracTimer;
reg				uartStepTimer;

reg[79:0]		uartTxFifo;
reg[79:0]		uartTxNextFifo;
reg				uartTxHasByte;
reg				uartTxNextHasByte;

reg[15:0]		uartRxFracTimer;
reg[15:0]		uartNextRxFracTimer;
reg				uartRxStepTimer;

reg[3:0]		uartRxBitPos;
reg[3:0]		uartNextRxBitPos;
reg[79:0]		uartRxFifo;
reg[79:0]		uartRxNextFifo;

reg				uartTxFifoFull;
reg				uartTxFifoEmpty;
reg				uartRxFifoFull;
reg				uartRxFifoReady;

reg				uartLastRx;
reg				uartLastTx;

reg[9:0]		uartRxNextByte;
reg[9:0]		uartRxByte;
reg				uartRxStrobe;

reg[9:0]		uartAddByte;
reg[9:0]		uartHoldByte;
reg[9:0]		uartNextHoldByte;

reg[9:0]		uartRxHoldByte;
reg[9:0]		uartNextRxHoldByte;

reg				mmioInOE;
reg				mmioInWR;
reg				mmioInQ;

reg				mmioInLastOE;
reg				mmioInLastWR;

reg		tMmioLowCSel;
// wire		tMmioLowCSel;
// assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

reg		tUartRxDebP;
reg		tNxtUartRxDebP;

always @*
begin
	gpioNextOut		= gpioOut;
	gpioNextDir		= gpioDir;

	tMmioOK			= UMEM_OK_READY;
//	tMmioOutData	= UV32_XX;
	tMmioOutData	= UV64_XX;
	
	tOutBounceIrq		= 0;
	tNxtUartRxDebP		= tUartRxDebP;

	tMmioLowCSel		= (tMmioAddr[27:16]==12'h000);
	mmioInOE			= (tMmioOpm[3]) && tMmioLowCSel;
	mmioInWR			= (tMmioOpm[4]) && tMmioLowCSel;
	mmioInQ				= tMmioOpm[1:0]==3;

	uartNextFracTimer	= uartFracTimer;
	uartStepTimer		= 0;
	uartNextBitPos		= uartBitPos;
	uartTxNextFifo		= uartTxFifo;
	uartTxNextHasByte	= uartTxHasByte;

	uartNextRxFracTimer	= uartRxFracTimer;
	uartRxStepTimer		= 0;
	uartRxNextFifo		= uartRxFifo;
	uartNextRxBitPos	= uartRxBitPos;
	uartRxNextByte		= uartRxByte;
	uartRxStrobe		= 0;
	
	uartNextHoldByte	= uartHoldByte;
	uartAddByte			= 0;

	uartNextRxHoldByte	= uartRxHoldByte;

	uartTx				= uartLastTx;
	uartRx				= tFixedPinsIn[1];
	
	nextTimer1MHz	= timer1MHz;
	nextTimer100MHz	= timer100MHz + 1;

`ifdef jx2_cpu_mmioclock_100
	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h0290;
`endif

`ifdef jx2_cpu_mmioclock_150
	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h01B5;
`endif

`ifdef jx2_cpu_mmioclock_75
	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h036A;
`endif

`ifdef jx2_cpu_mmioclock_50
	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h0520;
`endif

	if(stepTimer1MHz)
	begin
		nextTimer1MHz	= timer1MHz+1;

//		{ uartStepTimer, uartNextFracTimer }	=
//			{ 1'b0, uartFracTimer } +
//			17'h0275;
//		{ uartRxStepTimer, uartNextRxFracTimer }	=
//			{ 1'b0, uartRxFracTimer } +
//			17'h0275;

//		{ uartStepTimer, uartNextFracTimer }	=
//			{ 1'b0, uartFracTimer } +
//			17'h0EBF;
//		{ uartRxStepTimer, uartNextRxFracTimer }	=
//			{ 1'b0, uartRxFracTimer } +
//			17'h0EBF;

		{ uartStepTimer, uartNextFracTimer }	=
			{ 1'b0, uartFracTimer } +
			17'h1D7E;
		{ uartRxStepTimer, uartNextRxFracTimer }	=
			{ 1'b0, uartRxFracTimer } +
			17'h1D7E;
		uartRxStrobe = uartNextRxFracTimer[15] && !uartRxFracTimer[15];
	end
	
	if(reset)
	begin
		nextTimer1MHz	= 0;
	end
	
	uartTxFifoFull		=  uartTxFifo[79];
	uartTxFifoEmpty		= !uartTxFifo[19];
	uartRxFifoFull		=  uartRxFifo[79];
	uartRxFifoReady		=  uartRxFifo[ 9];
	
	if(uartStepTimer)
	begin
		uartNextBitPos = uartBitPos + 1;
		if(uartBitPos>=9)
		begin
			uartNextBitPos = 0;
			uartTxNextFifo =
				{ 10'b0, uartTxFifo[79:10] };
			uartTxNextHasByte = uartTxFifo[19];
		end
	end
	else
	begin
		if(uartHoldByte[9])
		begin
			if(!uartTxFifo[19])
			begin
				uartTxNextFifo[19:10]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[29])
			begin
				uartTxNextFifo[29:20]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[39])
			begin
				uartTxNextFifo[39:30]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[49])
			begin
				uartTxNextFifo[49:40]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[59])
			begin
				uartTxNextFifo[59:50]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[69])
			begin
				uartTxNextFifo[69:60]=uartHoldByte;
				uartNextHoldByte=0;
			end
			else
			if(!uartTxFifo[79])
			begin
				uartTxNextFifo[79:70]=uartHoldByte;
				uartNextHoldByte=0;
			end
		end
	end

	case(uartBitPos)
		4'h0: begin
			uartTx = !uartTxHasByte;
		end
		4'h1: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[0] : 1'b1;
		end
		4'h2: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[1] : 1'b1;
		end
		4'h3: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[2] : 1'b1;
		end
		4'h4: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[3] : 1'b1;
		end
		4'h5: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[4] : 1'b1;
		end
		4'h6: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[5] : 1'b1;
		end
		4'h7: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[6] : 1'b1;
		end
		4'h8: begin
			uartTx = uartTxHasByte ?
				uartTxFifo[7] : 1'b1;
		end
		4'h9: begin
			uartTx = 1'b1;
		end
		default: begin
		end
	endcase

	if(!uartRx && uartLastRx)
	begin
		if(uartRxBitPos>=9)
		begin
			uartNextRxBitPos	= 0;
			uartNextRxFracTimer	= 0;
			uartRxStepTimer		= 0;
			uartRxNextByte			= 0;
		end
	end

	if(uartRxStepTimer)
	begin
		uartNextRxBitPos = uartRxBitPos + 1;
		if(uartRxBitPos==9)
		begin
			uartNextRxBitPos	= uartRxBitPos;
		end
		else if(uartRxBitPos==8)
		begin
//			uartNextRxBitPos	= uartRxBitPos;
			uartRxNextByte[9]		= 1;
		end
	end
	
	if(uartRxStrobe)
	begin
		case(uartRxBitPos)
			4'h1:	uartRxNextByte[0]=uartRx;
			4'h2:	uartRxNextByte[1]=uartRx;
			4'h3:	uartRxNextByte[2]=uartRx;
			4'h4:	uartRxNextByte[3]=uartRx;
			4'h5:	uartRxNextByte[4]=uartRx;
			4'h6:	uartRxNextByte[5]=uartRx;
			4'h7:	uartRxNextByte[6]=uartRx;
			4'h8:	uartRxNextByte[7]=uartRx;
			default: begin
			end
		endcase
	end
	
	if(uartRxByte[9])
	begin
		$display("UART GetByte %02X", uartRxByte[7:0]);

		if(!uartRxFifo[ 9])
		begin
			uartRxNextFifo[ 9: 0]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[19])
		begin
			uartRxNextFifo[19:10]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[29])
		begin
			uartRxNextFifo[29:20]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[39])
		begin
			uartRxNextFifo[39:30]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[49])
		begin
			uartRxNextFifo[49:40]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[59])
		begin
			uartRxNextFifo[59:50]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[69])
		begin
			uartRxNextFifo[69:60]=uartRxByte;
			uartRxNextByte=0;
		end
		else
		if(!uartRxFifo[79])
		begin
			uartRxNextFifo[79:70]=uartRxByte;
			uartRxNextByte=0;
		end
	end

	casez(tMmioAddr[15:0])
		16'hE000: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
//			tMmioOutData	= timer1MHz[31:0];
//			tMmioOutData	= timer1MHz[63:0];
			tMmioOutData = {
				mmioInQ ? timer1MHz[63:32] : UV32_00,
				timer1MHz[31:0] } ;
		end
		16'hE004: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
//			tMmioOutData	= timer1MHz[63:32];
			tMmioOutData	= { UV32_00, timer1MHz[63:32] };
		end

		16'hE010: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;

			if(mmioInOE && !mmioInLastOE)
			begin
				tNxtUartRxDebP	= 1;
				uartNextRxHoldByte = uartRxFifo[9:0];
//				uartRxNextFifo[79: 0]=
//					{ 10'b0, uartRxFifo[79:10] };
//				tMmioOutData	= { UV24_00, uartNextRxHoldByte[7:0] };
				tMmioOutData	= { UV32_XX, UV24_00, uartNextRxHoldByte[7:0] };
			end
			else
			begin
//				tMmioOutData	= { UV24_00, uartRxHoldByte[7:0] };
				tMmioOutData	= { UV32_XX, UV24_00, uartRxHoldByte[7:0] };
			end
		end

		16'hE014: begin
			if(mmioInOE || mmioInWR)
				tMmioOK			= UMEM_OK_OK;
			if(mmioInWR && !mmioInLastWR)
				uartAddByte		= { 2'b10, tMmioInData[7:0]};
		end

		16'hE018: begin
			tMmioOutData	= {
				UV32_XX,
				UV24_00,
				4'b0000,
				uartTxFifoFull,
				uartTxFifoEmpty,
				uartRxFifoFull,
				uartRxFifoReady
			};

			if(mmioInOE)
			begin
				tMmioOK			= UMEM_OK_OK;

				if(tUartRxDebP)
				begin
					tNxtUartRxDebP	= 0;
					uartRxNextFifo[79: 0]=
						{ 10'b0, uartRxFifo[79:10] };
					tMmioOK			= UMEM_OK_HOLD;
				end
			end

		end

`ifndef def_true
		16'hE080: begin
			if(mmioInWR)
			begin
				tOutBounceIrq	= tMmioInData;
				tMmioOK			= UMEM_OK_OK;
			end
		end
`endif

		16'hE100: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
//			tMmioOutData	= gpioPinsIn;
			tMmioOutData	= { UV32_XX, gpioPinsIn };
		end
		16'hE104: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
//			tMmioOutData	= gpioOut;
			tMmioOutData	= { UV32_XX, gpioOut };
			if(mmioInWR)
				gpioNextOut = tMmioInData[31:0];
		end
		16'hE108: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
			tMmioOutData	= { UV32_XX, gpioDir };
			if(mmioInWR)
				gpioNextDir = tMmioInData[31:0];
		end

		16'hE110: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
			tMmioOutData	= { UV32_XX, gpioOut };
			if(mmioInWR)
				gpioNextOut = gpioOut | tMmioInData[31:0];
		end
		16'hE114: begin
			if(mmioInOE)
				tMmioOK			= UMEM_OK_OK;
			tMmioOutData	= { UV32_XX, gpioOut };
			if(mmioInWR)
				gpioNextOut = gpioOut&(~tMmioInData[31:0]);
		end

		default:
		begin
		end
	endcase
end


always @(posedge clock)
begin
	tFixedPinsOutB	<= tFixedPinsOut;
	tFixedPinsIn	<= fixedPinsIn;

	tUartRxDebP		<= tNxtUartRxDebP;

	tMmioOK2		<= tMmioOK;
	tMmioOutData2	<= tMmioOutData;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;

	mmioInLastOE	<= mmioInOE;
	mmioInLastWR	<= mmioInWR;

	gpioOut			<= gpioNextOut;
	gpioDir			<= gpioNextDir;

	timer1MHz		<= nextTimer1MHz;
	timer100MHz		<= nextTimer100MHz;
	fracTimer1MHz	<= nextFracTimer1MHz;
	
	uartFracTimer	<= uartNextFracTimer;
	uartBitPos		<= uartNextBitPos;
	uartTxFifo		<= uartTxNextFifo;
	uartTxHasByte	<= uartTxNextHasByte;

	uartRxFracTimer	<= uartNextRxFracTimer;
	uartRxBitPos	<= uartNextRxBitPos;
	uartRxFifo		<= uartRxNextFifo;
	uartRxByte		<= uartRxNextByte;

	uartLastTx		<= uartTx;
	uartLastRx		<= uartRx;

	uartHoldByte	<= uartAddByte[9] ?
		uartAddByte : uartNextHoldByte;

	uartRxHoldByte	<= uartNextRxHoldByte;

	tOutBounceIrqB	<= tOutBounceIrq;
end


endmodule
