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
double findLargest(double num)
{
    return nextafter(num, INFINITY);
}

/*
    Função auxiliar que encontra o próximo número real representável, menor ou igual ao número real informado.
    Parametros:
        num: Número real base
*/
double findSmallest(double num)
{
    return nextafter(num, -INFINITY);
}

/*
    Função que cria um intervalo referente a um número real.
    Parametros:
        num: Número real do qual se deseja criar o intervalo
*/
Range_t createRange(double num)
{
    Range_t range;

    range.largest = findLargest(num);
    range.smallest = findSmallest(num);

    return range;
}


/*  Função que faz a soma de dois intervalos.
    [a,b] + [c,d] = [a+c, b+d] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t addRange(Range_t num1, Range_t num2)
{
    Range_t res;

    res.smallest = findSmallest(num1.smallest + num2.smallest);
    res.largest = findLargest(num1.largest + num2.largest);
    
    return res;
}



/*  Função que faz a subtração de dois intervalos.
    [a,b] - [c,d] = [a-d, b-c] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t subtractRange(Range_t num1, Range_t num2)
{
    Range_t res;

    res.smallest = findSmallest(num1.smallest - num2.largest);
    res.largest = findLargest(num1.largest - num2.smallest);
    
    return res;
}

/*  Função que faz a multiplicação de dois intervalos.
    [a,b] * [c,d] = [min{a*c,a*d,b*c,b*d} , max{a*c,a*d,b*c,b*d}] 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t timeRange(Range_t num1, Range_t num2)
{
    Range_t res;
    
    double m1, m2, m3, m4;

    m1 = num1.smallest * num2.smallest;
    m2 = num1.smallest * num2.largest;
    m3 = num1.largest * num2.smallest;
    m4 = num1.largest * num2.largest;

    res.smallest = findSmallest(fmin(fmin(m1, m2), fmin(m3,m4)));
    res.largest = findLargest(fmax(fmax(m1, m2), fmax(m3,m4)));
    
    return res;
}

/*
    Função auxiliar para inverter e consequentemente trocar posição do maior valor com o menor.
    Parametros:
        num: Intervalo que se deseja inveter
*/
Range_t invertRange(Range_t num)
{
    Range_t res;
    
    res.smallest = 1 / num.largest;
    res.largest = 1 / num.smallest;
    
    return res;
}

/*  Função que faz a divisão de dois intervalos.
    [a,b] * [1/d,1/c], se 0 não pertence ao intervalo 
    Parametros:
        num1: Numerador
        num2: Denominador
*/
Range_t divisionRange(Range_t num1, Range_t num2)
{
    Range_t res, aux;
    
    if(((num2.smallest) <= 0) && ((num2.largest) >= 0) ) // Correção
    {
        res.largest = INFINITY;
        res.smallest = -INFINITY;
    }
    else
    {
        aux = invertRange(num2);
        res = timeRange(num1, aux);
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
    if(p == 0)
    {
        res.smallest = 1;
        res.largest = 1;
        return res;
    }

    // Caso 3,4,5
    if((p % 2) == 0)
    {
        // Caso 3 (p é par e a >= 0)
        if(num.smallest >= 0)
        {
            res.smallest = pow(num.smallest, p);
            res.largest = pow(num.largest, p);
            return res;
        }

        // Caso 4 (p é par e b < 0)
        if(num.largest < 0)
        {
            res.smallest = pow(num.largest, p);
            res.largest = pow(num.smallest, p);
            return res;
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
    
    return res;
}

/*
    Função que faz a alocação da struct PointsRange_t, que representa a tabela de pontos de intervalos.
    Parametros:
        size: Tamanho da tabela (quantidade de pontos)
*/
PointsRange_t *allocatePointsRangeArray(long long int size)
{
    PointsRange_t *array = (PointsRange_t *)malloc(size * sizeof(PointsRange_t));

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
            vpr[i].x = createRange(x);
            vpr[i].y = createRange(y);
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
        printf("x: [%lf, %lf]\n", intervals[i].x.smallest, intervals[i].x.largest);
        printf("y: [%lf, %lf]\n", intervals[i].y.smallest, intervals[i].y.largest);
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