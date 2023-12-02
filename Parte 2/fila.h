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

int isDeadlock(FilaCircular *fila, Pessoa pessoa)
{
    /**
     * antes de enfileirar a pessoa la no main, verifica-se se a inserção dela vai causar o DEADLOCK
     * se sim, onde inseri-la ?
     */
    Pessoa *p;
    int vet[3] = {0, 0, 0};

    if (pessoa.tipo == 1)
    {
        vet[0] = 1;
    }
    else if (pessoa.tipo == 2)
    {
        vet[1] = 2;
    }
    else if (pessoa.tipo == 3)
    {
        vet[2] = 3;
    }

    while (p != fila->tras)
    {
        if (p->tipo == 1)
        {
            vet[0] = 1;
        }
        else if (p->tipo == 2)
        {
            vet[1] = 2;
        }
        else if (p->tipo == 3)
        {
            vet[2] = 3;
        }
        p = p->proxima;
    }

    if (vet[0] == 1 && vet[1] == 2 && vet[3] == 3)
    {
        printf("DeadLock aqui, rapaziada!!!\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

void retornaTipoPessoa(FilaCircular *fila, int vet[])
{
    Pessoa *pessoa = fila->frente;

    while (pessoa != fila->tras->proxima)
    {
        // onde tiver 1, significa que existe pessoa daquele tipo na fila
        if (pessoa->tipo == 1)
        {
            vet[0] = 1;
        }
        else if (pessoa->tipo == 2)
        {
            vet[1] = 1;
        }
        else if (pessoa->tipo == 3)
        {
            vet[2] = 1;
        }
        else if (pessoa->tipo == 4)
        {
            vet[3] = 1;
        }
        pessoa = pessoa->proxima;
    }
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
            fila->frente = novaPessoa;
            fila->tras = novaPessoa;
            fila->tamanho++;
        }
        else
        {
            aux = fila->tras;
            fila->tras = novaPessoa;
            aux->proxima = novaPessoa;
            fila->tamanho++;
        }
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

Pessoa desenfileira(FilaCircular *fila, int prioridade)
{
    Pessoa pessoaRemovida;
    if (!estaVazia(fila))
    {
        if (prioridade == -1) // -1 = DEADLOCK
        {
            Pessoa *temp = fila->frente;
            fila->frente = fila->frente->proxima;
            pessoaRemovida = *temp;
            fila->tamanho--;
        }
        else
        {
            Pessoa *temp = fila->frente;
            Pessoa *aux = NULL;

            while (temp != fila->tras->proxima)
            {
                if (temp->prioridade == prioridade)
                {
                    break;
                }
                else
                {
                    aux = temp;
                    temp = temp->proxima;
                }
            }

            if (aux == NULL)
            {
                // se aux for NULL, significa que a pessoa a ser removida é a primeira da fila
                fila->frente = fila->frente->proxima;
            }
            else if (temp == fila->tras)
            {
                // se temp for igual ao ultimo da fila, significa que a pessoa a ser removida é a ultima da fila
                fila->tras = aux;
                aux->proxima = NULL;
            }
            else
            {
                // se aux e temp forem diferentes de NULL, significa que a pessoa a ser removida está no meio da fila
                aux->proxima = temp->proxima; // o anterior ao temp aponta para o proximo do temp
            }

            pessoaRemovida = *temp;
            fila->tamanho--;
        }
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
