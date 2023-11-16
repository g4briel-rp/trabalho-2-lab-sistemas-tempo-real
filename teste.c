#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

int main()
{
    // Crie a fila
    FilaCircular *fila = criarFila();

    // Preencha a fila com algumas pessoas
    Pessoa pessoa1 = {"Gravida", 0, 5, 1, 1, 0, NULL};
    Pessoa pessoa2 = {"idoso", 1, 2, 2, 2, 0, NULL};
    Pessoa pessoa3 = {"PCD", 2, 1, 3, 3, 0, NULL};
    Pessoa pessoa4 = {"P comum", 3, 1, 4, 4, 0, NULL};

    Pessoa pessoa5 = {"Gravida1", 0, 5, 1, 1, 0, NULL};
    Pessoa pessoa6 = {"idoso1", 1, 2, 2, 2, 0, NULL};
    Pessoa pessoa7 = {"PCD1", 2, 1, 3, 3, 0, NULL};
    Pessoa pessoa8 = {"P comum1", 3, 1, 4, 4, 0, NULL};

    enfileira(fila, pessoa4);
    printFila(fila);

    enfileira(fila, pessoa3);
    printFila(fila);

    enfileira(fila, pessoa2);
    printFila(fila);

    enfileira(fila, pessoa7);
    printFila(fila);

    Pessoa teste1 = desenfileira(fila);
    Pessoa teste2 = desenfileira(fila);
    Pessoa teste3 = desenfileira(fila);

    enfileira(fila, teste3);
    printFila(fila);

    // Libere a memória alocada para a fila
    destruirFila(fila);

    return 0;
}