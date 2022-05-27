#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;

// the difference between pthread_mutex_lock and pthread_mutex_trylock is that lock will check the 
// mutex and if it is used by another thread it will stop execution of that thread and wait until the mutex
// become available again, but trylcok will check if mutex lock is available and if not it will return a number
// other than 0, and continue execution.

void *func(){
    if( pthread_mutex_trylock(&lock) == 0){
        printf("lock the lock for this thread\n");
        sleep(1);
        pthread_mutex_unlock(&lock);
    }else{
        printf("could not lock the lock man!!!\n");
    }
}

int main(){
    pthread_t threads[4];

    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < 4; i++){
        if(pthread_create(threads+i, NULL, func, NULL) != 0){
            printf("could not create thread %d \n", i);
            return 1;
        }
    }

    for(int i = 0; i < 4; i++){
        if(pthread_join(threads[i], NULL) != 0){
            printf("could not join thread %d \n", i);
            return 2;
        }
    }

    pthread_mutex_destroy(&lock);

}