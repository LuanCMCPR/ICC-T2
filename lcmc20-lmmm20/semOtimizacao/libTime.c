/*
  Luan Carlos Maia Cruz - GRR20203891
  Leonardo Marin Mendes Martin - GRR20205675
*/


/*
  Observação: Esta biblioteca foi criada apenas pela função de tempo não possuir vínculo com nenhuma outra
  biblioteca 
*/

#include<time.h>

/*
  Função que calcula o tempo de execução de um determinado trecho do programa
  Retorno:
    Tempo de execução em milisegundos
*/
double timestamp()
{
  struct timespec tp;

  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

  return ( (double) tp.tv_sec*1.0e3 + (double) tp.tv_nsec*1.0e-6 );
}