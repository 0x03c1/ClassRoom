/*
 * Aula 1 - Primeira Janela
 * --------------------------------------------------------------
 * Este eh o esqueleto mais simples possivel de um programa
 * Raylib. Todo jogo que vamos construir comeca exatamente assim.
 *
 * Leia os comentarios na ordem e tente entender cada bloco
 * ANTES de compilar.
 */

#include "raylib.h"

int main(void) {

    /* -------- 1. INICIALIZACAO --------
     * Aqui dizemos a Raylib: "crie uma janela com esse tamanho e
     * esse titulo". O SetTargetFPS limita o jogo a rodar a 60
     * quadros por segundo (60 voltas do loop por segundo).
     */
    const int larguraTela = 800;
    const int alturaTela  = 600;

    InitWindow(larguraTela, alturaTela, "Aula 1 - PIF - Primeira Janela");
    SetTargetFPS(60);


    /* -------- 2. LOOP PRINCIPAL --------
     * O coracao do jogo. Enquanto o usuario nao fechar a janela
     * (clicando no X ou apertando ESC), repetimos:
     *   - desenhar tudo
     * Em proximas aulas, vamos tambem ATUALIZAR estado aqui dentro.
     */
    while (!WindowShouldClose()) {

        BeginDrawing();                      /* pegamos o pincel */

            ClearBackground(BLACK);          /* pinta o fundo de preto */

            DrawText("Ola, PIF!", 280, 280, 40, GREEN);
            DrawText("Pressione ESC para sair", 250, 340, 20, GRAY);

        EndDrawing();                        /* mostra o quadro pronto */
    }


    /* -------- 3. FINALIZACAO --------
     * Sempre que abrimos algo (janela, arquivo, conexao), temos
     * que fechar. Em C isso eh responsabilidade SUA, nao do
     * sistema. Se esquecer, da vazamento de recursos.
     */
    CloseWindow();

    return 0;
}
