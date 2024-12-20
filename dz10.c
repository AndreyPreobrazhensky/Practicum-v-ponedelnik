#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

#define N 240
#define N_ITER 50

int main(int argc, char **argv){
	int rank, size, Nx, Ny, Nz;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

        MPI_Comm comm;
        int dim[3], period[3], reorder;
        int coord[3], id;
        period[0] = 0; period[1] = 0; period[2] = 0;
	switch (size){
		case 2: dim[0] = 1; dim[1] = 1; dim[2] = 2; break;
		case 8: dim[0] = 2; dim[1] = 2; dim[2] = 2; break;
		case 12: dim[0] = 2; dim[1] = 3; dim[2] = 2; break;	
		case 16: dim[0] = 2; dim[1] = 4; dim[2] = 2; break;
	}
	MPI_Dims_create(size, 3, dim);
	MPI_Cart_create(MPI_COMM_WORLD, 3, dim, period, reorder, &comm);

        int cart_rank;

        MPI_Comm_rank(comm, &cart_rank);
        Nx = N / dim[0];
	Ny = N / dim[1];
	Nz = N / dim[2];
	double *setka = (double*)malloc((Nx + 2) * (Ny + 2) * (Nz + 2) * sizeof(double));
	double *new_setka = (double*)malloc((Nx + 2) * (Ny + 2) * (Nz + 2) * sizeof(double));
        double count = 0.0;
        srand(time(NULL));
	for (int i = 0; i < (Nx + 2) * (Ny + 2) * (Nz + 2); i++){ 
		new_setka[i] = setka[i] = (double)rand() / RAND_MAX;
	}
        MPI_Datatype type, typev;
	MPI_Type_contiguous(Nx, MPI_DOUBLE, &type);
        MPI_Type_commit(&type);
	MPI_Type_vector(Ny, 1, Nx + 2, MPI_DOUBLE, &typev);
	MPI_Type_commit(&typev);

	double t = MPI_Wtime();
        int nei[6]; // 5: UP, 4: DOWN, 2: LEFT, 3: RIGHT, 1: FORWARD, 0: BACK

	MPI_Cart_shift(comm, 0, 1, &nei[0], &nei[1]);
	MPI_Cart_shift(comm, 1, 1, &nei[2], &nei[3]);  
        MPI_Cart_shift(comm, 2, 1, &nei[4], &nei[5]);
        for (int iter = 0; iter < N_ITER; iter++){
	    MPI_Request req[12];
	    for (int i = 1; i < Nz + 1; i++){
	        MPI_Isend(setka + i * (Ny + 2) * (Nx + 2) + Nx + 3 , 1, type, nei[2], 0, comm, &req[0]);
	        MPI_Irecv(setka + i * (Ny + 2) * (Nx + 2) + (Nx + 2) * (Ny + 1) + 1, 1, type, nei[3], 0, comm, &req[1]);
                MPI_Isend(setka + i * (Ny + 2) * (Nx + 2) + (Nx + 2) * Ny + 1, 1, type, nei[3], 0, comm, &req[2]);
	        MPI_Irecv(setka + i * (Ny + 2) * (Nx + 2) + 1, 1, type, nei[2], 0, comm, &req[3]);

                MPI_Isend(setka + i * (Nx + 2) * (Ny + 2) + Nx + 3, 1, typev, nei[0], 0, comm, &req[8]);
	        MPI_Irecv(setka + i * (Nx + 2) * (Ny + 2) + (Nx + 2) * 2 - 1, 1, typev, nei[1], 0, comm, &req[9]);
                MPI_Isend(setka + i * (Nx + 2) * (Ny + 2) + (Nx + 2) * 2 - 2, 1, typev, nei[1], 0, comm, &req[10]);
	        MPI_Irecv(setka + i * (Nx + 2) * (Ny + 2) + Nx + 2, 1, typev, nei[0], 0, comm, &req[11]);
		
		MPI_Wait(&req[0], MPI_STATUS_IGNORE);			
		MPI_Wait(&req[1], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[2], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[3], MPI_STATUS_IGNORE);
		MPI_Wait(&req[8], MPI_STATUS_IGNORE);			
		MPI_Wait(&req[9], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[10], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[11], MPI_STATUS_IGNORE);
            }
            for (int j = 1; j < Ny + 1; j++){
	        MPI_Isend(setka + j * (Nx + 2) + (Nx + 2) * (Ny + 2) + Nx + 3, 1, type, nei[4], 0, comm, &req[4]);
	        MPI_Irecv(setka + j * (Nx + 2) + (Nx + 2) * (Ny + 2) * (Nz + 1) + Nx + 3, 1, type, nei[5], 0, comm, &req[5]);
                MPI_Isend(setka + (Nx + 2) * (Ny + 2) * Nz + j * (Nx + 2) + Nx + 3, 1, type, nei[5], 0, comm, &req[6]);
	        MPI_Irecv(setka + Nx + 3 + j * (Nx + 2), 1, type, nei[4], 0, comm, &req[7]);
		MPI_Wait(&req[4], MPI_STATUS_IGNORE);			
		MPI_Wait(&req[5], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[6], MPI_STATUS_IGNORE);
	        MPI_Wait(&req[7], MPI_STATUS_IGNORE);
            }
	    for (int i = 1; i < Nz + 1; i++){
		    for (int j = 1; j < Ny + 1; j++){
			    for (int k = 1; k < Nx + 1; k++){
		                new_setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k] = (setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k - 1] + setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k + 1] + setka[i * (Nx + 2) * (Ny + 2) + (j - 1) * (Nx + 2) + k] + setka[i * (Nx + 2) * (Ny + 2) + (j + 1) * (Nx + 2) + k] + setka[(i - 1) * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k] + setka[(i + 1) * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k]) / 6.0;
			    }
		    }
	    }
            if (iter != N_ITER - 1){
	        for (int i = 0; i < (Nx + 2) * (Ny + 2) * (Nz + 2); i++){
			setka[i] = new_setka[i];
		}
	    }
	}
	t = MPI_Wtime() - t;
	for (int i = 1; i < Nz + 1; i++){
		    for (int j = 1; j < Ny + 1; j++){
			    for (int k = 1; k < Nx + 1; k++){
		                count += (new_setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k] - setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k]) * (new_setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k] - setka[i * (Nx + 2) * (Ny + 2) + j * (Nx + 2) + k]); 
			    }
		    }
	}

	count = count / (Nx * Ny * Nz);
	double co = 0;
	MPI_Reduce(&count, &co, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
	if (rank == 0){
	    printf("norma : %lf\n", sqrt(co / size));
	    printf("time : %lf\n", t);
	}
	MPI_Type_free(&type);
	MPI_Type_free(&typev);
	free(setka);
	free(new_setka);
	MPI_Finalize();
	return 0;
}
	    







       
        

