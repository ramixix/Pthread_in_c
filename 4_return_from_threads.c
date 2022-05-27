#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define THREAD_NUM 8

// used pthread_join function to get return value (which is pointer) of a thread function.
// it takes a double void pointer variable as the second parametere, that is because it will 
// write the address of returned value to the address of give variable.

void *roll_dice(){
    int dice = (rand()%6) + 1;

    int *return_result = (int*) malloc(sizeof(int));
    *return_result = dice;
    return (void *)return_result;
}

int main(){
    srand(time(NULL));
    pthread_t threads_arr[THREAD_NUM];
    int *result;

    for(int i=0; i < THREAD_NUM; i++){
        if(pthread_create(threads_arr+i, NULL, roll_dice, NULL) != 0){
            printf("could not create thread %d\n", i);
            return 1;
        }
    }


    for(int i=0; i < THREAD_NUM; i++){
        if(pthread_join(threads_arr[i], (void **)(&result) ) != 0){
            printf("could not join thread %d\n", i);
            return 1;
        }
        
        printf("reulsts of %d : %d\n",i, *result);
        free(result);
    }
     
}