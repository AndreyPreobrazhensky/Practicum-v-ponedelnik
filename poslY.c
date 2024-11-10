#include <stdio.h>
#include "omp.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 4

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("argc");
	       	return 4;
	}
	int kit;
	kit = strtol(argv[1], NULL, 10);
        srand(time(NULL));

        
        double *S = (double*)malloc((N * N) * sizeof(double));
	double *Snew = (double*)malloc((N * N) * sizeof(double));
	for (int i = 0; i < N * N; i++){
                S[i] = (double)rand() / RAND_MAX;
		Snew[i] = S[i];
	}
	double count = 0;
	double t = omp_get_wtime();
	{
	        for (int j = 0; j < kit; j++){
		         
			 for (int g = 1; g < N - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			 } 
			 for (int y = 0; y < N * N; y++){
				if (j == kit - 1){
					count += (S[y] - Snew[y]) * (S[y] - Snew[y]);
				}	
				S[y] = Snew[y];
			 }
		}
	}
	t = omp_get_wtime() - t;

        free(S);
	free(Snew);
	printf("time: %lf\n count: %lf\n", t, sqrt(count));
	return 0;
}
