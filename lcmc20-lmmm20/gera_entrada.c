#include <stdio.h>
#include <stdlib.h>

#define RUIDO (((double)rand())/(K*(double)RAND_MAX))
#define PASSO (1.0/(double)K)

double f(double x, int N, long long int K, double *a) {
	double fx = a[0] + RUIDO;
	double px = 1;
	for (int i = 1; i <= N; ++i) {
		px *= x;
		fx += a[i]*px;
	}
	return fx;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("uso: %s <K>, onde <K> é a quantidade de pontos\n", argv[0]);
		return 1;
	}

	srand(2023-2);

	int N = 4;			// grau do polinomio
	double a[5] = {	0.00129,	// coeficientes do polinomio base
			0.0001,
			0.23e-4,
			0.57e-5,
			0.96e-6};

	long long int K = atoll(argv[1]);

	printf("%d\n", N);
	printf("%lld\n", K);
	double x = RUIDO;
	for (long long int i = 0; i < K; ++i) {
		printf("%1.9e %1.9e\n", x, f(x,N,K,a));
		x += PASSO;
	}
}
