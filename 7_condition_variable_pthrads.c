#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock_fuel;
pthread_cond_t is_there_enough_fuel;
int fuel = 0;

// show case the condition variables (pthread_cond_wait, pthread_cond_signal, pthread_cond_broadcast)

void *filling_fuel(){
    printf("filling fuel\n");
    for(int i = 0 ; i < 8; i++){
        pthread_mutex_lock(&lock_fuel);
        fuel += 15;
        printf("fuel available right now : %d\n", fuel);
        pthread_mutex_unlock(&lock_fuel);
        pthread_cond_signal(&is_there_enough_fuel);
        sleep(1);
    }

}


void *car(){
    printf("Here to get fuel\n");
    pthread_mutex_lock(&lock_fuel);
    while(fuel < 40){
        printf("not enoght fuel\n");
        pthread_cond_wait(&is_there_enough_fuel, &lock_fuel);
        printf("not enoght fuel1\n");
        // pthread_cond_wiat is equivalent to three operations :
        // unlock the mutex : pthread_mutex_unlock(&lock_fuel)
        // wait for signal to is_there_enough_fuel condition
        // lock the mutex again : pthread_mutex_lock (&lock_fuel)
    }
    
    fuel-=40;
    printf("wwwwwwwwwwwwww ? %d\n", fuel);
    pthread_mutex_unlock(&lock_fuel);
}


int main(){
    pthread_t threads[2];
    pthread_mutex_init(&lock_fuel, NULL);
    pthread_cond_init(&is_there_enough_fuel, NULL);

    if(pthread_create(&threads[0], NULL, &car, NULL) != 0){
        printf("could not make thread 1\n");
        return 1;
    }

    if(pthread_create(&threads[1], NULL, &filling_fuel, NULL) != 0){
        printf("could not make thread 2\n");
        return 2;
    }


    for(int i =0 ; i < 2 ; i++){
        if(pthread_join(threads[i], NULL) != 0){
            printf("could not join the thread %d \n", i);
            return 3;
        }
    }
   
   pthread_mutex_destroy(&lock_fuel);
   pthread_cond_destroy(&is_there_enough_fuel);

}