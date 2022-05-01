module ExBcdCpl4(vi, vo);
input[3:0]		vi;
output[3:0]		vo;
assign vo = 4'h9 - vi;
endmodule
