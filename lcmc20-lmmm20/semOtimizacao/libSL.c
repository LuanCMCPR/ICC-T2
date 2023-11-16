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
LinearSystem_t *allocateLinearSystem(int size)
{
    LinearSystem_t *system;

    system = (LinearSystem_t *)malloc(sizeof(LinearSystem_t));
    if (system == NULL)
    {
        fprintf(stderr, "Erro ao alocar sistema linear.\n");
        exit(3);
    }
    
    /* Aloca a matriz de coeficientes */
    system->cm = (Range_t **)malloc(size * sizeof(Range_t *));
    if (system->cm == NULL)
    {
        fprintf(stderr, "Erro ao alocar matriz de coeficientes.\n");
        free(system);
        exit(4);
    }

    /* Aloca cada linha da matriz de coeficientes */
    for (int i = 0; i < size; i++)
    {
        system->cm[i] = (Range_t *)malloc(size * sizeof(Range_t));
        for(int j = 0; j < size; j++)
        {
            system->cm[i][j].smallest = 0.0;
            system->cm[i][j].largest = 0.0;
        }
        if (system->cm[i] == NULL)
        {
            fprintf(stderr, "Erro ao alocar linha %d da matriz de coeficientes.\n", i);
            for (int j = 0; j < i; j++)
            {
                free(system->cm[j]);
            }
            free(system->cm);
            free(system);
            exit(5);
        }
    }

    /* Alocar o vetor de termos independentes */
    system->vit = allocateArrayRange(size);

    for(int i = 0; i < size; i++)
    {
        system->vit[i].smallest = 0.0;
        system->vit[i].largest = 0.0;
    }

    return system;
}

