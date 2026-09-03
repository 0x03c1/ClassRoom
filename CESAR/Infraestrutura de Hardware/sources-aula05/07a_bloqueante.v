// Deslocador de 3 estagios escrito com  =  (bloqueante)
module bloco (input clk, input d, output reg c);
    reg a, b;
    always @(posedge clk) begin
        a = d;
        b = a;
        c = b;
    end
endmodule
