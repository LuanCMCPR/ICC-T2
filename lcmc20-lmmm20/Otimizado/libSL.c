/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675
*/

/* Lib de Sistemas Lineares */

#include "libSL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
/*
    Função que aloca um Sistema Linear.
    Parametros:
        size: Tamanho do Sistema Linear
    Códigos de Erro:
        3 - Erro ao alocar sistema linear.
        4 - Erro ao alocar matriz de coeficientes
        5 - Erro ao alocar linha %d da matriz de coeficientes.
*/
/* OTIMIZAÇÃO: Função Removida, a struct PointsRange_t sera usada para representar o Sistema Linear */
LinearSystem_t *allocateLinearSystem(int size)
{
    LinearSystem_t *system;

    system = (LinearSystem_t*)malloc(sizeof(LinearSystem_t));
    if (system == NULL)
    {
        fprintf(stderr, "Erro ao alocar sistema linear.\n");
        exit(3);
    }
    
    // Aloca a matriz de coeficientes
    system->cm = (Range_t *)malloc(size * size * sizeof(Range_t));
    if (system->cm == NULL)
    {
        fprintf(stderr, "Erro ao alocar matriz de coeficientes.\n");
        free(system);
        exit(4);
    }

    
    // REMOVIDO:
    // // Aloca cada linha da matriz de coeficientes
    // for (int i = 0; i < size; i++)
    // {
    //     system->cm[i] = (Range_t *)malloc(size * sizeof(Range_t));
    //     for(int j = 0; j < size; j++)
    //     {
    //         system->cm[i][j].smallest = 0.0;
    //         system->cm[i][j].largest = 0.0;
    //     }
    //     if (system->cm[i] == NULL)
    //     {
    //         fprintf(stderr, "Erro ao alocar linha %d da matriz de coeficientes.\n", i);
    //         for (int j = 0; j < i; j++)
    //         {
    //             free(system->cm[j]);
    //         }
    //         free(system->cm);
    //         free(system);
    //         exit(5);
    //     }
    // }

    // Alocar o vetor de termos independentes
    system->vit = allocateArrayRange(size);

    memset(system->cm, 0, size * size * sizeof(Range_t));
    memset(system->vit, 0, size * sizeof(Range_t));
    // REMOVIDO:
    // for(int i = 0; i < size; i++)
    // {
    //     system->vit[i].smallest = 0.0;
    //     system->vit[i].largest = 0.0;
    // }

    return system;
}


/*
    Função que gera o Sistema Linear a partir do Método dos Minímos Quadrados.
    Parametros:
        vpr: Tabela de pontos em intervalos
        num_points: Número de pontos
        sizeLS: Tamanho do Sistema Linear
*/
/* OTIMIZAÇÃO:
        1. Alteração na forma de acesso da matriz, agora ela é alocada como um vetor
 */
