#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PESSOAS 9
#define CLIENTES 8

typedef struct Pessoa
{
    char nome[10];
    int prioridade;
    int qtdUsoCaixa;
    int indice;
    int tipo; // 1: gravida, 2: idoso, 3: pcd, 4: pessoa comum
    int frustracao;
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

int estaNaFila(FilaCircular *fila, Pessoa pessoa)
{
    Pessoa *atual = fila->frente;
    while (atual != NULL)
    {
        if (strcmp(atual->nome, pessoa.nome) == 0)
        {
            return 1;
        }
        atual = atual->proxima;
    }
    return 0;
}

void enfileira(FilaCircular *fila, Pessoa pessoa)
{
    Pessoa *novaPessoa = (Pessoa *)malloc(sizeof(Pessoa));
    Pessoa *aux;

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

        if (estaVazia(fila))
        {
            // Caso a fila esteja vazia ou a nova pessoa tenha prioridade mais alta que a primeira da fila,
            // insira a nova pessoa no início da fila.
            novaPessoa->proxima = fila->frente;
            fila->frente = novaPessoa;
        }
        else if (novaPessoa->prioridade < fila->frente->prioridade)
        {
            aux = fila->frente;
            fila->frente = novaPessoa;
            novaPessoa->proxima = aux;

            while (aux != NULL)
            {
                aux->frustracao++;
                if (aux->frustracao == 2)
                {
                    aux->prioridade--;
                    aux->frustracao = 0;
                }
                aux = aux->proxima;
            }
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

            // Incrementa a frustração para todas as pessoas a partir da nova pessoa até o final da fila.
            Pessoa *temp = novaPessoa->proxima;
            while (temp != NULL)
            {
                temp->frustracao++;
                if (temp->frustracao == 2)
                {
                    temp->prioridade--;
                    temp->frustracao = 0;
                }
                temp = temp->proxima;
            }
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
        printf("A fila esta vazia!\n\n");
        pessoaRemovida.nome[0] = '\0'; // Inicializa o nome como uma string vazia
    }
    // sempre que atender uma pessoa, sua frustação é zerada e sua prioridade volta ao normal
    pessoaRemovida.frustracao = 0;

    switch (pessoaRemovida.tipo)
    {
    case 1:
        // code
        pessoaRemovida.prioridade = 0;
        break;
    case 2:
        // code
        pessoaRemovida.prioridade = 1;
        break;
    case 3:
        // code
        pessoaRemovida.prioridade = 2;
        break;
    case 4:
        // code
        pessoaRemovida.prioridade = 3;
        break;
    default:
        break;
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

    printf("----------------------------FILA----------------------------\n");

    while (atual != NULL)
    {
        printf("%s\tfrustracao: %d\tprioridade: %d\n", atual->nome, atual->frustracao, atual->prioridade);
        atual = atual->proxima;
    }
    printf("------------------------------------------------------------\n");
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
