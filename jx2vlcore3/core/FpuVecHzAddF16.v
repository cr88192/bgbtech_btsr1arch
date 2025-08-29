module FpuVecHzAddF16(
	clock,
	regValA, regValAccI, regValAccO);

input			clock;
input [63:0]	regValA;
input [15:0]	regValAccI;
output[15:0]	regValAccO;

wire[15:0]		regValAx;
wire[15:0]		regValAy;
wire[15:0]		regValAz;
wire[15:0]		regValAw;

assign	regValAx = regValA[15: 0];
assign	regValAy = regValA[31:16];
assign	regValAz = regValA[47:32];
assign	regValAw = regValA[63:48];

reg[4:0]		tValExpI;
reg[4:0]		tValExpVx;
reg[4:0]		tValExpVy;
reg[4:0]		tValExpVz;
reg[4:0]		tValExpVw;
reg[4:0]		tValExpVm;
reg[4:0]		tValExpSb;
reg[1:0]		tValExpSi;

always @*
begin
	tValExpI  = regValAccI[14:10];
	tValExpVx = regValAx[14:10];
	tValExpVy = regValAy[14:10];
	tValExpVz = regValAz[14:10];
	tValExpVw = regValAw[14:10];
	
	tValExpSb[0]=(tValExpVx<tValExpVy) ;
	tValExpSb[1]=(tValExpVx<tValExpVz) ;
	tValExpSb[2]=(tValExpVx<tValExpVw) ;
	tValExpSb[3]=(tValExpVz<tValExpVw) ;
	tValExpSb[4]=(tValExpVy<tValExpVz) | (tValExpVy<tValExpVw);
	
	case(tValExpSb)
		5'h00:  tValExpSi=0;
		5'h01:  tValExpSi=1;
		5'h03:  tValExpSi=1;
		5'h07:  tValExpSi=1;
		5'h08:  tValExpSi=0;
		5'h09:  tValExpSi=1;
		5'h0D:  tValExpSi=1;
		5'h0F:  tValExpSi=1;
		5'h10:  tValExpSi=0;
		5'h12:  tValExpSi=2;
		5'h13:  tValExpSi=2;
		5'h16:  tValExpSi=2;
		5'h17:  tValExpSi=2;
		5'h18:  tValExpSi=0;
		5'h1C:  tValExpSi=3;
		5'h1D:  tValExpSi=3;
		5'h1E:  tValExpSi=3;
		5'h1F:  tValExpSi=3;
		default: tValExpSi=0;
	endcase
	
	case(tValExpSi)
		2'b00: tValExpVm=tValExpVx;
		2'b01: tValExpVm=tValExpVy;
		2'b10: tValExpVm=tValExpVz;
		2'b11: tValExpVm=tValExpVw;
	endcase
	
	
end

endmodule
