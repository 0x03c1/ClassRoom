module bloco (input clk, input en, input [3:0] d, output reg [3:0] q);
    always @(posedge clk) begin
        if (en)
            q <= d;         // mesmo if incompleto do arquivo 01. muda alguma coisa?
    end
endmodule
