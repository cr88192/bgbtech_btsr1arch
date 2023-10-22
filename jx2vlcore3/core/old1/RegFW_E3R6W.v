/*
Register Forwarding, EX1/EX2/EX3, 6 write ports
 */

module RegFW_E3R6W(
	clock,
	reset,

	exHold1,
	exHold2,

	regIdRs,		//Fetch ID
	regValRsA,		//Fetch Value (In)
	regValRs,		//Fetch Value (Out)
	regValRsH,		//Fetch Value (Out)

	regIdR1nA1,		//Destination ID (EX1, Lane 1)
	regValR1nA1,	//Destination Value (EX1, Lane 1)
	regIdR1nB1,		//Destination ID (EX1, Lane 2)
	regValR1nB1,	//Destination Value (EX1, Lane 2)
	regIdR1nC1,		//Destination ID (EX1, Lane 3)
	regValR1nC1,	//Destination Value (EX1, Lane 3)

	regIdR2nA1,		//Destination ID (EX1, Lane 1)
	regValR2nA1,	//Destination Value (EX1, Lane 1)
	regIdR2nB1,		//Destination ID (EX1, Lane 2)
	regValR2nB1,	//Destination Value (EX1, Lane 2)
	regIdR2nC1,		//Destination ID (EX1, Lane 3)
	regValR2nC1,	//Destination Value (EX1, Lane 3)

	regIdR1nA2,		//Destination ID (EX2, Lane 1)
	regValR1nA2,	//Destination Value (EX2, Lane 1)	
	regIdR1nB2,		//Destination ID (EX2, Lane 2)
	regValR1nB2,	//Destination Value (EX2, Lane 2)
	regIdR1nC2,		//Destination ID (EX2, Lane 3)
	regValR1nC2,	//Destination Value (EX2, Lane 3)

	regIdR2nA2,		//Destination ID (EX2, Lane 1)
	regValR2nA2,	//Destination Value (EX2, Lane 1)	
	regIdR2nB2,		//Destination ID (EX2, Lane 2)
	regValR2nB2,	//Destination Value (EX2, Lane 2)
	regIdR2nC2,		//Destination ID (EX2, Lane 3)
	regValR2nC2,	//Destination Value (EX2, Lane 3)

	regIdR1nA3,		//Destination ID (EX2, Lane 1)
	regValR1nA3,	//Destination Value (EX2, Lane 1)
	regIdR1nB3,		//Destination ID (EX2, Lane 2)
	regValR1nB3,	//Destination Value (EX2, Lane 2)
	regIdR1nC3,		//Destination ID (EX2, Lane 3)
	regValR1nC3,	//Destination Value (EX2, Lane 3)
	
	regIdR2nA3,		//Destination ID (EX2, Lane 1)
	regValR2nA3,	//Destination Value (EX2, Lane 1)
	regIdR2nB3,		//Destination ID (EX2, Lane 2)
	regValR2nB3,	//Destination Value (EX2, Lane 2)
	regIdR2nC3,		//Destination ID (EX2, Lane 3)
	regValR2nC3,	//Destination Value (EX2, Lane 3)

	gprEx1Flush1,
	gprEx1Flush2,

	gprEx2Flush1,
	gprEx2Flush2,

	gprEx3Flush1,
	gprEx3Flush2

	);

input			clock;
input			reset;

input			exHold1;
input			exHold2;

`input_gpr		regIdRs;
output[63:0]	regValRs;
input[63:0]		regValRsA;
output			regValRsH;

`input_gpr		regIdR1nA1;		//Destination ID
input[63:0]		regValR1nA1;	//Destination Value
`input_gpr		regIdR1nA2;		//Destination ID
input[63:0]		regValR1nA2;	//Destination Value
`input_gpr		regIdR1nA3;		//Destination ID
input[63:0]		regValR1nA3;	//Destination Value

