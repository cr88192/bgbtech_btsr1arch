/*
DMA Controller/Bus

Copies data 32 bits at a time to the MMIO bus.
Devices send simple commands to request data.

This device will serve both roles on the MMIO bus, and have a connection to main memory.

Will send data words based on a clock over a range of addresses.
Addresses will increment and wrap around if they hit their high limit.

A DmaReset command will reset the current addresses and clock to initial state.


Devices which use DMA will be daisy chained, each recieving and passing along a DMA command. It may send its own command if the incomming command is Idle.

Commands will consist of a 3-bit channel ID, with 5 bits of payload.
*/

module MemDmac(
	clock,		reset,

	memAddr,	memOpm,
	memDataIn,	memDataOut,
	memOK,

	mmioSlAddr,		mmioSlOpm,
	mmioMsAddr,		mmioMsOpm,
	mmioInData,		mmioOutData,
	mmioMsOK,		mmioSlOK,
	
	dmacCmd
	);

output[31:0]	memAddr;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
output[4:0]		memOpm;
input[1:0]		memOK;

input[31:0]		mmioInData;
output[31:0]	mmioOutData;
output[31:0]	mmioMsAddr;
output[4:0]		mmioMsOpm;
input[1:0]		mmioMsOK;
output[31:0]	mmioSlAddr;
output[4:0]		mmioSlOpm;
input[1:0]		mmioSlOK;

input[7:0]		dmacCmd;

/* Per-Channel State */
reg[25:0]		dmaLoAddrMmi[7:0];		//low address (MMIO, -2b)
reg[25:0]		dmaHiAddrMmi[7:0];		//high address (MMIO, -2b)
reg[25:0]		dmaCurAddrMmi[7:0];		//current address (MMIO, -2b)
reg[27:0]		dmaLoAddrRam[7:0];		//low address (RAM, -4b)
reg[27:0]		dmaHiAddrRam[7:0];		//high address (RAM, -4b)
reg[27:0]		dmaCurAddrRam[7:0];		//current address (RAM, -4b)
reg[15:0]		dmaCkRst[7:0];			//clock reset
reg[15:0]		dmaCkCur[7:0];			//clock current

reg[2:0]		dmaCurChan;
reg[2:0]		dmaNxtChan;

always @*
begin
	dmaNxtChan	= dmaCurChan;
end

always @(posedge clock)
begin
	dmaCurChan	<= dmaNxtChan;
end

endmodule
