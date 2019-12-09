/*
Interface with 16-bit DBI LCD panels.

Will allow two panels to be connected, with most of the pins being shared.
A separate CSX signal will be directed to each LCD.
The CSX signal will select which LCD is being accessed.


Registers
	E060: Status(R) / Control(W) Register
	E064: Read(R) / Write(W) Data

Status Register
    Bit 0: Data is Ready
	Bit 1: Busy / Operation in Progress

Data sent to the Write Data port will contain data and control bits:
	(23:0): Data Value
	(24): Data/Command
	(27:25): Output Select
		000: Broadcast (Both LCDs)
		001: First LCD
		010: Second LCD
		011: Third LCD (Reserved)
		10z: Reserved
		110: Reserved
		111: Special Command
	(31:28): Reserved (MBZ)

Special Commands
  1: Assert reset signal for 20us.

 */

`include "CoreDefs.v"

module ModLcdDbi16(
	/* verilator lint_off UNUSED */
	clock,			reset,

	dbi_data_i,		dbi_data_o,		dbi_data_d,

	dbi_resx_o,		dbi_dcx_o,
	dbi_wrx_o,		dbi_rdx_o,

	dbi_csx1_o,		dbi_csx2_o,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;
input			reset;

output			dbi_dcx_o;
output			dbi_wrx_o;
output			dbi_resx_o;
output			dbi_rdx_o;

output			dbi_csx1_o;
output			dbi_csx2_o;

input[15:0]		dbi_data_i;
output[15:0]	dbi_data_o;
output			dbi_data_d;

input[31:0]		mmioInData;
output[31:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

reg[31:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[31:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

assign		mmioOutData		= tMmioOutData2;
assign		mmioOK			= tMmioOK2;


reg				t_dbi_dcx_o;
reg				t_dbi_wrx_o;
reg				t_dbi_resx_o;
reg				t_dbi_rdx_o;
reg				t_dbi_csx1_o;
reg				t_dbi_csx2_o;
reg[15:0]		t_dbi_data_o;
reg				t_dbi_data_d;

reg				t_dbi_dcx_o2;
reg				t_dbi_wrx_o2;
reg				t_dbi_resx_o2;
reg				t_dbi_rdx_o2;
reg				t_dbi_csx1_o2;
reg				t_dbi_csx2_o2;
reg[15:0]		t_dbi_data_o2;
reg				t_dbi_data_d2;

assign		dbi_dcx_o	= t_dbi_dcx_o2;
assign		dbi_wrx_o	= t_dbi_wrx_o2;
assign		dbi_resx_o	= t_dbi_resx_o2;
assign		dbi_rdx_o	= t_dbi_rdx_o2;
assign		dbi_csx1_o	= t_dbi_csx1_o2;
assign		dbi_csx2_o	= t_dbi_csx2_o2;
assign		dbi_data_o	= t_dbi_data_o2;
assign		dbi_data_d	= t_dbi_data_d2;


reg[31:0]		tMmioAddr;
reg[31:0]		tMmioInData;
reg[4:0]		tMmioOpm;


wire		tMmioLowCSel;
assign		tMmioLowCSel	= (tMmioAddr[27:16]==12'h000);

wire		tMmioCSel;
assign		tMmioCSel = tMmioLowCSel && (tMmioAddr[15:4]==12'hE06);

reg				mmioInOE;
reg				mmioInWR;

reg				tMmioIsHold;

reg[31:0]		tRegCtrl;
reg[31:0]		tNextRegCtrl;

reg[31:0]		tNextWriteValue;
reg				tNextDoWrite;
reg				tNextDoReset;

reg				tDoWriteIsLcd0;
reg				tDoWriteIsLcd1;
reg				tDoWriteIsLcd2;
reg				tDoWriteIsSpecial;

reg[11:0]		tMmioLatchCyc;
reg[11:0]		tMmioNextLatchCyc;
reg				tMmioLatch;
//	reg		tMmioNextLatchCyc;


always @*
begin
	tMmioOutData	= UV32_XX;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE			= (tMmioOpm[3]) && tMmioCSel;
	mmioInWR			= (tMmioOpm[4]) && tMmioCSel;

//	t_dbi_dcx_o			= 1;
//	t_dbi_wrx_o			= 1;
//	t_dbi_resx_o		= 1;
//	t_dbi_rdx_o			= 1;
//	t_dbi_csx1_o		= 1;
//	t_dbi_csx2_o		= 1;

//	t_dbi_data_o		= 0;
//	t_dbi_data_d		= 0;

	tNextWriteValue		= 0;
	tNextDoWrite		= 0;
	tNextDoReset		= 0;
	tDoWriteIsLcd0		= 0;
	tDoWriteIsLcd1		= 0;
	tDoWriteIsLcd2		= 0;
	tDoWriteIsSpecial	= 0;
	
	tNextRegCtrl		= tRegCtrl;
	
	tMmioNextLatchCyc	= (tMmioLatchCyc!=0) ? (tMmioLatchCyc-1) : 0;

	tMmioIsHold			= tMmioLatch || (tMmioLatchCyc!=0);

	if((tMmioAddr[3:2]==2'b00) && mmioInOE)
	begin
		tMmioOutData		= tRegCtrl;
		tMmioOutData[7:0]	= 0;
		tMmioOutData[0]		= 0;
		tMmioOutData[1]		= tMmioIsHold;
		tMmioOK				= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b00) && mmioInWR)
	begin
		tNextRegCtrl	= tMmioInData;
		tMmioOK			= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b01) && mmioInOE)
	begin
		tMmioOutData	= UV32_00;
		tMmioOK			= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b01) && mmioInWR)
	begin
		if(tMmioIsHold)
		begin
			tMmioOK				= UMEM_OK_HOLD;
		end
		else
		begin
			tNextWriteValue		= tMmioInData;
			tNextDoWrite		= 1;
			tMmioNextLatchCyc	= 3;
			tMmioOK				= UMEM_OK_OK;

			tDoWriteIsLcd0		= (tMmioInData[27:25]==3'b000);
			tDoWriteIsLcd1		= (tMmioInData[27:25]==3'b001);
			tDoWriteIsLcd2		= (tMmioInData[27:25]==3'b010);
			tDoWriteIsSpecial	= (tMmioInData[27:25]==3'b111);
			
			if(tDoWriteIsSpecial)
			begin
				case(tMmioInData[5:0])
					6'h01: begin
						tNextDoReset		= 1;
						tMmioNextLatchCyc	= 2047;
					end
				endcase
			end
		end
	end

end	

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;

	tMmioAddr		<= mmioAddr;
	tMmioInData		<= mmioInData;
	tMmioOpm		<= mmioOpm;

	t_dbi_dcx_o2	<= t_dbi_dcx_o;
	t_dbi_wrx_o2	<= t_dbi_wrx_o;
	t_dbi_resx_o2	<= t_dbi_resx_o;
	t_dbi_rdx_o2	<= t_dbi_rdx_o;
	t_dbi_csx1_o2	<= t_dbi_csx1_o;
	t_dbi_csx2_o2	<= t_dbi_csx2_o;
	t_dbi_data_o2	<= t_dbi_data_o;
	t_dbi_data_d2	<= t_dbi_data_d;

	tRegCtrl		<= tNextRegCtrl;

	tMmioLatchCyc	<= tMmioNextLatchCyc;

	if(tNextDoWrite)
	begin
		t_dbi_dcx_o			<= !tNextWriteValue[24];
		t_dbi_wrx_o			<= 0;
		t_dbi_resx_o		<= !tNextDoReset;
		t_dbi_rdx_o			<= 1;
		t_dbi_csx1_o		<= !(tDoWriteIsLcd1 || tDoWriteIsLcd0);
		t_dbi_csx2_o		<= !(tDoWriteIsLcd2 || tDoWriteIsLcd0);

		if(tNextDoReset)
		begin
//			t_dbi_data_o		<= tNextWriteValue[15:0];
			t_dbi_data_d		<= 0;
			tMmioLatch			<= 1;
		end
		else
		begin
			t_dbi_data_o		<= tNextWriteValue[15:0];
			t_dbi_data_d		<= 1;
			tMmioLatch			<= 1;
		end
	end
	else
		if(tMmioLatchCyc!=0)
	begin
		/* Hold Output State */
		t_dbi_dcx_o			<= t_dbi_dcx_o;
		t_dbi_wrx_o			<= t_dbi_wrx_o;
		t_dbi_resx_o		<= t_dbi_resx_o;
		t_dbi_rdx_o			<= t_dbi_rdx_o;
		t_dbi_csx1_o		<= t_dbi_csx1_o;
		t_dbi_csx2_o		<= t_dbi_csx2_o;
		t_dbi_data_o		<= t_dbi_data_o;
		t_dbi_data_d		<= t_dbi_data_d;
	end
	else
	begin
		/* Idle State */
//		t_dbi_dcx_o			<= 1;
		t_dbi_wrx_o			<= 1;
		t_dbi_resx_o		<= 1;
		t_dbi_rdx_o			<= 1;
		t_dbi_csx1_o		<= 1;
		t_dbi_csx2_o		<= 1;

		t_dbi_data_o		<= t_dbi_data_o;
//		t_dbi_data_d		<= t_dbi_data_d;
		
		if(tMmioLatch)
		begin
			/* Not Idle Yet, return to Idle State */
			tMmioLatch		<= 0;
			tMmioLatchCyc	<= 3;

			t_dbi_dcx_o			<= t_dbi_dcx_o;
			t_dbi_data_d		<= t_dbi_data_d;
		end
		else
		begin
			/* True Idle */
			t_dbi_dcx_o			<= 1;
			t_dbi_data_d		<= 0;
		end
	end

end

endmodule
