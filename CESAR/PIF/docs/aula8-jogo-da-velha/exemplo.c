/*
 * Aula 8 - Jogo da Velha (Projeto Final)
 * --------------------------------------------------------------
 * Junta tudo das aulas 1 a 7. Dois jogadores alternam jogadas
 * clicando em celulas vazias. Quem fechar uma linha, coluna ou
 * diagonal vence. Tabuleiro cheio sem vencedor = empate.
 *
 * Controles:
 *   - Clique esquerdo: colocar peca na celula vazia sob o mouse
 *   - R: reiniciar partida
 *   - ESC: sair
 *
 * Convencoes do tabuleiro:
 *   0 = vazio    1 = X (jogador 1)    2 = O (jogador 2)
 */

#include "raylib.h"

#define LINHAS  3
#define COLUNAS 3

typedef enum {
    JOGANDO,
    VITORIA,
    EMPATE
} EstadoJogo;

/* -------- FUNCOES PURAS (sem efeitos colaterais)  --------
 * Estas funcoes apenas LEEM o tabuleiro e retornam respostas.
 * Sao excelentes candidatas a serem reescritas em Haskell.
 */

/* Retorna 1 se X venceu, 2 se O venceu, 0 se ninguem venceu */
int verificarVitoria(int tab[LINHAS][COLUNAS]) {

    /* Linhas */
    for (int l = 0; l < LINHAS; l++) {
        if (tab[l][0] != 0 && tab[l][0] == tab[l][1] && tab[l][1] == tab[l][2])
            return tab[l][0];
    }

    /* Colunas */
    for (int c = 0; c < COLUNAS; c++) {
        if (tab[0][c] != 0 && tab[0][c] == tab[1][c] && tab[1][c] == tab[2][c])
            return tab[0][c];
    }

    /* Diagonal principal */
    if (tab[0][0] != 0 && tab[0][0] == tab[1][1] && tab[1][1] == tab[2][2])
        return tab[0][0];

    /* Diagonal secundaria */
    if (tab[0][2] != 0 && tab[0][2] == tab[1][1] && tab[1][1] == tab[2][0])
        return tab[0][2];

    return 0;
}

bool tabuleiroCheio(int tab[LINHAS][COLUNAS]) {
    for (int l = 0; l < LINHAS; l++)
        for (int c = 0; c < COLUNAS; c++)
            if (tab[l][c] == 0) return false;
    return true;
}

/* -------- FUNCOES DE DESENHO -------- */

void desenharX(int px, int py, int tamCelula) {
    int margem = tamCelula / 5;
    int espessura = 8;

    /* Diagonal '\' */
    DrawLineEx(
        (Vector2){ px + margem,           py + margem },
        (Vector2){ px + tamCelula - margem, py + tamCelula - margem },
        espessura, BLUE);

    /* Diagonal '/' */
    DrawLineEx(
        (Vector2){ px + tamCelula - margem, py + margem },
        (Vector2){ px + margem,           py + tamCelula - margem },
        espessura, BLUE);
}

void desenharO(int px, int py, int tamCelula) {
    int centroX = px + tamCelula / 2;
    int centroY = py + tamCelula / 2;
    int raioExt = tamCelula / 2 - tamCelula / 5;
    int raioInt = raioExt - 8;

    /* Aneis para simular um "O" grosso */
    DrawCircle(centroX, centroY, raioExt, MAROON);
    DrawCircle(centroX, centroY, raioInt, RAYWHITE);
}

void desenharTabuleiro(int tab[LINHAS][COLUNAS], int tamCelula,
                       int larguraTela, int alturaTela) {

    /* Fundo do tabuleiro ja foi feito pelo ClearBackground */

    /* Linhas verticais */
    for (int c = 1; c < COLUNAS; c++) {
        DrawLineEx(
            (Vector2){ c * tamCelula, 0 },
            (Vector2){ c * tamCelula, alturaTela },
            4, DARKGRAY);
    }

    /* Linhas horizontais */
    for (int l = 1; l < LINHAS; l++) {
        DrawLineEx(
            (Vector2){ 0,            l * tamCelula },
            (Vector2){ larguraTela,  l * tamCelula },
            4, DARKGRAY);
    }

    /* Pecas */
    for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            int px = c * tamCelula;
            int py = l * tamCelula;
            if (tab[l][c] == 1) desenharX(px, py, tamCelula);
            if (tab[l][c] == 2) desenharO(px, py, tamCelula);
        }
    }
}

