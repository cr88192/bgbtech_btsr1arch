module ExModKrrShufD(
	valIn,
	valOut,
	shuf
	);

input[63:0]		valIn;
output[63:0]	valOut;
input[15:0]		shuf;

reg[63:0]		tValShuf0;
reg[63:0]		tValShuf1;
reg[63:0]		tValShuf2;
reg[63:0]		tValShuf3;
reg[63:0]		tValShuf4;
reg[63:0]		tValShuf5;

assign		valOut = tValShuf5;

always @*
begin
	tValShuf0 = valIn;
//	if(shuf[15])
//		tValShuf0 = ~valIn;

	tValShuf1 = tValShuf0;
	if(shuf[14])
		tValShuf1 = { tValShuf0[31: 0], tValShuf0[63:32] };

	tValShuf2 = tValShuf1;
	if(shuf[12])
		tValShuf2[31: 0] = { tValShuf1[15: 0], tValShuf1[31:16] };
	if(shuf[13])
		tValShuf2[63:32] = { tValShuf1[47:32], tValShuf1[63:48] };

	tValShuf3 = tValShuf2;
	if(shuf[8])
		tValShuf3[15: 0] = { tValShuf2[ 7: 0], tValShuf2[15: 8] };
	if(shuf[9])
		tValShuf3[31:16] = { tValShuf2[23:16], tValShuf2[31:24] };
	if(shuf[10])
		tValShuf3[47:32] = { tValShuf2[39:32], tValShuf2[47:40] };
	if(shuf[11])
		tValShuf3[63:48] = { tValShuf2[55:48], tValShuf2[63:56] };

	tValShuf4 = tValShuf3;
	if(shuf[0])
		tValShuf4[ 7: 0] = { tValShuf3[ 3: 0], tValShuf3[ 7: 4] };
	if(shuf[1])
		tValShuf4[15: 8] = { tValShuf3[11: 8], tValShuf3[15:12] };
	if(shuf[2])
		tValShuf4[23:16] = { tValShuf3[19:16], tValShuf3[23:20] };
	if(shuf[3])
		tValShuf4[31:24] = { tValShuf3[27:24], tValShuf3[31:28] };
	if(shuf[4])
		tValShuf4[39:32] = { tValShuf3[35:32], tValShuf3[39:36] };
	if(shuf[5])
		tValShuf4[47:40] = { tValShuf3[43:40], tValShuf3[47:44] };
	if(shuf[6])
		tValShuf4[55:48] = { tValShuf3[51:48], tValShuf3[55:52] };
	if(shuf[7])
		tValShuf4[63:56] = { tValShuf3[59:56], tValShuf3[63:60] };

	tValShuf5 = tValShuf4;
	if(shuf[15])
		tValShuf5 = {
			tValShuf4[31:16], tValShuf4[47:32],
			tValShuf4[63:48], tValShuf4[15: 0] };

end

endmodule
