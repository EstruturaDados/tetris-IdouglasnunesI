#include <stdio.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

#include <stdlib.h>
#include <time.h>

// ===============================================
// Tetris Stack — Fila Circular + Pilha com Trocas
// ===============================================

// -------------------------
// Constantes
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
// Prototipação
// -------------------------
void inicializarFila(Fila* f);
void inicializarPilha(Pilha* p);
Peca gerarPeca(Fila* f);

int enqueue(Fila* f, Peca p);
int dequeue(Fila* f, Peca* removida);
Peca* acessarFrente(Fila* f);

int push(Pilha* p, Peca x);
int pop(Pilha* p, Peca* removida);
Peca* acessarTopo(Pilha* p);

void exibirFila(const Fila* f);
void exibirPilha(const Pilha* p);
void exibirEstado(const Fila* f, const Pilha* p);

void trocarTopoComFrente(Fila* f, Pilha* p);
void trocarTres(Fila* f, Pilha* p);

void menu();

// -------------------------
// Implementações
// -------------------------
void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
    f->proximoId = 0;
}

void inicializarPilha(Pilha* p) {
    p->topo = -1;
}

// Gera uma peça aleatória
Peca gerarPeca(Fila* f) {
    const char tipos[] = {'I','O','T','L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = f->proximoId++;
    return p;
}

// Enfileirar (inserir)
int enqueue(Fila* f, Peca p) {
    if (f->tamanho == CAPACIDADE_FILA) return 0; // cheia
    f->dados[f->tras] = p;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->tamanho++;
    return 1;
}

// Desenfileirar (remover)
int dequeue(Fila* f, Peca* removida) {
    if (f->tamanho == 0) return 0; // vazia
    *removida = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// Acessa a frente sem remover
Peca* acessarFrente(Fila* f) {
    if (f->tamanho == 0) return NULL;
    return &f->dados[f->frente];
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

// Acessa topo sem remover
Peca* acessarTopo(Pilha* p) {
    if (p->topo == -1) return NULL;
    return &p->dados[p->topo];
}

// Exibir fila
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

// Exibir pilha
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

// Exibir estado completo
void exibirEstado(const Fila* f, const Pilha* p) {
    printf("\n=== Estado Atual ===\n");
    exibirFila(f);
    exibirPilha(p);
    printf("====================\n");
}

// Troca a frente da fila com o topo da pilha
void trocarTopoComFrente(Fila* f, Pilha* p) {
    Peca* frente = acessarFrente(f);
    Peca* topo = acessarTopo(p);
    if (frente && topo) {
        Peca temp = *frente;
        *frente = *topo;
        *topo = temp;
        printf("Troca realizada entre frente da fila e topo da pilha.\n");
    } else {
        printf("Nao foi possivel realizar a troca (faltam pecas).\n");
    }
}

// Troca múltipla (3 da fila com 3 da pilha)
void trocarTres(Fila* f, Pilha* p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("Nao ha pecas suficientes para a troca multipla.\n");
        return;
    }
    // Índices da fila (3 primeiros)
    int idx = f->frente;
    for (int i = 0; i < 3; i++) {
        Peca temp = f->dados[(idx + i) % CAPACIDADE_FILA];
        f->dados[(idx + i) % CAPACIDADE_FILA] = p->dados[p->topo - i];
        p->dados[p->topo - i] = temp;
    }
    printf("Troca multipla realizada entre 3 pecas da fila e 3 da pilha.\n");
}

// Menu
void menu() {
    printf("\nOpcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
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

    // Preencher fila inicial
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila, gerarPeca(&fila));
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        menu();
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peça
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                printf("Jogou a peca [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca(&fila));
            }
        } else if (opcao == 2) {
            // Reservar peça
            Peca frente;
            if (dequeue(&fila, &frente)) {
                if (push(&pilha, frente)) {
                    printf("Reservou a peca [%c %d]\n", frente.nome, frente.id);
                } else {
                    printf("Pilha cheia! Peca descartada [%c %d]\n", frente.nome, frente.id);
                }
                enqueue(&fila, gerarPeca(&fila));
            }
        } else if (opcao == 3) {
            // Usar peça da pilha
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("Usou a peca [%c %d]\n", usada.nome, usada.id);
                enqueue(&fila, gerarPeca(&fila));
            } else {
                printf("Nao ha pecas na pilha!\n");
            }
        } else if (opcao == 4) {
            trocarTopoComFrente(&fila, &pilha);
        } else if (opcao == 5) {
            trocarTres(&fila, &pilha);
        } else if (opcao == 0) {
            printf("Encerrando...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}



