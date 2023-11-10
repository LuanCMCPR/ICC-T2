/*
    Luan Carlos Maia Cruz - GRR20203891
    Leonardo Marin Mendes Martin - GRR20205675
*/

#include <stdio.h>
#include "libTime.h"
#include "libSL.h"
#include <stdlib.h>
#include <likwid.h>
#include <likwid-marker.h>

int main()
{
    long long int num_points;
    short int degreeN, sizeLS; // int8_t ??
    RangeVector x,y,sl,b;

    LIKWID_MARKER_INIT;
    /* Le o grau do poliônimo, guarda tamanho do sistema linear, 
       Le a quantidade de pontos e gerar a tabela de intervalos 
    */
    if((scanf("%d", &degreeN) == 1) && (scanf("%d", &num_points) == 1))
    {
        if((degreeN > 0) && (num_points > 0))
        {
            sizeLS = degreeN + 1;
            x = generatePointsRanges(num_points); 
        }
        else
        {
            fprintf(stderr,"Erro no número de pontos ou no grau do polinômio, que deve ser maior que zero\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr,"Erro ao ler grau e o numero de pontos\n");
        exit(2);
    }


}