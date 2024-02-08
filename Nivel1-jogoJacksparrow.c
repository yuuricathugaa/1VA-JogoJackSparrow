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

// Função para inicializar o cenário
void inicializarCidade(Pirata piratas[], Obstaculo obstaculos[], Posicao *barco, Jogador *jack) {
    // Inicializar a semente para geração de números aleatórios
    srand(time(NULL));

    // Inicializar piratas
    for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
        piratas[i].posicao.x = rand() % TAMANHO_CIDADE;
        piratas[i].posicao.y = rand() % TAMANHO_CIDADE;
        piratas[i].estaVivo = true;
    }

    // Inicializar obstáculos
    for (int i = 0; i < QUANTIDADE_OBSTACULOS; i++) {
        obstaculos[i].posicao.x = rand() % TAMANHO_CIDADE;
        obstaculos[i].posicao.y = rand() % TAMANHO_CIDADE;
        obstaculos[i].estaDisponivel = true;
    }

    // Inicializar o barco
    barco->x = 18;
    barco->y = 18;

    // Inicializar Jack na posição (0, 0)
    jack->posicao.x = 0;
    jack->posicao.y = 0;
    jack->temArma = false;
    jack->balas = 0;
}

// Função para mover Jack
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
            printf("Movimento inválido!\n");
            return;
    }

    // Verificar se a nova posição está dentro dos limites da cidade
    if (novaX < 0 || novaX >= TAMANHO_CIDADE || novaY < 0 || novaY >= TAMANHO_CIDADE) {
        printf("Movimento inválido! Fora dos limites da cidade.\n");
        return;
    }

    // Verificar se há um obstáculo na nova posição
    for (int i = 0; i < QUANTIDADE_OBSTACULOS; i++) {
        if (novaX == obstaculos[i].posicao.x && novaY == obstaculos[i].posicao.y && obstaculos[i].estaDisponivel) {
            printf("Você foi bloqueado por um obstáculo, tente outro movimento!\n");
            return;
        }
    }

    // Atualizar a posição de Jack
    jack->posicao.x = novaX;
    jack->posicao.y = novaY;
}

// Função para pegar a arma e as balas
void pegarArmaEBalas(Jogador *jack, Obstaculo obstaculos[], Posicao *arma, int balas[]) {
    // Verificar se Jack está na mesma posição que a arma
    if (jack->posicao.x == arma->x && jack->posicao.y == arma->y && !jack->temArma) {
        jack->temArma = true;
        printf("Jack pegou uma arma!\n");
        // Atualizar a posição da arma para fora do mapa
        arma->x = -1;
        arma->y = -1;
    }

    // Verificar se Jack está na mesma posição de uma bala
    for (int i = 0; i < QUANTIDADE_BALAS; i++) {
        if (jack->posicao.x == balas[i * 2] && jack->posicao.y == balas[i * 2 + 1]) {
            jack->balas++;
            printf("Jack pegou uma bala!\n");
            // Remover a bala do cenário
            balas[i * 2] = -1;
            balas[i * 2 + 1] = -1;
        }
    }
}


// Função para atualizar o cenário quando um pirata morre
void atualizarJogo(Pirata piratas[], Posicao *barco, Jogador *jack, bool *nivelConcluido) {
    // Verificar se Jack está na mesma posição que o barco
    if (jack->posicao.x == barco->x && jack->posicao.y == barco->y) {
        // Condições para tornar a vida de Jack mais difícil ao redor do barco
        bool piratasAoRedor = false;
        for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
            if (piratas[i].estaVivo && abs(piratas[i].posicao.x - barco->x) <= 1 && abs(piratas[i].posicao.y - barco->y) <= 1) {
                piratasAoRedor = true;
                break;
            }
        }
        if (!piratasAoRedor) {
            // Se não houver piratas ao redor do barco, o nível é concluído
            printf("Você concluiu o nível 1, agora Jack zarpa em direção a Salazar que o espera para uma batalha naval!!\n");
            *nivelConcluido = true;
            return;
        }
    }

    // Verificar se Jack está na mesma posição que o barco
    if (jack->posicao.x == barco->x && jack->posicao.y == barco->y) {
        // Condições para tornar a vida de Jack mais difícil ao redor do barco
        bool piratasAoRedor = false;
        for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
            if (piratas[i].estaVivo && abs(piratas[i].posicao.x - barco->x) <= 1 && abs(piratas[i].posicao.y - barco->y) <= 1) {
                piratasAoRedor = true;
                break;
            }
        }
        if (!piratasAoRedor) {
            // Se não houver piratas ao redor do barco, o nível é concluído
            printf("Você concluiu o nível 1, agora Jack zarpa em direção a Salazar que o espera para uma batalha naval!!\n");
            *nivelConcluido = true;
            return;
        }
    }

    // Atualizar o estado dos piratas quando morrem
    for (int i = 0; i < QUANTIDADE_PIRATAS; i++) {
        if (jack->temArma && jack->posicao.x == piratas[i].posicao.x && jack->posicao.y == piratas[i].posicao.y && piratas[i].estaVivo) {
            piratas[i].estaVivo = false;
            printf("Jack derrotou um pirata!\n");
        } else if (jack->posicao.x == piratas[i].posicao.x && jack->posicao.y == piratas[i].posicao.y) {
            printf("Jack foi pego por um pirata!\n");
            exit(0); // Jack é capturado e o jogo termina
        }
    }
}


