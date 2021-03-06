/*
Clock Pulser
 */

module MmiModClkp(
	clock,	reset,
	timer4MHz,
	timer1MHz,
	timer64kHz,
	timer1kHz,
	timer256Hz
	);

input	clock;
input	reset;

output	timer4MHz;
output	timer1MHz;
output	timer64kHz;
output	timer1kHz;
output	timer256Hz;


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

reg[15:0]	fracTimer1kHz;
reg[15:0]	nextFracTimer1kHz;
reg			stepTimer1kHz;

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

assign	timer4MHz	= stepTimer4MHzB;
assign	timer1MHz	= stepTimer1MHzB;
assign	timer64kHz	= stepTimer64kHzB;
assign	timer1kHz	= stepTimer1kHzB;

assign	timer256Hz	= stepTimer256Hz;

always @*
begin
	{ stepTimer1MHz, nextFracTimer1MHz }	=
		{ 1'b0, fracTimer1MHz } + 17'h0290;

	{ stepTimer4MHz, nextFracTimer4MHz }	=
		{ 1'b0, fracTimer4MHz } + 17'h0A40;

//	{ stepTimer32MHz, nextFracTimer32MHz }	=
//		{ 1'b0, fracTimer32MHz } + 17'h5200;

	{ stepTimer64kHz, nextFracTimer64kHz }	=
		{ 1'b0, fracTimer64kHz } + 21'h02AC;

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
end

endmodule
