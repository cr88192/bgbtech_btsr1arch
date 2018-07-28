/*
LPDDR Interface Module

State Machine, Transmit/Recieve Words

	00-0000	//Idle

	01-0zzz	//Read Setup
	01-1zzz	//Write Setup

	10-zzzz	//Read Word
	11-zzzz	//Write Word

*/

module Jx2ModDdr(
	clock,	reset,
	memDataIn,	memDataOut,
	memAddr,	memOpm,
	memOK,
	
	ddrData,
	ddrCmd,
	ddrClk);

input			clock;
input			reset;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[31:0]		memAddr;
input[5:0]		memOpm;
output[1:0]		memOK;
	
inout[15:0]		ddrData;		//DDR data pins
output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins

reg[15:0]		tDdrData;
reg				tDdrOut;
assign			ddrData = tDdrOut ? tDdrData : 16'hzzzz;

reg[9:0]		tDdrCmd;			//Command/Address pins
reg[1:0]		tDdrClk;			//clock pins
assign			ddrCmd = tDdrCmd;
assign			ddrClk = tDdrClk;

reg[1:0]		tMemOK;
assign			memOK = tMemOK;

output[127:0]	tMemDataOut;
assign			memDataOut = tMemDataOut;

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

reg[3:0]		accCkCas;		//Clock-CAS
reg[3:0]		accNextCkCas;


always @*
begin
	accNextState	= accState;
	accNextCkLo		= !accCkLo;

	accNextReadBlk	= accReadBlk;
	tDdrOut			= 0;
	tDdrData		= 0;

	tDdrCmd			= 10'b0000000111;
	tDdrClk			= accNextCkLo ? 2'b10 : 2'b01;

	accNextCkCas	= accCkCas;

	accNextAddr		= accAddr;
	accNextRowAddr	= accRowAddr;
	accNextColAddr	= accColAddr;
	accNextBaAddr	= accBaAddr;

	tMemOK			= UMEM_OK_READY;

	/* If doing something Load/Store requests can wait. */
	if(accState!=0)
	begin
		if(memOpm[3])
			tMemOK		= UMEM_OK_HOLD;
		if(memOpm[4])
			tMemOK		= UMEM_OK_HOLD;
	end

	case(accState)
	6'b000000: begin	/* Waiting for request */

		if(memOpm[3])	/* Load Tile */
		begin
			tMemOK		= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				accNextState = 6'b010000;
				
//				accNextAddr	= {4'h0, memAddr[27:4], 4'h0};
//				accNextRowAddr = accNextAddr[24:9];
//				accNextColAddr = {7'b0, accNextAddr[ 8:0]};

				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};
			end
		end
		else
			if(memOpm[4])	/* Store Tile */
		begin
			accNextReadBlk	= memDataIn;
			tMemOK			= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				accNextState = 6'b011000;

				accNextAddr	= {4'h0, memAddr[27:4], 4'h0};
//				accNextRowAddr = accNextAddr[24:9];
//				accNextColAddr = {7'b0, accNextAddr[ 8:0]};
//				accNextBaAddr = {1'b0, accNextAddr[26:25]};

				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};
			end
		end

	end

	6'b000001: begin	/* Access Complete, Hold */
		if(memOpm[4] || memOpm[3])
		begin
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

	
	
	/* Read Start */
	
	6'b010000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10  R9  R8  Hi  Lo */
		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};
		accNextState	= 6'b010001;
	end
	6'b010001: begin
		/* Row Activate (Fall) */
		/* R14 R13  R7  R6  R5  R4  R3  R2  R1  R0 */
		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b010010;
//		accNextCkCas	= 11;			/* RAS Latency (-1) */
		accNextCkCas	= 2;			/* RAS Latency (-1) */
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
			accNextCkCas	= accCkCas - 1
			accNextState	= 6'b010010;
		end
	end

	6'b010100: begin
		/* Column Read (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r Hi Lo Hi */

		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};
		accNextState	= 6'b010101;
	end

	6'b010101: begin
		/* Column Read (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */

		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b010110;
//		accNextState	= 6'b100000;
//		accNextCkCas	= 1;			/* CAS Latency (-1) */
		accNextCkCas	= 0;			/* CAS Latency (-1) */
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
			accNextCkCas	= accCkCas - 1
			accNextState	= 6'b010110;
		end
	end
	


	/* Write Start */
	
	6'b011000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10 R9 R8 Hi Lo */
		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};
		accNextState	= 6'b011001;
	end
	6'b011001: begin
		/* Row Activate (Fall) */
		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b011010;
//		accNextCkCas	= 11;			/* RAS Latency (-1) */
		accNextCkCas	= 2;			/* RAS Latency (-1) */
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
			accNextCkCas	= accCkCas - 1
			accNextState	= 6'b011010;
		end
	end

	6'b011100: begin
		/* Column Read (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r 1 0 1 */
		
		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};
		accNextState	= 6'b011101;
	end
	6'b011101: begin
		/* Column Read (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */
		tDdrCmd			= {accColAddr[11:3], 1'b1};
//		accNextState	= 6'b011110;
		accNextState	= 6'b110000;
//		accNextCkCas	= 1;			/* CAS Latency (-1) */
		accNextCkCas	= 0;			/* CAS Latency (-1) */
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
			accNextCkCas	= accCkCas - 1
			accNextState	= 6'b010110;
		end
	end
	

	
	/* Read Burst States */
	
	6'b100000: begin
		accNextState			= 6'b100001;
		accNextReadBlk[15:0]	= ddrData;
	end
	6'b100001: begin
		accNextState			= 6'b100010;
		accNextReadBlk[31:16]	= ddrData;
	end
	6'b100010: begin
		accNextState			= 6'b100011;
		accNextReadBlk[47:32]	= ddrData;
	end
	6'b100011: begin
		accNextState			= 6'b100100;
		accNextReadBlk[63:48]	= ddrData;
	end
	6'b100100: begin
		accNextState			= 6'b100101;
		accNextReadBlk[79:64]	= ddrData;
	end
	6'b100101: begin
		accNextState			= 6'b100110;
		accNextReadBlk[95:80]	= ddrData;
	end
	6'b100110: begin
		accNextState			= 6'b100111;
		accNextReadBlk[111:96]	= ddrData;
	end
	6'b100111: begin
		accNextState			= 6'b000000;
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

	endcase
end

always @(posedge clock)
begin
	accState	<= accNextState;
	accCkLo		<= accNextCkLo;
	accReadBlk	<= accNextReadBlk;
	accCkCas	<= accNextCkCas;

	accAddr		<= accNextAddr;
	accRowAddr	<= accNextRowAddr;
	accColAddr	<= accNextColAddr;
	accBaAddr	<= accNextBaAddr;
end

endmodule
