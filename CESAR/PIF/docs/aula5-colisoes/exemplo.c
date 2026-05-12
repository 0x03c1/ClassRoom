/*
 * Aula 5 - Colisoes
 * --------------------------------------------------------------
 * Mini-Pong: raquete embaixo, bola caindo. Quando a bola toca
 * a raquete -> +1 ponto. Quando passa direto -> -1 vida.
 *
 * Foco: uso de CheckCollisionCircleRec para detectar contato
 * entre uma bola (circulo) e uma raquete (retangulo).
 */

#include "raylib.h"

int main(void) {

    const int larguraTela = 800;
    const int alturaTela  = 600;

    InitWindow(larguraTela, alturaTela, "Aula 5 - PIF - Colisoes");
    SetTargetFPS(60);

    /* --- Raquete --- */
    Rectangle raquete = { larguraTela/2.0f - 60, alturaTela - 40, 120, 15 };
    float velRaquete  = 7.0f;

    /* --- Bola --- */
    Vector2 bola = { larguraTela/2.0f, 100 };
    Vector2 vel  = { 4.0f, 4.0f };
    float   raio = 12.0f;

    /* --- Score e vidas --- */
    int score = 0;
    int vidas = 3;

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO --------
         * Movimento da raquete (so horizontal)
         */
        if ((IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) && raquete.x > 0)
            raquete.x -= velRaquete;
        if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            && raquete.x + raquete.width < larguraTela)
            raquete.x += velRaquete;

        /* Movimento da bola */
        bola.x += vel.x;
        bola.y += vel.y;

        /* Colisao com as paredes laterais e topo (rebote simples) */
        if (bola.x - raio <= 0 || bola.x + raio >= larguraTela) vel.x = -vel.x;
        if (bola.y - raio <= 0) vel.y = -vel.y;

        /* >>>>>> COLISAO COM A RAQUETE <<<<<<
         * A funcao CheckCollisionCircleRec faz toda a matematica
         * para nos. Recebe: centro do circulo, raio, retangulo.
         */
        if (CheckCollisionCircleRec(bola, raio, raquete) && vel.y > 0) {
            vel.y = -vel.y;            /* rebote */
            bola.y = raquete.y - raio; /* tira a bola de dentro da raquete */
            score++;
        }

        /* A bola passou da raquete e caiu pela parte de baixo */
        if (bola.y - raio > alturaTela) {
            vidas--;
            /* Reposiciona a bola no topo */
            bola.x = larguraTela / 2.0f;
            bola.y = 100;
            vel.x = 4.0f;
            vel.y = 4.0f;
        }

        /* -------- DESENHO -------- */
        BeginDrawing();

            ClearBackground(BLACK);

            DrawCircleV(bola, raio, RAYWHITE);
            DrawRectangleRec(raquete, SKYBLUE);

            DrawText(TextFormat("Score: %d", score), 10, 10, 24, GREEN);
            DrawText(TextFormat("Vidas: %d", vidas),
                     larguraTela - 120, 10, 24, RED);

            if (vidas <= 0) {
                DrawText("GAME OVER",
                         larguraTela/2 - 130, alturaTela/2 - 30, 50, RED);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
