#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>


struct timeval tv1,tv2,dtv;

struct timezone tz;

void time_start() { gettimeofday(&tv1, &tz); }

long time_stop()

{ gettimeofday(&tv2, &tz);

  dtv.tv_sec= tv2.tv_sec -tv1.tv_sec;

  dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;

  if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }

  return dtv.tv_sec*1000+dtv.tv_usec/1000;

}
void *th_func(void *arg){
	int k, j, *as = (int*)arg, kol_vo;
        printf("%d %d %d\n", as[0], as[1], as[2]);	
	double ch, x, pil = 0, *result = (double*)as;
	ch = 1.0 / (double)as[1];
	k = (int)(as[1] / as[2]);
	kol_vo = k * (as[0] + 1);
	for (j = k * as[0]; j < kol_vo; j++){
		x = j * ch;
		pil += ch * (4.0 / (1.0 + x * x));
	}
	result[0] =  pil;
	printf("%f\n", pil);
}

int main(int argc, char* argv[]){
    time_start();
    double pi = 0;
    int i, N, *a[3], kol;
    if (argc != 3) return 1;
    N = strtol(argv[1], NULL, 10);
    kol = strtol(argv[2], NULL, 10);
    for (i = 0; i < kol; i++){
        a[i] = (int*)malloc(3 * sizeof(int));
	a[i][1] = N; a[i][2] = kol;
    }
    pthread_t pth[kol];
    for (i = 0; i < kol; i++){
	    a[i][0] = i;
	    pthread_create(&pth[i], NULL, th_func, a[i]);
    }

    for (i = 0; i < kol; i++){
	    pthread_join(pth[i], NULL);
            pi += ((double*)a[i])[0];
	    free(a[i]);
    }
    printf("%f\n", pi);
    printf("%ld\n", time_stop());
    return 0;
}







