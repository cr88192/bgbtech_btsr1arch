/*
Deal with an external H-Bridge via PWM.

This will assume a Quad H-Bridge or two Dual H-Bridge.
Assume that the Idle State for each H-Bridge driver is either 00 or 11.

TBD:
* Support servo PWM or Step/Dir signaling?...

Each output will have an 8-bit duty cycle, given as a value from 01..FF.
* 00: Disable (00 or 11)
* 01..7F: Negative (01=Max Negative)
* 80: Neutral (or 50% duty cycle)
* 81..FF: Positive (FF=Max Positive)

Control (Base+0):
  ( 3: 0): PWM Drive Type 0
  ( 7: 4): PWM Drive Type 1
  (11: 8): PWM Drive Type 2
  (15:12): PWM Drive Type 3
  (19:16): H-Bridge PWM Drive Clock

Drive Type:
* 0000: H-Bridge, Normal, Idle=00
* 0001: H-Bridge, Invert, Idle=00
* 0010: H-Bridge, Normal, Idle=11
* 0011: H-Bridge, Invert, Idle=11
* 0100: Servo, Normal
* 0101: Servo, Invert
* 0110: -
* 0111: -

Value (Base+8):
  ( 7: 0): PWM Value 0, 00..FF
  (15: 8): PWM Value 1, 00..FF
  (23:16): PWM Value 2, 00..FF
  (31:24): PWM Value 3, 00..FF

H-Bridge PWM Drive Clock:
* 0000:  762.9 Hz
* 0001:   1.52 kHz
* 0010:   3.05 kHz
* 0011:   6.10 kHz
* 0100:   12.2 kHz
* 0101:   24.4 kHz
* 0110:   48.8 kHz
* 0111:   97.6 kHz
* 1000:  195.3 kHz
* 1001:  390.6 kHz
* 1010:  781.3 kHz
* 1011:   1.56 MHz
* 1100:   3.13 MHz
* 1101:   6.25 MHz
* 1110:  12.50 MHz
* 1111:  25.00 MHz

Servo:
* Will drive pulses between 1ms and 2ms.

 */

