/*
LPDDR3 Interface Module

State Machine, Transmit/Recieve Words

CAS = Column Activate Strobe
  Divided into RL and WL.
    RL=Read Latency
    WL=Write Latency

The RL/WL values would depend both on chipset and what is written into the Mode Registers. These should match.

RAS = Row Activate Strobe
  This is how many cycles between row activation and beginning a Read/Write.


	00-0000		//Idle, Awaiting Request
	00-0001		//Hold, Waiting to return to Idle
	00-0010		//Mode Register Write (Rise)
	00-0011		//Mode Register Write (Fall)

	00-1zzz		//Precharge Row

	00-1000		//Precharge, Row Activate Rise
	00-1001		//Precharge, Row Activate Fall
	00-1010		//Precharge, RAS Hold Rise
	00-1011		//Precharge, RAS Hold Fall
	00-1100		//Precharge, Precharge Rise
	00-1101		//Precharge, Precharge Fall
	00-1110		//Precharge, Wait Spin (Rise), Returns Idle
	00-1111		//Precharge, Wait Spin (Fall), Returns Idle

	01-0000		//Read, Row Activate Rise
	01-0001		//Read, Row Activate Fall
	01-0010		//Read, RAS Hold Rise
	01-0011		//Read, RAS Hold Fall
	01-0100		//Read, Column Activate Rise
	01-0101		//Read, Column Activate Fall
	01-0110		//Read, CAS/RL Hold Rise
	01-0111		//Read, CAS/RL Hold Fall

	01-1000		//Write, Row Activate Rise
	01-1001		//Write, Row Activate Fall
	01-1010		//Write, RAS Hold Rise
	01-1011		//Write, RAS Hold Fall
	01-1100		//Write, Column Activate Rise
	01-1101		//Write, Column Activate Fall
	01-1110		//Write, CAS/WL Hold Rise
	01-1111		//Write, CAS/WL Hold Fall

	10-zzzz		//Read Burst
	11-zzzz		//Write Burst

Note: ddrData_I/ddrData_O;
These exist because verilator lacks tristate IO.


*/

`include "CoreDefs.v"

// `define mod_ddr3_isddr2		//We are dealing with DDR2, not DDR3

// `define mod_ddr_dbgprn		//Debug DDR module

module MmiModDdr3(
	clock,	reset,
	memDataIn,	memDataOut,
	memAddr,	memOpm,
	memOK,
	
//	ddrData,
	ddrData_I,
	ddrData_O,
	ddrData_En,
	ddrAddr,
	ddrBa,
	ddrCs, ddrRas, ddrCas, ddrWe, ddrCke,
	ddrClk);

input			clock;
input			reset;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[31:0]		memAddr;
input[4:0]		memOpm;
output[1:0]		memOK;
	
// inout[15:0]		ddrData;		//DDR data pins
input[15:0]		ddrData_I;		//DDR data pins
output[15:0]	ddrData_O;		//DDR data pins
output			ddrData_En;		//DDR data pins

