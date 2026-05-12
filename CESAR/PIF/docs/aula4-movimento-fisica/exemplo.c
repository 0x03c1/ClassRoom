/*
 * Aula 4 - Movimento e Fisica Simples
 * --------------------------------------------------------------
 * Uma bola quica livremente na tela. A regra eh:
 *     nova_posicao = posicao + velocidade
 * Quando bate na borda, invertemos o sinal da velocidade no
 * eixo correspondente. E so. Isso eh literalmente a fisica
 * do Pong, jogo de 1972.
 */

#include "raylib.h"

int main(void) {

    const int larguraTela = 800;
    const int alturaTela  = 600;

    InitWindow(larguraTela, alturaTela, "Aula 4 - PIF - Movimento e Fisica");
    SetTargetFPS(60);

    /* Estado da bola: posicao e velocidade como Vector2 (float) */
    Vector2 pos = { larguraTela / 2.0f, alturaTela / 2.0f };
    Vector2 vel = { 5.0f, 4.0f };
    float   raio = 20.0f;
    Color   corBola = SKYBLUE;

    /* Paleta para trocar a cor a cada rebote */
    Color paleta[] = { SKYBLUE, RED, GREEN, YELLOW, PURPLE, ORANGE, PINK };
    int   tamPaleta = 7;
    int   indiceCor = 0;
    int   rebotes   = 0;

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO --------
         * Aplicamos a regra fundamental do movimento.
         */
        pos.x += vel.x;
        pos.y += vel.y;

        /* Rebote nas bordas laterais.
         * Note que verificamos pos.x - raio (borda esquerda da bola)
         * e pos.x + raio (borda direita da bola), pois pos eh o CENTRO.
         */
        if (pos.x - raio <= 0 || pos.x + raio >= larguraTela) {
            vel.x = -vel.x;
            rebotes++;
            indiceCor = (indiceCor + 1) % tamPaleta;
            corBola = paleta[indiceCor];
        }

        /* Rebote no topo e no chao */
        if (pos.y - raio <= 0 || pos.y + raio >= alturaTela) {
            vel.y = -vel.y;
            rebotes++;
            indiceCor = (indiceCor + 1) % tamPaleta;
            corBola = paleta[indiceCor];
        }

        /* -------- DESENHO -------- */
        BeginDrawing();

            ClearBackground(BLACK);

            DrawCircleV(pos, raio, corBola);

            DrawText("Aula 4 - Movimento livre", 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("Rebotes: %d", rebotes),
                     10, 40, 20, LIME);
            DrawText(TextFormat("Velocidade: (%.1f, %.1f)", vel.x, vel.y),
                     10, alturaTela - 30, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
