#define _XOPEN_SOURCE 600 /* Or higher */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/syscall.h>

#define TH_NUM 8

pthread_mutex_t mutex_fuel;
int fuel = 50;

// 1. as you know, for a thread if we lock mutex twice then that thread is going to make a deadlock
// in our program. (means that thread will wait for ever and never finish execution. unless the main thread kills it.)

// 2. there is a solution to this problem, we actually can use lock the same mutex in the same thread multiple times 
// without getting a deadlock, but we have to set some attributes to the mutex itself.

// 3. As we know we can make a thread detached by passing an attribute in pthread_create function. well with pthread_mutex_init
// we can do the same.

// 5. What is a recursive mutex? A recursive mutex is a mutex that you can lock it more than once.
// but the important point is that if you lock a mutex for a thread 5 times, you need to unlock the same mutex for
// that thread 5 times. otherwise you will get a deadlock. 
// the lock mutex applies to specific thread. So if you lock mutex, lets say for thread number 1 recursivly for 3 times,
// and at end you unlock the same mutex just for 2 times then you will get a deadlock for thread number 1. And if thread number 2
// unlock the same mutex it won't have a effect on the deadlock of thread number 1. 

// 6. mutex recursive adds the mutex counter to number of locks you have. and then it comes back to zero when you 
// unlock for the same amount, and at that time, it is considered unlocked and it can be locked by the other threads.

// 7. Why do we need recursive mutex? if you created a multi thread program that uses recursive functions, then you
// might at some point have to actually lock a certain mutex inside a recursive function and that will be impossible 
// if you use the normal mutex. 

// 8. don't forget that even that you can lock a mutex for many times that you like , the recursive mutex is not the same as
// semaphores. recursive mutex can only be locked and then unlocked by the same thread. you can not lock it by a thread and 
// unlocked it by another thread. (this is possible with semaphors)

void *hey(){
    pthread_mutex_lock(&mutex_fuel);
    pthread_mutex_lock(&mutex_fuel);
    pthread_mutex_lock(&mutex_fuel);
    fuel += 50;
    printf("thread id : %d\n", (pid_t)syscall(SYS_gettid));
    printf("Fuel is incremented, the current value = %d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);
    pthread_mutex_unlock(&mutex_fuel);
    pthread_mutex_unlock(&mutex_fuel);
}

int main(){
    pthread_t threads[TH_NUM];
    // 4. To add an attribute to mutex : First define a variable of type pthread_mutexattr_t;
    // Then initial that created variable with pthread_mutexattr_init;
    // set the type of created variable to recursive using pthread_mutexattr_settype(&mutex_attr_variable, PTHREAD_MUTEX_RECURSIVE);
    // and pass the attribute variable to the pthread_mutex_init() function.
    // at the end don't forget to destroy the attribute variable. (this attribute is actually is going to get destroy when thread is created.)
    pthread_mutexattr_t recuresive_mutex_attribute;
    pthread_mutexattr_init(&recuresive_mutex_attribute);
    pthread_mutexattr_settype(&recuresive_mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex_fuel, &recuresive_mutex_attribute);

    for(int i = 0 ; i < TH_NUM; i++){
        if(pthread_create(threads+i, NULL, hey, NULL) != 0){
            perror("Could not create thread\n");
            exit(0);
        }
    }

    for(int i = 0 ; i < TH_NUM; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Could not join thread\n");
            exit(0);
        }
    }   

    pthread_mutex_destroy(&mutex_fuel);
    pthread_mutexattr_destroy(&recuresive_mutex_attribute);
}