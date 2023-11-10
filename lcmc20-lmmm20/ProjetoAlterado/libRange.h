
/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675

    Biblioteca de Aritmética Intervalar e Representação Númerica 
*/


/* Struct que representa os vetores com valores em seus intervalos */
typedef struct
{
    double *xs;  /* Menores valores do Intervalo de X */
    double *xl;  /* Maiores valores do Intervalo de X */
    double *ys;  /* Menores valores do Intervalo de Y */
    double *yl;  /* Maiores valores do Intervalo de Y */
    double *sls; /* Menores valores do Intervalo do Y */
    double *sll; /* Maiores valores do Intervalo do SL*/
    double *bs;  /* Menores valores do Intervalo do VTI */
    double *bl;  /* Maiores valores do Intervalo do VTI */
} VRanges;

