module Jx2FpuRcpA(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;

assign	regValFRn = tRegValFRn;

reg			tSgnB1;
reg[10:0]	tExpB1;
reg[51:0]	tFraB1;
reg[51:0]	tFraB1B;
reg[51:0]	tFraB1C;

reg[51:0]	tFraB1_Adj1;
reg[51:0]	tFraB1_Adj2;
reg[51:0]	tFraB1_Adj2A;
reg[51:0]	tFraB1_Adj2B;

always @*
begin
	tSgnB1	= regValFRm[63];
	tExpB1	= 2045-regValFRm[62:52];
	tFraB1	= ~regValFRm[51:0];

/*	
	case(regValFRm[51:48])
		4'h0: tFraB1_Adj1=52'h0000000000000;
		4'h1: tFraB1_Adj1=52'h0E1E1E1E1E1E2;
		4'h2: tFraB1_Adj1=52'h18E38E38E38E4;
		4'h3: tFraB1_Adj1=52'h20D79435E50D8;
		4'h4: tFraB1_Adj1=52'h2666666666666;
		4'h5: tFraB1_Adj1=52'h29E79E79E79E8;
		4'h6: tFraB1_Adj1=52'h2BA2E8BA2E8BA;
		4'h7: tFraB1_Adj1=52'h2BD37A6F4DE9C;
		4'h8: tFraB1_Adj1=52'h2AAAAAAAAAAAB;
		4'h9: tFraB1_Adj1=52'h2851EB851EB85;
		4'hA: tFraB1_Adj1=52'h24EC4EC4EC4EC;
		4'hB: tFraB1_Adj1=52'h2097B425ED098;
		4'hC: tFraB1_Adj1=52'h1B6DB6DB6DB6E;
		4'hD: tFraB1_Adj1=52'h158469EE5846A;
		4'hE: tFraB1_Adj1=52'h0EEEEEEEEEEEF;
		4'hF: tFraB1_Adj1=52'h07BDEF7BDEF7C;
	endcase
*/

	case(regValFRm[51:48])
		4'h0: tFraB1_Adj1=52'h0000000000000;
		4'h1: tFraB1_Adj1=52'h0E1E1E1E20000;
		4'h2: tFraB1_Adj1=52'h18E38E38E0000;
		4'h3: tFraB1_Adj1=52'h20D79435E0000;
		4'h4: tFraB1_Adj1=52'h2666666660000;
		4'h5: tFraB1_Adj1=52'h29E79E79E0000;
		4'h6: tFraB1_Adj1=52'h2BA2E8BA30000;
		4'h7: tFraB1_Adj1=52'h2BD37A6F50000;
		4'h8: tFraB1_Adj1=52'h2AAAAAAAB0000;
		4'h9: tFraB1_Adj1=52'h2851EB8520000;
		4'hA: tFraB1_Adj1=52'h24EC4EC4F0000;
		4'hB: tFraB1_Adj1=52'h2097B425F0000;
		4'hC: tFraB1_Adj1=52'h1B6DB6DB70000;
		4'hD: tFraB1_Adj1=52'h158469EE60000;
		4'hE: tFraB1_Adj1=52'h0EEEEEEEF0000;
		4'hF: tFraB1_Adj1=52'h07BDEF7BE0000;
	endcase

/*	
	case(regValFRm[47:44])
		4'h0: tFraB1_Adj2=52'h0000000000000;
		4'h1: tFraB1_Adj2=52'h000B7CC6CA77F;
		4'h2: tFraB1_Adj2=52'h001560745FAC2;
		4'h3: tFraB1_Adj2=52'h001DAED3B54C0;
		4'h4: tFraB1_Adj2=52'h00246BA363B9E;
		4'h5: tFraB1_Adj2=52'h00299A95D9B39;
		4'h6: tFraB1_Adj2=52'h002D3F518EF29;
		4'h7: tFraB1_Adj2=52'h002F5D7135CAA;
		4'h8: tFraB1_Adj2=52'h002FF883EBCBE;
		4'h9: tFraB1_Adj2=52'h002F140D696F4;
		4'hA: tFraB1_Adj2=52'h002CB38630D26;
		4'hB: tFraB1_Adj2=52'h0028DA5BBB891;
		4'hC: tFraB1_Adj2=52'h00238BF0A7898;
		4'hD: tFraB1_Adj2=52'h001CCB9CE3385;
		4'hE: tFraB1_Adj2=52'h00149CADD89A2;
		4'hF: tFraB1_Adj2=52'h000B026697AF9;
	endcase
*/

/*
	case(regValFRm[47:44])
		4'h0: tFraB1_Adj2=52'h0000000000000;
		4'h1: tFraB1_Adj2=52'h005AE4ED31204;
		4'h2: tFraB1_Adj2=52'h00B501A1BECDB;
		4'h3: tFraB1_Adj2=52'h010E58745AB7B;
		4'h4: tFraB1_Adj2=52'h0166EBB2D1AF8;
		4'h5: tFraB1_Adj2=52'h01BEBDA234EBA;
		4'h6: tFraB1_Adj2=52'h0215D07F0268A;
		4'h7: tFraB1_Adj2=52'h026C267D4C6C8;
		4'h8: tFraB1_Adj2=52'h02C1C1C8E032B;
		4'h9: tFraB1_Adj2=52'h0316A4856BC56;
		4'hA: tFraB1_Adj2=52'h036AD0CEA309C;
		4'hB: tFraB1_Adj2=52'h03BE48B864046;
		4'hC: tFraB1_Adj2=52'h04110E4EDA59D;
		4'hD: tFraB1_Adj2=52'h04632396A2115;
		4'hE: tFraB1_Adj2=52'h04B48A8CE99D3;
		4'hF: tFraB1_Adj2=52'h05054527932E2;
	endcase
*/

// /*
	case(regValFRm[47:44])
		4'h0: tFraB1_Adj2=52'h0000000000000;
		4'h1: tFraB1_Adj2=52'h005AE4ED30000;
		4'h2: tFraB1_Adj2=52'h00B501A1C0000;
		4'h3: tFraB1_Adj2=52'h010E587460000;
		4'h4: tFraB1_Adj2=52'h0166EBB2D0000;
		4'h5: tFraB1_Adj2=52'h01BEBDA230000;
		4'h6: tFraB1_Adj2=52'h0215D07F00000;
		4'h7: tFraB1_Adj2=52'h026C267D50000;
		4'h8: tFraB1_Adj2=52'h02C1C1C8E0000;
		4'h9: tFraB1_Adj2=52'h0316A48570000;
		4'hA: tFraB1_Adj2=52'h036AD0CEA0000;
		4'hB: tFraB1_Adj2=52'h03BE48B860000;
		4'hC: tFraB1_Adj2=52'h04110E4EE0000;
		4'hD: tFraB1_Adj2=52'h04632396A0000;
		4'hE: tFraB1_Adj2=52'h04B48A8CF0000;
		4'hF: tFraB1_Adj2=52'h0505452790000;
	endcase
// */

	case(regValFRm[47:44])
		4'h0: tFraB1_Adj2A=52'h00B37D6730000;
		4'h1: tFraB1_Adj2A=52'h0118ADACB0000;
		4'h2: tFraB1_Adj2A=52'h017B7FB3E0000;
		4'h3: tFraB1_Adj2A=52'h01DBF993B0000;
		4'h4: tFraB1_Adj2A=52'h023A214E00000;
		4'h5: tFraB1_Adj2A=52'h0295FCD020000;
		4'h6: tFraB1_Adj2A=52'h02EF91F2C0000;
		4'h7: tFraB1_Adj2A=52'h0346E67AB0000;
		4'h8: tFraB1_Adj2A=52'h039C001920000;
		4'h9: tFraB1_Adj2A=52'h03EEE46BD0000;
		4'hA: tFraB1_Adj2A=52'h043F98FD70000;
		4'hB: tFraB1_Adj2A=52'h048E234620000;
		4'hC: tFraB1_Adj2A=52'h04DA88AB60000;
		4'hD: tFraB1_Adj2A=52'h0524CE80C0000;
		4'hE: tFraB1_Adj2A=52'h056CFA0810000;
		4'hF: tFraB1_Adj2A=52'h05B3107180000;
	endcase

	case(regValFRm[47:44])
		4'h0: tFraB1_Adj2B=52'hFFA799DFC0000;
		4'h1: tFraB1_Adj2B=52'hFFF76AA530000;
		4'h2: tFraB1_Adj2B=52'h00480B90A0000;
		4'h3: tFraB1_Adj2B=52'h00997B2F80000;
		4'h4: tFraB1_Adj2B=52'h00EBB812D0000;
		4'h5: tFraB1_Adj2B=52'h013EC0CEE0000;
		4'h6: tFraB1_Adj2B=52'h019293FB60000;
		4'h7: tFraB1_Adj2B=52'h01E7303350000;
		4'h8: tFraB1_Adj2B=52'h023C941520000;
		4'h9: tFraB1_Adj2B=52'h0292BE4240000;
		4'hA: tFraB1_Adj2B=52'h02E9AD5F90000;
		4'hB: tFraB1_Adj2B=52'h0341601510000;
		4'hC: tFraB1_Adj2B=52'h0399D50DD0000;
		4'hD: tFraB1_Adj2B=52'h03F30AF810000;
		4'hE: tFraB1_Adj2B=52'h044D008530000;
		4'hF: tFraB1_Adj2B=52'h04A7B46960000;
	endcase

	
//	tFraB1B=tFraB1[51:0]-tFraB1_Adj1;
//	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2;

//	if(regValFRm[51])
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1+tFraB1_Adj2;
//	else
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2;

//	if(regValFRm[51])
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1+tFraB1_Adj2B;
//	else
//		tFraB1B=tFraB1[51:0]-tFraB1_Adj1-tFraB1_Adj2A;

/*
	case(regValFRm[51:49])
		3'b000:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-(tFraB1_Adj2A<<1);
		3'b001:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-(tFraB1_Adj2A>>1);
		3'b010:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-(tFraB1_Adj2A>>1);
		3'b011:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1-(tFraB1_Adj2A   );
		3'b100:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1+(tFraB1_Adj2B   );
		3'b101:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1+(tFraB1_Adj2B>>1);
		3'b110:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1+(tFraB1_Adj2B>>1);
		3'b111:	tFraB1B=tFraB1[51:0]-tFraB1_Adj1+(tFraB1_Adj2B<<1);
	endcase
*/

	tFraB1B=tFraB1[51:0]-tFraB1_Adj1;
	case(regValFRm[51:48])
		4'h0:	tFraB1C=tFraB1B-(tFraB1_Adj2<<1)-(tFraB1_Adj2>>1);
		4'h1:	tFraB1C=tFraB1B-(tFraB1_Adj2<<1);
		4'h2:	tFraB1C=tFraB1B-(tFraB1_Adj2   )-(tFraB1_Adj2>>1);
		4'h3:	tFraB1C=tFraB1B-(tFraB1_Adj2   );
		4'h4:	tFraB1C=tFraB1B-(tFraB1_Adj2>>1)-(tFraB1_Adj2>>2);
		4'h5:	tFraB1C=tFraB1B-(tFraB1_Adj2>>2)-(tFraB1_Adj2>>3);
		4'h6:	tFraB1C=tFraB1B-(tFraB1_Adj2>>3);
		4'h7:	tFraB1C=tFraB1B+(tFraB1_Adj2>>3);
		4'h8:	tFraB1C=tFraB1B+(tFraB1_Adj2>>2)+(tFraB1_Adj2>>3);
		4'h9:	tFraB1C=tFraB1B+(tFraB1_Adj2>>1);
		4'hA:	tFraB1C=tFraB1B+(tFraB1_Adj2>>1)+(tFraB1_Adj2>>2);
		4'hB:	tFraB1C=tFraB1B+(tFraB1_Adj2   );
		4'hC:	tFraB1C=tFraB1B+(tFraB1_Adj2   );
		4'hD:	tFraB1C=tFraB1B+(tFraB1_Adj2   )+(tFraB1_Adj2>>3);
		4'hE:	tFraB1C=tFraB1B+(tFraB1_Adj2   )+(tFraB1_Adj2>>2);
		4'hF:	tFraB1C=tFraB1B+(tFraB1_Adj2   )+(tFraB1_Adj2>>1);
	endcase

//	tRegValFRn	= { tSgnB1, tExpB1, tFraB1[51:0] };
//	tRegValFRn	= { tSgnB1, tExpB1, tFraB1B };
	tRegValFRn	= { tSgnB1, tExpB1, tFraB1C };
end

endmodule
