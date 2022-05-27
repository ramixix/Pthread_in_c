#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 5        

int fuel = 0;
pthread_mutex_t fuel_lock;
pthread_cond_t enogh_fuel;

// just remember that if more than 1 thread is wainting for condition variable, when pthread_cond_broadcast
// is called then all the waiting threads are going to be run in order, one after another.
// and also don't forget that when more than one thread calling a function that uses mutex
// the mutex value is going to be locked just by one of the threads at a time, a then when it gets unlock
// the other one of the threads lock it....

void *car(){
    printf("Here to get fuel\n");
    pthread_mutex_lock(&fuel_lock);
    while(fuel < 35){
        printf("not enough fuel\n");
        pthread_cond_wait(&enogh_fuel, &fuel_lock);
    }
    fuel-=35;
    printf("get fuel let goooo (fuel available %d)\n", fuel);
    pthread_mutex_unlock(&fuel_lock);
}

void *filling_fuel(){
    printf( "filling fuel (right now we have %d)\n", fuel);
    for(int i=0; i < 8; i++){
        pthread_mutex_lock(&fuel_lock);
        fuel+=55;
        printf("fuel available : %d\n", fuel);
        pthread_mutex_unlock(&fuel_lock);
        // pthread_cond_signal send a signal to just one of the waiting conditions and the other ones not get woken.
        // pthread_cond_broadcast send signal to all of the waiting threads and run them one by one after one another. (in order)
        pthread_cond_broadcast(&enogh_fuel);
        sleep(1);
    }
}

int main(){
    pthread_mutex_init(&fuel_lock, NULL);
    pthread_cond_init(&enogh_fuel, NULL);

    pthread_t threads[THREAD_NUM];
    for(int i = 0; i < THREAD_NUM; i++){
        if(i == THREAD_NUM - 1){
            if(pthread_create(&threads[i], NULL, filling_fuel, NULL) != 0){
                perror("Could not create thread (filling fuel)\n");
            }
        }else{
            if(pthread_create(&threads[i], NULL, car, NULL) != 0){
                perror("Could not create thread (car)\n");
            }
        }
    }


    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_join(threads[i], NULL) != 0 ){
            perror("could not join thread\n");
        }
    }

    pthread_mutex_destroy(&fuel_lock);
    pthread_cond_destroy(&enogh_fuel);
}