// O MESMO deslocador escrito com  <=  (nao bloqueante)
module bloco (input clk, input d, output reg c);
    reg a, b;
    always @(posedge clk) begin
        a <= d;
        b <= a;
        c <= b;
    end
endmodule
