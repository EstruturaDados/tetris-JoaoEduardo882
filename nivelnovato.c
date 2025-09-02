#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----- Definições -----

#define TAM_FILA 5

typedef struct {
    int id;
    char nome; // 'I', 'O', 'T', 'L', etc...
}Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} FilaPecas;

//----- Protótipos -----

voidinicializarFila(FilaPecas *f);
Peca gerarPeca();
int enfilerar(FilaPecas *f, Peca p);
int desenfileirar(FilaPecas *f);
void mostrarFila(FilaPecas *f);

//----- Função Principal -----

int main() {
    FilaPecas fila;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);

    do {
        printf("============================\n");
        printf(" TETRIS STACK - NÍVEL NOVATO\n");
        printf("============================\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("2 - Inserir nova peça (adicionar no final)\n");
        printf("3 - Visualizar fila\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if (desenfileirar(&fila)) {
                    printf("Peça jogada com sucesso!\n");
                } else {
                    printf("Fila vazia! Nada para jogar.\n");
                }
                break;
            case 2: {
                Peca nova = gerarPeca();
                if (enfilerar(&fila, nova)) {
                    printf("Nova peça inserida!: [%d-%c]\n, nova.id,nova.nome");
                } else {
                    printf("Fila cheia! Não e possivel inserir nova peça.\n");
                }
                break;
            }
            case 3:
               mostrarFila(&fila);
               break;
            case 0:
            printf("Encerrando o jogo.\n");
            break;
            default:
            printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;

}

// ----- Implementações -----
 void inicializarFila(FilaPecas *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;

    printf("Inicializando fila com 5 peças:\n");
    for (int i = 0; i < TAM_FILA; i++) {
        Peca p = gerarPeca();
        enfilerar(f, p);
        printf("Peça incial: [%d-%c\n]", p.id, p.nome);
    }
 }

 Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    Peca p;
    p.id = rand() % 1000; // ID único até 999
    p.nome = tipos[rand() % 7];
    return p;
 }

  enfileirar(FilaPecas *f, Peca p) {
    if (f->tamanho == TAM_FILA) {
        return 0; // Fila cheia
    }

    f->fila[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
    return 1;
}

int desenfileirar(FilaPecas *f) {
    if (f->tamanho == 0) {
        return 0; // Fila vazia
    }

    Peca removida = f->fila[f->inicio];
    printf("Peça removida: [%d-%c]\n", removida.id, removida.nome);
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return 1;
}

void mostrarFila(FilaPecas *f) {
    if (f->tamanho == 0) {
        printf("Fila está vazia!\n");
        return;
    }

    printf("Fila atual (%d peças):\n", f->tamanho);
    int i = f->inicio;
    for (int count = 0; count < f->tamanho; count++) {
        Peca p = f->fila[i];
        printf("[%d-%c] ", p.id, p.nome);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}
