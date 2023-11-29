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

/*
    Função auxiliar que encontra o próximo número real representável, maior ou igual ao número real informado.
    Parametros:
        num: Número real base
*/
/* OTIMIZAÇÃO : Função removida, nextafter pode ser chamado dentro das operações intervalares
double nextafter(double num)
{
    return nextafter(num, INFINITY);
}
*/

/*
    Função auxiliar que encontra o próximo número real representável, menor ou igual ao número real informado.
    Parametros:
        num: Número real base
*/
/* OTIMIZAÇÃO : Função removida, nextafter pode ser chamado dentro das operações intervalares
double nextafter(double num)
{
    return nextafter(num, -INFINITY);
}
*/

/*
    Função que cria um intervalo referente a um número real.
    Parametros:
        num: Número real do qual se deseja criar o intervalo
*/
Range_t createRange(double num)
{
    Range_t range;

    range.smallest = nextafter(num, -INFINITY);
    range.largest = nextafter(num, INFINITY);

    return range;
}


/*  Função que faz a soma de dois intervalos.
    [a,b] + [c,d] = [a+c, b+d] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
/* OTIMIZAÇÃO: Uso de inline para evitar as sucessivas chamadas da função */
 Range_t addRange(Range_t num1, Range_t num2)
{
    Range_t res;

    res.smallest = nextafter(num1.smallest + num2.smallest, -INFINITY);
    res.largest = nextafter(num1.largest + num2.largest, INFINITY);
    
    return res;
}

/*  Função que faz a subtração de dois intervalos.
    [a,b] - [c,d] = [a-d, b-c] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
/* OTIMIZAÇÃO: Uso de inline para evitar as sucessivas chamadas da função */
 Range_t subtractRange(Range_t num1, Range_t num2)
{
    Range_t res;

    res.smallest = nextafter(num1.smallest - num2.largest, -INFINITY);
    res.largest = nextafter(num1.largest - num2.smallest, INFINITY);
    
    return res;
}

/*  Função que faz a multiplicação de dois intervalos.
    [a,b] * [c,d] = [min{a*c,a*d,b*c,b*d} , max{a*c,a*d,b*c,b*d}] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
/* OTIMIZAÇÃO: Uso de inline para evitar as sucessivas chamadas da função */
 Range_t timeRange(Range_t num1, Range_t num2)
{
    Range_t res;
    
    double m1, m2;

    m1 = num1.smallest * num2.smallest;
    // m2 = num1.smallest * num2.largest;
    // m3 = num1.largest * num2.smallest;
    m2 = num1.largest * num2.largest;

    res.smallest = nextafter(m1, -INFINITY);
    res.largest = nextafter(m2, INFINITY);
    
    // res.smallest = nextafter(fmin(fmin(m1, m2), fmin(m3,m4)), -INFINITY);
    // res.largest = nextafter(fmax(fmax(m1, m2), fmax(m3,m4)), INFINITY);

    return res;
}

/*
    Função auxiliar para inverter e consequentemente trocar posição do maior valor com o menor.
    Parametros:
        num: Intervalo que se deseja inveter
*/
// Range_t invertRange(Range_t num)
// {
//     Range_t res;
    
//     res.smallest = 1 / num.largest;
//     res.largest = 1 / num.smallest;
    
//     return res;
// }
// Comentário:
// Removido, invertRange pode ser chamado dentro das operações intervalares

/*  Função que faz a divisão de dois intervalos.
    [a,b] * [1/d,1/c], se 0 não pertence ao intervalo 
    Parametros:
        num1: Numerador
        num2: Denominador
*/
Range_t divisionRange(Range_t num1, Range_t num2)
{
    Range_t res;
    
    if(((num2.smallest) <= 0) && ((num2.largest) >= 0) ) // Correção
    {
        res.largest = INFINITY;
        res.smallest = -INFINITY;
    }
    else
    {
        // aux.largest = 1 / num2.smallest;
        // aux.smallest = 1 / num2.largest;
        // res = timeRange(num1, aux);
        res.largest = num1.largest * 1 / num2.smallest;
        res.smallest = num1.smallest * 1 / num2.largest; 
        
    }
    
    return res;
}


