#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"

#define NUM_PEOPLE 9

// Pessoas
// Vanda -> idosa
// Valter -> marido de Vanda e idoso
// Maria -> gravida
// Marcos -> casado com Maria e carrega a filha no colo
// Paula -> pé quebrado
// Pedro -> noivo de Paula e está com o pé quebrado
// Sueli -> pessoa comum
// Silas -> namora com Sueli e é uma pessoa comum

// Ordem de Prioridade
// Gravida(ou com criança no colo) -> idoso
// Idoso -> Deficiente
// Deficiente -> Comum
// Prioridade igual, usar ordem de chegada

/** -------PARTE 1 DO TRABALHO
 * grávida  >   idoso
 * idoso    >   PCD
 * PCD      >   PComun
 * PComun   0
 */

/** -------PARTE 2 DO TRABALHO
 * grávida  >   idoso
 * idoso    >   PCD
 * PCD      >   Grávida
 * PComun   0
 */

/* Mutex to protect the resource. */
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

/*
  Condition variable to signal consumer that a new number is available for
  consumption.
*/
pthread_cond_t sig_gerente = PTHREAD_COND_INITIALIZER;
/*
  Condition variable to signal the producer that
  (a) the new number has been consumed,
  (b) generate another one.
*/
pthread_cond_t sig_cliente = PTHREAD_COND_INITIALIZER;

int number = 0; /* the resource */
Pessoa pessoas[NUM_PEOPLE];
FilaCircular fila;

// THREAD DO GERENTE

// enquanto pessoas estao ativas faça:
// sleep(INTERVALO_VERIFICACAO);
// caixa.verificar();

void *atender_pessoa(void *arg)
{
    printf("Gerente : \"Hello I am gerente #%ld. Ready to atender pessoas now\"\n", pthread_self());
    Pessoa p = *((Pessoa *)arg); // Converte-se o argumento para uma pessoa
}

// THREAD DAS PESSOAS A SEREM ATENDIDAS

// caixa.esperar(p); //exige mutex
// atendido_pelo_caixa(p); //nao exige exclusao mútua
// caixa.liberar(p); //exige mutex
// vai_embora_para_casa(p); //espera um certo tempo aleatório

void *solicitarAtendimento(void *arg)
{
    Pessoa p = *((Pessoa *)arg); // Converte-se o argumento para uma pessoa
    printf("\"%s está na fila do caixa {fila: }\"\n", p.nome);
}

// argc: quantidade de argumentos
// argv: os argumentos em si
int main(int argc, char *argv[])
{
    pthread_t monitor;
    int rc, i;
    pthread_t t[9];
    srand(time(NULL)); // inicializa a semente do gerador de números aleatórios
    initFilaCircular(&fila);

    // ###############################-- ARGV PARA INTEIRO --####################################
    char *p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    long arg = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0)
    {
        return 1; // In main(), returning non-zero means failure
    }

    if (arg < INT_MIN || arg > INT_MAX)
    {
        return 1;
    }

    int arg_int = arg;
    // ###############################-- ARGV PARA INTEIRO --####################################

    // nome, prioridade, qtdUsoCaixa
    Pessoa pessoas[9] = {
        {"gerente", -1, arg_int * 8},
        {"Vanda", 1, arg_int},
        {"Valter", 1, arg_int},
        {"Maria", 0, arg_int},
        {"Marcos", 0, arg_int},
        {"Paula", 2, arg_int},
        {"Pedro", 2, arg_int},
        {"Sueli", 3, arg_int},
        {"Silas", 3, arg_int},
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

    /* Wait for consumer/producer to exit. */
    for (i = 0; i < 9; i++)
        pthread_join(t[i], NULL);

    printf("Done..\n");
    return 0;
}