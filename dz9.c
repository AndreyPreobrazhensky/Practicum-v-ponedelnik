#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

#define N 600
#define B_SIZE 10

int main(int argc, char **argv){
	int rank, size, row_rank, row_size, col_rank, col_size;
	MPI_Comm row_Comm, col_Comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
        int len = (int)(sqrt(size));

	int color1 = rank % len;
	int color2 = rank / len;
    
        MPI_Comm_split(MPI_COMM_WORLD, color1, rank, &row_Comm);
        MPI_Comm_split(MPI_COMM_WORLD, color2, rank, &col_Comm);

        MPI_Comm_rank(row_Comm, &row_rank);
        MPI_Comm_size(row_Comm, &row_size);
    
        MPI_Comm_rank(col_Comm, &col_rank);
        MPI_Comm_size(col_Comm, &col_size);

	int *A = (int*)malloc((N * N / size) * sizeof(int));
	int *B = (int*)malloc((N * N / size) * sizeof(int));
        int *C = (int*)malloc((N * N / size) * sizeof(int));

	int *recv_row = (int*)malloc((N * B_SIZE / len) * sizeof(int));
	int *recv_col = (int*)malloc((N * B_SIZE / len) * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < N * N / size; i++){
		A[i] = rand() % 10;
		B[i] = rand() % 10;
		C[i] = 0;
	}

	double t_parallel = MPI_Wtime();
	for (int k = 0; k < N / B_SIZE; k++){
			if (row_rank == k / (N / B_SIZE / len)){
			    for (int l = 0; l < N * B_SIZE / len; l++){
				    recv_row[l] = B[k % (N / len / B_SIZE) * (N / len) + l];
			    }

			}
			MPI_Bcast(recv_row, B_SIZE * N / len, MPI_INT, k / (N / len), row_Comm);
	                if (col_rank == k / (N / B_SIZE / len)){
			    for (int l = 0; l < N * B_SIZE / len; l++){
				    recv_col[l] = A[k % (N / len / B_SIZE) * B_SIZE + (l / B_SIZE) * (N / len) + l % B_SIZE];
			    }
			}
			MPI_Bcast(recv_col, B_SIZE * N / len, MPI_INT, k / (N / len), col_Comm);
			for (int x = 0; x < N / len; x++){
				for (int y = 0; y < N / len; y++){
					int count = 0;
					for (int h = 0; h < B_SIZE; h++){
						count += recv_col[y * B_SIZE + h] * recv_row[h * (N / len) + x];
					}
					C[x * (N / len) + y] += count;
				}
			}
	}
	if (rank == 0){
	    printf("time parallel: %lf\n", MPI_Wtime() - t_parallel);
	}
        free(A);
	free(B);
	free(C);
	free(recv_row);
	free(recv_col);

	MPI_Finalize();
}





