#include <iostream>
#include "pthread.h"
#include <queue>

#define KOL_VO 30

pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t a, b;

class MyQueue
{
	std::queue<int> myqueue;
	int size;

	public:
	MyQueue(int sizem){
		
		size = sizem;
	}
	void put(int value){
		pthread_mutex_lock(&mut2);
		if (myqueue.size() == size){
			pthread_cond_wait(&b, &mut2);
		}
		myqueue.push(value);
		pthread_cond_signal(&a);
		std::cout << "We push: " << value << std::endl;
		pthread_mutex_unlock(&mut2);
	}

	int get(){
		pthread_mutex_lock(&mut2);
		while (myqueue.size() == 0){
			pthread_cond_wait(&a, &mut2);
		}
		int a = myqueue.front();
		myqueue.pop();
		std::cout << "We pop: " << a << std::endl;
		pthread_cond_signal(&b);
		pthread_mutex_unlock(&mut2);
		return a;
	}

};
MyQueue ochered(10);

void *funcr(void *arg){
	ochered.get();
}

void *funcw(void *arg){
	int* val = (int*)arg;
	ochered.put(*val); 
}

int main(){
	pthread_t nity[KOL_VO];

	pthread_cond_init(&a, NULL);
	pthread_cond_init(&b, NULL);
        int k[KOL_VO / 2];
	for (int i = 0; i < KOL_VO / 2; i++){
		k[i] = i;
		pthread_create(&nity[i], NULL, funcr, NULL);
	}

        for (int i = KOL_VO / 2; i < KOL_VO; i++){
	        pthread_create(&nity[i], NULL, funcw, &k[i - KOL_VO / 2]);
	}

	for (int i = 0; i < KOL_VO; i++){
		pthread_join(nity[i], NULL);
	}
	pthread_cond_destroy(&a);
	pthread_cond_destroy(&b);
	pthread_mutex_destroy(&mut2);
	return 0;
}	



