/*
 * Aula 7 - Mouse e Grid
 * --------------------------------------------------------------
 * Grade 3x3. A celula sob o mouse fica destacada. Ao clicar
 * com o botao esquerdo, ela "acende". Com o direito, "apaga".
 *
 * Este eh literalmente o esqueleto do jogo da velha. Na proxima
 * aula vamos so trocar "acender" por "colocar X ou O".
 */

#include "raylib.h"

#define LINHAS 3
#define COLUNAS 3

int main(void) {

    const int tamCelula = 180;
    const int larguraTela = tamCelula * COLUNAS;
    const int alturaTela  = tamCelula * LINHAS;

    InitWindow(larguraTela, alturaTela, "Aula 7 - PIF - Mouse e Grid");
    SetTargetFPS(60);

    /* Estado: 0 = vazio, 1 = preenchido */
    int grid[LINHAS][COLUNAS] = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    int totalCliques = 0;

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO --------
         * Conversao pixel -> grid.
         * Importante: divisao INTEIRA em C ja descarta o resto,
         * entao 350 / 180 = 1 (e nao 1.94).
         */
        Vector2 mouse = GetMousePosition();
        int colunaMouse = (int)mouse.x / tamCelula;
        int linhaMouse  = (int)mouse.y / tamCelula;

        /* Sempre valide os indices antes de acessar a matriz!
         * Se o mouse sair da janela (pode acontecer com janelas
         * pequenas ou mouse warp), podemos ter valores invalidos.
         */
        bool mouseValido = (colunaMouse >= 0 && colunaMouse < COLUNAS &&
                            linhaMouse  >= 0 && linhaMouse  < LINHAS);

        if (mouseValido) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                grid[linhaMouse][colunaMouse] = 1;
                totalCliques++;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                grid[linhaMouse][colunaMouse] = 0;
            }
        }

        /* -------- DESENHO -------- */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            /* 1) Desenha as celulas preenchidas + hover */
            for (int l = 0; l < LINHAS; l++) {
                for (int c = 0; c < COLUNAS; c++) {

                    int px = c * tamCelula;
                    int py = l * tamCelula;

                    if (grid[l][c] == 1) {
                        DrawRectangle(px, py, tamCelula, tamCelula, SKYBLUE);
                    }

                    /* Destaque do hover (so na celula sob o mouse) */
                    if (mouseValido && l == linhaMouse && c == colunaMouse) {
                        /* Fade deixa a cor semi-transparente */
                        DrawRectangle(px, py, tamCelula, tamCelula,
                                      Fade(YELLOW, 0.3f));
                    }
                }
            }

            /* 2) Desenha as linhas do grid POR CIMA das celulas */
            for (int i = 1; i < COLUNAS; i++) {
                DrawLine(i * tamCelula, 0,
                         i * tamCelula, alturaTela, BLACK);
            }
            for (int i = 1; i < LINHAS; i++) {
                DrawLine(0, i * tamCelula,
                         larguraTela, i * tamCelula, BLACK);
            }

            /* 3) HUD */
            DrawText(TextFormat("Cliques: %d", totalCliques),
                     10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
