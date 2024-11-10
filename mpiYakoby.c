#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1024

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("argc");
	       	return 4;
	}
	double co = 0, count = 0;
	int np, rank, kit;
	kit = strtol(argv[1], NULL, 10);
        srand(time(NULL));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Status status;

        
        double *S = (double*)malloc((N * N / np) * sizeof(double));
	double *Snew = (double*)malloc((N * N / np) * sizeof(double));
	double *fv = (double*)malloc((N - 2) * sizeof(double));
	double *fn = (double *)malloc((N - 2) * sizeof(double));
	for (int i = 0; i < N * (N / np); i++){
                S[i] = (double)rand() / RAND_MAX;
		Snew[i] = S[i];
	}
	if (rank == 0){
		for (int j = 0; j < kit; j++){
		        MPI_Sendrecv(S + N * (N / np - 1) + 1, N - 2, MPI_DOUBLE, 1, 0, fn, N - 2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int g = 1; g < N / np - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			}
			for (int l = 1; l < N - 1; l++){
				Snew[(N / np - 1) * N + l] = (S[(N / np - 1) * N + l - 1] + S[(N / np - 1) * N + l + 1] + fn[l - 1] + fn[l + 1]) / 4;
			}
			for (int y = 0; y < N * (N / np); y++){
				if (j == kit - 1){
					count += (S[y] - Snew[y]) * (S[y] - Snew[y]);
				}
				S[y] = Snew[y];
			}

		}
	}
	else if (rank == np - 1){
		for (int j = 0; j < kit; j++){
		        MPI_Sendrecv(S + 1, N - 2, MPI_DOUBLE, np - 2, 0, fv, N - 2, MPI_DOUBLE, np - 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int g = 1; g < N / np - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			}
			for (int l = 1; l < N - 1; l++){
				Snew[l] = (S[l - 1] + S[l + 1] + fv[l - 1] + fv[l + 1]) / 4;
			}
			for (int y = 0; y < N * (N / np); y++){
				if (j == kit - 1){
                                        count += (S[y] - Snew[y]) * (S[y] - Snew[y]);
                                }
				S[y] = Snew[y];
			}
		}
	}
	else
	{
	        for (int j = 0; j < kit; j++){
		         MPI_Sendrecv(S + 1, N - 2, MPI_DOUBLE, rank - 1, 0, fv, N - 2, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                         MPI_Sendrecv(S + N * (N / np - 1) + 1, N - 2, MPI_DOUBLE, rank + 1, 0, fn, N - 2, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                         for (int l = 1; l < N - 1; l++){
				Snew[l] = (S[l - 1] + S[l + 1] + fv[l - 1] + fv[l + 1]) / 4;
			 }
			 
			 for (int g = 1; g < N / np - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			 } 
			 for (int l = 1; l < N - 1; l++){
				Snew[(N / np - 1) * N + l] = (S[(N / np - 1) * N + l - 1] + S[(N / np - 1) * N + l + 1] + fn[l - 1] + fn[l + 1]) / 4;

			 }
			 for (int y = 0; y < N * (N / np); y++){
				if (j == kit - 1){
                                        count += (S[y] - Snew[y]) * (S[y] - Snew[y]);
                                }
				S[y] = Snew[y];
			 }
		}
	}
        MPI_Reduce(&count, &co, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        free(S);
	free(Snew);
	free(fv);
	free(fn);
	if (rank == 0){
            printf("norma: %lf\n", sqrt(co));
	}
	MPI_Finalize();
	return 0;
}
