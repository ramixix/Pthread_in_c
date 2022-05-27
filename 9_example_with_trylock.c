#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define Thread_Num 10

pthread_mutex_t stove_lock[4];
int stove_fuel[4] = {100, 100, 100, 100};

void *start_cooking(){
    for(int i =0 ; i < 4; i++){
        int needed_fuel = rand()%35;
        if(pthread_mutex_trylock(&stove_lock[i]) == 0){
            if(stove_fuel[i] - needed_fuel < 0){   
                printf("Not enough fuel left, i am going home ...\n");
            }else{
                stove_fuel[i] -= needed_fuel;
                printf("%d fuel is left.(from stove %d)\n", stove_fuel[i],i);
                usleep(500000);
            }
            pthread_mutex_unlock(&stove_lock[i]);
            break;
        }else{
            if(i==3){
                printf("stoves are not available yet, waiting!\n");
                usleep(300000);
                i = 0;
            }
        }
    }
}

int main(){
    for(int i = 0 ; i < 4; i++){
        pthread_mutex_init(&stove_lock[i], NULL);
    }
    pthread_t threads[Thread_Num];
    srand(0);

    for(int i =0; i < Thread_Num; i++){
        if(pthread_create(threads+i, NULL, start_cooking, NULL) != 0 ){
            perror("could not create thread man\n");
        }
    }

    for(int i = 0 ; i < Thread_Num; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("could not join threads man\n");
        }
    }

    for(int i = 0 ; i < 4; i++){
        pthread_mutex_destroy(&stove_lock[i]);
    }

    printf("\nfinal stove fuels that are left:\n| ");
    for(int i = 0 ; i < 4; i++){
        printf("%d | ", stove_fuel[i]);
    }
    
}