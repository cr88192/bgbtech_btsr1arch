/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

module ExModKrrShufE(
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

assign		valOut = tValShuf4;

always @*
begin
	tValShuf0 = valIn;
	if(shuf[15])
		tValShuf0 = {
			valIn[31:16], valIn[47:32],
			valIn[63:48], valIn[15: 0] };
//		tValShuf0 = ~valIn;

	tValShuf1 = tValShuf0;
	if(shuf[0])
		tValShuf1[ 7: 0] = { tValShuf0[ 3: 0], tValShuf0[ 7: 4] };
	if(shuf[1])
		tValShuf1[15: 8] = { tValShuf0[11: 8], tValShuf0[15:12] };
	if(shuf[2])
		tValShuf1[23:16] = { tValShuf0[19:16], tValShuf0[23:20] };
	if(shuf[3])
		tValShuf1[31:24] = { tValShuf0[27:24], tValShuf0[31:28] };
	if(shuf[4])
		tValShuf1[39:32] = { tValShuf0[35:32], tValShuf0[39:36] };
	if(shuf[5])
		tValShuf1[47:40] = { tValShuf0[43:40], tValShuf0[47:44] };
	if(shuf[6])
		tValShuf1[55:48] = { tValShuf0[51:48], tValShuf0[55:52] };
	if(shuf[7])
		tValShuf1[63:56] = { tValShuf0[59:56], tValShuf0[63:60] };

	tValShuf2 = tValShuf1;
	if(shuf[8])
		tValShuf2[15: 0] = { tValShuf1[ 7: 0], tValShuf1[15: 8] };
	if(shuf[9])
		tValShuf2[31:16] = { tValShuf1[23:16], tValShuf1[31:24] };
	if(shuf[10])
		tValShuf2[47:32] = { tValShuf1[39:32], tValShuf1[47:40] };
	if(shuf[11])
		tValShuf2[63:48] = { tValShuf1[55:48], tValShuf1[63:56] };

	tValShuf3 = tValShuf2;
	if(shuf[12])
		tValShuf3[31: 0] = { tValShuf2[15: 0], tValShuf2[31:16] };
	if(shuf[13])
		tValShuf3[63:32] = { tValShuf2[47:32], tValShuf2[63:48] };

	tValShuf4 = tValShuf3;
	if(shuf[14])
		tValShuf4 = { tValShuf3[31: 0], tValShuf3[63:32] };

end

endmodule
