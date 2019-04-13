module ExMul(
	clock, reset,
	valRs, valRt,
	valRn, idUIxt);

input	clock;
input	reset;

input[31:0]		valRs;
input[31:0]		valRt;
output[63:0]	valRn;
input[7:0]		idUIxt;

reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[31:0]	tMul1AA;
reg[31:0]	tMul1AB;
reg[31:0]	tMul1BA;
reg[31:0]	tMul1BB;
// reg[31:0]	tMul1AC;
// reg[31:0]	tMul1CA;
reg[31:0]	tMul1C;
reg[31:0]	ttMul1C;

reg[15:0]	tMul2A;
reg[15:0]	tMul2B;
reg[15:0]	tMul2C;
reg[15:0]	tMul2D;

reg[15:0]	tMul2E;
reg[15:0]	tMul2F;
reg[15:0]	tMul2G;

reg[63:0]	tMul3A;
reg[63:0]	tMul3B;
wire[63:0]	tMul3C;
ExCsAdd64F	mulAdd3(tMul3A, tMul3B, tMul3C);

always @*
begin
`ifndef def_true
	casez( { idUIxt[0], valRs[31], valRt[31] } )
		3'b000: ttMul1C = UV32_00;
		3'b001: ttMul1C = (-valRs[31:0]);
		3'b010: ttMul1C = (-valRt[31:0]);
//		3'b011: ttMul1C = (~valRs[31:0]) + (~valRt[31:0]) + 2;
//		3'b011: ttMul1C = (-valRs[31:0]) + (-valRt[31:0]);
		3'b011: ttMul1C = 32'h0002;		//HACK
		3'b1zz: ttMul1C = UV32_00;
	endcase
`endif

	if(idUIxt[0])
		ttMul1C = UV32_00;
	else
		ttMul1C =
			(valRt[31]?(-valRs[31:0]):UV32_00) +
			(valRs[31]?(-valRt[31:0]):UV32_00);

	tMul3A = { tMul2D, tMul2C, tMul2B, tMul2A  };
	tMul3B = { tMul2G, tMul2F, tMul2E, UV16_00 };
end

always @(posedge clock)
begin
	/* Stage 1 */
	tMul1AA <= { UV16_00, valRs[15: 0] } * { UV16_00, valRt[15: 0] };
	tMul1AB <= { UV16_00, valRs[15: 0] } * { UV16_00, valRt[31:16] };
	tMul1BA <= { UV16_00, valRs[31:16] } * { UV16_00, valRt[15: 0] };
	tMul1BB <= { UV16_00, valRs[31:16] } * { UV16_00, valRt[31:16] };

//	tMul1AC <= valRt[31] ? (-valRs[31:0]) : UV32_00;
//	tMul1CA <= valRs[31] ? (-valRt[31:0]) : UV32_00;
	tMul1C	<= ttMul1C;
	
	/* Stage 2 */
	tMul2A	<= tMul1AA[15:0];
	tMul2B	<= tMul1AA[31:16] + tMul1AB[15:0];
	tMul2C	<= tMul1AB[31:16] + tMul1BB[15:0];
	tMul2D	<= tMul1BB[31:16];

	tMul2E	<= tMul1BA[15: 0];
	tMul2F	<= tMul1BA[31:16] + tMul1C[15:0];
	tMul2G	<= tMul1C [31:16];
	
	/* Stage 3 */
	tValRn	<= tMul3C;
end

endmodule
