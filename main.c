#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 9 /* Máximo de iterações */
#define NUM_PEOPLE 9

/* Mutex to protect the resource. */
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

/*
  Condition variable to signal consumer that a new number is available for
  consumption.
*/
pthread_cond_t sig_consumer = PTHREAD_COND_INITIALIZER;
/*
  Condition variable to signal the producer that
  (a) the new number has been consumed,
  (b) generate another one.
*/
pthread_cond_t sig_producer = PTHREAD_COND_INITIALIZER;

typedef struct
{
    int qtdUsoCaixa;
    char nome[10];
    int prioridade;
    pthread_t thread;
    // Outras informações relevantes para a pessoa
} Pessoa;

int number; /* the resource */
Pessoa pessoas[NUM_PEOPLE];

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

void *consumer(void *dummy)
{
    printf("Consumer : \"Hello I am consumer #%ld. Ready to consume numbers"
           " now\"\n",
           pthread_self());

    while (1)
    {
        pthread_mutex_lock(&mu);
        /* Signal the producer that the consumer is ready. */
        pthread_cond_signal(&sig_producer);
        /* Wait for a new number. */
        pthread_cond_wait(&sig_consumer, &mu);
        /* Consume (print) the number. */
        printf("Consumer : %d\n", number);
        /* Unlock the mutex. */
        pthread_mutex_unlock(&mu);

        /*
          If the MAX number was the last consumed number, the consumer should
          stop.
        */
        if (number == MAX)
        {
            printf("Consumer done.. !!\n");
            break;
        }
    }
}

/**
  @func producer
  This function is responsible for incrementing the number and signalling the
  consumer.
*/
void *producer(void *dummy)
{
    printf("Producer : \"Hello I am producer #%ld. Ready to produce numbers"
           " now\"\n",
           pthread_self());

    while (1)
    {
        pthread_mutex_lock(&mu);
        number++;
        printf("Producer : %d\n", number);
        /*
          Signal the consumer that a new number has been generated for its
          consumption.
        */
        pthread_cond_signal(&sig_consumer);
        /*
          Now wait for consumer to confirm. Note, expect no confirmation for
          consumption of MAX from consumer.
        */
        if (number != MAX)
            pthread_cond_wait(&sig_producer, &mu);

        /* Unlock the mutex. */
        pthread_mutex_unlock(&mu);

        /* Stop if MAX has been produced. */
        if (number == MAX)
        {
            printf("Producer done.. !!\n");
            break;
        }
    }
}
int main()
{
    pthread_t monitor;
    int i;

    // Inicialize as pessoas com prioridades e quantidade de uso do caixa
    for (i = 0; i < NUM_PEOPLE; i++)
    {
        pessoas[i].prioridade = i % 4; // Prioridade de 0 a 3
        printf("Digite o nome da pessoa %d: ", i + 1);
        scanf("%s", pessoas[i].nome);
        printf("Digite a quantidade de vezes que a pessoa %s vai usar o caixa: ", pessoas[i].nome);
        scanf("%d", &pessoas[i].qtdUsoCaixa);
    }

    // Embaralhe aleatoriamente a ordem das pessoas
    srand(time(NULL));
    for (i = NUM_PEOPLE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Pessoa temp = pessoas[i];
        pessoas[i] = pessoas[j];
        pessoas[j] = temp;
    }

    /* Create consumer & producer threads. */
    // if ((rc = pthread_create(&t[0], NULL, consumer, NULL)))
    //     printf("Error creating the consumer thread..\n");
    // if ((rc = pthread_create(&t[1], NULL, producer, NULL)))
    //     printf("Error creating the producer thread..\n");

    // /* Wait for consumer/producer to exit. */
    // for (i = 0; i < 2; i++)
    //     pthread_join(t[i], NULL);

    printf("Done..\n");
    return 0;
}
