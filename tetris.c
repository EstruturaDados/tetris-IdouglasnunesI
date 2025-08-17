#include <stdio.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.


#include <stdlib.h>
#include <time.h>

// -------------------------
// Constantes de configuração
// -------------------------
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// -------------------------
// Estrutura da peça
// -------------------------
typedef struct {
    char nome;  // tipo da peça ('I','O','T','L')
    int id;     // identificador único
} Peca;

// -------------------------
// Estrutura da fila circular
// -------------------------
typedef struct {
    Peca dados[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
    int proximoId;
} Fila;

// -------------------------
// Estrutura da pilha
// -------------------------
typedef struct {
    Peca dados[CAPACIDADE_PILHA];
    int topo;
} Pilha;

// -------------------------
// Prototipação de funções
// -------------------------
void inicializarFila(Fila* f);
void inicializarPilha(Pilha* p);
Peca gerarPeca(Fila* f);

int enqueue(Fila* f, Peca p);
int dequeue(Fila* f, Peca* removida);
void exibirFila(const Fila* f);

int push(Pilha* p, Peca x);
int pop(Pilha* p, Peca* removida);
void exibirPilha(const Pilha* p);

void exibirEstado(const Fila* f, const Pilha* p);
void menu();

// -------------------------
// Implementações
// -------------------------

// Inicializa fila circular
void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
    f->proximoId = 0;
}

// Inicializa pilha
void inicializarPilha(Pilha* p) {
    p->topo = -1;
}

// Gera peça nova
Peca gerarPeca(Fila* f) {
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = f->proximoId++;
    return p;
}

// Insere na fila (enqueue)
int enqueue(Fila* f, Peca p) {
    if (f->tamanho == CAPACIDADE_FILA) return 0; // cheia
    f->dados[f->tras] = p;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->tamanho++;
    return 1;
}

// Remove da fila (dequeue)
int dequeue(Fila* f, Peca* removida) {
    if (f->tamanho == 0) return 0; // vazia
    *removida = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// Exibe fila
void exibirFila(const Fila* f) {
    printf("Fila de pecas:\t");
    if (f->tamanho == 0) {
        printf("(vazia)");
    } else {
        int idx = f->frente;
        for (int i = 0; i < f->tamanho; i++) {
            printf("[%c %d] ", f->dados[idx].nome, f->dados[idx].id);
            idx = (idx + 1) % CAPACIDADE_FILA;
        }
    }
    printf("\n");
}

// Push na pilha
int push(Pilha* p, Peca x) {
    if (p->topo == CAPACIDADE_PILHA - 1) return 0; // cheia
    p->dados[++p->topo] = x;
    return 1;
}

// Pop da pilha
int pop(Pilha* p, Peca* removida) {
    if (p->topo == -1) return 0; // vazia
    *removida = p->dados[p->topo--];
    return 1;
}

// Exibe pilha
void exibirPilha(const Pilha* p) {
    printf("Pilha de reserva (Topo -> Base):\t");
    if (p->topo == -1) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->dados[i].nome, p->dados[i].id);
        }
    }
    printf("\n");
}

// Exibe estado atual
void exibirEstado(const Fila* f, const Pilha* p) {
    printf("\n=== Estado Atual ===\n");
    exibirFila(f);
    exibirPilha(p);
    printf("====================\n");
}

// Menu
void menu() {
    printf("\nOpcoes de acao:\n");
    printf("1 - Jogar peca\n");
    printf("2 - Reservar peca\n");
    printf("3 - Usar peca reservada\n");
    printf("0 - Sair\n");
    printf("> ");
}

// -------------------------
// Função principal
// -------------------------
int main() {
    srand((unsigned)time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila, gerarPeca(&fila));
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        menu();
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peça (dequeue)
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                printf("Jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca(&fila)); // repõe no fim
            } else {
                printf("Fila vazia!\n");
            }
        } else if (opcao == 2) {
            // Reservar peça (fila -> pilha)
            Peca frente;
            if (dequeue(&fila, &frente)) {
                if (push(&pilha, frente)) {
                    printf("Reservou a peca [%c %d]\n", frente.nome, frente.id);
                } else {
                    printf("Pilha cheia! Peca descartada [%c %d]\n", frente.nome, frente.id);
                }
                enqueue(&fila, gerarPeca(&fila));
            } else {
                printf("Fila vazia!\n");
            }
        } else if (opcao == 3) {
            // Usar peça reservada (pop da pilha)
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("Usou a peca reservada [%c %d]\n", usada.nome, usada.id);
                enqueue(&fila, gerarPeca(&fila));
            } else {
                printf("Nao ha pecas reservadas!\n");
            }
        } else if (opcao == 0) {
            printf("Encerrando...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}


