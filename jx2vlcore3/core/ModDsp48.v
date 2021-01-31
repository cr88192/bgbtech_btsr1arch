`ifndef HAS_MODDSP48
`define HAS_MODDSP48
(* use_dsp48 = "yes" *) 
module ModDsp48( a, b, c, out );
input signed [17:0] a;
input signed [17:0] b;
input signed [47:0] c;
output signed [47:0] out;
assign out = a * b + c;
endmodule
`endif
