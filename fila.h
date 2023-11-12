#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

typedef struct
{
    char nome[10];
    int prioridade;
    int qtdUsoCaixa;
    // Outras informações relevantes para a pessoa
} Pessoa;

typedef struct
{
    Pessoa fila[MAX_SIZE];
    int front, rear;
} FilaCircular;

void initFilaCircular(FilaCircular *fila)
{
    fila->front = 0;
    fila->rear = 0;
}

int filaVazia(FilaCircular *fila)
{
    return (fila->front == fila->rear);
}

int filaCheia(FilaCircular *fila)
{
    return ((fila->rear + 1) % MAX_SIZE == fila->front);
}

int tamanhoFila(FilaCircular *fila)
{
    if (filaVazia(fila))
    {
        return 0;
    }

    if (fila->front < fila->rear)
    {
        return fila->rear - fila->front;
    }
    else
    {
        return MAX_SIZE - fila->front + fila->rear;
    }
}

void enfileira(FilaCircular *fila, Pessoa pessoa)
{
    if (filaCheia(fila))
    {
        printf("Fila cheia!\n");
        return;
    }

    fila->fila[fila->rear] = pessoa;
    fila->rear = (fila->rear + 1) % MAX_SIZE;
}

Pessoa desenfileira(FilaCircular *fila)
{
    Pessoa pessoa;

    if (filaVazia(fila))
    {
        printf("Fila vazia!\n");
        pessoa.prioridade = -1;
        return pessoa;
    }

    pessoa = fila->fila[fila->front];
    fila->front = (fila->front + 1) % MAX_SIZE;

    return pessoa;
}

int compara(const void *a, const void *b)
{
    return ((Pessoa *)a)->prioridade - ((Pessoa *)b)->prioridade;
}

void ordena(FilaCircular *fila)
{
    int tam = tamanhoFila(fila);
    Pessoa *array = malloc(tam * sizeof(int));

    for (int i = 0; i < tam; i++)
    {
        array[i] = desenfileira(fila);
    }

    qsort(array, tam, sizeof(Pessoa), compara);

    for (int i = 0; i < tam; i++)
    {
        enfileira(fila, array[i]);
    }

    free(array);
}

void printFila(FilaCircular *fila)
{
    int i;

    if (filaVazia(fila))
    {
        printf("Fila vazia!\n");
        return;
    }

    printf("Fila: ");
    for (i = fila->front; i != fila->rear; i = (i + 1) % MAX_SIZE)
    {
        printf("%s ", fila->fila[i].nome);
    }
    printf("\n");
}

#endif