`include "CoreDefs.v"

module ModHBridge(
	/* verilator lint_off UNUSED */
	clock,			reset,

	out_pwm4x,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioSelfAddr
	);

input			clock;
input			reset;

output[7:0]		out_pwm4x;

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;
input[11:0]		mmioSelfAddr;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (tMmioAddr[15:4]==mmioSelfAddr);

reg[7:0]		tOutPwm;
reg[7:0]		tOutPwm2;
assign		out_pwm4x = tOutPwm2;

reg				mmioInOE;
reg				mmioInWR;

reg				mmioLatchWR;
reg				mmioNxtLatchWR;

reg[31:0]		tRegCtrl;
reg[31:0]		tNxtRegCtrl;

reg[31:0]		tRegValue;
reg[31:0]		tNxtRegValue;

reg[7:0]		tPwmAcc0;
reg[7:0]		tPwmAcc1;
reg[7:0]		tPwmAcc2;
reg[7:0]		tPwmAcc3;

reg[7:0]		tNxtPwmAcc0;
reg[7:0]		tNxtPwmAcc1;
reg[7:0]		tNxtPwmAcc2;
reg[7:0]		tNxtPwmAcc3;

reg[9:0]		tServoRamp0;
reg[9:0]		tServoRamp1;
reg[9:0]		tServoRamp2;
reg[9:0]		tServoRamp3;
reg[9:0]		tNxtServoRamp0;
reg[9:0]		tNxtServoRamp1;
reg[9:0]		tNxtServoRamp2;
reg[9:0]		tNxtServoRamp3;

reg				tPwmBit0;
reg				tPwmBit1;
reg				tPwmBit2;
reg				tPwmBit3;
reg				tPwmStrobe;

reg[17:0]		tPwmClkAcc;
reg[17:0]		tNxtPwmClkAcc;
reg				tPwmClkBit;
reg				tPwmClkBitL1;
reg				tPwmClkBitL2;

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE		= (tMmioOpm[3]) && tMmioSelfCSel;
	mmioInWR		= (tMmioOpm[4]) && tMmioSelfCSel;
//	mmioNxtLatchWR	= mmioInWR && mmioLatchWR;


	tNxtRegCtrl		= tRegCtrl;
	tNxtRegValue	= tRegValue;

	tNxtPwmClkAcc	= tPwmClkAcc;
	tPwmClkBit = tPwmClkAcc[ { 1'b0, ~tRegCtrl[19:16] } ];
	tPwmStrobe	= tPwmClkBitL1 && !tPwmClkBitL2;

	{ tPwmBit0, tNxtPwmAcc0 }	=
		{ 1'b0, tPwmAcc0 } + { 1'b0, tRegValue[ 7: 0] };
	{ tPwmBit1, tNxtPwmAcc1 }	=
		{ 1'b0, tPwmAcc1 } + { 1'b0, tRegValue[15: 8] };
	{ tPwmBit2, tNxtPwmAcc2 }	=
		{ 1'b0, tPwmAcc2 } + { 1'b0, tRegValue[23:16] };
	{ tPwmBit3, tNxtPwmAcc3 }	=
		{ 1'b0, tPwmAcc3 } + { 1'b0, tRegValue[31:24] };

	tOutPwm[1:0] = (tPwmBit0 ^ tRegCtrl[ 0]) ? 2'b01 : 2'b10;
	tOutPwm[3:2] = (tPwmBit1 ^ tRegCtrl[ 4]) ? 2'b01 : 2'b10;
	tOutPwm[5:4] = (tPwmBit2 ^ tRegCtrl[ 8]) ? 2'b01 : 2'b10;
	tOutPwm[7:6] = (tPwmBit3 ^ tRegCtrl[12]) ? 2'b01 : 2'b10;

	tNxtServoRamp0 = { 2'b00, tRegValue[ 7: 0] } + 165;
	tNxtServoRamp1 = { 2'b00, tRegValue[15: 8] } + 165;
	tNxtServoRamp2 = { 2'b00, tRegValue[23:16] } + 165;
	tNxtServoRamp3 = { 2'b00, tRegValue[31:24] } + 165;

	if(tRegCtrl[ 2])
	begin
		tOutPwm[0] = tPwmClkAcc[16:7] < tServoRamp0;
		tOutPwm[1] = 0;
	end

	if(tRegCtrl[ 6])
	begin
		tOutPwm[2] = tPwmClkAcc[16:7] < tServoRamp1;
		tOutPwm[3] = 0;
	end

	if(tRegCtrl[10])
	begin
		tOutPwm[4] = tPwmClkAcc[16:7] < tServoRamp2;
		tOutPwm[5] = 0;
	end

	if(tRegCtrl[15])
	begin
		tOutPwm[4] = tPwmClkAcc[16:7] < tServoRamp3;
		tOutPwm[5] = 0;
	end



	if(tRegValue[ 7: 0] == 0)
		tOutPwm[1:0] = tRegCtrl[ 1] ? 2'b11 : 2'b00;
	if(tRegValue[15: 8] == 0)
		tOutPwm[3:2] = tRegCtrl[ 5] ? 2'b11 : 2'b00;
	if(tRegValue[23:16] == 0)
		tOutPwm[5:4] = tRegCtrl[ 9] ? 2'b11 : 2'b00;
	if(tRegValue[31:24] == 0)
		tOutPwm[7:6] = tRegCtrl[13] ? 2'b11 : 2'b00;


	if((tMmioAddr[3:2]==2'b00) && mmioInOE)
	begin
		tMmioOutData	= { 32'h00, tRegCtrl };
		tMmioOK			= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInOE)
	begin
		tMmioOutData	= { 32'h00, tRegValue };
		tMmioOK			= UMEM_OK_OK;
	end


	if((tMmioAddr[3:2]==2'b00) && mmioInWR)
	begin
		tNxtRegCtrl		= tMmioInData[31:0];
		tMmioOK			= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInWR)
	begin
		tNxtRegValue	= tMmioInData[31:0];
		tMmioOK			= UMEM_OK_OK;
	end

end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;
	
	tOutPwm2		<= tOutPwm;

	tPwmClkAcc		<= tNxtPwmClkAcc;
	tPwmClkBitL1	<= tPwmClkBit;
	tPwmClkBitL2	<= tPwmClkBitL1;

	if(tPwmStrobe)
	begin
		tPwmAcc0		<= tNxtPwmAcc0;
		tPwmAcc1		<= tNxtPwmAcc1;
		tPwmAcc2		<= tNxtPwmAcc2;
		tPwmAcc3		<= tNxtPwmAcc3;
	end

	tServoRamp0		<= tNxtServoRamp0;
	tServoRamp1		<= tNxtServoRamp1;
	tServoRamp2		<= tNxtServoRamp2;
	tServoRamp3		<= tNxtServoRamp3;

	tRegCtrl		<= tNxtRegCtrl;
	tRegValue		<= tNxtRegValue;

//	tDivCnt			<= tNxtDivCnt;
//	tDivRst			<= tNxtDivRst;

	mmioLatchWR		<= mmioNxtLatchWR;
end

endmodule