`input_gpr		regIdR2nA1;		//Destination ID
input[63:0]		regValR2nA1;	//Destination Value
`input_gpr		regIdR2nA2;		//Destination ID
input[63:0]		regValR2nA2;	//Destination Value
`input_gpr		regIdR2nA3;		//Destination ID
input[63:0]		regValR2nA3;	//Destination Value


`input_gpr		regIdR1nB1;		//Destination ID
input[63:0]		regValR1nB1;	//Destination Value
`input_gpr		regIdR1nB2;		//Destination ID
input[63:0]		regValR1nB2;	//Destination Value
`input_gpr		regIdR1nB3;		//Destination ID
input[63:0]		regValR1nB3;	//Destination Value

`input_gpr		regIdR2nB1;		//Destination ID
input[63:0]		regValR2nB1;	//Destination Value
`input_gpr		regIdR2nB2;		//Destination ID
input[63:0]		regValR2nB2;	//Destination Value
`input_gpr		regIdR2nB3;		//Destination ID
input[63:0]		regValR2nB3;	//Destination Value


`input_gpr		regIdR1nC1;		//Destination ID
input[63:0]		regValR1nC1;	//Destination Value
`input_gpr		regIdR1nC2;		//Destination ID
input[63:0]		regValR1nC2;	//Destination Value
`input_gpr		regIdR1nC3;		//Destination ID
input[63:0]		regValR1nC3;	//Destination Value

`input_gpr		regIdR2nC1;		//Destination ID
input[63:0]		regValR2nC1;	//Destination Value
`input_gpr		regIdR2nC2;		//Destination ID
input[63:0]		regValR2nC2;	//Destination Value
`input_gpr		regIdR2nC3;		//Destination ID
input[63:0]		regValR2nC3;	//Destination Value

input			gprEx1Flush1;
input			gprEx2Flush1;
input			gprEx3Flush1;

input			gprEx1Flush2;
input			gprEx2Flush2;
input			gprEx3Flush2;


reg[63:0]	tRegValRs;
reg			tValRsZz;
assign	regValRs = tRegValRs;

always @*
begin
	
	tRegValRs	= regValRsA;
	tValRsZz	= 0;
	
	case(regIdRs)
		JX2_GR_IMM:		tValRsZz	= 1;
		JX2_GR_ZZR:		tValRsZz	= 1;
		JX2_GR_JIMM:	tValRsZz	= 1;
		JX2_GR_R8IMMH:	tValRsZz	= 1;
		default:		tValRsZz	= 0;
	endcase

	if(!tValRsZz)
	begin
		if(regIdRs==regIdR1nC3)
			tRegValRs=regValR1nC3;
		if(regIdRs==regIdR1nB3)
			tRegValRs=regValR1nB3;
		if(regIdRs==regIdR1nA3)
			tRegValRs=regValR1nA3;

		if(regIdRs==regIdR2nC3)
			tRegValRs=regValR2nC3;
		if(regIdRs==regIdR2nB3)
			tRegValRs=regValR2nB3;
		if(regIdRs==regIdR2nA3)
			tRegValRs=regValR2nA3;


		if(regIdRs==regIdR1nC2)
			tRegValRs=regValR1nC2;
		if(regIdRs==regIdR1nB2)
			tRegValRs=regValR1nB2;
		if(regIdRs==regIdR1nA2)
			tRegValRs=regValR1nA2;

		if(regIdRs==regIdR2nC2)
			tRegValRs=regValR2nC2;
		if(regIdRs==regIdR2nB2)
			tRegValRs=regValR2nB2;
		if(regIdRs==regIdR2nA2)
			tRegValRs=regValR2nA2;


		if(regIdRs==regIdR1nC1)
			tRegValRs=regValR1nC1;
		if(regIdRs==regIdR1nB1)
			tRegValRs=regValR1nB1;
		if(regIdRs==regIdR1nA1)
			tRegValRs=regValR1nA1;

		if(regIdRs==regIdR2nC1)
			tRegValRs=regValR2nC1;
		if(regIdRs==regIdR2nB1)
			tRegValRs=regValR2nB1;
		if(regIdRs==regIdR2nA1)
			tRegValRs=regValR2nA1;
	end
end


endmodule
