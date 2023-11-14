#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include <stdlib.h>

#define PESSOAS 9
#define CLIENTES 8

typedef struct Pessoa
{
    char nome[10];
    int prioridade;
    int qtdUsoCaixa;
    int indice;
    // Outras informações relevantes para a pessoa
    struct Pessoa *proxima;
} Pessoa;
typedef struct FilaCircular
{
    Pessoa *frente;
    Pessoa *tras;
    int tamanho;
} FilaCircular;

FilaCircular *criarFila()
{
    FilaCircular *fila = (FilaCircular *)malloc(sizeof(FilaCircular));
    if (fila == NULL)
    {
        // Tratamento de erro, falha na alocação de memória
        return NULL;
    }

    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;

    return fila;
}

int estaVazia(FilaCircular *fila)
{
    return (fila->frente == NULL);
}

int filaCheia(FilaCircular *fila)
{
    return (fila->tamanho == CLIENTES);
}

int tamanhoFila(FilaCircular *fila)
{
    return fila->tamanho;
}

void enfileira(FilaCircular *fila, Pessoa pessoa)
{
    Pessoa *novaPessoa = (Pessoa *)malloc(sizeof(Pessoa));

    // eu só posso enfileirar alguem se a fila não estiver cheia
    if (!filaCheia(fila))
    {
        if (novaPessoa == NULL)
        {
            // Tratamento de erro, falha na alocação de memória
            return;
        }
        *novaPessoa = pessoa;
        novaPessoa->proxima = NULL;

        if (estaVazia(fila) || pessoa.prioridade < fila->frente->prioridade)
        {
            // Caso a fila esteja vazia ou a nova pessoa tenha prioridade mais alta que a primeira da fila,
            // insira a nova pessoa no início da fila.
            novaPessoa->proxima = fila->frente;
            fila->frente = novaPessoa;
        }
        else
        {
            // Caso contrário, percorra a fila para encontrar a posição correta.
            Pessoa *atual = fila->frente;
            while (atual->proxima != NULL && atual->proxima->prioridade <= pessoa.prioridade)
            {
                atual = atual->proxima;
            }
            novaPessoa->proxima = atual->proxima;
            atual->proxima = novaPessoa;
        }

        if (novaPessoa->proxima == NULL)
        {
            // Se a nova pessoa estiver no final da fila, atualize fila->tras.
            fila->tras = novaPessoa;
        }

        fila->tamanho++;
    }
}

Pessoa primeira_da_fila(FilaCircular *fila)
{
    Pessoa pessoa;
    if (!estaVazia(fila))
    {
        pessoa = *(fila->frente);
    }
    else
    {
        pessoa.nome[0] = '\0'; // Inicializa o nome como uma string vazia
    }
    return pessoa;
}

Pessoa desenfileira(FilaCircular *fila)
{
    Pessoa pessoaRemovida;
    if (!estaVazia(fila))
    {
        Pessoa *temp = fila->frente;
        fila->frente = fila->frente->proxima;
        pessoaRemovida = *temp;
        free(temp);
        fila->tamanho--;
    }
    else
    {
        // Tratamento de erro, a fila está vazia
        pessoaRemovida.nome[0] = '\0'; // Inicializa o nome como uma string vazia
    }
    return pessoaRemovida;
}

void printFila(FilaCircular *fila)
{
    if (fila == NULL || estaVazia(fila))
    {
        printf("A fila está vazia.\n");
        return;
    }

    Pessoa *atual = fila->frente;

    printf("--------------FILA--------------\n");

    while (atual != NULL)
    {
        printf("%s\t", atual->nome);
        atual = atual->proxima;
    }
    printf("\n--------------------------------\n");
}

void destruirFila(FilaCircular *fila)
{
    if (fila != NULL)
    {
        while (fila->frente != NULL)
        {
            Pessoa *temp = fila->frente;
            fila->frente = fila->frente->proxima;
            free(temp);
        }
        free(fila);
    }
}

#endif
