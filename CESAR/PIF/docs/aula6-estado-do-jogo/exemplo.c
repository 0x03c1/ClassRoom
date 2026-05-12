/*
 * Aula 6 - Estado do Jogo
 * --------------------------------------------------------------
 * Tres telas: MENU, JOGANDO, GAME_OVER. Use ENTER para avancar.
 * Apos 10 segundos jogando, vai para game over.
 *
 * Esse padrao com enum + switch eh a base de organizacao de
 * qualquer jogo serio. Voce vai reutilizar isso a vida toda.
 */

#include "raylib.h"

typedef enum {
    TELA_MENU,
    TELA_JOGANDO,
    TELA_GAMEOVER
} EstadoJogo;

int main(void) {

    const int larguraTela = 800;
    const int alturaTela  = 600;

    InitWindow(larguraTela, alturaTela, "Aula 6 - PIF - Estado do Jogo");
    SetTargetFPS(60);

    EstadoJogo estado = TELA_MENU;

    /* Estado interno do gameplay */
    Vector2 pos = { larguraTela / 2.0f, alturaTela / 2.0f };
    Vector2 vel = { 5.0f, 4.0f };
    float   raio = 20.0f;
    double  tempoInicio = 0.0;
    double  duracaoPartida = 10.0; /* segundos */

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO --------
         * Aqui o switch lida com o estado atual. Cada case eh
         * praticamente um "mini-loop" para aquela tela.
         */
        switch (estado) {

            case TELA_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    /* Resetando o estado de jogo ao iniciar */
                    pos.x = larguraTela / 2.0f;
                    pos.y = alturaTela / 2.0f;
                    vel.x = 5.0f;
                    vel.y = 4.0f;
                    tempoInicio = GetTime();
                    estado = TELA_JOGANDO;
                }
                break;

            case TELA_JOGANDO:
                /* Movimento + rebote (mesmo da aula 4) */
                pos.x += vel.x;
                pos.y += vel.y;
                if (pos.x - raio <= 0 || pos.x + raio >= larguraTela) vel.x = -vel.x;
                if (pos.y - raio <= 0 || pos.y + raio >= alturaTela) vel.y = -vel.y;

                /* Apos a duracao, vai para game over */
                if (GetTime() - tempoInicio >= duracaoPartida) {
                    estado = TELA_GAMEOVER;
                }
                break;

            case TELA_GAMEOVER:
                if (IsKeyPressed(KEY_ENTER)) {
                    estado = TELA_MENU;
                }
                break;
        }

        /* -------- DESENHO --------
         * Outro switch desenha cada tela.
         * Manter atualizacao e desenho separados eh boa pratica:
         * fica facil debugar e modificar depois.
         */
        BeginDrawing();
            ClearBackground(BLACK);

            switch (estado) {

                case TELA_MENU:
                    DrawText("PIF - Demonstracao de Estado",
                             140, 200, 32, RAYWHITE);
                    DrawText("Pressione ENTER para jogar",
                             200, 320, 22, GREEN);
                    DrawText("ESC para sair",
                             310, 360, 18, GRAY);
                    break;

                case TELA_JOGANDO: {
                    double tempoRestante = duracaoPartida - (GetTime() - tempoInicio);
                    if (tempoRestante < 0) tempoRestante = 0;

                    DrawCircleV(pos, raio, SKYBLUE);
                    DrawText(TextFormat("Tempo: %.1f", tempoRestante),
                             10, 10, 24, YELLOW);
                    break;
                }

                case TELA_GAMEOVER:
                    DrawText("FIM DE JOGO",
                             220, 220, 50, RED);
                    DrawText("Pressione ENTER para voltar ao menu",
                             140, 320, 22, RAYWHITE);
                    break;
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
