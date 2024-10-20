#include "omp.h"
#include <stdio.h>
#include <immintrin.h>
#include <math.h>

int main(){
	//сделать матрицу с размером кратным 4м
        int size;
        printf("введите размер матрицы, кратный 4м\n");
        scanf("%d", &size);
        double **mas1, h, **mas2, **masr, **masp;
	mas1 = (double**)malloc(size * sizeof(double*));	
	mas2 = (double**)malloc(size * sizeof(double*));
	masr = (double**)malloc(size * sizeof(double*));
        masp = (double**)malloc(size * sizeof(double*));

	for (int i = 0; i < size; i++){
		mas1[i] = (double*)malloc(size * sizeof(double));
	        mas2[i] = (double*)malloc(size * sizeof(double));
		masr[i] = (double*)malloc(size * sizeof(double));
		masp[i] = (double*)malloc(size * sizeof(double));
		for (int j = 0; j < size; j++){
			h = (double)i;
			mas1[i][j] = h / (j + 1);
			mas2[i][j] = (j + 1) / (h + 1);
			masr[i][j] = 0;
			//printf("(%lf, %lf) ", h / (j + 1), (j + 1) / (h + 1));
		}
		//printf("\n");
	}
	//printf("\n");
        double t = omp_get_wtime();
	//написать перемножение матриц
	double *value;
	for (int i = 0; i < size; i+=4){ //   проходим все строки с шагом 4
					 //
		for (int k = 0; k < size; k+= 4){ // проходим по столбцам итоговой матрицы с шагом 4
						  // пишем в новую матрицу по 4 вектора-столбца.
						  //
			__m256d t1 = _mm256_set_pd(masr[i][k], masr[i + 1][k], masr[i + 2][k], masr[i + 3][k]);				
			__m256d t2 = _mm256_set_pd(masr[i][k + 1], masr[i + 1][k + 1], masr[i + 2][k + 1], masr[i + 3][k + 1]);
                        __m256d t3 = _mm256_set_pd(masr[i][k + 2], masr[i + 1][k + 2], masr[i + 2][k + 2], masr[i + 3][k + 2]);
                        __m256d t4 = _mm256_set_pd(masr[i][k + 3], masr[i + 1][k + 3], masr[i + 2][k + 3], masr[i + 3][k + 3]);
			for (int j = 0; j < size; j+=4){
				__m256d r1 = _mm256_set_pd(mas1[i][j], mas1[i + 1][j], mas1[i + 2][j], mas1[i + 3][j]);				
			        __m256d r2 = _mm256_set_pd(mas1[i][j + 1], mas1[i + 1][j + 1], mas1[i + 2][j + 1], mas1[i + 3][j + 1]);
                                __m256d r3 = _mm256_set_pd(mas1[i][j + 2], mas1[i + 1][j + 2], mas1[i + 2][j + 2], mas1[i + 3][j + 2]);
                                __m256d r4 = _mm256_set_pd(mas1[i][j + 3], mas1[i + 1][j + 3], mas1[i + 2][j + 3], mas1[i + 3][j + 3]);
			
                                __m256d l11 = _mm256_set_pd(mas2[j][k], mas2[j][k], mas2[j][k], mas2[j][k]);
                                __m256d l12 = _mm256_set_pd(mas2[j + 1][k], mas2[j + 1][k], mas2[j + 1][k], mas2[j + 1][k]);
                                __m256d l13 = _mm256_set_pd(mas2[j + 2][k], mas2[j + 2][k], mas2[j + 2][k], mas2[j + 2][k]);
                                __m256d l14 = _mm256_set_pd(mas2[j + 3][k], mas2[j + 3][k], mas2[j + 3][k], mas2[j + 3][k]);

			       	__m256d l21 = _mm256_set_pd(mas2[j][k + 1], mas2[j][k + 1], mas2[j][k + 1], mas2[j][k + 1]);
                                __m256d l22 = _mm256_set_pd(mas2[j + 1][k + 1], mas2[j + 1][k + 1], mas2[j + 1][k + 1], mas2[j + 1][k + 1]);
                                __m256d l23 = _mm256_set_pd(mas2[j + 2][k + 1], mas2[j + 2][k + 1], mas2[j + 2][k + 1], mas2[j + 2][k + 1]);
                                __m256d l24 = _mm256_set_pd(mas2[j + 3][k + 1], mas2[j + 3][k + 1], mas2[j + 3][k + 1], mas2[j + 3][k + 1]);

				__m256d l31 = _mm256_set_pd(mas2[j][k + 2], mas2[j][k + 2], mas2[j][k + 2], mas2[j][k + 2]);
                                __m256d l32 = _mm256_set_pd(mas2[j + 1][k + 2], mas2[j + 1][k + 2], mas2[j + 1][k + 2], mas2[j + 1][k + 2]);
                                __m256d l33 = _mm256_set_pd(mas2[j + 2][k + 2], mas2[j + 2][k + 2], mas2[j + 2][k + 2], mas2[j + 2][k + 2]);
                                __m256d l34 = _mm256_set_pd(mas2[j + 3][k + 2], mas2[j + 3][k + 2], mas2[j + 3][k + 2], mas2[j + 3][k + 2]);
				
				__m256d l41 = _mm256_set_pd(mas2[j][k + 3], mas2[j][k + 3], mas2[j][k + 3], mas2[j][k + 3]);
                                __m256d l42 = _mm256_set_pd(mas2[j + 1][k + 3], mas2[j + 1][k + 3], mas2[j + 1][k + 3], mas2[j + 1][k + 3]);
                                __m256d l43 = _mm256_set_pd(mas2[j + 2][k + 3], mas2[j + 2][k + 3], mas2[j + 2][k + 3], mas2[j + 2][k + 3]);
                                __m256d l44 = _mm256_set_pd(mas2[j + 3][k + 3], mas2[j + 3][k + 3], mas2[j + 3][k + 3], mas2[j + 3][k + 3]);
				
				__m256d h1 = _mm256_mul_pd(r1, l11);
		        	__m256d h2 = _mm256_mul_pd(r2, l12);
                                __m256d h3 = _mm256_mul_pd(r3, l13);
                                __m256d h4 = _mm256_mul_pd(r4, l14);
				h1 = _mm256_add_pd(h1, h2);
                                h1 = _mm256_add_pd(h1, h3);
                                h1 = _mm256_add_pd(h1, h4);
                                t1 = _mm256_add_pd(t1, h1);
				
				h1 = _mm256_mul_pd(r1, l21);
		        	h2 = _mm256_mul_pd(r2, l22);
                                h3 = _mm256_mul_pd(r3, l23);
                                h4 = _mm256_mul_pd(r4, l24);
				h1 = _mm256_add_pd(h1, h2);
                                h1 = _mm256_add_pd(h1, h3);
                                h1 = _mm256_add_pd(h1, h4);
                                t2 = _mm256_add_pd(t2, h1);

                                h1 = _mm256_mul_pd(r1, l31);
		        	h2 = _mm256_mul_pd(r2, l32);
                                h3 = _mm256_mul_pd(r3, l33);
                                h4 = _mm256_mul_pd(r4, l34);
				h1 = _mm256_add_pd(h1, h2);
                                h1 = _mm256_add_pd(h1, h3);
                                h1 = _mm256_add_pd(h1, h4);
                                t3 = _mm256_add_pd(t3, h1);

                                h1 = _mm256_mul_pd(r1, l41);
		        	h2 = _mm256_mul_pd(r2, l42);
                                h3 = _mm256_mul_pd(r3, l43);
                                h4 = _mm256_mul_pd(r4, l44);
				h1 = _mm256_add_pd(h1, h2);
                                h1 = _mm256_add_pd(h1, h3);
                                h1 = _mm256_add_pd(h1, h4);
                                t4 = _mm256_add_pd(t4, h1);
			}
			value = (double*) &t1;
			for (int f = 0; f < 4; f++){
				masr[i + f][k] = value[3 - f];
			}

			value = (double*) &t2;
			for (int f = 0; f < 4; f++){
				masr[i + f][k + 1] = value[3 - f];
			}

                        value = (double*) &t3;
			for (int f = 0; f < 4; f++){
				masr[i + f][k + 2] = value[3 - f];
			}

                        value = (double*) &t4;
			for (int f = 0; f < 4; f++){
				masr[i + f][k + 3] = value[3 - f];
			}


		}
	}


        t = omp_get_wtime() - t;


       // последовательный вариант
       //
       //
       double tp = omp_get_wtime();
       for (int i = 0; i < size; i++){
	       for (int j = 0; j < size; j++){
		       double sum = 0.0;
		       for (int k = 0; k < size; k++){
			       sum += mas1[i][k] * mas2[k][j];
		       }
		       masp[i][j] = sum;
	       }
       }
       tp = omp_get_wtime() - tp;

	//написать вывод результата
        double eps = 0, epmax = 0, ep = 0;	
        for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			ep = fabs(masr[i][j] - masp[i][j]);
			eps += ep;
			if (ep > epmax) epmax = ep;
			//printf("%lf ", masr[i][j]);
		}
		//printf("\n");
		free(mas1[i]);
		free(mas2[i]);
		free(masr[i]);
		free(masp[i]);
	}
        free(mas1);
	free(mas2);
	free(masr);
	free(masp);
	printf("time: %lf,\ntime posledovatelno: %lf\n", t, tp);
	printf("summ: %lf, max: %lf\n", eps, epmax);



	return 0;
}
