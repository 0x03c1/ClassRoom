// Latch e flip-flop lado a lado, no MESMO sinal.
// Gera onda.vcd para abrir no GTKWave.
`timescale 1ns/1ps

module latch_d (input en, input d, output reg q);
    always @(*) if (en) q = d;
endmodule

module flipflop_d (input clk, input d, output reg q);
    always @(posedge clk) q <= d;
endmodule

module tb;
    reg clk = 0, d = 0;
    wire q_latch, q_ff;

    latch_d    L (.en(clk), .d(d), .q(q_latch));   // o MESMO fio nos dois
    flipflop_d F (.clk(clk), .d(d), .q(q_ff));

    always #10 clk = ~clk;                         // 10 ns baixo, 10 ns alto

    initial begin
        $dumpfile("onda.vcd");
        $dumpvars(0, tb);
        $display("   tempo  clk  d | latch  flip-flop");
        $monitor("  %5t   %b   %b |   %b        %b", $time, clk, d, q_latch, q_ff);
        #5  d = 1;      // clock BAIXO: ninguem reage
        #10 d = 0;      // clock ALTO: o latch acompanha na hora
        #3  d = 1;      // clock ALTO de novo: o latch acompanha de novo
        #12 d = 0;      // clock BAIXO: so o latch congela
        #20 d = 1;
        #25 $finish;
    end
endmodule
