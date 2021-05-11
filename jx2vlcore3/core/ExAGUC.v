/*
Add With Scale

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [5:4]=Type (B/W/L/Q)
  [  3]=?
  [  2]=ZExt (0=SX, 1=ZX)
  [1:0]=Scale (B/W/L/Q)

 */

module ExAGUC(
	regValRm,
	regValRi,
	idUIxt,
	regOutAddr,
	addrEnJq);

input[47:0]		regValRm;
input[47:0]		regValRi;
input[7:0]		idUIxt;
input			addrEnJq;

output[47:0]	regOutAddr;

reg[47:0]	tRegValRi;
reg[47:0]	tRiSc0;
reg[47:0]	tRiSc1;
reg[47:0]	tRiSc2;
reg[47:0]	tRiSc3;
reg[47:0]	tRiSc;

reg[47:0]	tAddrSc0;
// reg[47:0]	tAddrSc1;
// reg[47:0]	tAddrSc2;
// reg[47:0]	tAddrSc3;

reg[16:0]	tAddrSc0A;
// reg[16:0]	tAddrSc1A;
// reg[16:0]	tAddrSc2A;
// reg[16:0]	tAddrSc3A;

reg[16:0]	tAddrSc0B;
// reg[16:0]	tAddrSc1B;
// reg[16:0]	tAddrSc2B;
// reg[16:0]	tAddrSc3B;

reg[16:0]	tAddrSc0B0;
// reg[16:0]	tAddrSc1B0;
// reg[16:0]	tAddrSc2B0;
// reg[16:0]	tAddrSc3B0;
reg[16:0]	tAddrSc0B1;
// reg[16:0]	tAddrSc1B1;
// reg[16:0]	tAddrSc2B1;
// reg[16:0]	tAddrSc3B1;

reg[16:0]	tAddrSc0C0;
// reg[16:0]	tAddrSc1C0;
// reg[16:0]	tAddrSc2C0;
// reg[16:0]	tAddrSc3C0;
reg[16:0]	tAddrSc0C1;
// reg[16:0]	tAddrSc1C1;
// reg[16:0]	tAddrSc2C1;
// reg[16:0]	tAddrSc3C1;

reg[15:0]	tAddrSc0C;
// reg[15:0]	tAddrSc1C;
// reg[15:0]	tAddrSc2C;
// reg[15:0]	tAddrSc3C;

reg			tCaVal0A;
reg			tCaVal0B;
reg			tCaVal0C;

reg[47:0]	tAddr;

assign		regOutAddr = tAddr;

always @*
begin
//	tRegValRi = regValRi;
//	tRegValRi = { regValRi[31] ? 16'hFFFF : 16'h0000, regValRi[31:0] };
	tRegValRi = { regValRi[32] ? 15'h7FFF : 15'h0000, regValRi[32:0] };

	tRiSc0 = tRegValRi;
	tRiSc1 = { tRegValRi[46:0], 1'b0 };
	tRiSc2 = { tRegValRi[45:0], 2'b0 };
	tRiSc3 = { tRegValRi[44:0], 3'b0 };

	case(idUIxt[1:0])
		2'b00:	tRiSc=tRiSc0;
		2'b01:	tRiSc=tRiSc1;
		2'b10:	tRiSc=tRiSc2;
		2'b11:	tRiSc=tRiSc3;
	endcase

	tAddrSc0A  = { 1'b0, regValRm[15: 0] } + { 1'b0, tRiSc[15: 0] };
	tAddrSc0B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc[31:16] } + 0;
	tAddrSc0B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc[31:16] } + 1;
	tAddrSc0C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc[47:32] } + 0;
	tAddrSc0C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc[47:32] } + 1;
	tAddrSc0B = tAddrSc0A[16] ? tAddrSc0B1 : tAddrSc0B0;

	tCaVal0A = 0;
	tCaVal0B = tAddrSc0A[16];
	tCaVal0C = tCaVal0B ? tAddrSc0B1[16] : tAddrSc0B0[16];

	tAddrSc0C = 16'h0000;
	if(addrEnJq)
	begin
//		tAddrSc0C = tAddrSc0B[16] ? tAddrSc0C1[15:0] : tAddrSc0C0[15:0];
		tAddrSc0C = tCaVal0C ? tAddrSc0C1[15:0] : tAddrSc0C0[15:0];
	end

	tAddrSc0 = { tAddrSc0C[15:0], tAddrSc0B[15:0], tAddrSc0A[15:0] };
	tAddr = tAddrSc0;
end

endmodule
