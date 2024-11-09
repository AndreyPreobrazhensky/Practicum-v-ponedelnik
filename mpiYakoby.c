#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define N 2048

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("argc");
	       	return 4;
	}
	int np, rank, kit;
	kit = strtol(argv[1], NULL, 10);
        srand(time(NULL));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Status status;

        if (rank == 0){
		printf("Hello from 1\n");

	}
        else
	{
		printf("Hello else\n");
	}
        double *S = (double*)malloc((N * N / np) * sizeof(double)); // create massive
	double *Snew = (double*)malloc((N * N / np) * sizeof(double));
	double *fv = (double*)malloc((N - 2) * sizeof(double));
	double *fn = (double *)malloc((N - 2) * sizeof(double));
	for (int i = 0; i < N * N / np; i++){
                S[i] = (double)rand() / 1000;
	}
 
	if (rank == 0){
		for (int j = 0; j < kit; j++){
			printf("iter in 0\n");
		        MPI_Send(S + N * (N / np - 1) + 1, N - 2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(fn, N - 2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
			for (int g = 1; g < N / np - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			}
			for (int l = 1; l < N - 1; l++){
				Snew[(N / np - 1) * N + l] = (S[(N / np - 1) * N + l - 1] + S[(N / np - 1) * N + l + 1] + fn[l - 1] + fn[l + 1]) / 4;
			}
			for (int y = 0; y < N * N / np; y++){
				S[y] = Snew[y];
			}

		}
	}
	else if (rank == np - 1){
		for (int j = 0; j < kit; j++){
			printf("iter in %d\n", np - 1);
		        MPI_Send(S + 1, N - 2, MPI_DOUBLE, np - 2, 0, MPI_COMM_WORLD);
			MPI_Recv(fv, N - 2, MPI_DOUBLE, np - 2, 0, MPI_COMM_WORLD, &status);
			for (int g = 1; g < N / np - 1; g++){
				for (int l = 1; l < N - 1; l++){
					Snew[g * N + l] = (S[g * N + l - 1] + S[g * N + l + 1] + S[(g - 1) * N + l] + S[(g + 1) * N + l]) / 4;
				}
			}
			for (int l = 1; l < N - 1; l++){
				Snew[l] = (S[l - 1] + S[l + 1] + fv[l - 1] + fv[l + 1]) / 4;
			}
			for (int y = 0; y < N * N / np; y++){
				S[y] = Snew[y];
			}
		}
	}
	else
	{
	        for (int j = 0; j < kit; j++){
			printf("iter in %d\n", rank);
		         MPI_Send(S + 1, N - 2, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
                         MPI_Send(S + N * (N / np - 1) + 1, N - 2, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
			 MPI_Recv(fv, N - 2, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status);
                         MPI_Recv(fn, N - 2, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &status);
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
			 for (int y = 0; y < N * N / np; y++){
				S[y] = Snew[y];
			 }
		}
	}

        free(S);
	free(Snew);
	free(fv);
	free(fn);

	MPI_Finalize();
	return 0;
}
