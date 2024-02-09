//Aluno : Yuri Catunda F. de Sousa - 1ºVA
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define TAMANHO_CIDADE 20
#define QUANTIDADE_PIRATAS 50
#define QUANTIDADE_OBSTACULOS 50
#define QUANTIDADE_BALAS 20
#define PIRATAS_AROUND_BOAT 10

typedef struct {
    int x;
    int y;
} Posicao;

typedef struct {
    Posicao posicao;
    bool temArma;
    int balas;
} Jogador;

typedef struct {
    Posicao posicao;
    bool estaVivo;
} Pirata;

typedef struct {
    Posicao posicao;
    bool estaDisponivel;
} Obstaculo;

void inicializarCidade(Pirata piratas[], Obstaculo obstaculos[], Posicao *barco, Jogador *jack) {
    srand(time(NULL));
    for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
        do {
            piratas[i].posicao.x = rand() % TAMANHO_CIDADE;
            piratas[i].posicao.y = rand() % TAMANHO_CIDADE;
        } while (piratas[i].posicao.x == barco->x && piratas[i].posicao.y == barco->y);
        piratas[i].estaVivo = true;
    }
    
    for (int i = 0; i < QUANTIDADE_OBSTACULOS; i++) {
        do {
            obstaculos[i].posicao.x = rand() % TAMANHO_CIDADE;
            obstaculos[i].posicao.y = rand() % TAMANHO_CIDADE;
        } while ((obstaculos[i].posicao.x == barco->x && obstaculos[i].posicao.y == barco->y));
        obstaculos[i].estaDisponivel = true;
    }

    barco->x = 18;
    barco->y = 18;
    jack->posicao.x = 0;
    jack->posicao.y = 0;
    jack->temArma = false;
    jack->balas = 0;
}

void moverJogador(Jogador *jack, char direcao, Obstaculo obstaculos[]) {
    int novaX = jack->posicao.x;
    int novaY = jack->posicao.y;

    switch(direcao) {
        case 'W':
            novaX--;
            break;
        case 'S':
            novaX++;
            break;
        case 'A':
            novaY--;
            break;
        case 'D':
            novaY++;
            break;
        default:
            printf("Movimento inválido, escolha entre as opções A,W,S,D!\n");
            return;
    }

    if (novaX < 0 || novaX >= TAMANHO_CIDADE || novaY < 0 || novaY >= TAMANHO_CIDADE) {
        printf("Movimento inválido, fora dos limites da cidade!\n");
        return;
    }

    for (int i = 0; i < QUANTIDADE_OBSTACULOS; i++) {
        if (novaX == obstaculos[i].posicao.x && novaY == obstaculos[i].posicao.y && obstaculos[i].estaDisponivel) {
            printf("Você foi bloqueado por um obstáculo, tente outro movimento!\n");
            return;
        }
    }

    jack->posicao.x = novaX;
    jack->posicao.y = novaY;
}

void pegarArmaEBalas(Jogador *jack, Obstaculo obstaculos[], Posicao *arma, int balas[]) {
    if (jack->posicao.x == arma->x && jack->posicao.y == arma->y && !jack->temArma) {
        jack->temArma = true;
        printf("Jack pegou uma arma!\n");
        arma->x = -1;
        arma->y = -1;
    }

    for (int i = 0; i < QUANTIDADE_BALAS; i++) {
        if (jack->posicao.x == balas[i * 2] && jack->posicao.y == balas[i * 2 + 1]) {
            jack->balas++;
            printf("Jack pegou uma bala!\n");
            balas[i * 2] = -1;
            balas[i * 2 + 1] = -1;
        }
    }
}