/*
    Função que gera o Sistema Linear a partir do Método dos Minímos Quadrados.
    Parametros:
        vpr: Tabela de pontos em intervalos
        num_points: Número de pontos
        sizeLS: Tamanho do Sistema Linear
*/
LinearSystem_t *createLinearSystem(PointsRange_t *vpr, long long int num_points, int sizeLS)
{
    int i, j, power_x;
    LinearSystem_t *LS;
    Range_t res;

    LS = allocateLinearSystem(sizeLS);

    /* Cálculo dos coeficientes da primeira linha do Sistema Linear */
    for (i = 0; i < sizeLS; i++)
    {
        LS->cm[0][i].smallest = 0.0;
        LS->cm[0][i].largest = 0.0;

        /* Cálculo dos Somatórios de x^i */
        for (j = 0; j < num_points; j++)
        {
            res = powerRange(vpr[j].x, i);
            LS->cm[0][i].smallest += res.smallest; /* To do: usar addRange() */
            LS->cm[0][i].largest += res.largest;
        }
    }

    LS->vit[0].smallest = 0.0;
    LS->vit[0].largest = 0.0;

    /* Cálculo do coeficiente do Vetor de Termos Independentes da Primeira Linha */
    for(i = 0; i < num_points; i++)
    {
        /* To do: usar addRange() */
        LS->vit[0].smallest += vpr[i].y.smallest; 
        LS->vit[0].largest += vpr[i].y.largest;
    }

    /* Cálculo dos coeficientes das demais linhas */
    /* power_x serve para calcular o coeficiente gerado pelo último somatório da equação */
    power_x = sizeLS;
    for(i = 1; i < sizeLS; i++)
    {
        /* Copia os valores de cima para baixo com base na primeira linha */
        for (j = 0; j < sizeLS-1; j++)
        {
            LS->cm[i][j] = LS->cm[i-1][j+1]; 
        }

        LS->cm[i][sizeLS-1].smallest = 0.0;
        LS->cm[i][sizeLS-1].largest = 0.0;
        
        /* Cálcula o coeficiente do Vetor de Termos Independentes e o último coeficiente da equação */
        for (j = 0; j < num_points; j++)
        {
            /* Calcula o coeficiente de xn do Sistema Linear */
            res = powerRange(vpr[j].x, power_x); // x^k com k = power_x
            LS->cm[i][sizeLS-1].smallest += res.smallest;
            LS->cm[i][sizeLS-1].largest += res.largest;

            /* Calcula o coeficiente do Vetor de Termos Independentes */
            res = powerRange(vpr[j].x, i); // x[j]^i com i = linha
            Range_t mult = timeRange(vpr[j].y, res); // multiplica y[i] com x[j]^i
            LS->vit[i].smallest += mult.smallest;
            LS->vit[i].largest += mult.largest;
        }
        power_x++;
    }

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
        for (j = 0; j < size; j++)
        {
            printf("[%lf, %lf]x%d ", LS->cm[i][j].smallest, LS->cm[i][j].largest, j+1);
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
void swapLines(Range_t **cm, Range_t *vit, unsigned int a, unsigned int b)
{
    /* Troca as linhas da matriz */
    Range_t *l = cm[a];
    cm[a] = cm[b];
    cm[b] = l;

    /* Troca os termos independentes */
    Range_t it = vit[a];
    vit[a] = vit[b];
    vit[b] = it;

    // Comentário:
    // Aqui é feita a troca de linhas da matriz de coeficientes e do vetor de termos independentes.
    // Não tem muito a se alterar aqui, pois é uma função simples
}

/*
    Função que encontra o maior elemento(intervalo) da coluna no Sistema Linear.
    Parametros:
        cm: Matriz de coeficientes
        l: Linha atual
        n: Tamanho da matriz
*/
unsigned int findMaxPivot(Range_t **cm, unsigned int l, unsigned int n)
{
    /* Considera o primeiro termo como o maior */
    Range_t max = cm[l][l];

    /* O pivo está na primeira linha */
    unsigned int pivot = l;

    /* Procura por um pivô maior */
    for(int i = l+1; i < n ; i++)
    {
        if(fabs(cm[i][l].smallest) > fabs(max.largest)) // Correção 1
        {
            max = cm[i][l];
            pivot = i;
        }
    }

    /* Retorna em qual linha está o pivo */
    return pivot;

    // Comentário:
    // Aqui é feita a busca pelo maior elemento da coluna, para realizar o pivoteamento parcial.
    // Não tem muito a se alterar aqui, pois é uma função simples.
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
            swapLines(LS->cm,LS->vit,i,lPivot);
        

        // Comentário:
        // Aqui é feito o pivoteamento parcial, ou seja, a busca pelo maior elemento da coluna e a troca caso seja necessária
        // Talvez utilizar inline pode ser uma opção para evitar a chamada sucessiva das funções findMaxPivot e swapLines.
        // Não tem muito a se alterar aqui, pois é um processo que necessita ser feito para a eliminação gaussiana ser mais precisa no resultado.

        /* Vai para a próxima linha */
        for(int k = i+1; k < n; ++k)
        {
            /* Calcula o multiplicador */
            Range_t m = divisionRange(LS->cm[k][i], LS->cm[i][i]);
            /* Para garantir que o elemento abaixo do pivo será zero */
            LS->cm[k][i].smallest = 0.0;
            LS->cm[k][i].largest = 0.0;

            // Comentário:
            // Neste laço é feito o cálculo do multiplicador e depois a subtração dos elementos da linha atual com a linha do pivo.
            // Os elementos abaixo do pivo são zerados, pois o pivo é o maior elemento da coluna.
            // Talvez utilizar inline para a função divisionRange pode ser uma opção para evitar a chamada da função sucessivamente.
        
            /* Subtrai de cada elemento da equação a multiplicação
             * desde elemento com o multiplicador calculado */
            for(int j = i+1; j < n; ++j)
                LS->cm[k][j] = subtractRange(LS->cm[k][j], timeRange(m,LS->cm[i][j]));
            
            // Comentário:
            // Neste laço é feita a subtração dos elementos da linha atual com a linha do pivo.
            // Talvez utilizar inline para as funções subtractRange e time range pode ser uma opção para evitar a chamada da função sucessivamente.
            // Aqui podemos fazer uso de vetorização ( unroll + jam com o laço de cima)

            LS->vit[k] = subtractRange(LS->vit[k], timeRange(m,LS->vit[i]));

            // Comentário:
            // Aqui é feita a subtração do elemento do vetor de termos independentes da linha atual com o da linha do pivo.
            // É possível utilizar inline para as funções subtractRange e time range pode ser uma opção para evitar a chamada da função sucessivamente.
            // Aqui podemos fazer uso de vetorização ( unroll + jam usando o primeiro laço)
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
            x[i] = subtractRange(x[i], timeRange(LS->cm[i][j],x[j]));

        /* Divide pelo coeficiente*/
        // if( (LS->cm[i][i].smallest != 0.0) && (LS->cm[i][i].largest != 0.0)) // Correção 2
            x[i] = divisionRange(x[i], LS->cm[i][i]);
    }

    // Comentário:
    // Aqui é feita a retrosubstituição, ou seja, a resolução do sistema triangular superior.
    // Talvez utilizar inline para as funções divisionRange, subtractRange e timeRange pode ser uma opção para evitar a chamada da função sucessivamente.
    // Aqui podemos fazer uso de vetorização ( unroll + jam )
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
            res = timeRange(a[j], powerRange(vpr[i].x, j)); //  multiplica coef a com xi^j
            fx = addRange(fx, res); // acumulação do polinômio f(x)
        }
        rv[i] = subtractRange(vpr[i].y, fx); // realiza a subtração
    }
    return rv;
}

/*
    Função que libera memória alocada para um Sistema Linear.
    Parametros:
        LS: Sistema linear
        size: Tamanho do Sistema Linear
*/
void freeLinearSystem(LinearSystem_t *LS, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(LS->cm[i]);
    }
    free(LS->cm);
    free(LS->vit);
    free(LS);
}