output[13:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

// output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins

// parameter[7:0]	DDR_CAS_RL_M1	= 4;	//CAS RL Minus 1
parameter[7:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
// parameter[7:0]	DDR_CAS_WL_M1	= 4;	//CAS WL Minus 1
parameter[7:0]	DDR_CAS_WL_M1	= 2;	//CAS WL Minus 1
parameter[7:0]	DDR_RAS_M1		= 8;	//RAS Minus 1

parameter[7:0]	DDR_RAS_INIT	= 128;	//Wait several uS

// parameter[63:0]	DDR_DRI_INIT	= 64'h0000_0122_0201_0AFF;
// parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0010_8200;
parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0033_8200;

/* verilator lint_off UNOPTFLAT */

reg[15:0]		tDdrData;
reg				tDdrOut;
// assign			ddrData = tDdrOut ? tDdrData : 16'hzzzz;
// assign			ddrData = tDdrData;

wire[15:0]		ddrData;
assign			ddrData_O = tDdrData;
assign			ddrData = ddrData_I;
assign			ddrData_En = tDdrOut;

// reg[9:0]		tDdrCmd;			//Command/Address pins
reg[1:0]		tDdrClk;			//clock pins
// assign			ddrCmd = tDdrCmd;
assign			ddrClk = tDdrClk;

reg[13:0]		tDdrAddr;		//Address pins
reg[2:0]		tDdrBa;			//Bank Address pins
reg				tDdrCs;
reg				tDdrRas;
reg				tDdrCas;
reg				tDdrWe;
reg				tDdrCke;

assign		ddrAddr	= tDdrAddr;
assign		ddrBa	= tDdrBa;
assign		ddrCs	= tDdrCs;
assign		ddrRas	= tDdrRas;
assign		ddrCas	= tDdrCas;
assign		ddrWe	= tDdrWe;
assign		ddrCke	= tDdrCke;

reg[13:0]		tDdrLastAddr;	//Address pins
reg[2:0]		tDdrLastBa;		//Bank Address pins
reg				tDdrLastCs;
reg				tDdrLastRas;
reg				tDdrLastCas;
reg				tDdrLastWe;
reg				tDdrLastCke;

reg[1:0]		tMemOK;
reg[1:0]		tMemOK2;
assign			memOK = tMemOK2;

reg[127:0]		tMemDataOut;
reg[127:0]		tMemDataOut2;
assign			memDataOut = tMemDataOut2;

reg[127:0]		tMemDataIn;
reg[31:0]		tMemAddr;
reg[4:0]		tMemOpm;


reg[5:0]		accState;
reg[5:0]		accNextState;

reg				accCkLo;
reg				accNextCkLo;

reg[127:0]		accReadBlk;
reg[127:0]		accNextReadBlk;

reg[31:0]		accAddr;
reg[31:0]		accNextAddr;

reg[15:0]		accRowAddr;
reg[15:0]		accColAddr;
reg[ 2:0]		accBaAddr;
reg[15:0]		accNextRowAddr;
reg[15:0]		accNextColAddr;
reg[ 2:0]		accNextBaAddr;

reg[7:0]		accCkCas;		//Clock-CAS / Wait Time
reg[7:0]		accNextCkCas;


reg[15:0]		dreRowAddr;			//DRAM Refresh, Current Row
reg[15:0]		dreNextRowAddr;		//DRAM Refresh, Next Row
reg[15:0]		dreCount;			//DRAM Refresh, Cycle Count
reg[15:0]		dreNextCount;		//DRAM Refresh, Next Cycle Count
reg				dreIsZero;

reg[63:0]		driModeOut;			//DRAM Init, Mode Out
reg[63:0]		driNextModeOut;		//DRAM Init, Next Out
reg				driStillInit;

/* verilator lint_on UNOPTFLAT */


always @*
begin
	accNextState	= accState;
	accNextCkLo		= !accCkLo;

	accNextReadBlk	= accReadBlk;
	tDdrOut			= 0;
//	tDdrData		= 0;
	tDdrData		= 16'hzzzz;
	
	tMemDataOut		= UV128_XX;

//	tDdrCmd			= 10'b0000000111;
	tDdrClk			= accNextCkLo ? 2'b10 : 2'b01;

	if(accState[0])
	begin
		tDdrAddr	= tDdrLastAddr;
		tDdrBa		= tDdrLastBa;
		tDdrCke		= tDdrLastCke;
		tDdrCs		= tDdrLastCs;
		tDdrRas		= tDdrLastRas;
		tDdrCas		= tDdrLastCas;
		tDdrWe		= tDdrLastWe;
	end
	else
	begin
		tDdrAddr	= 0;
		tDdrBa		= 0;
		tDdrCke		= 1;
		tDdrCs		= 1;
		tDdrRas		= 1;
		tDdrCas		= 1;
		tDdrWe		= 1;
	end

	accNextCkCas	= accCkCas;

	accNextAddr		= accAddr;
	accNextRowAddr	= accRowAddr;
	accNextColAddr	= accColAddr;
	accNextBaAddr	= accBaAddr;

	dreNextRowAddr	= dreRowAddr;
	dreNextCount	= dreCount;
	
	driNextModeOut	= driModeOut;

	tMemOK			= UMEM_OK_READY;

	driStillInit	= (driModeOut[15:0]!=0);
	dreIsZero		= (dreCount == 0);

	if(!dreIsZero && accCkLo)
	begin
		dreNextCount = dreCount - 1;
	end

	if(driStillInit)
	begin
//		$display("DRI Mode %X", driModeOut);
	end

	/* If doing something Load/Store requests can wait. */
	if(accState!=0)
	begin
		if(tMemOpm[3])
			tMemOK		= UMEM_OK_HOLD;
		if(tMemOpm[4])
			tMemOK		= UMEM_OK_HOLD;
	end

//	$display("ModDdr: State %X", accState);

	case(accState)
	6'b000000: begin	/* Waiting for request */
//		$display("ModDdr: State 0");

		accNextState = 6'b000000;

		if(tMemOpm[3] && !driStillInit && !dreIsZero)	/* Load Request */
		begin
//			$display("ModDdr: Read Req, A=%X", tMemAddr);

			tMemOK		= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				accNextState = 6'b010000;
				
				accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};

//				accNextRowAddr = accNextAddr[24:9];
//				accNextColAddr = {7'b0, accNextAddr[ 8:0]};

				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};

