module ExOpClz(
	clock,		reset,
	idUCmd,		idUIxt,
	valRs,		valRn
	);

input	clock;
input	reset;

input[63:0]		valRs;
output[7:0]		valRn;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
// input			exHold;

reg[7:0]	tValRn;
assign 	valRn = tValRn;

reg[5:0]	tClzMsk;

reg[31:0]	tValShl5;
reg[15:0]	tValShl4;
reg[ 7:0]	tValShl3;
reg[ 3:0]	tValShl2;
reg[ 1:0]	tValShl1;
reg			tValShl0;

always @*
begin
	tClzMsk = 0;
	
	if(!idUIxt[0])
	begin
		tValShl5	= valRs[31:0];
		tClzMsk[5]	= 0;
	end else
//		if(valRs[63:32]==0)
		if(valRs[63:31]==0)
	begin
		tValShl5	= valRs[31:0];
		tClzMsk[5]	= 1;
	end else begin
		tValShl5	= valRs[63:32];
	end

//	if(tValShl5[31:16]==0)
	if(tValShl5[31:15]==0)
	begin
		tValShl4	= tValShl5[15:0];
		tClzMsk[4]	= 1;
	end else begin
		tValShl4	= tValShl5[31:16];
	end
	
//	if(tValShl4[15:8]==0)
	if(tValShl4[15:7]==0)
	begin
		tValShl3	= tValShl4[7:0];
		tClzMsk[3]	= 1;
	end else begin
		tValShl3	= tValShl4[15:8];
	end
	
//	if(tValShl3[7:4]==0)
	if(tValShl3[7:3]==0)
	begin
		tValShl2	= tValShl3[3:0];
		tClzMsk[2]	= 1;
	end else begin
		tValShl2	= tValShl3[7:4];
	end
	
//	if(tValShl2[3:2]==0)
	if(tValShl2[3:1]==0)
	begin
		tValShl1	= tValShl3[1:0];
		tClzMsk[1]	= 1;
	end else begin
		tValShl1	= tValShl3[3:2];
	end

//	if(tValShl1[1]==0)
	if(tValShl1[1:0]==0)
	begin
		tValShl0	= tValShl3[0];
		tClzMsk[0]	= 1;
	end else begin
		tValShl0	= tValShl3[1];
	end

	tValRn = { 2'b00, tClzMsk } + { UV7_00, (tValShl0==0) };
	
end

endmodule