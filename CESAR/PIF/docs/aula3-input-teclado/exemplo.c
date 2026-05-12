/*
 * Aula 3 - Input do Teclado
 * --------------------------------------------------------------
 * Um quadrado controlado pelo jogador. Use:
 *   - SETAS ou WASD para mover
 *   - ESPACO para trocar a cor
 *
 * Repare como o ESTADO (posicao, cor) vive FORA do loop e eh
 * MODIFICADO dentro dele. Esse eh o coracao do paradigma
 * imperativo: variaveis que mudam com o tempo.
 */

#include "raylib.h"

int main(void) {

    const int larguraTela = 800;
    const int alturaTela  = 600;

    InitWindow(larguraTela, alturaTela, "Aula 3 - PIF - Input do Teclado");
    SetTargetFPS(60);

    /* Estado inicial do jogador */
    int x = larguraTela / 2;
    int y = alturaTela / 2;
    int tamanho   = 50;
    int velocidade = 5;

    /* Paleta de cores para o quadrado e um indice que percorre */
    Color cores[] = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE };
    int   indiceCor = 0;
    int   totalCores = 6;

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO --------
         * IsKeyDown: enquanto a tecla esta segurada.
         * Usamos para movimento continuo.
         */
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += velocidade;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) x -= velocidade;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) y += velocidade;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) y -= velocidade;

        /* IsKeyPressed: dispara UMA VEZ por tecla pressionada.
         * Usamos para acoes discretas (pular, atirar, trocar cor).
         */
        if (IsKeyPressed(KEY_SPACE)) {
            indiceCor = (indiceCor + 1) % totalCores;
            /* '%' faz o indice voltar pro 0 quando passar do limite.
             * Esse padrao 'circular' eh muito comum em jogos. */
        }

        /* -------- DESENHO -------- */
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(x, y, tamanho, tamanho, cores[indiceCor]);
            DrawRectangleLines(x, y, tamanho, tamanho, BLACK);

            DrawText("Setas ou WASD: mover", 10, 10, 20, DARKGRAY);
            DrawText("ESPACO: trocar cor",   10, 35, 20, DARKGRAY);
            DrawText(TextFormat("Posicao: (%d, %d)", x, y),
                     10, alturaTela - 30, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
