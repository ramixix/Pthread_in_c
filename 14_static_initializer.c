#include <stdio.h>
#include <pthread.h>

#define THREAD_NUM 2

// 1. while using mutex and condtion variable as we saw before, we need to initial then 
// at first using pthread_mutex_init and pthread_cond_init and then destroy them at the end of 
// program using pthread_mutex_destroy and pthread_cond_destroy.
// but there is another way and that is using static initilizers. 
// (these static initializers are only availabe for mutex and cond, not for barrier or else.)

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
// it is not necessary to call pthread_mutex_destroy or pthread_cond_destroy at the end but it is good practice to do so.

void *th_func(){

}

int main(){
    pthread_t thread[THREAD_NUM];
    
    // pthread_mutex_init(&mutex1, NULL);
    // pthread_cond_init(&cond1, NULL);

    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_create(thread+i, NULL, th_func, NULL) != 0){
            perror("could not create thread\n");
        }
    }

    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_join(thread[i], NULL) != 0){
            perror("could not join thread\n");
        }
    }

    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&cond1);
}