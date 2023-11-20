/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675
*/

/* Lib de Aritmética Intervalar e Representação Númerica */

/* Struct que representa o Intervalo */
typedef struct
{
    double smallest; /* Menor valor do Intervalo */
    double largest;  /* Maior valor do Intervalo */
} Range_t;

/* Struct que representa a tabela de pontos de Intervalos */
/* OTIMIZAÇÃO: Mudança de um AoS para uma SoA */
typedef struct
{
    Range_t *x;  /* Vetor de Intervalos que representa xi's */
    Range_t *y;  /* Vetor de Intervalos que representa yi's */
} PointsRange_t;


/*
    Função que cria um intervalo referente a um número real
    Parametros:
        num: Número real do qual se deseja criar o intervalo
*/
Range_t createRange(double num);

/*  Função que faz a soma de dois intervalos
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t addRange(Range_t num1, Range_t num2);

/*  Função que faz a subtração de dois intervalos 
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t subtractRange(Range_t num1, Range_t num2);

/*  Função que faz a multiplicação de dois intervalos
    Parametros:
        num1: Primeiro intervalo
        num2: Segundo Intervalo
*/
Range_t timeRange(Range_t num1, Range_t num2);


/*  Função que faz a divisão de dois intervalos
    [a,b] * [1/d,1/c], se 0 não pertence ao intervalo 
    Parametros:
        num1: Numerador
        num2: Denominador
*/
Range_t divisionRange(Range_t num1, Range_t num2);

/*  Função que faz a potência de um intervalo
    Parametros:
        num: Intervalo
        p: Potência que se deseja elevar o intervalo num
*/
Range_t powerRange(Range_t num, long long int p);

/*
    Função que faz a alocação da struct PointsRange_t, que representa a tabela de pontos de intervalos
    Parametros:
        size: Tamanho da tabela (quantidade de pontos)
*/
PointsRange_t *allocatePointsRangeArray(long long int size);

/*
    Função que gera os intervalos para cada ponto da tabela de pontos de intervalos 
    Parametros:
        numPoints: Quantidade de pontos
*/
PointsRange_t *generatePointsRanges(long long int numPoints);

/* 
    Função que imprime o vetor de intervalos de pontos 
    Observação: Não é utilizada, utilizamos apenas para testar o programa
*/
void printIntervals(PointsRange_t *intervals, long long int n);

/*
    Função que imprime vetores de intervalos    
    Parametros:
        array: Vetor de intervalos
*/
void printArrayRange(Range_t *vector, long long int size);
