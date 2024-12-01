#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define N 1000
#define NUM_BASIC 20

int num_cells_alive(int *setka, int size){
	int count = 0;
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N / size; j++){
			if (setka[i * N + j] == 1){
				count += 1;
			}
		}
	}
	return count;
}

void game(int *setka, int *newsetka, int size, int rank){
	MPI_Request req1, req2;
	if (rank == 0){
		MPI_Send(setka, N, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
		MPI_Irecv(setka + N * (N / size + 1), N, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &req2);
	}
        else{
	        MPI_Send(setka, N, MPI_INT, (rank - 1) % size, 0, MPI_COMM_WORLD);
	        MPI_Irecv(setka + N * (N / size + 1), N, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &req2);
	}
	MPI_Send(setka + N * (N / size + 1), N, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
	MPI_Irecv(setka, N, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &req1);
	int count = 0, a, b, index;
	for (int y = 1; y < N / size - 1; y++){
		for (int x = 0; x < N; x++){
	            for (int i = 0; i < 3; i++){
		        for (int j = 0; j < 3; j++){
			    if ((i != 0) && (j != 0)){
                                a = (x + i - 1 + N) % N;
			        b = (y + j - 1 + N) % N;
			        count += setka[b + N * a];
			    }
		        }
	            }
		    index = y * N + x;
		    if ((count == 3) || ((count == 2) && (setka[index] == 1)))
			    newsetka[index] = 1;
		    
		    else
			    newsetka[index] = 0;
			
                }
	}

        MPI_Wait(&req1, MPI_STATUS_IGNORE);
        MPI_Wait(&req2, MPI_STATUS_IGNORE);	
	for (int x = 0; x < N; x++){
	            for (int i = 0; i < 3; i++){
		        for (int j = 0; j < 3; j++){
			    if ((i != 0) && (j != 0)){
                                a = (x + i - 1 + N) % N;
			        b = (j - 1 + N) % N;
			        count += setka[b + N * a];
			    }
		        }
	            }
		    index = x;
		    if ((count == 3) || ((count == 2) && (setka[index] == 1)))
			    newsetka[index] = 1;
		    
		    else
			    newsetka[index] = 0;
			
        }
	for (int x = 0; x < N; x++){
	            for (int i = 0; i < 3; i++){
		        for (int j = 0; j < 3; j++){
			    if ((i != 0) && (j != 0)){
                                a = (x + i - 1 + N) % N;
			        b = (N / size - 1 + j - 1 + N) % N;
			        count += setka[b + N * a];
			    }
		        }
	            }
		    index = (N / size - 1) * N + x;
		    if ((count == 3) || ((count == 2) && (setka[index] == 1)))
			    newsetka[index] = 1;
		    
		    else
			    newsetka[index] = 0;
			
        }
	
}

int main(int argc, char** argv){
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int *tmp;
        int* setka = (int*)calloc(N * (N / size + 2), sizeof(int));
	int* newsetka = (int*)calloc(N * (N / size + 2), sizeof(int));
	if (rank == 0){
            srand(rank * time(NULL));

	    for (int i = 0; i < N * (N / size + 2); i++){
		    setka[i] = (rand() % 10 < 5) ? 1 : 0;
	    }
	}
	int err = MPI_Bcast(setka, N * (N / size + 2), MPI_INT, 0, MPI_COMM_WORLD);
	int iter = 0, f = 0;

        double start = MPI_Wtime();
	int *prev_alive = (int*)calloc(size, sizeof(int));

	while (f != 1){
            game(setka, newsetka, size, rank);

            int alivep = num_cells_alive(setka, size);
    	    int *all_alive = (int*)calloc(size, sizeof(int));
	    MPI_Gather(&alivep, 1, MPI_INT, all_alive, 1, MPI_INT, 0, MPI_COMM_WORLD);

	    if (iter > NUM_BASIC){
		    if (rank == 0){
			    f = 0;
			    for (int i = 0; i < size; i++){
				    if (all_alive[i] == prev_alive[i]){
					    f = 1;
					    printf("Число живых клеток совпало на процессе %d на итерации %d\n", i, iter);
				    }
				    prev_alive[i] = all_alive[i];
			    }
		    }
	    }
	    MPI_Bcast(&f, 1, MPI_INT, 0, MPI_COMM_WORLD);
	    iter++;
	    tmp = setka;
	    setka = newsetka;
	    newsetka = tmp;
	    free(all_alive);


	}

	double end = MPI_Wtime();
	if (rank == 0){
	    printf("время работы: %lf\n", end - start);
	}
	free(prev_alive);
	free(setka);
	free(newsetka);
	MPI_Finalize();
	return 0;
}


            	    


