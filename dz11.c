#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 20000

int main(int argc, char **argv){
	int rank, size, row_rank, row_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int dim[2];
	switch (size){
		case 1: dim[0] = 1; dim[1] = 1; break;
		case 2: dim[0] = 2; dim[1] = 1; break;
		case 4: dim[0] = 2; dim[1] = 2; break;	
		case 6: dim[0] = 3; dim[1] = 2; break;	
		case 8: dim[0] = 4; dim[1] = 2; break;	
		case 12: dim[0] = 4; dim[1] = 3; break;
	        case 16: dim[0] = 4; dim[1] = 4; break;
	}
	srand(time(NULL));
	int Nx = N / dim[1];
	int Ny = N / dim[0];
	double *A = (double*)malloc(Nx * Ny * sizeof(double));
	double *c = (double*)calloc(Ny, sizeof(double));
	for (int i = 0; i < Nx * Ny; i++){
		A[i] = (double)rand() / RAND_MAX;
		//A[i] = 1.0;
	}
	MPI_Comm row_Comm;
	int color1 = rank / dim[0];
	MPI_Comm_split(MPI_COMM_WORLD, color1, rank, &row_Comm);

	MPI_Comm_rank(row_Comm, &row_rank);
        MPI_Comm_size(row_Comm, &row_size);

	double* b;
        MPI_Win win;

        MPI_Win_allocate(N * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &b, &win);
	if (rank == 0){
		//double *ofs = (double*)malloc(N * sizeof(double));
		for (int i = 0; i < N; i++){
			b[i] = (double)rand() / RAND_MAX;
			//b[i] = 1.0;
		}
		//MPI_Put(ofs, N, MPI_DOUBLE, 0, 0, N, MPI_DOUBLE, win);
		//double *u = (double*)calloc(N, sizeof(double));
		
	}
        MPI_Win_fence(0, win);
        double t = MPI_Wtime();

	double *h = (double*)malloc(Nx * sizeof(double));
	MPI_Get(h, Nx, MPI_DOUBLE, 0, Nx * row_rank, Nx, MPI_DOUBLE, win);
	MPI_Win_fence(0, win);
        if (rank == 0){
		for (int i = 0; i < N; i++){
			b[i] = 0;
		}	
		//MPI_Put(u, N, MPI_DOUBLE, 0, 0, N, MPI_DOUBLE, win);
	}
        for (int i = 0; i < Ny; i++){
	    for (int j = 0; j < Nx; j++){
	        c[i] += A[i * Nx + j] * h[j];
	    }
	}

        MPI_Win_fence(0, win);
	MPI_Win_lock_all(0, win);
	MPI_Accumulate(c, Ny, MPI_DOUBLE, 0, (rank / dim[1]) * Ny, Ny, MPI_DOUBLE, MPI_SUM, win);
        MPI_Win_unlock_all(win);	

	MPI_Win_fence(0, win);
	t = MPI_Wtime() - t;
	if (rank == 0){
            //for (int i = 0; i < N; i++){
	//	    printf("%lf\n", b[i]);
	  //  }
	    printf("time : %lf\n", t);
	}

	MPI_Win_free(&win);
        free(h);	
        free(A);
	free(c);
	MPI_Finalize();
	return 0;
}
