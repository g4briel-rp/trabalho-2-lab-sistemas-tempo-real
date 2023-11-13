#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

int main()
{
    // Crie a fila
    FilaCircular *fila = criarFila();

    // Preencha a fila com algumas pessoas
    Pessoa pessoa1 = {"Alice", 3, 5, 1, NULL};
    Pessoa pessoa2 = {"Bob", 1, 2, 2, NULL};
    Pessoa pessoa3 = {"Charlie", 2, 1, 3, NULL};

    printFila(fila);

    enfileira(fila, pessoa1);
    enfileira(fila, pessoa2);
    enfileira(fila, pessoa3);

    // Imprima a fila
    printFila(fila);

    // Desenfileire uma pessoa
    Pessoa pessoaRemovida = desenfileira(fila);
    printf("\nPessoa desenfileirada: %s\n", pessoaRemovida.nome);

    // Imprima a fila após a desenfileiração
    printf("\nFila após desenfileirar uma pessoa:\n");
    printFila(fila);

    // Libere a memória alocada para a fila
    destruirFila(fila);

    return 0;
}