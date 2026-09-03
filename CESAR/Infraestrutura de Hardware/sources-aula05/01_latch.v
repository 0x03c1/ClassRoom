// O QUE ESTE CODIGO CONSTROI? Preveja antes de rodar as ferramentas.
module bloco (input en, input d, output reg q);
    always @(*) begin
        if (en)
            q = d;          // e quando en = 0? o codigo nao diz.
    end
endmodule