void atualizarJogo(Pirata piratas[], Posicao *barco, Jogador *jack, bool *nivelConcluido) {
    if (jack->posicao.x == barco->x && jack->posicao.y == barco->y) {
        printf("Você concluiu o nível 1, agora Jack zarpa em direção a Salazar que o espera para uma batalha naval!!\n");
        *nivelConcluido = true;
        return;
    }

    for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
        if (jack->temArma && jack->posicao.x == piratas[i].posicao.x && jack->posicao.y == piratas[i].posicao.y && piratas[i].estaVivo) {
            piratas[i].estaVivo = false;
            printf("Jack derrotou um pirata!\n");
        } else if (jack->posicao.x == piratas[i].posicao.x && jack->posicao.y == piratas[i].posicao.y) {
            printf("Jack foi pego por um pirata!\n");
            exit(0);
        }
    }
}

void imprimirCidade(Pirata piratas[], Obstaculo obstaculos[], Posicao *barco, Jogador *jack, Posicao *arma, int balas[]) {
    for (int i = 0; i < TAMANHO_CIDADE; i++) {
        for (int j = 0; j < TAMANHO_CIDADE; j++) {
            bool impresso = false;
            
            for (int k = 0; k < QUANTIDADE_OBSTACULOS; k++) {
                if (i == obstaculos[k].posicao.x && j == obstaculos[k].posicao.y && obstaculos[k].estaDisponivel) {
                    printf("O ");
                    impresso = true;
                    break;
                }
            }

            if (!impresso) {
                for (int k = 0; k < QUANTIDADE_PIRATAS; k++) {
                    if (i == piratas[k].posicao.x && j == piratas[k].posicao.y && piratas[k].estaVivo) {
                        printf("P ");
                        impresso = true;
                        break;
                    }
                }
            }

            if (!impresso && i == jack->posicao.x && j == jack->posicao.y) {
                printf("J ");
                impresso = true;
            }

            if (!impresso && i == barco->x && j == barco->y) {
                printf("B ");
                impresso = true;
            }

            if (!impresso && i == arma->x && j == arma->y) {
                printf("A ");
                impresso = true;
            }

            if (!impresso) {
                for (int k = 0; k < QUANTIDADE_BALAS; k++) {
                    if (i == balas[k * 2] && j == balas[k * 2 + 1]) {
                        printf("* ");
                        impresso = true;
                        break;
                    }
                }
            }

            if (!impresso) {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int main() {
    Pirata piratas[QUANTIDADE_PIRATAS];
    Obstaculo obstaculos[QUANTIDADE_OBSTACULOS];
    Posicao barco;
    Jogador jack;
    Posicao arma;
    int balas[QUANTIDADE_BALAS * 2];
    bool nivelConcluido = false;

    inicializarCidade(piratas, obstaculos, &barco, &jack);

    arma.x = rand() % TAMANHO_CIDADE;
    arma.y = rand() % TAMANHO_CIDADE;
    
    while (arma.x == barco.x && arma.y == barco.y) {
        arma.x = rand() % TAMANHO_CIDADE;
        arma.y = rand() % TAMANHO_CIDADE;
    }

    for (int i = 0; i < QUANTIDADE_BALAS; i++) {
        do {
            balas[i * 2] = rand() % TAMANHO_CIDADE;
            balas[i * 2 + 1] = rand() % TAMANHO_CIDADE;
        } while (balas[i * 2] == barco.x && balas[i * 2 + 1] == barco.y);
    }

    char movimento;
    printf("Bem-vindo ao jogo do Jack Sparrow!\n");
    printf("Use W (cima), S (baixo), A (esquerda), D (direita) para mover Jack.\n");
    printf("O = Obstáculos;\n");
    printf("P = Piratas;\n");
    printf("J = Jack;\n");
    printf("B = Barco;\n");
    printf("A = Arma;\n");
    printf("* = Balas;\n");

    while (!nivelConcluido) {
        imprimirCidade(piratas, obstaculos, &barco, &jack, &arma, balas);
        printf("Digite sua jogada: ");
        scanf(" %c", &movimento);
        moverJogador(&jack, movimento, obstaculos);
        pegarArmaEBalas(&jack, obstaculos, &arma, balas);
        atualizarJogo(piratas, &barco, &jack, &nivelConcluido);
    }

    return 0;
}