LinearSystem_t *createLinearSystem(PointsRange_t *vpr, long long int num_points, int sizeLS)
{
    int i, j, power_x;
    LinearSystem_t *LS;
    Range_t res;

    LS = allocateLinearSystem(sizeLS);

    /* Cálculo dos coeficientes da primeira linha do Sistema Linear */
    
    LS->cm[0].smallest = nextafter(num_points, -INFINITY);
    LS->cm[0].largest = nextafter(num_points, INFINITY);


    for(i = 1; i < sizeLS; i++)
    {
        // REMOVIDO:
        // LS->cm[0 + i].smallest = 0.0;
        // LS->cm[0 + i].largest = 0.0;

        /* Cálculo dos Somatórios de x^i */
        for (j = 0; j < num_points; j++)
        {
            res = powerRange(vpr->x[j], i);
            LS->cm[0 + i].smallest += res.smallest; 
            LS->cm[0 + i].largest += res.largest;
        }    
        //     res = powerRange(vpr->x[j], i + 1);
        //     LS->cm[0 + i + 1].smallest += res.smallest; 
        //     LS->cm[0 + i + 1].largest += res.largest;   
 
        //     res = powerRange(vpr->x[j], i+2);
        //     LS->cm[0 + i + 2].smallest += res.smallest; 
        //     LS->cm[0 + i + 2].largest += res.largest;
            
        //     res = powerRange(vpr->x[j], i + 3);
        //     LS->cm[0 + i + 3].smallest += res.smallest; 
        //     LS->cm[0 + i + 3].largest += res.largest;  
        // }
        // for (j = 0; j < num_points; j+=2)
        // {
        //     LS->cm[0 + i]=addRange(powerRange(vpr->x[j], i), powerRange(vpr->x[j+1], i)); 
        //     LS->cm[0 + i+1]=addRange(powerRange(vpr->x[j+2], i), powerRange(vpr->x[j+3], i));
            
        // }

    }

    // REMOVIDO:
    // LS->vit[0].smallest = 0.0;
    // LS->vit[0].largest = 0.0;

    /* Cálculo do coeficiente do Vetor de Termos Independentes da Primeira Linha */
    for(i = 0; i < num_points; i++)
    {
        /* To do: usar addRange() */
        LS->vit[0].smallest += vpr->y[i].smallest;
        LS->vit[0].largest += vpr->y[i].largest;
    }

    /* Cálculo dos coeficientes das demais linhas */
    /* power_x serve para calcular o coeficiente gerado pelo último somatório da equação */
    power_x = sizeLS;
    for(i = 1; i < sizeLS; i++)
    {
        /* Copia os valores de cima para baixo com base na primeira linha */
        for (j = 0; j < sizeLS-1; j++)
            LS->cm[i*sizeLS + j] = LS->cm[((i-1)*(sizeLS)) + (j+1)];
        

        // REMOVIDO:
        // LS->cm[i + sizeLS-1].smallest = 0.0;
        // LS->cm[i + sizeLS-1].largest = 0.0;
        
        /* Cálcula o coeficiente do Vetor de Termos Independetes e o último coeficiente da equação */
        for (j = 0; j < num_points; j++)
        {
            /* Calcula o coeficiente de xn do Sistema Linear */
            res = powerRange(vpr->x[j], power_x); // x^k scom k = power_x
            LS->cm[(i*sizeLS) + (sizeLS-1)].smallest += res.smallest;
            LS->cm[(i*sizeLS) + (sizeLS-1)].largest += res.largest;
            /* Calcula o coeficiente do Vetor de Termos Independentes */
            res = powerRange(vpr->x[j], i); // x[j]^i com i = linha
            Range_t mult = timeRange(vpr->y[j], res); // multiplica y[i] com x[j]^i
            LS->vit[i].smallest += mult.smallest;
            LS->vit[i].largest += mult.largest;
        }
            
        power_x++;
    }

    // printf("[%lf, %lf]\n", LS->cm[(1*sizeLS) + (sizeLS-1)].smallest, LS->cm[(1*sizeLS) + (sizeLS-1)].largest);
    // printf("[%lf, %lf]\n", LS->cm[(2*sizeLS) + (sizeLS-1)].smallest, LS->cm[(2*sizeLS) + (sizeLS-1)].largest);

    printLinearSystem(LS, sizeLS);

    return LS;
}

/*
    Função que imprime o Sistema Linear.
    Observação: Não é utilizada, utilizamos apenas para testar o programa
*/
void printLinearSystem(LinearSystem_t *LS, int size)
{
    int i, j;

    printf("Sistema Linear:\n");

    for (i = 0; i < size; i++)
    {
        for (j = 0; j <     size; j++)
        {
            printf("[%lf, %lf]x%d ", LS->cm[i*size + j].smallest, LS->cm[i*size + j].largest, j+1);
        }
        printf("= [%lf, %lf]\n", LS->vit[i].smallest, LS->vit[i].largest);
    }
}

/*
    Função que faz a troca de duas linhas do Sistema Linear intervalar.
    Parametros:
        cm: Matriz de coeficientes
        vit: Vetor de termos independentes
        a: Linha a ser trocada
        b: linha a ser trocada
*/
/*
    OTIMIZAÇÃO: Alteração na forma de acesso da matriz, agora ela é alocada como um vetor. Faz a troca com ponteiros
*/
void swapLines(Range_t *cm, Range_t *vit, unsigned int a, unsigned int b, int numColumns)
{
    /* Troca as linhas da matriz */
    Range_t *ptrA = cm + a * numColumns;
    Range_t *ptrB = cm + b * numColumns;
    Range_t *ptrEnd = cm + numColumns*numColumns;

    while(ptrA < ptrEnd)
    {
        Range_t aux = *ptrA;
        *ptrA = *ptrB;
        *ptrB = aux;

        ptrA++;
        ptrB++;
    }   

    // REMOVIDO:
    // Range_t *l = cm[a];
    // cm[a] = cm[b];
    // cm[b] = l;

    /* Troca os termos independentes */
    Range_t it = vit[a];
    vit[a] = vit[b];
    vit[b] = it;
}

/*
    Função que encontra o maior elemento(intervalo) da coluna no Sistema Linear.
    Parametros:
        cm: Matriz de coeficientes
        l: Linha atual
        n: Tamanho da matriz
*/
/* OTIMIZAÇÃO: Alteração na forma de acesso da matriz, agora ela é alocada como um vetor */
unsigned int findMaxPivot(Range_t *cm, unsigned int l, unsigned int n)
{
    /* Considera o primeiro termo como o maior */
    Range_t max = cm[l*n + l];

    /* O pivo está na primeira linha */
    unsigned int pivot = l;

    /* Procura por um pivô maior */
    for(int i = l+1; i < n ; i++)
    {
        
        if(fabs(cm[i*n + l].smallest) > fabs(max.largest)) // Correção 1
        {
            max = cm[i*n + l];
            pivot = i;
        }
    }

    /* Retorna em qual linha está o pivo */
    return pivot;
}

