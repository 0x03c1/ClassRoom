/*
 * Aula 2 - Formas e Cores
 * --------------------------------------------------------------
 * Desenhamos um rosto simples para praticar varias funcoes de
 * desenho da Raylib. Repare como cada forma e posicionada e
 * como a ORDEM dos comandos define o que fica em cima.
 */

#include "raylib.h"

int main(void) {

    InitWindow(800, 600, "Aula 2 - PIF - Formas e Cores");
    SetTargetFPS(60);

    /* Criando nossas proprias cores */
    Color corPele  = { 250, 220, 180, 255 };  /* tom pessego */
    Color corFundo = {  30,  30,  46, 255 };  /* azul-noite */

    while (!WindowShouldClose()) {

        BeginDrawing();

            ClearBackground(corFundo);

            /* --- CABECA ---
             * Circulo preenchido + circulo so com borda em cima
             * (para dar contorno).
             */
            DrawCircle(400, 280, 120, corPele);
            DrawCircleLines(400, 280, 120, BLACK);

            /* --- OLHOS ---
             * Cada olho eh um circulo branco com um circulo preto
             * (a pupila) por cima. A ORDEM importa: se desenhassemos
             * a pupila antes do branco, ela ficaria escondida.
             */
            DrawCircle(355, 250, 18, WHITE);
            DrawCircle(355, 250,  8, BLACK);

            DrawCircle(445, 250, 18, WHITE);
            DrawCircle(445, 250,  8, BLACK);

            /* --- NARIZ ---
             * Um triangulo definido por tres vertices (Vector2).
             * IMPORTANTE: a Raylib espera os vertices em ordem
             * ANTI-HORARIA, senao o triangulo pode nao aparecer.
             */
            DrawTriangle(
                (Vector2){ 400, 270 },   /* topo */
                (Vector2){ 380, 310 },   /* base esquerda */
                (Vector2){ 420, 310 },   /* base direita */
                MAROON
            );

            /* --- BOCA ---
             * Linha simples por enquanto. No exercicio voce
             * vai trocar por um arco/sorriso.
             */
            DrawLine(360, 350, 440, 350, BLACK);

            /* --- HUD: textos informativos ---
             * Repare que GetScreenWidth retorna a largura atual,
             * util quando a janela pode ser redimensionada.
             */
            DrawText("Aula 2 - Formas e Cores", 10, 10, 22, RAYWHITE);
            DrawText("Tente adicionar orelhas, cabelo e sorriso!",
                     10, GetScreenHeight() - 30, 18, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
