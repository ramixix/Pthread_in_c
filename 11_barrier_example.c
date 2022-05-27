#define _XOPEN_SOURCE 600 /* Or higher */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 12

// here we create a barrier and specify that when 13 threads comes then barrier shall be open.
// after creating 12 threads each will run the roll_dice function and terminated, so what we do we add
// a barrier that stop 12 theards and when the main thread come to the barrier it make barrier off (12 + 1 = 13)
// and then they can continoue.

pthread_barrier_t set_status_barrier, time_Toannounce_winner;

int dice_values[THREAD_NUM];
int win_or_lose[THREAD_NUM];

void *roll_dice(void *t_index){
    int index = *(int *)t_index;
    while (1){
        int rolled_value = rand() % 7; // 0 - 6
        dice_values[index] = rolled_value;

        pthread_barrier_wait(&set_status_barrier);
        pthread_barrier_wait(&time_Toannounce_winner);
        if(win_or_lose[index] == 1){
            printf("Thread number %2d rolled:  %3d ==> Won :)\n", index, rolled_value);
        }
        else{
            printf("Thread number %2d rolled:  %3d ==> Lost :(\n", index, rolled_value);
        }
    }
    
    free(t_index);

}

int main(){
    srand(time(NULL));
    pthread_t threads[THREAD_NUM];
    pthread_barrier_init(&set_status_barrier, NULL, THREAD_NUM + 1);
    pthread_barrier_init(&time_Toannounce_winner, NULL, THREAD_NUM + 1);
    
    for(int i = 0 ; i < THREAD_NUM; i++){
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        if(pthread_create(threads+i, NULL, roll_dice, (void *)index) != 0){
            perror("Could not create thread\n");
        }
    }
    
    while(1){
        pthread_barrier_wait(&set_status_barrier);
        int max = 0;
        for(int i =0 ; i < THREAD_NUM; i++){
            if(dice_values[i] > max){
                max = dice_values[i];
            }
        }

        for(int i =0 ; i < THREAD_NUM; i++){
            if(dice_values[i] == max){
                win_or_lose[i] = 1;
            }else{
                win_or_lose[i] = 0;
            }
        }

        // sleep(1);
        pthread_barrier_wait(&time_Toannounce_winner);
        sleep(1);
        printf("===========  New Round  ========\n");
    }
    for(int i = 0 ; i < THREAD_NUM; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Could not join threads\n");
        }
    }

    pthread_barrier_destroy(&set_status_barrier);
    pthread_barrier_destroy(&time_Toannounce_winner);
}