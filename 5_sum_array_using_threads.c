#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARR_LEN 10

// calculate sum of element of an array by deviding array into two sub-array and let
// two thread to calculate each sub-array independently and simultaneously

int numbers[ARR_LEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void *sum_up(void *arr_offset){
    int sum = 0;
    for(int i=0; i < ARR_LEN/2; i++){
        sum += numbers[*(int *)arr_offset + i];
    }

    *(int *)arr_offset = sum;
    return arr_offset;
}

int main(){
    pthread_t threads[2];

    for(int i =0 ; i < 2 ; i++){
        int *arr_off = (int *)malloc(sizeof(int));
        *arr_off = i*(ARR_LEN/2);
        if(pthread_create(threads+i, NULL, sum_up, arr_off) != 0){
            printf("Could not create thread %d \n", i);
            return 1;
        }
    }
    int global_sum=0;
    for(int i =0 ; i < 2 ; i++){
        int *half_sum;
        if(pthread_join(threads[i], (void **)&half_sum) != 0){
            printf("Could not join thread %d \n", i);
            return 1;
        }
        printf("the sum of half : %d \n", *half_sum);
        global_sum+= *half_sum;
        free(half_sum);
    }

    printf("the total sum of array  : %d \n", global_sum);
}