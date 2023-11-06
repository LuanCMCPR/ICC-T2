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
    int degreeN, num_points, sizeLS;
    double tGeraSL, tSolSL, tResSL;
    PointsRange_t *vpr;
    LinearSystem_t *LS;
    Range_t *a, *r;

    LIKWID_MARKER_INIT;
    /* Le o grau do poliônimo, guarda tamanho do sistema linear, 
       Le a quantidade de pontos e gerar a tabela de intervalos 
    */
    if((scanf("%d", &degreeN) == 1) && (scanf("%d", &num_points) == 1))
    {
        if((degreeN > 0) && (num_points > 0))
        {
            sizeLS = degreeN + 1;
            vpr = generatePointsRanges(num_points); 
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

    /* Gera o sistema linear */
    LIKWID_MARKER_START("GERANDO_SISTEMA");
    tGeraSL = timestamp(); 
    LS = createLinearSystem(vpr, num_points, sizeLS);
    tGeraSL = timestamp() - tGeraSL;
    LIKWID_MARKER_STOP("GERANDO_SISTEMA");
    
    /* Resolve o sistema linear */
    a = allocateArrayRange(sizeLS);
    LIKWID_MARKER_START("SOLUCAO_SISTEMA");
    tSolSL = timestamp();
    classicEliminationWithPivot(LS, sizeLS);
    retroSubstitution(LS, a, sizeLS);
    tSolSL = timestamp() - tSolSL;
    LIKWID_MARKER_STOP("SOLUCAO_SISTEMA");

    /* Faz o calculo do resíduo, imprime coeficientes, resíduo e tempos de geração e solução */
    LIKWID_MARKER_START("CALCULO_RESIDUO");
    tResSL = timestamp();
    r = calculateResidualVector(vpr, a, num_points, sizeLS);
    tResSL = timestamp() - tResSL;
    LIKWID_MARKER_STOP("CALCULO_RESIDUO");

    printArrayRange(a, sizeLS);
    printArrayRange(r, num_points);
    printf("%1.8e\n", tGeraSL);
    printf("%1.8e\n", tSolSL);
    printf("%1.8e\n", tResSL);

    /* Libera memória alocada*/
    free(vpr);
    free(a);
    free(r);
    freeLinearSystem(LS, sizeLS);

    LIKWID_MARKER_CLOSE;
    return 0;

}