#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

int main()
{
    FilaCircular fila;
    initFilaCircular(&fila);

    Pessoa pessoas[9] = {
        {"gerente", 4, 0},
        {"Vanda", 1, 0},
        {"Valter", 1, 0},
        {"Maria", 0, 0},
        {"Marcos", 0, 0},
        {"Paula", 2, 0},
        {"Pedro", 2, 0},
        {"Sueli", 3, 0},
        {"Silas", 3, 0},
    };

    while (1)
    {
        if (filaVazia(&fila))
        {
            printf("Fila vazia!\n");
        }
        else if (filaCheia(&fila))
        {
            printf("Fila cheia!\n");
            break;
        }

        for (int i = 0; i < 9; i++)
        {
            enfileira(&fila, pessoas[i]);
            printf("%d\n", tamanhoFila(&fila));
            printFila(&fila);
            ordena(&fila);
            printFila(&fila);
        }
    }
}