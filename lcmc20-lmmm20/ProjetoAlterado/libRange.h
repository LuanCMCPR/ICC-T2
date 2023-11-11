
/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675

    Biblioteca de Aritmética Intervalar e Representação Númerica 
*/

typedef struct
{
    double *s; /* Menores valores dos Intervalos */
    double *l; /* Maiores valores dos Intervalos */
} RangeVector;

// Função genérica que aloca a estrutura e dois vetores de pontos com os menores valores e maiores valores dos intervalos.
RangeVector *allocateArraysOfRangeVector(long long int size);

/* 
   Eliminamos a função createRange(), findSmallest() e findLargest que chamavamos para definir os valores menores
   e maiores dos intervalos. Anteriormente tinhamos um vetor de structs de intervalos, ou seja, chamavamos nextafter()
   para calcular o próximo valor representável e inseriamos em:
   vpr[i].x.smallest/largest e vpr[i].y.smallest/largest

   A função se tornou void para facilitar a alteração dos dois vetores independentes, que antigamente eram elementos
   de um array de intervalos.
*/
void generatePointsRanges(long long int numPoints, RangeVector *x, RangeVector *y);

// Função para imprimir os valores dos vetores da estrutura RangeVector
void printRangeVectorValues(RangeVector *rv, long long int size);

// Função que libera a struct e seus vetores
void freeRangeVector(RangeVector *rangeVector);