#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_TABULEIRO 5
#define METADE_TABULEIRO (TAMANHO_TABULEIRO / 2)
#define QUANTIDADE_EMBARCACOES_JACK 1
#define QUANTIDADE_EMBARCACOES_SALAZAR 5
#define EMBARCACAO_JACK 'J'
#define EMBARCACAO_SALAZAR 'S'
#define VAZIO '.'


typedef struct {
    char grade[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int embarcacoesRestantes;
} Jogador;


void inicializarEmbarcacoes(Jogador *jogador, int quantidadeEmbarcacoes, int inicioX, int fimX) {
    srand(time(NULL));

    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            jogador->grade[i][j] = VAZIO;
        }
    }

    for (int k = 0; k < quantidadeEmbarcacoes; k++) {
        int x, y;
        do {
            x = rand() % (fimX - inicioX + 1) + inicioX;
            y = rand() % TAMANHO_TABULEIRO;
        } while (jogador->grade[x][y] != VAZIO);
        jogador->grade[x][y] = (quantidadeEmbarcacoes == QUANTIDADE_EMBARCACOES_JACK) ? EMBARCACAO_JACK : EMBARCACAO_SALAZAR;
    }
}

void imprimirGrade(Jogador jogador) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%c ", jogador.grade[i][j]);
        }
        printf("\n");
    }
}

void bombardear(Jogador *atacante, Jogador *defensor, char* coordenadas) {
    char *token;
    token = strtok(coordenadas, ",");
    int x = atoi(token);
    token = strtok(NULL, ",");
    int y = atoi(token);

    if (x < 0 || x >= TAMANHO_TABULEIRO || y < 0 || y >= TAMANHO_TABULEIRO) {
        printf("Coordenadas inválidas!\n");
        return;
    }

    if (defensor->grade[x][y] == VAZIO) {
        printf("Tiro na água!\n");
    } else if (defensor->grade[x][y] == EMBARCACAO_JACK || defensor->grade[x][y] == EMBARCACAO_SALAZAR) {
        printf("Você atingiu uma embarcação inimiga!\n");
        defensor->grade[x][y] = VAZIO;
        defensor->embarcacoesRestantes--;
    } else if (defensor->grade[x][y] == atacante->grade[x][y]) {
        printf("Você destruiu seu próprio navio!\n");
        atacante->grade[x][y] = VAZIO;
        atacante->embarcacoesRestantes--;
    } else {
        printf("Você atingiu sua própria embarcação!\n");
        atacante->grade[x][y] = VAZIO;
        atacante->embarcacoesRestantes--;
    }
}

int verificarVencedor(Jogador jogador1, Jogador jogador2) {
    if (jogador1.embarcacoesRestantes == 0) {
        return 2;
    } else if (jogador2.embarcacoesRestantes == 0) {
        return 1;
    }
    return 0;
}

int main() {
    Jogador jack = {.embarcacoesRestantes = QUANTIDADE_EMBARCACOES_JACK};
    Jogador salazar = {.embarcacoesRestantes = QUANTIDADE_EMBARCACOES_SALAZAR};

    printf("A batalha naval começou!\n");

    inicializarEmbarcacoes(&jack, QUANTIDADE_EMBARCACOES_JACK, 0, METADE_TABULEIRO - 1);
    inicializarEmbarcacoes(&salazar, QUANTIDADE_EMBARCACOES_SALAZAR, METADE_TABULEIRO, TAMANHO_TABULEIRO - 1);

    int vez = 1;
    while (1) {
        printf("Vez do Jogador %d\n", vez);
        printf("Grade do Jogador %d:\n", vez == 1 ? 1 : 2);
        if (vez == 1) {
            imprimirGrade(jack);
        } else {
            imprimirGrade(salazar);
        }

        char coordenadas[10];
        printf("Digite as coordenadas (linha,coluna) para bombardear o adversário: ");
        scanf("%s", coordenadas);

        if (vez == 1) {
            bombardear(&jack, &salazar, coordenadas);
        } else {
            bombardear(&salazar, &jack, coordenadas);
        }

        int resultado = verificarVencedor(jack, salazar);
        if (resultado != 0) {
            printf("Jogador %d venceu!\n", resultado);
            break;
        }

        vez = (vez == 1) ? 2 : 1;
    }

    return 0;
}