/*
    Função que aplica a Eliminação-Gaussiana com Pivoteamento Parcial.
    Parametros:
        LS: Matriz de coeficientes e termos independentes
        n: Ordem da matriz
*/ 
void classicEliminationWithPivot(LinearSystem_t *LS, unsigned int n)
{
    /* Inicia na primeira linha e primeira coluna */
    for(int i = 0; i < n ; ++i)
    {
         /* Pivoteamento parcial */
        unsigned int lPivot = findMaxPivot(LS->cm,i,n);
        if( i != lPivot )
            swapLines(LS->cm,LS->vit,i,lPivot, n);
        
        /* Vai para a próxima linha */
        for(int k = i+1; k < n; ++k)
        {
            /* Calcula o multiplicador */
            Range_t m = divisionRange(LS->cm[k*n + i], LS->cm[i*n + i]);
            /* Para garantir que o elemento abaixo do pivo será zero */
            LS->cm[k*n + i].smallest = 0.0;
            LS->cm[k*n + i].largest = 0.0;
        
            /* Subtrai de cada elemento da equação a multiplicação
             * desde elemento com o multiplicador calculado */
            for(int j = i+1; j < n; ++j)
                LS->cm[k*n + j] = subtractRange(LS->cm[k*n + j], timeRange(m,LS->cm[i*n + j]));
            
            LS->vit[k] = subtractRange(LS->vit[k], timeRange(m,LS->vit[i]));
        }
    }
}

/*
    Função que resolve Sistema Triangulares Superiores.
    Parametros:
        LS: Matriz de coeficientes e termos independentes
        x: Vetor de soluções
        n: Ordem da matriz
*/ 
void retroSubstitution(LinearSystem_t *LS, Range_t *x, int n)
{
    /* Começa na última linha */    
    for(int i = n-1; i >= 0; --i)
    {
        /* Atribui o valor da solução*/
        x[i] = LS->vit[i];

        /* Vai para a posição de um indíce acima e subtrai
         * os valores já calculados */
        for(int j = i+1; j < n; ++j)
            x[i] = subtractRange(x[i], timeRange(LS->cm[i*n + j],x[j]));

        /* Divide pelo coeficiente*/
        // if( (LS->cm[i][i].smallest != 0.0) && (LS->cm[i][i].largest != 0.0)) // Correção 2
            x[i] = divisionRange(x[i], LS->cm[i*n + i]);
    }
}


/*
    Função que aloca um vetor de intervalos.
    Parametros:
        size: Tamanho do vetor
    Códigos de Erro:
        6 - Erro ao alocar vetor de intervalos.
*/
Range_t *allocateArrayRange(int size)
{
    // Aloca memória para o vetor de Range_t
    Range_t *array = (Range_t *)malloc(size * sizeof(Range_t));

    memset(array, 0, size * sizeof(Range_t));

    // Verifica se a alocação foi bem-sucedida
    if (array == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor de intervalos.\n");
        exit(6);
    }

    return array;
}

/*
    Função que realiza o cálculo do Resíduo.
    Parametros:
        vpr: Tabela de Pontos em intervalos
        a: Vetor de coeficientes em intervalos 
        num_points: Número de pontos
        sizeLS: Tamanho do Sistema Linear
*/
Range_t* calculateResidualVector(PointsRange_t *vpr, Range_t *a,long long int num_points, int sizeLS)
{
    int i,j;
    Range_t *rv;
    Range_t fx, res;

    rv = allocateArrayRange(num_points);

    for(i = 0; i < num_points; i++)
    {
        fx.smallest = 0.0;
        fx.largest = 0.0;
        /* Calcula f(xi) */
        for(j = 0; j < sizeLS; j++)
        {
            res = timeRange(a[j], powerRange(vpr->x[i], j)); //  multiplica coef a com xi^j
            fx = addRange(fx, res); // acumulação do polinômio f(x)
        }
        rv[i] = subtractRange(vpr->y[i], fx); // realiza a subtração
    }
    return rv;
}

/*
    Função que libera memória alocada para um Sistema Linear.
    Parametros:
        LS: Sistema linear
        size: Tamanho do Sistema Linear
*/
/* OTIMIZAÇAO: Removido a variavel size */
void freeLinearSystem(LinearSystem_t *LS)
{
    free(LS->cm);
    free(LS->vit);
    free(LS);
}