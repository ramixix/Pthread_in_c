#include <stdio.h>
#include <pthread.h>

// showing race case when two threads try to modify a variable.

unsigned long int mail = 0;
void *add_to_main(){
    for(int i =0 ; i < 10000; i++){
        mail +=1;
    }

}

int main(){
    pthread_t t1, t2;

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
    return 0; 
}