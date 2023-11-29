/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675
*/

/* Lib de Sistemas Lineares */

#include "libRange.h"

/* Struct que representa um Sistema Linear de Intervalos */
    typedef struct 
    {
        Range_t *cm; /* Matriz de coeficientes intervalar */
        Range_t *vit; /* Vetor de termos independentes intervalar */
    } LinearSystem_t;

/*
    Função que aloca um Sistema Linear.
    Parametros:
        size: Tamanho do Sistema Linear
    Códigos de Erro:
        3 - Erro ao alocar sistema linear.
        4 - Erro ao alocar matriz de coeficientes
        5 - Erro ao alocar linha %d da matriz de coeficientes.
*/
LinearSystem_t *allocateLinearSystem(int size);

/* 
    Função que aloca e constrói o Sistema Linear.
    Parâmetros: Matriz de coeficientes, Vetor de termos independentes, Ordem da matriz.
    Retorno: Sistema Linear construído.  
*/
void createLinearSystem(LinearSystem_t *LS, PointsRange_t* vpr, long long int num_points,  int sizeLS);

/*
    Função que imprime o Sistema Linear.
    Observação: Não é utilizada, utilizamos apenas para testar o programa
*/
void printLinearSystem(LinearSystem_t *LS, int size);

/*
    Função que aplica a Eliminação-Gaussiana com pivoteamento parcial adaptada para intervalos
    Parametros:
        LS: Matriz de coeficientes e termos independentes
        n: Ordem da matriz
*/
void classicEliminationWithPivot(LinearSystem_t *LS, unsigned int n);

/*
    Função que resolve sistema triangulares superiores adaptada para intervalos
    Parametros:
        LS: Matriz de coeficientes e termos independentes
        x: Vetor de soluções
        n: Ordem da matriz
*/
void retroSubstitution(LinearSystem_t *LS, Range_t *x, int n);

/*
    Função que libera memória alocada para um Sistema Linear.
    Parametros:
        LS: Sistema linear
        size: Tamanho do Sistema Linear
*/
/* OTIMIZAÇAO: Removido a variavel size */
void freeLinearSystem(LinearSystem_t *LS);


/*
    Função que aloca um vetor de intervalos.
    Parametros:
        size: Tamanho do vetor
    Códigos de Erro:
        6 - Erro ao alocar vetor de intervalos.
*/
Range_t* allocateArrayRange(int size);

/*
    Função que faz o cálculo do resíduo
    Parametros:
        vpr: Tabela de Pontos em intervalos
        a: Vetor de coeficientes em intervalos 
        num_points: Número de pontos
        sizeLS: Tamanho do Sistema Linear
*/
void calculateResidualVector(Range_t *rv, PointsRange_t *vpr, Range_t *a,long long int num_points, int sizeLS);