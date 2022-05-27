#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TH_NUM 4

sem_t my_semaphore;
/*
    there is two operations that we can perform on semaphores, wait and post.
    wait decrease the value of semaphore by 1 only if it is not already equal to 0, otherwise it will wait until it became more than zero.
    post simply increase the value of semaphore by 1.

    The diffrence between semaphore and mutex is that, semaphore can be locked and unlock by any thread, That means if one thread
    locks it the other thread is able to unlock the semaphor, So it is shared between all threads. Where mutex is not, if one thread
    lock a mutex no other thread can lock or unlock that mutex until that thread unlock the mutex.

    Note : Mutex is used when you want just one thread to access a resource at a time. But semaphore is used when you want limit a resouce
    and say that you want just limit number of threads can access a resource at one time.
*/
void *th_func(void *th_id){
    sem_wait(&my_semaphore);
    printf("thread %d is running\n", *(int *)th_id);
    sleep(1);
    sem_post(&my_semaphore);
    free(th_id);  
}

int main(){
    pthread_t threads[TH_NUM];
    // the first paremetere is the reference to our semaphore variable.
    // the seconde parametere tells semaphore whether we are using multiple processes(if you have only multiple threads then you should pass 0 
    // but if you have multiple processes that might or might not have multiple threads that uses this semaphore then you should pass 1)
    // the third parametere is the initial value of semaphore.
    sem_init(&my_semaphore, 0, 2);

    for(int i = 0; i < TH_NUM ; i++){
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        if(pthread_create(threads+i, NULL, *th_func, id) != 0){
            printf("Count not create thread %d\n", i);
        }
    }

    for(int i = 0; i < TH_NUM ; i++){
        if(pthread_join(threads[i], NULL) != 0){
            printf("Count not create thread %d\n", i);
        }
    }

    sem_destroy(&my_semaphore);
    return 0;
}