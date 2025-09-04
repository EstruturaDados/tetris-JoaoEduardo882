#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ----- Estruturas -----

typedef struct {
    int id;
    char nome;
} Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int inicio, fim, tamanho;
} FilaPecas;

typedef struct {
    Peca pilha[TAM_PILHA];
    int topo;
} PilhaReserva;

// ----- Protótipos -----

void inicializarFila(FilaPecas *f);
void inicializarPilha(PilhaReserva *p);
Peca gerarPeca();

int enfileirar(FilaPecas *f, Peca p);
int desenfileirar(FilaPecas *f, Peca *removida);
void mostrarFila(FilaPecas *f);

int empilhar(PilhaReserva *p, Peca nova);
int desempilhar(PilhaReserva *p, Peca *usada);
void mostrarPilha(PilhaReserva *p);

// ----- Função Principal -----

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n=== TETRIS STACK - NÍVEL AVENTUREIRO ===\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada;
                if (desenfileirar(&fila, &jogada)) {
                    printf("Peça jogada: [%d-%c]\n", jogada.id, jogada.nome);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 2: {
                Peca reservada;
                if (desenfileirar(&fila, &reservada)) {
                    if (empilhar(&pilha, reservada)) {
                        printf("Peça [%d-%c] reservada com sucesso!\n", reservada.id, reservada.nome);
                        enfileirar(&fila, gerarPeca());
                    } else {
                        printf("Pilha de reserva cheia! Não foi possível reservar.\n");
                        // Recoloca a peça na frente da fila
                        fila.inicio = (fila.inicio - 1 + TAM_FILA) % TAM_FILA;
                        fila.fila[fila.inicio] = reservada;
                        fila.tamanho++;
                    }
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 3: {
                Peca usada;
                if (desempilhar(&pilha, &usada)) {
                    printf("Peça reservada usada: [%d-%c]\n", usada.id, usada.nome);
                } else {
                    printf("Pilha vazia! Nenhuma peça reservada.\n");
                }
                break;
            }
            case 0:
                printf("Encerrando o jogo.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        // Mostrar estado atual
        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ----- Fila -----

void inicializarFila(FilaPecas *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;

    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(f, gerarPeca());
    }
}

int enfileirar(FilaPecas *f, Peca p) {
    if (f->tamanho == TAM_FILA) return 0;

    f->fila[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
    return 1;
}

int desenfileirar(FilaPecas *f, Peca *removida) {
    if (f->tamanho == 0) return 0;

    *removida = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return 1;
}

void mostrarFila(FilaPecas *f) {
    printf("\nFila de Peças (%d/%d): ", f->tamanho, TAM_FILA);
    if (f->tamanho == 0) {
        printf("[vazia]");
    } else {
        int i = f->inicio;
        for (int count = 0; count < f->tamanho; count++) {
            printf("[%d-%c] ", f->fila[i].id, f->fila[i].nome);
            i = (i + 1) % TAM_FILA;
        }
    }
    printf("\n");
}

// ----- Pilha -----

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

int empilhar(PilhaReserva *p, Peca nova) {
    if (p->topo == TAM_PILHA - 1) return 0;

    p->topo++;
    p->pilha[p->topo] = nova;
    return 1;
}

int desempilhar(PilhaReserva *p, Peca *usada) {
    if (p->topo == -1) return 0;

    *usada = p->pilha[p->topo];
    p->topo--;
    return 1;
}

void mostrarPilha(PilhaReserva *p) {
    printf("Pilha de Reserva (%d/%d): ", p->topo + 1, TAM_PILHA);
    if (p->topo == -1) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%d-%c] ", p->pilha[i].id, p->pilha[i].nome);
        }
    }
    printf("\n");
}

// ----- Geração de Peça -----

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    Peca p;
    p.id = rand() % 1000;
    p.nome = tipos[rand() % 7];
    return p;
}
