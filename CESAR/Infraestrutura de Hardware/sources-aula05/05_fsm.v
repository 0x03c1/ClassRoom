module bloco (input clk, input rst, input x, output saida);
    localparam S0 = 2'd0, S1 = 2'd1, S2 = 2'd2;
    reg [1:0] estado, prox;

    always @(*) begin              // logica de proximo estado: combinacional
        case (estado)
            S0: prox = x ? S1 : S0;
            S1: prox = x ? S2 : S0;
            S2: prox = x ? S2 : S0;
            default: prox = S0;
        endcase
    end

    always @(posedge clk) begin    // registrador de estado: sequencial
        if (rst) estado <= S0;
        else     estado <= prox;
    end

    assign saida = (estado == S2);
endmodule
