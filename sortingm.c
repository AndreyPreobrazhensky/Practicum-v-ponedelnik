#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define CHUNK 64

int mfunk(const void *a, const void *b){
	if (*((int *)a) < *((int*)b)) return -1;
	else if (*((int*)a) == *((int*)b)) return 0;
	else return 1;
}

void sorts(int *mas, int l1, int l2){
	int k;
        for (int i = l1 + 1; i < l2; i++){
		for (int j = l1; j < i; j++){
			if (mas[i] < mas[j]){
			      k = mas[j];
		              mas[j] = mas[i];
		              mas[i] = k;
			}
		}
	}
}

void merge(int *mas, int l1, int l2, int l3){
	int *ml = (int *)malloc((l3 - l1) * sizeof(int));
	int l_t1 = l1, l_t2 = l2;
	for (int i = 0; i < (l3 - l1); i++){
		if (l_t1 < l2 && l_t2 < l3){
			if (mas[l_t1] < mas[l_t2]){
				ml[i] = mas[l_t1++];
			}
			else
			{
				ml[i] = mas[l_t2++];
			}
		}
		else if (l_t1 < l2){
			ml[i] = mas[l_t1++];
		}
		else ml[i] = mas[l_t2++];
	}
	for (int i = l1; i < l3; i++){
		mas[i] = ml[i - l1];
	}
	free(ml);
}

int main(int argc, char *argv[]){
	if (argc != 3) return 1;
	int N = strtol(argv[1], NULL, 10);
	int p = strtol(argv[2], NULL, 10);
	int *m = (int *)malloc(N * sizeof(int));
	int *n = (int *)malloc(N * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < N; i++){
		m[i] = rand();
	        //printf("%d ", m[i]);
	}
	printf("\n");
	double t = omp_get_wtime();
#pragma omp parallel num_threads(p)
#pragma omp single
{
	for (int i = 0; i < N / CHUNK; i++){
                #pragma omp task
		{
		sorts(m, i * CHUNK, (i + 1) * CHUNK);
		}
	}

	int k = N / CHUNK, z = 1;
        while (k != 1){
                #pragma omp task
		for (int i = 0; i < k; i += 2){
			merge(m, i * (CHUNK * z), (i + 1) * (CHUNK * z), (i + 2) * (CHUNK * z));
		}
	        z = z * 2;
	        k = k / 2;
	}
}
        t = omp_get_wtime() - t;
	double tr = omp_get_wtime();
	qsort(n, N, sizeof(int), mfunk);
	tr = omp_get_wtime() - tr;
	/*
	for (int i = 0; i < N; i++){
		printf("%d\n", m[i]);
	}*/
        printf("\nmy sort: %lf\nqsort: %lf\n", t, tr);
	free(n);
	free(m);
	return 0;
}









        

