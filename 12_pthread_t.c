#include <stdio.h>
#include <pthread.h>

#include <sys/syscall.h>

#define THREAD_NUM 2

void *routine(){
    // 2.
    // there are also another function called pthread_self(), that return the same number as we print using %ul.(pthread_self =Obtain the identifier of the current thread.)
    // (the type of returned variable is pthread_t).
    pthread_t th = pthread_self();
    printf("%ul\t(read the 2. explaination)\n", th);

    // 4. 
    // in unix based system we have function similar to getpid() (returns process id ) that instead of process id it returns thread id.
    // this function is called gettid(). but for calling this function you need to include <sys/syscall.h>. and then use syscall(SYS_gettid) 
    // to get the thread id as shown below. ( the syscall(SYS_gettid) actually returns a variable of type pid_t ) that is why we typecast it it.
    printf("%d\t the actual thread id (read the 4. explaination)\n", (pid_t)syscall(SYS_gettid));
}

void pthread_t_to_screen(){
    pthread_t thread[THREAD_NUM];

    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_create(thread+i, NULL, routine, NULL) != 0){
            perror("could not create thread\n");
        }

        // 1.
        // pthread_t is defined as unsigned long, so we can print threads to screen using %ul as shown below.
        // these numbers that we print to screen are the numbers that pthread_create, create and pthread_join use
        // to undentify threads and wait for them.
        printf("%ul\t(read the 1. explaination)\n", thread[i]);
    }

    for(int i = 0; i < THREAD_NUM; i++){
        if(pthread_join(thread[i], NULL) != 0){
            perror("could not join thread\n");
        }
    }
}


// 3.
// you should never use the shown ways to print the threads because on the specification of pthread api 
// it says that pthread_t should be treated as opaque(not transparent) data type. meaning that it not always
// guaranteed that it is going to be unsinged long type. it might change for example unsinged long long, a characther
// or even an structure. 
int main(){
    pthread_t_to_screen();
}