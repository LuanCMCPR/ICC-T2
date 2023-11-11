/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675
*/

/* Lib de Aritmética Intervalar e Representação Númerica */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <fenv.h>
#include <string.h>
#include "libRange.h"

/* Função genérica que aloca a estrutura e dois vetores de pontos com os menores valores e maiores valores dos intervalos. */
RangeVector *allocateArraysOfRangeVector(long long int size) 
{
    // Aloca dinamicamente a estrutura RangeVector
    RangeVector *newRangeVector = (RangeVector *)malloc(sizeof(RangeVector));

    if (newRangeVector == NULL) 
    {
        printf("Memory allocation failed\n");
        exit(1); 
    }

    // Aloca dinamicamente os arrays dentro da estrutura
    newRangeVector->s = (double *)malloc(size * sizeof(double));
    newRangeVector->l = (double *)malloc(size * sizeof(double));

    if (newRangeVector->s == NULL || newRangeVector->l == NULL) 
    {
        printf("Memory allocation failed\n");
        free(newRangeVector); 
        exit(1); 
    }

    // Retorna o ponteiro para a nova estrutura RangeVector alocada
    return newRangeVector;
}

/* 
   Eliminamos a função createRange(), findSmallest() e findLargest  que chamavamos para definir os valores menores
   e maiores dos intervalos. Anteriormente tinhamos um vetor de structs de intervalos, ou seja, chamavamos nextafter()
   para calcular o próximo valor representável e inseriamos em:
   vpr[i].x.smallest/largest e vpr[i].y.smallest/largest

   A função se tornou void para facilitar a alteração dos dois vetores independentes, que antigamente eram elementos
   de um array de intervalos.
*/
void generatePointsRanges(long long int numPoints, RangeVector *x, RangeVector *y)
{    
    double xi, yi;

    // Define o intervalos dos menores e maiores valores e insere os mesmo em vetores específicos
    for(long long int i = 0; i < numPoints; i++)
    {
        if (scanf("%lf %lf", &xi, &yi) != 2) 
        {
            fprintf(stderr, "Erro na leitura dos valores.\n");
            exit(EXIT_FAILURE);
        }
        
        x->s[i] = nextafter(xi, -INFINITY);
        x->l[i] = nextafter(xi, INFINITY);
        y->s[i] = nextafter(yi, -INFINITY);
        y->l[i] = nextafter(yi, INFINITY);
    }
}

// Função para imprimir os valores dos vetores da estrutura RangeVector
void printRangeVectorValues(RangeVector *rv, long long int size)
{
    fprintf(stdout, "Vector values:\n");
    for (int i = 0; i < size; ++i) {
        fprintf(stdout, "[%1.8e,%1.8e] ", rv->s[i], rv->l[i]);
    }
    printf("\n");
}

void freeRangeVector(RangeVector *rangeVector) 
{
    // Libera a memória alocada para os arrays dentro da estrutura
    free(rangeVector->s);
    free(rangeVector->l);

    // Libera a memória alocada para a própria estrutura
    free(rangeVector);
}
