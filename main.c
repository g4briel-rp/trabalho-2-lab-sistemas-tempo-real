#include "fila.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_PEOPLE 9
#define liberacao 1
#define atendimento 3
#define voltinha 5

/** -------PARTE 2 DO TRABALHO
 * grávida  >   idoso
 * idoso    >   PCD
 * PCD      >   Grávida
 * PComun   0
 */

/* Mutex to protect the resource. */
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cliente[8];

Pessoa pessoas[NUM_PEOPLE];
FilaCircular fila;

void *atender_pessoa(void *arg)
{
    Pessoa p = *((Pessoa *)arg); // Converte-se o argumento para uma pessoa

    for (int k = 0; k < 8; k++)
    {
        pthread_mutex_lock(&cliente[k]);
    }

    while (1)
    {
        pthread_mutex_lock(&mu);

        if (tamanhoFila(&fila) == 0)
        {
            pthread_mutex_unlock(&mu);
            sleep(5);
            continue;
        }

        printf("\n%s está verificando a fila.\n", p.nome);

        sleep(atendimento);

        Pessoa pessoa = desenfileira(&fila);

        pthread_mutex_unlock(&mu);

        printf("%s está sendo atendido.\n", pessoa.nome);

        sleep(liberacao);

        printf("%s foi embora.\n", pessoa.nome);

        pthread_mutex_unlock(&cliente[pessoa.indice]);

        p.qtdUsoCaixa--;

        if (p.qtdUsoCaixa == 0)
        {
            printf("Todos foram atendidos.\n");
            break;
        }
    }
}

void *solicitarAtendimento(void *arg)
{
    Pessoa p = *((Pessoa *)arg); // Converte-se o argumento para uma pessoa

    while (p.qtdUsoCaixa > 0)
    {
        pthread_mutex_lock(&mu);

        enfileira(&fila, p);

        printf("\n");
        printFila(&fila);
        printf("\n");

        pthread_mutex_unlock(&mu);

        printf("%s está esperando para ser atendido.\n", p.nome);

        pthread_mutex_lock(&cliente[p.indice]);

        p.qtdUsoCaixa--;

        sleep(voltinha);
    }
}

int main(int argc, char *argv[])
{
    int rc, i;
    pthread_t t[9];

    for (int j = 0; j < 8; j++)
    {
        pthread_mutex_init(&cliente[j], NULL);
    }

    srand(time(NULL)); // inicializa a semente do gerador de números aleatórios

    // ###############################-- ARGV PARA INTEIRO --####################################
    char *p;
    errno = 0;
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0)
    {
        return 1;
    }

    if (arg < INT_MIN || arg > INT_MAX)
    {
        return 1;
    }

    int arg_int = arg;
    // ###############################-- ARGV PARA INTEIRO --####################################

    // nome, prioridade, qtdUsoCaixa
    Pessoa pessoas[9] = {
        {"gerente", -1, arg_int * 8, -1},
        {"Vanda", 1, arg_int, 0, 2, 0},
        {"Valter", 1, arg_int, 1, 2, 0},
        {"Maria", 0, arg_int, 2, 1, 0},
        {"Marcos", 0, arg_int, 3, 1, 0},
        {"Paula", 2, arg_int, 4, 3, 0},
        {"Pedro", 2, arg_int, 5, 3, 0},
        {"Sueli", 3, arg_int, 6, 4, 0},
        {"Silas", 3, arg_int, 7, 4, 0},
    };

    printf("\n");

    // Embaralhe aleatoriamente a ordem das pessoas que não são gerentes
    srand(time(NULL));
    for (i = NUM_PEOPLE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        if (j != 0)
        {
            Pessoa temp = pessoas[i];
            pessoas[i] = pessoas[j];
            pessoas[j] = temp;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (i == 0)
        {
            if (pthread_create(&t[i], NULL, atender_pessoa, &pessoas[i]) != 0)
            {
                printf("Erro ao criar a thread da gerente %s\n", pessoas[i].nome);
                return 1;
            }
        }
        else
        {
            if (pthread_create(&t[i], NULL, solicitarAtendimento, &pessoas[i]) != 0)
            {
                printf("Erro ao criar a thread da pessoa %s\n", pessoas[i].nome);
                return 1;
            }
        }
    }

    for (i = 0; i < 9; i++)
        pthread_join(t[i], NULL);

    printf("Fim da execução!!!\n");
    return 0;
}