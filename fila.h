#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 8

typedef struct
{
    char iniciais[MAX_SIZE];
    int front, rear;
} FilaCircular;

void initFilaCircular(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
void enfileira(FilaCircular *fila, int valor);
int desenfileira(FilaCircular *fila);
void printFila(FilaCircular *fila);

// Inicializa a fila circular
void initFilaCircular(FilaCircular *fila)
{
    fila->front = -1;
    fila->rear = -1;
}

// Verifica se a fila está vazia
int filaVazia(FilaCircular *fila)
{
    return (fila->front == -1 && fila->rear == -1);
}

// Verifica se a fila está cheia
int filaCheia(FilaCircular *fila)
{
    return (fila->rear + 1) % MAX_SIZE == fila->front;
}

// Insere um elemento na fila
void enfileira(FilaCircular *fila, int valor)
{
    if (filaCheia(fila))
    {
        printf("A fila está cheia. Não é possível enfileirar %d.\n", valor);
        return;
    }

    if (filaVazia(fila))
    {
        fila->front = 0;
        fila->rear = 0;
    }
    else
    {
        fila->rear = (fila->rear + 1) % MAX_SIZE;
    }

    fila->iniciais[fila->rear] = valor;
}

// Remove um elemento da fila
int desenfileira(FilaCircular *fila)
{
    if (filaVazia(fila))
    {
        printf("A fila está vazia. Não é possível desenfileirar.\n");
        return -1; // Valor de erro
    }

    int valor = fila->iniciais[fila->front];

    if (fila->front == fila->rear)
    {
        fila->front = -1;
        fila->rear = -1;
    }
    else
    {
        fila->front = (fila->front + 1) % MAX_SIZE;
    }

    return valor;
}

// Imprime os elementos da fila
void printFila(FilaCircular *fila)
{
    if (filaVazia(fila))
    {
        printf("A fila está vazia.\n");
        return;
    }

    printf("Elementos da fila: ");
    int i = fila->front;
    do
    {
        printf("%d ", fila->iniciais[i]);
        i = (i + 1) % MAX_SIZE;
    } while (i != (fila->rear + 1) % MAX_SIZE);
    printf("\n");
}

#endif
