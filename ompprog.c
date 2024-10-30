#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define N 400000

int main(int argc, char* argv[]){
	if (argc != 4) return 1;
	struct drand48_data randBuffer;
	int a, b, x, y[N], x1;
	a = strtol(argv[1], NULL, 10);
	b = strtol(argv[2], NULL, 10);
	x = strtol(argv[3], NULL, 10);
	double p = 0.5, s, t[N], alltime;
	if ((x < a) || (x > b)){
		printf("incorrect x\n");
		return 1;
	}
	for (int i = 0; i < N; i++){ 
		y[i] = 0;
		t[i] = 0;
	}
        alltime = omp_get_wtime();
#pragma omp parallel private(s) shared(alltime)  //распараллеливаем на число потоков N
	{		   //копируем переменные р, х и s в каждый поток
	
		struct drand48_data randBuffer;

                srand48_r(alltime * omp_get_thread_num(), &randBuffer);
      
       #pragma omp for firstprivate(x1)
       for (int i = 0; i < N; i++){
	       x1 = x;
               t[i] = omp_get_wtime();
               while ((x1 != a) && (x1 != b)){ // пока частицу не поглотила граница
	               drand48_r(&randBuffer, &s);
	               if (s <= p){ // прибавляем 1 с вероятностью р
		            x1 +=1;
	               }
	               else{
		            x1 -= 1;
	               }
               }
               if (x1 == b) y[i] = 1; // попала ли частица с номером i в границу b

               t[i] = omp_get_wtime() - t[i]; 
}}
       alltime = omp_get_wtime() - alltime;
       int sum = 0;
       double sumt = 0;
       for (int i = 0; i < N; i++){ // считаем вероятность и среднее время жизни
	       sum += y[i];
	       sumt += t[i];
       }
       printf("alltime: %lf, meantime: %lf\n", alltime, sumt / N);
       printf("%lf\n", ((double)sum) / N);
        
       return 0;
}
        


