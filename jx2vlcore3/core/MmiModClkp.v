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
Clock Pulser
 */

module MmiModClkp(
	clock,	reset,
	timer4MHz,
	timer1MHz,
	timer64kHz,
	timer1kHz,
	timer256Hz,
	timerNPat,
	clock1MHz
	);

input	clock;
input	reset;

output	timer4MHz;
output	timer1MHz;
output	timer64kHz;
output	timer1kHz;
output	timer256Hz;
output	timerNPat;
output	clock1MHz;


reg[19:0]	fracTimer1MHz;
reg[19:0]	nextFracTimer1MHz;
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

reg[15:0]	fracTimer1kHz;
reg[15:0]	nextFracTimer1kHz;
reg			stepTimer1kHz;

reg[15:0]	fracTimerNa;
reg[15:0]	nextFracTimerNa;
reg			stepTimerNa;

reg[15:0]	fracTimerNb;
reg[15:0]	nextFracTimerNb;
reg			stepTimerNb;


reg			curClk512Hz;
reg			nxtClk512Hz;
reg			curClk256Hz;
reg			nxtClk256Hz;
reg			stepTimer512Hz;
reg			stepTimer256Hz;

reg			stepTimer4MHzB;
reg			stepTimer1MHzB;
reg			stepTimer64kHzB;
reg			stepTimer1kHzB;
reg			stepTimer256HzB;

reg			tTimerNPat;		//Noise Pattern

assign	timer4MHz	= stepTimer4MHzB;
assign	timer1MHz	= stepTimer1MHzB;
assign	timer64kHz	= stepTimer64kHzB;
assign	timer1kHz	= stepTimer1kHzB;

assign	timer256Hz	= stepTimer256Hz;
assign	timerNPat	= tTimerNPat;

reg			tClock1MHz;
assign	clock1MHz	= tClock1MHz;

always @*
begin

`ifdef jx2_cpu_mmioclock_150

	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 21'h01B50;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h06D4;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h01BF;

`endif

`ifdef jx2_cpu_mmioclock_100

	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 21'h02900;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h0A40;

//	{ stepTimer32MHz, nextFracTimer32MHz }	=
//		{ 1'b0, fracTimer32MHz } + 17'h5200;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h02AC;

`endif

`ifdef jx2_cpu_mmioclock_75

	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 21'h036A0;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h0DA8;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h037E;

`endif

`ifdef jx2_cpu_mmioclock_50

	{ stepTimer1MHz, nextFracTimer1MHz }	=
//		{ 1'b0, fracTimer1MHz } + 21'h05200;
		{ 1'b0, fracTimer1MHz } + 21'h051EB;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h1480;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h0558;

`endif

	{ stepTimer1kHz, nextFracTimer1kHz }	=
		{ 1'b0, fracTimer1kHz } +
		((stepTimer1MHz) ? 17'h0043 : 17'h0000);

	nxtClk512Hz		= curClk512Hz;
	stepTimer512Hz	= 0;
	if(stepTimer1kHz)
	begin
		nxtClk512Hz=!curClk512Hz;
		if(!curClk512Hz)
			stepTimer512Hz	= 1;
	end

	nxtClk256Hz		= curClk256Hz;
	stepTimer256Hz	= 0;
	if(stepTimer512Hz)
	begin
		nxtClk256Hz=!curClk256Hz;
		if(!curClk256Hz)
			stepTimer256Hz	= 1;
	end

//	{ stepTimerNa, nextFracTimerNa }	=
//		{ 1'b0, fracTimerNa } + 17'h00001;
//	{ stepTimerNb, nextFracTimerNb }	=
//		{ 1'b0, fracTimerNb } + 17'h0000D;

	{ stepTimerNa, nextFracTimerNa }	=
		{ 1'b0, fracTimerNa } + 17'h00003;
	{ stepTimerNb, nextFracTimerNb }	=
		{ 1'b0, fracTimerNb } + 17'h00011;

	tTimerNPat =
		JX2_KRR_FIXKEY3[fracTimerNa[5:0]] ^
		JX2_KRR_FIXKEY4[fracTimerNb[5:0]];
	
end

always @(posedge clock)
begin
	stepTimer4MHzB	<= stepTimer4MHz;
	stepTimer1MHzB	<= stepTimer1MHz;
	stepTimer64kHzB	<= stepTimer64kHz;
	stepTimer1kHzB	<= stepTimer1kHz;
	stepTimer256HzB	<= stepTimer256Hz;

	fracTimer1MHz	<= nextFracTimer1MHz;
	fracTimer4MHz	<= nextFracTimer4MHz;
//	fracTimer32MHz	<= nextFracTimer32MHz;
	fracTimer64kHz	<= nextFracTimer64kHz;
	fracTimer1kHz	<= nextFracTimer1kHz;
	curClk512Hz		<= nxtClk512Hz;
	curClk256Hz		<= nxtClk256Hz;

	fracTimerNa		<= nextFracTimerNa;
	fracTimerNb		<= nextFracTimerNb;
	
//	tClock1MHz		<= fracTimer1MHz[15];
	tClock1MHz		<= fracTimer1MHz[19];
end

endmodule
