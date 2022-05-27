#include <stdio.h>
#include <pthread.h>

#define Th_NUM 2

// detaching threads, allowing them to execute independently from each other. 
// Both threads continue without blocking nor synchronizing in any way. Note that when either one ends execution, its resources are released.
// The detached thread cannot be joined back once detached. so detached threads won't be joinable, so you can't wait for that thread to finish
// it's execution, and also every detached thread will clear their resources themselves.

void *called(){
    sleep(1);
    printf("End of the function\n");
}

int main(){
    pthread_t threads[Th_NUM];
    pthread_attr_t lets_detach;
    pthread_attr_init(&lets_detach);
    pthread_attr_setdetachstate(&lets_detach, PTHREAD_CREATE_DETACHED); // can be either PTHREAD_CREATE_JOINABLE  or PTHREAD_CREATE_DETACHED

    for(int i=0; i < Th_NUM; i++){
        if(pthread_create(threads+i, &lets_detach, called, NULL) != 0){
            perror("Could not create threads\n");
        }
/*
        if we detache threads from main thread, they can not be joined again and the pthread_join fucntion give us some error.(Could not join threads: Success)
        here the main and created threads will be detache, then before threads 'called' function finishes the main thread ends, and 
        the process ends and all the created threads will be terminated.

        it is better to use the attributes of a thread and create it as detach thread in first place as show above.
*/
    }


// below we first create the threads in a non-detach state and then changing it to be detach. creating detach threads
// this way can cause some problems, for instance if the thread is created and runs the function and finishs its execution
// even before it comes to the line that we call pthread_detach then we will have some problems.
/*
    for(int i=0; i < Th_NUM; i++){
        if(pthread_create(threads+i, &lets_detach, called, NULL) != 0){
            perror("Could not create threads\n");
        }
        pthread_detach(th[i]);
    }
    for(int i=0; i < Th_NUM; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Could not join threads\n");
        }
    }
*/  
    // if we sleep() for more than 1 second or call pthread_exit on main thread then the 'called' function will print it's message to screen.
    pthread_attr_destroy(&lets_detach);
    /* if we don't call the pthread_exit function by the main thread, when the main function comes to the end, the main thread
        will finish the execution and the process will be terminated. That means that if there be some other threads running at that time
        we don't wait for them to finish and just terminate them. pthread_exit make sure that other deatch threads are finished their opeartions. */ 
    pthread_exit(0);
}