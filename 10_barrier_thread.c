#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_NUM 3
pthread_barrier_t barrier;

// barrier is a point where the thread is going to wait for other threads and will proceed
// further only when predefined number of threads reach the same barrier in their respective programs. 
// In cases where you must wait for a number of tasks to be completed before an overall task can proceed, barrier can be used.

void *barrier_func(){
    printf("wating for enought number of threads come to barrier\n");
    sleep(1);
    pthread_barrier_wait(&barrier);
    printf("barrier is off.\n");
    sleep(1);
}


int main(){
    pthread_barrier_init(&barrier, NULL, 3);
    pthread_t threads[THREAD_NUM];

    for(int i =0; i <THREAD_NUM ; i++){
        if(pthread_create(threads+i, NULL, barrier_func, NULL) != 0 ){
            perror("could not create threads\n");
        }
    }

    for(int i =0; i <THREAD_NUM ; i++){
        if(pthread_join(threads[i], NULL) != 0 ){
            perror("could not join pthreads\n");
        }
    }

    pthread_barrier_destroy(&barrier);    
}