/* -------- FUNCAO QUE TEM EFEITO COLATERAL --------
 * Modifica o tabuleiro. Em Haskell, isso seria tratado de forma
 * diferente. Aqui em C, eh natural.
 */
void reiniciarJogo(int tab[LINHAS][COLUNAS], int *jogador,
                   EstadoJogo *estado, int *vencedor) {
    for (int l = 0; l < LINHAS; l++)
        for (int c = 0; c < COLUNAS; c++)
            tab[l][c] = 0;
    *jogador = 1;
    *estado  = JOGANDO;
    *vencedor = 0;
}

/* ============================================================ */

int main(void) {

    const int tamCelula   = 200;
    const int larguraTela = tamCelula * COLUNAS;
    const int alturaTela  = tamCelula * LINHAS + 80; /* +80 para a barra de status */

    InitWindow(larguraTela, alturaTela,
               "Aula 8 - PIF - Jogo da Velha");
    SetTargetFPS(60);

    int tabuleiro[LINHAS][COLUNAS] = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };
    int jogadorAtual = 1;
    EstadoJogo estado = JOGANDO;
    int vencedor = 0;

    while (!WindowShouldClose()) {

        /* -------- ATUALIZACAO -------- */
        if (estado == JOGANDO) {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 m = GetMousePosition();

                /* So consideramos cliques na area do tabuleiro */
                if (m.y < LINHAS * tamCelula) {
                    int col = (int)m.x / tamCelula;
                    int lin = (int)m.y / tamCelula;

                    /* Validacao + celula precisa estar vazia */
                    if (lin >= 0 && lin < LINHAS &&
                        col >= 0 && col < COLUNAS &&
                        tabuleiro[lin][col] == 0) {

                        tabuleiro[lin][col] = jogadorAtual;

                        /* Apos a jogada, checar fim de jogo */
                        vencedor = verificarVitoria(tabuleiro);
                        if (vencedor != 0) {
                            estado = VITORIA;
                        } else if (tabuleiroCheio(tabuleiro)) {
                            estado = EMPATE;
                        } else {
                            /* Alterna jogador (1 <-> 2) */
                            jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
                        }
                    }
                }
            }
        }

        /* R reinicia em qualquer estado */
        if (IsKeyPressed(KEY_R)) {
            reiniciarJogo(tabuleiro, &jogadorAtual, &estado, &vencedor);
        }

        /* -------- DESENHO -------- */
        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharTabuleiro(tabuleiro, tamCelula, larguraTela, alturaTela);

            /* Barra de status na parte de baixo */
            int yBarra = LINHAS * tamCelula;
            DrawRectangle(0, yBarra, larguraTela, 80, BLACK);

            const char *msg = "";
            Color corMsg = WHITE;

            switch (estado) {
                case JOGANDO:
                    msg = (jogadorAtual == 1)
                        ? "Vez de X (clique para jogar)"
                        : "Vez de O (clique para jogar)";
                    corMsg = (jogadorAtual == 1) ? SKYBLUE : ORANGE;
                    break;

                case VITORIA:
                    msg = (vencedor == 1)
                        ? "X venceu! Pressione R para jogar de novo"
                        : "O venceu! Pressione R para jogar de novo";
                    corMsg = GREEN;
                    break;

                case EMPATE:
                    msg = "Empate! Pressione R para jogar de novo";
                    corMsg = YELLOW;
                    break;
            }

            DrawText(msg, 20, yBarra + 28, 22, corMsg);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