/*  Função que faz a potência de um intervalo.
    [a,b]p = [1,1]     se p = 0
    [ap,bp]            se p é ímpar
    [ap,bp]            se p é par e a ≥ 0
    [bp,ap]            se p é par e b < 0
    [0,max{ap,bp}]     se p é par e a < 0 ≤ b
    Parametros:
        num: Intervalo
        p: Potência que se deseja elevar o intervalo num
*/
Range_t powerRange(Range_t num, long long int p)
{
    Range_t res;

    // Caso 1 (p é 0)
    // REMOVIDO:
    // if(p == 0)
    // {
    //     res.smallest = 1;
    //     res.largest = 1;
    //     return res;
    // }

    // Caso 3,4,5
    if((p % 2) == 0)
    {
        // Caso 3 (p é par e a >= 0)
        if(num.smallest >= 0)
        {
            res.smallest = pow(num.smallest, p);
            res.largest = pow(num.largest, p);

            // for(int i = 1; i <= p, i++)
            // {
            //     res.smallest *= num.smallest;
            //     res.largest *= num.largest;
            // }
            
            return res;
        }

        // Caso 4 (p é par e b < 0)
        if(num.largest < 0)
        {
            res.smallest = pow(num.largest, p);
            res.largest = pow(num.smallest, p);
            return res;

            // for(int i = 1; i <= p, i++)
            // {
            //     res.smallest *= num.largest * p;
            //     res.largest *= num.smallest * p;
            // }
        }

        // Já testou se  num.smallest(a) é >= 0, se chegou aqui é < 0
        // Já testou se num.largest(b) é < 0, se chegou aqui é >= 0
        // eliminamos dois testes que seriam    
        // "if(num.smallest < 0 && num.largest >= 0)"
    
        res.smallest = 0;
        res.largest = fmax(pow(num.smallest, p), pow(num.largest,p));
        return res; 
    }
    
    // Caso 2 (p é ímpar)
    res.smallest = pow(num.smallest, p);
    res.largest = pow(num.largest, p);

     // for(int i = 1; i <= p, i++)
    // {
    //     res.smallest *= num.smallest;
    //     res.largest *= num.largest;
    // }
    
    return res;
}

/*
    Função que faz a alocação da struct PointsRange_t, que representa a tabela de pontos de intervalos.
    Parametros:
        size: Tamanho da tabela (quantidade de pontos)
*/
/* OTIMIZAÇÃO: Alteração na forma de alocação da struct, pois passou a ser um SoA, não impacta na Geração, Solução ou Resíduo 
   é apenas para apresentar a modificação
 */
PointsRange_t *allocatePointsRangeArray(long long int size)
{
    PointsRange_t *array = (PointsRange_t *) malloc(sizeof(PointsRange_t));
    
    array->x = (Range_t *) malloc(size * sizeof(Range_t));
    array->y = (Range_t *) malloc(size * sizeof(Range_t));
    

    if (array == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o vetor de PointsRange_t.\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

/*
    Função que gera os intervalos para cada ponto da tabela de pontos de intervalos.
    Parametros:
        numPoints: Quantidade de pontos
*/
PointsRange_t *generatePointsRanges(long long int numPoints)
{
    PointsRange_t *vpr = allocatePointsRangeArray(numPoints);

    double x, y;

    for(long long int i = 0; i < numPoints; i++)
    {
        if(scanf("%lf %lf", &x, &y) == 2)
        {
            vpr->x[i] = createRange(x);
            vpr->y[i] = createRange(y);
        }
    }
    
    return vpr;
}

/* 
    Função que imprime o vetor de intervalos de pontos 
    Observação: Não é utilizada, utilizamos apenas para testar o programa
*/
void printIntervals(PointsRange_t *intervals, long long int n)
{
    for(long long int i = 0; i < n; i++)
    {
        printf("Intervalo %lld:\n", i + 1);
        printf("x: [%lf, %lf]\n", intervals->x[i].smallest, intervals->x[i].largest);
        printf("y: [%lf, %lf]\n", intervals->y[i].smallest, intervals->y[i].largest);
        printf("\n");
    }
}

/*
    Função que imprime vetores de intervalos.
    Parametros:
        array: Vetor de intervalos
*/
void printArrayRange(Range_t *array, long long int size)
{
    for(long long int i = 0; i < size; i++)
    {
        printf("[%1.8e,%1.8e] ", array[i].smallest, array[i].largest);
    }
    printf("\n");
}