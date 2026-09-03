`timescale 1ns/1ps
module tb;
    reg clk = 0, rst = 1, x = 0;
    wire saida;
    bloco DUT (.clk(clk), .rst(rst), .x(x), .saida(saida));
    always #10 clk = ~clk;
    integer i;
    reg [8:0] entrada = 9'b010110111;   // le da esquerda para a direita
    initial begin
        $dumpfile("fsm.vcd"); $dumpvars(0, tb);
        @(negedge clk); rst = 0;
        for (i = 8; i >= 0; i = i - 1) begin
            x = entrada[i];
            @(negedge clk);
            $display("  x=%b  ->  saida=%b", x, saida);
        end
        $finish;
    end
endmodule
