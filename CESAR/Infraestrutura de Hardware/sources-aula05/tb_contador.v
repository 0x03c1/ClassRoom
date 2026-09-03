`timescale 1ns/1ps
module tb;
    reg clk = 0, rst = 1, en = 1;
    wire [3:0] q;
    bloco DUT (.clk(clk), .rst(rst), .en(en), .q(q));
    always #10 clk = ~clk;
    initial begin
        $dumpfile("contador.vcd"); $dumpvars(0, tb);
        #25 rst = 0;
        repeat (18) @(posedge clk) $display("  t=%5t  en=%b  q=%2d", $time, en, q);
        en = 0;
        repeat (3)  @(posedge clk) $display("  t=%5t  en=%b  q=%2d  <- congelado", $time, en, q);
        en = 1;
        repeat (3)  @(posedge clk) $display("  t=%5t  en=%b  q=%2d", $time, en, q);
        $finish;
    end
endmodule
