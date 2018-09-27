module MixTest0(clock);

input clock;

reg[15:0] count;

always @(posedge clock)
begin
	count <= count & 1'b1;
//	count <= 1'b1;
end

endmodule