//				accNextRowAddr = tMemAddr[28:13];
//				accNextBaAddr = tMemAddr[12:10];
//				accNextColAddr = {6'b0, tMemAddr[ 9:0]};

//				$display("Read Row=%X Bank=%X Col=%X",
//					accNextRowAddr,
//					accNextBaAddr,
//					accNextColAddr);
			end
		end
		else
			if(tMemOpm[4] && !driStillInit && !dreIsZero)	/* Store Request */
		begin
			$display("ModDdr: Store Req, A=%X", tMemAddr);

			accNextReadBlk	= tMemDataIn;
			tMemOK			= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				if(tMemOpm==UMEM_OPM_WR_SW)
				begin
					accNextState = 6'b000010;
					accNextRowAddr = tMemDataIn[15:0];
				end
				else
				begin
					accNextState = 6'b011000;

					accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};
					accNextRowAddr = accNextAddr[28:13];
					accNextBaAddr = accNextAddr[12:10];
					accNextColAddr = {6'b0, accNextAddr[ 9:0]};
					accNextReadBlk = tMemDataIn;
				end
			end
		end
		else
		begin
			if(accCkLo)
			begin
//				$display("ModDdr: IdleClk");

				if(driModeOut[15:0]!=0)
				begin
					accNextState	= 6'b000010;
					accNextRowAddr	= driModeOut[15:0];
					driNextModeOut	= { 16'h0000, driModeOut[63:16] };
				end
				else
//				if(dreCount == 0)
				if(dreIsZero)
				begin
					accNextState	= 6'b001000;

					accNextRowAddr	= dreRowAddr;
					accNextBaAddr = 0;
					accNextColAddr = 0;

					dreNextRowAddr	= { 3'b000, dreRowAddr[12:0] + 13'h1 };
					dreNextCount = 8192;
				end
//				else
//				begin
//					dreNextCount = dreCount - 1;
//				end
			end
		end

	end

	6'b000001: begin	/* Access Complete, Hold */
		if(tMemOpm[4] || tMemOpm[3])
		begin
//			$display("DDR ReadBlk %X", accReadBlk);
			/* Hang out here until released. */
			tMemDataOut		= accReadBlk;
			tMemOK			= UMEM_OK_OK;
			accNextState	= 6'b000001;
		end
		else
		begin
			/* Return to waiting state. */
			accNextState	= 6'b000000;
		end
	end

	

	6'b000010: begin
		/* Mode Register Write (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10  R9  R8  Hi  Lo */
//		tDdrCmd			= {accRowAddr[13:8], 4'b0000};

		tDdrAddr		= {1'b0, accRowAddr[12: 0] };
		tDdrBa			= {1'b0, accRowAddr[14:13] };
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 0;	tDdrWe	= 0;

		if(accRowAddr[15:12]==4'h8)
		begin
			tDdrCs	= 0;	tDdrRas	= 1;
			tDdrCas	= 1;	tDdrWe	= 0;
		end

		accNextState	= 6'b000011;
	end
	6'b000011: begin
		/* Mode Register Write (Fall) */
		/* R14 R13  R7  R6  R5  R4  R3  R2  R1  R0 */
//		tDdrCmd			= {accRowAddr[7:0], accRowAddr[15:14]};
//		accNextState	= 6'b000001;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
//		if(accRowAddr[15:8]==8'h0A)
		if(accRowAddr[15:12]==4'h8)
			accNextCkCas	= DDR_RAS_INIT;
		accNextState	= 6'b001110;
	end
	


	/* Precharge Start */
	
	6'b001000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10 R9 R8 Hi Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b001001;
	end
	6'b001001: begin
		/* Row Activate (Fall) */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b001010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b001010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b001011;
	end
	6'b001011: begin
		/* RAS Hold (Fall) */
		accNextState	= 6'b001100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001010;
		end
	end

	6'b001100: begin
		/* Precharge (Rise) */
		/*  -  -  -  -  -  1  1  0  1  1 */
		
//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b11011};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 0;

		accNextState	= 6'b001101;
	end
	6'b001101: begin
		/* Precharge (Fall) */
		/*  -  -  -  -  -  -  -  -  -  - */
//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b001110;
		accNextCkCas	= DDR_CAS_RL_M1;	/* CAS Latency (-1) */
	end

	6'b001110: begin
		/* CAS Hold (Rise) */
		accNextState	= 6'b001111;
	end
	6'b001111: begin
		/* CAS Hold (Fall) */
		accNextState	= 6'b000000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001110;
//			$display("DDR: CAS Hold %d", accNextCkCas);
		end
	end


	/* Read Start */
	
	6'b010000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10  R9  R8  Hi  Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b010001;
	end
	6'b010001: begin
		/* Row Activate (Fall) */
		/* R14 R13  R7  R6  R5  R4  R3  R2  R1  R0 */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b010010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b010010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b010011;
	end
	6'b010011: begin
		/* RAS Hold (Fall) */
		accNextState	= 6'b010100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b010010;
		end
	end

	6'b010100: begin
		/* Column Read (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r Hi Lo Hi */

//		$display("DDR Read ColA %X", accColAddr);

//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrAddr[12]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 1;
		tDdrCas	= 0;	tDdrWe	= 1;

		accNextState	= 6'b010101;
	end

	6'b010101: begin
		/* Column Read (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */

//		$display("DDR Read ColB %X", accColAddr);

//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b010110;
		accNextCkCas	= DDR_CAS_RL_M1;
	end

	6'b010110: begin
		/* CAS Hold (Rise) */
		accNextState	= 6'b010111;
	end
	6'b010111: begin
		/* CAS Hold (Fall) */
		accNextState	= 6'b100000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b010110;
		end
	end
	


	/* Write Start */
	
	6'b011000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10 R9 R8 Hi Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b011001;
	end
	6'b011001: begin
		/* Row Activate (Fall) */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b011010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b011010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b011011;
	end
	6'b011011: begin
		/* RAS Hold (Fall) */
		accNextState	= 6'b011100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011010;
		end
	end

	6'b011100: begin
		/* Column Write (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r 1 0 1 */
		
//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrAddr[12]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 1;
		tDdrCas	= 0;	tDdrWe	= 0;

		accNextState	= 6'b011101;
	end
	6'b011101: begin
		/* Column Write (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */
//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b011110;
//		accNextState	= 6'b110000;
		accNextCkCas	= DDR_CAS_WL_M1;	/* CAS Latency (-1) */
	end

	6'b011110: begin
		/* CAS Hold (Rise) */
		accNextState	= 6'b011111;
	end
	6'b011111: begin
		/* CAS Hold (Fall) */
		accNextState	= 6'b110000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011110;
		end
	end
	

	
	/* Read Burst States */
	
	6'b100000: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word0 %X (Ck=%d)", ddrData, clock);
`endif
		accNextState			= 6'b100001;
		accNextReadBlk[15:0]	= ddrData;
	end
	6'b100001: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word1 %X (Ck=%d)", ddrData, clock);
`endif
		accNextState			= 6'b100010;
		accNextReadBlk[31:16]	= ddrData;
	end
	6'b100010: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word2 %X", ddrData);
`endif
		accNextState			= 6'b100011;
		accNextReadBlk[47:32]	= ddrData;
	end
	6'b100011: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word3 %X", ddrData);
`endif
		accNextState			= 6'b100100;
		accNextReadBlk[63:48]	= ddrData;
// `ifdef mod_ddr3_isddr2
`ifdef jx2_ddr_bl64b
		accNextState			= 6'b000001;
`endif
	end
	6'b100100: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word4 %X", ddrData);
`endif
		accNextState			= 6'b100101;
		accNextReadBlk[79:64]	= ddrData;
	end
	6'b100101: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word5 %X", ddrData);
`endif
		accNextState			= 6'b100110;
		accNextReadBlk[95:80]	= ddrData;
	end
	6'b100110: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word6 %X", ddrData);
`endif
		accNextState			= 6'b100111;
		accNextReadBlk[111:96]	= ddrData;
	end
	6'b100111: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word7 %X", ddrData);
`endif
		accNextState			= 6'b000001;
		accNextReadBlk[127:112]	= ddrData;
	end


	/* Write Burst States */
	
	6'b110000: begin
		accNextState			= 6'b110001;
		tDdrData 				= accReadBlk[15:0];
		tDdrOut					= 1;
	end
	6'b110001: begin
		accNextState			= 6'b110010;
		tDdrData 				= accReadBlk[31:16];
		tDdrOut					= 1;
	end
	6'b110010: begin
		accNextState			= 6'b110011;
		tDdrData 				= accReadBlk[47:32];
		tDdrOut					= 1;
	end
	6'b110011: begin
		accNextState			= 6'b110100;
		tDdrData 				= accReadBlk[63:48];
		tDdrOut					= 1;
// `ifdef mod_ddr3_isddr2
`ifdef jx2_ddr_bl64b
		accNextState			= 6'b000001;
`endif
	end
	6'b110100: begin
		accNextState			= 6'b110101;
		tDdrData 				= accReadBlk[79:64];
		tDdrOut					= 1;
	end
	6'b110101: begin
		accNextState			= 6'b110110;
		tDdrData 				= accReadBlk[95:80];
		tDdrOut					= 1;
	end
	6'b110110: begin
		accNextState			= 6'b110111;
		tDdrData 				= accReadBlk[111:96];
		tDdrOut					= 1;
	end
	6'b110111: begin
		accNextState			= 6'b000001;
		tDdrData 				= accReadBlk[127:112];
		tDdrOut					= 1;
	end

	default: begin
	end

	endcase
end

reg[7:0]	regInitSanity;

always @(posedge clock)
begin

	tMemOK2			<= tMemOK;
	tMemDataOut2	<= tMemDataOut;

	tMemDataIn		<= memDataIn;
	tMemAddr		<= memAddr;
	tMemOpm			<= memOpm;

	accState	<= accNextState;
	accCkLo		<= accNextCkLo;
	accReadBlk	<= accNextReadBlk;
	accCkCas	<= accNextCkCas;

	accAddr		<= accNextAddr;
	accRowAddr	<= accNextRowAddr;
	accColAddr	<= accNextColAddr;
	accBaAddr	<= accNextBaAddr;

	dreRowAddr	<= dreNextRowAddr;
	dreCount	<= dreNextCount;

	tDdrLastAddr	<= tDdrAddr;	//Address pins
	tDdrLastBa		<= tDdrBa;		//Bank Address pins
	tDdrLastCs		<= tDdrCs;
	tDdrLastRas		<= tDdrRas;
	tDdrLastCas		<= tDdrCas;
	tDdrLastWe		<= tDdrWe;
	tDdrLastCke		<= tDdrCke;

	if(reset || (regInitSanity!=8'h55))
	begin
		driModeOut	<= DDR_DRI_INIT;
		accState <= 0;
	end
	else
	begin
		driModeOut	<= driNextModeOut;
	end

	if(reset)
	begin
		regInitSanity	<= 0;
	end
	else
	begin
		regInitSanity	<= 8'h55;
	end

end

endmodule
