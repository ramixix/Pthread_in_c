#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 16

sem_t login_status;

/*
    Here imagine that we have a gaming server and it is cable of letting 12 people connecting to the server
    and play the available games, if anyone try to connect to the server while all the 12 connection are full
    then the player must wait until one of the gamers log out.

    Here we use semaphores to limit the number of users that can access the server at one time.
*/

void *game(void *player_id){
    int player_num = *(int*)player_id;
    printf("%d :| Player is waiting to login in...\n", player_num);
    sem_wait(&login_status);

    printf("%d :) Player is now playing the game.\n", player_num);
    // assume that player playes for some time and then logs out. 
    sleep(rand() % 4 + 1);
    printf("%d :( Player palyed and now is log out\n", player_num);
    
    sem_post(&login_status);
    free(player_id);
}

int main(){
    pthread_t threads[THREAD_NUM];

    sem_init(&login_status, 0, 12);
    
    for(int i = 0 ; i < THREAD_NUM; i++){
        int *th_id = (int *)malloc(sizeof(int));
        *th_id = i;
        if(pthread_create(threads+i, NULL, game, th_id) != 0){
            printf("Could not create thread %d\n", i);
            exit(1);
        }
    }

    for(int i = 0 ; i < THREAD_NUM; i++){
        if(pthread_join(threads[i], NULL) != 0 ){
            printf("Could not join the thread number %d\n", i);
            exit(2);
        }
    }

    sem_destroy(&login_status);
}