// Função para imprimir o estado atual do jogo
void imprimirCidade(Pirata piratas[], Obstaculo obstaculos[], Posicao *barco, Jogador *jack, Posicao *arma, int balas[]) {
    for (int i = 0; i < TAMANHO_CIDADE; i++) {
        for (int j = 0; j < TAMANHO_CIDADE; j++) {
            bool impresso = false;

            // Verificar se a posição atual contém um obstáculo
            for (int k = 0; k < QUANTIDADE_OBSTACULOS; k++) {
                if (i == obstaculos[k].posicao.x && j == obstaculos[k].posicao.y && obstaculos[k].estaDisponivel) {
                    printf("O ");
                    impresso = true;
                    break;
                }
            }

            // Verificar se a posição atual contém um pirata
            if (!impresso) {
                for (int k = 0; k < QUANTIDADE_PIRATAS; k++) {
                    if (i == piratas[k].posicao.x && j == piratas[k].posicao.y && piratas[k].estaVivo) {
                        printf("P ");
                        impresso = true;
                        break;
                    }
                }
            }

            // Verificar se a posição atual contém Jack
            if (!impresso && i == jack->posicao.x && j == jack->posicao.y) {
                printf("J ");
                impresso = true;
            }

            // Verificar se a posição atual contém o barco
            if (!impresso && i == barco->x && j == barco->y) {
                printf("B ");
                impresso = true;
            }

            // Verificar se a posição atual contém a arma
            if (!impresso && i == arma->x && j == arma->y) {
                printf("A ");
                impresso = true;
            }

            // Verificar se a posição atual contém uma bala
            if (!impresso) {
                for (int k = 0; k < QUANTIDADE_BALAS; k++) {
                    if (i == balas[k * 2] && j == balas[k * 2 + 1]) {
                        printf("* ");
                        impresso = true;
                        break;
                    }
                }
            }

            // Se nenhum dos elementos acima estiver presente, imprimir um espaço vazio
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

    // Inicializar o cenário
    inicializarCidade(piratas, obstaculos, &barco, &jack);

    // Inicializar a posição da arma
    arma.x = rand() % TAMANHO_CIDADE;
    arma.y = rand() % TAMANHO_CIDADE;

    // Inicializar a posição das balas
    for (int i = 0; i < QUANTIDADE_BALAS; i++) {
        balas[i * 2] = rand() % TAMANHO_CIDADE;
        balas[i * 2 + 1] = rand() % TAMANHO_CIDADE;
    }

    // Loop principal do jogo
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
        // Imprimir o estado atual do jogo
        imprimirCidade(piratas, obstaculos, &barco, &jack, &arma, balas);

        // Obter a entrada do jogador
        printf("Digite sua jogada: ");
        scanf(" %c", &movimento);

        // Mover Jack
        moverJogador(&jack, movimento, obstaculos);

        // Verificar se Jack pegou a arma ou as balas
        pegarArmaEBalas(&jack, obstaculos, &arma, balas);

        // Atualizar o cenário após a morte de piratas
        atualizarJogo(piratas, &barco, &jack, &nivelConcluido);
    }

    return 0;
}
