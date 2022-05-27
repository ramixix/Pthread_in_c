#include <stdio.h>
#include <pthread.h>

#include <time.h>

unsigned long int mail = 0;
pthread_mutex_t lock;

// remove the race condition that we saw in the 2_race_in_thread.c program using mutexes.

void *add_to_main(){
    for(int i =0 ; i < 10000; i++){

        pthread_mutex_lock(&lock);
        mail +=1;
        pthread_mutex_unlock(&lock);

    }

}

int main(){
    clock_t t = clock();
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    if(pthread_create(&t1, NULL, add_to_main, NULL) != 0){
        printf("could not create thread\n");
        return 1;
    }
    if(pthread_create(&t2, NULL, add_to_main, NULL) != 0){
        printf("could not create thread\n");
        return 1;
    }

    if(pthread_join(t1, NULL) != 0){
        printf("error in joing\n");
        return 2;
    }
    if(pthread_join(t2, NULL) != 0){
        printf("error in joing\n");
        return 2;
    }

    printf("the value of mail %d \n", mail);
    pthread_mutex_destroy(&lock);
    double time_taken = (double)(clock() - t ) / CLOCKS_PER_SEC;
    // when using mutex it takes more time to run.
    // never forget that you just need to use mutex in places that may have race conditions 
    // running all the code using mutex is similar to run code in serial way.
    printf("time taken by program when mutex is used : %f\n", time_taken);
    return 0; 
}