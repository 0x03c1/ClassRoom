module bloco (input clk, input rst, input en, output reg [3:0] q);
    always @(posedge clk) begin
        if (rst)      q <= 4'd0;
        else if (en)  q <= q + 1'b1;
    end
endmodule
