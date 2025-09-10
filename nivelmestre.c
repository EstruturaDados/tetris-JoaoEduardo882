#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ----- Structs -----

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

typedef struct {
    FilaPecas fila;
    PilhaReserva pilha;
} EstadoJogo;

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

void trocarTopoFilaPilha(FilaPecas *f, PilhaReserva *p);
void inverterFilaComPilha(FilaPecas *f, PilhaReserva *p);
void copiarEstado(EstadoJogo *dest, EstadoJogo *src);

// ----- Função Principal -----

int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    EstadoJogo atual, anterior;

    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    copiarEstado(&anterior, &(EstadoJogo){fila, pilha});
    copiarEstado(&atual, &anterior);

    do {
        printf("\n=== TETRIS STACK - NÍVEL MESTRE ===\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça do topo da pilha com a da frente da fila\n");
        printf("5 - Desfazer última jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        copiarEstado(&anterior, &atual); // Salva estado antes de qualquer ação

        switch (opcao) {
            case 1: {
                Peca jogada;
                if (desenfileirar(&atual.fila, &jogada)) {
                    printf("Peça jogada: [%d-%c]\n", jogada.id, jogada.nome);
                    enfileirar(&atual.fila, gerarPeca());
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 2: {
                Peca reservada;
                if (desenfileirar(&atual.fila, &reservada)) {
                    if (empilhar(&atual.pilha, reservada)) {
                        printf("Peça [%d-%c] reservada!\n", reservada.id, reservada.nome);
                        enfileirar(&atual.fila, gerarPeca());
                    } else {
                        printf("Pilha cheia! Reserva falhou.\n");
                        // Reinsere a peça na frente da fila
                        atual.fila.inicio = (atual.fila.inicio - 1 + TAM_FILA) % TAM_FILA;
                        atual.fila.fila[atual.fila.inicio] = reservada;
                        atual.fila.tamanho++;
                    }
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 3: {
                Peca usada;
                if (desempilhar(&atual.pilha, &usada)) {
                    printf("Peça usada da pilha: [%d-%c]\n", usada.id, usada.nome);
                } else {
                    printf("Pilha vazia!\n");
                }
                break;
            }
            case 4:
                trocarTopoFilaPilha(&atual.fila, &atual.pilha);
                break;
            case 5:
                copiarEstado(&atual, &anterior);
                printf("Última jogada desfeita!\n");
                break;
            case 6:
                inverterFilaComPilha(&atual.fila, &atual.pilha);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        mostrarFila(&atual.fila);
        mostrarPilha(&atual.pilha);

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
    if (p->topo >= TAM_PILHA - 1) return 0;
    p->topo++;
    p->pilha[p->topo] = nova;
    return 1;
}

int desempilhar(PilhaReserva *p, Peca *usada) {
    if (p->topo < 0) return 0;
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

// ----- Extras -----

void trocarTopoFilaPilha(FilaPecas *f, PilhaReserva *p) {
    if (f->tamanho == 0 || p->topo == -1) {
        printf("Não é possível trocar. Fila ou pilha vazia.\n");
        return;
    }
    int idx = f->inicio;
    Peca temp = f->fila[idx];
    f->fila[idx] = p->pilha[p->topo];
    p->pilha[p->topo] = temp;
    printf("Topo da pilha e frente da fila foram trocados!\n");
}

void inverterFilaComPilha(FilaPecas *f, PilhaReserva *p) {
    if (f->tamanho > TAM_PILHA || p->topo + 1 > TAM_FILA) {
        printf("Não é possível inverter: excede os tamanhos permitidos.\n");
        return;
    }

    // Salva os dados temporariamente
    Peca tempFila[TAM_FILA];
    int i = f->inicio;
    for (int c = 0; c < f->tamanho; c++) {
        tempFila[c] = f->fila[i];
        i = (i + 1) % TAM_FILA;
    }

    // Transfere pilha para fila
    inicializarFila(f);
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    for (int i = 0; i <= p->topo; i++) {
        enfileirar(f, p->pilha[i]);
    }

    // Transfere fila antiga para pilha
    inicializarPilha(p);
    for (int i = f->tamanho - 1; i >= 0 && i < TAM_PILHA; i--) {
        empilhar(p, tempFila[i]);
    }

    printf("Fila e pilha foram invertidas com sucesso!\n");
}

void copiarEstado(EstadoJogo *dest, EstadoJogo *src) {
    *dest = *src;
}

// ----- Geração -----

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    Peca p;
    p.id = rand() % 1000;
    p.nome = tipos[rand() % 7];
    return p;
}
